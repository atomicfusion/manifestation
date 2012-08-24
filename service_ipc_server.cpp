#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <linux/limits.h>
#include "sockets.h"
#include "common.h"
#include "diy.h"
#include "service_ipc_server.h"

//FIXME: Move the actualy mounting to the daemon; this should be only for the actual IPC.

int manifest_service_ipc_server(manifest_diy_tmp *tmp,int socket) {
	int object;
	int location_fd;
	char *location_path;
	unsigned long flags;
	int err = 0;

	object = recv_fd(socket);
	if (object < 0 && !err) {
		err = errno;
	}
	location_fd = recv_fd(socket);
	if (location_fd < 0 && !err) {
		err = errno;
	}
	if (read(socket,&flags,sizeof(flags)) != sizeof(flags) && !err) {
		err = errno;
	}
	
	location_path = fdpath(location_fd);
	if(!location_path) {
		err = ENOMEM;
	}
	/*int get_fd_path_ret = get_fd_path(location_fd,location_path,sizeof(location_path));
	if(get_fd_path_ret < 0 && !err) {
		err = errno;
	}
	if(get_fd_path_ret >= sizeof(location_path) && !err) {
		//FIXME: Is this even the right error to return? The issue is the buffer being to small. It's not the client's fault...
		err = ENAMETOOLONG;
	}*/
	
	//FIXME: Add error handling for the new read only mount support.
	if (!err && manifest_diy(object,location_path,flags,tmp)) {
		err = errno;
	}
	
	if(write(socket,&err,sizeof(err)) != sizeof(err) && !err) {
		err = errno;
	}
	
	close(object);
	close(location_fd);
	//FIXME: Check for close errors?
	
	free(location_path);
	//FIXME: Check free() errors?
	
	return err;
}
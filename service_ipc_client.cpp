#include <unistd.h>
#include <errno.h>
#include "sockets.h"
#include "service_ipc_client.h"

int manifest_service_ipc_client(int object,int location,unsigned long flags,int socket) {
	int err;
	
	if (send_fd(socket,object) < 0) {
		return -1;
	}
	
	if (send_fd(socket,location) < 0) {
		return -1;
	}
	
	if (write(socket,&flags,sizeof(flags)) < sizeof(flags)) {
		return -1;
	}
	
	if (read(socket,&err,sizeof(err)) < sizeof(err)) {
		return -1;
	}
	
	if(err == 0) {
		return 0;
	}
	
	errno = err;
	return -1;
}
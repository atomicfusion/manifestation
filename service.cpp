#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "sockets.h"
#include "service_ipc_client.h"
#include "manifestation.h"

//FIXME: Fixup error codes. For example: it doesn't make much sense to the user if it returns "No such file or directory." when the socket (that the user doesn't know about) doesn't exist.
//FIXME: Workout the heirarchy for determining socket path.

int manifestat(int object,int dirfd,const char *targetname,unsigned long flags) {
	int target_fd;
	const char *socket_path;
	int socket;
	
	target_fd = openat(dirfd,targetname,O_RDONLY);
	if(target_fd < 0) {
		return -1;
	}
	
	socket_path = getenv("MANIFESTATION_SOCKET");
	if(!socket_path) {
		//FIXME: Is ENOSYS the right error? Should we be returning ENOSYS on other socket errors?
		errno = ENOSYS;
		return -1;
	}
	
	socket = open_local_stream_socket(socket_path);
	if(socket < 0) {
		return -1;
	}
	
	return manifest_service_ipc_client(object,target_fd,flags,socket);
}
int manifest(int object,const char *target,unsigned long flags) {
	return manifestat(object,AT_FDCWD,target,flags);
}
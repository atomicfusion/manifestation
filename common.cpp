#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include "common.h"

//FIXME: Move this to a real library?
//INFO: This path is only valid for the current process. The rationale for using /proc/self/ is that programs should pass the filedescriptor on, rather than passing the path to it; even not using /proc/self/ would not help traverse PID namespaces. Also, getpid() is a syscall, and syscalls are slow (actually I'm not sure if getpid is slow).

char *fdpath(int fd) {
	char *path;
	if (asprintf(&path,"/proc/self/fd/%d",fd) < 0) {
		return NULL;
	}
	return path;
}
//FIXME: Decide to delete this.
/*
int snfdpath(int fd,char *path_buf,size_t path_max) {
	return snprintf(path_buf,path_max,"/proc/self/fd/%d",fd);
}
int asfdpath(int fd,char **path_ptr) {
	return asprintf(path_ptr,"/proc/self/fd/%d",fd);
}
*/
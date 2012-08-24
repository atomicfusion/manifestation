#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mount.h>
#include "manifestation.h"

int main(int argc,char **argv) {
	if(argc < 3 || argc > 4) {
		return 1;
	}
	
	int srcfd = open(argv[1],O_RDONLY);
	if(srcfd < 0) {
		return 2;
	}
	
	unsigned long flags = 0;
	if(argc == 4 && !strcmp(argv[3],"ro")) {
		flags = MS_RDONLY;
	}
	
	if(manifest(srcfd,argv[2],flags)) {
		fprintf(stderr,"Error: %s.\n",strerror(errno));
		return 3;
	}
	return 0;
}
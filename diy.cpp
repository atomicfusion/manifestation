#include <errno.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <stdlib.h>
#include "common.h"
#include "diy.h"

//FIXME: Work out shared subtrees. WrapFS/lofs seems perfect, but WrapFS is outdated and lofs is for Soalris. Could AUFS or UnionFS be abused to do what I want?

int umount_silent_errno;
static void umount_silent(const char *path) {
	//FIXME: Retry on interrupted umount!
	int olderrno;
	olderrno = errno;
	umount_silent_errno = 0;
	if(umount2(path,MNT_DETACH)) {
		umount_silent_errno = errno;
	}
	errno = olderrno;
}
int manifest_diy(int object,const char *target,unsigned long flags,manifest_diy_tmp *tmp) {
	char *object_path;
	
	object_path = fdpath(object);
	if(!object_path) {
		errno = ENOMEM;
		goto error;
	}
	
	if (flags&MS_RDONLY) {
		//INFO: The tmps are paths because of some odd thing where remounting didn't work on /proc/?/fd/? symlinks, even after the immediately previous mount succeded.
		char *tmp_path;
		struct stat target_stat;
		
		//INFO: Using paths rather than FDs tends to be racey. However, I'm not sure that doing open() and then passing the result of get_fd_path() would actually change anything. Also, I'm fairly sure that it's not a security issue, as if there is a race, mount will return ENOTDIR.
		stat(target,&target_stat);
		
		if(S_ISDIR(target_stat.st_mode)) {
			tmp_path = tmp->directory;
		} else {
			tmp_path = tmp->file;
		}
		
		//WARNING: Do not add MS_REC without making sure that umount_noerrno is also recursive.
		if(mount(object_path,tmp_path,0,MS_BIND,0)) {
			goto error;
		}
		if(mount(object_path,tmp_path,0,MS_BIND|MS_REMOUNT|MS_RDONLY,0)) {
			umount_silent(tmp_path);
			goto error;
		}
		if(mount(tmp_path,target,0,MS_MOVE,0)) {
			umount_silent(tmp_path);
			goto error;
		}
		//FIXME: What to do if umount_noerrno fails? The temporary files will be in a bad state. But what can be done about it? Will it ever happen?
	}
	else {
		if(mount(object_path,target,0,MS_BIND,0)) {
			goto error;
		}
	}
	//FIXME: Can free() change errno? Should we be saving errno?
	free(object_path);
	return 0;
	error:
	//FIXME: Howto not duplicate code with free()? Having some sort of "iserr" boolean seems nasty, but so is code duplication; were more than one memory region being freed, it could get to be a mess.
	free(object_path);
	return -1;
}
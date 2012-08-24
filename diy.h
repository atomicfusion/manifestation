#ifndef AFFLUENCE_MANIFESTATION_DIY_H_
#define AFFLUENCE_MANIFESTATION_DIY_H_

extern int umount_silent_errno;

struct manifest_diy_tmp {
	char *file;
	char *directory;
};

int manifest_diy(int object,const char *location,unsigned long flags,manifest_diy_tmp *tmp);

#endif /* AFFLUENCE_MANIFESTATION_DIY_H_ */
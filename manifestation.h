#ifndef AFFLUENCE_MANIFESTATION_MANIFESTATION_H_
#define AFFLUENCE_MANIFESTATION_MANIFESTATION_H_

int manifest(int object,const char *target,unsigned long flags);
int manifestat(int object,int dirfd,const char *targetname,unsigned long flags);

#endif /* AFFLUENCE_MANIFESTATION_MANIFESTATION_H_ */
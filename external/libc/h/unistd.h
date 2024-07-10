#ifndef __UNISTD_H__
#define __UNISTD_H__

#include <sys/types.h>
#include "fake.h"

#ifdef __cplusplus
extern "C" {
#endif



#define	F_OK	0
#define	R_OK	4
#define	W_OK	2
#define	X_OK	1

# define	SEEK_SET	0
# define	SEEK_CUR	1
# define	SEEK_END	2


// typedef int pid_t;


int usleep(useconds_t us);
unsigned int sleep (unsigned int seconds);

pid_t getpid(void);


int access(const char *filenpath, int mode);

ssize_t read(int fd, void * buf, size_t count);
ssize_t write(int fd, const void *buf, size_t nbyte);
int close(int fd);
off_t lseek(int, off_t, int);
int unlink(const char * pathname);

int fsync(int fd);;



//FIXME 
int fileno(FILE *stream); //should in stdio.h

int ftruncate(int fd, off_t  length);

int setenv(const char *envname, const char *envval, int overwrite);

long int random(void); //should in stdlib.h

void srandom(unsigned int seed); //should in stdlib.h

#ifdef __cplusplus
}
#endif



#endif
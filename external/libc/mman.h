#ifndef __MMAN_H__
#define __MMAN_H__

#include "fake.h"

__BEGIN_DECLS
/*
 * Protections are chosen from these bits, or-ed together
 */
#define    PROT_NONE    0x00    /* no permissions */
#define    PROT_READ    0x01    /* pages can be read */
#define    PROT_WRITE    0x02    /* pages can be written */
#define    PROT_EXEC    0x04    /* pages can be executed */

/*
 * Flags contain sharing type and options.
* Sharing types; choose one.
*/
#define    MAP_SHARED    0x0001        /* share changes */
#define    MAP_PRIVATE    0x0002        /* changes are private */
#define    MAP_COPY    MAP_PRIVATE    /* Obsolete */

/*
 * Other flags
 */
#define    MAP_FIXED     0x0010    /* map addr must be exactly as requested */
#define    MAP_RENAME     0x0020    /* Sun: rename private pages to file */
#define    MAP_NORESERVE     0x0040    /* Sun: don't reserve needed swap area */
#define    MAP_INHERIT     0x0080    /* region is retained after exec */
#define    MAP_NOEXTEND     0x0100    /* for MAP_FILE, don't change file size */
#define    MAP_HASSEMAPHORE 0x0200    /* region may contain semaphores */
#define    MAP_STACK     0x0400    /* region grows down, like a stack */
#define    MAP_NOSYNC     0x0800 /* page to but do not sync underlying file */

/*
 * Error return from mmap()
 */
#define MAP_FAILED    ((void *)-1)

/*
 * msync() flags
 */
#define MS_SYNC        0x0000    /* msync synchronously */
#define MS_ASYNC    0x0001    /* return immediately */
#define MS_INVALIDATE    0x0002    /* invalidate all cached data */

/*
 * Mapping type
 */
#define MAP_FILE    0x0000    /* map from file (default) */
#define MAP_ANON    0x1000    /* allocated from memory, swap space */
#define MAP_VPAGETABLE    0x2000    /* manage with virtualized page table */

/*
 * Extended flags
 */
#define MAP_NOCORE     0x00020000 /* dont include these pages in a coredump */

#define MAP_ANONYMOUS	MAP_ANON

void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int mprotect(void *addr, size_t len, int prot);
int msync(void *, size_t, int);
int mlock(const void *, size_t);
int munlock(const void *, size_t);
int munmap(void *, size_t);
int mincore(void*  start, size_t  length, unsigned char*  vec);

__END_DECLS
#endif

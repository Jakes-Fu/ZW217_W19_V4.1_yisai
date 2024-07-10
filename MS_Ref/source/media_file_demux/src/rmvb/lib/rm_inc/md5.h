

#ifndef md5_INCLUDED
#  define md5_INCLUDED


#include "helix_types.h"
/*
 * These should cause all callers and callees of the md5_ 
 * function names to use a unique Helix specific name space
 * of HX_md5_xxx. This will facilitate the inclusion of our 
 * code into code bases that also use the md5_ functions. 
 */

#define md5_init    HX_md5_init
#define md5_append  HX_md5_append
#define md5_finish  HX_md5_finish
#define MD5End      HX_MD5End
#define MD5Data     HX_MD5Data


/*
 * This package supports both compile-time and run-time determination of CPU
 * byte order.  If ARCH_IS_BIG_ENDIAN is defined as 0, the code will be
 * compiled to run only on little-endian CPUs; if ARCH_IS_BIG_ENDIAN is
 * defined as non-zero, the code will be compiled to run only on big-endian
 * CPUs; if ARCH_IS_BIG_ENDIAN is not defined, the code will be compiled to
 * run on either big- or little-endian CPUs, but will run slightly less
 * efficiently on either one than if ARCH_IS_BIG_ENDIAN is defined.
 */

typedef unsigned char md5_byte_t; /* 8-bit byte */
typedef unsigned int md5_word_t; /* 32-bit word */

/* Define the state of the MD5 Algorithm. */
typedef struct md5_state_s {
    md5_word_t count[2];	/* message length in bits, lsw first */
    md5_word_t abcd[4];		/* digest buffer */
    md5_byte_t buf[64];		/* accumulate block */
} md5_state_t;

#ifdef __cplusplus
extern "C" 
{
#endif

/* Initialize the algorithm. */
void md5_init(md5_state_t *pms);

/* Append a string to the message. */
void md5_append(md5_state_t *pms, const md5_byte_t *data, int nbytes);

/* Finish the message and return the digest. */
/* changed order to match expected usage. */
void md5_finish(md5_byte_t digest[16], md5_state_t *pms);


char * MD5End(md5_state_t *ctx, char *p);
char * MD5Data(char *, const unsigned char *, unsigned int);


#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif /* md5_INCLUDED */

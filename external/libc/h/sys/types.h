#ifndef __TYPES_H__
#define __TYPES_H__


#ifdef __cplusplus
extern "C" {
#endif


// clockid_t
#if !defined(__clockid_t_defined) && !defined(_CLOCKID_T_DECLARED)
// typedef	__clockid_t	clockid_t;
typedef	int	clockid_t; // FIX ME
#define	__clockid_t_defined
#define	_CLOCKID_T_DECLARED
#endif

// off_t
#ifndef __off_t_defined
# ifndef __USE_FILE_OFFSET64
typedef long int off_t;
# else
typedef long long int off_t;
# endif
# define __off_t_defined
#endif
#if defined __USE_LARGEFILE64 && !defined __off64_t_defined
typedef long long int off64_t;
# define __off64_t_defined
#endif

// mode_t
// #ifndef __mode_t_defined
// // typedef __mode_t mode_t;
// typedef unsigned int mode_t; // FIXME
// # define __mode_t_defined
// #endif

// ssize_t
typedef long		ssize_t;

#ifndef _SUSECONDS_T_DECLARED
typedef	long	suseconds_t;
#define	_SUSECONDS_T_DECLARED
#endif

typedef	long	useconds_t;


// strdup
char *strdup(const char *s); // FIXME just for compile



#ifdef __cplusplus
}
#endif


#endif
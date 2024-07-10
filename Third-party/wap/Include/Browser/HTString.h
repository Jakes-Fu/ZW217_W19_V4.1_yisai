/*                                                   String handling for libwww
                                         STRINGS
                                             
   Case-independent string comparison and allocations with copies etc
   
 */       
#ifndef HTSTRING_H 
#define HTSTRING_H

#ifndef HTUTILS_H
#include "HTUtils.h" /*Adjusted for DragonBall*/
#endif /* HTUTILS_H */

/*Case-insensitive string comparison

  The usual routines (comp instead of cmp) had some problem.
 */
/*Have been adjusted for DragonBall*/
extern int strcasecomp (const char *a, const char *b);
extern int strncasecomp (const char *a, const char *b, int n);

/*Malloced string manipulation
 */
#define StrAllocCopy(dest, src) HTSACopy (&(dest), src)
extern char * HTSACopy (char **dest, const char *src);

#endif /* HTSTRING_H */





#ifndef _FDL_SYS_H_
#define _FDL_SYS_H_

/* C header file */

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

/* Body of header file */
extern uint32 fdl_GetTickCount(void);
extern void fdl_delay(uint32 ms);

#ifdef __cplusplus  /* Insert end of extern C construct */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */

#endif
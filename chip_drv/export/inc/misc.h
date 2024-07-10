/*********************************************************/
//Description
//r0 holds the address of the semaphore
//r1 holds the ID of the process requesting the lock
/*********************************************************/
#ifndef _MISC_H_
#define _MISC_H_
#ifdef   __cplusplus
extern   "C"
{
#endif
uint32 _semaphore_exchange (uint32 *target, uint32 value);


#ifdef   __cplusplus
}
#endif
#endif


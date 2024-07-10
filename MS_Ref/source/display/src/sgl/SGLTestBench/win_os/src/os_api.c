/******************************************************************************
 ** File Name:      os_api.c                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/08/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces which    *
 **                 independent with RTOS.                                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/08/2002     Xueliang.Wang    Create.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef WIN32
    //#include "tb_hal.h"
	#include "os_import.h"
#else
    #include "time.h"
#endif

#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

// Memory operate(copy or set) max size.
#define SCI_MEM_OP_MAX_SIZE         (100 * 1024)

/**---------------------------------------------------------------------------*
 **                         Data & Function Prototype                         *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 ** MEMORY 
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    The SCI_Mem16Cpy function is used to copy short characters
//                  between buffers, it is mainly used by layer1, when access
//                  share memory, it has to access the memory by 16 bit.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Here count is the total size of the 16 number!
/*****************************************************************************/
PUBLIC void *SCI_Mem16Cpy(      // None
    uint16  *dest_ptr,          // New buffer
    const uint16 *src_ptr,      // Buffer to copy from
    uint16  count               // Number of characters in bytes to copy
    )
{
    uint16  i         = 0;
    uint16  *temp_ptr = dest_ptr;

    SCI_ASSERT(SCI_NULL != dest_ptr);
    SCI_ASSERT(0 == (((uint32) dest_ptr) % 2));
    SCI_ASSERT(0 == (((uint32) src_ptr)  % 2));
    SCI_ASSERT(count < SCI_MEM_OP_MAX_SIZE);

    for (i = 0; i < count/2; i++)
    {
	    *dest_ptr++ = *src_ptr++;
    }

    return temp_ptr;
}

/*****************************************************************************/
//  Description:    The SCI_Mem16Cpy function is used to set buffers to a 
//                  specified character, it is mainly used by layer1, when 
//                  access share memory, it has to access the memory by 16 bit.
//	Global resource dependence: 
//  Author: Richard.Yang
//	Note:           Here count is the total size of the 16 number!
/*****************************************************************************/
PUBLIC void *SCI_Mem16Set(  // None
    uint16  *dest_ptr,      // Pointer to destination
    int16   c,              // Character to set
    uint16  count           // Number of characters
    )
{
// @CR MS00113304
    uint32 *dest_ptr32 = PNULL;
    uint16 *ptail = &dest_ptr[count/2-1];
    uint32 c32 = (c<<16)|c;
    uint32 num32 = 0;
    uint16  *temp_ptr = dest_ptr;

    num32 = count/4;

    if (((uint32)dest_ptr%2)||(count%2))
    {
        SCI_ASSERT(0);
    }
    if ((uint32)dest_ptr%4)
    {
        *dest_ptr = c;
        count-=2;
        dest_ptr32 = (uint32*)(dest_ptr+1);
    }
    else
    {
        dest_ptr32 = (uint32*)dest_ptr;
    }
    if (count%4)
    {
        *ptail = c;
        count-=2;
    }
#ifndef WIN32
    _memset(dest_ptr32, count, c32);    //32λmemset
#else
    //num32 = count/4;
    while(num32)
    {
        *dest_ptr32++=c32;
        num32--;
    }
#endif

    return temp_ptr;
// end @CR MS00113304

/*
	uint16  i         = 0;
    uint16  *temp_ptr = dest_ptr;	

	SCI_ASSERT(SCI_NULL != dest_ptr);
	SCI_ASSERT(0 == (((uint32)dest_ptr)%2));
    SCI_ASSERT(count < SCI_MEM_OP_MAX_SIZE);

	for (i = 0; i < count/2; i++)
	{
		*dest_ptr++ = c;
	}

    return temp_ptr;
*/
}

 
/**---------------------------------------------------------------------------*
 ** TIME
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function gets the system time.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:           In win32, return the number of seconds elapsed since
//                  midnight (00:00:00), January 1, 1970, coordinated 
//                  universal time, according to the system clock.
//                  In ARM, it is not defined.
/*****************************************************************************/
PUBLIC uint32 SCI_GetCurrentTime(void)
{
#ifdef WIN32
    return (uint32)time(NULL);
#else
    // This function is implemented from timer_drv.c
	return OS_GetCurrentTime();
#endif  // End of WIN32
}

/*****************************************************************************/
//  Description:    This function retrieves the number of milliseconds that 
//                  have elapsed since the system was started.
//	Global resource dependence:
//  Author:         Richard.Yang
//	Note:           The elapsed time is stored as a uint32 value. Therefore, 
//                  the time will wrap around to zero if the system is run 
//                  continuously for 49.7 days. 
/*****************************************************************************/
PUBLIC uint32 SCI_GetTickCount(void)    // Number of milliseconds
{
#ifdef WIN32
    return GetTickCount();
#else
    // This function is implemented from timer_drv.c
    return OS_GetTickCount();
#endif  // End of WIN32
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

// End of os_api.c
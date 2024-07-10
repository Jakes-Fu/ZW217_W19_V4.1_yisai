/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_encode.c
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-1  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-1      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#include "std_header.h"
#include "comm_platform.h"
#include "comm_memory.h"
#include "comm_encode.h"

/*==========================================================
 * Function 	: comm_CreateRandkey
 * Author		: sunguochen      
 * Description : ����MD5�����randkey
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
void comm_CreateRandkey(char* timestep, char** randkey_ptr, comm_uint32 *randkey_len)
{
    uint32 len = 0;

    len = strlen(timestep);

    *randkey_ptr = comm_alloc(len + 1);

    memcpy(*randkey_ptr, timestep, len);

    *randkey_len = len;

}


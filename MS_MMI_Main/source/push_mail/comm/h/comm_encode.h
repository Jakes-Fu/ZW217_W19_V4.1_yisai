/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_encode.h
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-4-1  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-4-1      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef _MUSIC_ENCODE_H_
#define _MUSIC_ENCODE_H_

#include "comm_interface.h"

/*==========================================================
 * Function 	: comm_CreateRandkey
 * Author		: sunguochen      
 * Description : 生成MD5编码的randkey
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  4/1/2010 
 ==========================================================*/
void  comm_CreateRandkey(char * timestep, char ** randkey_ptr, comm_uint32 * randkey_len);


#endif


/*****************************************************************************
** File Name:      wsp_header_api.h                                          *
** Author:         kelly.li                                                  *
** Date:           04/24/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the WSP  header decoded interface *
*****************************************************************************/

#ifndef _WSP_HEADER_API_H_
#define _WSP_HEADER_API_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "stdio.h"
#include "sci_types.h"
#include "os_api.h"

 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
      
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/ 
typedef struct _HTTP_PARA
{
    char                *name;
    char                *value;
    struct _HTTP_PARA   *next;
}HTTP_PARA;

typedef struct _HTTP_HEADER
{
    char                    *field;
    char                    *value;
    HTTP_PARA               *parameter;
    struct _HTTP_HEADER     *next;  
}HTTP_HEADER;

/*****************************************************************************/
// 	Description : pack wsp string header to binary 
//	Global resource dependence : none
//  Author: Kelly.li
//	Note: the caller should free the buffer:packed_data_ptr
/*****************************************************************************/
PUBLIC BOOLEAN WSP_headerspack(
                             HTTP_HEADER * header_list_ptr,     //[IN]
                             BOOLEAN is_separate_content_type,  //[IN]
                             uint8** packed_data_ptr,           //[IN][OUT]
                             uint32* data_len_ptr               //[IN][OUT]
                             );

/*****************************************************************************/
// 	Description : unpack binary to wsp string header
//	Global resource dependence : none
//  Author: Kelly.li
//	Note:the caller should free the buffer:HTTP_HEADER *
/*****************************************************************************/
PUBLIC HTTP_HEADER *WSP_HeadersUnpack_Open(unsigned char *data, long data_len);

/*****************************************************************************/
// 	Description : free wsp unpack result buffer
//	Global resource dependence : none
//  Author: Kelly.li
//	Note: free the buffer
/*****************************************************************************/
PUBLIC void WSP_HeadersUnpack_Close(HTTP_HEADER *header);

#ifdef  __cplusplus
}
#endif

#endif

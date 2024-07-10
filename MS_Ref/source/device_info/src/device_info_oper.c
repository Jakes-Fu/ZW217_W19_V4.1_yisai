/******************************************************************************
 ** File Name:     device_info_oper.c                                         *
 ** Author:         gloria.he                                               *
 ** Date:           07/08/2015                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 07/08/2015     gloria      Create.                                   *
** 							   Modify.                                   *
 ******************************************************************************/
#include "diag.h"
#include "device_info.h"
#include "cmddef.h"
	
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description: Make a response packet for read action, using the specified  message header.  
//	Global resource dependence: None
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC  void    DevInfo_ReadRspMsg(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MSG_DEVINFO_READ_RES_T *devinfo_res_ptr,   // Inputed pointer of message header struct.
    uint16 device_len)
{
    uint8* ptr = NULL;
    uint32 len = 0;
    // Check input parameter.    
    SCI_ASSERT(SCI_NULL != devinfo_res_ptr);

   	// Malloc a memory block for response buffer.
    ptr =(uint8 *)SCI_ALLOC_APP(sizeof(MSG_DEVINFO_READ_RES_T));
	SCI_ASSERT(SCI_NULL != ptr);

	
	len = sizeof(MSG_DEVINFO_READ_RES_T) - sizeof(devinfo_res_ptr->dev_info)
        - sizeof(devinfo_res_ptr->reserved);
    devinfo_res_ptr->msg_head.len = len + device_len;
	SCI_MEMCPY((void *)(ptr), (void *)devinfo_res_ptr, len);
    SCI_MEMCPY((void *)(&ptr[len]), (void *)(&devinfo_res_ptr->dev_info), (uint32)device_len);
	*dest_len_ptr = devinfo_res_ptr->msg_head.len;
    *dest_ptr = ptr;
	return;
}
/*****************************************************************************/
// 	Description:  Make a response packet for write action, using the specified  message header.
//	Global resource dependence: None
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC  void    DevInfo_WriteRspMsg(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    MSG_DEVINFO_WRITE_RES_T *devinfo_res_ptr)   // Inputed pointer of message header struct.
{
    // Check input parameter.    
    SCI_ASSERT(SCI_NULL != devinfo_res_ptr);

   	// Malloc a memory block for response buffer.
    *dest_ptr =(uint8 *)SCI_ALLOC_APP(sizeof(MSG_DEVINFO_WRITE_RES_T));
	
	SCI_ASSERT(SCI_NULL != *dest_ptr);

	
	devinfo_res_ptr->msg_head.len=sizeof(MSG_DEVINFO_WRITE_RES_T);

	SCI_MEMCPY((void *)(*dest_ptr), (void *)devinfo_res_ptr, sizeof(MSG_DEVINFO_WRITE_RES_T));
	*dest_len_ptr = sizeof(MSG_DEVINFO_WRITE_RES_T);

	return;
}
/*****************************************************************************/
/*  Description :the device info handle including all the info's get and the product code write 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 HandleDeviceInfoReq(uint8       **dest_ptr,
                           uint16       *dest_len_ptr,
                           const uint8  *src_ptr,
                           uint16        src_len)
{
	
	MSG_HEAD_T* msg_head;
	MSG_DEVINFO_WRITE_RES_T write_res;
	MSG_DEVINFO_READ_RES_T read_res;
	MSG_DEVINFO_T* device_info;
	ERR_DEVOPER_E  ret;
	uint8* str = NULL;
	uint16 devinfo_len=0;


	msg_head = (MSG_HEAD_T *) src_ptr;
	device_info=(MSG_DEVINFO_T*)(src_ptr+sizeof(MSG_HEAD_T));
	
	
    memset(&read_res,0,sizeof(MSG_DEVINFO_READ_RES_T));
    memset(&write_res,0,sizeof(MSG_DEVINFO_WRITE_RES_T));

    memcpy(&(read_res.msg_head),msg_head,sizeof(MSG_HEAD_T));
    memcpy(&(write_res.msg_head),msg_head,sizeof(MSG_HEAD_T));

    read_res.cmd=device_info->cmd;
    read_res.type=device_info->type;
	
	SCI_TRACE_LOW("HandleDeviceInfoReq: (%d/%d/%d/%d)",
                   msg_head->type, msg_head->subtype,device_info->cmd,
                   device_info->type);
	
	
	switch(device_info->cmd)
	{
		
		case DEVICE_INFO_WRITE://write
		
			switch(device_info->type)
			{
				case PRODUCT_CODE_T://product code contained in the NDT conifg in nv, which support read only, return directly
					ret=ERR_NONE;//PRODUCT_CODE_Write(device_info->product_code);
					if(ERR_NONE==ret)//intentinal
					{	
						write_res.status=0;//success
						DEV_INFO_LOG("PRODUCT CODE WRITE PASS");
						
					}
					else
					{
						write_res.status=ret;//failed
						DEV_INFO_LOG("PRODUCT CODE WRITE FAILED");
					}
					break;
				default:
					ret=ERR_NOT_ALLOWED;
					write_res.status=ret;//failed
					break;
				
			}
			DevInfo_WriteRspMsg(dest_ptr, dest_len_ptr, &write_res);
			break;
		case DEVICE_INFO_READ:
			switch(device_info->type)
			{
				case HWID_T:
                    str = HWID_Read();
                    devinfo_len = strlen(str);
					memcpy(read_res.dev_info.hWid,str,devinfo_len);
					read_res.status=0;//success
					DEV_INFO_LOG("HWID READ PASS");
					break;
				case PRODUCT_TYPE_T:
					read_res.status=0;//success
					str = PRODUCT_TYPE_Read();
                    devinfo_len = strlen(str);
					memcpy(read_res.dev_info.product_type,str,devinfo_len);
					DEV_INFO_LOG("PRODUCT TYPE READ PASS");
					break;
				case PRODUCT_CODE_T://wait for nv id
					ret=PRODUCT_CODE_Read(read_res.dev_info.product_code);
                    devinfo_len = TA_CODE_LEN;
					if(ERR_NONE==ret)
					{	
						read_res.status=0;//success
						DEV_INFO_LOG("PRODUCT CODE READ PASS");
						
					}
					else
					{
						read_res.status=ret;//failed
						DEV_INFO_LOG("PRODUCT CODE READ FAILED");
					}
					break;
				case VERISON_T:
                    str = VERSION_Read();
                    devinfo_len = strlen(str);
					memcpy(read_res.dev_info.version,str,devinfo_len);
					read_res.status=0;//success
					DEV_INFO_LOG("VERSION READ PASS");
					break;	
				case PRODUCT_NAME_T:
                    str = PRODUCT_NAME_Read();
                    devinfo_len = strlen(str);
					memcpy(read_res.dev_info.product_name,str,devinfo_len);
					read_res.status=0;//success
					
					DEV_INFO_LOG("PRODUCT NAME READ PASS");
					break;
				default:
					ret=ERR_NOT_ALLOWED;
					read_res.status=ret;//failed
					break;
				
			}
			DevInfo_ReadRspMsg(dest_ptr, dest_len_ptr, &read_res, devinfo_len);
			break;
		default:
			 break;
	}
	
	return 1;
} 



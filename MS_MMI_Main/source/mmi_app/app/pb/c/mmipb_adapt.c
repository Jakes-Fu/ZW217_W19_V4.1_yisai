/****************************************************************************
** File Name:      mmipb_adapt.c                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file deal with standard data format and specific    *
                   physical device data format conversation*
**                 ����ļ���Ҫ�Ǵ���PB�ı�׼���ݸ�ʽ���ض������豸�����ݸ�ʽ
**                 ֮���ת��
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2011        baokun yin         Create
** 
****************************************************************************/
#define _MMIPB_ADAPT_C_  


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_pb_trc.h"
#include "mmipb_adapt.h"
#include "mmivc_common.h"
#include "mmivcard_export.h"
#include "mmifmm_export.h"
//#include "mmiim_multitap.h"
#ifdef SNS_SUPPORT
#include "gui_ucs2b_converter.h"
#include "mmisns_srv.h"
#endif
#include "mmiim.h"
#include "mmipb_app.h"
#include "gui_ucs2b_converter.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPB_ID_INFO_FILE_NAME_LEN   20
#define MMIPB_PHOTO_ID_INFO_SUFFIX  "_Photo.ini"
#define MMIPB_RING_ID_INFO_SUFFIX  "_Ring.ini"
#define MMIPB_ASP_ID_INFO_SUFFIX  "_ASP.ini"
#define MMIPB_GROUP_ID_INFO_SUFFIX  "_GROUP.ini"

#define MMIPB_NAME_SPACERS           1//��֮��ļ����


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL void CopyBcdNumberString(
 uint8       *out_len_ptr,       //OUT:
 uint8       *out_number_ptr,    //OUT:
 uint8       in_len,             //IN:
 uint8       *in_number_ptr      //IN:
 );
#ifdef MMIPB_COVERT_LETTER_SUPPORT
static MMIPB_MAP_KEY_T s_pbkey_table_common[] =
{
    { 'a', {0x00E0, 0x00E1, 0x00E2, 0x00E4, 0x00E5, 0x00E6, 0x0103, 0x0105, 0x03B1,0x00} },
    { 'b', {0x03B2,0x00} },
    { 'c', {0x00E7, 0x0107, 0x010D, 0x03C8,0x00} },
    { 'd', {0x0111, 0x20AB, 0x03B4,0x00} },
    { 'e', {0x011B, 0x00E8, 0x00E9, 0x00EA, 0x0117, 0x0119, 0x03B5,0x00} },
    { 'f', {0x0066, 0x03C6,0x00} },
    { 'g', {0x011F, 0x03B3,0x00} },
    { 'h', {0x03B7,0x00} },
    { 'i', {0x00EC, 0x00ED, 0x00EE, 0x012F, 0x03B9,0x00} },
    { 'j', {0x006A, 0x03BE,0x00} },
    { 'k', {0x03BA,0x00} },
    { 'l', {0x00A7, 0x00F8, 0x0131, 0x0142, 0x03BB,0x00} },
    { 'm', {0x03BC,0x00} },
    { 'n', {0x0144, 0x0148, 0x03BD,0x00} },
    { 'o', {0x00F2, 0x00F3, 0x00F4, 0x00F6, 0x00DF, 0x01A1, 0x0151, 0x03BF,0x00} },
    { 'p', {0x03C0,0x00} },
    { 'r', {0x0159, 0x03C1,0x00} },
    { 's', {0x015B, 0x015F, 0x0161, 0x03C3,0x00} },
    { 't', {0x0163, 0x03C4,0x00} },
    { 'u', {0x00F9, 0x00FA, 0x00FC, 0x016B, 0x016F, 0x0171, 0x0173, 0x01B0, 0x03B8,0x00} },
    { 'v', {0x03C9,0x00} },
    { 'w', {0x03C2,0x00} },
    { 'x', {0x03C7,0x00} },
    { 'y', {0x00FD, 0x03C5,0x00} },
    { 'z', {0x017A, 0x017C, 0x017E, 0x03B6,0x00} },
};
#endif
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/   

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

LOCAL void CopyBcdNumberString(
 uint8       *out_len_ptr,       //OUT:
 uint8       *out_number_ptr,    //OUT:
 uint8       in_len,             //IN:
 uint8       *in_number_ptr      //IN:
 )
{
    //SCI_TRACE_LOW("[MMIPB] CopyBcdNumberString out_len_ptr 0x%x out_number_ptr 0x%x in_number_ptr 0x%x !", out_len_ptr, out_number_ptr, in_number_ptr);

    if((PNULL == out_len_ptr) || (PNULL == out_number_ptr) || (PNULL == in_number_ptr))
    {
        return;
    }    
    if (0 < in_len)
    {
        //uint8   max_len = MMIPB_GetNumberBcdMaxLen();
        
        *out_len_ptr = (in_len >= MMIPB_BCD_NUMBER_MAX_LEN) ? MMIPB_BCD_NUMBER_MAX_LEN : in_len;
        
        
        SCI_MEMCPY(
            out_number_ptr,
            in_number_ptr,
            *out_len_ptr
            );
    }
    else
    {
        *out_len_ptr = 0;
        SCI_MEMSET(
            out_number_ptr,
            0XFF,
            MIN(MMIPB_BCD_NUMBER_MAX_LEN, in_len)
            );
    }
}


/*****************************************************************************
 	Description : �ǿո���Ϊ��ͷ���ַ�
	Global resource dependence : none
       Author: 
	Note:
*****************************************************************************/
PUBLIC wchar MMIPB_GetHeadLetter(const wchar *name_ptr)
{
    uint16 i = 0;
    uint16 name_len = 0;
    wchar  head_letter = 0;

    if(name_ptr != PNULL)
    {
        name_len = MMIAPICOM_Wstrlen(name_ptr);
        for(i = 0; i < name_len; i++)
        {
            //�Էǿո���Ϊ��ͷ
            if(name_ptr[i] != 0x20)
            {
                head_letter = name_ptr[i];
                break;
            }
        }
    }
    return head_letter;
}

/*****************************************************************************/
//  Description : ����group_iddӳ��ɶ�Ӧ�ü�¼�Ĵ������·�������ļ���
//                �������ļ��д�������·����
//  Global resource
//  Author: 
//  Note: out   u16filename_ptr
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_GetGroupRingFileName(uint32 group_id, MMIFILE_DEVICE_E file_device,wchar *u16filename_ptr)
{
    BOOLEAN ret = FALSE;
    uint8  u8filename[MMIPB_ID_INFO_FILE_NAME_LEN +1] = {0};
    wchar  wstr_filename[MMIPB_ID_INFO_FILE_NAME_LEN +1] = {0};
    //FILE_DEV_E_T  file_dev = FS_UDISK;
    uint16        full_path_len = 0;
    uint16        wstr_len  = 0;

    sprintf((char*)u8filename,"%ld%s",group_id,MMIPB_GROUP_ID_INFO_SUFFIX);


    wstr_len = MIN(strlen((char*)u8filename), MMIPB_ID_INFO_FILE_NAME_LEN);/*lint !e666*/
    MMI_STRNTOWSTR(wstr_filename, MMIPB_ID_INFO_FILE_NAME_LEN, u8filename, MMIPB_ID_INFO_FILE_NAME_LEN, wstr_len);
    full_path_len = MMIAPIFMM_CombineFullPathEx((wchar *)u16filename_ptr, 
        MMIFILE_FILE_NAME_MAX_LEN, 
        file_device,
        MMIMULTIM_DIR_PB,
        // TRUE,
        (wchar *)wstr_filename 
        );
    if(full_path_len <= MMIFILE_FILE_NAME_MAX_LEN)
    {
        u16filename_ptr[full_path_len] = 0;
    }
        
        ret = TRUE;

    return ret;
}


/*****************************************************************************/
//  Description : ɾ��group ����id�ļ�  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DeleteGroupRingInfo(uint32 group_id)
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    //uint32 read_size = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    //MMIFILE_DEVICE_E file_dev = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM -1; 
    
    do
    {
        if(MMIPB_GetGroupRingFileName(group_id, dev, u16filename))
        {
            file_ret = MMIAPIFMM_DeleteFile(u16filename, PNULL);
            //SCI_TRACE_LOW("[MMIPB]MMIPB_DeleteContactInfoFile file_ret %d",file_ret);
            if(SFS_NO_ERROR == file_ret)
            {
                ret = TRUE;
            }
        }
    }while(--dev >= MMI_DEVICE_UDISK);
	
    return ret;
}
 

/*****************************************************************************/
//  Description : ����storageidӳ��ɶ�Ӧ�ü�¼�Ĵ������ͼƬ·�������ļ���
//                �������ļ��д������ͼƬ��·����
//  Global resource
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_GetInfoFileNameByStorageId(
                                             uint16 entryid,   
                                             uint16 storageid, //IN:
									         MMIPB_FILE_TYPE file_type,//IN
									         MMIFILE_DEVICE_E file_device,//IN
									         wchar *u16filename_ptr//OUT
									         )
{
    BOOLEAN ret = FALSE;
    uint8  u8filename[MMIPB_ID_INFO_FILE_NAME_LEN +1] = {0};
    wchar  wstr_filename[MMIPB_ID_INFO_FILE_NAME_LEN +1] = {0};
    //FILE_DEV_E_T  file_dev = FS_UDISK;
    uint16        full_path_len = 0;
    uint16        wstr_len  = 0;
    uint32        contact_id = MMIPB_GetContactId(entryid, storageid);
    
    
    if(PNULL != u16filename_ptr)
    {
        if(MMIPB_PHOTO_FILE == file_type)
        {
            //photo id information file name
            sprintf((char*)u8filename,"%ld%s",contact_id,MMIPB_PHOTO_ID_INFO_SUFFIX);
        }
        else if(MMIPB_ASP_FILE == file_type)
        {
            //ring id informatio file name
            sprintf((char*)u8filename,"%ld%s",contact_id,MMIPB_ASP_ID_INFO_SUFFIX);				
        }
		else
		{
            //ring id informatio file name
            sprintf((char*)u8filename,"%ld%s",contact_id,MMIPB_RING_ID_INFO_SUFFIX);

		}


        wstr_len = MIN(strlen((char*)u8filename), MMIPB_ID_INFO_FILE_NAME_LEN);/*lint !e666*/
        MMI_STRNTOWSTR(wstr_filename, MMIPB_ID_INFO_FILE_NAME_LEN, u8filename, MMIPB_ID_INFO_FILE_NAME_LEN, wstr_len);
        full_path_len = MMIAPIFMM_CombineFullPathEx((wchar *)u16filename_ptr, 
            MMIFILE_FILE_NAME_MAX_LEN, 
            file_device,
            MMIMULTIM_DIR_PB,
            // TRUE,
            (wchar *)wstr_filename 
            );
        if(full_path_len <= MMIFILE_FILE_NAME_MAX_LEN)
        {
            u16filename_ptr[full_path_len] = 0;
        }
        
        ret = TRUE;
    }
    return ret;
    
}

/*****************************************************************************/
//  Description : ɾ��ͼƬid��Ϣ������id�ļ�  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DeleteContactInfoFile(
                                           uint16 entry_id,
                                           uint16 storage_id,
                                           MMIPB_FILE_TYPE file_type//IN
                                           )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    //uint32 read_size = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    //MMIFILE_DEVICE_E file_dev = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    
	do
	{
            if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, file_type, dev, u16filename))
    	    {
    		    file_ret = MMIAPIFMM_DeleteFile(u16filename, PNULL);
    		    //SCI_TRACE_LOW("[MMIPB]MMIPB_DeleteContactInfoFile file_ret %d",file_ret);
    		    if(SFS_NO_ERROR == file_ret)
    		    {
    			    ret = TRUE;
    		    }
    	    }
	}while(++dev < MMI_DEVICE_NUM);
	
    return ret;
}

/*****************************************************************************/
//  Description : ��photo_ptr���浽����group_id��entry_idӳ��ɵ�ͼƬid��Ϣ�ļ��� 
//  Global resource
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIPB_SavePhotoInfoByStorageId(
                                              uint16 entry_id,
                                              uint16 storage_id, //IN:
                                              const MMIPB_CUSTOM_PHOTO_T *photo_ptr//IN
                                              )
{
    BOOLEAN ret = FALSE;
    BOOLEAN info_file_exist = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 write_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	MMIFILE_DEVICE_E file_device = MMI_DEVICE_UDISK;
   
    SCI_TRACE_LOW("MMIPB_SavePhotoInfoByStorageId");

    if(PNULL != photo_ptr)
    {
        //find old file and re-write it
        do
        {
            //file_dev = MMIFILE_GetFileDeviceType(dev);

                if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, MMIPB_PHOTO_FILE, file_device, u16filename))
    		    {
                       if(MMIAPIFMM_IsFileExist(u16filename, MMIAPICOM_Wstrlen(u16filename)))
    			       {
    				       info_file_exist = TRUE;
    				       break;
    			       }
    		    }
        }while(++file_device < MMI_DEVICE_NUM);
    
        if(info_file_exist)
        {
            file_device = MMIAPIFMM_GetDeviceTypeByPath(u16filename, MMIFILE_DEVICE_LEN);
            SCI_MEMSET(u16filename,0,sizeof(u16filename));
        }
        else
        {
            file_device = MMIAPIFMM_GetDeviceTypeByPath(photo_ptr->file_name, MMIFILE_DEVICE_LEN);
        }

        if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, MMIPB_PHOTO_FILE, file_device, u16filename))
        {
            {
                //���ļ�
                file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {
                    file_ret = MMIAPIFMM_WriteFile(
                        file_handle, 
                        photo_ptr, 
                        sizeof(MMIPB_CUSTOM_PHOTO_T), 
                        &write_size, 
                        NULL
                        );
                    //SCI_TRACE_LOW:"[MMIPB]MMIPB_SavePhotoIDInfoToFile file_ret %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_397_112_2_18_2_42_59_1,(uint8*)"d",file_ret);
                    if(SFS_ERROR_NONE == file_ret)
                    {
                        ret = TRUE;
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }
                
            }
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : ��ͼƬid��Ϣ�ļ��ж�ȡͼƬ�����buffer
//  Global resource
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_ReadPhotoFile(
                                    uint16 entry_id,//IN
                                    uint16 storage_id, //IN:
                                    MMIPB_CUSTOM_PHOTO_T *photo_ptr//OUT
                                  )
{
    BOOLEAN ret = FALSE;
	BOOLEAN info_file_exist = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 read_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    
    if(PNULL != photo_ptr)
    {
        do
        {
            //file_dev = MMIFILE_GetFileDeviceType(dev);
                if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, MMIPB_PHOTO_FILE, dev, u16filename))
    		    {
                       if(MMIAPIFMM_IsFileExist(u16filename, MMIAPICOM_Wstrlen(u16filename)))
    			       {
    				       info_file_exist = TRUE;
    				       break;
    			       }
    		    }
        }while(++dev < MMI_DEVICE_NUM);
        
        if(info_file_exist)
        {
            file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
            if(SFS_INVALID_HANDLE != file_handle)
            {
                file_ret = MMIAPIFMM_ReadFile(
                    file_handle, 
                    photo_ptr, 
                    sizeof(MMIPB_CUSTOM_PHOTO_T), 
                    &read_size, 
                    NULL
                    );
                //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadPhotoFileName file_ret %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_459_112_2_18_2_42_59_2,(uint8*)"d",file_ret);
                if(SFS_ERROR_NONE == file_ret && read_size > 0 && (photo_ptr->file_name_len > 0) && (photo_ptr->file_name_len <= MMIFILE_FILE_NAME_MAX_LEN))
                {
                    ret = TRUE;
                }
                MMIAPIFMM_CloseFile(file_handle);
            }
        }
    }
    return ret;
}



/*****************************************************************************/
//  Description : ��ring_ptr���浽����group_idӳ��ɵ�����id��Ϣ�ļ��� 
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_SaveGroupRingInfo(
                                      uint32 group_id,
                                      const MMIPB_CUSTOM_RING_T *ring_ptr//IN
                                      )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 write_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	MMIFILE_DEVICE_E file_device = MMI_DEVICE_UDISK;

    if(PNULL != ring_ptr)
    {
        file_device = MMIAPIFMM_GetFirstValidDevice();

        if(MMIPB_GetGroupRingFileName(group_id,file_device, u16filename))
        {
            {
                //���ļ�
                file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {
                    file_ret = MMIAPIFMM_WriteFile(
                        file_handle, 
                        ring_ptr, 
                        sizeof(MMIPB_CUSTOM_RING_T), 
                        &write_size, 
                        NULL
                        );
                    //SCI_TRACE_LOW:"[MMIPB]MMIPB_SaveRingInfoToFile file_ret %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_509_112_2_18_2_42_59_3,(uint8*)"d",file_ret);
                    if(SFS_ERROR_NONE == file_ret)
                    {
                        ret = TRUE;
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }
                
            }
        }
    }
    return ret;
}





/*****************************************************************************/
//  Description : ������id��Ϣ�ļ��ж�ȡ����id��·����Ϣ  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_ReadGroupRingFile(
                                    uint32 group_id,//IN
                                    MMIPB_CUSTOM_RING_T *ring_ptr//OUT
                                   )
{
    BOOLEAN ret = FALSE;
	BOOLEAN info_file_exist = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 read_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    //MMIFILE_DEVICE_E    file_dev = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM-1;
    
    if(PNULL != ring_ptr)
    {
        do
        {

                if(MMIPB_GetGroupRingFileName(group_id, dev, u16filename))
    		    {
                       if(MMIAPIFMM_IsFileExist(u16filename, MMIAPICOM_Wstrlen(u16filename)))
    			       {
    				       info_file_exist = TRUE;
    				       break;
    			       }
    		    }
        }while(--dev >= MMI_DEVICE_UDISK);
        
        if(info_file_exist)
        {
            file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
            
            if(SFS_INVALID_HANDLE != file_handle)
            {
                file_ret =  MMIAPIFMM_ReadFile(
                    file_handle, 
                    ring_ptr, 
                    sizeof(MMIPB_CUSTOM_RING_T), 
                    &read_size, 
                    NULL
                    );
                //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadRingFileName file_ret %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_576_112_2_18_2_42_59_4,(uint8*)"d",file_ret);
                if(SFS_ERROR_NONE == file_ret && (read_size > 0) && (ring_ptr->file_name_len > 0) && (ring_ptr->file_name_len <= MMIFILE_FILE_NAME_MAX_LEN))
                {
                    ret = TRUE;
                }
                MMIAPIFMM_CloseFile(file_handle);
            }
        }
    }
    return ret;
}

/*****************************************************************************/
//  Description : ��ring_ptr���浽����entry_id_id��entry_idӳ��ɵ�����id��Ϣ�ļ��� 
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_SaveRingInfoByStorageId(
                                              uint16 entry_id,
                                              uint16 storage_id, //IN:
                                              const MMIPB_CUSTOM_RING_T *ring_ptr//IN
                                              )
{
    BOOLEAN ret = FALSE;
    BOOLEAN info_file_exist = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 write_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	MMIFILE_DEVICE_E file_device = MMI_DEVICE_UDISK;

    if(PNULL != ring_ptr)
    {
        do
        {

                if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, MMIPB_RING_FILE, file_device, u16filename))
    		    {
                       if(MMIAPIFMM_IsFileExist(u16filename, MMIAPICOM_Wstrlen(u16filename)))
    			       {
    				       info_file_exist = TRUE;
    				       break;
    			       }
    		    }
        }while(++file_device < MMI_DEVICE_NUM);

        if(info_file_exist)
        {
            file_device = MMIAPIFMM_GetDeviceTypeByPath(u16filename, MMIFILE_DEVICE_LEN);
            SCI_MEMSET(u16filename,0,sizeof(u16filename));
        }
        else
        {
            file_device = MMIAPIFMM_GetDeviceTypeByPath(ring_ptr->file_name, MMIFILE_DEVICE_LEN);
        }
        if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, MMIPB_RING_FILE,file_device, u16filename))
        {
            {
                //���ļ�
                file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {
                    file_ret = MMIAPIFMM_WriteFile(
                        file_handle, 
                        ring_ptr, 
                        sizeof(MMIPB_CUSTOM_RING_T), 
                        &write_size, 
                        NULL
                        );
                    //SCI_TRACE_LOW:"[MMIPB]MMIPB_SaveRingInfoToFile file_ret %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_648_112_2_18_2_42_59_5,(uint8*)"d",file_ret);
                    if(SFS_ERROR_NONE == file_ret)
                    {
                        ret = TRUE;
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }
                
            }
        }
    }
    return ret;
}


/*****************************************************************************/
//  Description : ������id��Ϣ�ļ��ж�ȡ����id��·����Ϣ  
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_ReadRingFile(
                                    uint16 entry_id,//IN
                                    uint16 storage_id, //IN:
                                    MMIPB_CUSTOM_RING_T *ring_ptr//OUT
                                   )
{
    BOOLEAN ret = FALSE;
	BOOLEAN info_file_exist = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 read_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    //MMIFILE_DEVICE_E    file_dev = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    
    if(PNULL != ring_ptr)
    {
        do
        {

                if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, MMIPB_RING_FILE, dev, u16filename))
    		    {
                       if(MMIAPIFMM_IsFileExist(u16filename, MMIAPICOM_Wstrlen(u16filename)))
    			       {
    				       info_file_exist = TRUE;
    				       break;
    			       }
    		    }
        }while(++dev < MMI_DEVICE_NUM);
        
        if(info_file_exist)
        {
            file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
            
            if(SFS_INVALID_HANDLE != file_handle)
            {
                file_ret =  MMIAPIFMM_ReadFile(
                    file_handle, 
                    ring_ptr, 
                    sizeof(MMIPB_CUSTOM_RING_T), 
                    &read_size, 
                    NULL
                    );
                //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadRingFileName file_ret %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_713_112_2_18_2_42_59_6,(uint8*)"d",file_ret);
                if(SFS_ERROR_NONE == file_ret && (read_size > 0) && (ring_ptr->file_name_len > 0) && (ring_ptr->file_name_len <= MMIFILE_FILE_NAME_MAX_LEN))
                {
                    ret = TRUE;
                }
                MMIAPIFMM_CloseFile(file_handle);
            }
        }
    }
    return ret;
}
#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : ��photo_ptr���浽����group_id��entry_idӳ��ɵ�ͼƬid��Ϣ�ļ��� 
//  Global resource
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIPB_SaveASPInfoByStorageId(
                                            uint16 entry_id,
                                            uint16 storage_id, //IN:
                                            const MMIPB_CUSTOM_RING_T *asp_ptr//IN
                                            )
{
    BOOLEAN ret = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 write_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
	MMIFILE_DEVICE_E file_device = MMI_DEVICE_UDISK;

    if(PNULL != asp_ptr)
    {
        file_device = MMIAPIFMM_GetDeviceTypeByPath(asp_ptr->file_name, MMIFILE_DEVICE_LEN);

        if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, MMIPB_ASP_FILE, file_device, u16filename))
        {
            {
                //���ļ�
                file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS, NULL, NULL);
                if(SFS_INVALID_HANDLE != file_handle)
                {
                    file_ret = MMIAPIFMM_WriteFile(
                        file_handle, 
                        asp_ptr, 
                        sizeof(MMIPB_CUSTOM_PHOTO_T), 
                        &write_size, 
                        NULL
                        );
                    //SCI_TRACE_LOW:"[MMIPB]MMIPB_SaveASPInfoToFile file_ret %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_761_112_2_18_2_43_0_7,(uint8*)"d",file_ret);
                    if(SFS_ERROR_NONE == file_ret)
                    {
                        ret = TRUE;
                    }
                    MMIAPIFMM_CloseFile(file_handle);
                }
                
            }
        }
    }
    return ret;
}   


/*****************************************************************************/
//  Description : ��ȡ��ϵ�˿������ļ��ģ�������ͼƬ����asp���ݽ�buffer 
//  Global resource
//  Author: 
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_ReadASPFile(
                                uint16 entry_id,//IN
                                uint16 storage_id, //IN:
                                MMIPB_CUSTOM_RING_T *ring_ptr//out
                                )
{
    BOOLEAN ret = FALSE;
    BOOLEAN info_file_exist = FALSE;
    uint16 u16filename[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32 read_size = 0;
    MMIFILE_HANDLE file_handle = 0;
    MMIFILE_ERROR_E file_ret = SFS_NO_ERROR;
    //MMIFILE_DEVICE_E file_dev = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
   
    if(PNULL != ring_ptr)
    {
        do
        {
                if(MMIPB_GetInfoFileNameByStorageId(entry_id, storage_id, MMIPB_ASP_FILE, dev, u16filename))
    		    {
                       if(MMIAPIFMM_IsFileExist(u16filename, MMIAPICOM_Wstrlen(u16filename)))
    			       {
    				       info_file_exist = TRUE;
    				       break;
    			       }
    		    }
        }while(++dev < MMI_DEVICE_NUM);
        
        if(info_file_exist)
        {
            file_handle = MMIAPIFMM_CreateFile(u16filename, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);
            
            if(SFS_INVALID_HANDLE != file_handle)
            {
                file_ret =  MMIAPIFMM_ReadFile(
                    file_handle, 
                    ring_ptr, 
                    sizeof(MMIPB_CUSTOM_RING_T), 
                    &read_size, 
                    NULL
                    );
                //SCI_TRACE_LOW:"[MMIPB]MMIPB_ReadRingFileName file_ret %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_826_112_2_18_2_43_0_8,(uint8*)"d",file_ret);
                if(SFS_ERROR_NONE == file_ret && (read_size > 0) && (ring_ptr->file_name_len > 0) && (ring_ptr->file_name_len <= MMIFILE_FILE_NAME_MAX_LEN))
                {
                    ret = TRUE;
                }
                MMIAPIFMM_CloseFile(file_handle);
            }
        }
    }
    return ret;
}
#endif


/*****************************************************************************/
//  Description : Ϊ���ص�contact�洢��������Ƭ�ȶ�����Ϣ�ṩ�ӿ�   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_PhotoInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_PHOTO_T    *photo_ptr
                                        )
{
    MMIPB_ERROR_E  ret = MMIPB_ERROR_SUCCESS;
    if(PNULL != photo_ptr && photo_ptr->is_valid)
    {
        //delete old info file first
        if(!MMIPB_SavePhotoInfoByStorageId(contact_ptr->contact_id,contact_ptr->storage_id, photo_ptr))
        {
            photo_ptr->is_valid = FALSE;
            return MMIPB_ERROR_NO_SPACE;
        }
        else
        {
            contact_ptr->photo_id_flag = (contact_ptr->photo_id_flag &  MMIPB_PHOTO_ID_CLEAN) + MMIPB_PHOTO_ID_EXIST;
		}
       
    }
    else
    {
        MMIPB_DeleteContactInfoFile(contact_ptr->contact_id,contact_ptr->storage_id, MMIPB_PHOTO_FILE);
    } 
    return ret;
}

/*****************************************************************************/
//  Description : Ϊ���ص�contact�洢��������Ƭ�ȶ�����Ϣ�ṩ�ӿ�   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_RingInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_RING_T    *ring_ptr
                                        )
{
    
    MMIPB_ERROR_E  ret = MMIPB_ERROR_SUCCESS;

    if(PNULL != ring_ptr && ring_ptr->is_valid)
    {
        //delete old info file first
        if(!MMIPB_SaveRingInfoByStorageId(contact_ptr->contact_id,contact_ptr->storage_id, ring_ptr))
        {
            ring_ptr->is_valid = FALSE;
            return MMIPB_ERROR_NO_SPACE;
        }
        else
        {
            contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_RING_ID_CLEAN) + MMIPB_RING_ID_EXIST;
            contact_ptr->ring_id_flag = (contact_ptr->ring_id_flag & MMIPB_FIXED_RING_FLAG_CLEAN);
        }
    }
    else
    {
        MMIPB_DeleteContactInfoFile(contact_ptr->contact_id,contact_ptr->storage_id, MMIPB_RING_FILE);
    }
    return ret;
}


#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : Ϊ���ص�contact�洢��������Ƭ�ȶ�����Ϣ�ṩ�ӿ�   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_AddContact_ASPInfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_RING_T     *asp_ptr
                                        )
{
    MMIPB_ERROR_E  ret = MMIPB_ERROR_SUCCESS;

	if(PNULL != asp_ptr && asp_ptr->is_valid)
	{
		//delete old info file first
		//need check
		if(!MMIPB_SaveASPInfoByStorageId(contact_ptr->contact_id, contact_ptr->storage_id, asp_ptr))
		{
			asp_ptr->is_valid = FALSE;
            return MMIPB_ERROR_NO_SPACE;
		}
		else
		{
			contact_ptr->asp_id_flag = MMIPB_ASP_ID_EXIST;
            
		}
	}
	else
	{
		//need check
		MMIPB_DeleteContactInfoFile(contact_ptr->contact_id, contact_ptr->storage_id, MMIPB_ASP_FILE);
	}    
    return ret;

}
#endif

/*****************************************************************************/
//  Description : Ϊ���ص�contact�洢��������Ƭ�ȶ�����Ϣ�ṩ�ӿ�   
//  Global resource dependence : none
//  Author: maryxiao
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_AddContact_Fileinfo(
                                        MMIPB_CONTACT_T *contact_ptr,
                                        MMIPB_CUSTOM_PHOTO_T    *photo_ptr, //IN:
                                        MMIPB_CUSTOM_RING_T     *ring_ptr, //IN:
                                        MMIPB_CUSTOM_RING_T     *asp_ptr
                                        )
{
	MMIPB_AddContact_PhotoInfo(contact_ptr, photo_ptr);
	MMIPB_AddContact_RingInfo(contact_ptr, ring_ptr);
#ifdef ASP_SUPPORT
	MMIPB_AddContact_ASPInfo(contact_ptr, asp_ptr);
#endif
      return TRUE;
   //  MMIAPIPB_AddContact(contact_ptr);
}

#ifdef SNS_SUPPORT  
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSnsTypeByGroupId(uint32 group_id, uint16 *sns_type_ptr)
{
        
    BOOLEAN ret =TRUE;
    if(PNULL == sns_type_ptr)
    {
        return FALSE;
    }
  
    switch(group_id)
    {
    case MMIPB_GROUP_SNS_SINA:
        *sns_type_ptr = MMI_SNS_SINA;
        break;
    case MMIPB_GROUP_SNS_TIWTTER:
        *sns_type_ptr = MMI_SNS_TIWTTER;
        break;
    case MMIPB_GROUP_SNS_FACEBOOK:
        *sns_type_ptr = MMI_SNS_FACEBOOK;
        break;
    default:
        ret = FALSE;
        break;
    }
   
    return ret;

}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetGroupIdBySnsType(uint16 sns_type)
{

    if(MMI_SNS_SINA == sns_type)
    {
        return MMIPB_GROUP_SNS_SINA;
    }
    else if(MMI_SNS_TIWTTER == sns_type)
    {
        return MMIPB_GROUP_SNS_TIWTTER;
    }
    else if(MMI_SNS_FACEBOOK == sns_type)
    {
        return MMIPB_GROUP_SNS_FACEBOOK;
    }
    else
    {
        return MMIPB_GROUP_SNS_MAX;
    }
}

/*****************************************************************************/
//  Description : תsns��Ϣ�ṹ�嵽contact �ṹ��
//  Global resource dependence : 
//  Author:MARYXIAO
//  Note:
/*****************************************************************************/
PUBLIC void MMIPB_SnsToContact(MMIPB_SNS_CONTACT_T* sns_contact_ptr, MMIPB_CONTACT_T* contact_ptr)
{    
    if(sns_contact_ptr == PNULL || contact_ptr == PNULL)
    {
        return;
    }
    SCI_MEMSET(contact_ptr, 0X00, sizeof(MMIPB_CONTACT_T));
    contact_ptr->storage_id = MMIPB_STORAGE_SNS;
    contact_ptr->contact_id = sns_contact_ptr->uiOffset;
    contact_ptr->union_t.group = 1<<MMIPB_GetGroupIdBySnsType(sns_contact_ptr->search_result.type);
    if(SCI_STRLEN(sns_contact_ptr->search_result.name))
    {
        GUI_UTF8ToWstr(contact_ptr->name.wstr, MMIPB_NAME_MAX_LEN, (uint8 *)sns_contact_ptr->search_result.name, MIN(SCI_STRLEN((uint8 *)sns_contact_ptr->search_result.name), MMIPB_NAME_MAX_LEN));/*lint !e64 !e666*/
        //MMIAPICOM_StrToWstr((uint8 *)sns_contact_ptr->search_result.name, contact_ptr->name.wstr);
        contact_ptr->name.wstr_len = MMIAPICOM_Wstrlen(contact_ptr->name.wstr);
    }
#ifdef MMIPB_MAIL_SUPPORT
    if(SCI_STRLEN(sns_contact_ptr->search_result.email))
    {
        //MMIAPICOM_StrToWstr((uint8 *)sns_contact_ptr->search_result.email, contact_ptr->mail.wstr);
        GUI_UTF8ToWstr(contact_ptr->mail.wstr, MMIPB_MAX_MAIL_LEN, (uint8 *)sns_contact_ptr->search_result.email, MIN(SCI_STRLEN((uint8 *)sns_contact_ptr->search_result.email), MMIPB_MAX_MAIL_LEN));/*lint !e64 !e666*/

        contact_ptr->mail.wstr_len = MMIAPICOM_Wstrlen(contact_ptr->mail.wstr);
    }
#endif
    if(SCI_STRLEN(sns_contact_ptr->search_result.file_path))
    {

        MMIPB_CUSTOM_PHOTO_T *custom_photo_ptr =PNULL;
        custom_photo_ptr = SCI_ALLOC_APPZ(sizeof(MMIPB_CUSTOM_PHOTO_T));
        if(custom_photo_ptr != PNULL)
        {
            //MMIAPICOM_StrToWstr((uint8 *)sns_contact_ptr->search_result.file_path, custom_photo_ptr->file_name);
            GUI_UTF8ToWstr(custom_photo_ptr->file_name, MMIFILE_FULL_PATH_MAX_LEN, (uint8 *)sns_contact_ptr->search_result.file_path, MIN(SCI_STRLEN((uint8 *)sns_contact_ptr->search_result.file_path), MMIFILE_FULL_PATH_MAX_LEN));/*lint !e64 !e666*/

            custom_photo_ptr->is_valid = TRUE;
            custom_photo_ptr->file_name_len = MMIAPICOM_Wstrlen(custom_photo_ptr->file_name);
            MMIPB_SavePhotoInfoByStorageId(contact_ptr->contact_id, contact_ptr->storage_id, custom_photo_ptr);
            contact_ptr->photo_id_flag =  contact_ptr->photo_id_flag+MMIPB_PHOTO_ID_EXIST; 

            SCI_FREE(custom_photo_ptr);
        }

        
    }
#ifndef PB_SUPPORT_LOW_MEMORY    
    if(SCI_STRLEN(sns_contact_ptr->search_result.birth))
    {
        //MMIAPICOM_StrToWstr((uint8 *)sns_contact_ptr->search_result.birth, contact_ptr->birth.wstr);
        GUI_UTF8ToWstr(contact_ptr->birth.wstr, MMIPB_SNS_BIRTH_LEN, (uint8 *)sns_contact_ptr->search_result.birth, MIN(SCI_STRLEN((uint8 *)sns_contact_ptr->search_result.birth), MMIPB_SNS_BIRTH_LEN));/*lint !e64 !e666*/

        contact_ptr->birth.wstr_len = MMIAPICOM_Wstrlen(contact_ptr->birth.wstr);        
    }
#endif    
    if(SCI_STRLEN(sns_contact_ptr->search_result.sign))
    {
        //MMIAPICOM_StrToWstr((uint8 *)sns_contact_ptr->search_result.sign, contact_ptr->sign.wstr);
        GUI_UTF8ToWstr(contact_ptr->sign.wstr, MMIPB_SNS_SIGN_LEN, (uint8 *)sns_contact_ptr->search_result.sign, MIN(SCI_STRLEN((uint8 *)sns_contact_ptr->search_result.sign), MMIPB_SNS_SIGN_LEN)); /*lint !e64 !e666*/

        contact_ptr->sign.wstr_len = MMIAPICOM_Wstrlen(contact_ptr->sign.wstr);   
    }
    SCI_TRACE_LOW("[mmipb] MMIPB_SnsToContact group=%d contact_id=%d name_len=%d",
                        contact_ptr->union_t.group,contact_ptr->contact_id,contact_ptr->name.wstr_len);    
#ifdef MMIPB_MAIL_SUPPORT
    SCI_TRACE_LOW("[mmipb] MMIPB_SnsToContact email_len=%d birth_len=%d sign_len=%d",
                        contact_ptr->mail.wstr_len,contact_ptr->birth.wstr_len,contact_ptr->sign.wstr_len);
#endif
}
#endif 
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��Ϊ�ֻ���ϵ�˵Ĵ洢��ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToPhoneContact(
                                            MMIPB_CONTACT_T 	*contact_ptr,//[IN]
                                            MMIPB_CONTACT_PHONE_T *phone_contact_ptr//[OUT]
                                            )
{
    
    if(PNULL == contact_ptr||PNULL == phone_contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    phone_contact_ptr->entry_id = contact_ptr->contact_id;
    phone_contact_ptr->storage_id = contact_ptr->storage_id;
#ifdef MMIPB_UID_SUPPORT
    phone_contact_ptr->uid = contact_ptr->uid;
#endif
    phone_contact_ptr->group = contact_ptr->union_t.group;
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    phone_contact_ptr->im_type = contact_ptr->im_type;
#endif
    SCI_MEMCPY(phone_contact_ptr->num_type, contact_ptr->num_type, MMIPB_MAX_NV_PHONE_NUM);
    SCI_MEMCPY(phone_contact_ptr->num_type, contact_ptr->num_type, sizeof(contact_ptr->num_type));
    SCI_MEMCPY(phone_contact_ptr->number, contact_ptr->number, sizeof(contact_ptr->number));

    phone_contact_ptr->ring_id_flag = contact_ptr->ring_id_flag;

    phone_contact_ptr->photo_id_flag = contact_ptr->photo_id_flag;

#ifdef ASP_SUPPORT
    phone_contact_ptr->asp_id_flag = contact_ptr->asp_id_flag;
#endif
#ifdef BT_PBAP_SUPPORT
    phone_contact_ptr->is_bt_contact = contact_ptr->is_bt_contact; 
#endif

    //name
    MMIPB_WstrToInternalString(contact_ptr->name.wstr,
        contact_ptr->name.wstr_len,
        phone_contact_ptr->name.alpha,
        &phone_contact_ptr->name.alpha_len,
        &phone_contact_ptr->name.is_ucs2,
            MMIPB_NAME_MAX_LEN
            );
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    //imessager
    MMIPB_WstrToInternalString(contact_ptr->imessager.wstr,
        contact_ptr->imessager.wstr_len,
        phone_contact_ptr->imessager.im_buf ,
        &phone_contact_ptr->imessager.im_len,
        &phone_contact_ptr->imessager.is_ucs2,
        MMIPB_IM_MAX_LEN
            );
#endif
#ifdef MMIPB_MAIL_SUPPORT
    //��ϵ��mail��ַ
    MMIPB_WstrToInternalString(contact_ptr->mail.wstr,
    contact_ptr->mail.wstr_len,
    phone_contact_ptr->mail.mail, 
    &(phone_contact_ptr->mail.mail_len),
    &phone_contact_ptr->mail.is_ucs2,
        MMIPB_MAX_MAIL_LEN
        );
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
//	//����¼
    //URL
    MMIPB_WstrToInternalString(contact_ptr->url.wstr,
        contact_ptr->url.wstr_len,
        phone_contact_ptr->url.url_buf,
        &phone_contact_ptr->url.url_len,
        &phone_contact_ptr->url.is_ucs2,
        MMIPB_URL_MAX_LEN
        );    
    //birth
    MMIPB_WstrToInternalString(contact_ptr->birth.wstr,
        contact_ptr->birth.wstr_len,
        phone_contact_ptr->birth.birth_buf,
        &phone_contact_ptr->birth.birth_len,
        &phone_contact_ptr->birth.is_ucs2,
            MMIPB_SNS_BIRTH_LEN
        );
    MMIPB_WstrToInternalString(contact_ptr->memo.wstr,
        contact_ptr->memo.wstr_len,
       phone_contact_ptr->memo.text_buf,
       &phone_contact_ptr->memo.text_len,
       &phone_contact_ptr->memo.is_ucs2,
        MMIPB_MAX_CUSTOM_TEXT_LEN
        );
    
    if(phone_contact_ptr->memo.text_len>0)	
    {
         phone_contact_ptr->memo.is_valid = TRUE;
    }
#endif
     

    return MMIPB_ERROR_SUCCESS;
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��Ϊ������ϵ�˵Ĵ洢��ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToMostUsedContact(
                                            MMIPB_CONTACT_T 	*contact_ptr,//[IN]
                                            MMIPB_CONTACT_MOSTUSED_T *phone_contact_ptr//[OUT]
                                            )
{
    //SCI_TRACE_LOW("MMIPB_ContactToMostUsedContact contact_ptr 0x%x",contact_ptr);
    
    if(PNULL == contact_ptr ||PNULL == phone_contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    phone_contact_ptr->entry_id = contact_ptr->contact_id;
    phone_contact_ptr->storage_id= contact_ptr->storage_id;
    phone_contact_ptr->used_freq = contact_ptr->union_t.used_freq;
    phone_contact_ptr->number = contact_ptr->number[0];

    return MMIPB_ERROR_SUCCESS;
}
#endif
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��ΪSIM���Ĵ洢��ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToSIMContact(
                                            MMIPB_CONTACT_T 	*contact_ptr,//[IN]
                                            uint16            max_num_len,//[IN]
                                            PHONEBOOK_ENTRY_T *sim_contact_ptr//OUT
                                            )
{
    //SCI_TRACE_LOW("MMIPB_ContactToSIMContact  contact_ptr 0x%x",contact_ptr);
    
    if(PNULL == sim_contact_ptr||PNULL == contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    sim_contact_ptr->entry_id = contact_ptr->contact_id;
    sim_contact_ptr->number_len = contact_ptr->number[0].number_len;

//    sim_contact_ptr->ton = MMIPB_GetNumberTypeFromUint8(contact_ptr->number[0].npi_ton);
//    sim_contact_ptr->npi = MMIPB_GetNumberPlanFromUint8(contact_ptr->number[0].npi_ton);

    MMIPB_BCDNumberToGSMNumber(
        &contact_ptr->number[0],
        &sim_contact_ptr->ton,
        &sim_contact_ptr->npi,
        sim_contact_ptr->number,
        &sim_contact_ptr->number_len
        );
    
    sim_contact_ptr->number_len = MIN(sim_contact_ptr->number_len, max_num_len);
    MMIPB_WstrToGSMAlphabet(contact_ptr->name.wstr, contact_ptr->name.wstr_len,
         sim_contact_ptr->alpha,&sim_contact_ptr->alpha_len, 
         MMIPB_GetContactMaxAlphaLen(MMIPB_GET_CONTACT_TYPE(contact_ptr->storage_id), MMIPB_GetDualSysByStorageId(contact_ptr->storage_id), MMIPB_CONTACT_NAME)
        //PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN
        );

    return MMIPB_ERROR_SUCCESS;
}
#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��ΪUSIM������չ��Ϣ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToUSIMAddtionalContact(
                                            MMIPB_CONTACT_T 	*contact_ptr,//[IN]
                                            PHONEBOOK_ADDITIONAL_ENTRY_T *add_contact_ptr//OUT
                                            )
{
    uint16 index =0;
    uint8  anr_count = 0;
    MMIPB_ERROR_E result = MMIPB_ERROR_SUCCESS;
    
     //SCI_TRACE_LOW:"MMIPB_ContactToUSIMAddtionalContact contact_ptr 0x%x"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_1275_112_2_18_2_43_1_9,(uint8*)"d",contact_ptr);

    if(PNULL == contact_ptr||PNULL == add_contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    
    anr_count = MMIPB_GetUSIMFreeANRCount(MMIPB_GetDualSysByStorageId(contact_ptr->storage_id));
    for(index = 0;index <MIN(anr_count,(MMIPB_MAX_NV_PHONE_NUM -1));index++)
    {
         MMIPB_BCDNumberToANRNumber(
            &contact_ptr->number[index+1],
            contact_ptr->num_type[index+1],
            &add_contact_ptr->anr_number[index].ton, 
            &add_contact_ptr->anr_number[index].npi, 
            add_contact_ptr->anr_number[index].number,
            &add_contact_ptr->anr_number[index].number_len,
            &add_contact_ptr->anr_number[index].aas_id
            );
         if(add_contact_ptr->anr_number[index].aas_id >= anr_count)
         {
             add_contact_ptr->anr_number[index].aas_id = 2;
         }
        
    }
#ifdef MMIPB_MAIL_SUPPORT    
    MMIPB_WstrToGSMAlphabet(contact_ptr->mail.wstr, 
        contact_ptr->mail.wstr_len,
        add_contact_ptr->email_addr.address,
        &add_contact_ptr->email_addr.address_len,
        MN_USIM_EMAIL_STRING_MAX_LEN);
#endif
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_WstrToGSMAlphabet(contact_ptr->alias.wstr,contact_ptr->alias.wstr_len,
        add_contact_ptr->sne_alpha.alpha,
        &add_contact_ptr->sne_alpha.alpha_num,
        MN_PHONEBOOK_ALPHA_STRING_MAX_LEN);
 #endif   
    add_contact_ptr->entry_id = contact_ptr->contact_id;

    return result;
}


/*****************************************************************************/
//  Description : ��USIM������չ��Ϣת��Ϊ��ϵ����Ϣ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_USIMAddtionalContactToContact(
                                            PHONEBOOK_ADDITIONAL_ENTRY_T *add_contact_ptr,//[IN]
                                            MMIPB_CONTACT_T 	*contact_ptr//[OUT]
                                            
                                            )
{
    uint16 index =0;
    
    MMIPB_ERROR_E result = MMIPB_ERROR_SUCCESS;
    
     //SCI_TRACE_LOW:"MMIPB_ContactToUSIMAddtionalContact contact_ptr 0x%x"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_1327_112_2_18_2_43_1_10,(uint8*)"d",contact_ptr);

    if(PNULL == contact_ptr||PNULL == add_contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }

    for(index = 0;index <MIN(MN_USIM_AAS_MAX_NUM, (MMIPB_MAX_NV_PHONE_NUM -1)); index++)/*lint !e506*/
    {
        MMIPB_GSMNumberToBCDNumber(
            &add_contact_ptr->anr_number[index].ton, 
            &add_contact_ptr->anr_number[index].npi, 
            add_contact_ptr->anr_number[index].number,
            add_contact_ptr->anr_number[index].number_len,
            &contact_ptr->number[index+1]
            );

        if(MMIPB_IsValidPhoneNumber(contact_ptr->number[index+1].npi_ton, 
            contact_ptr->number[index+1].number_len))
        {
            //aas idת��Ϊ��������
            contact_ptr->num_type[index+1] = MMIPB_GetNumTypeByAasID(add_contact_ptr->anr_number[index].aas_id);
        }
        else
        {
            //���벻����
            contact_ptr->num_type[index+1] = 0;      
        }        
    }
#ifdef MMIPB_MAIL_SUPPORT    
    MMIPB_GSMAlphabetToWstr(
        add_contact_ptr->email_addr.address,
        add_contact_ptr->email_addr.address_len,
        contact_ptr->mail.wstr, 
        &contact_ptr->mail.wstr_len,
        MN_USIM_EMAIL_STRING_MAX_LEN);
#endif
#ifdef CMCC_VCARD_SUPPORT
    MMIPB_GSMAlphabetToWstr(
        add_contact_ptr->sne_alpha.alpha,
        add_contact_ptr->sne_alpha.alpha_num,
        contact_ptr->alias.wstr,
        &contact_ptr->alias.wstr_len,
        MN_PHONEBOOK_ALPHA_STRING_MAX_LEN
        );
#endif    
    contact_ptr->contact_id = add_contact_ptr->entry_id;

    return result;
}

/*****************************************************************************/
//  Description : �ж��Ƿ�usim�ӿ�
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsUsim(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if(MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)//sim
    {
        if(
            MMI_SIM_TYPE_USIM == MMIPB_GetSimTypeByStorageId(storage_id) 
            && (PHONEBOOK_SIM_STORAGE == MMIPB_GET_CONTACT_TYPE(storage_id))
            )
        {
            ret = TRUE;
        }
    }
    SCI_TRACE_LOW("[baokun] MMIPB_IsUsim storage_id %d ret %d", storage_id, ret);
    return ret;
}
#endif

#ifdef MMI_VCARD_SUPPORT  
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��Ϊvcard���ݸ�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_ContactToVcard(
                                     MMIPB_CONTACT_T *contact_ptr,  //[IN] 
                                     uint32          data_max_size,//[IN]
                                     uint8 *data_ptr,//[OUT]
                                     uint32 *data_len_ptr//[OUT]
                                    )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_SUCCESS;
  
    //SCI_TRACE_LOW("[MMIPB]MMIPB_ContactToVcard contact_ptr 0x%x",contact_ptr);

    if((PNULL == contact_ptr) || (PNULL == data_ptr)||(PNULL == data_len_ptr) )
    {
        return MMIPB_ERROR_ERROR;
    }


    if(MMIAPIVCARD_ContactToVcard(contact_ptr,data_max_size,data_ptr,data_len_ptr))
    {
         result = MMIPB_ERROR_SUCCESS;
    }
    else
    {
        result = MMIPB_ERROR_ERROR;
    }
  
    return result;
}
#endif  
/*****************************************************************************/
//  Description : ���ֻ���ϵ����Ϣ������洢��ʽת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_PhoneContactToContact(
                                            MMIPB_CONTACT_PHONE_T  *phone_contact_ptr,//[IN]
                                            MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                            )
{
    uint16 index = 0;

    //SCI_TRACE_LOW("MMIPB_PhoneContactToContact contact_ptr 0x%x",contact_ptr);
    
    if(PNULL == contact_ptr||PNULL == phone_contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    contact_ptr->contact_id = phone_contact_ptr->entry_id;
    contact_ptr->storage_id = phone_contact_ptr->storage_id;
#ifdef MMIPB_UID_SUPPORT
    contact_ptr->uid = phone_contact_ptr->uid;
#endif 
    contact_ptr->union_t.group = phone_contact_ptr->group;
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    contact_ptr->im_type = phone_contact_ptr->im_type;
#endif
    contact_ptr->is_read_ext = TRUE;
    for(index = 0;index <MMIPB_MAX_NV_PHONE_NUM;index++)
    {             
        contact_ptr->number[index].number_len = phone_contact_ptr->number[index].number_len;
        contact_ptr->number[index].npi_ton = phone_contact_ptr->number[index].npi_ton;
        SCI_MEMCPY(contact_ptr->number[index].number,phone_contact_ptr->number[index].number,
            sizeof(phone_contact_ptr->number[index].number)
            );
        if(MMIPB_IsValidPhoneNumber(phone_contact_ptr->number[index].npi_ton,
            phone_contact_ptr->number[index].number_len))
        {
            contact_ptr->num_type[index] = phone_contact_ptr->num_type[index];
            if(contact_ptr->num_type[index] == MMIPB_INVLID_NUMBER)
            {
                //����������Ч��ȡĬ������
                contact_ptr->num_type[index] = index +1;
            }
        }
        else
        {
            //���벻����
            phone_contact_ptr->num_type[index] = MMIPB_INVLID_NUMBER;
        }
    }
 
    contact_ptr->ring_id_flag = phone_contact_ptr->ring_id_flag;

    contact_ptr->photo_id_flag = phone_contact_ptr->photo_id_flag;

#ifdef ASP_SUPPORT
    contact_ptr->asp_id_flag = phone_contact_ptr->asp_id_flag;
#endif
#ifdef BT_PBAP_SUPPORT
    contact_ptr->is_bt_contact = phone_contact_ptr->is_bt_contact; 
#endif

    //name
    MMIPB_InternalStringToWstr(
        phone_contact_ptr->name.alpha,
        phone_contact_ptr->name.alpha_len,
        phone_contact_ptr->name.is_ucs2,
        MMIPB_NAME_MAX_LEN,
        contact_ptr->name.wstr,
        &(contact_ptr->name.wstr_len)
            );
#ifdef MMIPB_IM_MESSAGE_SUPPORT
    //imessager
    MMIPB_InternalStringToWstr(
       phone_contact_ptr->imessager.im_buf, 
       phone_contact_ptr->imessager.im_len,
       phone_contact_ptr->imessager.is_ucs2,
       MMIPB_IM_MAX_LEN,
       contact_ptr->imessager.wstr,
       &contact_ptr->imessager.wstr_len
        );
#endif
#ifdef MMIPB_MAIL_SUPPORT
    //��ϵ��mail��ַ
    MMIPB_InternalStringToWstr(
        phone_contact_ptr->mail.mail,
        phone_contact_ptr->mail.mail_len,
        phone_contact_ptr->mail.is_ucs2,
        MMIPB_MAX_MAIL_LEN,
        contact_ptr->mail.wstr,
        &contact_ptr->mail.wstr_len
        );
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    //url
    MMIPB_InternalStringToWstr(
       phone_contact_ptr->url.url_buf,
       phone_contact_ptr->url.url_len,
       phone_contact_ptr->url.is_ucs2,
       MMIPB_URL_MAX_LEN,
       contact_ptr->url.wstr,
       &contact_ptr->url.wstr_len
        );
    //birth
    MMIPB_InternalStringToWstr(
       phone_contact_ptr->birth.birth_buf, 
       phone_contact_ptr->birth.birth_len,
       phone_contact_ptr->birth.is_ucs2,
       MMIPB_SNS_BIRTH_LEN,
       contact_ptr->birth.wstr,
       &contact_ptr->birth.wstr_len
        );    
	//����¼
    MMIPB_InternalStringToWstr(
       phone_contact_ptr->memo.text_buf, 
       phone_contact_ptr->memo.text_len,
       phone_contact_ptr->memo.is_ucs2,
       MMIPB_MAX_CUSTOM_TEXT_LEN,
       contact_ptr->memo.wstr,
       &contact_ptr->memo.wstr_len
        );
#endif
    return MMIPB_ERROR_SUCCESS;
}
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ��������ϵ����Ϣ������洢��ʽת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_MostUsedContactToContact(
                                                MMIPB_CONTACT_MOSTUSED_T  *mostused_contact_ptr,//[IN]
                                                MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                                )
{
    //SCI_TRACE_LOW("MMIPB_MostUsedContactToContact contact_ptr 0x%x",contact_ptr);
    
    if(PNULL == mostused_contact_ptr||PNULL == contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }

    contact_ptr->contact_id = mostused_contact_ptr->entry_id;
    contact_ptr->storage_id = mostused_contact_ptr->storage_id;
    contact_ptr->union_t.used_freq = mostused_contact_ptr->used_freq;

    contact_ptr->number[0] = mostused_contact_ptr->number;

    return MMIPB_ERROR_SUCCESS;
}
#endif
/*****************************************************************************/
//  Description : ����ϵ����Ϣ��SIM��������洢��ʽת��Ϊ��ϵ����Ϣ��ͨ�ø�
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_SIMContactToContact(
                                            PHONEBOOK_ENTRY_T   *sim_contact_ptr,//[IN]
                                            MN_DUAL_SYS_E       dual_sys,//[IN]
                                            PHONEBOOK_STORAGE_E storage_type,//[IN]
                                            MMIPB_CONTACT_T 	*contact_ptr//[OUT]
                                            )
{
    //SCI_TRACE_LOW("MMIPB_SIMContactToContact contact_ptr 0x%x",contact_ptr);
    
    if(PNULL == sim_contact_ptr||PNULL == contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    contact_ptr->contact_id = sim_contact_ptr->entry_id;
    //contact_ptr->num_type[0] = MMIPB_ADN_NUMBER;
    contact_ptr->union_t.group = 1 << (dual_sys + PB_GROUP_SIM_1);
    contact_ptr->storage_id = MMIPB_GetStorageID(dual_sys + MMIPB_STORAGE_SIM1, storage_type);
    MMIPB_GSMAlphabetToWstr(sim_contact_ptr->alpha,sim_contact_ptr->alpha_len, 
            contact_ptr->name.wstr,&contact_ptr->name.wstr_len, 
            MMIPB_NAME_MAX_LEN);

    MMIPB_GSMNumberToBCDNumber(&(sim_contact_ptr->ton),
        &(sim_contact_ptr->npi),
        sim_contact_ptr->number,
        sim_contact_ptr->number_len,
        contact_ptr->number
    );
	//judge isnot exist valid number when save sim contact, for Bug 113812
	if (MMIPB_IsExistValidNumber(contact_ptr->number, 1))
	{
		contact_ptr->num_type[0] = MMIPB_ADN_NUMBER;
	}
	else
	{
		contact_ptr->num_type[0] = MMIPB_INVLID_NUMBER;
    }

    return MMIPB_ERROR_SUCCESS;
}
#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : ��vcard����ת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIPB_VcardToContact(
                                    uint8 *data_ptr,//[IN]
                                    uint32  data_len,//[IN]
                                    MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                    )
{
    MMIPB_ERROR_E result = MMIPB_ERROR_SUCCESS;

    VCARD_ERROR_E vcard_ret = VCARD_NO_ERROR;
    //SCI_TRACE_LOW("[MMIPB]MMIPB_VcardToContact contact_ptr 0x%x",contact_ptr);

    if(PNULL == data_ptr||0==data_len||PNULL == contact_ptr)
    {
        return MMIPB_ERROR_ERROR;
    }
    SCI_MEMSET(contact_ptr, 0, sizeof(MMIPB_CONTACT_T));
    //��ʼ����
    vcard_ret = MMIAPIVCARD_VcardDataToContact(data_ptr, data_len, contact_ptr);
    if(VCARD_NO_ERROR != vcard_ret)
    {
        if(VCARD_FORMAT_ERROR == vcard_ret)
        {
            result = MMIPB_ERROR_INVALID_FORMAT;
        }
        else if(VCARD_EMPTY_FILE == vcard_ret)
        {
            result = MMIPB_ERROR_NO_ENTRY;
        }
        else if(VCARD_SPACE_FULL == vcard_ret)
        {
            result = MMIPB_ERROR_NO_SPACE;
        }
        else 
        {
            result = MMIPB_ERROR_ERROR;
        }
    }

    return result;
}

#endif

/*****************************************************************************/
//  Description : ת��ͨ���ַ���ΪNV������ַ�������
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_WstrToInternalString(
                                wchar * wstr_ptr,//[IN]
                                uint32  wstr_len,//[IN]
                                uint8   *alpha_ptr, //[OUT]
                                uint8   *alpha_len_ptr,//[OUT] 
                                BOOLEAN *is_ucs2_ptr,  //[OUT]
                                uint16  max_out_len
                                )
{
    BOOLEAN is_usc2 = FALSE;

    if(wstr_ptr == PNULL || alpha_ptr == PNULL || alpha_len_ptr == PNULL || max_out_len == 0 )
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_WstrToInternalString invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_1761_112_2_18_2_43_2_11,(uint8*)"");
        return FALSE;
    }
    is_usc2 = !MMIAPICOM_IsASCIIString(wstr_ptr, wstr_len); 

    SCI_MEMSET( alpha_ptr, 0X00, max_out_len);
    //set ucs2
    if(is_ucs2_ptr != PNULL)
    {
        *is_ucs2_ptr = is_usc2;
    }
    //set internal string
    if(wstr_len > 0)
    {
        if (is_usc2)
        {
            *alpha_len_ptr = MIN((wstr_len*sizeof(wchar)), max_out_len);
            SCI_MEMCPY(alpha_ptr, (uint8 *)wstr_ptr, *alpha_len_ptr);
            
            
        }
        else
        {
            *alpha_len_ptr = MIN(wstr_len, max_out_len);
            MMI_WSTRNTOSTR(alpha_ptr, max_out_len, wstr_ptr, max_out_len, *alpha_len_ptr);
        }
    }
    else
    {
            *alpha_len_ptr = 0;
    }
    return TRUE;

}

/*****************************************************************************/
//  Description : ת��NV������ַ�������Ϊͨ���ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_InternalStringToWstr(
                                uint8  * alpha_ptr, //[IN]
                                uint8   alpha_len,//[IN] 
                                BOOLEAN is_ucs2,  //[IN]
                                uint16   max_wstr_len,//[IN]
                                wchar * wstr_ptr,//[OUT]
                                uint16  *wstr_len_ptr//[OUT]
                                )
{
    if(alpha_ptr == PNULL ||wstr_ptr == PNULL || wstr_len_ptr == PNULL)
    {
        return FALSE;
    }
    if(is_ucs2)
    {
        *wstr_len_ptr = MIN((alpha_len/sizeof(wchar)), max_wstr_len);
        SCI_MEMCPY((uint8 *)wstr_ptr, alpha_ptr, (*wstr_len_ptr*sizeof(wchar)));
    }
    else
    {
        *wstr_len_ptr = MIN(alpha_len, max_wstr_len);
        MMI_STRNTOWSTR(wstr_ptr, max_wstr_len, alpha_ptr, max_wstr_len, *wstr_len_ptr);
    }
    return TRUE;
}
/*****************************************************************************/
//  Description : ת��ͨ���ַ���ΪgsmЭ��涨���ַ�����ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_WstrToGSMAlphabet(
                                wchar * wstr_ptr,//[IN]
                                uint32  wstr_len,//[IN]
                                uint8  * alpha_ptr, //[OUT]
                                uint8  * alpha_len_ptr,//[OUT] 
                                uint16  max_out_len
                                )
{
    BOOLEAN result  = TRUE;
    BOOLEAN is_usc2 = FALSE;
    
    uint16 input_len = 0;
    //uint16  out_len  = 0;
    //uint16  data_length = 0;
    uint16   output_len = 0;
    uint8   default_data[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN * 2] = {0};
    //uint8   ascii_str[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN] = {0};
    uint8   *in_str_ptr = PNULL;
    
    PHONEBOOK_ALPHABET_TYPE_E alpha_type  = PHONEBOOK_ALPHABET_ASCII;

    //SCI_TRACE_LOW("[MMIPB] MMIPB_WstrToGSMAlphabet wstr_ptr %s",wstr_ptr);
    
    if(( PNULL== wstr_ptr)||( PNULL == alpha_ptr)||(PNULL == alpha_len_ptr))
    {
        return FALSE;
    }
	
    if (0 < wstr_len)
    {
		/*here, we should calculate the max length allowed by gsm encoding to enable
		save contact name as long as possible*/
		input_len = MMIPB_GetMaxLengthAllowedOfGsmAlphabet(wstr_ptr, wstr_len, max_out_len, &is_usc2);

		if(!is_usc2)
		{
			input_len = (uint8)MIN(input_len, max_out_len);
		}
		else
		{
			input_len  = (uint8)(input_len *sizeof(wchar));
			in_str_ptr = (uint8 *)wstr_ptr;
		}
        
        alpha_type = is_usc2 ? PHONEBOOK_ALPHABET_UCS2 : PHONEBOOK_ALPHABET_ASCII;
        
        if (PHONEBOOK_ALPHABET_ASCII == alpha_type)
        {
            // translate the ascii to default according GSM03.38
            input_len = MIN(wstr_len, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);
            output_len = MMIAPICOM_Wchar2default(wstr_ptr,
                default_data,
                input_len
                );
            output_len = MIN(output_len, max_out_len);
            SCI_MEMCPY(alpha_ptr, default_data, sizeof(uint8)*output_len);
            //*alpha_len_ptr = data_length;
            //SCI_MEMCPY(in_str_ptr, default_data, input_len);	
        }
        else 
        {
            if(!MMIAPICOM_EncodeGSMAlphabet(
                alpha_type, 
                input_len, 
                in_str_ptr, 
                &output_len, 
                alpha_ptr,
                max_out_len
                ))
            {
                //SCI_TRACE_LOW:"PHONEBOOK_EncodeAlphabetToSimStandard return flase !"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_1899_112_2_18_2_43_2_12,(uint8*)"");
                output_len = MIN(MMIAPICOM_GetASCIIStringLen(wstr_ptr, wstr_len), max_out_len);/*lint !e666*/
                if(output_len > 0)
                {
                    input_len = MIN(output_len, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);
                    output_len = MMIAPICOM_Wchar2default(wstr_ptr,
                        default_data,
                        input_len
                        );
                    output_len = MIN(output_len, max_out_len);
                    SCI_MEMCPY(alpha_ptr, default_data, sizeof(uint8)*output_len);
                }
            }
        }
    }
    else
    {
        is_usc2 = FALSE;
        result = FALSE;
    }
    *alpha_len_ptr = (uint8)MIN(output_len, MMIPB_NAME_MAX_LEN);
	return result;
}


/*****************************************************************************/
//  Description : gsmЭ��涨���ַ�����ʽת��Ϊͨ���ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_GSMAlphabetToWstr(
                                uint8  * alpha_ptr,//[IN] 
                                uint8   alpha_len,//[IN]
                                wchar * wstr_ptr,//[OUT]
                                uint16  *wstr_len_ptr,//[OUT]
                                uint16  max_out_len
                                )
{
    BOOLEAN res = TRUE;
    //BOOLEAN is_ucs2 =FALSE;
    uint16   output_len = 0;
    uint8   output_str[MMIPB_NAME_MAX_LEN + 1] = {0};
    //uint8   ascii_data[MMIPB_NAME_MAX_LEN + 1] = {0};
    //uint16  data_length = 0;
    PHONEBOOK_ALPHABET_TYPE_E   alpha_type = PHONEBOOK_ALPHABET_ASCII;
    
    if(PNULL == alpha_ptr || (PNULL == wstr_ptr))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_GSMAlphabetToWstr invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_1939_112_2_18_2_43_2_13,(uint8*)"");
        return FALSE;
    } 
    
    if (0 < alpha_len)
    {
        if (!MMIAPICOM_DecodeGSMAlphabet(
            &alpha_type, 
            (uint8)MIN(alpha_len, MMIPB_NAME_MAX_LEN),
            alpha_ptr,
			MMIPB_NAME_MAX_LEN,
            &output_len,
            output_str
            ))
        {
            //SCI_TRACE_LOW:"MMIPB_GSMAlphabetToWstr decode alphabet with sim standard is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_1954_112_2_18_2_43_2_14,(uint8*)"");
        }
        res = TRUE;

    }

    if(PHONEBOOK_ALPHABET_ASCII == alpha_type)
    {
        *wstr_len_ptr = MMIAPICOM_Default2Wchar(output_str, wstr_ptr, MIN(output_len, max_out_len));
    }
    else
    {
        *wstr_len_ptr = (MIN(output_len, max_out_len))/sizeof(wchar);
        SCI_MEMCPY((uint8 *)wstr_ptr, output_str, (*wstr_len_ptr*sizeof(wchar)));

    }    
    return res;
}

/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��ΪgsmЭ��涨�ĸ�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_BCDNumberToGSMNumber(
                                    MMIPB_BCD_NUMBER_T  *bcd_number_ptr, 
                                    MN_NUMBER_TYPE_E    *ton_ptr,             
                                    MN_NUMBER_PLAN_E    *npi_ptr,      
                                    uint8               *gsm_number_ptr,
                                    uint8               *gsm_number_len_ptr
                                    )
{
    //SCI_TRACE_LOW("[MMIPB] MMIPB_BCDNumberToGSMNumber");

    if(PNULL == bcd_number_ptr || (PNULL == gsm_number_ptr) || (PNULL == gsm_number_len_ptr) || (PNULL == npi_ptr))
    {
        return FALSE;
    }     
        
    *ton_ptr = MMIPB_GetNumberTypeFromUint8(bcd_number_ptr->npi_ton);
    *npi_ptr = MMIPB_GetNumberPlanFromUint8(bcd_number_ptr->npi_ton);
    
    if (0 < bcd_number_ptr->number_len)
    {
        //uint8  max_len = MMIPB_GetNumberBcdMaxLen();
        
        *gsm_number_len_ptr = (bcd_number_ptr->number_len>= MMIPB_BCD_NUMBER_MAX_LEN) ? MMIPB_BCD_NUMBER_MAX_LEN : bcd_number_ptr->number_len;
        
        
        SCI_MEMCPY(
            gsm_number_ptr,
            bcd_number_ptr->number,
            *gsm_number_len_ptr
            );
    }
    else
    {
        *gsm_number_len_ptr = 0;
        SCI_MEMSET(
            gsm_number_ptr,
            0XFF,
            MMIPB_BCD_NUMBER_MAX_LEN
            );
    }
	return TRUE;
}

/*****************************************************************************/
//  Description : gsmЭ��涨�ĺ����ʽת��Ϊbcd��ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_GSMNumberToBCDNumber(
                                MN_NUMBER_TYPE_E    *ton_ptr,             
                                MN_NUMBER_PLAN_E    *npi_ptr,      
                                uint8               *gsm_number_ptr,
                                uint8                gsm_number_len,
                                MMIPB_BCD_NUMBER_T  *bcd_number_ptr
                                )
{
    //SCI_TRACE_LOW("[MMIPB] MMIPB_GSMNumberToBCDNumber");
    
    if(PNULL == bcd_number_ptr||PNULL == gsm_number_ptr)
    {
        return FALSE;
    }
    bcd_number_ptr->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(/*lint !e661*/
        *ton_ptr,
        *npi_ptr
        );

    CopyBcdNumberString(
        &bcd_number_ptr->number_len,
        bcd_number_ptr->number,/*lint !e661*/
        gsm_number_len,
        gsm_number_ptr
        );
	return TRUE;
}
#ifdef CMCC_VCARD_SUPPORT
/*****************************************************************************/
//  Description : ��aas idת��ΪUSIM�ĺ�������
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetNumTypeByAasID(uint8 aas_id)
{
    uint8 num_type = MMIPB_UNKNOWN_NUMBER;
     switch(aas_id)
     {
     case 1:
    	 num_type = MMIPB_OFFICE_NUMBER;
    	 break;
     case 2:
    	  num_type = MMIPB_HOME_NUMBER;
    	 break;
     case 3:
    	 num_type =MMIPB_FAX_NUMBER;
    	 break;
     default:
    	 break;
     }
    return num_type;
}

/*****************************************************************************/
//  Description : ͨ��number_typeȡ��aas_id
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetAasIDByNumType(uint8 num_type)
//-CR125125
{
     uint8 aas_id = 0;

     switch(num_type)
     {
     case MMIPB_OFFICE_NUMBER:
    	 aas_id = 1;
    	 break;
     case MMIPB_HOME_NUMBER:

    	 aas_id = 2;
    	 break;
     case MMIPB_FAX_NUMBER:
    	 aas_id = 3;
    	 break;
	 case MMIPB_UNKNOWN_NUMBER:
		 aas_id = 0;
		 break;
     default:
    	 break;
     }
     return aas_id;
}

/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��ΪgsmЭ��涨�ĸ�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN	MMIPB_BCDNumberToANRNumber (
                                    MMIPB_BCD_NUMBER_T  *bcd_number_ptr,//in
                                    uint8       number_type,//in
                                    MN_NUMBER_TYPE_E    *ton_ptr,             
                                    MN_NUMBER_PLAN_E    *npi_ptr,      
                                    uint8               *gsm_number_ptr,
                                    uint8               *gsm_number_len_ptr,
                                    uint8               *aas_id_ptr
                                    )
{
    if(PNULL == bcd_number_ptr ||(PNULL == ton_ptr)  || (PNULL == npi_ptr)||
        (PNULL == gsm_number_len_ptr) ||(PNULL == gsm_number_ptr)|| 
        (PNULL == aas_id_ptr))
    {
        return FALSE;
    }  
    //SCI_TRACE_LOW("[MMIPB] MMIPB_BCDNumberToANRNumber");
 
    MMIPB_BCDNumberToGSMNumber(bcd_number_ptr,ton_ptr,npi_ptr, 
            gsm_number_ptr, gsm_number_len_ptr);
    
    *aas_id_ptr = MMIPB_GetAasIDByNumType(number_type);   

    return TRUE;
}
#endif
/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��Ϊutf8�ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
uint8	MMIPB_BCDNumberToUTF8(
                                    MMIPB_BCD_NUMBER_T  *bcd_number_ptr,//in   
                                    uint8               str_max_len,//IN
                                    uint8               *str_ptr//OUT                                    
                                    )
{
    uint8 str_len = 0;
    MN_NUMBER_TYPE_E number_type = 0;
    if(PNULL == bcd_number_ptr ||(PNULL == str_ptr))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_BCDNumberToUTF8 invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_2166_112_2_18_2_43_2_15,(uint8*)"");
        return 0;
    }  
    number_type = MMIPB_GetNumberTypeFromUint8(bcd_number_ptr->npi_ton);
    str_len = MMIAPICOM_GenDispNumber(
                                        number_type,//MMIPB_GetNumberTypeFromUint8(bcd_number_ptr->npi_ton),
                                        bcd_number_ptr->number_len,
                                        bcd_number_ptr->number,
                                        str_ptr,
                                        str_max_len
                                        );       

    return str_len;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
uint16	MMIPB_GetBCDNumberStrlen(
                              MMIPB_BCD_NUMBER_T  *bcd_number_ptr//in                                       
                              )
{
    uint16 len = 0;
    
    if(bcd_number_ptr != PNULL)
    {
        if( MN_NUM_TYPE_INTERNATIONAL == MMIPB_GetNumberTypeFromUint8(bcd_number_ptr->npi_ton) )
        {
            len++;
        }
        if(bcd_number_ptr->number_len > 1)
        {
            len += (bcd_number_ptr->number_len <<1);
            if((bcd_number_ptr->number[bcd_number_ptr->number_len -1] & 0xF0) == 0xF0)
            {
                len--;
            }
        }
    }
    return len;
}

/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��Ϊwchar�ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
uint16	MMIPB_BCDNumberToWstr(
                              MMIPB_BCD_NUMBER_T  *bcd_number_ptr,//in   
                              uint16               wstr_max_len,//IN
                              wchar                *wstr_ptr//OUT                                    
                              )
{
    uint8 *utfstr_ptr = PNULL;
    uint16 len = 0;
    if(bcd_number_ptr != PNULL && wstr_ptr != PNULL && wstr_max_len > 0)
    {
        utfstr_ptr = SCI_ALLOCA(wstr_max_len*sizeof(uint8));
        
        if(utfstr_ptr != PNULL)
        {
            SCI_MEMSET(utfstr_ptr, 0x00, wstr_max_len*sizeof(uint8));
            len = MMIPB_BCDNumberToUTF8(bcd_number_ptr, wstr_max_len, utfstr_ptr);
            MMI_STRNTOWSTR(wstr_ptr, wstr_max_len, utfstr_ptr, len, len); 
            SCI_FREE(utfstr_ptr);
        }        
    }
    return len;
}
/*****************************************************************************/
//  Description : ��UTF8�ַ���ת��Ϊbcd��ʽ�ĵ绰����
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
void	MMIPB_UTF8ToBCDNumber(
                                uint8               *str_ptr,//IN 
                                MMIPB_BCD_NUMBER_T  *bcd_number_ptr//OUT                                                                          
                                )
{
    MMI_PARTY_NUMBER_T   party_num;

    if(PNULL == bcd_number_ptr ||(PNULL == str_ptr))
    {
        //SCI_TRACE_LOW:"[MMIPB] MMIPB_BCDNumberToString invalid param !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_2220_112_2_18_2_43_2_16,(uint8*)"");
        return;
    }  

    if (MMIAPICOM_GenPartyNumber((uint8 *)(str_ptr), strlen((char *)str_ptr), &party_num))
    {
        bcd_number_ptr->npi_ton = (uint8)((party_num.number_type << 0X04) | MMIPB_DEFAULT_NPI);
        bcd_number_ptr->number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
        SCI_MEMCPY(bcd_number_ptr->number, party_num.bcd_num, bcd_number_ptr->number_len);
    }
}

/*****************************************************************************
 	Description : get name's letters and it's header letters
	Global resource dependence : none
       Author: 
	Note:
*****************************************************************************/
BOOLEAN MMIPB_GetNameLetters(
	const MMIPB_NAME_T 	*name_ptr,//IN
	MMIPB_NAME_STRING_TYPE_T1  letter_type,//IN
	BOOLEAN                     is_transfer_letter,//IN
	MMIPB_NAME_LETTERS_T *letters_str//OUT
)
{
    wchar  letter_buf[40] = {0};
    wchar  head_letter_buf[2] = {0};
    uint16 i = 0;
    uint16 full_letter_len = 0;
    BOOLEAN pre_is_punct = TRUE;
    BOOLEAN is_punct = FALSE;
    
    if(PNULL == name_ptr || letters_str == PNULL)
    {
        //SCI_TRACE_LOW:"MMIPB_GetNameLetters name_ptr %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_2254_112_2_18_2_43_3_17,(uint8*)"d", name_ptr);
        return FALSE;
    }
    for(i = 0; i < MIN(name_ptr->wstr_len, MMIPB_NAME_MAX_LEN); i++)
    {
        //get text letters
        SCI_MEMSET(letter_buf, 0x00, sizeof(wchar)*40);
        MMIAPIIM_GetTextLetters(name_ptr->wstr[i], letter_buf);
        full_letter_len = MMIAPICOM_Wstrlen(letter_buf);
        if(letter_type &MMIPB_NAME_FULL_CHAR
          ||(letter_type &MMIPB_NAME_FULL_CHAR_SPACER)   
         )
        {
            if(letters_str->full_letters_len + full_letter_len <= MMIPB_NAME_MAX_LEN*6)
            {
                //set full letter
                if(is_transfer_letter)
                {
                    MMIAPICOM_Wstrncpy(&letters_str->full_letters[letters_str->full_letters_len], letter_buf, full_letter_len);
                }
                else
                {
                    MMIIM_GetWcharKeyValueByTable(letter_buf, &letters_str->full_letters[letters_str->full_letters_len], FALSE);/*lint !e718 !e746*/
                    
                }
            }
            letters_str->full_letters_len += full_letter_len;
        }
        if(letter_type &MMIPB_NAME_FULL_CHAR_SPACER 
            && full_letter_len > 1
            &&letters_str->full_letters_len + 1 <= MMIPB_NAME_MAX_LEN*6)
        {
             //full letter with space in china words             
             letters_str->full_letters[letters_str->full_letters_len] = MMIPB_NAME_SPACERS;
             letters_str->full_letters_len++;
        }
        if(letter_type &MMIPB_NAME_HEAD_CHAR)
        {
            if(*letter_buf == 0x20 || MMIAPIIM_IsPunct(*letter_buf))
            {
                is_punct = TRUE;
            }
            else
            {
                is_punct = FALSE;
            }            
            //set head letter
            if((!is_punct && pre_is_punct) //ǰһ���Ƿ��ţ�������ַ������������ַ�            
               ||full_letter_len > 1)//����Ƿ����֣���һ���ַ�����Ϊ���ַ�
            {
                head_letter_buf[0] = MMIPB_GetHeadLetter(letter_buf);                    
                
                if(head_letter_buf[0] > 0)
                {
                    if(is_transfer_letter)
                    {
                        letters_str->head_letters[letters_str->head_letters_len] = head_letter_buf[0];
                    }
                    else
                    {
                        MMIIM_GetWcharKeyValueByTable(head_letter_buf, &letters_str->head_letters[letters_str->head_letters_len], FALSE);                            
                    }
                    letters_str->head_letters_len++;                    
                }
            }
            pre_is_punct = is_punct;            
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : ��õ绰���Ĵ�����Ϣ
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_STORAGE_INFO_T MMIPB_GetContactStorageInfo //RETURN: �绰����Ϣ
                                           (
                                            uint16 storage_id
                                            )
{
    MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByStorageId(storage_id);
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_SIM_STORAGE;

    if(MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)
    {
        storage = MMIPB_GET_CONTACT_TYPE(storage_id);
    }
    else
    {
        storage = PHONEBOOK_NV_STORAGE;
    }
    return MMIPB_GetContactStorage(dual_sys, storage);

}

/*****************************************************************************/
//  Description : ��õ绰���Ŀռ���Ϣ
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetContactStorageSpace //RETURN: �ռ���Ϣ
                                           (
                                            uint16 storage_id
                                            )
{
    MN_DUAL_SYS_E dual_sys = MMIPB_GetDualSysByStorageId(storage_id);
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_SIM_STORAGE;

    if(MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)
    {
        storage = MMIPB_GET_CONTACT_TYPE(storage_id);
    }
    else
    {
        storage = PHONEBOOK_NV_STORAGE;
    }
    return MMIPB_GetStorageSpace(dual_sys, storage);

}

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : �ж��Ƿ�����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsMostUsedContact(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if(MMIPB_GET_STORAGE(storage_id) == MMIPB_STORAGE_PHONE)
    {
        if(MMIPB_GET_CONTACT_TYPE(storage_id) == MMIPB_MOSTUSED_PHONE_CONTACT)
        {
            ret = TRUE;
        }

    }
    return ret;
}


/*****************************************************************************/
//  Description : is the favouite contact
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsFavouriteContact(uint32 group)// group, not group_id
{
   BOOLEAN result = FALSE;
   
   if((group& (1<<PB_GROUP_FAVROUTE)) > 0)
   {
       result = TRUE;
   }
   return result;
}
#endif

#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description : �ж��Ƿ�T����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsTCardContact(uint16 storage_id,
                                     uint16 contact_id)
{
    BOOLEAN ret = FALSE;
    
    if(MMIPB_IsPhoneContact(storage_id)
        &&contact_id > MMINV_MAX_PHONEBOOK_RECORDS)
    {
        ret = TRUE;
    }
    return ret;
}
#endif

/*****************************************************************************/
//  Description : �ж��Ƿ��ֻ���ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsPhoneContact(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if(MMIPB_STORAGE_PHONE == MMIPB_GET_STORAGE(storage_id))
    {
        if(MMIPB_GET_CONTACT_TYPE(storage_id) == MMIPB_NORMAL_PHONE_CONTACT)
        {
            ret = TRUE;
        }

    }
    return ret;
}
#ifdef SNS_SUPPORT 
/*****************************************************************************/
//  Description : �ж��Ƿ�sns��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSNSContact(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if(MMIPB_GET_STORAGE(storage_id) == MMIPB_STORAGE_SNS)
    {
        ret = TRUE;        
    }
    return ret;
}
#endif

#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : �ж��Ƿ�vcard
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsVcardContact(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if(MMIPB_GET_STORAGE(storage_id) == MMIPB_STORAGE_FLASH)
    {
        ret = TRUE;        
    }
    return ret;
}
#endif

/*****************************************************************************/
//  Description : �ж��Ƿ�sim����¼
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:��������ϵ�ˣ��������룬�̶����ţ���������
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSimRecord(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if(MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)
    {
        ret = TRUE;        
    }
    return ret;
}

/*****************************************************************************/
//  Description : �ж��Ƿ�sim��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSIMContact(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if((MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)
        && (MMIPB_GET_CONTACT_TYPE(storage_id) ==PHONEBOOK_SIM_STORAGE)
        )
    {
        ret = TRUE;        
    }
    return ret;
}

#ifdef FDN_SUPPORT
/*****************************************************************************/
//  Description : �ж��Ƿ�fdn��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsFdnContact(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if((MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)
        && (MMIPB_GET_CONTACT_TYPE(storage_id) ==PHONEBOOK_FDN_STORAGE)
        )
    {
        ret = TRUE;        
    }
    return ret;
}
#endif

#if defined (MMIPB_SIMDN_SUPPORT) 
/*****************************************************************************/
//  Description : �ж��Ƿ�msisdn��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsMsisdnContact(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if((MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)
        && (MMIPB_GET_CONTACT_TYPE(storage_id) ==PHONEBOOK_MSISDN_STORAGE)
        )
    {
        ret = TRUE;        
    }
    return ret;
}
#endif

/*****************************************************************************/
//  Description : �ж��Ƿ�SDN��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIPB_IsSDNContact(uint16 storage_id)
{
    BOOLEAN ret = FALSE;

    if((MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)
        && (MMIPB_GET_CONTACT_TYPE(storage_id) ==PHONEBOOK_SDN_STORAGE)
        )
    {
        ret = TRUE;        
    }
    return ret;
}

/*****************************************************************************/
//  Description :ͨ��groupȡ�ø�group�������������� group id
//  Global resource dependence : 
//  Author:baokun
//  Note: return valid group count
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetAllGroupIDs(uint32 group, 
                                  uint8  group_id[],
                                  uint8  max_group_count
                                  )
{
    uint8 valid_group_count = 0;
    uint32 i =0;

    if(group_id != PNULL)
    {
        for(i =0; i < MMIPB_MAX_GROUP_NUM && (valid_group_count < max_group_count); i++)
        {
            if((1 << i) > group) /*lint !e574*/
            {
                break;
            }
            if((1 << i) & group)
            {
                group_id[valid_group_count] = i;
                valid_group_count++;
            }                
        }
    }
    return valid_group_count;
}
/*****************************************************************************/
//  Description : ����storage id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16	MMIPB_GetStorageID(uint8 storage_type, uint8 contact_type)
{
    return (contact_type << 8) + storage_type;

}

/*****************************************************************************/
//  Description : ����storage id
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPB_GetStorageIDByGroupID(uint8 group_id)
{
    uint16 storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
    if(MMIPB_IsInSIMGroup(group_id))
    {
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SIM1 + group_id - PB_GROUP_SIM_1, PHONEBOOK_SIM_STORAGE);
    }
    else if(MMIPB_IsInNvGroup(group_id))
    {
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_PHONE, MMIPB_NORMAL_PHONE_CONTACT);
    }
#ifdef SNS_SUPPORT    
    else if(MMIPB_IsSNSGroup(group_id))
    {
        storage_id = MMIPB_GetStorageID(MMIPB_STORAGE_SNS, MMIPB_NORMAL_PHONE_CONTACT);
    }
#endif
    return storage_id;
}

/*****************************************************************************/
//  Description : ��ȡdual sys
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPB_GetDualSysByStorageId(uint16 storage_id)
{
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;

    if(MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_SIM1)
    {
          dual_sys = MIN((MMIPB_GET_STORAGE(storage_id) - MMIPB_STORAGE_SIM1), MMI_DUAL_SYS_MAX); 
    }
    
    return dual_sys;
}

/*****************************************************************************/
//  Description : ��ȡdual sys
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPB_GetDualSysByGroupId(uint8 group_id)
{
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;

    if(group_id >= PB_GROUP_SIM_1 && group_id < PB_GROUP_SIM_1 + MMI_DUAL_SYS_MAX)
    {
          dual_sys = group_id -PB_GROUP_SIM_1;
    }
    
    return dual_sys;
}
/*****************************************************************************/
//  Description : ����entry id ��storage id�� ����ܱ�ʾһ��contact��Ψһid
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPB_GetContactId(uint16 entry_id, uint16 storage_id)
{
    uint32 id = 0;

    id = ((storage_id<<16) & 0xFFFF0000) | entry_id;
    //SCI_TRACE_LOW("[MMIPB] entry_id %, storage_id %d id %ld", entry_id, storage_id, id);
    return id;
}

/*****************************************************************************/
//  Description : ��ȡSIM TYPE
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMI_SIM_TYPE_E MMIPB_GetSimTypeByStorageId(uint16 storage_id)
{
    MMI_SIM_TYPE_E ret_type = MMI_SIM_TYPE_SIM;
    MN_DUAL_SYS_E dual_type = MN_DUAL_SYS_1;
    
    dual_type = MMIPB_GetDualSysByStorageId(storage_id);
    if(MMI_DUAL_SYS_MAX != dual_type)
    {
        //get sim type
#ifdef CMCC_VCARD_SUPPORT
        if(MNSIM_Is_UsimEx(dual_type))/*lint !e718 !e746 !e628*/
        {
            ret_type = MMI_SIM_TYPE_USIM;
        }
#endif
        //SCI_TRACE_LOW("[MMIPB]MMIPB_GetSimTypeByGroupID group_id %d ret_type %d", group_id, ret_type);
    }
    return ret_type;
}

/*****************************************************************************/
//  Description : Get First Valid Number index from number array number_t_ptr
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetFirstValidNumberIndex(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 num_count)
{
     uint8 loop_count = MIN(MMIPB_MAX_NV_PHONE_NUM, num_count);
     uint8  i = 0;
     uint8  result = MMIPB_INDEX_OF_MAX; 

     for(i = 0; i < loop_count; i++)
     {
         if(MMIPB_IsValidPhoneNumber(number_t_ptr[i].npi_ton, number_t_ptr[i].number_len))
         {
             result = i;
             break;
         }
     }
     return result;
}
/*****************************************************************************/
//  Description : ������ת��Ϊ�ַ���
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPB_ConvertNameToString
(
 MMI_STRING_T    	*str_ptr,   //OUT:
 const MMIPB_NAME_T	*name_ptr   //IN:
 )
{
    if((PNULL == name_ptr) || (PNULL == str_ptr) || (PNULL == str_ptr->wstr_ptr) )
    {
        //SCI_TRACE_LOW:"[MMIPB]MMIPB_ConvertNameToString name_ptr 0x%x str_ptr 0x%x!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPB_ADAPT_2665_112_2_18_2_43_3_18,(uint8*)"dd", name_ptr, str_ptr);
        return;
    }     
    //str_ptr->is_ucs2 = name_ptr->is_ucs2;
    str_ptr->wstr_len  = MIN((uint16)name_ptr->wstr_len, MMIPB_MAX_STRING_LEN);
    MMIAPICOM_Wstrncpy(str_ptr->wstr_ptr, name_ptr->wstr, str_ptr->wstr_len);
}


/*****************************************************************************/
// 	Description : judge whether the group id is in sim group
//	Global resource dependence : none
//  Author: MARYXIAO
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsInSIMGroupByStorage(uint16 storage_id)
{
   return (MMIPB_GET_STORAGE(storage_id)>= MMIPB_STORAGE_SIM1);
}


/*****************************************************************************/
// 	Description : judge whether the group id is in sim group
//	Global resource dependence : none
//  Author: MARYXIAO
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsInNvGroupByStorage(uint16 storage_id)
{
     return (MMIPB_GET_STORAGE(storage_id)== MMIPB_STORAGE_PHONE);
}

/*****************************************************************************/
// 	Description : judge whether the group id is in sim group
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsInSIMGroup(uint8 group_id)
{
    if((group_id >= PB_GROUP_SIM_1) && (group_id < PB_GROUP_SIM_1+(MMI_DUAL_SYS_MAX-MN_DUAL_SYS_1)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : judge whether the group id is in sim group
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsInNvGroup(uint8 group_id)
{
    if(MMIPB_IsInSIMGroup(group_id))
    {
        return FALSE;
    }
    else if(PB_GROUP_ALL == group_id)
    {
        return FALSE;
    }
    else if(group_id >=  PB_GROUP_HOME && group_id <= PB_GROUP_SCHOOLMATE)
    {
        return TRUE;
    }
#ifdef MMIPB_GROUP_MANAGE_SUPPORT
    else if(group_id >= MMIPB_SYSTEM_GROUP_NUM && group_id < MMIPB_SYSTEM_GROUP_NUM +MMIPB_USER_GROUP_NUM)
    {
        return TRUE;
    }
#endif
    else
    {
        return FALSE;
    }

}

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
// 	Description : judge whether the group id is user group
//	Global resource dependence : none
//  Author: starmer.sun
//	Note: �ж��Ƿ����Զ������
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsUserGroup(uint8 group_id)
{
    if(group_id >= MMIPB_SYSTEM_GROUP_NUM && group_id < MMIPB_SYSTEM_GROUP_NUM +MMIPB_USER_GROUP_NUM)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
#endif
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Discription: is sns group
//  Global resource dependence: none 
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsSNSGroup(uint16 group_id)
{
    return ((group_id>=MMIPB_GROUP_SNS_SINA) &&(group_id<MMIPB_GROUP_SNS_MAX));
}

#endif
/*****************************************************************************/
//  Discription: is SIM group
//  Global resource dependence: none 
//  Author: maryxiao
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsSIMGroup(uint16 group_id)
{
    return ((group_id>=PB_GROUP_SIM_1) &&(group_id<PB_GROUP_HOME));
}
/*****************************************************************************/
//  Description : �ж����Ƿ����
//  Global resource dependence : s_mmipb_group_info
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsExistedGroup  //RETURN: TRUE������; FALSE��������
(
 uint8   group_id    //IN: Ⱥ���id��
 )
{
    if(group_id >= MMIPB_MAX_GROUP_NUM)
    {
        return FALSE;
    }
    return TRUE;
}

#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : �ж��Ƿ��ǹ̶�����
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsFixedGroup  //RETURN: TRUE������; FALSE��������
(
    uint8   group_id    //IN: Ⱥ���id��
)
{
    if(group_id == PB_GROUP_MOSTUSED
      ||group_id == PB_GROUP_FAVROUTE
      )
    {
        return TRUE;
    }
    return FALSE;
}
#endif
/*****************************************************************************/
//  Description : �ж��Ƿ��Ǵ洢�豸�Ƿ��ǲ������û���ӵļ�¼���豸
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_IsFixedStorage  //RETURN: TRUE������; FALSE��������
(
    uint16   storage_id    //IN:
)
{
    BOOLEAN result = FALSE;

    if(MMIPB_GET_STORAGE(storage_id) == MMIPB_STORAGE_FLASH)
    {
        result = TRUE;
    }
    else if(
#ifdef SNS_SUPPORT
        MMIPB_IsSNSContact(storage_id)||
#else
        MMIPB_STORAGE_SNS == storage_id ||
#endif
        MMIPB_IsSDNContact(storage_id))
    {
        result = TRUE;
    }
    else if(MMIPB_GET_STORAGE(storage_id) >= MMIPB_STORAGE_MAX)
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : ��BYTE�л��NUMBER TYPE
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

PUBLIC MN_NUMBER_TYPE_E MMIPB_GetNumberTypeFromUint8(uint8   npi_ton )
{
    return ((MN_NUMBER_TYPE_E)(npi_ton >> 4));
}


/*****************************************************************************/
//  Description : �жϵ绰�����Ƿ���Ч
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

BOOLEAN MMIPB_IsValidPhoneNumber
(
 uint8 npi_ton,
 uint8 number_length
 )
{
    BOOLEAN result = FALSE;
    
    if ((0 < number_length)
        || (MN_NUM_TYPE_INTERNATIONAL == MMIPB_GetNumberTypeFromUint8(npi_ton)))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : �жϺ����������Ƿ������Ч����
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsExistValidNumber(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count)
{
     uint32 loop_count = MIN(MMIPB_MAX_NV_PHONE_NUM, max_count);
     uint32  i = 0;
     BOOLEAN result = FALSE; 

     for(i = 0; i < loop_count; i++)
     {
         if(MMIPB_IsValidPhoneNumber(number_t_ptr[i].npi_ton, number_t_ptr[i].number_len))
         {
             result = TRUE;
             break;
         }
     }

     return result;
}

/*****************************************************************************/
//  Description : check the contact valid
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsContactValid(MMIPB_CONTACT_T *contact_ptr)
{
   BOOLEAN result = FALSE;
   if(PNULL != contact_ptr)
   {
       if(contact_ptr->name.wstr_len>0)
       {
           result = TRUE;
       }
       else if(MMIPB_IsPhoneContact(contact_ptr->storage_id) )
       {
           //Nv entry must exist valid number or exist mail
           if(MMIPB_IsExistValidNumber(contact_ptr->number, MMIPB_MAX_NV_PHONE_NUM))
           {
               result = TRUE;
           }
       }
       else
       {
           //sim entry must exist ADN number
           if(MMIPB_IsExistValidNumber(contact_ptr->number, 1))
           {
               result = TRUE;
           }
           
       }
   }
   return result;
}


/*****************************************************************************/
//  Description : change file error to pb error
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_GetErrorType(MMIFILE_ERROR_E file_error)
{
    MMIPB_ERROR_E ret = MMIPB_ERROR_ERROR;
    if(SFS_ERROR_NONE == file_error)
    {
        ret = MMIPB_ERROR_SUCCESS;
    }
    else if(SFS_ERROR_NOT_EXIST == file_error)
    {
        ret = MMIPB_ERROR_FILE_NOT_EXIST;
    }
    else if(SFS_ERROR_NO_SPACE == file_error)
    {
        ret = MMIPB_ERROR_NO_SPACE;
    }
    return ret;
}
#ifdef MMIPB_COVERT_LETTER_SUPPORT
/*****************************************************************************/
//  Description : covert polish etc a, to english a
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC wchar ConvertLetters(wchar head_letter)
{
    int i= 0;
    int j = 0;
    BOOLEAN ret = FALSE;


    if(head_letter <MMIPB_MIN_MAP_KEY || head_letter >MMIPB_MAX_MAP_KEY)//���Ӽ�ֵ�뿼���޸������С��mapֵ
    {
        return head_letter;
    }

    for(i=0; i< sizeof(s_pbkey_table_common)/sizeof(s_pbkey_table_common[0]); i++)
    {
        for(j=0; j<MMIPB_MAP_KEY; j++)
        {
            if(s_pbkey_table_common[i].raw_key_code[j] ==0x00)
            {
                break;
            }
            if(s_pbkey_table_common[i].raw_key_code[j] == head_letter)
            {
                return s_pbkey_table_common[i].map_key;

            }
        }
    }
    return head_letter;
}
#endif
/*****************************************************************************/
//  Description : get the actual max length of contact name(save to sim) allowed
//	under pb-encoding 
//  Global resource dependence : arvin.zhang
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIPB_GetMaxLengthAllowedOfGsmAlphabet(
													wchar * wstr_ptr,//[IN]
                                					uint32  wstr_len,//[IN]
                                					uint16  max_out_len,//[IN]
                                					BOOLEAN *is_ucs2//[IN/OUT]
                                					)
{
	uint16 	max_len_allowed = wstr_len;
	BOOLEAN is_ucs2_string 	= TRUE;
	BOOLEAN is_over			= FALSE;
	wchar   max_ucs2 		= 0;
    wchar   min_ucs2 		= 0;
	MMITHEME_PB_ENCODE_TYPE_E   encode_type = MMITHEME_PB_ENCODE_NONE;

	if(( PNULL== wstr_ptr)||( PNULL == is_ucs2))
    {
        return FALSE;
    }

	SCI_TRACE_LOW("[arvinzhang]MMIPB_GetMaxLengthAllowedOfGsmAlphabet enter: wstr_len = %d,max_out_len = %d",wstr_len,max_out_len);
	
	//first,we should cut out the input wstr according to max_out_len
	if(max_len_allowed > max_out_len)
	{
		max_len_allowed = max_out_len;
	}
	
	//second,to calc the max length of input string on its corresponding encode type
	do
	{
		MMITHEME_GetMaxAndMinUcs2 (wstr_ptr, max_len_allowed, &max_ucs2, &min_ucs2);
		encode_type = MMITHEME_GetEncodeTypeForPb(max_ucs2, min_ucs2);

		switch(encode_type)
		{
			//no ucs2,pure ascii alphabet
			case MMITHEME_PB_ENCODE_NONE:
				{
					if(max_len_allowed <= max_out_len)
					{
						is_ucs2_string = FALSE;
						is_over = TRUE;
					}
					break;
				}
			//80, shouldn't be more than (max_out_len>>1)
			case MMITHEME_PB_ENCODE_80:
				{
					if(max_len_allowed <= (max_out_len>>1))
					{
						is_over = TRUE;
					}
					break;	
				}
			//81,the head length contains three chars
    		case MMITHEME_PB_ENCODE_81:
				{
					if(max_len_allowed <= (max_out_len - MMICOM_81STR_OFFSET))
					{
						is_over = TRUE;
					}
					break;
				}
			//82,the head length contains four chars
    		case MMITHEME_PB_ENCODE_82:
				{
					if(max_len_allowed <= (max_out_len - MMICOM_82STR_OFFSET))
					{
						is_over = TRUE;
					}
					break;
				}
			//fault-tolerant, return max_out_len>>1 directly
			default:
				{
					max_len_allowed = max_out_len>>1;
					is_over = TRUE;
					break;
				}
		}
		if(!is_over)
		{
			max_len_allowed-=1;
		}
	}while(!is_over && (max_len_allowed > max_out_len>>1));
	
	*is_ucs2 = is_ucs2_string;
	SCI_TRACE_LOW("[arvinzhang]MMIPB_GetMaxLengthAllowedOfGsmAlphabet exit: is_ucs2 = %d,max_len_allowed = %d",is_ucs2,max_len_allowed);
	
	return max_len_allowed;
}

#ifdef BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description:�ж��Ƿ�ΪBT Contact
//  Parameter: entry_id, storage_id
//  Global resource dependence :                                
//  Author: Yanyan.an
//  Return : [TRUE] is bt contact; [FALSE] is not bt contact
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsBTContact(uint16 entry_id, uint16 storage_id)
{
    return MMIPB_IsBTContactListNode(entry_id, storage_id);
}
#endif
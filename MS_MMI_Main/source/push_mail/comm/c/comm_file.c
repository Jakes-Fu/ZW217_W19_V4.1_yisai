/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: comm_file.c
 *	Author		: sunguochen    
 *	Description	: 
 *	Vesion		: 1.0        
 *	Date			: 2010-3-31  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-3-31      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/

#include "std_header.h"

#include "comm_platform.h"
#include "comm_file.h"

/*==========================================================
 * Function 	: comm_GetMusicRoot
 * Author		: sunguochen      
 * Description : ������ظ�Ŀ¼
                      ����п�������sd���ϣ�������ڱ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
comm_boolean comm_DownloadRootDir(char *path)
{
    MMIFILE_DEVICE_E dev       = MMI_DEVICE_SDCARD;
    BOOLEAN          result    = FALSE;
    wchar            *path_ptr = PNULL;
    
    wchar wroot[10]  = {0};
    
    if(!path)
    {
        return FALSE;
    }

    //�ж��Ƿ���TF��
   dev = MMIAPIFMM_GetFirstValidSD();
    if(dev >= MMI_DEVICE_NUM)
    {
        dev = MMI_DEVICE_UDISK;
    }

    path_ptr = MMIFILE_GetRootDir(MMIFILE_GetFileDeviceType(dev))
    if(PNULL == path_ptr)
    {
        return FALSE;
    }
    
    MMIAPICOM_Wstrcpy(wroot, path_ptr);
    WstrToStr(path_ptr, MMIAPICOM_Wstrlen(path_ptr), path);
    
    return TRUE;
}


/*==========================================================
 * Function 	: comm_DownloadFreeSpace
 * Author		: sunguochen      
 * Description : ���ʣ��洢�ռ�
                      ������������ȡ��ʣ��ռ�
                      �����ȡ����ʣ��ռ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  3/31/2010 
 ==========================================================*/
comm_uint32 comm_DownloadFreeSpace(void)
{
    uint32           free_space_high = 0;
    uint32           free_space_low  = 0;    
    MMIFILE_DEVICE_E dev             = MMI_DEVICE_SDCARD;
    SFS_ERROR_E      sfs_ret         = SFS_NO_ERROR;
    BOOLEAN          result         = FALSE;

    //�ж��Ƿ���TF��
    if(SFS_NO_ERROR != MMIAPISD_CheckSDSpace(COMM_MEMORY_REMAIN_SIZE, &dev))
    {
        dev = MMI_DEVICE_UDISK;
    }
    
    //get dev free memory
    MMIAPIFMM_GetDeviceFreeSpace(
                                MMIAPIFMM_GetDevicePath(dev), 
                                MMIAPIFMM_GetDevicePathLen(dev), 
                                &free_space_high, 
                                &free_space_low
                                );
 
    //leave 2M free space 
    if(free_space_low > COMM_MEMORY_REMAIN_SIZE)
    {
        free_space_low -= COMM_MEMORY_REMAIN_SIZE;
    }
    else
    {
        free_space_low = 0;
    }

    return free_space_low;

}





#include "sci_types.h"
#include "os_api.h"
#include "dal_lcd.h"
#include "sfs.h"

/*****************************************************************************/
//  Description:    The function used to initlize the lcd. If have dual lcd,
//                  sub lcd will also  be initlized by this function.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:           If have many lcd, this function will init all lcd.
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Init (void)
{	
	return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:    The function is used to close lcd when power off.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:           When power off , call this function. If have many lcd,
//                  this function will close all lcd.
/*****************************************************************************/
PUBLIC void LCD_Close (void)
{
}
/*****************************************************************************/
//  Description:    Get the lcd basic information..
//  Global resource dependence:
//  Author:         Jim.zhang
//  Note:
/*****************************************************************************/
PUBLIC  ERR_LCD_E  LCD_GetInfo (
                                    LCD_ID_E  lcd_id,           //id of lcd to operate
                                    LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
                                   )
{
	return ERR_LCD_NONE;	
}                                   

/*****************************************************************************/
//  Description:    Enter/Exit sleep mode to control power consume.
//  Global resource dependence:
//  Author:         Jim.zhang
//  Return:         error code.
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E   LCD_EnterSleep ( 
                                         LCD_ID_E  lcd_id,     
                                         BOOLEAN is_sleep   //SCI_TRUE:enter sleep: SCI_FALSE:exit sleep mode
                                        )
{
	return ERR_LCD_NONE;	
}                                         
                                       

/*****************************************************************************/
//  Description: Update LCD Timing
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_UpdateTiming (uint32 ahb_clk)
{
		return ERR_LCD_NONE;
}


/*****************************************************************************/
//  Description:  display text on the main lcd.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_DisplayText (uint16 x, uint16 y, char *text, uint16 len)
{
}

/*****************************************************************************/
//  Description:  clear the whole lcd to one color.
//    Global resource dependence:
//  Author: Jim.zhang
//    Note:
/*****************************************************************************/
PUBLIC void LCD_Clear (
                            LCD_ID_E lcd_id,     //id of lcd to operate.
                            uint32 color        //color to fill the whole lcd.
                         )
{
}                         

/*******************************************************************************/
//  Description:  Get LCD number int the system.
//  Global resource dependence:
//  Author: Younger.Yang
//  Note:
/*******************************************************************************/
PUBLIC uint32 LCD_GetUsedNum (void)
{
	return 0;
}

/*****************************************************************************/
//Description:    get the main lcd 's important parameter for Digital cammera
//Global resource dependence:
//Author:         Zhemin.lin
/*****************************************************************************/
PUBLIC void LCD_PointerSaveInAssertResetMode (void)
{
}
#if 0
typedef void (*FAT_ASSERTDUMP) (char *, uint32);


PUBLIC void FAT_SystemAssert(char* const buf,FAT_ASSERTDUMP dumpChars)
{
	return;
}


/*****************************************************************************/
//  Description:   Get disk status
//  Author: Jason.wu
//  Param
//       dev_name :The name of device ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: The disk has be mounted
//       Other: The disk has not be mounted
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_GetDeviceStatus(const uint16* dev_name)
{
	return SFS_ERROR_SYSTEM;	
}

/*****************************************************************************/
//  Description:   Delete a file
//  Author: Jason.wu
//  Param
//       name :The name of path and file ,must be unicode string
//       overlapped_ptr:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_DeleteFile(const uint16 *name,SFS_OVERLAPPED_T    *overlapped_ptr)
{
	return SFS_ERROR_SYSTEM;	
}

/*****************************************************************************/
//  Description:   Delete a directory
//  Author: Jason.wu
//  Param
//       path:The name of path you want to create ,must be unicode string
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_DeleteDirectory(const uint16 *path)
{
	return SFS_ERROR_SYSTEM;	
}

// Added in SFS2.0 Jason.wu
SFS_ERROR_E SFS_DeleteDirectoryAsyn(
	SFS_IN const uint16* pathName,
	SFS_IN const uint16* fileName,
	SFS_IN SFS_OVERLAPPED_T *overlapped_ptr
)
{
	return SFS_ERROR_SYSTEM;	
}


/*****************************************************************************/
//  Description:   Get disk free space
//  Author: Jason.wu
//  Param
//       device_name :The name of device ,must be unicode string
//       high_word:the high 32 bit value of free space
//       low_word:the low 32 bit value of free space
//  Return:
//       SFS_ERROR_NONE: Success ,the value of *high_word and *low_word is valid
//       Other: Fail ,the value of *high_word and *low_word is invalid
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_GetDeviceFreeSpace(const uint16 *device_name,uint32 *high_word,uint32 *low_word)
{
	return SFS_ERROR_SYSTEM;	
}

/*****************************************************************************/
//  Description:   Get disk used space
//  Author: Jason.wu
//  Param
//       device_name :The name of device ,must be unicode string
//       high_word:the high 32 bit value of used space
//       low_word:the low 32 bit value of used space
//  Return:
//       SFS_ERROR_NONE: Success ,the value of *high_word and *low_word is valid
//       Other: Fail ,the value of *high_word and *low_word is invalid
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_GetDeviceUsedSpace(const uint16 *device_name,uint32 *high_word,uint32 *low_word)
{
	return SFS_ERROR_SYSTEM;
}

/*--------------------------------------------------------*/
//file and dir operation

//note:	1	if the space of name and overlapped_ptr is allocated by caller,it must 
//			be free by caller itself after api return.
//		2	the pathname or filename must like 	"devname:\\foldername1\\foldername2\\filename " in unicode form and nothing else,
//		3	length of pathname must less than SFS_MAX_PATH
//			
/*--------------------------------------------------------*/

/*****************************************************************************/
//  Description:   Create or open a file
//  Author: Jason.wu
//  Param
//       file_name :The name of path and file ,must be unicode string
//       access_mode:please refer to "Open/Create  flag  "
//       share_mode:reserved
//       file_attri:reserved
//  Return:
//       None zero: Success,this value is the handle of file
//       Zero: fail
//  Note: 
/*****************************************************************************/
SFS_HANDLE SFS_CreateFile(
const uint16 *file_name, 
uint32 access_mode,
uint32 share_mode,	//Must be NULL,File System have not realize it 
uint32 file_attri		//Must be NULL,File System have not realize it 
)
{
	return 0;
}


/*****************************************************************************/
//  Description:  Close the find handle returned by SFS_FindFirstFile
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_FindFirstFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_FindClose(SFS_HANDLE sfs_handle)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;/*lint !e527*/
}

/*--------------------------------------------------------*/
//file internal operation
/*--------------------------------------------------------*/

/*****************************************************************************/
//  Description:  Get file length
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       size:If success ,the file length is stored in this param
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_GetFileSize(SFS_HANDLE sfs_handle, uint32*  size)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;/*lint !e527*/	
}

/*****************************************************************************/
//  Description:  Set file length
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       size:The length you want to set
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
//       If param size is less than file length ,some data in the tail of file will be lost
/*****************************************************************************/
SFS_ERROR_E SFS_SetFileSize(SFS_HANDLE sfs_handle, uint32   size)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;	/*lint !e527*/
}

/*****************************************************************************/
//  Description:  Read file
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       buffer:The data has be read will stored in this buffer 
//       bytes_to_read:the number you want to read ,unit is byte
//       bytes_read:the number you has be read infact
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_ReadFile(
	SFS_HANDLE handle,
	void *buffer,
	uint32 bytes_to_read,
	uint32 *bytes_read,
	SFS_OVERLAPPED_T *overlapped
)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;	/*lint !e527*/
}

/*****************************************************************************/
//  Description:  Write file
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       buffer:The data will be writen is stored in this buffer 
//       bytes_to_write:the number you want to write ,unit is byte
//       bytes_written:the number you has be write infact
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_WriteFile(
	SFS_HANDLE handle,
	const void *buffer,
	uint32 bytes_to_write,
	uint32 *bytes_written,
	SFS_OVERLAPPED_T *overlapped
)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;/*lint !e527*/	
}

/*****************************************************************************/
//  Description:  Flush file
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       overlapped:asynchronism param,if you give this parm,the result value will be returned in callback funtion
//  Return:
//       SFS_ERROR_NONE: Success
//       SFS_ERROR_IO_PENDING:if you call it in asynchronism way,the real result will be given in callback funtion after SFS finish the command
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_FlushFile(
	SFS_HANDLE handle,
	SFS_OVERLAPPED_T *overlapped
)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;	/*lint !e527*/
}

/*****************************************************************************/
//  Description:  Set current positon in file
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       offset:the relative posion of origin.
//       origin:the absolutely you will be seek.It can be SFS_SEEK_BEGIN ,SFS_SEEK_CUR,SFS_SEEK_END
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_SetFilePointer(
	SFS_HANDLE handle,
	__int64 offset,
	uint32 origin
)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;	/*lint !e527*/
}

/*****************************************************************************/
//  Description:  Get current positon in file
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//       origin:the absolutely you will be find.It can be SFS_SEEK_BEGIN ,SFS_SEEK_END
//       *currentpos:If api success ,this will stored the relative posion of origin.
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_GetFilePointer(
	SFS_HANDLE handle,
	uint32 origin,
	int32* currentpos 
)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;/*lint !e527*/	
}

/*****************************************************************************/
//  Description:  Close the handler returned by SFS_CreateFile
//  Author: Jason.wu
//  Param
//       sfs_handle:The handle returned by SFS_CreateFile
//  Return:
//       SFS_ERROR_NONE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
SFS_ERROR_E SFS_CloseFile(SFS_HANDLE handle)
{
	SCI_ASSERT(0);/*assert verified*/
	return SFS_ERROR_SYSTEM;	/*lint !e527*/
}
#endif

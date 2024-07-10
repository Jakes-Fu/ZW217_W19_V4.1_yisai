/*****************************************************************
 **  File Name: 	brw_plinterface.h 							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:在这个文件中实现同平台相关的接口,brw的其它文件  *
 **              统一使用这里的接口,其它.c大部分需要包含这个头   *
 **              文件,包含的时候要将sci_types.h,os_api.h需要包   *
 **              的目录加到编译的make文件中                      *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  07/04/2009		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef BRW_PLINTERFACE_H
#define BRW_PLINTERFACE_H
#include "sci_types.h"
#include "os_api.h"
#include "sfs.h"
//#include "caf.h"
//#include "mmi_filemgr.h"

#if (defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
#include "img_gzip.h"
#endif

#define BRW_FULL_PATH_MAX_LEN			SFS_MAX_PATH //系统全路径文件名最大长度
#define BRW_ROOT_DIR    "Browser" //浏览器系统文件根目录文件夹
#define BRW_ROOT_DIR_LEN    7
#define BRW_FFS_MAX_PATH SFS_MAX_PATH
//#define	BRW_MEM_DEBUG
#define BRW_FILE_SLASH  "\\"//0x5c:according to MMIFILE_SLASH in platform

#define BRWDebug_Printf	SCI_TRACE_LOW

typedef SFS_HANDLE BRWPL_FFS_HANDLE;
typedef SFS_OVERLAPPED_T	BRWPL_FFS_OVERLAPPED_T;
typedef SFS_DATE_T	BRWPL_FFS_DATE_T;
typedef SFS_TIME_T	BRWPL_FFS_TIME_T;
typedef SFS_FIND_DATA_T	BRWPL_FFS_FIND_DATA_T;
#ifndef wchar
typedef unsigned short wchar;
#endif

/*
 *	desc:check if folder is exist
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWPL_IsFolderExist(const uint16 *full_path_ptr,uint16 full_path_len);

PUBLIC BRWPL_FFS_HANDLE BRWPL_CreatFile(const uint16 *full_path_ptr);

/*
 *	desc:create folder
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *  return:TRUE if success else FALSE
 */
PUBLIC BOOLEAN BRWPL_CreateDir(const uint16 *full_path_ptr,uint16 full_path_len);

/*
 *	desc:copy file
 *  overlapped_ptr could be NULL, if using sync ffs
 */
PUBLIC BOOLEAN BRWPL_FileCopy(
							  uint16 *src_full_path_ptr, 
							  uint16 *dest_full_path_ptr
							  );

PUBLIC BOOLEAN BRWPL_DeleteDir(const uint16 *folder_path_ptr,const uint16 *folder_name_ptr);

/*
 *	desc:check if file is exist
 *  param:full_path_ptr  full path 
 *        full_path_len  length of path
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWPL_IsFileExist(const uint16 *full_path_ptr,uint16 full_path_len);

PUBLIC BRWPL_FFS_HANDLE BRWPL_FfsFindFirst(const uint16 *find, BRWPL_FFS_FIND_DATA_T *fd);

PUBLIC BOOLEAN BRWPL_FfsFindNext(BRWPL_FFS_HANDLE ffs_handler,BRWPL_FFS_FIND_DATA_T *fd);

PUBLIC BOOLEAN BRWPL_FindClose(BRWPL_FFS_HANDLE ffs_handler);

/*
 *	desc:get size of file
 *  param:full_path_ptr  full path 
 *        full_path_len  length of path
 *        size_ptr[output]   size of file
 *  return:TRUE if SUCCESS else FALSE
 */
PUBLIC BOOLEAN BRWPL_GetFileSize(const uint16 *full_path_ptr,uint16 full_path_len,uint32* size_ptr);
/*
 *	desc:read file(syn operation)
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *        buf_ptr   buffer to store data
 *        buf_max_len  max length of buffer
 *        read_len_ptr[output]  real length readed 
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWPL_ReadFileSyn(const uint16 *full_path_ptr,uint16 full_path_len,uint8* buf_ptr,
                                    uint32 buf_max_len,uint32* read_len_ptr);
PUBLIC BRWPL_FFS_HANDLE BRWPL_ReadFileAsync(
										const uint16 *full_path_ptr,
										uint8 *buf_ptr,
										uint32 bytes_to_read,
										BRWPL_FFS_OVERLAPPED_T *overlapped_ptr
									  );

PUBLIC BRWPL_FFS_HANDLE BRWPL_WriteFileAsync(
									   uint16 *full_path_ptr,
									   uint8 *buf_ptr,
									   uint32 write_size,
									   BRWPL_FFS_OVERLAPPED_T *overlapped_ptr
									   );
PUBLIC BRWPL_FFS_HANDLE BRWPL_AppendWriteFileAsync(
									   uint16 *full_path_ptr,
									   uint8 *buf_ptr,
									   uint32 write_size,
									   uint32 offset,
									   BRWPL_FFS_OVERLAPPED_T *overlapped_ptr,
									   SFS_ERROR_E  *sfs_error_ptr
									   );
PUBLIC BOOLEAN BRWPL_AppendWriteFileSync(
										 BRWPL_FFS_HANDLE ffs_handler,
										 uint8 *buf_ptr,
										 uint32 write_size,
										 uint32 offset
										 );
PUBLIC BOOLEAN BRWPL_CloseFile(BRWPL_FFS_HANDLE ffs_handler);
PUBLIC BOOLEAN BRWPL_RenameFile(uint16 *old_full_name_ptr, uint16 *new_full_name_ptr);

/*
 *	desc:write buffer to file(syn operation)
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *        buf_ptr   address of data to write
 *        to_write_len  the length of data want to write
 *  return:SFS_ERROR_E
 */
PUBLIC SFS_ERROR_E BRWPL_WriteFileSyn(const uint16 *full_path_ptr,uint16 full_path_len,const uint8* buf_ptr,
                                  uint32 to_write_len);
/*
 *	desc:delete file(syn operation)
 *  param:full_path_ptr  full path folder string
 *        full_path_len  length of path
 *  return:TRUE if exist else FALSE
 */
PUBLIC BOOLEAN BRWPL_DelFileSyn(const uint16 *full_path_ptr,uint16 full_path_len);
/*
function to print trace
*/
//void BRWDebug_Printf(const char* arg, ...);
/*
 *	desc:function for print warning,used to printed critical warning
 */
void BRWDebug_PrintWarning(const char* arg, ...);

#define   BRWDebug_Assert(_EXP)  SCI_ASSERT(_EXP)  
#define   BRWDebug_PAssert(_EXP,PRINT_STR)  SCI_PASSERT(_EXP,PRINT_STR)
/*
 *	desc:malloc memory
 *  param: size  size want to allocate
 *         file  name of file which call this function
 *         line  line num where call this function
 *  return:memory address if success,else return PNULL
 */
PUBLIC void * BRWMem_MallocEx(uint32 size,const char* file,uint32 line);
/*
 *	desc:free memory
 *  param: mem_ptr  memory address want to free
 *  return:void
 */
PUBLIC void   BRWMem_FreeEx(void* mem_ptr,char *pfile, int line);
/*
 *	desc:realloc memory
 *  param: mem_ptr  the former alloced memory address
  *        new_size new alloc size
 *  return:void
 */
PUBLIC void*   BRWMem_ReallocEx(void* mem_ptr, int new_size, char *pfile, int line);
/*
 *	desc:allocate memory
 *  param: size   size want to alloc
 */
#define BRWMem_Alloc(size) BRWMem_MallocEx(size,__FILE__,__LINE__) 
/*
 * desc:  free memory
 * param: memory address want to free
*/   
#define BRWMem_Free(mem_ptr) BRWMem_FreeEx(mem_ptr,__FILE__,__LINE__)
/*
 * desc:  realloc
 * param: memory address want to free
*/   
#define BRWMem_Realloc(mem_ptr, new_size) BRWMem_ReallocEx(mem_ptr, new_size, __FILE__,__LINE__)
/*
 *	desc: function like memcpy,if src or dest null,no assert
 */
PUBLIC void   BRW_Memcpy(void *dest, const void *src, uint32 size);
/*
 *	desc: function like memset,if dest null or size equal 0,no assert
 */
PUBLIC void   BRW_Memset(void*dest,uint8 value,uint32 size); 
/*
 *	function like memcmp
 */
PUBLIC uint32 BRW_Memcmp(const void * src_ptr, const void* * dest_ptr, uint32 count);
/*
 *	desc:get unicode char len
 */
PUBLIC uint32 BRW_WStrlen( const uint16* str);
/*
 *	desc:get length of null terminated string,if string null,no assert
 */
PUBLIC uint32 BRW_Strlen(const uint8 *string);
/*
 *	desc:function like strcpy,if src or dest null,no assert
 */
PUBLIC uint8* BRW_Strcpy(uint8 *dst, const uint8 *src);
/*
 *	desc:function like strcat,if src or dest null,no assert
 */
PUBLIC uint8* BRW_Strcat(uint8 *dst, const uint8 *src);
/*
 *	desc:function like strncpy,if src or dest null,no assert
 */
PUBLIC uint8 *BRW_Strncpy( uint8 *dst, const uint8 *src, uint32 len);
/*
 *	desc:function like strcmp,if string or str null ,no assert
 */
PUBLIC uint32 BRW_Strcmp(const uint8 * string, const uint8 * str);
/*
 *	desc:function like strncmp,if string or str null ,no assert
 */
PUBLIC uint32 BRW_Strncmp(const uint8 * string, const uint8 * str,uint32 size);
/*
 *	desc:function like stricmp,if string or str null ,no assert
 */
PUBLIC uint32 BRW_Stricmp(const uint8 * string, const uint8 * str);
/*
 *	desc:function like strnicmp,if string or str null ,no assert
 */
PUBLIC uint32 BRW_Strnicmp(const uint8 * string, const uint8 * str, uint32 size);
/*
 *	desc:function like strstr,if string or str null ,no assert
 */
PUBLIC uint8* BRW_Strstr(const uint8 *string, const uint8 *str);
/*
 *	desc:function like case insensitive strstr(like casestrstr in linux), if string null ,no assert
 */
PUBLIC const uint8 * BRW_CaseStrstr (const uint8 * s1, const uint8 * s2);
/*
 *	desc:function like strch,if string null ,no assert
 */
PUBLIC uint8* BRW_Strchr(const uint8 *str, uint8 c);
/*
 *	desc:function like atoi,if str null,no assert
 */
PUBLIC uint32 BRW_Atoi(const uint8 *str);
/*
 *	desc:check if long_str's tail equal with tail,consider case difference
 */
PUBLIC uint32  BRW_TailStrcmp(const uint8 *long_str, const uint8* tail);
/*
 *	desc:compare long_str's tail with tail,ingore case difference
 */
PUBLIC uint32  BRW_TailStricmp(const uint8 *long_str, const uint8* tail);
/*
 *	desc: get current system time,just like windows time_t
 */
PUBLIC uint32 BRWPL_GetCurTime(void);
/*
 *	desc:convert asc str to unicode wchar
 */
PUBLIC wchar* BRWPL_StrToWstr(const char* src_ptr,wchar* dst_ptr);
/*
 *	desc:convert unicode wchar to asc str
 */
PUBLIC char* BRWPL_WstrToStr(const wchar* src_ptr,char* dst_ptr);

PUBLIC wchar *BRWPL_WstrStrnCatWithAsciiStr(wchar *dest_ptr,	const char *src_ptr, uint32 dest_len );

PUBLIC wchar *BRW_WstrCat(wchar *dest_ptr,	const wchar *src_ptr);

/*
 *	desc:获取存放浏览器系统文件的设备名(unicode格式)
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC wchar* BRWPL_GetBrwSysDevName(uint16 *device_name_len_ptr);

/*
 *	desc:获取存放浏览器cookie文件的设备名(unicode格式)
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC wchar* BRWPL_GetBrwCookieDevName(uint16 *device_name_len_ptr);

/*
 *	desc:check and get validate device,without system device(C:)
 *	notes:for MS00254277
 *  param:device_name_len_ptr[out]  设备名长度
 *  return:unicode格式的设备名
 */
PUBLIC wchar* BRWPL_GetValidateDevName(uint16 *device_name_len_ptr);

/*
 *	desc:combine pathname with two name of dir_ptr and filename
 *  param:device_ptr  unicode格式设备名称
 *        device_len  设备长度,双字节单位
 *        dir_ptr unicode格式路径
 *        dir_len 路径长度
 *        file_name_ptr unicode格式文件名,可以为空
 *        file_name_len 文件名长度,双字节单位
 *        full_path_ptr unicode格式全路径
 *        full_path_len_ptr[in&out]  传入文件名buf最大大小,传出全路径最大长度
 */
PUBLIC BOOLEAN BRWPL_CombinePath(
                              const wchar     *device_ptr, 
                              uint16           device_len, 
                              const wchar     *dir_ptr,
                              uint16           dir_len,
                              const wchar     *file_name_ptr,
                              uint16           file_name_len,
                              wchar           *full_path_ptr,
                              uint16           *full_path_len_ptr
                              );

PUBLIC BOOLEAN BRWMem_CreatMemHandle(void);
PUBLIC BOOLEAN BRWMem_InitMem(void);

PUBLIC void BRWMem_DeleteMemHandle(void);
#ifdef BRW_MEM_DEBUG
PUBLIC void BRWMem_print_mem(void);
#endif

/*
 *	desc:get browser system file full path for write
 *	notes:
 *  param:
 *  return:result
 */
PUBLIC BOOLEAN BRWPL_GetValidDevSysBrwRootForWrite(
                wchar           *full_path_ptr,     //out
                uint16           *full_path_len_ptr,  //in/out, 双字节为单位
                uint32 *dev_ptr   //in/out,can be null
                );

#endif
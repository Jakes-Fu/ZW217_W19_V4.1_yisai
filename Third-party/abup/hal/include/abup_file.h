#ifndef _ABUP_FILE_H_
#define _ABUP_FILE_H_

abup_extern ABUP_BOOL abup_get_fota_drv_size(abup_uint64* total_size,abup_uint64* free_size);
abup_extern abup_int32 Abup_Open_File(abup_wchar *file_path,abup_uint32 flag);
abup_extern abup_int32 Abup_Seek_File(abup_int32 file_handle,abup_uint32 offset,abup_int32 origin);
abup_extern abup_int32 Abup_Remove_File(abup_wchar *file_path);
abup_extern abup_int32 Abup_Write_File(abup_int32 file_handle,abup_char *inBuff,abup_uint32 len,abup_uint32 *write_len);
abup_extern abup_int32 Abup_Read_File(abup_int32 file_handle,abup_char *outBuff,abup_uint32 len,abup_uint32 *write_len);
abup_extern void Abup_Close_File(abup_int32 file_handle);

abup_extern abup_wchar* abup_get_delta_file_path(void);
abup_extern abup_wchar* abup_get_delta_file_path12(void);
abup_extern abup_wchar* abup_get_delta_file_path21(void);
abup_extern abup_wchar* abup_get_version_file_path(void);
abup_extern abup_wchar* abup_get_delta_id_path(void);
abup_extern abup_int32 abup_Get_File_Size(abup_int32 file_handle,abup_uint32 *file_size);
abup_extern abup_wchar* abup_get_login_file_path(void);
abup_extern abup_wchar* abup_get_backup_file_path(void);

abup_extern void abup_set_append(void);
abup_extern void abup_reset_ipaddr(void);
abup_extern abup_uint32 abup_get_ipaddr(void);
abup_extern void abup_set_ipaddr( abup_uint32 ipaddr);
#endif
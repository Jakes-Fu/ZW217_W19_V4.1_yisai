#ifndef __HELPER_FILE_H
#define __HELPER_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "port_cfg.h"
#include "sds.h"

// RTOS platform, is not the simulator for PC version
#ifdef REAL_WATCH_RTOS
	os_FILE os_fopen_impl(char const* filename, char const* mode);
	size_t os_fwrite_impl(void const* buffer, size_t elementSize, size_t elementCount, os_FILE fileHandle);
	int os_fclose_impl(os_FILE fileHandle);
	int os_rename_impl(char const* oldFileName, char const* newFileName);
	int	os_remove_impl(char const* fileName);
	size_t os_fread_impl(void * buffer, size_t elementSize, size_t elementCount, os_FILE fileHandle);
	int os_fseek_impl(os_FILE fileHandle, long offset, int origin);
#endif

int		helper_fs_sdcard_ready(void);
sds		helper_load_file(const char* filename);
int		helper_save_file(const char* filename, sds data);
int		helper_save_file_limit(const char* filename, sds data, const int limitSize);
int		helper_delete_file(const char* filename);
int		helper_is_file_exist(const char* filename);
int		helper_is_folder_exist(const char* dstFolderPath);
int 	helper_new_folder(const char* dstFolderPath);
int 	helper_clear_cache(const char* dst, const char* prefix, const char* keyword);
int 	helper_clear_image(const char* dstFolderPath);
int 	helper_clear_mp3(const char* dstFolderPath);
long	helper_get_file_size(const char* filename);
int		helper_is_local_file(const char* filename);
const char * helper_get_real_path(const char * path);

int		helper_space_is_enough(const char * devname, const int minSize);
unsigned long helper_get_user_space(const char * devname);

#ifdef __cplusplus
}
#endif

#endif // __HELPER_FILE_H
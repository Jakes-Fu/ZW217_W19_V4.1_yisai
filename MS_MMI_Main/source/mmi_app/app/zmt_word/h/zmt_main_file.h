/*
 * @Author: yfm
 * @Date: 2022-08-01 11:12:49
 * @LastEditors: yfm
 * @LastEditTime: 2022-08-30 15:38:34
 * @FilePath: dsl_main_file.h
 * @Description: 
 */
#include "std_header.h"
#include "mmi_common.h"
#include "mmifmm_export.h"
#include "mmi_filemgr.h"
#include "mmifilearray_export.h"


PUBLIC BOOLEAN zmt_tfcard_exist(void);
PUBLIC uint32 zmt_tfcard_get_free_kb(void);
PUBLIC uint32 zmt_tfcard_get_used_kb(void);
PUBLIC BOOLEAN zmt_file_save(const char * file_name,uint8  * data_ptr, uint32 data_len,BOOLEAN is_add);
PUBLIC BOOLEAN zmt_file_read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len);
PUBLIC void zmt_file_data_add(void * data, uint32 size, char * file_path);
PUBLIC void zmt_file_data_write(void * data, uint32 size, char * file_path);
PUBLIC void * zmt_file_data_read(char * file_path,uint32 * data_size);
PUBLIC BOOLEAN zmt_file_delete(char * file_path);
PUBLIC BOOLEAN zmt_file_exist(char * file_path);
PUBLIC BOOLEAN zmt_dir_exsit(char * file_path);
PUBLIC BOOLEAN zmt_dir_creat(char * file_path);
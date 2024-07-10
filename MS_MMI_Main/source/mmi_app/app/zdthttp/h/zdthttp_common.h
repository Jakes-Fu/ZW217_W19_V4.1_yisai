#ifndef __ZDTHTTP_COMMON_H__
#define __ZDTHTTP_COMMON_H__

#include "std_header.h"
#include "mn_type.h"
#include "mmi_nv.h"
#include <stdio.h>
#include "nvitem.h"
#ifdef _RTOS
#include "sci_api.h" /*@tony.yao  replase sci_mem.h with sci_api.h*/
#else
#include "tasks_id.h"
#endif
#include "mn_api.h"
#include "mn_events.h"
#include "mn_type.h"
#include "nv_item_id.h"
#include "sio.h"
#include "sig_code.h"
#include "os_api.h"
#include "IN_message.h"
#include "socket_types.h"

#include "mmi_signal_ext.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "window_parse.h"
#include "guitext.h"
#include "guilcd.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "mmi_image.h"
#include "guiedit.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmiset_export.h"
#include "mmienvset_export.h"
#include "mmisms_export.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"


#ifdef FILE_LOG_SUPPORT
#define ZDTHTTP_LOG		Trace_Log_Buf_Print
#else
#define ZDTHTTP_LOG            SCI_TRACE_LOW
#endif

#define ZDTHTTP_RCV_TEMP_FILE "Temp.mp3"
#define ZDTHTTP_RCV_TEMP_DIR  "E:\\Temp\\"
#define ZDTHTTP_RCV_TEMP_FULLPATCH "E:\\Temp\\Temp.mp3"

BOOLEAN ZDT_HTTP_File_Save(const char * file_name,uint8  * data_ptr, uint32 data_len,BOOLEAN is_add);
BOOLEAN ZDT_HTTP_File_Remove(const char * file_name);
BOOLEAN ZDT_HTTP_File_Read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len);
BOOLEAN ZDT_HTTP_File_Exsit(const char * file_name);
BOOLEAN ZDT_HTTP_Dir_Exsit(const char * dir_path);
BOOLEAN ZDT_HTTP_File_RemoveDirFile(const char * dir_path);
MMIFILE_HANDLE  ZDT_HTTP_File_Open(const char * file_name,BOOLEAN is_read);
uint32  ZDT_HTTP_File_Write(MMIFILE_HANDLE file_handle,uint8  * data_ptr, uint32 data_len);
void  ZDT_HTTP_File_Close(MMIFILE_HANDLE file_handle);
uint32  ZDT_HTTP_File_GetSize(const char * file_name);
BOOLEAN  ZDT_HTTP_File_Rename(const char * file_name, const char * new_filename);
BOOLEAN ZDT_HTTP_MemFull(BOOLEAN is_tf, uint32 need_len);

#endif

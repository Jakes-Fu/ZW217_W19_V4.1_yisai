#ifndef __BEACON_COMMON_H__
#define __BEACON_COMMON_H__

#include "std_header.h"
#include "mn_type.h"
#include "mmi_nv.h"
#include "sci_types.h"
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
#include "mmifmm_export.h"


#define BEACON_LOG            SCI_TRACE_LOW

// Type to get parameter
typedef enum
{
    BT_GET_NUM_DEC = 0,  // Number - decimal
    BT_GET_NUM_HEX,      // Number - hexadecimal
    BT_GET_STR_WITH_DQ,  // "String"
    BT_GET_STR_WO_DQ     // String
} T_BT_GET_PARA_TYPE;

extern BOOLEAN BEACON_SIM_Exsit(void);

extern uint8 BEACON_Nibble2HexChar(uint8 bNibble);
extern uint8 BEACON_HexChar2Nibble(uint8 bHexChar);

extern uint8 BEACON_GetNextPara(T_BT_GET_PARA_TYPE tType, uint8** pCmd, uint16* pLen, uint32* pNum, uint8* pStr, uint16 wMaxStrLen);
extern uint16  BEACON_IsAllNumber(
                             uint8*  src_ptr, //[IN]  String的源指针
                             uint16  src_len //[IN]  源String的长度，byte数
                             );
extern uint16 BEACON_Str16_to_Int(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint8*  dest_ptr //[OUT] String的目标指针
                           );
extern uint16 BEACON_UCS_Str16_to_uint16(
                           uint8*  src_ptr, //[IN]  String的源指针
                           uint16  src_len, //[IN]  源String的长度，byte数
                           uint16*  dest_ptr, //[OUT] String的目标指针
                           uint16 dest_max_len
                           );
extern uint32  BEACON_Get_DayTimeSecond(void);
extern uint32  BEACON_Get_TimeSecond(void);

extern BOOLEAN  BEACON_DBFile_Read(char * path, char * filename, void *buffer, uint32 bytes_to_read, uint32 *bytes_read);
extern BOOLEAN  BEACON_DBFile_Write(char * path,char * filename, uint8 * data_ptr, uint32  data_len);
extern BOOLEAN BEACON_DBFile_Exist(char * path, char * filename);
extern uint32  BEACON_DBFile_GetSize(char * path, char * filename);
extern BOOLEAN  BEACON_DBFile_RemoveDir(char * path	);
extern BOOLEAN  BEACON_DBFile_RemoveFile(char * path, char * filename);

extern BOOLEAN BEACON_File_MemFull(BOOLEAN is_tf);
extern BOOLEAN BEACON_File_Save(const char * file_name,uint8  * data_ptr, uint32 data_len,BOOLEAN is_add);
extern BOOLEAN BEACON_File_Remove(const char * file_name);
extern BOOLEAN BEACON_File_Read(const char * file_name,uint8  * data_ptr, uint32  data_len, uint32 * read_len);
extern BOOLEAN BEACON_File_Exsit(const char * file_name);
extern BOOLEAN BEACON_Dir_Exsit(const char * dir_path);
extern BOOLEAN BEACON_File_RemoveDirFile(const char * dir_path);
extern MMIFILE_HANDLE  BEACON_File_Open(const char * file_name,BOOLEAN is_read);
extern uint32  BEACON_File_Write(MMIFILE_HANDLE file_handle,uint8  * data_ptr, uint32 data_len);
extern void  BEACON_File_Close(MMIFILE_HANDLE file_handle);
extern uint32  BEACON_File_GetSize(const char * file_name);
extern BOOLEAN  BEACON_File_Rename(const char * file_name, const char * new_filename);
#endif

#ifndef __ZFB_COMMON_H__
#define __ZFB_COMMON_H__

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


#define ZFB_LOG            SCI_TRACE_LOW

extern BOOLEAN ZFB_SIM_Exsit(void);

extern int ZFB_2VM_ConvertTo_Bmp(uint8 * qData,uint32 DataLen,uint16 zoom_vol,uint8 ** ppImg, uint32 *pLen);
extern int ZFB_BarCode_ConvertTo_Bmp(uint16* pBarCode,uint32 BarCodeLen,uint16 zoom_vol,uint8 ** ppImg, uint32 *pLen);

extern BOOLEAN  ZFB_DBFile_Read(char * path, char * filename, void *buffer, uint32 bytes_to_read, uint32 *bytes_read);
extern BOOLEAN  ZFB_DBFile_Write(char * path,char * filename, uint8 * data_ptr, uint32  data_len);
extern BOOLEAN ZFB_DBFile_Exist(char * path, char * filename);
extern uint32  ZFB_DBFile_GetSize(char * path, char * filename);
extern BOOLEAN  ZFB_DBFile_RemoveDir(char * path	);
extern BOOLEAN  ZFB_DBFile_RemoveFile(char * path, char * filename);
extern uint32 ZFBGetSystemTime(void);
#ifdef WIN32
extern int64 ZFBGetSystemTimeMS(void);
#else
extern long long ZFBGetSystemTimeMS(void);
#endif

#endif

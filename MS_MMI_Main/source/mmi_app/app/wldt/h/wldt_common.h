#ifndef __WLDT_COMMON_H__
#define __WLDT_COMMON_H__

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


#define WLDT_LOG            WLDT_AppTrace

extern BOOLEAN WLDT_SIM_Exsit(void);
extern uint32  WLDT_Get_DayTimeSecond(void);
extern uint32  WLDT_Get_TimeSecond(void);

extern BOOLEAN  WLDT_DBFile_Read(char * path, char * filename, void *buffer, uint32 bytes_to_read, uint32 *bytes_read);
extern BOOLEAN  WLDT_DBFile_Write(char * path,char * filename, uint8 * data_ptr, uint32  data_len);
extern BOOLEAN WLDT_DBFile_Exist(char * path, char * filename);
extern uint32  WLDT_DBFile_GetSize(char * path, char * filename);
extern BOOLEAN  WLDT_DBFile_RemoveDir(char * path	);
extern BOOLEAN  WLDT_DBFile_RemoveFile(char * path, char * filename);

#endif

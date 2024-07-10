#ifndef _MMIDV_MANAGER_H
#define _MMIDV_MANAGER_H

#include "sci_types.h"
//#include "mmk_type.h"
#include "dal_recorder.h"

/*****************************************************************************/
// 	Description : start to run the flow of dv
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_Open( void );

/*****************************************************************************/
// 	Description : start to preview video
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_StartPreview(void);

/*****************************************************************************/
// 	Description : stop dv preview
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_StopPreview(void);

/*****************************************************************************/
// 	Description : close dv
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_Close(void);

/*****************************************************************************/
// 	Description : start record dv
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_StartRecord(DRECORDER_END_CALLBACK RecordEndCallback,uint16 *filename);

/*****************************************************************************/
// 	Description : dc flow operation
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_PauseRecord(void);

/*****************************************************************************/
// 	Description : dc resume record
//	Global resource dependence : none
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_ResumeRecord(void);

/*****************************************************************************/
// 	Description : stop record video
//	Global resource dependence : 
//  Author: hui.zhao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDV_StopRecord(void);
#endif



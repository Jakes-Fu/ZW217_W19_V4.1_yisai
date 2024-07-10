#ifndef _JPEG_TASK_H_
#define _JPEG_TASK_H_

#if (defined(JPG_DEC_SUPPORT_JPG_HW) || defined(JPG_ENC_SUPPORT_JPG_HW))

#include "sci_types.h"
#include "os_api.h"
#include "jpeg.h"

#define	JPEG_EVENT_NONE				0x00000000
#define	JPEG_EVENT_DEC_OK			0x00000001
#define	JPEG_EVENT_ENC_OK			0x00000002
#define	JPEG_EVENT_OVERTIME			0x00000004 
#define	JPEG_EVENT_STOPPED			0x00000008
#define	JPEG_EVENT_ERROR			0x00000010

typedef struct
{
    xSignalHeaderRec    sig;
    uint32        		param;
}JPEG_SIGNAL_T;


/******************************************************************************
// Purpose:	send signal to jpeg task
// Author:	shan.he
// Input:      
			pContext	--point  to inner variables 		
// Output:  	None	
// Return:  	refer to JPEG_RET_E
// Note:    
******************************************************************************/
PUBLIC void JPEG_SendMsg(JPEG_SIG_E sig_code, uint32 param);

/******************************************************************************
// Purpose:	malloc resources used by jpeg		
// Author:	frank.yang
 // Input:        None		
// Output:  	None	
// Return:  	
// Note:    
******************************************************************************/
PUBLIC uint32 JPEG_RscAlloc(void);

/******************************************************************************
// Purpose:	free jpeg task resource
// Author:	shan.he
// Input:      
			pContext	--point  to inner variables 		
// Output:  	None	
// Return:  	refer to JPEG_RET_E
// Note:    
******************************************************************************/
PUBLIC uint32 JPEG_RscFree(void);

/******************************************************************************
// Purpose:	sync decode jpeg by hardware	
// Author:	shan.he
// Input:      
			pContext	--point  to inner variables 		
// Output:  	None	
// Return:  	refer to JPEG_RET_E
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_SyncHalDecode(JPEG_CONTEXT_T *pContext);

/******************************************************************************
// Purpose:	sync encode jpeg by hardware	
// Author:	shan.he
// Input:      
			pContext	--point  to inner variables 		
// Output:  	None	
// Return:  	refer to JPEG_RET_E
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_SyncHalEncode(JPEG_CONTEXT_T* pContext);

/******************************************************************************
// Purpose:	hardware sync decoding and encoding	
// Author:	frank.yang
// Input:      
			pContext	--point  to inner variables 		
// Output:  	None	
// Return:  	refer to JPEG_RET_E
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_SyncHalDecEnc(JPEG_CONTEXT_T* pContext);

/******************************************************************************
// Purpose:	async decode jpeg by hardware	
// Author:	shan.he
// Input:      
			pContext	--point  to inner variables 		
// Output:  	None	
// Return:  	refer to JPEG_RET_E
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_AsyncHalDecode(JPEG_CONTEXT_T *pContext);

/******************************************************************************
// Purpose:	async encode jpeg by hardware	
// Author:	shan.he
// Input:      
			pContext	--point  to inner variables 		
// Output:  	None	
// Return:  	refer to JPEG_RET_E
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_AsyncHalEncode(JPEG_CONTEXT_T *pContext);

/******************************************************************************
// Purpose:	wait until the jpeg task stopped		
// Author:	shan.he
// Input:      
			pContext	--point  to inner variables 		
// Output:  	None	
// Return:  	refer to JPEG_RET_E
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_WaitTaskStopped(JPEG_CONTEXT_T* pContext);

/******************************************************************************
// Purpose:	is jpeg task exits		
// Author:	shan.he
// Input:      		
// Output:  	None	
// Return:  
// Note:    
******************************************************************************/
PUBLIC BOOLEAN JPEG_IsTaskExist(void);

/******************************************************************************
// Purpose:	change the priority of jpeg task
// Author:	shan.he
// Input:      
			priority	--priority of the jpeg task		
// Output:  	None	
// Return:  	none
// Note:    
******************************************************************************/
PUBLIC JPEG_RET_E JPEG_SetTaskPriority(uint32 new_priority, uint32 *old_priority_ptr);
#endif
				
#endif		   //_JPEG_TASK_H_


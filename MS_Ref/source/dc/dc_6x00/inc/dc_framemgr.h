/******************************************************************************
 ** File Name:	dc_bufmgr.h                                               	  
 ** Author:		Frank.Yang		                                          
 ** DATE:		8/14/2008                                               
 ** Copyright:		2008 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:                                                              
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 ** ------------------------------------------------------------------------- 
 ** DATE              NAME             DESCRIPTION                            
 ******************************************************************************/


#ifndef _DC_BUFMGR_H_
#define _DC_BUFMGR_H_

#ifdef __cplusplus
	extern "C"
	{
#endif
#include "sci_types.h"
#include "dc_common_6600l.h"

#define DCAM_BUFFER_THRESHOLD          (1024*16)   /*reserved 32K bytes*/
#define MAX_FIFO_FRAME_NUM       32
#define MAX_FIFO_FRAME_MASK  0x1F


typedef struct DCAM_BUFFER_MGR_Tag
{
	DC_FRAME_T	frames[MAX_FIFO_FRAME_NUM];
	int				first_frame;
	int				last_frame;
	uint32			ring_begin;
	uint32			ring_end;
	uint32			base_addr;
	uint32			uplimited_addr;
	uint32			remain_len;
	uint32			buf_len;
	uint32			tail_fragment_len;
}DCAM_BUFFER_MGR_T;




/******************************************************************************/
// Description: 	init frames mgr
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		none
// Note:       		
/******************************************************************************/
PUBLIC void DC_FrameMgr_Init(uint8 *p_buf, uint32 dw_len);

/******************************************************************************/
// Description: 	create a frame
// Author:     		Frank.yang
// Input:      		param_ptr   point to the preview parameter
// Output:		none
// Return:	
// Note:       		none
/******************************************************************************/
PUBLIC DC_FRAME_T *DC_FrameMgr_CreateAFrame(uint32 dw_frame_size);


/******************************************************************************/
// Description: 	get a frame
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:	
// Note:       		none
/******************************************************************************/
PUBLIC DC_FRAME_T * DC_FrameMgr_GetAFrame(void);



/******************************************************************************/
// Description: 	free a frame
// Author:     		Frank.yang
// Input:      		p_frame--point to the frame needed to be free
// Output:		none
// Return:	
// Note:       		none
/******************************************************************************/
PUBLIC int DC_FrameMgr_FreeAFrame(DC_FRAME_T *p_frame);

#ifdef __cplusplus
    }
#endif

#endif
 

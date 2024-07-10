

#ifndef _DC_COMMON_6600L_H_
#define _DC_COMMON_6600L_H_

#ifdef __cplusplus
	extern "C"
	{
#endif

#include "sci_types.h"
#include "dal_dcamera.h"
#include "dal_recorder.h"


#define DCAM_STATUS_IDLE          0
#define DCAM_STATUS_DECODE_DONE	8
//#define DCAM_STATUS_DISPLAY_DONE	0x40//1111111111111

typedef enum 
{
	VIDEO_MODE_MJPG = 0,
	VIDEO_MODE_YUV,
	VIDEO_MODE_MAX
}VIDEO_MODE_E;

typedef struct
{
	uint8  *p_head;
	uint32 dw_head_len;
	uint32 dw_total_section_len;
	struct {uint8 *pbuf; uint32 dw_len;} sections[2];
	uint8  *p_tail;
	uint32 dw_tail_len;
}DC_FRAME_T;

/*return: 0 successful, others failed*/
typedef int (*DC_RECORD_CALLBACK_FUNC)( uint32  i_frame, DC_FRAME_T *p_frame);


typedef enum
{
	YUV_PLANE_2 = 0,
	YUV_PLANE_3,
	YUV_PLANE_MAX
}DC_YUV_PLANE_E;

typedef struct DISPLAY_DATA_FRAME_Tag
{
	uint8*		y_addr;
	uint8*		uv_addr;
	uint8*            uv1_addr;
	uint32		y_data_len;
	uint32		uv_data_len;
	uint32            uv1_data_len;
	uint32		yuv_type;
	DC_YUV_PLANE_E yuv_plane;
}DISPLAY_DATA_FRAME_T;


typedef struct
{
	VIDEO_MODE_E video_mode;
	int		i_lcd;                  					/* display on which lcd, only for mjpg*/
	DCAMERA_RECT_T st_disp_rect;			/* display rectangular on LCD, only for mjpg*/
	DCAMERA_RECT_T target_rect;
	int		i_frame_rate;           				/* frame rate*/
	int    	i_quality;							/* image quality, large value refer to hight quality*/
	uint16	w_target_width; 					/* target jpeg width*/
	uint16  	w_target_height;					/* target jpeg height*/

	DRECORDER_END_CALLBACK end_callback;
}DC_MJPG_PARAM_T;


typedef struct
{
	uint32 video_buf_addr;
	uint32 video_buf_len;
	int    i_lcd;					/* display on which lcd*/
	DCAMERA_RECT_T st_disp_rect;           /* display rectangular on LCD*/
	DCAMERA_RECT_T target_rect;           /* display rectangular on LCD*/    
	DCAMERA_RECT_T st_trim_rect;           /* trim rectangular*/
	uint16 src_width;
	uint16 src_height;
	DCAMERA_DISP_MODE_E         disp_mode;
	DCAMERA_ROTATION_MODE_E     rotation;	
	DCAMERA_CALL_BACK           review_callback; 	// call back function for MJPEG playback, don't care param1 and param2	
}DC_DPLAYER_DISPLAY_PARAM_T;



/******************************************************************************/
// Description: 	set mjpg parameters
// Author:     		Frank.yang
// Input:      		pst_param:		point to the mjpg parameter
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		before recording m-jpeg, call this function to set parameters
/******************************************************************************/
PUBLIC int DC_SetMjpgRecParam(const DC_MJPG_PARAM_T *pst_param);

/******************************************************************************/
// Description: 	start mjpg capture
// Author:     		Frank.yang
// Input:      		dw_framenum:	total frame number, 0 is forbidden
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_StartMjpgRec(uint32 dw_framenum, DC_RECORD_CALLBACK_FUNC pf_callback);



/******************************************************************************/
// Description: 	stop mjpg capture 
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_StopMjpgRec(void);


/******************************************************************************/
// Description: 	free memory frame in pool 
// Author:     		Frank.yang
// Input:      		p_frame:	point to the frame structure
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_FreeFrame(DC_FRAME_T * p_frame);

/******************************************************************************/
// Description: 	set video display parameter
// Author:     		Frank.yang
// Input:      		pst_param:
// Output:		none
// Return:		0           	successful
//				others      failed
// Note:       		
/******************************************************************************/
PUBLIC int DC_VideoSetDisplayParam(
							const DC_DPLAYER_DISPLAY_PARAM_T *pst_param);
/******************************************************************************/
// Description: 	query YUV running state
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		
// 				DCAM_STATUS_IDLE         			 0
// 				DCAM_STATUS_PREVIEW_FIRST		1
// 				DCAM_STATUS_DEC_FIRST			2
// 				DCAM_STATUS_ENCODE_DONE 		4
// 				DCAM_STATUS_DECODE_DONE		8
// 				DCAM_STATUS_DMA_DONE			0x10
// 				DCAM_STATUS_LCM_BUSY			0x20
// 				DCAM_STATUS_DISPLAY_DONE		0x40
// 				DCAM_STATUS_ERROR				0xffff0000
// Note:       		
/******************************************************************************/
PUBLIC int DC_QueryStatus(void);

#ifdef __cplusplus
    }
#endif


#endif



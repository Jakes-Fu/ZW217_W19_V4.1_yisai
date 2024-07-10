/******************************************************************************
 ** File Name:    isp_service.h                                               *
 ** Author:       Benny.zou                                                   *
 ** DATE:         6/25/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  SD6800 ISP service, more comments will be added later.                                                            *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 6/25/2006     Benny.zou       Create.                                     *
 ** 8/31/2006	  patric.fang	  change									  *
 *****************************************************************************/
 
#ifndef _ISP_SERVICE_H_
#define _ISP_SERVICE_H_

#include "isp_drv_sc8800h.h"


//frame name define
#define ISP_FRAME_INIMAGE       		0x01
#define ISP_FRAME_ENCODER       		0x02
#define ISP_FRAME_DISPLAY       		0x03

//image format define
#define ISP_IMAGE_YUV420        		0x00060000
#define ISP_IMAGE_YUV422        		0x00080000
#define ISP_IMAGE_YSHIFT        		0x08000000
#define ISP_IMAGE_UVSHIFT       		0x04000000
#define ISP_IMAGE_HSYNC         		0x02000000
#define ISP_IMAGE_VSYNC         		0x01000000
#define ISP_IMAGE_YUYV          		0x00300000
#define ISP_IMAGE_YVYU          		0x00200000
#define ISP_IMAGE_UYVY          		0x00100000
#define ISP_IMAGE_VYUY          		0x00000000



#define ISP_DISPLAY_NONE				0xFF


typedef struct _isp_address_t
{
    uint32 						yaddr;
    uint32 						uaddr;
    uint32 						vaddr;
}ISP_ADDRESS_T;


typedef enum
{
	ISP_SERVICE_CALLBACK_ENCODE 	= 1,
	ISP_SERVICE_CALLBACK_DISPLAY	= 2

}ISP_SERVICE_CALLBACK_ID_E;

typedef enum
{
    ISP_IMAGE_FORMAT_YUV422 = 0,
    ISP_IMAGE_FORMAT_YUV420,
    ISP_IMAGE_FORMAT_YUV400,    
    ISP_IMAGE_FORMAT_YUV420_3FRAME,        
    ISP_IMAGE_FORMAT_RGB565,
    ISP_IMAGE_FORMAT_RGB888,
    ISP_IMAGE_FORMAT_CCIR656,
    ISP_IMAGE_FORMAT_JPEG,
    ISP_IMAGE_FORMAT_RAW,
    ISP_IMAGE_FORMAT_RGB666,
    ISP_IMAGE_FORMAT_MAX
}ISP_IMAGE_FORMAT;

typedef enum
{
	ISP_ROTATION_0 = 0x00,
	ISP_ROTATION_90,
	ISP_ROTATION_180,
	ISP_ROTATION_270,
	ISP_ROTATION_MIRROR,
	ISP_ROTATION_MAX
}ISP_ROTATION_E;


typedef struct
{
	ISP_SIZE_T							input_size;
	ISP_RECT_T 							input_range;

	uint32	   							disp_lcd_id;
	uint32	   							disp_block_id;
	ISP_RECT_T 							disp_range;
	ISP_ROTATION_E						disp_rotation;		// 0: 0; 1: 90; 2: 180; 3: 270
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
}ISP_PREVIEW_PARAM_T;

typedef struct
{
	ISP_SIZE_T							input_size;
	ISP_RECT_T 							input_range;
	ISP_SIZE_T							output_size;
	uint32								yuv_shift; 			//0:   not shif128, output data is signed; 1 shift128, out put data is unsigned.
									   						//1    for hantro use
									   						//0xff for raw rgb debug
	uint32								skip_frame_num;		//frame num skip before capture
	ISP_IMAGE_FORMAT                    sensor_data_format;  														
	
}ISP_JPEG_PARAM_T;

typedef struct
{
	ISP_RECT_T 							input_range;
	ISP_IMAGE_FORMAT	   				input_format;
	ISP_SIZE_T							output_size;
	uint32								yuv_shift; //0:   not shif128, output data is signed; 1 shift128, out put data is unsigned.
									   //1    for hantro use
									   //0xff for raw rgb debug	
}ISP_COMPOUND_JPEG_CAPTURE_PARAM_T;

typedef struct
{
	ISP_SIZE_T							input_size;
	ISP_RECT_T 							input_range;
	ISP_IMAGE_FORMAT	   				input_format;//rgb or yuv
	ISP_ADDRESS_T 						input_addr;	//input address
	uint32	   							disp_lcd_id;
	uint32	   							disp_block_id;
	ISP_RECT_T 							disp_range;
	ISP_ROTATION_E						disp_rotation; // 0: 0; 1: 90; 2: 180; 3: 270
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
}ISP_REVIEW_PARAM_T;

typedef struct
{
	ISP_SIZE_T							input_size;
	ISP_RECT_T 							input_range;
	ISP_IMAGE_FORMAT	   				input_format;	//rgb or yuv
	ISP_ADDRESS_T 						input_addr;	//input address
	ISP_SIZE_T							output_size;
	ISP_IMAGE_FORMAT	   				output_format;	//rgb or yuv
	ISP_ADDRESS_T 						output_addr;	//input address
	uint32								yuv_shift; //0: not shif128, output data is signed; 1 shift128, out put data is unsigned.
									   //1 for hantro use 
	
}ISP_SCLAE_PARAM_T;

typedef struct
{
	ISP_SIZE_T							input_size;
	ISP_RECT_T 							input_range;
	ISP_SIZE_T							encode_size;
	uint32								yuv_shift;
	uint32	   							disp_lcd_id;
	uint32	   							disp_block_id;
	uint32								is_display;
	ISP_RECT_T 							disp_range;
	ISP_ROTATION_E						disp_rotation;
    ISP_IMAGE_FORMAT                    data_format;
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
	
}ISP_MPEG_PARAM_T;

typedef struct
{
	//local 
	ISP_SIZE_T							local_input_size;
	ISP_RECT_T 							local_input_range;
	ISP_SIZE_T							encode_size;
    ISP_ROTATION_E                      encode_rotation;    
	uint32								yuv_shift;
	uint32	   							local_disp_lcd_id;
	uint32	   							local_disp_block_id;
	ISP_RECT_T							local_disp_range;
	ISP_ROTATION_E						disp_rotation;
	uint32								local_review_enable;
	uint32								local_display_enable;
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated

    uint32                              local_invalidate_mode;
    
}ISP_VT_ENCODE_PARAM_T;

typedef struct
{
	//remote	
	ISP_SIZE_T							remote_input_size;
	ISP_RECT_T 							remote_input_range;
	uint32	   							remote_disp_lcd_id;
	uint32	   							remote_disp_block_id;
	ISP_RECT_T							remote_disp_range;
	ISP_ROTATION_E						remote_disp_rotation;
	ISP_ADDRESS_T 						input_addr;	//input address
	uint32								deblock_enable;
	uint32								remote_review_enable;	
	uint32								remote_display_enable;	
    ISP_RECT_T                          lcd_rect;// lcd rect to be invalidated
}ISP_VT_DECODE_PARAM_T;


/*****************************************************************************/
//  Description:    open isp service	
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceOpen(void);

/*****************************************************************************/
//  Description:    close isp service	
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceClose(void);

/*****************************************************************************/
//  Description:    set preview parameter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr);

uint32 ISP_ServiceGetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr);


/*****************************************************************************/
//  Description:    start preview
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceStartPreview(void);

/*****************************************************************************/
//  Description:    stop preview
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStopPreview(void);

/*****************************************************************************/
//  Description:    start review
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStartReview(void);

uint32 ISP_ServiceReviewNext(ISP_ADDRESS_T *addr_ptr);

void ISP_ServiceStopReview(void);

/*****************************************************************************/
//  Description:    start review
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T *param_ptr);

/*****************************************************************************/
//  Description:    set DC review 
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceReviewFromDC(uint32 b_from_dc);


/*****************************************************************************/
//  Description:    bypass review,not do scaling or converting 
//	Global resource dependence: none
//  Author:         Baggio.he
//	Note:           
/*****************************************************************************/
void ISP_ServiceReviewBypass(void);

/*****************************************************************************/
//  Description:    set JPEG capture parameter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetJpegCaptureParam(ISP_JPEG_PARAM_T * param_ptr);

/*****************************************************************************/
//  Description:    start capture JPEG 
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStartCaptureJpeg(void);

/*****************************************************************************/
//  Description:    set compound JPEG capture parameter
//	Global resource dependence: 
//  Author:         Liangwen.Zhen
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetCompoundJpegCaptureParam(ISP_COMPOUND_JPEG_CAPTURE_PARAM_T * param_ptr);

/*****************************************************************************/
//  Description:    start capture compound JPEG
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStartCaptureCompoundJpeg(void);

/*****************************************************************************/
//  Description:    set MPEG capture parameter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetMpegCaptureParam(ISP_MPEG_PARAM_T * param_ptr);

/*****************************************************************************/
//  Description:    enter MPEG mode
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceStartCaptureMpeg(void);

/*****************************************************************************/
//  Description:    exit MPEG; or JPEG if necessary
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStopCapture(void);


/*****************************************************************************/
//  Description:    set scale parameter
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetScaleParam(ISP_SCLAE_PARAM_T * param_ptr);

/*****************************************************************************/
//  Description:    start isp to resize image
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStartScale(void);

/*****************************************************************************/
//  Description:    register the callback function to isp service
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ID_E eid, int (*func)(void *));

/*****************************************************************************/
//  Description:    unregister the callback function
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_ID_E eid);

/*****************************************************************************/
//  Description:    get current yuv frame 
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceGetFrameAddress(void);

/*****************************************************************************/
//  Description:    free current yuv frame address
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceFreeFrameAddress(ISP_FRAME_T *frame_ptr);

/*****************************************************************************/
//  Description:    Set ISP Clk
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceSetClk(uint32 clk);

/*****************************************************************************/
//  Description:    start video telephone path 
//	Global resource dependence: none
//  Author:         Baggio.he
//	Note:           
/*****************************************************************************/
void ISP_ServiceStartVT(void);

/*****************************************************************************/
//  Description:    set remote review address in YVU 3 planes 
//	Global resource dependence: none
//  Author:         Baggio.he
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetVTDecodeAddr(ISP_ADDRESS_T *addr_ptr);

/*****************************************************************************/
//  Description:    set remote review parameters
//	Global resource dependence: none
//  Author:         Baggio.he
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr);

/*****************************************************************************/
//  Description:    set local review parameters
//	Global resource dependence: none
//  Author:         Baggio.he
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);

uint32 ISP_ServiceGetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr);


/*****************************************************************************/
//  Description:    stop video telephone path 
//	Global resource dependence: none
//  Author:         Baggio.he
//	Note:           
/*****************************************************************************/
void ISP_ServiceStopVT(void);


/*****************************************************************************/
//  Description:    to query whether isp is in vt application 
//	Global resource dependence: none
//  Author:         Baggio.he
//	Note:           
/*****************************************************************************/
BOOLEAN ISP_ServiceIsVTOn(void);

typedef void (* lcddone_func_t)(uint32 callback_param); //callback when invalidate end
/*****************************************************************************/
//  Description:    create a thread and its resource,register the thread to system and active it.
//  Author:         Jianping.Wang
//	Note:           return 0 is success ,other is error
/*****************************************************************************/
PUBLIC int32 UpdateLcdCtrl_Init(uint32 thread_pori);
/**********************************************************************************************************/
// Description: register callback function 
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void UpdateLcdCtrl_RegisterNoticeDone(lcddone_func_t func);
/*****************************************************************************/
//  Description:    fmark callback
//  Author:         Jianping.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void UpdateLcdCtrl_FmartTrigger(void);
/*****************************************************************************/
//  Description:    invalidate lcd 
//  Author:         Jianping.Wang
//	Note:           return 0 is success,other is error!
/*****************************************************************************/
PUBLIC int32 LCD_InvalidateRectWithFmark(uint32 lcd_id,
											 uint32 block_id,
											 uint32 buf_adddr,
											 uint32 callback_param,
											 uint16 left,
						                                    uint16 top,
						                                    uint16 right, 	
                                     						 uint16 bottom);
/*****************************************************************************/
//  Description:    create a thread and its resource,register the thread to system and active it.
//  Author:         Jianping.Wang
//	Note:           return 0 is success ,other is error
/*****************************************************************************/
PUBLIC int32 UpdateLcdCtrl_Close(void);                                     						 


#endif//_ISP_SERVICE_H_


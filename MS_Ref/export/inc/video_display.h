/***************************************************************************************
** File Name:      video_display.h                                         		 			   *
** DATE:           2009/11/19                                                		   *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         		   *
** Description:    																       *
****************************************************************************************

****************************************************************************************
**                         Edit History                                       		   *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                               				   *
** 2009/11/19     	       Create.
****************************************************************************************/

#ifndef _VIDEO_DISPLAY_H_
#define _VIDEO_DISPLAY_H_


/**------------------------------------------------------------------------------------*
 **                         Dependencies           			                           *
 **------------------------------------------------------------------------------------*/


/**------------------------------------------------------------------------------------*
 **							Compiler Flag											   *
 **------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


typedef enum
{
    DISP_DATATYP_JPG,               // JPEG.
    DISP_DATATYP_YUV420_MCU,        // YUV420, MCU.
    DISP_DATATYP_YUV420_2PANNEL,    // YUV420, Y pannel, UV pannel.
    DISP_DATATYP_YUV420_3PANNEL,    // YUV420, Y pannel, U pannel,V pannel.
    DISP_DATATYP_YUV422_2PANNEL,    // YUV422, Y pannel, UV pannel.
    DISP_DATATYP_YUV422_3PANNEL,    // YUV422, Y pannel, U pannel,V pannel.
    DISP_DATATYP_RGB565             // RGB565.

}VIDEO_DISP_DATATYP_E;

typedef union
{
    struct
    {
        uint8* p_data; // data point for JPEG,MCU,RGB565.
        uint32 len;
    }onepannel;
    
    struct
    {
        uint8 *p_y;
        uint8 *p_uv;
    }yuv2pannel; // data point for yuv 2 pannels.
    
    struct
    {
        uint8 *p_y;
        uint8 *p_u;
        uint8 *p_v;
    }yuv3pannel; // data point for yuv 3 pannels.

}VIDEO_DISP_DATA_U;


typedef struct
{
    VIDEO_DISP_DATATYP_E data_type;     // data type.
    VIDEO_DISP_DATA_U    data;

    uint32 org_w;           // valid image width and height(trim from dec out frame).
    uint32 org_h;
    uint32 dec_out_w;       // data width and height,general 16pixels aligned.
    uint32 dec_out_h;
    int    is_transposed;   // the frame has been post-processed by decoder(1) or not(0).

}VIDEO_DISP_FRAME_T;

typedef struct
{
    VIDEO_DISP_DATATYP_E data_type;     // data type.
    //
    uint32 w;           // data width and height.general 16pixels aligned.
    uint32 h;
    //
    uint32 trim_x;      // trim offset,general 4pixels aligned.
    uint32 trim_y;
    uint32 trim_w;      // trim width and height, invalid image.
    uint32 trim_h;
    // other property of the frame.
    uint32 rotate_angle;    // clowise, 0-360 degree.
    uint32 mirror_type;     // 0: no mirror, 1: Horizontal mirror, 2: vertical mirror.
}VIDEO_PP_FRM_PARAM_T;

typedef struct
{
    VIDEO_DISP_DATA_U   data;

    VIDEO_PP_FRM_PARAM_T    param;
    
}VIDEO_PP_FRAME_T;


typedef struct
{
    uint32               x;
    uint32               y;	
    uint32               dx;
    uint32               dy;
}VIDEO_DISP_RECT_T;

typedef struct
{
	VIDEO_DISP_RECT_T disp_rect; //image & osd display area
    VIDEO_DISP_RECT_T target_rect; //image display area
	uint32  disp_mode;			// 0: full picture and remain the w/h rate.
								// 1: full display area and remain the w/h rate.
								// 2: full picture and full display area,w/h rate may be changed.

	uint32  RotateAngle;		// clockwise 0：0o； 1：90o； 2：180o； 3：270o；
	uint32  LcdId; 				//LCD ID；如果是TV_ID，则表示要tv-out。
	uint32  LcdBlkId;			//LCD block ID
}VIDEO_DISP_PARAM_T;


typedef int (* PF_VIDEO_PP_CALLBACK)(VIDEO_PP_FRAME_T *p_frame);



/*****************************************************************************/
// 	Description : open the display module.
//	input       : none
//  output      : SCI_SUCCESS of SCI_ERROR.
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_Display_Open(void);

/*****************************************************************************/
// 	Description : close the display module.
//	input       : none
//  output      : SCI_SUCCESS of SCI_ERROR.
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_Display_Close(void);

/*****************************************************************************/
// 	Description : set the display paramter,angle and rect in virtual window,
//                the paramter will be active when next frame is displaying.
//	input       : p_disp_param,new display param.
//  output      : SCI_SUCCESS of SCI_ERROR.
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_Display_Setparam(VIDEO_DISP_PARAM_T *p_disp_param);


/*****************************************************************************/
// 	Description : display one frame.
//	input       : org_w & org_h, origin width and height.
//                p_frame,the info of the dec_out frame.
//  output      : SCI_SUCCESS of SCI_ERROR.
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_Display_Oneframe(VIDEO_DISP_FRAME_T *p_frame);



/*****************************************************************************/
// 	Description : post_process one frame.
//	input       : 1. p_out_param, the postprocess parameter.
//                2. p_frame_in, input frame.
//	output      : p_frame_out, output frame.
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC int Video_PostProcess_Oneframe( VIDEO_PP_FRM_PARAM_T *p_out_param,
                                       VIDEO_PP_FRAME_T *p_frame_in,
                                       VIDEO_PP_FRAME_T *p_frame_out,
                                       PF_VIDEO_PP_CALLBACK pf_video_pp_calback
                                       );


/**------------------------------------------------------------------------------------*
 **							Compiler Flag											   *
 **------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif //_VIDEO_DISPLAY_H_


/******************************************************************************
 ** File Name:      lcd_diag.c                                                *
 ** Author:         Fei.Zhang                                             *
 ** DATE:           12/29/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of LCD   *
 ** 				Diag communication										  *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/29/2010  Fei.zhang       Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#include "ms_customize_trc.h"
#include "os_api.h"
#include "cmddef.h"
#include "lcd_diag.h"
#include "sio.h"
#include "diag.h"
#include "dal_lcd.h"
#include "pe_process.h"
#include "jpeg_interface.h"

#define DEBUG_LCD_DIAG
#ifdef  DEBUG_LCD_DIAG
	#define DIAG_TRACE   SCI_TRACE_LOW 
#else
	#define DIAG_TRACE( _format_string )  
#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

#ifdef _WIN32
#define PIXEL_RGB565_TO_RGB888(A,B,C,D,E)  do{ \
	(E) = (((A)&0x1f)<<3); \
	(D) = ((((A)>>5) | (((B)&0x7)<<3))<<2); \
	(C) = ((B)&0xF8);  \
	\
}while(0)
#else
#ifdef  CHIP_ENDIAN_LITTLE
#define PIXEL_RGB565_TO_RGB888(A,B,C,D,E)  do{ \
	(E) = (((A)&0x1f)<<3); \
	(D) = ((((A)>>5) | (((B)&0x7)<<3))<<2); \
	(C) = ((B)&0xF8);  \
	\
}while(0)
#else
#define PIXEL_RGB565_TO_RGB888(A,B,C,D,E)  do{ \
	(C) = ((A)&0xf8); \
	(D) = ((((B)>>5) | (((A)&0x7)<<3))<<2); \
	(E) = (((B)&0x1f)<<3);  \
	\
}while(0)
#endif
#endif

// RGB to YUV
//Y = 0.299 * r + 0.587 * g + 0.114 * b;
#define RGB_TO_Y(_r, _g, _b)    (uint8)((77 * (_r) + 150 * (_g) + 29 * (_b)) >> 8)
//U = 128 - 0.1687 * r - 0.3313 * g + 0.5 * b 
#define RGB_TO_U(_r, _g, _b)    (uint8)(128 + ((128 * (_b) - 43 * (_r) - 85 * (_g)) >> 8))
//V = 128 + 0.5 * r - 0.4187 * g - 0.0813 * b 
#define RGB_TO_V(_r, _g, _b)    (uint8)(128  + ((128 * (_r) - 107 * (_g) - 21 * (_b)) >> 8))

#define SEND_DATA_SIZE  (32*1024)
#define ADDR_OFFSET     (64)
#define USE_JPEG        1
#define JPEG_ENC_TMP_BUF_SIZE  (72*1024)

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void Rgb565ToChannel_Y_UV_422(uint8 *yuv_buf_ptr, uint8 *rgb_buf_ptr, uint32 in_width, uint32 in_height)
{    
	uint32 	half_in_width = in_width >> 1;
	uint32	out_width = in_width + (in_width & 1);
	uint32 	i = 0, j = 0;
	uint8  	*y_ptr = yuv_buf_ptr;
	uint8	*uv_ptr = yuv_buf_ptr + out_width * in_height;
	int32 	r = 0, g = 0, b = 0;

    for (i=0; i<in_height; i++)
    {
    	for (j=0; j<half_in_width; j++)
    	{
	        uint8 data0 = *rgb_buf_ptr++;
			uint8 data1 = *rgb_buf_ptr++;
			uint8 u1, u2, v1, v2;

			PIXEL_RGB565_TO_RGB888(data0, data1, r, g, b);
	        *y_ptr++ = RGB_TO_Y(r, g, b);
	        u1 = RGB_TO_U(r, g, b);
			v1 = RGB_TO_V(r, g, b);
			
			data0 = *rgb_buf_ptr++;
			data1 = *rgb_buf_ptr++;
			PIXEL_RGB565_TO_RGB888(data0, data1, r, g, b);			
	        *y_ptr++ = RGB_TO_Y(r, g, b);
	        u2 = RGB_TO_U(r, g, b);
			v2 = RGB_TO_V(r, g, b);		

			*uv_ptr++ = (u1 + u2) >> 1;
			*uv_ptr++ = (v1 + v2) >> 1;
    	}

		if (1 == (in_width & 1))
		{
	        uint8 data0 = *rgb_buf_ptr++;
			uint8 data1 = *rgb_buf_ptr++;	

			PIXEL_RGB565_TO_RGB888(data0, data1, r, g, b);
	        *y_ptr++ = RGB_TO_Y(r, g, b);
	        *uv_ptr++ = RGB_TO_U(r, g, b);	
	        *y_ptr++ = RGB_TO_Y(r, g, b);
			*uv_ptr++ = RGB_TO_V(r, g, b);			
		}
    }

}

void RGB5656ToYUV(uint8  *src_ptr,uint8  *dst_ptr,uint16 width,uint16 height)
{

	//DIAG_TRACE:"PE_RGBToYUV:src=0x%x,dst=0x%x,width=%d,height=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_151_112_2_18_0_33_25_1733,(uint8*)"dddd", src_ptr,dst_ptr,width,height);

	Rgb565ToChannel_Y_UV_422(dst_ptr, src_ptr,  width, height);
}

uint32 YUVToJPG(uint8  *src_ptr,	uint8  *dst_ptr,uint16 width,uint16 height,uint32 dst_buf_size)
{
		JINF_ENC_IN_PARAM_T		enc_in_param = {0};
	       JINF_ENC_OUT_PARAM_T	enc_out_param = {0};
		JINF_RET_E				jinf_ret = JINF_FAILED;		
		uint32 ret = 0;
        uint8 *temp_ptr = 0;
		   
		enc_in_param.quality = JINF_QUALITY_HIGH;
		enc_in_param.src_width = width;
		enc_in_param.src_height = height;
		enc_in_param.target_buf_ptr = dst_ptr;
		enc_in_param.target_buf_size = dst_buf_size;
#ifdef JPG_ENC_SUPPORT_JPG_HW
		enc_in_param.yuv_info.data_endian = JINF_DATA_BIG_ENDIAN;
		enc_in_param.yuv_info.sample_format = JINF_FORMAT_YUV422;
		enc_in_param.yuv_info.data_pattern = JINF_YUV_PATTERN_CHN_Y_UV;
		enc_in_param.yuv_info.yuv_chn.y_chn_ptr = src_ptr;
		enc_in_param.yuv_info.yuv_chn.u_chn_ptr = src_ptr+ (width + (width & 1)) * height;
#else
		enc_in_param.yuv_info.data_endian = JINF_DATA_BIG_ENDIAN;
		enc_in_param.yuv_info.sample_format = JINF_FORMAT_YUV444;
		enc_in_param.yuv_info.data_pattern = JINF_YUV_PATTERN_SEQUENTIAL;
		enc_in_param.yuv_info.yuv_chn.y_chn_ptr = src_ptr;
#endif		
        temp_ptr = (uint8*)SCI_ALLOCA(JPEG_ENC_TMP_BUF_SIZE);
        if(!temp_ptr)
        {
            //DIAG_TRACE:"---PE_PROCESS:YUVToJPG malloc is fail!---"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_184_112_2_18_0_33_25_1734,(uint8*)"");
            return 0;
        }
		enc_in_param.encode_buf_ptr = temp_ptr;
		enc_in_param.encode_buf_size = JPEG_ENC_TMP_BUF_SIZE;

		jinf_ret = IMGJPEG_Encode(&enc_in_param, &enc_out_param);

            SCI_FREE(temp_ptr);
			
		if(JINF_SUCCESS == jinf_ret)
		{
			//DIAG_TRACE:"[pe_process.c] SaveJpeg successed! "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_196_112_2_18_0_33_26_1735,(uint8*)"");
			ret = enc_out_param.jpeg_buf_size;
		}	
		else
		{
			ret = 0;
		}	
		
	return ret;
}

/*****************************************************************************/
//  Description:    Handle read main buffer request
//	Global resource dependence:
//  Author:  Jianping.wang       
//	Note:
/*****************************************************************************/ 
LOCAL uint32 _savejpg(uint8  *src_ptr,	uint8  *dst_ptr,uint16 width,uint16 height,uint32 dst_buf_size,uint32 offset_width)
{
	uint32 ret = 0;
    uint8 *temp_ptr = 0;
       
    temp_ptr = dst_ptr;
    temp_ptr += ADDR_OFFSET*offset_width;
	RGB5656ToYUV(src_ptr,temp_ptr,width,height);
	     
	ret = YUVToJPG( temp_ptr,dst_ptr,width,height,dst_buf_size);	

	return ret;
}

LOCAL uint32 _LCD_DIAG_CmdRoutine( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
	uint16 *main_buf_ptr = PNULL;
	LCD_INFO_T lcd_info = {0};
	uint32 size = 0;
	uint32 sum = 0;
	uint32 seqnum = 0;	
	uint32 packet_size = 0;
	uint8 *temp_ptr = 0;
    uint16 *save_buf_ptr = 0;

	//DIAG_TRACE:"---HandleReadMainBuffer in---time =%d-"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_241_112_2_18_0_33_26_1736,(uint8*)"d",SCI_GetTickCount());

	LCD_GetInfo(LCD_ID_0,&lcd_info);

	main_buf_ptr = (uint16*)lcd_info.lcdbuff_ptr;	
	packet_size = (lcd_info.lcd_width*lcd_info.lcd_height)<<1;	

	*dest_ptr = SCI_ALLOCA(sizeof(MSG_HEAD_T)+packet_size+(ADDR_OFFSET*lcd_info.lcd_width));
    
	if(*dest_ptr == NULL)
	{
		*dest_len_ptr = 0;
		//DIAG_TRACE:"HandleReadMainBuffer:malloc failed!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_253_112_2_18_0_33_26_1737,(uint8*)"");
		return;
	}     	
	temp_ptr = *dest_ptr;
	memcpy(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T)); 
    
	#if USE_JPEG	 
    save_buf_ptr = (uint16*)SCI_ALLOCA(packet_size);
    if(!save_buf_ptr)
    {
        *dest_len_ptr = 0;
        SCI_FREE(*dest_ptr);
        //DIAG_TRACE:"HandleReadMainBuffer:malloc failed1!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_265_112_2_18_0_33_26_1738,(uint8*)"");
        return;
    }
    SCI_MEMCPY(save_buf_ptr,main_buf_ptr,packet_size);   
    size = _savejpg((uint8*)save_buf_ptr,(uint8*)((*dest_ptr)+sizeof(MSG_HEAD_T)),lcd_info.lcd_width,lcd_info.lcd_height,packet_size,lcd_info.lcd_width);         
    memcpy(temp_ptr, src_ptr, sizeof(MSG_HEAD_T));     
    SCI_FREE(save_buf_ptr);
    save_buf_ptr = 0;  
    #else
    SCI_MEMCPY((*dest_ptr+sizeof(MSG_HEAD_T)),main_buf_ptr,packet_size);
    size = packet_size;
    #endif
	//DIAG_TRACE:"HandleReadMainBuffer:size=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_277_112_2_18_0_33_26_1739,(uint8*)"d",size);
	if(size!=0)
       {
		 sum = 0;
		 seqnum = 1;
		 
		while(sum<size)
		{
		       if((size-sum)>SEND_DATA_SIZE)
		       {
				packet_size = SEND_DATA_SIZE;
				if(0 == sum)
				{
					((MSG_HEAD_T *) temp_ptr)->subtype = (size+SEND_DATA_SIZE-1)/SEND_DATA_SIZE | 0x80;
					seqnum++;
				}
				else
				{
					((MSG_HEAD_T *) temp_ptr)->subtype = seqnum++;
				}
		       }
			else
			{
				packet_size = size -sum;
				if(0 == sum)
				{
					((MSG_HEAD_T *) temp_ptr)->subtype = 0x81;
				}
				else
				{
					((MSG_HEAD_T *) temp_ptr)->subtype = 0;
				}
			}
			 
			((MSG_HEAD_T *) temp_ptr)->len = sizeof(MSG_HEAD_T) + packet_size;		
			*dest_len_ptr = sizeof(MSG_HEAD_T) + packet_size;	
			((MSG_HEAD_T *) temp_ptr)->type = DIAG_READ_LCD_BUFFER;	  
			sum += packet_size;			
		//	DIAG_TRACE("HandleReadMainBuffer: send len = %d,temp_ptr=0x%x",*dest_len_ptr,temp_ptr);
			SIO_SendPacket((void*)temp_ptr,*dest_len_ptr);
			temp_ptr += packet_size;
			memcpy(temp_ptr, src_ptr, sizeof(MSG_HEAD_T)); 
			
		}
	 }
	 
	*dest_len_ptr = 0;
    SCI_FREE(*dest_ptr);
    //DIAG_TRACE:"---HandleReadMainBuffer out---time =%d-"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_325_112_2_18_0_33_26_1740,(uint8*)"d",SCI_GetTickCount());

    return 1;	
}

/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is register diag command routine
//  Author:         Fei.Zhang
//  Note:           
/*****************************************************************************/
PUBLIC void LCD_DIAG_RegDiagCmdRoutine(void)
{    
    //DIAG_TRACE:"LCD: LCD_DIAG_RegDiagCmdRoutine"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_DIAG_337_112_2_18_0_33_26_1741,(uint8*)"" );
	
    DIAG_RegisterCmdRoutine(DIAG_READ_MAINBUF_F, _LCD_DIAG_CmdRoutine);
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of lcd_diag.c

/******************************************************************************
 ** File Name:      IMG.h                                                     *
 ** Author:         Zhemin.Lin                                                *
 ** DATE:           2004/05/11                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Image manager application interface                       *
 ** Note:           None                                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2004/05/11     Zhemin.Lin       Create.                                   *
 ******************************************************************************/

#ifndef DAL_IMG_H
#define DAL_IMG_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "sci_types.h"
#include "img_ref_types.h" 

/*return value*/
#define IMG_SUCCESS             0
#define IMG_FAILED                1//(-1)


/*error id*/
#define IMG_EID_NONE            0
#define IMG_EID_IMGTYPE         2
#define IMG_EID_FILEREAD        3
#define IMG_EID_FILEWRITE       4
#define IMG_EID_FILEOPEN        5
#define IMG_EID_MEMALLOC        6
#define IMG_EID_TBLOVERFLOW     7
#define IMG_EID_HASCOMPRESS     8
#define IMG_EID_NOTCOMPRESS     9
#define IMG_EID_INVALIDFRAMENUM 10
#define IMG_EID_INVALIDPARA     11
#define IMG_EID_NOTEXIST        12
#define IMG_EID_PIXELFORMAT		13

/*back ground type*/
typedef enum
{
	IMG_BG_NORMAL = 0,   
	IMG_BG_COMPOUND  
	
}IMG_RES_BG_TYPE_E;

/*back ground type*/
typedef enum
{
	RES_BG_TYPE1 = 0,     //IMG_BG_NORMAL = 0,   
	RES_BG_TYPE2            //IMG_BG_COMPOUND  
	
}IMG_RES_DEC_BG_TYPE_E;

typedef enum
{
	RES_IMG_TYPE1 = 0,     // IMGREF_FORMAT_RGB565
	RES_IMG_TYPE2            //IMGREF_FORMAT_ARGB888
	
}IMG_RES_DEC_IMG_TYPE_E;

/*region structure*/
typedef struct
{
    uint16 top;
    uint16 left;
    uint16 right;
    uint16 bottom;  
}IMG_RECT_T;

/*display device info*/
typedef struct
{
    uint32	r_bitmask;
    uint32  g_bitmask;
    uint32 	b_bitmask;
    uint16  width;
    uint16  height;
    uint8   *dispmem;
    uint16  bits_per_pixel;
    uint16  padding;
}IMG_DISPCTL_T;

/*display device info*/
typedef struct
{
    IMG_RES_BG_TYPE_E  bg_type;
    IMGREF_DATA_FORMAT_E  format;	
    uint16  width;
    uint16  height;
    void  *dstmem;
}IMG_RES_DST_CTL_T;

/*image info*/
typedef struct
{
    uint16  bits_per_pixel;	/*bit count per pixel*/
    uint16	used_palette;	/*if 0, don't use palette*/
    uint32	palette_addr;	/*if used palette, it refer to the palette start addr in the stream*/
    uint32	r_bitmask;		/*the bit mask of r color, if don't use palette*/
    uint32  g_bitmask;		/*the bit mask of g color, if don't use palette*/
    uint32 	b_bitmask;		/*the bit mask of b color, if don't use palette*/
    uint16  width;			/*image widht*/
    uint16  height;			/*image height*/
    uint16	framenum;		/*frame number, for bmp image, it always set 1*/
    uint16  padding;
}IMG_INFO_T;

typedef enum
{  
    IMG_RES_SUCCESS = 0,
	IMG_RES_UNKNOWN_FILE_TYPE ,			
	IMG_RES_MEMORY_NOT_ENOUGH ,
		
	IMG_RES_BAD_FILE_DATA,
	IMG_RES_FAIL
		
}IMG_RES_ERROR_E;

typedef enum
{  
    IMG_RES_BMP = 0,			//BMP
	IMG_RES_JPEG,				//±ê×¼JPEG
	IMG_RES_SJPEG,			//Spreadtrum private JPEG
	IMG_RES_PNG,				//PNG
	IMG_RES_GIF,				//GIF, with GIF Resource Header
	IMG_RES_ARGB,                   //copressed ARGB
	IMG_RES_GIF_NORMAL,      //standard GIF, No GIF Resource Header	
	IMG_RES_MOV_ARGB,                   //MOV compressed ARGB
	
	IMG_RES_MAX = 16
		
}IMG_RES_TYPE_E;

typedef enum
{
	IMG_COLOR_NORMAL = 0,   
	IMG_COLOR_EXCHANGE  
	
}IMG_RES_COLOR_TYPE_E;

typedef struct 
{  
    uint8* imgstream_ptr;    
    uint32  imgstreamsize;
    IMG_RECT_T imgrect;

    IMG_RES_COLOR_TYPE_E color_type;     
    uint32 color_value;
	
}IMG_RES_SRC_T;

typedef struct
{
    uint16  width;			/*image widht*/
    uint16  height;			/*image height*/
    uint16  framenum;		/*frame number, for bmp image, it always set 1*/
    uint16  frame_index;     /*MOV Index will be played*/ 
    uint16  interval; /*MOV Interval */ 
    uint16  pading;     
    IMG_RES_TYPE_E img_type;		/*image type*/
	IMG_RES_COLOR_TYPE_E color_type;    //color_exchange
	uint32 color_value;  //color change value	
}IMG_RES_INFO_T;

typedef struct 
{  
    IMG_RES_DST_CTL_T   *dstctl;
    IMG_RECT_T      dstrect;
	
}IMG_RES_DST_T;

typedef struct
{
	uint16 frame_index;  
	uint16 interval; //ms;

	BOOLEAN is_decode_finish;  //TRUE: Decode all 
	uint8   padding0;//reserved
	uint8   padding1;//reserved
	uint8   padding2;//reserved
}IMG_RES_MOV_FRAME_T;

typedef struct
{
    uint32 handler;
    IMG_RECT_T imgrect;
    //IMG_RES_DISP_T img_disp;
    IMG_RES_DST_T img_dst;	
    uint32 frame_index;
    BOOLEAN is_decode_cur;  //for gif get current frame
    uint8   padding0;//reserved
	uint8   padding1;//reserved
	uint8   padding2;//reserved

    IMG_RES_COLOR_TYPE_E color_type;    //color_exchange
    uint32 color_value;  //color change value
}IMG_RES_DST_MOV_INFO_T;


#define FLAG_ARGB888  0x51  
#define FLAG_RGB565   0X50  

typedef struct
{
	uint16 x;
	uint16 y;
	uint16 width;
	uint16 height;    
	uint16* bg_buf_ptr;
}BG_INFO_T;


typedef struct
{
	uint16 width;
	uint16 height;
	uint32 color_app;
	uint16 x1, y1, x2, y2;	
	uint16 normal_color;
	uint16 alpha_color;
	uint16 *color_table565_ptr;
	uint8 *color_tablealpha_ptr;	
       uint8 *color_table888_ptr;
	uint32 flag_argb_rgb565; 

      IMG_RES_COLOR_TYPE_E color_type;
	uint32 color_value;
	uint32 imgstream_size;
}ARGB_info_t;


typedef struct
{
	uint32 	*dst_ptr;
	uint32 	*width;
	uint8	*pcodedata;
	uint8   *datastream_ptr;
	uint32 	*codesize;
	uint32  bpp;
	uint32  *bits_left;
	uint32  *bits_buf;
	uint32  *marksize;	 
	uint32  *colortable_argb_ptr;
	uint16 *l_rgb565_ptr;	
	uint8 *l_argb888_ptr;
	uint8 *color_tablealpha_ptr;
	uint32 flag_argb_rgb565; 
	uint32   normal_color;
	uint32  alpha_color;
	uint32  left;
	uint32  right;

}IMG_RLE_INFO_T;


typedef uint32 (*DEC_FUNC_PTR)(uint8 *,uint32,const uint8 *,uint32*,uint32);
typedef uint32 (*DEC_PART_FUNC_PTR)(uint8 *,uint32,uint32,uint32,const uint8 *,uint32*,uint32);

typedef enum
{  
	RES_DEC_TYPE1 =0,     //BMP_DEC_8BIT_TO_RGB565 = 0,		
	RES_DEC_TYPE2,          //BMP_DEC_8BIT_TO_RGB888,			
	RES_DEC_TYPE3,         //BMP_DEC_8BIT_TO_ARGB888,			
	RES_DEC_TYPE4,        //BMP_DEC_16BIT_TO_RGB565,			
	RES_DEC_TYPE5, //BMP_DEC_16BIT_TO_RGB888,	
	RES_DEC_TYPE6, //BMP_DEC_16BIT_TO_ARGB888,
	RES_DEC_TYPE7, //BMP_DEC_24BIT_TO_RGB565,	
	RES_DEC_TYPE8, //BMP_DEC_24BIT_TO_RGB888,
	RES_DEC_TYPE9, //BMP_DEC_24BIT_TO_ARGB888,
	
	BMP_DEC_TYPE_UNKNOWN     	

}BMP_DEC_TYPE_E;


typedef struct bmp_decoder_tab_tag
{
	BMP_DEC_TYPE_E img_type;
    DEC_FUNC_PTR  decoder_func_ptr ;
    DEC_PART_FUNC_PTR  decoder_part_func_ptr;
}BMP_DECODER_TAB_T;


//argb_res_decode.h

typedef uint32 (*ALPHA_DEC_FUNC_PTR)(ARGB_info_t *argb_info_ptr, uint16 *src_ptr, BG_INFO_T* bg_info_ptr ,    IMG_RECT_T imgrect);

typedef uint32 (*ARGB_RLE_DecFun_Ptr)(IMG_RLE_INFO_T *);

typedef struct alpha_res_dec_tab_tag
{
	uint32 img_type;
	IMGREF_DATA_FORMAT_E img_format;
	IMG_RES_BG_TYPE_E bg_format;	
	ALPHA_DEC_FUNC_PTR  decoder_func_ptr ;	
}ALPHA_RES_DEC_TAB_T;

typedef struct argb_rle_dec_tab_tag
{
	//uint32 img_type;
	IMGREF_DATA_FORMAT_E img_format;
	IMG_RES_BG_TYPE_E bg_format;	
	ARGB_RLE_DecFun_Ptr  decoder_func_ptr ;	
}ARGB_RLE_DEC_TAB_T;


/*****************************************************************************/
//  Description :
//      try to display an bmp on a specified display device.
//  Author:
//      Zhemin.Lin
//  Input:
//      bmpstream	bmp stream
//      bmprect		region of bmp to be display
//		pdispctl	display controller
//		disprect	region to be display on
//  Return:
//      0       successful
//      others  failed
//  Note:
//      None
/*****************************************************************************/ 
uint32 IMG_DisplayBmp(
    const uint8     *bmpstream,
    IMG_RECT_T       *bmprect,
    IMG_DISPCTL_T   *pdispctl,
    IMG_RECT_T       *disprect
    );

/*****************************************************************************/
//  Description :
//      enable the transparent color, if enable, the 0 color represent
//		transparent color
//  Author:
//      Zhemin.Lin
//  Input:
//		enable	if 0, represent disable
//  Return:
//		void
//  Note:
//      None
/*****************************************************************************/
void IMG_EnableTransparentColor(uint32 enable);						
/******************************************************************************/
//  Description: Get Resource image Information
//  Author:  
//  Input:      
// 		    img_src_ptr:    source resource buffer; include : img butter; img size and img rect 

// 		    
// 		  
//  Output:  img_info_ptr:    img information buffer
//  Return:  Success: IMG_RES_SUCCESS
 //              Failure:        other
//  Note:       
/******************************************************************************/
IMG_RES_ERROR_E IMG_RES_GetImageInfo(
                                              const IMG_RES_SRC_T  *img_src_ptr, 
                                              IMG_RES_INFO_T *img_info_ptr
                                             );
/******************************************************************************/
//  Description: Display resouce image on some memory
//  Author:  
//  Input:      
// 		    img_src_ptr:    source resource buffer; include : img butter; img size and img rect 
//		    img_info_ptr:    img information buffer
// 		    img_disp_ptr:   display information buffer
// 		  
// 
//  Return:  Success: IMG_RES_SUCCESS
 //              Failure:        other
//  Note:       
/******************************************************************************/				
IMG_RES_ERROR_E IMG_RES_Display_Img(
                                            const IMG_RES_SRC_T  *img_src_ptr, 
                                            IMG_RES_DST_T *img_dst_ptr 
                                           );	
/******************************************************************************/
//  Description: Create Mov display Handler
//  Author:  
//  Input:      
// 		    img_src_ptr:    source resource buffer; include : img butter; img size and img rect 
//		    img_type:       image type
// 		    handler_ptr:   Handler Pointer
// 		  
// 
//  Return:  Success: IMG_RES_SUCCESS
 //              Failure:        other
//  Note:       
/******************************************************************************/
IMG_RES_ERROR_E IMG_RES_Create_Handler(
                                                const IMG_RES_SRC_T  *img_src_ptr, 
                                                uint32 *handler_ptr
                                               );
/******************************************************************************/
//  Description: Free Mov display Handler
//  Author:  
//  Input:      
//		 
// 		    handler_ptr:   Handler wil be free
// 		  
// 
//  Return:  Success: IMG_RES_SUCCESS
 //              Failure:        other
//  Note:       
/******************************************************************************/
IMG_RES_ERROR_E IMG_RES_Free_Handler(uint32 handler);
/******************************************************************************/
//  Description: Play One frame to buffer
//  Author:  
//  Input:      
// 		    img_src_ptr:    source resource buffer; include : img butter; img size and img rect 
//		    img_info_ptr:    img information buffer
// 		    handler_ptr:   Handler Pointer	  
// 
//  Return:  Success: IMG_RES_SUCCESS
//              Failure:        other
//  Note:     IMG_RES_Display_MOV Depend on IMG_RES_Create_Handler and IMG_RES_Free_Handler
/******************************************************************************/
IMG_RES_ERROR_E IMG_RES_Display_MOV(
                                                IMG_RES_DST_MOV_INFO_T *mov_info_ptr,  
                                                IMG_RES_MOV_FRAME_T *frame_info_ptr
                                            );

/*****************************************************************************/
//  Description :
//      get image info
//  Author:
//      Zhemin.Lin
//  Input:
//		stream		image stream
//		imge_info	image info struct
//  Return:
//		error ID
//  Note:
//      None
/*****************************************************************************/
uint32 IMG_GetImageInfo(
                            	const uint8 *stream,
                            	IMG_INFO_T *img_info
                             );

void *_memset(void *buffer, uint32 value, uint32 size);


//bmp_decode.c
PUBLIC uint32 RES_Dec_Bmp_Fun1(
	uint8 	*pline,
	uint32 	width,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32 usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun2(
	uint8 	*pline,
	uint32 	width,	
	uint32  left,
    uint32  right,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun3(
	uint8 	*pline,
	uint32 	width,	
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32 usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun4(
	uint8 	*pline,
	uint32 	width,
	uint32  left,
    uint32  right,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun5(
	uint8 	*pline,
	uint32 	width,	
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32 usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun6(
	uint8 	*pline,
	uint32 	width,
	uint32  left,
    uint32  right,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun7(
	uint8 	*pline,
	uint32 	width,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun8(
                                            uint8   *pline,
                                            uint32  width,
                                            uint32  left,
                                            uint32  right,
                                            const uint8 *pcodedata,
                                            uint32  *codesize,
                                            uint32  usedkeycolor
                                            );

PUBLIC uint32 RES_Dec_Bmp_Fun9(
	uint8 	*pline,
	uint32 	width,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun10(
                                            uint8   *pline,
                                            uint32  width,
                                            uint32  left,
                                            uint32  right,
                                            const uint8 *pcodedata,
                                            uint32  *codesize,
                                            uint32  usedkeycolor
                                            );

PUBLIC uint32 RES_Dec_Bmp_Fun11(
                                            uint8   *pline,
                                            uint32  width,                                           
                                            const uint8 *pcodedata,
                                            uint32  *codesize,
                                            uint32  usedkeycolor
                                            );

PUBLIC uint32 RES_Dec_Bmp_Fun12(
                                            uint8   *pline,
                                            uint32  width,
                                            uint32  left,
                                            uint32  right,
                                            const uint8 *pcodedata,
                                            uint32  *codesize,
                                            uint32  usedkeycolor
                                            );

PUBLIC uint32 RES_Dec_Bmp_Fun13(
	uint8 	*pline,
	uint32 	width,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun14(
	uint8 	*pline,
	uint32 	width,
	uint32  left,
    uint32  right,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun15(
	uint8 	*pline,
	uint32 	width,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun16(
	uint8 	*pline,
	uint32 	width,
	uint32  left,
    uint32  right,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun17(
	uint8 	*pline,
	uint32 	width,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);

PUBLIC uint32 RES_Dec_Bmp_Fun18(
	uint8 	*pline,
	uint32 	width,
	uint32  left,
    uint32  right,
	const uint8	*pcodedata,
	uint32 	*codesize,
	uint32  usedkeycolor
	);



//argb_res_decode.c
PUBLIC void RES_Dec_Argb_Fun7(IMG_RLE_INFO_T *dec_info_ptr)    ;

PUBLIC void RES_Dec_Argb_Fun9(IMG_RLE_INFO_T *dec_info_ptr)    ;

PUBLIC void RES_Dec_Argb_Fun8(IMG_RLE_INFO_T *dec_info_ptr)  ;

PUBLIC void RES_Dec_Argb_Fun10(IMG_RLE_INFO_T *dec_info_ptr)    ;

PUBLIC uint32 RES_Dec_Argb_Fun1(ARGB_info_t *argb_info_ptr, uint16 *src_ptr, BG_INFO_T* bg_info_ptr ,    IMG_RECT_T imgrect);

PUBLIC uint32 RES_Dec_Argb_Fun2(ARGB_info_t *argb_info_ptr, uint16 *src_ptr, BG_INFO_T* bg_info_ptr ,    IMG_RECT_T imgrect);

PUBLIC uint32 RES_Dec_Argb_Fun3(ARGB_info_t *argb_info_ptr, uint16 *src_ptr, BG_INFO_T* bg_info_ptr ,    IMG_RECT_T imgrect);

PUBLIC uint32 RES_Dec_Argb_Fun4(ARGB_info_t *argb_info_ptr, uint16 *src_ptr, BG_INFO_T* bg_info_ptr ,    IMG_RECT_T imgrect);

PUBLIC uint32 RES_Dec_Argb_Fun5(ARGB_info_t *argb_info_ptr, uint16 *src_ptr, BG_INFO_T* bg_info_ptr ,    IMG_RECT_T imgrect);

PUBLIC uint32 RES_Dec_Argb_Fun6(ARGB_info_t *argb_info_ptr, uint16 *src_ptr, BG_INFO_T* bg_info_ptr ,    IMG_RECT_T imgrect);



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif //IMG

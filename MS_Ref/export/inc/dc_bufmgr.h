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

 #ifndef _MM_BUFMGR_H_
#define _MM_BUFMGR_H_

#ifdef __cplusplus
	extern "C"
	{
#endif

#include "sci_types.h"

#define JPEG_SLICE_BUF_LEN(width)				(width*2*32)

#define MM_PRINT   SCI_TRACE_LOW

#define MM_MIN_BUF_SIZE_1280x960					(1000*1024)
#define MM_MIN_BUF_SIZE_1600x1200				(1400*1024)
#define MM_MIN_BUF_SIZE_2048x1536				(2000*1024)

#define MM_MAX_YUV_BUF_SIZE_640x480			(640*480*2)		
#define MM_MAX_YUV_BUF_SIZE_800x600			(800*600*2)		
#define MM_MAX_YUV_BUF_SIZE_1024x768			(1024*768*2)	
#define MM_MAX_YUV_BUF_SIZE_1280x1024			(1280*1024*2)		
#define MM_MAX_YUV_BUF_SIZE_1600x1200			(1600*1200*2)		
#define MM_MAX_YUV_BUF_SIZE_2048x1536			(2048*1536*2)		

#define MM_SNAPSHOT_YUV_OFFSET_1280x960		(150*1024)	//FROM 128K to 64K	// 970k - (600k + 30k +32k + 80k)
#define MM_SNAPSHOT_YUV_OFFSET_1600x1200		(150*1024)				// 1100k - (938k + 30k + 32k + 100k)		
#define MM_SNAPSHOT_YUV_OFFSET_2048x1536		(68*1024)		// 1800k - (1536k + 30k +32k + 128k)
#define MM_SNAPSHOT_YUV_OFFSET					(64*1024)

//Calc according to REVIEW case
#define MM_MAX_JPEG_BUF_SIZE_1280x960			(500*1024)			//970K - 188K(OSD) - 188K(RGB) - (30+20+32)K(JPEG DEC)
#define MM_MAX_JPEG_BUF_SIZE_1600x1200			(640*1024)			//1100K - 188K(OSD) - 188K(RGB) - (30+20+32)K(JPEG DEC)
#define MM_MAX_JPEG_BUF_SIZE_2048x1536			(1300*1024)		//1800K - 188K(OSD) - 188K(RGB) - (30+20+32)K(JPEG DEC)

#define MM_JPEGE_TEMP_BUF_SIZE_1280x960			(1280*32*2)		//32 line
#define MM_JPEGE_TEMP_BUF_SIZE_1600x1200		(1600*32*2)		//32 line
#define MM_JPEGE_TEMP_BUF_SIZE_2048x1536		(2048*32*2)		//32 line

#define MM_JPEGE_TEMP_BS_BUF_LEN				(30*1024)			// FW buffer + BitStream ping-pong output buffers

#define MM_JPEG_FW_BUF_LEN						(32*1024)		// firmware buffer

#define MM_JPEG_INTERPLATION_SWAP_LEN	              (5*1600)		// 
#define MM_JPEG_VGA_UXGA_LEN						(250*1024)		// 250K w*h*2*2
#define MM_PANORAMA_STAT_BUF_LEN		(3*1024)

#define MM_PANORAMA_YUV_BUF_LEN		(480*1024)
#define MM_PANORAMA_FRAME_YUV_BUF_LEN		(320*240*2)
#define MM_FOURinONE_YUV_BUF_LEN		(450*1024)
#define MM_FOURinONE_JPG_BUF_LEN		(300*1024)
#define MM_FOURinONE_FRAME_YUV_BUF_LEN		(320*240*2)
#define MM_BURSTSHOT_FRAME_YUV_BUF_LEN		(320*240*2)

#define MM_QVGA_JPEG_SIZE_MAX					(50*1024)
#define MM_BURSTSHOT_COUNT_MAX				(9)

#define MM_JPEGD_SCALING_BUF_LEN				(20*1024)
#define MM_JPEGD_TEMP_BUF_LEN					(32*1024)

#ifdef SRAM_SIZE_32MBIT
#define MM_MJPEG_FRAME_BUF_LEN				(150*1024+512)
#else
#define MM_MJPEG_FRAME_BUF_LEN				(150*1024+512)*3
#endif
#define MM_DV_MUX_BUF_LEN						(200*1024)

#define MM_DV_DEMUX_BUF_LEN					(920*1024)

#define MM_JPEG_DEC_BUF_LEN						(80*1024)
#define MM_JPEG_ENC_BUF_LEN						(33*1024)

#define MM_CASE_BLOCK_COUNT_MAX				(16)

typedef enum
{
	MM_DC_PREVIEW=0x0,
	MM_DC_PREVIEW_DISP,
	MM_DC_PREVIEW_ROT,

	MM_DC_CAPTURE=0x10,
	MM_DC_CAPTURE_EXIF,
	MM_DC_CAPTURE_THUMBNAIL,
	MM_DC_CAPTURE_JPEG,
	MM_DC_CAPTURE_YUV,
	MM_DC_CAPTURE_MINATURE_SRC,
	MM_DC_CAPTURE_MINATURE_SWAP,
	MM_DC_CAPTURE_QUICK_VIEW,
	MM_DC_CAPTURE_SWAP,
	MM_DC_CAPTURE_JPEG_FW,
	MM_DC_CAPTURE_DISP,
	MM_DC_CAPTURE_ROT,
	MM_DC_CAPTURE_EXIF_SWAP,

	MM_DC_PANORAMA=0x20,
	MM_DC_PANORAMA_JPEG,
	MM_DC_PANORAMA_STAT,
	MM_DC_PANORAMA_YUV,
	MM_DC_PANORAMA_PREVIEW,
	MM_DC_PANORAMA_QUICK_VIEW,
	MM_DC_PANORAMA_JPEG_ENC,
	
	MM_DC_FOURinONE=0x30,
	MM_DC_FOURinONE_JPEG,
	MM_DC_FOURinONE_OSD,
	MM_DC_FOURinONE_YUV,
	MM_DC_FOURinONE_PREVIEW,
	MM_DC_FOURinONE_QUICK_VIEW,
	MM_DC_FOURinONE_SWAP,	
	MM_DC_FOURinONE_JPEG_ENC,
	
	MM_DC_REVIEW= 0x40,
	MM_DC_REVIEW_YUV,
	MM_DC_REVIEW_FW,
	MM_DC_REVIEW_DISP,
	MM_DC_REVIEW_ROT,

	MM_DC_CONTINUESHOT=0x50,
	MM_DC_CONTINUESHOT_EXIF,
	MM_DC_CONTINUESHOT_JPEG,
	MM_DC_CONTINUESHOT_YUV,
	MM_DC_CONTINUESHOT_DISP,
	MM_DC_CONTINUESHOT_SWAP,
	MM_DC_CONTINUESHOT_JPEG_FW,
	MM_DC_CONTINUESHOT_ROT,
	
	MM_DV_SHOT= 0x60,
	MM_DV_SHOT_DISPLAY,	
	MM_DV_SHOT_ROT,
	MM_DV_SHOT_YUV,
	MM_DV_SHOT_CAP,
	MM_DV_SHOT_FRAME_POOL,
	MM_DV_SHOT_JPEG_ENC,
	MM_DV_SHOT_MUX,

	MM_DV_PLAY=0x70,
	MM_DV_PLAY_DISP,
	MM_DV_PLAY_ROT,
	
	MM_BLOCK_MAX
}MM_BLOCK_ID;

typedef enum
{
	MM_NOT_USED,
	MM_USED
}MM_BLOCK_STATUS_E;

typedef struct MM_BLOCK_MEM_Tag
{
	const MM_BLOCK_ID		block_id;
	uint32						block_offset;		//starting address relative to DC buffer
	uint32						block_size;		//size of the block
	MM_BLOCK_STATUS_E		status;			//indicate the block status,used or not used
	const BOOLEAN				exclusive;		//TRUE-exclusive use, assert if memory crossed; FALSE-shared use, do not assert if memory crossed
}MM_BLOCK_MEM_T;

typedef struct MM_BLOCK_MEM_TBL_tag
{
	const MM_BLOCK_MEM_T*	tbl_ptr;
	const uint32					tbl_num;
}MM_BLOCK_MEM_TBL_T;

typedef struct MM_CONTEXT_Tag
{
	uint8*	buf_ptr;
	uint32	buf_len;
	uint16	lcd_width;
	uint16	lcd_height;
	uint32	yuv_max_buf_len;
	uint32	yuv_buf_offset;
	uint32	jpeg_max_buf_len;
	uint32	quick_view_len;    
	uint32	jpege_temp_buf_len;
	uint32	count;
}MM_CONTEXT_T;

/******************************************************************************
// Description: get lcdc osd-rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of lcdc osd-rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetLcdcBufSize(void);

/******************************************************************************
// Description: get preview cap buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of preview cap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetPreviewCapBufSize(void);

/******************************************************************************
// Description: get preview rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of preview rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetPreviewRotBufSize(void);

/******************************************************************************
// Description: get capture osd buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture osd buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureRotBufSize(void);

/******************************************************************************
// Description: get capture jpg encoder fw buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture encoder fw buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureJpgFwBufSize(void);

/******************************************************************************
// Description: get capture interpolate swap buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture interpolate swap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureSwapBufSize(void);

/******************************************************************************
// Description: get capture display buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture interpolate swap buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureDispBufSize(void);

/******************************************************************************
// Description: get capture rotation  buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureRotationBufSize(void);

/******************************************************************************
// Description: get capture quick view buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture quick view buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureQuickViewBufSize(void);

/******************************************************************************
// Description: get capture yuv buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of preview osd buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureYuvBufSize(void);

/******************************************************************************
// Description: get capture minature jpg temp buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of minature jpg temp buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureMinAtureJpgTempBufSize(void);

/******************************************************************************
// Description: get capture exif  buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture exif buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureExifBufSize(void);

/******************************************************************************
// Description: get capture thumbnail buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of capture thumbnail buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetCaptureThumbnailBufSize(void);

/******************************************************************************
// Description: get continue shot jpg fw buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of continue shot jpg fw buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetContinueShotJpgFwBufSize(void);

/******************************************************************************
// Description: get continue shot rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of continue shot rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetContinueShotRotBufSize(void);

/******************************************************************************
// Description: get continue shot display buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of continue shot display buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetContinueShotDispBufSize(void);

/******************************************************************************
// Description: get continue shot yuv buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of continue shot yuv buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetContinueShotYuvBufSize(void);

/******************************************************************************
// Description: get image jpg size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of image jpg
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetImgJpgSize(void);

/******************************************************************************
// Description: get review yuv buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of review yuv buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetReviewYuvBufSize(void);

/******************************************************************************
// Description: get review rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of review rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetReviewRotBufSize(void);

/******************************************************************************
// Description: get review display buf buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of review display buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetReviewDispBufSize(void);

/******************************************************************************
// Description: get review jpg fw buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of review jpg fw buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetReviewJpgFwBufSize(void);

/******************************************************************************
// Description: get DV recorder disp buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder disp buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderDispBufSize(void);

/******************************************************************************
// Description: get DV recorder scale buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder yuv scale buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderScaleBufSize(void);

/******************************************************************************
// Description: get DV recorder rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderRotBufSize(void);

/******************************************************************************
// Description: get DV recorder capture yuv buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder capture yuv buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderCapBufSize(void);

/******************************************************************************
// Description: get DV recorder frame pool size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder frame poool
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderFramePoolSize(void);

/******************************************************************************
// Description: get DV recorder jpg fw buf
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder jpg fw buf
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvRecorderJpgFwBufSize(void);

/******************************************************************************
// Description: get DV play display buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvPlayDispBufSize(void);

/******************************************************************************
// Description: get DV play rotation buffer size
// Author:  Tim.zhu
// Input:   None
// Output:  None 
// Return:  uint32:the length of DV recorder rotation buffer
// Note:    none
******************************************************************************/
PUBLIC uint32 MM_GetDvPlayRotBufSize(void);

/******************************************************************************
// Description: 	dc buffer alloc
// Author:     		tim.zhu
// Input:      		none
// Output:		none
// Return:		none
// Note:       		none
******************************************************************************/
PUBLIC uint32 MM_AllocDCBuffer(void);

/******************************************************************************
// Description: 	multimedia buffer init
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		none
// Note:       		none
******************************************************************************/
PUBLIC uint32 MM_Init(uint32 dc_state);

/******************************************************************************
// Description: 	multimedia buffer exit
// Author:     		Frank.yang
// Input:      		none
// Output:		none
// Return:		none
// Note:       		none
******************************************************************************/
PUBLIC uint32 MM_Exit(void);


/******************************************************************************
// Description: 	malloc a block buffer in Multimedia unit
// Author:     		Frank.yang
// Input:      		id--the id of block to be wanted
// Output:		none
// Return:		the block buffer address
// Note:       		none
******************************************************************************/
PUBLIC void* MM_Malloc(MM_BLOCK_ID mem_id);

/******************************************************************************
// Description: 	get the size of the block  in Multimedia unit
// Author:     		Frank.yang
// Input:      		id--the id of block to be wanted
// Output:		none
// Return:		none
// Note:       		none
******************************************************************************/
PUBLIC uint32 MM_GetSize(MM_BLOCK_ID mem_id);

#ifdef __cplusplus
    }
#endif

#endif



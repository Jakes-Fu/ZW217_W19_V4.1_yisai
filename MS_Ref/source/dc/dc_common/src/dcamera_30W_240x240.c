/*******************************************************************************
 ** FileName:      dcamera_cfg.c                                               *
 ** Author:                                                                    *
 ** Date:                                                                      *
 ** CopyRight:                                                                 *
 ** Description:   Camera 方案配置                                                 *
 *******************************************************************************/


#include "ms_ref_dc_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "dcamera_common.h"
#include "dc_common.h"
#include "dcamera_mem.h"
#include "dal_dcamera.h"
#include "dcamera_cfg.h"
#include "dal_lcd.h"

#ifdef   __cplusplus
extern   "C"
{
#endif


///----------------------- Sensor data OUTPUT MODE   --------------------------

/* SPI 串口1线*/
//#define  SENSOR_DATA_OUTPUT_MODE  SPI_MODE_MASTER2_1

/* SPI 串口2线*/
#define  SENSOR_DATA_OUTPUT_MODE  SPI_MODE_MASTER2_2

//SDR 模式
//#define  SENSOR_DATA_DDR_MODE     FALSE

//DDR 模式
#define  SENSOR_DATA_DDR_MODE     TRUE


///----------------------------------------------------------------------------
static int Gouda_config(int mode, BLOCKCFG_T* cfg, DC_CAMERA_CFG ** param);
static uint32 get_GoudaOffset(int mode, DC_CAMERA_CFG ** param);
static uint32 Preview_init_config(DC_CAMERA_CFG* param);
static uint32 Snapshot_init_config(DC_CAMERA_CFG* param);
static uint32 Record_init_config(DC_CAMERA_CFG* param);
static uint32 VideoCall_init_config(DC_CAMERA_CFG* param);

PUBLIC DCAMERA_IOCTL_CFG_T  g_dcam_cfg_30W_240x240 =
{
    get_GoudaOffset,
    Gouda_config,

    Preview_init_config,
    Snapshot_init_config,
    Record_init_config,
    VideoCall_init_config

};

//各方案显示时的 gouda config
static int Gouda_config(int mode, BLOCKCFG_T* cfg, DC_CAMERA_CFG ** param)
{
    LCD_INFO_T lcd_info;
    LCD_GetInfo(0, &lcd_info);

    if(!cfg || !param)
    {
        SCI_PASSERT(SCI_FALSE, ("DC:config param error!"));
    }

    cfg->start_x = 0;
    cfg->start_y = 0;
    cfg->end_x = lcd_info.lcd_width-1;
    cfg->end_y = lcd_info.lcd_height-1;

    cfg->width = param[mode]->width * 2;  //stride

    cfg->colorkey_en = 0;
    cfg->alpha_sel   = 1;
    cfg->alpha       = 0xFF;
    cfg->priority    = 0;
    cfg->type        = 1;
    cfg->resolution  = LCD_RESOLUTION_YUV422;

    cfg->rotation    = param[mode]->preview_rot;  // 0: 0; 1: 90; clockwise

    if(cfg->rotation == 1)
    {
        LCD_SetImageLayerSourceSize(MAIN_LCD_ID, param[mode]->display_height, param[mode]->display_width);
    }
    else if(cfg->rotation == 0)
    {
        LCD_SetImageLayerSourceSize(MAIN_LCD_ID, param[mode]->display_width, param[mode]->display_height);
    }
    else
    {
        SCI_PASSERT(SCI_FALSE, ("DC: mode Error!"));
    }

    SCI_TRACE_LOW("DC:config lcd(w*h=%d*%d) YUV(w*h=%d*%d) rot=%d mod=%d",
                             lcd_info.lcd_width, lcd_info.lcd_height,
                             param[mode]->width, param[mode]->height,
                             cfg->rotation, mode );

    return 0;
}

//各方案显示时的 gouda offset
static uint32 get_GoudaOffset(int mode, DC_CAMERA_CFG ** param)
{

    uint32 offset_width = 0;
    uint32 offset_height = 0;

    if(!param)
    {
       SCI_PASSERT(SCI_FALSE, ("DC: get_GoudaOffset error!"));
    }

    if(DCAMERA_MODE_PREVIEW == mode)
    {
        if(param[DCAMERA_MODE_PREVIEW]->width > param[DCAMERA_MODE_PREVIEW]->display_width)
            offset_width = (param[DCAMERA_MODE_PREVIEW]->width - param[DCAMERA_MODE_PREVIEW]->display_width) / 2;
        if(param[DCAMERA_MODE_PREVIEW]->height > param[DCAMERA_MODE_PREVIEW]->display_height)
            offset_height = (param[DCAMERA_MODE_PREVIEW]->height - param[DCAMERA_MODE_PREVIEW]->display_height) / 2;
        if(param[DCAMERA_MODE_PREVIEW]->preview_rot)
        {
            return ((param[DCAMERA_MODE_PREVIEW]->height - offset_height - 1)\
                                * param[DCAMERA_MODE_PREVIEW]->width + offset_width )* 2;
        }
        else
        {
            return (offset_height * param[DCAMERA_MODE_PREVIEW]->width + offset_width) * 2;
        }
    }
    else if(DCAMERA_MODE_SNAPSHOT == mode)
    {
        if(param[DCAMERA_MODE_SNAPSHOT]->width > param[DCAMERA_MODE_SNAPSHOT]->display_width)
            offset_width = (param[DCAMERA_MODE_SNAPSHOT]->width - param[DCAMERA_MODE_SNAPSHOT]->display_width) / 2;
        if(param[DCAMERA_MODE_SNAPSHOT]->height > param[DCAMERA_MODE_PREVIEW]->display_height)
            offset_height = (param[DCAMERA_MODE_SNAPSHOT]->height - param[DCAMERA_MODE_SNAPSHOT]->display_height) / 2;
        if(param[DCAMERA_MODE_SNAPSHOT]->preview_rot)
        {
            return ((param[DCAMERA_MODE_SNAPSHOT]->height - offset_height - 1)\
                                * param[DCAMERA_MODE_SNAPSHOT]->width + offset_width )* 2;

        }
        else
        {
            return (offset_height * param[DCAMERA_MODE_SNAPSHOT]->width + offset_width) * 2;
        }
    }
    else if(DCAMERA_MODE_RECORD == mode)
    {
        if(param[DCAMERA_MODE_RECORD]->width > param[DCAMERA_MODE_RECORD]->display_width)
            offset_width = (param[DCAMERA_MODE_RECORD]->width - param[DCAMERA_MODE_RECORD]->display_width) / 2;
        if(param[DCAMERA_MODE_RECORD]->height > param[DCAMERA_MODE_RECORD]->display_height)
            offset_height = (param[DCAMERA_MODE_RECORD]->height - param[DCAMERA_MODE_RECORD]->display_height) / 2;
        if(param[DCAMERA_MODE_RECORD]->preview_rot)
        {
            return ((param[DCAMERA_MODE_RECORD]->height - offset_height - 1)\
                                * param[DCAMERA_MODE_RECORD]->width + offset_width )* 2;
        }
        else
        {
            return (offset_height * param[DCAMERA_MODE_RECORD]->width + offset_width) * 2;
        }
    }
    else
    {
        SCI_PASSERT(SCI_FALSE, ("DC:mode error!"));
    }

    return 0;
}


/*
 *  30W sensor 640x480. 预览输出YUV 320X240
 */
static uint32 Preview_init_config(DC_CAMERA_CFG* param)
{

    PDC_CAMERA_CFG pCfg = (DC_CAMERA_CFG*)param;

    if(!param)
    {
        SCI_PASSERT(SCI_FALSE, ("Preview_init_config  error!"));
    }

    pCfg->hal_cfg.rstActiveH            = FALSE;
    pCfg->hal_cfg.pdnActiveH            = TRUE;
    pCfg->hal_cfg.dropFrame             = FALSE;
    pCfg->hal_cfg.camClkDiv             = 42;//g_MclkDiv;
    pCfg->hal_cfg.endianess             = NO_SWAP;
    pCfg->hal_cfg.camId                 =  0;
    pCfg->hal_cfg.cropEnable            = FALSE;
    pCfg->hal_cfg.vsync_inv             = FALSE;
    pCfg->hal_cfg.spi_pixels_per_line   = 640;                 //240; //IspOutWidth;
    pCfg->hal_cfg.spi_pixels_per_column = 480;                 //IspOutHeight;
    pCfg->hal_cfg.spi_yuv_out           = SPI_OUT_Y0_U0_Y1_V0;
    pCfg->hal_cfg.spi_mode              = SENSOR_DATA_OUTPUT_MODE;
    pCfg->hal_cfg.spi_little_endian_en  = FALSE;
    pCfg->hal_cfg.spi_ctrl_clk_div      = 1;
    pCfg->hal_cfg.ddr_en                = SENSOR_DATA_DDR_MODE;   //ddr enable

    pCfg->hal_cfg.colRatio              = COL_RATIO_1_2;
    pCfg->hal_cfg.rowRatio              = ROW_RATIO_1_2;
    pCfg->hal_cfg.scaleEnable = TRUE;

    pCfg->hal_cfg.cropEnable     = TRUE;
    pCfg->hal_cfg.dstWinColStart = 36;
    pCfg->hal_cfg.dstWinColEnd   = 640-36;
    pCfg->hal_cfg.dstWinRowStart = 0;
    pCfg->hal_cfg.dstWinRowEnd   = 480;

    pCfg->preview_rot = 1;   //普通预览旋转90度
    
    pCfg->width  = 284;      
    pCfg->height = 240;

    pCfg->display_width = 284;
    pCfg->display_height =240;

    return 0;
}



/*
 *  30W sensor 640x480. 拍照输出YUV 640x480
 */
static uint32 Snapshot_init_config(DC_CAMERA_CFG* param)
{

    PDC_CAMERA_CFG pCfg = (DC_CAMERA_CFG*)param;

    if(!param)
    {
        SCI_PASSERT(SCI_FALSE, ("Snapshot_init_config  error!"));
    }

    pCfg->hal_cfg.rstActiveH            = FALSE;
    pCfg->hal_cfg.pdnActiveH            = TRUE;
    pCfg->hal_cfg.dropFrame             = FALSE;
    pCfg->hal_cfg.camClkDiv             = 42;//g_MclkDiv;
    pCfg->hal_cfg.endianess             = NO_SWAP;
    pCfg->hal_cfg.camId                 =  0;
    pCfg->hal_cfg.cropEnable            = FALSE;
    pCfg->hal_cfg.vsync_inv             = FALSE;
    pCfg->hal_cfg.spi_pixels_per_line   = 640;                      //320; //IspOutWidth;
    pCfg->hal_cfg.spi_pixels_per_column = 480;                      //IspOutHeight;
    pCfg->hal_cfg.spi_yuv_out           = SPI_OUT_Y0_U0_Y1_V0;
    pCfg->hal_cfg.spi_mode              = SENSOR_DATA_OUTPUT_MODE;  //2 wire;
    pCfg->hal_cfg.spi_little_endian_en  = FALSE;
    pCfg->hal_cfg.spi_ctrl_clk_div      = 1;
    pCfg->hal_cfg.ddr_en                = SENSOR_DATA_DDR_MODE;   //ddr enable

    pCfg->hal_cfg.colRatio              = COL_RATIO_1_1;
    pCfg->hal_cfg.rowRatio              = ROW_RATIO_1_1;
	
    pCfg->hal_cfg.cropEnable     = TRUE;
	pCfg->hal_cfg.dstWinColStart = 0;
	pCfg->hal_cfg.dstWinColEnd   = 640;
    pCfg->hal_cfg.dstWinRowStart = 0;
    pCfg->hal_cfg.dstWinRowEnd   = 480;

    pCfg->preview_rot = 1;  //拍照照片预览旋转90度
    pCfg->enc_rot     = 1;

    pCfg->width  = 640;
    pCfg->height = 480;

    pCfg->display_width = 640;
    pCfg->display_height = 480;


    return 0;
}


/*
 *  30W sensor 640x480. 录像输出YUV 240x240
 */
static uint32 Record_init_config(DC_CAMERA_CFG* param)
{
    PDC_CAMERA_CFG pCfg = (DC_CAMERA_CFG*)param;

    if(!param)
    {
        SCI_PASSERT(SCI_FALSE, ("Record_init_config  error!"));
    }

    pCfg->hal_cfg.rstActiveH            = FALSE;
    pCfg->hal_cfg.pdnActiveH            = TRUE;
    pCfg->hal_cfg.dropFrame             = FALSE;
    pCfg->hal_cfg.camClkDiv             = 42;//g_MclkDiv;
    pCfg->hal_cfg.endianess             = NO_SWAP;
    pCfg->hal_cfg.camId                 =  0;
    pCfg->hal_cfg.cropEnable            = FALSE;
    pCfg->hal_cfg.vsync_inv             = FALSE;
	pCfg->hal_cfg.spi_pixels_per_line   = 640;                 //320; //IspOutWidth;
	pCfg->hal_cfg.spi_pixels_per_column = 480;                 //IspOutHeight;
    pCfg->hal_cfg.spi_yuv_out           = SPI_OUT_Y0_U0_Y1_V0;
    pCfg->hal_cfg.spi_mode              = SENSOR_DATA_OUTPUT_MODE;
    pCfg->hal_cfg.spi_little_endian_en  = FALSE;
    pCfg->hal_cfg.spi_ctrl_clk_div      = 1;
    pCfg->hal_cfg.ddr_en                = SENSOR_DATA_DDR_MODE;   //ddr enable


	pCfg->hal_cfg.colRatio   = COL_RATIO_1_2;
	pCfg->hal_cfg.rowRatio   = ROW_RATIO_1_2;
	pCfg->hal_cfg.scaleEnable = TRUE;

	pCfg->hal_cfg.cropEnable     = TRUE;
	pCfg->hal_cfg.dstWinColStart = 36;
	pCfg->hal_cfg.dstWinColEnd   = 640-36;
	pCfg->hal_cfg.dstWinRowStart = 0;
	pCfg->hal_cfg.dstWinRowEnd   = 480;

    pCfg->preview_rot = 1;    //录像预览旋转90度
    
	pCfg->width  = 284;      
    pCfg->height = 240;

	pCfg->display_width = 284;
	pCfg->display_height = 240;

    return 0;
}


//视频通话Camera输出240x240,视频通话实际size由
//上层软件从该240x240 YUV数据中截取
static uint32 VideoCall_init_config(DC_CAMERA_CFG* param)
{

    PDC_CAMERA_CFG pCfg = (DC_CAMERA_CFG*)param;

    if(!param)
    {
        SCI_PASSERT(SCI_FALSE, ("VideoCall_init_config  error!"));
    }

    pCfg->hal_cfg.rstActiveH            = FALSE;
    pCfg->hal_cfg.pdnActiveH            = TRUE;
    pCfg->hal_cfg.dropFrame             = FALSE;
    pCfg->hal_cfg.camClkDiv             = 42;//g_MclkDiv;
    pCfg->hal_cfg.endianess             = NO_SWAP;
    pCfg->hal_cfg.camId                 =  0;
    pCfg->hal_cfg.cropEnable            = FALSE;
    pCfg->hal_cfg.vsync_inv             = FALSE;
    pCfg->hal_cfg.spi_pixels_per_line   = 640;                 //320; //IspOutWidth;
    pCfg->hal_cfg.spi_pixels_per_column = 480;                 //IspOutHeight;
    pCfg->hal_cfg.spi_yuv_out           = SPI_OUT_Y0_U0_Y1_V0;
    pCfg->hal_cfg.spi_mode              = SPI_MODE_MASTER2_2;  //2 wire;
    pCfg->hal_cfg.spi_little_endian_en  = FALSE;
    pCfg->hal_cfg.spi_ctrl_clk_div      = 1;
    pCfg->hal_cfg.ddr_en                = SENSOR_DATA_DDR_MODE;   //ddr enable


    pCfg->hal_cfg.colRatio   = COL_RATIO_1_2;
    pCfg->hal_cfg.rowRatio   = ROW_RATIO_1_2;
	pCfg->hal_cfg.scaleEnable = TRUE;

	pCfg->hal_cfg.cropEnable     = TRUE;
	pCfg->hal_cfg.dstWinColStart = 80;
	pCfg->hal_cfg.dstWinColEnd   = 640-80;
	pCfg->hal_cfg.dstWinRowStart = 0;
	pCfg->hal_cfg.dstWinRowEnd   = 480;

    pCfg->preview_rot = 1;       //视频通话送预览YUV数据旋转90度
    
    pCfg->width  = 240;
    pCfg->height = 240;

    return 0;
}





#ifdef   __cplusplus
}
#endif


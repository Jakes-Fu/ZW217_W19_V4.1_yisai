/******************************************************************************
 ** File Name:    Sensor_GT2005.c                                         *
 ** Author:         Mormo Hui                                             *
 ** Date:            2010/12/06                                            *
 ** Copyright:     Spreadtrum All Rights Reserved.        *
 ** Description:   implementation of digital camera register interface       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE             NAME             DESCRIPTION                                 *
 ** 2010/12/06   Mormo            Release                                            *
 ******************************************************************************/
#ifndef _GT2005_C_

#define _GT2005_C_
#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "i2c_api.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"
#include "sci_api.h"
#include "gpio_prod_api.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **      Extern Function Declaration         *
 **---------------------------------------------------------------------------*/
//extern uint32 OS_TickDelay(uint32 ticks);
                                           
/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define GT2005_I2C_ADDR_W               0x78
#define GT2005_I2C_ADDR_R               0x79
#define GT2005_I2C_ACK                  0x0
//#define SENSOR_WRITE_DELAY            0xffff
LOCAL BOOLEAN   bl_50Hz_GT2005  = FALSE;

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL uint32 GT2005_Power_On(uint32 power_on);
LOCAL uint32 GT2005_Identify(uint32 param);
LOCAL void    GT2005_WriteReg( uint16  subaddr, uint8 data );
LOCAL uint8     GT2005_ReadReg( uint16  subaddr);
LOCAL uint32 Set_GT2005_Brightness(uint32 level);
LOCAL uint32 Set_GT2005_Contrast(uint32 level);
LOCAL uint32 Set_GT2005_Image_Effect(uint32 effect_type);
LOCAL uint32 Set_GT2005_Ev(uint32 level);
LOCAL uint32 Set_GT2005_Anti_Flicker(uint32 mode);
LOCAL uint32 Set_GT2005_Preview_Mode(uint32 preview_mode);
LOCAL uint32 Set_GT2005_AWB(uint32 mode);
LOCAL uint32 GT2005_AE_Enable(uint32 param);
LOCAL uint32 GT2005_Before_Snapshot(uint32 sensor_snapshot_mode);
LOCAL void    GT2005_Set_Shutter(void);
LOCAL uint32 GT2005_After_Snapshot(uint32 para);
LOCAL uint32 Set_GT2005_Video_Mode(uint32 mode);
LOCAL void    GT2005_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr );
LOCAL uint32 GT2005_Change_Image_Format(uint32 param);
LOCAL uint32 Check_GT2005_Image_Format_Support(uint32 param); 
//LOCAL uint32 GT2005_Zoom(uint32 level);
LOCAL uint32 Set_GT2005_Hmirror_Enable(uint32 enable);
LOCAL uint32 Set_GT2005_Vmirror_Enable(uint32 enable);
LOCAL uint32 GT2005_GetExifInfo(uint32 param);
LOCAL uint32 GT2005_InitExifInfo(void);
/**---------------------------------------------------------------------------*
 **                         Local Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL EXIF_SPEC_PIC_TAKING_COND_T s_GT2005_exif = {0};


__align(4) const SENSOR_REG_T GT2005_YUV_COMMON[] = 
{
    {0x0102 , 0x01},
    {0x0103 , 0x00},
    {0x0105 , 0x00},
    {0x0106 , 0xF0},
    {0x0107 , 0x00},
    {0x0108 , 0x1C},
    {0x0109 , 0x01},
    {0x010A , 0x00},
    {0x010B , 0x00},
    {0x010C , 0x00},
    {0x010D , 0x08},
    {0x010E , 0x00},
    {0x010F , 0x08},
    {0x0110 , 0x06},
    {0x0111 , 0x40},
    {0x0112 , 0x04},
    {0x0113 , 0xB0},
    {0x0114 , 0x04},
    {0x0115 , 0x00},
    {0x0116 , 0x02},
    {0x0117 , 0x00},
    {0x0118 , 0x40},
    {0x0119 , 0x01},
    {0x011A , 0x04},
    {0x011B , 0x00},
    {0x011C , 0x01},
    {0x011D , 0x01},
    {0x011E , 0x00},
    {0x011F , 0x00},
    {0x0120 , 0x1C},
    {0x0121 , 0x00},
    {0x0122 , 0x04},
    {0x0123 , 0x00},
    {0x0124 , 0x00},
    {0x0125 , 0x00},
    {0x0126 , 0x00},
    {0x0127 , 0x00},
    {0x0128 , 0x00},
    {0x0200 , 0x20},
    {0x0201 , 0x00},
    {0x0202 , 0x48},
    {0x0203 , 0x00},
    {0x0204 , 0x00},
    {0x0205 , 0x09},
    {0x0206 , 0x0A},
    {0x0207 , 0x20},
    {0x0208 , 0x00},
    {0x0209 , 0x59},
    {0x020A , 0x01},
    {0x020B , 0x28},
    {0x020C , 0x44},
    {0x020D , 0xE2},
    {0x020E , 0xB0},
    {0x020F , 0x08},
    {0x0210 , 0xD6},
    {0x0211 , 0x00},
    {0x0212 , 0x20},
    {0x0213 , 0x81},
    {0x0214 , 0x15},
    {0x0215 , 0x00},
    {0x0216 , 0x00},
    {0x0217 , 0x00},
    {0x0218 , 0x44},
    {0x0219 , 0x30},
    {0x021A , 0x03},
    {0x021B , 0x28},
    {0x021C , 0x02},
    {0x021D , 0x60},
    {0x0220 , 0x00},
    {0x0221 , 0x00},
    {0x0222 , 0x00},
    {0x0223 , 0x00},
    {0x0224 , 0x00},
    {0x0225 , 0x00},
    {0x0226 , 0x00},
    {0x0227 , 0x00},
    {0x0228 , 0x00},
    {0x0229 , 0x00},
    {0x022A , 0x00},
    {0x022B , 0x00},
    {0x022C , 0x00},
    {0x022D , 0x00},
    {0x022E , 0x00},
    {0x022F , 0x00},
    {0x0230 , 0x00},
    {0x0231 , 0x00},
    {0x0232 , 0x00},
    {0x0233 , 0x00},
    {0x0234 , 0x00},
    {0x0235 , 0x00},
    {0x0236 , 0x00},
    {0x0237 , 0x00},
    {0x0238 , 0x00},
    {0x0239 , 0x00},
    {0x023A , 0x00},
    {0x023B , 0x00},
    {0x023C , 0x00},
    {0x023D , 0x00},
    {0x023E , 0x00},
    {0x023F , 0x00},
    {0x0240 , 0x00},
    {0x0241 , 0x00},
    {0x0242 , 0x00},
    {0x0243 , 0x00},
    {0x0244 , 0x00},
    {0x0245 , 0x00},
    {0x0246 , 0x00},
    {0x0247 , 0x00},
    {0x0248 , 0x00},
    {0x0249 , 0x00},
    {0x024A , 0x00},
    {0x024B , 0x00},
    {0x024C , 0x00},
    {0x024D , 0x00},
    {0x024E , 0x00},
    {0x024F , 0x00},
    {0x0250 , 0x01},
    {0x0251 , 0x00},
    {0x0252 , 0x00},
    {0x0253 , 0x00},
    {0x0254 , 0x00},
    {0x0255 , 0x00},
    {0x0256 , 0x00},
    {0x0257 , 0x00},
    {0x0258 , 0x00},
    {0x0259 , 0x00},
    {0x025A , 0x00},
    {0x025B , 0x00},
    {0x025C , 0x00},
    {0x025D , 0x00},
    {0x025E , 0x00},
    {0x025F , 0x00},
    {0x0260 , 0x00},
    {0x0261 , 0x00},
    {0x0262 , 0x00},
    {0x0263 , 0x00},
    {0x0264 , 0x00},
    {0x0265 , 0x00},
    {0x0266 , 0x00},
    {0x0267 , 0x00},
    {0x0268 , 0x8F},
    {0x0269 , 0xA3},
    {0x026A , 0xB4},
    {0x026B , 0x90},
    {0x026C , 0x00},
    {0x026D , 0xD0},
    {0x026E , 0x60},
    {0x026F , 0xA0},
    {0x0270 , 0x40},
    {0x0300 , 0x81},
    {0x0301 , 0xA8},
    {0x0302 , 0x0D},
    {0x0303 , 0x05},
    {0x0304 , 0x03},
    {0x0305 , 0x43},
    {0x0306 , 0x00},
    {0x0307 , 0x0D},
    {0x0308 , 0x00},
    {0x0309 , 0x55},
    {0x030A , 0x55},
    {0x030B , 0x55},
    {0x030C , 0x54},
    {0x030D , 0x13},
    {0x030E , 0x0A},
    {0x030F , 0x10},
    {0x0310 , 0x04},
    {0x0311 , 0xFF},
    {0x0312 , 0x98},
    {0x0313 , 0x32},
    {0x0314 , 0x9A},
    {0x0315 , 0x16},
    {0x0316 , 0x26},
    {0x0317 , 0x02},
    {0x0318 , 0x08},
    {0x0319 , 0x0C},
    {0x031A , 0x81},
    {0x031B , 0x00},
    {0x031C , 0x1D},
    {0x031D , 0x00},
    {0x031E , 0xFD},
    {0x031F , 0x00},
    {0x0320 , 0xE1},
    {0x0321 , 0x1A},
    {0x0322 , 0xDE},
    {0x0323 , 0x11},
    {0x0324 , 0x1A},
    {0x0325 , 0xEE},
    {0x0326 , 0x50},
    {0x0327 , 0x18},
    {0x0328 , 0x25},
    {0x0329 , 0x37},
    {0x032A , 0x24},
    {0x032B , 0x32},
    {0x032C , 0xA9},
    {0x032D , 0x32},
    {0x032E , 0xDB},
    {0x032F , 0x42},
    {0x0330 , 0x30},
    {0x0331 , 0x55},
    {0x0332 , 0x7F},
    {0x0333 , 0x14},
    {0x0334 , 0x81},
    {0x0335 , 0x14},
    {0x0336 , 0xFF},
    {0x0337 , 0x20},
    {0x0338 , 0x46},
    {0x0339 , 0x04},
    {0x033A , 0x04},
    {0x033B , 0x00},
    {0x033C , 0x00},
    {0x033D , 0x00},
    {0x033E , 0x03},
    {0x033F , 0x28},
    {0x0340 , 0x02},
    {0x0341 , 0x60},
    {0x0400 , 0xE8},
    {0x0401 , 0x40},
    {0x0402 , 0x00},
    {0x0403 , 0x00},
    {0x0404 , 0xF8},
    {0x0405 , 0x08},
    {0x0406 , 0x08},
    {0x0407 , 0x89},
    {0x0408 , 0x44},
    {0x0409 , 0x1F},
    {0x040A , 0x00},
    {0x040B , 0xF6},
    {0x040C , 0xE0},
    {0x040D , 0x00},
    {0x040E , 0x00},
    {0x040F , 0x00},
    {0x0410 , 0x00},
    {0x0411 , 0x00},
    {0x0412 , 0x01},
    {0x0413 , 0x00},
    {0x0414 , 0x04},
    {0x0415 , 0x04},
    {0x0416 , 0x01},
    {0x0417 , 0x00},
    {0x0418 , 0x02},
    {0x0419 , 0x02},
    {0x041A , 0x01},
    {0x041B , 0x01},
    {0x041C , 0x04},
    {0x041D , 0x04},
    {0x041E , 0x00},
    {0x041F , 0x08},
    {0x0420 , 0x40},
    {0x0421 , 0x40},
    {0x0422 , 0x40},
    {0x0423 , 0x35},
    {0x0424 , 0x40},
    {0x0425 , 0x40},
    {0x0426 , 0x42},
    {0x0427 , 0x3B},
    {0x0428 , 0x22},
    {0x0429 , 0x22},
    {0x042A , 0x21},
    {0x042B , 0x20},
    {0x042C , 0x22},
    {0x042D , 0x22},
    {0x042E , 0x1F},
    {0x042F , 0x1C},
    {0x0430 , 0x24},
    {0x0431 , 0x24},
    {0x0432 , 0x25},
    {0x0433 , 0x00},
    {0x0434 , 0x00},
    {0x0435 , 0x00},
    {0x0436 , 0x00},
    {0x0437 , 0x00},
    {0x0438 , 0x18},
    {0x0439 , 0x18},
    {0x043A , 0x00},
    {0x043B , 0x00},
    {0x043C , 0x18},
    {0x043D , 0x18},
    {0x043E , 0x10},
    {0x043F , 0x10},
    {0x0440 , 0x00},
    {0x0441 , 0x4b},
    {0x0442 , 0x00},
    {0x0443 , 0x00},
    {0x0444 , 0x31},
    {0x0445 , 0x00},
    {0x0446 , 0x00},
    {0x0447 , 0x00},
    {0x0448 , 0x00},
    {0x0449 , 0x00},
    {0x044A , 0x00},
    {0x044D , 0xE0},
    {0x044E , 0x05},
    {0x044F , 0x07},
    {0x0450 , 0x00},
    {0x0451 , 0x00},
    {0x0452 , 0x00},
    {0x0453 , 0x00},
    {0x0454 , 0x00},
    {0x0455 , 0x00},
    {0x0456 , 0x00},
    {0x0457 , 0x00},
    {0x0458 , 0x00},
    {0x0459 , 0x00},
    {0x045A , 0x00},
    {0x045B , 0x00},
    {0x045C , 0x00},
    {0x045D , 0x00},
    {0x045E , 0x00},
    {0x045F , 0x00},
    {0x0460 , 0x80},
    {0x0461 , 0x00},
    {0x0462 , 0x00},
    {0x0463 , 0x00},
    {0x0464 , 0x00},
    {0x0465 , 0x00},
    {0x0466 , 0x25},
    {0x0467 , 0x25},
    {0x0468 , 0x40},
    {0x0469 , 0x30},
    {0x046A , 0x28},
    {0x046B , 0x45},
    {0x046C , 0x3A},
    {0x046D , 0x33},
    {0x046E , 0x2F},
    {0x046F , 0x49},
    {0x0470 , 0x46},
    {0x0471 , 0x3D},
    {0x0472 , 0x30},
    {0x0473 , 0x3E},
    {0x0474 , 0x3D},
    {0x0475 , 0x32},
    {0x0476 , 0x34},
    {0x0477 , 0x40},
    {0x0600 , 0x00},
    {0x0601 , 0x24},
    {0x0602 , 0x45},
    {0x0603 , 0x0E},
    {0x0604 , 0x14},
    {0x0605 , 0x2F},
    {0x0606 , 0x01},
    {0x0607 , 0x0E},
    {0x0608 , 0x0E},
    {0x0609 , 0x37},
    {0x060A , 0x18},
    {0x060B , 0xA0},
    {0x060C , 0x20},
    {0x060D , 0x07},
    {0x060E , 0x47},
    {0x060F , 0x90},
    {0x0610 , 0x06},
    {0x0611 , 0x0C},
    {0x0612 , 0x28},
    {0x0613 , 0x13},
    {0x0614 , 0x0B},
    {0x0615 , 0x10},
    {0x0616 , 0x14},
    {0x0617 , 0x19},
    {0x0618 , 0x52},
    {0x0619 , 0xA0},
    {0x061A , 0x11},
    {0x061B , 0x33},
    {0x061C , 0x56},
    {0x061D , 0x20},
    {0x061E , 0x28},
    {0x061F , 0x2B},
    {0x0620 , 0x22},
    {0x0621 , 0x11},
    {0x0622 , 0x75},
    {0x0623 , 0x49},
    {0x0624 , 0x6E},
    {0x0625 , 0x80},
    {0x0626 , 0x02},
    {0x0627 , 0x0C},
    {0x0628 , 0x51},
    {0x0629 , 0x25},
    {0x062A , 0x01},
    {0x062B , 0x3D},
    {0x062C , 0x04},
    {0x062D , 0x01},
    {0x062E , 0x0C},
    {0x062F , 0x2C},
    {0x0630 , 0x0D},
    {0x0631 , 0x14},
    {0x0632 , 0x12},
    {0x0633 , 0x34},
    {0x0634 , 0x00},
    {0x0635 , 0x00},
    {0x0636 , 0x00},
    {0x0637 , 0xB1},
    {0x0638 , 0x22},
    {0x0639 , 0x32},
    {0x063A , 0x0E},
    {0x063B , 0x18},
    {0x063C , 0x88},
    {0x0640 , 0xB2},
    {0x0641 , 0xC0},
    {0x0642 , 0x01},
    {0x0643 , 0x26},
    {0x0644 , 0x13},
    {0x0645 , 0x88},
    {0x0646 , 0x64},
    {0x0647 , 0x00},
    {0x0681 , 0x1B},
    {0x0682 , 0xA0},
    {0x0683 , 0x28},
    {0x0684 , 0x00},
    {0x0685 , 0xB0},
    {0x0686 , 0x6F},
    {0x0687 , 0x33},
    {0x0688 , 0x1F},
    {0x0689 , 0x44},
    {0x068A , 0xA8},
    {0x068B , 0x44},
    {0x068C , 0x08},
    {0x068D , 0x08},
    {0x068E , 0x00},
    {0x068F , 0x00},
    {0x0690 , 0x01},
    {0x0691 , 0x00},
    {0x0692 , 0x01},
    {0x0693 , 0x00},
    {0x0694 , 0x00},
    {0x0695 , 0x00},
    {0x0696 , 0x00},
    {0x0697 , 0x00},
    {0x0698 , 0x2A},
    {0x0699 , 0x80},
    {0x069A , 0x1F},
    {0x069B , 0x00},
    {0x0F00 , 0x00},
    {0x0F01 , 0x00},
    {0x0100 , 0x01},
    {0x0102 , 0x02},
    {0x0104 , 0x03},
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////GAMMA//////////////////////////////////
    ///////////////////////////////////////////////////////////////////

    //-----------GAMMA Select(0)---------------//


    /*
        1:  //smallest gamma curve
            {0x0461 , 0x00},
            {0x0462 , 0x00},
            {0x0463 , 0x00},
            {0x0464 , 0x00},
            {0x0465 , 0x00},
            {0x0466 , 0x12},
            {0x0467 , 0x3B},
            {0x0468 , 0x34},
            {0x0469 , 0x26},
            {0x046A , 0x1E},
            {0x046B , 0x33},
            {0x046C , 0x2E},
            {0x046D , 0x2C},
            {0x046E , 0x28},
            {0x046F , 0x42},
            {0x0470 , 0x42},
            {0x0471 , 0x38},
            {0x0472 , 0x37},
            {0x0473 , 0x4D},
            {0x0474 , 0x48},
            {0x0475 , 0x44},
            {0x0476 , 0x40},
            {0x0477 , 0x56},

        2:
            {0x0461 , 0x00},
            {0x0462 , 0x00},
            {0x0463 , 0x00},
            {0x0464 , 0x00},
            {0x0465 , 0x00},
            {0x0466 , 0x29},
            {0x0467 , 0x37},
            {0x0468 , 0x3A},
            {0x0469 , 0x26},
            {0x046A , 0x21},
            {0x046B , 0x34},
            {0x046C , 0x34},
            {0x046D , 0x2B},
            {0x046E , 0x28},
            {0x046F , 0x41},
            {0x0470 , 0x3F},
            {0x0471 , 0x3A},
            {0x0472 , 0x36},
            {0x0473 , 0x47},
            {0x0474 , 0x44},
            {0x0475 , 0x3B},
            {0x0476 , 0x3B},
            {0x0477 , 0x4D},

        3:
            {0x0461 , 0x00},
            {0x0462 , 0x00},
            {0x0463 , 0x00},
            {0x0464 , 0x00},
            {0x0465 , 0x00},
            {0x0466 , 0x29},
            {0x0467 , 0x4B},
            {0x0468 , 0x41},
            {0x0469 , 0x2A},
            {0x046A , 0x25},
            {0x046B , 0x3A},
            {0x046C , 0x2C},
            {0x046D , 0x2B},
            {0x046E , 0x28},
            {0x046F , 0x40},
            {0x0470 , 0x3D},
            {0x0471 , 0x38},
            {0x0472 , 0x31},
            {0x0473 , 0x44},
            {0x0474 , 0x3E},
            {0x0475 , 0x3E},
            {0x0476 , 0x37},
            {0x0477 , 0x43},

        4:
            {0x0461 , 0x00},
            {0x0462 , 0x00},
            {0x0463 , 0x00},
            {0x0464 , 0x00},
            {0x0465 , 0x00},
            {0x0466 , 0x2F},
            {0x0467 , 0x4E},
            {0x0468 , 0x50},
            {0x0469 , 0x31},
            {0x046A , 0x27},
            {0x046B , 0x3C},
            {0x046C , 0x35},
            {0x046D , 0x27},
            {0x046E , 0x23},
            {0x046F , 0x46},
            {0x0470 , 0x3A},
            {0x0471 , 0x32},
            {0x0472 , 0x32},
            {0x0473 , 0x38},
            {0x0474 , 0x3E},
            {0x0475 , 0x36},
            {0x0476 , 0x33},
            {0x0477 , 0x41},

        5:  //largest gamma curve
            {0x0461 , 0x00},
            {0x0462 , 0x00},
            {0x0463 , 0x00},
            {0x0464 , 0x00},
            {0x0465 , 0x15},
            {0x0466 , 0x33},
            {0x0467 , 0x61},
            {0x0468 , 0x56},
            {0x0469 , 0x30},
            {0x046A , 0x22},
            {0x046B , 0x3E},
            {0x046C , 0x2E},
            {0x046D , 0x2B},
            {0x046E , 0x28},
            {0x046F , 0x3C},
            {0x0470 , 0x38},
            {0x0471 , 0x2F},
            {0x0472 , 0x2A},
            {0x0473 , 0x3C},
            {0x0474 , 0x34},
            {0x0475 , 0x31},
            {0x0476 , 0x31},
            {0x0477 , 0x39},
        */

    //-------------H_V_Switch(Normal)---------------//
            {0x0101 , 0x00},
            
     /*GT2005_H_V_Switch,

        1:  // normal
                {0x0101 , 0x00},
                
        2:  // IMAGE_H_MIRROR
                {0x0101 , 0x01},
                
        3:  // IMAGE_V_MIRROR
                {0x0101 , 0x02},
                
        4:  // IMAGE_HV_MIRROR
                {0x0101 , 0x03},
    */          
    //-------------H_V_Select End--------------//

       {0x0109 , 0x00},
    {0x010A , 0x04},
    {0x010B , 0x0B},
    {0x0110 , 0x02},
    {0x0111 , 0x80},
    {0x0112 , 0x01},
    {0x0113 , 0xE0},


       {SENSOR_WRITE_DELAY, 20},
    

};
__align(4) const SENSOR_REG_T GT2005_YUV_1600X1200[] = 
{

    {0x0109 , 0x01},
    {0x010A , 0x00},
    {0x010B , 0x00},
    {0x0110 , 0x06},
    {0x0111 , 0x40},
    {0x0112 , 0x04},
    {0x0113 , 0xB0},


};
__align(4) const SENSOR_REG_T GT2005_YUV_1280X960[] = 
{

    {0x0109 , 0x01},
    {0x010A , 0x00},
    {0x010B , 0x0B},
    {0x0110 , 0x05},
    {0x0111 , 0x00},
    {0x0112 , 0x03},
    {0x0113 , 0xC0},

    
};

__align(4) const SENSOR_REG_T GT2005_YUV_640X480[] = 
{
    {0x0109 , 0x00},
    {0x010A , 0x04},
    {0x010B , 0x0B},
    {0x0110 , 0x02},
    {0x0111 , 0x80},
    {0x0112 , 0x01},
    {0x0113 , 0xE0},


};

__align(4) const SENSOR_REG_T GT2005_JPEG_MODE[] =
{
    {0xff , 0xff}
};



LOCAL SENSOR_REG_TAB_INFO_T s_GT2005_resolution_Tab_YUV[]=
{   
    { ADDR_AND_LEN_OF_ARRAY(GT2005_YUV_COMMON), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422 },
    // YUV422 PREVIEW 1
    { ADDR_AND_LEN_OF_ARRAY(GT2005_YUV_640X480),    640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
    { ADDR_AND_LEN_OF_ARRAY(GT2005_YUV_1280X960),   1280, 960, 24, SENSOR_IMAGE_FORMAT_YUV422},
    { ADDR_AND_LEN_OF_ARRAY(GT2005_YUV_1600X1200),1600, 1200, 24,SENSOR_IMAGE_FORMAT_YUV422},
    { PNULL,                    0,      0,      0  ,        0,       0      },   


    // YUV422 PREVIEW 2 
     { PNULL,                   0,      0,      0  ,        0,        0      }, 
     { PNULL,                   0,      0,      0  ,        0,        0      },   
     { PNULL,                   0,      0,      0  ,        0,        0      },   
     { PNULL,                   0,      0,      0  ,        0,        0      }

};

 

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_GT2005_ioctl_func_tab = 
{
// Internal 
    PNULL,
    GT2005_Power_On, 
    PNULL,
    GT2005_Identify,
    
    PNULL,       
    PNULL,           
    PNULL,
    PNULL,

// External
    GT2005_AE_Enable,
    Set_GT2005_Hmirror_Enable,
    Set_GT2005_Vmirror_Enable,
    
    Set_GT2005_Brightness,
    Set_GT2005_Contrast, 
    PNULL,
    PNULL, 
    Set_GT2005_Preview_Mode, 
    Set_GT2005_Image_Effect, 
    GT2005_Before_Snapshot,
    GT2005_After_Snapshot,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    Set_GT2005_AWB,    
    PNULL, 
    PNULL, 
    Set_GT2005_Ev, 
    Check_GT2005_Image_Format_Support, 
    GT2005_Change_Image_Format, 
    PNULL,
    GT2005_GetExifInfo,
    PNULL,
    Set_GT2005_Anti_Flicker,
    PNULL,//Set_GT2005_Video_Mode,   
    PNULL,
    PNULL
};

 

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
PUBLIC SENSOR_INFO_T g_GT2005_yuv_info =
{
    GT2005_I2C_ADDR_W,              // salve i2c write address
    GT2005_I2C_ADDR_R,              // salve i2c read address
    
    SENSOR_I2C_REG_16BIT|\
    SENSOR_I2C_FREQ_100,            // bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
                                    // bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
                                    // other bit: reseved
    SENSOR_HW_SIGNAL_PCLK_N|\
    SENSOR_HW_SIGNAL_VSYNC_P|\
    SENSOR_HW_SIGNAL_HSYNC_P,       // bit0: 0:negative; 1:positive -> polarily of pixel clock
                                    // bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
                                    // bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
                                    // other bit: reseved                                           

    // preview mode
    SENSOR_ENVIROMENT_NORMAL|\
    SENSOR_ENVIROMENT_NIGHT|\
    SENSOR_ENVIROMENT_SUNNY,        
    
    // image effect
    SENSOR_IMAGE_EFFECT_NORMAL|\
    SENSOR_IMAGE_EFFECT_BLACKWHITE|\
    SENSOR_IMAGE_EFFECT_RED|\
    SENSOR_IMAGE_EFFECT_GREEN|\
    SENSOR_IMAGE_EFFECT_BLUE|\
    SENSOR_IMAGE_EFFECT_YELLOW|\
    SENSOR_IMAGE_EFFECT_NEGATIVE|\
    SENSOR_IMAGE_EFFECT_CANVAS,
    
    // while balance mode
    0,
        
    0x77777,                                // bit[0:7]: count of step in brightness, contrast, sharpness, saturation
                                    // bit[8:31] reseved
    
    SENSOR_LOW_PULSE_RESET,     // reset pulse level
    10,                             // reset pulse width(ms)
    
    SENSOR_LOW_LEVEL_PWDN,      // 1: high level valid; 0: low level valid
    
    2,                              // count of identify code 
    0x0000, 0x51,                       // supply two code to identify sensor.
    0x0001, 0x38,                       // for Example: index = 0-> Device id, index = 1 -> version id                                          
                                            
    SENSOR_AVDD_2800MV,         // voltage of avdd  
    
    1600,                           // max width of source image
    1200,                           // max height of source image
    "GT2005",                       // name of sensor                                               

    SENSOR_IMAGE_FORMAT_YUV422, // define in SENSOR_IMAGE_FORMAT_E enum,
                                    // if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
    SENSOR_IMAGE_PATTERN_YUV422_YUYV,   // pattern of input image form sensor;          

    s_GT2005_resolution_Tab_YUV,    // point to resolution table information structure
    &s_GT2005_ioctl_func_tab,       // point to ioctl function table
            
    PNULL,                          // information and table about Rawrgb sensor
    PNULL,                          // extend information about sensor  
    SENSOR_AVDD_2800MV,                    // iovdd
    SENSOR_AVDD_1500MV,                    // dvdd
    3,                     // skip frame num before preview 
    2,                     // skip frame num before capture     
    0,                     // deci frame num during preview;        
    0,                     // deci frame num during video preview;
    0,                     // threshold enable
    0,                     // threshold mode
    0,                     // threshold start postion   
    0,                     // threshold end postion 
    0,                     // i2c_dev_handler
    0
};



/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL void GT2005_WriteReg( uint16  subaddr, uint8 value )
{
    Sensor_WriteReg(subaddr,value);
    //SCI_TRACE_LOW:"GT2005 WriteReg reg/value(%x,%x) "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_879_112_2_18_0_30_55_835,(uint8*)"dd", subaddr, value);

}
LOCAL uint8 GT2005_ReadReg( uint16  subaddr)
{
    uint8 value = 0;

    value = Sensor_ReadReg(subaddr);
    //SCI_TRACE_LOW:"GT2005 ReadReg reg/value(%x,%x) "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_887_112_2_18_0_30_55_836,(uint8*)"dd",subaddr,value );

    return value;
}

LOCAL uint32 GT2005_Power_On(uint32 power_on)
{
    SENSOR_AVDD_VAL_E       dvdd_val=g_GT2005_yuv_info.dvdd_val;
    SENSOR_AVDD_VAL_E       avdd_val=g_GT2005_yuv_info.avdd_val;
    SENSOR_AVDD_VAL_E       iovdd_val=g_GT2005_yuv_info.iovdd_val;  
    BOOLEAN                 power_down=g_GT2005_yuv_info.power_down_level;      
    BOOLEAN                 reset_level=g_GT2005_yuv_info.reset_pulse_level;
    uint32              reset_width=g_GT2005_yuv_info.reset_pulse_width;        
    
    if(SCI_TRUE == power_on)
    {
        Sensor_SetVoltage(dvdd_val, avdd_val, iovdd_val);
        
        GPIO_SetSensorPwdn(!power_down);/*lint !e730*/

        // Open Mclk in default frequency
        Sensor_SetMCLK(SENSOR_DEFALUT_MCLK);   
        
        SCI_Sleep(10);
        Sensor_SetResetLevel(reset_level);
        SCI_Sleep(reset_width);
        Sensor_SetResetLevel(!reset_level);/*lint !e730*/
        SCI_Sleep(10);
    }
    else
    {
        GPIO_SetSensorPwdn(power_down);/*lint !e730*/        
        SCI_Sleep(10);
        Sensor_SetResetLevel(reset_level);

        Sensor_SetMCLK(SENSOR_DISABLE_MCLK);           

        Sensor_SetVoltage(SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED, SENSOR_AVDD_CLOSED);        
    }
    
    //SCI_TRACE_LOW:"set_GT2005_Power_On"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_927_112_2_18_0_30_55_837,(uint8*)"");
    
    return SCI_SUCCESS;
}


LOCAL uint32 Set_GT2005_Hmirror_Enable(uint32 enable)
{
    uint16 reg0x0101;
    reg0x0101 = GT2005_ReadReg(0x0101);

    if (enable)
    {
        reg0x0101 = (reg0x0101 | 0x01);
        GT2005_WriteReg(0x0101, reg0x0101);
    }
    else
    {
        reg0x0101 = (reg0x0101 & (~(0x01)));
        GT2005_WriteReg(0x0101, reg0x0101);
    }
    return 0;
}

LOCAL uint32 Set_GT2005_Vmirror_Enable(uint32 enable)
{
    uint16 reg0x0101;
    reg0x0101 = GT2005_ReadReg(0x0101);

    if (enable)
    {
        reg0x0101 = (reg0x0101 | 0x02);
        GT2005_WriteReg(0x0101, reg0x0101);
    }
    else
    {
        reg0x0101 = (reg0x0101 & (~(0x02)));
        GT2005_WriteReg(0x0101, reg0x0101);
    }
    return 0;
}
/******************************************************************************/
// Description: sensor probe function
// Author:     benny.zou
// Input:      none
// Output:     result
// Return:     0           successful
//             others      failed
// Note:       this function only to check whether sensor is work, not identify 
//              whitch it is!!
/******************************************************************************/
LOCAL uint32 Set_GT2005_Preview_Mode(uint32 preview_mode)
{
    switch (preview_mode)
    {
        case DCAMERA_ENVIRONMENT_NORMAL: 
        {
            GT2005_WriteReg(0x0312, 0x08);  // 1/2 Frame rate   
            break;
        }
        case DCAMERA_ENVIRONMENT_NIGHT:
        {
            GT2005_WriteReg(0x0312, 0x98);  // 1/3 Frame rate   
            break;
        }
        default:
        {
            break;
        }
     
    } 

    //SCI_TRACE_LOW:"set_GT2005_preview_mode: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_997_112_2_18_0_30_55_838,(uint8*)"d", preview_mode);
//  OS_TickDelay(100);
      return 0;
}
LOCAL uint32 GT2005_Identify(uint32 param)
{
#define GT2005_ID_H_VALUE   0x51
#define GT2005_ID_L_VALUE   0x38
        
    uint8 id_h_value = 0;
    uint8 id_l_value = 0;
    uint32 ret_value = 0xFF;

    id_h_value = GT2005_ReadReg(0x0000);
    //SCI_TRACE_LOW:"GT2005_Identify-id_h_value %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1011_112_2_18_0_30_56_839,(uint8*)"d", id_h_value);
    
    id_l_value = GT2005_ReadReg(0x0001);
    //SCI_TRACE_LOW:"GT2005_Identify-id_l_value %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1014_112_2_18_0_30_56_840,(uint8*)"d", id_l_value);

    if((GT2005_ID_H_VALUE == id_h_value) && (GT2005_ID_L_VALUE == id_l_value))
    {
            
        ret_value = 0;
        //SCI_TRACE_LOW:"It Is GT2005 Sensor !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1020_112_2_18_0_30_56_841,(uint8*)"");
    }

    GT2005_InitExifInfo();
    return ret_value;

}


LOCAL void GT2005_Write_Group_Regs( SENSOR_REG_T* sensor_reg_ptr )
{
    uint32 i;

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) || (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        GT2005_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

}


__align(4) const SENSOR_REG_T GT2005_brightness_tab[][2]=
{
    {{0x0201 , 0x90},{0xff , 0xff}},
    {{0x0201 , 0xb0},{0xff , 0xff}},
    {{0x0201 , 0xf0},{0xff , 0xff}},
    {{0x0201 , 0x00},{0xff , 0xff}},
    {{0x0201 , 0x10},{0xff , 0xff}},
    {{0x0201 , 0x30},{0xff , 0xff}},
    {{0x0201 , 0x70},{0xff , 0xff}}
};

LOCAL uint32 Set_GT2005_Brightness(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GT2005_brightness_tab[level];

//  SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/

    GT2005_Write_Group_Regs(sensor_reg_ptr);

    //SCI_TRACE_LOW:"set_GT2005_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1061_112_2_18_0_30_56_842,(uint8*)"d", level);

    return 0;
}



__align(4) const SENSOR_REG_T GT2005_contrast_tab[][2]=
{
    {{0x0200 , 0x90},{0xff , 0xff}},
    {{0x0200 , 0xb0},{0xff , 0xff}},
    {{0x0200 , 0xf0},{0xff , 0xff}},
    {{0x0200 , 0x00},{0xff , 0xff}},
    {{0x0200 , 0x10},{0xff , 0xff}},
    {{0x0200 , 0x30},{0xff , 0xff}},
    {{0x0200 , 0x70},{0xff , 0xff}}
};

LOCAL uint32 Set_GT2005_Contrast(uint32 level)
{
 
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GT2005_contrast_tab[level];

//  SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/

    GT2005_Write_Group_Regs(sensor_reg_ptr);

    //SCI_TRACE_LOW:"set_GT2005_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1088_112_2_18_0_30_56_843,(uint8*)"d", level);

    return 0;
}



__align(4) const SENSOR_REG_T GT2005_image_effect_tab[][4]= 
{
    //Normal
    {{0x0115 , 0x00},{0xff , 0xff},{0xff , 0xff},{0xff , 0xff}},
    //BLACK&WHITE
    {{0x0115 , 0x06},{0xff , 0xff},{0xff , 0xff},{0xff , 0xff}},
    //RED
    {{0x0115 , 0x0a},{0x026e , 0x70},{0x026f , 0xf0},{0xff , 0xff}},
    //GREEN
    {{0x0115 , 0x0a},{0x026e , 0x20},{0x026f , 0x00},{0xff , 0xff}},
    //BLUE
    {{0x0115 , 0x0a},{0x026e , 0xf0},{0x026f , 0x00},{0xff , 0xff}},
    //YELLOW
    {{0x0115 , 0x0a},{0x026e , 0x00},{0x026f , 0x80},{0xff , 0xff}},
    //NEGATIVE
    {{0x0115 , 0x09},{0xff , 0xff},{0xff , 0xff},{0xff , 0xff}},
    //SEPIA
    {{0x0115 , 0x0a},{0x026e , 0x30},{0x026f , 0x80},{0xff , 0xff}}
};

LOCAL uint32 Set_GT2005_Image_Effect(uint32 effect_type)
{
   
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GT2005_image_effect_tab[effect_type];

//  SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/

    GT2005_Write_Group_Regs(sensor_reg_ptr);

    //SCI_TRACE_LOW:"set_GT2005_image_effect: effect_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1124_112_2_18_0_30_56_844,(uint8*)"d", effect_type);

    return 0;
}



__align(4) const SENSOR_REG_T GT2005_ev_tab[][3]=
{   
    {{0x0300 , 0x81},{0x0301 , 0x20},{0xff , 0xff}},
    {{0x0300 , 0x81},{0x0301 , 0x40},{0xff , 0xff}},
    {{0x0300 , 0x81},{0x0301 , 0x60},{0xff , 0xff}},
    {{0x0300 , 0x81},{0x0301 , 0x80},{0xff , 0xff}},
    {{0x0300 , 0x81},{0x0301 , 0xa0},{0xff , 0xff}},
    {{0x0300 , 0x81},{0x0301 , 0xc0},{0xff , 0xff}},
    {{0x0300 , 0x81},{0x0301 , 0xe0},{0xff , 0xff}}
};

LOCAL uint32 Set_GT2005_Ev(uint32 level)
{
  
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GT2005_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/
     GT2005_Write_Group_Regs(sensor_reg_ptr );

    //SCI_TRACE_LOW:"set_GT2005_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1150_112_2_18_0_30_56_845,(uint8*)"d", level);

    return 0;
}



/******************************************************************************/
// Description: anti 50/60 hz banding flicker
// Global resource dependence: 
// Author:
// Note:
//      
/******************************************************************************/
LOCAL uint32 Set_GT2005_Anti_Flicker(uint32 mode)
{ 
    //PLL Setting 18.6FPS Under 48MHz PCLK
    GT2005_WriteReg(0x0116 , 0x02);
    GT2005_WriteReg(0x0118 , 0x40);
    GT2005_WriteReg(0x0119 , 0x01);
    GT2005_WriteReg(0x011a , 0x04); 
    GT2005_WriteReg(0x011B , 0x00);

    GT2005_WriteReg(0x0313 , 0x35); 
    GT2005_WriteReg(0x0314 , 0x36); 

    switch(mode)
    {
        case DCAMERA_FLICKER_50HZ:
        GT2005_WriteReg(0x0315 , 0x16);                             
        break;

        case DCAMERA_FLICKER_60HZ:
        GT2005_WriteReg(0x0315 , 0x56);                             
        break;

        default:
        return 0;
    }

    //SCI_TRACE_LOW:"set_GT2005_anti_flicker-mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1188_112_2_18_0_30_56_846,(uint8*)"d",mode);
//  OS_TickDelay(200); 

    return 0;
}






__align(4) const SENSOR_REG_T GT2005_awb_tab[][10]=
{
    //Auto
    {
        {0x031a , 0x81},                                                             
        {0x0320 , 0xE1},
        {0x0321 , 0x1A},                
        {0x0322 , 0xDE},        
        {0x0323 , 0x11},
        {0x0441 , 0x4B},
        {0x0442 , 0x00},
        {0x0443 , 0x00},
        {0x0444 , 0x31},
        {0xff , 0xff}
    },

    //Office
    {
        {0x0320 , 0x02},
        {0x0321 , 0x02},
        {0x0322 , 0x02},
        {0x0323 , 0x02},
        {0x0441 , 0x32},
        {0x0442 , 0x1c},
        {0x0443 , 0x1c},
        {0x0444 , 0x9d},
        {0xff , 0xff},
        {0xff , 0xff}
    },
    
    //U30  //not use
    {
        {0xff , 0xff},
        {0xff , 0xff},
        {0xff , 0xff},       
        {0xff , 0xff},    
        {0xff , 0xff},  
        {0xff , 0xff},
        {0xff , 0xff},
        {0xff , 0xff},         
        {0xff , 0xff},
           {0xff , 0xff}
    },  
    
    //CWF  //not use
    {
        {0xff , 0xff},
        {0xff , 0xff},
        {0xff , 0xff},       
        {0xff , 0xff},    
        {0xff , 0xff},  
        {0xff , 0xff},
        {0xff , 0xff},
        {0xff , 0xff},         
        {0xff , 0xff},
        {0xff , 0xff}
    }, 
    
    //HOME
    {
        {0x0320 , 0x02},
        {0x0321 , 0x02},
        {0x0322 , 0x02},
        {0x0323 , 0x02},
        {0x0441 , 0x3a},
        {0x0442 , 0x17},
        {0x0443 , 0x17},
        {0x0444 , 0x6a},   
        {0xff, 0xff},
        {0xff, 0xff}
    },  
    
    //SUN:
    {
        {0x0320 , 0x02},
        {0x0321 , 0x02},
        {0x0322 , 0x02},
        {0x0323 , 0x02},  
        {0x0441 , 0x48},
        {0x0442 , 0x16},
        {0x0443 , 0x16},
        {0x0444 , 0x3d},     
        {0xff , 0xff},
        {0xff , 0xff}
    },
    
    //CLOUD:
    {
        {0x0320 , 0x02},
        {0x0321 , 0x02},
        {0x0322 , 0x02},
        {0x0323 , 0x02},  
        {0x0441 , 0x3a},
        {0x0442 , 0x04},
        {0x0443 , 0x04},
        {0x0444 , 0x12}, 
        {0xff , 0xff},
        {0xff , 0xff}
    }
};

LOCAL uint32 Set_GT2005_AWB(uint32 mode)
{
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GT2005_awb_tab[mode];
    
//  SCI_ASSERT(PNULL != sensor_reg_ptr);    /*assert verified*/
    
    GT2005_Write_Group_Regs(sensor_reg_ptr);
    
//  OS_TickDelay(100); 
    //SCI_TRACE_LOW:"set_GT2005_awb_mode: mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1310_112_2_18_0_30_56_847,(uint8*)"d", mode);
    
    return 0;
}



LOCAL uint32 GT2005_AE_Enable(uint32 param)
{   

 //      SCI_TRACE_LOW("SENSOR: set_ae_awb_enable: ae_enable = %d", ae_enable);
 //      SCI_TRACE_LOW("SENSOR: set_ae_awb_enable: ae_enable = %d", awb_enable);
    return 0;
}



LOCAL void    GT2005_Set_Shutter(void)
{
    uint16 shutter,AGain_shutter,DGain_shutter;

    //GT2005_WriteReg(0x020B , 0x28);
    //GT2005_WriteReg(0x020C , 0x44);
    //GT2005_WriteReg(0x040B , 0x44);

    GT2005_WriteReg(0x0300 , 0xc1);

    shutter = (GT2005_ReadReg(0x0012)<<8 )|( GT2005_ReadReg(0x0013));    
    AGain_shutter = (GT2005_ReadReg(0x0014)<<8 )|( GT2005_ReadReg(0x0015));
    DGain_shutter = (GT2005_ReadReg(0x0016)<<8 )|( GT2005_ReadReg(0x0017));
    GT2005_WriteReg(0x0300 , 0x41); //close ALC

    //shutter = shutter / 2; 

    GT2005_WriteReg(0x0305 , shutter&0xff);           
    GT2005_WriteReg(0x0304 , (shutter>>8)&0xff); 

    GT2005_WriteReg(0x0307 , AGain_shutter&0xff);      
    GT2005_WriteReg(0x0306 , (AGain_shutter>>8)&0xff); //AG

    GT2005_WriteReg(0x0308,  (DGain_shutter>>2)&0xff);   //DG
    
}


LOCAL uint32 GT2005_Before_Snapshot(uint32 param)
{
    GT2005_WriteReg(0x0300 , 0xc1);
    GT2005_Set_Shutter();
    Sensor_SetMode(param);
    //SCI_TRACE_LOW:"SENSOR_GT2005: Before Snapshot"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1360_112_2_18_0_30_56_848,(uint8*)"");

    return 0;
}



LOCAL uint32 GT2005_After_Snapshot(uint32 para)
{
    //GT2005_WriteReg(0x0119 , 0x01);
    //GT2005_WriteReg(0x011b , 0x00);   
    GT2005_WriteReg(0x0300 , 0x81);
    //OS_TickDelay(100);
    GT2005_Change_Image_Format(SENSOR_IMAGE_FORMAT_YUV422);
    
    //SCI_TRACE_LOW:"SENSOR_GT2005: After Snapshot"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1375_112_2_18_0_30_56_849,(uint8*)"");

    return 0;
}

LOCAL uint32 GT2005_Change_Image_Format(uint32 param)
{
    SENSOR_REG_TAB_INFO_T st_jpeg_reg_table_info = { ADDR_AND_LEN_OF_ARRAY(GT2005_JPEG_MODE), 0,0,0, 0};
    SENSOR_REG_TAB_INFO_T st_yuv422_reg_table_info = { ADDR_AND_LEN_OF_ARRAY(GT2005_YUV_COMMON),0,0,0,0};
    uint32 ret_val = SCI_ERROR;

    switch(param)
    {
        case SENSOR_IMAGE_FORMAT_YUV422:
            ret_val = Sensor_SendRegTabToSensor(&st_yuv422_reg_table_info);
            break;

        case SENSOR_IMAGE_FORMAT_JPEG:
            ret_val = Sensor_SendRegTabToSensor(&st_jpeg_reg_table_info);
            break;

        default:
                   break;
    }

    return ret_val;
}

 LOCAL uint32 Check_GT2005_Image_Format_Support(uint32 param) 
{
    uint32 ret_val = SCI_ERROR;

    switch(param)
    {
    case SENSOR_IMAGE_FORMAT_YUV422:
        ret_val = SCI_SUCCESS;
        break;

    case SENSOR_IMAGE_FORMAT_JPEG:
        ret_val = SCI_ERROR;//SCI_SUCCESS;
        break;

    default:
        SCI_PASSERT(0, ("GT2005 only support SENSOR_IMAGE_FORMAT_JPEG & SENSOR_IMAGE_FORMAT_YUV422, input is %d", param));  /*assert to do*/
        break;
    }

    return ret_val;
}
/******************************************************************************/
// Description: set video mode
// Global resource dependence: 
// Author:
// Note:
//       
/******************************************************************************/
__align(4) const SENSOR_REG_T GT2005_video_mode_nor_tab[][8]=
{
    // normal mode 
    {{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff}},    
    //vodeo mode     10fps Under 13MHz MCLK
    {{0x0116 , 0x01},{0x0118 , 0x45},{0x0119 , 0x02},{0x011a , 0x04},{0x011B , 0x02},{0x0313 , 0x32}, {0x0314 , 0xCE}, {0xff , 0xff}},
    // UPCC  mode     10fps Under 13MHz MCLK
    {{0x0116 , 0x01},{0x0118 , 0x45},{0x0119 , 0x02},{0x011a , 0x04},{0x011B , 0x02},{0x0313 , 0x32}, {0x0314 , 0xCE}, {0xff , 0xff}}
};    


LOCAL uint32 Set_GT2005_Video_Mode(uint32 mode)
{
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)GT2005_video_mode_nor_tab[mode];

    SCI_ASSERT(mode <=DCAMERA_MODE_MAX);    /*assert to do*/
//    SCI_ASSERT(PNULL != sensor_reg_ptr);  /*assert verified*/

    GT2005_Write_Group_Regs(sensor_reg_ptr );

    //SCI_TRACE_LOW:"set_GT2005_video_mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1450_112_2_18_0_30_56_850,(uint8*)"d",mode);
    return SCI_SUCCESS;
}   

LOCAL uint32 GT2005_InitExifInfo(void)
{
    EXIF_SPEC_PIC_TAKING_COND_T* exif_ptr=&s_GT2005_exif;

    //SCI_TRACE_LOW:"SENSOR: GT2005_InitExifInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_GT2005_1457_112_2_18_0_30_57_851,(uint8*)"");

    exif_ptr->valid.FNumber = 1;
    exif_ptr->FNumber.numerator = 14;
    exif_ptr->FNumber.denominator = 5;
    
    exif_ptr->valid.ExposureProgram = 1;
    exif_ptr->ExposureProgram = 0x04;

    //exif_ptr->SpectralSensitivity[MAX_ASCII_STR_SIZE];    
    //exif_ptr->ISOSpeedRatings;
    //exif_ptr->OECF;
    
    //exif_ptr->ShutterSpeedValue;
    
    exif_ptr->valid.ApertureValue=1;
    exif_ptr->ApertureValue.numerator=14;
    exif_ptr->ApertureValue.denominator=5;
    
    //exif_ptr->BrightnessValue;
    //exif_ptr->ExposureBiasValue;

    exif_ptr->valid.MaxApertureValue=1;
    exif_ptr->MaxApertureValue.numerator=14;
    exif_ptr->MaxApertureValue.denominator=5;
    
    //exif_ptr->SubjectDistance;
    //exif_ptr->MeteringMode;
    //exif_ptr->LightSource;
    //exif_ptr->Flash;

    exif_ptr->valid.FocalLength=1;
    exif_ptr->FocalLength.numerator=289;
    exif_ptr->FocalLength.denominator=100;
    
    //exif_ptr->SubjectArea;
    //exif_ptr->FlashEnergy;
    //exif_ptr->SpatialFrequencyResponse;
    //exif_ptr->FocalPlaneXResolution;
    //exif_ptr->FocalPlaneYResolution;
    //exif_ptr->FocalPlaneResolutionUnit;
    //exif_ptr->SubjectLocation[2];
    //exif_ptr->ExposureIndex;
    //exif_ptr->SensingMethod;

    exif_ptr->valid.FileSource=1;
    exif_ptr->FileSource=0x03;

    //exif_ptr->SceneType;
    //exif_ptr->CFAPattern;
    //exif_ptr->CustomRendered;

    exif_ptr->valid.ExposureMode=1;
    exif_ptr->ExposureMode=0x00;

    exif_ptr->valid.WhiteBalance=1;
    exif_ptr->WhiteBalance=0x00;
    
    //exif_ptr->DigitalZoomRatio;
    //exif_ptr->FocalLengthIn35mmFilm;
    //exif_ptr->SceneCaptureType;   
    //exif_ptr->GainControl;
    //exif_ptr->Contrast;
    //exif_ptr->Saturation;
    //exif_ptr->Sharpness;
    //exif_ptr->DeviceSettingDescription;
    //exif_ptr->SubjectDistanceRange;

    return SCI_SUCCESS;
}

LOCAL uint32 GT2005_GetExifInfo(uint32 param)
{
    return (uint32)&s_GT2005_exif;
}   
#endif


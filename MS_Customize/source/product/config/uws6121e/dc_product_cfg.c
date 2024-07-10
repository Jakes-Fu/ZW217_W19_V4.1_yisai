/******************************************************************************
 ** File Name:      dc_cfg.c                                           *
 ** Author:         Tim.zhu                                             *
 ** DATE:           11/16/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product configure dc parameter    *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/16/2009     Tim.zhu	  Create.                         		  *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "os_api.h"
#include "dc_product_cfg.h"
#include "dc_cfg.h"
/*lint -save -e551 */
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL uint32 _DC_GetProductCfgInfo(uint32 param);
LOCAL uint32 _DC_GetExifPrimaryPriDescInfo(uint32 param);
LOCAL uint32 _DC_GetExifSpecUserInfo(uint32 param);

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/

#if (defined(LOW_MEMORY_SUPPORT_16X16) || defined(LOW_MEMORY_SUPPORT_32X16))
#ifdef MULTI_BIN_RAM_32MBIT_LEVEL4
#define DC_MEM_SIZE        (620*1024) //(550*1024)
#define DV_MEM_SIZE        (620*1024) //(520*1024)
#define SP_DC_MEM_SIZE     (400*1024)
#define ATV_MEM_SIZE       (400*1024)
#define MINIDC_MEM_SIZE    (400*1024)
#define MINIDV_MEM_SIZE    (400*1024)
#define UPCC_MEM_SIZE      (400*1024)
#define VIDEOCALL_MEM_SIZE (320 * 1024)
#else
#define DC_MEM_SIZE        (620*1024)
#define DV_MEM_SIZE        (620*1024)
#define SP_DC_MEM_SIZE     (520*1024)
#define ATV_MEM_SIZE       (520*1024)
#define MINIDC_MEM_SIZE    (520*1024)
#define MINIDV_MEM_SIZE    (520*1024)
#define UPCC_MEM_SIZE      (520*1024)
#define VIDEOCALL_MEM_SIZE (320 * 1024)
#endif

#else

/***********************************
* calculate mem size:
* yuv420 : maximum width*height*1.5 + 20*1024
* yuv422 : maximum width*height*2   + 20*1024
************************************/
#ifdef DC_2M_SUPPORT
#define DC_MEM_SIZE        (620*1024)
#else
#define DC_MEM_SIZE        (620*1024)
#define DV_MEM_SIZE        (620*1024)
#endif

#define VIDEOCALL_MEM_SIZE (320 * 1024)

//#define DC_MEM_SIZE        (550*1024)
//#define DV_MEM_SIZE        (520*1024)
#define SP_DC_MEM_SIZE     (520*1024)
#define ATV_MEM_SIZE       (520*1024)
#define MINIDC_MEM_SIZE    (520*1024)
#define MINIDV_MEM_SIZE    (520*1024)
#define UPCC_MEM_SIZE      (520*1024)	

#if 0
#define SP_DC_MEM_SIZE     (1200*1024)
#define DV_MEM_SIZE        (1200*1024)
#define ATV_MEM_SIZE       (1200*1024)
#define MINIDC_MEM_SIZE    (600*1024)
#define MINIDV_MEM_SIZE    (1000*1024)
#define UPCC_MEM_SIZE      (1200*1024)
#endif

#endif

/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/

LOCAL const EXIF_PRI_DESC_T s_dc_exif_pri_desc_info=
{
    {
        0x00,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01,
        0x01
    },
    "Default",          //Date
    "Default Co. Ltd",  //ImageDescription
    "Default",          //Make
    "Default",          //Model
    "Default v0.0.0",   //Software
    "Default",          //Artist
    "Default"           //Copyright
};

LOCAL EXIF_SPEC_USER_T s_dc_exif_spec_user_info={0x00};

LOCAL const DC_PRODUCT_CFG_T s_dc_product_cfg_info=
{
    SCI_FALSE,
    SCI_FALSE,
    SCI_FALSE,
    SCI_FALSE,
    DC_MEM_SIZE,
    DV_MEM_SIZE,
    VIDEOCALL_MEM_SIZE,
    ATV_MEM_SIZE,
    SP_DC_MEM_SIZE,
    MINIDC_MEM_SIZE,
    MINIDV_MEM_SIZE,
    UPCC_MEM_SIZE,
    DC_MAX_VIDEO_MODE_QVGA,
    DC_PRODUCT_FLASH_TYPE_DISABLE,
    DC_PRODUCT_MEM_TYPE_NOR
};

// custom cfg function
LOCAL const DC_PRODUCT_CFG_FUNC_TAB_T s_dc_product_cfg_fun=
{
    _DC_GetProductCfgInfo,
    _DC_GetExifPrimaryPriDescInfo,
    _DC_GetExifSpecUserInfo,
    PNULL
};
/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  This function is used to get dc custom cfg
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL uint32 _DC_GetProductCfgInfo(uint32 param)
{    
    return (uint32)&s_dc_product_cfg_info;
}

/*****************************************************************************/
//  Description:  This function is used to get exif primary pri desc
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL uint32 _DC_GetExifPrimaryPriDescInfo(uint32 param)
{

    return (uint32)&s_dc_exif_pri_desc_info;
}

/*****************************************************************************/
//  Description:  This function is used to get exif primary pri desc
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
LOCAL uint32 _DC_GetExifSpecUserInfo(uint32 param)
{
    
    return (uint32)&s_dc_exif_spec_user_info;
}

/*****************************************************************************/
//  Description:  This function is used to get dc custom cfg function    
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC DC_PRODUCT_CFG_FUNC_TAB_T_PTR DC_GetDcProductCfgFun(void)
{
    return (DC_PRODUCT_CFG_FUNC_TAB_T_PTR)&s_dc_product_cfg_fun;
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // end of dc_cfg.c    


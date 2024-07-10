/******************************************************************************
 ** File Name:      dc_product_cfg.h                                           *
 ** Author:         Tim.zhu                                             *
 ** DATE:           10/14/2010                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the product configure dc parameter    *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/14/2010     Tim.zhu	  Create.                         		  *
 ******************************************************************************/

#ifndef DC_PRODUCT_CFG_H_
#define DC_PRODUCT_CFG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

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

/**---------------------------------------------------------------------------*
 **                         Macro define         	                          *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef enum
{
    DC_MAX_VIDE0_MODE_QCIF=0x01,
    DC_MAX_VIDEO_MODE_QVGA,
    DC_MAX_VIDEO_MODE_CIF,
    DC_MAX_VIDEO_MODE_VGA,
    DC_MAX_VIDEO_MODE_MAX
}DC_MAX_VIDEO_MODE_E;

typedef enum
{
    DC_PRODUCT_FLASH_TYPE_DISABLE=0x00,
    DC_PRODUCT_FLASH_TYPE_LED,
    DC_PRODUCT_FLASH_TYPE_XENON,
    DC_PRODUCT_FLASH_TYPE_MAX
}DC_PRODUCT_FLASH_TYPE_E;

typedef enum
{
    DC_PRODUCT_MEM_TYPE_NAND=0x00,
    DC_PRODUCT_MEM_TYPE_NOR,
    DC_PRODUCT_MEM_TYPE_MAX
}DC_PRODUCT_MEM_TYPE_E;

typedef enum
{
    DC_PRODUCT_IOCTL_CMD_MAX
}DC_PRODUCT_IOCTL_CMD_E;

typedef struct _dc_product_mem_cfg_tag
{
    BOOLEAN exif_eb;
    BOOLEAN thumbnail_eb;
    BOOLEAN reverse1;
    BOOLEAN reverse0;
    uint32 dc_mode_mem;
    uint32 dv_mode_mem;
    uint32 vt_mode_mem;
    uint32 atv_mode_mem;
    uint32 sp_mode_mem;
    uint32 minidc_mode_mem;
    uint32 minidv_mode_mem;
    uint32 upcc_mode_mem;
    DC_MAX_VIDEO_MODE_E max_video_mode;
    DC_PRODUCT_FLASH_TYPE_E flash_type;
    DC_PRODUCT_MEM_TYPE_E mem_type;
} DC_PRODUCT_CFG_T, *DC_PRODUCT_CFG_T_PTR;

typedef struct _dc_product_cfg_func_tab_tag
{
    uint32(*get_productcfg)(uint32 param);
    uint32(*get_exifprimarypridesc)(uint32 param);
    uint32(*get_exifspecuser)(uint32 param);
    uint32(*dc_product_ioctl)(DC_PRODUCT_IOCTL_CMD_E ctl_cmd, void* param);
}DC_PRODUCT_CFG_FUNC_TAB_T, *DC_PRODUCT_CFG_FUNC_TAB_T_PTR;


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:  This function is used to get dc Param    
//  Author:         Tim.zhu
//  Note:           
/*****************************************************************************/
PUBLIC DC_PRODUCT_CFG_FUNC_TAB_T_PTR DC_GetDcProductCfgFun(void);

/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif 

#endif

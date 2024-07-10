/*******************************************************************************
 ** FileName:      dcamera_cfg.c                                               *
 ** Author:                                                                    *
 ** Date:                                                                      *
 ** CopyRight:                                                                 *
 ** Description:   Camera 方案配置                                                 *
 *******************************************************************************/
#ifndef DCAMERA_CFG_H
#define DCAMERA_CFG_H

#include "ms_ref_dc_trc.h"
#include "sci_types.h"
#include "os_api.h"
//#include "dcamera_common.h"
#include "isp_service_uws6121e.h"

#ifdef   __cplusplus
extern   "C"
{
#endif



typedef struct
{
    HAL_CAMERA_CFG_T hal_cfg;

    //0:NO rotate 1: Clockwize 90 2: 180 3:270
    int preview_rot;      //预览旋转
    int enc_rot;

    uint32 width;            //该方案输出YUV数据宽度
    uint32 height;           //该方案输出YUV数据高度

	uint32 display_width;    //实际显示在屏幕上的YUV数据宽度
	uint32 display_height;   //实际显示在屏幕上的YUV数据高度

}DC_CAMERA_CFG, *PDC_CAMERA_CFG;


typedef enum
{
    DCAMERA_MODE_PREVIEW = 0,
    DCAMERA_MODE_SNAPSHOT,
    DCAMERA_MODE_RECORD,
    DCAMERA_MODE_VIDEOCALL,
    DCAMERA_MODE_MAXIMUM,

}DCAMERA_MODE_E;

typedef enum
{
    DCAMERA_DUMP_INVALID = 0,
    DCAMERA_DUMP_DDR = 1,
    DCAMERA_DUMP_FILE,
    DCAMERA_DUMP_MODE_MAX
}DCAMERA_DUMP_MODE_E;


typedef struct
{
    uint32 (*gouda_offset)(int mode, DC_CAMERA_CFG ** param);
    int (*gouda_config)(int mode, BLOCKCFG_T* cfg, DC_CAMERA_CFG ** param);

    uint32 (*preview_config)(DC_CAMERA_CFG* param);
    uint32 (*snapshot_config)(DC_CAMERA_CFG* param);
    uint32 (*record_config)(DC_CAMERA_CFG* param);
    uint32 (*videocall_config)(DC_CAMERA_CFG* param);

}DCAMERA_IOCTL_CFG_T,*PDCAMERA_IOCTL_CFG_T;


extern const  DC_CAMERA_CFG *p_Dcam_cfg[];

PDC_CAMERA_CFG Dcamera_get_config(int mode);

int Dcamera_Gouda_config(int mode, BLOCKCFG_T* cfg);
uint32 DCamera_get_GoudaOffset(int mode);

#ifdef   __cplusplus
}
#endif


#endif

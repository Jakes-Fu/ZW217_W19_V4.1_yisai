/*******************************************************************************
 ** FileName:      dcamera_cfg.c                                               *
 ** Author:                                                                    *
 ** Date:                                                                      *
 ** CopyRight:                                                                 *
 ** Description:   8910 Camera ·½°¸ÅäÖÃ                                        *
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



static PDCAMERA_IOCTL_CFG_T p_Dcam_ioctl = NULL;

#if defined(W217_W18_DRV_CAMERA_GC6133)
extern const DCAMERA_IOCTL_CFG_T  g_dcam_cfg_8W_240x240;
#else
//extern const DCAMERA_IOCTL_CFG_T  g_dcam_cfg_8W_128x128;
//extern const DCAMERA_IOCTL_CFG_T  g_dcam_cfg_8W_240x240;
extern const DCAMERA_IOCTL_CFG_T  g_dcam_cfg_30W_240x240;
//extern const DCAMERA_IOCTL_CFG_T  g_dcam_cfg_30W_128x128;
#endif



static DC_CAMERA_CFG s_Preview_cfg;
static DC_CAMERA_CFG s_Snapshot_cfg;
static DC_CAMERA_CFG s_Record_cfg;
static DC_CAMERA_CFG s_VideoCall_cfg;

const DC_CAMERA_CFG *p_Dcam_cfg[] =

                             { &s_Preview_cfg,
                               &s_Snapshot_cfg,
                               &s_Record_cfg,
                               &s_VideoCall_cfg,
                             };

PDCAMERA_IOCTL_CFG_T camera_get_cfg()
{
#if defined(W217_W18_DRV_CAMERA_GC6133)
        return (DCAMERA_IOCTL_CFG_T*) &g_dcam_cfg_8W_240x240;
#else
	//return (DCAMERA_IOCTL_CFG_T*) &g_dcam_cfg_8W_128x128;
	//return (DCAMERA_IOCTL_CFG_T*) &g_dcam_cfg_8W_240x240;
	return (DCAMERA_IOCTL_CFG_T*) &g_dcam_cfg_30W_240x240;
	//return (DCAMERA_IOCTL_CFG_T*) &g_dcam_cfg_30W_128x128;
#endif
}


int Dcamera_Gouda_config(int mode, BLOCKCFG_T* cfg)
{
    if(!cfg || !p_Dcam_ioctl)
    {
        SCI_PASSERT(SCI_FALSE, ("Dcamera_Gouda_config Error!"));
    }

    return p_Dcam_ioctl->gouda_config(mode, cfg, p_Dcam_cfg);

}

uint32 DCamera_get_GoudaOffset(int mode)
{
    SCI_TRACE_LOW("DC:get_GoudaOffset");
    return p_Dcam_ioctl->gouda_offset(mode, p_Dcam_cfg);
}




void Dcamera_init_config()
{
    uint32 iRet = 0;

    p_Dcam_ioctl = camera_get_cfg();
    if(!p_Dcam_ioctl)
    {
        SCI_PASSERT(SCI_FALSE, ("camera_get_cfg Error!"));
    }

    iRet = p_Dcam_ioctl->preview_config(&s_Preview_cfg);
    if(iRet < 0)
    {
        SCI_PASSERT(SCI_FALSE, ("preview_config Error!"));
    }

    iRet = p_Dcam_ioctl->snapshot_config(&s_Snapshot_cfg);
    if(iRet < 0)
    {
        SCI_PASSERT(SCI_FALSE, ("snapshot_config Error!"));
    }

    iRet = p_Dcam_ioctl->record_config(&s_Record_cfg);
	if(iRet < 0)
    {
        SCI_PASSERT(SCI_FALSE, ("record_config Error!"));
    }

    iRet = p_Dcam_ioctl->videocall_config(&s_VideoCall_cfg);
	if(iRet < 0)
    {
        SCI_PASSERT(SCI_FALSE, ("videocall_config Error!"));
    }


     SCI_TRACE_LOW("DC:init_config over");

}


#ifdef   __cplusplus
}
#endif


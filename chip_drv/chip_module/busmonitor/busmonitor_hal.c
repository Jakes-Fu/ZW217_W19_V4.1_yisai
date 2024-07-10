/******************************************************************************
 ** File Name:      busmonitor_hal.c                                                *
 ** Author:         liuhao                                                   *
 ** DATE:           07/30/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define the hal layer of I2C device.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/30/2010     liuhao     Create.                                   *
 ******************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"

#include "busmonitor_drvapi.h"
#include "busmonitor_phy.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_BM
#ifdef DEBUG_BM
#define BM_PRINT   SCI_TRACE_LOW
#else
#define BM_PRINT
#endif

/**---------------------------------------------------------------------------*
 **                            Macro Define
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                            Local Variables
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                            Global Variables
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
PUBLIC void bm_disable (void)
{
    bm_close();
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
PUBLIC void bm_monitor_set(
			uint32 bm_mode,
			const struct bm_setting *cfg,
			ON_BM_RAISE bm_callback
)
{
    BM_POINT_SETTING setting = {0};

    setting.addr_min = cfg->addr_min;
    setting.addr_max = cfg->addr_max;
    setting.addr_mask = 0;

    setting.data_min = cfg->data_min;
    setting.data_max = cfg->data_max;
    setting.data_mask = 0;

    setting.access_mode = bm_mode;
    setting.size_en = BM_DISABLE;
    setting.burst_en = BM_DISABLE;
    setting.chn_sel = BM_ARM_DATA;
    setting.master_en = BM_DISABLE;
    setting.on_raise = bm_callback;

    bm_set_point (0, &setting);
}

/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
PUBLIC void bm_monitor_get (
    uint32 *bm_match_addr,
    uint32 *bm_match_data
)
{
    bm_get_match (bm_match_addr, bm_match_data);
}

#if 1
uint32 bm_test_var[16];
/*********************************************************************************************************
** Function name:
** Descriptions:
** input parameters:
**
**
**
** output parameters:
** Returned value:
*********************************************************************************************************/
PUBLIC void bm_test (void)
{
	struct bm_setting cfg;

	cfg.addr_min = &bm_test_var[0];
    cfg.addr_max = &bm_test_var[15];
    cfg.data_min = 25;
    cfg.data_max = 0xfffffffe;
	
    bm_monitor_set(BM_ACCESS_MODE_WRITE, &cfg, NULL);

    bm_test_var[4] = 0x12213443;
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

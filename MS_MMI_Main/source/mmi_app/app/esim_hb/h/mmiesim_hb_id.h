#ifndef  _MMIESIM_HB_ID_H_
#define  _MMIESIM_HB_ID_H_
 /**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
   extern   "C"
   {
#endif

#define WIN_ID_DEF(win_id)          win_id

typedef enum
{
    ESIM_WIN_ID_START = (MMI_MODULE_ESIM_HB << 16),

    #include "mmiesim_hb_id.def"
    ESIM_MAX_WIN_ID
}ESIM_WINDOW_ID_E;


#undef WIN_ID_DEF

// 命名规则: 模块名 + 窗口名 + WIN_ID  ,  eg: CC_DIALING_WIN_ID (CC部分呼叫窗口ID)
typedef enum
{
    ESIM_CTRL_ID_START = ESIM_MAX_WIN_ID,
    ESIM_INFO_LIST_CTRL_ID,
    ESIM_ICCID_CTRL_ID,
    ESIM_EUICC_CTRL_ID,
    ESIM_MAX_CTRL_ID
}ESIM_CTRL_ID_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif


#ifndef  _MMIJAVA_ID_H_    
#define  _MMIJAVA_ID_H_   

#include "mmi_module.h"
#include "os_api.h"
#include "mmk_type.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif

#define WIN_ID_DEF(win_id)          win_id
// window ID
typedef enum
{
    JAVA_WIN_ID_START = (MMI_MODULE_JAVA << 16),

#include "mmijava_id.def"

    JAVA_MAX_WIN_ID
}JAVA_WINDOW_ID_E;

#undef WIN_ID_DEF

typedef enum
{
    MMIJAVA_CTRL_ID_START = JAVA_MAX_WIN_ID,
    MMIJAVA_CTRL_ID_IM,
    MMIJAVA_CTRL_ID_EXIT_VM
}MMIJAVA_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIJAVA_RegWinIdNameArr(void);

/*****************************************************************************/
// 	Description : get sun java module id
//	Global resource dependence : none
//  Author: yihg.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIJAVA_GetModuleID(void);


#ifdef   __cplusplus
    }
#endif

#endif


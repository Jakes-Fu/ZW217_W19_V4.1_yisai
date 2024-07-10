

#include "std_header.h"
#include "sci_types.h"
#include "mmi_module.h"
#include "Mmi_modu_main.h"

// #define WIN_ID_DEF(win_id)          #win_id

const uint8 apple_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
	"APPLE_MAIN_WIN_ID"   //APPLE_MAIN_WIN_ID
};

// #undef WIN_ID_DEF


/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void Apple_RegWinIdNameArr(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_SXH_APPLE, (const uint8**)apple_id_name_arr);
}


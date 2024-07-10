


#include "std_header.h"
#include "sci_types.h"

#include "mmi_modu_main.h"

/*the length of apple nv*/
const uint16 apple_nv_len[]=
{
    64,                         // APPLE_GLOBAL_SET,    
};

/*****************************************************************************/
// 	Description : register game module nv len and max item
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void Apple_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_SXH_APPLE, apple_nv_len, sizeof(apple_nv_len) / sizeof(uint16));
}













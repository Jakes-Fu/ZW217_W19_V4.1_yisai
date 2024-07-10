#ifdef WRE_SUPPORT

#include "sci_types.h"
#include "mmi_module.h"
#include "mmiwre.h"

const uint16 mmiwre_nv_len[] =
{      
    sizeof(MMIWRE_RUNTIME_PATH_T),
    sizeof(uint32),	//added by leichi for wre audio and vido volume	
};
 
/*****************************************************************************/
// 	Description : register wre module nv len and max item
//	Global resource dependence : none
//  Author: Qingjun.YU
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWRE_RegWreNv(void)
{      
    MMI_RegModuleNv(MMI_MODULE_WRE, mmiwre_nv_len, sizeof(mmiwre_nv_len) / sizeof(uint16));
    return TRUE;
}

#endif

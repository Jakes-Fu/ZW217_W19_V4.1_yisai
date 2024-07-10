#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmiesim_hb_id.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 esim_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmiesim_hb_id.def"
};

#undef WIN_ID_DEF

PUBLIC void ESIM_HB_RegIdNameArr(void){
    MMI_RegWinIdNameArr(MMI_MODULE_ESIM_HB, (const uint8 **)esim_id_name_arr);
}

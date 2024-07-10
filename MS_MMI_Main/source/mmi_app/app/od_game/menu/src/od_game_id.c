#ifdef OD_GAME_SUPPORT
#include "std_header.h"
#include "sci_types.h"
#include "Mmi_modu_main.h"

#define WIN_ID_DEF(win_id, win_id_name)          win_id_name,

const uint8 od_game_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
#include "od_game_id.def"    
};

#undef WIN_ID_DEF

PUBLIC void od_game_RegWinIdNameArr(void)
{
#ifndef MMI_AUTOTEST_LOW_MEMORY
    MMI_RegWinIdNameArr(MMI_MODULE_OD_GAME, od_game_id_name_arr);
#endif
}
#endif

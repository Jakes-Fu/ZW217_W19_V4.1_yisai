#ifndef _OD_GAME_BILLING_H_
#define _OD_GAME_BILLING_H_
#include "od_game_profile.h"

typedef struct
{
    OD_GAME_TYPE game_type;
    OD_GAME_SIM_TYPE sim_type;
    OD_U8 imei[16];
    OD_U32 plmn;
    OD_U16 unlock_type;
    OD_U16 random_code;
    OD_U8 free_trials;
    OD_FuncPtr entry_func_ptr;
    od_game_profile_struct profile;
}OD_GAME_BILLING_INFO;


extern void od_game_buy_lsk_function(OD_U16 cur_selection);
extern void od_game_unlock_done_func(OD_U32 inputcode);
extern void od_game_buy_again();
extern void od_game_send_billing_sms();
#endif

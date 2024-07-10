#ifndef _OD_GAME_PROFILE_H_
#define _OD_GAME_PROFILE_H_
#include "od_game_menu.h"
typedef struct
{
    OD_U32 profile_id;
    OD_U8 short_code[20];
    OD_U8 keyword[20];
    OD_U8 price[10];
    OD_U8 currency_code[5];  
    OD_U8 no_mo;
    OD_BOOL vat_text_is_needed;
    OD_BOOL sms_count_text_is_needed;
    OD_U16 provider_id;
}od_game_profile_struct;

typedef struct
{
    OD_U32 plmn;
    const od_game_profile_struct* profile;
}od_game_profile_data_struct;

extern OD_BOOL od_game_get_profile_info(OD_U32 plmn,od_game_profile_struct* profile);
#endif

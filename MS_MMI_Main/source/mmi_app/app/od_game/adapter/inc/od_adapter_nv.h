#ifndef __OD_ADAPTER_NV_H__
#define __OD_ADAPTER_NV_H__
#include "od_game_menu.h"
#include "sci_types.h"
#include "mmi_module.h"


#define OD_MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define OD_MMINV_WRITE(_item_id,_addr) \
			 MMI_WriteNVItem(_item_id, _addr)
			 
typedef enum
{  
	MMINV_OD_GAME_START = MMI_MODULE_OD_GAME<< 16, 
	MMINV_OD_NV_APN_CONFIG,
#ifdef OD_DOODLE_JUMP_SUPPORT
       MMINV_OD_DOODLE_JUMP_NV_ID,
#endif
#ifdef OD_CROSSY_ROAD_SUPPORT
       MMINV_OD_CROSSY_ROAD_NV_ID,
#endif
#ifdef OD_STICK_HERO_SUPPORT
       MMINV_STICK_HERO_NV_ID,
#endif
#ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
       MMINV_BUBBLE_PARTY_MIX_NV_ID,
#endif
#ifdef OD_SUPER_PET_SUPPORT
       MMINV_SUPER_PET_NV_ID,
#endif
#ifdef OD_RUNNER_SUPPORT
       MMINV_RUNNER_NV_ID,
#endif
	MMINV_OD_GAME_NV_ITEM_NUM
}PHY_APP_NV_ITEM_E;

#define OD_GAME_MAX_LEVEL_COUNT      (3)

typedef struct
{
	OD_U8 init;
    OD_U8 level;
    OD_U8 level_2;	
    OD_U8 sound;
    OD_U32 scores_list[OD_GAME_MAX_LEVEL_COUNT];
#ifdef OD_GAME_BILLING_SUPPORT
    OD_U16 unlock_type;
    OD_U16 random_type;
    OD_U8 free_trials;
#endif    
}od_game_nv_t;
#endif  /* __OD_ADAPTER_NV_H__ */

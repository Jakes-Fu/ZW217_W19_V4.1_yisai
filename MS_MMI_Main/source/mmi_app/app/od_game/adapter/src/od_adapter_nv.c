#include "mmi_module.h"
#include "mn_type.h"
#include "mmi_nv.h"

#include "od_adapter_nv.h"
#include "od_adapter_log.h"
#include "od_adapter_gprs.h"

const OD_U16 od_game_nv_len[] =
{
      300,
	  sizeof(OD_ACCOUNT_DATA_STRUCT)*OD_APN_CONFIG_MAX,	
#ifdef OD_DOODLE_JUMP_SUPPORT
       sizeof(od_game_nv_t),
#endif
#ifdef OD_CROSSY_ROAD_SUPPORT
       sizeof(od_game_nv_t),
#endif
#ifdef OD_STICK_HERO_SUPPORT
       sizeof(od_game_nv_t),
#endif
#ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
       sizeof(od_game_nv_t),
#endif
#ifdef OD_SUPER_PET_SUPPORT
       sizeof(od_game_nv_t),
#endif
#ifdef OD_RUNNER_SUPPORT
       sizeof(od_game_nv_t),
#endif
};

void od_game_RegNv()
{
    MMI_RegModuleNv(MMI_MODULE_OD_GAME, od_game_nv_len, sizeof(od_game_nv_len)/sizeof(od_game_nv_len[0]));
}

#if 1//恢复出厂设置保留NV
static OD_U8 * s_od_game_buf=OD_NULL;

void od_game_load_nv()
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    OD_U16 cn = sizeof(od_game_nv_len)/sizeof(od_game_nv_len[0]);
    OD_U16 i;
    OD_U16 size=0;

    for(i=0;i<cn;i++)
    {
        size += od_game_nv_len[i];
    }
	
    if(s_od_game_buf != OD_NULL)
    {
        od_game_free(s_od_game_buf);
        s_od_game_buf = OD_NULL;
    }

    s_od_game_buf = (OD_U8 *)od_game_malloc(size);
    if(s_od_game_buf == OD_NULL)
    {        
        return ;
    }
    size = 0;
    for(i=0;i<cn;i++)
    {
        MMINV_READ(MMINV_OD_GAME_START+i, s_od_game_buf+size, return_value);
    	if (MN_RETURN_SUCCESS != return_value)
    	{		
    	}
        size += od_game_nv_len[i];
    }
}


void od_game_save_nv()
{
    OD_U16 cn = sizeof(od_game_nv_len)/sizeof(od_game_nv_len[0]);
    OD_U16 i;
    OD_U16 size=0;

        
    if(s_od_game_buf==OD_NULL) return;
    for(i=0;i<cn;i++)
    {
        MMINV_WRITE(MMINV_OD_GAME_START+i, s_od_game_buf+size);	
        size += od_game_nv_len[i];
    }
    
    od_game_free(s_od_game_buf);
    s_od_game_buf = OD_NULL;
}


OD_U32 od_game_get_nvram_id(OD_GAME_TYPE game_type)
{
    switch(game_type)
    {
#ifdef OD_DOODLE_JUMP_SUPPORT
        case OD_GAME_DOODLE_JUMP:
            return MMINV_OD_DOODLE_JUMP_NV_ID;
#endif      
#ifdef OD_CROSSY_ROAD_SUPPORT
        case OD_GAME_CROSSY_ROAD:
            return MMINV_OD_CROSSY_ROAD_NV_ID;
#endif
#ifdef OD_STICK_HERO_SUPPORT
        case OD_GAME_STICK_HERO:
            return MMINV_STICK_HERO_NV_ID;
#endif
#ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
        case OD_GAME_BUBBLE_PARTY_MIX:
            return MMINV_BUBBLE_PARTY_MIX_NV_ID;
#endif
#ifdef OD_SUPER_PET_SUPPORT
        case OD_GAME_SUPER_PET:
            return MMINV_SUPER_PET_NV_ID;
#endif
        default:
            return game_type;
    }
}
#endif

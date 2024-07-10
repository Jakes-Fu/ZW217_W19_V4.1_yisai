/******************************************************************************
 ** File Name:      dolphin_phy_cfg.c                                         *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Rigister definition for physical layer.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                   *
 *****************************************************************************/

#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "audio_drvapi.h"
#include "dolphin_phy_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
#define DOLPHIN_CFG_DBG       SCI_TRACE_LOW

#define DOL_CFG_ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct{
    uint32          mask;
    uint32          shift;
}DOL_BIT_MASK_T;


typedef struct{
    uint32          mode;
    uint32          pga_mode;
    DOL_BIT_MASK_T  bit_mask[4];
}DOL_PGA_SET_MAP_T;

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL DOL_PGA_FUN_T* s_p_pga_set_ops=0;
LOCAL DOL_PGA_SET_MAP_T s_pga_set_cfg[]=
{
{AUDIO_SPK             ,DOL_PGA_GOD                        ,{{0x0F,0},{0x00,0},{0x00,0},{0x00,0}}},
{AUDIO_HP              ,DOL_PGA_GOD|DOL_PGA_GO             ,{{0x0F,0},{0x1F,4},{0x00,0},{0x00,0}}},
{AUDIO_EAR             ,DOL_PGA_GOD|DOL_PGA_GOBTL          ,{{0x0F,0},{0x01,4},{0x00,0},{0x00,0}}},
{AUDIO_SPK|AUDIO_HP    ,DOL_PGA_GOD|DOL_PGA_GO             ,{{0x0F,0},{0x1F,4},{0x00,0},{0x00,0}}},

{AUDIO_LINEIN|AUDIO_SPK,DOL_PGA_GOB                        ,{{0x1F,0},{0x00,0},{0x00,0},{0x00,0}}},
{AUDIO_LINEIN|AUDIO_HP ,DOL_PGA_GOB|DOL_PGA_GO             ,{{0x1F,0},{0x1F,5},{0x00,0},{0x00,0}}},
{AUDIO_LINEIN|AUDIO_EAR,DOL_PGA_GOB|DOL_PGA_GOBTL          ,{{0x1F,0},{0x01,5},{0x00,0},{0x00,0}}},

{AUDIO_MIC             ,DOL_PGA_GI|DOL_PGA_GIM             ,{{0x0F,0},{0x01,4},{0x00,0},{0x00,0}}},
{AUDIO_LINEIN|AUDIO_MIC,DOL_PGA_GI|DOL_PGA_GIM|DOL_PGA_GOLR,{{0x0F,0},{0x01,4},{0x07,5},{0x00,0}}},
};

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC int32 DOLPHIN_PGA_Init_Set(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    s_p_pga_set_ops = DOL_Get_PGA_OPS(param);
    return ret;
}

LOCAL int32 _DOLPHIN_PGA_Fun_Set(AUDIO_CTL_PGA_T * pga,uint32 pga_mode)
{
    int32 ret = AUDIO_HAL_ERROR;
    DOL_PGA_FUN_T* p_pga_set = s_p_pga_set_ops;
    //DOLPHIN_CFG_DBG:"[AUDDRV][DOL]real set pga 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOLPHIN_PHY_CFG_78_112_2_17_23_2_45_145,(uint8*)"d",pga_mode);
    for(;0!=p_pga_set->mode;p_pga_set++)
    {
        if( (p_pga_set->mode) == pga_mode )
        {
            //DOLPHIN_CFG_DBG:"[AUDDRV][DOL]find pga 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOLPHIN_PHY_CFG_83_112_2_17_23_2_45_146,(uint8*)"d",p_pga_set->pFun);
            if( p_pga_set->pFun )
            {
                ret = p_pga_set->pFun(pga);
            }
        }
    }
    return ret;
}


LOCAL int32 _DOLPHIN_PGA_Parse_Set(AUDIO_CTL_PGA_T * pga,uint32 index)
{
    int32 ret = AUDIO_HAL_ERROR;
    uint32 mask_index = 0;
    uint32 i = 0;
    uint32 mask,shift;
    AUDIO_CTL_PGA_T pag_set ={0};
    pag_set.id = pga->id;
    pag_set.chan = pga->chan;
    pag_set.mode = pga->mode;
    //DOLPHIN_CFG_DBG:"[AUDDRV][DOL]all pga_mode 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOLPHIN_PHY_CFG_104_112_2_17_23_2_45_147,(uint8*)"d",(s_pga_set_cfg[index].pga_mode));
    for(i=0; i < DOL_PGA_MAX_BIT;i++ )
    {
        if( (1<<i) & (s_pga_set_cfg[index].pga_mode) )
        {
            //DOLPHIN_CFG_DBG:"[AUDDRV][DOL]set pga_mode 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOLPHIN_PHY_CFG_109_112_2_17_23_2_45_148,(uint8*)"d",(1<<i));
            mask = s_pga_set_cfg[index].bit_mask[mask_index].mask;
            shift = s_pga_set_cfg[index].bit_mask[mask_index].shift;
            mask_index++;
            pag_set.level = ((((pga->level)>>shift)&mask));
            ret = _DOLPHIN_PGA_Fun_Set(&pag_set,(1<<i));
        }
    }
    return ret;
}

LOCAL int32 _DOLPHIN_PGA_Setting(AUDIO_CTL_PGA_T * pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    uint32 i = 0;
    for(i=0;i<DOL_CFG_ARR_SIZE(s_pga_set_cfg);i++)
    {
        if( (pga->mode) == (s_pga_set_cfg[i].mode) )
        {
            ret = _DOLPHIN_PGA_Parse_Set(pga,i);
        }
    }
    return ret;
}


PUBLIC int32 DOLPHIN_PGA_Set(AUDIO_CTL_PGA_T * pga)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( 0==s_p_pga_set_ops )
    {
        DOLPHIN_PGA_Init_Set(0);
    }
    if( s_p_pga_set_ops )
    {
        if( AUDIO_HP_MIC & pga->mode )
        {
            pga->mode &= ~(AUDIO_HP_MIC);
            pga->mode |= AUDIO_MIC;
        }
        ret = _DOLPHIN_PGA_Setting(pga);
    }
    else
    {
        //DOLPHIN_CFG_DBG:"[AUDDRV][DOL]pga set"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOLPHIN_PHY_CFG_153_112_2_17_23_2_45_149,(uint8*)"");
        ret = AUDIO_HAL_ERROR;
    }
    
    return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif



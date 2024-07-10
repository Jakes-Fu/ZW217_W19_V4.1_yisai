/******************************************************************************
 ** File Name:      usc15241_phy_cfg.c                                         *
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
#include "usc15241_phy_cfg.h"

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
#define USC15241_CFG_DBG       SCI_TRACE_LOW

#define USC15241_CFG_ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct{
    uint32          mask;
    uint32          shift;
}USC15241_BIT_MASK_T;


typedef struct{
    uint32          mode;
    uint32          pga_mode;
    USC15241_BIT_MASK_T  bit_mask[4];
}USC15241_PGA_SET_MAP_T;

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL USC15241_PGA_FUN_T* s_p_pga_set_ops=0;
LOCAL USC15241_PGA_SET_MAP_T s_pga_set_cfg[]=
{
{AUDIO_SPK             ,USC15241_PGA_GOD                        ,{{0x0F,0},{0x00,0},{0x00,0},{0x00,0}}},
{AUDIO_HP              ,USC15241_PGA_GOD|USC15241_PGA_GO             ,{{0x0F,0},{0x1F,4},{0x00,0},{0x00,0}}},
{AUDIO_EAR             ,USC15241_PGA_GOD|USC15241_PGA_GOBTL          ,{{0x0F,0},{0x01,4},{0x00,0},{0x00,0}}},

{AUDIO_LINEIN|AUDIO_SPK,USC15241_PGA_GOB                        ,{{0x1F,0},{0x00,0},{0x00,0},{0x00,0}}},
{AUDIO_LINEIN|AUDIO_HP ,USC15241_PGA_GOB|USC15241_PGA_GO             ,{{0x1F,0},{0x1F,5},{0x00,0},{0x00,0}}},
{AUDIO_LINEIN|AUDIO_EAR,USC15241_PGA_GOB|USC15241_PGA_GOBTL          ,{{0x1F,0},{0x01,5},{0x00,0},{0x00,0}}},

{AUDIO_MIC             ,USC15241_PGA_GI|USC15241_PGA_GIM             ,{{0x0F,0},{0x01,4},{0x00,0},{0x00,0}}},
{AUDIO_LINEIN|AUDIO_MIC,USC15241_PGA_GI|USC15241_PGA_GIM|USC15241_PGA_GOLR,{{0x0F,0},{0x01,4},{0x07,5},{0x00,0}}},
};

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC int32 USC15241_PGA_Init_Set(uint32 param)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    s_p_pga_set_ops = USC15241_Get_PGA_OPS(param);
    return ret;
}

LOCAL int32 _USC15241_PGA_Fun_Set(AUDIO_CTL_PGA_T * pga,uint32 pga_mode)
{
    int32 ret = AUDIO_HAL_ERROR;
    USC15241_PGA_FUN_T* p_pga_set = s_p_pga_set_ops;
    //USC15241_CFG_DBG:"[AUDDRV][USC15241]real set pga 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_PHY_CFG_77_112_2_17_23_3_5_282,(uint8*)"d",pga_mode);
    for(;0!=p_pga_set->mode;p_pga_set++)
    {
        if( (p_pga_set->mode) == pga_mode )
        {
            //USC15241_CFG_DBG:"[AUDDRV][USC15241]find pga 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_PHY_CFG_82_112_2_17_23_3_5_283,(uint8*)"d",p_pga_set->pFun);
            if( p_pga_set->pFun )
            {
                ret = p_pga_set->pFun(pga);
            }
        }
    }
    return ret;
}


LOCAL int32 _USC15241_PGA_Parse_Set(AUDIO_CTL_PGA_T * pga,uint32 index)
{
    int32 ret = AUDIO_HAL_ERROR;
    uint32 mask_index = 0;
    uint32 i = 0;
    uint32 mask,shift;
    AUDIO_CTL_PGA_T pag_set ={0};
    pag_set.id = pga->id;
    pag_set.chan = pga->chan;
    pag_set.mode = pga->mode;
    //USC15241_CFG_DBG:"[AUDDRV][USC15241]all pga_mode 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_PHY_CFG_103_112_2_17_23_3_5_284,(uint8*)"d",(s_pga_set_cfg[index].pga_mode));
    for(i=0; i < USC15241_PGA_MAX_BIT;i++ )
    {
        if( (1<<i) & (s_pga_set_cfg[index].pga_mode) )
        {
            //USC15241_CFG_DBG:"[AUDDRV][USC15241]set pga_mode 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_PHY_CFG_108_112_2_17_23_3_5_285,(uint8*)"d",(1<<i));
            mask = s_pga_set_cfg[index].bit_mask[mask_index].mask;
            shift = s_pga_set_cfg[index].bit_mask[mask_index].shift;
            mask_index++;
            pag_set.level = ((((pga->level)>>shift)&mask));
            ret = _USC15241_PGA_Fun_Set(&pag_set,(1<<i));
        }
    }
    return ret;
}

LOCAL int32 _USC15241_PGA_Setting(AUDIO_CTL_PGA_T * pga)
{
    int32 ret = AUDIO_HAL_ERROR;
    uint32 i = 0;
    for(i=0;i<USC15241_CFG_ARR_SIZE(s_pga_set_cfg);i++)
    {
        if( (pga->mode) == (s_pga_set_cfg[i].mode) )
        {
            ret = _USC15241_PGA_Parse_Set(pga,i);
        }
    }
    return ret;
}


PUBLIC int32 USC15241_PGA_Set(AUDIO_CTL_PGA_T * pga)
{
    int32 ret = AUDIO_HAL_SUCCESS;
    if( 0==s_p_pga_set_ops )
    {
        USC15241_PGA_Init_Set(0);
    }
    if( s_p_pga_set_ops )
    {
        if( AUDIO_HP_MIC & pga->mode )
        {
            pga->mode &= ~(AUDIO_HP_MIC);
            pga->mode |= AUDIO_MIC;
        }
        ret = _USC15241_PGA_Setting(pga);
    }
    else
    {
        //USC15241_CFG_DBG:"[AUDDRV][USC15241]pga set"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_PHY_CFG_152_112_2_17_23_3_5_286,(uint8*)"");
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



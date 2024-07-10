#ifdef UI_P3D_SUPPORT   
/*****************************************************************************
** File Name:      mmi_winse_imp.c                                    *
** Author:          arvin.wu                                                *
** Date:           12/10/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  10/2011      Arvin.wu            Create                                  *
******************************************************************************/
#ifndef _MMI_WINSE_IMP_C_
#define _MMI_WINSE_IMP_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h" 
#include "mmi_winse.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 #ifndef ARRAY_SIZE
#define ARRAY_SIZE( a )     (sizeof(a)/sizeof (a)[0])
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                                                  */
/*---------------------------------------------------------------------------*/
                
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
extern const MMI_WIN_SE_IMPLEMENTOR  g_c_winse_fade_anim;
extern const MMI_WIN_SE_IMPLEMENTOR  g_c_winse_fade_inout;
extern const MMI_WIN_SE_IMPLEMENTOR  g_c_winse_fade_vert_InOut;
LOCAL const MMI_WIN_SE_IMPLEMENTOR *const s_const_se_implementor[] = 
{
    &g_c_winse_fade_anim,
    &g_c_winse_fade_inout,
    &g_c_winse_fade_vert_InOut
};

LOCAL uint16 s_winse_implementor_current_index = 0;
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:   获得当前特效实现者
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_SE_IMPLEMENTOR*MMIWINSEIMP_GetSeImplementor(void)
{
    if(s_winse_implementor_current_index < ARRAY_SIZE(s_const_se_implementor))
    {
        return (MMI_WIN_SE_IMPLEMENTOR*)s_const_se_implementor[s_winse_implementor_current_index];
    }

    return PNULL;
}

/*****************************************************************************/
//  Description:   预设当前类型的特效
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIWINSEIMP_SetSeImplementorType(uint32 win_se_type)
{
    uint32 i = 0;
    
    for(i=0;i<ARRAY_SIZE(s_const_se_implementor);i++)
    {
        if(s_const_se_implementor[i] &&  win_se_type == s_const_se_implementor[i]->win_se_type)
        {
            s_winse_implementor_current_index = (uint16)i;
            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description:   根据当前设置的特效风格，取得相应的特效
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ANIM_TYPE_E MMIWINSE_GetAnimTypeBySetingStyle(uint32 seting_style)
{

    uint32 i = 0;
    
    for(i=0;i<ARRAY_SIZE(s_const_se_implementor);i++)
    {
        if(s_const_se_implementor[i] &&  seting_style == s_const_se_implementor[i]->win_se_set_style)
        {
            return (MMI_WIN_ANIM_TYPE_E)s_const_se_implementor[i]->win_se_type;
        }
    }  

    return MMI_WIN_ANIM_NONE;
}

/*****************************************************************************/
//  Description:   根据当前设置的特效风格，取得相应的特效
//  Global resource dependence:
//  Author: Arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIWINSE_GetNameIdBySetingStyle(uint32 seting_style)
{

    uint32 i = 0;
    
    for(i=0;i<ARRAY_SIZE(s_const_se_implementor);i++)
    {
        if(s_const_se_implementor[i] &&  seting_style == s_const_se_implementor[i]->win_se_set_style)
        {
            return s_const_se_implementor[i]->winse_name_id;
        }
    }  

    return 0;
}
#endif /*_MMI_WINSE_IMP_C_*/
#endif

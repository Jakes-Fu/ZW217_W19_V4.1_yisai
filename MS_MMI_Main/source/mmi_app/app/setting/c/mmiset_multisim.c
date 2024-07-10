/*****************************************************************************
** File Name:      mmiset_multisim.c                                            *
** Author:                                                                     *
** Date:           03/06/2004                                                  *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe dual sim setting              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2007       bruce.chi          Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "guilabel.h"
#include "guilistbox.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmisms_export.h"
#include "mmicc_export.h"
#include "mmiussd_export.h"
#include "mmi_textfun.h"
//#include "mmisms_text.h"
#include "mmistk_export.h"
#include "mmiidle_export.h"
#include "mmi_appmsg.h"
#include "mmipb_export.h"
#include "mmi_appmsg.h"
#include "mmi_nv.h"
//#include "mmiset_call.h"
#include "mmiset_call_internal.h"
//#include "mmiset_display.h"
#include "mmiset_nv.h"
#include "mmiset_id.h"
//#include "mmiset_menutable.h"
#include "mmiset_text.h"
#include "mmiset_wintab.h"
#include "mmipub.h"
#include "mmi_anim.h"
//#include "mmiset_multisim.h"
#include "mmiset_multisim_internal.h"
#include "mmipb_export.h"
#ifdef MMI_MULTISIM_COLOR_SUPPORT
#include "mmiidle_statusbar.h"
#endif

#ifdef MMI_GPRS_DATA_SIM_SET
#include "mmiset_export.h"
#endif 

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISET_DUAL_SIM_NUM  MMI_DUAL_SYS_MAX
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifndef MMI_MULTI_SIM_SYS_SINGLE
LOCAL  MN_DUAL_SYS_E        s_dual_sys_for_cf = MN_DUAL_SYS_1;  
//记录sys类型，用来设置呼叫转移（call forward）
LOCAL  MMISET_DUALSYS_TYPE_E s_dual_sys_setting = MMISET_DAULSYS_MAX;
#endif
//设置一个非法的数据做初始值
#ifdef MMI_MULTISIM_COLOR_SUPPORT
const GUI_COLOR_T   default_sim_color_index[]={0,1,2,3};    //default value

LOCAL const uint16   sim_color_list[MAX_SIM_COLOR_NUM]={
    GUI_RGB2RGB565(240, 21, 0),    /*lint !e572*/
    GUI_RGB2RGB565(255, 0, 151),    
    GUI_RGB2RGB565(221, 107, 176),    
    GUI_RGB2RGB565(51, 153, 51),    
    GUI_RGB2RGB565(144, 64, 0),    /*lint !e572*/
    GUI_RGB2RGB565(162, 0, 255),    
    GUI_RGB2RGB565(0, 119, 244),    
    GUI_RGB2RGB565(1, 172, 170)    /*lint !e778*/
};

LOCAL const GUI_COLOR_T   sim_color_id_list[MAX_SIM_COLOR_NUM]={
	MMITHEME_COLOR_1,
    MMITHEME_COLOR_2,
    MMITHEME_COLOR_3,
    MMITHEME_COLOR_4,
    MMITHEME_COLOR_5,
    MMITHEME_COLOR_6,
    MMITHEME_COLOR_7,
    MMITHEME_COLOR_8
};
#endif

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : set default sim name
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDefaultSimName(BOOLEAN is_lang_change)
{
    uint32 i = 0;
    MMI_TEXT_ID_T sim_name_id[MMI_DUAL_SYS_MAX] = {
        TXT_SIM_SEL_SIM1,
        TXT_SIM_SEL_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        TXT_SIM_SEL_SIM3,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SIM_SEL_SIM4,
#endif
#endif
        };
    MMISET_SIM_NAME_T sim_name[MMI_DUAL_SYS_MAX] = {0};

    if (is_lang_change)
    {
        MN_RETURN_RESULT_E	 return_value = MN_RETURN_FAILURE;

        MMINV_READ(MMINV_SET_SIM_NAME, sim_name, return_value);
        
        if (MN_RETURN_SUCCESS != return_value)
        {
            is_lang_change = FALSE;
        }
    }

    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if (!is_lang_change || sim_name[i].is_default)
        {
            MMI_STRING_T sim_num_str = {0};

            MMI_GetLabelTextByLang(sim_name_id[i], &sim_num_str);

            SCI_MEMSET(&sim_name[i], 0x00, sizeof(MMISET_SIM_NAME_T));
            sim_name[i].wstr_len = MIN(sim_num_str.wstr_len, MMISET_MAX_SIM_NAME_LEN);

            sim_name[i].is_default = TRUE;

            MMI_WSTRNCPY(sim_name[i].wstr_arr,
                                    MMISET_MAX_SIM_NAME_LEN,
                                    sim_num_str.wstr_ptr,
                                    sim_name[i].wstr_len,
                                    sim_name[i].wstr_len);
        }
    }

    MMINV_WRITE(MMINV_SET_SIM_NAME, sim_name);

    return TRUE;
}

/*****************************************************************************/
// 	Description : reset dual sim factory setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDefaultDualSysSetting(void)
{
    // MMISET_SIM_NAME_T	sim_name = {0};
    MN_RETURN_RESULT_E	return_value = MN_RETURN_FAILURE;
    
    MMISET_MAJOR_SIM_SEL_TYPE_E func_array[MMISET_MAJOR_SIM_SEL_FUNC_MAX] = {0};
    int                         i = 0;
    
    BOOLEAN                     answer_func_array[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX] = {0};
//    MN_DUAL_SYS_E               e_dual_sys = 0;
//     uint8                       ready_times = 0;
    
    //sim name
    MMIAPISET_SetDefaultSimName(FALSE);
#ifdef MMI_MULTISIM_COLOR_SUPPORT    
    MMIAPISET_SetDefaultSimColor();
#endif    
    
    //major sim fuction
    for (i = 0; i < MMISET_MAJOR_SIM_SEL_FUNC_MAX; i++)
    {
        func_array[i] = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
    }
    
    func_array[MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL] = MMISET_MAJOR_SIM_SEL_SIM1;//special for MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL
    
    MMINV_WRITE(MMINV_SET_MAJOR_SIM_SEL_FUNC, func_array);
    
    // answer with original sim   
    for (i = 0; i < MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX; i++)
    {
        answer_func_array[i] = FALSE;
    }
    
    MMINV_WRITE(MMINV_SET_ANSWER_WITH_ORIGINAL_SIM_FUNC, answer_func_array);

#if 0    
    e_dual_sys = MMIAPISET_GetMultiSysSetting();
    if ((MMISET_DAULSYS_ALL_SIM_ON != e_dual_sys)
        && MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1)
        &&MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
    {
        MMIAPISET_SetDualSysSetting(MMISET_DAULSYS_ALL_SIM_ON);
        
        ready_times = MMIAPIPB_GetSimReadyTimes();
        if(0 == ready_times)
        {
            ready_times++;
        }
        MMIAPIPB_SetSimReadyTimes( ready_times);		
        
        //according to user input, change ps
        if ((e_dual_sys == MMISET_DAULSYS_ONLY_SIM1_ON) && MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2))
        {
            //记录用户自己设置的情况
            MMISET_SetUserDualSysSetting(MMISET_DAULSYS_ALL_SIM_ON);
            MNPHONE_StartupPsEx(MN_DUAL_SYS_2);
        }            
        
        if ((e_dual_sys == MMISET_DAULSYS_ONLY_SIM2_ON) && MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
        {   
            //记录用户自己设置的情况
            MMISET_SetUserDualSysSetting(MMISET_DAULSYS_ALL_SIM_ON);
            MNPHONE_StartupPsEx(MN_DUAL_SYS_1);
        }
        
        //release pb sort result

        //MMIAPIPB_SetInitStatusAfterDualsimSettingChange(e_dual_sys, MMISET_DAULSYS_ALL_SIM_ON);
        
        //release sms sort result
        MMIAPISMS_InitSIMSmsList();
        //MMIAPISMS_SetInitStatusAfterDualsimSettingChange(e_dual_sys, MMISET_DAULSYS_ALL_SIM_ON);
    }  
    else if ((MMISET_DAULSYS_ALL_SIM_ON != e_dual_sys)
        &&(MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1) || MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_2)))
    {
        //有一张卡不存在,只修改user setting
        MMISET_SetUserDualSysSetting(MMISET_DAULSYS_ALL_SIM_ON);
    }
#endif

    return return_value;
}

/*****************************************************************************/
// 	Description : get sim name
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMISET_SIM_NAME_T MMIAPISET_GetSimName(MN_DUAL_SYS_E dual_sys)
{
    MMISET_SIM_NAME_T sim_name[MMI_DUAL_SYS_MAX] = {0};
    uint32 return_value = MN_RETURN_SUCCESS;

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPISET_GetSimName error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_260_112_2_18_2_51_26_245,(uint8*)"d", dual_sys);
        return sim_name[MN_DUAL_SYS_1];
    }

    MMINV_READ(MMINV_SET_SIM_NAME, sim_name, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPISET_SetDefaultSimName(FALSE);

        MMINV_READ(MMINV_SET_SIM_NAME, sim_name, return_value);
    }

    return sim_name[dual_sys];
}

/*****************************************************************************/
// 	Description : set sim name
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSimName(MN_DUAL_SYS_E dual_sys, MMISET_SIM_NAME_T * sim_name_ptr)
{
    uint32 return_value = MN_RETURN_SUCCESS;
    MMISET_SIM_NAME_T sim_name[MMI_DUAL_SYS_MAX] = {0};
    BOOLEAN is_default = FALSE;
    MMI_TEXT_ID_T sim_name_id[MMI_DUAL_SYS_MAX] = {
        TXT_SIM_SEL_SIM1,
        TXT_SIM_SEL_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)        
        TXT_SIM_SEL_SIM3,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SIM_SEL_SIM4,
#endif
#endif
        };
    MMI_STRING_T sim_num_str = {0};

    if (PNULL == sim_name_ptr
        || sim_name_ptr->wstr_len > MMISET_MAX_SIM_NAME_LEN
        || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    MMINV_READ(MMINV_SET_SIM_NAME, sim_name, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPISET_SetDefaultSimName(FALSE);

        MMINV_READ(MMINV_SET_SIM_NAME, sim_name, return_value);
    }

    MMI_GetLabelTextByLang(sim_name_id[dual_sys], &sim_num_str);

    if (sim_num_str.wstr_len == sim_name_ptr->wstr_len
        && 0 == MMIAPICOM_Wstrncmp(sim_num_str.wstr_ptr, sim_name_ptr->wstr_arr, sim_num_str.wstr_len))
    {
        is_default = TRUE;
    }
    else
    {
        is_default = FALSE;
    }

    SCI_MEMSET(&sim_name[dual_sys], 0x00, sizeof(MMISET_SIM_NAME_T));
    SCI_MEMCPY(&sim_name[dual_sys], sim_name_ptr, sizeof(MMISET_SIM_NAME_T));
    sim_name[dual_sys].is_default = is_default;

    MMINV_WRITE(MMINV_SET_SIM_NAME, sim_name);

    return TRUE;
}

/*****************************************************************************/
//  Description : get sim icon 
//  Global resource dependence : 
//  Author: Sam.hua
//  Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPISET_GetSimIcon(MN_DUAL_SYS_E dual_sys)
{
    MMI_IMAGE_ID_T  sim_icon[MMI_DUAL_SYS_MAX] = {
        IMAGE_SIM1,
        IMAGE_SIM2,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_SIM3,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        IMAGE_SIM4,
#endif
#endif
    };


    if (dual_sys>=MMI_DUAL_SYS_MAX)
    {
        return IMAGE_NULL;
    }

    return sim_icon[dual_sys];
}


#ifdef MMI_MULTISIM_COLOR_SUPPORT

/*****************************************************************************/
// 	Description : get sim color ID
//	Global resource dependence : 
//  Author: Sam.hua
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetSimColorID(MN_DUAL_SYS_E dual_sys)
{
    MMISET_SIM_COLOR_T sim_color[MMI_DUAL_SYS_MAX] = {0};
    uint32 return_value = MN_RETURN_SUCCESS;


    MMINV_READ(MMINV_SET_SIM_COLOR, sim_color, return_value);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPISET_GetSimColorID error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_377_112_2_18_2_51_26_246,(uint8*)"d", dual_sys);
        return sim_color[MN_DUAL_SYS_1].bg_color;
    }

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPISET_SetDefaultSimColor();
        MMINV_READ(MMINV_SET_SIM_COLOR, sim_color, return_value);
    }

    return sim_color_id_list[sim_color[dual_sys].index];
}

/*****************************************************************************/
// 	Description : get sim color
//	Global resource dependence : 
//  Author: Sam.hua
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISET_GetSimColor(MN_DUAL_SYS_E dual_sys)
{
    MMISET_SIM_COLOR_T sim_color[MMI_DUAL_SYS_MAX] = {0};
    uint32 return_value = MN_RETURN_SUCCESS;


    MMINV_READ(MMINV_SET_SIM_COLOR, sim_color, return_value);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPISET_GetSimColor error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_404_112_2_18_2_51_26_247,(uint8*)"d", dual_sys);
        return sim_color[MN_DUAL_SYS_1].bg_color;
    }

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPISET_SetDefaultSimColor();
        MMINV_READ(MMINV_SET_SIM_COLOR, sim_color, return_value);
    }

    return sim_color[dual_sys].bg_color;
}

/*****************************************************************************/
// 	Description : set default sim color
//	Global resource dependence : 
//  Author: Sam.hua
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_SetDefaultSimColor(void)
{
    MN_DUAL_SYS_E   dual_sys = 0;
    MMISET_SIM_COLOR_T sim_color[MMI_DUAL_SYS_MAX] = {0};

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
    {
        sim_color[dual_sys].index = default_sim_color_index[dual_sys];
        sim_color[dual_sys].bg_color = sim_color_list[sim_color[dual_sys].index];
    }

    MMINV_WRITE(MMINV_SET_SIM_COLOR, sim_color);

    return TRUE;
}
/*****************************************************************************/
// 	Description : set sim color by index
//	Global resource dependence : 
//  Author: Sam.hua
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSimColorByIndex(MN_DUAL_SYS_E dual_sys, uint16 index)
{
    uint32 return_value = MN_RETURN_SUCCESS;
    MMISET_SIM_COLOR_T sim_color[MMI_DUAL_SYS_MAX] = {0};

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    if (index >=MAX_SIM_COLOR_NUM)
    {
        return FALSE;
    }

    MMINV_READ(MMINV_SET_SIM_COLOR, sim_color, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPISET_SetDefaultSimColor();
        MMINV_READ(MMINV_SET_SIM_COLOR, sim_color, return_value);
    }
    sim_color[dual_sys].index=index;
    sim_color[dual_sys].bg_color= sim_color_list[index];
    //Update StatusBar settings
    //MMIMAIN_UpdateStatusBarBySimColor(dual_sys,sim_color_list[index]);
    //Update ListSettings
    MMINV_WRITE(MMINV_SET_SIM_COLOR, sim_color);

    return TRUE;
}

/*****************************************************************************/
// 	Description : get sim Color by index
//	Global resource dependence : 
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_GetSimColorByIndex(uint16 index)
{
    if (index >=MAX_SIM_COLOR_NUM)
    {
        return sim_color_list[0];      
    }
    return sim_color_list[index];
}



/*****************************************************************************/
// 	Description : get index from sim Color 
//	Global resource dependence : 
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_GetSimColorIndex(MN_DUAL_SYS_E dual_sys)
{
    MMISET_SIM_COLOR_T sim_color[MMI_DUAL_SYS_MAX] = {0};
    uint32 return_value = MN_RETURN_SUCCESS;


    MMINV_READ(MMINV_SET_SIM_COLOR, sim_color, return_value);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPISET_GetSimColor error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_500_112_2_18_2_51_27_248,(uint8*)"d", dual_sys);
        return sim_color[MN_DUAL_SYS_1].index;
    }

    if (MN_RETURN_SUCCESS != return_value)
    {
        MMIAPISET_SetDefaultSimColor();
        MMINV_READ(MMINV_SET_SIM_COLOR, sim_color, return_value);
    }

    return sim_color[dual_sys].index;
}



#endif

/*****************************************************************************/
// 	Description :获得主sim的nv设置
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMISET_MAJOR_SIM_SEL_TYPE_E MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type)
{
#if 0
    uint32 return_value = 0;
    MMISET_MAJOR_SIM_SEL_TYPE_E func_array[MMISET_MAJOR_SIM_SEL_FUNC_MAX] = {0};
    int i = 0;
    
    SCI_ASSERT(e_func_type < MMISET_MAJOR_SIM_SEL_FUNC_MAX);/* assert to do */
    
    MMINV_READ(MMINV_SET_MAJOR_SIM_SEL_FUNC, func_array, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        for (i = 0; i < MMISET_MAJOR_SIM_SEL_FUNC_MAX; i++)
        {
            func_array[i] = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
        }
        
        //special for MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL
        func_array[MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL] = MMISET_MAJOR_SIM_SEL_SIM1;
        
        MMINV_WRITE(MMINV_SET_MAJOR_SIM_SEL_FUNC, func_array);
    }
    
    return func_array[e_func_type];
#endif
    return MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
}

/*****************************************************************************/
// 	Description :设置主sim的nv设置
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type, MMISET_MAJOR_SIM_SEL_TYPE_E e_sim_sel_type)
{
    uint32 return_value = 0;
    MMISET_MAJOR_SIM_SEL_TYPE_E func_array[MMISET_MAJOR_SIM_SEL_FUNC_MAX] = {0};
    int i = 0;
    
    if(e_func_type >= MMISET_MAJOR_SIM_SEL_FUNC_MAX ||e_sim_sel_type >= MMISET_MAJOR_SIM_SEL_TYPE_MAX)
    {
        //SCI_TRACE_LOW:"MMISET_SetMajorSimSelType  MMISET_SetMajorSimSelType =%d, e_sim_sel_type= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_561_112_2_18_2_51_27_249,(uint8*)"dd",e_func_type,e_sim_sel_type);
        return FALSE;
    }
//   SCI_ASSERT((e_func_type < MMISET_MAJOR_SIM_SEL_FUNC_MAX) && (e_sim_sel_type < MMISET_MAJOR_SIM_SEL_TYPE_MAX));/* assert to do */
    
    MMINV_READ(MMINV_SET_MAJOR_SIM_SEL_FUNC, func_array, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        for (i = 0; i < MMISET_MAJOR_SIM_SEL_FUNC_MAX; i++)
        {
            func_array[i] = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
        }
        
        //special for MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL
        func_array[MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL] = MMISET_MAJOR_SIM_SEL_SIM1;
    }
    
    func_array[e_func_type] = e_sim_sel_type;
    
    MMINV_WRITE(MMINV_SET_MAJOR_SIM_SEL_FUNC, func_array);

    return TRUE;   
}

/*****************************************************************************/
// 	Description :设置本卡回复的nv
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetAnswerWithOriginalSimType(BOOLEAN is_answer_with_original_sim[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX])
{
    uint32 return_value = MN_RETURN_SUCCESS;
    uint32	need_write_to_nv = FALSE;
    BOOLEAN func_array[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX] = {0};
    int i = 0;
    
    MMINV_READ(MMINV_SET_ANSWER_WITH_ORIGINAL_SIM_FUNC, func_array, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        need_write_to_nv = TRUE;
        return_value = FALSE;
        SCI_MEMSET(func_array,FALSE,sizeof(func_array));
    }
    else
    {
        for (i = 0; i < MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX; i++)
        {
            if(func_array[i] != is_answer_with_original_sim[i])
            {
                return_value = TRUE;
                need_write_to_nv = TRUE;
                SCI_MEMCPY(func_array, is_answer_with_original_sim, sizeof(func_array));
                break;
            }
        }
    }
    if(need_write_to_nv)
    {
        MMINV_WRITE(MMINV_SET_ANSWER_WITH_ORIGINAL_SIM_FUNC, func_array);
    }
    return return_value;   
}
#endif

/*****************************************************************************/
// 	Description :获取当前SIM卡的第一个的本机号码?
//	Global resource dependence : 
//  Author: sam.hua
//	Note: 
/*****************************************************************************/
LOCAL void GetSIMDisplayNumberByIndex(MN_DUAL_SYS_E dual_sys,MMI_STRING_T   *in_str,uint16 max_len)
{
    MMIPB_BCD_NUMBER_T sim_number={0};
    uint8 sim_num[MMIPB_BCD_NUMBER_MAX_LEN*2+2]={0};

    MMIAPIPB_GetMSISDNumberArrayBySim(&(sim_number),dual_sys,0);    //Get first one

    if(sim_number.number_len >0)
    {
        MMIAPICOM_GenDispNumber(MMIPB_GetNumberTypeFromUint8(sim_number.npi_ton),
                sim_number.number_len, sim_number.number, sim_num, max_len);
        MMI_STRNTOWSTR(in_str->wstr_ptr, max_len,sim_num,MMIPB_BCD_NUMBER_MAX_LEN*2+2, MIN(MMIPB_BCD_NUMBER_MAX_LEN*2+2,max_len));
        in_str->wstr_len= MMIAPICOM_Wstrlen(in_str->wstr_ptr);
    }
    return;
}

/*****************************************************************************/
// 	Description :获取 网络名+卡名
//	Global resource dependence : 
//  Author: sam.hua
//	Note:
/*****************************************************************************/
LOCAL void GetOpnAndSimNameDisplay(MN_DUAL_SYS_E dual_sys,MMI_STRING_T   *in_str,    
                        MMISET_APPEND_SIM_TYPE_E         append_type,uint16 max_len)
{
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN+1] = {0};
    MMI_STRING_T network_str = {0};
    MMISET_SIM_NAME_T   sim_str     = {0};
    MMI_STRING_T   sim_strid     = {0};
    MMI_STRING_T   split_strid     = {0};
    wchar          split_str[] = {(wchar)' ',0};
    MMI_STRING_T   temp_strid     = {0};
    wchar temp_str[GUILIST_STRING_MAX_NUM+1] = {0};
    
    if (max_len>MMIPHONE_MAX_OPER_NAME_LEN+1)
    {
        max_len=MMIPHONE_MAX_OPER_NAME_LEN+1;
    }

    SCI_MEMSET(&sim_str,0x00,sizeof(MMISET_SIM_NAME_T));
    sim_str = MMIAPISET_GetSimName(dual_sys);
    
    //get opn_spn information for display in sim selection page!!
    SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
    network_str.wstr_ptr = (wchar *)opn_spn_name;
    SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));

    if (MMISET_APPEND_STK_CARD == append_type)
    {
        BOOLEAN is_exist_stk_name = FALSE;

        is_exist_stk_name = MMIAPISTK_GetStkName((MN_DUAL_SYS_E)dual_sys, &network_str);

        if (!is_exist_stk_name)
        {
            MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)dual_sys, &network_str);
        }
    }
    else
    {
        MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)dual_sys, &network_str);
    }
    //Add split char
    split_strid.wstr_ptr = split_str;
    split_strid.wstr_len = (uint16)MMIAPICOM_Wstrlen(split_str);
    temp_strid.wstr_ptr = temp_str;
    MMIAPICOM_MergeTwoString(&network_str, &split_strid, &temp_strid, GUILIST_STRING_MAX_NUM+1);
    
    //
    sim_strid.wstr_ptr = sim_str.wstr_arr;
    sim_strid.wstr_len = sim_str.wstr_len;
    MMIAPICOM_MergeTwoString(&temp_strid, &sim_strid, in_str, GUILIST_STRING_MAX_NUM+1);
    return;
}

/*****************************************************************************/
// Description : to append sys ok item to listbox by sim name
// Global resource dependence : 
// Author:xiaoqing.lu
// Note: 凡是调用到这个函数的位置，不能引用list index判断哪张sim卡
//  必须通过GUILIST_GetItemData 取得 item_t.user_data的数据作为sim卡
//  判断的信息--- By Michael for Quad Sim New Featrue!!!
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendMultiSimName(
                                        MMI_CTRL_ID_T ctrl_id,
                                        MMI_TEXT_ID_T left_softkey_id,
                                        uint32 max_num,
                                        MMISET_APPEND_SIM_TYPE_E append_type
                                        )
{
    MMIAPISET_AppendMultiSimNameExt(ctrl_id, 
								left_softkey_id, 
#ifdef MMI_GUI_STYLE_MINISCREEN
								IMAGE_NULL,
#else
								TXT_NULL, 
#endif								
								STXT_RETURN, 
								max_num, 
								append_type);
}

/*****************************************************************************/
// 	Description : to append sys ok item to listbox by sim name
//	Global resource dependence : 
//  Author:xiaoqing.lu
// Note: 凡是调用到这个函数的位置，不能引用list index 判断哪张sim card,
//  必须通过GUILIST_GetItemData 取得" item_t.user_data" 的数据
//  作为是哪张sim card 的信息--- By Michael for Quad Sim New Featrue!!!
/*****************************************************************************/
PUBLIC void MMIAPISET_AppendMultiSimNameExt(
    MMI_CTRL_ID_T ctrl_id,
    MMI_TEXT_ID_T left_softkey_id,
    MMI_TEXT_ID_T middle_softkey_id,
    MMI_TEXT_ID_T right_softkey_id,
    uint32 max_num,
    MMISET_APPEND_SIM_TYPE_E append_type
   )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    uint32 i = 0;
#ifndef  MMI_MULTISIM_COLOR_SUPPORT
    MMISET_SIM_NAME_T   sim_str     = {0};
    wchar opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN+1] = {0};
    MMI_STRING_T network_str = {0};
#endif
    MMI_IMAGE_ID_T sim_img_id[MMI_DUAL_SYS_MAX] ={0};
    MMI_IMAGE_ID_T no_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    MMI_IMAGE_ID_T gray_sim_img_id[MMI_DUAL_SYS_MAX] = {0};
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
    MMI_STRING_T    simnum_strid={0};
    wchar wsimnum_str[MMIPB_BCD_NUMBER_MAX_LEN*2+2]={0};

    MMI_STRING_T    list_strid={0};     //Opn+SimName
    wchar wlist_str[GUILIST_STRING_MAX_NUM+1]={0};
#endif

#ifdef MMI_GPRS_DATA_SIM_SET
    MMISET_GPRS_MODE_TYPE_E       mode_type = DATA_CARD_MANUAL_SELECT;
#endif

    for(i = MN_DUAL_SYS_1; i<MMI_DUAL_SYS_MAX; i++)
    {
        sim_img_id[i] = IMAGE_IDLE_PROMPT_SIM1 + (i - MN_DUAL_SYS_1);    //shoule IMAGE_IDLE_PROMPT_NO_SIM2 == IMAGE_IDLE_PROMPT_NO_SIM1+1.atc
        no_sim_img_id[i] = IMAGE_IDLE_PROMPT_NO_SIM1 + (i - MN_DUAL_SYS_1);
        gray_sim_img_id[i] = IMAGE_IDLE_PROMPT_GRAY_SIM1 + (i - MN_DUAL_SYS_1);
    }
    
    if (max_num < MMISET_DUAL_SIM_NUM)
    {
        return;
    }
    
    for (i = 0; i < MMISET_DUAL_SIM_NUM; i++)
    {
        BOOLEAN is_append = FALSE;

        switch(append_type)
        {
        case MMISET_APPEND_SYS_OK:
            is_append = MMIAPIPHONE_IsSimAvailable(i);
            break;

        case MMISET_APPEND_SIM_OK:
            is_append = MMIAPIPHONE_IsSimCardOk(i);
            break;

        case MMISET_APPEND_PS_READY:
            is_append = MMIAPIPHONE_GetIsPsReady(i);
            break;

        case MMISET_APPEND_PIN1_PERMIT:
            is_append = MMIAPIPHONE_IsPermitSetPin1(i);
            break;

        case MMISET_APPEND_STK_CARD:
            is_append = MMIAPISTK_IsStkCard(i);
            break;

        case MMISET_APPEND_SIM_ALL:
            is_append = TRUE;
            break;

#ifdef MMI_GPRS_DATA_SIM_SET
        case MMISET_APPEND_GPRS_MODE:
            mode_type = MMIAPISET_GetGPRSModeType();
            if (1 < MMIAPIPHONE_GetSimExistedNum(PNULL, 0) 
                && DATA_CARD_MANUAL_SELECT != mode_type)
            {
                is_append = mode_type == (DATA_CARD_MODE_1 + (i - MN_DUAL_SYS_1));
            }
            else
            {
                is_append = MMIAPIPHONE_IsSimCardOk(i);
            }
            break;
#endif 

        default:
            is_append = FALSE;
            break;
        }

        if (is_append)
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            MMI_IMAGE_ID_T image_id = IMAGE_NULL;
            
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
            item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2;
#else
            item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
#endif            
            item_t.item_state  |= GUIITEM_STATE_SELFADAPT_RECT;
            item_t.item_data_ptr = &item_data;
            item_t.user_data = i;//indicate which sim is appended !!

            if (e_dualsys_setting & (0x01 << i))
            {
                if (MMIAPIPHONE_GetSimExistedStatus(i))
                {
#ifdef MMI_MULTISIM_COLOR_SUPPORT    
                    image_id =IMAGE_LIST_SELECT_SIM_BG;    //use common sim bg here
#else
                    image_id = sim_img_id[i];
#endif                                    
                }
                else
                {
#ifdef MMI_MULTISIM_COLOR_SUPPORT    
                    image_id =IMAGE_LIST_SELECT_SIM_BG;    //use common sim bg here
#else                
                    image_id = no_sim_img_id[i];
#endif                    
                }
            }
            else
            {
                if (MMIAPIPHONE_GetSimExistedStatus(i))
                {
#ifdef MMI_MULTISIM_COLOR_SUPPORT    
                    image_id =IMAGE_LIST_SELECT_SIM_BG;    //use common sim bg here
#else                
                    image_id = gray_sim_img_id[i];
#endif                    
                }
                else
                {
#ifdef MMI_MULTISIM_COLOR_SUPPORT    
                    image_id =IMAGE_LIST_SELECT_SIM_BG;    //use common sim bg here
#else                
                    image_id = no_sim_img_id[i];
#endif                    
                }
            }

            item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
            item_data.item_content[0].item_data.image_id = image_id;
#ifdef MMI_MULTISIM_COLOR_SUPPORT    
            item_data.item_content[0].bg_color_id=MMIAPISET_GetSimColorID(i);
            item_data.item_content[0].is_default =TRUE;
#endif
#ifdef  MMI_MULTISIM_COLOR_SUPPORT
            list_strid.wstr_ptr = wlist_str;
            SCI_MEMSET(list_strid.wstr_ptr, 0x00, sizeof(wlist_str));
            GetOpnAndSimNameDisplay(i,&list_strid,append_type,GUILIST_STRING_MAX_NUM);


            item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_len = list_strid.wstr_len;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = list_strid.wstr_ptr;


            simnum_strid.wstr_ptr = wsimnum_str;
            SCI_MEMSET(simnum_strid.wstr_ptr, 0x00, sizeof(wsimnum_str));
            GetSIMDisplayNumberByIndex(i,&simnum_strid,MMIPB_BCD_NUMBER_MAX_LEN*2+2);
    
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[2].item_data.text_buffer.wstr_len = simnum_strid.wstr_len;
            item_data.item_content[2].item_data.text_buffer.wstr_ptr = simnum_strid.wstr_ptr;
#else
            sim_str = MMIAPISET_GetSimName(i);

            //display SIM Card Display string!!
            item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
            
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_arr;

            //get opn_spn information for display in sim selection page!!
            SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
            network_str.wstr_ptr = (wchar *)opn_spn_name;
            SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
            
            if (MMISET_APPEND_STK_CARD == append_type)
            {
                BOOLEAN is_exist_stk_name = FALSE;

                is_exist_stk_name = MMIAPISTK_GetStkName((MN_DUAL_SYS_E)i, &network_str);

                if (!is_exist_stk_name)
                {
                    MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)i, &network_str);
                }
            }
            else
            {
                MMIAPISET_GetSimServiceString((MN_DUAL_SYS_E)i, &network_str);
            }
    
            item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[2].item_data.text_buffer.wstr_len = network_str.wstr_len;
            item_data.item_content[2].item_data.text_buffer.wstr_ptr = network_str.wstr_ptr;
#endif
            //set softkey
            item_data.softkey_id[0] = left_softkey_id;
            item_data.softkey_id[1] = middle_softkey_id;
            item_data.softkey_id[2] = right_softkey_id;
            
            GUILIST_AppendItem(ctrl_id, &item_t);      
        }
    }

    GUILIST_SetOwnSofterKey(ctrl_id, TRUE);
#else
    {
        MMI_STRING_T sim_str = {0};
        GUILIST_ITEM_T      item_t = {0};
        GUILIST_ITEM_DATA_T item_data = {0};

        SCI_MEMSET(&item_t, 0x00, sizeof(item_t));
        SCI_MEMSET(&item_data, 0x00, sizeof(item_data));

        item_t.item_style = GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2;
        item_t.item_data_ptr = &item_data;
        item_t.user_data = MN_DUAL_SYS_1;

        if (append_type != MMISET_APPEND_SYS_OK)    //Only support for SYS_OK
        {
            return;
        }
        
        MMI_GetLabelTextByLang(TXT_GPRS,&sim_str);

        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = IMAGE_IDLE_PROMPT_SIM;
        
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_len = sim_str.wstr_len;
        
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = sim_str.wstr_ptr;

        //set softkey
        item_data.softkey_id[0] = TXT_COMMON_OK;
        item_data.softkey_id[1] = TXT_NULL;
        item_data.softkey_id[2] = STXT_RETURN;
        
        GUILIST_AppendItem(ctrl_id, &item_t);
    }
#endif  //!defined(MMI_MULTI_SIM_SYS_SINGLE)
}
/*****************************************************************************/
// 	Description :获得本卡回复的nv设置
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_TYPE_E e_func_type)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    uint32 return_value = 0;
    BOOLEAN func_array[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX] = {0};
    int i = 0;
    
    MMINV_READ(MMINV_SET_ANSWER_WITH_ORIGINAL_SIM_FUNC, func_array, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        for (i = 0; i < MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX; i++)
        {
#ifdef MMI_GPRS_DATA_SIM_SET
            if (MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS == i
                || MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MMS == i)
            {
                func_array[i] = TRUE;
            }
            else
            {
                func_array[i] = FALSE;
            }
#else
            func_array[i] = FALSE;
#endif
        }
        
        MMINV_WRITE(MMINV_SET_ANSWER_WITH_ORIGINAL_SIM_FUNC, func_array);
    }
    
    if(e_func_type >= MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX )
    {
        //SCI_TRACE_LOW:"MMIAPISET_GetAnswerWithOriginalSimType   e_func_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_970_112_2_18_2_51_28_250,(uint8*)"d",e_func_type);
        return FALSE;
    }    
    //SCI_ASSERT(e_func_type < MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX);/* assert to do */
    return (BOOLEAN)(func_array[e_func_type]);
#else
    return TRUE;
#endif
}

/*****************************************************************************/
// 	Description : 获得mmi 用户设置多sim卡系统设置情况，适用于开机判断情况。
//	Global resource dependence : 
//  Author: bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMultiSysSetting(MMISET_DUALSYS_TYPE_E e_dual_sys)
{
    //SCI_ASSERT(e_dual_sys < MMISET_DAULSYS_MAX);/* assert to do */
    if(e_dual_sys >= MMISET_DAULSYS_MAX )
    {
        //SCI_TRACE_LOW:"MMISET_SetMultiSysSetting   e_dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_989_112_2_18_2_51_28_251,(uint8*)"d",e_dual_sys);
        return ;
    }    
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    s_dual_sys_setting = e_dual_sys;
#endif
    
    MMINV_WRITE(MMINV_USER_SET_SIM_ONOFF_TYPE, &e_dual_sys);
}

/*****************************************************************************/
// 	Description : 获得mmi 双sim卡系统设置情况
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMISET_DUALSYS_TYPE_E MMIAPISET_GetMultiSysSetting(void)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)

    MMISET_DUALSYS_TYPE_E   e_dual_sys = 0;
    uint32                  return_value = 0;

    if(MMISET_DAULSYS_MAX == s_dual_sys_setting)
    {
        MMINV_READ(MMINV_USER_SET_SIM_ONOFF_TYPE, &e_dual_sys, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            e_dual_sys = MMISET_DAULSYS_ALL_SIM_ON;
            MMINV_WRITE(MMINV_USER_SET_SIM_ONOFF_TYPE, &e_dual_sys);
        }

        s_dual_sys_setting = e_dual_sys;
    }
    else
    {
        e_dual_sys = s_dual_sys_setting;
    }
    
    //SCI_TRACE_LOW:"MMIAPISET_GetMultiSysSetting e_dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_1027_112_2_18_2_51_28_252,(uint8*)"d", e_dual_sys);

    return e_dual_sys;

#else

    return MMISET_DAULSYS_ONLY_SIM1_ON;

#endif
}

/*****************************************************************************/
// 	Description : get mmi multi sim
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPISET_GetMultiSysSettingNum(uint16 *idx_ptr, uint16 array_len)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
    uint32	num = 0;
    uint32 i = 0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if ((e_dualsys_setting & (1 << i)) && MMIAPIPHONE_GetSimExistedStatus(i))
        {
            if ((idx_ptr != PNULL) && (num < array_len))
            {
                idx_ptr[num] = i;
            }
            num++;
        }
    }

    return num;
}

/*****************************************************************************/
// 	Description : 获得当前主sim卡的id 
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPISET_GetActiveSim(void)
{


#ifdef MMI_MULTI_SIM_SYS_SINGLE
    return MN_DUAL_SYS_1;
#else
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MN_DUAL_SYS_E e_dual_sys = MN_DUAL_SYS_1;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (sim_num > 0)
    {
        e_dual_sys = (MN_DUAL_SYS_E)sim_sys;
    }
    else
    {
        MMIAPIPHONE_GetPSReadyNum(&sim_sys, 1);

        e_dual_sys = (MN_DUAL_SYS_E)sim_sys;
    }

    return e_dual_sys;
#endif
}

/*****************************************************************************/
// 	Description : 获得当前主sim卡的选项，包括ps是否启动
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMISET_MAJOR_SIM_SEL_TYPE_E MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type)
{
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    return MMISET_MAJOR_SIM_SEL_SIM1;
#else
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MMISET_MAJOR_SIM_SEL_TYPE_E e_major_sim_type = 0;
    
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);

    if (sim_num == 0)
    {
        e_major_sim_type = MMISET_MAJOR_SIM_SEL_SIM1;
    }    
    else if (sim_num == 1)
    {
        e_major_sim_type = MMISET_MAJOR_SIM_SEL_SIM1 +( sim_sys - MN_DUAL_SYS_1);
    }
    else
    {
        e_major_sim_type = MMISET_GetMajorSimSelType(e_func_type);
        if ((e_major_sim_type != MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
            && !MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)((int32)e_major_sim_type - MMISET_MAJOR_SIM_SEL_SIM1)))
        {
            if (e_func_type == MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL)
            {
                e_major_sim_type++;
                if (e_major_sim_type == MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                {
                    e_major_sim_type = MMISET_MAJOR_SIM_SEL_SIM1;
                }
            }
            else
            {
                e_major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
            }
            MMISET_SetMajorSimSelType(e_func_type, e_major_sim_type);
        }
    }
    
    //SCI_TRACE_LOW:"mmiset:MMIAPISET_GetMajorSim e_major_sim_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_1139_112_2_18_2_51_28_253,(uint8*)"d", e_major_sim_type);

    return e_major_sim_type;
#endif
}

/*****************************************************************************/
// 	Description : 获得当前主sim卡的选项，不包括ps
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMISET_MAJOR_SIM_SEL_TYPE_E MMIAPISET_GetMajorSimCard(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type)
{
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    return MMISET_MAJOR_SIM_SEL_SIM1;
#else
    uint16 sim_ok = MN_DUAL_SYS_1;
    uint32 sim_num = 0;
    MMISET_MAJOR_SIM_SEL_TYPE_E e_major_sim_type = 0;
    
    sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

    if (sim_num == 0)
    {
        e_major_sim_type = MMISET_MAJOR_SIM_SEL_SIM1;
    }    
    else if (sim_num == 1)
    {
        e_major_sim_type = MMISET_MAJOR_SIM_SEL_SIM1 +( sim_ok - MN_DUAL_SYS_1);
    }
    else
    {
        e_major_sim_type = MMISET_GetMajorSimSelType(e_func_type);
        if ((e_major_sim_type != MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
            && !MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)((int32)e_major_sim_type - MMISET_MAJOR_SIM_SEL_SIM1)))
        {
            if (e_func_type == MMISET_MAJOR_SIM_SEL_FUNC_SPEED_DIAL)
            {
                e_major_sim_type++;
                if (e_major_sim_type == MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                {
                    e_major_sim_type = MMISET_MAJOR_SIM_SEL_SIM1;
                }
            }
            else
            {
                e_major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
            }
            MMISET_SetMajorSimSelType(e_func_type, e_major_sim_type);
        }
    }
    
    //SCI_TRACE_LOW:"mmiset:MMIAPISET_GetMajorSim e_major_sim_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_MULTISIM_1190_112_2_18_2_51_28_254,(uint8*)"d", e_major_sim_type);
    return e_major_sim_type;
#endif
}

/*****************************************************************************/
// 	Description : 设置呼叫转移的dual sys 类型
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void   MMIAPISET_SetSysForCf(MN_DUAL_SYS_E    dual_sys)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    s_dual_sys_for_cf = dual_sys;
#endif
}

/*****************************************************************************/
// 	Description : 获得呼叫转移的dual sys 类型
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E   MMIAPISET_GetSysForCf(void)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    return s_dual_sys_for_cf;
#else
    return 0;/*lint !e64*/
#endif
}

/*****************************************************************************/
// 	Description : get plmn service status string to display select sim
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_GetSimServiceString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *network_str_ptr)
{
    MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();

    if (dual_sys >= MMI_DUAL_SYS_MAX || PNULL == network_str_ptr)
    {
        return;
    }

    if (MMIAPISET_GetFlyMode())
    {
        MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, network_str_ptr);
    }
    else if (e_dualsys_setting & (1 << dual_sys))
    {
        MMIAPIPHONE_GetSimAndServiceString(dual_sys, network_str_ptr);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SIM_DEACTIVATED, network_str_ptr);
    }
}


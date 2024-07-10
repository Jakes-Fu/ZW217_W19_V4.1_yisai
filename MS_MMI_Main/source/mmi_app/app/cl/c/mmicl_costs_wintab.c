/*****************************************************************************
** File Name:      mmicl_costs_wintab.c                                      *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call costs                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       Jassmine.Meng       Create
******************************************************************************/

#define _MMICL_COSTS_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cl_trc.h"
#ifdef AOC_SUPPORT
#include "window_parse.h"
#include "mmi_menutable.h"
#include "mmicl_export.h"
#include "mmi_nv.h"
#include "guilistbox.h"
#include "mmipb_export.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiphone_export.h"
#include "guimsgbox.h"
#include "mmisms_export.h"
#include "mmicl_costs_wintab.h"
#include "mmi_textfun.h"
#include "mmisms_text.h"
#include "mmipub.h"
#include "mmicc_id.h"
#include "mmicc_menutable.h"
#include "mmicc_text.h"
#include "mmi_anim.h"
#include "mmk_app.h"
//#include "guires.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "guirichtext.h"
#include "guiform.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define INTEGR_PART_LEN                           16          //�������ֳ���
#define FRACTIONAL_PART_LEN                        2          //С�����ֳ���
#define CURRENCY_LEN                               3          //���ҵ�λ����
#define MMICL_COSTS_MAX_INPUT_LEN                  8          //ͨ�����޳���
#define MMICL_COSTS_RATE_INPUT_LEN                 8          //ͨ�����ʳ���

#define CL_FOCUS_LINE_COLOR                         0X72BC

LOCAL SIM_PUCT_INFO_T                   s_sim_puct_info[MMI_DUAL_SYS_MAX] = {0};		//PUCT�Ʒѽṹʵ��
LOCAL BOOLEAN                           s_is_get_puct[MMI_DUAL_SYS_MAX] = {0};   //�Ʒ���Ϣ�Ƿ��ñ�־λ

#ifndef MMI_MULTI_SIM_SYS_SINGLE
LOCAL MN_DUAL_SYS_E                     s_sim_select = MN_DUAL_SYS_1;
LOCAL uint32                                  s_menu_select = 0;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/******************************************************************************************************************/
// 	 
//	����ͨ�����õĹ��ô��ڴ�����
//  
/******************************************************************************************************************/
LOCAL MMI_RESULT_E HandleCallCostsWindow(
                                         MMI_WIN_ID_T		win_id, 	// STARTUP NORMAL���ڵ�ID
                                         MMI_MESSAGE_ID_E 	msg_id, 	// ���ڵ��ڲ���ϢID
                                         DPARAM 		    param		// ��Ӧ��Ϣ�Ĳ���
                                         );


/****************************************************************************************************************/
// 	 
//	�޸�ͨ�����޴��ڴ�����
//  
/****************************************************************************************************************/
LOCAL MMI_RESULT_E HandleCallCostsMaxUpdateWindow(
                                                  MMI_WIN_ID_T		win_id, 	// STARTUP NORMAL���ڵ�ID
                                                  MMI_MESSAGE_ID_E 	msg_id, 	// ���ڵ��ڲ���ϢID
                                                  DPARAM 		    param		// ��Ӧ��Ϣ�Ĳ���
                                                  );


/****************************************************************************************************************/
// 	 
//	�޸�ͨ�����ʴ��ڴ�����
//  
/****************************************************************************************************************/
LOCAL MMI_RESULT_E HandleCallCostsRateUpateWindow(
                                                  MMI_WIN_ID_T		win_id, 	// STARTUP NORMAL���ڵ�ID
                                                  MMI_MESSAGE_ID_E 	msg_id, 	// ���ڵ��ڲ���ϢID
                                                  DPARAM 		    param		// ��Ӧ��Ϣ�Ĳ���
                                                  );

/****************************************************************************************************************/
//
//  ͨ���Ʒ������㴰�ڴ�����
//
/****************************************************************************************************************/
LOCAL MMI_RESULT_E  HandleCallCostsResetWindow(
                                               MMI_WIN_ID_T		win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM			param
                                               );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E MMICL_GetSimSelect(void);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : to handle sim select window
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  CostSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_SetSimSelect(MN_DUAL_SYS_E sim_select);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_SetMenuSelect(uint32 menu_select);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL uint32 MMICL_GetMenuSelect(void);
#endif

/******************************************************************************************************************/
//
//                        ͨ���Ʒ�ģ�鴰�ڶ���
//
/******************************************************************************************************************/
//��ʾ����ͨ�����õĹ��ô���
WINDOW_TABLE( MMICL_CALL_COSTS_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleCallCostsWindow ),    
    WIN_ID( MMICL_CALL_COSTS_WIN_ID ),
    WIN_TITLE(TXT_NULL),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMICL_CALL_COSTS_CTRL_ID),
    
    END_WIN
};


//�޸�ͨ�����޷��õĴ���
WINDOW_TABLE( MMICL_CALL_COSTS_MAX_UPDATE_WIN_TAB ) = 
{
    WIN_TITLE( TXT_CL_SET_COST_MAX ),
    WIN_FUNC( (uint32)HandleCallCostsMaxUpdateWindow ),    
    WIN_ID( MMICL_CALL_COSTS_MAX_UPDATE_WIN_ID ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(MMICL_COSTS_MAX_INPUT_LEN,MMICL_COSTS_MAX_EDIT_CTRL_ID),
    END_WIN
};

//�޸�ͨ�����ʵĴ���
WINDOW_TABLE( MMICL_CALL_COSTS_RATE_UPDATE_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_CL_SET_COST_RATE ),
    WIN_FUNC( (uint32)HandleCallCostsRateUpateWindow ),    
    WIN_ID( MMICL_CALL_COSTS_RATE_UPDATE_WIN_ID ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMICL_CALL_COSTS_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMICL_COSTS_CURRENCY_FORM_CTRL_ID,MMICL_CALL_COSTS_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMICL_COSTS_CURRENCY_LABEL_CTRL_ID,MMICL_COSTS_CURRENCY_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE,CURRENCY_LEN,MMICL_COSTS_CURRENCY_VAL_LABEL_CTRL_ID,MMICL_COSTS_CURRENCY_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMICL_COSTS_RATE_FORM_CTRL_ID,MMICL_CALL_COSTS_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMICL_COSTS_RATE_LABEL_CTRL_ID,MMICL_COSTS_RATE_FORM_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE,MMICL_COSTS_RATE_INPUT_LEN,MMICL_COSTS_RATE_VAL_LABEL_CTRL_ID,MMICL_COSTS_RATE_FORM_CTRL_ID),

    END_WIN
};

//�Ʒ������㴰��
WINDOW_TABLE( MMICL_CALL_COSTS_RESET_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_TITLE( TXT_CL_COST_RESET ),
    WIN_FUNC( (uint32)HandleCallCostsResetWindow ),    
    WIN_ID( MMICL_CALL_COSTS_RESET_WIN_ID ),
    WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
   
    END_WIN
};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*		//Sam.hua	use SelectSimFunc  
// for dual_sim
WINDOW_TABLE( MMICL_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleCostSelectSimWinMsg ),    
    WIN_ID( MMICL_COST_SIM_WIN_ID ),
    WIN_TITLE( TXT_SIM_SEL ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMICL_COST_SIM_LISTBOX_CTRL_ID ),
    END_WIN
};
*/
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
//----------------------------------------------------------------------------------------------------------------
//  ��������        DecodeCurrencyChar
//  ��������:       �����ҵ�λ�ַ���ѹΪ��׼�ַ�
//  ȫ�ֱ���ʹ��:	��
//  ����ֵ:         ��׼�ַ�
//----------------------------------------------------------------------------------------------------------------

LOCAL uint8 DecodeCurrencyChar(uint8 coded_char)
{
    if ((coded_char<=0x7f) && (coded_char!=0x0A) && (coded_char!=0x0D))
    {
        return coded_char;
    }
    else if(coded_char == 0xFF)              
    {
        return '\0';
    }
    else
    {
        return ' ';
    }
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         SavePuctInfo
//  ��������:        ����PUCT�źŽṹ
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL void SavePuctInfo(
                        APP_MN_PUCT_INFO_CNF_T* puct_info_ptr          //PUCT�źŽṹָ��  (�������)
                        )
{
    if(PNULL == puct_info_ptr)
    {
        //SCI_TRACE_LOW:"SavePuctInfo call_info_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_255_112_2_18_2_11_21_25,(uint8*)"");
        return;
    }
    s_sim_puct_info[puct_info_ptr->dual_sys].is_ok = puct_info_ptr->is_ok;
    s_sim_puct_info[puct_info_ptr->dual_sys].is_service_support = puct_info_ptr->is_service_support;
    s_sim_puct_info[puct_info_ptr->dual_sys].currency_code[0] = DecodeCurrencyChar(puct_info_ptr->puct_info.currency_code[0]); 
    s_sim_puct_info[puct_info_ptr->dual_sys].currency_code[1] = DecodeCurrencyChar(puct_info_ptr->puct_info.currency_code[1]); 
    s_sim_puct_info[puct_info_ptr->dual_sys].currency_code[2] = DecodeCurrencyChar(puct_info_ptr->puct_info.currency_code[2]); 
    s_sim_puct_info[puct_info_ptr->dual_sys].currency_code[CURRENCY_LEN] = 0;
    s_sim_puct_info[puct_info_ptr->dual_sys].ex_sign = puct_info_ptr->puct_info.ex_sign;
    s_sim_puct_info[puct_info_ptr->dual_sys].ex = puct_info_ptr->puct_info.ex;
    s_sim_puct_info[puct_info_ptr->dual_sys].eppu = puct_info_ptr->puct_info.eppu;
}

#ifdef MMI_MULTI_SIM_SYS_SINGLE
//----------------------------------------------------------------------------------------------------------------
//  ��������         IsAocSupported
//  ��������:        �ж�SIM���Ƿ�֧��AOC����
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN IsAocSupported(MN_DUAL_SYS_E dual_sys)
{
    return s_sim_puct_info[dual_sys].is_service_support;
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         IsGetPuctInfoOK
//  ��������:        �ж�SIM���Ƿ�֧��AOC����
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL BOOLEAN IsGetPuctInfoOK(MN_DUAL_SYS_E dual_sys)
{
    return s_sim_puct_info[dual_sys].is_ok;
}
#endif //#ifdef MMI_MULTI_SIM_SYS_SINGLE

//----------------------------------------------------------------------------------------------------------------
//  ��������         PuctDecodeToChargeStr
//  ��������:        ��PUCT���ýṹ���ձ�׼����Ϊ��Ӧ���ַ���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
void PuctDecodeToChargeStr( uint32  charge,                         //���û���
                           int8    exponent,                       //����ָ��
                           uint8*  currency,                       //���û��ҵ�λ
                           uint8*  out_str,                        //�����ַ���
                           BOOLEAN is_with_currency                //�Ƿ���ϻ��ҵ�λ��׺
                           ) 
{
    uint8  zero_str[]="0000000";
    uint8  temp[INTEGR_PART_LEN + 1] = {0};
    int16  len = 0, integral_len = 0, fractional_start_pos = 0;
    uint8  integral_str[INTEGR_PART_LEN + 1] = {0};
    uint8  fractional_str[FRACTIONAL_PART_LEN + 1] = {0};
    uint8  charge_str[FRACTIONAL_PART_LEN + INTEGR_PART_LEN + 1] = {0};
    uint16 i = 0;
    uint16 j = 0;
    
    if(PNULL == currency ||PNULL == out_str)
    {
        //SCI_TRACE_LOW:"PuctDecodeToChargeStr currency OR out_str is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_317_112_2_18_2_11_21_26,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(integral_str,0,(INTEGR_PART_LEN+1));
    SCI_MEMSET(fractional_str,0,(FRACTIONAL_PART_LEN+1));
    SCI_MEMSET(charge_str,0,(FRACTIONAL_PART_LEN+INTEGR_PART_LEN+1));
    
    if (exponent>=0)
    {
        sprintf((char*)integral_str,"%lu",charge);
        //����ָ��С��7�� ������������Ҫ��0
        len = strlen((char*)integral_str);
        if(exponent <= 7 && charge > 0)  
        {
            SCI_MEMCPY(integral_str+len,zero_str,exponent);
        }
        if(len + exponent > INTEGR_PART_LEN)
        {
            //SCI_TRACE_LOW:"PuctDecodeToChargeStr error len = %d, exponent = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_336_112_2_18_2_11_22_27,(uint8*)"dd", len, exponent);
            return;
        }
        integral_str[len + exponent] = 0;
        
        //���ǻ���Ϊ����ʾС�������λ������ֻ��ʾ��������
        if(charge == 0)
        {
            sprintf((char*)charge_str, "%s.%s", integral_str, "00");
            if (strlen((char *)charge_str) >= sizeof(charge_str))
            {
                //SCI_TRACE_LOW:"PuctDecodeToChargeStr error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_347_112_2_18_2_11_22_28,(uint8*)"");
                return;
            }
        }
        else
        {
            sprintf((char*)charge_str,"%s",integral_str);
            if (strlen((char *)charge_str) >= sizeof(charge_str))
            {
                //SCI_TRACE_LOW:"PuctDecodeToChargeStr error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_356_112_2_18_2_11_22_29,(uint8*)"");
                return;
            }
        }
    }
    else
    {
        sprintf((char*)temp, "%lu", charge);
        sprintf((char*)fractional_str, "%s", "00");
        sprintf((char*)integral_str, "%s", "0");
        
        len = strlen((char*)temp);
        if (len != 0)
        {
            //������������ֳ���
            integral_len = len + exponent;
            //�ó����������ַ���
            for(i = 0; i < integral_len; ++i)   
            {
                integral_str[i] = temp[i];
            }
            //�����С������ʼλ��
            if(integral_len<0)
            {
                fractional_start_pos -= integral_len;
            }
            //����С������ʼλ�����ó�С�������ַ���
            for(j = fractional_start_pos; (j < FRACTIONAL_PART_LEN) && (i < len); ++j,++i)
            {
                fractional_str[j] = temp[i];
            }
        }
        
        sprintf((char*)charge_str,"%s.%s",integral_str,fractional_str);
        if (strlen((char *)charge_str) >= sizeof(charge_str))
        {
            //SCI_TRACE_LOW:"PuctDecodeToChargeStr error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_392_112_2_18_2_11_22_30,(uint8*)"");
            return;
        }
    }
    //�����Ƿ�����ҵ�λ��׺�����ɷ����ַ���
    if(is_with_currency)
    {
        sprintf((char*)out_str,"%s %s",charge_str,currency);
    }
    else
    {
        sprintf((char*)out_str,"%s",charge_str);
    }
    
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         ConvertStrToPuct
//  ��������:        ���ַ���תΪPUCT��ʽ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          �ɹ�����TRUE ������ FALSE
//----------------------------------------------------------------------------------------------------------------
BOOLEAN ConvertStrToPuct(
                         MMI_STRING_T	        *str_info,       //����ķ����ַ���
                         uint32					*eppu,           //�����EPPUֵ
                         int8                   *ex              //�����ָ��ֵ
                         )
{
    uint8   eppu_str[ FRACTIONAL_PART_LEN + INTEGR_PART_LEN + 1] = {0};
    wchar   str[FRACTIONAL_PART_LEN + INTEGR_PART_LEN + 1] = {0};
    int32  n = 0, eppu_value = 0;
    uint32 str_len = 0;

    BOOLEAN bDot = FALSE;
    
    wchar*   p1=str;
    uint8*   p2=eppu_str;
    
    if(PNULL == str_info ||PNULL == eppu ||PNULL == ex)
    {
        //SCI_TRACE_LOW:"PuctDecodeToChargeStr str_info OR eppu OR ex is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_432_112_2_18_2_11_22_31,(uint8*)"");
        return FALSE;
    }
    
    SCI_MEMSET(eppu_str, 0, sizeof(eppu_str));

    str_len = MIN(FRACTIONAL_PART_LEN+INTEGR_PART_LEN, str_info->wstr_len);

    MMI_WSTRNCPY( str,FRACTIONAL_PART_LEN+INTEGR_PART_LEN,
        str_info->wstr_ptr,str_info->wstr_len,
        str_len
        );
    
    str[str_len]=0;
    
    while(*p1!='\0')                        //����Ƿ��г����ֺ�.������ַ�
    {
        if(!((*p1 >= 0x30 && *p1 <= 0x39) || *p1=='.')) 
        {
            return FALSE;
        }
        p1++;
    }
    
    p1=str;
    
    while(*p1!='\0')
    {
        if(!bDot) 
        {
            if(*p1!='.') 
            {
                *p2++ = *p1++;
            }
            else 
            {
                bDot=TRUE;
                p1++;
            }          //������С����������
        }
        else
        {
            if(n>1) 
            {
                break;                  //ֻȡС�������λ
            }
            *p2++ = *p1++;
            n++;
        }
    }
    
    *p2=0;                                  //��ӽ�����
    eppu_value = atoi((char*)eppu_str);     //��EPPU�ַ��������Ӧ��EPPUֵ
    
    if(n > 0)                                 //��ΪС�����λ��exȡ��
    {
        *ex = -n;
    }
    else
    {
        *ex = 0;                              
    }
    
    while(eppu_value > 4095)                  //��EPPUֵ>4095��Ҫ����EPPU��exʹ
        //EPPUֵ<=4095
    {
        eppu_value = eppu_value/10;
        (*ex)++;
    }
    
    *eppu=eppu_value;
    return TRUE;
}


//----------------------------------------------------------------------------------------------------------------
//  ��������         GetUpdateAcmMax
//  ��������:        ��ȡҪ���µ� ACM MAX ֵ
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ����ACM MAX��ֵ
//----------------------------------------------------------------------------------------------------------------
uint32 GetUpdateAcmMax(MMI_STRING_T * costs_max_str,            //�����ͨ������ֵ�ַ���
                       uint8   ex,                              //�����ָ��
                       BOOLEAN sign,                            //�����ָ���ķ���
                       uint32  eppu                             //�����eppuֵ
                       )
{
    uint32 acm_max = 0, i = 0;
    uint8  temp[MMICL_COSTS_MAX_INPUT_LEN + 1] = {0};
    uint16 len = costs_max_str->wstr_len;
   
     MMI_WSTRNTOSTR( (uint8 *)temp, MMICL_COSTS_MAX_INPUT_LEN + 1,
         costs_max_str->wstr_ptr,len,
         len
         );

//     MMI_MEMCPY(temp,MMICL_COSTS_MAX_INPUT_LEN + 1,
//         costs_max_str->wstr_ptr,len,
//         len
//         );
    
    if(len >= MMICL_COSTS_MAX_INPUT_LEN + 1)
    {
        //SCI_TRACE_LOW:"GetUpdateAcmMax error len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_535_112_2_18_2_11_22_32,(uint8*)"d", len);
        return 0;
    }
    
    temp[len] = 0;
    
    acm_max = atol( (char*)temp );
    
    if (eppu == 0)
    {
        return 0;
    }
    
    if(sign)                         //��ָ��
    {
        //acm_max = acm_max/eppu;
        for(i = 0; i < ex; i++)      
        {
            acm_max=acm_max/10;
        }
    }
    else                             //��ָ��
    {
        for(i=0;i<ex;i++)
        {
            acm_max=acm_max*10;
        }
        //acm_max = acm_max/eppu;
    }

    return acm_max;
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         DisplayClCostsTotal
//  ��������:        ��ʾ��ͨ������
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL void DisplayClCostsTotal(MMI_CTRL_ID_T ctrl_id)
{
    MNSIM_AOC_T        aoc = {0};
    int8               exponent = 0;
    uint32             charge = 0;
    MMI_STRING_T       title  = {0};
    uint8              charge_str[INTEGR_PART_LEN + FRACTIONAL_PART_LEN + CURRENCY_LEN + 1] = {0};
    wchar              charge_wstr[INTEGR_PART_LEN + FRACTIONAL_PART_LEN + CURRENCY_LEN + 1] = {0};
    uint8               i = 0;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_1;
    MMI_STRING_T        lable_str = {0};
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    uint16 index = 0;
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    wchar defaul_text[] = {'0','.','0','0'};

#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    MMI_TEXT_ID_T  label_string[] = {
        TXT_CL_SIM1_COST_ALL,
        TXT_CL_SIM2_COST_ALL,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_CL_SIM3_COST_ALL,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_CL_SIM4_COST_ALL,
#endif
#endif
    };
#endif

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    if(0 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return ;
    }

    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)i))
        {
            continue;
        }

        dual_sys = (MN_DUAL_SYS_E)i;
                
        aoc = MNSIM_GetAocEx(dual_sys);
        
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_COST_ALL, &title);
#else
        MMI_GetLabelTextByLang(label_string[i], &title); 
#endif

        item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &title, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = item_text;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_COST_ALL, &lable_str);
        GUIWIN_SetTitleText(MMICL_CALL_COSTS_WIN_ID, lable_str.wstr_ptr, lable_str.wstr_len, FALSE);

        GUIWIN_SetSoftkeyTextId(MMICL_CALL_COSTS_WIN_ID,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);

        if(s_sim_puct_info[dual_sys].is_ok && s_sim_puct_info[dual_sys].is_service_support)
        {
            if(s_sim_puct_info[dual_sys].ex_sign)   
            {
                exponent = s_sim_puct_info[dual_sys].ex;    
            }
            else   
            {
                exponent =- s_sim_puct_info[dual_sys].ex;
            }
            
            //charge = s_sim_puct_info[dual_sys].eppu * aoc.acm_unit;
            charge = aoc.acm_unit;
            //��PUCT�ṹ��ѹΪ�����ַ���    
            PuctDecodeToChargeStr(charge,exponent,                      
                s_sim_puct_info[dual_sys].currency_code,
                charge_str,
                TRUE
                );

            title.wstr_ptr = charge_wstr;
            title.wstr_len = strlen((char*)charge_str);
            MMI_STRNTOWSTR( charge_wstr, INTEGR_PART_LEN + FRACTIONAL_PART_LEN + CURRENCY_LEN, 
                                        (uint8 *)charge_str, title.wstr_len, title.wstr_len );
        }
        else
        {
            title.wstr_ptr = defaul_text;
            title.wstr_len = sizeof(defaul_text)/sizeof(wchar);
        }        


        item_data.text_data.buf.len = MIN(title.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = title.wstr_ptr;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    }
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         DisplayClCostsMax
//  ��������:        ��ʾͨ������
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL void DisplayClCostsMax(MMI_CTRL_ID_T ctrl_id)
{
    MNSIM_AOC_T        aoc = {0};
    int8               exponent = 0;
    uint32             charge=0;
    MMI_STRING_T       title = {0};
    MMI_STRING_T       lable_str = {0};
    uint8              charge_str[INTEGR_PART_LEN+FRACTIONAL_PART_LEN+CURRENCY_LEN+1] = {0};
    wchar              charge_wstr[INTEGR_PART_LEN+FRACTIONAL_PART_LEN+CURRENCY_LEN+1] = {0};
    uint8               i = 0;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_1;
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    uint16 index = 0;
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    wchar defaul_text[] = {'0','.','0','0'};

#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    MMI_TEXT_ID_T  label_string[] = {
        TXT_CL_SIM1_COST_MAX,
        TXT_CL_SIM2_COST_MAX,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_CL_SIM3_COST_MAX,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_CL_SIM4_COST_MAX,
#endif
#endif
    };
#endif

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    if(0 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return ;
    }
    
    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)i))
        {
            continue;
        }

        dual_sys = (MN_DUAL_SYS_E)i;
        
        aoc = MNSIM_GetAocEx(dual_sys);

#ifdef MMI_MULTI_SIM_SYS_SINGLE
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_COST_MAX, &title);
#else
        MMI_GetLabelTextByLang(label_string[i], &title); 
#endif
        
        item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &title, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = item_text;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_COST_MAX, &lable_str);
        
        GUIWIN_SetTitleText(MMICL_CALL_COSTS_WIN_ID, lable_str.wstr_ptr, lable_str.wstr_len, FALSE);

        GUIWIN_SetSoftkeyTextId(MMICL_CALL_COSTS_WIN_ID,  TXT_EDIT, TXT_NULL, STXT_RETURN, FALSE);

        if(s_sim_puct_info[dual_sys].is_ok && s_sim_puct_info[dual_sys].is_service_support)
        {
            if(s_sim_puct_info[dual_sys].ex_sign)   
            {
                exponent = s_sim_puct_info[dual_sys].ex;    
            }
            else   
            {
                exponent =- s_sim_puct_info[dual_sys].ex;
            }
            
            //charge = s_sim_puct_info[dual_sys].eppu * aoc.acm_max;
            charge = aoc.acm_max;
            
            //��PUCT�ṹ��ѹΪ�����ַ���    
            PuctDecodeToChargeStr(charge,exponent,                      
                s_sim_puct_info[dual_sys].currency_code,
                charge_str,
                TRUE);

            title.wstr_ptr = charge_wstr;
            title.wstr_len = strlen((char*)charge_str);
            MMI_STRNTOWSTR( charge_wstr, INTEGR_PART_LEN+FRACTIONAL_PART_LEN+CURRENCY_LEN, 
                (uint8 *)charge_str, title.wstr_len, title.wstr_len );
        }
        else
        {
            title.wstr_ptr = defaul_text;
            title.wstr_len = sizeof(defaul_text)/sizeof(wchar);
        }
        

        item_data.text_data.buf.len = MIN(title.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = title.wstr_ptr;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        
    }
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         DisplayClCostsRate
//  ��������:        ��ʾͨ������
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL void DisplayClCostsRate(MMI_CTRL_ID_T ctrl_id)
{
    int8               exponent = 0;
    uint32             charge=0;
    MMI_STRING_T       title = {0};
    uint8              charge_str[INTEGR_PART_LEN+FRACTIONAL_PART_LEN+CURRENCY_LEN+1] = {0};
    wchar              charge_wstr[INTEGR_PART_LEN+FRACTIONAL_PART_LEN+CURRENCY_LEN+1] = {0};
    uint8               i = 0;
    MN_DUAL_SYS_E       dual_sys = MN_DUAL_SYS_1;
    MMI_STRING_T lable_str = {0};
    GUIRICHTEXT_ITEM_T item_data = {0};/*lint !e64*/
    uint16 index = 0;
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};
    wchar defaul_text[] = {'0','.','0','0'};

#ifndef MMI_MULTI_SIM_SYS_SINGLE 
    MMI_TEXT_ID_T  label_string[] = {
        TXT_CL_SIM1_COST_RATE,
        TXT_CL_SIM2_COST_RATE,
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_CL_SIM3_COST_RATE,
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_CL_SIM4_COST_RATE,
#endif
#endif
    };
#endif

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

    if(0 == MMIAPIPHONE_GetSimCardOkNum(PNULL, 0))
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return ;
    }
    
    for(i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)i))
        {
            continue;
        }
        dual_sys = (MN_DUAL_SYS_E)i;

#ifdef MMI_MULTI_SIM_SYS_SINGLE
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_COST_RATE, &title);
#else
        MMI_GetLabelTextByLang(label_string[i], &title); 
#endif
        
        item_data.text_data.buf.len = MMIAPICOM_CopyStr2Buf(item_text, &title, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = item_text;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_COST_RATE, &lable_str);
        
        GUIWIN_SetTitleText(MMICL_CALL_COSTS_WIN_ID, lable_str.wstr_ptr, lable_str.wstr_len,  FALSE);

        GUIWIN_SetSoftkeyTextId(MMICL_CALL_COSTS_WIN_ID,  TXT_EDIT, TXT_NULL, STXT_RETURN, FALSE);

        if(s_sim_puct_info[dual_sys].is_ok && s_sim_puct_info[dual_sys].is_service_support)
        {
            if(s_sim_puct_info[dual_sys].ex_sign)   
            {
                exponent = s_sim_puct_info[dual_sys].ex;    
            }
            else   
            {
                exponent =- s_sim_puct_info[dual_sys].ex;
            }
            
            charge = s_sim_puct_info[dual_sys].eppu;
            
            //��PUCT�ṹ��ѹΪ�����ַ���    
            PuctDecodeToChargeStr(charge,
                exponent,                      
                s_sim_puct_info[dual_sys].currency_code,
                charge_str,
                TRUE);
            
            title.wstr_ptr = charge_wstr;
            title.wstr_len = strlen((char*)charge_str);
            MMI_STRNTOWSTR( charge_wstr, INTEGR_PART_LEN+FRACTIONAL_PART_LEN+CURRENCY_LEN, 
                (uint8 *)charge_str, title.wstr_len, title.wstr_len );
        }
        else
        {
            title.wstr_ptr = defaul_text;
            title.wstr_len = sizeof(defaul_text)/sizeof(wchar);
        }
        
        item_data.text_data.buf.len = MIN(title.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
        item_data.text_data.buf.str_ptr = title.wstr_ptr;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        
    }
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         HandlePuctInfoCnf
//  ��������:        ��ȡPUCT�źŴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
void HandlePuctInfoCnf(DPARAM  param)
{
    uint32 group_id = 0, menu_id = 0;
    APP_MN_PUCT_INFO_CNF_T puct_info_cnf = *((APP_MN_PUCT_INFO_CNF_T *)param);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 i = 0;
#endif
    
    if(!s_is_get_puct[puct_info_cnf.dual_sys]) 
    {
        return;
    }
    
    SavePuctInfo(&puct_info_cnf);
    
    s_is_get_puct[puct_info_cnf.dual_sys] = FALSE;
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    for(i=0; i<MMI_DUAL_SYS_MAX; i++)
    {
        if(s_is_get_puct[i])
        {
            return;
        }
    }
#else
    //����SIM����֧��ͨ���Ʒѹ���
    if(!IsAocSupported(puct_info_cnf.dual_sys))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CL_NOT_PRESENT);
        return;
    }
    //���ǻ�ȡPUCT����
    if(!IsGetPuctInfoOK(puct_info_cnf.dual_sys))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CL_NOT_PRESENT);
        return;
    }
#endif
    //���cl�˵������Ѿ��رգ�
    if (!MMK_IsOpenWin(MMICL_MAIN_MENU_WIN_ID))
    {
        return;
    }
    
    MMIPUB_CloseWaitWin(MAIN_WAIT_WIN_ID);
    
    MMK_CreateWin((uint32 *)MMICL_CALL_COSTS_WIN_TAB,PNULL);
    GUIMENU_GetId(MMICL_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
    //������ѡ�˵��ͬ������Ҫ��ʾ�ķ�����ֵ
    switch (menu_id)
    {
    case ID_CL_COSTS_TOTAL:
        DisplayClCostsTotal(MMICL_CALL_COSTS_CTRL_ID);
        break;
        
    case ID_CL_COSTS_MAX:
        DisplayClCostsMax(MMICL_CALL_COSTS_CTRL_ID);
        break;
        
    case ID_CL_COSTS_RATE:
        DisplayClCostsRate(MMICL_CALL_COSTS_CTRL_ID);
        break;
    default:
        break;
    }
}


//----------------------------------------------------------------------------------------------------------------
//  ��������         HandleGetPuctWaitWinMsg
//  ��������:        ��ȡPUCT�ȴ�������Ϣ������
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//----------------------------------------------------------------------------------------------------------------
MMI_RESULT_E HandleGetPuctWaitWinMsg(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_KEYUP_CANCEL:
        MMIPUB_CloseWaitWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return recode;
}
//----------------------------------------------------------------------------------------------------------------
//  ��������         MMISMSCB_HandleMainMenuMsg
//  ��������:        С���㲥���˵���Ϣ����ӿں���
//  ȫ�ֱ���ʹ��:    s_is_get_puct
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
void  MMICL_COSTS_HandleMainMenuMsg(
                                    MMI_MENU_ID_T menu_id
                                    )
{
    MMI_STRING_T          wait_text = {0};
    uint16                  i = 0;
    uint16            sim_ok = MN_DUAL_SYS_1;
    uint16            sim_ok_num = 0;
    
        if(MMIAPIPHONE_GetSimCardOkNum(PNULL, 0) == 0)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
            return;
        }
        
        switch(menu_id)
        {
        case ID_CL_COSTS_TOTAL:
        case ID_CL_COSTS_MAX:
        case ID_CL_COSTS_RATE:
            //����ȴ�����
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_COMMON_WAITING, &wait_text);
            MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MAIN_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleGetPuctWaitWinMsg);
            
            for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
            {
                if (MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)i))
                {
                    s_is_get_puct[i] = TRUE;        //�Ƿ�Ϊ GET PUCT ״̬
                    MNSIM_GetPuctEx((MN_DUAL_SYS_E)i);
                }
            }
            break;
            
        case ID_CL_COSTS_RESET:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMICL_SetMenuSelect(ID_CL_COSTS_RESET);
#endif
            sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
            
            if(1 == sim_ok_num)
            {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                MMICL_SetSimSelect((MN_DUAL_SYS_E)sim_ok );
#endif
                MMIAPIPHONE_SetCurHandlingPinSimID((MN_DUAL_SYS_E)sim_ok );
                if (MMIAPIPHONE_IsAocSupport(MMICL_GetSimSelect()))
                {
                    if(MMIAPIPHONE_IsPin2RequiredACM(MMICL_GetSimSelect()))
                    {
                        MMIAPIPHONE_HandlePinAndPukFlow(
                            MNSIM_PIN_VERIFY,
                            MNSIM_PIN2,
                            (uint32)MMICL_CALL_COSTS_RESET_WIN_TAB,
                            FALSE);
                    }
                    else
                    {
                        MMK_CreateWin((uint32 *)MMICL_CALL_COSTS_RESET_WIN_TAB,PNULL);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_SUPPORT);
                }
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE            
            else if (sim_ok_num > 1)
            {
                MMICL_SelectSIMFunc(CostSelectSimCallback, MMISET_APPEND_SIM_OK, PNULL);
            }
#endif            
            else
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            
            break;
        default:
            break;
        }
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         HandleCallCostsWindow
//  ��������:        ����ͨ�����õĹ��ô��ڴ�����
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleCallCostsWindow(
                                         MMI_WIN_ID_T		win_id, 	// STARTUP NORMAL���ڵ�ID
                                         MMI_MESSAGE_ID_E 	msg_id, 	// ���ڵ��ڲ���ϢID
                                         DPARAM 		    param		// ��Ӧ��Ϣ�Ĳ���
                                         )
{
    MMI_RESULT_E			recode = MMI_RESULT_TRUE;
    uint32					group_id = 0, menu_id = 0;
    GUI_LCD_DEV_INFO		lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    uint32                             sim_ok_num = 0;
    uint16                             sim_ok = MN_DUAL_SYS_1;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    sim_ok_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,MMICL_CALL_COSTS_CTRL_ID);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMICL_MAIN_MENU_CTRL_ID,&group_id,&menu_id);
        //������ѡ�˵��ͬ��������ͬ�ı༭����
        switch (menu_id)
        {
        case ID_CL_COSTS_MAX:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMICL_SetMenuSelect(ID_CL_COSTS_MAX);
#endif
            if(0 == sim_ok_num)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else if(sim_ok_num > 1)
            {
                MMICL_SelectSIMFunc(CostSelectSimCallback, MMISET_APPEND_SIM_OK, PNULL);
            }
#endif
            else
            {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                MMICL_SetSimSelect((MN_DUAL_SYS_E)sim_ok);
#endif
                MMIAPIPHONE_SetCurHandlingPinSimID((MN_DUAL_SYS_E)sim_ok);


                //��Ҫ��PIN2��֤������PIN2��֤���壬����ֱ�������޸�ͨ�����޴���
                if (MMIAPIPHONE_IsAocSupport(MMICL_GetSimSelect()))
                {
                    if(MMIAPIPHONE_IsPin2RequiredACMMax(MMICL_GetSimSelect()))
                    {
                        MMIAPIPHONE_HandlePinAndPukFlow(
                            MNSIM_PIN_VERIFY,
                            MNSIM_PIN2,
                            (uint32)MMICL_CALL_COSTS_MAX_UPDATE_WIN_TAB,
                            FALSE
                            );
                    }
                    else  
                    {
                        MMK_CreateWin((uint32*)MMICL_CALL_COSTS_MAX_UPDATE_WIN_TAB,PNULL);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_SUPPORT);
                }
            }
            break;
            
        case ID_CL_COSTS_RATE:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMICL_SetMenuSelect(ID_CL_COSTS_RATE);
#endif
            if(0 == sim_ok_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_SUPPORT);
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else if(sim_ok_num > 1)
            {
                MMICL_SelectSIMFunc(CostSelectSimCallback, MMISET_APPEND_SIM_OK, PNULL);
            }
#endif
            else
            {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                MMICL_SetSimSelect((MN_DUAL_SYS_E)sim_ok);
#endif
                MMIAPIPHONE_SetCurHandlingPinSimID((MN_DUAL_SYS_E)sim_ok);


                //��Ҫ��PIN2��֤������PIN2��֤���壬����ֱ�������޸�ͨ�����ʴ���     
                if (MMIAPIPHONE_IsAocSupport(MMICL_GetSimSelect()))
                {
                    if(MMIAPIPHONE_IsPin2RequiredPuct(MMICL_GetSimSelect()))
                    {
                        MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_VERIFY,MNSIM_PIN2,
                            (uint32)MMICL_CALL_COSTS_RATE_UPDATE_WIN_TAB,
                            FALSE);
                    }
                    else      
                    {
                        MMK_CreateWin((uint32*)MMICL_CALL_COSTS_RATE_UPDATE_WIN_TAB,PNULL);
                    }
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_SUPPORT);
                }
            }
            
            break;
            
        case ID_CL_COSTS_TOTAL:
            //MMK_CloseWin( win_id ); 
            break;
            
        default:
            break;
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         HandleCallCostsMaxUpdateWindow
//  ��������:        �޸�ͨ�����޴��ڴ�����
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleCallCostsMaxUpdateWindow(
                                                  MMI_WIN_ID_T    win_id, 	// STARTUP NORMAL���ڵ�ID
                                                  MMI_MESSAGE_ID_E 	 msg_id, 	// ���ڵ��ڲ���ϢID
                                                  DPARAM 		     param		// ��Ӧ��Ϣ�Ĳ���
                                                  )
{
    
    GUI_RECT_T				rect =   MMITHEME_GetClientRect();
    MMI_RESULT_E			recode = MMI_RESULT_TRUE;
    MMI_STRING_T			title_str = {0};
    MMI_STRING_T	costs_max_str = {0};
    uint32					acm_max = 0;
    GUI_LCD_DEV_INFO		lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MN_DUAL_SYS_E                  dual_sys = MN_DUAL_SYS_1;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMI_GetLabelTextByLang(TXT_CL_SET_COST_MAX, &title_str);
        GUIWIN_SetTitleText(win_id, title_str.wstr_ptr, title_str.wstr_len, FALSE);
        MMK_SetAtvCtrl(win_id,MMICL_COSTS_MAX_EDIT_CTRL_ID);
        break;
        
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        dual_sys = MMICL_GetSimSelect();
        GUIEDIT_GetString(MMICL_COSTS_MAX_EDIT_CTRL_ID,&costs_max_str);
        acm_max = GetUpdateAcmMax(
            &costs_max_str,
            s_sim_puct_info[dual_sys].ex,
            s_sim_puct_info[dual_sys].ex_sign,
            s_sim_puct_info[dual_sys].eppu
            );
        MNSIM_UpdateAcmMaxEx(dual_sys, acm_max);
#if !defined(MMI_PDA_SUPPORT)
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
#endif
        MMK_CloseWin(win_id);
        MMK_CloseWin( MMICL_CALL_COSTS_WIN_ID );
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL: 
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         ResetCallCostsRateUpdateWinTitle
//  ��������:        �������ͨ�����ʵĴ��ڱ���
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL void ResetCallCostsRateUpdateWinTitle(void)
{
    MMI_STRING_T          title = {0};
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_SET_COST_RATE, &title);
    GUIWIN_SetTitleText(MMICL_CALL_COSTS_RATE_UPDATE_WIN_ID,title.wstr_ptr,title.wstr_len, FALSE);
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         SetCallCostsRateStringInfo
//  ��������:        ����ͨ�����ʱ༭���ʼֵ
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL void SetCallCostsRateStringInfo(MN_DUAL_SYS_E dual_sys)
{
    int8    exponent = 0;
    uint8   charge_str[INTEGR_PART_LEN+FRACTIONAL_PART_LEN+CURRENCY_LEN+1] = {0};
    wchar   charge_wstr[INTEGR_PART_LEN+FRACTIONAL_PART_LEN+CURRENCY_LEN+1] = {0};
    wchar   currency_code[4];
    MMI_STRING_T    cur_str = {0};
    MMI_STRING_T    char_str = {0};
    MMI_STRING_T  title = {0};

    GUIFORM_SetStyle(MMICL_COSTS_CURRENCY_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMICL_COSTS_RATE_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);

    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_INPUT_CURRENCY, &title);
    GUILABEL_SetText(MMICL_COSTS_CURRENCY_LABEL_CTRL_ID,&title,FALSE);

    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_CL_INPUT_COST_RATE, &title);
    GUILABEL_SetText(MMICL_COSTS_RATE_LABEL_CTRL_ID,&title,FALSE);

    cur_str.wstr_ptr = currency_code;
    cur_str.wstr_len = strlen((char*)s_sim_puct_info[dual_sys].currency_code);
    MMI_STRNTOWSTR( currency_code, CURRENCY_LEN, (uint8 *)s_sim_puct_info[dual_sys].currency_code, cur_str.wstr_len, cur_str.wstr_len );

    GUIEDIT_SetString(MMICL_COSTS_CURRENCY_VAL_LABEL_CTRL_ID, cur_str.wstr_ptr, cur_str.wstr_len);
    
    if(s_sim_puct_info[dual_sys].ex_sign)    
    {
        exponent=s_sim_puct_info[dual_sys].ex;
    }
    else
    {
        exponent=-s_sim_puct_info[dual_sys].ex;
    }
    
    PuctDecodeToChargeStr(s_sim_puct_info[dual_sys].eppu,exponent,
        s_sim_puct_info[dual_sys].currency_code,
        charge_str,
        FALSE);

    char_str.wstr_ptr = charge_wstr;
    char_str.wstr_len = strlen((char*)charge_str);
    MMI_STRNTOWSTR( charge_wstr, INTEGR_PART_LEN + FRACTIONAL_PART_LEN + CURRENCY_LEN, 
        (uint8 *)charge_str, char_str.wstr_len, char_str.wstr_len );

    GUIEDIT_SetString(MMICL_COSTS_RATE_VAL_LABEL_CTRL_ID, char_str.wstr_ptr, char_str.wstr_len);
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         CostsRateUpdate
//  ��������:        �޸�ͨ������
//  ȫ�ֱ���ʹ��:    s_sim_puct_info
//  ����ֵ:          ��
//----------------------------------------------------------------------------------------------------------------
LOCAL void CostsRateUpdate(MN_DUAL_SYS_E dual_sys)
{
    MMI_STRING_T                charge_edit_str = {0};              //�����ʱ༭���ַ���
    MMI_STRING_T                currency_edit_str = {0};            //���ѻ��ҵ�λ�༭���ַ���
    uint32						eppu = 0;
    int8						exponent = 0;
    MNSIM_PUCT_INFO_T			puct_eppu = {0};

    GUIEDIT_GetString(MMICL_COSTS_CURRENCY_VAL_LABEL_CTRL_ID, &currency_edit_str);

    GUIEDIT_GetString(MMICL_COSTS_RATE_VAL_LABEL_CTRL_ID, &charge_edit_str);

    if(s_sim_puct_info[dual_sys].ex_sign)    
    {
        exponent=s_sim_puct_info[dual_sys].ex;
    }
    else
    {
        exponent=-s_sim_puct_info[dual_sys].ex;
    }

    eppu = s_sim_puct_info[dual_sys].eppu;
    
    if(currency_edit_str.wstr_len < CURRENCY_LEN)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CL_INPUT_CURRENCY_ERROR);
    }
    else if(!ConvertStrToPuct(&charge_edit_str,&eppu,&exponent))    //���зǷ��ַ�
    {
        MMIPUB_OpenAlertWarningWin(TXT_CL_INPUT_COST_RATE_ERROR);
    }
    else
    {
        SCI_MEMSET(&puct_eppu, 0, sizeof(MNSIM_PUCT_INFO_T));

        MMI_WSTRNTOSTR( (uint8 *)puct_eppu.currency_code, MN_MAX_CURRENCY_CODE_NUM,
            currency_edit_str.wstr_ptr, currency_edit_str.wstr_len, MN_MAX_CURRENCY_CODE_NUM );

        //strncpy((char*)puct_eppu.currency_code, (char*)currency_edit_str.str_ptr,MN_MAX_CURRENCY_CODE_NUM);
        
        if(currency_edit_str.wstr_len < 3)
        {
            puct_eppu.currency_code[CURRENCY_LEN-1]=0;
        }
        
        if(exponent < 0)   
        {
            puct_eppu.ex_sign = FALSE; 
            puct_eppu.ex = -exponent;  
        }
        else
        {
            puct_eppu.ex_sign=TRUE;
            puct_eppu.ex=exponent;  
        }
        
        puct_eppu.eppu=eppu;
        
        MNSIM_UpdatePuctEx(dual_sys, puct_eppu); 
#if !defined(MMI_PDA_SUPPORT)
        MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
#endif
        MMK_CloseWin( MMICL_CALL_COSTS_WIN_ID );                           
        MMK_CloseWin( MMICL_CALL_COSTS_RATE_UPDATE_WIN_ID );
    }
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         HandleCallCostsRateUpateWindow
//  ��������:        �޸�ͨ�����ʴ��ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E HandleCallCostsRateUpateWindow(
                                                  MMI_WIN_ID_T    win_id, 	// STARTUP NORMAL���ڵ�ID
                                                  MMI_MESSAGE_ID_E   msg_id, 	// ���ڵ��ڲ���ϢID
                                                  DPARAM 		      param		// ��Ӧ��Ϣ�Ĳ���
                                                  )
{
    
    MMI_RESULT_E			recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO		lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MN_DUAL_SYS_E           dual_sys = MMICL_GetSimSelect();
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
        
    switch(msg_id) 
    {
    case MSG_OPEN_WINDOW:
        ResetCallCostsRateUpdateWinTitle();
        SetCallCostsRateStringInfo(dual_sys);

        MMK_SetAtvCtrl(win_id,MMICL_COSTS_CURRENCY_VAL_LABEL_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        CostsRateUpdate(dual_sys);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

//----------------------------------------------------------------------------------------------------------------
//  ��������         HandleCallCostsResetWindow
//  ��������:        ��ռ��������ڴ�����
//  ȫ�ֱ���ʹ��:    ��
//  ����ֵ:          MMI_RESULT_TRUE �Ѵ���MMI_RESULT_FALSE δ����
//----------------------------------------------------------------------------------------------------------------
LOCAL MMI_RESULT_E  HandleCallCostsResetWindow(
                                               MMI_WIN_ID_T win_id,       // STARTUP NORMAL���ڵ�ID
                                               MMI_MESSAGE_ID_E msg_id,      // ���ڵ��ڲ���ϢID
                                               DPARAM param                  // ��Ӧ��Ϣ�Ĳ���
                                               )
{
    MMI_RESULT_E			recode = MMI_RESULT_TRUE;
    MN_RETURN_RESULT_E		return_val = MN_RETURN_FAILURE;
    GUI_RECT_T				rect = MMITHEME_GetClientRect();
    GUI_LCD_DEV_INFO		lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    ERR_MNDATAMAG_CODE_E    result = ERR_MNDATAMAG_NO_ERR;
    MN_DUAL_SYS_E           dual_sys = MMICL_GetSimSelect();
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_COMMON_OK,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN,FALSE);
        MMIPUB_OpenQueryWinByTextId(TXT_CL_COST_RESET,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
        break;
        
    case MSG_FULL_PAINT:
        LCD_FillRect(&lcd_dev_info, rect, MMI_GRAY_WHITE_COLOR);
        break;
        
    case MSG_PROMPTWIN_OK:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(PNULL);
        result = MNSIM_InitAcmUnitEx(dual_sys);
        if (ERR_MNDATAMAG_NO_ERR == result)
        {
            return_val = MN_RETURN_SUCCESS;
        }
        
        if(return_val==MN_RETURN_SUCCESS)      //�Ʒ�������ɹ�����ʾ�ɹ�
        {
#if !defined(MMI_PDA_SUPPORT)
            MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
#endif
        }
        else
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        //�ر�ѯ�ʴ���
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E MMICL_GetSimSelect(void)
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    return s_sim_select;
#else 
    return MN_DUAL_SYS_1;
#endif
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : to handle sim select window
//  Global resource dependence : 
//  Author:bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  CostSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    uint32          menu_select = 0;

    if (isReturnOK)
    {
        MMICL_SetSimSelect((MN_DUAL_SYS_E)dual_sys);
        MMIAPIPHONE_SetCurHandlingPinSimID((MN_DUAL_SYS_E)dual_sys);
        menu_select = MMICL_GetMenuSelect();

        switch(menu_select)
        {
        case ID_CL_COSTS_MAX:
            //��Ҫ��PIN2��֤������PIN2��֤���壬����ֱ�������޸�ͨ�����޴���
            if (MMIAPIPHONE_IsAocSupport((MN_DUAL_SYS_E)dual_sys)
            {
                if(MMIAPIPHONE_IsPin2RequiredACMMax((MN_DUAL_SYS_E)dual_sys)
                {
                MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_VERIFY,
                                        MNSIM_PIN2,
                                        (uint32)MMICL_CALL_COSTS_MAX_UPDATE_WIN_TAB,
                                        FALSE
                                        );
                }
            else  
            {
                MMK_CreateWin((uint32*)MMICL_CALL_COSTS_MAX_UPDATE_WIN_TAB, (ADD_DATA)param);
            }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_SUPPORT);
            }
            break;

        case ID_CL_COSTS_RATE:
            //��Ҫ��PIN2��֤������PIN2��֤���壬����ֱ�������޸�ͨ�����ʴ���     
            if (MMIAPIPHONE_IsAocSupport((MN_DUAL_SYS_E)dual_sys)
            {
            if(MMIAPIPHONE_IsPin2RequiredPuct((MN_DUAL_SYS_E)dual_sys)
            {
                MMIAPIPHONE_HandlePinAndPukFlow(MNSIM_PIN_VERIFY,MNSIM_PIN2,
                                        (uint32)MMICL_CALL_COSTS_RATE_UPDATE_WIN_TAB,
                                        FALSE);
            }
            else      
            {
                MMK_CreateWin((uint32*)MMICL_CALL_COSTS_RATE_UPDATE_WIN_TAB, (ADD_DATA)param);
            }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_SUPPORT);
            }
            break;

        case ID_CL_COSTS_RESET:
            if (MMIAPIPHONE_IsAocSupport((MN_DUAL_SYS_E)dual_sys)
            {
            if(MMIAPIPHONE_IsPin2RequiredACM((MN_DUAL_SYS_E)dual_sys)
            {
                MMIAPIPHONE_HandlePinAndPukFlow(
                    MNSIM_PIN_VERIFY,
                    MNSIM_PIN2,
                    (uint32)MMICL_CALL_COSTS_RESET_WIN_TAB,
                    FALSE);
            }
            else
            {
                MMK_CreateWin((uint32 *)MMICL_CALL_COSTS_RESET_WIN_TAB, (ADD_DATA)param);
            }
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_SIM_NOT_SUPPORT);
            }
            break;

        default:
            break;
        }
    }
    return recode;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_SetSimSelect(MN_DUAL_SYS_E sim_select)
{
    if(MMI_DUAL_SYS_MAX <= sim_select)
    {
        //SCI_TRACE_LOW:"MMICL_SetSimSelect sim_select = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICL_COSTS_WINTAB_1683_112_2_18_2_11_24_33,(uint8*)"d",sim_select);
        return;
    }
    s_sim_select = sim_select;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL void MMICL_SetMenuSelect(uint32 menu_select)
{
    s_menu_select = menu_select;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:bown.zhang
//	Note: 
/*****************************************************************************/
LOCAL uint32 MMICL_GetMenuSelect(void)
{
    return s_menu_select;
}
#endif

#endif  //AOC_SUPPORT


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527

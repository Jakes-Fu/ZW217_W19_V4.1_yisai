/****************************************************************************
** File Name:      mmisms_set.c                                            *
** Author:                                                                  *
** Date:           9/27/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe saving SMS.                *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 09/27/2007     liming.deng      Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_sms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_set.h"
#include "mmi_nv.h"
#include "mmisms_text.h"
#include "mmisms_id.h"
#include "mmi_modu_main.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern MMISMS_GLOBAL_T g_mmisms_global;      // SMS的全局变量

extern const wchar sms_default_to_ascii_table[];

extern const wchar sms_default_to_ascii_extension_table[];

const wchar default_to_turkish_table[128] =
{
    0x0040,0x00A3,0x0024,0x00A5,0x20AC,0x00E9,0x00F9,0x0131,
    0x00F2,0x00C7,0x000A,0x011E,0x011F,0x000D,0x00C5,0x00E5,
    0x0394,0x005F,0x03A6,0x0393,0x039B,0x03A9,0x03A0,0x03A8,
    0x03A3,0x0398,0x039E,0x001b,0x015E,0x015F,0x00DF,0x00C9,
    0x0020,0x0021,0x0022,0x0023,0x00A4,0x0025,0x0026,0x0027,
    0x0028,0x0029,0x002A,0x002B,0x002C,0x002D,0x002E,0x002F,
    0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,
    0x0038,0x0039,0x003A,0x003B,0x003C,0x003D,0x003E,0x003F,
    0x0130,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,
    0x0048,0x0049,0x004A,0x004B,0x004C,0x004D,0x004E,0x004F,
    0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,0x0057,
    0x0058,0x0059,0x005A,0x00C4,0x00D6,0x00D1,0x00DC,0x00A7,
    0x00E7,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,
    0x0068,0x0069,0x006A,0x006B,0x006C,0x006D,0x006E,0x006F,
    0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,
    0x0078,0x0079,0x007A,0x00E4,0x00F6,0x00F1,0x00FC,0x00E0
};

const wchar default_to_turkish_extension_table[128] =
{
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x005E,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x007B,0x007D,0x0020,0x0020,0x0020,0x0020,0x0020,0x005C,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x005B,0x007E,0x005D,0x0020,
    0x007C,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x011E,
    0x0020,0x0130,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x015E,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x00E7,0x0020,0x20AC,0x0020,0x011F,
    0x0020,0x0131,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x015F,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020
};

const wchar default_to_spanish_extension_table[128] =
{
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x00E7,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x005E,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x007B,0x007D,0x0020,0x0020,0x0020,0x0020,0x0020,0x005C,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x005B,0x007E,0x005D,0x0020,
    0x007C,0x00C1,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x00CD,0x0020,0x0020,0x0020,0x0020,0x0020,0x00D3,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x00DA,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x00E1,0x0020,0x0020,0x0020,0x20AC,0x0020,0x0020,
    0x0020,0x00ED,0x0020,0x0020,0x0020,0x0020,0x0020,0x00F3,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x00FA,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
};

const wchar default_to_portuguese_table[128] =
{
    0x0040,0x00A3,0x0024,0x00A5,0x00EA,0x00E9,0x00F9,0x00ED,
    0x00F3,0x00C7,0x000A,0x00D4,0x00F4,0x000D,0x00C5,0x00E5,
    0x0044,0x005F,0x00AA,0x00C7,0x00C0,0x221E,0x005E,0x005C,
    0x20AC,0x00D3,0x007C,0x001b,0x00C2,0x00E2,0x00CA,0x00C9,
    0x0020,0x0021,0x0022,0x0023,0x00BA,0x0025,0x0026,0x0027,
    0x0028,0x0029,0x002A,0x002B,0x002C,0x002D,0x002E,0x002F,
    0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,
    0x0038,0x0039,0x003A,0x003B,0x003C,0x003D,0x003E,0x003F,
    0x00CD,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,0x0047,
    0x0048,0x0049,0x004A,0x004B,0x004C,0x004D,0x004E,0x004F,
    0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,0x0057,
    0x0058,0x0059,0x005A,0x00C4,0x00D6,0x00DA,0x00DC,0x00A7,
    0x007E,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,
    0x0068,0x0069,0x006A,0x006B,0x006C,0x006D,0x006E,0x006F,
    0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,
    0x0078,0x0079,0x007A,0x00E3,0x00F5,0x0060,0x00FC,0x00E0
};

const wchar default_to_portuguese_extension_table[128] =
{
    0x0020,0x0020,0x0020,0x0020,0x0020,0x00EA,0x0020,0x0020,
    0x0020,0x00E7,0x0020,0x00D4,0x00F4,0x0020,0x00C1,0x00E1,
    0x0020,0x0020,0x03A6,0x0393,0x005E,0x03A9,0x03A0,0x03A8,
    0x03A3,0x0398,0x0020,0x0020,0x0020,0x0020,0x0020,0x00CA,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x007B,0x007D,0x0020,0x0020,0x0020,0x0020,0x0020,0x005C,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x0020,0x005B,0x007E,0x005D,0x0020,
    0x007C,0x00C0,0x0020,0x0020,0x0020,0x0020,0x0020,0x0020,
    0x0020,0x00CD,0x0020,0x0020,0x0020,0x0020,0x0020,0x00D3,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x00DA,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x00C3,0x00D5,0x0020,0x0020,0x0020,
    0x0020,0x00C2,0x0020,0x0020,0x0020,0x20AC,0x0020,0x0020,
    0x0020,0x00ED,0x0020,0x0020,0x0020,0x0020,0x0020,0x00F3,
    0x0020,0x0020,0x0020,0x0020,0x0020,0x00FA,0x0020,0x0020,
    0x0020,0x0020,0x0020,0x00E3,0x00F5,0x0020,0x0020,0x00E2,
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : set the default preset SMS to NV
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void SetDefaultPresetSms2Nv(void);

/*****************************************************************************/
//  Description : whether character is Turkish
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsTurkish(wchar value);

/*****************************************************************************/
//  Description : whether character is Portuguese
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsPortuguese(wchar value);

/*****************************************************************************/
//  Description : whether character is Portuguese
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsDefaultSpanish(wchar value);

/*****************************************************************************/
//  Description : whether character is Portuguese
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsDefaultPortuguese(wchar value);

/*****************************************************************************/
//  Description : whether character is Spanish or Portuguese
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsSpanishOrPortuguese(wchar value);

/*****************************************************************************/
//  Description : whether character is Spanish or Turkish
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsSpanishOrTurkish(wchar value);

/*****************************************************************************/
//  Description : operate the turkish character as the character in default ext table 
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN NationalR8AsDefaultExt(
                                uint16   national_wchar,         //IN:
                                uint8   *default_char_ptr,   //OUT:
                                MMISMS_R8_LANGUAGE_E lang    //IN:
                                );

/*****************************************************************************/
//     Description : set default preset SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetDefaultPresetSms(BOOLEAN is_clear)
{
    uint32 i = 0;
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    MMINV_SMS_FREQUENT_WORD_T word = {0};
    MMI_STRING_T text_str = {0};

    if (is_clear)//清除用户数据
    {
        SCI_MEMSET(&word, 0x00, sizeof(word));

        MMINV_WRITE(MMINV_SMS_FREQUENT_WORD, &word);

        SetDefaultPresetSms2Nv();
    }
    else
    {
        MMINV_READ(MMINV_SMS_FREQUENT_WORD, &word, nv_value);
        
        for (i = 0; i < MMISMS_DEFAULT_WORD_NUM; i++)
        {
            if (word.word[i].is_default_word)
            {
                MMI_GetLabelTextByLang(TXT_SMS_DEFAULT_WORD1+i, &text_str);

                SCI_MEMSET(&word.word[i].wstr_data, 0x00, sizeof(word.word[i].wstr_data));

                word.word[i].wstr_len = MIN(text_str.wstr_len, MMISMS_FREQUENT_WORD_LEN);

                MMI_WSTRNCPY(
                                            word.word[i].wstr_data,
                                            MMISMS_FREQUENT_WORD_LEN,
                                            text_str.wstr_ptr,
                                            word.word[i].wstr_len,
                                            word.word[i].wstr_len
                                            );
            }
        }

        MMINV_WRITE(MMINV_SMS_FREQUENT_WORD, &word);
    }
}

/*****************************************************************************/
//     Description : set the default preset SMS to NV
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void SetDefaultPresetSms2Nv(void)
{
    uint32 i = 0;
    MMI_STRING_T text_str = {0};
    MMINV_SMS_FREQUENT_WORD_T word = {0};
        
    for (i = 0; i<MMISMS_DEFAULT_WORD_NUM; i++)
    {
        MMI_GetLabelTextByLang(TXT_SMS_DEFAULT_WORD1+i, &text_str);
        
        word.word[i].wstr_len = MIN(text_str.wstr_len, MMISMS_FREQUENT_WORD_LEN);
        word.word[i].is_default_word = TRUE;
        
        MMI_WSTRNCPY(
                                    word.word[i].wstr_data,
                                    MMISMS_FREQUENT_WORD_LEN,
                                    text_str.wstr_ptr,
                                    word.word[i].wstr_len,
                                    word.word[i].wstr_len
                                    );
    }
    
    MMINV_WRITE(MMINV_SMS_FREQUENT_WORD, &word);
}

/*****************************************************************************/
//     Description : to set the reference number to NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void  MMISMS_SetRefNumToNV(
                                  MMINV_SMS_REFERENCE_NUM_T   ref_num
                                  )
{
    //SCI_TRACE_LOW:"MMISMS: MMISMS_SetRefNumToNV ref_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_279_112_2_18_2_53_26_456,(uint8*)"d", ref_num);
    
    MMINV_WRITE(MMINV_SMS_REFERENCE_NUM, &ref_num);
    g_mmisms_global.nv_param.ref_num = ref_num;
}

/*****************************************************************************/
//     Description : to get the valid period from NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC MMINV_SMS_VALID_PERIOD_T MMISMS_GetValidPeriod( void )
{
    MMINV_SMS_VALID_PERIOD_T vaild_period = 0;
    MN_RETURN_RESULT_E  return_value    =   MN_RETURN_FAILURE;
    
    MMINV_READ( MMINV_SMS_VALID_PERIOD, &vaild_period, return_value );
    if (MN_RETURN_SUCCESS != return_value)
    {
        vaild_period = g_mmisms_global.nv_param.vaild_period;
    }

    return vaild_period;
}

/*****************************************************************************/
//     Description : to set the valid period from NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetValidPeriod(
                                  MMINV_SMS_VALID_PERIOD_T   vaild_period
                                  )
{
    MMINV_WRITE(MMINV_SMS_VALID_PERIOD, &vaild_period);    
    
    g_mmisms_global.nv_param.vaild_period = vaild_period;
}

/*****************************************************************************/
//     Description : to delete a pointed frequent word in NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_DeletePointedWord(                    //RETURN:
                                        uint8    item_id        //IN: the item index started form 0
                                        )
{
    BOOLEAN ret_val = FALSE;
    // uint8 i = 0;
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    MMINV_SMS_FREQUENT_WORD_T freq_word = {0};
        
    MMINV_READ(MMINV_SMS_FREQUENT_WORD, &freq_word, nv_value);
    
    if (MN_RETURN_SUCCESS == nv_value && MMISMS_FREQUENT_WORD_NUM > item_id)
    {
        SCI_MEMSET(&freq_word.word[item_id], 0, sizeof(MMI_NV_SMS_PHRASE_T));
        
        MMINV_WRITE(MMINV_SMS_FREQUENT_WORD, &freq_word);
        
        ret_val = TRUE;
    }
    
    return (ret_val);
}

/*****************************************************************************/
//     Description : to get the all frequent word in NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC void  MMISMS_GetFrequentWord( 
                                    MMINV_SMS_FREQUENT_WORD_T    *word_ptr //[out]:the frequent word
                                    )
                                    
{
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    
    if (PNULL == word_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_GetFrequentWord word_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_356_112_2_18_2_53_26_457,(uint8*)"");
        return;
    }
    SCI_MEMSET(word_ptr, 0, sizeof (MMINV_SMS_FREQUENT_WORD_T));
    
    MMINV_READ(MMINV_SMS_FREQUENT_WORD, word_ptr, nv_value);
}

/*****************************************************************************/
//     Description : to modify a pointed frequent word in NV
//    Global resource dependence : none
//  Author:louis.wei
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ModifyPointedWord(
                                        uint8                item_id,        //IN: the item index started form 0
                                        MMI_NV_SMS_PHRASE_T    *item_phase_ptr //IN: the word of pointed item
                                        )
{
    BOOLEAN ret_val  = FALSE;
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    MMINV_SMS_FREQUENT_WORD_T freq_word = {0};
    
    if (PNULL == item_phase_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_ModifyPointedWord item_phase_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_379_112_2_18_2_53_26_458,(uint8*)"");
        return FALSE;
    }        
    // read from NV
    MMINV_READ(MMINV_SMS_FREQUENT_WORD, &freq_word, nv_value);
    
    if (MN_RETURN_SUCCESS == nv_value && MMISMS_FREQUENT_WORD_NUM > item_id)
    {
        SCI_MEMCPY( 
            &freq_word.word[item_id],
            item_phase_ptr, 
            sizeof (MMI_NV_SMS_PHRASE_T) 
            );

        freq_word.word[item_id].is_default_word = FALSE;

        if (item_id < MMISMS_DEFAULT_WORD_NUM)
        {
            MMI_STRING_T ori_text_str = {0};
            MMI_STRING_T new_text_str = {0};

            MMI_GetLabelTextByLang(TXT_SMS_DEFAULT_WORD1+item_id, &ori_text_str);
            new_text_str.wstr_ptr = (wchar *)freq_word.word[item_id].wstr_data;
            new_text_str.wstr_len = freq_word.word[item_id].wstr_len;

            if (ori_text_str.wstr_len == new_text_str.wstr_len
                && (0 == MMIAPICOM_Wstrncmp(ori_text_str.wstr_ptr, new_text_str.wstr_ptr,new_text_str.wstr_len)))
            {
                freq_word.word[item_id].is_default_word = TRUE;
            }
        }
        MMINV_WRITE(MMINV_SMS_FREQUENT_WORD, &freq_word);
        
        ret_val = TRUE;
    }
    
    return (ret_val);
}

/*****************************************************************************/
//     Description : init preset SMS
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitPresetSms(void)
{
    MMINV_SMS_FREQUENT_WORD_T word = {0};
    MN_RETURN_RESULT_E nv_value = MN_RETURN_SUCCESS;
    
    // read preset SMS from NV
    MMINV_READ(MMINV_SMS_FREQUENT_WORD, &word, nv_value);
    
    if (MN_RETURN_SUCCESS != nv_value)
    {
        // set the default value
        SetDefaultPresetSms2Nv();
    }
}

/*****************************************************************************/
//     Description : to get the sc addr called by window
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 得到信息服务中心号码
/*****************************************************************************/
PUBLIC BOOLEAN    MMISMS_AppGetSCAddr(
                                      MN_DUAL_SYS_E   dual_sys,
                                      uint8    *sc_addr,        //OUT:
                                      uint8    *sc_addr_len,    //OUT:
                                      uint8    max_addr_len    //IN:
                                      )
{
    BOOLEAN                ret_val = FALSE;
    MN_CALLED_NUMBER_T  sc_addr_t = {MN_NUM_TYPE_UNKNOW,MN_NUM_PLAN_UNKNOW,0,0};
#ifndef MMI_SMS_MULTI_PARAM_SUPPORT
    ERR_MNSMS_CODE_E    retcode = ERR_MNSMS_NONE;
#endif
    MN_RETURN_RESULT_E  result  = MN_RETURN_FAILURE;
    
    if (PNULL == sc_addr || PNULL == sc_addr_len)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_AppGetSCAddr sc_addr sc_addr_len = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_512_112_2_18_2_53_26_459,(uint8*)"");
        return FALSE;
    }
    
#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
    if(g_mmisms_global.set_mode.tp_sc_addr_present)
    {
        result = MN_RETURN_SUCCESS;

        SCI_MEMCPY(&sc_addr_t,&g_mmisms_global.set_mode.tp_sc_addr_t,sizeof(MN_CALLED_NUMBER_T));
    }
#else
    retcode = MNSMS_GetScAddrEx(dual_sys, &sc_addr_t);

    if (ERR_MNSMS_NONE == retcode)
    {
        result  = MN_RETURN_SUCCESS;
    }
#endif

    if (MN_RETURN_SUCCESS == result)
    {
        // translate the BCD code to string number
        *sc_addr_len = MMIAPICOM_GenDispNumber( 
            sc_addr_t.number_type, 
            sc_addr_t.num_len, 
            sc_addr_t.party_num,
            sc_addr,
            max_addr_len
            );

        ret_val = TRUE;
    }
    else
    {
        *sc_addr_len = 0;
    }
    
    return (ret_val);
}

/*****************************************************************************/
//     Description : judge whether the character is R8 and need one byte
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsR8SingleByte( wchar value )
{
    BOOLEAN is_r8singlebyte = FALSE;

    if(( value == 0x00E8 ) || ( value == 0x00E9 ) || ( value == 0x00F9 ) ||
        ( value == 0x00EC ) || ( value == 0x00F2 ) || ( value == 0x00C7 ) || 
        ( value == 0x00D8 ) || ( value == 0x00F8 ) || ( value == 0x00C5 ) || 
        ( value == 0x00E5 ) || ( value == 0x00C6 ) || ( value == 0x00E6 ) || 
        ( value == 0x00DF ) || ( value == 0x00C4 ) || ( value == 0x00D6 ) ||
        ( value == 0x00D1 ) || ( value == 0x00DC ) || ( value == 0x00A7 ) || 
        ( value == 0x00E4 ) || ( value == 0x00F6 ) || ( value == 0x00F1 ) || 
        ( value == 0x00FC ) || ( value == 0x00E0 ) || ( value == 0x00C9 ) || 
        ( value == 0x00A1 ) || ( value == 0x00BF ) || ( value == 0x00AA ) ||
        ( value == 0x221E ) || ( value == 0x00BA ))
    {
        is_r8singlebyte = TRUE;
    }

    return is_r8singlebyte;
}

/*****************************************************************************/
//     Description : judge whether the character is R8 and need two bytes
//    Global resource dependence : 
//  Author: fengming.huang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsR8TwoBytes( wchar value )
{
    BOOLEAN is_r8twobytes = FALSE;
    
    if(( value == 0x00E7 ) || ( value == 0x00C1 ) || ( value == 0x00CD ) || 
        ( value == 0x00D3 ) ||( value == 0x00DA ) || ( value == 0x00E1 ) || 
        ( value == 0x00ED ) ||( value == 0x00F3 ) || ( value == 0x00FA ) ||
        ( value == 0x00EA ) ||( value == 0x00D4 ) || ( value == 0x00F4 ) || 
        ( value == 0x00CA ) ||( value == 0x00C0 ) || ( value == 0x00C3 ) ||
        ( value == 0x00D5 ) ||( value == 0x00C2 ) || ( value == 0x00E3 ) ||
        ( value == 0x00F5 ) ||( value == 0x00E2 ) || ( value == 0x001b)  ||
        ( value == 0x015E ) ||( value == 0x015F ) || ( value == 0x011E ) ||
        ( value == 0x011F ) ||( value == 0x0130 ) || ( value == 0x0131 ))
    {
        is_r8twobytes = TRUE;
    }

    return is_r8twobytes;
}

/*****************************************************************************/
//  Description : decode the sms user data head
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_DecodeUserDataHead (                             // return void
                MN_SMS_USER_DATA_HEAD_T     *user_data_header_ptr_t, // in---the user data head structure before decode
                MN_SMS_USER_HEAD_T          *user_head_ptr_t)        // out---the user data head structure after decode
{
    uint8 cur_off = 0;
    uint8 temp_num = 0;
        
    if (PNULL == user_data_header_ptr_t || PNULL == user_head_ptr_t)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_DecodeUserDataHead user_data_header_ptr_t user_head_ptr_t = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_615_112_2_18_2_53_27_460,(uint8*)"");
        return;
    }
    SCI_MEMSET((void*)user_head_ptr_t,0,sizeof(MN_SMS_USER_HEAD_T));    
    while(cur_off < user_data_header_ptr_t->length)
    {
        if(temp_num < MN_SMS_MAX_IEI_NUM)
        {
            user_head_ptr_t->iei_arr[temp_num].iei = (MN_SMS_IEI_ID_E)user_data_header_ptr_t->user_header_arr[cur_off];
            cur_off++;
            user_head_ptr_t->iei_arr[temp_num].length = user_data_header_ptr_t->user_header_arr[cur_off];
            cur_off++;
            if((user_head_ptr_t->iei_arr[temp_num].length <= MN_SMS_USER_DATA_LENGTH - 3)&&
                (user_head_ptr_t->iei_arr[temp_num].length > 0))
            {
                user_head_ptr_t->iei_arr[temp_num].iei_data_t = SCI_ALLOC_APP(user_head_ptr_t->iei_arr[temp_num].length);
                SCI_MEMSET((char *)user_head_ptr_t->iei_arr[temp_num].iei_data_t,0x00,user_head_ptr_t->iei_arr[temp_num].length);
                if(user_head_ptr_t->iei_arr[temp_num].iei_data_t != PNULL)
                {
                    SCI_MEMCPY((void*)user_head_ptr_t->iei_arr[temp_num].iei_data_t,
                               &(user_data_header_ptr_t->user_header_arr[cur_off]),
                               user_head_ptr_t->iei_arr[temp_num].length);
                }
                cur_off += user_head_ptr_t->iei_arr[temp_num].length;
            }
            temp_num++;
        }
        else
        {
            break;
        }
    }
    
    user_head_ptr_t->iei_num = temp_num;
    return;
}

/*****************************************************************************/
//  Description : free the sms user data head space
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_FreeUserDataHeadSpace(                             // return void
                MN_SMS_USER_HEAD_T          *user_head_ptr_t)        // in---the user data head structure to be free
{
    uint8   i = 0;
    
    if (PNULL == user_head_ptr_t)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_FreeUserDataHeadSpace user_head_ptr_t = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_663_112_2_18_2_53_27_461,(uint8*)"");
        return;
    }
    while(i<user_head_ptr_t->iei_num)
    {
        if(user_head_ptr_t->iei_arr[i].iei_data_t != PNULL)
        {
            
            SCI_FREE(user_head_ptr_t->iei_arr[i].iei_data_t);
        }
        user_head_ptr_t->iei_arr[i].iei = MN_SMS_CONCAT_8_BIT_REF_NUM;
        user_head_ptr_t->iei_arr[i].length = 0;
        i++;
        if(i >= MN_SMS_MAX_IEI_NUM)
        {
            break;
        }
    }
    user_head_ptr_t->iei_num = 0;
    return;
}

/*****************************************************************************/
//  Description : judge the language of R8
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMISMS_R8_LANGUAGE_E MMISMS_JudgeR8Language(
                MN_SMS_USER_HEAD_T    *user_head_ptr_t)
{
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
    int16 i =0;

    for(i=0; i<user_head_ptr_t->iei_num; i++)
    {
        if(user_head_ptr_t->iei_arr[i].iei == MN_SMS_NATIONAL_LANG_SINGLE_SHIFT)
        {
	     if(user_head_ptr_t->iei_arr[i].length == MMISMS_R8_HEAD_IEL)
	         language = (MMISMS_R8_LANGUAGE_E)(*user_head_ptr_t->iei_arr[i].iei_data_t);
        }
	 else if(user_head_ptr_t->iei_arr[i].iei == MN_SMS_NATIONAL_LANG_LOCK_SHIFT)
        {
	     if(user_head_ptr_t->iei_arr[i].length == MMISMS_R8_HEAD_IEL)
	         language = (MMISMS_R8_LANGUAGE_E)(*user_head_ptr_t->iei_arr[i].iei_data_t);
        }
    }

    return language;
}

/*****************************************************************************/
//  Description : judge the language of R8
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_IsR8Language(
                MMISMS_R8_LANGUAGE_E    language)
{
    BOOLEAN is_r8_language = FALSE;

    if((MMISMS_R8_LANGUAGE_TURKISH == language) ||
        (MMISMS_R8_LANGUAGE_SPANISH == language) ||
        (MMISMS_R8_LANGUAGE_PORTUGUESE == language)) 
    {
        is_r8_language = TRUE;
    }

    return is_r8_language;
}

/*****************************************************************************/
//  Description : Change from 03.38 alphabet table to International alphabet table
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMISMS_Default2NationalR8(                          //RETURN:
                                      uint8     *default_ptr,   //IN:
                                      uint16      *national_ptr,        //OUT:
                                      uint16     str_len,            //IN:
                                      MMISMS_R8_LANGUAGE_E language,   //IN
                                      BOOLEAN is_lock_shift,
                                      BOOLEAN is_single_shift
                                      )
{
    uint16 national_len = 0;
    int32 i      = 0;

    if (PNULL == default_ptr || PNULL == national_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_Default2NationalR8 default_ptr national_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_750_112_2_18_2_53_27_462,(uint8*)"");
        return 0;
    }
    
    for (i=0; i<str_len; i++)
    {
            if (default_ptr[i] == DEFAULT_EXT_MARK)
            {
                i++;

                if(TRUE == is_single_shift)
                {
                    switch(language)
                    {
                        case MMISMS_R8_LANGUAGE_TURKISH://Turish
                        {
                            national_ptr[national_len] = default_to_turkish_extension_table[default_ptr[i]];
                            national_len++;
                        }
                        break;

                        case MMISMS_R8_LANGUAGE_SPANISH://Spanish
                        {
                            national_ptr[national_len] = default_to_spanish_extension_table[default_ptr[i]];
                            national_len++;
                        }
                        break;

                        case MMISMS_R8_LANGUAGE_PORTUGUESE ://Portuguese
                        {
                            national_ptr[national_len] = default_to_portuguese_extension_table[default_ptr[i]];
                            national_len++;
                        }
                        break;

                        default:
                        {
                            national_ptr[national_len] = sms_default_to_ascii_extension_table[default_ptr[i]];
                            national_len++;
                        }
                        break;
                    }
                }

                else
                {
                    national_ptr[national_len] = sms_default_to_ascii_extension_table[default_ptr[i]];
                    national_len++;
                }
            }
            else
            {
                if(TRUE == is_lock_shift)
                {
                    switch(language)
                    {
                        case MMISMS_R8_LANGUAGE_TURKISH://Turish
                        {
                            national_ptr[national_len] = default_to_turkish_table[default_ptr[i]];
                            national_len++;
                        }
                        break;

                        case MMISMS_R8_LANGUAGE_PORTUGUESE ://Portuguese
                        {
                            national_ptr[national_len] = default_to_portuguese_table[default_ptr[i]];
                            national_len++;
                        }
                        break;

                        default:
                        {
                            national_ptr[national_len] = sms_default_to_ascii_table[default_ptr[i]];
                            national_len++;
                        }
                        break;
                    }
                }

                else
                {
                    national_ptr[national_len] = sms_default_to_ascii_table[default_ptr[i]];
                    national_len++;
                }
            }
    }

    return (national_len);
}

/*****************************************************************************/
//  Description : Change from Turkish alphabet table to 03.38 alphabet table
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMISMS_NationalR82default(                          //RETURN: the length of default string
                         uint16      *national_ptr,     //IN:
                         uint8      *default_ptr,   //OUT:
                         uint16     str_len,         //IN:
                         MMISMS_R8_LANGUAGE_E       lang    //IN:
                         )
{
    uint16  default_len = 0;
    int32   i = 0, j = 0;
    uint8   temp_char   = 0;

    if (PNULL == default_ptr || PNULL == national_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMISMS_Default2NationalR8 default_ptr national_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_858_112_2_18_2_53_27_463,(uint8*)"");
        return 0;
    }
    for (i=0; i<str_len; i++)
    {
        if (NationalR8AsDefaultExt(national_ptr[i], &temp_char,lang))
        {
            default_ptr[default_len] = DEFAULT_EXT_MARK;
            default_len++;
            default_ptr[default_len] = temp_char;
            default_len++;
        }
        else
        {
            if(MMISMS_R8_LANGUAGE_TURKISH == lang)
            {
                for(j=0; j<128; j++ )
                {
                    if(national_ptr[i] == default_to_turkish_table[j])
                    {
                        default_ptr[default_len] = j;
                        default_len++;
                        break;
                    }
                }
            }

            else if(MMISMS_R8_LANGUAGE_PORTUGUESE== lang)
            {
                for(j=0; j<128; j++ )
                {
                    if(national_ptr[i] == default_to_portuguese_table[j])
                    {
                        default_ptr[default_len] = j;
                        default_len++;
                        break;
                    }
                }
            }    

            else
            {
                for(j=0; j<128; j++ )
                {
                    if(national_ptr[i] == sms_default_to_ascii_table[j])
                    {
                        default_ptr[default_len] = j;
                        default_len++;
                        break;
                    }
                }    
            }
        }

        if (default_len == MMISMS_R8_MESSAGE_LEN)
        {
            return (default_len);
        }
    }

    return (default_len);
}

/*****************************************************************************/
//  Description : Decode R8 Header
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC void MMISMS_DecodeR8Header(                          //RETURN:
                                      MN_SMS_USER_HEAD_T    *user_head_ptr_t,   //IN:
                                      BOOLEAN      *is_lock_shift,        //OUT:
                                      BOOLEAN      *is_single_shift            //OUT:
                                      )
{
    int16 i = 0;

    for(i=0; i<user_head_ptr_t->iei_num; i++)
    {
        if(user_head_ptr_t->iei_arr[i].iei == MN_SMS_NATIONAL_LANG_SINGLE_SHIFT)
        {
	     *is_single_shift = TRUE;
        }
	 else if(user_head_ptr_t->iei_arr[i].iei == MN_SMS_NATIONAL_LANG_LOCK_SHIFT)
        {
	     *is_lock_shift = TRUE;
        }
    }

    return;
}

/*****************************************************************************/
//  Description : Decode R8 Language to judge language
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC MMISMS_R8_LANGUAGE_E MMISMS_R8_DecodeContentForLanguage(
                                    wchar *content,
                                    uint16 len,
                                    BOOLEAN *is_single_shift)
{
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
    int16 i = 0,j = 0;

#ifdef MMI_R8_SUPPORT
    for(i=0;i<len;i++)
    {
        if((MMISMS_IsR8SingleByte(content[i]) || (content[i] == 0x00E7))
            && (MMISMS_R8_LANGUAGE_NONE == language)
            && !MMISMS_R8_IsDefaultPortuguese(content[i])
            && !MMISMS_R8_IsDefaultSpanish(content[i]))
        {
            language = MMISMS_R8_LANGUAGE_LATIN;
        }

        if(content[i] == 0x00E7)
        {
            *is_single_shift = TRUE;
        }

        else if(MMISMS_R8_IsDefaultPortuguese(content[i]) )
        {
            if((MMISMS_R8_LANGUAGE_NONE == language)
                || (MMISMS_R8_LANGUAGE_LATIN == language)
                || (MMISMS_R8_LANGUAGE_PORTUGUESE == language)
                || (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language))
            {
                language = MMISMS_R8_LANGUAGE_PORTUGUESE;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }    
        }

        else if(MMISMS_R8_IsDefaultSpanish(content[i]) )
        {
            if((MMISMS_R8_LANGUAGE_NONE == language)
                || (MMISMS_R8_LANGUAGE_LATIN == language)
                || (MMISMS_R8_LANGUAGE_SPANISH == language)
                || (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language)
                || (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }    
        }
        

        else if(MMISMS_R8_IsTurkish(content[i]))
        {
            if((MMISMS_R8_LANGUAGE_NONE == language) || 
                (MMISMS_R8_LANGUAGE_TURKISH == language) ||
                (MMISMS_R8_LANGUAGE_LATIN == language) ||
                (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language))
            {
                language = MMISMS_R8_LANGUAGE_TURKISH;
                *is_single_shift = TRUE;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }
        }

        else if(MMISMS_R8_IsPortuguese(content[i]))
        {
            if((MMISMS_R8_LANGUAGE_NONE == language) || 
                (MMISMS_R8_LANGUAGE_PORTUGUESE == language) ||
                (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language) ||
                (MMISMS_R8_LANGUAGE_LATIN == language))
            {
                language = MMISMS_R8_LANGUAGE_PORTUGUESE;
                *is_single_shift = TRUE;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }
        }

        else if(MMISMS_R8_IsSpanishOrPortuguese(content[i]))
        {
            if((MMISMS_R8_LANGUAGE_NONE == language) || 
                (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language) ||
                (MMISMS_R8_LANGUAGE_LATIN == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE;
                *is_single_shift = TRUE;
            }
            else if((MMISMS_R8_LANGUAGE_SPANISH == language) ||
                    (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH;
            }
            else if(MMISMS_R8_LANGUAGE_PORTUGUESE == language)
            {
                language = MMISMS_R8_LANGUAGE_PORTUGUESE;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }
        }

        else if(MMISMS_R8_IsSpanishOrTurkish(content[i]))
        {
            if((MMISMS_R8_LANGUAGE_NONE == language) || 
                (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language) ||
                (MMISMS_R8_LANGUAGE_LATIN == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH;
            }
            else if((MMISMS_R8_LANGUAGE_SPANISH == language) ||
                    (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH;
            }
            else if(MMISMS_R8_LANGUAGE_TURKISH == language)
            {
                language = MMISMS_R8_LANGUAGE_TURKISH;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }
        }
        else if((content[i] == 0xA3) || (content[i] == 0xA5))
        {
            language = MMISMS_R8_LANGUAGE_UNICODE;
            return language;            
        }
    }

    if(MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language)
    {
        language = MMISMS_R8_LANGUAGE_SPANISH;
    }
    else if((MMISMS_R8_LANGUAGE_LATIN == language) ||
               (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language))
    {
        language = MMISMS_R8_LANGUAGE_TURKISH;
    }

    for(i=0;i<len;i++)
    {
        if (MMITHEME_IsR82ByteChar(content[i]))
        {
            *is_single_shift = TRUE;
            break;
        }
    }

    for(i=0;i<len;i++)
    {
            switch(language)
            {
                case MMISMS_R8_LANGUAGE_TURKISH:
                    for(j=0;j<128;j++)
                    {
                        if((content[i] == default_to_turkish_table[j]) ||
                            (content[i] == default_to_turkish_extension_table[j]))
                        {
                            break;
                        }
                        if(j == 127)
                        {
                            language = MMISMS_R8_LANGUAGE_UNICODE;
                            return language;
                        }
                    }
                    break;

                case MMISMS_R8_LANGUAGE_SPANISH:
                    for(j=0;j<128;j++)
                    {
                        if((content[i] == sms_default_to_ascii_table[j]) ||
                           (content[i] == default_to_spanish_extension_table[j]))
                        {
                            break;
                        }
                        if(j == 127)
                        {
                            language = MMISMS_R8_LANGUAGE_UNICODE;
                            return language;
                        }
                    }
                    break;

                case MMISMS_R8_LANGUAGE_PORTUGUESE:
                    for(j=0;j<128;j++)
                    {
                        if((content[i] == default_to_portuguese_table[j]) ||
                            (content[i] == default_to_portuguese_extension_table[j]))
                        {
                            break;
                        }
                        if(j == 127)
                        {
                            language = MMISMS_R8_LANGUAGE_UNICODE;
                            return language;
                        }
                    }
                    break;

                default:
                    break;
            }
    }
#endif

    return language;
}

/*****************************************************************************/
//  Description : whether character is Turkish
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsTurkish(wchar value)
{
    BOOLEAN is_turkish = FALSE;

    if((0x015E == value) || (0x015F == value) || (0x011E == value) ||
        (0x011F == value) || (0x0130 == value) || (0x0131 == value))
    {
        is_turkish = TRUE;
    }
    
    return is_turkish;
}

/*****************************************************************************/
//  Description : whether character is Portuguese
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsPortuguese(wchar value)
{
    BOOLEAN is_portuguese = FALSE;

    if((0x00C0 == value) || (0x00C2 == value) || (0x00EA == value) ||
        (0x00D4 == value) || (0x00F4 == value) || (0x00CA == value) ||
        (0x00C3 == value) || (0x00E3 == value) || (0x00D5 == value) ||
        (0x00F5 == value) || (0x00E2 == value))
    {
        is_portuguese = TRUE;
    }
    
    return is_portuguese;
}

/*****************************************************************************/
//  Description : whether character is Portuguese
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsDefaultSpanish(wchar value)
{
    BOOLEAN is_default_spanish = FALSE;

    if((0x00A1 == value) || (0x00BF == value))
    {
        is_default_spanish = TRUE;
    }
    
    return is_default_spanish;
}

/*****************************************************************************/
//  Description : whether character is Portuguese
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsDefaultPortuguese(wchar value)
{
    BOOLEAN is_default_portuguese = FALSE;
    
    if((0x00BA == value) || (0x221E == value) || (0x00AA == value))
    {
        is_default_portuguese = TRUE;
    }
    
    return is_default_portuguese;
}

/*****************************************************************************/
//  Description : whether character is Spanish or Portuguese
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsSpanishOrPortuguese(wchar value)
{
    BOOLEAN is_spanish_or_portuese = FALSE;

    if((0x00DA == value) || (0x00FA == value) || (0x00CD == value) ||
        (0x00ED == value) || (0x00D3 == value) || (0x00F3 == value) ||
        (0x00C1 == value) || (0x00E1 == value))
    {
        is_spanish_or_portuese = TRUE;
    }
    
    return is_spanish_or_portuese;
}

/*****************************************************************************/
//  Description : whether character is Spanish or Turkish
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN MMISMS_R8_IsSpanishOrTurkish(wchar value)
{
    BOOLEAN is_spanish_or_turkish = FALSE;
    
    return is_spanish_or_turkish;
}

/*****************************************************************************/
//  Description : operate the turkish character as the character in default ext table 
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN NationalR8AsDefaultExt(
                                uint16   national_wchar,         //IN:
                                uint8   *default_char_ptr,   //OUT:
                                MMISMS_R8_LANGUAGE_E lang    //IN:
                                )
{
    BOOLEAN     result = TRUE;

    switch (national_wchar) 
    {
    case '^':
        *default_char_ptr = 0x14;
        break;

    case '{':
        *default_char_ptr = 0x28;
        break;

    case '}':
        *default_char_ptr = 0x29;
        break;

    case '\\':
        *default_char_ptr = 0x2f;
        break;

    case '[':
        *default_char_ptr = 0x3c;
        break;

    case '~':
        *default_char_ptr = 0x3d;
        break;

    case ']':
        *default_char_ptr = 0x3e;
        break;
        
    case '|':
        *default_char_ptr = 0x40;
        break;
        
    case EURO_UNICODE:
        *default_char_ptr = 0x65;
        break;
        
    default:
        *default_char_ptr = 0x20;
        result = FALSE;
        break;
    }

    if((( MMISMS_R8_LANGUAGE_TURKISH == lang ) || ( MMISMS_R8_LANGUAGE_SPANISH== lang ) ||
        ( MMISMS_R8_LANGUAGE_PORTUGUESE == lang )) && ( FALSE == result ))
    {
        result = TRUE;
        
        switch(lang)
        {
            case MMISMS_R8_LANGUAGE_TURKISH:
            {
                switch(national_wchar)
                {
                    case 0x011E:
                        *default_char_ptr = 0x47;
                        break;

                    case 0x0130:
                        *default_char_ptr = 0x49;
                        break;    

                    case 0x015E:
                        *default_char_ptr = 0x53;
                        break;    

                    case 0x00E7:
                        *default_char_ptr = 0x63;
                        break;   

                    case 0x011F:
                        *default_char_ptr = 0x67;
                        break;

                    case 0x015F:
                        *default_char_ptr = 0x73;
                        break;    

                    case 0x0131:
                        *default_char_ptr = 0x69;
                        break;    

                    default:
                        *default_char_ptr = 0x20;
                        result = FALSE;
                        break;
                }

                break;
            }
            
            case MMISMS_R8_LANGUAGE_SPANISH:
            {
                switch(national_wchar)
                {
                    case 0x00E7:
                        *default_char_ptr = 0x09;
                        break;

                    case 0x00C1:
                        *default_char_ptr = 0x41;
                        break;    

                    case 0x00CD:
                        *default_char_ptr = 0x49;
                        break;    

                    case 0x00D3:
                        *default_char_ptr = 0x4F;
                        break;   

                    case 0x00DA:
                        *default_char_ptr = 0x55;
                        break;

                    case 0x00E1:
                        *default_char_ptr = 0x61;
                        break;

                    case 0x00ED:
                        *default_char_ptr = 0x69;
                        break;       

                    case 0x00F3:
                        *default_char_ptr = 0x6F;
                        break;   

                    case 0x00FA:
                        *default_char_ptr = 0x75;
                        break;       

                    default:
                        *default_char_ptr = 0x20;
                        result = FALSE;
                        break;
                }

                break;
            }
            
            case MMISMS_R8_LANGUAGE_PORTUGUESE:
            {
                switch(national_wchar)
                {
                    case 0x00EA:
                        *default_char_ptr = 0x05;
                        break;

                    case 0x00E7:
                        *default_char_ptr = 0x09;
                        break;    

                    case 0x00D4:
                        *default_char_ptr = 0x0B;
                        break;    

                    case 0x00F4:
                        *default_char_ptr = 0x0C;
                        break;   

                    case 0x00C1:
                        *default_char_ptr = 0x0E;
                        break;

                    case 0x00E1:
                        *default_char_ptr = 0x0F;
                        break;

                    case 0x00CA:
                        *default_char_ptr = 0x1F;
                        break;           

                    case 0x00C0:
                        *default_char_ptr = 0x41;
                        break;       

                    case 0x00CD:
                        *default_char_ptr = 0x49;
                        break;       

                    case 0x00D3:
                        *default_char_ptr = 0x4F;
                        break;           

                    case 0x00DA:
                        *default_char_ptr = 0x55;
                        break;       

                    case 0x00C3:
                        *default_char_ptr = 0x5B;
                        break;       

                    case 0x00D5:
                        *default_char_ptr = 0x5C;
                        break;       

                    case 0x00C2:
                        *default_char_ptr = 0x61;
                        break;           

                    case 0x00ED:
                        *default_char_ptr = 0x69;
                        break;           

                    case 0x00F3:
                        *default_char_ptr = 0x6F;
                        break;       

                    case 0x00FA:
                        *default_char_ptr = 0x75;
                        break;       

                    case 0x00E3:
                        *default_char_ptr = 0x7B;
                        break;       

                    case 0x00F5:
                        *default_char_ptr = 0x7C;
                        break;       

                    case 0x00E2:
                        *default_char_ptr = 0x7F;
                        break;           

                    default:
                        *default_char_ptr = 0x20;
                        result = FALSE;
                        break;
                }

                break;
            }
            
            default:
                *default_char_ptr = 0x20;
                result = FALSE;
                break;
        }
    }

    return (result);
}


#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : init  SMS TTS flag
//	Global resource dependence : 
//  Author: barry peng
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitTTSFlag(void)
{
    MMISMS_GetTTSInfo();
}
/*****************************************************************************/
// 	Description : get the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_GetTTSInfo( void )
{
    BOOLEAN					is_tts_open = FALSE;    
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get TTS info from NV
    MMINV_READ(MMINV_SMS_TTS_FLAG,&is_tts_open,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_tts_open =FALSE; //默认关闭自动朗读
        MMINV_WRITE(MMINV_SMS_TTS_FLAG,&is_tts_open);
    }
    
    return (is_tts_open);
}
/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetTTSInfo(
                              MMI_CTRL_ID_T	    ctrl_id
                              )
{
    uint16      cur_selection = 0;
    BOOLEAN     is_tts_open = FALSE;
    
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
    // set the global virable
    if ( 0 == cur_selection)
    {
        is_tts_open = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPISET_SetTTSInfo cur_selection = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_SET_1590_112_2_18_2_53_29_464,(uint8*)"d",cur_selection);
        is_tts_open = FALSE;
    }
    
    //write the status of TTS into NV
    MMINV_WRITE(MMINV_SMS_TTS_FLAG,&is_tts_open);
}

/*****************************************************************************/
// 	Description : init  SMS TTS flag
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_InitTTSVol(void)
{
    uint8 	tts_vol = MMISET_VOL_SIX;
    MN_RETURN_RESULT_E	nv_value = MN_RETURN_SUCCESS;
    
    MMINV_READ(MMINV_SMS_TTS_VOL, &tts_vol, nv_value);
    
    if (MN_RETURN_SUCCESS != nv_value)
    {
        tts_vol = MMISET_VOL_SIX;
        MMINV_WRITE(MMINV_SMS_TTS_VOL, &tts_vol);
    }
}
/*****************************************************************************/
// 	Description : get the info of TTS setting status
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMISMS_GetTTSVol( void )
{
    uint8 	tts_vol = MMISET_VOL_SIX;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get TTS info from NV
    MMINV_READ(MMINV_SMS_TTS_VOL,&tts_vol,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        tts_vol = MMISET_VOL_SIX;
        MMINV_WRITE(MMINV_SMS_TTS_VOL,&tts_vol);
    }
    
    return (tts_vol);
}
/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetTTSVol(
                             uint8 vol
                             )
{   
    if(vol <= MMISET_VOL_MAX)
    {
        MMINV_WRITE(MMINV_SMS_TTS_VOL,&vol);
    }
}
/*****************************************************************************/
// 	Description : add set tts list item
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_AppendTTSListItem(
                                     MMI_CTRL_ID_T	    ctrl_id
                                     )
{
    uint16                      cur_selection = 0;
    BOOLEAN                     is_tts_open = FALSE;
    
    MMIAPISET_AppendListItemByTextId( TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );	
    //get current tts setting info
    is_tts_open = MMISMS_GetTTSInfo();
    //the status is open
    if (is_tts_open)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);    
}
#endif

#ifdef MMI_SMS_MULTI_PARAM_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC MMISMS_SET_SIMMODE_T MMISMS_Get_Setmode(void)
{
    return g_mmisms_global.set_mode;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_Set_Setmode(MMISMS_SET_SIMMODE_T *set_mode_ptr)
{
    if (PNULL == set_mode_ptr)
    {
        return;
    }
    SCI_MEMCPY((void *) (&g_mmisms_global.set_mode), (const void *)set_mode_ptr, sizeof(MMISMS_SET_SIMMODE_T))
}

/*****************************************************************************/
//  Description : to save the sim MN
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMS_SaveSIMModeToMN(void)
{
    MN_SMS_PARAM_T *sms_param_ptr = PNULL;  

    sms_param_ptr = SCI_ALLOCAZ(sizeof(MN_SMS_PARAM_T));

    if (PNULL == sms_param_ptr)
    {
        return;
    } 

    MNSMS_GetSmsParamEx(
                        g_mmisms_global.set_mode.dual_sys,
                        g_mmisms_global.set_mode.mode + 1,
                        sms_param_ptr
                       );

    sms_param_ptr->alpha_id_present = g_mmisms_global.set_mode.alpha_id_present;
    if(g_mmisms_global.set_mode.alpha_id_present)
    {
        SCI_MEMCPY(&sms_param_ptr->alpha_id_t,&g_mmisms_global.set_mode.alpha_id_t,sizeof(MN_SMS_SUPPORT_LANG_T));
    }
    else
    {
        SCI_MEMSET(&sms_param_ptr->alpha_id_t,0x00,sizeof(MN_SMS_SUPPORT_LANG_T));
    }

    sms_param_ptr->tp_sc_addr_present = g_mmisms_global.set_mode.tp_sc_addr_present;
    if(g_mmisms_global.set_mode.tp_sc_addr_present)
    {
        SCI_MEMCPY(&sms_param_ptr->tp_sc_addr_t,&g_mmisms_global.set_mode.tp_sc_addr_t,sizeof(MN_CALLED_NUMBER_T));
    }
    else
    {
        SCI_MEMSET(&sms_param_ptr->tp_sc_addr_t,0x00,sizeof(MN_CALLED_NUMBER_T));
    }

    sms_param_ptr->tp_pid_present = g_mmisms_global.set_mode.tp_pid_present;
    if(g_mmisms_global.set_mode.tp_pid_present)
    {
        sms_param_ptr->tp_pid = g_mmisms_global.set_mode.tp_pid;
    }
    else
    {
        sms_param_ptr->tp_pid = MN_SMS_PID_DEFAULT_TYPE;
    }

    sms_param_ptr->tp_vp_present = g_mmisms_global.set_mode.tp_vp_present;
    if(g_mmisms_global.set_mode.tp_vp_present)
    {
        sms_param_ptr->vp_second = g_mmisms_global.set_mode.vp_second;
    }
    else
    {
        sms_param_ptr->vp_second = MMISMS_VALID_PERIOD_MAX;
    }

    MNSMS_SetSmsParamEx(
                        g_mmisms_global.set_mode.dual_sys,
                        g_mmisms_global.set_mode.mode + 1,
                        sms_param_ptr);
    
    SCI_FREE(sms_param_ptr);	
}

#endif
/*****************************************************************************/
// 	Description : correct time stamp
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMISMS_CorrectTimeStamp(MN_SMS_TIME_STAMP_T *time_stamp_ptr, MMISMS_TYPE_E type)
{
    if (PNULL != time_stamp_ptr)
    {
        uint16 year = 0;
        uint8 day_max = 0;

        //correct year
        if (time_stamp_ptr->year > 99)//年0-99
        {
            time_stamp_ptr->year = 0;
        }

        //correct month
        if (0 == time_stamp_ptr->month || time_stamp_ptr->month > 12)//月1-12
        {
            time_stamp_ptr->month = 1;
        }

        //correct day
        if (1 == time_stamp_ptr->month
            || 3 == time_stamp_ptr->month
            || 5 == time_stamp_ptr->month
            || 7 == time_stamp_ptr->month
            || 8 == time_stamp_ptr->month
            || 10 == time_stamp_ptr->month
            || 12 == time_stamp_ptr->month)
        {
            day_max = 31;
        }
        else if (2 == time_stamp_ptr->month)
        {
            if (MMISMS_MT_TYPE == type)
            {
                year = time_stamp_ptr->year + MMISMS_MT_OFFSERT_YEAR;
            }
            else
            {
                year = time_stamp_ptr->year + MMISMS_OFFSERT_YEAR;
            }
        
            if (MMIAPICOM_IsLeapYear(year))//闰年
            {
                day_max = 29;
            }
            else
            {
                day_max = 28;
            }
        }
        else
        {
            day_max = 30;
        }

        if (0 == time_stamp_ptr->day || time_stamp_ptr->day > day_max)//日1-最大天数
        {
            time_stamp_ptr->day = 1;
        }
     
        //correct hour
        if (time_stamp_ptr->hour > 23)//小时0-23
        {
            time_stamp_ptr->hour = 0;
        }

        //correct minute
        if (time_stamp_ptr->minute > 59)//分钟0-59
        {
            time_stamp_ptr->minute = 0;
        }

        //correct second
        if (time_stamp_ptr->second > 59)//秒0-59
        {
            time_stamp_ptr->second = 0;
        }
    }
}

#ifdef MMI_SMS_ALPHABET_SETTING_SUPPORT
/*****************************************************************************/
//  Description : Convert Char to Alphabet
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC wchar MMISMS_ConvertChar2Alphbet(wchar ch)
{
    wchar convert_ch = 0;

    if ( MMIAPICOM_IsUcs2Char(ch) 
        && ( !MMIAPICOM_IsDefaultChar(ch) &&
        !MMIAPICOM_IsDefaultExtChar(ch)))
    {
        switch (ch)
        {
        case 0xE0://in default alphabet table
        case 0xE1:
        case 0xE2:
        case 0xE3:
        case 0xE4://in default alphabet table
        case 0xE5://in default alphabet table
        case 0xE6://in default alphabet table
        case 0x101:
        case 0x1CE:
            convert_ch = 'a';
            break;

        case 0xE7:
            convert_ch = 'c';
            break;

        case 0xE8://in default alphabet table
        case 0xE9://in default alphabet table
        case 0xEA:
        case 0xEB:
        case 0x113:
        case 0x11B:
            convert_ch = 'e';
            break;

        case 0xEC://in default alphabet table
        case 0xED:
        case 0xEE:
        case 0xEF:
        case 0x12B:
        case 0x1D0:
            convert_ch = 'i';
            break;

        case 0xF1://in default alphabet table
        case 0x144:
        case 0x148:
            convert_ch = 'n';
            break;

        case 0xF2://in default alphabet table
        case 0xF3:
        case 0xF4:
        case 0xF5:
        case 0xF6://in default alphabet table
        case 0x14D:
        case 0x1D2:
            convert_ch = 'o';
            break;

        case 0x161:
            convert_ch = 's';
            break;

        case 0xF9://in default alphabet table
        case 0xFA:
        case 0xFB:
        case 0xFC://in default alphabet table
        case 0x16B:
        case 0x1D4:
        case 0x1D6:
        case 0x1D8:
        case 0x1DA:
        case 0x1DC:
            convert_ch = 'u';
            break;

        case 0xFD:
        case 0xFF:
            convert_ch = 'y';
            break;

        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xC3:
        case 0xC4://in default alphabet table
        case 0xC5://in default alphabet table
        case 0xC6://in default alphabet table
            convert_ch = 'A';
            break;

        case 0xC7://in default alphabet table
            convert_ch = 'C';
            break;

        case 0xC8:
        case 0xC9://in default alphabet table
        case 0xCA:
        case 0xCB:
            convert_ch = 'E';
            break;

        case 0xCC:
        case 0xCD:
        case 0xCE:
        case 0xCF:
            convert_ch = 'I';
            break;

        case 0xD1://in default alphabet table
            convert_ch = 'N';
            break;

        case 0xD2:
        case 0xD3:
        case 0xD4:
        case 0xD5:
        case 0xD6://in default alphabet table
            convert_ch = 'O';
            break;

        case 0x160:
            convert_ch = 'S';
            break;

        case 0xD9:
        case 0xDA:
        case 0xDB:
        case 0xDC://in default alphabet table
            convert_ch = 'U';
            break;

        case 0xDD:
        case 0x178:
            convert_ch = 'Y';
            break;

        default:
            convert_ch = 0x20;
            break;
        }
    }
    else
    {
        convert_ch = ch;
    }

    return convert_ch;
}
#endif

/*****************************************************************************/
//     Description : to handle the signal APP_MN_WRITE_SMS_PARAM_CNF
//    Global resource dependence : none
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_HandleWriteSMSParamCnf(DPARAM param)
{
    //MnWriteSmsParamCnfS *sig_ptr = (MnWriteSmsParamCnfS *)param;

    MMK_CloseWin(MMISMS_WRITE_SMS_PARAM_WAITING_WIN_ID);
}


#ifdef _ATEST_
/*****************************************************************************/
//  Description : Decode R8 Language to judge language
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC MMISMS_R8_LANGUAGE_E Atest_MMISMS_R8_DecodeContentForLanguage(
                                    wchar *content,
                                    uint16 len,
                                    BOOLEAN *is_single_shift)
{
    MMISMS_R8_LANGUAGE_E language = MMISMS_R8_LANGUAGE_NONE;
    int16 i = 0,j = 0;
    for(i=0;i<len;i++)
    {
        if((MMISMS_IsR8SingleByte(content[i]) || (content[i] == 0x00E7))
            && (MMISMS_R8_LANGUAGE_NONE == language)
            && !MMISMS_R8_IsDefaultPortuguese(content[i])
            && !MMISMS_R8_IsDefaultSpanish(content[i]))
        {
            language = MMISMS_R8_LANGUAGE_LATIN;
        }
        if(content[i] == 0x00E7)
        {
            *is_single_shift = TRUE;
        }
        else if(MMISMS_R8_IsDefaultPortuguese(content[i]) )
        {
            if((MMISMS_R8_LANGUAGE_NONE == language)
                || (MMISMS_R8_LANGUAGE_LATIN == language)
                || (MMISMS_R8_LANGUAGE_PORTUGUESE == language)
                || (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language))
            {
                language = MMISMS_R8_LANGUAGE_PORTUGUESE;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }    
        }
        else if(MMISMS_R8_IsDefaultSpanish(content[i]) )
        {
            if((MMISMS_R8_LANGUAGE_NONE == language)
                || (MMISMS_R8_LANGUAGE_LATIN == language)
                || (MMISMS_R8_LANGUAGE_SPANISH == language)
                || (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language)
                || (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }    
        }
        else if(MMISMS_R8_IsTurkish(content[i]))
        {
            if((MMISMS_R8_LANGUAGE_NONE == language) || 
                (MMISMS_R8_LANGUAGE_TURKISH == language) ||
                (MMISMS_R8_LANGUAGE_LATIN == language) ||
                (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language))
            {
                language = MMISMS_R8_LANGUAGE_TURKISH;
                *is_single_shift = TRUE;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }
        }
        else if(MMISMS_R8_IsPortuguese(content[i]))
        {
            if((MMISMS_R8_LANGUAGE_NONE == language) || 
                (MMISMS_R8_LANGUAGE_PORTUGUESE == language) ||
                (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language) ||
                (MMISMS_R8_LANGUAGE_LATIN == language))
            {
                language = MMISMS_R8_LANGUAGE_PORTUGUESE;
                *is_single_shift = TRUE;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }
        }
        else if(MMISMS_R8_IsSpanishOrPortuguese(content[i]))
        {
            if((MMISMS_R8_LANGUAGE_NONE == language) || 
                (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language) ||
                (MMISMS_R8_LANGUAGE_LATIN == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE;
                *is_single_shift = TRUE;
            }
            else if((MMISMS_R8_LANGUAGE_SPANISH == language) ||
                    (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH;
            }
            else if(MMISMS_R8_LANGUAGE_PORTUGUESE == language)
            {
                language = MMISMS_R8_LANGUAGE_PORTUGUESE;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }
        }
        else if(MMISMS_R8_IsSpanishOrTurkish(content[i]))
        {
            if((MMISMS_R8_LANGUAGE_NONE == language) || 
                (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language) ||
                (MMISMS_R8_LANGUAGE_LATIN == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH;
            }
            else if((MMISMS_R8_LANGUAGE_SPANISH == language) ||
                    (MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language))
            {
                language = MMISMS_R8_LANGUAGE_SPANISH;
            }
            else if(MMISMS_R8_LANGUAGE_TURKISH == language)
            {
                language = MMISMS_R8_LANGUAGE_TURKISH;
            }
            else
            {
                language = MMISMS_R8_LANGUAGE_UNICODE;
                return language;
            }
        }
        else if((content[i] == 0xA3) || (content[i] == 0xA5))
        {
            language = MMISMS_R8_LANGUAGE_UNICODE;
            return language;            
        }
    }
    if(MMISMS_R8_LANGUAGE_SPANISH_OR_PORTUGUESE == language)
    {
        language = MMISMS_R8_LANGUAGE_SPANISH;
    }
    else if((MMISMS_R8_LANGUAGE_LATIN == language) ||
               (MMISMS_R8_LANGUAGE_SPANISH_OR_TURKISH == language))
    {
        language = MMISMS_R8_LANGUAGE_TURKISH;
    }
    for(i=0;i<len;i++)
	{
		if (MMITHEME_IsR82ByteChar(content[i]))
		{
			*is_single_shift = TRUE;
			break;
		}
	}
    for(i=0;i<len;i++)
    {
            switch(language)
            {
                case MMISMS_R8_LANGUAGE_TURKISH:
                    for(j=0;j<128;j++)
                    {
                        if((content[i] == default_to_turkish_table[j]) ||
                            (content[i] == default_to_turkish_extension_table[j]))
                        {
                            break;
                        }
                        if(j == 127)
                        {
                            language = MMISMS_R8_LANGUAGE_UNICODE;
                            return language;
                        }
                    }
                    break;
                case MMISMS_R8_LANGUAGE_SPANISH:
                    for(j=0;j<128;j++)
                    {
                        if((content[i] == sms_default_to_ascii_table[j]) ||
                            (content[i] == default_to_spanish_extension_table[j]))
                        {
                            break;
                        }
                        if(j == 127)
                        {
                            language = MMISMS_R8_LANGUAGE_UNICODE;
                            return language;
                        }
                    }
                    break;
                case MMISMS_R8_LANGUAGE_PORTUGUESE:
                    for(j=0;j<128;j++)
                    {
                        if((content[i] == default_to_portuguese_table[j]) ||
                            (content[i] == default_to_portuguese_extension_table[j]))
                        {
                            break;
                        }
                        if(j == 127)
                        {
                            language = MMISMS_R8_LANGUAGE_UNICODE;
                            return language;
                        }
                    }
                    break;
                default:
                    break;
            }
    }
    return language;
}
#endif

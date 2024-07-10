/*****************************************************************************
** File Name:      ctrledit_count.c                                          *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2009      Jassmine              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrltextedit.h"
#include "mmi_theme.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get add string length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetAddStrLen (
    BOOLEAN                       is_sms,             //in:is sms
    BOOLEAN                       is_pb_encode,       //in:is pb encode
    wchar                         *add_str_ptr,       //in:add string
    uint16                        add_str_len,        //in:add string len
    uint16                        cur_str_len,        //in:current string len
    GUIEDIT_SMS_ENCODE_E			sms_encode,			//in:sms encode type
    GUIEDIT_TEXT_MAX_T            *max_len_ptr,       //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T           *ucs2_ptr,          //in:max and min ucs2
    GUIEDIT_TEXT_SPCHAR_NUM_T     *spchar_num_ptr,    //in:special char number
    GUIEDIT_TEXT_LANGCHAR_NUM_T   *langchar_num_ptr   //in:language private char num
);

/*****************************************************************************/
//  Description : get edit max len by special char number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetEditMaxLen (
    BOOLEAN                      is_pb_encode,       //in:is pb encode
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,       //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T          *ucs2_ptr,          //in:max and min ucs2
    GUIEDIT_TEXT_SPCHAR_NUM_T    *spchar_num_ptr,    //in:special char number
    GUIEDIT_TEXT_LANGCHAR_NUM_T  *langchar_num_ptr   //in:language private char num
);

/*****************************************************************************/
//  Description : is need convert ucs2 char for add string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsConvertUcs2ForAdd (
    BOOLEAN                       is_sms,                 //in:
    wchar                         *add_str_ptr,           //in:
    uint16                        add_str_len,            //in:
    GUIEDIT_SMS_ENCODE_E			sms_encode,				//in:
    GUIEDIT_TEXT_LANGCHAR_NUM_T   *cur_langchar_num_ptr   //in:
);

/*****************************************************************************/
//  Description : get add no ucs2 len, when current string no ucs2
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,获得添加无ucs2的长度
/*****************************************************************************/
LOCAL uint16 GetAddNoUcs2LenByNoUcs2 (
    BOOLEAN                    is_sms,         //in:
    wchar                      *add_str_ptr,   //in:
    uint16                     add_str_len,    //in:
    uint16                     cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E		sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T         *max_len_ptr,   //in:
    GUIEDIT_TEXT_SPCHAR_NUM_T  *spchar_num_ptr //in:
);

/*****************************************************************************/
//  Description : get add no ucs2,no r8 2byte len, when current string no ucs2, no r8 2byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,获得添加无ucs2,无r8 2byte的长度
/*****************************************************************************/
LOCAL uint16 GetAddNoR82ByteLenByNoR82Byte (
    BOOLEAN                      is_sms,         //in:
    wchar                        *add_str_ptr,   //in:
    uint16                       add_str_len,    //in:
    uint16                       cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E			sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,   //in:
    GUIEDIT_TEXT_SPCHAR_NUM_T    *spchar_num_ptr //in:
);

/*****************************************************************************/
//  Description : get add no ucs2,no r8 2byte,1byte len, when current string no ucs2, no r8 2byte,,1byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,无r8 1byte，获得无r8 1byte的长度
/*****************************************************************************/
LOCAL uint16 GetAddNoR81ByteLenByNoR81Byte (
    BOOLEAN              is_sms,         //in:
    wchar                *add_str_ptr,   //in:
    uint16               add_str_len,    //in:
    uint16               cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E	sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T   *max_len_ptr    //in:
);

/*****************************************************************************/
//  Description : get add default len, when current string no ucs2,no r8 2byte,no r8 1byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,无r8 1byte,获得添加有default的长度
/*****************************************************************************/
LOCAL uint16 GetAddDefaultLenByNoR81Byte (
    wchar					*add_str_ptr,   //in:
    uint16					add_str_len,    //in:
    uint16					add_default_len,//in:
    uint16					cur_str_len,    //in:
    uint16					max_len_ascii,  //in:
    GUIEDIT_SMS_ENCODE_E	sms_encode		//in:sms encode type
);

/*****************************************************************************/
//  Description : get add r8 1byte len, when current string no ucs2,no r8 2byte,no r8 1byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,无r8 1byte,获得添加有r8 1byte的长度
/*****************************************************************************/
LOCAL uint16 GetAddR81ByteLenByNoR81Byte (
    BOOLEAN                is_sms,         //in:
    wchar                  *add_str_ptr,   //in:
    uint16                 add_str_len,    //in:
    uint16                 add_default,    //in:
    uint16                 cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E	sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T     *max_len_ptr    //in:
);

/*****************************************************************************/
//  Description : get add default len, when current string no ucs2,no r8 2byte,has r8 1byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,有r8 1byte,获得添加有default的长度
/*****************************************************************************/
LOCAL uint16 GetAddDefaultLenByNoR82Byte (
    wchar					*add_str_ptr,		//in:
    uint16					add_str_len,		//in:
    uint16					add_default_len,	//in:
    uint16					cur_str_len,		//in:
    uint16					max_len_r8_1byte,	//in:
    GUIEDIT_SMS_ENCODE_E	sms_encode			//in:sms encode type
);

/*****************************************************************************/
//  Description : get add r8 2byte len, when current string no ucs2,no r8 2byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,获得添加有r8 2byte的长度
/*****************************************************************************/
LOCAL uint16 GetAddR82ByteLenByNoR82Byte (
    BOOLEAN                    is_sms,         //in:
    wchar                      *add_str_ptr,   //in:
    uint16                     add_str_len,    //in:
    uint16                     cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E		sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T         *max_len_ptr,   //in:
    GUIEDIT_TEXT_SPCHAR_NUM_T  *spchar_num_ptr //in:
);

/*****************************************************************************/
//  Description : get add r8 2byte or default len, when current string no ucs2,has r8 2byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,有r8 2byte,获得添加无ucs2的长度
/*****************************************************************************/
LOCAL uint16 GetAddNoUcs2LenByR82Byte (
    wchar					*add_str_ptr,		//in:
    uint16				add_str_len,		//in:
    uint16				cur_str_len,		//in:
    uint16				max_len_r8_2byte,	//in:
    GUIEDIT_SMS_ENCODE_E	sms_encode			//in:sms encode type
);

/*****************************************************************************/
//  Description : get add ucs2 len, when current string no ucs2
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,获得添加有ucs2的长度
/*****************************************************************************/
LOCAL uint16 GetAddUcs2LenByNoUcs2 (
    BOOLEAN                      is_sms,             //in:
    wchar                        *add_str_ptr,       //in:
    uint16                       add_str_len,        //in:
    uint16                       cur_str_len,        //in:
    GUIEDIT_SMS_ENCODE_E			sms_encode,			//in:sms encode type
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,       //in:
    GUIEDIT_TEXT_SPCHAR_NUM_T    *spchar_num_ptr,    //in:
    GUIEDIT_TEXT_LANGCHAR_NUM_T  *langchar_num_ptr   //in:
);

/*****************************************************************************/
//  Description : get the first ucs2 of add string
//  Global resource dependence :
//  Author: Jassmine
//  Note: 获得添加需要转换为ucs2的第一个字符长度
/*****************************************************************************/
LOCAL uint16 GetFirstConvertUcs2Char (
    BOOLEAN                        is_sms,             //in:
    wchar                          *add_str_ptr,       //in:
    uint16                         add_str_len,        //in:
    GUIEDIT_SMS_ENCODE_E			sms_encode,			//in:sms encode type
    GUIEDIT_TEXT_LANGCHAR_NUM_T    *langchar_num_ptr   //in:
);

/*****************************************************************************/
//  Description : get add string len, when support pb encode
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetAddStrLenForPb (
    wchar                *add_str_ptr,   //in:add string
    uint16               add_str_len,    //in:add string len
    uint16               cur_str_len,    //in:current string len
    uint16               default_ext_num,//in:default char number
    GUIEDIT_TEXT_MAX_T   *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T  *ucs2_ptr       //in:max and min ucs2
);

/*****************************************************************************/
//  Description : get encode type after add string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMITHEME_PB_ENCODE_TYPE_E GetEncodeTypeAfterAdd (
    wchar                 *add_str_ptr,   //in:add string
    uint16                add_str_len,    //in:add string len
    GUIEDIT_TEXT_UCS2_T   *ucs2_ptr       //in:max and min ucs2
);

/*****************************************************************************/
//  Description : get add 80 len, when current string no 80
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,获得添加有80编码的长度
/*****************************************************************************/
LOCAL uint16 GetAdd80LenByNo80 (
    wchar                        *add_str_ptr,   //in:add string
    uint16                       add_str_len,    //in:add string len
    uint16                       cur_str_len,    //in:current string len
    uint16                       default_ext_num,//in:default char number
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T          *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E    cur_encode_type //in:pointed encode type
);

/*****************************************************************************/
//  Description : get the first pointed type of add string
//  Global resource dependence :
//  Author: Jassmine
//  Note: 获得添加需要转换为80,81,82的第一个字符长度
/*****************************************************************************/
LOCAL uint16 GetFirstConvertTypeChar (
    wchar                      *add_str_ptr,   //in:add string
    uint16                     add_str_len,    //in:add string len
    GUIEDIT_TEXT_UCS2_T        *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E  encode_type     //in:pointed encode type
);

/*****************************************************************************/
//  Description : get add no 80 len, when current string no 80
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,获得添加无80编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo80LenByNo80 (
    wchar                      *add_str_ptr,   //in:add string
    uint16                     add_str_len,    //in:add string len
    uint16                     cur_str_len,    //in:current string len
    uint16                     default_ext_num,//in:default char number
    GUIEDIT_TEXT_MAX_T         *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T        *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E  cur_encode_type //in:current encode type
);

/*****************************************************************************/
//  Description : get add no 80 len, when current string 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,有82编码,获得添加无80编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo80LenBy82 (
    wchar                *add_str_ptr,   //in:add string
    uint16               add_str_len,    //in:add string len
    uint16               add_default,    //in:add default string len
    uint16               cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T   *max_len_ptr    //in:ucs2,r8,ascii max len
);

/*****************************************************************************/
//  Description : get add no 81 len, when current string 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,获得添加有82编码的长度
/*****************************************************************************/
LOCAL uint16 GetAdd82LenByNo82 (
    wchar                        *add_str_ptr,   //in:add string
    uint16                       add_str_len,    //in:add string len
    uint16                       add_default,    //in:add default string len
    uint16                       cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T          *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E    cur_encode_type //in:pointed encode type
);

/*****************************************************************************/
//  Description : get add no 82 len, when current string no 82
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,获得添加无82编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo82LenByNo82 (
    wchar                      *add_str_ptr,   //in:add string
    uint16                     add_str_len,    //in:add string len
    uint16                     cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T         *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T        *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E  cur_encode_type //in:current encode type
);

/*****************************************************************************/
//  Description : get add no 82 len, when current string 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,获得添加有81编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo82LenBy81 (
    wchar                *add_str_ptr,   //in:add string
    uint16               add_str_len,    //in:add string len
    uint16               cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T   *max_len_ptr    //in:ucs2,r8,ascii max len
);

/*****************************************************************************/
//  Description : get add no 81 len, when current string 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,无81编码,获得添加有81编码的长度
/*****************************************************************************/
LOCAL uint16 GetAdd81LenByNo81 (
    wchar                *add_str_ptr,   //in:add string
    uint16               add_str_len,    //in:add string len
    uint16               add_default,    //in:add default string len
    uint16               cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T   *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T  *ucs2_ptr       //in:max and min ucs2
);

/*****************************************************************************/
//  Description : get add no 81 len, when current string no 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,无81编码,获得添加无81编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo81LenByNo81 (
    wchar              *add_str_ptr,   //in:add string
    uint16             add_str_len,    //in:add string len
    uint16             cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T *max_len_ptr    //in:ucs2,r8,ascii max len
);

/*****************************************************************************/
//  Description : adjust max and min ucs2
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustMaxMinUcs2 (
    BOOLEAN             is_add,         //in:is add or delete
    wchar               *str_ptr,       //in:
    uint16              str_len,        //in:
    uint16              del_start_pos,  //in:only for delete
    wchar               *all_str_ptr,   //in:only for delete
    uint16              all_str_len,    //in:only for delete
    GUIEDIT_TEXT_UCS2_T *ucs2_ptr       //in/out:max and min ucs2
);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get commit and underline length for ucs2 != ascii
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TEXTEDIT_GetAddLenForDifferent (
    CTRLTEXTEDIT_OBJ_T *textedit_ctrl_ptr, //in:
    GUIEDIT_ADD_STR_T  *add_str_ptr    //in/out:
)
{
    BOOLEAN                     result = FALSE;
    uint16                      cur_str_len = 0;
    uint16                      commit_len = 0;
    uint16                      highlight_len = 0;
    uint16                      highlight_min_pos = 0;
    uint16                      underline_len = 0;
    GUIEDIT_TEXT_UCS2_T         ucs2_value = {0};
    GUIEDIT_TEXT_SPCHAR_NUM_T   highlight_spchar_num = {0};
    GUIEDIT_TEXT_SPCHAR_NUM_T   spchar_num = {0};
    GUIEDIT_TEXT_SPCHAR_NUM_T   commit_spchar_num = {0};
    GUIEDIT_TEXT_LANGCHAR_NUM_T highlight_langchar_num = {0};
    GUIEDIT_TEXT_LANGCHAR_NUM_T langchar_num = {0};
    GUIEDIT_TEXT_LANGCHAR_NUM_T commit_langchar_num = {0};
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;

    if (
        //(GUIEDIT_TYPE_TEXT == edit_ctrl_ptr->type) &&
        (textedit_ctrl_ptr->max_len.ucs2 != textedit_ctrl_ptr->max_len.ascii))
    {
        //get highlight len
        highlight_len = BASEEDIT_GetHighlightLen (baseedit_ctrl_ptr);

        //get current string len
        cur_str_len = (uint16) (baseedit_ctrl_ptr->str_len - highlight_len);

        //get highlight min pos
        highlight_min_pos = BASEEDIT_GetHighlightMinPos (baseedit_ctrl_ptr);

        //get highlight ucs2,r8 2byte,r8 1byte，default ext number
        MMITHEME_CountSpecialChar (textedit_ctrl_ptr->is_sms,
                                   baseedit_ctrl_ptr->str_ptr + highlight_min_pos,
                                   highlight_len,
                                   textedit_ctrl_ptr->sms_encode,
                                   &highlight_spchar_num);

        //get current ucs2,r8 2byte,r8 1byte,default ext number
        spchar_num.ucs2 = (uint16) (textedit_ctrl_ptr->char_num.ucs2 - highlight_spchar_num.ucs2);

        if (textedit_ctrl_ptr->is_sms)
        {
            SCI_ASSERT( /*assert verified*/
                textedit_ctrl_ptr->char_num.default_ext >= highlight_spchar_num.default_ext
                && textedit_ctrl_ptr->char_num.r8_2byte >= highlight_spchar_num.r8_2byte
                && textedit_ctrl_ptr->char_num.r8_1byte >= highlight_spchar_num.r8_1byte);

            spchar_num.default_ext = (uint16) (textedit_ctrl_ptr->char_num.default_ext - highlight_spchar_num.default_ext);
            spchar_num.r8_2byte    = (uint16) (textedit_ctrl_ptr->char_num.r8_2byte - highlight_spchar_num.r8_2byte);
            spchar_num.r8_1byte    = (uint16) (textedit_ctrl_ptr->char_num.r8_1byte - highlight_spchar_num.r8_1byte);

            //get highlight language private,shared char num
            MMITHEME_CountLangChar (baseedit_ctrl_ptr->str_ptr + highlight_min_pos,
                                    highlight_len,
                                    &highlight_langchar_num);

            //set current language private,shared char num
            SCI_ASSERT( /*assert verified*/
                textedit_ctrl_ptr->lan_num.tur >= highlight_langchar_num.tur
                && textedit_ctrl_ptr->lan_num.esp >= highlight_langchar_num.esp
                && textedit_ctrl_ptr->lan_num.por >= highlight_langchar_num.por
                && textedit_ctrl_ptr->lan_num.tur_esp >= highlight_langchar_num.tur_esp
                && textedit_ctrl_ptr->lan_num.esp_por >= highlight_langchar_num.esp_por);

            langchar_num.tur     = (uint16) (textedit_ctrl_ptr->lan_num.tur - highlight_langchar_num.tur);
            langchar_num.esp     = (uint16) (textedit_ctrl_ptr->lan_num.esp - highlight_langchar_num.esp);
            langchar_num.por     = (uint16) (textedit_ctrl_ptr->lan_num.por - highlight_langchar_num.por);
            langchar_num.tur_esp = (uint16) (textedit_ctrl_ptr->lan_num.tur_esp - highlight_langchar_num.tur_esp);
            langchar_num.esp_por = (uint16) (textedit_ctrl_ptr->lan_num.esp_por - highlight_langchar_num.esp_por);
        }
        else if (textedit_ctrl_ptr->is_pb_encode)
        {
            SCI_ASSERT(textedit_ctrl_ptr->char_num.default_ext >= highlight_spchar_num.default_ext);    /*assert verified*/
            spchar_num.default_ext = (uint16) (textedit_ctrl_ptr->char_num.default_ext - highlight_spchar_num.default_ext);
        }

        //init max and min ucs2
        ucs2_value = textedit_ctrl_ptr->ucs2;

        //adjust max and min ucs2
        if (textedit_ctrl_ptr->is_pb_encode)
        {
            AdjustMaxMinUcs2 (FALSE,
                              baseedit_ctrl_ptr->str_ptr + highlight_min_pos,
                              highlight_len,
                              highlight_min_pos,
                              baseedit_ctrl_ptr->str_ptr,
                              baseedit_ctrl_ptr->str_len,
                              &ucs2_value);
        }

        //get add commit string length
        commit_len = GetAddStrLen (textedit_ctrl_ptr->is_sms,
                                   textedit_ctrl_ptr->is_pb_encode,
                                   add_str_ptr->commit_ptr,
                                   add_str_ptr->commit_len,
                                   cur_str_len,
                                   textedit_ctrl_ptr->sms_encode,
                                   &textedit_ctrl_ptr->max_len,
                                   &ucs2_value,
                                   &spchar_num,
                                   &langchar_num);

        //set commit str len
        add_str_ptr->commit_len = commit_len;

        if (commit_len == add_str_ptr->commit_len)
        {
            //get current string len
            cur_str_len = (uint16) (cur_str_len + commit_len);

            //get commit ucs2,r8 2byte,r8 1byte，default ext number
            MMITHEME_CountSpecialChar (textedit_ctrl_ptr->is_sms,
                                       add_str_ptr->commit_ptr,
                                       commit_len,
                                       textedit_ctrl_ptr->sms_encode,
                                       &commit_spchar_num);

            //get current ucs2,r8 2byte,r8 1byte,default ext number
            spchar_num.ucs2 = (uint16) (spchar_num.ucs2 + commit_spchar_num.ucs2);

            if (textedit_ctrl_ptr->is_sms)
            {
                spchar_num.default_ext = (uint16) (spchar_num.default_ext + commit_spchar_num.default_ext);
                spchar_num.r8_2byte    = (uint16) (spchar_num.r8_2byte + commit_spchar_num.r8_2byte);
                spchar_num.r8_1byte    = (uint16) (spchar_num.r8_1byte + commit_spchar_num.r8_1byte);

                //get highlight language private,shared char num
                MMITHEME_CountLangChar (baseedit_ctrl_ptr->str_ptr + highlight_min_pos,
                                        highlight_len,
                                        &commit_langchar_num);

                //set current language private,shared char num
                langchar_num.tur     = (uint16) (textedit_ctrl_ptr->lan_num.tur + commit_langchar_num.tur);
                langchar_num.esp     = (uint16) (textedit_ctrl_ptr->lan_num.esp + commit_langchar_num.esp);
                langchar_num.por     = (uint16) (textedit_ctrl_ptr->lan_num.por + commit_langchar_num.por);
                langchar_num.tur_esp = (uint16) (textedit_ctrl_ptr->lan_num.tur_esp + commit_langchar_num.tur_esp);
                langchar_num.esp_por = (uint16) (textedit_ctrl_ptr->lan_num.esp_por + commit_langchar_num.esp_por);
            }
            else if (textedit_ctrl_ptr->is_pb_encode)
            {
                spchar_num.default_ext = (uint16) (spchar_num.default_ext + commit_spchar_num.default_ext);
            }

            //adjust max and min ucs2
            if (textedit_ctrl_ptr->is_pb_encode)
            {
                AdjustMaxMinUcs2 (TRUE,
                                  add_str_ptr->commit_ptr,
                                  commit_len,
                                  0,
                                  baseedit_ctrl_ptr->str_ptr,
                                  baseedit_ctrl_ptr->str_len,
                                  &ucs2_value);
            }

            //get add underline string length
            underline_len = GetAddStrLen (textedit_ctrl_ptr->is_sms,
                                          textedit_ctrl_ptr->is_pb_encode,
                                          add_str_ptr->underline_ptr,
                                          add_str_ptr->underline_len,
                                          cur_str_len,
                                          textedit_ctrl_ptr->sms_encode,
                                          &textedit_ctrl_ptr->max_len,
                                          &ucs2_value,
                                          &spchar_num,
                                          &langchar_num);
        }

        //set underline str len
        add_str_ptr->underline_len = underline_len;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : adjust special char number,include r8 2byte,r8 1byte,default ext.
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void TEXTEDIT_AdjustSpecialCharNum (
    CTRLTEXTEDIT_OBJ_T *textedit_ctrl_ptr,
    BOOLEAN            is_add,         //in:is add or delete
    wchar              *str_ptr,       //in:
    uint16             str_len,        //in:
    uint16             del_start_pos   //in:only for delete
)
{
    GUIEDIT_TEXT_SPCHAR_NUM_T   add_spchar_num = {0};
    GUIEDIT_TEXT_LANGCHAR_NUM_T add_langchar_num = {0};
    CTRLBASEEDIT_OBJ_T* baseedit_ctrl_ptr = (CTRLBASEEDIT_OBJ_T*)textedit_ctrl_ptr;

    if (GUIEDIT_TYPE_TEXT == baseedit_ctrl_ptr->type)
    {
        //get add ucs2,r8 2byte,r8 1byte，default ext number
        MMITHEME_CountSpecialChar (textedit_ctrl_ptr->is_sms,
                                   str_ptr,
                                   str_len,
                                   textedit_ctrl_ptr->sms_encode,
                                   &add_spchar_num);

        if (textedit_ctrl_ptr->is_sms)
        {
            //get add language private,shared char num
            MMITHEME_CountLangChar (str_ptr,
                                    str_len,
                                    &add_langchar_num);
        }

        //set ucs2,r8 2byte,r8 1byte,default ext number
        if (is_add)
        {
            textedit_ctrl_ptr->char_num.ucs2 = (uint16) (textedit_ctrl_ptr->char_num.ucs2 + add_spchar_num.ucs2);

            if (textedit_ctrl_ptr->is_sms)
            {
                textedit_ctrl_ptr->char_num.default_ext = (uint16) (textedit_ctrl_ptr->char_num.default_ext + add_spchar_num.default_ext);
                textedit_ctrl_ptr->char_num.r8_2byte    = (uint16) (textedit_ctrl_ptr->char_num.r8_2byte + add_spchar_num.r8_2byte);
                textedit_ctrl_ptr->char_num.r8_1byte    = (uint16) (textedit_ctrl_ptr->char_num.r8_1byte + add_spchar_num.r8_1byte);

                //set current language private,shared char num
                textedit_ctrl_ptr->lan_num.tur     = (uint16) (textedit_ctrl_ptr->lan_num.tur + add_langchar_num.tur);
                textedit_ctrl_ptr->lan_num.esp     = (uint16) (textedit_ctrl_ptr->lan_num.esp + add_langchar_num.esp);
                textedit_ctrl_ptr->lan_num.por     = (uint16) (textedit_ctrl_ptr->lan_num.por + add_langchar_num.por);
                textedit_ctrl_ptr->lan_num.tur_esp = (uint16) (textedit_ctrl_ptr->lan_num.tur_esp + add_langchar_num.tur_esp);
                textedit_ctrl_ptr->lan_num.esp_por = (uint16) (textedit_ctrl_ptr->lan_num.esp_por + add_langchar_num.esp_por);
            }
            else if (textedit_ctrl_ptr->is_pb_encode)
            {
                textedit_ctrl_ptr->char_num.default_ext = (uint16) (textedit_ctrl_ptr->char_num.default_ext + add_spchar_num.default_ext);
            }
        }
        else
        {
            textedit_ctrl_ptr->char_num.ucs2 = (uint16) (textedit_ctrl_ptr->char_num.ucs2 - add_spchar_num.ucs2);

            if (textedit_ctrl_ptr->is_sms)
            {
                SCI_ASSERT( /*assert verified*/
                    textedit_ctrl_ptr->char_num.default_ext >= add_spchar_num.default_ext
                    && textedit_ctrl_ptr->char_num.r8_2byte >= add_spchar_num.r8_2byte
                    && textedit_ctrl_ptr->char_num.r8_1byte >= add_spchar_num.r8_1byte);

                textedit_ctrl_ptr->char_num.default_ext = (uint16) (textedit_ctrl_ptr->char_num.default_ext - add_spchar_num.default_ext);
                textedit_ctrl_ptr->char_num.r8_2byte    = (uint16) (textedit_ctrl_ptr->char_num.r8_2byte - add_spchar_num.r8_2byte);
                textedit_ctrl_ptr->char_num.r8_1byte    = (uint16) (textedit_ctrl_ptr->char_num.r8_1byte - add_spchar_num.r8_1byte);

                //set current language private,shared char num
                SCI_ASSERT( /*assert verified*/
                    textedit_ctrl_ptr->lan_num.tur >= add_langchar_num.tur
                    && textedit_ctrl_ptr->lan_num.esp >= add_langchar_num.esp
                    && textedit_ctrl_ptr->lan_num.por >= add_langchar_num.por
                    && textedit_ctrl_ptr->lan_num.tur_esp >= add_langchar_num.tur_esp
                    && textedit_ctrl_ptr->lan_num.esp_por >= add_langchar_num.esp_por);
                textedit_ctrl_ptr->lan_num.tur     = (uint16) (textedit_ctrl_ptr->lan_num.tur - add_langchar_num.tur);
                textedit_ctrl_ptr->lan_num.esp     = (uint16) (textedit_ctrl_ptr->lan_num.esp - add_langchar_num.esp);
                textedit_ctrl_ptr->lan_num.por     = (uint16) (textedit_ctrl_ptr->lan_num.por - add_langchar_num.por);
                textedit_ctrl_ptr->lan_num.tur_esp = (uint16) (textedit_ctrl_ptr->lan_num.tur_esp - add_langchar_num.tur_esp);
                textedit_ctrl_ptr->lan_num.esp_por = (uint16) (textedit_ctrl_ptr->lan_num.esp_por - add_langchar_num.esp_por);
            }
            else if (textedit_ctrl_ptr->is_pb_encode)
            {
                SCI_ASSERT(textedit_ctrl_ptr->char_num.default_ext >= add_spchar_num.default_ext);   /*assert verified*/
                textedit_ctrl_ptr->char_num.default_ext = (uint16) (textedit_ctrl_ptr->char_num.default_ext - add_spchar_num.default_ext);
            }
        }

        if (textedit_ctrl_ptr->is_pb_encode)
        {
            //adjust max and min ucs2
            AdjustMaxMinUcs2 (is_add,
                              str_ptr,
                              str_len,
                              del_start_pos,
                              baseedit_ctrl_ptr->str_ptr,
                              baseedit_ctrl_ptr->str_len,
                              &textedit_ctrl_ptr->ucs2);
        }

        //adjust string max len
        baseedit_ctrl_ptr->str_max_len = GetEditMaxLen (textedit_ctrl_ptr->is_pb_encode,
                                         &textedit_ctrl_ptr->max_len,
                                         &textedit_ctrl_ptr->ucs2,
                                         &textedit_ctrl_ptr->char_num,
                                         &textedit_ctrl_ptr->lan_num);
    }
}

/*****************************************************************************/
//  Description : is need convert ucs2 char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TEXTEDIT_IsConvertUcs2 (
    uint16                      ucs2_num,           //in:
    GUIEDIT_TEXT_LANGCHAR_NUM_T *langchar_num_ptr   //in:
)
{
    BOOLEAN     result = FALSE;

    if (0 == ucs2_num)
    {
        //edit include two language private char
        if (0 < langchar_num_ptr->tur)
        {
            if ( (0 < langchar_num_ptr->esp) ||
                    (0 < langchar_num_ptr->por) ||
                    (0 < langchar_num_ptr->esp_por))
            {
                result = TRUE;
            }
        }
        else if (0 < langchar_num_ptr->esp)
        {
            if ( (0 < langchar_num_ptr->tur) ||
                    (0 < langchar_num_ptr->por))
            {
                result = TRUE;
            }
        }
        else if (0 < langchar_num_ptr->por)
        {
            if ( (0 < langchar_num_ptr->tur) ||
                    (0 < langchar_num_ptr->esp) ||
                    (0 < langchar_num_ptr->tur_esp))
            {
                result = TRUE;
            }
        }
    }
    else
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get add string length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetAddStrLen (
    BOOLEAN                       is_sms,             //in:is sms
    BOOLEAN                       is_pb_encode,       //in:is pb encode
    wchar                         *add_str_ptr,       //in:add string
    uint16                        add_str_len,        //in:add string len
    uint16                        cur_str_len,        //in:current string len
    GUIEDIT_SMS_ENCODE_E          sms_encode,			//in:sms encode type
    GUIEDIT_TEXT_MAX_T            *max_len_ptr,       //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T           *ucs2_ptr,          //in:max and min ucs2
    GUIEDIT_TEXT_SPCHAR_NUM_T     *spchar_num_ptr,    //in:special char number
    GUIEDIT_TEXT_LANGCHAR_NUM_T   *langchar_num_ptr   //in:language private char num
)
{
    BOOLEAN     is_convert_ucs2 = FALSE;
    uint16      add_len = 0;
    uint16      max_len = 0;

    //set add len
    add_len = add_str_len;

    if (0 < add_len)
    {
        //get max len
        max_len = GetEditMaxLen (is_pb_encode, max_len_ptr, ucs2_ptr, spchar_num_ptr, langchar_num_ptr);

        //adjust add len
        if ( (cur_str_len + add_len) > max_len)
        {
            add_len = (uint16) (max_len - cur_str_len);
        }

        if (is_pb_encode)
        {
            add_len = GetAddStrLenForPb (add_str_ptr,
                                         add_len,
                                         cur_str_len,
                                         spchar_num_ptr->default_ext,
                                         max_len_ptr,
                                         ucs2_ptr);
        }
        else
        {
            if (!TEXTEDIT_IsConvertUcs2 (spchar_num_ptr->ucs2, langchar_num_ptr))
            {
                //add string is convert ucs2
                is_convert_ucs2 = IsConvertUcs2ForAdd (is_sms,
                                                       add_str_ptr,
                                                       add_len,
                                                       sms_encode,
                                                       langchar_num_ptr);

                //当前无ucs2
                if (is_convert_ucs2)
                {
                    //当前无ucs2,获得添加有ucs2的长度
                    add_len = GetAddUcs2LenByNoUcs2 (is_sms,
                                                     add_str_ptr,
                                                     add_len,
                                                     cur_str_len,
                                                     sms_encode,
                                                     max_len_ptr,
                                                     spchar_num_ptr,
                                                     langchar_num_ptr);
                }
                else
                {
                    //当前无ucs2,获得添加无ucs2的长度
                    add_len = GetAddNoUcs2LenByNoUcs2 (is_sms,
                                                       add_str_ptr,
                                                       add_len,
                                                       cur_str_len,
                                                       sms_encode,
                                                       max_len_ptr,
                                                       spchar_num_ptr);
                }
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get edit max len by special char number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetEditMaxLen (
    BOOLEAN                      is_pb_encode,       //in:is pb encode
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,       //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T          *ucs2_ptr,          //in:max and min ucs2
    GUIEDIT_TEXT_SPCHAR_NUM_T    *spchar_num_ptr,    //in:special char number
    GUIEDIT_TEXT_LANGCHAR_NUM_T  *langchar_num_ptr   //in:language private char num
)
{
    uint16  max_len = 0;

    if (TEXTEDIT_IsConvertUcs2 (spchar_num_ptr->ucs2, langchar_num_ptr))
    {
        if (is_pb_encode)
        {
            max_len = MMITHEME_GetMaxLenForPb (max_len_ptr->ascii,
                                               max_len_ptr->ucs2,
                                               MMITHEME_GetEncodeTypeForPb (ucs2_ptr->max, ucs2_ptr->min));
        }
        else
        {
            max_len = max_len_ptr->ucs2;
        }
    }
    else
    {
        if (0 == spchar_num_ptr->r8_2byte)
        {
            if (0 == spchar_num_ptr->r8_1byte)
            {
                max_len = max_len_ptr->ascii;
            }
            else
            {
                max_len = max_len_ptr->r8_1byte;
            }
        }
        else
        {
#if 1
			max_len = max_len_ptr->r8_2byte;
#else
			if (langchar_num_ptr->por > 0 
                || langchar_num_ptr->tur > 0
                || langchar_num_ptr->tur_esp > 0)
            {
                max_len = max_len_ptr->r8_2byte;  // tur, por, use national shift lock + national single shift table.
            }
            else
            {
                max_len = max_len_ptr->r8_1byte;  // esp , use default 7-bit + spanish single shift table.
            }
#endif
            
        }
    }

    return (max_len);
}

/*****************************************************************************/
//  Description : is need convert ucs2 char for add string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsConvertUcs2ForAdd (
    BOOLEAN                       is_sms,                 //in:
    wchar                         *add_str_ptr,           //in:
    uint16                        add_str_len,            //in:
    GUIEDIT_SMS_ENCODE_E			sms_encode,				//in:
    GUIEDIT_TEXT_LANGCHAR_NUM_T   *cur_langchar_num_ptr   //in:
)
{
    BOOLEAN                     result = FALSE;
    uint16                      add_ucs2_num = 0;
    GUIEDIT_TEXT_LANGCHAR_NUM_T langchar_num = {0};
    GUIEDIT_TEXT_LANGCHAR_NUM_T add_langchar_num = {0};

    //get add ucs2 number
    add_ucs2_num = MMITHEME_CountUcs2Char (is_sms, add_str_ptr, add_str_len, sms_encode);

    if (0 < add_ucs2_num)
    {
        result = TRUE;
    }
    else
    {
        if (is_sms)
        {
            //get add language private,shared char num
            MMITHEME_CountLangChar (add_str_ptr,
                                    add_str_len,
                                    &add_langchar_num);

            //set language private,shared char num
            langchar_num = *cur_langchar_num_ptr;

            //set current language private,shared char num
            langchar_num.tur     = (uint16) (langchar_num.tur + add_langchar_num.tur);
            langchar_num.esp     = (uint16) (langchar_num.esp + add_langchar_num.esp);
            langchar_num.por     = (uint16) (langchar_num.por + add_langchar_num.por);
            langchar_num.tur_esp = (uint16) (langchar_num.tur_esp + add_langchar_num.tur_esp);
            langchar_num.esp_por = (uint16) (langchar_num.esp_por + add_langchar_num.esp_por);

            //is convert ucs2
            result = TEXTEDIT_IsConvertUcs2 (add_ucs2_num, &langchar_num);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : get add no ucs2 len, when current string no ucs2
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,获得添加无ucs2的长度
/*****************************************************************************/
LOCAL uint16 GetAddNoUcs2LenByNoUcs2 (
    BOOLEAN                    is_sms,         //in:
    wchar                      *add_str_ptr,   //in:
    uint16                     add_str_len,    //in:
    uint16                     cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E		sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T         *max_len_ptr,   //in:
    GUIEDIT_TEXT_SPCHAR_NUM_T  *spchar_num_ptr //in:
)
{
    uint16      add_len = add_str_len;
    uint16      add_r8_2byte = 0;

    if (0 < add_len)
    {
        if (is_sms)
        {
            //get add r8 2byte number
            add_r8_2byte = MMITHEME_CountR82ByteChar (add_str_ptr, add_len);
        }

        //adjust current string len
        cur_str_len = (uint16) (cur_str_len + spchar_num_ptr->r8_2byte + spchar_num_ptr->default_ext);

        if (0 == spchar_num_ptr->r8_2byte)
        {
            //当前无ucs2,无r8 2byte
            if (0 == add_r8_2byte)
            {
                //当前无ucs2,无r8 2byte,获得无r8 2byte的长度
                add_len = GetAddNoR82ByteLenByNoR82Byte (is_sms,
                          add_str_ptr,
                          add_len,
                          cur_str_len,
                          sms_encode,
                          max_len_ptr,
                          spchar_num_ptr);
            }
            else
            {
                //当前无ucs2,无r8 2byte,获得添加有r8 2byte的长度
                add_len = GetAddR82ByteLenByNoR82Byte (is_sms,
                                                       add_str_ptr,
                                                       add_len,
                                                       cur_str_len,
                                                       sms_encode,
                                                       max_len_ptr,
                                                       spchar_num_ptr);
            }
        }
        else
        {
            //当前无ucs2,有r8 2byte
            add_len = GetAddNoUcs2LenByR82Byte (add_str_ptr,
                                                add_len,
                                                cur_str_len,
                                                max_len_ptr->r8_2byte,
                                                sms_encode);
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add no ucs2,no r8 2byte len, when current string no ucs2, no r8 2byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,获得添加无ucs2,无r8 2byte的长度
/*****************************************************************************/
LOCAL uint16 GetAddNoR82ByteLenByNoR82Byte (
    BOOLEAN                      is_sms,         //in:
    wchar                        *add_str_ptr,   //in:
    uint16                       add_str_len,    //in:
    uint16                       cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E			sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,   //in:
    GUIEDIT_TEXT_SPCHAR_NUM_T    *spchar_num_ptr //in:
)
{
    uint16      add_len = add_str_len;
    uint16      add_r8_1byte_num = 0;
    uint16      add_default_num = 0;

    if (0 < add_len)
    {
        if (is_sms)
        {
            //get add r8 1byte number
            add_r8_1byte_num = MMITHEME_CountR81ByteChar (add_str_ptr, add_len);

            //get add default ext number
            add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, add_len, sms_encode);
        }

        if (0 == spchar_num_ptr->r8_1byte)
        {
            //当前无ucs2,无r8 2byte,无r8 1byte
            if (0 == add_r8_1byte_num)
            {
                //当前无ucs2,无r8 2byte,无r8 1byte，获得无r8 1byte的长度
                add_len = GetAddNoR81ByteLenByNoR81Byte (is_sms,
                          add_str_ptr,
                          add_len,
                          cur_str_len,
                          sms_encode,
                          max_len_ptr);
            }
            else
            {
                //当前无ucs2,无r8 2byte,无r8 1byte,获得添加有r8 1byte的长度
                add_len = GetAddR81ByteLenByNoR81Byte (is_sms,
                                                       add_str_ptr,
                                                       add_len,
                                                       add_default_num,
                                                       cur_str_len,
                                                       sms_encode,
                                                       max_len_ptr);
            }
        }
        else
        {
            //当前无ucs2,无r8 2byte,有r8 1byte
            if (0 == add_default_num)
            {
                //get add len
                if ( (add_len + cur_str_len) > max_len_ptr->r8_1byte)
                {
                    add_len = (uint16) (max_len_ptr->r8_1byte - cur_str_len);
                }
            }
            else
            {
                //当前无ucs2,无r8 2byte,有r8 1byte,获得添加有default的长度
                add_len = GetAddDefaultLenByNoR82Byte (add_str_ptr,
                                                       add_len,
                                                       add_default_num,
                                                       cur_str_len,
                                                       max_len_ptr->r8_1byte,
                                                       sms_encode);
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add no ucs2,no r8 2byte,1byte len, when current string no ucs2, no r8 2byte,,1byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,无r8 1byte，获得无r8 1byte的长度
/*****************************************************************************/
LOCAL uint16 GetAddNoR81ByteLenByNoR81Byte (
    BOOLEAN              is_sms,         //in:
    wchar                *add_str_ptr,   //in:
    uint16               add_str_len,    //in:
    uint16               cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E	sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T   *max_len_ptr    //in:
)
{
    uint16      add_len = add_str_len;
    uint16      add_default_num = 0;

    if (0 < add_len)
    {
        if (is_sms)
        {
            //get add default ext number
            add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, add_len, sms_encode);
        }

        if (0 == add_default_num)
        {
            //get add len
            if ( (add_len + cur_str_len) > max_len_ptr->ascii)
            {
                add_len = (uint16) (max_len_ptr->ascii - cur_str_len);
            }
        }
        else
        {
            //当前无ucs2,无r8 2byte,无r8 1byte,获得添加有default的长度
            add_len = GetAddDefaultLenByNoR81Byte (add_str_ptr,
                                                   add_len,
                                                   add_default_num,
                                                   cur_str_len,
                                                   max_len_ptr->ascii,
                                                   sms_encode);
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add default len, when current string no ucs2,no r8 2byte,no r8 1byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,无r8 1byte,获得添加有default的长度
/*****************************************************************************/
LOCAL uint16 GetAddDefaultLenByNoR81Byte (
    wchar					*add_str_ptr,   //in:
    uint16					add_str_len,    //in:
    uint16					add_default_len,//in:
    uint16					cur_str_len,    //in:
    uint16					max_len_ascii,  //in:
    GUIEDIT_SMS_ENCODE_E	sms_encode		//in:sms encode type
)
{
    uint16  i = 0;
    uint16  add_len = add_str_len;
    uint16  char_len = 0;

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len + add_default_len) > max_len_ascii))
    {
        for (i = 0; i < add_len; i++)
        {
            //get char len
            if (MMITHEME_IsDefaultChar (add_str_ptr[i], sms_encode))
            {
                char_len = 2;
            }
            else
            {
                char_len = 1;
            }

            //get current string len
            cur_str_len = (uint16) (cur_str_len + char_len);

            if (cur_str_len > max_len_ascii)
            {
                add_len = i;
                break;
            }
            else if (cur_str_len == max_len_ascii)
            {
                add_len = (uint16) (i + 1);
                break;
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add r8 1byte len, when current string no ucs2,no r8 2byte,no r8 1byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,无r8 1byte,获得添加有r8 1byte的长度
/*****************************************************************************/
LOCAL uint16 GetAddR81ByteLenByNoR81Byte (
    BOOLEAN                is_sms,         //in:
    wchar                  *add_str_ptr,   //in:
    uint16                 add_str_len,    //in:
    uint16                 add_default,    //in:
    uint16                 cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E	sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T     *max_len_ptr    //in:
)
{
    uint16  i = 0;
    uint16  add_len = add_str_len;
    uint16  add_default_num = 0;
    uint16  first_r8_1byte_len = 0;

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len + add_default) > max_len_ptr->r8_1byte))
    {
        //get the first r8 1byte of add string
        for (i = 0; i < add_len; i++)
        {
            if (MMITHEME_IsR81ByteChar (add_str_ptr[i]))
            {
                first_r8_1byte_len = (uint16) (i + 1);
                break;
            }
        }

        //get add default ext number
        add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, first_r8_1byte_len, sms_encode);

        //get add len
        if ( (cur_str_len + first_r8_1byte_len + add_default_num) > max_len_ptr->r8_1byte)
        {
            add_len = (uint16) (first_r8_1byte_len - 1);

            //当前无ucs2,无r8 2byte,无r8 1byte,获得添加无r8 1byte的长度
            add_len = GetAddNoR81ByteLenByNoR81Byte (is_sms,
                      add_str_ptr,
                      add_len,
                      cur_str_len,
                      sms_encode,
                      max_len_ptr);
        }
        else
        {
            //当前无ucs2,无r8 2byte,有r8 1byte
            if (0 == add_default)
            {
                //get add len
                if ( (add_len + cur_str_len) > max_len_ptr->r8_1byte)
                {
                    add_len = (uint16) (max_len_ptr->r8_1byte - cur_str_len);
                }
            }
            else
            {
                //当前无ucs2,无r8 2byte,有r8 1byte,获得添加有default的长度
                add_len = GetAddDefaultLenByNoR82Byte (add_str_ptr,
                                                       add_len,
                                                       add_default,
                                                       cur_str_len,
                                                       max_len_ptr->r8_1byte,
                                                       sms_encode);
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add default len, when current string no ucs2,no r8 2byte,has r8 1byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,有r8 1byte,获得添加有default的长度
/*****************************************************************************/
LOCAL uint16 GetAddDefaultLenByNoR82Byte (
    wchar					*add_str_ptr,		//in:
    uint16					add_str_len,		//in:
    uint16					add_default_len,	//in:
    uint16					cur_str_len,		//in:
    uint16					max_len_r8_1byte,	//in:
    GUIEDIT_SMS_ENCODE_E	sms_encode			//in:sms encode type
)
{
    uint16  i = 0;
    uint16  add_len = add_str_len;
    uint16  char_len = 0;

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len + add_default_len) > max_len_r8_1byte))
    {
        for (i = 0; i < add_len; i++)
        {
            //get char len
            if (MMITHEME_IsDefaultChar (add_str_ptr[i], sms_encode))
            {
                char_len = 2;
            }
            else
            {
                char_len = 1;
            }

            //get current string len
            cur_str_len = (uint16) (cur_str_len + char_len);

            if (cur_str_len > max_len_r8_1byte)
            {
                add_len = i;
                break;
            }
            else if (cur_str_len == max_len_r8_1byte)
            {
                add_len = (uint16) (i + 1);
                break;
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add r8 2byte len, when current string no ucs2,no r8 2byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,无r8 2byte,获得添加有r8 2byte的长度
/*****************************************************************************/
LOCAL uint16 GetAddR82ByteLenByNoR82Byte (
    BOOLEAN                    is_sms,         //in:
    wchar                      *add_str_ptr,   //in:
    uint16                     add_str_len,    //in:
    uint16                     cur_str_len,    //in:
    GUIEDIT_SMS_ENCODE_E		sms_encode,		//in:sms encode type
    GUIEDIT_TEXT_MAX_T         *max_len_ptr,   //in:
    GUIEDIT_TEXT_SPCHAR_NUM_T  *spchar_num_ptr //in:
)
{
    uint16      i = 0;
    uint16      add_len = add_str_len;
    uint16      add_r8_2byte_num = 0;
    uint16      add_default_num = 0;
    uint16      first_r8_2byte_len = 0;

    //get add r8 2byte,default number
    add_r8_2byte_num = MMITHEME_CountR82ByteChar (add_str_ptr, add_len);
    add_default_num  = MMITHEME_CountDefaultExt (add_str_ptr, add_len, sms_encode);

    if ( (cur_str_len + add_len + add_r8_2byte_num + add_default_num) > max_len_ptr->r8_2byte)
    {
        //get the first r8 2byte of add string
        for (i = 0; i < add_len; i++)
        {
            if (MMITHEME_IsR82ByteChar (add_str_ptr[i]))
            {
                first_r8_2byte_len = (uint16) (i + 1);
                break;
            }
        }

        //get add default ext number
        add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, first_r8_2byte_len, sms_encode);

        //get add len
        if ( (cur_str_len + first_r8_2byte_len + 1 + add_default_num) > max_len_ptr->r8_2byte)
        {
            add_len = (uint16) (first_r8_2byte_len - 1);

            //当前无ucs2,无r8 2byte,获得添加无r8 2byte的长度
            add_len = GetAddNoR82ByteLenByNoR82Byte (is_sms,
                      add_str_ptr,
                      add_len,
                      cur_str_len,
                      sms_encode,
                      max_len_ptr,
                      spchar_num_ptr);
        }
        else
        {
            //当前无ucs2,无r8 2byte,获得添加有r8 2byte的长度
            add_len = GetAddNoUcs2LenByR82Byte (add_str_ptr,
                                                add_len,
                                                cur_str_len,
                                                max_len_ptr->r8_2byte,
                                                sms_encode);
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add r8 2byte or default len, when current string no ucs2,has r8 2byte
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,有r8 2byte,获得添加无ucs2的长度
/*****************************************************************************/
LOCAL uint16 GetAddNoUcs2LenByR82Byte (
    wchar					*add_str_ptr,		//in:
    uint16				add_str_len,		//in:
    uint16				cur_str_len,		//in:
    uint16				max_len_r8_2byte,	//in:
    GUIEDIT_SMS_ENCODE_E	sms_encode			//in:sms encode type
)
{
    uint16      i = 0;
    uint16      add_len = add_str_len;
    uint16      add_r8_2byte_num = 0;
    uint16      add_default_num = 0;
    uint16      char_len = 0;

    //get add r8 2byte,default number
    add_r8_2byte_num = MMITHEME_CountR82ByteChar (add_str_ptr, add_len);
    add_default_num  = MMITHEME_CountDefaultExt (add_str_ptr, add_len, sms_encode);

    if ( (cur_str_len + add_len + add_r8_2byte_num + add_default_num) > max_len_r8_2byte)
    {
        for (i = 0; i < add_len; i++)
        {
            //get char len
            if ( (MMITHEME_IsR82ByteChar (add_str_ptr[i])) ||
                    (MMITHEME_IsDefaultChar (add_str_ptr[i], sms_encode)))
            {
                char_len = 2;
            }
            else
            {
                char_len = 1;
            }

            //get current string len
            cur_str_len = (uint16) (cur_str_len + char_len);

            if (cur_str_len > max_len_r8_2byte)
            {
                add_len = i;
                break;
            }
            else if (cur_str_len == max_len_r8_2byte)
            {
                add_len = (uint16) (i + 1);
                break;
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add ucs2 len, when current string no ucs2
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无ucs2,获得添加有ucs2的长度
/*****************************************************************************/
LOCAL uint16 GetAddUcs2LenByNoUcs2 (
    BOOLEAN                      is_sms,             //in:
    wchar                        *add_str_ptr,       //in:
    uint16                       add_str_len,        //in:
    uint16                       cur_str_len,        //in:
    GUIEDIT_SMS_ENCODE_E			sms_encode,			//in:sms encode type
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,       //in:
    GUIEDIT_TEXT_SPCHAR_NUM_T    *spchar_num_ptr,    //in:
    GUIEDIT_TEXT_LANGCHAR_NUM_T  *langchar_num_ptr   //in:
)
{
    uint16      add_len = add_str_len;
    uint16      first_convert_ucs2 = 0;

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len) > max_len_ptr->ucs2))
    {
        //get the first convert ucs2 of add string
        first_convert_ucs2 = GetFirstConvertUcs2Char (is_sms,
                             add_str_ptr,
                             add_len,
                             sms_encode,
                             langchar_num_ptr);

        //get add len
        if ( (cur_str_len + first_convert_ucs2) > max_len_ptr->ucs2)
        {
            add_len = (uint16) (first_convert_ucs2 - 1);

            //当前无ucs2,获得添加无ucs2的长度
            add_len = GetAddNoUcs2LenByNoUcs2 (is_sms,
                                               add_str_ptr,
                                               add_len,
                                               cur_str_len,
                                               sms_encode,
                                               max_len_ptr,
                                               spchar_num_ptr);
        }
        else
        {
            add_len = (uint16) (max_len_ptr->ucs2 - cur_str_len);
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get the first ucs2 of add string
//  Global resource dependence :
//  Author: Jassmine
//  Note: 获得添加需要转换为ucs2的第一个字符长度
/*****************************************************************************/
LOCAL uint16 GetFirstConvertUcs2Char (
    BOOLEAN                        is_sms,             //in:
    wchar                          *add_str_ptr,       //in:
    uint16                         add_str_len,        //in:
    GUIEDIT_SMS_ENCODE_E			sms_encode,			//in:sms encode type
    GUIEDIT_TEXT_LANGCHAR_NUM_T    *langchar_num_ptr   //in:
)
{
    BOOLEAN                     is_find_usc2 = FALSE;
    BOOLEAN                     is_find_tur = FALSE;
    BOOLEAN                     is_find_esp = FALSE;
    BOOLEAN                     is_find_por = FALSE;
    BOOLEAN                     is_find_tur_esp = FALSE;
    BOOLEAN                     is_find_esp_por = FALSE;
    uint16                      i = 0;
    uint16                      first_convert = 0;
    uint16                      first_ucs2 = 0;
    GUIEDIT_TEXT_LANGCHAR_NUM_T first_langchar = {0};

    if (is_sms)
    {
        //get the first ucs2,Turkish,Spanish,Portuguese private etc. char
        for (i = 0; i < add_str_len; i++)
        {
            if ( (!is_find_usc2) &&
                    (MMITHEME_IsUcs2Char (is_sms, add_str_ptr[i], sms_encode)))
            {
                first_ucs2   = (uint16) (i + 1);
                is_find_usc2 = TRUE;
            }
            else if ( (!is_find_tur) &&
                      (MMITHEME_IsR8Turkish (add_str_ptr[i])))
            {
                first_langchar.tur = (uint16) (i + 1);
                is_find_tur = TRUE;
            }
            else if ( (!is_find_esp) &&
                      (MMITHEME_IsR8Spanish (add_str_ptr[i])))
            {
                first_langchar.esp = (uint16) (i + 1);
                is_find_esp = TRUE;
            }
            else if ( (!is_find_por) &&
                      (MMITHEME_IsR8Portuguese (add_str_ptr[i])))
            {
                first_langchar.por = (uint16) (i + 1);
                is_find_por = TRUE;
            }
            else if ( (!is_find_tur_esp) &&
                      (MMITHEME_IsSpanishAndTurkish (add_str_ptr[i])))
            {
                first_langchar.tur_esp = (uint16) (i + 1);
                is_find_tur_esp = TRUE;
            }
            else if ( (!is_find_esp_por) &&
                      (MMITHEME_IsSpanishAndPortuguese (add_str_ptr[i])))
            {
                first_langchar.esp_por = (uint16) (i + 1);
                is_find_esp_por = TRUE;
            }
        }

        //get the first convert ucs2
        first_convert = add_str_len;

        if (is_find_usc2)
        {
            first_convert = (uint16) MIN (first_convert, first_ucs2);
        }

        if (0 < langchar_num_ptr->tur)
        {
            if (is_find_esp)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.esp);
            }

            if (is_find_por)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.por);
            }

            if (is_find_esp_por)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.esp_por);
            }
        }

        if (0 < langchar_num_ptr->esp)
        {
            if (is_find_tur)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.tur);
            }

            if (is_find_por)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.por);
            }
        }

        if (0 < langchar_num_ptr->por)
        {
            if (is_find_tur)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.tur);
            }

            if (is_find_esp)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.esp);
            }

            if (is_find_tur_esp)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.tur_esp);
            }
        }

        if (0 < langchar_num_ptr->tur_esp)
        {
            if (is_find_por)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.por);
            }
        }

        if (0 < langchar_num_ptr->esp_por)
        {
            if (is_find_tur)
            {
                first_convert = (uint16) MIN (first_convert, first_langchar.tur);
            }
        }
    }
    else
    {
        //get the first ucs2 of add string
        for (i = 0; i < add_str_len; i++)
        {
            if (MMITHEME_IsUcs2Char (is_sms, add_str_ptr[i], sms_encode))
            {
                first_convert = (uint16) (i + 1);
                break;
            }
        }
    }

    return (first_convert);
}

/*****************************************************************************/
//  Description : get add string len, when support pb encode
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL uint16 GetAddStrLenForPb (
    wchar                *add_str_ptr,   //in:add string
    uint16               add_str_len,    //in:add string len
    uint16               cur_str_len,    //in:current string len
    uint16               default_ext_num,//in:default char number
    GUIEDIT_TEXT_MAX_T   *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T  *ucs2_ptr       //in:max and min ucs2
)
{
    uint16                      add_len = add_str_len;
    MMITHEME_PB_ENCODE_TYPE_E   cur_encode_type = MMITHEME_PB_ENCODE_NONE;
    MMITHEME_PB_ENCODE_TYPE_E   add_encode_type = MMITHEME_PB_ENCODE_NONE;

    //get encode type
    cur_encode_type = MMITHEME_GetEncodeTypeForPb (ucs2_ptr->max, ucs2_ptr->min);

    //当前非80编码
    if (MMITHEME_PB_ENCODE_80 != cur_encode_type)
    {
        //get encode type after add string
        add_encode_type = GetEncodeTypeAfterAdd (add_str_ptr, add_len, ucs2_ptr);

        if (MMITHEME_PB_ENCODE_80 == add_encode_type)
        {
            //当前非80编码,添加后80编码
            add_len = GetAdd80LenByNo80 (add_str_ptr,
                                         add_len,
                                         cur_str_len,
                                         default_ext_num,
                                         max_len_ptr,
                                         ucs2_ptr,
                                         cur_encode_type);
        }
        else
        {
            //当前非80编码,添加后非80编码
            add_len = GetAddNo80LenByNo80 (add_str_ptr,
                                           add_len,
                                           cur_str_len,
                                           default_ext_num,
                                           max_len_ptr,
                                           ucs2_ptr,
                                           cur_encode_type);
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get encode type after add string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMITHEME_PB_ENCODE_TYPE_E GetEncodeTypeAfterAdd (
    wchar                 *add_str_ptr,   //in:add string
    uint16                add_str_len,    //in:add string len
    GUIEDIT_TEXT_UCS2_T   *ucs2_ptr       //in:max and min ucs2
)
{
    uint16                      add_ucs2_num = 0;
    GUIEDIT_TEXT_UCS2_T         ucs2_value = *ucs2_ptr;
    MMITHEME_PB_ENCODE_TYPE_E   encode_type = MMITHEME_PB_ENCODE_NONE;

    //get encode type
    encode_type = MMITHEME_GetEncodeTypeForPb (ucs2_value.max, ucs2_value.min);

    //get add ucs2 number
    add_ucs2_num = MMITHEME_CountUcs2Char (FALSE, add_str_ptr, add_str_len, GUIEDIT_SMS_ENCODE_AUTO);

    if (0 < add_ucs2_num)
    {
        //get max and min ucs2
        AdjustMaxMinUcs2 (TRUE,
                          add_str_ptr,
                          add_str_len,
                          0,
                          PNULL,
                          0,
                          &ucs2_value);

        //get encode type
        encode_type = MMITHEME_GetEncodeTypeForPb (ucs2_value.max, ucs2_value.min);
    }

    return (encode_type);
}

/*****************************************************************************/
//  Description : get add 80 len, when current string no 80
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,获得添加有80编码的长度
/*****************************************************************************/
LOCAL uint16 GetAdd80LenByNo80 (
    wchar                        *add_str_ptr,   //in:add string
    uint16                       add_str_len,    //in:add string len
    uint16                       cur_str_len,    //in:current string len
    uint16                       default_ext_num,//in:default char number
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T          *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E    cur_encode_type //in:pointed encode type
)
{
    uint16      add_len = add_str_len;
    uint16      max_80_len = 0;
    uint16      first_convert_80 = 0;

    //get max 80 encode len
    max_80_len = MMITHEME_GetMaxLenForPb (max_len_ptr->ascii, max_len_ptr->ucs2, MMITHEME_PB_ENCODE_80);

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len) > max_80_len))
    {
        //get the first convert 80 of add string
        first_convert_80 = GetFirstConvertTypeChar (add_str_ptr,
                           add_len,
                           ucs2_ptr,
                           MMITHEME_PB_ENCODE_80);

        //get add len
        if ( (cur_str_len + first_convert_80) > max_80_len)
        {
            add_len = (uint16) (first_convert_80 - 1);

            //当前无80,获得添加无80的长度
            add_len = GetAddNo80LenByNo80 (add_str_ptr,
                                           add_len,
                                           cur_str_len,
                                           default_ext_num,
                                           max_len_ptr,
                                           ucs2_ptr,
                                           cur_encode_type);
        }
        else
        {
            add_len = (uint16) (max_80_len - cur_str_len);
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get the first pointed type of add string
//  Global resource dependence :
//  Author: Jassmine
//  Note: 获得添加需要转换为80,81,82的第一个字符长度
/*****************************************************************************/
LOCAL uint16 GetFirstConvertTypeChar (
    wchar                      *add_str_ptr,   //in:add string
    uint16                     add_str_len,    //in:add string len
    GUIEDIT_TEXT_UCS2_T        *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E  encode_type     //in:pointed encode type
)
{
    uint16      i = 0;
    uint16      first_convert = 0;
    uint16      max_ucs2 = ucs2_ptr->max;
    uint16      min_ucs2 = ucs2_ptr->min;

    //get the first encode type of add string
    for (i = 0; i < add_str_len; i++)
    {
        //is ucs2 char
        if (MMITHEME_IsUcs2Char (FALSE, add_str_ptr[i], GUIEDIT_SMS_ENCODE_AUTO))
        {
            //get max and min ucs2
            max_ucs2 = (wchar) MAX (max_ucs2, add_str_ptr[i]);
            min_ucs2 = (wchar) MIN (min_ucs2, add_str_ptr[i]);

            if (encode_type == MMITHEME_GetEncodeTypeForPb (max_ucs2, min_ucs2))
            {
                first_convert = (uint16) (i + 1);
                break;
            }
        }
    }

    return (first_convert);
}

/*****************************************************************************/
//  Description : get add no 80 len, when current string no 80
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,获得添加无80编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo80LenByNo80 (
    wchar                      *add_str_ptr,   //in:add string
    uint16                     add_str_len,    //in:add string len
    uint16                     cur_str_len,    //in:current string len
    uint16                     default_ext_num,//in:default char number
    GUIEDIT_TEXT_MAX_T         *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T        *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E  cur_encode_type //in:current encode type
)
{
    uint16                      add_len = add_str_len;
    uint16                      add_default_num = 0;
    MMITHEME_PB_ENCODE_TYPE_E   add_encode_type = MMITHEME_PB_ENCODE_NONE;

    if (0 < add_len)
    {
        //adjust current string len
        cur_str_len = (uint16) (cur_str_len + default_ext_num);

        //get encode type after add string
        add_encode_type = GetEncodeTypeAfterAdd (add_str_ptr, add_len, ucs2_ptr);

        //get add default ext number
        add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, add_len, GUIEDIT_SMS_ENCODE_AUTO);

        if (MMITHEME_PB_ENCODE_82 == cur_encode_type)
        {
            //当前无80编码,有82编码
            add_len = GetAddNo80LenBy82 (add_str_ptr, add_len, add_default_num, cur_str_len, max_len_ptr);
        }
        else
        {
            //当前无80编码,无82编码
            if (MMITHEME_PB_ENCODE_82 == add_encode_type)
            {
                //当前无80编码,无82编码,获得添加有82编码的长度
                add_len = GetAdd82LenByNo82 (add_str_ptr,
                                             add_len,
                                             add_default_num,
                                             cur_str_len,
                                             max_len_ptr,
                                             ucs2_ptr,
                                             cur_encode_type);
            }
            else
            {
                //当前无80编码,无82编码,获得添加无82编码的长度
                add_len = GetAddNo82LenByNo82 (add_str_ptr,
                                               add_len,
                                               cur_str_len,
                                               max_len_ptr,
                                               ucs2_ptr,
                                               cur_encode_type);
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add no 80 len, when current string 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,有82编码,获得添加无80编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo80LenBy82 (
    wchar                *add_str_ptr,   //in:add string
    uint16               add_str_len,    //in:add string len
    uint16               add_default,    //in:add default string len
    uint16               cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T   *max_len_ptr    //in:ucs2,r8,ascii max len
)
{
    uint16      i = 0;
    uint16      add_len = add_str_len;
    uint16      max_82_len = 0;
    uint16      char_len = 0;

    //get max 82 encode len
    max_82_len = MMITHEME_GetMaxLenForPb (max_len_ptr->ascii, max_len_ptr->ucs2, MMITHEME_PB_ENCODE_82);

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len + add_default) > max_82_len))
    {
        for (i = 0; i < add_len; i++)
        {
            //get char len
            if (MMITHEME_IsDefaultChar (add_str_ptr[i], GUIEDIT_SMS_ENCODE_AUTO))
            {
                char_len = 2;
            }
            else
            {
                char_len = 1;
            }

            //get current string len
            cur_str_len = (uint16) (cur_str_len + char_len);

            if (cur_str_len > max_82_len)
            {
                add_len = i;
                break;
            }
            else if (cur_str_len == max_82_len)
            {
                add_len = (uint16) (i + 1);
                break;
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add no 81 len, when current string 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,获得添加有82编码的长度
/*****************************************************************************/
LOCAL uint16 GetAdd82LenByNo82 (
    wchar                        *add_str_ptr,   //in:add string
    uint16                       add_str_len,    //in:add string len
    uint16                       add_default,    //in:add default string len
    uint16                       cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T           *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T          *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E    cur_encode_type //in:pointed encode type
)
{
    uint16      add_len = add_str_len;
    uint16      max_82_len = 0;
    uint16      first_convert_82 = 0;
    uint16      add_default_num = 0;

    //get max 82 encode len
    max_82_len = MMITHEME_GetMaxLenForPb (max_len_ptr->ascii, max_len_ptr->ucs2, MMITHEME_PB_ENCODE_82);

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len + add_default) > max_82_len))
    {
        //get the first convert 82 of add string
        first_convert_82 = GetFirstConvertTypeChar (add_str_ptr,
                           add_len,
                           ucs2_ptr,
                           MMITHEME_PB_ENCODE_82);

        //get add default ext number
        add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, first_convert_82, GUIEDIT_SMS_ENCODE_AUTO);

        //get add len
        if ( (cur_str_len + first_convert_82 + add_default_num) > max_82_len)
        {
            add_len = (uint16) (first_convert_82 - 1);

            //当前无80编码,无82编码,获得添加无82编码的长度
            add_len = GetAddNo82LenByNo82 (add_str_ptr,
                                           add_len,
                                           cur_str_len,
                                           max_len_ptr,
                                           ucs2_ptr,
                                           cur_encode_type);
        }
        else
        {
            //当前无80编码,有82编码
            add_len = GetAddNo80LenBy82 (add_str_ptr, add_len, add_default, cur_str_len, max_len_ptr);
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add no 82 len, when current string no 82
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,获得添加无82编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo82LenByNo82 (
    wchar                      *add_str_ptr,   //in:add string
    uint16                     add_str_len,    //in:add string len
    uint16                     cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T         *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T        *ucs2_ptr,      //in:max and min ucs2
    MMITHEME_PB_ENCODE_TYPE_E  cur_encode_type //in:current encode type
)
{
    uint16                      add_len = add_str_len;
    uint16                      add_default_num = 0;
    MMITHEME_PB_ENCODE_TYPE_E   add_encode_type = MMITHEME_PB_ENCODE_NONE;

    if (0 < add_len)
    {
        //get encode type after add string
        add_encode_type = GetEncodeTypeAfterAdd (add_str_ptr, add_len, ucs2_ptr);

        //get add default ext number
        add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, add_len, GUIEDIT_SMS_ENCODE_AUTO);

        if (MMITHEME_PB_ENCODE_81 == cur_encode_type)
        {
            //当前无80编码,无82编码,有81编码
            add_len = GetAddNo82LenBy81 (add_str_ptr, add_len, cur_str_len, max_len_ptr);
        }
        else
        {
            //当前无80编码,无82编码,无81编码
            if (MMITHEME_PB_ENCODE_81 == add_encode_type)
            {
                //当前无80编码,无82编码,无81编码,获得添加有81编码的长度
                add_len = GetAdd81LenByNo81 (add_str_ptr,
                                             add_len,
                                             add_default_num,
                                             cur_str_len,
                                             max_len_ptr,
                                             ucs2_ptr);
            }
            else
            {
                //当前无80编码,无82编码,无81编码,获得添加无81编码的长度
                add_len = GetAddNo81LenByNo81 (add_str_ptr, add_len, cur_str_len, max_len_ptr);
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add no 82 len, when current string 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,获得添加有81编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo82LenBy81 (
    wchar                *add_str_ptr,   //in:add string
    uint16               add_str_len,    //in:add string len
    uint16               cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T   *max_len_ptr    //in:ucs2,r8,ascii max len
)
{
    uint16      i = 0;
    uint16      add_len = add_str_len;
    uint16      max_81_len = 0;
    uint16      add_default_num = 0;
    uint16      char_len = 0;

    //get max 81 encode len
    max_81_len = MMITHEME_GetMaxLenForPb (max_len_ptr->ascii, max_len_ptr->ucs2, MMITHEME_PB_ENCODE_81);

    //get add default ext number
    add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, add_len, GUIEDIT_SMS_ENCODE_AUTO);

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len + add_default_num) > max_81_len))
    {
        for (i = 0; i < add_len; i++)
        {
            //get char len
            if (MMITHEME_IsDefaultChar (add_str_ptr[i], GUIEDIT_SMS_ENCODE_AUTO))
            {
                char_len = 2;
            }
            else
            {
                char_len = 1;
            }

            //get current string len
            cur_str_len = (uint16) (cur_str_len + char_len);

            if (cur_str_len > max_81_len)
            {
                add_len = i;
                break;
            }
            else if (cur_str_len == max_81_len)
            {
                add_len = (uint16) (i + 1);
                break;
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add no 81 len, when current string 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,无81编码,获得添加有81编码的长度
/*****************************************************************************/
LOCAL uint16 GetAdd81LenByNo81 (
    wchar                *add_str_ptr,   //in:add string
    uint16               add_str_len,    //in:add string len
    uint16               add_default,    //in:add default string len
    uint16               cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T   *max_len_ptr,   //in:ucs2,r8,ascii max len
    GUIEDIT_TEXT_UCS2_T  *ucs2_ptr       //in:max and min ucs2
)
{
    uint16      add_len = add_str_len;
    uint16      max_81_len = 0;
    uint16      first_convert_81 = 0;
    uint16      add_default_num = 0;

    //get max 81 encode len
    max_81_len = MMITHEME_GetMaxLenForPb (max_len_ptr->ascii, max_len_ptr->ucs2, MMITHEME_PB_ENCODE_81);

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len + add_default) > max_81_len))
    {
        //get the first convert 81 of add string
        first_convert_81 = GetFirstConvertTypeChar (add_str_ptr,
                           add_len,
                           ucs2_ptr,
                           MMITHEME_PB_ENCODE_81);

        //get add default ext number
        add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, first_convert_81, GUIEDIT_SMS_ENCODE_AUTO);

        //get add len
        if ( (cur_str_len + first_convert_81 + add_default_num) > max_81_len)
        {
            add_len = (uint16) (first_convert_81 - 1);

            //当前无80编码,无82编码,无81编码,获得添加无81编码的长度
            add_len = GetAddNo81LenByNo81 (add_str_ptr, add_len, cur_str_len, max_len_ptr);
        }
        else
        {
            //当前无80编码,无82编码,有81编码
            add_len = GetAddNo82LenBy81 (add_str_ptr, add_len, cur_str_len, max_len_ptr);
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : get add no 81 len, when current string no 81
//  Global resource dependence :
//  Author: Jassmine
//  Note: 当前无80编码,无82编码,无81编码,获得添加无81编码的长度
/*****************************************************************************/
LOCAL uint16 GetAddNo81LenByNo81 (
    wchar              *add_str_ptr,   //in:add string
    uint16             add_str_len,    //in:add string len
    uint16             cur_str_len,    //in:current string len
    GUIEDIT_TEXT_MAX_T *max_len_ptr    //in:ucs2,r8,ascii max len
)
{
    uint16      i = 0;
    uint16      add_len = add_str_len;
    uint16      max_none_len = 0;
    uint16      add_default_num = 0;
    uint16      char_len = 0;

    //get max none encode len
    max_none_len = MMITHEME_GetMaxLenForPb (max_len_ptr->ascii, max_len_ptr->ucs2, MMITHEME_PB_ENCODE_NONE);

    //get add default ext number
    add_default_num = MMITHEME_CountDefaultExt (add_str_ptr, add_len, GUIEDIT_SMS_ENCODE_AUTO);

    if ( (0 < add_len) &&
            ( (cur_str_len + add_len + add_default_num) > max_none_len))
    {
        for (i = 0; i < add_len; i++)
        {
            //get char len
            if (MMITHEME_IsDefaultChar (add_str_ptr[i], GUIEDIT_SMS_ENCODE_AUTO))
            {
                char_len = 2;
            }
            else
            {
                char_len = 1;
            }

            //get current string len
            cur_str_len = (uint16) (cur_str_len + char_len);

            if (cur_str_len > max_none_len)
            {
                add_len = i;
                break;
            }
            else if (cur_str_len == max_none_len)
            {
                add_len = (uint16) (i + 1);
                break;
            }
        }
    }

    return (add_len);
}

/*****************************************************************************/
//  Description : adjust max and min ucs2
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void AdjustMaxMinUcs2 (
    BOOLEAN             is_add,         //in:is add or delete
    wchar               *str_ptr,       //in:
    uint16              str_len,        //in:
    uint16              del_start_pos,  //in:only for delete
    wchar               *all_str_ptr,   //in:only for delete
    uint16              all_str_len,    //in:only for delete
    GUIEDIT_TEXT_UCS2_T *ucs2_ptr       //in/out:max and min ucs2
)
{
    wchar       max_ucs2 = 0;
    wchar       min_ucs2 = 0;
    wchar       *left_str_ptr = PNULL;
    uint16      left_str_len = 0;

    if (MMITHEME_GetMaxAndMinUcs2 (str_ptr, str_len, &max_ucs2, &min_ucs2))
    {
        //adjust max and min ucs2
        if (is_add)
        {
            ucs2_ptr->max = (wchar) MAX (ucs2_ptr->max, max_ucs2);
            ucs2_ptr->min = (wchar) MIN (ucs2_ptr->min, min_ucs2);
        }
        else
        {
            if ( (ucs2_ptr->max == max_ucs2) ||
                    (ucs2_ptr->min == min_ucs2))
            {
                //reset max and min ucs2
                ucs2_ptr->max = 0;
                ucs2_ptr->min = 0xFFFF;

                if (0 < del_start_pos)
                {
                    //get max and min ucs2 before delete string
                    left_str_ptr = all_str_ptr;
                    left_str_len = del_start_pos;

                    if (MMITHEME_GetMaxAndMinUcs2 (left_str_ptr, left_str_len, &max_ucs2, &min_ucs2))
                    {
                        ucs2_ptr->max = (wchar) MAX (ucs2_ptr->max, max_ucs2);
                        ucs2_ptr->min = (wchar) MIN (ucs2_ptr->min, min_ucs2);
                    }
                }

                if (del_start_pos + str_len < all_str_len)
                {
                    //get max and min ucs2 after delete string
                    left_str_ptr = all_str_ptr + del_start_pos + str_len;
                    left_str_len = (uint16) (all_str_len - del_start_pos - str_len);

                    if (MMITHEME_GetMaxAndMinUcs2 (left_str_ptr, left_str_len, &max_ucs2, &min_ucs2))
                    {
                        ucs2_ptr->max = (wchar) MAX (ucs2_ptr->max, max_ucs2);
                        ucs2_ptr->min = (wchar) MIN (ucs2_ptr->min, min_ucs2);
                    }
                }
            }
        }
    }
}





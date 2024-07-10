/*****************************************************************************
** File Name:      mmimp3_lyric.c                                            *
** Author:                                                                   *
** Date:           24/03/2004                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2007       Jassmine.Meng       Create                                  *
** 11/2009       xingdong.li         Modified                                *
******************************************************************************/
#ifdef MMIAP_LYRIC_SUPPORT
#define _MMIMP3_LYRIC_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"

#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiap_common.h"
#include "mmiap_list.h"
#include "mmiap_lyric_internal.h"
#include "mmimp3_export.h"
#include "mmifmm_export.h"
//#include "mmi_id.h"
#include "mmimp3_id.h"
#include "mmi_appmsg.h"
#include "guifont.h"
#include "mmk_timer.h"
#include "guilabel.h"
#include "mmimp3_image.h"
#include "mmidisplay_data.h"
#include "mmi_default.h"
#include "mmi_image.h"
#include "guistring.h"
#include "guires.h"
#include "guiownerdraw.h"
#include "gui_ucs2b_converter.h"
#include "mmiap_setting.h"
#include "mmiapwin_main.h"
#include "mmiap_play.h"

#include "mmiap_list_control.h"
#include "mmiapwin_main.h"
#include "mmiap_play.h"
#include "mmiap_appmain.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define LYRIC_ANIM_FRAME_INTERVAL   100
#define LYRIC_ANIM_FRAME_NUM    6

typedef enum
{
    LYRIC_DISP_MIN_S = 0,
    LYRIC_DISP_NORMAL_S = LYRIC_DISP_MIN_S,     /* Display lyric normally */
    //LYRIC_DISP_WAITINT_SWITCH_S,
    LYRIC_DISP_SWITCH_S,                                /* Display lyric anim effect */
    //LYRIC_DISP_WAITINT_SCROLL_S,
    //LYRIC_DISP_SCROLL_S,
    LYRIC_DISP_MAX_S
} LYRIC_DISP_STATE_E;

typedef struct
{
    LYRIC_DISP_STATE_E state;
    uint8 anim_frame;   /* 当前动画帧的序号；递减，0是最后一帧*/
    uint8 anim_frame_total; /* 总的动画帧数*/
    uint8 anim_time;    /*动画间隔时间*/
} LYRIC_RUNNING_INFO_T;

typedef struct
{
    MMIMP3_LYRIC_FILE_INFO_T      file_info;
    MMIMP3_LYRIC_DISPLAY_INFO_T   display_info;  /*lint !e64*/
    LYRIC_RUNNING_INFO_T lyric_running_info;
} MMIAP_LYRIC_INFO_T;

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

LOCAL MMIAP_LYRIC_INFO_T *s_mmiap_lyric_info_ptr = PNULL;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : initialize mp3 lyric
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void InitLyric(void);

/*****************************************************************************/
//  Description : open lyric file
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenLyricFile(
    uint16      cur_index
);

/*****************************************************************************/
//  Description : close lyric file
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void CloseLyricFile(
    void
);

/*****************************************************************************/
//  Description : Alloc lyric parse buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AllocParseBuffer(
    void
);

/*****************************************************************************/
//  Description : Alloc the tag info buffer
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AllocTagInfoBuffer(
    void
);

/*****************************************************************************/
//  Description : Free lyric parse buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void FreeParseBuffer(
    void
);

/*****************************************************************************/
//  Description : Free all the buffer allocated
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void FreeLyricBuffer(
    void
);

/*****************************************************************************/
//  Description : Free the sentence buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void FreeLyricSentenceBuffer(
    void
);

/*****************************************************************************/
//  Description : Free the parsed data buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void FreeParseDataBuffer(
    void
);

/*****************************************************************************/
//  Description : Reset the displaying Sentence Info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
/*
LOCAL void ResetLyricDisplaySentenceInfo(
                void
                );
*/
/*****************************************************************************/
//  Description : Read the lyric sentence in sequence.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadLyricSentence(
    uint32 sentence_index,
    uint32 *array_index_ptr
);

/*****************************************************************************/
//  Description : Get the lyric sentence in sequence.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_LYRIC_SENTENCE_T *GetLyricSentence(
    uint32 sentence_index
);

/*****************************************************************************/
//  Description : get lyric file name
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLyricFileFullPathName(
    uint16  *lyric_name_ptr    //in/out
);


/*****************************************************************************/
//  Description : sort lyric timer tag
//  Global resource dependence :
//  Author:Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL int LyricTimeTagSort(
    const void   *arg1,
    const void   *arg2
);

/*****************************************************************************/
//  Description : Start Lyric Timer
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StartLyricTimer(
    void
);

/*****************************************************************************/
//  Description : stop lyric timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopLyricTimer(void);

/*****************************************************************************/
//  Description : is lyric file opened
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLyricFileOpened(void);

/*****************************************************************************/
//  Description : Parse Lyric File
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseLyricFile(
    void
);

/*****************************************************************************/
//  Description : Test Next Lyric Char
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TestNextLyricChar(uint16 lyric_char);

/*****************************************************************************/
//  Description : Save Cur Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveCurTag(uint32 tag_time);

/*****************************************************************************/
//  Description : Save Cur Sentence
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveCurSentence(uint32 offset, uint32 len);

/*****************************************************************************/
//  Description : Save Empty Sentence
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveEmptySentence(void);

/*****************************************************************************/
//  Description : Save Time Offset
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveTimeOffset(uint16 *tag_offset_ptr, uint32 offset_len);

/*****************************************************************************/
//  Description : Sort Lyric Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SortLyricTag(void);

/*****************************************************************************/
//  Description : Find Next Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindNextTag(
    uint16  *tag_value_ptr1,        //[OUT]
    uint32  *tag_len_ptr1,          //[IN&OUT]
    uint16  *tag_value_ptr2,        //[OUT]
    uint32  *tag_len_ptr2           //[IN&OUT]
);

/*****************************************************************************/
//  Description : Find Left Square Bracket
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindLeftSquareBracket(void);

/*****************************************************************************/
//  Description : Find Colon
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindColon(
    uint16  *tag_value_ptr1,
    uint32  *tag_len_ptr1
);

/*****************************************************************************/
//  Description : Find Right Square Bracket
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindRightSquareBracket(
    uint16  *tag_value_ptr2,
    uint32  *tag_len_ptr2
);

/*****************************************************************************/
//  Description : Check Tag Type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_LYRIC_TAG_TYPE_E CheckTagType(
    uint16  *lyric_tag_ptr
);

/*****************************************************************************/
//  Description : Find Digital Part
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindDigitalPart(
    uint16  *tag_value_ptr,         //[IN]
    uint32  tag_len,                //[IN]
    uint16  *integer_value_ptr,     //[OUT]
    uint32  *integer_len_ptr,       //[OUT]
    uint16  *decimal_value_ptr,     //[OUT]
    uint32  *decimal_len_ptr        //[OUT]
);

/*****************************************************************************/
//  Description : Check Time Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckTimeTag(
    uint16  *tag_value_ptr1,    //[IN]
    uint32  tag_value_len1,     //[IN]
    uint16  *tag_value_ptr2,    //[IN]
    uint32  tag_value_len2,     //[IN]
    uint32  *tag_time_ptr       //[OUT]
);

/*****************************************************************************/
//  Description : Find Next Sentence
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindNextSentence(uint32 *offset_ptr, uint32 *len_ptr);

/*****************************************************************************/
//  Description : Get Next Lyric Char
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNextLyricChar(void);

/*****************************************************************************/
//  Description : Is File End
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFileEnd(void);

/*****************************************************************************/
//  Description : Get Lyric Cur Pos
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricCurWcharPos(void);

/*****************************************************************************/
//  Description : Get Lyric Tag Num
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricTagNum(void);
/*****************************************************************************/
//  Description : Get Lyric Sentence Num
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricSentenceNum(void);

/*****************************************************************************/
//  Description : Get text file type
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLyricTextFileType(//type
    MMIFILE_HANDLE file_handle, // in
    GUI_CHAR_CODE_TYPE_E *file_type // out
);

/*****************************************************************************/
//  Description : Get text file head len
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLyricTextFileHeadLen(
    GUI_CHAR_CODE_TYPE_E lyric_code_type
);

/*****************************************************************************/
//  Description : Get other code len
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint16 GetOtherCodeLenFromUsc(
    GUI_CHAR_CODE_TYPE_E  code_type,
    const wchar           *wstr_ptr,
    uint16                wstr_len
);

/*****************************************************************************/
//  Description : Set Multi Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetMultiTag(BOOLEAN is_multi_tag_line);

/*****************************************************************************/
//  Description : Is Multi Tag In Line
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMultiTagInLine(void);

/*****************************************************************************/
//  Description : Go Back One Char
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GoBackOneChar(void);

/*****************************************************************************/
//  Description : Init lyric disp ctrl var.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void InitLyricDispCtrl(void);

/*****************************************************************************/
//  Description : Set Lyric display control state.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetLyricDispState(LYRIC_DISP_STATE_E state);

/*****************************************************************************/
//  Description : Set lyric anim time.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetLyricAnimTime(uint32 time);

/*****************************************************************************/
//  Description : Set current lyric anim frame and total frame.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetLyricAnimFrame(uint8 anim_frame, uint8 anim_frame_total);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get ap lyric info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_LYRIC_INFO_T *GetAPLyricInfoPtr(void)
{
    return s_mmiap_lyric_info_ptr;
}

/*****************************************************************************/
//  Description : create ap lyric info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAPLyricInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIAP_LYRIC_INFO_T *info_ptr = GetAPLyricInfoPtr();
    uint32 info_size = sizeof(*info_ptr);


    if (PNULL == info_ptr)
    {
        info_ptr = SCI_ALLOCA(info_size);
    }

    if (PNULL != info_ptr)
    {
        s_mmiap_lyric_info_ptr = info_ptr;

        SCI_MEMSET(info_ptr, 0, info_size);

        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : destroy lyric ino
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyLyricInfo(
    void
)
{
    if (PNULL != s_mmiap_lyric_info_ptr)
    {
        SCI_FREE(s_mmiap_lyric_info_ptr);
    }
}

/*****************************************************************************/
//  Description : get ap lyric file info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_LYRIC_FILE_INFO_T *GetLyricFileInfoPtr(
    void
)
{
    MMIAP_LYRIC_INFO_T *info_ptr = GetAPLyricInfoPtr();
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        file_ptr = &info_ptr->file_info;
    }

    return file_ptr;
}

/*****************************************************************************/
//  Description : get ap lyric display info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_LYRIC_DISPLAY_INFO_T *GetLyricDisplayInfoPtr(
    void
)
{
    MMIAP_LYRIC_INFO_T *info_ptr = GetAPLyricInfoPtr();
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        display_ptr = &info_ptr->display_info;
    }

    return display_ptr;
}

/*****************************************************************************/
//  Description : get lyric running info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL LYRIC_RUNNING_INFO_T *GetLyricRunningInfoPtr(
    void
)
{
    MMIAP_LYRIC_INFO_T *info_ptr = GetAPLyricInfoPtr();
    LYRIC_RUNNING_INFO_T *run_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        run_ptr = &info_ptr->lyric_running_info;
    }

    return run_ptr;
}

/*****************************************************************************/
//  Description : initialize mp3 lyric
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void InitLyric(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();

    if (PNULL == file_ptr
            || PNULL == display_ptr
       )
    {
        return;
    }

    SCI_MEMSET(file_ptr,   0, sizeof(MMIMP3_LYRIC_FILE_INFO_T));
    file_ptr->is_buf_parse_finished = TRUE;//add by CR247908 2011.06.14
    SCI_MEMSET(display_ptr, 0, sizeof(MMIMP3_LYRIC_DISPLAY_INFO_T));
}

/*****************************************************************************/
//  Description : Free all the buffer allocated
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void FreeLyricBuffer(
    void
)
{
    //SCI_TRACE_LOW:"[MMIMP3]:FreeLyricBuffer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_451_112_2_18_2_4_18_237, (uint8*)"");
    FreeParseBuffer();
    FreeLyricSentenceBuffer();
    FreeParseDataBuffer();
}

/*****************************************************************************/
//  Description : Free the sentence buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void FreeLyricSentenceBuffer(
    void
)
{
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();
    uint8 i = 0;


    if (PNULL == display_ptr)
    {
        return;
    }

    for(i = 0; i < MMIMP3_LYRIC_DISPLAY_LINE_NUM; i++)
    {
        if(PNULL != display_ptr->lyric_display_sentence.lyric_sentence[i].lyric_content_ptr)
        {
            SCI_FREE(display_ptr->lyric_display_sentence.lyric_sentence[i].lyric_content_ptr);
            display_ptr->lyric_display_sentence.lyric_sentence[i].lyric_content_ptr = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description : Free the parsed data buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void FreeParseDataBuffer(
    void
)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    uint8 i = 0;



    if (PNULL == file_ptr)
    {
        return;
    }


    if(PNULL != file_ptr->time_tag_ptr)
    {
        SCI_FREE(file_ptr->time_tag_ptr);
        file_ptr->time_tag_ptr = PNULL;
    }

    for(i = 0; i < MMIMP3_LYRIC_SENTENCE_NUM / MMIMP3_LYRIC_SECTION_STC_NUM; i++)
    {
        if(PNULL != file_ptr->parsed_info_ptr[i])
        {
            SCI_FREE(file_ptr->parsed_info_ptr[i]);
            file_ptr->parsed_info_ptr[i] = PNULL;
        }
    }
}

/*****************************************************************************/
//  Description : Alloc the tag info buffer
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AllocTagInfoBuffer(
    void
)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();

    BOOLEAN     result = TRUE;


    if (PNULL == file_ptr)
    {
        return result;
    }


    //SCI_TRACE_LOW:"[MMIMP3]:lyric AllocTagInfoBuffer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_513_112_2_18_2_4_18_238, (uint8*)"");

    if(PNULL == file_ptr->time_tag_ptr)
    {
        file_ptr->time_tag_ptr = SCI_ALLOCA(MMIMP3_LYRIC_TAG_NUM * sizeof(MMIMP3_LYRIC_TIME_TAG_INFO_T));

        if(PNULL != file_ptr->time_tag_ptr)
        {
            SCI_MEMSET(file_ptr->time_tag_ptr, 0, (MMIMP3_LYRIC_TAG_NUM * sizeof(MMIMP3_LYRIC_TIME_TAG_INFO_T)));
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : Parse Lyric File
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseLyricFile(
    void
)
{
    BOOLEAN result = FALSE;
    uint16  tag_first_value[MMIMP3_LYRIC_TAG_LEN + 1] = {0};
    uint32  tag_first_len = MMIMP3_LYRIC_TAG_LEN + 1;
    uint16  tag_second_value[MMIMP3_LYRIC_TAG_LEN + 1] = {0};
    uint32  tag_second_len = MMIMP3_LYRIC_TAG_LEN + 1;
    uint32  tag_time = 0;
    uint32  sentence_offset = 0;
    uint32  sentence_len = 0;
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();


    if (PNULL == display_ptr)
    {
        return result;
    }

    AllocParseBuffer();
    AllocTagInfoBuffer();

    while(FindNextTag(tag_first_value, &tag_first_len, tag_second_value, &tag_second_len))
    {
        switch(CheckTagType(tag_first_value))
        {
        case MMIMP3_LYRIC_TYPE_TIME:
            if(CheckTimeTag(tag_first_value, tag_first_len, tag_second_value, tag_second_len, &tag_time))
            {
                SaveCurTag(tag_time);

                if(!TestNextLyricChar(LYRIC_LEFT_SQUARE_BRACKET))
                {
                    if(FindNextSentence(&sentence_offset, &sentence_len))
                    {
                        SaveCurSentence(sentence_offset, sentence_len);
                    }
                    else
                    {
                        SaveEmptySentence();    //' ',算作1句，不宜清理，否则会处理为歌词文件不存在
                    }
                }
                else
                {
                    SetMultiTag(TRUE);
                }
            }
            else if(IsMultiTagInLine())         //非法 tag
            {
                if(!TestNextLyricChar(LYRIC_LEFT_SQUARE_BRACKET))
                {
                    SetMultiTag(FALSE);

                    if(FindNextSentence(&sentence_offset, &sentence_len))
                    {
                        SaveCurSentence(sentence_offset, sentence_len);
                    }
                    else
                    {
                        SaveEmptySentence();    //' ',算作1句，不宜清理，否则会处理为歌词文件不存在
                    }
                }
            }

            break;

        case MMIMP3_LYRIC_TIME_OFFSET:
            SaveTimeOffset(tag_second_value, tag_second_len);
            break;

        default:
            break;
        }

        tag_first_len  = MMIMP3_LYRIC_TAG_LEN + 1;
        tag_second_len = MMIMP3_LYRIC_TAG_LEN + 1;
    }

    if(0 < GetLyricTagNum())
    {
        SortLyricTag();
        display_ptr->lyric_type = MMIMP3_LYRIC_PARSED;
        result = TRUE;
    }

    FreeParseBuffer();

    return (result);
}

/*****************************************************************************/
//  Description : Test Next Lyric Char
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TestNextLyricChar(uint16 lyric_char)
{
    BOOLEAN result = FALSE;

    if(!IsFileEnd())
    {
        if(lyric_char == GetNextLyricChar())
        {
            result = TRUE;
        }

        GoBackOneChar();
    }

    return result;
}

/*****************************************************************************/
//  Description : Save Cur Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveCurTag(uint32 tag_time)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();

    BOOLEAN result = FALSE;



    if (PNULL == file_ptr)
    {
        return result;
    }

    if(file_ptr->time_tag_num < MMIMP3_LYRIC_TAG_NUM)
    {
        file_ptr->time_tag_ptr[file_ptr->time_tag_num].time = tag_time;
        file_ptr->time_tag_ptr[file_ptr->time_tag_num].sentence_num = (file_ptr->sentence_num >= MMIMP3_LYRIC_SENTENCE_NUM) ? (MMIMP3_LYRIC_SENTENCE_NUM - 1) : (file_ptr->sentence_num);

        file_ptr->time_tag_num ++;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Save Cur Sentence
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveCurSentence(uint32 offset, uint32 len)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();

    BOOLEAN result = FALSE;
    uint32  file_section_num = 0;


    if (PNULL == file_ptr)
    {
        return result;
    }


    if(MMIMP3_LYRIC_SENTENCE_NUM > file_ptr->sentence_num)
    {
        file_ptr->sentence_num++;

        file_section_num = (file_ptr->sentence_num - 1) / (MMIMP3_LYRIC_SECTION_STC_NUM);

        if(PNULL == file_ptr->parsed_info_ptr[file_section_num])
        {
            //SCI_TRACE_LOW:"[MMIMP3] Alloc lyric section_id = %d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_668_112_2_18_2_4_19_239, (uint8*)"d", file_section_num);
            file_ptr->parsed_info_ptr[file_section_num] = SCI_ALLOC_APP(MMIMP3_LYRIC_SECTION_STC_NUM * sizeof(MMIMP3_LYRIC_PARSED_INFO_T));

            if(PNULL == file_ptr->parsed_info_ptr[file_section_num])
            {
                return FALSE;
            }

            SCI_MEMSET(file_ptr->parsed_info_ptr[file_section_num], 0, (MMIMP3_LYRIC_SECTION_STC_NUM * sizeof(MMIMP3_LYRIC_PARSED_INFO_T)));
        }

        (file_ptr->parsed_info_ptr[file_section_num] + (file_ptr->sentence_num - 1) % MMIMP3_LYRIC_SECTION_STC_NUM)->offset = offset;
        (file_ptr->parsed_info_ptr[file_section_num] + (file_ptr->sentence_num - 1) % MMIMP3_LYRIC_SECTION_STC_NUM)->length = len;

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Save Empty Sentence
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveEmptySentence(void)
{
    return SaveCurSentence(0, 0);
}

/*****************************************************************************/
//  Description : Save Time Offset
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveTimeOffset(uint16 *tag_offset_ptr, uint32 offset_len)
{
    BOOLEAN result = FALSE;
    uint8   asci_tag[MMIMP3_LYRIC_TAG_LEN + 1] = {0};
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();



    if (PNULL == display_ptr)
    {
        return result;
    }
    MMIAPICOM_WstrToStr(tag_offset_ptr, (uint8*)asci_tag);

    if(0 < offset_len)
    {
        display_ptr->lyric_time_offset = atol((char*)asci_tag);

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Sort Lyric Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SortLyricTag(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();


    if (PNULL == file_ptr)
    {
        return;
    }

    if(0 < GetLyricTagNum())
    {
        qsort(file_ptr->time_tag_ptr, file_ptr->time_tag_num, sizeof(MMIMP3_LYRIC_TIME_TAG_INFO_T), LyricTimeTagSort);
    }
}

/*****************************************************************************/
//  Description : Find Next Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindNextTag(
    uint16  *tag_value_ptr1,        //[OUT]
    uint32  *tag_len_ptr1,          //[IN&OUT]
    uint16  *tag_value_ptr2,        //[OUT]
    uint32  *tag_len_ptr2           //[IN&OUT]
)
{
    BOOLEAN result = FALSE;
    uint32  tag_len1 = 0;
    uint32  tag_len2 = 0;

    if(PNULL == tag_value_ptr1  ||
            PNULL == tag_value_ptr2  ||
            0     == *tag_len_ptr1   ||
            0     == *tag_len_ptr1)
    {
        //SCI_TRACE_LOW:"FindNextTag,tag_value_ptr1 = 0x%x,*tag_len_ptr1 = %d ,tag_value_ptr2 = 0x%x,*tag_len_ptr2 = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_746_112_2_18_2_4_19_240, (uint8*)"dddd", tag_value_ptr1, *tag_len_ptr1, tag_value_ptr2, *tag_len_ptr2);
        return FALSE;
    }

    while(FindLeftSquareBracket())
    {
        tag_len1 = *tag_len_ptr1;
        tag_len2 = *tag_len_ptr2;
        SCI_MEMSET(tag_value_ptr1, 0, *tag_len_ptr1 * sizeof(wchar));
        SCI_MEMSET(tag_value_ptr2, 0, *tag_len_ptr2 * sizeof(wchar));

        if(FindColon(tag_value_ptr1, &tag_len1) &&
                FindRightSquareBracket(tag_value_ptr2, &tag_len2))
        {
            result = TRUE;
            break;
        }
        else if(IsMultiTagInLine())
        {
            tag_len1 = *tag_len_ptr1;
            tag_len2 = 0;
            SCI_MEMSET(tag_value_ptr1, 0, *tag_len_ptr1 * sizeof(wchar));
            SCI_MEMSET(tag_value_ptr2, 0, *tag_len_ptr2 * sizeof(wchar));

            if(FindRightSquareBracket(tag_value_ptr1, &tag_len1))  //此处tag_value_ptr1无意义
            {
                result = TRUE;
                break;
            }
            else if(TestNextLyricChar(LYRIC_SENTENCE_END_CHAR_D) ||
                    TestNextLyricChar(LYRIC_SENTENCE_END_CHAR_A))
            {
                tag_len1 = 0;
                tag_len2 = 0;
                SCI_MEMSET(tag_value_ptr1, 0, *tag_len_ptr1 * sizeof(wchar));
                SCI_MEMSET(tag_value_ptr2, 0, *tag_len_ptr2 * sizeof(wchar));
                result = TRUE;
                break;
            }
        }
    }

    if(result)
    {
        *tag_len_ptr1 = tag_len1;
        *tag_len_ptr2 = tag_len2;
    }

    return result;
}

/*****************************************************************************/
//  Description : Find Left Square Bracket
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindLeftSquareBracket(void)
{
    BOOLEAN result = FALSE;

    while(!IsFileEnd())
    {
        if(LYRIC_LEFT_SQUARE_BRACKET == GetNextLyricChar())
        {
            result = TRUE;
            break;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : Find Colon
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindColon(
    uint16  *tag_value_ptr1,
    uint32  *tag_len_ptr1
)
{
    BOOLEAN result = FALSE;
    uint16  lyric_char = 0;
    uint32  tag_len = 0;
    BOOLEAN head_space = TRUE;

    while(!IsFileEnd())
    {
        lyric_char = GetNextLyricChar();

        if(LYRIC_COLON == lyric_char)
        {
            //if(tag_len >= LYRIC_TAG_MIN_LEN)      //长度下限不在此处判断，有:即TRUE
            {
                result = TRUE;
            }
            break;
        }
        else if(LYRIC_LEFT_SQUARE_BRACKET == lyric_char)
        {
            GoBackOneChar();
            result = FALSE;
            break;
        }
        else if(LYRIC_RIGHT_SQUARE_BRACKET == lyric_char)
        {
            GoBackOneChar();
            result = FALSE;
            break;
        }
        else if(LYRIC_SENTENCE_END_CHAR_D == lyric_char)
        {
            GoBackOneChar();
            result = FALSE;
            break;
        }
        else if(LYRIC_SENTENCE_END_CHAR_A == lyric_char)
        {
            GoBackOneChar();
            result = FALSE;
            break;
        }
        else
        {
            if(head_space)
            {
                if(LYRIC_SPACE != lyric_char)
                {
                    head_space = FALSE;
                }
            }

            if(!head_space)
            {
                if(tag_len < *tag_len_ptr1 - 1) //atol的存在，需要预留数组尾部
                {
                    *(tag_value_ptr1 + tag_len) = lyric_char;
                    tag_len ++;
                }
            }
        }
    }

    if(result)
    {
        *tag_len_ptr1 = tag_len;
    }

    return result;
}

/*****************************************************************************/
//  Description : Find Right Square Bracket
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindRightSquareBracket(
    uint16  *tag_value_ptr2,
    uint32  *tag_len_ptr2
)
{
    BOOLEAN result = FALSE;
    uint16  lyric_char = 0;
    uint32  tag_len = 0;
    BOOLEAN head_space = TRUE;

    while(!IsFileEnd())
    {
        lyric_char = GetNextLyricChar();

        if(LYRIC_RIGHT_SQUARE_BRACKET == lyric_char)
        {
            //if(tag_len >= LYRIC_TAG_MIN_LEN)      //长度无下限，比如 [by:]
            {
                result = TRUE;
            }
            break;
        }
        else if(LYRIC_LEFT_SQUARE_BRACKET == lyric_char)
        {
            GoBackOneChar();
            result = FALSE;
            break;
        }
        else if(LYRIC_SENTENCE_END_CHAR_D == lyric_char)
        {
            GoBackOneChar();
            result = FALSE;
            break;
        }
        else if(LYRIC_SENTENCE_END_CHAR_A == lyric_char)
        {
            GoBackOneChar();
            result = FALSE;
            break;
        }
        else
        {
            if(head_space)
            {
                if(LYRIC_SPACE != lyric_char)
                {
                    head_space = FALSE;
                }
            }

            if(!head_space)
            {
                if(tag_len < *tag_len_ptr2 - 1)     //atol的存在，需要预留数组尾部
                {
                    *(tag_value_ptr2 + tag_len) = lyric_char;
                    tag_len ++;
                }
            }
        }
    }

    if(result)
    {
        *tag_len_ptr2 = tag_len;
    }

    return result;
}

/*****************************************************************************/
//  Description : Check Tag Type
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_LYRIC_TAG_TYPE_E CheckTagType(
    uint16  *lyric_tag_ptr
)
{
    MMIMP3_LYRIC_TAG_TYPE_E  tag_type = MMIMP3_LYRIC_TYPE_NONE;
    uint8  asci_tag[MMIMP3_LYRIC_TAG_LEN + 1] = {0};

    MMIAPICOM_WstrToStr(lyric_tag_ptr, (uint8*)asci_tag);

    if(0 == MMIAPICOM_Stricmp((uint8*)MMIMP3_LYRIC_ARTIST, asci_tag))
    {
        tag_type = MMIMP3_LYRIC_SONG_ARTIST;
    }
    else if(0 == MMIAPICOM_Stricmp((uint8*)MMIMP3_LYRIC_NAME, asci_tag))
    {
        tag_type = MMIMP3_LYRIC_SONG_NAME;
    }
    else if(0 == MMIAPICOM_Stricmp((uint8*)MMIMP3_LYRIC_TRACK, asci_tag))
    {
        tag_type = MMIMP3_LYRIC_TRACK_NAME;
    }
    else if(0 == MMIAPICOM_Stricmp((uint8*)MMIMP3_LYRIC_AUTHER, asci_tag))
    {
        tag_type = MMIMP3_LYRIC_AUTHER_NAME;
    }
    else if(0 == MMIAPICOM_Stricmp((uint8*)MMIMP3_LYRIC_OFFSET, asci_tag))
    {
        tag_type = MMIMP3_LYRIC_TIME_OFFSET;
    }
    else
    {
        tag_type = MMIMP3_LYRIC_TYPE_TIME;
    }

    return (tag_type);
}


/*****************************************************************************/
//  Description : Find Digital Part
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindDigitalPart(
    uint16  *tag_value_ptr,         //[IN]
    uint32  tag_len,                //[IN]
    uint16  *integer_value_ptr,     //[OUT]
    uint32  *integer_len_ptr,       //[OUT]
    uint16  *decimal_value_ptr,     //[OUT]
    uint32  *decimal_len_ptr        //[OUT]
)
{
    BOOLEAN result = TRUE;
    BOOLEAN is_integer_part = TRUE;
    uint32 integer_len = 0;
    uint32 decimal_len = 0;
    uint32 i = 0;

    if(PNULL == tag_value_ptr      ||
            PNULL == integer_value_ptr  ||
            PNULL == decimal_value_ptr  ||
            tag_len >= *integer_len_ptr ||
            tag_len >= *decimal_len_ptr)
    {
        //SCI_TRACE_LOW:"FindDigitalPart, input error, tag_value_ptr = %x, tag_len = %d,integer_value_ptr = 0x%x,*integer_len_ptr = %d ,decimal_value_ptr = 0x%x,*decimal_len_ptr = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_1029_112_2_18_2_4_19_241, (uint8*)"dddddd", tag_value_ptr, tag_len, integer_value_ptr, *integer_len_ptr, decimal_value_ptr, *decimal_len_ptr);
        return FALSE;
    }

    if(LYRIC_TAG_MIN_LEN > tag_len)
    {
        return FALSE;
    }

    for(i = 0; i < tag_len; i++)
    {
        if(is_integer_part)
        {
            if(LYRIC_NUMBER_ZERO <= tag_value_ptr[i] &&
                    LYRIC_NUMBER_NINE >= tag_value_ptr[i])
            {
                integer_value_ptr[integer_len++] = tag_value_ptr[i];
            }
            else if(LYRIC_POINT == tag_value_ptr[i])
            {
                is_integer_part = FALSE;

                if(integer_len < LYRIC_TAG_MIN_LEN)
                {
                    result = FALSE;
                    break;
                }
            }
            else
            {
                if(integer_len < LYRIC_TAG_MIN_LEN)
                {
                    result = FALSE;
                    break;
                }
                else
                {
                    result = TRUE;
                    break;
                }
            }
        }
        else
        {
            if(LYRIC_NUMBER_ZERO <= tag_value_ptr[i] &&
                    LYRIC_NUMBER_NINE >= tag_value_ptr[i])
            {
                decimal_value_ptr[decimal_len++] = tag_value_ptr[i];

                if(decimal_len >= LYRIC_TAG_DECIMAL_MAX_LEN)
                {
                    result = TRUE;
                    break;
                }
            }
            else
            {
                result = TRUE;
                break;
            }
        }
    }

    if(result)
    {
        *integer_len_ptr = integer_len;
        *decimal_len_ptr = decimal_len;
    }

    return result;
}


/*****************************************************************************/
//  Description : Check Time Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckTimeTag(
    uint16  *tag_value_ptr1,    //[IN]
    uint32  tag_value_len1,     //[IN]
    uint16  *tag_value_ptr2,    //[IN]
    uint32  tag_value_len2,     //[IN]
    uint32  *tag_time_ptr       //[OUT]
)
{
    BOOLEAN result = FALSE;
    uint16  integer_part[MMIMP3_LYRIC_TAG_LEN + 1] = {0};
    uint32  integer_len = MMIMP3_LYRIC_TAG_LEN + 1;
    uint16  decimal_part[MMIMP3_LYRIC_TAG_LEN + 1] = {0};
    uint32  decimal_len = MMIMP3_LYRIC_TAG_LEN + 1;
    uint32  minute_time = 0;
    uint32  second_time = 0;
    uint32  i = 0;
    uint8   asci_integer_part[MMIMP3_LYRIC_TAG_LEN + 1] = {0};
    uint8   asci_decimal_part[MMIMP3_LYRIC_TAG_LEN + 1] = {0};

    if(FindDigitalPart(tag_value_ptr1, tag_value_len1, integer_part, &integer_len, decimal_part, &decimal_len))
    {
        /*  //无需支持小数
        if(0 < decimal_len)
        {
            minute_time = atol((char*)decimal_part)*60*1000;
            for(i = 0; i < decimal_len; i++)
            {
                minute_time /= LYRIC_DECIMAL_BASE;
            }
        }
        */
        if(0 < integer_len)
        {
            MMIAPICOM_WstrToStr(integer_part, (uint8*)asci_integer_part);
            minute_time += (uint32)atol((char*)asci_integer_part) * 60 * 1000;
        }

        integer_len = MMIMP3_LYRIC_TAG_LEN + 1;
        decimal_len = MMIMP3_LYRIC_TAG_LEN + 1;
        SCI_MEMSET(integer_part, 0, integer_len * sizeof(wchar));
        SCI_MEMSET(decimal_part, 0, decimal_len * sizeof(wchar));
        SCI_MEMSET(asci_integer_part, 0, MMIMP3_LYRIC_TAG_LEN + 1);
        SCI_MEMSET(asci_decimal_part, 0, MMIMP3_LYRIC_TAG_LEN + 1);

        if(FindDigitalPart(tag_value_ptr2, tag_value_len2, integer_part, &integer_len, decimal_part, &decimal_len))
        {
            if(0 < decimal_len)
            {
                MMIAPICOM_WstrToStr(decimal_part, (uint8*)asci_decimal_part);
                second_time = (uint32)atol((char*)asci_decimal_part) * 1000;

                for(i = 0; i < decimal_len; i++)
                {
                    second_time /= LYRIC_DECIMAL_BASE;
                }
            }

            if(0 < integer_len)
            {
                MMIAPICOM_WstrToStr(integer_part, (uint8*)asci_integer_part);
                second_time += (uint32)atol((char*)asci_integer_part) * 1000;
            }

            result = TRUE;
        }
    }

    *tag_time_ptr = minute_time + second_time;

    return result;
}


/*****************************************************************************/
//  Description : Find Next Sentence
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN FindNextSentence(uint32 *offset_ptr, uint32 *len_ptr)
{
    BOOLEAN result = FALSE;
    uint16  cur_wchar_pos = 0;
    uint32  sentence_start_offset = 0;
    uint32  sentence_end_offset = 0;
    uint16  lyric_char = 0;
    BOOLEAN head_space = TRUE;
    uint32  sentence_unicode_offset = 0;
    uint32  sentence_unicode_num = 0;
    BOOLEAN is_new_data_read = FALSE;
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();



    if (PNULL == file_ptr)
    {
        return result;
    }

    sentence_start_offset = file_ptr->temp_buf_start_offset;
    cur_wchar_pos = GetLyricCurWcharPos();

    if(cur_wchar_pos > 0)
    {
        if(cur_wchar_pos < file_ptr->temp_buf_unicode_num)
        {
            sentence_start_offset += GetOtherCodeLenFromUsc(
                                         file_ptr->file_code_type,
                                         file_ptr->temp_unicode_buf_ptr,
                                         cur_wchar_pos
                                     );  //0 != cur_wchar_pos
            is_new_data_read = FALSE;

        }
    }

    while(!IsFileEnd())
    {
        if(GetLyricCurWcharPos() == file_ptr->temp_buf_unicode_num)
        {
            is_new_data_read = TRUE;
            sentence_end_offset = file_ptr->temp_buf_end_offset;
        }

        lyric_char = GetNextLyricChar();

        if(LYRIC_SENTENCE_END_CHAR_D == lyric_char)
        {
            break;
        }

        if(LYRIC_SENTENCE_END_CHAR_A == lyric_char)
        {
            GoBackOneChar();
            break;
        }

        if(LYRIC_LEFT_SQUARE_BRACKET == lyric_char)
        {
            GoBackOneChar();
            break;
        }
        else
        {
            //清理前端空格
            if(head_space)
            {
                if(LYRIC_SPACE != lyric_char)
                {
                    if(sentence_unicode_offset > 0)
                    {
                        sentence_start_offset += GetOtherCodeLenFromUsc(
                                                     file_ptr->file_code_type,
                                                     file_ptr->temp_unicode_buf_ptr + cur_wchar_pos,
                                                     sentence_unicode_offset
                                                 );
                    }

                    head_space = FALSE;
                }
                else
                {
                    if(is_new_data_read)
                    {
                        sentence_start_offset = file_ptr->temp_buf_start_offset;
                        sentence_unicode_offset = 0;
                        cur_wchar_pos = 0;
                        is_new_data_read = FALSE;
                    }

                    sentence_unicode_offset++;
                }
            }

            if(!head_space)
            {
                if(sentence_unicode_num < MMIMP3_LYRIC_SENTENCE_MAX_LEN - 1)   //需要预留数组尾部
                {
                    if(is_new_data_read)
                    {
                        //sentence_end_offset = file_ptr->temp_buf_end_offset;
                        is_new_data_read = FALSE;
                    }

                    sentence_unicode_num++;
                }
                else
                {
                    break;
                }
            }
        }
    }

    if(0 < sentence_unicode_num)
    {
        if(GetLyricCurWcharPos() > 0)
        {
            if(GetLyricCurWcharPos() == file_ptr->temp_buf_unicode_num)
            {
                sentence_end_offset = file_ptr->temp_buf_end_offset;
            }
            else
            {
                sentence_end_offset = 0;
                sentence_end_offset += GetOtherCodeLenFromUsc(
                                           file_ptr->file_code_type,
                                           file_ptr->temp_unicode_buf_ptr,
                                           GetLyricCurWcharPos()
                                       );

                sentence_end_offset += file_ptr->temp_buf_start_offset; //更新偏移
            }
        }

        if(sentence_end_offset >= sentence_start_offset)
        {
            *offset_ptr = sentence_start_offset;
            *len_ptr    = sentence_end_offset - sentence_start_offset + 1;
        }
        else
        {
            *offset_ptr = 0;
            *len_ptr    = 0;
            sentence_unicode_num = 0;
        }
    }

    if(0 < sentence_unicode_num)
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Get Next Lyric Char
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNextLyricChar(void)
{
    uint16       cur_char = 0;
    BOOLEAN      file_read_result = FALSE;
    uint32       read_size = 0;
    uint32       read_offset = 0;
    uint32       unicode_num = 0;
    uint32       other_code_len = 0;
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();


    if (PNULL == file_ptr)
    {
        return cur_char;
    }

    if(!IsFileEnd())
    {
        if((file_ptr->is_buf_parse_finished/* || 0 == file_ptr->cur_wchar_pos*/) && !file_ptr->is_read_finished) // modified by CR247908 2011.06.14
        {

            if(0 == file_ptr->file_read_offset)
            {
                read_offset = file_ptr->file_head_len;
            }
            else
            {
                read_offset = file_ptr->file_read_offset + 1;
            }

            if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_ptr->file_handle, read_offset, SFS_SEEK_BEGIN))
            {
                if(file_ptr->file_len > read_offset + MMIMP3_LYRIC_TEMP_DATA_LEN)
                {
                    read_size = MMIMP3_LYRIC_TEMP_DATA_LEN;
                }
                else
                {
                    if(file_ptr->file_len > read_offset)
                    {
                        read_size = file_ptr->file_len - read_offset;
                    }

                    file_ptr->is_read_finished = TRUE;
                }

                file_ptr->file_read_offset = read_offset + read_size - 1;

                SCI_MEMSET(file_ptr->temp_parse_buf_ptr, 0, MMIMP3_LYRIC_TEMP_DATA_LEN);

                if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_ptr->file_handle, file_ptr->temp_parse_buf_ptr, read_size, &read_size, NULL))
                {
                    file_read_result = TRUE;
                }
            }

            if(file_read_result)
            {
                if(file_ptr->is_buf_parse_finished)
                {
                    file_ptr->cur_wchar_pos = 0;
                    file_ptr->is_buf_parse_finished = FALSE;
                }

                SCI_MEMSET(file_ptr->temp_unicode_buf_ptr, 0, MMIMP3_LYRIC_TEMP_WCHAR_LEN * sizeof(wchar));

                unicode_num = GUI_OtherCodeToWstr(
                                  file_ptr->temp_unicode_buf_ptr,
                                  MMIMP3_LYRIC_TEMP_WCHAR_LEN,
                                  file_ptr->file_code_type,
                                  file_ptr->temp_parse_buf_ptr,
                                  read_size
                              );

                other_code_len = GetOtherCodeLenFromUsc(file_ptr->file_code_type, file_ptr->temp_unicode_buf_ptr, unicode_num);

                file_ptr->temp_buf_start_offset = read_offset;
                file_ptr->temp_buf_end_offset   = read_offset + other_code_len - 1;
                file_ptr->temp_buf_unicode_num  = unicode_num;

                if(0 == file_ptr->temp_buf_unicode_num)
                {
                    file_ptr->is_buf_parse_finished = TRUE;
                }
            }
        }
        else
        {
            file_read_result = TRUE;
        }

        if(file_read_result)
        {
            if(!file_ptr->is_buf_parse_finished)
            {
                cur_char = file_ptr->temp_unicode_buf_ptr[file_ptr->cur_wchar_pos];
                file_ptr->cur_wchar_pos++;

                if(file_ptr->cur_wchar_pos == file_ptr->temp_buf_unicode_num)
                {
                    file_ptr->is_buf_parse_finished = TRUE;
                }
            }
        }
        else
        {
            file_ptr->temp_buf_end_offset = file_ptr->file_len - 1;
            MMIMP3_FatalError();
        }
    }

    return (cur_char);
}

/*****************************************************************************/
//  Description : Get other code len
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint16 GetOtherCodeLenFromUsc(
    GUI_CHAR_CODE_TYPE_E  code_type,
    const wchar           *wstr_ptr,
    uint16                wstr_len
)
{
    uint32       other_code_len = 0;
    uint8        *other_code_buf = PNULL;
    uint32       other_code_buf_len = 0;

    if(wstr_len > MMIMP3_LYRIC_TEMP_WCHAR_LEN)
    {
        //SCI_TRACE_LOW:"[MMIMP3]:GetOtherCodeLenFromUsc,wstr_len = %d,ERROR!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_1444_112_2_18_2_4_21_242, (uint8*)"d", wstr_len);
        return 0;
    }

    other_code_buf_len = (wstr_len + 1) * 4;
    other_code_buf = SCI_ALLOC_APP(other_code_buf_len);

    if(PNULL == other_code_buf)
    {
        return 0;
    }

    SCI_MEMSET(other_code_buf, 0, other_code_buf_len);

    other_code_len = GUI_WstrToOtherCode(
                         code_type,
                         other_code_buf,
                         other_code_buf_len,
                         wstr_ptr,
                         wstr_len
                     );

    SCI_FREE(other_code_buf);
    other_code_buf = PNULL;

    return other_code_len;
}

/*****************************************************************************/
//  Description : Set Multi Tag
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void SetMultiTag(BOOLEAN is_multi_tag_line)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();

    if (PNULL != file_ptr)
    {
        file_ptr->is_multi_tag_line = is_multi_tag_line;
    }
}

/*****************************************************************************/
//  Description : Is Multi Tag In Line
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMultiTagInLine(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    BOOLEAN is_multi_tag_line = FALSE;


    if (PNULL != file_ptr)
    {
        is_multi_tag_line = file_ptr->is_multi_tag_line;
    }

    return is_multi_tag_line;
}

/*****************************************************************************/
//  Description : Go Back One Char
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GoBackOneChar(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();


    if (PNULL == file_ptr)
    {
        return FALSE;
    }

    if(file_ptr->cur_wchar_pos > 0)
    {
        file_ptr->cur_wchar_pos --;
        return TRUE;
    }

    //无else

    return FALSE;
}

/*****************************************************************************/
//  Description : Is File End
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFileEnd(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();


    if (PNULL == file_ptr)
    {
        return FALSE;
    }


    if(file_ptr->file_len == 0)
    {
        return FALSE;
    }
    else if(file_ptr->is_read_finished &&
            file_ptr->is_buf_parse_finished)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Get Lyric Cur Pos
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricCurWcharPos(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    uint32 cur_wchar_pos = 0;


    if (PNULL != file_ptr)
    {
        cur_wchar_pos = file_ptr->cur_wchar_pos;
    }

    return cur_wchar_pos;
}

/*****************************************************************************/
//  Description : Get text file type
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLyricTextFileType(//type
    MMIFILE_HANDLE file_handle, // in
    GUI_CHAR_CODE_TYPE_E *file_type // out
)
{
#if 1
    GUI_CHAR_CODE_TYPE_E type = GUI_CHAR_CODE_ANSI;
    uint32 len = 1024;
    MMIFILE_ERROR_E file_read_error = SFS_NO_ERROR;
    uint8*          data_str_ptr = PNULL;

    if(PNULL == file_handle || PNULL == file_type)
    {
        return FALSE;
    }

    data_str_ptr = (uint8 *)SCI_ALLOCA(len + 1);

    if(PNULL == data_str_ptr)
    {
        return FALSE;
    }

    MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);
    file_read_error = MMIAPIFMM_ReadFile(file_handle, data_str_ptr, len, &len, PNULL);

    if(file_read_error != SFS_NO_ERROR)
    {
        SCI_FREE(data_str_ptr);
        return FALSE;
    }

    type = GUI_GetCodeType(data_str_ptr, len);

    if(GUI_CHAR_CODE_ANSI == type)
    {
        if(MMIMP3_TXT_CODE_TYPE_BIG5 == APSETTING_GetCodeType())
        {
            type = GUI_CHAR_CODE_ANSI_BIG5;
        }
    }
    else if(GUI_CHAR_CODE_ANSI_BIG5 == type)
    {
        if(MMIMP3_TXT_CODE_TYPE_GB == APSETTING_GetCodeType())
        {
            type = GUI_CHAR_CODE_ANSI;
        }
    }
    else if(GUI_CHAR_CODE_UCS2_BE == type)
    {
        type = GUI_CHAR_CODE_UCS2_LE;
    }

    *file_type = type;

    //SCI_TRACE_LOW:"[MMIMP3]: GetLyricTextFileType, type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_1589_112_2_18_2_4_21_243, (uint8*)"d", type);

    SCI_FREE(data_str_ptr);

    return TRUE;
#else
    BOOLEAN result = TRUE;
    uint8 head[MMIMP3_LYRIC_FILE_HEAD_MAX_LEN] = {0};
    GUI_CHAR_CODE_TYPE_E type = GUI_CHAR_CODE_ANSI;
    uint32 len = 0;
    MMIFILE_ERROR_E file_read_error = SFS_NO_ERROR;

    if(PNULL == file_handle || PNULL == file_type)
    {
        return FALSE;
    }

    len =   MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);

    file_read_error = MMIAPIFMM_ReadFile(file_handle, head, MMIMP3_LYRIC_FILE_HEAD_MAX_LEN, &len, PNULL);

    if(file_read_error != SFS_NO_ERROR ||
            MMIMP3_LYRIC_FILE_HEAD_MAX_LEN != len)
    {
        result = FALSE;
    }

    if(0xFE == head[0] && 0xFF == head[1])
    {
        type = GUI_CHAR_CODE_UCS2_BE;
    }
    else if(0xFF == head[0] && 0xFE == head[1])
    {
        type = GUI_CHAR_CODE_UCS2_LE;
    }
    else if(0xEF == head[0] && 0xBB == head[1] && 0xBF == head[2])
    {
        type = GUI_CHAR_CODE_UTF8;
    }
    else
    {
        if(MMIMP3_TXT_CODE_TYPE_BIG5 == APSETTING_GetCodeType())
        {
            type = GUI_CHAR_CODE_ANSI_BIG5;
        }
        else
        {
            type = GUI_CHAR_CODE_ANSI;
        }
    }

    *file_type = type;

    //SCI_TRACE_LOW:"[MMIMP3]: GetLyricTextFileType, type = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_1642_112_2_18_2_4_21_244, (uint8*)"dd", type, result);

    return result;
#endif
}

/*****************************************************************************/
//  Description : Get text file head len
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLyricTextFileHeadLen(
    GUI_CHAR_CODE_TYPE_E lyric_code_type
)
{
    uint16 len = 0;

    switch(lyric_code_type)
    {
    case GUI_CHAR_CODE_ANSI:
    case GUI_CHAR_CODE_ANSI_BIG5:
        len = MMIMP3_LYRIC_FILE_ASCI_HEAD_LEN;
        break;

    case GUI_CHAR_CODE_UCS2_BE:
    case GUI_CHAR_CODE_UCS2_LE:
        len = MMIMP3_LYRIC_FILE_UNICODE_HEAD_LEN;
        break;

    case GUI_CHAR_CODE_UTF8:
        len = MMIMP3_LYRIC_FILE_UTF8_HEAD_LEN;
        break;

    default:
        break;
    }

    return len;
}

/*****************************************************************************/
//  Description : Get Lyric Tag Num
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricTagNum(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    uint32 time_tag_num = 0;


    if (PNULL != file_ptr)
    {
        time_tag_num = file_ptr->time_tag_num;
    }

    return time_tag_num;
}

/*****************************************************************************/
//  Description : Get Lyric Sentence Num
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricSentenceNum(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    uint32 sentence_num = 0;



    if (PNULL != file_ptr)
    {
        sentence_num = file_ptr->sentence_num;
    }

    return sentence_num;
}

/*****************************************************************************/
//  Description : Read the lyric sentence in sequence.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadLyricSentence(
    uint32 sentence_index,
    uint32 *array_index_ptr
)
{
    uint8    *lyric_sentence_data = PNULL;
    uint16   lyric_sentence[MMIMP3_LYRIC_SENTENCE_MAX_LEN + 1] = {0};
    uint32   lyric_sentence_len = {0};
    uint32   offset = 0;
    uint32   read_size = 0;
    uint32   next_pos = 0;
    BOOLEAN  is_sentence_read = FALSE;
    BOOLEAN  file_read_result = FALSE;
    uint8    i = 0;
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();


    if (PNULL == file_ptr
            || PNULL == display_ptr
       )
    {
        return file_read_result;
    }

    for(i = 0; i < MMIMP3_LYRIC_DISPLAY_LINE_NUM; i++)
    {
        if(sentence_index == display_ptr->lyric_display_sentence.lyric_sentence[i].sentence_id &&
                0 < display_ptr->lyric_display_sentence.lyric_sentence[i].length)
        {
            is_sentence_read = TRUE;
            *array_index_ptr = i;
            break;
        }
    }

    if(!is_sentence_read)
    {
        next_pos = display_ptr->lyric_display_sentence.next_vacant_pos;

        display_ptr->lyric_display_sentence.next_vacant_pos = (next_pos + 1) % MMIMP3_LYRIC_DISPLAY_LINE_NUM;
        *array_index_ptr = next_pos;

        if(display_ptr->lyric_display_sentence.sentence_num < MMIMP3_LYRIC_DISPLAY_LINE_NUM)
        {
            display_ptr->lyric_display_sentence.sentence_num = (display_ptr->lyric_display_sentence.sentence_num + 1) % MMIMP3_LYRIC_DISPLAY_LINE_NUM;
        }

        offset    = (file_ptr->parsed_info_ptr[sentence_index / MMIMP3_LYRIC_SECTION_STC_NUM] + sentence_index % MMIMP3_LYRIC_SECTION_STC_NUM)->offset;
        read_size = (file_ptr->parsed_info_ptr[sentence_index / MMIMP3_LYRIC_SECTION_STC_NUM] + sentence_index % MMIMP3_LYRIC_SECTION_STC_NUM)->length;

        if(0 < read_size && read_size < (MMIMP3_LYRIC_SENTENCE_MAX_LEN << 3)) //不同格式可能数倍长度
        {
            if(offset     < file_ptr->file_len &&
                    read_size  < file_ptr->file_len &&
                    offset <= file_ptr->file_len - read_size &&  //【MS00215637 和溢出】 offset + read_size <= file_ptr->file_len &&
                    SFS_INVALID_HANDLE != file_ptr->file_handle)
            {
                lyric_sentence_data = SCI_ALLOCA(read_size + 1);

                if(PNULL != lyric_sentence_data)
                {
                    SCI_MEMSET(lyric_sentence_data, 0, read_size + 1);

                    if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(
                                file_ptr->file_handle,
                                offset,
                                SFS_SEEK_BEGIN
                            ))
                    {
                        if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(
                                    file_ptr->file_handle,
                                    lyric_sentence_data,
                                    read_size,
                                    &read_size,
                                    NULL
                                ))
                        {

                            lyric_sentence_len = GUI_OtherCodeToWstr(
                                                     lyric_sentence,
                                                     MMIMP3_LYRIC_SENTENCE_MAX_LEN + 1,
                                                     file_ptr->file_code_type,
                                                     lyric_sentence_data,
                                                     read_size
                                                 );

                            file_read_result = TRUE;
                        }
                    }
                }

                if(PNULL != lyric_sentence_data)
                {
                    SCI_FREE(lyric_sentence_data);
                    lyric_sentence_data = PNULL;
                }
            }
        }
        else
        {
            lyric_sentence[0] = LYRIC_SPACE;
            lyric_sentence_len = 1;
            file_read_result = TRUE;
        }

        if(file_read_result)
        {
            if(PNULL != display_ptr->lyric_display_sentence.lyric_sentence[next_pos].lyric_content_ptr)
            {
                SCI_FREE(display_ptr->lyric_display_sentence.lyric_sentence[next_pos].lyric_content_ptr);
            }

            display_ptr->lyric_display_sentence.lyric_sentence[next_pos].lyric_content_ptr = SCI_ALLOCA((lyric_sentence_len + 1) * sizeof(wchar));

            if(PNULL != display_ptr->lyric_display_sentence.lyric_sentence[next_pos].lyric_content_ptr)
            {
                SCI_MEMSET(display_ptr->lyric_display_sentence.lyric_sentence[next_pos].lyric_content_ptr, 0, ((lyric_sentence_len + 1) * sizeof(wchar)));

                MMI_WSTRNCPY(
                    display_ptr->lyric_display_sentence.lyric_sentence[next_pos].lyric_content_ptr, lyric_sentence_len + 1,
                    lyric_sentence, lyric_sentence_len, lyric_sentence_len);

                display_ptr->lyric_display_sentence.lyric_sentence[next_pos].length = lyric_sentence_len;
                display_ptr->lyric_display_sentence.lyric_sentence[next_pos].sentence_id = sentence_index;
            }
        }
    }
    else
    {
        file_read_result = TRUE;
    }

    /*
    //SCI_TRACE_LOW:"[MMIMP3]:ReadLyricSentence, exit, sentence_index = %d, next_pos =%d, num = %d, sentence_id = %d, %d, %d, %d, %d ,%d, %d,length = %d, array_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMP3_LYRIC_1836_112_2_18_2_4_21_245,(uint8*)"dddddddddddd",
        sentence_index,
        display_ptr->lyric_display_sentence.next_vacant_pos,
        display_ptr->lyric_display_sentence.sentence_num,
        display_ptr->lyric_display_sentence.lyric_sentence[0].sentence_id,
        display_ptr->lyric_display_sentence.lyric_sentence[1].sentence_id,
        display_ptr->lyric_display_sentence.lyric_sentence[2].sentence_id,
        display_ptr->lyric_display_sentence.lyric_sentence[3].sentence_id,
        display_ptr->lyric_display_sentence.lyric_sentence[4].sentence_id,
        display_ptr->lyric_display_sentence.lyric_sentence[5].sentence_id,
        display_ptr->lyric_display_sentence.lyric_sentence[6].sentence_id,
        sentence_len,
        array_index_ptr);
    */
    if(!file_read_result)
    {
        MMIMP3_FatalError();
    }

    return file_read_result;
}

/*****************************************************************************/
//  Description : Get the lyric sentence in sequence.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL MMIMP3_LYRIC_SENTENCE_T *GetLyricSentence(
    uint32 sentence_index
)
{
    uint32 array_index = 0;
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();
    MMIMP3_LYRIC_SENTENCE_T *sentence_ptr = PNULL;


    if (PNULL == display_ptr)
    {
        return sentence_ptr;
    }

    ReadLyricSentence(sentence_index, &array_index);

    //SCI_TRACE_LOW("[MMIMP3]:GetLyricSentence, sentence_index = %d, array_index = %d",sentence_index,array_index);

    return &display_ptr->lyric_display_sentence.lyric_sentence[array_index];
}

/*****************************************************************************/
//  Description : Reset the displaying Sentence Info.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
/*
LOCAL void ResetLyricDisplaySentenceInfo(
                void
                )
{
    FreeLyricSentenceBuffer();
    SCI_MEMSET(&display_ptr->lyric_display_sentence, 0, sizeof(MMIMP3_LYRIC_DISPLAY_SENTENCE_INFO_T));
}
*/
/*****************************************************************************/
//  Description : start mp3 lyric asynchronously
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_StartLyric(
    void
)
{
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};

    if(MMIMP3_PLAYER_AUDIO_ACTIVE == MMIAPIMP3_GetPlayerState())
    {
        if(IsLyricFileOpened())
        {
            if(ParseLyricFile())
            {
                MMIMP3_GetPlayingDynInfo(&dyn_info, TRUE);

                if(0 < dyn_info.cur_offset)
                {
                    MMIMP3_SeekLyric(dyn_info.cur_offset, FALSE);
                }
                else
                {
                    InitLyricDispCtrl();
                    StartLyricTimer();
                }


                MMIMP3_HandleLyricBackLight(TRUE);
                {
                    MMIAP_LYRIC_WIN_CALLBACK_PARAM_T req_param = {0};

                    req_param.is_update_lyric = TRUE;
                    req_param.is_update_lyric_now = TRUE;
                    MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN, MMIAPWIN_FLAG_FUNC_LYRIC, &req_param, sizeof(req_param));
                }
            }
            else
            {
                CloseLyricFile();
                FreeLyricBuffer();
                InitLyric();
            }
        }
    }
}

/*****************************************************************************/
//  Description : play mp3 lyric
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_OpenLyric(
    void
)
{
    BOOLEAN   result = FALSE;
    uint16    cur_index = MMIMP3_GetCurPlayFileIndex();

    //InitLyric();
    MMIMP3_StopLyric();

    if(OpenLyricFile(cur_index))
    {
        //if(ParseLyricFile())
        {
            MMIAP_LYRIC_WIN_CALLBACK_PARAM_T req_param = {0};

            req_param.is_update_lyric = TRUE;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_VIRTUAL, MMIAPWIN_FLAG_FUNC_LYRIC, &req_param, sizeof(req_param));

            result = TRUE;
        }
    }

    if(!result)
    {
        CloseLyricFile();
        FreeLyricBuffer();
        InitLyric();
    }

    return result;
}

/*****************************************************************************/
//  Description : open lyric file,set lyric file info
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenLyricFile(
    uint16      cur_index
)
{
    BOOLEAN          result = FALSE;
    uint16           lyric_full_path[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    uint32           file_size = 0;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();
    wchar *file_dev_ptr = PNULL;
    
    

    if (PNULL == file_ptr
            || PNULL == display_ptr
       )
    {
        return result;
    }

    //get lyric file name
    if(!GetLyricFileFullPathName(lyric_full_path))
    {
        //SCI_TRACE_LOW:"[MMIMP3]: OpenLyricFile: No music file!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_1969_112_2_18_2_4_22_246, (uint8*)"");
        return FALSE;
    }

    file_ptr->file_handle = MMIAPIFMM_CreateFile((const uint16*)lyric_full_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, 0, 0);

    if(file_ptr->file_handle == SFS_INVALID_HANDLE)
    {
        //遍历所有DEV
        for(dev = MMI_DEVICE_NUM - 1 ; dev >= MMI_DEVICE_UDISK; dev--)
        {
            file_dev_ptr = MMIAPIFMM_GetDevicePath(dev);
            if(PNULL != file_dev_ptr)
            {
                lyric_full_path[0] = *file_dev_ptr;
            }
            file_ptr->file_handle = MMIAPIFMM_CreateFile((const uint16*)lyric_full_path, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, 0, 0);

            if(SFS_INVALID_HANDLE != file_ptr->file_handle)
            {
                result = TRUE;
                break;
            }
        }
    }
    else
    {
        result = TRUE;
    }


    if(!result)
    {
        return FALSE;//not exist!
    }

    file_size = MMIAPIFMM_GetFileSize(file_ptr->file_handle);

    if(file_size <= MMIMP3_LYRIC_FILE_MAX_SIZE && file_size > 0)
    {
        file_ptr->file_len      = file_size;
        display_ptr->lyric_type = MMIMP3_LYRIC_EXIST;

        if(GetLyricTextFileType(file_ptr->file_handle, &file_ptr->file_code_type))
        {
            file_ptr->file_head_len = GetLyricTextFileHeadLen(file_ptr->file_code_type);
        }
        else
        {
            result = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : close lyric file
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void CloseLyricFile(
    void
)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();


    if (PNULL == file_ptr)
    {
        return ;
    }

    if(SFS_INVALID_HANDLE != file_ptr->file_handle)
    {
        MMIAPIFMM_CloseFile(file_ptr->file_handle);
        file_ptr->file_handle = SFS_INVALID_HANDLE;
    }
}

/*****************************************************************************/
//  Description : is lyric file opened
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsLyricFileOpened(void)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    BOOLEAN is_file_opened = FALSE;


    if (PNULL != file_ptr)
    {
        is_file_opened = SFS_INVALID_HANDLE != file_ptr->file_handle;
    }

    return is_file_opened;
}

/*****************************************************************************/
//  Description : Alloc lyric parse buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AllocParseBuffer(
    void
)
{
    BOOLEAN     result = TRUE;
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();


    if (PNULL == file_ptr)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"[MMIMP3]: lyric AllocParseBuffer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_2055_112_2_18_2_4_22_247, (uint8*)"");

    if(PNULL == file_ptr->temp_unicode_buf_ptr)
    {
        file_ptr->temp_unicode_buf_ptr = SCI_ALLOC_APP(MMIMP3_LYRIC_TEMP_WCHAR_LEN * sizeof(wchar));

        if(PNULL != file_ptr->temp_unicode_buf_ptr)
        {
            SCI_MEMSET(file_ptr->temp_unicode_buf_ptr, 0, MMIMP3_LYRIC_TEMP_WCHAR_LEN * sizeof(wchar));
        }
    }

    if(PNULL == file_ptr->temp_parse_buf_ptr)
    {
        file_ptr->temp_parse_buf_ptr = SCI_ALLOC_APP(MMIMP3_LYRIC_TEMP_DATA_LEN);

        if(PNULL != file_ptr->temp_parse_buf_ptr)
        {
            SCI_MEMSET(file_ptr->temp_parse_buf_ptr, 0, MMIMP3_LYRIC_TEMP_DATA_LEN);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : Free lyric parse buffer.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void FreeParseBuffer(
    void
)
{
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();


    if (PNULL == file_ptr)
    {
        return;
    }

    if(PNULL != file_ptr->temp_unicode_buf_ptr)
    {
        SCI_FREE(file_ptr->temp_unicode_buf_ptr);
        file_ptr->temp_unicode_buf_ptr = PNULL;
    }

    if(PNULL != file_ptr->temp_parse_buf_ptr)
    {
        SCI_FREE(file_ptr->temp_parse_buf_ptr);
        file_ptr->temp_parse_buf_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : get lyric file name
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLyricFileFullPathName(
    uint16  *lyric_name_ptr    //in/out
)
{
    uint16  ucs2_lyric_suffix[MMIMP3_LYRIC_SUFFIX_LEN + 1] = {0};
    MMIMP3_LIST_FILE_BODY_T cur_play_info = {0};

    if(PNULL == lyric_name_ptr)
    {
        //SCI_TRACE_LOW:"PNULL == lyric_name_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_2110_112_2_18_2_4_22_248, (uint8*)"");
        return FALSE;
    }

    cur_play_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(!MMIMP3_GetCurPlayFileFullPath(cur_play_info.full_path_name,&(cur_play_info.full_path_len)))
    {
        return FALSE;
    }

    MMI_STRNTOWSTR(ucs2_lyric_suffix, MMIMP3_LYRIC_SUFFIX_LEN,
                   (uint8*)MMIMP3_LYRIC_SUFFIX, MMIMP3_LYRIC_SUFFIX_LEN, MMIMP3_LYRIC_SUFFIX_LEN);

    //set lyric file name
    MMIAPICOM_Wstrncpy(lyric_name_ptr, cur_play_info.full_path_name, cur_play_info.full_path_len);

    //add lyric suffix
    MMIAPICOM_Wstrncpy(lyric_name_ptr + cur_play_info.full_path_len - MMIMP3_SUFFIX_ACTUAL_LEN,
                       ucs2_lyric_suffix, MMIMP3_SUFFIX_ACTUAL_LEN);

    return TRUE;
}

/*****************************************************************************/
//  Description : sort lyric timer tag
//  Global resource dependence :
//  Author:Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL int LyricTimeTagSort(
    const void   *arg1,
    const void   *arg2
)
{
    int                             result = 0;
    MMIMP3_LYRIC_TIME_TAG_INFO_T    *tag_ptr1 = (MMIMP3_LYRIC_TIME_TAG_INFO_T*)arg1;
    MMIMP3_LYRIC_TIME_TAG_INFO_T    *tag_ptr2 = (MMIMP3_LYRIC_TIME_TAG_INFO_T*)arg2;

    if(PNULL == tag_ptr1 || PNULL == tag_ptr2)
    {
        //SCI_TRACE_LOW:"PNULL == tag_ptr1 || PNULL == tag_ptr2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_2147_112_2_18_2_4_22_249, (uint8*)"");
        return result;
    }

    if((tag_ptr1->time) > (tag_ptr2->time))
    {
        result = 1;
    }
    else if((tag_ptr1->time) == (tag_ptr2->time))
    {
        result = 0;
    }
    else
    {
        result = -1;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Init lyric disp ctrl var.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void InitLyricDispCtrl(void)
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();


    if (PNULL != run_ptr)
    {
        SCI_MEMSET(run_ptr, 0, sizeof(*run_ptr));
    }
}

/*****************************************************************************/
//  Description : Set Lyric display control state.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetLyricDispState(LYRIC_DISP_STATE_E state)
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();


    if (PNULL == run_ptr)
    {
        return;
    }

    if(state < LYRIC_DISP_MIN_S || state >= LYRIC_DISP_MAX_S)
    {
        return;
    }

    switch(state)
    {
    case LYRIC_DISP_NORMAL_S:
        SetLyricAnimFrame(0, 0);
        break;

    case LYRIC_DISP_SWITCH_S:
        //SetLyricAnimFrame(LYRIC_ANIM_FRAME_NUM, LYRIC_ANIM_FRAME_NUM);
        //SetLyricAnimTime(LYRIC_ANIM_FRAME_INTERVAL);
        break;

    default:
        break;
    }

    run_ptr->state = state;
}

/*****************************************************************************/
//  Description : Get Lyric display control state.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL LYRIC_DISP_STATE_E GetLyricDispState(void)
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();
    LYRIC_DISP_STATE_E state = 0;


    if (PNULL != run_ptr)
    {
        state = run_ptr->state;
    }

    return state;
}

/*****************************************************************************/
//  Description : Set lyric anim time.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetLyricAnimTime(uint32 time)
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();


    if (PNULL != run_ptr)
    {
        run_ptr->anim_time = time;
    }
}

/*****************************************************************************/
//  Description : Get lyric anim time.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricAnimTime()
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();
    uint32 anim_time = 0;


    if (PNULL != run_ptr)
    {
        anim_time = run_ptr->anim_time;
    }

    return anim_time;
}

/*****************************************************************************/
//  Description : Decrease the lyric anim frame no.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void LyricAnimFrameDec()
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();


    if (PNULL == run_ptr)
    {
        return;
    }

    if(run_ptr->anim_frame > 0)
    {
        run_ptr->anim_frame--;
    }
}

/*****************************************************************************/
//  Description : Get current lyric anim frame.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint8 GetLyricAnimFrame(void)
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();
    uint8 anim_frame = 0;


    if (PNULL != run_ptr)
    {
        anim_frame = run_ptr->anim_frame;
    }

    return anim_frame;
}

/*****************************************************************************/
//  Description : Get lyric anim frame total.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint8 GetLyricAnimFrameTotal(void)
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();
    uint8 frame_total = 0;


    if (PNULL != run_ptr)
    {
        frame_total = run_ptr->anim_frame_total;
    }

    return frame_total;
}

/*****************************************************************************/
//  Description : Set current lyric anim frame and total frame.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetLyricAnimFrame(uint8 anim_frame, uint8 anim_frame_total)
{
    LYRIC_RUNNING_INFO_T *run_ptr = GetLyricRunningInfoPtr();


    if (PNULL == run_ptr)
    {
        return;
    }

    run_ptr->anim_frame = anim_frame;
    run_ptr->anim_frame_total = anim_frame_total;
}

/*****************************************************************************/
//  Description : Draw gradul effect
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DrawGradualEffect(
    uint32 *layer_buff,
    uint32 buff_height,
    uint32 buff_width,
    uint8 begin_alpha,
    uint8 end_alpha
)
{
    uint32 i = 0;
    uint32 j = 0;
    uint8 *tmp_ptr = (uint8 *)layer_buff;
    float alpha = 0;
    float alpha_step = 0;

    if(PNULL == layer_buff || 0 == buff_height)
    {
        return;
    }

    if(255 == begin_alpha && 255 == end_alpha)
    {
        return;
    }

    alpha_step = (float)(end_alpha - begin_alpha) / buff_height;

#if (!defined(__BIG_ENDIAN)) && (!defined(__BigEndian))
    tmp_ptr = (uint8 *)tmp_ptr + 3;
#endif

    alpha = (float)begin_alpha;

    for(i = 1; i <= buff_height; i++)
    {
        alpha += alpha_step;

        for(j = 0; j < buff_width; j++)
        {
            if(0 != *tmp_ptr)
            {
                *tmp_ptr = (*tmp_ptr * (uint8)alpha) >> 8;
            }

            tmp_ptr += sizeof(uint32);
        }
    }
}

/*****************************************************************************/
//  Description : Create lyric layer
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateLyricLayer(uint32 lcd_id, GUI_RECT_T rect, GUI_LCD_DEV_INFO *layer)
{
    UILAYER_CREATE_T    create_info = {0};
    UILAYER_APPEND_BLT_T append_layer = {0};

    /* create a new 888 layer */
    create_info.lcd_id = lcd_id;
    create_info.owner_handle = 0;
    create_info.offset_x = rect.left;
    create_info.offset_y = rect.top;
    create_info.width = (rect.right + 1 - rect.left);
    create_info.height = (rect.bottom + 1 - rect.top);
    create_info.is_bg_layer = FALSE;    /* FALSE is 888 layer */
    create_info.is_static_layer = TRUE; /* If TRUE, will alloc memory automaticlly */
    UILAYER_CreateLayer(
        &create_info,
        layer
    );

    UILAYER_SetLayerColorKey(layer, TRUE, UILAYER_TRANSPARENT_COLOR);

    /* fill color key */
    UILAYER_Clear(layer);

    append_layer.lcd_dev_info = *layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);

    UILAYER_SetLayerPosition(layer, rect.left, rect.top);


}

/*****************************************************************************/
//  Description : Get the lyric real disp line.
//  Global resource dependence :
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricDispLine(
    uint32 line_index,
    uint32 lines_per_scr,
    uint32 top_line,
    uint32 playing_line,
    uint32 total_line
)
{
    uint32 disp_line = 0;
    uint32 half_line_num = lines_per_scr / 2;

    if(playing_line < half_line_num)
    {
        disp_line = half_line_num - playing_line + line_index;
    }
    else if(playing_line > total_line - half_line_num)
    {
        if(top_line + line_index < total_line)
        {
            disp_line = line_index;
        }
        else
        {
            disp_line = 0xFFFF;
        }
    }
    else
    {
        disp_line = line_index;
    }

    return disp_line;
}

/*****************************************************************************/
//  Description : Get lyric gradual alpha.
//  Global resource dependence :
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void GetLyricAlpha(
    uint32 disp_line,
    uint32 lines_per_scr,
    uint32 distance,
    uint32 font_height,
    uint32 space_height,
    uint8 *begin_alpha,
    uint8 *end_alpha
)
{
    const uint8 max_alpha = MMIMP3_LYRIC_TEXT_MAX_ALPHA;
    const uint8 min_alpha = MMIMP3_LYRIC_TEXT_MIN_ALPHA;
    uint16 coe = (max_alpha - min_alpha) * 2;

    uint32 line_height = space_height + font_height;
    uint32 begin_pos = disp_line * line_height;
    uint32 end_pos = begin_pos + font_height;

    if(disp_line < lines_per_scr / 2)
    {
        *begin_alpha = min_alpha + begin_pos * coe / distance;
        *end_alpha = *begin_alpha + font_height * coe / distance;
    }
    else if(lines_per_scr / 2 == disp_line)
    {
        *begin_alpha = 255;
        *end_alpha = 255;
    }
    else if(disp_line < lines_per_scr)
    {
        *end_alpha = min_alpha + (distance - end_pos) * coe / distance;
        *begin_alpha = *end_alpha + font_height * coe / distance;
    }
    else
    {
        *end_alpha = 0;
        *begin_alpha = 0;
    }

}

/*****************************************************************************/
//  Description : Get lyric disp offset position.
//  Global resource dependence :
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void GetLyricDispPos(
    uint32 disp_line,
    uint32 lines_per_scr,
    uint32 font_height,
    uint32 space_height,
    uint16 *offset_x,
    uint16 *offset_y
)
{
    if(disp_line <= lines_per_scr * 2)
    {
        *offset_x = 0;
        *offset_y = disp_line * (font_height + space_height);
    }
    else
    {
        *offset_x = 0xffff;
        *offset_y = 0xffff;
    }
}

/*****************************************************************************/
//  Description : Revise lyric timer and set animate info.
//  Global resource dependence :
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint32 ReviseLyricAnimInfo(uint32 time)
{
    uint32 time_differ = time;

    if(time_differ > LYRIC_ANIM_FRAME_INTERVAL * LYRIC_ANIM_FRAME_NUM)
    {
        time_differ -= LYRIC_ANIM_FRAME_INTERVAL * LYRIC_ANIM_FRAME_NUM;
        SetLyricAnimFrame(LYRIC_ANIM_FRAME_NUM, LYRIC_ANIM_FRAME_NUM);
        SetLyricAnimTime(LYRIC_ANIM_FRAME_INTERVAL);
    }
    else if(time_differ > LYRIC_ANIM_FRAME_INTERVAL * LYRIC_ANIM_FRAME_NUM / 2)
    {
        time_differ = time_differ / (LYRIC_ANIM_FRAME_NUM / 2);

        if(0 == time_differ)
        {
            time_differ = 1;
            SetLyricAnimFrame(2, 2);
        }
        else
        {
            SetLyricAnimFrame(LYRIC_ANIM_FRAME_NUM / 2, LYRIC_ANIM_FRAME_NUM / 2);
        }

        SetLyricAnimTime((uint32)time_differ);
    }
    else
    {
        time_differ = 1;
        SetLyricAnimFrame(2, 2);
        SetLyricAnimTime((uint32)time_differ);
    }

    return time_differ;
}

/*****************************************************************************/
//  Description : Get one lyric sentence occupied lines.
//  Global resource dependence :
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint16 GetLyricSentenceOccupyLines(uint32 sentence_no, GUI_RECT_T rect, uint32 max_line)
{
    MMIMP3_LYRIC_SENTENCE_T *sentence_ptr = PNULL;
    uint16 sentence_line_num = 0;
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();



    if (PNULL == file_ptr)
    {
        return sentence_line_num;
    }

    sentence_ptr = GetLyricSentence(file_ptr->time_tag_ptr[sentence_no].sentence_num);
    sentence_line_num =  GUI_CalculateStringLinesByPixelNum(
                             rect.right - rect.left,
                             sentence_ptr->lyric_content_ptr,
                             sentence_ptr->length,
                             MMIMP3_LYRIC_TEXT_FONT,
                             MMIMP3_LYRIC_CHAR_SPACE,
                             TRUE
                         );

    if(0 == sentence_line_num)
    {
        sentence_line_num = 1;
    }
    else if(sentence_line_num > max_line)
    {
        sentence_line_num = max_line;
    }

    return sentence_line_num;
}

/*****************************************************************************/
//  Description : Get lyric time by line.
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint32 GetLyricTimeByLine(uint32 cur_line, uint32 *new_line)
{
    uint32  time_differ = 0;
    uint32 line_index = cur_line;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();


    if (PNULL == file_ptr
            || PNULL == display_ptr
       )
    {
        return time_differ;
    }

    switch(GetLyricDispState())
    {
    case LYRIC_DISP_NORMAL_S:
        MMIMP3_GetPlayingDynInfo(&dyn_info, TRUE);

        while(line_index + 1 < file_ptr->time_tag_num)
        {
            if(file_ptr->time_tag_ptr[line_index + 1].time > (int32)(dyn_info.cur_time * 1000) + display_ptr->lyric_time_offset)
            {
                time_differ = file_ptr->time_tag_ptr[line_index + 1].time - display_ptr->lyric_time_offset - (int32)(dyn_info.cur_time * 1000);
            }

#if 0

            if(time_differ > LYRIC_ANIM_FRAME_INTERVAL * LYRIC_ANIM_FRAME_NUM)
            {
                time_differ -= LYRIC_ANIM_FRAME_INTERVAL * LYRIC_ANIM_FRAME_NUM;
                SetLyricAnimFrame(LYRIC_ANIM_FRAME_NUM, LYRIC_ANIM_FRAME_NUM);
                SetLyricAnimTime(LYRIC_ANIM_FRAME_INTERVAL);
            }
            else if(time_differ > LYRIC_ANIM_FRAME_INTERVAL * LYRIC_ANIM_FRAME_NUM / 2)
            {
                time_differ = time_differ / (LYRIC_ANIM_FRAME_NUM / 2);

                if(0 == time_differ)
                {
                    time_differ = 1;
                    SetLyricAnimFrame(2, 2);
                }
                else
                {
                    SetLyricAnimFrame(LYRIC_ANIM_FRAME_NUM / 2, LYRIC_ANIM_FRAME_NUM / 2);
                }

                SetLyricAnimTime((uint32)time_differ);
            }
            else
            {
                time_differ = 1;
                SetLyricAnimFrame(2, 2);
                SetLyricAnimTime((uint32)time_differ);
            }

#endif
            time_differ = ReviseLyricAnimInfo(time_differ);
            break;
        }

        *new_line = line_index;
        break;

    case LYRIC_DISP_SWITCH_S:
        time_differ = GetLyricAnimTime();
        *new_line = cur_line;
        break;

    default:
        break;

    }

    return time_differ;
}

/*****************************************************************************/
//  Description : display lyric immediately
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIAP_DisplayLyric(
    MMIAP_DISPLAY_LYRIC_T  disp_param //in
)
{
    uint32 i = 0;
    uint32 cur_process_tag = 0;

    GUI_RECT_T lyric_text_rect = {0};  /*lint !e64*/
    GUI_RECT_T cur_text_rect  = {0};  /*lint !e64*/
    GUI_RECT_T lyric_bg_rect   = {0};  /*lint !e64*/

    MMIMP3_LYRIC_SENTENCE_T *sentence_ptr = PNULL;
    uint16 font_height = 0;

    /* The space between two line */
    uint32 lyric_line_space = 0;
    /* The lines that it is can be displayed on one screen */
    uint32 lyric_line_num   = 0;
    /* The lines displayed in lyric layer before current line */
    uint32 lyric_line_before_cur = 0;
    /* The height of lyric display area, it not equal to the the height of the rect---lyric_text_rect */
    uint32 lyric_area_height = 0;

    GUISTR_STYLE_T text_style  = {0};  /*lint !e64*/
    MMI_STRING_T str_ptr          = {0};

    /* the lines of one sentence occupied */
    uint32 sentence_line_num = 0;
    /* The lines of current sentence occupied */
    uint32 cur_sentence_line_num = 0;

    GUI_LCD_DEV_INFO layer = {0};
    GUI_RECT_T layer_rect = {0};
    GUILCD_DRAW_DC_T draw_dc = {0};
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();



    MMI_IMAGE_ID_T bg_img_id = IMAGE_NULL;//background img id
    GUI_LCD_DEV_INFO lcd_dev_info = {0};//bg lcd dev info
    GUI_FONT_T  font_type = 0; //font type
    uint8 line_space = 0; //line space
    uint8 char_space = 0;
    uint8 margin_v = 0;//text margin v
    uint8 margin_h = 0;// text margin h
    uint32 max_line_num = 0;
    GUI_COLOR_T focus_text_color = 0;
    uint8 min_alpha = 0;
    uint8 max_alpha = 0;


    if (PNULL == display_ptr
            || PNULL == file_ptr
       )
    {
        return FALSE;
    }


    /*lint -e529*/

    if(MMIMP3_LYRIC_PARSED  != display_ptr->lyric_type)
    {
        return FALSE;
    }


    font_type = disp_param.font_type;
    lyric_bg_rect = disp_param.bg_rect;
    lyric_text_rect = disp_param.text_rect;
    lcd_dev_info = disp_param.lcd_dev_info;
    margin_v = disp_param.margin_v;
    margin_h = disp_param.margin_h;
    line_space = disp_param.line_space;
    char_space = disp_param.char_space;
    focus_text_color = disp_param.focus_text_color;
    max_line_num = disp_param.max_line_num;
    min_alpha = disp_param.min_alpha;
    max_alpha = disp_param.max_alpha;
    bg_img_id = disp_param.bg_img_id;

    font_height = GUIFONT_GetHeight(font_type);


    //SCI_TRACE_LOW:"[MMIMP3]:MMIMP3 DisplayLyricImmediately  cur_time_tag_index = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_2645_112_2_18_2_4_23_250, (uint8*)"d", display_ptr->lyric_cur_time_tag_index);


    lyric_text_rect.top += margin_v;
    lyric_text_rect.bottom -= margin_v;
    cur_text_rect = lyric_text_rect;


    /* 1st. part: Create one layer, which height is about the double of lyric_text_rect;
        And then, display lyric to this layer one by one till out of the layer
    */
    {

        /* Display the lyric background */
        GUIRES_DisplayImg(PNULL, &lyric_bg_rect, &lyric_bg_rect, MMK_GetFocusWinId(), bg_img_id, &lcd_dev_info);//&lcd_dev_info);
        /* Firstly we get lines that it can display in one screen,
            There are lyric_line_num lines lyrics and (lyric_line_num-1) lines space, so lyric_line_num is calculated like following code
        */
        lyric_line_num = (lyric_text_rect.bottom - lyric_text_rect.top + 1 + line_space) / (font_height + line_space);

        if(lyric_line_num > max_line_num)
        {
            lyric_line_num = max_line_num;
        }

        if(lyric_line_num >= 1)
        {
            /* Keep the line be odd */
            if(0 == (lyric_line_num % 2))
            {
                lyric_line_num -= 1;
            }
        }
        else
        {
            return FALSE;
        }

        /* Re-calculate the space between each line */
        if(lyric_line_num != 0)
        {
            lyric_line_space = (lyric_text_rect.bottom - lyric_text_rect.top) / (int16)lyric_line_num - font_height;

            if(lyric_line_space > line_space * 2)
            {
                lyric_line_space = line_space * 2;
            }
        }

        /* Get the real lyric area height, it not equal to the height of lyric_text_rect */
        lyric_area_height = (font_height + lyric_line_space) * lyric_line_num;

        layer_rect = lyric_text_rect;
        /* The lay rect height is the double of lyric text display rect for lyric display animate. */
        layer_rect.bottom += (layer_rect.bottom - layer_rect.top + 1) * 2;

        /* make the layer display in the middle of lyric area */
        layer_rect.top += (lyric_text_rect.bottom - lyric_text_rect.top - (int16)(lyric_line_num * (font_height + lyric_line_space))) / 2;
        layer_rect.bottom += (lyric_text_rect.bottom - lyric_text_rect.top - (int16)(lyric_line_num * (font_height + lyric_line_space))) / 2;

        CreateLyricLayer(lcd_dev_info.lcd_id, layer_rect, &layer);

        if(!LCD_PrepareDrawParam(&layer, &draw_dc))
        {
            /* need free layer???? TBD */
            return FALSE;
        }

        /* we get the first line that will be display on the screen.
            It's not start from lyric_cur_time_tag_index, but is lyric_cur_time_tag_index - lylic_line_num/2 normlly.
        */
        if(display_ptr->lyric_cur_time_tag_index > lyric_line_num / 2)
        {
            display_ptr->lyric_display_start_tag = display_ptr->lyric_cur_time_tag_index - lyric_line_num / 2;
        }
        else
        {
            display_ptr->lyric_display_start_tag = 0;
        }

        /* After get the start line number, we should revise it, because the current line  maybe not displayed wholely */
        {
            uint32 start_line = display_ptr->lyric_display_start_tag;
            uint32 flag = FALSE;
            uint32 line_totoal = 0;

            while(start_line < display_ptr->lyric_cur_time_tag_index)
            {
                for(i = start_line; i <= display_ptr->lyric_cur_time_tag_index; i++)
                {
                    line_totoal += GetLyricSentenceOccupyLines(i, layer_rect, lyric_line_num);

                    if(line_totoal > lyric_line_num * 2)
                    {
                        start_line++;
                        flag = TRUE;
                        break;
                    }
                }

                if(!flag)
                {
                    break;
                }
            }

            /* Get the real start number */
            display_ptr->lyric_display_start_tag = start_line;
        }
        /* Next, we display the lyric sentence one by one */
        i = 0;
        cur_process_tag  = display_ptr->lyric_display_start_tag;

        while((i < lyric_line_num * 2) && (cur_process_tag < file_ptr->time_tag_num))
        {
            //if(display_ptr->lyric_display_start_tag + i >= file_ptr->time_tag_num)
            //{
            //    break;
            //}
            sentence_ptr = GetLyricSentence(file_ptr->time_tag_ptr[cur_process_tag].sentence_num);
            sentence_line_num =  GUI_CalculateStringLinesByPixelNum(
                                     layer_rect.right - layer_rect.left,
                                     sentence_ptr->lyric_content_ptr,
                                     sentence_ptr->length,
                                     font_type,
                                     char_space,
                                     TRUE
                                 );

            if(sentence_line_num > 0)
            {
                /* If some sentence occupied more than lyric_line_num, Don't display the exeeded pard */
                if(sentence_line_num > lyric_line_num)
                {
                    sentence_line_num = lyric_line_num;
                }

                str_ptr.wstr_len = sentence_ptr->length;
                str_ptr.wstr_ptr = sentence_ptr->lyric_content_ptr;

                /* Set text style */
                text_style.align = ALIGN_HMIDDLE;
                text_style.font = font_type;
                text_style.char_space = char_space;
                text_style.line_space = lyric_line_space;
                text_style.font_color = focus_text_color;

                cur_text_rect.top = layer_rect.top + (int16)((font_height + lyric_line_space) * i);
                cur_text_rect.bottom = cur_text_rect.top + (int16)((font_height + lyric_line_space) * sentence_line_num);

                GUISTR_DrawTextToLCDInRect(
                    (const GUI_LCD_DEV_INFO *)&layer,
                    (const GUI_RECT_T      *)&cur_text_rect,
                    (const GUI_RECT_T      *)&cur_text_rect,
                    (const MMI_STRING_T    *)&str_ptr,
                    &text_style,
                    GUISTR_STATE_ALIGN,
                    GUISTR_TEXT_DIR_AUTO
                );
            }
            /* If some sentence has no words, it occupy one line still */
            else if(0 == sentence_line_num)
            {
                sentence_line_num = 1;
            }

            /* If it is current sentence, we save some info relate with it */
            if(cur_process_tag == display_ptr->lyric_cur_time_tag_index)
            {
                lyric_line_before_cur = i;
                cur_sentence_line_num = sentence_line_num;
            }

            cur_process_tag ++;
            i += sentence_line_num;
        }

        if(0 < cur_process_tag)
        {
            cur_process_tag --;
        }

        if(i > lyric_line_num)
        {
            display_ptr->lyric_display_end_tag = (cur_process_tag > 0) ? (cur_process_tag - 1) : cur_process_tag;
        }
        else
        {
            display_ptr->lyric_display_end_tag = cur_process_tag;
        }
    }

    /* 2rd. part: Get params of effect by lyric display state.
        This part is very difficult to understand.
    */
    {
        int16 y_offset_base = 0;
        int16 y_offset_effect = 0;
        int16 y_offset_total = 0;
        int16 offset_line = 0;
        int32 up_effect_height = 0;
        int32 down_effect_height = 0;
        uint32 hilight_height = 0;
        uint32 mem_offset_line = 0;

        /* 当前行之前有若干行显示，
            如果当前行正好在中间一行显示，     offset_line = 0;
            如果当前行前面不够半屏显示的行，offset_line < 0;
            如果当前行前面超过半屏显示的行，offset_line > 0;
        */
        offset_line = (lyric_line_before_cur + ((cur_sentence_line_num + 1) / 2 - 1)) - lyric_line_num / 2;

        /* 根据偏移的行得到偏移的像素*/
        y_offset_base = offset_line * (int16)(font_height + lyric_line_space);

        switch(GetLyricDispState())
        {
        case LYRIC_DISP_SWITCH_S:
            /* Firstly, we get the  occupyed lines of the next sentence */
            sentence_ptr = GetLyricSentence(file_ptr->time_tag_ptr[display_ptr->lyric_cur_time_tag_index + 1].sentence_num);
            sentence_line_num =  GUI_CalculateStringLinesByPixelNum(
                                     layer_rect.right - layer_rect.left,
                                     sentence_ptr->lyric_content_ptr,
                                     sentence_ptr->length,
                                     font_type,
                                     char_space,
                                     TRUE
                                 );

            if(0 == sentence_line_num)
            {
                sentence_line_num = 1;
            }
            else if(sentence_line_num > lyric_line_num)
            {
                sentence_line_num = lyric_line_num;
            }

            /* 根据下一句歌词所占用的行数确定每帧动画移动的像素数*/
            {
                uint32 cur_line_height = (font_height + lyric_line_space) * (cur_sentence_line_num);
                uint32 next_line_height = (font_height + lyric_line_space) * (sentence_line_num);
                uint32 total_frame = GetLyricAnimFrameTotal();
                uint32 cur_frame = total_frame  - GetLyricAnimFrame();
                uint32 move_line = 0;

                /* 先得到静态时的值,再根据当前帧进行偏移*/
                hilight_height = (uint32)(cur_sentence_line_num) * (font_height + lyric_line_space);
                up_effect_height = (lyric_area_height / 2 - ((cur_sentence_line_num - 1) / 2) * (font_height + lyric_line_space)) - (font_height + lyric_line_space) / 2;
                down_effect_height = (int32)(lyric_area_height - hilight_height - (uint32)up_effect_height);
                up_effect_height = (up_effect_height < 0) ? 0 : up_effect_height;
                down_effect_height = (down_effect_height < 0) ? 0 : down_effect_height;


                /* 确定从当前行移动到下一行需要移动的行数*/
                move_line = (cur_sentence_line_num + 1) / 2 + (1 - cur_sentence_line_num % 2) + (sentence_line_num - 1) / 2;
                /* 根据行数确定每帧移动的像素数*/
                y_offset_effect = move_line * (font_height + lyric_line_space) * cur_frame / (total_frame);


                /* 减掉动画偏移量,再加上当前高亮语句消失的高度*/
                up_effect_height = up_effect_height - y_offset_effect + (int32)(cur_line_height * cur_frame / total_frame);
                /* 加上偏移量，再减掉 下一句增加的高度*/
                down_effect_height = down_effect_height + y_offset_effect - (int32)(next_line_height * cur_frame / total_frame);


                /* 重新校正*/
                up_effect_height = (up_effect_height < 0) ? 0 : up_effect_height;
                down_effect_height = (down_effect_height < 0) ? 0 : down_effect_height;
                hilight_height = lyric_area_height - (uint32)up_effect_height - (uint32)down_effect_height;
            }
            break;

        case LYRIC_DISP_NORMAL_S:
            hilight_height = (cur_sentence_line_num) * (font_height + lyric_line_space);
            up_effect_height = (lyric_area_height / 2 - ((cur_sentence_line_num - 1) / 2) * (font_height + lyric_line_space)) - (font_height + lyric_line_space) / 2;
            down_effect_height = (int32)lyric_area_height - (int32)hilight_height - up_effect_height;

            up_effect_height = (up_effect_height < 0) ? 0 : up_effect_height;
            down_effect_height = (down_effect_height < 0) ? 0 : down_effect_height;
            break;

        default:
            break;
        }

        /* In LYRIC_DISP_SWITCH_S state, y_offset_effect > 0;
            In LYRIC_DISP_NORMAL_S state, y_offset_effect = 0;
        */
        y_offset_total = y_offset_base + y_offset_effect;


        /* y_offset_total < 0, whitch means lyric sentence is not display from the head of lyric area.
            Generally, it happened at the beginning of one song often
        */
        if(y_offset_total < 0)
        {
            mem_offset_line = 0;
            /* 校正上半部分的特效高度*/
            up_effect_height = up_effect_height + y_offset_total;

            if(up_effect_height < 0)
            {
                up_effect_height = 0;
            }
        }
        else
        {
            mem_offset_line = y_offset_total;
        }


        /* 3th. part: Do alpha and blt layer */
        {
            uint8 begin_alpha = min_alpha;

            /* if lyric sentence is not display from the head of lyric area,
                effect ALPHA value shoule start from min_alpha
            */
            if(y_offset_total < 0)
            {
                //begin_alpha += (max_alpha - min_alpha) * abs(y_offset_total)/((lyric_area_height - font_height)/2);
                begin_alpha += ((max_alpha - min_alpha) * (uint16)(-y_offset_total)) / ((lyric_area_height - font_height) / 2); /*lint !e573*/
            }

            /* Do the UP half of lyric effect */
            DrawGradualEffect(
                (uint32 *)draw_dc.lcd_buffer_ptr + draw_dc.mem_width * mem_offset_line,
                up_effect_height,
                draw_dc.mem_width,
                begin_alpha,
                max_alpha
            );

            /* Do the Down half of lyric effect */
            DrawGradualEffect(
                (uint32 *)draw_dc.lcd_buffer_ptr + draw_dc.mem_width * (mem_offset_line + (uint32)up_effect_height + hilight_height),
                down_effect_height,
                draw_dc.mem_width,
                max_alpha,
                min_alpha
            );
        }

        /* re-locate the layer position for BLT */
        if(y_offset_total != 0)
        {
            UILAYER_SetLayerPosition(
                &layer,
                layer_rect.left,
                layer_rect.top - y_offset_total
            );
        }

        /* To reduce the blt pixels, re-set blt real rect */
        layer_rect.bottom = layer_rect.top + (int16)lyric_area_height - 1;
        UILAYER_BltLayerToLayer(
            &lcd_dev_info,
            &layer,
            &layer_rect,
            TRUE
        );
    }
    UILAYER_RELEASELAYER(&layer);   /*lint !e506 !e774*/
    //    UILAYER_ReleaseLayer(&layer);
    /*lint -e529*/

    return (TRUE);
}

/*****************************************************************************/
//  Description : display lyric immediately
//  Global resource dependence :
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_DisplayLyricImmediately(
    void
)
{
    BOOLEAN result = FALSE;
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T lyric_text_rect = {0};  /*lint !e64*/
    GUI_RECT_T lyric_bg_rect   = {0};  /*lint !e64*/

    MMIAP_DISPLAY_LYRIC_T  disp_param = {0};



    if(!((MMIMP3_DISPLAY_LYRIC_ON == APSETTING_GetDisplayStyle() || MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT == APSETTING_GetDisplayStyle())
            && (MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState() || MMIMP3_AUDIO_PAUSED  == MMIMP3_GetAudioState())
        )
      )
    {
        return FALSE;
    }

    if(GUIANIM_TYPE_NONE == GUIANIM_GetAnimType(MMIMP3_GetMainAnimCtrlId()))
    {
        SCI_TRACE_LOW("[MMIMP3]:MMIMP3_DisplayLyricImmediately anim type none!!!!");
        return FALSE;
    }



    MMIMP3_GetMainAnimValidDisplayRectBg(&lyric_bg_rect);
    MMIMP3_GetMainAnimValidDisplayRectBg(&lyric_text_rect);


    disp_param.bg_img_id = IMAGE_MP3_OSD_BG_DOWN;
    disp_param.lcd_dev_info = lcd_dev_info;
    disp_param.bg_rect = lyric_bg_rect;
    disp_param.text_rect = lyric_text_rect;
    disp_param.font_type = MMIMP3_LYRIC_TEXT_FONT;

    disp_param.margin_v = MMIMP3_LYRIC_TEXT_MARGIN_V;
    disp_param.line_space = MMIMP3_LYRIC_LINE_SPACE;
    disp_param.char_space = MMIMP3_LYRIC_CHAR_SPACE;
    disp_param.focus_text_color = MMIMP3_LYRIC_FOCUS_TEXT_COLOR;
    disp_param.min_alpha = MMIMP3_LYRIC_TEXT_MIN_ALPHA;
    disp_param.max_alpha = MMIMP3_LYRIC_TEXT_MAX_ALPHA;
    disp_param.max_line_num = MMIMP3_LYRIC_DISPLAY_LINE_NUM;


    GUIANIM_UpdateRect(lyric_bg_rect, MMIMP3_GetMainAnimCtrlId());
    result = MMIAPIAP_DisplayLyric(disp_param);

    return result;
}

/*****************************************************************************/
//  Description : get lyric display timer id
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIMP3_GetLyricTimerId(void)
{
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();
    uint8 timer_id = 0;


    if (PNULL != display_ptr)
    {
        timer_id = display_ptr->lyric_disp_timer_id;
    }

    return (timer_id);
}

/*****************************************************************************/
//  Description : handle mp3 lyric timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_HandleLyricTimer(uint8 timer_id, uint32 param)
{
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();


    if (PNULL == display_ptr
            || PNULL == file_ptr
       )
    {
        return;
    }

    StopLyricTimer();

    if(MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState())
    {
        switch(GetLyricDispState())
        {
        case LYRIC_DISP_NORMAL_S:
            SetLyricDispState(LYRIC_DISP_SWITCH_S);
            LyricAnimFrameDec();
            break;

        case LYRIC_DISP_SWITCH_S:
            LyricAnimFrameDec();

            if(GetLyricAnimFrame() == 0)
            {
                SetLyricDispState(LYRIC_DISP_NORMAL_S);

                if(display_ptr->lyric_cur_time_tag_index + 1 < file_ptr->time_tag_num)
                {
                    display_ptr->lyric_cur_time_tag_index++;
                }

            }

            break;

        default:
            break;
        }

        if(MMIMP3_DISPLAY_LYRIC_ON == APSETTING_GetDisplayStyle()
                || MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT == APSETTING_GetDisplayStyle()
          )
        {
            MMIAP_LYRIC_WIN_CALLBACK_PARAM_T req_param = {0};

            req_param.is_update_lyric = TRUE;
            req_param.is_update_lyric_now = TRUE;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN, MMIAPWIN_FLAG_FUNC_LYRIC, &req_param, sizeof(req_param));
        }


        StartLyricTimer();
    }
}

/*****************************************************************************/
//  Description : Start Lyric Timer
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StartLyricTimer(
    void
)
{
    int32  time_differ = 0;
    uint32 lyric_tag_index = 0;
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();


    if (PNULL == display_ptr)
    {
        return;
    }
    lyric_tag_index = display_ptr->lyric_cur_time_tag_index;

    //MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    //LYRIC_DISP_STATE_E state = 0;
    if(MMIMP3_LYRIC_PARSED   != display_ptr->lyric_type ||
            MMIMP3_AUDIO_STOPPED  == MMIMP3_GetAudioState())
    {
        return;
    }

    time_differ = GetLyricTimeByLine(display_ptr->lyric_cur_time_tag_index, &lyric_tag_index);

    if(0 < time_differ)
    {
        if(display_ptr->lyric_disp_timer_id != 0)
        {
            MMK_StopTimer(display_ptr->lyric_disp_timer_id);
            display_ptr->lyric_disp_timer_id = 0;
        }

        display_ptr->lyric_disp_timer_id = MMK_CreateTimerCallback(time_differ, MMIAPIMP3_HandleLyricTimer, NULL, FALSE);
    }

    if(display_ptr->lyric_cur_time_tag_index != lyric_tag_index)
    {
        display_ptr->lyric_cur_time_tag_index = lyric_tag_index;

        {
            MMIAP_LYRIC_WIN_CALLBACK_PARAM_T req_param = {0};

            req_param.is_update_lyric = TRUE;
            req_param.is_update_lyric_now = FALSE;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN, MMIAPWIN_FLAG_FUNC_LYRIC, &req_param, sizeof(req_param));
        }
    }

    //SCI_TRACE_LOW:"[MMIMP3]:StartLyricTimer: timer_id = %d,time_differ = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_3093_112_2_18_2_4_24_251, (uint8*)"dd", display_ptr->lyric_disp_timer_id, time_differ);
}

/*****************************************************************************/
//  Description : stop lyric
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_StopLyric(void)
{
    MMIMP3_HandleLyricBackLight(FALSE);

    //stop lyric timer
    StopLyricTimer();

    CloseLyricFile();

    FreeLyricBuffer();
    //reset lyric info
    InitLyric();
}

/*****************************************************************************/
//  Description : stop lyric timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StopLyricTimer(void)
{
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();



    if (PNULL == display_ptr)
    {
        return;
    }

    if(0 < display_ptr->lyric_disp_timer_id)
    {
        MMK_StopTimer(display_ptr->lyric_disp_timer_id);
        display_ptr->lyric_disp_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : pause lyric
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_PauseLyric(void)
{
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();



    if (PNULL == display_ptr)
    {
        return;
    }

    if((MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState()) &&
            (0 < display_ptr->lyric_disp_timer_id))
    {
        //if (!MMK_PauseTimer(display_ptr->lyric_disp_timer_id))
        if(!MMK_StopTimer(display_ptr->lyric_disp_timer_id))
        {
            //SCI_TRACE_LOW:"MMIMP3_PauseLyric: pause timer fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_3139_112_2_18_2_4_24_252, (uint8*)"");
        }

        display_ptr->lyric_disp_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : resume lyric
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ResumeLyric(void)
{
    //if (0 < display_ptr->lyric_disp_timer_id)
    {
        /*
        if (MMIMP3_AUDIO_PAUSED == MMIMP3_GetAudioState())
        {
            if (!MMK_ResumeTimer(display_ptr->lyric_disp_timer_id))
            {
                //SCI_TRACE_LOW:"MMK_ResumeTimer: resume timer fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMP3_LYRIC_3158_112_2_18_2_4_24_253,(uint8*)"");

                MMIMP3_SetAudioState(MMIMP3_AUDIO_PLAYING);
                MMIMP3_DisplayLyricImmediately();
                StartLyricTimer();
            }
        }
        else*/
        {

            {
                MMIAP_LYRIC_WIN_CALLBACK_PARAM_T req_param = {0};

                req_param.is_update_lyric = TRUE;
                req_param.is_update_lyric_now = TRUE;
                MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN, MMIAPWIN_FLAG_FUNC_LYRIC, &req_param, sizeof(req_param));
            }


            StartLyricTimer();
        }
    }
}

/*****************************************************************************/
//  Description : seek lyric by time offset
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SeekLyric(
    uint32     mp3_offset,
    BOOLEAN    is_update
)
{
    uint32              i = 0;
    int32               time_differ = 0;
    int32               time_total_offset = 0;
    MMIMP3_PLAYING_DYN_INFO_T dyn_info = {0};
    MMIMP3_LYRIC_FILE_INFO_T *file_ptr = GetLyricFileInfoPtr();
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();


    if (PNULL == file_ptr
            || PNULL == display_ptr
       )
    {
        return;
    }

    if(MMIMP3_LYRIC_PARSED  != display_ptr->lyric_type ||
            MMIMP3_AUDIO_STOPPED == MMIMP3_GetAudioState())
    {
        return;
    }

    MMIMP3_GetPlayingDynInfo(&dyn_info, FALSE);

    InitLyricDispCtrl();

    if(0 != dyn_info.total_length)
    {
        //get time total offset
        time_total_offset = (int32)(((uint64_t)mp3_offset * dyn_info.total_time / dyn_info.total_length) * 1000);

        //SCI_TRACE_LOW:"MMIMP3_SeekLyric:mp3_offset = %d,time_total_offset = %d,time_tag_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_3205_112_2_18_2_4_24_254, (uint8*)"ddd", mp3_offset, time_total_offset, file_ptr->time_tag_num);

        //seek lyric
        for(i = 0; i < file_ptr->time_tag_num; i++)
        {
            //SCI_TRACE_LOW:"MMIMP3_SeekLyric:i = %d,time = %d,lyric_time_offset = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_3210_112_2_18_2_4_24_255, (uint8*)"ddd", i, file_ptr->time_tag_ptr[i].time, display_ptr->lyric_time_offset);

            if(file_ptr->time_tag_ptr[i].time >= time_total_offset + display_ptr->lyric_time_offset)
            {
                time_differ = file_ptr->time_tag_ptr[i].time - display_ptr->lyric_time_offset - time_total_offset;
                break;
            }
        }

        //set current timer tag index
        if(0 < i)
        {
            display_ptr->lyric_cur_time_tag_index = i - 1;
        }
        else
        {
            display_ptr->lyric_cur_time_tag_index = 0;
        }

        //SCI_TRACE_LOW:"MMIMP3_SeekLyric:lyric_cur_time_tag_index = %d,time_differ = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMIMP3_LYRIC_3229_112_2_18_2_4_24_256, (uint8*)"dd", display_ptr->lyric_cur_time_tag_index, time_differ);

        //Stop lyric timer
        StopLyricTimer();

        if(is_update)
        {
            MMIAP_LYRIC_WIN_CALLBACK_PARAM_T req_param = {0};

            req_param.is_update_lyric = TRUE;
            req_param.is_update_lyric_now = TRUE;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN, MMIAPWIN_FLAG_FUNC_LYRIC, &req_param, sizeof(req_param));
        }

        //creat time differ timer
        //if (0 < time_differ)
        if(i < file_ptr->time_tag_num)
        {
            time_differ = ReviseLyricAnimInfo(time_differ);
            display_ptr->lyric_disp_timer_id = MMK_CreateTimerCallback(time_differ, MMIAPIMP3_HandleLyricTimer, NULL, FALSE);
        }
        else
        {
            //StartLyricTimer();
        }
    }
}

/*****************************************************************************/
//  Description : seek lyric by time offset
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMP3_SeekLyric(
    uint32     mp3_offset,
    BOOLEAN    is_update
)
{
    MMIMP3_SeekLyric(mp3_offset, is_update);
}

/*****************************************************************************/
//  Description : Handle lyric display style with light always on or not.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_HandleLyricBackLight(
    BOOLEAN  need_light_always_on
)
{
    if(need_light_always_on)
    {
        if(MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT == APSETTING_GetDisplayStyle()
                && MMIMP3_IsLyricExist())
        {
            MMIAP_LYRIC_WIN_CALLBACK_PARAM_T req_param = {0};

            req_param.is_update_backlight = TRUE;
            req_param.is_light_on = TRUE;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN, MMIAPWIN_FLAG_FUNC_LYRIC, &req_param, sizeof(req_param));
        }
    }
    else
    {
        if(MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT == APSETTING_GetDisplayStyle()  //设置接口不能常调用 MMIDEFAULT_AllowTurnOffBackLight
                && MMIMP3_IsLyricExist())
        {
            MMIAP_LYRIC_WIN_CALLBACK_PARAM_T req_param = {0};

            req_param.is_update_backlight = TRUE;
            req_param.is_light_on = FALSE;
            MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN, MMIAPWIN_FLAG_FUNC_LYRIC, &req_param, sizeof(req_param));
        }
    }
}

/*****************************************************************************/
//  Description : Get whether lyric file exist.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsLyricExist(
    void
)
{
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();
    BOOLEAN is_exist = FALSE;


    if (PNULL != display_ptr)
    {
        is_exist = MMIMP3_LYRIC_EXIST == display_ptr->lyric_type
                   || MMIMP3_LYRIC_PARSED == display_ptr->lyric_type;
    }

    return is_exist;
}

/*****************************************************************************/
//  Description : Set the flag lyric file exist.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SetLyricExistState(
    MMIMP3_LYRIC_TYPE_E lyric_type
)
{
    MMIMP3_LYRIC_DISPLAY_INFO_T *display_ptr = GetLyricDisplayInfoPtr();


    if (PNULL != display_ptr)
    {
        display_ptr->lyric_type = lyric_type;
    }
}

/*****************************************************************************/
//  Description : Whether it is the lyric of the current background play
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMP3_IsBgPlayLyric(
    uint16      *full_name_ptr, //in
    uint16     full_name_len   //in
)
{
    BOOLEAN     result = FALSE;
    uint16      lyric_full_path[FMM_SEARCH_FILENAME_MAXLEN + 1] = {0};
    MMIMP3_LIST_FILE_BODY_T cur_play_file_info = {0};

    cur_play_file_info.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
    if(MMIMP3_AUDIO_STOPPED != MMIMP3_GetAudioState() &&
            MMIMP3_GetCurPlayFileFullPath(cur_play_file_info.full_path_name,&(cur_play_file_info.full_path_len)))
    {
        //get lyric file name
        GetLyricFileFullPathName(lyric_full_path);

        if(0 == MMIAPICOM_CompareTwoWstrExt(
                    full_name_ptr, full_name_len,
                    lyric_full_path, MMIAPICOM_Wstrlen(lyric_full_path),
                    FALSE
                ))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : init lyric info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitLyricInfo(
    void
)
{
    BOOLEAN ret = FALSE;


    ret = CreateAPLyricInfo();
    if (!ret)
    {
        return ret;
    }


    return ret;
}

#endif//MMIAP_LYRIC_SUPPORT

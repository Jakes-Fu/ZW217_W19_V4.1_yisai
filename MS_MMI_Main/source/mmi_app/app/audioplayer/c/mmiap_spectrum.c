/*****************************************************************************
** File Name:               mmiap_spectrum.c                                 *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifdef MMIAP_SPECTRUM_SUPPORT
#define _MMIAP_SPECTRUM_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmipub.h"
#include "mmi_text.h"
#include "guirichtext.h"
#include "guires.h"
#include "mmi_common.h"


//other module


//local module
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmimp3_image.h"
#include "mmiap_play.h"

#include "mmiap_setting.h"
#include "mmimp3_export.h"
#include "mmiapwin_main.h"
#include "mmiap_list.h"
#include "mmiapwin_main.h"
#include "mmiap_appmain.h"
#include "mmiap_spectrum.h"


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/


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

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/

LOCAL MMIAP_SPECTRUM_INFO_T *s_mmiap_spectrum_info_ptr = PNULL;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Start Mp3 Spec Timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StartSpecTimer(void);


/*****************************************************************************/
//  Description : stop Mp3 Freq Timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StopSpecTimer(void);


/*****************************************************************************/
//  Description : handle Mp3 spec Timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleSpecTimer(uint8 timer_id, uint32 param);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get ap spectrum info ptr
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL MMIAP_SPECTRUM_INFO_T *GetAPSpectrumInfoPtr(void)
{
    return s_mmiap_spectrum_info_ptr;
}

/*****************************************************************************/
//  Description : create ap spectrum info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateAPSpectrumInfo(void)
{
    BOOLEAN ret = FALSE;
    MMIAP_SPECTRUM_INFO_T *info_ptr = GetAPSpectrumInfoPtr();
    uint32 info_size = sizeof(*info_ptr);


    if (PNULL == info_ptr)
    {
        info_ptr = SCI_ALLOCA(info_size);
    }

    if (PNULL != info_ptr)
    {
        s_mmiap_spectrum_info_ptr = info_ptr;

        SCI_MEMSET(info_ptr, 0, info_size);

        ret = TRUE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : destroy spectrum ino
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroySpectrumInfo(
    void
)
{
    if (PNULL != s_mmiap_spectrum_info_ptr)
    {
        SCI_FREE(s_mmiap_spectrum_info_ptr);
    }
}

/*****************************************************************************/
//  Description : Is Spectrum Open
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_IsSpectrumOpen(void)
{
    MMIAP_SPECTRUM_INFO_T *info_ptr = GetAPSpectrumInfoPtr();
    BOOLEAN is_open = FALSE;


    if (PNULL != info_ptr)
    {
        is_open = info_ptr->is_spec_open;
    }

    return (is_open);
}

/*****************************************************************************/
//  Description : set Spectrum Open
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetSpectrumOpen(
    BOOLEAN is_open //in
)
{
    MMIAP_SPECTRUM_INFO_T *info_ptr = GetAPSpectrumInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->is_spec_open =is_open;
    }
}

/*****************************************************************************/
//  Description : get timer id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint8 GetSpectrumTimerID(
    void
)
{
    MMIAP_SPECTRUM_INFO_T *info_ptr = GetAPSpectrumInfoPtr();
    uint8 timer_id = 0;


    if (PNULL != info_ptr)
    {
        timer_id = info_ptr->timer_id;
    }

    return timer_id;
}

/*****************************************************************************/
//  Description : set timer id
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetSpectrumTimerID(
    uint8 timer_id //in
)
{
    MMIAP_SPECTRUM_INFO_T *info_ptr = GetAPSpectrumInfoPtr();


    if (PNULL != info_ptr)
    {
        info_ptr->timer_id = timer_id;
    }
}

/*****************************************************************************/
//  Description : get top line head pointer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint16* GetSpectrumTopLineHeadPtr(
    void
)
{
    MMIAP_SPECTRUM_INFO_T *info_ptr = GetAPSpectrumInfoPtr();
    uint16 *head_ptr = PNULL;


    if (PNULL != info_ptr)
    {
        head_ptr = info_ptr->spec_top_line;
    }

    return head_ptr;
}

/*****************************************************************************/
//  Description : Start Mp3 spectrum.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_StartSpectrum(
    void
)
{
    BOOLEAN result = FALSE;
    BOOLEAN is_open = FALSE;
    MMIAP_WIN_CALLBACK_RET_E cb_ret = 0;


    {
        MMIAP_SPECTRUM_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.is_check_win_focus = TRUE;
        cb_ret = MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                                     MMIAPWIN_FLAG_FUNC_SPECTRUM,
                                     &win_param,
                                     sizeof(win_param)
                                    );
    }

    if(MMIAP_WIN_CALLBACK_RET_CONTINUE == cb_ret
            && MMIMP3_DISPLAY_SPECTRUM == APSETTING_GetDisplayStyle()
            && MMIMP3_AUDIO_PLAYING == MMIMP3_GetAudioState())
    {
        StartSpecTimer();

        is_open = MMIMP3_IsSpectrumOpen();
        if(!is_open)
        {
            is_open = MMISRVAUD_OpenSpectrum(MMIAPIMP3_GetBGPlayerHandle());
            SetSpectrumOpen(is_open);
            MMIMP3_ClearSpectrumInfo();
        }

        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Stop Mp3 spectrum.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_StopSpectrum(
    BOOLEAN is_clear
)
{
    BOOLEAN result = TRUE;

    StopSpecTimer();

    MMISRVAUD_CloseSpectrum(MMIAPIMP3_GetBGPlayerHandle());
    SetSpectrumOpen(FALSE);

    if(is_clear)
    {
        MMIMP3_ClearSpectrumInfo();
    }

    return result;
}

/*****************************************************************************/
//  Description : stop Mp3 Freq Timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StopSpecTimer(void)
{
    uint8 timer_id = 0;


    timer_id = GetSpectrumTimerID();
    if(0 != timer_id)
    {
        MMK_StopTimer(timer_id);
        SetSpectrumTimerID(0);
    }
}
/*****************************************************************************/
//  Description : Start Mp3 Freq Timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void StartSpecTimer(void)
{
    uint8 timer_id = 0;
    uint32 time_out = 0;


    StopSpecTimer();

#ifdef BLUETOOTH_SUPPORT

    if(MMIAPIMP3_IsThroughBt())
    {
        time_out = 2 * MMIMP3_SPECTRUM_TIME;
    }
    else
#endif
    {
        time_out = MMIMP3_SPECTRUM_TIME;
    }

    timer_id =  MMK_CreateTimerCallback(time_out, HandleSpecTimer, NULL, FALSE);
    SetSpectrumTimerID(timer_id);

}

/*****************************************************************************/
//  Description : handle Mp3 Freq Timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void HandleSpecTimer(uint8 timer_id, uint32 param)
{
    {
        MMIAP_SPECTRUM_WIN_CALLBACK_PARAM_T win_param = {0};


        win_param.is_show_spectrum = TRUE;
        MMIAP_DoWinCallback(MMIAP_WIN_TYPE_MAIN,
                            MMIAPWIN_FLAG_FUNC_SPECTRUM,
                            &win_param,
                            sizeof(win_param)
                           );
    }

    StartSpecTimer();
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void AllocSpectrumInfo(MMISRVAUD_SPECTRUM_INFO_T * spectrum_info_ptr)
{
    if(PNULL != spectrum_info_ptr)
    {
        spectrum_info_ptr->spec_num = MMIMP3_FREQ_NUM_MAX;
        spectrum_info_ptr->spec_buf_ptr = (uint16*)SCI_ALLOC_APPZ(MMIMP3_FREQ_NUM_MAX * sizeof(uint16));
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
LOCAL void FreeSpectrumInfo(MMISRVAUD_SPECTRUM_INFO_T * spectrum_info_ptr)
{
    if(PNULL != spectrum_info_ptr)
    {
        if (PNULL != spectrum_info_ptr->spec_buf_ptr)
        {
            SCI_FREE(spectrum_info_ptr->spec_buf_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : Display mp3 Frequecy image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIAP_DisplaySpectrumContent(
    MMIAP_DISPLAY_SPECTRUM_T disp_param //in
)
{
    uint32 i = 0;

    /*lint -e529*/
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T spec_image_rect  = {0};
    GUI_RECT_T spec_image_rect_orig  = {0};
    uint32     spectrum_unit    = 0;
    int16      left_margin      = 0;
    MMISRVAUD_SPECTRUM_INFO_T spectrum_info = {0};
    uint16     top_line         = 0;
    uint16 *head_ptr = GetSpectrumTopLineHeadPtr();

    uint8 margin = 0;
    GUI_COLOR_T color = 0;
    uint32 max_freq_num = 0;//ÆµµãÊý
    MMISRV_HANDLE_T audio_handle = PNULL;

    if (PNULL == head_ptr)
    {
        return;
    }

    lcd_dev_info = disp_param.lcd_dev_info;
    margin = disp_param.margin;
    max_freq_num = disp_param.max_freq_num;
    color = disp_param.color;
    spec_image_rect_orig = disp_param.rect;
    audio_handle = disp_param.audio_handle;


    spec_image_rect_orig.left   += margin;
    spec_image_rect_orig.right  -= margin;
    spec_image_rect_orig.top    += margin * 2;
    spec_image_rect_orig.bottom -= margin * 2;
    spectrum_unit = (spec_image_rect_orig.right - spec_image_rect_orig.left) / max_freq_num; /*lint !e573 !e737*/
    left_margin   = (spec_image_rect_orig.right - spec_image_rect_orig.left) % max_freq_num / 2; /*lint !e573 !e737*/
    spec_image_rect_orig.left   += left_margin;
    spec_image_rect_orig.right  -= left_margin;


    AllocSpectrumInfo(&spectrum_info);
    if (PNULL != audio_handle)
    {
        MMISRVAUD_GetSpectrum(audio_handle, &spectrum_info);
    }

    if(0 == spectrum_info.max_spec_value
            || PNULL == spectrum_info.spec_buf_ptr
            || 0 == spectrum_info.spec_num)
    {
        FreeSpectrumInfo(&spectrum_info);
        return;
    }

    if(spectrum_info.spec_num > max_freq_num)
    {
        FreeSpectrumInfo(&spectrum_info);
        return;
    }

    for(i = 0; i < spectrum_info.spec_num; i++)
    {
        spec_image_rect.bottom = spec_image_rect_orig.bottom;
        spec_image_rect.left   = spec_image_rect_orig.left + (uint16)(spectrum_unit * i);
        spec_image_rect.right  = spec_image_rect.left + (uint16)(spectrum_unit >> 1);

        if(spec_image_rect.right > spec_image_rect_orig.right)
        {
            break;
        }

        spec_image_rect.top = spec_image_rect_orig.bottom - spectrum_info.spec_buf_ptr[i] * (spec_image_rect_orig.bottom - spec_image_rect_orig.top) / (spectrum_info.max_spec_value);
        top_line = (uint16)(head_ptr[i] & 0x3ff);

        if(spec_image_rect.top > spec_image_rect_orig.bottom)
        {
            spec_image_rect.top = spec_image_rect_orig.bottom;
        }

        if(spec_image_rect.top < spec_image_rect_orig.top)
        {
            spec_image_rect.top = spec_image_rect_orig.top;
        }

        if(top_line > 0)
        {
            if(top_line < spec_image_rect.top)
            {
                if(top_line > spec_image_rect_orig.bottom)
                {
                    top_line = spec_image_rect_orig.bottom;
                }

                if(top_line < spec_image_rect_orig.top)
                {
                    top_line = spec_image_rect_orig.top;
                }

                LCD_DrawHLine(&lcd_dev_info, spec_image_rect.left, top_line, spec_image_rect.right, color);
                head_ptr[i] = (((head_ptr[i] >> 10) + 1) << 10) + (head_ptr[i] >> 10) + 1 + (head_ptr[i] & 0x3ff);
            }
            else
            {
                head_ptr[i] = spec_image_rect.top;
            }
        }
        else
        {
            head_ptr[i] = spec_image_rect.top;
        }

        LCD_FillRect(&lcd_dev_info, spec_image_rect, color);
    }

    FreeSpectrumInfo(&spectrum_info);

    /*lint -e529*/
}

/*****************************************************************************/
//  Description : Display mp3 Frequecy image.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_DisplaySpectrum(
    MMI_CTRL_ID_T ctrl_id //in
)
{

    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T spec_bg_rect     = {0};
    GUI_RECT_T spec_image_rect_orig  = {0};
    MMISRVAUD_SPECTRUM_INFO_T spectrum_info = {0};
    MMIAP_DISPLAY_SPECTRUM_T disp_param = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;


    if(MMIMP3_DISPLAY_SPECTRUM == APSETTING_GetDisplayStyle()
            && MMIAPIMP3_IsPlayerActive()
      )
    {
        //PDA use anim ctrl
        MMIMP3_GetMainAnimValidDisplayRect(&spec_bg_rect);
        //MMIMP3_GetMainAnimValidDisplayRect(&spec_image_rect);
        MMIMP3_GetMainAnimValidDisplayRect(&spec_image_rect_orig);

        GUIANIM_UpdateRect(spec_bg_rect, ctrl_id);
        if(MMIMP3_IsSpectrumOpen())
        {
            audio_handle = MMIAPIMP3_GetBGPlayerHandle();
        }

        disp_param.lcd_dev_info = lcd_dev_info;
        disp_param.max_freq_num = MMIMP3_FREQ_NUM_MAX;
        disp_param.margin = MMIMP3_SPECTRUM_MARGIN;
        disp_param.color = MMIMP3_SPEC_COLOR;
        disp_param.rect = spec_image_rect_orig;
        disp_param.audio_handle = audio_handle;

        MMIAPIAP_DisplaySpectrumContent(disp_param);
    }
}

/*****************************************************************************/
//  Description : clear mp3 spec.
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_ClearSpectrumInfo(void)
{
    uint16 *head_ptr = GetSpectrumTopLineHeadPtr();


    if(PNULL != head_ptr)
    {
        SCI_MEMSET(head_ptr, 0, sizeof(uint16) * (MMIMP3_FREQ_NUM_MAX + 1));
    }
}

/*****************************************************************************/
//  Description : init spectrum info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitSpectrumInfo(
    void
)
{
    BOOLEAN ret = FALSE;


    ret = CreateAPSpectrumInfo();
    if (!ret)
    {
        return ret;
    }


    return ret;
}
#endif

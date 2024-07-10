/*****************************************************************************
** File Name:      mmiap_lastinfo_ctrl.c                                     *
** Author:                                                                   *
** Date:           5/29/2012                                                 *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mp3 history list                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 5/2012         robert.wang        Create
******************************************************************************/


#ifdef MMIAP_F_HISTORY_PLAYLIST

#define MMIMP3_HISTORYLIST_C
/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#include "mmipub.h"


//local module
#include "mmiap_list_control.h"
#include "mmiap_play.h"
#include "mmiap_list.h"
#include "mmiap_lastinfo_manager.h"
#include "mmiap_lastinfo_ctrl.h"

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL BOOLEAN s_mp3_is_history_init = FALSE;
LOCAL MMIAP_HISTORY_PANEL_INFO_T s_ap_history_panel_info = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init history param
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void InitHistoryParam(void);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get history mp3 play list info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetHistoryPlayListInfo(void)
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_init = FALSE;
    MMIAP_HISTORY_OUT_PARAM_INFO_T param_out = {0};
    MMIMP3_PLAY_LIST_INFO_T play_list_info = {0};
    MMIMP3_PLAY_INFO_T *play_info_ptr = PNULL;
    MMIMP3_REVIEW_INFO_T review_info = {0};

    BOOLEAN is_music_list = 0;
    uint16 list_index = 0;
    uint16 file_index = 0;

	APLAYER_LIST_TYPE_E list_type = 0;


    InitHistoryParam();


    play_info_ptr = MMIAP_GetAPCurPlayInfo();

    if((PNULL == play_info_ptr)
      )
    {
        return ret;
    }


    do
    {

        ret = MMIAP_ReadFromHistoryFile(&param_out);

        if(ret)
        {
            play_list_info = param_out.context.cur_list_info;

            is_music_list = play_list_info.is_music_list;
            list_index = play_list_info.cur_list_index;
            file_index = param_out.context.active_index;

			list_type = MMIMP3_GetListType(is_music_list, list_index );
			 /*
			* fmm list type  my music list(防止用户往AUDIO文件夹中加文件，记录信息无效) 不保存
			*/

			if((PNULL == play_info_ptr)
					|| (APLAYER_LIST_TYPE_FMM == list_type) || (APLAYER_LIST_TYPE_MYMUSIC == list_type)
			  )
			{
				return FALSE;
			}		


            ret = MMIMP3_SetCurPlayInfo(is_music_list,
                                        list_index,
                                        file_index,
                                        MMIMP3_GetMp3PlayMode()
                                       );


            //ret = MMIMP3_CheckCurPlayListIndex();

            if (ret)
            {
                play_info_ptr->dyn_playing_info = param_out.context.dyn_playing_info;
	            play_info_ptr->old_info = param_out.context.old_info;
                review_info = param_out.context.review_info;

                if(MMIMP3_REVIEW_STATE_NONE != review_info.state)
                {
                    param_out.context.panel_info.is_review_button_focus = TRUE;
                }

                s_ap_history_panel_info = param_out.context.panel_info;

                MMIAP_SetReviewInfo(&review_info);
            }
        }

        if(ret)
        {
            is_init = TRUE;
        }

    }
    while(0);


    MMIAP_SetHistoryInitFlag(is_init);


    SCI_TRACE_LOW("[MMIMP3] MMIMP3_GetHistoryPlayListInfo ret=%d", \
                  ret);

    return ret;
}

/*****************************************************************************/
//  Description : save mp3 play list info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SavePlayListInfo(void)
{
    MMIAP_HISTORY_IN_PARAM_INFO_T param_in = {0};
    MMIMP3_PLAY_INFO_T *play_info_ptr = PNULL;
    MMIMP3_REVIEW_INFO_T review_info = {0};
    MMIMP3_PLAY_LIST_INFO_T play_list_info = MMIMP3_GetCurPlayListInfo();
    uint32 active_index = MMIMP3_GetCurPlayFileIndex();
    APLAYER_LIST_TYPE_E list_type = 0;


    play_info_ptr = MMIAP_GetAPCurPlayInfo();
    
    list_type = MMIMP3_GetListType(play_list_info.is_music_list,
                                  play_list_info.cur_list_index
                                  );

    SCI_TRACE_LOW("[MMIMP3] MMIAP_SavePlayListInfo list_type=%d",\
                  list_type);

    /*
    * fmm list type  my music list(防止用户往AUDIO文件夹中加文件，记录信息无效) 不保存
    */

    if((PNULL == play_info_ptr)
            || (APLAYER_LIST_TYPE_FMM == list_type) || (APLAYER_LIST_TYPE_MYMUSIC == list_type)
      )
    {
        return ;
    }

    SCI_TRACE_LOW("[MMIMP3] MMIAP_SavePlayListInfo");

    MMIAP_GetReviewInfo(&review_info);


    param_in.context.cur_list_info = play_list_info;
    param_in.context.dyn_playing_info = play_info_ptr->dyn_playing_info;
    param_in.context.active_index = active_index;

    //old info
    param_in.context.old_info.player_state = play_info_ptr->player_state;
    param_in.context.old_info.audio_state = play_info_ptr->audio_state;

    //review info
    param_in.context.review_info = review_info;


    MMIAP_GetHistoryPanelInfo(&param_in.context.panel_info);
    MMIAP_SaveToHistoryFile(&param_in);
}

/*****************************************************************************/
//  Description : set history init flag
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetHistoryInitFlag(BOOLEAN is_init)
{
    s_mp3_is_history_init = is_init;

    SCI_TRACE_LOW("[MMIMP3] MMIAP_SetHistoryInitFlag is_init=%d",
                  is_init);
}

/*****************************************************************************/
//  Description : get history init flag
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsHistoryInit(void)
{
    BOOLEAN is_init = FALSE;


    is_init = s_mp3_is_history_init;

    SCI_TRACE_LOW("[MMIMP3] MMIAP_IsHistoryInit is_init=%d",
                  is_init);

    return is_init;
}

/*****************************************************************************/
//  Description : init history param
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void InitHistoryParam(void)
{
    s_mp3_is_history_init = FALSE;
}

/*****************************************************************************/
//  Description : get history review button valid
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetReviewButtonStatusFromLastInfo(void)
{
    BOOLEAN is_valid = FALSE;


    is_valid = s_ap_history_panel_info.is_review_button_focus;

    SCI_TRACE_LOW("[MMIMP3] MMIAP_GetReviewButtonStatusFromLastInfo is_valid=%d", \
                  is_valid);

    return is_valid;
}

/*****************************************************************************/
//  Description : reset last info nv
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ResetLastInfoNV(void)
{
    MMIAP_HISTORY_NV_INFO_T nv_info = {0};

    
    MMIAP_ResetLastInfoMgrNV(&nv_info);
}
#endif

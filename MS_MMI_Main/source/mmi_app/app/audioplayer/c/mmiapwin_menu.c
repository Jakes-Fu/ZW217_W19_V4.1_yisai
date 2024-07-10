/*****************************************************************************
** File Name:               mmiapwin_menu.c                                  *
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

#define _MMIAPWIN_MENU_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_audioplayer_trc.h"

#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmipub.h"
#include "guilcd.h"
#include "mmi_appmsg.h"


//gui & mmk
#include "guilistbox.h"
#include "guirichtext.h"
#include "guitext.h"
#include "guiedit.h"
#include "guistring.h"
#include "guiref_scale.h"

#include "guiownerdraw.h"
#include "guistatusbar.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif


#include "mmimultim_image.h"


#include "mmi_text.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "wma_adp.h"


//other module
#include "mmi_pubmenuwin.h"
#include "mmi_menutable.h"

//local module
#include "mmimp3_id.h"
#include "mmimp3_text.h"
#include "mmimp3_image.h"

#include "mmiap_play.h"
#include "mmiap_setting.h"
#include "mmiapwin_set.h"
#include "mmimp3_export.h"
#include "mmiapwin_main.h"


#ifdef MMIAP_LYRIC_SUPPORT
#include "mmiap_lyric_internal.h"
#endif

#ifdef MMIAP_F_HISTORY_PLAYLIST
#include "mmiap_lastinfo_ctrl.h"
#endif

#ifdef MMIAP_SPECTRUM_SUPPORT
#include "mmiap_spectrum.h"
#endif

#ifdef MMIAP_MUSIC_LIST_SUPPORT
#include "mmiap_list.h"
#include "mmiapwin_list.h"
#include "mmiapwin_multilist.h"
#endif
#include "mmiap_list_control.h"

#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT
#include "mmiap_album.h"
#endif

#ifdef MMIAP_MUSICMARK_SUPPORT
#include "mmiap_musicmark.h"
#include "mmiapwin_musicmark.h"
#endif

#include "mmiap_control.h"
#include "mmiapwin_common.h"

#include "mmiap_applet.h"
#include "mmiapwin_menu.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
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

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN CopyNodeContext(
    MMIAP_MENU_NODE_T *dst_ptr,//in
    MMIAP_MENU_NODE_T *src_ptr
)
{
    BOOLEAN ret = FALSE;
    uint32 size_of_node = sizeof(MMIAP_MENU_NODE_T);


    if (PNULL != dst_ptr
            && PNULL != src_ptr
       )
    {
        MMI_MEMCPY(dst_ptr, size_of_node,
                   src_ptr, size_of_node,
                   size_of_node
                  );


        ret = TRUE;
    }

    return ret;
}


/*****************************************************************************/
//  Description : 获得list opt return节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListOptRtn(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_OPT_RETURN,
        TXT_MP3_RETURN_TO_PLAY_WIN,
        IMAGE_NULL
    };

    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得list del 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListDel(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_DEL,
        TXT_DELETE,
        IMAGE_NULL

    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得list add节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListAdd(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_ADD,
        TXT_MP3_ADD,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);


    return ret;
}

/*****************************************************************************/
//  Description : 获得list del  all节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListDelAll(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_DEL_ALL,
        TXT_DELALL,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得list search all节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListSearchAll(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_SEARCH_ALL,
        TXT_MP3_SEARCH_ALL_MUSIC,
        IMAGE_NULL

    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得list mark节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListMark(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_MARK,
        STXT_MARK,
#ifdef MMI_PDA_SUPPORT
        IMAGE_COMMON_OPTMENU_MARK
#else
        IMAGE_NULL
#endif
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得sub mark节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSubMark(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SUB_MARK,
        STXT_MARK,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得cancel mark节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfCancelMark(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_CANCEL_MARK,
        TXT_CANCEL_MARK,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得mark all节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfMarkAll(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_MARK_ALL,
        TXT_MARK_ALL,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得cancel all mark 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfCancelAllMark(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_CANCEL_ALL_MARK,
        TXT_CANCEL_MARK_ALL,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得list list节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListList(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_PLAY_LIST_LIST,
        TXT_MP3_MUSIC_LIST,
#ifdef MMI_PDA_SUPPORT
        IMAGE_MP3_MENU_MUSIC_LIST
#else
        IMAGE_NULL
#endif
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得 list list del节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListListDel(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_LIST_DEL,
        TXT_MP3_LIST_DEL,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得 list list rename节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListListRename(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_LIST_RENAME,
        TXT_COMMON_RENAME,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得 list new节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfListNew(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_LIST_NEW,
        TXT_MP3_LIST_NEW,
#ifdef MMI_PDA_SUPPORT
        IMAGE_MP3_MENU_NEW_LIST
#else
        IMAGE_NULL
#endif
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得call ring节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfCallRing(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SET_CALL_RING,
        TXT_COMMON_SET_CALL_RING,
#ifdef MMI_PDA_SUPPORT
        IMAGE_MP3_MENU_SET_CALL_RING
#else
        IMAGE_NULL
#endif
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得set display 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSetDisplay(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_DISPLAY_SET,
        TXT_DISPLAY_SET,
#ifdef MMI_PDA_SUPPORT
        IMAGE_MP3_MENU_DISPLAY_SETTING
#else
        IMAGE_NULL
#endif
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得alarm ring节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfAlarmRing(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SET_ALARM_RING,
        TXT_MP3_SET_ALARM_RING,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得 添加至新建播放列表 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfAddToNewList(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
#ifdef MMIAP_MUSIC_LIST_SUPPORT

#ifdef MMI_PDA_SUPPORT
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_ADD_TO_NEW_LIST,
        TXT_MP3_PDA_ADD_TO_NEW_LIST,
        IMAGE_NULL
    };
#else
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_ADD_TO_NEW_LIST,
        TXT_MP3_ADD_TO_NEW_LIST,
        IMAGE_NULL
    };
#endif



    ret = CopyNodeContext(node_out_ptr, &node_context);
#endif
    return ret;

}

#ifdef MMIAP_MUSICMARK_SUPPORT
/*****************************************************************************/
//  Description : 获得 //添加至音乐书签 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfAddToMusicMark(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

#ifdef MMI_PDA_SUPPORT
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_ADD_TO_MUSIC_MARK,
        TXT_MP3_PDA_ADD_TO_MUSIC_MARK,
        IMAGE_NULL
    };
#else
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_ADD_TO_MUSIC_MARK,
        TXT_MP3_ADD_TO_MUSIC_MARK,
        IMAGE_NULL
    };
#endif


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得music mark节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfMusicMark(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_MUSIC_MARK_MENU,
        TXT_MP3_MUSIC_MARK_MENU,
#ifdef MMI_PDA_SUPPORT
        IMAGE_MP3_MENU_MUSIC_MARK
#else
        IMAGE_NULL
#endif
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}

/*****************************************************************************/
//  Description : 获得add music mark节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfAddMusicMark(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_MARK_ADD,
        TXT_ADD_BOOKMARK,
        IMAGE_COMMON_OPTMENU_ADD
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;

}
#endif

/*****************************************************************************/
//  Description : 获得 //设置 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSetting(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SETTINGS,
        STXT_MAIN_SETTING,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 //退出 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfExit(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_EXIT,
        STXT_EXIT,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 add 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfAdd(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
#if defined MMIAP_MUSIC_LIST_SUPPORT || defined MMIAP_MUSICMARK_SUPPORT

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_ADD,
        STXT_ADD,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);
#endif
    return ret;

}

/*****************************************************************************/
//  Description : 获得 set ring 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSetRing(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SET_RING,
        TXT_MP3_SET_RING,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 set shuffle 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSetShuffle(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

#ifdef CMCC_UI_STYLE
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SET_SHUFFLE,
        TXT_MP3_PLAY_SEQUENCE,
        IMAGE_NULL
    };
#else
    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SET_SHUFFLE,
        TXT_SET_SHUFFLE,
        IMAGE_NULL
    };
#endif


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 set repeat 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSetRepeat(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SET_REPEAT,
        TXT_COMMON_REPEAT,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 review close 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfReviewClose(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
#if defined( MMI_AUDIOPLAYER_REVIEW_SUPPORT) && !defined(TOUCH_PANEL_SUPPORT)


    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_REVIEW,
        TXT_MP3_REVIEW_CLOSE,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);
#endif

    return ret;
}

/*****************************************************************************/
//  Description : 获得 review open 节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfReviewOpen(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_REVIEW,
        TXT_MP3_REVIEW_OPEN,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);
#endif
    return ret;
}

/*****************************************************************************/
//  Description : 获得 add to play list节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfAddToPlayList(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;
#ifdef MMIAP_MUSIC_LIST_SUPPORT

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_ADD_TO_PLAY_LIST,
        TXT_MP3_ADD_TO_SAVED_LIST,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);
#endif

    return ret;
}

/*****************************************************************************/
//  Description : 获得 del from list节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfDelFromList(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_DEL_FROM_LIST,
        TXT_MP3_DEL_FROM_LIST,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 del real节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfDelReal(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_DEL_REAL,
        TXT_MP3_DEL_REAL_MUSIC,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 mms send节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfMMSSend(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SEND_BY_MMS,
        TXT_COMM_MMS_SEND,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : 获得 bt send节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfBTSend(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_SEND_BY_BT,
        TXT_COMMON_SEND_BY_BT,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}
#endif

/*****************************************************************************/
//  Description : 获得 detail节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfDetail(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_DETAIL,
        STXT_DETAIL,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 add to list节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfAddToList(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_ADD_TO_LIST,
/*#ifdef MMI_PDA_SUPPORT
        TXT_MP3_PDA_ADD_TO_SAVED_LIST,
#else*/
        TXT_MP3_ADD_TO_SAVED_LIST,
//#endif
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 send节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSend(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_SEND,
        TXT_SEND,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}
#ifdef MMIAP_SORT_SUPPORT
/*****************************************************************************/
//  Description : 获得 sort节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSort(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_SORT,
        TXT_COMMON_SORT,
#ifdef MMI_PDA_SUPPORT
        IMAGE_COMMON_OPTMENU_SORT
#else
        IMAGE_NULL
#endif
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 sort by none节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSortByNone(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_SORT_BY_NONE,
        TXT_MP3_SORT_BY_NONE,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 sort by time节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSortByTime(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_SORT_BY_TIME,
        TXT_TIME,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 sort by size节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSortBySize(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_SORT_BY_SIZE,
        TXT_COMMON_SIZE,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}

/*****************************************************************************/
//  Description : 获得 sort by name节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfSortByName(
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_SORT_BY_NAME,
        TXT_MP3_SORT_BY_TITLE,
        IMAGE_NULL
    };


    ret = CopyNodeContext(node_out_ptr, &node_context);

    return ret;
}
#endif

/*****************************************************************************/
//  Description : 获得 list list copy to device节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNodeContextOfCopyToDev(
    uint32 menu_id, //in
    MMIAP_MENU_NODE_T *node_out_ptr //out
)
{
    BOOLEAN ret = FALSE;

    MMIAP_MENU_NODE_T node_context =
    {
        ID_MP3_LIST_LIST_COPY,
        TXT_NULL,
        IMAGE_NULL
    };
    MMIFILE_DEVICE_E device = 0;

    if (PNULL != node_out_ptr)
    {
        device = menu_id - node_context.menu_id + MMI_DEVICE_UDISK;
        if (device < MMI_DEVICE_NUM)
        {
            node_context.menu_id = menu_id;
            node_context.text_id = MMIAPIFMM_GetDeviceCopyText(device);

            ret = CopyNodeContext(node_out_ptr, &node_context);
        }
    }

    return ret;

}



/*****************************************************************************/
//  Description : 获得节点ID对应的节点内容
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetNodeContext(
    MMIMP3_ID_MENU_E menu_id, //in
    MMIAP_MENU_NODE_T *node_out_ptr, //out
    uint32 which_value //in 相同ID的 哪个内容
)
{
    BOOLEAN ret = FALSE;


    if (PNULL == node_out_ptr)
    {
        return ret;
    }
#ifdef BT_AV_SNK_SUPPORT
    if(MMIAPIMp3_IsBtMusic())
    {
        if(menu_id == ID_MP3_EXIT)
        {
            ret = GetNodeContextOfExit(node_out_ptr);
        }
    }
    else
#endif
    {
        switch(menu_id)
        {
        case ID_MP3_LIST_OPT_RETURN:
            ret = GetNodeContextOfListOptRtn(node_out_ptr);
            break;
        case ID_MP3_LIST_DEL:
            ret = GetNodeContextOfListDel(node_out_ptr);
            break;
        case ID_MP3_LIST_ADD:
            ret = GetNodeContextOfListAdd(node_out_ptr);
            break;
        case ID_MP3_LIST_DEL_ALL:
            ret = GetNodeContextOfListDelAll(node_out_ptr);
            break;
        case ID_MP3_LIST_SEARCH_ALL:
            ret = GetNodeContextOfListSearchAll(node_out_ptr);
            break;
        case ID_MP3_LIST_MARK:
            ret = GetNodeContextOfListMark(node_out_ptr);
            break;
        case ID_MP3_SUB_MARK:
            ret = GetNodeContextOfSubMark(node_out_ptr);
            break;
        case ID_MP3_CANCEL_MARK:
            ret = GetNodeContextOfCancelMark(node_out_ptr);
            break;
        case ID_MP3_MARK_ALL:
            ret = GetNodeContextOfMarkAll(node_out_ptr);
            break;
        case ID_MP3_CANCEL_ALL_MARK:
            ret = GetNodeContextOfCancelAllMark(node_out_ptr);
            break;
        case ID_MP3_PLAY_LIST_LIST:
            ret = GetNodeContextOfListList(node_out_ptr);
            break;
        case ID_MP3_LIST_LIST_DEL:
            ret = GetNodeContextOfListListDel(node_out_ptr);
            break;
        case ID_MP3_LIST_LIST_RENAME:
            ret = GetNodeContextOfListListRename(node_out_ptr);
            break;
        case ID_MP3_LIST_LIST_NEW:
            ret = GetNodeContextOfListNew(node_out_ptr);
            break;
    
    #ifdef MMIAP_MUSICMARK_SUPPORT
        case ID_MP3_MUSIC_MARK_MENU:
            ret = GetNodeContextOfMusicMark(node_out_ptr);
            break;
        case ID_MP3_MARK_ADD:
            ret = GetNodeContextOfAddMusicMark(node_out_ptr);
            break;
        case ID_MP3_ADD_TO_MUSIC_MARK:
            ret = GetNodeContextOfAddToMusicMark(node_out_ptr);
            break;
    #endif
        case ID_MP3_SET_CALL_RING:
            ret = GetNodeContextOfCallRing(node_out_ptr);
            break;
        case ID_MP3_DISPLAY_SET:
            ret = GetNodeContextOfSetDisplay(node_out_ptr);
            break;
        case ID_MP3_SET_ALARM_RING:
            ret = GetNodeContextOfAlarmRing(node_out_ptr);
            break;
        case ID_MP3_ADD_TO_NEW_LIST:
            ret = GetNodeContextOfAddToNewList(node_out_ptr);
            break;
        case ID_MP3_SETTINGS:
            ret = GetNodeContextOfSetting(node_out_ptr);
            break;
        case ID_MP3_EXIT:
            ret = GetNodeContextOfExit(node_out_ptr);
            break;
        case ID_MP3_ADD:
            ret = GetNodeContextOfAdd(node_out_ptr);
            break;
        case ID_MP3_SET_RING:
            ret = GetNodeContextOfSetRing(node_out_ptr);
            break;
        case ID_MP3_SET_SHUFFLE:
            ret = GetNodeContextOfSetShuffle(node_out_ptr);
            break;
        case ID_MP3_SET_REPEAT:
            ret = GetNodeContextOfSetRepeat(node_out_ptr);
            break;
        case ID_MP3_REVIEW:
            if (which_value)
            {
                ret = GetNodeContextOfReviewOpen(node_out_ptr);
            }
            else
            {
                ret = GetNodeContextOfReviewClose(node_out_ptr);
            }
            break;
        case ID_MP3_ADD_TO_PLAY_LIST:
            ret = GetNodeContextOfAddToPlayList(node_out_ptr);
            break;
        case ID_MP3_LIST_DEL_FROM_LIST:
            ret = GetNodeContextOfDelFromList(node_out_ptr);
            break;
        case ID_MP3_LIST_DEL_REAL:
            ret = GetNodeContextOfDelReal(node_out_ptr);
            break;
        case ID_MP3_SEND_BY_MMS:
            ret = GetNodeContextOfMMSSend(node_out_ptr);
            break;
    #ifdef BLUETOOTH_SUPPORT
        case ID_MP3_SEND_BY_BT:
            ret = GetNodeContextOfBTSend(node_out_ptr);
            break;
    #endif
        case ID_MP3_LIST_DETAIL:
            ret = GetNodeContextOfDetail(node_out_ptr);
            break;
        case ID_MP3_LIST_ADD_TO_LIST:
            ret = GetNodeContextOfAddToList(node_out_ptr);
            break;
        case ID_MP3_LIST_SEND:
            ret = GetNodeContextOfSend(node_out_ptr);
            break;
    #ifdef MMIAP_SORT_SUPPORT
        case ID_MP3_LIST_SORT:
            ret = GetNodeContextOfSort(node_out_ptr);
            break;
        case ID_MP3_LIST_SORT_BY_NONE:
            ret = GetNodeContextOfSortByNone(node_out_ptr);
            break;
        case ID_MP3_LIST_SORT_BY_TIME:
            ret = GetNodeContextOfSortByTime(node_out_ptr);
            break;
        case ID_MP3_LIST_SORT_BY_SIZE:
            ret = GetNodeContextOfSortBySize(node_out_ptr);
            break;
        case ID_MP3_LIST_SORT_BY_NAME:
            ret = GetNodeContextOfSortByName(node_out_ptr);
            break;
    #endif
        default:
            if (ID_MP3_LIST_LIST_COPY <= menu_id
                    && menu_id <= ID_MP3_LIST_LIST_OPT_END
               )
            {
                ret = GetNodeContextOfCopyToDev(menu_id, node_out_ptr);
            }
            break;
        }
    }

    SCI_TRACE_LOW("[MMIMP3] MMIAP_GetNodeContext menu_id=%d, ret=%d", \
                  menu_id, ret);

    return ret;
}

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_InsertMp3NodeByLabel(
    MMI_CTRL_ID_T      ctrl_id,        //控件id
    MMI_IMAGE_ID_T     image_id,       //图片id, for PDA style pop menu
    MMI_TEXT_ID_T      text_id,        //插入的节点的文本
    uint32             node_id,        //节点编号
    uint32             parent_node_id, //父节点编号
    uint16             index           //位置
)
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;

#ifdef MMI_PDA_SUPPORT
    node_item.select_icon_id = image_id;
#endif

    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}

/*****************************************************************************/
//  Description : Set Player Opt Menu Grey Status
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void  SetPlayerOptMenuGreyStatus(
    MMI_CTRL_ID_T  ctrl_id
)
{
    BOOLEAN is_gray = FALSE;
    uint32 total_num = 0;

    MMIAP_STATIC_MENU_GRAY_PARAM_T menu_gray_param = {0};


#ifdef MMI_PDA_SUPPORT
    MMIAP_MENU_ITEM_T menu_group[]=
    {
        //设为来电铃音、设为闹钟铃音
        {ID_MP3_SET_CALL_RING, 0},
        {ID_MP3_SET_ALARM_RING, 0},

        //添加至已存播放列表、至新建播放列表、至音乐书签
        {ID_MP3_ADD_TO_PLAY_LIST, 0},
        {ID_MP3_ADD_TO_NEW_LIST, 0},
        {ID_MP3_ADD_TO_MUSIC_MARK, 0}
    };
#else
    MMIAP_MENU_ITEM_T menu_group[]=
    {
        {ID_MP3_ADD, 0},
        {ID_MP3_SET_RING, 0},
        {ID_MP3_ADD_TO_PLAY_LIST,ID_MP3_ADD},
        {ID_MP3_ADD_TO_NEW_LIST, ID_MP3_ADD},
        {ID_MP3_ADD_TO_MUSIC_MARK, ID_MP3_ADD},
        {ID_MP3_SET_CALL_RING, ID_MP3_SET_RING},
        {ID_MP3_SET_ALARM_RING, ID_MP3_SET_RING}

    };
#endif


    MMIMP3_PLAY_LIST_INFO_T list_info = MMIMP3_GetCurPlayListInfo();
    MMIAP_MENU_ITEM_T menu_group2[]=
    {
        {ID_MP3_MUSIC_MARK_MENU, 0},
#ifdef MMI_PDA_SUPPORT
        {ID_MP3_ADD_TO_MUSIC_MARK, 0}
#else
        {ID_MP3_ADD_TO_MUSIC_MARK, ID_MP3_ADD}
#endif
    };


    total_num = MMIMP3_GetCurPlayListTotalNum();
    if(0 < total_num)
    {
        is_gray = FALSE;
    }
    else
    {
        is_gray = TRUE;
    }

    menu_gray_param.menu_group_ptr = menu_group;
    menu_gray_param.total_num = ARR_SIZE(menu_group);
    menu_gray_param.is_gray = is_gray;
    menu_gray_param.ctrl_id = ctrl_id;
    menu_gray_param.gray_func = PNULL;
    MMIAP_BuildMenuGrayByCtrl(&menu_gray_param);


#ifndef MMI_PDA_SUPPORT
#ifndef TOUCH_PANEL_SUPPORT

    if((MMIMP3_AUDIO_STOPPED != MMIMP3_GetAudioState() &&
            MMIMP3_PLAYER_AUDIO_ACTIVE == MMIAPIMP3_GetPlayerState())

            && MMIMP3_IsCurFileSeekSupport()) //MMIAPIMP3_IsPlayerActive()
    {
        is_gray = FALSE;
    }
    else
    {
        is_gray = TRUE;
    }

    GUIMENU_SetNodeGrayedByNodeId(is_gray, ID_MP3_REVIEW, NULL, ctrl_id);

#endif
#endif //MMI_PDA_SUPPORT



    if(!list_info.is_music_list)
    {
        menu_gray_param.menu_group_ptr = menu_group2;
        menu_gray_param.total_num = ARR_SIZE(menu_group2);
        menu_gray_param.is_gray = TRUE;
        menu_gray_param.ctrl_id = ctrl_id;
        menu_gray_param.gray_func = PNULL;
        MMIAP_BuildMenuGrayByCtrl(&menu_gray_param);
    }
}

/*****************************************************************************/
//  Description :  build dynamic menu to ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_SLISTHANDLE_T MMIAP_CreateDynamicMenu(
    uint32 element_size //in
)
{
    MMI_SLISTHANDLE_T handle = PNULL;


    handle = MMI_SLIST_Create(element_size);

    return handle;
}

/*****************************************************************************/
//  Description :  build dynamic menu to ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyDynamicMenu(
    MMI_SLISTHANDLE_T handle //in
)
{
    MMI_SLIST_Destroy(&handle);
}

/*****************************************************************************/
//  Description :  add item to dynamic menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_AddDynamicMenuItem(
    MMI_SLISTHANDLE_T handle, //in
    MMIAP_MENU_ITEM_T *item_ptr //in
)
{
    MMI_SLIST_Append(handle, item_ptr);
}

/*****************************************************************************/
//  Description :  build dynamic menu to ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_BuildDynamicMenuToCtrl(
    MMI_SLISTHANDLE_T menu_group_handle, //in
    GETNODECONTEXT_CALLBACK func_ptr, //in
    BOOLEAN is_display_image, //in
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN ret = FALSE;

    uint32 i = 0;
    uint32 j = 0;
    uint32 k = 0;


    MMIAP_MENU_ITEM_T *item_ptr = PNULL;
    MMIAP_MENU_NODE_T node = {0};
    MMIMP3_ID_MENU_E menu_id = 0;
    MMIMP3_ID_MENU_E parent_id = 0;

    BOOLEAN is_sub = FALSE;

    MMI_IMAGE_ID_T     image_id = IMAGE_NULL;
    MMI_TEXT_ID_T      text_id = 0;
    BOOLEAN context_ret = FALSE;

    MMI_SLISTNODEHANDLE_T list_node_ptr  = PNULL;



    if (PNULL == menu_group_handle
       )
    {
        return ret;
    }



    MMI_SLIST_FOREACH(menu_group_handle, list_node_ptr)
    {
        item_ptr = (MMIAP_MENU_ITEM_T*)MMI_SLIST_GetElement(list_node_ptr);
        if (PNULL == item_ptr)
        {
            break;
        }


        context_ret = FALSE;
        menu_id = item_ptr->menu_id;


        //当前item 无效，不再是sub menu
        if (0 == item_ptr->parent_id)
        {
            is_sub = FALSE;
            parent_id = 0;
            j = 0;
        }
        else
        {
            is_sub = TRUE;
            parent_id = item_ptr->parent_id;
        }


        if (PNULL != func_ptr)
        {
            context_ret = func_ptr(menu_id,&node);
        }

        if (!context_ret)
        {
            //获得节点内容
            context_ret = MMIAP_GetNodeContext(menu_id,&node, 0);
        }

        if (!context_ret)
        {
            continue;
        }


        //插入节点
        if (is_display_image)
        {
            image_id = node.image_id;
        }
        else
        {
            image_id = IMAGE_NULL;
        }
        text_id = node.text_id;

        if (is_sub)
        {
            k = j;
            j++;
        }
        else
        {
            k = i;
            i++;
        }


        MMIAP_InsertMp3NodeByLabel(
            ctrl_id,
            image_id,
            text_id,
            menu_id,
            parent_id,
            k
        );

    }


    ret = TRUE;

    return ret;
}

/*****************************************************************************/
//  Description : build menu to ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_BuildMenuToCtrl(
    MMIAP_MENU_ITEM_T *menu_group_ptr, //in
    uint32 total_num,//in
    GETNODECONTEXT_CALLBACK func_ptr, //in
    BOOLEAN is_display_image, //in
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN ret = FALSE;

    uint32 i = 0;
    uint32 j = 0;
    uint32 k = 0;
    uint32 count = 0;

    MMIAP_MENU_ITEM_T *item_ptr = PNULL;
    MMIAP_MENU_NODE_T node = {0};
    MMIMP3_ID_MENU_E menu_id = 0;
    MMIMP3_ID_MENU_E parent_id = 0;
    BOOLEAN is_sub = FALSE;

    MMI_IMAGE_ID_T     image_id = IMAGE_NULL;
    MMI_TEXT_ID_T      text_id = 0;
    BOOLEAN context_ret = FALSE;


    if (PNULL == menu_group_ptr
       )
    {
        return ret;
    }

    for(count = 0; count < total_num; count++)
    {
        item_ptr = (MMIAP_MENU_ITEM_T*)(menu_group_ptr + count);
        if (PNULL == item_ptr) /*lint !e774*/
        {
            break;
        }


        context_ret = FALSE;
        menu_id = item_ptr->menu_id;


        //当前item 无效，不再是sub menu
        if (0 == item_ptr->parent_id)
        {
            is_sub = FALSE;
            parent_id = 0;
            j = 0;
        }
        else
        {
            is_sub = TRUE;
            parent_id = item_ptr->parent_id;
        }


        if (PNULL != func_ptr)
        {
            context_ret = func_ptr(menu_id,&node);
        }

        if (!context_ret)
        {
            //获得节点内容
            context_ret = MMIAP_GetNodeContext(menu_id,&node, 0);
        }

        if (!context_ret)
        {
            continue;
        }


        //插入节点
        if (is_display_image)
        {
            image_id = node.image_id;
        }
        else
        {
            image_id = IMAGE_NULL;
        }
        text_id = node.text_id;

        if (is_sub)
        {
            k = j;
            j++;
        }
        else
        {
            k = i;
            i++;
        }


        MMIAP_InsertMp3NodeByLabel(
            ctrl_id,
            image_id,
            text_id,
            menu_id,
            parent_id,
            k
        );

    }


    ret = TRUE;

    return ret;
}

/*****************************************************************************/
//  Description :  add item to dynamic menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_AddDynMenuGrayItem(
    MMI_SLISTHANDLE_T handle, //in
    MMIAP_DYN_MENU_GRAY_T *item_ptr //in
)
{
    MMI_SLIST_Append(handle, item_ptr);
}

/*****************************************************************************/
//  Description :  build dyn menu gray by ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_BuildDynMenuGrayByCtrl(
    MMI_SLISTHANDLE_T menu_group_handle, //in
    MMI_CTRL_ID_T ctrl_id
)
{
    BOOLEAN ret = FALSE;


    uint32 menu_id = 0;
    uint32 parent_id = 0;
    MMIAP_DYN_MENU_GRAY_T *item_ptr = PNULL;

    BOOLEAN is_gray = FALSE;
    MMI_SLISTNODEHANDLE_T list_node_ptr  = PNULL;



    if (PNULL == menu_group_handle
       )
    {
        return ret;
    }



    MMI_SLIST_FOREACH(menu_group_handle, list_node_ptr)
    {
        item_ptr = (MMIAP_DYN_MENU_GRAY_T*)MMI_SLIST_GetElement(list_node_ptr);
        if (PNULL == item_ptr)
        {
            break;
        }

        is_gray = item_ptr->is_gray;
        menu_id = item_ptr->menu_item.menu_id;
        parent_id = item_ptr->menu_item.parent_id;

        GUIMENU_SetNodeGrayedByNodeId(is_gray,
                                      menu_id,
                                      parent_id,
                                      ctrl_id
                                     );

    }


    ret = TRUE;

    return ret;
}

/*****************************************************************************/
//  Description :  build dyn menu gray by ctrl
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_BuildMenuGrayByCtrl(
    MMIAP_STATIC_MENU_GRAY_PARAM_T *menu_gray_param_ptr //in
)
{
    BOOLEAN ret = FALSE;

    uint32 i = 0;

    uint32 menu_id = 0;
    uint32 parent_id = 0;
    MMIAP_MENU_ITEM_T *item_ptr = PNULL;
    MMIAP_MENU_ITEM_T *menu_group_ptr = PNULL;
    uint32 total_num = 0;
    BOOLEAN is_org_gray = FALSE;
    MMI_CTRL_ID_T ctrl_id = 0;
    MENU_GRAY_CALLBACK func = PNULL;


    BOOLEAN is_gray = FALSE;
    BOOLEAN func_ret = FALSE;
    BOOLEAN is_func_gray = FALSE;

    if (PNULL == menu_gray_param_ptr)
    {
        return ret;
    }

    menu_group_ptr = menu_gray_param_ptr->menu_group_ptr;
    total_num = menu_gray_param_ptr->total_num;
    is_org_gray = menu_gray_param_ptr->is_gray;
    ctrl_id = menu_gray_param_ptr->ctrl_id;
    func = menu_gray_param_ptr->gray_func;

    if (PNULL == menu_group_ptr
       )
    {
        return ret;
    }


    for (i = 0; i < total_num; i++)
    {
        item_ptr = (MMIAP_MENU_ITEM_T*)(menu_group_ptr + i);
        if (PNULL == item_ptr) /*lint !e774*/
        {
            break;
        }
        func_ret = FALSE;

        menu_id = item_ptr->menu_id;
        parent_id = item_ptr->parent_id;

        if (PNULL != func)
        {
            is_func_gray = is_org_gray;
            func_ret = func(menu_id, &is_func_gray);
        }

        /*若是FUNC 处理，使用获得的值
        *否则，使用默认的值。
        */
        if (func_ret)
        {
            is_gray = is_func_gray;
        }
        else
        {
            is_gray = is_org_gray;
        }

        GUIMENU_SetNodeGrayedByNodeId(is_gray,
                                      menu_id,
                                      parent_id,
                                      ctrl_id
                                     );

    }


    ret = TRUE;

    return ret;
}


/*****************************************************************************/
//  Description : menu node callback
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenPlayerOptMenuCallback(
    MMIMP3_ID_MENU_E menu_id, /*in*/
    MMIAP_MENU_NODE_T *node_out_ptr /*out*/
)
{
    BOOLEAN is_local = FALSE;
    BOOLEAN ret = FALSE;
    uint32 which_value = 0;


    switch(menu_id)
    {
        //review menu
#ifndef MMI_PDA_SUPPORT
#ifndef TOUCH_PANEL_SUPPORT
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT
    case ID_MP3_REVIEW:
        MMIAP_UpdateReviewMenuStatus();
        if(MMIAP_IsReviewOpen())
        {
            which_value = FALSE;
        }
        else
        {
            which_value = TRUE;
        }
        is_local = TRUE;
        break;
#endif
#endif
#endif
    default:
        break;
    }

    if (is_local
            && PNULL != node_out_ptr
       ) /*lint !e774*/
    {
        ret = MMIAP_GetNodeContext(menu_id,node_out_ptr, which_value);
    }



    return ret;
}

/*****************************************************************************/
//  Description : handle pub menu win ok handler
//  Global resource dependence :
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void HandlePlayerOptPopMenuOK(
    MMIPUB_MENU_NOTIFY_T *param_ptr
)
{
    MMI_MENU_ID_T       menu_id = ID_MP3_EXIT;
    MMI_WIN_ID_T        alert_win_id = MMIMP3_ALERT_WIN_ID;

#ifdef DRM_SUPPORT
    MMIMP3_LIST_FILE_BODY_T cur_file = {0};
#endif
    BOOLEAN         is_create_new_list = FALSE;
    wchar   full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

#ifdef MMIAP_MUSICMARK_SUPPORT
    MMIMP3_PLAY_LIST_INFO_T list_info = MMIMP3_GetCurPlayListInfo();
    MMIFILE_ERROR_E add_result = SFS_NO_ERROR;
#endif


    if (PNULL == param_ptr)
    {
        return;
    }


    menu_id = GUIMENU_GetCurNodeId(param_ptr->ctrl_id);
    switch(menu_id)
    {

#ifdef BT_AV_SNK_SUPPORT
    case ID_MP3_MODE:
        if(!MMIAPIMp3_IsBtMusic())
        {
            // switch to bt music, music should come from smart phone via a2dp
            if(MMIAPIBT_isHfuConnected())
            {
                BT_ADDRESS addr = {0};
                MMIAPIBT_GetHfuConnectedAddress(&addr);
                MMIAPIBT_A2dpSinkConnect(addr);
            }
        }
        else
        {
            // switch to local music
            MMIAPIBT_A2dpSinkDisconnect();
        }
        MMK_CloseWin(param_ptr->win_id);
        break;
#endif
    case ID_MP3_PLAY_LIST_LIST:
#ifdef CMCC_UI_STYLE
        MMIAP_SetOpenMp3Entry(MMIMP3_ENTRY_TYPE_PLAYER);
#endif
#ifdef MMIAP_MUSIC_LIST_SUPPORT
        MMIAPMULTILIST_EnterMultiListWin();
#else
        MMIAPLISTWIN_OpenCurPlayList();
#endif
        break;

#ifdef MMIAP_MUSICMARK_SUPPORT

    case ID_MP3_MUSIC_MARK_MENU:
        MMIAPMUSICMARK_Enter();
        break;
#endif
#ifdef MMIAP_MUSIC_LIST_SUPPORT

    case ID_MP3_ADD_TO_PLAY_LIST:
    case ID_MP3_ADD_TO_NEW_LIST:
        if (ID_MP3_ADD_TO_NEW_LIST == menu_id)
        {
            is_create_new_list = TRUE;
        }
        if(!MMIAPMULTILIST_AddCurFileToList(is_create_new_list))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_CUR_LIST_NOT_SUPPORT_OPERATION, TXT_NULL,
                                        IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }

        break;
#endif
#ifdef MMIAP_MUSICMARK_SUPPORT

    case ID_MP3_ADD_TO_MUSIC_MARK:
        if(!list_info.is_music_list)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_CUR_LIST_NOT_SUPPORT_OPERATION, TXT_NULL,
                                        IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        else
        {
            add_result = MMIMP3_AddMusicMark(PNULL);

            if(SFS_NO_ERROR == add_result)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MP3_SUCCESS_ADD_TO_MUSIC_MARK, TXT_NULL,
                                            IMAGE_PUBWIN_SUCCESS, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else if (SFS_ERROR_FOLD_FULL == add_result)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMON_BOOKMARK_FULL, TXT_NULL,
                                            IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else if(SFS_ERROR_NO_SPACE == add_result)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_NO_SPACE,
                                            TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ADD_FAIL, TXT_NULL,
                                            IMAGE_PUBWIN_FAIL, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }

        break;
#endif //MMIAP_MUSICMARK_SUPPORT

    case ID_MP3_SET_CALL_RING:
#ifdef DRM_SUPPORT
        cur_file.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
        if(MMIMP3_GetCurPlayFileFullPath(cur_file.full_path_name,&(cur_file.full_path_len)))
        {
            if(!MMIAPIDRM_CanDrmFileSetToWSPC(cur_file.full_path_name, cur_file.full_path_len, DRMFILE_SETTING_CALLRING))
            {
                //不可以设置此DRM文件为铃声
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_DRM_NOT_SET_RINGTONE, TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                //                    MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_RINGTONE);
                break;
            }
        }

#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMIAPISET_OpenUssdSelectWin(MMISET_MP3_PLAYER_CALL_RING);
#else
        MMIAPIMP3_SetMp3AsCallRing(MN_DUAL_SYS_1);
#endif
        break;

    case ID_MP3_SET_ALARM_RING:
#ifdef DRM_SUPPORT
        cur_file.full_path_len = FMM_SEARCH_FILENAME_MAXLEN;
        if(MMIMP3_GetCurPlayFileFullPath(cur_file.full_path_name,&(cur_file.full_path_len)))
        {
            if(!MMIAPIDRM_CanDrmFileSetToWSPC(cur_file.full_path_name, cur_file.full_path_len, DRMFILE_SETTING_CALLRING))
            {
                //不可以设置此DRM文件为铃声
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_DRM_NOT_SET_RINGTONE, TXT_NULL, IMAGE_PUBWIN_WARNING, &alert_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
                //                    MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_RINGTONE);
                break;
            }
        }

#endif
        MMIMP3_GetCurPlayFileFullPath(full_path,&full_path_len);
        MMIAPSETWIN_OpenSetAlarmWin(full_path,full_path_len);
        break;

    case ID_MP3_EQ_SET:
        /*
        if (MMIAPIBT_GetActiveBTHeadset())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_BT_NOT_SUPPORT_EQ,TXT_NULL,
                IMAGE_PUBWIN_WARNING,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else*/
    {
        MMIAPSETWIN_OpenEQSetWin();
    }

    break;

    case ID_MP3_DISPLAY_SET:
        MMIAPSETWIN_OpenDisplaySetWin();
        break;

#ifdef BLUETOOTH_SUPPORT

    case ID_MP3_BT_SET:
        MMIAPSETWIN_OpenBTSetWin();
        break;

    case ID_MP3_BT_HEADSET_LIST:
        MMIAPSETWIN_OpenBTHeadSetWin();
        break;
#endif

#if defined BIG5_SUPPORT

    case ID_MP3_CODE_TYPE:
        MMIAPSETWIN_OpenTxtCodeTypeSetWin();
        break;
#endif

    case ID_MP3_SET_REPEAT:
        MMIAPSETWIN_OpenRepeatModeSetWin();
        break;

    case ID_MP3_SET_SHUFFLE:
        MMIAPSETWIN_OpenShuffleModeSetWin();
        break;

#ifndef TOUCH_PANEL_SUPPORT
#ifdef MMI_AUDIOPLAYER_REVIEW_SUPPORT

    case ID_MP3_REVIEW:
        MMIAP_HandleReviewMenu();
        break;
#endif
#endif

        /*
        case ID_MP3_PLAYER_OPT_RETURN:
            MMK_CloseWin(win_id);
            break;
        */
    case ID_MP3_EXIT:
#ifdef BT_DIALER_SUPPORT
    SCI_TRACE_LOW("lzk log. ID_MP3_EXIT Pressed.");
    MMK_SendMsg(MMIMP3_MAIN_PLAY_WIN_ID,MSG_APP_CANCEL,0);
#else
    MMIAPIMP3_StopApplet(FALSE);
#endif
    break;

#ifdef MMI_PDA_SUPPORT
#ifdef MMIAP_LYRIC_SUPPORT

    case ID_MP3_DISPLAY_LYRIC_ON:
    case ID_MP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT:
#endif
    case ID_MP3_DISPLAY_SPECTRUM:
    case ID_MP3_DISPLAY_ANIM:
    case ID_MP3_DISPLAY_ALBUM:
    {
        switch(menu_id)
        {
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT

        case ID_MP3_DISPLAY_ALBUM:
            MMIMP3_SetDisplayStyle(MMIMP3_DISPLAY_ALBUM);
            MMIAP_UpdateAnimCtrlInfo(FALSE);
            break;
#endif
#ifdef MMIAP_LYRIC_SUPPORT

        case ID_MP3_DISPLAY_LYRIC_ON:
            MMIMP3_SetDisplayStyle(MMIMP3_DISPLAY_LYRIC_ON);
            MMIAP_UpdateAnimCtrlInfo(FALSE);
            break;

        case ID_MP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT:
            MMIMP3_SetDisplayStyle(MMIMP3_DISPLAY_LYRIC_ON_WITH_BACKLIGHT);
            MMIAP_UpdateAnimCtrlInfo(FALSE);
            break;
#endif
#ifdef MMIAP_SPECTRUM_SUPPORT

        case ID_MP3_DISPLAY_SPECTRUM:
            MMIMP3_SetDisplayStyle(MMIMP3_DISPLAY_SPECTRUM);
            MMIAP_UpdateAnimCtrlInfo(FALSE);
            break;
#endif

        default:
            break;
        }

        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,
        //IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

    }
    break;
#endif

    case ID_MP3_SETTINGS:
        MMIAPSETWIN_OpenSettingWin();
        break;

    default:
        break;
    }

}

/*****************************************************************************/
//  Description : init player opt pop menu
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
LOCAL void InitPlayerOptPopMenu(
    MMIPUB_MENU_NOTIFY_T *param_ptr
)
{
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	MMI_STRING_T title_text = {0};
#endif
#ifdef MMI_PDA_SUPPORT
//PDA style menu
    MMIAP_MENU_ITEM_T menu_list[]=
    {
        {ID_MP3_PLAY_LIST_LIST, 0},
        {ID_MP3_MUSIC_MARK_MENU, 0},
        {ID_MP3_SET_CALL_RING, 0},
        {ID_MP3_DISPLAY_SET, 0},
        {ID_MP3_ADD_TO_PLAY_LIST, 0},
        {ID_MP3_SET_ALARM_RING, 0},
        {ID_MP3_ADD_TO_NEW_LIST, 0},
        {ID_MP3_ADD_TO_MUSIC_MARK, 0},
        {ID_MP3_SETTINGS, 0},
        {ID_MP3_EXIT, 0}
    };
#else
    MMIAP_MENU_ITEM_T menu_list[]=
    {
        {ID_MP3_PLAY_LIST_LIST,0},
        {ID_MP3_MUSIC_MARK_MENU,0},
        {ID_MP3_ADD, 0},
        {ID_MP3_ADD_TO_PLAY_LIST,ID_MP3_ADD},
        {ID_MP3_ADD_TO_NEW_LIST, ID_MP3_ADD},
        {ID_MP3_ADD_TO_MUSIC_MARK, ID_MP3_ADD},
        {ID_MP3_SET_RING, 0},
        {ID_MP3_SET_CALL_RING, ID_MP3_SET_RING},
#if !defined (CMCC_UI_STYLE) && !defined (MMI_ALARM_DEL_ADD_SUPPORT)
        {ID_MP3_SET_ALARM_RING, ID_MP3_SET_RING},
#endif
        {ID_MP3_SET_SHUFFLE, 0},
        {ID_MP3_SET_REPEAT, 0},
        {ID_MP3_SETTINGS, 0},
        {ID_MP3_REVIEW, 0},
        {ID_MP3_EXIT, 0}
    };
#endif

    MMI_CTRL_ID_T ctrl_id = 0;


    if (PNULL == param_ptr)
    {
        return;
    }

    ctrl_id = param_ptr->ctrl_id;

    MMIAP_CloseVolumePanel();

    MMIAP_BuildMenuToCtrl(menu_list, ARR_SIZE(menu_list),
                          OpenPlayerOptMenuCallback,
                          TRUE,
                          ctrl_id
                         );

#if (defined BT_AV_SNK_SUPPORT || defined BT_AV_CT_SUPPORT) && !defined MMI_PDA_SUPPORT
    if(MMIAPIBT_isHfuConnected())
    {
        if(!MMIAPIMp3_IsBtMusic())
        {
            MMIAP_InsertMp3NodeByLabel(
            ctrl_id,
            IMAGE_NULL,
            TXT_MP3_BT_MODE,
            ID_MP3_MODE,
            0, 
            0
            );
        }
        else
        {
            MMIAP_InsertMp3NodeByLabel(
            ctrl_id,
            IMAGE_NULL,
            TXT_MP3_LOCAL_MODE,
            ID_MP3_MODE,
            0, 
            0
            );
        }
    }
    else
    {
        if(MMIAPIBT_IsA2dpSinkConnected())
        {
            MMIAP_InsertMp3NodeByLabel(
            ctrl_id,
            IMAGE_NULL,
            TXT_MP3_LOCAL_MODE,
            ID_MP3_MODE,
            0, 
            0
            );
        }
    }
#endif

//PDA style menu
#ifdef MMI_PDA_SUPPORT
    //set context menu button style
    {
        GUIMENU_BUTTON_INFO_T   button_info = {0};

        //display sub context menu
        SCI_MEMSET(&button_info, 0, sizeof(button_info));
        button_info.is_static   = FALSE;
        button_info.node_id     = ID_MP3_DISPLAY_SET;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
    }

#endif // MMI_PDA_SUPPORT

    SetPlayerOptMenuGreyStatus(ctrl_id);
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
	MMI_GetLabelTextByLang(STXT_OPTION, &title_text);
	GUIMENU_SetMenuTitle( &title_text,ctrl_id );
#endif
}

/*****************************************************************************/
//  Description : open opt menu
//  Global resource dependence :
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAP_OpenPlayerOptMenu(
    MMI_WIN_ID_T dst_win_id
)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};


    menuparam.group_id = MENU_DYNA;
    menuparam.win_id = (MMI_WIN_ID_T)MMIMP3_PLAYER_OPT_MENU_WIN_ID;
    menuparam.ctrl_id = (MMI_CTRL_ID_T)MMIMP3_PLAYER_OPT_MENU_CTRL_ID;
    menuparam.applet_handle = SPRD_MP3_APPLET_ID;

    menuparam.dst_handle = dst_win_id;
    menuparam.func.OnMenuOk = HandlePlayerOptPopMenuOK;
    menuparam.func.OnMenuInit = InitPlayerOptPopMenu;
    menuparam.menu_style = GUIMENU_STYLE_POPUP;

    MMIPUB_OpenPubMenuWin(&menuparam);
}

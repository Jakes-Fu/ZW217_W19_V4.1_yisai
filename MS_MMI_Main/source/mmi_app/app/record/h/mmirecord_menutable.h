/*****************************************************************************
** File Name:      mmirecord_menutable.h                                     *
** Author:         haiyang.hu                                                *
** Date:           2006/09/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to RECORD menu                          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18     haiyang.hu       Create                                    *
******************************************************************************/

#ifndef _MMIRECORD_MENUTABLE_H_
#define _MMIRECORD_MENUTABLE_H_

#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

    /*-------------------------------------------------------------------------*/
    /*                         TYPES/CONSTANTS                                 */
    /*-------------------------------------------------------------------------*/


//Menu ID
    typedef enum
    {
        MMIRECORD_ID_MENU_START = (MMI_MODULE_RECORD << 16),
        ID_RECORD_STARTRECORD,
        ID_RECORD_LIST,
        ID_RECORD_PLAY,
        //ID_RECORD_APPEND,
        ID_RECORD_DETAIL,
        ID_RECORD_RENAME,
        ID_RECORD_SETTORING,
        //ID_RECORD_SETTOLEAVEWORD,
        ID_RECORD_DELETE,
        ID_RECORD_DELETEALL,
        ID_RECORD_START,
        //ID_RECORD_START_LIST,
        ID_RECORD_STORIGE_POS,
        ID_RECORD_STORIGE_FORMAT,//andy.he_record
        ID_RECORD_STOPPED_PLAY,
        ID_RECORD_AGAIN,
        ID_RECORD_SAVE,
        ID_RECORD_INSERT,
        //ID_RECORD_STOPPED_LIST,
        ID_RECORD_SEND,
        ID_RECORD_SEND_BY_MMS,
        ID_RECORD_SEND_BY_BT,
        ID_RECORD_UDISK,
        ID_RECORD_UDISK_MAX = ID_RECORD_UDISK + MMI_SDCARD_MAX_NUM,
        ID_RECORD_STORIGE_FORMAT_WAV,//andy.he_record
        ID_RECORD_STORIGE_FORMAT_MP3,
        ID_RECORD_STORIGE_FORMAT_AMR,
//    ID_RECORD_STORAGE_CARD,
//#ifdef	 DUAL_TCARD_SUPPORT
//    ID_RECORD_STORAGE_CARD1,/*ID_RECORD_UDISK + MMI_SDCARD_MAX_NUM*/
// #endif
        MMIRECORD_ID_MENU_MAX
    }
    MMIRECORD_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
                 _ITEMCOUNT) \
_TABLEID,

    typedef enum
    {
        MMIRECORD_MENU_LABEL_START = (MMI_MODULE_RECORD << 16),

#include "mmirecord_menutable.def"

        MMIRECORD_MENUTABLE_MAX
    } MMIRECORD_MENU_LABEL_E;

#undef MENU_DEF


    /**---------------------------------------------------------------------------*
     **                         Compiler Flag                                     *
     **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIRECORD_MENUTABLE_H_


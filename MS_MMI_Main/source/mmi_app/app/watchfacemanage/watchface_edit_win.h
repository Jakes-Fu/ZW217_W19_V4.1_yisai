/*****************************************************************************
** File Name:    watchface_edit_win.h                                        *
** Author:        yifan.yang                                                 *
** Date:           8/25/2021                                                 *
** Copyright:    2004 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:  This file is used to describe watchface edit                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/25/2021     yifan.yang            Create                                *
******************************************************************************/
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define LAUNCHER_EDITCLOCKWIN_RECT DP2PX_RECT(48, 48, 48+144, 48+144)
#define LAUNCHER_EDITCLOCKWIN_NUM 4
#define LAUNCHER_EDITCLOCKWIN_LEFTBTNRECT DP2PX_RECT(0, 108, 24, 108+24)
#define LAUNCHER_EDITCLOCKWIN_RIGHTBTNRECT DP2PX_RECT(240-24, 108, 240, 108+24)
/**--------------------------------------------------------------------------*
**                         PUBLIC FUNCTION                                   *
**---------------------------------------------------------------------------*/
/****************************************************************************
//  Description : open watchface eidt window                                *
//  Patameter:  [In]:       None                                            *
                [Out]:      None                                            *
                [Return]:   None                                            *
//  Author: yifan.yang                                                      *
//  Note:                                                                   *
*****************************************************************************/
PUBLIC void WATCHFACE_Edit_Slidepage_Enter(void);

/****************************************************************************
//  Description : update img                                                *
//  Patameter:  [In]:       None                                            *
                [Out]:      None                                            *
                [Return]:   None                                            *
//  Author: yifan.yang                                                      *
//  Note:   update img when add or delete watchface                         *
*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Update_PreviewImg(void);

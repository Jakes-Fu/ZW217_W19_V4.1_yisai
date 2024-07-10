/****************************************************************************
** File Name:      cafmodule.h                                             *
** Author:         James.Zhang                                             *
** Date:           06/16/2009                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create                                 *
**                                                                         * 
****************************************************************************/

#ifndef  _MMI_APPLET_TABLE_H_    
#define  _MMI_APPLET_TABLE_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "mmi_modu_main.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
//mmi applet guid, 0x8000~0xFFFF
//guid≤ªƒ‹÷ÿ∏¥!!!
#define SPRD_IDLE_APPLET_ID   (SPRD_MMI_GUID_START + 1)
#define SPRD_SAMPLE_APPLET_ID (SPRD_MMI_GUID_START + 2)
#define SPRD_WWW_APPLET_ID (SPRD_MMI_GUID_START + 3)
#define SPRD_DOWNLOAD_APPLET_ID (SPRD_MMI_GUID_START + 4)
#define SPRD_VT_APPLET_ID       (SPRD_MMI_GUID_START + 5)
#define SPRD_ANSMACHINE_APPLET_ID  (SPRD_MMI_GUID_START + 6) //answer machine
#define SPRD_DCD_RESET_APPLET_ID (SPRD_MMI_GUID_START + 7)
#define SPRD_GAME_LINK_APPLET_ID (SPRD_MMI_GUID_START + 8)//the game of link
#define SPRD_GAME_BOX_APPLET_ID (SPRD_MMI_GUID_START + 9)//the game of box
//#define SPRD_GAME_QUINTET_APPLET_ID (SPRD_MMI_GUID_START + 10)//the game of quintet
//#define SPRD_GAME_SQUARE_APPLET_ID (SPRD_MMI_GUID_START + 11)//the game of square
#define SPRD_MP3_APPLET_ID (SPRD_MMI_GUID_START + 10)//mp3
#define SPRD_FM_APPLET_ID (SPRD_MMI_GUID_START + 11)//fm
#define SPRD_EBOOK_APPLET_ID (SPRD_MMI_GUID_START + 12)//ebook
#define SPRD_JAVA_APPLET_ID (SPRD_MMI_GUID_START + 13)//java

#ifdef MCARE_V31_SUPPORT
#define MCARE_APPLET_ID (SPRD_MMI_GUID_START + 15) //mcare
#endif
#define SPRD_CAMERA_APPLET_ID (SPRD_MMI_GUID_START + 14)//camera

#ifdef MMIUZONE_SUPPORT
#define SPRD_UZONE_APPLET_ID (SPRD_MMI_GUID_START + 16) //uzone
#endif
#define SPRD_PB_APPLET_ID (SPRD_MMI_GUID_START + 17)//PB
#define SPRD_CC_APPLET_ID       (SPRD_MMI_GUID_START + 18) //cc

#ifdef WRE_SUPPORT
#define SPRD_WRE_APPLET_ID (SPRD_MMI_GUID_START + 19)//WRE
#endif

#ifdef MMI_RECORD_SUPPORT
#define SPRD_RECORD_APPLET_ID (SPRD_MMI_GUID_START + 20)//record
#endif
#ifdef MMI_REFACTOR_SAMPLE
#define SPRD_APPSAMPLE_APPLET_ID       (SPRD_MMI_GUID_START + 21) //APPSAMPLE
#endif
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif

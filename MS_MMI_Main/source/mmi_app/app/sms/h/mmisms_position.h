/*****************************************************************************
** File Name:      mmisms_displaysize.h                                         *
** Author:                                                                   *
** Date:           13/04/2007                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2007     maggie,ren              Creat
******************************************************************************/

#ifndef _MMISMS_POSITION_H
#define _MMISMS_POSITION_H

#ifdef MAINLCD_SIZE_128X160
#define SMS_SENDING_TEXT_RECT       {10,65,118,80}//{10, 75, 229, 105}
#define SMS_SENDING_IMAGE_RECT      {0,108,128,150}//{0, 174, 239, 280}
#elif MAINLCD_SIZE_176X220
#define SMS_SENDING_TEXT_RECT       {10,65,166,105}//{10, 75, 229, 105}
#define SMS_SENDING_IMAGE_RECT      {0,108,176,197}//{0, 174, 239, 280}
#elif MAINLCD_SIZE_240X320
#define SMS_SENDING_TEXT_RECT       {10, 75, 229, 105}
#define SMS_SENDING_IMAGE_RECT      {0, 174, 239, 280}

#elif MAINLCD_SIZE_128X64//WZYTD
#define SMS_SENDING_TEXT_RECT       {10, 75, 229, 105}
#define SMS_SENDING_IMAGE_RECT      {0, 174, 239, 280}

#elif MAINLCD_SIZE_240X400
#define SMS_SENDING_TEXT_RECT       {10, 75, 229, 105}
#define SMS_SENDING_IMAGE_RECT      {0, 174, 239, 280}

#elif MAINLCD_SIZE_320X480
#define SMS_SENDING_TEXT_RECT       {10, 75, 229, 105}
#define SMS_SENDING_IMAGE_RECT      {0, 174, 239, 280}

#else
#error
#endif

#endif //_MMISMS_POSITION_H

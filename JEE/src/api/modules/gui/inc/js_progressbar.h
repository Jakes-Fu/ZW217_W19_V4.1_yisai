/*****************************************************************************
** File Name:     js_progressbar.h                                           *
** Author:           shuting.ma                                              *
** Date:             11/11/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to describe progressbar set api         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2021      shuting.ma            Create                                  *
******************************************************************************/

#ifndef __JS_PROGRESSBAR_H__
#define __JS_PROGRESSBAR_H__

#include "jerry_bindings.h"

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_PUBLIC_HANDLER(js_gui_progressbar_create);
DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_step);
DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_slide);
DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_type);
DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_bg_color);
DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_fg_color);
DECLARE_PUBLIC_HANDLER(js_gui_progressbar_set_percent);
DECLARE_PUBLIC_HANDLER(js_gui_progressbar_destory);

#ifdef __cplusplus
}
#endif

#endif // __JS_PROGRESSBAR_H__


/*****************************************************************************
** File Name:      js_text.h                                                    *
** Author:          miao.liu2                                                    *
** Date:             11/10/2021                                               *
** Copyright:                                                                      *
** Description:    This file is used to describe text set api          *
******************************************************************************
**                         Important Edit History                              *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 11/2021      miao.liu2              Creat
******************************************************************************/

#ifndef __JS_TEXT_H__
#define __JS_TEXT_H__

#include "jerry_bindings.h"

#ifdef __cplusplus
extern "C" {
#endif

DECLARE_PUBLIC_HANDLER(js_gui_text_create);
DECLARE_PUBLIC_HANDLER(js_gui_text_destroy);
DECLARE_PUBLIC_HANDLER(js_gui_text_set_string);
DECLARE_PUBLIC_HANDLER(js_gui_text_set_font);
DECLARE_PUBLIC_HANDLER(js_gui_text_set_align);
DECLARE_PUBLIC_HANDLER(js_gui_text_set_scroll);
DECLARE_PUBLIC_HANDLER(js_gui_text_set_ellipsis_ex);

#ifdef __cplusplus
}
#endif

#endif // __JS_TEXT_H__

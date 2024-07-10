/*****************************************************************************
** File Name:      mmifmm_interface.h                                        *
** Author:                                                                   *
** Date:             12/19/2007                                              *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe fmm interface module        *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2007       liqing.peng          Create                                 *
******************************************************************************/


#ifndef _MMIFMM_INTERFACE_H_
#define _MMIFMM_INTERFACE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmifilearray_export.h"
#include "mmifmm_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
/**--------------------------------------------------------------------------*
 **                         Macro Definition                                 *
 **--------------------------------------------------------------------------*/
#define FMM_FILE_SUFFIX_MAXLEN      12

//typedef enum
//{
//    FMM_OPENWIN_RET_NULL,
//
//    FMM_OPENWIN_SUCCESS,
//    FMM_OPENWIN_FAIL,
//    FMM_OPENWIN_FILE_NOT_EXIST
//}MMIFMM_OPENWIN_RET_E;

//typedef enum
//{
////    WIN_DISP_NULL,  
//
//    WIN_DISP_1LINE,     //单行文本方式
//    WIN_DISP_2LINE      //双行缩略图方式
// }MMIFMM_WINDISP_TYPE_E;

//回调函数类型, is_success表示操作结果是否成功, 结果以 FILEARRAY 列表的方式返回
typedef void (*MMIFMM_OpenFileCallBack)(
                                        BOOLEAN     is_success,
                                        FILEARRAY   file_array
                                        );
typedef BOOLEAN (*MMIFMM_SelectFileCallBack)(
                                            uint16             total_mark_num,
                                            FILEARRAY_DATA_T   * file_data_ptr
                                          );
//typedef struct
//{
//    wchar                   *path_ptr;      //默认打开目录，如果是NULL，则从根目录打开
//    uint16                  path_len;
//    MMIFMM_FILTER_T         *filter_ptr;    //筛选参数
//    MMIFMM_FUNC_TYPE_E      func_type;      //类型
//    BOOLEAN                 is_multi_sel;   //是否多选
//    BOOLEAN                 is_sel_folder; //是否选择文件夹，传给s_is_sel_folder
//    BOOLEAN                 is_internal_call;
//    PROCESSMSG_FUNC         handle_func;    //自定义处理函数
//    FILEARRAY               ret_array;      //输出数组
//    MMIFMM_WINDISP_TYPE_E   disp_type;      //display type
//    MMIFMM_OpenFileCallBack callback;        //回调函数
//    MMIFMM_SelectFileCallBack select_cbk; //针对标记状态的callback,针对有些应用要对标记的文件或文件夹进行操作,
//                                            //如果没有这项需求，传PNULL
// }MMIFMM_OPEN_LAYER_WIN_INFO_T;
/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 打开一个窗口, 显示某一级目录, 并把当前目录下所有文件夹和
//                所有符合筛选条件的文件列出来
//  Return: 用户在窗口中选中文件后, 通过回调函数返回结果给上一级窗口
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIFMM_OpenFileWinByLayerEx(MMIFMM_OPEN_LAYER_WIN_INFO_T *open_layer_info);

/*****************************************************************************/
//  Description : 打开一个窗口, 显示某一级目录, 并把当前目录下所有文件夹和
//                所有符合筛选条件的文件列出来
//  Return: 用户在窗口中选中文件后, 通过回调函数返回结果给上一级窗口
//  参数说明:
//      ret_array： 一定要的，如果为NULL，则不返回文件
//      callback：  一定要的，如果为NULL，则不返回文件
// 
//      path_ptr/path_len：打开目录，如果是NULL，则从根目录打开
//      filter_ptr：  搜索关键字符串，例如"*.mp3"
// 
//      func_type:  一般填 FUNC_FIND_FILE_AND_ALLFOLER，在选择文件的同时会把所有文件夹显示出来
//      is_multi_sel：是否返回多个文件，对于选择单个文件，填FALSE
// 
//      handle_func：   基本用不到，填NULL
//
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIAPIFMM_OpenFileWinByLayer(
                                wchar                   *path_ptr,      //默认打开目录，如果是NULL，则从根目录打开
                                uint16                  path_len,
                                MMIFMM_FILTER_T         *filter_ptr,    //筛选参数
                                MMIFMM_FUNC_TYPE_E      func_type,      //类型
                                BOOLEAN                 is_multi_sel,   //是否多选
                                PROCESSMSG_FUNC         handle_func,    //自定义处理函数
                                FILEARRAY               ret_array,      //输出数组
                                MMIFMM_OpenFileCallBack callback,        //回调函数
                                BOOLEAN                 is_sel_folder //是否选择文件夹，传给s_is_sel_folder,
                                                                        //目前，is_sel_folder参数仅在复制，移动，图片浏览器三个应用为TRUE,其余默认为FALSE,cr164694
                                );


/*****************************************************************************/
//  Description : 打开一个窗口, 显示某一级目录, 并把当前目录下所有文件夹和
//                所有符合筛选条件的文件列出来
//  Return: 用户在窗口中选中文件后, 通过回调函数返回结果给上一级窗口
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIAPIFMM_OpenFileWinByLayerEx(MMIFMM_OPEN_LAYER_WIN_INFO_T *open_layer_info); 
/*****************************************************************************/
//  Description : ComputTitleShow
//  Global resource dependence :
//  Author: liyan.zhu
//  Note:2012.11.20-for cr 91840:使title显示更自适应
/*****************************************************************************/
PUBLIC void MMIFMM_ComputTitleShow(
								   MMI_CTRL_ID_T        tab_ctrl_id,
								   MMI_STRING_T        *title_string,
								   wchar               *current_pathname_ptr,
								   wchar               *title1_ptr
						   );

/*****************************************************************************/
//  Description : set preview picture anim control param
//  Global resource dependence :
//  Author: liyan.zhu
//  Note:CR191213
/*****************************************************************************/
BOOLEAN MMIAPIFMM_IsPreviewPicSucc (
									wchar          *full_path_wstr_ptr,//in
									uint16         full_path_wstr_len //in:字节数
									);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : is show preview icon in list item content
//  Return: 
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetIsHavePreviewIcon(BOOLEAN    is_show);
#endif

/*****************************************************************************/
//  Description :
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIFMM_SetOpenFileWinCurFile(
                                       uint16   *file_name_ptr,
                                       uint16   file_name_len
                                       );

/*****************************************************************************/
//  Description :
//  Return: 
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIFMM_GetOpenFileWinFile(
                                      uint16        index,
                                      FILEARRAY_DATA_T  *file_data_ptr
                                       );
#endif //_MMIFMM_INTERFACE_H_

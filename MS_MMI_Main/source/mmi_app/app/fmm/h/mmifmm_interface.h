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
//    WIN_DISP_1LINE,     //�����ı���ʽ
//    WIN_DISP_2LINE      //˫������ͼ��ʽ
// }MMIFMM_WINDISP_TYPE_E;

//�ص���������, is_success��ʾ��������Ƿ�ɹ�, ����� FILEARRAY �б�ķ�ʽ����
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
//    wchar                   *path_ptr;      //Ĭ�ϴ�Ŀ¼�������NULL����Ӹ�Ŀ¼��
//    uint16                  path_len;
//    MMIFMM_FILTER_T         *filter_ptr;    //ɸѡ����
//    MMIFMM_FUNC_TYPE_E      func_type;      //����
//    BOOLEAN                 is_multi_sel;   //�Ƿ��ѡ
//    BOOLEAN                 is_sel_folder; //�Ƿ�ѡ���ļ��У�����s_is_sel_folder
//    BOOLEAN                 is_internal_call;
//    PROCESSMSG_FUNC         handle_func;    //�Զ��崦����
//    FILEARRAY               ret_array;      //�������
//    MMIFMM_WINDISP_TYPE_E   disp_type;      //display type
//    MMIFMM_OpenFileCallBack callback;        //�ص�����
//    MMIFMM_SelectFileCallBack select_cbk; //��Ա��״̬��callback,�����ЩӦ��Ҫ�Ա�ǵ��ļ����ļ��н��в���,
//                                            //���û���������󣬴�PNULL
// }MMIFMM_OPEN_LAYER_WIN_INFO_T;
/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��һ������, ��ʾĳһ��Ŀ¼, ���ѵ�ǰĿ¼�������ļ��к�
//                ���з���ɸѡ�������ļ��г���
//  Return: �û��ڴ�����ѡ���ļ���, ͨ���ص��������ؽ������һ������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIFMM_OpenFileWinByLayerEx(MMIFMM_OPEN_LAYER_WIN_INFO_T *open_layer_info);

/*****************************************************************************/
//  Description : ��һ������, ��ʾĳһ��Ŀ¼, ���ѵ�ǰĿ¼�������ļ��к�
//                ���з���ɸѡ�������ļ��г���
//  Return: �û��ڴ�����ѡ���ļ���, ͨ���ص��������ؽ������һ������
//  ����˵��:
//      ret_array�� һ��Ҫ�ģ����ΪNULL���򲻷����ļ�
//      callback��  һ��Ҫ�ģ����ΪNULL���򲻷����ļ�
// 
//      path_ptr/path_len����Ŀ¼�������NULL����Ӹ�Ŀ¼��
//      filter_ptr��  �����ؼ��ַ���������"*.mp3"
// 
//      func_type:  һ���� FUNC_FIND_FILE_AND_ALLFOLER����ѡ���ļ���ͬʱ��������ļ�����ʾ����
//      is_multi_sel���Ƿ񷵻ض���ļ�������ѡ�񵥸��ļ�����FALSE
// 
//      handle_func��   �����ò�������NULL
//
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIAPIFMM_OpenFileWinByLayer(
                                wchar                   *path_ptr,      //Ĭ�ϴ�Ŀ¼�������NULL����Ӹ�Ŀ¼��
                                uint16                  path_len,
                                MMIFMM_FILTER_T         *filter_ptr,    //ɸѡ����
                                MMIFMM_FUNC_TYPE_E      func_type,      //����
                                BOOLEAN                 is_multi_sel,   //�Ƿ��ѡ
                                PROCESSMSG_FUNC         handle_func,    //�Զ��崦����
                                FILEARRAY               ret_array,      //�������
                                MMIFMM_OpenFileCallBack callback,        //�ص�����
                                BOOLEAN                 is_sel_folder //�Ƿ�ѡ���ļ��У�����s_is_sel_folder,
                                                                        //Ŀǰ��is_sel_folder�������ڸ��ƣ��ƶ���ͼƬ���������Ӧ��ΪTRUE,����Ĭ��ΪFALSE,cr164694
                                );


/*****************************************************************************/
//  Description : ��һ������, ��ʾĳһ��Ŀ¼, ���ѵ�ǰĿ¼�������ļ��к�
//                ���з���ɸѡ�������ļ��г���
//  Return: �û��ڴ�����ѡ���ļ���, ͨ���ص��������ؽ������һ������
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_OPENWIN_RET_E MMIAPIFMM_OpenFileWinByLayerEx(MMIFMM_OPEN_LAYER_WIN_INFO_T *open_layer_info); 
/*****************************************************************************/
//  Description : ComputTitleShow
//  Global resource dependence :
//  Author: liyan.zhu
//  Note:2012.11.20-for cr 91840:ʹtitle��ʾ������Ӧ
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
									uint16         full_path_wstr_len //in:�ֽ���
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

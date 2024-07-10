/*****************************************************************************
** File Name:      mmimemo_func.h                                          *
** Author:          fen.xie                                                    *
** Date:            05/08/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the memo             *
*****************************************************************************/
#ifndef MEMO_FUNC_H
#define MEMO_FUNC_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "mmi_module.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "guilistbox.h"
#include "mmipub.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/    
#define MMIMEMO_LIST_MAX_NUM        1000

#define MMIMEMO_DIR  (g_memo_dir)
#define MMIMEMO_CONTENT_MAX_LEN        1000

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                             *
 **--------------------------------------------------------------------------*/    

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : MMIMEMO_Init
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMEMO_Init(
                        MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                        MMI_MESSAGE_ID_E    msg_id);
                        
/*****************************************************************************/
// Description : load the file array for memo, and init the s_memo_info
// Global resource dependence :s_memo_info
// Author: fenx.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_LoadFileArray(
                        MMI_WIN_ID_T        win_id,         //如果win_id和msg_id都为0，则采用同步方式搜索文件
                        MMI_MESSAGE_ID_E    msg_id);
                        
/*****************************************************************************/
// Description : after load the file array for memo, update the s_memo_info
// Global resource dependence :s_memo_info
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_LoadFinish(void);

/*****************************************************************************/
// Description : destory the file array for memo
// Global resource dependence :s_memo_info
// Author: fenx.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_DestroyFileArray(void);

/*****************************************************************************/
// Description : get the total num of memo
// Global resource dependence :s_memo_info
// Author: fenx.xie
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMEMO_GetTotalNum(void);

/*****************************************************************************/
// Description : Get the content of one memo
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_GetContent(
                            uint16 in_index, //in
                            MMI_STRING_T *out_content_ptr,//in/out:out_content->wstr_ptr need free outside
                            uint32  max_wstr_len, //in,允许的wstr最大长度   
                            SCI_DATE_T  *modify_date_ptr,   //è??ú
                            SCI_TIME_T  *modify_time_ptr    //ê±??
                            );
/*****************************************************************************/
// Description : Get the file name of one memo
// Global resource dependence :
// Author: guodong.liu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_GetFileName(
                            uint16 in_index, //in
                            wchar *out_filename_ptr,//in/out:out_content->wstr_ptr need free outside
                            uint32  max_wstr_len //in,允许的wstr最大长度 
                            );
/*****************************************************************************/
// Description : Deletes one memo specified by in_index
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_DelOne(uint16 in_index);

/*****************************************************************************/
// Description : Adds a new memo
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_AddOne(MMI_STRING_T in_url_str, //IN
                                            BOOLEAN in_for_widget_memo, //IN
                                            uint16 *out_index//OUT:new index
                                            );
                                            
/*****************************************************************************/
//  Description : MMIMEMO_IsNoSpace
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 是否为空间不足导致的保存失败
/*****************************************************************************/
PUBLIC BOOLEAN MMIMEMO_IsNoSpace(void);
                                            
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


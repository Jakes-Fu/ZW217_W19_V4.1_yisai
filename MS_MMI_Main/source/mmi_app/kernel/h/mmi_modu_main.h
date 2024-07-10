/*****************************************************************************
** File Name:      mmi_modu_main.h                                                   *
** Author:                                                                   *
** Date:           11/17/2005                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2005       wancan.you       Create
******************************************************************************/

#ifndef _MMI_MODU_MAIN_H_
#define _MMI_MODU_MAIN_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_app.h"
#include "guimenu.h"
#include "mmi_ring.h"
#include "mmi_font.h"
#include "mn_api.h"
#include "mmi_resource_def.h"

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

#ifdef MMI_WINIDNAME_SUPPORT
    #define MMI_RegWinIdNameArr(module_id, win_id_name_ptr)     MMI_RegWinIdNameArrFunc(module_id, win_id_name_ptr)
#else
    //release版本关闭autotest，一来减少错误，二来减少const变量所占内存
    #define MMI_RegWinIdNameArr(module_id, win_id_name_ptr)
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
#ifdef MMI_WINIDNAME_SUPPORT
/*****************************************************************************/
//  Description : register the win id name array
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegWinIdNameArrFunc(uint16 module_id, const uint8** win_id_name_ptr);
#endif

/*****************************************************************************/
//  Description : register the menu group for every module  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegMenuGroup(uint16 moduleid, const GUIMENU_GROUP_T *menu_table);

/*****************************************************************************/
//  Description : get the menu group for every module  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC GUIMENU_GROUP_T* MMI_GetMenuGroup(
                                         MMI_MENU_GROUP_ID_T group_id
                                         );

/*****************************************************************************/
//  Description : get Text ptr  by Label
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC const wchar* MMI_GetLabelText(MMI_TEXT_ID_T label);

/*****************************************************************************/
//  Description : get Text English
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_GetLabelTextEnglish(MMI_TEXT_ID_T label,
                                            MMI_STRING_T     *str_ptr   //[out] the data of text
                                            );
/*****************************************************************************/
//  Description : to get the text by label and language
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMI_GetLabelTextByLang( 
                                   MMI_TEXT_ID_T    label,     //[in] the label 
                                   MMI_STRING_T     *str_ptr   //[out] the data of text
                                   );

/*****************************************************************************/
//  Description : to get the text by label and language
//  Global resource dependence : 
//  Author:
//  Note: Application should Free str_ptr->wstr_ptr
/*****************************************************************************/
PUBLIC void MMI_GetLabelTextByLangEx( 
                                   MMI_TEXT_ID_T    label,     //[in] the label 
                                   MMI_STRING_T     *str_ptr   //[out] the data of text
                                   );

/*****************************************************************************/
//  Description : to get chinese label text
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
//PUBLIC void MMI_GetChineseLabelText( 
//                                   MMI_TEXT_ID_T    label,     //[in] the label 
//                                   MMI_STRING_T     *str_ptr   //[out] the data of text
//                                    );

/*****************************************************************************/
//  Description : to get english label text
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMI_GetEnglishLabelText( 
                                   MMI_TEXT_ID_T    label,     //[in] the label 
                                   MMI_STRING_T     *str_ptr   //[out] the data of text
                                   );

/*****************************************************************************/
//  Description : check the text whether is NULL
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_IsTextNull(MMI_TEXT_ID_T label);

/*****************************************************************************/
//  Description : to get the ring by label
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_GetRingInfo( MMI_RING_ID_T label, MMIAUD_RING_DATA_INFO_T * pRingInfo );

/*****************************************************************************/
//  Description : to get the data by label
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_GetDataInfo(MMI_DATA_ID_T label, MMIRES_DATA_INFO_T* dataInfo);

/*****************************************************************************/
//  Description : register module nv len and max nv item 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegModuleNv(uint16 module_id, const uint16 nv_len[], uint16 nv_max_item);

/*****************************************************************************/
//  Description : Read NV info by id  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMI_ReadNVItem(uint32 item_id, void * buffer_ptr);

/*****************************************************************************/
//  Description : write NV info by id  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_WriteNVItem(uint32 item_id, void * buffer_ptr);

/*****************************************************************************/
//  Description : get true NV item by item id in module 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMI_GetTrueNVItemId(                      //RETURN: 
                                  uint32    item_id     //IN: 
                                  );
/*****************************************************************************/
//  Description : set reg modle nv start flag
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_RegModuleNvStart (void);

/*****************************************************************************/
//  Description : set reg modle nv end flag
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_RegModuleNvEnd (void);

/*****************************************************************************/
//  Description : load resource to ram (text and font)
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMI_LoadResource(void);

/*****************************************************************************/
//  Description : free resource
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMI_FreeRes(uint32 win_id);

/*****************************************************************************/
//  Description : free all resource
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMI_FreeAllRes(void);

/*****************************************************************************/
//  Description : free Wall paper resource
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMI_FreeWallPaperRes(uint32 win_id);

/*****************************************************************************/
//  Description : load new text resource to ram when changing language
//  Global resource dependence : none
//  Author: yongwei.he
//  Note:
/*****************************************************************************/
PUBLIC void MMI_LoadNewTextRes(void);

/*****************************************************************************/
//  Description : check whether all module autotest win_id_name is registered
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_CheckAutoTestWinId(void);

/*****************************************************************************/
//  Description : init winid name arr
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_InitilizeWinIdNameArr(void);

/*****************************************************************************/
//  Description : register the applet info
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegAppletInfo(
                              uint16                          module_id, 
                              const CAF_STATIC_APPLET_INFO_T* applet_info_ptr
                              );

/*****************************************************************************/
//  Description : get the applet info
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC CAF_STATIC_APPLET_INFO_T** MMI_GetAppletInfo(
                                                    uint32* num_ptr
                                                    );


#ifdef MMI_WINIDNAME_SUPPORT
/*****************************************************************************/
//  Description : register the menu group for every module  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_GetWinIdNameArr(MMI_WIN_ID_T win_id, uint8 **name_arr_ptr);

#endif
/*****************************************************************************/
//  Description :  get nv item num of all mmi mudule 
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		
/*****************************************************************************/
PUBLIC uint32 MMI_GetAllMMINVItem(void);

/*****************************************************************************/
//  Description :  get nv size of all module, including ps ref efs,etc
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetAllNVSize(void);

/*****************************************************************************/
//     Description : get sum size of all modules nv size
//    Global resource dependence : 
//  Author: gang.tong,modified by ying.xu   
//    Note: 2010.08.17
/*****************************************************************************/
PUBLIC uint32 MMI_GetAllModuleSumNvSize(void);

#ifndef WIN32
/*****************************************************************************/
//  Description : get freg handler
//  Global resource dependence :                          
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMI_GetFreqHandler(void);
#endif

#if 0
/*****************************************************************************/
//  Description :  calulator minimun nv config 
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetFlashSecterSize(void) ;
/*****************************************************************************/
//  Description :  GetConfigurableFlashSecterNum
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetConfigurableFlashSecterNum(void);

/*****************************************************************************/
//  Description :  calulator minimun nv config 
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetMinimumNVConfig(void);

/*****************************************************************************/
//  Description :  calulator optium  nv config 
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetOptimumNVConfig(void);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif


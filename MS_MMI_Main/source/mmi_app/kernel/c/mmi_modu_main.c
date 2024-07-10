/*****************************************************************************
** File Name:      mmi_modu_main.c                                               *
** Author:                                                                   *
** Date:           11/21/2005                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to control mmi module                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2005       wancan.you     Creat
******************************************************************************/

//#ifndef _MMI_MODU_MAIN_C_
#define _MMI_MODU_MAIN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_kernel_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_modu_main.h"
//#include "mmi_resource_def.h"
#include "mmi_module.h" 
#include "mmi_image.h"
#include "mmi_anim.h"
#include "mmi_text.h" 
#include "mmi_ring.h"
#include "mmi_font.h"
#include "mmi_menutable.h"
#include "mmi_nv.h"
#include "mn_api.h"
#include "nvitem.h"
#include "block_mem.h"
//#include "mmi_id.h"
#include "mmidc_export.h"
#include "mmiset_nv.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_resource.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmipb_export.h"
#include "mmk_app.h"
#include "mmisms_export.h"
#include "flash.h"
#include "mmifm_export.h"
#include "mmicc_export.h"
#include "mmisrv_nv.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define MMI_UINV_USER_BASE		(MMISRV_GetEndId())
#define MMI_UINV_MAX_ID         (MMISRV_GetMaxId())

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL BOOLEAN       s_is_nv_reg_done = FALSE;
LOCAL uint16        s_mmi_nv_max_item[MMI_MAX_MODU_NUM] = {0};  // 每个模块最大的NV的数目
LOCAL const uint16  *s_mmi_nv_len[MMI_MAX_MODU_NUM];            //每个模块各个nv的长度
LOCAL uint32        s_mmi_nv_num = 0;                           //MMI模块的NV数目       
LOCAL const GUIMENU_GROUP_T     *s_gui_menu_group_ptr[MMI_MAX_MODU_NUM];    // 静态菜单group组指针
#ifdef MMI_WINIDNAME_SUPPORT
    LOCAL const uint8     **s_win_id_name_ptr[MMI_MAX_MODU_NUM] = {0};    // win id name数组
#endif
LOCAL uint8 s_win_id_name_result[MMI_WIN_ID_NAME_MAX_LENGTH+1] = {0};

LOCAL const CAF_STATIC_APPLET_INFO_T* s_static_applet_array[MMI_MAX_MODU_NUM] = {0};
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
#ifdef MMI_SIM_LANGUAGE_SUPPORT
extern uint8 g_ksp_Lang; // 0代表auto, 1 代码英文，
#else
extern uint8 g_ksp_Lang; // 0代表英文，
#endif

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#ifdef WIN32
// declaration for WIN32 simulator 
NORMCP_ENTITY_PTR NORMCP_GetEntity(uint32 manu_id, uint32 devc_id, uint32 extend_id, uint32 *index);
void FLASH_GetID(uint32 *pManu_id, uint32 *pDev_id, uint32 *pExtend_id);
#endif

/*****************************************************************************/
//  Description : get module max nv item num 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNVMAXItem(uint16 module_id);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                                                             */
/*---------------------------------------------------------------------------*/

#ifdef MMI_WINIDNAME_SUPPORT
/*****************************************************************************/
//  Description : init winid name arr
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_InitilizeWinIdNameArr(void)
{
    uint32 i = 0;
    
    //SCI_TRACE_LOW:"mmi_modu_main.c enter MMI_InitilizeWinIdNameArr()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_101_112_2_18_2_0_10_216,(uint8*)"");
    
    for (i = 0; i < MMI_MAX_MODU_NUM; i++)
    {
        s_win_id_name_ptr[i] = PNULL;
    }
}

/*****************************************************************************/
//  Description : register the win id name array
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegWinIdNameArrFunc(uint16 module_id, const uint8** win_id_name_ptr)
{
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/

    s_win_id_name_ptr[module_id] = (const uint8**)win_id_name_ptr;
}


/*****************************************************************************/
//  Description : register the menu group for every module  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_GetWinIdNameArr(MMI_WIN_ID_T win_id, uint8 **name_arr_ptr)
{
    uint16 module_id = 0;
    uint16 id_offset = 0;
    const uint8 *win_id_name_ptr = PNULL;

    module_id = MMICOM_GET_WORD_HB(win_id);
    id_offset = MMICOM_GET_WORD_LB(win_id);

    //SCI_TRACE_LOW:"module_id = %d, id_offset = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_136_112_2_18_2_0_10_217,(uint8*)"dd", module_id, id_offset);
    
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/

    win_id_name_ptr = (const uint8*)s_win_id_name_ptr[module_id];
    
    //返回窗口名字
    //第一个为BEGIN_ID，不对应实际窗口
    if ( win_id_name_ptr && 0 != id_offset )    
    {
        MMI_MEMCPY(
            s_win_id_name_result, 
            MMI_WIN_ID_NAME_MAX_LENGTH, 
            (win_id_name_ptr + (id_offset - 1) * MMI_WIN_ID_NAME_MAX_LENGTH), 
            MMI_WIN_ID_NAME_MAX_LENGTH, 
            MMI_WIN_ID_NAME_MAX_LENGTH);
    }
    else    //直接返回窗口数字
    {
        sprintf((char*)s_win_id_name_result, "%ld", win_id);
    }

    *name_arr_ptr = s_win_id_name_result;
}

#endif

/*****************************************************************************/
//  Description : register the menu group for every module  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegMenuGroup(uint16 module_id, const GUIMENU_GROUP_T *c_menu_table)
{
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/

    if (PNULL == c_menu_table)
    {
        SCI_TRACE_HIGH("mmi_modu_main.c MMI_RegMenuGroup: PNULL pointer!");/*assert verified*/
    }

    s_gui_menu_group_ptr[module_id] = (const GUIMENU_GROUP_T*)c_menu_table;
}

/*****************************************************************************/
//  Description : get the menu group for every module  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC GUIMENU_GROUP_T* MMI_GetMenuGroup(
                                         MMI_MENU_GROUP_ID_T group_id
                                         )
{
    uint16              module_id = MMICOM_GET_WORD_HB(group_id);
    uint16              offset = MMICOM_GET_WORD_LB(group_id);
    GUIMENU_GROUP_T*    group_ptr = PNULL;

    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/

    if (0 == group_id)
    {
        return NULL;
    }

    group_ptr = (GUIMENU_GROUP_T *)&s_gui_menu_group_ptr[module_id][offset];

    return group_ptr;
}

/*****************************************************************************/
//  Description : get Image ptr  by Label
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note: add this fuc to get size of image,the fuc is temp for test
/*****************************************************************************/
PUBLIC const uint8 *MMI_GetLabelImage(
                                      MMI_IMAGE_ID_T label,
                                      MMI_WIN_ID_T win_id,
                                      uint32 *size
                                      )
{
	MMI_HANDLE_TYPE_E handle_type = MMI_HANDLE_NONE;
    win_id = MMK_ConvertIdToHandle(win_id);
	//SCI_ASSERT(win_id != 0); /*assert verified*/
	handle_type = MMK_GetHandleType(win_id);	

    if ( MMI_HANDLE_NONE == handle_type )
    {
        //SCI_TRACE_LOW:"[RES]: image handle is not belong window, type = 0x%x, label = 0x%x, win_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_220_112_2_18_2_0_10_218,(uint8*)"ddd", handle_type, label, win_id);
        return PNULL;
    }

	//SCI_PASSERT(MMI_HANDLE_NONE != handle_type, ("handle_type = 0x%x", handle_type)); /*assert verified*/
// 	if(MMI_HANDLE_WINDOW != handle_type)
// 	{
// 		SCI_TRACE_LOW("[RES]: image handle is not belong window, type = 0x%x, label = 0x%x, win_id = 0x%x", handle_type, label, win_id);
// 	}
    return MMIRES_GetImage( label, win_id, size );
}

/*****************************************************************************/
//  Description : get Image ptr  by Label and win
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC const uint8* MMI_GetLabelAnim(
                                     MMI_ANIM_ID_T  label,      //in:
                                     MMI_WIN_ID_T   win_id,     //in:
                                     uint32         *size_ptr   //in:may PNULL
                                     )
{
    MMI_HANDLE_TYPE_E handle_type = MMI_HANDLE_NONE;
    win_id = MMK_ConvertIdToHandle(win_id);
	//SCI_ASSERT(win_id != 0); /*assert verified*/
	handle_type = MMK_GetHandleType(win_id);	

    if ( MMI_HANDLE_NONE == handle_type )
    {
        //SCI_TRACE_LOW:"[RES]: anim handle is not belong window, type = 0x%x, label = 0x%x, win_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_249_112_2_18_2_0_10_219,(uint8*)"ddd", handle_type, label, win_id);
        return PNULL;
    }

	//SCI_PASSERT(MMI_HANDLE_NONE != handle_type, ("handle_type = 0x%x", handle_type)); /*assert verified*/
// 	if(MMI_HANDLE_WINDOW != handle_type)
// 	{
// 		SCI_TRACE_LOW("[RES]: anim handle is not belong window, type = 0x%x, label = 0x%x, win_id = 0x%x", handle_type, label, win_id);
// 	}
    return MMIRES_GetAnim(label, win_id, size_ptr); 
}
    
/*****************************************************************************/
//  Description : get Text ptr  by Label
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC const wchar* MMI_GetLabelText(MMI_TEXT_ID_T label)
{
    MMI_STRING_T str = {0};
    //SCI_PASSERT( MMI_SYS_RES_FLAG == ( label & MMI_SYS_RES_FLAG ), ("please use MMIRES_GetText(....) instead of MMI_GetLabelText()")); /*assert verified*/
    return MMIRES_GetText(label, MMK_GetFirstAppletHandle(), &str);
}   /*-- end of ksp_GetLabelText() --*/

/*****************************************************************************/
//  Description : get Text English
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_GetLabelTextEnglish(MMI_TEXT_ID_T label,
                                            MMI_STRING_T     *str_ptr   //[out] the data of text
                                            )
{   
    uint8 old_lang = g_ksp_Lang;

#ifdef MMI_SIM_LANGUAGE_SUPPORT
    //g_ksp_Lang = 0;
    g_ksp_Lang = 1; // 1 ---english,see in MMISET_LANGUAGE_TYPE_E
#else
    g_ksp_Lang = 0;
#endif

    if( (MMI_SYS_RES_FLAG == ( label & MMI_SYS_RES_FLAG ) ) || (0 == label))
    {
        MMIRES_GetText(label, MMK_GetFirstAppletHandle(), str_ptr);
    }
    else
    {
        //SCI_PASSERT( MMI_SYS_RES_FLAG == ( label & MMI_SYS_RES_FLAG ), ("please use MMIRES_GetText(....) instead of MMI_GetLabelTextEnglish()")); /*assert verified*/
    }
    g_ksp_Lang = old_lang;
}   /*-- end of ksp_GetLabelText() --*/

/*****************************************************************************/
//  Description : load new text resource to ram when changing language
//  Global resource dependence : none
//  Author: yongwei.he
//  Note:
/*****************************************************************************/
PUBLIC void MMI_LoadNewTextRes(void)
{
    MMIRES_LoadNewTextRes();  
}
/*****************************************************************************/
//  Description : to get the text by label and language
//  Global resource dependence : 
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMI_GetLabelTextByLang( 
                                   MMI_TEXT_ID_T    label,     //[in] the label 
                                   MMI_STRING_T     *str_ptr   //[out] the data of text
                                   )
{
//    uint8* re_str_ptr = NULL;

    //SCI_ASSERT( PNULL != str_ptr ); /*assert verified*/
    if ( PNULL == str_ptr )
    {
        return;
    }
    
    if( (MMI_SYS_RES_FLAG == ( label & MMI_SYS_RES_FLAG ) ) || (0 == label))
    {
        MMIRES_GetText(label, MMK_GetFirstAppletHandle(), str_ptr);
    }
    else
    {
        /* please use MMIRES_GetText(....) instead of MMI_GetLabelTextByLang() */
        //SCI_PASSERT(0, ("please use MMIRES_GetText(....) instead of MMI_GetLabelTextByLang()")); /*assert verified*/
    }
    return ;
}

/*****************************************************************************/
//  Description : to get the text by label and language
//  Global resource dependence : 
//  Author:
//  Note: Application should Free str_ptr->wstr_ptr
/*****************************************************************************/
PUBLIC void MMI_GetLabelTextByLangEx( 
                                   MMI_TEXT_ID_T    label,     //[in] the label 
                                   MMI_STRING_T     *str_ptr   //[out] the data of text
                                   )
{    
    if( (MMI_SYS_RES_FLAG == ( label & MMI_SYS_RES_FLAG ) ) || (0 == label))
    {
        MMIRES_GetTextEx(label, MMK_GetFirstAppletHandle(), str_ptr);
    }
    else
    {
        /* please use MMIRES_GetTextEx(....) instead of MMI_GetLabelTextByLangEx() */
        //SCI_PASSERT(0, ("please use MMIRES_GetTextEx(....) instead of MMI_GetLabelTextByLangEx()")); /*assert verified*/
    }
    return ;
}

/*****************************************************************************/
//  Description : check the text whether is NULL
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_IsTextNull(MMI_TEXT_ID_T label)
{
    BOOLEAN is_text_null = FALSE;

    // added COMMON_TXT_NULL by @arthur
    // bug fixed for some assert when using COMMON_TXT_NULL
    //  in menu.
    if( TXT_NULL == label || COMMON_TXT_NULL == label || 0 == MMICOM_GET_WORD_LB(label))
    {
        is_text_null = TRUE;
    }

    return is_text_null;
}

/*****************************************************************************/
//  Description : to get the ring by label
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_GetRingInfo(MMI_RING_ID_T label, MMIAUD_RING_DATA_INFO_T * pRingInfo)
{
    if( MMI_SYS_RES_FLAG == ( label & MMI_SYS_RES_FLAG ) )
    {
        return MMIRES_GetRingInfo( label, MMK_GetFirstAppletHandle(), pRingInfo);
    }
    else
    {
        /* please use MMIRES_GetRingInfo(....) instead of MMI_GetRingInfo() */
        //SCI_PASSERT(0, ("please use MMIRES_GetRingInfo(....) instead of MMI_GetRingInfo()"));/*lint !e527 */ /*assert verified*/
        
        return FALSE;/*lint !e527 */
    }
}

/*****************************************************************************/
//  Description : to get the data by label
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_GetDataInfo(MMI_DATA_ID_T label, MMIRES_DATA_INFO_T* dataInfo)
{
    if( MMI_SYS_RES_FLAG == ( label & MMI_SYS_RES_FLAG ) )
    {
        return MMIRES_GetDataInfo( label, MMK_GetFirstAppletHandle(), dataInfo);
    }
    else
    {
        /* please use MMIRES_GetRingInfo(....) instead of MMI_GetRingInfo() */
        //SCI_PASSERT(0, ("please use MMIRES_GetDataInfo(....) instead of MMI_GetDataInfo()"));/*lint !e527 */ /*assert verified*/
        
        return FALSE;/*lint !e527 */
    }
}

/*****************************************************************************/
//  Description : register module nv len and max nv item 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegModuleNv(uint16 module_id, const uint16 nv_len[], uint16 nv_max_item)
{
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id && !s_is_nv_reg_done); /*assert verified*/

    s_mmi_nv_len[module_id] = (const uint16 *)nv_len;
    s_mmi_nv_max_item[module_id] = nv_max_item;
   // s_mmi_nv_num += nv_max_item ;   //count mmi nv item num
}

/*****************************************************************************/
//  Description : get module max nv item num 
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL uint16 GetNVMAXItem(uint16 module_id)
{
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/

    return s_mmi_nv_max_item[module_id];
}

/*****************************************************************************/
//  Description : Read NV info by id and size  
//  Global resource dependence : none
//  Author: bin.wang1
//  Note: module which not resgister item size use this API
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMI_ReadNVItemEx( uint32 item_id, uint16 item_length, void* buffer_ptr )
{
    uint16 module_id = 0;
    uint16 nv_offset = 0;
    uint16 true_item_id = MMI_UINV_USER_BASE;
    uint16 i = 0;
    NVITEM_ERROR_E reCode = NVERR_NONE;

    SCI_ASSERT(s_is_nv_reg_done); /*assert verified*/
    SCI_ASSERT(buffer_ptr != PNULL); /*assert verified*/

    module_id = MMICOM_GET_WORD_HB(item_id);
    nv_offset = MMICOM_GET_WORD_LB(item_id);

    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(GetNVMAXItem(module_id) > nv_offset); /*assert verified*/

    if (0 != module_id) /*NV base is MMI_UINV_USER_BASE*/
    {
        for (i = 1; i <= module_id; i++)
        {
            true_item_id = true_item_id + GetNVMAXItem(i - 1); /*calculate module NV base*/
        }
    }

    true_item_id = true_item_id + nv_offset;    /*calculate true NV id*/

    SCI_ASSERT((true_item_id <= MMI_UINV_MAX_ID) && (true_item_id >= MMI_UINV_USER_BASE)); /*MAX NV item*/ /*assert verified*/
    SCI_ASSERT(item_length != 0); /*assert verified*/

    reCode = EFS_NvitemRead( true_item_id, item_length, buffer_ptr );

    if ( NVERR_NONE == reCode )
    {
        return MN_RETURN_SUCCESS;
    }

    return MN_RETURN_FAILURE;
}

/*****************************************************************************/
//  Description : Read NV info by id  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMI_ReadNVItem(uint32 item_id, void * buffer_ptr)
{
    uint16 module_id = 0;
    uint16 nv_offset = 0;

    SCI_ASSERT(s_is_nv_reg_done); /*assert verified*/

    module_id = MMICOM_GET_WORD_HB(item_id);
    nv_offset = MMICOM_GET_WORD_LB(item_id);

    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(GetNVMAXItem(module_id) > nv_offset); /*assert verified*/

    return MMI_ReadNVItemEx( item_id, s_mmi_nv_len[module_id][nv_offset], buffer_ptr );
}

/*****************************************************************************/
//  Description : write NV info by id and size 
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMI_WriteNVItemEx( uint32 item_id, uint16 item_length, void* buffer_ptr )
{
    uint16 module_id = 0;
    uint16 nv_offset = 0;
    uint16 true_item_id = MMI_UINV_USER_BASE;
    uint16 i = 0;
    NVITEM_ERROR_E reCode = NVERR_NONE;

    SCI_ASSERT(s_is_nv_reg_done); /*assert verified*/
    SCI_ASSERT(buffer_ptr != PNULL); /*assert verified*/
    module_id = MMICOM_GET_WORD_HB(item_id);
    nv_offset = MMICOM_GET_WORD_LB(item_id);

    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(GetNVMAXItem(module_id) > nv_offset);  /*assert verified*/

    if (0 != module_id)/*NV base is MMI_UINV_USER_BASE*/
    {
        for (i = 1; i <= module_id; i++)
        {
            true_item_id = true_item_id + GetNVMAXItem(i - 1); /*calculate module NV base*/
        }
    }

    true_item_id = true_item_id + nv_offset; /*calculate module NV base*/

    SCI_ASSERT((true_item_id <= MMI_UINV_MAX_ID) && (true_item_id >= MMI_UINV_USER_BASE)); /*MAX NV item*/ /*assert verified*/
    SCI_ASSERT(item_length != 0); /*assert verified*/

    reCode = EFS_NvitemWrite( true_item_id, item_length, buffer_ptr, FALSE );
}

/*****************************************************************************/
//  Description : write NV info by id  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_WriteNVItem(uint32 item_id, void * buffer_ptr)
{
    uint16 module_id = 0;
    uint16 nv_offset = 0;

    SCI_ASSERT(s_is_nv_reg_done); /*assert verified*/

    module_id = MMICOM_GET_WORD_HB(item_id);
    nv_offset = MMICOM_GET_WORD_LB(item_id);

    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(GetNVMAXItem(module_id) > nv_offset);  /*assert verified*/

    MMI_WriteNVItemEx( item_id, s_mmi_nv_len[module_id][nv_offset], buffer_ptr );
}

/*****************************************************************************/
//  Description : get true NV item by item id in module 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMI_GetTrueNVItemId(                      //RETURN: 
                                  uint32    item_id     //IN: 
                                  )
{
    uint16 true_item_id = MMI_UINV_USER_BASE;
    uint16 module_id    = 0;
    uint16 nv_offset    = 0;
    uint16 i            = 0;

    module_id = MMICOM_GET_WORD_HB(item_id);
    nv_offset = MMICOM_GET_WORD_LB(item_id);

    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id && s_is_nv_reg_done); /*assert verified*/
    SCI_ASSERT(GetNVMAXItem(module_id) > nv_offset); /*assert verified*/

    for (i=0; i<module_id; i++)
    {
        true_item_id = true_item_id + GetNVMAXItem(i); /*calculate module NV base*/
    }

    true_item_id = true_item_id + nv_offset;    /*calculate true NV id*/

    return (true_item_id);
}

/*****************************************************************************/
//  Description : set reg modle nv start flag
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_RegModuleNvStart (void)
{
    s_is_nv_reg_done = FALSE;

    return TRUE;
}

/*****************************************************************************/
//  Description : set reg modle nv end flag
//  Global resource dependence : none
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMI_RegModuleNvEnd (void)
{
    s_is_nv_reg_done = TRUE;
    return TRUE;
}

/*****************************************************************************/
//  Description : free resource
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMI_FreeRes(uint32 win_id)
{   
    win_id = MMK_ConvertIdToHandle(win_id);
    MMIRES_FreeNodeResByHandle(win_id);
}


/*****************************************************************************/
//  Description : free all resource
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMI_FreeAllRes(void)
{
    MMIRES_FreeAllNodeRes();
}

#ifdef MMI_WINIDNAME_SUPPORT
/*****************************************************************************/
//  Description : check whether all module autotest win_id_name is registered
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMI_CheckAutoTestWinId(void)
{
    uint32 i = 0;
    
    //SCI_TRACE_LOW:"mmi_modu_main.c enter MMI_CheckAutoTestWinId()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_595_112_2_18_2_0_11_220,(uint8*)"");
    
    for (i = 0; i < MMI_MAX_MODU_NUM; i++)
    {
        if (PNULL == s_win_id_name_ptr[i])
        {
            //SCI_TRACE_LOW:"MMI_CheckAutoTestWinId module%d win_id_name is NULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_601_112_2_18_2_0_11_221,(uint8*)"d", i);
        }
    }
}
#endif

/*****************************************************************************/
//  Description :  get nv item num of all mmi mudule 
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		
/*****************************************************************************/
PUBLIC uint32 MMI_GetAllMMINVItem(void)
{   
    uint32  mmi_nv_num  = 0;
    uint32  module_id = 0 ;
	for(module_id = 0; module_id < MMI_MAX_MODU_NUM; module_id ++)
    {
        /* get sum nv item */
        mmi_nv_num +=   GetNVMAXItem(module_id); 
	}

    mmi_nv_num += MMISRV_GetAllMMINVItem();

	return mmi_nv_num ;
}
/*****************************************************************************/
//  Description :  get nv size of all module, including ps ref efs,etc
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetAllNVSize(void)
{
	const uint32 PSREF_NV_LENGTN  =  30 ; 	//30*1024 bytes
	const uint32  FIXED_NV_LENGTN  = 16  ;	//16*1024 bytes
	const uint32  EFS_NV_LENGTN  =	  20 ; 		//20*1024 bytes
	 return PSREF_NV_LENGTN + FIXED_NV_LENGTN + EFS_NV_LENGTN +  MMI_GetAllModuleSumNvSize() ;
}

/*****************************************************************************/
//  Description :  calulator minimun nv config 
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
#if 0
PUBLIC uint32 MMI_GetFlashSecterSize(void)
{
	uint32 nMID = 0 ;
     uint32 nDevID = 0 ; 
     uint32 nExtID = 0;    
	 
    NORMCP_ENTITY_PTR   normcp_entity_ptr = PNULL; 
    
    FLASH_GetID(&nMID, &nDevID, &nExtID);

    //SCI_TRACE_LOW:"MMI_MODU_MAIN:TV FLASH MID[0x%x] DID[0x%x] EID[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_648_112_2_18_2_0_11_222,(uint8*)"ddd",nMID,nDevID,nExtID);
    
    normcp_entity_ptr = NORMCP_GetEntity(nMID, nDevID, nExtID, NULL);

    SCI_ASSERT(normcp_entity_ptr);	 /*assert verified*/

	 return  normcp_entity_ptr->nor_flash_cfg_ptr->sect_size >> 10;
}

/*****************************************************************************/
//  Description :  GetConfigurableFlashSecterNum
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetConfigurableFlashSecterNum(void)
{
	uint32 nMID = 0 ;
     uint32 nDevID = 0 ; 
     uint32 nExtID = 0;    
	 
    NORMCP_ENTITY_PTR   normcp_entity_ptr = PNULL; 
    
    FLASH_GetID(&nMID, &nDevID, &nExtID);

    //SCI_TRACE_LOW:"MMI_MODU_MAIN:TV FLASH MID[0x%x] DID[0x%x] EID[0x%x]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_671_112_2_18_2_0_11_223,(uint8*)"ddd",nMID,nDevID,nExtID);
    
    normcp_entity_ptr = NORMCP_GetEntity(nMID, nDevID, nExtID, NULL);

    SCI_ASSERT(normcp_entity_ptr);	 /*assert verified*/

  return  normcp_entity_ptr->nor_flash_cfg_ptr->sect_num - normcp_entity_ptr->nor_flash_cfg_ptr->file_sect_num ;
	  
}

/*****************************************************************************/
//  Description :  calulator minimun nv config 
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetMinimumNVConfig(void)
{	
   	 return  (uint32)(MMI_GetAllNVSize()/MMI_GetFlashSecterSize()) + 1 + 1;
}

/*****************************************************************************/
//  Description :  calulator optium  nv config 
//  Global resource dependence : none
//  Author: 		ying.xu
//  Note:		return value counted in KB
/*****************************************************************************/
PUBLIC uint32 MMI_GetOptimumNVConfig(void)
{  
	 return  (uint32)(MMI_GetAllNVSize()/MMI_GetFlashSecterSize()) + 1 + 2;
}

#endif

#ifdef MMI_SAVE_RUNING_NV_INFO
typedef struct 
{
    uint32      mod_id;
    uint8*      mod_name;
}BLOCK_MEM_STR_T;

#define MACRO_RES_PRJ_TYPE MACRO_RES_PRJ_TEST_MOD
#include "macro_res_prj_def.h"

BLOCK_MEM_STR_T nv_table_str[] =
{
    #include "mmi_res_prj_def.h"
};

#undef MACRO_RES_PRJ_TYPE
#endif

/*****************************************************************************/
//     Description : get sum size of all modules nv size
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMI_GetAllModuleSumNvSize(void)
{
    uint32 all_module_sum_size = 0;
    uint32 cur_module_sum_size = 0;
    uint32 all_nv_item_num = 0;
    uint16 module_id = 0;
    uint16 i = 0;
#ifdef MMI_SAVE_RUNING_NV_INFO
    uint16 name_index = 0;   
    uint32 bytes_written = 0;
    wchar file_name[] = L"E:\\RUNNING_NV_INFO.txt";
    uint8       mod_name[10] = {0};
    MMIFILE_HANDLE file_handle = 0; 
    extern int8 *itoa( int32 v, int8* str, uint32 r); 
#endif
#ifdef MMI_SAVE_RUNING_NV_INFO
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_CREATE_ALWAYS, 0, 0);
#endif
    
    for(module_id = 0; module_id < MMI_MAX_MODU_NUM; module_id ++)
    {
        /* get sum nv item */
        all_nv_item_num = all_nv_item_num + GetNVMAXItem(module_id); 

        /* get sum nv size */
        cur_module_sum_size = 0;
        if(MMI_MODULE_SMS == module_id)
        {
            /* sum sms module nv size */
            cur_module_sum_size = MMIAPISMS_GetSumNvSize();
            //SCI_TRACE_LOW:"[MMINV]:MMI_MODULE_SMS total size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_774_112_2_18_2_0_11_224,(uint8*)"d", cur_module_sum_size);
        }
        else if(MMI_MODULE_PB == module_id)
        {
            /* sum pb module nv size */    
            cur_module_sum_size = MMIAPIPB_GetSumNvSize();
            //SCI_TRACE_LOW:"[MMINV]:MMI_MODULE_PB total size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_780_112_2_18_2_0_11_225,(uint8*)"d", cur_module_sum_size);
        }
#ifdef KURO_SUPPORT
        else if(MMI_MODULE_KURO == module_id)
        {
            /* sum kuro module nv size */    
            cur_module_sum_size = MMIAPIKUR_GetSumNvSize();
            //SCI_TRACE_LOW:"[MMINV]:MMI_MODULE_KURO total size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_787_112_2_18_2_0_11_226,(uint8*)"d", cur_module_sum_size);
        }
#endif
        else if(MMI_MODULE_FM == module_id)
        {
            cur_module_sum_size = MMIAPIFM_GetSumNvSize();
            //SCI_TRACE_LOW:"[MMINV]:MMI_MODULE_FM total size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_793_112_2_18_2_0_11_227,(uint8*)"d", cur_module_sum_size);
        }
        else if(MMI_MODULE_CC == module_id)
        {
            cur_module_sum_size = MMIAPICC_GetNvSize();
        }
        else
        {    
            //SCI_TRACE_LOW("[MMINV]:%s ", nv_table_str[module_id].nv_string);
            //SCI_TRACE_LOW:"[MMINV]:%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_798_112_2_18_2_0_11_228,(uint8*)"d", module_id);
            for(i = 0; i < GetNVMAXItem(module_id); i ++)
            {
                /* sum every item nv size */
                cur_module_sum_size += s_mmi_nv_len[module_id][i];
                //SCI_TRACE_LOW:"[MMINV]:NV_ID=%d size=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_803_112_2_18_2_0_11_229,(uint8*)"dd", i,s_mmi_nv_len[module_id][i]);
            }
           // SCI_TRACE_LOW("[MMINV]:%s total size=%d", nv_table_str[module_id].nv_string,cur_module_sum_size);
            //SCI_TRACE_LOW:"[MMINV]:%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_806_112_2_18_2_0_11_230,(uint8*)"d", module_id);
        }
#ifdef MMI_SAVE_RUNING_NV_INFO
       if(file_handle != SFS_INVALID_HANDLE)
       {                          
             name_index = 0xFFFF;
             //mod name
             for(i =0; i < MMI_MAX_MODU_NUM;i++)
             {
                 if(nv_table_str[i].mod_id == module_id)
                 {
                      name_index = i;
                      break;
                 }                 
             }
             if(name_index != 0xFFFF)
             {
                 MMIAPIFMM_WriteFile(file_handle, nv_table_str[name_index].mod_name, strlen(nv_table_str[name_index].mod_name), &bytes_written,0);
                 
                 MMIAPIFMM_WriteFile(file_handle, "  ", 2, &bytes_written,0);
                 //mode size
                 
                 SCI_MEMSET(mod_name, 0x00, sizeof(mod_name));
                 itoa(cur_module_sum_size , mod_name,10);/*lint !e64*/
                 MMIAPIFMM_WriteFile(file_handle, mod_name, sizeof(mod_name), &bytes_written,0);     
                 MMIAPIFMM_WriteFile(file_handle, "\r\n", 2, &bytes_written,0);
             }
       }
#endif
        /* sum current module nv sum size */
        all_module_sum_size += cur_module_sum_size;
    }
    //SCI_TRACE_LOW:"[MMINV]: total module nv size = %d, all nv item num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_MODU_MAIN_811_112_2_18_2_0_11_231,(uint8*)"dd", all_module_sum_size, all_nv_item_num);
#ifdef MMI_SAVE_RUNING_NV_INFO
    if(file_handle != SFS_INVALID_HANDLE)
    {
        //total nv size
        MMIAPIFMM_WriteFile(file_handle, "TOTAL NV SIZE:  ", strlen("TOTAL NV SIZE:  "), &bytes_written,0);
        SCI_MEMSET(mod_name, 0x00, sizeof(mod_name));
        itoa(all_module_sum_size , mod_name,10);/*lint !e64*/
        MMIAPIFMM_WriteFile(file_handle, mod_name, sizeof(mod_name), &bytes_written,0);     
        MMIAPIFMM_WriteFile(file_handle, "\r\n", 2, &bytes_written,0);

        MMIAPIFMM_CloseFile(file_handle);
    }
#endif

    all_module_sum_size += MMISRV_GetAllModuleSumNvSize();

    return all_module_sum_size >> 10;
    
}


/*****************************************************************************/
//  Description : register the applet info
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegAppletInfo(
                             uint16                          module_id, 
                             const CAF_STATIC_APPLET_INFO_T* applet_info_ptr
                             )
{
    SCI_ASSERT(MMI_MAX_MODU_NUM > module_id); /*assert verified*/

    if ( PNULL == applet_info_ptr )
    {
        SCI_TRACE_HIGH("mmi_modu_main.c MMI_RegAppletInfo: PNULL pointer!");/*assert verified*/
    }

    s_static_applet_array[module_id] = (const CAF_STATIC_APPLET_INFO_T*)applet_info_ptr;
}

/*****************************************************************************/
//  Description : get the applet info
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC CAF_STATIC_APPLET_INFO_T** MMI_GetAppletInfo(
                                                    uint32* num_ptr
                                                    )
{
    SCI_ASSERT( PNULL != num_ptr ); /*assert verified*/

    *num_ptr = MMI_MAX_MODU_NUM;

    return (CAF_STATIC_APPLET_INFO_T**)s_static_applet_array;
}

#ifdef WIN32
// declaration for WIN32 simulator 
//  Author: 		ying.xu
NORMCP_ENTITY_PTR NORMCP_GetEntity(uint32 manu_id, uint32 devc_id, uint32 extend_id, uint32 *index)
{
    static NORMCP_ENTITY_T normcp = {0} ;
    static NOR_FLASH_CONFIG_T norflash = {0}  ;
    normcp.nor_flash_cfg_ptr = &norflash;
    normcp.nor_flash_cfg_ptr->sect_size = 64 << 10;
   normcp.nor_flash_cfg_ptr->sect_num =  1024 ;
   normcp.nor_flash_cfg_ptr->file_sect_num = 512 ;
    return &normcp ;
}
void FLASH_GetID(uint32 *pManu_id, uint32 *pDev_id, uint32 *pExtend_id)
{
    pManu_id = 1;
    pDev_id = 2;
    pExtend_id = 3;
}
#endif

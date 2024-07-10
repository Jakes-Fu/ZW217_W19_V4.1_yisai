/*****************************************************************************
** File Name:      mmipb_atc.h                                               *
** Author:         Tracy Zhang                                               *
** Date:           15/11/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi phonebook files that    *
**                 sync witch pc.                                            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2004        Tracy Zhang      Create
******************************************************************************/

#ifndef _MMIPB_ATC_H_
#define _MMIPB_ATC_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_atc.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
 
#ifdef _MMIPB_ATC_C_
#define EXT_PB_ATC
#else
#define EXT_PB_ATC extern
#endif    

#ifdef MMI_SYNCTOOL_V3

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncHeadInfo( 
							    GROUP_HEAD_INFO_T *  group_head_info_ptr  // OUT
							    );



/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncDelete( 
							uint32   group_id  // IN
							);

/*****************************************************************************/
// 	Description : 提供电话簿信息
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
EXT_PB_ATC BOOLEAN MMIPB_SyncHeadInfo
(
    PB_HEAD_INFO_T  *pb_head_info_ptr,  //OUT:
    uint8	 	    pos           //IN:
);


/*****************************************************************************/
// 	Description : 读电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMIPB_SyncRead
(
    PB_PHONEBOOK_ENTRY_T     *pb_item_ptr,  //OUT:
    uint8	 	    pos           //IN:
);


/*****************************************************************************/
// 	Description : 添加电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncAdd
(
    uint8	 	    pos,           //IN:
    PB_PHONEBOOK_ENTRY_T    *pb_item_ptr,   //IN:
    uint16* contact_id_ptr
);


/*****************************************************************************/
// 	Description : 删除电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncDelete
(
    uint8	 	pos,        //IN:
    uint16      entry_id    //IN:
);


/*****************************************************************************/
// 	Description : 修改电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncModify 
(
    uint8	 	pos,        //IN:
    PB_PHONEBOOK_ENTRY_T    *pb_item_ptr   //IN: 记录
);
/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncRead( 
                            GROUP_ENTRY_T *  group_info_ptr  // IN & OUT
							);

/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:      liqing.peng
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncModify( 
                            uint16           group_id,    //IN:
							GROUP_ENTRY_T *  group_info_ptr  // IN 
							);
/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:   xiaohua.liu
//	Note:
/*****************************************************************************/
BOOLEAN MMIPBGROUP_SyncAdd( 
							GROUP_ENTRY_T *group_info_ptr,  // IN
							uint32* id_out_ptr
							);
/*****************************************************************************/
// 	Description : This function is provided by phonebook, used by MMI AT
//	Global resource dependence : none
//  Author:   xiaohua.liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_GetPbGroupIdFromStr(PB_PHONEBOOK_ENTRY_T *pb_entry_ptr,char* begin_pos,char* end_pos);
#endif


#ifdef MMI_SYNCTOOL_SUPPORT
/*****************************************************************************/
// 	Description : 提供电话簿信息
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
EXT_PB_ATC BOOLEAN MMIPB_SyncHeadInfo
(
    PB_HEAD_INFO_T  *pb_head_info_ptr,  //OUT:
    uint8	 	    pos           //IN:
);


/*****************************************************************************/
// 	Description : 读电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
BOOLEAN MMIPB_SyncRead
(
    PB_PHONEBOOK_ENTRY_T     *pb_item_info_ptr,  //OUT:
    uint8	 	    pos           //IN:
);


/*****************************************************************************/
// 	Description : 添加电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncAdd
(
    uint8	 	    pos,           //IN:
    PB_PHONEBOOK_ENTRY_T    *pb_item_info_ptr   //IN:
);


/*****************************************************************************/
// 	Description : 删除电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncDelete
(
    uint8	 	pos,        //IN:
    uint16      entry_id    //IN:
);


/*****************************************************************************/
// 	Description : 修改电话簿记录
//	Global resource dependence : none
//  Author:       Tracy Zhang
//	Note:
/*****************************************************************************/
MMIPB_ERROR_E MMIPB_SyncModify 
(
    uint8	 	pos,        //IN:
    PB_PHONEBOOK_ENTRY_T    *pb_item_info_ptr   //IN: 记录
);

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

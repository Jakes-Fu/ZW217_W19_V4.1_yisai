/*************************************************************************
 ** File Name:      mmi_id.c                                          *
 ** Author:         haiyang.hu                                           *
 ** Date:           2006/09/18                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/18     haiyang.hu       Create.                              *
*************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmi_module.h"
#include "mmi_modu_main.h"
#include "mmi_id.h"
#include "mmk_type.h"

#define WIN_ID_DEF(win_id)          #win_id

const uint8 mmi_id_name_arr[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include "mmi_id.def"    
};

#undef WIN_ID_DEF

/*****************************************************************************/
// 	Description : Register acc menu group
//	Global resource dependence : none
//  Author: haiyang.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMICOMMON_RegWinIdNameArr(void)
{
    //mmi applet��guid��win id\ctrl id�����ظ�
    //����ķ�Χ��win id\ctrl idҲ�����ظ�
    //�������id�Ҿ����������
    SCI_ASSERT( TOTAL_CONTROL_ID_NUMBER <= SPRD_MMI_GUID_START  /*assert to do*/
        && MMI_MODULE_MAX_NUM <= MMK_MODULE_MAX_NUM );

    MMI_RegWinIdNameArr(MMI_MODULE_COMMON, (const uint8**)mmi_id_name_arr);
//    MMI_RegWinIdNameArr(uint16 module_id, const uint8** win_id_name_ptr);
}



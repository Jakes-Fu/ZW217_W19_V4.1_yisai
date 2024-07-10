/******************************************************************************
文件名称:mmipim_nv.c
文件描述:
文件说明:
开发人员:王晓林
首次开发日期:2007-9.25
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2007-09-25            王晓林                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/


#define _MMIPIM_NV_C_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
//#include "sci_types.h"
#include "mmipim_nv.h"
#include "mmi_modu_main.h"
 #include "mmipim_pb.h"
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.4
#include "Vcalendar_parse.h"
#endif /* HS_PIM_VCAL */
//#include "mmipim_wintab.h"

/*the length of PIM nv*/
uint16 pim_nv_len[] =
{
  	sizeof(auth_type),
	sizeof(SmlEncoding_t),
	MMIPIM_ANCHOR_LEN,
	sizeof(net_setting_info_type), //chenxiang 20071101 pim_setting
	sizeof(proxy_setting_info_type),
	sizeof(server_setting_info_type),
	sizeof(wap_url_info_type), //chenxiang 20071101 pim_setting
    MMIPIM_MAX_PHONEBOOK_RECORDS,
#ifdef HS_PIM_UA_SETTING
       sizeof(uint32),
       sizeof(MMIPIM_UA_INFO_T),
#endif
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16
       sizeof(sync_type),
#endif
#ifdef HS_PIM_VCAL  //Added by wangzhefeng  2009.9.4
        MMIPIM_MAX_VCAL_NUM,
	 MMIPIM_ANCHOR_LEN,//added by wangzhefeng 20091208 for 日程更新的时间戳
#endif /* HS_PIM_VCAL */
       MMIPIM_ANCHOR_LEN
};

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: 王晓林
//	Note:
/*****************************************************************************/
PUBLIC void MMIPIM_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_PIM, pim_nv_len, sizeof(pim_nv_len) / sizeof(uint16));
}

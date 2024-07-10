/*****************************************************************************
** File Name:      mmipim_pb.h                                                   *
** Author:           陈翔                                                        *
** Date:           26/10/2007                                                *
** Copyright:                                                                    *
** Description:                                         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2007        chenxiang      Create
******************************************************************************/

#ifndef _MMIPIM_PB_H_
#define _MMIPIM_PB_H_ 

#include "mmipim.h"
#include "pimsmldtd.h"
#include "mmipb_nv.h"


#define MMIPIM_PB_NAME_LEN  40  //15个汉字，30个英文(为适应服务器改为20个汉字，20个字符)
#define MMIPIM_PB_TEL_LEN   40    //40个字符,合法电话号码字符 // chenxiang 20091030  30-->40
#define MMIPIM_PB_ADR_POSE_BOX_LEN  30  //至少30个字符，包括中英文
#define MMIPIM_PB_ADR_DETAIL_LEN  50  //至少50个字符，包括中英文
#define MMIPIM_PB_ADR_ROAD_LEN  MMIPB_MAX_WORKADR_LEN  //至少50个字符，包括中英文
#define MMIPIM_PB_ADR_CITY_LEN  25  //至少25个字符，包括中英文
#define MMIPIM_PB_ADR_PROVINCE_LEN  25  //至少25个字符，包括中英文
#define MMIPIM_PB_ADR_POSE_CODE_LEN  10  //至少10个字符.数字
#define MMIPIM_PB_ADR_COUNTRY_LEN  25  //至少25个字符，包括中英文
#define MMIPIM_PB_URL_LEN  100  //100个字符,包括中英文
#define MMIPIM_PB_ORG_LEN  MMIPB_MAX_ORG_LEN //30个字符,包括中英文
#define MMIPIM_PB_TITLE_LEN  MMIPB_MAX_TITLE_LEN  //30个字符,包括中英文
#define MMIPIM_PB_EMAIL_LEN  MMIPB_MAX_MAIL_LEN  //50个字符,包括中英文

#define MMIPIM_PB_NOTE_LEN  40  //50个字符,包括中英文

#define MMIPIM_MAX_PHONEBOOK_RECORDS  MMINV_MAX_PHONEBOOK_RECORDS



typedef struct _MMIPIM_PB_LOG_INF
{
    BOOLEAN is_valid;
    uint8 * pb_status_list_ptr;
} MMIPIM_PB_LOG_INF;


/******************************************************************* 
FUNCTION 函数名:MMIPIM_PB_Add
功能描述：添加电话簿记录
** 输  出: 
** 作  者:陈翔
** 日  期:2007-10-26
** 版本: ver 1.0
**注意: chenxiang 20071026 
****************************************************************/
PUBLIC short MMIPIM_PB_Add(short id, void* userData, SmlAddPtr_t pContent);

/******************************************************************* 
FUNCTION 函数名:MMIPIM_PB_replace
功能描述：替换电话簿记录
** 输  出: 
** 作  者:陈翔
** 日  期:2007-10-26
** 版本: ver 1.0
**注意: chenxiang 20071026  
****************************************************************/
PUBLIC short MMIPIM_PB_replace(short id, void* userData, SmlReplacePtr_t pContent);

/******************************************************************* 
FUNCTION 函数名:MMIPIM_PB_delete
功能描述：删除电话簿中的记录
** 输  出: 
** 作  者:陈翔
** 日  期:2007-10-26
** 版本: ver 1.0
**注意:  chenxiang 20071026 
****************************************************************/
PUBLIC short MMIPIM_PB_delete(short id, void* userData, SmlDeletePtr_t pContent);


/******************************************************************* 
FUNCTION 函数名:MMIPIM_GetPBVcardItem
功能描述：将电话簿中的一个记录转成VCARD存放在buffer中
** 输  出: 
** 作  者:陈翔
** 日  期:2007-10-26
** 版本: ver 1.0
**注意:
****************************************************************/
PUBLIC int MMIPIM_GetPBVcardItem
(
uint16 entry_id , //IN: 电话簿记录号
char* buffer,      // OUT: 输出的VCARD
uint32 buffersize //IN: 缓冲区大小
);

/*==========================================================
 * author		: wangxiaolin      
 * function 	: MMIPB_GetRecentPBItemsCount
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/9/2009 
 ==========================================================*/
uint16 MMIPB_GetRecentPBItemsCount(PB_PIM_STATUS_E pim_status);

/*==========================================================
 * author		: wangxiaolin      
 * function 	: MMIPB_GetPimStatus
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/9/2009 
 ==========================================================*/
PB_PIM_STATUS_E MMIPB_GetPimStatus(uint16 entry_id);

/*==========================================================
 * author		: wangxiaolin      
 * function 	: MMIPB_UpdatePimStatus
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/9/2009 
 ==========================================================*/
 BOOLEAN MMIPB_UpdatePimStatus   //RETURN: TRUE,操作成功；FALSE,操作失败
(
	uint16				entry_id,	//IN: 记录号
	PB_PIM_STATUS_E		pim_status	//IN: PIM状态
);

/*==========================================================
 * author		: wangxiaolin      
 * function 	: MMIPB_UpdateAllPimStatus
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/9/2009 
 ==========================================================*/
 void MMIPB_UpdateAllPimStatus(PB_PIM_STATUS_E pim_status );


unsigned int MMIPB_GetAllPBItemsCount(void);

unsigned int MMIPB_GetFreeItemsCount(void);
#endif

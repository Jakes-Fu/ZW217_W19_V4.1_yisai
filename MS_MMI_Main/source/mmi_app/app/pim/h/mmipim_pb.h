/*****************************************************************************
** File Name:      mmipim_pb.h                                                   *
** Author:           ����                                                        *
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


#define MMIPIM_PB_NAME_LEN  40  //15�����֣�30��Ӣ��(Ϊ��Ӧ��������Ϊ20�����֣�20���ַ�)
#define MMIPIM_PB_TEL_LEN   40    //40���ַ�,�Ϸ��绰�����ַ� // chenxiang 20091030  30-->40
#define MMIPIM_PB_ADR_POSE_BOX_LEN  30  //����30���ַ���������Ӣ��
#define MMIPIM_PB_ADR_DETAIL_LEN  50  //����50���ַ���������Ӣ��
#define MMIPIM_PB_ADR_ROAD_LEN  MMIPB_MAX_WORKADR_LEN  //����50���ַ���������Ӣ��
#define MMIPIM_PB_ADR_CITY_LEN  25  //����25���ַ���������Ӣ��
#define MMIPIM_PB_ADR_PROVINCE_LEN  25  //����25���ַ���������Ӣ��
#define MMIPIM_PB_ADR_POSE_CODE_LEN  10  //����10���ַ�.����
#define MMIPIM_PB_ADR_COUNTRY_LEN  25  //����25���ַ���������Ӣ��
#define MMIPIM_PB_URL_LEN  100  //100���ַ�,������Ӣ��
#define MMIPIM_PB_ORG_LEN  MMIPB_MAX_ORG_LEN //30���ַ�,������Ӣ��
#define MMIPIM_PB_TITLE_LEN  MMIPB_MAX_TITLE_LEN  //30���ַ�,������Ӣ��
#define MMIPIM_PB_EMAIL_LEN  MMIPB_MAX_MAIL_LEN  //50���ַ�,������Ӣ��

#define MMIPIM_PB_NOTE_LEN  40  //50���ַ�,������Ӣ��

#define MMIPIM_MAX_PHONEBOOK_RECORDS  MMINV_MAX_PHONEBOOK_RECORDS



typedef struct _MMIPIM_PB_LOG_INF
{
    BOOLEAN is_valid;
    uint8 * pb_status_list_ptr;
} MMIPIM_PB_LOG_INF;


/******************************************************************* 
FUNCTION ������:MMIPIM_PB_Add
������������ӵ绰����¼
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��: chenxiang 20071026 
****************************************************************/
PUBLIC short MMIPIM_PB_Add(short id, void* userData, SmlAddPtr_t pContent);

/******************************************************************* 
FUNCTION ������:MMIPIM_PB_replace
�����������滻�绰����¼
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��: chenxiang 20071026  
****************************************************************/
PUBLIC short MMIPIM_PB_replace(short id, void* userData, SmlReplacePtr_t pContent);

/******************************************************************* 
FUNCTION ������:MMIPIM_PB_delete
����������ɾ���绰���еļ�¼
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��:  chenxiang 20071026 
****************************************************************/
PUBLIC short MMIPIM_PB_delete(short id, void* userData, SmlDeletePtr_t pContent);


/******************************************************************* 
FUNCTION ������:MMIPIM_GetPBVcardItem
�������������绰���е�һ����¼ת��VCARD�����buffer��
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��:
****************************************************************/
PUBLIC int MMIPIM_GetPBVcardItem
(
uint16 entry_id , //IN: �绰����¼��
char* buffer,      // OUT: �����VCARD
uint32 buffersize //IN: ��������С
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
 BOOLEAN MMIPB_UpdatePimStatus   //RETURN: TRUE,�����ɹ���FALSE,����ʧ��
(
	uint16				entry_id,	//IN: ��¼��
	PB_PIM_STATUS_E		pim_status	//IN: PIM״̬
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

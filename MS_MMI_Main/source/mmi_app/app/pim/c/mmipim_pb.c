/****************************************************************************
** File Name:      mmipim_pb.c                                          *
** Author:          ����                                                       *
** Date:           26/10/2007                                              *
** Copyright:      
** Description:    PIM ��PB �Ľӿ�ģ��
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 10/2007       chenxiang         Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmi_app_pim_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#define _MMIPIM_PB_C_  



#include "PIMsmlcore.h"
#include "PIMlibmem.h"
#include "PIMsmlerr.h"
#include "PIMdatasynctask.h"
#include "PIMCommhttp.h"

#include"mmipim.h"

//#include "mmipb_mnapi.h"
#include "mmipb_export.h"
#include "VCard.h"
#include "Vcard_parse.h"

#include "mmi_appmsg.h"
#include "mmipim_id.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmipim_pb.h"
#include "mmipim_nv.h"
#ifdef HS_PIM_VCAL 
#include "Vcalendar_parse.h"
#endif /* HS_PIM_VCAL */
#include "mmipim_wintab.h"
#include "mmipb_common.h"

#define MAX_CHAR 512

#define MMIPIM_HANDLE          SFS_HANDLE
#define MMIPIM_ERROR_E         SFS_ERROR_E
typedef  unsigned char     byte; /*lint !e761*/
  
extern task_protocol_step_type* hs_protocol_step;
extern task_protocol_step_type* hs_protocol_step_priv;
extern uint8 hs_g_sessionID;

extern status_cmd_queue_type*  hs_status_cmd_que;

extern SmlNewMapItemListPtr_t  hs_globlemapItemList;
//extern MMIPB_SORT_AND_ENTRY_ID_T   g_mmipb_entry_list;

extern VCARD_TYPE Vcardfile;
extern PIM_T g_pim;
extern BOOLEAN g_is_pim_sync;//�Ƿ�����ͬ��

static uint8 pb_status_arr[MMIPIM_MAX_PHONEBOOK_RECORDS]={0};
static MMIPIM_PB_LOG_INF g_pb_log_inf={FALSE,pb_status_arr};


#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
static int data_cur_len=0;
static int data_max_len=0;
static char* large_obj_buf_ptr=NULL;
static char luri_str[100]={0};
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */

static char wrBuf1[1000];

extern task_relay_info_type*     hs_task_relay_info;

#ifdef HS_PIM_LOG_FILE
uint32 hs_logFileCount = 0;
#endif

#ifdef HS_PIM_CACHE_MAP //  wangxiaolin 20090804
BOOLEAN hs_isSendingCacheMap = FALSE;
 // C:\PimMap.dat
//__align (4)
//static uint8 hs_map_filename[] = {0x00, 0x43, 0x00, 0x3A, 0x00, 0x5C, 0x00, 0x50, 0x00, 0x69, 0x00, 0x6D, 0x00, 0x4D, 0x00, 0x61, 0x00, 0x70, 0x00, 0x2E, 0x00, 0x64, 0x00, 0x61, 0x00, 0x74, 0x00, 0x00};
static uint16 hs_map_filename[] = {0x0043, 0x003A, 0x005C, 0x0050, 0x0069, 0x006D, 0x004D, 0x0061, 0x0070, 0x002E, 0x0064, 0x0061, 0x0074, 0x0000};

#endif

  /*���ݳ���*/
char hs_clientpbookdb[]="./mobile-phonebook";
char hs_clientcalendardb[]="./mobile-calendar";
char hs_syncmlnextnonce[]="nonce";


//extern BOOLEAN  MNNV_GetIMEI(   MN_IMEI_T imei  );
extern void hs_pim_convert_scitime2str(SCI_TM_T current_time , char* str);


/******************************************************************* 
** ������:hs_imeitype2char
** ������������imei 9���ֽڵ�����ת�����ַ���IMEI
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
*******************************************************************/
LOCAL void hs_imeitype2char(char* dest, byte* source);

/*****************************************************************************/
// 	Description : �Ų��ܼ������һ����¼
//	Global resource dependence : none
//  Author:       ������
//	Note:      �ӿں���2007.11.29
/*****************************************************************************/
static BOOLEAN MMIPB_PimAdd
(
  MMIPB_CONTACT_T     *pb_entry_ptr
)
{
    uint8   i        = 0;
    BOOLEAN   num_flag = FALSE;

    //syncml_cb_message:"@@MMIPB_PimAdd, enter! ptr=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_126_112_2_18_2_46_58_20,(uint8*)"d",pb_entry_ptr);

    if(PNULL == pb_entry_ptr)
    {
        return FALSE;
    }

    if (!MMIAPIPB_IsPbReady())
    {
        return (FALSE);
    }
    //syncml_cb_message:"@@MMIPB_PimAdd, step 1! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_137_112_2_18_2_46_58_21,(uint8*)"");
    for (i=0; i<MMIPB_MAX_NV_PHONE_NUM; i++)
    {
        if (MMIAPIPB_IsValidPhoneNumber(pb_entry_ptr->number[i].npi_ton, 
                                        pb_entry_ptr->number[i].number_len)
           )
        {
            num_flag=TRUE;
            break;
        }
    }
#ifdef MMIPB_MAIL_SUPPORT
    if ((!num_flag) && (pb_entry_ptr->mail.wstr_len > 0))
    {
        num_flag=TRUE;
    }
#endif
    //syncml_cb_message:"@@MMIPB_PimAdd! num_flag=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_153_112_2_18_2_46_58_22,(uint8*)"d",num_flag);

    if ((!num_flag) && (0 == pb_entry_ptr->name.wstr_len)) 
    {
        return FALSE;
    }

    if(MMIPB_ERROR_SUCCESS!=MMIAPIPB_AddContact(pb_entry_ptr))
    {
        return FALSE;
    }

    return TRUE;
}


/*****************************************************************************/
// 	Description : �Ų��ܼ���ɾ��һ����¼
//	Global resource dependence : none
//  Author:       ������
//	Note:      �ӿں���2007.11.29
/*****************************************************************************/
static BOOLEAN MMIPB_PimDelete
(
     uint16      entry_id   
)
{
    if(MMIPB_ERROR_SUCCESS==MMIAPIPB_DeleteContact(entry_id,MMIAPIPB_GetStorageId(MN_DUAL_SYS_1, PHONEBOOK_NV_STORAGE)))
    {
        return TRUE;
    }

    return FALSE;
}


/*****************************************************************************/
// 	Description : �Ų��ܼ����޸�һ����¼
//	Global resource dependence : none
//  Author:      ������
//	Note:      �ӿں���2007.11.29
/*****************************************************************************/
static BOOLEAN MMIPB_PimModify 
(
   MMIPB_CONTACT_T     *pb_entry_ptr
)
{
    if(MMIPB_ERROR_SUCCESS==MMIAPIPB_ModifyContact(pb_entry_ptr))
    {
       return TRUE; 
    }
    return FALSE;
}


/*****************************************************************************/
// 	Description : ��ȡһ���绰���Ƿ���δͬ��������״̬
//	Global resource dependence : none
//  Author:  ����
//	Note: chenxiang pb_pim_log
/*****************************************************************************/
PB_PIM_STATUS_E MMIPB_GetPimStatus   //RETURN: PIM״̬
(
	uint16				entry_id		//IN: ��¼��
)
{
    MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
    //SCI_TRACE_MID("mmipim_pb: MMIPB_GetPimStatus1 entry_id = %d,is_valid=%d", entry_id,g_pb_log_inf.is_valid);

    if ((entry_id > MMIPIM_MAX_PHONEBOOK_RECORDS) ||(0 == entry_id))
    {
        return (PB_RECENT_INVALID);
    }

    if(g_pb_log_inf.is_valid)
    {
       //SCI_TRACE_MID("mmipim_pb: MMIPB_GetPimStatus1 is_valid=%d, entry_id = %d,state=%d", g_pb_log_inf.is_valid,entry_id,g_pb_log_inf.pb_status_list_ptr[entry_id -1]);

        return ((PB_PIM_STATUS_E)g_pb_log_inf.pb_status_list_ptr[entry_id -1]);
    }
    else
    {
        nv_result=MMI_ReadNVItem(MMI_PIM_PB_LOG_INFO,g_pb_log_inf.pb_status_list_ptr);
        if(MN_RETURN_SUCCESS==nv_result)
        {
           //SCI_TRACE_LOW:"mmipim_pb: MMIPB_GetPimStatus2 ,read sucess! "
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_232_112_2_18_2_46_58_23,(uint8*)"");

            g_pb_log_inf.is_valid=TRUE;
            return ((PB_PIM_STATUS_E)g_pb_log_inf.pb_status_list_ptr[entry_id -1]);
        }
        else
        {
            //SCI_TRACE_LOW:"mmipim_pb: MMIPB_GetPimStatus3 ,read fail! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_239_112_2_18_2_46_58_24,(uint8*)"");
            return (PB_RECENT_INVALID);
        }
    }
}

PUBLIC void  MMIPIM_InitPbStatus(void)
{
        MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
        nv_result=MMI_ReadNVItem(MMI_PIM_PB_LOG_INFO,g_pb_log_inf.pb_status_list_ptr);
        if(MN_RETURN_SUCCESS==nv_result)
        {
            g_pb_log_inf.is_valid=TRUE;
        }
        else
        {
             uint8 pb_status_arr_init[MMIPIM_MAX_PHONEBOOK_RECORDS]={0};
             MMI_WriteNVItem(MMI_PIM_PB_LOG_INFO,pb_status_arr_init);

        }
 }

/*****************************************************************************/
// 	Description : ����һ���绰����¼û��ͬ������ӣ��޸ģ�ɾ������
//	Global resource dependence : none
//  Author:  ����
//	Note: chenxiang pb_pim_log
/*****************************************************************************/
 BOOLEAN MMIPB_UpdatePimStatus   //RETURN: TRUE,�����ɹ���FALSE,����ʧ��
(
	uint16				entry_id,	//IN: ��¼��
	PB_PIM_STATUS_E		pim_status	//IN: PIM״̬
)
{
       //uint32 item_id;
       //MN_RETURN_RESULT_E  nv_result   = MN_RETURN_FAILURE;
	PB_PIM_STATUS_E old_status=PB_RECENT_INVALID;
       PB_PIM_STATUS_E new_status=PB_RECENT_INVALID;
	//BOOLEAN  result   = FALSE;


	if ((entry_id > MMIPIM_MAX_PHONEBOOK_RECORDS)
	    ||(0 == entry_id))
	{
	    return (FALSE);
	}
	

       old_status = MMIPB_GetPimStatus(entry_id);

	//SCI_TRACE_MID:"mmipim_pb: MMIPB_UpdatePimStatus entry_id = %d,old=%d,new=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_287_112_2_18_2_46_58_25,(uint8*)"ddd", entry_id,old_status,pim_status);

	//�ж���״̬��ֵ
	switch (old_status)
	{
		case PB_NO_MODIFY:
			if (PB_NO_MODIFY == pim_status)
			{
				return (TRUE);
			}
			else if (PB_RECENT_ADD == pim_status || PB_RECENT_REPLACE == pim_status || PB_RECENT_DELETE == pim_status)
			{
				new_status = pim_status;
			}
			else
			{
				return (FALSE);
			}
			break;

		case PB_RECENT_ADD:
			if (PB_NO_MODIFY == pim_status)
			{
				new_status = PB_NO_MODIFY;
			}
			else if (PB_RECENT_ADD == pim_status)
			{
				return (TRUE); // ����������ᷢ��
			}
			else if (PB_RECENT_REPLACE == pim_status)
			{
				return (TRUE);
			}
			else if (PB_RECENT_DELETE == pim_status)
			{
				new_status = PB_NO_MODIFY;
			}
			else
			{
				return (FALSE);
			}
			break;

		case PB_RECENT_REPLACE:
			if (PB_NO_MODIFY == pim_status)
			{
				new_status = PB_NO_MODIFY;
			}
			else if (PB_RECENT_ADD == pim_status)
			{
				return (TRUE); // ����������ᷢ��
			}
			else if (PB_RECENT_REPLACE == pim_status)
			{
				return (TRUE);
			}
			else if (PB_RECENT_DELETE == pim_status)
			{
				new_status = PB_RECENT_DELETE;
			}
			else
			{
				return (FALSE);
			}
			break;
			
		case PB_RECENT_DELETE:
			if (PB_NO_MODIFY == pim_status)
			{
				new_status = PB_NO_MODIFY;
			}
			else if (PB_RECENT_ADD == pim_status)
			{
				new_status = PB_RECENT_REPLACE;
			}
			else if (PB_RECENT_REPLACE == pim_status)
			{
				new_status = PB_RECENT_REPLACE; // ����������ᷢ��
			}
			else if (PB_RECENT_DELETE == pim_status)
			{
				return (TRUE); // ����������ᷢ��
			}
			else
			{
				return (FALSE);
			}
			break;
            case PB_RECENT_INVALID:
                   return FALSE;
			
		default:
		       if (PB_RECENT_ADD == pim_status || PB_RECENT_REPLACE == pim_status || PB_RECENT_DELETE == pim_status||PB_NO_MODIFY == pim_status)
			{
				new_status = pim_status;
			}
			else
			{
				return (FALSE);
			}
			break;

	}

    //��дȫ�ֱ�����дNV
    g_pb_log_inf.pb_status_list_ptr[entry_id -1]=(uint8)new_status;
    //syncml_cb_message(("@@mmipim MMIPB_UpdatePimStatus is_invalid=%d ",g_pb_log_inf.is_valid));
    //SCI_TRACE_LOW:"mmipim_pb: MMIPB_UpdatePimStatus entry_id = %d,old=%d,new=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_394_112_2_18_2_46_58_26,(uint8*)"ddd", entry_id,old_status,pim_status);

    //û�з���ֵ����α�֤д��������ȷ��?
    MMI_WriteNVItem(MMI_PIM_PB_LOG_INFO,g_pb_log_inf.pb_status_list_ptr);
    
    return (TRUE);

}

/*****************************************************************************/
// 	Description : �������м�¼��ͬ��״̬
//	Global resource dependence : none
//  Author:  ����
//	Note: chenxiang pb_pim_log
/*****************************************************************************/
 void MMIPB_UpdateAllPimStatus
(
	PB_PIM_STATUS_E		pim_status	//IN: PIM״̬
        )
{
    uint32 i = 0;
    for (i = 0; i < MMIPIM_MAX_PHONEBOOK_RECORDS; i++)
    {
        /*********************************************************************
        ����ͻ��˺ͷ�����ͬʱɾ�����ٴ���ͬ������Ҫ
        ���״̬�������´������˫��ͬ���������������
        ����һϵ��ɾ���Ĳ���ָ�����ͬ��ʱ�����ԭ״
        ̬��ADD����REPLACE�������Ϊ������ԭ�򣬸���û�г�
        ������ʱ���ܽ�״̬��Ϊ��ͬ����by wangxiaolin ,2010.01.09
        **********************************************************************/
#if 0//del by wangzhefeng  2010.2.25
        /* ��������������ն��½���ͬ�ļ�¼ʱ��ͬ����
        ������¼��״̬ʼ��Ϊ�½�������(�������case1.5)*/
        if(PB_RECENT_DELETE==g_pb_log_inf.pb_status_list_ptr[i])
#endif
        {
             g_pb_log_inf.pb_status_list_ptr[i] = (uint8)pim_status;
        }
    }
    
    //û�з���ֵ����α�֤д��������ȷ��?
    MMI_WriteNVItem(MMI_PIM_PB_LOG_INFO,g_pb_log_inf.pb_status_list_ptr);
}
/*****************************************************************************/
// 	Description : ��ȡ���޸ĵĵ绰����¼����
//	Global resource dependence : none
//  Author:  ����
//	Note: chenxiang pb_pim_log
/*****************************************************************************/
uint16 MMIPB_GetRecentPBItemsCount(PB_PIM_STATUS_E pim_status)
{
	uint16 i, result;
	result = 0;
	for (i = 1; i <= MMIPIM_MAX_PHONEBOOK_RECORDS; i++)
	{
		if (MMIPB_GetPimStatus(i) == pim_status)
			result++;
	}
	return (result);
}

/*****************************************************************************/
// 	Description : ��ȡ�绰����¼����
//	Global resource dependence : none
//  Author:  ������
//	Note: 
/*****************************************************************************/
unsigned int MMIPB_GetAllPBItemsCount(void)
{
	unsigned int  result=0;
    //result= (unsigned int)(g_mmipb_entry_list.nv_entry_id_t.max_record_num[MN_DUAL_SYS_1]-g_mmipb_entry_list.nv_entry_id_t.spare_id[MN_DUAL_SYS_1]);
    result=MMIAPIPB_GetPhoneContactCount();
    //syncml_cb_message:"@@mmipim MMIPB_GetAllPBItemsCount all_pb_item=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_460_112_2_18_2_46_59_27,(uint8*)"d",result);
    return (result);
}

unsigned int MMIPB_GetFreeItemsCount(void)
{
	unsigned int  result=0;
    //result= (unsigned int)(g_mmipb_entry_list.nv_entry_id_t.spare_id[MN_DUAL_SYS_1]);
    result=MMIAPIPB_GetStorageMaxCount(PHONEBOOK_NV_STORAGE)-MMIAPIPB_GetPhoneContactCount();
    //syncml_cb_message:"@@mmipim MMIPB_GetFreeItemsCount free_num=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_469_112_2_18_2_46_59_28,(uint8*)"d",result);
	return (result);
}


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/******************************************************************* 
FUNCTION ������:MMIPIM_VCard_PB
����������VCARD ת�绰����¼
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��: chenxiang 20071026   �ο�Mmimultim_wintab.c	2382	case MULTIM_FILE_SAVE_TO_PB:
****************************************************************/
static BOOLEAN MMIPIM_VCard_PB(char * vCardData,  MMIPB_CONTACT_T * pb_entry)
{
	int result = 0;
     // syncml_cb_message(("@@MMIPIM_VCard_PB vcard content=%s ",vCardData));
	result = VcardParser(vCardData, &Vcardfile, pb_entry);
      //syncml_cb_message(("@@MMIPIM_VCard_PB pb entry=%s ",pb_entry));
	ReleaseVcardMem(&Vcardfile);
	if (1 == result)
		return (TRUE);
	else
		return (FALSE);
}

/******************************************************************* 
FUNCTION ������:MMIPIM_PB_Add
������������ӵ绰����¼
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��: chenxiang 20071026    ÿ��ֻ���һ����¼?  ?
****************************************************************/
PUBLIC short MMIPIM_PB_Add(short id, void* userData, SmlAddPtr_t pContent)
{
	SmlStatusPtr_t  status=NULL;
	SmlSourceRefListPtr_t sourcelist=NULL;
	char* vCardData=NULL;
	char* tempcharptr=NULL;
	SmlNewMapItemListPtr_t maplist=NULL;
	SmlNewMapItemListPtr_t temp=NULL;
	int str_len=0;


#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
   // char * temp_buffer_ptr_for_large_obj=NULL;
    SmlMetInfMetInfPtr_t met_inf_ptr=NULL;
    char * data_max_len_str=NULL;
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */

	uint16 rec_id;
	MMIPB_CONTACT_T entry_add;
       PIM_T_P  pMe=MMIPIM_GetPIM();//add by wangxiaolin
       //�ӿ������⣬��Ҫ�޸�
	rec_id = MMIAPIPB_GetFreeContactId(MMIPB_STORAGE_PHONE); // ��ȡһ��δʹ�õ�id
	//syncml_cb_message:"@@MMIPIM_PB_Add free_id=%d ,time=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_528_112_2_18_2_46_59_29,(uint8*)"dd",rec_id,PIM_GetCurTime());
	status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
       if(NULL==status)
       {
           	return hs_smlFreeProtoElement(pContent); 
       }
	sourcelist=(SmlSourceRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlSourceRefList_t));
       if(NULL==sourcelist)
       {
             hs_smlLibFree(status);
           	return hs_smlFreeProtoElement(pContent); 
       }
       
	hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
	hs_smlLibMemset(sourcelist, 0, (long)sizeof(SmlSourceRefList_t));

	status->elementType=SML_PE_STATUS;
	status->sourceRefList=sourcelist;  

	status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, hs_status_cmd_que->msgref->content);
	//status->cmdRef=pContent->cmdID;
	status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->cmdID->content); // chenxiang_memory 20080223
	status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "Add");
	//status->sourceRefList->sourceRef=pContent->itemList->item->source->locURI;
	status->sourceRefList->sourceRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->itemList->item->source->locURI->content); // chenxiang_memory 20080223
	status->sourceRefList->next=NULL;
    
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
        if((SML_PCDATA_EXTENSION== pContent->meta->contentType)
            &&(SML_EXT_METINF==pContent->meta->extension))
        {
            met_inf_ptr=(SmlMetInfMetInfPtr_t)(pContent->meta->content);
            data_max_len_str=hs_smlPcdata2String(__FILE__, __LINE__,met_inf_ptr->maxobjsize);                 
            data_max_len=hs_smlString2unsignedInt(data_max_len_str);
            hs_smlLibFree(data_max_len_str);
        }
        else
        {
            //data_max_len=0;
        }
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
	vCardData=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->data);  //malloc room for vCardData here,shall be free in future
       if(NULL==vCardData)
       {
             hs_smlLibFree(status);
             hs_smlLibFree(sourcelist);
           	return hs_smlFreeProtoElement(pContent); 
       }

       //syncml_cb_message:"mmipim @@ vCardData_content=%s "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_577_112_2_18_2_46_59_30,(uint8*)"s",vCardData);
       
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
       //�����ĵ�һ����
       if(data_max_len>0)
       {
        	//syncml_cb_message:"@@MMIPIM_PB_Add 4 "
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_583_112_2_18_2_46_59_31,(uint8*)"");

             large_obj_buf_ptr=hs_smlLibMalloc(__FILE__, __LINE__,data_max_len);
             
             if(NULL==large_obj_buf_ptr)
             {
                    hs_smlLibFree(status);
                    hs_smlLibFree(sourcelist);
                    hs_smlLibFree(vCardData);
               	return hs_smlFreeProtoElement(pContent); 
             }
             hs_smlLibMemset(large_obj_buf_ptr,0,data_max_len);

             //�����ַ���       
             hs_smlLibMemcpy(large_obj_buf_ptr,vCardData,strlen(vCardData)); 
             data_cur_len=strlen(vCardData);
             
             //����Luid
             hs_smlLibMemcpy(luri_str, pContent->itemList->item->source->locURI->content, pContent->itemList->item->source->locURI->length);
     
             //�����ʹ�����������
             status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "213");  //add fail
             //syncml_message:"@@MMIPIM_PB_Add��receive the large object,first chunk!"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_605_112_2_18_2_46_59_32,(uint8*)"");
       }
       else
       {
        	//syncml_cb_message:"@@MMIPIM_PB_Add 5 "
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_609_112_2_18_2_46_59_33,(uint8*)"");

             //���������İ�      
             if(0==hs_smlLibStrcmp(luri_str,pContent->itemList->item->source->locURI->content))
             {
                   //�������һ����
                   if(pContent->itemList->item->flags)
                   {
                         if(data_cur_len+pContent->itemList->item->data->length<data_max_len)
                         {
                              hs_smlLibMemcpy(large_obj_buf_ptr+data_cur_len,vCardData,strlen(vCardData)); 
                              data_cur_len+=strlen(vCardData);/*lint !e737*/
                              status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "213");  
                              //syncml_message:"@@MMIPIM_PB_Add��receive the large object,midile chunk!"
                              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_622_112_2_18_2_46_59_34,(uint8*)"");
                         }
                         else//�����������������һ������
                         {
                              status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "424");  //add fail
                              hs_smlLibFree(large_obj_buf_ptr);
                              //syncml_message:"@@MMIPIM_PB_Add��receive the large object,midile chunk,but the size is invalid!"
                              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_628_112_2_18_2_46_59_35,(uint8*)"");
                         }
                   }
                   else//���������һ����
                   {
                         if(data_cur_len+pContent->itemList->item->data->length==data_max_len)
                         {
                                 hs_smlLibMemcpy(large_obj_buf_ptr+data_cur_len,vCardData,strlen(vCardData)); 
                              
                                 hs_smlLibMemset(&entry_add, 0, sizeof(MMIPB_CONTACT_T));

                                 pMe->g_sync_result.receive_num++;
                                 if(0==MMIPB_GetFreeItemsCount())
                                 {
                                 		 status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "420");  //memory full !
                              		 hs_smlLibFree(large_obj_buf_ptr);
                              		 //syncml_message:"mmipim @@memory full! in handle add.@@"
                              		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_644_112_2_18_2_46_59_36,(uint8*)"");
                                 }
                      #ifdef PIM_CMCC_TEST_1001//Added by wangxiaolin   2010.1.8 for �������1001��ɾ��
                                else if(pMe->g_sync_result.deleted_num_for_phone==1000) //��������ɾ�������
                                {
                                          status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //memory full !
                              		 //syncml_message:"mmipim @@server process error! in handle add.@@"
                              		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_650_112_2_18_2_46_59_37,(uint8*)"");
                                }
                      #endif /* PIM_CMCC_TEST_1001 */
                                 else
                                 {
                      #ifdef PIM_CMCC_TEST_1001  //Added by wangxiaolin   2010.1.8 for �������1001��ɾ��
                                       BOOL    is_same_luid=FALSE;
                                       tempcharptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->source->locURI);
       
                                       temp=hs_globlemapItemList;
                                       while(NULL!=temp)
                                       {
                                           if(0==hs_smlLibStrcmp(tempcharptr, temp->target))
                                           {
                                               is_same_luid=TRUE;
                                               break;
                                           }
                                           else
                                           {
                                               temp=temp->next;
                                           }
                                       }
                                        hs_smlLibFree(tempcharptr);
                                        tempcharptr=NULL;
                                        if(is_same_luid)
                                        {
                                              status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //memory full !
                               		      //syncml_message:"mmipim @@server process error! in handle add.@@"
                               		      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_677_112_2_18_2_46_59_38,(uint8*)"");
                                        }
                                        else
                  #endif /* PIM_CMCC_TEST_1001 */
                                    	//   VCARD --> �绰����¼
                                         if( TRUE == MMIPIM_VCard_PB(large_obj_buf_ptr, &entry_add) )
                                   	{
                                                entry_add.contact_id= rec_id;
                                   		//   ��ӵ绰����¼
                                   		if(TRUE ==MMIPB_PimAdd(&entry_add))
                                   		{   
                                                      //syncml_message:"@@mmipim MMIPIM_PB_Add free_id=%d "
                                                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_688_112_2_18_2_46_59_39,(uint8*)"d",entry_add.contact_id);
                                                      //������ʾͬ�����
                                                      pMe->g_sync_result.added_num_for_phone++; //add by wangxiaolin 
                                                      
                                   			status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "201");  //add success
                                   			//syncml_message:"mmipim @@add record ok!! in handle add.@@"
                                   			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_693_112_2_18_2_46_59_40,(uint8*)"");
                                   			//  ���map �������=========================
                                   			
                                   			maplist=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlNewMapItemList_t));
                                                      if(NULL==maplist)
                                                      {
                                                             hs_smlLibFree(status);
                                                             hs_smlLibFree(sourcelist);
                                                             hs_smlLibFree(vCardData);
                                                             hs_smlLibFree(large_obj_buf_ptr);
                                                        	return hs_smlFreeProtoElement(pContent); 
                                                      }
                                   			hs_smlLibMemset(maplist, 0, (long)sizeof(SmlNewMapItemList_t));
                     
                                   			tempcharptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->source->locURI);
											str_len=hs_smlLibStrlen(tempcharptr);
											hs_smlLibMemcpy(maplist->target, tempcharptr, MIN(str_len, (MMIPIM_MAX_LOCAL_ID_LEN-1))); // chenxiang 20080702 target
                                   			hs_smlLibFree(tempcharptr);
                                   			tempcharptr=NULL;
                                               
                                   			tempcharptr=hs_smlunsignedInt2String(__FILE__, __LINE__, entry_add.contact_id);
											str_len=hs_smlLibStrlen(tempcharptr);
											hs_smlLibMemcpy(maplist->source, tempcharptr, MIN(str_len, (MMIPIM_MAX_LOCAL_ID_LEN-1))); // chenxiang 20080702 source
                                   			hs_smlLibFree(tempcharptr);
                                   			tempcharptr=NULL;

                                   			if(NULL==hs_globlemapItemList)
                                   			{
                                   				hs_globlemapItemList=maplist;
                                   			}
                                   			else
                                   			{
                                   				temp=hs_globlemapItemList;
                                   				while(NULL!=temp->next)
                                   				{
                                   					temp=temp->next;
                                   				}
                                   				temp->next=maplist;
                                   			}
                                                      hs_smlLibFree(large_obj_buf_ptr);
                                   			//��ӽ���=================================
                                   		}
                                   		else
                                   		{
                                   			status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add fail
                                   			hs_smlLibFree(large_obj_buf_ptr);
                                   			//syncml_message:"mmipim @@add record error!! in handle add.@@"
                                   			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_739_112_2_18_2_46_59_41,(uint8*)"");
                                   		}
                                   	}
                                  	else
                                  	{
                                  		 status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add fail
                                  		 hs_smlLibFree(large_obj_buf_ptr);
                                  		 //syncml_message:"mmipim @@analysis vcard error!! in handle add.@@"
                                  		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_746_112_2_18_2_46_59_42,(uint8*)"");
                                  	}
                                 }
                         }
                       else//�����������������һ������
                       {
                            status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "424");  //��Ƭ��С����
                            hs_smlLibFree(large_obj_buf_ptr);
                            //syncml_message:"@@MMIPIM_PB_Add��receive the large object,last chunk,but the size is invalid!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_754_112_2_18_2_46_59_43,(uint8*)"");
                       }
                       hs_smlLibMemset(luri_str,0,sizeof(luri_str));
                       data_max_len=0;

                   }
             }
           else//һ�����Ĵ���
           {
               	//syncml_cb_message:"@@MMIPIM_PB_Add 6 ,time=%d"
               	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_763_112_2_18_2_46_59_44,(uint8*)"d",PIM_GetCurTime());

                    //��luri��ʼ������ֹ���ֵ�͵�ǰ��ֵ��ͬ����ɴ����жϡ�
                    hs_smlLibMemset(luri_str,0,sizeof(luri_str));
                    data_max_len=0;

                    hs_smlLibMemset(&entry_add, 0, sizeof(MMIPB_CONTACT_T));

                    pMe->g_sync_result.receive_num++;
                    
                  if(0==MMIPB_GetFreeItemsCount())
                   {
                   		 status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "420");  //memory full !
                		 //syncml_message:"mmipim @@memory full! in handle add.@@"
                		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_776_112_2_18_2_46_59_45,(uint8*)"");
                   }
#ifdef PIM_CMCC_TEST_1001//Added by wangxiaolin   2010.1.8 for �������1001��ɾ��
                  else if(pMe->g_sync_result.deleted_num_for_phone==1000)
                  {
                            status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //memory full !
                		 //syncml_message:"mmipim @@server process error! in handle add.@@"
                		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_782_112_2_18_2_46_59_46,(uint8*)"");
                  }
#endif /* PIM_CMCC_TEST_1001 */
                   else
                   {
                #ifdef PIM_CMCC_TEST_1001  //Added by wangxiaolin   2010.1.8 for �������1001��ɾ��
                           BOOL    is_same_luid=FALSE;
                           tempcharptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->source->locURI);

                           temp=hs_globlemapItemList;
                           while(NULL!=temp)
                           {
                               if(0==hs_smlLibStrcmp(tempcharptr, temp->target))
                               {
                                   is_same_luid=TRUE;
                                   break;
                               }
                               else
                               {
                                   temp=temp->next;
                               }
                           }
                            hs_smlLibFree(tempcharptr);
                            tempcharptr=NULL;
                            if(is_same_luid)
                            {
                                  status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //memory full !
                   		      //syncml_message:"mmipim @@server process error! in handle add.@@"
                   		      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_809_112_2_18_2_46_59_47,(uint8*)"");
                            }
                            else
                  #endif /* PIM_CMCC_TEST_1001 */
                       	//   VCARD --> �绰����¼
                       	if( TRUE == MMIPIM_VCard_PB(vCardData, &entry_add) )
                       	{
                   		entry_add.contact_id= rec_id;
                               //syncml_cb_message:"@@MMIPIM vcard_parse finished ,time=%d"
                               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_817_112_2_18_2_46_59_48,(uint8*)"d",PIM_GetCurTime());

                   		//   ��ӵ绰����¼
                   		if(TRUE ==MMIPB_PimAdd(&entry_add))
                   		{   
                                      //������ʾͬ�����
                                      pMe->g_sync_result.added_num_for_phone++; //add by wangxiaolin 
                                      
                   			//  ǰ����ӵ�ʱ��������״̬���˴���״̬��Ϊ��ͬ��
                   			MMIPB_UpdatePimStatus( rec_id, PB_NO_MODIFY );
                   			status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "201");  //add success // chenxiang 20090206 200-->201
                   			//syncml_cb_message:"mmipim @@mmipim add record ok!! in handle add.@@ time=%d"
                   			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_828_112_2_18_2_46_59_49,(uint8*)"d",PIM_GetCurTime());
                   			//  ���map �������=========================
                   			
                         			maplist=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlNewMapItemList_t));
                                     if(NULL==maplist)
                                     {
                                            hs_smlLibFree(status);
                                            hs_smlLibFree(sourcelist);
                                            hs_smlLibFree(vCardData);
                                       	return hs_smlFreeProtoElement(pContent); 
                                     }
                   			hs_smlLibMemset(maplist, 0, (long)sizeof(SmlNewMapItemList_t));
           
                   			tempcharptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->source->locURI);
							str_len=hs_smlLibStrlen(tempcharptr);
                   			hs_smlLibMemcpy(maplist->target, tempcharptr, MIN(str_len, (MMIPIM_MAX_LOCAL_ID_LEN-1))); // chenxiang 20080702 target
                   			hs_smlLibFree(tempcharptr);
                   			tempcharptr=NULL;
                               
                   			tempcharptr=hs_smlunsignedInt2String(__FILE__, __LINE__, rec_id);
							str_len=hs_smlLibStrlen(tempcharptr);
                   			hs_smlLibMemcpy(maplist->source, tempcharptr, MIN(str_len, (MMIPIM_MAX_LOCAL_ID_LEN-1))); // chenxiang 20080702 source
                   			hs_smlLibFree(tempcharptr);
                   			tempcharptr=NULL;
             
                   			if(NULL==hs_globlemapItemList)
                   			{
                   				hs_globlemapItemList=maplist;
                   			}
                   			else
                   			{
                   				temp=hs_globlemapItemList;
                   				while(NULL!=temp->next)
                   				{
                   					temp=temp->next;
                   				}
                   				temp->next=maplist;
                   			}
                   			//��ӽ���=================================
                   		}
                   		else
                   		{
                   			status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add fail
                   			//syncml_cb_message:"mmipim @@add record error!! in handle add.@@"
                   			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_871_112_2_18_2_46_59_50,(uint8*)"");
                   		}
                   	}
                   	else
                   	{
                   		 status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add fail
                   		 //syncml_cb_message:"mmipim @@analysis vcard error!! in handle add.@@"
                   		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_877_112_2_18_2_46_59_51,(uint8*)"");
                   	}
                     }
               }
       }
 #else/* PIM_SUPPORT_LARGE_OBJECT_PROC */

       hs_smlLibMemset(&entry_add, 0, sizeof(MMIPB_CONTACT_T));
 
       pMe->g_sync_result.receive_num++;
       
       if(0==MMIPB_GetFreeItemsCount())
        {
    		 status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "420");  //memory full !
     		 //syncml_message:"mmipim @@memory full! in handle add.@@"
     		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_891_112_2_18_2_47_0_52,(uint8*)"");
        }
 #ifdef PIM_CMCC_TEST_1001//Added by wangxiaolin   2010.1.8 for �������1001��ɾ��
        else if(pMe->g_sync_result.deleted_num_for_phone==1000)
        {
                  status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //memory full !
      		 //syncml_message:"mmipim @@server process error! in handle add.@@"
      		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_897_112_2_18_2_47_0_53,(uint8*)"");
        }
#endif /* PIM_CMCC_TEST_1001 */
        else
        {
      #ifdef PIM_CMCC_TEST_1001  //Added by wangxiaolin   2010.1.8 for �������1001��ɾ��
           BOOL    is_same_luid=FALSE;
           tempcharptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->source->locURI);

           temp=hs_globlemapItemList;
           while(NULL!=temp)
           {
               if(0==hs_smlLibStrcmp(tempcharptr, temp->target))
               {
                   is_same_luid=TRUE;
                   break;
               }
               else
               {
                   temp=temp->next;
               }
           }
            hs_smlLibFree(tempcharptr);
            tempcharptr=NULL;
            if(is_same_luid)
            {
                  status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //memory full !
   		      //syncml_message:"mmipim @@server process error! in handle add.@@"
   		      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_924_112_2_18_2_47_0_54,(uint8*)"");
            }
            else
      #endif /* PIM_CMCC_TEST_1001 */
       	//   VCARD --> �绰����¼
       	if( TRUE == MMIPIM_VCard_PB(vCardData, &entry_add) )
           {
       		entry_add.contact_id= rec_id;
       		//   ��ӵ绰����¼
       		if(TRUE ==MMIPB_PimAdd(&entry_add))
       		{   
                          //������ʾͬ�����
                          pMe->g_sync_result.added_num_for_phone++; //add by wangxiaolin 
                          
       			//  ��״̬��Ϊ��ͬ��
       			MMIPB_UpdatePimStatus( rec_id, PB_NO_MODIFY );
       			status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "201");  //add success // chenxiang 20090206 200-->201
       			//syncml_cb_message:"@@mmipim add record ok!! in handle add.@@"
       			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_941_112_2_18_2_47_0_55,(uint8*)"");
       			//  ���map �������=========================
       			
       			maplist=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlNewMapItemList_t));
                        if(NULL==maplist)
                        {
                               hs_smlLibFree(status);
                               hs_smlLibFree(sourcelist);
                               hs_smlLibFree(vCardData);
                          	return hs_smlFreeProtoElement(pContent); 
                        }
       			hs_smlLibMemset(maplist, 0, (long)sizeof(SmlNewMapItemList_t));

       			tempcharptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->source->locURI);
				str_len=hs_smlLibStrlen(tempcharptr);
       			hs_smlLibMemcpy(maplist->target, tempcharptr, MIN(str_len, (MMIPIM_MAX_LOCAL_ID_LEN-1))); // chenxiang 20080702 target
       			hs_smlLibFree(tempcharptr);
       			tempcharptr=NULL;
                   
       			tempcharptr=hs_smlunsignedInt2String(__FILE__, __LINE__, rec_id);
				str_len=hs_smlLibStrlen(tempcharptr);
       			hs_smlLibMemcpy(maplist->source, tempcharptr, MIN(str_len, (MMIPIM_MAX_LOCAL_ID_LEN-1))); // chenxiang 20080702 source
       			hs_smlLibFree(tempcharptr);
       			tempcharptr=NULL;

       			if(NULL==hs_globlemapItemList)
       			{
       				hs_globlemapItemList=maplist;
       			}
       			else
       			{
       				temp=hs_globlemapItemList;
       				while(NULL!=temp->next)
       				{
       					temp=temp->next;
       				}
       				temp->next=maplist;
       			}
       			//��ӽ���=================================
       		}
       		else
       		{
       			status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add fail
       			//syncml_cb_message:"mmipim @@add record error!! in handle add.@@"
       			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_984_112_2_18_2_47_0_56,(uint8*)"");
       		}
       	}
       	else
       	{
       		 status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add fail
       		 //syncml_cb_message:"mmipim @@analysis vcard error!! in handle add.@@"
       		 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_990_112_2_18_2_47_0_57,(uint8*)"");
       	}
        }
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */

       
    	hs_smlLibFree(vCardData); //free vCardData
	hs_myAddstatustoCQ(status);
    //syncml_cb_message:"mmipim @@ add record finish.@@time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_998_112_2_18_2_47_0_58,(uint8*)"d",PIM_GetCurTime());
	
	return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}

/******************************************************************* 
FUNCTION ������:MMIPIM_PB_replace
�����������滻�绰����¼
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��: chenxiang 20071026  
****************************************************************/
PUBLIC short MMIPIM_PB_replace(short id, void* userData, SmlReplacePtr_t pContent)
{
	SmlStatusPtr_t  status=NULL;
	SmlTargetRefListPtr_t targetlist=NULL;
	char* vCardData=NULL;
	uint16 rec_id=0;
       char * rec_id_ptr=PNULL;
	MMIPB_CONTACT_T *entry_replace = PNULL;  
   #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
      //char * temp_buffer_ptr_for_large_obj=NULL;
      SmlMetInfMetInfPtr_t met_inf_ptr=NULL;
      char * data_max_len_str=NULL;
   #endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
       PIM_T_P  pMe=MMIPIM_GetPIM();//add by wangxiaolin
	status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
       if(NULL==status)
       {
           	return hs_smlFreeProtoElement(pContent); 
       }
	targetlist=(SmlTargetRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlTargetRefList_t));
       if(NULL==targetlist)
       {
             hs_smlLibFree(status);
           	return hs_smlFreeProtoElement(pContent); 
       }

	hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
	hs_smlLibMemset(targetlist, 0, (long)sizeof(SmlTargetRefList_t));

	status->elementType=SML_PE_STATUS;
	status->targetRefList=targetlist;  
	status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, hs_status_cmd_que->msgref->content);
	//status->cmdRef=pContent->cmdID;
	status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->cmdID->content); // chenxiang_memory 20080223
	status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "Replace");
	//status->targetRefList->targetRef=pContent->itemList->item->target->locURI;
	status->targetRefList->targetRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->itemList->item->target->locURI->content); // chenxiang_memory 20080223

      rec_id_ptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->target->locURI);
      //syncml_cb_message:"@@MMIPIM_PB_replace id_srt=%s "
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1050_112_2_18_2_47_0_59,(uint8*)"s",rec_id_ptr);

      rec_id=(uint16)hs_smlString2unsignedInt(rec_id_ptr);
      hs_smlLibFree(rec_id_ptr);
      
	status->targetRefList->next=NULL;
    
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
        if((SML_PCDATA_EXTENSION== pContent->meta->contentType)
            &&(SML_EXT_METINF==pContent->meta->extension))
        {
            met_inf_ptr=(SmlMetInfMetInfPtr_t)(pContent->meta->content);
            data_max_len_str=hs_smlPcdata2String(__FILE__, __LINE__,met_inf_ptr->maxobjsize);                 
            data_max_len=hs_smlString2unsignedInt(data_max_len_str);
            hs_smlLibFree(data_max_len_str);
        }
        else
        {
            //data_max_len=0;
        }
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */

    
	vCardData=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->data);  //malloc room for vCardData here,shall be free in future
       if(NULL==vCardData)
       {
             hs_smlLibFree(status);
             hs_smlLibFree(targetlist);
           	return hs_smlFreeProtoElement(pContent); 
       }
       //syncml_cb_message:"@@MMIPIM_PB_replace id=%d,content=%s "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1080_112_2_18_2_47_0_60,(uint8*)"ds",rec_id,vCardData);

#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
       //�����ĵ�һ����
       if(data_max_len>0)
       {

             large_obj_buf_ptr=hs_smlLibMalloc(__FILE__, __LINE__,data_max_len);
             
             if(NULL==large_obj_buf_ptr)
             {
                    hs_smlLibFree(status);
                    hs_smlLibFree(targetlist);
                    hs_smlLibFree(vCardData);
               	return hs_smlFreeProtoElement(pContent); 
             }
             hs_smlLibMemset(large_obj_buf_ptr,0,data_max_len);

             //�����ַ���       
             hs_smlLibMemcpy(large_obj_buf_ptr,vCardData,strlen(vCardData)); 
             data_cur_len=strlen(vCardData);
             
             //����Luid
             hs_smlLibMemcpy(luri_str, pContent->itemList->item->target->locURI->content, pContent->itemList->item->target->locURI->length);
     
             //�����ʹ�����������
             status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "213");  //add fail
             //syncml_message:"@@MMIPIM_PB_replace��receive the large object,first chunk!"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1107_112_2_18_2_47_0_61,(uint8*)"");
       }
       else
       {
        	//syncml_cb_message:"@@MMIPIM_PB_replace,large object's middle chunk!or normal object!"
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1111_112_2_18_2_47_0_62,(uint8*)"");
        	//syncml_cb_message:"@@MMIPIM_PB_replace, g_luri=%s,m_luri=%s "
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1112_112_2_18_2_47_0_63,(uint8*)"ss",luri_str,pContent->itemList->item->target->locURI->content);

             //���������İ�      
             if(0==hs_smlLibStrcmp(luri_str,(char*)pContent->itemList->item->target->locURI->content))
             {
                      //syncml_cb_message:"@@MMIPIM_PB_replace 2 "
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1117_112_2_18_2_47_0_64,(uint8*)"");

                   //�������һ����
                   if(pContent->itemList->item->flags)
                   {
                        //syncml_cb_message:"@@MMIPIM_PB_replace 3 "
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1122_112_2_18_2_47_0_65,(uint8*)"");

                         if(data_cur_len+pContent->itemList->item->data->length<data_max_len)
                         {
                              hs_smlLibMemcpy(large_obj_buf_ptr+data_cur_len,vCardData,strlen(vCardData)); 
                              data_cur_len+=strlen(vCardData);/*lint !e737*/
                              status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "213");  
                              //syncml_message:"@@MMIPIM_PB_replace��receive the large object,midile chunk!"
                              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1129_112_2_18_2_47_0_66,(uint8*)"");
                         }
                         else//�����������������һ������
                         {
                             //syncml_message:"@@MMIPIM_PB_replace��receive the large object,midile chunk,but the size is invalid!"
                             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1133_112_2_18_2_47_0_67,(uint8*)"");
                              status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //add fail
                              hs_smlLibFree(large_obj_buf_ptr);
                         }
                   }
                   else//���������һ����
                   {

                         if(data_cur_len+pContent->itemList->item->data->length==data_max_len)
                         {
                            	entry_replace = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
                                SCI_MEMSET(entry_replace, 0x00, sizeof(MMIPB_CONTACT_T));
                                 hs_smlLibMemcpy(large_obj_buf_ptr+data_cur_len,vCardData,strlen(vCardData)); 
                              
                                 hs_smlLibMemset(entry_replace, 0, sizeof(MMIPB_CONTACT_T));
                                 
                                 pMe->g_sync_result.receive_num++;
                                 //   VCARD --> �绰����¼
                                 if( TRUE == MMIPIM_VCard_PB(large_obj_buf_ptr, entry_replace) )
                                 {
                                     entry_replace->contact_id = rec_id;
                                     
                                     // ��ӵ绰����¼
                                     if(TRUE ==MMIPB_PimModify(entry_replace))
                                     {
                                         pMe->g_sync_result.replaced_num_for_phone++; //add by wangxiaolin 
                                         
                                         //  ǰ���޸ļ�¼��ʱ��������״̬���˴���״̬��Ϊ��ͬ��
                                         MMIPB_UpdatePimStatus( rec_id, PB_NO_MODIFY );
                                         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add success
                                         //syncml_cb_message:"mmipim@@replace record ok!! in handle replace.@@"
                                         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1163_112_2_18_2_47_0_68,(uint8*)"");
                                     }
                                     else
                                     {
                                         status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //replace fail
                                         //syncml_cb_message:"mmipim@@replace record error!! in handle replace 1 .@@"
                                         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1168_112_2_18_2_47_0_69,(uint8*)"");
                                     }
                                }
                            	SCI_FREE(entry_replace);
 
                         }
                         else//�����������������һ������
                         {
                              //syncml_message:"@@MMIPIM_PB_replace��receive the large object,last chunk,but the size is invalid!"
                              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1176_112_2_18_2_47_0_70,(uint8*)"");
                              status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //replace fail
                         }
                         hs_smlLibFree(large_obj_buf_ptr);
                         hs_smlLibMemset(luri_str,0,sizeof(luri_str));
                         data_max_len=0;
                   }
             }
           else//һ�����Ĵ���
           {

                entry_replace = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
                SCI_MEMSET(entry_replace, 0x00, sizeof(MMIPB_CONTACT_T));
                //syncml_cb_message:"@@MMIPIM_PB_replace 5 "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1189_112_2_18_2_47_0_71,(uint8*)"");
                hs_smlLibMemset(entry_replace, 0, sizeof(MMIPB_CONTACT_T));
                hs_smlLibMemset(luri_str,0,sizeof(luri_str));
                 data_max_len=0;

                pMe->g_sync_result.receive_num++;
                //   VCARD --> �绰����¼
                if( TRUE == MMIPIM_VCard_PB(vCardData, entry_replace) )
                {
                    entry_replace->contact_id = rec_id;
                    // ��ӵ绰����¼
                    if(TRUE ==MMIPB_PimModify(entry_replace))
                    {
                        pMe->g_sync_result.replaced_num_for_phone++; //add by wangxiaolin 
                        
                        //  ǰ���޸ļ�¼��ʱ��������״̬���˴���״̬��Ϊ��ͬ��
                        MMIPB_UpdatePimStatus( rec_id, PB_NO_MODIFY );
                        status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add success
                        //syncml_cb_message:"mmipim@@replace record ok!! in handle replace.@@"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1207_112_2_18_2_47_0_72,(uint8*)"");
                    }
                    else
                    {
                        //syncml_cb_message:"mmipim@@replace record error!! in handle replace 2.@@"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1211_112_2_18_2_47_0_73,(uint8*)"");
                        status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //replace fail
                    }
                }
                else
                {
                    //syncml_cb_message:"mmipim@@analysis vcard error!! in handle replace. 3@@"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1217_112_2_18_2_47_0_74,(uint8*)"");
                    status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //replace fail
                }       
                SCI_FREE(entry_replace);
            }
       }
 #else/* PIM_SUPPORT_LARGE_OBJECT_PROC */
    	entry_replace = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
        SCI_MEMSET(entry_replace, 0x00, sizeof(MMIPB_CONTACT_T));
	hs_smlLibMemset(entry_replace, 0, sizeof(MMIPB_CONTACT_T));
 
       pMe->g_sync_result.receive_num++;
	//   VCARD --> �绰����¼
	if( TRUE == MMIPIM_VCard_PB(vCardData, entry_replace) )
	{
		entry_replace->entry_id = rec_id;
		// ��ӵ绰����¼
		if(TRUE ==MMIPB_PimModify(entry_replace))
		{
                    pMe->g_sync_result.replaced_num_for_phone++; //add by wangxiaolin 
                    
               	//  ǰ���޸ļ�¼��ʱ��������״̬���˴���״̬��Ϊ��ͬ��
			MMIPB_UpdatePimStatus( rec_id, PB_NO_MODIFY );
			status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add success
			//syncml_cb_message:"mmipim@@replace record ok!! in handle replace.@@"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1241_112_2_18_2_47_0_75,(uint8*)"");
		}
		else
		{
			status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //replace fail
			//syncml_cb_message:"mmipim@@replace record error!! in handle replace.@@"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1246_112_2_18_2_47_0_76,(uint8*)"");
		}
	}
	else
	{
		status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "400");  //replace fail
		//syncml_cb_message:"mmipim@@analysis vcard error!! in handle replace.@@"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1252_112_2_18_2_47_0_77,(uint8*)"");
	}
    	SCI_FREE(entry_replace);

  #endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */

	hs_smlLibFree(vCardData); //free vCardData
	hs_myAddstatustoCQ(status);
	return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}


/******************************************************************* 
FUNCTION ������:MMIPIM_PB_delete
����������ɾ���绰���еļ�¼
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��:  chenxiang 20071026 
****************************************************************/
PUBLIC short MMIPIM_PB_delete(short id, void* userData, SmlDeletePtr_t pContent)
{
	SmlStatusPtr_t  status;
	SmlTargetRefListPtr_t targetlist;
	uint16 rec_id=0;
       char * rec_id_ptr=PNULL;
       PIM_T_P  pMe=MMIPIM_GetPIM();//add by wangxiaolin
	status=(SmlStatusPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlStatus_t));
       if(NULL==status)
       {
           	return hs_smlFreeProtoElement(pContent); 
       }
	targetlist=(SmlTargetRefListPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlTargetRefList_t));
       if(NULL==targetlist)
       {
             hs_smlLibFree(status);
           	return hs_smlFreeProtoElement(pContent); 
       }

	hs_smlLibMemset(status, 0, (long)sizeof(SmlStatus_t));
	hs_smlLibMemset(targetlist, 0, (long)sizeof(SmlTargetRefList_t));

	status->elementType=SML_PE_STATUS;
	status->targetRefList=targetlist;  
	status->msgRef=hs_smlString2Pcdata(__FILE__, __LINE__, hs_status_cmd_que->msgref->content);
	//status->cmdRef=pContent->cmdID;
	status->cmdRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->cmdID->content); // chenxiang_memory 20080223
	status->cmd=hs_smlString2Pcdata(__FILE__, __LINE__, "Delete");
	//status->targetRefList->targetRef=pContent->itemList->item->target->locURI;
	status->targetRefList->targetRef=hs_smlString2Pcdata(__FILE__, __LINE__, pContent->itemList->item->target->locURI->content); // chenxiang_memory 20080223

      rec_id_ptr=hs_smlPcdata2String(__FILE__, __LINE__, pContent->itemList->item->target->locURI);
      //syncml_cb_message:"@@MMIPIM_PB_delete id_srt=%s "
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1304_112_2_18_2_47_0_78,(uint8*)"s",rec_id_ptr);
      rec_id=(uint16)hs_smlString2unsignedInt(rec_id_ptr);
      
      hs_smlLibFree(rec_id_ptr);
 
       status->targetRefList->next=NULL;
       //syncml_cb_message:"@@MMIPIM_PB_delete id=%d "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1310_112_2_18_2_47_0_79,(uint8*)"d",rec_id);
	//ɾ����¼
       pMe->g_sync_result.receive_num++;
    	if ( TRUE == MMIPB_PimDelete( rec_id))
	{
             pMe->g_sync_result.deleted_num_for_phone++; //add by wangxiaolin  // chenxiang 20080702 deleted_num_for_phone
            
             //  ǰ��ɾ����ʱ��������״̬���˴���״̬��Ϊ��ͬ��
		MMIPB_UpdatePimStatus( rec_id, PB_NO_MODIFY );
		status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "200");  //add success
		//syncml_cb_message:"mmipim@@delete record ok!! in handle delete.@@"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1320_112_2_18_2_47_0_80,(uint8*)"");
	}
	else  
	{
              //��ͬ����Ҫ���ǣ�����Ҳ���Ҫɾ���ļ�¼������
		status->data=hs_smlString2Pcdata(__FILE__, __LINE__, "211");  //delete fail
		//syncml_cb_message:"mmipim@@delete reocrd error!! in handle delete.@@"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1326_112_2_18_2_47_0_81,(uint8*)"");
	}
	hs_myAddstatustoCQ(status);
	return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}


/******************************************************************* 
FUNCTION ������:MMIPIM_GetPBVcardItem
�������������绰���е�һ����¼ת��VCARD�����buffer��
** ��  ��: 
** ��  ��:����
** ��  ��:2007-10-26
** �汾: ver 1.0
**ע��: chenxiang 20071026 
****************************************************************/
PUBLIC int MMIPIM_GetPBVcardItem
(
uint16 entry_id , //IN: �绰����¼��
char* buffer,      // OUT: �����VCARD
uint32 buffersize //IN: ��������С
)
{

	//BOOLEAN result;
	MMIPB_CONTACT_T *pb_entry = PNULL; 
	unsigned int bytes_write;
	uint32 totalsize =0;
	int	ucs2_name_len = 0;
	uint8 length = 0;
	uint8   str_ptr[100];
	char utf8_ptr[MAX_CHAR];
	char qp_ptr[MAX_CHAR] = {0};
       int max_lenth_limit_by_vcard=0;
        
	pb_entry = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
    SCI_MEMSET(pb_entry, 0x00, sizeof(MMIPB_CONTACT_T));
	hs_smlLibMemset(pb_entry, 0, sizeof(MMIPB_CONTACT_T)); // chenxiang 20080202
	pb_entry->contact_id= entry_id;
      //syncml_cb_message:"@@MMIPIM_GetPBVcardItem id=%d "
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1364_112_2_18_2_47_1_82,(uint8*)"d",entry_id);
	//�ӵ绰���ж�ȡһ����¼
	MMIAPIPB_GetContactInfo(pb_entry->contact_id,MMIAPIPB_GetStorageId(MN_DUAL_SYS_1, PHONEBOOK_NV_STORAGE),pb_entry);


	hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
	//��vcardͷд���ļ�
	sprintf(wrBuf1, "%s\r\n", BEGIN_VCARD_DELIMITER);

	hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
	bytes_write = SCI_STRLEN(wrBuf1);
	totalsize +=bytes_write;
	//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem  start = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1376_112_2_18_2_47_1_83,(uint8*)"d",totalsize);


	// Write Version..
	hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
	hs_smlLibStrcpy(wrBuf1, "VERSION:2.1\r\n");

	hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));

	bytes_write = SCI_STRLEN(wrBuf1);
	totalsize +=bytes_write;
	//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem  head ,totalsize= %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1387_112_2_18_2_47_1_84,(uint8*)"d",totalsize);


	// Write Name..
	if((pb_entry->name.wstr_len) !=0)
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(utf8_ptr, 0, sizeof(utf8_ptr));

		//ucs->utf8

		hs_smlLibMemset(qp_ptr, 0, sizeof(qp_ptr));

              //�ж��Ƿ��Ǻ���
              if(MMIAPICOM_IsASCIIString(pb_entry->name.wstr,pb_entry->name.wstr_len))
              {
                max_lenth_limit_by_vcard=(MMIPIM_PB_NAME_LEN/2+10);
              }
              else
              {
                max_lenth_limit_by_vcard=MMIPIM_PB_NAME_LEN/2;

              }
	
		//SCI_TRACE_LOW:"MMIPIM_GetPBVcardItem entry_ptr->entry_name.wstr_len / 2 = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1411_112_2_18_2_47_1_85,(uint8*)"d", pb_entry->name.wstr_len / 2);
             
             //�����ת�������⣬���Կ���GUI_WstrToUTF8����ת��
		VmUnicodeToByte((const uint16*)pb_entry->name.wstr, 
						MIN(pb_entry->name.wstr_len,max_lenth_limit_by_vcard),
						(uint8*)utf8_ptr, 
						sizeof(utf8_ptr));

		//utf8->qp
		ucs2_name_len = EncodeQuoted((unsigned char*)utf8_ptr, qp_ptr, SCI_STRLEN(utf8_ptr));


		sprintf(wrBuf1, "N;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:%s\r\n",qp_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,name, totalsize = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1428_112_2_18_2_47_1_86,(uint8*)"d",totalsize);
	}

	// Write Mobile Tel..
	if((pb_entry->number[0].number_len) !=0)//number_t[0]
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(str_ptr, 0, sizeof(str_ptr));
        length = MMIAPIPB_BCDNumberToString(&pb_entry->number[0], 42, str_ptr);
// 		length= MMIAPICOM_GenDispNumber(
// 								MMIPB_GetNumberTypeFromUint8(pb_entry->number[0].npi_ton),
// 								(uint8)MIN(pb_entry->number[0].number_len,MMIPIM_PB_TEL_LEN/2),
// 								pb_entry->number[0].number,
// 								str_ptr,
// 								42
// 								);

		//uim_tk_test_print_data(str_ptr, SCI_STRLEN((char*)str_ptr));
		sprintf((char*)wrBuf1, "TEL;CELL:%s\r\n", str_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,tel0, totalsize= %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1450_112_2_18_2_47_1_87,(uint8*)"d",totalsize);
	}


	// Write Home Tel..
	if((pb_entry->number[1].number_len) !=0)//number_t[0]
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(str_ptr, 0, sizeof(str_ptr));
        length = MMIAPIPB_BCDNumberToString(&pb_entry->number[1], 42, str_ptr);
// 		length= MMIAPICOM_GenDispNumber(
// 								MMIPB_GetNumberTypeFromUint8(pb_entry->number[1].npi_ton),
// 								(uint8)MIN(pb_entry->number[1].number_len,MMIPIM_PB_TEL_LEN/2),
// 								pb_entry->number[1].number,
// 								str_ptr,
// 								42
// 								);
             sprintf((char*)wrBuf1, "TEL;HOME:%s\r\n", str_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,tel1, totalsize= %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1471_112_2_18_2_47_1_88,(uint8*)"d",totalsize);
	}

#ifndef PB_SURPPORT_LOW_MEMORY
	// Write OFFICE Tel..
	if((pb_entry->number[2].number_len) !=0)//VCARD_NUM_TYPE_OFFICE
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(str_ptr, 0, sizeof(str_ptr));
        length = MMIAPIPB_BCDNumberToString(&pb_entry->number[2], 42, str_ptr);
// 		length= MMIAPICOM_GenDispNumber(
// 								MMIPB_GetNumberTypeFromUint8(pb_entry->number[2].npi_ton),
// 								(uint8)MIN(pb_entry->number[2].number_len,MMIPIM_PB_TEL_LEN/2),
// 								pb_entry->number[2].number,
// 								str_ptr,
// 								42
// 	      					);
             sprintf((char*)wrBuf1, "TEL;WORK:%s\r\n", str_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,tel2, totalsize= %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1492_112_2_18_2_47_1_89,(uint8*)"d",totalsize);
	}
#ifdef HS_PIM_SUPPORT_ALL_FIELD  //Added by wangxiaolin  2009.7.9 for �绰�������ֶ�
    //�绰����
    /*addi_number_info[4].addi_number  -> TEL*/
	if((pb_entry->number[3].number_len) !=0)
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(str_ptr, 0, sizeof(str_ptr));
        length = MMIAPIPB_BCDNumberToString(&pb_entry->number[3], 42, str_ptr);
// 		length= MMIAPICOM_GenDispNumber(
// 								MMIPB_GetNumberTypeFromUint8(pb_entry->number[3].npi_ton),
// 								MIN(pb_entry->number[3].number_len,MMIPIM_PB_TEL_LEN/2),
// 								pb_entry->number[3].number,
// 								str_ptr,
// 								42
// 	      					);
             sprintf((char*)wrBuf1, "TEL:%s\r\n", str_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,tel3, totalsize= %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1513_112_2_18_2_47_1_90,(uint8*)"d",totalsize);
	}
    
    
    //����
    /*addi_number_info[5].addi_number  ->  TEL;FAX*/ //�������
    	if((pb_entry->number[4].number_len) !=0)
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(str_ptr, 0, sizeof(str_ptr));
        length = MMIAPIPB_BCDNumberToString(&pb_entry->number[4], 42, str_ptr);
// 		length= MMIAPICOM_GenDispNumber(
// 								MMIPB_GetNumberTypeFromUint8(pb_entry->number[4].npi_ton),
// 								MIN(pb_entry->number[4].number_len,MMIPIM_PB_TEL_LEN/2),
// 								pb_entry->number[4].number,
// 								str_ptr,
// 								42
// 	      					);
            sprintf((char*)wrBuf1, "TEL;FAX:%s\r\n", str_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,tel4, totalsize= %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1535_112_2_18_2_47_1_91,(uint8*)"d",totalsize);
	}
   
#endif /* HS_PIM_SUPPORT_ALL_FIELD */

#endif  //PB_SURPPORT_LOW_MEMORY

#ifdef MMIPB_MAIL_SUPPORT
	// Write email 
	if((pb_entry->mail.wstr_len)!= 0)
	{

      	       hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(utf8_ptr, 0, sizeof(utf8_ptr));

		//ucs->utf8
		hs_smlLibMemset(qp_ptr, 0, sizeof(qp_ptr));
		//SCI_TRACE_LOW:"mmipim entry_ptr->mail.wstr_len / 2 = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1551_112_2_18_2_47_1_92,(uint8*)"d", pb_entry->mail.wstr_len/ 2);
              
		VmUnicodeToByte((const uint16*)pb_entry->mail.wstr, 
						MIN(pb_entry->mail.wstr_len, MMIPIM_PB_EMAIL_LEN),
						(uint8*)utf8_ptr, 
						sizeof(utf8_ptr));
            //SCI_TRACE_LOW:"mmipim entry_ptr->mail= %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1557_112_2_18_2_47_1_93,(uint8*)"s", utf8_ptr);
		//utf8->qp
		ucs2_name_len = EncodeQuoted((unsigned char*)utf8_ptr, qp_ptr, SCI_STRLEN(utf8_ptr));
        
		//sprintf(wrBuf1, "EMAIL;INTERNET:%s\r\n",qp_ptr);
		sprintf(wrBuf1, "EMAIL;INTERNET;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:%s\r\n",qp_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,Email, totalsize = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1566_112_2_18_2_47_1_94,(uint8*)"d",totalsize);
	}
#endif

#ifdef HS_PIM_SUPPORT_ALL_FIELD  //Added by wangxiaolin  2009.7.9 for �绰�������ֶ�
#ifndef PB_SUPPORT_LOW_MEMORY
	// ��ע
	if((pb_entry->memo.wstr_len) !=0)
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(utf8_ptr, 0, sizeof(utf8_ptr));

		//ucs->utf8

		hs_smlLibMemset(qp_ptr, 0, sizeof(qp_ptr));

	
		//SCI_TRACE_LOW:"MMIPIM_GetPBVcardItem entry_ptr->custom_text.wstr_len = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1582_112_2_18_2_47_1_95,(uint8*)"d", pb_entry->memo.wstr_len );
             
             //�����ת�������⣬���Կ���GUI_WstrToUTF8����ת��
		VmUnicodeToByte((const uint16*)pb_entry->memo.wstr, 
						MIN(pb_entry->memo.wstr_len, MMIPIM_PB_NOTE_LEN),
						(uint8*)utf8_ptr, 
						sizeof(utf8_ptr));

		//utf8->qp
		ucs2_name_len = EncodeQuoted((unsigned char*)utf8_ptr, qp_ptr, SCI_STRLEN(utf8_ptr));


		sprintf(wrBuf1, "NOTE;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:%s\r\n",qp_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,Note,totalsize = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1599_112_2_18_2_47_1_96,(uint8*)"d",totalsize);
	}
#endif    
#if 0
  	// ְ��
	if((pb_entry->title.wstr_len) !=0)
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(utf8_ptr, 0, sizeof(utf8_ptr));

		//ucs->utf8

		hs_smlLibMemset(qp_ptr, 0, sizeof(qp_ptr));
	
		//SCI_TRACE_LOW:"MMIPIM_GetPBVcardItem entry_ptr->title.wstr_len = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1612_112_2_18_2_47_1_97,(uint8*)"d", pb_entry->title.wstr_len );
             
             //�����ת�������⣬���Կ���GUI_WstrToUTF8����ת��
		VmUnicodeToByte((const uint16*)pb_entry->title.wstr, 
						MIN(pb_entry->title.wstr_len,MMIPIM_PB_TITLE_LEN), 
						(uint8*)utf8_ptr, 
						sizeof(utf8_ptr));

		//utf8->qp
		ucs2_name_len = EncodeQuoted((unsigned char*)utf8_ptr, qp_ptr, SCI_STRLEN(utf8_ptr));

		sprintf(wrBuf1, "TITLE;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:%s\r\n",qp_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,title,totalsize  = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1628_112_2_18_2_47_1_98,(uint8*)"d",totalsize);
	}
#endif
    #if 0
  	// ��˾����
	if((pb_entry->company.wstr_len) !=0)
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(utf8_ptr, 0, sizeof(utf8_ptr));

		//ucs->utf8
		hs_smlLibMemset(qp_ptr, 0, sizeof(qp_ptr));
		//SCI_TRACE_LOW:"MMIPIM_GetPBVcardItem entry_ptr->org.wstr_len = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1639_112_2_18_2_47_1_99,(uint8*)"d", pb_entry->company.wstr_len );
             //�����ת�������⣬���Կ���GUI_WstrToUTF8����ת��
		VmUnicodeToByte((const uint16*)pb_entry->company.wstr, 
						MIN(pb_entry->company.wstr_len, MMIPIM_PB_ORG_LEN),
						(uint8*)utf8_ptr, 
						sizeof(utf8_ptr));

		//utf8->qp
		ucs2_name_len = EncodeQuoted((unsigned char*)utf8_ptr, qp_ptr, SCI_STRLEN(utf8_ptr));
		sprintf(wrBuf1, "ORG;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:%s\r\n",qp_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,org,totalsize  = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1653_112_2_18_2_47_1_100,(uint8*)"d",totalsize);
	}

  	// ������ַ
	if((pb_entry.work_adr.wstr_len) !=0)
	{
		hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
		hs_smlLibMemset(utf8_ptr, 0, sizeof(utf8_ptr));

		//ucs->utf8
		hs_smlLibMemset(qp_ptr, 0, sizeof(qp_ptr));
		//SCI_TRACE_LOW:"entry_ptr->work_adr.wstr_len = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1664_112_2_18_2_47_1_101,(uint8*)"d", pb_entry.work_adr.wstr_len );
             //�����ת�������⣬���Կ���GUI_WstrToUTF8����ת��
		VmUnicodeToByte((const uint16*)pb_entry.work_adr.wstr, 
						MIN(pb_entry.work_adr.wstr_len,MMIPIM_PB_ADR_ROAD_LEN), 
						(uint8*)utf8_ptr, 
						sizeof(utf8_ptr));

		//utf8->qp
		ucs2_name_len = EncodeQuoted((unsigned char*)utf8_ptr, qp_ptr, SCI_STRLEN(utf8_ptr));
		sprintf(wrBuf1, "ADR;WORK;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:;%s;;;;;\r\n",qp_ptr);
		hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));
		
		bytes_write = SCI_STRLEN(wrBuf1);
		totalsize += bytes_write;
		//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem,work_adr,totalsize  = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1678_112_2_18_2_47_1_102,(uint8*)"d",totalsize);
	}
#endif

  #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.6.22  
    //��ͷ��
#if 0//Added by wangxiaolin  2009.9.5
    if(pb_entry.photo_ring_id_flag& MMIPB_PHOTO_ID_EXIST)
    {
        char * suffix_pos=NULL;
        MMIPB_CUSTOM_PHOTO_T     custom_photo ={0};
         int photo_data_size=0;

        if(MMIPB_ReadPhotoFileName(pb_entry.group_id, pb_entry.entry_id, &custom_photo))
        {

            if(custom_photo.is_valid&custom_photo.file_size<PHOTO_FILE_LEN)
            {
                //syncml_message:" enter MMIPIM_GetPBVcardItem totalsize file_size  = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1696_112_2_18_2_47_1_103,(uint8*)"d",custom_photo.file_size);

                hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
                //hs_smlLibMemset(str_ptr, 0, sizeof(str_ptr)); //��ʾ����
                //hs_smlLibMemcpy(str_ptr,suffix_pos+1,10);
                //sprintf(wrBuf1, "PHOTO;CHARSET=UTF-8;ENCODING=BASE64;TYPE=%s:r\n", str_ptr);

                sprintf(wrBuf1, "PHOTO;CHARSET=UTF-8;ENCODING=BASE64;TYPE=JPG:" );

                hs_smlLibMemcpy(buffer + totalsize, wrBuf1, hs_smlLibStrlen(wrBuf1));
                bytes_write = hs_smlLibStrlen(wrBuf1);
                totalsize += bytes_write;
                
                //���ļ�ϵͳ��ȡphoto����ת����BASE64��������
                photo_data_size=PhotoFile2Base64String((const char*)custom_photo.file_name,buffer + totalsize);
               
                totalsize+=photo_data_size;
            
            }
            else
            {
                //syncml_message:" MMIPIM_GetPBVcardItem ,object is too big,memory is not enough!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1717_112_2_18_2_47_1_104,(uint8*)"");
            }
        }
        else
        {
                 //syncml_message:" MMIPIM_GetPBVcardItem ,get photo data fail!"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1722_112_2_18_2_47_1_105,(uint8*)"");
        }
    }
#endif//Added by wangxiaolin  2009.9.5
 #endif //

#endif /* HS_PIM_SUPPORT_ALL_FIELD */


	// Write End..
	hs_smlLibMemset(wrBuf1, 0, sizeof(wrBuf1));
	strcpy(wrBuf1, "END:VCARD\r\n");

	hs_smlLibMemcpy(buffer + totalsize, wrBuf1, SCI_STRLEN(wrBuf1));

	bytes_write = SCI_STRLEN(wrBuf1);
	totalsize +=bytes_write;
	//SCI_TRACE_LOW:" enter MMIPIM_GetPBVcardItem ,end ,totalsize = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1739_112_2_18_2_47_1_106,(uint8*)"d",totalsize);

	if (buffersize >= totalsize)
	{
		buffer[totalsize] = 0;
		buffer[totalsize + 1] = 0;
	}
	else
	{
        if (buffersize>=2)
        {
            buffer[buffersize-2] = 0;
        }
        if (buffersize>=1)
        {
            buffer[buffersize-1] = 0;
        }
	}

   SCI_FREE(pb_entry);
   return MIN(totalsize,buffersize);

}
/******************************************************************* 
FUNCTION ������:hs_myGetAddedPBItems
�����������õ�δͬ�����¼ӵĵ绰����Ŀ,����
**                        itemlist�����Ա�����Add����ʹ�á�
** ��  ��: itemlist�����һ��ָ�롣
** ��  ��:����
** ��  ��:2006-8-2
** �汾: ver 1.0
**ע��:�˴���̬�����ڴ棬�������ĵ�����syncml_AddCmd
**             ���ͷš�
****************************************************************/
SmlItemListPtr_t hs_myGetAddedPBItems(short syncway) /* chenxiang 20071026 */
{

	uint16 i=0;
	boolean retval=FALSE; // chenxiang 20071124
	char*  buffer=PNULL;
       char*  temp=PNULL; // chenxiang_memory 20080312
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
       uint32 buffersize=MAX_DATA_BUF_LEN;
       int data_lenth=0;
#else
	uint32 buffersize=VCARD_LEN_EXCLUDE_PHOTO_SEGMENT;
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
       int total_data_len=0;
	SmlItemListPtr_t tmptr=NULL;
	SmlItemListPtr_t itemList=NULL;
	SmlItemPtr_t item=NULL;
	SmlSourcePtr_t source=NULL;
	MMIPB_CONTACT_T *pb_entry = PNULL;  // chenxiang 20080202	
    
       PIM_T_P  pMe=MMIPIM_GetPIM();
       
	buffer=hs_smlLibMalloc(__FILE__, __LINE__, (long)buffersize);
    
       if(buffer==NULL)
       {
                return NULL;
       }
       
    
	hs_smlLibMemset(buffer, 0, buffersize);
	pb_entry = (MMIPB_CONTACT_T *)SCI_ALLOC_APP(sizeof(MMIPB_CONTACT_T));
        SCI_MEMSET(pb_entry, 0x00, sizeof(MMIPB_CONTACT_T));

	//wangxiaolin 20071210
	for( i =(uint16)( pMe->g_sync_result.current_num_for_add+1); i <=MMIPIM_MAX_PHONEBOOK_RECORDS; i++ )
	{
        /*Start of  wangxiaolin 2009.8.25 for �Ż����� */
              if( pMe->g_sync_result.added_num_for_server==pMe->g_sync_result.send_num -pMe->g_sync_result.deleted_num_for_server-pMe->g_sync_result.replaced_num_for_server)
              {
                    //syncml_cb_message:"^@@^MMIPIM hs_myGetAddedPBItems ,add_is_final,without check all items!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1812_112_2_18_2_47_1_107,(uint8*)"");
                     pMe->g_sync_result.add_is_final=TRUE;
                     break;
              }
       /*End   of  wangxiaolin 2009.8.25 for �Ż����� */
           
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.9 for ����ͬ��&  ˢ��ͬ��
		if((SYNC_TYPE_S2WAY == syncway )||( SYNC_TYPE_RFCLI== syncway )) //two way slow sync
		{
                	hs_smlLibMemset(pb_entry, 0, sizeof(MMIPB_CONTACT_T)); // chenxiang 20080202
			pb_entry->contact_id= i;  // chenxiang 20080202
                    if (MMIPB_ERROR_SUCCESS==MMIAPIPB_GetContactInfo(pb_entry->contact_id,MMIAPIPB_GetStorageId(MN_DUAL_SYS_1, PHONEBOOK_NV_STORAGE),(MMIPB_CONTACT_T *)&pb_entry))
			{
				retval = TRUE;  //������в�Ϊ�յ�����
			}
                    else //add by wangxiaolin
                    {
                        retval = FALSE;
                    }
		}
		else if((SYNC_TYPE_2WAY == syncway )||( SYNC_TYPE_1WAYFCLI== syncway ))
		{  //two way (fast) sync
                   if (PB_RECENT_ADD== MMIPB_GetPimStatus(i))
                   {
                        retval = TRUE;
                   }
                   else
                   {
                        retval = FALSE;
                   }
    	     }
            else
            {
                //syncml_message:"MMIPIM hs_myGetAddedPBItems,no need updata local contact! "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1845_112_2_18_2_47_1_108,(uint8*)"");
            }
#else
		if( SYNC_TYPE_S2WAY == syncway ) //two way slow sync
		{
                	hs_smlLibMemset(pb_entry, 0, sizeof(MMIPB_CONTACT_T)); // chenxiang 20080202
			pb_entry->contact_id= i;  // chenxiang 20080202
                    if (MMIPB_ERROR_SUCCESS==MMIAPIPB_GetContactInfo(pb_entry->contact_id,MMIAPIPB_GetStorageId(MN_DUAL_SYS_1, PHONEBOOK_NV_STORAGE),(MMIPB_CONTACT_T *)&pb_entry))
			{
				retval = TRUE;  //������в�Ϊ�յ�����
			}
                    else //add by wangxiaolin
                    {
                        retval = FALSE;
                    }
		}
		else
		{  //two way (fast) sync
			if (PB_RECENT_ADD== MMIPB_GetPimStatus(i))
			{
                       retval = TRUE;
                     }
                     else
                     {
                        retval = FALSE;
                     }
		}
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
		if (retval)
		{   
                    //add by wangxiaolin 
                    pMe->g_sync_result.send_num++;
                    
                    //��ȡ�绰����¼����ת��vcard��ʽ                  
                   data_lenth= MMIPIM_GetPBVcardItem(i , buffer, buffersize);
                    
                   total_data_len+=data_lenth;
            #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
                   //�ж��Ƿ��Ǵ����
                   if(data_lenth>PIM_MAX_VCARD_DATA_LEN)
                   {
                       //����󵥶����ͣ������б����ʽ
                       if(itemList!=NULL)
   
                       {
                           pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
                           //�������ε��б�����������´�
                           break;
                       }
                       else
                       {
                            itemList=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItemList_t));
                            if(NULL==itemList)
                            {
                                break;
                            }
                            hs_smlLibMemset(itemList, 0, (long)sizeof(SmlItemList_t));

                            item=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItem_t));
                            if(NULL==item)
                            {
                                hs_smlLibFree(itemList);
                                break;
                            }
                            hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));

                            source=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlSource_t));
                            if(NULL==source)
                            {
                                hs_smlLibFree(itemList);
                                hs_smlLibFree(item);
                                break;
                            }
                            
                            hs_smlLibMemset(source, 0, (long)sizeof(SmlSource_t));

                            itemList->next=tmptr;
                            tmptr=itemList;

                            itemList->item=item;

                            item->source=source;
                            temp = hs_smlunsignedInt2String(__FILE__, __LINE__, i); // chenxiang_memory 20080312
                            source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, temp);    // chenxiang_memory 20080312
                            hs_smlLibFree(temp); // chenxiang_memory 20080312
                            source->locName=NULL;

                            item->meta = NULL;
                            item->flags=0x0400;
                            item->data=hs_smlString2Pcdata(__FILE__, __LINE__, (char*)buffer);
                            //syncml_message:"MMIPIM hs_myGetAddedPBItems,content_len=%d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_1935_112_2_18_2_47_2_109,(uint8*)"d",item->data->length);
                            hs_myhttplogs((char *)(buffer),20480);
                            hs_smlLibMemset(buffer, 0, buffersize);

                            pMe->g_sync_result.current_num_for_add=i;
                            pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;

                            //�������ε��б�����������´�
                            break;
                       }
                }
                else//��ӱ��ڵ�
         #endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
                {
                    itemList=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItemList_t));
                    if(NULL==itemList)
                    {
                        break;
                    }
                    hs_smlLibMemset(itemList, 0, (long)sizeof(SmlItemList_t));

                    item=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItem_t));
                    if(NULL==item)
                    {
                        hs_smlLibFree(itemList);
                        break;
                    }
                    hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));

                    source=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlSource_t));
                    if(NULL==source)
                    {
                        hs_smlLibFree(itemList);
                        hs_smlLibFree(item);
                        break;
                    }
                    hs_smlLibMemset(source, 0, (long)sizeof(SmlSource_t));

                    itemList->next=tmptr;
                    tmptr=itemList;

                    itemList->item=item;

                    item->source=source;
                    temp = hs_smlunsignedInt2String(__FILE__, __LINE__, i); // chenxiang_memory 20080312
                    source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, temp);    // chenxiang_memory 20080312
                    hs_smlLibFree(temp); // chenxiang_memory 20080312
                    source->locName=NULL;

                    item->meta = NULL;
                    item->data=hs_smlString2Pcdata(__FILE__, __LINE__, (char*)buffer);
                    hs_smlLibMemset(buffer, 0, buffersize);
              }
           }
           pMe->g_sync_result.current_num_for_add=i;
          
           //�ֶ��message���ͣ�ÿ�η���20����¼,>0�Ǳ�֤û�м�¼ʱ��������ѭ����
           //�������������󣬲Ż�����������־
           if(((pMe->g_sync_result.send_num>0)
                &&((pMe->g_sync_result.send_num-pMe->g_sync_result.send_num_bak)%MAX_NUM_IN_ONE_MSG_BUFFER==0)
                &&(pMe->g_sync_result.send_num!=pMe->g_sync_result.send_num_bak))
                ||(total_data_len>PIM_MAX_VCARD_DATA_LEN))
           {
                    pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
              	break;
           }
	}
      //syncml_cb_message:"@@MMIPIM send_num=%d,current_num_for_add=%d "
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2002_112_2_18_2_47_2_110,(uint8*)"dd",pMe->g_sync_result.send_num,i);
      if(MMIPIM_MAX_PHONEBOOK_RECORDS<=i)
      {
        //syncml_cb_message:"@@MMIPIM current_num_for_add=MMIPIM_MAX_PHONEBOOK_RECORDS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2005_112_2_18_2_47_2_111,(uint8*)"");
        pMe->g_sync_result.add_is_final=TRUE;
      }
	hs_smlLibFree(buffer);        
	SCI_FREE(pb_entry);
	buffer=NULL;
	return itemList;
}



/******************************************************************* 
FUNCTION ������:hs_myGetReplacedPBItems
�����������õ�δͬ���ĸ��ĵĵ绰����Ŀ,����
**                        itemlist�����Ա�����Replace����ʹ�á��ж�
**                        �Ƿ�����ͬ������ͬ����״̬�£�������
**                        �ĵ绰����Ŀ����replace����
**����:TRUE:��ͬ����FALSE:��ͬ��
** ��  ��: itemlist�����һ��ָ�롣
** ��  ��:����
** ��  ��:2006-8-2
** �汾: ver 1.0
**ע��:�˴���̬�����ڴ棬�������ĵ�����syncml_ReplaceCmd
**             ���ͷš�
****************************************************************/
SmlItemListPtr_t hs_myGetReplacedPBItems(void) /* chenxiang 20071124 */
{
    uint16 i=0;

    char*  buffer=NULL;
    char*  temp=NULL; // chenxiang_memory 20080312
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
   uint32 buffersize=MAX_DATA_BUF_LEN;
   int data_lenth=0;
#else
   uint32 buffersize=VCARD_LEN_EXCLUDE_PHOTO_SEGMENT;
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
    int total_data_len=0;
    SmlItemListPtr_t itemList=NULL;
    SmlItemListPtr_t tmptr=NULL;
    SmlItemPtr_t item=NULL;
    SmlSourcePtr_t source=NULL;
    PIM_T_P  pMe=MMIPIM_GetPIM();
    buffer=hs_smlLibMalloc(__FILE__, __LINE__, (long)buffersize);
    if(NULL==buffer)
    {
        return NULL;
    }
    
    hs_smlLibMemset(buffer, 0, buffersize);
    
    //syncml_efs_message:"MMIPIM &&& enter hs_myGetReplacedPBItems "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2055_112_2_18_2_47_2_112,(uint8*)"");
    
    for( i =(uint16)( pMe->g_sync_result.current_num_for_replace+1); i <= MMIPIM_MAX_PHONEBOOK_RECORDS; i++ )
    {
/*Start of  wangxiaolin 2009.8.25 for �Ż����� */
      if( pMe->g_sync_result.replaced_num_for_server==pMe->g_sync_result.send_num -pMe->g_sync_result.deleted_num_for_server)
      {
            //syncml_cb_message:"^@@^MMIPIM hs_myGetReplacedPBItems ,replace_is_final,without check all items!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2061_112_2_18_2_47_2_113,(uint8*)"");
             pMe->g_sync_result.replace_is_final=TRUE;
             break;
      }
/*End   of  wangxiaolin 2009.8.25 for �Ż����� */
        
      if(PB_RECENT_REPLACE == MMIPB_GetPimStatus(i)) // chenxiang 20071124
      {
          pMe->g_sync_result.send_num++;
          //��ȡ�绰����¼����ת��vcard��ʽ                  
         data_lenth= MMIPIM_GetPBVcardItem(i , buffer, buffersize);
         total_data_len+=data_lenth;
         
#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //wangxiaolin  2009.8.6  for
         //�ж��Ƿ��Ǵ����
         if(data_lenth>PIM_MAX_VCARD_DATA_LEN)
         {
             //����󵥶����ͣ������б����ʽ
             if(itemList!=NULL)
  
             {
                 pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
                 //�������ε��б�����������´�
                 break;
             }
             else
             {
                  itemList=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItemList_t));
                  if(NULL==itemList)
                  {
                      break;
                  }
                  hs_smlLibMemset(itemList, 0, (long)sizeof(SmlItemList_t));
  
                  item=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItem_t));
                  if(NULL==item)
                  {
                      hs_smlLibFree(itemList);
                      break;
                  }
                  hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));
  
                  source=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlSource_t));
                  if(NULL==source)
                  {
                      hs_smlLibFree(itemList);
                      hs_smlLibFree(item);
                      break;
                  }
                  hs_smlLibMemset(source, 0, (long)sizeof(SmlSource_t));
  
                  itemList->next=tmptr;
                  tmptr=itemList;
  
                  itemList->item=item;
  
                  item->source=source;
                  temp = hs_smlunsignedInt2String(__FILE__, __LINE__, i); // chenxiang_memory 20080312
                  source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, temp);    // chenxiang_memory 20080312
                  hs_smlLibFree(temp); // chenxiang_memory 20080312
                  source->locName=NULL;
  
                  item->meta = NULL;
                  item->flags=0x0400;//0x0400, MoreData flag
                  item->data=hs_smlString2Pcdata(__FILE__, __LINE__, (char*)buffer);
                  //syncml_message:"MMIPIM hs_myGetAddedPBItems,content_len=%d"
                  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2125_112_2_18_2_47_2_114,(uint8*)"d",item->data->length);
                  //hs_myhttplogs((char *)(buffer),20480);
                  hs_smlLibMemset(buffer, 0, buffersize);
  
                  pMe->g_sync_result.current_num_for_replace=i;
                  pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
  
                  //�������ε��б�����������´�
                  break;
             }
      }
      else//��ӱ��ڵ�
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
       {
             itemList=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItemList_t));
             if(NULL==itemList)
              {
                  break;
              }
              hs_smlLibMemset(itemList, 0, (long)sizeof(SmlItemList_t));
              
              item=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItem_t));
              if(NULL==item)
              {
                  hs_smlLibFree(itemList);
                  break;
              }
              hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));
              
              source=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlSource_t));
              if(NULL==source)
              {
                  hs_smlLibFree(itemList);
                  hs_smlLibFree(item);
                  break;
              }
              hs_smlLibMemset(source, 0, (long)sizeof(SmlSource_t));

              itemList->next=tmptr;
              tmptr=itemList;

              itemList->item=item;
              
              item->source=source;
              temp = hs_smlunsignedInt2String(__FILE__, __LINE__, i); // chenxiang_memory 20080312
              source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, temp);   // chenxiang_memory 20080312
              hs_smlLibFree(temp); // chenxiang_memory 20080312
              source->locName=NULL;

              item->meta = NULL;
       //       syncml_efs_message(("MMIPIM &&& replaced item,content=%s",buffer));
              item->data=hs_smlString2Pcdata(__FILE__, __LINE__, (char*)buffer);
              hs_smlLibMemset(buffer, 0, buffersize);
          }
      }
       pMe->g_sync_result.current_num_for_replace=i;
       if(((pMe->g_sync_result.send_num>0)
            &&((pMe->g_sync_result.send_num-pMe->g_sync_result.send_num_bak)%MAX_NUM_IN_ONE_MSG_BUFFER==0)
            &&(pMe->g_sync_result.send_num!=pMe->g_sync_result.send_num_bak))
              ||(total_data_len>PIM_MAX_VCARD_DATA_LEN))
       {
	   	pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
             break;
       }

    }
     if(MMIPIM_MAX_PHONEBOOK_RECORDS<=i)
    {
        pMe->g_sync_result.replace_is_final=TRUE;
       //���ñ�־������׼����������ӵ�����
     #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for �����
         hs_syncml_set_large_object_proc_flag(FALSE);
     #endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */
    }
    hs_smlLibFree(buffer);
    buffer=NULL;
    return itemList;
}


/******************************************************************* 
FUNCTION ������:hs_myGetDeletedPBItems
�����������õ�δͬ�����Ѿ�ɾ���ĵ绰����Ŀ,����
**                        itemlist�����Ա�����Delete����ʹ�á�
** ��  ��: itemlist�����һ��ָ�롣
** ��  ��:����
** ��  ��:2006-8-2
** �汾: ver 1.0
**ע��:�˴���̬�����ڴ棬�������ĵ�����syncml_DeleteCmd
**             ���ͷš�
****************************************************************/
SmlItemListPtr_t hs_myGetDeletedPBItems(void)
{
    uint16 i=0;
    char*  temp; // chenxiang_memory 20080312
    SmlItemListPtr_t tmptr=NULL;
    SmlItemListPtr_t itemList=NULL;
    SmlItemPtr_t item=NULL;
    SmlSourcePtr_t source=NULL;
    PIM_T_P  pMe=MMIPIM_GetPIM();
    for( i =(uint16)( pMe->g_sync_result.current_num_for_delete+1); i <=MMIPIM_MAX_PHONEBOOK_RECORDS; i++ )
    {
/*Start of  wangxiaolin 2009.8.25 for �Ż����� */
      if( pMe->g_sync_result.deleted_num_for_server==pMe->g_sync_result.send_num)
      {
            //syncml_cb_message:"^@@^MMIPIM hs_myGetDeletedPBItems ,delete_is_final,without check all items!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2228_112_2_18_2_47_2_115,(uint8*)"");
             pMe->g_sync_result.delete_is_final=TRUE;
             break;
      }
/*End   of  wangxiaolin 2009.8.25 for �Ż����� */
      if (PB_RECENT_DELETE== MMIPB_GetPimStatus(i))
      {
          //add by wangxiaolin
          pMe->g_sync_result.send_num++;
	   //pMe->m_recent_status_count.recent_delete_count++;
          itemList=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItemList_t));
          if(NULL==itemList)
          {
              break;
          }
          hs_smlLibMemset(itemList, 0, (long)sizeof(SmlItemList_t));
          
          item=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItem_t));
          if(NULL==item)
          {
              hs_smlLibFree(itemList);
              break;
          }
          hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));
          
          source=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlSource_t));
          if(NULL==source)
          {
              hs_smlLibFree(itemList);
              hs_smlLibFree(item);
              break;
          }
          hs_smlLibMemset(source, 0, (long)sizeof(SmlSource_t));

          itemList->next=tmptr;
          tmptr=itemList;

          itemList->item=item;
          
          item->source=source;
          temp = hs_smlunsignedInt2String(__FILE__, __LINE__, i); // chenxiang_memory 20080312
          source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, temp); // chenxiang_memory 20080312
          hs_smlLibFree(temp); // chenxiang_memory 20080312
          source->locName=NULL;

          item->meta = NULL;

      	}
       pMe->g_sync_result.current_num_for_delete=i;
    //   syncml_cb_message(("@@MMIPIM send_num=%d,current_num_for_delete=%d ",pMe->g_sync_result.send_num,i));
       if((pMe->g_sync_result.send_num>0)&&(pMe->g_sync_result.send_num%(2*MAX_NUM_IN_ONE_MSG_BUFFER)==0)&&(pMe->g_sync_result.send_num>0)&&(pMe->g_sync_result.send_num!=pMe->g_sync_result.send_num_bak))
       {     
             pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
       	break;
       }

    }
    if(MMIPIM_MAX_PHONEBOOK_RECORDS<=i)
    {
         pMe->g_sync_result.delete_is_final=TRUE;
        //syncml_cb_message:"^@@^MMIPIM pMe->g_sync_result.delete_is_final=TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2287_112_2_18_2_47_2_116,(uint8*)"");
    }
    return itemList;
}
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
/*==========================================================
 * author    : wangzhefeng      
 * function  : hs_myGetAddedVCALItems
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
SmlItemListPtr_t hs_myGetAddedVCALItems(short syncway) /* chenxiang 20071026 */
{

    uint16 i=0;
    boolean retval=FALSE; // chenxiang 20071124
    char*  buffer=PNULL;
    char*  temp=PNULL; // chenxiang_memory 20080312
    int data_lenth=0;
    uint32 buffersize=VCARD_LEN_EXCLUDE_PHOTO_SEGMENT;
    int total_data_len=0;
    SmlItemListPtr_t tmptr=NULL;
    SmlItemListPtr_t itemList=NULL;
    SmlItemPtr_t item=NULL;
    SmlSourcePtr_t source=NULL;
    PIM_T_P  pMe=MMIPIM_GetPIM();

    buffer=hs_smlLibMalloc(__FILE__, __LINE__, (long)buffersize);

    //syncml_cb_message:"^@@^MMIPIM hs_myGetAddedVCALItems begin!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2318_112_2_18_2_47_2_117,(uint8*)"");
    if(buffer==NULL)
    {
        //syncml_cb_message:"^@@^MMIPIM hs_myGetAddedVCALItems,buffer == NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2321_112_2_18_2_47_2_118,(uint8*)"");
        return NULL;
    }


    hs_smlLibMemset(buffer, 0, buffersize);

    //wangxiaolin 20071210
    for( i =(uint16)( pMe->g_sync_result.current_num_for_add+1); i <=MMIPIM_MAX_VCAL_NUM; i++ )
    {
        /*Start of  wangxiaolin 2009.8.25 for �Ż����� */
        if( pMe->g_sync_result.added_num_for_server==pMe->g_sync_result.send_num -pMe->g_sync_result.deleted_num_for_server-pMe->g_sync_result.replaced_num_for_server)
        {
            //syncml_cb_message:"^@@^MMIPIM hs_myGetAddedVCALItems ,add_is_final,without check all items!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2334_112_2_18_2_47_2_119,(uint8*)"");
            pMe->g_sync_result.add_is_final=TRUE;
            break;
        }
        /*End   of  wangxiaolin 2009.8.25 for �Ż����� */

#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.9 for ����ͬ��&  ˢ��ͬ��
        if((SYNC_TYPE_S2WAY == syncway )||( SYNC_TYPE_RFCLI== syncway )) //two way slow sync
        {
            if ( MMIPIM_IsScheItemValid(i))
            {
                retval = TRUE;  //������в�Ϊ�յ�����
            }
            else
            {
                retval = FALSE;
            }
        }
        else if((SYNC_TYPE_2WAY == syncway )||( SYNC_TYPE_1WAYFCLI== syncway ))
        {  //two way (fast) sync
            if (VCAL_RECENT_ADD == MMIVCAL_GetPimStatus(i))
            {
                retval = TRUE;
            }
            else
            {
                retval = FALSE;
            }
        }
        else
        {
            //syncml_message:"MMIPIM hs_myGetAddedPBItems,no need updata local contact! "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2365_112_2_18_2_47_3_120,(uint8*)"");
        }
#else
        if( SYNC_TYPE_S2WAY == syncway ) //two way slow sync
        {
            if ( MMIPIM_IsScheItemValid(i))
            {
                retval = TRUE;  //������в�Ϊ�յ�����
            }
            else //add by wangxiaolin
            {
                retval = FALSE;
            }
        }
        else
        {  //two way (fast) sync
            if (VCAL_RECENT_ADD == MMIVCAL_GetPimStatus(i))
            {
                retval = TRUE;
            }
            else
            {
                retval = FALSE;
            }
        }
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
        if (retval)
        {   
            //add by wangxiaolin 
            pMe->g_sync_result.send_num++;

            //��ȡ�ճ̣���ת��vcalendar��ʽ                  
            data_lenth= MMIPIM_GetVcalItem(i , buffer, buffersize);
            //syncml_message:"MMIPIM hs_myGetAddedVCALItems,change vcalendar finished!data_lenth = %d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2398_112_2_18_2_47_3_121,(uint8*)"d",data_lenth);
            if(0 == data_lenth)
            {
                break;
            }

            total_data_len+=data_lenth;
            itemList=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItemList_t));
            if(NULL==itemList)
            {
                break;
            }
            hs_smlLibMemset(itemList, 0, (long)sizeof(SmlItemList_t));

            item=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItem_t));
            if(NULL==item)
            {
                hs_smlLibFree(itemList);
                break;
            }
            hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));

            source=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlSource_t));
            if(NULL==source)
            {
                hs_smlLibFree(itemList);
                hs_smlLibFree(item);
                break;
            }
            hs_smlLibMemset(source, 0, (long)sizeof(SmlSource_t));

            itemList->next=tmptr;
            tmptr=itemList;

            itemList->item=item;

            item->source=source;
            temp = hs_smlunsignedInt2String(__FILE__, __LINE__, i); // chenxiang_memory 20080312
            source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, temp);    // chenxiang_memory 20080312
            hs_smlLibFree(temp); // chenxiang_memory 20080312
            source->locName=NULL;

            item->meta = NULL;
            item->data=hs_smlString2Pcdata(__FILE__, __LINE__, (char*)buffer);
            hs_smlLibMemset(buffer, 0, buffersize);
        }
        pMe->g_sync_result.current_num_for_add=i;

        //�ֶ��message���ͣ�ÿ�η���20����¼,>0�Ǳ�֤û�м�¼ʱ��������ѭ����
        //�������������󣬲Ż�����������־
        if(((pMe->g_sync_result.send_num>0)
        &&(pMe->g_sync_result.send_num%MAX_NUM_IN_ONE_MSG_BUFFER==0)
        &&(pMe->g_sync_result.send_num!=pMe->g_sync_result.send_num_bak))
        ||(total_data_len>PIM_MAX_VCARD_DATA_LEN))
        {
            pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
            break;
        }
    }
    //syncml_cb_message:"@@MMIPIM send_num=%d,current_num_for_add=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2457_112_2_18_2_47_3_122,(uint8*)"dd",pMe->g_sync_result.send_num,i);
    if(MMIPIM_MAX_VCAL_NUM<=i)
    {
        //syncml_cb_message:"@@MMIPIM current_num_for_add=MMIPIM_MAX_VCAL_NUM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2460_112_2_18_2_47_3_123,(uint8*)"");
        pMe->g_sync_result.add_is_final=TRUE;
    }
    hs_smlLibFree(buffer);
    buffer=NULL;
    return itemList;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : hs_myGetReplacedVCALItems
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
SmlItemListPtr_t hs_myGetReplacedVCALItems(void) /* chenxiang 20071124 */
{
    uint16 i=0;

    char*  buffer=NULL;
    char*  temp=NULL; // chenxiang_memory 20080312
   int data_lenth=0;
   uint32 buffersize=VCARD_LEN_EXCLUDE_PHOTO_SEGMENT;
    int total_data_len=0;
    SmlItemListPtr_t itemList=NULL;
    SmlItemListPtr_t tmptr=NULL;
    SmlItemPtr_t item=NULL;
    SmlSourcePtr_t source=NULL;
    PIM_T_P  pMe=MMIPIM_GetPIM();
    buffer=hs_smlLibMalloc(__FILE__, __LINE__, (long)buffersize);
    if(NULL==buffer)
    {
        return NULL;
    }
    
    hs_smlLibMemset(buffer, 0, buffersize);
    
    //syncml_efs_message:"MMIPIM &&& enter hs_myGetReplacedPBItems "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2498_112_2_18_2_47_3_124,(uint8*)"");
    
    for( i =(uint16)( pMe->g_sync_result.current_num_for_replace+1); i <= MMIPIM_MAX_VCAL_NUM; i++ )
    {
/*Start of  wangxiaolin 2009.8.25 for �Ż����� */
      if( pMe->g_sync_result.replaced_num_for_server==pMe->g_sync_result.send_num -pMe->g_sync_result.deleted_num_for_server)
      {
            //syncml_cb_message:"^@@^MMIPIM hs_myGetReplacedPBItems ,replace_is_final,without check all items!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2504_112_2_18_2_47_3_125,(uint8*)"");
             pMe->g_sync_result.replace_is_final=TRUE;
             break;
      }
/*End   of  wangxiaolin 2009.8.25 for �Ż����� */
        
      if(VCAL_RECENT_REPLACE == MMIVCAL_GetPimStatus(i)) // chenxiang 20071124
      {
          pMe->g_sync_result.send_num++;
          //��ȡ�ճ̣���ת��vcalendar��ʽ                  
         data_lenth= MMIPIM_GetVcalItem(i , buffer, buffersize);
        //syncml_message:"MMIPIM hs_myGetReplacedVCALItems,change vcalendar finished!data_lenth = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2514_112_2_18_2_47_3_126,(uint8*)"d",data_lenth);
             if(0 == data_lenth)
            {
                break;
            }
             total_data_len+=data_lenth;
         
             itemList=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItemList_t));
             if(NULL==itemList)
              {
                  break;
              }
              hs_smlLibMemset(itemList, 0, (long)sizeof(SmlItemList_t));
              
              item=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItem_t));
              if(NULL==item)
              {
                  hs_smlLibFree(itemList);
                  break;
              }
              hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));
              
              source=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlSource_t));
              if(NULL==source)
              {
                  hs_smlLibFree(itemList);
                  hs_smlLibFree(item);
                  break;
              }
              hs_smlLibMemset(source, 0, (long)sizeof(SmlSource_t));

              itemList->next=tmptr;
              tmptr=itemList;

              itemList->item=item;
              
              item->source=source;
              temp = hs_smlunsignedInt2String(__FILE__, __LINE__, i); // chenxiang_memory 20080312
              source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, temp);   // chenxiang_memory 20080312
              hs_smlLibFree(temp); // chenxiang_memory 20080312
              source->locName=NULL;

              item->meta = NULL;
       //       syncml_efs_message(("MMIPIM &&& replaced item,content=%s",buffer));
              item->data=hs_smlString2Pcdata(__FILE__, __LINE__, (char*)buffer);
              hs_smlLibMemset(buffer, 0, buffersize);
      }
       pMe->g_sync_result.current_num_for_replace=i;
       if(((pMe->g_sync_result.send_num>0)
            &&(pMe->g_sync_result.send_num%MAX_NUM_IN_ONE_MSG_BUFFER==0)
            &&(pMe->g_sync_result.send_num!=pMe->g_sync_result.send_num_bak))
              ||(total_data_len>PIM_MAX_VCARD_DATA_LEN))
       {
	   	pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
             break;
       }

    }
     if(MMIPIM_MAX_VCAL_NUM<=i)
    {
        pMe->g_sync_result.replace_is_final=TRUE;
    }
    hs_smlLibFree(buffer);
    buffer=NULL;
    return itemList;
}

/*==========================================================
 * author    : wangzhefeng      
 * function  : hs_myGetDeletedVCALItems
 * decr      : 
 * parameter : 
 * return    : 
 * effect    : NO
 * history   :created  9/11/2009 
 ==========================================================*/
SmlItemListPtr_t hs_myGetDeletedVCALItems(void)
{
    uint16 i=0;
    char*  temp; // chenxiang_memory 20080312
    SmlItemListPtr_t tmptr=NULL;
    SmlItemListPtr_t itemList=NULL;
    SmlItemPtr_t item=NULL;
    SmlSourcePtr_t source=NULL;
    PIM_T_P  pMe=MMIPIM_GetPIM();
    
    for( i =(uint16)( pMe->g_sync_result.current_num_for_delete+1); i <=MMIPIM_MAX_VCAL_NUM; i++ )
    {
/*Start of  wangxiaolin 2009.8.25 for �Ż����� */
      if( pMe->g_sync_result.deleted_num_for_server==pMe->g_sync_result.send_num)
      {
            //syncml_cb_message:"^@@^MMIPIM hs_myGetDeletedVCALItems ,delete_is_final,without check all items!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2603_112_2_18_2_47_3_127,(uint8*)"");
             pMe->g_sync_result.delete_is_final=TRUE;
             break;
      }
/*End   of  wangxiaolin 2009.8.25 for �Ż����� */
      if (VCAL_RECENT_DELETE == MMIVCAL_GetPimStatus(i))
      {
          //add by wangxiaolin
          pMe->g_sync_result.send_num++;
	   //pMe->m_recent_status_count.recent_delete_count++;
          itemList=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItemList_t));
          if(NULL==itemList)
          {
              break;
          }
          hs_smlLibMemset(itemList, 0, (long)sizeof(SmlItemList_t));
          
          item=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlItem_t));
          if(NULL==item)
          {
              hs_smlLibFree(itemList);
              break;
          }
          hs_smlLibMemset(item, 0, (long)sizeof(SmlItem_t));
          
          source=hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlSource_t));
          if(NULL==source)
          {
              hs_smlLibFree(itemList);
              hs_smlLibFree(item);
              break;
          }
          hs_smlLibMemset(source, 0, (long)sizeof(SmlSource_t));

          itemList->next=tmptr;
          tmptr=itemList;

          itemList->item=item;
          
          item->source=source;
          temp = hs_smlunsignedInt2String(__FILE__, __LINE__, i); // chenxiang_memory 20080312
          source->locURI=hs_smlString2Pcdata(__FILE__, __LINE__, temp); // chenxiang_memory 20080312
          hs_smlLibFree(temp); // chenxiang_memory 20080312
          source->locName=NULL;

          item->meta = NULL;

      	}
       pMe->g_sync_result.current_num_for_delete=i;
    //   syncml_cb_message(("@@MMIPIM send_num=%d,current_num_for_delete=%d ",pMe->g_sync_result.send_num,i));
       if((pMe->g_sync_result.send_num>0)&&(pMe->g_sync_result.send_num%(2*MAX_NUM_IN_ONE_MSG_BUFFER)==0)&&(pMe->g_sync_result.send_num>0)&&(pMe->g_sync_result.send_num!=pMe->g_sync_result.send_num_bak))
       {     
             pMe->g_sync_result.send_num_bak=pMe->g_sync_result.send_num;
       	break;
       }

    }
    if(MMIPIM_MAX_VCAL_NUM<=i)
    {
         pMe->g_sync_result.delete_is_final=TRUE;
        //syncml_cb_message:"^@@^MMIPIM pMe->g_sync_result.delete_is_final=TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2662_112_2_18_2_47_3_128,(uint8*)"");
    }
    return itemList;
}

#endif /* HS_PIM_VCAL */


/******************************************************************* 
FUNCTION ������:hs_myGetAllPBItemsCount
�����������õ�δͬ�����¼ӵĵ绰����Ŀ����
** ��  ��: unsigned int
** ��  ��:������
** ��  ��:2007.12.06
** �汾: ver 1.0
****************************************************************/
unsigned int hs_myGetAllPBItemsCount(void)
{

	unsigned int num=0;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    	num = MMIPB_GetAllPBItemsCount();
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    }
    else
    {
        num = MMIVCAL_GetAllVCALItemsCount();
    }
#endif /* HS_PIM_VCAL */
	return num;
}


#ifdef HS_PIM_LOG_FILE
/*==========================================================
 * author		: chenxiang      
 * function 	: hs_myPrintLogFiles
 * decr		: ��xml �ļ�д��洢��
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/5/2009 
 ==========================================================*/
static void hs_myPrintLogFiles(unsigned int sessionID, unsigned int messageID, char* filename, char* buffer, uint32  length)
{
    MMIFILE_HANDLE	file_handle = PNULL;
    uint32 opt_mode=0;
    uint32 charswrite = 0;
    uint16 path_filename_len = 0;
    uint16  path_filename[100] = {0};
    uint16 i = 0;
    uint16 offset = 0;
    uint32 free_space_high = 0;
    uint32 free_space_low = 0;
    
    // D:\PimLog\  �ļ���·��
    uint16 path_name[] = {0x0044, 0x003A, 0x005C, 0x0050, 0x0069, 0x006D, 0x004C, 0x006F, 0x0067, 0x005C, 0x00, 0x00};

    path_name[0] = *MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK);
    hs_logFileCount++;
    //syncml_cb_message:"MMIPIM ----hs_myPrintLogFiles----%d.xml----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2723_112_2_18_2_47_3_129,(uint8*)"d", hs_logFileCount);
    
/*Start of  wangzhefeng 2009.10.30 for �����������log�洢1000��ʱд�ļ��������� */
    if(1000==hs_logFileCount)
    {
        hs_logFileCount = 0;
    }
/*End   of  wangzhefeng 2009.10.30 for �����������log�洢1000��ʱд�ļ��������� */
    
    if (!MMIAPIFMM_IsFolderExist((uint16 *)path_name, 10))
    {
        // �����ļ���
        MMIAPIFMM_CreateDir((uint16 *)path_name, 10);
    }

    // ��ȡ�µ�xml �ļ���
    hs_smlLibMemcpy(path_filename, path_name, 20);
    
    offset = 10;
    
    path_filename[offset] =(uint16)( 0x30 + (hs_logFileCount / 100));
    offset+=1;
    path_filename[offset] = (uint16)(0x30 + ((hs_logFileCount % 100) / 10));
    offset+=1;
    path_filename[offset] = (uint16)(0x30 + (hs_logFileCount % 10));
    offset+=1;
    path_filename[offset] = 0x5F; // '_'
    offset+=1;

    path_filename[offset] = (uint16)(0x30 + (sessionID / 100));
    offset+=1;
    path_filename[offset] = (uint16)(0x30 + ((sessionID % 100) / 10));
    offset+=1;
    path_filename[offset] = (uint16)(0x30 + (sessionID % 10));
    offset+=1;
    path_filename[offset] = 0x5F; // '_'
    offset+=1;
    
    path_filename[offset] = (uint16)(0x30 + (messageID / 100));
    offset+=1;
    path_filename[offset] =(uint16)(0x30 + ((messageID % 100) / 10));
    offset+=1;
    path_filename[offset] = (uint16)(0x30 + (messageID % 10));
    offset+=1;
    path_filename[offset] = 0x5F; // '_'
    offset+=1;
    
    for(i = 0; i < hs_smlLibStrlen(filename); i++)
    {
        path_filename[offset] = filename[i];
        offset+=1;
    }

    path_filename[offset] = 0x2E; // '.'
    offset+=1;
    path_filename[offset] = 0x78; // 'x'
    offset+=1;
    path_filename[offset] = 0x6D; // 'm'
    offset+=1;
    path_filename[offset] = 0x6C; // 'l'
    
   // path_filename_len = (offset + 1) / 2;
    
    if (MMIAPIFMM_IsFileExist((uint16 *)path_filename, path_filename_len))
    {
        MMIAPIFMM_DeleteFile((uint16 *)path_filename, NULL);
    }

    MMIAPIFMM_GetDeviceFreeSpace((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK),MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK),&free_space_high, &free_space_low);
    
    if(length < free_space_low || 0 != free_space_high)
    {
        opt_mode = SFS_MODE_CREATE_NEW | SFS_MODE_WRITE;/*lint !e655*/
        file_handle=   MMIAPIFMM_CreateFile((uint16 *)path_filename, 
        						opt_mode, 
        						PNULL, 
        						PNULL
        						);
        MMIAPIFMM_WriteFile(file_handle, buffer, length, &charswrite, NULL);
        MMIAPIFMM_CloseFile(file_handle);
    }
}


#endif



/******************************************************************* 
FUNCTION ������:hs_myGetPhoneBookFreeIDs
�����������õ���ǰ���ֻ�ʣ��ĵ绰���ɴ洢��Ŀ
** ��  ��: malloc ��һ��Smlpcdataptr�����ݣ�����������syncml
                  �ĵ�����syncml core�ͷš�
** ��  ��:����
** ��  ��:2006-8-28
** �汾: ver 1.0
**ע��:��̬������ڴ棬��������syncml
                  �ĵ�����syncml core�ͷš�
****************************************************************/
SmlPcdataPtr_t hs_myGetPhoneBookFreeIDs(void)
{
   SmlPcdataPtr_t pcdata;
   unsigned int id_count;
   char* idfree=NULL;
 //  id_count=(unsigned int)(efs_cache.num_of_records-efs_cache.records_in_use);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
   id_count = MMIPB_GetFreeItemsCount()+1;
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    }
    else
    {
        id_count = MMIVCAL_GetFreeItemsCount()+1;
    }
#endif /* HS_PIM_VCAL */
   idfree=hs_smlunsignedInt2String(__FILE__, __LINE__, id_count);
   pcdata=hs_smlString2Pcdata(__FILE__, __LINE__, idfree);
   hs_smlLibFree(idfree);
   return pcdata;
}


short hs_myHandleAdd(short id, void* userData, SmlAddPtr_t pContent) /* chenxiang */
{

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
        syncml_cb_message(("MMIPIM @@hs_myHandleAdd@@ MMIPIM_PB_Add start!"));//wangzhefeng add 20090915
    return (MMIPIM_PB_Add(id, userData, pContent));
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    }
    else
    {
        syncml_cb_message(("MMIPIM @@hs_myHandleAdd@@ MMIPIM_VCAL_Add start!"));//wangzhefeng add 20090915
        return (MMIPIM_VCAL_Add(id, userData, pContent));
    }
#endif /* HS_PIM_VCAL */

}



short hs_myHandledelete(short id, void* userData, SmlDeletePtr_t pContent) /* chenxiang 20071026 */
{
    //syncml_cb_message:"MMIPIM @@handle hs_myHandledelete @@"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2866_112_2_18_2_47_4_130,(uint8*)"");
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    return (MMIPIM_PB_delete(id, userData, pContent));
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    }
    else
    {
        return (MMIPIM_VCAL_delete(id, userData, pContent));
    }
#endif /* HS_PIM_VCAL */
}

/*==========================================================
 * author		: wangxiaolin      
 * function 	: hs_myHandlestatus
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/16/2009 
 ==========================================================*/
short hs_myHandlestatus(short id, void* userData, SmlStatusPtr_t pContent)
{
   SmlStatusPtr_t status;
   int statuscode;
   char* cmd;
   char* itemchar;
   unsigned int rec_id;
   SmlItemListPtr_t templist;
   //uint32 buffersize=1000;
   char* statusdata=NULL;

   status=pContent;
   //syncml_cb_message:"MMIPIM @@handle status @@"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2901_112_2_18_2_47_4_131,(uint8*)"");
   statusdata=hs_smlPcdata2String(__FILE__, __LINE__, status->data);
   statuscode=atoi(statusdata);
   hs_smlLibFree(statusdata);
   //syncml_cb_message:"MMIPIM @@status is %d@@"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2905_112_2_18_2_47_4_132,(uint8*)"d",statuscode);
   cmd=hs_smlPcdata2String(__FILE__, __LINE__, status->cmd);
   //syncml_cb_message:"MMIPIM @@status cmd is %s@@"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2907_112_2_18_2_47_4_133,(uint8*)"s",cmd);
   switch(statuscode)
   {
      //�ɹ��࣬2XX
      case 212:     //�ն˼�Ȩͨ��
      {
          if (0 == hs_task_relay_info->authored)
          {
            hs_task_relay_info->authored = 1;
          }
      }
      break;

      case 213:      //�����ְ�ʱ�����յ�������Ϣ
        //syncml_message:"MMIPIM ** large object,send next block of the object!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2921_112_2_18_2_47_4_134,(uint8*)"");
        break;


      case 215:      //�û�ѡ��ȡ���ò�
      case 216:      //ԭ�Ӳ����ع��ɹ�
      break;
      
      case 222:      //������һ�����ݰ�
      {
         //syncml_http_message:"MMIPIM ** send next message of the package"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2931_112_2_18_2_47_4_135,(uint8*)"");
      }
      break;

     case 211:  //Delete�������������δɾ��
     break;

      case 201:     //�ɹ�
      case 200:     //������ӳɹ�
      case 207:    //�Ժϲ���ʽ�ĳ�ͻ����
      case 208:    //�ն�Ϊ׼�ĳ�ͻ����
      {
         uint16 item_num = 0;
         PIM_T_P  pMe = MMIPIM_GetPIM();
         
         //modify the phonebook status code to SYNCED
         if((0==hs_smlLibStrcmp(cmd, "Add"))||(0==hs_smlLibStrcmp(cmd, "Replace")) || (0==hs_smlLibStrcmp(cmd, "Delete"))) // chenxiang 20080202
         {
            item_num = 0;
            //syncml_cb_message:"MMIPIM @@status cmdref equal to %s@@"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2950_112_2_18_2_47_4_136,(uint8*)"s",cmd);
            templist=status->itemList;
            if(NULL==templist)   //only one item is operated!
            {
                item_num++;
                itemchar=hs_smlPcdata2String(__FILE__, __LINE__, status->sourceRefList->sourceRef);
                rec_id=hs_smlString2unsignedInt(itemchar);
                hs_smlLibFree(itemchar); // chenxiang_memory 20080313
            
                //modify the phone book status code to SYNCED
                // modify by wangxiaolin 2010.1.11
                 if( (((201 == statuscode)||(207 == statuscode)||(208 == statuscode)) && (0 == hs_smlLibStrcmp(cmd, "Add") ))
                      ||(((201 == statuscode)||(200 == statuscode)||(207 == statuscode)||(208 == statuscode)) && (0 == hs_smlLibStrcmp(cmd, "Replace") ))
                      ||((211 != statuscode) && (0 == hs_smlLibStrcmp(cmd, "Delete"))))
                 {
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
                        if(SYNC_MODE_PB == g_pim.m_sync_mode)
                        {
#endif /* HS_PIM_VCAL */
                        MMIPB_UpdatePimStatus((uint16) rec_id, PB_NO_MODIFY );
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
                        }
                        else
                        {
                            MMIVCAL_UpdatePimStatus((uint16) rec_id, VCAL_NO_MODIFY );
                        }
#endif /* HS_PIM_VCAL */
                 }
                //syncml_cb_message:"MMIPIM @@templist is pnull"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2978_112_2_18_2_47_4_137,(uint8*)"");
                //syncml_cb_message:"MMIPIM @@pbm modify the NO.%d item's data to SYNCED ok!!!! @@"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_2979_112_2_18_2_47_4_138,(uint8*)"d",rec_id);
                  
            }
            else
                
            {    //some items are operated!
                 // modify by wangxiaolin 2010.1.11
                 if( (((201 == statuscode)||(207 == statuscode)||(208 == statuscode)) && (0 == hs_smlLibStrcmp(cmd, "Add") ))
                    ||(((201 == statuscode)||(200 == statuscode)||(207 == statuscode)||(208 == statuscode)) && (0 == hs_smlLibStrcmp(cmd, "Replace") ))
                    ||((211 != statuscode) && (0 == hs_smlLibStrcmp(cmd, "Delete"))))
                 {
                      while(NULL!=templist)
                      {
                        item_num++;
                        itemchar=hs_smlPcdata2String(__FILE__, __LINE__, templist->item->source->locURI);
                        rec_id=hs_smlString2unsignedInt(itemchar);
                        hs_smlLibFree(itemchar); // chenxiang_memory 20080313
                        //modify the phone book status code to SYNCED
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
                        if(SYNC_MODE_PB == g_pim.m_sync_mode)
                        {
#endif /* HS_PIM_VCAL */
                         MMIPB_UpdatePimStatus((uint16) rec_id, PB_NO_MODIFY );
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
                        }
                        else
                        {
                            MMIVCAL_UpdatePimStatus((uint16) rec_id, VCAL_NO_MODIFY );
                        }
#endif /* HS_PIM_VCAL */
                            //syncml_cb_message:"MMIPIM @@pbm modify the NO.%d item's data to SYNCED ok!!!! @@"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3009_112_2_18_2_47_4_139,(uint8*)"d",rec_id);
                                     
                            templist=templist->next;
                          
                      }
                  }
                  else  //�������������ݿ�ʧ�ܣ����������ʧ�ܣ�����200�����by wangxiaolin 2010.1.11
                  {
                      //ֻ�ͷ��ڴ棬����Ҫ����״̬
                      while(NULL!=templist)
                      {
                            itemchar=hs_smlPcdata2String(__FILE__, __LINE__, templist->item->source->locURI);
                            rec_id=hs_smlString2unsignedInt(itemchar);
                            hs_smlLibFree(itemchar); 
                            //syncml_cb_message:"MMIPIM @@server process error,canot modify the NO.%d item's data to SYNCED ok!!!! @@"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3023_112_2_18_2_47_4_140,(uint8*)"d",rec_id);
                            templist=templist->next;
                          
                      }
                  }
            }

             if (((201 == statuscode)||(207 == statuscode)||(208 == statuscode)) && (0 == hs_smlLibStrcmp(cmd, "Add") ))
             {
                     pMe->g_sync_result.real_added_num_for_server =(uint16)( pMe->g_sync_result.real_added_num_for_server +item_num);
             }
             else if (((201 == statuscode)||(200 == statuscode)||(207 == statuscode)||(208 == statuscode)) && (0 == hs_smlLibStrcmp(cmd, "Replace") ))
             {
                     pMe->g_sync_result.real_replaced_num_for_server =(uint16)( pMe->g_sync_result.real_replaced_num_for_server +item_num);
             }
             else if ((211 != statuscode) && (0 == hs_smlLibStrcmp(cmd, "Delete")))
             {
                     pMe->g_sync_result.real_deleted_num_for_server = (uint16)(pMe->g_sync_result.real_deleted_num_for_server +item_num);
             }
         }
        //the delete links will be deleted at the end of this session in hs_syncml_Terminate.
        //Here we do nothing.
      }
      break;
      
      //�ض�����3XX
      case 303:
        //   ���������յ���ʼ���ĺ󣬷�����Ҫ�ض�����������������
        //   �򷵻ش�״̬�룬����status��itemԪ���и���������������URL��
        //   �ͻ������URL���·����ʼ�����ġ�
      break;
      

      //�ͻ���״̬��4XX

      case 401://��Ȩʧ��
      {
            //chenxiang_auth 20071215
        if (status->chal) // SyncHdr
        {
              char * status_auth_type = PNULL;
              char * status_auth_nextnonce = PNULL;
              status_auth_type = hs_smlPcdata2String(__FILE__, __LINE__, ((SmlMetInfMetInf_t*)(status->chal->meta->content))->type);
              //syncml_cb_message:"MMIPIM @@status_auth_type is %s@@"
              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3066_112_2_18_2_47_4_141,(uint8*)"s",status_auth_type);
              if (0 == xppStrcmp(status_auth_type, "syncml:auth-md5"))
              {
                  status_auth_nextnonce = hs_smlPcdata2String(__FILE__, __LINE__, ((SmlMetInfMetInf_t*)(status->chal->meta->content))->nextnonce);
                  if (AUTH_B64 == g_pim.m_authtype)
                  {
                      xppMemcpy(hs_task_relay_info->nextnonce, status_auth_nextnonce, xppStrlen(status_auth_nextnonce));
                      hs_task_relay_info->nextnonce[xppStrlen(status_auth_nextnonce)] = 0;
                      g_pim.m_authtype = AUTH_MD5;
                      hs_task_relay_info->authtype = AUTH_MD5;
                      *hs_protocol_step  =  STEP_CLIENT_INIT_SYNC;
                      //syncml_cb_message:"MMIPIM ** hs_myHandlestatus  syncml:auth-md5 AUTH_B64 "
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3077_112_2_18_2_47_4_142,(uint8*)"");
                  }
                  else if (AUTH_MD5 == g_pim.m_authtype)
                  {
                      xppMemcpy(hs_task_relay_info->nextnonce, status_auth_nextnonce, xppStrlen(status_auth_nextnonce));
                      hs_task_relay_info->nextnonce[xppStrlen(status_auth_nextnonce)] = 0;
                      //syncml_cb_message:"MMIPIM ** hs_myHandlestatus  syncml:auth-md5 AUTH_MD5 authenticate failer"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3083_112_2_18_2_47_4_143,(uint8*)"");
                      *hs_protocol_step  =  STEP_SYNC_ERROR;
                      HS_Data_Sync_PostMsgToUI( PIM_MSG_SYNC_FAIL, (DPARAM)&statuscode, sizeof(int));
                  }
                  else
                  {
                      //syncml_cb_message:"MMIPIM ** hs_myHandlestatus syncml:auth-md5 g_pim.m_authtype ??"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3089_112_2_18_2_47_4_144,(uint8*)"");
                      *hs_protocol_step  =  STEP_SYNC_ERROR;
                      HS_Data_Sync_PostMsgToUI( PIM_MSG_SYNC_FAIL, (DPARAM)&statuscode, sizeof(int));
                  }
                  hs_smlLibFree(status_auth_nextnonce);
              }
              else if (0 == xppStrcmp(status_auth_type, "syncml:auth-basic"))
              {
                  if (AUTH_MD5 == g_pim.m_authtype)
                  {
                      g_pim.m_authtype = AUTH_B64;
                      hs_task_relay_info->authtype = AUTH_B64;
                      *hs_protocol_step  =  STEP_CLIENT_INIT_SYNC;
                      //syncml_cb_message:"MMIPIM ** hs_myHandlestatus  syncml:auth-basic AUTH_MD5 "
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3102_112_2_18_2_47_4_145,(uint8*)"");
                  }
                  else if (AUTH_B64 == g_pim.m_authtype)
                  {
                      //syncml_cb_message:"MMIPIM ** hs_myHandlestatus  syncml:auth-basic AUTH_B64 authenticate failer"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3106_112_2_18_2_47_4_146,(uint8*)"");
                      *hs_protocol_step  =  STEP_SYNC_ERROR;
                      HS_Data_Sync_PostMsgToUI( PIM_MSG_SYNC_FAIL, (DPARAM)&statuscode, sizeof(int));
                  }
                  else
                  {
                      //syncml_cb_message:"MMIPIM ** hs_myHandlestatus syncml:auth-basic g_pim.m_authtype ??"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3112_112_2_18_2_47_4_147,(uint8*)"");
                      *hs_protocol_step  =  STEP_SYNC_ERROR;
                      HS_Data_Sync_PostMsgToUI( PIM_MSG_SYNC_FAIL, (DPARAM)&statuscode, sizeof(int));
                  }
              }
              else
              {
                  //syncml_cb_message:"MMIPIM ** hs_myHandlestatus  status_auth_type ??"
                  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3119_112_2_18_2_47_4_148,(uint8*)"");
                   *hs_protocol_step  =  STEP_SYNC_ERROR;
                  HS_Data_Sync_PostMsgToUI( PIM_MSG_SYNC_FAIL, (DPARAM)&statuscode, sizeof(int));
              }
              hs_smlLibFree(status_auth_type);
          }
      }
      break;
      
      //����Ҫ��ʾ�����Լ��������״̬
      case 404:        //�ն�����Ķ���δ�ҵ�,����ʾ����������Ĵ���
      
      case 409:        //��˫�����Խ�����ݳ�ͻ,����ʾ�����������ͬ��
      
      case 418:        //Ŀ�������Ѿ�����,����ʾ���������������
      
      case 419:        //��������Ϊ׼�ĳ�ͻ���,����ʾ�����������ͬ��
      
      break;

      //�쳣״̬����Ҫ�ն�ͬ�����������Ӧ����ʾ
      case 400:       //��������������﷨��䷨�����޷�ִ��,ͬ��ʧ��
      
      case 403:        // ����������ͬ�����ڽ��У����Ժ�����

      case 406:        //ͬ��ʧ�ܣ���֧�ָ�ͬ����ʽ

      case 407:        // �������û���������
        
      case 408:        // �Ự��ʱ�����Ժ�����
      
      case 412:        //ͬ��ʧ�ܣ���������ͬ������
      
      case 420:        // ͬ��ʧ�ܣ�ͨѶ¼��������

        *hs_protocol_step  =  STEP_SYNC_ERROR;
        HS_Data_Sync_PostMsgToUI( PIM_MSG_SYNC_FAIL, (DPARAM)&statuscode, sizeof(int));
      break;

      //��ǰ��֧�ֵ�״̬
      case 423:        //��ɾ������.��ǰ��֧����ɾ��
      
      case 424:        //��Ƭ��С����
      
      break;

      
      //��������״̬��5XX

     case 508:  //server find error for sync way, need slow sync 
      {      
           //syncml_http_message:"MMIPIM ** change the sync type to slow sync. "
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3170_112_2_18_2_47_4_149,(uint8*)"");
           hs_task_relay_info->synctype=SYNC_TYPE_S2WAY;  
      }
      break;

      //�쳣״̬����Ҫ�ն�ͬ�����������Ӧ����ʾ
      case 500:        // ����������δ֪����ͬ��ʧ��
        
      case 501:       //��������֧�ֿͻ������������,ͬ��ʧ��
        
      case 503:       //������æ�����Ժ�����

      case 505:       //SyncML DTD�汾�Ų�֧��,ͬ��ʧ��
      
      case 510:        // ͬ��ʧ�ܣ�����ͬ�����ܵ����ݿ�����

      case 511:       //ͬ��ʧ�ܣ�����������
        
      case 512:        // ͬ��ʧ��(512)

      case 513:        // ͬ��Э�鲻֧��
        *hs_protocol_step  =  STEP_SYNC_ERROR;
        HS_Data_Sync_PostMsgToUI( PIM_MSG_SYNC_FAIL, (DPARAM)&statuscode, sizeof(int));
      break;

      case 514:        // �����Ѿ��ɹ�ȡ��
      break;

      default:  //default
       break;         
   }
   
   hs_smlLibFree(cmd); // chenxiang_memory 20080223
   return hs_smlFreeProtoElement(pContent); // HS_SML_ERR_OK; // chenxiang_memory 20080223
}

short hs_myHandlereplace(short id, void* userData, SmlReplacePtr_t pContent)
{
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == g_pim.m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
	return (MMIPIM_PB_replace(id, userData, pContent));
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    }
    else
    {
        return (MMIPIM_VCAL_replace(id, userData, pContent));
    }
#endif /* HS_PIM_VCAL */
}

#ifdef HS_PIM_CACHE_MAP //  wangxiaolin 20090804

/*==========================================================
 * author		: chenxiang      
 * function 	: hs_myLoadCacheMap
 * decr		: �Ѵ洢���е�map ��Ϣ���뵽�ڴ��������
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/4/2009 
 ==========================================================*/
BOOLEAN hs_myLoadCacheMap()
{
    MMIFILE_HANDLE	file_handle = PNULL;
    MMIFILE_ERROR_E ffs_error = SFS_NO_ERROR;
    uint32 file_size = 0;
    uint32 opt_mode=0;
    uint32 i = 0;
    uint32 map_item_count = 0;
    uint32 charsread = 0;
    char buffer[MMIPIM_MAX_LOCAL_ID_LEN * 2];
    SmlNewMapItemListPtr_t maplist=NULL;
    SmlNewMapItemListPtr_t temp=NULL;


    //syncml_cb_message:"MMIPIM hs_myLoadCacheMap"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3246_112_2_18_2_47_4_150,(uint8*)"");
    // ���ļ�ϵͳ��ȡ״̬
    if (!MMIAPIFMM_IsFileExist((uint16 *)hs_map_filename, 13))
    {
        return FALSE;
    }
    else
    {
	opt_mode = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;/*lint !e655*/
	file_handle=   MMIAPIFMM_CreateFile((uint16 *)hs_map_filename, 
							opt_mode, 
							PNULL, 
							PNULL
							);
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        if (0 == file_size)
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        else
        {
            map_item_count = file_size / (MMIPIM_MAX_LOCAL_ID_LEN * 2);
            for (i = 0; i < map_item_count; i++)
            {
                hs_smlLibMemset(buffer, 0, sizeof(buffer));
                ffs_error = MMIAPIFMM_ReadFile(file_handle,buffer,sizeof(buffer),&charsread,PNULL);
                if (0 < charsread && SFS_NO_ERROR == ffs_error)
                {
                    uint16				entry_id	=0;
                    uint8                       item_status=0;
              #ifdef HS_PIM_VCAL  //Added by wangxiaolin  2009.9.22 
                    PIM_T_P  pMe=MMIPIM_GetPIM();
              #endif /* HS_PIM_VCAL */
                    
                    maplist=hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(SmlNewMapItemList_t));
                    if(NULL==maplist)
                    {
                            MMIAPIFMM_CloseFile(file_handle);
                            return FALSE;
                    }
                    hs_smlLibMemset(maplist, 0, (long)sizeof(SmlNewMapItemList_t));
                    hs_smlLibMemcpy(maplist->target, buffer, MMIPIM_MAX_LOCAL_ID_LEN);
                    hs_smlLibMemcpy(maplist->source, &buffer[MMIPIM_MAX_LOCAL_ID_LEN], MMIPIM_MAX_LOCAL_ID_LEN);
                    //syncml_cb_message:"MMIPIM read PimMap[%d] target = %s "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3290_112_2_18_2_47_4_151,(uint8*)"ds", i, maplist->target);
                    //syncml_cb_message:"MMIPIM read PimMap[%d] source = %s "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3291_112_2_18_2_47_4_152,(uint8*)"ds", i, maplist->source);
                    
                    entry_id=hs_smlString2unsignedInt(maplist->source);
             #ifdef HS_PIM_VCAL  //Added by wangxiaolin  2009.9.22 
                    if(SYNC_MODE_PB == pMe->m_sync_mode)
                    {
             #endif /* HS_PIM_VCAL */
                        item_status=MMIPB_GetPimStatus(entry_id);
             #ifdef HS_PIM_VCAL  //Added by wangxiaolin  2009.9.22 
                    }
                    else if(SYNC_MODE_VCAL== pMe->m_sync_mode)
                    {
                        item_status=MMIVCAL_GetPimStatus(entry_id);
                    }
              #endif /* HS_PIM_VCAL */
  
                    //ͬ��ʧ�ܺ�û��������������
                    if (PB_NO_MODIFY==(PB_PIM_STATUS_E) item_status)//modify by wangxiaolin 2009.09.22,
                    {
                        if(NULL == hs_globlemapItemList)
                        {
                            hs_globlemapItemList = maplist;
                        }
                        else
                        {
                            temp = hs_globlemapItemList;
                            while(NULL != temp->next)
                            {
                                temp = temp->next;
                            }
                            temp->next = maplist;
                        }
                    }
                    else
                    {
                        //syncml_cb_message:"MMIPIM  this item has been modified ,id = %d "
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3326_112_2_18_2_47_4_153,(uint8*)"d", entry_id);
                    }
                }
            }
        }
        MMIAPIFMM_CloseFile(file_handle);
    }
    return TRUE;
}

/*==========================================================
 * author		: chenxiang      
 * function 	: hs_myClearCacheMap
 * decr		: ����ڴ��кʹ洢���е�map ��Ϣ
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/4/2009 
 ==========================================================*/
void hs_myClearCacheMap()
{
    //syncml_cb_message:"MMIPIM hs_myClearCacheMap"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3346_112_2_18_2_47_4_154,(uint8*)"");
    if (NULL != hs_globlemapItemList)
    {
        hs_smlFreeNewMapItemList(hs_globlemapItemList);

        hs_globlemapItemList = NULL;
    }
      
    // ���ļ�ϵͳ��ȡ״̬
    if (MMIAPIFMM_IsFileExist((uint16 *)hs_map_filename, 13))
    {
        MMIAPIFMM_DeleteFile((uint16 *)hs_map_filename, NULL);
    }
}



/*==========================================================
 * author		: chenxiang      
 * function 	: hs_mySaveCacheMap
 * decr		: ���ڴ��е�map �����浽�洢����
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/4/2009 
 * modify by wangxiaolin 2010.01.11.ÿ��map���¶�д���ļ�ϵͳ���б��棬
 * ������һ����֮��ͷ��͸�server�����ͺ�Ϳ����ͷŻ�����
 * ��map������ռ�ö�̬�ڴ����������ͬ���ɹ����������
 * ���ļ�ϵͳ�л����map��
 ==========================================================*/
void hs_mySaveCacheMap()
{
    MMIFILE_HANDLE	file_handle = PNULL;
    //MMIFILE_ERROR_E ffs_error = SFS_NO_ERROR;
    uint32 opt_mode=0;
    char buffer[MMIPIM_MAX_LOCAL_ID_LEN * 2];
    SmlNewMapItemListPtr_t temp = NULL;
    uint32 charswrite = 0;
    
    //syncml_cb_message:"MMIPIM hs_mySaveCacheMap"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3384_112_2_18_2_47_5_155,(uint8*)"");
#ifdef PIM_ONLY_CACHE_LAST_MESSAGE_MAP
    if (MMIAPIFMM_IsFileExist((uint16 *)hs_map_filename, 13))
    {
        MMIAPIFMM_DeleteFile((uint16 *)hs_map_filename, NULL);
    }
#endif

    if (NULL != hs_globlemapItemList)
    {
        // ������д���ļ�
        if (MMIAPIFMM_IsFileExist((uint16 *)hs_map_filename, 13))
        {
            opt_mode = SFS_MODE_APPEND| SFS_MODE_WRITE;/*lint !e655*/
        }
        else
        {
            opt_mode = SFS_MODE_CREATE_NEW | SFS_MODE_WRITE;/*lint !e655*/
        }
        
        file_handle=   MMIAPIFMM_CreateFile((uint16 *)hs_map_filename, 
        						opt_mode, 
        						PNULL, 
        						PNULL
        						);
        temp = hs_globlemapItemList;
        while ((NULL != temp)&&(NULL != file_handle)) //modify by wangxiaolin 2009.11.02 
        {
            hs_smlLibMemset(buffer, 0, sizeof(buffer));
            hs_smlLibMemcpy(buffer, temp->target, MMIPIM_MAX_LOCAL_ID_LEN);
            hs_smlLibMemcpy(&buffer[MMIPIM_MAX_LOCAL_ID_LEN], temp->source, MMIPIM_MAX_LOCAL_ID_LEN);
            MMIAPIFMM_WriteFile(file_handle, buffer, sizeof(buffer), &charswrite, NULL);
            //syncml_cb_message:"MMIPIM write PimMap target = %s "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3416_112_2_18_2_47_5_156,(uint8*)"s", temp->target);
            //syncml_cb_message:"MMIPIM write PimMap source = %s "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3417_112_2_18_2_47_5_157,(uint8*)"s", temp->source);
            temp = temp->next;
        }
        MMIAPIFMM_CloseFile(file_handle);
    }
}

/*==========================================================
 * author		: chenxiang      
 * function 	: hs_myGetFlagOfSendingCacheMap
 * decr		: �жϵ�ǰ�Ƿ��ڷ��ͻ����map ��Ϣ
 * parameter	: 
 * return		: hs_isSendingCacheMap
 * effect		: NO
 * history	:created  2/4/2009 
 ==========================================================*/
BOOLEAN hs_myGetFlagOfSendingCacheMap()
{
    if (hs_isSendingCacheMap)
        //syncml_cb_message:"MMIPIM hs_myGetFlagOfSendingCacheMap TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3435_112_2_18_2_47_5_158,(uint8*)"");
    else
        //syncml_cb_message:"MMIPIM hs_myGetFlagOfSendingCacheMap FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3437_112_2_18_2_47_5_159,(uint8*)"");
    
    return hs_isSendingCacheMap;
}

/*==========================================================
 * author		: chenxiang      
 * function 	: hs_mySetFlagOfSendingCacheMap
 * decr		: ���õ�ǰ�Ƿ��ڷ��ͻ����map ��Ϣ�ı�־
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  2/4/2009 
 ==========================================================*/
void hs_mySetFlagOfSendingCacheMap(BOOLEAN flag)
{
    hs_isSendingCacheMap = flag;
    
    if (hs_isSendingCacheMap)
        //syncml_cb_message:"MMIPIM hs_mySetFlagOfSendingCacheMap TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3455_112_2_18_2_47_5_160,(uint8*)"");
    else
        //syncml_cb_message:"MMIPIM hs_mySetFlagOfSendingCacheMap FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3457_112_2_18_2_47_5_161,(uint8*)"");
}

#endif /* HS_PIM_CACHE_MAP */


/******************************************************************* 
FUNCTION ������:hs_myPrintxmlorwbxml
������������ӡ�����ɵ�XML�ĵ���WBXML�ĵ����ļ�
                           ��������ݵ�ʱ��ѹ����ʽȷ������ӡ
                            ���ĵ���output.xml��
** ��  ��: null
** ��  ��:����
** ��  ��:2005-5-10
** �汾: ver 1.0
****************************************************************/

void hs_myPrintxmlorwbxml(char* filename,  short id)
{

 #ifdef FEATURE_PIM_DEBUG 
   WsmSmGlobals_t  wsmsmtemp;
   uint16   offset = 0;
   //syncml_efs_message:"MMIPIM &&& step=%s,id=%d "
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3479_112_2_18_2_47_5_162,(uint8*)"sd",filename,id);
/*Start of  wangxiaolin 2009.9.22 for if encodetype=wbxml,not print */
  if(hs_task_relay_info->encodetype!=SML_XML)
  {
    return;
  }
/*End   of  wangxiaolin 2009.9.22 for if encodetype=wbxml,not print */
  if(hs_pGlobalAnchor!=NULL)
  {
    wsmsmtemp=hs_pGlobalAnchor->wsmGlobals->wsmSm;
     while(wsmsmtemp->memH!=id)
     {
            wsmsmtemp=wsmsmtemp->next;
         if(wsmsmtemp==NULL)
         {
               //syncml_efs_message:"error occurs! reason: wrong wsmglobal id"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3492_112_2_18_2_47_5_163,(uint8*)"");
            return;
         }
     }
    offset= id-1;
    if(offset<HS_MAX_WSM_BUFFERS)
    {
        hs_myPrintbuffer(wsmsmtemp->winH,hs_pGlobalAnchor->wsmGlobals->wsmBuf[offset].usedBytes);
    }
#ifdef HS_PIM_LOG_FILE
      hs_myPrintLogFiles(hs_task_relay_info->sessionID, hs_task_relay_info->messageID - 1, filename, wsmsmtemp->winH,hs_pGlobalAnchor->wsmGlobals->wsmBuf[id-1].usedBytes);
#endif
   }
#endif
    return;
}


short hs_Data_Sync_main()
{
   return   hs_Data_Sync_init();
}


/******************************************************************* 
** ������:hs_Data_Sync_init
** ������������ʼ��taskģ�飬�����ڴ���䡣
***����:
** ��  ��: null
** ��  ��:������
** ��  ��:2007-9-28
**Global resource dependence : 
*******************************************************************/
short hs_Data_Sync_init()
{
    uint8           imei[MN_MAX_MOBILE_ID_LENGTH]={0};
    char           imeiStr[MN_MAX_MOBILE_ID_LENGTH<<1]={0};
    MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;

    SCI_TM_T current_time={0};

    char  nv_lastanchor[MMIPIM_ANCHOR_LEN]={0};
    SmlEncoding_t  nv_sync_encode_type;
    auth_type nv_sync_auth_type;
    proxy_setting_info_type nv_proxy_setting;
    server_setting_info_type nv_server_setting;

    PIM_T_P  pMe = MMIPIM_GetPIM();

    char* lasta=NULL;
    char* nexta=NULL;

    char* nextnonce=NULL;
    char* proxyaddr=NULL;
    char* proxyport=NULL;
    char* proxyuname=NULL;
    char* proxypword=NULL;


    char* serveraddr=NULL;
    char* serverport=NULL;
    char* sphonebookdb=NULL;
    char* scalendardb=NULL;
    char* user=NULL;
    char* password=NULL;

    char* clientimei=NULL;
    char* cphonebookdb=NULL;
    char* ccanlendardb=NULL;
    MN_DUAL_SYS_E dual_sys=MMIPIM_GetSim();

    //syncml_task_message:"MMIPIM  ^_^hs_Data_Sync_init,time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3562_112_2_18_2_47_5_164,(uint8*)"d",PIM_GetCurTime());

    //get imsi from nv
    if (MNNV_GetIMEIEx(dual_sys, imei))
    {
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,imei,MN_MAX_IMEI_LENGTH<<1,(char*)imeiStr);
    }

    nv_sync_auth_type=pMe->m_authtype;
    nv_sync_encode_type=pMe->m_sml_encoding_type;
    nv_proxy_setting=pMe->m_proxy_setting;
    nv_server_setting=pMe->m_server_setting;

    //��ʼ�����̿��Ʊ������趨��ֵ
    hs_protocol_step = (task_protocol_step_type*)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(task_protocol_step_type));
    if(NULL==hs_protocol_step)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(hs_protocol_step, 0 , (long)sizeof(task_protocol_step_type));
    *hs_protocol_step  =  STEP_CLIENT_INIT_SYNC;

    hs_protocol_step_priv= (task_protocol_step_type*)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(task_protocol_step_type));
    if(NULL==hs_protocol_step_priv)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(hs_protocol_step_priv, 0 , (long)sizeof(task_protocol_step_type));
    *hs_protocol_step_priv  = * hs_protocol_step;
    
    //��ʼ�����̿��Ʊ������趨��ֵ
    hs_task_relay_info = (task_relay_info_type*)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(task_relay_info_type));
    if(NULL==hs_task_relay_info)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(hs_task_relay_info, 0 , (long)sizeof(task_relay_info_type));
 
    //��ʼ��status cmd queue,�趨��ֵ
    hs_status_cmd_que=(status_cmd_queue_type*)hs_smlLibMalloc(__FILE__, __LINE__, (long)sizeof(status_cmd_queue_type));
    if(NULL==hs_status_cmd_que)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(hs_status_cmd_que, 0 , (long)sizeof(status_cmd_queue_type));
    hs_status_cmd_que->queue=NULL;
    hs_status_cmd_que->totalnumber=0;

    hs_task_relay_info->sessionID = hs_g_sessionID++; // chenxiang 20090206
    if (hs_g_sessionID > 200) //The maximum length of a SessionID is 4 bytes. Note, for a client having an 8 bit incrementing SessionID counter is enough for practical implementations.
        hs_g_sessionID = 1;
    hs_task_relay_info->messageID=1;
    hs_task_relay_info->cmdID=1;

    //get last anchor from NV
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.12.8
    if(SYNC_MODE_PB == pMe->m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
    MMINV_READ(MMIPIM_LAST_ANCHOR,nv_lastanchor,return_value); // chenxiang 20090213 ���û��ʱ���, ������ͬ��
     //SCI_TRACE_LOW:"MMIPIM \"\"\"\"\"last_anchor = %s\"\"\"\"\""
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3622_112_2_18_2_47_5_165,(uint8*)"s", nv_lastanchor);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.12.8
    }
    else
    {
        MMINV_READ(MMIPIM_VCAL_LAST_ANCHOR,nv_lastanchor,return_value); // chenxiang 20090213 ���û��ʱ���, ������ͬ��
        //SCI_TRACE_LOW:"MMIPIM \"\"\"\"\"vcal_last_anchor = %s\"\"\"\"\""
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3628_112_2_18_2_47_5_166,(uint8*)"s", nv_lastanchor);
    }
#endif /* HS_PIM_VCAL */

    if (MN_RETURN_SUCCESS != return_value)
    {
        hs_smlLibMemset(nv_lastanchor,0,sizeof(nv_lastanchor));
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.12.8
        if(SYNC_MODE_PB == pMe->m_sync_mode)
        {
#endif /* HS_PIM_VCAL */
        MMINV_WRITE(MMIPIM_LAST_ANCHOR,nv_lastanchor);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.12.8
        }
        else
        {
            MMINV_WRITE(MMIPIM_VCAL_LAST_ANCHOR,nv_lastanchor);
        }
#endif /* HS_PIM_VCAL */
  #ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.15 
        hs_task_relay_info->synctype=pMe->m_sync_type;
  #else
        hs_task_relay_info->synctype=SYNC_TYPE_S2WAY;
  #endif
    }
    else
    {
  #ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.15 
        hs_task_relay_info->synctype=pMe->m_sync_type;
  #else
        hs_task_relay_info->synctype=SYNC_TYPE_2WAY;
 #endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
    }


   //when 2way sync,for init and display
   if((SYNC_TYPE_2WAY==hs_task_relay_info->synctype)||(SYNC_TYPE_1WAYFCLI==hs_task_relay_info->synctype))
   {
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.11
    if(SYNC_MODE_PB == pMe->m_sync_mode)
    {
#endif /* HS_PIM_VCAL */
       pMe->m_recent_status_count.recent_add_count = MMIPB_GetRecentPBItemsCount(PB_RECENT_ADD); 
       pMe->m_recent_status_count.recent_replace_count = MMIPB_GetRecentPBItemsCount(PB_RECENT_REPLACE); 
       pMe->m_recent_status_count.recent_delete_count = MMIPB_GetRecentPBItemsCount(PB_RECENT_DELETE); 
       //SCI_TRACE_LOW:"MMIPIM \"\"\"\"\"recent_add_count = %d\"\"\"\"\""
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3673_112_2_18_2_47_5_167,(uint8*)"d", pMe->m_recent_status_count.recent_add_count);
       //SCI_TRACE_LOW:"MMIPIM \"\"\"\"\"recent_replace_count = %d\"\"\"\"\""
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3674_112_2_18_2_47_5_168,(uint8*)"d", pMe->m_recent_status_count.recent_replace_count);
       //SCI_TRACE_LOW:"MMIPIM \"\"\"\"\"recent_delete_count = %d\"\"\"\"\""
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3675_112_2_18_2_47_5_169,(uint8*)"d", pMe->m_recent_status_count.recent_delete_count);

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    }
    else if(SYNC_MODE_VCAL== pMe->m_sync_mode)
    {
       pMe->m_vcal_recent_status_count.recent_add_count = MMIVCAL_GetRecentVCALItemsCount(VCAL_RECENT_ADD); 
       pMe->m_vcal_recent_status_count.recent_replace_count = MMIVCAL_GetRecentVCALItemsCount(VCAL_RECENT_REPLACE);
       pMe->m_vcal_recent_status_count.recent_delete_count = MMIVCAL_GetRecentVCALItemsCount(VCAL_RECENT_DELETE); 
       //SCI_TRACE_LOW:"MMIPIM \"\"\"\"\"vcal_recent_add_count = %d\"\"\"\"\""
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3684_112_2_18_2_47_5_170,(uint8*)"d", pMe->m_vcal_recent_status_count.recent_add_count);
       //SCI_TRACE_LOW:"MMIPIM \"\"\"\"\"vcal_recent_replace_count = %d\"\"\"\"\""
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3685_112_2_18_2_47_5_171,(uint8*)"d", pMe->m_vcal_recent_status_count.recent_replace_count);
       //SCI_TRACE_LOW:"MMIPIM \"\"\"\"\"vcal_recent_delete_count = %d\"\"\"\"\""
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3686_112_2_18_2_47_5_172,(uint8*)"d", pMe->m_vcal_recent_status_count.recent_delete_count);
    }
#endif /* HS_PIM_VCAL */
   }
   
    lasta = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_ANCHOR_LEN);
    if(NULL==lasta)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(lasta, 0 , MMIPIM_ANCHOR_LEN);
    hs_task_relay_info->last_anchor=lasta;
    hs_smlLibStrcpy(lasta, (char*)(nv_lastanchor));

    nexta = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_ANCHOR_LEN);
    if(NULL==nexta)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(nexta, 0 , MMIPIM_ANCHOR_LEN);
    hs_task_relay_info->next_anchor=nexta; 

    //��ȡ��ǰʱ�䣬��ת����next anchor��ʽ����ȥ
    TM_GetTime(&current_time);
    hs_pim_convert_scitime2str(current_time,nexta);

    hs_task_relay_info->cred=NULL;
    hs_task_relay_info->authored=0;
    hs_task_relay_info->encodetype=(SmlEncoding_t)(nv_sync_encode_type);
    hs_task_relay_info->workspaceid=(short)0;
    hs_task_relay_info->authtype=nv_sync_auth_type;

    //����proxy.
    hs_task_relay_info->use_proxy=nv_proxy_setting.use_proxy;
    proxyaddr = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_IP_LEN + 1);
    if(NULL==proxyaddr)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(proxyaddr, 0 , MMIPIM_MAX_IP_LEN + 1);
    hs_task_relay_info->proxy_ip=proxyaddr;
    hs_smlLibStrcpy(proxyaddr,(char*)(nv_proxy_setting.addr));

    proxyport = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_PORT_LEN + 1);
    if(NULL==proxyport)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(proxyport, 0 , MMIPIM_MAX_PORT_LEN + 1);
    hs_task_relay_info->proxy_port=proxyport;
    hs_smlLibStrcpy(proxyport,(char*)(nv_proxy_setting.port));

    proxyuname= (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_USER_LEN + 1);
    if(NULL==proxyuname)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(proxyuname, 0 , MMIPIM_MAX_USER_LEN + 1);
    hs_task_relay_info->proxy_username=proxyuname;
    hs_smlLibStrcpy(proxyuname,(char*)(nv_proxy_setting.username));

    proxypword= (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_PASSWORD_LEN + 1);
    if(NULL==proxypword)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(proxypword, 0 , MMIPIM_MAX_PASSWORD_LEN + 1);
    hs_task_relay_info->proxy_password=proxypword;
    hs_smlLibStrcpy(proxypword,(char*)(nv_proxy_setting.password));

    //���÷�����
    serveraddr = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_URL_LEN + 1);
    if(NULL==serveraddr)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(serveraddr, 0 , MMIPIM_MAX_URL_LEN + 1);
    hs_task_relay_info->syncml_pim_server_name=serveraddr;
    hs_smlLibStrcpy(serveraddr,(char*)(nv_server_setting.addr));

    sphonebookdb = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_DATABASE_LEN + 1);
    if(NULL==sphonebookdb)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(sphonebookdb, 0 , MMIPIM_MAX_DATABASE_LEN + 1);
    hs_task_relay_info->syncml_pim_server_phonebook_db=sphonebookdb;
    hs_smlLibStrcpy(sphonebookdb,(char*)(nv_server_setting.pbdb));

    scalendardb = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_DATABASE_LEN + 1);
    if(NULL==scalendardb)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }    
    hs_smlLibMemset(scalendardb, 0 , MMIPIM_MAX_DATABASE_LEN + 1);
    hs_task_relay_info->syncml_pim_server_calendar_db=scalendardb;
    hs_smlLibStrcpy(scalendardb,(char*)(nv_server_setting.cddb));

    serverport = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_PORT_LEN + 1);
    if(NULL==serverport)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }        
    hs_smlLibMemset(serverport, 0 , MMIPIM_MAX_PORT_LEN + 1);
    hs_task_relay_info->syncml_pim_server_port=serverport;
    hs_smlLibStrcpy(serverport,(char*)(nv_server_setting.port));

    user = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_USER_LEN + 1);
    if(NULL==user)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }         
    hs_smlLibMemset(user, 0 , MMIPIM_MAX_USER_LEN + 1);
    hs_task_relay_info->syncml_pim_username=user;
    hs_smlLibStrcpy(user,(char*)(nv_server_setting.username));	

    password = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_PASSWORD_LEN + 1);
    if(NULL==password)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }        
    hs_smlLibMemset(password, 0 , MMIPIM_MAX_PASSWORD_LEN + 1);
    hs_task_relay_info->syncml_pim_password=password;
    hs_smlLibStrcpy(password,(char*)(nv_server_setting.password));	

    //����client
    clientimei = (char*)hs_smlLibMalloc(__FILE__, __LINE__, (MN_MAX_IMEI_LENGTH<<1)+5);
    if(NULL==clientimei)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    } 
    hs_smlLibMemset(clientimei, 0 , (MN_MAX_IMEI_LENGTH<<1)+5);
    hs_task_relay_info->syncml_pim_client_imei=clientimei;
    hs_imeitype2char(clientimei,(byte*)imeiStr);

    cphonebookdb = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_DATABASE_LEN + 1);
    if(NULL==cphonebookdb)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    } 
    hs_smlLibMemset(cphonebookdb, 0 , MMIPIM_MAX_DATABASE_LEN + 1);
    hs_task_relay_info->syncml_pim_client_phonebook_db=cphonebookdb;
    hs_smlLibStrcpy(cphonebookdb,hs_clientpbookdb);	

    ccanlendardb = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_MAX_DATABASE_LEN + 1);
    if(NULL==ccanlendardb)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    } 
    hs_smlLibMemset(ccanlendardb, 0 , MMIPIM_MAX_DATABASE_LEN + 1);
    hs_task_relay_info->syncml_pim_client_calendar_db=ccanlendardb;
    hs_smlLibStrcpy(ccanlendardb,hs_clientcalendardb);	

    nextnonce = (char*)hs_smlLibMalloc(__FILE__, __LINE__, MMIPIM_NEXT_NONCE);
    if(NULL==nextnonce)
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    } 
    hs_smlLibMemset(nextnonce, 0 , MMIPIM_NEXT_NONCE);
    hs_task_relay_info->syncml_pim_nextnonce=nextnonce;
    hs_smlLibStrcpy(nextnonce,hs_syncmlnextnonce);
    
    pMe->g_sync_result.server_number_of_change = 0;
    pMe->g_sync_result.client_number_of_change = 0;

    //initalize the syncml praser
    if(HS_SML_ERR_OK!=hs_syncml_init())
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    //syncml_task_message:"MMIPIM  ^_^ hs_syncml_init ok!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3856_112_2_18_2_47_6_173,(uint8*)"");

    g_is_pim_sync=TRUE; //add by wangxiaolin 

    if(HS_SML_ERR_OK!=hs_syncml_Comm_Init())
    {
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }

    //syncml_task_message:"MMIPIM  ^_^ syncml communication init ok!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_PB_3865_112_2_18_2_47_6_174,(uint8*)"");

    pMe->is_init=TRUE;
    return HS_SML_ERR_OK;
	
}


/*==========================================================
 * author		: wangxiaolin      
 * function 	: hs_imeitype2char
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/18/2009 
 ==========================================================*/
void hs_imeitype2char(char* dest, byte* source)
{
    char * dest_temp = dest;
    *dest_temp='I';
    dest_temp++;
    *dest_temp='M';
    dest_temp++;
    *dest_temp='E';
    dest_temp++;
    *dest_temp='I';
    dest_temp++;
    *dest_temp=':';
    dest_temp++;
    hs_smlLibMemcpy(dest_temp, &source[1], 15);
}



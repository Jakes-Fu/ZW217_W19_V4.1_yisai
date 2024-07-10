/*******************************************************************************
 ** FileName:      mmiPB_win32_simu.c                                          *
 ** Author:        baokun.yin                                                  *
 ** Date:          2009/04/01                                                  *
 ** CopyRight:     2009, Spreadtrum, Incoporated, All right reserved            *
 ** Description:   define BT simu interface                                    *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME                   DESCRIPTION                          *
 ** 2008/04/01     baokun.yin             Created                              *
 *******************************************************************************/

#define MMIPB_WIN32_SIMU_C


#include "std_header.h"
//#include "mmipb_win32_simu.h"
#include "window_parse.h"

#include "mmk_timer.h"

#include "mmidisplay_data.h"
#include "mmipub.h"

//#include "mmi_btinterface.h"
#include "mmi_appmsg.h"


#include "mmi_common.h"
#include "mmi_nv.h"

//#include "mmipb_internal.h"

#include "block_mem.h"
//#include "mmimultim.h"
#include "mmipb_id.h"
#include "mmi_id.h"
#ifdef __cplusplus
	extern "C"
	{
#endif

#ifdef CMCC_VCARD_SUPPORT
///*---------------------------------------------------------------------------*
//**                            GLOBAL FUCNTION IMPLEMENT                       *
//**---------------------------------------------------------------------------*/
uint8 s_simu_timer_id = 0;
uint16 s_pb_entry_id = 0;
PHONEBOOK_AAS_ENTRY_T	aas_entry_arr[MN_USIM_AAS_MAX_NUM] = 
{
	{(sizeof("Office Number") -1), "Office Number"},
	{sizeof("Home Number") -1, "Home Number"},
	{sizeof("FAX Number") -1, "FAX Number"}
};

PHONEBOOK_ANR_NUMBER_T anr_num1 = 
{
	{0x00, 0x11, 0x22, 0x33},
	1, 5,
    1,1,1,1,1
    
};
PHONEBOOK_ADDITIONAL_ENTRY_T addtional_entry_arr[10] = 
{
	//entry 1
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{0x11111111/*{0x11,0x22,0x32,0x34,0x55}*/,0, 5, 1,1,1,1,1},
		{0x22222222,1, 5, 1,1,1,1,1},
		{0x33333333,2, 5, 1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin1@163.com") -1, "baokun.yin1@163.com"},
	   1,//entry_id
	},
	//entry 2
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x22,0x22,0x32,0x34,0x55},0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55},1, 5, 1,1,1,1,1},
		{{0x44,0x22,0x32,0x34,0x55},2, 5, 1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin2@163.com") -1, "baokun.yin2@163.com"},
	   2,//entry_id
	},
	//entry 3
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x33,0x22,0x32,0x34,0x55},0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55},1, 5, 1,1,1,1,1},
		{{0x44,0x22,0x32,0x34,0x55},2, 5, 1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin3@163.com") -1, "baokun.yin3@163.com"},
	   3,//entry_id
	},
	//entry 4
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x44,0x22,0x32,0x34,0x55},0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55},1, 5, 1,1,1,1,1},
		{{0x44,0x22,0x32,0x34,0x55},2, 5, 1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin4@163.com") -1, "baokun.yin4@163.com"},
	   4,//entry_id
	},
	//entry 5
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x55,0x22,0x32,0x34,0x55},0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55},1, 5, 1,1,1,1,1},
		{{0x44,0x22,0x32,0x34,0x55},2, 5, 1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin5@163.com") -1, "baokun.yin5@163.com"},
	   5,//entry_id
	},
	//entry 6
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x66,0x22,0x32,0x34,0x55},0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55},1, 5, 1,1,1,1,1},
		{{0x44,0x22,0x32,0x34,0x55},2, 5, 1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin6@163.com") -1, "baokun.yin6@163.com"},
	   6,//entry_id
	},
	//entry 7
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x77,0x22,0x32,0x34,0x55},0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55},1, 5, 1,1,1,1,1},
		{{0x44,0x22,0x32,0x34,0x55},2, 5, 1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin7@163.com")-1, "baokun.yin7@163.com"},
	   7,//entry_id
	},
	//entry 8
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x88,0x22,0x32,0x34,0x55},0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55},1, 5, 1,1,1,1,1},
		{{0x44,0x22,0x32,0x34,0x55},2, 5, 1,1,1,1,1}
		},
	    //email_addr
	   {sizeof("baokun.yin8@163.com")-1, "baokun.yin8@163.com"},
	   8,//entry_id
	},
	//entry 9
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x99,0x22,0x32,0x34,0x55}, 0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55}, 1, 5, 1,1,1,1,1},
		{ {0x44,0x22,0x32,0x34,0x55},2, 5,1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin9@163.com") -1, "baokun.yin9@163.com"},
	   9,//entry_id
	},
	//entry 10
	{
		//anr_number[MN_USIM_AAS_MAX_NUM]
		{{{0x00,0x22,0x32,0x34,0x55}, 0, 5, 1,1,1,1,1},
		{{0x32,0x22,0x32,0x34,0x55}, 1, 5, 1,1,1,1,1},
		{{0x44,0x22,0x32,0x34,0x55},2, 5, 1,1,1,1,1}
		},
	   //email_addr
	   {sizeof("baokun.yin0@163.com") -1, "baokun.yin0@163.com"},
	   10,//entry_id
 	}
	
};

void MNSIM_SetEventAddInfoReadCnf(
   	 APP_MN_READ_ADDITIONAL_ENTRY_T       *add_info_ptr
	);

void MNSIM_SetEventUpdateusersInfoCnf(
    	APP_MN_UPDATE_USER_INFO_RESULT_T      *update_result_ptr
	);

void MNSIM_SetEventDelUserInfoCnf(
	APP_MN_UPDATE_USER_INFO_RESULT_T      *update_result_ptr
	);
///////

//typedef struct _PHONEBOOK_ADDITIONAL_ENTRY_T
//{
//    uint16                     entry_id;	
//    PHONEBOOK_ANR_NUMBER_T   	anr_number[MN_USIM_AAS_MAX_NUM];
//    MN_USIM_EMAIL_STRING_T             		email_addr;
//}PHONEBOOK_ADDITIONAL_ENTRY_T;
//
//typedef struct _PHONEBOOK_ANR_NUMBER_T
//{
//    uint8               aas_id; //EFaasÖÐµÄ¼ÇÂ¼ºÅ
//    uint8               number_len;
//    MNBCD               number[PHONEBOOK_MAX_DN_NUMBER_LEN];
//    MN_NUMBER_TYPE_E    ton;
//    MN_NUMBER_PLAN_E    npi;
//    BOOLEAN   	   adn_id_exist; /*if byte 16 and 17 exist*/
//    uint8     	   adn_sfi; /*byte 16, sfi of ADN*/
//    uint8     	   id_in_adn; /*byte 17, corespond of rec. id in adn*/
// } PHONEBOOK_ANR_NUMBER_T;
//////////
/*****************************************************************************/
//  Discription: handle bluetooth message 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleVirtulWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );

/*****************************************************************************/
//  Discription: create a timer 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void   createTimer(void);

/*****************************************************************************/
//  Discription: stop a timer
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void   stopTimer(void);


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 //window table define
WINDOW_TABLE(MMI_PB_SIMULATE_WIN_TAB) = 
{ 
    //CLEAR_LCD,
        WIN_PRIO( WIN_LOWEST_LEVEL ),
        WIN_FUNC((uint32) HandleVirtulWinMsg ),    
        WIN_ID(MMIPB_SIMU_WIN_ID),
        //WIN_TITLE(TXT_BLUETOOTH),
        //WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        END_WIN    
        
};

/*****************************************************************************/
//  Discription: handle bluetooth message 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleVirtulWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
        case MSG_TIMER:               
            break;

        default:
            break;
    }
    return result;
}


/*****************************************************************************/
//  Discription: create a timer 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  createTimer(void)
{
    if(s_simu_timer_id != 0)
    {
        //stop it first
        MMK_StopTimer(s_simu_timer_id);
    }
    s_simu_timer_id = MMK_CreateWinTimer(MMIPB_SIMU_WIN_ID, 500,TRUE);
}

/*****************************************************************************/
//  Discription: stop a timer
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  stopTimer(void)
{
    if(s_simu_timer_id != 0)
    {
        //stop it first
        MMK_StopTimer(s_simu_timer_id);
    }
}


uint8 PHONEBOOK_ReadUsimAas(
			uint8 				    rec_num,      //in/out
			PHONEBOOK_AAS_ENTRY_T	*aas_entry_ptr //in/out
			)
{
	SCI_ASSERT(PNULL != aas_entry_ptr);

    rec_num = MN_USIM_AAS_MAX_NUM;

	SCI_MEMCPY(aas_entry_ptr, aas_entry_arr, sizeof(aas_entry_arr));

	return ERR_MNDATAMAG_NO_ERR;
}

uint8 PHONEBOOK_Update3GPhonebook (
		    PHONEBOOK_ENTRY_T       	    *entry_ptr,  //in
		    BOOLEAN							anr_flag[MN_USIM_AAS_MAX_NUM],	 //in
		    BOOLEAN							email_flag, //in
		    BOOLEAN							adn_flag, //in
		    PHONEBOOK_ADDITIONAL_ENTRY_T	                    *additional_entry_ptr)
{

	int i = 0;
	APP_MN_UPDATE_USER_INFO_RESULT_T ret_cnf = {0};

	ret_cnf.adn_entry_id = entry_ptr->entry_id;
	ret_cnf.adn_flag = adn_flag; 
    ret_cnf.adn_result = OPERATE_SUCCESS;

	for(i = 0; i< MN_USIM_AAS_MAX_NUM; i++)
	{
		ret_cnf.anr_flag[i] = anr_flag[i]; 
		ret_cnf.anr_result[i] = OPERATE_SUCCESS;
	}

    ret_cnf.email_flag = email_flag;
	ret_cnf.email_result = TRUE;
	
	MNSIM_SetEventUpdateusersInfoCnf(&ret_cnf);
}


ERR_MNDATAMAG_CODE_E PHONEBOOK_Delete3GPhonebook (uint32  entry_id  /*in*/)
{
	int i = 0;
	APP_MN_UPDATE_USER_INFO_RESULT_T ret_cnf = {0};

	ret_cnf.adn_entry_id = entry_id;
	ret_cnf.adn_flag = TRUE; 
    ret_cnf.adn_result = OPERATE_SUCCESS;

	for(i = 0; i< MN_USIM_AAS_MAX_NUM; i++)
	{
		ret_cnf.anr_flag[i] = TRUE; 
		ret_cnf.anr_result[i] = OPERATE_SUCCESS;
	}

    ret_cnf.email_flag = TRUE;
	ret_cnf.email_result = TRUE;
	MNSIM_SetEventDelUserInfoCnf(&ret_cnf);
}

ERR_MNDATAMAG_CODE_E PHONEBOOK_ReadUsimAdditionalEntry (uint32 entry_id  /*in*/)
{
	APP_MN_READ_ADDITIONAL_ENTRY_T add_info = {0};
    int i  = 0;
	
 	add_info.adn_entry_id = entry_id;
	for(i =0; i < MN_USIM_AAS_MAX_NUM; i++)
	{
		
		if(addtional_entry_arr[entry_id].anr_number[i].number_len > 0)
		{
		add_info.anr_ret[i] = TRUE;
		add_info.anr_number[i] = addtional_entry_arr[entry_id].anr_number[i];
		}
		if(addtional_entry_arr[entry_id].email_addr.address_len > 0)
		{

           add_info.email_ret = TRUE;
		   add_info.email_addr = addtional_entry_arr[entry_id].email_addr;
		}
	}

	MNSIM_SetEventAddInfoReadCnf(&add_info);
}

void MNSIM_SetEventAddInfoReadCnf(
   	 APP_MN_READ_ADDITIONAL_ENTRY_T       *add_info_ptr
	)
{
	APP_MN_READ_ADDITIONAL_ENTRY_CNF_T     *cnf_t_ptr = PNULL;

	if(PNULL != add_info_ptr)
	{
		//cnf_t_ptr = (APP_MN_READ_ADDITIONAL_ENTRY_CNF_T *)SCI_ALLOC_APP(sizeof(APP_MN_READ_ADDITIONAL_ENTRY_CNF_T));

		MMI_CREATE_SIGNAL(
			cnf_t_ptr,
			APP_MN_READ_ADDITIONAL_ENTRY_CNF,
			sizeof(APP_MN_READ_ADDITIONAL_ENTRY_CNF_T),
			P_APP);
		SCI_ASSERT(cnf_t_ptr != PNULL);
		cnf_t_ptr->adn_entry_id = add_info_ptr->adn_entry_id;
		cnf_t_ptr->add_info = *add_info_ptr;		
		MMI_SEND_SIGNAL(cnf_t_ptr, P_APP);
	}
}

void MNSIM_SetEventUpdateusersInfoCnf(
    	APP_MN_UPDATE_USER_INFO_RESULT_T      *update_result_ptr
	)
{
	APP_MN_UPDATE_USER_INFO_CNF_T     *cnf_t_ptr = PNULL;

	if(PNULL != update_result_ptr)
	{
		//cnf_t_ptr = (APP_MN_UPDATE_USER_INFO_CNF_T *)SCI_ALLOC_APP(sizeof(APP_MN_UPDATE_USER_INFO_CNF_T));
		//SCI_ASSERT(cnf_t_ptr != PNULL);

		MMI_CREATE_SIGNAL(
			cnf_t_ptr,
			APP_MN_UPDATE_USER_INFO_CNF,
			sizeof(APP_MN_UPDATE_USER_INFO_CNF_T),
			P_APP);
		SCI_ASSERT(cnf_t_ptr != PNULL);
		cnf_t_ptr->adn_entry_id = update_result_ptr->adn_entry_id;
		cnf_t_ptr->update_result = *update_result_ptr;		
		MMI_SEND_SIGNAL(cnf_t_ptr, P_APP);
	}
}

void MNSIM_SetEventDelUserInfoCnf(
	APP_MN_UPDATE_USER_INFO_RESULT_T      *update_result_ptr
	)
{
	APP_MN_DEL_USER_INFO_CNF_T     *cnf_t_ptr = PNULL;

	if(PNULL != update_result_ptr)
	{
		//cnf_t_ptr = (APP_MN_DEL_USER_INFO_CNF_T *)SCI_ALLOC_APP(sizeof(APP_MN_DEL_USER_INFO_CNF_T));
		//SCI_ASSERT(cnf_t_ptr != PNULL);

		MMI_CREATE_SIGNAL(
			cnf_t_ptr,
			APP_MN_DEL_USER_INFO_CNF,
			sizeof(APP_MN_DEL_USER_INFO_CNF_T),
			P_APP);
		SCI_ASSERT(cnf_t_ptr != PNULL);
		cnf_t_ptr->adn_entry_id = update_result_ptr->adn_entry_id;
		cnf_t_ptr->update_result = *update_result_ptr;
		MMI_SEND_SIGNAL(cnf_t_ptr, P_APP);
	}
}

PUBLIC BOOLEAN MNSIM_Is_Usim(MN_DUAL_SYS_E dual_sys)
{
   return TRUE;
 }
#endif

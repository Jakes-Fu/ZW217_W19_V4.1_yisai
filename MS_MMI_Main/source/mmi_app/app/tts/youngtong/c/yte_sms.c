// defined(YTE_SUPPORT)

//#include "yte_mocor.h"
#include "mmisms_app.h"
#include "mmisms_export.h"
#include "yte_type.h"

#ifdef  MAX
#undef  MAX
#endif
#ifdef  MIN
#undef  MIN
#endif
#define MAX(a,b)                        (((a)>(b))?(a):(b))
#define MIN(a,b)                        (((a)<(b))?(a):(b))

extern MMISMS_OPER_ERR_E yte_SendMsg( 
													 MN_DUAL_SYS_E            dual_sys,
													 MN_CALLED_NUMBER_T        *dest_number_ptr,    //IN:
													 APP_SMS_USER_DATA_T         *user_data_ptr,        //IN: the user data
													 MN_SMS_STORAGE_E            storage,            //IN: the storage (SIM or NV)
													 MN_SMS_ALPHABET_TYPE_E    alphabet_type,        //IN: the dcs
													 BOOLEAN                    is_more_msg            //IN: the more message to send 
    );


int yte_SendSMS(const char * pszDst, const char * pszMsg,int isUsc2, MN_DUAL_SYS_E dual_sys)
{

	int rv = 0;
	MMISMS_OPER_ERR_E   send_result = MMISMS_NO_DEST_ADDRESS;	
	MN_CALLED_NUMBER_T  dest_num = {MN_NUM_TYPE_UNKNOW, MN_NUM_PLAN_UNKNOW, 0, 0};
	APP_SMS_USER_DATA_T user_data = {0};
	MMISMS_CONTENT_T    content = {0};
	uint8               total_num = 0;
	MN_SMS_USER_DATA_HEAD_T    user_head = {0};
	MN_SMS_STORAGE_E storage = MN_SMS_NO_STORED;
	MN_SMS_ALPHABET_TYPE_E alphabet_type = MN_SMS_DEFAULT_ALPHABET;	
	MMI_PARTY_NUMBER_T party_number = {MN_NUM_TYPE_UNKNOW, 0, 0};
	uint8 *dest_addr_ptr = (uint8*)pszDst;
	uint8 dest_addr_len = strlen(pszDst);
	uint8 ascii_content[MMISMS_MAX_MESSAGE_LEN] = {0};	// add by vion
	uint8 default_data[MMISMS_MAX_MESSAGE_LEN * 2] = {0};
	uint16 data_length = 0;
	
	yte_memset(&user_data, 0, sizeof (APP_SMS_USER_DATA_T));
	yte_memset(&content, 0, sizeof (MMISMS_CONTENT_T));	
	if(isUsc2)
	{
		alphabet_type = MN_SMS_UCS2_ALPHABET;
		content.length = MIN(yte_str_ucslen((const uint16*)pszMsg)*sizeof(uint16), MMISMS_MAX_UCS2_MESSAGE_LEN);
		yte_log("send msg content.length = %d", content.length);		
		yte_memcpy(content.data, (uint8 *)pszMsg, content.length);		
	}
	else
	{
		alphabet_type =  MN_SMS_DEFAULT_ALPHABET;
		yte_wstrn_to_str(ascii_content,
                                        MMISMS_MAX_MESSAGE_LEN,
                                        ((const uint16*)pszMsg),
                                        yte_str_ucslen((const uint16*)pszMsg),
                                        MIN(yte_str_ucslen((const uint16*)pszMsg), MMISMS_MAX_MESSAGE_LEN));
		
		//translate the ascii to default according GSM03.38
        data_length = yte_ascii_2_default(
                ascii_content,
                default_data,
                MIN(yte_str_ucslen((const uint16*)pszMsg), MMISMS_MAX_MESSAGE_LEN)
                );
		content.length = MIN(data_length,  MMISMS_MAX_MESSAGE_LEN);
		yte_memcpy(
                content.data,
                default_data,
                content.length 
                );
	}
	
	yte_sms_split_long_msg(&content,FALSE);
	yte_memset(&user_head, 0,sizeof (MN_SMS_USER_DATA_HEAD_T));	
	yte_sms_get_user_data(1, &content, &user_head,TRUE,&user_data,PNULL);
	yte_memset(&party_number, 0, sizeof (MMI_PARTY_NUMBER_T));
	yte_sms_get_party_number(dest_addr_ptr, dest_addr_len, &party_number);
	yte_memset(&dest_num, 0, sizeof (MN_CALLED_NUMBER_T));	
	dest_num.number_type = party_number.number_type;
	dest_num.num_len = MIN( party_number.num_len, MN_MAX_ADDR_BCD_LEN);
	yte_memcpy(
		(void *)dest_num.party_num,
		(void *)party_number.bcd_num ,
		dest_num.num_len 
		);
	yte_sms_set_oper_status(MMISMS_SEND_SMS);
	yte_log("send msg dual_sys = %d, dest_num.partynum = %s", dual_sys, dest_num.party_num);	
	send_result = yte_SendMsg(dual_sys, &dest_num, &user_data, storage, alphabet_type, FALSE);
	yte_log("send msg send_result = %d", send_result);
	if (MMISMS_NO_ERR == send_result)
	{
		rv = 1; //send ok , return 1
	}	
	return rv;
}

int yte_sms_filter(void* data)
{
	char num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    unsigned short num_len = 0;
	MN_SMS_ALPHABET_TYPE_E alphabet_type;
	char* contentPtr=NULL;
	char yte_SMSRecvContent[256];
    APP_MN_SMS_IND_T *sig_ptr = (APP_MN_SMS_IND_T *)data;
    APP_SMS_USER_DATA_T sms_user_data_t = {0};

	if(1 == yte_sms_core_check(data))
	{
		return 1;
	}
	
    if(PNULL == sig_ptr )
    {
        return 0;
    }
    
	alphabet_type = sig_ptr->sms_t.dcs.alphabet_type;
	yte_sms_decode_user_head_and_content(
        sig_ptr->dual_sys,
        sig_ptr->sms_t.user_head_is_exist,
        &(sig_ptr->sms_t.dcs),
        &(sig_ptr->sms_t.user_data_t),
        &sms_user_data_t
        );
	
    num_len = yte_sms_get_net_display_number(
										 (MN_NUMBER_TYPE_E)(sig_ptr->sms_t.origin_addr_t.number_type),
                                         (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                         sig_ptr->sms_t.origin_addr_t.num_len),
                                         sig_ptr->sms_t.origin_addr_t.party_num,
                                         num,
                                         (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                        );	
	yte_memset(yte_SMSRecvContent, 0, sizeof(yte_SMSRecvContent));

	if(MN_SMS_UCS2_ALPHABET == alphabet_type)
	{
		yte_str_swtich_low_height(yte_SMSRecvContent,(char*)sms_user_data_t.user_valid_data_t.user_valid_data_arr, sms_user_data_t.user_valid_data_t.length);
	}
	else
	{	
		//ascii
		int16 tmp_num_buf2[MMISMS_PBNUM_MAX_LEN + 1];
		yte_memset(yte_SMSRecvContent, 0, sizeof(yte_SMSRecvContent));		
		yte_convert_alphabet_to_unicode(sms_user_data_t.user_valid_data_t.user_valid_data_arr, yte_SMSRecvContent, sizeof(yte_SMSRecvContent)); //big endian
		yte_memset(tmp_num_buf2, 0, sizeof(tmp_num_buf2));
		yte_str_swtich_low_height(tmp_num_buf2,  yte_SMSRecvContent, sizeof(yte_SMSRecvContent)); //to low endian
		yte_memset(yte_SMSRecvContent, 0, sizeof(yte_SMSRecvContent));
		yte_memcpy(yte_SMSRecvContent, tmp_num_buf2, sizeof(tmp_num_buf2));
	}
	contentPtr = yte_SMSRecvContent;
	if(yte_checkSMSContent((char*)num, (char*)contentPtr) == 1)	
	{
		yte_sms_update_status_ex(sig_ptr->dual_sys, sig_ptr->storage, sig_ptr->record_id, MN_SMS_FREE_SPACE );
		return 1;
	}
	return 0;
}
//#endif

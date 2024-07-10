#ifndef _ABUP_NET_H_
#define _ABUP_NET_H_

#include "mmk_type.h"
#include "socket_types.h"

/*******************************************************************************
 **                        ERROR index define                                  *
 ******************************************************************************/
 
#define ABUP_ERR_FAIL              0
#define ABUP_ERR_SUCCESS           1

//#define ABUP_ERR_PHONE_FAILURE                                   0
//#define ABUP_ERR_NO_CONNECT_TO_PHONE                             1
#define ABUP_ERR_PHONE_ADAPTOR_LINK_RESERVED                     2
#define ABUP_ERR_OPERATION_NOT_ALLOWED                           3
#define ABUP_ERR_OPERATION_NOT_SUPPORTED                         4
#define ABUP_ERR_PH_SIM_PIN_REQUIRED                             5
#define ABUP_ERR_PH_FSIM_PIN_REQUIRED                            6
#define ABUP_ERR_PH_FSIM_PUK_REQUIRED                            7

#define ABUP_ERR_SIM_NOT_INSERTED                                10
#define ABUP_ERR_SIM_PIN_REQUIRED                                11
#define ABUP_ERR_SIM_PUK_REQUIRED                                12
#define ABUP_ERR_SIM_FAILURE                                     13
#define ABUP_ERR_SIM_BUSY                                        14
#define ABUP_ERR_SIM_WRONG                                       15
#define ABUP_ERR_INCORRECT_PASSWORD                              16
#define ABUP_ERR_SIM_PIN2_REQUIRED                               17
#define ABUP_ERR_SIM_PUK2_REQUIRED                               18
#define ABUP_ERR_SIM_PUK2_BLOCKED                                19

#define ABUP_ERR_MEMORY_FULL                                     20
#define ABUP_ERR_INVALID_INDEX                                   21
#define ABUP_ERR_NOT_FOUND                                       22
#define ABUP_ERR_MEMORY_FAILURE                                  23
#define ABUP_ERR_TEXT_STRING_TOO_LONG                            24
#define ABUP_ERR_INVALID_CHARACTERS_IN_TEXT_STRING               25
#define ABUP_ERR_DIAL_STRING_TOO_LONG                            26
#define ABUP_ERR_INVALID_CHARACTERS_IN_DIAL_STRING               27
#define ABUP_ERR_GPRS_OPERATION_FAILURE                          28
#define ABUP_ERR_GPRS_SEND_DATA_FAILURE                          29

#define ABUP_ERR_NO_NETWORK_SERVICE                              30
#define ABUP_ERR_NETWORK_TIMEOUT                                 31
#define ABUP_ERR_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY        32

#define ABUP_ERR_NETWORK_PERSONALISATION_PIN_REQUIRED            40
#define ABUP_ERR_NETWORK_PERSONALISATION_PUK_REQUIRED            41
#define ABUP_ERR_NETWORK_SUBSET_PERSONALISATION_PIN_REQUIRED     42
#define ABUP_ERR_NETWORK_SUBSET_PERSONALISATION_PUK_REQUIRED     43
#define ABUP_ERR_SERVICE_PROVIDER_PERSONALISATION_PIN_REQUIRED   44
#define ABUP_ERR_SERVICE_PROVIDER_PERSONALISATION_PUK_REQUIRED   45
#define ABUP_ERR_CORPORATE_PERSONALISATION_PIN_REQUIRED          46
#define ABUP_ERR_CORPORATE_PERSONALISATION_PUK_REQUIRED          47
#define ABUP_ERR_HIDDEN_KEY_REQUIRED                             48 /* NOTE: This key is required


/*cell info structure*/
typedef struct{
    abup_uint16 arfcn;           /*ARFCN*/
    abup_uint8 bsic;              /*BSIC*/
    abup_uint8 rxlev;            /*Received signal level*/
    abup_uint16 mcc;            /*MCC*/
    abup_uint16 mnc;            /*MNC*/
    abup_uint16 lac;              /*LAC*/
    abup_uint16 ci;                /*CI*/
	abup_uint16  updatetimes;
}abup_cell_info_struct;

abup_extern abup_uint32 abup_encode_dataaccount_id(void);

abup_extern abup_int32 abup_get_rcvd_block_size(void);
abup_extern ABUP_BOOL abup_config_addr_hdlr(void* inMsg);

abup_extern abup_int32 Abup_GetHostByName( abup_int32 request_id, const abup_char* domain, abup_uint32* addr_buf,abup_uint32 nwk_account_id);

abup_extern TCPIP_SOCKET_T abup_open_socket(void);

abup_extern abup_int32 abup_connect(abup_int32 request_id,abup_uint32 addr_buf,abup_uint16 addr_len,abup_uint16 port);

abup_extern void abup_soc_close(abup_socint soc_id);

abup_extern ABUP_BOOL abup_soc_hdlr(void *msg_ptr);

abup_extern abup_int32  abup_soc_send(abup_socint soc_id,Request* req );

abup_extern abup_int32 abup_soc_recv(abup_socint soc_id,abup_char* buf,abup_int32 len,abup_uint32 flag);

abup_extern void abup_soc_callback_init(ABUP_SOCKET_MGR_STRUCT* socket_mgr, ABUP_SOCMGR_USR abup_usr);

abup_extern void abup_net_set_state(abup_int32 netsm);

abup_extern abup_int32 abup_net_get_state(void);

abup_extern void abup_boot_hal_init(void);
abup_extern void abup_net_cell_req(void);
abup_extern void abup_net_cell_dereq(void);

abup_extern ABUP_BOOL abup_net_pdp_is_active(void);
abup_extern void abup_net_pdp_userevt_exe(void);



#endif

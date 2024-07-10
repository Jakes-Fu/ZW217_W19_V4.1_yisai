/*******************************************************************************
 ** FileName:      mmibt_win32_simu.c                                          *
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

#define MMIBT_WIN32_SIMU_C

#include "std_header.h"

#include "window_parse.h"

#include "mmk_timer.h"

#include "mmidisplay_data.h"
#include "mmipub.h"

//#include "mmi_btinterface.h"
#include "mmi_appmsg.h"


#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmibt_nv.h"


#include "block_mem.h"
#include "mmibt_id.h"

#include "bt_abs.h"
#include "bt_cfg.h"
#include "a2dp_api.h"

#include "mmibt_func.h"
#ifdef __cplusplus
	extern "C"
	{
#endif

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
#define  MMIBT_DEFAULT_TIME           1500
typedef enum
{
	BT_SEARCH,	
	BT_SEND,	
	BT_RECEIVE,	    
	BT_ACTION_MAX
}BT_ACTION_TYPE;

BT_DEVICE_INFO	s_inq_list[20] = {
    {
        {'b','l','u','e','t','o','o','t','h',' ','1',0},
        {0x33,0x44,0x55,0x66,0x77,0x88},
        BT_SERVICE_HANDSET|BT_SERVICE_HANDFREE
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','2',0},
        {0x11,0x44,0x55,0x66,0x77,0x88},
        BT_SERVICE_A2DP|BT_SERVICE_HANDSET|BT_SERVICE_HANDFREE
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','3',0},
        {0x22,0x44,0x55,0x66,0x77,0x88},
        BT_SERVICE_OPP
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','4',0},
        {0x00,0x44,0x55,0x66,0x77,0x88},
        BT_SERVICE_OPP
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','5',0},
        {0x55,0x44,0x55,0x66,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','6',0},
        {0x66,0x44,0x55,0x66,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','7',0},
        {0x66,0x44,0x55,0x66,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','8',0},
        {0x66,0x44,0x55,0x68,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','9',0},
        {0x66,0x44,0x55,0x69,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','0',0},
        {0x66,0x44,0x55,0x10,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','1',0},
        {0x66,0x44,0x55,0x11,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','2',0},
        {0x66,0x44,0x55,0x12,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','3',0},
        {0x66,0x44,0x55,0x13,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','4',0},
        {0x66,0x44,0x55,0x66,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','5',0},
        {0x66,0x44,0x14,0x57,0x78,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','6',0},
        {0x66,0x44,0x55,0x11,0x57,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','7',0},
        {0x66,0x44,0x55,0x17,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','8',0},
        {0x66,0x44,0x55,0x18,0x77,0x88},
        BT_SERVICE_ALL
    },
    {
        {'b','l','u','e','t','o','o','t','h',' ','1','9',0},
        {0x66,0x44,0x55,0x19,0x77,0x88},
        BT_SERVICE_ALL
    },  
    {
        {'b','l','u','e','t','o','o','t','h',' ','2','0',0},
        {0x66,0x44,0x55,0x20,0x77,0x88},
        BT_SERVICE_ALL
    },     
};

BT_DEVICE_INFO s_pair_device =  {{'r','e','m','o','t','e',' ','p','a','i','r',' ','b','l','u','e','t','o','o','t','h',0}, {0x66, 0x44,0x55,0x66,0x77,0x88}, BT_SERVICE_OPP};
BT_DEVICE_INFO s_remote_device =  {{'r','e','m','o','t','e',' ','d','e','f','b','l','u','e','t','o','o','t','h',0}, {0x12,0x44,0x55,0x66,0x77,0x88}, BT_SERVICE_OPP};

BT_INFO  s_bt_info = {0};

BT_CALLBACK    s_bt_callback = PNULL;
A2DP_CALLBACK  s_bt_a2dp_callback = PNULL;

static uint8 s_simu_timer_id = 0;
uint8        s_inquried_num = 0;
uint8        s_paired_num = 0;
uint32       s_total_file_size = 0;
uint32       s_transfered_size = 0;
uint32       s_step_size = 0;

BT_ACTION_TYPE s_current_action = BT_ACTION_MAX;
BOOLEAN s_hfg_active = FALSE; 
BOOLEAN s_a2dp_active = FALSE;
BOOLEAN s_is_hfg_play_music = FALSE;
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

/*****************************************************************************/
//  Discription: search one device and inform mmi
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  searchOneDevice(void);

/*****************************************************************************/
//  Discription: search one device and inform mmi
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  searchFinished(void);

/*****************************************************************************/
//  Discription: find device in db
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN  findDevice(BT_DEVICE_INFO *device_list_ptr,uint8 device_num ,BT_DEVICE_INFO *device_find_ptr);

/*****************************************************************************/
//  Discription: send packet
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  sendingPacket(void);

/*****************************************************************************/
//  Discription: receive packet
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  receivingPacket(void);

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 //window table define
WINDOW_TABLE(MMI_BT_SIMULATE_WIN_TAB) = 
{ 
    //CLEAR_LCD,
        WIN_PRIO( WIN_LOWEST_LEVEL ),
        WIN_FUNC((uint32) HandleVirtulWinMsg ),    
        WIN_ID(MMI_BT_SIMULATE_WIN_ID),
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
            switch(s_current_action)
            {
            case BT_SEARCH:
                if(s_inquried_num >= BT_DEVICE_MAX_NUM)
                {
                    //send inqury finished message
                    searchFinished();
                }
                else
                {
                    searchOneDevice();
                    s_inquried_num++;
                }
            	break;
            case BT_SEND:
                s_transfered_size += s_step_size;
                sendingPacket();
                break;
            case BT_RECEIVE:
                s_transfered_size += s_step_size;
                receivingPacket();
                break;
            default:
				
                break;
            }
            break;
			case MSG_CLOSE_WINDOW:
			case MSG_KEYUP_RED:
                BT_Stop();
				MMK_CloseWin(MMI_BT_SIMULATE_WIN_ID);
				break;
        default:
			result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

/*****************************************************************************/
//  Discription: find device in db
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN  findDevice(BT_DEVICE_INFO *device_list_ptr,uint8 device_num , BT_DEVICE_INFO *device_find_ptr)
{
    BOOLEAN ret = FALSE;
    uint32  i = 0;
    //uint32  device_num = 0;
    if(PNULL != device_list_ptr && PNULL != device_find_ptr)
    {
        //device_num = ARR_SIZE(device_list_ptr);
        for(i = 0; i < device_num; i++)
        {
            if(!memcmp(&(device_list_ptr[i].addr), &(device_find_ptr->addr), sizeof(BT_ADDRESS)))
            {
                ret = TRUE;
                SCI_MEMCPY(device_find_ptr, &device_list_ptr[i], sizeof(BT_DEVICE_INFO));
                break;
            }
        }
    }
    return ret;
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
    s_simu_timer_id = MMK_CreateWinTimer(MMI_BT_SIMULATE_WIN_ID, MMIBT_DEFAULT_TIME,TRUE);
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

/*****************************************************************************/
//  Discription: search one device and inform mmi
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  searchOneDevice(void)
{
    BT_MSG_T     msg_body = {0};

    msg_body.body_ptr = (BT_DEVICE_INFO *)SCI_ALLOC_APP(sizeof(BT_DEVICE_INFO));
    if(msg_body.body_ptr != PNULL)
    {
        msg_body.body_size = sizeof(BT_DEVICE_INFO);
        SCI_MEMCPY(msg_body.body_ptr, &s_inq_list[s_inquried_num], sizeof(BT_DEVICE_INFO));
    }
    msg_body.msg_id = ID_STATUS_CM_INQUIRY_RES;
    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);   
}

/*****************************************************************************/
//  Discription: search one device and inform mmi
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  searchFinished(void)
{
    BT_MSG_T     msg_body = {0};
    
    stopTimer();
    s_current_action = BT_ACTION_MAX;
    s_inquried_num = 0;
    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_CM_INQUIRY_FINISH;
    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);   
}


/*****************************************************************************/
//  Discription: send packet
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  sendingPacket(void)
{
    BT_MSG_T msg_body = {0};
    //BT_MSG_T msg_body = {0};
    BT_FILE_TRANSFER_T *file_ptr = PNULL;
    
    //BT_DEVICE_INFO  device_info
    //msg_body.body_ptr 
    file_ptr = (BT_FILE_TRANSFER_T *)SCI_ALLOC_APP(sizeof(BT_FILE_TRANSFER_T));
    if(file_ptr != PNULL)
    {
        msg_body.body_size = sizeof(BT_FILE_TRANSFER_T);
        //SCI_MEMCPY(&(device_ptr->addr), addr, sizeof(BT_ADDRESS));
        file_ptr->transmitted_count = s_transfered_size;
    }
    msg_body.body_ptr = file_ptr;    
    //stopTimer();
    //s_current_action = BT_ACTION__MAX;
    //s_inquried_num = 0;
    //msg_body.body_size = 0;
    if(s_transfered_size >= s_total_file_size)
    {
        //finished
        msg_body.msg_id = ID_STATUS_OPC_PUT_FINAL;
        stopTimer();
        s_current_action = BT_ACTION_MAX;
    }
    else
    {
        //sending
        msg_body.msg_id = ID_STATUS_OPC_PUT_OBJECT;
    }
    //msg_body.msg_id = ID_STATUS_OPC_PUT_OBJECT;
    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);  
}

/*****************************************************************************/
//  Discription: receive packet
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void  receivingPacket(void)
{
    BT_MSG_T msg_body = {0};
    //BT_MSG_T msg_body = {0};
    BT_FILE_TRANSFER_T *file_ptr = PNULL;
    
    //BT_DEVICE_INFO  device_info
    //msg_body.body_ptr 
    file_ptr = (BT_FILE_TRANSFER_T *)SCI_ALLOC_APP(sizeof(BT_FILE_TRANSFER_T));
    if(file_ptr != PNULL)
    {
        msg_body.body_size = sizeof(BT_FILE_TRANSFER_T);
        //SCI_MEMCPY(&(device_ptr->addr), addr, sizeof(BT_ADDRESS));
        file_ptr->transmitted_count = s_transfered_size;
    }
    msg_body.body_ptr = file_ptr;    
    //stopTimer();
    //s_current_action = BT_ACTION__MAX;
    //s_inquried_num = 0;
    //msg_body.body_size = 0;
    if(s_transfered_size >= s_total_file_size)
    {
        //finished
        msg_body.msg_id = ID_STATUS_OPS_PUT_FINAL;
        stopTimer();
        s_current_action = BT_ACTION_MAX;
    }
    else
    {
        //sending
        msg_body.msg_id = ID_STATUS_OPS_PUT_OBJECT;
    }
    //msg_body.msg_id = ID_STATUS_OPC_PUT_OBJECT;
    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);  
}

/******************************************************************************
 * FUNCTION:		BT_Init
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *		func - 
 *
 * RETURNS:
 *
 * MESSAGES:
 * 		None.
 ******************************************************************************/
BT_STATUS BT_Init(BT_CALLBACK callback)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    s_bt_callback = callback;
    SCI_MEMSET(&(s_bt_info), 0x00, sizeof(BT_INFO));
    MMINV_READ(BT_REF_INFO,&(s_bt_info), return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        //need reset bt info
        s_bt_info.status = FALSE;
        s_bt_info.visibility = BT_PAGE_ENABLE;
    }
	
//lzk mark
#ifdef BT_DIALER_SUPPORT	
	s_bt_info.dev_count = 1;
	s_bt_info.dev_list[0].dev_class = BT_SERVICE_OPP + BT_SERVICE_FTP + BT_SERVICE_MAP_SERVER + BT_SERVICE_PBAP_SERVER;
	s_bt_info.dev_list[0].addr.addr[0] = 0x11;
	s_bt_info.dev_list[0].addr.addr[1] = 0x22;

    MMIAPICOM_StrToWstr("MOCOR", s_bt_info.local_name);
#else
	s_bt_info.dev_count = 1;
	s_bt_info.dev_list[0].dev_class = BT_SERVICE_OPP + BT_SERVICE_FTP;
	s_bt_info.dev_list[0].addr.addr[0] = 0x11;
	s_bt_info.dev_list[0].addr.addr[1] = 0x22;
#endif
	s_paired_num = 1;
    s_is_hfg_play_music = FALSE;
	BT_Stop();
    //MMK_CreateWin(MMI_BT_SIMULATE_WIN_TAB, PNULL);
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:		BT_Deinit
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *		None.
 *
 * RETURNS:
 *
 * MESSAGES:
 *		None.
 ******************************************************************************/
BT_STATUS BT_Deinit(void)
{
    MMINV_WRITE(BT_REF_INFO,&(s_bt_info));
    MMK_CloseWin(MMI_BT_SIMULATE_WIN_ID);

    return BT_SUCCESS;
}


/******************************************************************************
 * FUNCTION:		BT_Start
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *		None.
 *
 * RETURNS:
 *		BT_SUCCESS -
 *		BT_PENDING -
 *
 * MESSAGES:
 *		ID_STATUS_ME_BT_ON_CFM
 *			status = BT_SUCCESS/BT_ERROR
 ******************************************************************************/
BT_STATUS BT_Start(void)
{
    BT_MSG_T     msg_body = {0};
	
	if(!MMK_IsOpenWin(MMI_BT_SIMULATE_WIN_ID))
	{
		MMK_CreateWin(MMI_BT_SIMULATE_WIN_TAB, PNULL);
        MMK_HideWindow( MMI_BT_SIMULATE_WIN_ID, TRUE );
	}
    s_bt_info.status = TRUE;
    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_BT_ON_RES;
    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
     return BT_PENDING;
    //MMK_SendMsg();
}

/******************************************************************************
 * FUNCTION:		BT_Stop
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *		None.
 *
 * RETURNS:
 *		BT_SUCCESS -
 *		BT_PENDING -
 *
 * MESSAGES:
 *		ID_STATUS_ME_BT_OFF_CFM
 *			status = BT_SUCCESS/BT_ERROR
 ******************************************************************************/
BT_STATUS BT_Stop(void)
{
    BT_MSG_T msg_body = {0};

    s_bt_info.status = FALSE;
    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_BT_OFF_RES;
    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
     return BT_PENDING;
}

/******************************************************************************
 * FUNCTION:		BT_GetState
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *		None.
 *
 * RETURNS:
 *		TRUE  -
 *		FALSE -
 *
 * MESSAGES:
 *		None.
 ******************************************************************************/
BOOLEAN BT_GetState(void)
{
    return s_bt_info.status;
}

/******************************************************************************
 * FUNCTION:		BT_SearchDevice
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *		service_type - 
 *
 * RETURNS:
 *		BT_PENDING - 
 *
 * MESSAGES:
 *		ID_STATUS_DEV_INQ_RES
 *			data.dev_info
 *		
 *		ID_STATUS_DEV_INQ_ADDR_RES
 *			data.dev_info
 *
 *		ID_STATUS_DEV_INQ_CMP
 *
 *		ID_STATUS_DEV_INQ_CANCEL
 ******************************************************************************/
BT_STATUS BT_SearchDevice(uint32 service_type)
{
    s_current_action = BT_SEARCH;
    s_inquried_num = 0;
    createTimer();
     return BT_PENDING;
}

/******************************************************************************
 * FUNCTION:		BT_CancelSearch
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 * 		None.
 *
 * RETURNS:
 *		BT_PENDING - 
 *
 * MESSAGES:
 *		ID_STATUS_DEV_INQ_CANCEL
 ******************************************************************************/
BT_STATUS BT_CancelSearch(void)
{
    BT_MSG_T msg_body = {0};
    s_current_action = BT_ACTION_MAX;
    s_inquried_num = 0;
    stopTimer();
    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_CM_CANCEL_INQUIRY_RES;
    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
     return BT_PENDING;
}

/******************************************************************************
 * FUNCTION:        BT_PairDevice
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr - 
 *
 * RETURNS:
 *      BT_PENDING - 
 *
 * MESSAGES:
 *      ID_STATUS_PAIR_COMPLETE
 *          status = BT_SUCCESS / BT_ERROR
 *
 *      ID_STATUS_DEV_PIN_REQ
 *          data.dev_info
 ******************************************************************************/
BT_STATUS BT_PairDevice(const BT_ADDRESS *addr)
{
    BT_MSG_T msg_body = {0};
    BT_DEVICE_INFO *device_ptr = PNULL;
    
    //BT_DEVICE_INFO  device_info
    //msg_body.body_ptr 
    device_ptr = (BT_DEVICE_INFO *)SCI_ALLOC_APP(sizeof(BT_DEVICE_INFO));
    if(device_ptr != PNULL)
    {
        msg_body.body_size = sizeof(BT_DEVICE_INFO);
        SCI_MEMCPY(&(device_ptr->addr), addr, sizeof(BT_ADDRESS));
        if(findDevice(s_inq_list, BT_DEVICE_MAX_NUM, device_ptr))
        {
           SCI_MEMCPY(&s_pair_device, device_ptr, sizeof(BT_DEVICE_INFO));
        }
        else
        {
            SCI_MEMCPY(device_ptr->name, s_pair_device.name, sizeof(uint16) *BT_DEVICE_NAME_SIZE);
        }
    }
    msg_body.body_ptr = device_ptr;
    //msg_body.body_size = sizeof(BT_DEVICE_INFO);
    msg_body.msg_id = ID_STATUS_SC_PASSKEY_REQ;
    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
    return BT_PENDING;
}

/******************************************************************************
 * FUNCTION:        BT_InputPairPin
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr   - 
 *      pin    -
 *      length -
 *
 * RETURNS:
 *      BT_PENDING - 
 *
 * MESSAGES:
 *      ID_STATUS_PAIR_COMPLETE
 *          status = BT_SUCCESS / BT_ERROR
 ******************************************************************************/
BT_STATUS BT_InputPairPin(const BT_ADDRESS *addr, const uint8 *pin, int length)
{
    BT_MSG_T msg_body = {0};
    BT_DEVICE_INFO *device_ptr = PNULL;

    //BT_DEVICE_INFO  device_info
    //msg_body.body_ptr 
    device_ptr = (BT_DEVICE_INFO *)SCI_ALLOC_APP(sizeof(BT_DEVICE_INFO));
    if(device_ptr!= PNULL)
    {
        msg_body.body_size = sizeof(BT_DEVICE_INFO);
        SCI_MEMCPY(&(device_ptr->addr), addr, sizeof(BT_ADDRESS));
        SCI_MEMCPY(device_ptr->name, s_pair_device.name, sizeof(uint16) *BT_DEVICE_NAME_SIZE);
        msg_body.body_ptr  = device_ptr;
    }
    msg_body.body_size = sizeof(BT_DEVICE_INFO);
    msg_body.msg_id = ID_STATUS_SC_PAIR_RES;

    if(s_paired_num < BT_DEVICE_MAX_NUM)
    {
        SCI_MEMCPY(&(s_bt_info.dev_list[s_paired_num]), device_ptr, sizeof(BT_DEVICE_INFO));
        s_paired_num++;
        msg_body.status = BT_SUCCESS;
    }
    else
    {
        msg_body.status = BT_ERROR;
        s_paired_num = 0;
    }
    BT_UpdateNV();
    s_bt_callback(&msg_body);
    return BT_PENDING;
}

/******************************************************************************
 * FUNCTION:        BT_CancelPair
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr - 
 *
 * RETURNS:
 *      BT_PENDING - 
 *
 * MESSAGES:
 *      ID_STATUS_PAIR_COMPLETE
 *          status = BT_ERROR
 ******************************************************************************/
BT_STATUS BT_CancelPair(const BT_ADDRESS *addr)
{
    BT_MSG_T msg_body = {0};
    //BT_DEVICE_INFO  device_info

    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_SC_PAIR_RES;
    msg_body.status = BT_ERROR;
    s_bt_callback(&msg_body);
    return BT_PENDING;    
}

/******************************************************************************
 * FUNCTION:        BT_GetPairedDeviceCount
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      service_type -
 *
 * RETURNS:
 *      The count of paired devices of the specified service type.
 *
 * MESSAGES:
 *      None.
 ******************************************************************************/
int BT_GetPairedDeviceCount(uint32 service_type)
{

    uint32 i = 0;
    int num_count = 0;
    for (i = 0; i < s_paired_num; i ++)
    {
        if(service_type & s_bt_info.dev_list[i].dev_class)
        {
            num_count ++;
        }
    }
    return num_count;   
}

/******************************************************************************
 * FUNCTION:        BT_GetPairedDeviceInfo
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      service_type -
 *      index        -
 *      info         -
 *
 * RETURNS:
 *
 * MESSAGES:
 *      None.
 ******************************************************************************/
BT_STATUS BT_GetPairedDeviceInfo(uint32 service_type, int index, BT_DEVICE_INFO *info)
{
    BT_STATUS status = BT_ERROR;
    int       pair_index = index-1;
//     BT_DEVICE_INFO test = {0};

    if(pair_index < s_paired_num && PNULL != info)
    {
        SCI_MEMCPY(info, &(s_bt_info.dev_list[pair_index]), sizeof(BT_DEVICE_INFO));
        status = BT_SUCCESS;
    }
    return status;
}

/******************************************************************************
 * FUNCTION:        BT_GetPairedDeviceName
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr - 
 *      name -
 *
 * RETURNS:
 *
 * MESSAGES:
 *      None.
 ******************************************************************************/
BT_STATUS BT_GetPairedDeviceName(const BT_ADDRESS *addr, uint16 *name)
{
    BT_STATUS status = BT_ERROR;
    BT_DEVICE_INFO device = {0};

    if(PNULL != name && PNULL != addr)
    {
       SCI_MEMCPY(&device.addr, addr, sizeof(BT_ADDRESS));
       if(findDevice(s_bt_info.dev_list,s_paired_num, &device))
       {
            SCI_MEMCPY(name, device.name, sizeof(uint16)*BT_DEVICE_NAME_SIZE);
            status = BT_SUCCESS;
       }
    }
    return status;
}

/******************************************************************************
 * FUNCTION:        BT_SetPairedDeviceName
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr - 
 *      name -
 *
 * RETURNS:
 *
 * MESSAGES:
 *      None.
 ******************************************************************************/
BT_STATUS BT_SetPairedDeviceName(const BT_ADDRESS *addr, const uint16 *name)
{
    BT_STATUS status = BT_ERROR;
//     BT_DEVICE_INFO device = {0};
    uint32       i = 0;

    if(PNULL != name && PNULL != addr)
    {
        for (i = 0; i < s_paired_num; i ++)
        {
            if(!memcmp(&(s_bt_info.dev_list[i].addr), addr, sizeof(BT_ADDRESS)))
            {
                SCI_MEMCPY(s_bt_info.dev_list[i].name, name, sizeof(uint16)*BT_DEVICE_NAME_SIZE);
                status = BT_SUCCESS;
            }
        }        
    }
    return status;
}

/******************************************************************************
 * FUNCTION:       BT_GetPairedDeviceService
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr - 
 *
 * RETURNS:
 *      Service list of the specified device.
 *
 * MESSAGES:
 *      None.
 ******************************************************************************/
uint32 BT_GetPairedDeviceService(const BT_ADDRESS *addr)
{
    uint32  service = 0;
    BT_DEVICE_INFO device = {0};

    if(PNULL != addr)
    {
       SCI_MEMCPY(&device.addr, addr, sizeof(BT_ADDRESS));
       if(findDevice(s_bt_info.dev_list,s_paired_num, &device))
       {
            
            service = device.dev_class;
       }
    }
    return service;    
}

 
/******************************************************************************
 * FUNCTION:        BT_RemovePairedDevice
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr - 
 *
 * RETURNS:
 *      BT_PENDING -
 *
 * MESSAGES:
 *      ID_STATUS_PAIR_DELETED
 *          status = BT_SUCCESS / BT_ERROR
 ******************************************************************************/
BT_STATUS BT_RemovePairedDevice(const BT_ADDRESS *addr)
{
    BT_STATUS status = BT_ERROR;
    uint32       i = 0;
    uint32       j = 0;
    BT_MSG_T msg_body = {0};
    //BT_DEVICE_INFO  device_info

    if( PNULL != addr)
    {
        for (i = 0; i < s_paired_num; i ++)
        {
            if(!memcmp(&(s_bt_info.dev_list[i].addr), addr, sizeof(BT_ADDRESS)))
            {
                for(j =i; j < s_paired_num; j++)
                {
                    SCI_MEMCPY(&(s_bt_info.dev_list[j]), &(s_bt_info.dev_list[j + 1]), sizeof(BT_DEVICE_INFO));
                }
                status = BT_PENDING;
                break;
            }
        }        
    }

    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_SC_DEPAIR_RES;
    msg_body.status = SCI_SUCCESS;
     BT_UpdateNV();
    s_bt_callback(&msg_body); 

    return status;
}

/******************************************************************************
 * FUNCTION:        BT_SetVisibility
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      visible -
 *
 * RETURNS:
 *      BT_PENDING - 
 * 
 * MESSAGES:
 *      ID_STATUS_DISCOVER_REQ
 *          status
 *
 *      ID_STATUS_UN_DISCOVER_REQ
 *          status
 ******************************************************************************/  
BT_STATUS BT_SetVisibility(BT_SCAN_E visible)
{
    BT_MSG_T msg_body = {0}; 

    s_bt_info.visibility = visible;

    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_CM_VISIBLE_RES;
    msg_body.status = BT_SUCCESS;
     BT_UpdateNV();
    s_bt_callback(&msg_body);    
    return BT_PENDING;
}

/******************************************************************************
 * FUNCTION:        BT_GetVisibility
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      None.
 *
 * RETURNS:
 *      TRUE  -
 *      FALSE -
 *
 * MESSAGES:
 *      None.
 ******************************************************************************/
BT_SCAN_E BT_GetVisibility(void)
{
    return s_bt_info.visibility;
}

/******************************************************************************
 * FUNCTION:        BT_SetLocalName
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      name - 
 * 
 * RETURNS:
 *      
 * MESSAGES:
 *      None.
 ******************************************************************************/
BT_STATUS BT_SetLocalName(const uint16 *name)
{
    if(PNULL != name)
    {
        SCI_MEMCPY(s_bt_info.local_name, name, sizeof(uint16)*BT_DEVICE_NAME_SIZE);
        return BT_SUCCESS;
    }
    return BT_ERROR;
}

/******************************************************************************
 * FUNCTION:        BT_GetLocalName
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      name -
 *
 * RETURNS:
 *
 * MESSAGES:
 *      None.
 ******************************************************************************/
BT_STATUS BT_GetLocalName(uint16 *name)
{
    if(PNULL != name)
    {
        SCI_MEMCPY(name, s_bt_info.local_name, sizeof(uint16)*BT_DEVICE_NAME_SIZE);
        return BT_SUCCESS;
    }
    return BT_ERROR;
}

/******************************************************************************
 * File transfer functions
 ******************************************************************************/

/******************************************************************************
 * FUNCTION:        Send File with opp function
 *
 * PARAMETERS:
 *      addr      -
 *      dev       -
 *      file_path -
 *
 * RETURNS:
 *      BT_PENDING -
 *
 * MESSAGES:
 *      
 ******************************************************************************/
BT_STATUS BT_OppSendFile(const BT_ADDRESS *addr, const uint16 *file_path)
{
    SFS_HANDLE     file_handle = 0;
    BT_MSG_T       msg_body = {0};
    //BT_DEVICE_INFO *device_ptr = PNULL;

    //BT_DEVICE_INFO  device_info
    //msg_body.body_ptr 
    //device_ptr = (BT_DEVICE_INFO *)SCI_ALLOC_APP(sizeof(BT_DEVICE_INFO));
//    if(device_ptr!= PNULL)
//    {
//        msg_body.body_size = sizeof(BT_DEVICE_INFO);
//        SCI_MEMCPY(&(device_ptr->addr), addr, sizeof(BT_ADDRESS));
//        SCI_MEMCPY(device_ptr->name, s_pair_device.name, sizeof(uint16) *BT_DEVICE_NAME_SIZE);
//        msg_body.body_ptr  = device_ptr;
//     }
    msg_body.body_size = 0;
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_OPC_CONNECT_RES;
    s_bt_callback(&msg_body);
    
    file_handle = SFS_CreateFile((const uint16 *)file_path,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0); /*lint !e655*/
    s_total_file_size = MMIAPIFMM_GetFileSize(file_handle);
    SFS_CloseFile(file_handle);
    s_transfered_size = 0;
    s_step_size = s_total_file_size /10;
    s_current_action = BT_SEND;
    createTimer();
    return BT_PENDING;
}

BT_STATUS BT_CancelOppSendFile(void)
{
    BT_MSG_T  msg_body = {0};
    
    s_current_action = BT_ACTION_MAX;
    stopTimer();
    msg_body.body_size = 0;
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_OPC_PUT_ABORT;
    s_bt_callback(&msg_body);
    return BT_PENDING;

}

BT_STATUS BT_OppReceiveFile(const uint16 *file_path)
{    
    s_current_action = BT_RECEIVE;
    s_transfered_size = 0;
    s_step_size = s_total_file_size/10;
    createTimer();
    receivingPacket();
    return BT_PENDING;
}

BT_STATUS BT_CancelOppReceiveFile(void)
{
    BT_MSG_T  msg_body = {0};
    
    s_current_action = BT_ACTION_MAX;
    stopTimer();
    msg_body.body_size = 0;
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_OPS_PUT_ABORT;
    s_bt_callback(&msg_body);  
    return BT_PENDING;
}

BT_STATUS BT_FtpReceiveFile(const uint16 *file_name)
{
    return BT_OppReceiveFile(file_name);
}

BT_STATUS BT_CancelFtpReceiveFile(void)
{
    return BT_CancelOppReceiveFile();
}

BT_STATUS BT_CreateNewFolder(BOOLEAN  is_enable)
{
    return BT_SUCCESS;
}

BT_STATUS BT_DeleteFile(BOOLEAN  is_enable)
{
    return BT_SUCCESS;
}


/******************************************************************************
 * FUNCTION:		BT_ActivateHS
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *	addr - 
 *
 * RETURNS:
 *	BT_PENDING - 
 *
 * MESSAGES:
 *	ID_STATUS_HFAG_SLC_CONNECTED
 *		status
 ******************************************************************************/
BT_STATUS BT_ActivateHS(const BT_ADDRESS *addr)
{
    BT_MSG_T    msg_body = {0};
    BT_ADDRESS  *bt_addr_ptr = PNULL;
    
    bt_addr_ptr = (BT_ADDRESS  *)SCI_ALLOC_APP(sizeof(BT_ADDRESS));
    SCI_MEMCPY(bt_addr_ptr, addr, sizeof(BT_ADDRESS));
    SCI_MEMCPY(&s_bt_info.active_headset.addr, addr, sizeof(BT_ADDRESS));
    findDevice(s_bt_info.dev_list, s_paired_num, &s_bt_info.active_headset);
    msg_body.body_ptr = bt_addr_ptr;
    msg_body.body_size = sizeof(BT_ADDRESS);
    msg_body.msg_id = ID_STATUS_HFAG_SLC_CONNECTED;

    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
    s_hfg_active = TRUE;
    s_is_hfg_play_music = FALSE;
    return BT_PENDING;
}

/******************************************************************************
 * FUNCTION:		BT_DeactivateHS
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *	None.
 *
 * RETURNS:
 *	BT_PENDING - 
 *
 * MESSAGES: 
 *	ID_STATUS_HFAG_SLC_DISCONNECTED
 ******************************************************************************/
BT_STATUS BT_DeactivateHS(void)
{
    BT_MSG_T    msg_body = {0};
    BT_DEVICE_INFO  *bt_addr_ptr = PNULL;

    bt_addr_ptr = (BT_DEVICE_INFO  *)SCI_ALLOC_APP(sizeof(BT_DEVICE_INFO));
    SCI_MEMCPY(bt_addr_ptr, &s_bt_info.active_headset, sizeof(BT_DEVICE_INFO));
    msg_body.body_ptr = bt_addr_ptr;
    msg_body.body_size = sizeof(BT_DEVICE_INFO);
    msg_body.msg_id = ID_STATUS_HFAG_SLC_DISCONNECTED;

    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
    s_hfg_active = FALSE;
    s_is_hfg_play_music = FALSE;
    return BT_PENDING;
}

/******************************************************************************
 * FUNCTION:		BT_GetActivatedHS
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *	addr - 
 *
 * RETURNS:
 *	TRUE  -
 *	FALSE -
 *
 * MESSAGES:
 *	None.
 ******************************************************************************/
BOOLEAN BT_GetActivatedHS(BT_ADDRESS *addr)
{
   return s_hfg_active;
}

/******************************************************************************
 * FUNCTION:		BT_StartRing
 *
 * DESCRIPTION: 
 *
 * PARAMETERS:
 *	phone_num -
 *
 * RETURNS:
 *	BT_PENDING - 
 *
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_StartRing(const uint8 *phone_num)
{
    
    BT_MSG_T    msg_body = {0};
//     BT_ADDRESS  *bt_addr_ptr = PNULL;

    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_HFAG_AUDIO_CONNECTED;

    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:		BT_StopRing
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *	None.
 *
 * RETURNS:
 *	
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_StopRing(void)
{
    BT_MSG_T    msg_body = {0};

    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_HFAG_AUDIO_DISCONNECTED;

    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:		BT_StartAlert
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *	None.
 *
 * RETURNS:
 *	BT_PENDING - 
 * 
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_StartAlert(void)
{
   return BT_StartRing(PNULL);
}

/******************************************************************************
 * FUNCTION:        BT_RemoteAlerting
 *
 * DESCRPTION:
 *
 * PARAMETERS:
 *  None.
 *
 * RETURNS:
 *  BT_PENDING - 
 *
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_RemoteAlerting(void)
{
    return BT_SUCCESS;
}
 

/******************************************************************************
 * FUNCTION:		BT_StopAlert
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *	None.
 *
 * RETURNS:
 *
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_StopAlert(void)
{
    return BT_StopRing();
}

/*****************************************************************************/
//  Discription: bt call connect
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
BT_STATUS BT_HfgCallIndStatus(uint32 status)
{
    return BT_SUCCESS;
}


/******************************************************************************
 * FUNCTION:		BT_CallConnected
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *	None.
 *
 * RETURNS:
 *	BT_PENDING - 
 * 
 * MESSAGES:
 *	ID_STATUS_HFAG_AUDIO_CONNECTED
 *****************************************************************************/
BT_STATUS BT_CallConnected(void)
{
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:		BT_CallDisconnected
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *	None.
 *
 * RETURNS:
 *	BT_PENDING - 
 *
 * MESSAGES:
 *	ID_STATUS_HFAG_AUDIO_DISCONNECTED
 ******************************************************************************/
BT_STATUS BT_CallDisconnected(void)
{
    return BT_SUCCESS;    
}

/******************************************************************************
 * FUNCTION:		BT_TransferAudioConn
 *
 * PARAMETERS:
 *	direction - BT_AUDIO_CONN_AG_TO_HF / BT_AUDIO_CONN_HF_TO_AG
 *
 * RETURNS:
 *	BT_PENDING -
 *
 * MESSAGES:
 *	ID_STATUS_HFAG_AUDIO_DISCONNECTED 
 *	ID_STATUS_HFAG_AUDIO_CONNECTED
 ******************************************************************************/
BT_STATUS BT_TransferAudioConn(uint8 direction)
{

    BT_MSG_T    msg_body = {0};

    msg_body.body_size = 0;

    if(BT_AUDIO_CONN_AG_TO_HF == direction)
    {
        //audio on
        msg_body.msg_id = ID_STATUS_HFAG_AUDIO_CONNECTED;
    }
    else
    {
        msg_body.msg_id = ID_STATUS_HFAG_AUDIO_DISCONNECTED;
    }

    msg_body.status = BT_SUCCESS;
    s_bt_callback(&msg_body);
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:        BT_SetVolume
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      channel -
 *      value   -
 *
 * RETURNS:
 *
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_SetVolume(uint8 channel, uint8 value)
{
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:        BT_GetVolume
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      channel -
 *
 * RETURNS:
 *
 * MESSAGES:
 ******************************************************************************/
uint8 BT_GetVolume(uint8 channel)
{
    return 5;
}

/******************************************************************************
 * FUNCTION:		BT_SetCallStatus
 * 
 * PARAMETERS:
 *	cell_service -
 * 	active_calls -
 *	call_setup   - BT_CALL_SETUP_IDLE / BT_CALL_SETUP_INCOMING
 *		       / BT_CALL_SETUP_OUTGOING / BT_CALL_SETUP_OUTGOING_ALERTED	
 *
 * RETURNS:
 * 
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_SetCallStatus(BOOLEAN cell_service, uint8 active_calls,uint8 hold_call, uint8 call_setup)
{
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:		BT_GetConnStatus
 * 
 * PARAMETERS:
 *		addr -
 *		service_type - BT_SERVICE_HSHF
 *
 * RETURNS:
 * 
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_GetConnStatus(BT_ADDRESS* addr, uint32 service_type)
{
     BT_STATUS return_value = BT_ERROR;
     if(s_hfg_active)
     {
         if(PNULL != addr)
         {
             if(!memcmp(addr, &(s_bt_info.active_headset.addr), sizeof(BT_ADDRESS)))
             {
                  return_value = BT_SUCCESS;
             }
         }
     }
     return return_value;
}
    
    
/*
 *  Set Ftp root folder path
 *  path: the folder path
 *  attribute:the folder is read only or write only or read&write
 *  mode : "r"/"R" -- Read Only,  "w"/"W" -- Write Only, "rw" -- read and write
 */
BT_STATUS BT_SetFtpRootFolder(const uint16* path, const char* mode)
{
  return BT_SUCCESS;  
}

/******************************************************************************
 * FUNCTION:        BT_EnterEutMode
 *
 * PARAMETERS:
 *      None.
 *
 * RETURNS:
 *
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_EnterEutMode(void)
{
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:		BT_GetBdAddr
 *
 * PARAMETERS:
 *		addr - a pointer to BT_ADDRESS struct
 *
 * RETURNS:
 *		None.
 *
 * MESSAGES:
 *		None.
 ******************************************************************************/
void BT_GetBdAddr(BT_ADDRESS *addr)
{
    if(PNULL != addr)
    {
        addr->addr[0] = 0x01;
        addr->addr[1] = 0x23;
        addr->addr[2] = 0x45;
        addr->addr[3] = 0x67;
        addr->addr[4] = 0x89;
        addr->addr[5] = 0x10;
    }
}
   
//////////////A2DP/////////////////////////////////////////////////////////////

/*****************************************************************************/
//  Description:    Init the A2DP Device Parameter.
//  Author:         yuhua.shi
//  Note:
//****************************************************************************/
PUBLIC BT_STATUS A2DP_InitDevice(A2DP_DEVICE_INIT_PARAM_T* param)
{
    if(PNULL != param)
    {
        s_bt_a2dp_callback = param->callback;
    }
    return BT_SUCCESS;
}


/*****************************************************************************/
//  Description:    Connect the A2DP Device selected.
//  Author:         
//  Note:
//****************************************************************************/
PUBLIC BT_STATUS A2DP_ConnectDevice(BT_ADDRESS addr)
{
    BT_MSG_T msg_body = {0};
    BT_A2DP_INFO_T   *a2dp_info_ptr = PNULL; 
    BT_DEVICE_INFO   device = {0};

    SCI_MEMCPY(&device.addr, &addr, sizeof(BT_ADDRESS));
    findDevice(s_bt_info.dev_list, s_paired_num, &device);
    SCI_MEMCPY(&s_bt_info.active_headset, &device, sizeof(BT_DEVICE_INFO));
    a2dp_info_ptr = SCI_ALLOC_APP(sizeof(BT_A2DP_INFO_T));
    SCI_MEMCPY(&(a2dp_info_ptr->dev_addr), &addr, sizeof(BT_ADDRESS));

    msg_body.body_ptr = a2dp_info_ptr;
    msg_body.body_size = sizeof(BT_A2DP_INFO_T);
    msg_body.msg_id = ID_STATUS_A2DP_CONNECTED;
    msg_body.status = BT_SUCCESS;
    s_a2dp_active = TRUE;
    s_bt_a2dp_callback(&msg_body);
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Description:    Disconnect the A2DP Device selected.
//  Author:        
//  Note:
//****************************************************************************/
PUBLIC BT_STATUS A2DP_DisConnectDevice(void)
{
    BT_MSG_T msg_body = {0};
    BT_A2DP_INFO_T   *a2dp_info_ptr = PNULL; 

    a2dp_info_ptr = SCI_ALLOC_APP(sizeof(BT_A2DP_INFO_T));
    SCI_MEMCPY(&(a2dp_info_ptr->dev_addr), &s_bt_info.active_headset.addr, sizeof(BT_ADDRESS));

    msg_body.body_ptr = a2dp_info_ptr;
    msg_body.body_size = sizeof(BT_A2DP_INFO_T);
    msg_body.msg_id = ID_STATUS_A2DP_DISCONNECTED;
    msg_body.status = BT_SUCCESS;
    s_a2dp_active = FALSE;
    s_bt_a2dp_callback(&msg_body);
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Description:    Open A2DP Device.
//  Author:        
//  Note:
//****************************************************************************/
PUBLIC BT_STATUS A2DP_OpenDevice(uint32 uiSampleRate)
{
    BT_MSG_T msg_body = {0};


    msg_body.body_ptr = PNULL;
    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_A2DP_SRC_START_STREAM;
    msg_body.status = BT_SUCCESS;
    s_a2dp_active = FALSE; 
    s_bt_a2dp_callback(&msg_body);
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Description:    Close A2DP Device
//  Author:         
//  Note:
//****************************************************************************/
PUBLIC BT_STATUS A2DP_CloseDevice(void)
{
    BT_MSG_T msg_body = {0};


    msg_body.body_ptr = PNULL;
    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_A2DP_SRC_CLOSE_STREAM;
    msg_body.status = BT_SUCCESS;
    s_a2dp_active = FALSE; 
    s_bt_a2dp_callback(&msg_body);
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Description:    Pause A2DP Device.
//  Author:         yuhua.shi
//  Note:
//****************************************************************************/
PUBLIC BT_STATUS A2DP_PauseDevice(void)
{
    BT_MSG_T msg_body = {0};


    msg_body.body_ptr = PNULL;
    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_A2DP_SRC_SUSPEND_STREAM;
    msg_body.status = BT_SUCCESS;
    s_a2dp_active = FALSE; 
    s_bt_a2dp_callback(&msg_body);
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Description:    Resume A2DP Device.
//  Author:         
//  Note:
//****************************************************************************/
PUBLIC BT_STATUS A2DP_ResumeDevice(void)
{
    BT_MSG_T msg_body = {0};


    msg_body.body_ptr = PNULL;
    msg_body.body_size = 0;
    msg_body.msg_id = ID_STATUS_A2DP_SRC_RESUME_STREAM;
    msg_body.status = BT_SUCCESS;
    s_a2dp_active = FALSE; 
    s_bt_a2dp_callback(&msg_body);
    return BT_SUCCESS;    
}

/*****************************************************************************/
//  Description:    Start the data sending after the a2dp stream is stated
//  Author:         yuhua.shi
//  Note:
//  Parameter:      None
//  Returns:        BT_SUCCESS: start data successful
//					BT_ERROR:   start data fail 
//****************************************************************************/
PUBLIC BT_STATUS A2DP_StartStreamData(void)
{
    return BT_SUCCESS;
}

/*****************************************************************************/
//  Description:    Send Abort request
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC BT_STATUS A2DP_Abort(void)
{
   return A2DP_CloseDevice();
}

/*****************************************************************************/
//  Description:    Get sample rate
//  Author:         ming.zhang
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC uint32 A2DP_GetSampleRate(void)
{
    return 44000;
}

/*****************************************************************************/
//  Description:   simulate send file to phone
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC void MMIBTAPI_Simu_Send(const uint16* file_path)
{
    SFS_HANDLE     file_handle = 0;
    BT_MSG_T       msg_body = {0};
    BT_FILE_INFO   *file_info_ptr = PNULL;
    uint16         name_len = 0;
    wchar          file_name[BT_FILE_NAME_SIZE] = {0};
    //BT_DEVICE_INFO *device_ptr = PNULL;
    file_handle = SFS_CreateFile((const uint16 *)file_path,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0); /*lint !e655*/
    s_total_file_size = MMIAPIFMM_GetFileSize(file_handle);
    SFS_CloseFile(file_handle);
   
    file_info_ptr = (BT_FILE_INFO *)SCI_ALLOC_APP(sizeof(BT_FILE_INFO));

    SCI_MEMSET(file_info_ptr, 0, sizeof(BT_FILE_INFO));

    SCI_MEMCPY(&(file_info_ptr->remote_dev), &s_remote_device, sizeof(BT_DEVICE_INFO));
    name_len = MIN(BT_FILE_NAME_SIZE, MMIAPICOM_Wstrlen(file_path));

    MMIAPICOM_GetFileName(file_path, name_len, file_name, &name_len);
    name_len = MIN(BT_FILE_NAME_SIZE, name_len);
    MMIAPICOM_Wstrncpy(file_info_ptr->name, file_name, name_len);
    file_info_ptr->size = s_total_file_size;
    msg_body.body_ptr = file_info_ptr;
    
    msg_body.body_size = sizeof(BT_FILE_INFO);
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_OPS_PUT_REQ;
    s_bt_callback(&msg_body);
    
    s_transfered_size = 0;
    s_step_size = 100;
    s_current_action = BT_RECEIVE;
    //createTimer();
}

/*****************************************************************************/
//  Description:   simulate cancel send file to phone
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC void MMIBTAPI_Simu_Send_Cancel(void)
{

    BT_MSG_T       msg_body = {0};

    msg_body.body_ptr = PNULL;
    msg_body.body_size = 0;
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_OPS_PUT_ABORT;
    s_bt_callback(&msg_body);
   
    s_current_action = BT_ACTION_MAX;
    stopTimer();
}

/*****************************************************************************/
//  Description:   simulate cancel receive file from phone
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC void MMIBTAPI_Simu_Receive_Cancel(void)
{
    BT_MSG_T       msg_body = {0};

    msg_body.body_ptr = PNULL;
    msg_body.body_size = 0;
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_OPS_PUT_ABORT;
    s_bt_callback(&msg_body);
   
    s_current_action = BT_ACTION_MAX;
    stopTimer();
}

/*****************************************************************************/
//  Description:   remote pair
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC void MMIBTAPI_Simu_Pair(void)
{
    BT_MSG_T       msg_body = {0};
    BT_DEVICE_INFO  *device_info_ptr = PNULL;

    device_info_ptr = SCI_ALLOC_APP(sizeof(BT_DEVICE_INFO));

    SCI_MEMCPY(device_info_ptr, &s_remote_device, sizeof(BT_DEVICE_INFO));
    msg_body.body_ptr = device_info_ptr;
    msg_body.body_size = sizeof(BT_DEVICE_INFO);
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_SC_PASSKEY_REQ;
    s_bt_callback(&msg_body);
}

 /******************************************************************************
 * FUNCTION:		BT_HFGOpenDevice
 * 
 * PARAMETERS:
 *	addr -
 *
 * 	service_type  -
 *
 * RETURNS:
 * 
 * MESSAGES:
 ******************************************************************************/
 uint16 BT_HFGOpenDevice(void )
 {
     s_is_hfg_play_music = TRUE;
     return 0;
 }

 
 /******************************************************************************
 * FUNCTION:		BT_HFGPlay
 *
 * PARAMETERS:
 *
 * RETURNS:
 *	BT_PENDING -
 *
 * MESSAGES:
 *	ID_STATUS_HFAG_PLAY_CFM 
 ******************************************************************************/
BT_STATUS BT_HFGPlay(void)
{
    BT_MSG_T       msg_body = {0};

    msg_body.body_size = 0;
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_HFAG_PLAY_CFM;
    s_bt_callback(&msg_body);
    return BT_PENDING;
}
 /******************************************************************************
 * FUNCTION:		BT_HFGStop
 *
 * PARAMETERS:
 *
 * RETURNS:
 *	BT_PENDING -
 *
 * MESSAGES:
 *	ID_STATUS_HFAG_STOP_CFM  
 ******************************************************************************/
BT_STATUS BT_HFGStop(void)
{
    BT_MSG_T       msg_body = {0};

    msg_body.body_size = 0;
    msg_body.status = BT_SUCCESS;
    msg_body.msg_id = ID_STATUS_HFAG_STOP_CFM;
    s_bt_callback(&msg_body);   
    return BT_PENDING;    
}

/******************************************************************************
 * FUNCTION:		BT_SaveToNV
 *
 * PARAMETERS:
 *	N/A
 *
 * RETURNS:
 *	BT_SUCCESS - Save NV successfully.
 *  BT_ERROR   - Save NV failed.
 *
 * MESSAGES:
 ******************************************************************************/
BT_STATUS BT_UpdateNV(void)
{
    MMINV_WRITE(BT_REF_INFO,&(s_bt_info));
    return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:		BT_AudioStart
 *
 * DESCRIPTION: Begin to play the audio
 *
 * PARAMETERS:
 *	None.
 *
 * RETURNS:
 *	BT_PENDING - 
 *
 * MESSAGES:
 *	ID_STATUS_HFAG_AUDIO_CONNECTED
 ******************************************************************************/
BT_STATUS  BT_AudioStart(void)
{
    return BT_SUCCESS; 
}

BT_STATUS  BT_AudioStop(void)
{
    return BT_SUCCESS; 
}

BT_FTP_SERVER_STATE_E BT_FTSGetState(BT_ADDRESS addr)
{
    return BT_FTS_RECEIVE_IDLE;
}

/******************************************************************************
 * FUNCTION:        BT_GetDeviceType
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr - 
 *
 * RETURNS:
 *      BT_PENDING - 
 *
 * MESSAGES:
 *      ID_STATUS_PAIR_COMPLETE
 *          status = BT_ERROR
 ******************************************************************************/
BT_STATUS BT_GetDeviceType(BT_DEVICE_INFO * dev_info, uint16 * dev_type)
{
    if(PNULL != dev_info)
    {
        if(BT_SERVICE_ALL == dev_info->dev_class )
        {
            *dev_type = BT_DEVICE_CLASS_COMPUTER;
        }
        else if(dev_info->dev_class & (BT_SERVICE_HANDFREE|BT_SERVICE_HANDSET| BT_SERVICE_A2DP))
        {
            *dev_type = BT_DEVICE_CLASS_AUDIO;
        }
        else if(BT_SERVICE_OPP & dev_info->dev_class)
        {
            *dev_type = BT_DEVICE_CLASS_PHONE;
        }
        else
        {
            *dev_type = BT_DEVICE_CLASS_COMPUTER;
        }
   }
   return BT_SUCCESS;
}

BT_STATUS BT_CancelFtpTransfer(void)
{
	return	0;
}

BT_STATUS BT_FtpSendFile(const uint16 *file_name)
{
	return 0;
}

void BT_AcceptDunReq(void)
{
	return;
}
void BT_RejectDunReq(void)
{
	return;
}
BT_STATUS BT_FtsConnect(BOOLEAN  is_enable)
{
	return BT_SUCCESS;
}
BT_STATUS BT_OpsConnect(BOOLEAN  is_enable)
{
	return BT_SUCCESS;
}
/******************************************************************************
 * FUNCTION:        BT_SetDeviceAuthorisation
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      is_authorised - 
 *
 * RETURNS:
 *      BT_SUCESS - 
 *      BT_ERROR
 *
 * MESSAGES:
 *
 ******************************************************************************/
BT_STATUS BT_SetDeviceAuthorisation(const BT_ADDRESS *addr_ptr,const BOOLEAN is_authorised)
{
   return BT_SUCCESS;
}
#ifdef MMIBT_HEADSET_AUTHORIZE 
/******************************************************************************
 * FUNCTION:        BT_AcceptAuthorisation
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      is_authorised - 
 *
 * RETURNS:
 *      BT_SUCESS - 
 *      BT_ERROR
 *
 * MESSAGES:
 *
 ******************************************************************************/
BT_STATUS BT_AcceptAuthorisation(const BT_ADDRESS *addr_ptr,const BOOLEAN is_authorised)
{
    return BT_SUCCESS;
}
#endif
/******************************************************************************
 * FUNCTION:        BT_GetDeviceAuthorisation
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      is_authorised - 
 *
 * RETURNS:
 *      BT_SUCESS - 
 *      BT_ERROR
 * MESSAGES:
 *
 ******************************************************************************/
BT_STATUS BT_GetDeviceAuthorisation(const BT_ADDRESS *addr_ptr,BOOLEAN *is_authorised_ptr)
{
   return BT_SUCCESS;
}

/******************************************************************************
 * FUNCTION:        BT_HfuGetConnStatus
 * 
 * DESCRIPTION:
 *
 * PARAMETERS:
 *      addr - 
 *
 * RETURNS:
 *      BT_SUCESS - 
 *      BT_ERROR
 * MESSAGES:
 *
 ******************************************************************************/
BOOLEAN BT_HfuGetConnStatus(BT_ADDRESS *addr)
{
    return BT_SUCCESS; 
}

#ifdef BT_BQB_SUPPORT
BT_STATUS BT_CIEVCommandIndicator(uint32 ciev_command,uint32 command_status)
{
    return BT_SUCCESS;
}

void BT_ClccCommandSend(uint8 idx, uint8 dir, uint8 status,
	uint8 mode,uint8 mpty,uint8* number,uint8* call_type)
{
    return;
}
BT_STATUS BT_SetNameOfNetOperator(uint8* name)
{
    return BT_SUCCESS;
}
void BT_ATStatusSend(BOOLEAN status)
{
    return;
}

void BT_SetLocalNumber(uint8* number)
{
    return;
}
#endif

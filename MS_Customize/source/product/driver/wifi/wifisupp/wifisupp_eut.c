/****************************************************************************************
** File Name:      wifieut_main.c                                                       *
** Author:         bruce.chi                                                            *
** Date:           20010.5.18                                                           *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is used for wifi supplicant eut functions' implementation  *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2010.5       bruce.chi         Create                                                *
**                                                                                      *
*****************************************************************************************/


/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "cfl_mem.h"
#include "wifisupp_api.h"
#include "wifisupp_eut.h"
#include "diag.h"
#include "nvitem.h"
#include "nv_item_id.h"
#include "wifi_drv.h"
#include "wifisupp_eut_adapter.h"
#include "tcpip_types.h"

/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
    WIFISUPP_EUT_CMD_ENTER_MODE     =0, //Enter/Leave EUT mode [控制进入模式]
    WIFISUPP_EUT_CMD_WRITE_MAC      =1, //Write WIFI address [写WIFI地址]
    WIFISUPP_EUT_CMD_READ_MAC       =2, //Read WIFI address [读WIFI地址]
    WIFISUPP_EUT_CMD_SET_CHANNEL    =3, //Set channel [设置频点] 
    WIFISUPP_EUT_CMD_SET_DATA_RATIO =4, //Set data ratio [设置速率]
    WIFISUPP_EUT_CMD_SET_MODE       =5, //Set WIFI mode [设置工作模式]
    WIFISUPP_EUT_CMD_SET_PCL        =6, //Set PCL [设置功率等级]
    WIFISUPP_EUT_CMD_TX             =7, //Start/Stop TX [发射]
    WIFISUPP_EUT_CMD_RX             =8, //Start/Stop RX [接收]
    WIFISUPP_EUT_CMD_READ_RX_VAL    =9, //Read WIFI Packets[接收]
    WIFISUPP_EUT_CMD_READ_TXFACOTOR =10,//Read WIFI factor[读WIFI factor]
    WIFISUPP_EUT_CMD_WRITE_TXFACTOR =11,//Write WIFI factor[写WIFI factor]
    WIFISUPP_EUT_CMD_SET_TXFACTOR   =12,//Set WIFI factor[设置WIFI factor]
    WIFISUPP_EUT_CMD_SET_CRYSTALCAP =13,//Set WIFI crystalcap[设置WIFI芯片内部负载电容] 
    
    WIFISUPP_EUT_CMD_READ_CALDATA = 14, // Read calibration data from phone RAM
    WIFISUPP_EUT_CMD_WRITE_CALDATA=15, //Write calibration data to phone RAM
    WIFISUPP_EUT_CMD_OVERRIDE_CALDATA=16, //override the calibration data with the one sent by MobileTester.

    WIFISUPP_EUT_CMD_READ_REGISTER=17,  // Read a register on the Wi-Fi chip
    WIFISUPP_EUT_CMD_WRITE_REGISTER=18, // Write a register on the Wi-Fi chip 

    WIFISUPP_EUT_CMD_CUSTOM_DATA = 25,
    
    WIFISUPP_EUT_MAX
}WIFISUPP_EUT_CMD_CODE_E;

typedef enum
{
    WIFISUPP_EUT_STATUS_FINISHED,   //sync function that has finished
    WIFISUPP_EUT_STATUS_PENDING,    //async function that is pending
    WIFISUPP_EUT_STATUS_MAX
}WIFISUPP_EUT_STATUS_E;             //eut status

typedef struct
{
    uint8 *param_ptr;
    uint32 len;
    uint32 command_code;
}WIFISUPP_EUT_PARAM_T;

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/
typedef struct
{
    uint32 pcl;         //Power Control Level 1~5
    uint32 n_factor;    //the Wifi Tx factor
}WIFISUPP_EUT_TX_FACTOR_T;

typedef struct
{
    //input
    uint8                   param_arr[WIFISUPP_COMMAND_MAX_LEN + 1]; //param array
    uint32                  len;                                     //param array len

    //slef private
    uint32                  command_code;                           //command code
    WIFISUPP_EUT_STATUS_E   status;                                 //request status
    BOOLEAN                 is_success;                             //is the action result success

    //response
    uint8                   mac[WIFI_MAC_ADDR_LEN];                 //mac
    uint32                  rx_total_packets;                       //rx total packets
    uint32                  rx_error_packets;                       //rx total error packets
    WIFISUPP_EUT_TX_FACTOR_T tx_facotr;                             //tx factor
    uint32                  read_register;                          //register read result
#ifdef WIFI_VENDOR_REALTEK
    uint32                  tx_total_packets;                       //rx total packets
    uint32                  tx_error_packets;
    uint8                   txreport;
    uint8                    rxreport;
#endif
}WIFISUPP_EUT_CONTEXT_T;

typedef struct
{
    uint32  command_code;
    BOOLEAN result;
    
    uint8   mac[WIFI_MAC_ADDR_LEN]; 
}WIFISUPP_EUT_CNF_T;

typedef struct
{
    MSG_HEAD_T  msg_head;
    uint8       param_arr[WIFISUPP_COMMAND_MAX_LEN + 1]; //param array
}WIFISUPP_EUT_CONFIRM_T;

/**-------------------------------------------------------------------------*
**                         GLOBAL VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
**                         STATIC CLASS DEFINITION                          *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                         STATIC VARIABLE DEFINITION                       *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         LOCAL FUNCTION DECLARE                           *
**--------------------------------------------------------------------------*/
LOCAL uint32 WIFISUPP_EUTHandler(uint8 **dest_pptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len);
LOCAL WIFISUPP_EUT_HANDLE_T WIFISUPP_EUTHandleReq(const WIFISUPP_EUT_PARAM_T *req_ptr);
PUBLIC BOOLEAN WIFISUPP_CheckStatus(WIFISUPP_EUT_HANDLE_T wifi_handle);
PUBLIC BOOLEAN WIFISUPP_EUTGetCnfString(WIFISUPP_EUT_HANDLE_T wifi_handle, WIFISUPP_EUT_CONFIRM_T *cnf_ptr);
PUBLIC BOOLEAN WIFISUPP_EUTWriteMACAddr(uint8 hw_addr[WIFI_MAC_ADDR_LEN]);
PUBLIC BOOLEAN WIFISUPP_EUTReadMACAddr(uint8 hw_addr[WIFI_MAC_ADDR_LEN]);
PUBLIC BOOLEAN WIFISUPP_EUTDelReq(WIFISUPP_EUT_HANDLE_T wifi_handle);
LOCAL BOOLEAN EUTSetAyncFinished(WIFISUPP_EUT_HANDLE_T wifi_handle, BOOLEAN is_success);
extern unsigned char wifi_eut_setmac(unsigned char* mac_addr);



LOCAL __inline int Atoi4Bytes(
                        uint8 *p,       //[IN]
                        uint32 *n_ptr   //[OUT]
                        )
{
    *n_ptr = ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
    return sizeof(uint32);
}

LOCAL __inline uint16 Encode2Bytes(
                        uint8 *p,       //[IN]
                        uint16 *n_ptr   //[OUT]
                        )
{
    *n_ptr =  ((p[0] << 8) | p[1]);
    return sizeof(uint16);
}

LOCAL __inline int Itoa4Bytes(uint32 i, uint8 *p)
{
    p[0] = (i & 0xFF000000) >> 24;
    p[1] = (i & 0x00FF0000) >> 16;
    p[2] = (i & 0x0000FF00) >> 8;
    p[3] = (i & 0x000000FF);

    return 4;   //encoded len
}

LOCAL __inline int Itoa2Bytes(uint16 i, uint8 *p)
{
    p[0] = (i & 0xFF00) >> 8;
    p[1] = (i & 0x00FF);

    return 2;   //encoded len
}

/**-------------------------------------------------------------------------*
**                         IMPLEMENTATION                                   *
**--------------------------------------------------------------------------*/
/****************************************************************************/
//Description : WIFI Eut init
//Global resource dependence : 
//Author: bruce.chi
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN WIFISUPP_EUTInit(void)
{
    DIAG_RegisterCmdRoutine(DIAG_WIFI_TEST_F, WIFISUPP_EUTHandler);
    
    return TRUE;
}

LOCAL uint32 WIFISUPP_EUTHandler(uint8 **dest_pptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len)
{
    MSG_HEAD_T             *msg_head_ptr = (MSG_HEAD_T*)src_ptr;
    WIFISUPP_EUT_PARAM_T    eut_in_param = {0};
    WIFISUPP_EUT_HANDLE_T   handle = 0;

    //copy the param
    eut_in_param.param_ptr = (uint8 *)src_ptr + sizeof(MSG_HEAD_T);
    eut_in_param.len = src_len - sizeof(MSG_HEAD_T);
    eut_in_param.command_code = msg_head_ptr->subtype;

    //do
    handle = WIFISUPP_EUTHandleReq(&eut_in_param);

    //check
    while(!WIFISUPP_CheckStatus(handle))
    {
        SCI_Sleep(500);
    }

    //send confirm
    *dest_pptr = SCI_ALLOCA(sizeof(WIFISUPP_EUT_CONFIRM_T));
    memset(*dest_pptr, 0, sizeof(WIFISUPP_EUT_CONFIRM_T));
    
    WIFISUPP_EUTGetCnfString(handle, (WIFISUPP_EUT_CONFIRM_T *)(*dest_pptr));
    *dest_len_ptr = ((WIFISUPP_EUT_CONFIRM_T *)(*dest_pptr))->msg_head.len;

    //delete the req
    WIFISUPP_EUTDelReq(handle);

    return 1;
}

/****************************************************************************/
//Description : Handle command string that come from diag
//Global resource dependence : 
//Author: bruce.chi
//Note: request string should be in big endian format
/****************************************************************************/
LOCAL WIFISUPP_EUT_HANDLE_T WIFISUPP_EUTHandleReq(const WIFISUPP_EUT_PARAM_T *req_ptr)
{
//#ifdef __BIG_ENDIAN
    WIFISUPP_EUT_CONTEXT_T *context_ptr = 0;

    if (req_ptr == PNULL)
    {
        return 0;
    }

    if (req_ptr->param_ptr == PNULL)
    {
        return 0;
    }

    //save in context
    context_ptr = SCI_ALLOC(sizeof(WIFISUPP_EUT_CONTEXT_T));
    memset(context_ptr, 0, sizeof(WIFISUPP_EUT_CONTEXT_T));
    //SCI_TRACE_LOW:"req_ptr->len=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_256_112_2_18_0_36_23_2298,(uint8*)"d", req_ptr->len);
    if (req_ptr->len > WIFISUPP_COMMAND_MAX_LEN)
    {
        //SCI_TRACE_LOW:"WIFISUPP_EUTHandleReq req_ptr->len > WIFISUPP_COMMAND_MAX_LEN"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_259_112_2_18_0_36_23_2299,(uint8*)"");
        return 0;
    }
    context_ptr->len = MIN(WIFISUPP_COMMAND_MAX_LEN, req_ptr->len);
    memcpy(context_ptr->param_arr, req_ptr->param_ptr,context_ptr->len);

    //get command code
    context_ptr->command_code = req_ptr->command_code;
    //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq command_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_267_112_2_18_0_36_23_2300,(uint8*)"d", context_ptr->command_code);


    switch(context_ptr->command_code)
    {
        case WIFISUPP_EUT_CMD_ENTER_MODE:   //0, async Enter/Leave EUT mode [控制进入模式]
        {
            uint32 bEnterOrLeave = 0;

            //bEnterOrLeave = 
            Atoi4Bytes(&context_ptr->param_arr[0], &bEnterOrLeave);

            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
            bEnterOrLeave = ntohl(bEnterOrLeave);
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_ENTER_MODE bEnterOrLeave=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_281_112_2_18_0_36_23_2301,(uint8*)"d", bEnterOrLeave);
            WIFISUPP_EUTEnterMode(context_ptr, (BOOLEAN)bEnterOrLeave);
        }
        break;

        case WIFISUPP_EUT_CMD_WRITE_MAC:    //1, sync Write WIFI address [写WIFI地址]
        {
            uint8 mac[WIFI_MAC_ADDR_LEN] = {0};

            memcpy(mac, &context_ptr->param_arr[0], WIFI_MAC_ADDR_LEN);
            WIFISUPP_EUTWriteMACAddr(mac);

            context_ptr->status = WIFISUPP_EUT_STATUS_FINISHED;
            context_ptr->is_success = TRUE;
        }
        break;
            
        case WIFISUPP_EUT_CMD_READ_MAC:     //2, sync Read WIFI address [读WIFI地址]
        {
            // uint8 mac[WIFI_MAC_ADDR_LEN] = {0};

            WIFISUPP_EUTReadMACAddr(context_ptr->mac);

            context_ptr->status = WIFISUPP_EUT_STATUS_FINISHED;
            context_ptr->is_success = TRUE;
        }
        break;

        case WIFISUPP_EUT_CMD_SET_CHANNEL:  //3, async Set channel [设置频点] 
        {
            uint32 channel = 0;
        
            //channel = 
            Atoi4Bytes(&context_ptr->param_arr[0], &channel);

            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
            channel = ntohl(channel);

            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_CHANNEL channel=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_319_112_2_18_0_36_23_2302,(uint8*)"d", channel);
            WIFISUPP_EUTSetChannel(context_ptr, channel);
        }
        break;
            
        case WIFISUPP_EUT_CMD_SET_DATA_RATIO:   //4, async Set data ratio [设置速率]
        {
            uint32 ratio = 0;
        
            //ratio = 
            Atoi4Bytes(&context_ptr->param_arr[0], &ratio);
        
            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
            ratio = ntohl(ratio);
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_DATA_RATIO ratio=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_333_112_2_18_0_36_23_2303,(uint8*)"d", ratio);
            WIFISUPP_EUTSetDataRatio(context_ptr, ratio);
        }
        break;
            
        case WIFISUPP_EUT_CMD_SET_MODE: //5, async Set WIFI mode [设置工作模式]
        {
            uint32 mode = 0;
        
            //mode = 
            Atoi4Bytes(&context_ptr->param_arr[0], &mode);
        
            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_MODE mode=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_346_112_2_18_0_36_23_2304,(uint8*)"d", mode);
            WIFISUPP_EUTSetMode(context_ptr, mode);
        }
        break;

        case WIFISUPP_EUT_CMD_SET_PCL:  //6, async Set PCL [设置功率等级]
        {
            uint32 pcl = 0;
        
            //pcl = 
            Atoi4Bytes(&context_ptr->param_arr[0], &pcl);
        
            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
            pcl = ntohl(pcl);
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_PCL pcl=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_360_112_2_18_0_36_23_2305,(uint8*)"d", pcl);
            WIFISUPP_EUTSetPCL(context_ptr, pcl);
        }
        break;
            
        case WIFISUPP_EUT_CMD_TX:       //7, async Start/Stop TX [发射]
        {
            uint32 tx_on_off = 0;
            int    len = 0;
            int    tx_packet_mode = 0;          
        
            //tx_on_off = 
            len += Atoi4Bytes(&context_ptr->param_arr[0], &tx_on_off);  // 1: on, 0: off
            len += Atoi4Bytes(&context_ptr->param_arr[len], (uint32 *)&tx_packet_mode); //0: FRAME, 1: TX99, 2, TX100, 3:SINE
        
            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_TX tx_on_off=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_376_112_2_18_0_36_24_2306,(uint8*)"d", tx_on_off);
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_TX tx_packet_mode=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_377_112_2_18_0_36_24_2307,(uint8*)"d", tx_packet_mode);
            WIFISUPP_EUTSetTx(context_ptr, (WIFISUPP_BOOL)tx_on_off, tx_packet_mode);
        }
        break;
            
        case WIFISUPP_EUT_CMD_RX:       //8, async Start/Stop RX [接收]
        {
            uint32 rx_command = 0;
        
            //rx_on_off = 
            Atoi4Bytes(&context_ptr->param_arr[0], &rx_command);    // 0: off, 1: on, 2: clear
            
            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
            rx_command = ntohl(rx_command);
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_RX rx_command=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_391_112_2_18_0_36_24_2308,(uint8*)"d", rx_command);
            WIFISUPP_EUTSetRx(context_ptr, rx_command);
        }
        break;
            
        case WIFISUPP_EUT_CMD_READ_RX_VAL:  //9, async Read WIFI Packets[接收]
        //SCI_TRACE_LOW:"WIFISUPP_EUT_CMD_READ_RX_VAL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_397_112_2_18_0_36_24_2309,(uint8*)"");
            WIFISUPP_EUTReadRxVal(context_ptr);    //later
            break;

        case WIFISUPP_EUT_CMD_READ_TXFACOTOR://10,sync Read WIFI factor[读WIFI factor]
        break;

        case WIFISUPP_EUT_CMD_WRITE_TXFACTOR://11, sync Write WIFI factor[写WIFI factor]
        break;
            
        case WIFISUPP_EUT_CMD_SET_TXFACTOR: //12, async
        {
            uint32  pcl = 0;
            uint32  tx_factor = 0;
            uint32  dummy = 0;
            int     len= 0;

            len += Atoi4Bytes(&context_ptr->param_arr[len], &dummy);
            len += Atoi4Bytes(&context_ptr->param_arr[len], &pcl);
            len += Atoi4Bytes(&context_ptr->param_arr[len], &tx_factor);
        
            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_SET_TXFACTOR tx_factor=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_419_112_2_18_0_36_24_2310,(uint8*)"d", tx_factor);
            WIFISUPP_EUTSetTxFactor(context_ptr, pcl, tx_factor);
        }
        break;

     case WIFISUPP_EUT_CMD_CUSTOM_DATA:
     {
#ifdef WIFI_VENDOR_REALTEK
    context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;
    //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_CUSTOM_DATA:  len is %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_428_112_2_18_0_36_24_2311,(uint8*)"d",context_ptr->len);
    WIFISUPP_EUTCusData(context_ptr, (uint*)context_ptr->param_arr);
#endif
     }
     break;
        #ifdef WIFI_SUPPORT_AR6302
        case WIFISUPP_EUT_CMD_SET_CRYSTALCAP: //14, sync Set WIFI crystalcap [设置WIFI芯片内部负载电容]     
        {
            uint8 szCrystalInOutCap[2];
            
            memcpy(szCrystalInOutCap, &context_ptr->param_arr[0], 2);   

            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;                      
            WIFISUPP_EUTSetCrystalCap(context_ptr, szCrystalInOutCap);  
        }
        break;

        case WIFISUPP_EUT_CMD_READ_CALDATA:
        {
            //SCI_TRACE_LOW:"WIFISUPP_EUT_CMD_READ_CALDATA"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_447_112_2_18_0_36_24_2312,(uint8*)"");
            WIFISUPP_EUTReadCalData(context_ptr);
        }
        break;

        case WIFISUPP_EUT_CMD_WRITE_CALDATA:
        {
            const char* pBuf = NULL;
            //SCI_TRACE_LOW:"WIFISUPP_EUT_CMD_READ_CALDATA"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_455_112_2_18_0_36_24_2313,(uint8*)"");

            pBuf = (char *)(context_ptr->param_arr);  

            context_ptr->status = WIFISUPP_EUT_STATUS_PENDING;  
            WIFISUPP_EUTWriteCalData(context_ptr, pBuf);
        }
        break;

        case WIFISUPP_EUT_CMD_OVERRIDE_CALDATA:
        {
        }
        break;
        #endif

        #if defined(WIFI_VENDOR_CSR)
        case WIFISUPP_EUT_CMD_READ_REGISTER: //17, sync read register     
        {
            uint32  addr = 0;
            int     len = 0;
            WIFISUPP_BOOL rc;
            uint32  my_to_read = 0;

            len += Atoi4Bytes(&context_ptr->param_arr[len], &addr);
            
            rc = WIFISUPP_EUTReadRegister(context_ptr, addr, &my_to_read);

            /*
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_READ_REGISTER addr=%x my_to_read=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_484_112_2_18_0_36_24_2314,(uint8*)"dd",addr, my_to_read );
             */
            if( rc )
            {
                context_ptr->is_success = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_READ_REGISTER failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_492_112_2_18_0_36_24_2315,(uint8*)"");
                context_ptr->is_success = FALSE;
            }
            
            context_ptr->status = WIFISUPP_EUT_STATUS_FINISHED;
            context_ptr->read_register = my_to_read;
        }
        break;

        case WIFISUPP_EUT_CMD_WRITE_REGISTER: //18, sync write register     
        {
            uint32  addr = 0;
            uint32  write_temp = 0;
            int     len = 0;
            WIFISUPP_BOOL rc;

            len += Atoi4Bytes(&context_ptr->param_arr[len], &addr);
            len += Atoi4Bytes(&context_ptr->param_arr[len], &write_temp);

            /*
            //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_WRITE_REGISTER addr1=%x write_temp=%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_513_112_2_18_0_36_24_2316,(uint8*)"dd",addr, write_temp );
             */

            rc = WIFISUPP_EUTWriteRegister(context_ptr, addr, write_temp);

            if( rc )
            {
                context_ptr->is_success = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"wifi: WIFISUPP_EUTHandleReq WIFISUPP_EUT_CMD_WRITE_REGISTER failed %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_524_112_2_18_0_36_24_2317,(uint8*)"d", rc );
                context_ptr->is_success = FALSE;
            }
            
            context_ptr->status = WIFISUPP_EUT_STATUS_FINISHED;            
        }
        break; 
        #endif        

        default:
            break;
    }

    return context_ptr;
//#else
//    SCI_ASSERT(0);
//#endif
}

/****************************************************************************/
//Description : delete request and free the handle
//Global resource dependence : 
//Author: bruce.chi
//Note: 
/****************************************************************************/
PUBLIC BOOLEAN WIFISUPP_EUTDelReq(WIFISUPP_EUT_HANDLE_T wifi_handle)
{
    if (wifi_handle != 0)
    {
        SCI_Free(wifi_handle);
    }

    return TRUE;
}

/****************************************************************************/
//Description : check whether the operation has finished
//Global resource dependence : 
//Author: bruce.chi
//Note:     TRUE: finished. FALSE: pending
/****************************************************************************/
PUBLIC BOOLEAN WIFISUPP_CheckStatus(WIFISUPP_EUT_HANDLE_T wifi_handle)
{
    WIFISUPP_EUT_CONTEXT_T *context_ptr = (WIFISUPP_EUT_CONTEXT_T *)wifi_handle;

    if (context_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"WIFISUPP_CheckStatus context_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_567_112_2_18_0_36_24_2318,(uint8*)"");
        return TRUE;
    }

    if (context_ptr->status == WIFISUPP_EUT_STATUS_FINISHED)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/****************************************************************************/
//Description : Get confirm of wifi command
//Global resource dependence : 
//Author: bruce.chi
//Note: If wifi has finished handling the req, this function will return TRUE, 
//      and diag can get the confirm string. 
//      If returned value is FALSE, diag should loop for confirm.
/****************************************************************************/
PUBLIC BOOLEAN WIFISUPP_EUTGetCnfString(WIFISUPP_EUT_HANDLE_T wifi_handle, WIFISUPP_EUT_CONFIRM_T *cnf_ptr)
{
    WIFISUPP_EUT_CONTEXT_T *context_ptr = (WIFISUPP_EUT_CONTEXT_T *)wifi_handle;

    if (context_ptr == 0 || cnf_ptr == 0)
    {
        return FALSE;
    }

    cnf_ptr->msg_head.type = DIAG_WIFI_TEST_F;
    cnf_ptr->msg_head.subtype = context_ptr->command_code;
    switch(context_ptr->command_code)
    {
        case WIFISUPP_EUT_CMD_ENTER_MODE:       // 0，Enter/Leave EUT mode [控制进入模式]
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }
                
            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;

        case WIFISUPP_EUT_CMD_WRITE_MAC:    // 1，Write WIFI address [写WIFI地址]
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
            
        case WIFISUPP_EUT_CMD_READ_MAC: // 2，Read WIFI address [读WIFI地址]
        {
            uint32  operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }
            
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + sizeof(operate_result) + WIFI_MAC_ADDR_LEN;

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);

            memcpy(&cnf_ptr->param_arr[len], context_ptr->mac, sizeof(context_ptr->mac));
            len += (int)sizeof(context_ptr->mac);

            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;

        case WIFISUPP_EUT_CMD_SET_CHANNEL:      // 3，Set channel [设置频点] 
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
            
        case WIFISUPP_EUT_CMD_SET_DATA_RATIO:   // 4，Set data ratio [设置速率]
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
            
        case WIFISUPP_EUT_CMD_SET_MODE:     // 5，Set WIFI mode [设置工作模式]
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
        
        case WIFISUPP_EUT_CMD_SET_PCL:  // 6，Set PCL [设置功率等级]
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
            
        case WIFISUPP_EUT_CMD_TX:   // 7，Start/Stop TX [发射]
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
            
        case WIFISUPP_EUT_CMD_RX:   // 8，Start/Stop RX [接收]
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
            
        case WIFISUPP_EUT_CMD_READ_RX_VAL:  // 9，Read WIFI Packets[接收]
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            len += Itoa4Bytes(context_ptr->rx_total_packets, &cnf_ptr->param_arr[len]);
            len += Itoa4Bytes(context_ptr->rx_error_packets, &cnf_ptr->param_arr[len]);

            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;

        case WIFISUPP_EUT_CMD_READ_TXFACOTOR:   //10, Read WIFI factor[读WIFI factor]
        break;
        
        case WIFISUPP_EUT_CMD_WRITE_TXFACTOR:   //11, Write WIFI factor[写WIFI factor]
        break;
        
        case WIFISUPP_EUT_CMD_SET_TXFACTOR:     //12，Set WIFI factor[设置WIFI factor] 
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
 case WIFISUPP_EUT_CMD_CUSTOM_DATA:
     {
#ifdef WIFI_VENDOR_REALTEK
       
        int     len = 0;
        memcpy(cnf_ptr->param_arr+len, context_ptr->param_arr, 304);
        cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)(len + 304);
         {
            int j = 0;
            for(j = 0; j<5 ;j++)
                //SCI_TraceLow:":%x\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_833_112_2_18_0_36_24_2319,(uint8*)"d", *((uint32*)(cnf_ptr->param_arr) + j));
        }
#endif
     }
      break;
        #ifdef WIFI_SUPPORT_AR6302
        case WIFISUPP_EUT_CMD_SET_CRYSTALCAP:   // 14，sync Set WIFI crystalcap [设置WIFI芯片内部负载电容]  
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;  
        #endif        

        case WIFISUPP_EUT_CMD_READ_CALDATA:
        {
            uint32 operate_result = 0;
            int     len = 0;
            //SCI_TRACE_LOW:"WIFISUPP_EUTSendCnf command_code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_863_112_2_18_0_36_24_2320,(uint8*)"d", context_ptr->command_code);
            

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);

            len += 1024;
            memcpy(cnf_ptr->param_arr, context_ptr->param_arr, 1024);

            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;

        case WIFISUPP_EUT_CMD_WRITE_CALDATA:
        {
            uint32 operate_result = 0;
            int     len = 0;
            //SCI_TRACE_LOW:"WIFISUPP_EUTSendCnf command_code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_888_112_2_18_0_36_25_2321,(uint8*)"d", context_ptr->command_code);

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;

        case WIFISUPP_EUT_CMD_OVERRIDE_CALDATA:
        {
            //SCI_TRACE_LOW:"WIFISUPP_EUTSendCnf command_code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_906_112_2_18_0_36_25_2322,(uint8*)"d", context_ptr->command_code);
        }
        break;

        #if defined(WIFI_VENDOR_CSR)
        case WIFISUPP_EUT_CMD_READ_REGISTER:
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);
            len += Itoa4Bytes(context_ptr->read_register, &cnf_ptr->param_arr[len]);

            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;

        case WIFISUPP_EUT_CMD_WRITE_REGISTER:
        {
            uint32 operate_result = 0;
            int     len = 0;

            if (context_ptr->is_success)
            {
                operate_result = 0;
            }
            else
            {
                operate_result = 1;
            }

            len += Itoa4Bytes(operate_result, cnf_ptr->param_arr);

            cnf_ptr->msg_head.len = sizeof(MSG_HEAD_T) + (uint16)len;
        }
        break;
        #endif
        
        default:
            //SCI_TRACE_LOW:"WIFISUPP_EUTSendCnf command_code=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_954_112_2_18_0_36_25_2323,(uint8*)"d", context_ptr->command_code);
            return FALSE;
    }

    return TRUE;
}


//sync func
PUBLIC BOOLEAN WIFISUPP_EUTWriteMACAddr(uint8 hw_addr[WIFI_MAC_ADDR_LEN])
{
#ifndef WIN32
    WIFI_ADDR_T         mac={0};
    WIFI_TX_FACTOR_T    factor = {0};

    //Write NV
    WIFI_GetNvParam(&mac, &factor);
    memcpy(mac.address, hw_addr, WIFI_NV_ADDR_LEN);
    WIFI_SaveNvParam(&mac, &factor);
    wifi_eut_setmac(mac.address);
#endif
    return TRUE;
}

//sync func
PUBLIC BOOLEAN WIFISUPP_EUTReadMACAddr(uint8 hw_addr[WIFI_MAC_ADDR_LEN])
{
#ifndef WIN32
    WIFI_ADDR_T         mac={0};
    
    //Read NV
    WIFI_GetNvParam(&mac, 0);
    memcpy(hw_addr, mac.address, WIFI_NV_ADDR_LEN);
#endif

    return TRUE;
}

//sync func
PUBLIC BOOLEAN WIFISUPP_EUTReadTxFactor(WIFISUPP_EUT_TX_FACTOR_T *factor_ptr)
{
    // WIFI_TX_FACTOR_T    factor={0};

    if (factor_ptr == 0)
    {
        return FALSE;
    }
#ifndef WIN32
    //Read NV
//    WIFI_GetNvParam(0, &factor);
//    factor->pcl = factor.;
//    factor->
#endif

    return TRUE;
}

//sync func
PUBLIC BOOLEAN WIFISUPP_EUTWriteTxFactor(uint8 hw_addr[WIFI_MAC_ADDR_LEN])
{
    // WIFI_ADDR_T         mac={0};
    // WIFI_TX_FACTOR_T    factor = {0};

#ifndef WIN32
    //Write NV
//    WIFI_GetNvParam(&mac, &factor);
//    memcpy(mac.address, hw_addr, sizeof(mac.address));
//    WIFI_SaveNvParam(&mac, &factor);
#endif

    return TRUE;
}

/****************************************************************************/
//Description : set async operation result
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
LOCAL BOOLEAN EUTSetAyncFinished(WIFISUPP_EUT_HANDLE_T wifi_handle, BOOLEAN is_success)
{
    WIFISUPP_EUT_CONTEXT_T *context_ptr = (WIFISUPP_EUT_CONTEXT_T *)wifi_handle;

    if (context_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"WIFISUPP_EUTSyncFinished context_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_1037_112_2_18_0_36_25_2324,(uint8*)"");
        return FALSE;
    }

    context_ptr->status = WIFISUPP_EUT_STATUS_FINISHED;
    context_ptr->is_success = is_success;

    return TRUE;
}

/****************************************************************************/
//Description : enter eut mode
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTEnterModeCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description : set channel 1~13
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetChannelCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description : set data ratio, 1Mbps, 2Mbps, 5.5, 11. 6, 9, 12, 18, 24, 36, 48, 54Mbps
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetDataRatioCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description : set wifi mode
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetModeCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description : set tx factor
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTxFactorCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description :  set pcl
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetPCLCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description : set tx on/off
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetTxCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description : set rx on/off
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetRxCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description : read rx total packets and error packets
//Global resource dependence : 
//Author: bruce.chi
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadRxValCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success, uint32 total_packets, uint32 err_packets)
{
    WIFISUPP_EUT_CONTEXT_T *context_ptr = (WIFISUPP_EUT_CONTEXT_T *)handle;

    if (context_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"WIFISUPP_EUTReadRxValCnf context_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_1129_112_2_18_0_36_25_2325,(uint8*)"");
        return FALSE;
    }

    context_ptr->rx_total_packets = total_packets;
    context_ptr->rx_error_packets = err_packets;

    return EUTSetAyncFinished(handle, is_success);
}

/****************************************************************************/
//Description : set crystal cap value in wifi chip
//Global resource dependence : 
//Author: 
//Note: async func
/****************************************************************************/
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTSetCrystalCapCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}

PUBLIC WIFISUPP_BOOL WIFISUPP_EUTReadCalDataCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success, const char* pBuf)
{
    WIFISUPP_EUT_CONTEXT_T *context_ptr = (WIFISUPP_EUT_CONTEXT_T *)handle;

    if (context_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"WIFISUPP_EUTReadCalDataCnf context_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_1154_112_2_18_0_36_25_2326,(uint8*)"");
        return FALSE;
    }

    if (is_success && pBuf)
        SCI_MEMCPY(context_ptr->param_arr, pBuf, 1024);

    return EUTSetAyncFinished(handle, is_success);
}

PUBLIC WIFISUPP_BOOL WIFISUPP_EUTWriteCalDataCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success)
{
    return EUTSetAyncFinished(handle, is_success);
}
#ifdef WIFI_VENDOR_REALTEK
PUBLIC WIFISUPP_BOOL WIFISUPP_EUTCusDataCnf(WIFISUPP_EUT_HANDLE_T handle, WIFISUPP_BOOL is_success, uint *pBuf)
{
    WIFISUPP_EUT_CONTEXT_T *context_ptr = (WIFISUPP_EUT_CONTEXT_T *)handle;
    if (context_ptr == PNULL)
    {
        //SCI_TRACE_LOW:"WIFISUPP_EUTReadRxValCnf context_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,WIFISUPP_EUT_1174_112_2_18_0_36_25_2327,(uint8*)"");
        return FALSE;
    }
    memcpy(context_ptr->param_arr, pBuf, 304);

    return EUTSetAyncFinished(handle, is_success);
}
#endif

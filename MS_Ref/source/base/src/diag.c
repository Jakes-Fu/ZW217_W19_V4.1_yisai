/******************************************************************************
 ** File Name:      diag.c                                                    *
 ** Author:         Richard Yang                                              *
 ** DATE:           22/08/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 22/08/2001     Richard.Yang     Create.                                   *
 ** 19/02/2002     Xueliang.Wang    Modify.                                   *
 ** 20/05/2003     Eric.zhou        Modify.                                   *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_base_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "comm_def.h"
#include "sci_log.h"
#include "keypad.h"
#include "nvitem.h"
#include "layer1_diag.h"
#include "diag.h"
#include "sio.h"
#include "at_common.h"
#include "os_apiext.h"
#include "version.h"
#include "threadx_assert.h"
#include "production_test.h"
#include "efs.h"
#include "chip.h"
#include "nv_item_id.h"
#include "driver_export.h"
#include "flash.h"
#include "mem_dump.h"
#include "ps_diag.h"

#ifdef DUAL_BATTERY_SUPPORT
#include "dualbat_prod.h"
#include "dualbat_drvapi.h"
#endif
#include "ref_param.h"
#include "adi_hal_internal.h"
#include "adc_drvapi.h"
#include "efuse_drvapi.h"

#ifdef SECURE_BOOT_SUPPORT
#include "anti.h"
#include "simage_types.h"
#include "hal_efuse_def.h"
#include "efuse.h"
#include "ap_clk.h"
#include  "lps_apb.h"
#endif

#if defined(SPRD_ENGPC_SUPPORT)
#include "chnl_cp.h"
#include "sprd_fts_type.h"
#include "chnl_pc.h"
#endif

//
// @lin.liu. Add MMI/Current Production Test Register.
//
void Register_MMI_Test_CmdRoutine( void );
void Register_Current_Test_CmdRoutine( void );

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

#define false 0
#define true 1

typedef unsigned char        bool;
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

extern void RM_SetCalibrationMode(BOOLEAN is_calibration);

extern uint32 RM_GetMcuMode(void);
extern uint32 ipc_get_init_status(void);
extern void    RM_Cali_NvAccess_CAL_Mode(uint8     **recv_buffer,uint16 *dest_len,MSG_HEAD_T *recv_msg_ptr);
extern uint8 HALO_factory_CheckLicense(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len);
extern uint8 HALO_factory_ReadUid(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len);
extern uint8 HALO_factory_WriteLicense(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len);

/* LOG GPRS layer data flow*/
#ifdef LOG_FLOW_CONTROL

#define MAX_LAYER_NUM    5

LOCAL uint32 flow_cnt[MAX_LAYER_NUM * 2];
//SCI_TIMER flow_cnt_timer;
SCI_TIMER_PTR flow_cnt_timer;
BOOLEAN  is_timer_on = SCI_TRUE;
#endif

//Add for write product information erase
LOCAL MSG_HEAD_T s_product_info_erase;
LOCAL MSG_HEAD_T s_rf_query_req;

typedef enum {
    WRITE_EFUSE_DISABLE,
    WRITE_EFUSE_ENABLE,
    READ_EFUSE_DISABLE,
    READ_EFUSE_ENABLE,
    EFUSE_ERR
}efuse_flag_t;


typedef enum {
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_31 = 31, /* w217 has 32 adc-channel */
    ADC_CHANNEL_MAX
}adc_channel_t;

LOCAL uint32 write_efuse_flag = WRITE_EFUSE_ENABLE;
LOCAL uint32 read_efuse_flag = READ_EFUSE_DISABLE;

#ifndef WIN32
#ifdef CMMB_SUPPORT
extern void DTLRFVT_HandleToolMsg(uint8       **dest_ptr,
                                  uint16       *dest_len_ptr,
                                  const uint8  *src_ptr,
                                  uint16        src_len);
#endif
#endif
/*lint -save -e668 -e613*/

extern uint32 _CHGMNG_GetChannelVol (ADC_E adc_channel, ADC_SCALE_TYPE_E scale);

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description:    This function parses the command message of the source
//                  and restore the response message to the destination buffer.
//    Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void DisposeMsg(  // None
    uint8 **dest,       // Pointer of the response message.
    uint16 *dest_len,   // Pointer of size of the response message in uin8.
    const uint8 *src,   // Pointer of the input message.
    uint16 src_len);    // Size of the source buffer in uint8.

/**---------------------------------------------------------------------------*
 **                     Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function returns the software version
//                  of the Mobile Station.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void GetSwVersion(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.

/*****************************************************************************/
//  Description:    This function returns the software version
//                  of the Mobile Station.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void GetCpSwVersion(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    This function returns the software version
//                  of the Mobile Station.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void GetLogVersion(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.

/*****************************************************************************/
//  Description:    This function returns the bad command response
//                  back to client.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleBadCommand(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    This function set the auto trace option of
//                  the trace buffer.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void RegisterOk(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    This function disable the auto trace option of
//                  the trace buffer.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void UnRegisterOk(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    This function init the diagnostic task.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
//LOCAL void InitDiag(void); //deleted by Johnson 2006/10/09

/*****************************************************************************/
//  Description:    This function get log filter info of some modules.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void GetLogFilterInfo(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.


/*****************************************************************************/
//  Description:    This function set the log filter of the given module.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void SetLogFilter(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.

LOCAL void PressSoftKey(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.

LOCAL void PowerMS(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.

/*****************************************************************************/
//  Description:    Handle log switch.
//    Global resource dependence:
//  Author:         Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleLogSwitch( // None
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );

#ifdef LOG_FLOW_CONTROL
/*****************************************************************************/
//  Description:    This function initialize flow.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void InitDiagFlow(void);


/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL SCI_CALLBACK CheckFlowCntTimer(
    uint32 id);


/*****************************************************************************/
//  Description:    Handle all AT command.
//    Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void SetFlowLog(     // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len);        // Size of the source buffer in uint8.

#endif // LOG_FLOW_CONTROL


/*****************************************************************************/
//  Description:    Send SIM card information to ChannelServer
//    Global resource dependence:
//  Author: Richard.Yang
//    Note:
/*****************************************************************************/
LOCAL void SendSimInfoToChannelServer(xSignalHeader signal_ptr);

LOCAL void SendSimCmd(
    uint8       **dest_ptr,    // Pointer of the response message
    uint16      *dest_len_ptr, // Pointer of size of the response message in uin8
    const uint8 *src_ptr,      // Pointer of the input message
    uint16      src_len        // Size of the source buffer in uint8
);

// @Xueliang.Wang added(2002-12-31)
/*****************************************************************************/
//  Description:    Handle MCU Read or Write command.
//    Global resource dependence:
//  Author: Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleMcuAccess( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );

/*****************************************************************************/
//  Description:    Check parameters of access MCU command.
//    Global resource dependence:
//  Author:         Xueliang.Wang
//    Note:           If the parameters is valid, return MCU_ACCESS_SUCCESS;
//                  else return error code.
/*****************************************************************************/
LOCAL uint32 CheckMcuAccessParam(
    uint32  start_addr,     // Access start address.
    uint32  size,           // Size to be access.
    uint32  cmd_type,       // MCU_READ or MCU_WRITE
    uint32  access_mode     // BYTE_ACCESS or WORD_ACCESS or DWORD_ACCESS
    );

// @Xueliang.Wang added(2003-01-14)
/*****************************************************************************/
//  Description:    Change MCU mode to "mode", then reset MCU.
//    Global resource dependence:
//  Author: Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleChangeMode(    // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );
// End added.

/*****************************************************************************/
//  Description:    Handle NVItem Read or Write command.
//    Global resource dependence:
//  Author:         Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleNvitemAccess( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );


/*****************************************************************************/
//  Description:    Get MS run mode
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void DIAG_GetMcuMode( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );

/*****************************************************************************/
//  Description:    Handle Pin test command
//    Global resource dependence:
//  Author:         Lin.liu
//    Note:
/*****************************************************************************/
#if 0
LOCAL void HandleProductionTestExtendCommand( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );
#endif
/*****************************************************************************/
//  Description:    Handle power supply command
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void HandleBatteryVoltage( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );
/*****************************************************************************/
//  Description:    Handle ADC value to voltage
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void HandleADCToVoltage( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );
/*****************************************************************************/
//  Description:    callback function for erase customer info completely
//                  completely
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL  void EraseProductInfoCallback( // None
    EFS_ERROR_E     error_code, // efs operation error
    EFS_RESULT         result,     // operation result
    EFS_PARAM         param       // parameter to efs
    );
/*****************************************************************************/
//  Description:    Handle deep sleep command
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void HandleRFQuery( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );
/*****************************************************************************/
//  Description:    Handle layer/ps request
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleLayer1PsReq( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    );

LOCAL void HandleReadADC (// None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
);

/**---------------------------------------------------------------------------*
 **                     Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Main thread of diagnostic task. It manages the command
//                  messages from client application tools and send response
//                  back to channel manager.
//    Global resource dependence:
//  Author: Richard.Yang
//    Note:            It recever messages from CM, DIAG, P_SIM
/*****************************************************************************/
THREAD_ENTRY(DIAG)/*lint -esym(765,thread_entry_DIAG)*/
{
    uint32            status          = 0;
    MSG_HEAD_T        *msg_head       = SCI_NULL;
    xSignalHeader    sig_ptr            = SCI_NULL;
    uint8            *output_msg_ptr = SCI_NULL;
    uint16            output_len      = 0;
    int32           type;
    int ret = 0;

    //InitDiag();   //deleted by Johnson.sun 2006/10/09

#ifdef LOG_FLOW_CONTROL
    InitDiagFlow();
#endif
    //The more conventional form of infinite loop prefix is for(;;)
    //while(1)
    for(;;)
    {
        //status = SCI_GetMessage(DIAG, (void **) &, &src_ID, SCI_WAIT_FOREVER);

        sig_ptr = SCI_GetSignal(DIAG);

        if (SCI_NULL == sig_ptr)    continue;

        switch (sig_ptr->Sender)
        {
            case P_SIM :        // Check if the message is from SIM card(P_SIM).


                //--------------------
                //    xSignalHeader        <--- sig_ptr
                //--------------------
                //    message
                //--------------------
                SendSimInfoToChannelServer(sig_ptr);
                SCI_FREE(sig_ptr);
                continue;

        #ifdef LOG_FLOW_CONTROL

            case DIAG_ACCESS_MCU_MAX_LENGTH:// Check if the message is from DIAG.

                if ((*((uint32 *)(sig_ptr + 1)) == ~SCI_NULL))
                {
                    //--------------------
                    //    xSignalHeader        <--- sig_ptr
                    //--------------------
                    //    (val = ~SCI_NULL)
                    //--------------------
                    SCI_LogFlowCnt(flow_cnt, MAX_LAYER_NUM);
                    SCI_FREE(sig_ptr);
                    continue;
                }

            break;

        #endif
            case P_LAYER1:
#ifndef WIN32
            {
                extern void Layer1_HandleLayer1Signal(xSignalHeader  sig_ptr);

                 Layer1_HandleLayer1Signal(sig_ptr);
            }
#endif
            break;

            default:// Message from CM.

                msg_head = (MSG_HEAD_T *)(sig_ptr + 1);
                type = msg_head->type;
                // if message type is request&response.
                if ((MSG_REQ_REP_START <= type) && (MSG_REQ_REP_END >= type))
                {
                    uint16    size = msg_head->len;

                    output_len = 0;

#ifdef SPRD_ENGPC_SUPPORT
#ifndef WIN32   //bug2073320
                    //add engpc process
                    ret = chnl_diag_trans(&output_msg_ptr, &output_len, (const uint8 *)msg_head, size);
                    if(ret == ENG_DIAG_NO_RESPONSE){
                        goto free; //break;
                    }
                    //if(ret == ENG_DIAG_RET_TO_CP){
                    //    HandleBadCommand(&output_msg_ptr, &output_len, (const uint8 *)msg_head, size);
                    //    break;
                    //}

                    if(output_len > 0 && msg_head->type==0x68){
                        /*if diag with at, then send like this:
                        1. empty diag
                        2. diag response
                        3, diag at ok
                        */
                        pc_write_empty_diag();
                           status = SIO_SendPacket(output_msg_ptr, output_len);
                        SCI_FREE(output_msg_ptr);
                        output_len = 0;

                        pc_write_at_ok();
                        goto free; //break;
                    }
#endif
                    if (output_len == 0)
                    {
#endif //SPRD_ENGPC_SUPPORT
                        // Dispose the command messages.
                        SCI_TraceLow("DisposeMsg...");
                        DisposeMsg(&output_msg_ptr, &output_len, (const uint8 *)msg_head, size);
#ifdef SPRD_ENGPC_SUPPORT
                    }
#endif //SPRD_ENGPC_SUPPORT

                    // In some case, it doesn't return result by output_msg_ptr.
                    if (output_len != 0)
                    {
                        status = SIO_SendPacket(output_msg_ptr, output_len);
                        SCI_FREE(output_msg_ptr);
                        output_len = 0;
                        //status = es_queue_send(DIAG, CM, output_msg_ptr, output_len, SCI_PRI_NORMAL, SCI_NO_WAIT);
                    }

                    //SCI_FREE(sig_ptr);
                }
                else {
                    SCI_TraceLow("diag: type %d error", type);
                }
free:
            SCI_FREE(sig_ptr);
            break;

        }//end of switch
    }
}


// For we not init the s_diag_cmd_routine, and the init-value in ARM and WIN32 is different.
//#ifndef WIN32

//
// Decrisption:
//  When Diag receive a command from remote, It will call the diag command routine callback(if it is not NULL)
//  to allow the client (driver or others) to handle the command first.
//  If the routine return non-zero(that's mean the client has done the command), the diag wouldn't
//  handle the command and output the result(returned by the client) to the remote.
//  If the routine return zero(that's mean the client not handle the command), the diag handle the
//  Command continue.
//
//typedef uint32 (*DIAG_CMD_ROUTINE)( uint8 **dest, uint16 *dest_len, const uint8 *src, uint16 src_len);

static  DIAG_CMD_ROUTINE   s_diag_cmd_routine[REQ_MAX_F] = {NULL};

void DIAG_InitCmdRoutine( void )
{
    int  type = 0;

    for( type = 0; type < REQ_MAX_F; type++ )
    {
        s_diag_cmd_routine[ type ] = NULL;
    }
}

void DIAG_RegisterCmdRoutine( diag_cmd_code_enum_type   type,
    DIAG_CMD_ROUTINE  routine )
{
    if( type < REQ_MAX_F )
        s_diag_cmd_routine[ type ] = routine;
}

void DIAG_UnRegisterCmdRoutine( diag_cmd_code_enum_type type )
{
    if( type < REQ_MAX_F )
        s_diag_cmd_routine[ type ] = NULL;
}
//#endif // WIN32


#ifdef SHELL_DEBUG_SUPPORT
/*
 *********************************************
 ** Added by Eddie.Li for support SHELL. Begin
 *********************************************
 */
#define EXT_CMD_DEBUG 0
#if (EXT_CMD_DEBUG == 1)
#define EXT_CMD_TRACE(_x_) SCI_TraceLow _x_
#define EXT_CMD_ASSERT(_x_) SCI_Assert(_x_)
#else
#define EXT_CMD_TRACE(...)
#endif

LOCAL void EXT_CMD_TRACE_BUF(char * buf, uint32 len)
{
    char temp_buf[64];

    SCI_MEMSET(temp_buf, 0x0, sizeof(temp_buf));
    SCI_MEMCPY(temp_buf, buf, len);
    EXT_CMD_TRACE(("[DIAG] Found the cmd!!\n"));

    EXT_CMD_TRACE(("[DIAG] cmd=%s, len=%d\n", temp_buf, len));
}

LOCAL void SIO_SendPacket_t(void * src, uint32 size)
{
    MEM_PACK_T *info_ptr = (MEM_PACK_T*)src;

    uint32 count = size/sizeof(MEM_PACK_T);

    EXT_CMD_TRACE(("SIO_SendPacket_t: src =0x%08x, mem count=%d\n", src, count));

    for (; count; count-- )
    {
        if(IS_3G_VERSION(info_ptr))
        {
            MEM_INFO_3G_T*  mem_info_ptr = &info_ptr->pack_body.mem_info_3G;/*lint -esym(529,mem_info_ptr)*/
            EXT_CMD_TRACE(("\n\t:No. %04d freg=%01d size=%06d name=%s (line %ld)",
                         mem_info_ptr->block_num, info_ptr->fregment,mem_info_ptr->size, mem_info_ptr->file_name, mem_info_ptr->line));
        }/*lint -esym(529,mem_info_ptr)*/
        else
        {
            MEM_INFO_2G_T*  mem_info_ptr = &info_ptr->pack_body.mem_info_2G;
            EXT_CMD_TRACE(("\n\t:No. %04d freg=%01d size=%06d name=%s (line %ld)",
                         mem_info_ptr->block_num, info_ptr->fregment,mem_info_ptr->size, mem_info_ptr->file_name, mem_info_ptr->line));
        }/*lint -esym(529,mem_info_ptr)*/
    }
}
//test

/*  ----------------------------------------------------------------------------
    _wcsnicmp()

    Case-insensitive compare of two strings.
    NOTE: This version is not sensitive to locale and only handles a-z, A-Z.
*/
LOCAL int
DIAG_csnicmp(const char * pwsz1, const char * pwsz2, int cwch)
{
    char wch1, wch2;
    int iRes = 0;

    if (cwch)
    {
        do
        {
            wch1 = *pwsz1++;
            if (wch1 >= 'A' && wch1 <= 'Z')
            {
                wch1 += 'a' - 'A';
            }

            wch2 = *pwsz2++;
            if (wch2 >= 'A' && wch2 <= 'Z')
            {
                wch2 += 'a' - 'A';
            }
        }
        while (--cwch && wch1 && wch1 == wch2);

        iRes = (int) (wch1 - wch2);
    }

    return iRes;
}

static DIAG_PRIMARY_CMD_LIST_T *diag_pri_cmd_list_header = NULL;

LOCAL int DIAG_PriCmdExisted(DIAG_PRIMARY_CMD_T *pri_cmd)
{
    DIAG_PRIMARY_CMD_LIST_T * pri_cmd_ptr = diag_pri_cmd_list_header;
    int                    result       = FALSE;

    EXT_CMD_TRACE(("[DIAG] DIAG_PriCmdExisted:\n"));

    while (pri_cmd_ptr)
    {
        /* Found the match string in the primary list.      */
        if ( 0 == DIAG_csnicmp(pri_cmd_ptr->regester_cmd->primary_cmd, pri_cmd->primary_cmd, strlen(pri_cmd->primary_cmd)) )
        {
            /* Registered primary cmd has been existed.      */
            EXT_CMD_TRACE_BUF(pri_cmd->primary_cmd, strlen(pri_cmd->primary_cmd));/*lint !e605*/
            result = TRUE;
            break;
        }
        else
        {
            /* Try next item. */
            pri_cmd_ptr = pri_cmd_ptr->next;
        }
    }

    return result;

}

LOCAL int DIAG_SecCmdExisted(DIAG_PRIMARY_CMD_LIST_T * pri_cmd, DIAG_SECONDLY_CMD_T * sec_cmd)
{

    DIAG_SECONDLY_CMD_HANDLER_LIST_T * temp_secondly_cmd_ptr = pri_cmd->cmd_handler_header;
    int                             result                  = FALSE;

    EXT_CMD_TRACE(("[DIAG] DIAG_SecCmdExisted:\n"));

    while (temp_secondly_cmd_ptr)
    {
         /* Found the match string in the primary list.      */
        if ( 0 == DIAG_csnicmp(temp_secondly_cmd_ptr->sec_cmd_handler->secondly_cmd,
                                 sec_cmd->secondly_cmd, strlen(sec_cmd->secondly_cmd)) )
        {
            EXT_CMD_TRACE_BUF(sec_cmd->secondly_cmd, strlen(sec_cmd->secondly_cmd));/*lint !e605*/
            result = TRUE;

            break;
        }
        else
        {
            /* Try next item. */
            temp_secondly_cmd_ptr = temp_secondly_cmd_ptr->next;
        }
    }

    return result;

}

HANDLE DIAG_RegisterPriCmd(DIAG_PRIMARY_CMD_T *reg_cmd)
{
    DIAG_PRIMARY_CMD_LIST_T *primary_cmd;

    /* Primary cmd has been existed and return! */
    if ( DIAG_PriCmdExisted(reg_cmd) )
    {
        return (HANDLE)0;
    }

    primary_cmd = (DIAG_PRIMARY_CMD_LIST_T*)SCI_ALLOC_APP(sizeof(DIAG_PRIMARY_CMD_LIST_T));

    SCI_ASSERT(primary_cmd);/*assert verified*/

    EXT_CMD_TRACE(("[DIAG] DIAG_RegisterPriCmd:\n"));
    EXT_CMD_TRACE(("[DIAG] cmd:%s, help:%s\n", reg_cmd->primary_cmd, reg_cmd->help));

    primary_cmd->regester_cmd         = reg_cmd;
    primary_cmd->cmd_handler_header   = NULL;
    primary_cmd->next                 = NULL;
    primary_cmd->prev                 = NULL;

    /* Not empty list. */
    if ( !diag_pri_cmd_list_header )
    {
       EXT_CMD_TRACE(("[DIAG] DIAG_RegisterPriCmd: the list is empty and add new item in pri cmd list \n"));
       diag_pri_cmd_list_header = primary_cmd;
       primary_cmd->next            = NULL;
       primary_cmd->prev            = NULL;
    }
    else
    {
        EXT_CMD_TRACE(("[DIAG] DIAG_RegisterPriCmd: Add new item in pri cmd list \n"));
        primary_cmd->next                  = diag_pri_cmd_list_header;
        diag_pri_cmd_list_header->prev = primary_cmd;
        diag_pri_cmd_list_header       = primary_cmd;
    }

    /* Debug for trace all registered pri cmd.      */
    {
        DIAG_PRIMARY_CMD_LIST_T *temp_pri_cmd_ptr = diag_pri_cmd_list_header;
        while(temp_pri_cmd_ptr)
        {
            EXT_CMD_TRACE(("[DIAG] DIAG_RegisterPriCmd: Registered Pri CMD = %s \n", temp_pri_cmd_ptr->regester_cmd->primary_cmd));
            temp_pri_cmd_ptr = temp_pri_cmd_ptr->next;
        }

    }

    return (HANDLE)primary_cmd;
}

int32 DIAG_RegisterSecCmd(HANDLE handle, DIAG_SECONDLY_CMD_T *sec_cmd_handler)
{
    DIAG_PRIMARY_CMD_LIST_T             * primary_cmd =
                     (DIAG_PRIMARY_CMD_LIST_T*)handle;

    DIAG_SECONDLY_CMD_HANDLER_LIST_T    * secondly_cmd;
    int32                                 result = FALSE;

    /* The secondly cmd has been existed and return.  */
    if ( DIAG_SecCmdExisted(primary_cmd, sec_cmd_handler) )
    {
        return result;
    }

    secondly_cmd = (DIAG_SECONDLY_CMD_HANDLER_LIST_T*)SCI_ALLOC_APP(sizeof(DIAG_SECONDLY_CMD_HANDLER_LIST_T));

    SCI_ASSERT(secondly_cmd);/*assert verified*/

    EXT_CMD_TRACE(("[DIAG] DIAG_RegisterSecCmd: pri_cmd_ptr=0x%08x, pri_cmd=%s, sec_cmd=%s\n",
               primary_cmd, primary_cmd->regester_cmd->primary_cmd, sec_cmd_handler->secondly_cmd));

    /* Add the secondly command's handler to command list. */
    secondly_cmd->sec_cmd_handler = sec_cmd_handler;

    /* The secondly cmd list is not empty.....  */
    if (primary_cmd->cmd_handler_header)
    {
        /* Add the new registered command. */
        EXT_CMD_TRACE(("[DIAG] DIAG_RegisterSecCmd: add new item.\n"));
        secondly_cmd->next                       = primary_cmd->cmd_handler_header;
        secondly_cmd->prev                       = NULL;
        primary_cmd->cmd_handler_header          = secondly_cmd;
    }
    else
    {
        /* Add the new registered command. */
        EXT_CMD_TRACE(("[DIAG] DIAG_RegisterSecCmd: the list is empty and add new item.\n"));
        primary_cmd->cmd_handler_header       = secondly_cmd;
        secondly_cmd->prev                    = NULL;
        secondly_cmd->next                    = NULL;
    }


    /* Debugging for tracing all registered pri cmd.      */
    {
        DIAG_SECONDLY_CMD_HANDLER_LIST_T *temp_sec_cmd_ptr = primary_cmd->cmd_handler_header;
        while(temp_sec_cmd_ptr)
        {
            EXT_CMD_TRACE(("[DIAG] DIAG_RegisterSecCmd: Registered Sec CMD = %s \n", temp_sec_cmd_ptr->sec_cmd_handler->secondly_cmd));
            temp_sec_cmd_ptr = temp_sec_cmd_ptr->next;
        }

    }

    result = TRUE;

    return result;

}

void DIAG_UnregisterPriCmd(HANDLE handle)
{
    DIAG_PRIMARY_CMD_LIST_T          * primary_cmd =
                     (DIAG_PRIMARY_CMD_LIST_T*)handle;

    DIAG_SECONDLY_CMD_HANDLER_LIST_T * secondly_cmd_ptr, * temp_secondly_cmd_ptr;

    SCI_ASSERT(primary_cmd);/*assert verified*/

    EXT_CMD_TRACE(("[DIAG] DIAG_UnregisterPriCmd: pri_cmd=%s\n",
               primary_cmd->regester_cmd->primary_cmd));


    /* Fetch the first item of secondly cmd.      */
    temp_secondly_cmd_ptr = secondly_cmd_ptr = primary_cmd->cmd_handler_header;

    primary_cmd->cmd_handler_header = NULL;

    while (secondly_cmd_ptr)
    {
        secondly_cmd_ptr = secondly_cmd_ptr->next;
        secondly_cmd_ptr->prev = NULL;

        EXT_CMD_TRACE(("[DIAG] DIAG_UnregisterPriCmd: Del......pri_cmd=%s sec_cmd=%s\n",
               primary_cmd->regester_cmd->primary_cmd, temp_secondly_cmd_ptr->sec_cmd_handler->secondly_cmd));

        SCI_ASSERT(temp_secondly_cmd_ptr);/*assert verified*/

        /* Free one secondly registered cmd. */
        temp_secondly_cmd_ptr->next = NULL;
        temp_secondly_cmd_ptr->prev = NULL;
        SCI_FREE(temp_secondly_cmd_ptr);

        temp_secondly_cmd_ptr = secondly_cmd_ptr;
    }

    EXT_CMD_TRACE(("[DIAG] DIAG_UnregisterPriCmd: secondly cmd list is empty and will free the pri cmd.\n "));


    if (diag_pri_cmd_list_header == primary_cmd)
    {
         /* unregistered item is the first item.      */
        diag_pri_cmd_list_header = primary_cmd->next;
    }
    else
    {
        primary_cmd->prev->next = primary_cmd->next;

        if (primary_cmd->next != NULL)
            primary_cmd->next->prev = primary_cmd->prev;

    }

    /* Free the primary registered cmd.      */
    primary_cmd->next = NULL;
    primary_cmd->prev = NULL;
    SCI_FREE(primary_cmd);

}

void DIAG_SendFormatInfo(const char *x_format, ...)
{
    char       format_str[MAX_STR_SIZE ];
    va_list    args;
    int        nBuf;

    va_start(args, x_format);                                    /*lint !e718 !e64 !e746*/
    nBuf = vsprintf(format_str, x_format, args);
    /* was there an error? */
    SCI_ASSERT((nBuf >= 0) && (nBuf < MAX_STR_SIZE - 1));/*assert verified*/
    /* Was the expanded string too long? */
    va_end(args);                                                            /* lint !e718 */

    DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)format_str, (uint32)nBuf);

}


void DIAG_SendInfoHead(DIAG_SHELL_DATA_TYPE_T type,  uint32 total_len)
{
       DIAG_CONSOLE_CMD_IND_T ind_cmd;
       SCI_ASSERT( type == SHELL_DATA_TEXT || type == SHELL_DATA_BINARY || type == SHELL_DATA_MEM_INFO );/*assert verified*/
       EXT_CMD_TRACE(("[DIAG] DIAG_SendInfoHead: type:0x%X, total_len=%d\r\n", type, total_len));

        ind_cmd.msg_header.len     = sizeof(DIAG_CONSOLE_CMD_IND_T);
        ind_cmd.msg_header.type    = DIAG_CONSOLE_MS;
        ind_cmd.msg_header.subtype = (SHELL_DATA_TEXT == type) ? 0 : 0Xee;
        ind_cmd.msg_type           = SHELL_MSG_IND;
        ind_cmd.data_type          = type;
        ind_cmd.data_len           = total_len;
        ind_cmd.frag               = DIAG_PACKET_HEAD_FALG;
     SIO_SendPacket((uint8*)&ind_cmd, sizeof(DIAG_CONSOLE_CMD_IND_T));
}


void DIAG_SendInfoData(DIAG_SHELL_DATA_TYPE_T type, uint8 * info, uint32 info_len)
{
        DIAG_CONSOLE_CMD_IND_T ind_cmd;
       // uint32     data_len = 0;
       // uint32  len = 0;
        uint8 * data_ptr = PNULL;

        SCI_ASSERT( type == SHELL_DATA_TEXT || type == SHELL_DATA_BINARY || type == SHELL_DATA_MEM_INFO );/*assert verified*/

        if( info_len > 0 )
        {
             SCI_ASSERT(info);/*assert verified*/
        }
        EXT_CMD_TRACE(("[DIAG] DIAG_SendInfoData: type=%d, info_len=%d\n", type, info_len));

        ind_cmd.msg_header.type    = DIAG_CONSOLE_MS;
        ind_cmd.msg_type           = SHELL_MSG_IND;
        ind_cmd.data_type          = type;
        ind_cmd.frag    =  DIAG_PACKET_DATA_FALG;
        ind_cmd.msg_header.len     = sizeof(DIAG_CONSOLE_CMD_IND_T) + info_len;
        ind_cmd.data_len           = info_len;
        ind_cmd.seq_num            = 0;
        ind_cmd.msg_header.subtype = (SHELL_DATA_TEXT == type) ? 0 : 0Xee;
      //  data_len = 0;

     data_ptr = (uint8*)SCI_ALLOC_APP(info_len+sizeof(DIAG_CONSOLE_CMD_IND_T));
     if(data_ptr)
                {
            SCI_MEMCPY(data_ptr, (uint8*)&ind_cmd, sizeof(DIAG_CONSOLE_CMD_IND_T));
            SCI_MEMCPY((data_ptr + sizeof(DIAG_CONSOLE_CMD_IND_T)), info, info_len);
            SIO_SendPacket(data_ptr, (info_len + sizeof(DIAG_CONSOLE_CMD_IND_T)));
         SCI_FREE(data_ptr);
                }
}

void DIAG_SendInfoEnd(DIAG_SHELL_DATA_TYPE_T type)
{
       DIAG_CONSOLE_CMD_IND_T ind_cmd;
       SCI_ASSERT( type == SHELL_DATA_TEXT || type == SHELL_DATA_BINARY || type == SHELL_DATA_MEM_INFO );/*assert verified*/
       EXT_CMD_TRACE(("[DIAG] DIAG_SendInfoEnd: type:0x%X\r\n", type));

        ind_cmd.msg_header.len     = sizeof(DIAG_CONSOLE_CMD_IND_T);
        ind_cmd.msg_header.type    = DIAG_CONSOLE_MS;
        ind_cmd.msg_header.subtype = (SHELL_DATA_TEXT == type) ? 0 : 0Xee;
        ind_cmd.msg_type           = SHELL_MSG_IND;
        ind_cmd.data_type          = type;
        ind_cmd.data_len           = 0;
        ind_cmd.frag               = DIAG_PACKET_END_FALG;
        SIO_SendPacket((uint8*)&ind_cmd, sizeof(DIAG_CONSOLE_CMD_IND_T));
}

void DIAG_SendInfo(DIAG_SHELL_DATA_TYPE_T type, uint8 * info, uint32 info_len)
{
     DIAG_SendInfoHead(type, info_len);
     DIAG_SendInfoData(type, info, info_len);
     DIAG_SendInfoEnd(type);
}


void DIAG_SendScriptInfo(DIAG_SHELL_ERR_CODE_T status)
{
        DIAG_CONSOLE_CMD_IND_T ind_cmd;
     //   uint32                 data_len, len;
    //    uint8                * data_temp_ptr;

        ind_cmd.msg_header.len     = sizeof(DIAG_CONSOLE_CMD_IND_T);
        ind_cmd.msg_header.type    = DIAG_CONSOLE_MS;
        ind_cmd.msg_header.subtype = 0xdd;

        ind_cmd.msg_type           = SHELL_MSG_IND;
        ind_cmd.data_type          = SHELL_DATA_SCRIPTS;
        ind_cmd.data_len           = 0;
        ind_cmd.frag               = DIAG_PACKET_END_FALG; /* Only one packet. */
        ind_cmd.status             = status;

        SIO_SendPacket((uint8*)&ind_cmd, sizeof(DIAG_CONSOLE_CMD_IND_T));

        EXT_CMD_TRACE(("[DIAG] DIAG_SendScriptInfo : send over! status=%d\n", status));
}

LOCAL const char * DIAG_SkipWhitespace(const char * pwsz)
{
    //The more conventional form of
    //infinite loop prefix is for(;;)
    //while (TRUE)
    for(;;)
    {
        switch (*pwsz)
        {
        case 0x9:
        case 0xA:
        case 0xD:
        case 0x20:
            pwsz++;
            continue;
        default:
            break;
        }
        break;
    }

    return pwsz;
}

LOCAL const char * DIAG_SkipNonWhitespace(const char * pwsz)
{
    //The more conventional form of
    //infinite loop prefix is for(;;)
    //while (TRUE)
    for(;;)
    {
        switch (*pwsz)
        {
        case 0:
        case 0x9:
        case 0xA:
        case 0xD:
        case 0x20:
            break;

        default:
            pwsz++;
            continue;
        }
        break;
    }

    return pwsz;
}

#if 0
LOCAL void DIAG_SendRespond(DIAG_PARSE_RESULT_T err_code)
{

    DIAG_CONSOLE_CMD_RES_T   cmd_res;

    /* Send the respond to the tools.                   */
    if(err_code == DIAG_PARSE_PRIMARY_CMD_NOT_FOUND  ||
       err_code == DIAG_PARSE_SECONDLY_CMD_NOT_FOUND ||
       err_code == DIAG_PARSE_TOO_MANY_ARG )
    {
        cmd_res.err_code   = SHELL_CMD_ERROR;
        DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)"Input cmd error!", strlen("Input cmd error!"));
    }
    else
    {
        cmd_res.err_code   = SHELL_SUCCESS;
    }
    cmd_res.msg_type   = SHELL_MSG_RES;

    cmd_res.msg_header.len = sizeof(DIAG_CONSOLE_CMD_REQ_T);
    cmd_res.msg_header.type = DIAG_SYSTEM_F;
    cmd_res.msg_header.subtype = DIAG_CONSOLE_MS;

    /*
    for(i=0; i<sizeof(DIAG_CONSOLE_CMD_RES_T); i++)
    {
        EXT_CMD_TRACE((":0x%02x" , *(((uint8*)&cmd_res)+i) ));
    }

    EXT_CMD_TRACE(("***********"));
    */

    SIO_SendPacket((uint8*)&cmd_res, sizeof(DIAG_CONSOLE_CMD_RES_T));

}
#endif


LOCAL DIAG_PARSE_RESULT_T
DIAG_ParsePrimaryCmd( const char * cmd_start,
                          const char * cmd_end,
                          DIAG_PRIMARY_CMD_LIST_T ** primary_cmd_ptr )
{
    DIAG_PRIMARY_CMD_LIST_T * pri_cmd_ptr = diag_pri_cmd_list_header;
    DIAG_PRIMARY_CMD_LIST_T * pri_cmd_ptr_temp;

    DIAG_PARSE_RESULT_T       result = DIAG_PARSE_PRIMARY_CMD_NOT_FOUND;

    EXT_CMD_TRACE(("[DIAG] DIAG_ParsePrimaryCmd: \n"));

    /* If input string is "help" or "?", directly return! */
    if( 0 == DIAG_csnicmp("help", cmd_start, 4/*(cmd_end-cmd_start)*/) ||\
        0 == DIAG_csnicmp("?", cmd_start, 1/*(cmd_end-cmd_start)*/) )
    {

       pri_cmd_ptr_temp = pri_cmd_ptr;

       while(pri_cmd_ptr_temp)
       {
           DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)pri_cmd_ptr_temp->regester_cmd->help,
                           strlen(pri_cmd_ptr_temp->regester_cmd->help) );
            /* Try next item. */
            pri_cmd_ptr_temp = pri_cmd_ptr_temp->next;
       }
       return DIAG_PARSE_HELP_CMD;
    }


    while (pri_cmd_ptr)
    {
        /* Found the match string in the primary list.      */
        if ( 0 == DIAG_csnicmp(pri_cmd_ptr->regester_cmd->primary_cmd, cmd_start, (cmd_end-cmd_start)) )
        {
            EXT_CMD_TRACE_BUF((char*)cmd_start, (cmd_end-cmd_start));
            *primary_cmd_ptr = pri_cmd_ptr;
            result = DIAG_PARSE_OK;

            break;
        }
        else
        {
            /* Try next item. */
            pri_cmd_ptr = pri_cmd_ptr->next;
        }
    }

    return result;

}

LOCAL DIAG_PARSE_RESULT_T
DIAG_ParseSecondlyCmd( const char * cmd_start,
                           const char * cmd_end,
                           DIAG_PRIMARY_CMD_LIST_T           * primary_cmd_ptr,
                           DIAG_SECONDLY_CMD_HANDLER_LIST_T ** secondly_cmd_ptr )
{

    DIAG_SECONDLY_CMD_HANDLER_LIST_T * temp_secondly_cmd_ptr = primary_cmd_ptr->cmd_handler_header;

    DIAG_PARSE_RESULT_T       result = DIAG_PARSE_SECONDLY_CMD_NOT_FOUND;

    EXT_CMD_TRACE(("[DIAG] DIAG_ParseSecondlyCmd: \n"));

    /* Secondly commmand len is 0, then return not found. */
    if((cmd_end-cmd_start)==0)
    {
        return DIAG_PARSE_SECONDLY_CMD_NOT_FOUND;
    }

    /* If input string is "help" or "?", directly return! */
    if( 0 == DIAG_csnicmp("help", cmd_start, 4/*(cmd_end-cmd_start)*/) ||\
        0 == DIAG_csnicmp("?", cmd_start, 1/*(cmd_end-cmd_start)*/) )
    {
       DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)primary_cmd_ptr->regester_cmd->help,
                       strlen(primary_cmd_ptr->regester_cmd->help) );

       return DIAG_PARSE_HELP_CMD;
    }

    while (temp_secondly_cmd_ptr)
    {
         /* Found the match string in the primary list.      */
        if ( 0 == DIAG_csnicmp(temp_secondly_cmd_ptr->sec_cmd_handler->secondly_cmd, cmd_start, (cmd_end-cmd_start)) )
        {
            EXT_CMD_TRACE_BUF((char *)cmd_start, (cmd_end-cmd_start));
            *secondly_cmd_ptr = temp_secondly_cmd_ptr;
            result = DIAG_PARSE_OK;

            break;
        }
        else
        {
            /* Try next item. */
            temp_secondly_cmd_ptr = temp_secondly_cmd_ptr->next;
        }
    }

    return result;
}

LOCAL DIAG_PARSE_RESULT_T
DIAG_ParseArg(           const char * cmd_start,
                           DIAG_SECONDLY_CMD_HANDLER_LIST_T *secondly_cmd,
                           int        * argc,
                           /*char      ** argv[]*/ char ** argv )
{
    DIAG_PARSE_RESULT_T       result = DIAG_PARSE_OK;

    const char * cmdline_start, * cmdline;
    int  ind = 0;

    cmdline = cmd_start;

    EXT_CMD_TRACE(("[DIAG] DIAG_ParseArg: \n"));

    while (*cmdline != '\0')
    {
        cmdline_start = cmdline = DIAG_SkipWhitespace(cmdline);
        cmdline = DIAG_SkipNonWhitespace(cmdline);

        EXT_CMD_TRACE_BUF((char *)cmdline_start, (cmdline-cmdline_start));

        /* Add one char for storing '\0' */
        argv[ind] = (char*)SCI_ALLOC_APP(cmdline - cmdline_start + 1);
        SCI_ASSERT(argv[ind]);/*assert verified*/

        SCI_MEMSET(argv[ind], 0x0, (cmdline - cmdline_start + 1));
        SCI_MEMCPY(argv[ind], cmdline_start, (cmdline - cmdline_start));

        EXT_CMD_TRACE(("[DIAG] DIAG_ParseArg: arg[%d]=%s \n", ind, argv[ind]));

        ind++;

        if (ind > (int)secondly_cmd->sec_cmd_handler->max_argc)
        {
            /* Free the argv */
            int i;

            for(i=0; i<ind; i++)
            {
                SCI_ASSERT(argv[i]);/*assert verified*/
                SCI_FREE(argv[i]);
            }

            /* argc is 0 */
            *argc = 0;

            result = DIAG_PARSE_TOO_MANY_ARG;
            break;
        }

    }

    *argc = ind;

    return result;
}

DIAG_PARSE_RESULT_T
DIAG_ParseCmdline(const char *cmdline, uint32 cmdline_len)
{
    DIAG_PARSE_RESULT_T       parse_result = DIAG_PARSE_OK;
    const char              * cmdline_start;
    DIAG_PARSING_STATE_T      parsing_state = DIAG_PARSING_PRIMARY_CMD;
    DIAG_PRIMARY_CMD_LIST_T * primary_cmd = NULL;
    DIAG_SECONDLY_CMD_HANDLER_LIST_T  * secondly_cmd = NULL;

    int    ind, argc = 0;
    char * argv[20];

    EXT_CMD_TRACE(("[DIAG] DIAG_ParseCmdline: \n"));

    if(cmdline_len == 0)
    {
        parse_result = DIAG_PARSE_PRIMARY_CMD_NOT_FOUND;
        goto PARSE_OVER;
    }

    SCI_ASSERT(cmdline);/*assert verified*/

    //The more conventional form of
    //infinite loop prefix is for(;;)
    //while (TRUE)
    for(;;)
    {
        cmdline_start = cmdline = DIAG_SkipWhitespace(cmdline);

        if (!cmdline_start)
            break;

        /* Parse failed and break. */
        if (DIAG_PARSE_OK != parse_result)
            break;

        /* Parsing is finished and break. */
        if (DIAG_PARSING_END == parsing_state)
            break;

        switch (parsing_state)
        {
            case DIAG_PARSING_IDLE:
            case DIAG_PARSING_PRIMARY_CMD:

                EXT_CMD_TRACE(("[DIAG] DIAG_ParseCmdline: parse pri cmd \n"));

                cmdline = DIAG_SkipNonWhitespace(cmdline);
                parse_result = DIAG_ParsePrimaryCmd(cmdline_start, cmdline, &primary_cmd);
                parsing_state = DIAG_PARSING_SECONDLY_CMD;

                break;

            case DIAG_PARSING_SECONDLY_CMD:

                EXT_CMD_TRACE(("[DIAG] DIAG_ParseCmdline: parse secondly cmd \n"));

                cmdline = DIAG_SkipNonWhitespace(cmdline);
                parse_result = DIAG_ParseSecondlyCmd(cmdline_start, cmdline, primary_cmd, &secondly_cmd);
                parsing_state = DIAG_PARSING_ARG;

                break;

            case DIAG_PARSING_ARG:

                EXT_CMD_TRACE(("[DIAG] DIAG_ParseCmdline: parse args \n"));

                parse_result = DIAG_ParseArg(cmdline, secondly_cmd, &argc, /*&argv*/argv);
                parsing_state = DIAG_PARSING_END;

                /* Trace the result after the parsing arg. */
                for (ind = 0; ind < argc; ind++)
                {
                    EXT_CMD_TRACE(("[DIAG] DIAG_ParseCmdline: argc=%d, argv=%s \n", argc, argv[ind]));
                }
                break;

            default:
                break;

        }
    }

PARSE_OVER:

    /* Send Respond to tools and tell tools the parse fails. */
    /*DIAG_SendRespond(parse_result);*/

    /* Parsing successfully, and execute the handler. */
    if ( DIAG_PARSE_OK == parse_result )
    {
        (secondly_cmd->sec_cmd_handler->handler)( argc, argv );/*lint !e613*/

        EXT_CMD_TRACE(("[DIAG] DIAG_ParseCmdline: free argv! argc=%d, parse_result=%d, parsing_state= %d\n", argc, parse_result, parsing_state));

        for(ind=0; ind<argc; ind++)
        {
            if(argv[ind])/*lint !e676 */
            {
                SCI_FREE(argv[ind]);  /*lint !e676 */
            }
        }
        return parse_result;
    }
    else if ( DIAG_PARSE_HELP_CMD == parse_result )
    {
        EXT_CMD_TRACE(("[DIAG] DIAG_ParseCmdline: A Help cmd!\n"));
        return parse_result;
    }
    else
    {
        EXT_CMD_TRACE(("[DIAG] DIAG_ParseCmdline: parse failed!, parse_result=%d, parsing_state= %d\n",parse_result, parsing_state));
        DIAG_SendInfo(SHELL_DATA_TEXT, (uint8*)"Cmd input error! Pls input 'help' or '?'", strlen("Cmd input error! Pls input 'help' or '?'"));
        return parse_result;
    }

}

LOCAL void DIAG_DisposeShellCmd(void * sh_cmd)
{

    DIAG_CONSOLE_CMD_REQ_T * cmd_req = (DIAG_CONSOLE_CMD_REQ_T*)sh_cmd;
    DIAG_CONSOLE_CMD_RES_T   cmd_res;
    DIAG_PARSE_RESULT_T      parse_result;
    uint8                  * ptr_tmp;
    uint32                   cmdline_len = cmd_req->cmd_line_len;
    char                   * cmdline = (char*)(cmd_req + 1);
    int                         i;
    ptr_tmp = (uint8*)sh_cmd;

#if 1
    /* Send the respond to the tools.                   */
    cmd_res.err_code   = SHELL_SUCCESS;
    cmd_res.msg_type   = SHELL_MSG_RES;

    cmd_res.msg_header = *(MSG_HEAD_T*)cmd_req;
    cmd_res.msg_header.len = sizeof(DIAG_CONSOLE_CMD_REQ_T);
    cmd_res.msg_header.type = DIAG_SYSTEM_F;
    cmd_res.msg_header.subtype = DIAG_CONSOLE_MS;

    /*
    for(i=0; i<sizeof(DIAG_CONSOLE_CMD_RES_T); i++)
    {
        EXT_CMD_TRACE((":0x%02x" , *(((uint8*)&cmd_res)+i) ));
    }

    EXT_CMD_TRACE(("***********"));
    */

    SIO_SendPacket((uint8*)&cmd_res, sizeof(DIAG_CONSOLE_CMD_RES_T));
#endif


    EXT_CMD_TRACE(("[DIAG] DIAG_DisposeShellCmd: cmdline_len=%d,msg_type=%d:\n", cmdline_len, cmd_req->msg_type ));

    for(i = 0; i < (int)cmdline_len && i<20; i++)
    {
        EXT_CMD_TRACE((":0x%02x" , *(cmdline+i) ));
    }
    EXT_CMD_TRACE(("***********"));


    /* Parse the cmdline received from tools.           */
    parse_result = DIAG_ParseCmdline(cmdline, cmdline_len);

    EXT_CMD_TRACE(("[DIAG] DIAG_DisposeShellCmd: Parse finished and result=%d.\n", parse_result));

}

/*
 *******************************************
 ** Added by Eddie.Li for support SHELL End
 *******************************************
 */
#endif /* SHELL_DEBUG_SUPPORT*/


/*****************************************************************************/
//  Description:    This function parses the command message of the source
//                  and restore the response message to the destination buffer.
//    Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//    Note:
/*****************************************************************************/

#ifndef PLATFORM_UWS6121E

LOCAL void HandleReadEfuseReq(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len)
{
    EFUSE_RETURN_E ret = EFUSE_RESULT_SUCCESS;
        uint16 uId;
        uint32 uData;
    uint32 length;
    MSG_HEAD_T *msg_head = NULL;
    uId = *(uint16 *)(src_ptr + sizeof(MSG_HEAD_T));

    SCI_TRACE_LOW("DATA uID = %d\r\n", uId);

#ifdef SECURE_BOOT_SUPPORT
#if defined(PLATFORM_SC6531EFM)
    if((uId >= 2) && (uId <=6))
    {//hash id 2~6 from PC tool switch to hash id 9~13
        uId = uId + 7;
        SCI_TRACE_LOW("DATA switched uID = %d\r\n", uId);
    }
#endif
#endif

#ifndef WIN32
    EFUSE_HAL_Open();

    if(ret == EFUSE_HAL_Read((uint32)uId, &uData))
    {
        length = 6;
    } else {
        length = 1;
    }

    EFUSE_HAL_Close();
#endif

    SCI_TRACE_LOW("EFUSE_HAL_Read = %d\r\n", ret);
    SCI_TRACE_LOW("DATA uDATA = %x\r\n", uData);
    SCI_TRACE_LOW("DATA length = %d\r\n", length);

    msg_head = (MSG_HEAD_T *)SCI_ALLOC(length + sizeof(MSG_HEAD_T));
    *dest_ptr = (uint8 *)msg_head;

    *dest_len_ptr = length + sizeof(MSG_HEAD_T);
    SCI_MEMCPY((void*)msg_head, src_ptr, sizeof(MSG_HEAD_T));

    if(length == 1)
    {
        SCI_MEMSET((void *)(msg_head + 1), 0x00, length);
    }
    else
    {
#ifdef SECURE_BOOT_SUPPORT
#if defined(PLATFORM_SC6531EFM)
                if((uId >= 9) && (uId <=13))
        {//hash id 9~13 switch to hash id 2~6 to PC tool
            uId = uId - 7;
        }
#endif
#endif
        SCI_MEMCPY((uint8 *)(msg_head + 1), (uint8 *)&uId, sizeof(uint16));
        SCI_MEMCPY((uint8 *)(msg_head + 1) + 2, (uint8 *)&uData, sizeof(uint32));
    }

    msg_head->len = *dest_len_ptr;
    SCI_TRACE_LOW("msg_head_seq_num=%d",msg_head->seq_num);
    SCI_TRACE_LOW("msg_head_len=%d",msg_head->len);
    SCI_TRACE_LOW("msg_head_type=%d",msg_head->type);
    SCI_TRACE_LOW("msg_head_subtype=%d",msg_head->subtype);
}
#else // #define PLATFORM_UWS6121E
#ifdef SECURE_BOOT_SUPPORT
typedef struct
{
    uint8 flag;
    uint8 isenable;
} SecFlag;
static SecFlag SecEnable = {0x5A, 0};
#define OSI_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define SIMAGE_HEADER_CHECK_TYPE(type) ((type)&0xf)       // header->header_flags
#define SIMAGE_DATA_CRYPT_TYPE(type) ((type)&0xf)         // header->data_flags
#define SIMAGE_CERT_PKA_TYPE(type) ((type)&0xf)           // cert->pk_type
#define SIMAGE_CERT_HASH_TYPE(type) (((type) >> 8) & 0xf) // cert->pk_type
#define SIMAGE_CERT_ENABLE_DOWNLOAD_MASK (1U << 31)
#define OSI_ALIGN_UP(v, n) (((unsigned long)(v) + (n)-1) & ~((n)-1))

static bool prvCheckChecksum(unsigned type, const osiBits256_t *check, const void *data, unsigned size)
{
    osiBits256_t calc;
    if (size == 0) // not to check checksum for empty
        return true;

    memset(&calc, 0, sizeof(calc));
    if (type == SIMAGE_HEADER_CHECK_CRC)
    {
        unsigned crc = crc32Calc(data, size);
        memcpy(&calc, &crc, 4);
    }
    else if (type == SIMAGE_HEADER_CHECK_SHA256)
    {
        if (!sha256Calc(data, size, &calc))
            return false;
    }
#ifdef CONFIG_SIMAGE_HEADER_SM3_SUPPORT
    else if (type == SIMAGE_HEADER_CHECK_SM3)
    {
        if (!sm3Calc(data, size, &calc))
            return false;
    }
#endif
    else
    {
        return false;
    }

    return memcmp(&calc, check, sizeof(calc)) == 0;
}
bool simageHeaderValid(const simageHeader_t *header, unsigned max_size)
{
   unsigned check_type = 0;
   unsigned expected = 0;
   const char *header_data = NULL;
   unsigned header_data_len = 0;

    if (header == NULL)
        return false;

    check_type = SIMAGE_HEADER_CHECK_TYPE(header->header_flags);

    if (header->magic != SIMAGE_HEADER_MAGIC ||
        header->header_size != SIMAGE_HEADER_SIZE ||
        header->format_version == 0 ||
        header->format_version > SIMAGE_FORMAT_VERSION_V2)
        return false;

    if (header->image_size > max_size)
        return false;

#define CHECK_LAYOUT(offset, size)                        \
    if (size > 0)                                         \
    {                                                     \
        if (offset != expected)                           \
            return false;                                 \
        expected += OSI_ALIGN_UP(size, SIMAGE_ALIGNMENT); \
    }

    // the layout is fixed
    expected = header->header_size;
    CHECK_LAYOUT(header->data_offset, header->data_size);
    CHECK_LAYOUT(header->priv_data_offset, header->priv_data_size);
    CHECK_LAYOUT(header->keycert_offset, header->keycert_size);
    CHECK_LAYOUT(header->privcert_offset, header->privcert_size);
    CHECK_LAYOUT(header->debugcert_offset, header->debugcert_size);
    CHECK_LAYOUT(header->develcert_offset, header->develcert_size);
    CHECK_LAYOUT(header->chaincert_offset, header->chaincert_size);

    // no other contents
    if (expected != header->image_size)
        return false;

    header_data = (const char *)header + SIMAGE_HEADER_CHECKSUM_OFFSET;
    header_data_len = header->header_size - SIMAGE_HEADER_CHECKSUM_OFFSET;
    if (!prvCheckChecksum(check_type, &header->header_checksum, header_data, header_data_len))
        return false;

    return true;
}

bool simagePayloadValid(const simageHeader_t *header)
{
    const char *payload = (const char *)header + header->header_size;
    unsigned payload_len = header->image_size - header->header_size;
    unsigned check_type = SIMAGE_HEADER_CHECK_TYPE(header->header_flags);
    if (!prvCheckChecksum(check_type, &header->payload_checksum, payload, payload_len))
        return false;

    return true;
}
/**
 * \brief helper to get chaincert by simage header
 *
 * \param header simage header
 * \return chaincert, NULL for not exist
 */
static  const simageChainCert_t *simageGetChainCert(const simageHeader_t *header)
{
    uint32 ptr = 0;
    if (header->chaincert_offset == 0)
        return NULL;

    ptr = (uint32)header + header->chaincert_offset;
    return (const simageChainCert_t *)ptr;
}
/**
 * \brief helper to get keycert by simage header
 *
 * \param header simage header
 * \return keycert, NULL for not exist
 */
static  const simageKeyCert_t *simageGetKeyCert(const simageHeader_t *header)
{
    uint32 ptr = 0;
    if (header->keycert_offset == 0)
        return NULL;

    ptr = (uint32)header + header->keycert_offset;
    return (const simageKeyCert_t *)ptr;
}
static bool prvCalcHash(uint32 type, const void *data, unsigned size, osiBits256_t *hash)
{
    if (type == SIMAGE_HASHTYPE_SHA256)
        return halRomSha256Calc(data, size, hash);

    if (type == SIMAGE_HASHTYPE_SM3)
    {
        return halRomSm3Calc(data, size, hash);
    }

    return false;
}
static unsigned prvPubkeySize(uint32 type)
{
    if (type == SIMAGE_PKATYPE_ECC)
        return sizeof(simageEccPubkey_t);

    if (type == SIMAGE_PKATYPE_SM2)
        return sizeof(simageSm2Pubkey_t);

    if (type == SIMAGE_PKATYPE_RSA || type == SIMAGE_PKATYPE_RSA_PSS)
        return sizeof(simageRsaPubkey_t);

    return 0;
}
bool simageCertHashMatch(const simageCertHeader_t *cert, const osiBits256_t *expected, const void *data, unsigned size)
{
    unsigned hash_type = SIMAGE_CERT_HASH_TYPE(cert->pk_type);
    osiBits256_t hash;
    if (!prvCalcHash(hash_type, data, size, &hash))
        return false;

    return memcmp(expected, &hash, sizeof(osiBits256_t)) == 0;
}
bool simageCertVerify(const simageCertHeader_t *cert, const osiBits256_t *pubkey_hash)
{
    unsigned pka_type = SIMAGE_CERT_PKA_TYPE(cert->pk_type);
    unsigned hash_type = SIMAGE_CERT_HASH_TYPE(cert->pk_type);
    const void *cert_data = (const char *)cert + SIMAGE_SIGN_DATA_OFFSET;
    unsigned cert_data_size = cert->cert_size - SIMAGE_SIGN_DATA_OFFSET;
    osiBits256_t hash;

    // pubkey hash check is optional
    if (pubkey_hash != NULL)
    {
        if (!simageCertHashMatch(cert, pubkey_hash, &cert->pubkey,
                                 prvPubkeySize(pka_type)))
            return false;
    }


    if (!prvCalcHash(hash_type, cert_data, cert_data_size, &hash))
        return false;

    if (pka_type == SIMAGE_PKATYPE_RSA || pka_type == SIMAGE_PKATYPE_RSA_PSS)
    {
      return halRomRsaVerify(cert->pubkey.rsa.n_bytes, cert->pubkey.rsa.e,
                         &cert->pubkey.rsa.n, cert->pk_type,
                         &cert->sig.rsa.sig, cert->sig.rsa.bytes,
                         &hash, sizeof(hash));
    }

    else if (pka_type == SIMAGE_PKATYPE_ECC)
    {
         return halRomEccVerify(cert->pubkey.ecc.curve,
                         &cert->pubkey.ecc.x, sizeof(cert->pubkey.ecc.x),
                         &cert->pubkey.ecc.y, sizeof(cert->pubkey.ecc.y),
                         &cert->sig.ecc.r, sizeof(cert->sig.ecc.r),
                         &cert->sig.ecc.s, sizeof(cert->sig.ecc.s),
                         &hash, sizeof(hash));
    }
    else if (pka_type == SIMAGE_PKATYPE_SM2)
    {
    return halRomSm2Verify(&cert->pubkey.ecc.x, &cert->pubkey.ecc.y,
                         &cert->sig.ecc.r, &cert->sig.ecc.s,
                         &hash, sizeof(hash));
    }
    else
    {
        return false;
    }
    return true;
}
bool simageValid(const simageHeader_t *header, unsigned max_size)
{
    return simageHeaderValid(header, max_size) && simagePayloadValid(header);
}

bool simageCalcRotPK(const simageHeader_t *header, osiBits256_t *rotpk)
{
    const simageChainCert_t *chaincert = simageGetChainCert(header);
    const simageKeyCert_t *keycert = simageGetKeyCert(header);
    if (chaincert != NULL)
    {
        unsigned pka_type = SIMAGE_CERT_PKA_TYPE(chaincert->header.pk_type);
        unsigned hash_type = SIMAGE_CERT_HASH_TYPE(chaincert->header.pk_type);
        return prvCalcHash(hash_type, &chaincert->header.pubkey,
                           prvPubkeySize(pka_type), rotpk);
    }

    if (keycert != NULL)
    {
        unsigned pka_type = SIMAGE_CERT_PKA_TYPE(keycert->header.pk_type);
        unsigned hash_type = SIMAGE_CERT_HASH_TYPE(keycert->header.pk_type);

        return prvCalcHash(hash_type, &keycert->header.pubkey,
                           prvPubkeySize(pka_type), rotpk);
    }
    return false;
}
static  const simageDebugCert_t *simageGetDebugCert(const simageHeader_t *header)
{
    uint32 ptr = 0;
    if (header->debugcert_offset == 0)
        return NULL;

    ptr = (uint32)header + header->debugcert_offset;
    return (const simageDebugCert_t *)ptr;
}
static const simageDevelCert_t *simageGetDevelCert(const simageHeader_t *header)
{
   uint32 ptr = 0;
    if (header->develcert_offset == 0)
        return NULL;

    ptr = (uint32)header + header->develcert_offset;
    return (const simageDevelCert_t *)ptr;
}
bool simageSignVerify(const simageHeader_t *header,
                      const osiBits256_t *rotpk,
                      const osiBits64_t *soc_raw_id,
                      unsigned min_secure_version)
{
    simageKeyCert_t *keycert = NULL;
    simageChainCert_t *chaincert = NULL;
    char *payload = NULL;
    simageDebugCert_t *debugcert = NULL;
    simageDevelCert_t *develcert = NULL;

    if (header == NULL || rotpk == NULL)
        return false;

    keycert = simageGetKeyCert(header);
    if (keycert == NULL ||
        keycert->header.cert_type != SIMAGE_CERTTYPE_KEY ||
        keycert->header.cert_size != sizeof(simageKeyCert_t))
        return false;

    chaincert = simageGetChainCert(header);
    if (chaincert != NULL)
    {
        if ((chaincert->header.cert_type != SIMAGE_CERTTYPE_CHAIN &&
             chaincert->header.cert_type != SIMAGE_CERTTYPE_CHAIN_SOC) ||
            chaincert->header.cert_size != sizeof(simageChainCert_t))
            return false;

        if (!simageCertVerify(&chaincert->header, rotpk))
            return false;

        if (chaincert->header.cert_type == SIMAGE_CERTTYPE_CHAIN_SOC)
        {
            if (soc_raw_id == NULL)
                return false;

            if (!simageCertHashMatch(&chaincert->header, &chaincert->soc_id,
                                     soc_raw_id, sizeof(*soc_raw_id)))
                return false;
        }

        rotpk = &chaincert->hash_next_key;
    }

    if (!simageCertVerify(&keycert->header, rotpk))
        return false;

    if (keycert->security_version < min_secure_version)
        return false;

    if (keycert->data_size != header->data_size ||
        keycert->data_flags != header->data_flags ||
        keycert->data_load != header->data_load ||
        keycert->data_load_hi != header->data_load_hi ||
        keycert->data_entry != header->data_entry ||
        keycert->data_entry_hi != header->data_entry_hi)
        return false;

    payload = (const char *)header + header->data_offset;
    if (!simageCertHashMatch(&keycert->header, &keycert->hash_data,
                             payload, header->data_size))
        return false;

    debugcert = simageGetDebugCert(header);
    if (debugcert != NULL)
    {
        if (debugcert->header.cert_type != SIMAGE_CERTTYPE_DEBUG ||
            debugcert->header.cert_size != sizeof(simageDebugCert_t))
            return false;

        if (!simageCertVerify(&debugcert->header, &keycert->hash_next_key))
            return false;
    }

    develcert = simageGetDevelCert(header);
    if (develcert != NULL)
    {
        if (debugcert == NULL)
            return false;

        if (develcert->header.cert_type != SIMAGE_CERTTYPE_DEVELOP ||
            develcert->header.cert_size != sizeof(simageDevelCert_t))
            return false;

        if (!simageCertVerify(&develcert->header, &debugcert->hash_devel_key))
            return false;

        if (soc_raw_id == NULL)
            return false;

        if (!simageCertHashMatch(&develcert->header, &develcert->soc_id,
                                 soc_raw_id, sizeof(*soc_raw_id)))
            return false;
    }

    return true;
}

/**
* \brief set a bit in bitmap
*
* Bitmap will use 32bits word, and each word holds 32 bits.
* Caller should ensure parameters are valid.
*
* \param bitmap        word array for bitmap
* \param n             the bit to be set
*/
static  void osiBitmapSet(uint32 *bitmap, int n)
{
    bitmap[n / (sizeof(uint32) * 8)] |= (1 << (n % (sizeof(uint32) * 8)));
}

int diag_builtin_clz(U32 x)
{
                int n = 0;
                x |=1;
                while(!(x & 0x80000000))
                {
                                n++;
                                x <<= 1;
                }
                return n;
}

/**
* Version support 0 - 64, each bit for a version
*      - version 0    0x00000000 00000000
*      - version 1    0x00000000 00000001
*      - version 32   0x00000000 ffffffff
*      - version 34   0x00000003 ffffffff
*      - version 64   0xffffffff ffffffff
*/
unsigned halFromEfuseAntiRollback64(const uint32 verbits[2])
{
    if (verbits[1] != 0)
        return 64 - diag_builtin_clz(verbits[1]);
    if (verbits[0] != 0)
        return 32 - diag_builtin_clz(verbits[0]);
    return 0;
}

bool halToEfuseAntiRollback64(unsigned ver, uint32 verbits[2])
{
    int n = 0;
    if (ver > 64)
        return false;

    verbits[0] = verbits[1] = 0;
    for (n = 0; n < ver; n++)
        osiBitmapSet(verbits, n);

    return true;
}


bool drvSecureBootEnable(void)
{
    halEfuseLock3_t lock3;
    halEfuseBonding_t bonding;
    halEfuseRma_t rma;
    halEfuseReadOp_t efuse_ops[3];

    efuse_ops[0].op = HAL_EFUSE_OP_SINGLE_READ;
    efuse_ops[0].block_index = HAL_EFUSE_SINGLE_BLOCK_LOCK_3;
    efuse_ops[0].rval = &lock3.v;

    efuse_ops[1].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[1].block_index = HAL_EFUSE_DOUBLE_BLOCK_RMA;
    efuse_ops[1].rval = &rma.v;

    efuse_ops[2].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[2].block_index = HAL_EFUSE_DOUBLE_BLOCK_BONDING;
    efuse_ops[2].rval = &bonding.v;

    if (SecEnable.flag != 0x5A)
    {
        return SecEnable.isenable;
    }


    if (!halEfuseRead(efuse_ops, OSI_ARRAY_SIZE(efuse_ops)))
        return false;

    SecEnable.isenable = bonding.b.enable_secure_boot == 1 &&
                         lock3.b.rotpk == 0xffff && rma.v != 0x7f;
    SecEnable.flag = 0xA5;

    return SecEnable.isenable;
}

#define  CONFIG_BOOT_FLASH_ADDRESS  0x60000000
#define CONFIG_BOOT_FLASH_SIZE 0x14000

//#define ROM_TEST1
#ifdef ROM_TEST1
uint32 g_rotpk[8] = {0};
uint32 g_virbits[2] = {0};
uint32 g_bonding = 0;
uint32 g_lock3 = 0;
#endif
bool drvSecureWriteSecureFlags(void)
{
    halEfuseBonding_t bonding;
    osiBits256_t rotpk;
    const simageHeader_t *header = (const simageHeader_t *)CONFIG_BOOT_FLASH_ADDRESS;
    uint32 verbits[2] = {0};
    uint32 sn[2] = {0};
    halEfuseLock0_t lock0 = {0};
    halEfuseLock3_t lock3 = {0};
    halEfuseLock5_t lock5 = {0};
       const simageKeyCert_t *keycert = NULL;
    unsigned efuse_ver = 0;
    uint32 *rotpk_words = NULL;
    halEfuseReadOp_t efuse_ops1[7];
    halEfuseWriteOp_t efuse_ops2[12];

          efuse_ops1[0].op = HAL_EFUSE_OP_SINGLE_READ;
       efuse_ops1[0].block_index = HAL_EFUSE_SINGLE_BLOCK_LOCK_0;
       efuse_ops1[0].rval = &lock0.v;

       efuse_ops1[1].op = HAL_EFUSE_OP_SINGLE_READ;
       efuse_ops1[1].block_index = HAL_EFUSE_SINGLE_BLOCK_LOCK_3;
       efuse_ops1[1].rval = &lock3.v;

       efuse_ops1[2].op = HAL_EFUSE_OP_SINGLE_READ;
       efuse_ops1[2].block_index = HAL_EFUSE_SINGLE_BLOCK_LOCK_5;
       efuse_ops1[2].rval = &lock5.v;

       efuse_ops1[3].op = HAL_EFUSE_OP_SINGLE_READ;
       efuse_ops1[3].block_index = HAL_EFUSE_SINGLE_BLOCK_UID_0;
       efuse_ops1[3].rval = &sn[0];

       efuse_ops1[4].op = HAL_EFUSE_OP_SINGLE_READ;
       efuse_ops1[4].block_index = HAL_EFUSE_SINGLE_BLOCK_UID_1;
       efuse_ops1[4].rval = &sn[1];

       efuse_ops1[5].op = HAL_EFUSE_OP_DOUBLE_READ;
       efuse_ops1[5].block_index = HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0;
       efuse_ops1[5].rval = &verbits[0];

       efuse_ops1[6].op = HAL_EFUSE_OP_DOUBLE_READ;
       efuse_ops1[6].block_index = HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1;
       efuse_ops1[6].rval = &verbits[1];


    // Return true if secure boot is already enabled.
    if (drvSecureBootEnable())
    {
        SCI_TraceLow("secure boot is already enabled");
        return true;
    }
    SCI_TRACE_LOW("drvSecureBootEnable. OK");
    // Make sure bootloader contains valid key cert

    if (!simageValid(header, CONFIG_BOOT_FLASH_SIZE))
    {
        SCI_TRACE_LOW("bootloader is invalid");
        return false;
    }
SCI_TRACE_LOW("simageValid. OK");
    // Calculate rotpk from bootloader key cert

    if (!simageCalcRotPK(header, &rotpk))
    {
        SCI_TRACE_LOW("failed to calculate RoTPK");
    SCI_ASSERT(0);
        return false;
    }
SCI_TRACE_LOW("simageCalcRotPK. OK");

    if (!halEfuseRead(efuse_ops1, OSI_ARRAY_SIZE(efuse_ops1)))
    {
        SCI_TRACE_LOW( "efuse read failed");
        SCI_ASSERT(0);
        return false;
    }
 SCI_TRACE_LOW("SECUREBOOT: udi:%x, %x\n", sn[0],sn[1]);
    // Verify image.
    if (!simageSignVerify(header, &rotpk, (osiBits64_t *)sn, 0))
    {
        SCI_TRACE_LOW( "bootloader signature is invalid");
        SCI_ASSERT(0);
        return false;
    }
    keycert = simageGetKeyCert(header);
    if (keycert == NULL)
    {
    SCI_ASSERT(0);
        return false;
    }

    // Though not possible usually, check bootloader secure version
    efuse_ver = halFromEfuseAntiRollback64(verbits);
    if (keycert->security_version < efuse_ver)
    {
        SCI_TRACE_LOW("bootloader secure version too small, %d/%d",
                 keycert->security_version, efuse_ver);
        return false;
    }

    // Calculate anti-rollback efuse value
    if (!halToEfuseAntiRollback64(keycert->security_version, verbits))
        return false;

    // The efuse words to be written shouldn't be locked
    if (lock0.b.lock3 != 0 ||
        lock0.b.lock5 != 0 ||
        lock3.b.rotpk != 0 ||
        lock5.b.secure_counter1 != 0)
        return false;

    // Secure boot enable is NOT power failure safe. It will be safer to
    // disable interrupt for the whole, though it flash operation will
    // take time.
    //unsigned critical = osiEnterCritical();

    // To avoid integrity check break, bonding will be locked after write.
    // So, if there are other bits are needed to be written, should be
    // set here.


    bonding.b.enable_secure_boot = 1;
    bonding.b.disable_fdma = 1;
    bonding.b.disable_med_soft_key = 1;
    bonding.b.disable_ptest = 1;
    bonding.b.disable_swd_jtag = 1;
    bonding.b.disable_apa5_debug = 1;
    bonding.b.disable_cpa5_debug = 1;
    bonding.b.disable_swd_dbg_sys = 1;
    bonding.b.disable_djtag = 1;
    bonding.b.disable_pad_jtag = 1;
    bonding.b.disable_dbghost = 1;
    bonding.b.disable_sec_dap = 1;

#ifdef CONFIG_DUAL_BOOTLOADER_ENABLE
    bonding.b.boot_sector_count = (CONFIG_BOOT_FLASH_SIZE / 0x1000);
#endif

    lock3.b.rotpk = 0xffff;

    rotpk_words = (uint32 *)&rotpk;
    /*2*/
       efuse_ops2[0].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[0].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0;
       efuse_ops2[0].wval = rotpk_words[0];

       efuse_ops2[1].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[1].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_1;
       efuse_ops2[1].wval = rotpk_words[1];
       efuse_ops2[2].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[2].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_2;
       efuse_ops2[2].wval = rotpk_words[2];
       efuse_ops2[3].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[3].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_3;
       efuse_ops2[3].wval = rotpk_words[3];
       efuse_ops2[4].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[4].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_4;
       efuse_ops2[4].wval = rotpk_words[4];
       efuse_ops2[5].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[5].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_5;
       efuse_ops2[5].wval = rotpk_words[5];
       efuse_ops2[6].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[6].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_6;
       efuse_ops2[6].wval = rotpk_words[6];
       efuse_ops2[7].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[7].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_7;
       efuse_ops2[7].wval = rotpk_words[7];
       efuse_ops2[8].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[8].block_index = HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0;
       efuse_ops2[8].wval = verbits[0];
       efuse_ops2[9].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[9].block_index = HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1;
       efuse_ops2[9].wval = verbits[1];
       efuse_ops2[10].op = HAL_EFUSE_OP_DOUBLE_WRITE;
       efuse_ops2[10].block_index = HAL_EFUSE_DOUBLE_BLOCK_BONDING;
       efuse_ops2[10].wval =  bonding.v;
       efuse_ops2[11].op = HAL_EFUSE_OP_SINGLE_WRITE;
       efuse_ops2[11].block_index = HAL_EFUSE_SINGLE_BLOCK_LOCK_3;
       efuse_ops2[11].wval =  lock3.v;

#ifdef ROM_TEST1
         g_rotpk[0] = rotpk_words[0];
         g_rotpk[1] = rotpk_words[1];
      g_rotpk[2] = rotpk_words[2];
         g_rotpk[3] = rotpk_words[3];
      g_rotpk[4] = rotpk_words[4];
         g_rotpk[5] = rotpk_words[5];
      g_rotpk[6] = rotpk_words[6];
         g_rotpk[7] = rotpk_words[7];
         g_virbits[0] = verbits[0];
         g_virbits[1] = verbits[1];
         g_bonding = bonding.v;
         g_lock3 = lock3.v;
#endif

    if (!halEfuseWrite(efuse_ops2, OSI_ARRAY_SIZE(efuse_ops2)))
        goto failed_unlock;

    //osiExitCritical(critical);
    return true;

failed_unlock:
   // osiExitCritical(critical);
    return false;
}
//#define  ROM_TEST
#ifdef ROM_TEST
/**
 * NOR FLASH physical address
 */
#define CONFIG_NOR_PHY_ADDRESS 0x60000000

#define WARM_BOOT_MAGIC (0x4A554D50)     // JUMP
#define WARM_BOOT_MED_MAGIC (0x4A554D45) // JUME
#define WARM_BOOT_MAGIC_REG (hwp_idleLps->lps_res0)
#define WARM_BOOT_ADDRESS_REG (hwp_idleLps->lps_res1)

typedef struct
{
    bool secure_boot_enable;      ///< whether secure is enabled
    bool develcert_enable;        ///< whether develcert is enabled for secure debug
    bool download_enable;         ///< whether download is enabled, for disable download
    bool keypad_entermode_enable; ///< whether to enable download mode check by keypad
    bool uart1_boot_enable;       ///< whether uart1 will be polled during download
    bool uart4_boot_enable;       ///< whether uart4 will be polled during download
    bool usb_boot_enable;         ///< whether usb will be polled during download
    bool sdcard_boot_enable;      ///< whether sdcard boot is enabled, for disable sdcard boot
    bool boot_med_encrypt;        ///< whether bootloader is MED encrypted
    bool enable_faster_flash;     ///< whether enable faster flash configuration
    bool sm_certify_mode;         ///< whether iSM certification mode
    bool rma_med;                 ///< whether rma_med is set
    bool rma_all;                 ///< whether all rma bits are set
    uint32_t second_boot_offset;  ///< second bootloader offset for dual bootloader, in bytes
    unsigned min_secure_version;  ///< minimal valid security version
    osiBits64_t soc_raw_id;       ///< raw SOC ID
    osiBits256_t rotpk;           ///< root trusted public key hash
    osiBits256_t data_hash;       ///< data hash for sm certify
} romEfuseContent_t;

romEfuseContent_t gRomEfuse = {0};

unsigned romFromEfuseAntiRollback(const uint32_t verbits[2])
{
    if (verbits[1] != 0)
        return 64 - __builtin_clz(verbits[1]);
    if (verbits[0] != 0)
        return 32 - __builtin_clz(verbits[0]);
    return 0;
}
void romReadEfuseContent(void)
{
    halEfuseRma_t efuse_rma = {0};
    halEfuseBonding_t efuse_bonding = {0};
    halEfuseSmBonding_t efuse_sm_bonding;
    halEfuseLock3_t efuse_lock3;
    uint32_t uid[2];
    uint32_t rotpk[8] = {0}, secure_counter1[2] = {0};

    halEfuseReadOp_t efuse_ops[16];

    efuse_ops[0].op = HAL_EFUSE_OP_SINGLE_READ;
    efuse_ops[0].block_index = HAL_EFUSE_SINGLE_BLOCK_UID_0;
    efuse_ops[0].rval = &uid[0];

    efuse_ops[1].op = HAL_EFUSE_OP_SINGLE_READ;
    efuse_ops[1].block_index = HAL_EFUSE_SINGLE_BLOCK_UID_1;
    efuse_ops[1].rval = &uid[1];

    efuse_ops[2].op = HAL_EFUSE_OP_SINGLE_READ;
    efuse_ops[2].block_index = HAL_EFUSE_SINGLE_BLOCK_LOCK_3;
    efuse_ops[2].rval = &efuse_lock3.v;

    efuse_ops[3].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[3].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_0;
    efuse_ops[3].rval = &rotpk[0];

    efuse_ops[4].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[4].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_1;
    efuse_ops[4].rval = &rotpk[1];

    efuse_ops[5].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[5].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_2;
    efuse_ops[5].rval = &rotpk[2];

    efuse_ops[6].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[6].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_3;
    efuse_ops[6].rval = &rotpk[3];

    efuse_ops[7].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[7].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_4;
    efuse_ops[7].rval = &rotpk[4];

    efuse_ops[8].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[8].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_5;
    efuse_ops[8].rval = &rotpk[5];

    efuse_ops[9].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[9].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_6;
    efuse_ops[9].rval = &rotpk[6];

    efuse_ops[10].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[10].block_index = HAL_EFUSE_DOUBLE_BLOCK_ROTPK_7;
    efuse_ops[10].rval = &rotpk[7];

    efuse_ops[11].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[11].block_index = HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_0;
    efuse_ops[11].rval = & secure_counter1[0];

    efuse_ops[12].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[12].block_index = HAL_EFUSE_DOUBLE_BLOCK_SECURE_COUNTER1_1;
    efuse_ops[12].rval = &secure_counter1[1];

    efuse_ops[13].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[13].block_index = HAL_EFUSE_DOUBLE_BLOCK_SM_BONDING;
    efuse_ops[13].rval = &efuse_sm_bonding.v;

    efuse_ops[14].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[14].block_index = HAL_EFUSE_DOUBLE_BLOCK_RMA;
    efuse_ops[14].rval = &efuse_rma.v;

    efuse_ops[15].op = HAL_EFUSE_OP_DOUBLE_READ;
    efuse_ops[15].block_index = HAL_EFUSE_DOUBLE_BLOCK_BONDING;
    efuse_ops[15].rval = &efuse_bonding.v;

if (!halEfuseRead(efuse_ops, OSI_ARRAY_SIZE(efuse_ops)))
        return;

#ifdef ROM_TEST1
        secure_counter1[0] |= g_virbits[0];
        secure_counter1[1] |= g_virbits[1];
        efuse_bonding.v |= g_bonding;
        efuse_lock3.v |= g_lock3;
#endif

    gRomEfuse.rma_med = efuse_rma.b.rma_med;
    gRomEfuse.rma_all = ((efuse_rma.v & 0x7f) == 0x7f);

    SCI_TraceLow("romReadEfuseContent. rma_med:0x%x, ram_all:0x%x \n", gRomEfuse.rma_med, gRomEfuse.rma_all);



     SCI_TraceLow("romReadEfuseContent. enable_secure_boot = %d\n", efuse_bonding.b.enable_secure_boot);
    // read efuse only when needed
    if (efuse_bonding.b.enable_secure_boot && !gRomEfuse.rma_all)
    {
        gRomEfuse.sm_certify_mode = (efuse_sm_bonding.b.firmware_selection == 3);
     SCI_TraceLow("romReadEfuseContent. sm_certify_mode = %d\n", gRomEfuse.sm_certify_mode);
        if (gRomEfuse.sm_certify_mode)
        {
            SCI_ASSERT(0);
        }

        gRomEfuse.min_secure_version = romFromEfuseAntiRollback(secure_counter1);
        memcpy(&gRomEfuse.soc_raw_id, uid, sizeof(osiBits64_t));
        memcpy(&gRomEfuse.rotpk, rotpk, sizeof(osiBits256_t));
     SCI_TraceLow("sock_raw_id:%x:%x  %x:%x\n", gRomEfuse.soc_raw_id.data[0], uid[0], gRomEfuse.soc_raw_id.data[1],uid[1]);
     SCI_TraceLow("rotpk:%x %x %x %x %x %x %x %x\n",
         gRomEfuse.rotpk.data[0], gRomEfuse.rotpk.data[1], gRomEfuse.rotpk.data[2],gRomEfuse.rotpk.data[3],
         gRomEfuse.rotpk.data[3], gRomEfuse.rotpk.data[4], gRomEfuse.rotpk.data[5],gRomEfuse.rotpk.data[6]);
     SCI_TraceLow("rotpkraw:%x %x %x %x %x %x %x %x\n",
         rotpk[0], rotpk[1], rotpk[2],rotpk[3],rotpk[3], rotpk[4], rotpk[5],rotpk[6]);
    }
    else
    {
        efuse_lock3.v = 0;      // fake value
        efuse_sm_bonding.v = 0; // fake value
    }
    SCI_TraceLow("efuse_bonding:0x%x \n",efuse_bonding.v);
    gRomEfuse.secure_boot_enable = (efuse_bonding.b.enable_secure_boot &&
                                    efuse_lock3.b.rotpk == 0xffff);
    gRomEfuse.develcert_enable = !efuse_bonding.b.disable_develcert;
    gRomEfuse.download_enable = !efuse_bonding.b.disable_download;
    gRomEfuse.keypad_entermode_enable = !efuse_bonding.b.disable_keypad_entermode;
    gRomEfuse.uart1_boot_enable = !efuse_bonding.b.disable_uart1_boot;
    gRomEfuse.uart4_boot_enable = true; // enable always
    gRomEfuse.usb_boot_enable = !efuse_bonding.b.disable_usb_boot;
    gRomEfuse.sdcard_boot_enable = !efuse_bonding.b.disable_sdcard_boot;
    gRomEfuse.boot_med_encrypt = efuse_bonding.b.enable_boot_med_encrypt;
    gRomEfuse.enable_faster_flash = !efuse_bonding.b.disable_faster_flash;
    gRomEfuse.second_boot_offset = efuse_bonding.b.boot_sector_count * 4096;

    if (gRomEfuse.rma_all)
    {
        gRomEfuse.secure_boot_enable = false;
        gRomEfuse.develcert_enable = true;
        gRomEfuse.download_enable = true;
        gRomEfuse.boot_med_encrypt = false;
        gRomEfuse.sm_certify_mode = false;
        //romSecureDebugCtrl(0xffffffff);
    }
}
const romEfuseContent_t *romGetEfuseContent(void)
{
    return &gRomEfuse;
}
void romEnterDownload(unsigned timeout)
{
    //switch fast clock
    //hwp_apClk->cgm_ap_a5_sel_cfg = AP_CLK_CGM_AP_A5_SEL(0x5);

    const romEfuseContent_t *efuse = romGetEfuseContent();

    if (efuse->uart1_boot_enable)
    {
        SCI_TraceLow("UART1 enable, open uart1.\n");
    }

    if (efuse->uart4_boot_enable)
    {
        SCI_TraceLow("UART4 enable, open uart4.\n");
    }

    if (efuse->usb_boot_enable)
    {
        SCI_TraceLow("USB enable, open USB.\n");
    }

   // int loadpar = romDownloadIdentify(timeout);
    //if (loadpar < 0)
     //   return;

    //if (loadpar == ROM_LOADPAR_DOWNLOAD_UART1)
     //   romBslInit(&gBslUartOps, &gUart1, loadpar);
    //else if (loadpar == ROM_LOADPAR_DOWNLOAD_UART4)
   //     romBslInit(&gBslUartOps, &gUart4, loadpar);
    //else if (loadpar == ROM_LOADPAR_DOWNLOAD_USERIAL)
    //    romBslInit(&gBslUserialOps, &gUserial, loadpar);

   // romBslSendVersion();
    //romBslProcess(); // never return
}
void romColdBoot(void)
{
    const romEfuseContent_t *efuse = NULL;
    REG_LPS_APB_CFG_BOOT_MODE_T cfg_boot_mode = {0};
    romReadEfuseContent();
    efuse = romGetEfuseContent();

    // get boot mode and clear it
    cfg_boot_mode.v = hwp_lpsApb->cfg_boot_mode;
    SCI_TraceLow("cfg_boot_mode: 0x%x", cfg_boot_mode.v);

    // send release from deep power down command
    //
    // In case flash is not powered off, and in deep power down mode, and
    // jump to ROM, flash is not readable. Though it is possible that this
    // case won't happen, it is harmless to send command ABH.
    //romSpiFlashReleaseDeepPowerDown();

        romEnterDownload(30*1000);

}

void romBootStart(void)
{
    // read and clear magic
    unsigned magic = WARM_BOOT_MAGIC_REG;
    WARM_BOOT_MAGIC_REG = 0;

     SCI_TraceLow("romBootStart.magic:0x%x\n", magic);
    if (magic == WARM_BOOT_MAGIC ||
        magic == WARM_BOOT_MED_MAGIC)
    {
        // read jump entry
        uint32 entry = WARM_BOOT_ADDRESS_REG;
        SCI_TraceLow("romBootStart.entry:0x%x \n", entry );
        // we use 16MB as range check, even the real flash size may be smaller

    }

    romColdBoot();
}

void drvtestROM(void)
{
    romBootStart();
}
#endif
static void _handleWriteSecurityFlag(const MSG_HEAD_T *cmd)
{
        uint8 ret = 0;
    MSG_HEAD_T *msg = (MSG_HEAD_T *)cmd;
    MSG_HEAD_T *msg_response;
    uint8         cmd_subtype;

       SCI_TRACE_LOW("_handleWriteSecurityFlag.");
       ret = drvSecureWriteSecureFlags() ? 1 : 0;

#ifdef ROM_TEST
       drvtestROM();
#endif

    cmd_subtype = msg->subtype;

       msg_response = (MSG_HEAD_T *)SCI_ALLOC(1 + sizeof(MSG_HEAD_T));
    msg_response->seq_num   = msg->seq_num;
    msg_response->type         = msg->type;
    msg_response->subtype    = msg->subtype;
    msg_response->len        = sizeof(MSG_HEAD_T) + 1;

    SCI_MEMCPY((void *)(msg_response + 1), (void *)&ret, 1);
       SIO_SendPacket(msg_response, 1 + sizeof(MSG_HEAD_T));
    SCI_FREE(msg_response);

}

static void _handleReadSecurityFlag(const MSG_HEAD_T *cmd)
{
    uint8 ret = 0;
    MSG_HEAD_T *msg = (MSG_HEAD_T *)cmd;
    MSG_HEAD_T *msg_response;
    uint8         cmd_subtype;

       SCI_TRACE_LOW("_handleReadSecurityFlag.");
    ret = drvSecureBootEnable() ? 1 : 0;


    cmd_subtype = msg->subtype;

    msg_response = (MSG_HEAD_T *)SCI_ALLOC(1 + sizeof(MSG_HEAD_T));
    msg_response->seq_num   = msg->seq_num;
    msg_response->type         = msg->type;
    msg_response->subtype    = msg->subtype;
    msg_response->len        = sizeof(MSG_HEAD_T) + 1;

    SCI_MEMCPY((void *)(msg_response + 1), (void *)&ret, 1);
    SIO_SendPacket(msg_response, 1 + sizeof(MSG_HEAD_T));
    SCI_FREE(msg_response);
}
#endif
LOCAL uint32 Read_Unique_Id(uint8 *id)
{
    uint8 *des;
    uint32 id_low, id_high;
    id_low =  EFUSE1_HAL_Read(RDA_EFUSE_UNIQUE_ID_LOW_INDEX);
    id_high = EFUSE1_HAL_Read(RDA_EFUSE_UNIQUE_ID_HIGH_INDEX);
    des = id;


    *des++ = (id_low & 0xff000000) >> 24;
    *des++ = (id_low & 0x00ff0000) >> 16;
    *des++ = (id_low & 0x0000ff00) >> 8;
    *des++ = (id_low & 0x000000ff) >> 0;

    *des++ = (id_high & 0xff000000) >> 24;
    *des++ = (id_high & 0x00ff0000) >> 16;
    *des++ = (id_high & 0x0000ff00) >> 8;
    *des++ = (id_high & 0x000000ff) >> 0;
    return 0;
}

LOCAL uint32 Write_Unique_Id(uint8 *id)
{
#define SECURE_DEBUG
    uint32 id_low, id_high;
    #ifdef SECURE_DEBUG
    unsigned char data[] = {
        0x12, 0x34, 0x56, 0x78,
        0x12, 0x34, 0x56, 0x78,
    };
    id = &data[0];
    #endif

    id_low = *id << 24 |
            *(id + 1) << 16 |
            *(id + 2) << 8 |
            *(id + 3);

    id_high = *(id + 4 ) << 24|
            *(id + 4 + 1) << 16 |
            *(id + 4 + 2) << 8 |
            *(id + 4 + 3);

    EFUSE1_HAL_Write(RDA_EFUSE_UNIQUE_ID_LOW_INDEX, id_low);
    EFUSE1_HAL_Write(RDA_EFUSE_UNIQUE_ID_HIGH_INDEX, id_high);
    return 0;
}

LOCAL void HandleReadEfuseReq(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len)
{
    uint32 length;
    MSG_HEAD_T *msg_head = NULL;
    uint8 *id;

    id = (uint8 *)SCI_ALLOC(8);
    if(0 == Read_Unique_Id(id))
    {
        length = 8;
    } else {
        length = 1;
    }

    msg_head = (MSG_HEAD_T *)SCI_ALLOC(length + sizeof(MSG_HEAD_T));
    *dest_ptr = (uint8 *)msg_head;
    *dest_len_ptr = length + sizeof(MSG_HEAD_T);
    SCI_MEMCPY((void*)msg_head, src_ptr, sizeof(MSG_HEAD_T));
    if(length == 1)
    {
        SCI_MEMSET((void *)(msg_head + 1), 0x00, length);
    }
    else
    {
        SCI_MEMCPY((uint8 *)(msg_head + 1), (uint8 *)id, 8);
    }
    msg_head->len = *dest_len_ptr;
    SCI_TRACE_LOW("msg_head_seq_num=%d",msg_head->seq_num);
    SCI_TRACE_LOW("msg_head_len=%d",msg_head->len);
    SCI_TRACE_LOW("msg_head_type=%d",msg_head->type);
    SCI_TRACE_LOW("msg_head_subtype=%d",msg_head->subtype);
}
#endif



/*****************************************************************************/
//  Description:    This function parses the command message of the source
//                  and restore the response message to the destination buffer.
//    Global resource dependence: const char g_diag_atcmd_strtoken[];
//  Author: Xueliang.Wang
//    Note:
/*****************************************************************************/
#define UID_IN_STRING
LOCAL void HandleReadUIDReq(
    uint8 **dest_ptr,
    uint16 *dest_len_ptr,
    const uint8 *src_ptr,
    uint16 src_len)
{
    EFUSE_RETURN_E ret = EFUSE_RESULT_SUCCESS;
    uint8 uData[8]={0};
 #ifdef UID_IN_STRING
    uint8 uid_str[sizeof(uData)*2 + 1] = {'\0'};
    uint8 i = 0;
 #endif
    uint32 length;
    MSG_HEAD_T *msg_head = NULL;


    if(0 == Read_Unique_Id(uData))
    {
#ifdef UID_IN_STRING
        length = 8*2;
        for(i=0; i< length/2; i++) {
            sprintf(&uid_str[i*2], "%02x", uData[i]);
        }
#elif
        length = 8;
#endif
    } else {
        length = 1;
    }

    msg_head = (MSG_HEAD_T *)SCI_ALLOC(length + sizeof(MSG_HEAD_T));
    *dest_ptr = (uint8 *)msg_head;
    *dest_len_ptr = length + sizeof(MSG_HEAD_T);
    SCI_MEMCPY((void*)msg_head, src_ptr, sizeof(MSG_HEAD_T));
    if(length == 1)
    {
        SCI_MEMSET((void *)(msg_head + 1), 0x00, length);
    }
    else
    {
#ifdef UID_IN_STRING
        SCI_MEMCPY((uint8 *)(msg_head + 1), uid_str, length);
#elif
        SCI_MEMCPY((uint8 *)(msg_head + 1), uData, 8);
#endif
    }

    msg_head->len = *dest_len_ptr;
    SCI_TRACE_LOW("msg_head_seq_num=%d",msg_head->seq_num);
    SCI_TRACE_LOW("msg_head_len=%d",msg_head->len);
    SCI_TRACE_LOW("msg_head_type=%d",msg_head->type);
    SCI_TRACE_LOW("msg_head_subtype=%d",msg_head->subtype);
}



LOCAL void HandleSecurityFlagReq(const uint8 *src_ptr, uint16 src_len)
{
    uint32 rRet = 0;
    MSG_HEAD_T *msg = (MSG_HEAD_T *)src_ptr;
    MSG_HEAD_T *msg_response;
    uint8 data;
    uint8         cmd_subtype;

    cmd_subtype = msg->subtype;

    msg_response = (MSG_HEAD_T *)SCI_ALLOC(1 + sizeof(MSG_HEAD_T));
    msg_response->seq_num   = msg->seq_num;
    msg_response->type         = msg->type;
    msg_response->subtype    = msg->subtype;
    msg_response->len        = sizeof(MSG_HEAD_T) + 1;
    switch(cmd_subtype)
    {
#ifndef WIN32
                #ifndef PLATFORM_UWS6121E
        case DIAG_WRITE_SECURITY_FLAG:
            rRet = Write_SecurityFlag();
            break;
        case DIAG_READ_SECURITY_FLAG:
            rRet = Read_SecurityFlag();
            break;
        #else
        case DIAG_WRITE_SECURITY_FLAG:
            rRet += Write_Public_Key();
            rRet += Anti_Clone_Enable();
            rRet += Security_Enable();
            break;
        case DIAG_READ_SECURITY_FLAG:
            rRet = CheckSecureBootState();
            break;
        #endif
#endif
        default:
            break;
    }

    if(rRet)
    {
        data = 1;
    }
    else
    {
        data = 0;
    }

    SCI_MEMCPY((void *)(msg_response + 1), (void *)&data, 1);

    SCI_TRACE_LOW("msg_head_seq_num=%d",msg_response->seq_num);
    SCI_TRACE_LOW("msg_head_len=%d",msg_response->len);
    SCI_TRACE_LOW("msg_head_type=%d",msg_response->type);
    SCI_TRACE_LOW("msg_response_ret :: %d",*((uint32 *)(msg_response+1)));
    SIO_SendPacket(msg_response, 1 + sizeof(MSG_HEAD_T));
    SCI_FREE(msg_response);
}

LOCAL void HandleWriteHarshReq(const uint8 *src_ptr, uint16 src_len)
{
    EFUSE_RETURN_E rRet = EFUSE_RESULT_SUCCESS;
    MSG_HEAD_T *msg = (MSG_HEAD_T *)src_ptr;
    MSG_HEAD_T *msg_response;
    uint8 data = 0;

    msg_response = (MSG_HEAD_T *)SCI_ALLOC_APP(1 + sizeof(MSG_HEAD_T));
    msg_response->seq_num   = msg->seq_num;
    msg_response->type         = msg->type;
    msg_response->subtype    = msg->subtype;
    msg_response->len        = sizeof(MSG_HEAD_T) + 1;

    if(WRITE_EFUSE_DISABLE == write_efuse_flag)
    {
        data = 0;
    }
    else
    {
#ifndef WIN32
#ifdef SECURE_BOOT_SUPPORT
#ifdef PLATFORM_UWS6121E
#ifdef SECURE_DEBUG
        rRet = Write_Unique_Id(src_ptr);
#endif
#else
        //rRet = WriteBscHashValue();
        //rRet = Write_Public_Key();
#endif
#endif
#endif

        if(EFUSE_RESULT_SUCCESS == rRet)
        {
            data = 1;
        }
        //write_efuse_flag = WRITE_EFUSE_DISABLE;
    }
    SCI_TRACE_LOW("HandleWriteHarshReq DATA = %d\r\n", data);
    SCI_TRACE_LOW("HandleWriteHarshReq ret = %d\r\n", rRet);

    SCI_MEMCPY((void *)(msg_response + 1), (void *)&data, 1);
    SIO_SendPacket(msg_response, 1 + sizeof(MSG_HEAD_T));
    SCI_FREE(msg_response);
}

LOCAL void DisposeMsg(  // None
    uint8 **dest,       // Pointer of the response message.
    uint16 *dest_len,   // Pointer of size of the response message in uin8.
    const uint8 *src,   // Pointer of the input message.
    uint16 src_len)     // Size of the source buffer in uint8.
{
    uint8        cmd_type;
    uint8        cmd_subtype;
    MSG_HEAD_T  *msg_head;
       uint16 mcu_mode ;

    #ifdef _DEBUG
    MSG_HEAD_T  cmd_head;
    #endif


    msg_head    = (MSG_HEAD_T *) src;
    cmd_type    = msg_head->type;
    cmd_subtype = msg_head->subtype;

    SCI_TRACE_LOW("DisposeMsg cmd_type=%d, cmd_subtype=%d", cmd_type, cmd_subtype);

    #ifndef  WIN32
    if( cmd_type < REQ_MAX_F )
    {
        if( NULL != s_diag_cmd_routine[ cmd_type ] )
        {
            if( s_diag_cmd_routine[ cmd_type ]( dest, dest_len, src, src_len ) )
            {
                // has been handled.
                return ;
            }
        }
        else if(cmd_type == DIAG_DIRECT_NV || cmd_type == DIAG_DIRECT_PHSCHK)
        {

            *dest_len = 0;
            RM_Cali_NvAccess_CAL_Mode(dest,dest_len,msg_head);

            return ;
        }
    }
    #endif
//SCI_TRACE_LOW:"dISPOSEMSG"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_1441_112_2_18_1_3_56_2,(uint8*)"");
//----Cmd type Switch---------->>>
    switch(cmd_type)
    {
        case DIAG_PS_F:
            HandleLayer1PsReq( dest, dest_len, src, src_len );
            break;
        case DIAG_LOG_F:
            /*
                this case added by lliu(2001-12-24)
                log filter process
            */
            mcu_mode = POWER_GetResetMode();
            switch( cmd_subtype )
            {
                    case LOG_GET_FILTER:
                if ((CALIBRATION_MODE != mcu_mode) && (LAYER1_TEST_MODE != mcu_mode))
                {
                        GetLogFilterInfo(dest, dest_len, src, src_len);
                           }
                        break;
                    case LOG_SET_FILTER:
                           if ((CALIBRATION_MODE != mcu_mode) && (LAYER1_TEST_MODE != mcu_mode))
                           {
                        SetLogFilter(dest, dest_len, src, src_len);
                          }
                        break;

                case ARM_LOG_ENABLE:
                case ARM_LOG_DISABLE:
                case DSP_LOG_ENABLE:
                case DSP_LOG_DISABLE:
                HandleLogSwitch(src, src_len);
                break;

                #ifdef LOG_FLOW_CONTROL
                case LOG_SET_FLOW_LOG:
                    SetFlowLog(dest, dest_len, src, src_len);
                    break;
                #endif

                default:
                HandleBadCommand(dest, dest_len, src, src_len);
                break;
            }

            break;

        case DIAG_SOFTKEY_F:
            PressSoftKey(dest, dest_len, src, src_len);
            break;

        case DIAG_SYSTEM_F:
            // Copy message head structure.
            SCI_MEMCPY((void *)&cmd_head, (void*)src, sizeof(MSG_HEAD_T));
            switch(cmd_subtype)
            {
            SCI_TraceLow("DIAG_SYSTEM_F.cmd_subtype:%d", cmd_subtype);
#ifdef SECURE_BOOT_SUPPORT
#ifdef PLATFORM_UWS6121E
        case DIAG_WRITE_SECURITY_FLAG:
            _handleWriteSecurityFlag((MSG_HEAD_T *)src);
            break;

        case DIAG_READ_SECURITY_FLAG:
            _handleReadSecurityFlag((MSG_HEAD_T *)src);
            break;
#else
        case DIAG_WRITE_SECURITY_FLAG:
        case DIAG_READ_SECURITY_FLAG:
            HandleSecurityFlagReq(src, src_len);
             break;
#endif
#endif

                // Get the infomation, then trace out.
            case DIAG_STACK_INFO_F:    /* get stack used max size */
                cmd_head.subtype = ATCMD_SUCCESS;
                ATCOMMON_GenerateRspMsg(dest, dest_len, &cmd_head);
                break;

            case DIAG_MEMORY_INFO_F:    /* get memory used max size */
                cmd_head.subtype = ATCMD_SUCCESS;
                ATCOMMON_GenerateRspMsg(dest, dest_len, &cmd_head);
                SCI_PrintPoolInfo();
                break;

            case DIAG_MEM_ALLOC_INFO_F:
                cmd_head.subtype = ATCMD_SUCCESS;
                ATCOMMON_GenerateRspMsg(dest, dest_len, &cmd_head);
                SCI_PrintAllocatedMemInfo();
                break;
            case DIAG_READ_UID_V2_F:
                HandleReadUIDReq(dest, dest_len, src, src_len);
                break;
            case DIAG_ASSERT_MS:
                cmd_head.subtype = ATCMD_SUCCESS;
                ATCOMMON_GenerateRspMsg(dest, dest_len, &cmd_head);
                //always assert in debug mode
                SCI_ASSERT(FALSE);/*assert verified*/
                break;

                 case DIAG_READ_EFUSE_INFO_F:
            HandleReadEfuseReq(dest, dest_len, src, src_len);
                break;
    #ifdef HALO_SUPPORT
                 case DIAG_EFUSE_CHECK:
                HALO_factory_CheckLicense(dest, dest_len, src, src_len);
                break;
                 case DIAG_UID_READ:
                HALO_factory_ReadUid(dest, dest_len, src, src_len);
                break;
                 case DIAG_EFUSE_WRITE:
                HALO_factory_WriteLicense(dest, dest_len, src, src_len);
                break;
    #endif
#if defined (SECURE_BOOT_SUPPORT)
              case DIAG_WRITE_EFUSE_INFO_F:
            //Handling both the BSC write and writing of hash of bootloader onto efuse
            HandleWriteHarshReq(src, src_len );
            break;
#endif

#ifdef SHELL_DEBUG_SUPPORT
           case MSG_CONSOLE_A:
                //DIAG_disposeExtCmd(dest, dest_len, src);
                DIAG_DisposeShellCmd((uint8 *)src);
                break;
#endif /* SHELL_DEBUG_SUPPORT */

            default:
                cmd_head.subtype = ATCMD_BADCOMMAND;
                ATCOMMON_GenerateRspMsg(dest, dest_len, &cmd_head);
                break;
            }

            break;


    case DIAG_SWVER_F:           /* Get software version*/
        #ifndef WIN32
        switch(cmd_subtype)
        {
        case 0: //cp version
            GetCpSwVersion(dest, dest_len, src, src_len);
            break;
        case 1:
            GetLogVersion(dest, dest_len, src, src_len);
            break;
        case 2: //ap version
            GetSwVersion(dest, dest_len, src, src_len);
            break;
            default:
            break;
        }
        #endif
        break;

/* @Jim.zhang CR:MS13480  2004-09-15 */
/*
    // @Xueliang.Wang added
    case DIAG_AT_F:                // AT Like Command
    //case DIAG_AT_R:
//        HandleATCommand(dest, dest_len, src, src_len);
        SCI_ASSERT(0);
        break;
    // @Xueliang.Wang added end.
*/
/* end CR:MS13480 */
        //@Shijun.cui 2005-08-24 add for tools, DIAG ECHO. CR25558
        case DIAG_AT_F:
             HandleBadCommand(dest, dest_len, src, src_len);
        break;
        //end CR25558

        case MSG_REG:                /* Register message*/
            switch(cmd_subtype)
            {
            case PS_L2_A:
                RegisterOk(dest, dest_len, src, src_len);
                break;
            default:
                RegisterOk(dest, dest_len, src, src_len);
                break;
            }
            break;
        case MSG_UNREG:             /* Unregister message*/
            switch(cmd_subtype)
            {
            case PS_L2_A:
                UnRegisterOk(dest, dest_len, src, src_len);
                break;
            default:
                UnRegisterOk(dest, dest_len, src, src_len);
                break;
            }
            break;
        case DIAG_POWER_F:
            PowerMS(dest, dest_len, src, src_len);
            break;
        case DIAG_SIM_F:
            SendSimCmd(dest, dest_len, src, src_len);
            break;
        case DIAG_MCU_F:
            // Handle MCU Read or Write command.
            HandleMcuAccess(dest, dest_len, src, src_len);
            break;

        case DIAG_CHANGE_MODE_F:
            HandleChangeMode(dest, dest_len, src, src_len);
            break;

        case DIAG_NVITEM_F:
            HandleNvitemAccess(dest, dest_len, src, src_len);
            break;

        case DIAG_ADC_F:
            #ifndef WIN32
            HandleReadADC( dest, dest_len, src, src_len );
            #endif
        break;
        case DIAG_PRODUCTION_TEST_EXTEND_F:
            //HandleProductionTestExtendCommand( dest, dest_len, src, src_len );
        break;
        #if 0
        case DIAG_PIN_TEST_F:
            #ifndef WIN32
            HandlePinTest( dest, dest_len, src, src_len );
            #endif
        break;

        case DIAG_CALIBRATION_NOISE_F:
            #ifndef WIN32
            HandleCalibrationNoiseCheck( dest, dest_len, src, src_len );
            #endif
        break;
             #endif
        case DIAG_GET_MCU_MODE_F:
            DIAG_GetMcuMode( dest, dest_len, src, src_len);
        break;

        case DIAG_POWER_SUPPLY_F:
            switch(cmd_subtype)
            {
                case BATT_VOLTAGE_F:
                    HandleBatteryVoltage(dest, dest_len, src, src_len);
                break;
                case ADC_TO_VOLTAGE_F:
                    HandleADCToVoltage(dest,dest_len,src,src_len);
                 break;
                default:
                    HandleBadCommand(dest, dest_len, src, src_len);
                break;
            }
        break;

        case DIAG_RF_QUERY_F:
            HandleRFQuery( dest, dest_len, src, src_len);
        break;

        case DIAG_LAYER1_F:
            HandleLayer1PsReq( dest, dest_len, src, src_len );
        break;

#ifndef WIN32
#ifdef CMMB_SUPPORT
        case DIAG_TV_PARAM:
            DTLRFVT_HandleToolMsg(dest, dest_len, src, src_len);
        break;
#endif
#endif
        default:                     /* Send command error message*/
            HandleBadCommand(dest, dest_len, src, src_len);
            break;
        }
}

/*****************************************************************************/
//  Description:    This function returns the software version
//                  of the Mobile Station.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void GetSwVersion(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    MSG_HEAD_T  *head_ptr;
    char        *ver_str_ptr    = SCI_NULL;
    uint16      ver_str_len     = 0;

#ifndef    WIN32
    // Get version string and string length.
    //@Shijun.cui 2005-08-24 modify CR 25558
    //ver_str_ptr = (char *)SCI_GetSWBuildInfo();
    ver_str_ptr = (char *)COMMON_GetProjectVersionInfo();
    //end CR25558
    ver_str_len = strlen(ver_str_ptr); //lint !e746

    // Malloc a buf for response buf.
    head_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(ver_str_len + sizeof(MSG_HEAD_T));
    SCI_ASSERT(SCI_NULL != head_ptr);/*assert verified*/

    *dest_ptr       = (uint8 *)head_ptr;
    *dest_len_ptr   = ver_str_len + sizeof(MSG_HEAD_T);

    // Copy head structure.
    SCI_MEMCPY((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));
    //@Shijun.cui 2005-08-24 add CR25558
    head_ptr->len = ver_str_len + sizeof(MSG_HEAD_T);
    //end cr25558
    // Copy version string.
    SCI_MEMCPY((void *)(head_ptr + 1), ver_str_ptr, ver_str_len);/*lint !e516 -e516*/
    // Set response packet length.
    head_ptr->len = *dest_len_ptr;
#endif
}


/*****************************************************************************/
//  Description:    This function returns the software version
//                  of the Mobile Station.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void GetCpSwVersion(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    MSG_HEAD_T  *head_ptr;
    uint16      ver_str_len     = 0;
    uint32      mem_len         = 0;
    uint32      md_ver_num      = 0;
    char        md_ver_str[50]  = {0};

#ifndef    WIN32
    md_ver_str[49] = 0x5a;

    if (ipc_get_init_status()) {
        md_ver_num = ipc_get_modem_version();
        sprintf (md_ver_str, "CP Version: %u", md_ver_num);
    } else {
        sprintf (md_ver_str, "CP Version: %s", "ERROR:CP is not running!");
    }

    SCI_ASSERT(md_ver_str[49] == 0x5a);/*assert verified*/
    //end CR25558
    ver_str_len = strlen(md_ver_str); //lint !e746

    // Malloc a buf for response buf.
    head_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(ver_str_len + sizeof(MSG_HEAD_T));
    SCI_ASSERT(SCI_NULL != head_ptr);/*assert verified*/

    *dest_ptr       = (uint8 *)head_ptr;
    *dest_len_ptr   = ver_str_len + sizeof(MSG_HEAD_T);

    // Copy head structure.
    SCI_MEMCPY((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));
    SCI_MEMCPY((void *)(head_ptr + 1), md_ver_str, ver_str_len);/*lint !e516 -e516*/
    // Set response packet length.
    head_ptr->len = *dest_len_ptr;
#endif
}

/*****************************************************************************/
//  Description:    This function returns the software version
//                  of the Mobile Station.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void GetLogVersion(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    MSG_HEAD_T  *head_ptr;
    char        *ver_str_ptr    = SCI_NULL;
    uint16      ver_str_len     = 0;

#ifndef    WIN32
    // Get version string and string length.
    //@Shijun.cui 2005-08-24 modify CR 25558
    //ver_str_ptr = (char *)SCI_GetSWBuildInfo();
    ver_str_ptr = (char *)VERSION_GetInfo(LOG_VERSION);
    //end CR25558
    ver_str_len = strlen(ver_str_ptr);

    // Malloc a buf for response buf.
    head_ptr = (MSG_HEAD_T*)SCI_ALLOC_APP(ver_str_len + sizeof(MSG_HEAD_T));
    SCI_ASSERT(SCI_NULL != head_ptr);/*assert verified*/

    *dest_ptr       = (uint8 *)head_ptr;
    *dest_len_ptr   = ver_str_len + sizeof(MSG_HEAD_T);

    // Copy head structure.
    SCI_MEMCPY((void *)head_ptr, src_ptr, sizeof(MSG_HEAD_T));
    //@Shijun.cui 2005-08-24 add CR25558
    head_ptr->len = ver_str_len + sizeof(MSG_HEAD_T);
    //end cr25558
    // Copy version string.
    SCI_MEMCPY((void *)(head_ptr + 1), ver_str_ptr, ver_str_len);
    // Set response packet length.
    head_ptr->len = *dest_len_ptr;
#endif
}

//void diag_bad_command(
/*****************************************************************************/
//  Description:    This function returns the bad command response
//                  back to client.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleBadCommand(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8       *temp_dest_ptr = SCI_NULL;
    MSG_HEAD_T  cmd_head;

    // malloc a buf for response buf.
    temp_dest_ptr = (uint8*)SCI_ALLOC_APP(sizeof(MSG_HEAD_T));
    SCI_ASSERT(SCI_NULL != temp_dest_ptr);/*assert verified*/
    *dest_ptr     = temp_dest_ptr;

    // Copy head structure.
    SCI_MEMCPY((void *)&cmd_head, src_ptr, sizeof(MSG_HEAD_T));
    cmd_head.len  = sizeof(MSG_HEAD_T);
    SCI_MEMCPY(temp_dest_ptr, (void *)&cmd_head, sizeof(MSG_HEAD_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T);

    // Log a error message.
    //SCI_TraceLow:"Command type %d error!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_1786_112_2_18_1_3_57_3,(uint8*)"d", cmd_head.type);

}

//void diag_register_OK(
/*****************************************************************************/
//  Description:    This function set the auto trace option of
//                  the trace buffer.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void RegisterOk(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8       *temp_dest_ptr = SCI_NULL;

    temp_dest_ptr = (uint8*)SCI_ALLOC_APP(sizeof(MSG_HEAD_T)); /* malloc a buf for response buf.*/
    SCI_ASSERT(SCI_NULL != temp_dest_ptr);/*assert verified*/
    *dest_ptr     = temp_dest_ptr;

    // Copy head structure.
    SCI_MEMCPY(temp_dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T);
}

//void diag_unregister_OK()
/*****************************************************************************/
//  Description:    This function disable the auto trace option of
//                  the trace buffer.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void UnRegisterOk(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8       *temp_dest_ptr = SCI_NULL;

    temp_dest_ptr = (uint8*)SCI_ALLOC_APP(sizeof(MSG_HEAD_T)); /* malloc a buf for response buf.*/
    SCI_ASSERT(SCI_NULL != temp_dest_ptr);/*assert verified*/
    *dest_ptr     = temp_dest_ptr;

    // Copy head structure.
    SCI_MEMCPY(temp_dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T);
}

#ifdef CHIP_SC6600C
//extern void FFS_RegisterDiagCmdRoutine( void );
#endif

//void diag_init(void)
/*****************************************************************************/
//  Description:    This function init the diagnostic task.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void InitDiag(void)  //Modified by Johnson.sun 2006/10/09
{


#ifndef WIN32
    {   // This function must be impletment by the client.
        // This function allow the client to register there command routine.

        //@lin.liu. Init the diag command routine
        DIAG_InitCmdRoutine();

          //Register Diag cmd for calibration
        {
              extern void Layer1_RegCaliDiagHandle( void );

              Layer1_RegCaliDiagHandle ();
          }

        SCI_RegisterReadAssertInfoCmdRoutine();
        Register_MMI_Test_CmdRoutine();
        Register_Current_Test_CmdRoutine();
        Register_ApTest_CmdRoutine();

    }
        /* For test shell.Eddie.Li */
#ifdef SHELL_DEBUG_SUPPORT
        {
        extern void SHELL_TestRegisterShellCmd(void);
        SHELL_TestRegisterShellCmd();
        }
#endif /* SHELL_DEBUG_SUPPORT */
        /* For test shell.Eddie.Li */
#endif

}

/*****************************************************************************/
//  Description:    Handle log switch.
//    Global resource dependence:
//  Author:         Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleLogSwitch( // None
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    uint8 sub_type = ((MSG_HEAD_T *) src_ptr)->subtype;

    if (ARM_LOG_ENABLE == sub_type)
    {
        SCI_InitLogSwitch(SCI_TRUE);
    }
    else if (ARM_LOG_DISABLE == sub_type)
    {
        SCI_InitLogSwitch(SCI_FALSE);
#ifndef WIN32
        SIO_ResetBuf(1);
#endif
    }
    else if (DSP_LOG_ENABLE == sub_type)
    {
#ifndef WIN32
        SCI_ASSERT(0);/*assert verified*/
#endif  // WIN32
    }
    else    // if (DSP_LOG_DISABLE == sub_type)
    {
#ifndef WIN32
//        *(volatile uint32 *)GR_GEN0 &= ~(0x1002);   //for DSP UART port
        SCI_ASSERT(0);/*assert verified*/
#endif  // WIN32
    }

    // Send response packet.
    SIO_SendPacket( (void *)src_ptr, sizeof(MSG_HEAD_T));
}

//void diag_get_log_filter_info(
/*****************************************************************************/
//  Description:    This function get log filter info of some modules.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void GetLogFilterInfo(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint32      size            = 0;
    uint8       *temp_dest_ptr  = SCI_NULL;
    MSG_HEAD_T  cmd_head;

    // Check input parameter.
    SCI_ASSERT(SCI_NULL != src_ptr);/*assert verified*/

// @Xueliang.Wang modified(2003-02-11)
    //  First, we get size.
    //  If we known the size, we don't need  the step
    //size = SCI_LogGetFilterInfo(NULL);
    size = SCI_MAX_STATIC_THREAD_ID * sizeof(LOG_FILTER_T);
// End

    temp_dest_ptr = (uint8 *)SCI_ALLOC_APP(size + sizeof(MSG_HEAD_T));
    SCI_ASSERT( SCI_NULL !=  temp_dest_ptr);/*assert verified*/
    *dest_ptr     = temp_dest_ptr;

    // Copy head
    SCI_MEMCPY((void *)&cmd_head, src_ptr, sizeof(MSG_HEAD_T));
    cmd_head.len  = (uint16)(size + sizeof(MSG_HEAD_T));
    SCI_MEMCPY(temp_dest_ptr, (void *)&cmd_head, sizeof(MSG_HEAD_T));

    temp_dest_ptr += sizeof(MSG_HEAD_T);

    // get info
    size = SCI_LogGetFilterInfo(temp_dest_ptr);
    //*id_ptr,
    //    *(id_ptr + 1));

    *dest_len_ptr  = (uint16) (size + sizeof(MSG_HEAD_T));
}

//void diag_set_log_filter(
/*****************************************************************************/
//  Description:    This function set the log filter of the given module.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void SetLogFilter(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8       *temp_dest_ptr = SCI_NULL;
    MSG_HEAD_T  cmd_head;

    // Check input parameter.
    SCI_ASSERT(src_len >= (sizeof(MSG_HEAD_T) + sizeof(BLOCK_ID) + 1));/*assert verified*/

    SCI_LogSetFilter(  (uint8 *)(src_ptr + sizeof(MSG_HEAD_T)),
        (uint16 )(src_len - sizeof(MSG_HEAD_T)));

    // malloc a buf for response buf.
     temp_dest_ptr = (uint8*)SCI_ALLOC_APP(sizeof(MSG_HEAD_T));
    SCI_ASSERT(SCI_NULL !=  temp_dest_ptr);/*assert verified*/
    *dest_ptr  = temp_dest_ptr;

    // Copy head structure.
    SCI_MEMCPY((void *)&cmd_head, src_ptr, sizeof(MSG_HEAD_T));
    cmd_head.len  = sizeof(MSG_HEAD_T);
    SCI_MEMCPY(temp_dest_ptr, (void *)&cmd_head, sizeof(MSG_HEAD_T));

    *dest_len_ptr = sizeof(MSG_HEAD_T);
}

#ifdef LOG_FLOW_CONTROL

/*****************************************************************************/
//  Description:    This function initialize flow.
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void InitDiagFlow(void)
{
    int32   i   = 0;

    flow_cnt_timer = SCI_CreatePeriodTimer("flow data",
                        (TIMER_FUN) CheckFlowCntTimer, 0, 1000,
                        SCI_AUTO_ACTIVATE);

    is_timer_on = SCI_TRUE;

    for (i = 0; i < MAX_LAYER_NUM * 2; i++)
    {
        flow_cnt[i] = 0;
    }
}

//uint32 es_diag_set_flow(BLOCK_ID thread_ID, uint32 count)
/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
uint32 DIAG_SetFlow(
    BLOCK_ID thread_ID,
    FLOW_DIR_E direction,
    uint32 count)
{
    switch (thread_ID)
    {
    case P_APP:
        flow_cnt[0 + direction] += count;
        break;
    case P_SNDCPMain:
        flow_cnt[2 + direction] += count;
        break;
    case P_Llmain:
        flow_cnt[4 + direction] += count;
        break;
    case P_rlc:
        flow_cnt[6 + direction] += count;
        break;
    case P_MAC:
        flow_cnt[8 + direction] += count;
        break;
    default:
        //return SCI_ID_ERROR;
        return SCI_ERROR;
    }

    return SCI_SUCCESS;
}

//PRIVATE SCI_CALLBACK check_flow_cnt_timer(uint32 id)
/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL SCI_CALLBACK CheckFlowCntTimer(
    uint32 id)
{
    xSignalHeader    sig_ptr = SCI_NULL;

    sig_ptr = (xSignalHeader)SCI_ALLOC_APP(sizeof(xSignalHeaderRec) + 4);
    SCI_ASSERT(SCI_NULL != sig_ptr);/*assert verified*/

    sig_ptr->SignalSize = sizeof(xSignalHeaderRec) + 4;
    sig_ptr->Sender     = DIAG;
    *(uint32 *)(sig_ptr + 1)= ~SCI_NULL;
    SCI_SendSignal(sig_ptr, DIAG);

}

//void diag_set_flow_log()
/*****************************************************************************/
//  Description:
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void SetFlowLog(     // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint8  *temp_dest_ptr = SCI_NULL;
    uint8  flag           = 0;

    flag = *(src_ptr + sizeof(MSG_HEAD_T));

    if (flag)
    {
        if (!is_timer_on)
        {
            flow_cnt_timer = SCI_CreatePeriodTimer("flow data",
                                (TIMER_FUN) CheckFlowCntTimer, 0, 60,
                                SCI_AUTO_ACTIVATE);
            is_timer_on = SCI_TRUE;
        }
    }
    else
    {
        if (is_timer_on)
        {
            SCI_DeleteTimer(flow_cnt_timer);

            is_timer_on = SCI_FALSE;
        }
    }

    // Malloc a buf for response buf.
    temp_dest_ptr = SCI_ALLOC_APP(sizeof(MSG_HEAD_T));
    SCI_ASSERT(temp_dest_ptr != NULL);/*assert verified*/
    *dest_ptr     = temp_dest_ptr;

    // Copy head structure.
    SCI_MEMCPY(temp_dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T);

    return;
}

#endif // LOG_FLOW_CONTROL

// @Xueliang.Wang changed(2002-10-17)
// Change key_sig_ptr type from xSignalHeaderRec to KPDSVR_SIG_T.
LOCAL void PressSoftKey(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)        // Size of the source buffer in uint8.
{
    COMM_SOFTKEY_PTR    softkey_ptr;
    KPDSVR_SIG_T        *key_sig_ptr    = SCI_NULL;
    uint32              size            = sizeof(KPDSVR_SIG_T);

    uint8                cmd_subtype;
    MSG_HEAD_T          *msg_head;

    msg_head    = (MSG_HEAD_T *) src_ptr;
    cmd_subtype = msg_head->subtype;

    softkey_ptr = (COMM_SOFTKEY_PTR) (src_ptr + sizeof(MSG_HEAD_T));

    // Build response to channel server
    *dest_ptr = (uint8*)SCI_ALLOC_APP(sizeof(MSG_HEAD_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T);
    memcpy(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;

    switch(cmd_subtype)
    {
        case     DIAG_KEY_MSG_DOWN_INFO_F:
            //SCI_CREATE_SIGNAL(key_sig_ptr, KPD_DOWN, sizeof(KPDSVR_SIG_T), DIAG);
            key_sig_ptr = (KPDSVR_SIG_T *)SCI_ALLOC_APP(size);
            SCI_ASSERT(SCI_NULL != key_sig_ptr);/*assert verified*/
            key_sig_ptr->SignalSize = size;
            key_sig_ptr->Sender     = DIAG;
            key_sig_ptr->SignalCode = KPD_DOWN;

            // 重载了xSignalHeaderRec结构，用于传递键盘扫描码值
            //key_sig_ptr->Pre = (void *) softkey_ptr->key_code;
            key_sig_ptr->key = (int) softkey_ptr->key_code;

            SCI_SendSignal((xSignalHeader)key_sig_ptr, KPDSVR);

            //SCI_TRACE_LOW:"SIMULATOR KPD: generate down key, key code is %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2166_112_2_18_1_3_58_4,(uint8*)"d",key_sig_ptr->key);

        break;

        case     DIAG_KEY_MSG_UP_INFO_F:
            //SCI_CREATE_SIGNAL(key_sig_ptr, KPD_UP, sizeof(KPDSVR_SIG_T), DIAG);
            key_sig_ptr = (KPDSVR_SIG_T *)SCI_ALLOC_APP(size);
            SCI_ASSERT(SCI_NULL != key_sig_ptr);/*assert verified*/
            key_sig_ptr->SignalSize = size;
            key_sig_ptr->Sender     = DIAG;
            key_sig_ptr->SignalCode = KPD_UP;

            // 重载了xSignalHeaderRec结构，用于传递键盘扫描码值
            //key_sig_ptr->Pre = (void *) softkey_ptr->key_code;
            key_sig_ptr->key = (int) softkey_ptr->key_code;

            SCI_SendSignal((xSignalHeader)key_sig_ptr, KPDSVR);

            //SCI_TRACE_LOW:"SIMULATOR KPD: generate up key, key code is %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2184_112_2_18_1_3_58_5,(uint8*)"d",key_sig_ptr->key);

        break;

        case DIAG_NORMAL_KEY_MSG_INFO_F:
        default:
        {
            //SCI_CREATE_SIGNAL(key_sig_ptr, KPD_DOWN, sizeof(KPDSVR_SIG_T), DIAG);
            key_sig_ptr = (KPDSVR_SIG_T *)SCI_ALLOC_APP(size);
            SCI_ASSERT(SCI_NULL != key_sig_ptr);/*assert verified*/
            key_sig_ptr->SignalSize = size;
            key_sig_ptr->Sender     = DIAG;
            key_sig_ptr->SignalCode = KPD_DOWN;

            // 重载了xSignalHeaderRec结构，用于传递键盘扫描码值
            //key_sig_ptr->Pre = (void *) softkey_ptr->key_code;
            key_sig_ptr->key = (int) softkey_ptr->key_code;

            SCI_SendSignal((xSignalHeader)key_sig_ptr, KPDSVR);

            //SCI_CREATE_SIGNAL(key_sig_ptr, KPD_UP, sizeof(KPDSVR_SIG_T), DIAG);
            key_sig_ptr = (KPDSVR_SIG_T *)SCI_ALLOC_APP(size);
            SCI_ASSERT(SCI_NULL != key_sig_ptr);/*assert verified*/
            key_sig_ptr->SignalSize = size;
            key_sig_ptr->Sender     = DIAG;
            key_sig_ptr->SignalCode = KPD_UP;

            // 重载了xSignalHeaderRec结构，用于传递键盘扫描码值
            //key_sig_ptr->Pre = (void *) softkey_ptr->key_code;
            key_sig_ptr->key = (int) softkey_ptr->key_code;

            //SCI_TRACE_LOW:"SIMULATOR KPD: generate down and up key, key code is %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2215_112_2_18_1_3_58_6,(uint8*)"d",key_sig_ptr->key);
            SCI_SendSignal((xSignalHeader)key_sig_ptr, KPDSVR);
        }
        break;
    }
}

LOCAL void PowerMS(
    uint8 **dest_ptr,       // Pointer of the response message
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8
    const uint8 *src_ptr,   // Pointer of the input message
    uint16 src_len          // Size of the source buffer in uint8
)
{
    MSG_HEAD_T      *head_ptr;
    KPDSVR_SIG_T    *power_sig_ptr;

    head_ptr = (MSG_HEAD_T *) src_ptr;

    // Send response packet.
    SIO_SendPacket((void *)src_ptr, sizeof(MSG_HEAD_T));

    SCI_Sleep(2000);

    power_sig_ptr = (KPDSVR_SIG_T *)SCI_ALLOC_APP(sizeof(KPDSVR_SIG_T));
    SCI_ASSERT(SCI_NULL != power_sig_ptr);/*assert verified*/
    power_sig_ptr->SignalSize = sizeof(KPDSVR_SIG_T);
    power_sig_ptr->Sender     = DIAG;
    power_sig_ptr->key        = SCI_VK_POWER;

    if (head_ptr->subtype == POWER_OFF)
    {
        power_sig_ptr->SignalCode = KPD_SOFT_POWER_OFF;
    }
    else if (head_ptr->subtype == POWER_ON)
    {
        power_sig_ptr->SignalCode = KPD_SOFT_POWER_ON;
    }
    else
    {
        SCI_ASSERT(0);/*assert verified*/
    }

    SCI_SendSignal((xSignalHeader)power_sig_ptr, KPDSVR);

}

LOCAL uint32 sim_seq_num;

LOCAL void SendSimInfoToChannelServer(
    xSignalHeader signal_ptr
)
{
    MSG_HEAD_T    *msg;
    uint16        size;

    // Malloc for send to SIO.
    size = signal_ptr->SignalSize + sizeof(MSG_HEAD_T);
    msg = (MSG_HEAD_T *)SCI_ALLOC_APP(size);

    CM_COPY_MSG_HEAD(msg, sim_seq_num, size, DIAG_SIM_F, 0)

    SCI_MEMCPY(msg + 1, signal_ptr, signal_ptr->SignalSize);

    SIO_SendPacket(msg, size);

    SCI_FREE(msg);
}

LOCAL void SendSimCmd(
    uint8 **dest_ptr,       // Pointer of the response message
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8
    const uint8 *src_ptr,   // Pointer of the input message
    uint16 src_len          // Size of the source buffer in uint8
)
{
    xSignalHeader sim_signal;

    sim_signal = (xSignalHeader) SCI_ALLOC_APP(src_len - sizeof(MSG_HEAD_T));

    SCI_MEMCPY(sim_signal, src_ptr + sizeof(MSG_HEAD_T), src_len - sizeof(MSG_HEAD_T));

    sim_signal->Sender = DIAG;

    sim_seq_num = ((MSG_HEAD_T *) src_ptr)->seq_num;
    *dest_ptr = SCI_NULL;

    SCI_SendSignal(sim_signal, P_SIM);
}

/*****************************************************************************/
//  Description:    Get the data in chip address
//    Global resource dependence:
//  Author:         lichd
//    Note:
//       Make sure it's byte aligned addr if you want to get a byte in this addr.
//       Make sure it's hword aligned addr if you want to get a hword in this addr.
//       Make sure it's word aligned addr if you want to get a word in this addr.
/*****************************************************************************/
uint32 DIAG_GetReg(
                   uint32 addr,  // Maybe byte/hword/word aligned
                   uint32 size   // 1-byte,2-hword,4-word
                   )
#ifdef WIN32
{
    uint8*   src_ptr8  = (uint8*)(addr);
    uint16*  src_ptr16 = (uint16*)(addr);
    uint32*  src_ptr32 = (uint32*)(addr);
    uint32 mcu_data=0;

    SCI_PASSERT( ((size==1)                || \
                 ((size==2) && !(addr&0x01))|| \
                 ((size==4) && !(addr&0x03))), \
                 ("addr=0x%x,size=0x%x",addr,size));

    switch( size )
    {
        case 1:
            mcu_data = (*src_ptr8);
            break;
        case 2:
            mcu_data = (*src_ptr16);
            break;
        case 4:
            mcu_data = (*src_ptr32);
            break;
        default:
            break;
    }

    return (mcu_data);
}
#else
{
    uint32      mcu_data=0;
    uint32      mcu_addr= addr&(~0x03);
    uint32      pos_offset=addr&(0x03);

    SCI_PASSERT( ((size==1)                || \
                 ((size==2) && !(addr&0x01))|| \
                 ((size==4) && !(addr&0x03))), \
                 ("addr=0x%x,size=0x%x",addr,size));

    if(!ANA_IS_ANA_REG(addr))
    {
        mcu_data = CHIP_REG_GET(mcu_addr);
    }
    else
    {
        mcu_data = ANA_REG_GET (mcu_addr);
    }

#if defined(CHIP_ENDIAN_BIG)
    mcu_data = (mcu_data>>((3-pos_offset)<<3));
#else
    mcu_data = (mcu_data>>(pos_offset<<3));
#endif

    switch( size )
    {
        case 1:
            mcu_data = mcu_data&(0xFF);
            break;
        case 2:
            mcu_data = mcu_data&(0xFFFF);
            break;
        case 4:
            mcu_data = mcu_data&(0xFFFFFFFF);
            break;
        default:
            break;
    }

    return mcu_data;
}
#endif //ENDIF WIN32

void DIAG_SetReg(
                   uint32 addr,  // Maybe byte/hword/word aligned
                   uint32 size,  // 1-byte,2-hword,4-word
                   uint32 data   // Data to be written
                   )
#ifdef WIN32
{
    uint8*   dst_ptr8  = (uint8*)(addr);
    uint16*  dst_ptr16 = (uint16*)(addr);
    uint32*  dst_ptr32 = (uint32*)(addr);

    uint32 mcu_data=data;

    SCI_PASSERT( ((size==1)                || \
                 ((size==2) && !(addr&0x01))|| \
                 ((size==4) && !(addr&0x03))), \
                 ("addr=0x%x,size=0x%x",addr,size));

    switch( size )
    {
        case 1:
            mcu_data = mcu_data&(0xFF);
            *dst_ptr8 = mcu_data;
            break;
        case 2:
            mcu_data = mcu_data&(0xFFFF);
            *dst_ptr16 = mcu_data;
            break;
        case 4:
            mcu_data = mcu_data&(0xFFFFFFFF);
            *dst_ptr32 = mcu_data;
            break;
        default:
            break;
    }
}
#else
{
    uint32      mcu_data=0;
    uint32      mcu_addr= addr&(~0x03);
    uint32      pos_offset=addr&(0x03);

    SCI_PASSERT( ((size==1)                || \
                 ((size==2) && !(addr&0x01))|| \
                 ((size==4) && !(addr&0x03))), \
                 ("addr=0x%x,size=0x%x,data=0x%x",addr,size,data));

    if(!ANA_IS_ANA_REG(addr))
    {
        mcu_data = CHIP_REG_GET(mcu_addr);
    }
    else
    {
        mcu_data = ANA_REG_GET (mcu_addr);
    }

#if defined(CHIP_ENDIAN_BIG)
    pos_offset = ((3-pos_offset)<<3);
#else
    pos_offset = (pos_offset<<3);
#endif

    switch( size )
    {
        case 1:
            data = data&0xFF;
            mcu_data &= ~((uint32)0xFF<<pos_offset);
            break;
        case 2:
            data = data&(0xFFFF);
            mcu_data &= ~((uint32)0xFFFF<<pos_offset);
            break;
        case 4:
            data = data&(0xFFFFFFFF);
            mcu_data &= ~(0xFFFFFFFF<<pos_offset);
            break;
        default:
            break;
    }
    mcu_data |= (data<<pos_offset);

    if(!ANA_IS_ANA_REG(addr))
    {
        CHIP_REG_SET(mcu_addr, mcu_data);
    }
    else
    {
        ANA_REG_SET (mcu_addr, mcu_data);
    }
}
#endif //ENDIF WIN32

// @Xueliang.Wang add (2002-12-30)
/*****************************************************************************/
//  Description:    Handle MCU Read or Write command.
//    Global resource dependence:
//  Author: Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleMcuAccess( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    uint32      status          = 0;
    uint16      sig_code        = 0;
    uint8        *rsp_pkt        = SCI_NULL;
    uint32      rsp_pkt_len     = 0;
    MSG_HEAD_T  *msg_head_ptr   = (MSG_HEAD_T *)src_ptr;

    SCI_ASSERT((SCI_NULL == *dest_ptr) && (SCI_NULL != src_ptr));/*assert verified*/

    // Get Signal code.
    sig_code     = *(uint16 *)(msg_head_ptr + 1);
    if (sig_code == (uint16)MCU_WRITE)
    {
        /*
        // Input data packet is :
            ------------------------------
            | MSG_HEAD_T struct
            ------------------------------
            | DIAG_MCU_WRITE_T struct
            ------------------------------
            | Data to be writen to MCU
            ------------------------------

        // Response data packet is :
            ------------------------------
            | MSG_HEAD_T struct
            ------------------------------
            | DIAG_MCU_WRITE_T struct
            ------------------------------
        */

        DIAG_MCU_WRITE_PTR wr_head  = (DIAG_MCU_WRITE_PTR)(msg_head_ptr + 1);
        uint32  mode                = wr_head->mode;
        void    *addr               = wr_head->addr;
        void    *data_ptr           = wr_head + 1;
        uint32  size_to_wr          = wr_head->access_size;    // size to write
        uint32  size_wr             = 0;                    // size real write

        // Check input parameter.
        //SCI_TRACE_LOW:"MCU Write Addr:%p  Mode:%d  Size:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2531_112_2_18_1_3_59_7,(uint8*)"dd", addr, mode, size_to_wr);
        status = CheckMcuAccessParam((uint32)addr, size_to_wr, MCU_WRITE, mode);
        if (MCU_ACCESS_SUCCESS == status)
        {
            // Write data to memory.
            if (BYTE_ACCESS == mode)
            {
                uint8    *wr_src_ptr     = (uint8 *)data_ptr;
                uint8    *wr_dest_ptr    = (uint8 *)addr;
                for (size_wr = 0; size_wr < size_to_wr; size_wr++)
                {
                    DIAG_SetReg((uint32)(wr_dest_ptr++), sizeof(uint8), *wr_src_ptr++);
                }
            }
            else if (WORD_ACCESS == mode)
            {
                uint16    *wr_src_ptr     = (uint16 *)data_ptr;
                uint16    *wr_dest_ptr    = (uint16 *)addr;
                for (size_wr = 0; size_wr < size_to_wr; size_wr++)
                {
                    DIAG_SetReg((uint32)(wr_dest_ptr++), sizeof(uint16), *wr_src_ptr++);
                }
            }
            else    // DWORD_ACCESS
            {
                uint32    *wr_src_ptr     = (uint32 *)data_ptr;
                uint32    *wr_dest_ptr    = (uint32 *)addr;
                for (size_wr = 0; size_wr < size_to_wr; size_wr++)
                {
                    DIAG_SetReg((uint32 )(wr_dest_ptr++), sizeof(uint32), *wr_src_ptr++);
                }
            }
        }

        // Generate response packet.
        rsp_pkt_len = sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_WRITE_T);
        rsp_pkt     = (uint8 *)SCI_ALLOC_APP(rsp_pkt_len);
        SCI_ASSERT(SCI_NULL != rsp_pkt);/*assert verified*/
        // Set message head value.
        SCI_MEMCPY(rsp_pkt, msg_head_ptr, rsp_pkt_len);
        ((MSG_HEAD_T *)rsp_pkt)->len    = rsp_pkt_len;
        ((DIAG_MCU_WRITE_T *)(rsp_pkt + sizeof(MSG_HEAD_T)))->Size           = sizeof(DIAG_MCU_WRITE_T);
        ((DIAG_MCU_WRITE_T *)(rsp_pkt + sizeof(MSG_HEAD_T)))->access_size    = size_wr;
        ((DIAG_MCU_WRITE_T *)(rsp_pkt + sizeof(MSG_HEAD_T)))->addr           = (void *)status;

        *dest_ptr     = rsp_pkt;
        *dest_len_ptr = rsp_pkt_len;
    }
    else if (sig_code == (uint16)MCU_READ)
    {
        /*
        // Input data packet is :
            ------------------------------
            | MSG_HEAD_T struct
            ------------------------------
            | DIAG_MCU_READ_T struct
            ------------------------------

        // Response data packet is :
            If read successfully:
            ------------------------------
            | MSG_HEAD_T struct
            ------------------------------
            | DIAG_MCU_READ_T struct
            ------------------------------
            | Data read from MCU
            ------------------------------

           If read successfully:
            ------------------------------
            | MSG_HEAD_T struct
            ------------------------------
            | DIAG_MCU_READ_T struct
            ------------------------------
        */

        DIAG_MCU_READ_PTR   rd_head = (DIAG_MCU_READ_PTR)(msg_head_ptr + 1);
        uint32  mode                = rd_head->mode;
        void    *addr               = rd_head->addr;
        uint32  size_to_rd          = rd_head->access_size;    // Size to be read
        uint32  size_rd             = 0;                    // Size real read

        // Check input parameter.
        //SCI_TRACE_LOW:"MCU Read Addr:%p  Mode:%d  Bytes:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2614_112_2_18_1_3_59_8,(uint8*)"dd", addr, mode, size_to_rd);
        status = CheckMcuAccessParam((uint32)addr, size_to_rd, MCU_READ, mode);
        if (MCU_ACCESS_SUCCESS == status)
        {
            // Read data from memory.
            if (BYTE_ACCESS == mode)
            {
                uint8    *rd_src_ptr     = (uint8 *)addr;
                uint8    *rd_dest_ptr    = 0;

                // Malloc a memory to store data read from memory.
                rsp_pkt_len = sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_READ_T) + size_to_rd;
                rsp_pkt     = (uint8 *)SCI_ALLOC_APP(rsp_pkt_len);
                SCI_ASSERT(SCI_NULL != rsp_pkt);/*assert verified*/

                rd_dest_ptr = (uint8 *)(rsp_pkt + sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_READ_T));
                for (size_rd = 0; size_rd < size_to_rd; size_rd++)
                {
                    *rd_dest_ptr++ = DIAG_GetReg((uint32)(rd_src_ptr++), sizeof(uint8));
                }
            }
            else if (WORD_ACCESS == mode)
            {
                uint16    *rd_src_ptr     = (uint16 *)addr;
                uint16    *rd_dest_ptr    = 0;

                // Malloc a memory to store data read from memory.
                rsp_pkt_len = sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_READ_T) + size_to_rd * 2;
                rsp_pkt     = (uint8 *)SCI_ALLOC_APP(rsp_pkt_len);
                SCI_ASSERT(SCI_NULL != rsp_pkt);/*assert verified*/

                rd_dest_ptr = (uint16 *)(rsp_pkt + sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_READ_T));
                for (size_rd = 0; size_rd < size_to_rd; size_rd++)
                {
                    *rd_dest_ptr++ = DIAG_GetReg((uint32)(rd_src_ptr++), sizeof(uint16));
                }
            }
            else    // DWORD_ACCESS
            {
                uint32    *rd_src_ptr     = (uint32 *)addr;
                uint32    *rd_dest_ptr    = 0;

                // Malloc a memory to store data read from memory.
                rsp_pkt_len = sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_READ_T) + size_to_rd * 4;
                rsp_pkt     = (uint8 *)SCI_ALLOC_APP(rsp_pkt_len);
                SCI_ASSERT(SCI_NULL != rsp_pkt);/*assert verified*/

                rd_dest_ptr = (uint32 *)(rsp_pkt + sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_READ_T));
                for (size_rd = 0; size_rd < size_to_rd; size_rd++)
                {
                    *rd_dest_ptr++ = DIAG_GetReg((uint32)(rd_src_ptr++), sizeof(uint32));
                }
            }
        }
        else
        {
            size_rd = 0;

            // Malloc a memory to store data read from memory.
              rsp_pkt_len = sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_READ_T);
            rsp_pkt     = (uint8 *)SCI_ALLOC_APP(rsp_pkt_len);
            SCI_ASSERT(SCI_NULL != rsp_pkt);/*assert verified*/
        }

        // Set message head value.
        SCI_MEMCPY(rsp_pkt, msg_head_ptr, sizeof(MSG_HEAD_T) + sizeof(DIAG_MCU_READ_T));
        ((MSG_HEAD_T *)rsp_pkt)->len    = rsp_pkt_len;
        ((DIAG_MCU_READ_T *)(rsp_pkt + sizeof(MSG_HEAD_T)))->Size              = rsp_pkt_len - sizeof(MSG_HEAD_T);
        ((DIAG_MCU_READ_T *)(rsp_pkt + sizeof(MSG_HEAD_T)))->access_size    = size_rd;
        ((DIAG_MCU_READ_T *)(rsp_pkt + sizeof(MSG_HEAD_T)))->addr              = (void *)status;

        *dest_ptr       = rsp_pkt;
        *dest_len_ptr   = rsp_pkt_len;
    }
    else
    {
        // Should not run to here.
           HandleBadCommand(dest_ptr, dest_len_ptr, src_ptr, src_len);
           //SCI_TRACE_LOW:"ERROR: MCU access Command is ERROR !!!"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2692_112_2_18_1_3_59_9,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description:    Check parameters of access MCU command.
//    Global resource dependence:
//  Author:         Xueliang.Wang
//    Note:           If the parameters is valid, return MCU_ACCESS_SUCCESS;
//                  else return error code.
//
//                  ADDRESS                      ACCESS_MODE     Read/Write
//                  ECS0                         B/W/D           R
//                  Other ECS                    B/W/D           R/W
//                  Internal Share Memory        B               R2
//                                               W               R/W
//                  ECS CTL register             D               R/W
//                  Peripheral Address           B/W/D           R/W
//                  Version ID                   D               R
/*****************************************************************************/
LOCAL uint32 CheckMcuAccessParam(
    uint32  start_addr,     // Access start address.
    uint32  size,           // Size to be access.
    uint32  cmd_type,       // MCU_READ or MCU_WRITE
    uint32  access_mode     // BYTE_ACCESS or WORD_ACCESS or DWORD_ACCESS
    )
{
    uint32  status          = MCU_ACCESS_SUCCESS;
    uint32  end_addr        = 0;
    uint32  acc_condition   = 0;
    DRVEXP_MEMTBL_PARAMSTATUS_E  param_stat;

#ifndef WIN32

    // Check access size and start address alignment.
       if (BYTE_ACCESS == access_mode)
    {
        acc_condition |= BYTE_ACC;
        end_addr = size;
    }
    else if (WORD_ACCESS == access_mode)
    {
        if (start_addr & 0x1)
        {
            status = MCU_ACCESS_ADDR_ERR;
        }
        else
        {
            acc_condition |= WORD_ACC;
            end_addr = size * 2;
        }
    }
    else    // if (DWORD_ACCESS == access_mode)
    {
        if (start_addr & 0x3)
        {
            status = MCU_ACCESS_ADDR_ERR;
        }
        else
        {
            acc_condition |= DWORD_ACC;
            end_addr = size * 4;
        }
    }

    if (MCU_ACCESS_ADDR_ERR == status)
    {
        //SCI_TRACE_LOW:"ERROR: This address cann't access in this mode, alignment is error."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2757_112_2_18_1_3_59_10,(uint8*)"");
        return MCU_ACCESS_ADDR_ERR;
    }
    else if ((end_addr == 0) || (end_addr > DIAG_ACCESS_MCU_MAX_LENGTH))
    {
        //SCI_TRACE_LOW:"ERROR: Access size is 0 or too large(0< size(conver to bytes) < %d)."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2763_112_2_18_1_3_59_11,(uint8*)"d", DIAG_ACCESS_MCU_MAX_LENGTH);
        return MCU_ACCESS_SIZE_ERR;
    }
    else
    {
        end_addr = start_addr + end_addr - 1;
    }
    //add the read/write condition
    if (MCU_READ == cmd_type)
    {
        acc_condition |= READ_ACC;
    }
    else
    {
        acc_condition |= WRITE_ACC;
    }

    param_stat = SCI_GetParamStatus(start_addr,end_addr,acc_condition);/*lint !e655 */
    if(param_stat & (DRVEXP_MEMTBL_START_IN|DRVEXP_MEMTBL_COND_MATCH))/*lint !e655 */
    {
        return MCU_ACCESS_SUCCESS;
    }
    else if (param_stat & (DRVEXP_MEMTBL_START_IN|DRVEXP_MEMTBL_COND_NOT_MATCH))/*lint !e655 */
    {
        return MCU_ACCESS_MODE_ERR;
    }
    else
    {
        return MCU_ACCESS_RANGE_ERR;
    }

#endif

}

/*****************************************************************************/
//  Description:    Change MCU mode to "mode", then reset MCU.
//    Global resource dependence:
//  Author: Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleChangeMode(    // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    MCU_MODE_E   mode;
    uint16         freq;
    uint32   seq_num;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *) src_ptr;

    // Get mode, and Set MCU mode.
    mode = (MCU_MODE_E)msg_head->subtype;
    seq_num = msg_head->seq_num;

   //SCI_TRACE_LOW:"\"\"CHANGE MODE = %d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_2818_112_2_18_1_3_59_12,(uint8*)"d",mode);
    // Send response packet.
    SIO_SendPacket((void *)src_ptr, sizeof(MSG_HEAD_T));

    // delay 300ms, for SP7100A product. tools group needed.
    {
        uint32  tick_1, tick_2;

        tick_1 = SCI_GetTickCount();

        //The more conventional form of infinite loop prefix is for(;;)
        //while( 1 )
        for(;;)
        {
            tick_2 = SCI_GetTickCount();
            if( 300 < (tick_2 - tick_1) )
                break;
        }
    }


    SCI_DisableIRQ();
    if (BOOT_RESET_MODE == mode)
    {
        // Reset to boot mode.
        CHIP_BootReset();
    }
    else if (PRODUCTION_MODE== mode)
    {
        //eric.zhou add to handle production functin
        RM_SetCalibrationMode(SCI_TRUE);
    }
    else
    {
        /*in CALIBRATION_POST_MODE, should set the frequency parameter to global variable
           msg_head->seq_num higher 2 bytes carry the frequency information; big endian
           frequency information includes frequency(11bit) and band(4bit);
           bit    0   1   2   3   4   5   6   7    8   9   10  11  12  13  14  15
                   |                                |                                         |
                           seq_num[0]                       seq_num[1]
                   |                                     |   |    |    |    |    |    |   |
                   |                                       frequency(low 8 bit)
                   |   |   |        |   |   |   |
         frequency(high 3 bit)      band(4 bits)
            */
        switch(mode){
            case CALIBRATION_POST_MODE:
            case CALIBRATION_POST_NO_LCM_MODE:
            case TD_CALIBRATION_POST_MODE:
            case TD_CALIBRATION_POST_NO_LCM_MODE:
                freq = seq_num>>16;
                POWER_SetCalibrationPostModeFreq(freq);
                break;
            default:
                break;
        }
        /* @Xueliang.wang modify; CR4916; (2003-09-24) */
        //DEV_SetMode(mode);
        POWER_SetResetMode(mode);
        /* End; @Xueliang.wang modify; CR4916; (2003-09-24) */

        /* @Xueliang.Wang deleted; CR:5041; (2003-10-07) */
        // Because it will reset in function POWER_SetResetMode().
        //POWER_Reset();
        /* End; @Xueliang.Wang deleted; CR:5041; (2003-10-07) */
    }
    SCI_RestoreIRQ();
}

/*****************************************************************************/
//  Description:    Handle NVItem Read or Write command.
//    Global resource dependence:
//  Author:         Xueliang.Wang
//    Note:
/*****************************************************************************/
LOCAL void HandleNvitemAccess( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
      uint8   status;
    uint16  len;
    uint32  offset;
    uint32  address;
    uint16  id;
    uint8*  data_ptr = NULL;
    MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)src_ptr;

    switch (msg_head->subtype)
    {
    case NVITEM_READ:
        {
            id = *(uint16 *)(msg_head + 1);
            len = NVITEM_GetLength(id);

            if (len > 0)
            {
                *dest_len_ptr = sizeof(MSG_HEAD_T) + sizeof(uint16) + len;
                *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
                SCI_ASSERT(SCI_NULL != *dest_ptr);/*assert verified*/

                // Copy message head.
                SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T) + sizeof(uint16));

                /*to keep allignment*/
                data_ptr = (uint8*)SCI_ALLOC_APP(len);
                SCI_ASSERT(data_ptr != NULL);/*assert verified*/
                // Read NVItem.
                status = EFS_NvitemRead(id, len, data_ptr);
                if (NVERR_NONE != status)
                {
                    // Read failed.
                    *dest_len_ptr  = sizeof(MSG_HEAD_T);
                }

                SCI_MEMCPY((uint8*)(*dest_ptr + sizeof(MSG_HEAD_T) + sizeof(uint16)),data_ptr,len);
                SCI_FREE(data_ptr);

                ((MSG_HEAD_T  *)(*dest_ptr))->len      = *dest_len_ptr;
                ((MSG_HEAD_T  *)(*dest_ptr))->subtype  = status;
            }
            else
            {
                // No this ID or No content of it.
                msg_head->subtype = NVERR_NOT_EXIST;
                ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
            }

        }
        break;

    case NVITEM_WRITE:
        {
            id = *(uint16 *)(msg_head + 1);
            len = msg_head->len - sizeof(MSG_HEAD_T) - sizeof(uint16);

            /*to keep allignment*/
            data_ptr = (uint8*)SCI_ALLOC_APP(len);
            SCI_ASSERT(data_ptr != NULL);/*assert verified*/

            SCI_MEMCPY(data_ptr,(uint8 *)(src_ptr + sizeof(MSG_HEAD_T) + sizeof(uint16)),len);

            msg_head->subtype = EFS_NvitemWrite(id, len, data_ptr, 1);

            SCI_FREE(data_ptr);

            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
        }
        break;

    case NVITEM_GETLENGTH:
        {
            id = *(uint16 *)(msg_head + 1);

            // Get NVItem length.
            len = NVITEM_GetLength(id);

            *dest_len_ptr = sizeof(MSG_HEAD_T) + 2 * sizeof(uint16);
            *dest_ptr = (uint8*)SCI_ALLOC_APP(*dest_len_ptr);
            SCI_ASSERT(SCI_NULL != *dest_ptr);/*assert verified*/

            SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T) + sizeof(uint16));
            ((MSG_HEAD_T*)(*dest_ptr))->subtype = 0;
            ((MSG_HEAD_T*)(*dest_ptr))->len = *dest_len_ptr;
            *(uint16*)(*dest_ptr + sizeof(MSG_HEAD_T) + sizeof(uint16)) = len;
        }
        break;

    case NVITEM_DELETE:
        {
            uint16 from = *(uint16*)(msg_head + 1);
            uint16 to   = *(uint16*)((uint8*)msg_head + sizeof(MSG_HEAD_T) + sizeof(uint16));

            msg_head->subtype = EFS_NvitemDelete(from, to);
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
        }
        break;

    case NVITEM_PRODUCT_CTRL_READ:
        {
            /* The offset field is ignored */
            offset  = *(uint16 *)((uint32)msg_head + sizeof(MSG_HEAD_T));
            len     = *(uint16 *)((uint32)msg_head + sizeof(MSG_HEAD_T) + sizeof(uint16));
            address = FLASH_GetProductInfoAddr() + offset;  /*lint !e737 */

            *dest_len_ptr = sizeof(MSG_HEAD_T) + 2 * sizeof(uint16) + len;
            *dest_ptr = (uint8 *)SCI_ALLOC_APP(*dest_len_ptr);
            SCI_ASSERT(SCI_NULL != *dest_ptr);/*assert verified*/

            // Copy message head.
            SCI_MEMCPY(*dest_ptr, msg_head, sizeof(MSG_HEAD_T) + 2 * sizeof(uint16));

            /* Read product control information */
            // Read NVItem.
            status = EFS_Direct( SCI_TRUE, *dest_ptr + sizeof(MSG_HEAD_T) + 2 * sizeof(uint16),
                        address, len);
            if (status) {
                /* Operation failed */
                *dest_len_ptr = sizeof(MSG_HEAD_T);
            }

            ((MSG_HEAD_T  *)(*dest_ptr))->len       = *dest_len_ptr;
            ((MSG_HEAD_T  *)(*dest_ptr))->subtype   = status;
        }
        break;

    case NVITEM_PRODUCT_CTRL_WRITE:
        {
            /* The offset field is ignored */
            offset  = *(uint16 *)((uint32)msg_head + sizeof(MSG_HEAD_T));
            len     = *(uint16 *)((uint32)msg_head + sizeof(MSG_HEAD_T) + sizeof(uint16));
            address = FLASH_GetProductInfoAddr() + offset;    /*lint !e737 */

            /* Write product control information. */
            // Write NVItem.
                status = NVITEM_UpdateProductInfo(address, (uint8 *) (src_ptr + sizeof(MSG_HEAD_T) + 2 * sizeof(uint16)), len);
            msg_head->subtype = (status == SCI_TRUE)?0:1 ;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
        }
        break;

    case NVITEM_PRODUCT_CTRL_ERASE:
        {
            //We do nothing while receiving NVITEM_PRODUCT_CTRL_ERASE
            SCI_MEMCPY(&s_product_info_erase, src_ptr, sizeof(MSG_HEAD_T));
                EraseProductInfoCallback(0, 0, 0);

        }
        break;
    case NVITEM_USB_AUTO_MODE:
        {
            id = *((uint8*)(msg_head) + sizeof(MSG_HEAD_T));

            //SCI_TRACE_LOW:"NVITEM_USB_AUTO_MODE = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_3053_112_2_18_1_4_0_13,(uint8*)"d", id);
            REFPARAM_SetUsbAutoModeFlag(id);
            msg_head->subtype = 0; // return always
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);
        }
        break;
    default:
        {
            msg_head->subtype = ATCMD_BADCOMMAND;
            ATCOMMON_GenerateRspMsg(dest_ptr, dest_len_ptr, msg_head);

               //SCI_TRACE_LOW:"ERROR: NVItem access type is ERROR !!!"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_3064_112_2_18_1_4_0_14,(uint8*)"");
        }
        break;
    }
}

/*****************************************************************************/
//  Description:    Get Mcu mode
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void DIAG_GetMcuMode( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    uint32      ret = 0;
    MSG_HEAD_T *msg;

#ifndef WIN32

    msg = (MSG_HEAD_T *) SCI_ALLOC_APP(sizeof(MSG_HEAD_T) + sizeof(uint32));
    ret = RM_GetMcuMode();
    // Send response packet.
    SCI_MEMCPY(msg, src_ptr, sizeof(MSG_HEAD_T));
    msg->subtype = 0;           //0 indicates successful according with tools
    msg->len = sizeof(MSG_HEAD_T) + sizeof(uint32);

    SCI_MEMCPY((void *) (msg + 1), &ret, sizeof(uint32));

    *dest_ptr = (uint8 *)msg;
    *dest_len_ptr = sizeof(MSG_HEAD_T) + sizeof(uint32);

#endif    //win32
}
#if 0
#define MAX_PRODUCT_TEST_EXTEND    1024
static uint32 product_test_extend_response[ (MAX_PRODUCT_TEST_EXTEND >> 2) + 16 ];

LOCAL void HandleProductionTestExtendCommand( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    MSG_HEAD_T  *msg_head;
    uint32       size, rsp_len;

#ifndef WIN32

    SCI_MEMSET( product_test_extend_response, 0, MAX_PRODUCT_TEST_EXTEND );

    msg_head =     (MSG_HEAD_T *) src_ptr;
    rsp_len = SCI_ProductTest_Extend( (uint8 *)(msg_head + 1), src_len - sizeof(MSG_HEAD_T),
                (uint8 *)product_test_extend_response, MAX_PRODUCT_TEST_EXTEND );

    // build response msg
    SCI_ASSERT( rsp_len < MAX_PRODUCT_TEST_EXTEND );/*assert verified*/

    size = sizeof(MSG_HEAD_T) + rsp_len;
    msg_head = (MSG_HEAD_T *)SCI_ALLOC_APP( size );

    // Copy message head.
    SCI_MEMCPY( msg_head, src_ptr, sizeof(MSG_HEAD_T) );

    // copy response msg
    SCI_MEMCPY( msg_head + 1, product_test_extend_response, rsp_len );

    *dest_ptr       = (uint8 *)(msg_head);
    *dest_len_ptr   = size;
#endif
}
#endif
/*****************************************************************************/
//  Description:    Handle battery supply command
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void HandleBatteryVoltage( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    MSG_POWER_SUPPLY_T * msg_head_ptr = PNULL;
    uint32 battery_voltage = 0;
    uint32 adc_value = 0;
    uint8 i;

#ifndef WIN32
    for(i = 0; i < 4; i++)
    {
        adc_value += CHGMNG_GetVBATADCValue();
    }
    adc_value = adc_value >> 2;

    battery_voltage = CHGMNG_AdcvalueToVoltage(adc_value);

//    SCI_TRACE_LOW("HandleBatteryVoltage battery_voltage=%d", battery_voltage);

    msg_head_ptr = (MSG_POWER_SUPPLY_T *)SCI_ALLOC_APP( sizeof(MSG_POWER_SUPPLY_T) );

    // Copy message head.
    SCI_MEMCPY( msg_head_ptr, src_ptr, sizeof(MSG_HEAD_T) );

    msg_head_ptr->batt_voltage  =  battery_voltage / 10;
    msg_head_ptr->msg_head.len = sizeof(MSG_HEAD_T) + sizeof(uint32);

    *dest_ptr       = (uint8 *)(msg_head_ptr);
    *dest_len_ptr   = sizeof(MSG_POWER_SUPPLY_T);

#endif    //win32

}
/*****************************************************************************/
//  Description:    Handle ADC value to voltage
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void HandleADCToVoltage( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    MSG_POWER_SUPPLY_T * msg_head_ptr = PNULL;
    uint32 voltage = 0;
    uint32 adc_value = 0;
    volatile  uint32   adc_src, adc_result;
    uint8 i;
    uint8 scale;


    MSG_ADC_TO_VOLTAGE_T   *msg_head = (MSG_ADC_TO_VOLTAGE_T *) src_ptr;
#ifndef WIN32
    // Get ADC source
    adc_src = msg_head->channel_id;

#if 0 /* old code */
    if (adc_src <= ADC_CHANNEL_MAX)  //6531EFM use 0~9channel,pc tool dispose start from 1(the first channel)
    {
        if(0 == adc_src)
        {
            adc_src = 1;
        }

        adc_result = 0;


 #ifdef DUAL_BATTERY_SUPPORT
        if( SCI_TRUE == HandleDualBatChannelSel(adc_src) )
        {
            for (i = 0 ; i < 16 ; i++)
            {
                adc_result += ADC_GetResultDirectly ( (adc_src), ADC_SCALE_1V2);
            }

            #if 0
            //if vbatm cann't switchover vataux channel,just make CFT soft pass the calibration
            if(adc_result == 0)
            {
                adc_result = (0x01 << 4);
            }
            #endif
        }
        else
 #endif
        {
             if(adc_src ==ADC_CHANNEL_1 ||adc_src ==ADC_CHANNEL_6 ||adc_src ==ADC_CHANNEL_0){
               scale=ADC_SCALE_1V250;
            }else{
               scale=ADC_SCALE_3V233;
            }
//            if(adc_src -1==ADC_CHANNEL_6){
//              adc_result= _sprd_codec_ap_headmic_adcget();
//            }
//            else{



                for (i = 0 ; i < 16 ; i++)
                {
                   adc_result += ADC_GetResultDirectly ( (adc_src - 1), scale);
                }
                adc_result >>= 4;
//            }

        }
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_3442_112_2_18_1_4_0_15,(uint8*)"dd", adc_src, adc_result);

        adc_value= (adc_result & 0xFFFF);
    }
    else
    {
        // invalid subtype, invalid adc source.
       adc_value = 0xFFFF;
    }

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if(adc_src <= ADC_CHANNEL_MAX){
        switch(adc_src -1)
        {
            case ADC_CHANNEL_0:
                voltage=CHGMNG_AdcvalueToVoltageforSmallScale(adc_value);//small scale for test
                break;
            case ADC_CHANNEL_1:
                // which scale to voltage
                if(msg_head->scale)
                {
                    voltage = CHGMNG_AdcvalueToVoltageforSmallScale(adc_value)*10.25/4;//big scale for test
                }
                else
                {
                    voltage=CHGMNG_AdcvalueToVoltageforSmallScale(adc_value);//small scale for test
                }
                break;
            case ADC_CHANNEL_2:
                voltage = CHGMNG_AdcvalueToVoltage(adc_value);//big scale for test
                break;
            case ADC_CHANNEL_3:
                voltage = _CHGMNG_AdcValueToVChgVol(adc_value);
                break;

            case ADC_CHANNEL_6:
                voltage = Headmic_AdcvalueToVoltage(adc_value);
                break;
            case ADC_CHANNEL_4:
            case ADC_CHANNEL_5:
            case ADC_CHANNEL_7:
            case ADC_CHANNEL_8:
            case ADC_CHANNEL_9:
            // which scale to voltage
                if(msg_head->scale)
                {
                    voltage = CHGMNG_AdcvalueToVoltage(adc_value);//big scale for test
                }
                else
                {
                    voltage=CHGMNG_AdcvalueToVoltageforSmallScale(adc_value);//small scale for test
                }
                break;
            default:
                voltage = CHGMNG_AdcvalueToVoltage(adc_value);
        }
    }
#else
    voltage = CHGMNG_AdcvalueToVoltage(adc_value);
#endif


#else /* new code for W217 para form spec */
        SCI_ASSERT( adc_src < ADC_CHANNEL_MAX );

        adc_src = (adc_src == ADC_CHANNEL_0) ? ADC_CHANNEL_1 : adc_src;

        switch(adc_src) /* Scale configuration, adding as needed */
        {
            case ADC_CHANNEL_2:
            case ADC_CHANNEL_8:
                scale = ADC_SCALE_1V250;
            break;
            case ADC_CHANNEL_5:
                scale = ADC_SCALE_5V000;
            break;
            case ADC_CHANNEL_7:
            case ADC_CHANNEL_9:
                scale = ADC_SCALE_3V233;
            break;
            default:
                scale = ADC_SCALE_3V233;
            break;
        }
        voltage = _CHGMNG_GetChannelVol (adc_src, scale);
#endif

    msg_head_ptr = (MSG_POWER_SUPPLY_T *)SCI_ALLOC_APP( sizeof(MSG_POWER_SUPPLY_T) );

    // Copy message head.
    SCI_MEMCPY( msg_head_ptr, src_ptr, sizeof(MSG_HEAD_T) );

    msg_head_ptr->batt_voltage  =  voltage;
    msg_head_ptr->msg_head.len = sizeof(MSG_HEAD_T) + sizeof(uint32);

    *dest_ptr       = (uint8 *)(msg_head_ptr);
    *dest_len_ptr   = sizeof(MSG_POWER_SUPPLY_T);
#endif
}

/*****************************************************************************/
//  Description:    Handle deep sleep command
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void HandleRFQuery( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    xSignalHeader rf_signal;

    rf_signal = (xSignalHeader) SCI_ALLOC_APP(src_len - sizeof(MSG_HEAD_T));

    SCI_MEMCPY(rf_signal, src_ptr + sizeof(MSG_HEAD_T), src_len - sizeof(MSG_HEAD_T));

    rf_signal->Sender = DIAG;

//    sim_seq_num = ((MSG_HEAD_T *) src_ptr)->seq_num;
    *dest_ptr = SCI_NULL;

    SCI_MEMCPY((void*)&s_rf_query_req, src_ptr, sizeof(MSG_HEAD_T));

    SCI_SendSignal(rf_signal, P_RRC);
}

/*****************************************************************************/
//  Description:    callback function for erase customer info completely
//                  completely
//    Global resource dependence:
//  Author:         Shijun.cui
//    Note:
/*****************************************************************************/
LOCAL void EraseProductInfoCallback( // None
    EFS_ERROR_E     error_code, // efs operation error
    EFS_RESULT         result,     // operation result
    EFS_PARAM         param       // parameter to efs
    )
{
    // notify the tools production information erase ok
    if (NVERR_NONE == error_code)
    {
        s_product_info_erase.subtype = 0;// indicate successful in erase production info
    }
    else
    {
        // failed in erase production info
    }
    SIO_SendPacket((void*)&s_product_info_erase, sizeof(MSG_HEAD_T));
}

#if 0
/*****************************************************************************/
//  Description:    diag send response to diag UART
//    Global resource dependence:
//  Author:         Jimmy.Jia
//    Note:
/*****************************************************************************/
LOCAL void DIAG_HandleOwnMsg (xSignalHeader signal_ptr)
{
    MSG_HEAD_T    *msg;
    uint16        size;

    // Malloc for send to SIO.
    size = signal_ptr->SignalSize + sizeof(MSG_HEAD_T);
    msg = (MSG_HEAD_T *)SCI_ALLOC_APP(size);

    CM_COPY_MSG_HEAD(msg, sim_seq_num, size, DIAG_SIM_F, 0)

    SCI_MEMCPY(msg + 1, signal_ptr, signal_ptr->SignalSize);

    SIO_SendPacket(msg, size);

    SCI_FREE(msg);
}
#endif

/*****************************************************************************/
//  Description:    Handle autotest information
//    Global resource dependence:
//  Author:         Daniel.Luo
//    Note:
/*****************************************************************************/
LOCAL int DIAG_Trace_Autotest( // None
    //uint8 **dest_ptr,       // Pointer of the response message.
    //uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    int status = -1;

    MSG_HEAD_T    *msg;

    // Malloc for send to SIO.
    msg = (MSG_HEAD_T *)SCI_ALLOC_APP(src_len + sizeof(MSG_HEAD_T));
    //CM_COPY_MSG_HEAD(msg, sim_seq_num, src_len + sizeof(MSG_HEAD_T), DIAG_AUTOTEST_INFO_F, 0)
    msg->seq_num = sim_seq_num;
    msg->len     = src_len + sizeof(MSG_HEAD_T);
    msg->type    = DIAG_AUTOTEST_INFO_F;
    msg->subtype = 0;

    SCI_MEMCPY(msg + 1, src_ptr, src_len);

#ifdef WIN32
    status = SIO_SendPacket(msg, src_len + sizeof(MSG_HEAD_T));
#else
    status = SIO_SendFrame(msg, src_len + sizeof(MSG_HEAD_T));
#endif
    SCI_FREE(msg);

    return status;
}

/*****************************************************************************/
//  Description:    Handle autotest information
//    Global resource dependence:
//  Author:         Daniel.Luo
//    Note:
/*****************************************************************************/
PUBLIC int SCI_Trace_Autotest( // None
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )/*lint -esym(765, SCI_Trace_Autotest)*/
{
    int status = -1;
    //uint8 **dest_ptr;
    //uint16 *dest_len_ptr;

    //*dest_ptr = SCI_NULL;

    //DIAG_Trace_Autotest(dest_ptr, dest_len_ptr, src_ptr, src_len);
    status = DIAG_Trace_Autotest(src_ptr, src_len);

    return status;
}

/*****************************************************************************/
//  Description:    Handle layer/ps request
//    Global resource dependence:
//  Author:
//    Note:
/*****************************************************************************/
LOCAL void HandleLayer1PsReq( // None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    uint8        cmd_type;
    uint8        cmd_subtype;
    MSG_HEAD_T  *msg_head;

    msg_head    = (MSG_HEAD_T *) src_ptr;
    cmd_type    = msg_head->type;
    cmd_subtype = msg_head->subtype;

#ifndef WIN32
    if(DIAG_LAYER1_F == cmd_type)
    {

        Layer1_HandleDspDebug(dest_ptr, dest_len_ptr, src_ptr, src_len);
    }
    else if(DIAG_PS_F == cmd_type)
    {
        ENG_HandleDiagCmdRoutine(dest_ptr, dest_len_ptr, src_ptr, src_len);
    }
    else
    {
        *dest_len_ptr = 0;
    }
#endif

}


#ifdef DUAL_BATTERY_SUPPORT
LOCAL BOOLEAN HandleDualBatChannelSel(volatile uint32 adc_src)
{
    DBAT_PROD_FUN_T*  dbat_prod_fun_ex = DBAT_Prod_GetFun();
    DBAT_PROD_CFG_T*  dbat_cfg_tab_ex = DBAT_Prod_GetCfg();

    dbat_prod_fun_ex->CalibrationModeInit();

    if(adc_src == dbat_cfg_tab_ex->hw_param.adc_channel[DBAT_MAIN_BAT])
    {
        dbat_prod_fun_ex->SelectBat(DBAT_MAIN_BAT);
        SCI_Sleep(100);
        return SCI_TRUE;
    }
    else if(adc_src == dbat_cfg_tab_ex->hw_param.adc_channel[DBAT_AUX_BAT])
    {
        dbat_prod_fun_ex->SelectBat(DBAT_AUX_BAT);
        SCI_Sleep(100);
        return SCI_TRUE;
    }

    return SCI_FALSE;
}
#endif

/*****************************************************************************/
//  Description:    Handle Read ADC result command
//  Global resource dependence:
//  Author:         Lin.liu
//  Note:
/*****************************************************************************/
LOCAL void HandleReadADC (// None
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len          // Size of the source buffer in uint8.
)
{
    volatile      uint32       adc_src, adc_result, adc_tmp;
    uint32        tmp;
    MSG_HEAD_T   *msg_head = (MSG_HEAD_T *) src_ptr;
    MSG_ADC_T    *adc_ptr;
    uint16 i;
    BOOLEAN scale;
    // Get ADC source
    adc_src = msg_head->subtype;
    adc_tmp = adc_src;

    // Malloc a buffer for return
    tmp     = sizeof (MSG_ADC_T);
    adc_ptr = (MSG_ADC_T *) SCI_ALLOC_APP (tmp);
    SCI_ASSERT (SCI_NULL != adc_ptr);/*assert verified*/

    // Init the struct
    *dest_ptr                 = (uint8 *) adc_ptr;
    *dest_len_ptr             = tmp;
    adc_ptr->msg_head.len     = tmp;
    adc_ptr->msg_head.seq_num = msg_head->seq_num;
    adc_ptr->msg_head.type    = msg_head->type;

    if (adc_src <= 10)  // if (adc_src <= 8),jackeyling 20110707,6800H use 0~8channel,pc dispose 0~9
    {
        if(0 == adc_src)
        {
            adc_src = 1;
        }

        adc_result = 0;

 #ifdef DUAL_BATTERY_SUPPORT
        if( SCI_TRUE == HandleDualBatChannelSel(adc_src) )
        {
            for (i = 0 ; i < 16 ; i++)
            {
                adc_result += ADC_GetResultDirectly ( (adc_src ), ADC_SCALE_1V2);
            }

            #if 0
            //if vbatm cann't switchover vataux channel,just make CFT soft pass the calibration
            if(adc_result == 0)
            {
                adc_result = (0x01 << 4);
            }
            #endif
        }
        else
 #endif
        {
             if(adc_src ==ADC_CHANNEL_1 ||adc_src ==ADC_CHANNEL_6 ||adc_src ==ADC_CHANNEL_0){
               scale=ADC_SCALE_1V250;
            }else{
               scale=ADC_SCALE_3V233;
            }
//            if(adc_src -1==ADC_CHANNEL_6){
//              adc_result= _sprd_codec_ap_headmic_adcget();
//            }
//            else{
                for (i = 0 ; i < 16 ; i++)
                {
                   adc_result += ADC_GetResultDirectly ( (adc_src ), scale);
                }
                adc_result >>= 4;
//            }

        }
        //SCI_TRACE_LOW:"ReadADC.( %x = %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DIAG_3442_112_2_18_1_4_0_15,(uint8*)"dd", adc_tmp, adc_result);

        adc_ptr->result = (uint16) (adc_result & 0xFFFF);
        adc_ptr->msg_head.subtype = 0;
    }
    else
    {
        // invalid subtype, invalid adc source.
        adc_ptr->result = 0xFFFF;
        adc_ptr->msg_head.subtype = 1;
    }
}
/*lint -restore*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#include "ms_ref_base_trc.h"
#include "diag.h"
#include "cmddef.h"
#include "chg_drvapi.h"


#define _APAPI_DEBUG

#ifdef _APAPI_DEBUG
#include "sci_log.h"

#define APAPI_ASSERT(_x_)                SCI_ASSERT(_x_)
#define APAPI_VERIFY(_x_)                SCI_ASSERT(_x_)
#define APAPI_ERR_TRACE                  SCI_TRACE_LOW
#define APAPI_LOG_TRACE                  SCI_TRACE_LOW
#define APAPI_INF_TRACE                  SCI_TRACE_LOW 
#else
#define APAPI_ASSERT(_x_)                ((void)0)
#define APAPI_VERIFY(_x_)                ((void)(_x_))
#define APAPI_ERR_TRACE(_x_)            ((void)0)
#define APAPI_LOG_TRACE(_x_)            ((void)0)
#define APAPI_INF_TRACE(_x_)            ((void)0)
#endif /* _APAPI_DEBUG */

typedef enum
{
    DIAG_AP_CMD_ADC = 0x0001,
    DIAG_AP_CMD_FILE_OPER = 0x0003,
    DIAG_AP_CMD_SWITCH_CP = 0x000,
    DIAG_AP_CMD_BKLIGHT = 0x0005,
    DIAG_AP_CMD_PWRMODE = 0x0007,
    DIAG_AP_CMD_CHARGE_ONOFF = 0x0010,
    DIAG_AP_CMD_CHARGE_CURRENT = 0x0011,
    DIAG_AP_CMD_MMI_CHECK = 0x0013,
    MAX_DIAG_AP_CMD
}DIAG_AP_CMD_E;

typedef struct
{
    uint16 cmd;                     //DIAG_AP_CMD_E
    uint16 length;                 //CMD_REQ_DATA  的长度
}TOOLS_DIAG_AP_REQ_T;

typedef struct
{
    uint16 status;                //0 success, other failed
    uint16 length;                //ACK_RSP_DATA  的长度
}TOOLS_DIAG_AP_CNF_T;

typedef struct
{
    uint32 on_off;
}TOOLS_DIAG_AP_CHARGE_CMD_T;

typedef struct
{
    uint32 charging;
    uint32 battery;
}TOOL_DIAG_AP_CHARGE_CURRENT_CNF_T;

typedef struct
{
    uint16  uType;
    uint8  uBuf[256];
}TOOL_DIAG_AP_MMI_CIT_T;

typedef uint32 (*DIAG_AP_CALLBACK)(     
    uint8 **dest_ptr,       
    uint16 *dest_len_ptr,   
    const uint8 *src_ptr,   
    uint16 src_len          
);

LOCAL DIAG_AP_CALLBACK s_diag_ap_callback[MAX_DIAG_AP_CMD] = {NULL};

PUBLIC void DIAG_InitApCallback( void )
{
    int  type = 0;
    
    for( type = 0; type < MAX_DIAG_AP_CMD; type++ )
    {
        s_diag_ap_callback[ type ] = NULL;
    }
}

PUBLIC void DIAG_RegisterApCallback( uint32  type,  DIAG_AP_CALLBACK  routine )
{    
    if( type < MAX_DIAG_AP_CMD )
        s_diag_ap_callback[ type ] = routine;
}

PUBLIC void DIAG_UnRegisterApCallback( uint32  type )
{
    if( type < MAX_DIAG_AP_CMD )
        s_diag_ap_callback[ type ] = NULL;
}

LOCAL uint32 Handle_Ap_Charge_Onoff(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len         // Size of the source buffer in uint8.
 ) 
{
    uint16 ret;
    TOOLS_DIAG_AP_CHARGE_CMD_T *chg_cmd_st;

    SCI_TRACE_LOW("APSRV: Handle_Ap_Charge_Onoff\r\n");

    chg_cmd_st = (TOOLS_DIAG_AP_CHARGE_CMD_T *) (src_ptr + sizeof (MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_REQ_T));

    if(chg_cmd_st->on_off == 1){  //charge on 
        SCI_TRACE_LOW("APSRV: chg_cmd_st->on_off == 1\r\n");
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		while(1){
            if(CHGMNG_ThreadReady() != 0){
                break;
            }
            SCI_SLEEP(10); 
        }
#endif
        SCI_SLEEP(3000);
        CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_IN_MSG, 0);
        ret = 0;
    }
    else{ //charge off
        SCI_TRACE_LOW("APSRV: chg_cmd_st->on_off == 0\r\n");
        CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PESUDO_PLUG_OUT_MSG, 0);
        SCI_SLEEP(10); 
        ret = 0;
    }
    
    // Build response
    *dest_ptr = SCI_ALLOC (sizeof (MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T));
    if(NULL == *dest_ptr)
    {
        return 0;
    }

    *dest_len_ptr = sizeof (MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T);
    memset(*dest_ptr, 0, *dest_len_ptr);
    memcpy (*dest_ptr,  src_ptr,  sizeof (MSG_HEAD_T));
    ( (MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
    ( (TOOLS_DIAG_AP_CNF_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T)))->status = ret;
        
    return 1;
}

LOCAL uint32 Handle_Ap_Charge_Current(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len         // Size of the source buffer in uint8.
 ) 
{    
    uint16 ret = 1;
    uint32 battery_vol = 0;
    CHGMNG_STATE_INFO_T* chg_state_st = NULL;

    SCI_TRACE_LOW("APSRV: Handle_Ap_Charge_Current\r\n");
    
    // Build response
    *dest_ptr = SCI_ALLOC (sizeof (MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T) + sizeof(TOOL_DIAG_AP_CHARGE_CURRENT_CNF_T));
    if(NULL == *dest_ptr)
    {
        return 0;
    }

    //todo
    chg_state_st = CHGMNG_GetModuleState();
    if(chg_state_st != NULL){
        battery_vol = chg_state_st->charging_current;
        ret = 0;
    }    

    SCI_TRACE_LOW("APSRV: battery_vol:0x%x\r\n", battery_vol);

    *dest_len_ptr = sizeof (MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T) + sizeof(TOOL_DIAG_AP_CHARGE_CURRENT_CNF_T);
    memset(*dest_ptr, 0, *dest_len_ptr);
    memcpy (*dest_ptr,  src_ptr,  sizeof (MSG_HEAD_T));
    ( (MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
    ( (TOOLS_DIAG_AP_CNF_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T)))->status = ret;
    ( (TOOLS_DIAG_AP_CNF_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T)))->length = sizeof(TOOL_DIAG_AP_CHARGE_CURRENT_CNF_T);
     ( (TOOL_DIAG_AP_CHARGE_CURRENT_CNF_T *) ((*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->battery = battery_vol;
        
    return 1;
}


LOCAL uint32 Handle_Ap_MMI_CHECK(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len         // Size of the source buffer in uint8.
 )
{
    uint16 ret = 1;
    uint32 check_val = 0;

    SCI_TRACE_LOW("APSRV: Handle_Ap_MMI_Check\r\n");

    // Build response
    *dest_ptr = SCI_ALLOC (sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T) + sizeof(TOOL_DIAG_AP_MMI_CIT_T));
    if(NULL == *dest_ptr)
    {
        return 0;
    }

    *dest_len_ptr = sizeof (MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T) + sizeof(TOOL_DIAG_AP_MMI_CIT_T);
    memset(*dest_ptr, 0, *dest_len_ptr);
    memcpy (*dest_ptr,  src_ptr,  sizeof (MSG_HEAD_T));
    memcpy ((*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T),  src_ptr +  sizeof (MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T) ,  sizeof (TOOL_DIAG_AP_MMI_CIT_T));
    //copy the pack

    if(check_val = REF_GetMarkerValue() ){
        ret = 0;
    }

    ( (MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
    ( (TOOLS_DIAG_AP_CNF_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T)))->status = ret;
    ( (TOOLS_DIAG_AP_CNF_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T)))->length = sizeof(TOOL_DIAG_AP_MMI_CIT_T);

    //final test bit[255:252]
    if ((check_val & 0x8000000)  && ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uType == 0){ // check mmi flag bit
        ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uBuf[252] = 1;
        ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uBuf[253] = 1;
        ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uBuf[254] = 1;
        ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uBuf[255] = 1;
    }
    else{
        ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uBuf[252] = 0;
        ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uBuf[253] = 0;
        ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uBuf[254] = 0;
        ( (TOOL_DIAG_AP_MMI_CIT_T *)( (*dest_ptr)+sizeof(MSG_HEAD_T) + sizeof(TOOLS_DIAG_AP_CNF_T)))->uBuf[255] = 0;
    }
    return 1;
}

LOCAL uint32 Handle_Ap_Routine( 
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len          // Size of the source buffer in uint8.
    )
{
    
    TOOLS_DIAG_AP_REQ_T* ap_req_st = NULL; 

    SCI_TRACE_LOW("APSRV: Handle_Ap_Routine\r\n");
    
    ap_req_st = (TOOLS_DIAG_AP_REQ_T *)(src_ptr + sizeof(MSG_HEAD_T));

    if(!dest_ptr || !src_ptr)
    {
        return 0;
    }

    SCI_TRACE_LOW("APSRV: ap_req_st->cmd:0x%x\r\n", ap_req_st->cmd);
    
    if( ap_req_st->cmd < MAX_DIAG_AP_CMD )
    {
        if( NULL != s_diag_ap_callback[ap_req_st->cmd] )
        {
            if( s_diag_ap_callback[ap_req_st->cmd]( dest_ptr, dest_len_ptr, src_ptr, src_len ) )
            {
                return  1;
            }
        }
    }
    
    return 0;
}

PUBLIC void Register_ApTest_CmdRoutine (void)
{
    SCI_TRACE_LOW("APSRV: Register_ApTest_CmdRoutine\n\r");
    
    DIAG_InitApCallback();
    DIAG_RegisterApCallback( DIAG_AP_CMD_CHARGE_ONOFF, Handle_Ap_Charge_Onoff);
    DIAG_RegisterApCallback( DIAG_AP_CMD_CHARGE_CURRENT, Handle_Ap_Charge_Current);
    DIAG_RegisterApCallback( DIAG_AP_CMD_MMI_CHECK, Handle_Ap_MMI_CHECK);//DIAG_AP_CMD_MMI_CHECK
    DIAG_RegisterCmdRoutine (DIAG_AP_F,  Handle_Ap_Routine); 
}

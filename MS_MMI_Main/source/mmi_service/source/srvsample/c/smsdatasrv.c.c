
#include "mmisms_app.h"





//Declaration of local functions
void SrvSample_init(void);



//global variable declare
MMI_APPLICATION_T			g_srvsample_app;     

DATASRV_SMS_INSTANCE_T      sms_datasrv = {0};

//LOCAL 
PUBLIC MMI_RESULT_E SrvSample_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);

//Init
PUBLIC void SAMPLESRV_Init(void)
{
    g_srvsample_app.ProcessMsg = SrvSample_HandlePsMsg;

    //Call init
    SrvSample_init();
}

PUBLIC MMI_RESULT_E SrvSample_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;


    if(PNULL== app_ptr)
    {
        //Raise Error
        return MMI_RESULT_FALSE;
    }

    switch ( msg_id )
    {
        
    case APP_MN_READ_SMS_CNF:
        
        break;
        
        
    case APP_MN_EDIT_SMS_CNF :
        
        break;
        
    case APP_MN_WRITE_SMS_CNF:
        
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return ( result );
}

void SrvSample_init(void)
{
    SCI_MEMSET(&sms_datasrv,0,sizeof(DATASRV_SMS_INSTANCE_T));
    sms_datasrv.msg_list = MMI_DLIST_Create(1000,sizeof(MMISMS_STATE_T));

    sms_datasrv.srv_status = SRVSTATUS_INITIALIZING;

    //Get msg count of NV

    //Start read from nv and build msg_list

    MNSMS_GetSmsNumEx(dual_sys, storage, &max_sms_num, &used_sms_num);
    MNSMS_ReadSmsEx(dual_sys, storage, (MN_SMS_RECORD_ID_T )i);
}

void SrvSample_SetFlag(void)
{}


void SrvSample_ReadSMS(void)
{

}



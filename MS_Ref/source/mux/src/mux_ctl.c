/******************************************************************************
 ** File Name:      mux_ctl.c                                      *
 ** Author:         yayan.xu                                             *
 ** DATE:           07/10/2008                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:      The file defines MUX CTL behavior*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/10/2008     yayan.xu     Create.                                   *
 ******************************************************************************/
#include "ms_ref_mux_trc.h"
#include "mux_all.h"
#include "mux_api.h"
#include "mux_driver.h"
#ifdef _MUX_MODEM_
#include "atc_mux.h"
#include "env_atc_signal.h"
#endif/*_MUX_MODEM_*/



/*------------------------------------------------------------------------------
Defination :MUX var
------------------------------------------------------------------------------*/

MUX_MUTEX_PTR   mux_send_mutex    = NULL;    // MS00138417 

MUX_INFO_T       g_mux_sys_info[MAX_MUX_NUM];/*mux sys info*/

MUX_DLC_T                   g_mux_dlc_info[MUX_MAX_LINK_NUM];/*DLC info*/

/*Use in MUX RECV TASK, to save data from Driver*/
/*MUX_BUF_T mux_rx_buf;*//*Instead by mux_rev_buf in Mux_Data_Parse_Basic*/


uint8                       g_mux_alloc_dlc         = 1;//Used in allocate DLCI,range 1-63, 0 is reserved for CTRL.

/*common public ATC process function*/
MUX_CALLBACK                g_mux_common_recv_func  = NULL;


LOCAL MUX_SYS_PARA_T        g_default_sys_para      =
{
    MUX_OPTION_BASIC, MUX_FRAME_UIH, MUX_BAUDRATE_115200, 31, 10, 3, 30, 10, 2,0
};

//LOCAL unsigned char         mux_tx_buf[MUX_MAX_BUF_SIZE];
LOCAL uint32   mux_tx_buf[MUX_MAX_BUF_SIZE/4]; //MS00144475 4byte alliance
LOCAL uint8    *mux_tx_buf_ptr =(uint8 *) mux_tx_buf; //MS00144475 4byte alliance
/*------------------------------------------------------------------------------
Defination :function
------------------------------------------------------------------------------*/

LOCAL void          MUX_Wait_WakeUp(uint32 event_flag, MUX_INFO_T *mux_ptr);
LOCAL BOOLEAN       MUX_Res_Wait(uint32 event_flag, uint32 wait_time, MUX_INFO_T *mux_ptr);


LOCAL MUX_RETURN_E  MUX_Send_DM(MUX_INFO_T *config_data, uint8 dlci);
LOCAL MUX_RETURN_E  MUX_Send_UA(MUX_INFO_T *config_data, uint8 dlci);
LOCAL MUX_RETURN_E  MUX_Send_Disc(MUX_INFO_T *config_data, uint8 dlci);
LOCAL MUX_RETURN_E  MUX_Send_SABM(MUX_INFO_T *config_data, uint8 dlci);
LOCAL uint32        MUX_Send_MCC_NSC(MUX_MCC_TYPE cmd, uint8 cr, MUX_INFO_T *config_data );
LOCAL uint32        MUX_Send_MCC_MSC(MUX_V24_SIGNAL_T v24_signal, uint8 dlci, uint8 cr, MUX_INFO_T *config_data);
LOCAL uint32        MUX_Send_MCC_FCoff(uint8 cr, MUX_INFO_T *config_data);
LOCAL uint32        MUX_Send_MCC_FCon(uint8 cr, MUX_INFO_T *config_data);
LOCAL uint32        MUX_Send_MCC_TEST(uint8 *data, uint32 length, uint8 cr, MUX_INFO_T *config_data);
LOCAL uint32        MUX_Send_MCC_CLD(uint8 cr, MUX_INFO_T *config_data);
LOCAL uint32        MUX_Send_MCC_PSC(uint8 cr, MUX_INFO_T *config_data);
LOCAL uint32        MUX_Send_MCC_PN(MUX_PN_DATA_T *pn_data, uint8 dlci, uint8 cr,MUX_INFO_T *config_data);
LOCAL void          MUX_Check_PN(MUX_PN_DATA_T pn_data, uint8 dlci, MUX_INFO_T *config_data);
LOCAL void          MUX_MCCFrame_Parse(uint8 *data, uint32 len, MUX_INFO_T *config_data);

LOCAL MUX_RETURN_E  MUX_Open_Dlci(MUX_PN_DATA_T *sys_param, uint8 dlci, uint8 link_id, MUX_INFO_T *mux_ptr);
LOCAL MUX_RETURN_E  MUX_Close_Dlci(uint8 dlci, MUX_INFO_T *mux_ptr);

LOCAL uint32 MUX_Send_MCC_SNC(MUX_SNC_STRUCT_T *snc_pkt, uint8 cr, MUX_INFO_T *config_data);
LOCAL uint8  MUX_SNC_Get_Supported_Services();
LOCAL uint8  MUX_SNC_Get_Supported_Codecs();


/*****************************************************************************/
// Description :    external module call this interface to startup MUX service
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :   In UE side, Call API after ATC return OK for AT+CMUX.Because OK MUST NOT encode MUX Frame
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_Startup(MUX_SIDE_E side, MUX_SYS_PARA_T para)
{
    uint8   loop;
    char    cmd_ptr[MUX_AT_CMUX_LEN_MAX];
    uint8   cmd_len;
    BOOLEAN is_overtime;
    uint32  cmd_timeout = 3000;/*TEMP, 3seconds*/
    uint8   retry_times = 3;/*TEMP, retry 3 times*/
    uint8  mux_index =  MUX_1;
    MUX_INFO_T *mux_ptr = &g_mux_sys_info[mux_index];//Temp set, avoid interface modify

    if(mux_ptr->is_start != MUX_STATUS_OFF)
    {
        /*MUX has startup already*/
        //MUX_TRACE_WARNING:"MUX_Startup fail due to MUX is in ON"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_94_112_2_18_1_49_18_24,(uint8*)"");
        return MUX_RET_ALREADY_STARTUP;
    }

    /*check validity*/
    if (para.subset != MUX_FRAME_UIH)/*Support advance mode*/
    {
        /*In Current Version, only support Basic mode and UIH frame*/
        //MUX_TRACE_WARNING:"MUX_Startup fail due to parameter error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_102_112_2_18_1_49_18_25,(uint8*)"");
        return MUX_RET_WORK_MODE_ERR;
    }

    //MUX_TRACE_WARNING:"MUX_Startup: From %d\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_106_112_2_18_1_49_18_26,(uint8*)"d",side);

    mux_ptr->side = side;
    mux_ptr->cmdres_wait_flag =( 1& 0xffffffff) << 31;/*set bit31 to 1*/

    MUX_MEMCPY(&mux_ptr->sys_parameter, &para, sizeof(MUX_SYS_PARA_T));

    /*Init DLC info and DTI*/
    for (loop = 0; loop < MUX_MAX_LINK_NUM; loop++)
    {
        MUX_FlushDlci(loop);
    }

    /*Create event */
    if (PNULL == mux_ptr->g_MuxEvent_ptr)
    {
        mux_ptr->g_MuxEvent_ptr = MUX_CREATE_EVENT("Mux Event");
    }

    MUX_ASSERT(PNULL != mux_ptr->g_MuxEvent_ptr); /*assert verified*/    

#if 0 //MS00148955, create mutex when MUX init 
    //MS00138417
    if (NULL == mux_send_mutex)
    {
        mux_send_mutex = MUX_CREATE_MUTEX("MUX_SEND_MUTEX", MUX_INHERIT);
    }
#endif


    //Init mux_rev_buf info
    MUX_Rev_Buf_Init(mux_ptr);
    MUX_TRACE_LOW(("MUX_Startup: Clear tx buffer in %d!\r",mux_ptr->mux_index));    
	
    MUX_Ring_Clear(&mux_port[mux_index].tx_buf);

    switch (mux_ptr->side)
    {
        case MUX_SIDE_TE:
            while (retry_times)
            {
                mux_ptr->is_start = MUX_STATUS_ON_REQ;             
                /*First, Send AT+CMUX command to peer*/
                MUX_Assemble_CMUX_CMD(&(mux_ptr->sys_parameter), cmd_ptr, &cmd_len, retry_times);

                MUX_Data_Tx((uint8 *) cmd_ptr, cmd_len, mux_index);

                /*Adopt Event way to wait RESPONSE from peer.*/
                mux_ptr->cmdres_code = MUX_ATPlusCMUX_RET_INVALID;

                is_overtime = MUX_Res_Wait(mux_ptr->cmdres_wait_flag, cmd_timeout, mux_ptr);
                if (MUX_ATPlusCMUX_RET_SUCCESS == mux_ptr->cmdres_code)
                {
                    MUX_TRACE_LOW(("MUX_Startup: AT+CMUX command OK!\r"));
                    break;
                }
                /*if return error and timout, retry again*/
                /*else if((MUX_ATPlusCMUX_RET_FAIL == g_mux_sys_info.cmdres_code)
                ||is_overtime)*/                
                retry_times--;
            }
            /*if execute fails, directly return*/
            if (MUX_ATPlusCMUX_RET_SUCCESS != mux_ptr->cmdres_code)
            {
                //MUX_TRACE_WARNING:"MUX_Startup: AT+CMUX command Failure!\r"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_170_112_2_18_1_49_18_27,(uint8*)"");
                mux_ptr->is_start = MUX_STATUS_OFF;

                return MUX_RET_FAILURE;
            }

            /*assemble SABM DLC0 to peer end.*/
            mux_ptr->is_start = MUX_STATUS_ON;             
            g_mux_dlc_info[mux_ptr->ctl_link].dlci = MUX_CTRL_CHAN;
            if (MUX_RET_SUCCESS != MUX_Open_Dlci(PNULL, MUX_CTRL_CHAN, mux_ptr->ctl_link, mux_ptr))
            {
                //MUX_TRACE_WARNING:"MUX_Startup: fail due to OPEN Control DLCI failure!\r"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_181_112_2_18_1_49_18_28,(uint8*)"");
                mux_ptr->is_start = MUX_STATUS_OFF;
                return MUX_RET_FAILURE;
            }        
            break;
        case MUX_SIDE_UE:
            mux_ptr->is_start = MUX_STATUS_ON_REQ;
	     //MS00145432, first ATC notify MUX on, Then return OK
	     mux_ptr->TE_wait_OK = TRUE;//flag is true; mux send raw data, don't fill MUX frame
            /*According to protocol, consider start T3 timer, tempority NOT consider*/
            break;
        default:
            MUX_ASSERT(FALSE);/*assert verified*/
            break;
    }


    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    external module call this interface to read current MUX sys parameter
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      10th July.08
// Note :           
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_ReadSysPara(MUX_SYS_PARA_T *param)
{

    uint8  mux_index =  MUX_1;
    MUX_INFO_T *mux_ptr = &g_mux_sys_info[mux_index];//Temp set, avoid interface modify

    if (MUX_STATUS_OFF == mux_ptr->is_start)
    {
        /*MUX is NOT startup*/
        //MUX_TRACE_WARNING:"MUX_ReadSysPara fail due to NOT Startup"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_215_112_2_18_1_49_19_29,(uint8*)"");
        return MUX_RET_NOT_STARTUP;
    }

    MUX_ASSERT(NULL != param);/*assert verified*/

    MUX_MEMCPY(param, &mux_ptr->sys_parameter, sizeof(MUX_SYS_PARA_T));

    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    this function is used to wait a response in the time gived
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.7.25
// Note :     Used in send SABM or DISC case
/*****************************************************************************/
LOCAL BOOLEAN MUX_Res_Wait(uint32 event_flag, uint32 wait_time, MUX_INFO_T *mux_ptr)
{
    uint32  status, i_event_flag ;


    if(NULL == mux_ptr->g_MuxEvent_ptr)
    {
         //MUX_TRACE_WARNING:"MUX_Res_Wait: g_MuxEvent_ptr is null ptr!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_238_112_2_18_1_49_19_30,(uint8*)"");
         return FALSE;
    }

    MUX_TRACE_LOW(("MUX_Res_Wait:Mux wait for response!"));  

    /*MUX_GET_EVENT( g_MuxEvent_ptr, 
                        event_flag, 
                        MUX_AND_CLEAR, 
                        &i_event_flag, 
                        MUX_NO_WAIT );  
                     
    */

    status = MUX_GET_EVENT(mux_ptr->g_MuxEvent_ptr, event_flag, MUX_OR_CLEAR, &i_event_flag, wait_time);  

    MUX_TRACE_LOW(("MUX_Res_Wait:Mux Get response OK,status = %d!", status)); 

    if (status != 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/*****************************************************************************/
// Description :    this function is used to wait a response in the time gived
// Global resource dependence : 
// Author :       from amoi
// DATE :         2007.7.25
// Note :    Used in send SABM or DISC case
/*****************************************************************************/

LOCAL void MUX_Wait_WakeUp(uint32 event_flag, MUX_INFO_T *mux_ptr)
{
    if(NULL == mux_ptr->g_MuxEvent_ptr)
    {
         //MUX_TRACE_WARNING:"MUX_Wait_WakeUp: g_MuxEvent_ptr is null ptr!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_277_112_2_18_1_49_19_31,(uint8*)"");
         return;
    }

    MUX_SET_EVENT(mux_ptr->g_MuxEvent_ptr, event_flag, MUX_OR);

    return;
}

/*****************************************************************************/
// Description :    Receive SABM Frame handler
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      18th July.08
// Note :           
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_SABM_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr)
{
    uint8   link_id;
    uint8   dlci;


    MUX_TRACE_LOW(("MUX_SABM-packet received\n"));


    dlci = mux_pkt_head_ptr->addr.dlc_chn;  

    /*Three condition allow establish DLC.
    1, DLCI is in valid range. 
    2, DLCI is NOT Exist, NOT Establish.
    3, Have Empty link.*/
    if ((MUX_Valid_Dlci(dlci)) 
        && (MUX_DLC_LINK_INVALID == MUX_Get_Linkid(dlci, mux_ptr->mux_index)) 
        && (MUX_RET_SUCCESS == MUX_Get_NewLinkid(&link_id, mux_ptr->mux_index)))
    {
        //MUX_TRACE_WARNING:"MUX_SABM_Handler:Incomming connect on DLC %d; link_id %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_310_112_2_18_1_49_19_32,(uint8*)"dd", dlci, link_id);
        //MUX_Wait_WakeUp(g_mux_dlc_info[link_id].open_wait_event);
        MUX_FlushDlci(link_id);
        g_mux_dlc_info[link_id].dlci = dlci;
        g_mux_dlc_info[link_id].from = mux_ptr->mux_index;
        
#ifdef _MUX_MODEM_
        {
            /*Send Link establish indication to ATC*/
            ATC_MUX_LINK_SETUP_REQ_T   *sig_ptr = PNULL;    

            MUX_CREATE_SIGNAL(*(xSignalHeader*) &sig_ptr, ATC_MUX_LINK_SETUP_REQ, \
                                            sizeof(ATC_MUX_LINK_SETUP_REQ_T), MUX_IdentifyThread());

            sig_ptr->link_id = link_id;

            MUX_SEND_SIGNAL((xSignalHeader) sig_ptr, P_ATC);
        }
#endif/*_MUX_MODEM_*/

     
        /*Next notify peer to allow establish link*/
        MUX_Send_UA(mux_ptr, dlci);
        g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_CONNECT;

#ifdef _STATIC_LINK_SET_   
        if (g_mux_data_common_recv_func[link_id].entity_recv_func)
        {
            /*PPP has caledl already MUX_DTI_Create befor MUX startup*/
            /*Mux internal program call again for Create DTI*/
            MUX_DTI_Create(link_id, g_mux_data_common_recv_func[link_id]);
        }

#endif/*_STATIC_LINK_SET_*/
    }
    else/*Reject SABM*/
    {
	 link_id = MUX_Get_Linkid(dlci, mux_ptr->mux_index);
        if(MUX_DLC_LINK_INVALID != link_id)
        {
              /*if DLC have established already, Receive SABM again, Sending MUX_UA*/
	        //MUX_TRACE_WARNING:"MUX_SABM_Handler, DLC%d is exist, Sending MUX_UA \n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_351_112_2_18_1_49_19_33,(uint8*)"d", dlci);
	        MUX_Send_UA(mux_ptr, dlci);
        }
	 else
	 {
	        //MUX_TRACE_WARNING:"MUX_SABM_Handler: invalid dlci %d, sending MUX_DM\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_356_112_2_18_1_49_19_34,(uint8*)"d", dlci);
	        MUX_Send_DM(mux_ptr, dlci);
	 }
    }

    return MUX_RET_SUCCESS;
}
/*****************************************************************************/
// Description :    Receive UA Frame handler
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      18th July.08
// Note :           
/*****************************************************************************/
PUBLIC  MUX_RETURN_E MUX_UA_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr)
{
    uint8   link_id;
    uint8   dlci    = mux_pkt_head_ptr->addr.dlc_chn; 

    /*Nornal case: SABM or DISC Frame had been sent, wait response.*/
    /*First: check DLCI */
    /*Second: check link id*/
    if ((MUX_Valid_Dlci(dlci)) && (MUX_DLC_LINK_INVALID != (link_id = MUX_Get_Linkid(dlci, mux_ptr->mux_index))))
    {
        if (MUX_DLC_STATE_CONNECTING == g_mux_dlc_info[link_id].used_state)
        {
            /*After SABM Frame*/
            //MUX_TRACE_WARNING:"MUX: MUX_UA_Handler: Incomming CONNECTED  MUX_UA on dlci %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_381_112_2_18_1_49_19_35,(uint8*)"d", dlci);
            g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_CONNECT;
            MUX_Wait_WakeUp(g_mux_dlc_info[link_id].open_wait_event, mux_ptr);
        }
        else if (MUX_DLC_STATE_DISCONNECTING == g_mux_dlc_info[link_id].used_state)
        {
            /*After DISC Frame*/
            //MUX_TRACE_WARNING:"MUX:MUX_UA_Handler: Incomming DISCONNECTED  MUX_UA on dlci %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_388_112_2_18_1_49_19_36,(uint8*)"d", dlci);
            g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_DISCONNECT;
            MUX_Wait_WakeUp(g_mux_dlc_info[link_id].close_wait_event, mux_ptr);
#if 0  /*Move to Mux_Close_channel*/
            if(dlci== MUX_CTRL_CHAN)
            {
                /*DISC DLC0 means CLOSE MUX*/
                MUX_Upon_Close();
            }
            else
            {
                Mux_FlushDlci(link_id);
            }
#endif
        }
        else
        {
            /*discard, not expect response*/
            //MUX_TRACE_WARNING:"MUX_UA_Handler, unexpected response, discard,in link id %d,dlci %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_406_112_2_18_1_49_19_37,(uint8*)"dd", link_id,dlci);
        }
    }
    else
    {
        //MUX_TRACE_WARNING:"MUX: MUX_UA_Handler: invalid dlci %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_411_112_2_18_1_49_19_38,(uint8*)"d", dlci);
        return MUX_RET_LINK_NUM_ERR;
    }

    return MUX_RET_SUCCESS;
}
/*****************************************************************************/
// Description :    Receive DM Frame handler
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      18th July.08
// Note :           
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_DM_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr)
{
    uint8   link_id;
    uint8   dlci    = mux_pkt_head_ptr->addr.dlc_chn; 

    /*Nornal case: SABM or DISC Frame had been sent, wait response.*/
    /*First: check DLCI */
    /*Second: check link id*/
    if ((MUX_Valid_Dlci(dlci)) && (MUX_DLC_LINK_INVALID != (link_id = MUX_Get_Linkid(dlci, mux_ptr->mux_index))))
    {
        //MUX_TRACE_WARNING:"MUX: MUX_DM_Handler: Incomming MUX_DM on dlci %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_432_112_2_18_1_49_19_39,(uint8*)"d", dlci);

        if (MUX_DLC_STATE_CONNECTING == g_mux_dlc_info[link_id].used_state)
        {
            //MUX_TRACE_WARNING:"MUX: MUX_DM_Handler: Incomming CONNECTED  MUX_DM on dlci %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_436_112_2_18_1_49_19_40,(uint8*)"d", dlci);

            g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_REJECTED;

            /*Set open_wait_event, Send SABM function shall be running*/
            MUX_Wait_WakeUp(g_mux_dlc_info[link_id].open_wait_event, mux_ptr);
        }
        else if (MUX_DLC_STATE_DISCONNECTING == g_mux_dlc_info[link_id].used_state)
        {
            //MUX_TRACE_WARNING:"MUX:MUX_DM_Handler: Incomming DISCONNECTED  MUX_DM on dlci %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_445_112_2_18_1_49_19_41,(uint8*)"d", dlci);
            g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_REJECTED;

            MUX_Wait_WakeUp(g_mux_dlc_info[link_id].close_wait_event, mux_ptr);
        }
        else
        {
            /*discard, not expect response*/
            //MUX_TRACE_WARNING:"MUX_DM_Handler, unexpected response, discard in link id %d dlci %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_453_112_2_18_1_49_19_42,(uint8*)"dd", link_id, dlci);
        }
    }
    else
    {
        //MUX_TRACE_WARNING:"MUX_DM_Handler, invalid dlci %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_458_112_2_18_1_49_19_43,(uint8*)"d", dlci);
    }

    return MUX_RET_SUCCESS;
}
/*****************************************************************************/
// Description :    Receive DISC Frame handler
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      18th July.08
// Note :           
/*****************************************************************************/
PUBLIC  MUX_RETURN_E MUX_DISC_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr)
{
    uint8   link_id;
    uint8   dlci    = mux_pkt_head_ptr->addr.dlc_chn; 

    /*First: check DLCI */
    /*Second: check link id*/
    if ((MUX_Valid_Dlci(dlci)) && (MUX_DLC_LINK_INVALID != (link_id = MUX_Get_Linkid(dlci, mux_ptr->mux_index))))
    {
        //MUX_TRACE_WARNING:"MUX_DISC_Handler: Incomming MUX_DISC on channel %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_477_112_2_18_1_49_19_44,(uint8*)"d", dlci);
        if (MUX_DLC_STATE_DISCONNECT == g_mux_dlc_info[link_id].used_state)
        {
            /*DLC is  already in Disconnect, duplicate release, return DM.*/
            MUX_Send_DM(mux_ptr, dlci);
        }
        else
        {
#ifdef _MUX_MODEM_
            /*Notify ATC link release*/
            ATC_MUX_LINK_RELEASE_REQ_T *sig_ptr = PNULL;    

            MUX_CREATE_SIGNAL(*(xSignalHeader*) &sig_ptr, ATC_MUX_LINK_RELEASE_REQ, \
                                                    sizeof(ATC_MUX_LINK_RELEASE_REQ_T), MUX_IdentifyThread());

            sig_ptr->link_id = link_id;
            MUX_SEND_SIGNAL((xSignalHeader) sig_ptr, P_ATC);
#endif/*_MUX_MODEM_*/

            /*Receive DISC req*/
            MUX_Send_UA(mux_ptr, dlci);
        } 

        if (dlci == MUX_CTRL_CHAN)
        {
            /*DLC0 release means MUX close down*/
            MUX_Upon_Close(mux_ptr);
        }
        else
        {
            /*Clear DLC info*/
            MUX_FlushDlci(link_id);
        }
    }
    else
    {
        //MUX_TRACE_WARNING:"MUX_DISC_Handler: invalid dlci %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_513_112_2_18_1_49_19_45,(uint8*)"d", dlci);
        MUX_Send_DM(mux_ptr, dlci);
    }
    return MUX_RET_SUCCESS;
}


/*****************************************************************************/
// Description :    Receive UIH Frame handler
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      18th July.08
// Note :           
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_UIH_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr)
{
    uint32  inf_len;
    uint8   link_id;
    uint8   dlci;

    dlci = mux_pkt_head_ptr->addr.dlc_chn;

    /*DLCI is invalid or NOT find link id*/
    if ((!(MUX_Valid_Dlci(dlci))) || (MUX_DLC_LINK_INVALID == (link_id = MUX_Get_Linkid(dlci, mux_ptr->mux_index))))
    {
        //MUX_TRACE_WARNING:"MUX_UIH_Handler: invalid dlci %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_536_112_2_18_1_49_19_46,(uint8*)"d", dlci);
        MUX_Send_DM(mux_ptr, dlci);
        return MUX_RET_LINK_NUM_ERR;
    }


    /*ONLY connect and flow_stop status allow accept UIH/UI/I frame*/
    /*In conncet state, UIH maybe is user data and MSC message*/
    /*In flow stop state, there have no user data but have MSC message*/
    if ((g_mux_dlc_info[link_id].used_state != MUX_DLC_STATE_CONNECT) && (g_mux_dlc_info[link_id].used_state != MUX_DLC_STATE_FLOW_STOPPED))
    {
        //MUX_TRACE_WARNING:"MUX_UIH_Handler Error : DLCI %d (link %d)not connected, discard it!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_547_112_2_18_1_49_19_47,(uint8*)"dd", dlci, link_id);
        MUX_Send_DM(mux_ptr, dlci);
        return MUX_RET_DLC_STATUS_ERR;
    }

    if ((mux_pkt_head_ptr->length.ea) == 0)
    {
        inf_len = GET_LONG_LENGTH(mux_pkt_head_ptr->length);
        //inf_data_start = inf_data_buff;
        MUX_TRACE_LOW(("MUX long UIH packet received,  length = %d, DLC =%d, Link_id = %d\n", inf_len, dlci, link_id));
    }
    else
    {
        inf_len = mux_pkt_head_ptr->length.l_len;
        //inf_data_start = inf_data_buff;
        MUX_TRACE_LOW(("MUX Short MUX_UIH pkt received, length = %d, DLC=%d, Link_id = %d\n", inf_len, dlci,link_id));
    }

    if (dlci == MUX_CTRL_CHAN)
    {
        MUX_MCCFrame_Parse(inf_data_buff, inf_len, mux_ptr);
    }
    else
    {
          /*cr131891 : P-bit of UIH in DLC 0  maybe set 1, it means response for UIH*/
	    if (GET_PF(mux_pkt_head_ptr->control))
	    {
	        /*To UIH, both stations set the P-bit to 0*/
	        /*if P/F=1, error Frame */
	        //MUX_TRACE_WARNING:"MUX_UIH_Handler P/F set error, discard it in link %d,dlci %d!\n"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_576_112_2_18_1_49_19_48,(uint8*)"dd", link_id, dlci);
	        return MUX_RET_FAILURE;
	    }

        MUX_Forward_Up(link_id, inf_len, inf_data_buff);
    }

    return MUX_RET_SUCCESS;
}


/*****************************************************************************/
// Description :    Send UA Frame handler
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      18th July.08
// Note :           Refer to Amoi
/*****************************************************************************/
LOCAL MUX_RETURN_E MUX_Send_UA(MUX_INFO_T *config_data, uint8 dlci)
{
    uint32  res;
    uint8  *send_data_ptr = NULL;
    uint8   send_len = 0;
    uint8   buf[MUX_SHORTFRAME_OTHER_SIZE];
    
    //MUX_TRACE_WARNING:"Mux_Send_UA: Creating UA packet to DLCI %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_599_112_2_18_1_49_19_49,(uint8*)"d", dlci);

    switch (config_data->sys_parameter.mux_mode)
    {
        case MUX_OPTION_BASIC:
            {
                MUX_FRAME_HEAD *ua_head_ptr;
                

                MUX_MEMSET(buf, 0x00, MUX_SHORTFRAME_OTHER_SIZE);
                ua_head_ptr = (MUX_FRAME_HEAD *) (buf + 1);
                ua_head_ptr->addr.ea = 1;
                ua_head_ptr->addr.cr = (~(SET_CR(config_data->side)) & 0x1);
                ua_head_ptr->addr.dlc_chn = dlci;
                ua_head_ptr->control = SET_PF(MUX_UA);
                ua_head_ptr->length.ea = 1;
                ua_head_ptr->length.l_len = 0;
                buf[0] = MUX_BASIC_FLAG;
                buf[MUX_SHORTFRAME_OTHER_SIZE - 2] = MUX_Crc_Calc((uint8 *) ua_head_ptr, SHORT_CRC_CHECK);
                buf[MUX_SHORTFRAME_OTHER_SIZE - 1] = MUX_BASIC_FLAG;
                send_data_ptr = buf;
                send_len = MUX_SHORTFRAME_OTHER_SIZE;

                //save mux frame to log for wireshark analyzation
                SCI_TRACE_DATA(7, buf, send_len);                
            }
            break;
        case MUX_OPTION_ADVANCED:
            {
                MUX_ADVANCE_FRAME_HEAD_T   *ua_head_ptr;                

                MUX_MEMSET(buf, 0x00, MUX_SHORTFRAME_OTHER_SIZE);
                send_len = 0;
                buf[0] = MUX_ADVANCE_FLAG;
                send_len += 1;

                ua_head_ptr = (MUX_ADVANCE_FRAME_HEAD_T *) (buf + 1);
                ua_head_ptr->addr.ea = 1;
                ua_head_ptr->addr.cr = (~(SET_CR(config_data->side)) & 0x1);
                ua_head_ptr->addr.dlc_chn = dlci;
                ua_head_ptr->control = SET_PF(MUX_UA);
                send_len += 2;//sizeof(MUX_ADVANCE_FRAME_HEAD_T);

                buf[send_len++] = MUX_Crc_Calc((uint8 *) ua_head_ptr, 2);
                buf[send_len++] = MUX_ADVANCE_FLAG;
                send_data_ptr = buf;
            }
            break;
        case MUX_OPTION_ADVANCED_ERM:
        default:
            //MUX_TRACE_WARNING:"Mux_Send_UA: Fail in Creating UA packet to DLCI %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_646_112_2_18_1_49_20_50,(uint8*)"d", dlci);
            break;
    }

    res = MUX_Data_Tx(send_data_ptr, send_len, config_data->mux_index);

    return res;
}

/*****************************************************************************/
// Description :    Send DM Frame handler
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      18th July.08
// Note :           Refer to Amoi
/*****************************************************************************/
LOCAL MUX_RETURN_E MUX_Send_DM(MUX_INFO_T *config_data, uint8 dlci)
{
    MUX_RETURN_E    res;
    uint8          *send_data_ptr = NULL;
    uint8           send_len = 0;
    uint8           buf[MUX_SHORTFRAME_OTHER_SIZE];
    //MUX_TRACE_WARNING:"MUX_Send_DM: Creating DM packet to DLCI %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_666_112_2_18_1_49_20_51,(uint8*)"d", dlci);


    switch (config_data->sys_parameter.mux_mode)
    {
        case MUX_OPTION_BASIC:
            {
                MUX_FRAME_HEAD *dm_head_ptr;

                MUX_MEMSET(buf, 0x00, MUX_SHORTFRAME_OTHER_SIZE);
                dm_head_ptr = (MUX_FRAME_HEAD *) (buf + 1);
                dm_head_ptr->addr.ea = 1;
                dm_head_ptr->addr.cr = (~(SET_CR(config_data->side)) & 0x1);
                dm_head_ptr->addr.dlc_chn = dlci ;
                dm_head_ptr->control = SET_PF(MUX_DM);
                dm_head_ptr->length.ea = 1;
                dm_head_ptr->length.l_len = 0;
                buf[0] = MUX_BASIC_FLAG;
                buf[MUX_SHORTFRAME_OTHER_SIZE - 2] = MUX_Crc_Calc((uint8 *) dm_head_ptr, SHORT_CRC_CHECK);
                buf[MUX_SHORTFRAME_OTHER_SIZE - 1] = MUX_BASIC_FLAG;
                send_data_ptr = buf;
                send_len = MUX_SHORTFRAME_OTHER_SIZE;

                //save mux frame to log for wireshark analyzation
                SCI_TRACE_DATA(7, buf, send_len);                
            }
            break;
        case MUX_OPTION_ADVANCED:
            {
                MUX_ADVANCE_FRAME_HEAD_T   *dm_head_ptr;
                MUX_MEMSET(buf, 0x00, MUX_SHORTFRAME_OTHER_SIZE);
                send_len = 0;
                buf[0] = MUX_ADVANCE_FLAG;
                send_len += 1;

                dm_head_ptr = (MUX_ADVANCE_FRAME_HEAD_T *) (buf + 1);
                dm_head_ptr->addr.ea = 1;
                dm_head_ptr->addr.cr = (~(SET_CR(config_data->side)) & 0x1);
                dm_head_ptr->addr.dlc_chn = dlci;
                dm_head_ptr->control = SET_PF(MUX_DM);
                send_len += 2; //sizeof(MUX_ADVANCE_FRAME_HEAD_T);

                buf[send_len++] = MUX_Crc_Calc((uint8 *) dm_head_ptr, 2);
                buf[send_len++] = MUX_ADVANCE_FLAG;
                send_data_ptr = buf;
            }

            break;
        case MUX_OPTION_ADVANCED_ERM:
        default:
            //MUX_TRACE_WARNING:"MUX_Send_DM: Fail in Creating DM packet to DLCI %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_713_112_2_18_1_49_20_52,(uint8*)"d", dlci);
            break;
    }

    res = MUX_Data_Tx(send_data_ptr, send_len, config_data->mux_index);

    return res;
}


/*****************************************************************************/
// Description :    Send UIH Frame handler
// Global resource dependence : none
// Author :    yayan.xu  
// DATE :      18th July.08
// Note :           Refer to Amoi
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_Send_UIH(MUX_INFO_T *config_data, uint8 *data_ptr, uint32 data_len, uint8 dlci)
{
    uint32  i;
    uint8   buff[10];
    uint8   head_size;
    uint8   fcs;
    uint32  total_size  = 0;

    //MUX_TRACE_LOW(("Mux_Send_UIH: Creating UIH packet with %d bytes data to DLCI %d\n", data_len, dlci));

    //Judge data length, avoid memory execed MS00144475

   if (MUX_OPTION_BASIC == config_data->sys_parameter.mux_mode)
   {
        if ((data_len + MUX_MAX_HDR_SIZE + FLAG_SIZE) > MUX_MAX_BUF_SIZE)
        {
            //MUX_TRACE_WARNING:"Mux_Send_UIH: Fail due to length is too long in %d DLCI of %d bytes data\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_744_112_2_18_1_49_20_53,(uint8*)"dd", data_len, dlci);
            return MUX_RET_FAILURE;
        }
   }

/*+cr250844*/
/*-cr250844*/


    //MS00138417 
    if (mux_send_mutex)
    {
        MUX_GET_MUTEX(mux_send_mutex, MUX_INVALID_BLOCK_ID != MUX_IdentifyThread() ? MUX_WAIT_FOREVER : 0);
    }

    switch (config_data->sys_parameter.mux_mode)
    {
        case MUX_OPTION_BASIC:
            {
                MUX_FRAME_HEAD *uih_pkt_head_ptr;
                MUX_MEMSET(buff, 0x00, sizeof(buff));

                uih_pkt_head_ptr = (MUX_FRAME_HEAD *) buff;
                uih_pkt_head_ptr->addr.ea = 1;
                uih_pkt_head_ptr->addr.cr = ((SET_CR(config_data->side)) & 0x1);
                uih_pkt_head_ptr->addr.dlc_chn = dlci;
                uih_pkt_head_ptr->control = CLR_PF(MUX_UIH);

                if (data_len > MUX_SHORTFRAME_INF_MAX)
                {
                    /*length filed is 2 bytes*/
                    SET_LONG_LENGTH(uih_pkt_head_ptr->length, data_len);
                    head_size = sizeof(MUX_FRAME_HEAD);/*head_size = add+control+length*/
                    fcs = MUX_Crc_Calc(buff, LONG_CRC_CHECK);
                }
                else
                {
                    /*length filed is 1 bytes*/
                    uih_pkt_head_ptr->length.ea = 1;
                    uih_pkt_head_ptr->length.l_len = data_len;
                    head_size = sizeof(MUX_FRAME_HEAD) - 1;/*head_size = add+control+length*/
                    fcs = MUX_Crc_Calc(buff, SHORT_CRC_CHECK);
                }         

                /*1: basic flag
                   head_size : header field
                   data_len: data field
                   1: fcs
                   1: basic flag*/

                total_size = 0;

                mux_tx_buf_ptr[total_size++] = MUX_BASIC_FLAG;

                for (i = 0; i < head_size; i++)
                {
                    mux_tx_buf_ptr[total_size++] = buff[i];
                }

                for (i = 0; i < data_len; i++)
                {
                    mux_tx_buf_ptr[total_size++] = *data_ptr++;
                }

                mux_tx_buf_ptr[total_size++] = fcs;

                mux_tx_buf_ptr[total_size++] = MUX_BASIC_FLAG;
                //save mux frame to log for wireshark analyzation
                SCI_TRACE_DATA(7, mux_tx_buf_ptr, total_size);                
            }
            break;
        case MUX_OPTION_ADVANCED:
            {
                MUX_ADVANCE_FRAME_HEAD_T   *uih_pkt_head_ptr;
                MUX_MEMSET(buff, 0x00, sizeof(buff));

                uih_pkt_head_ptr = (MUX_ADVANCE_FRAME_HEAD_T *) buff;
                uih_pkt_head_ptr->addr.ea = 1;
                uih_pkt_head_ptr->addr.cr = ((SET_CR(config_data->side)) & 0x1);
                uih_pkt_head_ptr->addr.dlc_chn = dlci;
                uih_pkt_head_ptr->control = CLR_PF(MUX_UIH);
                head_size = 2;/*sizeof(uih_pkt_head_ptr)*/

                fcs = MUX_Crc_Calc((uint8 *) uih_pkt_head_ptr, 2);

                total_size = 0;

                mux_tx_buf_ptr[total_size++] = MUX_ADVANCE_FLAG;

                for (i = 0; i < head_size; i++)
                {
                    mux_tx_buf_ptr[total_size++] = buff[i];
                }

                for (i = 0; i < data_len; i++)
                {
                    if ((MUX_ADVANCE_FLAG == *data_ptr) || (MUX_ESCAPE_FLAG == *data_ptr))
                    {
                        /*convert char process*/     
                        mux_tx_buf_ptr[total_size++] = MUX_ESCAPE_FLAG;
                        mux_tx_buf_ptr[total_size++] = (*data_ptr ^ MUX_COMPLEMENT_BYTE);
                    }
                    else
                    {
                        mux_tx_buf_ptr[total_size++] = *data_ptr;
                    }

                    data_ptr++;

                    if (total_size > MUX_MAX_BUF_SIZE) //MS00144475 Judge data length, avoid memory execed
                    {
                        //MUX_TRACE_WARNING:"Mux_Send_UIH: Fail in Creating UIH packet with %d bytes data to DLCI %d due to length is too long\n"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_861_112_2_18_1_49_20_54,(uint8*)"dd", data_len, dlci);
                        return MUX_RET_FAILURE;
                    }

                }

                mux_tx_buf_ptr[total_size++] = fcs;
                mux_tx_buf_ptr[total_size++] = MUX_ADVANCE_FLAG;

            }
            break;
        case MUX_OPTION_ADVANCED_ERM:
        default:
            //MUX_TRACE_WARNING:"MUX_Send_UIH: Creating DM packet to DLCI %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_874_112_2_18_1_49_20_55,(uint8*)"ddd", dlci);
            break;
    }

    MUX_Data_Tx(mux_tx_buf_ptr, total_size, config_data->mux_index);   
    if (mux_send_mutex)
    {
        MUX_PUT_MUTEX(mux_send_mutex);
    }

    
#ifdef _MUX_DEBUG_
    /*In order to debug, trace*/
    MUX_TraceData(mux_tx_buf_ptr, total_size); 
#endif

    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    this function is used to Creates a SABM packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.7.29
// Note :           
/*****************************************************************************/
LOCAL MUX_RETURN_E MUX_Send_SABM(MUX_INFO_T *config_data, uint8 dlci)
{
    uint8           buf[MUX_SHORTFRAME_OTHER_SIZE];
    MUX_RETURN_E    res;
    uint8           send_len = 0;
    uint8          *send_data_ptr = NULL;

    //MUX_TRACE_WARNING:"Mux_Send_SABM: Creating SABM packet to DLCI %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_905_112_2_18_1_49_20_56,(uint8*)"d", dlci);
    switch (config_data->sys_parameter.mux_mode)
    {
        case MUX_OPTION_BASIC:
            {
                MUX_FRAME_HEAD *sabm_head_ptr;
                MUX_MEMSET(buf, 0x00, MUX_SHORTFRAME_OTHER_SIZE);
                sabm_head_ptr = (MUX_FRAME_HEAD *) (buf + 1);
                sabm_head_ptr->addr.ea = 1;
                sabm_head_ptr->addr.cr = ((SET_CR(config_data->side)) & 0x1);
                sabm_head_ptr->addr.dlc_chn = dlci;
                sabm_head_ptr->control = SET_PF(MUX_SABM);
                sabm_head_ptr->length.ea = 1;
                sabm_head_ptr->length.l_len = 0;
                buf[0] = MUX_BASIC_FLAG;
                buf[MUX_SHORTFRAME_OTHER_SIZE - 2] = MUX_Crc_Calc((uint8 *) sabm_head_ptr, SHORT_CRC_CHECK);
                buf[MUX_SHORTFRAME_OTHER_SIZE - 1] = MUX_BASIC_FLAG;
                send_data_ptr = buf;
                send_len = MUX_SHORTFRAME_OTHER_SIZE;

                //save mux frame to log for wireshark analyzation
                SCI_TRACE_DATA(7, buf, send_len);                
            }           
            break;
        case MUX_OPTION_ADVANCED:
            {
                MUX_ADVANCE_FRAME_HEAD_T   *sabm_head_ptr;
                MUX_MEMSET(buf, 0x00, MUX_SHORTFRAME_OTHER_SIZE);
                send_len = 0;
                buf[0] = MUX_ADVANCE_FLAG;
                send_len += 1;

                sabm_head_ptr = (MUX_ADVANCE_FRAME_HEAD_T *) (buf + 1);
                sabm_head_ptr->addr.ea = 1;
                sabm_head_ptr->addr.cr = ((SET_CR(config_data->side)) & 0x1);
                sabm_head_ptr->addr.dlc_chn = dlci;
                sabm_head_ptr->control = SET_PF(MUX_SABM);
                send_len += 2;//sizeof(MUX_ADVANCE_FRAME_HEAD_T);

                buf[send_len++] = MUX_Crc_Calc((uint8 *) sabm_head_ptr, 2);
                buf[send_len++] = MUX_ADVANCE_FLAG;
                send_data_ptr = buf;
            }
            break;
        case MUX_OPTION_ADVANCED_ERM:
        default:
            //MUX_TRACE_WARNING:"Mux_Send_SABM: Fail in Creating SABM packet to DLCI %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_948_112_2_18_1_49_20_57,(uint8*)"d", dlci);
            break;
    }

    res = MUX_Data_Tx(send_data_ptr, send_len, config_data->mux_index);

    return res;
}
/*****************************************************************************/
// Description :    this function is used to Parse the mux control channel information 
// Global resource dependence :
// Author :         shenmiliang
// DATE :         2007.7.25
// Note :           
/*****************************************************************************/
LOCAL void MUX_MCCFrame_Parse(uint8 *data, uint32 len, MUX_INFO_T *config_data)
{
    MUX_MCC_SHORT_FRAME_T  *mcc_short_pkt;
    uint8                   j;

    mcc_short_pkt = (MUX_MCC_SHORT_FRAME_T *) data;

    switch (mcc_short_pkt->h.type.type)
    {
        case MUX_PN:
            /*DLC parameter negotiation */
            {
                uint8               dlci;
                uint8               link_id;
                MUX_PN_STRUCT_T    *pn_pkt;

                pn_pkt = (MUX_PN_STRUCT_T *) (mcc_short_pkt->value);
                dlci = pn_pkt->dlci;
                if (MUX_DLC_LINK_INVALID == (link_id = MUX_Get_Linkid(dlci,config_data->mux_index)))
                {
                    //MUX_TRACE_WARNING:"Mux_MCCFrame_Parse error DLCI %d in PN\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_981_112_2_18_1_49_20_58,(uint8*)"d", dlci);
                    break;//discard
                }
                //MUX_TRACE_WARNING:"MUX: Received DLC %d (link %d)parameter negotiation, PN\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_984_112_2_18_1_49_20_59,(uint8*)"dd", dlci, link_id);

                if (mcc_short_pkt->h.type.cr == MUX_MCC_CMD)
                {
                    MUX_Check_PN(pn_pkt->pn_val, dlci, config_data);
                    MUX_Send_MCC_PN(&(g_mux_dlc_info[link_id].param), dlci, MUX_MCC_RSP, config_data);
                }
                else
                {
                    MUX_Check_PN(pn_pkt->pn_val, dlci, config_data);

                    if (MUX_DLC_STATE_NEGOTIATING == g_mux_dlc_info[link_id].used_state)
                    {
                        g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_CONNECTING;
                        MUX_Wait_WakeUp(g_mux_dlc_info[link_id].open_wait_event, config_data);
                    }
                }
                break;
            }       

        case MUX_PSC:
            //MUX_TRACE_WARNING:"MUX: Received DLC power saving msg\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1005_112_2_18_1_49_20_60,(uint8*)"");
            if (mcc_short_pkt->h.type.cr == MUX_MCC_CMD)
            {
                MUX_Send_MCC_PSC(MUX_MCC_RSP, config_data);
            }
            else
            {
                /*In current version, MUX never initiative send PSC message*/
                /*So recv Rsp, discard*/
            }
            /*How to implement Power saving ,Need consider again*/
            break;
        case MUX_CLD:
            if (mcc_short_pkt->h.type.cr == MUX_MCC_CMD)
            {
                MUX_Send_MCC_CLD(MUX_MCC_RSP, config_data);
                MUX_Upon_Close(config_data);
            }
            else
            {
                /*In current version, MUX never initiative send CLD message*/
                /*So recv Rsp, discard*/
            }
            break;
        case MUX_TEST:
             //MUX_TRACE_WARNING:"MUX: Received test  msg\n"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1030_112_2_18_1_49_20_61,(uint8*)"");

            if (mcc_short_pkt->h.type.cr == MUX_MCC_CMD)
            {
                if ((mcc_short_pkt->h.length.ea) == 0)
                {
                    MUX_MCC_LONG_FRAME_T   *mcc_long_pkt;
                    mcc_long_pkt = (MUX_MCC_LONG_FRAME_T *) mcc_short_pkt;
                    MUX_Send_MCC_TEST(mcc_long_pkt->value, GET_LONG_LENGTH(mcc_long_pkt->h.length), MUX_MCC_RSP, config_data);
                }
                else
                {
                    MUX_Send_MCC_TEST(mcc_short_pkt->value, mcc_short_pkt->h.length.len, MUX_MCC_RSP, config_data);
                }
            }
            else
            {
                /*In current version, MUX never initiative send TEST message*/
                /*So recv Rsp, discard*/
            }
            break;

        case MUX_FCON:
              //MUX_TRACE_WARNING:"MUX: Received FCON  msg\n"
              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1053_112_2_18_1_49_20_62,(uint8*)"");

            /*Flow control on command */
            if (mcc_short_pkt->h.type.cr == MUX_MCC_CMD)
            {
                for (j = 1; j < MUX_MAX_LINK_NUM; j++)
                {
                    if (MUX_DLC_STATE_FLOW_STOPPED == g_mux_dlc_info[j].used_state)
                    {
                        g_mux_dlc_info[j].used_state = MUX_DLC_STATE_CONNECT;
                    }
                }

                MUX_Send_MCC_FCon(MUX_MCC_RSP, config_data);
            }
            else
            {
                /*In current version, MUX never initiative send  FLOW CTL message*/
                /*So recv Rsp, discard*/
            }
            break;

        case MUX_FCOFF:
            //MUX_TRACE_WARNING:"MUX: Received FCOFF msg\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1076_112_2_18_1_49_20_63,(uint8*)"");
            /*Flow control off command */
            if (mcc_short_pkt->h.type.cr == MUX_MCC_CMD)
            {
                for (j = 1; j < MUX_MAX_LINK_NUM; j++)
                {
                    if (MUX_DLC_STATE_CONNECT == g_mux_dlc_info[j].used_state)
                    {
                        g_mux_dlc_info[j].used_state = MUX_DLC_STATE_FLOW_STOPPED;
                    }
                }

                MUX_Send_MCC_FCoff(MUX_MCC_RSP, config_data);
            }
            else
            {
                /*In current version, MUX never initiative send  FLOW CTL message*/
                /*So recv Rsp, discard*/
            }
            break;

        case MUX_MSC:
            /*Modem status command */
            {
                uint8           dlci;
                uint8           link_id;
                MUX_MSC_STRUCT *msc_pkt;

                msc_pkt = (MUX_MSC_STRUCT *) (mcc_short_pkt->value);
                dlci = msc_pkt->dlci.dlc_chn;

                if (MUX_DLC_LINK_INVALID == (link_id = MUX_Get_Linkid(dlci, config_data->mux_index)))
                {
                    //MUX_TRACE_WARNING:"Mux_MCCFrame_Parse error DLCI %d in MSC\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1109_112_2_18_1_49_20_64,(uint8*)"d", dlci);
                    break;//discard
                }

                if ((g_mux_dlc_info[link_id].used_state != MUX_DLC_STATE_CONNECT) && (g_mux_dlc_info[link_id].used_state != MUX_DLC_STATE_FLOW_STOPPED))
                {
                    MUX_Send_DM(config_data, dlci);
                    //MUX_TRACE_WARNING:"MUX reject received MSC message in dlci %d (link %d)\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1116_112_2_18_1_49_20_65,(uint8*)"dd", dlci, link_id);
                }
                else
                {
                    if (mcc_short_pkt->h.type.cr == MUX_MCC_CMD)
                    {
                        if (msc_pkt->v24_sigal.fc)
                        {
                            if (MUX_DLC_STATE_CONNECT == g_mux_dlc_info[link_id].used_state)
                            {
                                g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_FLOW_STOPPED;
                                //MUX_TRACE_WARNING:"MUX Received Flow off on dlci %d (link %d)\n"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1127_112_2_18_1_49_20_66,(uint8*)"dd", dlci, link_id);
                            }
                        }
                        else
                        {
                            if (MUX_DLC_STATE_FLOW_STOPPED == g_mux_dlc_info[link_id].used_state)
                            {
                                g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_CONNECT;
                                //MUX_TRACE_WARNING:"MUX Received Flow on on dlci %d (link %d)\n"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1135_112_2_18_1_49_21_67,(uint8*)"dd", dlci, link_id);
                            }
                        }
                        /*How to implement flow control ,Need consider again*/

                        MUX_Send_MCC_MSC(msc_pkt->v24_sigal, dlci, MUX_MCC_RSP, config_data);
                    }
                }    
                break;
            }
        case MUX_NSC:
            /*Non supported command resonse */
            //MUX_TRACE_WARNING:"MUX Received Non supported command response\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1147_112_2_18_1_49_21_68,(uint8*)"");
            break;
        case MUX_SNC:
            /* Service Negotiation Command  */
            {
                uint8               dlci;
                uint8               link_id;
                MUX_SNC_STRUCT_T   pkt;
                MUX_SNC_STRUCT_T* snc_pkt = &pkt;
                uint8 tchar[42] = {0};

                MUX_MEMSET(&pkt, 0x00, sizeof(MUX_SNC_STRUCT_T));
                MUX_MEMCPY(&pkt, mcc_short_pkt->value, mcc_short_pkt->h.length.len);
                dlci = snc_pkt->dlci.dlc_chn;

                if (MUX_DLC_LINK_INVALID == (link_id = MUX_Get_AllLinkid(dlci, config_data->mux_index)))
                {
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1109_112_2_18_1_49_20_64,(uint8*)"d", dlci);
                    break;//discard
                }

                ConvertBinToHex(data, (len > 20) ? 20 : len, tchar);
                MUX_TRACE_LOW(("MUX SNC Buffer len:%d, hex:%s", len, tchar));

                MUX_TRACE_LOW(("MUX_MCCFrame_Parse->MUX_SNC, dlci: %d, len: %d, used_state: %d, cid: %d, service: %d. BE", \
                                             dlci, mcc_short_pkt->h.length.len, g_mux_dlc_info[link_id].used_state, g_mux_dlc_info[link_id].cid, g_mux_dlc_info[link_id].service));


#ifdef MUX_PARSE_SNC_AFTER_DLC_CREATED
                if ((g_mux_dlc_info[link_id].used_state != MUX_DLC_STATE_CONNECT) && (g_mux_dlc_info[link_id].used_state != MUX_DLC_STATE_FLOW_STOPPED))
                {
                    MUX_Send_DM(config_data, dlci);
                }
                else
#endif
                {
                    // query only
                    if (mcc_short_pkt->h.length.len == 1)
                    {
                        // get supported services
                        snc_pkt->svc.svc      = MUX_SNC_Get_Supported_Services();
                        // get supported codec
                        snc_pkt->codec.type = MUX_SNC_Get_Supported_Codecs();
                    }
                    // param set
                    else
                    {
                        g_mux_dlc_info[link_id].service = MUX_DLC_SERVICE_NULL;
                        g_mux_dlc_info[link_id].cid = 0xFF;
                        g_mux_dlc_info[link_id].codec = MUX_DLC_CODEC_NULL;

                        // set service
                        g_mux_dlc_info[link_id].service = ((snc_pkt->svc.svc) & MUX_SNC_Get_Supported_Services());

                        // set codec
                        if (snc_pkt->svc.svc == MUX_DLC_SERVICE_VOICE) 
                        {
                            g_mux_dlc_info[link_id].codec = ((snc_pkt->codec.type) & MUX_SNC_Get_Supported_Codecs());
                        }

                        // set work mode
                        if ((g_mux_dlc_info[link_id].service & MUX_DLC_SERVICE_MASK) == 0)
                        {
                            // Set work mode to AT mode
                            g_mux_dlc_info[link_id].work_mode = MUX_DLC_AT_MODE;
                        }
                        #if 0  // AT Command can be sent between SNC enable data service and successfully ppp dial. And work_mode will be changed after PPP dial
                        else
                        {
                            // Set work mode to Data mode
                            g_mux_dlc_info[link_id].work_mode = MUX_DLC_DATA_MODE;
                        }
                        #endif
                        MUX_TRACE_LOW(("MUX work mode: %d", g_mux_dlc_info[link_id].work_mode));
                    }

                    MUX_Send_MCC_SNC(snc_pkt,  MUX_MCC_RSP, config_data);
                }

                MUX_TRACE_LOW(("MUX_MCCFrame_Parse->MUX_SNC, dlci: %d, len: %d, used_state: %d, cid: %d, service: %d, codec: %d. AF", 
                                             dlci, mcc_short_pkt->h.length.len, g_mux_dlc_info[link_id].used_state, g_mux_dlc_info[link_id].cid,
                                             g_mux_dlc_info[link_id].service, g_mux_dlc_info[link_id].codec));

                break;
            }    
        case MUX_RPN:
        default:
            /*Non supported command received */
            //MUX_TRACE_WARNING:"MUX Received a non supported command\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1152_112_2_18_1_49_21_69,(uint8*)"");
            MUX_Send_MCC_NSC(mcc_short_pkt->h.type, MUX_MCC_RSP, config_data);
            break;
    }
    return;
}
/*****************************************************************************/
// Description :    this function is used to set max_windown size.
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.7.25
// Note :           
/*****************************************************************************/
LOCAL void MUX_Check_PN(MUX_PN_DATA_T  pn_data, uint8 dlci, MUX_INFO_T *config_ptr)
{
    MUX_PN_DATA_T  *pn_data_confirm;
    uint32          frame_size;
    uint32          frame_size_confirm;

    /*MUX_UIH_handler has check link_id, so NOT need check again.*/
    uint8           link_id = MUX_Get_Linkid(dlci, config_ptr->mux_index);


    pn_data_confirm = &(g_mux_dlc_info[link_id].param);

    frame_size = GET_PN_MSG_FRAME_SIZE(&pn_data);

    MUX_TRACE_LOW(("MUX received PN. Frame size:%d, in dlci %d (link %d)\n", frame_size, dlci,link_id));


    frame_size_confirm = GET_PN_MSG_FRAME_SIZE(pn_data_confirm);
    frame_size = MIN(frame_size, frame_size_confirm);
    SET_PN_MSG_FRAME_SIZE(pn_data_confirm, frame_size);

    return;
}



/*****************************************************************************/
// Description :    this function is used to Creates a MCC_PN packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.8.1
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_PN(MUX_PN_DATA_T *pn_data, uint8 dlci, uint8 cr, MUX_INFO_T *config_data)
{
    uint8                   buf[MUX_MCCFRAME_BUFFSIZE];
    MUX_MCC_SHORT_FRAME_T  *mcc_msg;
    MUX_PN_STRUCT_T        *pn_packet;
    uint32                  size;
    uint32                  res;

    //MUX_TRACE_WARNING:"Mux_Send_MCC_PN start in dlci %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1202_112_2_18_1_49_21_70,(uint8*)"d", dlci);

    MUX_MEMSET(buf, 0x00, sizeof(buf));
    size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T) + sizeof(MUX_PN_STRUCT_T);

    mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
    mcc_msg->h.type.ea = 1;
    mcc_msg->h.type.cr = cr;
    mcc_msg->h.type.type = MUX_PN;
    mcc_msg->h.length.ea = 1;
    mcc_msg->h.length.len = 8;
    pn_packet = (MUX_PN_STRUCT_T *) (mcc_msg->value);
    pn_packet->dlci = dlci;
    pn_packet->res1 = 0;

    MUX_MEMCPY(&(pn_packet->pn_val), pn_data, sizeof(MUX_PN_DATA_T));

    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);

    return res;
}

/*****************************************************************************/
// Description :    this function is used to Creates a MCC_PSC packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.8.1
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_PSC(uint8 cr,MUX_INFO_T *config_data)
{
    uint8                   buf[MUX_MCCFRAME_BUFFSIZE];
    MUX_MCC_SHORT_FRAME_T  *mcc_msg;
    uint32                  size;
    uint32                  res;

    //MUX_TRACE_WARNING:"Mux_Send_MCC_PSC start cr = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1236_112_2_18_1_49_21_71,(uint8*)"d", cr);

    MUX_MEMSET(buf, 0x00, sizeof(buf));
    size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T);

    mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
    mcc_msg->h.type.ea = 1;
    mcc_msg->h.type.cr = cr;
    mcc_msg->h.type.type = MUX_PSC;
    mcc_msg->h.length.ea = 1;
    mcc_msg->h.length.len = 0;
    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);
    return res;
}


/*****************************************************************************/
// Description :    this function is used to Creates a MCC_CLD packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.8.1
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_CLD(uint8 cr, MUX_INFO_T *config_data)
{
    uint8                   buf[MUX_MCCFRAME_BUFFSIZE];
    MUX_MCC_SHORT_FRAME_T  *mcc_msg;
    uint32                  size;
    uint32                  res;

    //MUX_TRACE_WARNING:"Mux_Send_MCC_CLD start cr= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1264_112_2_18_1_49_21_72,(uint8*)"d",cr);

    MUX_MEMSET(buf, 0x00, sizeof(buf));
    size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T);

    mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
    mcc_msg->h.type.ea = 1;
    mcc_msg->h.type.cr = cr;
    mcc_msg->h.type.type = MUX_CLD;
    mcc_msg->h.length.ea = 1;
    mcc_msg->h.length.len = 0;
    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);

    return res;
}

/*****************************************************************************/
// Description :    this function is used to Creates a MCC_TEST packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.8.1
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_TEST(uint8 *data, uint32 length, uint8 cr, MUX_INFO_T *config_data)
{
    uint8   buf[MUX_MMC_SIZE_MAX];
    uint32  size = 0;
    uint32  res;

    //MUX_TRACE_WARNING:"Mux_Send_MCC_TEST start"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1291_112_2_18_1_49_21_73,(uint8*)"");

    MUX_MEMSET(buf, 0x00, sizeof(buf));

    MUX_ASSERT(length < MUX_MCC_VALUE_OCTETS_NUM_MAX);/*assert verified*/

    if (length > MUX_SHORTFRAME_INF_MAX)
    {
#if 0       
        MUX_MCC_LONG_FRAME *mcc_msg;
        
        size = sizeof(MUX_MCC_LONG_FRAME_HEAD)+length;
        mcc_msg = (MUX_MCC_LONG_FRAME *) buf;
        mcc_msg->h.type.ea = 1;
        mcc_msg->h.type.cr = cr;
        mcc_msg->h.type.type = MUX_TEST;
        SET_LONG_LENGTH(mcc_msg->h.length,length);
        MUX_MEMCPY(mcc_msg->value,data,length);
#endif  
        //MUX_TRACE_WARNING:"Mux_Send_MCC_TEST: TEST Message excede max length\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1310_112_2_18_1_49_21_74,(uint8*)"");
    }
    else
    {
        MUX_MCC_SHORT_FRAME_T  *mcc_msg;

        size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T) + length;

        mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
        mcc_msg->h.type.ea = 1;
        mcc_msg->h.type.cr = cr;
        mcc_msg->h.type.type = MUX_TEST;
        mcc_msg->h.length.ea = 1;
        mcc_msg->h.length.len = length;
        MUX_MEMCPY(mcc_msg->value, data, length);
    }

    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);

    return res;
}

/*****************************************************************************/
// Description :    this function is used to Creates a MCC_FCon packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.8.1
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_FCon(uint8 cr, MUX_INFO_T *config_data)
{
    uint8                   buf[MUX_MCCFRAME_BUFFSIZE];
    MUX_MCC_SHORT_FRAME_T  *mcc_msg;
    uint32                  size;
    uint32                  res;

    //MUX_TRACE_WARNING:"Mux_Send_MCC_FCon start cr = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1344_112_2_18_1_49_21_75,(uint8*)"d", cr);

    MUX_MEMSET(buf, 0x00, sizeof(buf));
    size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T);

    mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
    mcc_msg->h.type.ea = 1;
    mcc_msg->h.type.cr = cr;
    mcc_msg->h.type.type = MUX_FCON;
    mcc_msg->h.length.ea = 1;
    mcc_msg->h.length.len = 0;
    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);

    return res;
}

/*****************************************************************************/
// Description :    this function is used to Creates a MCC_FCoff packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.8.1
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_FCoff(uint8 cr ,MUX_INFO_T *config_data)
{
    uint8                   buf[MUX_MCCFRAME_BUFFSIZE];
    MUX_MCC_SHORT_FRAME_T  *mcc_msg;
    uint32                  size;
    uint32                  res;

    //MUX_TRACE_WARNING:"Mux_Send_MCC_FCoff start cr = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1372_112_2_18_1_49_21_76,(uint8*)"d", cr);

    MUX_MEMSET(buf, 0x00, sizeof(buf));
    size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T);

    mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
    mcc_msg->h.type.ea = 1;
    mcc_msg->h.type.cr = cr;
    mcc_msg->h.type.type = MUX_FCOFF;
    mcc_msg->h.length.ea = 1;
    mcc_msg->h.length.len = 0;

    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);

    return res;
}



/*****************************************************************************/
// Description :    this function is used to Creates a MCC_MSC packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.8.1
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_MSC(MUX_V24_SIGNAL_T v24_signal, uint8 dlci, uint8 cr, MUX_INFO_T *config_data)
{
    uint8                   buf[MUX_MCCFRAME_BUFFSIZE];
    MUX_MCC_SHORT_FRAME_T  *mcc_msg;
    MUX_MSC_STRUCT         *msc_packet; 
    uint32                  size;
    uint32                  res;

    //MUX_TRACE_WARNING:"Mux_Send_MCC_MSC start in dlci %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1404_112_2_18_1_49_21_77,(uint8*)"d", dlci);

    MUX_MEMSET(buf, 0x00, sizeof(buf));
    size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T) + sizeof(MUX_MSC_STRUCT);

    mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
    mcc_msg->h.type.ea = 1;
    mcc_msg->h.type.cr = cr;
    mcc_msg->h.type.type = MUX_MSC;
    mcc_msg->h.length.ea = 1;
    mcc_msg->h.length.len = 2;
    msc_packet = (MUX_MSC_STRUCT *) (mcc_msg->value);
    msc_packet->dlci.ea = 1;
    msc_packet->dlci.cr = 1;
    msc_packet->dlci.dlc_chn = dlci;
    msc_packet->v24_sigal = v24_signal;

    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);

    return res;
}

/*****************************************************************************/
// Description :    this function is used to Creates a MCC_MSC packet and put it to sendbuff
// Global resource dependence :
// Author :         yinqiu.hu
// DATE :         2013.10.31
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_SNC(MUX_SNC_STRUCT_T *snc_pkt, uint8 cr, MUX_INFO_T *config_data)
{
    uint8                   buf[MUX_MCCFRAME_BUFFSIZE];
    MUX_MCC_SHORT_FRAME_T  *mcc_msg;
    MUX_SNC_STRUCT_T         *snc_rsp_packet; 
    MUX_SNC_STRUCT_T         *snc_cmd_packet; 

    uint32                  size;
    uint32                  res;

    MUX_MEMSET(buf, 0x00, sizeof(buf));
    size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T) + sizeof(MUX_SNC_STRUCT_T);

    mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
    mcc_msg->h.type.ea = 1;
    mcc_msg->h.type.cr = cr;
    mcc_msg->h.type.type = MUX_SNC;
    mcc_msg->h.length.ea = 1;
    mcc_msg->h.length.len = 3;
    snc_rsp_packet = (MUX_SNC_STRUCT_T *) (mcc_msg->value);
    snc_rsp_packet->dlci.ea = 1;
    snc_rsp_packet->dlci.cr = 1;
    snc_rsp_packet->dlci.dlc_chn = snc_pkt->dlci.dlc_chn;
    snc_rsp_packet->svc.ea = 1;
    
    snc_rsp_packet->svc.svc = snc_pkt->svc.svc & MUX_SNC_Get_Supported_Services();
    snc_rsp_packet->codec.ea = 1;
    if (snc_pkt->svc.svc == MUX_DLC_SERVICE_VOICE)
    {	
        snc_rsp_packet->codec.type = MUX_SNC_Get_Supported_Codecs();
    }

    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);

    return res;
}
/*****************************************************************************/
// Description :    this function is used to Creates a MCC_NSC packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.8.1
// Note :           
/*****************************************************************************/
LOCAL uint32 MUX_Send_MCC_NSC(MUX_MCC_TYPE cmd, uint8 cr, MUX_INFO_T *config_data)
{
    uint8                   buf[MUX_MCCFRAME_BUFFSIZE];
    MUX_MCC_SHORT_FRAME_T  *mcc_msg;
    uint32                  size;
    uint32                  res;

    //MUX_TRACE_WARNING:"Mux_Send_MCC_NSC START"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1438_112_2_18_1_49_21_78,(uint8*)"");

    MUX_MEMSET(buf, 0x00, sizeof(buf));
    size = sizeof(MUX_MCC_SHORT_FRAME_HEAD_T) + sizeof(MUX_MCC_TYPE);

    mcc_msg = (MUX_MCC_SHORT_FRAME_T *) buf;
    mcc_msg->h.type.ea = 1;
    mcc_msg->h.type.cr = cr;
    mcc_msg->h.type.type = MUX_NSC;
    mcc_msg->h.length.ea = 1;
    mcc_msg->h.length.len = 1;
    MUX_MEMCPY((MUX_MCC_TYPE *) (mcc_msg->value), &cmd, sizeof(MUX_MCC_TYPE));

    res = MUX_Send_UIH(config_data, buf, size, MUX_CTRL_CHAN);

    return res;
}

/*****************************************************************************/
// Description :    this function is used to Creates a DISC packet and put it to sendbuff
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.7.29
// Note :           
/*****************************************************************************/
LOCAL MUX_RETURN_E MUX_Send_Disc(MUX_INFO_T *config_data, uint8 dlci)
{
    uint8           buf[MUX_SHORTFRAME_OTHER_SIZE];
    MUX_RETURN_E    res;
    uint8           send_len = 0;

    //MUX_TRACE_WARNING:"Mux_Send_Disc: Creating DISC packet to DLCI %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1467_112_2_18_1_49_21_79,(uint8*)"d", dlci);
    switch (config_data->sys_parameter.mux_mode)
    {
        case MUX_OPTION_BASIC:
            {
                MUX_FRAME_HEAD *disc_head_ptr;
                disc_head_ptr = (MUX_FRAME_HEAD *) (buf + 1);
                disc_head_ptr->addr.ea = 1;
                disc_head_ptr->addr.cr = ((SET_CR(config_data->side)) & 0x1);
                disc_head_ptr->addr.dlc_chn = dlci;
                disc_head_ptr->control = SET_PF(MUX_DISC);
                disc_head_ptr->length.ea = 1;
                disc_head_ptr->length.l_len = 0;
                buf[0] = MUX_BASIC_FLAG;
                buf[MUX_SHORTFRAME_OTHER_SIZE - 2] = MUX_Crc_Calc((uint8 *) disc_head_ptr, SHORT_CRC_CHECK);
                buf[MUX_SHORTFRAME_OTHER_SIZE - 1] = MUX_BASIC_FLAG;
                send_len = MUX_SHORTFRAME_OTHER_SIZE;

                //save mux frame to log for wireshark analyzation
                SCI_TRACE_DATA(7, buf, send_len);                
            }
            break;
        case MUX_OPTION_ADVANCED:
            {
                MUX_ADVANCE_FRAME_HEAD_T   *disc_head_ptr;

                send_len = 0;
                buf[0] = MUX_ADVANCE_FLAG;
                send_len += 1;

                disc_head_ptr = (MUX_ADVANCE_FRAME_HEAD_T *) (buf + 1);
                disc_head_ptr->addr.ea = 1;
                disc_head_ptr->addr.cr = ((SET_CR(config_data->side)) & 0x1);
                disc_head_ptr->addr.dlc_chn = dlci;
                disc_head_ptr->control = SET_PF(MUX_DISC);
                send_len += sizeof(MUX_ADVANCE_FRAME_HEAD_T);

                buf[send_len++] = MUX_Crc_Calc((uint8 *) disc_head_ptr, 2);
                buf[send_len++] = MUX_ADVANCE_FLAG;
            }
            break;
        case MUX_OPTION_ADVANCED_ERM:
        default:
            //MUX_TRACE_WARNING:"MUX_Send_Disc: Fail in Creating DISC packet to DLCI %d\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1507_112_2_18_1_49_21_80,(uint8*)"d", dlci);
            break;
    }


    res = MUX_Data_Tx(buf, send_len, config_data->mux_index);

    return res;
}
/*****************************************************************************/
// Description :    this function is used to close a dlc channel
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.7.29
// Note :     Only Used by TE side
/*****************************************************************************/
LOCAL MUX_RETURN_E MUX_Close_Dlci(uint8 dlci, MUX_INFO_T *mux_ptr)
{
    int     retry_times;
    BOOLEAN is_overtime;
    uint8   link_id;
    uint32  mux_timeout;

    MUX_ASSERT(mux_ptr->mux_index < MAX_MUX_NUM);/*assert verified*/
    
    link_id = MUX_Get_Linkid(dlci, mux_ptr->mux_index);
    if (MUX_DLC_LINK_INVALID == link_id)
    {
        //MUX_TRACE_WARNING:"Mux_Close_Channel: DLC %d is NOT exist \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1533_112_2_18_1_49_21_81,(uint8*)"d", dlci);
        return MUX_RET_FAILURE;
    }

    if ((MUX_DLC_STATE_DISCONNECT == g_mux_dlc_info[link_id].used_state) || (MUX_DLC_STATE_REJECTED == g_mux_dlc_info[link_id].used_state))
    {
        //MUX_TRACE_WARNING:"Mux_Close_Channel: DLC %d (link %d) is already in DISC state\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1539_112_2_18_1_49_21_82,(uint8*)"dd", dlci,link_id);

        return MUX_RET_SUCCESS;
    }
    else if (MUX_DLC_STATE_DISCONNECTING == g_mux_dlc_info[link_id].used_state)
    {
        /* Reentry */
        //MUX_TRACE_WARNING:"Mux_Close_Channel: DLC %d (link %d) is already in DISC state\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1546_112_2_18_1_49_21_83,(uint8*)"dd", dlci, link_id);
        return MUX_RET_FAILURE;
    }
    else
    {
        //MUX_TRACE_WARNING:"Mux_Close_Channel on channel %d (link %d)\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1551_112_2_18_1_49_21_84,(uint8*)"dd", dlci, link_id);

        /*N2: re-transmit times*/
        retry_times = mux_ptr->sys_parameter.N2;

        while (retry_times--)
        {
            g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_DISCONNECTING;
            MUX_Send_Disc(mux_ptr, dlci);
            /*Adopt Event way to wait RESPONSE from peer.*/
            mux_timeout = MAX(MUX_TIME_OUT, 10 * mux_ptr->sys_parameter.T2);
            is_overtime = MUX_Res_Wait(g_mux_dlc_info[link_id].close_wait_event, mux_timeout, mux_ptr);
            if (MUX_DLC_STATE_DISCONNECT == g_mux_dlc_info[link_id].used_state)
            {
                /*Release success*/
                MUX_TRACE_LOW(("MUX DLCI %d (link %d) release success!\n", dlci, link_id));
                break;
            }
            else if (!is_overtime)
            {
                //MUX_TRACE_WARNING:"MUX DLCI %d (link %d) Send DISC timeout!\n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1571_112_2_18_1_49_21_85,(uint8*)"dd", dlci, link_id);
                continue;
            }
        }

        /*Peer reject DISC request,
           Because the peer entity is already in a disconnected mode 
           So ,after N2 retry, Directly close.*/

        if (dlci == MUX_CTRL_CHAN)
        {
            /*DISC DLC0 means CLOSE MUX*/
            MUX_Upon_Close(mux_ptr);
        }
        else
        {
            MUX_FlushDlci(link_id);
        }
    }

    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    this function is used to close a dlc channel
// Global resource dependence :
// Author :         yayan.xu
// DATE :         2008.7.29
// Note :     Only Used by TE side
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_Close_Link(uint8 link_id)
{
    MUX_RETURN_E    ret;
    
    MUX_INFO_T *mux_ptr;
    MUX_ASSERT(g_mux_dlc_info[link_id].from < MAX_MUX_NUM);/*assert verified*/
    
    mux_ptr = &g_mux_sys_info[g_mux_dlc_info[link_id].from];
    
    if (MUX_STATUS_OFF == mux_ptr->is_start)
    {
        /*MUX is NOT startup*/
        //MUX_TRACE_WARNING:"MUX_Close_Link fail due to NOT Startup"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1611_112_2_18_1_49_21_86,(uint8*)"");
        return MUX_RET_NOT_STARTUP;
    }


    /*User CAN NOT call api to release DLC0/Link0*/
    if (MUX_CTRL_CHAN == g_mux_dlc_info[link_id].dlci)
    {
        //MUX_TRACE_WARNING:"MUX_Close_Link, NOT allow release DLC0\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1619_112_2_18_1_49_21_87,(uint8*)"");
        return MUX_RET_FAILURE;
    }

    ret = MUX_Close_Dlci(g_mux_dlc_info[link_id].dlci, &g_mux_sys_info[g_mux_dlc_info[link_id].from]);

    return ret;
}

/*****************************************************************************/
// Description :    this function is used to close MUx service
// Global resource dependence :
// Author :         yayan.xu
// DATE :         2008.7.29
// Note :     Only Used by TE side; TE side only support ONE MUX entry
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_Exit(void)
{
    MUX_RETURN_E    ret;

    MUX_INFO_T *mux_ptr = &g_mux_sys_info[MUX_1];//Temp set, avoid interface modify

    if (MUX_STATUS_OFF == mux_ptr->is_start)
    {
        /*MUX is NOT startup*/
        //MUX_TRACE_WARNING:"MUX_Exit fail due to NOT Startup"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1642_112_2_18_1_49_22_88,(uint8*)"");
        return MUX_RET_NOT_STARTUP;
    }
    /*Close DLC0 means Close MUX service*/
    ret = MUX_Close_Dlci(MUX_CTRL_CHAN, mux_ptr);
    
    /*delete event ptr.*/
    if (NULL != mux_ptr->g_MuxEvent_ptr)
    {
        MUX_DELETE_EVENT(mux_ptr->g_MuxEvent_ptr);
        mux_ptr->g_MuxEvent_ptr = NULL;
    }
	
#if 0	//MS00148955 create mutex when mux init, so delete when mux task delete.
   //MS00138417 clear mux_send_mutex before MUX exit
    if (NULL != mux_send_mutex)
    {
        MUX_DELETE_MUTEX(mux_send_mutex);
        mux_send_mutex = NULL;
    }
#endif

    return ret;
}

/*****************************************************************************/
// Description :    this function is used to open a dlc channel
// Global resource dependence :
// Author :         from amoi
// DATE :         2007.7.25
// Note :           
/*****************************************************************************/
LOCAL MUX_RETURN_E MUX_Open_Dlci(MUX_PN_DATA_T *sys_param, uint8 dlci, uint8 link_id, MUX_INFO_T *mux_ptr)
{
    uint8           retry_times;
    MUX_RETURN_E    retval      = MUX_RET_FAILURE;
    uint8           dlci_state;
    BOOLEAN         is_overtime;
    uint32          mux_timeout;
    BOOLEAN         is_need_pn  = TRUE;
    uint8           ctrl_link_id;

    if (link_id >= MUX_MAX_LINK_NUM)
    {
        //MUX_TRACE_WARNING:"MUX_Open_Dlci  DLCI:%d  NOT Exist!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1684_112_2_18_1_49_22_89,(uint8*)"d", dlci);
        return MUX_RET_FAILURE;
    }

    if (dlci != MUX_CTRL_CHAN)
    {
        /*MUX CTRL CHAN DLC0 is NOT Exist*/
        if (MUX_DLC_LINK_INVALID == (ctrl_link_id = MUX_Get_Linkid(MUX_CTRL_CHAN, mux_ptr->mux_index)))
        {
            //MUX_TRACE_WARNING:"MUX_Open_Dlci(dlci %d, link %d)  DLCI0  NOT Exist!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1693_112_2_18_1_49_22_90,(uint8*)"dd", dlci, link_id);
            return MUX_RET_FAILURE;
        }
        if ((g_mux_dlc_info[ctrl_link_id].used_state != MUX_DLC_STATE_CONNECT) && (g_mux_dlc_info[ctrl_link_id].used_state != MUX_DLC_STATE_FLOW_STOPPED))
        {
            //MUX_TRACE_WARNING:"MUX_Open_Dlci(dlci %d, link %d) due to link_status error of DLCI0!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1698_112_2_18_1_49_22_91,(uint8*)"dd", dlci, link_id);
            return MUX_RET_FAILURE;
        }
    }

    //MUX_TRACE_WARNING:"MUX_Open_Dlci: DLCI:%d(link %d) Start!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1703_112_2_18_1_49_22_92,(uint8*)"dd", dlci, link_id);

    if (PNULL == sys_param)
    {
        /*adopt default parameter,NOT NEED PN procedure*/
        is_need_pn = FALSE;
    }

    /*Stroe DLCI value for new opening link*/
    g_mux_dlc_info[link_id].dlci = dlci;
    g_mux_dlc_info[link_id].dlci = mux_ptr->mux_index;
    dlci_state = g_mux_dlc_info[link_id].used_state;


    switch (dlci_state)
    {
        case MUX_DLC_STATE_CONNECT:
        case MUX_DLC_STATE_FLOW_STOPPED:
            retval = MUX_RET_SUCCESS;
            break;

        case MUX_DLC_STATE_CONNECTING:
        case MUX_DLC_STATE_NEGOTIATING:
            {
                /*N2: re-transmit times*/
                retry_times = mux_ptr->sys_parameter.N2;
                /*T2: reponse timer for the MUX control channel*/
                mux_timeout = MAX(MUX_TIME_OUT, 10 * mux_ptr->sys_parameter.T2);

                while (retry_times--)
                {
                    /*Adopt Event way to wait RESPONSE from peer.*/

                    is_overtime = MUX_Res_Wait(g_mux_dlc_info[link_id].open_wait_event, mux_timeout,mux_ptr);
                    if ((MUX_DLC_STATE_CONNECT == g_mux_dlc_info[link_id].used_state) || (MUX_DLC_STATE_FLOW_STOPPED == g_mux_dlc_info[link_id].used_state))
                    {
                        retval = MUX_RET_SUCCESS;
                        break;
                    }
                    else if ((MUX_DLC_STATE_NEGOTIATING == g_mux_dlc_info[link_id].used_state) || (MUX_DLC_STATE_CONNECTING == g_mux_dlc_info[link_id].used_state) || !is_overtime)
                    {
                        continue;
                    }
                    else
                    {
                        retval = MUX_RET_FAILURE;
                        break;
                    }
                }

                if ((MUX_DLC_STATE_NEGOTIATING == g_mux_dlc_info[link_id].used_state) || (MUX_DLC_STATE_CONNECTING == g_mux_dlc_info[link_id].used_state) || (MUX_DLC_STATE_REJECTED == g_mux_dlc_info[link_id].used_state))
                {
                    g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_DISCONNECT;
                    retval = MUX_RET_FAILURE;
                }
                break;
            }
        case MUX_DLC_STATE_DISCONNECT:
        case MUX_DLC_STATE_REJECTED:
            {
                // other channel
                g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_NEGOTIATING;

                /*DLC0 NOT need PN procedure*/
                if ((dlci == MUX_CTRL_CHAN))
                {
                    g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_CONNECTING;
                }
                else if (FALSE == is_need_pn)
                {
                    /*sys_param is NULL, NOT need PN procedure*/
                    g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_CONNECTING;
                }
                else
                {
                    /*N2: re-transmit times*/
                    retry_times = mux_ptr->sys_parameter.N2;
                    /*T2: reponse timer for the MUX control channel*/
                    mux_timeout = MAX(MUX_TIME_OUT, 10 * mux_ptr->sys_parameter.T2);

                    while (retry_times--)
                    {
                        MUX_Send_MCC_PN(sys_param, dlci, MUX_MCC_CMD, mux_ptr);
                        is_overtime = MUX_Res_Wait(g_mux_dlc_info[link_id].open_wait_event, mux_timeout, mux_ptr);
                        if (MUX_DLC_STATE_CONNECTING == g_mux_dlc_info[link_id].used_state)
                        {
                            /*Negotiate success*/
                            break;
                        }
                        else if (!is_overtime)
                        {
                            //MUX_TRACE_WARNING:"MUX_Open_Dlci: MUX DLCI:%d Send MUX_PN_MSG timeout!\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1794_112_2_18_1_49_22_93,(uint8*)"d", dlci);
                            continue;
                        }
                    }
                }
                /*Next step: SABM establish DLC*/
                if (MUX_DLC_STATE_CONNECTING == g_mux_dlc_info[link_id].used_state)
                {
                    /*N2: re-transmit times*/
                    retry_times = mux_ptr->sys_parameter.N2;
                    /*T2: reponse timer for the MUX control channel*/
                    mux_timeout = MAX(MUX_TIME_OUT, 10 * mux_ptr->sys_parameter.T2);

                    while (retry_times--)
                    {
                        MUX_Send_SABM(mux_ptr, dlci);
                        is_overtime = MUX_Res_Wait(g_mux_dlc_info[link_id].open_wait_event, mux_timeout, mux_ptr);
                        if ((MUX_DLC_STATE_CONNECT == g_mux_dlc_info[link_id].used_state) || (MUX_DLC_STATE_FLOW_STOPPED == g_mux_dlc_info[link_id].used_state))
                        {
                            retval = MUX_RET_SUCCESS;
                            break;
                        }
                        else if (MUX_DLC_STATE_REJECTED == g_mux_dlc_info[link_id].used_state)
                        {
                            //MUX_TRACE_WARNING:"MUX DLCI:%d Send SABM got rejected!\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1818_112_2_18_1_49_22_94,(uint8*)"d", dlci);
                            retval = MUX_RET_FAILURE;
                            break;
                        }
                        else if (!is_overtime)
                        {
                            //MUX_TRACE_WARNING:"MUX DLCI:%d Send SABM timeout!\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1824_112_2_18_1_49_22_95,(uint8*)"d", dlci);
                            continue;
                        }
                    }
                }

                /*finally, establish failure*/
                if ((MUX_DLC_STATE_NEGOTIATING == g_mux_dlc_info[link_id].used_state) || (MUX_DLC_STATE_CONNECTING == g_mux_dlc_info[link_id].used_state) || (MUX_DLC_STATE_REJECTED == g_mux_dlc_info[link_id].used_state))
                {
                    g_mux_dlc_info[link_id].used_state = MUX_DLC_STATE_DISCONNECT;
                    retval = MUX_RET_FAILURE;
                }
                break;
            }   
        default :
            retval = MUX_RET_FAILURE;
            MUX_TRACE_LOW(("MUX DLCI:%d state is invalid!\n", dlci));
            break;
    }

    MUX_TRACE_LOW(("Mux_Open_Channel DLCI:%d End!\n", dlci));

    return retval;
}
/*****************************************************************************/
// Description :    this function is used to open a mux dlc
// Global resource dependence :
// Author :         yayan.xu
// DATE :         2008.7.29
// Note :     Only Used by TE side
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_Open_Link(MUX_PN_DATA_T *sys_param, uint8 *link_id, MUX_CALLBACK recv_func)
{
    MUX_RETURN_E    ret;
    uint8           new_link_id;
    uint8           loop;
    uint8           tmp_dlci;

    MUX_INFO_T *mux_ptr = &g_mux_sys_info[MUX_1];//Temp set, avoid interface modify

    if (MUX_STATUS_ON != mux_ptr->is_start)
    {
        /*MUX is NOT startup*/
        //MUX_TRACE_WARNING:"MUX: MUX_Open_Link fail due to NOT Startup"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1865_112_2_18_1_49_22_96,(uint8*)"");
        return MUX_RET_NOT_STARTUP;
    }

    /*if there have a link is in establish procedure, MUX has to reject new establish link request*/

    for (loop = 0; loop < MUX_MAX_LINK_NUM; loop++)
    {
        if (MUX_DLC_STATE_CONNECTING == g_mux_dlc_info[loop].used_state)
        {
            //MUX_TRACE_WARNING:"MUX: MUX_Open_Link fail due to there have a link %d is in establishing process\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1875_112_2_18_1_49_22_97,(uint8*)"d",loop);
            return MUX_RET_FAILURE;
        }
    }

    if (MUX_RET_SUCCESS == MUX_Get_NewLinkid(&new_link_id, mux_ptr->mux_index))
    {
        *link_id = new_link_id;
    }
    else
    {
        //MUX_TRACE_WARNING:"MUX: MUX_Open_Link fail due to that there can NOT allocate new link id\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1886_112_2_18_1_49_22_98,(uint8*)"");
        return MUX_RET_FAILURE;
    }


    /*Allocate DLCI for new link id*/
    tmp_dlci = MUX_ALLOC_DLCI;


    ret = MUX_Open_Dlci(sys_param, tmp_dlci, new_link_id, mux_ptr);

    if (MUX_RET_SUCCESS == ret)
    {
        g_mux_dlc_info[new_link_id].common_recv_func = recv_func;
        MUX_TRACE_LOW(("MUX: MUX_Open_Link SUECCESS, link_id = %d, DLCI= %d\r\n", new_link_id, g_mux_dlc_info[new_link_id].dlci));
    }
    else
    {
        /*clear link info*/
        MUX_FlushDlci(new_link_id);
        //MUX_TRACE_WARNING:"MUX: MUX_Open_Link Failure, link_id = %d, DLCI= %d\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1906_112_2_18_1_49_22_99,(uint8*)"dd", new_link_id, g_mux_dlc_info[new_link_id].dlci);
    }

    return ret;
}
/*****************************************************************************/
// Description :    MUX enter Close status, clear related to resource.
// Global resource dependence : Recv DISC DLC0; Recv UA for DSIC DLC0
// Author :       yayan.xu
// DATE :         2008.07.15
// Note :    Modify form Mux_upon_Disconnect (Amoi)
/*****************************************************************************/
PUBLIC MUX_RETURN_E MUX_Upon_Close(MUX_INFO_T *mux_ptr)
{
    uint8   j, loop_start, loop_end;
    if(MUX_STATUS_OFF == mux_ptr->is_start)
    {
         //MUX_TRACE_WARNING:"MUX_Upon_Close: MUX_%d not startup, directly return!"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_1921_112_2_18_1_49_22_100,(uint8*)"d", mux_ptr->mux_index);
         return MUX_RET_SUCCESS;
    }

	

    /*Clear Sys parameter*/
    mux_ptr->is_start = MUX_STATUS_OFF;
    mux_ptr->TE_wait_OK = FALSE;	 //MS00145432 

    MUX_MEMCPY(&(mux_ptr->sys_parameter), &g_default_sys_para, sizeof(MUX_SYS_PARA_T));


    if(MUX_2 == mux_ptr->mux_index)
    {
        loop_start = MUX_COM1_LINK_NUM;
        loop_end  = MUX_MAX_LINK_NUM;
    }
    else
    {
         loop_start = 0;
         loop_end  = MUX_COM1_LINK_NUM;
    }
    


    for (j = loop_start; j < loop_end; j++)
    {
        /*I don't know why need call MUX_Wait_Wakeup. */
        //MUX_Wait_WakeUp(g_mux_dlc_info[j].open_wait_event, mux_ptr);
        //MUX_Wait_WakeUp(g_mux_dlc_info[j].close_wait_event, mux_ptr);
#ifdef _MUX_MODEM_
        if (MUX_DLC_STATE_DISCONNECT != g_mux_dlc_info[j].used_state)
        {
            /*Notify ATC link release*/
            ATC_MUX_LINK_RELEASE_REQ_T *sig_ptr = PNULL;    

            MUX_CREATE_SIGNAL(*(xSignalHeader*) &sig_ptr, ATC_MUX_LINK_RELEASE_REQ, \
                                                    sizeof(ATC_MUX_LINK_RELEASE_REQ_T), MUX_IdentifyThread());

            sig_ptr->link_id = j;
            MUX_SEND_SIGNAL((xSignalHeader) sig_ptr, P_ATC);
        }
#endif/*_MUX_MODEM*/
        MUX_ResetDlci(j);
    }


    /*Notify ATC MUX CLOSE DOWN*/
#ifdef _MUX_MODEM_
    {
        ATC_MUX_CLOSE_REQ_T    *sig_ptr = PNULL;

        MUX_CREATE_SIGNAL(*(xSignalHeader*) &sig_ptr, ATC_MUX_CLOSE_REQ, \
                                                    sizeof(ATC_MUX_CLOSE_REQ_T), MUX_IdentifyThread());
        MUX_SEND_SIGNAL((xSignalHeader) sig_ptr, P_ATC);
    }
#endif/*#ifdef _MUX_MODEM_*/

#if 0     //MS00148955 

   //MS00138417 clear mux_send_mutex before MUX exit
    if (NULL != mux_send_mutex)
    {
        MUX_DELETE_MUTEX(mux_send_mutex);
        mux_send_mutex = NULL;
    }
#endif

     //MS00145432, exit mux mode, release mux_rev_buf
     if(mux_ptr->mux_rev_buf != NULL)
     {
	      MUX_FREE(mux_ptr->mux_rev_buf);
	      mux_ptr->mux_rev_buf = NULL;
     }
      //Return serial mode, only use control channel
       g_mux_dlc_info[mux_ptr->ctl_link].from = mux_ptr->mux_index;
    MUX_TRACE_LOW(("MUX_Upon_Close: Clear tx buffer in %d!\r",mux_ptr->mux_index));    
		
    MUX_Ring_Clear(&mux_port[mux_ptr->mux_index].tx_buf);


    return MUX_RET_SUCCESS;
}

/*****************************************************************************/
// Description :    this function is used to parse result code of AT+CMUX command.
// Global resource dependence :
// Author :         yayan.xu
// DATE :         2007.8.28
// Note :           
/*****************************************************************************/
MUX_RETURN_E MUX_Parse_Atcmd_Res(uint32 len, uint8 *data_ptr, MUX_INFO_T *mux_ptr)
{
    uint32          tmp_len ;
    uint8          *tmp_data_ptr;

    uint8           cmp_ok1[3]  = "3\r";
    uint8           cmp_ok2[4]  = "OK\r";
    uint8           cmp_ok3[7]  = "\r\nOK\r\n";


    uint8           findCount1  = 0;
    uint8           findCount2  = 0;
    uint8           findCount3  = 0;

    MUX_RETURN_E    ret         = MUX_RET_FAILURE;
    tmp_data_ptr = data_ptr;
    tmp_len = len;

    /* If it is possible that we got  'OK\r' or "\r\nOK\r\n", 3\r*/
    /*If response message include "OK\r", We think Receive successful response*/ 
    while (tmp_len--)
    {
        if (*tmp_data_ptr == cmp_ok1[findCount1])
        {
            findCount1++;
            if (findCount1 >= 2)/*find 3\r*/
            {
                // all characters found, signal with event
                MUX_TRACE_LOW(("MUX: Receive OK(1)\r\n"));
                mux_ptr->cmdres_code = MUX_ATPlusCMUX_RET_SUCCESS;
                MUX_Wait_WakeUp(mux_ptr->cmdres_wait_flag, mux_ptr);
                ret = MUX_RET_SUCCESS;
                break;
            }
        }
        else if (*tmp_data_ptr == cmp_ok2[findCount2])
        {
            findCount2++;
            if (findCount2 >= 3)/*find OK\r*/
            {
                // all characters found, signal with event
                MUX_TRACE_LOW(("MUX: Receive OK(2)\r\n"));
                mux_ptr->cmdres_code = MUX_ATPlusCMUX_RET_SUCCESS;
                MUX_Wait_WakeUp(mux_ptr->cmdres_wait_flag, mux_ptr);
                ret = MUX_RET_SUCCESS;                
                break;
            }
        }
        else if (*tmp_data_ptr == cmp_ok3[findCount3])
        {
            findCount3++;
            if (findCount3 >= 6)/*find \r\nOK\r\n*/
            {
                // all characters found, signal with event
                MUX_TRACE_LOW(("MUX: Receive OK(3)\r\n"));
                mux_ptr->cmdres_code = MUX_ATPlusCMUX_RET_SUCCESS;
                MUX_Wait_WakeUp(mux_ptr->cmdres_wait_flag, mux_ptr);
                ret = MUX_RET_SUCCESS;                
                break;
            }
        }
        else
        {
            // reset compare pointer and counter
            findCount1 = 0;
            findCount2 = 0;
            findCount3 = 0;
#if 0 /*yayan.xu mod@20081008*/
            // all characters found, signal with event
            MUX_TRACE_LOW(("MUX: NOT receive OK, return failure\r\n"));
            g_mux_sys_info.cmdres_code = MUX_ATPlusCMUX_RET_FAIL;
            MUX_Wait_WakeUp(g_mux_sys_info.cmdres_wait_flag, mux_ptr);
            ret = MUX_RET_FAILURE;
#endif/*if 0*/            
            break;
        }
        tmp_data_ptr++;
    }
    /*if the string don't include OK, return failure*/
    if(MUX_RET_FAILURE == ret)
    {/*return error for AT+CMUX command*/
            //MUX_TRACE_WARNING:"MUX: NOT receive OK, return failure\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MUX_CTL_2092_112_2_18_1_49_22_101,(uint8*)"");
            mux_ptr->cmdres_code = MUX_ATPlusCMUX_RET_FAIL;
            MUX_Wait_WakeUp(mux_ptr->cmdres_wait_flag, mux_ptr);
            ret = MUX_RET_FAILURE;
    }
    return ret;
}
uint8  MUX_SNC_Get_Supported_Services()
{
    return MUX_DLC_SERVICE_DATA | MUX_DLC_SERVICE_VOICE;
}

uint8  MUX_SNC_Get_Supported_Codecs()
{
    return MUX_DLC_CODEC_PCM64U | MUX_DLC_CODEC_PCM64A | MUX_DLC_CODEC_PCM128;
}

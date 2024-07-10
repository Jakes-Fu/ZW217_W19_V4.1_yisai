#ifdef FOTA_SUPPORT_ABUP

#include "abup_typedef.h"
#include "abup_debug.h"
#include "abup_net.h"
#include "ms_ref_base_trc.h"
#include "os_api.h"
#include "in_message.h"
#include "abup_thread.h"
#include "mn_type.h"
#include "mn_error.h"
#include "atc_plus_gprs.h"
#include "atc.h"
#include "dal_power.h"
#include "mmk_app.h"
#include "socket_types.h"
#include "mmi_module.h"
#include "priority_system.h"

#define ABUP_TASK_STACK_SIZE 	(20*1024)
#define ABUP_TASK_QUEUE_NUM 	50

abup_uint32 s_abup_task_id=-1;
static ABUP_BOOL s_abup_sim_ready=ABUP_FALSE;

static abup_uint8 abup_task_entry(xSignalHeaderRec *sig_ptr);
abup_uint32 abup_task_id(void);
static void abup_init_nv(void);
static void abup_task_main (abup_uint32 argc, void *argv);
void abup_register_event(void);
extern void abup_upgrade_thread_on(void);
extern void abup_get_new_version(void);

BLOCK_ID abup_task_create(void)
{
	BLOCK_ID task_id=0;
	
    abup_DebugPrint("abup_task_create--->");
	
    if(task_id == 0)
    {
		task_id = SCI_CreateThread(
			"ABUP",
			"ABUP_QUEUE",
			abup_task_main,
			0,
			0,
			ABUP_TASK_STACK_SIZE,
			ABUP_TASK_QUEUE_NUM,
			PRI_APP_TASK,     /*lint !e506*/       
			SCI_PREEMPT, 
			SCI_AUTO_START
			);
		abup_DebugPrint("abup_task_create: task_id = 0x%x", task_id);
    }

	abup_DebugSwitch(1);

    return task_id;
}

static void abup_task_main (abup_uint32 argc, void *argv)
{
	xSignalHeaderRec *sig_ptr = 0;
	abup_uint32 this_thread_id = SCI_IdentifyThread();
	abup_uint8  bFlags = 1;
	abup_socint soc_id=0;

	s_abup_task_id = this_thread_id;
	abup_DebugPrint("abup_task_main: task_id = 0x%x", s_abup_task_id);

	if(s_abup_sim_ready == ABUP_FALSE)
	{
		s_abup_sim_ready = ABUP_TRUE;
		//abup_register_event();
	}

	while(1)
	{
		sig_ptr = SCI_GetSignal(this_thread_id);
		abup_DebugPrint("%s,line=%d",__FUNCTION__,__LINE__);
		bFlags = abup_task_entry(sig_ptr);
		abup_memfree(sig_ptr);
		if(bFlags == 0)
		{
			break;
		}
	}
	//SCI_ThreadExit();
}

static abup_uint8 abup_task_entry(xSignalHeaderRec *sig_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	abup_uint8  bFlags = 1;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	if(sig_ptr)
		abup_DebugPrint("abup_task_entry: SignalCode: %d", (abup_uint32)(sig_ptr->SignalCode));
	
	switch(sig_ptr->SignalCode)
	{
		case SOCKET_READ_EVENT_IND:
		case SOCKET_WRITE_EVENT_IND:
		case SOCKET_CONNECT_EVENT_IND:
		case SOCKET_ACCEPT_EVENT_IND:
		case SOCKET_CONNECTION_CLOSE_EVENT_IND:
		{
			abup_DebugPrint("abup: SOCKET EVENT_IND: CONNECT/READ/WRITE/CLOSE");
			abup_soc_hdlr((void *)sig_ptr);
			break;
		}

		case SOCKET_ASYNC_GETHOSTBYNAME_CNF:
		{
			//handle socket related signal
			abup_DebugPrint("abup: SOCKET_ASYNC_GETHOSTBYNAME_CNF");
			abup_GetHostByName_Notify((void *)sig_ptr);
			break;
		}
		
        // the following signals for GPRS services
        case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
		{
            abup_DebugPrint("abup_Task: Signal Code: APP_MN_ACTIVATE_PDP_CONTEXT_CNF");
			abup_pdp_actpdpcontext_cnf((APP_MN_GPRS_EXT_T *)sig_ptr);	
			break;
        }
        case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF:
            abup_DebugPrint("abup_Task: Signal Code: APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, result:%d,pdp_num:%d,cid:%d, set atc_err_protected_flag",
                          ((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->result,
                          ((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->pdp_num,
                          ((APP_MN_GPRS_DEACTIVATE_CNF_T *)sig_ptr)->pdp_id[0]);	             
			break;

		case ATC_ABUP_INIT_MSG_IND:
		{
			AT_ABUP_MSG_T *sig = (AT_ABUP_MSG_T *)sig_ptr;
			abup_DebugPrint("abup: APP_MN_SIM_READY_IND & ATC_abup_INIT_MSG_IND");
			if(s_abup_sim_ready == ABUP_FALSE)
			{
				s_abup_sim_ready = ABUP_TRUE;
				abup_register_event();
				//abup_boot_hal_init();
			}
			break;
		}
			
		case ATC_ABUP_AT_MSG_IND:
		{
			AT_ABUP_MSG_T *sig = (AT_ABUP_MSG_T *)sig_ptr;
			abup_DebugPrint("abup: ATC_ABUP_AT_MSG_IND,cmd:%s,p0:%d,p1:%d,p2:%d",\
				sig->cmd, sig->param0, sig->param1,sig->param2);
			abup_fota_atcmd(sig->cmd, sig->param0, sig->param1,sig->param2, NULL);
			break;
		}

        case APP_MN_SCELL_RSSI_IND:
		{
			APP_MN_SCELL_RSSI_IND_T *sig = (APP_MN_SCELL_RSSI_IND_T *)sig_ptr;
			abup_DebugPrint("abup: APP_MN_SCELL_RSSI_IND, rxlev:%d", sig->rxlev);
			abup_net_cell_set_rxlev(sig->rxlev);
            break;
        }	
		
		case 0xFFF0:
			bFlags = 0;
			break;
		
		default:
			//SCI_TRACE_LOW:"DEFAULT:MWIN TASK ENTRY APP, EVENT:%d"
			//SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_SOCKET_709_112_2_18_3_10_59_193,(uint8*)"d",sig_ptr->SignalCode);
			break;
	}

	return bFlags;
}

void abup_register_event(void)
{
    POWER_RESTART_CONDITION_E start_condition = POWER_GetRestartCondition();

	abup_DebugPrint("abup:abup_register_event b");
//    if ((RESTART_BY_ALARM != start_condition &&RESTART_BY_CHARGE != start_condition)
//        || 1 == CLASSMARK_GetModelType() )
    {
        //Register MN GPRS event
        abup_DebugPrint("abup:abup_register_event reg");
        SCI_RegisterMsg(MN_APP_GPRS_SERVICE,
                                (abup_uint8)EV_MN_APP_SET_PDP_CONTEXT_CNF_F,
                                (abup_uint8)(MAX_MN_APP_GPRS_EVENTS_NUM - 1),
                                SCI_NULL); 
		SCI_RegisterMsg( MN_APP_PHONE_SERVICE, 
								(abup_uint8)EV_MN_APP_SCELL_RSSI_IND_F, 
								(abup_uint8)(EV_MN_APP_SCELL_RSSI_IND_F+1), 
								SCI_NULL );
	}
	abup_DebugPrint("abup:abup_register_event e");
}

abup_uint32 abup_task_id(void)
{
	return (abup_uint32)s_abup_task_id;
}

void abup_task_uninit(void)
{
	abup_DebugPrint("abup_task_uninit: task_id = 0x%x", s_abup_task_id);
	if (s_abup_task_id != -1)
	{
		MMIAPIPDP_Deactive(MMI_MODULE_COMMON);
		SCI_Sleep(1000);
		SCI_TerminateThread(s_abup_task_id);
		SCI_DeleteThread(s_abup_task_id);
		s_abup_task_id = -1;
	}
}

void abup_send_newat_msg(abup_char *cmd, abup_uint32 p0, abup_uint32 p1, abup_uint32 p2, abup_write func)
{
	MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    AT_ABUP_MSG_T *signal_ptr = NULL;
	abup_uint32 abup_taskid = abup_task_id();

    SCI_CREATE_SIGNAL(signal_ptr, ATC_ABUP_AT_MSG_IND,
                      sizeof(AT_ABUP_MSG_T), abup_taskid);//P_ATC

	if(signal_ptr)
	{
	    signal_ptr->dual_sys = dual_sys;//(MN_DUAL_SYS_E)(lparam>>4);
	    signal_ptr->cmd = cmd;
	    signal_ptr->param0 = p0;
	    signal_ptr->param1 = p1;
	    signal_ptr->param2 = p2;

		abup_DebugPrint("abup: abup_send_newat_msg,cmd:%s,p0:%d,p1:%d,p2:%d",\
			signal_ptr->cmd, signal_ptr->param0, signal_ptr->param1,signal_ptr->param2);

	    SCI_SEND_SIGNAL(signal_ptr, abup_taskid);//P_ATC
	}
}

void abup_send_socket_msg(void)
{	
    xSignalHeader   signal;
	abup_uint32 abup_taskid = abup_task_id();

    SCI_CREATE_SIGNAL(signal,SOCKET_CONNECT_EVENT_IND,sizeof(*signal),abup_taskid);
	
	abup_DebugPrint("abup_send_socket_msg---->taskid:%d",abup_taskid);
    SCI_SEND_SIGNAL(signal,abup_taskid);
}

void abup_upgrade_thread_on_ex(void)
{
	abup_stop_timer(abup_upgrade_thread_on_ex);
	if (s_abup_task_id != -1)
	{
		abup_DebugPrint("abup_upgrade_thread_on---->task is running!");
		return;
	}
	abup_task_create();
	abup_start_timer(3*1000, abup_upgrade_thread_on, ABUP_FALSE);
}

void abup_get_new_version_ex(void)
{
	abup_stop_timer(abup_get_new_version_ex);
	if (s_abup_task_id != -1)
	{
		abup_DebugPrint("abup_get_new_version---->task is running!");
		return;
	}
	abup_task_create();
	abup_start_timer(3*1000, abup_get_new_version, ABUP_FALSE);
}
#endif


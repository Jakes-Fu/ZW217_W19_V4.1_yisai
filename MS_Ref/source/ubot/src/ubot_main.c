/******************************************************************************
 ** File Name:    UMSC_SCSI_main.c                                            *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file is USB mass storage protocol main body ;          *
 **               This lib uses SCSI instructs set and loop inquire interrupt
 **                mode                                                       *
 **          Usb mass storage Scsi instructs set and Loop inquire mode A lib  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_ubot_trc.h"
#include "gadget.h"
#include "usb_ch9.h"
#include "udc_api.h"
#include "ubot_common.h"
#include "ubot_main.h"
#include "ubot_api.h"
#include "ubot_config.h"
#include "bsd.h"
#include "sci_log.h"
#include "run_mode.h"
#include "usb_drv.h"
#include "ubot_drv.h"
#include "ubot_storage.h"
#include "ref_param.h"
#include "usbservice_api.h"
#include "sc_reg.h"
#include "usb_device.h"
#include "priority_system.h"

#ifdef UDISK_PC_CONNECT
#include "umem_main.h"
#endif
#ifdef USB_PROTOCOL_SUPPORT_USB20
#include "usb20_app_config.h"
#endif
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define UBOT_STACK_SIZE                 0x1000
#define UBOT_QUEUE_NUM                  0x0010


#undef SCI_TRACE_LOW
#define SCI_TRACE_LOW( x )  RM_SendRes(x,strlen(x))
#undef SCI_TRACE_LOW
#define SCI_TRACE_LOW( x )

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
//static const char longname[] = "File-backed Storage Gadget";

/*--------------------------- Local Data ------------------------------------*/
BLOCK_ID        ubot_main_id = 0;

uint32 ubot_interface=0xFFFFFFFF;
uint8 ubot_task_execute_flag = 0;
LOCAL uint32 g_usb_interface = 1; 
LOCAL uint32 g_usb_configuration = 1;
/*--------------------------- Global Data -----------------------------------*/
//struct usb_request    *ep0req;

/*--------------------------- External Data ---------------------------------*/
extern uint32 UBOT_GetCOnfigDescriptor(uint8 ** para);
extern const UBOT_FUNCTION g_UBOT_function[];

LOCAL SCI_SEMAPHORE_PTR s_ubot_sem = PNULL;

/*----------------------------------------------------------------------------*
**                         External Function Definitions                      *
**---------------------------------------------------------------------------*/
//extern void SCI_InitLogSwitch(BOOLEAN);
//extern void USB_SetFreq(uint32);
//extern void RM_SendRes(uint8*, int32);

/*----------------------------------------------------------------------------*
**                         Local Function Definitions                         *
**---------------------------------------------------------------------------*/
LOCAL void UBOT_main (uint32, void *);

/*****************************************************************************/
//  Description:    Function is called when endpoint1 transfer is complete
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL void UBOT_Send_Complete (uint8 *buffer,uint32 length)
{
    g_UBOT_manager.cache.lock = UBOT_READY_LOCK;
}

/*****************************************************************************/
//  Description:    Function is called when endpoint2 transfer is complete
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL void UBOT_Receive_Complete (uint8 *buffer,uint32 length)
{
    xSignalHeader   signalout;
    //  SCI_TRACE_LOW("\nUBOT_Ep2_Complete() enter\n");

    g_UBOT_manager.cache.num += length;
    g_UBOT_manager.cache.lock = UBOT_READY_LOCK;

    /*trigger ubot task to receive cmd end*/
    if (g_UBOT_manager.state == UBOT_STATE_READY)
    {
        SCI_TRACE_LOW("\nUBOT_Ep2_Complete send signal\n");
        signalout = SCI_ALLOC_APP (sizeof (*signalout));
        signalout->SignalCode = UBOT_CMD_EXECUTE;
        signalout->SignalSize = sizeof (*signalout);
        signalout->Sender = SCI_IdentifyThread();
        SCI_SendSignal (signalout, ubot_main_id);
    }
}
LOCAL void  UBOT_Transfer_Complete(uint8 event,uint8 *buffer,uint32 length)
{
	switch(event&0x0F)
	{
		case EVENT_CTRL:
			break;
		case EVENT_SEND:
			UBOT_Send_Complete(buffer,length);
			break;
		case EVENT_RECEIVE:
			UBOT_Receive_Complete(buffer,length);
			break;
		default:
			break;
	}
}
/*****************************************************************************/
//  Description:    Function is used to bind ubot driver to udc driver
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC int ubot_bind (USB_GADGET_T *gadget)
{

    xSignalHeader   signal;
    
    //等前一次的UBOT_main thread退出
    while (ubot_main_id)
    {
        SCI_SLEEP(1);
    }
    
#ifdef WINUSB_SUPPORT
	UBOT_SetStrInfo();
#endif

    UDC_Config();
    {
	  uint32 call_back = (uint32)UBOT_Transfer_Complete;
         //uint8 *para=1;
         //UBOT_GetCOnfigDescriptor(&para);
         //UsbDevice_Create(para,gadget);
	  ubot_interface = UsbDevice_OpenInterface(USB_PROTOCOL_UBOT,call_back,0x03);
    }
    /* Start ubot thread to control read/write/start/stop */
    s_ubot_sem = SCI_CreateSemaphore("ubot_sem", 0);
    ubot_main_id = SCI_CreateAppThread ("T_UBOT_MAIN_HANDLER",
                                        "Q_UBOT",
                                        UBOT_main,
                                        0,
                                        0,
                                        UBOT_STACK_SIZE,
                                        UBOT_QUEUE_NUM,
                                        PRI_UBOT_MAIN,
                                        SCI_PREEMPT,
                                        SCI_AUTO_START);


    signal = SCI_ALLOC_APP (sizeof (*signal));
    signal->SignalCode = UBOT_CMD_PLUG_IN;
    signal->SignalSize = sizeof (*signal);
    signal->Sender = SCI_IdentifyThread();
    SCI_SendSignal (signal, ubot_main_id);
#ifdef USB_PROTOCOL_SUPPORT_USB20
    USB_SetService (USB_APP_UDISK,NULL);
#endif

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    Function is used to unbind ubot driver from udc driver
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void ubot_unbind (USB_GADGET_T *gadget)
{
    xSignalHeader signal;
  
    signal = SCI_ALLOC_APP (sizeof (*signal));
    signal->SignalCode = UBOT_CMD_PLUG_OUT;
    signal->SignalSize = sizeof (*signal);
    signal->Sender = SCI_IdentifyThread();
    SCI_SendSignal (signal, ubot_main_id);

#ifdef USB_PROTOCOL_SUPPORT_USB20
    USB_SetService (USB_APP_NONE,NULL);
#endif

    SCI_GetSemaphore(s_ubot_sem, SCI_WAIT_FOREVER);
}

/*****************************************************************************/
//  Description:    Function to handle device disconnection
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL void ubot_disconnect (USB_GADGET_T *gadget)
{
}

/*****************************************************************************/
//  Description:    Function to run endpoint0 setup request
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL int ubot_setup (USB_GADGET_T *gadget,
                      const USB_CTRLREQUEST_T *ctrl)
{
    int ret = FALSE;
    xSignalHeader	signalout;

    //  SCI_TRACE_LOW("\nubot_setup() enter\n");

    if ( (ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS)            //class request
    {
        //      SCI_TRACE_LOW("\nUSB_CLASS_REQUST\n");
        if ( (ctrl->bRequestType & USB_RECIP_MASK) != USB_RECIP_INTERFACE)
        {
            //SCI_TRACE_LOW:"\nUSB_ERR_INVALID_CLASSCMD\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_MAIN_224_112_2_18_1_54_47_15,(uint8*)"");
            USB_ErrorHandle (USB_ERR_INVALID_CLASSCMD);
            return ret;
        }

        switch (ctrl->bRequest)
        {
            case UBOT_CLASS_GET_MAX_LUN:
                //              SCI_TRACE_LOW("\nUBOT_CLASS_GET_MAX_LUN\n");
                UBOT_GetMaxLUN ( (ctrl->bRequestType&USB_DIR_IN),\
                                 (ctrl->wValue),\
                                 (ctrl->wIndex),\
                                 (ctrl->wLength));
                /*Start process masstorage .*/
		  signalout = SCI_ALLOC_APP(sizeof(*signalout));
		  signalout->SignalCode = UBOT_CMD_EXECUTE;
		  signalout->SignalSize = sizeof(*signalout);
		  signalout->Sender = SCI_IdentifyThread();
		  SCI_SendSignal(signalout, ubot_main_id);
                break;
            case UBOT_CLASS_RESET:
                //              SCI_TRACE_LOW("\nUBOT_CLASS_RESET\n");
                UBOT_HostReset ( (ctrl->bRequestType&USB_DIR_IN),\
                                 (ctrl->wValue),\
                                 (ctrl->wIndex),\
                                 (ctrl->wLength));
                break;
            default :
                USB_ErrorHandle (USB_ERR_INVALID_CLASSCMD);
                break;
        }
    }
    else                                                                    //standard request
    {
        //      SCI_TRACE_LOW("\nUSB_STANDARD_REQUST\n");
        switch (ctrl->bRequestType & USB_RECIP_MASK)                        //Recipient
        {
            case USB_RECIP_DEVICE:
                ret = UBOT_GetDevDescriptor ( (ctrl->bRequestType&USB_DIR_IN),\
                                              (ctrl->wValue),\
                                              (ctrl->wIndex),\
                                              (ctrl->wLength));
                break;
            case USB_RECIP_INTERFACE:
                ret = SCI_ERROR;
                //          SCI_TRACE_LOW("\nubot_setup() IGNORE INTERFACE\n");
                break;
            default :
                ret = SCI_ERROR;
                //          SCI_TRACE_LOW("\nubot_setup() IGNORE\n");
                break;
        }
    }

    //  SCI_TRACE_LOW("\nubot_setup() exit\n");
    return ret;
}

/*****************************************************************************/
//  Description:    Function to handle ubot suspend state
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL void ubot_suspend (USB_GADGET_T *gadget)
{
}

/*****************************************************************************/
//  Description:    Function to handle ubot resume state
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL void ubot_resume (USB_GADGET_T *gadget)
{
}

/*****************************************************************************/
//  Description:    Function to get ubot gadget driver interface
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL const USB_GADGET_DRIVER_T ubot_driver = {
    "UBOT",
#ifdef CONFIG_USB_GADGET_DUALSPEED
    USB_SPEED_HIGH,
#else
    USB_SPEED_FULL,
#endif
    ubot_bind,
    ubot_unbind,
    ubot_setup,
    ubot_disconnect,
    ubot_suspend,
    ubot_resume,
    UBOT_GetCOnfigDescriptor
};

const USB_GADGET_DRIVER_T *UBOT_GetHandler (void)
{
    return &ubot_driver;
}



/*****************************************************************************/
//  Description:    Ubot main thread function
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
LOCAL void UBOT_main (uint32 argc, void *argv)
{
    BSD_STATUS      status;
    int             i=0;
    xSignalHeader   signal;
    BLOCK_ID    tid = SCI_IdentifyThread();
    BOOLEAN is_ubot_main_task_exist = TRUE;
    UBOT_MANAGER_T *umass = &g_UBOT_manager;
    umass->cache.malloc_buf = NULL;

    while (is_ubot_main_task_exist)
    {
        signal = (xSignalHeader) SCI_GetSignal (tid);

        switch (signal->SignalCode)
        {
            case UBOT_CMD_PLUG_IN:
                ubot_config_init();
                UBOT_CustomerConfig();

                umass->state = UBOT_STATE_READY;

                /*BUS power;*/
                UDC_PowerSet (0);

                // malloc memory for ubot applicaton ;
                if (umass->cache.malloc_buf == NULL)
                    umass->cache.malloc_buf = SCI_ALLOC_APP (UBOT_CACHE_SIZE + 32);

                if (umass->cache.malloc_buf != NULL) {
                    umass->cache.buf = (uint8 *)(((uint32)umass->cache.malloc_buf + 31) / 32 * 32);
                    umass->cache.buf_size = UBOT_CACHE_SIZE;
                }

                // SCI_TRACE_LOW("\nubot_bind() ubot config\n");
                UBOT_ResetCache();
                UBOT_ClearCSW();
                UBOT_ClearCBW();

                //SCI_InitLogSwitch (FALSE); /*for upm task mips, especially in 8800h platform, sme task*/

                if (UBOT_MAX_DISK_NUM == g_UBOT_MaxLUN[0]) {
                    //SCI_TRACE_LOW:"UBOT get maxlun error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_MAIN_367_112_2_18_1_54_47_16,(uint8*)"");

                    SCI_SendEventToClient (USB_SERVICE, USB_ERROR, NULL);
                    USB_ErrorHandle (USB_ERR_BSD_OPENDEV);
                }

                USB_SetFreq (USB_FREQ_SET_LVL_HIGH);

                /*Sleep some time for shutdown ffs .*/
                SCI_Sleep (500);

                for (i = 0; i < (g_UBOT_MaxLUN[0] + 1); i++)
                {
                    status = BSD_Open ( (char *) g_UBOT_manager.dev[i].inf.dev_name, 
						BSD_GENERIC_READ | BSD_GENERIC_WRITE, &g_UBOT_manager.dev[i].inf.dev_handle);

                    UBOT_BSD_OPEN_CHECK (status);
                    Ubot_Cache_Init(i,
                        g_UBOT_manager.dev[i].inf.dev_handle,
                        g_UBOT_manager.dev[i].cache_number,
                        g_UBOT_manager.dev[i].page_number,
                        g_UBOT_manager.dev[i].page_size
                        );
                }

                break;

            case UBOT_CMD_PLUG_OUT:
                {
                    BSD_STATUS      status;
                    uint32 i;

                    Ubot_Cache_DeInit();

                    for (i=0; i< (g_UBOT_MaxLUN[0]+1) ; i++)
                    {
                          status = BSD_Close (g_UBOT_manager.dev[i].inf.dev_handle);
                          UBOT_BSD_CLOSE_CHECK (status);
                          g_UBOT_manager.dev[i].inf.dev_handle = NULL ;
                    }
                }
                
                UDC_PowerSet (1); /*Self power;*/
                //SCI_InitLogSwitch (REFPARAM_GetEnableArmLogFlag()); /*mainly for upm task mips*/

                ubot_config_exit();
                is_ubot_main_task_exist = FALSE;

                #ifdef UMEM_SUPPORT
                /*start added by feng tao*/
                if(UMEM_GetUpdateCtrlSecFlag())
                {
                    UMEM_SetUpdateCtrlSecFlag(SCI_FALSE);
                    Umem_UpdateCtrlSec();
                }
                #endif

                if (umass->cache.malloc_buf)
                    SCI_FREE(umass->cache.malloc_buf);
                umass->cache.malloc_buf = NULL;
                umass->cache.buf = NULL;
                umass->cache.buf_size = 0;
                ubot_interface=0xFFFFFFFF;
                SCI_PutSemaphore(s_ubot_sem);

                break;

            case UBOT_CMD_EXECUTE:
                do {
                    umass->state = g_UBOT_function[umass->state] (NULL,NULL);
                } while ( (umass->state != UBOT_STATE_READY) && (TRUE == UDC_GetConnect()));
                break;

            default :
                break;
        }

        SCI_FREE (signal);
    }

    // Empty the signal queue.
    while (1)   /*lint !e716*/
    {
        signal = (void*)SCI_PeekSignal(SCI_IdentifyThread());
        if (signal)
        {
            SCI_FREE(signal);
        }
        else
        {
            break;
        }
    }
    
    ubot_main_id = 0;
    SCI_DeleteSemaphore(s_ubot_sem);
    s_ubot_sem = PNULL;
    SCI_ThreadExit ();
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End


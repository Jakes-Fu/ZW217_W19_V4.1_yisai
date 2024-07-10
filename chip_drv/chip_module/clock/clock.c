/******************************************************************************
 ** File Name:      Clock.c                                                 *
 ** Author:         Yi.Qiu                                                  *
 ** DATE:           02/21/2010                                              *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                            *
 ******************************************************************************

 ******************************************************************************
 **                 Edit History                                            *
 ** ------------------------------------------------------------------------*
 ** DATE            NAME             DESCRIPTION                            *
 ** 02/21/2010      Yi.Qiu          Create.                                 *
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "hash.h"
#include "list.h"
#include "clock.h"
#include "clock_drvapi.h"
#include "uws6121e_reg_analog.h"
#include "adi_hal_internal.h"
#include "global_macros.h"

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
/* Enter critical section */
#define ENTER_CS() \
    if (!_acquire_mutex(g_pMutex)) { \
        CLOCK_ERR_TRACE(("[CLOCK:%s] Get Mutex failed.\r\n")); \
    }

/* Leave critical section */
#define LEAVE_CS() \
    _release_mutex(g_pMutex);

/******************************************************************************
 * Inline functions
 ******************************************************************************/
static __inline BOOLEAN _acquire_mutex (SCI_MUTEX_PTR mutex)
{
    CLOCK_ASSERT (mutex);/*assert verified*/
    return (SCI_SUCCESS == SCI_GetMutex (mutex,
                                         SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0));
}

static __inline void _release_mutex (SCI_MUTEX_PTR mutex)
{
    CLOCK_ASSERT (SCI_SUCCESS == SCI_PutMutex (mutex));/*assert verified*/
}

/******************************************************************************
 * Structure definitions
 ******************************************************************************/
struct _CLOCK_OBJECT
{
    struct _CLOCK   *Clock;             /* This clock */
    CLOCK_HANDLE    SrcClockObj;        /* Pointer to source clock of this clock */

    LIST_HEAD       SiblingClk;         /* List of clocks belongs to a clock source */
    LIST_HEAD       SubClock;           /* List of sub clocks  */
    LIST_HEAD       SubDevice;          /* List of devices attatched to the clock */

    BOOLEAN         bOpened;
};

struct _DEVICE_OBJECT
{
    struct _DEVICE  *Device;            /* This device */
    CLOCK_HANDLE    SrcClockObj;        /* Point to work clock of this device */
    DEVICE_HANDLE   ParentDevObj;       /* Point to parent device */

    LIST_HEAD       SubDevice;          /* List of sub devices */
    LIST_HEAD       SiblingSrc;         /* List of devices belong to a clock source */
    LIST_HEAD       SiblingDev;         /* List of devices belong to a parent device */

    uint32          Div;                /* Divider of work clock source */
    BOOLEAN         bOpened;
};

/***************************************
 * 26M clock type
 * clock id 0       Reserved
 * clock id 1       TSX + 32K
 * clock id 2       TSX + 32Kless
 * clock id 3       DCXO + 32K
 * clock id 4       DCXO + 32Kless
 * clock id 5       TCXO + 32K
 * clock id 6       TCXO + 32Kless
****************************************/
typedef enum
{
    CLOCK_26M_TSX,
    CLOCK_26M_DCXO,
    CLOCK_26M_TCXO,
    CLOCK_26M_UNIDENTIFIED
} CLOCK_26M_TYPE_T;

typedef enum
{
    CLOCK_32K,
    CLOCK_32K_LESS,
} CLOCK_32K_TYPE_T;

/******************************************************************************
 * Static functions prototypes
 ******************************************************************************/
LOCAL CLOCK_STATUS _InitClock (void);
LOCAL CLOCK_STATUS _InitDevice (void);
//LOCAL CLOCK_STATUS _ClockObjectCreate (HASH_TABLE *Hash, CLOCK *pClk);
LOCAL CLOCK_STATUS _OpenClock (CLOCK_HANDLE ClkObj);
LOCAL CLOCK_STATUS _CloseClock (CLOCK_HANDLE ClkObj);
LOCAL CLOCK_STATUS _ClockSetSource (CLOCK_HANDLE thiz, CLOCK_HANDLE pSrcObj);
//LOCAL CLOCK_STATUS _DeviceOpen (DEVICE_HANDLE pDevObj);
LOCAL CLOCK_STATUS _DeviceObjectCreate (HASH_TABLE *Hash, DEVICE *pDev);
LOCAL CLOCK_STATUS _DeviceSetParent (DEVICE_HANDLE thiz, DEVICE_HANDLE pParentObj);

/******************************************************************************
 * Static variables definitions
 ******************************************************************************/
LOCAL SCI_MUTEX_PTR g_pMutex    = SCI_NULL;
LOCAL CLOCK_CFG     g_ClkCfg;
LOCAL const char   *MUTEX_NAME  = "CLOCK MUTEX"; /* Name of the mutex */
LOCAL HASH_TABLE   *g_DevHash;
LOCAL CLOCK_HANDLE  g_ClkObj;

typedef unsigned long int uint32_t;
PUBLIC uint32_t rtc_mode = 0;

/******************************************************************************
 * Implementations
 ******************************************************************************/
PUBLIC void RTC_MODE_init (void)
{
    uint32_t clk32kless_ctrl0 = ANA_REG_GET(ANA_CLK32KLESS_CTRL0);
    rtc_mode = ((clk32kless_ctrl0 >> 4) & 0x00000001); //bit4
}

PUBLIC uint32_t RTC_MODE_get (void)
{
    return rtc_mode;
}

static uint32_t prvGetClock26M(void)
{
#ifdef XO_MODE_TCXO
    return CLOCK_26M_TCXO;
#else
#ifdef XO_MODE_DCXO
    return CLOCK_26M_DCXO;
#else
    if(CLOCK_32K == rtc_mode)
    {
        return CLOCK_26M_TCXO;
    }
    else
    {
        return CLOCK_26M_DCXO;
    }
#endif
#endif
}

PUBLIC uint32_t identifyClockId(void)
{
    uint32_t clock_type_id = 0;

    clock_type_id = ((prvGetClock26M() << 1) | RTC_MODE_get()) + 1;

    return clock_type_id;
}

/*****************************************************************************/
//  Description:    Config all system clocks and devices
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS CLOCK_Config (void)
{
    int i;
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    /* Get information from clock phy */
    CLOCKCFG_Get (&g_ClkCfg);

    /* Open clock source */
    for (i=0; i<g_ClkCfg.ClockNr; i++)
    {
        if (SCI_TRUE == g_ClkCfg.Clock[i].bOpen)
        {
            if (g_ClkCfg.Clock[i].Open != SCI_NULL)
            {
                /* Open clock source*/
                g_ClkCfg.Clock[i].Open();
            }
        }
        else
        {
            if (g_ClkCfg.Clock[i].Close != SCI_NULL)
            {
                /* Close clock source*/
                g_ClkCfg.Clock[i].Close();
            }
        }
    }

    /* Enable device clock */
    for (i=0; i<g_ClkCfg.DeviceNr; i++)
    {
        if (SCI_TRUE == g_ClkCfg.Device[i].bEnable)
        {
            if (g_ClkCfg.Device[i].Open != SCI_NULL)
            {
                /* Enable device clock */
                g_ClkCfg.Device[i].Open();
            }
        }
    }

    return status;
}

/*****************************************************************************/
//  Description:    Init system clocks and devices structre
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS CLOCK_System_Init (void)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    /* create mutex */
    g_pMutex = SCI_CreateMutex (MUTEX_NAME, SCI_INHERIT);
    CLOCK_ASSERT (SCI_NULL != g_pMutex);/*assert verified*/

    _InitClock();

    _InitDevice();

    return status;
}

/*****************************************************************************/
//  Description:    Get device object
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Find (const char *Name, DEVICE_HANDLE *pDevObj)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;
    CLOCK_ASSERT (Name != SCI_NULL);/*assert verified*/
    CLOCK_ASSERT (pDevObj != SCI_NULL);/*assert verified*/

    /* Get device object */
    *pDevObj = HASH_TABLE_Lookup (g_DevHash, Name);

    if (*pDevObj == SCI_NULL)
    {
        status = CLOCK_FAIL;
    }

    return status;
}

/*****************************************************************************/
//  Description:    Get clock object
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS CLOCK_Find (const char *Name, CLOCK_HANDLE *pClkObj)
{
    int i;
    CLOCK_STATUS status     = CLOCK_FAIL;
    CLOCK_ASSERT (Name != SCI_NULL);/*assert verified*/
    CLOCK_ASSERT (pClkObj != SCI_NULL);/*assert verified*/

    *pClkObj = SCI_NULL;

    /* Get clock object */
    for (i=0; i<g_ClkCfg.ClockNr; i++)
    {
        if (0 == strncmp (g_ClkCfg.Clock[i].Name, Name, strlen (Name)))/*lint !e718 !e746 -e718 -e746*/
        {
            *pClkObj = &g_ClkObj[i];
            status = CLOCK_SUCCESS;
            break;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description:    Get clock object
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS CLOCK_FindByValue (uint32 ClockValue, CLOCK_HANDLE *pClkObj)
{
    int i;
    CLOCK_STATUS status     = CLOCK_FAIL;
    CLOCK_ASSERT (pClkObj != SCI_NULL);/*assert verified*/

    *pClkObj = SCI_NULL;

    /* Get clock object */
    for (i=0; i<g_ClkCfg.ClockNr; i++)
    {
        if (ClockValue == g_ClkCfg.Clock[i].ClockValue)
        {
            *pClkObj = &g_ClkObj[i];
            status = CLOCK_SUCCESS;
            break;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description:    Set clock's source
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
LOCAL CLOCK_STATUS _ClockSetSource (CLOCK_HANDLE thiz, CLOCK_HANDLE pSrcObj)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    //CLOCK_LOG_TRACE:"[_CLOCK_SourceSet] 0x%x 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_243_112_2_17_23_3_44_498,(uint8*)"dd", thiz, pSrcObj);
    CLOCK_ASSERT (thiz != SCI_NULL);/*assert verified*/
    CLOCK_ASSERT (pSrcObj != SCI_NULL);/*assert verified*/

    do
    {
        /* Set new source clock */
        thiz->SrcClockObj = pSrcObj;

        /* Add clock to source's subclock list */
        LIST_ADD (&thiz->SiblingClk, &pSrcObj->SubClock);
    }
    while (0);

    return status;
}

/*****************************************************************************/
//  Description:    Enable device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_EnableClock (DEVICE_HANDLE thiz)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (SCI_NULL != thiz);/*assert verified*/
    //CLOCK_LOG_TRACE:"[DEVICE_ClockEnable] %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_269_112_2_17_23_3_44_499,(uint8*)"s", thiz->Device->Name);

    if (SCI_NULL != thiz->Device->Open)
    {
        /* Device open callback */
        thiz->Device->Open();
    }

    return status;
}

/*****************************************************************************/
//  Description:    Disable device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_DisableClock (DEVICE_HANDLE thiz)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (SCI_NULL != thiz);/*assert verified*/
    //CLOCK_LOG_TRACE:"[DEVICE_DisableClock] %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_289_112_2_17_23_3_44_500,(uint8*)"s", thiz->Device->Name);

    if (SCI_NULL != thiz->Device->Close)
    {
        /* Device disable callback */
        thiz->Device->Close();
    }

    return status;
}

/*****************************************************************************/
//  Description:    Reset device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Reset (DEVICE_HANDLE thiz)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (SCI_NULL != thiz);/*assert verified*/
    //CLOCK_LOG_TRACE:"[DEVICE_Reset] %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_309_112_2_17_23_3_44_501,(uint8*)"s", thiz->Device->Name);

    if (SCI_NULL != thiz->Device->Reset)
    {
        /* Device reset callback */
        thiz->Device->Reset();
    }

    return status;
}

/*****************************************************************************/
//  Description:    Open device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Open (DEVICE_HANDLE thiz)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (SCI_NULL != thiz);/*assert verified*/
    //CLOCK_LOG_TRACE:"[_DEVICE_Open] %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_329_112_2_17_23_3_44_502,(uint8*)"s", thiz->Device->Name);

    if (SCI_TRUE == thiz->bOpened)
    {
        //CLOCK_LOG_TRACE:"[DEVICE_Open] The device has been opened"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_333_112_2_17_23_3_44_503,(uint8*)"");

        return status;
    }

    /* Open parent device */
    if (SCI_NULL != thiz->ParentDevObj)
    {
        if (SCI_FALSE == thiz->ParentDevObj->bOpened)
        {
            DEVICE_Open (thiz->ParentDevObj);
        }
    }

    /* Open source clock */
    if (SCI_NULL != thiz->SrcClockObj)
    {
        if (SCI_FALSE == thiz->SrcClockObj->bOpened)
        {
            _OpenClock (thiz->SrcClockObj);
        }
    }

    thiz->bOpened = SCI_TRUE;

    if (SCI_NULL != thiz->Device->Open)
    {
        thiz->Device->Open();
    }


    return status;
}

/*****************************************************************************/
//  Description:    Close device
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_Close (DEVICE_HANDLE thiz)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (SCI_NULL != thiz);/*assert verified*/
    //CLOCK_LOG_TRACE:"[DEVICE_Close] %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_376_112_2_17_23_3_44_504,(uint8*)"s", thiz->Device->Name);

    do
    {
        LIST_HEAD       *item;
        DEVICE_HANDLE   pObj;

        if (SCI_FALSE == thiz->bOpened)
        {
            //CLOCK_LOG_TRACE:"[DEVICE_Close] Device has already been closed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_386_112_2_17_23_3_44_505,(uint8*)"",thiz->Device->Name);
            break;
        }

        /* If all sub devices were closed */
        item = thiz->SubDevice.next;

        while (&thiz->SubDevice != item)
        {
            pObj = (DEVICE_HANDLE) LIST_ENTRY (item, struct _DEVICE_OBJECT, SiblingDev);/*lint !e413 comfirmed by xuepeng*/

            if (SCI_TRUE == pObj->bOpened)
            {
                //CLOCK_LOG_TRACE:"[DEVICE_Close] Device %s was opened yet"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_400_112_2_17_23_3_44_506,(uint8*)"s",pObj->Device->Name);
                return status;
            }

            item = item->next;
        }

        thiz->bOpened = SCI_FALSE;


        if (SCI_NULL != thiz->Device->Close)
        {
            thiz->Device->Close();
        }

        /* Close source clock */
        if (SCI_NULL != thiz->SrcClockObj)
        {
            _CloseClock (thiz->SrcClockObj);
        }

        /* Close parent device */
        if (SCI_NULL != thiz->ParentDevObj)
        {
            if (SCI_TRUE == thiz->ParentDevObj->bOpened)
            {

                DEVICE_Close (thiz->ParentDevObj);
            }
        }
    }
    while (0);

    return status;
}

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetClockAndDiv (DEVICE_HANDLE thiz, int ClockValue, int Div)
{
    CLOCK_HANDLE pClkObj;
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (thiz   != SCI_NULL);/*assert verified*/

    ENTER_CS();

    do
    {
        /* Get clock source object */
        status = CLOCK_FindByValue (ClockValue, &pClkObj);

        if (status == CLOCK_FAIL)
        {
            //CLOCK_ERR_TRACE:"[DEVICE_SetClock] Fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_456_112_2_17_23_3_44_507,(uint8*)"");
            break;
        }

        /* Set source clock */
        thiz->SrcClockObj = pClkObj;

        /* Add device to source clock's device list */
        LIST_ADD (&thiz->SiblingSrc, &pClkObj->SubDevice);

        if (thiz->Device->SrcSet != SCI_NULL)
        {
            thiz->Device->SrcSet (pClkObj->Clock->Name);
        }

        if (thiz->Device->DivSet != SCI_NULL)
        {
            thiz->Device->DivSet (Div);
            thiz->Div = Div;
        }
    }
    while (0);

    LEAVE_CS();

    return status;
}

/*****************************************************************************/
//  Description:    Set device's work clock and divider
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetClock (DEVICE_HANDLE thiz, int ClockValue)
{
    CLOCK_HANDLE pClkObj;
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (thiz   != SCI_NULL);/*assert verified*/

    ENTER_CS();

    do
    {
        /* Get clock source object */
        status = CLOCK_FindByValue (ClockValue, &pClkObj);

        if (status == CLOCK_FAIL)
        {
            //CLOCK_ERR_TRACE:"[DEVICE_SetClock] Fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_504_112_2_17_23_3_44_508,(uint8*)"");
            break;
        }

        /* Set source clock */
        thiz->SrcClockObj = pClkObj;

        /* Add device to source clock's device list */
        LIST_ADD (&thiz->SiblingSrc, &pClkObj->SubDevice);

        if (thiz->Device->SrcSet != SCI_NULL)
        {
            thiz->Device->SrcSet (pClkObj->Clock->Name);
        }
    }
    while (0);

    LEAVE_CS();

    return status;
}

/*****************************************************************************/
//  Description:    Set device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC CLOCK_STATUS DEVICE_SetClockDiv (DEVICE_HANDLE thiz, int Div)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (thiz   != SCI_NULL);/*assert verified*/

    ENTER_CS();

    do
    {
        if (thiz->Device->DivSet != SCI_NULL)
        {
            thiz->Device->DivSet (Div);
            thiz->Div = Div;
        }
    }
    while (0);

    LEAVE_CS();

    return status;
}

/*****************************************************************************/
//  Description:    Get device's clock dividor
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC uint32 DEVICE_GetClockDiv (DEVICE_HANDLE thiz)
{
    CLOCK_ASSERT (thiz   != SCI_NULL);/*assert verified*/

    return thiz->Div;
}

/*****************************************************************************/
//  Description:    Get device's work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
PUBLIC uint32 DEVICE_GetClock (DEVICE_HANDLE thiz)
{
    //CLOCK_LOG_TRACE:"[DEVICE_GetClock] %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_570_112_2_17_23_3_44_509,(uint8*)"s", thiz);
    CLOCK_ASSERT (SCI_NULL != thiz);/*assert verified*/

    return thiz->SrcClockObj->Clock->ClockValue;
}

/*****************************************************************************/
//  Description:    Set device's interface clock and work clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
LOCAL CLOCK_STATUS _DeviceSetParent (DEVICE_HANDLE thiz, DEVICE_HANDLE pParentObj)
{
    CLOCK_STATUS    status      = CLOCK_SUCCESS;

    CLOCK_ASSERT (thiz          != SCI_NULL);/*assert verified*/
    CLOCK_ASSERT (pParentObj    != SCI_NULL);/*assert verified*/

    /* Set parent device */
    thiz->ParentDevObj = pParentObj;

    /* Add device to parent's subdevice list */
    LIST_ADD (&thiz->SiblingDev, &pParentObj->SubDevice);

    return status;
}

/*****************************************************************************/
//  Description:    Init clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
LOCAL CLOCK_STATUS _InitClock (void)
{
    int i;
    CLOCK_HANDLE pClkObj;
    CLOCK_HANDLE pSrcObj;
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    g_ClkObj = (CLOCK_HANDLE) SCI_ALLOC_APP ( (uint32) g_ClkCfg.ClockNr * sizeof (struct _CLOCK_OBJECT));
    CLOCK_ASSERT (SCI_NULL != g_ClkObj);/*assert verified*/

    for (i=0; i<g_ClkCfg.ClockNr; i++)
    {
        g_ClkObj[i].Clock = &g_ClkCfg.Clock[i];
        g_ClkObj[i].bOpened = g_ClkCfg.Clock[i].bOpen;
        g_ClkObj[i].SrcClockObj = SCI_NULL;
        INIT_LIST_HEAD (&g_ClkObj[i].SubDevice);
        INIT_LIST_HEAD (&g_ClkObj[i].SubClock);
        INIT_LIST_HEAD (&g_ClkObj[i].SiblingClk);
    }

    for (i=0; i<g_ClkCfg.ClockNr; i++)
    {
        if (0 != strcmp (g_ClkObj[i].Clock->DefaultSrc, g_ClkObj[i].Clock->Name))
        {
            status = CLOCK_Find (g_ClkObj[i].Clock->Name, &pClkObj);
            CLOCK_ASSERT (CLOCK_SUCCESS == status);/*assert verified*/

            status = CLOCK_Find (g_ClkObj[i].Clock->DefaultSrc, &pSrcObj);
            CLOCK_ASSERT (CLOCK_SUCCESS == status);/*assert verified*/

            /* Select default source for clocks */
            status = _ClockSetSource (pClkObj, pSrcObj);
            CLOCK_ASSERT (CLOCK_SUCCESS == status);/*assert verified*/
        }
    }

    return status;
}

/*****************************************************************************/
//  Description:    Init clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
LOCAL CLOCK_STATUS _InitDevice (void)
{
    int i;
    DEVICE_HANDLE pDevObj, pParDevObj;
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    /* Create device hash table */
    g_DevHash = HASH_TABLE_Create (g_ClkCfg.DeviceNr* 2);
    CLOCK_ASSERT (SCI_NULL != g_DevHash);/*assert verified*/

    for (i=0; i<g_ClkCfg.DeviceNr; i++)
    {
        /* Create device object */
        status = _DeviceObjectCreate (g_DevHash, &g_ClkCfg.Device[i]);
        CLOCK_ASSERT (CLOCK_SUCCESS == status);/*assert verified*/
    }

    for (i=0; i<g_ClkCfg.DeviceNr; i++)
    {
        status = DEVICE_Find (g_ClkCfg.Device[i].Name, &pDevObj);
        CLOCK_ASSERT (CLOCK_SUCCESS == status);/*assert verified*/

        status = DEVICE_Find (g_ClkCfg.Device[i].ParentDev, &pParDevObj);
        CLOCK_ASSERT (CLOCK_SUCCESS == status);/*assert verified*/


        /* Select work clock and parent for devices */
        if (g_ClkCfg.Device[i].ClockValue != SCI_NULL)
        {
            status = DEVICE_SetClockAndDiv (pDevObj, g_ClkCfg.Device[i].ClockValue, g_ClkCfg.Device[i].Div);
            CLOCK_ASSERT (CLOCK_SUCCESS == status);/*assert verified*/
        }

        if (pParDevObj->Device != pDevObj->Device)
        {
            status = _DeviceSetParent (pDevObj, pParDevObj);
            CLOCK_ASSERT (CLOCK_SUCCESS == status);/*assert verified*/
        }
    }

    return status;
}

/*****************************************************************************/
//  Description:    Open clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
LOCAL CLOCK_STATUS _OpenClock (CLOCK_HANDLE ClkObj)
{
    CLOCK_STATUS status     = CLOCK_SUCCESS;

    CLOCK_ASSERT (SCI_NULL != ClkObj);/*assert verified*/
    //CLOCK_LOG_TRACE:"[_OpenClock] IN (ClkObj = 0x%x)\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_695_112_2_17_23_3_45_510,(uint8*)"d", ClkObj);

    if (SCI_TRUE == ClkObj->bOpened)
    {
        //CLOCK_LOG_TRACE:"[_OpenClock] Clock has already been opened"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_699_112_2_17_23_3_45_511,(uint8*)"");
        return status;
    }

    if (SCI_NULL != ClkObj->SrcClockObj)
    {
        if (ClkObj->SrcClockObj->bOpened == SCI_FALSE)
        {
            _OpenClock (ClkObj->SrcClockObj);
        }
    }

    ClkObj->bOpened = SCI_TRUE;

    /* Clock open callback */
    if (SCI_NULL != ClkObj->Clock->Open)
    {
        ClkObj->Clock->Open();
    }

    return status;
}

/*****************************************************************************/
//  Description:    Close clock
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
LOCAL CLOCK_STATUS _CloseClock (CLOCK_HANDLE ClkObj)
{
    LIST_HEAD       *item;
    CLOCK_HANDLE    pObj;
    DEVICE_HANDLE   qObj;
    CLOCK_STATUS    status  = CLOCK_SUCCESS;

    CLOCK_ASSERT (SCI_NULL != ClkObj);/*assert verified*/
    //CLOCK_LOG_TRACE:"[_CloseClock] IN (%s)\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_734_112_2_17_23_3_45_512,(uint8*)"s", ClkObj->Clock->Name);

    do
    {
        if (SCI_FALSE == ClkObj->bOpened)
        {
            //CLOCK_LOG_TRACE:"[_CloseClock] Clock has already been closed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_740_112_2_17_23_3_45_513,(uint8*)"");
            break;
        }

        /* If all sub clocks were closed */
        item = ClkObj->SubClock.next;

        while (&ClkObj->SubClock != item)
        {
            pObj = (CLOCK_HANDLE) LIST_ENTRY (item, struct _CLOCK_OBJECT, SiblingClk);/*lint !e413 comfirmed by xuepeng*/

            if (SCI_TRUE == pObj->bOpened)
            {
                return status;
            }

            item = item->next;
        }

        /* If all sub Device were closed */
        item = ClkObj->SubDevice.next;

        while (&ClkObj->SubDevice != item)
        {
            qObj = (DEVICE_HANDLE) LIST_ENTRY (item, struct _DEVICE_OBJECT, SiblingSrc);/*lint !e413 comfirmed by xuepeng*/

            if (SCI_TRUE == qObj->bOpened)
            {
                return status;
            }

            item = item->next;
        }

        ClkObj->bOpened = SCI_FALSE;

        if (SCI_NULL != ClkObj->Clock->Close)
        {
            ClkObj->Clock->Close();
        }

        /* Close parent clock */
        if (SCI_NULL != ClkObj->SrcClockObj)
        {
            if (SCI_TRUE == ClkObj->SrcClockObj->bOpened)
            {
                _CloseClock (ClkObj->SrcClockObj);
            }
        }

    }
    while (0);

    return status;
}

/*****************************************************************************/
//  Description:    Create device object
//  Global resource dependence:
//  Author:         Yi.Qiu
//  Note:
/*****************************************************************************/
LOCAL CLOCK_STATUS _DeviceObjectCreate (HASH_TABLE *Hash, DEVICE *pDev)
{
    CLOCK_STATUS    status  = CLOCK_SUCCESS;
    DEVICE_HANDLE   pObj    = SCI_NULL;
    const char      *Name   = SCI_NULL;

    //CLOCK_LOG_TRACE:"[_DeviceObjectCreate] IN (pDev = 0x%x)\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_806_112_2_17_23_3_45_514,(uint8*)"d", pDev);
    CLOCK_ASSERT (SCI_NULL != pDev);/*assert verified*/

    do
    {
        Name = pDev->Name;

        if (SCI_NULL != HASH_TABLE_Lookup (Hash, Name))
        {
            //CLOCK_ERR_TRACE:"[_DeviceObjectCreate] <%s> has been added\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_815_112_2_17_23_3_45_515,(uint8*)"s", Name);
            status = CLOCK_FAIL;
            break;
        }

        if (SCI_NULL == (pObj = SCI_ALLOC_APP (sizeof (*pObj))))
        {
            //CLOCK_ERR_TRACE:"[_DeviceObjectCreate] Fail to allocate memoryrn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_823_112_2_17_23_3_45_516,(uint8*)"",sizeof (*pObj));
            status = CLOCK_FAIL;
            break;
        }

        SCI_MEMSET (pObj, 0, sizeof (*pObj));

        if (!HASH_TABLE_Insert (Hash, Name, (void *) pObj))
        {
            //CLOCK_ERR_TRACE:"[_DeviceObjectCreate] Fail to add device Name to "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CLOCK_833_112_2_17_23_3_45_517,(uint8*)"");
            status = CLOCK_FAIL;
            break;
        }

        pObj->bOpened       = SCI_FALSE;
        pObj->Device        = pDev;
        pObj->ParentDevObj  = SCI_NULL;
        pObj->SrcClockObj   = SCI_NULL;

        INIT_LIST_HEAD (&pObj->SiblingSrc);
        INIT_LIST_HEAD (&pObj->SubDevice);
        INIT_LIST_HEAD (&pObj->SiblingDev);

        return status;
    }
    while (0);

    if (NULL != pObj)
    {
        /* If p is not NULL, Name can't be in the symbol table already. So try
         * to delete it from symtab anyway.
         */
        HASH_TABLE_Delete (Hash, Name);
        SCI_FREE (pObj);
    }

    return status;
}


/* Copyright (C) 2020 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#include "usb_drv.h"
#include "usb_composite_device.h"
#include "usb_cops_func.h"
#include "usb_utils.h"
#include "osi_compiler.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "usb_serial.h"
#include "chg_drvapi.h"
#include "ucom_api.h"
#include "gpio_prod_api.h"
#include "usbservice_api.h"
#include "deepsleep_drvapi.h"
#include "chg_drvapi.h"

typedef enum
{
    USB_STATE_IDLE = 0,    // usb cable not connected
    USB_STATE_DEBOUNCE,    // usb cable has already connected, but not start enumerating  //not used
    USB_STATE_VBUS_STABLE, // usb vbus stable
    USB_STATE_CONNECT_WIP, // usb try enumerating
    USB_STATE_CONNECTED    // usb enumerated
}DrvUsbState_e;

typedef struct DrvUsb
{
    bool mEnable;// = false;
    uint32_t mDebounceTimeMs;// = 0;
    uint32_t mStableTime;// = 0;
    DrvUsbState_e mState;// = DrvUsbState::IDLE;
    drvUSBWorkMode_t mMode;// = DRV_USB_CHARGER_ONLY;
    int mDetMode;
    udc_t *mUdc;//; = PNULL;
    cops_t *mCops;// = PNULL;
    SCI_TIMER_PTR            s_vbus_debounce_timer_ptr;// = PNULL;
    SCI_TIMER_PTR            s_usb_NotRecognize_timer_ptr;// = PNULL;
    SCI_TIMER_PTR            s_reEnum_timer_ptr;// = PNULL;
    SCI_MUTEX_PTR mLock;// = PNULL;
}DrvUsb_t;

#define DRV_NAME_USB20 OSI_MAKE_TAG('U', 'S', 'B', '2')         // USB2

#define USB_USERIAL_MAX_COUNT       USB_MAX_NUM

DrvUsb_t gDrvUsb;// = PNULL;
#if defined(USB_MASS_STORAGE_SUPPORT)
uint8 udcConnEvent_has_been_sent = 0;
#endif
copsFunc_t *userialfuncs[USB_USERIAL_MAX_COUNT]={PNULL,PNULL,PNULL,PNULL,PNULL};

static void changeState(DrvUsbState_e newstate)
{
    USB_LOG_TRACE("[USB][DRV]usb change state %d->%d (%d/%d)",gDrvUsb.mState, newstate, gDrvUsb.mMode, gDrvUsb.mDebounceTimeMs);
    gDrvUsb.mState = newstate;
}

PUBLIC void USB_SetChargeStatus(uint32 status)
{
    changeState(status);

}
static bool readyStart(void)
{
    return gDrvUsb.mEnable && gDrvUsb.mState == USB_STATE_VBUS_STABLE && gDrvUsb.mMode != DRV_USB_CHARGER_ONLY;
}

static void lock()
{
    SCI_GetMutex(gDrvUsb.mLock,SCI_WAIT_FOREVER);
}

static void unlock()
{
    SCI_PutMutex(gDrvUsb.mLock);
}
static void _chargerOff(DrvUsb_t *d)
{
    /*//save old state*/
    uint8_t state = d->mState; 
    changeState(USB_STATE_IDLE);
    if (state == USB_STATE_CONNECT_WIP || state == USB_STATE_VBUS_STABLE){
        if (SCI_IsTimerActive (d->s_usb_NotRecognize_timer_ptr))
        {
            SCI_DeactiveTimer (d->s_usb_NotRecognize_timer_ptr);
        }
        if (SCI_IsTimerActive (d->s_reEnum_timer_ptr))
        {
            SCI_DeactiveTimer (d->s_reEnum_timer_ptr);
        }
    }
    else if (state == USB_STATE_DEBOUNCE)
    {
        if (d->mDebounceTimeMs != 0){
            if (SCI_IsTimerActive (d->s_vbus_debounce_timer_ptr))
            {
                SCI_DeactiveTimer (d->s_vbus_debounce_timer_ptr);
            }
            if (SCI_IsTimerActive (d->s_usb_NotRecognize_timer_ptr))
            {
                SCI_DeactiveTimer (d->s_usb_NotRecognize_timer_ptr);
            }
        }
    }
    udcStop(d->mUdc);
}
uint8 reEnumCnt = 0;
static void _notRecognizeWork(void *ctx)
{
    DrvUsb_t *d = (DrvUsb_t *)ctx;
    //lock();// don't use mutex in timer,will cause assert
    USB_LOG_TRACE("[USB][DRV]_notRecognizeWork  usb enumerate timeout\n");

    drvUsbStop();

    /*try enum again*/
    if(reEnumCnt < 3){
        drvUsbRestart();
    }
    //unlock();// don't use mutex in timer,will cause assert
}

static void _enableEnum(void *ctx)
{
    reEnumCnt++;
    USB_LOG_TRACE("[USB][DRV]_enableEnum reEnumCnt :%d:\n",reEnumCnt);
    //0Õâ¸ö²ÎÊýÓÃÓÚÇø·ÖtimerÀïµÄenable ºÍ Õý³£Ä£Ê½ÏÂµÄenable
    drvUsbEnable(0); //timerÏÂ²»ÄÜÊ¹ÓÃmutex
}

static void _startUsb(DrvUsb_t *d)
{
    changeState(USB_STATE_CONNECT_WIP);
    if (d->mDetMode != USB_DETMODE_AON){
        SCI_ChangeTimer (d->s_usb_NotRecognize_timer_ptr,_notRecognizeWork,CONFIG_USB_CONNECT_TIMEOUT);
        SCI_ActiveTimer(d->s_usb_NotRecognize_timer_ptr);
    }
    USB_LOG_TRACE("[USB][DRV]_startUsb\n");
    udcStart(d->mUdc);
}

static void _chargerOnWork(void *ctx)
{
    DrvUsb_t *d = (DrvUsb_t *)ctx;
    bool vbus_new = false; 
    uint8_t type;
    #if PMIC_CHARGER_DETECT_SUPPORT
    type = CHGMNG_IdentifyAdpType();  
    #else
    type = CHGMNG_ADP_USB;//now we suppose it is usb host
    #endif
    //USB_LOG_TRACE("usb charger on type %d", type);

    if (type == CHGMNG_ADP_USB)
        vbus_new = true;

    if (vbus_new && d->mState < USB_STATE_VBUS_STABLE)
    {
        d->mStableTime =SCI_GetTickCount();
        changeState(USB_STATE_VBUS_STABLE);
 
        USB_LOG_TRACE("[USB][DRV]send USB_VBUS_PLUG_IN\n");
        SCI_UPM_EnableDeepSleep (DISABLE_DEEP_SLEEP);

        UsbService_SendMsgToVbusThread(USB_VBUS_PLUG_IN,0);
    }
    //lock(); // don't use mutex in timer,will cause assert
    if (readyStart())
        _startUsb(d);
        
    //unlock();// don't use mutex in timer,will cause assert
}

static  void _vbusDebounceStart(DrvUsb_t *d)
{
    uint32 time_expire = 0;
    if(d->mDebounceTimeMs ==0)
        time_expire = 100;//100ms
    else
        time_expire = d->mDebounceTimeMs;//600ms

    SCI_ChangeTimer (d->s_vbus_debounce_timer_ptr,_chargerOnWork,time_expire);
    SCI_ActiveTimer (d->s_vbus_debounce_timer_ptr);
}

static void _udcEventNotifier(void *ctx, udcEvent_t event)
{
    DrvUsb_t *d = (DrvUsb_t *)ctx;
    
    USB_LOG_TRACE("[USB][DRV]_udcEventNotifier  udc event :%d:\n",event);
    if (event == UDC_CONNECTED)
    {
        reEnumCnt = 0 ;
        if (SCI_IsTimerActive (d->s_usb_NotRecognize_timer_ptr))
        {
            SCI_DeactiveTimer (d->s_usb_NotRecognize_timer_ptr);
        }
        if (SCI_IsTimerActive (d->s_reEnum_timer_ptr))
        {
            SCI_DeactiveTimer (d->s_reEnum_timer_ptr);
        }
        changeState(USB_STATE_CONNECTED);
#if defined(USB_MASS_STORAGE_SUPPORT)
        /*send to usbservice vbus_thread*/
        if(!udcConnEvent_has_been_sent){
            UsbService_SendMsgToVbusThread(USB_HOST_PLUG_IN,0);
            udcConnEvent_has_been_sent=1;
        }
        else
            USB_LOG_TRACE("[USB][DRV]USB_HOST_PLUG_IN has been sent :%d:\n",udcConnEvent_has_been_sent);
#else
        UsbService_SendMsgToVbusThread(USB_HOST_PLUG_IN,0);
#endif
        /*send to charger thread to update adt_type*/
        CHGMNG_SendMsgToChgTask(CHGMNG_USB_HOST_PLUG_IN_MSG, 0);
    }
    else if(event == UDC_DISCONNECT)
    {
        changeState(USB_STATE_VBUS_STABLE);
    }
}

PUBLIC udc_t* getUdcController()
{
    return gDrvUsb.mUdc;
}

void DrvUsb_exit(void)
{
    if (SCI_IsTimerActive (gDrvUsb.s_usb_NotRecognize_timer_ptr))
    {
        SCI_DeactiveTimer (gDrvUsb.s_usb_NotRecognize_timer_ptr);
    }
    if (SCI_IsTimerActive (gDrvUsb.s_vbus_debounce_timer_ptr))
    {
        SCI_DeactiveTimer (gDrvUsb.s_vbus_debounce_timer_ptr);
    }
    if (SCI_IsTimerActive (gDrvUsb.s_reEnum_timer_ptr))
    {
        SCI_DeactiveTimer (gDrvUsb.s_reEnum_timer_ptr);
    }
    reEnumCnt = 0;
    copsDestroy(gDrvUsb.mCops);
    udcDestroy(gDrvUsb.mUdc);
    gDrvUsb.mCops = PNULL;
    gDrvUsb.mUdc =PNULL;
    SCI_DeleteTimer(gDrvUsb.s_usb_NotRecognize_timer_ptr);
    SCI_DeleteTimer(gDrvUsb.s_vbus_debounce_timer_ptr);
    SCI_DeleteTimer(gDrvUsb.s_reEnum_timer_ptr);
    SCI_DeleteMutex(gDrvUsb.mLock);
}

// All resource will be released in destructure function
bool DrvUsb_init(uint32_t name, int detmode)
{
    if(gDrvUsb.mUdc)
    {
        USB_LOG_TRACE("[USB][DRV]usb already inited!\n");
        return true;
    }
    gDrvUsb.mLock = SCI_CreateMutex("drv_udc",SCI_NO_INHERIT);
    if (!gDrvUsb.mLock)
        return false;

    gDrvUsb.s_vbus_debounce_timer_ptr = SCI_CreateTimer ("vbus debounce timer",\
                                       _chargerOnWork,\
                                       &gDrvUsb,100 ,\
                                       SCI_NO_ACTIVATE);

    gDrvUsb.s_usb_NotRecognize_timer_ptr = SCI_CreateTimer ("USB Not Recognize Timer",\
                                       _notRecognizeWork,\
                                       &gDrvUsb,CONFIG_USB_CONNECT_TIMEOUT ,\
                                       SCI_NO_ACTIVATE);

    gDrvUsb.s_reEnum_timer_ptr = SCI_CreateTimer ("USB Re enumerate Timer",\
                                       _enableEnum,\
                                       &gDrvUsb,5000 ,\
                                       SCI_NO_ACTIVATE);

    if (!(gDrvUsb.s_vbus_debounce_timer_ptr && gDrvUsb.s_usb_NotRecognize_timer_ptr && gDrvUsb.s_reEnum_timer_ptr)){
        USB_ERR_TRACE("[USB][DRV]usb timer create fail\n");
        return false;
    }

    gDrvUsb.mUdc = udcCreate(name);
    gDrvUsb.mCops = copsCreate();
    if (!gDrvUsb.mUdc || !gDrvUsb.mCops){
        USB_ERR_TRACE("[USB][DRV]udc init fail\n");
        return false;
    }

    udcStop(gDrvUsb.mUdc);
    udcSetNotifier(gDrvUsb.mUdc, _udcEventNotifier, &gDrvUsb);
    udcBindDriver(gDrvUsb.mUdc, copsGetGadgetDriver(gDrvUsb.mCops));

    gDrvUsb.mMode = DRV_USB_CHARGER_ONLY;
    gDrvUsb.mDetMode = detmode;
    if (detmode == USB_DETMODE_AON)
    {
        gDrvUsb.mStableTime = SCI_GetTickCount();
        gDrvUsb.mState = USB_STATE_VBUS_STABLE;
    }
    else
    {
        gDrvUsb.mState = USB_STATE_IDLE;
    }
    GPIO_VBUS_RegIntHandler(drvUsbSetAttach);
    USB_LOG_TRACE("[USB][DRV]DrvUsb_init success\n");
    return true;
}

PUBLIC void drvUsbInit(void)
{
    USB_LOG_TRACE("[USB][DRV]Enter drvUsbInit\n");

    if (!DrvUsb_init(DRV_NAME_USB20, USB_DETMODE_CHARGER/*gSysnvUsbDetMode*/))
    {
        USB_ERR_TRACE("[USB][DRV]usb module init fail\n");
        DrvUsb_exit();
        SCI_ASSERT(0);

    }
}

void setSerialsFuncs(drvUSBWorkMode_t ucom_type)
{
    uint8 i;
    //clear userialfuncs first
    for (i=0;i<USB_USERIAL_MAX_COUNT; i++)
    {
         userialfuncs[i] = PNULL;
    }

    if(ucom_type == UCOM_CREATE_COM_DEBUG)
    {
        userialfuncs[0] = createDebugSerialFunc(DRV_NAME_USRL_COM1);//diag

    }else if(ucom_type == UCOM_CREATE_COM_DATA){

        userialfuncs[0] = createDebugSerialFunc(DRV_NAME_USRL_COM0);//at

    }else if(ucom_type == UCOM_CREATE_COM_DEBUGDATA){

        userialfuncs[0] = createDebugSerialFunc(DRV_NAME_USRL_COM0);//at
        userialfuncs[1] = createDebugSerialFunc(DRV_NAME_USRL_COM1);//diag

    }else if(ucom_type == UCOM_CREATE_COM_DATADISK){

        userialfuncs[0] = createDebugSerialFunc(DRV_NAME_USRL_COM0);//at

    }else if(ucom_type == UCOM_CREATE_COM_NPI){
        //userialfuncs[0] = createDebugSerialFunc(DRV_NAME_USRL_COM0);//at
        userialfuncs[0] = createDebugSerialFunc(DRV_NAME_USRL_COM1);//diag
        userialfuncs[1] = createDebugSerialFunc(DRV_NAME_USRL_COM3);//modem log
    }else if(ucom_type == UCOM_CREATE_COM_DEBUGDATA_MODEM){

        userialfuncs[0] = createDebugSerialFunc(DRV_NAME_USRL_COM0);//AT
        userialfuncs[1] = createDebugSerialFunc(DRV_NAME_USRL_COM1);//diag = ap log
        userialfuncs[2] = createDebugSerialFunc(DRV_NAME_USRL_COM2);//debug host
        userialfuncs[3] = createDebugSerialFunc(DRV_NAME_USRL_COM3);//modem log
        #ifdef PPP_USB
        userialfuncs[4] = createDebugSerialFunc(DRV_NAME_USRL_COM4);//PPP_AT
        #endif

    }
    /*else if(ucom_type == UCOM_CREATE_COM_ASSERT){ //to confirm,use vcom
        userialfuncs[0] = createDebugSerialFunc(DRV_NAME_USRL_COM1);//diag
        userialfuncs[1] = createDebugSerialFunc(DRV_NAME_USRL_COM3);//modem log
    }*/
}

static bool _setSerials(DrvUsb_t *d)
{
    uint8_t i = 0;
    copsFunc_t* f = PNULL;
    uint8_t ucom_type = DRV_USB_CHARGER_ONLY;
    uint16_t pid = USB_PID_6121E_SERIALS;
    for (i=0;i<USB_USERIAL_MAX_COUNT; i++)
    {
        f = userialfuncs[i];
        if (f == PNULL)
            break;

        if (!copsAddFunction(d->mCops, f))
        {
            goto fail;

        }
    }

     copsSetVendorId(d->mCops, USB_VID_SPRD); //0x1782

     ucom_type = UCOM_GetUcomType();

    if(ucom_type == UCOM_CREATE_COM_DATA)
        pid =0x3D13;//0x3D13
    else if(ucom_type == UCOM_CREATE_COM_DEBUG)
        pid =0x3d10;//0X3D10
    else if(ucom_type == UCOM_CREATE_COM_DEBUGDATA_MODEM)
        pid =USB_PID_6121E_SERIALS;//0X4d17
    else if(ucom_type == UCOM_CREATE_COM_DEBUGDATA)
        pid =USB_PID_6121E_SERIALS;//0X4d17
    else if(ucom_type == UCOM_CREATE_COM_CALI)
        pid =USB_SPREADTRUM_VCOM_ID;//0X4d00
    else if(ucom_type == UCOM_CREATE_COM_NPI )
        pid =USB_PID_6121E_NPI_SERIAL;//0X4d30
    else if(ucom_type == UCOM_CREATE_COM_ASSERT )
        pid =USB_PID_6121E_SERIALS;//0X4d17
#if defined(USB_MASS_STORAGE_SUPPORT)
    else if(ucom_type == DRV_USB_UDISK)
        pid =USB_PID_6121E_SERIALS;//0X4d17
#endif

    copsSetProductId(d->mCops, pid);
    return true;

fail:
    for (i=0;i<USB_USERIAL_MAX_COUNT; i++)
    {
        f = userialfuncs[i];
        if (f != PNULL)
            cfDestroy(f);
    }
    return false;
}
#if defined(USB_MASS_STORAGE_SUPPORT)
static bool _setSingleMsgMode(DrvUsb_t *d)
{
    copsFunc_t *f = PNULL;

    f = createMassStorageFunc(OSI_MAKE_TAG('E', 'M', 'M', 'C'),OSI_MAKE_TAG('U', 'M', 'S', '0'));

    if (!f)
        return false;

    if (!copsAddFunction(d->mCops, f))
    {
        cfDestroy(f);
        return false;
    }

    copsSetVendorId(d->mCops, USB_VID_SPRD);
    copsSetProductId(d->mCops, 0x4d01);
    return true;
}
#endif
static void _stopUsbBySoft(DrvUsb_t *d)
{
    uint8 state = d->mState;
    if (state > USB_STATE_VBUS_STABLE)
    {
        USB_LOG_TRACE("[USB][DRV]_stopUsbBySoft\n  ");
        changeState(USB_STATE_VBUS_STABLE);
        if (state == USB_STATE_CONNECT_WIP){
            if (SCI_IsTimerActive (d->s_usb_NotRecognize_timer_ptr))
            {
                SCI_DeactiveTimer (d->s_usb_NotRecognize_timer_ptr);
            }
        }
        udcStop(d->mUdc);
    }
}
extern PUBLIC void UCOM_SetUcomType(drvUSBWorkMode_t mode);

static bool vcom_SetWorkMode(drvUSBWorkMode_t mode)
{
    bool result = 0;
    DrvUsb_t* d = &gDrvUsb;
    uint8 i = 0;
    UCOM_SetUcomType(mode);

    //clear userialfuncs first
    for (i=0;i<USB_USERIAL_MAX_COUNT; i++)
    {
        userialfuncs[i] = PNULL;

    }

    if(mode == UCOM_CREATE_COM_CALI)
    {
        userialfuncs[0] = createVcomFunc(DRV_NAME_USRL_COM1);//u2s diag
        result = _setSerials(d);
        d->mMode = mode;
    }
    else if(mode == UCOM_CREATE_COM_ASSERT)
    {
        userialfuncs[0] = createVcomFunc(DRV_NAME_USRL_COM1);//diag
        userialfuncs[1] = createVcomFunc(DRV_NAME_USRL_COM3);//modem log
        result = _setSerials(d);
        d->mMode = mode;
    }
    return result;

}

PUBLIC BOOLEAN drvUsbSetWorkMode(drvUSBWorkMode_t mode)
{
    bool result = 0;
    DrvUsb_t* d = &gDrvUsb;
    USB_LOG_TRACE("[USB][DRV]usb set work mode %d/%d", mode, d->mMode);
    if (mode == d->mMode)
        return true;

    lock();

    _stopUsbBySoft(d);
    copsRemoveAllFunction(d->mCops);

    result = (mode == DRV_USB_CHARGER_ONLY);
    if (mode < DRV_USB_UDISK)
    {
        setSerialsFuncs(mode);
        result = _setSerials(d);
    }
    #if defined(USB_MASS_STORAGE_SUPPORT)
    else if (mode == DRV_USB_UDISK)//udisk is not support now
    {
        //setSerialsFuncs(UCOM_CREATE_COM_DEBUGDATA_MODEM);
        //result = _setSerials(d);
        if(1/*result*/)
        {
            result = _setSingleMsgMode(d);

        }
    }
    #endif
    else if (mode == DRV_USB_MTP) //mtp is not support now
    {
        //setSerialsFuncs(UCOM_GetUcomType());
        //result = _setSerials(d);
    }
    if (result)
    {
        d->mMode = mode;
        if (readyStart())
            _startUsb(d);
    }
    else
    {
        d->mMode = DRV_USB_CHARGER_ONLY;
    }
    unlock();

    USB_LOG_TRACE("[USB][DRV]drvUsbSetWorkMode  done\n",mode);
    return result;
}

drvUSBWorkMode_t drvUsbCurrentMode(void)
{
    return gDrvUsb.mMode;
}

PUBLIC BOOLEAN drvUsbIsConnected(void)
{
    return gDrvUsb.mState == USB_STATE_CONNECTED;
}

PUBLIC void drvUsbEnable(uint32 debounce)
{
    DrvUsb_t* d = &gDrvUsb;
    if (d->mEnable)
        return;
    USB_LOG_TRACE("[USB][DRV]drvUsbEnable\n");

    if(debounce != 0)
        lock();
    d->mDebounceTimeMs = debounce;
    d->mEnable = true;
    if (d->mDetMode != USB_DETMODE_AON) //bringup_temp,because vbus hotplug is not ready
    {
        if (SCI_IsTimerActive (d->s_usb_NotRecognize_timer_ptr))
        {
            SCI_DeactiveTimer (d->s_usb_NotRecognize_timer_ptr);
        }
        if(d->mState > USB_STATE_VBUS_STABLE)
        {
            USB_LOG_TRACE("[USB][DRV]invalid usb state when enable\n");
            //SCI_ASSERT(0);
            return;
        }
        //SCI_Assert(d->mState <= DrvUsbState::VBUS_STABLE, "invalid usb state when enable");
        if ( 0 /*d->mState != USB_STATE_IDLE*/) //aready done vbus debounce by pmic
        {
            changeState(USB_STATE_DEBOUNCE);
            _vbusDebounceStart(d);
        }
    }

    if (readyStart())
        _startUsb(d);
    if(debounce != 0)
        unlock();
}

void drvUsbDisable(void)
{
    DrvUsb_t* d = &gDrvUsb;
    if (!d->mEnable)
        return;
    lock();
    d->mEnable = false;
    _stopUsbBySoft(d);
    unlock();
}

bool drvUsbIsEnabled(void)
{
    return gDrvUsb.mEnable;
}

PUBLIC void drvUsbReset(void)
{
    DrvUsb_t* d = &gDrvUsb;
    if(!d->mEnable)
        return;
    udcStop(d->mUdc);
    d->mEnable = true;
    udcStart(d->mUdc);
}

bool drvUsbRemoteWakeup(void)
{
    DrvUsb_t* d = &gDrvUsb;
    if (d->mState == USB_STATE_CONNECTED)
    {
        return udcRemoteWakeup(d->mUdc);
    }
    return false;
}

PUBLIC void drvUsbSetAttach(uint32 attach)
{

    uint8_t type;
    DrvUsb_t* d = &gDrvUsb;
    if (d->mDetMode == USB_DETMODE_AON)
        return; 

    if (attach)  // cable inserted
    {
        USB_LOG_TRACE("[USB][DRV]vbus connect detected!\n");
    #if 0 //disable vbus debounce,cause pmic already done hw debounce
        changeState(USB_STATE_DEBOUNCE);
        _vbusDebounceStart(d);
    #else
        #if PMIC_CHARGER_DETECT_SUPPORT
        type = CHGMNG_IdentifyAdpType(); 
        #else
        /*uws6121e cannot recognize chargerType is host or standard charger
         * now we suppose it is usb host*/
        type = CHGMNG_ADP_USB;
        #endif

        if(type == CHGMNG_ADP_USB){
            d->mStableTime =SCI_GetTickCount();
            changeState(USB_STATE_VBUS_STABLE);
            USB_LOG_TRACE("[USB][DRV]send USB_VBUS_PLUG_IN\n");

            UsbService_SendMsgToVbusThread(USB_VBUS_PLUG_IN,0);
        }
    #endif
    }
    else        // cable plugout
    {
        USB_LOG_TRACE("[USB][DRV]vbus disconnect detected!\n");

        _chargerOff(d);
        SCI_UPM_EnableDeepSleep (ENABLE_DEEP_SLEEP);
        UsbService_SendMsgToVbusThread(USB_VBUS_PLUG_OUT,0);
        #if defined(USB_MASS_STORAGE_SUPPORT)
        udcConnEvent_has_been_sent =0;
        #endif

    }
}



//API for VCOM:polling

void vcom_udc_polling_handler(udc_t *udc)
{
    udcGdbPollIntr(udc);

}
extern void vcom_DwcStart(udc_t *udc);
void vcom_usb_Connect (udcNotifier_t udc_Notifier,uint8 is_assert)
{
/*
    if(gDrvUsb.mUdc)
    {
        USB_LOG_TRACE("[USB][VCOM]usb already inited!\n");
        return;
    }
*/
    USB_ERR_TRACE("[USB][VCOM]%s enter!\n",__FUNCTION__);
    gDrvUsb.mUdc = udcCreate(DRV_NAME_USB20);
    gDrvUsb.mCops = copsCreate();
    if (!gDrvUsb.mUdc || !gDrvUsb.mCops)
    {
        USB_ERR_TRACE("[USB][VCOM]usb udc init failed!\n");
        return;
    }

    //udcStop(gDrvUsb.mUdc);
    
    udcSetNotifier(gDrvUsb.mUdc, udc_Notifier, &gDrvUsb);
    udcBindDriver(gDrvUsb.mUdc, copsGetGadgetDriver(gDrvUsb.mCops));

    gDrvUsb.mState = USB_STATE_IDLE;
if(is_assert)
    vcom_SetWorkMode(UCOM_CREATE_COM_ASSERT);
else
    vcom_SetWorkMode(UCOM_CREATE_COM_CALI);
    //udcStart(gDrvUsb.mUdc);
    vcom_DwcStart(gDrvUsb.mUdc);

    vcom_udc_polling_handler(gDrvUsb.mUdc);
}

void vcom_usb_Disconnect (void)
{
    //for compiler temp
    USB_ERR_TRACE("%s enter\n",__FUNCTION__);
    copsDestroy(gDrvUsb.mCops);
    udcDestroy(gDrvUsb.mUdc);
    gDrvUsb.mCops = PNULL;
    gDrvUsb.mUdc =PNULL;
    USB_ERR_TRACE("%s done\n",__FUNCTION__);
}

BOOLEAN drvUsbIsSuspend(void)
{
    return udcUsbIsSuspend(gDrvUsb.mUdc);
}
void drvUsbStop()
{
    DrvUsb_t* d = &gDrvUsb;
    if (!d->mEnable)
        return;
    d->mEnable = false;
    USB_LOG_TRACE("drv usb, drvUsbStop");
    _stopUsbBySoft(d);
}

void drvUsbRestart()
{
    DrvUsb_t* d = &gDrvUsb;
    USB_LOG_TRACE("drv usb, drvUsbRestart");
    SCI_ChangeTimer (d->s_reEnum_timer_ptr,_enableEnum,5000);
    SCI_ActiveTimer (d->s_reEnum_timer_ptr);
}


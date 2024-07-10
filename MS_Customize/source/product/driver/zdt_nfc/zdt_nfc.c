#ifndef _ZDT_NFC_C
#define _ZDT_NFC_C

#include "zdt_nfc.h"
#include "priority_system.h"
#include "stdarg.h"
#include "ldo_drvapi.h"

static BLOCK_ID zdtnfc_taskid=SCI_INVALID_BLOCK_ID;
static BLOCK_ID zdtnfc_rcv_taskid=SCI_INVALID_BLOCK_ID;
BOOLEAN g_is_nfc_hw_on = FALSE;
BOOLEAN g_is_nfc_not_off = FALSE;
uint32 g_nfc_hw_pwon_time = 0;
LOCAL SCI_TIMER_PTR      s_zdt_nfc_check_timer = PNULL;
LOCAL uint8 s_is_nfc_hw_check = 0;
LOCAL BOOLEAN s_nfc_hw_check_laststatus = FALSE;
uint16        g_nfc_loc_once_cnt = 0;
static BOOLEAN is_nfc_ok = TRUE;
static BOOLEAN nfc_is_init = FALSE;
static BOOLEAN nfc_is_running = FALSE;
static BOOLEAN nfc_need_enter_sleep = FALSE;
static BOOLEAN nfc_is_in_sleep = FALSE;

extern uint8 g_nfc_card_simulation_init_over;

extern BOOLEAN ZDT_NFC_GetSleepStatus(void);

static uint8 Nibble2HexChar(uint8 bNibble)
{
    uint8  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

static uint32 U8_To_Hex(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
{
    uint8 low = 0;
    uint8 high = 0;
    uint32 i = 0;
    uint32 j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = (char)Nibble2HexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)Nibble2HexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

PUBLIC void ZDT_NFCTraceU8(const uint8 *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[NFC]:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[NFC]:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_NFCTraceI2C_Send(const uint8 *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[NFC][I2C S][%d]:%s",SCI_GetTickCount(),dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[NFC][I2C S][%d]:%s",SCI_GetTickCount(),dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void ZDT_NFCTraceI2C_RCV(const uint8 *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[NFC][I2C R][%d]:%s",SCI_GetTickCount(),dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[NFC][I2C R][%d]:%s",SCI_GetTickCount(),dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}
PUBLIC void ZDT_NFCTraceExt(const char *string, int len)
{
#ifndef RELEASE_INFO
    if (len < 128)
    {
        char buff[128] = {0};
        SCI_MEMCPY(buff,string,len);
        SCI_TraceLow("[NFC][%d]:%s",SCI_GetTickCount(),buff);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
            int l = len < 127 ? len : 127;
            SCI_MEMCPY(buff, p, l);
            buff[l] = 0;

            SCI_TraceLow("[NFC][%d]:%s",SCI_GetTickCount(),buff);

            p += l;
            len -= l;
        }
    }
#endif
}

void ZDT_NFCTrace(const char *pszFmt, ...)
{
#ifndef RELEASE_INFO
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif
    if (tmp_len >= 0)
    {
        ZDT_NFCTraceExt(buf,tmp_len);
    }
    va_end(va_args);
#endif
}

uint32 ZDT_NFC_GPIO_IntHandler(BOOLEAN is_high)
{
    #if 0
    static BOOLEAN s_int_is_low = FALSE;
    NFC_TRACE("ZDT_NFC_GPIO_IntHandler is_high=%d",is_high);
    if(s_int_is_low != is_high)
    {
        s_int_is_low = is_high;
        if(is_high)
        {
            NFC_TRACE(" IRQ HIGH");
            APP_SendSigTo_RCV_NFC(SIG_ZDT_NFC_INT,PNULL,0);
        }
    }
    #else
        //NFC_TRACE("ZDT_NFC_GPIO_IntHandler is_high=%d",is_high);
        if(is_high)
        {
            APP_SendSigTo_RCV_NFC(SIG_ZDT_NFC_INT,PNULL,0);
        }
    #endif
    return 0;
}

uint32 ZDT_NFC_ReadTimer_IntHandler(void)
{
    if(GPIO_GetNFCIRQ())
    {
        NFC_TRACE("ZDT_NFC_ReadTimer_IntHandler TimerOut");
        APP_SendSigTo_RCV_NFC(SIG_ZDT_NFC_INT,PNULL,0);
    }
    return 0;
}

BOOLEAN nfc_hw_is_ok(void) 
{
    return is_nfc_ok;
}

void ZDTNFC_HandleRcv(uint8 * rcv_buf, uint32 rcv_len)
{
    NFC_TRACE("ZDTNFC_HandleRcv rcv_len=%d",rcv_len);
}

void ZDTNFC_HandleSend(uint8 * send_buf, uint32 send_len)
{
    zdt_nfc_uart_write(send_buf,send_len);
}

BOOLEAN  ZDTNFC_HandlePowerOn(void)
{
    NFC_TRACE("ZDTNFC_HandlePowerOn Start %d",g_is_nfc_hw_on);
    if(s_is_nfc_hw_check)
    {
        s_nfc_hw_check_laststatus = TRUE;
    }
    if(g_is_nfc_hw_on == FALSE)
    {
        g_nfc_hw_pwon_time = SCI_GetCurrentTime();
        //zdt_nfc_uart_open();
        g_is_nfc_hw_on = TRUE;
        g_nfc_loc_once_cnt = 0;
    }
    return TRUE;
}

BOOLEAN  ZDTNFC_HandlePowerOff(void)
{
    NFC_TRACE("ZDTNFC Power OFF");
    if(s_is_nfc_hw_check)
    {
        s_nfc_hw_check_laststatus = FALSE;
        return TRUE;
    }
    if(g_is_nfc_hw_on)
    {
        if(g_is_nfc_not_off)
        {
            return TRUE;
        }
        //zdt_nfc_uart_close();
        g_is_nfc_hw_on = FALSE;
    }
    return TRUE;
}

LOCAL void ZDTNFC_CHK_TimerCallback(uint32 param)
{	    
    ZDT_NFC_CheckHW_End();
    return ;
}

BOOLEAN ZDTNFC_HandleHwCheckStart(void)
{    
    NFC_TRACE("ZDTNFC HWCheckStart %d",nfc_hw_is_ok());
    Nfc_Card_Reader();
    if(nfc_hw_is_ok() == FALSE && s_is_nfc_hw_check == 0)
    {
        s_nfc_hw_check_laststatus = g_is_nfc_hw_on;
        //ZDTNFC_HandlePowerOn();
        s_is_nfc_hw_check = 1;
        
        if(s_zdt_nfc_check_timer == NULL)
        {
            s_zdt_nfc_check_timer = SCI_CreateTimer("NFCCHK_Timer", ZDTNFC_CHK_TimerCallback,
                                        1, 10000, SCI_AUTO_ACTIVATE);
        }
        else
        {
            SCI_ChangeTimer(s_zdt_nfc_check_timer, ZDTNFC_CHK_TimerCallback, 10000);
            SCI_ActiveTimer(s_zdt_nfc_check_timer);
        }
    }
    return TRUE;
}

BOOLEAN ZDTNFC_HandleHwCheckEnd(void)
{
    NFC_TRACE("ZDTNFC HWCheckEnd %d",nfc_hw_is_ok());
    if(s_is_nfc_hw_check)
    {
        s_is_nfc_hw_check = 0;
        if(s_nfc_hw_check_laststatus == FALSE)
        {
            //ZDTNFC_HandlePowerOff();
        }
    }
    return TRUE;
}


BOOLEAN ZDTNFC_HandleTimer(void)
{
    NFC_TRACE("ZDTNFC_HandleTimer %d",nfc_hw_is_ok());
    Nfc_Card_Reader(); 
    return TRUE;
}


BOOLEAN ZDTNFC_HandleInt(void)
{
    //NFC_TRACE("ZDTNFC_HandleInt");
    if(nfc_is_running == FALSE)
    {
        return FALSE;
    }
    if(nfc_is_in_sleep)
    {
        return FALSE;
    }
    NFCC_ReceiveData();
    return TRUE;
}

static BOOLEAN nfc_init(void) 
{
    BOOLEAN ret = FALSE;
    if(nfc_is_init == FALSE)
    {
#ifndef ZDT_PCBA_ZC100_HW_V30_SUPPORT
        LDO_TurnOnLDO (LDO_LDO_SDIO);
#endif
        GPIO_NFCIRQCtrl(FALSE);
#ifdef ZDT_PCBA_ZC100_HW_V22_SUPPORT
        GPIO_SetNFC_LDO_SIM_EN(FALSE);
        GPIO_SetNFC_LDO_PVDD_EN(TRUE);
        SCI_Sleep(1500);
#endif
        NFCC_I2C_Init();
        nfc_is_running = TRUE;
        Snci_get_info_data();
        init_buffer();
        //NFC_Firmware_Update();//固件升级
        nfc_is_init = TRUE;
    }
    return ret;
}

void ZDTNFC_TaskEntry(uint32 argc, void *argv)
{	
    zdtnfc_taskid=SCI_IdentifyThread();
    
    NFC_TRACE("ZDTNFC_TaskEntry");
    nfc_init();
    
    while(1) 
    {
        SCI_Sleep(100);
        if(ZDT_NFC_GetSleepStatus() == FALSE)
        {
            NFCC_Init();          //NFC初始化
            Nfc_Card_Run();
        }
        else
        {
            ZDT_NFC_ExitSleepExt();
        }
    }
    
    NFC_TRACE("ZDTNFC_TaskEntry End");
}

void ZDTNFC_RCV_TaskEntry(uint32 argc, void *argv)
{	
    zdtnfc_rcv_taskid=SCI_IdentifyThread();
    
    NFC_TRACE("ZDTNFC_RCV_TaskEntry");
    while(1)
    {
    	xSignalHeaderRec*   sig_ptr = 0; 
        
    	sig_ptr = SCI_GetSignal( zdtnfc_rcv_taskid);
    	switch(sig_ptr->SignalCode)
    	{
                case SIG_ZDT_NFC_INT:
                    {
                        ZDT_NFC_SIG_T *  atc_sig = (ZDT_NFC_SIG_T*)sig_ptr;
                        
                        ZDTNFC_HandleInt();
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                        if(nfc_is_in_sleep == FALSE)
                        {
                            SCI_Sleep(1);
                            if(GPIO_GetNFCIRQ())
                            {
                                NFC_TRACE("ZDTNFC_RCV_TaskEntry IntHandler TimerOut");
                                APP_SendSigTo_RCV_NFC(SIG_ZDT_NFC_INT,PNULL,0);
                            }
                        }

                    }
                    break;
                    
                default:
                    break;

    	}
    	SCI_FREE(sig_ptr);
    }
}

BOOLEAN ZDTNFC_TaskCreate(void)
{
    BOOLEAN res = FALSE;
	if(zdtnfc_rcv_taskid == SCI_INVALID_BLOCK_ID)
	{
		zdtnfc_rcv_taskid = SCI_CreateThread("NFCRCV_THREAD",
			"NFCRCV_QUEUE",
			ZDTNFC_RCV_TaskEntry,
			NULL,
			NULL,
			1024*20,
			100,
			77,	// PRI_NFC_TASK 比MMI 74要高
			SCI_PREEMPT,
			SCI_AUTO_START
			);
        
		NFC_TRACE("ZDTNFC   ZDTNFC_TaskCreate, zdtnfc_rcv_taskid=0x%x", zdtnfc_rcv_taskid);
	}
    
	if(zdtnfc_taskid == SCI_INVALID_BLOCK_ID)
	{
		zdtnfc_taskid = SCI_CreateThread("ZDTNFC_THREAD",
			"ZDTNFC_QUEUE",
			ZDTNFC_TaskEntry,
			NULL,
			NULL,
			1024*20,
			10,
			78,	// PRI_NFC_TASK 比MMI 74要高
			SCI_PREEMPT,
			SCI_AUTO_START
			);
        
		NFC_TRACE("ZDTNFC   ZDTNFC_TaskCreate, zdtnfc_taskid=0x%x", zdtnfc_taskid);
        res = TRUE;
	}
    return res;
}

void ZDTNFC_TaskClose(void)
{

	if(SCI_INVALID_BLOCK_ID != zdtnfc_taskid)
	{
		SCI_TerminateThread(zdtnfc_taskid);
		SCI_DeleteThread(zdtnfc_taskid);
		zdtnfc_taskid = SCI_INVALID_BLOCK_ID;       
	}
	if(SCI_INVALID_BLOCK_ID != zdtnfc_rcv_taskid)
	{
		SCI_TerminateThread(zdtnfc_rcv_taskid);
		SCI_DeleteThread(zdtnfc_rcv_taskid);
		zdtnfc_rcv_taskid = SCI_INVALID_BLOCK_ID;       
	}
    
	NFC_TRACE("ZDTNFC   ZDTNFC_TaskClose");
}

BLOCK_ID ZDTNFC_TaskGetID(void)
{
	return zdtnfc_taskid;
}

BOOLEAN  APP_SendSigTo_NFC(ZDT_NFC_TASK_SIG_E sig_id, const char * data_ptr,uint32 data_len)
{
    uint8 * pstr = NULL;
    ZDT_NFC_SIG_T * psig = PNULL;
    BLOCK_ID cur_taskID=ZDTNFC_TaskGetID();
    
    if(SCI_INVALID_BLOCK_ID == cur_taskID)
    {
        NFC_TRACE("ZDTNFC   APP_SendSigTo_NFC Err Task sig_id=0x%x,data_len=%d",sig_id,data_len);
        return FALSE;
    }
    
    if(data_len != 0 && data_ptr != NULL)
    {
        pstr = SCI_ALLOCAZ(data_len+1);//free it in nfc task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("ZDTNFC   APP_SendTo_ATC Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_NFC_SIG_T), SCI_IdentifyThread());
    psig->len = data_len;
    psig->str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig, cur_taskID);
    return TRUE;
}

BLOCK_ID ZDTNFC_RCV_TaskGetID(void)
{
	return zdtnfc_rcv_taskid;
}

BOOLEAN  APP_SendSigTo_RCV_NFC(ZDT_NFC_TASK_SIG_E sig_id, const char * data_ptr,uint32 data_len)
{
    uint8 * pstr = NULL;
    ZDT_NFC_SIG_T * psig = PNULL;
    BLOCK_ID cur_taskID=ZDTNFC_RCV_TaskGetID();
    
    if(SCI_INVALID_BLOCK_ID == cur_taskID)
    {
        NFC_TRACE("ZDTNFC   APP_SendSigTo_RCV_NFC Err Task sig_id=0x%x,data_len=%d",sig_id,data_len);
        return FALSE;
    }
    
    if(data_len != 0 && data_ptr != NULL)
    {
        pstr = SCI_ALLOCAZ(data_len+1);//free it in nfc task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("ZDTNFC   APP_SendSigTo_RCV_NFC Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_NFC_SIG_T), SCI_IdentifyThread());
    psig->len = data_len;
    psig->str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig, cur_taskID);
    return TRUE;
}

BOOLEAN ZDT_NFC_Hw_IsOn(void)
{
    return g_is_nfc_hw_on;
}

void ZDT_NFC_Set_PowerNoOff(BOOLEAN no_off)
{
    g_is_nfc_not_off = no_off;
}

BOOLEAN ZDT_NFC_Init(void)
{    
    return ZDTNFC_TaskCreate();
}

BOOLEAN ZDT_NFC_Init_Ext(void)
{    
    g_nfc_card_simulation_init_over = 1;
    return ZDTNFC_TaskCreate();
}

int ZDT_NFC_PowerOn(void)
{    
	NFC_TRACE("zdtnfc ZDT_NFC_PowerOn  send SIG_ZDT_NFC_HW_PWON");

    //APP_SendSigTo_NFC(SIG_ZDT_NFC_HW_PWON,PNULL,0);
    return 0;
}

int ZDT_NFC_PowerOff(void)
{    
    //APP_SendSigTo_NFC(SIG_ZDT_NFC_HW_PWOFF,PNULL,0);
    return 0;
}

int ZDT_NFC_UartSend(uint8 * send_buf, uint32 send_len)
{    
    //APP_SendSigTo_NFC(SIG_ZDT_NFC_SEND,send_buf,send_len);
    return 0;
}

int ZDT_NFC_CheckHW_Start(void)
{    
    //APP_SendSigTo_NFC(SIG_ZDT_NFC_HW_CHECK_START,PNULL,0);
    return 0;
}

int ZDT_NFC_CheckHW_End(void)
{    
    //APP_SendSigTo_NFC(SIG_ZDT_NFC_HW_CHECK_END,PNULL,0);
    return 0;
}

int ZDT_NFC_EnterSleepExt(void)
{    
    if(nfc_need_enter_sleep && nfc_is_in_sleep == FALSE)
    {
        NFC_TRACE("---------EnterSleep---------");
        nfc_is_in_sleep = TRUE;
#if defined(ZDT_PCBA_ZC100_HW_V22_SUPPORT)
        GPIO_NFCIRQCtrl(FALSE);
        NFCC_I2C_DeInit();
        GPIO_SetNFC_I2C_Status(FALSE);
        GPIO_SetNFC_LDO_PVDD_EN(FALSE);
        SCI_Sleep(200);
#endif
        return 1;
    }
    return 0;
}

int ZDT_NFC_EnterSleep(void)
{    
    if(nfc_need_enter_sleep != TRUE)
    {
        nfc_need_enter_sleep = TRUE;
    }
    return 0;
}


int ZDT_NFC_ExitSleepExt(void)
{    
    if(nfc_need_enter_sleep == FALSE && nfc_is_in_sleep)
    {
        NFC_TRACE("---------ExitSleep---------");
#if defined(ZDT_PCBA_ZC100_HW_V22_SUPPORT)
        GPIO_SetNFC_I2C_Status(TRUE);
        NFCC_I2C_Init();
        GPIO_SetNFC_LDO_PVDD_EN(TRUE);
        SCI_Sleep(100);
#endif
        nfc_is_in_sleep = FALSE;
        return 1;
    }
    return 0;
}

int ZDT_NFC_ExitSleep(void)
{    
    if(nfc_need_enter_sleep)
    {
        nfc_need_enter_sleep = FALSE;
    }
    return 0;
}

int ZDT_NFC_SetSleepStatus(BOOLEAN is_sleep)
{    
    if(is_sleep)
    {
        ZDT_NFC_EnterSleep();
    }
    else
    {
        //ZDT_NFC_ExitSleep();
    }
    return 0;
}

BOOLEAN ZDT_NFC_GetSleepStatus(void)
{    
    return nfc_is_in_sleep;
}

#endif/*_ZDT_NFC_C*/

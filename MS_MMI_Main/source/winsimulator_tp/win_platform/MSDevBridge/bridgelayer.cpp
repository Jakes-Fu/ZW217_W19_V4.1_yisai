
#include <assert.h>
#include "BridgeLayer.h"
#include "bl_os.h"
#include "cm.h"
#include "cmddef.h"
#include "sci_api.h"
#include "tasks_id.h"

#include "keypad.h"
#include "time.h"

#include "power.h"
#include "tb_hal.h"

#ifdef MSCODE_MOD
//#include "dal_lcd.h"
#include "dal_dcamera.h"
#include "dal_recorder.h"
#include "sfs.h"
#include "dal_display.h"
#include "ref_lcd.h"
#endif

#include "tp_srv.h"
#ifdef PRJ_TDPS_UEIT_MMI
extern "C" {
extern int SCI_SendTracePacket(
							   uint8 *src,  // Message head
							   uint32    len   // Message head len
							   );

extern int SCI_SendLogPacket(uint32 *head_ptr,  // Message head
					  int    head_len,   // Message head len
					  uint32 *body_ptr,  // Message body, maybe NULL
					  int    body_len    // Message bodu len           
					  );
extern int SCI_SendPacket(void * src, int size);
}
#endif
#define MAX_TRACESTR_LEN    400
#define BACKSPACE_CHAR      0x08
#define END_CHAR            0x0D
#define ATC_BUF_SIZE        4096

#define SCI_TRACE_LOW     	SCI_Trace
#define SCI_TRACE_ID		SCI_TraceEx

#define RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))

typedef struct 
{
	volatile int     start_point;       // First data in the buffer.
    volatile int     end_point;         // Last data in the buffer.
    uint8   sio_buf_ptr[ATC_BUF_SIZE];  // Sio buffer address.
} SIO_BUF_S;

typedef struct
{
	int freq;
	int dur;
	int volume;
} beep_info_s;

uint8   gprs_debug;  // 1: GPRS_DEBUG  0: not defined GPRS_DEBUG.

static UINT         svrID;
static char         atc_end_ch1;        // End char of ATC;
static char         atc_end_ch2;        // End char of ATC;
static char         atc_bs_ch;          // Backspace char of ATC
static uint32       cur_atc_frame_size; // Current Enqueued atc frame size
static ATC_CALLBACK AT_callback_ind;    // Pointer to the AT callback
static SIO_BUF_S    atc_buf;

static uint32       user_port_mode;                                // ATC_MODE, DATA_MODE;

static MSK_MSSIM_FUN_LIST g_MSSimFunList = { 0 };

static int  ring_remove( SIO_BUF_S *pRing );
static int  ring_add( SIO_BUF_S *pRing, int ch );
static int  ring_space( SIO_BUF_S *pRing );
static void SendResToCS(uint16 seq_num);

//static DRECORDER_START_RECORD_T s_record = {0};

HANDLE      g_hSIOLock;

typedef struct KEY_NAME_MAP_TAG
{
    char*   pszKeyName;
    int     nKeyCode;

} KEY_NAME_MAP;

static KEY_NAME_MAP g_aKeyNameTable[] =
    {
        { "KEY_0",          SCI_VK_0 },
        { "KEY_1",          SCI_VK_1 },
        { "KEY_2",          SCI_VK_2 },
        { "KEY_3",          SCI_VK_3 },
        { "KEY_4",          SCI_VK_4 },
        { "KEY_5",          SCI_VK_5 },
        { "KEY_6",          SCI_VK_6 },
        { "KEY_7",          SCI_VK_7 },
        { "KEY_8",          SCI_VK_8 },
        { "KEY_9",          SCI_VK_9 },
        { "KEY_*",          SCI_VK_STAR },
        { "KEY_#",          SCI_VK_POUND },
        { "KEY_CANCEL",     SCI_VK_CANCEL },
        { "KEY_CALL",       SCI_VK_CALL },
        { "KEY_POWER",      SCI_VK_POWER },
        { "KEY_UP",         SCI_VK_UP },
        { "KEY_DOWN",       SCI_VK_DOWN },
        { "KEY_LEFT",       SCI_VK_LEFT },
        { "KEY_RIGHT",      SCI_VK_RIGHT },
    };


static uint16 bridge_decode_tmp_msg(uint8 **dest, uint8 *src)
{
	uint16       dest_len;           /* output buffer length*/
	uint16       src_len;            /* source buffer length*/
	const uint8  *src_ptr;           /* source buffer pointer*/
    uint8 *      dest_ptr;           /* dest buffer pointer*/
    int          i;

    /* Check if exist End Flag.*/
	src_ptr   = src;
	/* Frame error, no start flag or end flag.*/
	if (FLAG_BYTE != *src_ptr)
	{
		*dest = SCI_NULL;
		return SIO_NO_END_FLAG;
	}

	dest_len   = 0;
	src_len    = 0;
	src_ptr++;
    
	i = 0;
    /* Get the total size to be allocated.*/
	while (FLAG_BYTE != *(src_ptr + i))
    {
        switch (*(src_ptr + i))
        {
		case ESCAPE_BYTE:
			i += 2;
			dest_len++;
			break;
		default:
			i++;
			dest_len++;
			break;
		}
    }

	src_len = dest_len;

	/* Allocate meomory for decoded message*/
    /* Here add CM_PAD_SIZE to enable all signals have the same header.*/
    dest_ptr = (uint8 *) malloc(dest_len);

    /* Memory Free fail.*/
    assert(dest_ptr != SCI_NULL);

    *dest    = dest_ptr;
    /* Let the size to be NULL.
       We just copy message from now!
    */
//    dest_ptr += CM_PAD_SIZE;
	dest_len = 0;

	i = 0;
    /* Do de-escape.*/
	while (FLAG_BYTE != *(src_ptr + i))
	{
		switch (*(src_ptr + i))
		{
		case ESCAPE_BYTE:
			i++;
			*(dest_ptr + dest_len++) = *(src_ptr + i++) ^ COMPLEMENT_BYTE;
			break;
		default:
			*(dest_ptr + dest_len++) = *(src_ptr + i++);
			break;
		}
	}
	return (dest_len);
}

void BL_RegisterMSSimFunList(const MSK_MSSIM_FUN_LIST *pFunList)
{
#ifndef UNIT_TEST
    memcpy(&g_MSSimFunList, pFunList, sizeof(MSK_MSSIM_FUN_LIST));
#endif
}

void BL_StartRTOS()
{
    SCI_KernelEnter();
}

void BL_StopRTOS()
{
    void* pSig = NULL;

    SCI_THREAD_CONTEXT_SAVE;
    SCI_CREATE_SIGNAL(pSig, KPD_SOFT_POWER_OFF, sizeof(xSignalHeaderRec), 0);
    SCI_SEND_SIGNAL(pSig, KPDSVR);
    SCI_THREAD_CONTEXT_RESTORE;
}

int BL_GetKeyCodeByName(const char *lpszKeyName)
{
    int nKeyCount = sizeof( g_aKeyNameTable ) / sizeof( KEY_NAME_MAP );
    
    for ( int i = 0; i < nKeyCount; i++ )
    {
        if ( 0 == strcmp(g_aKeyNameTable[i].pszKeyName, lpszKeyName) )
        {
            return g_aKeyNameTable[i].nKeyCode;
        }
    }

    return -1; // -1 indicates invalid key code
}

// Send the key code to RTOS
void BL_SendKeycodeToRTOS(WORD wKeyCode, UINT msg)
{
	KPDSVR_SIG_T * m_msg;

    // Handle keys when MS is in POWER_ON state .
    if (POWER_MS_STATUS_POWER_ON == POWER_GetMsStatus())
    {
    	m_msg = (KPDSVR_SIG_T *) SCI_ALLOC(sizeof(KPDSVR_SIG_T));

    	m_msg->Sender     = 0;
	    m_msg->SignalCode = msg;
    	m_msg->SignalSize = sizeof(KPDSVR_SIG_T);
	    m_msg->Pre        = SCI_NULL;
    	m_msg->Suc        = SCI_NULL;

	    m_msg->key = wKeyCode;

        // Save the thread context first
        SCI_THREAD_CONTEXT_SAVE
        // Send message
#ifdef PRJ_TDPS_UEIT_MMI        
		if (svrID != 0)
#endif			
    		SCI_SendSignal((xSignalHeader) m_msg, svrID);
        // Restore the thread context
        SCI_THREAD_CONTEXT_RESTORE

    }
}

// Send touchpanel key code to rtos
// Beijing team provide
void BL_SendTPcodeToRTOS(WORD xKeyCode, WORD yKeyCode,UINT msg)
{
#ifdef MSCODE_MOD
	TPDSVR_SIG_T * m_msg;

    // Handle keys when MS is in POWER_ON state .
    if (POWER_MS_STATUS_POWER_ON == POWER_GetMsStatus())
    {
        m_msg = (TPDSVR_SIG_T *) SCI_ALLOC(sizeof(TPDSVR_SIG_T));
    	m_msg->Sender     = 0;
	    m_msg->SignalCode = msg;
    	m_msg->SignalSize = sizeof(TPDSVR_SIG_T);
	    m_msg->Pre        = SCI_NULL;
    	m_msg->Suc        = SCI_NULL;

	    m_msg->x_key = xKeyCode;
	    m_msg->y_key = yKeyCode;


        // Save the thread context first
        SCI_THREAD_CONTEXT_SAVE
        // Send message
    	SCI_SendSignal((xSignalHeader) m_msg, P_TP);
        // Restore the thread context
        SCI_THREAD_CONTEXT_RESTORE
	    //SCI_SendEventToClient( RESERVED_SERVICE1, (uint32)msg, m_msg);
    }
#endif
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   BL_SendMsgToRTOS
// AUTHOR:          Richard Yang
// INPUT:           pBuffer     ----    Buffer starting address
//                  nLength     ----    Buffer size
// RETURN:          None
// DESCRIPTION:     Parse the buffer and send each message to RTOS queue
// ----------------------------------------------------------------------------
UINT BL_SendMsgToRTOS(BYTE *pBuffer, int nLength)
{
    uint8 *pMsgBuf;      // Block buffer which contains ONE message
    uint16 len;

    // Handle when MS is in POWER_ON state .
    if (POWER_MS_STATUS_POWER_ON == POWER_GetMsStatus())
    {
        // Copy the message block to a new allocated memory
        len = bridge_decode_msg(&pMsgBuf, pBuffer);
        // Send the message to RTOS queue
        return bridge_queue_send(SIO, pMsgBuf, len, SCI_NO_WAIT);      
    }
    else
    {
		uint8  src[8];
        uint8  *dest;

        len = bridge_decode_tmp_msg(&pMsgBuf, pBuffer);

		memcpy(src, pMsgBuf, sizeof(MSG_HEAD_T));
        free(pMsgBuf);

		if (((MSG_HEAD_T *) src)->type <= MSG_REQ_END 
            && ((MSG_HEAD_T *) src)->type >= MSG_REQ_START)
		{
            ((MSG_HEAD_T *) src)->len     = 8;
            ((MSG_HEAD_T *) src)->type    = MSG_SEND_OK;
            ((MSG_HEAD_T *) src)->subtype = 0;

            dest = (uint8 *) malloc(len * 2);

            *dest = 0x7E;
            len = bridge_encode_msg(dest + 1, src, 8, END_FRAME);
            sio_flush_tx(dest, len + 1);

            return SCI_SUCCESS;
		}
		// @Xueliang.Wang added(2003-03-18)
		else if (((MSG_HEAD_T *) src)->type <= MSG_REQ_REP_END 
            && ((MSG_HEAD_T *) src)->type >= MSG_REQ_REP_START)
		{
		    return SCI_SUCCESS;
		}
		else
		{
            // @Richard There is some unknown case?
            // I am not sure so assert here if we found such case.
            assert(0);
			return SCI_ERROR;
        }  
    }
    
}

void BL_Free(void *pBuffer)
{
    free(pBuffer);
}

BOOL BL_IsPowerOff()
{
    return (POWER_MS_STATUS_POWER_OFF == POWER_GetMsStatus()) ? TRUE : FALSE;
}


#ifdef _CONTROL_PANEL
extern "C"
{
extern void PS_RegisterMsgHandler(int (*fpCallback)(xSignalHeader sig));
}
#endif // _CONTROL_PANEL

void BL_CPSetCallbackFunToPS(int (*fun_ptr)(xSignalHeader sig))
{
#ifdef _CONTROL_PANEL

    // PS_RegisterMsgHandler() will verify the pointer 'fun_ptr'
    PS_RegisterMsgHandler(fun_ptr);

#endif // _CONTROL_PANEL
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   sio_flush_tx
// AUTHOR:          Liu Jun
// INPUT:           src     --  message starting address
//                  size    --  message size
// RETURN:          None
// DESCRIPTION:     RTOS informs MSSim to forward message to Comm Sock
// ----------------------------------------------------------------------------
uint32 sio_flush_tx(unsigned char *src, uint32 size)
{
    return g_MSSimFunList.fp_MSSIM_sio_flush_tx(src, size);
}

uint32 sio_at_flush_tx(unsigned char *src, uint32 size)
{
    return g_MSSimFunList.fp_MSSIM_sio_at_flush_tx(src, size);
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   sio_free
// AUTHOR:          Liu Jun
// INPUT:           buffer_ptr  -- memory buffer to be freed
// RETURN:          void
// DESCRIPTION:     This function is called by RTOS to free useless buffers.
//                  It should be able to expend for future requirements.
// ----------------------------------------------------------------------------
void sio_free(unsigned char *buffer_ptr)
{
    g_MSSimFunList.fp_MSSIM_sio_free(buffer_ptr);
}

/*
** Returns the number of characters that can be added to this ring before it becomes full.
*/
static int ring_space( SIO_BUF_S *pRing )
{
  int ret = pRing->start_point - pRing->end_point - 1;
  return (ret >= 0) ? ret : (ret + ATC_BUF_SIZE);
}

/*
** Add 'ch' to this ring.
** Returns -1 if there is not enough room.
** Returns 0 otherwise.
*/
static int ring_add( SIO_BUF_S *pRing, int ch )
{
    if ( ring_space( pRing ) == 0 ) {
        return -1;
    }

    pRing->sio_buf_ptr[pRing->end_point++] = ch;

    if ( pRing->end_point >= ATC_BUF_SIZE )
        pRing->end_point = 0;

    return 0;
}

/*
** Remove a character from this ring.
** Returns -1 if there are no characters in this ring.
** Returns the character removed otherwise.
*/
static int ring_remove( SIO_BUF_S *pRing )
{
    int ret;

    if ( pRing->end_point == pRing->start_point ) {
        return -1;
    }

    ret = pRing->sio_buf_ptr[pRing->start_point++];

    if ( pRing->start_point >= ATC_BUF_SIZE )
        pRing->start_point = 0;

    return ret;
}

uint32 bridge_queue_send(BLOCK_ID src_ID, VOID *source_ptr, uint16 size, ULONG wait_option)
{

    // Save the thread context first
    SCI_THREAD_CONTEXT_SAVE

    MSG_HEAD_T * temp;
    temp = (MSG_HEAD_T *) source_ptr;
    unsigned char * temp_ptr = (unsigned char *) source_ptr;
    uint32 status = 0;

#ifndef UNIT_TEST
    // We need to send it to ATC
    if (temp->type == ATC_CMD_R)
    {
        cur_atc_frame_size = 0;

        for (int i = sizeof(MSG_HEAD_T); i < size; i++)
        {
            if ( ring_add( &atc_buf, temp_ptr[i] ) == -1 ) 
            {
                continue;
            }
            cur_atc_frame_size++;
        }

        assert(NULL != AT_callback_ind);
        AT_callback_ind(cur_atc_frame_size);
        
        SendResToCS(temp->seq_num);

        free(source_ptr);
    }
    else if(temp->type == COM_DATA_R)//串口数据
    {
        // Send the message to RTOS queue
        //status = SCI_SendMessage(src_ID, CM, source_ptr, SCI_PRI_NORMAL, SCI_NO_WAIT);
		//if (SCI_SUCCESS != status)
        //{
        //    SCI_FREE(source_ptr);
        //}
		uint32		sig_size	= size + sizeof(xSignalHeaderRec);
		xSignalHeader	sig_ptr = (xSignalHeader)SCI_ALLOC(sig_size);

		sig_ptr->Sender			= src_ID;
		sig_ptr->SignalSize		= sig_size;
		memcpy((void *)(sig_ptr + 1), source_ptr, size);
		free(source_ptr);

		SCI_SendSignal(sig_ptr, P_DUMMY);
    }
    else 
    {
        // Send the message to RTOS queue
        //status = SCI_SendMessage(src_ID, CM, source_ptr, SCI_PRI_NORMAL, SCI_NO_WAIT);
		//if (SCI_SUCCESS != status)
        //{
        //    SCI_FREE(source_ptr);
        //}
		uint32		sig_size	= size + sizeof(xSignalHeaderRec);
		xSignalHeader	sig_ptr = (xSignalHeader)SCI_ALLOC(sig_size);

		sig_ptr->Sender			= src_ID;
		sig_ptr->SignalSize		= sig_size;
		memcpy((void *)(sig_ptr + 1), source_ptr, size);
		free(source_ptr);

		SCI_SendSignal(sig_ptr, CM);
    }
    // Restore the thread context
    SCI_THREAD_CONTEXT_RESTORE
#endif
    return status;
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   SendResponseToChannelServer
// AUTHOR:          Richard Yang
// INPUT:           seq_num:          Seqeunce number for the message frame.
//                  
// RETURN:          None
// DESCRIPTION:     This function Send receive OK response to Channel server.
// ----------------------------------------------------------------------------
static void SendResToCS(uint16 seq_num)
{
    static MSG_HEAD_T send_data=
    {
        0,           // seq_num
        0x08,        // size
        MSG_SEND_OK, // Main type
        0            // subtype
    };
    
    send_data.seq_num  = seq_num;
    
    SIO_SendPacket((uint8 *) &send_data, sizeof(send_data));
}

int SIO_AT_SendPacket(void * src, int size)
{
    assert(src != SCI_NULL);
    assert(size <= MAX_PACKET_SIZE);

    // If the status of MS is not POWER ON, then return.
    if (POWER_MS_STATUS_POWER_ON != POWER_GetMsStatus())
    {
        return SCI_SUCCESS;
    }

    SCI_DisableIRQ();

    sio_at_flush_tx((unsigned char *)src, size);

    SCI_RestoreIRQ();

    return SCI_SUCCESS;
}

int SIO_SendPacket(void * src, int size)
{
    uint8 * dest;
    uint16 len;

    assert(src != SCI_NULL);
    assert(size <= MAX_PACKET_SIZE);

    // If the status of MS is not POWER ON, then return.
    if (POWER_MS_STATUS_POWER_ON != POWER_GetMsStatus())
    {
        return SCI_SUCCESS;
    }

    SCI_DisableIRQ();
#ifndef PRJ_TDPS_UEIT_MMI
    dest = (uint8 *) malloc(size * 2);
    // There may be some problem when use malloc, may cause dead lock.
//    dest = (uint8 *) SCI_ALLOC(size * 2);

    *dest = 0x7E;
    len = bridge_encode_msg(dest + 1, (uint8 *) src, (uint16) size, END_FRAME);
	
    sio_flush_tx(dest, len + 1);
#else
	SCI_SendPacket( src, size );
#endif
    SCI_RestoreIRQ();

    return SCI_SUCCESS;
}

// @Xueliang.Wang added : 2003-02-26
int SIO_SendTracePacket(void *src, int len)
{
    uint32  status = -1;
#ifndef PRJ_TDPS_UEIT_MMI
	status = SIO_SendLogPacket((uint32 *)src, len, SCI_NULL, 0);
#else
    status = SCI_SendTracePacket( (uint8*)src, len);//SIO_SendLogPacket((uint32 *)src, len, SCI_NULL, 0);
#endif   
    return status;
}

int SIO_SendLogPacket(uint32 *head_ptr, int head_len, uint32 *body_ptr, int body_len)
{
    uint16  size;
    uint32  status = -1;
    uint16  dest_len;
    uint16  send_len;
    unsigned char * sio_buffer;
#ifndef PRJ_TDPS_UEIT_MMI
#ifndef UNIT_TEST
	static int seqnum = 0;
    const unsigned char data[] = 
    {
        0x7e,        // Begin Flag
        0, 0,        // Seq_num 
        0, 0,        // dummy
        0, 0,        //  Warning:
                     //  Len  Because we can not set this value when we add information here!
                     //       We have to set it to ZERO, channel server and logel will care it more!
        DIAG_LOG_A,  // Type
        0,           // Subtype 
        0, 0         // lost_num
    };

    HANDLE hSIOLock;
	DWORD  RetCode;

    // If the status of MS is not POWER ON, then return.
    if (POWER_MS_STATUS_POWER_ON != POWER_GetMsStatus())
    {
        return 0;
    }

    SCI_DisableIRQ();

    hSIOLock = g_MSSimFunList.fp_MSSIM_GetSIOLockEvent();
	RetCode = WaitForSingleObject(hSIOLock, 0);

	if(RetCode != WAIT_OBJECT_0)
	{
        // If there is some task sending message, we have to ingnore this message.
        SCI_RestoreIRQ();
		return 0;
	}

	ResetEvent(hSIOLock);


    assert(head_ptr != SCI_NULL);
    assert(head_len != 0);

    /* Get the total size of the message, in order to use memcpy, 
       we should make all message align by 4 byes.*/
    size = head_len + body_len;

    /* There may be some problem when use malloc, may cause dead lock.*/
    sio_buffer = (uint8 *) malloc(size * 2 + sizeof(data));
//    sio_buffer = (uint8 *) SCI_ALLOC(size * 2 + sizeof(data));

    assert(sio_buffer != NULL);

    memcpy(sio_buffer, data, sizeof(data));
	//apple change for sequence number
	++seqnum;
	memcpy( sio_buffer+1, &seqnum, 4 );


    /* Set log message length.*/
//    *((uint16 *) head_ptr) = size - 2;
    
    send_len = sizeof(data);
    if (body_ptr != SCI_NULL)
    {
        dest_len = bridge_encode_msg(sio_buffer + sizeof(data) , (uint8 *)head_ptr, head_len, MED_FRAME);
        send_len += dest_len;
        dest_len = bridge_encode_msg(sio_buffer + send_len, (uint8 *) body_ptr, size - head_len, END_FRAME);
        send_len += dest_len;
    }
    else
    {
        dest_len = bridge_encode_msg(sio_buffer + sizeof(data), (uint8 *)head_ptr, size, END_FRAME);
        send_len += dest_len;
    }

    assert(send_len < size * 2 + sizeof(data));
    sio_flush_tx(sio_buffer, send_len);

    SetEvent(hSIOLock);

    SCI_RestoreIRQ();
#endif
#endif

#ifndef PRJ_TDPS_UEIT_MMI
	return 0;
#else
    return SCI_SendLogPacket( head_ptr,head_len, body_ptr, body_len);//0;
#endif
}

uint16 bridge_encode_msg(uint8 * dest, uint8 * src, uint16 size, uint32 flag)
{
    uint16 i;
    uint16 dest_len;

    dest_len = 0;
    for (i = 0; i < size; i++)
	{
		switch (*(src+i))
		{
		case FLAG_BYTE:
		case ESCAPE_BYTE:
			*(dest + dest_len++) = ESCAPE_BYTE;
			*(dest + dest_len++) = *(src + i) ^ COMPLEMENT_BYTE;
			break;
		default:
			*(dest + dest_len++) = *(src + i);
			break;
		}
	}
    if (flag == END_FRAME)
    {
        *(dest + dest_len++) = 0x7E;
    }
    return dest_len;
}

uint16 bridge_decode_msg(uint8 **dest, uint8 *src)
{
	uint16       dest_len;           /* output buffer length*/
	uint16       src_len;            /* source buffer length*/
	const uint8  *src_ptr;           /* source buffer pointer*/
    uint8 *      dest_ptr;           /* dest buffer pointer*/
    int          i;

    /* Check if exist End Flag.*/
	src_ptr   = src;
	/* Frame error, no start flag or end flag.*/
	if (FLAG_BYTE != *src_ptr)
	{
		*dest = SCI_NULL;
		return SIO_NO_END_FLAG;
	}

	dest_len   = 0;
	src_len    = 0;
	src_ptr++;
    
	i = 0;
    /* Get the total size to be allocated.*/
	while (FLAG_BYTE != *(src_ptr + i))
    {
        switch (*(src_ptr + i))
        {
		case ESCAPE_BYTE:
			i += 2;
			dest_len++;
			break;
		default:
			i++;
			dest_len++;
			break;
		}
    }

	src_len = dest_len;

	/* Allocate meomory for decoded message*/
    /* Here add CM_PAD_SIZE to enable all signals have the same header.*/
    //dest_ptr = (uint8 *) SCI_ALLOC(dest_len);
	dest_ptr = (uint8 *) malloc(dest_len);

    /* Memory Free fail.*/
    assert(dest_ptr != SCI_NULL);

    *dest    = dest_ptr;
    /* Let the size to be NULL.
       We just copy message from now!
    */
//    dest_ptr += CM_PAD_SIZE;
	dest_len = 0;

	i = 0;
    /* Do de-escape.*/
	while (FLAG_BYTE != *(src_ptr + i))
	{
		switch (*(src_ptr + i))
		{
		case ESCAPE_BYTE:
			i++;
			*(dest_ptr + dest_len++) = *(src_ptr + i++) ^ COMPLEMENT_BYTE;
			break;
		default:
			*(dest_ptr + dest_len++) = *(src_ptr + i++);
			break;
		}
	}
	return (dest_len);
}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   bridge_power_off
// AUTHOR:          Liu Jun
// INPUT:           hTimer      ---- Handle of the timer management thread in RTOS
//                  hTimerInt   ---- Handle of the timer interrupt thread in RTOS
// RETURN:          None
// DESCRIPTION:     RTOS call this functoin to handle the power-off event
//                  This function will be called by RTOS timer thread
// ----------------------------------------------------------------------------
void bridge_power_off(HANDLE hTimer, HANDLE hTimerInt)
{
#ifndef UNIT_TEST
    g_MSSimFunList.fp_MSSIM_bridge_power_off(hTimer, hTimerInt);
#endif
}


//
// Main LCD Panel
// 
#ifdef MSCODE_MOD
//
/*
typedef struct block_tag
{
	uint16      is_enable;  //1: enable this block;0: disable this block
	uint32      *mem_ptr;
//	LCD_STATE_E state;      //{idle,ready,busy,error}	
	BLOCKCFG_T  cfg;
}BLOCK_T;
*/
	  int   LCD_WIDTH	   =    240;//default
	  int   LCD_HEIGHT	   =    320;//default

const int   LCD_WIDTH_MAX  =	480;//max width
const int   LCD_HEIGHT_MAX =	480;//max height

const int   MAX_LCD_BLOCK_NUM =6;//0~5

//lcd buffer
LOCAL  BLOCK_T   s_block_info[MAX_LCD_ID][MAX_LCD_BLOCK_NUM];
LOCAL  uint16 s_lcd_block0_buf[LCD_WIDTH_MAX * LCD_HEIGHT_MAX];
LOCAL  uint16 s_lcd_block1_buf[LCD_WIDTH_MAX * LCD_HEIGHT_MAX];
LOCAL  uint16    s_lcd_buf_tmp[LCD_WIDTH_MAX * LCD_HEIGHT_MAX];
LOCAL  BOOLEAN s_is_direct_draw = FALSE;

#endif

uint32 LCD_GetPanelSize()
{
    SCI_DisableIRQ();
    uint32 size = g_MSSimFunList.fp_MSSIM_LCD_GetPanelSize();
    SCI_RestoreIRQ();
    return size;
}

//@hongliang.xin 2010-3-17 add "extern "C"" and "LCD_DIRECT_E"
extern "C"
ERR_LCD_E  LCD_SetDirection(
                                LCD_ID_E lcd_id,//ID of lcd to operate
	                            LCD_DIRECT_E lcd_angle                   
	                          )
{
	ERR_LCD_E ret = ERR_LCD_NONE;

    SCI_DisableIRQ();

	if (1 == lcd_angle || 3 == lcd_angle)
	{
		ret = ( ERR_LCD_E )g_MSSimFunList.fp_MSSIM_Rotate_Screen( 3 );
	}
	else
	{
		ret = ( ERR_LCD_E )g_MSSimFunList.fp_MSSIM_Rotate_Screen( 0 );
	}
 
    SCI_RestoreIRQ();

    return ret;
}


ERR_LCD_E  LCD_GetInfo(
					  LCD_ID_E  lcd_id,			//id of lcd to operate
					  LCD_INFO_T *lcd_info_ptr	//lcd information struct pointer
					  )
{
    //@ Liu Kai 2004-7-1
    SCI_DisableIRQ();
    ERR_LCD_E ret = ( ERR_LCD_E )g_MSSimFunList.fp_MSSIM_LCD_GetInfo( lcd_id,lcd_info_ptr );
#ifdef MSCODE_MOD
	//apple gao: resolve the sub lcd showing problem
	if( lcd_id == MAIN_LCD_ID )
	{
		lcd_info_ptr->lcdbuff_ptr = s_lcd_block1_buf;
	}
#endif    
    SCI_RestoreIRQ();

    return ret;
}

//james add for direct draw
extern "C" void LCD_SetDirectDraw( BOOLEAN is_direct )
{
    s_is_direct_draw = is_direct;
}

extern "C" BOOLEAN LCD_IsDirectDraw( void )
{
    return s_is_direct_draw;
}

extern "C" void* LCD_GetMSSIMBuffer( void )
{
    LCD_INFO_T lcd_info = {0};

    SCI_DisableIRQ();
    g_MSSimFunList.fp_MSSIM_LCD_GetInfo( 0, &lcd_info );
    SCI_RestoreIRQ();

    return lcd_info.lcdbuff_ptr;
}
//end of james add for direct draw

void LCD_SetPixelColor( WORD x, WORD y, WORD color )
{
    SCI_DisableIRQ();
    g_MSSimFunList.fp_MSSIM_LCD_SetPixelColor( x, y, color );
    SCI_RestoreIRQ();
}

extern "C"
WORD LCD_GetPixelColor( WORD x, WORD y )
{
    SCI_DisableIRQ();
    uint16 color = g_MSSimFunList.fp_MSSIM_LCD_GetPixelColor( x, y );
    SCI_RestoreIRQ();
    return color;
}

extern "C"
void LCD_DrawPixel( WORD x, WORD y, WORD color )
{
    SCI_DisableIRQ();
    g_MSSimFunList.fp_MSSIM_LCD_DrawPixel( x, y, color );
    SCI_RestoreIRQ();
}

#ifdef MSCODE_MOD

//#define SWAP_U16_ENDIAN(x) ((x >> 8) | ((x & 0xff) << 8))
#define GET_R(x) ((x & 0xf800) >> 11)
#define GET_G(x) ((x & 0x07e0) >> 5)
#define GET_B(x) (x & 0x001f)
#define BOMBINE_RGB(r, g, b) (((r & 0x1f) << 11) | ((g & 0x3f) << 5) | (b & 0x1f))
#define RGB555TO565(cr) (((cr & 0x7fe0) << 1) | (cr & 0x1f))

#define RGB888_TO_RGB565(rgb)  ((((rgb) >>8) & 0xf800) |(((rgb) >> 3) & 0x1f) |  ( ( ((rgb)>>0x05)&0x07e0)) ) 

LOCAL void CombinLCDBlock(void)	//合并LCD BLOCK的内容到主BUFFER
{
	int i = 0;
	int w = 0;
	int h = 0;
	int priority[MAX_LCD_BLOCK_NUM] = {0};	//从低到高

	BLOCK_T		*block_ptr = 0;
	BLOCKCFG_T	*cfg_ptr = 0;
	LCD_INFO_T  lcd_info = {0};
	
	uint16		*lcd_buf_ptr = 0;
	uint16		*lcd_buf_real_ptr = 0;
	uint16		*block0_buf_ptr = 0;
	uint16		lcd_width = 0;
	uint16		lcd_height = 0;
	uint16		cr_old = 0;
	uint16		cr_new = 0;
	uint16		cr_3 = 0;

	uint16		alpha = 0;
	uint8		r0, g0, b0;
	uint8		r1, g1, b1;
	uint8		r2, g2, b2;
	double		a_f = 0;

    uint16      block_width;
    uint16      block_height;
    uint16      block_buffer_start = 0;

	int         nStart = 2;
    
    if ( LCD_IsDirectDraw() )
    {
        return;
    }
	
	g_MSSimFunList.fp_MSSIM_LCD_GetInfo(0, &lcd_info);
	lcd_buf_real_ptr = (uint16 *)lcd_info.lcdbuff_ptr;
	lcd_width   = lcd_info.lcd_width;
	lcd_height  = lcd_info.lcd_height;
	SCI_MEMSET(s_lcd_buf_tmp,0,LCD_WIDTH_MAX * LCD_HEIGHT_MAX*2);

	lcd_buf_ptr = s_lcd_buf_tmp;

	if(s_block_info[0][0].is_enable)
	{
		block0_buf_ptr = (uint16 *)s_block_info[0][0].mem_ptr;
		block_width = s_block_info[0][0].cfg.end_x - s_block_info[0][0].cfg.start_x + 1;
		block_height = s_block_info[0][0].cfg.end_y - s_block_info[0][0].cfg.start_y + 1;
		block_buffer_start = s_block_info[0][0].cfg.start_x + s_block_info[0][0].cfg.start_y*LCD_WIDTH;

		if (0 == s_block_info[0][0].cfg.resolution) ////rgb888
		{			
			uint32 *src_buf_ptr = PNULL;
			uint16 *dst_buf_ptr = PNULL;

			src_buf_ptr = (uint32*)block0_buf_ptr;
			dst_buf_ptr = (uint16*)lcd_buf_ptr+block_buffer_start;
			for (i = 0; i < (lcd_width * lcd_height-block_buffer_start); i++)
			{
				*dst_buf_ptr = RGB8882RGB565(*src_buf_ptr);
				src_buf_ptr++;
				dst_buf_ptr++;
			}
		}
		else
		{
			SCI_MEMCPY(lcd_buf_ptr+block_buffer_start, block0_buf_ptr, (lcd_width * lcd_height-block_buffer_start) * 2);
		}
	}

	for (i = nStart; i < MAX_LCD_BLOCK_NUM; i++)
	{
		block_ptr = &s_block_info[0][i];
		cfg_ptr = &block_ptr->cfg;
		if(i != 2)//?????
		{
			block_ptr->is_enable = 0;			
		}
		if (block_ptr->is_enable)
		{
			if (cfg_ptr->resolution == 2)
			{
				for (h = cfg_ptr->start_y; h <= cfg_ptr->end_y; h++)
				{
					for (w = cfg_ptr->start_x; w <= cfg_ptr->end_x; w++)
					{					
						cr_new = *((uint16 *)block_ptr->mem_ptr + lcd_width * (h-cfg_ptr->start_y) + (w-cfg_ptr->start_x));
						if (cr_new != cfg_ptr->colorkey)
						{									
							*(lcd_buf_ptr + lcd_width * h + w) = cr_new;																	
						}	
					}
				}
			}
			else if (cfg_ptr->resolution == 0)
			{
				uint32 src_value;
				uint32 alpha;
				uint32 dst_r;
				uint32 dst_g;
				uint32 dst_b;
				uint32 *src_ptr = (uint32 *)(block_ptr->mem_ptr);
                uint32 src1_value;
				uint16 *dst_cur_ptr = 0;
				uint32 layer_alpha = cfg_ptr->alpha;

				for (h = cfg_ptr->start_y; h <= cfg_ptr->end_y; h++)
				{
					for (w = cfg_ptr->start_x; w <= cfg_ptr->end_x; w++)
					{
						src_value = *((uint32 *)src_ptr + lcd_width * (h-cfg_ptr->start_y) + (w-cfg_ptr->start_x));
                        alpha = (src_value>>24);
                        if(255 != layer_alpha)
                        {
                            alpha = ((alpha *layer_alpha)>>8);
                        }
                        src_value = RGB888_TO_RGB565(src_value);
                        dst_cur_ptr = (lcd_buf_ptr + lcd_width * h + w);
                        
                        if(s_block_info[0][0].is_enable)
                        {
                            //should modify because block0 has already copied to dst_cur_ptr.
                            src1_value = *(block0_buf_ptr + lcd_width * h + w - block_buffer_start);

                            if(0 == alpha)
                            {         
                                *dst_cur_ptr = src1_value;
                            }
                            else if(255 == alpha)
                            {
                                *dst_cur_ptr = src_value;
                            }
                            else
                            {
                                uint8 n;
                                uint32 result = 0;
                                uint32 img_color;
                                uint32 bk_color;

                                n = (alpha + 1) >> 3;
                                img_color = src_value;   
                                bk_color = src1_value;
                                img_color = (img_color | (img_color << 16)) & 0x7e0f81f;
                                bk_color = (bk_color | (bk_color << 16)) & 0x7e0f81f;
                                result = ((((img_color - bk_color) * n) >> 5) + bk_color) & 0x7e0f81f;
                                *dst_cur_ptr++ = ((result & 0xffff) | (result >> 16));

                            }
                        }
                        else
                        {
                            if(0 == alpha)
                            {
                                *dst_cur_ptr = 0;//*dst_cur_ptr = src1_value;
                            }
                            else if(255 == alpha)
                            {
                                *dst_cur_ptr = src_value;
                            }
                            else
                            {
                                uint8 n;
                                uint32 result = 0;
                                uint32 img_color;
                                uint32 bk_color;

                                n = (alpha + 1) >> 3;
                                img_color = src_value;
                                bk_color = 0;
                                img_color = (img_color | (img_color << 16)) & 0x7e0f81f;
                                bk_color = (bk_color | (bk_color << 16)) & 0x7e0f81f;
                                result = ((((img_color - bk_color) * n) >> 5) + bk_color) & 0x7e0f81f;
                                *dst_cur_ptr++ = ((result & 0xffff) | (result >> 16));

                            }

                        }

					}
				}
			}
			else
			{
				SCI_ASSERT(0);
			}
		}
	}

	SCI_MEMCPY(lcd_buf_real_ptr,lcd_buf_ptr,lcd_width*lcd_height*2);
}

#endif

PUBLIC ERR_LCD_E  LCD_InvalidateRect(
						  		LCD_ID_E lcd_id,//id of lcd to operate
						  		uint16 left,	//left of the windows to be refreshed.
						  		uint16 top, 	//top of the widonws to be refreshed.
						  		uint16 right, 	//right of the windows to be refreshed.
						  		uint16 bottom	//bottom of the windows to be refreshed.
						  		)
{
#ifdef MSCODE_MOD
    SCI_ASSERT(lcd_id < MAX_LCD_ID);
    SCI_ASSERT((left <= right) && (top <= bottom));
    
    if (lcd_id==MAIN_LCD_ID)
    {
        SCI_DisableIRQ();
        
        CombinLCDBlock();
#else        
	if (lcd_id==MAIN_LCD_ID)
	{
		SCI_DisableIRQ();
#endif
		g_MSSimFunList.fp_MSSIM_LCD_InvalidateRect( left, top, right, bottom );
		SCI_RestoreIRQ();
	}
	else
	{
		if (lcd_id==SUB_LCD_ID)
		{
			SCI_DisableIRQ();
			g_MSSimFunList.fp_MSSIM_SubLCD_InvalidateRect( left, top, right, bottom );
			SCI_RestoreIRQ();
		}
		else
			assert(0);
	}
	
	return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E  LCD_Invalidate(
							LCD_ID_E  lcd_id	//id of lcd to operate
							)
{
	if (lcd_id==MAIN_LCD_ID)
	{
		SCI_DisableIRQ();
#ifdef MSCODE_MOD
        //all all blocks together to main lcd buffer      
        CombinLCDBlock();
#endif        
		g_MSSimFunList.fp_MSSIM_LCD_Invalidate();
		SCI_RestoreIRQ();
	}
	else
	{
		if (lcd_id==SUB_LCD_ID)
		{
			SCI_DisableIRQ();
			g_MSSimFunList.fp_MSSIM_SubLCD_Invalidate();
			SCI_RestoreIRQ();
		}
		else
			assert(0);
	}
	
	return ERR_LCD_NONE;
}

ERR_LCD_E LCD_Init()
{
#ifdef MSCODE_MOD
    LCD_INFO_T	lcd_info = {0};

	//get lcd width and height
	g_MSSimFunList.fp_MSSIM_LCD_GetInfo(MAIN_LCD_ID, &lcd_info); 
	LCD_WIDTH  = lcd_info.lcd_width;
	LCD_HEIGHT = lcd_info.lcd_height;

    SCI_MEMSET(&s_block_info, 0, sizeof(s_block_info));
    s_block_info[0][0].is_enable = FALSE;
    s_block_info[0][0].mem_ptr = (uint32 *)s_lcd_block0_buf;
    s_block_info[0][0].cfg.start_x = 0;
    s_block_info[0][0].cfg.start_y = 0;
	s_block_info[0][0].cfg.end_x = lcd_info.lcd_width; - 1;
    s_block_info[0][0].cfg.end_y = lcd_info.lcd_height - 1;
	s_block_info[0][0].cfg.resolution = 2;
    s_block_info[0][0].cfg.type = 1;	//0-osd 1-image

	s_block_info[0][1].is_enable = TRUE;
    s_block_info[0][1].mem_ptr = (uint32 *)s_lcd_block1_buf;
	s_block_info[0][1].cfg.colorkey = 1;
    s_block_info[0][1].cfg.start_x = 0;
    s_block_info[0][1].cfg.start_y = 0;
	s_block_info[0][1].cfg.end_x = lcd_info.lcd_width; - 1;
    s_block_info[0][1].cfg.end_y = lcd_info.lcd_height - 1;
	s_block_info[0][1].cfg.resolution = 2;
    s_block_info[0][1].cfg.type = 0;	//0-osd 1-image
    
    g_MSSimFunList.fp_MSSIM_LCD_GetInfo(SUB_LCD_ID, &lcd_info);
	s_block_info[1][0].is_enable = TRUE;
	s_block_info[1][0].mem_ptr = (uint32 *)lcd_info.lcdbuff_ptr;
	s_block_info[1][0].cfg.start_x = 0;
	s_block_info[1][0].cfg.start_y = 0;
	s_block_info[1][0].cfg.end_x = lcd_info.lcd_width - 1;
	s_block_info[1][0].cfg.end_y = lcd_info.lcd_height - 1;

#ifdef LCD_TEST
    {
		static uint16 s_buf[240 * 320] = {0};
        SCI_MEMSET(s_buf, 255, sizeof(s_buf));
        s_block_info[0][1].is_enable = TRUE;
        s_block_info[0][1].mem_ptr = (uint32 *)s_buf;
        s_block_info[0][1].cfg.colorkey = 1;
        s_block_info[0][1].cfg.start_x = 50;
        s_block_info[0][1].cfg.start_y = 50;
        s_block_info[0][1].cfg.end_x = 91;
        s_block_info[0][1].cfg.end_y = 80;
        s_block_info[0][1].cfg.resolution = 2;
        s_block_info[0][1].cfg.type = 1;	//0-osd 1-image
        s_block_info[0][1].cfg.alpha = 220;
        s_block_info[0][1].cfg.width = 0;
    }
#endif

#endif
    return ERR_LCD_NONE;
}

void LCD_Close()
{
    NULL;
}

#ifdef MSCODE_MOD
PUBLIC ERR_LCD_E LCDC_GetBlockCtrl(LCD_ID_E lcd_id, uint32 block_id)
{
	return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_ReleaseBlockCtrl(LCD_ID_E lcd_id, uint32 block_id)
{
	return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_ConfigBlock(
                                 LCD_ID_E lcd_id, 
                                 uint32 blk_num, 
                                 BLOCKCFG_T config
                                 )
{
	return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_GetConfigBlock(
                                    LCD_ID_E lcd_id, 
                                    uint32 blk_num, 
                                    BLOCKCFG_T *cfg_ptr
                                    )
{
	return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_EnableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
   return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_DisableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
    return ERR_LCD_NONE;
}

PUBLIC uint32 * LCDC_GetBlockBuffer(
                                   LCD_ID_E lcd_id, 
                                   uint32 blk_num                                  
                                   )
{
    return s_block_info[lcd_id][blk_num].mem_ptr;	
}

PUBLIC ERR_LCD_E LCDC_SetBlockBuffer(
                                    LCD_ID_E lcd_id, 
                                    uint32 blk_num , 
                                    uint32 *buf_ptr
                                    )
{
	s_block_info[lcd_id][blk_num].mem_ptr = buf_ptr;
    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCD_ConfigBlock(
                                 LCD_ID_E lcd_id, 
                                 uint32 blk_num, 
                                 BLOCKCFG_T config
                                 )
{
    SCI_ASSERT(lcd_id < MAX_LCD_ID && blk_num < MAX_LCD_BLOCK_NUM);
    
    s_block_info[lcd_id][blk_num].cfg = config;
    
    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCD_GetConfigBlock(
                                    LCD_ID_E lcd_id, 
                                    uint32 blk_num, 
                                    BLOCKCFG_T *cfg_ptr
                                    )
{
    SCI_ASSERT(lcd_id < MAX_LCD_ID && blk_num < MAX_LCD_BLOCK_NUM);
    
    *cfg_ptr = s_block_info[lcd_id][blk_num].cfg;
    
    return ERR_LCD_NONE;
}

PUBLIC BOOLEAN LCD_GetBlockIsEnable (LCD_ID_E lcd_id, uint32 blk_num)
{
	SCI_ASSERT(lcd_id < MAX_LCD_ID && blk_num < MAX_LCD_BLOCK_NUM);

	return s_block_info[lcd_id][blk_num].is_enable;
}

PUBLIC ERR_LCD_E LCD_EnableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
    SCI_ASSERT(lcd_id < MAX_LCD_ID && blk_num < MAX_LCD_BLOCK_NUM);
    
    s_block_info[lcd_id][blk_num].is_enable = TRUE;
    
    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCD_DisableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
    SCI_ASSERT(lcd_id < MAX_LCD_ID && blk_num < MAX_LCD_BLOCK_NUM);

    s_block_info[lcd_id][blk_num].is_enable = FALSE;
    
    return ERR_LCD_NONE;
}

PUBLIC uint32 * LCD_GetBlockBuffer(
                                   LCD_ID_E lcd_id, 
                                   uint32 blk_num                                  
                                   )
{
    SCI_ASSERT(lcd_id < MAX_LCD_ID && blk_num < MAX_LCD_BLOCK_NUM);	
    
    return s_block_info[lcd_id][blk_num].mem_ptr;	
}

PUBLIC ERR_LCD_E LCD_SetBlockBuffer(
                                    LCD_ID_E lcd_id, 
                                    uint32 blk_num , 
                                    uint32 *buf_ptr
                                    )
{
    SCI_ASSERT(lcd_id < MAX_LCD_ID && blk_num < MAX_LCD_BLOCK_NUM);
    
    SCI_ASSERT(buf_ptr != 0);
    
    s_block_info[lcd_id][blk_num].mem_ptr = buf_ptr;
    return ERR_LCD_NONE;
}

#endif

void LCD_DisplayOnOff( uint32 display_on )
{
    NULL;
}

ERR_LCD_E   LCD_EnterSleep(
							     LCD_ID_E  lcd_id,	//id of lcd to operate
							     BOOLEAN is_sleep	//SCI_TRUE:enter sleep: SCI_FALSE:exit sleep mode
							     )
{
	if ((lcd_id==MAIN_LCD_ID)||(lcd_id==SUB_LCD_ID))
	{
	    return ERR_LCD_NONE;
	}
	else
	{
		return ERR_LCD_PARAMETER_WRONG;
	}
}

ERR_LCD_E LCD_SetContrast(
							 	LCD_ID_E  lcd_id,	//id of lcd to operate
							 	uint16  contrast	//contrast value to set to lcd.
							 	)
{
	if ((lcd_id==MAIN_LCD_ID)||(lcd_id==SUB_LCD_ID))
	{
	    return ERR_LCD_NONE;
	}
	else
	{
		return ERR_LCD_PARAMETER_WRONG;
	}
}

PUBLIC uint32 LCD_ChangeContrast( uint32  bInc )
{
    return 0;
}

#ifdef MSCODE_MOD

/*****************************************************************************/
//  Description:    Set the brightness of LCD.
//	Global resource dependence: 
//  Author:         Jim.zhang
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrightness(
	LCD_ID_E lcd_id,	//ID of lcd to operate.
	uint16 brightness	//brightness value to set.
	)
{
	return ERR_LCD_NONE;	
}
	

/*****************************************************************************/
//  Description:    Enable lcd to partial display mode, so can save power.
//	Global resource dependence: 
//  Author:         Jim.zhang
//  Return:         SCI_TRUE:SUCCESS ,SCI_FALSE:failed.
//	Note:           If all input parameters are 0, exit partial display mode.
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_SetDisplayWindow(
									   LCD_ID_E lcd_id,//ID of lcd to operate
									   uint16 left, 	//left of the display window
									   uint16 top, 	//top of the display window
									   uint16 right, 	//right of the display window
									   uint16 bottom	//bottom of the display window
									  )
{
	return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:  set start address of lcd
//	Global resource dependence:
//  Author: Younger.Yang
//	Note:
/*****************************************************************************/
extern "C"
PUBLIC ERR_LCD_E  LCD_SetStartAddress(
									  LCD_ID_E lcd_id,//ID of lcd to operate
									  uint16 left, 	//left of the display window
									  uint16 top 	//top of the display window
									  )
{
	return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:get memory status  
//	Global resource dependence:
//  Author: Jianping.wang
/*****************************************************************************/
extern "C"
PUBLIC LCD_STATE_E LCD_GetLCDState(LCD_ID_E lcd_id)
{
    return LCD_IDLE;
}
/*****************************************************************************/
//  Description: get brush lcd fashion  
//	Global resource dependence:
//  Author: Jianping.wang
/*****************************************************************************/
extern "C"
PUBLIC LCD_BRUSH_MODE_E LCD_GetBrushMode(LCD_ID_E lcd_id)
{
    return LCD_BRUSH_MODE_SYNC;
}

/*****************************************************************************/
//  Description: get lcd controller type  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC LCD_CTRL_MODULE_E LCD_GetCtrlModule(LCD_ID_E lcd_id)
{
	return LCD_CTRL_MODULE_LCDC;
}

/*****************************************************************************/
//  Description: set brush lcd fashion  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrushMode(LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode)
{
	return ERR_LCD_NONE;
}

			

/*****************************************************************************/
//  Description: Malloc memory
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC uint32 *LCD_Malloc(uint32 size)
{
	SCI_ASSERT(0);
	return 0;
}

/*****************************************************************************/
//  Description:  free memory 
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC void LCD_Free(uint32 *addr_ptr)
{
	SCI_ASSERT(0);

}


/*****************************************************************************/
//  Description:  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
//PUBLIC LCD_MEMORY_STATE_E LCD_GetMemoryStatus(LCD_ID_E lcd_id, uint32 *mem_ptr)
//{
//	SCI_ASSERT(0);
//	return MEMORY_IDLE;
//}

/*****************************************************************************/
//  Description: Update LCD Timing  
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_UpdateTiming(uint32 ahb_clk)
{
	return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:   
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
//PUBLIC void TV_Open(TV_MODE_E mode)
//{

//}

/*****************************************************************************/
//  Description:   
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC void TV_Refresh(void)
{

}

/*****************************************************************************/
//  Description:   
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
PUBLIC void TV_Close(void)
{
	return;
}



#endif

//@ Kai Liu 2004-6-30 CR10299
extern "C"
PUBLIC void LCD_Clear(
                      LCD_ID_E  lcd_id,	//id of lcd to operate
                      uint32 color		//color to fill the lcd.
                     )
{
    LCD_INFO_T lcd_info;
    ERR_LCD_E ret = LCD_GetInfo( lcd_id,&lcd_info );
    if( ERR_LCD_NONE != ret )
    {
        return;
    }
    
    char bytes_per_pixel = lcd_info.bits_per_pixel / 8;

    if( 0 == bytes_per_pixel )
    {
        // 1 bit per pixel
        if( color )
        {
            color = 0xFF;
        }
        else
        {
            color = 0;
        }
        memset( lcd_info.lcdbuff_ptr,color,lcd_info.lcd_height * lcd_info.lcd_width / 8 );
    }
    else if( 1 == bytes_per_pixel )
    {
        // 8 bit per pixel
        memset( lcd_info.lcdbuff_ptr,( color & 0xFF ),lcd_info.lcd_height * lcd_info.lcd_width );
    }
    else
    {
        // 16,24 or 32 bit per pixel
        unsigned char* lp_lcd_buff = (unsigned char*)lcd_info.lcdbuff_ptr;
        unsigned char* c = (unsigned char*)&color;
        for( int i=0;i<lcd_info.lcd_height * lcd_info.lcd_width;i++ )
        {
            for( int j=0;j<bytes_per_pixel;j++ )
            {
                *lp_lcd_buff++ = *( c + j );
            }
        }
    }

    // Redraw lcd
    LCD_Invalidate( lcd_id );
}

PUBLIC ERR_LCD_E LCD_SetUVBuffer (
    LCD_ID_E lcd_id,
    uint32 blk_num,
    uint32 *buf_ptr
)
{

    return ERR_LCD_NONE;
}


//
// Sub-LCD Panel
//

uint32 SubLCD_GetPanelSize()
{
    SCI_DisableIRQ();
    uint32 size = g_MSSimFunList.fp_MSSIM_SubLCD_GetPanelSize();
    SCI_RestoreIRQ();
    
    return size;
}

uint16 * SubLCD_GetBufferPointer()
{
    SCI_DisableIRQ();
    uint16 * pLCDBuffer = g_MSSimFunList.fp_MSSIM_SubLCD_GetBufferPointer();
    SCI_RestoreIRQ();
    
    return pLCDBuffer;
}

void SubLCD_SetPixelColor( WORD x, WORD y, WORD color )
{
    SCI_DisableIRQ();
    g_MSSimFunList.fp_MSSIM_SubLCD_SetPixelColor( x, y, color );
    SCI_RestoreIRQ();
}

WORD SubLCD_GetPixelColor( WORD x, WORD y )
{
    SCI_DisableIRQ();
    uint16 color = g_MSSimFunList.fp_MSSIM_SubLCD_GetPixelColor( x, y );
    SCI_RestoreIRQ();
    
    return color;
}

void SubLCD_DrawPixel( WORD x, WORD y, WORD color )
{
    SCI_DisableIRQ();
    g_MSSimFunList.fp_MSSIM_SubLCD_DrawPixel( x, y, color );
    SCI_RestoreIRQ();
}
/*
void SubLCD_InvalidateRect( WORD left, WORD top, WORD right, WORD bottom )
{
    SCI_DisableIRQ();
    g_MSSimFunList.fp_MSSIM_SubLCD_InvalidateRect( left, top, right, bottom );
    SCI_RestoreIRQ();
}

void SubLCD_Invalidate()
{
    SCI_DisableIRQ();
    g_MSSimFunList.fp_MSSIM_SubLCD_Invalidate();
    SCI_RestoreIRQ();
}
*/

void SubLCD_Init()
{
    NULL;
}

void SubLCD_Close()
{
    NULL;
}

void SubLCD_DisplayOnOff( uint32 display_on )
{
    NULL;
}

void SubLCD_SleepInOut( uint32 sleep_in )
{
    NULL;
}

uint32 SubLCD_SetContrast( uint32 contrast )
{
    return 0x20;
}

uint32 SubLCD_ChangeContrast( uint32  bInc )
{
    return 0;
}


// ----------------------------------------------------------------------------
// FUNCTION NAME:   SCI_Trace
// AUTHOR:          Liu Jun
// INPUT:           ...
// RETURN:          None
// DESCRIPTION:     Internal trace functions for RTOS (DEBUG mode in VC)
// ----------------------------------------------------------------------------
void SCI_Trace(const char * x_format, ...)
{
	va_list     args;
	int         nBuf;
    char        format_str[MAX_TRACESTR_LEN] = {0};
	static  int flag=0;

	if(!g_MSSimFunList.fp_MSSIM_IsTraceOn())
	{
		// Do not delete follow code, it will effect the MSDEV not start.
		if((flag++)<1000)
		{
			Sleep(1);
		}		
		return;
	}
	
    SCI_DisableIRQ();
	
	va_start(args, x_format);

	//apple:  support to outputing  trace to logel
	//[[
#ifdef TRACE_LOG_DISABLE_OPT
	nBuf = _vsnprintf(format_str, sizeof(format_str) - 1,x_format, args);
#else
	nBuf = _vsnprintf(format_str + sizeof(TRACE_LOG_HEAD_T) , sizeof(format_str) - 1, x_format, args);
#endif
	// was there an error? was the  expanded string too long?
	//assert((nBuf >= 0) && (nBuf < MAX_TRACESTR_LEN));
	va_end(args);
#ifndef TRACE_LOG_DISABLE_OPT
	int status = 0;
	
    TRACE_LOG_HEAD_T *ltrace_msg_ptr = (TRACE_LOG_HEAD_T *) format_str;
	
    SCI_ASSERT(format_str != SCI_NULL);
	
    // Add information to the string. 
    ltrace_msg_ptr->SignalCode   = TRACE_LOG;
    ltrace_msg_ptr->SignalSize   = sizeof(TRACE_LOG_HEAD_T) + nBuf + 1;
	
    // Now send the message to the buffer.
    status = SIO_SendTracePacket( (uint8*)format_str, 
		(sizeof(TRACE_LOG_HEAD_T) + nBuf + 1));
#endif
	//]]
#ifndef UNIT_TEST
    #ifdef TRACE_LOG_DISABLE_OPT
		g_MSSimFunList.fp_MSSIM_Trace(format_str);
    #else
        g_MSSimFunList.fp_MSSIM_Trace(format_str + sizeof(TRACE_LOG_HEAD_T));
    #endif
#endif
	
    SCI_RestoreIRQ();
	

}

// ----------------------------------------------------------------------------
// FUNCTION NAME:   SCI_TraceEx
// AUTHOR:          Liu Jun
// INPUT:           ...
// RETURN:          None
// DESCRIPTION:     Internal trace functions for RTOS (DEBUG mode in VC)
// ----------------------------------------------------------------------------
void SCI_TraceEx(uint32 trc_calss, const char * x_format,const uint8 * arg_type, ...)
{
	va_list     args;
	int         nBuf;
    char        format_str[MAX_TRACESTR_LEN] = {0};
	static  int flag=0;
	
	if(!g_MSSimFunList.fp_MSSIM_IsTraceOn())
	{
		// Do not delete follow code, it will effect the MSDEV not start.
		if((flag++)<1000)
		{
			Sleep(1);
		}
		return;
	}
	
    SCI_DisableIRQ();
	
	va_start(args, arg_type);

	//apple:  support to outputing  trace to logel
	//[[
#ifdef TRACE_LOG_DISABLE_OPT
	nBuf = _vsnprintf(format_str, sizeof(format_str) - 1,x_format, args);
#else
	nBuf = _vsnprintf(format_str + sizeof(TRACE_LOG_HEAD_T) , sizeof(format_str) - 1, x_format, args);
#endif
	// was there an error? was the  expanded string too long?
	//assert((nBuf >= 0) && (nBuf < MAX_TRACESTR_LEN));
	va_end(args);
#ifndef TRACE_LOG_DISABLE_OPT
	int status = 0;
	
    TRACE_LOG_HEAD_T *ltrace_msg_ptr = (TRACE_LOG_HEAD_T *) format_str;
	
    SCI_ASSERT(format_str != SCI_NULL);
	
    // Add information to the string. 
    ltrace_msg_ptr->SignalCode   = TRACE_LOG;
    ltrace_msg_ptr->SignalSize   = sizeof(TRACE_LOG_HEAD_T) + nBuf + 1;
	
    // Now send the message to the buffer.
    status = SIO_SendTracePacket( (uint8*)format_str, 
		(sizeof(TRACE_LOG_HEAD_T) + nBuf + 1));
#endif
	//]]
#ifndef UNIT_TEST
    #ifdef TRACE_LOG_DISABLE_OPT
		g_MSSimFunList.fp_MSSIM_Trace(format_str);
    #else
        g_MSSimFunList.fp_MSSIM_Trace(format_str + sizeof(TRACE_LOG_HEAD_T));
    #endif
#endif
	
    SCI_RestoreIRQ();
	

}

// Initialize the keypad
uint32 KPD_Create(BLOCK_ID svr)
{
    svrID = svr;
    return 0;
}

// Get the key code
uint16 KPD_ScanKey()
{
    return g_MSSimFunList.fp_MSSIM_KPD_ScanKey();
}

void SIO_ATC_ReadCmdLine( 
        uint8 * data_ptr,      // Output argument, point to the buf which is alloced
                               // by caller 
        uint length,           // The length of cmd line which should be read from
                               // sio
        uint * read_length_ptr // The actual read length
        )
{
    uint i;
    uint8 ch;

    for (i = 0; i < length; i++)
    {
        ch = ring_remove( &atc_buf );

        if (ch != -1)
        {
            if (ch == atc_end_ch1 || ch == atc_end_ch2)
            {
                *data_ptr = ch;
                *read_length_ptr += 1;
                break;
            }
            else if (ch == atc_bs_ch)
            {
                data_ptr--;
                *read_length_ptr -= 1;
            }
            else
            {
                *data_ptr++ = ch;
                *read_length_ptr += 1;
            }
        }
        else
        {
            break;
        }
    }
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void SIO_ATC_WriteCmdRes()                                            *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Write AT command to the sio                                           *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void SIO_ATC_WriteCmdRes(
    uint8 * res_data_ptr,  // Point to the response infomation buffer
    uint    length         // The length of response infomation buffer
)
{
    //TRACE((char *)res_data_ptr);

    uint8 * data;
    SCI_DisableIRQ();

    data = (uint8 *) malloc(sizeof(MSG_HEAD_T) + length);
    
    CM_COPY_MSG_HEAD(data, 0, (sizeof(MSG_HEAD_T) + length), DIAG_AT_A, 0)

    memcpy(data + sizeof(MSG_HEAD_T), res_data_ptr, length);

    SIO_AT_SendPacket(data, sizeof(MSG_HEAD_T) + length);

    free(data);

    SCI_RestoreIRQ();
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void SIO_ATC_SetCmdLineTerminateChar( )                               *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set terminate char of of AT command                                   *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void SIO_ATC_SetCmdLineTerminateChar( 
        uint8 terminate_char1,   // Default terminate char is '\n' 
        uint8 terminate_char2   // Default terminate char is '\0'
        )
{
    atc_end_ch1 = terminate_char1;
    atc_end_ch2 = terminate_char2;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void SIO_ATC_SetCmdLineBackSpaceChar()                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set backspace char of the AT command                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void SIO_ATC_SetCmdLineBackSpaceChar( 
        uint8 back_space_char  // Default backspace char value is 8
        )
{
    atc_bs_ch = back_space_char;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_ATC_SetDataMode()                                            *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Tell SIO change the state to data mode                                *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
/* @jun.liu cr5120 2003/10/10 */
void SIO_ATC_SetDataMode( BOOLEAN mode )
{
    NULL;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void SIO_ATC_SetCallback()                                            *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set callback function from SIO.                                       *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void SIO_ATC_SetCallback(ATC_CALLBACK callback)
{
    SCI_ASSERT(SCI_NULL != callback);

    AT_callback_ind = callback;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  * 
 **     void SIO_ATC_SetEcho()                                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Tell SIO to echo the char it receives                                 *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
PUBLIC void SIO_ATC_SetEcho(BOOLEAN IsEcho)
{
	return;
}

// HandFree and Hook
//LOCAL   b_hand_free_valid   = SCI_FALSE;    
LOCAL BOOLEAN b_is_hook_up        = SCI_FALSE;

/*****************************************************************************/
//  Description:    This function initialize GPIO interrupt.
//                  1. Enable keypad device.
//                  2. Enable keypad interrupt.
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
/*PUBLIC uint32 GPIO_Init(void)
{
    uint32  status    = 0;

    b_hand_free_valid   = SCI_FALSE;    
    b_is_hook_up        = SCI_FALSE;
    
    return status;
}
*/
/*****************************************************************************/
//  Description:    This function set LED of hand free.
//                  b_light = SCI_TRUE   turn on LED
//                  b_light = SCI_FALSE  turn off LED
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void GPIO_SetHandFreeLed(BOOLEAN b_light)
{
}

/*****************************************************************************/
//  Description:    This function return hook state(Pick up or put down).
//  Author:         Xueliang.Wang
//	Note:           SCI_TRUE:   Hook is Picked up
//                  SCI_FALSE:  Hook is Putted down
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_GetHookState(void)
{
    return b_is_hook_up;
}

/*****************************************************************************/
//  Description:    This function set LED of hand free.
//                  b_aux_device = SCI_TRUE   Enable aux audio device
//                  b_aux_device = SCI_FALSE  Disable aux audio device
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void GPIO_EnableAuxAudio(BOOLEAN b_aux_device)
{
}

/*****************************************************************************/
//  Description:    This function set BackLight.
//                  b_light = SCI_TRUE   turn on LED
//                  b_light = SCI_FALSE  turn off LED
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void GPIO_SetBackLight(BOOLEAN b_light)
{
}

/*****************************************************************************/
//  Description:    This function set LED1.
//                  b_light = SCI_TRUE   turn on LED
//                  b_light = SCI_FALSE  turn off LED
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void GPIO_SetLed1(BOOLEAN b_light)
{
}

/*****************************************************************************/
//  Description:    This function set LED2.
//                  b_light = SCI_TRUE   turn on LED
//                  b_light = SCI_FALSE  turn off LED
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void GPIO_SetLed2(BOOLEAN b_light)
{
}

/*****************************************************************************/
//  Description:    This function set LED3.
//                  Controled by Gpio Pin16(Function 4).
//                  b_light = SCI_TRUE   turn on LED
//                  b_light = SCI_FALSE  turn off LED
//  Author:         Xueliang.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void GPIO_SetLed3(BOOLEAN b_light)
{
}

/*****************************************************************************/
//  Description:    This function controls PWM_B playing.
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
//PUBLIC void AUDIO_Beep(uint32 freq, AUD_DRV_VOLUME_E volume)
PUBLIC void AUDIO_Beep(uint32 freq, uint16 volume)
{
    Beep(freq, 0xFFFFFFFF);
}

/*****************************************************************************/
//  Description:    This function stops PWM_B.
//  Author:         Richard.Yang
//	Note:           
/*****************************************************************************/
PUBLIC void AUDIO_Disable(void)
{
    Beep(1, 0x1);
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_SetBaudRate(uint32 port,uint32  baud_rate);                    *
 **                                                                           *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Set serial port's baud rate                                           *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     NONE                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 ** Author   Eric.zhou           2003/5/20                                    *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC BOOLEAN SIO_SetBaudRate(uint32 port, uint32 baud_rate)
{
   return 0;
}
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     uint32 SIO_GetBaudRate(uint32 port)                                     *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **      Get Serial Port's Baud Rate                                          *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **      Baud Rate                                                            *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 ** Author   Eric.zhou           2003/5/20                                    *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 SIO_GetBaudRate(uint32 port)
{
   return 0;
}




/* @Liu Jun 2003-12-25 */
/* temporary code, should be removed later */
extern "C"
{
// Implement only to support DPhone old
// lcd apis

// Only support 16-bit lcd
#define BACKGROUND_COLOR    0xfe0
#define TEXT_COLOR          0

    //@ Liu Kai 2004-9-3 CR13124
    void LCD_SetPixel(uint16 x, uint16 y, uint8 bit_state)
    {
        uint32 size = LCD_GetPanelSize();
        uint16 width = LOWORD( size );
        uint16 height = HIWORD( size );
        
        if( x < width && y < height )
        {
            if( bit_state )
            {
                LCD_SetPixelColor( x,y,WORD( TEXT_COLOR ) );
            }
            else
            {
                LCD_SetPixelColor( x,y,BACKGROUND_COLOR );
            }
        }
    }

    void LCD_SetMultiPixels(uint16 x, uint16 y, uint8 bits_mask, uint8 num)
    {
        if( num > 8 )
        {
            return;
        }

        BYTE ref = 0x01;
        for( int i=0;i<num;i++ )
        {
            if( bits_mask & ref )
            {
                LCD_SetPixel(x, y + i, TRUE);
            }
            else
            {
                LCD_SetPixel(x, y + i, FALSE);
            }
            
            ref <<= 1;
        }
    }
    void LCD_SetPixels(uint16 x, uint16 y, uint8 bits_mask)
    {
        LCD_SetMultiPixels( x,y,bits_mask,8 );
    }


    VOID CALLBACK LineDDAProc( int x,int y,long lPara )
    {
        LCD_SetPixel( x,y,lPara );
    }

    void LCD_Line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint8 color)
    {
        ::LineDDA( x1,y1,x2,y2,LineDDAProc,color );
    }
    void LCD_SetBacklightMode(uint32 mode)
    {
    }
    void LCD_Clr()
    {
#ifndef MSCODE_MOD
        LCD_Clear( MAIN_LCD_ID,BACKGROUND_COLOR );
#endif
    } 
   
}

//apple 2009-04-14
extern "C"
{
void GPIO_SetLcdBackLight(BOOLEAN b_on)
{
    b_on = b_on;
	//apple 2009-04-14
	g_MSSimFunList.fp_MSSIM_SetLcdBackLight( b_on );	
}

void GPIO_SetSlideState(BOOLEAN b_on)
{
    b_on = b_on;
	g_MSSimFunList.fp_MSSIM_SetSlideState( b_on );	
}

void GPIO_SetFlipState(BOOLEAN b_on)
{
    b_on = b_on;
	g_MSSimFunList.fp_MSSIM_SetFlipState( b_on );	
}

void GPIO_SetVibrator(BOOLEAN b_on)
{
	b_on = b_on;
	g_MSSimFunList.fp_MSSIM_SetVibrate( b_on );
}
void SIO_SendData2Com(char *data)
{
	g_MSSimFunList.fp_MSSIM_SendData2Com(data);
	//g_MSSimFunList.fp_MSSIM_Trace("----SIO_SendData2Com");
}
}//extern "C"

// #ifdef MSCODE_MODE

extern "C"
{
// g_MSSimFunList.fp_MSSIM_SetDCViewState( TRUE/FALSE,0,0,0,0 ),GUI will display full screen

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     VOID SIO_ATC_ReadCmdLine                                              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Read clear atc sio buffer content,the function only used in ISR       *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
PUBLIC void SIO_ATC_ClrCmdLine(
    void
)
{
    atc_buf.end_point = atc_buf.start_point;
}

/*****************************************************************************/
// Description :    this function changes userport from one mode to another,
//                       following mode transition can be done by this funcition:
//                       MIX_MODE-->ATC_MODE
//                       MIX_MODE-->DATA_MODE
//                       ATC_MODE-->MIX_MODE
//                       ATC_MODE-->DATA_MODE
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SetUserPortMode (uint32 mode)
{
#ifdef MUX_SUPPORT
    /*modify for mux func, for at data sent from mux instead of sio*/
    return SCI_SUCCESS;
#else

    if (user_port_mode == mode)
    {
        return SCI_SUCCESS;
    }

    switch (user_port_mode)
    {
        case MIX_MODE:
            user_port_mode = mode;
            break;
        case ATC_MODE:

            if (mode == MIX_MODE)
            {
                SCI_DisableIRQ();
                SIO_ATC_ClrCmdLine();//Shijun.cui 2005-02-02 Added.
                //there similarly may be some data in AT BUF while reboot after module assert
                //Should clear it!
                SCI_RestoreIRQ();
            }

            user_port_mode = mode;
            break;
        case DATA_MODE:
            SCI_TRACE_LOW ("SIO SIO_SetUserPortMode error, current mode is DATA_MODE\n");
            return SCI_ERROR;
        default:
            return SCI_ERROR;
    }

    return SCI_SUCCESS;
#endif
}

/*****************************************************************************/
// Description :    get the mode of COM_DATA port
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_GetUserPortMode (void)
{
    return (user_port_mode);
}
/*****************************************************************************/
//  Description:    This function used to set AT uart port mix mode
//  Author:
//  Parameter:  mix_enable  TRUE - set AT uart port in mix mode
//                          FALSE - set AT uart port not in mix mode
//  Note:
/*****************************************************************************/
PUBLIC void SIO_ATC_SetMixMode (
    BOOLEAN mix_enable
)
{
    gprs_debug = mix_enable;
}

} // extern "C"

//////////////////////////////////////////////////////////////////////////
// UI Layer display
extern "C"
{

#define X_RGB8882RGB565(rgb888)   ((((rgb888)&0xf80000)>>8)|(((rgb888)&0xfc00)>>5)|(((rgb888)&0xf8)>>3))
	
#define X_Blend_GET_R_FROM_RGB565_VALUE(_rgb565_value) (((_rgb565_value) >> 8) & 0xf8)
#define X_Blend_GET_G_FROM_RGB565_VALUE(_rgb565_value) (((_rgb565_value) >> 3) & 0xfc)
#define X_Blend_GET_B_FROM_RGB565_VALUE(_rgb565_value) (((_rgb565_value) << 3)  & 0xf8)
	
	
#define X_BLEND_RGB565TORGB888(value_565,value_888) do{ \
	value_888 = (( value_565&0xf800) | ( ( value_565 & 0x800 ) ? 0x700 : 0 ))<<8 \
	|((value_565&0x7e0 ) | ( ( value_565&0x20 ) ? 0x18 : 0 ))<<5 \
	|((( value_565&0x1f )<<3) | ( ( value_565 & 0x1 ) ? 7 : 0 )); \
}while(0)
LOCAL ERR_DISPLAY_E x_get_layer_display_rect(DISPLAY_BLEND_LAYER_T *layer_ptr, DISPLAY_RECT_T *rect_ptr,DISPLAY_RECT_T *lcd_rect_ptr)
{
	int16 left   = 0;
	int16 top    = 0;
	int16 right  = 0;
	int16 bottom = 0;
	int16 lcd_left  = rect_ptr->left;
	int16 lcd_top   = rect_ptr->top;
	int16 lcd_right  = rect_ptr->right;
	int16 lcd_bottom = rect_ptr->bottom;
	
	if(layer_ptr != PNULL)
	{
		//the lcd plane
		left   = layer_ptr->layer_info.pos.x;
		top    = layer_ptr->layer_info.pos.y;      
		right  = left + layer_ptr->layer_info.width-1;
		bottom = top + layer_ptr->layer_info.height-1;
		
		left = left > lcd_left ? left : lcd_left;
		top = top > lcd_top ? top : lcd_top;
		right = right > lcd_right ? lcd_right : right;
		bottom = bottom > lcd_bottom ? lcd_bottom : bottom;
		
		if((left>=right)||(top>=bottom))
			return ERR_DISPLAY_PARAMETER_WRONG;
		
		lcd_rect_ptr->left   = left;
		lcd_rect_ptr->top    = top;
		lcd_rect_ptr->right  = right;
		lcd_rect_ptr->bottom = bottom;
		
	}
	
	return ERR_DISPLAY_NONE;
}

/*****************************************************************************/
//  Description:  low layer blends with ARGB888
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
__inline void x_soft_blend_ARGB888(DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 *dst_buf,
                                          uint32 *u32_src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height
                                          )
{
    uint32 vcount = 0;
    uint32 hcount = 0;
    uint32 src_value = 0;
    uint32 dst_value = 0;
    uint8  alpha = 0;
    uint8  h_r = 0,l_r = 0;
    uint8  h_g = 0,l_g = 0;
    uint8  h_b = 0,l_b = 0;
    uint8 r = 0,g = 0,b = 0;    
    BOOLEAN is_colorkey_en = layer_cfg_ptr->layer_data.colorkey_en;
    uint32  colorkey = layer_cfg_ptr->layer_data.colorkey;

    X_BLEND_RGB565TORGB888(colorkey, colorkey);                      
    
    vcount = refresh_height;      
    if(!is_colorkey_en)
    {
    	while(vcount)
    	{
    	    hcount = refresh_width;
    	    while(hcount)
    	    {
    	        src_value = *u32_src_buf++;
    		    alpha = (src_value>>24);                            
                
                if(alpha==0)
                {
                    dst_buf++;
                }
                else if(alpha==0xFF)
                {
                    *dst_buf++ = src_value; 
                }
                else
                {
                    h_r = ((src_value&0x00ff0000)>>16);
                    h_g = ((src_value&0x0000ff00)>>8);
                    h_b = src_value&0x000000ff;
                    
        	        dst_value = *dst_buf;
                    l_r = ((dst_value&0x00ff0000)>>16);
                    l_g = ((dst_value&0x0000ff00)>>8);
                    l_b = dst_value&0x000000ff;	
                    
                    r = ((l_r<<8)+alpha*(h_r-l_r)+128)>>8;
                    g = ((l_g<<8)+alpha*(h_g-l_g)+128)>>8;
                    b = ((l_b<<8)+alpha*(h_b-l_b)+128)>>8;
                    *dst_buf++ = (r<<16 | g<< 8 | b);
                }                
    	        hcount--;
    	    }
    	    u32_src_buf += srcjumpcount;
    	    dst_buf += dstjumpcount;
    	    vcount--;
    	}
    }
    else
    {
    	while(vcount)
    	{
    	    hcount = refresh_width;
    	    while(hcount)
    	    {
    	        src_value = *u32_src_buf++;    		    
              
                if(colorkey == (src_value&0x00ffffff))
                {
                    dst_buf++;                       
                }
                else
                {
                    alpha = (src_value>>24);
                    if(alpha==0)
                    {
                        dst_buf++;
                    }
                    else if(alpha==0xFF)
                    {
                        *dst_buf++ = src_value; 
                    }
                    else
                    {
                        h_r = ((src_value&0x00ff0000)>>16);
                        h_g = ((src_value&0x0000ff00)>>8);
                        h_b = src_value&0x000000ff;       
                
                        dst_value = *dst_buf;
                        l_r = ((dst_value&0x00ff0000)>>16);
                        l_g = ((dst_value&0x0000ff00)>>8);
                        l_b = dst_value&0x000000ff;	
                        
                        r = ((l_r<<8)+alpha*(h_r-l_r)+128)>>8;
                        g = ((l_g<<8)+alpha*(h_g-l_g)+128)>>8;
                        b = ((l_b<<8)+alpha*(h_b-l_b)+128)>>8;
                        *dst_buf++ = (r<<16 | g<< 8 | b);
                    }
                }
    	        hcount--;
    	    }
    	    u32_src_buf += srcjumpcount;
    	    dst_buf += dstjumpcount;
    	    vcount--;
    	}
        
    }
    

}
/*****************************************************************************/
//  Description:  low layer blends with RGB565
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
/*****************************************************************************/
__inline void x_soft_blend_RGB565(DISPLAY_LAYER_INFO_T *layer_cfg_ptr,
                                          uint32 *dst_buf,
                                          uint16 *src_buf,
                                          uint16 srcjumpcount,
                                          uint16 dstjumpcount,
                                          uint16 refresh_width,
                                          uint16 refresh_height
                                          )
{
    uint32 vcount = 0;
    uint32 hcount = 0;
    uint32 src_u32_value = 0;
    uint32 dst_value = 0;
    uint8  alpha = 0;
    uint8  h_r = 0,l_r = 0;
    uint8  h_g = 0,l_g = 0;
    uint8  h_b = 0,l_b = 0;
    uint8 r = 0,g = 0,b = 0;   
    BOOLEAN is_colorkey_en = layer_cfg_ptr->layer_data.colorkey_en;
    uint32 colorkey = layer_cfg_ptr->layer_data.colorkey;
    
    X_BLEND_RGB565TORGB888(colorkey, colorkey);     
    
    vcount = refresh_height;    
    alpha = layer_cfg_ptr->layer_data.alpha&0xff;       
    if(!is_colorkey_en)
    {   
        if(alpha==0)
        {
            return;
        }
        else if(alpha==0xFF)
        {
        	while(vcount)
        	{
        	    hcount = refresh_width;
        	    while(hcount)
        	    {
        	        src_u32_value = *src_buf++;                  
                    X_BLEND_RGB565TORGB888(src_u32_value, src_u32_value);                      
                    *dst_buf++ = src_u32_value;                                               
        	        hcount--;
        	    }
        	    src_buf += srcjumpcount;
        	    dst_buf += dstjumpcount;
        	    vcount--;
        	}
        }
        else
        {
        	while(vcount)
        	{
        	    hcount = refresh_width;
        	    while(hcount)
        	    {
        	        src_u32_value = *src_buf++;                  
                    X_BLEND_RGB565TORGB888(src_u32_value, src_u32_value);                  
        		    
                    h_r = ((src_u32_value&0x00ff0000)>>16);
                    h_g = ((src_u32_value&0x0000ff00)>>8);
                    h_b = src_u32_value&0x000000ff;
                    
        	        dst_value = *dst_buf;
                    l_r = ((dst_value&0x00ff0000)>>16);
                    l_g = ((dst_value&0x0000ff00)>>8);
                    l_b = dst_value&0x000000ff;	
                   
                    r = ((l_r<<8)+alpha*(h_r-l_r)+128)>>8;
                    g = ((l_g<<8)+alpha*(h_g-l_g)+128)>>8;
                    b = ((l_b<<8)+alpha*(h_b-l_b)+128)>>8;
                    *dst_buf++ = (r<<16 | g<< 8 | b);
                                  
        	        hcount--;
        	    }
        	    src_buf += srcjumpcount;
        	    dst_buf += dstjumpcount;
        	    vcount--;
        	}

        }
    }
    else
    {
        if(alpha==0)
        {
            return;
        }
        else if(alpha==0xFF)
        {
        	while(vcount)
        	{
        	    hcount = refresh_width;
        	    while(hcount)
        	    {
        	        src_u32_value = *src_buf++;                  
                    X_BLEND_RGB565TORGB888(src_u32_value, src_u32_value);                       

                    if(colorkey == (src_u32_value&0x00ffffff))
                    {
                         dst_buf++;                        
                    }
                    else
                    {                                                
                        *dst_buf++ = src_u32_value;                         
                    }
        	        hcount--;
        	    }
        	    src_buf += srcjumpcount;
        	    dst_buf += dstjumpcount;
        	    vcount--;
        	}
        }
        else
        {
        	while(vcount)
        	{
        	    hcount = refresh_width;
        	    while(hcount)
        	    {
        	        src_u32_value = *src_buf++;                  
                    X_BLEND_RGB565TORGB888(src_u32_value, src_u32_value);                   		                            
        	                            
                    if(colorkey == (src_u32_value&0x00ffffff))
                    {
                         dst_buf++;                        
                    }
                    else
                    {            
                        h_r = ((src_u32_value&0x00ff0000)>>16);
                        h_g = ((src_u32_value&0x0000ff00)>>8);
                        h_b = src_u32_value&0x000000ff;             
                        dst_value = *dst_buf;
                        l_r = ((dst_value&0x00ff0000)>>16);
                        l_g = ((dst_value&0x0000ff00)>>8);
                        l_b = dst_value&0x000000ff;	
                        r = ((l_r<<8)+alpha*(h_r-l_r)+128)>>8;
                        g = ((l_g<<8)+alpha*(h_g-l_g)+128)>>8;
                        b = ((l_b<<8)+alpha*(h_b-l_b)+128)>>8;
                        *dst_buf++ = (r<<16 | g<< 8 | b);
                    }
        	        hcount--;
        	    }
        	    src_buf += srcjumpcount;
        	    dst_buf += dstjumpcount;
        	    vcount--;
        	}

        }

    }
}

PUBLIC ERR_DISPLAY_E MSDev_DISPLAY_BlendLayer(  LCD_ID_E lcd_id,
	DISPLAY_BLEND_LAYER_T *layer_arr[],
	uint32 layer_num,
    DISPLAY_RECT_T *rect_ptr)
{

	uint32 nMinLayerNum = 0;
	//获得MSDev是否显示UILayer标志，如果不显示则直接返回
	DISPLAY_BLEND_LAYER_T *pMSSimLayerArr = NULL;
	DWORD dwMaxLayerNum = 0;
	uint32 i=0;
	uint32 nEnablLayerNum = 0;
	uint32 *pDst = NULL;
	int    k =0;
	LCD_INFO_T lcd_info = {0};
	uint16 lcd_width = 0;
	uint16 lcd_height = 0;
	if(g_MSSimFunList.fp_MSSIM_GetLayerInfo(lcd_id,(void**)&pMSSimLayerArr,&dwMaxLayerNum) == 0)
	{
		return ERR_DISPLAY_NONE;
	}

	g_MSSimFunList.fp_MSSIM_LCD_GetInfo(0, &lcd_info);

	lcd_width  = lcd_info.lcd_width;
	lcd_height = lcd_info.lcd_height;

	if(( rect_ptr->right - rect_ptr->left +1 ) > lcd_width)
	{
		rect_ptr->right = lcd_width+rect_ptr->left-1;
	}
	if(( rect_ptr->bottom - rect_ptr->top +1 ) > lcd_height)
	{
		rect_ptr->bottom = lcd_height +rect_ptr->top-1;
    }

	//获得MSDev显示UILayer的Buf信息，应该ARGB888格式的数据,逐层转换
	nMinLayerNum = layer_num < dwMaxLayerNum ? layer_num : dwMaxLayerNum;

	for(i=0;i<nMinLayerNum; i++)
	{
		uint32 tmp = pMSSimLayerArr[k].layer_info.buf_addr;
		memcpy(pMSSimLayerArr+k,layer_arr[i],sizeof(DISPLAY_BLEND_LAYER_T));
		pMSSimLayerArr[k].layer_info.buf_addr = tmp;
		//memset((LPBYTE)tmp,0,1024*1024*4);			

		pDst = (uint32*)pMSSimLayerArr[k].layer_info.buf_addr;	

		{
			uint32 color = 0x3960A5;
			uint32 *pTmp = pDst;
			uint32 l = 0;
			uint32 j = 0;
			for(l=0; l<lcd_height; l++)
			{
				for(j=0;j<lcd_width;j++)
				{
					pTmp[j]= color;
				}
				pTmp += lcd_width;
			}
		}

		DISPLAY_RECT_T rect = {0};
		if(x_get_layer_display_rect(layer_arr[i],rect_ptr,&rect) != ERR_DISPLAY_NONE)
		{
			continue;
		}

		if ((DATA_TYPE_ARGB565 == layer_arr[i]->layer_info.layer_data.type) \
			|| (DATA_TYPE_RGB565 == layer_arr[i]->layer_info.layer_data.type))
		{			
			uint16 value_565 = 0;
			uint32 l = 0;
			uint32 j = 0;
			uint16 *pSrc = NULL;
			uint16 h = rect.bottom - rect.top +1;
			uint16 w = rect.right - rect.left +1;

			//HDC hdcScreen = ::GetDC(NULL);
			
			//pDst = pDst +  (rect.left - layer_arr[i]->layer_info.pos.x) \
			//	+ (rect.top - layer_arr[i]->layer_info.pos.y)* lcd_info.lcd_width;
			pDst = pDst + rect.top * lcd_width + rect.left;

			pSrc = (uint16 *)layer_arr[i]->layer_info.buf_addr + (rect_ptr->left - layer_arr[i]->layer_info.pos.x) \
				+ ((rect_ptr->top - layer_arr[i]->layer_info.pos.y)*layer_arr[i]->layer_info.width);  
			
			if(pSrc < (uint16 *)layer_arr[i]->layer_info.buf_addr)
			{
				pSrc = (uint16 *)layer_arr[i]->layer_info.buf_addr;
			}

			int16 srcjumpcount = layer_arr[i]->layer_info.width-w;
			int16 dstjumpcount = lcd_width - w;  
			if(srcjumpcount < 0)
				srcjumpcount = 0;
			if(dstjumpcount < 0)
				dstjumpcount = 0;
			x_soft_blend_RGB565(&layer_arr[i]->layer_info,pDst,pSrc,srcjumpcount,dstjumpcount,w,h);
/*			
			for( l=0 ; l<h ; l++)
			{
				for( j=0 ; j<w ; j++)
				{
					value_565 = pSrc[j];
					X_BLEND_RGB565TORGB888(value_565, pDst[j]);
					//if(k==2)
					//{
					 //  ::SetPixel(hdcScreen,j,l,RGB(GET_R(value_565)<<3,GET_G(value_565)<<2,GET_B(value_565)<<3));
					  //::SetPixel(hdcScreen,j + l*refresh_width,l,RGB(((BYTE*)(pDst+j))[1],((BYTE*)(pDst+j))[2],((BYTE*)(pDst+j))[3]));
				
					//}
				}

				pDst += lcd_info.lcd_width;
				pSrc += layer_arr[i]->layer_info.width;
			}  	*/
		}
		else if (DATA_TYPE_ARGB888 == layer_arr[i]->layer_info.layer_data.type)
		{
			uint32 *src_u32_buf = PNULL;       
			uint32 value_888 = 0;
			uint32 l = 0;
			uint32 j = 0;
			uint32 *pSrc = NULL;
			//HDC hdcScreen = ::GetDC(NULL);
			uint16 h = rect.bottom - rect.top +1;
			uint16 w = rect.right - rect.left +1;
			int16 srcjumpcount = layer_arr[i]->layer_info.width - w;//(rect_ptr->right - rect_ptr->left) - layer_arr[i]->layer_info.width;
			int16 dstjumpcount = lcd_width - w;  

			//pDst = pDst +  (rect.left - layer_arr[i]->layer_info.pos.x) \
			//	+ (rect.top - layer_arr[i]->layer_info.pos.y)* lcd_info.lcd_width;

			pDst = pDst + rect.top * lcd_info.lcd_width + rect.left;

			pSrc = (uint32*)layer_arr[i]->layer_info.buf_addr + (rect_ptr->left - layer_arr[i]->layer_info.pos.x) \
				+ ((rect_ptr->top - layer_arr[i]->layer_info.pos.y)*(layer_arr[i]->layer_info.width));	
			
			if(pSrc < (uint32 *)layer_arr[i]->layer_info.buf_addr)
			{
				pSrc = (uint32 *)layer_arr[i]->layer_info.buf_addr;
			}
			
			if(srcjumpcount < 0)
				srcjumpcount = 0;
			if(dstjumpcount < 0)
				dstjumpcount = 0;
			x_soft_blend_ARGB888(&layer_arr[i]->layer_info,pDst,pSrc,srcjumpcount,dstjumpcount,w,h);
			
/*			for( l=0 ; l<h; l++)
			{
				memcpy(pDst,pSrc,w*4);
				
				//for(j=0; j<w;j++ )
				//{
				//	if(k==2)
				//	{
				//	  ::SetPixel(hdcScreen,j,l,RGB(((BYTE*)(pSrc+j))[2],((BYTE*)(pSrc+j))[1],((BYTE*)(pSrc+j))[0]));
				//	}
				//}  	

				pDst += lcd_info.lcd_width;
				pSrc += layer_arr[i]->layer_info.width;
			}
*/
		}

		// use msdev to show buf_addr
		pMSSimLayerArr[k].layer_info.is_enable = layer_arr[i]->layer_info.buf_addr;
		k++;

	}
	for(; k<dwMaxLayerNum;k++ )
	{
		pMSSimLayerArr[k].layer_info.is_enable = FALSE;
	}

	g_MSSimFunList.fp_MSSIM_InvalidateLayerInfo();

	return ERR_DISPLAY_NONE;
}

PUBLIC void DCAMERA_StartIspRecord(void){}


PUBLIC void DCAMERA_StopIspRecord(void){}

PUBLIC void DCAMERA_GetPclk(uint32 *pclk)
{
	*pclk = 3000000;
}

}// extern "C" 
// end UI Layer display
//////////////////////////////////////////////////////////////////////////

//#endif
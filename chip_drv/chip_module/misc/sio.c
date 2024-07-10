/******************************************************************************
 ** File Name:      sio.c                                                     *
 ** Author:         Richard Yang                                              *
 ** DATE:           04/02/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Richard.Yang     Create.                                   *
 ** 05/13/2002     Richard.Yang     Add sio_send_packet, sio_send_log_packet  *
 ** 05/30/2003     Eric.zhou        Add AT through channel server and DEBUG   *
 **                                 and USER mode change functions            *
 ** 08/14/2003     Zhemin.Lin       add GetClkDividerValue,                   *
 **                                 SIO_UpdateClkDividerValue                 *
 **08/25/2003     Zhemin.Lin        add MIX_MODE for COM_DATA port  according to CR:MS00004213  *
 ** 08/28/2003    Zhemin.Lin        Modify according CR:MS00004031            *
 ** 09/12/2003    Zhemin.Lin        Modify according CR:MS00004678            *
 ** 09/23/2003    Xueliang.wang     Modify according CR:MS00004877            *
 ** 09/23/2003    jim.zhang         CR:MS00006386
 ** 11/03/2010    fei.zhang      Add DSP USB Log function       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "tasks_id.h"
#include "cmddef.h"
#include "cm.h"
#include "sio.h"
#include "uart_drvapi.h"  //extern declaration of uart
#include "ucom_api.h"     //extern declaration of usb virtual com
#include "ref_param.h"
#include "dma_drv_internal.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "task_monitor.h"
//#define BOOTUP_UART_LOG_SUPPORT

#ifdef BOOTUP_UART_LOG_SUPPORT
#include "power.h"
#endif //BOOTUP_UART_LOG_SUPPORT
#ifdef CARD_LOG
#include "logsave_output.h"
#endif

#ifdef DSP_USB_LOG
#include "mem_prod.h"
#include "ref_outport.h"
#include "dsp_log.h"
#endif

#ifdef BT_USB_LOG
#include "mem_prod.h"
#include "ref_outport.h"
#include "bt_log.h"
#endif

#include "char_dev.h"
#if defined(MODEM_LOG)
#include "modem_log.h"
#endif
/**---------------------------------------------------------------------------*
 **                                                 Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*-----------------------------SIO DEBUG MACOR-------------------------------*/
//#define _SIO_DEBUG_
//#define SIO_TIMEOUT_DEBUG


#define SIO_TRACE
#ifdef SIO_TRACE
#define SCI_SIO_TRACE   SCI_TRACE_LOW
#else
#define SCI_SIO_TRACE(...)
#endif

#if 0

#define SCI_PPP_TRACE   SCI_TRACE_LOW
#define SCI_PPP_TRACE_1   SCI_TRACE_LOW
#else
#define SCI_PPP_TRACE_1(...)
#define SCI_PPP_TRACE(...)

#endif



/*-----------------------------END--------------------------------------------*/
#if defined(GPS_SUPPORT) &&  defined(GPS_CHIP_VER_GREENEYE2)
#define DEBUG_PORT_SEND_BUF_SIZE     (3072+2048)
#else
#define DEBUG_PORT_SEND_BUF_SIZE     3072
#endif

#if defined(PRODUCT_DM)
#define DEBUG_PORT_REC_BUF_SIZE      (33*1024)
#else
#define DEBUG_PORT_REC_BUF_SIZE      (33*1024)   /*  For fix Bug 607186 set to 0x3000 , FDT Test Fail ,  huijie.feng*/
#endif
#define DATA_PORT_SEND_BUF_SIZE      4000
#define DATA_PORT_REC_BUF_SIZE       4000
#define DEBUG_LWORD_MARK             40

/*mix mode not used in mocor platform*/
#define DATA_PORT_MIXMODE_BUF_SIZE  (DATA_PORT_REC_BUF_SIZE>>3)

#if defined(PRODUCT_DM)
#define LOG_BUF_SIZE                 61440
#else
#ifdef TRACE_TYPE_ID
#define LOG_BUF_SIZE                 (30*1024)//(10240+2048)
#else
#define LOG_BUF_SIZE                ( (20480+2048)<<2 )
#endif
#endif

#define MAX_SIO_PPP_BITS_SHIFT       ( 9 )
#define MAX_SIO_PPP_FRAME_BUFF_SIZE  ( 1<<(MAX_SIO_PPP_BITS_SHIFT))  /*size:512*/
#define MAX_SIO_PPP_FRAME_SIZE       (DEBUG_PORT_SEND_BUF_SIZE>>MAX_SIO_PPP_BITS_SHIFT)

#define MAX_SIO_UART_FRAME_SHIFT     ( 11 )//( 8 )//10 for BT HCI LOG
#define MAX_SIO_UART_FRAME_BUFF_LEN  ( 1<<MAX_SIO_UART_FRAME_SHIFT)   /*2048B*/
#define MAX_SIO_UART_FRAME_NUM       (LOG_BUF_SIZE>>MAX_SIO_UART_FRAME_SHIFT)

#define MAX_SIO_USB_FRAME_SHIFT      ( 11 )
#define MAX_SIO_USB_FRAME_BUFF_LEN   ( 1<<MAX_SIO_USB_FRAME_SHIFT)   /*2048B*/
#define MAX_SIO_USB_FRAME_NUM        (LOG_BUF_SIZE>>MAX_SIO_USB_FRAME_SHIFT)

#define MAX_SIO_FRAME_NUM            MAX_SIO_UART_FRAME_NUM

#define DEBUG_HWORD_MARK             180
#define LOG_LWORD_MARK               100
#define LOG_MAX_FRM_SIZE             500

#define PPP_BUFFER_FULL              0
#define PPP_BUFFER_NOT_FULL          1
/*
if byte is 0x7e,0x7d, we need redefine it with format of '0x7d + XX'
the last one in buffer is 0x7d
*/
#define PPP_BUFFER_FULL_ERR1         2
/*
the last two byte in buffer is 0x7d + XX
*/
#define PPP_BUFFER_FULL_ERR2         3

#define SIO_MUTEX_MAX_TIME           (2000)                           // to avoid waiting mutex forever without exit

/*this two macor is used only for COM_DEBUG, !!!!!*/
/*the meaming of start point and end point of COM_DEBUG is contray with COM_DATA */
#define GET_BUF_FREE_SIZE(free_size, _buf)  \
    if (_buf->start_point >= _buf->end_point) \
    {\
        free_size = (int) ((int)_buf->size - _buf->start_point + _buf->end_point - 1);\
    }\
    else\
    {\
        free_size = (int) (_buf->end_point - _buf->start_point - 1);\
    }

#define GET_BUF_DATA_SIZE(data_size, _buf)  \
    if (_buf->start_point >= _buf->end_point) \
    {\
        data_size = (int) (_buf->start_point - _buf->end_point);\
    }\
    else\
    {\
        data_size = (int) ((int)(_buf->size) + _buf->start_point - _buf->end_point);\
    }


enum{
	DATA_NONE,
	DATA_DIAG,
	DATA_PPP,

	DATA_TOTAL,
};

/**---------------------------------------------------------------------------*
 **  Static Variables & functions
 **---------------------------------------------------------------------------*/
/*lint -esym(551,log_frm_flag)*/
LOCAL uint32    log_frm_flag;                                        // Log frame type. Indicate current sended frame
// status LOG_FRM_MED, LOG_FRM_NONE, LOG_FRM_END
LOCAL SIO_BUF_S log_buf_info;                                         // Log buffer control information
#ifdef TRACE_INFO_SUPPORT
LOCAL uint8     log_buf[LOG_BUF_SIZE];                                // Log buffer
#endif

// Because uartutil.c will use this array, we should not define it as static array
PUBLIC SIO_INFO_S      sio_port[MAX_LOGICAL_SIO_PORT_NUM];           // SIO information

LOCAL uint8  sio_atc_rec_buf[ATC_REC_BUF_SIZE];
#ifdef _U0_SOFTWARE_FLOW_CONTROL
LOCAL int     gXon_on       = 1;
LOCAL int     gRemoteXon_on = 1;
#endif

LOCAL uint8  sio_debug_send_buf[DEBUG_PORT_SEND_BUF_SIZE];            // Debug port tx buffer
LOCAL uint8  sio_debug_rec_buf[DEBUG_PORT_REC_BUF_SIZE];              // Debug port rx buffer
LOCAL uint8  sio_data_send_buf[DATA_PORT_SEND_BUF_SIZE];              // User port tx buffer
LOCAL uint8  sio_data_rec_buf[DATA_PORT_REC_BUF_SIZE];                // User port rx buffer

#ifdef PPP_USB
typedef enum
{
	PPP_READ = 0,
	ATC_READ = 1,
	ppp_INFO_MSG_MAX
} PPPREAD_MSG_SERVICE;
typedef struct PPPREAD_SIG_tag
{
    SIGNAL_VARS
	uint32  ppp_rec_num;
    uint8 port;
}PPPREAD_SIG_T;
LOCAL uint8  sio_data2_send_buf[DATA_PORT_SEND_BUF_SIZE];              // data2 port tx buffer
LOCAL uint8  sio_data2_rec_buf[DATA_PORT_REC_BUF_SIZE];                // data2 port rx buffer
LOCAL uint8  sio_atc2_rec_buf[ATC_REC_BUF_SIZE];
LOCAL const uint32 ppp_sig_ptr_size = sizeof(PPPREAD_SIG_T);
LOCAL BLOCK_ID PPPREADSVC = NULL;
#endif

LOCAL uint8  sio_mix_data_rec_buf[DATA_PORT_MIXMODE_BUF_SIZE];        // User port for data rx buffer //@Zhemin.Lin, 08/25/2003
LOCAL uint   g_end_point = 0;                                         // end_point of tx buffer used by COM_DATA.
#ifdef PPP_USB
LOCAL uint   g_end_point_2 = 0;                                         // end_point of tx buffer used by COM_DATA2
#endif
static SCI_TIMER_PTR s_sio_uart_sleep_timer = PNULL;
#define SIO_UART_SLEEP_TIMER_VALUE       5000//5s
/**---------------------------------------------------------------------------*
 **  Hardware functions
 **---------------------------------------------------------------------------*/
LOCAL SCI_EVENT_GROUP_PTR  comm_event = NULL;                         // Uart communication event
PUBLIC SCI_MUTEX_PTR   comm_mutex = NULL;                              // Uart communication event

//#ifdef _DATA_FLOW_CONTROL
LOCAL SCI_TIMER_PTR   Space_timer =NULL;                              // timer to check the space of Rx buffer for COM_DATA
LOCAL void Space_Check (void);
#ifdef PPP_USB
LOCAL SCI_TIMER_PTR   Space_timer_2 =NULL;                              // timer to check the space of Rx buffer for COM_DATA2
LOCAL void Space_Check_2 (void);

#endif

#ifdef PPP_USB
#define     SPACE_CHECK_DELAY       10                               // time delay of space check of Rx buffer for COM_DATA
#else
#define     SPACE_CHECK_DELAY       300                               // time delay of space check of Rx buffer for COM_DATA2
#endif
#define     EMPTY_SPACE_WATER_HIGH_MARK  (DATA_PORT_REC_BUF_SIZE>>2)                               // empty sapce water mark of rx buffer
//#endif

// @Tao.Zhou, limit the frequency of sio irq, 10/18/2006
LOCAL SCI_TIMER_PTR  s_sio_irqen_timer = PNULL;

LOCAL  BOOLEAN   s_is_at_from_debug_port = FALSE;
#ifdef PPP_USB
LOCAL  BOOLEAN   s_is_at_from_data2_port = FALSE;
#endif
LOCAL BOOLEAN s_sio_rx_int_is_disabled = FALSE;

#if defined(MODEM_LOG)
LOCAL   T_TRA_MODEM_LOG *p_tra_cp_buffInfo = NULL;
LOCAL   T_TRA_MODEM_LOG *p_tra_zsp_buffInfo = NULL;
#endif


#ifdef _SIO_DEBUG_
void sio_test_init (void);
static uint32 max_time = 0;
#endif

#ifdef PPP_USB

extern  int32 TCPIP_ResetPPPInterface( void ) ;
#endif
void sio_ppp_rx_buff_free(void);

/**---------------------------------------------------------------------------*
 **  Protocol functions
 **---------------------------------------------------------------------------*/
#define  SIO_FRAME_OK     0
#define  SIO_FRAME_ESCAPE 1
#define  SIO_FRAME_NORMAL 2

typedef enum
{
    SIO_FRAME_NONE = 0,
    SIO_FRAME_HEAD,
    SIO_FRAME_ERROR
} SIO_FRAME_TYPE_E;

typedef enum
{
    SIO_CHAR_ESCAPE,
    SIO_CHAR_NORMAL
} SIO_CHAR_TYPE_E;

LOCAL SIO_FRAME_TYPE_E PPP_frame_type; // PPP frame type
LOCAL SIO_CHAR_TYPE_E  char_mask_mode; // Character mode, check if there is a
// masked character here

#if defined(PRODUCT_DM)
extern int SCI_SendLogPacket (uint32 *head_ptr, // Message head
                              int    head_len,   // Message head len
                              uint32 *body_ptr,  // Message body, maybe NULL
                              int    body_len    // Message bodu len
                             );
extern int SCI_SendTracePacket (uint8 *src, // Message head
                                uint32    len   // Message head len
                               );
#endif


//add for debug
#if 1
#define REG_DEBUG_HOST_BASE        0x5017F000
typedef volatile unsigned int REG32;

typedef volatile struct
{
    REG32                          cmd;                          //0x00000000
    REG32                          data_REG;                     //0x00000004
    REG32                          event;                        //0x00000008
    REG32                          mode;                         //0x0000000C
    REG32                          h2p_status_REG;               //0x00000010
    REG32                          p2h_status_REG;               //0x00000014
    REG32                          irq;                          //0x00000018
} HWP_DEBUG_HOST_T;

#define hwp_debugHost              ((HWP_DEBUG_HOST_T*) REG_ACCESS_ADDRESS(REG_DEBUG_HOST_BASE))

 void mon_Event(uint32 evt)
{
   volatile uint32 count=0;
    //  wait for host sema to be free
    //  or time out.
    while((hwp_debugHost->event != 0) &&(count<30000))
    {
        count++;
    }

    hwp_debugHost->event = evt;
    for(count=0;count<10000;count++);
}

#endif


/**---------------------------------------------------------------------------*
 **  Ring buffer management functions
 **---------------------------------------------------------------------------*/
LOCAL int ring_add (SIO_BUF_S *pRing, int ch);
LOCAL int ring_remove (SIO_BUF_S *pRing);

/**---------------------------------------------------------------------------*
 **  User port variables & functions
 **---------------------------------------------------------------------------*/
//#define            BACKSPACE_CHAR          0x08
//#define            END_CHAR                0x0D
//#define            PLUS_CHAR               '+'

//#define            MODE_CHANGE_DELAY       1000

#ifdef PPP_USB
LOCAL uint32       user_port_mode_2;                                // ATC_MODE, DATA_MODE;
LOCAL char         atc_end_ch1[COM_MAX_PORT_NUM];                                   // End char of ATC;
LOCAL char         atc_end_ch2[COM_MAX_PORT_NUM];                                   // End char of ATC;
LOCAL char         atc_bs_ch[COM_MAX_PORT_NUM];                                     // Backspace char of ATC
LOCAL BOOLEAN  atc_is_echo[COM_MAX_PORT_NUM];

#else
LOCAL char         atc_end_ch1;                                   // End char of ATC;
LOCAL char         atc_end_ch2;                                   // End char of ATC;
LOCAL char         atc_bs_ch;                                     // Backspace char of ATC
LOCAL BOOLEAN  atc_is_echo;
#endif
LOCAL uint32       user_port_mode;                                // ATC_MODE, DATA_MODE;

LOCAL ATC_CALLBACK AT_callback_ind = NULL;                        // Pointer to the AT callback
LOCAL uint32       cur_atc_frame_size;                            // Current Enqueued atc frame size
LOCAL SCI_TIMER_PTR   ATC_timer        = NULL;                    // ATC timer

#ifdef PPP_USB
LOCAL uint32       cur_atc_frame_size_2;                            // Current Enqueued atc frame size
LOCAL SCI_TIMER_PTR   ATC_timer_2        = NULL;                    // ATC timer in data2 port!
#endif


LOCAL uint32        plus_num;                                      // Number of '+'
LOCAL uint32       last_rx_time;                                  // Last receive time in user port!
#ifdef PPP_USB
LOCAL uint32       last_rx_time_2;                                  // Last receive time in data2 port!
#endif

LOCAL uint32       IsChangeBaudrate      = 0;
LOCAL void ATC_CheckPPP (uint32 wParam);                           // Timer callback function to check '+++'

#ifdef PPP_USB

LOCAL void ATC_CheckPPP_2 (uint32 wParam);                           // Timer callback function to check '+++'

#endif

typedef struct SIO_FRAME_PPP_Tag
{
    uint16 count;
    uint16 head;
    uint16 tail;
    uint16 list_size;
    DATA_FRAME_T  frame_list[MAX_SIO_PPP_FRAME_SIZE];
} SIO_FRAME_PPP_T;

typedef struct SIO_USB_FRAME_CTRL_Tag
{
    uint16 count;
    uint16 head;
    uint16 tail;
    uint16 list_size;
    DATA_FRAME_T  frame_list[MAX_SIO_FRAME_NUM];
} SIO_FRAME_CTRL_T;

#ifdef DSP_USB_LOG
extern SIO_DSP_FRAME_CTRL_T    s_sio_dsp_frame_ctrl;  //DSP log buf information
PUBLIC uint32 armLogSendContinueCnt = 0;  //arm log continue send out count,must be cleared to 0 when other log be sent out
PUBLIC uint32 dspLogSendContinueCnt = 0;   //dsp log continue send out count,must be cleared to 0 when other log be sent out
#elif defined(BT_USB_LOG)
extern SIO_BT_FRAME_CTRL_T    s_sio_bt_frame_ctrl;  //DSP log buf information
PUBLIC uint32 armLogSendContinueCnt = 0;  //arm log continue send out count,must be cleared to 0 when other log be sent out
PUBLIC uint32 btLogSendContinueCnt = 0;   //bt log continue send out count,must be cleared to 0 when other log be sent out
#endif

typedef enum SIO_DATA_TYPE_E
{
    INVAIL_DATA_TYPE,
    NORMAL_DATA_TYPE,
    CMD_DATA_TYPE,
    DSP_DATA_TYPE,
    BT_DATA_TYPE
} SIO_DATA_TYPE_E;


typedef struct SIO_DMA_DATA_Tag
{
    uint8 *base_addr;
    uint32 data_len;
    SIO_DATA_TYPE_E  type;
} SIO_DMA_DATA_T;

LOCAL SIO_FRAME_CTRL_T   s_sio_frame_ctrl;
LOCAL SIO_FRAME_PPP_T    s_sio_ppp_ctrl;

PUBLIC uint32 s_sio_tx_dma_is_start = FALSE;
PUBLIC uint32 s_sio_tx_debug_2_is_start = FALSE;
LOCAL BOOLEAN s_sio_has_cmd_req = FALSE;
LOCAL SIO_DMA_DATA_T   s_sio_dma_data;

LOCAL uint32 s_sio_usb_padding_word =  0x7E7E7E7E;
LOCAL BOOLEAN  s_sio_is_need_pad_flag = FALSE;   //Only For USB
LOCAL BOOLEAN s_sio_packet_is_sending = FALSE;  // 1: finish, 0:acting
#if defined(GPS_SUPPORT) &&  defined(GPS_CHIP_VER_GREENEYE2)
#define SIO_SERIES_LOST_THLD   5
#define SIO_DFAULT_WAIT_TIME   5

LOCAL uint32  s_sio_packet_lost_count = 0;
LOCAL uint32  s_sio_packet_series_lost = 0;
LOCAL uint32  s_sio_send_wait_time = SIO_DFAULT_WAIT_TIME;
#endif
LOCAL void SIO_Usb_TxDma_CallBack (void);
LOCAL void SIO_FreeDmaFrameData (SIO_DMA_DATA_T *data_frame_ptr);
LOCAL BOOLEAN SIO_NewDmaFrameData (SIO_DMA_DATA_T *data_frame_ptr);

/*@jiajiang.lu 2018-07-13 */
#ifdef PPP_USB
LOCAL void SIO_Tx_Send (uint8 port_index);
#else
LOCAL void SIO_Tx_Send (void);
#endif


//debug
LOCAL uint32 s_debug_sio_dma_tx_start_cnt = 0; /*lint -esym(551,s_debug_sio_dma_tx_start_cnt)*/
LOCAL uint32 s_debug_sio_dma_tx_call_cnt  = 0; /*lint -esym(551,s_debug_sio_dma_tx_call_cnt) */
LOCAL uint32 s_sio_debug_dma_channel      = 0; /*lint -esym(551,s_sio_debug_dma_channel)     */
LOCAL uint32 s_sio_debug_uart_tx_addr     = 0; /*lint -esym(551,s_sio_debug_uart_tx_addr)    */
LOCAL uint32 s_sio_debug_com              = 0; /*lint -esym(551,s_sio_debug_com)             */
LOCAL uint32 s_usb_trace_cnt              = 0; /*lint -esym(551,s_usb_trace_cnt)             */
LOCAL uint32 s_debug_sio_usb_dam_call_cnt = 0; /*lint -esym(551,s_debug_sio_usb_dam_call_cnt)*/
LOCAL uint32 s_trace_count                = 0; /*lint -esym(551,s_trace_count)               */
LOCAL uint32 s_vaild_trace_cnt            = 0; /*lint -esym(551,s_vaild_trace_cnt)           */
LOCAL uint32 s_trace_too_long_cnt         = 0; /*lint -esym(551,s_trace_too_long_cnt)        */
LOCAL uint32 s_trace_no_space_cnt         = 0; /*lint -esym(551,s_trace_no_space_cnt)        */
LOCAL uint32 s_log_count                  = 0; /*lint -esym(551,s_log_count)                 */
LOCAL uint32 s_vaild_log_count            = 0; /*lint -esym(551,s_vaild_log_count)           */
LOCAL uint32 s_log_too_long_cnt           = 0; /*lint -esym(551,s_log_too_long_cnt)          */
LOCAL uint32 s_lost_frame_cnt             = 0; /*lint -esym(551,s_lost_frame_cnt)            */
LOCAL uint32 s_new_frame_cnt              = 0; /*lint -esym(551,s_new_frame_cnt)             */
LOCAL uint32 s_free_frame_cnt             = 0; /*lint -esym(551,s_free_frame_cnt)            */

//Add for USB Trace  End============



typedef struct FRM_HEADtag
{
    uint16 reserved1;
    uint8  end_flag;
    uint8  start_flag;
    uint32 seq_num;
    uint16 len;
    uint8  type;
    uint8  subtype;
    uint16 lost_frm_num;
} FRM_HEAD_T;

typedef struct SME_HEADtag
{
    uint32 seq_num;
    uint16 len;
    uint8  type;
    uint8  subtype;
    uint16 lost_frm_num;
} SME_HEAD_T;

LOCAL FRM_HEAD_T data =
{
    0x0,   // Reserved
    0x7E,  // End flag
    0x7E,  // Begin Flag
    0x0,   // sequnce number
    0x0,   // Warning:
    // Len  Because we can not set this value when we add
    //      information here! We have to set it to ZERO, channel
    //      server and logel will care it more!
    DIAG_LOG_A, // Type
    0x0,   // Subtype
    0x0,   // Lost frame number
};

//@Zhemin.Lin, CR:MS00004031, 08/28/2003,  begin
LOCAL void DefaultAtCallback (uint32 len);


#ifdef PPP_USB
//@Shijun.Cui 2005-02-02
LOCAL void DefaultAtHandle (uint32 len,uint8 sio_port);

#define DO_ATC_CALLBACK(len,port) \
    if ( NULL == AT_callback_ind) \
    {\
        DefaultAtHandle(len,port);\
    }\
    else \
    {\
        (*AT_callback_ind)(len,port);\
    }
#else
//@Shijun.Cui 2005-02-02
LOCAL void DefaultAtHandle (uint32 len);

#define DO_ATC_CALLBACK(len) \
    if ( NULL == AT_callback_ind) \
    {\
        DefaultAtHandle(len);\
    }\
    else \
    {\
        (*AT_callback_ind)(len);\
    }
#endif
//@Zhemin.Lin, CR:MS00004031, 08/28/2003,  end

//@Zhemin.Lin, CR:MS00004213, 2003/08/22, begin

#define SIO_DATA_PACKET    (0xDD    )
#define SIO_ATC_PACKET     (0xAA    )
#define SIO_ATC_PATTERN    (0x1D<<3 )
#define SIO_DATA_PATTERN   (0<<3    )
#define SIO_STATUS_PATTERN (1<<3    )
#define SIO_READY_PATTERN  (2<<3    )
#define SIO_BUSY_PATTERN   (3<<3    )

//the packet head struct of AT mix mode
#define MIX_PACKET_HEAD_LENGHT 3

#define MIX_PROTO_FIX_HEAD_LEN  4

typedef struct
{
    uint8 type;
    uint8 length_lsb;
    uint8 subtype;
} MIX_PACKET_HEAD_T;

typedef struct
{
    //@Zhemin.Lin, 09/12/2003, CR:MS00004678, must use os_api interface
    SCI_MUTEX_PTR mutexforwrite;        //mutex for write
    SIO_BUF_S data_rx_buf;
    uint32 recvlen;                //length has been received
    uint32 datalen;
    uint8 checksum;
    MIX_PACKET_HEAD_T packet;    //record the current receive packet head
    uint32 buf_used;
    uint8   sio_temp_buf[DATA_PORT_MIXMODE_BUF_SIZE];
} MIX_MODE_INFO;

LOCAL MIX_MODE_INFO mix_mode_info;

/*lint -esym(551,notify_handle)*/
/*lint -esym(551,data_recv_handle)*/
LOCAL SIO_NOTIFY_HANDLE notify_handle = NULL;  //notify handle
LOCAL ATC_CALLBACK data_recv_handle = NULL;  //data receive handle
LOCAL SIO_ERROR_HANDLE link_error_handle = NULL; // link error handle

//@Zhemin.Lin, 09/12/2003, CR:MS00004678, must use os_api interface, begin
//get mutex
#define SIO_SEMTAKE (SCI_GetMutex(mix_mode_info.mutexforwrite, SCI_WAIT_FOREVER))
//put mutex
#define SIO_SEMGIVE (SCI_PutMutex(mix_mode_info.mutexforwrite))
//@Zhemin.Lin, 09/12/2003, CR:MS00004678, must use os_api interface, end

#define DO_NOTIFY_HANDLE(notify_id, param)  \
    if(NULL != notify_handle) \
        (*notify_handle)(notify_id, param)

#define DO_ERROR_HANDLE(port_id, error_type) \
    if (NULL != link_error_handle) \
        (link_error_handle)(port_id, error_type)

#define RESET_MIX_MODE_INFO \
    mix_mode_info.checksum = 0;\
    mix_mode_info.datalen = 0;\
    mix_mode_info.recvlen = 0;\
    mix_mode_info.buf_used = 0;

LOCAL void DefaultDataCallback (uint32 len);
#define DO_DATA_CALLBACK(len) \
    if (NULL == data_recv_handle) \
    {\
        DefaultDataCallback(len);\
    }\
    else\
    {\
        (*data_recv_handle)(len);\
    }


////////////////add new/////////////////////////
#define MAX_RXBUF_SIZE          0x200/*for usb2.0 driver*/
#define MAX_FIFO_SIZE  0x80  /*should be in header file*/
__align (4) LOCAL uint8 s_uart_debug_rxbuf[MAX_RXBUF_SIZE];
__align (4) LOCAL uint8 s_uart_data_rxbuf[MAX_RXBUF_SIZE];
__align (4) LOCAL uint8 s_uart_data_txbuf[MAX_FIFO_SIZE];

#ifdef PPP_USB
__align (64) uint8 s_uart_data2_rxbuf[MAX_RXBUF_SIZE];
__align (64) uint8 s_uart_data2_txbuf[MAX_FIFO_SIZE];

#define PPP_TXBUF_CNT 16
#define PPP_TXBUF_SIZE 2048
#define PPP_TXBUF_MASK (PPP_TXBUF_CNT - 1)
#define PPP_RXBUF_SIZE 64*1024 //16384 //16k
#define PPP_RXBUF_MASK (PPP_RXBUF_SIZE - 1)

uint8* p_ppp_rx_buf = NULL;
uint8* p_ppp_rx_buf_tmp = NULL;

//__align (64) uint8 s_ppp_rx_buf[PPP_RXBUF_SIZE];
__align (64) uint8 s_ppp_dma_txbuf[PPP_TXBUF_CNT][PPP_TXBUF_SIZE];
uint32 g_pppbuf_alloc = 0;
uint32 g_pppbuf_free = 0;
uint32 g_ppp_rx_ridx = 0;
uint32 g_ppp_rx_widx = 0;
#endif

LOCAL const SIO_PORT_CVT_T s_sio_port_cvt_tbl[MAX_SIO_PORT_NUM] =
{
    /*----sio indx------phy index---------*/
    { COM0     ,   UART_COM0 },
    { COM1     ,   UART_COM1 },
    { COM2     ,   UART_COM2 },
    { COM3     ,   UART_COM3 },
    { VIR_COM0 ,   USB_COM0  },    //AT DATA
    { VIR_COM1 ,   USB_COM1  },    // AP DEBUG
    { DSK_COM0 ,   CARD_COM0 },
    { DSK_COM1 ,   CARD_COM1 },
    { VIR_COM2 ,   DUN_COM   },
    { VIR_COM3 ,   USB_COM4  },  // PPP_AT
    { VIR_COM4 ,   USB_COM3  },  // CP DEBUG

};

#define DECLARE_TEMP_VARS \
    int          i;\
    int          free_count = fifo_size - snd_size;\
    int          data_size; \
    register int temp_end_point;\
    uint8        *tx_buf_ptr;

LOCAL uint32 SIO_DebugPortHdl (uint32 event);

LOCAL uint32 SIO_DataPortHdl (uint32 event);
#ifdef PPP_USB
LOCAL uint32 SIO_DataPortHdl_2 (uint32 event);
#endif

/*@jim.zhang CR:MS8384 2004-04-02 */
extern uint8   gprs_debug;  // 1: GPRS_DEBUG  0: not defined GPRS_DEBUG.
/* end CR:MS8384 */

typedef struct
{
    MSG_HEAD_T head;
    uint8     buf[4096];
    uint32    cur_offset;
} PPP_BUF_T;

LOCAL PPP_BUF_T ppp_buf;
PUBLIC uint32 SIO_UpdatePPPBufSize (void);
//PUBLIC void SIO_ClearPPPBuf (void);
//PUBLIC uint8 *SIO_GetPPPBuf (void);

LOCAL BOOLEAN s_sio_uart1_rx_wakeup = SCI_FALSE;
#ifdef PPP_USB
LOCAL int s_last_cmd_pos[COM_MAX_PORT_NUM] = {0xffff};  //sometimes, last cmd not read by atc yet

#else
LOCAL int s_last_cmd_pos = 0xffff;  //sometimes, last cmd not read by atc yet
#endif
static void SIO_UartSleepTimerExpired(
    uint32 lparam
)
{
    if (sio_port[COM_DATA].phy_port ==UART0_E ||sio_port[COM_DATA].phy_port ==UART1_E)
    {
	//DPSLP_UART_EnableApbSleep(sio_port[COM_DATA].phy_port, ENABLE_APB_SLEEP);//bringup_tmp
    }

    if (sio_port[COM_DEBUG].phy_port ==UART0_E ||sio_port[COM_DEBUG].phy_port ==UART1_E)
    {
	//DPSLP_UART_EnableApbSleep(sio_port[COM_DEBUG].phy_port, ENABLE_APB_SLEEP);//bringup_tmp
    }
}

/*****************************************************************************/
// Description :  remote device wakeup UART by RXD
// Note   :    	ONLY support UART1 now; phy_port was reserved for future use
/*****************************************************************************/
PUBLIC void SIO_SetUartRxWakeup(uint32 phy_port, BOOLEAN mode)
{
   s_sio_uart1_rx_wakeup = mode;
}
LOCAL BOOLEAN SIO_GetUartRxWakeup(uint32 phy_port)
{
   return s_sio_uart1_rx_wakeup;
}


uint32 SIO_PhyPortfromIndex (uint32 index)
{
    uint32 port_num = 0;

    //here it's useless to judge availability of param

    if (s_sio_port_cvt_tbl[index].sio_port_index == index)
    {
        port_num = s_sio_port_cvt_tbl[index].phy_port_index;
    }
    else
    {
        SCI_ASSERT (0);/*assert to do*/
    }

    return port_num;
}

LOCAL void SIO_DefaultFlush (uint32 portnum)
{
    //dummy for uart
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     int CheckInput()                                                      *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function checks if the input char is a valid PPP char, and       *
 **     returns the PPP frame status.                                         *
 **                                                                           *
 ** OUTPUT                                                                    *
 **     None                                                                  *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Returns SIO_FRAME_OK if a completed PPP frame is received, the frame  *
 **     is like this 0x7E, ..., 0x7E; returns SIO_FRAME_ESCAPE if a masked    *
 **     character 0x7D or 0x7E is received; returns SIO_FRAME_NORMAL 2 if it  *
 **     is a normal useful char.                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     PPP_frame_type,  char_mask_mode                                       *
 **                                                                           *
 **---------------------------------------------------------------------------*/
LOCAL int CheckInput (int *nchar  // Character recevied from serial port
                     )
{
    // Initial value for status, we do not want this char
    int status = SIO_FRAME_ESCAPE;

    if (*nchar == FLAG_BYTE)
    {
        switch (PPP_frame_type)
        {
            case SIO_FRAME_NONE:
                // Now we begin to receive the PPP frame
                PPP_frame_type     = SIO_FRAME_HEAD;
                break;
            case SIO_FRAME_HEAD:
                // Now is the end of the PPP frame
                {
                    status         = SIO_FRAME_OK;
                    PPP_frame_type = SIO_FRAME_NONE;
                }
                break;
            case SIO_FRAME_ERROR:
            default:
                PPP_frame_type = SIO_FRAME_NONE;
                break;
        }

        return status;
    }

    /* Get Escape */
    if (*nchar == ESCAPE_BYTE)
    {
        // It is a masked byte, we do not need it
        char_mask_mode = SIO_CHAR_ESCAPE;
        return status;
    }

    /* Last time is escape */
    if (char_mask_mode == SIO_CHAR_ESCAPE)
    {
        *nchar         = *nchar ^ COMPLEMENT_BYTE;
        char_mask_mode = SIO_CHAR_NORMAL;
    }

    switch (PPP_frame_type)
    {
        case SIO_FRAME_NONE:
            // we are not ready to receive this char
            PPP_frame_type = SIO_FRAME_ERROR;
            break;
        case SIO_FRAME_HEAD:
            status   = SIO_FRAME_NORMAL;
            break;
        case SIO_FRAME_ERROR:
        default:
            break;
    }

    return status;
}

#if defined(PRODUCT_DM)
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void WriteTraceMsgToLogBuf()                                          *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function does not encode message since we send ASCII code        *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return the size put into the SIO buffer.                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
LOCAL int WriteTraceMsgToLogBuf (SIO_BUF_S *buf_ptr, uint8 *src, int size)
{
    register int    i;
    register int    start, end;
    uint8           *buf;
    register int    buf_size;

    /*
        Warning:
        We must make sure there is no interrupt here!
    */

    /* We can make sure the call routine is right! */
    //    SCI_ASSERT(buf_ptr != NULL);
    //    SCI_ASSERT(src     != NULL);

    start    = buf_ptr->start_point;
    end      = buf_ptr->end_point;
    buf      = buf_ptr->sio_buf_ptr;
    buf_size = buf_ptr->size;

    for (i = 0; i < size; i++)
    {
        buf[start++] = *src++;

        if (start >= buf_size)
        {
            start = 0;
        }

        /* Well, there is some problem when we write mask word,
           we have only DEBUG_LOW_MARK space for mask word.*/
        SCI_ASSERT (start != end);/*assert verified*/
    }

    buf_ptr->start_point = start;

    return size;


}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void EncodeMsg()                                                      *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function encode message as a PPP fram body, masked 0x7E to 0x7D  *
 **     0x5E; 0x7D to 0x7D 0x5D.                                              *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return the size put into the SIO buffer.                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
LOCAL int EncodeMsg (SIO_BUF_S *buf_ptr, uint8 *src, int size)
{
    register int    data_size = 0;
    register int    start, end;
    uint8           *buf;
    register int    buf_size;

    /*
        Warning:
        We must make sure there is no interrupt here!
    */

    /* We can make sure the call routine is right! */
    //    SCI_ASSERT(buf_ptr != NULL);
    //    SCI_ASSERT(src     != NULL);

    start    = buf_ptr->start_point;
    end      = buf_ptr->end_point;
    buf      = buf_ptr->sio_buf_ptr;
    buf_size = buf_ptr->size;


    while (size != 0)
    {
        switch (*src)
        {
            case FLAG_BYTE:
            case ESCAPE_BYTE:
                buf[start++] = ESCAPE_BYTE;

                if (start >= buf_size)
                {
                    start = 0;
                }

                buf[start++] = *src ^ COMPLEMENT_BYTE;
                data_size += 2;
                break;
            default:
                buf[start++] = *src;
                data_size++;
                break;
        }

        src++;

        if (start >= buf_size)
        {
            start = 0;
        }

        /* Well, there is some problem when we write mask word,
           we have only DEBUG_LOW_MARK space for mask word.*/
        SCI_ASSERT (start != end);/*assert verified*/

        size--;
    }

    buf_ptr->start_point = start;


    return data_size;

}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void EncodeFrmHead()                                                  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function encode the head of the message as a PPP frame head,     *
 **     masked 0x7E to 0x7D 0x5E; 0x7D to 0x7D 0x5D.                          *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return the size put into the SIO buffer.                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
LOCAL void EncodeFrmHead (const unsigned char *p_data, int size)
{
    int    i;

    int    start = 0;
    uint8  *buf = PNULL;
    int    buf_size = 0;
    uint32 data_size = 0;

    start    = log_buf_info.start_point;
    buf      = log_buf_info.sio_buf_ptr;
    buf_size = log_buf_info.size;

    if (size == 12)
    {
        for (i = 0; i < 2; i++)
        {
            buf[start++] = *p_data++;

            if (start >= buf_size)
            {
                start = 0;
            }
        }

    }
    else
    {
        buf[start++] = *p_data++;

        if (start >= buf_size)
        {
            start = 0;
        }
    }

    for (i = 0; i < 10; i++)
    {
        switch (*p_data)
        {
            case FLAG_BYTE:
            case ESCAPE_BYTE:
                buf[start++] = ESCAPE_BYTE;

                if (start >= buf_size)
                {
                    start = 0;
                }

                buf[start++] = *p_data ^ COMPLEMENT_BYTE;
                data_size += 2;
                break;
            default:
                buf[start++] = *p_data;
                data_size++;
                break;
        }

        p_data++;

        if (start >= buf_size)
        {
            start = 0;
        }
    }

    log_buf_info.start_point = start;

}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     int EncodeMsg_Dump()                                                  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function receives and encodes the data from input source point,  *
 **     send it to serial port directly.                                      *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return the size of byte written to the port.                          *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *
 **                                                                           *
 **---------------------------------------------------------------------------*/
LOCAL int EncodeMsg_Dump (uint32 port, uint8 *src, int size)
{
    uint32 data_size = 0;
    uint32 phy_port;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (SIO_ALREADY_OPEN != sio_port[port].open_flag)
    {
        return 0;
    }

    phy_port = SIO_PhyPortfromIndex (sio_port[port].phy_port);

    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        return 0;
    }

    while (size > 0)
    {
        switch (*src)
        {
            case FLAG_BYTE:
            case ESCAPE_BYTE:
                (sio_port[port].sio_op.put_char) (phy_port,ESCAPE_BYTE);
                (sio_port[port].sio_op.put_char) (phy_port,*src ^ COMPLEMENT_BYTE);

                data_size += 2;
                break;
            default:
                (sio_port[port].sio_op.put_char) (phy_port,*src);
                data_size++;
                break;
        }

        src++;
        size--;
    }

    return data_size;
}

#endif

/*
for arm , ps log and trace handle
*/
LOCAL int32 SIO_WriteLogToLogBuf (uint8 *des, uint32 len, uint8 *src, uint32 size)
{
    uint32 cnt = 0;
    uint8 *buf_ptr = des;

    while (size--)
    {
        switch (*src)
        {
            case FLAG_BYTE:
            case ESCAPE_BYTE:
                buf_ptr[cnt++] = ESCAPE_BYTE;

                if (cnt >= len)
                {
                    return -1;
                }

                buf_ptr[cnt++] = *src ^ COMPLEMENT_BYTE;

                if (cnt >= len)
                {
                    return -1;
                }

                break;
            default:
                buf_ptr[cnt++] = *src;

                if (cnt >= len)
                {
                    return -1;
                }

                break;
        }

        src++;
    }


    return (int32) cnt;
}

/*
for arm , ps log and trace handle
*/
LOCAL  uint32 SIO_WriteTraceToLogBuf (uint8 *des, uint32 len, uint8 *src, uint32 size)
{
    return SIO_WriteLogToLogBuf (des,len,src,size);
}

/*
for arm , ps log and trace handle
*/
LOCAL uint32 SIO_WriteFrmHeadToLogBuf (uint8 *des, uint32 len, uint8 *src, uint32 size)
{
    int    i;
    uint    start = 0;
    uint8  *buf_ptr = des;


    if (size == 12)
    {
        return start;
    }
    else
    {
        buf_ptr[start++] = *src++;
    }

    for (i = 0; i < 10; i++)
    {
        switch (*src)
        {
            case FLAG_BYTE:
            case ESCAPE_BYTE:
                buf_ptr[start++] = ESCAPE_BYTE;
                buf_ptr[start++] = *src ^ COMPLEMENT_BYTE;
                break;
            default:
                buf_ptr[start++] = *src;
                break;
        }

        src++;

    }

    return start;
}

PUBLIC uint32 SIO_UpdatePPPBufSize()
{
    ppp_buf.head.len = sizeof (MSG_HEAD_T) + ppp_buf.cur_offset;

    return ppp_buf.head.len;
}

PUBLIC void SIO_ClearPPPBuf()
{
    ppp_buf.cur_offset = 0;
}

PUBLIC uint8 *SIO_GetPPPBuf()
{
    return ( (uint8 *) &ppp_buf);
}

/*@jim.zhang CR:MS10775 2004-07-14 */

/*
** Enable / Disable the interrupt used by PPP
*/
LOCAL void SIO_Tx_Int_Enable (uint32 port,int enable_flag)
{
    uint32 phy_port = 0;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    phy_port = sio_port[port].phy_port;

    //SCI_ASSERT (phy_port < MAX_SIO_PORT_NUM);
    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        return ;
    }

    (sio_port[port].sio_op.tx_enable) (SIO_PhyPortfromIndex (phy_port),enable_flag);

   #if defined(PLATFORM_UWS6121E)
   if(enable_flag /*&& port == COM_DATA*/)
   {
   	#ifdef PPP_USB
	     SIO_Tx_Send((uint8)port);
	#else
	     SIO_Tx_Send();
	#endif
   }
   #endif

}

/*
** Enable / Disable the interrupt used by PPP, this function is only call once by PS project,since it has been
            replaced by SIO_Tx_Int_Enable().
*/

PUBLIC void uart_ppp_tx_int_enable (int enable_flag)
{
    uint32 phy_port;
#ifdef PPP_USB
    uint8 port_index= COM_DATA2;
#else
     uint8 port_index= COM_DATA;
#endif

    phy_port = sio_port[port_index].phy_port;

    if ( (phy_port < MAX_SIO_PORT_NUM) && (SIO_ALREADY_OPEN == sio_port[port_index].open_flag))
    {
        (sio_port[port_index].sio_op.tx_enable) (SIO_PhyPortfromIndex (phy_port),enable_flag);
    }

   #if defined(PLATFORM_UWS6121E)
   if(enable_flag)
   {

   	#ifdef PPP_USB
	     SIO_Tx_Send(port_index);
	#else
	     SIO_Tx_Send();
	#endif
   }
   #endif

}

/*
** Enable / Disable the interrupt used by PPP
*/
LOCAL void SIO_Rx_Int_Enable (uint32 port,int enable_flag)
{
    uint32 phy_port = 0;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/
   SCI_PPP_TRACE("ppp:SIO_Rx_Int_Enable,port%d",port);

    phy_port = sio_port[port].phy_port;

    //SCI_ASSERT (phy_port < MAX_SIO_PORT_NUM);
    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        return ;
    }
    if(sio_port[port].sio_op.rx_enable)
    {
    (sio_port[port].sio_op.rx_enable) (SIO_PhyPortfromIndex (phy_port),enable_flag);
    }
}

/*
* get phy port tx fifo available size,
*/
PUBLIC uint32 SIO_GetTxFifoCnt (uint32 port)
{
    uint16 fifocnt = 0;
    uint32 phy_port;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    phy_port = sio_port[port].phy_port;

    //phy port is valid
    //SCI_ASSERT (phy_port < MAX_SIO_PORT_NUM);
    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        return 0;
    }

    //get remain avaliable fifo size
    fifocnt = (sio_port[port].sio_op.get_txfifocnt) (SIO_PhyPortfromIndex (phy_port));

    return fifocnt;
}
LOCAL uint32 SIO_GetRxFifoCnt (uint32 port)
{
    uint16 fifocnt;
    uint32 phy_port;

    phy_port = sio_port[port].phy_port;
    fifocnt = UART_HAL_FifoSize (phy_port);

    if (phy_port >= MAX_SIO_PORT_NUM)
    {   // if invalid port, return default COM_DEBUG value
        return fifocnt;
    }
    if(sio_port[port].sio_op.get_rxfifocnt)
    {
    	fifocnt = (sio_port[port].sio_op.get_rxfifocnt) (SIO_PhyPortfromIndex (phy_port));
    }
    return fifocnt;
}
/*****************************************************************************/
// Description :    Change serial port's mode.
// Global resource dependence :
// Author :         Jim.zhang
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_ChangeMode (//return the operate result:success or fail
    uint32 mode                 //uart mode command.
)
{
#if 0
    uint32        curr_mode = 0xFF;
    SIO_CONTROL_S m_dcb;
    uint8         debug_phy_port;
    uint8         data_phy_port;
#endif
    SCI_ASSERT (mode < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    /*below code is not used now, so return success directly without do anything*/
    return SCI_SUCCESS;
#if 0
    /*
    * current mode is uart mode,COM_DEBUG/COM_USER can change each other
    */
    curr_mode = REFPARAM_GetUartMode();

    if (mode == curr_mode)
    {
        return SCI_SUCCESS;
    }

#ifdef MUX_SUPPORT
    return SCI_SUCCESS;
#endif

    SCI_DisableIRQ();

    SIO_Close (COM_DEBUG);
    SIO_Close (COM_USER);

    m_dcb.flow_control  = 0;
    debug_phy_port = REFPARAM_GetDebugPortPhyNo();
    data_phy_port  = REFPARAM_GetDataPortPhyNo();

    //creat COM_DEBUG
    m_dcb.baud_rate      = SIO_GetBaudRate (COM_DEBUG);
    SIO_Create (COM_DEBUG, data_phy_port, &m_dcb);

    //creat COM_DATA
    m_dcb.baud_rate      = SIO_GetBaudRate (COM_DATA);
    SIO_Create (COM_DATA, debug_phy_port, &m_dcb);
    SCI_RestoreIRQ();

    REFPARAM_SetUartMode (mode);
    REFPARAM_SetDebugPortPhyNo (data_phy_port);
    REFPARAM_SetDataPortPhyNo (debug_phy_port);

    return SCI_SUCCESS;
#endif
}


/*****************************************************************************/
// Description :    Get serial port's mode.
// Global resource dependence :
// Author :         Jim.zhang
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_GetMode()
{
    uint32 sio_mode = 0xFF;

    sio_mode = REFPARAM_GetUartMode();

    return sio_mode;
}

/* end CR:MS10775 */


//dummy function
/**---------------------------------------------------------------------------*
 ** Function     : void SIO_SetMode(uint32 mode)                              *
 ** Description  : Set Serial Port's Mode                                     *
 ** Parameters   : dummy                                                      *
 ** Return Value :                                                            *
 ** Dependencies : None                                                       *
 ** Author       : Eric.zhou                                                  *
 ** version:                                                                  *
 ** 05/29/2003     Eric.zhou          Creat                                   *
 **---------------------------------------------------------------------------*/
LOCAL uint32 SIO_SetMode (uint32 mode)
{
    return 0;
}

/**---------------------------------------------------------------------------*
 ** Function     : uint32 SIO_SetBaudRate(uint32 port,uint32  baud_rate)      *
 ** Description  : Set Serial Port's Baud Rate.                               *
 ** Parameters   : uint32 port       : COM_DATA/COM_DEBUG                     *
 **                uint32 baud_rate  : baud rate                              *
 ** Return Value : SCI_SUCCESS/SCI_ERROR                                      *
 ** Dependencies : None                                                       *
 ** Author       : Eric.zhou                                                  *
 ** version:                                                                  *
 ** 05/20/2003     Eric.zhou          Creat                                   *
 ** 03/09/2006     weihua.wang        modify for usb virtual com              *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 SIO_SetBaudRate (uint32 port, uint32 baud_rate)
{
    uint32 phy_port = sio_port[port].phy_port;
    uint32 ret;
    SIO_CONTROL_S   m_dcb;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (phy_port > COM3)
    {
        /*when phy port is usb, card or oxff, setting is unuseful*/
        return SCI_SUCCESS;
    }

    if ( (baud_rate > BAUD_1200) || (baud_rate < BAUD_921600))
    {
        return SCI_ERROR;
    }

    // Set baud rate
    m_dcb.baud_rate     = baud_rate;
    m_dcb.flow_control  = 0;

    ret = SCI_SUCCESS;

    switch (phy_port)
    {
        case COM0:
        case COM1:
        case COM2:
        case COM3:
            {
                SIO_Close (port);

                IsChangeBaudrate    = 1;
                SIO_Create (port, phy_port, &m_dcb);
                IsChangeBaudrate    = 0;
            }
            break;

	 case VIR_COM0:
        case VIR_COM1:
            {
                /*
                *though baudrate is useless for usb virtual com
                *update this info in sio struct
                */
                sio_port[port].dcb.baud_rate = baud_rate;

            }
            break;
        default:
            ret = SCI_ERROR;
            break;
    }


    if (COM_DEBUG == port)
        //return_val = EFS_NvitemWrite(NV_SERIAL_0_BAUD_RATE, sizeof(uint32), (uint8 *)&baud_rate, 1);
    {
        REFPARAM_SetDebugPortBaudRate (baud_rate);
    }
    else    //if(COM_DATA == port)
        //return_val = EFS_NvitemWrite(NV_SERIAL_1_BAUD_RATE, sizeof(uint32), (uint8 *)&baud_rate, 1);
    {
        REFPARAM_SetDataPortBaudRate (baud_rate);
    }

    return ret;
}

/**---------------------------------------------------------------------------*
 ** Function     : uint32 SIO_GetBaudRate(uint32 port)                        *
 ** Description  : Get Serial Port's Baud Rate.                               *
 ** Parameters   : uint32 port       : COM_DATA/COM_DEBUG                     *
 ** Return Value : Baud rate                                                  *
 ** Dependencies : None                                                       *
 ** Author       : Eric.zhou                                                  *
 ** version:                                                                  *
 ** 05/20/2003     Eric.zhou          Creat                                   *
 ** 03/09/2006     weihua.wang        modify for usb virtual com              *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 SIO_GetBaudRate (uint32 port)
{
    uint32         baud_rate;
    //NVITEM_ERROR_E return_val;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (COM_DEBUG == port)
        //return_val= EFS_NvitemRead(NV_SERIAL_0_BAUD_RATE, sizeof(uint32),(uint8 *)&baud_rate);
    {
        baud_rate = REFPARAM_GetDebugPortBaudRate();
    }
    else if (COM_DATA == port)
        //return_val= EFS_NvitemRead(NV_SERIAL_1_BAUD_RATE, sizeof(uint32),(uint8 *)&baud_rate);
    {
        baud_rate = REFPARAM_GetDataPortBaudRate();
    }
    else
    {
        return 0;
    }

    //if ((return_val != NVERR_NONE) || (baud_rate > BAUD_1200)||(baud_rate < BAUD_921600) )
    if ( (baud_rate > BAUD_1200) && (baud_rate < BAUD_921600))
    {
        //restore orignal baud_rate
        if (port == COM_DEBUG)
        {
            baud_rate = BAUD_115200;
            //EFS_NvitemWrite(NV_SERIAL_0_BAUD_RATE,sizeof(uint32),(uint8 *)&baud_rate,1);
            REFPARAM_SetDebugPortBaudRate (baud_rate);
        }
        else
        {
            baud_rate = BAUD_115200;
            //EFS_NvitemWrite(NV_SERIAL_1_BAUD_RATE,sizeof(uint32),(uint8 *)&baud_rate,1);
            REFPARAM_SetDataPortBaudRate (baud_rate);
        }
    }

    return baud_rate;
}

//maybe not used now
PUBLIC void SIO_BuildDCB (uint32 port, SIO_CONTROL_S *dcb)
{
}

PUBLIC  SIO_INFO_S *SIO_GetPPPPort()
{
    return &sio_port[COM_DATA];
}



LOCAL uint32 SIO_GetUartBaseAddr (uint8 com_id)
{
    uint32 addr = 0xFFFFFFFF;

    switch (com_id)
    {
        case UART_COM0:
            {
                addr = ARM_UART0_BASE  ;
                break;
            }
        case UART_COM1:
            {
                addr = ARM_UART1_BASE ;
                break;
            }
#if !(defined PRODUCT_DM)
    #if !defined(PLATFORM_SC6530) && !defined (PLATFORM_SC6531EFM)
        case UART_COM2:
            {
                addr = ARM_UART2_BASE ;
                break;
            }
    #endif
#else
        case UART_COM2:
            {
                addr = ARM_UART2_BASE ;
                break;
            }
#endif
        case UART_COM3:
            {
                addr = ZSP_UART_BASE ;
                break;
            }
        default:
            {
                SCI_ASSERT (0);/*assert to do*/
                break;
            }
    }

    return addr;
}

#if defined(PLATFORM_UWS6121E)
LOCAL uint32 sio_uart_dma_chn[UART_MAX] = {0xFF, 0xFF, 0xFF, 0xFF};
#else
LOCAL uint32 sio_uart_dma_chn[UART_MAX] = {0};
#endif
LOCAL uint32 SIO_GetUartDmaChannel (uint8 com_id)
{
    uint32 ret = 0xFFFFFFFF;

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM)
    if(sio_uart_dma_chn[com_id])
	{
	    return sio_uart_dma_chn[com_id];
    }
	else
	{
	    ret = DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_STANDARD);
	    UART_DMA_CONNECT(com_id, UART_DMA_TX_MODE, ret);
		sio_uart_dma_chn[com_id] = ret;
    }
#elif defined(PLATFORM_UWS6121E)
    if(sio_uart_dma_chn[com_id] != 0xFF)
	{
	    return sio_uart_dma_chn[com_id];
    }
	else
	{
	    ret = DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_STANDARD);
	    //UART_DMA_CONNECT(com_id, UART_DMA_TX_MODE, ret);
		sio_uart_dma_chn[com_id] = ret;
    }
#else
    switch (com_id)
    {
        case UART_COM0:
            {
                ret = DMA_UART0_TX;
                break;
            }
        case UART_COM1:
            {
                ret = DMA_UART1_TX;
                break;
            }
#if !(defined PRODUCT_DM)
        case UART_COM2:
            {
                ret = DMA_UART2_TX;
                break;
            }
#endif
        case UART_COM3:
            {
                ret = DMA_ZSPUART_TX;
                break;
            }
        default:
            {
                SCI_ASSERT (0);/*assert to do*/
                break;
            }
    }
#endif

    return ret;
}

LOCAL void SIO_FreeUartDmaChannel (uint8 com_id)
{
   uint32 channel =0;

#if defined(PLATFORM_SC6530) || defined (PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    if(sio_uart_dma_chn[com_id])
	{
	    channel = sio_uart_dma_chn[com_id];
		sio_uart_dma_chn[com_id] = 0;
    }
	else
	{
        //SCI_TRACE_LOW:"SIO_FreeUartDmaChannel, channel is empty\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_1443_112_2_17_23_5_24_1078,(uint8*)"");
        return;
	}

    //Free dma channel been allocated
    UART_DMA_CONNECT(com_id, UART_DMA_TX_MODE, 0);
    DMA_HAL_FreeChn(channel);
#endif
}

LOCAL void SIO_TxDma_CallBack (uint32 num)
{
    s_debug_sio_dma_tx_call_cnt++;
    //SCI_Uart_Dma_EnableDeepSleep(SIO_DMA_DEEPSLEEP_ENABLE);
    SIO_FreeDmaFrameData (&s_sio_dma_data);
    if ( (COM3 >= sio_port[COM_DEBUG].phy_port))
    {
        uint32 channel = SIO_GetUartDmaChannel (SIO_PhyPortfromIndex (sio_port[COM_DEBUG].phy_port));
        SCI_ASSERT (0xFFFFFFFF != channel);/*assert verified*/
        DMA_HAL_ChnEnable (channel, SCI_FALSE);
        if(!SIO_GetUartRxWakeup(SIO_PhyPortfromIndex (sio_port[COM_DEBUG].phy_port)))
        {
            DPSLP_UART_EnableApbSleep (SIO_PhyPortfromIndex (sio_port[COM_DEBUG].phy_port), ENABLE_APB_SLEEP);
        }
    }
    s_sio_tx_dma_is_start = FALSE;
    SIO_SendData();
}

#include "com_drvapi.h" //tao.shi debug
extern void UART_ChangeCallBack (uint32 port, UartCallback func); //tao.shi debug

LOCAL  void SIO_UartDma_WriteData (uint32 com_id, uint8 *start_addr, uint32 length)
{
    uint32 uart_base_addr = 0;
    uint32 channel = 0;

	//return; //for debug tao.shi

    SCI_ASSERT (start_addr);/*assert verified*/
    SCI_ASSERT (0 != length);/*assert verified*/

    //channel = SIO_GetUartDmaChannel (com_id);
    //SCI_ASSERT (0xFFFFFFFF != channel);/*assert verified*/
    //uart_base_addr = SIO_GetUartBaseAddr (com_id);
    //SCI_ASSERT (0xFFFFFFFF != uart_base_addr);/*assert verified*/

#if 0
    if (!sio_port[com_id].open_flag)
    {
        DMA_HAL_ChnEnable (channel, FALSE);
    }
    else
#endif
    {
        s_sio_debug_dma_channel = channel;
        s_sio_debug_uart_tx_addr = uart_base_addr;
        if(!SIO_GetUartRxWakeup(com_id))
        {
            DPSLP_UART_EnableApbSleep (com_id, ENABLE_APB_SLEEP);
        }
        //DMA_HAL_ChnIntConfig (channel, DMA_CHN_INT_TYPE_TRANSDONE, SIO_TxDma_CallBack);

        UART_ChangeCallBack (com_id, SIO_TxDma_CallBack); //tao.shi
        UART_DmaWriteData (com_id, start_addr, length, NULL); 		//ifc dma
        //UART_DmaWriteData (com_id, start_addr, length, channel); 	//axi dma

        DPSLP_UART_EnableApbSleep (com_id, DISABLE_APB_SLEEP);
        s_debug_sio_dma_tx_start_cnt++;
    }
}

LOCAL void SIO_CardLog_Callback (void)
{
    SCI_DisableIRQ();
    {
        SIO_FreeDmaFrameData (&s_sio_dma_data);
        s_sio_tx_dma_is_start = FALSE;
    }
    SCI_RestoreIRQ();

    SIO_SendData();
}

LOCAL void SIO_Usb_TxDma_CallBack (void)
{
    TM_SendTestPointRequest (0x66, SCI_GetTickCount());

    if (s_sio_irqen_timer)
    {
        SCI_DeactiveTimer (s_sio_irqen_timer);
    }

    s_debug_sio_usb_dam_call_cnt++;

    SIO_FreeDmaFrameData (&s_sio_dma_data);


    s_sio_tx_dma_is_start = FALSE;


    if (COM3 < sio_port[COM_DEBUG].phy_port)
    {
#if !defined(PLATFORM_UWS6121E)
        DMA_HAL_PollingTransDone (DMA_USB_EP3);
#endif
    }

    SIO_SendData();
}

LOCAL void SIO_FreeDmaFrameData (SIO_DMA_DATA_T *data_frame_ptr)
{
    SCI_ASSERT (data_frame_ptr);/*assert verified*/

    SCI_DisableIRQ();

    //SCI_ASSERT(INVAIL_DATA_TYPE != data_frame_ptr->type);
    if (NORMAL_DATA_TYPE == data_frame_ptr->type)
    {
        data_frame_ptr->type = INVAIL_DATA_TYPE;

        if (s_sio_frame_ctrl.count > 0)
        {
            s_sio_frame_ctrl.count--;
        }

        s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].pos = 0;
        s_sio_frame_ctrl.tail++;

        if (s_sio_frame_ctrl.tail >= s_sio_frame_ctrl.list_size)
        {
            s_sio_frame_ctrl.tail = 0;
        }

        s_free_frame_cnt++;
        //  TM_SendTestPointRequest(0xDD, s_sio_frame_ctrl.count);

    }

#ifdef DSP_USB_LOG
    else if (DSP_DATA_TYPE == data_frame_ptr->type)
    {
        data_frame_ptr->type = INVAIL_DATA_TYPE;

        if (s_sio_dsp_frame_ctrl.count > 0)
        {
            s_sio_dsp_frame_ctrl.count--;
        }

        s_sio_dsp_frame_ctrl.frame_list[s_sio_dsp_frame_ctrl.tail].pos = 0;
        s_sio_dsp_frame_ctrl.tail++;

        if (s_sio_dsp_frame_ctrl.tail >= s_sio_dsp_frame_ctrl.list_size)
        {
            s_sio_dsp_frame_ctrl.tail = 0;
        }

        SCI_SIO_TRACE ("!!!DSP FreeDmaBuffer. head = 0x%x, tail = 0x%x, count = 0x%x", s_sio_dsp_frame_ctrl.head, s_sio_dsp_frame_ctrl.tail, s_sio_dsp_frame_ctrl.count);

    }
#elif defined(BT_USB_LOG)
    else if (BT_DATA_TYPE == data_frame_ptr->type)
    {
        data_frame_ptr->type = INVAIL_DATA_TYPE;

        if (s_sio_bt_frame_ctrl.count > 0)
        {
            s_sio_bt_frame_ctrl.count--;
        }

        s_sio_bt_frame_ctrl.frame_list[s_sio_bt_frame_ctrl.tail].pos = 0;
        s_sio_bt_frame_ctrl.tail++;

        if (s_sio_bt_frame_ctrl.tail >= s_sio_bt_frame_ctrl.list_size)
        {
            s_sio_bt_frame_ctrl.tail = 0;
        }

        //SCI_SIO_TRACE ("!!!BT FreeDmaBuffer. head = 0x%x, tail = 0x%x, count = 0x%x", s_sio_bt_frame_ctrl.head, s_sio_bt_frame_ctrl.tail, s_sio_bt_frame_ctrl.count);

    }
#endif

    else if (CMD_DATA_TYPE == data_frame_ptr->type)
    {
        data_frame_ptr->type = INVAIL_DATA_TYPE;

        if (s_sio_ppp_ctrl.count > 0)
        {
            s_sio_ppp_ctrl.count--;
        }

        if (0 == s_sio_ppp_ctrl.count)
        {
            if(s_sio_packet_is_sending)
            {
              /*important!!!!*/
              //when ppp packet in on progress, we should exit until ppp packet is complete
            }
            else
            {
                s_sio_has_cmd_req = FALSE;
            }
        }

        s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].pos = 0;
        s_sio_ppp_ctrl.tail++;

        if (s_sio_ppp_ctrl.tail >= MAX_SIO_PPP_FRAME_SIZE)
        {
            s_sio_ppp_ctrl.tail = 0;
        }

        //  TM_SendTestPointRequest(0xCC, s_sio_ppp_ctrl.count);
    }
    else
    {
        //SCI_ASSERT(0);
    }

    SCI_RestoreIRQ();

}


#ifdef DSP_USB_LOG
LOCAL BOOLEAN SIO_NewDmaFrameData (SIO_DMA_DATA_T *data_frame_ptr)
{
    const uint32  maxContinueCnt = 5;
    uint32  armLogFillRate = 0;
    uint32  dspLogFillRate = 0;
    uint32  dspLogSendContinueCnt_tmp = 0;
    uint32  armLogSendContinueCnt_tmp = 0;

    SCI_ASSERT (data_frame_ptr);/*assert verified*/

    if (INVAIL_DATA_TYPE != data_frame_ptr->type)
    {
        return FALSE;
    }

    dspLogSendContinueCnt_tmp = dspLogSendContinueCnt;
    armLogSendContinueCnt_tmp = armLogSendContinueCnt;

    /******************************************************************
    ***********      Work Flow    ***************
    **    step1. Packet Frame(used for Diag,ex phone test, audio test...) has has the highest priority
    **    step2. calculate the ARM frame & DSP frame fill rate, higer fill rate might have higher priority
    **    step3. check if coutinue send count < max coutinue count or the lower priority has no data,
    **              send the data out, otherwise, the lower priority will be sent out first
    **
    ***********************************************************************/

    if (s_sio_has_cmd_req)
    {
        if (s_sio_ppp_ctrl.count > 0)
        {
            data_frame_ptr->base_addr = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].addr;

            /*input the actual buffer size to low level
              1. pos is zeor: the current frame is full
              2. pos is < buf size, current frame is pos
            */
            data_frame_ptr->data_len  = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].pos;

            if (s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].pos == 0)
            {
                data_frame_ptr->data_len  = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].length;
            }

            data_frame_ptr->type = CMD_DATA_TYPE;
            armLogSendContinueCnt_tmp = 0;
            dspLogSendContinueCnt_tmp = 0;
            //    TM_SendTestPointRequest(0xAA, SCI_GetTickCount());
            dspLogSendContinueCnt = dspLogSendContinueCnt_tmp>maxContinueCnt ? maxContinueCnt : dspLogSendContinueCnt_tmp;
            armLogSendContinueCnt = armLogSendContinueCnt_tmp>maxContinueCnt ? maxContinueCnt : armLogSendContinueCnt_tmp;

        }
        else
        {
            if(s_sio_packet_is_sending)
            {
              /*important!!!!*/
              //when ppp packet in on progress, we should exit until ppp packet is complete
            }
            else
            {
                s_sio_has_cmd_req = FALSE;
            }

            return FALSE;
        }
    }
    else
    {
        if ( (s_sio_frame_ctrl.count > 0) || (s_sio_dsp_frame_ctrl.count > 0))
        {
            //calculate the fill rate(%),higher fill rate would have higher priority
            armLogFillRate = s_sio_frame_ctrl.count * 100 /MAX_SIO_PPP_FRAME_SIZE;
            dspLogFillRate = s_sio_dsp_frame_ctrl.count * 100 /MAX_SIO_DSP_PPP_FRAME_SIZE;

            if (armLogFillRate > dspLogFillRate) //compare fill rate first
            {
                if ( (armLogSendContinueCnt_tmp < maxContinueCnt) || (s_sio_dsp_frame_ctrl.count == 0)) //check contunue count
                {
                    data_frame_ptr->base_addr = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].addr;
                    data_frame_ptr->data_len  = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].length;
                    data_frame_ptr->type = NORMAL_DATA_TYPE;
                    s_new_frame_cnt++;
                    armLogSendContinueCnt_tmp++;
                    dspLogSendContinueCnt_tmp = 0; //clear to 0
                    // TM_SendTestPointRequest(0xBB, SCI_GetTickCount());
                }
                else
                {
                    data_frame_ptr->base_addr = s_sio_dsp_frame_ctrl.frame_list[s_sio_dsp_frame_ctrl.tail].addr;
                    data_frame_ptr->data_len  = s_sio_dsp_frame_ctrl.frame_list[s_sio_dsp_frame_ctrl.tail].length;
                    data_frame_ptr->type = DSP_DATA_TYPE;
                    dspLogSendContinueCnt_tmp++;
                    armLogSendContinueCnt_tmp = 0;
                }
            }
            else
            {
                if ( (dspLogSendContinueCnt_tmp < maxContinueCnt) || (s_sio_frame_ctrl.count == 0))
                {
                    data_frame_ptr->base_addr = s_sio_dsp_frame_ctrl.frame_list[s_sio_dsp_frame_ctrl.tail].addr;
                    data_frame_ptr->data_len  = s_sio_dsp_frame_ctrl.frame_list[s_sio_dsp_frame_ctrl.tail].length;
                    data_frame_ptr->type = DSP_DATA_TYPE;
                    dspLogSendContinueCnt_tmp++;
                    armLogSendContinueCnt_tmp = 0;
                }
                else
                {
                    data_frame_ptr->base_addr = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].addr;
                    data_frame_ptr->data_len  = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].length;
                    data_frame_ptr->type = NORMAL_DATA_TYPE;
                    s_new_frame_cnt++;
                    armLogSendContinueCnt_tmp++;
                    dspLogSendContinueCnt_tmp = 0; //clear to 0
                    // TM_SendTestPointRequest(0xBB, SCI_GetTickCount());
                }

            }

            dspLogSendContinueCnt = dspLogSendContinueCnt_tmp>maxContinueCnt ? maxContinueCnt : dspLogSendContinueCnt_tmp;
            armLogSendContinueCnt = armLogSendContinueCnt_tmp>maxContinueCnt ? maxContinueCnt : armLogSendContinueCnt_tmp;

        }
        else
        {
            s_sio_tx_dma_is_start = FALSE;
            return FALSE;
        }
    }

    return TRUE;
}
#elif defined(BT_USB_LOG)
LOCAL BOOLEAN SIO_NewDmaFrameData (SIO_DMA_DATA_T *data_frame_ptr)
{
    const uint32  maxContinueCnt = 5;
    uint32  armLogFillRate = 0;
    uint32  btLogFillRate = 0;
    uint32  btLogSendContinueCnt_tmp = 0;
    uint32  armLogSendContinueCnt_tmp = 0;

    SCI_ASSERT (data_frame_ptr);/*assert verified*/

    if (INVAIL_DATA_TYPE != data_frame_ptr->type)
    {
        return FALSE;
    }

    btLogSendContinueCnt_tmp = btLogSendContinueCnt;
    armLogSendContinueCnt_tmp = armLogSendContinueCnt;

    /******************************************************************
    ***********      Work Flow    ***************
    **    step1. Packet Frame(used for Diag,ex phone test, audio test...) has has the highest priority
    **    step2. calculate the ARM frame & DSP frame fill rate, higer fill rate might have higher priority
    **    step3. check if coutinue send count < max coutinue count or the lower priority has no data,
    **              send the data out, otherwise, the lower priority will be sent out first
    **
    ***********************************************************************/

    if (s_sio_has_cmd_req)
    {
        if (s_sio_ppp_ctrl.count > 0)
        {
            data_frame_ptr->base_addr = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].addr;

            /*input the actual buffer size to low level
              1. pos is zeor: the current frame is full
              2. pos is < buf size, current frame is pos
            */
            data_frame_ptr->data_len  = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].pos;

            if (s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].pos == 0)
            {
                data_frame_ptr->data_len  = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].length;
            }

            data_frame_ptr->type = CMD_DATA_TYPE;
            armLogSendContinueCnt_tmp = 0;
            btLogSendContinueCnt_tmp = 0;
            //    TM_SendTestPointRequest(0xAA, SCI_GetTickCount());
            //coverity 15990.fix the dead code
            btLogSendContinueCnt = btLogSendContinueCnt_tmp; //btLogSendContinueCnt_tmp>maxContinueCnt ? maxContinueCnt : btLogSendContinueCnt_tmp;
            armLogSendContinueCnt = armLogSendContinueCnt_tmp; //armLogSendContinueCnt_tmp>maxContinueCnt ? maxContinueCnt : armLogSendContinueCnt_tmp;

        }
        else
        {
            if(s_sio_packet_is_sending)
            {
              /*important!!!!*/
              //when ppp packet in on progress, we should exit until ppp packet is complete
            }
            else
            {
                s_sio_has_cmd_req = FALSE;
            }

            return FALSE;
        }
    }
    else
    {
        if ( (s_sio_frame_ctrl.count > 0) || (s_sio_bt_frame_ctrl.count > 0))
        {
            //calculate the fill rate(%),higher fill rate would have higher priority
            armLogFillRate = s_sio_frame_ctrl.count * 100 /MAX_SIO_PPP_FRAME_SIZE;
            btLogFillRate = s_sio_bt_frame_ctrl.count * 100 /MAX_SIO_BT_PPP_FRAME_SIZE;

            if (armLogFillRate > btLogFillRate) //compare fill rate first
            {
                if ( (armLogSendContinueCnt_tmp < maxContinueCnt) || (s_sio_bt_frame_ctrl.count == 0)) //check contunue count
                {
                    data_frame_ptr->base_addr = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].addr;
                    data_frame_ptr->data_len  = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].length;
                    data_frame_ptr->type = NORMAL_DATA_TYPE;
                    s_new_frame_cnt++;
                    armLogSendContinueCnt_tmp++;
                    btLogSendContinueCnt_tmp = 0; //clear to 0
                    // TM_SendTestPointRequest(0xBB, SCI_GetTickCount());
                }
                else
                {
                    data_frame_ptr->base_addr = s_sio_bt_frame_ctrl.frame_list[s_sio_bt_frame_ctrl.tail].addr;
                    data_frame_ptr->data_len  = s_sio_bt_frame_ctrl.frame_list[s_sio_bt_frame_ctrl.tail].length;
                    data_frame_ptr->type = BT_DATA_TYPE;
                    btLogSendContinueCnt_tmp++;
                    armLogSendContinueCnt_tmp = 0;
                }
            }
            else
            {
                if ( (btLogSendContinueCnt_tmp < maxContinueCnt) || (s_sio_frame_ctrl.count == 0))
                {
                    data_frame_ptr->base_addr = s_sio_bt_frame_ctrl.frame_list[s_sio_bt_frame_ctrl.tail].addr;
                    data_frame_ptr->data_len  = s_sio_bt_frame_ctrl.frame_list[s_sio_bt_frame_ctrl.tail].length;
                    data_frame_ptr->type = BT_DATA_TYPE;
                    btLogSendContinueCnt_tmp++;
                    armLogSendContinueCnt_tmp = 0;
                }
                else
                {
                    data_frame_ptr->base_addr = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].addr;
                    data_frame_ptr->data_len  = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].length;
                    data_frame_ptr->type = NORMAL_DATA_TYPE;
                    s_new_frame_cnt++;
                    armLogSendContinueCnt_tmp++;
                    btLogSendContinueCnt_tmp = 0; //clear to 0
                    // TM_SendTestPointRequest(0xBB, SCI_GetTickCount());
                }

            }

            btLogSendContinueCnt = btLogSendContinueCnt_tmp>maxContinueCnt ? maxContinueCnt : btLogSendContinueCnt_tmp;
            armLogSendContinueCnt = armLogSendContinueCnt_tmp>maxContinueCnt ? maxContinueCnt : armLogSendContinueCnt_tmp;

        }
        else
        {
            s_sio_tx_dma_is_start = FALSE;
            return FALSE;
        }
    }

    return TRUE;
}
#else
LOCAL BOOLEAN SIO_NewDmaFrameData (SIO_DMA_DATA_T *data_frame_ptr)
{
    SCI_ASSERT (data_frame_ptr);/*assert verified*/

    if (INVAIL_DATA_TYPE != data_frame_ptr->type)
    {
        return FALSE;
    }

    if (s_sio_has_cmd_req)
    {
        if (s_sio_ppp_ctrl.count > 0)
        {
            data_frame_ptr->base_addr = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].addr;

            /*input the actual buffer size to low level
              1. pos is zeor: the current frame is full
              2. pos is < buf size, current frame is pos
            */
            data_frame_ptr->data_len  = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].pos;

            if (s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].pos == 0)
            {
                data_frame_ptr->data_len  = s_sio_ppp_ctrl.frame_list[s_sio_ppp_ctrl.tail].length;
            }

            data_frame_ptr->type = CMD_DATA_TYPE;

            //    TM_SendTestPointRequest(0xAA, SCI_GetTickCount());
        }
        else
        {
            if(s_sio_packet_is_sending)
            {
              /*important!!!!*/
              //when ppp packet in on progress, we should exit until ppp packet is complete
            }
            else
            {
                s_sio_has_cmd_req = FALSE;
            }

            return FALSE;
        }
    }
    else
    {
        if (s_sio_frame_ctrl.count > 0)
        {
            data_frame_ptr->base_addr = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].addr;
            data_frame_ptr->data_len  = s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.tail].length;
            data_frame_ptr->type = NORMAL_DATA_TYPE;
            s_new_frame_cnt++;
            // TM_SendTestPointRequest(0xBB, SCI_GetTickCount());
        }
        else
        {
            s_sio_tx_dma_is_start = FALSE;
            return FALSE;
        }
    }

    return TRUE;
}

#endif


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_Create()                                                     *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Create a device control block.                                        *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     The handle of SIO device.                                             *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC HSIO SIO_Create (uint32        port,
                        uint32        phy_port,
                        SIO_CONTROL_S *dcb
                       )
{
    UART_INIT_PARA_T uart_st;
    uint32 result;
    uint16 t = 0;


    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/
    SCI_ASSERT ((phy_port == 0xff) || (phy_port < MAX_SIO_PORT_NUM));/*assert verified*/

    if (((phy_port == DSK_COM0) || (phy_port == DSK_COM1)))
    {
        SCI_ASSERT (0);/*assert verified*/
    }

    if (PNULL == s_sio_uart_sleep_timer)
    {
    	s_sio_uart_sleep_timer = SCI_CreateTimer("SIO UART SLEEP Timer",
                                     SIO_UartSleepTimerExpired, 0, SIO_UART_SLEEP_TIMER_VALUE, SCI_NO_ACTIVATE);
    }

    if (0xff == phy_port)
    {
        //indicated current port can't be used,return directly
        sio_port[port].open_flag = 0;
        sio_port[port].phy_port  = phy_port;

        switch (port)
        {
            case COM_DEBUG:

                if (NULL == comm_event)
                {
                    comm_event = SCI_CreateEvent ("NULL");    // Create a comm event group
                }

                break;

            case COM_DEBUG2:
                break;

            case COM_DATA:
                sio_port[port].phy_port  = phy_port;
#ifdef PPP_USB
                user_port_mode_2      = ATC_MODE;
                atc_end_ch1[port]         = END_CHAR;
                atc_end_ch2[port]        = '\0';
                atc_bs_ch[port]           = BACKSPACE_CHAR;
                atc_is_echo[port]         = FALSE;

#else
                atc_end_ch1         = END_CHAR;
                atc_end_ch2         = '\0';
                atc_bs_ch           = BACKSPACE_CHAR;
                atc_is_echo         = FALSE;
#endif
		  user_port_mode      = ATC_MODE;

                /* Set receive and transmit buffer. */
                sio_port[port].tx_buf.sio_buf_ptr    = sio_data_send_buf;
                sio_port[port].tx_buf.size           = DATA_PORT_SEND_BUF_SIZE;
                sio_port[port].tx_buf.start_point    = 0;
                sio_port[port].tx_buf.end_point      = 0;
                sio_port[port].tx_buf.status         = 0;
                sio_port[port].tx_buf.lost_num       = 0;

                sio_port[port].rx_buf.sio_buf_ptr    = sio_data_rec_buf;
                sio_port[port].rx_buf.size           = DATA_PORT_REC_BUF_SIZE;
                sio_port[port].rx_buf.start_point    = 0;
                sio_port[port].rx_buf.end_point      = 0;
                sio_port[port].rx_buf.status         = 0;
                sio_port[port].rx_buf.lost_num       = 0;

                sio_port[port].atc_buf.sio_buf_ptr    = sio_atc_rec_buf;
                sio_port[port].atc_buf.size           = ATC_REC_BUF_SIZE;
                sio_port[port].atc_buf.start_point    = 0;
                sio_port[port].atc_buf.end_point      = 0;
                sio_port[port].atc_buf.status         = 0;
                sio_port[port].atc_buf.lost_num       = 0;
                sio_port[port].open_flag = SIO_ALREADY_OPEN;
                break;

	#ifdef PPP_USB
		case COM_DATA2:
	                sio_port[port].phy_port  = phy_port;
	                user_port_mode_2      = ATC_MODE;
		         atc_end_ch1[port]         = END_CHAR;
	                atc_end_ch2[port]         = '\0';
	                atc_bs_ch[port]           = BACKSPACE_CHAR;
                	  atc_is_echo[port]         = FALSE;
	                /* Set receive and transmit buffer. */
	                sio_port[port].tx_buf.sio_buf_ptr    = sio_data2_send_buf;
	                sio_port[port].tx_buf.size           = DATA_PORT_SEND_BUF_SIZE;
	                sio_port[port].tx_buf.start_point    = 0;
	                sio_port[port].tx_buf.end_point      = 0;
	                sio_port[port].tx_buf.status         = 0;
	                sio_port[port].tx_buf.lost_num       = 0;

	                sio_port[port].rx_buf.sio_buf_ptr    = sio_data2_rec_buf;
	                sio_port[port].rx_buf.size           = DATA_PORT_REC_BUF_SIZE;
	                sio_port[port].rx_buf.start_point    = 0;
	                sio_port[port].rx_buf.end_point      = 0;
	                sio_port[port].rx_buf.status         = 0;
	                sio_port[port].rx_buf.lost_num       = 0;

	                sio_port[port].atc_buf.sio_buf_ptr    = sio_atc2_rec_buf;
	                sio_port[port].atc_buf.size           = ATC_REC_BUF_SIZE;
	                sio_port[port].atc_buf.start_point    = 0;
	                sio_port[port].atc_buf.end_point      = 0;
	                sio_port[port].atc_buf.status         = 0;
	                sio_port[port].atc_buf.lost_num       = 0;
	                sio_port[port].open_flag = SIO_ALREADY_OPEN;
		                break;
	#endif

            default:
                break;
        }

        return NULL;
    }

    /* Check if the port is opened before. */
    if (SIO_ALREADY_OPEN == sio_port[port].open_flag)
    {
        //   do nothing , modify for assert debug of uart
        //  return NULL;
    }

    sio_port[port].phy_port  = phy_port;

    sio_port[port].dcb.baud_rate    = dcb->baud_rate;
    sio_port[port].dcb.parity       = 0;//ULCRNoParity;
    sio_port[port].dcb.stop_bits    = 0;//ULCRS1StopBit;
    sio_port[port].dcb.byte_size    = 0;//ULCR8bits;
    sio_port[port].dcb.flow_control = dcb->flow_control;

    ppp_buf.head.seq_num = 0;
    ppp_buf.head.type    = PPP_PACKET_A;
    ppp_buf.head.subtype = 0;
    ppp_buf.cur_offset   = 0;

    uart_st.tx_watermark  = TX_WATER_MARK; // 0~127B
    uart_st.rx_watermark  = UART_RxWaterMark(); // 0~127B
    uart_st.baud_rate     = dcb->baud_rate;
    uart_st.parity        = FALSE;
    uart_st.parity_enable = PARITY_DISABLE;
    uart_st.byte_size     = EIGHT_BITS;
    uart_st.stop_bits     = ONE_STOP_BIT;
    uart_st.flow_control  = dcb->flow_control;
    uart_st.ds_wakeup_type= DS_WAKEUP_BY_RXD;
    // ONLY enable in Following mode, for writing IMEI by UART,USB
    //coverity 35981. del the redundancy code
    //if(REFPARAM_GetUsbAutoAssertRelModeFlag())
    //{
        uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;//DS_WAKEUP_ENABLE
    //}
    //else
    //{
    //    uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
   // }
	if( CharDev_Install_Operation(&(sio_port[port].sio_op),phy_port) != TRUE)
	{
	    return NULL;
	}

    //register call back func
    switch (phy_port)
    {
        case COM0:
        case COM1:
        case COM2:
        case COM3:
            {

                sio_port[port].sio_op.flush          = SIO_DefaultFlush;

#ifdef TRACE_INFO_SUPPORT
                //Initalize USB Log&Trace Frame Control
                s_sio_frame_ctrl.count = 0;
                s_sio_frame_ctrl.head = 0;
                s_sio_frame_ctrl.tail = 0;
                s_sio_frame_ctrl.list_size = MAX_SIO_UART_FRAME_NUM;

                for (t = 0; t < s_sio_frame_ctrl.list_size; t++)
                {
                    s_sio_frame_ctrl.frame_list[t].addr = (uint8 *) &log_buf[t*MAX_SIO_UART_FRAME_BUFF_LEN];
                    s_sio_frame_ctrl.frame_list[t].length = MAX_SIO_UART_FRAME_BUFF_LEN;
                    s_sio_frame_ctrl.frame_list[t].pos = 0;
                }
#endif
            }
            break;
        case VIR_COM0:
	 case VIR_COM1:
	 case VIR_COM3:
	 case VIR_COM4:
             {
#ifdef TRACE_INFO_SUPPORT
                //Initalize USB Log&Trace Frame Control
                s_sio_frame_ctrl.count = 0;
                s_sio_frame_ctrl.head = 0;
                s_sio_frame_ctrl.tail = 0;
                s_sio_frame_ctrl.list_size = MAX_SIO_USB_FRAME_NUM;

                for (t = 0; t < s_sio_frame_ctrl.list_size; t++)
                {
                    s_sio_frame_ctrl.frame_list[t].addr = (uint8 *) &log_buf[t*MAX_SIO_USB_FRAME_BUFF_LEN];
                    s_sio_frame_ctrl.frame_list[t].length = MAX_SIO_USB_FRAME_BUFF_LEN;
                    s_sio_frame_ctrl.frame_list[t].pos = 0;
                }
#endif
            }

	      break;

        case DSK_COM0:
        case DSK_COM1:
            {
#ifdef TRACE_INFO_SUPPORT
                //Initalize USB Log&Trace Frame Control
                s_sio_frame_ctrl.count = 0;
                s_sio_frame_ctrl.head = 0;
                s_sio_frame_ctrl.tail = 0;
                s_sio_frame_ctrl.list_size = MAX_SIO_UART_FRAME_NUM;

                for (t = 0; t < s_sio_frame_ctrl.list_size; t++)
                {
                    s_sio_frame_ctrl.frame_list[t].addr = (uint8 *) &log_buf[t*MAX_SIO_UART_FRAME_BUFF_LEN];
                    s_sio_frame_ctrl.frame_list[t].length = MAX_SIO_UART_FRAME_BUFF_LEN;
                    s_sio_frame_ctrl.frame_list[t].pos = 0;
                }
#endif
            }
            break;
        default:
            break;
    }

    SCI_ASSERT (sio_port[port].sio_op.init != NULL);/*assert verified*/

    /* Get base register of uart. */
    switch (port)
    {
        case COM_DEBUG:

            if (NULL == comm_event)
            {
                comm_event = SCI_CreateEvent ("NULL");    // Create a comm event group
            }

            if (NULL == comm_mutex)
            {
                comm_mutex =  SCI_CreateMutex ("COMDEDUG_MUTEX", SCI_INHERIT); // Create a comm event group
#ifdef _SIO_DEBUG_
                sio_test_init();
#endif
            }

            /* Set receive and transmit buffer. */
            sio_port[port].tx_buf.sio_buf_ptr    = sio_debug_send_buf;
            sio_port[port].tx_buf.size           = DEBUG_PORT_SEND_BUF_SIZE;
            sio_port[port].tx_buf.start_point    = 0;
            sio_port[port].tx_buf.end_point      = 0;
            sio_port[port].tx_buf.status         = 0;
            sio_port[port].tx_buf.lost_num       = 0;

            sio_port[port].rx_buf.sio_buf_ptr    = sio_debug_rec_buf;
            sio_port[port].rx_buf.size           = DEBUG_PORT_REC_BUF_SIZE;
            sio_port[port].rx_buf.start_point    = 0;
            sio_port[port].rx_buf.end_point      = 0;
            sio_port[port].rx_buf.status         = 0;
            sio_port[port].rx_buf.lost_num       = 0;

            //Initalize PPP Frame Control
            s_sio_ppp_ctrl.count = 0;
            s_sio_ppp_ctrl.head = 0;
            s_sio_ppp_ctrl.tail = 0;
            s_sio_ppp_ctrl.list_size = MAX_SIO_PPP_FRAME_SIZE;

            for (t = 0; t < s_sio_ppp_ctrl.list_size; t++)
            {
                s_sio_ppp_ctrl.frame_list[t].addr = (uint8 *) &sio_debug_send_buf[t*MAX_SIO_PPP_FRAME_BUFF_SIZE];
                s_sio_ppp_ctrl.frame_list[t].length = MAX_SIO_PPP_FRAME_BUFF_SIZE;
                s_sio_ppp_ctrl.frame_list[t].pos = 0;
            }

            //Initalize s_sio_dma_data
            SCI_MEMSET ( ( (uint8 *) &s_sio_dma_data), 0, sizeof (SIO_DMA_DATA_T));

            s_sio_tx_dma_is_start = FALSE;  //DMA Is not busy
            s_sio_is_need_pad_flag = FALSE;
            s_sio_packet_is_sending = FALSE;
            s_sio_has_cmd_req = FALSE;
            s_sio_debug_com = phy_port;

            result = (sio_port[port].sio_op.init) (SIO_PhyPortfromIndex (phy_port),&uart_st,SIO_DebugPortHdl);

            if (
                ( (DSK_COM0 == phy_port) || (DSK_COM1 == phy_port))
                && (-1 == (int32) result)
            )
            {
                sio_port[port].open_flag = 0;
                sio_port[port].phy_port  = 0xFF;
                (sio_port[port].sio_op.close) (0);
                return 0;
            }

            if (phy_port <= (uint32) COM3)
            {
                //Close Uart Tx Fifo Interrupt
                uint32 uart_base_addr = SIO_GetUartBaseAddr (SIO_PhyPortfromIndex (phy_port));
                UART_PHY_TxInEnable (phy_port, 0);
                REG32 (uart_base_addr + 0xC) &= ~ (uint32) (1<<2);
#if !defined(PLATFORM_UWS6121E)
                REG32 (uart_base_addr + 0xC) &= ~ (uint32) ARM_UART_TX_IE_TX_IE_ENABLE;

                //Enable Uart Dma
                REG32 (uart_base_addr + ARM_UART_CTL1)  |= (uint32) BIT_15;
#endif
            }

            break;

        case COM_DEBUG2:
            result = (sio_port[port].sio_op.init) (SIO_PhyPortfromIndex (phy_port),&uart_st,SIO_Debug2_PortHdl);

            if (
                ( (DSK_COM0 == phy_port) || (DSK_COM1 == phy_port))
                && (-1 == (int32) result)
            )
            {
                sio_port[port].open_flag = 0;
                sio_port[port].phy_port  = 0xFF;
                (sio_port[port].sio_op.close) (0);
                return 0;
            }

	     if ( (VIR_COM0  == phy_port) || (VIR_COM1 == phy_port) ||(VIR_COM3 == phy_port) ||(VIR_COM4 == phy_port))
	     {
               SIO_OpenUsbComNotifyModem();
	     }

            break;

        case COM_DATA:
            {
                if(ATC_timer == NULL)
                {
                    ATC_timer = SCI_CreateTimer ("ATC TIMER", (TIMER_FUN) ATC_CheckPPP,
                                             0, MODE_CHANGE_DELAY, SCI_NO_ACTIVATE);
                }

//#ifdef _DATA_FLOW_CONTROL
                if(Space_timer == NULL)
                {
                    Space_timer =  SCI_CreateTimer ("Space TIMER", (TIMER_FUN) Space_Check ,
                                                0, SPACE_CHECK_DELAY, SCI_NO_ACTIVATE);
                }
//#endif

                if (!IsChangeBaudrate)
                {
		#ifdef PPP_USB
           		atc_end_ch1[port]         = END_CHAR;
                    atc_end_ch2[port]         = '\0';
                    atc_bs_ch[port]           = BACKSPACE_CHAR;
                    atc_is_echo[port]         = FALSE;

		#else
                    atc_end_ch1         = END_CHAR;
                    atc_end_ch2         = '\0';
                    atc_bs_ch           = BACKSPACE_CHAR;
                    atc_is_echo         = FALSE;
		#endif
		        user_port_mode      = ATC_MODE;

                }

                cur_atc_frame_size  = 0;
                plus_num            = 0;
                last_rx_time        = 0;

                /* Set receive and transmit buffer. */
                sio_port[port].tx_buf.sio_buf_ptr    = sio_data_send_buf;
                sio_port[port].tx_buf.size           = DATA_PORT_SEND_BUF_SIZE;
                sio_port[port].tx_buf.start_point    = 0;
                sio_port[port].tx_buf.end_point      = 0;
                sio_port[port].tx_buf.status         = 0;
                sio_port[port].tx_buf.lost_num       = 0;

                sio_port[port].rx_buf.sio_buf_ptr    = sio_data_rec_buf;
                sio_port[port].rx_buf.size           = DATA_PORT_REC_BUF_SIZE;
                sio_port[port].rx_buf.start_point    = 0;
                sio_port[port].rx_buf.end_point      = 0;
                sio_port[port].rx_buf.status         = 0;
                sio_port[port].rx_buf.lost_num       = 0;

                sio_port[port].atc_buf.sio_buf_ptr    = sio_atc_rec_buf;
                sio_port[port].atc_buf.size           = ATC_REC_BUF_SIZE;
                sio_port[port].atc_buf.start_point    = 0;
                sio_port[port].atc_buf.end_point      = 0;
                sio_port[port].atc_buf.status         = 0;
                sio_port[port].atc_buf.lost_num       = 0;

                //@Zhemin.Lin, CR:MS00004213, 2003/08/22, begin
                RESET_MIX_MODE_INFO;
                mix_mode_info.data_rx_buf.sio_buf_ptr    = sio_mix_data_rec_buf;
                mix_mode_info.data_rx_buf.size           = DATA_PORT_MIXMODE_BUF_SIZE;
                mix_mode_info.data_rx_buf.start_point    = 0;
                mix_mode_info.data_rx_buf.end_point      = 0;
                mix_mode_info.data_rx_buf.status         = 0;
                mix_mode_info.data_rx_buf.lost_num       = 0;

                //create mutex
                //@Zhemin.Lin, 09/12/2003, CR:MS00004678, must use os_api interface
                if( !mix_mode_info.mutexforwrite )
                {
                    mix_mode_info.mutexforwrite = SCI_CreateMutex ("mutex_sio_write",SCI_INHERIT);
                    SCI_ASSERT (SCI_NULL != mix_mode_info.mutexforwrite);/*assert verified*/
                }
                //@Zhemin.Lin, CR:MS00004213, 2003/08/22, end

#ifdef _U0_FLOW_CONTROL
                uart_st.flow_control  = HW_FLOW_CONTROL;
#endif
                //low init handle
                (sio_port[port].sio_op.init) (SIO_PhyPortfromIndex (phy_port),&uart_st,SIO_DataPortHdl);
            }
            break;

#ifdef PPP_USB

        case COM_DATA2:
            {
                if(ATC_timer_2 == NULL)
                {
                    ATC_timer_2 = SCI_CreateTimer ("ATC2 TIMER", (TIMER_FUN) ATC_CheckPPP_2,
                                             0, MODE_CHANGE_DELAY, SCI_NO_ACTIVATE);
                }

//#ifdef _DATA_FLOW_CONTROL
                if(Space_timer_2 == NULL)
                {
                    Space_timer_2 =  SCI_CreateTimer ("Space2 TIMER", (TIMER_FUN) Space_Check_2 ,
                                                0, SPACE_CHECK_DELAY, SCI_NO_ACTIVATE);
                }
//#endif

                if (!IsChangeBaudrate)
                {
                     user_port_mode_2      = ATC_MODE;
           		atc_end_ch1[port]         = END_CHAR;
                    atc_end_ch2[port]         = '\0';
                    atc_bs_ch[port]           = BACKSPACE_CHAR;
                    atc_is_echo[port]         = FALSE;
                }

                last_rx_time        = 0;

                /* Set receive and transmit buffer. */
                sio_port[port].tx_buf.sio_buf_ptr    = sio_data2_send_buf;
                sio_port[port].tx_buf.size           = DATA_PORT_SEND_BUF_SIZE;
                sio_port[port].tx_buf.start_point    = 0;
                sio_port[port].tx_buf.end_point      = 0;
                sio_port[port].tx_buf.status         = 0;
                sio_port[port].tx_buf.lost_num       = 0;

                sio_port[port].rx_buf.sio_buf_ptr    = sio_data2_rec_buf;
                sio_port[port].rx_buf.size           = DATA_PORT_REC_BUF_SIZE;
                sio_port[port].rx_buf.start_point    = 0;
                sio_port[port].rx_buf.end_point      = 0;
                sio_port[port].rx_buf.status         = 0;
                sio_port[port].rx_buf.lost_num       = 0;

                sio_port[port].atc_buf.sio_buf_ptr    = sio_atc_rec_buf;
                sio_port[port].atc_buf.size           = ATC_REC_BUF_SIZE;
                sio_port[port].atc_buf.start_point    = 0;
                sio_port[port].atc_buf.end_point      = 0;
                sio_port[port].atc_buf.status         = 0;
                sio_port[port].atc_buf.lost_num       = 0;

#ifdef _U0_FLOW_CONTROL
                uart_st.flow_control  = HW_FLOW_CONTROL;
#endif
                //low init handle
                (sio_port[port].sio_op.init) (SIO_PhyPortfromIndex (phy_port),&uart_st,SIO_DataPortHdl_2);
            }
            break;
#endif


        default:
            return NULL;
    }

    sio_port[port].open_flag = SIO_ALREADY_OPEN;
    /* Return handle of the sio contol block. */
    return ( (HSIO) &sio_port[port]);
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_Close(uint32 port);                                          *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     CLose SIO device.                                                     *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     The handle of SIO device.                                             *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC void SIO_Close (uint32 port)
{
    uint32 phy_port = sio_port[port].phy_port;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/
    SCI_DisableIRQ();

    if (SIO_ALREADY_OPEN != sio_port[port].open_flag)
    {
        //return directly
        sio_port[port].open_flag = 0;
        SCI_RestoreIRQ();
        return ;
    }

    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        sio_port[port].open_flag = 0;
        SCI_RestoreIRQ();
        return;
    }

    sio_port[port].open_flag = 0;
    SCI_RestoreIRQ();

    /* Below is Hardware close. */
    switch (port)
    {
        case COM_DEBUG:

            // @Tao.Zhou, limit the frequency of sio irq, 10/18/2006, begin
            if (NULL != s_sio_irqen_timer)
            {
                SCI_DeleteTimer (s_sio_irqen_timer);
                s_sio_irqen_timer = NULL;
            }

            // @Tao.Zhou, limit the frequency of sio irq, 10/18/2006, end
            //@CR98815 jiexia.yu
            if (s_sio_tx_dma_is_start)
            {
                if (phy_port < VIR_COM0)
                {
                    DMA_HAL_ChnEnable (SIO_GetUartDmaChannel (SIO_PhyPortfromIndex (phy_port)),
                                       SCI_FALSE);
                    SIO_FreeUartDmaChannel(SIO_PhyPortfromIndex (phy_port));
                    DPSLP_UART_EnableApbSleep (SIO_PhyPortfromIndex (phy_port), ENABLE_APB_SLEEP);
                    SIO_SetUartRxWakeup(SIO_PhyPortfromIndex (phy_port),SCI_FALSE);

                }

                s_sio_tx_dma_is_start = SCI_FALSE;
            }

            //@CR98815 jiexia.yu
            break;

        case COM_DEBUG2:
            SIO_CloseUsbComNotifyModem();
            break;

        case COM_DATA:

            /* Enable Uart1! */
            if (NULL != ATC_timer)
            {
                SCI_DeleteTimer (ATC_timer);
                ATC_timer = NULL;
            }

            break;

#ifdef PPP_USB
        case COM_DATA2:

            if (NULL != ATC_timer_2)
            {
                SCI_DeleteTimer (ATC_timer_2);
                ATC_timer_2 = NULL;
            }
            sio_ppp_rx_buff_free();
            break;
#endif

        default:
            SCI_ASSERT (0);/*assert verified*/
    }

    /* Below is Hardware close. */
    (sio_port[port].sio_op.close) (SIO_PhyPortfromIndex (phy_port));

}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_ReadPPPFrame()                                               *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Read a PPP frame in SIO buffer.                                       *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     If read a completed PPP frame, returns SIO_SUCCESS, else return       *
 **     SIO_FAIL.                                                             *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
void SIO_SetRecvBufferReadPoint(uint32 port,int32 end)
{
    uint32 free_data_size = 0;
    SCI_DisableIRQ();
    sio_port[port].rx_buf.end_point = end;
    SCI_RestoreIRQ();

    GET_BUF_FREE_SIZE (free_data_size, (&sio_port[COM_DEBUG].rx_buf))
    if (s_sio_rx_int_is_disabled == TRUE) {
        if ((sio_port[port].phy_port < MAX_SIO_PORT_NUM) && (SIO_ALREADY_OPEN == sio_port[port].open_flag))
        {
            SIO_Rx_Int_Enable(port,TRUE);
            s_sio_rx_int_is_disabled = FALSE;
        }
    }
}
PUBLIC  int SIO_ReadPPPFrame (uint32 port, // Port number COM_DATA, COM_DEBUG
                              uint8  *src,            // Data to receive
                              int    size,            // MAX size to receive
                              uint32 *read_len        // Actual size received
                             )
{
    int32    space = 0;
    int32    data_size;
    register int32 end;
    uint8    *buf;
    int32    buf_size;


    uint32 event_mask = 0;

#if defined(PRODUCT_DM)
    if ( (REFPARAM_GetUartRecvMode() == 0) && (SCI_IdentifyThread() != CM))
    {
        SCI_SuspendThread (SCI_IdentifyThread());
    }

#endif


     if (!size||!src||(port >=(MAX_LOGICAL_SIO_PORT_NUM)))
    {



          *read_len = 0;
        return SIO_PPP_FAIL;
    }
     space = 0;

    /* if recv data from sio port, read to src point, else block myself
     * (suspend thread) */

        /* If sio recv data, interrupt service will copy data to buffer and send
         * event to reader. But many times read_pppframe routine cannot read data
         * frequntely, therefore there are not data in the buffer when wait
         * an event coming. It is necessary continue to wait next event coming. */


        /* It is necessary that a full frame be copied to caller, or caller
         * will failed. */
        for (;;)
        {
            GET_BUF_DATA_SIZE (data_size, (&sio_port[port].rx_buf));


            /* wait for read a full frame */
           while (0 == data_size)
            {
                 SIO_WaitEvent (port, &event_mask);
                GET_BUF_DATA_SIZE (data_size, (&sio_port[port].rx_buf));

                }

            /* Make sure that the size is less than the total size of the buffer. */
            /*
                We get these value first in stack, in order to speed up the program.
                In this case, all caculations are used by registers.
            */
            end    = sio_port[port].rx_buf.end_point;
            buf      = sio_port[port].rx_buf.sio_buf_ptr;
            buf_size = (int32) sio_port[port].rx_buf.size;

            /* In each read time, space means read data count */

            /* Read data from buf. */
           while (data_size > 0)
            {
                int   nchar;
                int   v_status;

                nchar  = buf[end++];
                  data_size--;
                // Check if it is a valid char

                    // Get a completed frame
                    if (end >= buf_size)
                    {
                        end = 0;
                    }


                    /* only one entry out */
                  v_status = CheckInput (&nchar);

                  if (SIO_FRAME_OK == v_status)
                    {
                        SIO_SetRecvBufferReadPoint(port,end);
                        *read_len = space;

                    /* only one entry out */
                    return SIO_PPP_SUCCESS;
                }
                else if (SIO_FRAME_NORMAL == v_status)
                {
                    // message body
                    *src++ = (uint8) nchar;
                    space++;
                        size--;
                        if(size == 0)
                {
                    // masked char
                             break;
                        }
                  }

           }/* end while space<data_size */

           if(size == 0)
           {
                 break;
           }
           if(data_size == 0)
           {
                 SIO_SetRecvBufferReadPoint(port,end);
           }
           }
     SIO_SetRecvBufferReadPoint(port,end);

     *read_len = space;
     return SIO_PPP_FAIL;

}

LOCAL void __SendPaddingData (void)
{
    uint32 port = 0;
    port = sio_port[COM_DEBUG].phy_port;

    if ( (port  != VIR_COM0) && (port !=VIR_COM1))
    {
        return;
    }

    if ( (!s_sio_tx_dma_is_start) && s_sio_is_need_pad_flag && (!s_sio_packet_is_sending) && sio_port[COM_DEBUG].sio_op.write)
    {
        s_sio_tx_dma_is_start = TRUE;
        s_sio_is_need_pad_flag = FALSE;
        TM_SendTestPointRequest (0x9999, SCI_GetTickCount());
        (sio_port[COM_DEBUG].sio_op.write) (SIO_PhyPortfromIndex (port), (uint8 *) &s_sio_usb_padding_word,   sizeof (s_sio_usb_padding_word));
        TM_SendTestPointRequest (0xaaaa, SCI_GetTickCount());
    }

}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_WritePPPFrame()                                              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Write a PPP frame in SIO buffer.                                      *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return the size of byte written to the buffer.                        *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
/*
    //@Richard
    Well, we have to try to make sure the max time we will face.
    Because there is much time I have to disable interrupt.
*/

PUBLIC void SIO_SendData (void)
{
    BOOLEAN hasData;
    uint32 port;
    uint8 *addr;
    uint32 length;

    SCI_DisableIRQ();

    if (s_sio_tx_dma_is_start)
    {
        SCI_RestoreIRQ();
        return;
    }

    hasData = SIO_NewDmaFrameData (&s_sio_dma_data);

    if (!hasData)
    {
        // TM_SendTestPointRequest (0x8888, s_sio_is_need_pad_flag);
        __SendPaddingData();
        SCI_RestoreIRQ();
        return;
    }
    else
    {
        port = sio_port[COM_DEBUG].phy_port;
        addr = s_sio_dma_data.base_addr;
        length = s_sio_dma_data.data_len;

        if (port > MAX_SIO_PORT_NUM)
        {
            /*when running here, and usb plug out (153897)
              it will call sio_close first, and reset sio_port to org value,
              sio_port flag may set to 0xff, change assert to exception handle
            */
            SCI_RestoreIRQ();
            return;
        }

        SCI_ASSERT (addr);/*assert verified*/
        SCI_ASSERT (length);/*assert verified*/

        if (port <= COM3) //Use Uart Dma
        {
            s_sio_tx_dma_is_start = TRUE;
            SIO_UartDma_WriteData (SIO_PhyPortfromIndex (port),
                                   (uint8 *) addr,
                                   length);
            SCI_RestoreIRQ();
            return;
        }
        else if ( (port >= VIR_COM0) && (port<=VIR_COM1))
        {
            if (sio_port[COM_DEBUG].sio_op.write)
            {
                //add by paul begin cr127457
                if (0 == SIO_GetTxFifoCnt (COM_DEBUG))
                {
                    if ( (s_sio_frame_ctrl.count >= (s_sio_frame_ctrl.list_size-1)) || (s_sio_dma_data.type ==  CMD_DATA_TYPE))
                    {
                        SIO_FreeDmaFrameData (&s_sio_dma_data);
                    }

                    s_sio_dma_data.type = INVAIL_DATA_TYPE;
                    SCI_RestoreIRQ();
                    return;
                }
                else
                {
                    //s_sio_is_need_pad_flag = TRUE;  //why need padding? disable it for less usb interrupt
                    s_usb_trace_cnt++;
                    if(SIO_GetTxFifoCnt (COM_DEBUG) >= length)
                    {
                      s_sio_tx_dma_is_start = TRUE;
                    (sio_port[COM_DEBUG].sio_op.write) (SIO_PhyPortfromIndex (port),
                                                        (uint8 *) addr,
                                                        length);}
		       else
			   {
                     TM_SendTestPointRequest (0x88888, SIO_GetTxFifoCnt (COM_DEBUG));
                }

                    SCI_RestoreIRQ();
                    return;
                }

                //add by paul.luo end
            }
            else
            {
                SCI_RestoreIRQ();
                return; /*lint !e527 comfirmed by xuepeng*/
            }

        }
        else if ( (DSK_COM0 <= port) && (DSK_COM1 >= port))
        {
            s_sio_tx_dma_is_start = TRUE;
            SCI_RestoreIRQ();
            (sio_port[COM_DEBUG].sio_op.write) (SIO_PhyPortfromIndex (port),
                                                (uint8 *) addr,
                                                length);
            return;
        }
        else
        {
            SCI_RestoreIRQ();
            return; /*lint !e527 comfirmed by xuepeng*/
        }

    }
}


/*
write data to frame buffer
head_ptr : message head ptr to send
head_len: message leng
body_ptr: message body ptr, only used in send_log_pacekt
body_len: message body length
frame_header: frame header  7e +seq number + length + cmd type + cmd subtype + lost num
header_len: length
*/
LOCAL int32 _SIO_Sendpacket (uint8 *head_ptr, // Message head
                             int    head_len,   // Message head len
                             uint8 *body_ptr,  // Message body, maybe NULL
                             int    body_len,
                             FRM_HEAD_T *frame_header,int header_len)
{
#ifdef TRACE_INFO_SUPPORT
    int32     size;
    uint32  status = 0;
    uint8 head = 0;
    uint8 *buf = NULL;
    uint16 buf_size =  0 ;
    uint16 start =  0;
    int32 head_ret = 0;
    int32 body_ret = 0;
    int32 max_size = 0;
#if defined(PRODUCT_DM)
    volatile uint16 cnt;
    uint32 cur_tick, last_tick;
#endif
    SME_HEAD_T *frame_ptr = NULL;

//    SCI_ASSERT (head_ptr != NULL);/*assert verified*/ //optmize
//    SCI_ASSERT (head_len != 0);/*assert verified*/  //optmize

#ifdef CARD_LOG
    LogSaveOutput_Send_Signal_Message ( (uint8 *) head_ptr, head_len, (uint8 *) body_ptr, body_len);

#endif



    if (sio_port[COM_DEBUG].open_flag != SIO_ALREADY_OPEN)
    {
        return -1;
    }

    if(sio_port[COM_DEBUG].phy_port == VIR_COM1)
    {
        if(UCOM_PortIsOpenForWrite(SIO_PhyPortfromIndex(sio_port[COM_DEBUG].phy_port)) == 0)
        {
            //Debug_PrintDec("_SIO_Sendpacket return",__LINE__);
            return -1;
        }
    }
    s_log_count++;

    /*add for lost num display in 3g platform*/
    /*first save the sme lost num into data.lost_num to prevent sme message lost*/
    if (frame_header == NULL)
    {
       frame_ptr = (SME_HEAD_T *) (head_ptr);
        data.lost_frm_num += frame_ptr->lost_frm_num;
    }

    /* Get the total size of the message */
    size = head_len + body_len;

    //When the trace is too long then throw it away
    if (frame_header)
    {
        /*to enable put into 7e+seq_num+len+type+subtype+lost_num+***+7e*/
        max_size = s_sio_frame_ctrl.frame_list[head].length - 12;
    }
    else
    {
        /*to enable put into 7e+seq_num+len+type+subtype+lost_num+***+7e*/
        /*in 8801h 3g platform, frame head is added in sme task*/
        max_size = s_sio_frame_ctrl.frame_list[head].length - 2;
    }

    if (size >= max_size)
    {
        data.lost_frm_num++;
        s_log_too_long_cnt++;
        return -1;
    }

    //When there is not enough space then throw it away
#if 0//defined(PRODUCT_DM)
    cnt =   s_sio_frame_ctrl.count;
    max_size = s_sio_frame_ctrl.list_size;

    cur_tick = SCI_GetTickCount();

    /*return directly without wait*/
    /*write head flag until we got free space*/
    while (cnt >= max_size)
    {
        SCI_DisableIRQ();
        cnt = s_sio_frame_ctrl.count;
        SCI_RestoreIRQ();
        last_tick = SCI_GetTickCount();

        if ( (last_tick - cur_tick) > 1000) /*1s*/
        {
            data.lost_frm_num++;
            s_trace_no_space_cnt++;
            return -1;
        }

        SCI_Sleep (5);
    }

#else
    if (s_sio_frame_ctrl.count >= s_sio_frame_ctrl.list_size)
    {
        data.lost_frm_num++;
        s_trace_no_space_cnt++;
        return -1;
    }
#endif

    SCI_DisableIRQ();

    head = s_sio_frame_ctrl.head;
    buf = s_sio_frame_ctrl.frame_list[head].addr;
    buf_size = s_sio_frame_ctrl.frame_list[head].length;
    start = s_sio_frame_ctrl.frame_list[head].pos;

    /*When it is a new frame, then add the frame head*/
    if (start == 0)
    {
        if (data.lost_frm_num)
        {
            s_lost_frame_cnt = data.lost_frm_num;
        }

        buf[start++] = FLAG_BYTE;

        if (frame_header)
        {
            start += SIO_WriteFrmHeadToLogBuf ( (uint8 *) &buf[start], buf_size, (uint8 *) &frame_header->start_flag, header_len);
            frame_header->seq_num++;
            frame_header->lost_frm_num  = 0;
        }
    }

    /*Judge whether have enough space to save the trace,
      if not,then should close current frame, and create a new frame to save this trace*/
    if ( (start + size)  >= (buf_size - LOG_LWORD_MARK))
    {
#if 0 //defined(PRODUCT_DM)
        cnt =   s_sio_frame_ctrl.count;
        max_size = s_sio_frame_ctrl.list_size;
        cur_tick = SCI_GetTickCount();

        SCI_RestoreIRQ();

        /*return directly without wait*/
        /*write head flag until we got free space*/
        while (cnt >= max_size-1)
        {
            cnt = s_sio_frame_ctrl.count;

            last_tick = SCI_GetTickCount();

            if ( (last_tick - cur_tick) > 1000) /*1s*/
            {
                data.lost_frm_num++;
                s_trace_no_space_cnt++;
                return -1;
            }

            SCI_Sleep (5);
        }

        SCI_DisableIRQ();
#else
        if (s_sio_frame_ctrl.count >= s_sio_frame_ctrl.list_size)
        {
            data.lost_frm_num++;
            status = (uint32)-1;
            s_trace_no_space_cnt++;
        }
        else
#endif
        {
            //Add Finish Flag
#if 0 //may get the wrong length that lead to log lost
#if defined(PRODUCT_DM)
        /*For TD platform log length is set by SME module
          */
#else

        /*For GSM platform, to support  endian adaptive feature of channel server,
          *we need set correct log length here.
          */

#if defined(CHIP_ENDIAN_LITTLE)
        #define  LOG_LENGTH_HI   6
        #define  LOG_LENGTH_LO   5
#else
        #define  LOG_LENGTH_HI   5
        #define  LOG_LENGTH_LO   6
#endif
            uint16 packet_size = start - 1;

            buf[LOG_LENGTH_LO] = packet_size& 0xff;
            if((buf[LOG_LENGTH_LO] == ESCAPE_BYTE)||
			   ( buf[LOG_LENGTH_LO] == FLAG_BYTE))
            {
                /* tow more bytes are added when low byte of packet_size is equal to  0x7D or 0x7E*/
                buf[LOG_LENGTH_LO] += 2;
                buf[start++] = 0x0D;
                buf[start++] = 0x0A;
            }
            buf[LOG_LENGTH_HI] = packet_size>>8;
#endif
#endif
            buf[start++] = FLAG_BYTE;
            SCI_MEMSET ( ( (uint8 *) &buf[start]), FLAG_BYTE, (buf_size - start));

            s_sio_frame_ctrl.frame_list[head].pos = start;
            s_sio_frame_ctrl.count++;

            //Create a new frame
            s_sio_frame_ctrl.head++;

            if (s_sio_frame_ctrl.head >= s_sio_frame_ctrl.list_size)
            {
                s_sio_frame_ctrl.head = 0;
            }

            if (s_sio_frame_ctrl.count >= s_sio_frame_ctrl.list_size) {
                data.lost_frm_num++;
                status = (uint32)-1;
                s_trace_no_space_cnt++;
            } else {
                head = s_sio_frame_ctrl.head;
                buf = s_sio_frame_ctrl.frame_list[head].addr;
                buf_size = s_sio_frame_ctrl.frame_list[head].length;

                if (data.lost_frm_num)
                {
                    s_lost_frame_cnt = data.lost_frm_num;
                }


                if (frame_header)
                {
                    s_sio_frame_ctrl.frame_list[head].pos = SIO_WriteFrmHeadToLogBuf ( (uint8 *) buf, buf_size, (uint8 *) &data.start_flag, 11);

                    data.seq_num++;
                    data.lost_frm_num  = 0;
                }
                else
                {
                    /*it's the beginning of a new frame, so add 0x7e first!!*/
                    buf[0] = FLAG_BYTE;
                    s_sio_frame_ctrl.frame_list[head].pos = 1;
                }

                start = s_sio_frame_ctrl.frame_list[head].pos;

                /*add for lost num display in 3g platform*/
                /*before send out, remodify the lost num according to last info */
                if (frame_header == NULL)
                {
                    frame_ptr = (SME_HEAD_T *) (head_ptr);
                    frame_ptr->lost_frm_num = data.lost_frm_num;
                }

                head_ret = SIO_WriteLogToLogBuf ( (uint8 *) &buf[start], buf_size - start, (uint8 *) head_ptr, head_len);

                if (head_ret >= 0)
                {
                    /*add for lost num display in 3g platform*/
                    /*after send out, reset lost cnt*/
                    if (frame_header == NULL)
                    {
                        data.lost_frm_num = 0;
                    }

                    if (body_ptr != NULL)
                    {
                        body_ret = SIO_WriteLogToLogBuf ( (uint8 *) &buf[start + head_ret], buf_size - start - head_ret, (uint8 *) body_ptr, body_len);

                        if (body_ret >= 0)
                        {
                            s_sio_frame_ctrl.frame_list[head].pos = start + head_ret + body_ret;
    //                        s_vaild_log_count++;
                        }
                        else
                        {
                            data.lost_frm_num++;
                            status = (uint32)-1;
                            s_log_too_long_cnt++;
                        }
                    }
                    else
                    {
                        s_sio_frame_ctrl.frame_list[head].pos = start + head_ret;
     //                   s_vaild_log_count++;
                    }

                }
                else
                {
                    data.lost_frm_num++;
                    status = (uint32)-1;
     //               s_log_too_long_cnt++;
                }
    	    }
        }
    }
    else
    {

        /*add for lost num display in 3g platform*/
        /*before send out, remodify the lost num according to last info */
        if (frame_header == NULL)
        {
            frame_ptr = (SME_HEAD_T *) (head_ptr);
            frame_ptr->lost_frm_num = data.lost_frm_num;
        }

        head_ret = SIO_WriteLogToLogBuf ( (uint8 *) &buf[start], buf_size - start, (uint8 *) head_ptr, head_len);

        if (head_ret >= 0)
        {
            /*add for lost num display in 3g platform*/
            /*after send out, reset lost cnt*/
            if (frame_header == NULL)
            {
                data.lost_frm_num = 0;
            }

            if (body_ptr != NULL)
            {
                body_ret = SIO_WriteLogToLogBuf ( (uint8 *) &buf[start + head_ret], buf_size - (start + head_ret), (uint8 *) body_ptr, body_len);

                if (body_ret >= 0)
                {
                    s_sio_frame_ctrl.frame_list[head].pos = start + head_ret + body_ret;
//                    s_vaild_log_count++;
                }
                else
                {
                    data.lost_frm_num++;
                    status = (uint32)-1;
                    s_log_too_long_cnt++;
                }
            }
            else
            {
                s_sio_frame_ctrl.frame_list[head].pos = start + head_ret;
//                s_vaild_log_count++;
            }

        }
        else
        {
            data.lost_frm_num++;
            status = (uint32)-1;
//            s_log_too_long_cnt++;
        }

    }

    if (frame_header == NULL)
    {/*this condition only valid in td platform*/
        buf[s_sio_frame_ctrl.frame_list[head].pos++] = FLAG_BYTE;
        buf[s_sio_frame_ctrl.frame_list[head].pos++] = FLAG_BYTE;
    }

    SCI_RestoreIRQ();

    SIO_SendData();

    return status;
#else
    return  0;
#endif   // TRACE_INFO_SUPPORT
}

/*
 flush current sim frame packet, only used in 8801h platform
*/
LOCAL void _SIO_FlushCurPacket (void)
{
    SIO_FRAME_CTRL_T   *cur_frame_ptr = NULL;
    uint8 head = 0;
    uint8 *buf = NULL;
    uint16 buf_size =  0 ;
    uint16 start =  0;
    uint16 cnt,max_cnt;

    cur_frame_ptr = &s_sio_frame_ctrl;
    head     = cur_frame_ptr->head;
    buf        = cur_frame_ptr->frame_list[head].addr;
    buf_size = cur_frame_ptr->frame_list[head].length;
    start      = cur_frame_ptr->frame_list[head].pos;
    max_cnt = cur_frame_ptr->list_size;

    buf[start++] = FLAG_BYTE;
    SCI_MEMSET ( ( (uint8 *) &buf[start]), FLAG_BYTE, (buf_size - start));

    head++;

    if (head >= max_cnt)
    {
        head = 0;
    }

    cur_frame_ptr->head = head;
    cur_frame_ptr->frame_list[head].pos = 0;

    SCI_DisableIRQ();

    cnt = s_sio_frame_ctrl.count;

    if (cnt < max_cnt)
    {
        s_sio_frame_ctrl.count++;
    }

    SCI_RestoreIRQ();

    SIO_SendData();
}

/*
param: src: src buff ptr
           size: length to write
           written length: real size written
ret value : buf full or not

*/
LOCAL uint32 SIO_WritePPPFrameToBuf (int encode, uint8 *src, uint32 size,uint32 *written_size)
{
    DATA_FRAME_T  *list_ptr = s_sio_ppp_ctrl.frame_list;
    register int    data_size    = 0;
    uint32           ret_val        = PPP_BUFFER_NOT_FULL;
    uint16           header,  count,max_cnt;

    if (s_sio_ppp_ctrl.count >= s_sio_ppp_ctrl.list_size)
    {
        * written_size = 0;
        return PPP_BUFFER_FULL;
    }

    header   =  s_sio_ppp_ctrl.head;
    count     =  s_sio_ppp_ctrl.count;
    max_cnt =  s_sio_ppp_ctrl.list_size;

    /*don't reset the positon info when buffer is full
      in previous handle, we will fill the remain buffer with 0x7e to meet 6x00 requirement.
      but it will increase the payload and transfer time, in latest version, we decide delete the extra fill buffer
      so at the transfer we will use the buf.pos info to indicate how many in the buffer
      1. pos is zeor: the current frame is full
      2. pos is < buf size, current frame is pos
    */
    while (size != 0)
    {
		if(0 == encode){
				list_ptr[header].addr[list_ptr[header].pos++] = *src;

				if (list_ptr[header].pos >= list_ptr[header].length)
				{
					count++;
					header++;

					if (header >= max_cnt)
					{
						header = 0;
					}

					if (count >= max_cnt)
					{
						*written_size = data_size+1;
						s_sio_ppp_ctrl.head    = header;
						s_sio_ppp_ctrl.count   = count ;
						return PPP_BUFFER_FULL;
					}
					else
					{
						/*keep the postion info, the postion info will be used when refill the data to buffer*/
						list_ptr[header].pos = 0;
					}
				}
		}else{
        		switch (*src)
		        {
		            case FLAG_BYTE:
		            case ESCAPE_BYTE:
		                list_ptr[header].addr[list_ptr[header].pos++] = ESCAPE_BYTE;

		                if (list_ptr[header].pos >= list_ptr[header].length)
		                {
		                    count++;
		                    header++;

		                    if (header >= max_cnt)
		                    {
		                        header = 0;
		                    }

		                    if (count >= max_cnt)
		                    {
		                        *written_size = data_size;
		                        s_sio_ppp_ctrl.head  = header;
		                        s_sio_ppp_ctrl.count = count ;
		                        return PPP_BUFFER_FULL_ERR1;
		                    }
		                    else
		                    {
		                        /*keep the postion info, the postion info will be used when refill the data to buffer*/
		                        list_ptr[header].pos = 0;
		                    }
		                }

		                list_ptr[header].addr[list_ptr[header].pos++] = *src ^ COMPLEMENT_BYTE;

		                if (list_ptr[header].pos >= list_ptr[header].length)
		                {

		                    count++;
		                    header++;

		                    if (header >= max_cnt)
		                    {
		                        header = 0;
		                    }

		                    if (count >= max_cnt)
		                    {
		                        *written_size = data_size+1;
		                        s_sio_ppp_ctrl.head    = header;
		                        s_sio_ppp_ctrl.count   = count ;
		                        return PPP_BUFFER_FULL_ERR2;
		                    }
		                    else
		                    {
		                        /*keep the postion info, the postion info will be used when refill the data to buffer*/
		                        list_ptr[header].pos = 0;
		                    }
		                }

		                break;
		            default:
		                list_ptr[header].addr[list_ptr[header].pos++] = *src;

		                if (list_ptr[header].pos >= list_ptr[header].length)
		                {
		                    count++;
		                    header++;

		                    if (header >= max_cnt)
		                    {
		                        header = 0;
		                    }

		                    if (count >= max_cnt)
		                    {
		                        *written_size = data_size+1;
		                        s_sio_ppp_ctrl.head    = header;
		                        s_sio_ppp_ctrl.count   = count ;
		                        return PPP_BUFFER_FULL;
		                    }
		                    else
		                    {
		                        /*keep the postion info, the postion info will be used when refill the data to buffer*/
		                        list_ptr[header].pos = 0;
		                    }

		                }

		                break;
		        }
		}
        data_size ++;
        src++;
        size--;
    }

    *written_size = data_size;
    s_sio_ppp_ctrl.head    = header;
    s_sio_ppp_ctrl.count   = count ;
    return ret_val;
}

PUBLIC  int SIO_FastWritePPPFrame (uint32 port, uint8 *src, int size)
{
    int send_size = 0;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/
    SCI_ASSERT (src != NULL);/*assert verified*/

    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return size;
    }

    send_size = SIO_SendPacket (src,size);

    if (send_size == -1)
    {
        return -1;
    }
    else
    {
        return size;
    }
}

PUBLIC  int SIO_WritePPPFrame (uint32 port, uint8 *src, int size)
{
#if 0
    uint32 old_head = 0;
    uint32 old_count = 0;
    uint32 written_size = 0;
    uint32 ret;
    uint8 *src_ptr = src;
    DATA_FRAME_T  *list_ptr = s_sio_ppp_ctrl.frame_list;
    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/
    SCI_ASSERT (src_ptr != NULL);/*assert verified*/

    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return 0;
    }

    if (size >= ( (s_sio_ppp_ctrl.list_size - s_sio_ppp_ctrl.count) << MAX_SIO_PPP_BITS_SHIFT))
    {
        /*
        If the data length more than buffer length, then return
        */
        return 0;
    }


    SCI_DisableIRQ();

    old_head = s_sio_ppp_ctrl.head;
    old_count = s_sio_ppp_ctrl.count;

    list_ptr[s_sio_ppp_ctrl.head].pos = 0;
    list_ptr[s_sio_ppp_ctrl.head].addr[list_ptr[s_sio_ppp_ctrl.head].pos++] = FLAG_BYTE;

    ret = SIO_WritePPPFrameToBuf (src, size,&written_size);

    if (PPP_BUFFER_NOT_FULL == ret) //Success
    {
        //The last should be one frame
        list_ptr[s_sio_ppp_ctrl.head].addr[list_ptr[s_sio_ppp_ctrl.head].pos++] = FLAG_BYTE;

        SCI_MEMSET ( ( (uint8 *) &list_ptr[s_sio_ppp_ctrl.head].addr[list_ptr[s_sio_ppp_ctrl.head].pos]),
                     0x7E,
                     (list_ptr[s_sio_ppp_ctrl.head].length - list_ptr[s_sio_ppp_ctrl.head].pos));

        list_ptr[s_sio_ppp_ctrl.head].pos = list_ptr[s_sio_ppp_ctrl.head].length;
        s_sio_ppp_ctrl.count++;

        //Prepare for next frame
        s_sio_ppp_ctrl.head++;

        if (s_sio_ppp_ctrl.head >= MAX_SIO_PPP_FRAME_SIZE)
        {
            s_sio_ppp_ctrl.head = 0;
        }

        list_ptr[s_sio_ppp_ctrl.head].pos = 0;

        s_sio_has_cmd_req = TRUE; //Should Send the data first
    }
    else
    {
        s_sio_ppp_ctrl.head = old_head;
        s_sio_ppp_ctrl.count = old_count;
    }

    SCI_RestoreIRQ();

    SIO_SendData();

    return size;
#else
    //int32 free_size = 0;
    int32 send_size;
    //int32 remain_size = 0;
    //int32 count_send_size = 0;
    int32 cnt;
    uint16 count = 0;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/
    SCI_ASSERT (src != NULL);/*assert verified*/

    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return size;
    }

    if (REFPARAM_GetUartRecvMode() == 1) // add for l1it and eng phone
    {
        send_size = SIO_SendPacket (src,size);

        if (send_size == -1)
        {
            return -1;
        }
        else
        {
            return size;
        }
    }

    if (size >= (int32) s_sio_frame_ctrl.frame_list[0].length)
    {
        _SIO_FlushCurPacket();
        cnt = 0;

        for (;;)
        {
            cnt++;

            if (cnt > 300)
            {
                break;
            }

            count = s_sio_frame_ctrl.count ;

            if (count == 0)
            {
                break;
            }

            SCI_SLEEP (10);
        }

        send_size = SIO_SendPacket (src,size);
    }
    else
    {
        if (comm_mutex)
        {
            SCI_GetMutex (comm_mutex,  SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0);
        }

        send_size = _SIO_Sendpacket (src,size,NULL,NULL,NULL,NULL);

        if (comm_mutex)
        {
            SCI_PutMutex (comm_mutex);
        }
    }

    if (send_size == -1)
    {
        return -1;
    }
    else
    {
        return size;
    }

#endif
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_ClearError()                                                 *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Clear serial port error.                                              *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return -1 now.                                                        *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC  uint32 SIO_ClearError (uint32 port,
                               uint32 *error_ptr,
                               UARTSTAT_PTR_T *uart_stat_ptr
                              )
{
    /* We will support this function later! */
    return ( (uint32)-1);
}
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_SendLogPacket()                                              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Send a log packet to the buffer.                                      *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return -1 now.                                                        *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/

PUBLIC int SIO_SendLogPacket (uint32 *head_ptr, // Message head
                              int    head_len,   // Message head len
                              uint32 *body_ptr,  // Message body, maybe NULL
                              int    body_len    // Message bodu len
                             )
{
#if defined(PRODUCT_DM)
    return SCI_SendLogPacket (head_ptr,head_len,body_ptr,body_len);
#else
    return _SIO_Sendpacket ( (uint8 *) head_ptr, head_len,
                             (uint8 *) body_ptr,body_len,
                             &data, 11);
#endif
}

#ifdef BOOTUP_UART_LOG_SUPPORT
/*
* Desctiption: the following snippet separated by "BOOTUP_UART_LOG_SUPPORT"
*   is designed for boot-up uart log, and for the purpose of debug.
*   if necessary, log can be output from UART1 by using "SCI_TraceLow"
*   during the boot-up stage
* Author: @vine.yuan 2010.2.3 (quote from reference code)
*/
#define BOOTUP_LOG_MAX_LEN      (256)
#if defined(PLATFORM_UWS6121E)
#define SIO_TX_EMPTY(s)         ((s) & 0xFF)

#define WAIT_FIFO_EMPTY                                          \
    {                                                            \
        while( SIO_TX_EMPTY(hwp_uart2->uart_txfifo_sta));\
    }
#else
#define SIO_TX_EMPTY(s)         ((s) & 0xFF00)

#define WAIT_FIFO_EMPTY                                          \
    {                                                            \
        while( SIO_TX_EMPTY(*(volatile uint32*)(ARM_UART1_BASE + ARM_UART_STS1)));\
    }
#endif
/*****************************************************************************/
// Description : write char into Uart_txd fifo
// Global resource dependence :
// Author : vine.yuan @2010
// Note :
/*****************************************************************************/
LOCAL void WriteCharToUART (char c)
{
#if defined(PLATFORM_UWS6121E)
	while(hwp_uart2->uart_txfifo_stat & 0xFF >= ARM_UART_TX_FIFO_CNT(127) );
	hwp_uart2->uart_tx = c;
#else
    while ( ( ( (* (volatile uint32 *) (ARM_UART1_BASE + ARM_UART_STS1)) >> 8) &0xFF) >= 32) {};

    * (volatile uint32 *) (ARM_UART1_BASE + ARM_UART_TXD) = c;
#endif
}
#endif //BOOTUP_UART_LOG_SUPPORT

#define UART_LOG_MAX_LEN      (256)


PUBLIC int SIO_SendTracePacket (
    uint32 *src,  // Message head
    int    len   // Message head len
)
{
#if defined(PRODUCT_DM)
    //after assert ,can not send trace to sme buff
    return SCI_SendTracePacket ( (uint8 *) src, (uint32) len);
#else
#ifdef CARD_LOG
    LogSaveOutput_Send_Trace_Message ( (char *) src, (uint16) len);
#endif

#if 0  //tao.shi debug

    if (sio_port[COM_DEBUG].open_flag != SIO_ALREADY_OPEN)
    {
        return len;
    }

    if(sio_port[COM_DEBUG].phy_port == VIR_COM1)
	{
		if(UCOM_PortIsOpenForWrite(SIO_PhyPortfromIndex(sio_port[COM_DEBUG].phy_port)) == 0)
		{
			//Debug_PrintDec("_SIO_Sendpacket return",__LINE__);
			return -1;
		}
	}

if(sio_port[COM_DEBUG].phy_port<= COM3)
{
        char format_str[UART_LOG_MAX_LEN + 11 + 5];
        uint8*buf = format_str;
        uint32 start=0,buf_size =0;
        uint32 port;
        port = sio_port[COM_DEBUG].phy_port;
        buf_size=UART_LOG_MAX_LEN + 11 +5;

        //To add format header into buffer to be consistent with channel server.
        start += SIO_WriteFrmHeadToLogBuf ( (uint8 *) buf, buf_size,
                                            (uint8 *) &data.start_flag, 11);

        //If log string is too long, it should be trimmed down to maximum.
        if (len > UART_LOG_MAX_LEN)
        {
            len = UART_LOG_MAX_LEN;
        }

        //To write log into buffer based on the specified format
        start += SIO_WriteTraceToLogBuf ( (uint8 *) &buf[start], buf_size - start,
                                          (uint8 *) src, len);

        //Add end Flag
        buf[start++] = FLAG_BYTE;

        SCI_MEMSET ( ( (uint8 *) &buf[start]), 0x7E, (buf_size - start));

        (sio_port[COM_DEBUG].sio_op.write) (SIO_PhyPortfromIndex (port),
                                                        (uint8 *) buf,
                                                        start);

        return len;
    }
#endif

	if (sio_port[COM_DEBUG].open_flag != SIO_ALREADY_OPEN)
    {
        return len;
    }

    if (0 == SIO_GetTxFifoCnt (COM_DEBUG))
    {
        return len;
    }

    return _SIO_Sendpacket ( (uint8 *) src, len,
                             0, 0,
                             &data, 11);
#endif
}

#if 0
PUBLIC int SIO_SendPacket (void *src, int size)
{
    /* Modified SIO_SendPacket, and let the interface more flexible. */
    //return SIO_WritePPPFrame(COM_DEBUG, ((uint8 *)src) , size);
    return SIO_ShellSendPacket (NULL, 0, ( (uint8 *) src) , size);
}
#endif



PUBLIC int SIO_SendPacket (void *src, int size)
{
    return SIO_SendCommon(DATA_PPP, 1, src, size);
}

PUBLIC int SIO_SendDiag(void *src, int size)
{
    return SIO_SendCommon(DATA_DIAG, 1, src, size);
}

PUBLIC int SIO_SendCommData(void *src, int size)
{
    return SIO_SendCommon(DATA_NONE, 0, src, size);
}


PUBLIC int SIO_SendCommon(int type, int encode, void *src, int size)
{
    DATA_FRAME_T   *list_ptr = s_sio_ppp_ctrl.frame_list;
    register uint32 cur_pos, ret_val;
    uint32 written_len, cur_tick, last_tick;
    uint32 ret_status;
    volatile uint16 cnt, max_size;
    uint16  header;

#ifdef _SIO_DEBUG_
    uint32 pre_tick, curr_tick;
#endif

    if (sio_port[COM_DEBUG].open_flag != SIO_ALREADY_OPEN)
    {
        return size;
    }

    if (comm_mutex)
    {
        ret_status = SCI_GetMutex (comm_mutex,  SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SIO_MUTEX_MAX_TIME : 0);

        if (SCI_SUCCESS != ret_status)
        {
            return 0;
        }

    }

    cur_pos = 0;
    ret_val = PPP_BUFFER_NOT_FULL;
    cnt =   s_sio_ppp_ctrl.count;
    max_size = s_sio_ppp_ctrl.list_size;

    /*return directly without wait*/
    cur_tick = SCI_GetTickCount();


    /*write head flag until we got free space*/
    while (cnt >= max_size)
    {
        SCI_DisableIRQ();
        cnt = s_sio_ppp_ctrl.count;
        SCI_RestoreIRQ();
        last_tick = SCI_GetTickCount();

        if ( (last_tick - cur_tick) > 500) /*1s*/
        {
#ifdef SIO_TIMEOUT_DEBUG
            SCI_ASSERT (0);/*assert to do*/
#else
            TM_SendTestPointRequest (0x30000000+ (cnt<<8) +max_size, 20091110);

            if (comm_mutex)
            {
                SCI_PutMutex (comm_mutex);
            }

            return 0;
#endif
        }

        SCI_Sleep (5); //to release some mips for other task
    }

    header = s_sio_ppp_ctrl.head;
    list_ptr[header].pos = 0;
	if(type == DATA_PPP){
	    list_ptr[header].addr[list_ptr[header].pos++] = FLAG_BYTE;
	    list_ptr[header].addr[list_ptr[header].pos++] = FLAG_BYTE;
	    list_ptr[header].addr[list_ptr[header].pos++] = FLAG_BYTE;
    	list_ptr[header].addr[list_ptr[header].pos++] = FLAG_BYTE;
	}else if(type == DATA_DIAG){
			 list_ptr[header].addr[list_ptr[header].pos++] = FLAG_BYTE;
	}


    s_sio_packet_is_sending = TRUE;

    cur_tick = SCI_GetTickCount();

    while (cur_pos <= (uint32) size)
    {
        /*wait until have free space*/
        SCI_DisableIRQ();
        cnt = s_sio_ppp_ctrl.count;

        if (cnt >= max_size)
        {
            SCI_RestoreIRQ();
            last_tick = SCI_GetTickCount();

            if ( (last_tick - cur_tick) > 1000) /*1s*/
            {
#ifdef SIO_TIMEOUT_DEBUG
                SCI_ASSERT (0);/*assert to do*/
#else
                //              TM_SendTestPointRequest(0x40000000+(cnt<<8)+max_size, 20091110);
                s_sio_packet_is_sending = FALSE;

                SIO_SendData();

                if (comm_mutex)
                {
                    SCI_PutMutex (comm_mutex);
                }

                return 0;
#endif
            }

            /*when buf is full, sleep some time, temporary value*/
            SCI_SLEEP (5);
            continue;
        }

        cur_tick = SCI_GetTickCount();

        if (ret_val == PPP_BUFFER_FULL_ERR1)
        {
            /*a new frame, write the last byte remained last time
            */
            header = s_sio_ppp_ctrl.head;
            list_ptr[header].pos = 0;
            list_ptr[header].addr[list_ptr[header].pos++] = * ( ( (uint8 *) src) +cur_pos) ^ COMPLEMENT_BYTE;
            cur_pos +=1;
        }

        SCI_RestoreIRQ();

        /*encode , write data, return true size written*/
        SCI_DisableIRQ();
        written_len = 0;
#ifdef _SIO_DEBUG_
        pre_tick = SCI_GetTickCount();
#endif
        ret_val = SIO_WritePPPFrameToBuf ( encode,( (uint8 *) src+cur_pos) , (uint) size- cur_pos, &written_len);
        cur_pos += written_len;
#ifdef _SIO_DEBUG_
        curr_tick = SCI_GetTickCount();

        if (max_time < (curr_tick-pre_tick))
        {
            max_time = curr_tick-pre_tick;
        }

        //SCI_TRACE_LOW:"@@@@send tick used %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_3619_112_2_17_23_5_28_1079,(uint8*)"d", curr_tick-pre_tick);
#endif

        if (ret_val == PPP_BUFFER_NOT_FULL)
        {
            /*when buf is not full, exist , and fill end flag*/
            header = s_sio_ppp_ctrl.head;
            list_ptr[header].addr[list_ptr[header].pos++] = FLAG_BYTE;
            cur_pos++;
#if 0  //remove the buffer filling operation
            SCI_MEMSET ( ( (uint8 *) &list_ptr[header].addr[list_ptr[header].pos]),
                         0x7E,
                         (list_ptr[header].length - list_ptr[header].pos));

            list_ptr[header].pos = list_ptr[header].length;
#endif
            s_sio_ppp_ctrl.count++;

            /* Prepare for next frame */
            header++;

            if (header >= MAX_SIO_PPP_FRAME_SIZE)
            {
                header = 0;
            }

            list_ptr[header].pos = 0;
            s_sio_ppp_ctrl.head = header;

            s_sio_has_cmd_req = TRUE;
            SCI_RestoreIRQ();
            break;
        }
        else
        {
            /*PPP_BUFFER_FULL|| PPP_BUFFER_FULL_ERR2*/
            /*case :PPP_BUFFER_FULL_ERR1
            for this case, we need more handle,
            write the second byte ahead of the new frame, when we got free space    */
            /*when buf is full, send it right now*/
            s_sio_has_cmd_req = TRUE;
            SCI_RestoreIRQ();

            SIO_SendData();
        }
    }

    /* The last should be one frame */
    s_sio_packet_is_sending = FALSE;

    SIO_SendData();

    if (comm_mutex)
    {
        SCI_PutMutex (comm_mutex);
    }

    return size;
}

PUBLIC int SIO_SendFrame (void *src, int size)
{
    DATA_FRAME_T   *list_ptr = s_sio_ppp_ctrl.frame_list;
    uint32 ret_val;
    uint32 ret_status;
    uint32 written_len;
    uint32  max_size;
    volatile uint16  header, old_head, old_count;

    if (sio_port[COM_DEBUG].open_flag != SIO_ALREADY_OPEN)
    {
        return 0;
    }

    if (0 == SIO_GetTxFifoCnt (COM_DEBUG))
    {
        return 0;
    }

    max_size = s_sio_ppp_ctrl.list_size;

    if (size >= (int) ( (max_size - s_sio_ppp_ctrl.count) << MAX_SIO_PPP_BITS_SHIFT))
    {
        /*
        If the data length more than buffer length, then return
        */
        return 0;
    }

    if (comm_mutex)
    {
        ret_status = SCI_GetMutex (comm_mutex,  SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SIO_MUTEX_MAX_TIME : 0);

        if (SCI_SUCCESS != ret_status)
        {
            return 0;
        }

    }

    SCI_DisableIRQ();

    old_head  = s_sio_ppp_ctrl.head;
    old_count = s_sio_ppp_ctrl.count;
    header     = s_sio_ppp_ctrl.head;

    list_ptr[header].pos = 0;
    list_ptr[header].addr[list_ptr[header].pos++] = FLAG_BYTE;/*lint !e564 for symbol "header" */
    /*encode , write data, return true size written*/
    written_len = 0;
    ret_val = SIO_WritePPPFrameToBuf (1, ( (uint8 *) src) , (uint) size, &written_len);


    if (ret_val == PPP_BUFFER_NOT_FULL)
    {
        /*when buf is not full, exist , and fill end flag*/
        header = s_sio_ppp_ctrl.head;
        list_ptr[header].addr[list_ptr[header].pos++] = FLAG_BYTE; /*lint !e564 for symbol "header" */

        s_sio_ppp_ctrl.count++;

        /* Prepare for next frame */
        header++;

        if (header >= max_size)
        {
            header = 0;
        }

        list_ptr[header].pos = 0;
        s_sio_ppp_ctrl.head = header;
        s_sio_has_cmd_req = TRUE;

    }
    else
    {
        /*PPP_BUFFER_FULL|| PPP_BUFFER_FULL_ERR2*/
        /*case :PPP_BUFFER_FULL_ERR1*/
        s_sio_ppp_ctrl.head = old_head;
        s_sio_ppp_ctrl.count = old_count;
    }

    SCI_RestoreIRQ();
    SIO_SendData();

    if (comm_mutex)
    {
        SCI_PutMutex (comm_mutex);
    }

    return written_len;

}


/* Eddie.Li added for support cmd shell!  end!!*/
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_ResetBuf(uint32 port)                                        *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function reset local buffer of COM_PORT                          *
 **     (only useful for COM_DEBUG log buffer)                                *
 ** RETURN VALUE                                                              *
 **     VOID                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC void SIO_ResetBuf (uint32 port)
{
    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    SCI_DisableIRQ();

    switch (port)
    {
        case COM_DEBUG:
            PPP_frame_type   = SIO_FRAME_NONE;    // No PPP frame
            log_frm_flag     = LOG_FRM_NONE;      // No log frame
            char_mask_mode   = SIO_CHAR_NORMAL;   // No masked char

            /* Set log buffer. */
            break;

        case COM_DEBUG2:
            break;


        case COM_DATA:
            break;
        default:
            break;
    }

    SCI_RestoreIRQ();
}
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_WaitEvent()                                                  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function wait a comm event from a specific port.                 *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return the True or Flase.                                             *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 SIO_WaitEvent (uint32 port, uint32 *event_mask)
{
    uint32 actual_event;
    uint32 status;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    status = SCI_GetEvent (comm_event,
                           (port << 1),
                           SCI_AND_CLEAR,
                           &actual_event,
                           SCI_WAIT_FOREVER);

    /* @Richard, I will modified this tell upper task. */
    *event_mask = 0;

    return status;
}

/* -----------------------------------------------------------------------------------------------

   Ring Buffer Management Routines

   Each ring buffer has:-
   An 'in' pointer, which points to the next location that new data will be inserted at.
   An 'out' pointer, which points to the next character to be removed from the buffer.
   A 'limit' value, which is the size of the data array.
   A 'data' array, of 0..(limit-1) bytes.

   When in == out, the buffer is considered empty.
   When (in + 1 % limit) == out, the buffer is considered full.

   Note that if you have a 'limit' of 10, you will only be able to store 9 characters before
   the buffer full condition is reached.

   ----------------------------------------------------------------------------------------------- */

/*
** Returns the number of characters that can be added to this ring before it becomes full.
*/
LOCAL int ring_space (SIO_BUF_S *pRing)
{
    int ret = pRing->start_point - pRing->end_point - 1;
    return (ret >= 0) ? ret : (ret + (int) (pRing->size));
}

/*
** Returns the number of continuous memory space that can be memcpy to ring buf at one time.
*/
LOCAL int ring_memcpy_space (SIO_BUF_S *pRing)
{
    int ret = pRing->start_point - pRing->end_point - 1;

    if (ret>=0)
    {
        return ret;
    }
    else
    {
        if (pRing->start_point == 0)
        {
            return (int) pRing->size - pRing->end_point - 1;
        }
        else
        {
            return (int) pRing->size - pRing->end_point;
        }
    }
}

/*
** This function is for buf end point move only a memcpy operation on ring buf!
** after the memcpy,procedure should call this function to modify the end point!
** Please use this function in careful!!!
*/
LOCAL void ring_end_point_move (SIO_BUF_S *pRing, int nNum)
{
    pRing->end_point += nNum;

    if (pRing->end_point >= (int) pRing->size)
    {
        pRing->end_point -= (int) pRing->size;
    }
}

/*
** Add 'ch' to this ring.
** Returns -1 if there is not enough room.
** Returns 0 otherwise.
*/
LOCAL int ring_add (SIO_BUF_S *pRing, int ch)
{
    if (ring_space (pRing) == 0)
    {
        return -1;
    }

    pRing->sio_buf_ptr[pRing->end_point++] = ch;

    if (pRing->end_point >= (int) pRing->size)
    {
        pRing->end_point = 0;
    }

    return 0;
}

/*
** Remove a character from this ring.
** Returns -1 if there are no characters in this ring.
** Returns the character removed otherwise.
*/
LOCAL int ring_remove (SIO_BUF_S *pRing)
{
    int ret;

    if (pRing->end_point == pRing->start_point)
    {
        return -1;
    }

    ret = pRing->sio_buf_ptr[pRing->start_point++];

    if (pRing->start_point >= (int) pRing->size)
    {
        pRing->start_point = 0;
    }

    return ret;
}

PUBLIC int uart_init (int unit)
{
    return 0;
}

LOCAL void uart_closehook (void)
{
    //  irq_Disable( IRQSerialA | IRQSerialB );
}

PUBLIC void uart_putc (int port, int ch)
{
    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return ;
    }

    if (!gprs_debug)
    {
        SIO_INFO_S *pUart = &sio_port[port];

        /*
        * important
        * here we should judge whether port is uart1, and whether being used by dsp
        */
        // #richard.yang added start
        // #richard.yang added end

        SCI_DisableIRQ();

        if (sio_port[port].tx_buf.start_point - sio_port[port].tx_buf.end_point - 1 == 0)
        {
            /* note we had to wait for this port */
            //     pUart->tx_wait++;

            /* turn IRQs back off */
            SCI_RestoreIRQ();

            /*
             * wait for the ISR to drain some data from the ring buffer
             * (let's just hope that 'volatile' really works, shall we?)
             * NB: It is important that this loop is NOT critical.
             */
            //while ( ring_space(&pUart->tx_buf) == 0 );
            while (sio_port[port].tx_buf.start_point - sio_port[port].tx_buf.end_point - 1 == 0)
                {}

            SCI_DisableIRQ();

            ring_add (&pUart->tx_buf, ch);

            SCI_RestoreIRQ();
        }
        else
        {
            //ring_add( &pUart->tx_buf, ch );
            sio_port[port].tx_buf.sio_buf_ptr[sio_port[port].tx_buf.end_point++] = ch;

            if (sio_port[port].tx_buf.end_point >= (int) sio_port[port].tx_buf.size)
            {
                sio_port[port].tx_buf.end_point = 0;
            }

            SCI_RestoreIRQ();
        }
    }
    else
    {
        ppp_buf.buf[ppp_buf.cur_offset++] = (uint8) ch;
    }
}

/*
** take the mutex to protect the tx buffer of PPP.
**1 : success  0: fail
*/
PUBLIC int uart_mix_sem_take (void)
{
    if (MIX_MODE == user_port_mode)
    {
        if (SCI_SUCCESS != SIO_SEMTAKE)
        {
            //SCI_TRACE_LOW:"SIO writecmdres, mutex operate error\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4032_112_2_17_23_5_29_1080,(uint8*)"");
            return 0;
        }
    }

    return 1;
}


/*
** give the mutex to protect the tx buffer of ppp
*/
PUBLIC void uart_mix_sem_give (void)
{
    if (MIX_MODE == user_port_mode)
    {
        SIO_SEMGIVE;
    }
}



/*
** Fill the protocol header of Mix mode.
*/
#ifdef PPP_USB

PUBLIC void uart_ppp_fill_header (int end_point,uint8 port_index)
#else
PUBLIC void uart_ppp_fill_header (int end_point)
#endif
{
#ifdef PPP_USB
	SCI_PPP_TRACE("ppp:uart_ppp_fill_header,port%d",port_index);

       if(COM_DATA2 == port_index)
	   	return;
#endif

    if (MIX_MODE == user_port_mode)
    {
        int i = 0;
        int len = 0;
        int fill_point = end_point;
        uint8 checksum = 0;
        SIO_BUF_S *pTxbuffer = & sio_port[COM_DATA].tx_buf;

        // get the length of the ppp data.
        len = ( (int) g_end_point >= end_point) ?
              ( (int) g_end_point - end_point -3) :
              ( (int) g_end_point - end_point + (int) pTxbuffer->size -3) ;

        // ppp type
        pTxbuffer->sio_buf_ptr[fill_point ++ ] = SIO_DATA_PACKET;

        if (fill_point >= (int) pTxbuffer->size)
        {
            fill_point = 0;
        }

        // length (LSB 8 bits)
        pTxbuffer->sio_buf_ptr[fill_point ++ ] = len & 0xFF;

        if (fill_point >= (int) pTxbuffer->size)
        {
            fill_point = 0;
        }

        // type | MSB 3 bits of len
        pTxbuffer->sio_buf_ptr[fill_point ++ ] = (len >> 8) & 0x07 | SIO_DATA_PATTERN;

        fill_point = end_point;

        for (i=0; i< len+3; i++)
        {
            checksum += pTxbuffer->sio_buf_ptr[fill_point ++ ];

            if (fill_point >= (int) pTxbuffer->size)
            {
                fill_point = 0;
            }
        }

        pTxbuffer->sio_buf_ptr[ g_end_point ++ ] = checksum;

        if (g_end_point >= pTxbuffer->size)
        {
            g_end_point =  g_end_point - pTxbuffer->size;
        }

    }

}

/*
** Get the end_point of tx buffer of PPP
*/
#ifdef PPP_USB
PUBLIC int uart_mix_get_end_point(uint8 port_index)

#else
PUBLIC int uart_mix_get_end_point()
#endif
{
    int tmp;
    SIO_BUF_S *pTxBuffer = NULL;

#ifdef PPP_USB

	SCI_PPP_TRACE("ppp:uart_mix_get_end_point,port%d",port_index);

	if(COM_DATA2 == port_index)
	{
		pTxBuffer = &sio_port[port_index].tx_buf;
	    	tmp = pTxBuffer->end_point;
		g_end_point_2 = tmp;
		return tmp;
	}
#endif

    pTxBuffer = &sio_port[COM_DATA].tx_buf;
    tmp = pTxBuffer->end_point;

    g_end_point = tmp;

    if (MIX_MODE == user_port_mode)
    {
        g_end_point += MIX_PACKET_HEAD_LENGHT;

        if (g_end_point >= pTxBuffer->size)
        {
            g_end_point =  g_end_point - pTxBuffer->size;
        }
    }

    return tmp;

}


/*
** Set the end_point of Tx buffer of PPP
** and SO can send the data out.
*/

#ifdef PPP_USB
PUBLIC void uart_mix_set_end_point (uint32 end_point,uint8 port_index)
#else
PUBLIC void uart_mix_set_end_point (uint32 end_point)
#endif
{
#ifdef PPP_USB
	SCI_PPP_TRACE("ppp:uart_mix_set_end_point,port%d",port_index);
       if(COM_DATA2 == port_index)
       {
            if (sio_port[port_index].open_flag != SIO_ALREADY_OPEN)
            {
                return ;
            }

            if (end_point < DATA_PORT_SEND_BUF_SIZE)
            {
                sio_port[port_index].tx_buf.end_point  =  end_point;
                g_end_point_2 = end_point;
            }
            else
            {
                sio_port[port_index].tx_buf.end_point  =  g_end_point_2;
            }

        return;
       }
#endif

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return ;
    }

    if (end_point < DATA_PORT_SEND_BUF_SIZE)
    {
        sio_port[COM_DATA].tx_buf.end_point  =  end_point;
        g_end_point = end_point;
    }
    else
    {
        sio_port[COM_DATA].tx_buf.end_point  =  g_end_point;
    }
}

/*
** Check the buffer lenght for PPP data send out.
*/
#ifdef PPP_USB
PUBLIC int uart_mix_check (int len,uint8 port_index)
#else
PUBLIC int uart_mix_check (int len)
#endif
{

#ifdef PPP_USB
	SCI_PPP_TRACE("ppp:uart_mix_check,port%d",port_index);

	if(COM_DATA2 == port_index)
	{
	        int space = 0;
	        SIO_BUF_S *pTxBuffer = &sio_port[port_index].tx_buf;

	        // get the ring space
	        space = (pTxBuffer->start_point) - (pTxBuffer->end_point)  - 1 ;
	        space = (space >= 0) ? space : (space + (int) pTxBuffer->size) ;

		SCI_PPP_TRACE("ppp:space%d,len%d",space,len);
	        {
	            if (space < len)
	            {
	                return 0;
	            }
	            else
	            {
	                return 1;
	            }
	        }
	}
#endif

    if (!gprs_debug)
    {
        int space = 0;
        SIO_BUF_S *pTxBuffer = &sio_port[COM_DATA].tx_buf;

        // get the ring space
        space = (pTxBuffer->start_point) - (pTxBuffer->end_point)  - 1 ;
        space = (space >= 0) ? space : (space + (int) pTxBuffer->size) ;

	SCI_PPP_TRACE("ppp:space%d,len%d !",space,len);

        if (MIX_MODE == user_port_mode)
        {
            if (space < len + MIX_PROTO_FIX_HEAD_LEN)
            {
                //SCI_TRACE_LOW:"Length 1 of Tx buffer is not long enought.the buffer size is %d , the space is %d \n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4174_112_2_17_23_5_29_1081,(uint8*)"dd", pTxBuffer->size, space);
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if (space < len)
            {
                //SCI_TRACE_LOW:"Length 2 of Tx buffer is not long enought. the buffer size is %d , the space is %d \n"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4186_112_2_17_23_5_29_1082,(uint8*)"dd", pTxBuffer->size, space);
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        return 1;
    }

}


/*
** Send the char of PPP out.
** It's the function called by PPP.
** return value is useless
*/

PUBLIC int uart_ppp_putc (int port, uint8 ch)
{
  SIO_BUF_S *pBuf = NULL;

	//maybe not need here
    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return ch;
    }

   //SCI_PPP_TRACE("ppp:uart_ppp_putc,port%d,ch %x",port,ch);

#ifdef PPP_USB

    if(COM_DATA2 == port)
     {
		pBuf= &sio_port[port].tx_buf;
		pBuf->sio_buf_ptr[g_end_point_2++] = ch;

		if (g_end_point_2 >= pBuf->size)
		{
		    g_end_point_2 = 0;
		}

		 return ch;
    }

#endif

    if (!gprs_debug)
    {
        SIO_BUF_S *pBuf = &sio_port[port].tx_buf;

        pBuf->sio_buf_ptr[g_end_point++] = ch;

        if (g_end_point >= pBuf->size)
        {
            g_end_point = 0;
        }
    }
    else
    {
        ppp_buf.buf[ppp_buf.cur_offset++] = (uint8) ch;
    }

    return ch;
}

/*
** Get character from modem uart. Returns -1 if no such
** character is available.
*/
PUBLIC int uart_ppp_getc (int port)
{
    /* @jim.zhang CR:MS7087 */
    int ch = -1;
    uint8 port_mode = user_port_mode;

    SIO_BUF_S   *pRx_buf = &sio_port[port].rx_buf;


    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return ch;
    }

#ifdef PPP_USB

    if(COM_DATA2 == port)
      port_mode = user_port_mode_2;

#endif


    if (port_mode == ATC_MODE)
    {
        return ch;
    }

    /* end CR:MS7087 */

    //ch = ring_remove( &pUart->rx_buf );
    if (pRx_buf->start_point == pRx_buf->end_point)
    {
        ch = -1; // no char to get.
        SCI_PPP_TRACE("ppp:uart_ppp_getc,port%d,start %d,end %d",port,pRx_buf->start_point,pRx_buf->end_point);

    }
    else
    {
        ch = pRx_buf->sio_buf_ptr[pRx_buf->start_point++];

        if (pRx_buf->start_point >= (int) pRx_buf->size)
        {
            pRx_buf->start_point = 0;
        }
    }

   //SCI_PPP_TRACE("ppp:uart_ppp_getc,port%d,ch %x",port,ch);

    return ch;
}

/*
** Return ability of uart to accept more data
*/
LOCAL int uart_ready (int port)
{
    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return 0;
    }

    return ring_space (&sio_port[port].tx_buf);
}

#ifndef dputchar
LOCAL void dputchar (int ch)
{
}
#endif


/*
** timer callback
** check the space. when space is lager than 8888888, then enable rx interrupt.
*/

LOCAL void Space_Check (void)
{
    SIO_INFO_S *pUart = &sio_port[COM_DATA];
   SCI_PPP_TRACE("ppp:Space_Check,port%d");

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }

    if (ring_space (&pUart->rx_buf) > EMPTY_SPACE_WATER_HIGH_MARK)
    {
        (sio_port[COM_DATA].sio_op.rx_enable) (SIO_PhyPortfromIndex (pUart->phy_port),TRUE);
    }
    else
    {
        SCI_DeactiveTimer (Space_timer);
        SCI_ChangeTimer (Space_timer, Space_Check, SPACE_CHECK_DELAY);/*lint !e64 */
        SCI_ActiveTimer(Space_timer);
    }

}

#ifdef PPP_USB
LOCAL void Space_Check_2 (void)
{
    SIO_INFO_S *pUart = &sio_port[COM_DATA2];

   SCI_PPP_TRACE("ppp:Space_Check_2,port%d");

    if (sio_port[COM_DATA2].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }

    if (ring_space (&pUart->rx_buf) > EMPTY_SPACE_WATER_HIGH_MARK)
    {
        (sio_port[COM_DATA2].sio_op.rx_enable) (SIO_PhyPortfromIndex (pUart->phy_port),TRUE);
    }
    else
    {
        SCI_DeactiveTimer (Space_timer_2);
        SCI_ChangeTimer (Space_timer_2, Space_Check_2, SPACE_CHECK_DELAY);/*lint !e64 */
        SCI_ActiveTimer(Space_timer_2);
    }

}
#endif

LOCAL int getch (void)
{
    return -1;
}

LOCAL int kbhit (void)
{
    return -1;
}

#ifdef PPP_USB


/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     VOID SIO_ATC_ReadCmdLine                                              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Read AT command from sio                                              *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void SIO_ATC_ReadCmdLine (
    uint8 *data_ptr,       // Output argument, point to the buf which is alloced
    // by caller
    uint length,           // The length of cmd line which should be read from
    // sio
    uint *read_length_ptr, // The actual read length
    uint8 port_index
)
{
    uint i;
    uint8 ch;
    BOOLEAN is_zero_start = FALSE;
    SIO_INFO_S  *sio_ptr = (SIO_INFO_S *) &sio_port[port_index];

    if (sio_port[port_index].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }

   SCI_PPP_TRACE("ppp:SIO_ATC_ReadCmdLine,port%d",port_index);
    for (i = 0; i < length; i++)
    {
        ch = ring_remove (&sio_ptr->atc_buf);

        if ( (int8) ch != -1)
        {
            if (((i == 0) || (is_zero_start == TRUE)) && (ch == 0))
            {
                //filter zero ahead of AT command line, like 00 41 54
                is_zero_start = TRUE;
                continue;
            }
            else
            {
                is_zero_start = FALSE;
            }

            if (ch == atc_end_ch1[port_index] || ch == atc_end_ch2[port_index])
            {
                *data_ptr = ch;
                *read_length_ptr += 1;
                break;
            }
            else if (ch == atc_bs_ch[port_index])
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

    s_last_cmd_pos[port_index] = 0xffff;
}


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

PUBLIC void SIO_ATC_ClrCmdLine (
    uint8 port_index
)
{

    SIO_INFO_S  *sio_ptr = (SIO_INFO_S *) &sio_port[port_index];

    if (sio_port[port_index].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }
   SCI_PPP_TRACE("ppp:SIO_ATC_ClrCmdLine,port%d",port_index);

    sio_ptr->atc_buf.end_point = sio_ptr->atc_buf.start_point;

    s_last_cmd_pos[port_index] = 0xffff;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     VOID SIO_ATC_CheckNewCmd                                              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     check new AT command in receive buffer       *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
PUBLIC int SIO_ATC_CheckNewCmd(BOOLEAN *is_new_cmd_ptr, uint32 len,uint8 port_index)
{
    SIO_BUF_S *pRing = &(sio_port[port_index].atc_buf);
    int tmp_point = 0;
    int tmp_len = 0;// cmd line include prefix or fix str
    int cmd_len = 0;

    if (pRing->end_point == pRing->start_point)
    {
        return -1;
    }

    if (len == 0)
    {
        return -1;
    }
     SCI_PPP_TRACE("ppp:SIO_ATC_CheckNewCmd,%d",port_index);

    tmp_point = pRing->start_point;

    while (tmp_point != pRing->end_point)
    {
        tmp_len++;
        if ((pRing->sio_buf_ptr[tmp_point] == atc_end_ch1[port_index]) ||
            (pRing->sio_buf_ptr[tmp_point] == atc_end_ch2[port_index]))
        {
            //filter the case cmd with one or two prefix like <CR><LF><cmd line><CR><LF>
            if (cmd_len > 0)
            {
                if ((s_last_cmd_pos[port_index] == 0xffff)||(s_last_cmd_pos[port_index] != tmp_point))
                {
                    s_last_cmd_pos[port_index] = tmp_point;
                    *is_new_cmd_ptr = TRUE;
                    break;
                }
                else
                {
                    //try to search another cmd
                    cmd_len = 0;
                    tmp_len = 0;
                }
            }
        }
        else
        {
            cmd_len++;
        }

        tmp_point++;

        if (tmp_point >= (int) pRing->size)
        {
            tmp_point = 0;
        }
    }


    if (*is_new_cmd_ptr == FALSE)
    {
        tmp_point = pRing->end_point + 1;
        if(tmp_point >= (int) pRing->size)
        {
            tmp_point = 0;
        }

        //buffer is full, so empty buffer anyway.
        if (tmp_point == pRing->start_point)
        {
            SCI_TRACE_LOW("SIO: receive buffer is full, so empty it.");
            pRing->end_point = pRing->start_point;
            s_last_cmd_pos[port_index] = 0xffff;
        }
    }

    return tmp_len;
}

#else
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     VOID SIO_ATC_ReadCmdLine                                              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Read AT command from sio                                              *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
void SIO_ATC_ReadCmdLine (
    uint8 *data_ptr,       // Output argument, point to the buf which is alloced
    // by caller
    uint length,           // The length of cmd line which should be read from
    // sio
    uint *read_length_ptr  // The actual read length
)
{
    uint i;
    uint8 ch;
    BOOLEAN is_zero_start = FALSE;
    SIO_INFO_S  *sio_ptr = (SIO_INFO_S *) &sio_port[COM_DATA];

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }

    for (i = 0; i < length; i++)
    {
        ch = ring_remove (&sio_ptr->atc_buf);

        if ( (int8) ch != -1)
        {
            if (((i == 0) || (is_zero_start == TRUE)) && (ch == 0))
            {
                //filter zero ahead of AT command line, like 00 41 54
                is_zero_start = TRUE;
                continue;
            }
            else
            {
                is_zero_start = FALSE;
            }

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

    s_last_cmd_pos = 0xffff;
}


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

PUBLIC void SIO_ATC_ClrCmdLine (
    void
)
{

    SIO_INFO_S  *sio_ptr = (SIO_INFO_S *) &sio_port[COM_DATA];

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }

    sio_ptr->atc_buf.end_point = sio_ptr->atc_buf.start_point;

    s_last_cmd_pos = 0xffff;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     VOID SIO_ATC_CheckNewCmd                                              *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     check new AT command in receive buffer       *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
PUBLIC int SIO_ATC_CheckNewCmd(BOOLEAN *is_new_cmd_ptr, uint32 len)
{
    SIO_BUF_S *pRing = &(sio_port[COM_DATA].atc_buf);
    int tmp_point = 0;
    int tmp_len = 0;// cmd line include prefix or fix str
    int cmd_len = 0;

    if (pRing->end_point == pRing->start_point)
    {
        return -1;
    }

    if (len == 0)
    {
        return -1;
    }

    tmp_point = pRing->start_point;

    while (tmp_point != pRing->end_point)
    {
        tmp_len++;
        if ((pRing->sio_buf_ptr[tmp_point] == atc_end_ch1) ||
            (pRing->sio_buf_ptr[tmp_point] == atc_end_ch2))
        {
            //filter the case cmd with one or two prefix like <CR><LF><cmd line><CR><LF>
            if (cmd_len > 0)
            {
                if ((s_last_cmd_pos == 0xffff)||(s_last_cmd_pos != tmp_point))
                {
                    s_last_cmd_pos = tmp_point;
                    *is_new_cmd_ptr = TRUE;
                    break;
                }
                else
                {
                    //try to search another cmd
                    cmd_len = 0;
                    tmp_len = 0;
                }
            }
        }
        else
        {
            cmd_len++;
        }

        tmp_point++;

        if (tmp_point >= (int) pRing->size)
        {
            tmp_point = 0;
        }
    }


    if (*is_new_cmd_ptr == FALSE)
    {
        tmp_point = pRing->end_point + 1;
        if(tmp_point >= (int) pRing->size)
        {
            tmp_point = 0;
        }

        //buffer is full, so empty buffer anyway.
        if (tmp_point == pRing->start_point)
        {
            SCI_TRACE_LOW("SIO: receive buffer is full, so empty it.");
            pRing->end_point = pRing->start_point;
            s_last_cmd_pos = 0xffff;
        }
    }

    return tmp_len;
}
#endif


PUBLIC void SIO_PPP_UsbPlugOut (void)
{
#ifdef PPP_USB
   SCI_TRACE_LOW("SIO:PPP_UsbPlugOut,mode %d",user_port_mode_2);
   if(COM_DATA2 == user_port_mode_2)
   	TCPIP_ResetPPPInterface();
#endif

}

#if defined(PLATFORM_UWS6121E)
#ifdef PPP_USB
LOCAL uint32 SIO_TxForUserPort (uint8 *data_buf,uint32 fifo_size,uint8 port_index);

#else
LOCAL uint32 SIO_TxForUserPort (uint8 *data_buf,uint32 fifo_size);
#endif
/*@jiajiang.lu 2018-07-13 */
uint32 sio_ppp_tx_freebuf_cnt(void)
{
    uint32 tmp = 0;

    if (g_pppbuf_alloc > g_pppbuf_free)
    {
		tmp = (PPP_TXBUF_MASK - (g_pppbuf_alloc - g_pppbuf_free));
	}
    else if (g_pppbuf_free > g_pppbuf_alloc)
    {
		tmp = g_pppbuf_free - g_pppbuf_alloc - 1;
	}
    else
    {
		tmp = PPP_TXBUF_MASK;
	}

    //SCI_TraceLow("ppp: free cnt %d\n", tmp);
    return tmp;
}

uint8 *sio_ppp_alloc_tx_buf(void)
{
    uint8 *tmp = NULL;

    SCI_DisableIRQ();
    tmp = &s_ppp_dma_txbuf[g_pppbuf_alloc++][0];
    if (g_pppbuf_alloc == PPP_TXBUF_CNT)
        g_pppbuf_alloc = 0;
    SCI_RestoreIRQ();

    //SCI_TraceLow("ppp: alloc 0x%x\n", tmp);
    return tmp;
}

void sio_ppp_free_tx_buf(uint8 *buf)
{
    int i;

    SCI_TraceLow("ppp: free 0x%x\n", buf);
    SCI_DisableIRQ();

    g_pppbuf_free++;
    if (g_pppbuf_free == PPP_TXBUF_CNT)
        g_pppbuf_free = 0;

    SCI_RestoreIRQ();
}

void *g_ppp_data_buf;
uint32 g_ppp_send_status;
void sio_ppp_tx_enable(int enable_flag)
{
    uint32 *ppp_len;
	uint32 ucom_name;
	uint8 port_index = COM_DATA2;

	(sio_port[port_index].sio_op.tx_enable) (SIO_PhyPortfromIndex (sio_port[port_index].phy_port),enable_flag);

    //SCI_TraceLow("ppp: sio_ppp_tx_enable g_pppbuf_alloc:%d,g_pppbuf_free:%d,g_ppp_send_status:%d\n", g_pppbuf_alloc, g_pppbuf_free, g_ppp_send_status);
    if ((g_pppbuf_alloc != g_pppbuf_free) &&
        (g_ppp_send_status == 0))
    {
        g_ppp_send_status = 1;
        g_ppp_data_buf = &s_ppp_dma_txbuf[g_pppbuf_free][0];
        ppp_len = (uint32 *)(&s_ppp_dma_txbuf[g_pppbuf_free][2044]);
        if((sio_port[port_index].sio_op.write) (SIO_PhyPortfromIndex (sio_port[port_index].phy_port),g_ppp_data_buf,*ppp_len) == 0)
        {
            //enqueue fail
            g_ppp_send_status = 0; //²»Òª×èÈûÉÏ²ã¼ÌÐøtx
        }
    }
}

void sio_ppp_rx_buff_alloc(void)
{
    if(NULL == p_ppp_rx_buf_tmp)
    {
        p_ppp_rx_buf_tmp = SCI_ALLOC(PPP_RXBUF_SIZE+4);
        if(!p_ppp_rx_buf_tmp)
            SCI_ASSERT(0);
        p_ppp_rx_buf = ((uint32)p_ppp_rx_buf_tmp + 3) & (~3);
        g_ppp_rx_ridx = g_ppp_rx_widx = 0;

        //SCI_TraceLow("sio_ppp_rx_buff_alloc p_ppp_rx_buf_tmp:0x%x ",p_ppp_rx_buf_tmp);
    }
}

void sio_ppp_rx_buff_free(void)
{
    if(p_ppp_rx_buf_tmp){
        //SCI_TraceLow("sio_ppp_rx_buff_free p_ppp_rx_buf_tmp:0x%x ",p_ppp_rx_buf_tmp);
        SCI_FREE((void*)p_ppp_rx_buf_tmp);
    }
}
int sio_ppp_getc(void)
{
    int ch = -1;

    if (g_ppp_rx_ridx != g_ppp_rx_widx)
    {
        ch = p_ppp_rx_buf[g_ppp_rx_ridx++];
        if (g_ppp_rx_ridx == PPP_RXBUF_SIZE)
            g_ppp_rx_ridx = 0;
    }

    return ch;
}


PUBLIC BOOLEAN SIO_CheckIsAtcData(uint8 *data_ptr, uint32 data_len)
{

    if(data_ptr && data_len)
    {
        if((data_ptr[0] == 'a' || data_ptr[0] == 'A') && (data_ptr[1] == 't' || data_ptr[1] == 'T'))
        {
            return TRUE;
        }
    }

    return FALSE;
}

/*
** tx send to make uart interrupt occur
*/
#ifdef PPP_USB
LOCAL void SIO_Tx_Send (uint8 port_index)
#else
LOCAL void SIO_Tx_Send (void)
#endif
{
	uint32 num=0;
	uint32 fifo_cnt;
	uint8 *p_data_txbuff = s_uart_data_txbuf;
	uint32 phy_port;

#ifdef PPP_USB
    if(COM_DATA2 == port_index)
    {

    	p_data_txbuff=s_uart_data2_txbuf;
    }
#else
   uint8 port_index=COM_DATA;
#endif

	fifo_cnt = SIO_GetTxFifoCnt (port_index);
	if(fifo_cnt > MAX_FIFO_SIZE)
	{
		fifo_cnt = MAX_FIFO_SIZE;
	}
	if (fifo_cnt)
	{
		SCI_MEMSET((uint8 *)p_data_txbuff,0,MAX_FIFO_SIZE);
	#ifdef PPP_USB
	    num = SIO_TxForUserPort (p_data_txbuff,fifo_cnt,port_index);
	#else
	    num = SIO_TxForUserPort (p_data_txbuff,fifo_cnt);
	#endif
    //add log for bug1006386
    //SCI_TraceLow("ppp:SIO_Tx_Send,port %d,num%d,32K 0x%x",port_index,num,SLEEP_GetCur32kNum());
        if (num > 0)
        {
            if((sio_port[port_index].sio_op.write) (SIO_PhyPortfromIndex (sio_port[port_index].phy_port),p_data_txbuff,num) == 0)
            {
                //tx enqueue fail,retry
                SCI_TRACE_LOW("SIO_Tx_Send:tx enqueue fail,try again,port_index %d",port_index);
                //SCI_Sleep(1); //may be called in hisr0,cannot not sleep
                (sio_port[port_index].sio_op.write) (SIO_PhyPortfromIndex (sio_port[port_index].phy_port),p_data_txbuff,num);
            }
	   #ifndef PPP_USB
            if (s_sio_uart_sleep_timer)
	        {
	        	if(SCI_IsTimerActive(s_sio_uart_sleep_timer))
	        	{
	        		SCI_DeactiveTimer(s_sio_uart_sleep_timer);
	        	}
	        	SCI_ChangeTimer(s_sio_uart_sleep_timer, SIO_UartSleepTimerExpired, SIO_UART_SLEEP_TIMER_VALUE);
	        	SCI_ActiveTimer(s_sio_uart_sleep_timer);
	        }
		#endif
	    }
	    else
	    {
	        //when using usb virtual com, then should disable it(not closing intbits in usb isr handle)
	        //it will affect nothing to disable double times for uart,
	        SIO_Tx_Int_Enable (port_index,FALSE);
	    }
	}
	else
	{
	    //here only useful for uart,when current fifocnt
	    //is more than max UART_FIFO_SIZE,then only enable uart TX
	    //if current is usb, fifo_cnt will not be zero
	    //for bug1036551
	    phy_port = SIO_GetPhyPortNo(port_index);
	    if((phy_port >= COM0) && (phy_port <= COM3))
	    	SIO_Tx_Int_Enable (port_index,TRUE);

	   SCI_TRACE_LOW("SIO_Tx_Send:fifo is empty, phy port %d",phy_port);
	}
}

void SIO_SetAtFromDebugPort(void)
{
	s_is_at_from_debug_port = TRUE;
}
#endif

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
#ifdef PPP_USB
LOCAL void WriteCmdRes(
    uint8 *res_data_ptr,   // Point to the response infomation buffer
    uint    length,         // The length of response infomation buffer
    uint8 port_index
)
#else
LOCAL void WriteCmdRes(
    uint8 *res_data_ptr,   // Point to the response infomation buffer
    uint    length         // The length of response infomation buffer
)
#endif
{

#ifdef PPP_USB

	//SCI_PPP_TRACE("ppp:WriteCmdRes,port %d,user_port_mode_2=%d,len %d",port_index,user_port_mode_2,length);

	if((COM_DATA2 == port_index))
	{
		 uint i;
	        uint8 checksum;
	        int end_point;

		SCI_PPP_TRACE_1("ppp:WriteCmdRes,port %d,user_port_mode_2=%d,len %d",port_index,user_port_mode_2,length);

		if (sio_port[port_index].open_flag != SIO_ALREADY_OPEN)
		{
		    return;
		}

    	if (1)
	    {
	        uint i;
	        uint8 checksum;
	        int end_point;


	        /* to see if the ringspace of tx buffer is enough to hold the data packet */

	       // if (! uart_mix_sem_take())
	       // {
	            //SCI_TRACE_LOW:"SIO_ATC_WriteCmdRes : uart_mix_sem_take \n"
	       //     SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4470_112_2_17_23_5_30_1086,(uint8*)"");
	      //      return;
	     //   }

	        end_point = uart_mix_get_end_point(port_index);
	        uart_mix_set_end_point (end_point,port_index);

	        //SIO_Tx_Int_Enable(COM_DATA,0);    // disable tx interrupt to avoid sending an half wrong packet.


	        {
	            for (i = 0; i < length; i++)
	            {
	                uart_ppp_putc (port_index, *res_data_ptr++);
	            }
	        }

	        /*
	         * rewirte the end_point of tx buffer,so the packet can
	         * be send out now.
	         */
	        uart_mix_set_end_point (0xFFFFFFFF,port_index);

	        SIO_Tx_Int_Enable (port_index,1); // enable tx interrupt .

	        //give mutex
	        //uart_mix_sem_give();    //SIO_SEMGIVE;
	    }
	 return;

	}

#endif

    SCI_PPP_TRACE("ppp:WriteCmdRes,handle COM_DATA");

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }

    if (!s_is_at_from_debug_port)
    {
        uint i;
        uint8 checksum;
        int end_point;

        if (gprs_debug)
        {
            //SCI_TRACE_LOW:"SIO_ATC_WriteCmdRes -gprs_debug=0x%X\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4448_112_2_17_23_5_30_1083,(uint8*)"d",  gprs_debug);
            return;
        }

        /* to see if the user port is in the DATA MODE */
        if (DATA_MODE == user_port_mode)
        {
            //SCI_TRACE_LOW:"SIO: discard AT response in DATA_MODE \n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4455_112_2_17_23_5_30_1084,(uint8*)"");
            return;
        }

        /* to see if the ringspace of tx buffer is enough to hold the data packet */
	#ifdef PPP_USB
        if (! uart_mix_check (length,COM_DATA))
	#else
        if (! uart_mix_check (length))
	#endif
        {
            //SCI_TRACE_LOW:"SIO_ATC_WriteCmdRes \n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4462_112_2_17_23_5_30_1085,(uint8*)"");
            SIO_Tx_Int_Enable (COM_DATA,1); // enable tx interrupt .
            return;
        }


        if (! uart_mix_sem_take())
        {
            //SCI_TRACE_LOW:"SIO_ATC_WriteCmdRes : uart_mix_sem_take \n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4470_112_2_17_23_5_30_1086,(uint8*)"");
            return;
        }

	#ifdef PPP_USB
        end_point = uart_mix_get_end_point(COM_DATA);
        uart_mix_set_end_point (end_point,COM_DATA);
	#else
        end_point = uart_mix_get_end_point();
        uart_mix_set_end_point (end_point);
	#endif
        //SIO_Tx_Int_Enable(COM_DATA,0);    // disable tx interrupt to avoid sending an half wrong packet.


        if (MIX_MODE == user_port_mode)
        {
            //send packet head
            uart_ppp_putc (COM_DATA, SIO_ATC_PACKET);
            uart_ppp_putc (COM_DATA, 0x0FF & length);
            uart_ppp_putc (COM_DATA, SIO_ATC_PATTERN | ( (length>>8) & 7));

            checksum = SIO_ATC_PACKET + (0x0FF & length) + (SIO_ATC_PATTERN | ( (length>>8) & 7));

            //send packet body

            for (i = 0; i < length; i++)
            {
                checksum += *res_data_ptr;
                uart_ppp_putc (COM_DATA, *res_data_ptr++);
            }

            //send checksum
            uart_ppp_putc (COM_DATA, checksum);
        }
        else
        {
            for (i = 0; i < length; i++)
            {
                uart_ppp_putc (COM_DATA, *res_data_ptr++);
            }
        }

        /*
         * rewirte the end_point of tx buffer,so the packet can
         * be send out now.
         */
     	#ifdef PPP_USB
	 uart_mix_set_end_point (0xFFFFFFFF,COM_DATA);
	#else
	 uart_mix_set_end_point (0xFFFFFFFF);
	#endif

        SIO_Tx_Int_Enable (COM_DATA,1); // enable tx interrupt .

        //give mutex
        uart_mix_sem_give();    //SIO_SEMGIVE;
    }
    else
    {
        ppp_buf.head.len = sizeof (MSG_HEAD_T) + length;

        SCI_MEMCPY (ppp_buf.buf, res_data_ptr, length);

        SIO_SendPacket (&ppp_buf, ppp_buf.head.len);
    }
}

#if 1 //defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)|| defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C)|| defined(CHIP_VER_UIS8910A)
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
extern BOOLEAN UART_GetControllerBqbMode(void);
extern void UART_Controller_Bqb_Write(const uint8 *buffer, uint16 length);
#endif
#endif

#if 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
extern uint32 g_curr_uart_bt_use;
#endif
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
#ifdef PPP_USB
void SIO_ATC_WriteCmdRes (
    uint8 *res_data_ptr,   // Point to the response infomation buffer
    uint    length,         // The length of response infomation buffer

    uint8 port_index
)
#else
void SIO_ATC_WriteCmdRes (
    uint8 *res_data_ptr,   // Point to the response infomation buffer
    uint    length         // The length of response infomation buffer
)
#endif
{
#if 1 //defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
    //if (!g_curr_uart_bt_use)
#endif
    {
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
        if(!(UART_GetControllerBqbMode()))
#endif
#endif
	#ifdef PPP_USB
	    WriteCmdRes(res_data_ptr,length,port_index);

	#else
	    WriteCmdRes(res_data_ptr,length);
	#endif

        return;
    }
}

#if 1 //defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A)
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
/******************************************************************************
// Description: It is used for bqb test for bluetooth
// Dependence:
// Author:        Paul.Luo
// DATE:            2012.12.05                                                *
// Note:
******************************************************************************/
void SIO_BT_Write(
uint8 *res_data_ptr,   // Point to the response infomation buffer
    uint    length         // The length of response infomation buffer
    )

{
    SCI_TRACE_LOW("SIO_BT_Writes");
	if(UART_GetControllerBqbMode())

	#ifdef PPP_USB
	    WriteCmdRes(res_data_ptr,length,COM_DATA);

	#else
	    WriteCmdRes(res_data_ptr,length);
	#endif
    return ;
}
#endif
#endif

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

#ifdef PPP_USB
void SIO_ATC_SetCmdLineTerminateChar (
    uint8 terminate_char1,   // Default terminate char is '\n'
    uint8 terminate_char2,    // Default terminate char is '\0'
    uint8 port_index
)
{
    atc_end_ch1[port_index] = terminate_char1;
    atc_end_ch2[port_index] = terminate_char2;
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
void SIO_ATC_SetCmdLineBackSpaceChar (
    uint8 back_space_char,  // Default backspace char value is 8
    uint8 port_index
)
{
    atc_bs_ch[port_index] = back_space_char;
}
#else
void SIO_ATC_SetCmdLineTerminateChar (
    uint8 terminate_char1,   // Default terminate char is '\n'
    uint8 terminate_char2    // Default terminate char is '\0'
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
void SIO_ATC_SetCmdLineBackSpaceChar (
    uint8 back_space_char  // Default backspace char value is 8
)
{
    atc_bs_ch = back_space_char;
}
#endif
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
void SIO_ATC_SetDataMode (BOOLEAN mode)
{
#ifdef PPP_USB

	if (mode)
	{
	    user_port_mode_2 = DATA_MODE;
	}
	else
	{
	    user_port_mode_2 = ATC_MODE;
	}

	if (user_port_mode == MIX_MODE)
	{
		return;
	}

	//keep the COM_DATA on the ATC_MODE
	user_port_mode = ATC_MODE;

	SCI_PPP_TRACE_1("ppp:SIO_ATC_SetDataMode, mode1&2= %d&%d",user_port_mode,user_port_mode_2);


#else

    if (user_port_mode == MIX_MODE)
    {
        return;
    }

    if (mode)
    {
        user_port_mode = DATA_MODE;
    }
    else
    {
        user_port_mode = ATC_MODE;
    }
#endif
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
#ifdef PPP_USB
PUBLIC void SIO_ATC_SetEcho (BOOLEAN IsEcho,uint8 port_index)
{
    atc_is_echo[port_index] = IsEcho;
}
#else
PUBLIC void SIO_ATC_SetEcho (BOOLEAN IsEcho)
{
    atc_is_echo = IsEcho;
}
#endif

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
void SIO_ATC_SetCallback (ATC_CALLBACK callback)
{
    SCI_ASSERT (SCI_NULL != callback);/*assert verified*/

    AT_callback_ind = callback;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void ATC_CheckPPP()                                                   *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Callback to change the mode.                                          *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **---------------------------------------------------------------------------*/
#ifdef PPP_USB

LOCAL void ATC_CheckPPP_2 (uint32 wParam)
{
    uint32 cur_time = SCI_GetTickCount();

    if (MODE_CHANGE_DELAY < (cur_time - last_rx_time_2))
    {
        user_port_mode_2 = ATC_MODE;
    }
}

#endif

LOCAL void ATC_CheckPPP (uint32 wParam)
{
    uint32 cur_time = SCI_GetTickCount();

    if (MODE_CHANGE_DELAY < (cur_time - last_rx_time))
    {
        user_port_mode = ATC_MODE;
    }
}


PUBLIC void SIO_SaveToPPPBuf (char *p_data, uint32 len)
{
    uint i;
    SIO_INFO_S  *sio_ptr = (SIO_INFO_S *) &sio_port[COM_DATA];

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }

    for (i = 0; i < len; i++)
    {
        if (user_port_mode == ATC_MODE)
        {
            if (ring_add (&sio_ptr->atc_buf, * (p_data + i)) == -1)
            {
                continue;
            }
        }
        else if (ring_add (&sio_ptr->rx_buf, * (p_data + i)) == -1)
        {
            continue;
        }

        if (user_port_mode == ATC_MODE)
        {
            // Check if it is a plus char
            if (PLUS_CHAR != * (p_data + i))
            {
                plus_num = 0;
            }
            else
            {
                plus_num ++;

                if (plus_num == 3)
                {
                    user_port_mode = DATA_MODE;
                }
            }

            cur_atc_frame_size ++;

       #ifdef PPP_USB
        if ( (* (p_data + i) == atc_end_ch1[COM_DATA]) ||
                    (* (p_data + i) == atc_end_ch2[COM_DATA]))

	#else
        if ( (* (p_data + i) == atc_end_ch1) ||
                    (* (p_data + i) == atc_end_ch2))
        #endif
            {
                s_is_at_from_debug_port = TRUE;
                // OK, we should transmit this message to
                //@Zhemin.Lin, CR:MS00004031, 08/29/2003, begin
                //SCI_ASSERT(SCI_NULL != AT_callback_ind);
                //(*AT_callback_ind) (cur_atc_frame_size);
             #ifdef PPP_USB
                 DO_ATC_CALLBACK (cur_atc_frame_size,COM_DATA);
		#else
                 DO_ATC_CALLBACK (cur_atc_frame_size);
		#endif
                //@Zhemin.Lin, CR:MS00004031, 08/29/2003, end

                cur_atc_frame_size = 0;
				return;
            }
        }
    }

}

/*****************************************************************************/
// Description :    Register error handle, when sio link error,  driver will call this handle to deal
//                       the errors
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_RegisterErrorHandle (SIO_ERROR_HANDLE errorhandle)
{
    if (NULL != errorhandle)
    {
        link_error_handle = errorhandle;
        return SCI_SUCCESS;
    }

    return SCI_ERROR;

}

/*****************************************************************************/
// Description :    Register notify handle, under the mix mode, when sio drv receive a ready
//                       packet or busy packet or status packet, it will call the notify handle
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_RegisterNotifyHandle (SIO_NOTIFY_HANDLE notifyhandle)
{
    if (notifyhandle != NULL)
    {
        notify_handle = notifyhandle;
        return SCI_SUCCESS;
    }

    return SCI_ERROR;
}

/*****************************************************************************/
// Description :    Register data receive function
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_RegisterDataRecvHandle (ATC_CALLBACK recvhandle)
{
    if (recvhandle!= NULL)
    {
        data_recv_handle = recvhandle;
        return SCI_SUCCESS;
    }

    return SCI_ERROR;
}


/*****************************************************************************/
// Description :    under the MIX_MODE,  use this function to send data packet
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SendDataPacket (uint8 *res_data_ptr, uint32 length)
{
    uint32 i;
    uint8 checksum;
    int end_point;

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return SCI_ERROR;
    }

    if ( (res_data_ptr == NULL) || (length > DATA_PORT_SEND_BUF_SIZE))
    {
        return SCI_ERROR;
    }

    /* to see if the ringspace of tx buffer is enough to hold the data packet */
	#ifdef PPP_USB
        if (! uart_mix_check (length,COM_DATA))
	#else
        if (! uart_mix_check (length))
	#endif
	{
        //SCI_TRACE_LOW:"jim: in SIO_SendDataPacket : uart_mix_check \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4785_112_2_17_23_5_30_1087,(uint8*)"");
        return SCI_ERROR;
    	}

    if (! uart_mix_sem_take())
    {
        //SCI_TRACE_LOW:"jim: in SIO_SendDataPacket : uart_mix_sem_take \n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_4791_112_2_17_23_5_30_1088,(uint8*)"");
        return SCI_ERROR;
    }

  #ifdef PPP_USB
    end_point = uart_mix_get_end_point(COM_DATA);
  uart_mix_set_end_point (end_point,COM_DATA);
  #else
    end_point = uart_mix_get_end_point();
    uart_mix_set_end_point (end_point);
#endif

    if (MIX_MODE == user_port_mode)
    {
        //send packet head
        uart_ppp_putc (COM_DATA, SIO_DATA_PACKET);
        uart_ppp_putc (COM_DATA, 0x0FF & length);
        uart_ppp_putc (COM_DATA, SIO_DATA_PATTERN | ( (length>>8) & 7));

        checksum = SIO_DATA_PACKET + (0x0FF & length) + (SIO_DATA_PATTERN | ( (length>>8) & 7));

        //send packet body
        for (i = 0; i < length; i++)
        {
            checksum = checksum + (*res_data_ptr);
            uart_ppp_putc (COM_DATA, *res_data_ptr++);
        }

        //send checksum
        uart_ppp_putc (COM_DATA, checksum);
    }
    else
    {
        for (i = 0; i < length; i++)
        {
            uart_ppp_putc (COM_DATA, *res_data_ptr++);
        }
    }

    /*
     * rewirte the end_point of tx buffer,so the packet can
     * be send out now.
     */
   #ifdef PPP_USB
   uart_mix_set_end_point (0xFFFFFFFF,COM_DATA);
   #else
   uart_mix_set_end_point (0xFFFFFFFF);
   #endif
    SIO_Tx_Int_Enable (COM_DATA,1); // disable tx interrupt to avoid sending an half wrong packet.

    //give mutex
    uart_mix_sem_give();    //SIO_SEMGIVE;

    return SCI_SUCCESS;
}

/*****************************************************************************/
// Description :    under the MIX_MODE,  use this function to send status packet
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SendStatusPacket (uint8 status)
{
    uint8 checksum;
    int end_point;

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return SCI_ERROR;
    }

    if (MIX_MODE == user_port_mode)
    {
        /* to see if the ringspace of tx buffer is enough to hold the data packet */
	#ifdef PPP_USB
        if (! uart_mix_check (1,COM_DATA))
	#else
        if (! uart_mix_check (1))
	#endif
        {
            return SCI_ERROR;
        }


        if (! uart_mix_sem_take())
        {
            return SCI_ERROR;
        }

     #ifdef PPP_USB
        end_point = uart_mix_get_end_point(COM_DATA);
      uart_mix_set_end_point (end_point,COM_DATA);
     #else
         end_point = uart_mix_get_end_point();
      uart_mix_set_end_point (end_point);
     #endif
        //send packet head
        uart_ppp_putc (COM_DATA, SIO_DATA_PACKET);
        uart_ppp_putc (COM_DATA, 1);
        uart_ppp_putc (COM_DATA, SIO_STATUS_PATTERN);

        checksum = SIO_DATA_PACKET + 1 + SIO_DATA_PATTERN + status;

        //send packet body
        uart_ppp_putc (COM_DATA, status);
        //send checksum
        uart_ppp_putc (COM_DATA, checksum);


        /*
         * rewirte the end_point of tx buffer,so the packet can
         * be send out now.
         */
     #ifdef PPP_USB
      uart_mix_set_end_point (0xFFFFFFFF,COM_DATA);
     #else
       uart_mix_set_end_point (0xFFFFFFFF);
     #endif
        SIO_Tx_Int_Enable (COM_DATA,1); // disable tx interrupt to avoid sending an half wrong packet.
        //give mutex
        uart_mix_sem_give();    //SIO_SEMGIVE;
    }
    else
    {
        return SCI_ERROR;
    }

    return SCI_SUCCESS;
}

/*****************************************************************************/
// Description :    under the MIX_MODE,  use this function to send ready packet
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SendReadyPacket (void)
{
    uint8 checksum;
    int end_point;

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return SCI_ERROR;
    }

    if (MIX_MODE == user_port_mode)
    {
        /* to see if the ringspace of tx buffer is enough to hold the data packet */
        #ifdef PPP_USB
        if (! uart_mix_check (0,COM_DATA))
	#else
        if (! uart_mix_check (0))
	#endif
	{
            return SCI_ERROR;
        }


        if (! uart_mix_sem_take())
        {
            return SCI_ERROR;
        }

       #ifdef PPP_USB
       end_point = uart_mix_get_end_point(COM_DATA);
     	uart_mix_set_end_point (end_point,COM_DATA);
	#else
        end_point = uart_mix_get_end_point();
    	uart_mix_set_end_point (end_point);
	#endif
        //send packet head
        uart_ppp_putc (COM_DATA, SIO_DATA_PACKET);
        uart_ppp_putc (COM_DATA, 0);
        uart_ppp_putc (COM_DATA, SIO_READY_PATTERN);

        checksum = SIO_DATA_PACKET + 0 + SIO_READY_PATTERN;
        //send checksum
        uart_ppp_putc (COM_DATA, checksum);

        /*
         * rewirte the end_point of tx buffer,so the packet can
         * be send out now.
         */
       #ifdef PPP_USB
      uart_mix_set_end_point (0xFFFFFFFF,COM_DATA);
	#else
      uart_mix_set_end_point (0xFFFFFFFF);
	#endif
        SIO_Tx_Int_Enable (COM_DATA,1); // disable tx interrupt to avoid sending an half wrong packet.
        //give mutex
        uart_mix_sem_give();    //SIO_SEMGIVE;

    }
    else
    {
        return SCI_ERROR;
    }

    return SCI_SUCCESS;
}

/*****************************************************************************/
// Description :    under the MIX_MODE,  use this function to send busy packet
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC uint32 SIO_SendBusyPacket (void)
{
    uint8 checksum;
    int end_point;

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return SCI_ERROR;
    }

    if (MIX_MODE == user_port_mode)
    {
        /* to see if the ringspace of tx buffer is enough to hold the data packet */
 	#ifdef PPP_USB
        if (! uart_mix_check (0,COM_DATA))
	#else
        if (! uart_mix_check (0))
	#endif
	{
            return SCI_ERROR;
        }


        if (! uart_mix_sem_take())
        {
            return SCI_ERROR;
        }
 	#ifdef PPP_USB
        end_point = uart_mix_get_end_point(COM_DATA);
        uart_mix_set_end_point (end_point,COM_DATA);

	#else
        end_point = uart_mix_get_end_point();
        uart_mix_set_end_point (end_point);
	#endif
        //send packet head
        uart_ppp_putc (COM_DATA, SIO_DATA_PACKET);
        uart_ppp_putc (COM_DATA, 0);
        uart_ppp_putc (COM_DATA, SIO_BUSY_PATTERN);

        checksum = SIO_DATA_PACKET + 1 + SIO_BUSY_PATTERN;
        //send checksum
        uart_ppp_putc (COM_DATA, checksum);

        /*
         * rewirte the end_point of tx buffer,so the packet can
         * be send out now.
         */
       #ifdef PPP_USB
      uart_mix_set_end_point (0xFFFFFFFF,COM_DATA);
	#else
      uart_mix_set_end_point (0xFFFFFFFF);
	#endif
        SIO_Tx_Int_Enable (COM_DATA,1); // disable tx interrupt to avoid sending an half wrong packet.
        //give mutex
        uart_mix_sem_give();    //SIO_SEMGIVE;

    }
    else
    {
        return SCI_ERROR;
    }

    return SCI_SUCCESS;
}
/*****************************************************************************/
// Description :    this function is used to handle recv data from SIO, when current mode is MIX_MODE
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
LOCAL void SIO_MixRecv (SIO_INFO_S *sio_ptr,uint8 *src_buf,uint32 rec_num)
{
    uint8      *ptr         = (uint8 *) & (mix_mode_info.packet);
    SIO_BUF_S *sio_buf_ptr = NULL;
    uint32 i;
    uint8 nchar;

    while (rec_num)
    {
        nchar = *src_buf++;

        //decrease available data number
        rec_num--;

        //fill packet head
        if (mix_mode_info.recvlen < MIX_PACKET_HEAD_LENGHT)
        {
            if ( (SIO_DATA_PACKET != nchar)
                    && (SIO_ATC_PACKET != nchar)
                    && (0 == mix_mode_info.recvlen))
            {

                continue;
            }

            ptr[mix_mode_info.recvlen] = nchar;
            mix_mode_info.recvlen++;
            mix_mode_info.checksum += nchar;

            if (mix_mode_info.recvlen == MIX_PACKET_HEAD_LENGHT)
            {
                mix_mode_info.datalen = mix_mode_info.packet.length_lsb + ( (mix_mode_info.packet.subtype&7) <<8);
                //SCI_TRACE_LOW:"sio:datalen = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5054_112_2_17_23_5_31_1089,(uint8*)"d",mix_mode_info.datalen);

                //@Shijun.cui 2005-05-18 add for stronger in mix mode
                if (mix_mode_info.datalen > mix_mode_info.data_rx_buf.size)
                {
                    RESET_MIX_MODE_INFO;
                    //SCI_TRACE_LOW:"sio:reset mix 1."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5060_112_2_17_23_5_31_1090,(uint8*)"");
                }
                else if (SIO_DATA_PACKET == mix_mode_info.packet.type)
                {
                    if ( (mix_mode_info.packet.subtype & 0xF8) >
                            (SIO_DATA_PATTERN |
                             SIO_STATUS_PATTERN |
                             SIO_READY_PATTERN |
                             SIO_BUSY_PATTERN))
                    {
                        RESET_MIX_MODE_INFO;
                        //SCI_TRACE_LOW:"sio:reset mix 2."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5071_112_2_17_23_5_31_1091,(uint8*)"");
                    }
                }
                else if (SIO_ATC_PACKET == mix_mode_info.packet.type)
                {
                    if (SIO_ATC_PATTERN != (mix_mode_info.packet.subtype & 0xF8))
                    {
                        RESET_MIX_MODE_INFO;
                        //SCI_TRACE_LOW:"sio:reset mix 3."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5079_112_2_17_23_5_31_1092,(uint8*)"");
                    }
                }
            }

            continue;
        }

        if (SIO_DATA_PACKET == mix_mode_info.packet.type)
        {
            sio_buf_ptr = & (mix_mode_info.data_rx_buf);
        }
        else if (SIO_ATC_PACKET == mix_mode_info.packet.type)
        {
            sio_buf_ptr = & (sio_ptr->atc_buf);
        }
        else
        {
            //SCI_TRACE_LOW:"mix mode data type error."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5097_112_2_17_23_5_31_1093,(uint8*)"");
            //hyman, assert case
            SCI_ASSERT (0);/*assert verified*/
        }

        //current recev is checksum
        if ( (mix_mode_info.recvlen - MIX_PACKET_HEAD_LENGHT) == mix_mode_info.datalen)
        {
            if (nchar != mix_mode_info.checksum)
            {
                //call error handle
                DO_ERROR_HANDLE (COM_DATA, SIO_CHECKSUM_ERROR);
            }
            else
            {
                if (SIO_DATA_PACKET == mix_mode_info.packet.type) //DATA packet
                {
                    if (mix_mode_info.packet.subtype == SIO_READY_PATTERN)
                    {
                        DO_NOTIFY_HANDLE (SIO_NOTIFY_READY, 0);
                    }
                    else if (mix_mode_info.packet.subtype == SIO_BUSY_PATTERN)
                    {
                        DO_NOTIFY_HANDLE (SIO_NOTIFY_BUSY, 0);
                    }
                    else if (mix_mode_info.packet.subtype == SIO_STATUS_PATTERN)
                    {
                        DO_NOTIFY_HANDLE (SIO_NOTIFY_STATUS, mix_mode_info.sio_temp_buf[0]);
                    }
                    else //data frame
                    {
                        uint32 copy_buf_size = 0;
                        uint32 copied_size = 0;
                        SIO_BUF_S *pRx_buf = &sio_port[COM_DATA].rx_buf;

                        /*Modified by Shijun.Cui 2005-01-21 CR18757, avoiding longer time in UART IRQ*/
                        copy_buf_size = ring_memcpy_space (pRx_buf);

                        if (mix_mode_info.datalen > copy_buf_size)
                        {
                            if (copy_buf_size)
                            {
                                SCI_MEMCPY (&pRx_buf->sio_buf_ptr[pRx_buf->end_point], &mix_mode_info.sio_temp_buf[0], copy_buf_size);
                                ring_end_point_move (pRx_buf, copy_buf_size);
                                copied_size = copy_buf_size;
                            }

                            copy_buf_size = ring_memcpy_space (pRx_buf);

                            if (copy_buf_size)
                            {
                                uint32 to_copy_len;
                                to_copy_len = (mix_mode_info.datalen - copied_size) > copy_buf_size ? copy_buf_size : mix_mode_info.datalen - copied_size;
                                SCI_MEMCPY (&pRx_buf->sio_buf_ptr[pRx_buf->end_point], &mix_mode_info.sio_temp_buf[copied_size], to_copy_len);
                                ring_end_point_move (pRx_buf, to_copy_len);
                                copied_size += to_copy_len;
                            }

                            if (copied_size < mix_mode_info.datalen)
                            {
                                //SCI_TRACE_LOW:"MIX SIO: buffer full!"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5157_112_2_17_23_5_31_1094,(uint8*)"");
                            }
                        }
                        else
                        {

                            SCI_MEMCPY (&pRx_buf->sio_buf_ptr[pRx_buf->end_point], &mix_mode_info.sio_temp_buf[0], mix_mode_info.datalen);
                            ring_end_point_move (pRx_buf, mix_mode_info.datalen);

                        }

                        //end CR18757

                        //DO_DATA_CALLBACK(mix_mode_info.datalen);
                    }
                }
                else if (SIO_ATC_PACKET == mix_mode_info.packet.type)    //ATC packet
                {
                    /*@Jim.zhang CR:MS9076 2004-05-13 */

                    uint32 datalength = 0;          //the data length of integrated AT command in one package.
                    uint32 newatstart = 0;          //a new AT command start index(the length of all last at in the package)
                    static uint32 lastlength = 0;   //the data length of last unintegrated AT command.

                    uint32 copy_buf_size = 0;
                    uint32 copied_size = 0;
                    uint32 ring_space_size = 0;

                    /////////////////////////////////////////////////////////////
                    //Jason.cui 2005-01-25
                    ring_space_size = ring_space (sio_buf_ptr);

                    if (ring_space_size >= mix_mode_info.datalen)
                    {
                        copy_buf_size = ring_memcpy_space (sio_buf_ptr);

                        if (mix_mode_info.datalen > copy_buf_size)
                        {
                            SCI_MEMCPY (&sio_buf_ptr->sio_buf_ptr[sio_buf_ptr->end_point], &mix_mode_info.sio_temp_buf[0], copy_buf_size);
                            copied_size = copy_buf_size;
                            ring_end_point_move (sio_buf_ptr, copied_size);

                            //@Shijun.cui 2005-04-13
                            copy_buf_size = ring_memcpy_space (sio_buf_ptr);

                            if (mix_mode_info.datalen - copied_size <= copy_buf_size)
                            {
                                SCI_MEMCPY (&sio_buf_ptr->sio_buf_ptr[sio_buf_ptr->end_point], &mix_mode_info.sio_temp_buf[copied_size], mix_mode_info.datalen - copied_size);
                                ring_end_point_move (sio_buf_ptr, mix_mode_info.datalen - copied_size);

                            }
                            else
                            {
                                //will not go here
                                SCI_ASSERT (0);/*assert verified*/
                                //sio_buf_ptr->sio_buf_ptr[sio_buf_ptr->end_point] = atc_end_ch1; //avoid AT out of order!
                                //SCI_TRACE_LOW("SIO: discard AT command");//no buf here, discard AT COMMAND
                            }

                            //end

                        }
                        else
                        {
                            SCI_MEMCPY (&sio_buf_ptr->sio_buf_ptr[sio_buf_ptr->end_point], &mix_mode_info.sio_temp_buf[0], mix_mode_info.datalen);
                            ring_end_point_move (sio_buf_ptr, mix_mode_info.datalen);
                        }


                        for (i=0; i<mix_mode_info.datalen; i++)
                        {
                            nchar = mix_mode_info.sio_temp_buf[i];

				#ifdef PPP_USB
                            if (nchar == atc_end_ch1[COM_DATA] || nchar == atc_end_ch2[COM_DATA])
				#else
                            if (nchar == atc_end_ch1 || nchar == atc_end_ch2)
				#endif
                            {
                                if (lastlength)      //there is unintegrated AT command in last pacakge.
                                {
                                    datalength = i + 1 + lastlength; //add last unintegrated at command
                                    lastlength = 0;
                                }
                                else                //last pacakge is a integrated AT package.(have terminate char)
                                {
                                    datalength = i + 1 - newatstart;
                                }

                                s_is_at_from_debug_port= FALSE;
  				    #ifdef PPP_USB
                                DO_ATC_CALLBACK (datalength,COM_DATA);    // send message to upper layer.
				    #else
                                DO_ATC_CALLBACK (datalength);    // send message to upper layer.
                                #endif

                                newatstart = i + 1;
                            }
                        }

                        //the current package is an unintegrated at package.
                        #ifdef PPP_USB
			   if ( (nchar != atc_end_ch1[COM_DATA]) && (nchar != atc_end_ch2[COM_DATA]))
			   #else
			   if ( (nchar != atc_end_ch1) && (nchar != atc_end_ch2))
			   #endif
                        {
                            lastlength += mix_mode_info.datalen - newatstart;
                        }

                        /* end CR:MS9076 */
                    }
                    else
                    {
                        //Jason.cui, 2005-01-25 no enough memory to hold AT packet
                        //discard all the pending AT packet!
                        sio_buf_ptr->end_point = sio_buf_ptr->start_point;
                        //SCI_TRACE_LOW:"SIO: discard the pending AT packets."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5262_112_2_17_23_5_31_1095,(uint8*)"");
                    }
                }
            }

            //reset mix_mode_info
            RESET_MIX_MODE_INFO;

        }
        else  //recv databody of a packet
        {
            SCI_ASSERT (mix_mode_info.buf_used < mix_mode_info.data_rx_buf.size);/*assert verified*/
            mix_mode_info.sio_temp_buf[mix_mode_info.buf_used] = nchar;
            mix_mode_info.buf_used++;
            mix_mode_info.recvlen++;
            mix_mode_info.checksum += nchar;
        }

    }
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
                RESET_MIX_MODE_INFO;
		#ifdef PPP_USB
                SIO_ATC_ClrCmdLine(COM_DATA);//guangqiao.she,2019-2-27 Added.
                 SIO_ATC_ClrCmdLine(COM_DATA2);//guangqiao.she,2019-2-27 Added.
		#else
                SIO_ATC_ClrCmdLine();//Shijun.cui 2005-02-02 Added.
                #endif
                //there similarly may be some data in AT BUF while reboot after module assert
                //Should clear it!
                SCI_RestoreIRQ();
            }

            user_port_mode = mode;
            break;
        case DATA_MODE:
            //SCI_TRACE_LOW:"SIO SIO_SetUserPortMode error, current mode is DATA_MODE\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5323_112_2_17_23_5_31_1096,(uint8*)"");
            return SCI_ERROR;
        default:
            return SCI_ERROR;
    }

    return SCI_SUCCESS;
#endif
}

/*****************************************************************************/
// Description :    read a data packet body from recev ring buffer
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
LOCAL void SIO_ReadDataPacket (uint8 *data_ptr, uint32 length, uint32 *read_length_ptr)
{
    uint i;
    uint8 ch;

    //maybe not need here
    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return;
    }

    for (i = 0; i < length; i++)
    {
        ch = ring_remove (& (mix_mode_info.data_rx_buf));

        if ( (int8) ch != -1)
        {
            *data_ptr++ = ch;
            *read_length_ptr += 1;
        }
        else
        {
            break;
        }
    }
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
// Description :    get phy port no from logical port
// Global resource dependence :
// Author :        weihua.wang
// Note :          uint32 port: COM_DEUBG/COM_DATA
/*****************************************************************************/
PUBLIC uint32 SIO_GetPhyPortNo (uint32 port)
{
    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    return sio_port[port].phy_port;
}

PUBLIC void SIO_DumpSetPhyPortNo (uint32 port,uint32 phy_port)
{
    if(SCI_GetAssertFlag()){
        //sio_port[port].phy_port = phy_port;
        sio_port[port].open_flag = 0;
    }

}

/*****************************************************************************/
// Description :    read a char from data receive buffer, the port must be COM_DATA,
//                       and current mode is MIX_MODE, or DATA_MODE.
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC  int   SIO_GetChar (uint32 port)
{
    SIO_BUF_S *pbuf;

    if (COM_DATA  != port)
    {
        return (-1);
    }

    if (sio_port[COM_DATA].open_flag != SIO_ALREADY_OPEN)
    {
        return (-1);
    }

    if (MIX_MODE == user_port_mode)
    {
        pbuf = & (mix_mode_info.data_rx_buf);
        return (ring_remove (pbuf));
    }
    else if (DATA_MODE == user_port_mode)
    {
        pbuf = & (sio_port[COM_DATA].rx_buf);
        return (ring_remove (pbuf));
    }

    return (-1);
}

/*****************************************************************************/
// Description :    put a char to data send buffer, the port must be COM_DATA,
//                       and current mode is MIX_MODE, or DATA_MODE.
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
PUBLIC  int   SIO_PutChar (uint32 port , uint8 nchar)
{
    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return SCI_ERROR;
    }

    if ( (MIX_MODE == user_port_mode) && (COM_DATA == port))
    {
        if (SCI_SUCCESS == SIO_SendDataPacket (&nchar, 1))
        {
            return 0;
        }
        else
        {
            return (-1);
        }
    }

    uart_ppp_putc (port, nchar);

    /*
     * rewirte the end_point of tx buffer,so the packet can
     * be send out now.
     */
       #ifdef PPP_USB
      uart_mix_set_end_point (0xFFFFFFFF,port);
	#else
      uart_mix_set_end_point (0xFFFFFFFF);
	#endif
    return 0;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     int SIO_DumpGetChar()                                                 *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function scanf a char from uart receive register directly.       *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Returns a char from uart receive register.                            *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC int SIO_DumpGetChar (uint32 port)
{
    int32 nchar;
    uint32 phy_port;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (SIO_ALREADY_OPEN != sio_port[port].open_flag)
    {
        //return directly
        return -1;
    }

    phy_port = sio_port[port].phy_port;

    //phy port is valid
    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        return -1;
    }

    //get char from fifo
    nchar = (sio_port[port].sio_op.get_char) (SIO_PhyPortfromIndex (phy_port));

    return ( (int) nchar);

}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_DumpPutChar()                                                *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function put a char to the uart send register directly.          *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC void SIO_DumpPutChar (uint32 port, char nchar)
{
    uint32 phy_port;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (SIO_ALREADY_OPEN != sio_port[port].open_flag)
    {
        return ;
    }

    phy_port = sio_port[port].phy_port;

    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        return ;
    }

    //put char to fifo
    (sio_port[port].sio_op.put_char) (SIO_PhyPortfromIndex (phy_port),nchar);

}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_DumpPutChars()                                               *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function put chars to the destion directly.                      *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None now called in memdump only to reduce time cost                   *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC void SIO_DumpPutChars (uint32 port, uint8* data_ptr, int32 len)
{
    uint32 phy_port;
    int32  i;
    uint8  curval;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (SIO_ALREADY_OPEN != sio_port[port].open_flag)
    {
        return ;
    }

    phy_port = sio_port[port].phy_port;

    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        return ;
    }

    //put char to fifo
    if(sio_port[port].sio_op.put_chars != NULL)
    {
        (sio_port[port].sio_op.put_chars) (SIO_PhyPortfromIndex (phy_port),data_ptr, len);
    }
    else
    {
        for(i = 0; i < len; i++)
        {
            curval = *data_ptr++;
            if ((0x7E == curval) || (0x7D == curval))
            {
                (sio_port[port].sio_op.put_char) (SIO_PhyPortfromIndex (phy_port),0x7d);
                (sio_port[port].sio_op.put_char) (SIO_PhyPortfromIndex (phy_port),(curval ^ 0x20));
            }
            else
            {
                (sio_port[port].sio_op.put_char) (SIO_PhyPortfromIndex (phy_port),curval);
            }
        }
    }

}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_DumpPutCharEnd()                                             *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     This function indicated that send string is finished                  *
 **     all data should be sent out                                           *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     None                                                                  *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     None                                                                  *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC void SIO_DumpPutCharEnd (uint32 port)
{
    uint32 phy_port;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (SIO_ALREADY_OPEN != sio_port[port].open_flag)
    {
        return ;
    }

    phy_port = sio_port[port].phy_port;

    //phy port is valid
    if (phy_port >= MAX_SIO_PORT_NUM)
    {
        return ;
    }

    //put char to fifo
    (sio_port[port].sio_op.flush) (SIO_PhyPortfromIndex (phy_port));
}

/*****************************************************************************/
// Description :    default handle for data receive, just discarding this frame
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
LOCAL void DefaultDataCallback (uint32 len)
{
    uint32 i;

    //discard this frame
    for (i = 0; i < len; i++)
    {
        ring_remove (& (mix_mode_info.data_rx_buf));
    }

}

//@Zhemin.Lin, CR:MS00004213, 2003/08/22, end

/*****************************************************************************/
// Description :    default handle for AT receive, just discarding this frame
//                  DefaultAtCallback function is NOT adapt for this operation
// Global resource dependence :
// Author :         Shijun.Cui 2005-02-02
// Note :
/*****************************************************************************/
#ifdef PPP_USB
LOCAL void DefaultAtHandle (uint32 len,uint8 port_index)
{
    uint32 i;
    SIO_INFO_S  *sio_ptr = (SIO_INFO_S *) &sio_port[port_index];

    //discard this frame
    for (i = 0; i < len; i++)
    {
        ring_remove (& (sio_ptr->atc_buf));
    }

}
#else
LOCAL void DefaultAtHandle (uint32 len)
{
    uint32 i;
    SIO_INFO_S  *sio_ptr = (SIO_INFO_S *) &sio_port[COM_DATA];

    //discard this frame
    for (i = 0; i < len; i++)
    {
        ring_remove (& (sio_ptr->atc_buf));
    }

}
#endif

//@Zhemin.Lin, CR:MS00004031, 08/28/2003, begin

/*****************************************************************************/
// Description :    default handle for AT receive, just discarding this frame
// Global resource dependence :
// Author :         Zhemin.Lin
// Note :
/*****************************************************************************/
LOCAL void DefaultAtCallback (uint32 len)
{
    uint32 i;
    SIO_INFO_S  *sio_ptr = (SIO_INFO_S *) &sio_port[COM_DATA];

    //discard this frame
    for (i = 0; i < len; i++)
    {
        ring_remove (& (sio_ptr->rx_buf));
    }

}


void SIO_SendRemainLogMessage (void)
{
#ifdef TRACE_INFO_SUPPORT
    uint32 phy_port ;

    uint32 t = 0;
    DATA_FRAME_T *frame_ptr = NULL;
    uint16 tail;
    uint16 count;


    if (SIO_ALREADY_OPEN != sio_port[COM_DEBUG].open_flag)
    {
        return ;
    }
    tail = s_sio_frame_ctrl.tail;

    phy_port = SIO_PhyPortfromIndex (sio_port[COM_DEBUG].phy_port);

    frame_ptr = &s_sio_frame_ctrl.frame_list[s_sio_frame_ctrl.head];

    // frame_ptr->addr[frame_ptr->pos++] = FLAG_BYTE;
    SCI_MEMSET ( ( (uint8 *) &frame_ptr->addr[frame_ptr->pos]), FLAG_BYTE, (frame_ptr->length - frame_ptr->pos));
    s_sio_frame_ctrl.count++;
    count = s_sio_frame_ctrl.count;
    /* tx fifo is empty, so try send char  */
    while (count)
    {

        frame_ptr = &s_sio_frame_ctrl.frame_list[tail];

        for (t = 0; t < frame_ptr->length; t++)
        {
            /* write to com drv_fifo */
            (sio_port[COM_DEBUG].sio_op.put_char) (phy_port,frame_ptr->addr[t]);
        }

        tail++;

        if (tail >= s_sio_frame_ctrl.list_size)
        {
            tail = 0;
        }
        count--;
    }

    (sio_port[COM_DEBUG].sio_op.flush) (phy_port);
#endif
}
//@Zhemin.Lin, CR:MS00004031, 08/28/2003, end

/*@jim.zhang CR:MS00008486 04/06/2004 */

/*****************************************************************************/
// Description :   get the mix used state for GPRS.
// Global resource dependence :
// Author :        Jim.zhang
// Note : return 1 means PPP and Log are mixed, 0 means not mixed.
/*****************************************************************************/
PUBLIC BOOLEAN SIO_IsPPPMixedLogel (void)
{
    return gprs_debug;
}

/* end CR:MS00008486 */


/*****************************************************************************/
// Description :   check the DSR and set the DTR to enable/disable deep sleep
// Global resource dependence :
// Author :        Jim.zhang
// Note :          If DSR = 1: Disable deep sleep, DTR = 1
//                 If DSR = 0, Enable deep sleep,  DTR = 0
/*****************************************************************************/
PUBLIC void SIO_CheckDeepSleepFlag (void)
{

#ifdef _SIO_WAKEUP_ARM

    uint32 data_port  = sio_port[COM_DEBUG].phy_port;
    uint32 debug_port = sio_port[COM_DATA].phy_port;

    if ( (debug_port == COM0) || (data_port == COM0))
    {
        UART_CheckDeepSleepFlag();
    }

#endif  // _SIO_WAKEUP_ARM

}

/*****************************************************************************/
// Description :   Use the given baudrate to init the uart register.
//                 and can receivce/send data via it.
// Global resource dependence :
// Author :        Lin.liu
// Input:
//        port        : uart port number, UART_COM0,1
//        baudrate : the baudrate need to be set( 115200, 9600 etc ).
// Return:
//        None
// Note :
/*****************************************************************************/
PUBLIC void SIO_InitUartLowLevel (uint32 port,  uint32 baudrate)
{
    UART_InitUartLowLevel (port, baudrate);
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

/*****************************************************************************/
//  Description:    This function used to get AT uart port mix mode
//  Author:
//  Note:
//  Return:    FALSE - mix mode disable
//             TRUE  - mix mode enable
/*****************************************************************************/
LOCAL BOOLEAN SIO_ATC_GetMixMode (
    void
)
{
    return gprs_debug;
}

/*****************************************************************************/
// Description :   get com_debug/com_data rx buffer max size;
// Global resource dependence :
// Author :
// Input:     COM_Port No.
// Return:
//        COM_DEBUG/Data rx buffer max size
// Note :
/*****************************************************************************/
PUBLIC int32 SIO_GetRxBufMaxSize (uint32 port)
{
    int32 buf_size = 0;

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/

    if (COM_DEBUG == port)
    {
        buf_size = DEBUG_PORT_REC_BUF_SIZE;
    }
    else
    {
        buf_size = DATA_PORT_REC_BUF_SIZE;
    }

    return buf_size;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 SIO_RxForDebugPort (uint8 *data_buf,uint32 rec_num)
{
    //out endpoint data handle
    uint32   i;
    uint32   free_data_size;
    uint32   copy_data_size;
    uint32   start_point = (&sio_port[COM_DEBUG].rx_buf)->start_point;
    uint32   buf_size = (&sio_port[COM_DEBUG].rx_buf)->size;
    uint8 *rx_buf_ptr = (&sio_port[COM_DEBUG].rx_buf)->sio_buf_ptr;

    /* Get Current rx data buffer size */
    GET_BUF_FREE_SIZE (free_data_size, (&sio_port[COM_DEBUG].rx_buf))

    copy_data_size = free_data_size < rec_num ? free_data_size : rec_num;

    for (i = 0; i < copy_data_size; i++)
    {
        rx_buf_ptr[start_point++] = * (data_buf++);
        start_point %= buf_size;
    }

    (&sio_port[COM_DEBUG].rx_buf)->start_point = start_point;

    if (0 < copy_data_size)
    {
        uint32 status_E;

        status_E = SCI_SetEvent (comm_event, (COM_DEBUG << 1), SCI_OR);

        //endif
    }
    return copy_data_size;
    /****************************************************
    // if there is some more char which can not be put in
    // we have to ingnore
    *****************************************************/
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
#ifdef PPP_USB
LOCAL uint32 SIO_RxForUserPort (uint8 *data_buf,uint32 rec_num,uint8 port_index)
{
    unsigned char  nchar;
    SIO_INFO_S  *sio_ptr = &sio_port[port_index];
    uint32 receive_len =rec_num;

   //SCI_PPP_TRACE("ppp:SIO_RxForUserPort,port%d",port_index);

    	if(COM_DATA2 == port_index)
	{
         if( !SIO_CheckIsAtcData(data_buf,rec_num) )
         {
        	SCI_TRACE_LOW("sio: SIO_RxForUserPort: discard ppp_data in atc_mode");
        	return 0;
         }

	    {
	        {
	            /*
	                rx fifo full or timeout,
	                try read char from device
	            */
	            while (rec_num)
	            {
	                nchar = *data_buf++;
	                rec_num--;

#if 0
#ifdef _U0_SOFTWARE_FLOW_CONTROL

	                if (nchar == XON)
	                {
	                    gRemoteXon_on = 1;
	                }
	                else if (nchar == XOFF)
	                {
	                    gRemoteXon_on = 0;
	                }

#endif
#endif
	                if (user_port_mode_2 == ATC_MODE)
	                {
	                    if (ring_add (&sio_port[port_index].atc_buf, nchar) == -1)
	                    {
	                        //Hyman.wu
	                        //too long at command,it is should not valid at commands!
	                        //so just clear buffer,else will lead to overrun
	                        sio_port[port_index].atc_buf.end_point = sio_port[port_index].atc_buf.start_point;
	                        continue;
	                    }
	                }
	                else if (ring_add (&sio_port[port_index].rx_buf, nchar) == -1)
	                {
	                    //Hyman.wu,2005-1-24
	                    //too long data will lead to overrun and deep sleep assert
	                    //so discard it,this is the bad flow control but not assert
	                    //sio_port[COM_DATA].rx_buf.end_point = sio_port[COM_DATA].rx_buf.start_point;
	                    //continue;
	                    receive_len = receive_len-rec_num-1;                       //don't lost one char

	                    SIO_Rx_Int_Enable (port_index,0); // Disable Out_endp interrupt of usb

	                    SCI_ChangeTimer (Space_timer_2, Space_Check_2, SPACE_CHECK_DELAY);/*lint !e64 */
	                    SCI_ActiveTimer(Space_timer_2);
	                    break;
	                }

	                if (user_port_mode_2 == ATC_MODE)
	                {
	                    cur_atc_frame_size_2++;

	                    if (atc_is_echo[port_index])
	                    {
	                        if (1) //(uart_mix_sem_take())
	                        {
	                            if (ring_space (& (sio_port[port_index].tx_buf)))
	                            {
	                                // Echo back to the hyper teriminal
	                                uart_ppp_putc (port_index, nchar);

	                                //enable usb inpoint int
	                                SIO_Tx_Int_Enable (port_index,TRUE);

	                            }

	                            /*
	                             * rewirte the end_point of tx buffer,so the packet can
	                             * be send out now.
	                             */
	                            uart_mix_set_end_point (0xFFFFFFFF,port_index);
	                            //uart_mix_sem_give();
	                        }
	                    }

	                    if (nchar == atc_end_ch1[port_index] || nchar == atc_end_ch2[port_index])
	                    {
	                        s_is_at_from_data2_port = SCI_TRUE;
	                        // OK, we should transmit this message to
#ifdef HTW_INTEGRATION
	                        //
	                        //Have a unstable here beacuse "cur_atc_frame_size" is a local variable,
	                        //other task can change it at any time .
	                        SetLen (cur_atc_frame_size);
	                        NU_Activate_HISR (&RX_FULL_ISR_USER);
#else
	                        DO_ATC_CALLBACK (cur_atc_frame_size_2,port_index);
#endif

	                        cur_atc_frame_size_2 = 0;
	                    }
	                }
	            }
	        }

	        // Get the time now!
	        last_rx_time_2 = SCI_GetTickCount();
	    }
	   SCI_PPP_TRACE_1("ppp:SIO_RxForUserPort,len%d,rxbuff:start %d,end %d,32K 0x%x",receive_len,sio_port[port_index].rx_buf.start_point,sio_port[port_index].rx_buf.end_point,SLEEP_GetCur32kNum());
	  return receive_len;
	}

#if 1 //defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW)|| defined(CHIP_VER_UIS8910A)
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
    //SCI_TRACE_LOW("SIO_RxForUserPort");
    if(UART_GetControllerBqbMode())
    {
        UART_Controller_Bqb_Write(data_buf, rec_num);
        return receive_len;
    }
#endif
#endif

    if (MIX_MODE == user_port_mode)
    {
#ifdef _U0_FLOW_CONTROL

        if (ring_space (& (sio_port[COM_DATA].rx_buf)) <= EMPTY_SPACE_WATER_LOW_MARK)
        {
            SIO_Rx_Int_Enable (COM_DATA,0); // Disable Out_endp interrupt of usb
        }

#endif

        SIO_MixRecv (sio_ptr,data_buf,rec_num);
        return receive_len;
    }

    {
        uint32      cur_rx_time = SCI_GetTickCount();

#ifdef _U0_SOFTWARE_FLOW_CONTROL

        if ( (1 == gXon_on) && (ring_space (& (sio_port[COM_DATA].rx_buf)) <= DATA_PORT_REC_BUF_SIZE*3/4))
        {
            //SCI_TRACE_LOW:"Software flow control:send XOFF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5910_112_2_17_23_5_32_1097,(uint8*)"");
            gXon_on = 0;
            //add code here to send XOFF *******
        }

#endif

        // If it is possible that we got three '+'
        if ( (MAX_PLUS_NUMBER >= rec_num)
                && (MODE_CHANGE_DELAY < (cur_rx_time - last_rx_time))
                && DATA_MODE == user_port_mode)
        {
            uint32 i;
            uint8 plus_char[MAX_PLUS_NUMBER];
            uint32 plus_num_temp = MAX_PLUS_NUMBER;

            // Fisrt get the char from FIFO
            for (i = 0; i < MAX_PLUS_NUMBER; i++)
            {
                plus_char[i] = *data_buf++;

#ifdef _U0_SOFTWARE_FLOW_CONTROL

                if (plus_char[i] == XON)
                {
                    gRemoteXon_on = 1;
                }
                else if (plus_char[i] == XOFF)
                {
                    gRemoteXon_on = 0;
                }

#endif

                // Check if it is a plus char
                if (PLUS_CHAR != plus_char[i])
                {
                    plus_num_temp = 0;
                }
            }

            if (MAX_PLUS_NUMBER == plus_num_temp)
            {
                // Set Timer to check if we received a mode change command.
                SCI_DeactiveTimer (ATC_timer);
                SCI_ChangeTimer (ATC_timer, ATC_CheckPPP, MODE_CHANGE_DELAY + 100); // Add 100 to make the timer response longer, so that the timeout logic will be OK.
                SCI_ActiveTimer (ATC_timer);
            }
            else
            {
                // No, so we have to put the data back to sio buffer
                for (i = 0; i < MAX_PLUS_NUMBER; i++)
                {
                    if (ring_add (&sio_port[COM_DATA].rx_buf, plus_char[i]) == -1)
                    {
                        continue;
                    }
                }
            }
        }
        else
        {
            /*
                rx fifo full or timeout,
                try read char from device
            */
            while (rec_num)
            {
                nchar = *data_buf++;
                rec_num--;

#ifdef _U0_SOFTWARE_FLOW_CONTROL

                if (nchar == XON)
                {
                    gRemoteXon_on = 1;
                }
                else if (nchar == XOFF)
                {
                    gRemoteXon_on = 0;
                }

#endif

                if (user_port_mode == ATC_MODE)
                {
                    if (ring_add (&sio_port[COM_DATA].atc_buf, nchar) == -1)
                    {
                        //Hyman.wu
                        //too long at command,it is should not valid at commands!
                        //so just clear buffer,else will lead to overrun
                        sio_port[COM_DATA].atc_buf.end_point = sio_port[COM_DATA].atc_buf.start_point;
                        continue;
                    }
                }
                else if (ring_add (&sio_port[COM_DATA].rx_buf, nchar) == -1)
                {
                    //Hyman.wu,2005-1-24
                    //too long data will lead to overrun and deep sleep assert
                    //so discard it,this is the bad flow control but not assert
                    //sio_port[COM_DATA].rx_buf.end_point = sio_port[COM_DATA].rx_buf.start_point;
                    //continue;
                    receive_len = receive_len-rec_num-1;                       //don't lost one char
                    SIO_Rx_Int_Enable (COM_DATA,0); // Disable Out_endp interrupt of usb

                    SCI_ChangeTimer (Space_timer, Space_Check, SPACE_CHECK_DELAY);/*lint !e64 */
                    SCI_ActiveTimer(Space_timer);
                    break;
                }

                if (user_port_mode == ATC_MODE)
                {
                    cur_atc_frame_size++;

                    if (atc_is_echo[port_index])
                    {
                        if (uart_mix_sem_take())
                        {
                            if (ring_space (& (sio_port[COM_DATA].tx_buf)))
                            {
                                // Echo back to the hyper teriminal
                                uart_ppp_putc (COM_DATA, nchar);

                                //enable usb inpoint int
                                SIO_Tx_Int_Enable (COM_DATA,TRUE);

                            }

                            /*
                             * rewirte the end_point of tx buffer,so the packet can
                             * be send out now.
                             */
                            uart_mix_set_end_point (0xFFFFFFFF,COM_DATA);
                            uart_mix_sem_give();
                        }
                    }

                    if (nchar == atc_end_ch1[COM_DATA] || nchar == atc_end_ch2[COM_DATA])
                    {
                        s_is_at_from_debug_port = FALSE;
                        // OK, we should transmit this message to
#ifdef HTW_INTEGRATION
                        //
                        //Have a unstable here beacuse "cur_atc_frame_size" is a local variable,
                        //other task can change it at any time .
                        SetLen (cur_atc_frame_size);
                        NU_Activate_HISR (&RX_FULL_ISR_USER);
#else
                        DO_ATC_CALLBACK (cur_atc_frame_size,COM_DATA);
#endif

                        cur_atc_frame_size = 0;
                    }
                }
            }
        }

        // Get the time now!
        last_rx_time = SCI_GetTickCount();
    }
   return receive_len;
}
#else
LOCAL uint32 SIO_RxForUserPort (uint8 *data_buf,uint32 rec_num)
{
    unsigned char  nchar;
    SIO_INFO_S  *sio_ptr = &sio_port[COM_DATA];
    uint32 receive_len =rec_num;

#if 1 //defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIX8910MPW)|| defined(CHIP_VER_UIS8910A)
#ifdef BLUETOOTH_SUPPORT_SPRD_BT
    //SCI_TRACE_LOW("SIO_RxForUserPort");
    if(UART_GetControllerBqbMode())
    {
        UART_Controller_Bqb_Write(data_buf, rec_num);
        return receive_len;
    }
#endif
#endif

    if (MIX_MODE == user_port_mode)
    {
#ifdef _U0_FLOW_CONTROL

        if (ring_space (& (sio_port[COM_DATA].rx_buf)) <= EMPTY_SPACE_WATER_LOW_MARK)
        {
            SIO_Rx_Int_Enable (COM_DATA,0); // Disable Out_endp interrupt of usb
        }

#endif

        SIO_MixRecv (sio_ptr,data_buf,rec_num);
        return receive_len;
    }

    {
        uint32      cur_rx_time = SCI_GetTickCount();

#ifdef _U0_SOFTWARE_FLOW_CONTROL

        if ( (1 == gXon_on) && (ring_space (& (sio_port[COM_DATA].rx_buf)) <= DATA_PORT_REC_BUF_SIZE*3/4))
        {
            //SCI_TRACE_LOW:"Software flow control:send XOFF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_5910_112_2_17_23_5_32_1097,(uint8*)"");
            gXon_on = 0;
            //add code here to send XOFF *******
        }

#endif

        // If it is possible that we got three '+'
        if ( (MAX_PLUS_NUMBER >= rec_num)
                && (MODE_CHANGE_DELAY < (cur_rx_time - last_rx_time))
                && DATA_MODE == user_port_mode)
        {
            uint32 i;
            uint8 plus_char[MAX_PLUS_NUMBER];
            uint32 plus_num_temp = MAX_PLUS_NUMBER;

            // Fisrt get the char from FIFO
            for (i = 0; i < MAX_PLUS_NUMBER; i++)
            {
                plus_char[i] = *data_buf++;

#ifdef _U0_SOFTWARE_FLOW_CONTROL

                if (plus_char[i] == XON)
                {
                    gRemoteXon_on = 1;
                }
                else if (plus_char[i] == XOFF)
                {
                    gRemoteXon_on = 0;
                }

#endif

                // Check if it is a plus char
                if (PLUS_CHAR != plus_char[i])
                {
                    plus_num_temp = 0;
                }
            }

            if (MAX_PLUS_NUMBER == plus_num_temp)
            {
                // Set Timer to check if we received a mode change command.
                SCI_DeactiveTimer (ATC_timer);
                SCI_ChangeTimer (ATC_timer, ATC_CheckPPP, MODE_CHANGE_DELAY + 100); // Add 100 to make the timer response longer, so that the timeout logic will be OK.
                SCI_ActiveTimer (ATC_timer);
            }
            else
            {
                // No, so we have to put the data back to sio buffer
                for (i = 0; i < MAX_PLUS_NUMBER; i++)
                {
                    if (ring_add (&sio_port[COM_DATA].rx_buf, plus_char[i]) == -1)
                    {
                        continue;
                    }
                }
            }
        }
        else
        {
            /*
                rx fifo full or timeout,
                try read char from device
            */
            while (rec_num)
            {
                nchar = *data_buf++;
                rec_num--;

#ifdef _U0_SOFTWARE_FLOW_CONTROL

                if (nchar == XON)
                {
                    gRemoteXon_on = 1;
                }
                else if (nchar == XOFF)
                {
                    gRemoteXon_on = 0;
                }

#endif

                if (user_port_mode == ATC_MODE)
                {
                    if (ring_add (&sio_port[COM_DATA].atc_buf, nchar) == -1)
                    {
                        //Hyman.wu
                        //too long at command,it is should not valid at commands!
                        //so just clear buffer,else will lead to overrun
                        sio_port[COM_DATA].atc_buf.end_point = sio_port[COM_DATA].atc_buf.start_point;
                        continue;
                    }
                }
                else if (ring_add (&sio_port[COM_DATA].rx_buf, nchar) == -1)
                {
                    //Hyman.wu,2005-1-24
                    //too long data will lead to overrun and deep sleep assert
                    //so discard it,this is the bad flow control but not assert
                    //sio_port[COM_DATA].rx_buf.end_point = sio_port[COM_DATA].rx_buf.start_point;
                    //continue;
                    receive_len = receive_len-rec_num-1;                       //don't lost one char
                    SIO_Rx_Int_Enable (COM_DATA,0); // Disable Out_endp interrupt of usb

                    SCI_ChangeTimer (Space_timer, Space_Check, SPACE_CHECK_DELAY);/*lint !e64 */
                    SCI_ActiveTimer(Space_timer);
                    break;
                }

                if (user_port_mode == ATC_MODE)
                {
                    cur_atc_frame_size++;

                    if (atc_is_echo)
                    {
                        if (uart_mix_sem_take())
                        {
                            if (ring_space (& (sio_port[COM_DATA].tx_buf)))
                            {
                                // Echo back to the hyper teriminal
                                uart_ppp_putc (COM_DATA, nchar);

                                //enable usb inpoint int
                                SIO_Tx_Int_Enable (COM_DATA,TRUE);

                            }

                            /*
                             * rewirte the end_point of tx buffer,so the packet can
                             * be send out now.
                             */
                            uart_mix_set_end_point (0xFFFFFFFF);
                            uart_mix_sem_give();
                        }
                    }

                    if (nchar == atc_end_ch1 || nchar == atc_end_ch2)
                    {
                        s_is_at_from_debug_port = FALSE;
                        // OK, we should transmit this message to
#ifdef HTW_INTEGRATION
                        //
                        //Have a unstable here beacuse "cur_atc_frame_size" is a local variable,
                        //other task can change it at any time .
                        SetLen (cur_atc_frame_size);
                        NU_Activate_HISR (&RX_FULL_ISR_USER);
#else
                        DO_ATC_CALLBACK (cur_atc_frame_size);
#endif

                        cur_atc_frame_size = 0;
                    }
                }
            }
        }

        // Get the time now!
        last_rx_time = SCI_GetTickCount();
    }
   return receive_len;
}
#endif

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
LOCAL uint32 SIO_TxForDebugPort (uint8 *data_buf,uint32 fifo_size)
{
    return 0;
}

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:
/*****************************************************************************/
#ifdef PPP_USB
LOCAL uint32 SIO_TxForUserPort (uint8 *data_buf,uint32 fifo_size,uint8 port_index)
#else
LOCAL uint32 SIO_TxForUserPort (uint8 *data_buf,uint32 fifo_size)
#endif
{
    uint32   snd_size = 0;
    uint32   s_start_point = 0;
    uint32   s_end_point   = 0;
    uint32   s_size        = 0;
    uint8   *s_buf         = NULL;
    SIO_BUF_S *temp_tx_buf = NULL;
    uint32   remain_size = 0;
    unsigned char  nchar;
    uint8 port = COM_DATA;

#ifdef PPP_USB
    if(COM_DATA2 == port_index)
	port = COM_DATA2;
#endif

    s_start_point = (&sio_port[port].tx_buf)->start_point;
    s_end_point   = (&sio_port[port].tx_buf)->end_point;
    s_size        = (&sio_port[port].tx_buf)->size;
    s_buf         = (&sio_port[port].tx_buf)->sio_buf_ptr;
    temp_tx_buf = &sio_port[port].tx_buf;

    snd_size = 0;

#ifdef _U0_SOFTWARE_FLOW_CONTROL

    if (gRemoteXon_on)   // Remote device can receive data.
    {
#endif
        /*in 6600/6800, for the limitation of usb bluk transfer, no empty packet at INtranfser
          only under these case: short packet/empty packet or the exact size host dirver
          can invoke upper layer to read data.
          so we should send a short packet to prevent this case*/

        /*find the data size in com_data tx buffer*/
        if (temp_tx_buf->end_point >= temp_tx_buf->start_point)
        {
            remain_size = (uint32) (temp_tx_buf->end_point - temp_tx_buf->start_point);
        }
        else
        {
            remain_size = (uint32) ( (int) (temp_tx_buf->size) + temp_tx_buf->end_point - temp_tx_buf->start_point);
        }

        if (remain_size == fifo_size)
        {
            fifo_size = fifo_size - 1;
        }

        while (fifo_size)
        {
            if (s_end_point == s_start_point)
            {
                /* no char in buffer, so disable tx irq  */

                break;
            }

            nchar = s_buf[s_start_point++];

            if (s_start_point >= s_size)
            {
                s_start_point = 0;
            }

            *data_buf++ = nchar;
            snd_size++;

            fifo_size--;
        }/* End while */

        (&sio_port[port].tx_buf)->start_point = s_start_point;
#ifdef _U0_SOFTWARE_FLOW_CONTROL
    }

#endif

    return snd_size;
}


LOCAL uint32 SIO_DebugPortHdl (uint32 event)
{
    uint32 num=0,read_len=0;
    uint32 phy_port;
    SCI_ASSERT (event < COM_MAX_EVENT);/*assert verified*/

    phy_port = sio_port[COM_DEBUG].phy_port;

    //here we just for uart
    switch (event)
    {
        case EVENT_DATA_TO_READ:
            {
                uint32 free_data_size = 0;
                GET_BUF_FREE_SIZE (free_data_size, (&sio_port[COM_DEBUG].rx_buf))
                if (free_data_size > sizeof(s_uart_debug_rxbuf)) {
                    read_len = SIO_GetRxFifoCnt(COM_DEBUG);
                    num = (sio_port[COM_DEBUG].sio_op.read) (SIO_PhyPortfromIndex (phy_port),s_uart_debug_rxbuf,read_len);

                    if (num > 0)
                    {
                        /*
                            if ((NULL != s_sio_irqen_timer) && SCI_IsTimerActive(s_sio_irqen_timer))
                            {
                                SCI_DeactiveTimer(s_sio_irqen_timer);
                            }
                        */
                        if (s_sio_uart_sleep_timer)
                        {
                           if(SCI_IsTimerActive(s_sio_uart_sleep_timer))
                           {
                           		SCI_DeactiveTimer(s_sio_uart_sleep_timer);
                           }
                           SCI_ChangeTimer(s_sio_uart_sleep_timer, SIO_UartSleepTimerExpired, SIO_UART_SLEEP_TIMER_VALUE);
                           SCI_ActiveTimer(s_sio_uart_sleep_timer);
                        }
                        read_len = SIO_RxForDebugPort (s_uart_debug_rxbuf,num);
                    }
                } else {
                    uint32 status_E;
                    SIO_Rx_Int_Enable(COM_DEBUG,FALSE);
                    s_sio_rx_int_is_disabled = TRUE;
                    status_E = SCI_SetEvent (comm_event, (COM_DEBUG << 1), SCI_OR);
                }
            }
            break;
        case EVENT_WRITE_COMPLETE:
            {
                if((phy_port <= COM3) && (phy_port >= COM0))
                {
                    SCI_TraceLow("SIO_DebugPortHdl EVENT_WRITE_COMPLETE COM%d",phy_port);
                    UART_Tx_Int_Enable(phy_port,FALSE);
                }
                if ( (phy_port > COM3) && (phy_port < DSK_COM0))
                {
                    SIO_Usb_TxDma_CallBack();
                }

                if ( (DSK_COM0 <= phy_port) && (DSK_COM1 >= phy_port))
                {
                    SIO_CardLog_Callback();
                }
            }
            break;
        case EVENT_INIT_COMPLETE:
            break;
        case EVENT_SHUTDOWN_COMPLETE:
            break;
        default:
            break;

    }
    return read_len;
}



LOCAL uint32 SIO_Debug2_PortHdl (uint32 event)
{
    uint32 num=0,read_len=0;
    uint32 phy_port;
    SCI_ASSERT (event < COM_MAX_EVENT);/*assert verified*/

    phy_port = sio_port[COM_DEBUG2].phy_port;

    //here we just for uart
    switch (event)
    {
        case EVENT_DATA_TO_READ:

            break;
        case EVENT_WRITE_COMPLETE:
            {
                if ( ((phy_port > COM3) && (phy_port < DSK_COM0))  || ((phy_port > DSK_COM1) && (phy_port <= VIR_COM4)))
                {
                    SIO_ModemLogUsbSendCallBack();
                }

            }
            break;
        case EVENT_INIT_COMPLETE:
            break;
        case EVENT_SHUTDOWN_COMPLETE:
            break;
        default:
            break;

    }
    return read_len;
}


LOCAL uint32 SIO_DataPortHdl (uint32 event)
{
    uint32 num=0,read_len=0;
    uint32 phy_port;
    uint32 fifo_cnt;

    SCI_ASSERT (event < COM_MAX_EVENT);/*assert verified*/

    phy_port = sio_port[COM_DATA].phy_port;

    switch (event)
    {
        case EVENT_DATA_TO_READ:   //receive data from uart or vcom
            {
		        fifo_cnt = SIO_GetRxFifoCnt(COM_DATA);
                num = (sio_port[COM_DATA].sio_op.read) (SIO_PhyPortfromIndex (phy_port),s_uart_data_rxbuf,fifo_cnt);

                if (num > 0)
                {
                   if (s_sio_uart_sleep_timer)
                   {
                       if(SCI_IsTimerActive(s_sio_uart_sleep_timer))
                       {
                       		SCI_DeactiveTimer(s_sio_uart_sleep_timer);
                       }
                       SCI_ChangeTimer(s_sio_uart_sleep_timer, SIO_UartSleepTimerExpired, SIO_UART_SLEEP_TIMER_VALUE);
                       SCI_ActiveTimer(s_sio_uart_sleep_timer);
                   }
                    //here only useful for uart,when current fifocnt
                    //is more than max UART_FIFO_SIZE,then only enable uart TX
                    //if current is usb, fifo_cnt will not be zero
          #ifdef PPP_USB
                  read_len = SIO_RxForUserPort (s_uart_data_rxbuf,num,COM_DATA);
		  #else
                  read_len = SIO_RxForUserPort (s_uart_data_rxbuf,num);
		  #endif
                }
                num = read_len;
            }
            break;
        case EVENT_WRITE_COMPLETE: //send data to uart or vcom
            {
                fifo_cnt = SIO_GetTxFifoCnt (COM_DATA);
                if (fifo_cnt > sizeof (s_uart_data_txbuf))
                {
                	fifo_cnt = sizeof(s_uart_data_txbuf);
                }
                if (fifo_cnt)
                {
            #ifdef PPP_USB
                    num = SIO_TxForUserPort (s_uart_data_txbuf,fifo_cnt,COM_DATA);
            #else
                    num = SIO_TxForUserPort (s_uart_data_txbuf,fifo_cnt);

            #endif

                    //add log for bug1006386
                    SCI_TraceLow("ppp:SIO_DataPortHdl,num%d ",num);
                    if (num > 0)
                    {
                        (sio_port[COM_DATA].sio_op.write) (SIO_PhyPortfromIndex (phy_port),s_uart_data_txbuf,num);
                        if (s_sio_uart_sleep_timer)
                        {
                        	if(SCI_IsTimerActive(s_sio_uart_sleep_timer))
                        	{
                        		SCI_DeactiveTimer(s_sio_uart_sleep_timer);
                        	}
                        	SCI_ChangeTimer(s_sio_uart_sleep_timer, SIO_UartSleepTimerExpired, SIO_UART_SLEEP_TIMER_VALUE);
                        	SCI_ActiveTimer(s_sio_uart_sleep_timer);
                    }
                    }
                    else
                    {
                        //when using usb virtual com, then should disable it(not closing intbits in usb isr handle)
                        //it will affect nothing to disable double times for uart,
                        SIO_Tx_Int_Enable (COM_DATA,FALSE);
                    }
                }
                else
                {
                    //here only useful for uart,when current fifocnt
                    //is more than max UART_FIFO_SIZE,then only enable uart TX
                    //if current is usb, fifo_cnt will not be zero
                    SIO_Tx_Int_Enable (COM_DATA,TRUE);
                }
            }
            break;
        case EVENT_INIT_COMPLETE:
            break;
        case EVENT_SHUTDOWN_COMPLETE:
            break;
        default:
            break;

    }
    return num;
}

#ifdef PPP_USB
void ppp_clear_send_status(void)
{
    if ((user_port_mode_2 == DATA_MODE) && g_ppp_send_status && g_ppp_data_buf)
    {
        sio_ppp_free_tx_buf(g_ppp_data_buf);
        g_ppp_data_buf = NULL;
        g_ppp_send_status = 0;
        //SCI_TRACE_LOW("sio: UCOM tx done");
        sio_ppp_tx_enable(1);
        //return;
    }
}

LOCAL uint32 SIO_DataPortHdl_2 (uint32 event)
{
    uint32 num=0,read_len=0;
    uint32 phy_port;
    uint32 port_num;
    uint32 fifo_cnt;
    uint32 ppp_rxbuf_tail;
    uint8 *ppp_rxbuf;

    SCI_ASSERT (event < COM_MAX_EVENT);/*assert verified*/

    phy_port = sio_port[COM_DATA2].phy_port;
    //SCI_TRACE_LOW("ppp:SIO_DataPortHdl_2,event%d,mode %d,32K 0x%x",
    //        event, user_port_mode_2, SLEEP_GetCur32kNum());
    switch (event)
    {
        case EVENT_DATA_TO_READ:   //receive data from uart or vcom
            {
                fifo_cnt = SIO_GetRxFifoCnt(COM_DATA2);
                port_num = SIO_PhyPortfromIndex(phy_port);

            //while (fifo_cnt)
            //{
                if (fifo_cnt && (user_port_mode_2 == ATC_MODE))
                {
                    sio_ppp_rx_buff_free();
                    SCI_MEMSET(s_uart_data2_rxbuf,0,MAX_RXBUF_SIZE);
                    num = (sio_port[COM_DATA2].sio_op.read)(port_num, s_uart_data2_rxbuf, /*1*/fifo_cnt);
                    if (num > 0)
                    {
                        read_len = SIO_RxForUserPort(s_uart_data2_rxbuf, num, COM_DATA2);
                        //fifo_cnt--;
                    }
                }
                /*else
                {
                    break;
                }*/
            //};
                else if (fifo_cnt && (user_port_mode_2 == DATA_MODE))
                {
                    sio_ppp_rx_buff_alloc();

                    ppp_rxbuf = (p_ppp_rx_buf + g_ppp_rx_widx);
                    ppp_rxbuf_tail = PPP_RXBUF_SIZE - g_ppp_rx_widx;

                    SCI_TraceLow("ppp:3 rx %u, tail %u, 32k 0x%x\n",
                        fifo_cnt, ppp_rxbuf_tail, SLEEP_GetCur32kNum());

                    if (ppp_rxbuf_tail > fifo_cnt)
                    {
                        num = (sio_port[COM_DATA2].sio_op.read)(port_num, ppp_rxbuf, fifo_cnt);
                        if (num > 0)
                        {
                            g_ppp_rx_widx += num;
                            g_ppp_rx_widx &= PPP_RXBUF_MASK;
                        }
                    }
                    else
                    {
                        num = (sio_port[COM_DATA2].sio_op.read)(port_num, ppp_rxbuf, ppp_rxbuf_tail);
                        if (num > 0)
                        {
                            g_ppp_rx_widx += num;
                            g_ppp_rx_widx &= PPP_RXBUF_MASK;

                            SCI_ASSERT(num == ppp_rxbuf_tail);
                            num = (sio_port[COM_DATA2].sio_op.read)(port_num, p_ppp_rx_buf, fifo_cnt - ppp_rxbuf_tail);
                            if (num > 0)
                            {
                                g_ppp_rx_widx += num;
                                g_ppp_rx_widx &= PPP_RXBUF_MASK;
                            }
                        }
                    }
                    {
						SCI_TRACE_LOW("sio: SIO_RxForUserPort  rec_num:%d ",num);
						SCI_SetThreadEvent(PPPREADSVC, 1 << PPP_READ, SCI_OR);
                     }
                }

                //SCI_PPP_TRACE("ppp:SIO_DataPortHdl_2,reading %d,readed %d",num,read_len);
                num = read_len;
            }
            break;
        case EVENT_WRITE_COMPLETE: //send data to uart or vcom
            if ((user_port_mode_2 == DATA_MODE) /*&& g_ppp_send_status */&& g_ppp_data_buf)
            {
                sio_ppp_free_tx_buf(g_ppp_data_buf);
                g_ppp_data_buf = NULL;
                g_ppp_send_status = 0;
                //SCI_TRACE_LOW("sio: UCOM tx done");
                sio_ppp_tx_enable(1);
                //return;
            }
            break;
        case EVENT_INIT_COMPLETE:
            break;
        case EVENT_SHUTDOWN_COMPLETE:
        	{
				SCI_DisableIRQ();
				g_ppp_send_status = 0;
			    g_pppbuf_free = g_pppbuf_alloc = 0;
				SCI_MEMSET(s_ppp_dma_txbuf,0,16*2048);
			    SCI_RestoreIRQ();
        	}
            break;
        default:
            break;

    }
    return num;
}

#endif


void SIO_ReInitWithBtUartShutDown(void)
{
    UART_INIT_PARA_T uart_st;

#if defined(CHIP_VER_UIX8910MPW)
    uart_st.tx_watermark  = TX_WATER_MARK; // 0~127B
    uart_st.rx_watermark  = UART_RxWaterMark(); // 0~127B
    uart_st.baud_rate     = sio_port[COM_DATA].dcb.baud_rate;
    uart_st.parity        = FALSE;
    uart_st.parity_enable = PARITY_DISABLE;
    uart_st.byte_size     = EIGHT_BITS;
    uart_st.stop_bits     = ONE_STOP_BIT;
    uart_st.flow_control  = NO_FLOW_CONTROL;
    uart_st.ds_wakeup_type= DS_WAKEUP_BY_RXD;

    // ONLY enable in Following mode, for writing IMEI by UART,USB
    if(REFPARAM_GetUsbAutoAssertRelModeFlag())
    {
        uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;//DS_WAKEUP_ENABLE
    }
    else
    {
        uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
    }

    UART_Initilize(SIO_PhyPortfromIndex(REFPARAM_GetDataPortPhyNo()),&uart_st,SIO_DebugPortHdl);
#elif defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E)
    uart_st.tx_watermark  = TX_WATER_MARK; // 0~127B
    uart_st.rx_watermark  = UART_RxWaterMark(); // 0~127B
    uart_st.baud_rate     = sio_port[COM_DEBUG].dcb.baud_rate;
    uart_st.parity        = FALSE;
    uart_st.parity_enable = PARITY_DISABLE;
    uart_st.byte_size     = EIGHT_BITS;
    uart_st.stop_bits     = ONE_STOP_BIT;
    uart_st.flow_control  = NO_FLOW_CONTROL;
    uart_st.ds_wakeup_type= DS_WAKEUP_BY_RXD;

    // ONLY enable in Following mode, for writing IMEI by UART,USB
    //coverity 36906. del the redundancy code
   // if(REFPARAM_GetUsbAutoAssertRelModeFlag())
   // {
        uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;//DS_WAKEUP_ENABLE
    //}
    //else
    //{
    //    uart_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
    //}

    UART_Initilize(SIO_PhyPortfromIndex(REFPARAM_GetDataPortPhyNo()),&uart_st,SIO_DebugPortHdl);
#endif
}




#if defined(PRODUCT_DM)
/*****************************************************************************/
// Description :  set uart1 and uart2 for ui test  (6800 DVB)
// Global resource dependence :
// Author :        yuehz
// Input:
//
// Return:
//        None
// Note :   20060418
/*****************************************************************************/
LOCAL void SIO_SetUARTConfigForUITest (void)
{

}


/*****************************************************************************/
// Description :  restore uart1 and uart2 for ui test(6800 DVB)
// Global resource dependence :
// Author :        yuehz
// Input:
//
// Return:
//        None
// Note :   20060418    modified 20060512
/*****************************************************************************/
LOCAL void SIO_RestoreUARTConfigForUITest (void)
{

}

/*****************************************************************************/
// Description :  get a char from one of three coms
// Global resource dependence :
// Author :        yuehz
// Input:
//
// Return:
//        None
// Note :   20060417
/*****************************************************************************/
LOCAL char SIO_GetAChar (uint32 phy_port)
{
    char ch=NULL;

    if (phy_port==COM0)
    {
        if ( ( (* (volatile uint32 *) (ARM_UART0_BASE+0x0c)) & 0x0FF) >0)
        {
            ch=* (volatile uint32 *) (ARM_UART0_BASE+0x04);
        }
    }
    else if (phy_port==COM1)
    {
        if ( ( (* (volatile uint32 *) (ARM_UART1_BASE+0x0c)) & 0x0FF) >0)
        {
            ch=* (volatile uint32 *) (ARM_UART1_BASE+0x04);
        }
    }
    else if (phy_port==COM2)
    {
        if ( ( (* (volatile uint32 *) (ARM_UART2_BASE+0x0c)) & 0x0FF) >0)
        {
            ch=* (volatile uint32 *) (ARM_UART2_BASE+0x04);
        }
    }

    return ch;
}

/*****************************************************************************/
// Description :  put a char to one of three coms
// Global resource dependence :
// Author :        yuehz
// Input:
//
// Return:
//        None
// Note :   20060417
/*****************************************************************************/
LOCAL void SIO_PutAChar (uint32 phy_port,char ch)
{

    if (phy_port==COM0)
    {
        while ( ( (* (volatile uint32 *) (ARM_UART0_BASE+0x0c)) >>8) & 0x0FF) {};

        * (volatile uint32 *) (ARM_UART0_BASE) =ch;
    }
    else if (phy_port==COM1)
    {
        while ( ( (* (volatile uint32 *) (ARM_UART1_BASE+0x0c)) >>8) & 0x0FF) {};

        * (volatile uint32 *) (ARM_UART1_BASE) =ch;
    }
    else if (phy_port==COM2)
    {
        while ( ( (* (volatile uint32 *) (ARM_UART2_BASE+0x0c)) >>8) & 0x0FF) {};

        * (volatile uint32 *) (ARM_UART2_BASE) =ch;
    }


}

PUBLIC uint32 SIO_CreateComm (uint32 brate, uint32 char_size, uint32 fctype,
                              uint32 num_stop_bits, uint32 ptype, uint16 port_no, uint32 *logic_port
                             )
{
    SIO_CONTROL_S   m_dcb;
    uint32      debug_phy_port;
    uint32      hid_com_dev = NULL;

    debug_phy_port = sio_port[COM_DEBUG].phy_port;

    m_dcb.flow_control   = 0;

    if (sio_port[COM_DEBUG].dcb.baud_rate == 0)
    {
        sio_port[COM_DEBUG].dcb.baud_rate = SIO_GetBaudRate (COM_DEBUG);
    }

    m_dcb.baud_rate =   sio_port[COM_DEBUG].dcb.baud_rate;

    hid_com_dev = (uint32) SIO_Create (COM_DEBUG, debug_phy_port, &m_dcb);

    *logic_port = COM_DEBUG;

    return hid_com_dev;
}

PUBLIC  int HIGH_WritePPPFrame (uint32 port, uint8 *src, int size)
{
    return size;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void SIO_DumpWritePPPFrame()                                          *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Receive data from input source, encode it as ppp frame and then write *
 **     it into serial port directly.                                         *
 **                                                                           *
 ** RETURN VALUE                                                              *
 **     Return the size of byte written to the buffer.                        *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **     Called by com_serial_write routine                                    *
 **                                                                           *
 **---------------------------------------------------------------------------*/
PUBLIC  int SIO_DumpWritePPPFrame (uint32 port, uint8 *src, int size)
{
#if 0
    int    send_size;
    int    remain_size = 0;
    int    count_send_size = 0;
    int    free_count = 0;
#endif

    SCI_ASSERT (port < MAX_LOGICAL_SIO_PORT_NUM);/*assert verified*/
    SCI_ASSERT (src != NULL);/*assert verified*/

    if (sio_port[port].open_flag != SIO_ALREADY_OPEN)
    {
        return 0;
    }

    /*header flag*/
    SIO_DumpPutChar (port,FLAG_BYTE) ;

    /* Message body */

    EncodeMsg_Dump (port, src, size);


    /*tail flag*/
    SIO_DumpPutChar (port,FLAG_BYTE) ;

    return size;
}
#endif
#ifdef PPP_USB
LOCAL void SIO_PPP_RxForUserPort (uint32 port,uint32 rec_num)
{
	//unsigned char  nchar;
	uint32 receive_len =rec_num;
	SCI_TRACE_LOW("sio: SIO_PPP_RxForUserPort rec_num:%d ",receive_len);
	PPP_Receive_UL_Data(port, receive_len, PNULL);
}
LOCAL void ppp_read_entry(uint32 argc, void *argv)
{
    PPPREAD_SIG_T *ppp_sig_ptr_read = SCI_NULL;
	uint32 actual_flags;
    while (TRUE)
    {
    	SCI_GetThreadEvent(PPPREADSVC,
				(1 << PPP_READ),
				SCI_OR_CLEAR,
				&actual_flags,
				SCI_WAIT_FOREVER);

		PPP_Receive_UL_Data (0, 0,PNULL);
    }
}
PUBLIC void ppp_read_init(void)
{
    PPPREADSVC = SCI_CreateThread("ppp_read",
                     "ppp_read",
                      ppp_read_entry,
                      0,
                      0,
                      4096,
                      200,
                      24,
                      SCI_PREEMPT,
                      SCI_AUTO_START);
}
#endif
#ifdef _SIO_DEBUG_
static BLOCK_ID s_sio_test_task;

static void sio_test_task (uint32 argc, void *argv)
{
    uint8 *data_ptr,*data_ptr1;
    uint32 total_size;
    int32 i;
    uint32 old_tick, new_tick;

    total_size = DEBUG_PORT_SEND_BUF_SIZE*2;
    data_ptr= (uint8 *) SCI_ALLOC_APP (total_size);
    data_ptr1 = (uint8 *) SCI_ALLOC_APP (100);

    for (i= 0; i< 100; i++)
    {
        * (data_ptr1+i) = i;
    }

    //    SCI_InitLogSwitch(FALSE);
    SCI_SLEEP (10000);
    SCI_SLEEP (10000);
    SCI_SLEEP (5000);

    for (;;)
    {
        SCI_MEMSET (data_ptr, 0x55, total_size);
        SCI_MEMSET (data_ptr+ (total_size>>1), 0xaa, total_size>>1);

        //
        //test sio_sendpacket
        //case 1, the last one is 0x7e
        * (data_ptr+total_size -2) = 0x7e;
        old_tick = SCI_GetTickCount();
        SIO_SendPacket (data_ptr, total_size);
        new_tick = SCI_GetTickCount();
        //SCI_TRACE_LOW:"@@@@tick used %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_6483_112_2_17_23_5_34_1098,(uint8*)"d", (new_tick-old_tick));

        old_tick = SCI_GetTickCount();
        SIO_SendPacket (data_ptr1, 100);
        new_tick = SCI_GetTickCount();
        //SCI_TRACE_LOW:"@@@@tick used %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_6488_112_2_17_23_5_34_1099,(uint8*)"d", (new_tick-old_tick));

        //case 2, the last two is 0x7e,X
        * (data_ptr+total_size -3) = 0x7e;
        * (data_ptr+total_size -2) = 0x55;
        old_tick = SCI_GetTickCount();
        SIO_SendPacket (data_ptr, total_size);
        new_tick = SCI_GetTickCount();
        //SCI_TRACE_LOW:"@@@@tick used %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_6496_112_2_17_23_5_34_1100,(uint8*)"d", (new_tick-old_tick));

        SIO_SendPacket (data_ptr1, 100);

        //case3: write data is more than max buffer size, and last two byte is 0x7e
        * (data_ptr+total_size -3) = 0x55;
        * (data_ptr+total_size -2) = 0x55;
        * (data_ptr+DEBUG_PORT_SEND_BUF_SIZE -2) = 0x7e;
        old_tick = SCI_GetTickCount();
        SIO_SendPacket (data_ptr, total_size);
        new_tick = SCI_GetTickCount();
        //SCI_TRACE_LOW:"@@@@tick used %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_6507_112_2_17_23_5_34_1101,(uint8*)"d", (new_tick-old_tick));

        SIO_SendPacket (data_ptr1, 100);

        //case4s: write data is more than max buffer size, and last two byte is 0x7e 0xaa
        * (data_ptr+DEBUG_PORT_SEND_BUF_SIZE -3) = 0x7e;
        * (data_ptr+DEBUG_PORT_SEND_BUF_SIZE -2) = 0x7e;
        old_tick = SCI_GetTickCount();
        SIO_SendPacket (data_ptr, total_size);
        new_tick = SCI_GetTickCount();
        //SCI_TRACE_LOW:"@@@@tick used %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_6517_112_2_17_23_5_34_1102,(uint8*)"d", (new_tick-old_tick));

        SIO_SendPacket (data_ptr1, 100);

        //SCI_TRACE_LOW:"@@@@max tick used %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIO_6521_112_2_17_23_5_34_1103,(uint8*)"d", max_time);
        SCI_SLEEP (2000);

    }
}

void sio_test_init (void)
{
    s_sio_test_task = SCI_CreateThread ("sio_test_task",
                                        NULL,
                                        sio_test_task,
                                        0,
                                        0,
                                        4096,
                                        1,
                                        31,
                                        SCI_PREEMPT,
                                        SCI_AUTO_START
                                       );

}

#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif /* End of sio.c*/

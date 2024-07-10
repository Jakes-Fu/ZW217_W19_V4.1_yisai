/******************************************************************************
 ** File Name:      mux_all.h                                      *
 ** Author:         yayan.xu                                              *
 ** DATE:           07/10/2008                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    All header info for MUX module*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                       NAME             DESCRIPTION                               *
 ** 07/10/2008     yayan.xu                 Create.                                   *
 ******************************************************************************/
#ifndef _MUX_ALL_H
#define _MUX_ALL_H
#include "sci_types.h"
#ifndef _WINDOWS
#include "os_api.h"
#include "sci_api.h"
#endif
#include "mux_os.h"
#ifdef   __cplusplus
extern   "C"
{
#endif

#ifndef _WINDOWS
#define MUX_TRACE_LOW( _format_string )   SCI_TRACE_LOW _format_string
//#define MUX_TRACE_LOW( _format_string )   
#define MUX_TRACE_WARNING(_format_string)  SCI_TRACE_LOW _format_string
#ifdef _MUX_DEBUG_
#define MUX_ASSERT(_exp)              SCI_ASSERT(_exp)
#else /*_MUX_DEBUG_*/
#define MUX_ASSERT(_exp) 
#endif/*_MUX_DEBUG_*/
#endif

    /*------------------------------------------------------------------------------
    Defination :MUX contanst 
    ------------------------------------------------------------------------------*/


#define MAX_MUX_NUM  2 // 1  // Support 2 MUX entry
#define MUX_1    0 //First MUX entry
#define MUX_2    1 //Second MUX entry


    /*MUX RECV TASK info*/
#define MUX_SLEEP_TIME    40
#ifndef MODEM_ZBX_MUX
#define MUX_REC_DATA_MAX   30000    //120000 //MS00242355  30000   /* 6344 = 1.1*1024*1024/8/(1000/40)*1.1 */
#define MUX_SEND_DATA_MAX  30000    //120000   /* 6344 = 1.1*1024*1024/8/(1000/40)*1.1 */
#else
#define MUX_REC_DATA_MAX   10000    //120000 //MS00242355  30000   /* 6344 = 1.1*1024*1024/8/(1000/40)*1.1 */
#define MUX_SEND_DATA_MAX  10000    //120000   /* 6344 = 1.1*1024*1024/8/(1000/40)*1.1 */
#endif

    /*Support DLC number*/
#define MUX_COM1_LINK_NUM  16 // OMS phone require mux link equals 16¡£
#define MUX_COM2_LINK_NUM  1

#define MUX_MAX_LINK_NUM  (MUX_COM1_LINK_NUM + MUX_COM2_LINK_NUM)//5 /*MS00130031, add 5*/
#define MUX_COM1_CTL_LINK 0
#define MUX_COM2_CTL_LINK  MUX_COM1_LINK_NUM
#define MUX_DLC_LINK_INVALID  0xFF
#define MUX_MAX_DLC_NUM  63/*Protocol define maximum value*/
 /* the max size of the mux recive buff*/
//#define MUX_MAX_BUF_SIZE  3300  //above   2* MUX_MAX_TOTAL_FRAME_SIZE because advacne mode, need convert.
#define MUX_MAX_BUF_SIZE 4300 // add buf size @yayan.xu 

#define MUX_MAX_TOTAL_FRAME_SIZE (MUX_MTU_DEFAULT + MUX_MAX_HDR_SIZE + FLAG_SIZE)
#define ADDRESS_FIELD_OFFSET 1              /*offset from start flag */
#define MUX_SHORTFRAME_OTHER_SIZE (FCS_SIZE+FLAG_SIZE+MUX_SHORT_HEAD_SZIE)
#define MUX_LONGFRAME_OTHER_SIZE (FCS_SIZE+FLAG_SIZE+MUX_LONG_HEAD_SIZE)
#define MUX_MCCFRAME_BUFFSIZE 30

#define MUX_TIME_OUT 1000  //250 is too short to TE MUX.
    //FRAME TYPE
#define MUX_SABM 0x2f
#define MUX_UA 0x63
#define MUX_DM 0x0f
#define MUX_DISC 0x43
#define MUX_UIH 0xef
#define MUX_UI 0x03

#define SET_PF(ctr) ((ctr) | (1 << 4))
#define SET_CR(ctr) ((ctr == MUX_SIDE_TE)? 1: 0)
#define CLR_PF(ctr) ((ctr) & 0xef)
#define GET_PF(ctr) (((ctr) >> 4) & 0x1)

#define GET_EA(a) (a&0x1)

#define GET_CR(a) (a&0x2)

#define GET_DLCI(a) (a>>2)

#define GET_CONTROL_FIELD(a)  (*(a+2))
#define PROCESS_SABM(a) ((a == (SET_PF(MUX_SABM)) )||(a == MUX_SABM))
#define PROCESS_UA(a) (a == (SET_PF(MUX_UA))||(a == MUX_UA))
#define PROCESS_DM(a) (a == (SET_PF(MUX_DM))||(a == MUX_DM))
#define PROCESS_DISC(a) (a == (SET_PF(MUX_DISC))||(a == MUX_DISC))
#define PROCESS_UIH(a)  (a == (SET_PF(MUX_UIH))||(a == MUX_UIH))
#define PROCESS_UI(a) (a == (SET_PF(MUX_UI))||(a == MUX_UI))

#define MUX_CTRL_CHAN 0 
#define MUX_BASIC_OPEN_FLAG   0xF9
#define MUX_BASIC_CLOSE_FLAG    MUX_BASIC_OPEN_FLAG

#define MUX_MAX_ERING_SIZE  256

#define FCS_SIZE  1
#define FLAG_SIZE  2
#define MUX_SHORT_HEAD_SZIE  3
#define MUX_LONG_HEAD_SIZE  4

#define MUX_MCC_VALUE_OCTETS_NUM_MAX  8
#define MUX_CONMAND_SHORTFRAME_SIZE (FCS_SIZE+2*FLAG_SIZE+MUX_SHORT_HEAD_SZIE)
#define MUX_COMMAND_LONGFRAME_SIZE (FCS_SIZE+2*FLAG_SIZE+MUX_LONG_HEAD_SIZE)
#define MUX_MMC_HEAD_SIZE  3
#define MUX_MMC_SIZE_MAX (MUX_MMC_HEAD_SIZE +MUX_MCC_VALUE_OCTETS_NUM_MAX)
#define MUX_SHORTFRAME_INF_MAX 127
#define MUX_LONGFRAME_INF_MAX 32767
#define GET_LENGTH_FIELD(a) (*(a+MUX_SHORT_HEAD_SZIE))
#define GET_LENGTH_H_FIELD(a) (*(a+MUX_SHORT_HEAD_SZIE+1))

#define GET_PARAMETER_LENGTH_FIELD(a) (*(a))
#define GET_PARAMETER_LENGTH_H_FIELD(a) (*(a+1))

#define GET_PN_MSG_FRAME_SIZE(pn) ( ((pn)->frame_sizeh << 8) | ((pn)->frame_sizel))
#define SET_PN_MSG_FRAME_SIZE(pn, size) \
            (pn)->frame_sizel = (size) & 0xff; \
            (pn)->frame_sizeh = (size) >> 8;


#define GET_LONG_LENGTH(a) ( ((a).h_len << 7) | ((a).l_len) )
#define SET_LONG_LENGTH(a, length) \
            (a).ea = 0; \
            (a).l_len = length & 0x7F; \
            (a).h_len = (length >> 7) & 0xFF; 

#define GET_SHORT_LENGTH(a) (a>>1 )
#define SET_SHORT_LENGTH(a, length) \
            (a).ea = 1; \
            (a).l_len = length; \
            (a).h_len = 0X00;

#define SET_MMC_TYPE(type,cr,ea ) (type&(~(cr<1))&(~ea))

#define MUX_MAX_HDR_SIZE 5                
#define MUX_MTU_DEFAULT 3000//1600 MS00146101  /* the max size of the information in mux frame */

#define MUX_BASIC_FLAG 0xF9
    /* the type field in a multiplexer command packet */
#define MUX_PN 0x20 //DLC parameter negotiation (PN)
#define MUX_PSC 0x10  //Power Saving Control (PSC)
#define MUX_CLD 0x30 //Multiplexer close down (CLD)
#define MUX_TEST 0x8 //Test Command (Test)
#define MUX_FCON 0x28 //Flow Control On Command (FCon)
#define MUX_FCOFF 0x18 //Flow Control Off Command (FCoff)
#define MUX_MSC 0x38 //Modem Status Command (MSC)
#define MUX_NSC 0x4 //Non Supported Command Response (NSC)
#define MUX_RPN 0x24 //Remote Port Negotiation Command (RPN)
#define MUX_RLS 0x14 //Remote Line Status Command(RLS)
#define MUX_SNC 0x34//Service Negotiation Command (SNC)


    /*MESSAGE TYPE operation*/
#define GET_MCC_TYPE(a) (*a)
#define GET_MMC_LENGTH_FIELD(a) (*(a+1))
#define GET_MMC_LENGTH_H_FIELD(a) (*(a+2))

    /*PN operation*/
#define GET_MCC_PN_DLCI(a)  (*a)
#define GET_MCC_PN_L(a)        ((*a))
#define GET_MCC_PN_CL(a)      ()
#define GET_MCC_PN_P(a)        ()
#define GET_MCC_PN_T(a)        ()
#define GET_MCC_PN_N1(a)     ()
#define GET_MCC_PN_N2(a)     ()
#define GET_MCC_PN_K(a)        ()


#define MUX_CTRL_CHAN 0/* The control channel is defined as DLCI 0 */
#define MUX_CTRL_LINK     MUX_CTRL_CHAN /*General, link0 is mux control link*/
#define MUX_MCC_CMD 1/* Multiplexer command cr */
#define MUX_MCC_RSP 0/* Multiplexer response cr */


    /*DLC->work_mode*/
#define MUX_DLC_AT_MODE  0x01 /*default value*/
#define MUX_DLC_DATA_MODE  0x02


// DLC->service:  Service type
#define MUX_DLC_SERVICE_NULL		0
#define MUX_DLC_SERVICE_MASK		0x03
#define MUX_DLC_SERVICE_DATA		0x01
#define MUX_DLC_SERVICE_VOICE		0x02

// DLC_codec: codec type
#define MUX_DLC_CODEC_NULL		0
#define MUX_DLC_CODEC_MASK		0x3F
#define MUX_DLC_CODEC_3GPPTS46021	0x01	// V1: Voice (coded - 3GPP TS 46.021)
#define MUX_DLC_CODEC_PCM64U			(1<<1)	// V2: Voice (coded - PCM 64 kbit/s U-law)
#define MUX_DLC_CODEC_ADPCM32		(1<<2)	// V3: Voice (coded ADPCM 32kbit/s)
#define MUX_DLC_CODEC_HALFRATE		(1<<3)	// V4: Voice (coded halfrate)
#define MUX_DLC_CODEC_PCM64A			(1<<4)	// V5: Voice (coded - PCM 64kbit/s A-law)
#define MUX_DLC_CODEC_PCM128			(1<<5)	// V6: Voice (coded PCM 128kbit/s)
                                          
    /*DLC->open_wait_evnet and close_wait_event*/
#define MUX_OPEN_WAIT_EVENT 1
#define MUX_CLOSE_WAIT_EVENT (1<<1)

#define MUX_ALLOC_DLCI  g_mux_alloc_dlc++;\
    if(MUX_MAX_DLC_NUM == g_mux_alloc_dlc ) g_mux_alloc_dlc=1;


#define SHORT_CRC_CHECK 3
#define LONG_CRC_CHECK 4

#define            MUX_MODE_CHANGE_DELAY 1000 // 1000 milliseconds

#define MUX_AT_CMUX_LEN_MAX  60/*command character number of 
                                                         AT+CMUX=<mode>[,<subset>[,
                                                            <port_speed>[,<N1>[,<T1>
                                                            [,<N2>[,<T2>[,<T3>
                                                            [,<k>]]]]]]]]*/

    /*value for g_mux_sys_info.cmdres_code*/
#define MUX_ATPlusCMUX_RET_SUCCESS  0x00
#define MUX_ATPlusCMUX_RET_FAIL   0x01
#define MUX_ATPlusCMUX_RET_INVALID  0xFF


#define MUX_ADVANCE_FLAG 0x7E
#define MUX_MAX_ADVANCE_HDR_SIZE  3 /*To Basic mode, MUX_MAX_HDR_SIZE is 5.*/
#define MUX_ESCAPE_FLAG 0x7D
#define MUX_COMPLEMENT_BYTE 0x20


    /*------------------------------------------------------------------------------
    struct :  the system config struct define
    ------------------------------------------------------------------------------*/
    typedef enum
    {
        MUX_SIDE_UE                         = 0,
        /*MUX work in UE side*/
        MUX_SIDE_TE
        /*MUX work in TE side*/
    }       MUX_SIDE_E;


    /*whether the MUX start-up or not*/
    typedef enum
    {
        MUX_STATUS_OFF              = 0,
        MUX_STATUS_ON,
        MUX_STATUS_ON_REQ,
        MUX_STATUS_OFF_REQ
    }       MUX_START_E;


    /*
    The multiplexer has three operating options, basic, advanced without error recovery and advanced 
    with error recovery. The characteristics of the options are:
    Basic:
    -   length indicator used instead of the HDLC transparency mechanism;
    -   different flag octet from that used by HDLC;
    -   can not be used on links which use XON/XOFF flow control;
    -   may have longer recovery procedure from loss of synchronisation.
    Advanced without error recovery:
    -   asynchronous HDLC procedures in accordance with ISO/IEC 13239;
    -   can be used on links which use XON/XOFF flow control;
    -   recovers quickly from loss of synchronisation.
    Advanced with error recovery:
    -   Uses HDLC error-recovery procedures.
    */
    typedef enum
    {
        MUX_OPTION_BASIC                    = 0,
        MUX_OPTION_ADVANCED,
        MUX_OPTION_ADVANCED_ERM
    }       MUX_STATUS_E;

    /*Table 4: Meaning of I-bits
    MeaningI1I2I3I4
    Use UIH frames0000
    Use UI frames1000
    Use I frames (note)0100
    NOTE:Refer to clause 6.
    */
    typedef enum
    {
        MUX_FRAME_UIH               = 0,
        MUX_FRAME_UI,
        MUX_FRAME_I,
        MUX_FRAME_UNKNOWN
    }       MUX_FRAME_E;

    /*Table 12: Meaning of B-bits
    MeaningB1B2B3B4B5B6B7B8
    2 400 bit/s00000000
    4 800 bit/s10000000
    7 200 bit/s01000000
    9 600 bit/s11000000
    19 200 bit/s00100000
    38 400 bit/s10100000
    57 600 bit/s01100000
    115 200 bit/s11100000
    230 400 bit/s00010000
    */
    typedef enum
    {
        //MUX_BAUDRATE_2400 = 0,
        // MUX_BAUDRATE_4800,
        // MUX_BAUDRATE_7200,
        MUX_BAUDRATE_9600  =1,
        MUX_BAUDRATE_19200,
        MUX_BAUDRATE_38400,
        MUX_BAUDRATE_57600,
        MUX_BAUDRATE_115200,
        MUX_BAUDRATE_230400,
        MUX_BAUDRATE_RESERVED
    }       MUX_BAUDRATE_E;


    typedef struct
    {
        uint8   addrss;
        uint8   control;
        uint8   reserved[2];
        uint32  length;//advance mode, length is valid data length excepte EA*/
        uint8  *data_ptr;
    }MUX_FRAME_T;

    /*
    system_parameters   = Port speed [9,6 | 19,2 | 38,4 | 57,6 | 115,2 |230,4 kBit/s],
    Maximum Frame Size [1 - 128 in Basic mode, 
                                      1 - 512 in HDLC modes   
                                      default: 31 for the basic option and 64 for the advanced option]
    Acknowledgement Timer [0,01s-2,55s, default: 0,1s]
    Maximum number of retransmissions [0 - 100, default : 3]
    Response timer for the multiplexer control channel  [0,01s-2,55s, default: 0,3s]
    Wake up response timer [1s - 255s, default 10s]
    Window size for error recovery mode [1 - 7, default : 2]
    */
    typedef struct
    {
        MUX_STATUS_E    mux_mode;//MUX work mode: basic, advance, advance with ERM
        MUX_FRAME_E     subset;//Used Information frame type: UIH, UI,I. Default: UIH.
        MUX_BAUDRATE_E  port_speed;//transmission rate, default: 115 200 bit/s
        uint32          N1;// maximum frame size, range: 1-1- 32768 default Value : 31
        uint8           T1;//acknowledgement timer in units of ten milliseconds. Default: 10  (100ms)
        uint8           N2;// maximum number of re-transmissions,range:(0-100);default:3.
        uint8           T2;// response timer for the multiplexer control channel in units of ten milliseconds, range:2-255, default: 30 (300ms)
        uint8           T3;// wake up response timer in seconds. Range: 1-255, default :10
        uint8           K;// window size, for Advanced operation with Error Recovery options, Range:1-7,default:2
        uint8           reserve[3];
    } MUX_SYS_PARA_T;



    typedef struct
    {
        MUX_START_E     is_start;//MUX start flag: ON/OFF 
        MUX_SIDE_E      side; //MUX work end flag the mux is MUX/TE or MUX/UE
        uint32          cmdres_wait_flag;//startup MUX event flag.
        uint32          cmdres_code;/*Response of AT+CMUX 0x00: SUCCESS, 0x01: FAILURE*/
        MUX_SYS_PARA_T  sys_parameter;//configured by AT+CMUX command.
        uint8          ctl_link;
        uint8          mux_index; //Just for print convenience
        uint8          reserve1[2];

        //RECV TASK info
        MUX_EVENT_GROUP_PTR   g_MuxRecv_Eptr;
        uint32     mux_recvtask_event;
        uint32     mux_recv_task_id;
        MUX_EVENT_GROUP_PTR   g_MuxEvent_ptr;

        //Parse MUX frame info
        unsigned char *mux_rev_buf;
        unsigned char * end_ptr;
        unsigned char * start_ptr;
        int                    framelen;

        BOOLEAN   adv_escape_flag; // Only used in advance  mode
        BOOLEAN   adv_start_flag;//Only used in advance mode
        BOOLEAN   TE_wait_OK; //special for OK of response to AT+CMUX
        uint8          reserve;        
    }MUX_INFO_T;

    /*------------------------------------------------------------------------------
    struct :  the DTI  config struct define
    ------------------------------------------------------------------------------*/
    typedef void (*MUX_CALLBACK)(uint8 link_id, uint32 data_length, uint8 *data_ptr);

    typedef struct
    {
        MUX_CALLBACK    entity_recv_func;//entity send data to peer
    }MUX_ENTITY_T;


    /*------------------------------------------------------------------------------
    struct :  the DLC config struct define
    ------------------------------------------------------------------------------*/

#if !defined(__BIG_ENDIAN) && !defined(__BigEndian)	/*Used little endian system*/
    /* PN-command */
    typedef struct
    {
        uint8   frame_type  : 4;//Used Information frame type: UIH, UI,I. Default: UIH.
        uint8   credit_flow : 4; 

        uint8   prior       : 6;
        uint8   res2        : 2; 


        uint8   ack_timer;//T1:acknowledgement timer in units of ten milliseconds. Default: 10  (100ms)
        uint8   frame_sizel; //N1 maximum frame size, range: 1-1- 32768 default Value : 31
        uint8   frame_sizeh;
        uint8   max_nbrof_retrans;//N2 maximum number of re-transmissions,range:(0-100);default:3.
        uint8   credits; //K: window size, for Advanced operation with Error Recovery options, Range:1-7,default:2
    }MUX_PN_DATA_T;

    typedef struct
    {
        uint8           dlci    : 6;
        uint8           res1    : 2;
        MUX_PN_DATA_T   pn_val;
    } MUX_PN_STRUCT_T;

    /* MSC-command */
    typedef struct
    {
        uint8   ea          : 1;
        uint8   fc          : 1;    
        uint8   rtc         : 1;
        uint8   rtr         : 1;
        uint8   reserved    : 2;
        uint8   ic          : 1;
        uint8   dv          : 1;
    }MUX_V24_SIGNAL_T;


    typedef struct
    {
        uint8   ea      : 1;
        uint8   cr      : 1;
        uint8   dlc_chn : 6;
    } MUX_ADDRESS_FIELD;

    typedef struct
    {
        uint8   ea      : 1;
        uint8   l_len   : 7;
        uint8   h_len;
    }MUX_LENGTH_STURCT;

    typedef struct
    {
        uint8   ea  : 1;
        uint8   len : 7;
    } MUX_SHORT_LENGTH;

    typedef struct
    {
        uint8   ea      : 1;
        uint8   l_len   : 7;

        uint8   h_len;
    }MUX_LONG_LENGTH;

    typedef struct
    {
        uint8   ea      : 1;
        uint8   cr      : 1;    
        uint8   type    : 6;
    } MUX_MCC_TYPE;

    typedef struct
    {
        uint8   ea      : 1;
        uint8   svc     : 7;         // S1: Date;  S2: Voice
    }MUX_SNC_SVC_TYPE;

    typedef struct
    {
        uint8   ea      : 1;
        uint8   type   : 7;         // V1: Voice (coded - 3GPP TS 46.021)
                                          // V2: Voice (coded - PCM 64 kbit/s U-law)
                                          // V3: Voice (coded ADPCM 32kbit/s)
                                          // V4: Voice (coded halfrate)
                                          // V5: Voice (coded - PCM 64kbit/s A-law)
                                          // V6: Voice (coded PCM 128kbit/s)
    }MUX_SNC_VOICE_CODEC;
#else/*#ifdef WIN32*/
    /* PN-command */
    typedef struct
    {
        uint8   credit_flow : 4; 
        uint8   frame_type  : 4;//Used Information frame type: UIH, UI,I. Default: UIH.

        uint8   res2        : 2; 
        uint8   prior       : 6;


        uint8   ack_timer;//T1:acknowledgement timer in units of ten milliseconds. Default: 10  (100ms)
        uint8   frame_sizel; //N1 maximum frame size, range: 1-1- 32768 default Value : 31
        uint8   frame_sizeh;
        uint8   max_nbrof_retrans;//N2 maximum number of re-transmissions,range:(0-100);default:3.
        uint8   credits; //K: window size, for Advanced operation with Error Recovery options, Range:1-7,default:2
    }MUX_PN_DATA_T;

    typedef struct
    {
        uint8           res1    : 2;
        uint8           dlci    : 6;
        MUX_PN_DATA_T   pn_val;
    } MUX_PN_STRUCT_T;

    /* MSC-command */
    typedef struct
    {
        uint8   dv          : 1;
        uint8   ic          : 1;
        uint8   reserved    : 2;
        uint8   rtr         : 1;
        uint8   rtc         : 1;
        uint8   fc          : 1;
        uint8   ea          : 1;
    }MUX_V24_SIGNAL_T;


    typedef struct
    {
        uint8   dlc_chn : 6;
        uint8   cr      : 1;
        uint8   ea      : 1;
    } MUX_ADDRESS_FIELD;

    typedef struct
    {
        uint8   l_len   : 7;
        uint8   ea      : 1;
        uint8   h_len;
    }MUX_LENGTH_STURCT;

    typedef struct
    {
        uint8   len : 7;
        uint8   ea  : 1;
    } MUX_SHORT_LENGTH;

    typedef struct
    {
        uint8   l_len   : 7;
        uint8   ea      : 1;
        uint8   h_len;
    }MUX_LONG_LENGTH;

    typedef struct
    {
        uint8   type    : 6;
        uint8   cr      : 1;
        uint8   ea      : 1;
    } MUX_MCC_TYPE;

    typedef struct
    {
        uint8   svc     : 7;         // S1: Date;  S2: Voice
        uint8   ea      : 1;
    }MUX_SNC_SVC_TYPE;

    typedef struct
    {
        uint8   type   : 7;         // V1: Voice (coded - 3GPP TS 46.021)
                                          // V2: Voice (coded - PCM 64 kbit/s U-law)
                                          // V3: Voice (coded ADPCM 32kbit/s)
                                          // V4: Voice (coded halfrate)
                                          // V5: Voice (coded - PCM 64kbit/s A-law)
                                          // V6: Voice (coded PCM 128kbit/s)
        uint8   ea      : 1;
    }MUX_SNC_VOICE_CODEC;

#endif/*#ifdef WIN32*/

    /*MUX Buffer is Ring type*/
    typedef struct
    {
        uint32  size;
        uint32  start_point;
        uint32  end_point;
        uint8  *data_ptr;
    }MUX_BUF_T ;

    //convergence layer type
    typedef enum
    {
        MUX_CONVERGENCE_TYPE_NULL,
        MUX_CONVERGENCE_TYPE_ONE,
        MUX_CONVERGENCE_TYPE_TWO,
        MUX_CONVERGENCE_TYPE_THREE,
        MUX_CONVERGENCE_TYPE_FOUR
    }       MUX_CONVERGENCE_TYPE_E;

    /* DLC ->used_state: the main state referr to 27.010*/
    typedef enum
    {
        MUX_DLC_STATE_DISCONNECT                                        = 0,
        MUX_DLC_STATE_CONNECTING,
        /*from send SABM to receive UA*/
        MUX_DLC_STATE_CONNECT,
        /*already establish success, DLC can transfer data*/
        MUX_DLC_STATE_DISCONNECTING,
        /*from send Disc to receive UA*/
        MUX_DLC_STATE_REJECTED,
        /*receive DM for issue SABM or DISC*/
        MUX_DLC_STATE_FLOW_STOPPED,
        MUX_DLC_STATE_NEGOTIATING,
        /*PN message*/   
        MUX_DLC_STATE_RESERVED
        /*Maybe add new state for DLC.e.g. Flowcontrol*/
    }       MUX_DLCI_STATE_T;    

    typedef  struct
    {
        uint8               dlci;
        uint8               work_mode;//DLC work mode: AT mode or Data mode;
        uint8               from; //dlci establish direction
        uint8               reserved;
        MUX_DLCI_STATE_T    used_state; //DLC establish/release status.
        MUX_V24_SIGNAL_T    v24_status;//Refer to MSC message.
        MUX_PN_DATA_T       param;//configured by PN Message.
        uint8               res;//size of  MUX_PN_DATA_T is 7.need a byte to ensure 4 bytes
        uint32              open_wait_event;
        uint32              close_wait_event;    
        MUX_BUF_T           rx_buf;//Used to store entire MUX Frame,in current version. not used
        struct com_line    *p_line;//Used for MPPP
        MUX_TIMER_PTR       mux_plus_timer;// for process three plus+++
        MUX_CALLBACK        common_recv_func;//In UE side, it means ATC recv call back function
        MUX_ENTITY_T        entity;
        uint8		        cid;		// For PPP, use this channel for data transfer when cid is matchs the one used in ppp
        uint8               service;	// For SNC, recode the current channel type
        uint8               codec;		// For SNC, recode the current codec type
    }MUX_DLC_T;


    /*------------------------------------------------------------------------------
    struct : MUX frame struct define
    ------------------------------------------------------------------------------*/ 
    typedef struct
    {
        MUX_ADDRESS_FIELD   addr;
        uint8               control;
        MUX_LENGTH_STURCT   length;
    }MUX_FRAME_HEAD;

    typedef struct
    {
        MUX_MCC_TYPE        type;
        MUX_SHORT_LENGTH    length;
    } MUX_MCC_SHORT_FRAME_HEAD_T;

    typedef struct
    {
        MUX_MCC_SHORT_FRAME_HEAD_T  h;
        uint8                       value[1];
    }  MUX_MCC_SHORT_FRAME_T;

    typedef struct
    {
        MUX_MCC_TYPE    type;
        MUX_LONG_LENGTH length;
    } MUX_MCC_LONG_FRAME_HEAD_T;

    typedef struct
    {
        MUX_MCC_LONG_FRAME_HEAD_T   h;
        uint8                       value[1];
    } MUX_MCC_LONG_FRAME_T;

    typedef struct
    {
        MUX_ADDRESS_FIELD   dlci;
        MUX_V24_SIGNAL_T    v24_sigal;
        //  MUX_BREAK_SIGNAL break_signals;
    }MUX_MSC_STRUCT;

    typedef struct
    {
        MUX_ADDRESS_FIELD   dlci;
        MUX_SNC_SVC_TYPE    svc;
        MUX_SNC_VOICE_CODEC codec;
    }MUX_SNC_STRUCT_T;

    typedef struct
    {
        MUX_ADDRESS_FIELD   addr;
        uint8               control;
    }MUX_ADVANCE_FRAME_HEAD_T;

    /*------------------------------------------------------------------------------
    struct :  MUX comon return value
    ------------------------------------------------------------------------------*/

    /*MUX Return value*/
    typedef enum
    {
        MUX_RET_SUCCESS                                                             = 0,
        MUX_RET_FAILURE,
        MUX_RET_LINK_NUM_ERR,
        /*input link is invalid*/
        MUX_RET_WORK_MODE_ERR,
        /*MUX SYS work mode error*/
        MUX_RET_NOT_STARTUP,
        /*MUX mode is not startup, sys is in serial port mode*/
        MUX_RET_ALREADY_STARTUP,
        /* MUX has startup already, startup again*/
        MUX_RET_DLC_STATUS_ERR,
        /*DLC status error,eg.DLC in disconnect status, NOT receive data*/
        MUX_RET_DLC_MODE_ERR
        /*DLC work mode is error */
    }       MUX_RETURN_E;

    /*MUX signal enum*/
    typedef enum
    {
        MUX_SIG_NULL,
        MUX_RECEIVE_DATA_FROM_DRIVER,
        MUX_SIG_MAX
    }                   MUX_SIG_E;

    /*------------------------------------------------------------------------------
    Defination :MUX function
    ------------------------------------------------------------------------------*/
    PUBLIC void             MUX_Init(void);
    void                    MUX_TraceData(unsigned char *data, uint32 datalen);
    uint32                  MUX_Crc_Check(uint8 *data, uint32 length, uint8 check_sum);
    uint8                   MUX_Crc_Calc(uint8 *data, uint32 length);
    MUX_RETURN_E            MUX_FlushDlci(uint8 link_id);
    MUX_RETURN_E            MUX_DlcSetPara(MUX_PN_DATA_T *para_ptr, MUX_PN_DATA_T *new_para_ptr);
    void                    MUX_Default_ATC_Callback(uint8 link_id, uint32 len, uint8 *data_ptr);
    uint8 MUX_Get_Linkid(uint8 dlci,  uint8  from);
    uint8 MUX_Get_AllLinkid(uint8 dlci,  uint8  from);
        
    uint8                   MUX_Valid_Dlci(uint8 dlci);

    MUX_RETURN_E MUX_Get_NewLinkid(uint8 *link_id, uint8 from);
    
    void                    MUX_Check_ThreePlus(uint32 link_id);
    MUX_RETURN_E            MUX_Forward_Up(uint8 link_id, uint32 len, uint8 *data_ptr);
   
    PUBLIC MUX_RETURN_E MUX_Upon_Close(MUX_INFO_T *mux_ptr);
    
    PUBLIC  MUX_RETURN_E    MUX_Send_UIH(MUX_INFO_T *config_data, uint8 *data_ptr, uint32 data_len, uint8 dlci);
    PUBLIC MUX_RETURN_E     MUX_UIH_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr);
    PUBLIC MUX_RETURN_E     MUX_DISC_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr);
    PUBLIC MUX_RETURN_E     MUX_DM_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr);
    PUBLIC MUX_RETURN_E     MUX_UA_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr);
    PUBLIC MUX_RETURN_E     MUX_SABM_Handler(MUX_FRAME_HEAD *mux_pkt_head_ptr, uint8 *inf_data_buff, MUX_INFO_T *mux_ptr);

    MUX_RETURN_E MUX_Parse_Atcmd_Res(uint32 len, uint8 *data_ptr, MUX_INFO_T *mux_ptr);
    MUX_RETURN_E            MUX_Assemble_CMUX_CMD(MUX_SYS_PARA_T *para, char *output_ptr, uint8 *output_len, uint8 times);

MUX_RETURN_E  MUX_Rev_Buf_Init(MUX_INFO_T *mux_ptr);

    PUBLIC void MUX_RecvTaskInit(void);

    /*------------------------------------------------------------------------------
    Defination :MUX var
    ------------------------------------------------------------------------------*/

    extern MUX_INFO_T   g_mux_sys_info[MAX_MUX_NUM];/*mux sys info*/

    extern MUX_DLC_T    g_mux_dlc_info[MUX_MAX_LINK_NUM];/*DLC info*/

    /*Use in MUX RECV TASK, to save data from Driver*/
    /*MUX_BUF_T mux_rx_buf;*//*Instead by mux_rev_buf in Mux_Data_Parse_Basic*/


    extern uint8        g_mux_alloc_dlc;//Used in allocate DLCI,range 1-63, 0 is reserved for CTRL.

    /*common public ATC process function*/
    extern MUX_CALLBACK g_mux_common_recv_func ;

    extern MUX_ENTITY_T g_mux_data_common_recv_func[MUX_MAX_LINK_NUM];

#ifdef   __cplusplus
}
#endif
#endif/*_MUX_ALL_H*/



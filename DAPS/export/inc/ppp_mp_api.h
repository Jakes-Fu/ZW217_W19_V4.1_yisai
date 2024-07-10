/*******************************************************************************
 ** File Name:      ppp_mp_api.h                                              *
 ** Author:         Zhang Ziyi                                                 *
 ** Date:           2012-6-16                                                  *
 ** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.          *
 ** Description:    PPP module API functions                                   *
 **                 other ATC modules.                                         *
 ******************************************************************************/
#ifndef PPP_MP_API_H_
#define PPP_MP_API_H_

#include "os_api.h"
#include "tcpip_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PPP_E_SUCCESS 0
#define PPP_E_NO_MEM  (-1)                   /**<error code no memory */
#define PPP_E_ERROR (-1000)

#define PPP_INVALID_MEM_HANDLE (0)

#define PPP_GEN_FRAME_BUF_SIZE 4096
#define PPP_FRAME_FIFO_SIZE  32

#define PPP_MAX_DROP_PACKET_SIZE (1600)
#define PPP_CHECKING_NONE (0)
#define PPP_CHECKING_FIND_START_SYNC (1)
#define PPP_CHECKING_FIND_FF (2)
#define PPP_CHECKING_FIND_03 (3)
#define PPP_CHECKING_FIND_FF_ESCAPED (4)
#define PPP_CHECKING_FIND_03_ESCAPED (5)
#define PPP_CHECKING_FIND_END_SYNC (6)

#define PPP_LCP_ECHOMAXFAILS   6
#define PPP_LCPECHOTIMER_STOPFLAG  0xffffffff

 /** PPP mode , Titan is only under Server mode*/
typedef enum PPP_Mode
{
  PPP_CLIENT                     = 0,           /**<start PPP as client            */
  PPP_SERVER,           				/**<start PPP as server            */
  PPP_TRANSPARENT         				/**<start PPP in transparent mode  */
} PPP_Mode_e;

/** PPP MRU , Maximum-Receive-Unit*/
typedef enum PPP_MRU_E
{
  PPP_MRU_DEFAULT = 1500,                          /**<default value for MRU          */
} PPP_MRU_e;

/** PPP restart timer */
typedef enum PPP_Rt
{
  PPP_RT_DEFAULT = 3                              /**<default value for restart timer */
} PPP_Rt_e;

/** PPP max configure */
typedef enum PPP_Mc
{
  PPP_MC_DEFAULT = 10                             /**<default value for max configure */
} PPP_Mc_e;

/** PPP max terminate */
typedef enum PPP_Mt
{
  PPP_MT_DEFAULT = 2                              /**<default value for max terminate */
} PPP_Mt_e;

/** PPP max failure */
typedef enum PPP_Mf
{
  PPP_MF_DEFAULT = 5                              /**<default value for max failure  */
} PPP_Mf_e;

/** PPP Authentication Protocol */
typedef enum PPP_Ap
{
  PPP_AP_PAP = 0,                                 /**<password authentication protocol */
  PPP_AP_CHAP = 1,                                /**<challenge handshake authentication protocol */
  PPP_AP_AUTO = 2,                                 /**<automatic authentication       */
  PPP_AP_NO = 3                                     /**<no authentication              */
} PPP_Ap_e;

#define PPP_PPP_LOGIN_NAME_LENGTH 25
#define PPP_DEFAULT_MRU  1500

/** Modify PPP AP Mode**/
#define PPP_AP_MODE  PPP_AP_AUTO
#define PPP_LOGIN_NAME       "card"
#define PPP_LOGIN_PASSWORD   "1234"

typedef struct PPP_Login
{
  uint8                        NameLen;                 /* length of name                                     */
  uint8                        Name[PPP_PPP_LOGIN_NAME_LENGTH]; /* login name                                         */
  uint8                        PasswordLen;             /* length of password                                 */
  uint8                        Password[PPP_PPP_LOGIN_NAME_LENGTH]; /* login password                                     */
} PPP_Login_t;

/** PPP Async Control Character Map */
typedef enum PPP_Accm
{
  PPP_ACCM_DEFAULT               = -1,                /**<default value for accm         */
  PPP_ACCM_OFF                   = 0                  /**<disable control character mapping */
} PPP_Accm_e;

typedef struct PPP_ConfigParam
{
  PPP_Ap_e                  Ap;                       /**<Authentication Protocol      */
  uint8                         echo_fails;
  uint32                        echo_interval;
} PPP_ConfigParam_t;

typedef enum
{
	PPP_PDP_IPV4_TYPE	= 0,
	PPP_PDP_IPV6_TYPE,
	PPP_PDP_IPV4V6_TYPE,
	PPP_PDP_ALL_TYPE,
	PPP_PDP_INVALID_TYPE
}PPP_PdpType_e;

#define SIZE_IPV4_ADDR		(4)
#define SIZE_IPV6_ADDR		(16)

typedef struct
{
 uint8 Addr4[SIZE_IPV4_ADDR];
}IPV4_TYPE_t;

typedef struct
{
  uint8 Addr6[SIZE_IPV6_ADDR];
}IPV6_TYPE_t;

typedef union
{
  IPV4_TYPE_t	IPV4_Addr;
  IPV6_TYPE_t    IPV6_Addr;
}IP_Address_u;

  /** PPP IP Parameters*/
typedef struct PPP_MP_IP
{
  PPP_PdpType_e    CtrlIP_Address;                 /**<IP version */  
  IP_Address_u        IP_Address;                     /**<IP address */
} PPP_IP_t;

typedef void* PPP_LinkHandle_t;

/* PPP -> L4 */
typedef enum PPP_SignallingEvent
{
  PPP_SIG_ESTABLISH_CNF,	//PPP NCP success/failure
  PPP_SIG_PDP_ACTIVATE_REQ,    // Host IPCP Config-Req
  PPP_SIG_MO_TERMINATE_REQ,  // Host request to hang up
  PPP_SIG_TERMINATED,          // PPP link terminated
} PPP_SignallingEvent_e;

typedef struct
{
  PPP_PdpType_e	pdp_type;
}PPP_SIG_PDP_ACTIVATE_REQ_T;

typedef struct
{
  PPP_PdpType_e	pdp_type;
  uint32 nsapi[PPP_PDP_ALL_TYPE];
}PPP_SIG_PDP_DEACTIVATE_REQ_T;

typedef struct PPP_EstablishReq
{
  SIGNAL_VARS
  PPP_Mode_e                Mode;                     /**<Working Mode                         */
  uint16                      MRU;                      /**<Maximum Receive Unit */
  uint8                        Rt;                       /**<Restart Timer Value          */
  uint8                        Mc;                       /**<Max Configure                */
  uint8                        Mt;                       /**<Max Terminate                */
  uint8                        Mf;                       /**<Max Failure                  */
  PPP_Ap_e                  Ap;                       /**<Authentication Protocol      */
  uint32                     Accm;                     /**<Async Control Character Map  */
}PPP_EstablishReq_t;


/*  L4 -> PPP */
typedef enum
{
  PPP_AAL_ESTABLISH_REQ,
  PPP_AAL_TERMINATE_IND,
  PPP_AAL_PDP_ACTIVATE_RSP,
  PPP_AAL_PDP_ACTIVATE_REJ,
  PPP_AAL_DATA_REQ,
  PPP_AAL_TIMEOUT_REQ,
  PPP_PEER_KILLED,
}PPP_ATC_Signal_e;

typedef struct
{
  SIGNAL_VARS
  int     result;
  PPP_PdpType_e pdp_type;
  uint32 nsapi[PPP_PDP_ALL_TYPE];
}PPP_AAL_TERMINATE_RSP_T;

/** PPP Lower Layer Status*/
typedef enum PPP_LowerLayer
{
  PPP_LOWER_LAYER_DOWN,           /**<lower layer is not available   */
  PPP_LOWER_LAYER_UP              /**<lower layer is available       */
} PPP_LowerLayer_e;

/** PPP Terminate Req Parameters */
typedef struct PPP_TerminateReq
{
  SIGNAL_VARS
  uint16 usCid;
  uint8 nsapi;
  PPP_PdpType_e pdp_type;
  uint32 term_cause;
} PPP_TerminateReq_t;

typedef struct
{
  uint32 pdp_addr;
  uint32 pri_dns_addr;
  uint32 sec_dns_addr;
  uint32 gateway;
}PPP_Ipv4PdpParms;

typedef struct
{
  uint8 pdp_addr[16];
  uint16 compress;
}PPP_Ipv6PdpParms;

typedef struct
{
  SIGNAL_VARS
  uint8 nsapi;
  struct
  {
    PPP_Ipv4PdpParms Ipv4Parms;
    PPP_Ipv6PdpParms Ipv6Parms;
  }PPP_PdpParms; 
}PPP_AAL_PDP_ACTIVATE_CNF_T;


typedef struct
{
  SIGNAL_VARS
  PPP_PdpType_e pdp_type;
}PPP_AAL_PDP_REJECT_CNF_T;


typedef struct
{
  SIGNAL_VARS
  void* pkt;
  uint16 ptype;
}PPP_AAL_SEND_DATA_T;

typedef struct
{
	SIGNAL_VARS
	uint32 code;
}PPP_AAL_TIMEOUT_REQ_T;

typedef struct
{
  uint8 *pFrame;
  uint32 FrameLen;
} PPP_FrameInfo_t;

/*  DSM -> PPP */
typedef enum
{
  PDP_ACTIVATE_REJ, /* PDP Active Fail */
  PDP_DEACTIVE_IND, /* PDP Deactive IND */
}PPP_STOP_CAUSE_e;

typedef enum
{
  TERMINATE_LINK_DISCONNECT, /* Uart or Ucom disconnected */
  TERMINATE_OTHERS, /* others cause */
}PPP_TERMINATE_CAUSE_e;
/*****************************************************************************/
// Description: start PPP task wen power on
// Global : none
// Author : qin.he
/*****************************************************************************/
BLOCK_ID PPP_TaskStart(void);
/*****************************************************************************/
// Description: Async start PPP module
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: ulDeviceId (IN) -- ppp data translate device
//        PARAM2: usCid (IN) -- PDP Context id
//        PARAM3: usSim (IN) -- SIM id
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int PPP_Start(uint32 ulDeviceId, uint16 usCid, uint16 usSim);

/*****************************************************************************/
// Description: Async start PPP module
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: usSim (IN) -- SIM id
//        PARAM2: usCid (IN) -- PDP Context id
//        PARAM3: nsapi (IN) -- pdp nsapi for PDP_DEACTIVE_CNF and PDP_DEACTIVE_IND
//        PARAM4: Cause (IN) -- PPP stop Cause
//        PARAM5: pdp_type (IN) -- pdp type(IPV4/IPv6/IPV4V6) for PDP_ACTIVATE_REJ
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int PPP_Stop(uint16 usSim, uint16 usCid, uint8 nsapi, PPP_STOP_CAUSE_e Cause, PPP_PdpType_e pdp_type);
/*****************************************************************************/
// Description: Async Terminate PPP module when uart or ucom disconnect
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: ulDeviceId (IN) -- ppp data translate device
//        PARAM2: Cause (IN) -- ppp terminate cause
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int PPP_Terminate(uint32 ulDeviceId, PPP_TERMINATE_CAUSE_e Cause);

char PPP_CheckPeerAlive(void); //for bug 1776827

/*****************************************************************************/
// Description: Send PPP negotiation packet to PPP thread.
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: ulDeviceId (IN) -- ppp data translate device
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int32 PPP_Receive_UL_Data(uint32 ulDeviceId, uint32 len, uint8 *data_ptr);
/*****************************************************************************/
// Description: Receivie data from GSM/GPRS network ,translate to PPP packet and send it to PC.
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: pkt_data_ptr (IN) -- packet data info pointer to be received
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int32 PPP_Receive_DL_Data(void *pktt);
/*****************************************************************************/
// Description: send signal to PPP task
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: prim (IN) -- signale data
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int Send_Signal_toPPP(void *prim);

/*****************************************************************************/
// Description: Get PPP link id
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: ulLinkId (OUT) -- pp data translate device
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int PPP_GetLinkId(uint32* ulLinkId);

/*****************************************************************************/
// Description: Config NCP Parameters
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: ip_addr (IN) -- IP address
//        PARAM2: dns1_addr (IN) -- first dns address
//        PARAM3: dns2_addr (IN) -- second dns address
//        PARAM4: gateway_addr (IN) -- gateway address
//        PARAM5: subnet_mask_addr (IN) -- subnet mask address
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int PPP_ConfigNCPParameters(TCPIP_IPADDR_T ip_addr, TCPIP_IPADDR_T dns1_addr, 
      TCPIP_IPADDR_T dns2_addr, TCPIP_IPADDR_T gateway_addr, TCPIP_IPADDR_T subnet_mask_addr);

/*****************************************************************************/
// Description: Set PPP is ndis mode or tethering mode
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: uint8 ndis_mode [IN] -- true: ndis mode, false: tethering mode
//        PARAM2: uint32 cid [IN] -- PDP Context id
//        PARAM3: uint16 port_start [IN] -- NDIS PPP PDN multiplux reserved port start
//        PARAM4: uint16 port_end [IN] -- NDIS PPP PDN multiplux reserved port end
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int PPP_SetPPPMode(uint8 ndis_mode, uint32 cid, uint16 port_start, uint16 port_end);

/*****************************************************************************/
// Description: Get PPP is ndis mode or tethering mode
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: uint8 *ndis_mode [OUT] -- true: ndis mode, false: tethering mode
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int PPP_GetPPPMode(uint8 *ndis_mode);

/*****************************************************************************/
// Description: check input data is PPP PKT
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: uint8 *data_ptr [IN] -- input data
//        PARAM2: uint32 data_len [IN] -- input data len
//        RETURN: TRUE --- ppp data, FALSE --- not ppp data
/*****************************************************************************/
BOOLEAN PPP_CheckIsPppData(uint8 *data_ptr, uint32 data_len);

//check input data is PPP Pkt --- interface from 8910
int prvPppCheck(uint8 *data, uint32 size);

/*****************************************************************************/
// Description: check PPP use PDN multiplex
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: uint16 cid [IN] -- cid
//        PARAM1: uint8  b_ndis [IN] -- ndis multiplux or tethering multiplex with ppp
//        RETURN: TRUE --- can multiplex, FALSE --- can not multiplex
/*****************************************************************************/
BOOLEAN PPP_CheckUseMultiplexPDN(uint16 cid, uint8 b_ndis);

/*****************************************************************************/
// Description: get PPP net id
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        RETURN: 0 --- PPP not start, netid --- ppp net id
/*****************************************************************************/
TCPIP_NETID_T PPP_GetNetId(void);

/*****************************************************************************/
// Description: Config PPP Param
// Global : none
// Author : qin.he
// Note : for unified multi-bearer use
//        PARAM1: PPP_ConfigParam_t* ConfParam_ptr (IN) -- PPP auth config param
//        RETURN: PPP_E_SUCCESS --- success, other faile
/*****************************************************************************/
int PPP_SetConfigParam(PPP_ConfigParam_t* ConfParam_ptr);

/*****************************************************************************/
// Description: open PPP modem service with modem ID and net interface
// Global : none
// Author : qin.he
// Note : PARAM1: nsapi (IN) -- naspi
//        PARAM2: usCid (IN) -- PDP Context id
//        PARAM2: usSim (IN) -- sim id
//        PARAM2: pdp_type (IN) -- PDP type
//        RETURN: TCPIP_ERROR_OK - open modem service succeed; else - fail
/*****************************************************************************/
PUBLIC TCPIP_ERROR_E TCPIP_PPPMP_OpenPppModemService( uint8 nsapi, uint8 usCid );
uint8 PPP_IsPPPProcessing(void);
int PPP_GetCid(void);

#ifdef __cplusplus
}
#endif

#endif  /* !PPP_MP_API_H_ */

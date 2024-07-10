/*************************************************************************/
/*                                                                       */
/*Copyright (c) 2005.12.08  CQCYIT                                       */
/*                                                                       */
/*************************************************************************/
/*************************************************************************/
/*                                                                       */
/* FILE NAME    engineer.hec                      VERSION  3.0.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*                                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*   All the measurement structure used in trace will be declared here.  */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/* NAME             DATE        VERSION   REMARKS                        */
/* Chen Xianliang   2006.03.06  v0.0.1    Created initial version 0.0.1  */
/*************************************************************************/
#ifndef ENGIEER_HET
#define ENGIEER_HET

//L1 message
typedef struct _TIMESLOT_ISCP_T
{
     unsigned char      timeslot;
     char               iscp;        //单位：DBm
} TIMESLOT_ISCP_T;

typedef struct _TIMESLOT_ISCP_ARR_T
{
    unsigned char          timeslot_iscp_number;
    TIMESLOT_ISCP_T        timeslot_iscp_arr[7];
} TIMESLOT_ISCP_ARR_T;

typedef struct _DPCH_RSCP_T
{
     unsigned char          timeslot;
     char           dpch_rscp;    //单位：DBm
} DPCH_RSCP_T;

typedef struct _DPCH_RSCP_ARR_T
{
     unsigned char                   dpch_rscp_number;
     DPCH_RSCP_T             dpch_rscp_arr[7];
} DPCH_RSCP_ARR_T;

typedef struct _TIMING_ADVANCE_T
{
   unsigned short              timing_advance;    //单位：chip
} TIMING_ADVANCE_T;

typedef struct _UE_TX_POWER_T
{
    char          ue_tx_power;       // DBm
} UE_TX_POWER_T;

typedef struct _CCTRCH_SIR_T
{
    char        cctrch_sir;    // DBm
} CCTRCH_SIR_T;

typedef struct _UL_TARGET_T
{
   char       ul_target_sir;  // DBm
} UL_TARGET_T;

typedef struct _TR_BLER_T
{
   unsigned char   tr_channel_id;
   char            tr_channel_bler;   //单位：DBm
}TR_BLER_T;

typedef struct _BLER_ARR_T
{
  unsigned char        tr_bler_number;
  TR_BLER_T            tr_bler_arr[32];
}BLER_ARR_T;

typedef struct _L1_INFO_T
{
  TIMESLOT_ISCP_ARR_T       Timeslot_ISCP;
  DPCH_RSCP_ARR_T           DPCH_RSCP;
  TIMING_ADVANCE_T          Timing_Advance;
  UE_TX_POWER_T             UE_TX_Power;
  CCTRCH_SIR_T              CCTRCH_SIR;
  UL_TARGET_T               Uplink_Target_SIR;
  BLER_ARR_T                BLER;
}L1_INFO_T;

//L2 message

typedef struct _L2_INFO_T
{
  unsigned char            mac_CRCError_statistic;
  
}L2_INFO_T;

//L3 message
typedef struct _SCELL_PCCPCH_RSCP_T
{
    unsigned char  scell_pccpch_rscp;
} SCELL_PCCPCH_RSCP_T;

typedef struct _SCELL_PATHLOSS_T
{
    unsigned char       scell_pathloss;
} SCELL_PATHLOSS_T;

typedef struct _SCELL_CELL_ID_T
{
    unsigned long      scell_cell_id;
} SCELL_CELL_ID_T;

typedef struct _SCELL_CELL_PARAM_ID_T
{
   unsigned char      scell_cell_param_id;
} SCELL_CELL_PARAM_ID_T;

typedef struct _SCELL_UARFCN_T
{
   unsigned short       scell_uarfcn;
} SCELL_UARFCN_T;

typedef struct _SCELL_URA_ID_LIST_T
{
    unsigned char   scell_ura_id_num;
    unsigned short    scell_ura_id[8];
} SCELL_URA_ID_LIST_T;

typedef struct _SCELL_DRX_COEFFICIENT_T
{
    unsigned char   scell_cs_drx_coefficient;
} SCELL_DRX_COEFFICIENT_T;

typedef struct _SCELL_CELL_BARRED_T
{
    BOOLEAN       scell_barred;
} SCELL_CELL_BARRED_T;

typedef struct _SCELL_SINTRASEARCH_T
{
   unsigned char  scell_sintrasearch;
} SCELL_SINTRASEARCH_T;

typedef struct _SCELL_SINTERSEARCH_T
{
   unsigned char   scell_sintersearch;
}SCELL_SINTERSEARCH_T;

typedef struct _SCELL_SSEARCH_HCS_T
{
   long int  scell_ssearchhcs;
} SCELL_SSEARCH_HCS_T;

typedef struct _SCELL_TRESELECTIONS_T
{
   unsigned char  scell_treselections;
}SCELL_TRESELECTIONS_T;

typedef struct _SCELL_HCS_PRIO_T
{
   unsigned short  scell_hcs_prio;
} SCELL_HCS_PRIO_T;

typedef struct _SCELL_QHYSTS_T
{
   unsigned char scell_qhysts;
} SCELL_QHYSTS_T;

typedef struct _SCELL_SRXLEV_T
{
   char  scell_srxlev;
}SCELL_SRXLEV_T;

typedef struct _SCELL_RS_T
{
   unsigned char scell_rs;
} SCELL_RS_T;

typedef struct _SCELL_CARRIER_RSSI_T
{
   char  scell_rssi;
} SCELL_CARRIER_RSSI_T;

typedef struct _SCELL_HS_T
{
   long int  scell_hs;
} SCELL_HS_T;

typedef unsigned short  NCELL_UARFCN_T;
typedef unsigned char   NCELL_PCCPCH_RSCP_T;
typedef unsigned char   NCELL_PATHLOSS_T;
typedef TIMESLOT_ISCP_T NCELL_TIMESLOT_ISCP_T;
typedef char            NCELL_RN_T;
typedef char            NCELL_QOFFSET_T;
typedef unsigned char   NCELL_CARRIER_RSSI_T;

typedef struct _NCELL_PARAM_T
{
    unsigned char           ncell_param_id;
    NCELL_UARFCN_T          ncell_uarfcn;
    NCELL_PCCPCH_RSCP_T     ncell_pccpch_rscp;
    NCELL_PATHLOSS_T        ncell_pathloss;
    unsigned char           iscp_num;
    NCELL_TIMESLOT_ISCP_T   ncell_timeslot_iscp[6];
    NCELL_RN_T              ncell_rn;
    NCELL_CARRIER_RSSI_T    ncell_carrier_rssi;
    NCELL_QOFFSET_T         ncell_qoffset;
} NCELL_PARAM_T;

typedef struct _NCELL_CELL_PARAM_LIST_T
{
    unsigned char   ncell_num;
    NCELL_PARAM_T   ncell_param[6];
} NCELL_CELL_PARAM_LIST_T;

typedef struct _TRCH_INFO_T
{
     unsigned char           trch_id;
     unsigned char           trch_direction; /* 0 ul, 1 dl */
} TRCH_INFO_T;

typedef struct _DTCH_PARAM_T
{
    unsigned char        trch_num;
    TRCH_INFO_T  trch_info[32];
} DTCH_PARAM_T;

typedef struct _PHYCH_INFO_T
{
    unsigned char           phych_id;         /* 1..48 */
    unsigned char           phych_direction;   /* 0 ul, 1 dl */
    unsigned char           timeslot;         /* 0..6 */
    unsigned char           channel_code;      /* cc1-1 ……*/
    unsigned char           midamble_configuration;
    unsigned char           midamble_shift;
} PHYCH_INFO_T;

typedef struct _DPCH_PARAM_T
{
    unsigned char           phych_num;         /* 1..48 */
    PHYCH_INFO_T            phych_info[48];
    unsigned char           repetition_period;  /* 1,2,4,8,16,32,64*/
    unsigned char           repetition_length;
} DPCH_PARAM_T;

typedef struct _L3_INFO_T
{
  SCELL_PCCPCH_RSCP_T        scell_PCCPCH_RSCP;    
  SCELL_PATHLOSS_T           scell_pathloss;
  SCELL_CELL_ID_T            scell_cell_id;
  SCELL_CELL_PARAM_ID_T      scell_cell_param_id;
  SCELL_UARFCN_T             scell_UARFCN;
  SCELL_URA_ID_LIST_T        scell_URA_id_list;
  SCELL_DRX_COEFFICIENT_T    scell_DRX_coefficient;
  SCELL_CELL_BARRED_T        scell_Cell_Barred;
  SCELL_SINTRASEARCH_T       scell_Sintrasearch;
  SCELL_SINTERSEARCH_T       scell_Sintersearch;
  SCELL_SSEARCH_HCS_T        scell_SsearchHCS;
  SCELL_TRESELECTIONS_T      scell_Treselections;
  SCELL_HCS_PRIO_T           scell_HCS_PRIO;
  SCELL_QHYSTS_T             scell_Qhysts;
  SCELL_SRXLEV_T             scell_Srxlev;
  SCELL_RS_T                 scell_Rs;
  SCELL_CARRIER_RSSI_T       scell_Carrier_RSSI;
  SCELL_HS_T                 scell_Hs;
	NCELL_CELL_PARAM_LIST_T		 ncell_param_list;
  DTCH_PARAM_T               delicated_Trch_param;
  DPCH_PARAM_T               delicated_Phych_param;
}L3_INFO_T;

typedef struct _MM_INFO_T
{
  
  unsigned char                    mcc[2];
  unsigned char                    mnc;
  unsigned char                    lac[2];
  unsigned char                    cellIdentifier[2];
  unsigned char                    drxParameter[2];
  unsigned short                   uarfcn;
  unsigned char                    att;
  unsigned short                   t3212Len;//unit:s
}MM_INFO_T;

typedef struct _GMM_INFO_T
{
  
  unsigned char                    rac;
  unsigned char                    nmo;
  unsigned short                   t3302Len;//unit:s
  unsigned short                   t3312Len;//unit:s
}GMM_INFO_T;

typedef struct _NAS_INFO_T
{
  MM_INFO_T      mm_info;
  GMM_INFO_T     gmm_info;
}NAS_INFO_T;
#endif

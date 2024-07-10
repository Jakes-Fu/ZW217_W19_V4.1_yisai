/******************************************************************************
 ** File Name:      ps_eng.h                                                  *
 ** Author:         Fancier fan                                               *
 ** Date:           03/11/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *         
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 03/11/2002     fancier fan      Create.                                   *
 ******************************************************************************/

#if !defined (_PS_INFO_H_)
#define _PS_INFO_H_

#include "sci_types.h"
#include "gsm_gprs.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/* MAX cell number monitored by RRC module */
#define DIAG_CELL_MAX 7
#define SAVE_MSG_SIZE 40
#define SCI_LOG_ASSERT_STATE(str) SCI_LogAssertState(__FILE__,__LINE__,str);
#define MM_MAX_REJ_INFO_NUM     8

typedef struct
{
    uint16           mcc;             
    uint16           mnc;             
    uint16           lac;              
    uint16           cell_id;          
    uint16           pm;               
    unsigned char    pwrc;             
    unsigned char    dtx;              
    uint8            rlt;              
    unsigned char    att;              
    uint8            bsag;             
    uint16           ccch;             
    uint16           cbq2;             
    uint8            bspamf;           
    uint8            maxre;            
    uint8            txint;             
    unsigned char    cba;               
    uint16           re;               
    uint16           ac;               
    uint8            crh;              
    uint8            tpmxcch;          
    uint8            ram;              
    unsigned char    osp_present;      
    uint8            cbp;              
    uint8            cro;              
    uint8            tmpo;             
    uint8            ptm;              
    unsigned char    cbch_present;     
    uint16           cbch_chan;        
    uint8            cbch_tn;               
    uint8            cbch_tsc;              
    unsigned char    cbch_is_hopping;       
    uint8            cbch_maio;             
    uint8            cbch_hsn;              
    uint16           cbch_arfcn;            

} DIAG_SERVING_CELL_INFO_T;

typedef struct
{
    unsigned char  cell_present;                  
    uint16         arfcn;                         
    uint8          bsic;                          
    uint16         lac;                           
    uint8          rxlev;                         
    int16          c1;                            
    int16          c2;                            
    unsigned char  gprs_supported;                
    int16          c31;                           
    int16          c32;                           

} DIAG_CELL_PRIMARY_INFO_T;

typedef DIAG_CELL_PRIMARY_INFO_T DIAG_CELL_PRIMARY_INFO_ARRAY[DIAG_CELL_MAX];

typedef struct
{
	unsigned short SignalCode;
	unsigned short Size;
    DIAG_CELL_PRIMARY_INFO_ARRAY cell_list;    // The first record for serving cell
    DIAG_SERVING_CELL_INFO_T scell_info;      // The detailed information of serving cell
}DIAG_CELL_INFO_LOG_T;

typedef struct
{
    unsigned char    scell_present;     
    uint16           mcc;             
    uint16           mnc;             
    uint16           scell_lac;   
    uint16           scell_arfcn;                         
    uint8            scell_bsic;                          
    uint16           scell_rxlev;                         
    uint16           scell_id;          
    unsigned char    scell_pwrc;             
    unsigned char    scell_dtx;              
    unsigned char    scell_att;              
    uint16           scell_re;               
    uint16           scell_ac;               
    unsigned char    scell_gprs_supported;   
    unsigned char    scell_cbch_present;     

} DIAG_NET_INFO_LOG_T;

typedef struct
{
    unsigned char   cell_present;
    uint16          arfcn;
    uint8           bsic;
    uint16          rssi;
    unsigned char   identity_present;
    uint16          mcc;
    uint16          mnc;
    uint16          mnc_digit_num;
    uint16          lac;
    uint16          cell_id;
    BOOLEAN  fake_cell_flag;

} DIAG_CELL_INFO_T;
#ifdef MODEM_PLATFORM
typedef struct {
    uint16  mcc;
    uint16  mnc;
    uint16  mnc_digit_num;
    uint16  lac;
    uint16  cell_id;
    uint16  arfcn;
    uint32  band;
    uint8   bsic;
    int16   rxlev;
}DIAG_SCANED_CELL_INFO_T;


typedef struct
{
    uint8 cell_num;
    DIAG_SCANED_CELL_INFO_T  scaned_cell_info[SCANED_CELLS_NUM];
}DIAG_SCANED_CELL_INFO_ARR_T;
#endif // MODEM_PLATFORM
typedef struct 
{
    uint32  pos;
    xSignalHeaderRec *msg_ptr[SAVE_MSG_SIZE];
} BASE_MSG_PTR_T;

typedef struct
{
	uint32  tm_year;
	uint32  tm_mon;
	uint32  tm_day;
	uint32  tm_hour;
	uint32  tm_min;
	uint32  tm_sec;
	uint8   domain;
	uint8   cause_sel;
	uint8   rej_cause;
	uint16 mcc;
	uint16 mnc;
	uint16 mnc_digital_num;
} ENG_REJ_INFO_T;

typedef struct ENG_MM_STATUS_INFO_s
{
    uint8   mm_state;
    uint8   update_state;
	uint8   lu_state;
	uint8   att_state;
    uint8   tmsi_valid;
	uint8   tmsi[4];
	uint32  t3212_val;
	uint32  remaining_t3212_val;
	uint16  lu_failed_cnt;
	uint16  cm_rej_cause;
	//uint8   pending_proc;
	uint32  remaining_HPLMN_val;
	uint16  mcc;
	uint16  mnc;
	uint16  mnc_digital_num;
    uint16  lac;
    uint16  rac;
    uint8   msc_version;
	uint16  hplmn_mcc;
	uint16  hplmn_mnc;
	uint8    rej_info_num;
	ENG_REJ_INFO_T rej_info[MM_MAX_REJ_INFO_NUM];
} ENG_MM_STATUS_INFO_T;

typedef struct ENG_GMM_STATUS_INFO_T_s{
	uint8    gmm_state;
	uint8    update_state;
	uint8    attach_status;
	uint8    ptmsi_valid;
	uint8    ptmsi[4];
	uint16   t3312_val;
	uint16   t3312_remaining_val;
	uint16   att_fail_cnt;
	uint16   rau_fail_cnt;
	BOOLEAN  cipher_status;
	uint16   rej_cause;
	uint8    nmo;
	BOOLEAN  sgsn_version;
	uint8    rej_info_num;
	ENG_REJ_INFO_T rej_info[MM_MAX_REJ_INFO_NUM];
} ENG_GMM_STATUS_INFO_T;

typedef struct RR_STATUS_INFO_s
{
    uint8   main_state; 
    uint8   sub_state;
    uint8   rr_service_state;
    uint8   rr_c1_all_low_flag;
    uint8   is_another_rr_traffic;
    uint8   is_resel;
    uint8   is_fast_plmn;
    uint8   is_edge_support;
    uint8   scell_band;
    uint8   chan_mode;
    uint8   chan_type;
    uint16  scell_arfcn;
    uint8   scell_bsic;
    uint8   scell_rxlev;
    uint8   scell_quality;
    uint8   scell_c1;
    uint8   scell_c2;
	uint8   scell_txpwr_dbm;
    uint8   ncell_num;
    uint8   ncell_sync[6];
    uint16  ncell_arfcn[6];
    uint8   ncell_rxlev[6];
    uint8   ncell_c1[6];
    uint8   ncell_c2[6];
} RR_STATUS_INFO_T;

/*****************************************************************************/
//  Description: Get RR debug info.
//  Global resource dependence :
//  Author: Jean Chen
//  Note:
/*****************************************************************************/
extern RR_STATUS_INFO_T* RR_GetStatusInfo(uint32 dual_sys);

extern void GetCellInfoFromPs( 
                         DIAG_CELL_INFO_LOG_T* cell_info  // Output: cell information sent to Monitoring tool
                         );


/*****************************************************************************/
// 	Description: get cells selection and reselection params and cell info. To be used by AT
//	Global resource dependence : none
//  Author: xiangxin.gu
//	Note:
/*****************************************************************************/
extern void PS_GetRRCellsInfoBsicAndEx(
	uint16 dual_sys, 
	RR_CELLS_INFO_T *rr_cells_info_bsic_ptr, 
	RR_CELLS_INFO_TRAFFIC_T *rr_cells_info_bsic_traffic_ptr, 
	RR_CELLS_INFO_EX_T *rr_cells_info_ex_ptr,
	BOOLEAN *is_traffic_ptr);

extern void GetNetInfoFromPs(
                             DIAG_NET_INFO_LOG_T* net_info // Output: Network information want to be sent by reporting sub-system
                             );

extern void PS_ForceCampon(
          		uint16 sim_no,
				uint16 arfcn_count,  ///1 -- 6
          		uint16 *arfcn_list
				);

extern void PS_CancelForceCampon(
				void
				);

/*****************************************************************************/
// 	Description:
//	Global resource dependence : none
//  Author: Yihong.zhang
//	Note: 
/*****************************************************************************/
extern int32 RRC_GetTimesOfPowerSweep(uint32 dual_sys);

/*****************************************************************************/
// 	Description:
//	Global resource dependence : none
//  Author: Yihong.zhang
//	Note: 
/*****************************************************************************/
extern int32 RRC_GetTimesOfNoCellAvailable(uint32 dual_sys);

/*****************************************************************************/
// 	Description: 
//	Global resource dependence : none
//  Author: Yihong.zhang
//	Note:
/*****************************************************************************/
extern void RRC_GetCellBasicInfo(
                          uint32 dual_sys,
                          DIAG_CELL_INFO_T *scell_info,
                          DIAG_CELL_INFO_T *ncell_info
                          );

/*****************************************************************************/
// Description : Get the address of RRC cell information buffer 
// Global resource dependence : none
// Author: Lucian.hu
/*****************************************************************************/
extern void RRC_GetCellInfoPtr( 
                               uint32 dual_sys,
                               CELL_INFO_T ** cell_arr_ptr, //output cell information array address 
                               int16 ** scell_index_ptr     //output serving cell index of cell array
                               );

/*****************************************************************************/
// Description : Get the address of RRC Resource buffer 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void RRC_GetRrResPtr( 
                               uint32 dual_sys,
                               RES_PARAM_T ** rr_res_ptr //output rr res address 
                               );

/*****************************************************************************/
// Description : Get the rr network select parameter  
// Global resource dependence : none
// Author: shijun
/*****************************************************************************/
extern SEL_PARAM_T RRC_GetSelParam( 
			uint32 dual_sys 
			);

/*****************************************************************************/
// Description : Get the address of rr_power_sweep_list buffer
// Global resource dependence : none
// Author: Yu.Sun
/*****************************************************************************/
extern void RRC_GetPowerSweepInfoPtr( 
                               uint32 dual_sys,
                               ARFCN_POWERLEVEL_T ** power_sweep_result,
                               int16 ** arfcn_num
                               );

/*****************************************************************************/
// Description : Get the address of TMSI 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void MM_GetTMSI( 
                               uint32 dual_sys,
                               TMSI_T        **tmsi_ptr
                               );

/*****************************************************************************/
// Description : Get the address of KC 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void MM_GetKC( 
                               uint32 dual_sys,
                               KC_T        **kc_ptr
                               );

/*****************************************************************************/
// Description : Get the address of CKSN 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void MM_GetCKSN( 
                               uint32 dual_sys,
                               CKSN_T        **cksn_ptr
                               );

/*****************************************************************************/
// Description : Get the address of PTMSI 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void GMM_GetPTMSI( 
                               uint32          dual_sys,
                               P_TMSI_T        **ptmsi_ptr
                               );

/*****************************************************************************/
// Description : Get the address of GU 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void GMM_GetGU( 
                               uint32                dual_sys,
                               GMM_STATE_UPDATED     **gu_ptr
                               );

/*****************************************************************************/
// Description : Get the address of TLLI current 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void GMM_GetTLLICur( 
                               uint32     dual_sys,
                               TLLI_T     **tlli_cur_ptr
                               );

/*****************************************************************************/
// Description : Get the address of NSAPI number 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void SM_GetNsapiNumPtr( 
                               uint32       dual_sys,
                               int16        *nsapi_num_ptr
                               );
/*************************************************************************/
// 	Description : get the cipher state
//	Global resource dependence : 
//  Author: Hyman.wu
//	Note: 
/*************************************************************************/
extern BOOLEAN LLCENG_IsDataCiphered(void);

//begin cr79575 yang wuding 2007.5.23
/********************************************************************
//    DESCRIPTION:Check if g_sim_card_state is SIM_INITIALIZE_STATE or
//                SIM_ACTIVE_STATE.
//    Global resource dependence :
//    Author:hanjun.liu
//    Note:
***********************************************************************/
extern uint8 REF_CheckSimState(void);

/*****************************************************************************/
//  Description:    Repacket GPRS message which includes pointer., the 
//                  destination packet saved in dest_ptr. If the message with 
//                  no pointer, don't handle it, dest_ptr is still NULL.
//	Global resource dependence: 
//  Author: hanjun.liu
//	Note:
/*****************************************************************************/
extern PUBLIC void REF_PktGprsMsg(         // None
    uint8     **dest_ptr,       // Destination packet, save packed message.
    uint16    msg_type,         // Message type. Low 8 bit is struct type
    const uint8     *src_ptr,   // Source message
    uint16    *len              // Length of src_ptr when input,
                                // length of dest_ptr when output,
    );

// Init PS.
extern uint32 SCI_InitPs(void);

/*******************************************************************/
//	Function name   REF_GetSizeOfClassmark
// 	Discription     Get the size of struct PS_CLASSMARK_T,add it for 
//                  build ref lib.
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
PUBLIC uint32 REF_GetSizeOfClassmark(void);

/*****************************************************************************/
//  Description:    Send a request to set ARM clock.
//  Author:         hanjun.liu
//	Note:           
/*****************************************************************************/
extern void REF_SendSetArmClkReq(uint32 clk);

/*****************************************************************************/
//  Description:    copy cell info
//  Author:         
//	Note:           
/*****************************************************************************/
extern void RRC_CopyCellInfo(
    DIAG_CELL_PRIMARY_INFO_T *dest_cell,
    CELL_INFO_T *src_cell
    );

/*****************************************************************************/
//  Description:    copy scell info
//  Author:         
//	Note:           
/*****************************************************************************/
extern void RRC_CopyScellInfo(
    DIAG_SERVING_CELL_INFO_T *dest_scell,
    CELL_INFO_T *src_scell
    );
#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description:
//  Global resource dependence : none
//  Author: binxian.duan
//  Note:
/*****************************************************************************/
extern void RRC_GetScanedCellInfo(
    uint32 dual_sys,
    DIAG_SCANED_CELL_INFO_ARR_T * cell_list
    );

/*****************************************************************************/
//  Description:force to grab Si3 if flag is true.
//  Global resource dependence : none
//  Author: binxian.duan
//  Note:
/*****************************************************************************/
void RRC_SetCellScanFlag(
    uint32 dual_sys, 
    BOOLEAN flag
);
#endif // MODEM_PLATFORM
/*****************************************************************************/
//  Description:    BASE init save message function
//  Author:         
//	Note:           
/*****************************************************************************/
extern void InitSaveMsg(
         void
         );

extern void SCI_LogAssertState(char *file, uint16 line, char *user_str);

/*****************************************************************************/
//  Description:    BASE init save message function
//  Author:         
//	Note:           
/*****************************************************************************/
extern void SCI_SaveBaseMsg(
            xSignalHeaderRec *sig_ptr,
            uint32           comm_ref
            );

/*****************************************************************************/
//  Description: Samsung Eng mode, get MM info
//  Author: Yu.Sun
//	Note:           
/*****************************************************************************/
extern ENG_MM_STATUS_INFO_T* ENG_MM_GetStatusInfo(uint32 dual_sys);

/*****************************************************************************/
//  Description: Samsung Eng mode, get GMM info
//  Author: winnie.zhu
//	Note:           
/*****************************************************************************/
extern ENG_GMM_STATUS_INFO_T* ENG_GMM_GetStatusInfo(uint32 dual_sys);
#ifdef MODEM_PLATFORM
/*****************************************************************************/
//  Description: get the RLA and TXP of server cell.
//               参数ms_txpwr_maxcch和rxlev_access_min为功率水平，取值范围如下:
//               ms_txpwr_maxcch: 0~31
//               rxlev_access_min: 0~63
//  Global resource dependence:
//  Author: binxian.duan
//  Note:
/*****************************************************************************/
extern void RRC_GetScellInfo_RLA_TXP(
            uint32 dual_sys, 
            uint8 *ms_txpwr_maxcch, 
            uint8 *rxlev_access_min
);
#endif
/**********************************************************************
//    DESCRIPTION: when set to ture, CS paging will be discarded 
//                 if pdp active.
//    Global resource dependence :
//    Author:
//    Note:
***********************************************************************/
extern void PS_SetPacketDataFirst(
       BOOLEAN isonoff
);
/**********************************************************************
//    DESCRIPTION: get the value whether PacketDataFirst or not.
//    Global resource dependence :
//    Author:
//    Note:
***********************************************************************/
extern BOOLEAN PS_GetPacketDataFirst();

/******************************************************************************
//    Description:
//        Is GPRS active or not?
//    Global resource dependence :
//    Arthor: Suzhi.Hao
//    Note:
******************************************************************************/
extern BOOLEAN RLC_IsGPRSActive(void);

/*****************************************************************************/
//  Description: Set receive paging in TBF on or off
//  Global resource dependence : none
//  Author: Yu.Sun
//  Note:
/*****************************************************************************/
extern void RRPROC_SetTbfPaging(
    uint16  dual_sys,      //in -- system index, 0 or 1
    BOOLEAN tbf_paging_on  //in -- 
);

#ifdef MODEM_PLATFORM
/*************************************************************************/
// 	Description : modify the rach parameter -- max retrans
//	Global resource dependence : 
//  Author: xiaoxiao.yan
//	Note: 
/*************************************************************************/
extern void RRC_SetUserRachParam_Retrans(
         uint32   dual_sys,                 //in -- system index, 0 or 1
         BOOLEAN  retrans_flag,             //in -- retrans optimization or not
         uint8    retrans,                  //in -- user modify param for retrans
         uint8    retrans_upper_bound       //in -- user modify param for retrans upper bound                       
);
/*************************************************************************/
// 	Description : modify the rach parameter -- rach delay
//	Global resource dependence : 
//  Author: xiaoxiao.yan 
//	Note: 
/*************************************************************************/
extern void RRC_SetUserRachParam_Rach_Delay(
         uint32   dual_sys,                 //in -- system index, 0 or 1
         BOOLEAN  rach_delay_flag,          //in -- rach delay optimization or not
         uint8    rach_delay,               //in -- user modify param for rach delay
         uint16   rach_delay_lower_bound    //in -- user modify param for rach delay lower bound                
);
/*************************************************************************/
// 	Description : modify the rach parameter -- t3122/3142
//	Global resource dependence : 
//  Author: xiaoxiao.yan
//	Note: 
/*************************************************************************/
extern void RRC_SetUserRachParam_t31x2(
         uint32   dual_sys,                 //in -- system index, 0 or 1
         BOOLEAN  t31x2_flag,               //in -- t31x2 optimization or not
         uint8    t31x2_timeout,            //in -- user modify param for T31x2
         uint32   t31x2_lower_bound         //in -- user modify param for T31x2 lower bound                      
);
/*************************************************************************/
// 	Description : modify the rach parameter -- t3126/3146
//	Global resource dependence : 
//  Author: xiaoxiao.yan 
//	Note: 
/*************************************************************************/
extern void RRC_SetUserRachParam_t31x6(
         uint32   dual_sys,                 //in -- system index, 0 or 1
         BOOLEAN  t31x6_flag,               //in -- t31x6 optimization or not
         uint8    t31x6_timeout,            //in -- user modify param for T31x6
         uint32   t31x6_lower_bound         //in -- user modify param for T31x6 lower bound                       
);
#endif  // MODEM_PLATFORM
/*****************************************************************************/
// Description : Get the value of T3380 
// Global resource dependence : none
// Author: star.liu
/*****************************************************************************/
extern uint8 SM_GetT3380Value(void);

/*****************************************************************************/
// Description : Get the value of T3390 
// Global resource dependence : none
// Author: star.liu
/*****************************************************************************/
extern uint8 SM_GetT3390Value(void);

/*****************************************************************************/
// Description : Set the value of T3380 
// Global resource dependence : none
// Author: star.liu
/*****************************************************************************/
extern void SM_SetT3380Value(uint8 set_t3380_value);

/*****************************************************************************/
// Description : Set the value of T3390 
// Global resource dependence : none
// Author: star.liu
/*****************************************************************************/
extern void SM_SetT3390Value(uint8 set_t3390_value);

/*****************************************************************************/
// Description : Set the value of T3390 expried count
// Global resource dependence : none
// Author: 
/*****************************************************************************/
extern void SM_SetT3390ExpCount(uint8 set_t3390_exp_count);

/*****************************************************************************/
// Description : Get the value of T3390 expried count
// Global resource dependence : none
// Author: 
/*****************************************************************************/
extern uint8 SM_GetT3390ExpCount(void);

/*************************************************************************/
// 	Description : set MAC user paramter
//	Global resource dependence :
//  Author: Hardy.Hou
//	Note:
/*************************************************************************/
extern void MAC_SetUserParam(
    uint8    rej_time                  //in -- set reject time, 0 -- not set, 1~255 set time value(unit - 20ms)
);

/*****************************************************************************/
//  Description: SET RR IS POS mode
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
extern void RRC_SetPosModeFlag(
    BOOLEAN  pos_mode_flag                       
);

/*****************************************************************************/
// Description : Set wait the second pdp accept message
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
extern void SM_WaitPdpAcceptEnable(BOOLEAN is_enable);

/*****************************************************************************/
//  Description: Set disable Pwr flag
//  Global resource dependence : none
//  Author: xiaoxiao.yan
/*****************************************************************************/
extern void RRPROC_Set_disablePwr(BOOLEAN disable_Pwr);

/*****************************************************************************/
//  Description: Set identify BS flag
//  Global resource dependence : none
//  Author: xiaoxiao.yan
/*****************************************************************************/
extern void RRPROC_Set_identifyBS(BOOLEAN start_identify);

/*****************************************************************************/
//  Description: Set identify BS flag
//  Global resource dependence : none
//  Author: xiaoxiao.yan
/*****************************************************************************/
extern void RRPROC_Set_identifyBSMode(BOOLEAN identify_mode);

/*****************************************************************************/
//  Description:
//  Global resource dependence :
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
extern void RRC_SetPagingWakeUpReport(BOOLEAN flag);

/*****************************************************************************/
//  Description: 
//  Global resource dependence:
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
extern void PS_SetPagingWakeUpReport(BOOLEAN flag);

/*****************************************************************************/
//  Description: 
//  Global resource dependence:
//  Author: wuding.yang
//  Note:
/*****************************************************************************/
extern BOOLEAN PS_GetPagingWakeUpReport(void);

#endif /* end of _PS_INFO_H_ */

/******************************************************************************
 ** File Name:      ps_output.h                                               *
 ** Author:         shijun cui                                                *
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
 ** 03/11/2002     shijun cui       Create.                                   *
 ******************************************************************************/

#if !defined (_PS_OUTPUT_H)
#define _PS_OUTPUT_H
#include "sci_types.h"
#include "comm_def.h"
#include "gsm_gprs.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


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
} DIAG_CELL_INFO_T;

/* Copy the primary cell information */
#if !defined(_SUPPORT_GPRS_) && !defined(GPRS_SUPPORT)	
#define DIAG_COPY_CELL( dst_cell, src_cell ) \
    (dst_cell)->cell_present   = (src_cell)->cell_present;\
    (dst_cell)->arfcn          = (src_cell)->arfcn;\
    (dst_cell)->bsic           = (src_cell)->bsic;\
    (dst_cell)->lac            = (src_cell)->lai.lac;\
    (dst_cell)->rxlev          = (src_cell)->rssi;\
    (dst_cell)->c1             = (src_cell)->c1;\
    (dst_cell)->c2             = (src_cell)->c2;\
    (dst_cell)->gprs_supported = FALSE;\
    (dst_cell)->c31            = (src_cell)->c31;\
    (dst_cell)->c32            = (src_cell)->c32;
#else
#define DIAG_COPY_CELL( dst_cell, src_cell ) \
    (dst_cell)->cell_present   = (src_cell)->cell_present;\
    (dst_cell)->arfcn          = (src_cell)->arfcn;\
    (dst_cell)->bsic           = (src_cell)->bsic;\
    (dst_cell)->lac            = (src_cell)->lai.lac;\
    (dst_cell)->rxlev          = (src_cell)->rssi;\
    (dst_cell)->c1             = (src_cell)->c1;\
    (dst_cell)->c2             = (src_cell)->c2;\
    (dst_cell)->gprs_supported = (src_cell)->gprs_ind.gprs_ind_present;\
    (dst_cell)->c31            = (src_cell)->c31;\
    (dst_cell)->c32            = (src_cell)->c32;
#endif

/* Copy the detailed information of serving cell */
#define DIAG_COPY_SCELL( dst_cell, src_cell )        \
    (dst_cell)->mcc              = (src_cell)->lai.mcc;\
    (dst_cell)->mnc              = (src_cell)->lai.mnc;\
    (dst_cell)->lac              = (src_cell)->lai.lac;\
    (dst_cell)->cell_id          = (src_cell)->cell_id;\
    (dst_cell)->pm               = (src_cell)->page_mode;\
    (dst_cell)->pwrc             = (src_cell)->cell_options.pwrc;\
    (dst_cell)->dtx              = (src_cell)->cell_options.dtx_allowed;\
    (dst_cell)->rlt              = (src_cell)->cell_options.rlt;\
    (dst_cell)->att              = (src_cell)->ctrl_chan_desc.att;\
    (dst_cell)->bsag             = (src_cell)->ctrl_chan_desc.bs_ag;\
    (dst_cell)->ccch             = (src_cell)->ctrl_chan_desc.ccch_config;\
    (dst_cell)->cbq2             = (src_cell)->ctrl_chan_desc.cbq2;\
    (dst_cell)->bspamf           = (src_cell)->ctrl_chan_desc.bs_pa;\
    (dst_cell)->maxre            = (src_cell)->rach_ctrl_param.max_retrans;\
    (dst_cell)->txint            = (src_cell)->rach_ctrl_param.tx_integer;\
    (dst_cell)->cba              = (src_cell)->rach_ctrl_param.cba;\
    (dst_cell)->re               = (src_cell)->rach_ctrl_param.re_est;\
    (dst_cell)->ac               = (src_cell)->rach_ctrl_param.access_class;\
    (dst_cell)->crh              = (src_cell)->cell_sel_param.crh;\
    (dst_cell)->tpmxcch          = (src_cell)->cell_sel_param.ms_txpwr_maxcch;\
    (dst_cell)->ram              = (src_cell)->cell_sel_param.rxlev_access_min;\
    (dst_cell)->osp_present      = (src_cell)->option_sel_param.sel_param_present;\
    (dst_cell)->cbp              = (src_cell)->option_sel_param.cbq;\
    (dst_cell)->cro              = (src_cell)->option_sel_param.cell_resel_off;\
    (dst_cell)->tmpo             = (src_cell)->option_sel_param.temporary_off;\
    (dst_cell)->ptm              = (src_cell)->option_sel_param.penalty_time;\
    (dst_cell)->cbch_present     = (src_cell)->cbch_present;\
    (dst_cell)->cbch_chan        = (src_cell)->cbch_chan_desc.dch_type;\
    (dst_cell)->cbch_tn          = (src_cell)->cbch_chan_desc.tn;\
    (dst_cell)->cbch_tsc         = (src_cell)->cbch_chan_desc.tsc;\
    (dst_cell)->cbch_is_hopping  = (src_cell)->cbch_chan_desc.hopping_ind;\
    (dst_cell)->cbch_maio        = (src_cell)->cbch_chan_desc.maio;\
    (dst_cell)->cbch_hsn         = (src_cell)->cbch_chan_desc.hsn; \
    (dst_cell)->cbch_arfcn       = (src_cell)->cbch_chan_desc.arfcn;

extern void GetCellInfoFromPs( 
                         DIAG_CELL_INFO_LOG_T* cell_info  // Output: cell information sent to Monitoring tool
                         );

extern void GetNetInfoFromPs(
                             DIAG_NET_INFO_LOG_T* net_info // Output: Network information want to be sent by reporting sub-system
                             );

/*****************************************************************************/
extern int32 RRC_GetTimesOfPowerSweep(void);

/*****************************************************************************/
// 	Description:
//	Global resource dependence : none
//  Author: Yihong.zhang
//	Note: 
/*****************************************************************************/
extern int32 RRC_GetTimesOfNoCellAvailable(void);
/*****************************************************************************/
// 	Description: 
//	Global resource dependence : none
//  Author: Yihong.zhang
//	Note:
/*****************************************************************************/
extern void RRC_GetCellBasicInfo(
                          DIAG_CELL_INFO_T *scell_info,
                          DIAG_CELL_INFO_T *ncell_info
                          );
// Description : Get the address of RRC cell information buffer 
// Global resource dependence : none
// Author: Lucian.hu
/*****************************************************************************/
extern void RRC_GetCellInfoPtr( 
                               CELL_INFO_T ** cell_arr_ptr, //output cell information array address 
                               int16 ** scell_index_ptr     //output serving cell index of cell array
                               );

/*****************************************************************************/
// Description : Get the address of RRC Resource buffer 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void RRC_GetRrResPtr( 
                               RES_PARAM_T ** rr_res_ptr //output rr res address 
                               );

/*****************************************************************************/
// Description : Get the address of TMSI 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void MM_GetTMSI( 
                               TMSI_T        **tmsi_ptr
                               );

/*****************************************************************************/
// Description : Get the address of KC 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void MM_GetKC( 
                               KC_T        **kc_ptr
                               );

/*****************************************************************************/
// Description : Get the address of CKSN 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void MM_GetCKSN( 
                               CKSN_T        **cksn_ptr
                               );

/*****************************************************************************/
// Description : Get the address of PTMSI 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void GMM_GetPTMSI( 
                               P_TMSI_T        **ptmsi_ptr
                               );

/*****************************************************************************/
// Description : Get the address of GU 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void GMM_GetGU( 
                               GMM_STATE_UPDATED     **gu_ptr
                               );

/*****************************************************************************/
// Description : Get the address of TLLI current 
// Global resource dependence : none
// Author: William Qian
/*****************************************************************************/
extern void GMM_GetTLLICur( 
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
extern BOOLEAN LLCCC_IsDataCiphered(void);

//begin cr78558 yang wuding 2007.5.15
void PS_ForceCampon(
          		uint16 sim_no,
				uint16 arfcn_count,  ///1 -- 6
          		uint16 *arfcn_list
				);

void PS_CancelForceCampon(
				void
				);

/********************************************************************
//    DESCRIPTION:Check if g_sim_card_state is SIM_INITIALIZE_STATE or
//                SIM_ACTIVE_STATE.
//    Global resource dependence :
//    Author:hanjun.liu
//    Note:
***********************************************************************/
PUBLIC uint8 REF_CheckSimState(void);

/*****************************************************************************/
//  Description:    Repacket GPRS message which includes pointer., the 
//                  destination packet saved in dest_ptr. If the message with 
//                  no pointer, don't handle it, dest_ptr is still NULL.
//	Global resource dependence: 
//  Author: hanjun.liu
//	Note:
/*****************************************************************************/
PUBLIC void REF_PktGprsMsg(         // None
    uint8     **dest_ptr,       // Destination packet, save packed message.
    uint16    msg_type,         // Message type. Low 8 bit is struct type
    const uint8     *src_ptr,   // Source message
    uint16    *len              // Length of src_ptr when input,
                                // length of dest_ptr when output,
    );

extern uint32 SCI_InitPs(void); 

//end cr78558 yang wuding 2007.5.15

#endif /* end of _PS_OUTPUT_H */
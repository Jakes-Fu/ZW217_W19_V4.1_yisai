/******************************************************************************
** File Name:      sim_win_test.c                                            *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This functions that can be used to test sim layer in      *
**                 window simulator without sim hardware:use files in        *
**                 Harddisk in windows replace the files and contents in SIM *
**                 card and make the full protocol PS can run in windows test*
**                 with the windows SIM card simulator.                      *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/
#include "windows.h"
#include "sim_include.h"
#include "power.h"

#ifdef   __cplusplus
extern   "C"
{
#endif  //__cplusplus

#ifdef SIM_TEST_IN_WIN32

//the position of the corresponding chv in the CHV info file
typedef enum
{
    SIM_POS_CHV1,
    SIM_POS_CHV2,
    SIM_POS_UNBLOCK_CHV1,
    SIM_POS_UNBLOCK_CHV2
}
SIM_CHV_POS;


#ifdef SIM_SIMULATOR
#define SIM_DISABLE_IRQ
#define SIM_RESTORE_IRQ
#else
#ifdef SIM_READER
#define SIM_DISABLE_IRQ
#define SIM_RESTORE_IRQ
#else
#include "dal_power.h"
#define SIM_DISABLE_IRQ     if (POWER_MS_STATUS_POWER_ON == POWER_GetMsStatus())\
{\
    SCI_DISABLE_IRQ\
}
#define SIM_RESTORE_IRQ     if (POWER_MS_STATUS_POWER_ON == POWER_GetMsStatus()) \
{\
    SCI_RESTORE_IRQ\
}
#endif  //SIM_READER
#endif  //SIM_SIMULATOR


//------------------------------local variable declare-------------------------------
STATIC BOOLEAN CHV1_Verified[4] = {FALSE, FALSE, FALSE, FALSE};
STATIC BOOLEAN CHV2_Verified[4] = {FALSE, FALSE, FALSE, FALSE};
STATIC int cur_record_num[4] = {0, 0, 0, 0};
STATIC uint8 cur_ext_record_id[4] = {0, 0, 0, 0};
STATIC char full_path[4][MAX_PATH];
static BOOLEAN bBaseDirDecided[4] = {FALSE, FALSE, FALSE, FALSE};
//the SIM card info file directory
STATIC char base_directory[4][MAX_PATH];
//the SIM chv file name
STATIC char chv_file_path[4][MAX_PATH];
//the file that save the SIM card file select data
const STATIC char select_file[SIM_FILE_TOTAL_NUM][40] =
{
    "MF_SELECT.inf",
    "DF_GSM_SELECT.inf",
    "DF_DCS1800_SELECT.inf",
    "DF_TELECOM_SELECT.inf",
    "DF_IS41_SELECT.inf",
    "DF_FP_CTS_SELECT.inf",
    "DF_ADF_USIM_SELECT.inf",
    "DF_ADF_USIM_DF_GSM_SELECT.inf",
    "DF_IRIDIUM_SELECT.inf",
    "DF_GLOBST_SELECT.inf",
    "DF_ICO_SELECT.inf",
    "DF_ACeS_SELECT.inf",
    "DF_MEXE_SELECT.inf",
    "DF_PCS1900_SELECT.inf",
    "DF_CTS_SELECT.inf",
    "DF_SoLSA_SELECT.inf",
    "DF_TELECOM_PB_SELECT.inf",
    "DF_GRAPHICS_SELECT.inf",
    "DF_ADF_USIM_DF_PB_SELECT.inf",
    "DF_ADF_USIM_DF_SOLSA_SELECT.inf",
    "DF_ADF_USIM_DF_MEXE_SELECT.inf",
    "DF_CDMA_SELECT.inf",
    "DF_ORANGE_SELECT.inf",
    "EF_ICCID_SELECT.inf",
    "EF_DIR_SELECT.inf",
    "EF_ELP_SELECT.inf",
    "EF_MF_ARR_SELECT.inf",
    "EF_LP_SELECT.inf",
    "EF_IMSI_SELECT.inf",
    "EF_KC_SELECT.inf",
    "EF_PLMNsel_SELECT.inf",
    "EF_HPLMN_SELECT.inf",
    "EF_ACMmax_SELECT.inf",
    "EF_SST_SELECT.inf",
    "EF_ACM_SELECT.inf",
    "EF_GID1_SELECT.inf",
    "EF_GID2_SELECT.inf",
    "EF_PUCT_SELECT.inf",
    "EF_CBMI_SELECT.inf",
    "EF_SPN_SELECT.inf",
    "EF_CBMID_SELECT.inf",
    "EF_BCCH_SELECT.inf",
    "EF_ACC_SELECT.inf",
    "EF_FPLMN_SELECT.inf",
    "EF_LOCI_SELECT.inf",
    "EF_AD_SELECT.inf",
    "EF_PHASE_SELECT.inf",
    "EF_VGCS_SELECT.inf",
    "EF_VGCSS_SELECT.inf",
    "EF_VBS_SELECT.inf",
    "EF_VBSS_SELECT.inf",
    "EF_eMLPP_SELECT.inf",
    "EF_AAem_SELECT.inf",
    "EF_ECC_SELECT.inf",
    "EF_CBMIR_SELECT.inf",
    "EF_DCK_SELECT.inf",
    "EF_CNL_SELECT.inf",
    "EF_NIA_SELECT.inf",
    "EF_KcGRPS_SELECT.inf",
    "EF_LOCIGPRS_SELECT.inf",
    "EF_SUME_SELECT.inf",
    "EF_PLMNWACT_SELECT.inf",
    "EF_OPLMNWACT_SELECT.inf",
    "EF_HPLMNACT_SELECT.inf",
    "EF_CPBCCH_SELECT.inf",
    "EF_INVSCAN_SELECT.inf",
    "EF_PNN_SELECT.inf",
    "EF_OPL_SELECT.inf",
    "EF_MBDN_SELECT.inf",
    "EF_EXT6_SELECT.inf",
    "EF_MBI_SELECT.inf",
    "EF_MWIS_SELECT.inf",
    "EF_CFIS_SELECT.inf",
    "EF_EXT7_SELECT.inf",
    "EF_SPDI_SELECT.inf",
    "EF_MMSN_SELECT.inf",
    "EF_EXT8_SELECT.inf",
    "EF_MMSICP_SELECT.inf",
    "EF_MMSUP_SELECT.inf",
    "EF_MMSUCP_SELECT.inf",
    "EF_SAI_SELECT.inf",
    "EF_SLL_SELECT.inf",
    "EF_ADN_SELECT.inf",
    "EF_FDN_SELECT.inf",
    "EF_SMS_SELECT.inf",
    "EF_CCP_SELECT.inf",
    "EF_ECCP_SELECT.inf",
    "EF_MSISDN_SELECT.inf",
    "EF_SMSP_SELECT.inf",
    "EF_SMSS_SELECT.inf",
    "EF_LND_SELECT.inf",
    "EF_SMSR_SELECT.inf",
    "EF_SDN_SELECT.inf",
    "EF_EXT1_SELECT.inf",
    "EF_EXT2_SELECT.inf",
    "EF_EXT3_SELECT.inf",
    "EF_BDN_SELECT.inf",
    "EF_EXT4_SELECT.inf",
    "EF_TELECOM_SUME_SELECT.inf",
    "EF_CMI_SELECT.inf",
    "EF_IMG_SELECT.inf",
    "EF_CDMA_IMSI_SELECT.inf",
    "EF_ONSTRING_SELECT.inf",
    "EF_ONSHORTFORM_SELECT.inf",
    "EF_CPHS_INFO_SELECT.inf",
    "EF_VMW_FLAG_SELECT.inf",
    "EF_CCF_SELECT.inf",
    "EF_CSP_SELECT.inf",
    "EF_MAILBOX_NUM_SELECT.inf",
    "EF_INFONUM_SELECT.inf",
    "EF_CPHS_SST_SELECT.inf",
    "EF_MEXE_SELECT.inf",
    "EF_ORPK_SELECT.inf",
    "EF_ARPK_SELECT.inf",
    "EF_TPRPK_SELECT.inf",
    "EF_TELECOM_ARR_SELECT.inf",
    "EF_TELECOM_PB_PSC_SELECT.inf",
    "EF_TELECOM_PB_CC_SELECT.inf",
    "EF_TELECOM_PB_PUID_SELECT.inf",
    "EF_TELECOM_PB_PBR_SELECT.inf",
    "EF_TELECOM_PB_IAP_SELECT.inf",
    "EF_TELECOM_PB_ADN_SELECT.inf",
    "EF_TELECOM_PB_EXT1_SELECT.inf",
    "EF_TELECOM_PB_PBC_SELECT.inf",
    "EF_TELECOM_PB_GRP_SELECT.inf",
    "EF_TELECOM_PB_AAS_SELECT.inf",
    "EF_TELECOM_PB_GAS_SELECT.inf",
    "EF_TELECOM_PB_ANR_SELECT.inf",
    "EF_TELECOM_PB_SNE_SELECT.inf",
    "EF_TELECOM_PB_CCP1_SELECT.inf",
    "EF_TELECOM_PB_UID_SELECT.inf",
    "EF_TELECOM_PB_EMAIL_SELECT.inf",
    "EF_TELECOM_PB_ADN1_SELECT.inf",
    "EF_USIM_GSM_KC_SELECT.inf",
    "EF_USIM_GSM_KCGPRS_SELECT.inf",
    "EF_USIM_GSM_CPBCCH_SELECT.inf",
    "EF_USIM_GSM_INVSCAN_SELECT.inf",
    "EF_USIM_PB_PSC_SELECT.inf",
    "EF_USIM_PB_CC_SELECT.inf",
    "EF_USIM_PB_PUID_SELECT.inf",
    "EF_USIM_PB_PBR_SELECT.inf",
    "EF_USIM_PB_IAP_SELECT.inf",
    "EF_USIM_PB_ADN_SELECT.inf",
    "EF_USIM_PB_EXT1_SELECT.inf",
    "EF_USIM_PB_PBC_SELECT.inf",
    "EF_USIM_PB_GRP_SELECT.inf",
    "EF_USIM_PB_AAS_SELECT.inf",
    "EF_USIM_PB_GAS_SELECT.inf",
    "EF_USIM_PB_ANR_SELECT.inf",
    "EF_USIM_PB_SNE_SELECT.inf",
    "EF_USIM_PB_CCP1_SELECT.inf",
    "EF_USIM_PB_UID_SELECT.inf",
    "EF_USIM_PB_EMAIL_SELECT.inf",
    "EF_USIM_LI_SELECT.inf",
    "EF_USIM_ARR_SELECT.inf",
    "EF_USIM_IMSI_SELECT.inf",
    "EF_USIM_DCK_SELECT.inf",
    "EF_USIM_HPLMN_SELECT.inf",
    "EF_USIM_CNL_SELECT.inf",
    "EF_USIM_ACMMAX_SELECT.inf",
    "EF_USIM_UST_SELECT.inf",
    "EF_USIM_ACM_SELECT.inf",
    "EF_USIM_FDN_SELECT.inf",
    "EF_USIM_SMS_SELEECT.inf",
    "EF_USIM_GID1_SELECT.inf",
    "EF_USIM_GID2_SELECT.inf",
    "EF_USIM_MSISDN_SELECT.inf",
    "EF_USIM_PUCT_SELECT.inf",
    "EF_USIM_SMSP_SELECT.inf",
    "EF_USIM_SMSS_SELECT.inf",
    "EF_USIM_CBMI_SELECT.inf",
    "EF_USIM_SPN_SELECT.inf",
    "EF_USIM_SMSR_SELECT.inf",
    "EF_USIM_CBMID_SELECT.inf",
    "EF_USIM_SDN_SELECT.inf",
    "EF_USIM_EXT2_SELECT.inf",
    "EF_USIM_EXT3_SELECT.inf",
    "EF_USIM_BDN_SELECT.inf",
    "EF_USIM_CBMIR_SELECT.inf",
    "EF_USIM_EXT4_SELECT.inf",
    "EF_USIM_EST_SELECT.inf",
    "EF_USIM_CMI_SELECT.inf",
    "EF_USIM_PLMNWACT_SELECT.inf",
    "EF_USIM_OPLMNWACT_SELECT.inf",
    "EF_USIM_HPLMNWACT_SELECT.inf",
    "EF_USIM_PSLOCI_SELECT.inf",
    "EF_USIM_ACC_SELECT.inf",
    "EF_USIM_FPLMN_SELECT.inf",
    "EF_USIM_LOCI_SELECT.inf",
    "EF_USIM_AD_SELECT.inf",
    "EF_USIM_VGCS_SELECT.inf",
    "EF_USIM_VGCSS_SELECT.inf",
    "EF_USIM_VBS_SELECT.inf",
    "EF_USIM_VBSS_SELECT.inf",
    "EF_USIM_EMLPP_SELECT.inf",
    "EF_USIM_AAEM_SELECT.inf",
    "EF_USIM_ECC_SELECT.inf",
    "EF_USIM_PNN_SELECT.inf",
    "EF_USIM_OPL_SELECT.inf",
    "EF_USIM_MBDN_SELECT.inf",
    "EF_USIM_EXT6_SELECT.inf",
    "EF_USIM_MBI_SELECT.inf",
    "EF_USIM_MWIS_SELECT.inf",
    "EF_USIM_CFIS_SELECT.inf",
    "EF_USIM_EXT7_SELECT.inf",
    "EF_USIM_SPDI_SELECT.inf",
    "EF_USIM_MMSN_SELECT.inf",
    "EF_USIM_EXT8_SELECT.inf",
    "EF_USIM_MMSICP_SELECT.inf",
    "EF_USIM_MMSUP_SELECT.inf",
    "EF_USIM_MMSUCP_SELECT.inf",
    "EF_USIM_NIA_SELECT.inf",
    "EF_USIM_EHPLMN_SELECT.inf",
    "EF_USIM_EHPLMNPI_SELECT.inf",
    "EF_USIM_SPNI_SELECT.inf",
    "EF_USIM_PNNI_SELECT.inf",
    "EF_USIM_SOLSA_SAI_SELECT.inf",
    "EF_USIM_SOLSA_SLL_SELECT.inf",
    "EF_USIM_MEXE_SST_SELECT.inf",
    "EF_USIM_MEXE_ORPK_SELECT.inf",
    "EF_USIM_MEXE_ARPK_SELECT.inf",
    "EF_USIM_MEXE_TPRPK_SELECT.inf",
    "EF_CST_SELECT.inf",
    "EF_ORANGE_OCT_SELECT.inf",
    "EF_ORANGE_ORANGE_DYN2_FLAG_SELECT.inf",
    "EF_ORANGE_ORANGE_CSP_LINE2_SELECT.inf",
    "EF_ORANGE_ORANGE_PARAM_SELECT.inf",
    "EF_ORANGE_ORANGE_ACM2_SELECT.inf",
    "EF_ORANGE_ORANGE_DYN_FLAG_SELECT.inf"
};

//the char array is the file name that save the EF content
const STATIC char ef_file_name[SIM_FILE_TOTAL_NUM][35] =
{
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "EF_ICCID.inf",
    "EF_DIR.inf",
    "EF_ELP.inf",
    "EF_MF_ARR.inf",
    "EF_LP.inf",
    "EF_IMSI.inf",
    "EF_KC.inf",
    "EF_PLMNsel.inf",
    "EF_HPLMN.inf",
    "EF_ACMmax.inf",
    "EF_SST.inf",
    "EF_ACM.inf",
    "EF_GID1.inf",
    "EF_GID2.inf",
    "EF_PUCT.inf",
    "EF_CBMI.inf",
    "EF_SPN.inf",
    "EF_CBMID.inf",
    "EF_BCCH.inf",
    "EF_ACC.inf",
    "EF_FPLMN.inf",
    "EF_LOCI.inf",
    "EF_AD.inf",
    "EF_PHASE.inf",
    "EF_VGCS.inf",
    "EF_VGCSS.inf",
    "EF_VBS.inf",
    "EF_VBSS.inf",
    "EF_eMLPP.inf",
    "EF_AAem.inf",
    "EF_ECC.inf",
    "EF_CBMIR.inf",
    "EF_DCK.inf",
    "EF_CNL.inf",
    "EF_NIA.inf",
    "EF_KcGRPS.inf",
    "EF_LOCIGPRS.inf",
    "EF_SUME.inf",
    "EF_PLMNWACT.inf",
    "EF_OPLMNWACT.inf",
    "EF_HPLMNACT.inf",
    "EF_CPBCCH.inf",
    "EF_INVSCAN.inf",
    "EF_PNN.inf",
    "EF_OPL.inf",
    "EF_MBDN.inf",
    "EF_EXT6.inf",
    "EF_MBI.inf",
    "EF_MWIS.inf",
    "EF_CFIS.inf",
    "EF_EXT7.inf",
    "EF_SPDI.inf",
    "EF_MMSN.inf",
    "EF_EXT8.inf",
    "EF_MMSICP.inf",
    "EF_MMSUP.inf",
    "EF_MMSUCP.inf",
    "EF_SAI.inf",
    "EF_SLL.inf",
    "EF_ADN.inf",
    "EF_FDN.inf",
    "EF_SMS.inf",
    "EF_CCP.inf",
    "EF_ECCP.inf",
    "EF_MSISDN.inf",
    "EF_SMSP.inf",
    "EF_SMSS.inf",
    "EF_LND.inf",
    "EF_SMSR.inf",
    "EF_SDN.inf",
    "EF_EXT1.inf",
    "EF_EXT2.inf",
    "EF_EXT3.inf",
    "EF_BDN.inf",
    "EF_EXT4.inf",
    "EF_TELECOM_SUME.inf",
    "EF_CMI.inf",
    "EF_IMG.inf",
    "EF_CDMA_IMSI.inf",
    "EF_ONSTRING.inf",
    "EF_ONSHORTFORM.inf",
    "EF_CPHS_INFO.inf",
    "EF_VMW_FLAG.inf",
    "EF_CCF.inf",
    "EF_CSP.inf",
    "EF_MAILBOX_NUM.inf",
    "EF_INFONUM.inf",
    "EF_CPHS_SST.inf",
    "EF_MEXE.inf",
    "EF_ORPK.inf",
    "EF_ARPK.inf",
    "EF_TPRPK.inf",
    "EF_TELECOM_ARR.inf",
    "EF_TELECOM_PB_PSC.inf",
    "EF_TELECOM_PB_CC.inf",
    "EF_TELECOM_PB_PUID.inf",
    "EF_TELECOM_PB_PBR.inf",
    "EF_TELECOM_PB_IAP.inf",
    "EF_TELECOM_PB_ADN.inf",
    "EF_TELECOM_PB_EXT1.inf",
    "EF_TELECOM_PB_PBC.inf",
    "EF_TELECOM_PB_GRP.inf",
    "EF_TELECOM_PB_AAS.inf",
    "EF_TELECOM_PB_GAS.inf",
    "EF_TELECOM_PB_ANR.inf",
    "EF_TELECOM_PB_SNE.inf",
    "EF_TELECOM_PB_CCP1.inf",
    "EF_TELECOM_PB_UID.inf",
    "EF_TELECOM_PB_EMAIL.inf",
    "EF_TELECOM_PB_ADN1.inf",
    "EF_USIM_GSM_KC.inf",
    "EF_USIM_GSM_KCGPRS.inf",
    "EF_USIM_GSM_CPBCCH.inf",
    "EF_USIM_GSM_INVSCAN.inf",
    "EF_USIM_PB_PSC.inf",
    "EF_USIM_PB_CC.inf",
    "EF_USIM_PB_PUID.inf",
    "EF_USIM_PB_PBR.inf",
    "EF_USIM_PB_IAP.inf",
    "EF_USIM_PB_ADN.inf",
    "EF_USIM_PB_EXT1.inf",
    "EF_USIM_PB_PBC.inf",
    "EF_USIM_PB_GRP.inf",
    "EF_USIM_PB_AAS.inf",
    "EF_USIM_PB_GAS.inf",
    "EF_USIM_PB_ANR.inf",
    "EF_USIM_PB_SNE.inf",
    "EF_USIM_PB_CCP1.inf",
    "EF_USIM_PB_UID.inf",
    "EF_USIM_PB_EMAIL.inf",
    "EF_USIM_LI.inf",
    "EF_USIM_ARR.inf",
    "EF_USIM_IMSI.inf",
    "EF_USIM_DCK.inf",
    "EF_USIM_HPLMN.inf",
    "EF_USIM_CNL.inf",
    "EF_USIM_ACMMAX.inf",
    "EF_USIM_UST.inf",
    "EF_USIM_ACM.inf",
    "EF_USIM_FDN.inf",
    "EF_USIM_SMS.inf",
    "EF_USIM_GID1.inf",
    "EF_USIM_GID2.inf",
    "EF_USIM_MSISDN.inf",
    "EF_USIM_PUCT.inf",
    "EF_USIM_SMSP.inf",
    "EF_USIM_SMSS.inf",
    "EF_USIM_CBMI.inf",
    "EF_USIM_SPN.inf",
    "EF_USIM_SMSR.inf",
    "EF_USIM_CBMID.inf",
    "EF_USIM_SDN.inf",
    "EF_USIM_EXT2.inf",
    "EF_USIM_EXT3.inf",
    "EF_USIM_BDN.inf",
    "EF_USIM_CBMIR.inf",
    "EF_USIM_EXT4.inf",
    "EF_USIM_EST.inf",
    "EF_USIM_CMI.inf",
    "EF_USIM_PLMNWACT.inf",
    "EF_USIM_OPLMNWACT.inf",
    "EF_USIM_HPLMNWACT.inf",
    "EF_USIM_PSLOCI.inf",
    "EF_USIM_ACC.inf",
    "EF_USIM_FPLMN.inf",
    "EF_USIM_LOCI.inf",
    "EF_USIM_AD.inf",
    "EF_USIM_VGCS.inf",
    "EF_USIM_VGCSS.inf",
    "EF_USIM_VBS.inf",
    "EF_USIM_VBSS.inf",
    "EF_USIM_EMLPP.inf",
    "EF_USIM_AAEM.inf",
    "EF_USIM_ECC.inf",
    "EF_USIM_PNN.inf",
    "EF_USIM_OPL.inf",
    "EF_USIM_MBDN.inf",
    "EF_USIM_EXT6.inf",
    "EF_USIM_MBI.inf",
    "EF_USIM_MWIS.inf",
    "EF_USIM_CFIS.inf",
    "EF_USIM_EXT7.inf",
    "EF_USIM_SPDI.inf",
    "EF_USIM_MMSN.inf",
    "EF_USIM_EXT8.inf",
    "EF_USIM_MMSICP.inf",
    "EF_USIM_MMSUP.inf",
    "EF_USIM_MMSUCP.inf",
    "EF_USIM_NIA.inf",
    "EF_USIM_EHPLMN.inf",
    "EF_USIM_EHPLMNPI.inf",
    "EF_USIM_SPNI.inf",
    "EF_USIM_PNNI.inf",
    "EF_USIM_SOLSA_SAI.inf",
    "EF_USIM_SOLSA_SLL.inf",
    "EF_USIM_MEXE_SST.inf",
    "EF_USIM_MEXE_ORPK.inf",
    "EF_USIM_MEXE_ARPK.inf",
    "EF_USIM_MEXE_TPRPK.inf",
    "EF_CST.inf",
    "EF_ORANGE_OCT.inf",
    "EF_ORANGE_ORANGE_DYN2_FLAG.inf",
    "EF_ORANGE_ORANGE_CSP_LINE2.inf",
    "EF_ORANGE_ORANGE_PARAM.inf",
    "EF_ORANGE_ORANGE_ACM2.inf",
    "EF_ORANGE_ORANGE_DYN_FLAG.inf"
};

//-------------------------------local function declare----------------------------
/**********************************************************************
//    Description:
//      Get the base directory of the sim set files
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void GetBaseDir(int card_num);

/**********************************************************************
//    Description:
//      read CHV from the CHV info file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E GetCHV( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_POS chv_pos,    //the chv position in the CHV file
    SIM_CHV_T *chv      //out:the chv been read out
);

/**********************************************************************
//    Description:
//      update the CHV in the chv info file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E UpdateCHV(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_POS chv_pos,    //the chv position in the CHV file
    SIM_CHV_T chv   //the chv used to update
);

/**********************************************************************
//    Description:
//      get the file path that save the SIM card file select data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E GetSelectFilePath(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path    //out:the pointer of the file path
);


/**********************************************************************
//    Description:
//      get the file path that save the SIM card file content
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E GetFilePath(    //return value:the result of operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path    //out:the pointer of the path
);

/**********************************************************************
//    Description:
//      simulator the Read Transparent file by read the file that save the
//    corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E ReadTrasparentFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file
    uint16 offset,  //the reading offset
    uint8 data_len, //the reading data len
    uint16 *read_data_len,  //out:the data length that have readed
    uint8 *data //out:the data readed out
);

/**********************************************************************
//    Description:
//      simulator the Read record file by read the file that save the
//    corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E ReadRecordFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    uint8 record_num,   //the record num
    uint8 record_len,   //the record len
    uint16 *data_len,   //out:the data len that readed out
    uint8 *data     //out:the data that readed out
);

/**********************************************************************
//    Description:
//      simulator the update Transparent file by update the file that
//    save the corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E UpdateTransparentFile(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16 offset,  //the update offset
    uint8 data_len, //the update data len
    uint8 *data //in:the update data
);

/**********************************************************************
//    Description:
//      simulator the update record file by update the file that
//    save the corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E UpdateRecordFile(   //retrun value:the result of the oepration
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    uint8 record_num,   //the reocrd num to be updated
    uint8 record_len,   //the update data len
    uint8 *data //in:the udpate data
);

/**********the following is for static button sim card setting---start**********/
STATIC SIM_SERVICE_RESULT_E UpdateCHV_static(  //return value:the result of the operation
    SIM_CHV_POS chv_pos,    //the chv position in the CHV file
    SIM_CHV_T chv,   //the chv used to update
    int card_num
);

STATIC SIM_SERVICE_RESULT_E GetCHV_static( //return value:the result of the operation
    SIM_CHV_POS chv_pos,    //the chv position in the CHV file
    SIM_CHV_T *chv,      //out:the chv been read out
    int card_num
);

STATIC SIM_SERVICE_RESULT_E GetSelectFilePath_static(  //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path,   //out:the pointer of the file path
    int card_num
);
/**********the above is for static button sim card setting---end**********/
//---------------------------function define----------------------------------
/**********************************************************************
//    Description:
//      Get the base directory of the sim set files
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMTIW_GetCHVVerificated(
    uint32 cur_sim_task,
    BOOLEAN *chv1_v,    //out:the chv1 verificated state
    BOOLEAN *chv2_v     //out:the chv1 verificated state
)
{
    *chv1_v = CHV1_Verified[cur_sim_task];
    *chv2_v = CHV2_Verified[cur_sim_task];
}

/**********************************************************************
//    Description:
//      Get the base directory of the sim set files
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMTIW_SetCHVVerificated(
    uint32 cur_sim_task,
    BOOLEAN chv1_v, //the chv1 verificated state
    BOOLEAN chv2_v  //the chv1 verificated state
)
{
    CHV1_Verified[cur_sim_task] = chv1_v;
    CHV2_Verified[cur_sim_task] = chv2_v;
}

/**********************************************************************
//    Description:
//      update the CHV in the chv info file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E UpdateCHV(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_POS chv_pos,    //the chv position in the CHV file
    SIM_CHV_T chv   //the chv used to update
)
{
    DWORD file_attribute;
    char str[32];
    //apple change for start problem
    //FILE* chv_file;
    HANDLE hFile;
    int ccbOpr = 0;
    int offset;


    SIM_DISABLE_IRQ

    if(!bBaseDirDecided[cur_sim_task])
    {
        SIMTIW_GetSettingFileDir(base_directory[cur_sim_task], cur_sim_task);

        sprintf(chv_file_path[cur_sim_task], "%schv_file.inf", base_directory[cur_sim_task]);
    }

    SCI_MEMSET(str, 0xff, 32);
    offset = chv_pos * 8;

    //if the file is reade only
    file_attribute = GetFileAttributes(chv_file_path[cur_sim_task]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(chv_file_path[cur_sim_task], file_attribute);
    }

    //chv_file = fopen(chv_file_path[cur_sim_task],"rb");
    //if (chv_file)
    hFile = CreateFile(chv_file_path[cur_sim_task],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //fread(str,32,1,chv_file);
        ReadFile(hFile,
                 str,
                 32,
                 &ccbOpr,
                 NULL);
        CloseHandle(hFile);
        //fclose(chv_file);

    }

    SCI_MEMCPY(str + offset, chv.secret_code, 8);
    //chv_file = fopen(chv_file_path[cur_sim_task],"wb");
    //if (chv_file)
    hFile = CreateFile(chv_file_path[cur_sim_task],
                       GENERIC_WRITE ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fwrite(str,32,1,chv_file) != 1)
        if(!WriteFile(hFile, str, 32, &ccbOpr, NULL) || ccbOpr != 32)
        {
            //fclose(chv_file);
            CloseHandle(hFile);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(chv_file);

        SIM_RESTORE_IRQ
        return SIM_SERVICE_OK;
    }
    else
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      read CHV from the CHV info file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E GetCHV( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_POS chv_pos,    //the chv position in the CHV file
    SIM_CHV_T *chv      //out:the chv been read out
)
{
    //apple change for start problem
    //FILE* chv_file;
    HANDLE hFile;
    int ccbOpr = 0;
    int offset;


    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != chv);

    if(!bBaseDirDecided[cur_sim_task])
    {
        SIMTIW_GetSettingFileDir(base_directory[cur_sim_task], cur_sim_task);

        sprintf(chv_file_path[cur_sim_task], "%schv_file.inf", base_directory[cur_sim_task]);
    }

    offset = chv_pos * 8;
    //chv_file = fopen(chv_file_path[cur_sim_task],"rb");
    //if (chv_file)
    hFile = CreateFile(chv_file_path[cur_sim_task],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fseek(chv_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(chv_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fread(chv->secret_code,8,1,chv_file) != 1)
        if(!ReadFile(hFile,
                     chv->secret_code,
                     8,
                     &ccbOpr,
                     NULL) || ccbOpr != 8)
        {
            CloseHandle(hFile);
            //fclose(chv_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(chv_file);

        SIM_RESTORE_IRQ
        return SIM_SERVICE_OK;
    }
    else
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return  SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//      get the file name that save the SIM card file select data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_GetSelectFileName(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path    //out:the pointer of the file name
)
{

    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != file_path);

    //the file must be a valid sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    sprintf(full_path[cur_sim_task], "%s", select_file[file_name]);

    *file_path = full_path[cur_sim_task];

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      get the file path that save the SIM card file select data
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E GetSelectFilePath(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path    //out:the pointer of the file path
)
{

    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != file_path);

    if(!bBaseDirDecided[cur_sim_task])
    {
        SIMTIW_GetSettingFileDir(base_directory[cur_sim_task], cur_sim_task);

        sprintf(chv_file_path[cur_sim_task], "%schv_file.inf", base_directory[cur_sim_task]);
    }

    //the file must be a valid sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    sprintf(full_path[cur_sim_task], "%s%s", base_directory[cur_sim_task], select_file[file_name]);

    *file_path = full_path[cur_sim_task];

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      get the file path that save the SIM card file content
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E GetFilePath(    //return value:the result of operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path    //out:the pointer of the path
)
{

    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != file_path);

    if(!bBaseDirDecided[cur_sim_task])
    {
        SIMTIW_GetSettingFileDir(base_directory[cur_sim_task], cur_sim_task);

        sprintf(chv_file_path[cur_sim_task], "%schv_file.inf", base_directory[cur_sim_task]);
    }

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    sprintf(full_path[cur_sim_task], "%s%s", base_directory[cur_sim_task], ef_file_name[file_name]);

    *file_path = full_path[cur_sim_task];

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

/**********************************************************************
//    Description:
//      simulator select file in really SIM card by reading the file
//    that save the corresponding sim file select data.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_SelectFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data  //out:the select data of the sim file
)
{
    //FILE* select_file;
    HANDLE hFile;
    char *file_path = NULL;
    int ccbOpr = 0;

    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != select_data);

    //add the dcs 1800 function 2002-11-20
    if((SIMFS_IsDCS1800SIM(0)) && (SIM_FILE_DF_GSM == file_name)) //wuding temmply
    {
        file_name = SIM_FILE_DF_DCS1800;
    }

    GetSelectFilePath(cur_sim_task, file_name, &file_path);
    //select_file = fopen(full_path[cur_sim_task],"rb");
    hFile = CreateFile(full_path[cur_sim_task],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    //if (select_file)
    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fread(select_data,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T),1,select_file) != 1)

        if(!ReadFile(hFile,
                     select_data,
                     sizeof(SIM_RESPONSE_DATA_OF_SELECT_T),
                     &ccbOpr,
                     NULL) ||  ccbOpr !=  sizeof(SIM_RESPONSE_DATA_OF_SELECT_T))
        {
            ////fclose(select_file);
            CloseHandle(hFile);
            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }
        else
        {
            ////fclose(select_file);
            CloseHandle(hFile);
            SIM_RESTORE_IRQ
            return SIM_SERVICE_OK;
        }
    }
    else
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      update the common data of the file select data of a DF or MF
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void UpdateFileCharacter(
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the file name of the file
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data   //the select data of the sim file
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* select_file;
    char *file_path = NULL;
    DWORD file_attribute;
    SIM_RESPONSE_DATA_OF_SELECT_T new_select;

    SIM_DISABLE_IRQ

    if(SIM_FILE_DF_GRAPHICS < file_name)
    {
        SIM_RESTORE_IRQ
        return;
    }

    if(SIM_SERVICE_OK != SIMTIW_SelectFile(cur_sim_task, file_name, &new_select))
    {
        SIM_RESTORE_IRQ
        return;
    }

    SCI_MEMCPY(&(new_select.filecharacter_efstructure), &(select_data->filecharacter_efstructure), (sizeof(SIM_RESPONSE_DATA_OF_SELECT_T) - 13));

    GetSelectFilePath(cur_sim_task, file_name, &file_path);

    //if the file is reade only
    file_attribute = GetFileAttributes(full_path[cur_sim_task]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(full_path[cur_sim_task], file_attribute);
    }

    //select_file = fopen(full_path[cur_sim_task],"wb");
    //if (select_file)
    hFile = CreateFile(full_path[cur_sim_task],
                       GENERIC_WRITE ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fwrite(&new_select,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T),1,select_file) != 1)
        if(!WriteFile(hFile, &new_select, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T), &ccbOpr, NULL)
            || ccbOpr != sizeof(SIM_RESPONSE_DATA_OF_SELECT_T))
        {
            CloseHandle(hFile);
            //fclose(select_file);

            SIM_RESTORE_IRQ
            return;
        }

        CloseHandle(hFile);
        //fclose(select_file);

        SIM_RESTORE_IRQ
        return;
    }
    else
    {

        SIM_RESTORE_IRQ
        return;
    }

    SIM_RESTORE_IRQ
    return;
}

/**********************************************************************
//    Description:
//      update the file that save the corresponding sim file select data
//    with the sim file select data.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_UpdateSelectData(   //the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RESPONSE_DATA_OF_SELECT_T select_data   //the select data of the sim file
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* select_file;
    char *file_path = NULL;
    DWORD file_attribute;
    SIM_FILE_NAME_E temp_file;

    SIM_DISABLE_IRQ


    GetSelectFilePath(cur_sim_task, file_name, &file_path);

    //if the file is reade only
    file_attribute = GetFileAttributes(full_path[cur_sim_task]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(full_path[cur_sim_task], file_attribute);
    }

    //select_file = fopen(full_path[cur_sim_task],"wb");
    //if (select_file)
    hFile = CreateFile(full_path[cur_sim_task],
                       GENERIC_WRITE ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fwrite(&select_data,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T),1,select_file) != 1)
        if(!WriteFile(hFile, &select_data, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T), &ccbOpr, NULL)
            || ccbOpr != sizeof(SIM_RESPONSE_DATA_OF_SELECT_T))
        {
            CloseHandle(hFile);
            //fclose(select_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(select_file);

        //if it's updating the Directory file information,then updating the file
        if(SIM_FILE_DF_GRAPHICS >= file_name)
        {
            for(temp_file = SIM_FILE_MF; temp_file <= SIM_FILE_DF_GRAPHICS; temp_file += 1)
            {
                UpdateFileCharacter(cur_sim_task, temp_file, &select_data);
            }
        }

        SIM_RESTORE_IRQ
        return SIM_SERVICE_OK;
    }
    else
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      simulator the Read Transparent file by read the file that save the
//    corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E ReadTrasparentFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file
    uint16 offset,  //the reading offset
    uint8 data_len, //the reading data len
    uint16 *read_data_len,  //out:the data length that have readed
    uint8 *data //out:the data readed out
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    int true_len;
    char *file_path = NULL;


    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != read_data_len);
    SCI_ASSERT(NULL != data);

    GetFilePath(cur_sim_task, file_name, &file_path);
    //sim_file = fopen(full_path[cur_sim_task],"rb");
    //if (sim_file)

    hFile = CreateFile(full_path[cur_sim_task],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        true_len = 0 == data_len ? SIM_MAX_R_APDU_DATA_LEN : data_len;
        //if (fseek(sim_file,0,SEEK_END) != 0)
        ccbOpr = SetFilePointer(hFile, 0, NULL, FILE_END);

        if(ccbOpr == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        //*read_data_len = (uint16)(ftell(sim_file) - offset);
        *read_data_len = ccbOpr - offset;

        if(true_len > *read_data_len)
        {
            true_len =  *read_data_len;
        }

        //if (fseek(sim_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        //get the really data length
        *read_data_len = true_len;

        //*read_data_len  = (uint8)fread(data,1,true_len,sim_file);
        ReadFile(hFile,
                 data,
                 true_len,
                 &ccbOpr,
                 NULL);
        *read_data_len = ccbOpr;

        if(*read_data_len <= 0)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }
        else
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_OK;
        }
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/********************************************************************************/
STATIC int32 GetFileOffset(
    uint32                     cur_sim_task,
    SIM_FILE_NAME_E            file_name,      //the SIM file
    SIM_RECORD_ACCESS_MODE_E   record_mode,    //the record access mode
    uint8                      record_num,     //the record num
    uint8                      record_len      //the record len
)
{
    int32   offset = -1;    // invalid

    if(SIM_RECORD_ABSOLUTE == record_mode)
    {
        if(0 < record_num)
        {
            cur_record_num[cur_sim_task] = record_num;
        }

        offset = (cur_record_num[cur_sim_task] - 1) * record_len;
    }
    else
    {
        if(SIM_RECORD_NEXT == record_mode)
        {
            cur_record_num[cur_sim_task]++;

            if((cur_record_num[cur_sim_task] > g_sim_card_setting[cur_sim_task].lnd_info.max_record_num)
                && (SIM_FILE_EF_LND == file_name))
            {
                cur_record_num[cur_sim_task] = 1;
            }

            offset = (cur_record_num[cur_sim_task] - 1) * record_len;
        }
        else if(SIM_RECORD_PREVIOUS == record_mode)
        {
            cur_record_num[cur_sim_task]--;

            if((0 == cur_record_num[cur_sim_task])
                && (SIM_FILE_EF_LND == file_name))
            {
                cur_record_num[cur_sim_task] = g_sim_card_setting[cur_sim_task].lnd_info.max_record_num;
            }

            offset = (cur_record_num[cur_sim_task] - 1) * record_len;
        }
        else
        {
            offset = -1;
        }
    }

    return (offset);
}

/**********************************************************************
//    Description:
//      simulator the Read record file by read the file that save the
//    corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E ReadRecordFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    uint8 record_num,   //the record num
    uint8 record_len,   //the record len
    uint16 *data_len,   //out:the data len that readed out
    uint8 *data     //out:the data that readed out
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    int offset, true_record_len;
    char *file_path = NULL;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != data_len);
    SCI_ASSERT(NULL != data);

    true_record_len = 0 == record_len ? SIM_MAX_R_APDU_DATA_LEN : record_len;
    *data_len = true_record_len;
    GetFilePath(cur_sim_task, file_name, &file_path);
    //sim_file = fopen(full_path[cur_sim_task],"rb");
    //if (sim_file)
    hFile = CreateFile(full_path[cur_sim_task],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        offset = GetFileOffset(cur_sim_task, file_name, record_mode, record_num, true_record_len);

        if(0 > offset)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fseek(sim_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fread(data,true_record_len,1,sim_file) != 1)
        if(!ReadFile(hFile,
                     data,
                     true_record_len,
                     &ccbOpr,
                     NULL) || ccbOpr != true_record_len)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(sim_file);

        SIM_RESTORE_IRQ
        return  SIM_SERVICE_OK;
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//      simulator the Read EF file by read the file that save the
//    corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_ReadFile(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16  offset, //the reading offset of the transparent file
    uint8   record_num, //the record num of the record file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of the record file
    uint16 *filesize_recordlen, //the file sizeof transparent file or the record len of record file
    uint16 *data_len,   //the data len that readed out
    uint8  *data    //the data that readed out
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != filesize_recordlen);
    SCI_ASSERT(NULL != data_len);
    SCI_ASSERT(NULL != data);

    //get the select data of the file
    ser_result = SIMTIW_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //if the file is not a EF file
    if(SIM_FILE_T_EF != select_data.file_type)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    /*  //check the access condition
        GET_FILE_ACCESS_CONDITION(file_access,select_data);
        if (SIM_ACCESS_CHV1 == file_access.seek_read_access)
        {
            if (!CHV1_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 == file_access.seek_read_access)
        {
            if (!CHV2_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 < file_access.seek_read_access)
        {
            return SIM_SERVICE_ACCESS_FAIL;
        }*/

    //do different read for the special file structure
    //read transparent file,corresponding to Read Binary
    if(SIM_EF_S_TRANSPARENT == select_data.filecharacter_efstructure)
    {
        *filesize_recordlen = select_data.freespace_filesize;

        SIM_RESTORE_IRQ
        return  ReadTrasparentFile(cur_sim_task, file_name, offset, record_num, data_len, data);
    }
    //read record file corespoding to Read Record
    else if((SIM_EF_S_LINEAR_FIXED == select_data.filecharacter_efstructure) ||
            (SIM_EF_S_CYCLIC == select_data.filecharacter_efstructure))
    {
        *filesize_recordlen = select_data.dfnumber_recordlen == 0 ? SIM_MAX_R_APDU_DATA_LEN : select_data.dfnumber_recordlen;

        SIM_RESTORE_IRQ
        return  ReadRecordFile(cur_sim_task, file_name, mode, record_num, select_data.dfnumber_recordlen, data_len, data);
    }
    //the unknow file sturcture
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      simulator the update Transparent file by update the file that
//    save the corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E UpdateTransparentFile(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16 offset,  //the update offset
    uint8 data_len, //the update data len
    uint8 *data //in:the update data
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    int true_len;
    char *file_path = NULL;
    DWORD file_attribute;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != data);

    true_len = data_len;

    GetFilePath(cur_sim_task, file_name, &file_path);

    //if the file is reade only
    file_attribute = GetFileAttributes(full_path[cur_sim_task]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(full_path[cur_sim_task], file_attribute);
    }

    //sim_file = fopen(full_path[cur_sim_task],"r+b");
    //if (sim_file)
    hFile = CreateFile(full_path[cur_sim_task],
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fseek(sim_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fwrite(data,true_len,1,sim_file) != 1)
        if(!WriteFile(hFile, data, true_len, &ccbOpr, NULL)
            || ccbOpr != true_len)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(sim_file);

        SIM_RESTORE_IRQ
        return SIM_SERVICE_OK;
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//      simulator the update record file by update the file that
//    save the corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_RESULT_E UpdateRecordFile(   //retrun value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    uint8 record_num,   //the record num to be updated
    uint8 record_len,   //the update data len
    uint8 *data //in:the update data
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    char *file_path = NULL;
    int offset, true_record_len;
    DWORD file_attribute;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != data);
    SCI_ASSERT(((record_num >= 1) && (SIM_RECORD_ABSOLUTE == record_mode))
               || (SIM_RECORD_PREVIOUS == record_mode)
               || (SIM_RECORD_NEXT == record_mode));

    true_record_len = record_len;
    offset = GetFileOffset(cur_sim_task, file_name, record_mode, record_num, true_record_len);

    if(0 > offset)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    GetFilePath(cur_sim_task, file_name, &file_path);

    //if the file is reade only
    file_attribute = GetFileAttributes(full_path[cur_sim_task]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(full_path[cur_sim_task], file_attribute);
    }

    //sim_file = fopen(full_path[cur_sim_task],"r+b");
    //if (sim_file)
    hFile = CreateFile(full_path[cur_sim_task],
                       GENERIC_WRITE | GENERIC_READ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fseek(sim_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fwrite(data,true_record_len,1,sim_file) != 1)
        if(!WriteFile(hFile, data, true_record_len, &ccbOpr, NULL)
            || ccbOpr != true_record_len)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(sim_file);

        SIM_RESTORE_IRQ
        return  SIM_SERVICE_OK;
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      simulator the update EF file by update the file that
//    save the corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_UpdateFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16  offset, //the update offset of the transparent file
    uint8       record_num, //the record num of the record file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode
    uint8       data_len,   //the update data len
    uint8  *data    //in:the update data
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_FILE_ACCESS_CONDITION_T file_access;

    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != data);

    //get the select data of the file
    ser_result = SIMTIW_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //if the file is not a EF file
    if(SIM_FILE_T_EF != select_data.file_type)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    if(SIM_FILE_EF_ACMmax == file_name)
    {

        //check the access condition
        GET_FILE_ACCESS_CONDITION(file_access, (&select_data));

        if((SIM_ACCESS_CHV1 == file_access.update_access) &&
            ((!CHV1_Verified[cur_sim_task]) && (g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled)))
        {
            if(g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked)
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV1_NEEDED;
            }
            else if(g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.chv_blocked)
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_U_CHV1_NEEDED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV_BLOCKED;
            }
        }
        else if((SIM_ACCESS_CHV2 == file_access.update_access) && (!CHV2_Verified[cur_sim_task]))
        {
            if(g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked)
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV2_NEEDED;
            }
            else if(g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.chv_blocked)
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_U_CHV2_NEEDED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV_BLOCKED;
            }
        }
        else if(SIM_ACCESS_CHV2 < file_access.update_access)
        {
            SIM_RESTORE_IRQ
            return SIM_UPDATE_ACCESS_NEVER;
        }
    }

    //do different update for the special file structure
    //update transparent file,corresponding to Update Binary
    if(SIM_EF_S_TRANSPARENT == select_data.filecharacter_efstructure)
    {
        SIM_RESTORE_IRQ
        return  UpdateTransparentFile(cur_sim_task, file_name, offset, data_len, data);
    }
    //update record file corespoding to Update Record
    else if((SIM_EF_S_LINEAR_FIXED == select_data.filecharacter_efstructure) ||
            (SIM_EF_S_CYCLIC == select_data.filecharacter_efstructure))
    {
        SIM_RESTORE_IRQ
        return  UpdateRecordFile(cur_sim_task, file_name, mode, record_num, select_data.dfnumber_recordlen, data);
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      simulator the Seek in file by Seek the file that save the
//    corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_SeekInFile( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_SEEK_TYPE_E seek_type,  //the seek type
    SIM_SEEK_MODE_E seek_mode,  //the seek mode
    uint8   pattern_len,    //the seek patter len
    uint8 *pattern, //in:the seek patter
    uint8 *record_num   //out:the reocor num that been seek out
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    uint8 *whole_file, *single_record, *ptemp;
    uint16 total_record_num, i, j;
    BOOLEAN the_same;
    char *file_path = NULL;
    uint8 cur_record = 0;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != pattern);
    SCI_ASSERT(NULL != record_num);

    //get the select data of the file
    ser_result = SIMTIW_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //if the file is not a EF file
    if(SIM_FILE_T_EF != select_data.file_type)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    //the seek file must be a fixed linear or cyclic structure
    if((SIM_EF_S_LINEAR_FIXED != select_data.filecharacter_efstructure) &&
        (SIM_EF_S_CYCLIC != select_data.filecharacter_efstructure))
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    //check the pattern len,ensure not longer than the record length
    if((0 == pattern_len) ||
        (pattern_len > select_data.dfnumber_recordlen))
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_INCONSISTENT_FILE;
    }

    /*  //check the access condition
        GET_FILE_ACCESS_CONDITION(file_access,select_data);
        if (SIM_ACCESS_CHV1 == file_access.seek_read_access)
        {
            if (!CHV1_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 == file_access.seek_read_access)
        {
            if (!CHV2_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 < file_access.seek_read_access)
        {
            return SIM_SERVICE_ACCESS_FAIL;
        }
    */
    //open the EF file to seek
    GetFilePath(cur_sim_task, file_name, &file_path);
    //sim_file = fopen(full_path[cur_sim_task],"rb");
    //if (NULL == sim_file)
    hFile = CreateFile(full_path[cur_sim_task],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile == INVALID_HANDLE_VALUE)
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //alloc buf for seek
    whole_file = (uint8 *)SCI_ALLOC_BASE(select_data.freespace_filesize);
    single_record = (uint8 *)SCI_ALLOC_BASE(select_data.dfnumber_recordlen);

    //read the whole file to the buf at once
    //if (fread(whole_file,select_data.freespace_filesize,1,sim_file) != 1)
    if(!ReadFile(hFile,
                 whole_file,
                 select_data.freespace_filesize,
                 &ccbOpr,
                 NULL) || ccbOpr != select_data.freespace_filesize)
    {
        SCI_FREE(whole_file);
        SCI_FREE(single_record);
        CloseHandle(hFile);
        //fclose(sim_file);

        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //get the total record num in the sim file
    total_record_num = select_data.freespace_filesize / select_data.dfnumber_recordlen;


    //start to seek,only support the type2 and From the beginning forwards seek
    if(seek_mode == SIM_SEEK_NEXT_FORWARD)
    {
        cur_record = cur_ext_record_id[cur_sim_task];
    }
    else
    {
        cur_record = 0;
    }

    ptemp = whole_file + cur_record * select_data.dfnumber_recordlen;

    for(i = cur_record; i < total_record_num; i++)
    {
        //get a record
        SCI_MEMCPY(single_record, ptemp, select_data.dfnumber_recordlen);
        ptemp += select_data.dfnumber_recordlen;

        the_same = TRUE;

        //compare the record with the pattern string
        for(j = 0; j < select_data.dfnumber_recordlen; j++)
        {
            if(single_record[j] != pattern[j])
            {
                the_same = FALSE;
                break;
            }
        }

        //find the record that with the pattern string
        if(the_same)
        {
            SCI_FREE(whole_file);
            SCI_FREE(single_record);
            CloseHandle(hFile);
            //fclose(sim_file);
            *record_num = (uint8)(i + 1);
            cur_ext_record_id[cur_sim_task] = (uint8)(i + 1);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_OK;
        }
    }

    //there are none record with the patten string in the file
    SCI_FREE(whole_file);
    SCI_FREE(single_record);
    CloseHandle(hFile);
    //fclose(sim_file);

    SIM_RESTORE_IRQ
    return SIM_SERVICE_SEEK_NOT_FIND_PATTERN;
}


/**********************************************************************
//    Description:
//      simulator the increase file by increase the file that save the
//    corresponding sim file content.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_Increase(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_INSTR_INCREASE_DATA_T data,   //the increase data
    SIM_INSTR_INCREASE_RESULT_T *result //out:the file content after increase
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_ACMMAX_T acm_max;
    SIM_EF_MAPPING_ACM_T acm;

    SCI_ASSERT(NULL != result);
    SCI_ASSERT(SIM_FILE_EF_ACM == file_name);

    ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_ACMmax, &acm_max); //wuding temply...

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_ACM, 1, SIM_RECORD_ABSOLUTE, &acm); //wuding temply

    if(SIM_SERVICE_OK != ser_result)
    {
        return ser_result;
    }

    //check the acm value and the acm max
    if(acm_max.acm_max_value < acm.acm_value)
    {
        return SIM_SERVICE_INCREASE_MAX_VALUE;
    }

    acm.acm_value += ((uint32)(data.data[0] << 16) + (uint32)(data.data[1] << 8) + data.data[2]);

    if(0xffffff < acm.acm_value)
    {
        return SIM_SERVICE_INCREASE_MAX_VALUE;
    }

    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_ACM, 1, SIM_RECORD_ABSOLUTE, &acm, FALSE); //wuding temmply

    if(SIM_SERVICE_OK == ser_result)
    {
        SCI_MEMCPY(&(result->added_value), &data, sizeof(SIM_INSTR_INCREASE_DATA_T));
        result->new_value.data[0] = (uint8)(acm.acm_value >> 16);
        result->new_value.data[1] = (uint8)(acm.acm_value >> 8);
        result->new_value.data[2] = (uint8)acm.acm_value;
    }

    return ser_result;
}



/**********************************************************************
//    Description:
//      simulator the verify CHV in SIM card by verify corresponding chv
//    in the chv saved file.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_VerifyCHV(  //return value:the reuslt of the operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E chv_indication,    //the chv indication
    SIM_CHV_T sim_chv   //the chv to be verifyed
)
{
    SIM_CHV_POS chv_pos;
    SIM_CHV_T card_chv;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result;
    uint8 i, remain_false;


    SIM_DISABLE_IRQ


    ser_result = SIMTIW_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return ser_result;
    }

    if(SIM_CHV1 == chv_indication)
    {
        //chv1 secrect code initialized or not
        if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv1_status))
        {
            SIM_RESTORE_IRQ
            return SIM_SERVICE_CHV_NOT_INITIALIZED;
        }

        //if the pin1 is disalbed,the change operation should not be excuted
        if((SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK & select_data.filecharacter_efstructure))
        {
            SIM_RESTORE_IRQ
            return  SIM_SERVICE_CHV_CONTRADICT_STATUS;
        }

        //the remain false number of chv 1
        remain_false = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv1_status;

        //the remain false number is 0,means the chv1 already get blocked
        if(0 == remain_false)
        {
            SIM_RESTORE_IRQ
            return SIM_SERVICE_CHV_CONTRADICT_STATUS;
        }

        chv_pos = SIM_POS_CHV1;
    }
    else if(SIM_CHV2 == chv_indication)
    {
        //the chv2 secret code initialized or not
        if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv2_status))
        {
            SIM_RESTORE_IRQ
            return SIM_SERVICE_CHV_NOT_INITIALIZED;
        }

        //the remain false number of chv2
        remain_false = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv2_status;

        //the remain false number is 0,means the chv already get blocked
        if(0 == remain_false)
        {
            SIM_RESTORE_IRQ
            return SIM_SERVICE_CHV_CONTRADICT_STATUS;
        }

        chv_pos = SIM_POS_CHV2;
    }
    else
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    ser_result = GetCHV(cur_sim_task, chv_pos, &card_chv);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return ser_result;
    }

    //compared with the input sim chv
    for(i = 0; i < 8; i++)
    {
        if(sim_chv.secret_code[i] != card_chv.secret_code[i])
        {
            //update the gsm's  select data
            remain_false--;

            if(SIM_CHV1 == chv_indication)
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain--;
                select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
                select_data.chv1_status = select_data.chv1_status | remain_false;
            }
            else
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain--;
                select_data.chv2_status = select_data.chv2_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
                select_data.chv2_status = select_data.chv2_status | remain_false;
            }

            SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);

            //the chv is blocked at the error chv
            if(0 == remain_false)
            {
                if(SIM_CHV1 == chv_indication)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
                }
                else
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked = TRUE;
                }

                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_VERIFY_CHV_ERROR;
            }
        }
    }

    //update the gsm's  select data
    remain_false = 3;

    if(SIM_CHV1 == chv_indication)
    {
        CHV1_Verified[cur_sim_task] = TRUE;
        g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = remain_false;
        select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
        select_data.chv1_status = select_data.chv1_status | remain_false;
    }
    else
    {
        CHV2_Verified[cur_sim_task] = TRUE;
        g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = remain_false;
        select_data.chv2_status = select_data.chv2_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
        select_data.chv2_status = select_data.chv2_status | remain_false;
    }

    SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);

    SIM_RESTORE_IRQ
    //the verify chv is ok
    return SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//      simulator the change CHV in SIM card by change corresponding chv
//    in the chv saved file.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_ChangeCHV(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E chv_indication,    //the chv indication
    SIM_CHV_T old_chv,  //the old chv
    SIM_CHV_T new_chv   //the new chv
)
{
    SIM_CHV_POS chv_pos;
    SIM_CHV_T card_chv;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result;
    int i, remain_false;

    SIM_DISABLE_IRQ

    ser_result = SIMTIW_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return ser_result;
    }

    if(SIM_CHV1 == chv_indication)
    {
        //chv1 secrect code initialized or not
        if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv1_status))
        {
            SIM_RESTORE_IRQ
            return SIM_SERVICE_CHV_NOT_INITIALIZED;
        }

        //if the pin1 is disalbed,the change operation should not be excuted
        if((SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK & select_data.filecharacter_efstructure))
        {
            SIM_RESTORE_IRQ
            return  SIM_SERVICE_CHV_CONTRADICT_STATUS;
        }

        //the remain false number of chv 1
        remain_false = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv1_status;

        //the remain false number is 0,means the chv1 already get blocked
        if(0 == remain_false)
        {
            SIM_RESTORE_IRQ
            return SIM_SERVICE_CHV_CONTRADICT_STATUS;
        }

        chv_pos = SIM_POS_CHV1;
    }
    else if(SIM_CHV2 == chv_indication)
    {
        //the chv2 secret code initialized or not
        if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv2_status))
        {
            SIM_RESTORE_IRQ
            return SIM_SERVICE_CHV_NOT_INITIALIZED;
        }

        //the remain false number of chv2
        remain_false = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv2_status;

        //the remain false number is 0,means the chv already get blocked
        if(0 == remain_false)
        {
            SIM_RESTORE_IRQ
            return SIM_SERVICE_CHV_CONTRADICT_STATUS;
        }

        chv_pos = SIM_POS_CHV2;
    }
    else
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    //get the chv in the card
    ser_result = GetCHV(cur_sim_task, chv_pos, &card_chv);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return ser_result;
    }

    //compared with the input old chv
    for(i = 0; i < 8; i++)
    {
        if(old_chv.secret_code[i] != card_chv.secret_code[i])
        {
            //update the gsm's  select data
            remain_false--;

            if(SIM_CHV1 == chv_indication)
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = remain_false;
                select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
                select_data.chv1_status = select_data.chv1_status | remain_false;
            }
            else
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = remain_false;
                select_data.chv2_status = select_data.chv2_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
                select_data.chv2_status = select_data.chv2_status | remain_false;
            }

            SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);

            //the chv is blocked at the error chv
            if(0 == remain_false)
            {
                SIM_RESTORE_IRQ

                if(SIM_CHV1 == chv_indication)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
                }
                else
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.chv_blocked = TRUE;
                }

                return SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHANGE_CHV_ERROR;
            }
        }
    }

    remain_false = 3;

    if(SIM_CHV1 == chv_indication)
    {
        CHV1_Verified[cur_sim_task] = TRUE;
        g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = remain_false;
        select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
        select_data.chv1_status = select_data.chv1_status | remain_false;
    }
    else
    {
        CHV2_Verified[cur_sim_task] = TRUE;
        g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = remain_false;
        select_data.chv2_status = select_data.chv2_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
        select_data.chv2_status = select_data.chv2_status | remain_false;
    }

    SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);

    SIM_RESTORE_IRQ
    return UpdateCHV(cur_sim_task, chv_pos, new_chv);
}


/**********************************************************************
//    Description:
//      simulator the disable CHV in SIM card by verify the chv1 in the
//    chv saved file and then disable the chv in the file that save the
//    select data of EF_GSM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_DisableCHV( //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_T chv1  //the chv1
)
{
    SIM_CHV_POS chv_pos;
    SIM_CHV_T card_chv;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result;
    int i, remain_false;


    SIM_DISABLE_IRQ


    ser_result = SIMTIW_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }


    //if the chv is disabled,the chv verify finished successful
    if((SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK & select_data.filecharacter_efstructure))
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_CHV_CONTRADICT_STATUS;
    }

    //chv1 secrect code initialized or not
    if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv1_status))
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_CHV_NOT_INITIALIZED;
    }

    //the remain false number of chv 1
    remain_false = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv1_status;

    //the remain false number is 0,means the chv1 already get blocked
    if(0 == remain_false)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_CHV_CONTRADICT_STATUS;
    }

    //get the chv in the card
    chv_pos = SIM_POS_CHV1;
    ser_result = GetCHV(cur_sim_task, chv_pos, &card_chv);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //compared with the input old chv
    for(i = 0; i < 8; i++)
    {
        if(chv1.secret_code[i] != card_chv.secret_code[i])
        {
            //update the gsm's  select data
            remain_false--;
            g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = remain_false;
            select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
            select_data.chv1_status = select_data.chv1_status | remain_false;
            SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);

            //the chv is blocked at the error chv
            if(0 == remain_false)
            {

                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
                SIM_RESTORE_IRQ
                return  SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return  SIM_SERVICE_CHANGE_CHV_ERROR;
            }
        }
    }

    //update the gsm's  select data
    remain_false = 3;
    CHV1_Verified[cur_sim_task] = TRUE;
    g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled = FALSE;
    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = remain_false;
    select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
    select_data.chv1_status = select_data.chv1_status | remain_false;

    //set chv disable
    g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled = FALSE;
    select_data.filecharacter_efstructure = (SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK) | select_data.filecharacter_efstructure;

    SIM_RESTORE_IRQ
    return SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);
}


/**********************************************************************
//    Description:
//      simulator the enable CHV in SIM card by verify the chv1 in the
//    chv saved file and then enable the chv in the file that save the
//    select data of EF_GSM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_EnableCHV(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_CHV_T chv1  //the chv1
)
{
    SIM_CHV_POS chv_pos;
    SIM_CHV_T card_chv;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result;
    int i, remain_false;


    SIM_DISABLE_IRQ


    ser_result = SIMTIW_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }


    //if the chv is enabled,the chv enable is error
    if(!(SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK & select_data.filecharacter_efstructure))
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_CHV_CONTRADICT_STATUS;
    }

    //chv1 secrect code initialized or not
    if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv1_status))
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_CHV_NOT_INITIALIZED;
    }

    //the remain false number of chv 1
    remain_false = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.chv1_status;

    //the remain false number is 0,means the chv1 already get blocked
    if(0 == remain_false)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_CHV_CONTRADICT_STATUS;
    }

    //get the chv in the card
    chv_pos = SIM_POS_CHV1;
    ser_result = GetCHV(cur_sim_task, chv_pos, &card_chv);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //compared with the input old chv
    for(i = 0; i < 8; i++)
    {
        if(chv1.secret_code[i] != card_chv.secret_code[i])
        {
            //update the gsm's  select data
            remain_false--;
            g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = remain_false;
            select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
            select_data.chv1_status = select_data.chv1_status | remain_false;
            SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);

            //the chv is blocked at the error chv
            if(0 == remain_false)
            {
                g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
                SIM_RESTORE_IRQ
                return  SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return  SIM_SERVICE_CHANGE_CHV_ERROR;
            }
        }
    }

    //update the gsm's  select data
    remain_false = 3;
    CHV1_Verified[cur_sim_task] = TRUE;
    g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled = TRUE;
    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = remain_false;
    select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
    select_data.chv1_status = select_data.chv1_status | remain_false;

    //set chv enable
    //@yongxia.zhang,cr7016,2004-02-02,begin
    //g_sim_card_setting.gsm_data.chv1_status.chv_blocked = TRUE;
    g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_verified = TRUE;
    //cr7016,end
    select_data.filecharacter_efstructure = (~SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK) & select_data.filecharacter_efstructure;

    SIM_RESTORE_IRQ
    return SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);
}



/**********************************************************************
//    Description:
//      simulator the unblock CHV in SIM card by verify the unblock chv
//    in the chv saved file and then change the chv1 in the chv saved
//    file and the change the chv status in the file that save the select
//    data of DF_GSM
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_UnblockCHV( //return value:the result of operation
    uint32 cur_sim_task,
    SIM_CHV_INDICATION_E chv_indication,    //chv indication
    SIM_CHV_T unblock_chv,  //the unblock chv
    SIM_CHV_T new_chv   //the new chv
)
{
    SIM_CHV_POS chv_pos;
    SIM_CHV_T       card_chv;
    SIM_RESPONSE_DATA_OF_SELECT_T   select_data;
    SIM_SERVICE_RESULT_E    ser_result;
    int i, remain_false;


    SIM_DISABLE_IRQ


    ser_result = SIMTIW_SelectFile(cur_sim_task, SIM_FILE_DF_GSM, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    if(SIM_CHV1 == chv_indication)
    {
        //chv1 secrect code initialized or not
        if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv1_status))
        {
            SIM_RESTORE_IRQ
            return  SIM_SERVICE_CHV_NOT_INITIALIZED;
        }

        //unblock chv1 secrect code initialized or not
        if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.unblock_chv1_status))
        {
            SIM_RESTORE_IRQ
            return  SIM_SERVICE_CHV_NOT_INITIALIZED;
        }

        //the remain false number of unblock chv 1
        remain_false = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.unblock_chv1_status;

        //the remain false number is 0,means the unblock chv1 already get blocked
        if(0 == remain_false)
        {
            SIM_RESTORE_IRQ
            return  SIM_SERVICE_CHV_CONTRADICT_STATUS;
        }

        chv_pos = SIM_POS_UNBLOCK_CHV1;
    }
    else if(SIM_CHV2 == chv_indication)
    {
        //chv2 secrect code initialized or not
        if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.chv2_status))
        {
            SIM_RESTORE_IRQ
            return  SIM_SERVICE_CHV_NOT_INITIALIZED;
        }

        //unblock chv2 secrect code initialized or not
        if(!(SIM_GSM_SPECIFIC_CS_SC_INITIAL_MARK & select_data.unblock_chv2_status))
        {
            SIM_RESTORE_IRQ
            return  SIM_SERVICE_CHV_NOT_INITIALIZED;
        }

        //the remain false number of unblock chv 2
        remain_false = SIM_GSM_SPECIFIC_CS_RAMAIN_MARK & select_data.unblock_chv2_status;

        //the remain false number is 0,means the unblock chv2 already get blocked
        if(0 == remain_false)
        {
            SIM_RESTORE_IRQ
            return  SIM_SERVICE_CHV_CONTRADICT_STATUS;
        }

        chv_pos = SIM_POS_UNBLOCK_CHV2;
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    //get the unblock chv
    ser_result = GetCHV(cur_sim_task, chv_pos, &card_chv);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //check the input unblock chv
    for(i = 0; i < 8; i++)
    {
        if(unblock_chv.secret_code[i] != card_chv.secret_code[i])
        {
            //update the gsm's  select data
            remain_false--;

            if(SIM_CHV1 == chv_indication)
            {
                g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain = remain_false;
                select_data.unblock_chv1_status = select_data.unblock_chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
                select_data.unblock_chv1_status = select_data.unblock_chv1_status | remain_false;
            }
            else
            {
                g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.false_remain = remain_false;
                select_data.unblock_chv2_status = select_data.unblock_chv2_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
                select_data.unblock_chv2_status = select_data.unblock_chv2_status | remain_false;
            }

            SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);

            //the chv is blocked at the error chv
            if(0 == remain_false)
            {
                if(SIM_CHV1 == chv_indication)
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.chv_blocked = TRUE;
                }
                else
                {
                    g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv2_status.chv_blocked = TRUE;
                }

                SIM_RESTORE_IRQ
                return  SIM_SERVICE_CHV_FAIL_WITH_BLOCKED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return  SIM_SERVICE_CHANGE_CHV_ERROR;
            }
        }
    }

    if(SIM_CHV1 == chv_indication)
    {
        //set the chv1 remain false  to 3
        remain_false = 3;
        CHV1_Verified[cur_sim_task] = TRUE;
        g_sim_card_setting[cur_sim_task].gsm_data.chv1_enabled = TRUE;
        g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.false_remain = remain_false;
        g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = FALSE;
        select_data.filecharacter_efstructure = (~SIM_GSM_SPECIFIC_CHV1_ENABLE_MARK) & select_data.filecharacter_efstructure;
        select_data.chv1_status = select_data.chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
        select_data.chv1_status = select_data.chv1_status | remain_false;
        //upadte the chv1 by the new chv
        UpdateCHV(cur_sim_task, SIM_POS_CHV1, new_chv);
        //set unblock chv1 remain false to 10
        remain_false = 10;
        g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain = remain_false;
        select_data.unblock_chv1_status = select_data.unblock_chv1_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
        select_data.unblock_chv1_status = select_data.unblock_chv1_status | remain_false;
    }
    else
    {
        //set the chv2 remain false  to 3
        remain_false = 3;
        CHV2_Verified[cur_sim_task] = TRUE;
        g_sim_card_setting[cur_sim_task].gsm_data.chv2_status.false_remain = remain_false;
        g_sim_card_setting[cur_sim_task].gsm_data.chv1_status.chv_blocked = TRUE;
        select_data.chv2_status = select_data.chv2_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
        select_data.chv2_status = select_data.chv2_status | remain_false;
        //upadte the chv2 by the new chv
        UpdateCHV(cur_sim_task, SIM_POS_CHV2, new_chv);
        //set unblock chv2 remain false to 10
        remain_false = 10;
        g_sim_card_setting[cur_sim_task].gsm_data.unblock_chv1_status.false_remain = remain_false;
        select_data.unblock_chv2_status = select_data.unblock_chv2_status & (~SIM_GSM_SPECIFIC_CS_RAMAIN_MARK);
        select_data.unblock_chv2_status = select_data.unblock_chv2_status | remain_false;
    }

    SIM_RESTORE_IRQ
    return SIMTIW_UpdateSelectData(cur_sim_task, SIM_FILE_DF_GSM, select_data);
}


/**********************************************************************
//    Description:
//      simulator invlidate EF file by change validate in the file that
//    save the select data of the corresponding EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_InvalidateFile( //return value:the result of the operatin
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name   //the sim file name
)
{
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result;


    SIM_DISABLE_IRQ

    //get select data of the file
    ser_result = SIMTIW_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //check the access condition
    /*  GET_FILE_ACCESS_CONDITION(file_access,select_data);

        if (SIM_ACCESS_CHV1 == file_access.invalidate_access)
        {
            if (!CHV1_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 == file_access.invalidate_access)
        {
            if (!CHV2_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 < file_access.invalidate_access)
        {
            return SIM_SERVICE_ACCESS_FAIL;
        }*/

    //if the file is not a EF file
    if(SIM_FILE_T_EF != select_data.file_type)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    //the file have been invalidate
    if(!(select_data.file_status & SIM_SELECT_INVALIDATE_MARK))
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_CONTRADICT_VALIDATION;
    }

    //mark the invalide mark
    select_data.file_status = select_data.file_status & (~SIM_SELECT_INVALIDATE_MARK);

    SIM_RESTORE_IRQ
    return SIMTIW_UpdateSelectData(cur_sim_task, file_name, select_data);
}


/**********************************************************************
//    Description:
//      simulator rehabilitate EF file by change validate in the file that
//    save the select data of the corresponding EF file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_RehabilitateFile(   //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name   //the sim file name
)
{
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_SERVICE_RESULT_E ser_result;


    SIM_DISABLE_IRQ

    //get select data of the file
    ser_result = SIMTIW_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //if the file is not a EF file
    if(SIM_FILE_T_EF != select_data.file_type)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    //check the access condition

    /*  GET_FILE_ACCESS_CONDITION(file_access,select_data);
        if (SIM_ACCESS_CHV1 == file_access.invalidate_access)
        {
            if (!CHV1_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 == file_access.invalidate_access)
        {
            if (!CHV2_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 < file_access.invalidate_access)
        {
            return SIM_SERVICE_ACCESS_FAIL;
        }

    */
    //the file have been validate
    if(select_data.file_status & SIM_SELECT_INVALIDATE_MARK)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_CONTRADICT_VALIDATION;
    }

    //mark the invalide mark
    select_data.file_status = select_data.file_status | SIM_SELECT_INVALIDATE_MARK;

    SIM_RESTORE_IRQ
    return SIMTIW_UpdateSelectData(cur_sim_task, file_name, select_data);
}



/**********************************************************************
//    Description:
//      simulator run GSM algorithm by return the fixed algorithm result.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_RunGSMAlgorithm(    //return value:the result of the opertion
    RAND_T rand,    //the algorithm rand
    SIM_GSM_ALGORITHM_RESULT_T *result  //out:the algorithm result
)
{
    SCI_ASSERT(NULL != result);

    SCI_MEMSET(&(result->kc), '*', SIM_EF_KC_KC_LEN);
    SCI_MEMSET(&(result->sers), '&', SIM_SERS_LEN);
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      simulator the sim sleep operation
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_Sleep(void) //return value:the result of the operation
{
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      get all the chvs that save in the chv file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_GetAllCHV(  //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_ALL_CHV_T *chvs //out:all of the sim chv
)
{
    SIM_SERVICE_RESULT_E ser_result;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != chvs);

    ser_result = GetCHV(cur_sim_task, SIM_POS_CHV1, &(chvs->chv1));

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = GetCHV(cur_sim_task, SIM_POS_UNBLOCK_CHV1, &(chvs->unblock_chv1));

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = GetCHV(cur_sim_task, SIM_POS_CHV2, &(chvs->chv2));

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = GetCHV(cur_sim_task, SIM_POS_UNBLOCK_CHV2, &(chvs->unblock_chv2));

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}


/**********************************************************************
//    Description:
//      get all the chvs that save in the chv file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_UpdateAllCHV(   //return vlaue:the result of the operation
    uint32 cur_sim_task,
    SIM_ALL_CHV_T *chvs //in:all of the sim chvs
)
{
    SIM_SERVICE_RESULT_E ser_result;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != chvs);

    ser_result = UpdateCHV(cur_sim_task, SIM_POS_CHV1, chvs->chv1);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = UpdateCHV(cur_sim_task, SIM_POS_UNBLOCK_CHV1, chvs->unblock_chv1);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = UpdateCHV(cur_sim_task, SIM_POS_CHV2, chvs->chv2);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = UpdateCHV(cur_sim_task, SIM_POS_UNBLOCK_CHV2, chvs->unblock_chv2);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}



/**********************************************************************
//    Description:
//      create the ef simulator file according the select data that from
//    the corresponding ef file
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMTIW_CreatEFFile(    //return vlaue:the result of the operation
    uint32 cur_sim_task,
    SIM_FILE_NAME_E file_name   //the sim file name
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_EF_FILE_INFO_T ef_info;
    SIM_DF_MF_INFO_T df_info;
    char *file_path;
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* pfile;
    uint8 record_num, i;
    DWORD file_attribute;


    SIM_DISABLE_IRQ


    ser_result = SIMTIW_SelectFile(cur_sim_task, file_name, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    if(!SIMINSTR_DecodeSelectData(&select_data, &ef_info, &df_info))
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    GetFilePath(cur_sim_task, file_name, &file_path);
    //if the file is reade only
    file_attribute = GetFileAttributes(file_path);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(file_path, file_attribute);
    }

    //pfile = fopen(file_path,"wb");
    //if (pfile)
    hFile = CreateFile(file_path,
                       GENERIC_WRITE ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //the file is a Trandparent file,so just set the file content to 0xff
        if(SIM_EF_S_TRANSPARENT == ef_info.file_structure)
        {
            file_path = (char *)malloc(ef_info.file_size);
            SCI_MEMSET(file_path, 0xff, ef_info.file_size);
            //fwrite(file_path,ef_info.file_size,1,pfile);
            WriteFile(hFile, file_path, ef_info.file_size, &ccbOpr, NULL);
            free(file_path);
            CloseHandle(hFile);
            //fclose(pfile);

        }
        //the file a record file,so must enrasure all the record to empty record.
        else
        {
            CloseHandle(hFile);
            //fclose(pfile);
            //get the record num from the select data
            record_num = (uint8)(ef_info.file_size / ef_info.record_len);

            //enrasure each record.
            for(i = 1; i <= record_num; i++)
            {
                SIMSER_UpdateEFRecord(cur_sim_task, file_name, i,
                                      SIM_RECORD_ABSOLUTE, NULL, TRUE); //wuding temply
            }
        }

    }
    else
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

/**********the following is for static button sim card setting---start**********/
SIM_SERVICE_RESULT_E SIMTIW_GetSelectFileName_static(  //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path,    //out:the pointer of the file name
    int card_num
)
{

    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != file_path);

    //the file must be a valid sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    sprintf(full_path[card_num], "%s", select_file[file_name]);

    *file_path = full_path[card_num];

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

STATIC SIM_SERVICE_RESULT_E UpdateCHV_static(  //return value:the result of the operation
    SIM_CHV_POS chv_pos,    //the chv position in the CHV file
    SIM_CHV_T chv,   //the chv used to update
    int card_num
)
{
    DWORD file_attribute;
    char str[32];
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* chv_file;
    int offset;


    SIM_DISABLE_IRQ

    if(!bBaseDirDecided[card_num])
    {
        SIMTIW_GetSettingFileDir(base_directory[card_num], card_num);

        sprintf(chv_file_path[card_num], "%schv_file.inf", base_directory[card_num]);
    }

    SCI_MEMSET(str, 0xff, 32);
    offset = chv_pos * 8;

    //if the file is reade only
    file_attribute = GetFileAttributes(chv_file_path[card_num]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(chv_file_path[card_num], file_attribute);
    }

    //chv_file = fopen(chv_file_path[card_num],"rb");
    //if (chv_file)
    hFile = CreateFile(chv_file_path[card_num],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //fread(str,32,1,chv_file);
        ReadFile(hFile,
                 str,
                 32,
                 &ccbOpr,
                 NULL);
        CloseHandle(hFile);
        //fclose(chv_file);
    }

    SCI_MEMCPY(str + offset, chv.secret_code, 8);
    //chv_file = fopen(chv_file_path[card_num],"wb");
    //if (chv_file)
    hFile = CreateFile(chv_file_path[card_num],
                       GENERIC_WRITE ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fwrite(str,32,1,chv_file) != 1)
        if(!WriteFile(hFile, str, 32, &ccbOpr, NULL)
            || ccbOpr != 32)
        {
            CloseHandle(hFile);
            //fclose(chv_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(chv_file);

        SIM_RESTORE_IRQ
        return SIM_SERVICE_OK;
    }
    else
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

SIM_SERVICE_RESULT_E SIMTIW_UpdateAllCHV_static(   //return vlaue:the result of the operation
    SIM_ALL_CHV_T *chvs, //in:all of the sim chvs
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != chvs);

    ser_result = UpdateCHV_static(SIM_POS_CHV1, chvs->chv1, card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = UpdateCHV_static(SIM_POS_UNBLOCK_CHV1, chvs->unblock_chv1, card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = UpdateCHV_static(SIM_POS_CHV2, chvs->chv2, card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = UpdateCHV_static(SIM_POS_UNBLOCK_CHV2, chvs->unblock_chv2, card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

STATIC SIM_SERVICE_RESULT_E GetCHV_static( //return value:the result of the operation
    SIM_CHV_POS chv_pos,    //the chv position in the CHV file
    SIM_CHV_T *chv,      //out:the chv been read out
    int card_num
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* chv_file;
    int offset;


    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != chv);

    if(!bBaseDirDecided[card_num])
    {
        SIMTIW_GetSettingFileDir(base_directory[card_num], card_num);

        sprintf(chv_file_path[card_num], "%schv_file.inf", base_directory[card_num]);
    }

    offset = chv_pos * 8;
    //chv_file = fopen(chv_file_path[card_num],"rb");
    //if (chv_file)
    hFile = CreateFile(chv_file_path[card_num],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fseek(chv_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(chv_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fread(chv->secret_code,8,1,chv_file) != 1)
        if(!ReadFile(hFile,
                     chv->secret_code,
                     8,
                     &ccbOpr,
                     NULL) || ccbOpr != 8)
        {
            CloseHandle(hFile);
            //fclose(chv_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(chv_file);

        SIM_RESTORE_IRQ
        return SIM_SERVICE_OK;
    }
    else
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return  SIM_SERVICE_OK;
}


SIM_SERVICE_RESULT_E SIMTIW_GetAllCHV_static(  //return value:the result of the operation
    SIM_ALL_CHV_T *chvs, //out:all of the sim chv
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != chvs);

    ser_result = GetCHV_static(SIM_POS_CHV1, &(chvs->chv1), card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = GetCHV_static(SIM_POS_UNBLOCK_CHV1, &(chvs->unblock_chv1), card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = GetCHV_static(SIM_POS_CHV2, &(chvs->chv2), card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    ser_result = GetCHV_static(SIM_POS_UNBLOCK_CHV2, &(chvs->unblock_chv2), card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

STATIC SIM_SERVICE_RESULT_E GetSelectFilePath_static(  //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path,   //out:the pointer of the file path
    int card_num
)
{

    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != file_path);

    if(!bBaseDirDecided[card_num])
    {
        SIMTIW_GetSettingFileDir(base_directory[card_num], card_num);

        sprintf(chv_file_path[card_num], "%schv_file.inf", base_directory[card_num]);
    }

    //the file must be a valide sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    sprintf(full_path[card_num], "%s%s", base_directory[card_num], select_file[file_name]);

    *file_path = full_path[card_num];

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

SIM_SERVICE_RESULT_E SIMTIW_SelectFile_static( //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,  //out:the select data of the sim file
    int card_num
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* select_file;
    char *file_path = NULL;

    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != select_data);

    //add the dcs 1800 function 2002-11-20
    if((SIMFS_IsDCS1800SIM_static(card_num)) && (SIM_FILE_DF_GSM == file_name))
    {
        file_name = SIM_FILE_DF_DCS1800;
    }

    GetSelectFilePath_static(file_name, &file_path, card_num);
    //select_file = fopen(full_path[card_num],"rb");
    //if (select_file)
    hFile = CreateFile(full_path[card_num],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fread(select_data,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T),1,select_file) != 1)
        if(!ReadFile(hFile,
                     select_data,
                     sizeof(SIM_RESPONSE_DATA_OF_SELECT_T),
                     &ccbOpr,
                     NULL) || ccbOpr != sizeof(SIM_RESPONSE_DATA_OF_SELECT_T))
        {
            CloseHandle(hFile);
            //fclose(select_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }
        else
        {
            CloseHandle(hFile);
            //fclose(select_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_OK;
        }
    }
    else
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

static void UpdateFileCharacter_static(
    SIM_FILE_NAME_E file_name,  //the file name of the file
    SIM_RESPONSE_DATA_OF_SELECT_T *select_data,    //the select data of the sim file
    int card_num
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* select_file;
    char *file_path = NULL;
    DWORD file_attribute;
    SIM_RESPONSE_DATA_OF_SELECT_T new_select;

    SIM_DISABLE_IRQ

    if(SIM_FILE_DF_GRAPHICS < file_name)
    {
        SIM_RESTORE_IRQ
        return;
    }

    if(SIM_SERVICE_OK != SIMTIW_SelectFile_static(file_name, &new_select, card_num), card_num)
    {
        SIM_RESTORE_IRQ
        return;
    }

    SCI_MEMCPY(&(new_select.filecharacter_efstructure), &(select_data->filecharacter_efstructure), (sizeof(SIM_RESPONSE_DATA_OF_SELECT_T) - 13));

    GetSelectFilePath_static(file_name, &file_path, card_num);

    //if the file is reade only
    file_attribute = GetFileAttributes(full_path[card_num]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(full_path[card_num], file_attribute);
    }

    //select_file = fopen(full_path[card_num],"wb");
    //if (select_file)
    hFile = CreateFile(full_path[card_num],
                       GENERIC_WRITE,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fwrite(&new_select,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T),1,select_file) != 1)
        if(!WriteFile(hFile, &new_select, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T), &ccbOpr, NULL)
            || ccbOpr != sizeof(SIM_RESPONSE_DATA_OF_SELECT_T))
        {
            CloseHandle(hFile);
            //fclose(select_file);

            SIM_RESTORE_IRQ
            return;
        }

        CloseHandle(hFile);
        //fclose(select_file);

        SIM_RESTORE_IRQ
        return;
    }
    else
    {

        SIM_RESTORE_IRQ
        return;
    }

    SIM_RESTORE_IRQ
    return;
}


SIM_SERVICE_RESULT_E SIMTIW_UpdateSelectData_static(   //the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RESPONSE_DATA_OF_SELECT_T select_data,   //the select data of the sim file
    int card_num
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* select_file;
    char *file_path = NULL;
    DWORD file_attribute;
    SIM_FILE_NAME_E temp_file;

    SIM_DISABLE_IRQ


    GetSelectFilePath_static(file_name, &file_path, card_num);

    //if the file is reade only
    file_attribute = GetFileAttributes(full_path[card_num]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(full_path[card_num], file_attribute);
    }

    //select_file = fopen(full_path[card_num],"wb");
    //if (select_file)
    hFile = CreateFile(full_path[card_num],
                       GENERIC_WRITE ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fwrite(&select_data,sizeof(SIM_RESPONSE_DATA_OF_SELECT_T),1,select_file) != 1)
        if(!WriteFile(hFile, &select_data, sizeof(SIM_RESPONSE_DATA_OF_SELECT_T), &ccbOpr, NULL)
            || ccbOpr != sizeof(SIM_RESPONSE_DATA_OF_SELECT_T))
        {
            CloseHandle(hFile);
            //fclose(select_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(select_file);

        //if it's updating the Directory file information,then updating the file
        if(SIM_FILE_DF_GRAPHICS >= file_name)
        {
            for(temp_file = SIM_FILE_MF; temp_file <= SIM_FILE_DF_GRAPHICS; temp_file += 1)
            {
                UpdateFileCharacter_static(temp_file, &select_data, card_num);
            }
        }

        SIM_RESTORE_IRQ
        return SIM_SERVICE_OK;
    }
    else
    {

        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

void SIMTIW_GetCHVVerificated_static(
    BOOLEAN *chv1_v,    //out:the chv1 verificated state
    BOOLEAN *chv2_v,    //out:the chv1 verificated state
    int card_num
)
{
    *chv1_v = CHV1_Verified[card_num];
    *chv2_v = CHV2_Verified[card_num];
}

void SIMTIW_SetCHVVerificated_static(
    BOOLEAN chv1_v, //the chv1 verificated state
    BOOLEAN chv2_v,     //the chv1 verificated state
    int card_num
)
{
    CHV1_Verified[card_num] = chv1_v;
    CHV2_Verified[card_num] = chv2_v;
}

STATIC SIM_SERVICE_RESULT_E GetFilePath_static(    //return value:the result of operation
    SIM_FILE_NAME_E file_name,  //the file name
    char **file_path,   //out:the pointer of the path
    int card_num
)
{

    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != file_path);

    if(!bBaseDirDecided[card_num])
    {
        SIMTIW_GetSettingFileDir(base_directory[card_num], card_num);

        sprintf(chv_file_path[card_num], "%schv_file.inf", base_directory[card_num]);
    }

    //the file must be a valid sim file
    if(SIM_IS_INVALIDE_FILE(file_name))
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_INVALIDATE_FILE_NAME;
    }

    sprintf(full_path[card_num], "%s%s", base_directory[card_num], ef_file_name[file_name]);

    *file_path = full_path[card_num];

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

SIM_SERVICE_RESULT_E SIMTIW_CreatEFFile_static(    //return vlaue:the result of the operation
    SIM_FILE_NAME_E file_name,   //the sim file name
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_EF_FILE_INFO_T ef_info;
    SIM_DF_MF_INFO_T df_info;
    char *file_path;
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* pfile;
    uint8 record_num, i;
    DWORD file_attribute;


    SIM_DISABLE_IRQ


    ser_result = SIMTIW_SelectFile_static(file_name, &select_data, card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    if(!SIMINSTR_DecodeSelectData(&select_data, &ef_info, &df_info))
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    GetFilePath_static(file_name, &file_path, card_num);
    //if the file is reade only
    file_attribute = GetFileAttributes(file_path);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(file_path, file_attribute);
    }

    //pfile = fopen(file_path,"wb");
    //if (pfile)
    hFile = CreateFile(file_path,
                       GENERIC_WRITE ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //the file is a Trandparent file,so just set the file content to 0xff
        if(SIM_EF_S_TRANSPARENT == ef_info.file_structure)
        {
            file_path = (char *)malloc(ef_info.file_size);
            SCI_MEMSET(file_path, 0xff, ef_info.file_size);
            //fwrite(file_path,ef_info.file_size,1,pfile);
            WriteFile(hFile, file_path, ef_info.file_size, &ccbOpr, NULL);
            free(file_path);
            CloseHandle(hFile);
            //fclose(pfile);

        }
        //the file a record file,so must enrasure all the record to empty record.
        else
        {
            CloseHandle(hFile);
            //fclose(pfile);
            //get the record num from the select data
            record_num = (uint8)(ef_info.file_size / ef_info.record_len);

            //enrasure each record.
            for(i = 1; i <= record_num; i++)
            {
                SIMSER_UpdateEFRecord_static(0, file_name, i,
                                             SIM_RECORD_ABSOLUTE, NULL, TRUE, card_num); //wuding temply
            }
        }

    }
    else
    {
        SIM_RESTORE_IRQ
        return SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

STATIC SIM_SERVICE_RESULT_E UpdateRecordFile_static(   //retrun value:the result of the oepration
    SIM_FILE_NAME_E file_name,  //the sim file name
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    uint8 record_num,   //the reocrd num to be updated
    uint8 record_len,   //the update data len
    uint8 *data, //in:the udpate data
    int card_num
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    char *file_path = NULL;
    int offset, true_record_len;
    DWORD file_attribute;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != data);
    SCI_ASSERT(record_num >= 1);

    true_record_len = record_len;
    offset = GetFileOffset(card_num, file_name, record_mode, record_num, true_record_len);

    if(0 > offset)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    GetFilePath_static(file_name, &file_path, card_num);

    //if the file is reade only
    file_attribute = GetFileAttributes(full_path[card_num]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(full_path[card_num], file_attribute);
    }

    //sim_file = fopen(full_path[card_num],"r+b");
    //if (sim_file)
    hFile = CreateFile(full_path[card_num],
                       GENERIC_READ | GENERIC_WRITE ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fseek(sim_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fwrite(data,true_record_len,1,sim_file) != 1)
        if(!WriteFile(hFile, data, true_record_len, &ccbOpr, NULL)
            || ccbOpr != true_record_len)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(sim_file);

        SIM_RESTORE_IRQ
        return  SIM_SERVICE_OK;
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

STATIC SIM_SERVICE_RESULT_E UpdateTransparentFile_static(  //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16 offset,  //the update offset
    uint8 data_len, //the update data len
    uint8 *data, //in:the update data
    int card_num
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    int true_len;
    char *file_path = NULL;
    DWORD file_attribute;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != data);

    true_len = data_len;

    GetFilePath_static(file_name, &file_path, card_num);

    //if the file is reade only
    file_attribute = GetFileAttributes(full_path[card_num]);

    if((-1 != file_attribute) && (file_attribute & FILE_ATTRIBUTE_READONLY))
    {
        //remove the file read only attribute
        file_attribute &= ~FILE_ATTRIBUTE_READONLY;
        SetFileAttributes(full_path[card_num], file_attribute);
    }

    //sim_file = fopen(full_path[card_num],"r+b");
    //if (sim_file)
    hFile = CreateFile(full_path[card_num],
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        //if (fseek(sim_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fwrite(data,true_len,1,sim_file) != 1)
        if(!WriteFile(hFile, data, true_len, &ccbOpr, NULL)
            || ccbOpr != true_len)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(sim_file);

        SIM_RESTORE_IRQ
        return SIM_SERVICE_OK;
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

SIM_SERVICE_RESULT_E SIMTIW_UpdateFile_static( //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16  offset, //the update offset of the transparent file
    uint8       record_num, //the record num of the record file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode
    uint8       data_len,   //the update data len
    uint8  *data,    //in:the update data
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_FILE_ACCESS_CONDITION_T file_access;

    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != data);

    //get the select data of the file
    ser_result = SIMTIW_SelectFile_static(file_name, &select_data, card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //if the file is not a EF file
    if(SIM_FILE_T_EF != select_data.file_type)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    if(SIM_FILE_EF_ACMmax == file_name)
    {

        //check the access condition
        GET_FILE_ACCESS_CONDITION(file_access, (&select_data));

        if((SIM_ACCESS_CHV1 == file_access.update_access) &&
            ((!CHV1_Verified[card_num]) && (g_sim_card_setting[card_num].gsm_data.chv1_enabled)))
        {
            if(g_sim_card_setting[card_num].gsm_data.chv1_status.chv_blocked)
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV1_NEEDED;
            }
            else if(g_sim_card_setting[card_num].gsm_data.unblock_chv1_status.chv_blocked)
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_U_CHV1_NEEDED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV_BLOCKED;
            }
        }
        else if((SIM_ACCESS_CHV2 == file_access.update_access) && (!CHV2_Verified[card_num]))
        {
            if(g_sim_card_setting[card_num].gsm_data.chv2_status.chv_blocked)
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV2_NEEDED;
            }
            else if(g_sim_card_setting[card_num].gsm_data.unblock_chv2_status.chv_blocked)
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_U_CHV2_NEEDED;
            }
            else
            {
                SIM_RESTORE_IRQ
                return SIM_SERVICE_CHV_BLOCKED;
            }
        }
        else if(SIM_ACCESS_CHV2 < file_access.update_access)
        {
            SIM_RESTORE_IRQ
            return SIM_UPDATE_ACCESS_NEVER;
        }
    }

    //do different update for the special file structure
    //update transparent file,corresponding to Update Binary
    if(SIM_EF_S_TRANSPARENT == select_data.filecharacter_efstructure)
    {
        SIM_RESTORE_IRQ
        return  UpdateTransparentFile_static(file_name, offset, data_len, data, card_num);
    }
    //update record file corespoding to Update Record
    else if((SIM_EF_S_LINEAR_FIXED == select_data.filecharacter_efstructure) ||
            (SIM_EF_S_CYCLIC == select_data.filecharacter_efstructure))
    {
        SIM_RESTORE_IRQ
        return  UpdateRecordFile_static(file_name, mode, record_num, select_data.dfnumber_recordlen, data, card_num);
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

STATIC SIM_SERVICE_RESULT_E ReadTrasparentFile_static( //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file
    uint16 offset,  //the reading offset
    uint8 data_len, //the reading data len
    uint16 *read_data_len,  //out:the data length that have readed
    uint8 *data, //out:the data readed out
    int card_num
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    int true_len;
    char *file_path = NULL;


    SIM_DISABLE_IRQ

    SCI_ASSERT(NULL != read_data_len);
    SCI_ASSERT(NULL != data);

    GetFilePath_static(file_name, &file_path, card_num);
    //sim_file = fopen(full_path[card_num],"rb");
    //if (sim_file)
    hFile = CreateFile(full_path[card_num],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        true_len = 0 == data_len ? SIM_MAX_R_APDU_DATA_LEN : data_len;
        //if (fseek(sim_file,0,SEEK_END) != 0)
        ccbOpr = SetFilePointer(hFile, 0, NULL, FILE_END);

        if(ccbOpr == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        //*read_data_len = (uint16)(ftell(sim_file) - offset);
        *read_data_len = ccbOpr - offset;

        if(true_len > *read_data_len)
        {
            true_len =  *read_data_len;
        }

        //if (fseek(sim_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }

        //get the really data length
        *read_data_len = true_len;

        //*read_data_len  = (uint8)fread(data,1,true_len,sim_file);
        ReadFile(hFile,
                 data,
                 true_len,
                 &ccbOpr,
                 NULL);
        *read_data_len = ccbOpr;

        if(*read_data_len <= 0)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return SIM_SERVICE_ERROR_UNKNOW;
        }
        else
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_OK;
        }
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

STATIC SIM_SERVICE_RESULT_E ReadRecordFile_static( //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file
    SIM_RECORD_ACCESS_MODE_E record_mode,   //the record access mode
    uint8 record_num,   //the record num
    uint8 record_len,   //the record len
    uint16 *data_len,   //out:the data len that readed out
    uint8 *data,     //out:the data that readed out
    int card_num
)
{
    //apple change for start problem
    HANDLE hFile;
    int ccbOpr = 0;
    //FILE* sim_file;
    int offset, true_record_len;
    char *file_path = NULL;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != data_len);
    SCI_ASSERT(NULL != data);

    true_record_len = 0 == record_len ? SIM_MAX_R_APDU_DATA_LEN : record_len;
    *data_len = true_record_len;
    GetFilePath_static(file_name, &file_path, card_num);
    //sim_file = fopen(full_path[card_num],"rb");
    //if (sim_file)
    hFile = CreateFile(full_path[card_num],
                       GENERIC_READ ,
                       FILE_SHARE_DELETE | FILE_SHARE_READ
                       | FILE_SHARE_WRITE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_SEQUENTIAL_SCAN,
                       NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        offset = GetFileOffset(card_num, file_name, record_mode, record_num, true_record_len);

        if(0 > offset)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fseek(sim_file,offset,SEEK_SET) != 0)
        if(SetFilePointer(hFile, offset, NULL, FILE_BEGIN) == -1)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        //if (fread(data,true_record_len,1,sim_file) != 1)
        if(!ReadFile(hFile,
                     data,
                     true_record_len,
                     &ccbOpr,
                     NULL) || ccbOpr != true_record_len)
        {
            CloseHandle(hFile);
            //fclose(sim_file);

            SIM_RESTORE_IRQ
            return  SIM_SERVICE_ERROR_UNKNOW;
        }

        CloseHandle(hFile);
        //fclose(sim_file);

        SIM_RESTORE_IRQ
        return  SIM_SERVICE_OK;
    }
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }

    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

SIM_SERVICE_RESULT_E SIMTIW_ReadFile_static(   //return value:the result of the operation
    SIM_FILE_NAME_E file_name,  //the sim file name
    uint16  offset, //the reading offset of the transparent file
    uint8   record_num, //the record num of the record file
    SIM_RECORD_ACCESS_MODE_E mode,  //the record access mode of the record file
    uint16 *filesize_recordlen, //the file sizeof transparent file or the record len of record file
    uint16 *data_len,   //the data len that readed out
    uint8  *data,    //the data that readed out
    int card_num
)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;


    SIM_DISABLE_IRQ


    SCI_ASSERT(NULL != filesize_recordlen);
    SCI_ASSERT(NULL != data_len);
    SCI_ASSERT(NULL != data);

    //get the select data of the file
    ser_result = SIMTIW_SelectFile_static(file_name, &select_data, card_num);

    if(SIM_SERVICE_OK != ser_result)
    {
        SIM_RESTORE_IRQ
        return  ser_result;
    }

    //if the file is not a EF file
    if(SIM_FILE_T_EF != select_data.file_type)
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_INCONSISTENT_FILE;
    }

    /*  //check the access condition
        GET_FILE_ACCESS_CONDITION(file_access,select_data);
        if (SIM_ACCESS_CHV1 == file_access.seek_read_access)
        {
            if (!CHV1_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 == file_access.seek_read_access)
        {
            if (!CHV2_Verified)
            {
                return SIM_SERVICE_ACCESS_FAIL;
            }
        }
        else if (SIM_ACCESS_CHV2 < file_access.seek_read_access)
        {
            return SIM_SERVICE_ACCESS_FAIL;
        }*/

    //do different read for the special file structure
    //read transparent file,corresponding to Read Binary
    if(SIM_EF_S_TRANSPARENT == select_data.filecharacter_efstructure)
    {
        *filesize_recordlen = select_data.freespace_filesize;

        SIM_RESTORE_IRQ
        return  ReadTrasparentFile_static(file_name, offset, record_num, data_len, data, card_num);
    }
    //read record file corespoding to Read Record
    else if((SIM_EF_S_LINEAR_FIXED == select_data.filecharacter_efstructure) ||
            (SIM_EF_S_CYCLIC == select_data.filecharacter_efstructure))
    {
        *filesize_recordlen = select_data.dfnumber_recordlen == 0 ? SIM_MAX_R_APDU_DATA_LEN : select_data.dfnumber_recordlen;

        SIM_RESTORE_IRQ
        return  ReadRecordFile_static(file_name, mode, record_num, select_data.dfnumber_recordlen, data_len, data, card_num);
    }
    //the unknow file sturcture
    else
    {
        SIM_RESTORE_IRQ
        return  SIM_SERVICE_ERROR_UNKNOW;
    }


    SIM_RESTORE_IRQ
    return SIM_SERVICE_OK;
}

/**********the above is for static button sim card setting---end***********/
#endif  //SIM_TEST_IN_WIN32

#ifdef   __cplusplus
}
#endif  //__cplusplus

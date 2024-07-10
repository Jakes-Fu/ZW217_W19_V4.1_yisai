/******************************************************************************
 ** File Name:      ms_ref_mux_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:60
#ifndef _MS_REF_MUX_TRC_H_
#define _MS_REF_MUX_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MUX_API_67_112_2_18_1_49_16_0 "MUX_Set_Atc_CallbackFunc fail due to link id(%d) execed"
#define MUX_API_73_112_2_18_1_49_16_1 "MUX_Set_Atc_CallbackFunc fail due to status error in link %d"
#define MUX_API_93_112_2_18_1_49_16_2 "MUX_DTI_Destroy fail due to link id(%d) execed"
#define MUX_API_132_112_2_18_1_49_17_3 "MUX_DTI_Query_Entity fail due to link id(%d) execed"
#define MUX_API_161_112_2_18_1_49_17_4 "MUX_DTI_Create fail due to link id(%d) execed"
#define MUX_API_179_112_2_18_1_49_17_5 "MUX_DTI_Create fail due to status error in link %d"
#define MUX_API_188_112_2_18_1_49_17_6 "MUX_DTI_Create fail due to status error in link %d"
#define MUX_API_233_112_2_18_1_49_17_7 "MUX set ON after send OK"
#define MUX_API_243_112_2_18_1_49_17_8 "MUX_Write fail due to link id(%d) execed, data_len = %d"
#define MUX_API_250_112_2_18_1_49_17_9 "MUX_Write: Flow stopped on all channels, return zero link id %d, data_len = %d\n"
#define MUX_API_255_112_2_18_1_49_17_10 "MUX_Write: Flow stopped, return zero link id %d, data_len = %d\n"
#define MUX_API_261_112_2_18_1_49_17_11 "MUX_Write fail due to status error in link id %d,data_len = %d\n"
#define MUX_API_268_112_2_18_1_49_17_12 "MUX_Write fail due to forbid user data in DLCI 0, data_len = %d\n"
#define MUX_API_322_112_2_18_1_49_17_13 "MUX_Linkid_Couple type:%d, link id %d"
#define MUX_API_341_112_2_18_1_49_17_14 "MUX_Linkid_Couple get Linkid fail!"
#define MUX_API_344_112_2_18_1_49_17_15 "MUX_Linkid_Couple return link id %d"
#define MUX_COMMON_85_112_2_18_1_49_17_16 "MUX MUX_Crc_Check: CRC check failed\n"
#define MUX_COMMON_188_112_2_18_1_49_18_17 "MUX: MUX_Default_ATC_Callback in link_id = %d, len = %d"
#define MUX_COMMON_210_112_2_18_1_49_18_18 "MUX_Get_Linkid, 0XFF"
#define MUX_COMMON_263_112_2_18_1_49_18_19 "MUX_Get_NewLinkid  fails"
#define MUX_COMMON_278_112_2_18_1_49_18_20 "MUX_TraceDatat datalen = %d \n"
#define MUX_COMMON_285_112_2_18_1_49_18_21 "0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, \r\n"
#define MUX_COMMON_292_112_2_18_1_49_18_22 "0x%02x, \r\n"
#define MUX_COMMON_333_112_2_18_1_49_18_23 "MUX_Assemble_CMUX_CMD: outptr is null pointer\r"
#define MUX_CTL_94_112_2_18_1_49_18_24 "MUX_Startup fail due to MUX is in ON"
#define MUX_CTL_102_112_2_18_1_49_18_25 "MUX_Startup fail due to parameter error"
#define MUX_CTL_106_112_2_18_1_49_18_26 "MUX_Startup: From %d\r"
#define MUX_CTL_170_112_2_18_1_49_18_27 "MUX_Startup: AT+CMUX command Failure!\r"
#define MUX_CTL_181_112_2_18_1_49_18_28 "MUX_Startup: fail due to OPEN Control DLCI failure!\r"
#define MUX_CTL_215_112_2_18_1_49_19_29 "MUX_ReadSysPara fail due to NOT Startup"
#define MUX_CTL_238_112_2_18_1_49_19_30 "MUX_Res_Wait: g_MuxEvent_ptr is null ptr!"
#define MUX_CTL_277_112_2_18_1_49_19_31 "MUX_Wait_WakeUp: g_MuxEvent_ptr is null ptr!"
#define MUX_CTL_310_112_2_18_1_49_19_32 "MUX_SABM_Handler:Incomming connect on DLC %d; link_id %d\n"
#define MUX_CTL_351_112_2_18_1_49_19_33 "MUX_SABM_Handler, DLC%d is exist, Sending MUX_UA \n"
#define MUX_CTL_356_112_2_18_1_49_19_34 "MUX_SABM_Handler: invalid dlci %d, sending MUX_DM\n"
#define MUX_CTL_381_112_2_18_1_49_19_35 "MUX: MUX_UA_Handler: Incomming CONNECTED  MUX_UA on dlci %d\n"
#define MUX_CTL_388_112_2_18_1_49_19_36 "MUX:MUX_UA_Handler: Incomming DISCONNECTED  MUX_UA on dlci %d\n"
#define MUX_CTL_406_112_2_18_1_49_19_37 "MUX_UA_Handler, unexpected response, discard,in link id %d,dlci %d\n"
#define MUX_CTL_411_112_2_18_1_49_19_38 "MUX: MUX_UA_Handler: invalid dlci %d\n"
#define MUX_CTL_432_112_2_18_1_49_19_39 "MUX: MUX_DM_Handler: Incomming MUX_DM on dlci %d\n"
#define MUX_CTL_436_112_2_18_1_49_19_40 "MUX: MUX_DM_Handler: Incomming CONNECTED  MUX_DM on dlci %d\n"
#define MUX_CTL_445_112_2_18_1_49_19_41 "MUX:MUX_DM_Handler: Incomming DISCONNECTED  MUX_DM on dlci %d\n"
#define MUX_CTL_453_112_2_18_1_49_19_42 "MUX_DM_Handler, unexpected response, discard in link id %d dlci %d\n"
#define MUX_CTL_458_112_2_18_1_49_19_43 "MUX_DM_Handler, invalid dlci %d\n"
#define MUX_CTL_477_112_2_18_1_49_19_44 "MUX_DISC_Handler: Incomming MUX_DISC on channel %d\n"
#define MUX_CTL_513_112_2_18_1_49_19_45 "MUX_DISC_Handler: invalid dlci %d\n"
#define MUX_CTL_536_112_2_18_1_49_19_46 "MUX_UIH_Handler: invalid dlci %d\n"
#define MUX_CTL_547_112_2_18_1_49_19_47 "MUX_UIH_Handler Error : DLCI %d (link %d)not connected, discard it!\n"
#define MUX_CTL_576_112_2_18_1_49_19_48 "MUX_UIH_Handler P/F set error, discard it in link %d,dlci %d!\n"
#define MUX_CTL_599_112_2_18_1_49_19_49 "Mux_Send_UA: Creating UA packet to DLCI %d\n"
#define MUX_CTL_646_112_2_18_1_49_20_50 "Mux_Send_UA: Fail in Creating UA packet to DLCI %d\n"
#define MUX_CTL_666_112_2_18_1_49_20_51 "MUX_Send_DM: Creating DM packet to DLCI %d\n"
#define MUX_CTL_713_112_2_18_1_49_20_52 "MUX_Send_DM: Fail in Creating DM packet to DLCI %d\n"
#define MUX_CTL_744_112_2_18_1_49_20_53 "Mux_Send_UIH: Fail due to length is too long in %d DLCI of %d bytes data\n"
#define MUX_CTL_861_112_2_18_1_49_20_54 "Mux_Send_UIH: Fail in Creating UIH packet with %d bytes data to DLCI %d due to length is too long\n"
#define MUX_CTL_874_112_2_18_1_49_20_55 "MUX_Send_UIH: Creating DM packet to DLCI %d\n"
#define MUX_CTL_905_112_2_18_1_49_20_56 "Mux_Send_SABM: Creating SABM packet to DLCI %d\n"
#define MUX_CTL_948_112_2_18_1_49_20_57 "Mux_Send_SABM: Fail in Creating SABM packet to DLCI %d\n"
#define MUX_CTL_981_112_2_18_1_49_20_58 "Mux_MCCFrame_Parse error DLCI %d in PN\n"
#define MUX_CTL_984_112_2_18_1_49_20_59 "MUX: Received DLC %d (link %d)parameter negotiation, PN\n"
#define MUX_CTL_1005_112_2_18_1_49_20_60 "MUX: Received DLC power saving msg\n"
#define MUX_CTL_1030_112_2_18_1_49_20_61 "MUX: Received test  msg\n"
#define MUX_CTL_1053_112_2_18_1_49_20_62 "MUX: Received FCON  msg\n"
#define MUX_CTL_1076_112_2_18_1_49_20_63 "MUX: Received FCOFF msg\n"
#define MUX_CTL_1109_112_2_18_1_49_20_64 "Mux_MCCFrame_Parse error DLCI %d in MSC\n"
#define MUX_CTL_1116_112_2_18_1_49_20_65 "MUX reject received MSC message in dlci %d (link %d)\n"
#define MUX_CTL_1127_112_2_18_1_49_20_66 "MUX Received Flow off on dlci %d (link %d)\n"
#define MUX_CTL_1135_112_2_18_1_49_21_67 "MUX Received Flow on on dlci %d (link %d)\n"
#define MUX_CTL_1147_112_2_18_1_49_21_68 "MUX Received Non supported command response\n"
#define MUX_CTL_1152_112_2_18_1_49_21_69 "MUX Received a non supported command\n"
#define MUX_CTL_1202_112_2_18_1_49_21_70 "Mux_Send_MCC_PN start in dlci %d"
#define MUX_CTL_1236_112_2_18_1_49_21_71 "Mux_Send_MCC_PSC start cr = %d"
#define MUX_CTL_1264_112_2_18_1_49_21_72 "Mux_Send_MCC_CLD start cr= %d"
#define MUX_CTL_1291_112_2_18_1_49_21_73 "Mux_Send_MCC_TEST start"
#define MUX_CTL_1310_112_2_18_1_49_21_74 "Mux_Send_MCC_TEST: TEST Message excede max length\r\n"
#define MUX_CTL_1344_112_2_18_1_49_21_75 "Mux_Send_MCC_FCon start cr = %d"
#define MUX_CTL_1372_112_2_18_1_49_21_76 "Mux_Send_MCC_FCoff start cr = %d"
#define MUX_CTL_1404_112_2_18_1_49_21_77 "Mux_Send_MCC_MSC start in dlci %d"
#define MUX_CTL_1438_112_2_18_1_49_21_78 "Mux_Send_MCC_NSC START"
#define MUX_CTL_1467_112_2_18_1_49_21_79 "Mux_Send_Disc: Creating DISC packet to DLCI %d\n"
#define MUX_CTL_1507_112_2_18_1_49_21_80 "MUX_Send_Disc: Fail in Creating DISC packet to DLCI %d\n"
#define MUX_CTL_1533_112_2_18_1_49_21_81 "Mux_Close_Channel: DLC %d is NOT exist \n"
#define MUX_CTL_1539_112_2_18_1_49_21_82 "Mux_Close_Channel: DLC %d (link %d) is already in DISC state\n"
#define MUX_CTL_1546_112_2_18_1_49_21_83 "Mux_Close_Channel: DLC %d (link %d) is already in DISC state\n"
#define MUX_CTL_1551_112_2_18_1_49_21_84 "Mux_Close_Channel on channel %d (link %d)\n"
#define MUX_CTL_1571_112_2_18_1_49_21_85 "MUX DLCI %d (link %d) Send DISC timeout!\n"
#define MUX_CTL_1611_112_2_18_1_49_21_86 "MUX_Close_Link fail due to NOT Startup"
#define MUX_CTL_1619_112_2_18_1_49_21_87 "MUX_Close_Link, NOT allow release DLC0\n"
#define MUX_CTL_1642_112_2_18_1_49_22_88 "MUX_Exit fail due to NOT Startup"
#define MUX_CTL_1684_112_2_18_1_49_22_89 "MUX_Open_Dlci  DLCI:%d  NOT Exist!\n"
#define MUX_CTL_1693_112_2_18_1_49_22_90 "MUX_Open_Dlci(dlci %d, link %d)  DLCI0  NOT Exist!\n"
#define MUX_CTL_1698_112_2_18_1_49_22_91 "MUX_Open_Dlci(dlci %d, link %d) due to link_status error of DLCI0!\n"
#define MUX_CTL_1703_112_2_18_1_49_22_92 "MUX_Open_Dlci: DLCI:%d(link %d) Start!\n"
#define MUX_CTL_1794_112_2_18_1_49_22_93 "MUX_Open_Dlci: MUX DLCI:%d Send MUX_PN_MSG timeout!\n"
#define MUX_CTL_1818_112_2_18_1_49_22_94 "MUX DLCI:%d Send SABM got rejected!\n"
#define MUX_CTL_1824_112_2_18_1_49_22_95 "MUX DLCI:%d Send SABM timeout!\n"
#define MUX_CTL_1865_112_2_18_1_49_22_96 "MUX: MUX_Open_Link fail due to NOT Startup"
#define MUX_CTL_1875_112_2_18_1_49_22_97 "MUX: MUX_Open_Link fail due to there have a link %d is in establishing process\r\n"
#define MUX_CTL_1886_112_2_18_1_49_22_98 "MUX: MUX_Open_Link fail due to that there can NOT allocate new link id\r\n"
#define MUX_CTL_1906_112_2_18_1_49_22_99 "MUX: MUX_Open_Link Failure, link_id = %d, DLCI= %d\r\n"
#define MUX_CTL_1921_112_2_18_1_49_22_100 "MUX_Upon_Close: MUX_%d not startup, directly return!"
#define MUX_CTL_2092_112_2_18_1_49_22_101 "MUX: NOT receive OK, return failure\r\n"
#define MUX_DRIVER_148_112_2_18_1_49_23_102 "mux has %d data need send"
#define MUX_DRIVER_177_112_2_18_1_49_23_103 "MUX_Create: Fail due to port(%d) parameter error"
#define MUX_DRIVER_191_112_2_18_1_49_23_104 "MUX_Create: Fail due to phy_port parameter error in port %d"
#define MUX_DRIVER_284_112_2_18_1_49_23_105 "MUX_Create: Open UART/USB success"
#define MUX_DRIVER_310_112_2_18_1_49_24_106 "MUX_Reset: Fail due to port(%d) parameter error"
#define MUX_DRIVER_336_112_2_18_1_49_24_107 "MUX_Reset: Open UART/USB success"
#define MUX_DRIVER_364_112_2_18_1_49_24_108 "MUX_Close: Fail due to port(%d) parameter error"
#define MUX_DRIVER_390_112_2_18_1_49_24_109 "MUX_Close: Close UART/USB success"
#define MUX_DRIVER_631_112_2_18_1_49_24_110 "MUX_RxForPort (%d): Disable Out_endp interrupt of usb"
#define MUX_DRIVER_639_112_2_18_1_49_24_111 "MUX_RxForPort (%d): Software flow control:send XOFF"
#define MUX_DRIVER_662_112_2_18_1_49_24_112 "MUX_RxForPort (%d): Rx buffer over writtn!"
#define MUX_DRIVER_793_112_2_18_1_49_25_113 "MUX_Data_Tx fails  len is zero!!"
#define MUX_DRIVER_799_112_2_18_1_49_25_114 "MUX_Data_Tx fails  PORT %d is closed!"
#define MUX_DRIVER_806_112_2_18_1_49_25_115 "MUX_Data_Tx fails in MUX_%d due to get MUTEX fails"
#define MUX_DRIVER_822_112_2_18_1_49_25_116 "  mux free_size low 0x203000d0[0x%x],0x203000cc[0x%x],0x203000c0[0x%x],0x203000d8[0x%x]"
#define MUX_DRIVER_833_112_2_18_1_49_25_117 "MUX_Data_Tx full skip date %d in %d PORT"
#define MUX_DRIVER_838_112_2_18_1_49_25_118 "0x203000d0[0x%x],0x203000cc[0x%x],0x203000c0[0x%x],0x203000d8[0x%x]"
#define MUX_DRIVER_1039_112_2_18_1_49_25_119 "MUX_Fill_Rxbuff: Disable Out_endp interrupt of usb"
#define MUX_DRIVER_1046_112_2_18_1_49_25_120 "MUX_Fill_Rxbuff: Software flow control:send XOFF"
#define MUX_DRIVER_1069_112_2_18_1_49_25_121 "MUX_Fill_Rxbuff: Rx buffer over writtn!"
#define MUX_RECV_70_112_2_18_1_49_25_122 "MUX_Res_Wait: g_MuxRecv_Eptr is null ptr in MUX entry1!"
#define MUX_RECV_171_112_2_18_1_49_26_123 "MUX_Res_Wait: g_MuxRecv_Eptr is null ptr in MUX entry2!"
#define MUX_RECV_348_112_2_18_1_49_26_124 "MUX_Get_Data_Block discard data(len=%d)  in %d:"
#define MUX_RECV_368_112_2_18_1_49_26_125 "MUX_Get_Data_Block: work_mode error"
#define MUX_RECV_433_112_2_18_1_49_26_126 "MUX_Forward_Up: ATC task Not Ready,Discard Recv Data!"
#define MUX_RECV_446_112_2_18_1_49_26_127 "MUX_Forward_Up: REcv task Not Ready,Discard Recv Data!"
#define MUX_RECV_542_112_2_18_1_49_27_128 "MUX Error: frame length:%d is bigger than Max total frame size:%d\n"
#define MUX_RECV_588_112_2_18_1_49_27_129 "MUX: Lost synchronization in MUX %d\n"
#define MUX_RECV_671_112_2_18_1_49_27_130 "MUX MUX_Data_Parse_Advance: !!!!! Exceed buffer boundary in MUX %d!\n"
#define MUX_RECV_714_112_2_18_1_49_27_131 "MUX Error: frame length:%d is bigger than Max total frame size:%d\n"
#define MUX_RECV_750_112_2_18_1_49_27_132 "MUX: CRC check failure!\n"
#define MUX_RECV_812_112_2_18_1_49_27_133 "Mux_Frame_Parse: illegal packet in DLCI %d\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_MUX_TRC)
TRACE_MSG(MUX_API_67_112_2_18_1_49_16_0,"MUX_Set_Atc_CallbackFunc fail due to link id(%d) execed")
TRACE_MSG(MUX_API_73_112_2_18_1_49_16_1,"MUX_Set_Atc_CallbackFunc fail due to status error in link %d")
TRACE_MSG(MUX_API_93_112_2_18_1_49_16_2,"MUX_DTI_Destroy fail due to link id(%d) execed")
TRACE_MSG(MUX_API_132_112_2_18_1_49_17_3,"MUX_DTI_Query_Entity fail due to link id(%d) execed")
TRACE_MSG(MUX_API_161_112_2_18_1_49_17_4,"MUX_DTI_Create fail due to link id(%d) execed")
TRACE_MSG(MUX_API_179_112_2_18_1_49_17_5,"MUX_DTI_Create fail due to status error in link %d")
TRACE_MSG(MUX_API_188_112_2_18_1_49_17_6,"MUX_DTI_Create fail due to status error in link %d")
TRACE_MSG(MUX_API_233_112_2_18_1_49_17_7,"MUX set ON after send OK")
TRACE_MSG(MUX_API_243_112_2_18_1_49_17_8,"MUX_Write fail due to link id(%d) execed, data_len = %d")
TRACE_MSG(MUX_API_250_112_2_18_1_49_17_9,"MUX_Write: Flow stopped on all channels, return zero link id %d, data_len = %d\n")
TRACE_MSG(MUX_API_255_112_2_18_1_49_17_10,"MUX_Write: Flow stopped, return zero link id %d, data_len = %d\n")
TRACE_MSG(MUX_API_261_112_2_18_1_49_17_11,"MUX_Write fail due to status error in link id %d,data_len = %d\n")
TRACE_MSG(MUX_API_268_112_2_18_1_49_17_12,"MUX_Write fail due to forbid user data in DLCI 0, data_len = %d\n")
TRACE_MSG(MUX_API_322_112_2_18_1_49_17_13,"MUX_Linkid_Couple type:%d, link id %d")
TRACE_MSG(MUX_API_341_112_2_18_1_49_17_14,"MUX_Linkid_Couple get Linkid fail!")
TRACE_MSG(MUX_API_344_112_2_18_1_49_17_15,"MUX_Linkid_Couple return link id %d")
TRACE_MSG(MUX_COMMON_85_112_2_18_1_49_17_16,"MUX MUX_Crc_Check: CRC check failed\n")
TRACE_MSG(MUX_COMMON_188_112_2_18_1_49_18_17,"MUX: MUX_Default_ATC_Callback in link_id = %d, len = %d")
TRACE_MSG(MUX_COMMON_210_112_2_18_1_49_18_18,"MUX_Get_Linkid, 0XFF")
TRACE_MSG(MUX_COMMON_263_112_2_18_1_49_18_19,"MUX_Get_NewLinkid  fails")
TRACE_MSG(MUX_COMMON_278_112_2_18_1_49_18_20,"MUX_TraceDatat datalen = %d \n")
TRACE_MSG(MUX_COMMON_285_112_2_18_1_49_18_21,"0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, \r\n")
TRACE_MSG(MUX_COMMON_292_112_2_18_1_49_18_22,"0x%02x, \r\n")
TRACE_MSG(MUX_COMMON_333_112_2_18_1_49_18_23,"MUX_Assemble_CMUX_CMD: outptr is null pointer\r")
TRACE_MSG(MUX_CTL_94_112_2_18_1_49_18_24,"MUX_Startup fail due to MUX is in ON")
TRACE_MSG(MUX_CTL_102_112_2_18_1_49_18_25,"MUX_Startup fail due to parameter error")
TRACE_MSG(MUX_CTL_106_112_2_18_1_49_18_26,"MUX_Startup: From %d\r")
TRACE_MSG(MUX_CTL_170_112_2_18_1_49_18_27,"MUX_Startup: AT+CMUX command Failure!\r")
TRACE_MSG(MUX_CTL_181_112_2_18_1_49_18_28,"MUX_Startup: fail due to OPEN Control DLCI failure!\r")
TRACE_MSG(MUX_CTL_215_112_2_18_1_49_19_29,"MUX_ReadSysPara fail due to NOT Startup")
TRACE_MSG(MUX_CTL_238_112_2_18_1_49_19_30,"MUX_Res_Wait: g_MuxEvent_ptr is null ptr!")
TRACE_MSG(MUX_CTL_277_112_2_18_1_49_19_31,"MUX_Wait_WakeUp: g_MuxEvent_ptr is null ptr!")
TRACE_MSG(MUX_CTL_310_112_2_18_1_49_19_32,"MUX_SABM_Handler:Incomming connect on DLC %d; link_id %d\n")
TRACE_MSG(MUX_CTL_351_112_2_18_1_49_19_33,"MUX_SABM_Handler, DLC%d is exist, Sending MUX_UA \n")
TRACE_MSG(MUX_CTL_356_112_2_18_1_49_19_34,"MUX_SABM_Handler: invalid dlci %d, sending MUX_DM\n")
TRACE_MSG(MUX_CTL_381_112_2_18_1_49_19_35,"MUX: MUX_UA_Handler: Incomming CONNECTED  MUX_UA on dlci %d\n")
TRACE_MSG(MUX_CTL_388_112_2_18_1_49_19_36,"MUX:MUX_UA_Handler: Incomming DISCONNECTED  MUX_UA on dlci %d\n")
TRACE_MSG(MUX_CTL_406_112_2_18_1_49_19_37,"MUX_UA_Handler, unexpected response, discard,in link id %d,dlci %d\n")
TRACE_MSG(MUX_CTL_411_112_2_18_1_49_19_38,"MUX: MUX_UA_Handler: invalid dlci %d\n")
TRACE_MSG(MUX_CTL_432_112_2_18_1_49_19_39,"MUX: MUX_DM_Handler: Incomming MUX_DM on dlci %d\n")
TRACE_MSG(MUX_CTL_436_112_2_18_1_49_19_40,"MUX: MUX_DM_Handler: Incomming CONNECTED  MUX_DM on dlci %d\n")
TRACE_MSG(MUX_CTL_445_112_2_18_1_49_19_41,"MUX:MUX_DM_Handler: Incomming DISCONNECTED  MUX_DM on dlci %d\n")
TRACE_MSG(MUX_CTL_453_112_2_18_1_49_19_42,"MUX_DM_Handler, unexpected response, discard in link id %d dlci %d\n")
TRACE_MSG(MUX_CTL_458_112_2_18_1_49_19_43,"MUX_DM_Handler, invalid dlci %d\n")
TRACE_MSG(MUX_CTL_477_112_2_18_1_49_19_44,"MUX_DISC_Handler: Incomming MUX_DISC on channel %d\n")
TRACE_MSG(MUX_CTL_513_112_2_18_1_49_19_45,"MUX_DISC_Handler: invalid dlci %d\n")
TRACE_MSG(MUX_CTL_536_112_2_18_1_49_19_46,"MUX_UIH_Handler: invalid dlci %d\n")
TRACE_MSG(MUX_CTL_547_112_2_18_1_49_19_47,"MUX_UIH_Handler Error : DLCI %d (link %d)not connected, discard it!\n")
TRACE_MSG(MUX_CTL_576_112_2_18_1_49_19_48,"MUX_UIH_Handler P/F set error, discard it in link %d,dlci %d!\n")
TRACE_MSG(MUX_CTL_599_112_2_18_1_49_19_49,"Mux_Send_UA: Creating UA packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_646_112_2_18_1_49_20_50,"Mux_Send_UA: Fail in Creating UA packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_666_112_2_18_1_49_20_51,"MUX_Send_DM: Creating DM packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_713_112_2_18_1_49_20_52,"MUX_Send_DM: Fail in Creating DM packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_744_112_2_18_1_49_20_53,"Mux_Send_UIH: Fail due to length is too long in %d DLCI of %d bytes data\n")
TRACE_MSG(MUX_CTL_861_112_2_18_1_49_20_54,"Mux_Send_UIH: Fail in Creating UIH packet with %d bytes data to DLCI %d due to length is too long\n")
TRACE_MSG(MUX_CTL_874_112_2_18_1_49_20_55,"MUX_Send_UIH: Creating DM packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_905_112_2_18_1_49_20_56,"Mux_Send_SABM: Creating SABM packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_948_112_2_18_1_49_20_57,"Mux_Send_SABM: Fail in Creating SABM packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_981_112_2_18_1_49_20_58,"Mux_MCCFrame_Parse error DLCI %d in PN\n")
TRACE_MSG(MUX_CTL_984_112_2_18_1_49_20_59,"MUX: Received DLC %d (link %d)parameter negotiation, PN\n")
TRACE_MSG(MUX_CTL_1005_112_2_18_1_49_20_60,"MUX: Received DLC power saving msg\n")
TRACE_MSG(MUX_CTL_1030_112_2_18_1_49_20_61,"MUX: Received test  msg\n")
TRACE_MSG(MUX_CTL_1053_112_2_18_1_49_20_62,"MUX: Received FCON  msg\n")
TRACE_MSG(MUX_CTL_1076_112_2_18_1_49_20_63,"MUX: Received FCOFF msg\n")
TRACE_MSG(MUX_CTL_1109_112_2_18_1_49_20_64,"Mux_MCCFrame_Parse error DLCI %d in MSC\n")
TRACE_MSG(MUX_CTL_1116_112_2_18_1_49_20_65,"MUX reject received MSC message in dlci %d (link %d)\n")
TRACE_MSG(MUX_CTL_1127_112_2_18_1_49_20_66,"MUX Received Flow off on dlci %d (link %d)\n")
TRACE_MSG(MUX_CTL_1135_112_2_18_1_49_21_67,"MUX Received Flow on on dlci %d (link %d)\n")
TRACE_MSG(MUX_CTL_1147_112_2_18_1_49_21_68,"MUX Received Non supported command response\n")
TRACE_MSG(MUX_CTL_1152_112_2_18_1_49_21_69,"MUX Received a non supported command\n")
TRACE_MSG(MUX_CTL_1202_112_2_18_1_49_21_70,"Mux_Send_MCC_PN start in dlci %d")
TRACE_MSG(MUX_CTL_1236_112_2_18_1_49_21_71,"Mux_Send_MCC_PSC start cr = %d")
TRACE_MSG(MUX_CTL_1264_112_2_18_1_49_21_72,"Mux_Send_MCC_CLD start cr= %d")
TRACE_MSG(MUX_CTL_1291_112_2_18_1_49_21_73,"Mux_Send_MCC_TEST start")
TRACE_MSG(MUX_CTL_1310_112_2_18_1_49_21_74,"Mux_Send_MCC_TEST: TEST Message excede max length\r\n")
TRACE_MSG(MUX_CTL_1344_112_2_18_1_49_21_75,"Mux_Send_MCC_FCon start cr = %d")
TRACE_MSG(MUX_CTL_1372_112_2_18_1_49_21_76,"Mux_Send_MCC_FCoff start cr = %d")
TRACE_MSG(MUX_CTL_1404_112_2_18_1_49_21_77,"Mux_Send_MCC_MSC start in dlci %d")
TRACE_MSG(MUX_CTL_1438_112_2_18_1_49_21_78,"Mux_Send_MCC_NSC START")
TRACE_MSG(MUX_CTL_1467_112_2_18_1_49_21_79,"Mux_Send_Disc: Creating DISC packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_1507_112_2_18_1_49_21_80,"MUX_Send_Disc: Fail in Creating DISC packet to DLCI %d\n")
TRACE_MSG(MUX_CTL_1533_112_2_18_1_49_21_81,"Mux_Close_Channel: DLC %d is NOT exist \n")
TRACE_MSG(MUX_CTL_1539_112_2_18_1_49_21_82,"Mux_Close_Channel: DLC %d (link %d) is already in DISC state\n")
TRACE_MSG(MUX_CTL_1546_112_2_18_1_49_21_83,"Mux_Close_Channel: DLC %d (link %d) is already in DISC state\n")
TRACE_MSG(MUX_CTL_1551_112_2_18_1_49_21_84,"Mux_Close_Channel on channel %d (link %d)\n")
TRACE_MSG(MUX_CTL_1571_112_2_18_1_49_21_85,"MUX DLCI %d (link %d) Send DISC timeout!\n")
TRACE_MSG(MUX_CTL_1611_112_2_18_1_49_21_86,"MUX_Close_Link fail due to NOT Startup")
TRACE_MSG(MUX_CTL_1619_112_2_18_1_49_21_87,"MUX_Close_Link, NOT allow release DLC0\n")
TRACE_MSG(MUX_CTL_1642_112_2_18_1_49_22_88,"MUX_Exit fail due to NOT Startup")
TRACE_MSG(MUX_CTL_1684_112_2_18_1_49_22_89,"MUX_Open_Dlci  DLCI:%d  NOT Exist!\n")
TRACE_MSG(MUX_CTL_1693_112_2_18_1_49_22_90,"MUX_Open_Dlci(dlci %d, link %d)  DLCI0  NOT Exist!\n")
TRACE_MSG(MUX_CTL_1698_112_2_18_1_49_22_91,"MUX_Open_Dlci(dlci %d, link %d) due to link_status error of DLCI0!\n")
TRACE_MSG(MUX_CTL_1703_112_2_18_1_49_22_92,"MUX_Open_Dlci: DLCI:%d(link %d) Start!\n")
TRACE_MSG(MUX_CTL_1794_112_2_18_1_49_22_93,"MUX_Open_Dlci: MUX DLCI:%d Send MUX_PN_MSG timeout!\n")
TRACE_MSG(MUX_CTL_1818_112_2_18_1_49_22_94,"MUX DLCI:%d Send SABM got rejected!\n")
TRACE_MSG(MUX_CTL_1824_112_2_18_1_49_22_95,"MUX DLCI:%d Send SABM timeout!\n")
TRACE_MSG(MUX_CTL_1865_112_2_18_1_49_22_96,"MUX: MUX_Open_Link fail due to NOT Startup")
TRACE_MSG(MUX_CTL_1875_112_2_18_1_49_22_97,"MUX: MUX_Open_Link fail due to there have a link %d is in establishing process\r\n")
TRACE_MSG(MUX_CTL_1886_112_2_18_1_49_22_98,"MUX: MUX_Open_Link fail due to that there can NOT allocate new link id\r\n")
TRACE_MSG(MUX_CTL_1906_112_2_18_1_49_22_99,"MUX: MUX_Open_Link Failure, link_id = %d, DLCI= %d\r\n")
TRACE_MSG(MUX_CTL_1921_112_2_18_1_49_22_100,"MUX_Upon_Close: MUX_%d not startup, directly return!")
TRACE_MSG(MUX_CTL_2092_112_2_18_1_49_22_101,"MUX: NOT receive OK, return failure\r\n")
TRACE_MSG(MUX_DRIVER_148_112_2_18_1_49_23_102,"mux has %d data need send")
TRACE_MSG(MUX_DRIVER_177_112_2_18_1_49_23_103,"MUX_Create: Fail due to port(%d) parameter error")
TRACE_MSG(MUX_DRIVER_191_112_2_18_1_49_23_104,"MUX_Create: Fail due to phy_port parameter error in port %d")
TRACE_MSG(MUX_DRIVER_284_112_2_18_1_49_23_105,"MUX_Create: Open UART/USB success")
TRACE_MSG(MUX_DRIVER_310_112_2_18_1_49_24_106,"MUX_Reset: Fail due to port(%d) parameter error")
TRACE_MSG(MUX_DRIVER_336_112_2_18_1_49_24_107,"MUX_Reset: Open UART/USB success")
TRACE_MSG(MUX_DRIVER_364_112_2_18_1_49_24_108,"MUX_Close: Fail due to port(%d) parameter error")
TRACE_MSG(MUX_DRIVER_390_112_2_18_1_49_24_109,"MUX_Close: Close UART/USB success")
TRACE_MSG(MUX_DRIVER_631_112_2_18_1_49_24_110,"MUX_RxForPort (%d): Disable Out_endp interrupt of usb")
TRACE_MSG(MUX_DRIVER_639_112_2_18_1_49_24_111,"MUX_RxForPort (%d): Software flow control:send XOFF")
TRACE_MSG(MUX_DRIVER_662_112_2_18_1_49_24_112,"MUX_RxForPort (%d): Rx buffer over writtn!")
TRACE_MSG(MUX_DRIVER_793_112_2_18_1_49_25_113,"MUX_Data_Tx fails  len is zero!!")
TRACE_MSG(MUX_DRIVER_799_112_2_18_1_49_25_114,"MUX_Data_Tx fails  PORT %d is closed!")
TRACE_MSG(MUX_DRIVER_806_112_2_18_1_49_25_115,"MUX_Data_Tx fails in MUX_%d due to get MUTEX fails")
TRACE_MSG(MUX_DRIVER_822_112_2_18_1_49_25_116,"  mux free_size low 0x203000d0[0x%x],0x203000cc[0x%x],0x203000c0[0x%x],0x203000d8[0x%x]")
TRACE_MSG(MUX_DRIVER_833_112_2_18_1_49_25_117,"MUX_Data_Tx full skip date %d in %d PORT")
TRACE_MSG(MUX_DRIVER_838_112_2_18_1_49_25_118,"0x203000d0[0x%x],0x203000cc[0x%x],0x203000c0[0x%x],0x203000d8[0x%x]")
TRACE_MSG(MUX_DRIVER_1039_112_2_18_1_49_25_119,"MUX_Fill_Rxbuff: Disable Out_endp interrupt of usb")
TRACE_MSG(MUX_DRIVER_1046_112_2_18_1_49_25_120,"MUX_Fill_Rxbuff: Software flow control:send XOFF")
TRACE_MSG(MUX_DRIVER_1069_112_2_18_1_49_25_121,"MUX_Fill_Rxbuff: Rx buffer over writtn!")
TRACE_MSG(MUX_RECV_70_112_2_18_1_49_25_122,"MUX_Res_Wait: g_MuxRecv_Eptr is null ptr in MUX entry1!")
TRACE_MSG(MUX_RECV_171_112_2_18_1_49_26_123,"MUX_Res_Wait: g_MuxRecv_Eptr is null ptr in MUX entry2!")
TRACE_MSG(MUX_RECV_348_112_2_18_1_49_26_124,"MUX_Get_Data_Block discard data(len=%d)  in %d:")
TRACE_MSG(MUX_RECV_368_112_2_18_1_49_26_125,"MUX_Get_Data_Block: work_mode error")
TRACE_MSG(MUX_RECV_433_112_2_18_1_49_26_126,"MUX_Forward_Up: ATC task Not Ready,Discard Recv Data!")
TRACE_MSG(MUX_RECV_446_112_2_18_1_49_26_127,"MUX_Forward_Up: REcv task Not Ready,Discard Recv Data!")
TRACE_MSG(MUX_RECV_542_112_2_18_1_49_27_128,"MUX Error: frame length:%d is bigger than Max total frame size:%d\n")
TRACE_MSG(MUX_RECV_588_112_2_18_1_49_27_129,"MUX: Lost synchronization in MUX %d\n")
TRACE_MSG(MUX_RECV_671_112_2_18_1_49_27_130,"MUX MUX_Data_Parse_Advance: !!!!! Exceed buffer boundary in MUX %d!\n")
TRACE_MSG(MUX_RECV_714_112_2_18_1_49_27_131,"MUX Error: frame length:%d is bigger than Max total frame size:%d\n")
TRACE_MSG(MUX_RECV_750_112_2_18_1_49_27_132,"MUX: CRC check failure!\n")
TRACE_MSG(MUX_RECV_812_112_2_18_1_49_27_133,"Mux_Frame_Parse: illegal packet in DLCI %d\n")
END_TRACE_MAP(MS_REF_MUX_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_MUX_TRC_H_


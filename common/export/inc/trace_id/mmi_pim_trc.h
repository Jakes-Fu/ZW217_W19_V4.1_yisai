/******************************************************************************
 ** File Name:      mmi_pim_trc.h                                         *
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
//trace_id:262
#ifndef _MMI_PIM_TRC_H_
#define _MMI_PIM_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define PIMCALLBACK_HANDLER_836_112_2_18_3_25_24_0 "MMIPIM @@hs_smlPcdata2String size=%d"
#define PIMCALLBACK_HANDLER_897_112_2_18_3_25_24_1 "MMIPIM @@hs_smlString2unsignedInt =%s"
#define PIMCALLBACK_HANDLER_951_112_2_18_3_25_24_2 "MMIPIM  @@ leave hs_smlString2unsignedInt =%d"
#define PIMCALLBACK_HANDLER_1251_112_2_18_3_25_24_3 "MMIPIM @@handle alert@@"
#define PIMCALLBACK_HANDLER_1259_112_2_18_3_25_24_4 "MMIPIM @@handle alert@@ alert for two way sync"
#define PIMCALLBACK_HANDLER_1265_112_2_18_3_25_24_5 "MMIPIM @@handle alert@@ alert for slow sync"
#define PIMCALLBACK_HANDLER_1271_112_2_18_3_25_24_6 "MMIPIM @@handle alert@@alert for more"
#define PIMCALLBACK_HANDLER_1278_112_2_18_3_25_24_7 "MMIPIM @@handle alert@@ alert for one way sync from client only!"
#define PIMCALLBACK_HANDLER_1286_112_2_18_3_25_25_8 "MMIPIM @@handle alert@@ alert for refresh sync from client only!"
#define PIMCALLBACK_HANDLER_1294_112_2_18_3_25_25_9 "MMIPIM @@handle alert@@ alert for one way sync from server only!"
#define PIMCALLBACK_HANDLER_1302_112_2_18_3_25_25_10 "MMIPIM @@handle alert@@ alert for refresh sync from server only!"
#define PIMCALLBACK_HANDLER_1385_112_2_18_3_25_25_11 "^@@^MMIPIM hs_myHandleFinal !"
#define PIMCALLBACK_HANDLER_1663_112_2_18_3_25_25_12 "MMIPIM &&& enter hs_myGetNumberofChanges"
#define PIMCALLBACK_HANDLER_1693_112_2_18_3_25_25_13 "MMIPIM &&& add_num=%d,del=%d,rep=%d,num=%d"
#define PIMCALLBACK_HANDLER_1713_112_2_18_3_25_25_14 "MMIPIM &&& enter hs_myGetNumberofAllItem"
#define PIMCALLBACK_HANDLER_1744_112_2_18_3_25_25_15 "MMIPIM &&&buf_lenth=%d"
#define PIMCALLBACK_HANDLER_1767_112_2_18_3_25_26_16 "MMIPIM &&& hs_myhttplogs"
#define PIMCODEC_2147_112_2_18_3_25_30_17 "mmipim hs_devinfEncBlock tag id=%d "
#define PIMCODEC_4789_112_2_18_3_25_36_18 "MMIPIM  ----------------------------------------TN_CMDID"
#define PIMCODEC_4798_112_2_18_3_25_36_19 "MMIPIM  ----------------------------------------TN_MSGREF"
#define PIMCODEC_4806_112_2_18_3_25_36_20 "MMIPIM  ----------------------------------------TN_CMDREF"
#define PIMCODEC_4814_112_2_18_3_25_36_21 "MMIPIM  ----------------------------------------TN_CMD"
#define PIMCODEC_4823_112_2_18_3_25_36_22 "MMIPIM  ----------------------------------------TN_DATA"
#define PIMCODEC_4832_112_2_18_3_25_36_23 "MMIPIM  ----------------------------------------TN_CHAL"
#define PIMCODEC_4840_112_2_18_3_25_36_24 "MMIPIM  ----------------------------------------TN_CRED"
#define PIMCODEC_4850_112_2_18_3_25_36_25 "MMIPIM  ----------------------------------------TN_ITEM"
#define PIMCODEC_4858_112_2_18_3_25_36_26 "MMIPIM  ----------------------------------------TN_TARGETREF"
#define PIMCODEC_4866_112_2_18_3_25_36_27 "MMIPIM  ----------------------------------------TN_SOURCEREF"
#define PIMCODEC_4875_112_2_18_3_25_36_28 "MMIPIM-----------------------------------------hs_smlFreeStatus  11111111 rc = %4x"
#define PIMCODEC_4881_112_2_18_3_25_36_29 "MMIPIM-----------------------------------------hs_smlFreeStatus  22222222 rc = %4x"
#define PIMCODEC_4890_112_2_18_3_25_36_30 "MMIPIM-----------------------------------------hs_smlFreeStatus  3333333 rc = %4x"
#define PIMCOMMHTTP_234_112_2_18_3_25_38_31 "MMIPIM ** pdp has been deactived!"
#define PIMCOMMHTTP_251_112_2_18_3_25_38_32 "MMIPIM *********read socket select ..... "
#define PIMCOMMHTTP_265_112_2_18_3_25_38_33 "MMIPIM **receive data error: Error %d"
#define PIMCOMMHTTP_279_112_2_18_3_25_38_34 "MMIPIM *********write socket select ..... "
#define PIMCOMMHTTP_301_112_2_18_3_25_38_35 "MMIPIM ** send data error: Error %d"
#define PIMCOMMHTTP_317_112_2_18_3_25_38_36 "MMIPIM ** communication ,no socket is avilable"
#define PIMCOMMHTTP_345_112_2_18_3_25_38_37 "mmipim create pim_socket"
#define PIMCOMMHTTP_407_112_2_18_3_25_38_38 "mmipim socket close socket=%d"
#define PIMCOMMHTTP_443_112_2_18_3_25_39_39 "mmipim socket_connect socket =%d"
#define PIMCOMMHTTP_499_112_2_18_3_25_39_40 "mmipim socket_recv socket =%d"
#define PIMCOMMHTTP_527_112_2_18_3_25_39_41 "MMIPIM HS_HTTP_Prepare "
#define PIMCOMMHTTP_658_112_2_18_3_25_39_42 "MMIPIM HS_HTTP_Release "
#define PIMCOMMHTTP_671_112_2_18_3_25_39_43 "MMIPIM HS_HTTP_Release 1"
#define PIMCOMMHTTP_677_112_2_18_3_25_39_44 "MMIPIM HS_HTTP_Release 2"
#define PIMCOMMHTTP_683_112_2_18_3_25_39_45 "MMIPIM HS_HTTP_Release 3"
#define PIMCOMMHTTP_690_112_2_18_3_25_39_46 "MMIPIM HS_HTTP_Release 4"
#define PIMCOMMHTTP_696_112_2_18_3_25_39_47 "MMIPIM HS_HTTP_Release 5"
#define PIMCOMMHTTP_702_112_2_18_3_25_39_48 "MMIPIM HS_HTTP_Release 6"
#define PIMCOMMHTTP_708_112_2_18_3_25_39_49 "MMIPIM HS_HTTP_Release 7"
#define PIMCOMMHTTP_714_112_2_18_3_25_39_50 "MMIPIM HS_HTTP_Release 8"
#define PIMCOMMHTTP_721_112_2_18_3_25_39_51 "MMIPIM HS_HTTP_Release 9"
#define PIMCOMMHTTP_724_112_2_18_3_25_39_52 "MMIPIM HS_HTTP_Release 10"
#define PIMCOMMHTTP_767_112_2_18_3_25_39_53 "MMIPIM **Socket init fail!"
#define PIMCOMMHTTP_774_112_2_18_3_25_39_54 "MMIPIM ** Connect Successful:"
#define PIMCOMMHTTP_783_112_2_18_3_25_39_55 "MMIPIM ** Connect Failed: Error %d"
#define PIMCOMMHTTP_811_112_2_18_3_25_39_56 "MMIPIM **HS_HTTP_SendData"
#define PIMCOMMHTTP_817_112_2_18_3_25_39_57 "MMIPIM ** pdp has been deactived!"
#define PIMCOMMHTTP_834_112_2_18_3_25_39_58 "MMIPIM ** HS_HTTP_SendData SOCKET_STEP_CONNECT"
#define PIMCOMMHTTP_837_112_2_18_3_25_39_59 "MMIPIM **Begin open socket\n"
#define PIMCOMMHTTP_845_112_2_18_3_25_39_60 "MMIPIM ** error! proxy address and port convert error."
#define PIMCOMMHTTP_849_112_2_18_3_25_39_61 "MMIPIM **=======Connect %s:%s========="
#define PIMCOMMHTTP_856_112_2_18_3_25_39_62 "MMIPIM ** error! server address and port convert error."
#define PIMCOMMHTTP_860_112_2_18_3_25_39_63 "MMIPIM **=======Connect %s:%s========="
#define PIMCOMMHTTP_876_112_2_18_3_25_39_64 "MMIPIM ** Connect Successful:"
#define PIMCOMMHTTP_888_112_2_18_3_25_39_65 "MMIPIM ** HTTP protocol error code=%d"
#define PIMCOMMHTTP_891_112_2_18_3_25_39_66 "MMIPIM ** Connect has been done:"
#define PIMCOMMHTTP_907_112_2_18_3_25_39_67 "MMIPIM **Socket init fail!"
#define PIMCOMMHTTP_917_112_2_18_3_25_40_68 "MMIPIM ** socket already connect!"
#define PIMCOMMHTTP_932_112_2_18_3_25_40_69 "MMIPIM **SOCKET_STEP_1"
#define PIMCOMMHTTP_938_112_2_18_3_25_40_70 "MMIPIM ** send length=%d"
#define PIMCOMMHTTP_943_112_2_18_3_25_40_71 "MMIPIM  ^_^ Begin Write xml logs.\n"
#define PIMCOMMHTTP_946_112_2_18_3_25_40_72 "MMIPIM  ^_^ Write xml logs complete.\n"
#define PIMCOMMHTTP_957_112_2_18_3_25_40_73 "MMIPIM ** send next packet"
#define PIMCOMMHTTP_962_112_2_18_3_25_40_74 "MMIPIM ** writing http header complete..."
#define PIMCOMMHTTP_969_112_2_18_3_25_40_75 "MMIPIM ** post data sent event"
#define PIMCOMMHTTP_982_112_2_18_3_25_40_76 "MMIPIM ** HTTP protocol error code=%d"
#define PIMCOMMHTTP_987_112_2_18_3_25_40_77 "MMIPIM ** writing header..."
#define PIMCOMMHTTP_994_112_2_18_3_25_40_78 "MMIPIM ** Write Data Failed"
#define PIMCOMMHTTP_1006_112_2_18_3_25_40_79 "MMIPIM **HS_HTTP_SendData SOCKET_STEP_2"
#define PIMCOMMHTTP_1011_112_2_18_3_25_40_80 "MMIPIM ** send length=%d"
#define PIMCOMMHTTP_1026_112_2_18_3_25_40_81 "MMIPIM ** send next packet"
#define PIMCOMMHTTP_1034_112_2_18_3_25_40_82 "MMIPIM ** writing syncml body data complete..."
#define PIMCOMMHTTP_1041_112_2_18_3_25_40_83 "MMIPIM **post data sent event"
#define PIMCOMMHTTP_1054_112_2_18_3_25_40_84 "MMIPIM ** HTTP protocol error code=%d"
#define PIMCOMMHTTP_1058_112_2_18_3_25_40_85 "MMIPIM ** writing contents..."
#define PIMCOMMHTTP_1065_112_2_18_3_25_40_86 "MMIPIM ** Write Failed: Error %d\n"
#define PIMCOMMHTTP_1079_112_2_18_3_25_40_87 "MMIPIM **HS_HTTP_SendData SOCKET_STEP_3"
#define PIMCOMMHTTP_1082_112_2_18_3_25_40_88 "MMIPIM **Send all data ok!"
#define PIMCOMMHTTP_1096_112_2_18_3_25_40_89 "MMIPIM **HS_HTTP_SendData ,step type error!"
#define PIMCOMMHTTP_1131_112_2_18_3_25_40_90 "MMIPIM ** socket=-1"
#define PIMCOMMHTTP_1137_112_2_18_3_25_40_91 "MMIPIM ** pdp has been deactived!"
#define PIMCOMMHTTP_1148_112_2_18_3_25_40_92 "MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_1"
#define PIMCOMMHTTP_1151_112_2_18_3_25_40_93 "MMIPIM chenxiang !!!!!!!hs_totalrec == %d"
#define PIMCOMMHTTP_1154_112_2_18_3_25_40_94 "MMIPIM ** http contents received is %d"
#define PIMCOMMHTTP_1160_112_2_18_3_25_40_95 "MMIPIM ** Begin read  http head data !\n"
#define PIMCOMMHTTP_1162_112_2_18_3_25_40_96 "MMIPIM ** http head data  read complete!\n"
#define PIMCOMMHTTP_1170_112_2_18_3_25_40_97 "MMIPIM ** All packets less than HS_HEADER_BUFFER_LEN bytes!"
#define PIMCOMMHTTP_1180_112_2_18_3_25_40_98 "MMIPIM ---------------(HTTP 500)---------------------"
#define PIMCOMMHTTP_1194_112_2_18_3_25_40_99 "MMIPIM **socket connection closed."
#define PIMCOMMHTTP_1204_112_2_18_3_25_40_100 "MMIPIM ** HTTP protocol error!return status is not 200 ok!..."
#define PIMCOMMHTTP_1208_112_2_18_3_25_40_101 "MMIPIM ** http headersize=%d"
#define PIMCOMMHTTP_1209_112_2_18_3_25_40_102 "MMIPIM ** http contents size=%d"
#define PIMCOMMHTTP_1218_112_2_18_3_25_40_103 "MMIPIM ** reading syncml contents complete..."
#define PIMCOMMHTTP_1230_112_2_18_3_25_40_104 "MMIPIM ** Reading http header less than HS_HEADER_BUFFER_LEN"
#define PIMCOMMHTTP_1254_112_2_18_3_25_40_105 "MMIPIM ** HTTP protocol error code=%d"
#define PIMCOMMHTTP_1258_112_2_18_3_25_40_106 "MMIPIM ** Reading syncml body data ..."
#define PIMCOMMHTTP_1266_112_2_18_3_25_40_107 "MMIPIM -------------(errno == EPIPE)---------------------"
#define PIMCOMMHTTP_1280_112_2_18_3_25_40_108 "MMIPIM **socket connection closed."
#define PIMCOMMHTTP_1288_112_2_18_3_25_40_109 "MMIPIM ** Read Failed"
#define PIMCOMMHTTP_1302_112_2_18_3_25_40_110 "MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_2"
#define PIMCOMMHTTP_1306_112_2_18_3_25_40_111 "MMIPIM ** http contents received is %d"
#define PIMCOMMHTTP_1317_112_2_18_3_25_40_112 "MMIPIM ** content_length in header is %d"
#define PIMCOMMHTTP_1318_112_2_18_3_25_40_113 "MMIPIM ** http contents received is %d"
#define PIMCOMMHTTP_1325_112_2_18_3_25_40_114 "MMIPIM ** reading syncml contents complete..."
#define PIMCOMMHTTP_1340_112_2_18_3_25_40_115 "MMIPIM ** receive next packet"
#define PIMCOMMHTTP_1346_112_2_18_3_25_40_116 "MMIPIM ** HTTP protocol error!receive count not equal to content length"
#define PIMCOMMHTTP_1359_112_2_18_3_25_40_117 "MMIPIM ** HTTP protocol error code=%d"
#define PIMCOMMHTTP_1363_112_2_18_3_25_40_118 "MMIPIM ** Reading contents..."
#define PIMCOMMHTTP_1370_112_2_18_3_25_40_119 "MMIPIM -------------(errno == EPIPE)---------------------"
#define PIMCOMMHTTP_1383_112_2_18_3_25_41_120 "MMIPIM **socket connection closed."
#define PIMCOMMHTTP_1390_112_2_18_3_25_41_121 "MMIPIM ** Read Failed: Error %d\n"
#define PIMCOMMHTTP_1404_112_2_18_3_25_41_122 "MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_3"
#define PIMCOMMHTTP_1406_112_2_18_3_25_41_123 "MMIPIM **Receive all data ok!"
#define PIMCOMMHTTP_1446_112_2_18_3_25_41_124 "MMIPIM **socket connection closed."
#define PIMCOMMHTTP_1556_112_2_18_3_25_41_125 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMCOMMHTTP_1574_112_2_18_3_25_41_126 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMCOMMHTTP_1581_112_2_18_3_25_41_127 "MMIPIM chenxiang !!!!!!! hs_http_globle->usedsize == %d"
#define PIMCOMMHTTP_1584_112_2_18_3_25_41_128 "MMIPIM chenxiang !!!!!!! ------------headerbuffer------start------"
#define PIMCOMMHTTP_1586_112_2_18_3_25_41_129 "%s"
#define PIMCOMMHTTP_1587_112_2_18_3_25_41_130 "MMIPIM chenxiang !!!!!!! ------------headerbuffer-------end-----"
#define PIMCOMMHTTP_1611_112_2_18_3_25_41_131 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMCOMMHTTP_1621_112_2_18_3_25_41_132 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMCOMMHTTP_1631_112_2_18_3_25_41_133 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMCOMMHTTP_1683_112_2_18_3_25_41_134 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMCOMMHTTP_1695_112_2_18_3_25_41_135 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMCOMMHTTP_1729_112_2_18_3_25_41_136 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMCOMMHTTP_1741_112_2_18_3_25_41_137 "MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s"
#define PIMDATASYNCTASK_144_112_2_18_3_25_43_138 "MMIPIM  ^_^ enter syncml hs_Data_Sync_terminate!\n"
#define PIMDATASYNCTASK_177_112_2_18_3_25_43_139 "MMIPIM  ^_^ syncml hs_Data_Sync_terminate! free last_anchor\n"
#define PIMDATASYNCTASK_183_112_2_18_3_25_43_140 "MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free next_anchor\n"
#define PIMDATASYNCTASK_191_112_2_18_3_25_43_141 "MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free proxy_ip\n"
#define PIMDATASYNCTASK_261_112_2_18_3_25_43_142 "MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free syncml_pim_nextnonce\n"
#define PIMDATASYNCTASK_265_112_2_18_3_25_43_143 "MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free hs_task_relay_info\n"
#define PIMDATASYNCTASK_274_112_2_18_3_25_43_144 "MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free hs_status_cmd_que\n"
#define PIMDATASYNCTASK_280_112_2_18_3_25_43_145 "MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free all instance\n"
#define PIMDATASYNCTASK_298_112_2_18_3_25_43_146 "MMIPIM  ^_^leave syncml hs_Data_Sync_terminate!\n"
#define PIMDATASYNCTASK_317_112_2_18_3_25_43_147 "MMIPIM  ^_^ Begin client init!\n"
#define PIMDATASYNCTASK_325_112_2_18_3_25_43_148 "MMIPIM  ^_^ Begin Server init!\n"
#define PIMDATASYNCTASK_333_112_2_18_3_25_43_149 "MMIPIM  ^_^ Begin client modify!\n"
#define PIMDATASYNCTASK_342_112_2_18_3_25_43_150 "MMIPIM  ^_^ Begin Server modify!\n"
#define PIMDATASYNCTASK_351_112_2_18_3_25_43_151 "MMIPIM  ^_^ Begin client status and map send!\n"
#define PIMDATASYNCTASK_359_112_2_18_3_25_43_152 "MMIPIM  ^_^ Begin server map ack!\n"
#define PIMDATASYNCTASK_369_112_2_18_3_25_43_153 "MMIPIM  ^_^ All sync operation is over,then release the resources!\n"
#define PIMDATASYNCTASK_371_112_2_18_3_25_43_154 "MMIPIM  ^_^ synchornization is ok! save the last anchor!"
#define PIMDATASYNCTASK_385_112_2_18_3_25_43_155 "MMIPIM  ^_^ more data,send alert 222 for more!\n"
#define PIMDATASYNCTASK_393_112_2_18_3_25_43_156 "MMIPIM  *_* STEP_SYNC_ERROR!\n"
#define PIMDATASYNCTASK_429_112_2_18_3_25_43_157 "MMIPIM  ^_^ Begin hs_got_free_id workspaceid= %d"
#define PIMDATASYNCTASK_447_112_2_18_3_25_43_158 "MMIPIM  ^_^ Begin hs_step_client_init_sync_process,time=%d"
#define PIMDATASYNCTASK_455_112_2_18_3_25_43_159 "MMIPIM  ^_^ Begin step_client_init_sync_process1,sync_mode=%d"
#define PIMDATASYNCTASK_457_112_2_18_3_25_43_160 "MMIPIM  ^_^ Begin step_client_init_sync_process1"
#define PIMDATASYNCTASK_470_112_2_18_3_25_43_161 "MMIPIM  ^_^ Begin step_client_init_sync_process2"
#define PIMDATASYNCTASK_503_112_2_18_3_25_44_162 "MMIPIM  ^_^ Begin step_client_init_sync_process3"
#define PIMDATASYNCTASK_532_112_2_18_3_25_44_163 "MMIPIM  ^_^ Begin Write xml logs.\n"
#define PIMDATASYNCTASK_534_112_2_18_3_25_44_164 "MMIPIM  ^_^ xml logs write complete.\n"
#define PIMDATASYNCTASK_537_112_2_18_3_25_44_165 "MMIPIM  ^_^ client init data ok,begin send!time=%d \n"
#define PIMDATASYNCTASK_540_112_2_18_3_25_44_166 "MMIPIM  ^_^ client init data send ok!time=%d \n"
#define PIMDATASYNCTASK_562_112_2_18_3_25_44_167 "MMIPIM  ^_^ Begin hs_step_server_init_sync_process 1,time=%d"
#define PIMDATASYNCTASK_567_112_2_18_3_25_44_168 "MMIPIM  ^_^ prepare ok,begin receive!\n"
#define PIMDATASYNCTASK_569_112_2_18_3_25_44_169 "MMIPIM  ^_^ server init receive ok!\n"
#define PIMDATASYNCTASK_576_112_2_18_3_25_44_170 "MMIPIM  ^_^ Begin hs_step_server_init_sync_process 2 ,time=%d"
#define PIMDATASYNCTASK_579_112_2_18_3_25_44_171 "MMIPIM  ^_^ Begin Write xml logs.\n"
#define PIMDATASYNCTASK_581_112_2_18_3_25_44_172 "MMIPIM  ^_^ xml logs write complete.\n"
#define PIMDATASYNCTASK_583_112_2_18_3_25_44_173 "MMIPIM  ^_^ analysis recevie data.\n"
#define PIMDATASYNCTASK_585_112_2_18_3_25_44_174 "MMIPIM  ^_^ analysis server init complete.\n"
#define PIMDATASYNCTASK_649_112_2_18_3_25_44_175 "MMIPIM  ^_^ Begin hs_step_client_modi_process,time=%d"
#define PIMDATASYNCTASK_650_112_2_18_3_25_44_176 "MMIPIM  ^_^ enter hs_step_client_modi_process,synctype=%d"
#define PIMDATASYNCTASK_664_112_2_18_3_25_44_177 "MMIPIM  ^_^ prepare to send cache map....."
#define PIMDATASYNCTASK_672_112_2_18_3_25_44_178 "MMIPIM  ^_^ workspaceid=%d,authored=%d"
#define PIMDATASYNCTASK_685_112_2_18_3_25_44_179 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_S2WAY\n"
#define PIMDATASYNCTASK_703_112_2_18_3_25_44_180 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFCLI \n"
#define PIMDATASYNCTASK_711_112_2_18_3_25_44_181 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFCLI\n"
#define PIMDATASYNCTASK_721_112_2_18_3_25_44_182 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFSER\n"
#define PIMDATASYNCTASK_728_112_2_18_3_25_44_183 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFSER\n"
#define PIMDATASYNCTASK_744_112_2_18_3_25_44_184 "MMIPIM  ^_^add_is_final=%d, delete_is_final=%d,replace_is_final=%d"
#define PIMDATASYNCTASK_766_112_2_18_3_25_44_185 "MMIPIM  ^_^ Begin Write xml logs.\n"
#define PIMDATASYNCTASK_768_112_2_18_3_25_44_186 "MMIPIM  ^_^ Write xml logs complete.\n"
#define PIMDATASYNCTASK_771_112_2_18_3_25_44_187 "MMIPIM  ^_^ client modi data ok,begin send!time=%d"
#define PIMDATASYNCTASK_774_112_2_18_3_25_44_188 "MMIPIM  ^_^ client modi data send ok!\n"
#define PIMDATASYNCTASK_792_112_2_18_3_25_44_189 "@@step_server_conf_modi_processf  ,start time=%d"
#define PIMDATASYNCTASK_798_112_2_18_3_25_44_190 "MMIPIM  ^_^ Begin hs_step_server_conf_modi_process 1,time=%d"
#define PIMDATASYNCTASK_803_112_2_18_3_25_44_191 "MMIPIM  ^_^ prepare ok,begin receive!\n"
#define PIMDATASYNCTASK_805_112_2_18_3_25_44_192 "MMIPIM  ^_^ server init receive ok!\n"
#define PIMDATASYNCTASK_812_112_2_18_3_25_44_193 "MMIPIM  ^_^ Begin hs_step_server_conf_modi_process 2,time=%d"
#define PIMDATASYNCTASK_816_112_2_18_3_25_44_194 "MMIPIM  ^_^ Begin Write xml logs.\n"
#define PIMDATASYNCTASK_818_112_2_18_3_25_44_195 "MMIPIM  ^_^ xml logs write complete.\n"
#define PIMDATASYNCTASK_821_112_2_18_3_25_44_196 "MMIPIM  ^_^ analysis recevie data.\n"
#define PIMDATASYNCTASK_823_112_2_18_3_25_44_197 "MMIPIM  ^_^ analysis server conf complete.\n"
#define PIMDATASYNCTASK_859_112_2_18_3_25_44_198 "@@step_server_conf_modi_processf  ,end time=%d"
#define PIMDATASYNCTASK_881_112_2_18_3_25_44_199 "MMIPIM  ^_^ Begin hs_step_client_conf_map_process,time=%d"
#define PIMDATASYNCTASK_912_112_2_18_3_25_45_200 "MMIPIM  ^_^ Begin Write xml logs.\n"
#define PIMDATASYNCTASK_916_112_2_18_3_25_45_201 "MMIPIM  ^_^ Write xml logs complete.\n"
#define PIMDATASYNCTASK_918_112_2_18_3_25_45_202 "MMIPIM  ^_^ client conf data ok,begin send!time=%d"
#define PIMDATASYNCTASK_921_112_2_18_3_25_45_203 "MMIPIM  ^_^ client conf data send ok!\n"
#define PIMDATASYNCTASK_945_112_2_18_3_25_45_204 "MMIPIM  ^_^ Begin hs_step_server_mapack_process  1,time=%d"
#define PIMDATASYNCTASK_950_112_2_18_3_25_45_205 "MMIPIM  ^_^ prepare ok,begin receive!\n"
#define PIMDATASYNCTASK_952_112_2_18_3_25_45_206 "MMIPIM  ^_^ server map ack receive ok!\n"
#define PIMDATASYNCTASK_959_112_2_18_3_25_45_207 "MMIPIM  ^_^ Begin hs_step_server_mapack_process  2,time=%d"
#define PIMDATASYNCTASK_970_112_2_18_3_25_45_208 "MMIPIM  ^_^ Begin Write xml logs.\n"
#define PIMDATASYNCTASK_972_112_2_18_3_25_45_209 "MMIPIM  ^_^ xml logs write complete.\n"
#define PIMDATASYNCTASK_975_112_2_18_3_25_45_210 "MMIPIM  ^_^ analysis recevie data.\n"
#define PIMDATASYNCTASK_977_112_2_18_3_25_45_211 "MMIPIM  ^_^ analysis server map ack complete.\n"
#define PIMDATASYNCTASK_1018_112_2_18_3_25_45_212 "MMIPIM  ^_^ Begin hs_step_server_more,time=%d"
#define PIMDATASYNCTASK_1051_112_2_18_3_25_45_213 "MMIPIM  ^_^ send cache map another times....."
#define PIMDATASYNCTASK_1058_112_2_18_3_25_45_214 "MMIPIM  ^_^ workspaceid=%d,authored=%d"
#define PIMDATASYNCTASK_1071_112_2_18_3_25_45_215 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_S2WAY\n"
#define PIMDATASYNCTASK_1081_112_2_18_3_25_45_216 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFCLI\n"
#define PIMDATASYNCTASK_1086_112_2_18_3_25_45_217 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFCLI\n"
#define PIMDATASYNCTASK_1091_112_2_18_3_25_45_218 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFSER\n"
#define PIMDATASYNCTASK_1095_112_2_18_3_25_45_219 "MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFSER\n"
#define PIMDATASYNCTASK_1130_112_2_18_3_25_45_220 "MMIPIM  ^_^ Begin Write xml logs.\n"
#define PIMDATASYNCTASK_1132_112_2_18_3_25_45_221 "MMIPIM  ^_^ Write xml logs complete.\n"
#define PIMDATASYNCTASK_1135_112_2_18_3_25_45_222 "MMIPIM  ^_^ server more data ok,begin send!time=%d"
#define PIMDATASYNCTASK_1138_112_2_18_3_25_45_223 "MMIPIM  ^_^ server more data send ok!time=%d"
#define PIMDATASYNCTASK_1154_112_2_18_3_25_45_224 "MMIPIM  ^_^ Hold on for 0.2 second!!"
#define PIMLIBMEM_95_112_2_18_3_25_50_225 "MMIPIM chenxiang ptr is not in the link ! pObject = 0x%x"
#define PIMLIBMEM_121_112_2_18_3_25_50_226 "MMIPIM PrintLeakLink %d  ptr: %d  file : %s  line %d"
#define PIMLIBMEM_124_112_2_18_3_25_50_227 "MMIPIM PrintLeakLink total: %d  sml_malloc_num = %d"
#define PIMSMLCORE_425_112_2_18_3_25_53_228 "MMIPIM enter hs_smlStartSync "
#define PIMSMLCORE_774_112_2_18_3_25_54_229 "MMIPIM chenxiang hs_smlTerminateInstance 1"
#define PIMSMLCORE_780_112_2_18_3_25_54_230 "MMIPIM chenxiang hs_smlTerminateInstance 2"
#define PIMSMLCORE_783_112_2_18_3_25_54_231 "MMIPIM chenxiang hs_smlTerminateInstance 3 , rc = 0x%4x"
#define PIMSMLCORE_1451_112_2_18_3_25_55_232 "MMIPIM  $$ final call back called!$$"
#define PIMSYNCMLCOMM_138_112_2_18_3_25_55_233 "MMIPIM  *_*hs_smlLockReadBuffer"
#define PIMSYNCMLCOMM_159_112_2_18_3_25_55_234 "MMIPIM  *_*use the bearer interface to send."
#define PIMSYNCMLCOMM_180_112_2_18_3_25_55_235 "MMIPIM  *_*  hs_syncml_Comm_ReSendData"
#define PIMSYNCMLCOMM_186_112_2_18_3_25_56_236 "MMIPIM  *_*use the bearer interface to send."
#define PIMSYNCMLCOMM_208_112_2_18_3_25_56_237 "MMIPIM  *_*use the bearer interface to send another times"
#define PIMSYNCMLCOMM_222_112_2_18_3_25_56_238 "MMIPIM  *_*Begin receive data!"
#define PIMSYNCMLCOMM_224_112_2_18_3_25_56_239 "MMIPIM  *_*hs_smlLockWriteBuffer"
#define PIMSYNCMLCOMM_231_112_2_18_3_25_56_240 "MMIPIM  *_*use the bearer interface to receive."
#define PIMSYNCMLCOMM_245_112_2_18_3_25_56_241 "MMIPIM  *_*use the bearer interface to receive another times"
#define PIMSYNCMLMGR_182_112_2_18_3_25_56_242 "MMIPIM $$ enter hs_calc_md5_cred "
#define PIMSYNCMLMGR_183_112_2_18_3_25_56_243 "MMIPIM $$ username=%s,pas_word=%s"
#define PIMSYNCMLMGR_193_112_2_18_3_25_56_244 "MMIPIM $$ creddata=%s"
#define PIMSYNCMLMGR_338_112_2_18_3_25_57_245 "MMIPIM $$ hs_syncml_startmessage  anth_type=%d "
#define PIMSYNCMLMGR_374_112_2_18_3_25_57_246 "MMIPIM $$ hs_syncml_startmessage  creddata=%s "
#define PIMSYNCMLMGR_395_112_2_18_3_25_57_247 "MMIPIM $$ hs_syncml_startmessage  creddata=%s "
#define PIMSYNCMLMGR_681_112_2_18_3_25_57_248 "MMIPIM  ^_^ Begin hs_syncml_PutCmd g_pim.m_ua_type=%d"
#define PIMSYNCMLMGR_1001_112_2_18_3_25_58_249 "MMIPIM  ^_^ Begin hs_syncml_PutVCALCmd"
#define PIMSYNCMLMGR_1204_112_2_18_3_25_58_250 "MMIPIM  ^_^ Begin hs_smlPutCmd"
#define PIMSYNCMLMGR_1206_112_2_18_3_25_58_251 "MMIPIM  ^_^ End hs_smlPutCmd"
#define PIMSYNCMLMGR_1365_112_2_18_3_25_59_252 "MMIPIM hs_syncml_AddCmd,now the large object is proc 1 ..."
#define PIMSYNCMLMGR_1387_112_2_18_3_25_59_253 "MMIPIM hs_syncml_AddCmd,the first chunk of the large object, total_len=%d,cur_len=%d"
#define PIMSYNCMLMGR_1410_112_2_18_3_25_59_254 "MMIPIM ,hs_syncml_AddCmd ,first ,content_len=%d"
#define PIMSYNCMLMGR_1425_112_2_18_3_25_59_255 "MMIPIM hs_syncml_AddCmd,the last chunk of the large object, total_len=%d,cur_len=%d"
#define PIMSYNCMLMGR_1433_112_2_18_3_25_59_256 "MMIPIM ,hs_syncml_AddCmd ,last ,content_len=%d"
#define PIMSYNCMLMGR_1444_112_2_18_3_25_59_257 "MMIPIM hs_syncml_AddCmd,the midle chunk of the large object, total_len=%d,cur_len=%d"
#define PIMSYNCMLMGR_1451_112_2_18_3_25_59_258 "MMIPIM ,hs_syncml_AddCmd ,middle ,content_len=%d"
#define PIMSYNCMLMGR_1492_112_2_18_3_25_59_259 "MMIPIM hs_syncml_AddCmd,now the large object is proc 2 ..."
#define PIMSYNCMLMGR_1625_112_2_18_3_25_59_260 "MMIPIM  $$enter hs_syncml_DeleteCmd $$"
#define PIMSYNCMLMGR_1639_112_2_18_3_25_59_261 "MMIPIM  $$ del.itemList=%d"
#define PIMSYNCMLMGR_1870_112_2_18_3_26_0_262 "MMIPIM hs_syncml_ReplaceCmd,now the large object is proc 1 ..."
#define PIMSYNCMLMGR_1892_112_2_18_3_26_0_263 "MMIPIM hs_syncml_ReplaceCmd,the first chunk of the large object, total_len=%d,cur_len=%d"
#define PIMSYNCMLMGR_1904_112_2_18_3_26_0_264 "MMIPIM ,hs_syncml_ReplaceCmd ,first ,content_len=%d"
#define PIMSYNCMLMGR_1919_112_2_18_3_26_0_265 "MMIPIM hs_syncml_ReplaceCmd,the last chunk of the large object, total_len=%d,cur_len=%d"
#define PIMSYNCMLMGR_1926_112_2_18_3_26_0_266 "MMIPIM ,hs_syncml_ReplaceCmd ,last ,content_len=%d"
#define PIMSYNCMLMGR_1937_112_2_18_3_26_0_267 "MMIPIM hs_syncml_ReplaceCmd,the midle chunk of the large object, total_len=%d,cur_len=%d"
#define PIMSYNCMLMGR_1944_112_2_18_3_26_0_268 "MMIPIM ,hs_syncml_ReplaceCmd ,middle ,content_len=%d"
#define PIMSYNCMLMGR_1973_112_2_18_3_26_0_269 "MMIPIM hs_syncml_ReplaceCmd,now the large object is proc 2 ..."
#define PIMSYNCMLMGR_2017_112_2_18_3_26_0_270 "MMIPIM  $$ enter hs_syncml_ReplaceCmd!$$"
#define PIMSYNCMLMGR_2035_112_2_18_3_26_0_271 "MMIPIM  $$ there is no items to be replaced!$$"
#define PIMSYNCMLMGR_2196_112_2_18_3_26_0_272 "MMIPIM  $$ release all instances,instance NO.%d is NULL!$$"
#define PIMSYNCMLMGR_2198_112_2_18_3_26_0_273 "MMIPIM  $$release all instances,instance NO.%d release OK!$$"
#define PIMSYNCMLMGR_2200_112_2_18_3_26_0_274 "MMIPIM  $$release all instances,instance NO.%d release error!$$"
#define PIMSYNCMLMGR_2223_112_2_18_3_26_0_275 "MMIPIM  $$Delete all deleted pb items error!$$"
#define PIMSYNCMLMGR_2255_112_2_18_3_26_1_276 "MMIPIM  $$enter hs_syncml_modify_for_fast_sync $$"
#define PIMSYNCMLMGR_2257_112_2_18_3_26_1_277 "MMIPIM  $$end hs_syncml_DeleteCmd"
#define PIMSYNCMLMGR_2260_112_2_18_3_26_1_278 "MMIPIM  $$delete command error!$$"
#define PIMSYNCMLMGR_2264_112_2_18_3_26_1_279 "MMIPIM  $$begin hs_syncml_ReplaceCmd"
#define PIMSYNCMLMGR_2268_112_2_18_3_26_1_280 "MMIPIM  $$ replace command error!"
#define PIMSYNCMLMGR_2270_112_2_18_3_26_1_281 "MMIPIM  $$end hs_syncml_ReplaceCmd"
#define PIMSYNCMLMGR_2273_112_2_18_3_26_1_282 "MMIPIM  $$begin hs_syncml_ReplaceCmd"
#define PIMSYNCMLMGR_2277_112_2_18_3_26_1_283 "MMIPIM  $$ add command error!$$"
#define PIMWSM_168_112_2_18_3_26_1_284 "MMIPIM chenxiang hs_removeInfo 1"
#define PIMWSM_177_112_2_18_3_26_1_285 "MMIPIM chenxiang hs_removeInfo 2"
#define PIMWSM_188_112_2_18_3_26_1_286 "MMIPIM chenxiang hs_removeInfo 3"
#define PIMWSM_197_112_2_18_3_26_1_287 "MMIPIM chenxiang hs_removeInfo 4"
#define PIMWSM_421_112_2_18_3_26_2_288 "MMIPIM wangxiaolin_mem init buffer memory!addr=0x%x"
#define PIMWSM_485_112_2_18_3_26_2_289 "MMIPIM wangxiaolin  hs_smClose , memH=%d"
#define PIMWSM_489_112_2_18_3_26_2_290 "MMIPIM wangxiaolin_mem  hs_smClose ,free  winH!addr=0x%x "
#define PIMWSM_571_112_2_18_3_26_2_291 "MMIPIM wangxiaolin_mem  hs_smSetSize ,free  winH!addr=0x%x "
#define PIMWSM_894_112_2_18_3_26_2_292 "MMIPIM chenxiang hs_wsmClose 1"
#define PIMWSM_900_112_2_18_3_26_3_293 "MMIPIM chenxiang hs_wsmClose 2"
#define PIMWSM_905_112_2_18_3_26_3_294 "MMIPIM chenxiang hs_wsmClose 3 wsmRet = 0x%4x"
#define PIMWSM_932_112_2_18_3_26_3_295 "MMIPIM chenxiang hs_wsmDestroy 1"
#define PIMWSM_938_112_2_18_3_26_3_296 "MMIPIM chenxiang hs_wsmDestroy 2"
#define PIMWSM_942_112_2_18_3_26_3_297 "MMIPIM chenxiang hs_wsmDestroy 3"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_PIM_TRC)
TRACE_MSG(PIMCALLBACK_HANDLER_836_112_2_18_3_25_24_0,"MMIPIM @@hs_smlPcdata2String size=%d")
TRACE_MSG(PIMCALLBACK_HANDLER_897_112_2_18_3_25_24_1,"MMIPIM @@hs_smlString2unsignedInt =%s")
TRACE_MSG(PIMCALLBACK_HANDLER_951_112_2_18_3_25_24_2,"MMIPIM  @@ leave hs_smlString2unsignedInt =%d")
TRACE_MSG(PIMCALLBACK_HANDLER_1251_112_2_18_3_25_24_3,"MMIPIM @@handle alert@@")
TRACE_MSG(PIMCALLBACK_HANDLER_1259_112_2_18_3_25_24_4,"MMIPIM @@handle alert@@ alert for two way sync")
TRACE_MSG(PIMCALLBACK_HANDLER_1265_112_2_18_3_25_24_5,"MMIPIM @@handle alert@@ alert for slow sync")
TRACE_MSG(PIMCALLBACK_HANDLER_1271_112_2_18_3_25_24_6,"MMIPIM @@handle alert@@alert for more")
TRACE_MSG(PIMCALLBACK_HANDLER_1278_112_2_18_3_25_24_7,"MMIPIM @@handle alert@@ alert for one way sync from client only!")
TRACE_MSG(PIMCALLBACK_HANDLER_1286_112_2_18_3_25_25_8,"MMIPIM @@handle alert@@ alert for refresh sync from client only!")
TRACE_MSG(PIMCALLBACK_HANDLER_1294_112_2_18_3_25_25_9,"MMIPIM @@handle alert@@ alert for one way sync from server only!")
TRACE_MSG(PIMCALLBACK_HANDLER_1302_112_2_18_3_25_25_10,"MMIPIM @@handle alert@@ alert for refresh sync from server only!")
TRACE_MSG(PIMCALLBACK_HANDLER_1385_112_2_18_3_25_25_11,"^@@^MMIPIM hs_myHandleFinal !")
TRACE_MSG(PIMCALLBACK_HANDLER_1663_112_2_18_3_25_25_12,"MMIPIM &&& enter hs_myGetNumberofChanges")
TRACE_MSG(PIMCALLBACK_HANDLER_1693_112_2_18_3_25_25_13,"MMIPIM &&& add_num=%d,del=%d,rep=%d,num=%d")
TRACE_MSG(PIMCALLBACK_HANDLER_1713_112_2_18_3_25_25_14,"MMIPIM &&& enter hs_myGetNumberofAllItem")
TRACE_MSG(PIMCALLBACK_HANDLER_1744_112_2_18_3_25_25_15,"MMIPIM &&&buf_lenth=%d")
TRACE_MSG(PIMCALLBACK_HANDLER_1767_112_2_18_3_25_26_16,"MMIPIM &&& hs_myhttplogs")
TRACE_MSG(PIMCODEC_2147_112_2_18_3_25_30_17,"mmipim hs_devinfEncBlock tag id=%d ")
TRACE_MSG(PIMCODEC_4789_112_2_18_3_25_36_18,"MMIPIM  ----------------------------------------TN_CMDID")
TRACE_MSG(PIMCODEC_4798_112_2_18_3_25_36_19,"MMIPIM  ----------------------------------------TN_MSGREF")
TRACE_MSG(PIMCODEC_4806_112_2_18_3_25_36_20,"MMIPIM  ----------------------------------------TN_CMDREF")
TRACE_MSG(PIMCODEC_4814_112_2_18_3_25_36_21,"MMIPIM  ----------------------------------------TN_CMD")
TRACE_MSG(PIMCODEC_4823_112_2_18_3_25_36_22,"MMIPIM  ----------------------------------------TN_DATA")
TRACE_MSG(PIMCODEC_4832_112_2_18_3_25_36_23,"MMIPIM  ----------------------------------------TN_CHAL")
TRACE_MSG(PIMCODEC_4840_112_2_18_3_25_36_24,"MMIPIM  ----------------------------------------TN_CRED")
TRACE_MSG(PIMCODEC_4850_112_2_18_3_25_36_25,"MMIPIM  ----------------------------------------TN_ITEM")
TRACE_MSG(PIMCODEC_4858_112_2_18_3_25_36_26,"MMIPIM  ----------------------------------------TN_TARGETREF")
TRACE_MSG(PIMCODEC_4866_112_2_18_3_25_36_27,"MMIPIM  ----------------------------------------TN_SOURCEREF")
TRACE_MSG(PIMCODEC_4875_112_2_18_3_25_36_28,"MMIPIM-----------------------------------------hs_smlFreeStatus  11111111 rc = %4x")
TRACE_MSG(PIMCODEC_4881_112_2_18_3_25_36_29,"MMIPIM-----------------------------------------hs_smlFreeStatus  22222222 rc = %4x")
TRACE_MSG(PIMCODEC_4890_112_2_18_3_25_36_30,"MMIPIM-----------------------------------------hs_smlFreeStatus  3333333 rc = %4x")
TRACE_MSG(PIMCOMMHTTP_234_112_2_18_3_25_38_31,"MMIPIM ** pdp has been deactived!")
TRACE_MSG(PIMCOMMHTTP_251_112_2_18_3_25_38_32,"MMIPIM *********read socket select ..... ")
TRACE_MSG(PIMCOMMHTTP_265_112_2_18_3_25_38_33,"MMIPIM **receive data error: Error %d")
TRACE_MSG(PIMCOMMHTTP_279_112_2_18_3_25_38_34,"MMIPIM *********write socket select ..... ")
TRACE_MSG(PIMCOMMHTTP_301_112_2_18_3_25_38_35,"MMIPIM ** send data error: Error %d")
TRACE_MSG(PIMCOMMHTTP_317_112_2_18_3_25_38_36,"MMIPIM ** communication ,no socket is avilable")
TRACE_MSG(PIMCOMMHTTP_345_112_2_18_3_25_38_37,"mmipim create pim_socket")
TRACE_MSG(PIMCOMMHTTP_407_112_2_18_3_25_38_38,"mmipim socket close socket=%d")
TRACE_MSG(PIMCOMMHTTP_443_112_2_18_3_25_39_39,"mmipim socket_connect socket =%d")
TRACE_MSG(PIMCOMMHTTP_499_112_2_18_3_25_39_40,"mmipim socket_recv socket =%d")
TRACE_MSG(PIMCOMMHTTP_527_112_2_18_3_25_39_41,"MMIPIM HS_HTTP_Prepare ")
TRACE_MSG(PIMCOMMHTTP_658_112_2_18_3_25_39_42,"MMIPIM HS_HTTP_Release ")
TRACE_MSG(PIMCOMMHTTP_671_112_2_18_3_25_39_43,"MMIPIM HS_HTTP_Release 1")
TRACE_MSG(PIMCOMMHTTP_677_112_2_18_3_25_39_44,"MMIPIM HS_HTTP_Release 2")
TRACE_MSG(PIMCOMMHTTP_683_112_2_18_3_25_39_45,"MMIPIM HS_HTTP_Release 3")
TRACE_MSG(PIMCOMMHTTP_690_112_2_18_3_25_39_46,"MMIPIM HS_HTTP_Release 4")
TRACE_MSG(PIMCOMMHTTP_696_112_2_18_3_25_39_47,"MMIPIM HS_HTTP_Release 5")
TRACE_MSG(PIMCOMMHTTP_702_112_2_18_3_25_39_48,"MMIPIM HS_HTTP_Release 6")
TRACE_MSG(PIMCOMMHTTP_708_112_2_18_3_25_39_49,"MMIPIM HS_HTTP_Release 7")
TRACE_MSG(PIMCOMMHTTP_714_112_2_18_3_25_39_50,"MMIPIM HS_HTTP_Release 8")
TRACE_MSG(PIMCOMMHTTP_721_112_2_18_3_25_39_51,"MMIPIM HS_HTTP_Release 9")
TRACE_MSG(PIMCOMMHTTP_724_112_2_18_3_25_39_52,"MMIPIM HS_HTTP_Release 10")
TRACE_MSG(PIMCOMMHTTP_767_112_2_18_3_25_39_53,"MMIPIM **Socket init fail!")
TRACE_MSG(PIMCOMMHTTP_774_112_2_18_3_25_39_54,"MMIPIM ** Connect Successful:")
TRACE_MSG(PIMCOMMHTTP_783_112_2_18_3_25_39_55,"MMIPIM ** Connect Failed: Error %d")
TRACE_MSG(PIMCOMMHTTP_811_112_2_18_3_25_39_56,"MMIPIM **HS_HTTP_SendData")
TRACE_MSG(PIMCOMMHTTP_817_112_2_18_3_25_39_57,"MMIPIM ** pdp has been deactived!")
TRACE_MSG(PIMCOMMHTTP_834_112_2_18_3_25_39_58,"MMIPIM ** HS_HTTP_SendData SOCKET_STEP_CONNECT")
TRACE_MSG(PIMCOMMHTTP_837_112_2_18_3_25_39_59,"MMIPIM **Begin open socket\n")
TRACE_MSG(PIMCOMMHTTP_845_112_2_18_3_25_39_60,"MMIPIM ** error! proxy address and port convert error.")
TRACE_MSG(PIMCOMMHTTP_849_112_2_18_3_25_39_61,"MMIPIM **=======Connect %s:%s=========")
TRACE_MSG(PIMCOMMHTTP_856_112_2_18_3_25_39_62,"MMIPIM ** error! server address and port convert error.")
TRACE_MSG(PIMCOMMHTTP_860_112_2_18_3_25_39_63,"MMIPIM **=======Connect %s:%s=========")
TRACE_MSG(PIMCOMMHTTP_876_112_2_18_3_25_39_64,"MMIPIM ** Connect Successful:")
TRACE_MSG(PIMCOMMHTTP_888_112_2_18_3_25_39_65,"MMIPIM ** HTTP protocol error code=%d")
TRACE_MSG(PIMCOMMHTTP_891_112_2_18_3_25_39_66,"MMIPIM ** Connect has been done:")
TRACE_MSG(PIMCOMMHTTP_907_112_2_18_3_25_39_67,"MMIPIM **Socket init fail!")
TRACE_MSG(PIMCOMMHTTP_917_112_2_18_3_25_40_68,"MMIPIM ** socket already connect!")
TRACE_MSG(PIMCOMMHTTP_932_112_2_18_3_25_40_69,"MMIPIM **SOCKET_STEP_1")
TRACE_MSG(PIMCOMMHTTP_938_112_2_18_3_25_40_70,"MMIPIM ** send length=%d")
TRACE_MSG(PIMCOMMHTTP_943_112_2_18_3_25_40_71,"MMIPIM  ^_^ Begin Write xml logs.\n")
TRACE_MSG(PIMCOMMHTTP_946_112_2_18_3_25_40_72,"MMIPIM  ^_^ Write xml logs complete.\n")
TRACE_MSG(PIMCOMMHTTP_957_112_2_18_3_25_40_73,"MMIPIM ** send next packet")
TRACE_MSG(PIMCOMMHTTP_962_112_2_18_3_25_40_74,"MMIPIM ** writing http header complete...")
TRACE_MSG(PIMCOMMHTTP_969_112_2_18_3_25_40_75,"MMIPIM ** post data sent event")
TRACE_MSG(PIMCOMMHTTP_982_112_2_18_3_25_40_76,"MMIPIM ** HTTP protocol error code=%d")
TRACE_MSG(PIMCOMMHTTP_987_112_2_18_3_25_40_77,"MMIPIM ** writing header...")
TRACE_MSG(PIMCOMMHTTP_994_112_2_18_3_25_40_78,"MMIPIM ** Write Data Failed")
TRACE_MSG(PIMCOMMHTTP_1006_112_2_18_3_25_40_79,"MMIPIM **HS_HTTP_SendData SOCKET_STEP_2")
TRACE_MSG(PIMCOMMHTTP_1011_112_2_18_3_25_40_80,"MMIPIM ** send length=%d")
TRACE_MSG(PIMCOMMHTTP_1026_112_2_18_3_25_40_81,"MMIPIM ** send next packet")
TRACE_MSG(PIMCOMMHTTP_1034_112_2_18_3_25_40_82,"MMIPIM ** writing syncml body data complete...")
TRACE_MSG(PIMCOMMHTTP_1041_112_2_18_3_25_40_83,"MMIPIM **post data sent event")
TRACE_MSG(PIMCOMMHTTP_1054_112_2_18_3_25_40_84,"MMIPIM ** HTTP protocol error code=%d")
TRACE_MSG(PIMCOMMHTTP_1058_112_2_18_3_25_40_85,"MMIPIM ** writing contents...")
TRACE_MSG(PIMCOMMHTTP_1065_112_2_18_3_25_40_86,"MMIPIM ** Write Failed: Error %d\n")
TRACE_MSG(PIMCOMMHTTP_1079_112_2_18_3_25_40_87,"MMIPIM **HS_HTTP_SendData SOCKET_STEP_3")
TRACE_MSG(PIMCOMMHTTP_1082_112_2_18_3_25_40_88,"MMIPIM **Send all data ok!")
TRACE_MSG(PIMCOMMHTTP_1096_112_2_18_3_25_40_89,"MMIPIM **HS_HTTP_SendData ,step type error!")
TRACE_MSG(PIMCOMMHTTP_1131_112_2_18_3_25_40_90,"MMIPIM ** socket=-1")
TRACE_MSG(PIMCOMMHTTP_1137_112_2_18_3_25_40_91,"MMIPIM ** pdp has been deactived!")
TRACE_MSG(PIMCOMMHTTP_1148_112_2_18_3_25_40_92,"MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_1")
TRACE_MSG(PIMCOMMHTTP_1151_112_2_18_3_25_40_93,"MMIPIM chenxiang !!!!!!!hs_totalrec == %d")
TRACE_MSG(PIMCOMMHTTP_1154_112_2_18_3_25_40_94,"MMIPIM ** http contents received is %d")
TRACE_MSG(PIMCOMMHTTP_1160_112_2_18_3_25_40_95,"MMIPIM ** Begin read  http head data !\n")
TRACE_MSG(PIMCOMMHTTP_1162_112_2_18_3_25_40_96,"MMIPIM ** http head data  read complete!\n")
TRACE_MSG(PIMCOMMHTTP_1170_112_2_18_3_25_40_97,"MMIPIM ** All packets less than HS_HEADER_BUFFER_LEN bytes!")
TRACE_MSG(PIMCOMMHTTP_1180_112_2_18_3_25_40_98,"MMIPIM ---------------(HTTP 500)---------------------")
TRACE_MSG(PIMCOMMHTTP_1194_112_2_18_3_25_40_99,"MMIPIM **socket connection closed.")
TRACE_MSG(PIMCOMMHTTP_1204_112_2_18_3_25_40_100,"MMIPIM ** HTTP protocol error!return status is not 200 ok!...")
TRACE_MSG(PIMCOMMHTTP_1208_112_2_18_3_25_40_101,"MMIPIM ** http headersize=%d")
TRACE_MSG(PIMCOMMHTTP_1209_112_2_18_3_25_40_102,"MMIPIM ** http contents size=%d")
TRACE_MSG(PIMCOMMHTTP_1218_112_2_18_3_25_40_103,"MMIPIM ** reading syncml contents complete...")
TRACE_MSG(PIMCOMMHTTP_1230_112_2_18_3_25_40_104,"MMIPIM ** Reading http header less than HS_HEADER_BUFFER_LEN")
TRACE_MSG(PIMCOMMHTTP_1254_112_2_18_3_25_40_105,"MMIPIM ** HTTP protocol error code=%d")
TRACE_MSG(PIMCOMMHTTP_1258_112_2_18_3_25_40_106,"MMIPIM ** Reading syncml body data ...")
TRACE_MSG(PIMCOMMHTTP_1266_112_2_18_3_25_40_107,"MMIPIM -------------(errno == EPIPE)---------------------")
TRACE_MSG(PIMCOMMHTTP_1280_112_2_18_3_25_40_108,"MMIPIM **socket connection closed.")
TRACE_MSG(PIMCOMMHTTP_1288_112_2_18_3_25_40_109,"MMIPIM ** Read Failed")
TRACE_MSG(PIMCOMMHTTP_1302_112_2_18_3_25_40_110,"MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_2")
TRACE_MSG(PIMCOMMHTTP_1306_112_2_18_3_25_40_111,"MMIPIM ** http contents received is %d")
TRACE_MSG(PIMCOMMHTTP_1317_112_2_18_3_25_40_112,"MMIPIM ** content_length in header is %d")
TRACE_MSG(PIMCOMMHTTP_1318_112_2_18_3_25_40_113,"MMIPIM ** http contents received is %d")
TRACE_MSG(PIMCOMMHTTP_1325_112_2_18_3_25_40_114,"MMIPIM ** reading syncml contents complete...")
TRACE_MSG(PIMCOMMHTTP_1340_112_2_18_3_25_40_115,"MMIPIM ** receive next packet")
TRACE_MSG(PIMCOMMHTTP_1346_112_2_18_3_25_40_116,"MMIPIM ** HTTP protocol error!receive count not equal to content length")
TRACE_MSG(PIMCOMMHTTP_1359_112_2_18_3_25_40_117,"MMIPIM ** HTTP protocol error code=%d")
TRACE_MSG(PIMCOMMHTTP_1363_112_2_18_3_25_40_118,"MMIPIM ** Reading contents...")
TRACE_MSG(PIMCOMMHTTP_1370_112_2_18_3_25_40_119,"MMIPIM -------------(errno == EPIPE)---------------------")
TRACE_MSG(PIMCOMMHTTP_1383_112_2_18_3_25_41_120,"MMIPIM **socket connection closed.")
TRACE_MSG(PIMCOMMHTTP_1390_112_2_18_3_25_41_121,"MMIPIM ** Read Failed: Error %d\n")
TRACE_MSG(PIMCOMMHTTP_1404_112_2_18_3_25_41_122,"MMIPIM ** HS_HTTP_RecData   SOCKET_STEP_3")
TRACE_MSG(PIMCOMMHTTP_1406_112_2_18_3_25_41_123,"MMIPIM **Receive all data ok!")
TRACE_MSG(PIMCOMMHTTP_1446_112_2_18_3_25_41_124,"MMIPIM **socket connection closed.")
TRACE_MSG(PIMCOMMHTTP_1556_112_2_18_3_25_41_125,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMCOMMHTTP_1574_112_2_18_3_25_41_126,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMCOMMHTTP_1581_112_2_18_3_25_41_127,"MMIPIM chenxiang !!!!!!! hs_http_globle->usedsize == %d")
TRACE_MSG(PIMCOMMHTTP_1584_112_2_18_3_25_41_128,"MMIPIM chenxiang !!!!!!! ------------headerbuffer------start------")
TRACE_MSG(PIMCOMMHTTP_1586_112_2_18_3_25_41_129,"%s")
TRACE_MSG(PIMCOMMHTTP_1587_112_2_18_3_25_41_130,"MMIPIM chenxiang !!!!!!! ------------headerbuffer-------end-----")
TRACE_MSG(PIMCOMMHTTP_1611_112_2_18_3_25_41_131,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMCOMMHTTP_1621_112_2_18_3_25_41_132,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMCOMMHTTP_1631_112_2_18_3_25_41_133,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMCOMMHTTP_1683_112_2_18_3_25_41_134,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMCOMMHTTP_1695_112_2_18_3_25_41_135,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMCOMMHTTP_1729_112_2_18_3_25_41_136,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMCOMMHTTP_1741_112_2_18_3_25_41_137,"MMIPIM chenxiang !!!!!!! headerbuffer over flow !!! FILE: %s , LINE: %s")
TRACE_MSG(PIMDATASYNCTASK_144_112_2_18_3_25_43_138,"MMIPIM  ^_^ enter syncml hs_Data_Sync_terminate!\n")
TRACE_MSG(PIMDATASYNCTASK_177_112_2_18_3_25_43_139,"MMIPIM  ^_^ syncml hs_Data_Sync_terminate! free last_anchor\n")
TRACE_MSG(PIMDATASYNCTASK_183_112_2_18_3_25_43_140,"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free next_anchor\n")
TRACE_MSG(PIMDATASYNCTASK_191_112_2_18_3_25_43_141,"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free proxy_ip\n")
TRACE_MSG(PIMDATASYNCTASK_261_112_2_18_3_25_43_142,"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free syncml_pim_nextnonce\n")
TRACE_MSG(PIMDATASYNCTASK_265_112_2_18_3_25_43_143,"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free hs_task_relay_info\n")
TRACE_MSG(PIMDATASYNCTASK_274_112_2_18_3_25_43_144,"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free hs_status_cmd_que\n")
TRACE_MSG(PIMDATASYNCTASK_280_112_2_18_3_25_43_145,"MMIPIM  ^_^ syncml hs_Data_Sync_terminate!free all instance\n")
TRACE_MSG(PIMDATASYNCTASK_298_112_2_18_3_25_43_146,"MMIPIM  ^_^leave syncml hs_Data_Sync_terminate!\n")
TRACE_MSG(PIMDATASYNCTASK_317_112_2_18_3_25_43_147,"MMIPIM  ^_^ Begin client init!\n")
TRACE_MSG(PIMDATASYNCTASK_325_112_2_18_3_25_43_148,"MMIPIM  ^_^ Begin Server init!\n")
TRACE_MSG(PIMDATASYNCTASK_333_112_2_18_3_25_43_149,"MMIPIM  ^_^ Begin client modify!\n")
TRACE_MSG(PIMDATASYNCTASK_342_112_2_18_3_25_43_150,"MMIPIM  ^_^ Begin Server modify!\n")
TRACE_MSG(PIMDATASYNCTASK_351_112_2_18_3_25_43_151,"MMIPIM  ^_^ Begin client status and map send!\n")
TRACE_MSG(PIMDATASYNCTASK_359_112_2_18_3_25_43_152,"MMIPIM  ^_^ Begin server map ack!\n")
TRACE_MSG(PIMDATASYNCTASK_369_112_2_18_3_25_43_153,"MMIPIM  ^_^ All sync operation is over,then release the resources!\n")
TRACE_MSG(PIMDATASYNCTASK_371_112_2_18_3_25_43_154,"MMIPIM  ^_^ synchornization is ok! save the last anchor!")
TRACE_MSG(PIMDATASYNCTASK_385_112_2_18_3_25_43_155,"MMIPIM  ^_^ more data,send alert 222 for more!\n")
TRACE_MSG(PIMDATASYNCTASK_393_112_2_18_3_25_43_156,"MMIPIM  *_* STEP_SYNC_ERROR!\n")
TRACE_MSG(PIMDATASYNCTASK_429_112_2_18_3_25_43_157,"MMIPIM  ^_^ Begin hs_got_free_id workspaceid= %d")
TRACE_MSG(PIMDATASYNCTASK_447_112_2_18_3_25_43_158,"MMIPIM  ^_^ Begin hs_step_client_init_sync_process,time=%d")
TRACE_MSG(PIMDATASYNCTASK_455_112_2_18_3_25_43_159,"MMIPIM  ^_^ Begin step_client_init_sync_process1,sync_mode=%d")
TRACE_MSG(PIMDATASYNCTASK_457_112_2_18_3_25_43_160,"MMIPIM  ^_^ Begin step_client_init_sync_process1")
TRACE_MSG(PIMDATASYNCTASK_470_112_2_18_3_25_43_161,"MMIPIM  ^_^ Begin step_client_init_sync_process2")
TRACE_MSG(PIMDATASYNCTASK_503_112_2_18_3_25_44_162,"MMIPIM  ^_^ Begin step_client_init_sync_process3")
TRACE_MSG(PIMDATASYNCTASK_532_112_2_18_3_25_44_163,"MMIPIM  ^_^ Begin Write xml logs.\n")
TRACE_MSG(PIMDATASYNCTASK_534_112_2_18_3_25_44_164,"MMIPIM  ^_^ xml logs write complete.\n")
TRACE_MSG(PIMDATASYNCTASK_537_112_2_18_3_25_44_165,"MMIPIM  ^_^ client init data ok,begin send!time=%d \n")
TRACE_MSG(PIMDATASYNCTASK_540_112_2_18_3_25_44_166,"MMIPIM  ^_^ client init data send ok!time=%d \n")
TRACE_MSG(PIMDATASYNCTASK_562_112_2_18_3_25_44_167,"MMIPIM  ^_^ Begin hs_step_server_init_sync_process 1,time=%d")
TRACE_MSG(PIMDATASYNCTASK_567_112_2_18_3_25_44_168,"MMIPIM  ^_^ prepare ok,begin receive!\n")
TRACE_MSG(PIMDATASYNCTASK_569_112_2_18_3_25_44_169,"MMIPIM  ^_^ server init receive ok!\n")
TRACE_MSG(PIMDATASYNCTASK_576_112_2_18_3_25_44_170,"MMIPIM  ^_^ Begin hs_step_server_init_sync_process 2 ,time=%d")
TRACE_MSG(PIMDATASYNCTASK_579_112_2_18_3_25_44_171,"MMIPIM  ^_^ Begin Write xml logs.\n")
TRACE_MSG(PIMDATASYNCTASK_581_112_2_18_3_25_44_172,"MMIPIM  ^_^ xml logs write complete.\n")
TRACE_MSG(PIMDATASYNCTASK_583_112_2_18_3_25_44_173,"MMIPIM  ^_^ analysis recevie data.\n")
TRACE_MSG(PIMDATASYNCTASK_585_112_2_18_3_25_44_174,"MMIPIM  ^_^ analysis server init complete.\n")
TRACE_MSG(PIMDATASYNCTASK_649_112_2_18_3_25_44_175,"MMIPIM  ^_^ Begin hs_step_client_modi_process,time=%d")
TRACE_MSG(PIMDATASYNCTASK_650_112_2_18_3_25_44_176,"MMIPIM  ^_^ enter hs_step_client_modi_process,synctype=%d")
TRACE_MSG(PIMDATASYNCTASK_664_112_2_18_3_25_44_177,"MMIPIM  ^_^ prepare to send cache map.....")
TRACE_MSG(PIMDATASYNCTASK_672_112_2_18_3_25_44_178,"MMIPIM  ^_^ workspaceid=%d,authored=%d")
TRACE_MSG(PIMDATASYNCTASK_685_112_2_18_3_25_44_179,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_S2WAY\n")
TRACE_MSG(PIMDATASYNCTASK_703_112_2_18_3_25_44_180,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFCLI \n")
TRACE_MSG(PIMDATASYNCTASK_711_112_2_18_3_25_44_181,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFCLI\n")
TRACE_MSG(PIMDATASYNCTASK_721_112_2_18_3_25_44_182,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFSER\n")
TRACE_MSG(PIMDATASYNCTASK_728_112_2_18_3_25_44_183,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFSER\n")
TRACE_MSG(PIMDATASYNCTASK_744_112_2_18_3_25_44_184,"MMIPIM  ^_^add_is_final=%d, delete_is_final=%d,replace_is_final=%d")
TRACE_MSG(PIMDATASYNCTASK_766_112_2_18_3_25_44_185,"MMIPIM  ^_^ Begin Write xml logs.\n")
TRACE_MSG(PIMDATASYNCTASK_768_112_2_18_3_25_44_186,"MMIPIM  ^_^ Write xml logs complete.\n")
TRACE_MSG(PIMDATASYNCTASK_771_112_2_18_3_25_44_187,"MMIPIM  ^_^ client modi data ok,begin send!time=%d")
TRACE_MSG(PIMDATASYNCTASK_774_112_2_18_3_25_44_188,"MMIPIM  ^_^ client modi data send ok!\n")
TRACE_MSG(PIMDATASYNCTASK_792_112_2_18_3_25_44_189,"@@step_server_conf_modi_processf  ,start time=%d")
TRACE_MSG(PIMDATASYNCTASK_798_112_2_18_3_25_44_190,"MMIPIM  ^_^ Begin hs_step_server_conf_modi_process 1,time=%d")
TRACE_MSG(PIMDATASYNCTASK_803_112_2_18_3_25_44_191,"MMIPIM  ^_^ prepare ok,begin receive!\n")
TRACE_MSG(PIMDATASYNCTASK_805_112_2_18_3_25_44_192,"MMIPIM  ^_^ server init receive ok!\n")
TRACE_MSG(PIMDATASYNCTASK_812_112_2_18_3_25_44_193,"MMIPIM  ^_^ Begin hs_step_server_conf_modi_process 2,time=%d")
TRACE_MSG(PIMDATASYNCTASK_816_112_2_18_3_25_44_194,"MMIPIM  ^_^ Begin Write xml logs.\n")
TRACE_MSG(PIMDATASYNCTASK_818_112_2_18_3_25_44_195,"MMIPIM  ^_^ xml logs write complete.\n")
TRACE_MSG(PIMDATASYNCTASK_821_112_2_18_3_25_44_196,"MMIPIM  ^_^ analysis recevie data.\n")
TRACE_MSG(PIMDATASYNCTASK_823_112_2_18_3_25_44_197,"MMIPIM  ^_^ analysis server conf complete.\n")
TRACE_MSG(PIMDATASYNCTASK_859_112_2_18_3_25_44_198,"@@step_server_conf_modi_processf  ,end time=%d")
TRACE_MSG(PIMDATASYNCTASK_881_112_2_18_3_25_44_199,"MMIPIM  ^_^ Begin hs_step_client_conf_map_process,time=%d")
TRACE_MSG(PIMDATASYNCTASK_912_112_2_18_3_25_45_200,"MMIPIM  ^_^ Begin Write xml logs.\n")
TRACE_MSG(PIMDATASYNCTASK_916_112_2_18_3_25_45_201,"MMIPIM  ^_^ Write xml logs complete.\n")
TRACE_MSG(PIMDATASYNCTASK_918_112_2_18_3_25_45_202,"MMIPIM  ^_^ client conf data ok,begin send!time=%d")
TRACE_MSG(PIMDATASYNCTASK_921_112_2_18_3_25_45_203,"MMIPIM  ^_^ client conf data send ok!\n")
TRACE_MSG(PIMDATASYNCTASK_945_112_2_18_3_25_45_204,"MMIPIM  ^_^ Begin hs_step_server_mapack_process  1,time=%d")
TRACE_MSG(PIMDATASYNCTASK_950_112_2_18_3_25_45_205,"MMIPIM  ^_^ prepare ok,begin receive!\n")
TRACE_MSG(PIMDATASYNCTASK_952_112_2_18_3_25_45_206,"MMIPIM  ^_^ server map ack receive ok!\n")
TRACE_MSG(PIMDATASYNCTASK_959_112_2_18_3_25_45_207,"MMIPIM  ^_^ Begin hs_step_server_mapack_process  2,time=%d")
TRACE_MSG(PIMDATASYNCTASK_970_112_2_18_3_25_45_208,"MMIPIM  ^_^ Begin Write xml logs.\n")
TRACE_MSG(PIMDATASYNCTASK_972_112_2_18_3_25_45_209,"MMIPIM  ^_^ xml logs write complete.\n")
TRACE_MSG(PIMDATASYNCTASK_975_112_2_18_3_25_45_210,"MMIPIM  ^_^ analysis recevie data.\n")
TRACE_MSG(PIMDATASYNCTASK_977_112_2_18_3_25_45_211,"MMIPIM  ^_^ analysis server map ack complete.\n")
TRACE_MSG(PIMDATASYNCTASK_1018_112_2_18_3_25_45_212,"MMIPIM  ^_^ Begin hs_step_server_more,time=%d")
TRACE_MSG(PIMDATASYNCTASK_1051_112_2_18_3_25_45_213,"MMIPIM  ^_^ send cache map another times.....")
TRACE_MSG(PIMDATASYNCTASK_1058_112_2_18_3_25_45_214,"MMIPIM  ^_^ workspaceid=%d,authored=%d")
TRACE_MSG(PIMDATASYNCTASK_1071_112_2_18_3_25_45_215,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_S2WAY\n")
TRACE_MSG(PIMDATASYNCTASK_1081_112_2_18_3_25_45_216,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFCLI\n")
TRACE_MSG(PIMDATASYNCTASK_1086_112_2_18_3_25_45_217,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFCLI\n")
TRACE_MSG(PIMDATASYNCTASK_1091_112_2_18_3_25_45_218,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_1WAYFSER\n")
TRACE_MSG(PIMDATASYNCTASK_1095_112_2_18_3_25_45_219,"MMIPIM  ^_^ Begin client modify,SYNC_TYPE_RFSER\n")
TRACE_MSG(PIMDATASYNCTASK_1130_112_2_18_3_25_45_220,"MMIPIM  ^_^ Begin Write xml logs.\n")
TRACE_MSG(PIMDATASYNCTASK_1132_112_2_18_3_25_45_221,"MMIPIM  ^_^ Write xml logs complete.\n")
TRACE_MSG(PIMDATASYNCTASK_1135_112_2_18_3_25_45_222,"MMIPIM  ^_^ server more data ok,begin send!time=%d")
TRACE_MSG(PIMDATASYNCTASK_1138_112_2_18_3_25_45_223,"MMIPIM  ^_^ server more data send ok!time=%d")
TRACE_MSG(PIMDATASYNCTASK_1154_112_2_18_3_25_45_224,"MMIPIM  ^_^ Hold on for 0.2 second!!")
TRACE_MSG(PIMLIBMEM_95_112_2_18_3_25_50_225,"MMIPIM chenxiang ptr is not in the link ! pObject = 0x%x")
TRACE_MSG(PIMLIBMEM_121_112_2_18_3_25_50_226,"MMIPIM PrintLeakLink %d  ptr: %d  file : %s  line %d")
TRACE_MSG(PIMLIBMEM_124_112_2_18_3_25_50_227,"MMIPIM PrintLeakLink total: %d  sml_malloc_num = %d")
TRACE_MSG(PIMSMLCORE_425_112_2_18_3_25_53_228,"MMIPIM enter hs_smlStartSync ")
TRACE_MSG(PIMSMLCORE_774_112_2_18_3_25_54_229,"MMIPIM chenxiang hs_smlTerminateInstance 1")
TRACE_MSG(PIMSMLCORE_780_112_2_18_3_25_54_230,"MMIPIM chenxiang hs_smlTerminateInstance 2")
TRACE_MSG(PIMSMLCORE_783_112_2_18_3_25_54_231,"MMIPIM chenxiang hs_smlTerminateInstance 3 , rc = 0x%4x")
TRACE_MSG(PIMSMLCORE_1451_112_2_18_3_25_55_232,"MMIPIM  $$ final call back called!$$")
TRACE_MSG(PIMSYNCMLCOMM_138_112_2_18_3_25_55_233,"MMIPIM  *_*hs_smlLockReadBuffer")
TRACE_MSG(PIMSYNCMLCOMM_159_112_2_18_3_25_55_234,"MMIPIM  *_*use the bearer interface to send.")
TRACE_MSG(PIMSYNCMLCOMM_180_112_2_18_3_25_55_235,"MMIPIM  *_*  hs_syncml_Comm_ReSendData")
TRACE_MSG(PIMSYNCMLCOMM_186_112_2_18_3_25_56_236,"MMIPIM  *_*use the bearer interface to send.")
TRACE_MSG(PIMSYNCMLCOMM_208_112_2_18_3_25_56_237,"MMIPIM  *_*use the bearer interface to send another times")
TRACE_MSG(PIMSYNCMLCOMM_222_112_2_18_3_25_56_238,"MMIPIM  *_*Begin receive data!")
TRACE_MSG(PIMSYNCMLCOMM_224_112_2_18_3_25_56_239,"MMIPIM  *_*hs_smlLockWriteBuffer")
TRACE_MSG(PIMSYNCMLCOMM_231_112_2_18_3_25_56_240,"MMIPIM  *_*use the bearer interface to receive.")
TRACE_MSG(PIMSYNCMLCOMM_245_112_2_18_3_25_56_241,"MMIPIM  *_*use the bearer interface to receive another times")
TRACE_MSG(PIMSYNCMLMGR_182_112_2_18_3_25_56_242,"MMIPIM $$ enter hs_calc_md5_cred ")
TRACE_MSG(PIMSYNCMLMGR_183_112_2_18_3_25_56_243,"MMIPIM $$ username=%s,pas_word=%s")
TRACE_MSG(PIMSYNCMLMGR_193_112_2_18_3_25_56_244,"MMIPIM $$ creddata=%s")
TRACE_MSG(PIMSYNCMLMGR_338_112_2_18_3_25_57_245,"MMIPIM $$ hs_syncml_startmessage  anth_type=%d ")
TRACE_MSG(PIMSYNCMLMGR_374_112_2_18_3_25_57_246,"MMIPIM $$ hs_syncml_startmessage  creddata=%s ")
TRACE_MSG(PIMSYNCMLMGR_395_112_2_18_3_25_57_247,"MMIPIM $$ hs_syncml_startmessage  creddata=%s ")
TRACE_MSG(PIMSYNCMLMGR_681_112_2_18_3_25_57_248,"MMIPIM  ^_^ Begin hs_syncml_PutCmd g_pim.m_ua_type=%d")
TRACE_MSG(PIMSYNCMLMGR_1001_112_2_18_3_25_58_249,"MMIPIM  ^_^ Begin hs_syncml_PutVCALCmd")
TRACE_MSG(PIMSYNCMLMGR_1204_112_2_18_3_25_58_250,"MMIPIM  ^_^ Begin hs_smlPutCmd")
TRACE_MSG(PIMSYNCMLMGR_1206_112_2_18_3_25_58_251,"MMIPIM  ^_^ End hs_smlPutCmd")
TRACE_MSG(PIMSYNCMLMGR_1365_112_2_18_3_25_59_252,"MMIPIM hs_syncml_AddCmd,now the large object is proc 1 ...")
TRACE_MSG(PIMSYNCMLMGR_1387_112_2_18_3_25_59_253,"MMIPIM hs_syncml_AddCmd,the first chunk of the large object, total_len=%d,cur_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1410_112_2_18_3_25_59_254,"MMIPIM ,hs_syncml_AddCmd ,first ,content_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1425_112_2_18_3_25_59_255,"MMIPIM hs_syncml_AddCmd,the last chunk of the large object, total_len=%d,cur_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1433_112_2_18_3_25_59_256,"MMIPIM ,hs_syncml_AddCmd ,last ,content_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1444_112_2_18_3_25_59_257,"MMIPIM hs_syncml_AddCmd,the midle chunk of the large object, total_len=%d,cur_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1451_112_2_18_3_25_59_258,"MMIPIM ,hs_syncml_AddCmd ,middle ,content_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1492_112_2_18_3_25_59_259,"MMIPIM hs_syncml_AddCmd,now the large object is proc 2 ...")
TRACE_MSG(PIMSYNCMLMGR_1625_112_2_18_3_25_59_260,"MMIPIM  $$enter hs_syncml_DeleteCmd $$")
TRACE_MSG(PIMSYNCMLMGR_1639_112_2_18_3_25_59_261,"MMIPIM  $$ del.itemList=%d")
TRACE_MSG(PIMSYNCMLMGR_1870_112_2_18_3_26_0_262,"MMIPIM hs_syncml_ReplaceCmd,now the large object is proc 1 ...")
TRACE_MSG(PIMSYNCMLMGR_1892_112_2_18_3_26_0_263,"MMIPIM hs_syncml_ReplaceCmd,the first chunk of the large object, total_len=%d,cur_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1904_112_2_18_3_26_0_264,"MMIPIM ,hs_syncml_ReplaceCmd ,first ,content_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1919_112_2_18_3_26_0_265,"MMIPIM hs_syncml_ReplaceCmd,the last chunk of the large object, total_len=%d,cur_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1926_112_2_18_3_26_0_266,"MMIPIM ,hs_syncml_ReplaceCmd ,last ,content_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1937_112_2_18_3_26_0_267,"MMIPIM hs_syncml_ReplaceCmd,the midle chunk of the large object, total_len=%d,cur_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1944_112_2_18_3_26_0_268,"MMIPIM ,hs_syncml_ReplaceCmd ,middle ,content_len=%d")
TRACE_MSG(PIMSYNCMLMGR_1973_112_2_18_3_26_0_269,"MMIPIM hs_syncml_ReplaceCmd,now the large object is proc 2 ...")
TRACE_MSG(PIMSYNCMLMGR_2017_112_2_18_3_26_0_270,"MMIPIM  $$ enter hs_syncml_ReplaceCmd!$$")
TRACE_MSG(PIMSYNCMLMGR_2035_112_2_18_3_26_0_271,"MMIPIM  $$ there is no items to be replaced!$$")
TRACE_MSG(PIMSYNCMLMGR_2196_112_2_18_3_26_0_272,"MMIPIM  $$ release all instances,instance NO.%d is NULL!$$")
TRACE_MSG(PIMSYNCMLMGR_2198_112_2_18_3_26_0_273,"MMIPIM  $$release all instances,instance NO.%d release OK!$$")
TRACE_MSG(PIMSYNCMLMGR_2200_112_2_18_3_26_0_274,"MMIPIM  $$release all instances,instance NO.%d release error!$$")
TRACE_MSG(PIMSYNCMLMGR_2223_112_2_18_3_26_0_275,"MMIPIM  $$Delete all deleted pb items error!$$")
TRACE_MSG(PIMSYNCMLMGR_2255_112_2_18_3_26_1_276,"MMIPIM  $$enter hs_syncml_modify_for_fast_sync $$")
TRACE_MSG(PIMSYNCMLMGR_2257_112_2_18_3_26_1_277,"MMIPIM  $$end hs_syncml_DeleteCmd")
TRACE_MSG(PIMSYNCMLMGR_2260_112_2_18_3_26_1_278,"MMIPIM  $$delete command error!$$")
TRACE_MSG(PIMSYNCMLMGR_2264_112_2_18_3_26_1_279,"MMIPIM  $$begin hs_syncml_ReplaceCmd")
TRACE_MSG(PIMSYNCMLMGR_2268_112_2_18_3_26_1_280,"MMIPIM  $$ replace command error!")
TRACE_MSG(PIMSYNCMLMGR_2270_112_2_18_3_26_1_281,"MMIPIM  $$end hs_syncml_ReplaceCmd")
TRACE_MSG(PIMSYNCMLMGR_2273_112_2_18_3_26_1_282,"MMIPIM  $$begin hs_syncml_ReplaceCmd")
TRACE_MSG(PIMSYNCMLMGR_2277_112_2_18_3_26_1_283,"MMIPIM  $$ add command error!$$")
TRACE_MSG(PIMWSM_168_112_2_18_3_26_1_284,"MMIPIM chenxiang hs_removeInfo 1")
TRACE_MSG(PIMWSM_177_112_2_18_3_26_1_285,"MMIPIM chenxiang hs_removeInfo 2")
TRACE_MSG(PIMWSM_188_112_2_18_3_26_1_286,"MMIPIM chenxiang hs_removeInfo 3")
TRACE_MSG(PIMWSM_197_112_2_18_3_26_1_287,"MMIPIM chenxiang hs_removeInfo 4")
TRACE_MSG(PIMWSM_421_112_2_18_3_26_2_288,"MMIPIM wangxiaolin_mem init buffer memory!addr=0x%x")
TRACE_MSG(PIMWSM_485_112_2_18_3_26_2_289,"MMIPIM wangxiaolin  hs_smClose , memH=%d")
TRACE_MSG(PIMWSM_489_112_2_18_3_26_2_290,"MMIPIM wangxiaolin_mem  hs_smClose ,free  winH!addr=0x%x ")
TRACE_MSG(PIMWSM_571_112_2_18_3_26_2_291,"MMIPIM wangxiaolin_mem  hs_smSetSize ,free  winH!addr=0x%x ")
TRACE_MSG(PIMWSM_894_112_2_18_3_26_2_292,"MMIPIM chenxiang hs_wsmClose 1")
TRACE_MSG(PIMWSM_900_112_2_18_3_26_3_293,"MMIPIM chenxiang hs_wsmClose 2")
TRACE_MSG(PIMWSM_905_112_2_18_3_26_3_294,"MMIPIM chenxiang hs_wsmClose 3 wsmRet = 0x%4x")
TRACE_MSG(PIMWSM_932_112_2_18_3_26_3_295,"MMIPIM chenxiang hs_wsmDestroy 1")
TRACE_MSG(PIMWSM_938_112_2_18_3_26_3_296,"MMIPIM chenxiang hs_wsmDestroy 2")
TRACE_MSG(PIMWSM_942_112_2_18_3_26_3_297,"MMIPIM chenxiang hs_wsmDestroy 3")
END_TRACE_MAP(MMI_PIM_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_PIM_TRC_H_


/******************************************************************************
 ** File Name:      mmi_push_mail.h                                         *
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
//trace_id:263
#ifndef _MMI_PUSH_MAIL_H_
#define _MMI_PUSH_MAIL_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define COMM_DEBUG_165_112_2_18_3_26_5_0 "comm_TraceFile: len :%d"
#define COMM_HTTP_356_112_2_18_3_26_7_1 "Mail:comm_NetProxyRespResult cmd is to long1"
#define COMM_HTTP_371_112_2_18_3_26_7_2 "Mail:comm_NetProxyRespResult cmd is to long2"
#define COMM_HTTP_383_112_2_18_3_26_7_3 "Mail:comm_NetProxyRespResult cmd is to long3"
#define COMM_HTTP_486_112_2_18_3_26_7_4 "Mail:comm_NetProxyRespResult not exit http"
#define COMM_HTTP_493_112_2_18_3_26_7_5 "Mail:comm_NetProxyRespResult is ok 200"
#define COMM_HTTP_508_112_2_18_3_26_7_6 "Mail:comm_NetProxyRespResult connect result = %s"
#define COMM_HTTP_516_112_2_18_3_26_7_7 "Mail:comm_NetProxyRespResult not exit 200"
#define COMM_MEMORY_154_112_2_18_3_26_8_8 "Mail:mail_mem_link_free: %x file: %s line:%d"
#define COMM_MEMORY_190_112_2_18_3_26_8_9 "Mail:debug_comm_alloc size :%d- %s:%d"
#define COMM_MEMORY_205_112_2_18_3_26_8_10 "Mail:debug_comm_alloc NULL"
#define COMM_MEMORY_263_112_2_18_3_26_8_11 "Mail:comm_alloc NULL"
#define COMM_OMA_DOWNLOAD_294_112_2_18_3_26_9_12 "comm_start_oma_download mem err"
#define COMM_OMA_DOWNLOAD_307_112_2_18_3_26_9_13 "comm_start_oma_download is downloading, can not down"
#define COMM_OMA_DOWNLOAD_351_112_2_18_3_26_9_14 "comm_start_oma_download error !"
#define COMM_OMA_DOWNLOAD_402_112_2_18_3_26_9_15 "comm_PostSuccessResult %s"
#define COMM_OMA_DOWNLOAD_505_112_2_18_3_26_9_16 "comm_PostFailResult %s"
#define COMM_SOCKET_134_112_2_18_3_26_11_17 "Mail:comm_sock_listnode handle:%d index:%d node:%d"
#define COMM_SOCKET_219_112_2_18_3_26_11_18 "Mail:  socket sess node: %d\n"
#define COMM_SOCKET_223_112_2_18_3_26_11_19 "Mail:  ssl session handle %d\n"
#define COMM_SOCKET_228_112_2_18_3_26_11_20 "Mail: no ssl session\n"
#define COMM_SOCKET_250_112_2_18_3_26_11_21 "Mail:  comm_sock_sslhandler sess node: %d\n"
#define COMM_SOCKET_534_112_2_18_3_26_12_22 "Mail: start decrypt data len :%d"
#define COMM_SOCKET_583_112_2_18_3_26_12_23 "Mail: start decrypt data len :%d"
#define COMM_SOCKET_753_112_2_18_3_26_12_24 "MAIL: comm_sock_errno sock :%x err :%d "
#define COMM_SOCKET_782_112_2_18_3_26_12_25 "Mail: close ssl sock handle : %d  result:%d"
#define COMM_SOCKET_786_112_2_18_3_26_12_26 "Mail: close sock result:%d"
#define COMM_SOCKET_967_112_2_18_3_26_13_27 "Mail: sock create ssl error \n"
#define COMM_SOCKET_972_112_2_18_3_26_13_28 "Mail: sock ssl handle :%d \n"
#define COMM_SOCKET_990_112_2_18_3_26_13_29 "Mail: sock ssl addr: %s %d \n"
#define COMM_SOCKET_993_112_2_18_3_26_13_30 "Mail: sock ssl shake result: %d \n"
#define COMM_SOCKET_1019_112_2_18_3_26_13_31 "Mail: start encrypt data len :%d"
#define COMM_SOCKET_1027_112_2_18_3_26_13_32 "Mail: start encrypt error :%d"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_PUSH_MAIL)
TRACE_MSG(COMM_DEBUG_165_112_2_18_3_26_5_0,"comm_TraceFile: len :%d")
TRACE_MSG(COMM_HTTP_356_112_2_18_3_26_7_1,"Mail:comm_NetProxyRespResult cmd is to long1")
TRACE_MSG(COMM_HTTP_371_112_2_18_3_26_7_2,"Mail:comm_NetProxyRespResult cmd is to long2")
TRACE_MSG(COMM_HTTP_383_112_2_18_3_26_7_3,"Mail:comm_NetProxyRespResult cmd is to long3")
TRACE_MSG(COMM_HTTP_486_112_2_18_3_26_7_4,"Mail:comm_NetProxyRespResult not exit http")
TRACE_MSG(COMM_HTTP_493_112_2_18_3_26_7_5,"Mail:comm_NetProxyRespResult is ok 200")
TRACE_MSG(COMM_HTTP_508_112_2_18_3_26_7_6,"Mail:comm_NetProxyRespResult connect result = %s")
TRACE_MSG(COMM_HTTP_516_112_2_18_3_26_7_7,"Mail:comm_NetProxyRespResult not exit 200")
TRACE_MSG(COMM_MEMORY_154_112_2_18_3_26_8_8,"Mail:mail_mem_link_free: %x file: %s line:%d")
TRACE_MSG(COMM_MEMORY_190_112_2_18_3_26_8_9,"Mail:debug_comm_alloc size :%d- %s:%d")
TRACE_MSG(COMM_MEMORY_205_112_2_18_3_26_8_10,"Mail:debug_comm_alloc NULL")
TRACE_MSG(COMM_MEMORY_263_112_2_18_3_26_8_11,"Mail:comm_alloc NULL")
TRACE_MSG(COMM_OMA_DOWNLOAD_294_112_2_18_3_26_9_12,"comm_start_oma_download mem err")
TRACE_MSG(COMM_OMA_DOWNLOAD_307_112_2_18_3_26_9_13,"comm_start_oma_download is downloading, can not down")
TRACE_MSG(COMM_OMA_DOWNLOAD_351_112_2_18_3_26_9_14,"comm_start_oma_download error !")
TRACE_MSG(COMM_OMA_DOWNLOAD_402_112_2_18_3_26_9_15,"comm_PostSuccessResult %s")
TRACE_MSG(COMM_OMA_DOWNLOAD_505_112_2_18_3_26_9_16,"comm_PostFailResult %s")
TRACE_MSG(COMM_SOCKET_134_112_2_18_3_26_11_17,"Mail:comm_sock_listnode handle:%d index:%d node:%d")
TRACE_MSG(COMM_SOCKET_219_112_2_18_3_26_11_18,"Mail:  socket sess node: %d\n")
TRACE_MSG(COMM_SOCKET_223_112_2_18_3_26_11_19,"Mail:  ssl session handle %d\n")
TRACE_MSG(COMM_SOCKET_228_112_2_18_3_26_11_20,"Mail: no ssl session\n")
TRACE_MSG(COMM_SOCKET_250_112_2_18_3_26_11_21,"Mail:  comm_sock_sslhandler sess node: %d\n")
TRACE_MSG(COMM_SOCKET_534_112_2_18_3_26_12_22,"Mail: start decrypt data len :%d")
TRACE_MSG(COMM_SOCKET_583_112_2_18_3_26_12_23,"Mail: start decrypt data len :%d")
TRACE_MSG(COMM_SOCKET_753_112_2_18_3_26_12_24,"MAIL: comm_sock_errno sock :%x err :%d ")
TRACE_MSG(COMM_SOCKET_782_112_2_18_3_26_12_25,"Mail: close ssl sock handle : %d  result:%d")
TRACE_MSG(COMM_SOCKET_786_112_2_18_3_26_12_26,"Mail: close sock result:%d")
TRACE_MSG(COMM_SOCKET_967_112_2_18_3_26_13_27,"Mail: sock create ssl error \n")
TRACE_MSG(COMM_SOCKET_972_112_2_18_3_26_13_28,"Mail: sock ssl handle :%d \n")
TRACE_MSG(COMM_SOCKET_990_112_2_18_3_26_13_29,"Mail: sock ssl addr: %s %d \n")
TRACE_MSG(COMM_SOCKET_993_112_2_18_3_26_13_30,"Mail: sock ssl shake result: %d \n")
TRACE_MSG(COMM_SOCKET_1019_112_2_18_3_26_13_31,"Mail: start encrypt data len :%d")
TRACE_MSG(COMM_SOCKET_1027_112_2_18_3_26_13_32,"Mail: start encrypt error :%d")
END_TRACE_MAP(MMI_PUSH_MAIL)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_PUSH_MAIL_H_


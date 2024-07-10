/******************************************************************************
 ** File Name:      daps_trc.h                                         *
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
//trace_id:7
#ifndef _DAPS_TRC_H_
#define _DAPS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define DLMALLOC_API_176_112_2_17_23_14_12_0 "memcheck:aligned alig = %d ,s = %u \n"
#define DLMALLOC_API_179_112_2_17_23_14_12_1 "memcheck:aligned mem_ptr = %x ,s = %u \n"
#define DLMALLOC_API_228_112_2_17_23_14_12_2 "memcheck:rl new size_1 = %x \n"
#define DLMALLOC_API_233_112_2_17_23_14_12_3 "memcheck:rl new size_2 = %x \n"
#define DLMALLOC_API_295_112_2_17_23_14_12_4 "CFL_DestroyMspace invalid input"
#define DLMALLOC_API_314_112_2_17_23_14_12_5 "CFL_MspaceMalloc invalid input"
#define DLMALLOC_API_336_112_2_17_23_14_12_6 "CFL_MspaceFree invalid input"
#define DLMALLOC_API_358_112_2_17_23_14_12_7 "CFL_MspaceRealloc invalid input"
#define DLMALLOC_API_381_112_2_17_23_14_12_8 "CFL_MspaceGetInfo invalid input"
#define CFL_MD5_231_112_2_17_23_14_17_9 "CFL_MD5Hash input error"
#define CFL_MD5_234_112_2_17_23_14_17_10 "CFL_MD5Hash dest_ptr =%x,md =%x"
#define CFL_MD5_236_112_2_17_23_14_17_11 "CFL_MD5Hash after init dest_ptr =%x,md =%x"
#define CFL_MD5_238_112_2_17_23_14_17_12 "CFL_MD5Hash after update dest_ptr =%x,md =%x"
#define CFL_MD5_240_112_2_17_23_14_17_13 "CFL_MD5Hash after final dest_ptr =%x,md =%x"
#define CFL_MD5_252_112_2_17_23_14_17_14 "CFL_MD5Init mdcontext =%x"
#define CFL_MD5_284_112_2_17_23_14_17_15 "CFL_MD5Update input error"
#define CFL_MD5_288_112_2_17_23_14_17_16 "CFL_MD5Update mdcontext =%x"
#define CFL_MD5_315_112_2_17_23_14_17_17 "CFL_MD5Update i=%d,ii=%d"
#define CFL_MD5_320_112_2_17_23_14_17_18 "CFL_MD5Update end mdcontext =%x"
#define CFL_MD5_337_112_2_17_23_14_17_19 "CFL_MD5Final mdcontext =%x"
#define CFL_MD5_353_112_2_17_23_14_17_20 "CFL_MD5Final after update mdcontext =%x"
#define CFL_MD5_362_112_2_17_23_14_17_21 "CFL_MD5Final after append mdcontext =%x,i=%d,ii=%d"
#define CFL_MD5_364_112_2_17_23_14_17_22 "CFL_MD5Final after transform mdcontext =%x"
#define CFL_MD5_376_112_2_17_23_14_17_23 "CFL_MD5Final after srore mdcontext =%x,i=%d,ii=%d"
#define CFL_MEM_DEBUG_169_112_2_17_23_14_19_24 "\n[DAPS] MEMORY TRACE START\n"
#define CFL_MEM_DEBUG_173_112_2_17_23_14_19_25 "(%d)Leak at: %s(%d)\n"
#define CFL_MEM_DEBUG_174_112_2_17_23_14_19_26 "Address: 0x%08x"
#define CFL_MEM_DEBUG_178_112_2_17_23_14_19_27 "\n[DAPS] MEMORY LEAK %d\n"
#define CFL_MEM_DEBUG_179_112_2_17_23_14_19_28 "\n[DAPS] MEMORY TRACE END\n"
#define CFL_MEM_DEBUG_180_112_2_17_23_14_19_29 "\n[DAPS] MEMORY PEAK %u\n"
#define CFL_MEM_DEBUG_181_112_2_17_23_14_19_30 "\n[DAPS] MEMORY USED %u\n"
#define CFL_CHAR_164_112_2_17_23_14_20_31 "\n CFLCHAR_StrNICmp input error"
#define CFL_CHAR_192_112_2_17_23_14_20_32 "CFL_Snprintf warning:buffer maybe has truncated!"
#define CFL_STRING_64_112_2_17_23_14_21_33 "CFL_MEMMOVE: invalid input"
#define CFL_STRING_110_112_2_17_23_14_21_34 "CFL_GetChar: invalid input"
#define CFL_STRING_116_112_2_17_23_14_21_35 "CFL_GetChar: invalid pos"
#define CFL_STRING_138_112_2_17_23_14_21_36 "CFL_SetChar : invalid string"
#define CFL_STRING_144_112_2_17_23_14_21_37 "CFL_SetChar : invalid pos"
#define CFL_STRING_170_112_2_17_23_14_21_38 "CFL_GETCHAR: invalid input"
#define CFL_STRING_368_112_2_17_23_14_22_39 "CFL_OCTSTRDELET: invalid input"
#define CFL_STRING_401_112_2_17_23_14_22_40 "CFL_octstrcreateempty:create string fail"
#define CFL_STRING_444_112_2_17_23_14_22_41 "CFL_GROW: realloc fail "
#define CFL_STRING_455_112_2_17_23_14_22_42 "CFL_STRGROW: input invalid"
#define CFL_STRING_481_112_2_17_23_14_22_43 "CFL_CREATEFROMDATA: input invalid"
#define CFL_STRING_502_112_2_17_23_14_22_44 "CFL_CREATEFROMDATA: alloc space fail"
#define CFL_STRING_510_112_2_17_23_14_22_45 "CFL_CREATEFROMDATA: create fail"
#define CFL_STRING_558_112_2_17_23_14_22_46 "CFL_Octstrdestroy: invalid input"
#define CFL_STRING_584_112_2_17_23_14_22_47 "CFL_OctstrCopy: invalid input"
#define CFL_STRING_646_112_2_17_23_14_22_48 "CFL_OctstrInsertData: invalid input"
#define CFL_STRING_694_112_2_17_23_14_22_49 "CFL_APPENDATA: invalid input"
#define CFL_STRING_730_112_2_17_23_14_22_50 "CFL_APPENDCSTR: invalid input"
#define CFL_STRING_764_112_2_17_23_14_22_51 "CFL_APPENDCHAR : invalid input"
#define CFL_STRING_815_112_2_17_23_14_22_52 "CFL_string cat :not enough space"
#define CFL_STRING_845_112_2_17_23_14_22_53 "CFL_RETRIEVEDATA: invalid input"
#define CFL_STRING_1098_112_2_17_23_14_23_54 "CFL_PACKOctstr: invalid input"
#define CFL_STRING_1165_112_2_17_23_14_23_55 "#CFL CFL_StrClone len==0!"
#define CFL_STRING_1184_112_2_17_23_14_23_56 "#CFL CFL_StrClone alloc fail!"
#define CFL_WCHAR_49_112_2_17_23_14_24_57 "\n CFLWCHAR_StrCmp input error"
#define CFL_WCHAR_86_112_2_17_23_14_24_58 "\n CFLWCHAR_StrCmpWithAsciiStr input error"
#define CFL_WCHAR_437_112_2_17_23_14_25_59 "\n--------\n"
#define CFL_WCHAR_444_112_2_17_23_14_25_60 "%s"
#define CFL_WCHAR_451_112_2_17_23_14_25_61 "%s"
#define CFL_WCHAR_453_112_2_17_23_14_25_62 "\n--------\n"
#define CFL_TIMER_92_112_2_17_23_14_26_63 "CFLTIMER_CreateTimer invalid input"
#define CFL_TIMER_105_112_2_17_23_14_26_64 "CFLTIMER_CreateTimer InsertNode fail"
#define CFL_TIMER_112_112_2_17_23_14_26_65 "CFLTIMER_CreateTimer created_timer_data_ptr is null"
#define CFL_TIMER_115_112_2_17_23_14_26_66 "CFLTIMER creat, context_ptr:%x,sig_code:%x"
#define CFL_TIMER_144_112_2_17_23_14_26_67 "CFLTIMER_CreateTimer SCI_CreateTimer fail!"
#define CFL_TIMER_168_112_2_17_23_14_26_68 "CFLTIMER delete,get data err!"
#define CFL_TIMER_171_112_2_17_23_14_26_69 "CFLTIMER delete, context_ptr:%x,sig_code:%x"
#define CFL_TIMER_240_112_2_17_23_14_26_70 "CFLTIMER CALLBACK, context_ptr:%x"
#define CFL_TIMER_249_112_2_17_23_14_26_71 "CFLTIMER CALLBACK, alloc memory fail!"
#define CFL_TIMER_416_112_2_17_23_14_27_72 "CFLTIMER GetNodeData, invalid timer node!"
#define PEERSOCKET_187_112_2_17_23_17_3_73 "udp_callback: The packet is not mine!"
#define PEERSOCKET_196_112_2_17_23_17_3_74 "udp_callback() data_khk_rec%d"
#define PEERSOCKET_199_112_2_17_23_17_3_75 "error:recv data_len = NULL"
#define PEERSOCKET_204_112_2_17_23_17_3_76 "recv data_len = %d"
#define PEERSOCKET_227_112_2_17_23_17_3_77 "free recv data ptr"
#define PEERSOCKET_253_112_2_17_23_17_3_78 "fhost = 0x%x"
#define PEERSOCKET_259_112_2_17_23_17_3_79 "%s"
#define PEERSOCKET_346_112_2_17_23_17_3_80 "peerSocket:sci_sock_socket() csocket=0x%08x"
#define PEERSOCKET_360_112_2_17_23_17_3_81 "peerSocket:peerCreate() error:%d"
#define PEERSOCKET_402_112_2_17_23_17_3_82 "peerSocket.c:peerConnect() socket:0x%08x,ip=%s"
#define PEERSOCKET_444_112_2_17_23_17_3_83 "peerConnect() ret:%d, ret=%d"
#define PEERSOCKET_449_112_2_17_23_17_3_84 "peerConnect() error0:%d"
#define PEERSOCKET_467_112_2_17_23_17_3_85 "peerConnect success"
#define PEERSOCKET_516_112_2_17_23_17_3_86 "mmiwap.c:peerSend() len=%d,buf=%d"
#define PEERSOCKET_530_112_2_17_23_17_3_87 "mmiwap.c:peerSend() socket = 0x%08x, s_socketname = 0x%08x"
#define PEERSOCKET_536_112_2_17_23_17_3_88 "mmiwap.c:peerSend() data_len=%d"
#define PEERSOCKET_545_112_2_17_23_17_3_89 "mmiwap.c:peerSend() error:%d"
#define PEERSOCKET_764_112_2_17_23_17_4_90 "mmiwap.c:cmsgethostip() IPAddress:%s"
#define PEERSOCKET_829_112_2_17_23_17_4_91 "mmiwap.c:cmsclosesocket() in"
#define PEERSOCKET_832_112_2_17_23_17_4_92 "mmiwap.c:cmsclosesocket() socket != s_socketname"
#define PEERSOCKET_839_112_2_17_23_17_4_93 "mmiwap.c:cmsclosesocket() ret=%d"
#define PEERSOCKET_843_112_2_17_23_17_4_94 "mmiwap.c:cmsclosesocket() error:%d"
#define PEERSOCKET_911_112_2_17_23_17_4_95 "mmiwap.c:cmsgetlasterror() before error=%d"
#define PEERSOCKET_966_112_2_17_23_17_4_96 "peerSetOpt() SocketId=%ld!"
#define PEERSOCKET_984_112_2_17_23_17_4_97 "mmiwap.c:cmsbsd_setsockopt() DB_NET_FAIL!"
#define PEERSOCKET_1012_112_2_17_23_17_4_98 "fhost = 0x%x"
#define PEERSOCKET_1018_112_2_17_23_17_4_99 "%s"
#define PEERSOCKET_1057_112_2_17_23_17_4_100 "send abort data"
#define PEERSOCKET_1063_112_2_17_23_17_4_101 "redirect connect "
#define PEERSOCKET_1080_112_2_17_23_17_4_102 "re create socket error!"
#define PEERSOCKET_1086_112_2_17_23_17_4_103 "Normorl send"
#define PEERSOCKET_1092_112_2_17_23_17_4_104 "mmiwap.c:Cms_SendPDU() error send failed!!!"
#define PEERSOCKET_1097_112_2_17_23_17_4_105 "mmiwap.c:Cms_SendPDU() data_khk_send%d"
#define PEERSOCKET_1115_112_2_17_23_17_4_106 "mmiwap.c:Cms_RecvPDU() data_len=%d"
#define QF_24_112_2_17_23_17_7_107 "qhsm onAssert__ WINDOWS_PLATFORM"
#define QF_27_112_2_17_23_17_7_108 "qhsm onAssert__ error"
#define QHSM_39_112_2_17_23_17_7_109 "qhsm HSM_Init HSM not executed"
#define QHSM_47_112_2_17_23_17_7_110 "qhsm HSM_Init error first"
#define QHSM_55_112_2_17_23_17_7_111 "qhsm HSM_Init error second"
#define QHSM_78_112_2_17_23_17_7_112 "qhsm HSM_TranStatic cannot target top state"
#define QHSM_85_112_2_17_23_17_7_113 "qhsm HSM_TranStatic error"
#define QHSM_200_112_2_17_23_17_8_114 "qhsm HsmTranSetup__ inLCA error entry"
#define QHSM_210_112_2_17_23_17_8_115 "qhsm HsmTranSetup__ inLCA error"
#define QHSM_225_112_2_17_23_17_8_116 "qhsm HsmTranSetup__ inLCA transition initialized"
#define QHSMTRAN_15_112_2_17_23_17_8_117 "HSM_TranDyn input error!"
#define QHSMTRAN_21_112_2_17_23_17_8_118 "qhsm HSM_TranDyn cannot target top state"
#define QHSMTRAN_28_112_2_17_23_17_8_119 "qhsm HSM_TranDyn error"
#define QHSMTRAN_97_112_2_17_23_17_8_120 "qhsm HSM_TranDyn error inLCA entry"
#define QHSMTRAN_107_112_2_17_23_17_8_121 "qhsm HSM_TranDyn error inLCA"
#define HTTP_API_67_112_2_17_23_17_8_122 "\n http HTTP_InitRequest task id = %u"
#define HTTP_API_83_112_2_17_23_17_8_123 "\n http HTTP_InitRequest error_code = %d"
#define HTTP_API_133_112_2_17_23_17_8_124 "\n http HTTP_GetRequest error_code = %d"
#define HTTP_API_183_112_2_17_23_17_8_125 "\n http HTTP_PostRequest error_code = %d"
#define HTTP_API_233_112_2_17_23_17_9_126 "\n http HTTP_HeadRequest error_code = %d"
#define HTTP_API_253_112_2_17_23_17_9_127 "HTTP_AuthResponse request_id =%d"
#define HTTP_API_286_112_2_17_23_17_9_128 "\n http HTTP_AuthResponse error_code = %d"
#define HTTP_API_330_112_2_17_23_17_9_129 "\n http HTTP_CloseRequest error_code = %d"
#define HTTP_API_349_112_2_17_23_17_9_130 "\n http HTTP_CancelRequest request_id =%d"
#define HTTP_API_379_112_2_17_23_17_9_131 "\n http HTTP_CancelRequest error_code = %d"
#define HTTP_API_401_112_2_17_23_17_9_132 "HTTP_HeaderResponse request_id =%d"
#define HTTP_API_440_112_2_17_23_17_9_133 "\n http HTTP_HeaderResponse error_code = %d"
#define HTTP_API_489_112_2_17_23_17_9_134 "\n http HTTP_CookieSetReq error_code = %d"
#define HTTP_API_586_112_2_17_23_17_9_135 "\n http HTTP_CookieClear error_code = %d"
#define HTTP_API_616_112_2_17_23_17_9_136 "\n http HTTP_CacheClear 1 error_code = %d"
#define HTTP_API_633_112_2_17_23_17_9_137 "\n http HTTP_CacheClear 2 error_code = %d"
#define HTTP_API_652_112_2_17_23_17_9_138 "\n http HTTP_CacheClear 3 error_code = %d"
#define HTTP_API_666_112_2_17_23_17_9_139 "\n http HTTP_CacheClear 4 error_code = %d"
#define HTTP_API_718_112_2_17_23_17_10_140 "HTTP_SslCertResponse request_id =%d"
#define HTTP_API_748_112_2_17_23_17_10_141 "\n http HTTP_SslCertResponse error_code = %d"
#define HTTP_AUTH_70_112_2_17_23_17_10_142 "\n http HttpAuthenDigestResponseCreate alloc error"
#define HTTP_AUTH_87_112_2_17_23_17_10_143 "\n http HttpAuthenDigestResponseDestroy input error \n"
#define HTTP_AUTH_155_112_2_17_23_17_10_144 "\n http HttpAuthenDigestRequestCreate alloc error \n"
#define HTTP_AUTH_172_112_2_17_23_17_10_145 "\n http HttpAuthenDigestRequestDestory input error \n"
#define HTTP_AUTH_280_112_2_17_23_17_10_146 "\n http HttpAuthenBasicEncode input error \n"
#define HTTP_AUTH_287_112_2_17_23_17_10_147 "\n http HttpAuthenBasicEncode src_ptr alloc error \n"
#define HTTP_AUTH_292_112_2_17_23_17_10_148 "\n http HttpAuthenBasicEncode src_ptr=%s"
#define HTTP_AUTH_299_112_2_17_23_17_10_149 "\n http HttpAuthenBasicEncode dst_ptr alloc error \n"
#define HTTP_AUTH_328_112_2_17_23_17_10_150 "\n http HttpAuthenDigestEncode input error \n"
#define HTTP_AUTH_334_112_2_17_23_17_10_151 "\n http HttpAuthenDigestEncode alloc error \n"
#define HTTP_AUTH_341_112_2_17_23_17_10_152 "\n http HttpAuthenDigestEncode HttpAuthenDigestRequsetSetParam error \n"
#define HTTP_AUTH_381_112_2_17_23_17_11_153 "\n http HttpAuthenDigestCalcHA1 input error \n"
#define HTTP_AUTH_424_112_2_17_23_17_11_154 "\n http HttpAuthenDigestCalcResult input error \n"
#define HTTP_AUTH_483_112_2_17_23_17_11_155 "\n http HttpAuthenDigestRequsetSetParam input error \n"
#define HTTP_AUTH_666_112_2_17_23_17_11_156 "\n http HttpAuthenDigestRequsetSetParam error_code =%d \n"
#define HTTP_AUTH_720_112_2_17_23_17_11_157 "\n http HttpAuthenDigestResponseParse input error \n"
#define HTTP_AUTH_726_112_2_17_23_17_11_158 "\n http HttpAuthenDigestResponseParse alloc error \n"
#define HTTP_AUTH_753_112_2_17_23_17_11_159 "\n http HttpAuthenDigestResponseParse 1 error_code =%d"
#define HTTP_AUTH_770_112_2_17_23_17_11_160 "\n http HttpAuthenDigestResponseParse error net data \n"
#define HTTP_AUTH_779_112_2_17_23_17_11_161 "\n http HttpAuthenDigestResponseParse 2 error_code=%d"
#define HTTP_AUTH_813_112_2_17_23_17_11_162 "\n http HttpAuthenDigestResponseSaveParam invalid input \n"
#define HTTP_AUTH_858_112_2_17_23_17_11_163 "\n http HttpAuthenDigestResponseSaveParam tail = pnull"
#define HTTP_AUTH_1010_112_2_17_23_17_12_164 "\n HTTP HttpAuthenDigestResponseSaveParam unknown param \n"
#define HTTP_AUTH_1017_112_2_17_23_17_12_165 "\n http HttpAuthenDigestResponseSaveParam error_code=%d"
#define HTTP_AUTH_1025_112_2_17_23_17_12_166 "\n http HttpAuthenDigestResponseSaveParam destroy request_digest_ptr"
#define HTTP_AUTH_1037_112_2_17_23_17_12_167 "\n http HttpAuthenDigestResponseSaveParam CLEAR error_code=%d"
#define HTTP_AUTH_1053_112_2_17_23_17_12_168 "\n http HttpAuthenDigestTokenFind input error \n"
#define HTTP_AUTH_1066_112_2_17_23_17_12_169 "\n http HttpAuthenDigestTokenFind auth map type =%d"
#define HTTP_AUTH_1070_112_2_17_23_17_12_170 "\n http HttpAuthenDigestTokenFind error \n"
#define HTTP_AUTH_1089_112_2_17_23_17_12_171 "\n http HttpAuthenGetInfo input error \n"
#define HTTP_AUTH_1130_112_2_17_23_17_12_172 "\n HTTP HttpAuthenGetInfo unknown scheme\n"
#define HTTP_AUTH_1163_112_2_17_23_17_12_173 "\n http HttpAuthenCombineResult input error \n"
#define HTTP_AUTH_1174_112_2_17_23_17_12_174 "\n http HttpAuthenCombineResult alloc error \n"
#define HTTP_AUTH_1189_112_2_17_23_17_12_175 "\n http HttpAuthenCombineResult 1 input error \n"
#define HTTP_AUTH_1195_112_2_17_23_17_12_176 "\n http HttpAuthenCombineResult 1 alloc error \n"
#define HTTP_CACHE_120_112_2_17_23_17_13_177 "\n http HttpSaveDataToCache error_code =%d"
#define HTTP_CACHE_147_112_2_17_23_17_13_178 "\n http HandleCacheDataToApp 1 input error \n"
#define HTTP_CACHE_154_112_2_17_23_17_13_179 "\n http HandleCacheDataToApp 2 input error \n"
#define HTTP_CACHE_170_112_2_17_23_17_13_180 "\n http HandleCacheDataToApp file not found error \n"
#define HTTP_CACHE_177_112_2_17_23_17_13_181 "\n http HandleCacheDataToApp get file size error \n"
#define HTTP_CACHE_195_112_2_17_23_17_13_182 "\n http HandleCacheDataToApp alloc error \n"
#define HTTP_CACHE_205_112_2_17_23_17_13_183 "\n http HandleCacheDataToApp file read error \n"
#define HTTP_CACHE_217_112_2_17_23_17_13_184 "\n http HandleCacheDataToApp file write error \n"
#define HTTP_CACHE_253_112_2_17_23_17_13_185 "\n http HandleCacheDataToApp send HTTP_SIG_GET_CNF"
#define HTTP_CACHE_263_112_2_17_23_17_13_186 "\n http HandleCacheDataToApp error method\n"
#define HTTP_CACHE_API_70_112_2_17_23_17_13_187 "\n HttpCacheNodeSetExpires input error \n"
#define HTTP_CACHE_API_112_112_2_17_23_17_13_188 "\n http HttpCacheInitialize input error \n"
#define HTTP_CACHE_API_119_112_2_17_23_17_13_189 "\n http HttpCacheInitialize HTTP_ERROR_NO_MEMORY"
#define HTTP_CACHE_API_125_112_2_17_23_17_13_190 "\n http HttpCacheInitialize HTTP_ERROR_INVALID_INPUT 1"
#define HTTP_CACHE_API_131_112_2_17_23_17_13_191 "\n http HttpCacheInitialize HTTP_ERROR_INVALID_INPUT 2"
#define HTTP_CACHE_API_148_112_2_17_23_17_13_192 "\n http HttpCacheInitialize HTTP_ERROR_FILE_OPEN"
#define HTTP_CACHE_API_156_112_2_17_23_17_13_193 "\n http HttpCacheInitialize HTTP_ERROR_FILE_GET_SIZE"
#define HTTP_CACHE_API_180_112_2_17_23_17_13_194 "\n http HttpCacheInitialize HTTP_ERROR_FILE_READ"
#define HTTP_CACHE_API_191_112_2_17_23_17_13_195 "\n http HttpCacheInitialize curr count = 0 or curr size = 0"
#define HTTP_CACHE_API_220_112_2_17_23_17_14_196 "\n http HttpCacheInitialize alloc error \n"
#define HTTP_CACHE_API_233_112_2_17_23_17_14_197 "\n http HttpCacheInitialize str_token_handle alloc error \n"
#define HTTP_CACHE_API_285_112_2_17_23_17_14_198 "\n http HttpCacheInitialize HTTP_ERROR_INVALID_INPUT 2"
#define HTTP_CACHE_API_358_112_2_17_23_17_14_199 "\n http HttpCacheNodeDestroy input error \n"
#define HTTP_CACHE_API_407_112_2_17_23_17_14_200 "\n http HttpCacheNodeCreate input error\n"
#define HTTP_CACHE_API_414_112_2_17_23_17_14_201 "\n http HttpCacheNodeCreate 1 alloc error\n"
#define HTTP_CACHE_API_424_112_2_17_23_17_14_202 "\n http HttpCacheNodeCreate 2 alloc error\n"
#define HTTP_CACHE_API_457_112_2_17_23_17_14_203 "\n http HttpCacheAppend 1 input error\n"
#define HTTP_CACHE_API_465_112_2_17_23_17_14_204 "\n http HttpCacheAppend 2 input error\n"
#define HTTP_CACHE_API_488_112_2_17_23_17_14_205 "\n http HttpCacheAppend HTTP_ERROR_FILE_OPEN"
#define HTTP_CACHE_API_499_112_2_17_23_17_14_206 "\n http HttpCacheAppend HTTP_ERROR_FILE_WRITE"
#define HTTP_CACHE_API_537_112_2_17_23_17_14_207 "\n http HttpGetCache input error\n"
#define HTTP_CACHE_API_544_112_2_17_23_17_14_208 "\n http HttpGetCache alloc error\n"
#define HTTP_CACHE_API_554_112_2_17_23_17_14_209 "n http HttpGetCache md_buf =%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x"
#define HTTP_CACHE_API_565_112_2_17_23_17_14_210 "\n http HttpGetCache 2 input error\n"
#define HTTP_CACHE_API_583_112_2_17_23_17_14_211 "\n http HttpGetCache HTTP_ERROR_FILE_OPEN"
#define HTTP_CACHE_API_608_112_2_17_23_17_14_212 "\n http HttpGetCache not get cache node\n"
#define HTTP_CACHE_API_626_112_2_17_23_17_14_213 "n http HttpCacheFindNodeByName 1 node file name =%s,file_name_ptr =%s"
#define HTTP_CACHE_API_630_112_2_17_23_17_14_214 "\n http HttpCacheFindNodeByName not find node"
#define HTTP_CACHE_API_648_112_2_17_23_17_15_215 "\n http HttpCacheDelete 1 input error\n"
#define HTTP_CACHE_API_656_112_2_17_23_17_15_216 "\n http HttpCacheDelete 2 input error\n"
#define HTTP_CACHE_API_718_112_2_17_23_17_15_217 "\n http HttpCacheFinalize 1 input error\n"
#define HTTP_CACHE_API_724_112_2_17_23_17_15_218 "\n http HttpCacheFinalize 2 input error\n"
#define HTTP_CACHE_API_729_112_2_17_23_17_15_219 "\n http HttpCacheFinalize 3 input error\n"
#define HTTP_CACHE_API_836_112_2_17_23_17_15_220 "\n http HttpCacheFinalize error_code =%d\n"
#define HTTP_CACHE_API_872_112_2_17_23_17_15_221 "\n http HttpCacheDeleteLRU 1 input error\n"
#define HTTP_CACHE_API_883_112_2_17_23_17_15_222 "\n http HttpCacheDeleteLRU 2 input error\n"
#define HTTP_CACHE_API_919_112_2_17_23_17_15_223 "\n http HttpCacheInsert input error\n"
#define HTTP_CACHE_API_957_112_2_17_23_17_15_224 "\n http HttpSetCachePath input error\n"
#define HTTP_CACHE_API_976_112_2_17_23_17_15_225 "\n http HttpSetCacheIndexFile input error\n"
#define HTTP_CACHE_API_994_112_2_17_23_17_15_226 "\n http HttpCacheSetEnable input error\n"
#define HTTP_CACHE_API_1031_112_2_17_23_17_15_227 "\n http HttpCacheGetEtag input error\n"
#define HTTP_CACHE_API_1052_112_2_17_23_17_15_228 "\n http HttpCacheGetLastModified input error\n"
#define HTTP_CHUNK_72_112_2_17_23_17_16_229 "\n http HttpChunkDecode 1 input error\n"
#define HTTP_CHUNK_81_112_2_17_23_17_16_230 "\n http HttpChunkDecode 2 input error\n"
#define HTTP_CHUNK_88_112_2_17_23_17_16_231 "\n http HttpChunkDecode alloc error\n"
#define HTTP_CHUNK_99_112_2_17_23_17_16_232 "\n http HttpChunkDecode net data error\n"
#define HTTP_CHUNK_107_112_2_17_23_17_16_233 "\n http HttpChunkDecode free content\n"
#define HTTP_CHUNK_153_112_2_17_23_17_16_234 "\n http delete timer"
#define HTTP_CHUNK_159_112_2_17_23_17_16_235 "\n http create timer"
#define HTTP_CHUNK_188_112_2_17_23_17_16_236 "\n http HttpChunkDecode file write error\n"
#define HTTP_CHUNK_205_112_2_17_23_17_16_237 "\n http chunk state = %d\n"
#define HTTP_CHUNK_232_112_2_17_23_17_16_238 "\n http HttpChunkDecode 1 error_code =%d"
#define HTTP_CHUNK_268_112_2_17_23_17_16_239 "\n http Http_Idle send HTTP_SIG_GET_CNF"
#define HTTP_CHUNK_284_112_2_17_23_17_16_240 "\n http HandleHttpContent redirect error \n"
#define HTTP_CHUNK_323_112_2_17_23_17_16_241 "\n http HttpChunkDecode 2 error_code =%d"
#define HTTP_CHUNK_355_112_2_17_23_17_16_242 "\n http HttpChunkDecode 3 error_code =%d"
#define HTTP_CHUNK_492_112_2_17_23_17_17_243 "\n http HttpChunkDecode send HTTP_SIG_GET_CNF"
#define HTTP_CHUNK_504_112_2_17_23_17_17_244 "\n HTTP HttpChunkDecode unsupport method\n"
#define HTTP_CHUNK_523_112_2_17_23_17_17_245 "\n http delete timer"
#define HTTP_CHUNK_530_112_2_17_23_17_17_246 "\n http create timer"
#define HTTP_CHUNK_553_112_2_17_23_17_17_247 "\n http HttpChunkParse input error\n"
#define HTTP_CHUNK_635_112_2_17_23_17_17_248 "\n http HttpChunkParse 1 chunk error stop\n"
#define HTTP_CHUNK_700_112_2_17_23_17_17_249 "\n http HttpChunkParse bad chunk error\n"
#define HTTP_CHUNK_711_112_2_17_23_17_17_250 "\n http HttpChunkParse 2 chunk error stop\n"
#define HTTP_CHUNK_716_112_2_17_23_17_17_251 "\n http HttpChunkParse chunk error state\n"
#define HTTP_CHUNK_722_112_2_17_23_17_17_252 "\n http HttpChunkParse ok\n"
#define HTTP_CHUNK_737_112_2_17_23_17_17_253 "\n http HTTPChunkReadExt input error\n"
#define HTTP_CHUNK_758_112_2_17_23_17_17_254 "\n http HTTPChunkInit input error\n"
#define HTTP_CHUNK_778_112_2_17_23_17_17_255 "\n http HttpChunkCreate alloc error\n"
#define HTTP_CHUNK_798_112_2_17_23_17_17_256 "\n http HttpChunkDestroy input error\n"
#define HTTP_COOKIE_API_144_112_2_17_23_17_18_257 "\n http HttpCookieNodeCreate input error\n"
#define HTTP_COOKIE_API_151_112_2_17_23_17_18_258 "\n http HttpCookieNodeCreate alloc error\n"
#define HTTP_COOKIE_API_194_112_2_17_23_17_18_259 "\n http HttpCookieParse 1 alloc error\n"
#define HTTP_COOKIE_API_207_112_2_17_23_17_18_260 "\n http HttpCookieParse 2 alloc error\n"
#define HTTP_COOKIE_API_223_112_2_17_23_17_18_261 "\n http HttpCookieParse 3 alloc error\n"
#define HTTP_COOKIE_API_439_112_2_17_23_17_18_262 "\n http HttpCookieParse cookie_node_ptr=%x\n"
#define HTTP_COOKIE_API_512_112_2_17_23_17_18_263 "\n http HttpCookiePut input error\n"
#define HTTP_COOKIE_API_530_112_2_17_23_17_18_264 "HTTP HttpCookiePut find domain %s"
#define HTTP_COOKIE_API_541_112_2_17_23_17_18_265 "HTTP HttpCookiePut find port %d"
#define HTTP_COOKIE_API_553_112_2_17_23_17_18_266 "HTTP HttpCookiePut find path %s"
#define HTTP_COOKIE_API_573_112_2_17_23_17_19_267 "HTTP COOKIE REPLACE name =%s"
#define HTTP_COOKIE_API_585_112_2_17_23_17_19_268 "http HttpCookiePut value =%s"
#define HTTP_COOKIE_API_591_112_2_17_23_17_19_269 "http HttpCookiePut other name =%s"
#define HTTP_COOKIE_API_595_112_2_17_23_17_19_270 "http HttpCookiePut other value =%s"
#define HTTP_COOKIE_API_616_112_2_17_23_17_19_271 "\n http HttpCookiePut alloc 1 error\n"
#define HTTP_COOKIE_API_648_112_2_17_23_17_19_272 "\n http HttpCookiePut 2 alloc error\n"
#define HTTP_COOKIE_API_658_112_2_17_23_17_19_273 "\n http HttpCookiePut 3 alloc error\n"
#define HTTP_COOKIE_API_685_112_2_17_23_17_19_274 "\n http HttpCookiePut 4 alloc error\n"
#define HTTP_COOKIE_API_795_112_2_17_23_17_19_275 "\n http HttpCookieInitialize input error\n"
#define HTTP_COOKIE_API_803_112_2_17_23_17_19_276 "\n http HttpCookieInitialize alloc error\n"
#define HTTP_COOKIE_API_813_112_2_17_23_17_19_277 "\n http HttpCookieInitialize HTTP_ERROR_FILE_OPEN"
#define HTTP_COOKIE_API_822_112_2_17_23_17_19_278 "\n http HttpCookieInitialize file read error\n"
#define HTTP_COOKIE_API_1098_112_2_17_23_17_20_279 "\n http HttpCookieFinalize input error\n"
#define HTTP_COOKIE_API_1115_112_2_17_23_17_20_280 "\n http HttpCookieFinalize alloc error\n"
#define HTTP_COOKIE_API_1128_112_2_17_23_17_20_281 "\n http HttpCookieFinalize HTTP_ERROR_FILE_OPEN"
#define HTTP_COOKIE_API_1143_112_2_17_23_17_20_282 "\n http HttpCookieFinalize file write 1 error\n"
#define HTTP_COOKIE_API_1168_112_2_17_23_17_20_283 "\n http HttpCookieFinalize 2 file write error\n"
#define HTTP_COOKIE_API_1193_112_2_17_23_17_20_284 "\n http HttpCookieFinalize 3 file write error\n"
#define HTTP_COOKIE_API_1216_112_2_17_23_17_20_285 "\n http HttpCookieFinalize 5 file write error\n"
#define HTTP_COOKIE_API_1242_112_2_17_23_17_20_286 "\n http HttpCookieFinalize 6 file write error\n"
#define HTTP_COOKIE_API_1266_112_2_17_23_17_20_287 "\n http HttpCookieFinalize 6 file write error\n"
#define HTTP_COOKIE_API_1295_112_2_17_23_17_20_288 "\n http HttpCookieFinalize 7 file write error\n"
#define HTTP_COOKIE_API_1412_112_2_17_23_17_20_289 "\n http HttpCookieSetEnable input error\n"
#define HTTP_COOKIE_API_1437_112_2_17_23_17_20_290 "\n http HttpSetCookieFile input error\n"
#define HTTP_COOKIE_API_1478_112_2_17_23_17_20_291 "\n http HttpGetCookie input error or no cookie\n"
#define HTTP_COOKIE_API_1481_112_2_17_23_17_20_292 "http HttpGetCookie input domain =%s,path =%s,port =%d"
#define HTTP_COOKIE_API_1493_112_2_17_23_17_20_293 "HttpGetCookie domain =%s,tail =%d,path =%s,port =%d"
#define HTTP_COOKIE_API_1503_112_2_17_23_17_20_294 "HttpGetCookie domain =%s,tail =%d"
#define HTTP_COOKIE_API_1513_112_2_17_23_17_20_295 "HttpGetCookie PORT =%d"
#define HTTP_COOKIE_API_1528_112_2_17_23_17_20_296 "HttpGetCookie path=%s"
#define HTTP_COOKIE_API_1551_112_2_17_23_17_20_297 "HttpGetCookie 1 domain =%s,tail =%d,path =%s,port =%d"
#define HTTP_COOKIE_API_1561_112_2_17_23_17_20_298 "HttpGetCookie 1 domain =%s,tail =%d"
#define HTTP_COOKIE_API_1571_112_2_17_23_17_20_299 "HttpGetCookie 1 PORT =%d"
#define HTTP_COOKIE_API_1586_112_2_17_23_17_21_300 "HttpGetCookie 1 path=%s"
#define HTTP_COOKIE_API_1600_112_2_17_23_17_21_301 "\n http HttpGetCookie 1 mark_parent_ptr null\n"
#define HTTP_COOKIE_API_1607_112_2_17_23_17_21_302 "\n http HttpGetCookie alloc error\n"
#define HTTP_COOKIE_API_1626_112_2_17_23_17_21_303 "http HttpGetCookie maker temp_str_ptr =%s"
#define HTTP_COOKIE_API_1650_112_2_17_23_17_21_304 "http HttpGetCookie maker1 temp1_str_ptr =%s"
#define HTTP_COOKIE_API_1656_112_2_17_23_17_21_305 "http HttpGetCookie maker1 temp1_str_ptr =%s after skip"
#define HTTP_COOKIE_API_1678_112_2_17_23_17_21_306 "\n http HttpGetCookie temp_str_ptr null\n"
#define HTTP_COOKIE_API_1695_112_2_17_23_17_21_307 "http HttpGetCookie format_str_ptr =%s,format_str_len = %d"
#define HTTP_COOKIE_API_1720_112_2_17_23_17_21_308 "\n http HttpCookieFormat input error\n"
#define HTTP_COOKIE_API_1728_112_2_17_23_17_21_309 "\n http HttpCookieFormat alloc error\n"
#define HTTP_COOKIE_API_1760_112_2_17_23_17_21_310 "HttpCookieFormat now =%d,expires =%d"
#define HTTP_COOKIE_API_1766_112_2_17_23_17_21_311 "http HttpCookieFormat buffer 1 =%s,ret =%d"
#define HTTP_COOKIE_API_1772_112_2_17_23_17_21_312 "http HttpCookieFormat buffer 2 =%s,ret =%d"
#define HTTP_COOKIE_API_1792_112_2_17_23_17_21_313 "\n http HttpCookieFormat input 1 error\n"
#define HTTP_COOKIE_API_1811_112_2_17_23_17_21_314 "\n http HttpCookieFormat 1 alloc error\n"
#define HTTP_COOKIE_API_1824_112_2_17_23_17_21_315 "\n http HttpCookieFormat 1 alloc error\n"
#define HTTP_COOKIE_API_1835_112_2_17_23_17_21_316 "http HttpCookieFormat cookie_str_ptr=%s"
#define HTTP_COOKIE_API_1842_112_2_17_23_17_21_317 "http HttpCookieFormat 1 cookie_str_ptr=%s"
#define HTTP_COOKIE_API_1955_112_2_17_23_17_21_318 "HTTP HttpGetCookieListByJS find domain %s"
#define HTTP_COOKIE_API_1965_112_2_17_23_17_21_319 "HTTP HttpGetCookieListByJS find port %d"
#define HTTP_COOKIE_API_2009_112_2_17_23_17_21_320 "HTTP COOKIE delete name =%s"
#define HTTP_DATE_PARSER_278_112_2_17_23_17_22_321 "http HttpDateParse wday_num =%x"
#define HTTP_DATE_PARSER_287_112_2_17_23_17_22_322 "http HttpDateParse mon_num =%x"
#define HTTP_DATE_PARSER_298_112_2_17_23_17_22_323 "http HttpDateParse tz_offset =%x"
#define HTTP_DATE_PARSER_307_112_2_17_23_17_22_324 "http HttpDateParse 1"
#define HTTP_DATE_PARSER_376_112_2_17_23_17_22_325 "http HttpDateParse 2"
#define HTTP_DATE_PARSER_392_112_2_17_23_17_22_326 "http HttpDateParse 3"
#define HTTP_DATE_PARSER_434_112_2_17_23_17_22_327 "http HttpDateParse 4"
#define HTTP_EVENT_38_112_2_17_23_17_23_328 "\n http HttpEventQueueInit input error\n"
#define HTTP_EVENT_58_112_2_17_23_17_23_329 "\n http HttpEventQueueInsert input error\n"
#define HTTP_EVENT_65_112_2_17_23_17_23_330 "\n http HttpEventQueueInsert alloc error\n"
#define HTTP_EVENT_102_112_2_17_23_17_23_331 "\n http HttpEventQueueFetch input error or null\n"
#define HTTP_EVENT_152_112_2_17_23_17_23_332 "\n http HttpEventQueueClear input error\n"
#define HTTP_HEADER_42_112_2_17_23_17_23_333 "\n http HttpSetAcceptHeader input error\n"
#define HTTP_HEADER_49_112_2_17_23_17_23_334 "\n http HttpSetAcceptHeader alloc error\n"
#define HTTP_HEADER_78_112_2_17_23_17_23_335 "\n http HttpSetAcceptCharsetHeader input error\n"
#define HTTP_HEADER_85_112_2_17_23_17_23_336 "\n http HttpSetAcceptCharsetHeader alloc error\n"
#define HTTP_HEADER_114_112_2_17_23_17_23_337 "\n http HttpSetAcceptEncodingHeader input error\n"
#define HTTP_HEADER_121_112_2_17_23_17_23_338 "\n http HttpSetAcceptEncodingHeader alloc error\n"
#define HTTP_HEADER_150_112_2_17_23_17_23_339 "\n http HttpSetAcceptLanguageHeader input error\n"
#define HTTP_HEADER_157_112_2_17_23_17_23_340 "\n http HttpSetAcceptLanguageHeader alloc error\n"
#define HTTP_HEADER_186_112_2_17_23_17_24_341 "\n http HttpSetConnectionHeader input error\n"
#define HTTP_HEADER_193_112_2_17_23_17_24_342 "\n http HttpSetConnectionHeader alloc error\n"
#define HTTP_HEADER_222_112_2_17_23_17_24_343 "\n http HttpSetRefererHeader input error\n"
#define HTTP_HEADER_229_112_2_17_23_17_24_344 "\n http HttpSetRefererHeader alloc error\n"
#define HTTP_HEADER_258_112_2_17_23_17_24_345 "\n http HttpSetUserAgentHeader input error\n"
#define HTTP_HEADER_265_112_2_17_23_17_24_346 "\n http HttpSetUserAgentHeader alloc error\n"
#define HTTP_HEADER_294_112_2_17_23_17_24_347 "\n http HttpSetContentTypeHeader input error\n"
#define HTTP_HEADER_301_112_2_17_23_17_24_348 "\n http HttpSetContentTypeHeader alloc error\n"
#define HTTP_HEADER_330_112_2_17_23_17_24_349 "\n http HttpSetUAProfileHeader input error\n"
#define HTTP_HEADER_337_112_2_17_23_17_24_350 "\n http HttpSetUAProfileHeader alloc error\n"
#define HTTP_HEADER_366_112_2_17_23_17_24_351 "\n http HttpSetHeaderField input error\n"
#define HTTP_HEADER_369_112_2_17_23_17_24_352 "\n http HttpSetHeaderField token=%x,value=%x"
#define HTTP_HEADER_374_112_2_17_23_17_24_353 "\n http HttpSetHeaderField alloc error\n"
#define HTTP_HEADER_397_112_2_17_23_17_24_354 "\n http HttpDestroyHeaderList input error\n"
#define HTTP_HEADER_429_112_2_17_23_17_24_355 "\n http HttpRequestGetHeaderField input error\n"
#define HTTP_HEADER_446_112_2_17_23_17_24_356 "\n http HttpRequestGetHeaderField alloc error\n"
#define HTTP_HSM_513_112_2_17_23_17_25_357 "\n http HTTPHSM_Create host_handle =%x\n"
#define HTTP_HSM_516_112_2_17_23_17_25_358 "\n http HTTPHSM_Create input error\n"
#define HTTP_HSM_523_112_2_17_23_17_25_359 "\n http HTTPHSM_Create 1 alloc error\n"
#define HTTP_HSM_534_112_2_17_23_17_25_360 "\n http HTTPHSM_Create 2 alloc error\n"
#define HTTP_HSM_545_112_2_17_23_17_25_361 "\n http session 1 = %d\n"
#define HTTP_HSM_552_112_2_17_23_17_25_362 "\n http HTTPHSM_Create http_hsm_ptr = %d"
#define HTTP_HSM_604_112_2_17_23_17_25_363 "\n http HTTPHSM_Destroy hsm_ptr = %d"
#define HTTP_HSM_633_112_2_17_23_17_26_364 "\n http HTTPHSM_DispatchSignal input error\n"
#define HTTP_HSM_636_112_2_17_23_17_26_365 "\n http HTTPHSM_DispatchSignal hsm_ptr=%x,signal_code=%d\n"
#define HTTP_HSM_682_112_2_17_23_17_26_366 "\n http HTTPHSM_DealRequest input error\n"
#define HTTP_HSM_693_112_2_17_23_17_26_367 "\n http HTTPHSM_DealRequest new socket for connect method\n"
#define HTTP_HSM_710_112_2_17_23_17_26_368 "\n http HTTPHSM_DealRequest request_ptr=%x,flag = FALSE\n"
#define HTTP_HSM_714_112_2_17_23_17_26_369 "\n http HTTPHSM_DealRequest 1 input error\n"
#define HTTP_HSM_832_112_2_17_23_17_26_370 "http HttpSaveDataToFile create file error!"
#define HTTP_HSM_864_112_2_17_23_17_26_371 "\n http HTTP_TransCallback input error\n"
#define HTTP_HSM_871_112_2_17_23_17_26_372 "\n http HTTP_TransCallback recv NETTRANS_CONNECT_IND\n"
#define HTTP_HSM_876_112_2_17_23_17_26_373 "\n http HTTP_TransCallback recv NETTRANS_CONNECT_IND succ\n"
#define HTTP_HSM_883_112_2_17_23_17_26_374 "\n http HTTP_TransCallback recv NETTRANS_CONNECT_IND timeout\n"
#define HTTP_HSM_889_112_2_17_23_17_26_375 "\n http HTTP_TransCallback recv NETTRANS_CONNECT_IND fail\n"
#define HTTP_HSM_894_112_2_17_23_17_26_376 "\n http HTTP_TransCallback recv NETTRANS_DATA_IN_IND\n"
#define HTTP_HSM_899_112_2_17_23_17_26_377 "\n http HTTP_TransCallback recv NETTRANS_DATA_IN_IND succ\n"
#define HTTP_HSM_903_112_2_17_23_17_26_378 "\n http TransCallback recv NETMGR_DATA_IN_IND error\n"
#define HTTP_HSM_907_112_2_17_23_17_26_379 "\n http HTTP_TransCallback recv NETTRANS_DATA_PROGRESS_IN_IND\n"
#define HTTP_HSM_912_112_2_17_23_17_26_380 "\n http HTTP_TransCallback recv NETTRANS_DATA_PROGRESS_IN_IND succ\n"
#define HTTP_HSM_916_112_2_17_23_17_26_381 "\n http TransCallback recv NETTRANS_DATA_PROGRESS_IN_IND error\n"
#define HTTP_HSM_920_112_2_17_23_17_26_382 "\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND\n"
#define HTTP_HSM_925_112_2_17_23_17_26_383 "\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND succ\n"
#define HTTP_HSM_934_112_2_17_23_17_26_384 "\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND succ len < 0\n"
#define HTTP_HSM_945_112_2_17_23_17_26_385 "\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND fail\n"
#define HTTP_HSM_951_112_2_17_23_17_26_386 "\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND timeout\n"
#define HTTP_HSM_955_112_2_17_23_17_26_387 "\n http TransCallback recv NETMGR_SEND_RETURN_IND error\n"
#define HTTP_HSM_960_112_2_17_23_17_26_388 "\n http HTTP_TransCallback recv NETTRANS_RECV_RETURN_IND\n"
#define HTTP_HSM_965_112_2_17_23_17_26_389 "\n http HTTP_TransCallback recv NETTRANS_RECV_RETURN_IND succ\n"
#define HTTP_HSM_974_112_2_17_23_17_26_390 "\n http HTTP_TransCallback recv NETTRANS_RECV_RETURN_IND succ len < 0\n"
#define HTTP_HSM_985_112_2_17_23_17_26_391 "\n http HTTP_TransCallback recv NETTRANS_RECV_RETURN_IND fail\n"
#define HTTP_HSM_989_112_2_17_23_17_26_392 "\n http TransCallback recv NETMGR_RECV_RETURN_IND error\n"
#define HTTP_HSM_996_112_2_17_23_17_26_393 "\n http HTTP_TransCallback recv NETTRANS_NORMAL_DISCONNECT_IND\n"
#define HTTP_HSM_1000_112_2_17_23_17_27_394 "\n http HTTP_TransCallback recv NETTRANS_EXCEPTION_DISCONNECT_IND\n"
#define HTTP_HSM_1004_112_2_17_23_17_27_395 "\n http TransCallback default error\n"
#define HTTP_HSM_1055_112_2_17_23_17_27_396 "\n HTTP QHSM_Http receive time out signal \n"
#define HTTP_HSM_1059_112_2_17_23_17_27_397 "\n http delete timer"
#define HTTP_HSM_1065_112_2_17_23_17_27_398 "\n http delete progress timer "
#define HTTP_HSM_1091_112_2_17_23_17_27_399 "\n http QHSM_Http HTTP_TIMEOUT HttpCacheDelete fail"
#define HTTP_HSM_1099_112_2_17_23_17_27_400 "\n HTTP QHSM_Http receive server exception close signal \n"
#define HTTP_HSM_1103_112_2_17_23_17_27_401 "\n http delete timer"
#define HTTP_HSM_1109_112_2_17_23_17_27_402 "\n http delete progress timer"
#define HTTP_HSM_1136_112_2_17_23_17_27_403 "\n http QHSM_Http HTTP_SERVER_EXCEPTION_CLOSE HttpCacheDelete fail"
#define HTTP_HSM_1145_112_2_17_23_17_27_404 "\n HTTP QHSM_Http receive cancel signal \n"
#define HTTP_HSM_1149_112_2_17_23_17_27_405 "\n http delete timer"
#define HTTP_HSM_1155_112_2_17_23_17_27_406 "\n http delete progress timer"
#define HTTP_HSM_1201_112_2_17_23_17_27_407 "\n http QHSM_Http HTTP_SIG_CANCEL_REQ HttpCacheDelete fail"
#define HTTP_HSM_1210_112_2_17_23_17_27_408 "\n HTTP QHSM_Http receive error signal \n"
#define HTTP_HSM_1214_112_2_17_23_17_27_409 "\n http delete timer"
#define HTTP_HSM_1220_112_2_17_23_17_27_410 "\n http delete progress timer"
#define HTTP_HSM_1246_112_2_17_23_17_27_411 "\n http QHSM_Http HTTP_ERROR HttpCacheDelete fail"
#define HTTP_HSM_1258_112_2_17_23_17_27_412 "\n http delete timer"
#define HTTP_HSM_1264_112_2_17_23_17_27_413 "\n http delete progress timer"
#define HTTP_HSM_1272_112_2_17_23_17_27_414 "\n HTTP QHSM_Http receive ssl cancel signal \n"
#define HTTP_HSM_1276_112_2_17_23_17_27_415 "\n http delete timer"
#define HTTP_HSM_1282_112_2_17_23_17_27_416 "\n http delete progress timer"
#define HTTP_HSM_1308_112_2_17_23_17_27_417 "\n http QHSM_Http HTTP_SSL_CANCEL HttpCacheDelete fail"
#define HTTP_HSM_1373_112_2_17_23_17_27_418 "\n HTTP Http_Idle receive connect server signal \n"
#define HTTP_HSM_1389_112_2_17_23_17_27_419 "\n HTTP QHSM_Http receive server close signal \n"
#define HTTP_HSM_1458_112_2_17_23_17_28_420 "\n http Http_Idle send HTTP_SIG_GET_CNF"
#define HTTP_HSM_1474_112_2_17_23_17_28_421 "\n http Http_Idle redirect error \n"
#define HTTP_HSM_1684_112_2_17_23_17_28_422 "\n http Http_Idle send HTTP_SIG_GET_CNF"
#define HTTP_HSM_1719_112_2_17_23_17_28_423 "\n http Http_Idle HTTP_ERROR_SERVER_CLOSE HttpCacheDelete fail"
#define HTTP_HSM_1728_112_2_17_23_17_28_424 "\n http Http_Idle HTTP_SERVER_CLOSE"
#define HTTP_HSM_1881_112_2_17_23_17_28_425 "\n HTTP handle cache error"
#define HTTP_HSM_1901_112_2_17_23_17_28_426 "\n HTTP Http_Connecting receive connect succ signal \n"
#define HTTP_HSM_1915_112_2_17_23_17_29_427 "\n HTTP Http_Connecting receive connect fail or server close signal \n"
#define HTTP_HSM_1961_112_2_17_23_17_29_428 "\n HTTP Http_Connected receive handshake signal \n"
#define HTTP_HSM_1974_112_2_17_23_17_29_429 "\n HTTP Http_Connecting receive proxy connect signal \n"
#define HTTP_HSM_1987_112_2_17_23_17_29_430 "\n HTTP Http_Connecting receive send data signal \n"
#define HTTP_HSM_2040_112_2_17_23_17_29_431 "\n http delete handshake timer"
#define HTTP_HSM_2047_112_2_17_23_17_29_432 "\n http create handshake timer"
#define HTTP_HSM_2054_112_2_17_23_17_29_433 "\n http delete handshake timer"
#define HTTP_HSM_2062_112_2_17_23_17_29_434 "\n http delete handshake timer"
#define HTTP_HSM_2069_112_2_17_23_17_29_435 "\n HTTP Http_Handshaking receive read data signal \n"
#define HTTP_HSM_2078_112_2_17_23_17_29_436 "\n HTTP Http_Handshaking receive ssl cert ind signal \n"
#define HTTP_HSM_2082_112_2_17_23_17_29_437 "\n http delete handshake timer"
#define HTTP_HSM_2093_112_2_17_23_17_29_438 "\n HTTP Http_Handshaking receive ssl cert rsp signal \n"
#define HTTP_HSM_2097_112_2_17_23_17_29_439 "\n http delete handshake timer"
#define HTTP_HSM_2104_112_2_17_23_17_29_440 "\n http create handshake timer"
#define HTTP_HSM_2117_112_2_17_23_17_29_441 "\n HTTP Http_Handshaking receive handshake succ signal \n"
#define HTTP_HSM_2130_112_2_17_23_17_29_442 "\n HTTP Http_Handshaking receive handshake fail signal \n"
#define HTTP_HSM_2139_112_2_17_23_17_29_443 "\n HTTP Http_Handshaking receive read succ signal \n"
#define HTTP_HSM_2148_112_2_17_23_17_29_444 "\n HTTP Http_Handshaking receive read fail signal \n"
#define HTTP_HSM_2157_112_2_17_23_17_29_445 "\n HTTP Http_Handshaking receive send succ signal \n"
#define HTTP_HSM_2167_112_2_17_23_17_29_446 "\n HTTP Http_Handshaking receive HTTP_SERVER_CLOSE signal \n"
#define HTTP_HSM_2180_112_2_17_23_17_29_447 "\n HTTP Http_Handshaking error handle request=%d \n"
#define HTTP_HSM_2183_112_2_17_23_17_29_448 "\n HTTP Http_Handshaking not in idle \n"
#define HTTP_HSM_2189_112_2_17_23_17_29_449 "\n http error handle delete handshake timer"
#define HTTP_HSM_2194_112_2_17_23_17_29_450 "\n HTTP Http_Handshaking request \n"
#define HTTP_HSM_2223_112_2_17_23_17_29_451 "\n http Http_ProxyHandshake delete handshake timer"
#define HTTP_HSM_2230_112_2_17_23_17_29_452 "\n http Http_ProxyHandshake create handshake timer"
#define HTTP_HSM_2237_112_2_17_23_17_29_453 "\n http Http_ProxyHandshake delete handshake timer"
#define HTTP_HSM_2245_112_2_17_23_17_29_454 "\n http Http_ProxyHandshake delete handshake timer"
#define HTTP_HSM_2252_112_2_17_23_17_29_455 "\n HTTP Http_ProxyHandshake receive read data signal \n"
#define HTTP_HSM_2261_112_2_17_23_17_29_456 "\n HTTP Http_ProxyHandshake receive proxy connect succ signal \n"
#define HTTP_HSM_2278_112_2_17_23_17_29_457 "\n HTTP Http_ProxyHandshake receive proxy connect fail signal \n"
#define HTTP_HSM_2287_112_2_17_23_17_29_458 "\n HTTP Http_ProxyHandshake receive read succ signal \n"
#define HTTP_HSM_2296_112_2_17_23_17_29_459 "\n HTTP Http_ProxyHandshake receive read fail signal \n"
#define HTTP_HSM_2349_112_2_17_23_17_29_460 "\n http Http_ProxyHandshake error handle delete handshake timer"
#define HTTP_HSM_2379_112_2_17_23_17_29_461 "\n HTTP Http_Sending receive send succ signal \n"
#define HTTP_HSM_2388_112_2_17_23_17_29_462 "\n HTTP Http_Sending receive send fail signal \n"
#define HTTP_HSM_2397_112_2_17_23_17_29_463 "\n HTTP Http_Sending receive ssl send end signal \n"
#define HTTP_HSM_2406_112_2_17_23_17_29_464 "\n HTTP Http_Sending receive HTTP_SERVER_CLOSE signal \n"
#define HTTP_HSM_2501_112_2_17_23_17_30_465 "\n http delete timer"
#define HTTP_HSM_2508_112_2_17_23_17_30_466 "\n http create timer"
#define HTTP_HSM_2513_112_2_17_23_17_30_467 "\n HTTP Http_ReceivingRecv receive read data signal \n"
#define HTTP_HSM_2518_112_2_17_23_17_30_468 "\n http delete timer"
#define HTTP_HSM_2548_112_2_17_23_17_30_469 "\n http delete recv progress timer"
#define HTTP_HSM_2562_112_2_17_23_17_30_470 "\n HTTP Http_ReceivingRecv receive read succ signal \n"
#define HTTP_HSM_2572_112_2_17_23_17_30_471 "HTTP Http_ReceivingRecv read success flag = true"
#define HTTP_HSM_2577_112_2_17_23_17_30_472 "\n HTTP Http_ReceivingRecv receive read fail signal \n"
#define HTTP_HSM_2581_112_2_17_23_17_30_473 "\n http delete timer"
#define HTTP_HSM_2587_112_2_17_23_17_30_474 "\n http delete progress timer"
#define HTTP_HSM_2606_112_2_17_23_17_30_475 "\n HTTP Http_ReceivingRecv receive time out signal \n"
#define HTTP_HSM_2610_112_2_17_23_17_30_476 "\n http delete timer"
#define HTTP_HSM_2616_112_2_17_23_17_30_477 "\n http delete progress timer"
#define HTTP_HSM_2637_112_2_17_23_17_30_478 "HTTP Http_ReceivingRecv HTTP_SERVER_CLOSE"
#define HTTP_HSM_2640_112_2_17_23_17_30_479 "HTTP Http_ReceivingRecv read event = %d"
#define HTTP_HSM_2643_112_2_17_23_17_30_480 "HTTP Http_ReceivingRecv read succ is in idle=%d"
#define HTTP_HSM_2677_112_2_17_23_17_30_481 "\n http delete timer"
#define HTTP_HSM_2683_112_2_17_23_17_30_482 "\n http delete progress timer"
#define HTTP_HSM_2702_112_2_17_23_17_30_483 "http Http_ReceivingRecv cache delete"
#define HTTP_HSM_2730_112_2_17_23_17_30_484 "\n HTTP Http_ReceivingAuthPending receive need auth rsp signal \n"
#define HTTP_HSM_2748_112_2_17_23_17_30_485 "\n HTTP Http_ReceivingAuthPending recieve server close signal \n"
#define HTTP_HSM_2805_112_2_17_23_17_30_486 "\n HTTP Http_ReceivingHeaderRspPending receive read data signal \n"
#define HTTP_HSM_2822_112_2_17_23_17_30_487 "\n HTTP Http_ReceivingHeaderRspPending receive server close signal \n"
#define HTTP_HSM_2841_112_2_17_23_17_30_488 "\n HTTP Http_ReceivingHeaderRspPending receive header rsp signal \n"
#define HTTP_HSM_2892_112_2_17_23_17_30_489 "\n http Http_ReceivingHeaderRspPending no read and close flag\n"
#define HTTP_HSM_2919_112_2_17_23_17_30_490 "http Http_ReceivingHeaderRspPending cache delete"
#define HTTP_HSM_2943_112_2_17_23_17_31_491 "\n http HandleIdleIdleReq input error\n"
#define HTTP_HSM_2947_112_2_17_23_17_31_492 "\n http HandleIdleIdleReq request_ptr=%x\n"
#define HTTP_HSM_2957_112_2_17_23_17_31_493 "HTTP HandleIdleIdleReq uri =%s"
#define HTTP_HSM_2963_112_2_17_23_17_31_494 "\n http HandleIdleIdleReq 1 input error\n"
#define HTTP_HSM_2975_112_2_17_23_17_31_495 "\n http HandleIdleIdleReq 2 input error\n"
#define HTTP_HSM_3004_112_2_17_23_17_31_496 "\n http HandleIdleIdleReq file open error\n"
#define HTTP_HSM_3058_112_2_17_23_17_31_497 "\n http HandleIdleIdleReq error code = %d\n"
#define HTTP_HSM_3104_112_2_17_23_17_31_498 "\n http HandleCacheDataToApp 1 input error \n"
#define HTTP_HSM_3113_112_2_17_23_17_31_499 "\n http HandleCacheDataToApp 2 input error \n"
#define HTTP_HSM_3129_112_2_17_23_17_31_500 "\n http HandleCacheDataToApp file not found error \n"
#define HTTP_HSM_3138_112_2_17_23_17_31_501 "\n http HandleCacheDataToApp get file size error \n"
#define HTTP_HSM_3146_112_2_17_23_17_31_502 "\n http HandleCacheDataToApp get file size error \n"
#define HTTP_HSM_3165_112_2_17_23_17_31_503 "\n http HandleCacheDataToApp alloc error \n"
#define HTTP_HSM_3178_112_2_17_23_17_31_504 "\n http HandleCacheDataToApp file read error \n"
#define HTTP_HSM_3203_112_2_17_23_17_31_505 "\n http HandleIdleCacheReadSucc free content\n"
#define HTTP_HSM_3215_112_2_17_23_17_31_506 "\n http HandleIdleCacheReadSucc content_ptr =%d\n"
#define HTTP_HSM_3225_112_2_17_23_17_31_507 "\n http HandleIdleCacheReadSucc response_ptr->content_total_len=%d"
#define HTTP_HSM_3319_112_2_17_23_17_31_508 "\n http HandleHttpCacheData parse header fail"
#define HTTP_HSM_3322_112_2_17_23_17_31_509 "\n HTTP HandleHttpCacheData cache size = %d,header len =%d"
#define HTTP_HSM_3332_112_2_17_23_17_31_510 "\nHTTP HandleHttpCacheData cache data error"
#define HTTP_HSM_3340_112_2_17_23_17_31_511 "\n http HandleHttpCacheData state code=%d"
#define HTTP_HSM_3347_112_2_17_23_17_31_512 "\nHTTP HandleHttpCacheData cache data error"
#define HTTP_HSM_3366_112_2_17_23_17_31_513 "\n http HandleHttpRecvData HTTP_ERROR_FILE_OPEN"
#define HTTP_HSM_3374_112_2_17_23_17_31_514 "\n http HandleHttpRecvData HTTP_ERROR_FILE_WRITE"
#define HTTP_HSM_3420_112_2_17_23_17_31_515 "\n http HandleHttpRecvData HTTP_ERROR_FILE_OPEN"
#define HTTP_HSM_3428_112_2_17_23_17_31_516 "\n http HandleHttpRecvData HTTP_ERROR_FILE_WRITE"
#define HTTP_HSM_3487_112_2_17_23_17_32_517 "\n http HandleHttpRecvData error_code=%d"
#define HTTP_HSM_3584_112_2_17_23_17_32_518 "\n http HandleReceivingHeaderRspPendingHeaderRsp HTTP_ERROR_FILE_OPEN"
#define HTTP_HSM_3649_112_2_17_23_17_32_519 "\n http HandleHttpContent content len = 0"
#define HTTP_HSM_3664_112_2_17_23_17_32_520 "HTTP HandleHttpCacheContent sfs_error_code = %d"
#define HTTP_HSM_3672_112_2_17_23_17_32_521 "\n http HandleHttpContent file write error\n"
#define HTTP_HSM_3688_112_2_17_23_17_32_522 "HTTP HandleHttpCacheContent free content"
#define HTTP_HSM_3693_112_2_17_23_17_32_523 "HTTP HandleHttpCacheContent response_ptr->rsp_header_info.content_length = %d,response_ptr->content_total_len = %d"
#define HTTP_HSM_3713_112_2_17_23_17_32_524 "\n http HandleHttpContent send HTTP_SIG_GET_CNF"
#define HTTP_HSM_3988_112_2_17_23_17_33_525 "\n HTTP HandleConnectedProxyConn port = %d"
#define HTTP_HSM_3995_112_2_17_23_17_33_526 "\n HTTP HandleConnectedProxyConn ssl port"
#define HTTP_HSM_4280_112_2_17_23_17_33_527 "\n http delete timer"
#define HTTP_HSM_4286_112_2_17_23_17_33_528 "\n http delete progress timer"
#define HTTP_HSM_4355_112_2_17_23_17_33_529 "\n http delete timer"
#define HTTP_HSM_4361_112_2_17_23_17_33_530 "\n http delete progress timer"
#define HTTP_HSM_4396_112_2_17_23_17_33_531 "\n http delete timer"
#define HTTP_HSM_4402_112_2_17_23_17_33_532 "\n http delete progress timer"
#define HTTP_HSM_4486_112_2_17_23_17_34_533 "HTTP proxy connect no memory len = %d"
#define HTTP_HSM_4509_112_2_17_23_17_34_534 "HTTP proxy connect no memory 1 len=%d"
#define HTTP_HSM_4523_112_2_17_23_17_34_535 "HTTP connect header received"
#define HTTP_HSM_4528_112_2_17_23_17_34_536 "HTTP connect header not received all"
#define HTTP_HSM_4538_112_2_17_23_17_34_537 "HTTP proxy connect succ"
#define HTTP_HSM_4640_112_2_17_23_17_34_538 "\n http delete timer"
#define HTTP_HSM_4646_112_2_17_23_17_34_539 "\n http delete progress timer"
#define HTTP_HSM_4688_112_2_17_23_17_34_540 "\n http delete timer"
#define HTTP_HSM_4694_112_2_17_23_17_34_541 "\n http delete progress timer"
#define HTTP_HSM_4783_112_2_17_23_17_34_542 "HTTP HandleSendingSendSucc trans Http_ReceivingRecv"
#define HTTP_HSM_4881_112_2_17_23_17_34_543 "HTTP HandleReceivingRecvReadSucc free content 1"
#define HTTP_HSM_4900_112_2_17_23_17_34_544 "HTTP HandleReceivingRecvReadSucc content_ptr = %d,buf_len =%d"
#define HTTP_HSM_4916_112_2_17_23_17_34_545 "\n http HandleReceivingRecvReadSucc response_ptr->content_total_len=%d"
#define HTTP_HSM_5019_112_2_17_23_17_35_546 "HandleHttpRecvData input invalid"
#define HTTP_HSM_5064_112_2_17_23_17_35_547 "\n http HandleHttpRecvData state code=%d disconnect socket"
#define HTTP_HSM_5068_112_2_17_23_17_35_548 "\n http HandleHttpRecvData state code=%d"
#define HTTP_HSM_5075_112_2_17_23_17_35_549 "\n http delete timer"
#define HTTP_HSM_5081_112_2_17_23_17_35_550 "\n http create timer"
#define HTTP_HSM_5101_112_2_17_23_17_35_551 "\n http HandleHttpRecvData HTTP_ERROR_FILE_OPEN"
#define HTTP_HSM_5109_112_2_17_23_17_35_552 "\n http HandleHttpRecvData HTTP_ERROR_FILE_WRITE"
#define HTTP_HSM_5181_112_2_17_23_17_35_553 "\n http Http_Idle send HTTP_SIG_GET_CNF"
#define HTTP_HSM_5196_112_2_17_23_17_35_554 "\n http HandleHttpRecvData redirect"
#define HTTP_HSM_5290_112_2_17_23_17_35_555 "\n http HandleHttpRecvData HTTP_ERROR_FILE_OPEN"
#define HTTP_HSM_5298_112_2_17_23_17_35_556 "\n http HandleHttpRecvData HTTP_ERROR_FILE_WRITE"
#define HTTP_HSM_5342_112_2_17_23_17_35_557 "\n http HandleHttpRecvData HTTP_RESPONSE_CODE_NOT_MODIFIED error_code=%d"
#define HTTP_HSM_5391_112_2_17_23_17_35_558 "\n http HandleHttpRecvData error_code=%d"
#define HTTP_HSM_5418_112_2_17_23_17_35_559 "http HandleHttpContent input invalid"
#define HTTP_HSM_5447_112_2_17_23_17_35_560 "\n http HandleHttpContent content len = 0"
#define HTTP_HSM_5456_112_2_17_23_17_35_561 "\n http HandleHttpContent HTTP_TRANSFER_ENCODING_CHUNKED chunk decode error_code=%d"
#define HTTP_HSM_5467_112_2_17_23_17_35_562 "HTTP HandleHttpContent free content 1"
#define HTTP_HSM_5470_112_2_17_23_17_35_563 "http HandleHttpContent HTTP_ALLOC no memory"
#define HTTP_HSM_5483_112_2_17_23_17_36_564 "HTTP HandleHttpContent free content 2"
#define HTTP_HSM_5488_112_2_17_23_17_36_565 "http HandleHttpContent CFL_ReAlloc no memory"
#define HTTP_HSM_5501_112_2_17_23_17_36_566 "HTTP HandleHttpContent free content 3"
#define HTTP_HSM_5510_112_2_17_23_17_36_567 "HandleHttpContent(1) http delete timer"
#define HTTP_HSM_5515_112_2_17_23_17_36_568 " HandleHttpContent(1) http create timer"
#define HTTP_HSM_5531_112_2_17_23_17_36_569 "HTTP HandleHttpContent free content 4"
#define HTTP_HSM_5534_112_2_17_23_17_36_570 "http HandleHttpContent HTTP_ALLOC no memory"
#define HTTP_HSM_5547_112_2_17_23_17_36_571 "HandleHttpContent style=%d"
#define HTTP_HSM_5562_112_2_17_23_17_36_572 "\n http HandleHttpContent file write error\n"
#define HTTP_HSM_5589_112_2_17_23_17_36_573 "n http HandleHttpContent response_ptr->rsp_header_info.content_length=%d,response_ptr->content_total_len=%d"
#define HTTP_HSM_5617_112_2_17_23_17_36_574 "\n http HandleHttpContent request_ptr->net_request_handle = %d,request_ptr->uri_str_ptr =%s\n"
#define HTTP_HSM_5654_112_2_17_23_17_36_575 "\n http Http_Idle send HTTP_SIG_GET_CNF"
#define HTTP_HSM_5671_112_2_17_23_17_36_576 "\n http HandleHttpContent redirect error \n"
#define HTTP_HSM_5678_112_2_17_23_17_36_577 "HandleHttpContent response_ptr->rsp_header_info.response_code=%d "
#define HTTP_HSM_5876_112_2_17_23_17_36_578 "\n http HandleHttpContent send HTTP_SIG_GET_CNF"
#define HTTP_HSM_5908_112_2_17_23_17_36_579 "HandleHttpContent(2)  http delete timer"
#define HTTP_HSM_5914_112_2_17_23_17_36_580 "HandleHttpContent(2)  http create timer"
#define HTTP_HSM_5967_112_2_17_23_17_36_581 "HTTP HandleDataFromCache free content 1"
#define HTTP_HSM_6009_112_2_17_23_17_37_582 "\n http HandleDataFromCache error_code=%d"
#define HTTP_HSM_6157_112_2_17_23_17_37_583 "\n http HandleReceivingHeaderRspPendingHeaderRsp HTTP_ERROR_FILE_OPEN"
#define HTTP_HSM_6200_112_2_17_23_17_37_584 "\n http HttpSecurityPostMessage recv SSL_SEND_MESSAGE_HANDSHAKE_SUCC\n"
#define HTTP_HSM_6205_112_2_17_23_17_37_585 "\n http HttpSecurityPostMessage recv SSL_SEND_MESSAGE_FAIL\n"
#define HTTP_HSM_6210_112_2_17_23_17_37_586 "\n http HttpSecurityPostMessage recv SSL_SEND_MESSAGE_CLOSE_BY_SERVER\n"
#define HTTP_HSM_6215_112_2_17_23_17_37_587 "\n http HttpSecurityPostMessage recv SSL_SEND_MESSAGE_SEND_END\n"
#define HTTP_HSM_6222_112_2_17_23_17_37_588 "\n http HttpSecurityPostMessage recv error\n"
#define HTTP_HSM_6262_112_2_17_23_17_37_589 "\n http HttpSecurityRecvDataOut free content 1"
#define HTTP_HSM_6275_112_2_17_23_17_37_590 "\n http HttpSecurityRecvDataOut content_ptr =%d"
#define HTTP_HSM_6291_112_2_17_23_17_37_591 "\n http HttpSecurityRecvDataOut response_ptr->content_total_len=%d"
#define HTTP_HSM_6320_112_2_17_23_17_37_592 "http HttpSecurityRecvDataOut cache delete"
#define HTTP_HSM_6391_112_2_17_23_17_37_593 "\n http HTTP_DealSignal recv HTTP_SIG_INIT_REQ \n"
#define HTTP_HSM_6396_112_2_17_23_17_37_594 "\n http HTTP_DealSignal recv HTTP_SIG_CANCEL_REQ \n"
#define HTTP_HSM_6401_112_2_17_23_17_37_595 "\n http HTTP_DealSignal recv HTTP_SIG_CLOSE_REQ \n"
#define HTTP_HSM_6406_112_2_17_23_17_37_596 "\n http HTTP_DealSignal recv HTTP_SIG_NEED_AUTH_RSP \n"
#define HTTP_HSM_6411_112_2_17_23_17_37_597 "\n http HTTP_DealSignal recv HTTP_SIG_HEADER_RSP \n"
#define HTTP_HSM_6416_112_2_17_23_17_37_598 "\n http HTTP_DealSignal recv HTTP_SIG_SSL_CERT_UNTRUST_RSP \n"
#define HTTP_HSM_6421_112_2_17_23_17_37_599 "\n http HTTP_DealSignal recv HTTP_SIG_TIMEOUT_IND \n"
#define HTTP_HSM_6425_112_2_17_23_17_37_600 "\n http HTTP_DealSignal recv HTTP_SIG_RECV_PROGRESS_TIMEOUT_IND \n"
#define HTTP_HSM_6429_112_2_17_23_17_37_601 "\n http HTTP_DealSignal recv HTTP_SIG_HANDSHAKE_TIMEOUT_IND \n"
#define HTTP_HSM_6447_112_2_17_23_17_37_602 "\n HTTP HTTP_DealSignal unknown signal\n"
#define HTTP_HSM_6624_112_2_17_23_17_38_603 "HTTP HTTP_CookieClear context_id =%d"
#define HTTP_HSM_6633_112_2_17_23_17_38_604 "HTTP HttpHandleCookieClearReq HttpCookieFinalize 1 error_code = %d"
#define HTTP_HSM_6642_112_2_17_23_17_38_605 "HTTP HttpHandleCookieClearReq HttpCookieFinalize 2 error_code = %d"
#define HTTP_HSM_6647_112_2_17_23_17_38_606 "HTTP HttpHandleCookieClearReq HttpCookieFinalize 3 error_code = %d"
#define HTTP_HSM_6651_112_2_17_23_17_38_607 "HTTP HttpHandleCookieClearReq HttpCookieFinalize 4 error_code = %d"
#define HTTP_HSM_6684_112_2_17_23_17_38_608 "HTTP HttpHandleCacheDeleteReq context_id =%d"
#define HTTP_HSM_6695_112_2_17_23_17_38_609 "\n http HttpHandleCacheDeleteReq alloc error\n"
#define HTTP_HSM_6715_112_2_17_23_17_38_610 "\n http HttpHandleCacheDeleteReq file name error\n"
#define HTTP_HSM_6734_112_2_17_23_17_38_611 "\n http HttpHandleCacheDeleteReq HTTP_ERROR_FILE_OPEN"
#define HTTP_HSM_6772_112_2_17_23_17_38_612 "HTTP HttpHandleNeedAuthRsp 1"
#define HTTP_HSM_7189_112_2_17_23_17_39_613 "\n http http init\n"
#define HTTP_HSM_7273_112_2_17_23_17_39_614 "\n http HTTP_GetInitInfo proxy host111 = %s"
#define HTTP_HSM_7278_112_2_17_23_17_39_615 "\n http HTTP_GetInitInfo proxy password = %s"
#define HTTP_HSM_7282_112_2_17_23_17_39_616 "\n http HTTP_GetInitInfo proxy port =%d"
#define HTTP_HSM_7287_112_2_17_23_17_39_617 "\n http HTTP_GetInitInfo proxy user name = %s"
#define HTTP_HSM_7293_112_2_17_23_17_39_618 "HTTP_GetInitInfo"
#define HTTP_HSM_7354_112_2_17_23_17_39_619 "\n http HTTP_SigErrCnf HTTP_SIG_INIT_REQ\n"
#define HTTP_HSM_7359_112_2_17_23_17_39_620 "\n http HTTP_SigErrCnf HTTP_SIG_CANCEL_REQ\n"
#define HTTP_HSM_7364_112_2_17_23_17_39_621 "\n http HTTP_SigErrCnf HTTP_SIG_CLOSE_REQ\n"
#define HTTP_HSM_7369_112_2_17_23_17_39_622 "\n http HTTP_SigErrCnf HTTP_SIG_NEED_AUTH_RSP\n"
#define HTTP_HSM_7374_112_2_17_23_17_39_623 "\n http HTTP_SigErrCnf HTTP_SIG_HEADER_RSP\n"
#define HTTP_HSM_7390_112_2_17_23_17_39_624 "\n http HTTP_SigErrCnf HTTP_SIG_SSL_CERT_UNTRUST_RSP\n"
#define HTTP_HSM_7396_112_2_17_23_17_39_625 "\n http HTTP_SigErrCnf HTTP_SIG_GET_REQ\n"
#define HTTP_HSM_7401_112_2_17_23_17_39_626 "\n http HTTP_SigErrCnf HTTP_SIG_HEAD_REQ\n"
#define HTTP_HSM_7406_112_2_17_23_17_39_627 "\n http HTTP_SigErrCnf HTTP_SIG_POST_REQ\n"
#define HTTP_HSM_7411_112_2_17_23_17_39_628 "\n HTTP HTTP_SigErrCnf unknown signal \n"
#define HTTP_REQUEST_116_112_2_17_23_17_40_629 "\n http HTTP_CreateRequest net_request_handle =%x\n"
#define HTTP_REQUEST_119_112_2_17_23_17_40_630 "\n http HTTP_CreateRequest input error\n"
#define HTTP_REQUEST_139_112_2_17_23_17_40_631 "\n http hsm = %d\n"
#define HTTP_REQUEST_151_112_2_17_23_17_40_632 "\n http HTTP_CreateRequest HTTP_SIG_GET_REQ\n"
#define HTTP_REQUEST_162_112_2_17_23_17_40_633 "\n http HTTP_CreateRequest HTTP_SIG_POST_REQ\n"
#define HTTP_REQUEST_173_112_2_17_23_17_40_634 "\n http HTTP_CreateRequest HTTP_SIG_HEAD_REQ\n"
#define HTTP_REQUEST_183_112_2_17_23_17_40_635 "\n HTTP HTTP_CreateRequest unknown request\n"
#define HTTP_REQUEST_196_112_2_17_23_17_40_636 "\n HTTP HTTP_CreateRequest request id = %d,id address =%d\n"
#define HTTP_REQUEST_216_112_2_17_23_17_40_637 "\n http HTTP_DestroyRequest input error\n"
#define HTTP_REQUEST_223_112_2_17_23_17_40_638 "\n http HTTP_DestroyRequest 1 input error\n"
#define HTTP_REQUEST_333_112_2_17_23_17_41_639 "HTTP_DestroyRequest request_id =%d,id address =%d"
#define HTTP_REQUEST_435_112_2_17_23_17_41_640 "http CreateGetRequest scheme=%s"
#define HTTP_REQUEST_566_112_2_17_23_17_41_641 "http CreatePostRequest scheme=%s"
#define HTTP_REQUEST_677_112_2_17_23_17_41_642 "\n http CreatePostRequest post file \n"
#define HTTP_REQUEST_692_112_2_17_23_17_41_643 "\n http CreatePostRequest post buffer copy \n"
#define HTTP_REQUEST_697_112_2_17_23_17_41_644 "\n http CreatePostRequest post buffer not copy \n"
#define HTTP_REQUEST_702_112_2_17_23_17_41_645 "\n http CreatePostRequest post buffer null \n"
#define HTTP_REQUEST_740_112_2_17_23_17_41_646 "http CreateHeadRequest scheme=%s"
#define HTTP_REQUEST_894_112_2_17_23_17_42_647 "\n http SaveAuthParam error_code=%d"
#define HTTP_REQUEST_946_112_2_17_23_17_42_648 "\n HTTP HttpRequestFormat 1 port = %d"
#define HTTP_REQUEST_954_112_2_17_23_17_42_649 "\n HTTP HTTPRequestFormat malloc failed\n"
#define HTTP_REQUEST_1448_112_2_17_23_17_43_650 "\n HTTP HttpRequestFormat port = %d"
#define HTTP_REQUEST_1542_112_2_17_23_17_43_651 "\n HTTP HTTPRequestFormat, need to proxyAuthenticate\n"
#define HTTP_REQUEST_1562_112_2_17_23_17_43_652 "\n HTTP Proxy_Authentication get is:%s\n"
#define HTTP_REQUEST_1570_112_2_17_23_17_43_653 "\n HTTP HTTPRequestFormat, need not to proxyAuthenticate\n"
#define HTTP_REQUEST_1599_112_2_17_23_17_43_654 "\n HTTP HttpRequestFormat ETAG get is:%s"
#define HTTP_REQUEST_1620_112_2_17_23_17_43_655 "\n HTTP HttpRequestFormat last modified get is:%s"
#define HTTP_REQUEST_1630_112_2_17_23_17_43_656 "\n HTTP HTTPRequestFormat, need to wwwAuthenticate\n"
#define HTTP_REQUEST_1652_112_2_17_23_17_43_657 "\n HTTP HTTPRequestFormat, need not to wwwAuthenticate\n"
#define HTTP_REQUEST_1736_112_2_17_23_17_43_658 "http HttpRequestFormat realloc error"
#define HTTP_REQUEST_1769_112_2_17_23_17_43_659 "\n HTTP HTTPRequestFormat out\n"
#define HTTP_REQUEST_1790_112_2_17_23_17_43_660 "\n http HTTP_UpdateRequest input error\n"
#define HTTP_REQUEST_1825_112_2_17_23_17_43_661 "\n http HTTP_UpdateRequest \n"
#define HTTP_REQUEST_1841_112_2_17_23_17_44_662 "\n http HttpTraceGetParam accept_ptr %s\n"
#define HTTP_REQUEST_1845_112_2_17_23_17_44_663 "\n http HttpTraceGetParam accept_charset_ptr %s\n"
#define HTTP_REQUEST_1847_112_2_17_23_17_44_664 "\n http HttpTraceGetParam accept_encoding  %d\n"
#define HTTP_REQUEST_1850_112_2_17_23_17_44_665 "\n http HttpTraceGetParam accept_language_ptr %s\n"
#define HTTP_REQUEST_1854_112_2_17_23_17_44_666 "\n http HttpTraceGetParam user_name_ptr %s\n"
#define HTTP_REQUEST_1858_112_2_17_23_17_44_667 "\n http HttpTraceGetParam password_ptr %s\n"
#define HTTP_REQUEST_1862_112_2_17_23_17_44_668 "\n http HttpTraceGetParam entity_ptr %s\n"
#define HTTP_REQUEST_1864_112_2_17_23_17_44_669 "\n http HttpTraceGetParam entity_len %d\n"
#define HTTP_REQUEST_1868_112_2_17_23_17_44_670 "\n http HttpTraceGetParamurl %s\n"
#define HTTP_REQUEST_1875_112_2_17_23_17_44_671 "\n http HttpTraceGetParam header_name_ptr %s\n"
#define HTTP_REQUEST_1879_112_2_17_23_17_44_672 "\n http HttpTraceGetParam header_value_ptr %s\n"
#define HTTP_REQUEST_1882_112_2_17_23_17_44_673 "\n http HttpTraceGetParam is_header_use_file %d\n"
#define HTTP_REQUEST_1883_112_2_17_23_17_44_674 "\n http HttpTraceGetParam get_data.style %d\n"
#define HTTP_REQUEST_1884_112_2_17_23_17_44_675 "\n http HttpTraceGetParam need_net_prog_ind %d\n"
#define HTTP_REQUEST_1887_112_2_17_23_17_44_676 "\n http HttpTraceGetParam referer_ptr %s\n"
#define HTTP_REQUEST_1891_112_2_17_23_17_44_677 "\n http HttpTraceGetParam ua_profile_ptr %s\n"
#define HTTP_REQUEST_1895_112_2_17_23_17_44_678 "\n http HttpTraceGetParam user_agent_ptr %s\n"
#define HTTP_REQUEST_1912_112_2_17_23_17_44_679 "\n http HttpTracePostParam accept_ptr %s\n"
#define HTTP_REQUEST_1916_112_2_17_23_17_44_680 "\n http HttpTracePostParam accept_charset_ptr %s\n"
#define HTTP_REQUEST_1918_112_2_17_23_17_44_681 "\n http HttpTracePostParam accept_encoding  %d\n"
#define HTTP_REQUEST_1921_112_2_17_23_17_44_682 "\n http HttpTracePostParam accept_language_ptr %s\n"
#define HTTP_REQUEST_1925_112_2_17_23_17_44_683 "\n http HttpTracePostParam user_name_ptr %s\n"
#define HTTP_REQUEST_1929_112_2_17_23_17_44_684 "\n http HttpTracePostParam password_ptr %s\n"
#define HTTP_REQUEST_1933_112_2_17_23_17_44_685 "\n http HttpTracePostParam entity_ptr %s\n"
#define HTTP_REQUEST_1935_112_2_17_23_17_44_686 "\n http HttpTracePostParam entity_len %d\n"
#define HTTP_REQUEST_1939_112_2_17_23_17_44_687 "\n http HttpTracePostParam uri %s\n"
#define HTTP_REQUEST_1946_112_2_17_23_17_44_688 "\n http HttpTracePostParam header_name_ptr %s\n"
#define HTTP_REQUEST_1950_112_2_17_23_17_44_689 "\n http HttpTracePostParam header_value_ptr %s\n"
#define HTTP_REQUEST_1953_112_2_17_23_17_44_690 "\n http HttpTracePostParam is_header_use_file %d\n"
#define HTTP_REQUEST_1954_112_2_17_23_17_44_691 "\n http HttpTracePostParam get_data.style %d\n"
#define HTTP_REQUEST_1955_112_2_17_23_17_44_692 "\n http HttpTracePostParam need_net_prog_ind %d\n"
#define HTTP_REQUEST_1958_112_2_17_23_17_44_693 "\n http HttpTracePostParam referer_ptr %s\n"
#define HTTP_REQUEST_1962_112_2_17_23_17_44_694 "\n http HttpTracePostParam ua_profile_ptr %s\n"
#define HTTP_REQUEST_1966_112_2_17_23_17_44_695 "\n http HttpTracePostParam user_agent_ptr %s\n"
#define HTTP_REQUEST_1970_112_2_17_23_17_44_696 "\n http HttpTracePostParam body_type_ptr %s\n"
#define HTTP_REQUEST_1972_112_2_17_23_17_44_697 "\n http HttpTracePostParam post_body.is_use_file %d\n"
#define HTTP_REQUEST_1988_112_2_17_23_17_44_698 "\n http HttpTraceHeadParam accept_ptr %s\n"
#define HTTP_REQUEST_1992_112_2_17_23_17_44_699 "\n http HttpTraceHeadParam accept_charset_ptr %s\n"
#define HTTP_REQUEST_1994_112_2_17_23_17_44_700 "\n http HttpTraceHeadParam accept_encoding  %d\n"
#define HTTP_REQUEST_1997_112_2_17_23_17_44_701 "\n http HttpTraceHeadParam accept_language_ptr %s\n"
#define HTTP_REQUEST_2001_112_2_17_23_17_44_702 "\n http HttpTraceHeadParam user_name_ptr %s\n"
#define HTTP_REQUEST_2005_112_2_17_23_17_44_703 "\n http HttpTraceHeadParam password_ptr %s\n"
#define HTTP_REQUEST_2009_112_2_17_23_17_44_704 "\n http HttpTraceHeadParam entity_ptr %s\n"
#define HTTP_REQUEST_2011_112_2_17_23_17_44_705 "\n http HttpTraceHeadParam entity_len %d\n"
#define HTTP_REQUEST_2016_112_2_17_23_17_44_706 "\n http HttpTraceHeadParam header_name_ptr %s\n"
#define HTTP_REQUEST_2020_112_2_17_23_17_44_707 "\n http HttpTraceHeadParam header_value_ptr %s\n"
#define HTTP_REQUEST_2023_112_2_17_23_17_44_708 "\n http HttpTraceHeadParam is_header_use_file %d\n"
#define HTTP_REQUEST_2024_112_2_17_23_17_44_709 "\n http HttpTraceHeadParam need_net_prog_ind %d\n"
#define HTTP_REQUEST_2027_112_2_17_23_17_44_710 "\n http HttpTraceHeadParam referer_ptr %s\n"
#define HTTP_REQUEST_2031_112_2_17_23_17_44_711 "\n http HttpTraceHeadParam ua_profile_ptr %s\n"
#define HTTP_REQUEST_2035_112_2_17_23_17_44_712 "\n http HttpTraceHeadParam user_agent_ptr %s\n"
#define HTTP_RESPONSE_174_112_2_17_23_17_44_713 "\n http HTTP_DestroyResponse free content 1"
#define HTTP_RESPONSE_327_112_2_17_23_17_45_714 "\n http HttpResponseParseHead input error\n"
#define HTTP_RESPONSE_401_112_2_17_23_17_45_715 "\n  HTTP HttpResponseParseHead parseheader error"
#define HTTP_RESPONSE_406_112_2_17_23_17_45_716 "\n  HTTP HttpResponseParseHead free content"
#define HTTP_RESPONSE_434_112_2_17_23_17_45_717 "\n http HttpResponseParseStateCode input error"
#define HTTP_RESPONSE_442_112_2_17_23_17_45_718 "\n http HttpResponseParseStateCode state code error"
#define HTTP_RESPONSE_594_112_2_17_23_17_46_719 "\n  HTTP ParseHttpHeader input error"
#define HTTP_RESPONSE_600_112_2_17_23_17_46_720 "\n  HTTP ParseHttpHeader separate error"
#define HTTP_RESPONSE_773_112_2_17_23_17_46_721 "\n HTTP HttpSeparate read line error"
#define HTTP_RESPONSE_961_112_2_17_23_17_46_722 "HttpParseResponseCacheHeader expired"
#define HTTP_RESPONSE_969_112_2_17_23_17_46_723 "\n HTTP HttpParseResponseCacheHeader unknown param \n"
#define HTTP_RESPONSE_1024_112_2_17_23_17_46_724 "\n http HttpParseResponseContentLengthHeader content len=%d"
#define HTTP_RESPONSE_1140_112_2_17_23_17_47_725 "HTTP HttpParseResponseSetCookieHeader URL scheme = %s,cookie path =%s "
#define HTTP_RESPONSE_1144_112_2_17_23_17_47_726 "HttpParseResponseSetCookieHeader path ptr=%s,fragment =%s, query =%s"
#define HTTP_RESPONSE_1151_112_2_17_23_17_47_727 "HttpParseResponseSetCookieHeader cookie node domain =%s,expire str =%s,secure =%d"
#define HTTP_RESPONSE_1155_112_2_17_23_17_47_728 "HttpParseResponseSetCookieHeader cookie node name =%s,value=%s,other name=%s,other v=%s"
#define HTTP_RESPONSE_1159_112_2_17_23_17_47_729 "HttpParseResponseSetCookieHeader tail =%d,path =%s,port=%d"
#define HTTP_RESPONSE_1337_112_2_17_23_17_47_730 "http HttpParseResponseLocationHeader value = %s"
#define HTTP_RESPONSE_1410_112_2_17_23_17_47_731 "\n http HttpParseResponseLocationHeader request_ptr->uri_str_ptr = %s"
#define HTTP_RESPONSE_1944_112_2_17_23_17_48_732 "HttpParseResponseExpiresHeader expires =%x,now = %x"
#define HTTP_RESPONSE_1947_112_2_17_23_17_48_733 "HttpParseResponseExpiresHeader need save"
#define HTTP_RESPONSE_1954_112_2_17_23_17_48_734 "HttpParseResponseExpiresHeader expired"
#define HTTP_SERIALIZE_1917_112_2_17_23_17_52_735 "\n http HttpSerializeHttpInitReqSigT error"
#define HTTP_SERIALIZE_2027_112_2_17_23_17_53_736 "\n http HttpSerializeHttpGetReqSigT error"
#define HTTP_SERIALIZE_2136_112_2_17_23_17_53_737 "\n http HttpSerializeHttpPostReqSigT error"
#define HTTP_SERIALIZE_2245_112_2_17_23_17_53_738 "\n http HttpSerializeHttpHeadReqSigT error"
#define HTTP_SERIALIZE_2535_112_2_17_23_17_54_739 "\n http HttpSerializeHttpAuthRspSigT error"
#define HTTP_SERIALIZE_2693_112_2_17_23_17_54_740 "\n http HttpSerializeHttpGetCnfSigT error"
#define HTTP_SERIALIZE_2806_112_2_17_23_17_54_741 "\n http HttpSerializeHttpPostCnfSigT error"
#define HTTP_SERIALIZE_2919_112_2_17_23_17_54_742 "\n http HttpSerializeHttpHeadCnfSigT error"
#define HTTP_SERIALIZE_3396_112_2_17_23_17_55_743 "\n http HttpSerializeHttpCookieSetReqSigT error"
#define HTTP_SERIALIZE_4207_112_2_17_23_17_57_744 "\n http HttpSerializeHttpSslGetCertCnfSigT error"
#define HTTP_SIGNAL_71_112_2_17_23_17_58_745 "\n http HttpSendNetIndToApp send HTTP_SIG_NET_PROG_IND request_id =%d,error code = %d"
#define HTTP_SIGNAL_110_112_2_17_23_17_59_746 "\n http HttpSendDataIndToApp send HTTP_SIG_DATA_IND request_id =%d,error code=%d"
#define HTTP_SIGNAL_148_112_2_17_23_17_59_747 "\n http HttpSendHeaderIndToApp send HTTP_SIG_HEADER_IND request_id =%d,error code=%d"
#define HTTP_SIGNAL_179_112_2_17_23_17_59_748 "\n http HttpSendHeadCnfToApp send HTTP_SIG_HEAD_CNF request_id =%d,error code=%d"
#define HTTP_SIGNAL_202_112_2_17_23_17_59_749 "\n http HttpSendErrorIndToApp error_code = %d"
#define HTTP_SIGNAL_210_112_2_17_23_17_59_750 "\n http HttpSendErrorIndToApp request_id =%d,err code=%d"
#define HTTP_SIGNAL_231_112_2_17_23_17_59_751 "\n http HttpSendCookieSetCnfToApp error_code = %d"
#define HTTP_SIGNAL_239_112_2_17_23_17_59_752 "\n http HttpSendCookieSetCnfToApp err code =%d"
#define HTTP_SIGNAL_270_112_2_17_23_17_59_753 "\n http HttpSendCancelCnfToApp send HTTP_SIG_CANCEL_CNF request_id =%d,err code=%d"
#define HTTP_SIGNAL_303_112_2_17_23_17_59_754 "\n http HttpSendGetCnfToApp send HTTP_SIG_GET_CNF request_id =%d,err code=%d"
#define HTTP_SIGNAL_336_112_2_17_23_17_59_755 "\n http HttpSendPostCnfToApp send HTTP_SIG_POST_CNF request_id =%d,err code=%d"
#define HTTP_SIGNAL_368_112_2_17_23_17_59_756 "\n http HttpSendNeedAuthToApp send HTTP_SIG_NEED_AUTH_IND request_id =%d,err code=%d"
#define HTTP_SIGNAL_402_112_2_17_23_17_59_757 "\n http HttpSendInitCnfToApp send HTTP_SIG_INIT_CNF context_id =%d,err code=%d"
#define HTTP_SIGNAL_428_112_2_17_23_17_59_758 "\n http HttpSendCloseCnfToApp send HTTP_SIG_CLOSE_CNF context_id =%d,err code=%d"
#define HTTP_SIGNAL_454_112_2_17_23_17_59_759 "\n http HttpSendClearCookieCnfToApp send HTTP_SIG_COOKIE_CLEAR_CNF err code=%d"
#define HTTP_SIGNAL_480_112_2_17_23_17_59_760 "\n http HttpSendCacheDeleteCnfToApp send HTTP_SIG_CACHE_DELETE_CNF, err code =%d"
#define HTTP_SIGNAL_509_112_2_17_23_18_0_761 "\n http HttpSendRequestIndToApp send HTTP_SIG_REQUEST_ID_IND request_id =%d,err code=%d"
#define HTTP_SIGNAL_542_112_2_17_23_18_0_762 "\n http HttpSendReadCacheResult send HTTP_SIG_READ_CACHE_SUCC or fail request_id =%d,err code =%d"
#define HTTP_SIGNAL_573_112_2_17_23_18_0_763 "\n http HttpSendSslCertIndToApp send HTTP_SIG_SSL_CERT_UNTRUST_IND request_id =%d,err code=%d"
#define HTTP_SIGNAL_610_112_2_17_23_18_0_764 "\n http HttpSendRedirectIndToApp send HTTP_SIG_REDIRECT_IND request_id =%d,err code=%d"
#define HTTP_TEST_88_112_2_17_23_18_0_765 "\n HTTP TEST no memory \n"
#define HTTP_TEST_181_112_2_17_23_18_0_766 "\n HTTP TEST no memory \n"
#define HTTP_TEST_267_112_2_17_23_18_0_767 "\n HTTP TEST no memory \n"
#define HTTP_TEST_306_112_2_17_23_18_1_768 "\n HTTP TEST no memory \n"
#define HTTP_TEST_416_112_2_17_23_18_1_769 "\n http test auth request id = %d\n"
#define HTTP_TEST_422_112_2_17_23_18_1_770 "\n http test request id = %d\n"
#define HTTP_TEST_426_112_2_17_23_18_1_771 "\n http test recv HTTP_SIG_GET_CNF\n"
#define HTTP_TEST_431_112_2_17_23_18_1_772 "\n http test recv HTTP_SIG_HEAD_CNF\n"
#define HTTP_TEST_437_112_2_17_23_18_1_773 "\n http test data ind rsp code=%d,request_id =%d\n"
#define HTTP_TEST_441_112_2_17_23_18_1_774 "\n http test recv HTTP_SIG_HEADER_IND\n"
#define HTTP_TEST_450_112_2_17_23_18_1_775 "\n http test recv HTTP_SIG_ERROR_IND, ERROR=%d\n"
#define HTTP_TEST_454_112_2_17_23_18_1_776 "\n http test recv HTTP_SIG_CLOSE_CNF\n"
#define HTTP_TEST_463_112_2_17_23_18_1_777 "\n http test net prog ind,%d,%d,%d\n"
#define HTTP_TEST_466_112_2_17_23_18_1_778 "\n http test redirect ind\n"
#define HTTP_TEST_469_112_2_17_23_18_1_779 "\n http test recv HTTP_SIG_POST_CNF\n"
#define HTTP_TEST_526_112_2_17_23_18_1_780 "\n http test auth request id = %d\n"
#define HTTP_TEST_532_112_2_17_23_18_1_781 "\n http test request id = %d\n"
#define HTTP_TEST_536_112_2_17_23_18_1_782 "\n http test recv HTTP_SIG_GET_CNF\n"
#define HTTP_TEST_541_112_2_17_23_18_1_783 "\n http test recv HTTP_SIG_HEAD_CNF\n"
#define HTTP_TEST_547_112_2_17_23_18_1_784 "\n http test data ind rsp code=%d,request_id =%d\n"
#define HTTP_TEST_551_112_2_17_23_18_1_785 "\n http test recv HTTP_SIG_HEADER_IND\n"
#define HTTP_TEST_560_112_2_17_23_18_1_786 "\n http test recv HTTP_SIG_ERROR_IND, ERROR=%d\n"
#define HTTP_TEST_564_112_2_17_23_18_1_787 "\n http test recv HTTP_SIG_CLOSE_CNF\n"
#define HTTP_TEST_573_112_2_17_23_18_1_788 "\n http test net prog ind,%d,%d,%d\n"
#define HTTP_TEST_576_112_2_17_23_18_1_789 "\n http test redirect ind\n"
#define HTTP_TEST_579_112_2_17_23_18_1_790 "\n http test recv HTTP_SIG_POST_CNF\n"
#define HTTP_UTIL_113_112_2_17_23_18_1_791 "\n HTTP HttpReadLine error"
#define HTTP_UTIL_124_112_2_17_23_18_2_792 "\n HTTP HttpReadLine succ"
#define HTTP_UTIL_816_112_2_17_23_18_3_793 "http HttpCookie_memInit %d"
#define HTTP_UTIL_844_112_2_17_23_18_3_794 "http HttpCookie_memExit %d"
#define RVADMUTEX_61_112_2_17_23_18_20_795 "RvAdMutexLock error."
#define RVADMUTEX_87_112_2_17_23_18_20_796 "RvAdMutexLock error."
#define RVSELECT_2674_112_2_17_23_19_0_797 "SIP:RvSelectWaitAndBlock out of blocking\n"
#define RVSELECT_2686_112_2_17_23_19_1_798 "SIP: RvSelectWaitAndBlock, recv_len == %d, socket recv error:%d\n"
#define RVSOCKET_1809_112_2_17_23_19_5_799 "SIP:RvSocketSendBuffer ourSocket=0x%x, bytesToSend=%d"
#define RTP_API_68_112_2_17_23_20_11_800 "RTP:RTP_CreateSession error_code =%d"
#define RTP_API_115_112_2_17_23_20_11_801 "RTP:RTP_GetSrrrSendingMode error_code =%d"
#define RTP_API_131_112_2_17_23_20_11_802 "RTP:RTP_GetSrrrSendingMode error_code =%d"
#define RTP_API_147_112_2_17_23_20_11_803 "RTP:RTP_SetSrrrReceivingMode error_code =%d"
#define RTP_API_163_112_2_17_23_20_11_804 "RTP:RTP_GetSrrrReceivingMode error_code =%d"
#define RTP_API_180_112_2_17_23_20_11_805 "RTP:RTP_SendRtcpSrrrReq, input error!"
#define RTP_API_187_112_2_17_23_20_11_806 "RTP:RTP_SendRtcpSrrrReq, no memory!"
#define RTP_API_199_112_2_17_23_20_11_807 "RTP:RTP_SendRtcpSrrrReq !"
#define RTP_API_217_112_2_17_23_20_11_808 "RTP:RTP_SendRtcpAppDataReq, input error!"
#define RTP_API_224_112_2_17_23_20_11_809 "RTP:RTP_SendRtcpAppDataReq, data length error!"
#define RTP_API_232_112_2_17_23_20_11_810 "RTP:RTP_SendRtcpAppDataReq, no memory!"
#define RTP_API_263_112_2_17_23_20_11_811 "RTP:RTP_SendRtcpAppDataReq !"
#define RTP_API_280_112_2_17_23_20_11_812 "RTP:RTP_GetRemoteAddressBySsrc error_code =%d"
#define RTP_API_295_112_2_17_23_20_11_813 "RTP:RTP_TerminateSession error_code =%d"
#define RTP_MGR_339_112_2_17_23_20_12_814 "RTP:, un-handled event:%d"
#define RTP_MGR_421_112_2_17_23_20_13_815 "RTP: GetNodeData, invalid timer node!"
#define RTP_MGR_738_112_2_17_23_20_13_816 "RTP:RTPMgr_Init RTP_InitStack failed\n"
#define RTP_MGR_744_112_2_17_23_20_13_817 "RTP:RTPMgr_Init RTCP_Init failed\n"
#define RTP_MGR_796_112_2_17_23_20_13_818 "RTP READ DATA LEN = %d"
#define RTP_MGR_1668_112_2_17_23_20_15_819 "RTP RTP_GetRemoteAddressBySsrcHandler rtcp port =%d"
#define SDP_API_438_112_2_17_23_20_17_820 "#SDP SDP_MediaDescrGetAttrValueByName error get attr_ptr at the number %d index"
#define SDP_API_485_112_2_17_23_20_17_821 "#SDP SDP_MsgGetAttrValueByName error get attr_ptr at the number %d index"
#define SIP_MGR_CALL_732_112_2_17_23_20_24_822 "SIP: SIPMGR_CallInviteReqHandler, error line = %d\n"
#define SIP_MGR_CALL_770_112_2_17_23_20_24_823 "SIP: SIPMGR_CallAckReqHandler, RvSipCallLegAck fail\n"
#define SIP_MGR_CALL_816_112_2_17_23_20_24_824 "SIP: SIPMGR_CallDisconnectReqHandler, error line = %d\n"
#define SIP_MGR_CALL_873_112_2_17_23_20_24_825 "SIP: SIPMGR_CallCancelReqHandler, error line = %d\n"
#define SIP_MGR_CALL_1022_112_2_17_23_20_24_826 "SIP:SIPMGR_CallInviteRspHandler, error line = %d"
#define SIP_MGR_CALL_1103_112_2_17_23_20_25_827 "SIP: CallLegStateRedirectedHandler, SIPMGR_GetIdByCallClientHandle fail\n"
#define SIP_MGR_CALL_1133_112_2_17_23_20_25_828 "SIP: CallLegStateRedirectedHandler, ConstructMsgHandleInCalllegHandle fail\n"
#define SIP_MGR_CALL_1140_112_2_17_23_20_25_829 "SIP: CallLegStateRedirectedHandler, RvSipCallLegConnect fail\n"
#define SIP_MGR_CALL_1161_112_2_17_23_20_25_830 "SIP: CallLegStateRedirectedHandler, RvSipCallLegDisconnect fail\n"
#define SIP_MGR_CALL_1170_112_2_17_23_20_25_831 "SIP: CallLegStateRedirectedHandler, error callleg_state=%d\n"
#define SIP_MGR_CALL_1189_112_2_17_23_20_25_832 "SIP: CallLegStateUnauthenticatedHandler, start to authenticate\n"
#define SIP_MGR_CALL_1256_112_2_17_23_20_25_833 "SIP:CallLegStateOfferingHandler, GetRegSessionIdForOffering fail\n"
#define SIP_MGR_CALL_1262_112_2_17_23_20_25_834 "SIP:CallLegStateOfferingHandler, SIPMGR_RegGetContext fail\n"
#define SIP_MGR_CALL_1270_112_2_17_23_20_25_835 "SIP:CallLegStateOfferingHandler, GetFreeSessinID fail\n"
#define SIP_MGR_CALL_1366_112_2_17_23_20_25_836 "SIP:CallLegStateAcceptedHandler, callee send invite 200 to peer\n"
#define SIP_MGR_CALL_1380_112_2_17_23_20_25_837 "SIP:CallLegStateInvitingHandler, caller send invite to peer\n"
#define SIP_MGR_CALL_1422_112_2_17_23_20_25_838 "SIP: CallLegStateConnectedHandler, error eDirection = %d\n"
#define SIP_MGR_CALL_1549_112_2_17_23_20_25_839 "SIP:CallLegStateDisconnectingHandler, send bye to peer\n"
#define SIP_MGR_CALL_1630_112_2_17_23_20_26_840 "SIP: CallLegStateRemoteAcceptedHandler, do nothing\n"
#define SIP_MGR_CALL_1661_112_2_17_23_20_26_841 "SIP:CallLegStateCancellingHandler, send cancel to peer\n"
#define SIP_MGR_CALL_1729_112_2_17_23_20_26_842 "SIP:CallLegCreatedEvHandler,Incoming call-leg %p was created\n\n"
#define SIP_MGR_CALL_1977_112_2_17_23_20_26_843 "SIP:CallLegStateChangedEvHandle==>default eState%d \n"
#define SIP_MGR_CALL_2226_112_2_17_23_20_27_844 "SIP: SIPMGR_CallUpdateReqHandler, error line = %d\n"
#define SIP_MGR_CALL_2296_112_2_17_23_20_27_845 "SIP: CallTransHandleStateReqRecv, error line = %d\n"
#define SIP_MGR_CALL_2311_112_2_17_23_20_27_846 "SIP: CallTransHandleStateResSent, update request sent to peer\n"
#define SIP_MGR_CALL_2358_112_2_17_23_20_27_847 "SIP: CallTransHandleStateResRecv, responseCode = %d\n"
#define SIP_MGR_CALL_2391_112_2_17_23_20_27_848 "SIP: CallTransHandleStateResRecv, error line = %d\n"
#define SIP_MGR_CALL_2465_112_2_17_23_20_27_849 "SIP: SIPMGR_CallUpdateRspHandler, error line = %d\n"
#define SIP_MGR_CALL_2671_112_2_17_23_20_28_850 "SIP: SendMsgToApp, need not to send msg_id = %d [0]\n"
#define SIP_MGR_CALL_2739_112_2_17_23_20_28_851 "SIP:CallLegStateProceedingHandler response_code=%d"
#define SIP_MGR_PUB_400_112_2_17_23_20_30_852 "SIP:SIPMGR_PubSessionReqHandler,Failed to create new publish-client\n"
#define SIP_MGR_PUB_409_112_2_17_23_20_30_853 "SIP:SIPMGR_PubSessionReqHandler,get context id failed\n"
#define SIP_MGR_PUB_892_112_2_17_23_20_31_854 "SIP:SIPMGR_PubPublishReqHandler, RvSipPubClientPublish failed\n"
#define SIP_MGR_PUB_932_112_2_17_23_20_31_855 "SIP: SIPMGR_PubRemoveReqHandler, ConstructMsgHandleInPubHandle failed\n"
#define SIP_MGR_PUB_940_112_2_17_23_20_31_856 "SIP:SIPMGR_PubRemoveReqHandler, RvSipPubClientRemove failed\n"
#define SIP_MGR_PUB_976_112_2_17_23_20_31_857 "SIP:SIPMGR_PubRefreshReqHandler, is_refresh_manual FALSE\n"
#define SIP_MGR_PUB_991_112_2_17_23_20_31_858 "SIP: SIPMGR_PubRefreshReqHandler, ConstructMsgHandleInPubHandle failed\n"
#define SIP_MGR_PUB_999_112_2_17_23_20_31_859 "SIP:SIPMGR_PubRefreshReqHandler, RvSipPubClientPublish failed\n"
#define SIP_MGR_PUB_1037_112_2_17_23_20_31_860 "SIP:SIPMGR_PubTerminateReqHandler, RvSipPubClientTerminate failed\n"
#define SIP_MGR_PUB_1109_112_2_17_23_20_31_861 "SIP:PubClientStateRedirectedHandler, GetSessIdByPubHandle failed\n"
#define SIP_MGR_PUB_1137_112_2_17_23_20_31_862 "SIP: PubClientStateRedirectedHandler, ConstructMsgHandleInPubHandle failed\n"
#define SIP_MGR_PUB_1144_112_2_17_23_20_31_863 "SIP: PubClientStateRedirectedHandler, RvSipPubClientPublish fail"
#define SIP_MGR_PUB_1344_112_2_17_23_20_32_864 "SIP:PubClientStatePublishedHandler, pub_state error = %d\n"
#define SIP_MGR_PUB_1478_112_2_17_23_20_32_865 "SIP:PubClientStateMsgSendFailureHandler, pub_state error = %d\n"
#define SIP_MGR_PUB_1616_112_2_17_23_20_32_866 "SIP:PubClientObjExpiredEvHandler, is_refresh_manual TRUE\n"
#define SIP_MGR_PUB_1637_112_2_17_23_20_32_867 "SIP: PubClientObjExpiredEvHandler, ConstructMsgHandleInPubHandle failed\n"
#define SIP_MGR_PUB_1645_112_2_17_23_20_32_868 "SIP:PubClientObjExpiredEvHandler, RvSipPubClientPublish failed\n"
#define SIP_MGR_REG_343_112_2_17_23_20_34_869 "SIP:SIPMGR_RegGetSessIdByHandle fail hRegClient=0x%x"
#define SIP_MGR_REG_423_112_2_17_23_20_34_870 "SIP:SIPMGR_RegSessionReqHandler Failed to create new register-client"
#define SIP_MGR_REG_1026_112_2_17_23_20_35_871 "SIP:SIPMGR_RegReqHandler, error line = %d\n"
#define SIP_MGR_REG_1122_112_2_17_23_20_35_872 "SIP:SIPMGR_DeRegReqHandler, error line = %d\n"
#define SIP_MGR_REG_1258_112_2_17_23_20_35_873 "SIP:RegClientStateChangedEvHandler eState=%d\n"
#define SIP_MGR_REG_1273_112_2_17_23_20_35_874 "SIP: RegClientStateRegisteringHandler, send register to registar\n"
#define SIP_MGR_REG_1297_112_2_17_23_20_35_875 "SIP: RegClientStateRedirectedHandler SIPMGR_RegGetSessIdByHandle fail"
#define SIP_MGR_REG_1318_112_2_17_23_20_36_876 "SIP: RegClientStateRedirectedHandler ConstructMsgHandleInRegHandle fail"
#define SIP_MGR_REG_1326_112_2_17_23_20_36_877 "SIP: RegClientStateRedirectedHandler RvSipRegClientRegister fail"
#define SIP_MGR_REG_1364_112_2_17_23_20_36_878 "SIP:RegClientStateUnauthenticatedHandler RvSipRegClientAuthenticate fail"
#define SIP_MGR_REG_1457_112_2_17_23_20_36_879 "SIP: RegClientStateRegisteredHandler, error reg_state = %d\n"
#define SIP_MGR_REG_1608_112_2_17_23_20_36_880 "SIP:RegClientMsgRecvEvHandler status code:%d"
#define SIP_MGR_REG_1624_112_2_17_23_20_36_881 "SIP:RegClientMsgToSendEvHandler Message Sent (register-client %p)\n"
#define SIP_MGR_REG_1641_112_2_17_23_20_36_882 "SIP:RegClientObjExpiresEvHandler Message Sent (register-client %p)\n"
#define SIP_MGR_REG_1851_112_2_17_23_20_37_883 "SIP: SendFailMsgToApp, reg_state = %d\n"
#define SIP_MGR_SUBS_451_112_2_17_23_20_38_884 "SIP:	SIPMGR_SubsSessionReqHandler, Failed to	create new subscription!\n"
#define SIP_MGR_SUBS_615_112_2_17_23_20_38_885 "SIP: UpdateSubsToHeaderInSubsHandle, RvSipMsgGetToHeader	failed\n"
#define SIP_MGR_SUBS_622_112_2_17_23_20_38_886 "SIP: UpdateSubsToHeaderInSubsHandle, RvSipSubsGetDialogObj failed\n"
#define SIP_MGR_SUBS_629_112_2_17_23_20_38_887 "SIP: UpdateSubsToHeaderInSubsHandle, RvSipCallLegGetToHeader failed\n"
#define SIP_MGR_SUBS_636_112_2_17_23_20_38_888 "SIP: UpdateSubsToHeaderInSubsHandle, RvSipPartyHeaderCopy failed\n"
#define SIP_MGR_SUBS_756_112_2_17_23_20_39_889 "SIP:	SIPMGR_SubsReqHandler, error line = %d\n"
#define SIP_MGR_SUBS_857_112_2_17_23_20_39_890 "SIP:	SIPMGR_SubsTerminateReqHandler,	RvSipSubsTerminate Fail!\n"
#define SIP_MGR_SUBS_909_112_2_17_23_20_39_891 "SIP:	SubsNotifyEvHandler, RvSipNotifyAccept Fail!\n"
#define SIP_MGR_SUBS_918_112_2_17_23_20_39_892 "SIP:	SubsNotifyEvHandler, RvSipNotifyReject Fail! response code = %d!\n"
#define SIP_MGR_SUBS_1057_112_2_17_23_20_39_893 "SIP:RegClientStateChangedEvHandler, eState=%d\n"
#define SIP_MGR_SUBS_1085_112_2_17_23_20_39_894 "SIP:	SubsStateRedirectedHandler, SIPMGR_SubsGetIdByHandle fail!\n"
#define SIP_MGR_SUBS_1109_112_2_17_23_20_39_895 "SIP:	SubsStateRedirectedHandler, ConstructMsgHandleInSubsHandle fail\n"
#define SIP_MGR_SUBS_1117_112_2_17_23_20_39_896 "SIP:	SubsStateRedirectedHandler, RvSipSubsSubscribe fail!\n"
#define SIP_MGR_SUBS_1220_112_2_17_23_20_39_897 "SIP:	SubsStateUnauthenticatedHandler, SIPMGR_SubsGetIdByHandle fail!\n"
#define SIP_MGR_SUBS_1230_112_2_17_23_20_39_898 "SIP: SubsStateUnauthenticatedHandler, RvSipSubsGetDialogObj Fail!\n"
#define SIP_MGR_SUBS_1239_112_2_17_23_20_39_899 "SIP: SubsStateUnauthenticatedHandler, RvSipCallLegAuthenticate Fail!\n"
#define SIP_MGR_SUBS_1367_112_2_17_23_20_40_900 "SIP: SubsStatePendingHandler, pre_subs_state ERROR = %d\n"
#define SIP_MGR_SUBS_1433_112_2_17_23_20_40_901 "SIP: SubsStateActiveHandler, pre_subs_state ERROR = %d\n"
#define SIP_MGR_SUBS_1573_112_2_17_23_20_40_902 "SIP: SubsStateTerminatedHandler,	pre_subs_state ERROR = %d\n"
#define SIP_MGR_SUBS_1621_112_2_17_23_20_40_903 "SIP:	SubsStateMsgSendFailureHandler,	pre_subs_state ERROR = %d\n"
#define SIP_MGR_SUBS_1723_112_2_17_23_20_40_904 "SIP:SubsExpirationAlertEvHandler, RvSipSubsRefresh failed\n"
#define SIP_MGR_SUBS_1923_112_2_17_23_20_41_905 "SIP: GetNotInfoandSendNotIndToApp, Input	parameter point	is NULL!\n"
#define SIP_MGR_SUBS_1934_112_2_17_23_20_41_906 "SIP: GetNotInfoandSendNotIndToApp, Get SubscriptionState	Header Fail!\n"
#define SIP_MGR_SUBS_1990_112_2_17_23_20_41_907 "SIP:	GetNotInfoandSendNotIndToApp, subs reason error!\n"
#define SIP_MGR_SUBS_2001_112_2_17_23_20_41_908 "SIP: GetNotInfoandSendNotIndToApp, sub state error!\n"
#define SIP_MGR_SUBS_2020_112_2_17_23_20_41_909 "SIP:	GetNotInfoandSendNotIndToApp, SIPPARS_DecodeSipBody Fail!\n"
#define SIP_MGR_TASK_199_112_2_17_23_20_41_910 "SIP: SipMgrMsgHandle, remote address = %x\n"
#define SIP_MGR_TASK_209_112_2_17_23_20_41_911 "\nSIP:Get           %.40s    id=0x%x\n"
#define SIP_MGR_TASK_218_112_2_17_23_20_42_912 "\nRTPRTCP:Get           %.40s    id=0x%x\n"
#define SIP_MGR_TASK_521_112_2_17_23_20_42_913 "SIP:InitSipStack  Application failed to construct the stack\n"
#define SIP_MGR_TASK_526_112_2_17_23_20_42_914 "SIP:InitSipStack The RADVISION SIP stack was constructed successfully. Version - %sn"
#define SIP_MGR_TASK_551_112_2_17_23_20_42_915 "SIP: SIPMGR_SockInit, send socket open error:%d\n"
#define SIP_MGR_TASK_562_112_2_17_23_20_42_916 "SIP: SIPMGR_SockInit, receive socket open error:%d\n"
#define SIP_MGR_TASK_574_112_2_17_23_20_42_917 "SIP: SIPMGR_SockInit, receive socket bind error:%d\n"
#define SIP_MGR_TASK_584_112_2_17_23_20_42_918 "SIP: SIPMGR_SockInit SUCCESS!\n"
#define SIP_MGR_TASK_634_112_2_17_23_20_42_919 "\nSIP:Send to app   %.40s    id=0x%x\n"
#define SIP_MGR_TASK_658_112_2_17_23_20_42_920 "\nRTPRTCP:Send to app   %.40s    id=0x%x\n"
#define SIP_MGR_TASK_686_112_2_17_23_20_42_921 "SIP: SIP_SendSignalToMgr, send message to sip manager error:%d, send_len = %d\n"
#define SIP_PARSER_385_112_2_17_23_20_44_922 "SIP:GetRedirectedUri, redirected_uri_ptr is PNULL\n"
#define RTCP_392_112_2_17_23_21_15_923 "RTP:RTP_OpenFrom, open socket fail!"
#define RTCP_1659_112_2_17_23_21_17_924 "RTCP RECV RTCP_PACKET_TYPE_SR"
#define RTCP_1676_112_2_17_23_21_17_925 "RTCP RECV RTCP_PACKET_TYPE_RR"
#define RTCP_1697_112_2_17_23_21_17_926 "RTCP RECV RTCP_PACKET_TYPE_SDES"
#define RTCP_1742_112_2_17_23_21_17_927 "RTCP RECV RTCP_PACKET_TYPE_BYE"
#define RTCP_1755_112_2_17_23_21_17_928 "RTCP RECV RTCP_PACKET_TYPE_APP"
#define RTCP_1777_112_2_17_23_21_17_929 "RTP:rtcpProcessRTCPPacket type error = %d"
#define RTCP_1883_112_2_17_23_21_17_930 "RTCP RTCP_TimerCallback ip = %x,port =%x"
#define RTP_222_112_2_17_23_21_19_931 "RTP:RTP_OpenFrom, open socket fail!"
#define RTP_338_112_2_17_23_21_19_932 "RTP RTP_OpenEx bind so %x to %08x:%d error - %d"
#define RTP_357_112_2_17_23_21_19_933 "RTP RTP_OpenEx bind so %x to %08x:%d error - %d"
#define RTP_662_112_2_17_23_21_20_934 "RTP_Unpack timestamp =%x"
#define RTP_735_112_2_17_23_21_20_935 "RTP RTP_Read remote ip =%x,remote port =%d"
#define RTP_750_112_2_17_23_21_20_936 "RTP RTP_Read RTCP_SetRemoteAddress remote ip =%x,remote port =%d"
#define RTP_828_112_2_17_23_21_20_937 "RTP RTP_ReadWithRemoteAddress ADDRESS IP =%x, PORT =%d,s->socket =%x,rvd_len = %d"
#define RTP_TEST_332_112_2_17_23_21_22_938 "rtp test: get send rtp req"
#define RTP_TEST_346_112_2_17_23_21_22_939 "rtp test: get send rtcp req"
#define RTP_TEST_351_112_2_17_23_21_22_940 "rtp test: get send rtcp app req"
#define RTP_TEST_357_112_2_17_23_21_22_941 "\n rtp test :get sr rr send cnf"
#define RTP_TEST_362_112_2_17_23_21_22_942 "\n rtp test :get rtcp data send cnf"
#define RTP_TEST_366_112_2_17_23_21_22_943 "\n rtp test :get rtcp sr/rr send ind"
#define RTP_TEST_373_112_2_17_23_21_22_944 "\n rtp test :get rtcp sr/rr rcvd ind"
#define RTP_TEST_378_112_2_17_23_21_22_945 "\n rtp test :get rtcp sr/rr data rcvd ind"
#define RTP_TEST_383_112_2_17_23_21_22_946 "\n rtp test :get rtcp bye rcvd ind"
#define RTP_TEST_394_112_2_17_23_21_22_947 "\n rtp test :terminate rtp session\n"
#define MAINAPP_RTP_1028_112_2_17_23_37_43_948 "StartIdleTimer: focus and flip!"
#define MAINAPP_RTP_1048_112_2_17_23_37_43_949 "StartIdleTimer: the idle timer has started!"
#define MAINAPP_RTP_1109_112_2_17_23_37_43_950 "StartIdleTimer: the idle timer has stoped!"
#define MAINAPP_RTP_1155_112_2_17_23_37_43_951 "MAIN_SetIdleBlueToothState: the blue tooth status is same!"
#define MAINAPP_RTP_1182_112_2_17_23_37_43_952 "MAIN_SetIdleBlueToothState: the blue tooth status is same!"
#define MAINAPP_RTP_1223_112_2_17_23_37_43_953 "MAIN_SetIdleNetWorkName: the name id is %d!"
#define MAINAPP_RTP_1252_112_2_17_23_37_43_954 "MAIN_SetIdleServiceProvidorName: tha name len = %d, is ucs2 = %d"
#define MAINAPP_RTP_1383_112_2_17_23_37_44_955 "MAIN_SetIdleChargeState: the is_charge is same!"
#define MAINAPP_RTP_1477_112_2_17_23_37_44_956 "MAIN_SetIdleSmsState: the sms_state =%d is same!"
#define MAINAPP_RTP_1512_112_2_17_23_37_44_957 "MAIN_SetIdleCallState: the call_state is same!"
#define MAINAPP_RTP_1525_112_2_17_23_37_44_958 "MAIN_SetIdleGprsState: gprs_state = %d"
#define MAINAPP_RTP_1829_112_2_17_23_37_44_959 "MAIN_HandleSysMsg: begin MISC_MSG_RTC_MIN!"
#define MAINAPP_RTP_1909_112_2_17_23_37_45_960 "MAIN_HandleSysMsg: can't handle the msg!"
#define MAINAPP_RTP_1972_112_2_17_23_37_45_961 "DrawFmName s_fm_rect.left= %d"
#define MAINAPP_RTP_1973_112_2_17_23_37_45_962 "DrawFmName s_fm_rect.right= %d"
#define MAINAPP_RTP_1974_112_2_17_23_37_45_963 "DrawFmName s_fm_rect.top= %d"
#define MAINAPP_RTP_1975_112_2_17_23_37_45_964 "DrawFmName s_fm_rect.bottom= %d"
#define MAINAPP_RTP_2041_112_2_17_23_37_45_965 "[MMIDC]: IdleWin_HandleMsg, MSG_OPEN_WINDOW"
#define MAINAPP_RTP_2089_112_2_17_23_37_45_966 "IdleWin_HandleMsg MSG_FULL_PAINT"
#define MAINAPP_RTP_2134_112_2_17_23_37_45_967 "IDLEWIN_HandleMsg: handle MSG_IDLE_UPDATE_DATETIME!"
#define MAINAPP_RTP_2144_112_2_17_23_37_45_968 "IdleWin_HandleMsg: MSG_IDLE_UPDATE_NETWORK!"
#define MAINAPP_RTP_2167_112_2_17_23_37_45_969 "IdleWin_HandleMsg: MSG_IDLE_UPDATE_MP3!"
#define MAINAPP_RTP_2454_112_2_17_23_37_46_970 "IDLEWIN_HandleWinMsg: handle headset MSG!"
#define MAINAPP_RTP_2470_112_2_17_23_37_46_971 "IdleWin_HandleMsg:received timer msg!"
#define MAINAPP_RTP_2477_112_2_17_23_37_46_972 "IdleWin_HandleMsg: handle the anim wallpater timer!"
#define MAINAPP_RTP_2508_112_2_17_23_37_46_973 "IdleWin_HandleMsg: MMK_GetFocusWinId() is =%d"
#define MAINAPP_RTP_2650_112_2_17_23_37_46_974 "MMI: IdleWin_HandleMsg, received tp msg_id "
#define MAINAPP_RTP_2657_112_2_17_23_37_46_975 "IdleWin_HandleMsg: point.x=%d"
#define MAINAPP_RTP_2658_112_2_17_23_37_46_976 "IdleWin_HandleMsg: point.y=%d"
#define MAINAPP_RTP_2659_112_2_17_23_37_46_977 "IdleWin_HandleMsg s_mp3_rect.left= %d"
#define MAINAPP_RTP_2660_112_2_17_23_37_46_978 "IdleWin_HandleMsg s_mp3_rect.right= %d"
#define MAINAPP_RTP_2661_112_2_17_23_37_46_979 "IdleWin_HandleMsg s_mp3_rect.top= %d"
#define MAINAPP_RTP_2662_112_2_17_23_37_46_980 "IdleWin_HandleMsg s_mp3_rect.bottom= %d"
#define MAINAPP_RTP_2663_112_2_17_23_37_46_981 "IdleWin_HandleMsg s_idlewin_info.is_display_mp3= %d"
#define MAINAPP_RTP_2688_112_2_17_23_37_46_982 "IdleWin_HandleMsg open mp3 player!"
#define MAINAPP_RTP_2721_112_2_17_23_37_46_983 "IDLEWIN_HandleWinMsg: MSG_STK_SET_UP_IDLE_MODE"
#define MAINAPP_RTP_2751_112_2_17_23_37_46_984 "MMI: IdleWin_HandleMsg, don't handle message, msg_id = %d"
#define MAINAPP_RTP_2781_112_2_17_23_37_46_985 "OpenDialingNumberBock, enter"
#define MAINAPP_RTP_2786_112_2_17_23_37_46_986 "[mainapp.c]: dialing number block have been opened"
#define MAINAPP_RTP_3181_112_2_17_23_37_47_987 "kick the key VT"
#define MAINAPP_RTP_3277_112_2_17_23_37_47_988 "FuncDialingWin_HandleMsg MSG_KEYLONG_HASH!"
#define MAINAPP_RTP_3311_112_2_17_23_37_47_989 "FuncDialingWin_HandleMsg:result_info_ptr->protect_type %d is error!"
#define MAINAPP_RTP_3332_112_2_17_23_37_47_990 "[mainapp.c]: close dialing number block"
#define MAINAPP_RTP_3336_112_2_17_23_37_47_991 "[mainapp.c]: dialing number block have been closed!"
#define MAINAPP_RTP_3344_112_2_17_23_37_48_992 "[mainapp.c]: BLOCK disable failure!"
#define MAINAPP_RTP_3347_112_2_17_23_37_48_993 " CloseDialingNumberBlock: close block successful!"
#define MAINAPP_RTP_3369_112_2_17_23_37_48_994 "main_app_init:call_ring_type is %d!"
#define MAINAPP_RTP_3464_112_2_17_23_37_48_995 "MAIN_StopIdleTimer: the sim error timer has stoped!"
#define MAINAPP_RTP_3662_112_2_17_23_37_48_996 "OutputIdleWinWords idle_mode->len= %d"
#define MAINAPP_RTP_3684_112_2_17_23_37_48_997 "OutputIdleWinWords s_words_rect.left= %d"
#define MAINAPP_RTP_3685_112_2_17_23_37_48_998 "OutputIdleWinWords s_words_rect.right= %d"
#define MAINAPP_RTP_3686_112_2_17_23_37_48_999 "OutputIdleWinWords s_words_rect.top= %d"
#define MAINAPP_RTP_3687_112_2_17_23_37_48_1000 "OutputIdleWinWords s_words_rect.bottom= %d"
#define MAINAPP_RTP_3743_112_2_17_23_37_48_1001 "OutputIdleWinNetworkName point.x= %d"
#define MAINAPP_RTP_3744_112_2_17_23_37_48_1002 "OutputIdleWinNetworkName point.y= %d"
#define MAINAPP_RTP_3769_112_2_17_23_37_48_1003 "OutputIdleWinNetworkName, is_get_cphs_info = %d"
#define MAINAPP_RTP_3784_112_2_17_23_37_48_1004 "OutputIdleWinNetworkName s_network_rect.left= %d"
#define MAINAPP_RTP_3785_112_2_17_23_37_48_1005 "OutputIdleWinNetworkName s_network_rect.right= %d"
#define MAINAPP_RTP_3786_112_2_17_23_37_48_1006 "OutputIdleWinNetworkName s_network_rect.top= %d"
#define MAINAPP_RTP_3787_112_2_17_23_37_48_1007 "OutputIdleWinNetworkName s_network_rect.bottom= %d"
#define MAINAPP_RTP_3810_112_2_17_23_37_48_1008 "OutputIdleWinNetworkName, get the network name from sim card!"
#define MAINAPP_RTP_3839_112_2_17_23_37_48_1009 "OutputIdleWinNetworkName s_network_rect.left= %d"
#define MAINAPP_RTP_3840_112_2_17_23_37_48_1010 "OutputIdleWinNetworkName s_network_rect.right= %d"
#define MAINAPP_RTP_3841_112_2_17_23_37_48_1011 "OutputIdleWinNetworkName s_network_rect.top= %d"
#define MAINAPP_RTP_3842_112_2_17_23_37_49_1012 "OutputIdleWinNetworkName s_network_rect.bottom= %d"
#define MAINAPP_RTP_3853_112_2_17_23_37_49_1013 "OutputIdleWinNetworkName, get the network name using the old method!"
#define MAINAPP_RTP_4245_112_2_17_23_37_49_1014 "DisplayIdleMp3Name g_idle_cur_mp3_name.length= %d"
#define MAINAPP_RTP_4246_112_2_17_23_37_49_1015 "DisplayIdleMp3Name g_idle_cur_mp3_name.is_ucs2= %d"
#define MAINAPP_RTP_4254_112_2_17_23_37_49_1016 "DisplayIdleMp3Name string_width= %d"
#define MAINAPP_RTP_4269_112_2_17_23_37_49_1017 "DisplayIdleMp3Name s_mp3_rect.left= %d"
#define MAINAPP_RTP_4270_112_2_17_23_37_49_1018 "DisplayIdleMp3Name s_mp3_rect.right= %d"
#define MAINAPP_RTP_4271_112_2_17_23_37_49_1019 "DisplayIdleMp3Name s_mp3_rect.top= %d"
#define MAINAPP_RTP_4272_112_2_17_23_37_49_1020 "DisplayIdleMp3Name s_mp3_rect.bottom= %d"
#define MAINAPP_RTP_4331_112_2_17_23_37_49_1021 "DisplayIdleCycleMp3Name s_mp3_rect.left= %d"
#define MAINAPP_RTP_4332_112_2_17_23_37_49_1022 "DisplayIdleCycleMp3Name s_mp3_rect.right= %d"
#define MAINAPP_RTP_4333_112_2_17_23_37_49_1023 "DisplayIdleCycleMp3Name s_mp3_rect.top= %d"
#define MAINAPP_RTP_4334_112_2_17_23_37_49_1024 "DisplayIdleCycleMp3Name s_mp3_rect.bottom= %d"
#define MAINAPP_RTP_4625_112_2_17_23_37_50_1025 "DisplayStatusBarIcon start MMI_WIN_ICON_GPRS is_gprs is %d"
#define MAINAPP_RTP_4628_112_2_17_23_37_50_1026 "DisplayStatusBarIcon MMI_WIN_ICON_GPRS sim is OK!"
#define MAINAPP_RTP_4642_112_2_17_23_37_50_1027 "DisplayStatusBarIcon image_id = IMAGE_IDLE_TITLE_ICON_GPRS_DEACTIVE"
#define MAINAPP_RTP_4809_112_2_17_23_37_50_1028 "DisplayStatusBarIcon is charge: image_id=%d"
#define MAINAPP_RTP_4816_112_2_17_23_37_50_1029 "DisplayStatusBarIcon not charge: image_id=%d"
#define MAINAPP_RTP_4980_112_2_17_23_37_51_1030 "MAIN_SetIdleSmsState: the line_state is same!"
#define MAINAPP_RTP_4997_112_2_17_23_37_51_1031 "MAIN_SetIdleSmsState: the voice message is same!"
#define MAINAPP_RTP_5106_112_2_17_23_37_51_1032 "MMIMAIN_HandleIdelTimer: handle the idle icon timer!"
#define MAINAPP_RTP_5179_112_2_17_23_37_51_1033 "MMI_GetFreeSpace: free_space = %d"
#define MAINAPP_RTP_5341_112_2_17_23_37_51_1034 "Handle_HW_Icon_Exec_Func : s_cur_hw_icon_index = %d"
#define MMIMAIN_RTP_235_112_2_17_23_37_53_1035 "KeypadCallBackFunc: the key message is too much!"
#define MMIMAIN_RTP_263_112_2_17_23_37_53_1036 "TPCallBackFunc, P_TPP = %d"
#define MMIMAIN_RTP_286_112_2_17_23_37_53_1037 "TPCallBackFunc: the TP message is too much!"
#define MMIMAIN_RTP_411_112_2_17_23_37_53_1038 "Receive CHR_CAP_IND"
#define MMIMAIN_RTP_417_112_2_17_23_37_53_1039 "Receive CHR_CHARGE_START_IND"
#define MMIMAIN_RTP_423_112_2_17_23_37_53_1040 "Receive CHR_CHARGE_END_IND"
#define MMIMAIN_RTP_429_112_2_17_23_37_53_1041 "Receive CHR_WARNING_IND"
#define MMIMAIN_RTP_435_112_2_17_23_37_53_1042 "Receive CHR_SHUTDOWN_IND"
#define MMIMAIN_RTP_441_112_2_17_23_37_53_1043 "Receive CHR_CHARGE_FINISH"
#define MMIMAIN_RTP_447_112_2_17_23_37_53_1044 "Receive CHR_CHARGE_DISCONNECT"
#define MMIMAIN_RTP_453_112_2_17_23_37_53_1045 "Receive CHR_CHARGE_FAULT"
#define MMIMAIN_RTP_459_112_2_17_23_37_53_1046 "ChargeCallBackFunc:event_id = %d"
#define MMIMAIN_RTP_741_112_2_17_23_37_54_1047 "APP_Task: begin init, time = %d"
#define MMIMAIN_RTP_791_112_2_17_23_37_54_1048 "APP_Task: restart_condition is %d,time is %dms"
#define MMIMAIN_RTP_890_112_2_17_23_37_54_1049 "APP_Task: end init, time = %d"
#define MMIMAIN_RTP_918_112_2_17_23_37_54_1050 "--------RTP: received RTP_RTCP_SESSION_INIT_CNF!!!\n"
#define MMIMAIN_RTP_922_112_2_17_23_37_54_1051 "--------RTP: received RTP_DATA_SEND_CNF!!!\n"
#define MMIMAIN_RTP_928_112_2_17_23_37_54_1052 "--------RTP: received RTP_DATA_RCVD_IND!!!\n"
#define MMIMAIN_RTP_936_112_2_17_23_37_54_1053 "--------RTP: received RTCP_SRRR_AUTO_SEND_SET_CNF!!!\n"
#define MMIMAIN_RTP_967_112_2_17_23_37_54_1054 "SignalCode is %d"
#define MMIMAIN_RTP_980_112_2_17_23_37_54_1055 "fdn is disable,start init adn"
#define MMIMAIN_RTP_1017_112_2_17_23_37_54_1056 "Receive UPM_PLUGIN"
#define MMIMAIN_RTP_1028_112_2_17_23_37_54_1057 "Receive UPM_PLUGOUT"
#define MMIMAIN_RTP_1039_112_2_17_23_37_54_1058 "Receive UPM_READY"
#define MMIMAIN_RTP_1050_112_2_17_23_37_54_1059 "Receive UPM_ERROR"
#define MMIMAIN_RTP_1061_112_2_17_23_37_54_1060 "UPMCallBackFunc:event_id = %d"
#define NETMGR_API_88_112_2_17_23_37_55_1061 "#NETMGR REG SIG ERROR, sig type:%d,pro type:%d"
#define NETMGR_API_158_112_2_17_23_37_55_1062 "#NETMGR NETMGR_DealNextRequest error,host_ptr==0"
#define NETMGR_API_163_112_2_17_23_37_55_1063 "#NETMGR NETMGR_DealNextRequest error,cur request==0"
#define NETMGR_API_311_112_2_17_23_37_55_1064 "#NETMGR NETMGR_Redirect error host==0"
#define NETMGR_API_329_112_2_17_23_37_55_1065 "#NETMGR NETMGR_Redirect url is wrong"
#define NETMGR_API_532_112_2_17_23_37_55_1066 "#NETMGR NETMGR_SetProxyAuthenInfo error host==0"
#define NETMGR_HOST_67_112_2_17_23_37_56_1067 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_HOST_87_112_2_17_23_37_56_1068 "#NETMGR HOST_CreatHost, host:%x"
#define NETMGR_HOST_89_112_2_17_23_37_56_1069 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_HOST_94_112_2_17_23_37_56_1070 "#NETMGR HOST_CreatHost, pro host:%x"
#define NETMGR_HOST_114_112_2_17_23_37_56_1071 "#NETMGR HOST_DestroyHost, host:%x, pro host:%x"
#define NETMGR_HOST_117_112_2_17_23_37_56_1072 "#NETMGR HOST_DestroyHost, invalid host handler"
#define NETMGR_INSTANCE_175_112_2_17_23_37_57_1073 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_199_112_2_17_23_37_57_1074 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_204_112_2_17_23_37_57_1075 "#NETMGR creat netmgr instance error:alloc instance mem failed!"
#define NETMGR_INSTANCE_228_112_2_17_23_37_57_1076 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_232_112_2_17_23_37_57_1077 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_273_112_2_17_23_37_57_1078 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_277_112_2_17_23_37_57_1079 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_292_112_2_17_23_37_57_1080 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_296_112_2_17_23_37_57_1081 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_319_112_2_17_23_37_57_1082 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_324_112_2_17_23_37_57_1083 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_329_112_2_17_23_37_57_1084 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_349_112_2_17_23_37_57_1085 "#NETMGR MANAGER_DestroyInstance, invalid netmgr instance!"
#define NETMGR_INSTANCE_357_112_2_17_23_37_57_1086 "#NETMGR MANAGER_DestroyInstance, set context_handle 0 !"
#define NETMGR_INSTANCE_440_112_2_17_23_37_57_1087 "#NETMGR TASK ENTRY TRANS TIMER"
#define NETMGR_INSTANCE_444_112_2_17_23_37_57_1088 "#NETMGR TASK ENTRY keep alive timer"
#define NETMGR_INSTANCE_454_112_2_17_23_37_57_1089 "#NETMGR TASK ENTRY SOCK, EVENT:%d"
#define NETMGR_INSTANCE_459_112_2_17_23_37_57_1090 "#NETMGR TASK ENTRY APP, EVENT:%d"
#define NETMGR_INSTANCE_469_112_2_17_23_37_57_1091 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_499_112_2_17_23_37_57_1092 "#NETMGR HandleKeepAliveTimer"
#define NETMGR_INSTANCE_573_112_2_17_23_37_57_1093 "#NETMGR handle app signal error:pro type:%d,sig_type:%d"
#define NETMGR_INSTANCE_639_112_2_17_23_37_58_1094 "#NETMGR HandleAppInitSig,creat netmgr instance failed!!"
#define NETMGR_INSTANCE_643_112_2_17_23_37_58_1095 "#NETMGR HandleAppInitSig,id_for_sockbind:%d"
#define NETMGR_INSTANCE_678_112_2_17_23_37_58_1096 "#NETMGR HandleAppRequestSig, invalid netmgr instance!"
#define NETMGR_INSTANCE_689_112_2_17_23_37_58_1097 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_INSTANCE_703_112_2_17_23_37_58_1098 "NETMGR HandleAppRequestSig, url is wrong"
#define NETMGR_INSTANCE_814_112_2_17_23_37_58_1099 "#NETMGR HandleAppCancelSig,request_node_handler->request_ptr == 0!!!"
#define NETMGR_INSTANCE_845_112_2_17_23_37_58_1100 "#NETMGR HandleAppCloseSig, invalid netmgr instance!"
#define NETMGR_INSTANCE_1388_112_2_17_23_37_59_1101 "#NETMGR MANAGER_GetDefaultHostPort host=0!"
#define NETMGR_INSTANCE_1395_112_2_17_23_37_59_1102 "#NETMGR MANAGER_GetDefaultHostPort netmgr_ptr=0!"
#define NETMGR_INSTANCE_1403_112_2_17_23_37_59_1103 "#NETMGR MANAGER_GetDefaultHostPort net_request_ptr=0!"
#define NETMGR_INSTANCE_1506_112_2_17_23_37_59_1104 "#NETMGR MANAGER_GetDefaultHostPort default port=%d!"
#define NETMGR_REQUEST_62_112_2_17_23_37_59_1105 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_REQUEST_69_112_2_17_23_38_0_1106 "NETMGR REQUEST_CreatRequest url is wrong"
#define NETMGR_REQUEST_84_112_2_17_23_38_0_1107 "#NETMGR REQUEST_CreatRequest:%x, pro request:%x"
#define NETMGR_REQUEST_87_112_2_17_23_38_0_1108 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_REQUEST_109_112_2_17_23_38_0_1109 "#NETMGR REQUEST_DestroyRequest:%x, pro request:%x"
#define NETMGR_REQUEST_113_112_2_17_23_38_0_1110 "#NETMGR REQUEST_DestroyRequest error host==0"
#define NETMGR_TEST_50_112_2_17_23_38_0_1111 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_71_112_2_17_23_38_0_1112 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_TEST_74_112_2_17_23_38_0_1113 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_TEST_83_112_2_17_23_38_0_1114 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_97_112_2_17_23_38_0_1115 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_TEST_183_112_2_17_23_38_0_1116 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_206_112_2_17_23_38_0_1117 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_233_112_2_17_23_38_0_1118 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_276_112_2_17_23_38_1_1119 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_299_112_2_17_23_38_1_1120 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_318_112_2_17_23_38_1_1121 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_332_112_2_17_23_38_1_1122 "#NETMGR recv data:%s"
#define NETMGR_TEST_APP_30_112_2_17_23_38_1_1123 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_APP_56_112_2_17_23_38_1_1124 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_APP_78_112_2_17_23_38_1_1125 "#NETMGR send confirm!!!"
#define NETMGR_TEST_APP_85_112_2_17_23_38_1_1126 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_APP_99_112_2_17_23_38_1_1127 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_TEST_APP_111_112_2_17_23_38_1_1128 "#NETMGR TEST CLOSE!!"
#define NETMGR_TEST_APP_114_112_2_17_23_38_1_1129 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_URL_267_112_2_17_23_38_2_1130 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_URL_319_112_2_17_23_38_2_1131 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_URL_339_112_2_17_23_38_2_1132 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_URL_359_112_2_17_23_38_2_1133 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_URL_413_112_2_17_23_38_2_1134 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_URL_495_112_2_17_23_38_2_1135 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_URL_510_112_2_17_23_38_2_1136 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_URL_572_112_2_17_23_38_2_1137 "URL_GetStringByRule, len = %d"
#define NETMGR_URL_574_112_2_17_23_38_2_1138 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETMGR_URL_677_112_2_17_23_38_2_1139 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_URL_682_112_2_17_23_38_2_1140 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_URL_687_112_2_17_23_38_2_1141 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_URL_692_112_2_17_23_38_3_1142 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_URL_697_112_2_17_23_38_3_1143 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_URL_702_112_2_17_23_38_3_1144 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETMGR_URL_707_112_2_17_23_38_3_1145 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETTRANS_API_73_112_2_17_23_38_3_1146 "#NETMGR NETTRANS_Connect netmgr instance is null!"
#define NETTRANS_API_192_112_2_17_23_38_3_1147 "#NETMGR NETTRANS_Disconnect netmgr instance is null!"
#define NETTRANS_API_211_112_2_17_23_38_3_1148 "#NETMGR NETTRANS_Disconnect invalid connection!"
#define NETTRANS_API_247_112_2_17_23_38_3_1149 "#NETMGR NETTRANS_Recv netmgr instance is null!"
#define NETTRANS_API_301_112_2_17_23_38_3_1150 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETTRANS_API_351_112_2_17_23_38_4_1151 "#NETMGR trans recv alloc read signal error!"
#define NETTRANS_API_366_112_2_17_23_38_4_1152 "#NETMGR trans recv alloc signal error!"
#define NETTRANS_API_392_112_2_17_23_38_4_1153 "#NETMGR trans recv sci_sock_errno EWOULDBLOCK"
#define NETTRANS_API_397_112_2_17_23_38_4_1154 "#NETMGR recv fail, err:%d"
#define NETTRANS_API_446_112_2_17_23_38_4_1155 "#NETMGR NETTRANS_Send netmgr instance is null!"
#define NETTRANS_API_503_112_2_17_23_38_4_1156 "#NETMGR NETTRANS_Send, invalid input!"
#define NETTRANS_API_561_112_2_17_23_38_4_1157 "#NETMGR NETTRANS_SendFile netmgr instance is null!"
#define NETTRANS_API_680_112_2_17_23_38_4_1158 "#NETMGR NETTRANS_FindHostConnectted netmgr instance is null!"
#define NETTRANS_API_705_112_2_17_23_38_4_1159 "#NETMGR NETTRANS_FindHostDnsing netmgr instance is null!"
#define NETTRANS_API_731_112_2_17_23_38_4_1160 "#NETMGR NETTRANS_FindHostConnecting netmgr instance is null!"
#define NETTRANS_API_757_112_2_17_23_38_4_1161 "#NETMGR NETTRANS_HandleHostDestroy netmgr instance is null!"
#define NETTRANS_CONNECTION_130_112_2_17_23_38_5_1162 "#NETMGR CONNECTION_DestroyConnection invalid connection in the indicated list"
#define NETTRANS_CONNECTION_208_112_2_17_23_38_5_1163 "#NETMGR CONNECTION_GetConnectionBySocket invalid netmgr 1!"
#define NETTRANS_CONNECTION_227_112_2_17_23_38_5_1164 "#NETMGR CONNECTION_GetConnectionBySocket invalid netmgr 2!"
#define NETTRANS_CONNECTION_275_112_2_17_23_38_5_1165 "#NETMGR CONNECTION_GetConnectionByGethostId invalid netmgr 1!"
#define NETTRANS_CONNECTION_294_112_2_17_23_38_5_1166 "#NETMGR CONNECTION_GetConnectionByGethostId invalid netmgr 2!"
#define NETTRANS_CONNECTION_380_112_2_17_23_38_5_1167 "#NETMGR CONNECTION_CheckIfConnectionValid invalid connection_ptr!"
#define NETTRANS_CONNECTION_391_112_2_17_23_38_5_1168 "#NETMGR CONNECTION_CheckIfConnectionValid invalid netmgr 1!"
#define NETTRANS_CONNECTION_423_112_2_17_23_38_5_1169 "#NETMGR CONNECTION_CheckIfConnectionValid invalid netmgr 2!"
#define NETTRANS_MSG_67_112_2_17_23_38_6_1170 "#NETMGR TRANS_HandleTimer invalid connection_ptr"
#define NETTRANS_MSG_73_112_2_17_23_38_6_1171 "#NETMGR TRANS_HandleTimer, invalid connection node in connection_ptr!"
#define NETTRANS_MSG_79_112_2_17_23_38_6_1172 "#NETMGR invalid timer arrived!"
#define NETTRANS_MSG_84_112_2_17_23_38_6_1173 "#NETMGR none exist timer arrived!"
#define NETTRANS_MSG_89_112_2_17_23_38_6_1174 "#NETMGR TRANS_HandleTimer, Bad connection!"
#define NETTRANS_MSG_97_112_2_17_23_38_6_1175 "#NETMGR TRANS_HandleTimer, invalid netmanager_instance_ptr!"
#define NETTRANS_MSG_102_112_2_17_23_38_6_1176 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_MSG_135_112_2_17_23_38_6_1177 "#NETMGR TRANS_HandleTimer, connection_ptr in connecting_list_node_handler is null!"
#define NETTRANS_MSG_154_112_2_17_23_38_6_1178 "#NETMGR TRANS_HandleTimer NETTRANS_CONNECTION_STATE_DNSING, insert to connecting list fail"
#define NETTRANS_MSG_231_112_2_17_23_38_6_1179 "#NETMGR socket event:SOCKET_READ_EVENT_IND"
#define NETTRANS_MSG_245_112_2_17_23_38_6_1180 "#NETMGR socket event:SOCKET_READ_BUFFER_STATUS_EVENT_IND"
#define NETTRANS_MSG_263_112_2_17_23_38_6_1181 "#NETMGR socket event:SOCKET_WRITE_EVENT_IND"
#define NETTRANS_MSG_278_112_2_17_23_38_6_1182 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_MSG_321_112_2_17_23_38_6_1183 "#NETMGR socket event:SOCKET_CONNECT_EVENT_IND"
#define NETTRANS_MSG_330_112_2_17_23_38_6_1184 "#NETMGR TRANS_HandleSocketSig, invalid connection_ptr!"
#define NETTRANS_MSG_376_112_2_17_23_38_7_1185 "#NETMGR socket event:SOCKET_CONNECTION_CLOSE_EVENT_IND"
#define NETTRANS_MSG_405_112_2_17_23_38_7_1186 "#NETMGR socket event:SOCKET_ASYNC_GETHOSTBYNAME_CNF"
#define NETTRANS_MSG_414_112_2_17_23_38_7_1187 "#NETMGR socket event:SOCKET_ASYNC_GETHOSTBYNAME_CNF, invalid connection"
#define NETTRANS_MSG_440_112_2_17_23_38_7_1188 "#NETMGR TRANS_HandleSocketSig, invalid new_connection_ptr!"
#define NETTRANS_MSG_461_112_2_17_23_38_7_1189 "#NETMGR TRANS_HandleTimer NETTRANS_CONNECTION_STATE_DNSING, insert to connecting list fail"
#define NETTRANS_MSG_482_112_2_17_23_38_7_1190 "#NETMGR socket event: unknown socket event %d"
#define NETTRANS_MSG_508_112_2_17_23_38_7_1191 "#NETMGR TRANS_Callback, host_handle==0!"
#define NETTRANS_MSG_536_112_2_17_23_38_7_1192 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_MSG_573_112_2_17_23_38_7_1193 "#NETMGR TRANS_SendReport, host_handle==0!"
#define NETTRANS_MSG_578_112_2_17_23_38_7_1194 "#NETMGR TRANS_SendReport, netmanager_instance_ptr==0!"
#define NETTRANS_MSG_611_112_2_17_23_38_7_1195 "#NETMGR TRANS_HandleDnsInd, netmanager_instance_ptr==0!"
#define NETTRANS_MSG_629_112_2_17_23_38_7_1196 "#NETMGR TRANS_HandleDnsInd, connection_ptr==0!"
#define NETTRANS_SOCK_68_112_2_17_23_38_7_1197 "#NETMGR SOCK_GetHostByName connection_ptr->host_handle == 0!"
#define NETTRANS_SOCK_73_112_2_17_23_38_7_1198 "#NETMGR SOCK_GetHostByName host name == 0!"
#define NETTRANS_SOCK_88_112_2_17_23_38_7_1199 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_100_112_2_17_23_38_8_1200 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_115_112_2_17_23_38_8_1201 "CFL TIMER CREAT, file:%s, line:%d"
#define NETTRANS_SOCK_128_112_2_17_23_38_8_1202 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_155_112_2_17_23_38_8_1203 "#NETMGR SOCK_GetHostByNameAsync,ERR input"
#define NETTRANS_SOCK_161_112_2_17_23_38_8_1204 "#NETMGR SOCK_GetHostByNameAsync,ERR connection"
#define NETTRANS_SOCK_166_112_2_17_23_38_8_1205 "#NETMGR SOCK_GetHostByNameAsync,ERR host_ptr"
#define NETTRANS_SOCK_172_112_2_17_23_38_8_1206 "#NETMGR SOCK_GetHostByNameAsync,ERR netmgr_ptr"
#define NETTRANS_SOCK_182_112_2_17_23_38_8_1207 "#NETMGR SOCK_GetHostByNameAsync,ERR sci_async_gethostbyname"
#define NETTRANS_SOCK_210_112_2_17_23_38_8_1208 "#NETMGR SOCK_Connect, connection_ptr == 0!"
#define NETTRANS_SOCK_217_112_2_17_23_38_8_1209 "#NETMGR SOCK_Connect, host_ptr==0!"
#define NETTRANS_SOCK_244_112_2_17_23_38_8_1210 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_269_112_2_17_23_38_8_1211 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_306_112_2_17_23_38_8_1212 "CFL TIMER CREAT, file:%s, line:%d"
#define NETTRANS_SOCK_331_112_2_17_23_38_8_1213 "#NETMGR SOCK_Creat err, host=0!"
#define NETTRANS_SOCK_337_112_2_17_23_38_8_1214 "#NETMGR SOCK_Creat err, netmgr=0!"
#define NETTRANS_SOCK_358_112_2_17_23_38_8_1215 "#NETMGR SOCK_Creat tcp fail"
#define NETTRANS_SOCK_378_112_2_17_23_38_8_1216 "#NETMGR SOCK_Creat udp fail"
#define NETTRANS_SOCK_404_112_2_17_23_38_8_1217 "#NETMGR SOCK_Send, connection_ptr==0!"
#define NETTRANS_SOCK_409_112_2_17_23_38_8_1218 "#NETMGR SOCK_Send, socket id==0!"
#define NETTRANS_SOCK_414_112_2_17_23_38_8_1219 "#NETMGR SOCK_Send, send buffer==0!"
#define NETTRANS_SOCK_421_112_2_17_23_38_8_1220 "NETMGR SOCK_Send connection_ptr->socket_id %x mss = %d"
#define NETTRANS_SOCK_463_112_2_17_23_38_8_1221 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_496_112_2_17_23_38_8_1222 "CFL TIMER CREAT, file:%s, line:%d"
#define NETTRANS_SOCK_500_112_2_17_23_38_8_1223 "#NETMGR trans send buffer, socket ENOBUFS!"
#define NETTRANS_SOCK_513_112_2_17_23_38_8_1224 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_545_112_2_17_23_38_9_1225 "#NETMGR SOCK_SendByUdp, connection_ptr==0!"
#define NETTRANS_SOCK_550_112_2_17_23_38_9_1226 "#NETMGR SOCK_SendByUdp, socket id==0!"
#define NETTRANS_SOCK_555_112_2_17_23_38_9_1227 "#NETMGR SOCK_SendByUdp, send buffer==0!"
#define NETTRANS_SOCK_600_112_2_17_23_38_9_1228 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_633_112_2_17_23_38_9_1229 "CFL TIMER CREAT, file:%s, line:%d"
#define NETTRANS_SOCK_636_112_2_17_23_38_9_1230 "#NETMGR trans send buffer, socket ENOBUFS!"
#define NETTRANS_SOCK_650_112_2_17_23_38_9_1231 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_686_112_2_17_23_38_9_1232 "#NETMGR SOCK_SendFile, invalid connection_ptr!"
#define NETTRANS_SOCK_692_112_2_17_23_38_9_1233 "#NETMGR SOCK_SendFile, invalid send file param!"
#define NETTRANS_SOCK_706_112_2_17_23_38_9_1234 "NETMGR SOCK_SendFile connection_ptr->socket_id %x mss = %d"
#define NETTRANS_SOCK_740_112_2_17_23_38_9_1235 "CFL MEM ALLOC buf:%x, file:%s, line:%d"
#define NETTRANS_SOCK_772_112_2_17_23_38_9_1236 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_804_112_2_17_23_38_9_1237 "CFL TIMER CREAT, file:%s, line:%d"
#define NETTRANS_SOCK_809_112_2_17_23_38_9_1238 "#NETMGR trans send buffer, socket ENOBUFS!"
#define NETTRANS_SOCK_814_112_2_17_23_38_9_1239 "CFL MEM FREE buf:%x, file:%s, line:%d"
#define NETTRANS_SOCK_829_112_2_17_23_38_9_1240 "CFL TIMER DELETE, file:%s, line:%d"
#define NETTRANS_SOCK_910_112_2_17_23_38_9_1241 "#NETMGR trans recv relloc error!"
#define NETTRANS_SOCK_947_112_2_17_23_38_9_1242 "#NETMGR SOCK_UdpGetHostByName, connection_ptr == 0!"
#define NETTRANS_SOCK_953_112_2_17_23_38_9_1243 "#NETMGR SOCK_UdpGetHostByName, host_ptr == 0!"
#define NETTRANS_SOCK_958_112_2_17_23_38_9_1244 "#NETMGR SOCK_UdpGetHostByName, netmanager_instance_ptr == 0!"
#define DD_XML_HANDLER_322_112_2_17_23_38_11_1245 "\nODM: Download description has more 'type' than system support(%d)"
#define DD_XML_HANDLER_413_112_2_17_23_38_11_1246 "Invalid parameter"
#define HTTP_INTERFACE_366_112_2_17_23_38_19_1247 "ODM:receive http error ind!"
#define HTTP_INTERFACE_370_112_2_17_23_38_19_1248 "ODM:receive http error ind! result = %d"
#define ODM_399_112_2_17_23_38_21_1249 "ODM_StartDownload: length=%d,is use proxy = %d"
#define ODM_402_112_2_17_23_38_21_1250 "ODM_StartDownload: proxy host=%s\n"
#define ODM_428_112_2_17_23_38_21_1251 "\n ODM_StartDownload error_code = %d"
#define ODM_468_112_2_17_23_38_21_1252 "\n ODM_CancelDownload error_code = %d"
#define ODM_508_112_2_17_23_38_21_1253 "\n ODM_DownloadConfirmationResponse error_code = %d"
#define ODM_548_112_2_17_23_38_21_1254 "\n ODM_AuthResponse error_code = %d"
#define ODM_580_112_2_17_23_38_21_1255 "\n ODM_GetSessionInfo error_code = %d"
#define ODM_611_112_2_17_23_38_21_1256 "\n ODM_SubscribeIndication error_code = %d"
#define ODM_642_112_2_17_23_38_21_1257 "\n ODM_DesubscribeIndication error_code = %d"
#define OMADL_SERIALIZE_126_112_2_17_23_38_24_1258 "\n OMADL OMADL_SerializeStartDownloadReqSigT error"
#define OMADL_UNITTEST_276_112_2_17_23_38_25_1259 "\nODMUT: Receive signal code: %d"
#define OMADL_UNITTEST_279_112_2_17_23_38_25_1260 "\nODM: Receive an signal, but do not know who sent it"
#define OMADL_UNITTEST_294_112_2_17_23_38_25_1261 "\nThrow ODM_UT_TEST_CASE message because we are testing"
#define OMADL_UNITTEST_300_112_2_17_23_38_25_1262 "\nODMUT: ODM_START_DOWNLOAD_CNF"
#define OMADL_UNITTEST_301_112_2_17_23_38_25_1263 "\nSession id: %08x"
#define OMADL_UNITTEST_302_112_2_17_23_38_25_1264 "\nMagic: %08x"
#define OMADL_UNITTEST_303_112_2_17_23_38_25_1265 "\nResult: %08x"
#define OMADL_UNITTEST_308_112_2_17_23_38_25_1266 "\nODMUT: ODM_CANCEL_DOWNLOAD_CNF"
#define OMADL_UNITTEST_309_112_2_17_23_38_25_1267 "\nSession id: %08x"
#define OMADL_UNITTEST_310_112_2_17_23_38_25_1268 "\nResult: %08x"
#define OMADL_UNITTEST_315_112_2_17_23_38_26_1269 "\nODMUT: ODM_DOWNLOAD_CONFIRMATION_IND"
#define OMADL_UNITTEST_316_112_2_17_23_38_26_1270 "\nSession id: %08x"
#define OMADL_UNITTEST_336_112_2_17_23_38_26_1271 "\nODMUT: ODM_DOWNLOAD_AUTH_IND"
#define OMADL_UNITTEST_337_112_2_17_23_38_26_1272 "\nSession id: %08x"
#define OMADL_UNITTEST_338_112_2_17_23_38_26_1273 "\nAuth type: %08x"
#define OMADL_UNITTEST_353_112_2_17_23_38_26_1274 "\n:)CASE NO AUTH FAILED\n"
#define OMADL_UNITTEST_366_112_2_17_23_38_26_1275 "\nODMUT: ODM_DOWNLOAD_PROGRESS_IND"
#define OMADL_UNITTEST_367_112_2_17_23_38_26_1276 "\nSession id: %08x"
#define OMADL_UNITTEST_368_112_2_17_23_38_26_1277 "\nStage: %d"
#define OMADL_UNITTEST_369_112_2_17_23_38_26_1278 "\nParam: %d\n"
#define OMADL_UNITTEST_374_112_2_17_23_38_26_1279 "\nODMUT: ODM_DOWNLOAD_SUCCESS_IND"
#define OMADL_UNITTEST_375_112_2_17_23_38_26_1280 "\nSession id: %08x"
#define OMADL_UNITTEST_387_112_2_17_23_38_26_1281 "\nDUAUT test finish:%d:%d\n"
#define OMADL_UNITTEST_402_112_2_17_23_38_26_1282 "\nODMUT: ODM_DOWNLOAD_ERROR_IND"
#define OMADL_UNITTEST_403_112_2_17_23_38_26_1283 "\nSession id: %08x"
#define OMADL_UNITTEST_404_112_2_17_23_38_26_1284 "\nError no: %d"
#define OMADL_UNITTEST_415_112_2_17_23_38_26_1285 "\nDUAUT test finish:%d:%d\n"
#define OMADL_UNITTEST_429_112_2_17_23_38_26_1286 "\nODMUT: ODM_DOWNLOAD_GET_SESSION_LIST_CNF"
#define OMADL_UNITTEST_431_112_2_17_23_38_26_1287 "\nCount = %d"
#define OMADL_UNITTEST_433_112_2_17_23_38_26_1288 "\nSessionID[%d] = %d"
#define OMADL_UNITTEST_460_112_2_17_23_38_26_1289 "\nODMUT: ODM_DOWNLOAD_GET_SESSION_INFO_CNF"
#define OMADL_UNITTEST_462_112_2_17_23_38_26_1290 "\ndd url: %s"
#define OMADL_UNITTEST_466_112_2_17_23_38_26_1291 "\nODMUT: ODM_DOWNLOAD_GET_SESSION_INFO_CNF"
#define OMADL_UNITTEST_468_112_2_17_23_38_26_1292 "\nResult: %08x"
#define OMADL_UNITTEST_481_112_2_17_23_38_26_1293 "\nODMUT: ODM_DESUBSCRIBE_INDICATION_CNF"
#define OMADL_UNITTEST_482_112_2_17_23_38_26_1294 "\nResult: %08x"
#define OMADL_UNITTEST_488_112_2_17_23_38_26_1295 "\nODMUT: ODM_MANAGER_PROGRESS_IND"
#define OMADL_UNITTEST_489_112_2_17_23_38_26_1296 "\nSession id: %08x"
#define OMADL_UNITTEST_490_112_2_17_23_38_26_1297 "\nStage: %d"
#define OMADL_UNITTEST_491_112_2_17_23_38_26_1298 "\nParam: %d\n"
#define OMADL_UNITTEST_496_112_2_17_23_38_26_1299 "\nODMUT: ODM_MANAGER_SUCCESS_IND"
#define OMADL_UNITTEST_497_112_2_17_23_38_26_1300 "\nSession id: %08x"
#define OMADL_UNITTEST_502_112_2_17_23_38_26_1301 "\nODMUT: ODM_MANAGER_ERROR_IND"
#define OMADL_UNITTEST_503_112_2_17_23_38_26_1302 "\nSession id: %08x"
#define OMADL_UNITTEST_504_112_2_17_23_38_26_1303 "\nError no: %d"
#define OMADL_UNITTEST_532_112_2_17_23_38_26_1304 "\nODMUT1: ProcAppUTTestSignal()"
#define OMADL_UNITTEST_533_112_2_17_23_38_26_1305 "\n case_no: %d"
#define OMADL_UNITTEST_534_112_2_17_23_38_26_1306 "\n param: %d"
#define OMADL_UNITTEST_749_112_2_17_23_38_27_1307 "\nODM UNIT TEST:%d %d\n\n"
#define OMADL_UNITTEST_768_112_2_17_23_38_27_1308 "ODMUT_StartTestOneCase:\n"
#define OMADL_UTIL_411_112_2_17_23_38_28_1309 "\nODM MEMORY TRACE START:\n"
#define OMADL_UTIL_415_112_2_17_23_38_28_1310 "(%d)Leak at: %s(%d)\n"
#define OMADL_UTIL_419_112_2_17_23_38_28_1311 "\nODM MEMORY LEAK: %d\n"
#define OMADL_UTIL_420_112_2_17_23_38_28_1312 "\nODM MEMORY TRACE END:\n"
#define WSP_INTERFACE_308_112_2_17_23_38_28_1313 "\n ODM wsp data ind file write error\n"
#define WSP_INTERFACE_381_112_2_17_23_38_29_1314 "ODM:receive wsp error ind!"
#define WSP_INTERFACE_385_112_2_17_23_38_29_1315 "ODM:receive wsp error ind! result = %d"
#define WSP_INTERFACE_646_112_2_17_23_38_29_1316 "\n ODM:wsp get document file open error\n"
#define DRM_API_75_112_2_17_23_38_30_1317 "[DRM] Command is: DRM_InstallContentReq download name =%s,new name=%sn"
#define DRM_API_109_112_2_17_23_38_30_1318 "[DRM] Command is: DRM_InstallContentReq ret=%d\n"
#define DRM_API_132_112_2_17_23_38_30_1319 "[DRM] Command is: DRM_InstallRightsReq rights=%s\n"
#define DRM_API_166_112_2_17_23_38_30_1320 "[DRM] Command is: DRM_InstallRightsReq ret=%d\n"
#define DRM_API_198_112_2_17_23_38_30_1321 "[DRM] DRM_InstallFileReq DRM_INVALID_TPYE\n"
#define DRM_API_204_112_2_17_23_38_30_1322 "[DRM] DRM_InstallFileReq ret_code=%d\n"
#define DRM_API_250_112_2_17_23_38_30_1323 "[DRM] Command is: DRM_GetContentMimeTypeReq ret=%d\n"
#define DRM_API_281_112_2_17_23_38_30_1324 "[DRM] Command is: DRM_IsDRMFile ret=%d\n"
#define DRM_API_309_112_2_17_23_38_30_1325 "[DRM] Command is: DRM_GetDRMFileType ret=%d\n"
#define DRM_API_324_112_2_17_23_38_31_1326 "[DRM] Command DRM_IsRightsExpired handle=%d permission =%d\n"
#define DRM_API_341_112_2_17_23_38_31_1327 "[DRM] Command is: DRM_IsRightsExpired ret=%d\n"
#define DRM_API_371_112_2_17_23_38_31_1328 "[DRM] Command is: DRM_GetRightsConstraint ret=%d\n"
#define DRM_API_394_112_2_17_23_38_31_1329 "[DRM] Command is: DRM_GetRightsInfo ret=%d\n"
#define DRM_API_418_112_2_17_23_38_31_1330 "[DRM] Command is: DRM_ConsumeRights ret=%d\n"
#define DRM_API_507_112_2_17_23_38_31_1331 "[DRM] Command is: DRM_GetRightsInterval ret=%d\n"
#define DRM_API_549_112_2_17_23_38_31_1332 "[DRM] Command is: DRM_DeleteRightsHandleReq ret=%d\n"
#define DRM_API_585_112_2_17_23_38_31_1333 "[DRM] Command is: DRM_CancelInstallFileReq ret=%d\n"
#define DRM_COMMON_230_112_2_17_23_38_32_1334 "\n[DRM] DRM_GetNetTime !is_net_time =%u,net_time= %u handle_type_ptr=%u\n"
#define DRM_COMMON_249_112_2_17_23_38_32_1335 "DayToMonth:error day = %d\n"
#define DRM_COMMON_761_112_2_17_23_38_33_1336 "\n[DRM] DRM_GetRightsIMEI is_suc=%d\n"
#define DRM_COMMON_812_112_2_17_23_38_33_1337 "\n[DRM] DRM_CheckDiskIsNoSpace ret=%d\n"
#define DRM_COMMON_869_112_2_17_23_38_33_1338 "DRM_IRAM_Enable:lock IRAM ERROR\n"
#define DRM_COMMON_878_112_2_17_23_38_33_1339 "DRM_IRAM_Enable:lock IRAM ret =%d\n"
#define DRM_COMMON_919_112_2_17_23_38_33_1340 "DRM_IRAM_Disable:ERROR \n"
#define DRM_COMMON_925_112_2_17_23_38_33_1341 "DRM_IRAM_Disable:unable to unlock IRAM ret =%d\n"
#define DRM_COMMON_946_112_2_17_23_38_33_1342 "DRM_IRAM_Init:INIT IRAM \n"
#define DRM_COMMON_960_112_2_17_23_38_33_1343 "DRM_IRAM_Add_Count cout =%d \n"
#define DRM_COMMON_975_112_2_17_23_38_33_1344 "DRM_IRAM_Reduce_Count cout =%d \n"
#define DRM_COMMON_996_112_2_17_23_38_33_1345 "DRM_IRAM_CHECK_IS_Ulock_Iram cout =%d \n"
#define DRM_DH_559_112_2_17_23_38_35_1346 "[DRM] Command is: DRM_SIG_DH_INSTALL_FILE_REQ\n"
#define DRM_DH_563_112_2_17_23_38_35_1347 "[DRM] Command is: DRM_SIG_DH_DELETE_RIGHTS_FILE_REQ\n"
#define DRM_DH_567_112_2_17_23_38_35_1348 "[DRM] Command is: DRM_SIG_GET_FILE_MIME_REQ\n"
#define DRM_DH_571_112_2_17_23_38_35_1349 "[DRM] Command is: DRM_SIG_CANCEL_INSTALL_FILE_REQ\n"
#define DRM_DH_575_112_2_17_23_38_35_1350 "[DRM] Command is: DRM_SIG_DEFAULT code=%d\n"
#define DRM_DH_580_112_2_17_23_38_35_1351 "[DRM] DrmDLHandlerMain ret_code = %d\n"
#define DRM_DH_624_112_2_17_23_38_35_1352 "[DRM] DeleteRightsFileByContentId FL_DeleteFile sfs ret =%d\n"
#define DRM_DH_634_112_2_17_23_38_35_1353 "[DRM] DeleteRightsFileByContentId ret=%d\n"
#define DRM_DH_784_112_2_17_23_38_35_1354 "[DRM] IsDCFFormat error_code=%d is_it_dcf=%d\n"
#define DRM_DH_812_112_2_17_23_38_35_1355 "[DRM] ParseDCF"
#define DRM_DH_843_112_2_17_23_38_35_1356 "[DRM] Mime type: %s\n"
#define DRM_DH_855_112_2_17_23_38_36_1357 "[DRM] Content id: %s\n"
#define DRM_DH_913_112_2_17_23_38_36_1358 "[DRM] Rights issuer: %s\n"
#define DRM_DH_929_112_2_17_23_38_36_1359 "[DRM] Rights issuer: %s\n"
#define DRM_DH_947_112_2_17_23_38_36_1360 "[DRM] Offset: %d, data length: %d\n"
#define DRM_DH_951_112_2_17_23_38_36_1361 "[DRM] ParseDCF ret code: %d\n"
#define DRM_DH_1035_112_2_17_23_38_36_1362 "[DRM] OpenDrmEntityInfoFile start\n"
#define DRM_DH_1083_112_2_17_23_38_36_1363 "[DRM] OpenDrmEntityInfoFile handle: %0x\n"
#define DRM_DH_1127_112_2_17_23_38_36_1364 "[DRM] SaveDrmEntityInfoToFile: result %d\n"
#define DRM_DH_1152_112_2_17_23_38_36_1365 "[DRM] LoadDrmEntityInfoFromFile no memery!\n"
#define DRM_DH_1198_112_2_17_23_38_36_1366 "[DRM] LoadDrmEntityInfoFromFile: DRM_STATUS_RIGHTS_PRESENT\n"
#define DRM_DH_1236_112_2_17_23_38_36_1367 "[DRM] LoadDrmEntityInfoFromFile: ret =%d\n"
#define DRM_DH_1252_112_2_17_23_38_36_1368 "[DRM] Count: %d\n"
#define DRM_DH_1253_112_2_17_23_38_36_1369 "[DRM] Used count: %d\n"
#define DRM_DH_1258_112_2_17_23_38_36_1370 "[DRM] Interval: %d\n"
#define DRM_DH_1259_112_2_17_23_38_36_1371 "[DRM] Start Interval: %d\n"
#define DRM_DH_1260_112_2_17_23_38_36_1372 "[DRM] End Interval: %d\n"
#define DRM_DH_1265_112_2_17_23_38_36_1373 "[DRM] Start time: %d\n"
#define DRM_DH_1266_112_2_17_23_38_36_1374 "[DRM] End   time: %d\n"
#define DRM_DH_1281_112_2_17_23_38_36_1375 "\n[DRM] Content id: %s\n"
#define DRM_DH_1282_112_2_17_23_38_36_1376 "[DRM] Rights issuer: %s\n"
#define DRM_DH_1283_112_2_17_23_38_36_1377 "[DRM] Mime type: %d\n"
#define DRM_DH_1284_112_2_17_23_38_36_1378 "[DRM] Status: %d\n"
#define DRM_DH_1285_112_2_17_23_38_36_1379 "[DRM] Mehod Level:%d\n"
#define DRM_DH_1294_112_2_17_23_38_36_1380 "[DRM]  permission\n"
#define DRM_DH_1300_112_2_17_23_38_36_1381 "[DRM] No permission info\n"
#define DRM_DH_1331_112_2_17_23_38_36_1382 "[DRM] ParseDrmMsg\n"
#define DRM_DH_1382_112_2_17_23_38_37_1383 "[DRM] Mime type: %s\n"
#define DRM_DH_1430_112_2_17_23_38_37_1384 "[DRM] Encoding type: %d\n"
#define DRM_DH_1475_112_2_17_23_38_37_1385 "[DRM] ParseDrmMsg ret code: %d\n"
#define DRM_DH_1588_112_2_17_23_38_37_1386 "[DRM] ProcDrmMsgFL\n"
#define DRM_DH_1602_112_2_17_23_38_37_1387 "[DRM] Create file failed\n"
#define DRM_DH_1611_112_2_17_23_38_37_1388 "[DRM] DRM DH: disk no space."
#define DRM_DH_1674_112_2_17_23_38_37_1389 "[DRM] ProcDrmMsgFL, result = %d\n"
#define DRM_DH_1704_112_2_17_23_38_37_1390 "[DRM] SaveDrmMsgToFile\n"
#define DRM_DH_1807_112_2_17_23_38_37_1391 "[DRM] SaveDrmMsgToFile, result = %d\n"
#define DRM_DH_1830_112_2_17_23_38_37_1392 "[DRM] ProcDrmMsgCD\n"
#define DRM_DH_1844_112_2_17_23_38_37_1393 "[DRM] DRM DH: Can not open input file."
#define DRM_DH_1853_112_2_17_23_38_37_1394 "[DRM] DRM DH: disk no space."
#define DRM_DH_1910_112_2_17_23_38_38_1395 "[DRM] Content id = %s\n"
#define DRM_DH_1942_112_2_17_23_38_38_1396 "[DRM] ProcDrmMsgCD, result = %d\n"
#define DRM_DH_1964_112_2_17_23_38_38_1397 "[DRM] ProcDrmContent\n"
#define DRM_DH_1976_112_2_17_23_38_38_1398 "[DRM] DH: Can not open input file."
#define DRM_DH_1985_112_2_17_23_38_38_1399 "[DRM] DRM DH: disk no space."
#define DRM_DH_2023_112_2_17_23_38_38_1400 "[DRM] ProcDrmContent, result = %d\n"
#define DRM_DH_2094_112_2_17_23_38_38_1401 "[DRM] SaveDcfFile, result = %d\n"
#define DRM_DH_2175_112_2_17_23_38_38_1402 "[DRM] CopyDcfFile, result = %d\n"
#define DRM_DH_2195_112_2_17_23_38_38_1403 "[DRM] ProcDrmMsgDCF\n"
#define DRM_DH_2227_112_2_17_23_38_38_1404 "[DRM] ProcDrmMsgDCF, result = %d\n"
#define DRM_DH_2302_112_2_17_23_38_38_1405 "[DRM] GetMethodLevel, level = %d\n"
#define DRM_DH_2332_112_2_17_23_38_38_1406 "[DRM] DH: Can not open input file."
#define DRM_DH_2379_112_2_17_23_38_39_1407 "[DRM] ProcDrmRights, result = %d\n"
#define DRM_DH_2451_112_2_17_23_38_39_1408 "[DRM] DRM_SendInstallCnfToApp\n"
#define DRM_DH_2478_112_2_17_23_38_39_1409 "[DRM] DRM_ProcDrmFile\n"
#define DRM_DH_2565_112_2_17_23_38_39_1410 "[DRM] DRM_ProcDrmFile, error_code = %d\n"
#define DRM_DH_2586_112_2_17_23_38_39_1411 "[DRM] ProcRightsInBuff \n"
#define DRM_DH_2596_112_2_17_23_38_39_1412 "[DRM] ProcRightsInBuff type =%d \n"
#define DRM_DH_2601_112_2_17_23_38_39_1413 "[DRM] DRM DH: disk no space."
#define DRM_DH_2642_112_2_17_23_38_39_1414 "[DRM] ProcRightsInBuff ret=%d\n"
#define DRM_DH_2674_112_2_17_23_38_39_1415 "[DRM] GetContentMimeTypeOfDrmContent, mime_type = %d\n"
#define DRM_DH_2704_112_2_17_23_38_39_1416 "[DRM] DH: Can not open input file."
#define DRM_DH_2739_112_2_17_23_38_39_1417 "[DRM] GetContentMimeTypeOfDrmMsgCD, mime_type = %d\n"
#define DRM_DH_2771_112_2_17_23_38_39_1418 "[DRM] GetContentMimeTypeOfDrmMsgDCF, mime_type = %d\n"
#define DRM_DH_2831_112_2_17_23_38_39_1419 "[DRM] GetContentMimeTypeOfDrmMsgFL, mime_type = %d\n"
#define DRM_DH_2912_112_2_17_23_38_40_1420 "[DRM] DRM_ContentMimeCNFToApp\n"
#define DRM_DH_2949_112_2_17_23_38_40_1421 "[DRM] DRM_ProContentMimeType ret=%d\n"
#define DRM_DH_2979_112_2_17_23_38_40_1422 "[DRM] SaveDrmMsgInPrivateFormat !\n"
#define DRM_DH_3097_112_2_17_23_38_40_1423 "\n[DRM] ReadDrmEntityInfoFormFile ret =%d\n"
#define DRM_DH_3174_112_2_17_23_38_40_1424 "\n[DRM] CreateDrmEntityInfoFile create handle error !\n"
#define DRM_DH_3182_112_2_17_23_38_40_1425 "\n[DRM] CreateDrmEntityInfoFile memery alloc error !\n"
#define DRM_DH_3187_112_2_17_23_38_40_1426 "\n[DRM] CreateDrmEntityInfoFile !handle =%0x08\n"
#define DRM_DH_3308_112_2_17_23_38_40_1427 "\n[DRM] UpdateDrmConstraint !\n"
#define DRM_DH_3342_112_2_17_23_38_40_1428 "\n[DRM] UpdateDrmRights !\n"
#define DRM_DH_3380_112_2_17_23_38_41_1429 "[DRM] DCFSDWriteDrmEntityInfo ret =%d\n"
#define DRM_DH_3428_112_2_17_23_38_41_1430 "[DRM] DCFSDReadDrmEntityInfo ret =%d\n"
#define DRM_DH_3586_112_2_17_23_38_41_1431 "\n[DRM] ProcDcfDrmEntityInfo ret=%d\n"
#define DRM_DH_3630_112_2_17_23_38_41_1432 "\n[DRM] GenerateContentID !\n"
#define DRM_DH_3662_112_2_17_23_38_41_1433 "\n[DRM] GenerateKey !\n"
#define DRM_DH_3825_112_2_17_23_38_41_1434 "\n[DRM] DrmBase64ToBinary ret=%d\n"
#define DRM_DH_3900_112_2_17_23_38_42_1435 "[DRM] DRM_ProDeleteRightsFile ret=%d\n"
#define DRM_DH_3942_112_2_17_23_38_42_1436 "[DRM] DRM_DeleteALLRightsFile ret=%d\n"
#define DRM_DH_4000_112_2_17_23_38_42_1437 "[DRM] DRM_DeleteInvalidRightsFile ret=%d\n"
#define DRM_DH_4028_112_2_17_23_38_42_1438 "[DRM] DRM_DeleteRightsCNFToApp\n"
#define DRM_DH_4094_112_2_17_23_38_42_1439 "[DRM] DRM_ProCancelInstall error_code =%d\n"
#define DRM_DH_4123_112_2_17_23_38_42_1440 "[DRM] ParseDrmCDWbxmlMsg\n"
#define DRM_DH_4167_112_2_17_23_38_42_1441 "[DRM] Mime type: %s\n"
#define DRM_DH_4285_112_2_17_23_38_42_1442 "[DRM] Encoding type: %d\n"
#define DRM_DH_4298_112_2_17_23_38_42_1443 "[DRM] Mime type: %s\n"
#define DRM_DH_4349_112_2_17_23_38_42_1444 "[DRM] ParseDrmMsg ret code: %d\n"
#define DRM_DH_4414_112_2_17_23_38_43_1445 "[DRM] GetMethodIsCDWbxml is wbxml = %d\n"
#define DRM_IO_292_112_2_17_23_38_44_1446 "[DRM] Cmd: DRM_IRP_IO_CREATE_FILE\n"
#define DRM_IO_326_112_2_17_23_38_44_1447 "[DRM] Cmd: DRM_IRP_IO_CLOSE_FILE\n"
#define DRM_IO_331_112_2_17_23_38_44_1448 "[DRM] Cmd: DRM_SIG_IO_DELETE_FILE\n"
#define DRM_IO_336_112_2_17_23_38_44_1449 "[DRM] Cmd: DRM_IRP_IO_SET_FILE_POINTER\n"
#define DRM_IO_344_112_2_17_23_38_44_1450 "[DRM] Cmd: DRM_IRP_IO_GET_FILE_POINTER\n"
#define DRM_IO_355_112_2_17_23_38_44_1451 "[DRM] Cmd: DRM_IRP_IO_GET_FILE_SIZE\n"
#define DRM_IO_364_112_2_17_23_38_44_1452 "[DRM] Cmd: DRM_SIG_IO_CONSUME_FILE_RIGHTS\n"
#define DRM_IO_370_112_2_17_23_38_44_1453 "[DRM] Cmd: DRM_SIG_IO_SD_ENCRPT_READ\n"
#define DRM_IO_377_112_2_17_23_38_44_1454 "[DRM] Cmd: DRM_SIG_IO_GET_FILE_RIGHTS\n"
#define DRM_IO_384_112_2_17_23_38_44_1455 "[DRM] Cmd: DRM IO default \n"
#define DRM_IO_433_112_2_17_23_38_44_1456 "[DRM] Cmd: DRM_SIG_CANCEL_INSTALL_FILE_REQ \n"
#define DRM_IO_437_112_2_17_23_38_44_1457 "[DRM] Cmd: DRM IO default \n"
#define DRM_IO_487_112_2_17_23_38_44_1458 "[DRM] ProcCreateFileReq ret=%d\n"
#define DRM_IO_539_112_2_17_23_38_44_1459 "[DRM] OpenDCFFile CheckDrmRights is TRUE \n"
#define DRM_IO_551_112_2_17_23_38_44_1460 "[DRM] OpenDCFFile FL_DeleteFile expired rights file\n"
#define DRM_IO_553_112_2_17_23_38_44_1461 "[DRM] OpenDCFFile CheckDrmRights is FALSE \n"
#define DRM_IO_558_112_2_17_23_38_44_1462 "[DRM] OpenDCFFile LoadDrmEntityInfoFromFile is FALSE \n"
#define DRM_IO_578_112_2_17_23_38_44_1463 "[DRM] OpenDCFFile ParseDCF is TRUE file method_level is =%d\n"
#define DRM_IO_583_112_2_17_23_38_44_1464 "[DRM] OpenDCFFile ParseDCF is FALSE \n"
#define DRM_IO_612_112_2_17_23_38_44_1465 "[DRM] OpenDCFFile drm_io_context_ptr io_impl_ptr is NULL \n"
#define DRM_IO_614_112_2_17_23_38_44_1466 "[DRM] OpenDCFFile drm_io_context_ptr->is_allow_io is true \n"
#define DRM_IO_631_112_2_17_23_38_44_1467 "[DRM] OpenDCFFile ret=%d\n"
#define DRM_IO_667_112_2_17_23_38_44_1468 "[DRM] ProcReadFileReq ret=%d\n"
#define DRM_IO_711_112_2_17_23_38_44_1469 "[DRM] ProcCloseFileReq ret=%d\n"
#define DRM_IO_760_112_2_17_23_38_44_1470 "[DRM] ProcDeleteFileReq ret=%d\n"
#define DRM_IO_795_112_2_17_23_38_45_1471 "[DRM] ProcSetFilePointerReq ret=%d\n"
#define DRM_IO_830_112_2_17_23_38_45_1472 "[DRM] ProcGetFilePointerReq ret=%d\n"
#define DRM_IO_863_112_2_17_23_38_45_1473 "[DRM] ProcGetFileSizeReq ret =%d\n"
#define DRM_IO_896_112_2_17_23_38_45_1474 "[DRM] ProcGetFileSizeReq ret =%d\n"
#define DRM_IO_932_112_2_17_23_38_45_1475 "[DRM] ConsumeRights  cur system time =%u net time =%u cur_nettime_handle_type =%u\n"
#define DRM_IO_1089_112_2_17_23_38_45_1476 "[DRM] CheckDrmRightsConstraint cursystemtime=%u nettime=%u cur_nettime_hanle_type=%d\n"
#define DRM_IO_1215_112_2_17_23_38_45_1477 "[DRM] ProcSDEncrptReadReq ret =%d\n"
#define DRM_IO_1273_112_2_17_23_38_45_1478 "[DRM] ProcGetFileRights ret =%d\n"
#define DRM_IO_IMPL_SD_255_112_2_17_23_38_48_1479 "[DRM] SDDrmIoOpenImpl ret =%d \n"
#define DRM_IO_IMPL_SD_336_112_2_17_23_38_48_1480 "[DRM] SDDrmIoCloseImpl ret =%d \n"
#define DRM_IO_IMPL_SD_416_112_2_17_23_38_48_1481 "[DRM] SDDrmIoSetPosImpl offset =%d  origin =%d\n"
#define DRM_IO_IMPL_SD_480_112_2_17_23_38_48_1482 "[DRM] SDDrmIoGetPosImpl ret =%d origin =%d offset =%d\n"
#define DRM_IO_IMPL_SD_517_112_2_17_23_38_48_1483 "[DRM] SDDrmIoGetFileSize size =%d \n"
#define DRM_IO_IMPL_SD_734_112_2_17_23_38_49_1484 "[DRM] DrmReadDecryptData postion =%d postion_num =%d postion_lit =%d bytes_to_read =%d bytes_read =%dn"
#define DRM_IO_IMPL_SD_740_112_2_17_23_38_49_1485 "[DRM] DrmRemovePadding\n"
#define DRM_IO_IMPL_SD_764_112_2_17_23_38_49_1486 "[DRM] DrmReadDecryptData ret=%d bytes_to_read =%d bytes_read =%d\n"
#define DRM_IO_IMPL_SD_796_112_2_17_23_38_49_1487 " SD DecryptData 0x=%0x"
#define DRM_SFS_278_112_2_17_23_38_50_1488 "[DRM] DRM_CreateFile Open DRM file success, drm_handle = %08x\n"
#define DRM_SFS_303_112_2_17_23_38_50_1489 "[DRM] DRM_CreateFile Open normal file success, drm_handle = %08x\n"
#define DRM_SFS_365_112_2_17_23_38_50_1490 "[DRM] DRM_CreateFileAsyn Open DRM file success, drm_handle = %08x\n"
#define DRM_SFS_389_112_2_17_23_38_50_1491 "[DRM] DRM_CreateFileAsyn Open normal file success, handle = %08x\n"
#define DRM_SFS_395_112_2_17_23_38_50_1492 "[DRM] DRM_CreateFileAsyn Open normal file success, handle = %08x\n"
#define DRM_SFS_401_112_2_17_23_38_50_1493 "[DRM] DRM_CreateFileAsyn Open file success, handle = %08x\n"
#define DRM_SFS_676_112_2_17_23_38_50_1494 "[DRM] DRM_CloseFile drm file error_code =%d\n"
#define DRM_SFS_682_112_2_17_23_38_50_1495 "[DRM] DRM_CloseFile normal file error_code =%d\n"
#define DRM_SFS_689_112_2_17_23_38_50_1496 "[DRM] DRM_CloseFile error_code =%d\n"
#define DRM_SFS_757_112_2_17_23_38_51_1497 "[DRM] DRM_DeleteFile\n"
#define DRM_SFS_764_112_2_17_23_38_51_1498 "[DRM] Delete DRM file success\n"
#define DRM_SFS_775_112_2_17_23_38_51_1499 "[DRM] delete normal file success\n"
#define DRM_SFS_1327_112_2_17_23_38_52_1500 "[DRM] DRM_SDSetEncrypt ret =%d\n"
#define DRM_SFS_1421_112_2_17_23_38_52_1501 "[DRM] DRM_GetRightsHandle ret =%d\n"
#define DRM_SFS_1545_112_2_17_23_38_52_1502 "[DRM] Invalid irp 0x%x"
#define DRM_SFS_1583_112_2_17_23_38_52_1503 "[DRM] DrmCallbackFunc default\n"
#define DRM_SFS_1603_112_2_17_23_38_52_1504 "[DRM] DRM DrmIoDefaultParam ,task id is invalid\n"
#define DRM_SFS_1753_112_2_17_23_38_53_1505 "[DRM] DRM 1 priorityCur ERROR \n"
#define DRM_SFS_1770_112_2_17_23_38_53_1506 "[DRM] DRM 2 priorityCurFFS ERROR \n"
#define DRM_SFS_1780_112_2_17_23_38_53_1507 "[DRM] DRM 3 old_priority_ptr ERROR \n"
#define DRM_SFS_1812_112_2_17_23_38_53_1508 "[DRM] DRM DrmIoWaitComplete failed ,OS error\n"
#define DRM_SFS_1818_112_2_17_23_38_53_1509 "[DRM] DrmIoSendIrp Send signal failed \n"
#define DRM_SFS_1829_112_2_17_23_38_53_1510 "[DRM] DRM FFS 4 old_priority_ptr ERROR \n"
#define DRM_SFS_1849_112_2_17_23_38_53_1511 "[DRM] DrmCheckIsDRM file name is NULL\n"
#define DRM_SFS_1861_112_2_17_23_38_53_1512 "[DRM] DrmCheckIsDRM  ret_val=%d\n"
#define DRM_SFS_1886_112_2_17_23_38_53_1513 "[DRM] DrmParseDCF  ret_val=%d\n"
#define DRM_UNIT_TEST_224_112_2_17_23_38_53_1514 "DRM_IO_CALLBACK DRM_IO_CALLBACK\n"
#define DRM_XML_593_112_2_17_23_38_55_1515 "[DRM] DRM XML parse error code: %d\n"
#define DRM_XML_598_112_2_17_23_38_55_1516 "[DRM] DRM XML parse version id is error"
#define DRM_XML_604_112_2_17_23_38_55_1517 "[DRM] DRM ParseCDXmlRights ret %d\n"
#define DRM_XML_671_112_2_17_23_38_55_1518 "[DRM] DRM XML parse error code: %d\n"
#define DRM_XML_676_112_2_17_23_38_55_1519 "[DRM] DRM XML parse version id is error"
#define DRM_XML_680_112_2_17_23_38_55_1520 "[DRM] DRM ParseXmlRightsInBuff ret %d\n"
#define DRM_XML_772_112_2_17_23_38_55_1521 "[DRM] DRM XML parse error code: %d\n"
#define DRM_XML_777_112_2_17_23_38_55_1522 "[DRM] DRM XML parse version id is error"
#define DRM_XML_781_112_2_17_23_38_55_1523 "[DRM] DRM ParseXmlRights ret %d\n"
#define DRM_XML_1090_112_2_17_23_38_56_1524 "\n[DRM] Content ID: %s\n"
#define DRM_XML_1773_112_2_17_23_38_57_1525 "\n[DRM] Count: %s\n"
#define DRM_XML_1790_112_2_17_23_38_57_1526 "\n[DRM] Interval: %s\n"
#define DRM_XML_1806_112_2_17_23_38_58_1527 "\n[DRM] Start time: %s\n"
#define DRM_XML_1813_112_2_17_23_38_58_1528 "\n[DRM] End time: %s\n"
#define DRM_XML_2072_112_2_17_23_38_58_1529 "[DRM] Parse Interval: %d year %d month %d day %d hour %d minute %d seconds\n"
#define DRM_XML_2094_112_2_17_23_38_58_1530 "[DRM] Value: %d\n"
#define DRM_XML_2170_112_2_17_23_38_58_1531 "[DRM] Parse Interval: %d year %d month %d day %d hour %d minute %d seconds\n"
#define DRM_XML_2174_112_2_17_23_38_58_1532 "[DRM] Value: %d"
#define DRM_XML_2347_112_2_17_23_38_59_1533 "[DRM] ParseWBXML failed: %d"
#define DRM_XML_2353_112_2_17_23_38_59_1534 "[DRM] ParseWBXML version id failed "
#define DRM_XML_2459_112_2_17_23_38_59_1535 "[DRM] ParseWBXML failed: %d"
#define DRM_XML_2465_112_2_17_23_38_59_1536 "[DRM] ParseWBXML version id failed "
#define PUA_MAIN_172_112_2_17_23_39_3_1537 "PUA: receive unknown sip message from: %08x\n"
#define PUA_MAIN_177_112_2_17_23_39_3_1538 "[ PUA ] Receive unknown message from: %08x\n"
#define PUA_MAIN_256_112_2_17_23_39_3_1539 "[ PUBLISH ] PUA_PRES_SRC_CREATE_REQ (APP -> PUA)"
#define PUA_MAIN_265_112_2_17_23_39_3_1540 "[ PUBLISH ] PUA_PRES_SRC_CREATE_CNF (PUA -> APP)"
#define PUA_MAIN_266_112_2_17_23_39_3_1541 "[ PUBLISH ] Error code: %d"
#define PUA_MAIN_267_112_2_17_23_39_3_1542 "[ PUBLISH ] Module id: %08x"
#define PUA_MAIN_268_112_2_17_23_39_3_1543 "[ PUBLISH ] Instance id: %08x"
#define PUA_MAIN_277_112_2_17_23_39_3_1544 "[ PUBLISH ] PUA_PRES_SRC_CLOSE_REQ (APP -> PUA)"
#define PUA_MAIN_278_112_2_17_23_39_3_1545 "[ PUBLISH ] Instance id: %08x"
#define PUA_MAIN_287_112_2_17_23_39_3_1546 "[ PUBLISH ] PUA_PRES_SRC_CLOSE_CNF (PUA -> APP)"
#define PUA_MAIN_288_112_2_17_23_39_3_1547 "[ PUBLISH ] Error code: %d"
#define PUA_MAIN_289_112_2_17_23_39_3_1548 "[ PUBLISH ] Instance id: %08x"
#define PUA_MAIN_299_112_2_17_23_39_3_1549 "[ PUBLISH ] PUA_PRES_SRC_PUB_REQ (APP -> PUA)"
#define PUA_MAIN_300_112_2_17_23_39_3_1550 "[ PUBLISH ] Instance id: %08x"
#define PUA_MAIN_309_112_2_17_23_39_4_1551 "[ PUBLISH ] PUA_PRES_SRC_PUB_CNF (PUA -> APP)"
#define PUA_MAIN_310_112_2_17_23_39_4_1552 "[ PUBLISH ] Error code: %d"
#define PUA_MAIN_311_112_2_17_23_39_4_1553 "[ PUBLISH ] Instance id: %08x"
#define PUA_MAIN_320_112_2_17_23_39_4_1554 "[ PUBLISH ] PUA_PRES_SRC_UNPUB_REQ (APP -> PUA)"
#define PUA_MAIN_321_112_2_17_23_39_4_1555 "[ PUBLISH ] Instance id: %08x"
#define PUA_MAIN_330_112_2_17_23_39_4_1556 "[ PUBLISH ] PUA_PRES_SRC_UNPUB_CNF (PUA -> APP)"
#define PUA_MAIN_331_112_2_17_23_39_4_1557 "[ PUBLISH ] Error code: %d"
#define PUA_MAIN_332_112_2_17_23_39_4_1558 "[ PUBLISH ] Instance id: %08x"
#define PUA_MAIN_338_112_2_17_23_39_4_1559 "[ PUBLISH ] PUA_PRES_SRC_AUTH_IND (PUA -> APP)"
#define PUA_MAIN_344_112_2_17_23_39_4_1560 "[ PUBLISH ] PUA_PRES_SRC_AUTH_RSP (APP -> PUA)"
#define PUA_MAIN_350_112_2_17_23_39_4_1561 "[ PUBLISH ] PUA_PRES_SRC_PUBLISHING_IND (PUA -> APP)"
#define PUA_MAIN_356_112_2_17_23_39_4_1562 "[ PUBLISH ] PUA_PRES_SRC_REDIRECTED_IND (PUA -> APP)"
#define PUA_MAIN_365_112_2_17_23_39_4_1563 "[ PUBLISH ] PUA_PRES_SRC_ERROR_IND (PUA -> APP)"
#define PUA_MAIN_366_112_2_17_23_39_4_1564 "[ PUBLISH ] Error code: %d"
#define PUA_MAIN_367_112_2_17_23_39_4_1565 "[ PUBLISH ] Instance id: %08x"
#define PUA_MAIN_373_112_2_17_23_39_4_1566 "[ PUBLISH ] PUA_PRES_SRC_REF_IND (PUA -> APP)"
#define PUA_MAIN_382_112_2_17_23_39_4_1567 "[ PUBLISH ] PUA_PRES_SRC_TERMINATED_IND (PUA -> APP)"
#define PUA_MAIN_383_112_2_17_23_39_4_1568 "[ PUBLISH ] Instance id: %08x"
#define PUBLISH_HSM_630_112_2_17_23_39_7_1569 "[ PUBLISH ] IdleState Entry"
#define PUBLISH_HSM_635_112_2_17_23_39_7_1570 "[ PUBLISH ] IdleState Exit"
#define PUBLISH_HSM_720_112_2_17_23_39_7_1571 "[ PUBLISH ] InitingState Entry"
#define PUBLISH_HSM_725_112_2_17_23_39_7_1572 "[ PUBLISH ] InitingState Exit"
#define PUBLISH_HSM_872_112_2_17_23_39_7_1573 "[ PUBLISH ] InitUnpubPendingState Entry"
#define PUBLISH_HSM_877_112_2_17_23_39_7_1574 "[ PUBLISH ] InitUnpubPendingState Exit"
#define PUBLISH_HSM_1071_112_2_17_23_39_8_1575 "[ PUBLISH ] PubProcState Entry"
#define PUBLISH_HSM_1076_112_2_17_23_39_8_1576 "[ PUBLISH ] PubProcState Exit"
#define PUBLISH_HSM_1284_112_2_17_23_39_8_1577 "[ PUBLISH ] TerminatingState Entry"
#define PUBLISH_HSM_1289_112_2_17_23_39_8_1578 "[ PUBLISH ] TerminatingState Exit"
#define PUBLISH_HSM_1336_112_2_17_23_39_8_1579 "[ PUBLISH ] TerminatedState Entry"
#define PUBLISH_HSM_1341_112_2_17_23_39_8_1580 "[ PUBLISH ] TerminatedState Exit"
#define PUBLISH_HSM_1384_112_2_17_23_39_8_1581 "[ PUBLISH ] PubProcPublishingState Entry"
#define PUBLISH_HSM_1389_112_2_17_23_39_8_1582 "[ PUBLISH ] PubProcPublishingState Exit"
#define PUBLISH_HSM_1512_112_2_17_23_39_8_1583 "[ PUBLISH ] PubProcPublishedState Entry"
#define PUBLISH_HSM_1520_112_2_17_23_39_8_1584 "[ PUBLISH ] PubProcPublishedState Exit"
#define PUBLISH_HSM_1626_112_2_17_23_39_9_1585 "[ PUBLISH ] PubProcFailedState Entry"
#define PUBLISH_HSM_1631_112_2_17_23_39_9_1586 "[ PUBLISH ] PubProcFailedState Exit"
#define PUBLISH_HSM_1710_112_2_17_23_39_9_1587 "[ PUBLISH ] PubProcRefreshingState Entry"
#define PUBLISH_HSM_1715_112_2_17_23_39_9_1588 "[ PUBLISH ] PubProcRefreshingState Exit"
#define PUBLISH_HSM_1880_112_2_17_23_39_9_1589 "[ PUBLISH ] PubProcPubPendingState Entry"
#define PUBLISH_HSM_1885_112_2_17_23_39_9_1590 "[ PUBLISH ] PubProcPubPendingState Exit"
#define PUBLISH_HSM_2006_112_2_17_23_39_9_1591 "[ PUBLISH ] PubProcUnpubPendingState Entry"
#define PUBLISH_HSM_2011_112_2_17_23_39_9_1592 "[ PUBLISH ] PubProcUnpubPendingState Exit"
#define SUBS_HSM_597_112_2_17_23_39_11_1593 "[ SUBSCRIBE ] SubscribeHSM_IdleState Entry"
#define SUBS_HSM_602_112_2_17_23_39_11_1594 "[ SUBSCRIBE ] SubscribeHSM_IdleState Exit"
#define SUBS_HSM_650_112_2_17_23_39_11_1595 "[ SUBSCRIBE ] SubscribeHSM_InitingState Entry"
#define SUBS_HSM_655_112_2_17_23_39_11_1596 "[ SUBSCRIBE ] SubscribeHSM_InitingState Exit"
#define SUBS_HSM_690_112_2_17_23_39_11_1597 "[ PUA WATCHER ] session id: %d"
#define SUBS_HSM_796_112_2_17_23_39_11_1598 "[ SUBSCRIBE ] SubscribeHSM_InitUnsubPendingState Entry"
#define SUBS_HSM_801_112_2_17_23_39_11_1599 "[ SUBSCRIBE ] SubscribeHSM_InitUnsubPendingState Exit"
#define SUBS_HSM_938_112_2_17_23_39_12_1600 "[ SUBSCRIBE ] SubscribeHSM_UnsubscribingState Entry"
#define SUBS_HSM_943_112_2_17_23_39_12_1601 "[ SUBSCRIBE ] SubscribeHSM_UnsubscribingState Exit"
#define SUBS_HSM_1009_112_2_17_23_39_12_1602 "[ SUBSCRIBE ] SubscribeHSM_TerminatingState Entry"
#define SUBS_HSM_1014_112_2_17_23_39_12_1603 "[ SUBSCRIBE ] SubscribeHSM_TerminatingState Exit"
#define SUBS_HSM_1058_112_2_17_23_39_12_1604 "[ SUBSCRIBE ] SubscribeHSM_TerminatedState Entry"
#define SUBS_HSM_1063_112_2_17_23_39_12_1605 "[ SUBSCRIBE ] SubscribeHSM_TerminatedState Exit"
#define SUBS_HSM_1114_112_2_17_23_39_12_1606 "[ SUBSCRIBE ] SubscribeHSM_SubsProcState Entry"
#define SUBS_HSM_1119_112_2_17_23_39_12_1607 "[ SUBSCRIBE ] SubscribeHSM_SubsProcState Exit"
#define SUBS_HSM_1301_112_2_17_23_39_12_1608 "[ SUBSCRIBE ] SubscribeHSM_SubsProcSubscribingState Entry"
#define SUBS_HSM_1306_112_2_17_23_39_12_1609 "[ SUBSCRIBE ] SubscribeHSM_SubsProcSubscribingState Exit"
#define SUBS_HSM_1346_112_2_17_23_39_12_1610 "PUA: unknown subscription state"
#define SUBS_HSM_1347_112_2_17_23_39_12_1611 "PUA: terminate it now"
#define SUBS_HSM_1413_112_2_17_23_39_13_1612 "[ SUBSCRIBE ] SubscribeHSM_SubsProc2XXRCVDState Entry"
#define SUBS_HSM_1418_112_2_17_23_39_13_1613 "[ SUBSCRIBE ] SubscribeHSM_SubsProc2XXRCVDState Exit"
#define SUBS_HSM_1467_112_2_17_23_39_13_1614 "PUA: unknown subscription state"
#define SUBS_HSM_1468_112_2_17_23_39_13_1615 "PUA: terminate it now"
#define SUBS_HSM_1498_112_2_17_23_39_13_1616 "[ SUBSCRIBE ] SubscribeHSM_SubsProcPendingState Entry"
#define SUBS_HSM_1503_112_2_17_23_39_13_1617 "[ SUBSCRIBE ] SubscribeHSM_SubsProcPendingState Exit"
#define SUBS_HSM_1549_112_2_17_23_39_13_1618 "PUA: unknown subscription state"
#define SUBS_HSM_1550_112_2_17_23_39_13_1619 "PUA: terminate it now"
#define SUBS_HSM_1585_112_2_17_23_39_13_1620 "[ SUBSCRIBE ] SubscribeHSM_SubsProcNotifyRCVDState Entry"
#define SUBS_HSM_1590_112_2_17_23_39_13_1621 "[ SUBSCRIBE ] SubscribeHSM_SubsProcNotifyRCVDState Exit"
#define SUBS_HSM_1671_112_2_17_23_39_13_1622 "[ SUBSCRIBE ] SubscribeHSM_SubsProcActiveState Entry"
#define SUBS_HSM_1676_112_2_17_23_39_13_1623 "[ SUBSCRIBE ] SubscribeHSM_SubsProcActiveState Exit"
#define SUBS_HSM_1719_112_2_17_23_39_13_1624 "PUA: unknown subscription state"
#define SUBS_HSM_1720_112_2_17_23_39_13_1625 "PUA: terminate it now"
#define SUBS_HSM_1756_112_2_17_23_39_13_1626 "[ SUBSCRIBE ] SubscribeHSM_SubsProcRefreshingState Entry"
#define SUBS_HSM_1761_112_2_17_23_39_13_1627 "[ SUBSCRIBE ] SubscribeHSM_SubsProcRefreshingState Exit"
#define SUBS_HSM_1800_112_2_17_23_39_13_1628 "[ SUBSCRIBE ] SubscribeHSM_SubsProcUnsubPendingState Entry"
#define SUBS_HSM_1805_112_2_17_23_39_13_1629 "[ SUBSCRIBE ] SubscribeHSM_SubsProcUnsubPendingState Exit"
#define XML_PIDF_67_112_2_17_23_39_18_1630 "Context initialization failed.\n"
#define XML_PIDF_125_112_2_17_23_39_18_1631 "Context initialization failed.\n"
#define XML_PIDF_135_112_2_17_23_39_18_1632 "encoded XML message:\n"
#define XML_PIDF_140_112_2_17_23_39_18_1633 "Encoding failed\n"
#define MAINAPP_1028_112_2_17_23_39_21_1634 "StartIdleTimer: focus and flip!"
#define MAINAPP_1048_112_2_17_23_39_21_1635 "StartIdleTimer: the idle timer has started!"
#define MAINAPP_1109_112_2_17_23_39_21_1636 "StartIdleTimer: the idle timer has stoped!"
#define MAINAPP_1155_112_2_17_23_39_21_1637 "MAIN_SetIdleBlueToothState: the blue tooth status is same!"
#define MAINAPP_1182_112_2_17_23_39_21_1638 "MAIN_SetIdleBlueToothState: the blue tooth status is same!"
#define MAINAPP_1223_112_2_17_23_39_21_1639 "MAIN_SetIdleNetWorkName: the name id is %d!"
#define MAINAPP_1252_112_2_17_23_39_21_1640 "MAIN_SetIdleServiceProvidorName: tha name len = %d, is ucs2 = %d"
#define MAINAPP_1383_112_2_17_23_39_21_1641 "MAIN_SetIdleChargeState: the is_charge is same!"
#define MAINAPP_1477_112_2_17_23_39_21_1642 "MAIN_SetIdleSmsState: the sms_state =%d is same!"
#define MAINAPP_1512_112_2_17_23_39_21_1643 "MAIN_SetIdleCallState: the call_state is same!"
#define MAINAPP_1525_112_2_17_23_39_22_1644 "MAIN_SetIdleGprsState: gprs_state = %d"
#define MAINAPP_1829_112_2_17_23_39_22_1645 "MAIN_HandleSysMsg: begin MISC_MSG_RTC_MIN!"
#define MAINAPP_1909_112_2_17_23_39_22_1646 "MAIN_HandleSysMsg: can't handle the msg!"
#define MAINAPP_1972_112_2_17_23_39_22_1647 "DrawFmName s_fm_rect.left= %d"
#define MAINAPP_1973_112_2_17_23_39_22_1648 "DrawFmName s_fm_rect.right= %d"
#define MAINAPP_1974_112_2_17_23_39_22_1649 "DrawFmName s_fm_rect.top= %d"
#define MAINAPP_1975_112_2_17_23_39_22_1650 "DrawFmName s_fm_rect.bottom= %d"
#define MAINAPP_2041_112_2_17_23_39_23_1651 "[MMIDC]: IdleWin_HandleMsg, MSG_OPEN_WINDOW"
#define MAINAPP_2089_112_2_17_23_39_23_1652 "IdleWin_HandleMsg MSG_FULL_PAINT"
#define MAINAPP_2134_112_2_17_23_39_23_1653 "IDLEWIN_HandleMsg: handle MSG_IDLE_UPDATE_DATETIME!"
#define MAINAPP_2144_112_2_17_23_39_23_1654 "IdleWin_HandleMsg: MSG_IDLE_UPDATE_NETWORK!"
#define MAINAPP_2167_112_2_17_23_39_23_1655 "IdleWin_HandleMsg: MSG_IDLE_UPDATE_MP3!"
#define MAINAPP_2454_112_2_17_23_39_23_1656 "IDLEWIN_HandleWinMsg: handle headset MSG!"
#define MAINAPP_2470_112_2_17_23_39_23_1657 "IdleWin_HandleMsg:received timer msg!"
#define MAINAPP_2477_112_2_17_23_39_23_1658 "IdleWin_HandleMsg: handle the anim wallpater timer!"
#define MAINAPP_2508_112_2_17_23_39_23_1659 "IdleWin_HandleMsg: MMK_GetFocusWinId() is =%d"
#define MAINAPP_2645_112_2_17_23_39_24_1660 "MMI: IdleWin_HandleMsg, received tp msg_id "
#define MAINAPP_2652_112_2_17_23_39_24_1661 "IdleWin_HandleMsg: point.x=%d"
#define MAINAPP_2653_112_2_17_23_39_24_1662 "IdleWin_HandleMsg: point.y=%d"
#define MAINAPP_2654_112_2_17_23_39_24_1663 "IdleWin_HandleMsg s_mp3_rect.left= %d"
#define MAINAPP_2655_112_2_17_23_39_24_1664 "IdleWin_HandleMsg s_mp3_rect.right= %d"
#define MAINAPP_2656_112_2_17_23_39_24_1665 "IdleWin_HandleMsg s_mp3_rect.top= %d"
#define MAINAPP_2657_112_2_17_23_39_24_1666 "IdleWin_HandleMsg s_mp3_rect.bottom= %d"
#define MAINAPP_2658_112_2_17_23_39_24_1667 "IdleWin_HandleMsg s_idlewin_info.is_display_mp3= %d"
#define MAINAPP_2683_112_2_17_23_39_24_1668 "IdleWin_HandleMsg open mp3 player!"
#define MAINAPP_2716_112_2_17_23_39_24_1669 "IDLEWIN_HandleWinMsg: MSG_STK_SET_UP_IDLE_MODE"
#define MAINAPP_2746_112_2_17_23_39_24_1670 "MMI: IdleWin_HandleMsg, don't handle message, msg_id = %d"
#define MAINAPP_2776_112_2_17_23_39_24_1671 "OpenDialingNumberBock, enter"
#define MAINAPP_2781_112_2_17_23_39_24_1672 "[mainapp.c]: dialing number block have been opened"
#define MAINAPP_3176_112_2_17_23_39_25_1673 "kick the key VT"
#define MAINAPP_3272_112_2_17_23_39_25_1674 "FuncDialingWin_HandleMsg MSG_KEYLONG_HASH!"
#define MAINAPP_3306_112_2_17_23_39_25_1675 "FuncDialingWin_HandleMsg:result_info_ptr->protect_type %d is error!"
#define MAINAPP_3327_112_2_17_23_39_25_1676 "[mainapp.c]: close dialing number block"
#define MAINAPP_3331_112_2_17_23_39_25_1677 "[mainapp.c]: dialing number block have been closed!"
#define MAINAPP_3339_112_2_17_23_39_25_1678 "[mainapp.c]: BLOCK disable failure!"
#define MAINAPP_3342_112_2_17_23_39_25_1679 " CloseDialingNumberBlock: close block successful!"
#define MAINAPP_3364_112_2_17_23_39_25_1680 "main_app_init:call_ring_type is %d!"
#define MAINAPP_3459_112_2_17_23_39_25_1681 "MAIN_StopIdleTimer: the sim error timer has stoped!"
#define MAINAPP_3646_112_2_17_23_39_26_1682 "OutputIdleWinWords idle_mode->len= %d"
#define MAINAPP_3668_112_2_17_23_39_26_1683 "OutputIdleWinWords s_words_rect.left= %d"
#define MAINAPP_3669_112_2_17_23_39_26_1684 "OutputIdleWinWords s_words_rect.right= %d"
#define MAINAPP_3670_112_2_17_23_39_26_1685 "OutputIdleWinWords s_words_rect.top= %d"
#define MAINAPP_3671_112_2_17_23_39_26_1686 "OutputIdleWinWords s_words_rect.bottom= %d"
#define MAINAPP_3727_112_2_17_23_39_26_1687 "OutputIdleWinNetworkName point.x= %d"
#define MAINAPP_3728_112_2_17_23_39_26_1688 "OutputIdleWinNetworkName point.y= %d"
#define MAINAPP_3753_112_2_17_23_39_26_1689 "OutputIdleWinNetworkName, is_get_cphs_info = %d"
#define MAINAPP_3768_112_2_17_23_39_26_1690 "OutputIdleWinNetworkName s_network_rect.left= %d"
#define MAINAPP_3769_112_2_17_23_39_26_1691 "OutputIdleWinNetworkName s_network_rect.right= %d"
#define MAINAPP_3770_112_2_17_23_39_26_1692 "OutputIdleWinNetworkName s_network_rect.top= %d"
#define MAINAPP_3771_112_2_17_23_39_26_1693 "OutputIdleWinNetworkName s_network_rect.bottom= %d"
#define MAINAPP_3794_112_2_17_23_39_26_1694 "OutputIdleWinNetworkName, get the network name from sim card!"
#define MAINAPP_3823_112_2_17_23_39_26_1695 "OutputIdleWinNetworkName s_network_rect.left= %d"
#define MAINAPP_3824_112_2_17_23_39_26_1696 "OutputIdleWinNetworkName s_network_rect.right= %d"
#define MAINAPP_3825_112_2_17_23_39_26_1697 "OutputIdleWinNetworkName s_network_rect.top= %d"
#define MAINAPP_3826_112_2_17_23_39_26_1698 "OutputIdleWinNetworkName s_network_rect.bottom= %d"
#define MAINAPP_3837_112_2_17_23_39_26_1699 "OutputIdleWinNetworkName, get the network name using the old method!"
#define MAINAPP_4229_112_2_17_23_39_27_1700 "DisplayIdleMp3Name g_idle_cur_mp3_name.length= %d"
#define MAINAPP_4230_112_2_17_23_39_27_1701 "DisplayIdleMp3Name g_idle_cur_mp3_name.is_ucs2= %d"
#define MAINAPP_4238_112_2_17_23_39_27_1702 "DisplayIdleMp3Name string_width= %d"
#define MAINAPP_4253_112_2_17_23_39_27_1703 "DisplayIdleMp3Name s_mp3_rect.left= %d"
#define MAINAPP_4254_112_2_17_23_39_27_1704 "DisplayIdleMp3Name s_mp3_rect.right= %d"
#define MAINAPP_4255_112_2_17_23_39_27_1705 "DisplayIdleMp3Name s_mp3_rect.top= %d"
#define MAINAPP_4256_112_2_17_23_39_27_1706 "DisplayIdleMp3Name s_mp3_rect.bottom= %d"
#define MAINAPP_4315_112_2_17_23_39_27_1707 "DisplayIdleCycleMp3Name s_mp3_rect.left= %d"
#define MAINAPP_4316_112_2_17_23_39_27_1708 "DisplayIdleCycleMp3Name s_mp3_rect.right= %d"
#define MAINAPP_4317_112_2_17_23_39_27_1709 "DisplayIdleCycleMp3Name s_mp3_rect.top= %d"
#define MAINAPP_4318_112_2_17_23_39_27_1710 "DisplayIdleCycleMp3Name s_mp3_rect.bottom= %d"
#define MAINAPP_4609_112_2_17_23_39_28_1711 "DisplayStatusBarIcon start MMI_WIN_ICON_GPRS is_gprs is %d"
#define MAINAPP_4612_112_2_17_23_39_28_1712 "DisplayStatusBarIcon MMI_WIN_ICON_GPRS sim is OK!"
#define MAINAPP_4626_112_2_17_23_39_28_1713 "DisplayStatusBarIcon image_id = IMAGE_IDLE_TITLE_ICON_GPRS_DEACTIVE"
#define MAINAPP_4793_112_2_17_23_39_28_1714 "DisplayStatusBarIcon is charge: image_id=%d"
#define MAINAPP_4800_112_2_17_23_39_28_1715 "DisplayStatusBarIcon not charge: image_id=%d"
#define MAINAPP_4964_112_2_17_23_39_28_1716 "MAIN_SetIdleSmsState: the line_state is same!"
#define MAINAPP_4981_112_2_17_23_39_28_1717 "MAIN_SetIdleSmsState: the voice message is same!"
#define MAINAPP_5090_112_2_17_23_39_29_1718 "MMIMAIN_HandleIdelTimer: handle the idle icon timer!"
#define MAINAPP_5163_112_2_17_23_39_29_1719 "MMI_GetFreeSpace: free_space = %d"
#define MAINAPP_5325_112_2_17_23_39_29_1720 "Handle_HW_Icon_Exec_Func : s_cur_hw_icon_index = %d"
#define MMIGAME_WINTAB_135_112_2_17_23_39_30_1721 "mmigame_wintab.c:enter main menu handle function"
#define PUA_UNIT_TEST_232_112_2_17_23_40_33_1722 "[ PUAUT ] Receive an signal, but do not know who sent it"
#define PUA_UNIT_TEST_307_112_2_17_23_40_33_1723 "\nPUAUT: ProcAppUTTestSignal()"
#define PUA_UNIT_TEST_308_112_2_17_23_40_33_1724 "\nPUAUT: Command is %s, %s, %s"
#define PUA_UNIT_TEST_351_112_2_17_23_40_34_1725 "[ PUAUT ] REG parameters error"
#define PUA_UNIT_TEST_528_112_2_17_23_40_34_1726 "PUAUT: Register failed, because of no resource.\n"
#define PUA_UNIT_TEST_543_112_2_17_23_40_34_1727 "PUAUT: Register success"
#define PUA_UNIT_TEST_545_112_2_17_23_40_34_1728 "PUAUT: Register failed, because of no resource.\n"
#define PUA_UNIT_TEST_584_112_2_17_23_40_34_1729 "PUAUT[ PUB ]: Create presence source failed(%d).\n"
#define PUA_UNIT_TEST_610_112_2_17_23_40_34_1730 "PUAUT[ PUB ]: Unpublish failed"
#define PUA_UNIT_TEST_665_112_2_17_23_40_34_1731 "PUAUT[ WATCHER ]: Create watcher success"
#define PUA_UNIT_TEST_672_112_2_17_23_40_34_1732 "PUAUT[ WATCHER ]: Create watcher failed(%d).\n"
#define PUA_UNIT_TEST_703_112_2_17_23_40_34_1733 "PUAUT[ WATCHER ]: Unsubscribe success"
#define PUA_UNIT_TEST_705_112_2_17_23_40_34_1734 "PUAUT[ WATCHER ]: Unsubscribe failed"
#define PUA_UNIT_TEST_727_112_2_17_23_40_34_1735 "PUAUT[ WATCHER ]: close success"
#define PUA_UNIT_TEST_729_112_2_17_23_40_34_1736 "PUAUT[ WATCHER ]: close failed"
#define PUA_UNIT_TEST_737_112_2_17_23_40_34_1737 "PUAUT[ WATCHER ]: redirected failed"
#define PUA_UNIT_TEST_741_112_2_17_23_40_34_1738 "PUAUT[ WATCHER ]: NOTIFY IND"
#define PUA_UNIT_TEST_771_112_2_17_23_40_34_1739 "PUAUT[ WINFO ]: Create watcher success"
#define PUA_UNIT_TEST_778_112_2_17_23_40_34_1740 "PUAUT[ WINFO ]: Create watcher failed(%d).\n"
#define PUA_UNIT_TEST_809_112_2_17_23_40_34_1741 "PUAUT[ WINFO ]: Unsubscribe success"
#define PUA_UNIT_TEST_811_112_2_17_23_40_34_1742 "PUAUT[ WINFO ]: Unsubscribe failed"
#define PUA_UNIT_TEST_833_112_2_17_23_40_35_1743 "PUAUT[ WINFO ]: close success"
#define PUA_UNIT_TEST_835_112_2_17_23_40_35_1744 "PUAUT[ WINFO ]: close failed"
#define PUA_UNIT_TEST_843_112_2_17_23_40_35_1745 "PUAUT[ WINFO ]: redirected failed"
#define PUA_UNIT_TEST_847_112_2_17_23_40_35_1746 "PUAUT[ WINFO ]: NOTIFY IND"
#define PUA_UNIT_TEST_867_112_2_17_23_40_35_1747 "PUAUT: Registering\n"
#define PUA_UNIT_TEST_887_112_2_17_23_40_35_1748 "PUAUT[ PUB ]: PUA_PRES_SRC_PUB_CNF"
#define PUA_UNIT_TEST_888_112_2_17_23_40_35_1749 "PUAUT[ PUB ]: error code(%d)"
#define PUA_UNIT_TEST_889_112_2_17_23_40_35_1750 "PUAUT[ PUB ]: instance id(%08x)"
#define PUA_UNIT_TEST_895_112_2_17_23_40_35_1751 "PUAUT[ SIGNAL ]: unknown message"
#define PUA_UNIT_TEST_1286_112_2_17_23_40_35_1752 "Parse result \"%s %s\""
#define PUA_UT_DRIVE_465_112_2_17_23_40_38_1753 "PUAUT: TestOneCase:\n"
#define PUA_UT_DRIVE_499_112_2_17_23_40_38_1754 "[PUA UT DRIVER] Send signal: (%s) (%s) (%s)"
#define PUA_UT_DRIVE_504_112_2_17_23_40_38_1755 "[PUA UT DRIVER] Send signal: (%d)"
#define PUA_UT_DRIVE_523_112_2_17_23_40_38_1756 "[PUA UT DRIVER] Receive signal PUA_UT_STOP_TEST_IND"
#define PUA_UT_DRIVE_527_112_2_17_23_40_38_1757 "[PUA UT DRIVER] Receive signal PUA_UT_SEND_CMD_CNF"
#define PUA_UT_DRIVE_537_112_2_17_23_40_38_1758 "PUAUT: [FATAL ERROR] System requirement: VC WIN32\n"
#define POC_ADAP_AUDIO_86_112_2_17_23_40_39_1759 "POCPS:POCADAP_AudioCreatePlayerHandle audio_handle=0x%x"
#define POC_ADAP_AUDIO_145_112_2_17_23_40_39_1760 "POCPS:POCADAP_AudioStartPlay audio_handle=0x%x, dev_mode=%d, status=%d"
#define POC_ADAP_AUDIO_187_112_2_17_23_40_39_1761 "POCPS:POCADAP_AudioSetPlayerParams audio_handle=0x%x, dev_mode=%d, vol=%d"
#define POC_ADAP_AUDIO_231_112_2_17_23_40_39_1762 "POCPS:POCADAP_AudioSetDataAndPlay audio_handle=0x%x, buf_ptr=0x%x, buf_len=%d, status=%d"
#define POC_ADAP_AUDIO_256_112_2_17_23_40_39_1763 "POCPS:POCADAP_AudioStopPlay audio_handle=0x%x"
#define POC_ADAP_AUDIO_286_112_2_17_23_40_39_1764 "POCPS:POCADAP_AudioClosePlayerHandle audio_handle=0x%x"
#define POC_ADAP_AUDIO_303_112_2_17_23_40_39_1765 "POCPS:POCADAP_AudioCreateRecorderHandle audio_handle=0x%x"
#define POC_ADAP_AUDIO_341_112_2_17_23_40_39_1766 "POCPS:POCADAP_AudioStartRecord audio_handle=0x%x, status=%d"
#define POC_ADAP_AUDIO_382_112_2_17_23_40_39_1767 "POCPS:POCADAP_AudioGetRecordData audio_handle=0x%x, status=%d, data_length=%d"
#define POC_ADAP_AUDIO_417_112_2_17_23_40_39_1768 "POCPS:POCADAP_AudioStopRecord audio_handle=0x%x, status=%d"
#define POC_ADAP_AUDIO_440_112_2_17_23_40_39_1769 "POCPS:POCADAP_AudioCloseRecorderHandle audio_handle=0x%x"
#define POC_ADAP_AUDIO_458_112_2_17_23_40_39_1770 "POCPS:PlayerCallBack play_result=%d"
#define POC_ADAP_AUDIO_479_112_2_17_23_40_39_1771 "POCPS:RecorderCallBack data_length=%d"
#define POC_ADAP_OS_123_112_2_17_23_40_40_1772 "POCPS: POCADAP_OSSetupQueue start\n"
#define POC_ADAP_OS_147_112_2_17_23_40_40_1773 "POCPS:Send to app   %.32s    id=0x%x\n"
#define POC_ADAP_OS_157_112_2_17_23_40_40_1774 "POCPS:POCADAP_OSSendSignalToApp failed,errno:%d\n"
#define POC_ADAP_OS_170_112_2_17_23_40_40_1775 "POCPS:POCADAP_OSSendSignalToApp failed\n"
#define POC_ADAP_OS_195_112_2_17_23_40_40_1776 "POCPS:POCADAP_OSSendSignalToPoC failed,errno:%d\n"
#define POC_ADAP_OS_208_112_2_17_23_40_40_1777 "POCPS:POCADAP_OSSendSignalToPoC failed\n"
#define POC_DEJ_222_112_2_17_23_40_58_1778 "POCPS: POCDEJ_HasDataToPlay no data"
#define POC_MAIN_105_112_2_17_23_40_59_1779 "POCPS: POCMAIN_Start s_poc_task_id=0x%x"
#define POC_MAIN_282_112_2_17_23_41_0_1780 "POCPS:Get           %.32s    id=0x%x\n"
#define AMR_UNPACK_353_112_2_17_23_41_11_1781 "RTPPL:RTPPL_AmrUnpackConstruct,input error!"
#define AMR_UNPACK_370_112_2_17_23_41_11_1782 "RTPPL:RTPPL_AmrUnpackConstruct,amr clock rate error!"
#define AMR_UNPACK_378_112_2_17_23_41_11_1783 "RTPPL_AmrUnpackConstruct timestamp =%x"
#define AMR_UNPACK_390_112_2_17_23_41_11_1784 "RTPPL:RTPPL_AmrUnpackConstruct,create list fail for no memory!"
#define AMR_UNPACK_397_112_2_17_23_41_11_1785 "RTPPL:RTPPL_AmrUnpackConstruct,init buffer error!"
#define AMR_UNPACK_449_112_2_17_23_41_11_1786 "RTPPL:RTPPL_AmrUnpackConstruct,unpack toc error!"
#define AMR_UNPACK_459_112_2_17_23_41_11_1787 "RTPPL:RTPPL_AmrUnpackConstruct,skip crc error!"
#define AMR_UNPACK_468_112_2_17_23_41_11_1788 "RTPPL:RTPPL_AmrUnpackConstruct,get toc count error!"
#define AMR_UNPACK_476_112_2_17_23_41_11_1789 "RTPPL:RTPPL_AmrUnpackConstruct,update frame block error!"
#define AMR_UNPACK_497_112_2_17_23_41_11_1790 "RTPPL:RTPPL_AmrUnpackConstruct,sorted/linear copy error!"
#define AMR_UNPACK_506_112_2_17_23_41_11_1791 "RTPPL:RTPPL_AmrUnpackConstruct,alloc result buffer error!"
#define AMR_UNPACK_515_112_2_17_23_41_11_1792 "RTPPL:RTPPL_AmrUnpackConstruct,convert block to buffer error!"
#define AMR_UNPACK_567_112_2_17_23_41_12_1793 "RTPPL:RTPPL_AmrUnpackConstruct,occur error!"
#define AMR_UNPACK_613_112_2_17_23_41_12_1794 "RTPPL:RTPPL_AmrGetFrameParam,input error!"
#define AMR_UNPACK_624_112_2_17_23_41_12_1795 "RTPPL:RTPPL_AmrGetFrameParam,frame count error!"
#define AMR_UNPACK_656_112_2_17_23_41_12_1796 "RTPPL:RTPPL_AmrUnpackDestruct,input error!"
#define AMR_UNPACK_738_112_2_17_23_41_12_1797 "RTPPL:ConvertBlockBufToList,input error!"
#define AMR_UNPACK_746_112_2_17_23_41_12_1798 "RTPPL:ConvertBlockBufToList,create list error!"
#define AMR_UNPACK_754_112_2_17_23_41_12_1799 "ConvertBlockBufToList timestamp =%x"
#define AMR_UNPACK_769_112_2_17_23_41_12_1800 "ConvertBlockBufToList frame timestamp 1 =%x"
#define AMR_UNPACK_774_112_2_17_23_41_12_1801 "ConvertBlockBufToList frame timestamp 2 =%x"
#define AMR_UNPACK_789_112_2_17_23_41_12_1802 "RTPPL:ConvertBlockBufToList,alloc frame data buffer error!"
#define AMR_UNPACK_805_112_2_17_23_41_12_1803 "RTPPL:ConvertBlockBufToList,insert new node error!"
#define AMR_UNPACK_854_112_2_17_23_41_12_1804 "RTPPL:UpdateFrameBlockCount,input error!"
#define AMR_UNPACK_879_112_2_17_23_41_12_1805 "RTPPL:BlockBufferSetBlockCount,input error!"
#define AMR_UNPACK_898_112_2_17_23_41_12_1806 "RTPPL:BlockBufferSetBlockCount,alloc block buffer error!"
#define AMR_UNPACK_914_112_2_17_23_41_12_1807 "RTPPL:BlockBufferSetBlockCount,alloc block size error!"
#define AMR_UNPACK_1099_112_2_17_23_41_13_1808 "RTPPL:GetTocTypeByIndex,input error!"
#define AMR_UNPACK_1142_112_2_17_23_41_13_1809 "RTPPL:GetTocTypeByIndex,input error!"
#define H263_UNPACK_161_112_2_17_23_41_14_1810 "RTPPL:RTPPL_H263Unpack,input error!"
#define H263_UNPACK_193_112_2_17_23_41_14_1811 "RTPPL:RTPPL_H263Unpack,payload header is Mode A!"
#define H263_UNPACK_241_112_2_17_23_41_15_1812 "RTPPL:RTPPL_H263Unpack,payload header is Mode B!"
#define H263_UNPACK_252_112_2_17_23_41_15_1813 "RTPPL:RTPPL_H263Unpack,payload header is Mode C!"
#define H263_UNPACK_282_112_2_17_23_41_15_1814 "RTPPL:RTPPL_H263PlusUnpackConstruct,input error!"
#define H263_UNPACK_330_112_2_17_23_41_15_1815 "RTPPL:RTPPL_H263PlusUnpackConstruct,exist VRC header!"
#define H263_UNPACK_369_112_2_17_23_41_15_1816 "RTPPL:RTPPL_H263PlusUnpackConstruct,exist extra header!"
#define H263_UNPACK_426_112_2_17_23_41_15_1817 "RTPPL:RTPPL_H263PlusUnpackConstruct,occur error = %d"
#define H263_UNPACK_443_112_2_17_23_41_15_1818 "RTPPL:RTPPL_H263PlusGetResult,input error!"
#define H263_UNPACK_466_112_2_17_23_41_15_1819 "RTPPL:RTPPL_H263PlusUnpackDestruct,input error!"
#define H264_UNPACK_179_112_2_17_23_41_16_1820 "RTPPL_H264UnpackConstruct timestamp =%x"
#define H264_UNPACK_507_112_2_17_23_41_16_1821 "RTPPL: RTPPL_H264UnpackConstruct,h264 payload struct error!"
#define MPEG4_UNPACK_120_112_2_17_23_41_17_1822 "RTPPL RTPPL_Mpeg4UnpackConstruct input error"
#define MPEG4_UNPACK_138_112_2_17_23_41_17_1823 "RTPPL RTPPL_Mpeg4UnpackConstruct have au header"
#define MPEG4_UNPACK_145_112_2_17_23_41_17_1824 "RTPPL RTPPL_Mpeg4UnpackConstruct alloc error"
#define MPEG4_UNPACK_155_112_2_17_23_41_17_1825 "RTPPL RTPPL_Mpeg4UnpackConstruct create list error"
#define MPEG4_UNPACK_243_112_2_17_23_41_17_1826 "RTPPL RTPPL_Mpeg4UnpackConstruct cts delta value is 0"
#define MPEG4_UNPACK_265_112_2_17_23_41_17_1827 "RTPPL RTPPL_Mpeg4UnpackConstruct dts delta value is 0"
#define MPEG4_UNPACK_463_112_2_17_23_41_17_1828 "RTPPL RTPPL_Mpeg4GetAuNum input error"
#define MPEG4_UNPACK_492_112_2_17_23_41_18_1829 "RTPPL RTPPL_Mpeg4GetAuParam input error"
#define MPEG4_UNPACK_503_112_2_17_23_41_18_1830 "RTPPL RTPPL_Mpeg4GetAuParam 1 input error"
#define MPEG4_UNPACK_534_112_2_17_23_41_18_1831 "RTPPL RTPPL_Mpeg4UnpackDestruct input error"
#define MPEG4_UNPACK_590_112_2_17_23_41_18_1832 "RTPPL Mpeg4ShowBits input error"
#define MPEG4_UNPACK_641_112_2_17_23_41_18_1833 "RTPPL InsertNewAuNode input error"
#define MPEG4_UNPACK_654_112_2_17_23_41_18_1834 "RTPPL InsertNewAuNode insert error"
#define MPEG4_UNPACK_672_112_2_17_23_41_18_1835 "RTPPL DestroyAuList input error"
#define RTSP_ADAPTER_83_112_2_17_23_41_18_1836 "RTSP RTSPADP_AdapterMethodToSignal method =%d,status_code =%d"
#define RTSP_ADAPTER_132_112_2_17_23_41_18_1837 "\n rtsp AdapterMethodToSignal param error\n"
#define RTSP_ADAPTER_270_112_2_17_23_41_18_1838 "rtsp AdapterPlayMethodToSignal RTSP_HSM_PLAY_AUTH_IND "
#define RTSP_ADAPTER_398_112_2_17_23_41_19_1839 "rtsp AdapterSetParameterMethodToSignal RTSP_HSM_SETPARAM_AUTH_IND "
#define RTSP_API_51_112_2_17_23_41_19_1840 "\n rtsp RTSP_InitSession task id = %d"
#define RTSP_API_68_112_2_17_23_41_19_1841 "\n rtsp RTSP_InitSession error_code = %d"
#define RTSP_API_91_112_2_17_23_41_19_1842 "RTSP RTSP_OptionsReq context_id =%d"
#define RTSP_API_98_112_2_17_23_41_19_1843 "\n rtsp RTSP_OptionsReq task id = %d"
#define RTSP_API_115_112_2_17_23_41_19_1844 "\n rtsp RTSP_OptionsReq error_code = %d"
#define RTSP_API_142_112_2_17_23_41_19_1845 "RTSP RTSP_OptionsAuthRsp context_id =%d"
#define RTSP_API_149_112_2_17_23_41_19_1846 "\n rtsp RTSP_OptionsAuthRsp task id = %d"
#define RTSP_API_166_112_2_17_23_41_19_1847 "\n rtsp RTSP_OptionsAuthRsp error_code = %d"
#define RTSP_API_189_112_2_17_23_41_19_1848 "RTSP RTSP_DescribeReq context_id =%d"
#define RTSP_API_196_112_2_17_23_41_19_1849 "\n rtsp RTSP_DescribeReq task id = %d"
#define RTSP_API_213_112_2_17_23_41_19_1850 "\n rtsp RTSP_DescribeReq error_code = %d"
#define RTSP_API_240_112_2_17_23_41_19_1851 "RTSP RTSP_DescribeAuthRsp context_id =%d"
#define RTSP_API_247_112_2_17_23_41_19_1852 "\n rtsp RTSP_DescribeAuthRsp task id = %d"
#define RTSP_API_264_112_2_17_23_41_19_1853 "\n rtsp RTSP_DescribeAuthRsp error_code = %d"
#define RTSP_API_287_112_2_17_23_41_19_1854 "RTSP RTSP_PlayReq context_id =%d"
#define RTSP_API_294_112_2_17_23_41_19_1855 "\n rtsp RTSP_PlayReq task id = %d"
#define RTSP_API_311_112_2_17_23_41_20_1856 "\n rtsp RTSP_PlayReq error_code = %d"
#define RTSP_API_334_112_2_17_23_41_20_1857 "RTSP RTSP_PauseReq context_id =%d"
#define RTSP_API_341_112_2_17_23_41_20_1858 "\n rtsp RTSP_PauseReq task id = %d"
#define RTSP_API_358_112_2_17_23_41_20_1859 "\n rtsp RTSP_PauseReq error_code = %d"
#define RTSP_API_381_112_2_17_23_41_20_1860 "RTSP RTSP_SetupReq context_id =%d"
#define RTSP_API_388_112_2_17_23_41_20_1861 "\n rtsp RTSP_SetupReq task id = %d"
#define RTSP_API_405_112_2_17_23_41_20_1862 "\n rtsp RTSP_SetupReq error_code = %d"
#define RTSP_API_432_112_2_17_23_41_20_1863 "RTSP RTSP_SetupAuthRsp context_id =%d"
#define RTSP_API_439_112_2_17_23_41_20_1864 "\n rtsp RTSP_SetupAuthRsp task id = %d"
#define RTSP_API_456_112_2_17_23_41_20_1865 "\n rtsp RTSP_SetupAuthRsp error_code = %d"
#define RTSP_API_483_112_2_17_23_41_20_1866 "RTSP RTSP_PlayAuthRsp context_id =%d"
#define RTSP_API_490_112_2_17_23_41_20_1867 "\n rtsp RTSP_PlayAuthRsp task id = %d"
#define RTSP_API_507_112_2_17_23_41_20_1868 "\n rtsp RTSP_PlayAuthRsp error_code = %d"
#define RTSP_API_530_112_2_17_23_41_20_1869 "RTSP RTSP_TeardownReq context_id =%d"
#define RTSP_API_537_112_2_17_23_41_20_1870 "\n rtsp RTSP_TeardownReq task id = %d"
#define RTSP_API_554_112_2_17_23_41_20_1871 "\n rtsp RTSP_TeardownReq error_code = %d"
#define RTSP_API_577_112_2_17_23_41_20_1872 "RTSP RTSP_AnnounceReq context_id =%d"
#define RTSP_API_584_112_2_17_23_41_20_1873 "\n rtsp RTSP_AnnounceReq task id = %d"
#define RTSP_API_601_112_2_17_23_41_20_1874 "\n rtsp RTSP_AnnounceReq error_code = %d"
#define RTSP_API_624_112_2_17_23_41_20_1875 "RTSP RTSP_RecordReq context_id =%d"
#define RTSP_API_631_112_2_17_23_41_20_1876 "\n rtsp RTSP_RecordReq task id = %d"
#define RTSP_API_648_112_2_17_23_41_20_1877 "\n rtsp RTSP_RecordReq error_code = %d"
#define RTSP_API_671_112_2_17_23_41_20_1878 "RTSP RTSP_CloseSession context_id =%d"
#define RTSP_API_678_112_2_17_23_41_20_1879 "\n rtsp RTSP_CloseSession task id = %d"
#define RTSP_API_695_112_2_17_23_41_20_1880 "\n rtsp RTSP_CloseSession error_code = %d"
#define RTSP_API_718_112_2_17_23_41_20_1881 "RTSP RTSP_SetParamReq context_id =%d"
#define RTSP_API_725_112_2_17_23_41_20_1882 "\n rtsp RTSP_SetParamReq task id = %d"
#define RTSP_API_742_112_2_17_23_41_20_1883 "\n rtsp RTSP_SetParamReq error_code = %d"
#define RTSP_API_769_112_2_17_23_41_20_1884 "RTSP RTSP_SetParamAuthRsp context_id =%d"
#define RTSP_API_776_112_2_17_23_41_20_1885 "\n rtsp RTSP_SetParamAuthRsp task id = %d"
#define RTSP_API_793_112_2_17_23_41_21_1886 "\n rtsp RTSP_SetParamAuthRsp error_code = %d"
#define RTSP_AUTH_119_112_2_17_23_41_21_1887 "RTSP RTSPAUTH_GetInfo input error"
#define RTSP_AUTH_133_112_2_17_23_41_21_1888 "RTSP RTSPAUTH_GetInfo 1 proxy_ptr = PNULL"
#define RTSP_AUTH_143_112_2_17_23_41_21_1889 "RTSP RTSPAUTH_GetInfo 1 auth_ptr = PNULL"
#define RTSP_AUTH_157_112_2_17_23_41_21_1890 "RTSP RTSPAUTH_GetInfo 2 proxy_ptr = PNULL"
#define RTSP_AUTH_167_112_2_17_23_41_21_1891 "RTSP RTSPAUTH_GetInfo 2 auth_ptr = PNULL"
#define RTSP_AUTH_202_112_2_17_23_41_21_1892 "RTSP RTSPAUTH_DigestResponseCreate alloc fail"
#define RTSP_AUTH_219_112_2_17_23_41_21_1893 "RTSP RTSPAUTH_DigestResponseDestroy input error"
#define RTSP_AUTH_290_112_2_17_23_41_21_1894 "RTSP RTSPAUTH_DigestRequestCreate alloc fail"
#define RTSP_AUTH_307_112_2_17_23_41_21_1895 "RTSP RTSPAUTH_FreeDigestRequest input error"
#define RTSP_AUTH_416_112_2_17_23_41_21_1896 "RTSP RTSPAUTH_DigestResponseParse response_ptr =%d,value_ptr =%d"
#define RTSP_AUTH_503_112_2_17_23_41_22_1897 "RTSP RtspAuthenBasicEncode user_name_ptr =%d,password_ptr =%d"
#define RTSP_AUTH_544_112_2_17_23_41_22_1898 "RTSP RtspAuthenDigestEncode input error"
#define RTSP_AUTH_595_112_2_17_23_41_22_1899 "RTSP RtspAuthenDigestCalcHA1 input error"
#define RTSP_AUTH_632_112_2_17_23_41_22_1900 "RTSP RtspAuthenDigestCalcResult input error"
#define RTSP_AUTH_682_112_2_17_23_41_22_1901 "RTSP RtspAuthenDigestRequestSetParam request_ptr =%d,name_ptr =%d,password_ptr =%d"
#define RTSP_AUTH_929_112_2_17_23_41_22_1902 "\n RTSP RtspAuthenDigestResponseSaveParam invalid input \n"
#define RTSP_AUTH_1130_112_2_17_23_41_23_1903 "\n RTSP RTSPAuthenDigestResponseSaveParam unknown param \n"
#define RTSP_AUTH_1141_112_2_17_23_41_23_1904 "\n RTSP RTSPAuthenDigestResponseSaveParam error_code=%d"
#define RTSP_AUTH_1153_112_2_17_23_41_23_1905 "\n RTSP RTSPAuthenDigestResponseSaveParam destroy request_digest_ptr"
#define RTSP_AUTH_1165_112_2_17_23_41_23_1906 "\n RTSP RTSPAuthenDigestResponseSaveParam CLEAR error_code=%d"
#define RTSP_AUTH_1181_112_2_17_23_41_23_1907 "\n rtsp RtspAuthenDigestTokenFind input error \n"
#define RTSP_AUTH_1194_112_2_17_23_41_23_1908 "\n rtsp RtspAuthenDigestTokenFind auth map type =%d"
#define RTSP_AUTH_1198_112_2_17_23_41_23_1909 "\n rtsp RtspAuthenDigestTokenFind error \n"
#define RTSP_AUTH_1219_112_2_17_23_41_23_1910 "\n rtsp RtspAuthenCombineResult input error \n"
#define RTSP_AUTH_1230_112_2_17_23_41_23_1911 "\n rtsp RtspAuthenCombineResult alloc error \n"
#define RTSP_AUTH_1245_112_2_17_23_41_23_1912 "\n rtsp RtspAuthenCombineResult 1 input error \n"
#define RTSP_AUTH_1251_112_2_17_23_41_23_1913 "\n rtsp RtspAuthenCombineResult 1 alloc error \n"
#define RTSP_COOKIE_52_112_2_17_23_41_23_1914 "\n rtsp RTSPCOOKIE_NodeCreate input error\n"
#define RTSP_COOKIE_59_112_2_17_23_41_23_1915 "\n rtsp RTSPCOOKIE_NodeCreate alloc error\n"
#define RTSP_COOKIE_95_112_2_17_23_41_24_1916 "\n RTSP RTSPCOOKIE_Parse 1 alloc error\n"
#define RTSP_COOKIE_106_112_2_17_23_41_24_1917 "\n rtsp RTSPCOOKIE_Parse 2 alloc error\n"
#define RTSP_COOKIE_122_112_2_17_23_41_24_1918 "\n RTSP RTSPCookieParse 3 alloc error\n"
#define RTSP_COOKIE_336_112_2_17_23_41_24_1919 "\n rtsp RTSPCOOKIE_Put input error\n"
#define RTSP_COOKIE_400_112_2_17_23_41_24_1920 "\n RTSP RTSPCookiePut alloc 1 error\n"
#define RTSP_COOKIE_425_112_2_17_23_41_24_1921 "\n RTSP RTSPCookiePut 2 alloc error\n"
#define RTSP_COOKIE_435_112_2_17_23_41_24_1922 "\n RTSP RTSPCookiePut 3 alloc error\n"
#define RTSP_COOKIE_458_112_2_17_23_41_24_1923 "\n RTSP RTSPCookiePut 4 alloc error\n"
#define RTSP_COOKIE_482_112_2_17_23_41_24_1924 "RTSP RTSPCOOKIE_DestroyNode cookie_node_ptr = %d"
#define RTSP_COOKIE_542_112_2_17_23_41_24_1925 "\n http RTSPGetCookie input error or no cookie\n"
#define RTSP_COOKIE_593_112_2_17_23_41_25_1926 "\n RTSP RTSPGetCookie mark_parent_ptr null\n"
#define RTSP_COOKIE_600_112_2_17_23_41_25_1927 "\n RTSP RTSPGetCookie alloc error\n"
#define RTSP_COOKIE_633_112_2_17_23_41_25_1928 "\n RTSP RTSPGetCookie temp_str_ptr null\n"
#define RTSP_COOKIE_664_112_2_17_23_41_25_1929 "\n http RTSPCookieFormat input error\n"
#define RTSP_COOKIE_672_112_2_17_23_41_25_1930 "\n RTSP RTSPCookieFormat alloc error\n"
#define RTSP_COOKIE_694_112_2_17_23_41_25_1931 "\n RTSP RTSPCookieFormat input 1 error\n"
#define RTSP_COOKIE_713_112_2_17_23_41_25_1932 "\n RTSP RTSPCookieFormat 1 alloc error\n"
#define RTSP_COOKIE_726_112_2_17_23_41_25_1933 "\n RTSP RTSPCookieFormat 1 alloc error\n"
#define RTSP_EVENT_32_112_2_17_23_41_25_1934 "RTSP RTSPEVENT_QueueInit input error"
#define RTSP_EVENT_50_112_2_17_23_41_25_1935 "RTSP RTSPEVENT_QueueInsert input error"
#define RTSP_EVENT_53_112_2_17_23_41_25_1936 "rtsp RTSPEVENT_QueueInsert event =%d"
#define RTSP_EVENT_58_112_2_17_23_41_25_1937 "RTSP RTSPEVENT_QueueInsert alloc error"
#define RTSP_EVENT_93_112_2_17_23_41_25_1938 "RTSP RTSPEVENT_QueueFetch input error"
#define RTSP_HEADER_122_112_2_17_23_41_26_1939 "RTSP RTSPHDR_InsertHeader input error"
#define RTSP_HEADER_180_112_2_17_23_41_26_1940 "RTSP RTSPHDR_FindHeader input error"
#define RTSP_HEADER_368_112_2_17_23_41_26_1941 "\n rtsp RTSPHDR_SetHeaderField input error,request =%d,token =%d,value =%d\n"
#define RTSP_HEADER_371_112_2_17_23_41_26_1942 "rtsp RTSPHDR_SetHeaderField token =%s"
#define RTSP_HEADER_375_112_2_17_23_41_26_1943 "rtsp RTSPHDR_SetHeaderField value =%s"
#define RTSP_HEADER_383_112_2_17_23_41_26_1944 "\n RTSP RTSPHDR_SetHeaderField alloc error\n"
#define RTSP_HEADER_466_112_2_17_23_41_27_1945 "\n rtsp RTSPHDR_GetHeaderValue param error\n"
#define RTSP_HOST_147_112_2_17_23_41_27_1946 "RTSP RTSPHOST_FreeHostList host_list_handle =%d"
#define RTSP_HSM_558_112_2_17_23_41_28_1947 "RTSP RTSPHSM_DispatchSignal dispatch signal code =%d"
#define RTSP_HSM_561_112_2_17_23_41_28_1948 "RTSP RTSPHSM_DispatchSignal hsm_state =%d"
#define RTSP_HSM_566_112_2_17_23_41_28_1949 "RTSP RTSPHSM_DispatchSignal event =%d"
#define RTSP_HSM_597_112_2_17_23_41_28_1950 "RTSP RTSPHSM_DealRequest input error"
#define RTSP_HSM_753_112_2_17_23_41_29_1951 "RTSP RTSP_TransCallback input error"
#define RTSP_HSM_762_112_2_17_23_41_29_1952 "RTSP RTSP_TransCallback host_ptr =%d,context_ptr =%d"
#define RTSP_HSM_771_112_2_17_23_41_29_1953 "RTSP RTSP_TransCallback sig_ptr alloc error"
#define RTSP_HSM_866_112_2_17_23_41_29_1954 "\n rtsp TransCallback recv NETMGR_SEND_RETURN_IND error\n"
#define RTSP_HSM_894_112_2_17_23_41_29_1955 "\n rtsp TransCallback create response error\n"
#define RTSP_HSM_979_112_2_17_23_41_29_1956 "\n rtsp TransCallback recv NETMGR_RECV_RETURN_IND error\n"
#define RTSP_HSM_1003_112_2_17_23_41_29_1957 "\n RTSP TransCallback default error\n"
#define RTSP_HSM_1050_112_2_17_23_41_29_1958 "\n rtsp init\n"
#define RTSP_HSM_1065_112_2_17_23_41_29_1959 "RTSP RTSP_CreateContext alloc error"
#define RTSP_HSM_1082_112_2_17_23_41_29_1960 "RTSP RTSP_CreateContext host_list_handle =%d"
#define RTSP_HSM_1086_112_2_17_23_41_29_1961 "RTSP RTSP_CreateContext method_chart_list =%d"
#define RTSP_HSM_1208_112_2_17_23_41_30_1962 "RTSP RTSP_BuildAndSendReq input error"
#define RTSP_HSM_1218_112_2_17_23_41_30_1963 "RTSP RTSP_BuildAndSendReq alloc error"
#define RTSP_HSM_1342_112_2_17_23_41_30_1964 "RTSP RtspHandleRecvData host_ptr input error"
#define RTSP_HSM_1349_112_2_17_23_41_30_1965 "RTSP RtspHandleRecvData context_ptr =%d,response_ptr =%d"
#define RTSP_HSM_1461_112_2_17_23_41_30_1966 "\n rtsp RtspConstructor param error\n"
#define RTSP_HSM_1478_112_2_17_23_41_30_1967 "\n rtsp RtspDestructor param error\n"
#define RTSP_HSM_1494_112_2_17_23_41_30_1968 "\n rtsp RtspInitial param error\n"
#define RTSP_HSM_1514_112_2_17_23_41_30_1969 "RTSP QHSM_Rtsp context id =%d"
#define RTSP_HSM_1516_112_2_17_23_41_30_1970 "RTSP QHSM_Rtsp context_ptr =%d"
#define RTSP_HSM_1525_112_2_17_23_41_30_1971 "RTSP QHSM_Rtsp recv RTSP_HSM_TIMEOUT_IND"
#define RTSP_HSM_1530_112_2_17_23_41_30_1972 "RTSP QHSM_Rtsp RTSP_HSM_TIMEOUT_IND host_ptr =%d,context_ptr =%d"
#define RTSP_HSM_1559_112_2_17_23_41_30_1973 "RTSP QHSM_Rtsp recv RTSP_HSM_CLOSE_REQ"
#define RTSP_HSM_1563_112_2_17_23_41_30_1974 "RTSP QHSM_Rtsp RTSP_HSM_CLOSE_REQ host_ptr =%d,context_ptr =%d"
#define RTSP_HSM_1575_112_2_17_23_41_30_1975 "RTSP QHSM_Rtsp recv RTSP_HSM_PEROID_TIMER_IND"
#define RTSP_HSM_1582_112_2_17_23_41_30_1976 "RTSP QHSM_Rtsp recv RTSP_HSM_SERVER_CLOSE_IND or RTSP_HSM_EXCEPTION_CLOSE_IND"
#define RTSP_HSM_1628_112_2_17_23_41_30_1977 "RTSP QHSM_Rtsp recv other method"
#define RTSP_HSM_1632_112_2_17_23_41_30_1978 "RTSP QHSM_Rtsp other method host_ptr =%d,context_ptr =%d"
#define RTSP_HSM_1662_112_2_17_23_41_31_1979 "RTSP QHSM_Rtsp recv RTSP_HSM_DESTROY_HSM"
#define RTSP_HSM_1668_112_2_17_23_41_31_1980 "RTSP QHSM_Rtsp recv RTSP_HSM_TEARDOWN_REQ"
#define RTSP_HSM_1687_112_2_17_23_41_31_1981 "\n rtsp QHSM_Rtsp param error\n"
#define RTSP_HSM_1704_112_2_17_23_41_31_1982 "RTSP RTSP_DealSignal input error"
#define RTSP_HSM_1713_112_2_17_23_41_31_1983 "RTSP RTSP_DealSignal RTSP_SIG_INIT_SESSION_REQ"
#define RTSP_HSM_1718_112_2_17_23_41_31_1984 "RTSP RTSP_DealSignal RTSP_SIG_CLOSE_SESSION_REQ"
#define RTSP_HSM_1723_112_2_17_23_41_31_1985 "RTSP RTSP_DealSignal RTSP_SIG_OPTIONS_AUTH_RSP"
#define RTSP_HSM_1728_112_2_17_23_41_31_1986 "RTSP RTSP_DealSignal RTSP_SIG_DESCRIBE_AUTH_RSP"
#define RTSP_HSM_1733_112_2_17_23_41_31_1987 "RTSP RTSP_DealSignal RTSP_SIG_SETUP_AUTH_RSP"
#define RTSP_HSM_1738_112_2_17_23_41_31_1988 "RTSP RTSP_DealSignal RTSP_SIG_PLAY_AUTH_RSP"
#define RTSP_HSM_1744_112_2_17_23_41_31_1989 "RTSP RTSP_DealSignal RTSP_SIG_TEARDOWN_REQ"
#define RTSP_HSM_1749_112_2_17_23_41_31_1990 "RTSP RTSP_DealSignal RTSP_SIG_PERIOD_OPTIONS_TIMEOUT_IND"
#define RTSP_HSM_1753_112_2_17_23_41_31_1991 "RTSP RTSP_DealSignal RTSP_SIG_TIMEOUT_IND"
#define RTSP_HSM_1757_112_2_17_23_41_31_1992 "\n RTSP RTSP_DealSignal signal = %d\n"
#define RTSP_HSM_1804_112_2_17_23_41_31_1993 "RTSP RtspHandleOptionsAuthRsp input error"
#define RTSP_HSM_1845_112_2_17_23_41_31_1994 "RTSP RtspHandleDescribeAuthRsp input error"
#define RTSP_HSM_1886_112_2_17_23_41_31_1995 "RTSP RtspHandlePlayAuthRsp input error"
#define RTSP_HSM_1927_112_2_17_23_41_31_1996 "RTSP RtspHandleSetupAuthRsp input error"
#define RTSP_HSM_1971_112_2_17_23_41_31_1997 "RTSP RtspHandlePeriodOptions input error"
#define RTSP_HSM_2003_112_2_17_23_41_31_1998 "RTSP RtspHandleTimeoutInd input error"
#define RTSP_HSM_2044_112_2_17_23_41_31_1999 "RTSP RtspHandleTeardownReq input error"
#define RTSP_HSM_2092_112_2_17_23_41_31_2000 "\n rtsp Rtsp_Init param error\n"
#define RTSP_HSM_2118_112_2_17_23_41_31_2001 "RTSP Rtsp_InitIdle recv RTSP_HSM_SETUP_REQ"
#define RTSP_HSM_2125_112_2_17_23_41_31_2002 "RTSP Rtsp_InitIdle recv RTSP_HSM_DESCRIBE_REQ"
#define RTSP_HSM_2132_112_2_17_23_41_31_2003 "RTSP Rtsp_InitIdle recv RTSP_HSM_STOP_REQ"
#define RTSP_HSM_2144_112_2_17_23_41_31_2004 "\n rtsp Rtsp_InitIdle param error\n"
#define RTSP_HSM_2163_112_2_17_23_41_32_2005 "\n rtsp Rtsp_InitDescribePending param error\n"
#define RTSP_HSM_2181_112_2_17_23_41_32_2006 "RTSP Rtsp_InitDescribePendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_2203_112_2_17_23_41_32_2007 "RTSP Rtsp_InitDescribePendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_2242_112_2_17_23_41_32_2008 "RTSP Rtsp_InitDescribePendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_2246_112_2_17_23_41_32_2009 "RTSP Rtsp_InitDescribePendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_2268_112_2_17_23_41_32_2010 "\n rtsp Rtsp_InitDescribePendingSending param error\n"
#define RTSP_HSM_2316_112_2_17_23_41_32_2011 "RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_AUTH_IND"
#define RTSP_HSM_2319_112_2_17_23_41_32_2012 "RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_AUTH_IND param error"
#define RTSP_HSM_2347_112_2_17_23_41_32_2013 "RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_OK_IND"
#define RTSP_HSM_2367_112_2_17_23_41_32_2014 "RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_ERROR_IND"
#define RTSP_HSM_2387_112_2_17_23_41_32_2015 "RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_REDIRECT_IND"
#define RTSP_HSM_2402_112_2_17_23_41_32_2016 "\n rtsp Rtsp_InitDescribePendingRecving param error\n"
#define RTSP_HSM_2423_112_2_17_23_41_32_2017 "RTSP Rtsp_InitDescribePendingAuthPending recv RTSP_HSM_DESCRIBE_AUTH_RSP"
#define RTSP_HSM_2461_112_2_17_23_41_32_2018 "\n rtsp Rtsp_InitDescribePendingAuthPending param error\n"
#define RTSP_HSM_2482_112_2_17_23_41_32_2019 "RTSP RtspHandleDescribePendingAuthRsp input error"
#define RTSP_HSM_2545_112_2_17_23_41_32_2020 "RTSP RTSP_SaveAuthParamToContext input error"
#define RTSP_HSM_2579_112_2_17_23_41_32_2021 "\n rtsp SaveAuthParamToContext error_code=%d"
#define RTSP_HSM_2597_112_2_17_23_41_32_2022 "\n rtsp Rtsp_InitDescribePending param error\n"
#define RTSP_HSM_2615_112_2_17_23_41_32_2023 "RTSP Rtsp_InitSetupPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_2626_112_2_17_23_41_32_2024 "RTSP Rtsp_InitSetupPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_2666_112_2_17_23_41_33_2025 "RTSP Rtsp_InitSetupPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_2670_112_2_17_23_41_33_2026 "RTSP Rtsp_InitSetupPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_2694_112_2_17_23_41_33_2027 "\n rtsp Rtsp_InitSetupPendingSending param error\n"
#define RTSP_HSM_2747_112_2_17_23_41_33_2028 "RTSP Rtsp_InitSetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND"
#define RTSP_HSM_2750_112_2_17_23_41_33_2029 "RTSP Rtsp_InitSetupPendingRecving param RTSP_HSM_SETUP_AUTH_IND error"
#define RTSP_HSM_2777_112_2_17_23_41_33_2030 "RTSP Rtsp_InitSetupPendingRecving recv RTSP_HSM_SETUP_OK_IND"
#define RTSP_HSM_2800_112_2_17_23_41_33_2031 "RTSP setup 1 sender =%d,context_id = %d"
#define RTSP_HSM_2812_112_2_17_23_41_33_2032 "RTSP Rtsp_InitSetupPendingRecving recv RTSP_HSM_SETUP_ERROR_IND"
#define RTSP_HSM_2835_112_2_17_23_41_33_2033 "RTSP Rtsp_InitSetupPendingRecving recv RTSP_HSM_SETUP_REDIRECT_IND"
#define RTSP_HSM_2864_112_2_17_23_41_33_2034 "\n rtsp Rtsp_InitSetupPendingRecving param error\n"
#define RTSP_HSM_2886_112_2_17_23_41_33_2035 "RTSP Rtsp_InitSetupPendingAuthPending recv RTSP_HSM_SETUP_AUTH_RSP"
#define RTSP_HSM_2927_112_2_17_23_41_33_2036 "\n rtsp Rtsp_InitSetupPendingAuthPending param error\n"
#define RTSP_HSM_2948_112_2_17_23_41_33_2037 "RTSP RtspHandleSetupPendingAuthRsp input error"
#define RTSP_HSM_3012_112_2_17_23_41_33_2038 "RTSP RtspHandlePlayPendingAuthRsp input error"
#define RTSP_HSM_3073_112_2_17_23_41_33_2039 "\n rtsp Rtsp_InitDescribePending param error\n"
#define RTSP_HSM_3091_112_2_17_23_41_33_2040 "RTSP Rtsp_InitTeardownPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_3112_112_2_17_23_41_33_2041 "RTSP Rtsp_InitTeardownPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_3150_112_2_17_23_41_34_2042 "RTSP Rtsp_InitTeardownPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_3154_112_2_17_23_41_34_2043 "RTSP Rtsp_InitTeardownPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_3175_112_2_17_23_41_34_2044 "\n rtsp Rtsp_InitTeardownPendingSending param error\n"
#define RTSP_HSM_3221_112_2_17_23_41_34_2045 "RTSP Rtsp_InitTeardownPendingRecving recv RTSP_HSM_TEARDOWN_OK_IND"
#define RTSP_HSM_3245_112_2_17_23_41_34_2046 "RTSP Rtsp_InitTeardownPendingRecving recv RTSP_HSM_TEARDOWN_ERROR_IND"
#define RTSP_HSM_3275_112_2_17_23_41_34_2047 "\n rtsp Rtsp_InitTeardownPendingRecving param error\n"
#define RTSP_HSM_3301_112_2_17_23_41_34_2048 "\n rtsp Rtsp_Ready param error\n"
#define RTSP_HSM_3327_112_2_17_23_41_34_2049 "RTSP Rtsp_ReadyOnready recv RTSP_HSM_PLAY_REQ"
#define RTSP_HSM_3334_112_2_17_23_41_34_2050 "RTSP Rtsp_ReadyOnready recv RTSP_HSM_STOP_REQ"
#define RTSP_HSM_3341_112_2_17_23_41_34_2051 "RTSP Rtsp_ReadyOnready recv RTSP_HSM_SETUP_REQ"
#define RTSP_HSM_3348_112_2_17_23_41_34_2052 "RTSP Rtsp_ReadyOnready recv RTSP_HSM_ANNOUNCE_REQ"
#define RTSP_HSM_3361_112_2_17_23_41_34_2053 "\n rtsp Rtsp_Ready param error\n"
#define RTSP_HSM_3388_112_2_17_23_41_34_2054 "\n rtsp Rtsp_ReadyPlayPending param error\n"
#define RTSP_HSM_3409_112_2_17_23_41_34_2055 "RTSP Rtsp_ReadyPlayPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_3413_112_2_17_23_41_34_2056 "RTSP Rtsp_ReadyPlayPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_3434_112_2_17_23_41_34_2057 "\n rtsp Rtsp_ReadyPlayPendingSending param error\n"
#define RTSP_HSM_3482_112_2_17_23_41_34_2058 "RTSP Rtsp_ReadyPlayPendingRecving recv RTSP_HSM_PLAY_OK_IND"
#define RTSP_HSM_3505_112_2_17_23_41_34_2059 "RTSP Rtsp_ReadyPlayPendingRecving recv RTSP_HSM_PLAY_ERROR_IND"
#define RTSP_HSM_3527_112_2_17_23_41_34_2060 "RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_REDIRECT_IND"
#define RTSP_HSM_3537_112_2_17_23_41_34_2061 "RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_AUTH_IND"
#define RTSP_HSM_3543_112_2_17_23_41_34_2062 "RTSP RECV RTSP_HSM_PLAY_AUTH_IND SEND PLAY"
#define RTSP_HSM_3556_112_2_17_23_41_34_2063 "RTSP RECV RTSP_HSM_PLAY_AUTH_IND SEND AUTH IND"
#define RTSP_HSM_3577_112_2_17_23_41_34_2064 "\n rtsp Rtsp_ReadyPlayPendingRecving param error\n"
#define RTSP_HSM_3604_112_2_17_23_41_34_2065 "\n rtsp Rtsp_ReadyTeardownPending param error\n"
#define RTSP_HSM_3625_112_2_17_23_41_34_2066 "RTSP Rtsp_ReadyTeardownPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_3629_112_2_17_23_41_34_2067 "RTSP Rtsp_ReadyTeardownPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_3650_112_2_17_23_41_35_2068 "\n rtsp Rtsp_ReadyTeardownPendingSending param error\n"
#define RTSP_HSM_3698_112_2_17_23_41_35_2069 "RTSP Rtsp_ReadyTeardownPendingRecving recv RTSP_HSM_TEARDOWN_OK_IND"
#define RTSP_HSM_3720_112_2_17_23_41_35_2070 "RTSP Rtsp_ReadyTeardownPendingRecving recv RTSP_HSM_TEARDOWN_ERROR_IND"
#define RTSP_HSM_3748_112_2_17_23_41_35_2071 "\n rtsp Rtsp_ReadyTeardownPendingRecving param error\n"
#define RTSP_HSM_3774_112_2_17_23_41_35_2072 "\n rtsp Rtsp_ReadySetupPending param error\n"
#define RTSP_HSM_3792_112_2_17_23_41_35_2073 "RTSP Rtsp_ReadyPlayPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_3810_112_2_17_23_41_35_2074 "RTSP Rtsp_ReadyPlayPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_3844_112_2_17_23_41_35_2075 "RTSP Rtsp_ReadySetupPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_3865_112_2_17_23_41_35_2076 "RTSP Rtsp_ReadySetupPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_3902_112_2_17_23_41_35_2077 "RTSP Rtsp_ReadyTeardownPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_3920_112_2_17_23_41_35_2078 "RTSP Rtsp_ReadyTeardownPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_3954_112_2_17_23_41_35_2079 "RTSP Rtsp_ReadyAnnouncePendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_3972_112_2_17_23_41_35_2080 "RTSP Rtsp_ReadyAnnouncePendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_4006_112_2_17_23_41_35_2081 "RTSP Rtsp_ReadyRecordPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_4024_112_2_17_23_41_35_2082 "RTSP Rtsp_ReadyRecordPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_4060_112_2_17_23_41_35_2083 "RTSP Rtsp_ReadySetupPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_4064_112_2_17_23_41_35_2084 "RTSP Rtsp_ReadySetupPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_4088_112_2_17_23_41_35_2085 "\n rtsp Rtsp_ReadySetupPendingSending param error\n"
#define RTSP_HSM_4145_112_2_17_23_41_36_2086 "RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND"
#define RTSP_HSM_4148_112_2_17_23_41_36_2087 "RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND param error"
#define RTSP_HSM_4176_112_2_17_23_41_36_2088 "RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_OK_IND"
#define RTSP_HSM_4199_112_2_17_23_41_36_2089 "RTSP setup 2 sender =%d,context_id = %d"
#define RTSP_HSM_4212_112_2_17_23_41_36_2090 "RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_ERROR_IND"
#define RTSP_HSM_4235_112_2_17_23_41_36_2091 "RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_REDIRECT_IND"
#define RTSP_HSM_4264_112_2_17_23_41_36_2092 "\n rtsp Rtsp_ReadySetupPendingRecving param error\n"
#define RTSP_HSM_4286_112_2_17_23_41_36_2093 "RTSP Rtsp_ReadySetupPendingAuthPending recv RTSP_HSM_SETUP_AUTH_RSP"
#define RTSP_HSM_4327_112_2_17_23_41_36_2094 "\n rtsp Rtsp_ReadySetupPendingAuthPending param error\n"
#define RTSP_HSM_4353_112_2_17_23_41_36_2095 "\n rtsp Rtsp_ReadyAnnouncePending param error\n"
#define RTSP_HSM_4373_112_2_17_23_41_36_2096 "RTSP Rtsp_ReadyAnnouncePendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_4377_112_2_17_23_41_36_2097 "RTSP Rtsp_ReadyAnnouncePendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_4398_112_2_17_23_41_36_2098 "\n rtsp Rtsp_ReadyAnnouncePendingSending param error\n"
#define RTSP_HSM_4446_112_2_17_23_41_36_2099 "RTSP Rtsp_ReadyAnnouncePendingRecving recv RTSP_HSM_ANNOUNCE_OK_IND"
#define RTSP_HSM_4465_112_2_17_23_41_36_2100 "RTSP Rtsp_ReadyAnnouncePendingRecving recv RTSP_HSM_ANNOUNCE_ERROR_IND"
#define RTSP_HSM_4490_112_2_17_23_41_36_2101 "\n rtsp Rtsp_ReadyAnnouncePendingRecving param error\n"
#define RTSP_HSM_4510_112_2_17_23_41_36_2102 "RTSP Rtsp_ReadyAnnounced recv RTSP_HSM_STOP_REQ"
#define RTSP_HSM_4518_112_2_17_23_41_36_2103 "RTSP Rtsp_ReadyAnnounced recv RTSP_HSM_RECORD_REQ"
#define RTSP_HSM_4532_112_2_17_23_41_36_2104 "\n rtsp Rtsp_ReadyAnnouncd param error\n"
#define RTSP_HSM_4558_112_2_17_23_41_36_2105 "\n rtsp Rtsp_ReadyRecordPending param error\n"
#define RTSP_HSM_4579_112_2_17_23_41_36_2106 "RTSP Rtsp_ReadyRecordPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_4583_112_2_17_23_41_36_2107 "RTSP Rtsp_ReadyRecordPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_4604_112_2_17_23_41_36_2108 "\n rtsp Rtsp_ReadyRecordPendingSending param error\n"
#define RTSP_HSM_4650_112_2_17_23_41_37_2109 "RTSP Rtsp_ReadyRecordPendingRecving recv RTSP_HSM_RECORD_OK_IND"
#define RTSP_HSM_4670_112_2_17_23_41_37_2110 "RTSP Rtsp_ReadyRecordPendingRecving recv RTSP_HSM_RECORD_ERROR_IND"
#define RTSP_HSM_4695_112_2_17_23_41_37_2111 "\n rtsp Rtsp_ReadyRecordPendingRecving param error\n"
#define RTSP_HSM_4721_112_2_17_23_41_37_2112 "\n rtsp Rtsp_Playing param error\n"
#define RTSP_HSM_4747_112_2_17_23_41_37_2113 "RTSP Rtsp_PlayingPlay recv RTSP_HSM_PLAY_REQ"
#define RTSP_HSM_4754_112_2_17_23_41_37_2114 "RTSP Rtsp_PlayingPlay recv RTSP_HSM_STOP_REQ"
#define RTSP_HSM_4761_112_2_17_23_41_37_2115 "RTSP Rtsp_PlayingPlay recv RTSP_HSM_SETUP_REQ"
#define RTSP_HSM_4768_112_2_17_23_41_37_2116 "RTSP Rtsp_PlayingPlay recv RTSP_HSM_PAUSE_REQ"
#define RTSP_HSM_4781_112_2_17_23_41_37_2117 "\n rtsp Rtsp_Playing param error\n"
#define RTSP_HSM_4800_112_2_17_23_41_37_2118 "\n rtsp Rtsp_PlayingPlayPending param error\n"
#define RTSP_HSM_4818_112_2_17_23_41_37_2119 "RTSP Rtsp_PlayingPlayPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_4837_112_2_17_23_41_37_2120 "RTSP Rtsp_PlayingPlayPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_4873_112_2_17_23_41_37_2121 "RTSP Rtsp_PlayingPlayPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_4877_112_2_17_23_41_37_2122 "RTSP Rtsp_PlayingPlayPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_4898_112_2_17_23_41_37_2123 "\n rtsp Rtsp_PlayingPlayPendingSending param error\n"
#define RTSP_HSM_4944_112_2_17_23_41_37_2124 "RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_OK_IND"
#define RTSP_HSM_4965_112_2_17_23_41_37_2125 "RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_REDIRECT_IND"
#define RTSP_HSM_4975_112_2_17_23_41_37_2126 "RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_AUTH_IND"
#define RTSP_HSM_4981_112_2_17_23_41_37_2127 "RTSP RECV RTSP_HSM_PLAY_AUTH_IND SEND PLAY"
#define RTSP_HSM_4996_112_2_17_23_41_37_2128 "RTSP RECV RTSP_HSM_PLAY_AUTH_IND SEND AUTH IND"
#define RTSP_HSM_5009_112_2_17_23_41_37_2129 "RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_ERROR_IND"
#define RTSP_HSM_5034_112_2_17_23_41_37_2130 "\n rtsp Rtsp_PlayingPlayPendingRecving param error\n"
#define RTSP_HSM_5053_112_2_17_23_41_37_2131 "\n rtsp Rtsp_PlayingPausePending param error\n"
#define RTSP_HSM_5071_112_2_17_23_41_37_2132 "RTSP Rtsp_PlayingPausePendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_5090_112_2_17_23_41_37_2133 "RTSP Rtsp_PlayingPausePendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_5127_112_2_17_23_41_38_2134 "RTSP Rtsp_PlayingPausePendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_5131_112_2_17_23_41_38_2135 "RTSP Rtsp_PlayingPausePendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_5152_112_2_17_23_41_38_2136 "\n rtsp Rtsp_PlayingPausePendingSending param error\n"
#define RTSP_HSM_5201_112_2_17_23_41_38_2137 "RTSP Rtsp_PlayingPausePendingRecving recv RTSP_HSM_PAUSE_OK_IND"
#define RTSP_HSM_5222_112_2_17_23_41_38_2138 "RTSP Rtsp_PlayingPausePendingRecving recv RTSP_HSM_PAUSE_ERROR_IND"
#define RTSP_HSM_5248_112_2_17_23_41_38_2139 "\n rtsp Rtsp_PlayingPausePendingRecving param error\n"
#define RTSP_HSM_5267_112_2_17_23_41_38_2140 "\n rtsp Rtsp_PlayingTeardownPending param error\n"
#define RTSP_HSM_5285_112_2_17_23_41_38_2141 "RTSP Rtsp_PlayingTeardownPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_5304_112_2_17_23_41_38_2142 "RTSP Rtsp_PlayingTeardownPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_5341_112_2_17_23_41_38_2143 "RTSP Rtsp_PlayingTeardownPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_5345_112_2_17_23_41_38_2144 "RTSP Rtsp_PlayingTeardownPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_5366_112_2_17_23_41_38_2145 "\n rtsp Rtsp_PlayingTeardownPendingSending param error\n"
#define RTSP_HSM_5413_112_2_17_23_41_38_2146 "RTSP Rtsp_PlayingTeardownPendingRecving recv RTSP_HSM_TEARDOWN_OK_IND"
#define RTSP_HSM_5436_112_2_17_23_41_38_2147 "RTSP Rtsp_PlayingTeardownPendingRecving recv RTSP_HSM_TEARDOWN_ERROR_IND"
#define RTSP_HSM_5464_112_2_17_23_41_38_2148 "\n rtsp Rtsp_PlayingTeardownPendingRecving param error\n"
#define RTSP_HSM_5491_112_2_17_23_41_38_2149 "\n rtsp Rtsp_PlayingSetupPending param error\n"
#define RTSP_HSM_5509_112_2_17_23_41_38_2150 "RTSP Rtsp_PlayingSetupPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_5531_112_2_17_23_41_38_2151 "RTSP Rtsp_PlayingSetupPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_5571_112_2_17_23_41_38_2152 "RTSP Rtsp_PlayingSetupPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_5575_112_2_17_23_41_38_2153 "RTSP Rtsp_PlayingSetupPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_5599_112_2_17_23_41_38_2154 "\n rtsp Rtsp_PlayingSetupPendingSending param error\n"
#define RTSP_HSM_5655_112_2_17_23_41_39_2155 "RTSP Rtsp_PlayingSetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND"
#define RTSP_HSM_5687_112_2_17_23_41_39_2156 "RTSP Rtsp_PlayingSetupPendingRecving recv RTSP_HSM_SETUP_OK_IND"
#define RTSP_HSM_5710_112_2_17_23_41_39_2157 "RTSP setup 3 sender =%d,context_id = %d"
#define RTSP_HSM_5722_112_2_17_23_41_39_2158 "RTSP Rtsp_PlayingSetupPendingRecving recv RTSP_HSM_SETUP_ERROR_IND"
#define RTSP_HSM_5745_112_2_17_23_41_39_2159 "RTSP Rtsp_PlayingSetupPendingRecving recv RTSP_HSM_SETUP_REDIRECT_IND"
#define RTSP_HSM_5772_112_2_17_23_41_39_2160 "\n rtsp Rtsp_PlayingSetupPendingRecving param error\n"
#define RTSP_HSM_5794_112_2_17_23_41_39_2161 "RTSP Rtsp_PlayingSetupPendingAuthPending recv RTSP_HSM_SETUP_AUTH_RSP"
#define RTSP_HSM_5837_112_2_17_23_41_39_2162 "\n rtsp Rtsp_PlayingSetupPendingAuthPending param error\n"
#define RTSP_HSM_5859_112_2_17_23_41_39_2163 "RTSP Rtsp_PlayingPlayPendingAuthPending recv RTSP_HSM_PLAY_AUTH_RSP"
#define RTSP_HSM_5899_112_2_17_23_41_39_2164 "\n rtsp Rtsp_PlayingPlayPendingAuthPending param error\n"
#define RTSP_HSM_5921_112_2_17_23_41_39_2165 "RTSP Rtsp_PlayingPlayPendingAuthPending recv RTSP_HSM_PLAY_AUTH_RSP"
#define RTSP_HSM_5961_112_2_17_23_41_39_2166 "\n rtsp Rtsp_PlayingPlayPendingAuthPending param error\n"
#define RTSP_HSM_5988_112_2_17_23_41_39_2167 "\n rtsp Rtsp_Recording param error\n"
#define RTSP_HSM_6016_112_2_17_23_41_39_2168 "RTSP Rtsp_RecordingRecord recv RTSP_HSM_RECORD_REQ"
#define RTSP_HSM_6023_112_2_17_23_41_39_2169 "RTSP Rtsp_RecordingRecord recv RTSP_HSM_STOP_REQ"
#define RTSP_HSM_6030_112_2_17_23_41_39_2170 "RTSP Rtsp_RecordingRecord recv RTSP_HSM_SETUP_REQ"
#define RTSP_HSM_6037_112_2_17_23_41_39_2171 "RTSP Rtsp_RecordingRecord recv RTSP_HSM_PAUSE_REQ"
#define RTSP_HSM_6052_112_2_17_23_41_39_2172 "\n rtsp Rtsp_Recordinf param error\n"
#define RTSP_HSM_6079_112_2_17_23_41_39_2173 "\n rtsp Rtsp_RecordingRecordPending param error\n"
#define RTSP_HSM_6100_112_2_17_23_41_39_2174 "RTSP Rtsp_RecordingRecordPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_6104_112_2_17_23_41_39_2175 "RTSP Rtsp_RecordingRecordPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_6125_112_2_17_23_41_40_2176 "\n rtsp Rtsp_RecordingRecordPendingSending param error\n"
#define RTSP_HSM_6174_112_2_17_23_41_40_2177 "RTSP Rtsp_RecordingRecordPendingRecving recv RTSP_HSM_RECORD_OK_IND"
#define RTSP_HSM_6198_112_2_17_23_41_40_2178 "RTSP Rtsp_RecordingRecordPendingRecving recv RTSP_HSM_RECORD_ERROR_IND"
#define RTSP_HSM_6227_112_2_17_23_41_40_2179 "\n rtsp Rtsp_RecordingRecordPendingRecving param error\n"
#define RTSP_HSM_6254_112_2_17_23_41_40_2180 "\n rtsp Rtsp_RecordingPausePending param error\n"
#define RTSP_HSM_6275_112_2_17_23_41_40_2181 "RTSP Rtsp_RecordingPausePendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_6279_112_2_17_23_41_40_2182 "RTSP Rtsp_RecordingPausePendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_6300_112_2_17_23_41_40_2183 "\n rtsp Rtsp_RecordingPausePendingSending param error\n"
#define RTSP_HSM_6347_112_2_17_23_41_40_2184 "RTSP Rtsp_RecordingPausePendingRecving recv RTSP_HSM_PAUSE_OK_IND"
#define RTSP_HSM_6368_112_2_17_23_41_40_2185 "RTSP Rtsp_RecordingPausePendingRecving recv RTSP_HSM_PAUSE_ERROR_IND"
#define RTSP_HSM_6393_112_2_17_23_41_40_2186 "\n rtsp Rtsp_RecordingPausePendingRecving param error\n"
#define RTSP_HSM_6420_112_2_17_23_41_40_2187 "\n rtsp Rtsp_RecordingTeardownPending param error\n"
#define RTSP_HSM_6441_112_2_17_23_41_40_2188 "RTSP Rtsp_RecordingTeardownPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_6445_112_2_17_23_41_40_2189 "RTSP Rtsp_RecordingTeardownPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_6466_112_2_17_23_41_40_2190 "\n rtsp Rtsp_RecordingTeardownPendingSending param error\n"
#define RTSP_HSM_6514_112_2_17_23_41_40_2191 "RTSP Rtsp_RecordingTeardownPendingRecving recv RTSP_HSM_TEARDOWN_OK_IND"
#define RTSP_HSM_6540_112_2_17_23_41_40_2192 "RTSP Rtsp_RecordingTeardownPendingRecving recv RTSP_HSM_TEARDOWN_ERROR_IND"
#define RTSP_HSM_6571_112_2_17_23_41_40_2193 "\n rtsp Rtsp_RecordingTeardownPendingRecving param error\n"
#define RTSP_HSM_6598_112_2_17_23_41_40_2194 "\n rtsp Rtsp_RecordingSetupPending param error\n"
#define RTSP_HSM_6620_112_2_17_23_41_41_2195 "RTSP Rtsp_RecordingSetupPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_6642_112_2_17_23_41_41_2196 "RTSP Rtsp_RecordingSetupPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_6684_112_2_17_23_41_41_2197 "RTSP Rtsp_RecordingRecordPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_6703_112_2_17_23_41_41_2198 "RTSP Rtsp_RecordingRecordPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_6742_112_2_17_23_41_41_2199 "RTSP Rtsp_RecordingTeardownPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_6761_112_2_17_23_41_41_2200 "RTSP Rtsp_RecordingTeardownPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_6800_112_2_17_23_41_41_2201 "RTSP Rtsp_RecordingPausePendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_HSM_6819_112_2_17_23_41_41_2202 "RTSP Rtsp_RecordingPausePendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_HSM_6857_112_2_17_23_41_41_2203 "RTSP Rtsp_RecordingSetupPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_HSM_6861_112_2_17_23_41_41_2204 "RTSP Rtsp_RecordingSetupPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_HSM_6885_112_2_17_23_41_41_2205 "\n rtsp Rtsp_RecordingSetupPendingSending param error\n"
#define RTSP_HSM_6940_112_2_17_23_41_41_2206 "RTSP Rtsp_RecordingSetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND"
#define RTSP_HSM_6972_112_2_17_23_41_41_2207 "RTSP Rtsp_RecordingSetupPendingRecving recv RTSP_HSM_SETUP_OK_IND"
#define RTSP_HSM_6995_112_2_17_23_41_41_2208 "RTSP setup 4 sender =%d,context_id = %d"
#define RTSP_HSM_7008_112_2_17_23_41_41_2209 "RTSP Rtsp_RecordingSetupPendingRecving recv RTSP_HSM_SETUP_ERROR_IND"
#define RTSP_HSM_7031_112_2_17_23_41_41_2210 "RTSP Rtsp_RecordingSetupPendingRecving recv RTSP_HSM_SETUP_REDIRECT_IND"
#define RTSP_HSM_7059_112_2_17_23_41_41_2211 "\n rtsp Rtsp_RecordingSetupPendingRecving param error\n"
#define RTSP_HSM_7081_112_2_17_23_41_41_2212 "RTSP Rtsp_RecordingSetupPendingAuthPending recv RTSP_HSM_SETUP_AUTH_RSP"
#define RTSP_HSM_7142_112_2_17_23_41_42_2213 "\n rtsp Rtsp_RecordingSetupPendingAuthPending param error\n"
#define RTSP_HSM_7160_112_2_17_23_41_42_2214 "\n rtsp RTSP_SigErrCnf RTSP_SIG_INIT_SESSION_REQ\n"
#define RTSP_HSM_7165_112_2_17_23_41_42_2215 "\n rtsp RTSP_SigErrCnf RTSP_SIG_CLOSE_SESSION_REQ\n"
#define RTSP_HSM_7170_112_2_17_23_41_42_2216 "\n rtsp RTSP_SigErrCnf RTSP_SIG_OPTIONS_REQ\n"
#define RTSP_HSM_7175_112_2_17_23_41_42_2217 "\n rtsp RTSP_SigErrCnf RTSP_SIG_DESCRIBE_REQ\n"
#define RTSP_HSM_7180_112_2_17_23_41_42_2218 "\n rtsp RTSP_SigErrCnf RTSP_SIG_SET_PARAM_REQ\n"
#define RTSP_HSM_7185_112_2_17_23_41_42_2219 "\n rtsp RTSP_SigErrCnf RTSP_SIG_SETUP_REQ\n"
#define RTSP_HSM_7190_112_2_17_23_41_42_2220 "\n rtsp RTSP_SigErrCnf RTSP_SIG_PLAY_REQ\n"
#define RTSP_HSM_7195_112_2_17_23_41_42_2221 "\n rtsp RTSP_SigErrCnf RTSP_SIG_PAUSE_REQ\n"
#define RTSP_HSM_7200_112_2_17_23_41_42_2222 "\n rtsp RTSP_SigErrCnf RTSP_SIG_ANNOUNCE_REQ\n"
#define RTSP_HSM_7205_112_2_17_23_41_42_2223 "\n rtsp RTSP_SigErrCnf RTSP_SIG_RECORD_REQ\n"
#define RTSP_HSM_7210_112_2_17_23_41_42_2224 "\n rtsp RTSP_SigErrCnf RTSP_SIG_TEARDOWN_REQ\n"
#define RTSP_HSM_7215_112_2_17_23_41_42_2225 "\n rtsp RTSP_SigErrCnf RTSP_SIG_OPTIONS_AUTH_RSP\n"
#define RTSP_HSM_7220_112_2_17_23_41_42_2226 "\n rtsp RTSP_SigErrCnf RTSP_SIG_SET_PARAM_AUTH_RSP\n"
#define RTSP_HSM_7225_112_2_17_23_41_42_2227 "\n rtsp RTSP_SigErrCnf RTSP_SIG_DESCRIBE_AUTH_RSP\n"
#define RTSP_HSM_7229_112_2_17_23_41_42_2228 "\n rtsp RTSP_SigErrCnf RTSP_SIG_PLAY_AUTH_RSP\n"
#define RTSP_HSM_7234_112_2_17_23_41_42_2229 "\n rtsp RTSP_SigErrCnf RTSP_SIG_SETUP_AUTH_RSP\n"
#define RTSP_HSM_7243_112_2_17_23_41_42_2230 "\n HTTP RTSP_SigErrCnf unknown signal \n"
#define RTSP_HSM_7647_112_2_17_23_41_43_2231 "\n rtsp RtspHandleHsmFree hsm_ptr = %d"
#define RTSP_METHOD_174_112_2_17_23_41_43_2232 "RTSP RTSPMTD_FreeMethodChart method_chart_list =%d"
#define RTSP_OPTIONS_HSM_137_112_2_17_23_41_44_2233 "RTSP RTSPHSM_OptionsDispatchSignal dispatch signal"
#define RTSP_OPTIONS_HSM_139_112_2_17_23_41_44_2234 "RTSP RTSPHSM_OptionsDispatchSignal hsm_state =%d"
#define RTSP_OPTIONS_HSM_143_112_2_17_23_41_44_2235 "RTSP RTSPHSM_OptionsDispatchSignal event =%d"
#define RTSP_OPTIONS_HSM_173_112_2_17_23_41_44_2236 "\n rtsp RtspOptionsConstructor param error\n"
#define RTSP_OPTIONS_HSM_190_112_2_17_23_41_44_2237 "\n rtsp RtspDestructor param error\n"
#define RTSP_OPTIONS_HSM_217_112_2_17_23_41_44_2238 "\n rtsp QHSM_RtspOptions param error\n"
#define RTSP_OPTIONS_HSM_235_112_2_17_23_41_44_2239 "\n rtsp Rtsp_OptionsPendingIdle RTSP_HSM_OPTIONS_REQ"
#define RTSP_OPTIONS_HSM_247_112_2_17_23_41_44_2240 "\n rtsp Rtsp_OptionsPendingIdle param error\n"
#define RTSP_OPTIONS_HSM_273_112_2_17_23_41_44_2241 "\n rtsp Rtsp_OptionsPendingConnecting RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_OPTIONS_HSM_279_112_2_17_23_41_44_2242 "\n rtsp Rtsp_OptionsPendingConnecting RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_OPTIONS_HSM_328_112_2_17_23_41_44_2243 "\n rtsp Rtsp_OptionsPendingSending RTSP_HSM_SEND_SUCC_IND"
#define RTSP_OPTIONS_HSM_333_112_2_17_23_41_44_2244 "\n rtsp Rtsp_OptionsPendingSending RTSP_HSM_SEND_FAIL_IND"
#define RTSP_OPTIONS_HSM_354_112_2_17_23_41_44_2245 "\n rtsp Rtsp_OptionsPendingSending param error\n"
#define RTSP_OPTIONS_HSM_402_112_2_17_23_41_44_2246 "\n rtsp Rtsp_OptionsPendingRecving RTSP_HSM_OPTIONS_AUTH_IND"
#define RTSP_OPTIONS_HSM_433_112_2_17_23_41_44_2247 "\n rtsp Rtsp_OptionsPendingRecving RTSP_HSM_OPTIONS_OK_IND"
#define RTSP_OPTIONS_HSM_458_112_2_17_23_41_44_2248 "\n rtsp Rtsp_OptionsPendingRecving RTSP_HSM_OPTIONS_ERROR_IND"
#define RTSP_OPTIONS_HSM_483_112_2_17_23_41_44_2249 "\n rtsp Rtsp_OptionsPendingRecving RTSP_HSM_OPTIONS_REDIRECT_IND"
#define RTSP_OPTIONS_HSM_499_112_2_17_23_41_44_2250 "\n rtsp Rtsp_OptionsPendingRecving param error\n"
#define RTSP_OPTIONS_HSM_522_112_2_17_23_41_44_2251 "\n rtsp Rtsp_OptionsPendingAuthPending RTSP_HSM_OPTIONS_AUTH_RSP"
#define RTSP_OPTIONS_HSM_578_112_2_17_23_41_45_2252 "\n rtsp Rtsp_OptionsPendingAuthPending param error\n"
#define RTSP_OPTIONS_HSM_658_112_2_17_23_41_45_2253 "\n rtsp RtspOptionsInitial param error\n"
#define RTSP_REQUEST_147_112_2_17_23_41_45_2254 "RTSP RTSP_CreateRequest header_list_handle =%d"
#define RTSP_REQUEST_193_112_2_17_23_41_45_2255 "RTSP RTSP_CreateRequest options"
#define RTSP_REQUEST_219_112_2_17_23_41_45_2256 "RTSP RTSP_CreateRequest self options"
#define RTSP_REQUEST_247_112_2_17_23_41_45_2257 "RTSP RTSP_CreateRequest describe"
#define RTSP_REQUEST_274_112_2_17_23_41_45_2258 "RTSP RTSP_CreateRequest setup"
#define RTSP_REQUEST_298_112_2_17_23_41_45_2259 "RTSP RTSP_CreateRequest self setup"
#define RTSP_REQUEST_322_112_2_17_23_41_45_2260 "RTSP RTSP_CreateRequest play"
#define RTSP_REQUEST_342_112_2_17_23_41_45_2261 "RTSP RTSP_CreateRequest pause"
#define RTSP_REQUEST_363_112_2_17_23_41_45_2262 "RTSP RTSP_CreateRequest announce"
#define RTSP_REQUEST_382_112_2_17_23_41_46_2263 "RTSP RTSP_CreateRequest record"
#define RTSP_REQUEST_403_112_2_17_23_41_46_2264 "RTSP RTSP_CreateRequest stop"
#define RTSP_REQUEST_424_112_2_17_23_41_46_2265 "RTSP RTSP_CreateRequest self stop"
#define RTSP_REQUEST_445_112_2_17_23_41_46_2266 "RTSP RTSP_CreateRequest set param"
#define RTSP_REQUEST_523_112_2_17_23_41_46_2267 "RTSP RTSP_FreeRequest header_list_handle =%d"
#define RTSP_REQUEST_557_112_2_17_23_41_46_2268 "RTSP RtspCreateOptionsReq input error"
#define RTSP_REQUEST_591_112_2_17_23_41_46_2269 "RTSP RtspCreateOptionsReq no auth param"
#define RTSP_REQUEST_636_112_2_17_23_41_46_2270 "RTSP RtspCreateDescribeReq input error"
#define RTSP_REQUEST_670_112_2_17_23_41_46_2271 "RTSP RtspCreateDescribeReq no auth param"
#define RTSP_REQUEST_715_112_2_17_23_41_46_2272 "RTSP RtspCreateSetupReq input error"
#define RTSP_REQUEST_749_112_2_17_23_41_46_2273 "RTSP RtspCreateSetupReq no auth param"
#define RTSP_REQUEST_770_112_2_17_23_41_46_2274 "RTSP RtspCreatePlayReq input error"
#define RTSP_REQUEST_827_112_2_17_23_41_46_2275 "RTSP RtspCreatePauseReq input error"
#define RTSP_REQUEST_884_112_2_17_23_41_47_2276 "RTSP RtspCreateAnnounceReq input error"
#define RTSP_REQUEST_947_112_2_17_23_41_47_2277 "RTSP RtspCreateRecordReq input error"
#define RTSP_REQUEST_1003_112_2_17_23_41_47_2278 "RTSP RtspCreateTeardownReq input error"
#define RTSP_REQUEST_1058_112_2_17_23_41_47_2279 "RTSP RtspCreateSetparamReq input error"
#define RTSP_REQUEST_1147_112_2_17_23_41_47_2280 "\n rtsp SaveAuthParam error_code=%d"
#define RTSP_REQUEST_1167_112_2_17_23_41_47_2281 "\n rtsp RTSP_UpdateRequest input error\n"
#define RTSP_REQUEST_1191_112_2_17_23_41_47_2282 "\n rtsp RTSP_UpdateRequest \n"
#define RTSP_REQUEST_1322_112_2_17_23_41_47_2283 "\n rtsp RTSP_GetInitInfo proxy host = %s"
#define RTSP_REQUEST_1326_112_2_17_23_41_47_2284 "\n rtsp RTSP_GetInitInfo proxy port =%d"
#define RTSP_RESPONSE_124_112_2_17_23_41_48_2285 "RTSP RTSPRSP_CreateResponse alloc error"
#define RTSP_RESPONSE_144_112_2_17_23_41_48_2286 "RTSP RTSPRSP_FreeResponse input error"
#define RTSP_RESPONSE_209_112_2_17_23_41_48_2287 "RTSP RTSPRSP_ParseResponseHeader input error"
#define RTSP_RESPONSE_330_112_2_17_23_41_49_2288 "RTSP RTSPRSP_ParseContentLengthHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_353_112_2_17_23_41_49_2289 "RTSP RTSPRSP_ParseSetCookieHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_404_112_2_17_23_41_49_2290 "RTSP RTSPRSP_ParseLocationHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_523_112_2_17_23_41_49_2291 "RTSP RTSPRSP_ParseSessionHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_611_112_2_17_23_41_49_2292 "RTSP RTSPRSP_ParseAuthenticationWWWHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_657_112_2_17_23_41_50_2293 "RTSP RTSPRSP_ParseAuthecticationProxyHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_699_112_2_17_23_41_50_2294 "RTSP RTSPRSP_ParsePublicHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_724_112_2_17_23_41_50_2295 "RTSP RTSPRSP_ParseCseqHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_926_112_2_17_23_41_50_2296 "RTSP RtspSetHeaderHandler input error"
#define RTSP_RESPONSE_1298_112_2_17_23_41_51_2297 "RTSP RTSPRSP_ParseCseqHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d"
#define RTSP_RESPONSE_1304_112_2_17_23_41_51_2298 "\n rtsp RTSPRSP_ParseTransportHeader 2 alloc error\n"
#define RTSP_RESPONSE_1317_112_2_17_23_41_51_2299 "\n RTSP RTSPRSP_ParseTransportHeader 3 alloc error\n"
#define RTSP_RESPONSE_1356_112_2_17_23_41_51_2300 "\n RTSP RTSPRSP_ParseTransportHeader SUCCESS\n"
#define RTSP_SERIALIZE_537_112_2_17_23_41_52_2301 "\n rtsp RTSPSERIALIZE_InitReqSigT error"
#define RTSP_SERIALIZE_833_112_2_17_23_41_53_2302 "\n rtsp RTSPSERIALIZE_OptionsReqSigT error"
#define RTSP_SERIALIZE_1545_112_2_17_23_41_54_2303 "\n rtsp RTSPSERIALIZE_OptionsAuthRspSigT error"
#define RTSP_SERIALIZE_1641_112_2_17_23_41_54_2304 "\n rtsp RTSPSERIALIZE_DescribeReqSigT error"
#define RTSP_SERIALIZE_2018_112_2_17_23_41_55_2305 "\n rtsp RTSPSERIALIZE_DescribeAuthRspSigT error"
#define RTSP_SERIALIZE_2114_112_2_17_23_41_55_2306 "\n rtsp RTSPSERIALIZE_PlayReqSigT error"
#define RTSP_SERIALIZE_2369_112_2_17_23_41_56_2307 "\n rtsp RTSPSERIALIZE_PauseReqSigT error"
#define RTSP_SERIALIZE_2624_112_2_17_23_41_56_2308 "\n rtsp RTSPSERIALIZE_SetupReqSigT error"
#define RTSP_SERIALIZE_3113_112_2_17_23_41_57_2309 "\n rtsp RTSPSERIALIZE_DescribeAuthRspSigT error"
#define RTSP_SERIALIZE_3183_112_2_17_23_41_58_2310 "\n rtsp RTSPSERIALIZE_PlayAuthRspSigT error"
#define RTSP_SERIALIZE_3306_112_2_17_23_41_58_2311 "\n rtsp RTSPSERIALIZE_TeardownReqSigT error"
#define RTSP_SERIALIZE_3463_112_2_17_23_41_58_2312 "\n rtsp RTSPSERIALIZE_StopReqSigT error"
#define RTSP_SERIALIZE_3656_112_2_17_23_41_59_2313 "\n rtsp RTSPSERIALIZE_AnnounceReqSigT error"
#define RTSP_SERIALIZE_3977_112_2_17_23_42_0_2314 "\n rtsp RTSPSERIALIZE_PlayReqSigT error"
#define RTSP_SERIALIZE_4233_112_2_17_23_42_0_2315 "\n rtsp RTSPSERIALIZE_SetparamReqSigT error"
#define RTSP_SERIALIZE_4587_112_2_17_23_42_1_2316 "\n rtsp RTSPSERIALIZE_SetparamAuthRspSigT error"
#define RTSP_SETPARAM_HSM_140_112_2_17_23_42_2_2317 "RTSP RTSPHSM_SetparamDispatchSignal dispatch signal"
#define RTSP_SETPARAM_HSM_142_112_2_17_23_42_2_2318 "RTSP RTSPHSM_SetparamDispatchSignal hsm_state =%d"
#define RTSP_SETPARAM_HSM_146_112_2_17_23_42_2_2319 "RTSP RTSPHSM_SetparamDispatchSignal event =%d"
#define RTSP_SETPARAM_HSM_178_112_2_17_23_42_2_2320 "\n rtsp RtspSetparamConstructor param error\n"
#define RTSP_SETPARAM_HSM_195_112_2_17_23_42_2_2321 "\n rtsp RtspSetparamInitial param error\n"
#define RTSP_SETPARAM_HSM_211_112_2_17_23_42_2_2322 "\n rtsp RtspSetparamDestructor param error\n"
#define RTSP_SETPARAM_HSM_238_112_2_17_23_42_2_2323 "\n rtsp rtsp Rtsp_SetparamPending error\n"
#define RTSP_SETPARAM_HSM_257_112_2_17_23_42_2_2324 "RTSP Rtsp_SetparamPendingIdle recv RTSP_HSM_SETPARAM_REQ"
#define RTSP_SETPARAM_HSM_269_112_2_17_23_42_2_2325 "\n rtsp Rtsp_SetparamPendingIdle param error\n"
#define RTSP_SETPARAM_HSM_294_112_2_17_23_42_2_2326 "RTSP Rtsp_SetparamPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND"
#define RTSP_SETPARAM_HSM_300_112_2_17_23_42_2_2327 "RTSP Rtsp_SetparamPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND"
#define RTSP_SETPARAM_HSM_346_112_2_17_23_42_2_2328 "RTSP Rtsp_SetparamPendingSending recv RTSP_HSM_SEND_SUCC_IND"
#define RTSP_SETPARAM_HSM_350_112_2_17_23_42_2_2329 "RTSP Rtsp_SetparamPendingSending recv RTSP_HSM_SEND_FAIL_IND"
#define RTSP_SETPARAM_HSM_373_112_2_17_23_42_2_2330 "\n rtsp Rtsp_SetparamPendingSending param error\n"
#define RTSP_SETPARAM_HSM_421_112_2_17_23_42_2_2331 "RTSP Rtsp_SetparamPendingRecving recv RTSP_HSM_SETPARAM_AUTH_IND"
#define RTSP_SETPARAM_HSM_452_112_2_17_23_42_3_2332 "RTSP Rtsp_SetparamPendingRecving recv RTSP_HSM_SETPARAM_OK_IND"
#define RTSP_SETPARAM_HSM_474_112_2_17_23_42_3_2333 "RTSP Rtsp_SetparamPendingRecving recv RTSP_HSM_SETPARAM_ERROR_IND or RTSP_HSM_TIMEOUT_IND"
#define RTSP_SETPARAM_HSM_498_112_2_17_23_42_3_2334 "RTSP Rtsp_SetparamPendingRecving recv RTSP_HSM_SETPARAM_REDIRECT_IND"
#define RTSP_SETPARAM_HSM_513_112_2_17_23_42_3_2335 "\n rtsp Rtsp_SetparamPendingRecving param error\n"
#define RTSP_SETPARAM_HSM_540_112_2_17_23_42_3_2336 "RTSP Rtsp_SetparamPendingAuthPending recv RTSP_HSM_SETPARAM_AUTH_RSP"
#define RTSP_SETPARAM_HSM_579_112_2_17_23_42_3_2337 "\n rtsp Rtsp_SetparamPendingAuthPending param error\n"
#define RTSP_SETPARAM_HSM_600_112_2_17_23_42_3_2338 "RTSP RtspHandleSetparamPendingAuthRsp input error, %d,%d"
#define RTSP_SETPARAM_HSM_608_112_2_17_23_42_3_2339 "RTSP RtspHandleSetparamPendingAuthRsp host_ptr = PNULL"
#define RTSP_SETPARAM_HSM_615_112_2_17_23_42_3_2340 "RTSP RtspHandleSetparamPendingAuthRsp request_ptr = PNULL"
#define RTSP_SETPARAM_HSM_625_112_2_17_23_42_3_2341 "RTSP RtspHandleSetparamPendingAuthRsp RTSP_SaveAuthParam error"
#define RTSP_SETPARAM_HSM_631_112_2_17_23_42_3_2342 "RTSP RtspHandleSetparamPendingAuthRsp rtsp_context_ptr = PNULL"
#define RTSP_SETPARAM_HSM_638_112_2_17_23_42_3_2343 "RTSP RtspHandleSetparamPendingAuthRsp RTSP_SaveAuthParamToContext error"
#define RTSP_SETPARAM_HSM_647_112_2_17_23_42_3_2344 "RTSP RtspHandleSetparamPendingAuthRsp NETMGR_SetProxyAuthenInfo error"
#define RTSP_SIGNAL_46_112_2_17_23_42_3_2345 "\n rtsp RTSPSIG_SendOptionsCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_80_112_2_17_23_42_3_2346 "\n rtsp RTSPSIG_SendOptionsAuthInd status code = %d,context_id =%d"
#define RTSP_SIGNAL_114_112_2_17_23_42_3_2347 "\n rtsp RTSPSIG_SendSetparamCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_148_112_2_17_23_42_3_2348 "\n rtsp RTSPSIG_SendSetparamAuthInd status code = %d,context_id =%d"
#define RTSP_SIGNAL_180_112_2_17_23_42_3_2349 "\n rtsp RTSPSIG_SendInitCnf task id = %d,context_id =%d"
#define RTSP_SIGNAL_211_112_2_17_23_42_3_2350 "\n rtsp RTSPSIG_SendCloseCnf task id = %d,context_id =%d"
#define RTSP_SIGNAL_244_112_2_17_23_42_4_2351 "\n rtsp RTSPSIG_SendSetparamCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_278_112_2_17_23_42_4_2352 "\n rtsp RTSPSIG_SendSetparamAuthInd status code = %d,context_id =%d"
#define RTSP_SIGNAL_312_112_2_17_23_42_4_2353 "\n rtsp RTSPSIG_SendPlayCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_346_112_2_17_23_42_4_2354 "\n rtsp RTSPSIG_SendPauseCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_380_112_2_17_23_42_4_2355 "\n rtsp RTSPSIG_SendAnnounceCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_414_112_2_17_23_42_4_2356 "\n rtsp RTSPSIG_SendRecordCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_448_112_2_17_23_42_4_2357 "\n rtsp RTSPSIG_SendTeardownCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_485_112_2_17_23_42_4_2358 "\n rtsp RTSPSIG_SendSetupCnf status code = %d,context_id =%d"
#define RTSP_SIGNAL_495_112_2_17_23_42_4_2359 "\n rtsp RTSPSIG_SendSetupCnf"
#define RTSP_SIGNAL_520_112_2_17_23_42_4_2360 "\n rtsp RTSPSIG_SendSetupAuthInd status code = %d,context_id =%d"
#define RTSP_SIGNAL_555_112_2_17_23_42_4_2361 "\n rtsp RTSPSIG_SendPlayAuthInd status code = %d,context_id =%d"
#define RTSP_SIGNAL_564_112_2_17_23_42_4_2362 "RTSPSIG_SendPlayAuthInd SIGNAL =%x,sender =%d"
#define RTSP_SIGNAL_592_112_2_17_23_42_4_2363 "RTSP RTSPSIG_StopReq context_id =%d"
#define RTSP_SIGNAL_599_112_2_17_23_42_4_2364 "\n rtsp RTSPSIG_StopReq task id = %d"
#define RTSP_SIGNAL_616_112_2_17_23_42_4_2365 "\n rtsp RTSPSIG_StopReq error_code = %d"
#define RTSP_SIGNAL_639_112_2_17_23_42_4_2366 "RTSP RTSPSIG_StopSelfReq context_id =%d"
#define RTSP_SIGNAL_646_112_2_17_23_42_5_2367 "\n rtsp RTSPSIG_StopReq task id = %d"
#define RTSP_SIGNAL_663_112_2_17_23_42_5_2368 "\n rtsp RTSPSIG_StopReq error_code = %d"
#define RTSP_SIGNAL_735_112_2_17_23_42_5_2369 "\n rtsp RTSPSIG_SendServerClose task id = %d,context_id =%d"
#define RTSP_SIGNAL_765_112_2_17_23_42_5_2370 "RTSP RTSP_OptionsSelfReq context_id =%d"
#define RTSP_SIGNAL_772_112_2_17_23_42_5_2371 "\n rtsp RTSP_OptionsReq task id = %d"
#define RTSP_SIGNAL_789_112_2_17_23_42_5_2372 "\n rtsp RTSP_OptionsReq error_code = %d"
#define SNTP_192_112_2_17_23_42_6_2373 "[SNTP] Command  SNTP_SIG_GET_TIME_REQ \n"
#define SNTP_196_112_2_17_23_42_6_2374 "[SNTP] Command SOCKET_READ_EVENT_IND \n"
#define SNTP_200_112_2_17_23_42_6_2375 "[SNTP] Command SNTP_SIG_UDP_SOCKET_TIMER_IND \n"
#define SNTP_204_112_2_17_23_42_6_2376 "[SNTP] Command  SNTP_Main default \n"
#define SNTP_264_112_2_17_23_42_6_2377 "[SNTP] Command  SNTP_ReqNetTimeProc ret =%d \n"
#define SNTP_293_112_2_17_23_42_7_2378 "[SNTP] Command  SNTP_ReqSocketReadProc ret =%d \n"
#define SNTP_321_112_2_17_23_42_7_2379 "[SNTP] Command  SNTP_NetSendHandle SNTP_HD_SOCKET_SEND_ERROR \n"
#define SNTP_326_112_2_17_23_42_7_2380 "[SNTP] Command  SNTP_NetSendHandle!ret_val=%d \n"
#define SNTP_371_112_2_17_23_42_7_2381 "[SNTP] Command  SNTP_NetReadHandle SNTP_HD_SOCKET_RECEIVE_ERROR\n"
#define SNTP_375_112_2_17_23_42_7_2382 "[SNTP] Command  SNTP_NetReadHandle ret =%d \n"
#define SNTP_400_112_2_17_23_42_7_2383 "[SNTP] Command  SNTP_NetTimeReqDnsHandle SNTP_HD_DNS_ERROR \n"
#define SNTP_413_112_2_17_23_42_7_2384 "[SNTP] Command  SNTP_NetTimeReqDnsHandle SNTP_HD_SOCKET_ERROR \n"
#define SNTP_419_112_2_17_23_42_7_2385 "[SNTP] Command  SNTP_NetTimeReqDnsHandle ret =%d \n"
#define SNTP_444_112_2_17_23_42_7_2386 "[SNTP] Command  SNTP_NetTimeReqIPHandle SNTP_HD_SOCKET_ERROR \n"
#define SNTP_450_112_2_17_23_42_7_2387 "[SNTP] Command  SNTP_NetTimeReqIPHandle ret =%d \n"
#define SNTP_498_112_2_17_23_42_7_2388 "[SNTP] SNTP_NetTimeReqCNF!ret_val =%d\n"
#define SNTP_558_112_2_17_23_42_7_2389 "[SNTP] SNTP_SocketTimerOutProc!ret_val =%d\n"
#define SNTP_API_71_112_2_17_23_42_7_2390 "[SNTP] Command is: SNTP_GetNTPTime ret=%d\n"
#define SNTP_UNIT_49_112_2_17_23_42_9_2391 "[SNTP] SNTP_SockCreat sock ERROR"
#define SNTP_UNIT_60_112_2_17_23_42_9_2392 "[SNTP] SNTP SockCreat  is fail"
#define SNTP_UNIT_66_112_2_17_23_42_9_2393 "[SNTP]SNTP SockCreat setopt ERROR"
#define SNTP_UNIT_97_112_2_17_23_42_9_2394 "[SNTP] SNTP_SendtoData  is error"
#define SNTP_UNIT_124_112_2_17_23_42_9_2395 "[SNTP] SNTP RevData fd is error"
#define SNTP_UNIT_180_112_2_17_23_42_9_2396 "[SNTP] SNTP_GetHostByName!ret_val =%d\n"
#define SNTP_UNIT_202_112_2_17_23_42_9_2397 "[SNTP] SNTP_GetHostByNameAsync is ERROR"
#define SNTP_UNIT_290_112_2_17_23_42_10_2398 "[SNTP] SNTP_FindNoteByIndex!ret_val =%d\n"
#define SNTP_UNIT_313_112_2_17_23_42_10_2399 "[SNTP] SNTP_UpdateNodeByNodeHand!ret_val =%d\n"
#define SNTP_UNIT_344_112_2_17_23_42_10_2400 "[SNTP] SNTP_DeleteNoteByIndex!ret_val =%d\n"
#define SNTP_UNIT_368_112_2_17_23_42_10_2401 "[SNTP] SNTP_SerializeNettimeReqSigT!\n"
#define MD2_93_112_2_17_23_42_14_2402 "ssl matrixMd2Init invalid input"
#define MD2_108_112_2_17_23_42_14_2403 "ssl matrixMd2Update invalid input"
#define MD2_135_112_2_17_23_42_14_2404 "ssl matrixMd2Final invalid input"
#define RSA_79_112_2_17_23_42_23_2405 "ssl_rsa_crypt error: mp_init_multi\n"
#define RSA_84_112_2_17_23_42_23_2406 "ssl_rsa_crypt error: mp_read_unsigned_bin\n"
#define RSA_97_112_2_17_23_42_23_2407 "decrypt error: mp_exptmod dP, p\n"
#define RSA_102_112_2_17_23_42_23_2408 "decrypt error: mp_exptmod dQ, q\n"
#define RSA_106_112_2_17_23_42_23_2409 "decrypt error: sub tmpb, tmp\n"
#define RSA_110_112_2_17_23_42_23_2410 "decrypt error: mp_mulmod qP, p\n"
#define RSA_114_112_2_17_23_42_23_2411 "decrypt error: mp_mul q \n"
#define RSA_118_112_2_17_23_42_23_2412 "decrypt error: mp_add tmp \n"
#define RSA_123_112_2_17_23_42_23_2413 "ssl_rsa_crypt error: mp_exptmod\n"
#define RSA_129_112_2_17_23_42_23_2414 "ssl_rsa_crypt error: mp_exptmod\n"
#define RSA_133_112_2_17_23_42_23_2415 "ssl_rsa_crypt error: invalid type param\n"
#define RSA_141_112_2_17_23_42_23_2416 "ssl_rsa_crypt error: mp_unsigned_bin_size\n"
#define RSA_157_112_2_17_23_42_23_2417 "ssl_rsa_crypt error: mp_to_unsigned_bin\n"
#define RSA_191_112_2_17_23_42_23_2418 "RSA encryption data too large: %d\n"
#define RSA_205_112_2_17_23_42_23_2419 "Error gathering RSA pad entropy\n"
#define ASN1_53_112_2_17_23_42_26_2420 "Unsupported: ASN indefinite length\n"
#define ASN1_93_112_2_17_23_42_26_2421 "ASN getBig failed\n"
#define ASN1_99_112_2_17_23_42_26_2422 "ASN getBig failed\n"
#define ASN1_120_112_2_17_23_42_26_2423 "ASN getSerialNumber failed\n"
#define ASN1_126_112_2_17_23_42_26_2424 "ASN getSerialNumber failed\n"
#define ASN1_240_112_2_17_23_42_26_2425 "Unsupported algorithm identifier\n"
#define ASN1_396_112_2_17_23_42_26_2426 "ASN getInteger failed\n"
#define ASN1_404_112_2_17_23_42_26_2427 "ASN getInteger failed\n"
#define RSAPKI_74_112_2_17_23_42_27_2428 "Osdep open failure\n"
#define RSAPKI_145_112_2_17_23_42_27_2429 "ssl filename is null"
#define RSAPKI_160_112_2_17_23_42_27_2430 "SFS_CreateFile() error\n"
#define RSAPKI_168_112_2_17_23_42_27_2431 "SFS_GetFileSize() error\n"
#define RSAPKI_182_112_2_17_23_42_27_2432 "SFS_ReadFile() error\n"
#define RSAPKI_197_112_2_17_23_42_27_2433 "SFS_CloseFile() error\n"
#define RSAPKI_240_112_2_17_23_42_27_2434 "Error parsing private key buffer\n"
#define RSAPKI_253_112_2_17_23_42_27_2435 "No password given for encrypted private key\n"
#define RSAPKI_258_112_2_17_23_42_27_2436 "Unrecognized private key file encoding\n"
#define RSAPKI_266_112_2_17_23_42_27_2437 "Invalid private key file salt\n"
#define RSAPKI_276_112_2_17_23_42_27_2438 "3DES has been disabled for private key decrypt\n"
#define RSAPKI_283_112_2_17_23_42_27_2439 "Error parsing private key buffer\n"
#define RSAPKI_299_112_2_17_23_42_27_2440 "Unable to base64 decode private key\n"
#define RSAPKI_377_112_2_17_23_42_27_2441 "Unable to ASN parse private key.\n"
#define RSAPKI_451_112_2_17_23_42_28_2442 "ASN sequence parse error\n"
#define RSAPKI_465_112_2_17_23_42_28_2443 "ASN key extract parse error\n"
#define RSAPKI_584_112_2_17_23_42_28_2444 "Parsing untested DN attrib type\n"
#define X509_101_112_2_17_23_42_28_2445 "Error: Calling matrixRsaReadKeys against a library\n"
#define X509_102_112_2_17_23_42_28_2446 "built without USE_FILE_SYSTEM defined\n"
#define X509_202_112_2_17_23_42_29_2447 "Error reading private key file: %s\n"
#define X509_209_112_2_17_23_42_29_2448 "Error parsing private key file: %s\n"
#define X509_230_112_2_17_23_42_29_2449 "Error reading CA cert file %s\n"
#define X509_239_112_2_17_23_42_29_2450 "Error parsing CA cert %s\n"
#define X509_258_112_2_17_23_42_29_2451 "No valid CA certs in %s\n"
#define X509_305_112_2_17_23_42_29_2452 "Exceeded maximum cert chain length of %dn"
#define X509_310_112_2_17_23_42_29_2453 "Couldn't open file %s\n"
#define X509_330_112_2_17_23_42_29_2454 "Unable to base64 decode certificate\n"
#define X509_381_112_2_17_23_42_29_2455 "Unable to read certificate file %s\n"
#define X509_417_112_2_17_23_42_29_2456 "Error reading cert file %s\n"
#define X509_511_112_2_17_23_42_29_2457 "Error reading private key mem\n"
#define X509_521_112_2_17_23_42_29_2458 "Error parsing CA cert\n"
#define X509_608_112_2_17_23_42_30_2459 "Initial cert parse error\n"
#define X509_628_112_2_17_23_42_30_2460 "ASN sequence parse error\n"
#define X509_637_112_2_17_23_42_30_2461 "ASN version parse error\n"
#define X509_642_112_2_17_23_42_30_2462 "Warning: non-v3 certificate version: %dn"
#define X509_648_112_2_17_23_42_30_2463 "ASN serial number parse error\n"
#define X509_730_112_2_17_23_42_30_2464 "Parse error: mismatched signature type\n"
#define X509_969_112_2_17_23_42_30_2465 "Error parsing cert extension\n"
#define X509_979_112_2_17_23_42_30_2466 "Expecting OCTET STRING in ext parse\n"
#define X509_1082_112_2_17_23_42_30_2467 "Unsupported subjectAltName type\n"
#define X509_1132_112_2_17_23_42_31_2468 "Error auth key-id name type: %dn"
#define X509_1215_112_2_17_23_42_31_2469 "Unknown critical ext encountered\n"
#define X509_1286_112_2_17_23_42_31_2470 "Warning:  No CA to validate cert with\n"
#define X509_1287_112_2_17_23_42_31_2471 "\tPerforming self-signed CA test\n"
#define X509_1332_112_2_17_23_42_31_2472 "Unsupported signature algorithm\n"
#define X509_1337_112_2_17_23_42_31_2473 "ssl matrixX509ValidateCertInternal wrong sigLen"
#define CIPHERSUITE_376_112_2_17_23_42_32_2474 "Error: output buffer too small for NULL encrypt\n"
#define CIPHERSUITE_387_112_2_17_23_42_32_2475 "Error: output buffer too small for NULL decrypt\n"
#define SSL_ALERT_60_112_2_17_23_42_32_2476 "SSL_AlertSend : invalid input"
#define SSL_ALERT_67_112_2_17_23_42_32_2477 "SSL_AlertSend : PDU create fail"
#define SSL_ALERT_78_112_2_17_23_42_32_2478 "SSL_AlertSend :pdu pack fail"
#define SSL_ALERT_85_112_2_17_23_42_32_2479 "SSL_AlertSend :payload pack fail"
#define SSL_ALERT_122_112_2_17_23_42_32_2480 "SSL_AlertEventHandle : invalid input"
#define SSL_ALERT_132_112_2_17_23_42_32_2481 "SSL_ALERTEVENTHANDLE :error event create fail"
#define SSL_ALERT_145_112_2_17_23_42_33_2482 "SSL_ALERTEVENTHANDLE :warning event create fail"
#define SSL_ALGOPROCESS_286_112_2_17_23_42_35_2483 "SSLALGOPROC_CalculateSslPrf: encounter problem\n"
#define SSL_ALGOPROCESS_723_112_2_17_23_42_36_2484 "SSL_CALC: alloc space for tempout_ptr fail"
#define SSL_ALGOPROCESS_818_112_2_17_23_42_36_2485 "SSL_CALC: alloc temp_output_ptr fail"
#define SSL_ALGOPROCESS_1585_112_2_17_23_42_37_2486 "SSL_DECRYPT: not space for decrypt data"
#define SSL_ALGOPROCESS_1635_112_2_17_23_42_37_2487 "SSL_DECRYPT: SSL_OctstrGetChar error"
#define SSL_ALGOPROCESS_1640_112_2_17_23_42_37_2488 "SSL_DECRYPT: Get padding length error"
#define SSL_ALGOPROCESS_1648_112_2_17_23_42_37_2489 "SSL_DECRYPT: SSL_OctstrDelete error"
#define SSL_ALGOPROCESS_1728_112_2_17_23_42_38_2490 "SSL_ENCRYPT: tls calculate client mac fail"
#define SSL_ALGOPROCESS_1827_112_2_17_23_42_38_2491 "SSL_ENCRYPT: NOT SPACE FOR THE ENCRYPTED DATA"
#define SSL_ALGOPROCESS_1871_112_2_17_23_42_38_2492 "SSL_DataEncrypt:event create fail"
#define SSL_ALGOPROCESS_1902_112_2_17_23_42_38_2493 "ssl_decryptpdulist: invalid input"
#define SSL_ALGOPROCESS_1918_112_2_17_23_42_38_2494 "SSL_DecryptPDU: decrypt pdu fail"
#define SSL_ALGOPROCESS_2003_112_2_17_23_42_38_2495 "SSL_VERIFYMAC: tlsCALCULATE SERVER MAC FAIL"
#define SSL_ALGOPROCESS_2358_112_2_17_23_42_39_2496 "SSL_ALGOPROC: calculate tls prf fail"
#define SSL_ALGOPROCESS_2389_112_2_17_23_42_39_2497 "SSL_GetRsaPublickeyincert: invalid input"
#define SSL_API_121_112_2_17_23_42_39_2498 "SSL_AsyncRegCallback:the handle not exist\n"
#define SSL_API_148_112_2_17_23_42_40_2499 "SSL_Init:can't support half handshake\n "
#define SSL_API_189_112_2_17_23_42_40_2500 "SSL_Create : input is invalid\n"
#define SSL_API_196_112_2_17_23_42_40_2501 "ssl_create: create machine fail!\n"
#define SSL_API_202_112_2_17_23_42_40_2502 "SSL_Create 0x%x"
#define SSL_API_222_112_2_17_23_42_40_2503 "SSL_CLOSE 0x%x"
#define SSL_API_227_112_2_17_23_42_40_2504 "SSL_CLOSE: ssl_handle is invalid\n"
#define SSL_API_234_112_2_17_23_42_40_2505 "SSL_Close : handle not find in table\n"
#define SSL_API_258_112_2_17_23_42_40_2506 "SSL_ProtocolChoose: the handler is invalid\n"
#define SSL_API_290_112_2_17_23_42_40_2507 "SSL_Handshake: the input is invalid\n"
#define SSL_API_297_112_2_17_23_42_40_2508 "SSL_Handshake: the handler not registered\n"
#define SSL_API_325_112_2_17_23_42_40_2509 "SSL_Handshake: client hello event create fail\n"
#define SSL_API_354_112_2_17_23_42_40_2510 "SSL_DecryptPasser: len=%d"
#define SSL_API_363_112_2_17_23_42_40_2511 "SSL_DecryptPasser: the input is invalid\n"
#define SSL_API_369_112_2_17_23_42_40_2512 "SSL_DecryptPasser: handler not registered\n"
#define SSL_API_450_112_2_17_23_42_40_2513 "SSL_DecryptPasser : error code"
#define SSL_API_473_112_2_17_23_42_40_2514 "SSL_EncryptPasser: len=%d"
#define SSL_API_476_112_2_17_23_42_40_2515 "SSL_EncryptPasser: input invalid\n"
#define SSL_API_482_112_2_17_23_42_40_2516 "SSL_EncryptPasser: handler not registered\n"
#define SSL_API_536_112_2_17_23_42_40_2517 "SSL_EncryptPasser: error code\n"
#define SSL_API_561_112_2_17_23_42_40_2518 "SSL_AsyncMessageProc: input invalid\n"
#define SSL_API_567_112_2_17_23_42_40_2519 "SSL_AsyncMessageProc: handler not registered\n"
#define SSL_API_605_112_2_17_23_42_40_2520 "SSL_AsyncMessageProc: no memeory\n"
#define SSL_API_614_112_2_17_23_42_40_2521 "SSL_AsyncMessageProc: file open error\n"
#define SSL_API_631_112_2_17_23_42_41_2522 "SSL_AsyncMessageProc: no memeory_2\n"
#define SSL_API_640_112_2_17_23_42_41_2523 "SSL_AsyncMessageProc: file open error_2\n"
#define SSL_API_660_112_2_17_23_42_41_2524 "SSL WILL IGNORE IT"
#define SSL_API_685_112_2_17_23_42_41_2525 "SSL_EncryptPasserFile: input invalid"
#define SSL_API_691_112_2_17_23_42_41_2526 "SSL_EncryptPasserFile: handler not registered"
#define SSL_API_699_112_2_17_23_42_41_2527 "SSL_ENCRYPTPASSERFILE :SFS_CreateFile() error\n"
#define SSL_API_706_112_2_17_23_42_41_2528 "SSL_ENCRYPTPASSERFILE:SFS_GetFileSize() error\n"
#define SSL_API_722_112_2_17_23_42_41_2529 "SSL_ENCRYPTFILE: can not allocate enough space"
#define SSL_API_739_112_2_17_23_42_41_2530 "SSL_ENCRYPTPASSERFILE:SFS_ReadFile() error\n"
#define SSL_API_761_112_2_17_23_42_41_2531 "SSL_ENCRYPTPASSERFILE:SFS_ReadFile() error\n"
#define SSL_API_785_112_2_17_23_42_41_2532 "SSL_ENCRYPTPASSERFILE:SFS_ReadFile() error\n"
#define SSL_API_797_112_2_17_23_42_41_2533 "SSL_ENCRYPTPASSERFILE:error code\n"
#define SSL_API_828_112_2_17_23_42_41_2534 "SSL_UserCnfCert enter!"
#define SSL_API_831_112_2_17_23_42_41_2535 "SSL_UserCnfCert: input invalid\n"
#define SSL_API_837_112_2_17_23_42_41_2536 "SSL_UserCnfCert: handler not registered\n"
#define SSL_API_843_112_2_17_23_42_41_2537 "SSL_UserCnfCert user trust the cert"
#define SSL_API_856_112_2_17_23_42_41_2538 "SSL_UserCnf: event create fail\n"
#define SSL_API_863_112_2_17_23_42_41_2539 "SSL_UserCnfCert user do not trust cert"
#define SSL_API_873_112_2_17_23_42_41_2540 "SSL_UserCnf: error_code \n"
#define SSL_API_892_112_2_17_23_42_41_2541 "SSL_AddMachineToTable: ssl_handle is invalid"
#define SSL_API_914_112_2_17_23_42_41_2542 "SSL_ADDMachinToTable: we need larger number macro\n"
#define SSL_API_928_112_2_17_23_42_41_2543 "SSL_DeleteMachineFromTable: input is invalid"
#define SSL_API_950_112_2_17_23_42_41_2544 "SSL_DeleteMachineFromTable:handle not found\n"
#define SSL_API_986_112_2_17_23_42_41_2545 "SSL_GetMachineInTable: input is invalid\n"
#define SSL_API_1023_112_2_17_23_42_41_2546 "SSL_GetCurrentCertInfo: cert is invalid\n"
#define SSL_API_1029_112_2_17_23_42_41_2547 "SSL_GetCurrentCertInfo: handler not registered\n"
#define SSL_API_1037_112_2_17_23_42_42_2548 "SSL_GETCURRENTINFO: not after ptr alloc fail\n"
#define SSL_API_1048_112_2_17_23_42_42_2549 "SSL_GETCURRENTINFO: not before ptr alloc fail\n"
#define SSL_API_1059_112_2_17_23_42_42_2550 "SSL_GETCURRENTINFO: serialnumber ptr alloc fail\n"
#define SSL_API_1075_112_2_17_23_42_42_2551 "SSL_GETCURRENTINFO: subjectid ptr alloc fail\n"
#define SSL_API_1086_112_2_17_23_42_42_2552 "SSL_GETCURRENTINFO: unique userid ptr alloc fail"
#define SSL_API_1100_112_2_17_23_42_42_2553 "SSL_GETCURRENTINFO: issuser name ptr alloc fail\n"
#define SSL_API_1111_112_2_17_23_42_42_2554 "SSL_GETCURRENTINFO: issuer country ptr alloc fail\n"
#define SSL_API_1122_112_2_17_23_42_42_2555 "SSL_GETCURRENTINFO: issuer hash ptr alloc fail\n"
#define SSL_API_1133_112_2_17_23_42_42_2556 "SSL_GETCURRENTINFO:issuer locality ptr alloc fail\n"
#define SSL_API_1144_112_2_17_23_42_42_2557 "SSL_GETCURRENTINFO: issuer organization ptr alloc fail\n"
#define SSL_API_1155_112_2_17_23_42_42_2558 "SSL_GETCURRENTINFO: issuer orgunit alloc fail\n"
#define SSL_API_1166_112_2_17_23_42_42_2559 "SSL_GETCURRENTINFO: issuer state ptr alloc fail\n"
#define SSL_API_1177_112_2_17_23_42_42_2560 "SSL_GETCURRENTINFO: issuer commonname alloc fail\n"
#define SSL_API_1188_112_2_17_23_42_42_2561 "SSL_GETCURRENTINFO:subject country ptr alloc fail\n"
#define SSL_API_1199_112_2_17_23_42_42_2562 "SSL_GETCURRENTINFO:subject hash alloc fail\n"
#define SSL_API_1210_112_2_17_23_42_42_2563 "SSL_GETCURRENTINFO: subject locality ptr alloc fail\n"
#define SSL_API_1221_112_2_17_23_42_42_2564 "SSL_GETCURRENTINFO: subject orgnanization ptr alloc fail\n"
#define SSL_API_1232_112_2_17_23_42_42_2565 "SSL_GETCURRENTINFO: subject orgunit ptr alloc fail\n"
#define SSL_API_1243_112_2_17_23_42_42_2566 "SSL_GETCURRENTINFO:subject state ptr alloc fail\n"
#define SSL_API_1362_112_2_17_23_42_42_2567 "SSL_FreeCurrentCertInfo: cert is invalid"
#define SSL_API_1506_112_2_17_23_42_42_2568 "SSL_SessionSave: session_ptr is invalid"
#define SSL_API_1560_112_2_17_23_42_43_2569 "SSL_LoadSession: list_ptr is invalid"
#define SSL_API_1617_112_2_17_23_42_43_2570 "SSL_CertHashSaveFile: input is invalid"
#define SSL_API_1641_112_2_17_23_42_43_2571 "SSL_CertHashSaveFile :SFS_CreateFile() error\n"
#define SSL_API_1654_112_2_17_23_42_43_2572 "SSL_CertHashSaveFile :SFS_GetFileSize() error\n"
#define SSL_API_1665_112_2_17_23_42_43_2573 "SSL_CertHashSaveFile :error_code\n"
#define SSL_API_1695_112_2_17_23_42_43_2574 "SSL_LoadCertHashFromFile: input is invalid\n"
#define SSL_API_1703_112_2_17_23_42_43_2575 "SSL_LoadCertHashFromFile :SFS_CreateFile() error\n"
#define SSL_API_1710_112_2_17_23_42_43_2576 "SSL_LoadCertHashFromFile :SFS_GetFileSize() error\n"
#define SSL_API_1723_112_2_17_23_42_43_2577 "SSL_LoadCertHashFromFile :SFS_ReadFile() error\n"
#define SSL_API_1730_112_2_17_23_42_43_2578 "SSL_LoadCertHashFromFile :create hash_ptr fail error\n"
#define SSL_API_1741_112_2_17_23_42_43_2579 "SSL_LoadCertHashFromFile :error_code\n"
#define SSL_API_1775_112_2_17_23_42_43_2580 "SSL_SaveDataToFile SFS_CreateFile error!"
#define SSL_CERT_100_112_2_17_23_42_43_2581 "SSL_LoadCertificate: input invalid"
#define SSL_CERT_213_112_2_17_23_42_44_2582 "SSL_IsCertUserConfirm: input invalid"
#define SSL_EVENT_421_112_2_17_23_42_45_2583 "SSL_EventCreate: malloc fail"
#define SSL_EVENT_471_112_2_17_23_42_45_2584 "SSL_EVENTHANDLE: event type=%d"
#define SSL_EVENT_476_112_2_17_23_42_45_2585 "SSL_EVENTHANDLE: EventAppendToQueue"
#define SSL_EVENT_489_112_2_17_23_42_45_2586 "SSL_EVENTHANDLE: DECRYPT FAIL HERE NEED ALERT!!!"
#define SSL_EVENT_502_112_2_17_23_42_45_2587 "SSL_EVENTHANDLE: SSLEVENT_EventHandle condition=%d"
#define SSL_EVENT_526_112_2_17_23_42_45_2588 "SSL_EVENTHANDLE: state jump loop"
#define SSL_HANDSHAKE_202_112_2_17_23_42_46_2589 "SSL_HANDRECEIVED DATA :invalid input\n"
#define SSL_HANDSHAKE_220_112_2_17_23_42_46_2590 "SSL_HANDRECEIVED DATA :pdu unpack fail\n"
#define SSL_HANDSHAKE_245_112_2_17_23_42_46_2591 "SSL_HANDRECEIVED DATA :error_code\n"
#define SSL_HANDSHAKE_273_112_2_17_23_42_46_2592 "SSL_EncryptUserDataToSend: invalid input\n"
#define SSL_HANDSHAKE_286_112_2_17_23_42_46_2593 "SSL_EncryptUserDataToSend: PDU create fail\n"
#define SSL_HANDSHAKE_304_112_2_17_23_42_46_2594 "SSL_EncryptUserDataToSend: PDU add fail\n"
#define SSL_HANDSHAKE_428_112_2_17_23_42_46_2595 "SSLHAND_SendClientHello :invalid input\n"
#define SSL_HANDSHAKE_492_112_2_17_23_42_46_2596 "SSLHAND_SendClientHello : encounters error\n"
#define SSL_HANDSHAKE_526_112_2_17_23_42_46_2597 "SSLHAND_HandleServerHello enter"
#define SSL_HANDSHAKE_600_112_2_17_23_42_47_2598 "SSLHAND_HandleServerHello : after SSLEVENT_EventHandle"
#define SSL_HANDSHAKE_612_112_2_17_23_42_47_2599 "SSLHAND_HandleServerHello : after SSL_HANDSHAKE_FULL"
#define SSL_HANDSHAKE_617_112_2_17_23_42_47_2600 "SSLHAND_HandleServerHello end"
#define SSL_HANDSHAKE_621_112_2_17_23_42_47_2601 "SSLHAND_HandleServerHello : encounters error\n"
#define SSL_HANDSHAKE_674_112_2_17_23_42_47_2602 "SSLHAND_HandleServerCert : encounters error\n"
#define SSL_HANDSHAKE_702_112_2_17_23_42_47_2603 "SSLHAND_HandleServerHelloDone"
#define SSL_HANDSHAKE_726_112_2_17_23_42_47_2604 "SSLHAND_HandleServerHelloDone SSL_CERT_NOTCONFIRMED"
#define SSL_HANDSHAKE_737_112_2_17_23_42_47_2605 "SSLHAND_HandleServerHelloDone SSLSTATE_SERVERHELLODONE"
#define SSL_HANDSHAKE_747_112_2_17_23_42_47_2606 "SSLHAND_HandleServerHelloDone end"
#define SSL_HANDSHAKE_751_112_2_17_23_42_47_2607 "SSLHAND_HandleServerHelloDone : encounters error\n"
#define SSL_HANDSHAKE_851_112_2_17_23_42_47_2608 "SSLHAND_HandleClientExchange"
#define SSL_HANDSHAKE_907_112_2_17_23_42_47_2609 "SSLHAND_HandleClientExchange : encounters error\n"
#define SSL_HANDSHAKE_989_112_2_17_23_42_48_2610 "SSLHAND_HandleServerFinished : encounters error\n"
#define SSL_HANDSHAKE_1022_112_2_17_23_42_48_2611 "SendChangeCipherAndFinshedMessage"
#define SSL_HANDSHAKE_1065_112_2_17_23_42_48_2612 "SendChangeCipherAndFinshedMessage end"
#define SSL_HANDSHAKE_1069_112_2_17_23_42_48_2613 "SendChangeCipherAndFinshedMessage: encounters error\n"
#define SSL_HANDSHAKE_1143_112_2_17_23_42_48_2614 "SSL_HANDSHAKE,master key generate fail\n"
#define SSL_HANDSHAKE_1182_112_2_17_23_42_48_2615 "SSLHAND_GenerateMasterKey: encounters error\n"
#define SSL_MACHINE_116_112_2_17_23_42_49_2616 ""
#define SSL_MACHINE_124_112_2_17_23_42_49_2617 ""
#define SSL_MACHINE_317_112_2_17_23_42_49_2618 "SSLMACHINE_Destroy 0x%x"
#define SSL_PDU_253_112_2_17_23_42_50_2619 "SSL_PDUPACK: encrypt fail"
#define SSL_PDU_657_112_2_17_23_42_51_2620 "SSL_PduAdd: invalid input"
#define SSL_PDU_671_112_2_17_23_42_51_2621 "SSL_PduAdd: pdu pack fail"
#define SSL_PDU_681_112_2_17_23_42_51_2622 "SSL_PduAdd: payload pack fail"
#define SSL_PDU_690_112_2_17_23_42_51_2623 "SSL_PduAdd: insert pdu fail"
#define SSL_PDU_720_112_2_17_23_42_51_2624 "SSL_PayloadPack: invalid input"
#define SSL_PDU_795_112_2_17_23_42_52_2625 "SSL_PayloadPack: string create fail"
#define SSL_PDU_885_112_2_17_23_42_52_2626 "SSLPDU_UnpackPDU error\n"
#define SSL_PDU_1134_112_2_17_23_42_52_2627 "SSL_packrsasecret: invalid input"
#define SSL_PDU_1412_112_2_17_23_42_53_2628 "SSL_DATAGRAMUNPACK: event create fail"
#define SSL_PDUSUPPORT_129_112_2_17_23_42_53_2629 "SSL_version:invalid input"
#define SSL_PDUSUPPORT_605_112_2_17_23_42_54_2630 "ssl:GetRandom = %s"
#define SSL_PDUSUPPORT_755_112_2_17_23_42_55_2631 "SSL_CREATECIPHERSUITE LIST FAIL"
#define SSL_PDUSUPPORT_847_112_2_17_23_42_55_2632 "SSL_ENCRYPTSECRETE: rsa fail"
#define STRM_API_70_112_2_17_23_42_56_2633 "#STRM STRM_Init!"
#define STRM_API_74_112_2_17_23_42_56_2634 "#STRM STRM_Init invalid input!"
#define STRM_API_80_112_2_17_23_42_56_2635 "#STRM STRM_Init STRMHSM_CreatHsm fail!"
#define STRM_API_87_112_2_17_23_42_56_2636 "#STRM STRM_Init sig_ptr alloc fail!"
#define STRM_API_94_112_2_17_23_42_56_2637 "#STRM STRM_Init STRMHSM ThreadId is invalid!"
#define STRM_API_130_112_2_17_23_42_56_2638 "#STRM STRM_Play strm_handle=0x%x,palyer_handle=0x%x!"
#define STRM_API_134_112_2_17_23_42_56_2639 "#STRM STRM_Play invalid input!"
#define STRM_API_140_112_2_17_23_42_56_2640 "#STRM STRM_Play creat signal fail!"
#define STRM_API_159_112_2_17_23_42_56_2641 "#STRM STRM_Pause strm_handle=0x%x!"
#define STRM_API_163_112_2_17_23_42_56_2642 "#STRM STRM_Pause invalid input!"
#define STRM_API_170_112_2_17_23_42_56_2643 "#STRM STRM_Pause creat signal fail!"
#define STRM_API_190_112_2_17_23_42_56_2644 "#STRM STRM_Seek strm_handle=0x%x!"
#define STRM_API_194_112_2_17_23_42_56_2645 "#STRM STRM_Seek invalid input!"
#define STRM_API_200_112_2_17_23_42_56_2646 "#STRM STRM_Resume creat signal fail!"
#define STRM_API_220_112_2_17_23_42_56_2647 "#STRM STRM_Resume strm_handle=0x%x!"
#define STRM_API_224_112_2_17_23_42_56_2648 "#STRM STRM_Resume invalid input!"
#define STRM_API_230_112_2_17_23_42_56_2649 "#STRM STRM_Resume creat signal fail!"
#define STRM_API_250_112_2_17_23_42_56_2650 "#STRM STRM_Stop strm_handle=0x%x!"
#define STRM_API_254_112_2_17_23_42_56_2651 "#STRM STRM_Stop invalid input!"
#define STRM_API_261_112_2_17_23_42_56_2652 "#STRM STRM_Stop creat signal fail!"
#define STRM_API_280_112_2_17_23_42_56_2653 "#STRM STRM_Close strm_handle=0x%x!"
#define STRM_API_284_112_2_17_23_42_56_2654 "#STRM STRM_Close invalid input!"
#define STRM_API_291_112_2_17_23_42_56_2655 "#STRM STRM_Close creat signal fail!"
#define STRM_API_312_112_2_17_23_42_57_2656 "#STRM SendAuthRsp strm_handle=0x%x!"
#define STRM_API_316_112_2_17_23_42_57_2657 "#STRM SendAuthRsp invalid input!"
#define STRM_API_321_112_2_17_23_42_57_2658 "#STRM SendAuthRsp username or psw cannot be NULL!"
#define STRM_API_334_112_2_17_23_42_57_2659 "#STRM SendAuthRsp serialize err!, msg_id:%x"
#define STRM_API_350_112_2_17_23_42_57_2660 "#STRM STRM_DescribeAuthRsp strm_handle=0x%x!"
#define STRM_API_363_112_2_17_23_42_57_2661 "#STRM STRM_SetupAuthRsp strm_handle=0x%x!"
#define STRM_API_375_112_2_17_23_42_57_2662 "#STRM STRM_PlayAuthRsp strm_handle=0x%x!"
#define STRM_API_393_112_2_17_23_42_57_2663 "#STRM STRM_SendInitCnf strm_handle=0x%x!"
#define STRM_API_396_112_2_17_23_42_57_2664 "#STRM STRM_SendInitCnf invalid input!"
#define STRM_API_407_112_2_17_23_42_57_2665 "#STRM STRM_SendInitCnf serialize FAIL!"
#define STRM_API_427_112_2_17_23_42_57_2666 "#STRM STRM_SendAuthInd strm_handle=0x%x!"
#define STRM_API_431_112_2_17_23_42_57_2667 "#STRM STRM_SendAuthInd invalid input!"
#define STRM_API_437_112_2_17_23_42_57_2668 "#STRM STRM_SendAuthInd creat signal FAIL!"
#define STRM_API_455_112_2_17_23_42_57_2669 "#STRM STRM_SendPlayCnf strm_handle=0x%x!"
#define STRM_API_459_112_2_17_23_42_57_2670 "#STRM STRM_SendPlayCnf invalid input!"
#define STRM_API_466_112_2_17_23_42_57_2671 "#STRM STRM_SendPlayCnf creat signal FAIL!"
#define STRM_API_484_112_2_17_23_42_57_2672 "#STRM STRM_SendStopCnf strm_handle=0x%x dest_id=0x%x!"
#define STRM_API_488_112_2_17_23_42_57_2673 "#STRM STRM_SendStopCnf invalid input!"
#define STRM_API_495_112_2_17_23_42_57_2674 "#STRM STRM_SendStopCnf creat signal FAIL!"
#define STRM_API_514_112_2_17_23_42_57_2675 "#STRM STRM_SendBufferingInd strm_handle=0x%x!"
#define STRM_API_518_112_2_17_23_42_57_2676 "#STRM STRM_SendBufferingInd invalid input!"
#define STRM_API_524_112_2_17_23_42_57_2677 "#STRM STRM_SendBufferingInd creat signal FAIL!"
#define STRM_API_543_112_2_17_23_42_57_2678 "#STRM STRM_SendGoodbyeInd strm_handle=0x%x!"
#define STRM_API_547_112_2_17_23_42_57_2679 "#STRM STRM_SendGoodbyeInd invalid input!"
#define STRM_API_553_112_2_17_23_42_57_2680 "#STRM STRM_SendGoodbyeInd creat signal FAIL!"
#define STRM_API_572_112_2_17_23_42_57_2681 "#STRM STRM_SendBufferEndInd strm_handle=0x%x!"
#define STRM_API_576_112_2_17_23_42_57_2682 "#STRM STRM_SendBufferEndInd invalid input!"
#define STRM_API_582_112_2_17_23_42_57_2683 "#STRM STRM_SendBufferEndInd creat signal FAIL!"
#define STRM_API_603_112_2_17_23_42_57_2684 "#STRM STRM_BufProInd strm_handle=0x%x!"
#define STRM_API_607_112_2_17_23_42_57_2685 "#STRM STRM_BufProInd invalid input!"
#define STRM_API_613_112_2_17_23_42_57_2686 "#STRM STRM_BufProInd creat signal FAIL!"
#define STRM_API_634_112_2_17_23_42_57_2687 "#STRM STRM_SendDataTimeoutInd strm_handle=0x%x!"
#define STRM_API_638_112_2_17_23_42_57_2688 "#STRM STRM_SendDataTimeoutInd invalid input!"
#define STRM_API_644_112_2_17_23_42_57_2689 "#STRM STRM_SendDataTimeoutInd creat signal FAIL!"
#define STRM_API_663_112_2_17_23_42_57_2690 "#STRM STRM_SendPauseCnf strm_handle=0x%x!"
#define STRM_API_667_112_2_17_23_42_57_2691 "#STRM STRM_SendPauseCnf invalid input!"
#define STRM_API_673_112_2_17_23_42_57_2692 "#STRM STRM_SendPauseCnf creat signal FAIL!"
#define STRM_API_692_112_2_17_23_42_57_2693 "#STRM STRM_SendResumeCnf strm_handle=0x%x!"
#define STRM_API_696_112_2_17_23_42_57_2694 "#STRM STRM_SendResumeCnf invalid input!"
#define STRM_API_702_112_2_17_23_42_58_2695 "#STRM STRM_SendResumeCnf creat signal FAIL!"
#define STRM_API_721_112_2_17_23_42_58_2696 "#STRM STRM_SendSeekCnf strm_handle=0x%x!"
#define STRM_API_725_112_2_17_23_42_58_2697 "#STRM STRM_SendSeekCnf invalid input!"
#define STRM_API_731_112_2_17_23_42_58_2698 "#STRM STRM_SendSeekCnf creat signal FAIL!"
#define STRM_API_750_112_2_17_23_42_58_2699 "#STRM STRM_SendCloseCnf strm_handle=0x%x!"
#define STRM_API_754_112_2_17_23_42_58_2700 "#STRM STRM_SendCloseCnf invalid input!"
#define STRM_API_760_112_2_17_23_42_58_2701 "#STRM STRM_SendCloseCnf creat signal FAIL!"
#define STRM_API_781_112_2_17_23_42_58_2702 "#STRM STRM_NotifyAppErr strm_handle=0x%x!"
#define STRM_API_785_112_2_17_23_42_58_2703 "#STRM STRM_NotifyAppErr invalid input!"
#define STRM_API_791_112_2_17_23_42_58_2704 "#STRM STRM_NotifyAppErr STRM_INIT_CNF"
#define STRM_API_795_112_2_17_23_42_58_2705 "#STRM STRM_NotifyAppErr STRM_INIT_CNF sig_ptr CREAT fail"
#define STRM_API_803_112_2_17_23_42_58_2706 "#STRM STRM_NotifyAppErr STRM_PLAY_CNF"
#define STRM_API_807_112_2_17_23_42_58_2707 "#STRM STRM_NotifyAppErr STRM_PLAY_CNF sig_ptr CREAT fail"
#define STRM_API_813_112_2_17_23_42_58_2708 "#STRM STRM_NotifyAppErr STRM_STOP_CNF"
#define STRM_API_817_112_2_17_23_42_58_2709 "#STRM STRM_NotifyAppErr STRM_STOP_CNF sig_ptr CREAT fail"
#define STRM_API_823_112_2_17_23_42_58_2710 "#STRM STRM_NotifyAppErr STRM_PAUSE_CNF"
#define STRM_API_827_112_2_17_23_42_58_2711 "#STRM STRM_NotifyAppErr STRM_PAUSE_CNF sig_ptr CREAT fail"
#define STRM_API_833_112_2_17_23_42_58_2712 "#STRM STRM_NotifyAppErr STRM_SEEK_CNF"
#define STRM_API_837_112_2_17_23_42_58_2713 "#STRM STRM_NotifyAppErr STRM_SEEK_CNF sig_ptr CREAT fail"
#define STRM_API_843_112_2_17_23_42_58_2714 "#STRM STRM_NotifyAppErr STRM_RESUME_CNF"
#define STRM_API_847_112_2_17_23_42_58_2715 "#STRM STRM_NotifyAppErr STRM_RESUME_CNF sig_ptr CREAT fail"
#define STRM_API_853_112_2_17_23_42_58_2716 "#STRM STRM_NotifyAppErr STRM_CLOSE_CNF"
#define STRM_API_857_112_2_17_23_42_58_2717 "#STRM STRM_NotifyAppErr STRM_SEEK_CNF sig_ptr CREAT fail"
#define STRM_BUFFER_282_112_2_17_23_42_59_2718 "#STRM STRMBUF_Creat, input invalid!"
#define STRM_BUFFER_291_112_2_17_23_42_59_2719 "#STRM STRMBUF_Creat, creat mutex fail!"
#define STRM_BUFFER_298_112_2_17_23_42_59_2720 "#STRM STRMBUF_Creat, StrmMem_CreatMemHandle fail!"
#define STRM_BUFFER_306_112_2_17_23_42_59_2721 "#STRM STRMBUF_Creat, creat audio buffer fail!"
#define STRM_BUFFER_312_112_2_17_23_42_59_2722 "#STRM STRMBUF_Creat, creat video buffer fail!"
#define STRM_BUFFER_373_112_2_17_23_42_59_2723 "#STRM STRMBUF_ClearBufData!"
#define STRM_BUFFER_376_112_2_17_23_42_59_2724 "#STRM STRMBUF_ClearBufData invalid input!"
#define STRM_BUFFER_450_112_2_17_23_42_59_2725 "#STRM STRMBUF_GetNsnNun input invalid!"
#define STRM_BUFFER_568_112_2_17_23_42_59_2726 "#STRM STRMBUF_ModifiyBufSize input invalid!"
#define STRM_BUFFER_598_112_2_17_23_42_59_2727 "#STRM STRMBUF_InsertPacket input invalid!"
#define STRM_BUFFER_607_112_2_17_23_42_59_2728 "#STRM STRMBUF_InsertPacket, buffer FULL, packet ignored"
#define STRM_BUFFER_641_112_2_17_23_42_59_2729 "#STRM STRMBUF_InsertPacket, unsupported encode type"
#define STRM_BUFFER_648_112_2_17_23_42_59_2730 "#STRM STRMBUF_InsertPacket, buffer FULL after inserted"
#define STRM_BUFFER_673_112_2_17_23_42_59_2731 "#STRM InsertAmr input invalid!"
#define STRM_BUFFER_682_112_2_17_23_42_59_2732 "#STRM InsertAmr, creat list fail!"
#define STRM_BUFFER_691_112_2_17_23_42_59_2733 "#STRM InsertAmr buffer is empty"
#define STRM_BUFFER_697_112_2_17_23_42_59_2734 "#STRM InsertAmr, ignore the out-of-time packet1!"
#define STRM_BUFFER_705_112_2_17_23_42_59_2735 "#STRM InsertAmr, insert node fail1!"
#define STRM_BUFFER_712_112_2_17_23_42_59_2736 "#STRM InsertAmr, alloc payload memory fail1!"
#define STRM_BUFFER_724_112_2_17_23_42_59_2737 "#STRM InsertAmr, the first packet inserted, initial timestamp:0x%x"
#define STRM_BUFFER_736_112_2_17_23_42_59_2738 "#STRM InsertAmr, ignore the out-of-time packet2!"
#define STRM_BUFFER_749_112_2_17_23_42_59_2739 "#STRM InsertAmr, invalid node!"
#define STRM_BUFFER_771_112_2_17_23_43_0_2740 "#STRM InsertAmr, insert node fail2!"
#define STRM_BUFFER_778_112_2_17_23_43_0_2741 "#STRM InsertAmr alloc payload memory fail2!"
#define STRM_BUFFER_787_112_2_17_23_43_0_2742 "#STRM InsertAmr, update initial timestamp:0x%x"
#define STRM_BUFFER_860_112_2_17_23_43_0_2743 "#STRM InsertPacketAccTimestampSeq, input invalidl!"
#define STRM_BUFFER_871_112_2_17_23_43_0_2744 "#STRM InsertPacketAccTimestampSeq, creat list fail!"
#define STRM_BUFFER_880_112_2_17_23_43_0_2745 "#STRM InsertPacketAccTimestampSeq buffer is empty"
#define STRM_BUFFER_886_112_2_17_23_43_0_2746 "#STRM InsertPacketAccTimestampSeq, ignore the out-of-time packet1!"
#define STRM_BUFFER_894_112_2_17_23_43_0_2747 "#STRM InsertPacketAccTimestampSeq, creat sub_list fail1!"
#define STRM_BUFFER_902_112_2_17_23_43_0_2748 "#STRM InsertPacketAccTimestampSeq, insert sub_node fail1!"
#define STRM_BUFFER_910_112_2_17_23_43_0_2749 "#STRM InsertPacketAccTimestampSeq, alloc sub_node payload mem fail1!"
#define STRM_BUFFER_924_112_2_17_23_43_0_2750 "#STRM InsertPacketAccTimestampSeq, insert sub_list fail1!"
#define STRM_BUFFER_936_112_2_17_23_43_0_2751 "#STRM InsertPacketAccTimestampSeq, the first packet inserted, initial timestamp :0x%x"
#define STRM_BUFFER_948_112_2_17_23_43_0_2752 "#STRM InsertPacketAccTimestampSeq, ignore the out-of-time packet2!"
#define STRM_BUFFER_959_112_2_17_23_43_0_2753 "#STRM InsertPacketAccTimestampSeq, invalid node!"
#define STRM_BUFFER_982_112_2_17_23_43_0_2754 "#STRM InsertPacketAccTimestampSeq, invalid node2!"
#define STRM_BUFFER_988_112_2_17_23_43_0_2755 "#STRM InsertPacketAccTimestampSeq, invalid sub_list!"
#define STRM_BUFFER_998_112_2_17_23_43_0_2756 "#STRM InsertPacketAccTimestampSeq, invalid sub_node_data_ptr!"
#define STRM_BUFFER_1019_112_2_17_23_43_0_2757 "#STRM InsertPacketAccTimestampSeq, insert sub_node fail2!"
#define STRM_BUFFER_1025_112_2_17_23_43_0_2758 "#STRM InsertPacketAccTimestampSeq, alloc sub_node payload mem fail2!"
#define STRM_BUFFER_1038_112_2_17_23_43_0_2759 "#STRM InsertPacketAccTimestampSeq, creat sub_list fail2!"
#define STRM_BUFFER_1046_112_2_17_23_43_0_2760 "#STRM InsertPacketAccTimestampSeq, insert sub_node fail1!"
#define STRM_BUFFER_1055_112_2_17_23_43_0_2761 "#STRM InsertPacketAccTimestampSeq, alloc sub_node payload mem fail1!"
#define STRM_BUFFER_1073_112_2_17_23_43_0_2762 "#STRM InsertPacketAccTimestampSeq, invalid node2!"
#define STRM_BUFFER_1097_112_2_17_23_43_0_2763 "#STRM InsertPacketAccTimestampSeq, insert sub_list fail1!"
#define STRM_BUFFER_1109_112_2_17_23_43_0_2764 "#STRM InsertPacketAccTimestampSeq, update initial timestamp :0x%x"
#define STRM_BUFFER_1146_112_2_17_23_43_0_2765 "#STRM InsertPacketAccTimestampSeqIndex, input invalidl!"
#define STRM_BUFFER_1156_112_2_17_23_43_0_2766 "#STRM InsertPacketAccTimestampSeqIndex, creat list fail!"
#define STRM_BUFFER_1167_112_2_17_23_43_0_2767 "#STRM InsertPacketAccTimestampSeqIndex, list is empty"
#define STRM_BUFFER_1173_112_2_17_23_43_0_2768 "#STRM InsertPacketAccTimestampSeqIndex, ignore the out-of-time packet!"
#define STRM_BUFFER_1180_112_2_17_23_43_0_2769 "#STRM InsertPacketAccTimestampSeqIndex, AddTimestampNodeToListByPos fail1!"
#define STRM_BUFFER_1189_112_2_17_23_43_0_2770 "#STRM InsertPacketAccTimestampSeqIndex, insert sub_list fail1!"
#define STRM_BUFFER_1198_112_2_17_23_43_0_2771 "#STRM InsertPacketAccTimestampSeqIndex, AddPacketNodeToListByPos fail!"
#define STRM_BUFFER_1207_112_2_17_23_43_0_2772 "#STRM InsertPacketAccTimestampSeqIndex, the first packet inserted, initial timestamp :0x%x"
#define STRM_BUFFER_1218_112_2_17_23_43_0_2773 "#STRM InsertPacketAccTimestampSeqIndex, ignore the out-of-time packet2!"
#define STRM_BUFFER_1230_112_2_17_23_43_0_2774 "#STRM InsertPacketAccTimestampSeqIndex, invalid cts_node!"
#define STRM_BUFFER_1237_112_2_17_23_43_0_2775 "#STRM InsertPacketAccTimestampSeqIndex, invalid sequence_list!"
#define STRM_BUFFER_1252_112_2_17_23_43_1_2776 "#STRM InsertPacketAccTimestampSeqIndex, AddPacketNodeToListByPos fail!"
#define STRM_BUFFER_1262_112_2_17_23_43_1_2777 "#STRM InsertPacketAccTimestampSeqIndex, insert sub_list fail1!"
#define STRM_BUFFER_1272_112_2_17_23_43_1_2778 "#STRM InsertPacketAccTimestampSeqIndex, AddPacketNodeToListByPos fail!"
#define STRM_BUFFER_1285_112_2_17_23_43_1_2779 "#STRM InsertPacketAccTimestampSeqIndex, AddTimestampNodeToListByPos fail1!"
#define STRM_BUFFER_1295_112_2_17_23_43_1_2780 "#STRM InsertPacketAccTimestampSeqIndex, insert sub_list fail1!"
#define STRM_BUFFER_1305_112_2_17_23_43_1_2781 "#STRM InsertPacketAccTimestampSeqIndex, AddPacketNodeToListByPos fail!"
#define STRM_BUFFER_1313_112_2_17_23_43_1_2782 "#STRM InsertPacketAccTimestampSeqIndex, update initial timestamp :0x%x"
#define STRM_BUFFER_1340_112_2_17_23_43_1_2783 "#STRM InsertH264 input invalid!"
#define STRM_BUFFER_1349_112_2_17_23_43_1_2784 "#STRM InsertH264, creat list fail!"
#define STRM_BUFFER_1362_112_2_17_23_43_1_2785 "#STRM InsertH264, ignore the out-of-time packet1!"
#define STRM_BUFFER_1371_112_2_17_23_43_1_2786 "#STRM InsertH264, insert node fail1!"
#define STRM_BUFFER_1378_112_2_17_23_43_1_2787 "#STRM InsertH264, alloc payload memory fail1!"
#define STRM_BUFFER_1391_112_2_17_23_43_1_2788 "#STRM InsertH264, the first packet inserted, initial_timestamp:0x%x"
#define STRM_BUFFER_1402_112_2_17_23_43_1_2789 "#STRM InsertH264, ignore the out-of-time packet2!"
#define STRM_BUFFER_1412_112_2_17_23_43_1_2790 "#STRM InsertH264, invalid node!"
#define STRM_BUFFER_1462_112_2_17_23_43_1_2791 "#STRM InsertH264, insert fail2!"
#define STRM_BUFFER_1469_112_2_17_23_43_1_2792 "#STRM InsertH264, alloc payload mem fail2!"
#define STRM_BUFFER_1480_112_2_17_23_43_1_2793 "#STRM InsertH264, update initial_timestamp:%d"
#define STRM_BUFFER_1505_112_2_17_23_43_1_2794 "#STRM InsertMPEG4!"
#define STRM_BUFFER_1524_112_2_17_23_43_1_2795 "#STRM CheckIfSendPacket input invalid:first:%d, last:%d"
#define STRM_BUFFER_1535_112_2_17_23_43_1_2796 "#STRM CheckIfSendPacket :first:%d, last:%d, start:%d, end:%d"
#define STRM_BUFFER_1557_112_2_17_23_43_1_2797 "#STRM STRMBUF_GetPacket, input invalid!"
#define STRM_BUFFER_1598_112_2_17_23_43_1_2798 "#STRM STRMBUF_GetPacket, unsupported encode type = %d!"
#define STRM_BUFFER_1615_112_2_17_23_43_1_2799 "#STRM STRMBUF_FreePacket input invalid!"
#define STRM_BUFFER_1638_112_2_17_23_43_1_2800 "#STRM GetAmrPacket input invalid!"
#define STRM_BUFFER_1644_112_2_17_23_43_1_2801 "#STRM GetAmrPacket, list null"
#define STRM_BUFFER_1652_112_2_17_23_43_1_2802 "#STRM GetAmrPacket, empty list"
#define STRM_BUFFER_1671_112_2_17_23_43_1_2803 "#STRM GetAmrPacket, not enough buffer data to send"
#define STRM_BUFFER_1848_112_2_17_23_43_2_2804 "#STRM GetPacketAccTimestampSeq, input invalid"
#define STRM_BUFFER_1854_112_2_17_23_43_2_2805 "#STRM GetPacketAccTimestampSeq, list null"
#define STRM_BUFFER_1860_112_2_17_23_43_2_2806 "#STRM GetPacketAccTimestampSeq, STRM_BUF_GET_RESULT_EMPTY!"
#define STRM_BUFFER_1877_112_2_17_23_43_2_2807 "#STRM GetPacketAccTimestampSeq, not enough buffer data to send"
#define STRM_BUFFER_1885_112_2_17_23_43_2_2808 "#STRM GetPacketAccTimestampSeq, not enough buffer data to send 2"
#define STRM_BUFFER_1895_112_2_17_23_43_2_2809 "#STRM GetPacketAccTimestampSeq, empty list"
#define STRM_BUFFER_1911_112_2_17_23_43_2_2810 "#STRM GetPacketAccTimestampSeq, sub_node_data_ptr is NULL or last"
#define STRM_BUFFER_1948_112_2_17_23_43_2_2811 "#STRM GetPacketAccTimestampSeq, alloc payload mem fail or is_list_bad=%d!"
#define STRM_BUFFER_2008_112_2_17_23_43_2_2812 "#STRM GetPacketAccTimestampSeq, STRM_BUF_GET_RESULT_FAIL!"
#define STRM_BUFFER_2048_112_2_17_23_43_2_2813 "#STRM GetPacketAccTimestampSeqIndex, input invalid"
#define STRM_BUFFER_2055_112_2_17_23_43_2_2814 "#STRM GetPacketAccTimestampSeqIndex, list null"
#define STRM_BUFFER_2062_112_2_17_23_43_2_2815 "#STRM GetPacketAccTimestampSeqIndex, STRM_BUF_GET_RESULT_EMPTY!"
#define STRM_BUFFER_2078_112_2_17_23_43_2_2816 "#STRM GetPacketAccTimestampSeqIndex, not enough buffer data to send"
#define STRM_BUFFER_2087_112_2_17_23_43_2_2817 "#STRM GetPacketAccTimestampSeqIndex, empty list"
#define STRM_BUFFER_2090_112_2_17_23_43_2_2818 "#STRM GetPacketAccTimestampSeqIndex, timestamp:0x%x"
#define STRM_BUFFER_2122_112_2_17_23_43_2_2819 "#STRM GetPacketAccTimestampSeqIndex fail:alloc data_ptr!"
#define STRM_BUFFER_2178_112_2_17_23_43_2_2820 "#STRM GetPacketAccTimestampSeqIndex fail:data_len > au_size!"
#define STRM_BUFFER_2188_112_2_17_23_43_2_2821 "#STRM GetPacketAccTimestampSeqIndex fail:data_len > au_size!"
#define STRM_BUFFER_2198_112_2_17_23_43_2_2822 "#STRM GetPacketAccTimestampSeqIndex fail:packet number != 1!"
#define STRM_BUFFER_2265_112_2_17_23_43_3_2823 "#STRM GetPacketAccTimestampSeqIndex fail:data_ptr invalid!"
#define STRM_BUFFER_2272_112_2_17_23_43_3_2824 "#STRM GetPacketAccTimestampSeqIndex fail:timestamp node doesn't have sequence node!"
#define STRM_BUFFER_2278_112_2_17_23_43_3_2825 "#STRM GetPacketAccTimestampSeqIndex, pos:0x%x"
#define STRM_BUFFER_2281_112_2_17_23_43_3_2826 "#STRM GetPacketAccTimestampSeqIndex, STRM_BUF_GET_RESULT_FAIL!"
#define STRM_BUFFER_2290_112_2_17_23_43_3_2827 "#STRM GetPacketAccTimestampSeqIndex fail: data_len!=au_size!"
#define STRM_BUFFER_2317_112_2_17_23_43_3_2828 "#STRM GetH264Packet input invalid!"
#define STRM_BUFFER_2323_112_2_17_23_43_3_2829 "#STRM GetH264Packet, list null"
#define STRM_BUFFER_2330_112_2_17_23_43_3_2830 "#STRM GetH264Packet, empty list"
#define STRM_BUFFER_2337_112_2_17_23_43_3_2831 "#STRM GetH264Packet, invalid list node!"
#define STRM_BUFFER_2343_112_2_17_23_43_3_2832 "#STRM GetH264Packet, invalid list node!"
#define STRM_BUFFER_2354_112_2_17_23_43_3_2833 "#STRM GetH264Packet, not enough buffer data!"
#define STRM_BUFFER_2365_112_2_17_23_43_3_2834 "#STRM GetH264Packet, invalid list node!"
#define STRM_BUFFER_2417_112_2_17_23_43_3_2835 "#STRM GetH264Packet, invalid temp_node_data_ptr!"
#define STRM_BUFFER_2453_112_2_17_23_43_3_2836 "#STRM GetH264Packet, PackH264Fragments fail!"
#define STRM_BUFFER_2483_112_2_17_23_43_3_2837 "#STRM GetH264Packet, DeleteH264NodeBetween fail!"
#define STRM_BUFFER_2496_112_2_17_23_43_3_2838 "#STRM GetH264Packet packet is empty STRM_BUF_GET_RESULT_FAIL!"
#define STRM_BUFFER_2523_112_2_17_23_43_3_2839 "#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr is null!"
#define STRM_BUFFER_2534_112_2_17_23_43_3_2840 "#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr2 is null!"
#define STRM_BUFFER_2539_112_2_17_23_43_3_2841 "#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr->seq_num!=seq!"
#define STRM_BUFFER_2553_112_2_17_23_43_3_2842 "#STRM CheckIfBetween2NodesSeqNumContinuous pack_ptr3 is null!"
#define STRM_BUFFER_2558_112_2_17_23_43_3_2843 "#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr->seq_num!=seq!2"
#define STRM_BUFFER_2568_112_2_17_23_43_3_2844 "#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr is null!"
#define STRM_BUFFER_2579_112_2_17_23_43_3_2845 "#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr2 is null!"
#define STRM_BUFFER_2584_112_2_17_23_43_3_2846 "#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr->seq_num!=seq!"
#define STRM_BUFFER_2598_112_2_17_23_43_3_2847 "#STRM CheckIfBetween2NodesSeqNumContinuous pack_ptr3 is null!"
#define STRM_BUFFER_2603_112_2_17_23_43_3_2848 "#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr->seq_num!=seq!2"
#define STRM_BUFFER_2634_112_2_17_23_43_3_2849 "#STRM PackH264Fragments input invalid!"
#define STRM_BUFFER_2686_112_2_17_23_43_3_2850 "#STRM PackH264Fragments memcpy fatal error!"
#define STRM_BUFFER_2712_112_2_17_23_43_4_2851 "#STRM DeleteH264NodeBetween input invalid!"
#define STRM_BUFFER_2722_112_2_17_23_43_4_2852 "#STRM DeleteH264NodeBetween, invalid node!"
#define STRM_BUFFER_2753_112_2_17_23_43_4_2853 "#STRM FindNodeInListByTimestamp, invalid input!"
#define STRM_BUFFER_2764_112_2_17_23_43_4_2854 "#STRM FindNodeInListByTimestamp, invalid node!"
#define STRM_BUFFER_2800_112_2_17_23_43_4_2855 "#STRM FindNodeInListBySequence, invalid input!"
#define STRM_BUFFER_2811_112_2_17_23_43_4_2856 "#STRM FindNodeInListBySequence, invalid node!"
#define STRM_BUFFER_2848_112_2_17_23_43_4_2857 "#STRM FindPosToInsertNodeByMpeg4Index, invalid input!"
#define STRM_BUFFER_2858_112_2_17_23_43_4_2858 "#STRM FindPosToInsertNodeByMpeg4Index, invalid packet_node_data_ptr!"
#define STRM_BUFFER_2891_112_2_17_23_43_4_2859 "#STRM FindPosToInsertNodeByTimestamp, invalid input!"
#define STRM_BUFFER_2901_112_2_17_23_43_4_2860 "#STRM FindPosToInsertNodeByTimestamp, invalid node2!"
#define STRM_BUFFER_2934_112_2_17_23_43_4_2861 "#STRM FindPosToInsertNodeBySequence, invalid input!"
#define STRM_BUFFER_2944_112_2_17_23_43_4_2862 "#STRM FindPosToInsertNodeBySequence, invalid node2!"
#define STRM_BUFFER_2976_112_2_17_23_43_4_2863 "#STRM AddPacketNodeToListByPos, invalid input!"
#define STRM_BUFFER_2988_112_2_17_23_43_4_2864 "#STRM AddPacketNodeToListByPos, insert packet_node_data_ptr fail!"
#define STRM_BUFFER_2996_112_2_17_23_43_4_2865 "#STRM AddPacketNodeToListByPos, alloc sub_node payload mem fail2!"
#define STRM_BUFFER_3017_112_2_17_23_43_4_2866 "#STRM AddTimestampNodeToListByPos, invalid input!"
#define STRM_BUFFER_3025_112_2_17_23_43_4_2867 "#STRM AddTimestampNodeToListByPos, creat sequence_list fail1!"
#define STRM_BUFFER_3034_112_2_17_23_43_4_2868 "#STRM AddTimestampNodeToListByPos, insert sub_list fail1!"
#define STRM_BUFFER_3056_112_2_17_23_43_4_2869 "#STRM AddSequenceNodeToListByPos, invalid input!"
#define STRM_BUFFER_3064_112_2_17_23_43_4_2870 "#STRM AddSequenceNodeToListByPos, creat sequence_list fail1!"
#define STRM_BUFFER_3070_112_2_17_23_43_4_2871 "#STRM AddSequenceNodeToListByPos, insert sub_list fail1!"
#define STRM_BUFFER_3093_112_2_17_23_43_4_2872 "#STRM DestroyPacketList invalid input!"
#define STRM_BUFFER_3132_112_2_17_23_43_4_2873 "STRM DestroySequenceNode invalid input!"
#define STRM_BUFFER_3160_112_2_17_23_43_4_2874 "#STRM DestroySeqenceList invalid input!"
#define STRM_BUFFER_3188_112_2_17_23_43_5_2875 "#STRM DestroyTimestampNode invalid input!"
#define STRM_BUFFER_3215_112_2_17_23_43_5_2876 "#STRM DestroyTimestampList invalid input!"
#define STRM_BUFFER_3249_112_2_17_23_43_5_2877 "#STRM DestroyBufListFor2Levels invalid input!"
#define STRM_BUFFER_3286_112_2_17_23_43_5_2878 "#STRM StrmMem_CreatMemHandle!"
#define STRM_BUFFER_3305_112_2_17_23_43_5_2879 "#STRM StrmMem_CreatMemHandle:Mem Handle has been initiated!"
#define STRM_BUFFER_3326_112_2_17_23_43_5_2880 "#STRM StrmMem_CreatMemHandle:Mem Handle has been initiated!"
#define STRM_BUFFER_3333_112_2_17_23_43_5_2881 "#STRM StrmMem_CreatMemHandle fail!"
#define STRM_BUFFER_3346_112_2_17_23_43_5_2882 "#STRM StrmMem_DeleteMemHandle!"
#define STRM_BUFFER_TEST_193_112_2_17_23_43_6_2883 "#STRM buffer test, amr produce, init_timestamp = %d"
#define STRM_BUFFER_TEST_460_112_2_17_23_43_6_2884 "#STRM Buffer Test, insert AMR, seq=%d, timestamp=%d, result:%d"
#define STRM_BUFFER_TEST_463_112_2_17_23_43_6_2885 "#STRM Buffer Test, insert AMR, init_timestamp=%d"
#define STRM_BUFFER_TEST_477_112_2_17_23_43_6_2886 "#STRM Buffer Test, insert h263, seq=%d, timestamp=%d, result:%d"
#define STRM_BUFFER_TEST_480_112_2_17_23_43_6_2887 "#STRM Buffer Test, insert h263, init_timestamp=%d"
#define STRM_BUFFER_TEST_494_112_2_17_23_43_6_2888 "#STRM Buffer Test, insert h264, seq=%d, string:%s, result:%d"
#define STRM_BUFFER_TEST_497_112_2_17_23_43_6_2889 "#STRM Buffer Test, insert h264, init_timestamp=%d"
#define STRM_BUFFER_TEST_504_112_2_17_23_43_6_2890 "#STRM Buffer Test, insert h264, seq:%d, list:%s"
#define STRM_BUFFER_TEST_522_112_2_17_23_43_6_2891 "#STRM Buffer Test, insert h264, seq=%d, string:%s, result:%d"
#define STRM_BUFFER_TEST_525_112_2_17_23_43_6_2892 "#STRM Buffer Test, insert h264, init_timestamp=%d"
#define STRM_BUFFER_TEST_532_112_2_17_23_43_6_2893 "#STRM Buffer Test, insert h264, seq:%d, list:%s"
#define STRM_BUFFER_TEST_550_112_2_17_23_43_6_2894 "#STRM Buffer Test,  Mpeg4Insert, seq=%d, timestamp=%d, result:%d"
#define STRM_BUFFER_TEST_553_112_2_17_23_43_6_2895 "#STRM Buffer Test, Mpeg4Insert, init_timestamp=%d"
#define STRM_BUFFER_TEST_579_112_2_17_23_43_6_2896 "#STRM Buffer Test, packed amr buf_size:%d"
#define STRM_BUFFER_TEST_580_112_2_17_23_43_6_2897 "#STRM Buffer Test, packed amr string:%s"
#define STRM_BUFFER_TEST_608_112_2_17_23_43_6_2898 "#STRM Buffer Test, packed h263 buf_size:%d"
#define STRM_BUFFER_TEST_609_112_2_17_23_43_6_2899 "#STRM Buffer Test, packed h263 string:%s"
#define STRM_BUFFER_TEST_638_112_2_17_23_43_6_2900 "#STRM Buffer Test, packed h264 buf_size:%d"
#define STRM_BUFFER_TEST_639_112_2_17_23_43_6_2901 "#STRM Buffer Test, packed h264 string:%s"
#define STRM_BUFFER_TEST_668_112_2_17_23_43_6_2902 "#STRM Buffer Test, packed mpeg4 buf_size:%d"
#define STRM_BUFFER_TEST_669_112_2_17_23_43_6_2903 "#STRM Buffer Test, packed mpeg4 string:%s"
#define STRM_HSM_354_112_2_17_23_43_7_2904 "#STRM StrmHsmConstructor, ERR input!"
#define STRM_HSM_373_112_2_17_23_43_7_2905 "#STRM StrmHsmDestructor, ERR input!"
#define STRM_HSM_404_112_2_17_23_43_7_2906 "#STRM StrmStreaming, ERR input!"
#define STRM_HSM_470_112_2_17_23_43_7_2907 "#STRM StrmIdle, ERR input!"
#define STRM_HSM_491_112_2_17_23_43_7_2908 "#STRM StrmIdle init rtsp session failed!"
#define STRM_HSM_516_112_2_17_23_43_7_2909 "#STRM StrmInitRtspPending, ERR input!"
#define STRM_HSM_538_112_2_17_23_43_8_2910 "#STRM StrmInitRtspPending parse SDP fail!"
#define STRM_HSM_549_112_2_17_23_43_8_2911 "#STRM StrmInitRtspPending no SDP buffer while using http!"
#define STRM_HSM_569_112_2_17_23_43_8_2912 "#STRM StrmInitRtspPending RTSP DESCRIBE FAILED!"
#define STRM_HSM_576_112_2_17_23_43_8_2913 "#STRM StrmInitRtspPending RTSP init return FAILED!"
#define STRM_HSM_599_112_2_17_23_43_8_2914 "#STRM HandleDscrbCnf, ERR input!"
#define STRM_HSM_618_112_2_17_23_43_8_2915 "#STRM HandleDscrbCnf creat RTP1 FAILED!"
#define STRM_HSM_630_112_2_17_23_43_8_2916 "#STRM HandleDscrbCnf creat RTP2 FAILED!"
#define STRM_HSM_641_112_2_17_23_43_8_2917 "#STRM HandleDscrbCnf send init cnf FAILED!"
#define STRM_HSM_648_112_2_17_23_43_8_2918 "#STRM StrmDescribePending RTSP describe return FAILED!"
#define STRM_HSM_668_112_2_17_23_43_8_2919 "#STRM StrmDescribePending, ERR input!"
#define STRM_HSM_700_112_2_17_23_43_8_2920 "#STRM StrmDescribePending Send auth ind fail!"
#define STRM_HSM_729_112_2_17_23_43_8_2921 "#STRM StrmInitiated, ERR input!"
#define STRM_HSM_761_112_2_17_23_43_8_2922 "#STRM StrmDescribeAuthing, ERR input!"
#define STRM_HSM_794_112_2_17_23_43_8_2923 "#STRM HandleDscrbAuthRsp input invalid!"
#define STRM_HSM_800_112_2_17_23_43_8_2924 "#STRM HandleDscrbAuthRsp err input auth info!"
#define STRM_HSM_809_112_2_17_23_43_8_2925 "#STRM HandleDscrbAuthRsp send auth rsp failed!"
#define STRM_HSM_829_112_2_17_23_43_8_2926 "#STRM StrmSetup, ERR input!"
#define STRM_HSM_898_112_2_17_23_43_8_2927 "#STRM HandleSetupFirstCnf input invalid!"
#define STRM_HSM_923_112_2_17_23_43_8_2928 "#STRM StrmSetup1 RTSP SETUP FAILED!"
#define STRM_HSM_940_112_2_17_23_43_8_2929 "#STRM StrmSetup1 RTSP PLAY FAILED!"
#define STRM_HSM_948_112_2_17_23_43_8_2930 "#STRM StrmSetup1 RTSP setup1 return FAILED!"
#define STRM_HSM_963_112_2_17_23_43_8_2931 "#STRM HandleSetupAuthInd input invalid!"
#define STRM_HSM_985_112_2_17_23_43_8_2932 "#STRM HandleSetupAuthInd Send auth ind fail!"
#define STRM_HSM_1001_112_2_17_23_43_8_2933 "#STRM HandleSetupAuthInd input invalid!"
#define STRM_HSM_1023_112_2_17_23_43_9_2934 "#STRM HandlePlayAuthInd Send auth ind fail!"
#define STRM_HSM_1043_112_2_17_23_43_9_2935 "#STRM StrmSetup1, ERR input!"
#define STRM_HSM_1099_112_2_17_23_43_9_2936 "#STRM HandleSetupAuthRsp input invalid!"
#define STRM_HSM_1106_112_2_17_23_43_9_2937 "#STRM HandleSetupAuthRsp err input auth info!"
#define STRM_HSM_1115_112_2_17_23_43_9_2938 "#STRM HandleSetupAuthRsp auth rsp failed!"
#define STRM_HSM_1136_112_2_17_23_43_9_2939 "#STRM HandlePlayAuthRsp input invalid!"
#define STRM_HSM_1143_112_2_17_23_43_9_2940 "#STRM HandlePlayAuthRsp err input auth info!"
#define STRM_HSM_1152_112_2_17_23_43_9_2941 "#STRM HandlePlayAuthRsp auth rsp failed!"
#define STRM_HSM_1172_112_2_17_23_43_9_2942 "#STRM StrmSetup1Auth, ERR input!"
#define STRM_HSM_1205_112_2_17_23_43_9_2943 "#STRM StrmSetup1Auth, ERR input!"
#define STRM_HSM_1238_112_2_17_23_43_9_2944 "#STRM HandleSetup2Cnf input invalid!"
#define STRM_HSM_1255_112_2_17_23_43_9_2945 "#STRM HandleSetup2Cnf RTSP PLAY FAILED!"
#define STRM_HSM_1262_112_2_17_23_43_9_2946 "#STRM HandleSetup2Cnf RTSP setup2 return FAILED!"
#define STRM_HSM_1281_112_2_17_23_43_9_2947 "#STRM StrmSetup2, ERR input!"
#define STRM_HSM_1325_112_2_17_23_43_9_2948 "#STRM StrmSetup2Auth, ERR input!"
#define STRM_HSM_1356_112_2_17_23_43_9_2949 "#STRM HandleGoodbyeInd input invalid!"
#define STRM_HSM_1372_112_2_17_23_43_9_2950 "#STRM HandleGoodbyeInd wrong rtp handle!"
#define STRM_HSM_1387_112_2_17_23_43_9_2951 "#STRM HandleSsrcSetInd input invalid!"
#define STRM_HSM_1418_112_2_17_23_43_9_2952 "#STRM HandleSsrcSetInd, STRM_RTP_SSRC_SET_IND, wrong rtp handle"
#define STRM_HSM_1431_112_2_17_23_43_9_2953 "#STRM StrmPlay!"
#define STRM_HSM_1435_112_2_17_23_43_9_2954 "#STRM StrmPlay, ERR input!"
#define STRM_HSM_1441_112_2_17_23_43_9_2955 "#STRM StrmPlay, STRM_STOP_REQ!"
#define STRM_HSM_1457_112_2_17_23_43_9_2956 "#STRM StrmPlay STRM_RTP_SRRR_SEND_IND err!"
#define STRM_HSM_1490_112_2_17_23_43_10_2957 "#STRM StrmPlayPending, ERR input!"
#define STRM_HSM_1493_112_2_17_23_43_10_2958 "#STRM StrmPlayPending is_seek:%d!"
#define STRM_HSM_1537_112_2_17_23_43_10_2959 "#STRM StrmPlayPending,STRM_RTSP_PLAY_CNF is_seek play_start_time:%d!"
#define STRM_HSM_1551_112_2_17_23_43_10_2960 "#STRM StrmPlayPending,STRM_RTSP_PLAY_CNF play_start_time:%d!"
#define STRM_HSM_1563_112_2_17_23_43_10_2961 "#STRM StrmPlayPending resume, RTSP return failed!"
#define STRM_HSM_1569_112_2_17_23_43_10_2962 "#STRM StrmPlayPending seek, RTSP return failed!"
#define STRM_HSM_1577_112_2_17_23_43_10_2963 "#STRM StrmPlayPending, RTSP PLAY return failed!"
#define STRM_HSM_1606_112_2_17_23_43_10_2964 "#STRM StrmPlayBuffering, ERR input!"
#define STRM_HSM_1609_112_2_17_23_43_10_2965 "#STRM StrmPlayBuffering hsm_handle=0x%x,sig=0x%x!"
#define STRM_HSM_1622_112_2_17_23_43_10_2966 "#STRM StrmPlayBuffering, timer handle=0x%x!"
#define STRM_HSM_1628_112_2_17_23_43_10_2967 "#STRM StrmPlayBuffering, STRM_RTP_GOODBYE_IND!"
#define STRM_HSM_1640_112_2_17_23_43_10_2968 "#STRM StrmPlayBuffering, STRM_BUFFER_FULL_IND!"
#define STRM_HSM_1700_112_2_17_23_43_10_2969 "#STRM StrmPlayBuffering, STRM_PAUSE_REQ!"
#define STRM_HSM_1710_112_2_17_23_43_10_2970 "#STRM StrmPlayBuffering, STRM_SEEK_REQ!"
#define STRM_HSM_1722_112_2_17_23_43_10_2971 "#STRM StrmPlayBuffering, HSM_STATE_EXIT_SIG!"
#define STRM_HSM_1764_112_2_17_23_43_10_2972 "#STRM StrmGetFrame, input error: player_handle=0x%x,playload_type=%d,frame_data_ptr=0x%x!"
#define STRM_HSM_1774_112_2_17_23_43_10_2973 "#STRM StrmGetFrame couldn't get valid strm_hsm!"
#define STRM_HSM_1778_112_2_17_23_43_10_2974 "#STRM StrmGetFrame, no DPLAYER_GetTime under WIN32"
#define STRM_HSM_1811_112_2_17_23_43_10_2975 "#STRM StrmGetFrame,hsm=0x%x,player=0x%x,type:%d,end:%d,cur:%d,start:%d, data_pos(timestamp):%d"
#define STRM_HSM_1815_112_2_17_23_43_10_2976 "#STRM StrmGetFrame, type:%d, STRMBUF_GetPacket failed!"
#define STRM_HSM_1829_112_2_17_23_43_10_2977 "#STRM StrmGetFrame, the buffer nearly empty."
#define STRM_HSM_1861_112_2_17_23_43_10_2978 "#STRM StrmGetFrame, play end: cur_time=0x%x, end_time:0x%x"
#define STRM_HSM_1867_112_2_17_23_43_10_2979 "#STRM StrmGetFrame, send goodbye"
#define STRM_HSM_1877_112_2_17_23_43_10_2980 "#STRM StrmGetFrame, is_video_valid is %d, last_video_frame_time=0x%x, play end time is 0x%x"
#define STRM_HSM_1882_112_2_17_23_43_10_2981 "#STRM StrmGetFrame, the buffer is empty or nearly empty."
#define STRM_HSM_1887_112_2_17_23_43_10_2982 "#STRM StrmGetFrame, STRM_BUF_GET_RESULT_EMPTY. last_video_frame_time is %d"
#define STRM_HSM_1888_112_2_17_23_43_10_2983 "#STRM StrmGetFrame, STRM_BUF_GET_RESULT_EMPTY.last_audio_frame_time is %d"
#define STRM_HSM_1919_112_2_17_23_43_10_2984 "#STRM StrmTestDecode write file error!"
#define STRM_HSM_1941_112_2_17_23_43_10_2985 "#STRM StrmFreeFrame input invalid!"
#define STRM_HSM_1949_112_2_17_23_43_11_2986 "#STRM StrmFreeFrame couldn't get valid strm_hsm!"
#define STRM_HSM_1955_112_2_17_23_43_11_2987 "#STRM StrmFreeFrame type:%d pos:0x%x data_ptr=0x%x!"
#define STRM_HSM_1986_112_2_17_23_43_11_2988 "#STRM StrmPlayPlaying, ERR input!"
#define STRM_HSM_1989_112_2_17_23_43_11_2989 "#STRM StrmPlayPlaying, hsm_handle=0x%x, sig=0x%x!"
#define STRM_HSM_1997_112_2_17_23_43_11_2990 "#STRM StrmPlayPlaying, DPLAYER_StreamRxStart from time:%d!"
#define STRM_HSM_2000_112_2_17_23_43_11_2991 "#STRM StrmPlayPlaying, no DPLAYER_StreamRxStart under WIN32"
#define STRM_HSM_2010_112_2_17_23_43_11_2992 "#STRM StrmPlayPlaying MEDIA IS START timer id=%d!"
#define STRM_HSM_2064_112_2_17_23_43_11_2993 "#STRM StrmPlayPlaying MEDIA timer out don't data!"
#define STRM_HSM_2071_112_2_17_23_43_11_2994 "#STRM StrmPlayPlaying MEDIA timer out!"
#define STRM_HSM_2076_112_2_17_23_43_11_2995 "#STRM StrmPlayPlaying, DPLAYER_StreamRxStop!"
#define STRM_HSM_2079_112_2_17_23_43_11_2996 "#STRM StrmPlayPlaying, no DPLAYER_StreamRxStop under WIN32"
#define STRM_HSM_2089_112_2_17_23_43_11_2997 "#STRM StrmPlayPlaying MEDIA IS STOP timer id=%d"
#define STRM_HSM_2116_112_2_17_23_43_11_2998 "#STRM StrmPlayPausePending, ERR input!"
#define STRM_HSM_2160_112_2_17_23_43_11_2999 "#STRM StrmPlayPausePending seek, RTSP PAUSE return failed!"
#define STRM_HSM_2167_112_2_17_23_43_11_3000 "#STRM StrmPlayPausePending, RTSP PAUSE return failed!"
#define STRM_HSM_2192_112_2_17_23_43_11_3001 "#STRM StrmPlayPaused, ERR input!"
#define STRM_HSM_2243_112_2_17_23_43_11_3002 "#STRM StrmStopPending, ERR input!"
#define STRM_HSM_2246_112_2_17_23_43_11_3003 "#STRM StrmStopPending hsm_handle=0x%x, sig=0x%x!"
#define STRM_HSM_2264_112_2_17_23_43_11_3004 "#STRM StrmStopPending, RTSP STOP return failed!"
#define STRM_HSM_2286_112_2_17_23_43_11_3005 "#STRM SetSeekFlag input invalid!"
#define STRM_HSM_2289_112_2_17_23_43_11_3006 "#STRM SetSeekFlag to %d"
#define STRM_HSM_2304_112_2_17_23_43_11_3007 "#STRM SetResumeFlag input invalid!"
#define STRM_HSM_2307_112_2_17_23_43_11_3008 "#STRM SetResumeFlag to %d"
#define STRM_HSM_2322_112_2_17_23_43_11_3009 "#STRM SaveAuthInfo input invalid!"
#define STRM_HSM_2328_112_2_17_23_43_11_3010 "#STRM HandleAuthRsp err auth rsp param!"
#define STRM_HSM_2345_112_2_17_23_43_11_3011 "#STRM HandleAuthRsp err save username!"
#define STRM_HSM_2351_112_2_17_23_43_11_3012 "#STRM HandleAuthRsp err save password_ptr!"
#define STRM_HSM_2367_112_2_17_23_43_11_3013 "#STRM HandlePauseReq input invalid!"
#define STRM_HSM_2375_112_2_17_23_43_11_3014 "#STRM HandlePauseReq pause fail!"
#define STRM_HSM_2391_112_2_17_23_43_11_3015 "#STRM HandleCloseReq input invalid!"
#define STRM_HSM_2422_112_2_17_23_43_11_3016 "#STRM HandleCloseReq input invalid!"
#define STRM_HSM_2433_112_2_17_23_43_11_3017 "#STRM HandleStopReq teardown failed!"
#define STRM_HSM_2458_112_2_17_23_43_12_3018 "#STRM HandleRtcpSrrrSendInd input invalid!"
#define STRM_HSM_2487_112_2_17_23_43_12_3019 "#STRM StrmPlay STRM_RTP_SRRR_SEND_IND, error payload type!"
#define STRM_HSM_2512_112_2_17_23_43_12_3020 "#STRM HandleRtcpSrrrSendInd send NADU, free buffer:%x, nsn:%d, nun:%d"
#define STRM_HSM_2526_112_2_17_23_43_12_3021 "#STRM HandleRtcpSrrrSendInd get nsn nun fail!"
#define STRM_HSM_2550_112_2_17_23_43_12_3022 "#STRM HandlePlayReq input invalid!"
#define STRM_HSM_2560_112_2_17_23_43_12_3023 "#STRM HandlePlayReq, no DPLAYER_StreamRegistRxDataPath under WIN32"
#define STRM_HSM_2591_112_2_17_23_43_12_3024 "#STRM HandlePlayReq RTSP SETUP FAILED!"
#define STRM_HSM_2616_112_2_17_23_43_12_3025 "#STRM HandleSeekReq input invalid!"
#define STRM_HSM_2654_112_2_17_23_43_12_3026 "#STRM HandleSeekReq SEEK fail: invalid start_time!"
#define STRM_HSM_2658_112_2_17_23_43_12_3027 "#STRM HandleSeekReq, send goodbye"
#define STRM_HSM_2670_112_2_17_23_43_12_3028 "#STRM HandleSeekReq PAUSED SEEK fail!"
#define STRM_HSM_2679_112_2_17_23_43_12_3029 "#STRM HandleSeekReq NON PAUSED fail!"
#define STRM_HSM_2701_112_2_17_23_43_12_3030 "#STRM HandleResumeReq input invalid!"
#define STRM_HSM_2723_112_2_17_23_43_12_3031 "#STRM StrmPlayBuffering RESUME fail!"
#define STRM_HSM_2745_112_2_17_23_43_12_3032 "#STRM STRMHSM_InsertPacketToBuf input invalid!"
#define STRM_HSM_2753_112_2_17_23_43_12_3033 "#STRM STRMBUF_InsertPacket audio strm_handle=0x%x timestamp:0x%x, now buf_size=%d"
#define STRM_HSM_2759_112_2_17_23_43_12_3034 "#STRM STRMBUF_InsertPacket video strm_handle=0x%x timestamp:0x%x, now buf_size=%d"
#define STRM_HSM_2763_112_2_17_23_43_12_3035 "#STRM STRMHSM_InsertPacketToBuf, wrong payload type!"
#define STRM_HSM_2775_112_2_17_23_43_12_3036 "#STRM STRMHSM_InsertPacketToBuf, insert fail!"
#define STRM_HSM_2781_112_2_17_23_43_12_3037 "#STRM STRMHSM_InsertPacketToBuf, buffer FULL, ignore the packet!"
#define STRM_HSM_2786_112_2_17_23_43_12_3038 "#STRM STRMHSM_InsertPacketToBuf, first packet received, send STRM_RTP_SSRC_SET_IND"
#define STRM_HSM_2802_112_2_17_23_43_12_3039 "#STRM SendSetRtpSsrcInd input invalid!"
#define STRM_HSM_2808_112_2_17_23_43_12_3040 "#STRM SendSetRtpSsrcInd, creat signal fail!"
#define STRM_HSM_2828_112_2_17_23_43_12_3041 "#STRM SendBufferEmptyInd input invalid!"
#define STRM_HSM_2834_112_2_17_23_43_12_3042 "#STRM SendBufferEmptyInd creat signal FAIL!"
#define STRM_HSM_2852_112_2_17_23_43_12_3043 "#STRM SendBufferFullInd input invalid!"
#define STRM_HSM_2858_112_2_17_23_43_12_3044 "#STRM SendBufferEmptyInd creat signal FAIL!"
#define STRM_HSM_2875_112_2_17_23_43_12_3045 "#STRM AddHsmToList input invalid!"
#define STRM_HSM_2891_112_2_17_23_43_12_3046 "#STRM AddHsmToList creat list failed!"
#define STRM_HSM_2898_112_2_17_23_43_12_3047 "#STRM AddHsmToList insert node failed!"
#define STRM_HSM_2914_112_2_17_23_43_12_3048 "#STRM DeleteHsmFromList hsm_handle=0x%x!"
#define STRM_HSM_2918_112_2_17_23_43_12_3049 "#STRM DeleteHsmFromList input invalid!"
#define STRM_HSM_2923_112_2_17_23_43_13_3050 "#STRM DeleteHsmFromList no list exists!"
#define STRM_HSM_2942_112_2_17_23_43_13_3051 "#STRM DeleteHsmFromList node not exists!"
#define STRM_HSM_2971_112_2_17_23_43_13_3052 "#STRM CheckIfHsmValid no list exists!"
#define STRM_HSM_3005_112_2_17_23_43_13_3053 "#STRM STRMHSM_GetHandleByRtspHandle no list exists!"
#define STRM_HSM_3039_112_2_17_23_43_13_3054 "#STRM STRMHSM_GetRequestUrl input invalid!"
#define STRM_HSM_3057_112_2_17_23_43_13_3055 "#STRM STRMHSM_GetHandleByRtpHandle no list exists!"
#define STRM_HSM_3095_112_2_17_23_43_13_3056 "#STRM STRMHSM_GetHandleByRtpHandle no list exists!"
#define STRM_HSM_3132_112_2_17_23_43_13_3057 "#STRM STRMHSM_GetEncType input invalid!"
#define STRM_HSM_3160_112_2_17_23_43_13_3058 "#STRM STRMHSM_GetEncType input invalid!"
#define STRM_HSM_3182_112_2_17_23_43_13_3059 "#STRM STRMHSM_CreatHsm!"
#define STRM_HSM_3186_112_2_17_23_43_13_3060 "#STRM STRMHSM_CreatHsm input invalid!"
#define STRM_HSM_3192_112_2_17_23_43_13_3061 "#STRM STRMHSM_CreatHsm hsm_ptr alloc fail!"
#define STRM_HSM_3198_112_2_17_23_43_13_3062 "#STRM STRMHSM_CreatHsm StrmHsmConstructor fail!"
#define STRM_HSM_3209_112_2_17_23_43_13_3063 "#STRM STRMHSM_CreatHsm error input, url==0!"
#define STRM_HSM_3212_112_2_17_23_43_13_3064 "#STRM STRMHSM_CreatHsm input, url: %s"
#define STRM_HSM_3217_112_2_17_23_43_13_3065 "#STRM STRMHSM_CreatHsm url_ptr alloc fail!"
#define STRM_HSM_3234_112_2_17_23_43_13_3066 "#STRM STRMHSM_CreatHsm, unsupported URL scheme!"
#define STRM_HSM_3246_112_2_17_23_43_13_3067 "#STRM STRMHSM_CreatHsm, alloc sdp buffer fail!"
#define STRM_HSM_3256_112_2_17_23_43_13_3068 "#STRM STRMHSM_CreatHsm, no sdp file input while using http!"
#define STRM_HSM_3267_112_2_17_23_43_13_3069 "#STRM STRMHSM_CreatHsm user_agent_ptr alloc fail!"
#define STRM_HSM_3277_112_2_17_23_43_13_3070 "#STRM STRMHSM_CreatHsm uaprof_ptr alloc fail!"
#define STRM_HSM_3287_112_2_17_23_43_13_3071 "#STRM STRMHSM_CreatHsm proxy.host_ptr alloc fail!"
#define STRM_HSM_3298_112_2_17_23_43_13_3072 "#STRM STRMHSM_CreatHsm proxy.username_ptr alloc fail!"
#define STRM_HSM_3308_112_2_17_23_43_13_3073 "#STRM STRMHSM_CreatHsm proxy.password_ptr alloc fail!"
#define STRM_HSM_3320_112_2_17_23_43_13_3074 "#STRM STRMHSM_CreatHsm auth_info.username_ptr alloc fail!"
#define STRM_HSM_3329_112_2_17_23_43_13_3075 "#STRM STRMHSM_CreatHsm auth_info.password_ptr alloc fail!"
#define STRM_HSM_3338_112_2_17_23_43_13_3076 "#STRM STRMHSM_CreatHsm creat buffer fail!"
#define STRM_HSM_3343_112_2_17_23_43_13_3077 "#STRM STRMHSM_CreatHsm add to list fail!"
#define STRM_HSM_3361_112_2_17_23_43_13_3078 "#STRM STRMHSM_FreeHsm!"
#define STRM_HSM_3365_112_2_17_23_43_13_3079 "#STRM STRMHSM_FreeHsm input invalid!"
#define STRM_HSM_3481_112_2_17_23_43_14_3080 "#STRM STRMHSM_GetAudioPayload input invalid!"
#define STRM_HSM_3497_112_2_17_23_43_14_3081 "#STRM STRMHSM_GetVideoPayload input invalid!"
#define STRM_HSM_3513_112_2_17_23_43_14_3082 "#STRM STRMHSM_CheckIfCanRecvRtpPack input invalid!"
#define STRM_HSM_3525_112_2_17_23_43_14_3083 "#STRM STRMHSM_CheckIfCanRecvRtpPack strm_handle=0x%x Fail!"
#define STRM_HSM_3538_112_2_17_23_43_14_3084 "#STRM STRMHSM_GetNetId input invalid!"
#define STRM_HSM_3555_112_2_17_23_43_14_3085 "#STRM STRMHSM_GetSdpInfo input invalid!"
#define STRM_HSM_3589_112_2_17_23_43_14_3086 "#STRM StrmThreadEntry, strm_handle=0x%x, sig_code=0x%x"
#define STRM_HSM_3621_112_2_17_23_43_14_3087 "#STRM StrmThreadEntry, invalid signal"
#define STRM_HSM_3630_112_2_17_23_43_14_3088 "#STRM StrmThreadEntry, invalid strm_handle!"
#define STRM_HSM_3648_112_2_17_23_43_14_3089 "#STRM STRMHSM_Dispatch err sig_ptr==0!"
#define STRM_HSM_3676_112_2_17_23_43_14_3090 "#STRM STRMHSM_GetDataBufferTotalTime, input error"
#define STRM_HSM_3708_112_2_17_23_43_14_3091 "#STRM STRMHSM_GetDataBufferTotalTime, STRM_SDP_ENC_MP4A_LATM audio empty list"
#define STRM_HSM_3716_112_2_17_23_43_14_3092 "#STRM STRMHSM_GetDataBufferTotalTime, STRM_SDP_ENC_MP4A_LATM audio empty list"
#define STRM_HSM_3728_112_2_17_23_43_14_3093 "#STRM STRMHSM_GetDataBufferTotalTime, audio empty list"
#define STRM_HSM_3743_112_2_17_23_43_14_3094 "#STRM STRMHSM_GetDataBufferTotalTime, audio buf_ptr->audio_buf.initial_timestamp= %d, first_time =%d last_time =%d, sample_rate=%d"
#define STRM_HSM_3770_112_2_17_23_43_14_3095 "#STRM STRMHSM_GetDataBufferTotalTime STRM_SDP_ENC_H264, video empty list"
#define STRM_HSM_3788_112_2_17_23_43_14_3096 "#STRM STRMHSM_GetDataBufferTotalTime, video empty list"
#define STRM_HSM_3803_112_2_17_23_43_14_3097 "#STRM STRMHSM_GetDataBufferTotalTime, video buf_ptr->video_buf.initial_timestamp= %d, packet_in_list_ptr->timestamp =%d last_pack_ptr->timestamp =%d, sample_rate=%d"
#define STRM_HSM_3819_112_2_17_23_43_14_3098 "#STRM STRMHSM_GetDataBufferTotalTime,timestamp_size=%d"
#define STRM_HSM_3846_112_2_17_23_43_14_3099 "#STRM STRMRTP_SRRRSendMode input invalid!"
#define STRM_HSM_3856_112_2_17_23_43_14_3100 "#STRM STRM_SendRRPacket session1, get remote address fail!"
#define STRM_HSM_3865_112_2_17_23_43_14_3101 "#STRM STRM_SendRRPacket session1 rtp function err!"
#define STRM_HSM_3876_112_2_17_23_43_14_3102 "#STRM STRM_SendRRPacket session2, get remote address fail!"
#define STRM_HSM_3885_112_2_17_23_43_14_3103 "#STRM STRM_SendRRPacket session2 rtp function err!"
#define STRM_RTP_128_112_2_17_23_43_15_3104 "#STRM STRMRTP_CreatSession input invalid!"
#define STRM_RTP_131_112_2_17_23_43_15_3105 "#STRM creat RTP session, net_id:%d"
#define STRM_RTP_139_112_2_17_23_43_15_3106 "#STRM STRMRTP_CreatSession rtp function err!"
#define STRM_RTP_147_112_2_17_23_43_15_3107 "#STRM creat RTP session, rtcp port:%d, rtp port:%d, rtp handle:0x%x"
#define STRM_RTP_168_112_2_17_23_43_15_3108 "#STRM STRMRTP_CloseSession rtp function err!"
#define STRM_RTP_202_112_2_17_23_43_15_3109 "#STRM STRMRTP_SendRtcpNaduPacket input invalid!"
#define STRM_RTP_212_112_2_17_23_43_15_3110 "#STRM STRMRTP_SendNadu, get remote address fail!"
#define STRM_RTP_248_112_2_17_23_43_15_3111 "#STRM STRMRTP_SendRtcpNaduPacket rtp function err!"
#define STRM_RTP_268_112_2_17_23_43_15_3112 "#STRM RecvDataCb, error input!"
#define STRM_RTP_274_112_2_17_23_43_15_3113 "#STRM RecvDataCb, error strm_handle!"
#define STRM_RTP_280_112_2_17_23_43_15_3114 "#STRM RecvDataCb, wrong state, ignore packet!"
#define STRM_RTP_294_112_2_17_23_43_15_3115 "#STRM RecvDataCb, error payload_type!"
#define STRM_RTP_300_112_2_17_23_43_15_3116 "#STRM RecvDataCb, UnpackPayloadAndInsert error!"
#define STRM_RTP_326_112_2_17_23_43_15_3117 "#STRM UnpackPayloadAndInsert, get sdp info fail!"
#define STRM_RTP_334_112_2_17_23_43_15_3118 "#STRM UnpackPayloadAndInsert UnpackAmrAndInsert FAIL!"
#define STRM_RTP_341_112_2_17_23_43_15_3119 "#STRM UnpackPayloadAndInsert UnpackH263AndInsert FAIL!"
#define STRM_RTP_348_112_2_17_23_43_15_3120 "#STRM UnpackPayloadAndInsert UnpackH263plusAndInsert FAIL!"
#define STRM_RTP_355_112_2_17_23_43_15_3121 "#STRM UnpackPayloadAndInsert UnpackH264AndInsert FAIL!"
#define STRM_RTP_363_112_2_17_23_43_15_3122 "#STRM UnpackPayloadAndInsert UnpackMP4AndInsert FAIL!"
#define STRM_RTP_370_112_2_17_23_43_15_3123 "#STRM UnpackPayloadAndInsert UnpackMP4AndInsert FAIL!"
#define STRM_RTP_375_112_2_17_23_43_15_3124 "#STRM UnpackPayloadAndInsert unsupport enc_type!"
#define STRM_RTP_403_112_2_17_23_43_15_3125 "#STRM UnpackAmrAndInsert input invalid!"
#define STRM_RTP_419_112_2_17_23_43_15_3126 "#STRM UnpackAmr RTPPL_AmrUnpackConstruct fail:%d"
#define STRM_RTP_435_112_2_17_23_43_15_3127 "#STRM RTPPL frame alloc fail"
#define STRM_RTP_452_112_2_17_23_43_15_3128 "#STRM UnpackAmrAndInsert insert fail!"
#define STRM_RTP_488_112_2_17_23_43_16_3129 "#STRM UnpackH263AndInsert input invalid!"
#define STRM_RTP_496_112_2_17_23_43_16_3130 "#STRM UnpackH263AndInsert fail:%d"
#define STRM_RTP_517_112_2_17_23_43_16_3131 "#STRM UnpackH263AndInsert insert fail!"
#define STRM_RTP_543_112_2_17_23_43_16_3132 "#STRM UnpackH263plusAndInsert input invalid!"
#define STRM_RTP_552_112_2_17_23_43_16_3133 "#STRM UnpackH263plusAndInsert RTPPL_H263PlusUnpackConstruct fail:%d"
#define STRM_RTP_570_112_2_17_23_43_16_3134 "#STRM UnpackH263plusAndInsert insert fail!"
#define STRM_RTP_601_112_2_17_23_43_16_3135 "#STRM UnpackH264AndInsert input invalid!"
#define STRM_RTP_611_112_2_17_23_43_16_3136 "#STRM UnpackH264AndInsert RTPPL_H264UnpackConstruct fail:%d"
#define STRM_RTP_627_112_2_17_23_43_16_3137 "#STRM h264 nalu param alloc fail"
#define STRM_RTP_657_112_2_17_23_43_16_3138 "#STRM UnpackH264AndInsert insert fail!"
#define STRM_RTP_694_112_2_17_23_43_16_3139 "#STRM UnpackMP4AndInsert input invalid!"
#define STRM_RTP_711_112_2_17_23_43_16_3140 "#STRM UnpackMP4AndInsert insert fail!"
#define STRM_RTP_743_112_2_17_23_43_16_3141 "#STRM UnpackMPEG4AndInsert input invalid!"
#define STRM_RTP_757_112_2_17_23_43_16_3142 "#STRM UnpackMPEG4AndInsert unknown payload_type!"
#define STRM_RTP_779_112_2_17_23_43_16_3143 "#STRM UnpackMPEG4AndInsert RTPPL_H264UnpackConstruct fail:%d"
#define STRM_RTP_795_112_2_17_23_43_16_3144 "#STRM h264 nalu param alloc fail"
#define STRM_RTP_818_112_2_17_23_43_16_3145 "#STRM UnpackMPEG4AndInsert insert fail: au_size=0x%x!"
#define STRM_RTP_848_112_2_17_23_43_16_3146 "#STRM AdapSrrrSendInd input invalid!"
#define STRM_RTP_855_112_2_17_23_43_16_3147 "#STRM AdapSrrrSendInd, wrong strm_handle!"
#define STRM_RTP_861_112_2_17_23_43_16_3148 "#STRM AdapSrrrSendInd, error when creat signal!"
#define STRM_RTP_882_112_2_17_23_43_16_3149 "#STRM AdapGoodbyeInd input invalid!"
#define STRM_RTP_889_112_2_17_23_43_16_3150 "#STRM AdapGoodbyeInd, wrong strm_handle!"
#define STRM_RTP_895_112_2_17_23_43_16_3151 "#STRM AdapGoodbyeInd, error when creat signal!"
#define STRM_RTP_916_112_2_17_23_43_16_3152 "#STRM STRMRTP_SigAdap input invalid!"
#define STRM_RTP_944_112_2_17_23_43_16_3153 "#STRM STRMRTP_SRRRSendMode input invalid!"
#define STRM_RTP_949_112_2_17_23_43_17_3154 "#STRM STRMRTP_SRRRSendMode rtp_error_code =%d"
#define STRM_RTSP_266_112_2_17_23_43_17_3155 "#STRM STRMRTSP_InitSessionReq input invalid!"
#define STRM_RTSP_284_112_2_17_23_43_17_3156 "#STRM STRMRTSP_InitSessionReq, RTSP function error"
#define STRM_RTSP_302_112_2_17_23_43_17_3157 "#STRM AdapInitCnf input invalid!"
#define STRM_RTSP_308_112_2_17_23_43_17_3158 "#STRM AdapInitCnf, alloc adap signal fail!"
#define STRM_RTSP_337_112_2_17_23_43_17_3159 "#STRM CombineTransportHeadValue, alloc fail!"
#define STRM_RTSP_370_112_2_17_23_43_17_3160 "#STRM Combine3gppAdaptationHeadValue, alloc fail!"
#define STRM_RTSP_410_112_2_17_23_43_18_3161 "#STRM CombineRangeHeadValue ALLOC fail!"
#define STRM_RTSP_451_112_2_17_23_43_18_3162 "#STRM STRMRTSP_DescribeReq input invalid!"
#define STRM_RTSP_475_112_2_17_23_43_18_3163 "#STRM STRMRTSP_DescribeReq, alloc head ptr fail!"
#define STRM_RTSP_499_112_2_17_23_43_18_3164 "#STRM STRMRTSP_DescribeReq, RTSP function error"
#define STRM_RTSP_525_112_2_17_23_43_18_3165 "#STRM GetBaseUrlFromDescribeCnf input invalid!"
#define STRM_RTSP_532_112_2_17_23_43_18_3166 "#STRM GetBaseUrlFromDescribeCnf head_len==0!"
#define STRM_RTSP_538_112_2_17_23_43_18_3167 "#STRM GetBaseUrlFromDescribeCnf copy head fail!"
#define STRM_RTSP_565_112_2_17_23_43_18_3168 "#STRM GetBaseUrlFromDescribeCnf error head content base!"
#define STRM_RTSP_592_112_2_17_23_43_18_3169 "#STRM GetBaseUrlFromDescribeCnf error head content location!"
#define STRM_RTSP_624_112_2_17_23_43_18_3170 "#STRM AdapDescribeCnf input invalid!"
#define STRM_RTSP_631_112_2_17_23_43_18_3171 "#STRM AdapDescribeCnf, wrong strm_handle!"
#define STRM_RTSP_647_112_2_17_23_43_18_3172 "#STRM AdapDescribeCnf, alloc sdp buffer fail!"
#define STRM_RTSP_662_112_2_17_23_43_18_3173 "#STRM AdapDescribeCnf, describe cnf is STRM_RESULT_UNSUPPORTED!"
#define STRM_RTSP_668_112_2_17_23_43_18_3174 "#STRM AdapDescribeCnf, describe cnf is not RTSP_SUCCESS!"
#define STRM_RTSP_675_112_2_17_23_43_18_3175 "#STRM AdapDescribeCnf, serialize fail!"
#define STRM_RTSP_715_112_2_17_23_43_18_3176 "#STRM STRMRTSP_SetupReq input invalid!"
#define STRM_RTSP_747_112_2_17_23_43_18_3177 "#STRM STRMRTSP_SetupReq, alloc head ptr fail!"
#define STRM_RTSP_775_112_2_17_23_43_18_3178 "#STRM STRMRTSP_SetupReq, RTSP function error"
#define STRM_RTSP_809_112_2_17_23_43_18_3179 "#STRM AdapSetupCnf input invalid!"
#define STRM_RTSP_816_112_2_17_23_43_18_3180 "#STRM AdapSetupCnf, wrong strm_handle!"
#define STRM_RTSP_859_112_2_17_23_43_18_3181 "#STRM STRMRTSP_PlayReq input invalid!"
#define STRM_RTSP_885_112_2_17_23_43_18_3182 "#STRM STRMRTSP_PlayReq, alloc head ptr fail!"
#define STRM_RTSP_907_112_2_17_23_43_19_3183 "#STRM STRMRTSP_PlayReq, RTSP function error"
#define STRM_RTSP_937_112_2_17_23_43_19_3184 "#STRM GetPlayTimeRange input invalid"
#define STRM_RTSP_952_112_2_17_23_43_19_3185 "#STRM GetPlayTimeRange alloc mem fail"
#define STRM_RTSP_961_112_2_17_23_43_19_3186 "#STRM GetPlayTimeRange, Range:%s, start_time:%d, end_time:%d"
#define STRM_RTSP_983_112_2_17_23_43_19_3187 "#STRM AdapPlayCnf input invalid"
#define STRM_RTSP_990_112_2_17_23_43_19_3188 "#STRM AdapPlayCnf, wrong strm_handle!"
#define STRM_RTSP_997_112_2_17_23_43_19_3189 "#STRM AdapPlayCnf, error when creat signal!"
#define STRM_RTSP_1029_112_2_17_23_43_19_3190 "#STRM STRMRTSP_PauseReq input invalid"
#define STRM_RTSP_1045_112_2_17_23_43_19_3191 "#STRM STRMRTSP_PauseReq, alloc head ptr fail!"
#define STRM_RTSP_1057_112_2_17_23_43_19_3192 "#STRM STRMRTSP_PauseReq, RTSP function error"
#define STRM_RTSP_1078_112_2_17_23_43_19_3193 "#STRM AdapPauseCnf input invalid"
#define STRM_RTSP_1085_112_2_17_23_43_19_3194 "#STRM AdapPauseCnf, wrong strm_handle!"
#define STRM_RTSP_1092_112_2_17_23_43_19_3195 "#STRM AdapPauseCnf, error when creat signal!"
#define STRM_RTSP_1122_112_2_17_23_43_19_3196 "#STRM STRMRTSP_TearDownReq input invalid"
#define STRM_RTSP_1139_112_2_17_23_43_19_3197 "#STRM STRMRTSP_SetupReq, alloc head ptr fail!"
#define STRM_RTSP_1152_112_2_17_23_43_19_3198 "#STRM STRMRTSP_TearDownReq, RTSP function error"
#define STRM_RTSP_1175_112_2_17_23_43_19_3199 "#STRM AdapTearDownCnf input invalid"
#define STRM_RTSP_1182_112_2_17_23_43_19_3200 "#STRM AdapTearDownCnf, wrong strm_handle!"
#define STRM_RTSP_1189_112_2_17_23_43_19_3201 "#STRM AdapTearDownCnf, error when creat signal!"
#define STRM_RTSP_1219_112_2_17_23_43_19_3202 "#STRM AdapSetParamCnf input invalid"
#define STRM_RTSP_1226_112_2_17_23_43_19_3203 "#STRM AdapSetParamCnf, wrong strm_handle!"
#define STRM_RTSP_1233_112_2_17_23_43_19_3204 "#STRM AdapTearDownCnf, error when creat signal!"
#define STRM_RTSP_1263_112_2_17_23_43_19_3205 "#STRM STRMRTSP_CloseSessionReq input invalid"
#define STRM_RTSP_1276_112_2_17_23_43_19_3206 "#STRM STRMRTSP_CloseSessionReq, rtsp function err!"
#define STRM_RTSP_1295_112_2_17_23_43_19_3207 "#STRM AdapCloseCnf input invalid"
#define STRM_RTSP_1302_112_2_17_23_43_19_3208 "#STRM AdapCloseCnf, wrong strm_handle!"
#define STRM_RTSP_1309_112_2_17_23_43_19_3209 "#STRM AdapCloseCnf, error when creat signal!"
#define STRM_RTSP_1338_112_2_17_23_43_19_3210 "#STRM AdapDescribeAuthInd input invalid"
#define STRM_RTSP_1345_112_2_17_23_43_19_3211 "#STRM AdapDescribeAuthInd, wrong strm_handle!"
#define STRM_RTSP_1351_112_2_17_23_43_19_3212 "#STRM AdapDescribeAuthInd, error when creat signal!"
#define STRM_RTSP_1373_112_2_17_23_43_19_3213 "#STRM STRMRTSP_DescribeAuthRsp input invalid"
#define STRM_RTSP_1386_112_2_17_23_43_20_3214 "#STRM STRMRTSP_DescribeAuthRsp, RTSP function error!"
#define STRM_RTSP_1404_112_2_17_23_43_20_3215 "#STRM AdapSetupAuthInd input invalid"
#define STRM_RTSP_1411_112_2_17_23_43_20_3216 "#STRM AdapSetupAuthInd, wrong strm_handle!"
#define STRM_RTSP_1417_112_2_17_23_43_20_3217 "#STRM AdapSetupAuthInd, error when creat signal!"
#define STRM_RTSP_1438_112_2_17_23_43_20_3218 "#STRM AdapPlayAuthInd input invalid"
#define STRM_RTSP_1445_112_2_17_23_43_20_3219 "#STRM AdapPlayAuthInd, wrong strm_handle!"
#define STRM_RTSP_1451_112_2_17_23_43_20_3220 "#STRM AdapPlayAuthInd, error when creat signal!"
#define STRM_RTSP_1474_112_2_17_23_43_20_3221 "#STRM STRMRTSP_SetupAuthRsp input invalid"
#define STRM_RTSP_1487_112_2_17_23_43_20_3222 "#STRM STRMRTSP_SetupAuthRsp, RTSP function error!"
#define STRM_RTSP_1508_112_2_17_23_43_20_3223 "#STRM STRMRTSP_PlayAuthRsp input invalid"
#define STRM_RTSP_1521_112_2_17_23_43_20_3224 "#STRM STRMRTSP_PlayAuthRsp, RTSP function error!"
#define STRM_RTSP_1539_112_2_17_23_43_20_3225 "#STRM STRMRTSP_SigAdap input invalid"
#define STRM_SDP_269_112_2_17_23_43_21_3226 "#STRM STRMSDP_Parse input invalid"
#define STRM_SDP_299_112_2_17_23_43_21_3227 "#STRM STRMSDP_Parse parse 3GPP-Adaptation-Support fail!"
#define STRM_SDP_305_112_2_17_23_43_21_3228 "#STRM STRMSDP_Parse parse session info fail!"
#define STRM_SDP_312_112_2_17_23_43_21_3229 "#STRM STRMSDP_Parse parse session name fail!"
#define STRM_SDP_322_112_2_17_23_43_21_3230 "#STRM STRMSDP_Parse GetAudioParam fail!"
#define STRM_SDP_329_112_2_17_23_43_21_3231 "#STRM STRMSDP_Parse GetVideoParam fail!"
#define STRM_SDP_336_112_2_17_23_43_21_3232 "#STRM STRMSDP_Parse session end:0x%x, audio_end:0x%x, video_end:0x%x"
#define STRM_SDP_387_112_2_17_23_43_21_3233 "#STRM STRMSDP_CloneInfo input invalid"
#define STRM_SDP_411_112_2_17_23_43_21_3234 "#STRM STRMSDP_CloneInfo clone audio_type_name_ptr FAIL!"
#define STRM_SDP_422_112_2_17_23_43_21_3235 "#STRM STRMSDP_CloneInfo audio_param.config_ptr ALLOC FAIL!"
#define STRM_SDP_435_112_2_17_23_43_21_3236 "#STRM STRMSDP_CloneInfo clone video_type_name_ptr FAIL!"
#define STRM_SDP_446_112_2_17_23_43_21_3237 "#STRM STRMSDP_CloneInfo video_param.config_ptr ALLOC FAIL!"
#define STRM_SDP_458_112_2_17_23_43_21_3238 "#STRM STRMSDP_CloneInfo audio_param.url_ptr clone FAIL!"
#define STRM_SDP_467_112_2_17_23_43_21_3239 "#STRM STRMSDP_CloneInfo video_param.url_ptr clone FAIL!"
#define STRM_SDP_476_112_2_17_23_43_21_3240 "#STRM STRMSDP_CloneInfo media_name_ptr clone FAIL!"
#define STRM_SDP_485_112_2_17_23_43_21_3241 "#STRM STRMSDP_CloneInfo session_info_ptr clone FAIL!"
#define STRM_SDP_498_112_2_17_23_43_21_3242 "#STRM STRMSDP_CloneInfo sprop_sets_ptr CFL_ALLOC  FAIL!"
#define STRM_SDP_522_112_2_17_23_43_21_3243 "#STRM STRMSDP_FreeSdpInfo input invalid"
#define STRM_SDP_593_112_2_17_23_43_21_3244 "#STRM GetNaduReportRate input invalid"
#define STRM_SDP_604_112_2_17_23_43_21_3245 "#STRM GetSessionInfo, descr_num==0, wrong SDP!"
#define STRM_SDP_625_112_2_17_23_43_21_3246 "#STRM GetNaduReportRate, doesn't support audio 3gpp adaptation"
#define STRM_SDP_637_112_2_17_23_43_21_3247 "#STRM GetNaduReportRate, doesn't support video 3gpp adaptation"
#define STRM_SDP_657_112_2_17_23_43_21_3248 "#STRM GetSessionInfo input invalid"
#define STRM_SDP_666_112_2_17_23_43_21_3249 "#STRM GetSessionInfo, string clone fail!"
#define STRM_SDP_686_112_2_17_23_43_21_3250 "#STRM GetSessionName input invalid"
#define STRM_SDP_695_112_2_17_23_43_21_3251 "#STRM GetSessionName, string clone fail!"
#define STRM_SDP_722_112_2_17_23_43_21_3252 "#STRM GetUrlInDescrp input invalid"
#define STRM_SDP_730_112_2_17_23_43_22_3253 "#STRM GetUrlInDescrp, get URL: %s"
#define STRM_SDP_734_112_2_17_23_43_22_3254 "#STRM GetUrlInDescrp, the URL is none or no control attribute"
#define STRM_SDP_743_112_2_17_23_43_22_3255 "#STRM GetUrlInDescrp, CLONE URL fail!"
#define STRM_SDP_752_112_2_17_23_43_22_3256 "#STRM GetUrlInDescrp, no base URL, cannot generate URL!"
#define STRM_SDP_761_112_2_17_23_43_22_3257 "#STRM GetUrlInDescrp, ALLOC URL fail!"
#define STRM_SDP_799_112_2_17_23_43_22_3258 "#STRM STRMSDP_GetTimeRange input invalid"
#define STRM_SDP_806_112_2_17_23_43_22_3259 "#STRM STRMSDP_GetTimeRange, unsupported range format: %s"
#define STRM_SDP_822_112_2_17_23_43_22_3260 "#STRM STRMSDP_GetTimeRange, clone fail"
#define STRM_SDP_854_112_2_17_23_43_22_3261 "#STRM STRMSDP_GetTimeRange, illegal form of range"
#define STRM_SDP_887_112_2_17_23_43_22_3262 "#STRM GetSessionRange input invalid"
#define STRM_SDP_895_112_2_17_23_43_22_3263 "#STRM GetSessionRange, Range:%s, start_time:%d, end_time:%d"
#define STRM_SDP_918_112_2_17_23_43_22_3264 "#STRM GetMediaRange input invalid"
#define STRM_SDP_926_112_2_17_23_43_22_3265 "#STRM GetMediaRange, Range:%s, start_time:%d, end_time:%d"
#define STRM_SDP_948_112_2_17_23_43_22_3266 "#STRM GetTimeInMsFromTimeString input invalid"
#define STRM_SDP_960_112_2_17_23_43_22_3267 "#STRM GetTimeInMsFromTimeString, WRONG time format!"
#define STRM_SDP_1004_112_2_17_23_43_22_3268 "#STRM IsAllDigit input invalid"
#define STRM_SDP_1031_112_2_17_23_43_22_3269 "#STRM IsAllHex input invalid"
#define STRM_SDP_1069_112_2_17_23_43_22_3270 "#STRM GetAudioParam input invalid"
#define STRM_SDP_1076_112_2_17_23_43_22_3271 "#STRM GetAudioParam, descr_num==0, wrong SDP!"
#define STRM_SDP_1096_112_2_17_23_43_22_3272 "#STRM GetAudioParam, unsupported protocol:%s"
#define STRM_SDP_1103_112_2_17_23_43_22_3273 "#STRM GetAudioParam, wrong rtpmap number:%d"
#define STRM_SDP_1108_112_2_17_23_43_22_3274 "#STRM GetAudioParam, GetRtpMapInfo fail"
#define STRM_SDP_1136_112_2_17_23_43_22_3275 "#STRM GetAudioParam, GetUrlInDescrp fail"
#define STRM_SDP_1144_112_2_17_23_43_22_3276 "#STRM GetAudioParam, GetFmtpInfo fail"
#define STRM_SDP_1155_112_2_17_23_43_22_3277 "#STRM GetAudioParam, no audio exists!"
#define STRM_SDP_1171_112_2_17_23_43_22_3278 "#STRM GetEncType input invalid"
#define STRM_SDP_1224_112_2_17_23_43_23_3279 "#STRM FindStrBefChar input invalid"
#define STRM_SDP_1286_112_2_17_23_43_23_3280 "#STRM GetFramesizeInfo input invalid"
#define STRM_SDP_1294_112_2_17_23_43_23_3281 "#STRM GetFramesizeInfo, no framesize exists"
#define STRM_SDP_1302_112_2_17_23_43_23_3282 "#STRM GetFramesizeInfo, no memory allocates"
#define STRM_SDP_1373_112_2_17_23_43_23_3283 "#STRM GetFramerateInfo input invalid"
#define STRM_SDP_1381_112_2_17_23_43_23_3284 "#STRM GetFramerateInfo, no framerate exists"
#define STRM_SDP_1389_112_2_17_23_43_23_3285 "#STRM GetFramerateInfo, no memory allocates"
#define STRM_SDP_1456_112_2_17_23_43_23_3286 "#STRM GetVideoParam input invalid"
#define STRM_SDP_1463_112_2_17_23_43_23_3287 "#STRM GetVideoParam, descr_num==0, wrong SDP!"
#define STRM_SDP_1483_112_2_17_23_43_23_3288 "#STRM GetVideoParam, unsupported protocol:%s"
#define STRM_SDP_1490_112_2_17_23_43_23_3289 "#STRM GetVideoParam, wrong rtpmap number:%d"
#define STRM_SDP_1495_112_2_17_23_43_23_3290 "#STRM GetVideoParam, GetRtpMapInfo fail"
#define STRM_SDP_1510_112_2_17_23_43_23_3291 "#STRM GetVideoParam, GetUrlInDescrp fail"
#define STRM_SDP_1516_112_2_17_23_43_23_3292 "#STRM GetVideoParam, GetFmtpInfo fail"
#define STRM_SDP_1525_112_2_17_23_43_23_3293 "#STRM GetFramerateInfo, GetFramesizeInfo fail"
#define STRM_SDP_1532_112_2_17_23_43_23_3294 "#STRM GetFramerateInfo, GetFramerateInfo fail"
#define STRM_SDP_1542_112_2_17_23_43_23_3295 "#STRM GetVideoParam, no video exists!"
#define STRM_SDP_1563_112_2_17_23_43_23_3296 "#STRM GetRtpMapInfo input invalid"
#define STRM_SDP_1571_112_2_17_23_43_23_3297 "#STRM GetRtpMapInfo, get encode name fail!"
#define STRM_SDP_1577_112_2_17_23_43_23_3298 "#STRM GetRtpMapInfo, clone encode name fail!"
#define STRM_SDP_1584_112_2_17_23_43_23_3299 "#STRM GetRtpMapInfo, wrong payload num: %d"
#define STRM_SDP_1591_112_2_17_23_43_23_3300 "#STRM GetRtpMapInfo, enc_name: %s, payload: %d, clock_rate: %d"
#define STRM_SDP_1599_112_2_17_23_43_23_3301 "#STRM GetRtpMapInfo, clone encode param fail!"
#define STRM_SDP_1616_112_2_17_23_43_23_3302 "#STRM GetRtpMapInfo, clone encode param fail!"
#define STRM_SDP_1672_112_2_17_23_43_23_3303 "#STRM GetConfigInfo input invalid"
#define STRM_SDP_1678_112_2_17_23_43_23_3304 "#STRM GetConfigInfo, no fmtp exists"
#define STRM_SDP_1689_112_2_17_23_43_23_3305 "#STRM GetConfigInfo, no fmtp exists"
#define STRM_SDP_1723_112_2_17_23_43_24_3306 "#STRM GetFmtpInfo input invalid"
#define STRM_SDP_1729_112_2_17_23_43_24_3307 "#STRM GetFmtpInfo, no fmtp exists"
#define STRM_SDP_1739_112_2_17_23_43_24_3308 "#STRM GetFmtpInfo GetFmtpAmrParam fail!"
#define STRM_SDP_1750_112_2_17_23_43_24_3309 "#STRM GetFmtpInfo GetFmtpH264Param fail!"
#define STRM_SDP_1761_112_2_17_23_43_24_3310 "#STRM GetFmtpInfo, unsupported encode type!"
#define STRM_SDP_1785_112_2_17_23_43_24_3311 "#STRM GetFmtpParam input invalid"
#define STRM_SDP_1795_112_2_17_23_43_24_3312 "#STRM GetFmtpParam, WRONG fmtp parameter format!"
#define STRM_SDP_1819_112_2_17_23_43_24_3313 "#STRM GetFmtpH264Param input invalid"
#define STRM_SDP_1839_112_2_17_23_43_24_3314 "#STRM GetFmtpAmrParam input invalid"
#define STRM_SDP_1868_112_2_17_23_43_24_3315 "#STRM GetFmtpMPEG4GenericParam input invalid"
#define STRM_SDP_1942_112_2_17_23_43_24_3316 "#STRM HexStr2Bin input invalid"
#define STRM_SDP_1973_112_2_17_23_43_24_3317 "#STRM GetFmtpH264ProLevelId input invalid"
#define STRM_SDP_1979_112_2_17_23_43_24_3318 "#STRM GetFmtpH264ProLevelId, copied_fmtp_str_ptr clone fail!"
#define STRM_SDP_1987_112_2_17_23_43_24_3319 "#STRM GetFmtpH264ProLevelId, wrong profile-level-id length!"
#define STRM_SDP_1996_112_2_17_23_43_24_3320 "#STRM GetFmtpH264ProLevelId, no profile level id exists!"
#define STRM_SDP_2013_112_2_17_23_43_24_3321 "#STRM GetFmtpSpropSetsNum input invalid"
#define STRM_SDP_2041_112_2_17_23_43_24_3322 "#STRM GetFmtpSpropSetsArr input invalid"
#define STRM_SDP_2082_112_2_17_23_43_24_3323 "#STRM GetFmtpSpropSets input invalid"
#define STRM_SDP_2088_112_2_17_23_43_24_3324 "#STRM GetFmtpSpropSets, copied_fmtp_str_ptr clone fail!"
#define STRM_SDP_2094_112_2_17_23_43_24_3325 "#STRM GetFmtpSpropSets, no sprop-parameter-sets exists!"
#define STRM_SDP_2100_112_2_17_23_43_24_3326 "#STRM GetFmtpSpropSets, error sets number: %d"
#define STRM_SDP_2127_112_2_17_23_43_24_3327 "#STRM GetFmtpSpropSets, CFLBASE64_Decode fail!"
#define STRM_SDP_2145_112_2_17_23_43_24_3328 "#STRM GetFmtpSpropSets, alloc all_sprop_sets_ptr fail!"
#define STRM_SDP_2196_112_2_17_23_43_25_3329 "#STRM GetFmtpOctetAlign input invalid"
#define STRM_SDP_2203_112_2_17_23_43_25_3330 "#STRM GetFmtpOctetAlign, copied_fmtp_str_ptr clone fail!"
#define STRM_SDP_2226_112_2_17_23_43_25_3331 "#STRM GetFmtpOctetAlign, wrong octet-align value:%d, set to default!"
#define STRM_SDP_2232_112_2_17_23_43_25_3332 "#STRM GetFmtpOctetAlign, wrong octet-align format, set to default value!"
#define STRM_SDP_2253_112_2_17_23_43_25_3333 "#STRM GetFmtpCrc input invalid"
#define STRM_SDP_2260_112_2_17_23_43_25_3334 "#STRM GetFmtpCrc, copied_fmtp_str_ptr clone fail!"
#define STRM_SDP_2276_112_2_17_23_43_25_3335 "#STRM GetFmtpCrc, wrong crc value:%d, set to default!"
#define STRM_SDP_2282_112_2_17_23_43_25_3336 "#STRM GetFmtpCrc, wrong crc format, set to default value!"
#define STRM_SDP_2303_112_2_17_23_43_25_3337 "#STRM GetFmtpRobustSorting input invalid"
#define STRM_SDP_2310_112_2_17_23_43_25_3338 "#STRM GetFmtpRobustSorting, copied_fmtp_str_ptr clone fail!"
#define STRM_SDP_2326_112_2_17_23_43_25_3339 "#STRM GetFmtpRobustSorting, wrong robust_sorting value:%d, set to default!"
#define STRM_SDP_2332_112_2_17_23_43_25_3340 "#STRM GetFmtpRobustSorting, wrong robust_sorting format, set to default value!"
#define STRM_SDP_2353_112_2_17_23_43_25_3341 "#STRM GetFmtpInterleaving input invalid"
#define STRM_SDP_2360_112_2_17_23_43_25_3342 "#STRM GetFmtpInterleaving, copied_fmtp_str_ptr clone fail!"
#define STRM_SDP_2376_112_2_17_23_43_25_3343 "#STRM GetFmtpInterleaving, wrong interleaving format, set to default value!"
#define STRM_SDP_2413_112_2_17_23_43_25_3344 "#STRM GetFmtpDigitalValueofPara input invalid"
#define STRM_SDP_2419_112_2_17_23_43_25_3345 "#STRM GetFmtpDigitalValueofPara, copied_fmtp_str_ptr clone fail!"
#define STRM_SDP_2430_112_2_17_23_43_25_3346 "#STRM GetFmtpDigitalValueofPara, no profile level id exists!"
#define STRM_SDP_2453_112_2_17_23_43_25_3347 "#STRM GetFmtpMpeg4Mode input invalid"
#define STRM_SDP_2459_112_2_17_23_43_25_3348 "#STRM GetFmtpMpeg4Mode, copied_fmtp_str_ptr clone fail!"
#define STRM_SERIALIZE_99_112_2_17_23_43_25_3349 "#STRM SerializeAuthInfo input invalid"
#define STRM_SERIALIZE_106_112_2_17_23_43_25_3350 "STRM SerializeAuthInfo, username err,len=%d"
#define STRM_SERIALIZE_116_112_2_17_23_43_25_3351 "STRM SerializeAuthInfo, password_ptr err,len=%d"
#define STRM_SERIALIZE_140_112_2_17_23_43_26_3352 "#STRM STRMSERIALIZE_AuthRsp input invalid"
#define STRM_SERIALIZE_149_112_2_17_23_43_26_3353 "STRM STRMSERIALIZE_AuthRsp alloc fail!"
#define STRM_SERIALIZE_162_112_2_17_23_43_26_3354 "STRM STRMSERIALIZE_AuthRsp serialize fail!"
#define STRM_SERIALIZE_196_112_2_17_23_43_26_3355 "#STRM SerializeExtSdpInfo input invalid"
#define STRM_SERIALIZE_204_112_2_17_23_43_26_3356 "#STRM SerializeExtSdpInfo, url1 error!"
#define STRM_SERIALIZE_216_112_2_17_23_43_26_3357 "#STRM SerializeExtSdpInfo, url2 error!"
#define STRM_SERIALIZE_229_112_2_17_23_43_26_3358 "#STRM SerializeExtSdpInfo, session_info_ptr error!"
#define STRM_SERIALIZE_242_112_2_17_23_43_26_3359 "#STRM SerializeExtSdpInfo, media_name_ptr error!"
#define STRM_SERIALIZE_255_112_2_17_23_43_26_3360 "#STRM SerializeExtSdpInfo, audio_param.audio_type_name_ptr error!"
#define STRM_SERIALIZE_268_112_2_17_23_43_26_3361 "#STRM SerializeExtSdpInfo, audio_param.config_ptr error!"
#define STRM_SERIALIZE_281_112_2_17_23_43_26_3362 "#STRM SerializeExtSdpInfo, video_param.config_ptr error!"
#define STRM_SERIALIZE_294_112_2_17_23_43_26_3363 "#STRM SerializeExtSdpInfo, video_param.video_type_name_ptr error!"
#define STRM_SERIALIZE_324_112_2_17_23_43_26_3364 "#STRM GetSdpExtInfoLen input invalid"
#define STRM_SERIALIZE_380_112_2_17_23_43_26_3365 "#STRM STRMSERIALIZE_InitCnf input invalid"
#define STRM_SERIALIZE_388_112_2_17_23_43_26_3366 "#STRM STRMSERIALIZE_InitCnf serialized_ptr alloc fail!"
#define STRM_SERIALIZE_403_112_2_17_23_43_26_3367 "STRM STRMSERIALIZE_InitCnf serialize fail!"
#define STRM_SERIALIZE_427_112_2_17_23_43_26_3368 "#STRM STRMSERIALIZE_RtspDscrbCnf input invalid"
#define STRM_SERIALIZE_435_112_2_17_23_43_26_3369 "#STRM STRMSERIALIZE_InitCnf serialized_ptr alloc fail!"
#define STRM_SERIALIZE_448_112_2_17_23_43_26_3370 "STRM STRMSERIALIZE_InitCnf serialize fail!"
#define STRM_TEST_APP_75_112_2_17_23_43_27_3371 "#STRMTEST StrmTestAppTask STRM_INIT_CNF"
#define STRM_TEST_APP_83_112_2_17_23_43_27_3372 "#STRMTEST StrmTestAppTask STRM_DESCRIBE_AUTH_IND"
#define STRM_TEST_APP_93_112_2_17_23_43_27_3373 "#STRMTEST StrmTestAppTask STRM_SETUP_AUTH_IND"
#define STRM_TEST_APP_100_112_2_17_23_43_27_3374 "#STRMTEST StrmTestAppTask STRM_PLAY_CNF"
#define STRM_TEST_APP_108_112_2_17_23_43_27_3375 "#STRMTEST StrmTestAppTask STRM_PLAY_AUTH_IND"
#define STRM_TEST_APP_118_112_2_17_23_43_27_3376 "#STRMTEST StrmTestAppTask STRM_PAUSE_CNF"
#define STRM_TEST_APP_127_112_2_17_23_43_27_3377 "#STRMTEST StrmTestAppTask STRM_RESUME_CNF"
#define STRM_TEST_APP_133_112_2_17_23_43_27_3378 "#STRMTEST StrmTestAppTask STRM_SEEK_CNF"
#define STRM_TEST_APP_137_112_2_17_23_43_27_3379 "#STRMTEST StrmTestAppTask STRM_BUFFERING_IND"
#define STRM_TEST_APP_140_112_2_17_23_43_27_3380 "#STRMTEST StrmTestAppTask STRM_BUFFER_END_IND"
#define STRM_TEST_APP_143_112_2_17_23_43_27_3381 "#STRMTEST StrmTestAppTask STRM_STOP_CNF"
#define STRM_TEST_REF_132_112_2_17_23_43_27_3382 "#STRM StrmTestDecode write file error!"
#define STRM_TEST_REF_151_112_2_17_23_43_27_3383 "#STRM_TEST StrmTestRefTask alloc memory error"
#define STRM_TEST_REF_173_112_2_17_23_43_27_3384 "#STRM_TEST StrmTestRefTask creat audio and video file  error"
#define STRM_TEST_REF_192_112_2_17_23_43_27_3385 "#STRM TEST_REF audio is empty!"
#define STRM_TEST_REF_207_112_2_17_23_43_27_3386 "#STRM TEST_REF video is empty!"
#define STRM_TEST_REF_213_112_2_17_23_43_27_3387 "#STRM TEST_REF video and audio both empty!"
#define STRM_TEST_REF_221_112_2_17_23_43_27_3388 "#STRM closefile error!"
#define STRM_TEST_RTP_SERVER_116_112_2_17_23_43_28_3389 "#STRM Buffer Test, insert AMR, seq=%d, timestamp=%d"
#define STRM_TEST_RTP_SERVER_132_112_2_17_23_43_28_3390 "#STRM Buffer Test, insert AMR, seq=%d, timestamp=%d"
#define MEM_CHECK_136_112_2_17_23_43_32_3391 "tcpip memchk: RecordMalloc() - alloc mem failed!"
#define MEM_CHECK_174_112_2_17_23_43_32_3392 "tcpip memchk: MEMORY NOT FREED -"
#define MEM_CHECK_179_112_2_17_23_43_32_3393 " - tcpip(%d): %s(%d), size = %d"
#define MEM_CHECK_188_112_2_17_23_43_32_3394 "tcpip memchk: TOTAL NO FREED MEM SIZE = %d"
#define MEM_CHECK_192_112_2_17_23_43_32_3395 "tcpip memchk: All MEMORY FREED!"
#define OSPORT_74_112_2_17_23_43_32_3396 "tcpip: TCPIP stack start up, version: %s"
#define OSPORT_122_112_2_17_23_43_33_3397 "tcpip: Priority of Task \"%s\" is %d "
#define OSPORT_128_112_2_17_23_43_33_3398 "tcpip: stack size of net stack is %d "
#define OSPORT_133_112_2_17_23_43_33_3399 "tcpip: stack size of clock stack is %d "
#define OSPORT_138_112_2_17_23_43_33_3400 "tcpip: the stack of ping stack is %d "
#define OSPORT_207_112_2_17_23_43_33_3401 "tcpip: tcpip_tasks_start() is no use any more!"
#define OSPORT_217_112_2_17_23_43_33_3402 "tcpip: tcpip_tasks_end() is no use any more!"
#define OSPORT_288_112_2_17_23_43_33_3403 "tcpip: error lock resource index!"
#define OSPORT_295_112_2_17_23_43_33_3404 "tcpip: resource %d is not exist, lock failed!"
#define OSPORT_305_112_2_17_23_43_33_3405 "tcpip: resource %d lock failed, mutex number = %d, task = %x !"
#define OSPORT_324_112_2_17_23_43_33_3406 "tcpip: error unlock resource index!"
#define OSPORT_335_112_2_17_23_43_33_3407 "tcpip: resource %d unlock failed, task = %x !"
#define PCLINK_DECODE_54_112_2_17_23_43_33_3408 "tcpip: alloc pclink decode buffer failed!"
#define PCLINK_DECODE_73_112_2_17_23_43_33_3409 "tcpip pclink decode warning: data_ptr(%p), data_len(%d), s_pclink_buff(%p)"
#define PCLINK_DECODE_101_112_2_17_23_43_34_3410 "tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_CFBB: Invalid data length( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_110_112_2_17_23_43_34_3411 "tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d)\n"
#define PCLINK_DECODE_121_112_2_17_23_43_34_3412 "tcpip pclink decode: 0x%x, DECS_DRCV -> DECS_CFBB: Receive too much( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_158_112_2_17_23_43_34_3413 "tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_IDLE: Invalid data length( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_167_112_2_17_23_43_34_3414 "tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d)\n"
#define PCLINK_DECODE_188_112_2_17_23_43_34_3415 "tcpip pclink decode: 0x%x, DECS_CFEB, Invalid nPkt(%d), nRcv(%d)n"
#define PCLINK_DECODE_197_112_2_17_23_43_34_3416 "tcpip pclink decode: Receive Link Setup Message! Set link setup flag!"
#define PCLINK_DECODE_201_112_2_17_23_43_34_3417 "tcpip pclink decode: Receive Link Close Message! Reset link setup flag!"
#define PCLINK_DECODE_208_112_2_17_23_43_34_3418 "tcpip pclink decode: Receive Packet from UART, len = %d!"
#define PCLINK_DECODE_242_112_2_17_23_43_34_3419 "tcpip pclink decode: 0x%x, DECS_CFBE, Invalid nPkt(%d), nRcv(%d)n"
#define PCLINK_DECODE_259_112_2_17_23_43_34_3420 "tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_IDLE: Invalid data length( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_270_112_2_17_23_43_34_3421 "tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d), nRcv(%d)n"
#define PCLINK_DECODE_281_112_2_17_23_43_34_3422 "tcpip pclink decode: 0x%x, DECS_DRCV -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_287_112_2_17_23_43_34_3423 "tcpip pclink decode: 0x%x, DECS_CFEB -> DECS_IDLE: End flag lost!\n"
#define PCLINK_DECODE_301_112_2_17_23_43_34_3424 "tcpip pclink decode: 0x%x, DECS_CFSB -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_326_112_2_17_23_43_34_3425 "tcpip pclink decode: 0x%x, DECS_CFBE, Invalid nPkt(%d), nRcv(%d)n"
#define PCLINK_DECODE_343_112_2_17_23_43_34_3426 "tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_IDLE: Invalid data length( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_354_112_2_17_23_43_34_3427 "tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d), nRcv(%d)n"
#define PCLINK_DECODE_365_112_2_17_23_43_34_3428 "tcpip pclink decode: 0x%x, DECS_DRCV -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_371_112_2_17_23_43_34_3429 "tcpip pclink decode: 0x%x, DECS_CFEB -> DECS_IDLE: End flag lost!\n"
#define PCLINK_DECODE_389_112_2_17_23_43_34_3430 "tcpip pclink decode: 0x%x, DECS_CFCB -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_420_112_2_17_23_43_34_3431 "tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_IDLE: Invalid data length( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_431_112_2_17_23_43_34_3432 "tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d), nRcv(%d)n"
#define PCLINK_DECODE_442_112_2_17_23_43_34_3433 "tcpip pclink decode: 0x%x, DECS_DRCV -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n"
#define PCLINK_DECODE_449_112_2_17_23_43_34_3434 "tcpip pclink decode: 0x%x, DECS_CFEB -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n"
#define STARTUP_PREP_174_112_2_17_23_43_35_3435 "tcpip debug: static Dnsbuf memory - 0x%x ~ 0x%x, size %d"
#define STARTUP_PREP_197_112_2_17_23_43_35_3436 "tcpip debug: static sockbuf memory - 0x%x ~ 0x%x, size %d"
#define STARTUP_PREP_275_112_2_17_23_43_36_3437 "tcpip: read dns from nv error!"
#define STARTUP_PREP_289_112_2_17_23_43_36_3438 "tcpip: default spare dns1: 0x%08x, dns2: 0x%08x"
#define TCPIP_BEARER_92_112_2_17_23_43_36_3439 "tcpip: register netif error - config check failed"
#define TCPIP_BEARER_101_112_2_17_23_43_36_3440 "tcpip: register netif error - no free netif found"
#define TCPIP_BEARER_148_112_2_17_23_43_36_3441 "tcpip: update netif error - config check failed"
#define TCPIP_BEARER_157_112_2_17_23_43_36_3442 "tcpip: update netif error - no netif found"
#define TCPIP_BEARER_165_112_2_17_23_43_36_3443 "tcpip: update netif error - netif %x is not open"
#define TCPIP_BEARER_202_112_2_17_23_43_36_3444 "tcpip: duplicate new netif error - no source netif found"
#define TCPIP_BEARER_210_112_2_17_23_43_36_3445 "tcpip: duplicate new netif error - source netif is not open"
#define TCPIP_BEARER_219_112_2_17_23_43_36_3446 "tcpip: duplicate new netif error - no dst netif found"
#define TCPIP_BEARER_263_112_2_17_23_43_36_3447 "tcpip: duplicate old netif error - no source netif found"
#define TCPIP_BEARER_272_112_2_17_23_43_36_3448 "tcpip: duplicate old netif error - no dst netif found"
#define TCPIP_BEARER_281_112_2_17_23_43_36_3449 "tcpip: duplicate old netif error - netif are not open: src(%d), dst(%d)"
#define TCPIP_BEARER_316_112_2_17_23_43_37_3450 "tcpip: dereg net interface - inval netid %x"
#define TCPIP_BEARER_323_112_2_17_23_43_37_3451 "tcpip: dereg net interface - netif %p is not open!"
#define TCPIP_BEARER_372_112_2_17_23_43_37_3452 "tcpip: netid(%x), in(%d), out(%d), in_rd(%d), out_rd(%d), in_drop(%d)"
#define TCPIP_BEARER_427_112_2_17_23_43_37_3453 "tcpip: open modem service - invalid net_id %x"
#define TCPIP_BEARER_436_112_2_17_23_43_37_3454 "tcpip: open modem service - inval netif(%p) state, open(%d), modem(%d)"
#define TCPIP_BEARER_444_112_2_17_23_43_37_3455 "tcpip: open modem service - ppp_ifopen() failed!"
#define TCPIP_BEARER_450_112_2_17_23_43_37_3456 "tcpip: open MODEM mode on netif(%x) - pppif(%p), modem_id(%d)"
#define TCPIP_BEARER_483_112_2_17_23_43_37_3457 "tcpip: close modem srv - invalid netid %x"
#define TCPIP_BEARER_491_112_2_17_23_43_37_3458 "tcpip: close MODEM mode on netif(%p)"
#define TCPIP_BEARER_520_112_2_17_23_43_37_3459 "tcpip: set IP address error - invalid addr_ptr"
#define TCPIP_BEARER_528_112_2_17_23_43_37_3460 "tcpip: set IP address error - invalid net_id %x"
#define TCPIP_BEARER_559_112_2_17_23_43_37_3461 "tcpip: tx cnf - invalid net_id %x"
#define TCPIP_BEARER_566_112_2_17_23_43_37_3462 "tcpip: tx cnf - error called on net_id %x"
#define TCPIP_BEARER_601_112_2_17_23_43_37_3463 "tcpip: TCPIP_RxInd() - invalid netid %x"
#define TCPIP_BEARER_613_112_2_17_23_43_37_3464 "tcpip: TCPIP_RxInd() - warning, len(%d) > mtu(%d)"
#define TCPIP_BEARER_623_112_2_17_23_43_37_3465 "tcpip: TCPIP_RxInd() - alloc buffer failed, len %d"
#define TCPIP_BEARER_653_112_2_17_23_43_37_3466 "tcpip: TCPIP_RxInd() - invalid prot pointer for pkt %p"
#define TCPIP_BEARER_680_112_2_17_23_43_37_3467 "tcpip: TCPIP_RxInd() - unknown packet type %d!"
#define TCPIP_BEARER_715_112_2_17_23_43_37_3468 "tcpip: TCPIP_RxInd() - no ppp supported, free pkt %x"
#define TCPIP_BEARER_749_112_2_17_23_43_37_3469 "tcpip: set tx flow ctrl - net_id is NULL"
#define TCPIP_BEARER_757_112_2_17_23_43_37_3470 "tcpip: set tx flow ctrl - invalid net_id %x"
#define TCPIP_BEARER_764_112_2_17_23_43_37_3471 "tcpip: tx flowctrl OPENED on netid %x"
#define TCPIP_BEARER_779_112_2_17_23_43_38_3472 "tcpip: tx flowctrl CLOSED on netid %x, last %d ms"
#define TCPIP_BEARER_801_112_2_17_23_43_38_3473 "tcpip: set tx rate - invalid net_id %x"
#define TCPIP_BEARER_811_112_2_17_23_43_38_3474 "tcpip: set tx rate %d bytes/s on netid %x"
#define TCPIP_BEARER_829_112_2_17_23_43_38_3475 "tcpip: set mtu - invalid net_id %x"
#define TCPIP_BEARER_849_112_2_17_23_43_38_3476 "tcpip: set mtu %d on netid %x"
#define TCPIP_BEARER_872_112_2_17_23_43_38_3477 "tcpip: resume TCPIP timer"
#define TCPIP_BEARER_925_112_2_17_23_43_38_3478 "tcpip: %s - invalid cfg_ptr"
#define TCPIP_BEARER_933_112_2_17_23_43_38_3479 "tcpip: %s - invalid packet type"
#define TCPIP_BEARER_940_112_2_17_23_43_38_3480 "tcpip: %s - invalid tx_req_fptr"
#define TCPIP_BEARER_949_112_2_17_23_43_38_3481 "tcpip: %s - invalid hard address - ptr(%p), len(%d)"
#define TCPIP_BEARER_957_112_2_17_23_43_38_3482 "tcpip: %s - invalid hard address for ETHERNET"
#define TCPIP_BEARER_1145_112_2_17_23_43_38_3483 "tcpip: Copy Net Interface - dst_netid(%p), src_netid(%p)"
#define TCPIP_PCLINK_73_112_2_17_23_43_38_3484 "tcpip pclink: get pclink flag from NV error!"
#define TCPIP_PCLINK_92_112_2_17_23_43_39_3485 "tcpip pclink: no free netif found!"
#define TCPIP_PCLINK_167_112_2_17_23_43_39_3486 "tcpip pclink: invalid input!"
#define TCPIP_PCLINK_173_112_2_17_23_43_39_3487 "tcpip pclink: to send pkt %p, iplen(%d)"
#define TCPIP_PCLINK_305_112_2_17_23_43_39_3488 "tcpip pclink: set pclink netif error - no pclink prepared!"
#define TCPIP_PCLINK_320_112_2_17_23_43_39_3489 "tcpip pclink: set ip address on netid %x failed!"
#define TCPIP_PCLINK_330_112_2_17_23_43_39_3490 "tcpip pclink: switch UART to DATA mode for PCLINK use!"
#define TCPIP_PS_164_112_2_17_23_43_40_3491 "tcpip ps: no netif in-use on GPRS"
#define TCPIP_PS_178_112_2_17_23_43_40_3492 "tcpip ps: invalid NSAPI %d"
#define TCPIP_PS_186_112_2_17_23_43_40_3493 "tcpip ps: get netid - invalid index %d for nsapi %d"
#define TCPIP_PS_202_112_2_17_23_43_40_3494 "tcpip ps: GetNsapiByNetid - netid is NULL"
#define TCPIP_PS_226_112_2_17_23_43_40_3495 "tcpip ps: GetNsapiByNetid - netid is NULL"
#define TCPIP_PS_237_112_2_17_23_43_40_3496 "tcpip ps: no cid found with netid %x"
#define TCPIP_PS_251_112_2_17_23_43_40_3497 "tcpip ps: GetLinkidByNetid - netid is NULL"
#define TCPIP_PS_262_112_2_17_23_43_40_3498 "tcpip ps: no link ID found with netid %x"
#define TCPIP_PS_326_112_2_17_23_43_40_3499 "tcpip ps: set net interface - inval nsapi(%d, %d)"
#define TCPIP_PS_396_112_2_17_23_43_40_3500 "tcpip ps: set ppp interface - inval nsapi(%d)"
#define TCPIP_PS_406_112_2_17_23_43_40_3501 "tcpip ps: set ppp interface - no ppp supported"
#define TCPIP_PS_421_112_2_17_23_43_40_3502 "tcpip: TCPIP_ResetPPPInterface is called"
#define TCPIP_PS_445_112_2_17_23_43_41_3503 "tcpip ps: GPRSTCPIP_SetPPPService - no netif in use for GPRS"
#define TCPIP_PS_453_112_2_17_23_43_41_3504 "tcpip ps: GPRSTCPIP_SetPPPService - no nsapi with netid %x"
#define TCPIP_PS_460_112_2_17_23_43_41_3505 "tcpip ps: GPRSTCPIP_SetPPPService - no ppp supported"
#define TCPIP_PS_506_112_2_17_23_43_41_3506 "tcpip: MPPP_SetPPPInterface - invalid input, nsapi(%d), link_id(%d), cid(%d)"
#define TCPIP_PS_519_112_2_17_23_43_41_3507 "tcpip: MPPP_SetPPPInterface - nsapi(%d), link_id(%d), cid(%d)"
#define TCPIP_PS_524_112_2_17_23_43_41_3508 "tcpip: MPPP_SetPPPInterface - no ppp supported"
#define TCPIP_PS_540_112_2_17_23_43_41_3509 "tcpip: MPPP_ResetPPPInterface is called"
#define TCPIP_PS_571_112_2_17_23_43_41_3510 "tcpip: ppp disconnect on netif %x by mux release"
#define TCPIP_PS_687_112_2_17_23_43_41_3511 "tcpip ps: RawSendToPs() - invalid input"
#define TCPIP_PS_711_112_2_17_23_43_41_3512 "tcpip ps: MNGPRS_SendDataReq() failed!"
#define TCPIP_PS_722_112_2_17_23_43_41_3513 "tcpip ps: MNGPRS_SendDataReq() failed!"
#define TCPIP_PS_750_112_2_17_23_43_41_3514 "tcpip ps: set netif - invalid index %d for nsapi %d"
#define TCPIP_PS_764_112_2_17_23_43_41_3515 "tcpip ps: set net interface for nsapi(%d, %d)"
#define TCPIP_PS_776_112_2_17_23_43_41_3516 "tcpip ps: update net interface for nsapi(%d, %d)"
#define TCPIP_PS_789_112_2_17_23_43_41_3517 "tcpip ps: invalid index %d for parent nsapi %d"
#define TCPIP_PS_794_112_2_17_23_43_41_3518 "tcpip ps: no netif for parent nsapi %d"
#define TCPIP_PS_807_112_2_17_23_43_41_3519 "tcpip ps: copy net interface for nsapi(%d, %d)"
#define TCPIP_PS_819_112_2_17_23_43_41_3520 "tcpip ps: update net interface by nsapi(%d, %d)"
#define TCPIP_PS_846_112_2_17_23_43_41_3521 "tcpip ps: reset net interface for nsapi %d"
#define TCPIP_PS_878_112_2_17_23_43_42_3522 "tcpip ps: open modem - invalid index %d for nsapi %d"
#define TCPIP_PS_882_112_2_17_23_43_42_3523 "tcpip ps: to open modem - nsapi %d, modem_id %d, cid %d"
#define TCPIP_PS_917_112_2_17_23_43_42_3524 "tcpip ps: close modem - invalid index %d for cid %d"
#define TCPIP_PS_922_112_2_17_23_43_42_3525 "tcpip ps: to close modem - nsapi %d, modem_id %d, cid %d"
#define TCPIP_VETHER_113_112_2_17_23_43_42_3526 "tcpip: Create task %s with ID : %d"
#define TCPIP_VETHER_115_112_2_17_23_43_42_3527 "tcpip: Priority of task %s is : %d"
#define TCPIP_VETHER_138_112_2_17_23_43_42_3528 "tcpip: TCPIPVETH_Open - no netid found with nsapi %d"
#define TCPIP_VETHER_146_112_2_17_23_43_42_3529 "tcpip: TCPIPVETH_Open - netif with nsapi %d is not open"
#define TCPIP_VETHER_162_112_2_17_23_43_42_3530 "tcpip: TCPIPVETH_Open - veth_open on minor %x!"
#define TCPIP_VETHER_166_112_2_17_23_43_42_3531 "tcpip: TCPIPVETH_Open - veth_open failed on minor %x!"
#define TCPIP_VETHER_171_112_2_17_23_43_42_3532 "tcpip: TCPIPVETH_Open - minor is in use!"
#define TCPIP_VETHER_176_112_2_17_23_43_42_3533 "tcpip: TCPIPVETH_Open - vEther is not supported!"
#define TCPIP_VETHER_192_112_2_17_23_43_42_3534 "tcpip: TCPIPVETH_Close - on minor %x!"
#define TCPIP_VETHER_205_112_2_17_23_43_42_3535 "tcpip: TCPIPVETH_close - vEther is not supported!"
#define TCPIP_VETHER_221_112_2_17_23_43_42_3536 "tcpip: TCPIPVETH_SendPacket - invalid input"
#define TCPIP_VETHER_231_112_2_17_23_43_42_3537 "tcpip: TCPIPVETH_SendPacket - veth_tx failed"
#define TCPIP_VETHER_235_112_2_17_23_43_42_3538 "tcpip: TCPIPVETH_SendToVeth - vEther is not supported, drop pkt %x"
#define TCPIP_VETHER_321_112_2_17_23_43_42_3539 "tcpip: invalid data para from vEth, ptr %x, len %d"
#define TCPIP_VETHER_332_112_2_17_23_43_42_3540 "tcpip: VethRecvTaskEntry - invalid minor"
#define TCPIP_VETHER_347_112_2_17_23_43_42_3541 "tcpip: VethToBearer - invalid input"
#define TCPIP_VETHER_357_112_2_17_23_43_43_3542 "tcpip: VethToBearer - alloc buffer failed, len %d"
#define TCPIP_VETHER_372_112_2_17_23_43_43_3543 "tcpip: VethToBearer - unsupported packet type %d!"
#define TCPIP_VETHER_403_112_2_17_23_43_43_3544 "tcpip: resume veth receive task!"
#define UART_MUX_68_112_2_17_23_43_43_3545 "ppp: create MUX link - netif %x, link_id %d, ret %d"
#define UART_MUX_84_112_2_17_23_43_43_3546 "ppp: release MUX link - netif %x, link_id %d, ret %d"
#define UART_MUX_100_112_2_17_23_43_43_3547 "ppp: UART recv mode: 1 - normal timing check!"
#define UART_MUX_103_112_2_17_23_43_43_3548 "ppp: UART recv mode: 2 - alloc buffer and wake up demux task!"
#define UART_MUX_105_112_2_17_23_43_43_3549 "ppp: UART recv mode: 3 - MUX directly upcall"
#define UART_MUX_127_112_2_17_23_43_43_3550 "ppp: no pppif found with link id %d"
#define UART_MUX_143_112_2_17_23_43_43_3551 "ppp: UARTMUX_SendBuffCheck - invalid input len %d"
#define UART_MUX_162_112_2_17_23_43_43_3552 "ppp: not enough send buffer (tot: %d, left: %d, need: %d)"
#define UART_MUX_185_112_2_17_23_43_43_3553 "ppp: UARTMUX_SendBuffData - invalid buffer(%p) or data ptr"
#define UART_MUX_198_112_2_17_23_43_43_3554 "ppp: send %d bytes on link id %d"
#define UART_MUX_213_112_2_17_23_43_43_3555 "ppp: send %d bytes on link id %d"
#define UART_MUX_222_112_2_17_23_43_43_3556 "ppp: send %d bytes on link id %d"
#define UART_MUX_232_112_2_17_23_43_43_3557 "ppp: UARTMUX_SendBuffData - start point(%d) = end point(%d)!"
#define UART_MUX_291_112_2_17_23_43_43_3558 "ppp: UARTMUX_PutChar - alloc send buffer failed!"
#define UART_MUX_300_112_2_17_23_43_43_3559 "ppp: UARTMUX_PutChar - alloc data buffer failed!"
#define UART_MUX_320_112_2_17_23_43_43_3560 "ppp: UARTMUX_PutChar - invalid buff_ptr %p or data_ptr"
#define UART_MUX_338_112_2_17_23_43_43_3561 "ppp: UARTMUX_PutChar - buffer is full, can't hold char 0x%x"
#define UART_MUX_370_112_2_17_23_43_44_3562 "ppp: MuxDataRecvUpcall - invalid input!"
#define UART_MUX_377_112_2_17_23_43_44_3563 "tcpip test: MuxDataRecvUpcall - drop %d bytes!"
#define UART_MUX_383_112_2_17_23_43_44_3564 "ppp: %d bytes received with link_id %d"
#define UART_MUX_431_112_2_17_23_43_44_3565 "ppp: receive buff is not enough, space(%d), data_len(%d), link_id(%d)"
#define UART_MUX_454_112_2_17_23_43_44_3566 "ppp: MuxDataRecvUpcall - invalid input!"
#define UART_MUX_461_112_2_17_23_43_44_3567 "tcpip test: MuxDataRecvUpcall - drop %d bytes!"
#define UART_MUX_467_112_2_17_23_43_44_3568 "ppp: %d bytes received with link_id %d"
#define UART_MUX_484_112_2_17_23_43_44_3569 "ppp: MuxDataRecvUpcall - no pppif matched with link_id %d"
#define UARTUTIL_142_112_2_17_23_43_44_3570 "ppp: ln_uconnect() called!"
#define UARTUTIL_162_112_2_17_23_43_44_3571 "ppp error: ln_udisconnect - line is NULL!"
#define UARTUTIL_169_112_2_17_23_43_44_3572 "ppp error: ln_udisconnect - pppif is NULL!"
#define UARTUTIL_173_112_2_17_23_43_44_3573 "ppp: ln_udisconnect() called!"
#define UARTUTIL_185_112_2_17_23_43_44_3574 "ppp: netif %p is not in modem mode!"
#define UARTUTIL_199_112_2_17_23_43_44_3575 "ppp: ppp_disconnect() - invalid netif_ptr or n_local!"
#define UARTUTIL_204_112_2_17_23_43_44_3576 "ppp: start pppif(%p) disconnect!"
#define UARTUTIL_215_112_2_17_23_43_44_3577 "ppp: ppp_disconnect() - wait for %d ms!"
#define UARTUTIL_229_112_2_17_23_43_44_3578 "ppp: switch UART to AT mode"
#define UARTUTIL_247_112_2_17_23_43_44_3579 "ppp: ppp_connect() - invalid netif_ptr or n_local!"
#define UARTUTIL_260_112_2_17_23_43_44_3580 "ppp: switch UART to DATA mode"
#define UARTUTIL_264_112_2_17_23_43_44_3581 "ppp: start pppif(%p) - ppp lower up!"
#define UARTUTIL_271_112_2_17_23_43_44_3582 "ppp: to start PPP connect - sleep %d ms ..."
#define UARTUTIL_327_112_2_17_23_43_45_3583 "ppp: uart recv %d bytes"
#define UARTUTIL_390_112_2_17_23_43_45_3584 "tcpip: UPM event - UPM_PLUGOUT is detected!"
#define UARTUTIL_433_112_2_17_23_43_45_3585 "ppp uart: uartrcv_addq() - invalid input!"
#define UARTUTIL_441_112_2_17_23_43_45_3586 "ppp uart: uart_data_add() - alloc UPACKET failed! "
#define UARTUTIL_449_112_2_17_23_43_45_3587 "ppp uart: uart_data_add() - alloc u_data failed! "
#define UARTUTIL_487_112_2_17_23_43_45_3588 "ppp uart warning: invalid upkt!"
#define UARTUTIL_510_112_2_17_23_43_45_3589 "ppp uart: invalid pppif(%p) or data_ptr(%p) of upkt %p!"
#define XIP_SEC_93_112_2_17_23_43_45_3590 "tcpip xip: deregister encrypt func pointer!"
#define XIP_SEC_100_112_2_17_23_43_45_3591 "tcpip xip: update encrypt func - old(%p), new(%p)"
#define XIP_SEC_104_112_2_17_23_43_45_3592 "tcpip xip: register encrypt func - %p"
#define XIP_SEC_121_112_2_17_23_43_46_3593 "tcpip xip: TCPIP_RegIpUpFunc() - input is NULL!"
#define XIP_SEC_140_112_2_17_23_43_46_3594 "tcpip xip: TCPIP_RegIpDownFunc() - input is NULL!"
#define XIP_SEC_160_112_2_17_23_43_46_3595 "tcpip xip: XIP_SendToEncrypt() - input pkt is NULL!"
#define XIP_SEC_172_112_2_17_23_43_46_3596 "tcpip xip: allocate XIPPACKET for UL packet %x failed!"
#define XIP_SEC_213_112_2_17_23_43_46_3597 "tcpip xip: packet encrypt queue is NULL!"
#define XIP_SEC_250_112_2_17_23_43_46_3598 "tcpip xip: to send encrypted pkt %x"
#define XIP_SEC_270_112_2_17_23_43_46_3599 "tcpip xip: to send encrypted pkt %x ( initial %x )"
#define XIP_SEC_276_112_2_17_23_43_46_3600 "tcpip xip: encrpyted pk_cpalloc() failed!"
#define XIP_SEC_282_112_2_17_23_43_46_3601 "tcpip xip: encrypted data error, pkt(%p)->nb_prot(%p), ptr(%p), len(%d)"
#define XIP_SEC_300_112_2_17_23_43_46_3602 "tcpip xip: pkt(%p, ID:%x) not match with encrypted data (ID:%x), drop(%d)"
#define XIP_SEC_324_112_2_17_23_43_46_3603 "tcpip xip: XIP_RecvToDecrypt() - input pkt is NULL!"
#define XIP_SEC_336_112_2_17_23_43_46_3604 "tcpip xip: allocate XIPPACKET for DL packet %x failed!"
#define XIP_SEC_381_112_2_17_23_43_46_3605 "tcpip xip: packet decrypt queue is NULL!"
#define XIP_SEC_452_112_2_17_23_43_46_3606 "tcpip xip: decrpyted pk_cpalloc() failed!"
#define XIP_SEC_458_112_2_17_23_43_46_3607 "tcpip xip: decrypted data error, pkt(%p)->nb_prot(%p), ptr(%p), len(%d)"
#define XIP_SEC_477_112_2_17_23_43_46_3608 "tcpip xip: pkt(%p, ID:%x) not match with decrypted data (ID:%x), drop(%d)"
#define XIP_SEC_551_112_2_17_23_43_47_3609 "tcpip xip: XIPTEST_DoEncrypt() - alloc buffer failed!"
#define XIP_SEC_564_112_2_17_23_43_47_3610 "tcpip xip: XIPTEST_DoEncrypt() - invalid data_ptr(%p), data_len(%d) or dataup_func(%p)"
#define XIP_SEC_572_112_2_17_23_43_47_3611 "tcpip xip: XIPTEST_DoEncrypt() - invalid xipdata_ptr(%p)!"
#define XIP_SEC_603_112_2_17_23_43_47_3612 "tcpip xip: XIPTEST_DoDecrypt() - alloc buffer failed!"
#define XIP_SEC_616_112_2_17_23_43_47_3613 "tcpip xip: XIPTEST_DoEncrypt() - invalid data_ptr(%p), data_len(%d) or datadown_func(%p)"
#define XIP_SEC_624_112_2_17_23_43_47_3614 "tcpip xip: XIPTEST_DoDecrypt() - invalid xipdata_ptr(%p)!"
#define XIP_SEC_648_112_2_17_23_43_47_3615 "tcpip xip: XIPTEST_AddToQueue() - invalid data_ptr(%p) or data_len(%d)"
#define XIP_SEC_667_112_2_17_23_43_47_3616 "tcpip xip: allocate XIPDATA with data ID %x failed!"
#define ET_ARP_142_112_2_17_23_43_49_3617 "tcpip: et_send() - no send pointer for pkt %p"
#define ET_ARP_320_112_2_17_23_43_49_3618 "tcpip: make_arp_entry - free pending pkt %p on arptabent %p"
#define ET_ARP_412_112_2_17_23_43_50_3619 "tcpip: send_arp() - add pkt %p on arptabent %p"
#define ET_ARP_489_112_2_17_23_43_50_3620 "tcpip: send_arp() - no send pointer for pkt %p"
#define ET_ARP_519_112_2_17_23_43_50_3621 "tcpip: send_arp() - no send pointer for pkt %p"
#define ET_ARP_554_112_2_17_23_43_50_3622 "tcpip: arpReply() - pk_alloc for pkt %x failed"
#define ET_ARP_625_112_2_17_23_43_50_3623 "tcpip: arpReply() - no send pointer for pkt %p"
#define ET_ARP_711_112_2_17_23_43_50_3624 "tcpip debug: IP conflict with MAC %02X:%02X:%02X:%02X:%02X:%02X"
#define ET_ARP_854_112_2_17_23_43_51_3625 "tcpip: send_via_arp() - drop pkt %x for pending time-out"
#define ET_ARP_860_112_2_17_23_43_51_3626 "tcpip: send_via_arp() - drop pkt %x for pending time-out"
#define ET_ARP_891_112_2_17_23_43_51_3627 "tcpip: send_via_arp() - drop pkt %x for no more pending"
#define ICMP_190_112_2_17_23_43_51_3628 "tcpip debug: icmprcv - cancel redirect in add_route for dest: %08x, hop1: %08x"
#define IFACE_658_112_2_17_23_43_53_3629 "tcpip error: if_getbynum - no netif found!"
#define IP_257_112_2_17_23_43_54_3630 "tcpip: ip_write_internal - pkt:%x, len:%u to %u.%u.%u.%u, can't route"
#define IP_302_112_2_17_23_43_55_3631 "tcpip: pkt %x on netif %x has no NF_NBPORT"
#define IP_335_112_2_17_23_43_55_3632 "tcpip: no fragment, pkt %x len %d > mtu %d on netif %x"
#define IP_499_112_2_17_23_43_55_3633 "tcpip: ip_fragment - small max ip size %d on netif %x for pkt %x"
#define IP_550_112_2_17_23_43_55_3634 "tcpip: ip_fragment() - pk_alloc() failed for pkt 0x%x"
#define IP_697_112_2_17_23_43_55_3635 "tcpip: iproute() - invalid host or hop1!"
#define IP_761_112_2_17_23_43_55_3636 "tcpip: iproute() - add_route() failed!"
#define IP_798_112_2_17_23_43_56_3637 "[ TCPIPINFO ] Link-layer is already released, but APP still try to use the network."
#define IP_802_112_2_17_23_43_56_3638 "tcpip: iproute() - no route found for host %08x, netif %p!"
#define IP_REASM_125_112_2_17_23_43_56_3639 "tcpip: ip_reassm() - req len %d > buff size %d, drop pkt %x"
#define IP_REASM_142_112_2_17_23_43_56_3640 "tcpip: ip_reassm() - no fragment slots, drop pkt %x"
#define IP_REASM_160_112_2_17_23_43_56_3641 "tcpip: ip_reassm() - alloc big buff fail, drop pkt %x"
#define IP_REASM_221_112_2_17_23_43_56_3642 "tcpip: ip_reassm() - no fragment slots, drop pkt %x"
#define IP_REASM_253_112_2_17_23_43_56_3643 "tcpip: ip_reassm() - fragment reassemble logic error, drop %x"
#define IPCONFLICT_CHK_82_112_2_17_23_43_57_3644 "tcpip: sci_ipconflict_reg - AddNodeByTask failed on netif %x"
#define IPCONFLICT_CHK_91_112_2_17_23_43_57_3645 "tcpip: sci_ipconflict_reg - handle %x on netif %x"
#define IPCONFLICT_CHK_95_112_2_17_23_43_57_3646 "tcpip: sci_ipconflict_reg - no ip conflict notify!"
#define IPCONFLICT_CHK_108_112_2_17_23_43_57_3647 "tcpip: sci_ipconflict_dereg - handle %x"
#define IPCONFLICT_CHK_124_112_2_17_23_43_57_3648 "tcpip: sci_ipconflict_dereg - no ip conflict notify!"
#define IPCONFLICT_CHK_152_112_2_17_23_43_57_3649 "tcpip debug: IPCONFLICT_Notify - less than %d seconds, do not notify."
#define IPCONFLICT_CHK_167_112_2_17_23_43_57_3650 "tcpip debug: alloc signal memory failed for IP conflict on netif %x"
#define IPCONFLICT_CHK_178_112_2_17_23_43_57_3651 "tcpip debug: send IP_CONFLICT event to task %x, handle %x, netif %x"
#define IPDEMUX_113_112_2_17_23_43_58_3652 "tcpip: ip_rcv() - BAD_IP_HEAD(%d): bad pkt len, pkt = %08xn"
#define IPDEMUX_138_112_2_17_23_43_58_3653 "tcpip: ip_rcv() - BAD_IP_HEAD(%d): bad version num, pkt = %08xn"
#define IPDEMUX_152_112_2_17_23_43_58_3654 "tcpip: ip_rcv() - BAD_IP_HEAD(%d): bad chksum, pkt %x"
#define IPDEMUX_220_112_2_17_23_43_58_3655 "tcpip: ip_rcv - pkt %x len %d > mtu %d on netif %x"
#define IPDEMUX_375_112_2_17_23_43_58_3656 "tcpip: pkt %x should be fragmentated, pktlen(%d) > mtu(%d)"
#define IPDEMUX_395_112_2_17_23_43_58_3657 "tcpip: ip_rcv - pkt %x len %d > mtu %d on netif %x"
#define IPNET_228_112_2_17_23_43_59_3658 "tcpip: fixup_subnet_mask - IP address 0x%x is bad"
#define IPNET_520_112_2_17_23_44_0_3659 "tcpip: modem mode not supported, free pkt %x"
#define IPNET_673_112_2_17_23_44_0_3660 "tcpip: ip2mac() - ifAdminStatus = NI_DOWN, drop pkt %x"
#define IPNET_715_112_2_17_23_44_0_3661 "tcpip: ip2mac() - no send pointer for pkt %x"
#define IPNET_741_112_2_17_23_44_0_3662 "tcpip error: ip2mac - unknown interface type for pkt %x"
#define IPNET_785_112_2_17_23_44_0_3663 "tcpip: found matched pkt %x in tx queue, drop pkt %x"
#define IPNET_804_112_2_17_23_44_0_3664 "tcpip: send pending pkt %x on netif %x"
#define IPNET_815_112_2_17_23_44_0_3665 "tcpip: remove pkt %x from tx queue on netif %x"
#define IPNET_827_112_2_17_23_44_1_3666 "tcpip: add pkt %x to tx queue on netif %x, qlen = %d"
#define IPNET_832_112_2_17_23_44_1_3667 "tcpip: PacketSend() - drop pkt %x for error %d"
#define IPNET_880_112_2_17_23_44_1_3668 "tcpip: cap output not support packet type %d"
#define IPNET_916_112_2_17_23_44_1_3669 "tcpip: TxToBearer() - async tx pkt %x fail, error %d"
#define IPNET_944_112_2_17_23_44_1_3670 "tcpip: TxToBearer() - sync tx pkt %x fail, error %d"
#define IPNET_960_112_2_17_23_44_1_3671 "tcpip: TxToBearer() - drop pkt %x for error %d"
#define IPNET_1019_112_2_17_23_44_1_3672 "tcpip: GetSubnetMask() - not ABC-class IP 0x%08x "
#define IPROUTE_138_112_2_17_23_44_2_3673 "tcpip add_route: rejected null parm, dest: %08x, mask: %08x"
#define IPSTART_293_112_2_17_23_44_4_3674 "tcpip: bad netif ptr %p"
#define APP_PING_120_112_2_17_23_44_8_3675 "tcpip: sci_ping_request - no ping supported"
#define APP_PING_138_112_2_17_23_44_8_3676 "tcpip: sci_ping_cancel() - cancel ping %x"
#define APP_PING_182_112_2_17_23_44_8_3677 "tcpip: sci_async_ping - no ping supported"
#define APP_PING_222_112_2_17_23_44_8_3678 "tcpip: ping_request - invalid input faddr!"
#define APP_PING_230_112_2_17_23_44_8_3679 "tcpip: ping_request - address len should < %d"
#define APP_PING_240_112_2_17_23_44_8_3680 "tcpip: ping_request - no ping callback set"
#define APP_PING_250_112_2_17_23_44_8_3681 "tcpip: ping_request - no ping message task set"
#define APP_PING_259_112_2_17_23_44_8_3682 "tcpip: ping_request - unsupport ping type %d"
#define APP_PING_268_112_2_17_23_44_8_3683 "tcpip: ping_request - no extern netif for netid %x"
#define APP_PING_276_112_2_17_23_44_8_3684 "tcpip: ping_request - create ping handle failed"
#define APP_PING_284_112_2_17_23_44_8_3685 "tcpip: ping_request - mem alloc failed!"
#define APP_PING_325_112_2_17_23_44_8_3686 "tcpip: ping_request - handle(%x), addr(%s), tmout(%dms), netif(%x), type(%s)"
#define APP_PING_336_112_2_17_23_44_8_3687 "tcpip: ping_request - resolve ping host "
#define APP_PING_356_112_2_17_23_44_8_3688 "tcpip: ping req error - ping_send() error %d"
#define APP_PING_449_112_2_17_23_44_9_3689 "tcpip: ping_result_notify - res(%d), time_delay(%dms), handle(%x)"
#define APP_PING_457_112_2_17_23_44_9_3690 "tcpip: ping_result_notify - no callback"
#define APP_PING_472_112_2_17_23_44_9_3691 "tcpip: ping_result_notify - unsupport notify type %d"
#define APP_PING_530_112_2_17_23_44_9_3692 "tcpip: delete ping %p from pingq error - %d, handle %x"
#define APP_PING_673_112_2_17_23_44_9_3693 "tcpip: ping_send() - icmpEcho error %d"
#define APP_PING_836_112_2_17_23_44_9_3694 "tcpip: ping_check() - resolve ping host "
#define APP_PING_882_112_2_17_23_44_10_3695 "tcpip: ping_check() - ping handle %x, state %d, do nothing"
#define APP_TCP_IF_202_112_2_17_23_44_10_3696 "tcpip: sci_gethostbyname_ext() - input NULL!"
#define APP_TCP_IF_207_112_2_17_23_44_10_3697 "tcpip: sci_gethostbyname_ext() - url \"%s\", netid %x"
#define APP_TCP_IF_284_112_2_17_23_44_10_3698 "tcpip: sci_parse_host_ext() - url \"%s\", netid %x"
#define APP_TCP_IF_390_112_2_17_23_44_11_3699 "tcpip: sci_udp_send_ext(netid %x) error - %d"
#define APP_TCP_IF_461_112_2_17_23_44_11_3700 "tcpip: sci_udp_open_ext() - invalid input: handler, lport or label!"
#define APP_TCP_IF_957_112_2_17_23_44_12_3701 "tcpip: app_recv_upcall() - udp data not for us!"
#define APP_TCP_IF_963_112_2_17_23_44_12_3702 "tcpip: app_recv_upcall() - udp data, pkt = %p, len = %d"
#define ASYNC_GETHOST_101_112_2_17_23_44_12_3703 "tcpip: sci_async_gethostbyname - invalid input!"
#define ASYNC_GETHOST_109_112_2_17_23_44_12_3704 "tcpip: sci_async_gethostbyname - no netif found!"
#define ASYNC_GETHOST_117_112_2_17_23_44_12_3705 "tcpip: should use sci_gethostaddress_ext() get local address"
#define ASYNC_GETHOST_126_112_2_17_23_44_12_3706 "tcpip: sci_async_gethostbyname - alloc memory failed!"
#define ASYNC_GETHOST_172_112_2_17_23_44_12_3707 "tcpip: sci_async_gethostbyname - url(%s), timeout(%d ms), netid(%x)"
#define ASYNC_GETHOST_177_112_2_17_23_44_12_3708 "tcpip: sci_async_gethostbyname - not support!"
#define ASYNC_GETHOST_222_112_2_17_23_44_12_3709 "tcpip: gethostbyname_check - success for URL(%s), addr(%s)"
#define ASYNC_GETHOST_233_112_2_17_23_44_12_3710 "tcpip: gethostbyname_check - error %d for URL(%s)"
#define ASYNC_GETHOST_244_112_2_17_23_44_12_3711 "tcpip: gethostbyname_check - timeout for URL(%s)"
#define ASYNC_GETHOST_340_112_2_17_23_44_13_3712 "tcpip: gethostbyname_bynetid - invalid netid %x"
#define ASYNC_GETHOST_394_112_2_17_23_44_13_3713 "tcpip: gethostbyname_informevent - invalid task"
#define ASYNC_GETHOST_426_112_2_17_23_44_13_3714 "tcpip: gethostbyname_informevent - alloc signal failed"
#define ASYNC_SOCK_243_112_2_17_23_44_14_3715 "tcpip: asyncselect_informevent - no mem for READ event"
#define ASYNC_SOCK_265_112_2_17_23_44_14_3716 "tcpip: asyncselect_informevent - no mem for READ_BUFFER event"
#define ASYNC_SOCK_274_112_2_17_23_44_14_3717 "tcpip debug: send READ_BUFFER_STATUS event for so %x to task %x"
#define ASYNC_SOCK_287_112_2_17_23_44_14_3718 "tcpip: asyncselect_informevent - no mem for WRITE event"
#define ASYNC_SOCK_308_112_2_17_23_44_14_3719 "tcpip: asyncselect_informevent - no mem for CONNECT event"
#define ASYNC_SOCK_318_112_2_17_23_44_14_3720 "tcpip debug: send CONNECT event for so %x to task %x, error %d"
#define ASYNC_SOCK_329_112_2_17_23_44_14_3721 "tcpip: asyncselect_informevent - no mem for CLOSE event"
#define ASYNC_SOCK_339_112_2_17_23_44_14_3722 "tcpip debug: send CLOSE event for so %x to task %x, error %d"
#define ASYNC_SOCK_351_112_2_17_23_44_14_3723 "tcpip: asyncselect_informevent - no mem for ACCEPT event"
#define ASYNC_SOCK_360_112_2_17_23_44_14_3724 "tcpip debug: send ACCEPT event for so %x to task %x"
#define ASYNC_SOCK_371_112_2_17_23_44_14_3725 "tcpip: asyncselect_informevent - no mem for FULL_CLOSE event"
#define ASYNC_SOCK_381_112_2_17_23_44_14_3726 "tcpip: send FULL_CLOSED event (%d) for so %x to task %x, reason = %d"
#define ASYNC_SOCK_415_112_2_17_23_44_14_3727 "tcpip: ping_inform_event - alloc signal failed"
#define ASYNC_SOCK_428_112_2_17_23_44_14_3728 "tcpip debug: ping message for handle %x, res %d, delay %dms"
#define ASYNC_SOCK_456_112_2_17_23_44_14_3729 "tcpip: dhcp_inform_event - alloc signal failed"
#define ASYNC_SOCK_474_112_2_17_23_44_14_3730 "tcpip debug: send dhcp message to %x on netid %x, res %d"
#define ASYNC_SOCK_511_112_2_17_23_44_14_3731 "tcpip: t_asyncselect() - input event is AS_NULL"
#define DHCSETUP_68_112_2_17_23_44_16_3732 "tcpip: sci_async_dhcp - invalid task ID!"
#define DHCSETUP_74_112_2_17_23_44_16_3733 "tcpip: sci_async_dhcp - no DHCP CLIENT supported!"
#define DHCSETUP_99_112_2_17_23_44_16_3734 "tcpip: sci_dhcp_request - invalid callback pointer!"
#define DHCSETUP_105_112_2_17_23_44_16_3735 "tcpip: sci_dhcp_request - no DHCP CLIENT supported!"
#define DHCSETUP_125_112_2_17_23_44_16_3736 "tcpip: sci_dhcp_cancel - no matched netif!"
#define DHCSETUP_133_112_2_17_23_44_16_3737 "tcpip: sci_dhcp_cancel - invalid netif index!"
#define DHCSETUP_141_112_2_17_23_44_16_3738 "tcpip: cancel dhcp request on netif %x"
#define DHCSETUP_171_112_2_17_23_44_16_3739 "tcpip: dhcp_request - no matched netif!"
#define DHCSETUP_179_112_2_17_23_44_16_3740 "tcpip: dhcp_request - invalid netif index!"
#define DHCSETUP_186_112_2_17_23_44_16_3741 "tcpip: dhcp_request - invalid hard address ptr or len %d!"
#define DHCSETUP_195_112_2_17_23_44_16_3742 "tcpip: dhcp_request - dhc_init_iface() error %d"
#define DHCSETUP_217_112_2_17_23_44_16_3743 "tcpip: dhcp_request - invalid dhcp type %d"
#define DHCSETUP_241_112_2_17_23_44_16_3744 "tcpip: dhcp_request - dhc_second() error %d"
#define DHCSETUP_248_112_2_17_23_44_16_3745 "tcpip: start DHCP request on netif %x"
#define NETMAIN_352_112_2_17_23_44_25_3746 "tcpip: stop TCPIP timer!"
#define NETMAIN_423_112_2_17_23_44_25_3747 "Spreadtrum TCP Echo Testing ......\n"
#define RESHOST_128_112_2_17_23_44_39_3748 "tcpip: in_reshost() - no DNS on netif %x"
#define TCPIP_DEBUG_284_112_2_17_23_44_46_3749 "tcpip debug: print pkt %p, len = %d"
#define TCPIP_DEBUG_294_112_2_17_23_44_46_3750 " - pktdata %d%d) %s"
#define TCPIP_DEBUG_302_112_2_17_23_44_46_3751 " - pktdata %d%d) %s"
#define TCPIP_DEBUG_316_112_2_17_23_44_46_3752 "tcpip: nofreed soq_len = %d "
#define TCPIP_DEBUG_332_112_2_17_23_44_46_3753 " - tcpip: so(%d) : 0x%x, tot_snd(%d), tot_rcv(%d), %s"
#define TCPIP_DEBUG_339_112_2_17_23_44_46_3754 " - tcpip: so(%d) : 0x%x, tot_snd(%d), tot_rcv(%d), UDP"
#define TCPIP_DEBUG_344_112_2_17_23_44_46_3755 " - tcpip: so(%d) : 0x%x, tot_snd(%d), tot_rcv(%d), %d"
#define TCPIP_DEBUG_353_112_2_17_23_44_46_3756 " - tcpip: - RCV pkt(%x): %p( %d, %d ), mbuf(%p): %s"
#define TCPIP_DEBUG_363_112_2_17_23_44_46_3757 " - tcpip: - SND pkt(%x): %p( %d, %d ), mbuf(%p): %s"
#define TCPIP_DEBUG_368_112_2_17_23_44_46_3758 "tcpip: bigfreeq = %d, lilfreeq = %d"
#define TCPIP_DEBUG_387_112_2_17_23_44_46_3759 "tcpip: inet_aton(), %s -> 0x%08x"
#define TCPIP_DEBUG_391_112_2_17_23_44_46_3760 "tcpip: inet_ntoa(), %s <- 0x%08x"
#define TCPIP_DEBUG_412_112_2_17_23_44_46_3761 "tcpip: no in-used net interface!"
#define TCPIP_DEBUG_420_112_2_17_23_44_46_3762 "tcpip: current in-used netif num(%d)"
#define TCPIP_DEBUG_427_112_2_17_23_44_46_3763 " - tcpip: index(%d), iface(%d), netid(%x), desc(%s), pkttype(%s)"
#define TCPIP_DEBUG_428_112_2_17_23_44_46_3764 " - tcpip:  ip address   - %s"
#define TCPIP_DEBUG_429_112_2_17_23_44_46_3765 " - tcpip:  subnet mask  - %s"
#define TCPIP_DEBUG_430_112_2_17_23_44_46_3766 " - tcpip:  gateway      - %s"
#define TCPIP_DEBUG_432_112_2_17_23_44_46_3767 " - tcpip:  dns pri/sec  - %s, %s"
#define TCPIP_DEBUG_437_112_2_17_23_44_46_3768 " - tcpip:  hard addr    - %s"
#define TCPIP_DEBUG_439_112_2_17_23_44_46_3769 " - tcpip:  mtu          - %d Byte"
#define TCPIP_DEBUG_455_112_2_17_23_44_46_3770 "tcpip: time(%04ds) - bigfreeq = %d, lilfreeq = %d"
#define TCPIP_DEBUG_487_112_2_17_23_44_47_3771 "tcpip: SimulatePktSend - invalid input"
#define TCPIP_DEBUG_497_112_2_17_23_44_47_3772 "tcpip: SimulatePktSend - alloc packet buffer failed, len = %d"
#define TCPIP_DEBUG_517_112_2_17_23_44_47_3773 "tcpip: SimulatePktSend - iproute failed"
#define TCPIP_DEBUG_557_112_2_17_23_44_47_3774 "tcpip: type %d is not supported for pkt %x"
#define TCPIP_DEBUG_571_112_2_17_23_44_47_3775 "tcpip: parse pkt(%x), len(%d) failed, print data:"
#define TCPIP_DEBUG_606_112_2_17_23_44_47_3776 "tcpip: PacketInfoIp() - invalid len %d < iplen %d"
#define TCPIP_DEBUG_866_112_2_17_23_44_47_3777 "[ TCPIP ] %s, pkt(%x):%x, netif(%x), IPID(%x), %s, %s(%d)"
#define TCPIP_DEBUG_872_112_2_17_23_44_47_3778 "[ TCPIP ] %s, pkt(%x):%x, netif(%x), IPID(%x), FRAG_OFFSET(%d), %s(%d)"
#define TCPIP_DEBUG_889_112_2_17_23_44_48_3779 "[ TCPIP ] %s, pkt(%x):%x, netif(%x), %s"
#define TCPIP_DEBUG_898_112_2_17_23_44_48_3780 "[ TCPIP ] %s, pkt(%x):%x, netif(%x), %s"
#define TCPIP_DEBUG_905_112_2_17_23_44_48_3781 "[ TCPIP ] %s, pkt(%x):%x, netif(%x), UNKNOWN TYPE(%d)"
#define CHAP_128_112_2_17_23_44_54_3782 "ppp ChapAuthWithPeer: link:%lx, our_name:%s, digest(type):%d"
#define CHAP_218_112_2_17_23_44_55_3783 "ppp Peer failed to respond to CHAP challenge"
#define CHAP_396_112_2_17_23_44_55_3784 "ppp ChapInput: rcvd short header."
#define CHAP_405_112_2_17_23_44_55_3785 "ppp ChapInput: rcvd illegal length."
#define CHAP_410_112_2_17_23_44_55_3786 "ppp ChapInput: rcvd short packet."
#define CHAP_433_112_2_17_23_44_55_3787 "ppp Unknown CHAP code (%d) received."
#define CHAP_468_112_2_17_23_44_55_3788 "ppp ChapReceiveChallenge: link %lx Rcvd id %d."
#define CHAP_474_112_2_17_23_44_55_3789 "ppp ChapReceiveChallenge: in state %d"
#define CHAP_480_112_2_17_23_44_55_3790 "ppp ChapReceiveChallenge: rcvd short packet."
#define CHAP_488_112_2_17_23_44_55_3791 "ppp ChapReceiveChallenge: rcvd short packet."
#define CHAP_500_112_2_17_23_44_55_3792 "ppp ChapReceiveChallenge: received name field: %s"
#define CHAP_508_112_2_17_23_44_55_3793 "ppp No CHAP secret found for authenticating us to %s"
#define CHAP_524_112_2_17_23_44_55_3794 "ppp MD5ing; id:%d, secret:%s, chal_len:%un"
#define CHAP_547_112_2_17_23_44_55_3795 "ppp MS-CHAPping; id:%d, secret:%s, chal_len:%un"
#define CHAP_555_112_2_17_23_44_55_3796 "ppp unknown digest type %d"
#define CHAP_590_112_2_17_23_44_55_3797 "ppp ChapReceiveResponse: link: %lx Rcvd id %d."
#define CHAP_596_112_2_17_23_44_55_3798 "ppp ChapReceiveResponse: in state %d"
#define CHAP_621_112_2_17_23_44_55_3799 "ppp ChapReceiveResponse: rcvd short packet."
#define CHAP_631_112_2_17_23_44_55_3800 "ppp ChapReceiveResponse: rcvd short packet."
#define CHAP_643_112_2_17_23_44_55_3801 "ppp ChapReceiveResponse: received name field: %s"
#define CHAP_654_112_2_17_23_44_55_3802 "ppp No CHAP secret found for authenticating %s"
#define CHAP_666_112_2_17_23_44_55_3803 "ppp MD5ing; id:%d, secret:%s, chal_len:%un"
#define CHAP_722_112_2_17_23_44_56_3804 "ppp MS-CHAP Authentication failure. Allowing retry. New ID is %d\n"
#define CHAP_742_112_2_17_23_44_56_3805 "ppp unknown digest type %d"
#define CHAP_768_112_2_17_23_44_56_3806 "ppp CHAP peer authentication failed"
#define CHAP_789_112_2_17_23_44_56_3807 "ppp ChapReceiveSuccess: link:%x Rcvd id %d."
#define CHAP_801_112_2_17_23_44_56_3808 "ppp ChapReceiveSuccess: in state %dn"
#define CHAP_830_112_2_17_23_44_56_3809 "ppp ChapReceiveFailure: link: %lx, Rcvd id %d."
#define CHAP_836_112_2_17_23_44_56_3810 "ppp ChapReceiveFailure: in state %dn"
#define CHAP_842_112_2_17_23_44_56_3811 "ppp CHAP authentication failed"
#define CHAP_885_112_2_17_23_44_56_3812 "ppp ChapSendChallenge: link: %lx, Sent id %d."
#define CHAP_932_112_2_17_23_44_56_3813 "ppp ChapSendStatus: Sent code %d, id %d."
#define IFPPP_212_112_2_17_23_44_57_3814 "ppp: del_ppp() - no mppp %x found"
#define IFPPP_401_112_2_17_23_44_57_3815 "tcpip: prep_ppp() - new_ppp failed, index %d"
#define IFPPP_501_112_2_17_23_44_58_3816 "tcpip: ppp_ifopen() error - no ppp interface found"
#define IFPPP_535_112_2_17_23_44_58_3817 "ppp error: ppp_pkt_send - invalid net %x or n_local %x for pkt %x"
#define IFPPP_545_112_2_17_23_44_58_3818 "ppp error: ppp_pkt_send - mppp->ifp(%x) != pkt->net(%x) for pkt %x"
#define IFPPP_566_112_2_17_23_44_58_3819 "ppp: ppp_pkt_send - retry dropped"
#define IFPPP_585_112_2_17_23_44_58_3820 "tcpip error: no space for MAC addr, nb_prot %x < nb_buff %x for pkt %x"
#define IFPPP_597_112_2_17_23_44_58_3821 "ppp warning: ppp_pkt_send - IPCP is not OPENED!"
#define IFPPP_900_112_2_17_23_44_58_3822 "ppp timeout (link %p), trys left: %dn"
#define IFPPP_1011_112_2_17_23_44_59_3823 "ppp link %p Auth FAILURE: %s failed authentication"
#define IFPPP_1055_112_2_17_23_44_59_3824 "ppp Updating Src IP\n"
#define IFPPP_1083_112_2_17_23_44_59_3825 "ppp: ppp_set_ipaddr - end TCP checksum loop"
#define IPCP_76_112_2_17_23_44_59_3826 "ppp: ipcp_resetci() - ouraddr: %u.%u.%u.%u"
#define IPCP_99_112_2_17_23_44_59_3827 "ppp Disabling VJ over PPPOE link %lx\n"
#define IPCP_220_112_2_17_23_44_59_3828 "ppp ipcp_addci: ouraddr==0.0.0.0\n"
#define IPCP_394_112_2_17_23_45_0_3829 "ppp local IP address %u.%u.%u.%un"
#define IPCP_400_112_2_17_23_45_0_3830 "ppp remote IP address %u.%u.%u.%un"
#define IPCP_443_112_2_17_23_45_0_3831 "ppp ipcp rcv NAK: bad VJ length %d\n"
#define IPCP_474_112_2_17_23_45_0_3832 "ppp ipcp NAK: DNS (0x%x) address %u.%u.%u.%u n"
#define IPCP_479_112_2_17_23_45_0_3833 "ppp ipcp rcv NAK: unknown option 0x%x\n"
#define IPCP_488_112_2_17_23_45_0_3834 "ppp ipcp odd NAK len\n"
#define IPCP_617_112_2_17_23_45_0_3835 "ppp ipcp_rejci: bad Rej pkt len\n"
#define IPCP_628_112_2_17_23_45_0_3836 "ppp ipcp_rejci: bad Rej pkt at opt 0x%x\n"
#define IPCP_693_112_2_17_23_45_0_3837 "ppp ipcp_reqci: bad CI length on opt 0x%x"
#define IPCP_708_112_2_17_23_45_0_3838 "ppp ipcp: received IPADDR - %u.%u.%u.%u"
#define IPCP_751_112_2_17_23_45_0_3839 "ppp %u.%u.%u.%u"
#define IPCP_768_112_2_17_23_45_1_3840 "ppp ipcp_reqci: ouraddr==0.0.0.0\n"
#define IPCP_776_112_2_17_23_45_1_3841 " \n"
#define IPCP_780_112_2_17_23_45_1_3842 "ppp ipcp: received COMPRESSTYPE "
#define IPCP_789_112_2_17_23_45_1_3843 "ppp (%d)"
#define IPCP_834_112_2_17_23_45_1_3844 "ppp ipcp: received DNSADDR(%x) - %u.%u.%u.%u"
#define IPCP_878_112_2_17_23_45_1_3845 "ppp (%s)\n"
#define IPCP_945_112_2_17_23_45_1_3846 "ppp ipcp: returning Configure-%s"
#define IPCP_977_112_2_17_23_45_1_3847 "ppp ipcp: up"
#define IPCP_985_112_2_17_23_45_1_3848 "ppp Could not determine remote IP address"
#define IPCP_994_112_2_17_23_45_1_3849 "ppp Peer is not authorized to use remote address %u.%u.%u.%u."
#define IPCP_1006_112_2_17_23_45_1_3850 "ppp Could not determine local IP address"
#define IPCP_1015_112_2_17_23_45_1_3851 "ppp IPCP: local  IP address %u.%u.%u.%u"
#define IPCP_1018_112_2_17_23_45_1_3852 "ppp IPCP: remote IP address %u.%u.%u.%u"
#define IPCP_1024_112_2_17_23_45_1_3853 "ppp setaddr failed"
#define IPCP_1044_112_2_17_23_45_1_3854 "ppp sifup failed"
#define IPCP_1092_112_2_17_23_45_1_3855 "ppp ipcp: down"
#define LCP_179_112_2_17_23_45_2_3856 "ppp LCP down."
#define LCP_217_112_2_17_23_45_2_3857 "ppp lcp_down: to call mppp->line.ln_disconnect()"
#define LCP_258_112_2_17_23_45_2_3858 "ppp lcp: Echo-Request, Rcvd id %d"
#define LCP_484_112_2_17_23_45_3_3859 "ppp: ci too big for buffer\n"
#define LCP_546_112_2_17_23_45_3_3860 "ppp rcvd NAK; "
#define LCP_552_112_2_17_23_45_3_3861 "ppp Bad LCP option 0x%x\n"
#define LCP_555_112_2_17_23_45_3_3862 "ppp opt:%s "
#define LCP_678_112_2_17_23_45_3_3863 "ppp lcp: Bad NAK option 0x%x\n"
#define LCP_692_112_2_17_23_45_3_3864 "ppp The line appears to be looped back; closing:"
#define LCP_698_112_2_17_23_45_3_3865 "ppp Got LCP NAK in OPENED state."
#define LCP_700_112_2_17_23_45_3_3866 "\n"
#define LCP_704_112_2_17_23_45_3_3867 "ppp lcp: rcvd bad NAK (option:0x%x)\n"
#define LCP_739_112_2_17_23_45_3_3868 "ppp lcp reject of 0x%x\n"
#define LCP_819_112_2_17_23_45_3_3869 "ppp lcp: REG for bad option 0x%x\n"
#define LCP_832_112_2_17_23_45_3_3870 "ppp lcp_rejci: bad; len %d opt: 0x%x"
#define LCP_895_112_2_17_23_45_3_3871 "ppp lcp_reqci: bad CI length!"
#define LCP_907_112_2_17_23_45_3_3872 "ppp lcp_reqci: rcvd MRU"
#define LCP_914_112_2_17_23_45_3_3873 "ppp (%d)"
#define LCP_931_112_2_17_23_45_3_3874 "ppp lcp_reqci: rcvd ASYNCMAP"
#define LCP_938_112_2_17_23_45_3_3875 "ppp (%lx)"
#define LCP_972_112_2_17_23_45_3_3876 "ppp lcp_reqci: rcvd AUTHTYPE"
#define LCP_1030_112_2_17_23_45_4_3877 "ppp (%x)"
#define LCP_1060_112_2_17_23_45_4_3878 "ppp Bad MD5 digest type %x, NAKing\n"
#define LCP_1082_112_2_17_23_45_4_3879 "ppp lcp_reqci: rcvd QUALITY"
#define LCP_1093_112_2_17_23_45_4_3880 "ppp (%x %lx)"
#define LCP_1108_112_2_17_23_45_4_3881 "ppp lcp_reqci: rcvd MAGICNUMBER"
#define LCP_1115_112_2_17_23_45_4_3882 "ppp (%lx)"
#define LCP_1134_112_2_17_23_45_4_3883 "ppp lcp_reqci: rcvd PCOMPRESSION"
#define LCP_1144_112_2_17_23_45_4_3884 "ppp lcp_reqci: rcvd ACCOMPRESSION"
#define LCP_1159_112_2_17_23_45_4_3885 "ppp lcp_reqci: rcvd MULTILINK_MRU"
#define LCP_1170_112_2_17_23_45_4_3886 "ppp lcp_reqci: rcvd MULTILINK_SSN"
#define LCP_1179_112_2_17_23_45_4_3887 "ppp lcp_reqci: rcvd MULTILINK_ENDPT"
#define LCP_1199_112_2_17_23_45_4_3888 "ppp lcp_reqci: rcvd unknown option %d"
#define LCP_1205_112_2_17_23_45_4_3889 "ppp (%s)\n"
#define LCP_1246_112_2_17_23_45_4_3890 "ppp lcp_reqci: returning CONF%s.\n"
#define LCP_1275_112_2_17_23_45_4_3891 "ppp lcp_finished: to call mppp->line.ln_disconnect()"
#define LCP_1291_112_2_17_23_45_4_3892 "ppp lcp_starting: to call mppp->line.ln_connect()"
#define PPP_DHCP_61_112_2_17_23_45_4_3893 "ppp_start_dhcp: ppp_timeout in %d secs\n"
#define PPP_DHCP_81_112_2_17_23_45_4_3894 "ppp dhcp_timeout: iface %d\n"
#define PPP_DHCP_124_112_2_17_23_45_4_3895 "DHCP assigned PPP link IP=%u.%u.%u.%un"
#define PPP_LOOP_172_112_2_17_23_45_5_3896 "ppp lbxover: no server\n"
#define PPP_LOOP_179_112_2_17_23_45_5_3897 "ppp lbxover: no client index\n"
#define PPPCHAR_155_112_2_17_23_45_6_3898 "ppp_inchar: no bigger buffer, size %d\n"
#define PPPCHAR_180_112_2_17_23_45_6_3899 "ppp_inchar: no bigger buffer, size %dn"
#define PPPCHAR_203_112_2_17_23_45_6_3900 "ppp_inchar: got PPP_FLAG "
#define PPPCHAR_225_112_2_17_23_45_6_3901 "ppp_inchar: flushed, ppp:%p, flushed=%dn"
#define PPPCHAR_238_112_2_17_23_45_6_3902 "ppp_inchar: pkt is null\n"
#define PPPCHAR_242_112_2_17_23_45_6_3903 "ppp_inchar: bad fcs, ppp:%p, len %d, data:\n"
#define PPPCHAR_255_112_2_17_23_45_6_3904 "ppp text: %s\n"
#define PPPCHAR_271_112_2_17_23_45_6_3905 "ppp_inchar: pkt is null\n"
#define PPPCHAR_280_112_2_17_23_45_6_3906 "ppp_inchar: ppp:%p short pkt (%d)\n"
#define PPPCHAR_346_112_2_17_23_45_6_3907 "ppp_inchar: pkt is null\n"
#define PPPCHAR_358_112_2_17_23_45_6_3908 "ppp %p: missing ALLSTATIONS, got 0x%x; flags %xn"
#define PPPCHAR_370_112_2_17_23_45_6_3909 "ppp %p: missing UI, got 0x%x\n"
#define PPPCHAR_476_112_2_17_23_45_6_3910 "ppp: send buffer check failed, discard pkt(%p), len = %d"
#define PPPCHAR_488_112_2_17_23_45_6_3911 "ppp: get uart semaphore error, discard pkt %p!"
#define PPPCHAR_495_112_2_17_23_45_6_3912 "ppp: uart tx buffer is not enough, discard pkt(%p), len = %d"
#define PPPCHAR_510_112_2_17_23_45_6_3913 "ppp: ppp_sendchars() - mppp(%x), link_id(%d), pkt(%x), len(%d)"
#define PPPCHAR_512_112_2_17_23_45_6_3914 "ppp: ppp_sendchars() - pkt(%x), len(%d) "
#define PPPCHAR_523_112_2_17_23_45_6_3915 "ppp: ln_putc error %d on link %p\n"
#define PPPCHAR_598_112_2_17_23_45_7_3916 "ppp fcs not ok ??\n"
#define PPPCHAR_629_112_2_17_23_45_7_3917 "ppp fcs not ok ??\n"
#define PPPFSM_273_112_2_17_23_45_7_3918 "ppp link %p rcvd:"
#define PPPFSM_305_112_2_17_23_45_8_3919 "ppp (%p) pkalloc failed for VJ pkt len %dn"
#define PPPFSM_326_112_2_17_23_45_8_3920 "ppp (%p) vj_uncompress failed on type %xn"
#define PPPFSM_367_112_2_17_23_45_8_3921 "ppp_inpkt: modem hung up\n"
#define PPPFSM_399_112_2_17_23_45_8_3922 "ppp_inpkt: (link %p): unsupported prot %x\n"
#define PPPFSM_420_112_2_17_23_45_8_3923 "ppp (link %p) truncated packet, lengths: hdr:%d, actual:%d\n"
#define PPPFSM_429_112_2_17_23_45_8_3924 "ppp_inpkt (link %p); prot: %s, code %s, state %s, len %dn"
#define PPPFSM_435_112_2_17_23_45_8_3925 "ppp (link %p) got pkt in state %sn"
#define PPPFSM_491_112_2_17_23_45_8_3926 "ppp (link %p) Prot-Reject for IPCP, closing\n"
#define PPPFSM_498_112_2_17_23_45_8_3927 "ppp (link %p) Prot-Reject for %x\n"
#define PPPFSM_535_112_2_17_23_45_8_3928 "ppp rcvd IDENTITY: %s\n"
#define PPPFSM_545_112_2_17_23_45_8_3929 "ppp (link %p) unknown code: %x"
#define PPPFSM_612_112_2_17_23_45_8_3930 "ppp fsm_input (link %p): bad state: %d, event:%d."
#define PPPFSM_621_112_2_17_23_45_8_3931 "ppp_fsm (link %p): %s state: %s -> %sn"
#define PPPFSM_786_112_2_17_23_45_9_3932 "ppp_fsm (link %p) sending %s, len:%dn"
#define PPPFSM_811_112_2_17_23_45_9_3933 "ppp LCP (%p) failed, closing IPCP\n"
#define PPPFSM_852_112_2_17_23_45_9_3934 "ppp_fsm_init (link %p)\n"
#define PPPFSM_1002_112_2_17_23_45_9_3935 "ppp allpktsend - DHCP pending?\n"
#define PPPFSM_1052_112_2_17_23_45_9_3936 "ppp link %p sending:"
#define PPPFSM_1061_112_2_17_23_45_9_3937 "PPP LOSSY_IO dropping TX (link %p)\n"
#define PPPFSM_1073_112_2_17_23_45_9_3938 "ppp_allpktsend (%p): lost line\n"
#define PPPFSM_1157_112_2_17_23_45_9_3939 "ppp lowerup: (link %p) %s\n"
#define PPPFSM_1235_112_2_17_23_45_9_3940 "ppp lowerdown (link %p): %s\n"
#define PPPFSM_1251_112_2_17_23_45_9_3941 "ppp open (link %p): %s\n"
#define PPPFSM_1256_112_2_17_23_45_9_3942 "ppp: ppp_open(), to open IPCP but LCP still not opened!"
#define PPPFSM_1286_112_2_17_23_45_9_3943 "ppp close (link %p): %s\n"
#define PPPFSM_1324_112_2_17_23_45_10_3944 "ppp %02x "
#define PPPFSM_1325_112_2_17_23_45_10_3945 "\n"
#define PPPMENU_408_112_2_17_23_45_11_3946 "ppp menu: link up on link %p\n"
#define PPPMENU_451_112_2_17_23_45_11_3947 "ppp menu: link down on link %p\n"
#define PPPSYS_297_112_2_17_23_45_13_3948 "ppp: ppp_line_init - want ouraddr: %u.%u.%u.%u"
#define PPPSYS_794_112_2_17_23_45_14_3949 "ppp: can't hold new addr %08x on mppp(%p), netif(%p)"
#define UPAP_184_112_2_17_23_45_14_3950 "ppp link %lx: No response to PAP authenticate-requests"
#define UPAP_293_112_2_17_23_45_15_3951 "ppp PAP authentication failed due to protocol-reject"
#define UPAP_298_112_2_17_23_45_15_3952 "ppp PAP authentication of peer failed (protocol-reject)"
#define UPAP_327_112_2_17_23_45_15_3953 "ppp upap_input: rcvd short header."
#define UPAP_336_112_2_17_23_45_15_3954 "ppp upap_input link %lx: runt, len: %d."
#define UPAP_340_112_2_17_23_45_15_3955 "ppp upap_input link %lx: code: %s, id %d, len %dn"
#define UPAP_346_112_2_17_23_45_15_3956 "ppp upap_input: unknown code %d\n"
#define UPAP_411_112_2_17_23_45_15_3957 "ppp upap_rauth: rcvd short packet."
#define UPAP_420_112_2_17_23_45_15_3958 "ppp upap_rauth: rcvd short packet."
#define UPAP_428_112_2_17_23_45_15_3959 "ppp upap_rauth: rcvd short packet."
#define UPAP_476_112_2_17_23_45_15_3960 "ppp upap_rauthack: rx pkt in state %d\n"
#define UPAP_486_112_2_17_23_45_15_3961 "ppp upap_rauthack: runt, len: %d."
#define UPAP_530_112_2_17_23_45_15_3962 "ppp upap_rauthnak: runt, len %d\n"
#define UPAP_536_112_2_17_23_45_15_3963 "ppp Link %lx: PAP auth failed\n"
#define UPAP_578_112_2_17_23_45_15_3964 "ppp upap_sauth: Sent id %d."
#define UPAP_624_112_2_17_23_45_15_3965 "ppp upap_sresp: Sent code %d, id %d."
#define DHCPCLNT_149_112_2_17_23_45_17_3966 "tcpip: dcp_init_iface - invalid iface %d"
#define DHCPCLNT_161_112_2_17_23_45_17_3967 "tcpip: dcp_init_iface - udp_open error on iface %d"
#define DHCPCLNT_204_112_2_17_23_45_18_3968 "tcpip: dhc_callback() - invalid iface %d"
#define DHCPCLNT_213_112_2_17_23_45_18_3969 "tcpip: get IP address from DHCP on netif(%p), desc(%s), pkttype(%d)"
#define DHCPCLNT_216_112_2_17_23_45_18_3970 "- tcpip: haddr   - %02X:%02X:%02X:%02X:%02X:%02X"
#define DHCPCLNT_217_112_2_17_23_45_18_3971 "- tcpip: ipaddr  - %s"
#define DHCPCLNT_218_112_2_17_23_45_18_3972 "- tcpip: snmask  - %s"
#define DHCPCLNT_219_112_2_17_23_45_18_3973 "- tcpip: gateway - %s"
#define DHCPCLNT_220_112_2_17_23_45_18_3974 "- tcpip: dnssrv1 - %s"
#define DHCPCLNT_221_112_2_17_23_45_18_3975 "- tcpip: dnssrv2 - %s"
#define DHCPCLNT_222_112_2_17_23_45_18_3976 "- tcpip: dhcpsrv - %s"
#define DHCPCLNT_254_112_2_17_23_45_18_3977 "tcpip: dhc_callback() - wrong notify type!"
#define DHCPCLNT_260_112_2_17_23_45_18_3978 "tcpip: dhc_callback() - DHCP is already bound, no notify!"
#define DHCPCLNT_269_112_2_17_23_45_18_3979 "tcpip: DHCP request timeout on netif %x, state %s"
#define DHCPCLNT_293_112_2_17_23_45_18_3980 "tcpip: dhc_callback() - wrong notify type"
#define DHCPCLNT_358_112_2_17_23_45_18_3981 "tcpip: dhc_upcall() - DHCP upcall not matched"
#define DHCPCLNT_366_112_2_17_23_45_18_3982 "tcpip: dhc_upcall() - DHCP is UNUSED on netif %x"
#define DHCPCLNT_377_112_2_17_23_45_18_3983 "tcpip: dhc_upcall() - invalid DHCP data fields on netif %x"
#define DHCPCLNT_385_112_2_17_23_45_18_3984 "tcpip: dhc_upcall() - DHCP data on netif %x is not for me"
#define DHCPCLNT_406_112_2_17_23_45_18_3985 "tcpip: dhc_upcall() - wrong DHCP type %d"
#define DHCPCLNT_420_112_2_17_23_45_18_3986 "tcpip: dhc_upcall() - wrong DHCP state %s on netif %x"
#define DHCPCLNT_432_112_2_17_23_45_18_3987 "tcpip: dhcpclnt.c(%d) - dhc_upcall()"
#define DHCPCLNT_448_112_2_17_23_45_18_3988 "tcpip: dhc_upcall() - dhc_rx_offer() error %d"
#define DHCPCLNT_471_112_2_17_23_45_18_3989 "tcpip: dhc_upcall() - receive DHCP type %d in DHCS_SELECTING on netif %x"
#define DHCPCLNT_487_112_2_17_23_45_18_3990 "tcpip: dhcpclnt.c(%d) - dhc_upcall()"
#define DHCPCLNT_495_112_2_17_23_45_18_3991 "tcpip: dhcpclnt.c(%d) - relay agent isn't match"
#define DHCPCLNT_509_112_2_17_23_45_18_3992 "tcpip: netif(%x), DHCP renew time - INFINITE, rebind time - INFINITE"
#define DHCPCLNT_516_112_2_17_23_45_18_3993 "tcpip: netif(%x), DHCP renew time - %ds, rebind time - %ds"
#define DHCPCLNT_523_112_2_17_23_45_18_3994 "tcpip: dhcpclnt.c(%d) - dhc_upcall()"
#define DHCPCLNT_550_112_2_17_23_45_18_3995 "tcpip: dhcpclnt.c(%d) - dhc_upcall()"
#define DHCPCLNT_558_112_2_17_23_45_19_3996 "tcpip: dhc_upcall() - bad DHCP state %s on netif %x"
#define DHCPCLNT_579_112_2_17_23_45_19_3997 "tcpip: DHCP state on netif %x is %s"
#define DHCPCLNT_619_112_2_17_23_45_19_3998 "tcpip: dhc_buildheader - illegal netif(%d) type - %d"
#define DHCPCLNT_629_112_2_17_23_45_19_3999 "tcpip: dhc_buildheader() -dhc_states[iface].rev_dhc_flag check begin..."
#define DHCPCLNT_633_112_2_17_23_45_19_4000 "tcpip: dhc_buildheader() - use broadcast!"
#define DHCPCLNT_638_112_2_17_23_45_19_4001 "tcpip: dhc_buildheader() -just use unicast"
#define DHCPCLNT_642_112_2_17_23_45_19_4002 "tcpip: dhc_buildheader() -dhc_states[iface].rev_dhc_flag= %x"
#define DHCPCLNT_647_112_2_17_23_45_19_4003 "tcpip: dhc_buildheader() -renewing use unicast"
#define DHCPCLNT_657_112_2_17_23_45_19_4004 "tcpip: dhc_buildheader() - invalid netif(%d) haddr len %d!"
#define DHCPCLNT_698_112_2_17_23_45_19_4005 "tcpip: dhc_discover() - udp_alloc failed!"
#define DHCPCLNT_708_112_2_17_23_45_19_4006 "tcpip: dhc_discover() -xid is %x"
#define DHCPCLNT_797_112_2_17_23_45_19_4007 "tcpip: dhc_rx_offer() - begin ntohl(bp->xid) =%x,ntohl(dhc_states[iface].xid)=%x"
#define DHCPCLNT_801_112_2_17_23_45_19_4008 "tcpip: dhc_rx_offer() -ERROR  the xid that replied from the server is larger than the xid that client used to send..."
#define DHCPCLNT_807_112_2_17_23_45_19_4009 "tcpip: dhc_rx_offer() - received DHCP type invalid %x, xid = %x"
#define DHCPCLNT_818_112_2_17_23_45_19_4010 "tcpip: dhc_rx_offer() - the sending type is unicast"
#define DHCPCLNT_823_112_2_17_23_45_19_4011 "tcpip: dhc_rx_offer() - the sending type is broadcast"
#define DHCPCLNT_828_112_2_17_23_45_19_4012 "tcpip: dhc_rx_offer() - xid match on netif %x, ours %x, recv %x successfully"
#define DHCPCLNT_833_112_2_17_23_45_19_4013 "tcpip: dhc_rx_offer() - xid not match on netif %x, ours %x, recv %x"
#define DHCPCLNT_841_112_2_17_23_45_19_4014 "tcpip: dhc_rx_offer() - dhc_extract_opts error on netif %d"
#define DHCPCLNT_847_112_2_17_23_45_19_4015 "tcpip: dhc_rx_offer() - server does not give our IP address"
#define DHCPCLNT_913_112_2_17_23_45_19_4016 "tcpip: dhc_request() - no UDP buffer"
#define DHCPCLNT_923_112_2_17_23_45_19_4017 "tcpip: dhc_request() - xids is %d"
#define DHCPCLNT_931_112_2_17_23_45_19_4018 "tcpip: dhc_request() - dhc_buildheader() error %d"
#define DHCPCLNT_1134_112_2_17_23_45_20_4019 "tcpip: fixup_subnet_mask - IP address 0x%x is bad"
#define DHCPCLNT_1459_112_2_17_23_45_20_4020 "tcpip: dhc_second() - dhc_discover() error %d on state %d"
#define DHCPCLNT_1471_112_2_17_23_45_20_4021 "tcpip: dhc_second() - dhc_reclaim() error %d on state %d"
#define DHCPCLNT_1503_112_2_17_23_45_20_4022 "tcpip: dhc_second() - bogus state %d"
#define DHCPCLNT_1558_112_2_17_23_45_21_4023 "tcpip: dhc_second() - dhc_reclaim() error %d on state %d"
#define DHCPCLNT_1591_112_2_17_23_45_21_4024 "tcpip: dhc_second() - dhc_request() error %d on state %d"
#define DHCPCLNT_1623_112_2_17_23_45_21_4025 "tcpip: dhc_halt error - invalid iface!"
#define DHCPCLNT_1628_112_2_17_23_45_21_4026 "tcpip: deact dhcp client on netif %x"
#define DHCPCLNT_1657_112_2_17_23_45_21_4027 "tcpip: dhc_reclaim() - ip on netif %x is NULL"
#define DHCPCLNT_1678_112_2_17_23_45_21_4028 "tcpip: dhc_reclaim() - add_route for server %08x on netif %x failed"
#define DHCPCLNT_1686_112_2_17_23_45_21_4029 "tcpip: dhc_reclaim() - server IP on netif %x is NULL"
#define DHCPCLNT_1794_112_2_17_23_45_21_4030 "tcpip: dhc set state %s"
#define DNSCLNT_175_112_2_17_23_45_22_4031 "tcpip: dnc_sendreq() - no DNS on netif %x"
#define DNSCLNT_279_112_2_17_23_45_22_4032 "tcpip warning: netif of dns_qs %d is NULL"
#define DNSCLNT_515_112_2_17_23_45_23_4033 "tcpip: DNS error - %s; flags %x"
#define DNSCLNT_901_112_2_17_23_45_23_4034 "tcpip: t_gethostbyname p_hostent->h_addr_list is NULL!"
#define DNSCLNT_920_112_2_17_23_45_23_4035 "tcpip: t_gethostbyname() - no DNS on netif %p"
#define DNSCLNT_1286_112_2_17_23_45_24_4036 "tcpip: dnsquery addr(0x%x ~ 0x%x), size %d, num %d"
#define EAPOL_80_112_2_17_23_45_25_4037 "tcpip: sci_eapol_tx() - netif %p should use ETHERNET packet"
#define EAPOL_109_112_2_17_23_45_25_4038 "tcpip: sci_eapol_tx() - invalid pkt %p, nb_prot %p, nb_buff %p"
#define EAPOL_131_112_2_17_23_45_25_4039 "tcpip: sci_eapol_tx() - no send ptr for pkt %p"
#define EAPOL_167_112_2_17_23_45_25_4040 "tcpip: set eapol rx callback ptr %p on netif %p"
#define EAPOL_207_112_2_17_23_45_25_4041 "tcpip: no eapol rx fptr for pkt %p on netif %p"
#define NETIF_181_112_2_17_23_45_27_4042 "tcpip: use spare dns on netif %x, dns1(%08x), dns2(%08x)"
#define NETIF_189_112_2_17_23_45_27_4043 "tcpip: use global spare dns, dns1(%08x), dns2(%08x)"
#define NETIF_224_112_2_17_23_45_27_4044 "tcpip: use default netif %x"
#define NETIF_288_112_2_17_23_45_27_4045 "tcpip warning: netif %x with seq %x is not opened"
#define NETIF_405_112_2_17_23_45_27_4046 "tcpip: get throughput on netif 0x%x, in(%d), out(%d)"
#define NETIF_427_112_2_17_23_45_27_4047 "tcpip: clear throughput on netif 0x%x"
#define NETIF_479_112_2_17_23_45_27_4048 "tcpip: set spare dns on netif 0x%x, dns1 %s, dns2 %s"
#define NETIF_517_112_2_17_23_45_27_4049 "tcpip: set global spare dns, dns1 %s, dns2 %s"
#define PKTALLOC_201_112_2_17_23_45_29_4050 "tcpip: pk_alloc() fail - buffer wanted oversize, %d > %d"
#define PKTALLOC_213_112_2_17_23_45_29_4051 "tcpip: pk_alloc() - bigfreeq = %d, lilfreeq = %d"
#define PKTALLOC_240_112_2_17_23_45_29_4052 "tcpip: pk_alloc() - alloc fail for data len %d"
#define PKTALLOC_261_112_2_17_23_45_29_4053 "tcpip: pk_alloc() - alloc buffer for pkt %p failed!"
#define PKTALLOC_329_112_2_17_23_45_29_4054 "tcpip: pk_cpalloc() invalid prot_bias(%x)"
#define PKTALLOC_338_112_2_17_23_45_29_4055 "tcpip: pk_cpalloc() from pkt %p with new data len %d failed!"
#define PKTALLOC_449_112_2_17_23_45_29_4056 "tcpip: pk_free - buffer %p already in bigfreeq"
#define PKTALLOC_458_112_2_17_23_45_29_4057 "tcpip: pk_free - buffer %p already in lilfreeq"
#define PKTALLOC_536_112_2_17_23_45_29_4058 "tcpip: pkt_big(%d) - %d; pkt_lil(%d) - %d "
#define PKTALLOC_543_112_2_17_23_45_29_4059 "tcpip error: packet static buffer is not enough!"
#define PKTALLOC_549_112_2_17_23_45_29_4060 "tcpip debug: no-used static net buffer size %d"
#define PKTALLOC_741_112_2_17_23_45_30_4061 "tcpip: netbuff addr(0x%x ~ 0x%x), size %d, bigbufs %d, lilbufs %d"
#define PKTALLOC_768_112_2_17_23_45_30_4062 "tcpip: netbuff addr 0x%x  was corrupted !"
#define PKTALLOC_775_112_2_17_23_45_30_4063 "tcpip: netbuff addr 0x%x  was corrupted !"
#define Q_TXCNF_143_112_2_17_23_45_30_4064 "tcpip warning: no matched info %p found in txcnfq, qlen = %d"
#define TX_FLOWCTRL_79_112_2_17_23_45_32_4065 "tcpip: drop pending pkt(%x), len(%d)"
#define TX_FLOWCTRL_123_112_2_17_23_45_32_4066 "tcpip: send pending pkt(%p, %d)"
#define TX_FLOWCTRL_147_112_2_17_23_45_32_4067 "tcpip: empty tx pending queue(len: %d) on netif(%x)"
#define TX_FLOWCTRL_261_112_2_17_23_45_33_4068 "tcpip: alloc backup pkt memory failed, %d"
#define TX_FLOWCTRL_331_112_2_17_23_45_33_4069 "tcpip: no backup packet on netif %x"
#define TX_FLOWCTRL_339_112_2_17_23_45_33_4070 "tcpip: invalid packet bias %d"
#define TX_FLOWCTRL_350_112_2_17_23_45_33_4071 "tcpip: pk_alloc failed for backup packet tx on netif %x"
#define TX_FLOWCTRL_371_112_2_17_23_45_33_4072 "tcpip: to send backup packet %x on netif %x"
#define TX_FLOWCTRL_458_112_2_17_23_45_33_4073 "tcpip: delete pending pkt %x with received ack %x"
#define TX_FLOWCTRL_481_112_2_17_23_45_33_4074 "tcpip: invalid packet buffer len %d"
#define TX_FLOWCTRL_573_112_2_17_23_45_33_4075 "tcpip: drop pkt %x from tx pending queue on netif 0x%x"
#define IN_PCB_168_112_2_17_23_45_34_4076 "tcpip: in_pcbconnect sin->sin_addr = %x, sin->sin_port = %d"
#define IN_PCB_176_112_2_17_23_45_34_4077 "tcpip: in_pcbconnect() - sin->sin_port == 0"
#define IN_PCB_193_112_2_17_23_45_34_4078 "tcpip: in_pcbconnect() error - inp == NULL || inp->ifp == NULL"
#define IN_PCB_199_112_2_17_23_45_34_4079 "tcpip: in_pcbconnect() error - sin->sin_addr.s_addr == INADDR_BROADCAST"
#define IN_PCB_214_112_2_17_23_45_34_4080 "tcpip: in_pcbconnect() error - iproute() failed!"
#define NPTCP_260_112_2_17_23_45_36_4081 "tcpip: no pkt avail for mbuf MT_TXDATA, len = %d"
#define NPTCP_269_112_2_17_23_45_36_4082 "tcpip: no pkt avail for mbuf %d, len = %d"
#define NPTCP_304_112_2_17_23_45_36_4083 "tcpip: m_getnbuf() - invalid mbuf type %d"
#define NPTCP_338_112_2_17_23_45_36_4084 "tcpip: m_getnbuf - getq fail"
#define NPTCP_884_112_2_17_23_45_37_4085 "tcpip: tcp_rcv() - BAD_TCP_HEAD(%d): bad chksum, pkt %08x"
#define NPTCP_1190_112_2_17_23_45_37_4086 "tcpip msslimit: change pkt(%p) mss %d -> %d"
#define NPTCP_1281_112_2_17_23_45_38_4087 "tcpip debug: np_stripoptions - ihlen %d"
#define NPTCP_1828_112_2_17_23_45_39_4088 "tcpip: mbuf addr(0x%x ~ 0x%x), size %d, num %d"
#define SOCKCALL_88_112_2_17_23_45_40_4089 "tcpip: creat socket 0x%x, type = %d, netif = %x"
#define SOCKCALL_121_112_2_17_23_45_40_4090 "tcpip: modify so %x with netif %x is not allowed"
#define SOCKCALL_126_112_2_17_23_45_40_4091 "tcpip: modify so %x netif %x -> %x"
#define SOCKCALL_213_112_2_17_23_45_41_4092 "tcpip: t_bind - so %x bind error %d"
#define SOCKCALL_376_112_2_17_23_45_41_4093 "tcpip: t_connect() - so %x, error ENETUNREACH"
#define SOCKCALL_415_112_2_17_23_45_41_4094 "tcpip: t_connect() - so %x is connected, %08x:%d -> %08x:%d"
#define SOCKCALL_423_112_2_17_23_45_41_4095 "tcpip: t_connect() - refuse so %x connection, %08x:%d -> %08x:%d"
#define SOCKCALL_470_112_2_17_23_45_41_4096 "tcpip: t_connect() - so %x, error ENETUNREACH"
#define SOCKCALL_740_112_2_17_23_45_42_4097 "tcpip: t_recv() - so %x, error ENETUNREACH"
#define SOCKCALL_760_112_2_17_23_45_42_4098 "tcpip: t_recv() - so %x received %d, total %d, left %d"
#define SOCKCALL_825_112_2_17_23_45_42_4099 "tcpip: t_recvfrom() - so %x, error %d, ENETUNREACH"
#define SOCKCALL_844_112_2_17_23_45_42_4100 "tcpip: t_recvfrom() - so %x received %d, total %d, left %d"
#define SOCKCALL_973_112_2_17_23_45_42_4101 "tcpip: t_sendto() - so %x, error %d, ENETUNREACH"
#define SOCKCALL_992_112_2_17_23_45_42_4102 "tcpip: t_sendto() - so %x sent %d, total %d, left %d"
#define SOCKCALL_1120_112_2_17_23_45_42_4103 "tcpip: t_send() - so %x, error %d, ENETUNREACH"
#define SOCKCALL_1132_112_2_17_23_45_42_4104 "tcpip: t_send() - so %x sent %d, total %d, left %d"
#define SOCKCALL_1198_112_2_17_23_45_42_4105 "tcpip: close socket 0x%x, tot_snd %d, tot_rcv %d, snd_left %d, rcv_left %d"
#define SOCKCALL_1209_112_2_17_23_45_43_4106 "tcpip: t_socketclose() - so %x, err %d"
#define SOCKET_262_112_2_17_23_45_43_4107 "tcpip: sofree() - to free so 0x%x, tot_snd = %d, tot_rcv = %d"
#define SOCKET_367_112_2_17_23_45_43_4108 "tcpip: soclose(), so %x with SS_NOFDREF directly return "
#define SOCKET_792_112_2_17_23_45_44_4109 "tcpip: clear so_rcv cr20961***!!!\"#"
#define SOCKET_1187_112_2_17_23_45_45_4110 "tcpip: actually set so %x with TCP_ACKDELAYTIME value %d ms"
#define SOCKET_1263_112_2_17_23_45_45_4111 "tcpip: actually set so %x with TCP_MAXSEG value %d"
#define SOCKET_1492_112_2_17_23_45_46_4112 "tcpip: forced_sofree() - to free TCP so %p"
#define SOCKET2_113_112_2_17_23_45_46_4113 "tcpip warning: soisconnected() - soqremque on so %x error"
#define SOCKET2_226_112_2_17_23_45_46_4114 "tcpip: sonewconn() - new socket %x, type = %d, netif = %x"
#define SOCKET2_707_112_2_17_23_45_47_4115 "tcpip:the sb->sb_cc = %d"
#define SOSELECT_386_112_2_17_23_45_48_4116 "tcpip: FD_CLR() - no so %x found in list %p"
#define SOSELECT_410_112_2_17_23_45_48_4117 "tcpip: FD_SET() - so %x is already set in list %p"
#define SOSELECT_418_112_2_17_23_45_48_4118 "tcpip: FD_SET() - list %p is full for so %x"
#define TCP_IN_355_112_2_17_23_45_49_4119 "[ TCPIPINFO ] Remote TCP sent RST(reset)"
#define TCP_IN_834_112_2_17_23_45_50_4120 "[ TCPIPINFO ] socket 0x%x has establised!"
#define TCP_IN_1044_112_2_17_23_45_51_4121 "tcpip debug: delay to send abnormal close event on so %x"
#define TCP_IN_1130_112_2_17_23_45_51_4122 "[ TCPIPINFO ] socket 0x%x has establised!"
#define TCP_IN_1599_112_2_17_23_45_52_4123 "[ TCPIPINFO ] Remote closed TCP connection by sending 'FIN'"
#define TCP_IN_1610_112_2_17_23_45_52_4124 "tcpip debug: delay to send normal close event on so %x"
#define TCP_IN_1822_112_2_17_23_45_52_4125 "tcpip: so %x, TCP local winscale %d, peer winscale %d"
#define TCP_IN_2036_112_2_17_23_45_53_4126 "tcpip tcpoptmz: tcp_xmit_timer() - tp(%x), t_rxtcur( M:%d, A:%d )"
#define TCP_OUT_237_112_2_17_23_45_54_4127 "tcpip: tcp_output(%d) - allocate memory failed!"
#define TCP_OUT_304_112_2_17_23_45_54_4128 "[ TCPIPINFO ] Local sent TCP_RST(reset)"
#define TCP_OUT_1000_112_2_17_23_45_55_4129 "tcpip exception: cp(%p) overflow tcp_optionbuf(%p, len:%d)"
#define TCP_SUBR_316_112_2_17_23_45_56_4130 "tcpip tcpoptmz: tcp_newtcpcb() - tp(%x), t_rxtcur( M:%d, A:%d ), rxt_shift(%d)"
#define TCP_TIMR_305_112_2_17_23_45_57_4131 "tcpip tcpoptmz: tcp_timers() - tp(%x), t_rxtcur( M:%d, A:%d ), rxt_shift(%d)"
#define TCP_TIMR_314_112_2_17_23_45_57_4132 "[ TCPIPINFO ] Local TCP totally retransmitted %d times, %d ms."
#define TCP_USR_506_112_2_17_23_45_59_4133 "[T CPIPINFO ] Local closed TCP connection by sending 'FIN' first!"
#define TCPPORT_128_112_2_17_23_46_0_4134 "tcpip: set TCP MSS = %d"
#define TCPPORT_156_112_2_17_23_46_0_4135 "tcpip: set global TCP send space %d"
#define UDPSOCK_88_112_2_17_23_46_2_4136 "tcpip warning: so %x recv buff not enough, drop pkt %x"
#define FAKETCP_THROUGHUP_114_112_2_17_23_46_3_4137 "tcpip test: FakeTcpThroughupInit failed!"
#define FAKETCP_THROUGHUP_218_112_2_17_23_46_3_4138 "tcpip test: THROUGHUP_FTCP_Test() to start!"
#define FAKETCP_THROUGHUP_223_112_2_17_23_46_3_4139 "tcpip test: THROUGHUP_FTCP_Test() to end!"
#define FAKETCP_THROUGHUP_248_112_2_17_23_46_3_4140 "tcpip test: fake TCP up-throughput task is already existed!"
#define FAKETCP_THROUGHUP_255_112_2_17_23_46_3_4141 "tcpip test: FakeTcpThroughupInit failed!"
#define FAKETCP_THROUGHUP_275_112_2_17_23_46_3_4142 "tcpip test: fake TCP throughup task ID : 0x%x "
#define FAKETCP_THROUGHUP_276_112_2_17_23_46_3_4143 "tcpip test: priority of fake TCP throughup task: %d "
#define FAKETCP_THROUGHUP_317_112_2_17_23_46_3_4144 "tcpip debug: create TCP on PORT %d <-> %d failed"
#define FAKETCP_THROUGHUP_323_112_2_17_23_46_3_4145 "tcpip debug: to start FakeTcpThroughupTest - on %s:%d  <-> %s:%d"
#define FAKETCP_THROUGHUP_354_112_2_17_23_46_3_4146 "tcpip debug: FakeTcpThroughupTest - totally sent %d bytes on PORT %d <-> %d"
#define FAKETCP_THROUGHUP_535_112_2_17_23_46_4_4147 "tcpip: CreateTcpPacket - packet buffer not enough"
#define MODEM_THROUGHUP_122_112_2_17_23_46_4_4148 "tcpip test: CreateUdpPacket() failed!"
#define MODEM_THROUGHUP_134_112_2_17_23_46_4_4149 "tcpip test: MODEMTHROUGHUP_Test() to start!"
#define MODEM_THROUGHUP_139_112_2_17_23_46_4_4150 "tcpip test: MODEMTHROUGHUP_Test() to end!"
#define MODEM_THROUGHUP_164_112_2_17_23_46_4_4151 "tcpip test: modem up-throughput task is already existed!"
#define MODEM_THROUGHUP_187_112_2_17_23_46_4_4152 "tcpip test: modem throughup task ID : 0x%x "
#define MODEM_THROUGHUP_188_112_2_17_23_46_4_4153 "tcpip test: priority of modem throughup task: %d "
#define MODEM_THROUGHUP_245_112_2_17_23_46_5_4154 "tcpip test: ModemThroughupTaskEntry - GetModemId() failed!"
#define MODEM_THROUGHUP_275_112_2_17_23_46_5_4155 "tcpip test: ModemThroughupTaskEntry - CreateUdpPacket() failed!"
#define MODEM_THROUGHUP_295_112_2_17_23_46_5_4156 "tcpip test: MNGPRS_GetPclinkCfg() error!"
#define MODEM_THROUGHUP_303_112_2_17_23_46_5_4157 "tcpip test: ipsrc %s, ipdst %s, timegap %d"
#define MODEM_THROUGHUP_335_112_2_17_23_46_5_4158 "tcpip test: found modem netif 0x%x"
#define MODEM_THROUGHUP_348_112_2_17_23_46_5_4159 "tcpip test: modem_id of netif 0x%x is %d, his_asyncmap %x"
#define MODEM_THROUGHUP_370_112_2_17_23_46_5_4160 "tcpip test: MuxToPPP - invalid input!"
#define MODEM_THROUGHUP_374_112_2_17_23_46_5_4161 "tcpip test: %d bytes received with link_id %d"
#define MODEM_THROUGHUP_382_112_2_17_23_46_5_4162 "tcpip test: MuxToPPP - no pppif matched with link_id %d"
#define MODEM_THROUGHUP_414_112_2_17_23_46_5_4163 "tcpip test: CreateUdpPacket() - invalid input"
#define MODEM_THROUGHUP_434_112_2_17_23_46_5_4164 "tcpip test: CreateUdpPacket() - too small buffer"
#define MODEM_THROUGHUP_445_112_2_17_23_46_5_4165 "tcpip test: CreateUdpPacket() - act_ptr %x, act_max %d"
#define MODEM_THROUGHUP_452_112_2_17_23_46_5_4166 "tcpip test: CreateUdpPacket() - too small packet"
#define MODEM_THROUGHUP_474_112_2_17_23_46_5_4167 "tcpip test: CreateUdpPacket() - invalid type"
#define MODEM_THROUGHUP_610_112_2_17_23_46_6_4168 "tcpip test: CreatePPPPacket() - invalid input"
#define MODEM_THROUGHUP_677_112_2_17_23_46_6_4169 "tcpip test: ppp fcs not ok\n"
#define PCLINK_UTILITY_144_112_2_17_23_46_6_4170 "tcpip test: MNGPRS_GetPclinkCfg() error!"
#define CONFIG_FILE_76_112_2_17_23_46_11_4171 "%s(%d) : malloc memory failed!\n"
#define CONFIG_FILE_113_112_2_17_23_46_11_4172 "%s(%d) : malloc memory failed!\n"
#define ETHER_ADAPTER_117_112_2_17_23_46_12_4173 "tcpip ether: adapter name is too long!"
#define ETHER_ADAPTER_140_112_2_17_23_46_12_4174 "tcpip ether: no empty position in adapter list"
#define ETHER_ADAPTER_214_112_2_17_23_46_12_4175 "tcpip ether: set adapter to mode %d error!"
#define ETHER_ADAPTER_221_112_2_17_23_46_12_4176 "tcpip ether: set adapter buffer size %d error!"
#define ETHER_ADAPTER_228_112_2_17_23_46_12_4177 "tcpip ether: set adapter read time-out %d error!"
#define ETHER_ADAPTER_283_112_2_17_23_46_12_4178 "tcpip ether: no free adapter handle found"
#define ETHER_ADAPTER_401_112_2_17_23_46_13_4179 "tcpip ether: PacketSetNumWrites() failed!"
#define ETHER_ADAPTER_408_112_2_17_23_46_13_4180 "tcpip ether: PacketSendPacket() failed!"
#define ETHER_ADAPTER_497_112_2_17_23_46_13_4181 "tcpip ether: Open adapter \"%s\" error - 0x%x"
#define ETHER_ADAPTER_501_112_2_17_23_46_13_4182 "tcpip ether: Open adapter \"%s\" OK"
#define ETHER_ADAPTER_539_112_2_17_23_46_13_4183 "tcpip ether: retrieve list of adapter name failed!"
#define ETHER_ADAPTER_554_112_2_17_23_46_13_4184 "tcpip ether: adapter name list is full!"
#define ETHER_ADAPTER_564_112_2_17_23_46_13_4185 "tcpip ether: Current Ethernet Adapter Name List - num(%d)"
#define ETHER_ADAPTER_567_112_2_17_23_46_13_4186 " %d) - %s "
#define ETHER_PARAM_122_112_2_17_23_46_13_4187 "tcpip ether: IP string(%s) length is not long enough!"
#define ETHER_PARAM_131_112_2_17_23_46_13_4188 "tcpip ether: not IP char %c exist in given IP string!"
#define ETHER_PARAM_144_112_2_17_23_46_13_4189 "tcpip ether: there should be 3 '.' chars in IP string %s!"
#define ETHER_PARAM_172_112_2_17_23_46_13_4190 "tcpip ether: %s - each number should be 3 at the most!"
#define ETHER_PARAM_183_112_2_17_23_46_13_4191 "tcpip ether: %s - each number should be in 0~255!"
#define ETHER_PARAM_208_112_2_17_23_46_14_4192 "tcpip ether: %s - not enough MAC string length!"
#define ETHER_PARAM_218_112_2_17_23_46_14_4193 "tcpip ether: %s - none MAC char exist!"
#define ETHER_PARAM_231_112_2_17_23_46_14_4194 "tcpip ether: %s - there should be 5 '-' chars in MAC string!"
#define ETHER_PARAM_260_112_2_17_23_46_14_4195 "tcpip ether: %s - invalid MAC string!"
#define ETHER_PARAM_323_112_2_17_23_46_14_4196 "tcpip ether: Convert IP string to long failed!"
#define ETHER_PARAM_349_112_2_17_23_46_14_4197 "tcpip ether: Get IP String from file failed!"
#define ETHER_PARAM_376_112_2_17_23_46_14_4198 "tcpip ether: Get MAC string from file failed!"
#define ETHER_PARAM_402_112_2_17_23_46_14_4199 "tcpip ether: Get device description string from file failed!"
#define ETHER_PARAM_428_112_2_17_23_46_14_4200 "tcpip ether: Get adapter name string from file failed!"
#define ETHER_PARAM_494_112_2_17_23_46_14_4201 "tcpip ether: AddEtherNetif - interface list is full!"
#define ETHER_PARAM_502_112_2_17_23_46_14_4202 "tcpip ether: AddEtherNetif - open adapter '%s' failed!"
#define ETHER_PARAM_541_112_2_17_23_46_14_4203 "tcpip ether: load MAC in section \"%s\" failed!"
#define ETHER_PARAM_548_112_2_17_23_46_14_4204 "tcpip ether: load IP in section \"%s\" failed!"
#define ETHER_PARAM_555_112_2_17_23_46_14_4205 "tcpip ether: load subnet mask in section \"%s\" failed!"
#define ETHER_PARAM_562_112_2_17_23_46_14_4206 "tcpip ether: load Gateway in section \"%s\" failed!"
#define ETHER_PARAM_569_112_2_17_23_46_14_4207 "tcpip ether: load DNS in section \"%s\" failed!"
#define ETHER_PARAM_576_112_2_17_23_46_14_4208 "tcpip ether: load secondary DNS in section \"%s\" failed!"
#define ETHER_PARAM_582_112_2_17_23_46_14_4209 "tcpip ether: load DHCP in section \"%s\" failed!"
#define ETHER_PARAM_589_112_2_17_23_46_14_4210 "tcpip ether: load device desc in section \"%s\" failed!"
#define ETHER_PARAM_596_112_2_17_23_46_14_4211 "tcpip ether: load adapter name in section \"%s\" failed!"
#define ETHER_PARAM_601_112_2_17_23_46_14_4212 "\ntcpip ether: net parameters from section \"%s\""
#define ETHER_PARAM_603_112_2_17_23_46_14_4213 " < MAC . . . . . %02X:%02X:%02X:%02X:%02X:%02X >"
#define ETHER_PARAM_604_112_2_17_23_46_14_4214 " < IP  . . . . . %s > "
#define ETHER_PARAM_605_112_2_17_23_46_14_4215 " < Gateway . . . %s > "
#define ETHER_PARAM_606_112_2_17_23_46_14_4216 " < SubnetMask  . %s > "
#define ETHER_PARAM_607_112_2_17_23_46_14_4217 " < DNS Pri . . . %s > "
#define ETHER_PARAM_608_112_2_17_23_46_14_4218 " < DNS Sec . . . %s > "
#define ETHER_PARAM_609_112_2_17_23_46_14_4219 " < DHCP  . . . . %s > "
#define ETHER_PARAM_610_112_2_17_23_46_14_4220 " < DeviceDesc  . %s > "
#define ETHER_PARAM_611_112_2_17_23_46_14_4221 " < NetAdapter  . %s > "
#define ETHER_PARAM_643_112_2_17_23_46_15_4222 "tcpip ether: DhcpCallBack - DHCP OK on netid %x"
#define ETHER_PARAM_647_112_2_17_23_46_15_4223 "tcpip ether: DhcpCallBack - DHCP failed %d!"
#define ETHER_PARAM_681_112_2_17_23_46_15_4224 "tcpip ether: get DHCP result event on netid %x"
#define ETHER_PARAM_696_112_2_17_23_46_15_4225 "tcpip ether: DHCP event - DHCP OK on netid %x"
#define ETHER_PARAM_700_112_2_17_23_46_15_4226 "tcpip ether: DHCP event - DHCP failed %d on netid %x!"
#define ETHER_PARAM_715_112_2_17_23_46_15_4227 "tcpip ether: ptr->netid - unresolved EVENT %d "
#define ETHER_PARAM_746_112_2_17_23_46_15_4228 "tcpip ether: Get ethernet configure file failed!"
#define ETHER_PARAM_835_112_2_17_23_46_15_4229 "tcpip ether: DHCP enabled on netif %x!"
#define ETHER_PARAM_841_112_2_17_23_46_15_4230 "tcpip ether: set net interface for nsapi(%d, %d) failed!"
#define ETHER_PARAM_849_112_2_17_23_46_15_4231 "tcpip ether: register net interface failed!"
#define ETHER_PARAM_855_112_2_17_23_46_15_4232 "tcpip ether: DHCP enabled on netif %x!"
#define ETHER_PARAM_876_112_2_17_23_46_15_4233 "tcpip ether: net id list is full"
#define ETHER_PARAM_1065_112_2_17_23_46_15_4234 "tcpip ether: ETHERPARAM_StartDhcp - sci_async_dhcp() error %d"
#define ETHER_PARAM_1067_112_2_17_23_46_15_4235 "tcpip ether: ETHERPARAM_StartDhcp - sci_dhcp_request() error %d"
#define ETHER_PARAM_1096_112_2_17_23_46_15_4236 "tcpip ether: ETHERPARAM_GetFirstNetid() should be called first!"
#define ETHER_PARAM_1154_112_2_17_23_46_16_4237 "tcpip ether: get ethernet state file failed!"
#define ETHER_PARAM_1234_112_2_17_23_46_16_4238 "tcpip ether: Create task %s with ID : %d"
#define ETHER_PARAM_1236_112_2_17_23_46_16_4239 "tcpip ether: Priority of task %s is : %d"
#define ETHERNET_TEST_115_112_2_17_23_46_16_4240 "\n%s(%d) : WSAStartup() failed! "
#define ETHERNET_TEST_122_112_2_17_23_46_16_4241 "\n%s(%d) : Can't find WINSOCK dll! "
#define ETHERNET_TEST_130_112_2_17_23_46_16_4242 "\n%s(%d) : gethostname() failed - %d! "
#define ETHERNET_TEST_139_112_2_17_23_46_16_4243 "\n%s(%d) : gethostbyname() failed - %d! "
#define ETHERNET_TEST_155_112_2_17_23_46_16_4244 "\n%s(%d) : WSACleanup() failed! "
#define ETHERNET_TEST_175_112_2_17_23_46_16_4245 "\n%s(%d) : DNSTest - GetTestUrl() failed! "
#define ETHERNET_TEST_183_112_2_17_23_46_16_4246 "\n%s(%d) : DNSTest - sci_gethostbyname() failed! "
#define ETHERNET_TEST_192_112_2_17_23_46_16_4247 "\nDNSTest Result:\n <URL> - %s\n <IP> - %s\n"
#define ETHERNET_TEST_215_112_2_17_23_46_16_4248 "n%s(%d) : UDPTest - sci_sock_socket() error : %d!n"
#define ETHERNET_TEST_223_112_2_17_23_46_16_4249 "\nUDPTest Client IP - %s : %d  "
#define ETHERNET_TEST_230_112_2_17_23_46_16_4250 "%s(%d) : UDPTest - sci_sock_bind() error : %d!n"
#define ETHERNET_TEST_247_112_2_17_23_46_16_4251 "n%s(%d) : UDPTest - sci_sock_sendto() error : %d! "
#define ETHERNET_TEST_268_112_2_17_23_46_16_4252 "\n%s(%d) : UDPTest - select() time out! "
#define ETHERNET_TEST_274_112_2_17_23_46_16_4253 "n%s(%d) : UDPTest - select() error : %d! "
#define ETHERNET_TEST_288_112_2_17_23_46_17_4254 "n%s(%d) : UDPTest - sci_sock_recvfrom() error : %d! "
#define ETHERNET_TEST_294_112_2_17_23_46_17_4255 "n%s(%d) : UDPTest - socket connection is broken : %d! "
#define ETHERNET_TEST_310_112_2_17_23_46_17_4256 "n%s(%d) : UDPTest - Reply is not from server, go on waiting! "
#define ETHERNET_TEST_317_112_2_17_23_46_17_4257 "\n%s(%d) : UDPTest - client socket is not set in read field! "
#define ETHERNET_TEST_346_112_2_17_23_46_17_4258 "n%s(%d) : TCPTest - sci_sock_socket() error : %d! "
#define ETHERNET_TEST_360_112_2_17_23_46_17_4259 "n%s(%d) : TCPTest - sci_sock_bind() error : %d! "
#define ETHERNET_TEST_373_112_2_17_23_46_17_4260 "n%s(%d) : TCPTest - sci_sock_connect() error : %d! "
#define ETHERNET_TEST_383_112_2_17_23_46_17_4261 "n%s(%d) : TCPTest - sci_sock_send() error : %d! "
#define ETHERNET_TEST_404_112_2_17_23_46_17_4262 "\n%s(%d) : TCPTest - sci_sock_select() time out! "
#define ETHERNET_TEST_410_112_2_17_23_46_17_4263 "n%s(%d) : TCPTest - sci_sock_select() error : %d! "
#define ETHERNET_TEST_422_112_2_17_23_46_17_4264 "n%s(%d) : TCPTest - sci_sock_recvfrom() error : %d! "
#define ETHERNET_TEST_428_112_2_17_23_46_17_4265 "n%s(%d) : TCPTest - socket connection is broken : %d! "
#define ETHERNET_TEST_444_112_2_17_23_46_17_4266 "n%s(%d) : TCPTest - client socket is not set in read field! "
#define ETHERNET_TEST_464_112_2_17_23_46_17_4267 "\n<TCP/IP Ethernet Test Start>\n"
#define ETHERNET_TEST_505_112_2_17_23_46_17_4268 "%s(%d) : Create Ethernet Test Task with ID : %dn"
#define ETHERNET_TEST_507_112_2_17_23_46_17_4269 "%s(%d) : Priority of Ethernet Test Task is : %dn"
#define ETHERNET_TEST_524_112_2_17_23_46_17_4270 "%s(%d) : Close Ethernet Test Task with ID : %d\n"
#define FD_OPERATION_179_112_2_17_23_46_18_4271 "\ntcpip ether: Found \"%s\" in \"%s\"\n"
#define FD_OPERATION_359_112_2_17_23_46_18_4272 "tcpip ether: FILEDIR_GetConfigFilePath - _getcwd() error!\n"
#define FD_OPERATION_369_112_2_17_23_46_18_4273 "tcpip ether: searching \"%s\" in \"%s\"...\n"
#define FD_OPERATION_380_112_2_17_23_46_18_4274 "tcpip ether: configure file \"%s\" is not found!\n"
#define FD_OPERATION_403_112_2_17_23_46_18_4275 "tcpip ether: FILEDIR_GetStateFilePath - _getcwd() error!\n"
#define FD_OPERATION_437_112_2_17_23_46_18_4276 "tcpip ether: FILEDIR_GetSettingFilePath - _getcwd() error!\n"
#define MUX_SIMULATE_49_112_2_17_23_46_19_4277 "MUX_DTI_Create fail due to link id(%d) execed"
#define MUX_SIMULATE_65_112_2_17_23_46_19_4278 "MUX_DTI_Destroy fail due to link id(%d) execed"
#define MUX_SIMULATE_81_112_2_17_23_46_19_4279 "tcpip simulator: MUX_Write() %d data with link id %d"
#define TCPIP_ETHER_359_112_2_17_23_46_20_4280 "tcpip ether: Create task %s with ID : %d"
#define TCPIP_ETHER_361_112_2_17_23_46_20_4281 "tcpip ether: Priority of task %s is : %d"
#define TCPIP_ETHER_418_112_2_17_23_46_20_4282 "tcpip ether: EtherTaskSendEntry - pkt pointer is NULL!"
#define TCPIP_ETHER_440_112_2_17_23_46_20_4283 "tcpip ether: TX buffer alloc failed!"
#define TCPIP_ETHER_506_112_2_17_23_46_21_4284 "tcpip ether: Create task %s(%d) with ID : %d"
#define TCPIP_ETHER_508_112_2_17_23_46_21_4285 "tcpip ether: Priority of task %s is : %d"
#define TCPIP_ETHER_539_112_2_17_23_46_21_4286 "tcpip ether: bind recv task %x to ether adapter failed"
#define TCPIP_ETHER_547_112_2_17_23_46_21_4287 "tcpip ether: alloc recv buffer in task %x failed"
#define TCPIP_ETHER_558_112_2_17_23_46_21_4288 "tcpip ether: config adapter as buff(%d), rd_tmout(%d), type(%d) failed"
#define TCPIP_ETHER_571_112_2_17_23_46_21_4289 "tcpip ether: packet receive failed!"
#define MBOX_PING_400_112_2_17_23_47_44_4290 "finish timer expires\n"
#define MBOX_PING_456_112_2_17_23_47_44_4291 "netbuf is used up, sleep a while\n"
#define MBOX_PING_542_112_2_17_23_47_45_4292 "---- WMIPING Statistics ----\n"
#define MBOX_PING_543_112_2_17_23_47_45_4293 "Packets transmitted: %d\n"
#define MBOX_PING_544_112_2_17_23_47_45_4294 "Packets received: %d\n"
#define MBOX_PING_561_112_2_17_23_47_45_4295 "Throughput = %.2f pkts/sec, %.2f Mbpsn"
#define SPI_TESTING_262_112_2_17_23_47_46_4296 "spitest_DoPioExternalAccess %d\n"
#define SPI_TESTING_273_112_2_17_23_47_46_4297 "spitest_DoPioExternalAccess %d\n"
#define SPI_TESTING_276_112_2_17_23_47_46_4298 "spitest_DoPioExternalAccess %d\n"
#define RTW_WLAN_UTIL_1476_112_2_17_23_49_14_4299 "------->%s, raid:%d mask:%8.8xn"
#define RTL8192C_PHYCFG_1268_112_2_17_23_49_56_4300 "---->_rtl8192c_phy_set_bwmode():20M\n"
#define RTL8192C_PHYCFG_1272_112_2_17_23_49_56_4301 "---->_rtl8192c_phy_set_bwmode():40M\n"
#define RTL8192CS_DESC_54_112_2_17_23_50_6_4302 "---->HAL_PRIME_CHNL_OFFSET_LOWER\n"
#define RTL8192CS_DESC_57_112_2_17_23_50_6_4303 "---->HAL_PRIME_CHNL_OFFSET_UPPER\n"
#define RTL8192CS_DESC_60_112_2_17_23_50_6_4304 "---->HAL_PRIME_CHNL_OFFSET_DONT_CARE\n"
#define RTL8192CS_DESC_63_112_2_17_23_50_6_4305 "---->HAL_PRIME_CHNL_OFFSET_WRONG\n"
#define RTL8192CS_GEN_228_112_2_17_23_50_7_4306 "&&&&&&&&&&BSSID-i: %x\n"
#define RTL8192CS_GEN_285_112_2_17_23_50_7_4307 "HW_VAR_AMPDU_MIN_SPACE:%x\n"
#define RTL8192CS_GEN_335_112_2_17_23_50_8_4308 "HW_VAR_AMPDU_FACTOR:%x\n"
#define RTL8192CS_INIT_143_112_2_17_23_50_20_4309 "---->Set RF Chip ID to RF_6052 and RF type to 1T1R.\n"
#define RTL8192CS_INIT_148_112_2_17_23_50_20_4310 "rf_chip=0x%x, rf_type=0x%x\n"
#define RTL8192CS_INIT_515_112_2_17_23_50_20_4311 "leonard 200 %x 204 %x\n"
#define GSPI_IO_249_112_2_17_23_50_25_4312 "&&&&&leonard temp_data[0-4]: %x %x %x %xn"
#define THREADX_INTFS_269_112_2_17_23_50_28_4313 "rtw_xmit_entry: rtw_if_up fail\n"
#define THREADX_INTFS_362_112_2_17_23_50_28_4314 "thx_relay_xmit_entry: rtw_if_up fail\n"
#define SOCKETADDR_330_112_2_17_23_51_3_4315 "FATAL: get_in_addr called on IPv6 address\n"
#define GNU_GETOPT_763_112_2_17_23_51_9_4316 "digits occur in two different argv-elements.\n"
#define GNU_GETOPT_765_112_2_17_23_51_9_4317 "option %c\n"
#define GNU_GETOPT_769_112_2_17_23_51_9_4318 "option a\n"
#define GNU_GETOPT_773_112_2_17_23_51_9_4319 "option b\n"
#define GNU_GETOPT_777_112_2_17_23_51_9_4320 "option c with value `%s'\n"
#define GNU_GETOPT_784_112_2_17_23_51_9_4321 "?? gnu_getopt returned character code 0%o ??\n"
#define GNU_GETOPT_789_112_2_17_23_51_9_4322 "non-option ARGV-elements: "
#define GNU_GETOPT_791_112_2_17_23_51_9_4323 "%s "
#define GNU_GETOPT_792_112_2_17_23_51_9_4324 "\n"
#define GNU_GETOPT_LONG_103_112_2_17_23_51_9_4325 "option %s"
#define GNU_GETOPT_LONG_105_112_2_17_23_51_9_4326 " with arg %s"
#define GNU_GETOPT_LONG_106_112_2_17_23_51_9_4327 "\n"
#define GNU_GETOPT_LONG_120_112_2_17_23_51_10_4328 "digits occur in two different argv-elements.\n"
#define GNU_GETOPT_LONG_122_112_2_17_23_51_10_4329 "option %c\n"
#define GNU_GETOPT_LONG_126_112_2_17_23_51_10_4330 "option a\n"
#define GNU_GETOPT_LONG_130_112_2_17_23_51_10_4331 "option b\n"
#define GNU_GETOPT_LONG_134_112_2_17_23_51_10_4332 "option c with value `%s'\n"
#define GNU_GETOPT_LONG_138_112_2_17_23_51_10_4333 "option d with value `%s'\n"
#define GNU_GETOPT_LONG_145_112_2_17_23_51_10_4334 "?? gnu_getopt returned character code 0%o ??\n"
#define GNU_GETOPT_LONG_150_112_2_17_23_51_10_4335 "non-option ARGV-elements: "
#define GNU_GETOPT_LONG_152_112_2_17_23_51_10_4336 "%s "
#define GNU_GETOPT_LONG_153_112_2_17_23_51_10_4337 "\n"
#define WSP_API_68_112_2_17_23_51_16_4338 "\n wsp WSP_InitRequest task id = %d"
#define WSP_API_80_112_2_17_23_51_16_4339 "\n wsp WSP_InitRequest error_code = %d"
#define WSP_API_129_112_2_17_23_51_16_4340 "\n wsp WSP_GetRequest error_code = %d"
#define WSP_API_178_112_2_17_23_51_16_4341 "\n wsp WSP_PostRequest error_code = %d"
#define WSP_API_229_112_2_17_23_51_16_4342 "\n wsp WSP_AuthResponse error_code = %d"
#define WSP_API_274_112_2_17_23_51_16_4343 "\n wsp WSP_CloseRequest error_code = %d"
#define WSP_API_322_112_2_17_23_51_16_4344 "\n wsp WSP_CancelRequest error_code = %d"
#define WSP_API_411_112_2_17_23_51_16_4345 "\n http HTTP_CookieClear error_code = %d"
#define WSP_API_442_112_2_17_23_51_17_4346 "\n wsp WSP_CacheClear 1 error_code = %d"
#define WSP_API_459_112_2_17_23_51_17_4347 "\n wsp WSP_CacheClear 2 error_code = %d"
#define WSP_API_478_112_2_17_23_51_17_4348 "\n http HTTP_CacheClear 3 error_code = %d"
#define WSP_API_492_112_2_17_23_51_17_4349 "\n wsp WSP_CacheClear 4 error_code = %d"
#define WSP_API_544_112_2_17_23_51_17_4350 "\n wsp WSP_WtlsCertResponse error_code = %d"
#define WSP_AUTH_150_112_2_17_23_51_17_4351 "\n wsp WspAuthenDigestEncode input error \n"
#define WSP_AUTH_156_112_2_17_23_51_17_4352 "\n wsp WspAuthenDigestEncode input error2 \n"
#define WSP_AUTH_164_112_2_17_23_51_17_4353 "\n wsp WspAuthenDigestEncode alloc error \n"
#define WSP_AUTH_172_112_2_17_23_51_17_4354 "\n wsp WspAuthenDigestEncode AuthenDigestRequsetSetParam error \n"
#define WSP_AUTH_219_112_2_17_23_51_17_4355 "\n wsp SetAuthenDigestRequsetParam input error \n"
#define WSP_AUTH_393_112_2_17_23_51_18_4356 "\n wsp SetAuthenDigestRequsetParam error_code =%d \n"
#define WSP_AUTH_454_112_2_17_23_51_18_4357 "\n wsp WSP_AuthenDigestResponseParse input error \n"
#define WSP_AUTH_461_112_2_17_23_51_18_4358 "\n wsp WSP_AuthenDigestResponseParse alloc error \n"
#define WSP_AUTH_488_112_2_17_23_51_18_4359 "\n wsp WSP_AuthenDigestResponseParse 1 error_code =%d"
#define WSP_AUTH_505_112_2_17_23_51_18_4360 "\n wsp WSP_AuthenDigestResponseParse error net data \n"
#define WSP_AUTH_514_112_2_17_23_51_18_4361 "\n wsp WSP_AuthenDigestResponseParse 2 error_code=%d"
#define WSP_AUTH_548_112_2_17_23_51_18_4362 "\n wsp AuthenDigestResponseSaveParam invalid input \n"
#define WSP_AUTH_741_112_2_17_23_51_18_4363 "\n wsp AuthenDigestResponseSaveParam unknown param \n"
#define WSP_AUTH_748_112_2_17_23_51_18_4364 "\n wsp AuthenDigestResponseSaveParam error_code=%d"
#define WSP_AUTH_767_112_2_17_23_51_18_4365 "\n wsp AuthenDigestResponseSaveParam CLEAR error_code=%d"
#define WSP_AUTH_783_112_2_17_23_51_18_4366 "\n wsp AuthenDigestTokenFind input error \n"
#define WSP_AUTH_796_112_2_17_23_51_18_4367 "\n wsp AuthenDigestTokenFind auth map type =%d"
#define WSP_AUTH_800_112_2_17_23_51_18_4368 "\n wsp AuthenDigestTokenFind error \n"
#define WSP_AUTH_819_112_2_17_23_51_18_4369 "\n wsp WSP_AuthenGetInfo input error \n"
#define WSP_AUTH_860_112_2_17_23_51_18_4370 "\n wsp WSP_AuthenGetInfo unknown scheme\n"
#define WSP_AUTH_893_112_2_17_23_51_19_4371 "\n wsp CombineAuthenEncodedResult input error \n"
#define WSP_AUTH_904_112_2_17_23_51_19_4372 "\n wsp CombineAuthenEncodedResult alloc error \n"
#define WSP_AUTH_919_112_2_17_23_51_19_4373 "\n wsp CombineAuthenEncodedResult 1 input error \n"
#define WSP_AUTH_925_112_2_17_23_51_19_4374 "\n wsp CombineAuthenEncodedResult 1 alloc error \n"
#define WSP_CACHE_118_112_2_17_23_51_19_4375 "\n wsp WSP_HandleCacheDataToApp 1 input error \n"
#define WSP_CACHE_125_112_2_17_23_51_19_4376 "\n wsp WSP_HandleCacheDataToApp 2 input error \n"
#define WSP_CACHE_142_112_2_17_23_51_19_4377 "\n wsp WSP_HandleCacheDataToApp file not found error \n"
#define WSP_CACHE_150_112_2_17_23_51_19_4378 "\n wsp WSP_HandleCacheDataToApp get file size error \n"
#define WSP_CACHE_168_112_2_17_23_51_19_4379 "\n wsp WSP_HandleCacheDataToApp alloc error \n"
#define WSP_CACHE_178_112_2_17_23_51_19_4380 "\n wsp WSP_HandleCacheDataToApp file read error \n"
#define WSP_CACHE_195_112_2_17_23_51_19_4381 "\n wsp WSP_HandleCacheDataToApp send HTTP_SIG_GET_CNF"
#define WSP_CACHE_202_112_2_17_23_51_19_4382 "\n wsp WSP_HandleCacheDataToApp error method\n"
#define WSP_EVENT_38_112_2_17_23_51_19_4383 "\n wsp WSP_EventQueueInit input error\n"
#define WSP_EVENT_56_112_2_17_23_51_19_4384 "\n wsp WSP_EventQueueInsert input error\n"
#define WSP_EVENT_64_112_2_17_23_51_19_4385 "\n wsp WSP_EventQueueInsert alloc error\n"
#define WSP_EVENT_99_112_2_17_23_51_20_4386 "\n wsp WSP_EventQueueFetch input error\n"
#define WSP_EVENT_146_112_2_17_23_51_20_4387 "\n wsp WSP_EventQueueClear input error\n"
#define WSP_EVENT_175_112_2_17_23_51_20_4388 "\n wsp WSP_EventDestroy input error\n"
#define WSP_EVENT_237_112_2_17_23_51_20_4389 "WSP:WSP_EventDestroy.sig code error = %d!"
#define WSP_HEADER_42_112_2_17_23_51_20_4390 "\n wsp WSP_SetAcceptHeader input error\n"
#define WSP_HEADER_49_112_2_17_23_51_20_4391 "\n wsp WSP_SetAcceptHeader alloc error\n"
#define WSP_HEADER_78_112_2_17_23_51_20_4392 "\n wsp WSP_SetAcceptCharsetHeader input error\n"
#define WSP_HEADER_85_112_2_17_23_51_20_4393 "\n wsp WSP_SetAcceptCharsetHeader alloc error\n"
#define WSP_HEADER_114_112_2_17_23_51_20_4394 "\n wsp WSP_SetAcceptEncodingHeader input error\n"
#define WSP_HEADER_121_112_2_17_23_51_20_4395 "\n wsp WSP_SetAcceptEncodingHeader alloc error\n"
#define WSP_HEADER_150_112_2_17_23_51_20_4396 "\n wsp WSP_SetAcceptLanguageHeader input error\n"
#define WSP_HEADER_157_112_2_17_23_51_20_4397 "\n wsp WSP_SetAcceptLanguageHeader alloc error\n"
#define WSP_HEADER_186_112_2_17_23_51_20_4398 "\n wsp WSP_SetRefererHeader input error\n"
#define WSP_HEADER_193_112_2_17_23_51_20_4399 "\n wsp WSP_SetRefererHeader alloc error\n"
#define WSP_HEADER_222_112_2_17_23_51_20_4400 "\n wsp WSP_SetUserAgentHeader input error\n"
#define WSP_HEADER_229_112_2_17_23_51_20_4401 "\n wsp WSP_SetUserAgentHeader alloc error\n"
#define WSP_HEADER_258_112_2_17_23_51_21_4402 "\n wsp WSP_SetContentTypeHeader input error\n"
#define WSP_HEADER_265_112_2_17_23_51_21_4403 "\n wsp WSP_SetContentTypeHeader alloc error\n"
#define WSP_HEADER_294_112_2_17_23_51_21_4404 "\n wsp WSP_SetUAProfileHeader input error\n"
#define WSP_HEADER_301_112_2_17_23_51_21_4405 "\n wsp WSP_SetUAProfileHeader alloc error\n"
#define WSP_HEADER_332_112_2_17_23_51_21_4406 "\n wsp WSP_SetHeaderField input error\n"
#define WSP_HEADER_339_112_2_17_23_51_21_4407 "\n wsp WSP_SetHeaderField alloc error\n"
#define WSP_HSM_374_112_2_17_23_51_22_4408 "\n wsp WSPHSM_Create host_handle =%x\n"
#define WSP_HSM_383_112_2_17_23_51_22_4409 "\n wsp WSPHSM_Create  alloc  wsp_hsm_ptr error\n"
#define WSP_HSM_394_112_2_17_23_51_22_4410 "\n wsp WSPHSM_Create  alloc  event_queue_ptr error\n"
#define WSP_HSM_405_112_2_17_23_51_22_4411 "\n wsp WSPHSM_Create session = %d\n"
#define WSP_HSM_412_112_2_17_23_51_22_4412 "\n wsp WSPHSM_Create wsp_hsm_ptr = %d"
#define WSP_HSM_453_112_2_17_23_51_22_4413 "\n wsp HandleHsmDestroy hsm_ptr = %d"
#define WSP_HSM_487_112_2_17_23_51_22_4414 "\n wsp WSPHSM_DispatchSignal input error\n"
#define WSP_HSM_490_112_2_17_23_51_22_4415 "\n wsp WSPHSM_DispatchSignal hsm_ptr=%d,signal_code=%d\n"
#define WSP_HSM_540_112_2_17_23_51_22_4416 "\n wsp WSPHSM_DealRequest input error\n"
#define WSP_HSM_546_112_2_17_23_51_22_4417 "\n wsp WSPHSM_DealRequest request_ptr=%d\n"
#define WSP_HSM_550_112_2_17_23_51_22_4418 "\n wsp WSPHSM_DealRequest hsm_ptr input error\n"
#define WSP_HSM_624_112_2_17_23_51_22_4419 "\n wsp QHSM_Wsp input error\n"
#define WSP_HSM_635_112_2_17_23_51_22_4420 "\n wsp QHSM_Wsp receive wtp destroy hsm ind! \n"
#define WSP_HSM_644_112_2_17_23_51_22_4421 "\n wsp QHSM_Wsp receive wsp destroy hsm signal! \n"
#define WSP_HSM_650_112_2_17_23_51_22_4422 "\n wsp QHSM_Wsp receive wtp send prog ind signal! \n"
#define WSP_HSM_665_112_2_17_23_51_23_4423 "\n wsp QHSM_Wsp receive wsp error signal! \n"
#define WSP_HSM_693_112_2_17_23_51_23_4424 "\n wsp Wsp_Idle input error! \n"
#define WSP_HSM_700_112_2_17_23_51_23_4425 "\n wsp Wsp_Idle receive app req signal! \n"
#define WSP_HSM_710_112_2_17_23_51_23_4426 "\n wsp Wsp_Idle receive connect proxy signal! \n"
#define WSP_HSM_724_112_2_17_23_51_23_4427 "\n wsp Wsp_Idle receive handshake req signal! \n"
#define WSP_HSM_756_112_2_17_23_51_23_4428 "\n wsp Wsp_Connecting input error! \n"
#define WSP_HSM_764_112_2_17_23_51_23_4429 "\n wsp Wsp_Connecting receive seg result signal! \n"
#define WSP_HSM_773_112_2_17_23_51_23_4430 "\n wsp Wsp_Connecting receive last result signal! \n"
#define WSP_HSM_783_112_2_17_23_51_23_4431 "\n wsp Wsp_Connecting receive wtp abort ind signal! \n"
#define WSP_HSM_789_112_2_17_23_51_23_4432 "\n wsp Wsp_Connecting receive cancel req signal! \n"
#define WSP_HSM_823_112_2_17_23_51_23_4433 "\n wsp Wsp_AuthRspPending input error! \n"
#define WSP_HSM_831_112_2_17_23_51_23_4434 "\n wsp Wsp_AuthRspPending receive cancel req signal! \n"
#define WSP_HSM_837_112_2_17_23_51_23_4435 "\n wsp Wsp_AuthRspPending receive app auth rsp signal! \n"
#define WSP_HSM_873_112_2_17_23_51_23_4436 "\n wsp Wsp_Handshaking input error! \n"
#define WSP_HSM_884_112_2_17_23_51_23_4437 "\n wsp delete handshake timer = %d"
#define WSP_HSM_891_112_2_17_23_51_23_4438 "\n wsp create handshake timer = %d"
#define WSP_HSM_898_112_2_17_23_51_23_4439 "\n wsp delete handshake timer = %d"
#define WSP_HSM_906_112_2_17_23_51_23_4440 "\n wsp delete handshake timer = %d"
#define WSP_HSM_912_112_2_17_23_51_23_4441 "\n wsp Wsp_Handshaking receive wtls handshake succ signal! \n"
#define WSP_HSM_916_112_2_17_23_51_23_4442 "\n wsp delete handshake timer = %d"
#define WSP_HSM_931_112_2_17_23_51_23_4443 "\n wsp Wsp_Idle receive connect proxy signal! \n"
#define WSP_HSM_948_112_2_17_23_51_23_4444 "\n wsp delete handshake timer = %d"
#define WSP_HSM_953_112_2_17_23_51_23_4445 "\n wsp Wsp_Handshaking receive wtls cert ind signal \n"
#define WSP_HSM_962_112_2_17_23_51_23_4446 "\n wsp delete handshake timer = %d"
#define WSP_HSM_969_112_2_17_23_51_23_4447 "\n wsp create handshake timer = %d"
#define WSP_HSM_972_112_2_17_23_51_23_4448 "\n wsp Wsp_Handshaking receive app untrust rsp signal! \n"
#define WSP_HSM_981_112_2_17_23_51_23_4449 "\n wsp delete handshake timer = %d"
#define WSP_HSM_984_112_2_17_23_51_23_4450 "\n wsp Wsp_Handshaking receive cancel req signal! \n"
#define WSP_HSM_1009_112_2_17_23_51_23_4451 "\n wsp Wsp_Connected input error! \n"
#define WSP_HSM_1016_112_2_17_23_51_23_4452 "\n wsp Wsp_Connected receive wtp invoke ind signal! \n"
#define WSP_HSM_1021_112_2_17_23_51_23_4453 "\n wsp Wsp_Connected receive cancel req signal! \n"
#define WSP_HSM_1027_112_2_17_23_51_23_4454 "\n wsp Wsp_Connected receive method invoke req signal! \n"
#define WSP_HSM_1060_112_2_17_23_51_23_4455 "\n wsp Wsp_Waiting input error! \n"
#define WSP_HSM_1068_112_2_17_23_51_23_4456 "\n wsp Wsp_Waiting receive seg result ind signal! \n"
#define WSP_HSM_1077_112_2_17_23_51_23_4457 "\n wsp Wsp_Waiting receive last result ind signal! \n"
#define WSP_HSM_1086_112_2_17_23_51_23_4458 "\n wsp Wsp_Waiting receive wtp abort ind signal! \n"
#define WSP_HSM_1095_112_2_17_23_51_23_4459 "\n wsp Wsp_Waiting receive cancel req signal! \n"
#define WSP_HSM_1130_112_2_17_23_51_23_4460 "\n wsp Wsp_Completing input error! \n"
#define WSP_HSM_1137_112_2_17_23_51_23_4461 "\n wsp Wsp_Completing entry ! \n"
#define WSP_HSM_1154_112_2_17_23_51_24_4462 "\n wsp Wsp_Completing destroy wsp hsm ! \n"
#define WSP_HSM_1161_112_2_17_23_51_24_4463 "\n wsp Wsp_Completing receive wsp  resuest fin ind signal! \n"
#define WSP_HSM_1190_112_2_17_23_51_24_4464 "\n wsp HandleCompletingRequestFinInd input error! \n"
#define WSP_HSM_1208_112_2_17_23_51_24_4465 "\n wsp HandleCompletingRequestFinInd disconnect wsp ! \n"
#define WSP_HSM_1216_112_2_17_23_51_24_4466 "\n wsp HandleCompletingRequestFinInd:re-handle current request! \n"
#define WSP_HSM_1223_112_2_17_23_51_24_4467 "\n wsp HandleCompletingRequestFinInd:inform net mgr redirect! \n"
#define WSP_HSM_1228_112_2_17_23_51_24_4468 "\n wsp HandleCompletingRequestFinInd redirect error \n"
#define WSP_HSM_1235_112_2_17_23_51_24_4469 "\n wsp HandleCompletingRequestFinInd:inform net mgr deal next request! \n"
#define WSP_HSM_1259_112_2_17_23_51_24_4470 "\n wsp HandleWtpDestroyedInd input error! \n"
#define WSP_HSM_1274_112_2_17_23_51_24_4471 "\n wsp HandleWtpDestroyedInd: destroy active wtp hsm! \n"
#define WSP_HSM_1281_112_2_17_23_51_24_4472 "\n wsp HandleWtpDestroyedInd: destroy connect wtp hsm! \n"
#define WSP_HSM_1287_112_2_17_23_51_24_4473 "\n wsp HandleWtpDestroyedInd: destroy method wtp hsm! \n"
#define WSP_HSM_1307_112_2_17_23_51_24_4474 "\n wsp HandleWtpDestroyedInd: to destroy wsp hsm! \n"
#define WSP_HSM_1328_112_2_17_23_51_24_4475 "\n wsp HandleWspError input error! \n"
#define WSP_HSM_1340_112_2_17_23_51_24_4476 "\n wsp HandleWspError abort transaction error! \n"
#define WSP_HSM_1364_112_2_17_23_51_24_4477 "\n wsp HandleAuthPendingAuthRsp input error! \n"
#define WSP_HSM_1495_112_2_17_23_51_24_4478 "\n wsp WSP_DealSignal recv WSP_SIG_HANDSHAKE_TIMEOUT_IND \n"
#define WSP_HSM_1499_112_2_17_23_51_24_4479 "\n WSP_DealSignal unknown signal\n"
#define WSP_HSM_1537_112_2_17_23_51_24_4480 "\n wsp HandleRTimeOutInd alloc error! \n"
#define WSP_HSM_1563_112_2_17_23_51_24_4481 "\n wsp HandleRTimeOutInd input error! \n"
#define WSP_HSM_1572_112_2_17_23_51_24_4482 "\n wsp HandleRTimeOutInd: app param error! \n"
#define WSP_HSM_1581_112_2_17_23_51_24_4483 "\n wsp HandleRTimeOutInd get request fail! \n"
#define WSP_HSM_1588_112_2_17_23_51_24_4484 "\n wsp HandleRTimeOutInd no active wtp hsm! \n"
#define WSP_HSM_1596_112_2_17_23_51_24_4485 "\n wsp HandleRTimeOutInd alloc error! \n"
#define WSP_HSM_1602_112_2_17_23_51_24_4486 "\n wsp HandleRTimeOutInd:inform wtp R timeout, handle =%x! \n"
#define WSP_HSM_1626_112_2_17_23_51_24_4487 "\n wsp HandleConnectedMethodInvokeReq input error! \n"
#define WSP_HSM_1645_112_2_17_23_51_24_4488 "\n wsp HandleConnectedMethodInvokeReq format request header error! \n"
#define WSP_HSM_1704_112_2_17_23_51_25_4489 "\n wsp HandleConnectedMethodInvokeReq send invoke error! \n"
#define WSP_HSM_1786_112_2_17_23_51_25_4490 "\n wsp HandleIdleAppReq data from cache success\n"
#define WSP_HSM_2043_112_2_17_23_51_25_4491 "WSP AbortWspRequest: input invalid!"
#define WSP_HSM_2051_112_2_17_23_51_25_4492 "WSP AbortWspRequest: memory alloc error!"
#define WSP_HSM_2055_112_2_17_23_51_25_4493 "wsp AbortWspRequest, abort reason = %d"
#define WSP_HSM_2337_112_2_17_23_51_26_4494 "WSP HandleIdleHandshake: input error fail!"
#define WSP_HSM_2348_112_2_17_23_51_26_4495 "WSP HandleIdleHandshake: port = %d!"
#define WSP_HSM_2353_112_2_17_23_51_26_4496 "WSP HandleIdleHandshake: create fail!"
#define WSP_HSM_2364_112_2_17_23_51_26_4497 "WSP HandleIdleHandshake: host = %s!"
#define WSP_HSM_2370_112_2_17_23_51_26_4498 "WSP HandleIdleHandshake: inet fail!"
#define WSP_HSM_2377_112_2_17_23_51_26_4499 "WSP HandleIdleHandshake: handshake fail!"
#define WSP_HSM_2417_112_2_17_23_51_26_4500 "\n wsp WSP_SecurityPostMessage recv WTLS_POSTMESSAGE_HANDSHAKE_SUCC\n"
#define WSP_HSM_2422_112_2_17_23_51_26_4501 "\n wsp WSP_SecurityPostMessage recv WTLS_POSTMESSAGE_CERT_NEED_CONFIRM\n"
#define WSP_HSM_2429_112_2_17_23_51_26_4502 "\n wsp WSP_SecurityPostMessage recv WTLS_POSTMESSAGE_FAIL\n"
#define WSP_HSM_2434_112_2_17_23_51_26_4503 "\n wsp WSP_SecurityPostMessage recv error\n"
#define WSP_HSM_2494_112_2_17_23_51_26_4504 "\n wsp HandleWaitingAbortInd: abort reason =%d! \n"
#define WSP_HSM_2581_112_2_17_23_51_26_4505 "WSP IsWspHeaderAllReceived: recv all header!"
#define WSP_HSM_2610_112_2_17_23_51_26_4506 "\n wsp HandleWaitingResultInd input error! \n"
#define WSP_HSM_2623_112_2_17_23_51_26_4507 "WSP:HandleWaitingResultInd, active tid =%d, rsp tid = %d, data_ptr = %d, data len = %d"
#define WSP_HSM_2686_112_2_17_23_51_27_4508 "WSP:HandleWaitingResultInd data error!"
#define WSP_HSM_2774_112_2_17_23_51_27_4509 "\n wsp HandleWaitingResultInd occurs error = %d! \n"
#define WSP_HSM_2887_112_2_17_23_51_27_4510 "WSP HandleWspContent, rsp code =%d"
#define WSP_HSM_3108_112_2_17_23_51_27_4511 "\n WSP HandleDataFromCache unknown method \n"
#define WSP_HSM_3235_112_2_17_23_51_28_4512 "WSP HandleCookieClearReq context_id =%d"
#define WSP_HSM_3245_112_2_17_23_51_28_4513 "WSP HandleCookieClearReq HttpCookieFinalize 1 error_code = %d"
#define WSP_HSM_3252_112_2_17_23_51_28_4514 "WSP HandleCookieClearReq HttpCookieFinalize 2 error_code = %d"
#define WSP_HSM_3257_112_2_17_23_51_28_4515 "WSP HandleCookieClearReq HttpCookieFinalize 3 error_code = %d"
#define WSP_HSM_3263_112_2_17_23_51_28_4516 "WSP HandleCookieClearReq HttpCookieFinalize 4 error_code = %d"
#define WSP_HSM_3639_112_2_17_23_51_28_4517 "\n WSP_SigErrCnf unknown signal \n"
#define WSP_HSM_3769_112_2_17_23_51_29_4518 "\n wsp HandleTransSendResult: error_code = %d!\n"
#define WSP_HSM_3774_112_2_17_23_51_29_4519 "\n wsp HandleTransSendResult: send wsp session disconnect succ!\n"
#define WSP_HSM_3790_112_2_17_23_51_29_4520 "\n wsp HandleTransSendResult: inform wsp send disconnect pdu error!\n"
#define WSP_HSM_3813_112_2_17_23_51_29_4521 "\n wsp HandleTransSendResult: inform wtp send succ!\n"
#define WSP_HSM_3818_112_2_17_23_51_29_4522 "\n wsp HandleTransSendResult: inform wtp send fail!\n"
#define WSP_HSM_3856_112_2_17_23_51_29_4523 "WSP: HandleTransRecvSucc, data len = %d"
#define WSP_HSM_3859_112_2_17_23_51_29_4524 "WSP: HandleTransRecvSucc, data len <=0, need not handle!"
#define WSP_HSM_3909_112_2_17_23_51_29_4525 "WSP: HandleTransRecvSucc, fragment data len = %d"
#define WSP_HSM_3952_112_2_17_23_51_29_4526 "WSP, HandleTransRecvSucc, unpack error!"
#define WSP_HSM_3958_112_2_17_23_51_29_4527 "WSP, HandleTransRecvSucc,no active wtp ptr!"
#define WSP_HSM_4006_112_2_17_23_51_29_4528 "wsp NotifyWspError invalid input!"
#define WSP_HSM_4019_112_2_17_23_51_29_4529 "wsp NotifyWspError!"
#define WSP_HSM_4052_112_2_17_23_51_29_4530 "\n wsp TransCallback NETMGR_DATA_IN_IND success\n"
#define WSP_HSM_4056_112_2_17_23_51_29_4531 "\n wsp TransCallback NETTRANS_Recv error\n"
#define WSP_HSM_4062_112_2_17_23_51_29_4532 "\n wsp TransCallback NETMGR_DATA_IN_IND error\n"
#define WSP_HSM_4071_112_2_17_23_51_29_4533 "\n wsp TransCallback NETTRANS_SEND_RETURN_IND error\n"
#define WSP_HSM_4079_112_2_17_23_51_29_4534 "\n wsp TransCallback NETTRANS_RECV_RETURN_IND success\n"
#define WSP_HSM_4083_112_2_17_23_51_29_4535 "\n wsp TransCallback HandleTransRecvSucc error\n"
#define WSP_HSM_4090_112_2_17_23_51_29_4536 "\n wsp TransCallback NETTRANS_RECV_RETURN_IND fail\n"
#define WSP_HSM_4101_112_2_17_23_51_29_4537 "\n wsp TransCallback NETMGR_RECV_RETURN_IND error\n"
#define WSP_HSM_4107_112_2_17_23_51_29_4538 "\n wsp TransCallback default error\n"
#define WSP_HSM_4125_112_2_17_23_51_29_4539 "\n wsp HandleWspHsmFatalError, input error!\n"
#define WSP_HSM_4129_112_2_17_23_51_29_4540 "\n wsp HandleWspHsmFatalError, error = %d \n"
#define WSP_HSM_4138_112_2_17_23_51_29_4541 "\n wsp HandleWspHsmFatalError abort transaction error = %d! \n"
#define WSP_REQUEST_150_112_2_17_23_51_30_4542 "\n WSP WSP_CreateRequest: header list handle = %d\n"
#define WSP_REQUEST_187_112_2_17_23_51_31_4543 "\n WSP WSP_CreateRequest unknown request\n"
#define WSP_REQUEST_195_112_2_17_23_51_31_4544 "\n WSP WSP_CreateRequest: request list handle =%d\n"
#define WSP_REQUEST_201_112_2_17_23_51_31_4545 "\n WSP WSP_CreateRequest request id =%d\n"
#define WSP_REQUEST_279_112_2_17_23_51_31_4546 "\n WSP WSP_DestroyRequest: destroy header list handle = %d\n"
#define WSP_REQUEST_297_112_2_17_23_51_31_4547 "\n WSP WSP_DestroyRequest: free post body copyed by wsp\n"
#define WSP_REQUEST_318_112_2_17_23_51_31_4548 "\n WSP WSP_DestroyRequest: destroy request list handle = %d\n"
#define WSP_REQUEST_563_112_2_17_23_51_31_4549 "\n WSP CreatePostRequest: wsp body is file\n"
#define WSP_REQUEST_586_112_2_17_23_51_32_4550 "\n WSP CreatePostRequest: wsp copy body buffer\n"
#define WSP_REQUEST_591_112_2_17_23_51_32_4551 "\n WSP CreatePostRequest post buffer not copy \n"
#define WSP_REQUEST_596_112_2_17_23_51_32_4552 "\n WSP CreatePostRequest post buffer null \n"
#define WSP_REQUEST_1099_112_2_17_23_51_33_4553 "\n WSP_RequestFormat, need to proxyAuthenticate\n"
#define WSP_REQUEST_1152_112_2_17_23_51_33_4554 "\n WSP_RequestFormat, need not to proxyAuthenticate\n"
#define WSP_REQUEST_1201_112_2_17_23_51_33_4555 "\n WSP_RequestFormat, need to wwwAuthenticate\n"
#define WSP_REQUEST_1249_112_2_17_23_51_33_4556 "\n WSP_RequestFormat, need not to wwwAuthenticate\n"
#define WSP_REQUEST_1499_112_2_17_23_51_33_4557 "\n wsp WSP_UpdateRequest input error\n"
#define WSP_REQUEST_1507_112_2_17_23_51_33_4558 "\n wsp WSP_UpdateRequest \n"
#define WSP_RESPONSE_718_112_2_17_23_51_35_4559 "\n WSP ParseResponseCacheHeader unknown param \n"
#define WSP_RESPONSE_1138_112_2_17_23_51_36_4560 "\n WSP WSP_ParseResponseLocationHeader request_ptr->uri_str_ptr = %s"
#define WSP_SESSION_174_112_2_17_23_51_43_4561 "\n WSP WSP_DestroySession enter!\n"
#define WSP_SIGNAL_85_112_2_17_23_51_43_4562 "\n WSP WSP_SendNetIndToApp send WSP_SIG_NET_PROG_IND!"
#define WSP_SIGNAL_123_112_2_17_23_51_43_4563 "\n WSP WSP_SendDataIndToApp send WSP_SIG_DATA_IND!"
#define WSP_SIGNAL_161_112_2_17_23_51_43_4564 "\n WSP WSP_SendHeaderIndToApp send WSP_SIG_HEADER_IND!"
#define WSP_SIGNAL_196_112_2_17_23_51_43_4565 "\n WSP WSP_SendErrorIndToApp send WSP_SIG_ERROR_IND!"
#define WSP_SIGNAL_231_112_2_17_23_51_44_4566 "\n WSP WSP_SendCancelCnfToApp send WSP_SIG_CANCEL_CNF!"
#define WSP_SIGNAL_274_112_2_17_23_51_44_4567 "\n WSP WSP_SendGetCnfToApp send WSP_SIG_GET_CNF!"
#define WSP_SIGNAL_318_112_2_17_23_51_44_4568 "\n WSP WSP_SendPostCnfToApp send WSP_SIG_POST_CNF!"
#define WSP_SIGNAL_354_112_2_17_23_51_44_4569 "\n WSP WSP_SendNeedAuthToApp send WSP_SIG_NEED_AUTH_IND!"
#define WSP_SIGNAL_390_112_2_17_23_51_44_4570 "\n WSP WSP_SendInitCnfToApp send WSP_SIG_INIT_CNF!"
#define WSP_SIGNAL_421_112_2_17_23_51_44_4571 "\n WSP WSP_SendCloseCnfToApp send WSP_SIG_CLOSE_CNF!"
#define WSP_SIGNAL_452_112_2_17_23_51_44_4572 "\n WSP WSP_SendCookieClearCnfToApp send WSP_SIG_COOKIE_CLEAR_CNF!"
#define WSP_SIGNAL_484_112_2_17_23_51_44_4573 "\n WSP WSP_SendRequestIndToApp send WSP_SIG_REQUEST_ID_IND!"
#define WSP_SIGNAL_520_112_2_17_23_51_44_4574 "\n WSP WSP_SendRedirectIndToApp send WSP_SIG_REDIRECT_IND"
#define WSP_SIGNAL_554_112_2_17_23_51_44_4575 "\n WSP WSP_SendWtlsUntrustIndToApp send WSP_SIG_WTLS_UNTRUST_IND!"
#define WSP_TEST_140_112_2_17_23_51_45_4576 "\n WSP TEST no memory \n"
#define WSP_TEST_258_112_2_17_23_51_45_4577 "\n wsp TEST no memory \n"
#define WSP_TEST_462_112_2_17_23_51_45_4578 "wsp get wsp init cnf"
#define WSP_TEST_469_112_2_17_23_51_45_4579 "\n wsp test auth request id = %d\n"
#define WSP_TEST_475_112_2_17_23_51_45_4580 "\n wsp test request id = %d\n"
#define WSP_TEST_480_112_2_17_23_51_45_4581 "\n wsp test recv WSP_SIG_GET_CNF\n"
#define WSP_TEST_486_112_2_17_23_51_45_4582 "\n wsp test data ind rsp code=%d,request_id =%d,len = %d\n"
#define WSP_TEST_496_112_2_17_23_51_45_4583 "\n wsp test recv WSP_SIG_HEADER_IND,charset = %s,content encoding = %d, content len = %d\n"
#define WSP_TEST_498_112_2_17_23_51_45_4584 "\n wsp test recv WSP_SIG_HEADER_IND, rsp_code = %d, content type =%s\n"
#define WSP_TEST_502_112_2_17_23_51_45_4585 "\nwsp test recv WSP_SIG_HEADER_IND, header= %x"
#define WSP_TEST_516_112_2_17_23_51_45_4586 "\n wsp test recv WSP_SIG_ERROR_IND, ERROR=%d\n"
#define WSP_TEST_522_112_2_17_23_51_45_4587 "\n wsp test recv WSP_SIG_CLOSE_CNF\n"
#define WSP_TEST_538_112_2_17_23_51_45_4588 "\n wsp test net prog ind,%d,%d,%d\n"
#define WSP_TEST_542_112_2_17_23_51_45_4589 "\n wsp test redirect ind\n"
#define WSP_TEST_546_112_2_17_23_51_45_4590 "\n wsp test recv HTTP_SIG_POST_CNF\n"
#define WTP_API_113_112_2_17_23_51_46_4591 "wsp,WTP_UnpackWdpData, input error!"
#define WTP_API_124_112_2_17_23_51_46_4592 "wsp,WTP_UnpackWdpData, alloc memory error!"
#define WTP_API_132_112_2_17_23_51_46_4593 "wsp,WTP_UnpackWdpData, recv error pdu!"
#define WTP_API_140_112_2_17_23_51_46_4594 "wsp,WTP_UnpackWdpData, ocstr create error!"
#define WTP_API_148_112_2_17_23_51_46_4595 "wsp,WTP_UnpackWdpData, get char error!"
#define WTP_API_167_112_2_17_23_51_46_4596 "wsp,WTP_UnpackWdpData, unpack signal pdu error!"
#define WTP_HSM_319_112_2_17_23_51_47_4597 "\n wsp WTPHSM_Create wtp_hsm_ptr = %d"
#define WTP_HSM_334_112_2_17_23_51_47_4598 "\n wsp Wtp_EventQueueClear input error\n"
#define WTP_HSM_364_112_2_17_23_51_47_4599 "\n wsp WTPHSM_Destroy wtp_hsm_ptr = %d"
#define WTP_HSM_391_112_2_17_23_51_47_4600 "\n wsp WTPHSM_Destroy: close file handle = %d"
#define WTP_HSM_408_112_2_17_23_51_47_4601 "\n wsp WTPHSM_Destroy: psn list handle = %d! \n"
#define WTP_HSM_415_112_2_17_23_51_47_4602 "\n wsp WTPHSM_Destroy: delete timer! \n"
#define WTP_HSM_529_112_2_17_23_51_47_4603 "\n wsp QHSM_Wtp input error! \n"
#define WTP_HSM_540_112_2_17_23_51_47_4604 "\n wsp QHSM_Wtp receive wtp abort request! \n"
#define WTP_HSM_550_112_2_17_23_51_47_4605 "\n wsp QHSM_Wtp receive wtp rcv abort ind! \n"
#define WTP_HSM_559_112_2_17_23_51_47_4606 "\n wsp QHSM_Wtp receive wtp rcv error pdu ind! \n"
#define WTP_HSM_568_112_2_17_23_51_47_4607 "\n wsp QHSM_Wtp receive wtp rcv error pdu ind! \n"
#define WTP_HSM_577_112_2_17_23_51_47_4608 "\n wsp QHSM_Wtp receive destroy wtp hsm req! \n"
#define WTP_HSM_605_112_2_17_23_51_47_4609 "\n wsp HandleWtp_AbortReq input error! \n"
#define WTP_HSM_616_112_2_17_23_51_47_4610 "\n wsp HandleWtp_AbortReq  send abort pdu error! \n"
#define WTP_HSM_644_112_2_17_23_51_47_4611 "\n wsp HandleWtp_RcvAbortInd input error! \n"
#define WTP_HSM_656_112_2_17_23_51_48_4612 "\n wsp HandleWtp_RcvAbortInd: destroy wtp hsm error! \n"
#define WTP_HSM_664_112_2_17_23_51_48_4613 "\n wsp HandleWtp_RcvAbortInd: alloc error! \n"
#define WTP_HSM_693_112_2_17_23_51_48_4614 "\n wsp HandleWtp_DestroyInd input error! \n"
#define WTP_HSM_701_112_2_17_23_51_48_4615 "\n wsp HandleWtp_DestroyInd: alloc error! \n"
#define WTP_HSM_732_112_2_17_23_51_48_4616 "\n wsp HandleWtp_RcvInvoke input error! \n"
#define WTP_HSM_743_112_2_17_23_51_48_4617 "\n wsp HandleWtp_RcvInvoke: alloc error! \n"
#define WTP_HSM_753_112_2_17_23_51_48_4618 "\n wsp HandleWtp_RcvInvoke: alloc2 error! \n"
#define WTP_HSM_787_112_2_17_23_51_48_4619 "\n wsp HandleWtp_RcvErrorPdu enter! \n"
#define WTP_HSM_792_112_2_17_23_51_48_4620 "\n wsp HandleWtp_RcvErrorPdu input error! \n"
#define WTP_HSM_800_112_2_17_23_51_48_4621 "\n wsp HandleWtp_RcvErrorPdu: send abort error! \n"
#define WTP_HSM_809_112_2_17_23_51_48_4622 "\n wsp HandleWtp_RcvErrorPdu: deatroy wtp hsm error! \n"
#define WTP_HSM_817_112_2_17_23_51_48_4623 "\n wsp HandleWtp_RcvErrorPdu: alloc error! \n"
#define WTP_HSM_844_112_2_17_23_51_48_4624 "\n wsp Wtp_Idle input error! \n"
#define WTP_HSM_851_112_2_17_23_51_48_4625 "\n wsp Wtp_Idle: rcv invoke req! \n"
#define WTP_HSM_879_112_2_17_23_51_48_4626 "\n wsp Wtp_Segsendackwait input error! \n"
#define WTP_HSM_886_112_2_17_23_51_48_4627 "\n wsp Wtp_Segsendackwait: rcv wtp R timeout \n"
#define WTP_HSM_895_112_2_17_23_51_48_4628 "\n wsp Wtp_Segsendackwait: rcv wtp ack ind \n"
#define WTP_HSM_904_112_2_17_23_51_48_4629 "\n wsp Wtp_Segsendackwait: rcv wtp Nack ind \n"
#define WTP_HSM_913_112_2_17_23_51_48_4630 "\n wsp Wtp_Segsendackwait: rcv wtp send succ cnf! \n"
#define WTP_HSM_922_112_2_17_23_51_48_4631 "\n wsp Wtp_Segsendackwait: rcv wtp send succ fail! \n"
#define WTP_HSM_1003_112_2_17_23_51_48_4632 "\n wsp HandleSegsendackwait_Rcvack: send ack pdu to verify tid,timer handle = %x! \n"
#define WTP_HSM_1007_112_2_17_23_51_48_4633 "\n wsp HandleSegsendackwait_Rcvack: tid does not match! \n"
#define WTP_HSM_1127_112_2_17_23_51_48_4634 "\n wsp HandleSegsendackwait_Rcvack: new timer handle = %x! \n"
#define WTP_HSM_1159_112_2_17_23_51_49_4635 "\n wsp HandleSegsendackwait_Rcvack: 2 send all packets! \n"
#define WTP_HSM_1166_112_2_17_23_51_49_4636 "\n wsp HandleSegsendackwait_Rcvack: send all packets! \n"
#define WTP_HSM_1190_112_2_17_23_51_49_4637 "\n wsp HandleSegsendackwait_Rcvack: error = %d! \n"
#define WTP_HSM_1226_112_2_17_23_51_49_4638 "\n wsp HandleSegsendackwait_RcvNack: missing num = %d\n"
#define WTP_HSM_1236_112_2_17_23_51_49_4639 "\n wsp HandleSegsendackwait_RcvNack: pack missed psn = %d\n"
#define WTP_HSM_1402_112_2_17_23_51_49_4640 "\n wsp HandleSegsendackwait_RcvNack: new timer handle = %x\n"
#define WTP_HSM_1483_112_2_17_23_51_49_4641 "wsp HandleSegsendackwait_Rtimeout: new timer handle =%x"
#define WTP_HSM_1678_112_2_17_23_51_50_4642 "\n wsp HandleSegsendackwait_Sendfail enter! \n"
#define WTP_HSM_1737_112_2_17_23_51_50_4643 "\n wsp Wtp_Resultwait input error! \n"
#define WTP_HSM_1744_112_2_17_23_51_50_4644 "\n wsp Wtp_Resultwait: rcv R timeout ind! \n"
#define WTP_HSM_1753_112_2_17_23_51_50_4645 "\n wsp Wtp_Resultwait: rcv ack ind! \n"
#define WTP_HSM_1762_112_2_17_23_51_50_4646 "\n wsp Wtp_Resultwait: rcv result cnf! \n"
#define WTP_HSM_1771_112_2_17_23_51_50_4647 "\n wsp Wtp_Resultwait: rcv seg result cnf! \n"
#define WTP_HSM_1780_112_2_17_23_51_50_4648 "\n wsp Wtp_Resultwait: rcv send succ cnf! \n"
#define WTP_HSM_1789_112_2_17_23_51_50_4649 "\n wsp Wtp_Resultwait: rcv send fail cnf! \n"
#define WTP_HSM_1861_112_2_17_23_51_50_4650 "wsp HandleResultwait_Rtimeout: new timer handle =%x"
#define WTP_HSM_1873_112_2_17_23_51_50_4651 "\n wsp HandleResultwait_Rtimeout: rcr is max! \n"
#define WTP_HSM_1945_112_2_17_23_51_50_4652 "wsp HandleResultwait_Rcvack: new timer handle = %x"
#define WTP_HSM_2055_112_2_17_23_51_50_4653 "\n WSP HandleResultwait_RcvSegresult: psn list handle = %d\n"
#define WTP_HSM_2069_112_2_17_23_51_50_4654 "WSP:HandleResultwait_RcvSegresult, packet offset = %d"
#define WTP_HSM_2075_112_2_17_23_51_50_4655 "WSP:HandleResultwait_RcvSegresult, recv new seg result!"
#define WTP_HSM_2086_112_2_17_23_51_50_4656 "WSP:HandleResultwait_RcvSegresult, sended = %d, data_len = %d, insert_pos=%d"
#define WTP_HSM_2128_112_2_17_23_51_50_4657 "\n wsp HandleResultwait_RcvSegresult: gtr ttr = %d, timer handle = %x\n"
#define WTP_HSM_2163_112_2_17_23_51_51_4658 "WSP:HandleResultwait_RcvSegresult, result to app send len =%d"
#define WTP_HSM_2174_112_2_17_23_51_51_4659 "\n wsp HandleResultwait_RcvSegresult: packet is not continue send Nack\n"
#define WTP_HSM_2226_112_2_17_23_51_51_4660 "\n wsp HandleResultwait_RcvSegresult: packet is not continue send Nack\n"
#define WTP_HSM_2239_112_2_17_23_51_51_4661 "\n wsp HandleResultwait_RcvSegresult: has receive last packet\n"
#define WTP_HSM_2243_112_2_17_23_51_51_4662 "\n wsp HandleResultwait_RcvSegresult: event is not last packet,but last packet has received already!\n"
#define WTP_HSM_2327_112_2_17_23_51_51_4663 "\n wsp SendResultIndtoWsp input error! \n"
#define WTP_HSM_2331_112_2_17_23_51_51_4664 "WSP SendResultIndtoWsp, send result to wsp ,len = %d"
#define WTP_HSM_2336_112_2_17_23_51_51_4665 "\n wsp SendResultIndtoWsp alloc error! \n"
#define WTP_HSM_2354_112_2_17_23_51_51_4666 "\n wsp SendResultIndtoWsp alloc 2 error! \n"
#define WTP_HSM_2400_112_2_17_23_51_51_4667 "\n wsp HandleResultwait_Rcvresult input error! \n"
#define WTP_HSM_2411_112_2_17_23_51_51_4668 "\n wsp HandleResultwait_Rcvresult: tid does not match! \n"
#define WTP_HSM_2424_112_2_17_23_51_51_4669 "\n WSP HandleResultwait_Rcvresult: psn list handle = %d\n"
#define WTP_HSM_2435_112_2_17_23_51_51_4670 "\n wsp HandleResultwait_Rcvresult:NOT_LAST_PACKET,has exist 0 psn! \n"
#define WTP_HSM_2446_112_2_17_23_51_51_4671 "WSP:HandleResultwait_Rcvresult, packet offset = %d"
#define WTP_HSM_2463_112_2_17_23_51_51_4672 "WSP:HandleResultwait_Rcvresult, inser= %d, sended = %d, datalen=%d"
#define WTP_HSM_2484_112_2_17_23_51_51_4673 "\n wsp HandleResultwait_Rcvresult: has receive last packet\n"
#define WTP_HSM_2488_112_2_17_23_51_51_4674 "\n wsp HandleResultwait_Rcvresult: event is not last packet,but last packet has received already!\n"
#define WTP_HSM_2545_112_2_17_23_51_51_4675 "\n wsp HandleResultwait_Rcvresult:NOT_LAST_PACKET,new timer handle = %x! \n"
#define WTP_HSM_2615_112_2_17_23_51_51_4676 "\n WSP HandleResultwait_Rcvresult: psn 2 list handle = %d\n"
#define WTP_HSM_2626_112_2_17_23_51_51_4677 "\n wsp HandleResultwait_Rcvresult:LAST_PACKET_OF_GROUP,has exist 0 psn! \n"
#define WTP_HSM_2724_112_2_17_23_51_52_4678 "\n wsp HandleResultwait_Rcvresult:LAST_PACKET_of_group,new timer handle = %x! \n"
#define WTP_HSM_2769_112_2_17_23_51_52_4679 "\n wsp HandleResultwait_Rcvresult: error_code =%d! \n"
#define WTP_HSM_2797_112_2_17_23_51_52_4680 "\n wsp WTP_SendDisconnectPdu input error! \n"
#define WTP_HSM_2805_112_2_17_23_51_52_4681 "\n wsp WTP_SendDisconnectPdu: pack invoke pdu header error! \n"
#define WTP_HSM_2814_112_2_17_23_51_52_4682 "\n wsp WTP_SendDisconnectPdu: alloc error! \n"
#define WTP_HSM_3020_112_2_17_23_51_52_4683 "\n wsp HandleWtpFatalError input error! \n"
#define WTP_HSM_3024_112_2_17_23_51_52_4684 "\n wsp HandleWtpFatalError enter, error_code = %d! \n"
#define WTP_HSM_3036_112_2_17_23_51_52_4685 "\n wsp HandleWtpFatalError,alloc error!\n"
#define WTP_HSM_3092_112_2_17_23_51_52_4686 "\n wsp BeginSarInvoke: last packet psn = %d! \n"
#define WTP_HSM_3096_112_2_17_23_51_52_4687 "\n wsp BeginSarInvoke: user data are too long,max than 255! \n"
#define WTP_HSM_3224_112_2_17_23_51_53_4688 "\n wsp BeginSarInvoke: timer handle  = %x! \n"
#define WTP_HSM_3623_112_2_17_23_51_53_4689 "\n wsp SendAllInvoke: timer handle  = %x! \n"
#define WTP_HSM_3740_112_2_17_23_51_54_4690 "WSP:IsPackageContinue, list handle is pnull!"
#define WTP_HSM_3750_112_2_17_23_51_54_4691 "WSP:IsPackageContinue, first node value =%d!"
#define WTP_HSM_3762_112_2_17_23_51_54_4692 "WSP:IsPackageContinue, compare, first =%d, second = %d!"
#define WTP_HSM_3814_112_2_17_23_51_54_4693 "\n wsp InsertPsnToList:enter psn = %d! num=%d \n"
#define WTP_HSM_3820_112_2_17_23_51_54_4694 "\n wsp InsertPsnToList fist psn"
#define WTP_HSM_3837_112_2_17_23_51_54_4695 "\n wsp InsertPsnToList: 0!=current node"
#define WTP_HSM_3840_112_2_17_23_51_54_4696 "\n wsp InsertPsnToList:current psn = %d! ,insert = %d\n"
#define WTP_HSM_3866_112_2_17_23_51_54_4697 "\n wsp InsertPsnToList, insert to next node!\n"
#define WTP_HSM_3875_112_2_17_23_51_54_4698 "\n WSP InsertPsnToList, insert to last node!\n"
#define WTP_HSM_3904_112_2_17_23_51_54_4699 "\n WSP InsertPsnToList, insert to first node!\n"
#define WTP_HSM_3917_112_2_17_23_51_54_4700 "\n wsp InsertPsnToList:over psn = %d! \n"
#define WTP_HSM_3927_112_2_17_23_51_54_4701 "\n wsp InsertPsnToList:over2 psn = %d, error_code = %d \n"
#define WTP_HSM_3951_112_2_17_23_51_54_4702 "WSP:WTP_IsPackageExist, packet is existed! psn = %d"
#define WTP_HSM_4269_112_2_17_23_51_55_4703 "\n wsp WTP_EventDestroy input error! \n"
#define WTP_HSM_4329_112_2_17_23_51_55_4704 "WSP:WSP_EventDestroy.sig code error!"
#define WTP_HSM_4363_112_2_17_23_51_55_4705 "\n wsp SendAbortPdu: abort tid =%d! \n"
#define WTP_HSM_4368_112_2_17_23_51_55_4706 "\n wsp SendAbortPdu: pack abort pdu error! \n"
#define WTP_HSM_4401_112_2_17_23_51_55_4707 "\n wsp SendAbortPdu:pack for trans error! \n"
#define WTP_HSM_4418_112_2_17_23_51_55_4708 "\n wsp SendAbortPdu: trans send error! \n"
#define WTP_HSM_4543_112_2_17_23_51_55_4709 "WSP:WTP_HandleExistPsn, not gtr ttr, need not handle!"
#define WTP_HSM_4550_112_2_17_23_51_55_4710 "WSP:WTP_HandleExistPsn, last packet of message!!"
#define WTP_HSM_4556_112_2_17_23_51_55_4711 "WSP:WTP_HandleExistPsn, last packet of group!!"
#define WTP_HSM_4579_112_2_17_23_51_55_4712 "WSP:WTP_HandleExistPsn, rcv gtr ttr, send result to wsp.len = %d"
#define WTP_HSM_4590_112_2_17_23_51_55_4713 "WSP:WTP_HandleExistPsn, last packet of message, destroy wtp hsm!!"
#define WTP_PDU_596_112_2_17_23_51_57_4714 "\n wsp WTP_PackNack: node psn = %d, i = %d\n"
#define WTP_PDU_603_112_2_17_23_51_57_4715 "\n wsp WTP_PackNack: missed num = %d! \n"
#define WTP_PDU_774_112_2_17_23_51_57_4716 "WTP WTP_PackAck : error , tpi len is too long and large than 3."
#define WTP_PDU_1815_112_2_17_23_51_59_4717 "UnpackInvokePdu : The CON is not 0 in the arrived PUD head.There are TPIs but you have no procedure"
#define WTP_PDU_2186_112_2_17_23_52_0_4718 "WSP:UnpackSegResultPdu, exist psn num = %d"
#define WTP_PDU_2190_112_2_17_23_52_0_4719 "WSP:UnpackSegResultPdu, exist 1 psn num = %d"
#define WTP_PDU_2324_112_2_17_23_52_0_4720 "wsp: WTP_UnpackSignalPdu, receive unkonwn pdu type"
#define WSP_HEADERS_913_112_2_17_23_52_3_4721 "unpack_well_known_field, error"
#define WSP_HEADERS_919_112_2_17_23_52_3_4722 "unpack_well_known_field, name is null error"
#define WSP_HEADERS_1113_112_2_17_23_52_3_4723 "wsp_headers_unpack: there is no content type!"
#define WSP_HEADERS_1731_112_2_17_23_52_4_4724 "WSP: pack_short_integer, integer invalid!"
#define WSP_LIST_124_112_2_17_23_52_7_4725 "WSP: list_insert, pos invalid!"
#define WSP_LIST_382_112_2_17_23_52_8_4726 "WSP: delete_items_from_list, input invalid!"
#define WSP_LIST_388_112_2_17_23_52_8_4727 "WSP: delete_items_from_list 3333, input invalid!"
#define WSP_OCTSTR_219_112_2_17_23_52_8_4728 "WSP: wspheader_octstr_append_char, input invalid!"
#define WSP_OCTSTR_381_112_2_17_23_52_9_4729 "WSP: wspheader_octstr_copy_real, input invalid!"
#define WSP_OCTSTR_422_112_2_17_23_52_9_4730 "WSP: wspheader_octstr_search_char, input invalid!"
#define WSP_PARSE_232_112_2_17_23_52_10_4731 "WSP: parse_set_error, input invalid!"
#define WTLS_ALERT_67_112_2_17_23_52_12_4732 "WTLS_AlertSend : invalid input"
#define WTLS_ALERT_74_112_2_17_23_52_12_4733 "WTLS_AlertSend : PDU create fail"
#define WTLS_ALERT_87_112_2_17_23_52_12_4734 "WTLS_AlertSend :pdu pack fail"
#define WTLS_ALERT_94_112_2_17_23_52_12_4735 "WTLS_AlertSend :payload pack fail"
#define WTLS_ALERT_126_112_2_17_23_52_12_4736 "WTLS_AlertEventHandle : invalid input"
#define WTLS_ALERT_137_112_2_17_23_52_12_4737 "WTLS_ALERTEVENTHANDLE :error event create fail"
#define WTLS_ALERT_150_112_2_17_23_52_12_4738 "WTLS_ALERTEVENTHANDLE :warning event create fail"
#define WTLS_ALGOPROCESS_192_112_2_17_23_52_13_4739 "WTLSALGOPROC_HASH: encounter problem =%d\n"
#define WTLS_ALGOPROCESS_319_112_2_17_23_52_14_4740 "WtlsHmacHash: encounter problem=%d\n"
#define WTLS_ALGOPROCESS_422_112_2_17_23_52_14_4741 "WtlsPHash: encounter problem =%d\n"
#define WTLS_ALGOPROCESS_500_112_2_17_23_52_14_4742 "WTLSALGOPROC_CalculateWTLSPrf: encounter problem=%d\n"
#define WTLS_ALGOPROCESS_559_112_2_17_23_52_14_4743 "WTLSALGOPROC_DataEncrypt:encounter problem =%d \n"
#define WTLS_ALGOPROCESS_696_112_2_17_23_52_14_4744 "WTLSALGOPROC_CalculateServerKeyBlock: encounter problem=%d\n"
#define WTLS_ALGOPROCESS_838_112_2_17_23_52_15_4745 "WTLSALGOPROC_CalculateClientKeyBlock: encounter problem=%d\n"
#define WTLS_ALGOPROCESS_1184_112_2_17_23_52_15_4746 "WTLSALGOPROC_Encrypt: encounter error=%d \n"
#define WTLS_ALGOPROCESS_1505_112_2_17_23_52_16_4747 "WTLSALGOPROC_Decrypt: encounter problem=%d\n"
#define WTLS_ALGOPROCESS_1627_112_2_17_23_52_16_4748 "WTLSALGOPROC_DecryptPduList: encounter problem=%d\n"
#define WTLS_ALGOPROCESS_1771_112_2_17_23_52_17_4749 "WtlsVerifyMac: encounter problem= %d\n"
#define WTLS_ALGOPROCESS_1838_112_2_17_23_52_17_4750 "WTLSALGOPROC_FinishedVerify: encounter problem= %d\n"
#define WTLS_API_119_112_2_17_23_52_17_4751 "WTLS_AsyncRegCallback:the handle not exist\n"
#define WTLS_API_126_112_2_17_23_52_17_4752 "WTLS_Handshake: the handler not registered\n"
#define WTLS_API_176_112_2_17_23_52_17_4753 "WTLS_Create : input is invalid\n"
#define WTLS_API_184_112_2_17_23_52_17_4754 "WTLS_create: create machine fail!\n"
#define WTLS_API_212_112_2_17_23_52_17_4755 "WTLS_AsyncRegCallback:the handle not exist\n"
#define WTLS_API_220_112_2_17_23_52_17_4756 "WTLS_Handshake: the handler not registered\n"
#define WTLS_API_228_112_2_17_23_52_17_4757 "WTLS_Close : destroy machine handle\n"
#define WTLS_API_251_112_2_17_23_52_17_4758 "WTLS_Handshake: the input is invalid\n"
#define WTLS_API_259_112_2_17_23_52_17_4759 "WTLS_Handshake: the handler not registered\n"
#define WTLS_API_283_112_2_17_23_52_17_4760 "WTLS_Handshake: client hello event create fail\n"
#define WTLS_API_319_112_2_17_23_52_17_4761 "WTLS_DecryptPasser: the input is invalid\n"
#define WTLS_API_327_112_2_17_23_52_17_4762 "WTLS_DecryptPasser: handler not registered\n"
#define WTLS_API_392_112_2_17_23_52_18_4763 "WTLS_DecryptPasser : encounter problem\n"
#define WTLS_API_422_112_2_17_23_52_18_4764 "WTLS_EncryptPasser: input invalid\n"
#define WTLS_API_430_112_2_17_23_52_18_4765 "WTLS_EncryptPasser: handler not registered\n"
#define WTLS_API_473_112_2_17_23_52_18_4766 "WTLS_EncryptPasser: error code\n"
#define WTLS_API_498_112_2_17_23_52_18_4767 "WTLS_UserCnfCert: input invalid\n"
#define WTLS_API_506_112_2_17_23_52_18_4768 "WTLS_UserCnfCert: handler not registered\n"
#define WTLS_API_518_112_2_17_23_52_18_4769 "WTLS_UserCnf: event create fail\n"
#define WTLS_API_534_112_2_17_23_52_18_4770 "WTLS_UserCnf: error_code \n"
#define WTLS_API_556_112_2_17_23_52_18_4771 "WTLS_AddMachineToTable: WTLS_handle is invalid"
#define WTLS_API_587_112_2_17_23_52_18_4772 "WTLS_DeleteMachineFromTable: input is invalid\n"
#define WTLS_API_665_112_2_17_23_52_18_4773 "WTLS_DeleteMachineFromTable: input is invalid\n"
#define WTLS_API_827_112_2_17_23_52_18_4774 "WTLS_LoadSession: list_ptr is invalid"
#define WTLS_CERT_117_112_2_17_23_52_19_4775 "WTLSCERT_LoadCert:encounter problem\n"
#define WTLS_EVENT_370_112_2_17_23_52_20_4776 "WTLS_EventCreate: malloc fail"
#define WTLS_HANDSHAKE_261_112_2_17_23_52_21_4777 "get_DhKeys:encounter problem=%d\n"
#define WTLS_HANDSHAKE_442_112_2_17_23_52_21_4778 "WTLSHAND_GetDhPubKey:encounter problem=%d\n"
#define WTLS_HANDSHAKE_616_112_2_17_23_52_21_4779 "WTLS_HANDRECEIVED DATA :error_code =%d\n"
#define WTLS_HANDSHAKE_683_112_2_17_23_52_21_4780 "WTLS_EncryptUserDataToSend: encounter problem=%d\n"
#define WTLS_HANDSHAKE_843_112_2_17_23_52_22_4781 "WTLSHAND_SendClientHello : encounters error =%d\n"
#define WTLS_HANDSHAKE_1218_112_2_17_23_52_22_4782 "WTLSHAND_HandleServerHello : encounters error =%d\n"
#define WTLS_HANDSHAKE_1383_112_2_17_23_52_23_4783 "WTLSHAND_HandleClientExchange : encounters erro =%d\n"
#define WTLS_HANDSHAKE_1516_112_2_17_23_52_23_4784 "SendChangeCipherAndFinshedMessage: encounters error=%d\n"
#define WTLS_HANDSHAKE_1810_112_2_17_23_52_24_4785 "WTLSHAND_HandleServerHelloFinish: encounter problem=%d\n"
#define WTLS_HANDSHAKE_1966_112_2_17_23_52_24_4786 "WTLSHAND_SendClientFinish: encounter problem =%d\n"
#define WTLS_HANDSHAKE_2109_112_2_17_23_52_24_4787 "WTLSHAND_HandleServerCCFinish: encounter problem=%d \n"
#define WTLS_MACHINE_122_112_2_17_23_52_25_4788 "WTLSMACHINE_MachineInitialize:encounter problem \n"
#define WTLS_MACHINE_159_112_2_17_23_52_25_4789 "WTLSMACHINE_Create: initial fail \n"
#define WTLS_MACHINE_165_112_2_17_23_52_25_4790 "WTLSMACHINE_Create: alloc fail \n"
#define WTLS_PDU_116_112_2_17_23_52_26_4791 "WTLSPDU_CreatePdu :encounter problem\n"
#define WTLS_PDU_280_112_2_17_23_52_26_4792 "WTLSPDU_PackPdu: encounter problem =%d\n"
#define WTLS_PDU_524_112_2_17_23_52_27_4793 "WTLSPDU_PackWtlsHandShakePdu:encounter problem =%d\n"
#define WTLS_PDU_811_112_2_17_23_52_27_4794 "WTLSPDU_AddPdu: encounter problem=%d\n"
#define WTLS_PDU_910_112_2_17_23_52_27_4795 "WTLS_PayloadPack: encounter problem =%d\n"
#define WTLS_PDU_1055_112_2_17_23_52_28_4796 "WTLSPDU_UnpackPDU :error= %d\n"
#define WTLS_PDU_1220_112_2_17_23_52_28_4797 "WTLSPDU_UnpackWtlsHandshakePdu : encounter problem=%d\n"
#define WTLS_PDU_1345_112_2_17_23_52_28_4798 "WTLSPDU_PayloadUnpack encounter problem =%d\n"
#define WTLS_PDU_1530_112_2_17_23_52_29_4799 "WTLS_DATAGRAMUNPACK: event create fail"
#define WTLS_PDU_1537_112_2_17_23_52_29_4800 "WTLS_DATAGRAMUNPACK: payloadlist unpack fail"
#define WTLS_PDUSUPPORT_442_112_2_17_23_52_29_4801 "WTLSPDUS_UnpackServerHello : encounter problem=%d\n"
#define WTLS_PDUSUPPORT_562_112_2_17_23_52_30_4802 "WTLSPDUS_PackKeyexSuiteList: encounter problem=%d\n"
#define WTLS_PDUSUPPORT_646_112_2_17_23_52_30_4803 "WTLSPDUS_PackBulkCipherSuiteList:encounter problem =%d\n"
#define WTLS_PDUSUPPORT_695_112_2_17_23_52_30_4804 "WTLSPDUS_PackCompressionMethodList : encounter problem =%d\n"
#define WTLS_PDUSUPPORT_752_112_2_17_23_52_30_4805 "WTLSPDUS_PackRandom : encounter problem =%d\n"
#define WTLS_PDUSUPPORT_914_112_2_17_23_52_30_4806 "WTLSPDUS_WtlsGetRandom : encounter error=%d\n"
#define WTLS_PDUSUPPORT_979_112_2_17_23_52_31_4807 "WTLSPDUS_CreateRandom: encounter problem=%d\n"
#define WTLS_PDUSUPPORT_1142_112_2_17_23_52_31_4808 "WTLSPDUS_GetEncryptedSecret: encounter problem=%d\n"
#define WTLS_PDUSUPPORT_1244_112_2_17_23_52_31_4809 "WTLSPDUS_CreateClientKeyexSuite : encounter problem =%d\n"
#define WTLS_PDUSUPPORT_1313_112_2_17_23_52_31_4810 "WTLS_CREATECIPHERSUITE LIST FAIL"
#define WTLS_PDUSUPPORT_1475_112_2_17_23_52_31_4811 "WTLSPDUS_UnPackWCert:encounter problem =%d\n"
#define WTLS_PDUSUPPORT_1565_112_2_17_23_52_32_4812 "WTLSPDUS_Unpack_Signature : encounter problem=%d \n"
#define WTLS_PDUSUPPORT_1674_112_2_17_23_52_32_4813 "WTLSPDUS_UnpackECPubKey:encounter problem =%d\n"
#define WTLS_PDUSUPPORT_1748_112_2_17_23_52_32_4814 "WTLSPDUS_UnpackPubKey : encounter problem =%d\n"
#define WTLS_PDUSUPPORT_1843_112_2_17_23_52_32_4815 "WTLSPDUS_UnpackParamSpec : encounter problem =%d\n"
#define WTLS_PDUSUPPORT_1931_112_2_17_23_52_32_4816 "WTLSPDUS_PackOctstr16: encounter problem=%d\n"
#define WTLS_PDUSUPPORT_1974_112_2_17_23_52_32_4817 "WTLSPDUS_UnpackOctstr16: encounter problem=%d\n"
#define WTLS_PDUSUPPORT_2025_112_2_17_23_52_33_4818 "WTLSPDUS_UnpackDHParams : encounter problem=%d \n"
#define WTLS_PDUSUPPORT_2139_112_2_17_23_52_33_4819 "WTLSPDUS_UnpackECParams encounter problem=%d\n"
#define WTLS_PDUSUPPORT_2273_112_2_17_23_52_33_4820 "WTLSPDUS_UnpackWtlsIdentify:encounter problem=%d\n"
#define WTLS_PDUSUPPORT_2745_112_2_17_23_52_34_4821 "WtlsPackDhParams:encounter problem=%d\n"
#define WTLS_PDUSUPPORT_2981_112_2_17_23_52_34_4822 "WTLSPDUS_PackParamSpec:encounter problem=%d\n"
#define WTLS_PDUSUPPORT_3044_112_2_17_23_52_35_4823 "WtlsPackKeyExSuite: encounter problem\n"
#define XMLEXPATIF_384_112_2_17_23_52_36_4824 "\n\nXML expat: (%d):%s\n"
#define XMLEXPATIF_680_112_2_17_23_52_36_4825 "XML ExpatRealloc len err, len:%d, new_size:%d"
#define XMLEXPATIF_686_112_2_17_23_52_36_4826 "XML ExpatRealloc, malloc p2 err!"
#define XMLEXPATIF_905_112_2_17_23_52_37_4827 "XML ConvertUtf8WithLenToUCS2 malloc try_buff_ptr fail!"
#define XMLEXPATIF_918_112_2_17_23_52_37_4828 "XML ConvertUtf8WithLenToUCS2 malloc dest_ptr fail!"
#define XMLEXPATIF_967_112_2_17_23_52_37_4829 "XML ConvertUtf8ArrayToUCS2Array malloc ucs_arr fail!"
#define XMLPARSE_6341_112_2_17_23_52_50_4830 "XML ExpatRealloc len err, len:%d, new_size:%d"
#define XMLPARSE_6347_112_2_17_23_52_50_4831 "XML ExpatRealloc, malloc p2 err!"
#define XMLTEST_52_112_2_17_23_52_53_4832 "\n\nSAX.startElementNs"
#define XMLTEST_90_112_2_17_23_52_53_4833 "\n\nSAX.startElementNs"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(DAPS_TRC)
TRACE_MSG(DLMALLOC_API_176_112_2_17_23_14_12_0,"memcheck:aligned alig = %d ,s = %u \n")
TRACE_MSG(DLMALLOC_API_179_112_2_17_23_14_12_1,"memcheck:aligned mem_ptr = %x ,s = %u \n")
TRACE_MSG(DLMALLOC_API_228_112_2_17_23_14_12_2,"memcheck:rl new size_1 = %x \n")
TRACE_MSG(DLMALLOC_API_233_112_2_17_23_14_12_3,"memcheck:rl new size_2 = %x \n")
TRACE_MSG(DLMALLOC_API_295_112_2_17_23_14_12_4,"CFL_DestroyMspace invalid input")
TRACE_MSG(DLMALLOC_API_314_112_2_17_23_14_12_5,"CFL_MspaceMalloc invalid input")
TRACE_MSG(DLMALLOC_API_336_112_2_17_23_14_12_6,"CFL_MspaceFree invalid input")
TRACE_MSG(DLMALLOC_API_358_112_2_17_23_14_12_7,"CFL_MspaceRealloc invalid input")
TRACE_MSG(DLMALLOC_API_381_112_2_17_23_14_12_8,"CFL_MspaceGetInfo invalid input")
TRACE_MSG(CFL_MD5_231_112_2_17_23_14_17_9,"CFL_MD5Hash input error")
TRACE_MSG(CFL_MD5_234_112_2_17_23_14_17_10,"CFL_MD5Hash dest_ptr =%x,md =%x")
TRACE_MSG(CFL_MD5_236_112_2_17_23_14_17_11,"CFL_MD5Hash after init dest_ptr =%x,md =%x")
TRACE_MSG(CFL_MD5_238_112_2_17_23_14_17_12,"CFL_MD5Hash after update dest_ptr =%x,md =%x")
TRACE_MSG(CFL_MD5_240_112_2_17_23_14_17_13,"CFL_MD5Hash after final dest_ptr =%x,md =%x")
TRACE_MSG(CFL_MD5_252_112_2_17_23_14_17_14,"CFL_MD5Init mdcontext =%x")
TRACE_MSG(CFL_MD5_284_112_2_17_23_14_17_15,"CFL_MD5Update input error")
TRACE_MSG(CFL_MD5_288_112_2_17_23_14_17_16,"CFL_MD5Update mdcontext =%x")
TRACE_MSG(CFL_MD5_315_112_2_17_23_14_17_17,"CFL_MD5Update i=%d,ii=%d")
TRACE_MSG(CFL_MD5_320_112_2_17_23_14_17_18,"CFL_MD5Update end mdcontext =%x")
TRACE_MSG(CFL_MD5_337_112_2_17_23_14_17_19,"CFL_MD5Final mdcontext =%x")
TRACE_MSG(CFL_MD5_353_112_2_17_23_14_17_20,"CFL_MD5Final after update mdcontext =%x")
TRACE_MSG(CFL_MD5_362_112_2_17_23_14_17_21,"CFL_MD5Final after append mdcontext =%x,i=%d,ii=%d")
TRACE_MSG(CFL_MD5_364_112_2_17_23_14_17_22,"CFL_MD5Final after transform mdcontext =%x")
TRACE_MSG(CFL_MD5_376_112_2_17_23_14_17_23,"CFL_MD5Final after srore mdcontext =%x,i=%d,ii=%d")
TRACE_MSG(CFL_MEM_DEBUG_169_112_2_17_23_14_19_24,"\n[DAPS] MEMORY TRACE START\n")
TRACE_MSG(CFL_MEM_DEBUG_173_112_2_17_23_14_19_25,"(%d)Leak at: %s(%d)\n")
TRACE_MSG(CFL_MEM_DEBUG_174_112_2_17_23_14_19_26,"Address: 0x%08x")
TRACE_MSG(CFL_MEM_DEBUG_178_112_2_17_23_14_19_27,"\n[DAPS] MEMORY LEAK %d\n")
TRACE_MSG(CFL_MEM_DEBUG_179_112_2_17_23_14_19_28,"\n[DAPS] MEMORY TRACE END\n")
TRACE_MSG(CFL_MEM_DEBUG_180_112_2_17_23_14_19_29,"\n[DAPS] MEMORY PEAK %u\n")
TRACE_MSG(CFL_MEM_DEBUG_181_112_2_17_23_14_19_30,"\n[DAPS] MEMORY USED %u\n")
TRACE_MSG(CFL_CHAR_164_112_2_17_23_14_20_31,"\n CFLCHAR_StrNICmp input error")
TRACE_MSG(CFL_CHAR_192_112_2_17_23_14_20_32,"CFL_Snprintf warning:buffer maybe has truncated!")
TRACE_MSG(CFL_STRING_64_112_2_17_23_14_21_33,"CFL_MEMMOVE: invalid input")
TRACE_MSG(CFL_STRING_110_112_2_17_23_14_21_34,"CFL_GetChar: invalid input")
TRACE_MSG(CFL_STRING_116_112_2_17_23_14_21_35,"CFL_GetChar: invalid pos")
TRACE_MSG(CFL_STRING_138_112_2_17_23_14_21_36,"CFL_SetChar : invalid string")
TRACE_MSG(CFL_STRING_144_112_2_17_23_14_21_37,"CFL_SetChar : invalid pos")
TRACE_MSG(CFL_STRING_170_112_2_17_23_14_21_38,"CFL_GETCHAR: invalid input")
TRACE_MSG(CFL_STRING_368_112_2_17_23_14_22_39,"CFL_OCTSTRDELET: invalid input")
TRACE_MSG(CFL_STRING_401_112_2_17_23_14_22_40,"CFL_octstrcreateempty:create string fail")
TRACE_MSG(CFL_STRING_444_112_2_17_23_14_22_41,"CFL_GROW: realloc fail ")
TRACE_MSG(CFL_STRING_455_112_2_17_23_14_22_42,"CFL_STRGROW: input invalid")
TRACE_MSG(CFL_STRING_481_112_2_17_23_14_22_43,"CFL_CREATEFROMDATA: input invalid")
TRACE_MSG(CFL_STRING_502_112_2_17_23_14_22_44,"CFL_CREATEFROMDATA: alloc space fail")
TRACE_MSG(CFL_STRING_510_112_2_17_23_14_22_45,"CFL_CREATEFROMDATA: create fail")
TRACE_MSG(CFL_STRING_558_112_2_17_23_14_22_46,"CFL_Octstrdestroy: invalid input")
TRACE_MSG(CFL_STRING_584_112_2_17_23_14_22_47,"CFL_OctstrCopy: invalid input")
TRACE_MSG(CFL_STRING_646_112_2_17_23_14_22_48,"CFL_OctstrInsertData: invalid input")
TRACE_MSG(CFL_STRING_694_112_2_17_23_14_22_49,"CFL_APPENDATA: invalid input")
TRACE_MSG(CFL_STRING_730_112_2_17_23_14_22_50,"CFL_APPENDCSTR: invalid input")
TRACE_MSG(CFL_STRING_764_112_2_17_23_14_22_51,"CFL_APPENDCHAR : invalid input")
TRACE_MSG(CFL_STRING_815_112_2_17_23_14_22_52,"CFL_string cat :not enough space")
TRACE_MSG(CFL_STRING_845_112_2_17_23_14_22_53,"CFL_RETRIEVEDATA: invalid input")
TRACE_MSG(CFL_STRING_1098_112_2_17_23_14_23_54,"CFL_PACKOctstr: invalid input")
TRACE_MSG(CFL_STRING_1165_112_2_17_23_14_23_55,"#CFL CFL_StrClone len==0!")
TRACE_MSG(CFL_STRING_1184_112_2_17_23_14_23_56,"#CFL CFL_StrClone alloc fail!")
TRACE_MSG(CFL_WCHAR_49_112_2_17_23_14_24_57,"\n CFLWCHAR_StrCmp input error")
TRACE_MSG(CFL_WCHAR_86_112_2_17_23_14_24_58,"\n CFLWCHAR_StrCmpWithAsciiStr input error")
TRACE_MSG(CFL_WCHAR_437_112_2_17_23_14_25_59,"\n--------\n")
TRACE_MSG(CFL_WCHAR_444_112_2_17_23_14_25_60,"%s")
TRACE_MSG(CFL_WCHAR_451_112_2_17_23_14_25_61,"%s")
TRACE_MSG(CFL_WCHAR_453_112_2_17_23_14_25_62,"\n--------\n")
TRACE_MSG(CFL_TIMER_92_112_2_17_23_14_26_63,"CFLTIMER_CreateTimer invalid input")
TRACE_MSG(CFL_TIMER_105_112_2_17_23_14_26_64,"CFLTIMER_CreateTimer InsertNode fail")
TRACE_MSG(CFL_TIMER_112_112_2_17_23_14_26_65,"CFLTIMER_CreateTimer created_timer_data_ptr is null")
TRACE_MSG(CFL_TIMER_115_112_2_17_23_14_26_66,"CFLTIMER creat, context_ptr:%x,sig_code:%x")
TRACE_MSG(CFL_TIMER_144_112_2_17_23_14_26_67,"CFLTIMER_CreateTimer SCI_CreateTimer fail!")
TRACE_MSG(CFL_TIMER_168_112_2_17_23_14_26_68,"CFLTIMER delete,get data err!")
TRACE_MSG(CFL_TIMER_171_112_2_17_23_14_26_69,"CFLTIMER delete, context_ptr:%x,sig_code:%x")
TRACE_MSG(CFL_TIMER_240_112_2_17_23_14_26_70,"CFLTIMER CALLBACK, context_ptr:%x")
TRACE_MSG(CFL_TIMER_249_112_2_17_23_14_26_71,"CFLTIMER CALLBACK, alloc memory fail!")
TRACE_MSG(CFL_TIMER_416_112_2_17_23_14_27_72,"CFLTIMER GetNodeData, invalid timer node!")
TRACE_MSG(PEERSOCKET_187_112_2_17_23_17_3_73,"udp_callback: The packet is not mine!")
TRACE_MSG(PEERSOCKET_196_112_2_17_23_17_3_74,"udp_callback() data_khk_rec%d")
TRACE_MSG(PEERSOCKET_199_112_2_17_23_17_3_75,"error:recv data_len = NULL")
TRACE_MSG(PEERSOCKET_204_112_2_17_23_17_3_76,"recv data_len = %d")
TRACE_MSG(PEERSOCKET_227_112_2_17_23_17_3_77,"free recv data ptr")
TRACE_MSG(PEERSOCKET_253_112_2_17_23_17_3_78,"fhost = 0x%x")
TRACE_MSG(PEERSOCKET_259_112_2_17_23_17_3_79,"%s")
TRACE_MSG(PEERSOCKET_346_112_2_17_23_17_3_80,"peerSocket:sci_sock_socket() csocket=0x%08x")
TRACE_MSG(PEERSOCKET_360_112_2_17_23_17_3_81,"peerSocket:peerCreate() error:%d")
TRACE_MSG(PEERSOCKET_402_112_2_17_23_17_3_82,"peerSocket.c:peerConnect() socket:0x%08x,ip=%s")
TRACE_MSG(PEERSOCKET_444_112_2_17_23_17_3_83,"peerConnect() ret:%d, ret=%d")
TRACE_MSG(PEERSOCKET_449_112_2_17_23_17_3_84,"peerConnect() error0:%d")
TRACE_MSG(PEERSOCKET_467_112_2_17_23_17_3_85,"peerConnect success")
TRACE_MSG(PEERSOCKET_516_112_2_17_23_17_3_86,"mmiwap.c:peerSend() len=%d,buf=%d")
TRACE_MSG(PEERSOCKET_530_112_2_17_23_17_3_87,"mmiwap.c:peerSend() socket = 0x%08x, s_socketname = 0x%08x")
TRACE_MSG(PEERSOCKET_536_112_2_17_23_17_3_88,"mmiwap.c:peerSend() data_len=%d")
TRACE_MSG(PEERSOCKET_545_112_2_17_23_17_3_89,"mmiwap.c:peerSend() error:%d")
TRACE_MSG(PEERSOCKET_764_112_2_17_23_17_4_90,"mmiwap.c:cmsgethostip() IPAddress:%s")
TRACE_MSG(PEERSOCKET_829_112_2_17_23_17_4_91,"mmiwap.c:cmsclosesocket() in")
TRACE_MSG(PEERSOCKET_832_112_2_17_23_17_4_92,"mmiwap.c:cmsclosesocket() socket != s_socketname")
TRACE_MSG(PEERSOCKET_839_112_2_17_23_17_4_93,"mmiwap.c:cmsclosesocket() ret=%d")
TRACE_MSG(PEERSOCKET_843_112_2_17_23_17_4_94,"mmiwap.c:cmsclosesocket() error:%d")
TRACE_MSG(PEERSOCKET_911_112_2_17_23_17_4_95,"mmiwap.c:cmsgetlasterror() before error=%d")
TRACE_MSG(PEERSOCKET_966_112_2_17_23_17_4_96,"peerSetOpt() SocketId=%ld!")
TRACE_MSG(PEERSOCKET_984_112_2_17_23_17_4_97,"mmiwap.c:cmsbsd_setsockopt() DB_NET_FAIL!")
TRACE_MSG(PEERSOCKET_1012_112_2_17_23_17_4_98,"fhost = 0x%x")
TRACE_MSG(PEERSOCKET_1018_112_2_17_23_17_4_99,"%s")
TRACE_MSG(PEERSOCKET_1057_112_2_17_23_17_4_100,"send abort data")
TRACE_MSG(PEERSOCKET_1063_112_2_17_23_17_4_101,"redirect connect ")
TRACE_MSG(PEERSOCKET_1080_112_2_17_23_17_4_102,"re create socket error!")
TRACE_MSG(PEERSOCKET_1086_112_2_17_23_17_4_103,"Normorl send")
TRACE_MSG(PEERSOCKET_1092_112_2_17_23_17_4_104,"mmiwap.c:Cms_SendPDU() error send failed!!!")
TRACE_MSG(PEERSOCKET_1097_112_2_17_23_17_4_105,"mmiwap.c:Cms_SendPDU() data_khk_send%d")
TRACE_MSG(PEERSOCKET_1115_112_2_17_23_17_4_106,"mmiwap.c:Cms_RecvPDU() data_len=%d")
TRACE_MSG(QF_24_112_2_17_23_17_7_107,"qhsm onAssert__ WINDOWS_PLATFORM")
TRACE_MSG(QF_27_112_2_17_23_17_7_108,"qhsm onAssert__ error")
TRACE_MSG(QHSM_39_112_2_17_23_17_7_109,"qhsm HSM_Init HSM not executed")
TRACE_MSG(QHSM_47_112_2_17_23_17_7_110,"qhsm HSM_Init error first")
TRACE_MSG(QHSM_55_112_2_17_23_17_7_111,"qhsm HSM_Init error second")
TRACE_MSG(QHSM_78_112_2_17_23_17_7_112,"qhsm HSM_TranStatic cannot target top state")
TRACE_MSG(QHSM_85_112_2_17_23_17_7_113,"qhsm HSM_TranStatic error")
TRACE_MSG(QHSM_200_112_2_17_23_17_8_114,"qhsm HsmTranSetup__ inLCA error entry")
TRACE_MSG(QHSM_210_112_2_17_23_17_8_115,"qhsm HsmTranSetup__ inLCA error")
TRACE_MSG(QHSM_225_112_2_17_23_17_8_116,"qhsm HsmTranSetup__ inLCA transition initialized")
TRACE_MSG(QHSMTRAN_15_112_2_17_23_17_8_117,"HSM_TranDyn input error!")
TRACE_MSG(QHSMTRAN_21_112_2_17_23_17_8_118,"qhsm HSM_TranDyn cannot target top state")
TRACE_MSG(QHSMTRAN_28_112_2_17_23_17_8_119,"qhsm HSM_TranDyn error")
TRACE_MSG(QHSMTRAN_97_112_2_17_23_17_8_120,"qhsm HSM_TranDyn error inLCA entry")
TRACE_MSG(QHSMTRAN_107_112_2_17_23_17_8_121,"qhsm HSM_TranDyn error inLCA")
TRACE_MSG(HTTP_API_67_112_2_17_23_17_8_122,"\n http HTTP_InitRequest task id = %u")
TRACE_MSG(HTTP_API_83_112_2_17_23_17_8_123,"\n http HTTP_InitRequest error_code = %d")
TRACE_MSG(HTTP_API_133_112_2_17_23_17_8_124,"\n http HTTP_GetRequest error_code = %d")
TRACE_MSG(HTTP_API_183_112_2_17_23_17_8_125,"\n http HTTP_PostRequest error_code = %d")
TRACE_MSG(HTTP_API_233_112_2_17_23_17_9_126,"\n http HTTP_HeadRequest error_code = %d")
TRACE_MSG(HTTP_API_253_112_2_17_23_17_9_127,"HTTP_AuthResponse request_id =%d")
TRACE_MSG(HTTP_API_286_112_2_17_23_17_9_128,"\n http HTTP_AuthResponse error_code = %d")
TRACE_MSG(HTTP_API_330_112_2_17_23_17_9_129,"\n http HTTP_CloseRequest error_code = %d")
TRACE_MSG(HTTP_API_349_112_2_17_23_17_9_130,"\n http HTTP_CancelRequest request_id =%d")
TRACE_MSG(HTTP_API_379_112_2_17_23_17_9_131,"\n http HTTP_CancelRequest error_code = %d")
TRACE_MSG(HTTP_API_401_112_2_17_23_17_9_132,"HTTP_HeaderResponse request_id =%d")
TRACE_MSG(HTTP_API_440_112_2_17_23_17_9_133,"\n http HTTP_HeaderResponse error_code = %d")
TRACE_MSG(HTTP_API_489_112_2_17_23_17_9_134,"\n http HTTP_CookieSetReq error_code = %d")
TRACE_MSG(HTTP_API_586_112_2_17_23_17_9_135,"\n http HTTP_CookieClear error_code = %d")
TRACE_MSG(HTTP_API_616_112_2_17_23_17_9_136,"\n http HTTP_CacheClear 1 error_code = %d")
TRACE_MSG(HTTP_API_633_112_2_17_23_17_9_137,"\n http HTTP_CacheClear 2 error_code = %d")
TRACE_MSG(HTTP_API_652_112_2_17_23_17_9_138,"\n http HTTP_CacheClear 3 error_code = %d")
TRACE_MSG(HTTP_API_666_112_2_17_23_17_9_139,"\n http HTTP_CacheClear 4 error_code = %d")
TRACE_MSG(HTTP_API_718_112_2_17_23_17_10_140,"HTTP_SslCertResponse request_id =%d")
TRACE_MSG(HTTP_API_748_112_2_17_23_17_10_141,"\n http HTTP_SslCertResponse error_code = %d")
TRACE_MSG(HTTP_AUTH_70_112_2_17_23_17_10_142,"\n http HttpAuthenDigestResponseCreate alloc error")
TRACE_MSG(HTTP_AUTH_87_112_2_17_23_17_10_143,"\n http HttpAuthenDigestResponseDestroy input error \n")
TRACE_MSG(HTTP_AUTH_155_112_2_17_23_17_10_144,"\n http HttpAuthenDigestRequestCreate alloc error \n")
TRACE_MSG(HTTP_AUTH_172_112_2_17_23_17_10_145,"\n http HttpAuthenDigestRequestDestory input error \n")
TRACE_MSG(HTTP_AUTH_280_112_2_17_23_17_10_146,"\n http HttpAuthenBasicEncode input error \n")
TRACE_MSG(HTTP_AUTH_287_112_2_17_23_17_10_147,"\n http HttpAuthenBasicEncode src_ptr alloc error \n")
TRACE_MSG(HTTP_AUTH_292_112_2_17_23_17_10_148,"\n http HttpAuthenBasicEncode src_ptr=%s")
TRACE_MSG(HTTP_AUTH_299_112_2_17_23_17_10_149,"\n http HttpAuthenBasicEncode dst_ptr alloc error \n")
TRACE_MSG(HTTP_AUTH_328_112_2_17_23_17_10_150,"\n http HttpAuthenDigestEncode input error \n")
TRACE_MSG(HTTP_AUTH_334_112_2_17_23_17_10_151,"\n http HttpAuthenDigestEncode alloc error \n")
TRACE_MSG(HTTP_AUTH_341_112_2_17_23_17_10_152,"\n http HttpAuthenDigestEncode HttpAuthenDigestRequsetSetParam error \n")
TRACE_MSG(HTTP_AUTH_381_112_2_17_23_17_11_153,"\n http HttpAuthenDigestCalcHA1 input error \n")
TRACE_MSG(HTTP_AUTH_424_112_2_17_23_17_11_154,"\n http HttpAuthenDigestCalcResult input error \n")
TRACE_MSG(HTTP_AUTH_483_112_2_17_23_17_11_155,"\n http HttpAuthenDigestRequsetSetParam input error \n")
TRACE_MSG(HTTP_AUTH_666_112_2_17_23_17_11_156,"\n http HttpAuthenDigestRequsetSetParam error_code =%d \n")
TRACE_MSG(HTTP_AUTH_720_112_2_17_23_17_11_157,"\n http HttpAuthenDigestResponseParse input error \n")
TRACE_MSG(HTTP_AUTH_726_112_2_17_23_17_11_158,"\n http HttpAuthenDigestResponseParse alloc error \n")
TRACE_MSG(HTTP_AUTH_753_112_2_17_23_17_11_159,"\n http HttpAuthenDigestResponseParse 1 error_code =%d")
TRACE_MSG(HTTP_AUTH_770_112_2_17_23_17_11_160,"\n http HttpAuthenDigestResponseParse error net data \n")
TRACE_MSG(HTTP_AUTH_779_112_2_17_23_17_11_161,"\n http HttpAuthenDigestResponseParse 2 error_code=%d")
TRACE_MSG(HTTP_AUTH_813_112_2_17_23_17_11_162,"\n http HttpAuthenDigestResponseSaveParam invalid input \n")
TRACE_MSG(HTTP_AUTH_858_112_2_17_23_17_11_163,"\n http HttpAuthenDigestResponseSaveParam tail = pnull")
TRACE_MSG(HTTP_AUTH_1010_112_2_17_23_17_12_164,"\n HTTP HttpAuthenDigestResponseSaveParam unknown param \n")
TRACE_MSG(HTTP_AUTH_1017_112_2_17_23_17_12_165,"\n http HttpAuthenDigestResponseSaveParam error_code=%d")
TRACE_MSG(HTTP_AUTH_1025_112_2_17_23_17_12_166,"\n http HttpAuthenDigestResponseSaveParam destroy request_digest_ptr")
TRACE_MSG(HTTP_AUTH_1037_112_2_17_23_17_12_167,"\n http HttpAuthenDigestResponseSaveParam CLEAR error_code=%d")
TRACE_MSG(HTTP_AUTH_1053_112_2_17_23_17_12_168,"\n http HttpAuthenDigestTokenFind input error \n")
TRACE_MSG(HTTP_AUTH_1066_112_2_17_23_17_12_169,"\n http HttpAuthenDigestTokenFind auth map type =%d")
TRACE_MSG(HTTP_AUTH_1070_112_2_17_23_17_12_170,"\n http HttpAuthenDigestTokenFind error \n")
TRACE_MSG(HTTP_AUTH_1089_112_2_17_23_17_12_171,"\n http HttpAuthenGetInfo input error \n")
TRACE_MSG(HTTP_AUTH_1130_112_2_17_23_17_12_172,"\n HTTP HttpAuthenGetInfo unknown scheme\n")
TRACE_MSG(HTTP_AUTH_1163_112_2_17_23_17_12_173,"\n http HttpAuthenCombineResult input error \n")
TRACE_MSG(HTTP_AUTH_1174_112_2_17_23_17_12_174,"\n http HttpAuthenCombineResult alloc error \n")
TRACE_MSG(HTTP_AUTH_1189_112_2_17_23_17_12_175,"\n http HttpAuthenCombineResult 1 input error \n")
TRACE_MSG(HTTP_AUTH_1195_112_2_17_23_17_12_176,"\n http HttpAuthenCombineResult 1 alloc error \n")
TRACE_MSG(HTTP_CACHE_120_112_2_17_23_17_13_177,"\n http HttpSaveDataToCache error_code =%d")
TRACE_MSG(HTTP_CACHE_147_112_2_17_23_17_13_178,"\n http HandleCacheDataToApp 1 input error \n")
TRACE_MSG(HTTP_CACHE_154_112_2_17_23_17_13_179,"\n http HandleCacheDataToApp 2 input error \n")
TRACE_MSG(HTTP_CACHE_170_112_2_17_23_17_13_180,"\n http HandleCacheDataToApp file not found error \n")
TRACE_MSG(HTTP_CACHE_177_112_2_17_23_17_13_181,"\n http HandleCacheDataToApp get file size error \n")
TRACE_MSG(HTTP_CACHE_195_112_2_17_23_17_13_182,"\n http HandleCacheDataToApp alloc error \n")
TRACE_MSG(HTTP_CACHE_205_112_2_17_23_17_13_183,"\n http HandleCacheDataToApp file read error \n")
TRACE_MSG(HTTP_CACHE_217_112_2_17_23_17_13_184,"\n http HandleCacheDataToApp file write error \n")
TRACE_MSG(HTTP_CACHE_253_112_2_17_23_17_13_185,"\n http HandleCacheDataToApp send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_CACHE_263_112_2_17_23_17_13_186,"\n http HandleCacheDataToApp error method\n")
TRACE_MSG(HTTP_CACHE_API_70_112_2_17_23_17_13_187,"\n HttpCacheNodeSetExpires input error \n")
TRACE_MSG(HTTP_CACHE_API_112_112_2_17_23_17_13_188,"\n http HttpCacheInitialize input error \n")
TRACE_MSG(HTTP_CACHE_API_119_112_2_17_23_17_13_189,"\n http HttpCacheInitialize HTTP_ERROR_NO_MEMORY")
TRACE_MSG(HTTP_CACHE_API_125_112_2_17_23_17_13_190,"\n http HttpCacheInitialize HTTP_ERROR_INVALID_INPUT 1")
TRACE_MSG(HTTP_CACHE_API_131_112_2_17_23_17_13_191,"\n http HttpCacheInitialize HTTP_ERROR_INVALID_INPUT 2")
TRACE_MSG(HTTP_CACHE_API_148_112_2_17_23_17_13_192,"\n http HttpCacheInitialize HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_CACHE_API_156_112_2_17_23_17_13_193,"\n http HttpCacheInitialize HTTP_ERROR_FILE_GET_SIZE")
TRACE_MSG(HTTP_CACHE_API_180_112_2_17_23_17_13_194,"\n http HttpCacheInitialize HTTP_ERROR_FILE_READ")
TRACE_MSG(HTTP_CACHE_API_191_112_2_17_23_17_13_195,"\n http HttpCacheInitialize curr count = 0 or curr size = 0")
TRACE_MSG(HTTP_CACHE_API_220_112_2_17_23_17_14_196,"\n http HttpCacheInitialize alloc error \n")
TRACE_MSG(HTTP_CACHE_API_233_112_2_17_23_17_14_197,"\n http HttpCacheInitialize str_token_handle alloc error \n")
TRACE_MSG(HTTP_CACHE_API_285_112_2_17_23_17_14_198,"\n http HttpCacheInitialize HTTP_ERROR_INVALID_INPUT 2")
TRACE_MSG(HTTP_CACHE_API_358_112_2_17_23_17_14_199,"\n http HttpCacheNodeDestroy input error \n")
TRACE_MSG(HTTP_CACHE_API_407_112_2_17_23_17_14_200,"\n http HttpCacheNodeCreate input error\n")
TRACE_MSG(HTTP_CACHE_API_414_112_2_17_23_17_14_201,"\n http HttpCacheNodeCreate 1 alloc error\n")
TRACE_MSG(HTTP_CACHE_API_424_112_2_17_23_17_14_202,"\n http HttpCacheNodeCreate 2 alloc error\n")
TRACE_MSG(HTTP_CACHE_API_457_112_2_17_23_17_14_203,"\n http HttpCacheAppend 1 input error\n")
TRACE_MSG(HTTP_CACHE_API_465_112_2_17_23_17_14_204,"\n http HttpCacheAppend 2 input error\n")
TRACE_MSG(HTTP_CACHE_API_488_112_2_17_23_17_14_205,"\n http HttpCacheAppend HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_CACHE_API_499_112_2_17_23_17_14_206,"\n http HttpCacheAppend HTTP_ERROR_FILE_WRITE")
TRACE_MSG(HTTP_CACHE_API_537_112_2_17_23_17_14_207,"\n http HttpGetCache input error\n")
TRACE_MSG(HTTP_CACHE_API_544_112_2_17_23_17_14_208,"\n http HttpGetCache alloc error\n")
TRACE_MSG(HTTP_CACHE_API_554_112_2_17_23_17_14_209,"n http HttpGetCache md_buf =%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x")
TRACE_MSG(HTTP_CACHE_API_565_112_2_17_23_17_14_210,"\n http HttpGetCache 2 input error\n")
TRACE_MSG(HTTP_CACHE_API_583_112_2_17_23_17_14_211,"\n http HttpGetCache HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_CACHE_API_608_112_2_17_23_17_14_212,"\n http HttpGetCache not get cache node\n")
TRACE_MSG(HTTP_CACHE_API_626_112_2_17_23_17_14_213,"n http HttpCacheFindNodeByName 1 node file name =%s,file_name_ptr =%s")
TRACE_MSG(HTTP_CACHE_API_630_112_2_17_23_17_14_214,"\n http HttpCacheFindNodeByName not find node")
TRACE_MSG(HTTP_CACHE_API_648_112_2_17_23_17_15_215,"\n http HttpCacheDelete 1 input error\n")
TRACE_MSG(HTTP_CACHE_API_656_112_2_17_23_17_15_216,"\n http HttpCacheDelete 2 input error\n")
TRACE_MSG(HTTP_CACHE_API_718_112_2_17_23_17_15_217,"\n http HttpCacheFinalize 1 input error\n")
TRACE_MSG(HTTP_CACHE_API_724_112_2_17_23_17_15_218,"\n http HttpCacheFinalize 2 input error\n")
TRACE_MSG(HTTP_CACHE_API_729_112_2_17_23_17_15_219,"\n http HttpCacheFinalize 3 input error\n")
TRACE_MSG(HTTP_CACHE_API_836_112_2_17_23_17_15_220,"\n http HttpCacheFinalize error_code =%d\n")
TRACE_MSG(HTTP_CACHE_API_872_112_2_17_23_17_15_221,"\n http HttpCacheDeleteLRU 1 input error\n")
TRACE_MSG(HTTP_CACHE_API_883_112_2_17_23_17_15_222,"\n http HttpCacheDeleteLRU 2 input error\n")
TRACE_MSG(HTTP_CACHE_API_919_112_2_17_23_17_15_223,"\n http HttpCacheInsert input error\n")
TRACE_MSG(HTTP_CACHE_API_957_112_2_17_23_17_15_224,"\n http HttpSetCachePath input error\n")
TRACE_MSG(HTTP_CACHE_API_976_112_2_17_23_17_15_225,"\n http HttpSetCacheIndexFile input error\n")
TRACE_MSG(HTTP_CACHE_API_994_112_2_17_23_17_15_226,"\n http HttpCacheSetEnable input error\n")
TRACE_MSG(HTTP_CACHE_API_1031_112_2_17_23_17_15_227,"\n http HttpCacheGetEtag input error\n")
TRACE_MSG(HTTP_CACHE_API_1052_112_2_17_23_17_15_228,"\n http HttpCacheGetLastModified input error\n")
TRACE_MSG(HTTP_CHUNK_72_112_2_17_23_17_16_229,"\n http HttpChunkDecode 1 input error\n")
TRACE_MSG(HTTP_CHUNK_81_112_2_17_23_17_16_230,"\n http HttpChunkDecode 2 input error\n")
TRACE_MSG(HTTP_CHUNK_88_112_2_17_23_17_16_231,"\n http HttpChunkDecode alloc error\n")
TRACE_MSG(HTTP_CHUNK_99_112_2_17_23_17_16_232,"\n http HttpChunkDecode net data error\n")
TRACE_MSG(HTTP_CHUNK_107_112_2_17_23_17_16_233,"\n http HttpChunkDecode free content\n")
TRACE_MSG(HTTP_CHUNK_153_112_2_17_23_17_16_234,"\n http delete timer")
TRACE_MSG(HTTP_CHUNK_159_112_2_17_23_17_16_235,"\n http create timer")
TRACE_MSG(HTTP_CHUNK_188_112_2_17_23_17_16_236,"\n http HttpChunkDecode file write error\n")
TRACE_MSG(HTTP_CHUNK_205_112_2_17_23_17_16_237,"\n http chunk state = %d\n")
TRACE_MSG(HTTP_CHUNK_232_112_2_17_23_17_16_238,"\n http HttpChunkDecode 1 error_code =%d")
TRACE_MSG(HTTP_CHUNK_268_112_2_17_23_17_16_239,"\n http Http_Idle send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_CHUNK_284_112_2_17_23_17_16_240,"\n http HandleHttpContent redirect error \n")
TRACE_MSG(HTTP_CHUNK_323_112_2_17_23_17_16_241,"\n http HttpChunkDecode 2 error_code =%d")
TRACE_MSG(HTTP_CHUNK_355_112_2_17_23_17_16_242,"\n http HttpChunkDecode 3 error_code =%d")
TRACE_MSG(HTTP_CHUNK_492_112_2_17_23_17_17_243,"\n http HttpChunkDecode send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_CHUNK_504_112_2_17_23_17_17_244,"\n HTTP HttpChunkDecode unsupport method\n")
TRACE_MSG(HTTP_CHUNK_523_112_2_17_23_17_17_245,"\n http delete timer")
TRACE_MSG(HTTP_CHUNK_530_112_2_17_23_17_17_246,"\n http create timer")
TRACE_MSG(HTTP_CHUNK_553_112_2_17_23_17_17_247,"\n http HttpChunkParse input error\n")
TRACE_MSG(HTTP_CHUNK_635_112_2_17_23_17_17_248,"\n http HttpChunkParse 1 chunk error stop\n")
TRACE_MSG(HTTP_CHUNK_700_112_2_17_23_17_17_249,"\n http HttpChunkParse bad chunk error\n")
TRACE_MSG(HTTP_CHUNK_711_112_2_17_23_17_17_250,"\n http HttpChunkParse 2 chunk error stop\n")
TRACE_MSG(HTTP_CHUNK_716_112_2_17_23_17_17_251,"\n http HttpChunkParse chunk error state\n")
TRACE_MSG(HTTP_CHUNK_722_112_2_17_23_17_17_252,"\n http HttpChunkParse ok\n")
TRACE_MSG(HTTP_CHUNK_737_112_2_17_23_17_17_253,"\n http HTTPChunkReadExt input error\n")
TRACE_MSG(HTTP_CHUNK_758_112_2_17_23_17_17_254,"\n http HTTPChunkInit input error\n")
TRACE_MSG(HTTP_CHUNK_778_112_2_17_23_17_17_255,"\n http HttpChunkCreate alloc error\n")
TRACE_MSG(HTTP_CHUNK_798_112_2_17_23_17_17_256,"\n http HttpChunkDestroy input error\n")
TRACE_MSG(HTTP_COOKIE_API_144_112_2_17_23_17_18_257,"\n http HttpCookieNodeCreate input error\n")
TRACE_MSG(HTTP_COOKIE_API_151_112_2_17_23_17_18_258,"\n http HttpCookieNodeCreate alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_194_112_2_17_23_17_18_259,"\n http HttpCookieParse 1 alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_207_112_2_17_23_17_18_260,"\n http HttpCookieParse 2 alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_223_112_2_17_23_17_18_261,"\n http HttpCookieParse 3 alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_439_112_2_17_23_17_18_262,"\n http HttpCookieParse cookie_node_ptr=%x\n")
TRACE_MSG(HTTP_COOKIE_API_512_112_2_17_23_17_18_263,"\n http HttpCookiePut input error\n")
TRACE_MSG(HTTP_COOKIE_API_530_112_2_17_23_17_18_264,"HTTP HttpCookiePut find domain %s")
TRACE_MSG(HTTP_COOKIE_API_541_112_2_17_23_17_18_265,"HTTP HttpCookiePut find port %d")
TRACE_MSG(HTTP_COOKIE_API_553_112_2_17_23_17_18_266,"HTTP HttpCookiePut find path %s")
TRACE_MSG(HTTP_COOKIE_API_573_112_2_17_23_17_19_267,"HTTP COOKIE REPLACE name =%s")
TRACE_MSG(HTTP_COOKIE_API_585_112_2_17_23_17_19_268,"http HttpCookiePut value =%s")
TRACE_MSG(HTTP_COOKIE_API_591_112_2_17_23_17_19_269,"http HttpCookiePut other name =%s")
TRACE_MSG(HTTP_COOKIE_API_595_112_2_17_23_17_19_270,"http HttpCookiePut other value =%s")
TRACE_MSG(HTTP_COOKIE_API_616_112_2_17_23_17_19_271,"\n http HttpCookiePut alloc 1 error\n")
TRACE_MSG(HTTP_COOKIE_API_648_112_2_17_23_17_19_272,"\n http HttpCookiePut 2 alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_658_112_2_17_23_17_19_273,"\n http HttpCookiePut 3 alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_685_112_2_17_23_17_19_274,"\n http HttpCookiePut 4 alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_795_112_2_17_23_17_19_275,"\n http HttpCookieInitialize input error\n")
TRACE_MSG(HTTP_COOKIE_API_803_112_2_17_23_17_19_276,"\n http HttpCookieInitialize alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_813_112_2_17_23_17_19_277,"\n http HttpCookieInitialize HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_COOKIE_API_822_112_2_17_23_17_19_278,"\n http HttpCookieInitialize file read error\n")
TRACE_MSG(HTTP_COOKIE_API_1098_112_2_17_23_17_20_279,"\n http HttpCookieFinalize input error\n")
TRACE_MSG(HTTP_COOKIE_API_1115_112_2_17_23_17_20_280,"\n http HttpCookieFinalize alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_1128_112_2_17_23_17_20_281,"\n http HttpCookieFinalize HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_COOKIE_API_1143_112_2_17_23_17_20_282,"\n http HttpCookieFinalize file write 1 error\n")
TRACE_MSG(HTTP_COOKIE_API_1168_112_2_17_23_17_20_283,"\n http HttpCookieFinalize 2 file write error\n")
TRACE_MSG(HTTP_COOKIE_API_1193_112_2_17_23_17_20_284,"\n http HttpCookieFinalize 3 file write error\n")
TRACE_MSG(HTTP_COOKIE_API_1216_112_2_17_23_17_20_285,"\n http HttpCookieFinalize 5 file write error\n")
TRACE_MSG(HTTP_COOKIE_API_1242_112_2_17_23_17_20_286,"\n http HttpCookieFinalize 6 file write error\n")
TRACE_MSG(HTTP_COOKIE_API_1266_112_2_17_23_17_20_287,"\n http HttpCookieFinalize 6 file write error\n")
TRACE_MSG(HTTP_COOKIE_API_1295_112_2_17_23_17_20_288,"\n http HttpCookieFinalize 7 file write error\n")
TRACE_MSG(HTTP_COOKIE_API_1412_112_2_17_23_17_20_289,"\n http HttpCookieSetEnable input error\n")
TRACE_MSG(HTTP_COOKIE_API_1437_112_2_17_23_17_20_290,"\n http HttpSetCookieFile input error\n")
TRACE_MSG(HTTP_COOKIE_API_1478_112_2_17_23_17_20_291,"\n http HttpGetCookie input error or no cookie\n")
TRACE_MSG(HTTP_COOKIE_API_1481_112_2_17_23_17_20_292,"http HttpGetCookie input domain =%s,path =%s,port =%d")
TRACE_MSG(HTTP_COOKIE_API_1493_112_2_17_23_17_20_293,"HttpGetCookie domain =%s,tail =%d,path =%s,port =%d")
TRACE_MSG(HTTP_COOKIE_API_1503_112_2_17_23_17_20_294,"HttpGetCookie domain =%s,tail =%d")
TRACE_MSG(HTTP_COOKIE_API_1513_112_2_17_23_17_20_295,"HttpGetCookie PORT =%d")
TRACE_MSG(HTTP_COOKIE_API_1528_112_2_17_23_17_20_296,"HttpGetCookie path=%s")
TRACE_MSG(HTTP_COOKIE_API_1551_112_2_17_23_17_20_297,"HttpGetCookie 1 domain =%s,tail =%d,path =%s,port =%d")
TRACE_MSG(HTTP_COOKIE_API_1561_112_2_17_23_17_20_298,"HttpGetCookie 1 domain =%s,tail =%d")
TRACE_MSG(HTTP_COOKIE_API_1571_112_2_17_23_17_20_299,"HttpGetCookie 1 PORT =%d")
TRACE_MSG(HTTP_COOKIE_API_1586_112_2_17_23_17_21_300,"HttpGetCookie 1 path=%s")
TRACE_MSG(HTTP_COOKIE_API_1600_112_2_17_23_17_21_301,"\n http HttpGetCookie 1 mark_parent_ptr null\n")
TRACE_MSG(HTTP_COOKIE_API_1607_112_2_17_23_17_21_302,"\n http HttpGetCookie alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_1626_112_2_17_23_17_21_303,"http HttpGetCookie maker temp_str_ptr =%s")
TRACE_MSG(HTTP_COOKIE_API_1650_112_2_17_23_17_21_304,"http HttpGetCookie maker1 temp1_str_ptr =%s")
TRACE_MSG(HTTP_COOKIE_API_1656_112_2_17_23_17_21_305,"http HttpGetCookie maker1 temp1_str_ptr =%s after skip")
TRACE_MSG(HTTP_COOKIE_API_1678_112_2_17_23_17_21_306,"\n http HttpGetCookie temp_str_ptr null\n")
TRACE_MSG(HTTP_COOKIE_API_1695_112_2_17_23_17_21_307,"http HttpGetCookie format_str_ptr =%s,format_str_len = %d")
TRACE_MSG(HTTP_COOKIE_API_1720_112_2_17_23_17_21_308,"\n http HttpCookieFormat input error\n")
TRACE_MSG(HTTP_COOKIE_API_1728_112_2_17_23_17_21_309,"\n http HttpCookieFormat alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_1760_112_2_17_23_17_21_310,"HttpCookieFormat now =%d,expires =%d")
TRACE_MSG(HTTP_COOKIE_API_1766_112_2_17_23_17_21_311,"http HttpCookieFormat buffer 1 =%s,ret =%d")
TRACE_MSG(HTTP_COOKIE_API_1772_112_2_17_23_17_21_312,"http HttpCookieFormat buffer 2 =%s,ret =%d")
TRACE_MSG(HTTP_COOKIE_API_1792_112_2_17_23_17_21_313,"\n http HttpCookieFormat input 1 error\n")
TRACE_MSG(HTTP_COOKIE_API_1811_112_2_17_23_17_21_314,"\n http HttpCookieFormat 1 alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_1824_112_2_17_23_17_21_315,"\n http HttpCookieFormat 1 alloc error\n")
TRACE_MSG(HTTP_COOKIE_API_1835_112_2_17_23_17_21_316,"http HttpCookieFormat cookie_str_ptr=%s")
TRACE_MSG(HTTP_COOKIE_API_1842_112_2_17_23_17_21_317,"http HttpCookieFormat 1 cookie_str_ptr=%s")
TRACE_MSG(HTTP_COOKIE_API_1955_112_2_17_23_17_21_318,"HTTP HttpGetCookieListByJS find domain %s")
TRACE_MSG(HTTP_COOKIE_API_1965_112_2_17_23_17_21_319,"HTTP HttpGetCookieListByJS find port %d")
TRACE_MSG(HTTP_COOKIE_API_2009_112_2_17_23_17_21_320,"HTTP COOKIE delete name =%s")
TRACE_MSG(HTTP_DATE_PARSER_278_112_2_17_23_17_22_321,"http HttpDateParse wday_num =%x")
TRACE_MSG(HTTP_DATE_PARSER_287_112_2_17_23_17_22_322,"http HttpDateParse mon_num =%x")
TRACE_MSG(HTTP_DATE_PARSER_298_112_2_17_23_17_22_323,"http HttpDateParse tz_offset =%x")
TRACE_MSG(HTTP_DATE_PARSER_307_112_2_17_23_17_22_324,"http HttpDateParse 1")
TRACE_MSG(HTTP_DATE_PARSER_376_112_2_17_23_17_22_325,"http HttpDateParse 2")
TRACE_MSG(HTTP_DATE_PARSER_392_112_2_17_23_17_22_326,"http HttpDateParse 3")
TRACE_MSG(HTTP_DATE_PARSER_434_112_2_17_23_17_22_327,"http HttpDateParse 4")
TRACE_MSG(HTTP_EVENT_38_112_2_17_23_17_23_328,"\n http HttpEventQueueInit input error\n")
TRACE_MSG(HTTP_EVENT_58_112_2_17_23_17_23_329,"\n http HttpEventQueueInsert input error\n")
TRACE_MSG(HTTP_EVENT_65_112_2_17_23_17_23_330,"\n http HttpEventQueueInsert alloc error\n")
TRACE_MSG(HTTP_EVENT_102_112_2_17_23_17_23_331,"\n http HttpEventQueueFetch input error or null\n")
TRACE_MSG(HTTP_EVENT_152_112_2_17_23_17_23_332,"\n http HttpEventQueueClear input error\n")
TRACE_MSG(HTTP_HEADER_42_112_2_17_23_17_23_333,"\n http HttpSetAcceptHeader input error\n")
TRACE_MSG(HTTP_HEADER_49_112_2_17_23_17_23_334,"\n http HttpSetAcceptHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_78_112_2_17_23_17_23_335,"\n http HttpSetAcceptCharsetHeader input error\n")
TRACE_MSG(HTTP_HEADER_85_112_2_17_23_17_23_336,"\n http HttpSetAcceptCharsetHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_114_112_2_17_23_17_23_337,"\n http HttpSetAcceptEncodingHeader input error\n")
TRACE_MSG(HTTP_HEADER_121_112_2_17_23_17_23_338,"\n http HttpSetAcceptEncodingHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_150_112_2_17_23_17_23_339,"\n http HttpSetAcceptLanguageHeader input error\n")
TRACE_MSG(HTTP_HEADER_157_112_2_17_23_17_23_340,"\n http HttpSetAcceptLanguageHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_186_112_2_17_23_17_24_341,"\n http HttpSetConnectionHeader input error\n")
TRACE_MSG(HTTP_HEADER_193_112_2_17_23_17_24_342,"\n http HttpSetConnectionHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_222_112_2_17_23_17_24_343,"\n http HttpSetRefererHeader input error\n")
TRACE_MSG(HTTP_HEADER_229_112_2_17_23_17_24_344,"\n http HttpSetRefererHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_258_112_2_17_23_17_24_345,"\n http HttpSetUserAgentHeader input error\n")
TRACE_MSG(HTTP_HEADER_265_112_2_17_23_17_24_346,"\n http HttpSetUserAgentHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_294_112_2_17_23_17_24_347,"\n http HttpSetContentTypeHeader input error\n")
TRACE_MSG(HTTP_HEADER_301_112_2_17_23_17_24_348,"\n http HttpSetContentTypeHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_330_112_2_17_23_17_24_349,"\n http HttpSetUAProfileHeader input error\n")
TRACE_MSG(HTTP_HEADER_337_112_2_17_23_17_24_350,"\n http HttpSetUAProfileHeader alloc error\n")
TRACE_MSG(HTTP_HEADER_366_112_2_17_23_17_24_351,"\n http HttpSetHeaderField input error\n")
TRACE_MSG(HTTP_HEADER_369_112_2_17_23_17_24_352,"\n http HttpSetHeaderField token=%x,value=%x")
TRACE_MSG(HTTP_HEADER_374_112_2_17_23_17_24_353,"\n http HttpSetHeaderField alloc error\n")
TRACE_MSG(HTTP_HEADER_397_112_2_17_23_17_24_354,"\n http HttpDestroyHeaderList input error\n")
TRACE_MSG(HTTP_HEADER_429_112_2_17_23_17_24_355,"\n http HttpRequestGetHeaderField input error\n")
TRACE_MSG(HTTP_HEADER_446_112_2_17_23_17_24_356,"\n http HttpRequestGetHeaderField alloc error\n")
TRACE_MSG(HTTP_HSM_513_112_2_17_23_17_25_357,"\n http HTTPHSM_Create host_handle =%x\n")
TRACE_MSG(HTTP_HSM_516_112_2_17_23_17_25_358,"\n http HTTPHSM_Create input error\n")
TRACE_MSG(HTTP_HSM_523_112_2_17_23_17_25_359,"\n http HTTPHSM_Create 1 alloc error\n")
TRACE_MSG(HTTP_HSM_534_112_2_17_23_17_25_360,"\n http HTTPHSM_Create 2 alloc error\n")
TRACE_MSG(HTTP_HSM_545_112_2_17_23_17_25_361,"\n http session 1 = %d\n")
TRACE_MSG(HTTP_HSM_552_112_2_17_23_17_25_362,"\n http HTTPHSM_Create http_hsm_ptr = %d")
TRACE_MSG(HTTP_HSM_604_112_2_17_23_17_25_363,"\n http HTTPHSM_Destroy hsm_ptr = %d")
TRACE_MSG(HTTP_HSM_633_112_2_17_23_17_26_364,"\n http HTTPHSM_DispatchSignal input error\n")
TRACE_MSG(HTTP_HSM_636_112_2_17_23_17_26_365,"\n http HTTPHSM_DispatchSignal hsm_ptr=%x,signal_code=%d\n")
TRACE_MSG(HTTP_HSM_682_112_2_17_23_17_26_366,"\n http HTTPHSM_DealRequest input error\n")
TRACE_MSG(HTTP_HSM_693_112_2_17_23_17_26_367,"\n http HTTPHSM_DealRequest new socket for connect method\n")
TRACE_MSG(HTTP_HSM_710_112_2_17_23_17_26_368,"\n http HTTPHSM_DealRequest request_ptr=%x,flag = FALSE\n")
TRACE_MSG(HTTP_HSM_714_112_2_17_23_17_26_369,"\n http HTTPHSM_DealRequest 1 input error\n")
TRACE_MSG(HTTP_HSM_832_112_2_17_23_17_26_370,"http HttpSaveDataToFile create file error!")
TRACE_MSG(HTTP_HSM_864_112_2_17_23_17_26_371,"\n http HTTP_TransCallback input error\n")
TRACE_MSG(HTTP_HSM_871_112_2_17_23_17_26_372,"\n http HTTP_TransCallback recv NETTRANS_CONNECT_IND\n")
TRACE_MSG(HTTP_HSM_876_112_2_17_23_17_26_373,"\n http HTTP_TransCallback recv NETTRANS_CONNECT_IND succ\n")
TRACE_MSG(HTTP_HSM_883_112_2_17_23_17_26_374,"\n http HTTP_TransCallback recv NETTRANS_CONNECT_IND timeout\n")
TRACE_MSG(HTTP_HSM_889_112_2_17_23_17_26_375,"\n http HTTP_TransCallback recv NETTRANS_CONNECT_IND fail\n")
TRACE_MSG(HTTP_HSM_894_112_2_17_23_17_26_376,"\n http HTTP_TransCallback recv NETTRANS_DATA_IN_IND\n")
TRACE_MSG(HTTP_HSM_899_112_2_17_23_17_26_377,"\n http HTTP_TransCallback recv NETTRANS_DATA_IN_IND succ\n")
TRACE_MSG(HTTP_HSM_903_112_2_17_23_17_26_378,"\n http TransCallback recv NETMGR_DATA_IN_IND error\n")
TRACE_MSG(HTTP_HSM_907_112_2_17_23_17_26_379,"\n http HTTP_TransCallback recv NETTRANS_DATA_PROGRESS_IN_IND\n")
TRACE_MSG(HTTP_HSM_912_112_2_17_23_17_26_380,"\n http HTTP_TransCallback recv NETTRANS_DATA_PROGRESS_IN_IND succ\n")
TRACE_MSG(HTTP_HSM_916_112_2_17_23_17_26_381,"\n http TransCallback recv NETTRANS_DATA_PROGRESS_IN_IND error\n")
TRACE_MSG(HTTP_HSM_920_112_2_17_23_17_26_382,"\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND\n")
TRACE_MSG(HTTP_HSM_925_112_2_17_23_17_26_383,"\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND succ\n")
TRACE_MSG(HTTP_HSM_934_112_2_17_23_17_26_384,"\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND succ len < 0\n")
TRACE_MSG(HTTP_HSM_945_112_2_17_23_17_26_385,"\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND fail\n")
TRACE_MSG(HTTP_HSM_951_112_2_17_23_17_26_386,"\n http HTTP_TransCallback recv NETTRANS_SEND_RETURN_IND timeout\n")
TRACE_MSG(HTTP_HSM_955_112_2_17_23_17_26_387,"\n http TransCallback recv NETMGR_SEND_RETURN_IND error\n")
TRACE_MSG(HTTP_HSM_960_112_2_17_23_17_26_388,"\n http HTTP_TransCallback recv NETTRANS_RECV_RETURN_IND\n")
TRACE_MSG(HTTP_HSM_965_112_2_17_23_17_26_389,"\n http HTTP_TransCallback recv NETTRANS_RECV_RETURN_IND succ\n")
TRACE_MSG(HTTP_HSM_974_112_2_17_23_17_26_390,"\n http HTTP_TransCallback recv NETTRANS_RECV_RETURN_IND succ len < 0\n")
TRACE_MSG(HTTP_HSM_985_112_2_17_23_17_26_391,"\n http HTTP_TransCallback recv NETTRANS_RECV_RETURN_IND fail\n")
TRACE_MSG(HTTP_HSM_989_112_2_17_23_17_26_392,"\n http TransCallback recv NETMGR_RECV_RETURN_IND error\n")
TRACE_MSG(HTTP_HSM_996_112_2_17_23_17_26_393,"\n http HTTP_TransCallback recv NETTRANS_NORMAL_DISCONNECT_IND\n")
TRACE_MSG(HTTP_HSM_1000_112_2_17_23_17_27_394,"\n http HTTP_TransCallback recv NETTRANS_EXCEPTION_DISCONNECT_IND\n")
TRACE_MSG(HTTP_HSM_1004_112_2_17_23_17_27_395,"\n http TransCallback default error\n")
TRACE_MSG(HTTP_HSM_1055_112_2_17_23_17_27_396,"\n HTTP QHSM_Http receive time out signal \n")
TRACE_MSG(HTTP_HSM_1059_112_2_17_23_17_27_397,"\n http delete timer")
TRACE_MSG(HTTP_HSM_1065_112_2_17_23_17_27_398,"\n http delete progress timer ")
TRACE_MSG(HTTP_HSM_1091_112_2_17_23_17_27_399,"\n http QHSM_Http HTTP_TIMEOUT HttpCacheDelete fail")
TRACE_MSG(HTTP_HSM_1099_112_2_17_23_17_27_400,"\n HTTP QHSM_Http receive server exception close signal \n")
TRACE_MSG(HTTP_HSM_1103_112_2_17_23_17_27_401,"\n http delete timer")
TRACE_MSG(HTTP_HSM_1109_112_2_17_23_17_27_402,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_1136_112_2_17_23_17_27_403,"\n http QHSM_Http HTTP_SERVER_EXCEPTION_CLOSE HttpCacheDelete fail")
TRACE_MSG(HTTP_HSM_1145_112_2_17_23_17_27_404,"\n HTTP QHSM_Http receive cancel signal \n")
TRACE_MSG(HTTP_HSM_1149_112_2_17_23_17_27_405,"\n http delete timer")
TRACE_MSG(HTTP_HSM_1155_112_2_17_23_17_27_406,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_1201_112_2_17_23_17_27_407,"\n http QHSM_Http HTTP_SIG_CANCEL_REQ HttpCacheDelete fail")
TRACE_MSG(HTTP_HSM_1210_112_2_17_23_17_27_408,"\n HTTP QHSM_Http receive error signal \n")
TRACE_MSG(HTTP_HSM_1214_112_2_17_23_17_27_409,"\n http delete timer")
TRACE_MSG(HTTP_HSM_1220_112_2_17_23_17_27_410,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_1246_112_2_17_23_17_27_411,"\n http QHSM_Http HTTP_ERROR HttpCacheDelete fail")
TRACE_MSG(HTTP_HSM_1258_112_2_17_23_17_27_412,"\n http delete timer")
TRACE_MSG(HTTP_HSM_1264_112_2_17_23_17_27_413,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_1272_112_2_17_23_17_27_414,"\n HTTP QHSM_Http receive ssl cancel signal \n")
TRACE_MSG(HTTP_HSM_1276_112_2_17_23_17_27_415,"\n http delete timer")
TRACE_MSG(HTTP_HSM_1282_112_2_17_23_17_27_416,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_1308_112_2_17_23_17_27_417,"\n http QHSM_Http HTTP_SSL_CANCEL HttpCacheDelete fail")
TRACE_MSG(HTTP_HSM_1373_112_2_17_23_17_27_418,"\n HTTP Http_Idle receive connect server signal \n")
TRACE_MSG(HTTP_HSM_1389_112_2_17_23_17_27_419,"\n HTTP QHSM_Http receive server close signal \n")
TRACE_MSG(HTTP_HSM_1458_112_2_17_23_17_28_420,"\n http Http_Idle send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_HSM_1474_112_2_17_23_17_28_421,"\n http Http_Idle redirect error \n")
TRACE_MSG(HTTP_HSM_1684_112_2_17_23_17_28_422,"\n http Http_Idle send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_HSM_1719_112_2_17_23_17_28_423,"\n http Http_Idle HTTP_ERROR_SERVER_CLOSE HttpCacheDelete fail")
TRACE_MSG(HTTP_HSM_1728_112_2_17_23_17_28_424,"\n http Http_Idle HTTP_SERVER_CLOSE")
TRACE_MSG(HTTP_HSM_1881_112_2_17_23_17_28_425,"\n HTTP handle cache error")
TRACE_MSG(HTTP_HSM_1901_112_2_17_23_17_28_426,"\n HTTP Http_Connecting receive connect succ signal \n")
TRACE_MSG(HTTP_HSM_1915_112_2_17_23_17_29_427,"\n HTTP Http_Connecting receive connect fail or server close signal \n")
TRACE_MSG(HTTP_HSM_1961_112_2_17_23_17_29_428,"\n HTTP Http_Connected receive handshake signal \n")
TRACE_MSG(HTTP_HSM_1974_112_2_17_23_17_29_429,"\n HTTP Http_Connecting receive proxy connect signal \n")
TRACE_MSG(HTTP_HSM_1987_112_2_17_23_17_29_430,"\n HTTP Http_Connecting receive send data signal \n")
TRACE_MSG(HTTP_HSM_2040_112_2_17_23_17_29_431,"\n http delete handshake timer")
TRACE_MSG(HTTP_HSM_2047_112_2_17_23_17_29_432,"\n http create handshake timer")
TRACE_MSG(HTTP_HSM_2054_112_2_17_23_17_29_433,"\n http delete handshake timer")
TRACE_MSG(HTTP_HSM_2062_112_2_17_23_17_29_434,"\n http delete handshake timer")
TRACE_MSG(HTTP_HSM_2069_112_2_17_23_17_29_435,"\n HTTP Http_Handshaking receive read data signal \n")
TRACE_MSG(HTTP_HSM_2078_112_2_17_23_17_29_436,"\n HTTP Http_Handshaking receive ssl cert ind signal \n")
TRACE_MSG(HTTP_HSM_2082_112_2_17_23_17_29_437,"\n http delete handshake timer")
TRACE_MSG(HTTP_HSM_2093_112_2_17_23_17_29_438,"\n HTTP Http_Handshaking receive ssl cert rsp signal \n")
TRACE_MSG(HTTP_HSM_2097_112_2_17_23_17_29_439,"\n http delete handshake timer")
TRACE_MSG(HTTP_HSM_2104_112_2_17_23_17_29_440,"\n http create handshake timer")
TRACE_MSG(HTTP_HSM_2117_112_2_17_23_17_29_441,"\n HTTP Http_Handshaking receive handshake succ signal \n")
TRACE_MSG(HTTP_HSM_2130_112_2_17_23_17_29_442,"\n HTTP Http_Handshaking receive handshake fail signal \n")
TRACE_MSG(HTTP_HSM_2139_112_2_17_23_17_29_443,"\n HTTP Http_Handshaking receive read succ signal \n")
TRACE_MSG(HTTP_HSM_2148_112_2_17_23_17_29_444,"\n HTTP Http_Handshaking receive read fail signal \n")
TRACE_MSG(HTTP_HSM_2157_112_2_17_23_17_29_445,"\n HTTP Http_Handshaking receive send succ signal \n")
TRACE_MSG(HTTP_HSM_2167_112_2_17_23_17_29_446,"\n HTTP Http_Handshaking receive HTTP_SERVER_CLOSE signal \n")
TRACE_MSG(HTTP_HSM_2180_112_2_17_23_17_29_447,"\n HTTP Http_Handshaking error handle request=%d \n")
TRACE_MSG(HTTP_HSM_2183_112_2_17_23_17_29_448,"\n HTTP Http_Handshaking not in idle \n")
TRACE_MSG(HTTP_HSM_2189_112_2_17_23_17_29_449,"\n http error handle delete handshake timer")
TRACE_MSG(HTTP_HSM_2194_112_2_17_23_17_29_450,"\n HTTP Http_Handshaking request \n")
TRACE_MSG(HTTP_HSM_2223_112_2_17_23_17_29_451,"\n http Http_ProxyHandshake delete handshake timer")
TRACE_MSG(HTTP_HSM_2230_112_2_17_23_17_29_452,"\n http Http_ProxyHandshake create handshake timer")
TRACE_MSG(HTTP_HSM_2237_112_2_17_23_17_29_453,"\n http Http_ProxyHandshake delete handshake timer")
TRACE_MSG(HTTP_HSM_2245_112_2_17_23_17_29_454,"\n http Http_ProxyHandshake delete handshake timer")
TRACE_MSG(HTTP_HSM_2252_112_2_17_23_17_29_455,"\n HTTP Http_ProxyHandshake receive read data signal \n")
TRACE_MSG(HTTP_HSM_2261_112_2_17_23_17_29_456,"\n HTTP Http_ProxyHandshake receive proxy connect succ signal \n")
TRACE_MSG(HTTP_HSM_2278_112_2_17_23_17_29_457,"\n HTTP Http_ProxyHandshake receive proxy connect fail signal \n")
TRACE_MSG(HTTP_HSM_2287_112_2_17_23_17_29_458,"\n HTTP Http_ProxyHandshake receive read succ signal \n")
TRACE_MSG(HTTP_HSM_2296_112_2_17_23_17_29_459,"\n HTTP Http_ProxyHandshake receive read fail signal \n")
TRACE_MSG(HTTP_HSM_2349_112_2_17_23_17_29_460,"\n http Http_ProxyHandshake error handle delete handshake timer")
TRACE_MSG(HTTP_HSM_2379_112_2_17_23_17_29_461,"\n HTTP Http_Sending receive send succ signal \n")
TRACE_MSG(HTTP_HSM_2388_112_2_17_23_17_29_462,"\n HTTP Http_Sending receive send fail signal \n")
TRACE_MSG(HTTP_HSM_2397_112_2_17_23_17_29_463,"\n HTTP Http_Sending receive ssl send end signal \n")
TRACE_MSG(HTTP_HSM_2406_112_2_17_23_17_29_464,"\n HTTP Http_Sending receive HTTP_SERVER_CLOSE signal \n")
TRACE_MSG(HTTP_HSM_2501_112_2_17_23_17_30_465,"\n http delete timer")
TRACE_MSG(HTTP_HSM_2508_112_2_17_23_17_30_466,"\n http create timer")
TRACE_MSG(HTTP_HSM_2513_112_2_17_23_17_30_467,"\n HTTP Http_ReceivingRecv receive read data signal \n")
TRACE_MSG(HTTP_HSM_2518_112_2_17_23_17_30_468,"\n http delete timer")
TRACE_MSG(HTTP_HSM_2548_112_2_17_23_17_30_469,"\n http delete recv progress timer")
TRACE_MSG(HTTP_HSM_2562_112_2_17_23_17_30_470,"\n HTTP Http_ReceivingRecv receive read succ signal \n")
TRACE_MSG(HTTP_HSM_2572_112_2_17_23_17_30_471,"HTTP Http_ReceivingRecv read success flag = true")
TRACE_MSG(HTTP_HSM_2577_112_2_17_23_17_30_472,"\n HTTP Http_ReceivingRecv receive read fail signal \n")
TRACE_MSG(HTTP_HSM_2581_112_2_17_23_17_30_473,"\n http delete timer")
TRACE_MSG(HTTP_HSM_2587_112_2_17_23_17_30_474,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_2606_112_2_17_23_17_30_475,"\n HTTP Http_ReceivingRecv receive time out signal \n")
TRACE_MSG(HTTP_HSM_2610_112_2_17_23_17_30_476,"\n http delete timer")
TRACE_MSG(HTTP_HSM_2616_112_2_17_23_17_30_477,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_2637_112_2_17_23_17_30_478,"HTTP Http_ReceivingRecv HTTP_SERVER_CLOSE")
TRACE_MSG(HTTP_HSM_2640_112_2_17_23_17_30_479,"HTTP Http_ReceivingRecv read event = %d")
TRACE_MSG(HTTP_HSM_2643_112_2_17_23_17_30_480,"HTTP Http_ReceivingRecv read succ is in idle=%d")
TRACE_MSG(HTTP_HSM_2677_112_2_17_23_17_30_481,"\n http delete timer")
TRACE_MSG(HTTP_HSM_2683_112_2_17_23_17_30_482,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_2702_112_2_17_23_17_30_483,"http Http_ReceivingRecv cache delete")
TRACE_MSG(HTTP_HSM_2730_112_2_17_23_17_30_484,"\n HTTP Http_ReceivingAuthPending receive need auth rsp signal \n")
TRACE_MSG(HTTP_HSM_2748_112_2_17_23_17_30_485,"\n HTTP Http_ReceivingAuthPending recieve server close signal \n")
TRACE_MSG(HTTP_HSM_2805_112_2_17_23_17_30_486,"\n HTTP Http_ReceivingHeaderRspPending receive read data signal \n")
TRACE_MSG(HTTP_HSM_2822_112_2_17_23_17_30_487,"\n HTTP Http_ReceivingHeaderRspPending receive server close signal \n")
TRACE_MSG(HTTP_HSM_2841_112_2_17_23_17_30_488,"\n HTTP Http_ReceivingHeaderRspPending receive header rsp signal \n")
TRACE_MSG(HTTP_HSM_2892_112_2_17_23_17_30_489,"\n http Http_ReceivingHeaderRspPending no read and close flag\n")
TRACE_MSG(HTTP_HSM_2919_112_2_17_23_17_30_490,"http Http_ReceivingHeaderRspPending cache delete")
TRACE_MSG(HTTP_HSM_2943_112_2_17_23_17_31_491,"\n http HandleIdleIdleReq input error\n")
TRACE_MSG(HTTP_HSM_2947_112_2_17_23_17_31_492,"\n http HandleIdleIdleReq request_ptr=%x\n")
TRACE_MSG(HTTP_HSM_2957_112_2_17_23_17_31_493,"HTTP HandleIdleIdleReq uri =%s")
TRACE_MSG(HTTP_HSM_2963_112_2_17_23_17_31_494,"\n http HandleIdleIdleReq 1 input error\n")
TRACE_MSG(HTTP_HSM_2975_112_2_17_23_17_31_495,"\n http HandleIdleIdleReq 2 input error\n")
TRACE_MSG(HTTP_HSM_3004_112_2_17_23_17_31_496,"\n http HandleIdleIdleReq file open error\n")
TRACE_MSG(HTTP_HSM_3058_112_2_17_23_17_31_497,"\n http HandleIdleIdleReq error code = %d\n")
TRACE_MSG(HTTP_HSM_3104_112_2_17_23_17_31_498,"\n http HandleCacheDataToApp 1 input error \n")
TRACE_MSG(HTTP_HSM_3113_112_2_17_23_17_31_499,"\n http HandleCacheDataToApp 2 input error \n")
TRACE_MSG(HTTP_HSM_3129_112_2_17_23_17_31_500,"\n http HandleCacheDataToApp file not found error \n")
TRACE_MSG(HTTP_HSM_3138_112_2_17_23_17_31_501,"\n http HandleCacheDataToApp get file size error \n")
TRACE_MSG(HTTP_HSM_3146_112_2_17_23_17_31_502,"\n http HandleCacheDataToApp get file size error \n")
TRACE_MSG(HTTP_HSM_3165_112_2_17_23_17_31_503,"\n http HandleCacheDataToApp alloc error \n")
TRACE_MSG(HTTP_HSM_3178_112_2_17_23_17_31_504,"\n http HandleCacheDataToApp file read error \n")
TRACE_MSG(HTTP_HSM_3203_112_2_17_23_17_31_505,"\n http HandleIdleCacheReadSucc free content\n")
TRACE_MSG(HTTP_HSM_3215_112_2_17_23_17_31_506,"\n http HandleIdleCacheReadSucc content_ptr =%d\n")
TRACE_MSG(HTTP_HSM_3225_112_2_17_23_17_31_507,"\n http HandleIdleCacheReadSucc response_ptr->content_total_len=%d")
TRACE_MSG(HTTP_HSM_3319_112_2_17_23_17_31_508,"\n http HandleHttpCacheData parse header fail")
TRACE_MSG(HTTP_HSM_3322_112_2_17_23_17_31_509,"\n HTTP HandleHttpCacheData cache size = %d,header len =%d")
TRACE_MSG(HTTP_HSM_3332_112_2_17_23_17_31_510,"\nHTTP HandleHttpCacheData cache data error")
TRACE_MSG(HTTP_HSM_3340_112_2_17_23_17_31_511,"\n http HandleHttpCacheData state code=%d")
TRACE_MSG(HTTP_HSM_3347_112_2_17_23_17_31_512,"\nHTTP HandleHttpCacheData cache data error")
TRACE_MSG(HTTP_HSM_3366_112_2_17_23_17_31_513,"\n http HandleHttpRecvData HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_HSM_3374_112_2_17_23_17_31_514,"\n http HandleHttpRecvData HTTP_ERROR_FILE_WRITE")
TRACE_MSG(HTTP_HSM_3420_112_2_17_23_17_31_515,"\n http HandleHttpRecvData HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_HSM_3428_112_2_17_23_17_31_516,"\n http HandleHttpRecvData HTTP_ERROR_FILE_WRITE")
TRACE_MSG(HTTP_HSM_3487_112_2_17_23_17_32_517,"\n http HandleHttpRecvData error_code=%d")
TRACE_MSG(HTTP_HSM_3584_112_2_17_23_17_32_518,"\n http HandleReceivingHeaderRspPendingHeaderRsp HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_HSM_3649_112_2_17_23_17_32_519,"\n http HandleHttpContent content len = 0")
TRACE_MSG(HTTP_HSM_3664_112_2_17_23_17_32_520,"HTTP HandleHttpCacheContent sfs_error_code = %d")
TRACE_MSG(HTTP_HSM_3672_112_2_17_23_17_32_521,"\n http HandleHttpContent file write error\n")
TRACE_MSG(HTTP_HSM_3688_112_2_17_23_17_32_522,"HTTP HandleHttpCacheContent free content")
TRACE_MSG(HTTP_HSM_3693_112_2_17_23_17_32_523,"HTTP HandleHttpCacheContent response_ptr->rsp_header_info.content_length = %d,response_ptr->content_total_len = %d")
TRACE_MSG(HTTP_HSM_3713_112_2_17_23_17_32_524,"\n http HandleHttpContent send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_HSM_3988_112_2_17_23_17_33_525,"\n HTTP HandleConnectedProxyConn port = %d")
TRACE_MSG(HTTP_HSM_3995_112_2_17_23_17_33_526,"\n HTTP HandleConnectedProxyConn ssl port")
TRACE_MSG(HTTP_HSM_4280_112_2_17_23_17_33_527,"\n http delete timer")
TRACE_MSG(HTTP_HSM_4286_112_2_17_23_17_33_528,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_4355_112_2_17_23_17_33_529,"\n http delete timer")
TRACE_MSG(HTTP_HSM_4361_112_2_17_23_17_33_530,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_4396_112_2_17_23_17_33_531,"\n http delete timer")
TRACE_MSG(HTTP_HSM_4402_112_2_17_23_17_33_532,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_4486_112_2_17_23_17_34_533,"HTTP proxy connect no memory len = %d")
TRACE_MSG(HTTP_HSM_4509_112_2_17_23_17_34_534,"HTTP proxy connect no memory 1 len=%d")
TRACE_MSG(HTTP_HSM_4523_112_2_17_23_17_34_535,"HTTP connect header received")
TRACE_MSG(HTTP_HSM_4528_112_2_17_23_17_34_536,"HTTP connect header not received all")
TRACE_MSG(HTTP_HSM_4538_112_2_17_23_17_34_537,"HTTP proxy connect succ")
TRACE_MSG(HTTP_HSM_4640_112_2_17_23_17_34_538,"\n http delete timer")
TRACE_MSG(HTTP_HSM_4646_112_2_17_23_17_34_539,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_4688_112_2_17_23_17_34_540,"\n http delete timer")
TRACE_MSG(HTTP_HSM_4694_112_2_17_23_17_34_541,"\n http delete progress timer")
TRACE_MSG(HTTP_HSM_4783_112_2_17_23_17_34_542,"HTTP HandleSendingSendSucc trans Http_ReceivingRecv")
TRACE_MSG(HTTP_HSM_4881_112_2_17_23_17_34_543,"HTTP HandleReceivingRecvReadSucc free content 1")
TRACE_MSG(HTTP_HSM_4900_112_2_17_23_17_34_544,"HTTP HandleReceivingRecvReadSucc content_ptr = %d,buf_len =%d")
TRACE_MSG(HTTP_HSM_4916_112_2_17_23_17_34_545,"\n http HandleReceivingRecvReadSucc response_ptr->content_total_len=%d")
TRACE_MSG(HTTP_HSM_5019_112_2_17_23_17_35_546,"HandleHttpRecvData input invalid")
TRACE_MSG(HTTP_HSM_5064_112_2_17_23_17_35_547,"\n http HandleHttpRecvData state code=%d disconnect socket")
TRACE_MSG(HTTP_HSM_5068_112_2_17_23_17_35_548,"\n http HandleHttpRecvData state code=%d")
TRACE_MSG(HTTP_HSM_5075_112_2_17_23_17_35_549,"\n http delete timer")
TRACE_MSG(HTTP_HSM_5081_112_2_17_23_17_35_550,"\n http create timer")
TRACE_MSG(HTTP_HSM_5101_112_2_17_23_17_35_551,"\n http HandleHttpRecvData HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_HSM_5109_112_2_17_23_17_35_552,"\n http HandleHttpRecvData HTTP_ERROR_FILE_WRITE")
TRACE_MSG(HTTP_HSM_5181_112_2_17_23_17_35_553,"\n http Http_Idle send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_HSM_5196_112_2_17_23_17_35_554,"\n http HandleHttpRecvData redirect")
TRACE_MSG(HTTP_HSM_5290_112_2_17_23_17_35_555,"\n http HandleHttpRecvData HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_HSM_5298_112_2_17_23_17_35_556,"\n http HandleHttpRecvData HTTP_ERROR_FILE_WRITE")
TRACE_MSG(HTTP_HSM_5342_112_2_17_23_17_35_557,"\n http HandleHttpRecvData HTTP_RESPONSE_CODE_NOT_MODIFIED error_code=%d")
TRACE_MSG(HTTP_HSM_5391_112_2_17_23_17_35_558,"\n http HandleHttpRecvData error_code=%d")
TRACE_MSG(HTTP_HSM_5418_112_2_17_23_17_35_559,"http HandleHttpContent input invalid")
TRACE_MSG(HTTP_HSM_5447_112_2_17_23_17_35_560,"\n http HandleHttpContent content len = 0")
TRACE_MSG(HTTP_HSM_5456_112_2_17_23_17_35_561,"\n http HandleHttpContent HTTP_TRANSFER_ENCODING_CHUNKED chunk decode error_code=%d")
TRACE_MSG(HTTP_HSM_5467_112_2_17_23_17_35_562,"HTTP HandleHttpContent free content 1")
TRACE_MSG(HTTP_HSM_5470_112_2_17_23_17_35_563,"http HandleHttpContent HTTP_ALLOC no memory")
TRACE_MSG(HTTP_HSM_5483_112_2_17_23_17_36_564,"HTTP HandleHttpContent free content 2")
TRACE_MSG(HTTP_HSM_5488_112_2_17_23_17_36_565,"http HandleHttpContent CFL_ReAlloc no memory")
TRACE_MSG(HTTP_HSM_5501_112_2_17_23_17_36_566,"HTTP HandleHttpContent free content 3")
TRACE_MSG(HTTP_HSM_5510_112_2_17_23_17_36_567,"HandleHttpContent(1) http delete timer")
TRACE_MSG(HTTP_HSM_5515_112_2_17_23_17_36_568," HandleHttpContent(1) http create timer")
TRACE_MSG(HTTP_HSM_5531_112_2_17_23_17_36_569,"HTTP HandleHttpContent free content 4")
TRACE_MSG(HTTP_HSM_5534_112_2_17_23_17_36_570,"http HandleHttpContent HTTP_ALLOC no memory")
TRACE_MSG(HTTP_HSM_5547_112_2_17_23_17_36_571,"HandleHttpContent style=%d")
TRACE_MSG(HTTP_HSM_5562_112_2_17_23_17_36_572,"\n http HandleHttpContent file write error\n")
TRACE_MSG(HTTP_HSM_5589_112_2_17_23_17_36_573,"n http HandleHttpContent response_ptr->rsp_header_info.content_length=%d,response_ptr->content_total_len=%d")
TRACE_MSG(HTTP_HSM_5617_112_2_17_23_17_36_574,"\n http HandleHttpContent request_ptr->net_request_handle = %d,request_ptr->uri_str_ptr =%s\n")
TRACE_MSG(HTTP_HSM_5654_112_2_17_23_17_36_575,"\n http Http_Idle send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_HSM_5671_112_2_17_23_17_36_576,"\n http HandleHttpContent redirect error \n")
TRACE_MSG(HTTP_HSM_5678_112_2_17_23_17_36_577,"HandleHttpContent response_ptr->rsp_header_info.response_code=%d ")
TRACE_MSG(HTTP_HSM_5876_112_2_17_23_17_36_578,"\n http HandleHttpContent send HTTP_SIG_GET_CNF")
TRACE_MSG(HTTP_HSM_5908_112_2_17_23_17_36_579,"HandleHttpContent(2)  http delete timer")
TRACE_MSG(HTTP_HSM_5914_112_2_17_23_17_36_580,"HandleHttpContent(2)  http create timer")
TRACE_MSG(HTTP_HSM_5967_112_2_17_23_17_36_581,"HTTP HandleDataFromCache free content 1")
TRACE_MSG(HTTP_HSM_6009_112_2_17_23_17_37_582,"\n http HandleDataFromCache error_code=%d")
TRACE_MSG(HTTP_HSM_6157_112_2_17_23_17_37_583,"\n http HandleReceivingHeaderRspPendingHeaderRsp HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_HSM_6200_112_2_17_23_17_37_584,"\n http HttpSecurityPostMessage recv SSL_SEND_MESSAGE_HANDSHAKE_SUCC\n")
TRACE_MSG(HTTP_HSM_6205_112_2_17_23_17_37_585,"\n http HttpSecurityPostMessage recv SSL_SEND_MESSAGE_FAIL\n")
TRACE_MSG(HTTP_HSM_6210_112_2_17_23_17_37_586,"\n http HttpSecurityPostMessage recv SSL_SEND_MESSAGE_CLOSE_BY_SERVER\n")
TRACE_MSG(HTTP_HSM_6215_112_2_17_23_17_37_587,"\n http HttpSecurityPostMessage recv SSL_SEND_MESSAGE_SEND_END\n")
TRACE_MSG(HTTP_HSM_6222_112_2_17_23_17_37_588,"\n http HttpSecurityPostMessage recv error\n")
TRACE_MSG(HTTP_HSM_6262_112_2_17_23_17_37_589,"\n http HttpSecurityRecvDataOut free content 1")
TRACE_MSG(HTTP_HSM_6275_112_2_17_23_17_37_590,"\n http HttpSecurityRecvDataOut content_ptr =%d")
TRACE_MSG(HTTP_HSM_6291_112_2_17_23_17_37_591,"\n http HttpSecurityRecvDataOut response_ptr->content_total_len=%d")
TRACE_MSG(HTTP_HSM_6320_112_2_17_23_17_37_592,"http HttpSecurityRecvDataOut cache delete")
TRACE_MSG(HTTP_HSM_6391_112_2_17_23_17_37_593,"\n http HTTP_DealSignal recv HTTP_SIG_INIT_REQ \n")
TRACE_MSG(HTTP_HSM_6396_112_2_17_23_17_37_594,"\n http HTTP_DealSignal recv HTTP_SIG_CANCEL_REQ \n")
TRACE_MSG(HTTP_HSM_6401_112_2_17_23_17_37_595,"\n http HTTP_DealSignal recv HTTP_SIG_CLOSE_REQ \n")
TRACE_MSG(HTTP_HSM_6406_112_2_17_23_17_37_596,"\n http HTTP_DealSignal recv HTTP_SIG_NEED_AUTH_RSP \n")
TRACE_MSG(HTTP_HSM_6411_112_2_17_23_17_37_597,"\n http HTTP_DealSignal recv HTTP_SIG_HEADER_RSP \n")
TRACE_MSG(HTTP_HSM_6416_112_2_17_23_17_37_598,"\n http HTTP_DealSignal recv HTTP_SIG_SSL_CERT_UNTRUST_RSP \n")
TRACE_MSG(HTTP_HSM_6421_112_2_17_23_17_37_599,"\n http HTTP_DealSignal recv HTTP_SIG_TIMEOUT_IND \n")
TRACE_MSG(HTTP_HSM_6425_112_2_17_23_17_37_600,"\n http HTTP_DealSignal recv HTTP_SIG_RECV_PROGRESS_TIMEOUT_IND \n")
TRACE_MSG(HTTP_HSM_6429_112_2_17_23_17_37_601,"\n http HTTP_DealSignal recv HTTP_SIG_HANDSHAKE_TIMEOUT_IND \n")
TRACE_MSG(HTTP_HSM_6447_112_2_17_23_17_37_602,"\n HTTP HTTP_DealSignal unknown signal\n")
TRACE_MSG(HTTP_HSM_6624_112_2_17_23_17_38_603,"HTTP HTTP_CookieClear context_id =%d")
TRACE_MSG(HTTP_HSM_6633_112_2_17_23_17_38_604,"HTTP HttpHandleCookieClearReq HttpCookieFinalize 1 error_code = %d")
TRACE_MSG(HTTP_HSM_6642_112_2_17_23_17_38_605,"HTTP HttpHandleCookieClearReq HttpCookieFinalize 2 error_code = %d")
TRACE_MSG(HTTP_HSM_6647_112_2_17_23_17_38_606,"HTTP HttpHandleCookieClearReq HttpCookieFinalize 3 error_code = %d")
TRACE_MSG(HTTP_HSM_6651_112_2_17_23_17_38_607,"HTTP HttpHandleCookieClearReq HttpCookieFinalize 4 error_code = %d")
TRACE_MSG(HTTP_HSM_6684_112_2_17_23_17_38_608,"HTTP HttpHandleCacheDeleteReq context_id =%d")
TRACE_MSG(HTTP_HSM_6695_112_2_17_23_17_38_609,"\n http HttpHandleCacheDeleteReq alloc error\n")
TRACE_MSG(HTTP_HSM_6715_112_2_17_23_17_38_610,"\n http HttpHandleCacheDeleteReq file name error\n")
TRACE_MSG(HTTP_HSM_6734_112_2_17_23_17_38_611,"\n http HttpHandleCacheDeleteReq HTTP_ERROR_FILE_OPEN")
TRACE_MSG(HTTP_HSM_6772_112_2_17_23_17_38_612,"HTTP HttpHandleNeedAuthRsp 1")
TRACE_MSG(HTTP_HSM_7189_112_2_17_23_17_39_613,"\n http http init\n")
TRACE_MSG(HTTP_HSM_7273_112_2_17_23_17_39_614,"\n http HTTP_GetInitInfo proxy host111 = %s")
TRACE_MSG(HTTP_HSM_7278_112_2_17_23_17_39_615,"\n http HTTP_GetInitInfo proxy password = %s")
TRACE_MSG(HTTP_HSM_7282_112_2_17_23_17_39_616,"\n http HTTP_GetInitInfo proxy port =%d")
TRACE_MSG(HTTP_HSM_7287_112_2_17_23_17_39_617,"\n http HTTP_GetInitInfo proxy user name = %s")
TRACE_MSG(HTTP_HSM_7293_112_2_17_23_17_39_618,"HTTP_GetInitInfo")
TRACE_MSG(HTTP_HSM_7354_112_2_17_23_17_39_619,"\n http HTTP_SigErrCnf HTTP_SIG_INIT_REQ\n")
TRACE_MSG(HTTP_HSM_7359_112_2_17_23_17_39_620,"\n http HTTP_SigErrCnf HTTP_SIG_CANCEL_REQ\n")
TRACE_MSG(HTTP_HSM_7364_112_2_17_23_17_39_621,"\n http HTTP_SigErrCnf HTTP_SIG_CLOSE_REQ\n")
TRACE_MSG(HTTP_HSM_7369_112_2_17_23_17_39_622,"\n http HTTP_SigErrCnf HTTP_SIG_NEED_AUTH_RSP\n")
TRACE_MSG(HTTP_HSM_7374_112_2_17_23_17_39_623,"\n http HTTP_SigErrCnf HTTP_SIG_HEADER_RSP\n")
TRACE_MSG(HTTP_HSM_7390_112_2_17_23_17_39_624,"\n http HTTP_SigErrCnf HTTP_SIG_SSL_CERT_UNTRUST_RSP\n")
TRACE_MSG(HTTP_HSM_7396_112_2_17_23_17_39_625,"\n http HTTP_SigErrCnf HTTP_SIG_GET_REQ\n")
TRACE_MSG(HTTP_HSM_7401_112_2_17_23_17_39_626,"\n http HTTP_SigErrCnf HTTP_SIG_HEAD_REQ\n")
TRACE_MSG(HTTP_HSM_7406_112_2_17_23_17_39_627,"\n http HTTP_SigErrCnf HTTP_SIG_POST_REQ\n")
TRACE_MSG(HTTP_HSM_7411_112_2_17_23_17_39_628,"\n HTTP HTTP_SigErrCnf unknown signal \n")
TRACE_MSG(HTTP_REQUEST_116_112_2_17_23_17_40_629,"\n http HTTP_CreateRequest net_request_handle =%x\n")
TRACE_MSG(HTTP_REQUEST_119_112_2_17_23_17_40_630,"\n http HTTP_CreateRequest input error\n")
TRACE_MSG(HTTP_REQUEST_139_112_2_17_23_17_40_631,"\n http hsm = %d\n")
TRACE_MSG(HTTP_REQUEST_151_112_2_17_23_17_40_632,"\n http HTTP_CreateRequest HTTP_SIG_GET_REQ\n")
TRACE_MSG(HTTP_REQUEST_162_112_2_17_23_17_40_633,"\n http HTTP_CreateRequest HTTP_SIG_POST_REQ\n")
TRACE_MSG(HTTP_REQUEST_173_112_2_17_23_17_40_634,"\n http HTTP_CreateRequest HTTP_SIG_HEAD_REQ\n")
TRACE_MSG(HTTP_REQUEST_183_112_2_17_23_17_40_635,"\n HTTP HTTP_CreateRequest unknown request\n")
TRACE_MSG(HTTP_REQUEST_196_112_2_17_23_17_40_636,"\n HTTP HTTP_CreateRequest request id = %d,id address =%d\n")
TRACE_MSG(HTTP_REQUEST_216_112_2_17_23_17_40_637,"\n http HTTP_DestroyRequest input error\n")
TRACE_MSG(HTTP_REQUEST_223_112_2_17_23_17_40_638,"\n http HTTP_DestroyRequest 1 input error\n")
TRACE_MSG(HTTP_REQUEST_333_112_2_17_23_17_41_639,"HTTP_DestroyRequest request_id =%d,id address =%d")
TRACE_MSG(HTTP_REQUEST_435_112_2_17_23_17_41_640,"http CreateGetRequest scheme=%s")
TRACE_MSG(HTTP_REQUEST_566_112_2_17_23_17_41_641,"http CreatePostRequest scheme=%s")
TRACE_MSG(HTTP_REQUEST_677_112_2_17_23_17_41_642,"\n http CreatePostRequest post file \n")
TRACE_MSG(HTTP_REQUEST_692_112_2_17_23_17_41_643,"\n http CreatePostRequest post buffer copy \n")
TRACE_MSG(HTTP_REQUEST_697_112_2_17_23_17_41_644,"\n http CreatePostRequest post buffer not copy \n")
TRACE_MSG(HTTP_REQUEST_702_112_2_17_23_17_41_645,"\n http CreatePostRequest post buffer null \n")
TRACE_MSG(HTTP_REQUEST_740_112_2_17_23_17_41_646,"http CreateHeadRequest scheme=%s")
TRACE_MSG(HTTP_REQUEST_894_112_2_17_23_17_42_647,"\n http SaveAuthParam error_code=%d")
TRACE_MSG(HTTP_REQUEST_946_112_2_17_23_17_42_648,"\n HTTP HttpRequestFormat 1 port = %d")
TRACE_MSG(HTTP_REQUEST_954_112_2_17_23_17_42_649,"\n HTTP HTTPRequestFormat malloc failed\n")
TRACE_MSG(HTTP_REQUEST_1448_112_2_17_23_17_43_650,"\n HTTP HttpRequestFormat port = %d")
TRACE_MSG(HTTP_REQUEST_1542_112_2_17_23_17_43_651,"\n HTTP HTTPRequestFormat, need to proxyAuthenticate\n")
TRACE_MSG(HTTP_REQUEST_1562_112_2_17_23_17_43_652,"\n HTTP Proxy_Authentication get is:%s\n")
TRACE_MSG(HTTP_REQUEST_1570_112_2_17_23_17_43_653,"\n HTTP HTTPRequestFormat, need not to proxyAuthenticate\n")
TRACE_MSG(HTTP_REQUEST_1599_112_2_17_23_17_43_654,"\n HTTP HttpRequestFormat ETAG get is:%s")
TRACE_MSG(HTTP_REQUEST_1620_112_2_17_23_17_43_655,"\n HTTP HttpRequestFormat last modified get is:%s")
TRACE_MSG(HTTP_REQUEST_1630_112_2_17_23_17_43_656,"\n HTTP HTTPRequestFormat, need to wwwAuthenticate\n")
TRACE_MSG(HTTP_REQUEST_1652_112_2_17_23_17_43_657,"\n HTTP HTTPRequestFormat, need not to wwwAuthenticate\n")
TRACE_MSG(HTTP_REQUEST_1736_112_2_17_23_17_43_658,"http HttpRequestFormat realloc error")
TRACE_MSG(HTTP_REQUEST_1769_112_2_17_23_17_43_659,"\n HTTP HTTPRequestFormat out\n")
TRACE_MSG(HTTP_REQUEST_1790_112_2_17_23_17_43_660,"\n http HTTP_UpdateRequest input error\n")
TRACE_MSG(HTTP_REQUEST_1825_112_2_17_23_17_43_661,"\n http HTTP_UpdateRequest \n")
TRACE_MSG(HTTP_REQUEST_1841_112_2_17_23_17_44_662,"\n http HttpTraceGetParam accept_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1845_112_2_17_23_17_44_663,"\n http HttpTraceGetParam accept_charset_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1847_112_2_17_23_17_44_664,"\n http HttpTraceGetParam accept_encoding  %d\n")
TRACE_MSG(HTTP_REQUEST_1850_112_2_17_23_17_44_665,"\n http HttpTraceGetParam accept_language_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1854_112_2_17_23_17_44_666,"\n http HttpTraceGetParam user_name_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1858_112_2_17_23_17_44_667,"\n http HttpTraceGetParam password_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1862_112_2_17_23_17_44_668,"\n http HttpTraceGetParam entity_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1864_112_2_17_23_17_44_669,"\n http HttpTraceGetParam entity_len %d\n")
TRACE_MSG(HTTP_REQUEST_1868_112_2_17_23_17_44_670,"\n http HttpTraceGetParamurl %s\n")
TRACE_MSG(HTTP_REQUEST_1875_112_2_17_23_17_44_671,"\n http HttpTraceGetParam header_name_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1879_112_2_17_23_17_44_672,"\n http HttpTraceGetParam header_value_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1882_112_2_17_23_17_44_673,"\n http HttpTraceGetParam is_header_use_file %d\n")
TRACE_MSG(HTTP_REQUEST_1883_112_2_17_23_17_44_674,"\n http HttpTraceGetParam get_data.style %d\n")
TRACE_MSG(HTTP_REQUEST_1884_112_2_17_23_17_44_675,"\n http HttpTraceGetParam need_net_prog_ind %d\n")
TRACE_MSG(HTTP_REQUEST_1887_112_2_17_23_17_44_676,"\n http HttpTraceGetParam referer_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1891_112_2_17_23_17_44_677,"\n http HttpTraceGetParam ua_profile_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1895_112_2_17_23_17_44_678,"\n http HttpTraceGetParam user_agent_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1912_112_2_17_23_17_44_679,"\n http HttpTracePostParam accept_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1916_112_2_17_23_17_44_680,"\n http HttpTracePostParam accept_charset_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1918_112_2_17_23_17_44_681,"\n http HttpTracePostParam accept_encoding  %d\n")
TRACE_MSG(HTTP_REQUEST_1921_112_2_17_23_17_44_682,"\n http HttpTracePostParam accept_language_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1925_112_2_17_23_17_44_683,"\n http HttpTracePostParam user_name_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1929_112_2_17_23_17_44_684,"\n http HttpTracePostParam password_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1933_112_2_17_23_17_44_685,"\n http HttpTracePostParam entity_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1935_112_2_17_23_17_44_686,"\n http HttpTracePostParam entity_len %d\n")
TRACE_MSG(HTTP_REQUEST_1939_112_2_17_23_17_44_687,"\n http HttpTracePostParam uri %s\n")
TRACE_MSG(HTTP_REQUEST_1946_112_2_17_23_17_44_688,"\n http HttpTracePostParam header_name_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1950_112_2_17_23_17_44_689,"\n http HttpTracePostParam header_value_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1953_112_2_17_23_17_44_690,"\n http HttpTracePostParam is_header_use_file %d\n")
TRACE_MSG(HTTP_REQUEST_1954_112_2_17_23_17_44_691,"\n http HttpTracePostParam get_data.style %d\n")
TRACE_MSG(HTTP_REQUEST_1955_112_2_17_23_17_44_692,"\n http HttpTracePostParam need_net_prog_ind %d\n")
TRACE_MSG(HTTP_REQUEST_1958_112_2_17_23_17_44_693,"\n http HttpTracePostParam referer_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1962_112_2_17_23_17_44_694,"\n http HttpTracePostParam ua_profile_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1966_112_2_17_23_17_44_695,"\n http HttpTracePostParam user_agent_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1970_112_2_17_23_17_44_696,"\n http HttpTracePostParam body_type_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1972_112_2_17_23_17_44_697,"\n http HttpTracePostParam post_body.is_use_file %d\n")
TRACE_MSG(HTTP_REQUEST_1988_112_2_17_23_17_44_698,"\n http HttpTraceHeadParam accept_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1992_112_2_17_23_17_44_699,"\n http HttpTraceHeadParam accept_charset_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_1994_112_2_17_23_17_44_700,"\n http HttpTraceHeadParam accept_encoding  %d\n")
TRACE_MSG(HTTP_REQUEST_1997_112_2_17_23_17_44_701,"\n http HttpTraceHeadParam accept_language_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_2001_112_2_17_23_17_44_702,"\n http HttpTraceHeadParam user_name_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_2005_112_2_17_23_17_44_703,"\n http HttpTraceHeadParam password_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_2009_112_2_17_23_17_44_704,"\n http HttpTraceHeadParam entity_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_2011_112_2_17_23_17_44_705,"\n http HttpTraceHeadParam entity_len %d\n")
TRACE_MSG(HTTP_REQUEST_2016_112_2_17_23_17_44_706,"\n http HttpTraceHeadParam header_name_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_2020_112_2_17_23_17_44_707,"\n http HttpTraceHeadParam header_value_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_2023_112_2_17_23_17_44_708,"\n http HttpTraceHeadParam is_header_use_file %d\n")
TRACE_MSG(HTTP_REQUEST_2024_112_2_17_23_17_44_709,"\n http HttpTraceHeadParam need_net_prog_ind %d\n")
TRACE_MSG(HTTP_REQUEST_2027_112_2_17_23_17_44_710,"\n http HttpTraceHeadParam referer_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_2031_112_2_17_23_17_44_711,"\n http HttpTraceHeadParam ua_profile_ptr %s\n")
TRACE_MSG(HTTP_REQUEST_2035_112_2_17_23_17_44_712,"\n http HttpTraceHeadParam user_agent_ptr %s\n")
TRACE_MSG(HTTP_RESPONSE_174_112_2_17_23_17_44_713,"\n http HTTP_DestroyResponse free content 1")
TRACE_MSG(HTTP_RESPONSE_327_112_2_17_23_17_45_714,"\n http HttpResponseParseHead input error\n")
TRACE_MSG(HTTP_RESPONSE_401_112_2_17_23_17_45_715,"\n  HTTP HttpResponseParseHead parseheader error")
TRACE_MSG(HTTP_RESPONSE_406_112_2_17_23_17_45_716,"\n  HTTP HttpResponseParseHead free content")
TRACE_MSG(HTTP_RESPONSE_434_112_2_17_23_17_45_717,"\n http HttpResponseParseStateCode input error")
TRACE_MSG(HTTP_RESPONSE_442_112_2_17_23_17_45_718,"\n http HttpResponseParseStateCode state code error")
TRACE_MSG(HTTP_RESPONSE_594_112_2_17_23_17_46_719,"\n  HTTP ParseHttpHeader input error")
TRACE_MSG(HTTP_RESPONSE_600_112_2_17_23_17_46_720,"\n  HTTP ParseHttpHeader separate error")
TRACE_MSG(HTTP_RESPONSE_773_112_2_17_23_17_46_721,"\n HTTP HttpSeparate read line error")
TRACE_MSG(HTTP_RESPONSE_961_112_2_17_23_17_46_722,"HttpParseResponseCacheHeader expired")
TRACE_MSG(HTTP_RESPONSE_969_112_2_17_23_17_46_723,"\n HTTP HttpParseResponseCacheHeader unknown param \n")
TRACE_MSG(HTTP_RESPONSE_1024_112_2_17_23_17_46_724,"\n http HttpParseResponseContentLengthHeader content len=%d")
TRACE_MSG(HTTP_RESPONSE_1140_112_2_17_23_17_47_725,"HTTP HttpParseResponseSetCookieHeader URL scheme = %s,cookie path =%s ")
TRACE_MSG(HTTP_RESPONSE_1144_112_2_17_23_17_47_726,"HttpParseResponseSetCookieHeader path ptr=%s,fragment =%s, query =%s")
TRACE_MSG(HTTP_RESPONSE_1151_112_2_17_23_17_47_727,"HttpParseResponseSetCookieHeader cookie node domain =%s,expire str =%s,secure =%d")
TRACE_MSG(HTTP_RESPONSE_1155_112_2_17_23_17_47_728,"HttpParseResponseSetCookieHeader cookie node name =%s,value=%s,other name=%s,other v=%s")
TRACE_MSG(HTTP_RESPONSE_1159_112_2_17_23_17_47_729,"HttpParseResponseSetCookieHeader tail =%d,path =%s,port=%d")
TRACE_MSG(HTTP_RESPONSE_1337_112_2_17_23_17_47_730,"http HttpParseResponseLocationHeader value = %s")
TRACE_MSG(HTTP_RESPONSE_1410_112_2_17_23_17_47_731,"\n http HttpParseResponseLocationHeader request_ptr->uri_str_ptr = %s")
TRACE_MSG(HTTP_RESPONSE_1944_112_2_17_23_17_48_732,"HttpParseResponseExpiresHeader expires =%x,now = %x")
TRACE_MSG(HTTP_RESPONSE_1947_112_2_17_23_17_48_733,"HttpParseResponseExpiresHeader need save")
TRACE_MSG(HTTP_RESPONSE_1954_112_2_17_23_17_48_734,"HttpParseResponseExpiresHeader expired")
TRACE_MSG(HTTP_SERIALIZE_1917_112_2_17_23_17_52_735,"\n http HttpSerializeHttpInitReqSigT error")
TRACE_MSG(HTTP_SERIALIZE_2027_112_2_17_23_17_53_736,"\n http HttpSerializeHttpGetReqSigT error")
TRACE_MSG(HTTP_SERIALIZE_2136_112_2_17_23_17_53_737,"\n http HttpSerializeHttpPostReqSigT error")
TRACE_MSG(HTTP_SERIALIZE_2245_112_2_17_23_17_53_738,"\n http HttpSerializeHttpHeadReqSigT error")
TRACE_MSG(HTTP_SERIALIZE_2535_112_2_17_23_17_54_739,"\n http HttpSerializeHttpAuthRspSigT error")
TRACE_MSG(HTTP_SERIALIZE_2693_112_2_17_23_17_54_740,"\n http HttpSerializeHttpGetCnfSigT error")
TRACE_MSG(HTTP_SERIALIZE_2806_112_2_17_23_17_54_741,"\n http HttpSerializeHttpPostCnfSigT error")
TRACE_MSG(HTTP_SERIALIZE_2919_112_2_17_23_17_54_742,"\n http HttpSerializeHttpHeadCnfSigT error")
TRACE_MSG(HTTP_SERIALIZE_3396_112_2_17_23_17_55_743,"\n http HttpSerializeHttpCookieSetReqSigT error")
TRACE_MSG(HTTP_SERIALIZE_4207_112_2_17_23_17_57_744,"\n http HttpSerializeHttpSslGetCertCnfSigT error")
TRACE_MSG(HTTP_SIGNAL_71_112_2_17_23_17_58_745,"\n http HttpSendNetIndToApp send HTTP_SIG_NET_PROG_IND request_id =%d,error code = %d")
TRACE_MSG(HTTP_SIGNAL_110_112_2_17_23_17_59_746,"\n http HttpSendDataIndToApp send HTTP_SIG_DATA_IND request_id =%d,error code=%d")
TRACE_MSG(HTTP_SIGNAL_148_112_2_17_23_17_59_747,"\n http HttpSendHeaderIndToApp send HTTP_SIG_HEADER_IND request_id =%d,error code=%d")
TRACE_MSG(HTTP_SIGNAL_179_112_2_17_23_17_59_748,"\n http HttpSendHeadCnfToApp send HTTP_SIG_HEAD_CNF request_id =%d,error code=%d")
TRACE_MSG(HTTP_SIGNAL_202_112_2_17_23_17_59_749,"\n http HttpSendErrorIndToApp error_code = %d")
TRACE_MSG(HTTP_SIGNAL_210_112_2_17_23_17_59_750,"\n http HttpSendErrorIndToApp request_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_231_112_2_17_23_17_59_751,"\n http HttpSendCookieSetCnfToApp error_code = %d")
TRACE_MSG(HTTP_SIGNAL_239_112_2_17_23_17_59_752,"\n http HttpSendCookieSetCnfToApp err code =%d")
TRACE_MSG(HTTP_SIGNAL_270_112_2_17_23_17_59_753,"\n http HttpSendCancelCnfToApp send HTTP_SIG_CANCEL_CNF request_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_303_112_2_17_23_17_59_754,"\n http HttpSendGetCnfToApp send HTTP_SIG_GET_CNF request_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_336_112_2_17_23_17_59_755,"\n http HttpSendPostCnfToApp send HTTP_SIG_POST_CNF request_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_368_112_2_17_23_17_59_756,"\n http HttpSendNeedAuthToApp send HTTP_SIG_NEED_AUTH_IND request_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_402_112_2_17_23_17_59_757,"\n http HttpSendInitCnfToApp send HTTP_SIG_INIT_CNF context_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_428_112_2_17_23_17_59_758,"\n http HttpSendCloseCnfToApp send HTTP_SIG_CLOSE_CNF context_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_454_112_2_17_23_17_59_759,"\n http HttpSendClearCookieCnfToApp send HTTP_SIG_COOKIE_CLEAR_CNF err code=%d")
TRACE_MSG(HTTP_SIGNAL_480_112_2_17_23_17_59_760,"\n http HttpSendCacheDeleteCnfToApp send HTTP_SIG_CACHE_DELETE_CNF, err code =%d")
TRACE_MSG(HTTP_SIGNAL_509_112_2_17_23_18_0_761,"\n http HttpSendRequestIndToApp send HTTP_SIG_REQUEST_ID_IND request_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_542_112_2_17_23_18_0_762,"\n http HttpSendReadCacheResult send HTTP_SIG_READ_CACHE_SUCC or fail request_id =%d,err code =%d")
TRACE_MSG(HTTP_SIGNAL_573_112_2_17_23_18_0_763,"\n http HttpSendSslCertIndToApp send HTTP_SIG_SSL_CERT_UNTRUST_IND request_id =%d,err code=%d")
TRACE_MSG(HTTP_SIGNAL_610_112_2_17_23_18_0_764,"\n http HttpSendRedirectIndToApp send HTTP_SIG_REDIRECT_IND request_id =%d,err code=%d")
TRACE_MSG(HTTP_TEST_88_112_2_17_23_18_0_765,"\n HTTP TEST no memory \n")
TRACE_MSG(HTTP_TEST_181_112_2_17_23_18_0_766,"\n HTTP TEST no memory \n")
TRACE_MSG(HTTP_TEST_267_112_2_17_23_18_0_767,"\n HTTP TEST no memory \n")
TRACE_MSG(HTTP_TEST_306_112_2_17_23_18_1_768,"\n HTTP TEST no memory \n")
TRACE_MSG(HTTP_TEST_416_112_2_17_23_18_1_769,"\n http test auth request id = %d\n")
TRACE_MSG(HTTP_TEST_422_112_2_17_23_18_1_770,"\n http test request id = %d\n")
TRACE_MSG(HTTP_TEST_426_112_2_17_23_18_1_771,"\n http test recv HTTP_SIG_GET_CNF\n")
TRACE_MSG(HTTP_TEST_431_112_2_17_23_18_1_772,"\n http test recv HTTP_SIG_HEAD_CNF\n")
TRACE_MSG(HTTP_TEST_437_112_2_17_23_18_1_773,"\n http test data ind rsp code=%d,request_id =%d\n")
TRACE_MSG(HTTP_TEST_441_112_2_17_23_18_1_774,"\n http test recv HTTP_SIG_HEADER_IND\n")
TRACE_MSG(HTTP_TEST_450_112_2_17_23_18_1_775,"\n http test recv HTTP_SIG_ERROR_IND, ERROR=%d\n")
TRACE_MSG(HTTP_TEST_454_112_2_17_23_18_1_776,"\n http test recv HTTP_SIG_CLOSE_CNF\n")
TRACE_MSG(HTTP_TEST_463_112_2_17_23_18_1_777,"\n http test net prog ind,%d,%d,%d\n")
TRACE_MSG(HTTP_TEST_466_112_2_17_23_18_1_778,"\n http test redirect ind\n")
TRACE_MSG(HTTP_TEST_469_112_2_17_23_18_1_779,"\n http test recv HTTP_SIG_POST_CNF\n")
TRACE_MSG(HTTP_TEST_526_112_2_17_23_18_1_780,"\n http test auth request id = %d\n")
TRACE_MSG(HTTP_TEST_532_112_2_17_23_18_1_781,"\n http test request id = %d\n")
TRACE_MSG(HTTP_TEST_536_112_2_17_23_18_1_782,"\n http test recv HTTP_SIG_GET_CNF\n")
TRACE_MSG(HTTP_TEST_541_112_2_17_23_18_1_783,"\n http test recv HTTP_SIG_HEAD_CNF\n")
TRACE_MSG(HTTP_TEST_547_112_2_17_23_18_1_784,"\n http test data ind rsp code=%d,request_id =%d\n")
TRACE_MSG(HTTP_TEST_551_112_2_17_23_18_1_785,"\n http test recv HTTP_SIG_HEADER_IND\n")
TRACE_MSG(HTTP_TEST_560_112_2_17_23_18_1_786,"\n http test recv HTTP_SIG_ERROR_IND, ERROR=%d\n")
TRACE_MSG(HTTP_TEST_564_112_2_17_23_18_1_787,"\n http test recv HTTP_SIG_CLOSE_CNF\n")
TRACE_MSG(HTTP_TEST_573_112_2_17_23_18_1_788,"\n http test net prog ind,%d,%d,%d\n")
TRACE_MSG(HTTP_TEST_576_112_2_17_23_18_1_789,"\n http test redirect ind\n")
TRACE_MSG(HTTP_TEST_579_112_2_17_23_18_1_790,"\n http test recv HTTP_SIG_POST_CNF\n")
TRACE_MSG(HTTP_UTIL_113_112_2_17_23_18_1_791,"\n HTTP HttpReadLine error")
TRACE_MSG(HTTP_UTIL_124_112_2_17_23_18_2_792,"\n HTTP HttpReadLine succ")
TRACE_MSG(HTTP_UTIL_816_112_2_17_23_18_3_793,"http HttpCookie_memInit %d")
TRACE_MSG(HTTP_UTIL_844_112_2_17_23_18_3_794,"http HttpCookie_memExit %d")
TRACE_MSG(RVADMUTEX_61_112_2_17_23_18_20_795,"RvAdMutexLock error.")
TRACE_MSG(RVADMUTEX_87_112_2_17_23_18_20_796,"RvAdMutexLock error.")
TRACE_MSG(RVSELECT_2674_112_2_17_23_19_0_797,"SIP:RvSelectWaitAndBlock out of blocking\n")
TRACE_MSG(RVSELECT_2686_112_2_17_23_19_1_798,"SIP: RvSelectWaitAndBlock, recv_len == %d, socket recv error:%d\n")
TRACE_MSG(RVSOCKET_1809_112_2_17_23_19_5_799,"SIP:RvSocketSendBuffer ourSocket=0x%x, bytesToSend=%d")
TRACE_MSG(RTP_API_68_112_2_17_23_20_11_800,"RTP:RTP_CreateSession error_code =%d")
TRACE_MSG(RTP_API_115_112_2_17_23_20_11_801,"RTP:RTP_GetSrrrSendingMode error_code =%d")
TRACE_MSG(RTP_API_131_112_2_17_23_20_11_802,"RTP:RTP_GetSrrrSendingMode error_code =%d")
TRACE_MSG(RTP_API_147_112_2_17_23_20_11_803,"RTP:RTP_SetSrrrReceivingMode error_code =%d")
TRACE_MSG(RTP_API_163_112_2_17_23_20_11_804,"RTP:RTP_GetSrrrReceivingMode error_code =%d")
TRACE_MSG(RTP_API_180_112_2_17_23_20_11_805,"RTP:RTP_SendRtcpSrrrReq, input error!")
TRACE_MSG(RTP_API_187_112_2_17_23_20_11_806,"RTP:RTP_SendRtcpSrrrReq, no memory!")
TRACE_MSG(RTP_API_199_112_2_17_23_20_11_807,"RTP:RTP_SendRtcpSrrrReq !")
TRACE_MSG(RTP_API_217_112_2_17_23_20_11_808,"RTP:RTP_SendRtcpAppDataReq, input error!")
TRACE_MSG(RTP_API_224_112_2_17_23_20_11_809,"RTP:RTP_SendRtcpAppDataReq, data length error!")
TRACE_MSG(RTP_API_232_112_2_17_23_20_11_810,"RTP:RTP_SendRtcpAppDataReq, no memory!")
TRACE_MSG(RTP_API_263_112_2_17_23_20_11_811,"RTP:RTP_SendRtcpAppDataReq !")
TRACE_MSG(RTP_API_280_112_2_17_23_20_11_812,"RTP:RTP_GetRemoteAddressBySsrc error_code =%d")
TRACE_MSG(RTP_API_295_112_2_17_23_20_11_813,"RTP:RTP_TerminateSession error_code =%d")
TRACE_MSG(RTP_MGR_339_112_2_17_23_20_12_814,"RTP:, un-handled event:%d")
TRACE_MSG(RTP_MGR_421_112_2_17_23_20_13_815,"RTP: GetNodeData, invalid timer node!")
TRACE_MSG(RTP_MGR_738_112_2_17_23_20_13_816,"RTP:RTPMgr_Init RTP_InitStack failed\n")
TRACE_MSG(RTP_MGR_744_112_2_17_23_20_13_817,"RTP:RTPMgr_Init RTCP_Init failed\n")
TRACE_MSG(RTP_MGR_796_112_2_17_23_20_13_818,"RTP READ DATA LEN = %d")
TRACE_MSG(RTP_MGR_1668_112_2_17_23_20_15_819,"RTP RTP_GetRemoteAddressBySsrcHandler rtcp port =%d")
TRACE_MSG(SDP_API_438_112_2_17_23_20_17_820,"#SDP SDP_MediaDescrGetAttrValueByName error get attr_ptr at the number %d index")
TRACE_MSG(SDP_API_485_112_2_17_23_20_17_821,"#SDP SDP_MsgGetAttrValueByName error get attr_ptr at the number %d index")
TRACE_MSG(SIP_MGR_CALL_732_112_2_17_23_20_24_822,"SIP: SIPMGR_CallInviteReqHandler, error line = %d\n")
TRACE_MSG(SIP_MGR_CALL_770_112_2_17_23_20_24_823,"SIP: SIPMGR_CallAckReqHandler, RvSipCallLegAck fail\n")
TRACE_MSG(SIP_MGR_CALL_816_112_2_17_23_20_24_824,"SIP: SIPMGR_CallDisconnectReqHandler, error line = %d\n")
TRACE_MSG(SIP_MGR_CALL_873_112_2_17_23_20_24_825,"SIP: SIPMGR_CallCancelReqHandler, error line = %d\n")
TRACE_MSG(SIP_MGR_CALL_1022_112_2_17_23_20_24_826,"SIP:SIPMGR_CallInviteRspHandler, error line = %d")
TRACE_MSG(SIP_MGR_CALL_1103_112_2_17_23_20_25_827,"SIP: CallLegStateRedirectedHandler, SIPMGR_GetIdByCallClientHandle fail\n")
TRACE_MSG(SIP_MGR_CALL_1133_112_2_17_23_20_25_828,"SIP: CallLegStateRedirectedHandler, ConstructMsgHandleInCalllegHandle fail\n")
TRACE_MSG(SIP_MGR_CALL_1140_112_2_17_23_20_25_829,"SIP: CallLegStateRedirectedHandler, RvSipCallLegConnect fail\n")
TRACE_MSG(SIP_MGR_CALL_1161_112_2_17_23_20_25_830,"SIP: CallLegStateRedirectedHandler, RvSipCallLegDisconnect fail\n")
TRACE_MSG(SIP_MGR_CALL_1170_112_2_17_23_20_25_831,"SIP: CallLegStateRedirectedHandler, error callleg_state=%d\n")
TRACE_MSG(SIP_MGR_CALL_1189_112_2_17_23_20_25_832,"SIP: CallLegStateUnauthenticatedHandler, start to authenticate\n")
TRACE_MSG(SIP_MGR_CALL_1256_112_2_17_23_20_25_833,"SIP:CallLegStateOfferingHandler, GetRegSessionIdForOffering fail\n")
TRACE_MSG(SIP_MGR_CALL_1262_112_2_17_23_20_25_834,"SIP:CallLegStateOfferingHandler, SIPMGR_RegGetContext fail\n")
TRACE_MSG(SIP_MGR_CALL_1270_112_2_17_23_20_25_835,"SIP:CallLegStateOfferingHandler, GetFreeSessinID fail\n")
TRACE_MSG(SIP_MGR_CALL_1366_112_2_17_23_20_25_836,"SIP:CallLegStateAcceptedHandler, callee send invite 200 to peer\n")
TRACE_MSG(SIP_MGR_CALL_1380_112_2_17_23_20_25_837,"SIP:CallLegStateInvitingHandler, caller send invite to peer\n")
TRACE_MSG(SIP_MGR_CALL_1422_112_2_17_23_20_25_838,"SIP: CallLegStateConnectedHandler, error eDirection = %d\n")
TRACE_MSG(SIP_MGR_CALL_1549_112_2_17_23_20_25_839,"SIP:CallLegStateDisconnectingHandler, send bye to peer\n")
TRACE_MSG(SIP_MGR_CALL_1630_112_2_17_23_20_26_840,"SIP: CallLegStateRemoteAcceptedHandler, do nothing\n")
TRACE_MSG(SIP_MGR_CALL_1661_112_2_17_23_20_26_841,"SIP:CallLegStateCancellingHandler, send cancel to peer\n")
TRACE_MSG(SIP_MGR_CALL_1729_112_2_17_23_20_26_842,"SIP:CallLegCreatedEvHandler,Incoming call-leg %p was created\n\n")
TRACE_MSG(SIP_MGR_CALL_1977_112_2_17_23_20_26_843,"SIP:CallLegStateChangedEvHandle==>default eState%d \n")
TRACE_MSG(SIP_MGR_CALL_2226_112_2_17_23_20_27_844,"SIP: SIPMGR_CallUpdateReqHandler, error line = %d\n")
TRACE_MSG(SIP_MGR_CALL_2296_112_2_17_23_20_27_845,"SIP: CallTransHandleStateReqRecv, error line = %d\n")
TRACE_MSG(SIP_MGR_CALL_2311_112_2_17_23_20_27_846,"SIP: CallTransHandleStateResSent, update request sent to peer\n")
TRACE_MSG(SIP_MGR_CALL_2358_112_2_17_23_20_27_847,"SIP: CallTransHandleStateResRecv, responseCode = %d\n")
TRACE_MSG(SIP_MGR_CALL_2391_112_2_17_23_20_27_848,"SIP: CallTransHandleStateResRecv, error line = %d\n")
TRACE_MSG(SIP_MGR_CALL_2465_112_2_17_23_20_27_849,"SIP: SIPMGR_CallUpdateRspHandler, error line = %d\n")
TRACE_MSG(SIP_MGR_CALL_2671_112_2_17_23_20_28_850,"SIP: SendMsgToApp, need not to send msg_id = %d [0]\n")
TRACE_MSG(SIP_MGR_CALL_2739_112_2_17_23_20_28_851,"SIP:CallLegStateProceedingHandler response_code=%d")
TRACE_MSG(SIP_MGR_PUB_400_112_2_17_23_20_30_852,"SIP:SIPMGR_PubSessionReqHandler,Failed to create new publish-client\n")
TRACE_MSG(SIP_MGR_PUB_409_112_2_17_23_20_30_853,"SIP:SIPMGR_PubSessionReqHandler,get context id failed\n")
TRACE_MSG(SIP_MGR_PUB_892_112_2_17_23_20_31_854,"SIP:SIPMGR_PubPublishReqHandler, RvSipPubClientPublish failed\n")
TRACE_MSG(SIP_MGR_PUB_932_112_2_17_23_20_31_855,"SIP: SIPMGR_PubRemoveReqHandler, ConstructMsgHandleInPubHandle failed\n")
TRACE_MSG(SIP_MGR_PUB_940_112_2_17_23_20_31_856,"SIP:SIPMGR_PubRemoveReqHandler, RvSipPubClientRemove failed\n")
TRACE_MSG(SIP_MGR_PUB_976_112_2_17_23_20_31_857,"SIP:SIPMGR_PubRefreshReqHandler, is_refresh_manual FALSE\n")
TRACE_MSG(SIP_MGR_PUB_991_112_2_17_23_20_31_858,"SIP: SIPMGR_PubRefreshReqHandler, ConstructMsgHandleInPubHandle failed\n")
TRACE_MSG(SIP_MGR_PUB_999_112_2_17_23_20_31_859,"SIP:SIPMGR_PubRefreshReqHandler, RvSipPubClientPublish failed\n")
TRACE_MSG(SIP_MGR_PUB_1037_112_2_17_23_20_31_860,"SIP:SIPMGR_PubTerminateReqHandler, RvSipPubClientTerminate failed\n")
TRACE_MSG(SIP_MGR_PUB_1109_112_2_17_23_20_31_861,"SIP:PubClientStateRedirectedHandler, GetSessIdByPubHandle failed\n")
TRACE_MSG(SIP_MGR_PUB_1137_112_2_17_23_20_31_862,"SIP: PubClientStateRedirectedHandler, ConstructMsgHandleInPubHandle failed\n")
TRACE_MSG(SIP_MGR_PUB_1144_112_2_17_23_20_31_863,"SIP: PubClientStateRedirectedHandler, RvSipPubClientPublish fail")
TRACE_MSG(SIP_MGR_PUB_1344_112_2_17_23_20_32_864,"SIP:PubClientStatePublishedHandler, pub_state error = %d\n")
TRACE_MSG(SIP_MGR_PUB_1478_112_2_17_23_20_32_865,"SIP:PubClientStateMsgSendFailureHandler, pub_state error = %d\n")
TRACE_MSG(SIP_MGR_PUB_1616_112_2_17_23_20_32_866,"SIP:PubClientObjExpiredEvHandler, is_refresh_manual TRUE\n")
TRACE_MSG(SIP_MGR_PUB_1637_112_2_17_23_20_32_867,"SIP: PubClientObjExpiredEvHandler, ConstructMsgHandleInPubHandle failed\n")
TRACE_MSG(SIP_MGR_PUB_1645_112_2_17_23_20_32_868,"SIP:PubClientObjExpiredEvHandler, RvSipPubClientPublish failed\n")
TRACE_MSG(SIP_MGR_REG_343_112_2_17_23_20_34_869,"SIP:SIPMGR_RegGetSessIdByHandle fail hRegClient=0x%x")
TRACE_MSG(SIP_MGR_REG_423_112_2_17_23_20_34_870,"SIP:SIPMGR_RegSessionReqHandler Failed to create new register-client")
TRACE_MSG(SIP_MGR_REG_1026_112_2_17_23_20_35_871,"SIP:SIPMGR_RegReqHandler, error line = %d\n")
TRACE_MSG(SIP_MGR_REG_1122_112_2_17_23_20_35_872,"SIP:SIPMGR_DeRegReqHandler, error line = %d\n")
TRACE_MSG(SIP_MGR_REG_1258_112_2_17_23_20_35_873,"SIP:RegClientStateChangedEvHandler eState=%d\n")
TRACE_MSG(SIP_MGR_REG_1273_112_2_17_23_20_35_874,"SIP: RegClientStateRegisteringHandler, send register to registar\n")
TRACE_MSG(SIP_MGR_REG_1297_112_2_17_23_20_35_875,"SIP: RegClientStateRedirectedHandler SIPMGR_RegGetSessIdByHandle fail")
TRACE_MSG(SIP_MGR_REG_1318_112_2_17_23_20_36_876,"SIP: RegClientStateRedirectedHandler ConstructMsgHandleInRegHandle fail")
TRACE_MSG(SIP_MGR_REG_1326_112_2_17_23_20_36_877,"SIP: RegClientStateRedirectedHandler RvSipRegClientRegister fail")
TRACE_MSG(SIP_MGR_REG_1364_112_2_17_23_20_36_878,"SIP:RegClientStateUnauthenticatedHandler RvSipRegClientAuthenticate fail")
TRACE_MSG(SIP_MGR_REG_1457_112_2_17_23_20_36_879,"SIP: RegClientStateRegisteredHandler, error reg_state = %d\n")
TRACE_MSG(SIP_MGR_REG_1608_112_2_17_23_20_36_880,"SIP:RegClientMsgRecvEvHandler status code:%d")
TRACE_MSG(SIP_MGR_REG_1624_112_2_17_23_20_36_881,"SIP:RegClientMsgToSendEvHandler Message Sent (register-client %p)\n")
TRACE_MSG(SIP_MGR_REG_1641_112_2_17_23_20_36_882,"SIP:RegClientObjExpiresEvHandler Message Sent (register-client %p)\n")
TRACE_MSG(SIP_MGR_REG_1851_112_2_17_23_20_37_883,"SIP: SendFailMsgToApp, reg_state = %d\n")
TRACE_MSG(SIP_MGR_SUBS_451_112_2_17_23_20_38_884,"SIP:	SIPMGR_SubsSessionReqHandler, Failed to	create new subscription!\n")
TRACE_MSG(SIP_MGR_SUBS_615_112_2_17_23_20_38_885,"SIP: UpdateSubsToHeaderInSubsHandle, RvSipMsgGetToHeader	failed\n")
TRACE_MSG(SIP_MGR_SUBS_622_112_2_17_23_20_38_886,"SIP: UpdateSubsToHeaderInSubsHandle, RvSipSubsGetDialogObj failed\n")
TRACE_MSG(SIP_MGR_SUBS_629_112_2_17_23_20_38_887,"SIP: UpdateSubsToHeaderInSubsHandle, RvSipCallLegGetToHeader failed\n")
TRACE_MSG(SIP_MGR_SUBS_636_112_2_17_23_20_38_888,"SIP: UpdateSubsToHeaderInSubsHandle, RvSipPartyHeaderCopy failed\n")
TRACE_MSG(SIP_MGR_SUBS_756_112_2_17_23_20_39_889,"SIP:	SIPMGR_SubsReqHandler, error line = %d\n")
TRACE_MSG(SIP_MGR_SUBS_857_112_2_17_23_20_39_890,"SIP:	SIPMGR_SubsTerminateReqHandler,	RvSipSubsTerminate Fail!\n")
TRACE_MSG(SIP_MGR_SUBS_909_112_2_17_23_20_39_891,"SIP:	SubsNotifyEvHandler, RvSipNotifyAccept Fail!\n")
TRACE_MSG(SIP_MGR_SUBS_918_112_2_17_23_20_39_892,"SIP:	SubsNotifyEvHandler, RvSipNotifyReject Fail! response code = %d!\n")
TRACE_MSG(SIP_MGR_SUBS_1057_112_2_17_23_20_39_893,"SIP:RegClientStateChangedEvHandler, eState=%d\n")
TRACE_MSG(SIP_MGR_SUBS_1085_112_2_17_23_20_39_894,"SIP:	SubsStateRedirectedHandler, SIPMGR_SubsGetIdByHandle fail!\n")
TRACE_MSG(SIP_MGR_SUBS_1109_112_2_17_23_20_39_895,"SIP:	SubsStateRedirectedHandler, ConstructMsgHandleInSubsHandle fail\n")
TRACE_MSG(SIP_MGR_SUBS_1117_112_2_17_23_20_39_896,"SIP:	SubsStateRedirectedHandler, RvSipSubsSubscribe fail!\n")
TRACE_MSG(SIP_MGR_SUBS_1220_112_2_17_23_20_39_897,"SIP:	SubsStateUnauthenticatedHandler, SIPMGR_SubsGetIdByHandle fail!\n")
TRACE_MSG(SIP_MGR_SUBS_1230_112_2_17_23_20_39_898,"SIP: SubsStateUnauthenticatedHandler, RvSipSubsGetDialogObj Fail!\n")
TRACE_MSG(SIP_MGR_SUBS_1239_112_2_17_23_20_39_899,"SIP: SubsStateUnauthenticatedHandler, RvSipCallLegAuthenticate Fail!\n")
TRACE_MSG(SIP_MGR_SUBS_1367_112_2_17_23_20_40_900,"SIP: SubsStatePendingHandler, pre_subs_state ERROR = %d\n")
TRACE_MSG(SIP_MGR_SUBS_1433_112_2_17_23_20_40_901,"SIP: SubsStateActiveHandler, pre_subs_state ERROR = %d\n")
TRACE_MSG(SIP_MGR_SUBS_1573_112_2_17_23_20_40_902,"SIP: SubsStateTerminatedHandler,	pre_subs_state ERROR = %d\n")
TRACE_MSG(SIP_MGR_SUBS_1621_112_2_17_23_20_40_903,"SIP:	SubsStateMsgSendFailureHandler,	pre_subs_state ERROR = %d\n")
TRACE_MSG(SIP_MGR_SUBS_1723_112_2_17_23_20_40_904,"SIP:SubsExpirationAlertEvHandler, RvSipSubsRefresh failed\n")
TRACE_MSG(SIP_MGR_SUBS_1923_112_2_17_23_20_41_905,"SIP: GetNotInfoandSendNotIndToApp, Input	parameter point	is NULL!\n")
TRACE_MSG(SIP_MGR_SUBS_1934_112_2_17_23_20_41_906,"SIP: GetNotInfoandSendNotIndToApp, Get SubscriptionState	Header Fail!\n")
TRACE_MSG(SIP_MGR_SUBS_1990_112_2_17_23_20_41_907,"SIP:	GetNotInfoandSendNotIndToApp, subs reason error!\n")
TRACE_MSG(SIP_MGR_SUBS_2001_112_2_17_23_20_41_908,"SIP: GetNotInfoandSendNotIndToApp, sub state error!\n")
TRACE_MSG(SIP_MGR_SUBS_2020_112_2_17_23_20_41_909,"SIP:	GetNotInfoandSendNotIndToApp, SIPPARS_DecodeSipBody Fail!\n")
TRACE_MSG(SIP_MGR_TASK_199_112_2_17_23_20_41_910,"SIP: SipMgrMsgHandle, remote address = %x\n")
TRACE_MSG(SIP_MGR_TASK_209_112_2_17_23_20_41_911,"\nSIP:Get           %.40s    id=0x%x\n")
TRACE_MSG(SIP_MGR_TASK_218_112_2_17_23_20_42_912,"\nRTPRTCP:Get           %.40s    id=0x%x\n")
TRACE_MSG(SIP_MGR_TASK_521_112_2_17_23_20_42_913,"SIP:InitSipStack  Application failed to construct the stack\n")
TRACE_MSG(SIP_MGR_TASK_526_112_2_17_23_20_42_914,"SIP:InitSipStack The RADVISION SIP stack was constructed successfully. Version - %sn")
TRACE_MSG(SIP_MGR_TASK_551_112_2_17_23_20_42_915,"SIP: SIPMGR_SockInit, send socket open error:%d\n")
TRACE_MSG(SIP_MGR_TASK_562_112_2_17_23_20_42_916,"SIP: SIPMGR_SockInit, receive socket open error:%d\n")
TRACE_MSG(SIP_MGR_TASK_574_112_2_17_23_20_42_917,"SIP: SIPMGR_SockInit, receive socket bind error:%d\n")
TRACE_MSG(SIP_MGR_TASK_584_112_2_17_23_20_42_918,"SIP: SIPMGR_SockInit SUCCESS!\n")
TRACE_MSG(SIP_MGR_TASK_634_112_2_17_23_20_42_919,"\nSIP:Send to app   %.40s    id=0x%x\n")
TRACE_MSG(SIP_MGR_TASK_658_112_2_17_23_20_42_920,"\nRTPRTCP:Send to app   %.40s    id=0x%x\n")
TRACE_MSG(SIP_MGR_TASK_686_112_2_17_23_20_42_921,"SIP: SIP_SendSignalToMgr, send message to sip manager error:%d, send_len = %d\n")
TRACE_MSG(SIP_PARSER_385_112_2_17_23_20_44_922,"SIP:GetRedirectedUri, redirected_uri_ptr is PNULL\n")
TRACE_MSG(RTCP_392_112_2_17_23_21_15_923,"RTP:RTP_OpenFrom, open socket fail!")
TRACE_MSG(RTCP_1659_112_2_17_23_21_17_924,"RTCP RECV RTCP_PACKET_TYPE_SR")
TRACE_MSG(RTCP_1676_112_2_17_23_21_17_925,"RTCP RECV RTCP_PACKET_TYPE_RR")
TRACE_MSG(RTCP_1697_112_2_17_23_21_17_926,"RTCP RECV RTCP_PACKET_TYPE_SDES")
TRACE_MSG(RTCP_1742_112_2_17_23_21_17_927,"RTCP RECV RTCP_PACKET_TYPE_BYE")
TRACE_MSG(RTCP_1755_112_2_17_23_21_17_928,"RTCP RECV RTCP_PACKET_TYPE_APP")
TRACE_MSG(RTCP_1777_112_2_17_23_21_17_929,"RTP:rtcpProcessRTCPPacket type error = %d")
TRACE_MSG(RTCP_1883_112_2_17_23_21_17_930,"RTCP RTCP_TimerCallback ip = %x,port =%x")
TRACE_MSG(RTP_222_112_2_17_23_21_19_931,"RTP:RTP_OpenFrom, open socket fail!")
TRACE_MSG(RTP_338_112_2_17_23_21_19_932,"RTP RTP_OpenEx bind so %x to %08x:%d error - %d")
TRACE_MSG(RTP_357_112_2_17_23_21_19_933,"RTP RTP_OpenEx bind so %x to %08x:%d error - %d")
TRACE_MSG(RTP_662_112_2_17_23_21_20_934,"RTP_Unpack timestamp =%x")
TRACE_MSG(RTP_735_112_2_17_23_21_20_935,"RTP RTP_Read remote ip =%x,remote port =%d")
TRACE_MSG(RTP_750_112_2_17_23_21_20_936,"RTP RTP_Read RTCP_SetRemoteAddress remote ip =%x,remote port =%d")
TRACE_MSG(RTP_828_112_2_17_23_21_20_937,"RTP RTP_ReadWithRemoteAddress ADDRESS IP =%x, PORT =%d,s->socket =%x,rvd_len = %d")
TRACE_MSG(RTP_TEST_332_112_2_17_23_21_22_938,"rtp test: get send rtp req")
TRACE_MSG(RTP_TEST_346_112_2_17_23_21_22_939,"rtp test: get send rtcp req")
TRACE_MSG(RTP_TEST_351_112_2_17_23_21_22_940,"rtp test: get send rtcp app req")
TRACE_MSG(RTP_TEST_357_112_2_17_23_21_22_941,"\n rtp test :get sr rr send cnf")
TRACE_MSG(RTP_TEST_362_112_2_17_23_21_22_942,"\n rtp test :get rtcp data send cnf")
TRACE_MSG(RTP_TEST_366_112_2_17_23_21_22_943,"\n rtp test :get rtcp sr/rr send ind")
TRACE_MSG(RTP_TEST_373_112_2_17_23_21_22_944,"\n rtp test :get rtcp sr/rr rcvd ind")
TRACE_MSG(RTP_TEST_378_112_2_17_23_21_22_945,"\n rtp test :get rtcp sr/rr data rcvd ind")
TRACE_MSG(RTP_TEST_383_112_2_17_23_21_22_946,"\n rtp test :get rtcp bye rcvd ind")
TRACE_MSG(RTP_TEST_394_112_2_17_23_21_22_947,"\n rtp test :terminate rtp session\n")
TRACE_MSG(MAINAPP_RTP_1028_112_2_17_23_37_43_948,"StartIdleTimer: focus and flip!")
TRACE_MSG(MAINAPP_RTP_1048_112_2_17_23_37_43_949,"StartIdleTimer: the idle timer has started!")
TRACE_MSG(MAINAPP_RTP_1109_112_2_17_23_37_43_950,"StartIdleTimer: the idle timer has stoped!")
TRACE_MSG(MAINAPP_RTP_1155_112_2_17_23_37_43_951,"MAIN_SetIdleBlueToothState: the blue tooth status is same!")
TRACE_MSG(MAINAPP_RTP_1182_112_2_17_23_37_43_952,"MAIN_SetIdleBlueToothState: the blue tooth status is same!")
TRACE_MSG(MAINAPP_RTP_1223_112_2_17_23_37_43_953,"MAIN_SetIdleNetWorkName: the name id is %d!")
TRACE_MSG(MAINAPP_RTP_1252_112_2_17_23_37_43_954,"MAIN_SetIdleServiceProvidorName: tha name len = %d, is ucs2 = %d")
TRACE_MSG(MAINAPP_RTP_1383_112_2_17_23_37_44_955,"MAIN_SetIdleChargeState: the is_charge is same!")
TRACE_MSG(MAINAPP_RTP_1477_112_2_17_23_37_44_956,"MAIN_SetIdleSmsState: the sms_state =%d is same!")
TRACE_MSG(MAINAPP_RTP_1512_112_2_17_23_37_44_957,"MAIN_SetIdleCallState: the call_state is same!")
TRACE_MSG(MAINAPP_RTP_1525_112_2_17_23_37_44_958,"MAIN_SetIdleGprsState: gprs_state = %d")
TRACE_MSG(MAINAPP_RTP_1829_112_2_17_23_37_44_959,"MAIN_HandleSysMsg: begin MISC_MSG_RTC_MIN!")
TRACE_MSG(MAINAPP_RTP_1909_112_2_17_23_37_45_960,"MAIN_HandleSysMsg: can't handle the msg!")
TRACE_MSG(MAINAPP_RTP_1972_112_2_17_23_37_45_961,"DrawFmName s_fm_rect.left= %d")
TRACE_MSG(MAINAPP_RTP_1973_112_2_17_23_37_45_962,"DrawFmName s_fm_rect.right= %d")
TRACE_MSG(MAINAPP_RTP_1974_112_2_17_23_37_45_963,"DrawFmName s_fm_rect.top= %d")
TRACE_MSG(MAINAPP_RTP_1975_112_2_17_23_37_45_964,"DrawFmName s_fm_rect.bottom= %d")
TRACE_MSG(MAINAPP_RTP_2041_112_2_17_23_37_45_965,"[MMIDC]: IdleWin_HandleMsg, MSG_OPEN_WINDOW")
TRACE_MSG(MAINAPP_RTP_2089_112_2_17_23_37_45_966,"IdleWin_HandleMsg MSG_FULL_PAINT")
TRACE_MSG(MAINAPP_RTP_2134_112_2_17_23_37_45_967,"IDLEWIN_HandleMsg: handle MSG_IDLE_UPDATE_DATETIME!")
TRACE_MSG(MAINAPP_RTP_2144_112_2_17_23_37_45_968,"IdleWin_HandleMsg: MSG_IDLE_UPDATE_NETWORK!")
TRACE_MSG(MAINAPP_RTP_2167_112_2_17_23_37_45_969,"IdleWin_HandleMsg: MSG_IDLE_UPDATE_MP3!")
TRACE_MSG(MAINAPP_RTP_2454_112_2_17_23_37_46_970,"IDLEWIN_HandleWinMsg: handle headset MSG!")
TRACE_MSG(MAINAPP_RTP_2470_112_2_17_23_37_46_971,"IdleWin_HandleMsg:received timer msg!")
TRACE_MSG(MAINAPP_RTP_2477_112_2_17_23_37_46_972,"IdleWin_HandleMsg: handle the anim wallpater timer!")
TRACE_MSG(MAINAPP_RTP_2508_112_2_17_23_37_46_973,"IdleWin_HandleMsg: MMK_GetFocusWinId() is =%d")
TRACE_MSG(MAINAPP_RTP_2650_112_2_17_23_37_46_974,"MMI: IdleWin_HandleMsg, received tp msg_id ")
TRACE_MSG(MAINAPP_RTP_2657_112_2_17_23_37_46_975,"IdleWin_HandleMsg: point.x=%d")
TRACE_MSG(MAINAPP_RTP_2658_112_2_17_23_37_46_976,"IdleWin_HandleMsg: point.y=%d")
TRACE_MSG(MAINAPP_RTP_2659_112_2_17_23_37_46_977,"IdleWin_HandleMsg s_mp3_rect.left= %d")
TRACE_MSG(MAINAPP_RTP_2660_112_2_17_23_37_46_978,"IdleWin_HandleMsg s_mp3_rect.right= %d")
TRACE_MSG(MAINAPP_RTP_2661_112_2_17_23_37_46_979,"IdleWin_HandleMsg s_mp3_rect.top= %d")
TRACE_MSG(MAINAPP_RTP_2662_112_2_17_23_37_46_980,"IdleWin_HandleMsg s_mp3_rect.bottom= %d")
TRACE_MSG(MAINAPP_RTP_2663_112_2_17_23_37_46_981,"IdleWin_HandleMsg s_idlewin_info.is_display_mp3= %d")
TRACE_MSG(MAINAPP_RTP_2688_112_2_17_23_37_46_982,"IdleWin_HandleMsg open mp3 player!")
TRACE_MSG(MAINAPP_RTP_2721_112_2_17_23_37_46_983,"IDLEWIN_HandleWinMsg: MSG_STK_SET_UP_IDLE_MODE")
TRACE_MSG(MAINAPP_RTP_2751_112_2_17_23_37_46_984,"MMI: IdleWin_HandleMsg, don't handle message, msg_id = %d")
TRACE_MSG(MAINAPP_RTP_2781_112_2_17_23_37_46_985,"OpenDialingNumberBock, enter")
TRACE_MSG(MAINAPP_RTP_2786_112_2_17_23_37_46_986,"[mainapp.c]: dialing number block have been opened")
TRACE_MSG(MAINAPP_RTP_3181_112_2_17_23_37_47_987,"kick the key VT")
TRACE_MSG(MAINAPP_RTP_3277_112_2_17_23_37_47_988,"FuncDialingWin_HandleMsg MSG_KEYLONG_HASH!")
TRACE_MSG(MAINAPP_RTP_3311_112_2_17_23_37_47_989,"FuncDialingWin_HandleMsg:result_info_ptr->protect_type %d is error!")
TRACE_MSG(MAINAPP_RTP_3332_112_2_17_23_37_47_990,"[mainapp.c]: close dialing number block")
TRACE_MSG(MAINAPP_RTP_3336_112_2_17_23_37_47_991,"[mainapp.c]: dialing number block have been closed!")
TRACE_MSG(MAINAPP_RTP_3344_112_2_17_23_37_48_992,"[mainapp.c]: BLOCK disable failure!")
TRACE_MSG(MAINAPP_RTP_3347_112_2_17_23_37_48_993," CloseDialingNumberBlock: close block successful!")
TRACE_MSG(MAINAPP_RTP_3369_112_2_17_23_37_48_994,"main_app_init:call_ring_type is %d!")
TRACE_MSG(MAINAPP_RTP_3464_112_2_17_23_37_48_995,"MAIN_StopIdleTimer: the sim error timer has stoped!")
TRACE_MSG(MAINAPP_RTP_3662_112_2_17_23_37_48_996,"OutputIdleWinWords idle_mode->len= %d")
TRACE_MSG(MAINAPP_RTP_3684_112_2_17_23_37_48_997,"OutputIdleWinWords s_words_rect.left= %d")
TRACE_MSG(MAINAPP_RTP_3685_112_2_17_23_37_48_998,"OutputIdleWinWords s_words_rect.right= %d")
TRACE_MSG(MAINAPP_RTP_3686_112_2_17_23_37_48_999,"OutputIdleWinWords s_words_rect.top= %d")
TRACE_MSG(MAINAPP_RTP_3687_112_2_17_23_37_48_1000,"OutputIdleWinWords s_words_rect.bottom= %d")
TRACE_MSG(MAINAPP_RTP_3743_112_2_17_23_37_48_1001,"OutputIdleWinNetworkName point.x= %d")
TRACE_MSG(MAINAPP_RTP_3744_112_2_17_23_37_48_1002,"OutputIdleWinNetworkName point.y= %d")
TRACE_MSG(MAINAPP_RTP_3769_112_2_17_23_37_48_1003,"OutputIdleWinNetworkName, is_get_cphs_info = %d")
TRACE_MSG(MAINAPP_RTP_3784_112_2_17_23_37_48_1004,"OutputIdleWinNetworkName s_network_rect.left= %d")
TRACE_MSG(MAINAPP_RTP_3785_112_2_17_23_37_48_1005,"OutputIdleWinNetworkName s_network_rect.right= %d")
TRACE_MSG(MAINAPP_RTP_3786_112_2_17_23_37_48_1006,"OutputIdleWinNetworkName s_network_rect.top= %d")
TRACE_MSG(MAINAPP_RTP_3787_112_2_17_23_37_48_1007,"OutputIdleWinNetworkName s_network_rect.bottom= %d")
TRACE_MSG(MAINAPP_RTP_3810_112_2_17_23_37_48_1008,"OutputIdleWinNetworkName, get the network name from sim card!")
TRACE_MSG(MAINAPP_RTP_3839_112_2_17_23_37_48_1009,"OutputIdleWinNetworkName s_network_rect.left= %d")
TRACE_MSG(MAINAPP_RTP_3840_112_2_17_23_37_48_1010,"OutputIdleWinNetworkName s_network_rect.right= %d")
TRACE_MSG(MAINAPP_RTP_3841_112_2_17_23_37_48_1011,"OutputIdleWinNetworkName s_network_rect.top= %d")
TRACE_MSG(MAINAPP_RTP_3842_112_2_17_23_37_49_1012,"OutputIdleWinNetworkName s_network_rect.bottom= %d")
TRACE_MSG(MAINAPP_RTP_3853_112_2_17_23_37_49_1013,"OutputIdleWinNetworkName, get the network name using the old method!")
TRACE_MSG(MAINAPP_RTP_4245_112_2_17_23_37_49_1014,"DisplayIdleMp3Name g_idle_cur_mp3_name.length= %d")
TRACE_MSG(MAINAPP_RTP_4246_112_2_17_23_37_49_1015,"DisplayIdleMp3Name g_idle_cur_mp3_name.is_ucs2= %d")
TRACE_MSG(MAINAPP_RTP_4254_112_2_17_23_37_49_1016,"DisplayIdleMp3Name string_width= %d")
TRACE_MSG(MAINAPP_RTP_4269_112_2_17_23_37_49_1017,"DisplayIdleMp3Name s_mp3_rect.left= %d")
TRACE_MSG(MAINAPP_RTP_4270_112_2_17_23_37_49_1018,"DisplayIdleMp3Name s_mp3_rect.right= %d")
TRACE_MSG(MAINAPP_RTP_4271_112_2_17_23_37_49_1019,"DisplayIdleMp3Name s_mp3_rect.top= %d")
TRACE_MSG(MAINAPP_RTP_4272_112_2_17_23_37_49_1020,"DisplayIdleMp3Name s_mp3_rect.bottom= %d")
TRACE_MSG(MAINAPP_RTP_4331_112_2_17_23_37_49_1021,"DisplayIdleCycleMp3Name s_mp3_rect.left= %d")
TRACE_MSG(MAINAPP_RTP_4332_112_2_17_23_37_49_1022,"DisplayIdleCycleMp3Name s_mp3_rect.right= %d")
TRACE_MSG(MAINAPP_RTP_4333_112_2_17_23_37_49_1023,"DisplayIdleCycleMp3Name s_mp3_rect.top= %d")
TRACE_MSG(MAINAPP_RTP_4334_112_2_17_23_37_49_1024,"DisplayIdleCycleMp3Name s_mp3_rect.bottom= %d")
TRACE_MSG(MAINAPP_RTP_4625_112_2_17_23_37_50_1025,"DisplayStatusBarIcon start MMI_WIN_ICON_GPRS is_gprs is %d")
TRACE_MSG(MAINAPP_RTP_4628_112_2_17_23_37_50_1026,"DisplayStatusBarIcon MMI_WIN_ICON_GPRS sim is OK!")
TRACE_MSG(MAINAPP_RTP_4642_112_2_17_23_37_50_1027,"DisplayStatusBarIcon image_id = IMAGE_IDLE_TITLE_ICON_GPRS_DEACTIVE")
TRACE_MSG(MAINAPP_RTP_4809_112_2_17_23_37_50_1028,"DisplayStatusBarIcon is charge: image_id=%d")
TRACE_MSG(MAINAPP_RTP_4816_112_2_17_23_37_50_1029,"DisplayStatusBarIcon not charge: image_id=%d")
TRACE_MSG(MAINAPP_RTP_4980_112_2_17_23_37_51_1030,"MAIN_SetIdleSmsState: the line_state is same!")
TRACE_MSG(MAINAPP_RTP_4997_112_2_17_23_37_51_1031,"MAIN_SetIdleSmsState: the voice message is same!")
TRACE_MSG(MAINAPP_RTP_5106_112_2_17_23_37_51_1032,"MMIMAIN_HandleIdelTimer: handle the idle icon timer!")
TRACE_MSG(MAINAPP_RTP_5179_112_2_17_23_37_51_1033,"MMI_GetFreeSpace: free_space = %d")
TRACE_MSG(MAINAPP_RTP_5341_112_2_17_23_37_51_1034,"Handle_HW_Icon_Exec_Func : s_cur_hw_icon_index = %d")
TRACE_MSG(MMIMAIN_RTP_235_112_2_17_23_37_53_1035,"KeypadCallBackFunc: the key message is too much!")
TRACE_MSG(MMIMAIN_RTP_263_112_2_17_23_37_53_1036,"TPCallBackFunc, P_TPP = %d")
TRACE_MSG(MMIMAIN_RTP_286_112_2_17_23_37_53_1037,"TPCallBackFunc: the TP message is too much!")
TRACE_MSG(MMIMAIN_RTP_411_112_2_17_23_37_53_1038,"Receive CHR_CAP_IND")
TRACE_MSG(MMIMAIN_RTP_417_112_2_17_23_37_53_1039,"Receive CHR_CHARGE_START_IND")
TRACE_MSG(MMIMAIN_RTP_423_112_2_17_23_37_53_1040,"Receive CHR_CHARGE_END_IND")
TRACE_MSG(MMIMAIN_RTP_429_112_2_17_23_37_53_1041,"Receive CHR_WARNING_IND")
TRACE_MSG(MMIMAIN_RTP_435_112_2_17_23_37_53_1042,"Receive CHR_SHUTDOWN_IND")
TRACE_MSG(MMIMAIN_RTP_441_112_2_17_23_37_53_1043,"Receive CHR_CHARGE_FINISH")
TRACE_MSG(MMIMAIN_RTP_447_112_2_17_23_37_53_1044,"Receive CHR_CHARGE_DISCONNECT")
TRACE_MSG(MMIMAIN_RTP_453_112_2_17_23_37_53_1045,"Receive CHR_CHARGE_FAULT")
TRACE_MSG(MMIMAIN_RTP_459_112_2_17_23_37_53_1046,"ChargeCallBackFunc:event_id = %d")
TRACE_MSG(MMIMAIN_RTP_741_112_2_17_23_37_54_1047,"APP_Task: begin init, time = %d")
TRACE_MSG(MMIMAIN_RTP_791_112_2_17_23_37_54_1048,"APP_Task: restart_condition is %d,time is %dms")
TRACE_MSG(MMIMAIN_RTP_890_112_2_17_23_37_54_1049,"APP_Task: end init, time = %d")
TRACE_MSG(MMIMAIN_RTP_918_112_2_17_23_37_54_1050,"--------RTP: received RTP_RTCP_SESSION_INIT_CNF!!!\n")
TRACE_MSG(MMIMAIN_RTP_922_112_2_17_23_37_54_1051,"--------RTP: received RTP_DATA_SEND_CNF!!!\n")
TRACE_MSG(MMIMAIN_RTP_928_112_2_17_23_37_54_1052,"--------RTP: received RTP_DATA_RCVD_IND!!!\n")
TRACE_MSG(MMIMAIN_RTP_936_112_2_17_23_37_54_1053,"--------RTP: received RTCP_SRRR_AUTO_SEND_SET_CNF!!!\n")
TRACE_MSG(MMIMAIN_RTP_967_112_2_17_23_37_54_1054,"SignalCode is %d")
TRACE_MSG(MMIMAIN_RTP_980_112_2_17_23_37_54_1055,"fdn is disable,start init adn")
TRACE_MSG(MMIMAIN_RTP_1017_112_2_17_23_37_54_1056,"Receive UPM_PLUGIN")
TRACE_MSG(MMIMAIN_RTP_1028_112_2_17_23_37_54_1057,"Receive UPM_PLUGOUT")
TRACE_MSG(MMIMAIN_RTP_1039_112_2_17_23_37_54_1058,"Receive UPM_READY")
TRACE_MSG(MMIMAIN_RTP_1050_112_2_17_23_37_54_1059,"Receive UPM_ERROR")
TRACE_MSG(MMIMAIN_RTP_1061_112_2_17_23_37_54_1060,"UPMCallBackFunc:event_id = %d")
TRACE_MSG(NETMGR_API_88_112_2_17_23_37_55_1061,"#NETMGR REG SIG ERROR, sig type:%d,pro type:%d")
TRACE_MSG(NETMGR_API_158_112_2_17_23_37_55_1062,"#NETMGR NETMGR_DealNextRequest error,host_ptr==0")
TRACE_MSG(NETMGR_API_163_112_2_17_23_37_55_1063,"#NETMGR NETMGR_DealNextRequest error,cur request==0")
TRACE_MSG(NETMGR_API_311_112_2_17_23_37_55_1064,"#NETMGR NETMGR_Redirect error host==0")
TRACE_MSG(NETMGR_API_329_112_2_17_23_37_55_1065,"#NETMGR NETMGR_Redirect url is wrong")
TRACE_MSG(NETMGR_API_532_112_2_17_23_37_55_1066,"#NETMGR NETMGR_SetProxyAuthenInfo error host==0")
TRACE_MSG(NETMGR_HOST_67_112_2_17_23_37_56_1067,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_HOST_87_112_2_17_23_37_56_1068,"#NETMGR HOST_CreatHost, host:%x")
TRACE_MSG(NETMGR_HOST_89_112_2_17_23_37_56_1069,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_HOST_94_112_2_17_23_37_56_1070,"#NETMGR HOST_CreatHost, pro host:%x")
TRACE_MSG(NETMGR_HOST_114_112_2_17_23_37_56_1071,"#NETMGR HOST_DestroyHost, host:%x, pro host:%x")
TRACE_MSG(NETMGR_HOST_117_112_2_17_23_37_56_1072,"#NETMGR HOST_DestroyHost, invalid host handler")
TRACE_MSG(NETMGR_INSTANCE_175_112_2_17_23_37_57_1073,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_199_112_2_17_23_37_57_1074,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_204_112_2_17_23_37_57_1075,"#NETMGR creat netmgr instance error:alloc instance mem failed!")
TRACE_MSG(NETMGR_INSTANCE_228_112_2_17_23_37_57_1076,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_232_112_2_17_23_37_57_1077,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_273_112_2_17_23_37_57_1078,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_277_112_2_17_23_37_57_1079,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_292_112_2_17_23_37_57_1080,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_296_112_2_17_23_37_57_1081,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_319_112_2_17_23_37_57_1082,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_324_112_2_17_23_37_57_1083,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_329_112_2_17_23_37_57_1084,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_349_112_2_17_23_37_57_1085,"#NETMGR MANAGER_DestroyInstance, invalid netmgr instance!")
TRACE_MSG(NETMGR_INSTANCE_357_112_2_17_23_37_57_1086,"#NETMGR MANAGER_DestroyInstance, set context_handle 0 !")
TRACE_MSG(NETMGR_INSTANCE_440_112_2_17_23_37_57_1087,"#NETMGR TASK ENTRY TRANS TIMER")
TRACE_MSG(NETMGR_INSTANCE_444_112_2_17_23_37_57_1088,"#NETMGR TASK ENTRY keep alive timer")
TRACE_MSG(NETMGR_INSTANCE_454_112_2_17_23_37_57_1089,"#NETMGR TASK ENTRY SOCK, EVENT:%d")
TRACE_MSG(NETMGR_INSTANCE_459_112_2_17_23_37_57_1090,"#NETMGR TASK ENTRY APP, EVENT:%d")
TRACE_MSG(NETMGR_INSTANCE_469_112_2_17_23_37_57_1091,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_499_112_2_17_23_37_57_1092,"#NETMGR HandleKeepAliveTimer")
TRACE_MSG(NETMGR_INSTANCE_573_112_2_17_23_37_57_1093,"#NETMGR handle app signal error:pro type:%d,sig_type:%d")
TRACE_MSG(NETMGR_INSTANCE_639_112_2_17_23_37_58_1094,"#NETMGR HandleAppInitSig,creat netmgr instance failed!!")
TRACE_MSG(NETMGR_INSTANCE_643_112_2_17_23_37_58_1095,"#NETMGR HandleAppInitSig,id_for_sockbind:%d")
TRACE_MSG(NETMGR_INSTANCE_678_112_2_17_23_37_58_1096,"#NETMGR HandleAppRequestSig, invalid netmgr instance!")
TRACE_MSG(NETMGR_INSTANCE_689_112_2_17_23_37_58_1097,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_INSTANCE_703_112_2_17_23_37_58_1098,"NETMGR HandleAppRequestSig, url is wrong")
TRACE_MSG(NETMGR_INSTANCE_814_112_2_17_23_37_58_1099,"#NETMGR HandleAppCancelSig,request_node_handler->request_ptr == 0!!!")
TRACE_MSG(NETMGR_INSTANCE_845_112_2_17_23_37_58_1100,"#NETMGR HandleAppCloseSig, invalid netmgr instance!")
TRACE_MSG(NETMGR_INSTANCE_1388_112_2_17_23_37_59_1101,"#NETMGR MANAGER_GetDefaultHostPort host=0!")
TRACE_MSG(NETMGR_INSTANCE_1395_112_2_17_23_37_59_1102,"#NETMGR MANAGER_GetDefaultHostPort netmgr_ptr=0!")
TRACE_MSG(NETMGR_INSTANCE_1403_112_2_17_23_37_59_1103,"#NETMGR MANAGER_GetDefaultHostPort net_request_ptr=0!")
TRACE_MSG(NETMGR_INSTANCE_1506_112_2_17_23_37_59_1104,"#NETMGR MANAGER_GetDefaultHostPort default port=%d!")
TRACE_MSG(NETMGR_REQUEST_62_112_2_17_23_37_59_1105,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_REQUEST_69_112_2_17_23_38_0_1106,"NETMGR REQUEST_CreatRequest url is wrong")
TRACE_MSG(NETMGR_REQUEST_84_112_2_17_23_38_0_1107,"#NETMGR REQUEST_CreatRequest:%x, pro request:%x")
TRACE_MSG(NETMGR_REQUEST_87_112_2_17_23_38_0_1108,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_REQUEST_109_112_2_17_23_38_0_1109,"#NETMGR REQUEST_DestroyRequest:%x, pro request:%x")
TRACE_MSG(NETMGR_REQUEST_113_112_2_17_23_38_0_1110,"#NETMGR REQUEST_DestroyRequest error host==0")
TRACE_MSG(NETMGR_TEST_50_112_2_17_23_38_0_1111,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_71_112_2_17_23_38_0_1112,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_74_112_2_17_23_38_0_1113,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_83_112_2_17_23_38_0_1114,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_97_112_2_17_23_38_0_1115,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_183_112_2_17_23_38_0_1116,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_206_112_2_17_23_38_0_1117,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_233_112_2_17_23_38_0_1118,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_276_112_2_17_23_38_1_1119,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_299_112_2_17_23_38_1_1120,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_318_112_2_17_23_38_1_1121,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_332_112_2_17_23_38_1_1122,"#NETMGR recv data:%s")
TRACE_MSG(NETMGR_TEST_APP_30_112_2_17_23_38_1_1123,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_APP_56_112_2_17_23_38_1_1124,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_APP_78_112_2_17_23_38_1_1125,"#NETMGR send confirm!!!")
TRACE_MSG(NETMGR_TEST_APP_85_112_2_17_23_38_1_1126,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_APP_99_112_2_17_23_38_1_1127,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_TEST_APP_111_112_2_17_23_38_1_1128,"#NETMGR TEST CLOSE!!")
TRACE_MSG(NETMGR_TEST_APP_114_112_2_17_23_38_1_1129,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_267_112_2_17_23_38_2_1130,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_319_112_2_17_23_38_2_1131,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_339_112_2_17_23_38_2_1132,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_359_112_2_17_23_38_2_1133,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_413_112_2_17_23_38_2_1134,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_495_112_2_17_23_38_2_1135,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_510_112_2_17_23_38_2_1136,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_572_112_2_17_23_38_2_1137,"URL_GetStringByRule, len = %d")
TRACE_MSG(NETMGR_URL_574_112_2_17_23_38_2_1138,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_677_112_2_17_23_38_2_1139,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_682_112_2_17_23_38_2_1140,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_687_112_2_17_23_38_2_1141,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_692_112_2_17_23_38_3_1142,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_697_112_2_17_23_38_3_1143,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_702_112_2_17_23_38_3_1144,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETMGR_URL_707_112_2_17_23_38_3_1145,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETTRANS_API_73_112_2_17_23_38_3_1146,"#NETMGR NETTRANS_Connect netmgr instance is null!")
TRACE_MSG(NETTRANS_API_192_112_2_17_23_38_3_1147,"#NETMGR NETTRANS_Disconnect netmgr instance is null!")
TRACE_MSG(NETTRANS_API_211_112_2_17_23_38_3_1148,"#NETMGR NETTRANS_Disconnect invalid connection!")
TRACE_MSG(NETTRANS_API_247_112_2_17_23_38_3_1149,"#NETMGR NETTRANS_Recv netmgr instance is null!")
TRACE_MSG(NETTRANS_API_301_112_2_17_23_38_3_1150,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETTRANS_API_351_112_2_17_23_38_4_1151,"#NETMGR trans recv alloc read signal error!")
TRACE_MSG(NETTRANS_API_366_112_2_17_23_38_4_1152,"#NETMGR trans recv alloc signal error!")
TRACE_MSG(NETTRANS_API_392_112_2_17_23_38_4_1153,"#NETMGR trans recv sci_sock_errno EWOULDBLOCK")
TRACE_MSG(NETTRANS_API_397_112_2_17_23_38_4_1154,"#NETMGR recv fail, err:%d")
TRACE_MSG(NETTRANS_API_446_112_2_17_23_38_4_1155,"#NETMGR NETTRANS_Send netmgr instance is null!")
TRACE_MSG(NETTRANS_API_503_112_2_17_23_38_4_1156,"#NETMGR NETTRANS_Send, invalid input!")
TRACE_MSG(NETTRANS_API_561_112_2_17_23_38_4_1157,"#NETMGR NETTRANS_SendFile netmgr instance is null!")
TRACE_MSG(NETTRANS_API_680_112_2_17_23_38_4_1158,"#NETMGR NETTRANS_FindHostConnectted netmgr instance is null!")
TRACE_MSG(NETTRANS_API_705_112_2_17_23_38_4_1159,"#NETMGR NETTRANS_FindHostDnsing netmgr instance is null!")
TRACE_MSG(NETTRANS_API_731_112_2_17_23_38_4_1160,"#NETMGR NETTRANS_FindHostConnecting netmgr instance is null!")
TRACE_MSG(NETTRANS_API_757_112_2_17_23_38_4_1161,"#NETMGR NETTRANS_HandleHostDestroy netmgr instance is null!")
TRACE_MSG(NETTRANS_CONNECTION_130_112_2_17_23_38_5_1162,"#NETMGR CONNECTION_DestroyConnection invalid connection in the indicated list")
TRACE_MSG(NETTRANS_CONNECTION_208_112_2_17_23_38_5_1163,"#NETMGR CONNECTION_GetConnectionBySocket invalid netmgr 1!")
TRACE_MSG(NETTRANS_CONNECTION_227_112_2_17_23_38_5_1164,"#NETMGR CONNECTION_GetConnectionBySocket invalid netmgr 2!")
TRACE_MSG(NETTRANS_CONNECTION_275_112_2_17_23_38_5_1165,"#NETMGR CONNECTION_GetConnectionByGethostId invalid netmgr 1!")
TRACE_MSG(NETTRANS_CONNECTION_294_112_2_17_23_38_5_1166,"#NETMGR CONNECTION_GetConnectionByGethostId invalid netmgr 2!")
TRACE_MSG(NETTRANS_CONNECTION_380_112_2_17_23_38_5_1167,"#NETMGR CONNECTION_CheckIfConnectionValid invalid connection_ptr!")
TRACE_MSG(NETTRANS_CONNECTION_391_112_2_17_23_38_5_1168,"#NETMGR CONNECTION_CheckIfConnectionValid invalid netmgr 1!")
TRACE_MSG(NETTRANS_CONNECTION_423_112_2_17_23_38_5_1169,"#NETMGR CONNECTION_CheckIfConnectionValid invalid netmgr 2!")
TRACE_MSG(NETTRANS_MSG_67_112_2_17_23_38_6_1170,"#NETMGR TRANS_HandleTimer invalid connection_ptr")
TRACE_MSG(NETTRANS_MSG_73_112_2_17_23_38_6_1171,"#NETMGR TRANS_HandleTimer, invalid connection node in connection_ptr!")
TRACE_MSG(NETTRANS_MSG_79_112_2_17_23_38_6_1172,"#NETMGR invalid timer arrived!")
TRACE_MSG(NETTRANS_MSG_84_112_2_17_23_38_6_1173,"#NETMGR none exist timer arrived!")
TRACE_MSG(NETTRANS_MSG_89_112_2_17_23_38_6_1174,"#NETMGR TRANS_HandleTimer, Bad connection!")
TRACE_MSG(NETTRANS_MSG_97_112_2_17_23_38_6_1175,"#NETMGR TRANS_HandleTimer, invalid netmanager_instance_ptr!")
TRACE_MSG(NETTRANS_MSG_102_112_2_17_23_38_6_1176,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_MSG_135_112_2_17_23_38_6_1177,"#NETMGR TRANS_HandleTimer, connection_ptr in connecting_list_node_handler is null!")
TRACE_MSG(NETTRANS_MSG_154_112_2_17_23_38_6_1178,"#NETMGR TRANS_HandleTimer NETTRANS_CONNECTION_STATE_DNSING, insert to connecting list fail")
TRACE_MSG(NETTRANS_MSG_231_112_2_17_23_38_6_1179,"#NETMGR socket event:SOCKET_READ_EVENT_IND")
TRACE_MSG(NETTRANS_MSG_245_112_2_17_23_38_6_1180,"#NETMGR socket event:SOCKET_READ_BUFFER_STATUS_EVENT_IND")
TRACE_MSG(NETTRANS_MSG_263_112_2_17_23_38_6_1181,"#NETMGR socket event:SOCKET_WRITE_EVENT_IND")
TRACE_MSG(NETTRANS_MSG_278_112_2_17_23_38_6_1182,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_MSG_321_112_2_17_23_38_6_1183,"#NETMGR socket event:SOCKET_CONNECT_EVENT_IND")
TRACE_MSG(NETTRANS_MSG_330_112_2_17_23_38_6_1184,"#NETMGR TRANS_HandleSocketSig, invalid connection_ptr!")
TRACE_MSG(NETTRANS_MSG_376_112_2_17_23_38_7_1185,"#NETMGR socket event:SOCKET_CONNECTION_CLOSE_EVENT_IND")
TRACE_MSG(NETTRANS_MSG_405_112_2_17_23_38_7_1186,"#NETMGR socket event:SOCKET_ASYNC_GETHOSTBYNAME_CNF")
TRACE_MSG(NETTRANS_MSG_414_112_2_17_23_38_7_1187,"#NETMGR socket event:SOCKET_ASYNC_GETHOSTBYNAME_CNF, invalid connection")
TRACE_MSG(NETTRANS_MSG_440_112_2_17_23_38_7_1188,"#NETMGR TRANS_HandleSocketSig, invalid new_connection_ptr!")
TRACE_MSG(NETTRANS_MSG_461_112_2_17_23_38_7_1189,"#NETMGR TRANS_HandleTimer NETTRANS_CONNECTION_STATE_DNSING, insert to connecting list fail")
TRACE_MSG(NETTRANS_MSG_482_112_2_17_23_38_7_1190,"#NETMGR socket event: unknown socket event %d")
TRACE_MSG(NETTRANS_MSG_508_112_2_17_23_38_7_1191,"#NETMGR TRANS_Callback, host_handle==0!")
TRACE_MSG(NETTRANS_MSG_536_112_2_17_23_38_7_1192,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_MSG_573_112_2_17_23_38_7_1193,"#NETMGR TRANS_SendReport, host_handle==0!")
TRACE_MSG(NETTRANS_MSG_578_112_2_17_23_38_7_1194,"#NETMGR TRANS_SendReport, netmanager_instance_ptr==0!")
TRACE_MSG(NETTRANS_MSG_611_112_2_17_23_38_7_1195,"#NETMGR TRANS_HandleDnsInd, netmanager_instance_ptr==0!")
TRACE_MSG(NETTRANS_MSG_629_112_2_17_23_38_7_1196,"#NETMGR TRANS_HandleDnsInd, connection_ptr==0!")
TRACE_MSG(NETTRANS_SOCK_68_112_2_17_23_38_7_1197,"#NETMGR SOCK_GetHostByName connection_ptr->host_handle == 0!")
TRACE_MSG(NETTRANS_SOCK_73_112_2_17_23_38_7_1198,"#NETMGR SOCK_GetHostByName host name == 0!")
TRACE_MSG(NETTRANS_SOCK_88_112_2_17_23_38_7_1199,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_100_112_2_17_23_38_8_1200,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_115_112_2_17_23_38_8_1201,"CFL TIMER CREAT, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_128_112_2_17_23_38_8_1202,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_155_112_2_17_23_38_8_1203,"#NETMGR SOCK_GetHostByNameAsync,ERR input")
TRACE_MSG(NETTRANS_SOCK_161_112_2_17_23_38_8_1204,"#NETMGR SOCK_GetHostByNameAsync,ERR connection")
TRACE_MSG(NETTRANS_SOCK_166_112_2_17_23_38_8_1205,"#NETMGR SOCK_GetHostByNameAsync,ERR host_ptr")
TRACE_MSG(NETTRANS_SOCK_172_112_2_17_23_38_8_1206,"#NETMGR SOCK_GetHostByNameAsync,ERR netmgr_ptr")
TRACE_MSG(NETTRANS_SOCK_182_112_2_17_23_38_8_1207,"#NETMGR SOCK_GetHostByNameAsync,ERR sci_async_gethostbyname")
TRACE_MSG(NETTRANS_SOCK_210_112_2_17_23_38_8_1208,"#NETMGR SOCK_Connect, connection_ptr == 0!")
TRACE_MSG(NETTRANS_SOCK_217_112_2_17_23_38_8_1209,"#NETMGR SOCK_Connect, host_ptr==0!")
TRACE_MSG(NETTRANS_SOCK_244_112_2_17_23_38_8_1210,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_269_112_2_17_23_38_8_1211,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_306_112_2_17_23_38_8_1212,"CFL TIMER CREAT, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_331_112_2_17_23_38_8_1213,"#NETMGR SOCK_Creat err, host=0!")
TRACE_MSG(NETTRANS_SOCK_337_112_2_17_23_38_8_1214,"#NETMGR SOCK_Creat err, netmgr=0!")
TRACE_MSG(NETTRANS_SOCK_358_112_2_17_23_38_8_1215,"#NETMGR SOCK_Creat tcp fail")
TRACE_MSG(NETTRANS_SOCK_378_112_2_17_23_38_8_1216,"#NETMGR SOCK_Creat udp fail")
TRACE_MSG(NETTRANS_SOCK_404_112_2_17_23_38_8_1217,"#NETMGR SOCK_Send, connection_ptr==0!")
TRACE_MSG(NETTRANS_SOCK_409_112_2_17_23_38_8_1218,"#NETMGR SOCK_Send, socket id==0!")
TRACE_MSG(NETTRANS_SOCK_414_112_2_17_23_38_8_1219,"#NETMGR SOCK_Send, send buffer==0!")
TRACE_MSG(NETTRANS_SOCK_421_112_2_17_23_38_8_1220,"NETMGR SOCK_Send connection_ptr->socket_id %x mss = %d")
TRACE_MSG(NETTRANS_SOCK_463_112_2_17_23_38_8_1221,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_496_112_2_17_23_38_8_1222,"CFL TIMER CREAT, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_500_112_2_17_23_38_8_1223,"#NETMGR trans send buffer, socket ENOBUFS!")
TRACE_MSG(NETTRANS_SOCK_513_112_2_17_23_38_8_1224,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_545_112_2_17_23_38_9_1225,"#NETMGR SOCK_SendByUdp, connection_ptr==0!")
TRACE_MSG(NETTRANS_SOCK_550_112_2_17_23_38_9_1226,"#NETMGR SOCK_SendByUdp, socket id==0!")
TRACE_MSG(NETTRANS_SOCK_555_112_2_17_23_38_9_1227,"#NETMGR SOCK_SendByUdp, send buffer==0!")
TRACE_MSG(NETTRANS_SOCK_600_112_2_17_23_38_9_1228,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_633_112_2_17_23_38_9_1229,"CFL TIMER CREAT, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_636_112_2_17_23_38_9_1230,"#NETMGR trans send buffer, socket ENOBUFS!")
TRACE_MSG(NETTRANS_SOCK_650_112_2_17_23_38_9_1231,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_686_112_2_17_23_38_9_1232,"#NETMGR SOCK_SendFile, invalid connection_ptr!")
TRACE_MSG(NETTRANS_SOCK_692_112_2_17_23_38_9_1233,"#NETMGR SOCK_SendFile, invalid send file param!")
TRACE_MSG(NETTRANS_SOCK_706_112_2_17_23_38_9_1234,"NETMGR SOCK_SendFile connection_ptr->socket_id %x mss = %d")
TRACE_MSG(NETTRANS_SOCK_740_112_2_17_23_38_9_1235,"CFL MEM ALLOC buf:%x, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_772_112_2_17_23_38_9_1236,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_804_112_2_17_23_38_9_1237,"CFL TIMER CREAT, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_809_112_2_17_23_38_9_1238,"#NETMGR trans send buffer, socket ENOBUFS!")
TRACE_MSG(NETTRANS_SOCK_814_112_2_17_23_38_9_1239,"CFL MEM FREE buf:%x, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_829_112_2_17_23_38_9_1240,"CFL TIMER DELETE, file:%s, line:%d")
TRACE_MSG(NETTRANS_SOCK_910_112_2_17_23_38_9_1241,"#NETMGR trans recv relloc error!")
TRACE_MSG(NETTRANS_SOCK_947_112_2_17_23_38_9_1242,"#NETMGR SOCK_UdpGetHostByName, connection_ptr == 0!")
TRACE_MSG(NETTRANS_SOCK_953_112_2_17_23_38_9_1243,"#NETMGR SOCK_UdpGetHostByName, host_ptr == 0!")
TRACE_MSG(NETTRANS_SOCK_958_112_2_17_23_38_9_1244,"#NETMGR SOCK_UdpGetHostByName, netmanager_instance_ptr == 0!")
TRACE_MSG(DD_XML_HANDLER_322_112_2_17_23_38_11_1245,"\nODM: Download description has more 'type' than system support(%d)")
TRACE_MSG(DD_XML_HANDLER_413_112_2_17_23_38_11_1246,"Invalid parameter")
TRACE_MSG(HTTP_INTERFACE_366_112_2_17_23_38_19_1247,"ODM:receive http error ind!")
TRACE_MSG(HTTP_INTERFACE_370_112_2_17_23_38_19_1248,"ODM:receive http error ind! result = %d")
TRACE_MSG(ODM_399_112_2_17_23_38_21_1249,"ODM_StartDownload: length=%d,is use proxy = %d")
TRACE_MSG(ODM_402_112_2_17_23_38_21_1250,"ODM_StartDownload: proxy host=%s\n")
TRACE_MSG(ODM_428_112_2_17_23_38_21_1251,"\n ODM_StartDownload error_code = %d")
TRACE_MSG(ODM_468_112_2_17_23_38_21_1252,"\n ODM_CancelDownload error_code = %d")
TRACE_MSG(ODM_508_112_2_17_23_38_21_1253,"\n ODM_DownloadConfirmationResponse error_code = %d")
TRACE_MSG(ODM_548_112_2_17_23_38_21_1254,"\n ODM_AuthResponse error_code = %d")
TRACE_MSG(ODM_580_112_2_17_23_38_21_1255,"\n ODM_GetSessionInfo error_code = %d")
TRACE_MSG(ODM_611_112_2_17_23_38_21_1256,"\n ODM_SubscribeIndication error_code = %d")
TRACE_MSG(ODM_642_112_2_17_23_38_21_1257,"\n ODM_DesubscribeIndication error_code = %d")
TRACE_MSG(OMADL_SERIALIZE_126_112_2_17_23_38_24_1258,"\n OMADL OMADL_SerializeStartDownloadReqSigT error")
TRACE_MSG(OMADL_UNITTEST_276_112_2_17_23_38_25_1259,"\nODMUT: Receive signal code: %d")
TRACE_MSG(OMADL_UNITTEST_279_112_2_17_23_38_25_1260,"\nODM: Receive an signal, but do not know who sent it")
TRACE_MSG(OMADL_UNITTEST_294_112_2_17_23_38_25_1261,"\nThrow ODM_UT_TEST_CASE message because we are testing")
TRACE_MSG(OMADL_UNITTEST_300_112_2_17_23_38_25_1262,"\nODMUT: ODM_START_DOWNLOAD_CNF")
TRACE_MSG(OMADL_UNITTEST_301_112_2_17_23_38_25_1263,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_302_112_2_17_23_38_25_1264,"\nMagic: %08x")
TRACE_MSG(OMADL_UNITTEST_303_112_2_17_23_38_25_1265,"\nResult: %08x")
TRACE_MSG(OMADL_UNITTEST_308_112_2_17_23_38_25_1266,"\nODMUT: ODM_CANCEL_DOWNLOAD_CNF")
TRACE_MSG(OMADL_UNITTEST_309_112_2_17_23_38_25_1267,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_310_112_2_17_23_38_25_1268,"\nResult: %08x")
TRACE_MSG(OMADL_UNITTEST_315_112_2_17_23_38_26_1269,"\nODMUT: ODM_DOWNLOAD_CONFIRMATION_IND")
TRACE_MSG(OMADL_UNITTEST_316_112_2_17_23_38_26_1270,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_336_112_2_17_23_38_26_1271,"\nODMUT: ODM_DOWNLOAD_AUTH_IND")
TRACE_MSG(OMADL_UNITTEST_337_112_2_17_23_38_26_1272,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_338_112_2_17_23_38_26_1273,"\nAuth type: %08x")
TRACE_MSG(OMADL_UNITTEST_353_112_2_17_23_38_26_1274,"\n:)CASE NO AUTH FAILED\n")
TRACE_MSG(OMADL_UNITTEST_366_112_2_17_23_38_26_1275,"\nODMUT: ODM_DOWNLOAD_PROGRESS_IND")
TRACE_MSG(OMADL_UNITTEST_367_112_2_17_23_38_26_1276,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_368_112_2_17_23_38_26_1277,"\nStage: %d")
TRACE_MSG(OMADL_UNITTEST_369_112_2_17_23_38_26_1278,"\nParam: %d\n")
TRACE_MSG(OMADL_UNITTEST_374_112_2_17_23_38_26_1279,"\nODMUT: ODM_DOWNLOAD_SUCCESS_IND")
TRACE_MSG(OMADL_UNITTEST_375_112_2_17_23_38_26_1280,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_387_112_2_17_23_38_26_1281,"\nDUAUT test finish:%d:%d\n")
TRACE_MSG(OMADL_UNITTEST_402_112_2_17_23_38_26_1282,"\nODMUT: ODM_DOWNLOAD_ERROR_IND")
TRACE_MSG(OMADL_UNITTEST_403_112_2_17_23_38_26_1283,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_404_112_2_17_23_38_26_1284,"\nError no: %d")
TRACE_MSG(OMADL_UNITTEST_415_112_2_17_23_38_26_1285,"\nDUAUT test finish:%d:%d\n")
TRACE_MSG(OMADL_UNITTEST_429_112_2_17_23_38_26_1286,"\nODMUT: ODM_DOWNLOAD_GET_SESSION_LIST_CNF")
TRACE_MSG(OMADL_UNITTEST_431_112_2_17_23_38_26_1287,"\nCount = %d")
TRACE_MSG(OMADL_UNITTEST_433_112_2_17_23_38_26_1288,"\nSessionID[%d] = %d")
TRACE_MSG(OMADL_UNITTEST_460_112_2_17_23_38_26_1289,"\nODMUT: ODM_DOWNLOAD_GET_SESSION_INFO_CNF")
TRACE_MSG(OMADL_UNITTEST_462_112_2_17_23_38_26_1290,"\ndd url: %s")
TRACE_MSG(OMADL_UNITTEST_466_112_2_17_23_38_26_1291,"\nODMUT: ODM_DOWNLOAD_GET_SESSION_INFO_CNF")
TRACE_MSG(OMADL_UNITTEST_468_112_2_17_23_38_26_1292,"\nResult: %08x")
TRACE_MSG(OMADL_UNITTEST_481_112_2_17_23_38_26_1293,"\nODMUT: ODM_DESUBSCRIBE_INDICATION_CNF")
TRACE_MSG(OMADL_UNITTEST_482_112_2_17_23_38_26_1294,"\nResult: %08x")
TRACE_MSG(OMADL_UNITTEST_488_112_2_17_23_38_26_1295,"\nODMUT: ODM_MANAGER_PROGRESS_IND")
TRACE_MSG(OMADL_UNITTEST_489_112_2_17_23_38_26_1296,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_490_112_2_17_23_38_26_1297,"\nStage: %d")
TRACE_MSG(OMADL_UNITTEST_491_112_2_17_23_38_26_1298,"\nParam: %d\n")
TRACE_MSG(OMADL_UNITTEST_496_112_2_17_23_38_26_1299,"\nODMUT: ODM_MANAGER_SUCCESS_IND")
TRACE_MSG(OMADL_UNITTEST_497_112_2_17_23_38_26_1300,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_502_112_2_17_23_38_26_1301,"\nODMUT: ODM_MANAGER_ERROR_IND")
TRACE_MSG(OMADL_UNITTEST_503_112_2_17_23_38_26_1302,"\nSession id: %08x")
TRACE_MSG(OMADL_UNITTEST_504_112_2_17_23_38_26_1303,"\nError no: %d")
TRACE_MSG(OMADL_UNITTEST_532_112_2_17_23_38_26_1304,"\nODMUT1: ProcAppUTTestSignal()")
TRACE_MSG(OMADL_UNITTEST_533_112_2_17_23_38_26_1305,"\n case_no: %d")
TRACE_MSG(OMADL_UNITTEST_534_112_2_17_23_38_26_1306,"\n param: %d")
TRACE_MSG(OMADL_UNITTEST_749_112_2_17_23_38_27_1307,"\nODM UNIT TEST:%d %d\n\n")
TRACE_MSG(OMADL_UNITTEST_768_112_2_17_23_38_27_1308,"ODMUT_StartTestOneCase:\n")
TRACE_MSG(OMADL_UTIL_411_112_2_17_23_38_28_1309,"\nODM MEMORY TRACE START:\n")
TRACE_MSG(OMADL_UTIL_415_112_2_17_23_38_28_1310,"(%d)Leak at: %s(%d)\n")
TRACE_MSG(OMADL_UTIL_419_112_2_17_23_38_28_1311,"\nODM MEMORY LEAK: %d\n")
TRACE_MSG(OMADL_UTIL_420_112_2_17_23_38_28_1312,"\nODM MEMORY TRACE END:\n")
TRACE_MSG(WSP_INTERFACE_308_112_2_17_23_38_28_1313,"\n ODM wsp data ind file write error\n")
TRACE_MSG(WSP_INTERFACE_381_112_2_17_23_38_29_1314,"ODM:receive wsp error ind!")
TRACE_MSG(WSP_INTERFACE_385_112_2_17_23_38_29_1315,"ODM:receive wsp error ind! result = %d")
TRACE_MSG(WSP_INTERFACE_646_112_2_17_23_38_29_1316,"\n ODM:wsp get document file open error\n")
TRACE_MSG(DRM_API_75_112_2_17_23_38_30_1317,"[DRM] Command is: DRM_InstallContentReq download name =%s,new name=%sn")
TRACE_MSG(DRM_API_109_112_2_17_23_38_30_1318,"[DRM] Command is: DRM_InstallContentReq ret=%d\n")
TRACE_MSG(DRM_API_132_112_2_17_23_38_30_1319,"[DRM] Command is: DRM_InstallRightsReq rights=%s\n")
TRACE_MSG(DRM_API_166_112_2_17_23_38_30_1320,"[DRM] Command is: DRM_InstallRightsReq ret=%d\n")
TRACE_MSG(DRM_API_198_112_2_17_23_38_30_1321,"[DRM] DRM_InstallFileReq DRM_INVALID_TPYE\n")
TRACE_MSG(DRM_API_204_112_2_17_23_38_30_1322,"[DRM] DRM_InstallFileReq ret_code=%d\n")
TRACE_MSG(DRM_API_250_112_2_17_23_38_30_1323,"[DRM] Command is: DRM_GetContentMimeTypeReq ret=%d\n")
TRACE_MSG(DRM_API_281_112_2_17_23_38_30_1324,"[DRM] Command is: DRM_IsDRMFile ret=%d\n")
TRACE_MSG(DRM_API_309_112_2_17_23_38_30_1325,"[DRM] Command is: DRM_GetDRMFileType ret=%d\n")
TRACE_MSG(DRM_API_324_112_2_17_23_38_31_1326,"[DRM] Command DRM_IsRightsExpired handle=%d permission =%d\n")
TRACE_MSG(DRM_API_341_112_2_17_23_38_31_1327,"[DRM] Command is: DRM_IsRightsExpired ret=%d\n")
TRACE_MSG(DRM_API_371_112_2_17_23_38_31_1328,"[DRM] Command is: DRM_GetRightsConstraint ret=%d\n")
TRACE_MSG(DRM_API_394_112_2_17_23_38_31_1329,"[DRM] Command is: DRM_GetRightsInfo ret=%d\n")
TRACE_MSG(DRM_API_418_112_2_17_23_38_31_1330,"[DRM] Command is: DRM_ConsumeRights ret=%d\n")
TRACE_MSG(DRM_API_507_112_2_17_23_38_31_1331,"[DRM] Command is: DRM_GetRightsInterval ret=%d\n")
TRACE_MSG(DRM_API_549_112_2_17_23_38_31_1332,"[DRM] Command is: DRM_DeleteRightsHandleReq ret=%d\n")
TRACE_MSG(DRM_API_585_112_2_17_23_38_31_1333,"[DRM] Command is: DRM_CancelInstallFileReq ret=%d\n")
TRACE_MSG(DRM_COMMON_230_112_2_17_23_38_32_1334,"\n[DRM] DRM_GetNetTime !is_net_time =%u,net_time= %u handle_type_ptr=%u\n")
TRACE_MSG(DRM_COMMON_249_112_2_17_23_38_32_1335,"DayToMonth:error day = %d\n")
TRACE_MSG(DRM_COMMON_761_112_2_17_23_38_33_1336,"\n[DRM] DRM_GetRightsIMEI is_suc=%d\n")
TRACE_MSG(DRM_COMMON_812_112_2_17_23_38_33_1337,"\n[DRM] DRM_CheckDiskIsNoSpace ret=%d\n")
TRACE_MSG(DRM_COMMON_869_112_2_17_23_38_33_1338,"DRM_IRAM_Enable:lock IRAM ERROR\n")
TRACE_MSG(DRM_COMMON_878_112_2_17_23_38_33_1339,"DRM_IRAM_Enable:lock IRAM ret =%d\n")
TRACE_MSG(DRM_COMMON_919_112_2_17_23_38_33_1340,"DRM_IRAM_Disable:ERROR \n")
TRACE_MSG(DRM_COMMON_925_112_2_17_23_38_33_1341,"DRM_IRAM_Disable:unable to unlock IRAM ret =%d\n")
TRACE_MSG(DRM_COMMON_946_112_2_17_23_38_33_1342,"DRM_IRAM_Init:INIT IRAM \n")
TRACE_MSG(DRM_COMMON_960_112_2_17_23_38_33_1343,"DRM_IRAM_Add_Count cout =%d \n")
TRACE_MSG(DRM_COMMON_975_112_2_17_23_38_33_1344,"DRM_IRAM_Reduce_Count cout =%d \n")
TRACE_MSG(DRM_COMMON_996_112_2_17_23_38_33_1345,"DRM_IRAM_CHECK_IS_Ulock_Iram cout =%d \n")
TRACE_MSG(DRM_DH_559_112_2_17_23_38_35_1346,"[DRM] Command is: DRM_SIG_DH_INSTALL_FILE_REQ\n")
TRACE_MSG(DRM_DH_563_112_2_17_23_38_35_1347,"[DRM] Command is: DRM_SIG_DH_DELETE_RIGHTS_FILE_REQ\n")
TRACE_MSG(DRM_DH_567_112_2_17_23_38_35_1348,"[DRM] Command is: DRM_SIG_GET_FILE_MIME_REQ\n")
TRACE_MSG(DRM_DH_571_112_2_17_23_38_35_1349,"[DRM] Command is: DRM_SIG_CANCEL_INSTALL_FILE_REQ\n")
TRACE_MSG(DRM_DH_575_112_2_17_23_38_35_1350,"[DRM] Command is: DRM_SIG_DEFAULT code=%d\n")
TRACE_MSG(DRM_DH_580_112_2_17_23_38_35_1351,"[DRM] DrmDLHandlerMain ret_code = %d\n")
TRACE_MSG(DRM_DH_624_112_2_17_23_38_35_1352,"[DRM] DeleteRightsFileByContentId FL_DeleteFile sfs ret =%d\n")
TRACE_MSG(DRM_DH_634_112_2_17_23_38_35_1353,"[DRM] DeleteRightsFileByContentId ret=%d\n")
TRACE_MSG(DRM_DH_784_112_2_17_23_38_35_1354,"[DRM] IsDCFFormat error_code=%d is_it_dcf=%d\n")
TRACE_MSG(DRM_DH_812_112_2_17_23_38_35_1355,"[DRM] ParseDCF")
TRACE_MSG(DRM_DH_843_112_2_17_23_38_35_1356,"[DRM] Mime type: %s\n")
TRACE_MSG(DRM_DH_855_112_2_17_23_38_36_1357,"[DRM] Content id: %s\n")
TRACE_MSG(DRM_DH_913_112_2_17_23_38_36_1358,"[DRM] Rights issuer: %s\n")
TRACE_MSG(DRM_DH_929_112_2_17_23_38_36_1359,"[DRM] Rights issuer: %s\n")
TRACE_MSG(DRM_DH_947_112_2_17_23_38_36_1360,"[DRM] Offset: %d, data length: %d\n")
TRACE_MSG(DRM_DH_951_112_2_17_23_38_36_1361,"[DRM] ParseDCF ret code: %d\n")
TRACE_MSG(DRM_DH_1035_112_2_17_23_38_36_1362,"[DRM] OpenDrmEntityInfoFile start\n")
TRACE_MSG(DRM_DH_1083_112_2_17_23_38_36_1363,"[DRM] OpenDrmEntityInfoFile handle: %0x\n")
TRACE_MSG(DRM_DH_1127_112_2_17_23_38_36_1364,"[DRM] SaveDrmEntityInfoToFile: result %d\n")
TRACE_MSG(DRM_DH_1152_112_2_17_23_38_36_1365,"[DRM] LoadDrmEntityInfoFromFile no memery!\n")
TRACE_MSG(DRM_DH_1198_112_2_17_23_38_36_1366,"[DRM] LoadDrmEntityInfoFromFile: DRM_STATUS_RIGHTS_PRESENT\n")
TRACE_MSG(DRM_DH_1236_112_2_17_23_38_36_1367,"[DRM] LoadDrmEntityInfoFromFile: ret =%d\n")
TRACE_MSG(DRM_DH_1252_112_2_17_23_38_36_1368,"[DRM] Count: %d\n")
TRACE_MSG(DRM_DH_1253_112_2_17_23_38_36_1369,"[DRM] Used count: %d\n")
TRACE_MSG(DRM_DH_1258_112_2_17_23_38_36_1370,"[DRM] Interval: %d\n")
TRACE_MSG(DRM_DH_1259_112_2_17_23_38_36_1371,"[DRM] Start Interval: %d\n")
TRACE_MSG(DRM_DH_1260_112_2_17_23_38_36_1372,"[DRM] End Interval: %d\n")
TRACE_MSG(DRM_DH_1265_112_2_17_23_38_36_1373,"[DRM] Start time: %d\n")
TRACE_MSG(DRM_DH_1266_112_2_17_23_38_36_1374,"[DRM] End   time: %d\n")
TRACE_MSG(DRM_DH_1281_112_2_17_23_38_36_1375,"\n[DRM] Content id: %s\n")
TRACE_MSG(DRM_DH_1282_112_2_17_23_38_36_1376,"[DRM] Rights issuer: %s\n")
TRACE_MSG(DRM_DH_1283_112_2_17_23_38_36_1377,"[DRM] Mime type: %d\n")
TRACE_MSG(DRM_DH_1284_112_2_17_23_38_36_1378,"[DRM] Status: %d\n")
TRACE_MSG(DRM_DH_1285_112_2_17_23_38_36_1379,"[DRM] Mehod Level:%d\n")
TRACE_MSG(DRM_DH_1294_112_2_17_23_38_36_1380,"[DRM]  permission\n")
TRACE_MSG(DRM_DH_1300_112_2_17_23_38_36_1381,"[DRM] No permission info\n")
TRACE_MSG(DRM_DH_1331_112_2_17_23_38_36_1382,"[DRM] ParseDrmMsg\n")
TRACE_MSG(DRM_DH_1382_112_2_17_23_38_37_1383,"[DRM] Mime type: %s\n")
TRACE_MSG(DRM_DH_1430_112_2_17_23_38_37_1384,"[DRM] Encoding type: %d\n")
TRACE_MSG(DRM_DH_1475_112_2_17_23_38_37_1385,"[DRM] ParseDrmMsg ret code: %d\n")
TRACE_MSG(DRM_DH_1588_112_2_17_23_38_37_1386,"[DRM] ProcDrmMsgFL\n")
TRACE_MSG(DRM_DH_1602_112_2_17_23_38_37_1387,"[DRM] Create file failed\n")
TRACE_MSG(DRM_DH_1611_112_2_17_23_38_37_1388,"[DRM] DRM DH: disk no space.")
TRACE_MSG(DRM_DH_1674_112_2_17_23_38_37_1389,"[DRM] ProcDrmMsgFL, result = %d\n")
TRACE_MSG(DRM_DH_1704_112_2_17_23_38_37_1390,"[DRM] SaveDrmMsgToFile\n")
TRACE_MSG(DRM_DH_1807_112_2_17_23_38_37_1391,"[DRM] SaveDrmMsgToFile, result = %d\n")
TRACE_MSG(DRM_DH_1830_112_2_17_23_38_37_1392,"[DRM] ProcDrmMsgCD\n")
TRACE_MSG(DRM_DH_1844_112_2_17_23_38_37_1393,"[DRM] DRM DH: Can not open input file.")
TRACE_MSG(DRM_DH_1853_112_2_17_23_38_37_1394,"[DRM] DRM DH: disk no space.")
TRACE_MSG(DRM_DH_1910_112_2_17_23_38_38_1395,"[DRM] Content id = %s\n")
TRACE_MSG(DRM_DH_1942_112_2_17_23_38_38_1396,"[DRM] ProcDrmMsgCD, result = %d\n")
TRACE_MSG(DRM_DH_1964_112_2_17_23_38_38_1397,"[DRM] ProcDrmContent\n")
TRACE_MSG(DRM_DH_1976_112_2_17_23_38_38_1398,"[DRM] DH: Can not open input file.")
TRACE_MSG(DRM_DH_1985_112_2_17_23_38_38_1399,"[DRM] DRM DH: disk no space.")
TRACE_MSG(DRM_DH_2023_112_2_17_23_38_38_1400,"[DRM] ProcDrmContent, result = %d\n")
TRACE_MSG(DRM_DH_2094_112_2_17_23_38_38_1401,"[DRM] SaveDcfFile, result = %d\n")
TRACE_MSG(DRM_DH_2175_112_2_17_23_38_38_1402,"[DRM] CopyDcfFile, result = %d\n")
TRACE_MSG(DRM_DH_2195_112_2_17_23_38_38_1403,"[DRM] ProcDrmMsgDCF\n")
TRACE_MSG(DRM_DH_2227_112_2_17_23_38_38_1404,"[DRM] ProcDrmMsgDCF, result = %d\n")
TRACE_MSG(DRM_DH_2302_112_2_17_23_38_38_1405,"[DRM] GetMethodLevel, level = %d\n")
TRACE_MSG(DRM_DH_2332_112_2_17_23_38_38_1406,"[DRM] DH: Can not open input file.")
TRACE_MSG(DRM_DH_2379_112_2_17_23_38_39_1407,"[DRM] ProcDrmRights, result = %d\n")
TRACE_MSG(DRM_DH_2451_112_2_17_23_38_39_1408,"[DRM] DRM_SendInstallCnfToApp\n")
TRACE_MSG(DRM_DH_2478_112_2_17_23_38_39_1409,"[DRM] DRM_ProcDrmFile\n")
TRACE_MSG(DRM_DH_2565_112_2_17_23_38_39_1410,"[DRM] DRM_ProcDrmFile, error_code = %d\n")
TRACE_MSG(DRM_DH_2586_112_2_17_23_38_39_1411,"[DRM] ProcRightsInBuff \n")
TRACE_MSG(DRM_DH_2596_112_2_17_23_38_39_1412,"[DRM] ProcRightsInBuff type =%d \n")
TRACE_MSG(DRM_DH_2601_112_2_17_23_38_39_1413,"[DRM] DRM DH: disk no space.")
TRACE_MSG(DRM_DH_2642_112_2_17_23_38_39_1414,"[DRM] ProcRightsInBuff ret=%d\n")
TRACE_MSG(DRM_DH_2674_112_2_17_23_38_39_1415,"[DRM] GetContentMimeTypeOfDrmContent, mime_type = %d\n")
TRACE_MSG(DRM_DH_2704_112_2_17_23_38_39_1416,"[DRM] DH: Can not open input file.")
TRACE_MSG(DRM_DH_2739_112_2_17_23_38_39_1417,"[DRM] GetContentMimeTypeOfDrmMsgCD, mime_type = %d\n")
TRACE_MSG(DRM_DH_2771_112_2_17_23_38_39_1418,"[DRM] GetContentMimeTypeOfDrmMsgDCF, mime_type = %d\n")
TRACE_MSG(DRM_DH_2831_112_2_17_23_38_39_1419,"[DRM] GetContentMimeTypeOfDrmMsgFL, mime_type = %d\n")
TRACE_MSG(DRM_DH_2912_112_2_17_23_38_40_1420,"[DRM] DRM_ContentMimeCNFToApp\n")
TRACE_MSG(DRM_DH_2949_112_2_17_23_38_40_1421,"[DRM] DRM_ProContentMimeType ret=%d\n")
TRACE_MSG(DRM_DH_2979_112_2_17_23_38_40_1422,"[DRM] SaveDrmMsgInPrivateFormat !\n")
TRACE_MSG(DRM_DH_3097_112_2_17_23_38_40_1423,"\n[DRM] ReadDrmEntityInfoFormFile ret =%d\n")
TRACE_MSG(DRM_DH_3174_112_2_17_23_38_40_1424,"\n[DRM] CreateDrmEntityInfoFile create handle error !\n")
TRACE_MSG(DRM_DH_3182_112_2_17_23_38_40_1425,"\n[DRM] CreateDrmEntityInfoFile memery alloc error !\n")
TRACE_MSG(DRM_DH_3187_112_2_17_23_38_40_1426,"\n[DRM] CreateDrmEntityInfoFile !handle =%0x08\n")
TRACE_MSG(DRM_DH_3308_112_2_17_23_38_40_1427,"\n[DRM] UpdateDrmConstraint !\n")
TRACE_MSG(DRM_DH_3342_112_2_17_23_38_40_1428,"\n[DRM] UpdateDrmRights !\n")
TRACE_MSG(DRM_DH_3380_112_2_17_23_38_41_1429,"[DRM] DCFSDWriteDrmEntityInfo ret =%d\n")
TRACE_MSG(DRM_DH_3428_112_2_17_23_38_41_1430,"[DRM] DCFSDReadDrmEntityInfo ret =%d\n")
TRACE_MSG(DRM_DH_3586_112_2_17_23_38_41_1431,"\n[DRM] ProcDcfDrmEntityInfo ret=%d\n")
TRACE_MSG(DRM_DH_3630_112_2_17_23_38_41_1432,"\n[DRM] GenerateContentID !\n")
TRACE_MSG(DRM_DH_3662_112_2_17_23_38_41_1433,"\n[DRM] GenerateKey !\n")
TRACE_MSG(DRM_DH_3825_112_2_17_23_38_41_1434,"\n[DRM] DrmBase64ToBinary ret=%d\n")
TRACE_MSG(DRM_DH_3900_112_2_17_23_38_42_1435,"[DRM] DRM_ProDeleteRightsFile ret=%d\n")
TRACE_MSG(DRM_DH_3942_112_2_17_23_38_42_1436,"[DRM] DRM_DeleteALLRightsFile ret=%d\n")
TRACE_MSG(DRM_DH_4000_112_2_17_23_38_42_1437,"[DRM] DRM_DeleteInvalidRightsFile ret=%d\n")
TRACE_MSG(DRM_DH_4028_112_2_17_23_38_42_1438,"[DRM] DRM_DeleteRightsCNFToApp\n")
TRACE_MSG(DRM_DH_4094_112_2_17_23_38_42_1439,"[DRM] DRM_ProCancelInstall error_code =%d\n")
TRACE_MSG(DRM_DH_4123_112_2_17_23_38_42_1440,"[DRM] ParseDrmCDWbxmlMsg\n")
TRACE_MSG(DRM_DH_4167_112_2_17_23_38_42_1441,"[DRM] Mime type: %s\n")
TRACE_MSG(DRM_DH_4285_112_2_17_23_38_42_1442,"[DRM] Encoding type: %d\n")
TRACE_MSG(DRM_DH_4298_112_2_17_23_38_42_1443,"[DRM] Mime type: %s\n")
TRACE_MSG(DRM_DH_4349_112_2_17_23_38_42_1444,"[DRM] ParseDrmMsg ret code: %d\n")
TRACE_MSG(DRM_DH_4414_112_2_17_23_38_43_1445,"[DRM] GetMethodIsCDWbxml is wbxml = %d\n")
TRACE_MSG(DRM_IO_292_112_2_17_23_38_44_1446,"[DRM] Cmd: DRM_IRP_IO_CREATE_FILE\n")
TRACE_MSG(DRM_IO_326_112_2_17_23_38_44_1447,"[DRM] Cmd: DRM_IRP_IO_CLOSE_FILE\n")
TRACE_MSG(DRM_IO_331_112_2_17_23_38_44_1448,"[DRM] Cmd: DRM_SIG_IO_DELETE_FILE\n")
TRACE_MSG(DRM_IO_336_112_2_17_23_38_44_1449,"[DRM] Cmd: DRM_IRP_IO_SET_FILE_POINTER\n")
TRACE_MSG(DRM_IO_344_112_2_17_23_38_44_1450,"[DRM] Cmd: DRM_IRP_IO_GET_FILE_POINTER\n")
TRACE_MSG(DRM_IO_355_112_2_17_23_38_44_1451,"[DRM] Cmd: DRM_IRP_IO_GET_FILE_SIZE\n")
TRACE_MSG(DRM_IO_364_112_2_17_23_38_44_1452,"[DRM] Cmd: DRM_SIG_IO_CONSUME_FILE_RIGHTS\n")
TRACE_MSG(DRM_IO_370_112_2_17_23_38_44_1453,"[DRM] Cmd: DRM_SIG_IO_SD_ENCRPT_READ\n")
TRACE_MSG(DRM_IO_377_112_2_17_23_38_44_1454,"[DRM] Cmd: DRM_SIG_IO_GET_FILE_RIGHTS\n")
TRACE_MSG(DRM_IO_384_112_2_17_23_38_44_1455,"[DRM] Cmd: DRM IO default \n")
TRACE_MSG(DRM_IO_433_112_2_17_23_38_44_1456,"[DRM] Cmd: DRM_SIG_CANCEL_INSTALL_FILE_REQ \n")
TRACE_MSG(DRM_IO_437_112_2_17_23_38_44_1457,"[DRM] Cmd: DRM IO default \n")
TRACE_MSG(DRM_IO_487_112_2_17_23_38_44_1458,"[DRM] ProcCreateFileReq ret=%d\n")
TRACE_MSG(DRM_IO_539_112_2_17_23_38_44_1459,"[DRM] OpenDCFFile CheckDrmRights is TRUE \n")
TRACE_MSG(DRM_IO_551_112_2_17_23_38_44_1460,"[DRM] OpenDCFFile FL_DeleteFile expired rights file\n")
TRACE_MSG(DRM_IO_553_112_2_17_23_38_44_1461,"[DRM] OpenDCFFile CheckDrmRights is FALSE \n")
TRACE_MSG(DRM_IO_558_112_2_17_23_38_44_1462,"[DRM] OpenDCFFile LoadDrmEntityInfoFromFile is FALSE \n")
TRACE_MSG(DRM_IO_578_112_2_17_23_38_44_1463,"[DRM] OpenDCFFile ParseDCF is TRUE file method_level is =%d\n")
TRACE_MSG(DRM_IO_583_112_2_17_23_38_44_1464,"[DRM] OpenDCFFile ParseDCF is FALSE \n")
TRACE_MSG(DRM_IO_612_112_2_17_23_38_44_1465,"[DRM] OpenDCFFile drm_io_context_ptr io_impl_ptr is NULL \n")
TRACE_MSG(DRM_IO_614_112_2_17_23_38_44_1466,"[DRM] OpenDCFFile drm_io_context_ptr->is_allow_io is true \n")
TRACE_MSG(DRM_IO_631_112_2_17_23_38_44_1467,"[DRM] OpenDCFFile ret=%d\n")
TRACE_MSG(DRM_IO_667_112_2_17_23_38_44_1468,"[DRM] ProcReadFileReq ret=%d\n")
TRACE_MSG(DRM_IO_711_112_2_17_23_38_44_1469,"[DRM] ProcCloseFileReq ret=%d\n")
TRACE_MSG(DRM_IO_760_112_2_17_23_38_44_1470,"[DRM] ProcDeleteFileReq ret=%d\n")
TRACE_MSG(DRM_IO_795_112_2_17_23_38_45_1471,"[DRM] ProcSetFilePointerReq ret=%d\n")
TRACE_MSG(DRM_IO_830_112_2_17_23_38_45_1472,"[DRM] ProcGetFilePointerReq ret=%d\n")
TRACE_MSG(DRM_IO_863_112_2_17_23_38_45_1473,"[DRM] ProcGetFileSizeReq ret =%d\n")
TRACE_MSG(DRM_IO_896_112_2_17_23_38_45_1474,"[DRM] ProcGetFileSizeReq ret =%d\n")
TRACE_MSG(DRM_IO_932_112_2_17_23_38_45_1475,"[DRM] ConsumeRights  cur system time =%u net time =%u cur_nettime_handle_type =%u\n")
TRACE_MSG(DRM_IO_1089_112_2_17_23_38_45_1476,"[DRM] CheckDrmRightsConstraint cursystemtime=%u nettime=%u cur_nettime_hanle_type=%d\n")
TRACE_MSG(DRM_IO_1215_112_2_17_23_38_45_1477,"[DRM] ProcSDEncrptReadReq ret =%d\n")
TRACE_MSG(DRM_IO_1273_112_2_17_23_38_45_1478,"[DRM] ProcGetFileRights ret =%d\n")
TRACE_MSG(DRM_IO_IMPL_SD_255_112_2_17_23_38_48_1479,"[DRM] SDDrmIoOpenImpl ret =%d \n")
TRACE_MSG(DRM_IO_IMPL_SD_336_112_2_17_23_38_48_1480,"[DRM] SDDrmIoCloseImpl ret =%d \n")
TRACE_MSG(DRM_IO_IMPL_SD_416_112_2_17_23_38_48_1481,"[DRM] SDDrmIoSetPosImpl offset =%d  origin =%d\n")
TRACE_MSG(DRM_IO_IMPL_SD_480_112_2_17_23_38_48_1482,"[DRM] SDDrmIoGetPosImpl ret =%d origin =%d offset =%d\n")
TRACE_MSG(DRM_IO_IMPL_SD_517_112_2_17_23_38_48_1483,"[DRM] SDDrmIoGetFileSize size =%d \n")
TRACE_MSG(DRM_IO_IMPL_SD_734_112_2_17_23_38_49_1484,"[DRM] DrmReadDecryptData postion =%d postion_num =%d postion_lit =%d bytes_to_read =%d bytes_read =%dn")
TRACE_MSG(DRM_IO_IMPL_SD_740_112_2_17_23_38_49_1485,"[DRM] DrmRemovePadding\n")
TRACE_MSG(DRM_IO_IMPL_SD_764_112_2_17_23_38_49_1486,"[DRM] DrmReadDecryptData ret=%d bytes_to_read =%d bytes_read =%d\n")
TRACE_MSG(DRM_IO_IMPL_SD_796_112_2_17_23_38_49_1487," SD DecryptData 0x=%0x")
TRACE_MSG(DRM_SFS_278_112_2_17_23_38_50_1488,"[DRM] DRM_CreateFile Open DRM file success, drm_handle = %08x\n")
TRACE_MSG(DRM_SFS_303_112_2_17_23_38_50_1489,"[DRM] DRM_CreateFile Open normal file success, drm_handle = %08x\n")
TRACE_MSG(DRM_SFS_365_112_2_17_23_38_50_1490,"[DRM] DRM_CreateFileAsyn Open DRM file success, drm_handle = %08x\n")
TRACE_MSG(DRM_SFS_389_112_2_17_23_38_50_1491,"[DRM] DRM_CreateFileAsyn Open normal file success, handle = %08x\n")
TRACE_MSG(DRM_SFS_395_112_2_17_23_38_50_1492,"[DRM] DRM_CreateFileAsyn Open normal file success, handle = %08x\n")
TRACE_MSG(DRM_SFS_401_112_2_17_23_38_50_1493,"[DRM] DRM_CreateFileAsyn Open file success, handle = %08x\n")
TRACE_MSG(DRM_SFS_676_112_2_17_23_38_50_1494,"[DRM] DRM_CloseFile drm file error_code =%d\n")
TRACE_MSG(DRM_SFS_682_112_2_17_23_38_50_1495,"[DRM] DRM_CloseFile normal file error_code =%d\n")
TRACE_MSG(DRM_SFS_689_112_2_17_23_38_50_1496,"[DRM] DRM_CloseFile error_code =%d\n")
TRACE_MSG(DRM_SFS_757_112_2_17_23_38_51_1497,"[DRM] DRM_DeleteFile\n")
TRACE_MSG(DRM_SFS_764_112_2_17_23_38_51_1498,"[DRM] Delete DRM file success\n")
TRACE_MSG(DRM_SFS_775_112_2_17_23_38_51_1499,"[DRM] delete normal file success\n")
TRACE_MSG(DRM_SFS_1327_112_2_17_23_38_52_1500,"[DRM] DRM_SDSetEncrypt ret =%d\n")
TRACE_MSG(DRM_SFS_1421_112_2_17_23_38_52_1501,"[DRM] DRM_GetRightsHandle ret =%d\n")
TRACE_MSG(DRM_SFS_1545_112_2_17_23_38_52_1502,"[DRM] Invalid irp 0x%x")
TRACE_MSG(DRM_SFS_1583_112_2_17_23_38_52_1503,"[DRM] DrmCallbackFunc default\n")
TRACE_MSG(DRM_SFS_1603_112_2_17_23_38_52_1504,"[DRM] DRM DrmIoDefaultParam ,task id is invalid\n")
TRACE_MSG(DRM_SFS_1753_112_2_17_23_38_53_1505,"[DRM] DRM 1 priorityCur ERROR \n")
TRACE_MSG(DRM_SFS_1770_112_2_17_23_38_53_1506,"[DRM] DRM 2 priorityCurFFS ERROR \n")
TRACE_MSG(DRM_SFS_1780_112_2_17_23_38_53_1507,"[DRM] DRM 3 old_priority_ptr ERROR \n")
TRACE_MSG(DRM_SFS_1812_112_2_17_23_38_53_1508,"[DRM] DRM DrmIoWaitComplete failed ,OS error\n")
TRACE_MSG(DRM_SFS_1818_112_2_17_23_38_53_1509,"[DRM] DrmIoSendIrp Send signal failed \n")
TRACE_MSG(DRM_SFS_1829_112_2_17_23_38_53_1510,"[DRM] DRM FFS 4 old_priority_ptr ERROR \n")
TRACE_MSG(DRM_SFS_1849_112_2_17_23_38_53_1511,"[DRM] DrmCheckIsDRM file name is NULL\n")
TRACE_MSG(DRM_SFS_1861_112_2_17_23_38_53_1512,"[DRM] DrmCheckIsDRM  ret_val=%d\n")
TRACE_MSG(DRM_SFS_1886_112_2_17_23_38_53_1513,"[DRM] DrmParseDCF  ret_val=%d\n")
TRACE_MSG(DRM_UNIT_TEST_224_112_2_17_23_38_53_1514,"DRM_IO_CALLBACK DRM_IO_CALLBACK\n")
TRACE_MSG(DRM_XML_593_112_2_17_23_38_55_1515,"[DRM] DRM XML parse error code: %d\n")
TRACE_MSG(DRM_XML_598_112_2_17_23_38_55_1516,"[DRM] DRM XML parse version id is error")
TRACE_MSG(DRM_XML_604_112_2_17_23_38_55_1517,"[DRM] DRM ParseCDXmlRights ret %d\n")
TRACE_MSG(DRM_XML_671_112_2_17_23_38_55_1518,"[DRM] DRM XML parse error code: %d\n")
TRACE_MSG(DRM_XML_676_112_2_17_23_38_55_1519,"[DRM] DRM XML parse version id is error")
TRACE_MSG(DRM_XML_680_112_2_17_23_38_55_1520,"[DRM] DRM ParseXmlRightsInBuff ret %d\n")
TRACE_MSG(DRM_XML_772_112_2_17_23_38_55_1521,"[DRM] DRM XML parse error code: %d\n")
TRACE_MSG(DRM_XML_777_112_2_17_23_38_55_1522,"[DRM] DRM XML parse version id is error")
TRACE_MSG(DRM_XML_781_112_2_17_23_38_55_1523,"[DRM] DRM ParseXmlRights ret %d\n")
TRACE_MSG(DRM_XML_1090_112_2_17_23_38_56_1524,"\n[DRM] Content ID: %s\n")
TRACE_MSG(DRM_XML_1773_112_2_17_23_38_57_1525,"\n[DRM] Count: %s\n")
TRACE_MSG(DRM_XML_1790_112_2_17_23_38_57_1526,"\n[DRM] Interval: %s\n")
TRACE_MSG(DRM_XML_1806_112_2_17_23_38_58_1527,"\n[DRM] Start time: %s\n")
TRACE_MSG(DRM_XML_1813_112_2_17_23_38_58_1528,"\n[DRM] End time: %s\n")
TRACE_MSG(DRM_XML_2072_112_2_17_23_38_58_1529,"[DRM] Parse Interval: %d year %d month %d day %d hour %d minute %d seconds\n")
TRACE_MSG(DRM_XML_2094_112_2_17_23_38_58_1530,"[DRM] Value: %d\n")
TRACE_MSG(DRM_XML_2170_112_2_17_23_38_58_1531,"[DRM] Parse Interval: %d year %d month %d day %d hour %d minute %d seconds\n")
TRACE_MSG(DRM_XML_2174_112_2_17_23_38_58_1532,"[DRM] Value: %d")
TRACE_MSG(DRM_XML_2347_112_2_17_23_38_59_1533,"[DRM] ParseWBXML failed: %d")
TRACE_MSG(DRM_XML_2353_112_2_17_23_38_59_1534,"[DRM] ParseWBXML version id failed ")
TRACE_MSG(DRM_XML_2459_112_2_17_23_38_59_1535,"[DRM] ParseWBXML failed: %d")
TRACE_MSG(DRM_XML_2465_112_2_17_23_38_59_1536,"[DRM] ParseWBXML version id failed ")
TRACE_MSG(PUA_MAIN_172_112_2_17_23_39_3_1537,"PUA: receive unknown sip message from: %08x\n")
TRACE_MSG(PUA_MAIN_177_112_2_17_23_39_3_1538,"[ PUA ] Receive unknown message from: %08x\n")
TRACE_MSG(PUA_MAIN_256_112_2_17_23_39_3_1539,"[ PUBLISH ] PUA_PRES_SRC_CREATE_REQ (APP -> PUA)")
TRACE_MSG(PUA_MAIN_265_112_2_17_23_39_3_1540,"[ PUBLISH ] PUA_PRES_SRC_CREATE_CNF (PUA -> APP)")
TRACE_MSG(PUA_MAIN_266_112_2_17_23_39_3_1541,"[ PUBLISH ] Error code: %d")
TRACE_MSG(PUA_MAIN_267_112_2_17_23_39_3_1542,"[ PUBLISH ] Module id: %08x")
TRACE_MSG(PUA_MAIN_268_112_2_17_23_39_3_1543,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUA_MAIN_277_112_2_17_23_39_3_1544,"[ PUBLISH ] PUA_PRES_SRC_CLOSE_REQ (APP -> PUA)")
TRACE_MSG(PUA_MAIN_278_112_2_17_23_39_3_1545,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUA_MAIN_287_112_2_17_23_39_3_1546,"[ PUBLISH ] PUA_PRES_SRC_CLOSE_CNF (PUA -> APP)")
TRACE_MSG(PUA_MAIN_288_112_2_17_23_39_3_1547,"[ PUBLISH ] Error code: %d")
TRACE_MSG(PUA_MAIN_289_112_2_17_23_39_3_1548,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUA_MAIN_299_112_2_17_23_39_3_1549,"[ PUBLISH ] PUA_PRES_SRC_PUB_REQ (APP -> PUA)")
TRACE_MSG(PUA_MAIN_300_112_2_17_23_39_3_1550,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUA_MAIN_309_112_2_17_23_39_4_1551,"[ PUBLISH ] PUA_PRES_SRC_PUB_CNF (PUA -> APP)")
TRACE_MSG(PUA_MAIN_310_112_2_17_23_39_4_1552,"[ PUBLISH ] Error code: %d")
TRACE_MSG(PUA_MAIN_311_112_2_17_23_39_4_1553,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUA_MAIN_320_112_2_17_23_39_4_1554,"[ PUBLISH ] PUA_PRES_SRC_UNPUB_REQ (APP -> PUA)")
TRACE_MSG(PUA_MAIN_321_112_2_17_23_39_4_1555,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUA_MAIN_330_112_2_17_23_39_4_1556,"[ PUBLISH ] PUA_PRES_SRC_UNPUB_CNF (PUA -> APP)")
TRACE_MSG(PUA_MAIN_331_112_2_17_23_39_4_1557,"[ PUBLISH ] Error code: %d")
TRACE_MSG(PUA_MAIN_332_112_2_17_23_39_4_1558,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUA_MAIN_338_112_2_17_23_39_4_1559,"[ PUBLISH ] PUA_PRES_SRC_AUTH_IND (PUA -> APP)")
TRACE_MSG(PUA_MAIN_344_112_2_17_23_39_4_1560,"[ PUBLISH ] PUA_PRES_SRC_AUTH_RSP (APP -> PUA)")
TRACE_MSG(PUA_MAIN_350_112_2_17_23_39_4_1561,"[ PUBLISH ] PUA_PRES_SRC_PUBLISHING_IND (PUA -> APP)")
TRACE_MSG(PUA_MAIN_356_112_2_17_23_39_4_1562,"[ PUBLISH ] PUA_PRES_SRC_REDIRECTED_IND (PUA -> APP)")
TRACE_MSG(PUA_MAIN_365_112_2_17_23_39_4_1563,"[ PUBLISH ] PUA_PRES_SRC_ERROR_IND (PUA -> APP)")
TRACE_MSG(PUA_MAIN_366_112_2_17_23_39_4_1564,"[ PUBLISH ] Error code: %d")
TRACE_MSG(PUA_MAIN_367_112_2_17_23_39_4_1565,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUA_MAIN_373_112_2_17_23_39_4_1566,"[ PUBLISH ] PUA_PRES_SRC_REF_IND (PUA -> APP)")
TRACE_MSG(PUA_MAIN_382_112_2_17_23_39_4_1567,"[ PUBLISH ] PUA_PRES_SRC_TERMINATED_IND (PUA -> APP)")
TRACE_MSG(PUA_MAIN_383_112_2_17_23_39_4_1568,"[ PUBLISH ] Instance id: %08x")
TRACE_MSG(PUBLISH_HSM_630_112_2_17_23_39_7_1569,"[ PUBLISH ] IdleState Entry")
TRACE_MSG(PUBLISH_HSM_635_112_2_17_23_39_7_1570,"[ PUBLISH ] IdleState Exit")
TRACE_MSG(PUBLISH_HSM_720_112_2_17_23_39_7_1571,"[ PUBLISH ] InitingState Entry")
TRACE_MSG(PUBLISH_HSM_725_112_2_17_23_39_7_1572,"[ PUBLISH ] InitingState Exit")
TRACE_MSG(PUBLISH_HSM_872_112_2_17_23_39_7_1573,"[ PUBLISH ] InitUnpubPendingState Entry")
TRACE_MSG(PUBLISH_HSM_877_112_2_17_23_39_7_1574,"[ PUBLISH ] InitUnpubPendingState Exit")
TRACE_MSG(PUBLISH_HSM_1071_112_2_17_23_39_8_1575,"[ PUBLISH ] PubProcState Entry")
TRACE_MSG(PUBLISH_HSM_1076_112_2_17_23_39_8_1576,"[ PUBLISH ] PubProcState Exit")
TRACE_MSG(PUBLISH_HSM_1284_112_2_17_23_39_8_1577,"[ PUBLISH ] TerminatingState Entry")
TRACE_MSG(PUBLISH_HSM_1289_112_2_17_23_39_8_1578,"[ PUBLISH ] TerminatingState Exit")
TRACE_MSG(PUBLISH_HSM_1336_112_2_17_23_39_8_1579,"[ PUBLISH ] TerminatedState Entry")
TRACE_MSG(PUBLISH_HSM_1341_112_2_17_23_39_8_1580,"[ PUBLISH ] TerminatedState Exit")
TRACE_MSG(PUBLISH_HSM_1384_112_2_17_23_39_8_1581,"[ PUBLISH ] PubProcPublishingState Entry")
TRACE_MSG(PUBLISH_HSM_1389_112_2_17_23_39_8_1582,"[ PUBLISH ] PubProcPublishingState Exit")
TRACE_MSG(PUBLISH_HSM_1512_112_2_17_23_39_8_1583,"[ PUBLISH ] PubProcPublishedState Entry")
TRACE_MSG(PUBLISH_HSM_1520_112_2_17_23_39_8_1584,"[ PUBLISH ] PubProcPublishedState Exit")
TRACE_MSG(PUBLISH_HSM_1626_112_2_17_23_39_9_1585,"[ PUBLISH ] PubProcFailedState Entry")
TRACE_MSG(PUBLISH_HSM_1631_112_2_17_23_39_9_1586,"[ PUBLISH ] PubProcFailedState Exit")
TRACE_MSG(PUBLISH_HSM_1710_112_2_17_23_39_9_1587,"[ PUBLISH ] PubProcRefreshingState Entry")
TRACE_MSG(PUBLISH_HSM_1715_112_2_17_23_39_9_1588,"[ PUBLISH ] PubProcRefreshingState Exit")
TRACE_MSG(PUBLISH_HSM_1880_112_2_17_23_39_9_1589,"[ PUBLISH ] PubProcPubPendingState Entry")
TRACE_MSG(PUBLISH_HSM_1885_112_2_17_23_39_9_1590,"[ PUBLISH ] PubProcPubPendingState Exit")
TRACE_MSG(PUBLISH_HSM_2006_112_2_17_23_39_9_1591,"[ PUBLISH ] PubProcUnpubPendingState Entry")
TRACE_MSG(PUBLISH_HSM_2011_112_2_17_23_39_9_1592,"[ PUBLISH ] PubProcUnpubPendingState Exit")
TRACE_MSG(SUBS_HSM_597_112_2_17_23_39_11_1593,"[ SUBSCRIBE ] SubscribeHSM_IdleState Entry")
TRACE_MSG(SUBS_HSM_602_112_2_17_23_39_11_1594,"[ SUBSCRIBE ] SubscribeHSM_IdleState Exit")
TRACE_MSG(SUBS_HSM_650_112_2_17_23_39_11_1595,"[ SUBSCRIBE ] SubscribeHSM_InitingState Entry")
TRACE_MSG(SUBS_HSM_655_112_2_17_23_39_11_1596,"[ SUBSCRIBE ] SubscribeHSM_InitingState Exit")
TRACE_MSG(SUBS_HSM_690_112_2_17_23_39_11_1597,"[ PUA WATCHER ] session id: %d")
TRACE_MSG(SUBS_HSM_796_112_2_17_23_39_11_1598,"[ SUBSCRIBE ] SubscribeHSM_InitUnsubPendingState Entry")
TRACE_MSG(SUBS_HSM_801_112_2_17_23_39_11_1599,"[ SUBSCRIBE ] SubscribeHSM_InitUnsubPendingState Exit")
TRACE_MSG(SUBS_HSM_938_112_2_17_23_39_12_1600,"[ SUBSCRIBE ] SubscribeHSM_UnsubscribingState Entry")
TRACE_MSG(SUBS_HSM_943_112_2_17_23_39_12_1601,"[ SUBSCRIBE ] SubscribeHSM_UnsubscribingState Exit")
TRACE_MSG(SUBS_HSM_1009_112_2_17_23_39_12_1602,"[ SUBSCRIBE ] SubscribeHSM_TerminatingState Entry")
TRACE_MSG(SUBS_HSM_1014_112_2_17_23_39_12_1603,"[ SUBSCRIBE ] SubscribeHSM_TerminatingState Exit")
TRACE_MSG(SUBS_HSM_1058_112_2_17_23_39_12_1604,"[ SUBSCRIBE ] SubscribeHSM_TerminatedState Entry")
TRACE_MSG(SUBS_HSM_1063_112_2_17_23_39_12_1605,"[ SUBSCRIBE ] SubscribeHSM_TerminatedState Exit")
TRACE_MSG(SUBS_HSM_1114_112_2_17_23_39_12_1606,"[ SUBSCRIBE ] SubscribeHSM_SubsProcState Entry")
TRACE_MSG(SUBS_HSM_1119_112_2_17_23_39_12_1607,"[ SUBSCRIBE ] SubscribeHSM_SubsProcState Exit")
TRACE_MSG(SUBS_HSM_1301_112_2_17_23_39_12_1608,"[ SUBSCRIBE ] SubscribeHSM_SubsProcSubscribingState Entry")
TRACE_MSG(SUBS_HSM_1306_112_2_17_23_39_12_1609,"[ SUBSCRIBE ] SubscribeHSM_SubsProcSubscribingState Exit")
TRACE_MSG(SUBS_HSM_1346_112_2_17_23_39_12_1610,"PUA: unknown subscription state")
TRACE_MSG(SUBS_HSM_1347_112_2_17_23_39_12_1611,"PUA: terminate it now")
TRACE_MSG(SUBS_HSM_1413_112_2_17_23_39_13_1612,"[ SUBSCRIBE ] SubscribeHSM_SubsProc2XXRCVDState Entry")
TRACE_MSG(SUBS_HSM_1418_112_2_17_23_39_13_1613,"[ SUBSCRIBE ] SubscribeHSM_SubsProc2XXRCVDState Exit")
TRACE_MSG(SUBS_HSM_1467_112_2_17_23_39_13_1614,"PUA: unknown subscription state")
TRACE_MSG(SUBS_HSM_1468_112_2_17_23_39_13_1615,"PUA: terminate it now")
TRACE_MSG(SUBS_HSM_1498_112_2_17_23_39_13_1616,"[ SUBSCRIBE ] SubscribeHSM_SubsProcPendingState Entry")
TRACE_MSG(SUBS_HSM_1503_112_2_17_23_39_13_1617,"[ SUBSCRIBE ] SubscribeHSM_SubsProcPendingState Exit")
TRACE_MSG(SUBS_HSM_1549_112_2_17_23_39_13_1618,"PUA: unknown subscription state")
TRACE_MSG(SUBS_HSM_1550_112_2_17_23_39_13_1619,"PUA: terminate it now")
TRACE_MSG(SUBS_HSM_1585_112_2_17_23_39_13_1620,"[ SUBSCRIBE ] SubscribeHSM_SubsProcNotifyRCVDState Entry")
TRACE_MSG(SUBS_HSM_1590_112_2_17_23_39_13_1621,"[ SUBSCRIBE ] SubscribeHSM_SubsProcNotifyRCVDState Exit")
TRACE_MSG(SUBS_HSM_1671_112_2_17_23_39_13_1622,"[ SUBSCRIBE ] SubscribeHSM_SubsProcActiveState Entry")
TRACE_MSG(SUBS_HSM_1676_112_2_17_23_39_13_1623,"[ SUBSCRIBE ] SubscribeHSM_SubsProcActiveState Exit")
TRACE_MSG(SUBS_HSM_1719_112_2_17_23_39_13_1624,"PUA: unknown subscription state")
TRACE_MSG(SUBS_HSM_1720_112_2_17_23_39_13_1625,"PUA: terminate it now")
TRACE_MSG(SUBS_HSM_1756_112_2_17_23_39_13_1626,"[ SUBSCRIBE ] SubscribeHSM_SubsProcRefreshingState Entry")
TRACE_MSG(SUBS_HSM_1761_112_2_17_23_39_13_1627,"[ SUBSCRIBE ] SubscribeHSM_SubsProcRefreshingState Exit")
TRACE_MSG(SUBS_HSM_1800_112_2_17_23_39_13_1628,"[ SUBSCRIBE ] SubscribeHSM_SubsProcUnsubPendingState Entry")
TRACE_MSG(SUBS_HSM_1805_112_2_17_23_39_13_1629,"[ SUBSCRIBE ] SubscribeHSM_SubsProcUnsubPendingState Exit")
TRACE_MSG(XML_PIDF_67_112_2_17_23_39_18_1630,"Context initialization failed.\n")
TRACE_MSG(XML_PIDF_125_112_2_17_23_39_18_1631,"Context initialization failed.\n")
TRACE_MSG(XML_PIDF_135_112_2_17_23_39_18_1632,"encoded XML message:\n")
TRACE_MSG(XML_PIDF_140_112_2_17_23_39_18_1633,"Encoding failed\n")
TRACE_MSG(MAINAPP_1028_112_2_17_23_39_21_1634,"StartIdleTimer: focus and flip!")
TRACE_MSG(MAINAPP_1048_112_2_17_23_39_21_1635,"StartIdleTimer: the idle timer has started!")
TRACE_MSG(MAINAPP_1109_112_2_17_23_39_21_1636,"StartIdleTimer: the idle timer has stoped!")
TRACE_MSG(MAINAPP_1155_112_2_17_23_39_21_1637,"MAIN_SetIdleBlueToothState: the blue tooth status is same!")
TRACE_MSG(MAINAPP_1182_112_2_17_23_39_21_1638,"MAIN_SetIdleBlueToothState: the blue tooth status is same!")
TRACE_MSG(MAINAPP_1223_112_2_17_23_39_21_1639,"MAIN_SetIdleNetWorkName: the name id is %d!")
TRACE_MSG(MAINAPP_1252_112_2_17_23_39_21_1640,"MAIN_SetIdleServiceProvidorName: tha name len = %d, is ucs2 = %d")
TRACE_MSG(MAINAPP_1383_112_2_17_23_39_21_1641,"MAIN_SetIdleChargeState: the is_charge is same!")
TRACE_MSG(MAINAPP_1477_112_2_17_23_39_21_1642,"MAIN_SetIdleSmsState: the sms_state =%d is same!")
TRACE_MSG(MAINAPP_1512_112_2_17_23_39_21_1643,"MAIN_SetIdleCallState: the call_state is same!")
TRACE_MSG(MAINAPP_1525_112_2_17_23_39_22_1644,"MAIN_SetIdleGprsState: gprs_state = %d")
TRACE_MSG(MAINAPP_1829_112_2_17_23_39_22_1645,"MAIN_HandleSysMsg: begin MISC_MSG_RTC_MIN!")
TRACE_MSG(MAINAPP_1909_112_2_17_23_39_22_1646,"MAIN_HandleSysMsg: can't handle the msg!")
TRACE_MSG(MAINAPP_1972_112_2_17_23_39_22_1647,"DrawFmName s_fm_rect.left= %d")
TRACE_MSG(MAINAPP_1973_112_2_17_23_39_22_1648,"DrawFmName s_fm_rect.right= %d")
TRACE_MSG(MAINAPP_1974_112_2_17_23_39_22_1649,"DrawFmName s_fm_rect.top= %d")
TRACE_MSG(MAINAPP_1975_112_2_17_23_39_22_1650,"DrawFmName s_fm_rect.bottom= %d")
TRACE_MSG(MAINAPP_2041_112_2_17_23_39_23_1651,"[MMIDC]: IdleWin_HandleMsg, MSG_OPEN_WINDOW")
TRACE_MSG(MAINAPP_2089_112_2_17_23_39_23_1652,"IdleWin_HandleMsg MSG_FULL_PAINT")
TRACE_MSG(MAINAPP_2134_112_2_17_23_39_23_1653,"IDLEWIN_HandleMsg: handle MSG_IDLE_UPDATE_DATETIME!")
TRACE_MSG(MAINAPP_2144_112_2_17_23_39_23_1654,"IdleWin_HandleMsg: MSG_IDLE_UPDATE_NETWORK!")
TRACE_MSG(MAINAPP_2167_112_2_17_23_39_23_1655,"IdleWin_HandleMsg: MSG_IDLE_UPDATE_MP3!")
TRACE_MSG(MAINAPP_2454_112_2_17_23_39_23_1656,"IDLEWIN_HandleWinMsg: handle headset MSG!")
TRACE_MSG(MAINAPP_2470_112_2_17_23_39_23_1657,"IdleWin_HandleMsg:received timer msg!")
TRACE_MSG(MAINAPP_2477_112_2_17_23_39_23_1658,"IdleWin_HandleMsg: handle the anim wallpater timer!")
TRACE_MSG(MAINAPP_2508_112_2_17_23_39_23_1659,"IdleWin_HandleMsg: MMK_GetFocusWinId() is =%d")
TRACE_MSG(MAINAPP_2645_112_2_17_23_39_24_1660,"MMI: IdleWin_HandleMsg, received tp msg_id ")
TRACE_MSG(MAINAPP_2652_112_2_17_23_39_24_1661,"IdleWin_HandleMsg: point.x=%d")
TRACE_MSG(MAINAPP_2653_112_2_17_23_39_24_1662,"IdleWin_HandleMsg: point.y=%d")
TRACE_MSG(MAINAPP_2654_112_2_17_23_39_24_1663,"IdleWin_HandleMsg s_mp3_rect.left= %d")
TRACE_MSG(MAINAPP_2655_112_2_17_23_39_24_1664,"IdleWin_HandleMsg s_mp3_rect.right= %d")
TRACE_MSG(MAINAPP_2656_112_2_17_23_39_24_1665,"IdleWin_HandleMsg s_mp3_rect.top= %d")
TRACE_MSG(MAINAPP_2657_112_2_17_23_39_24_1666,"IdleWin_HandleMsg s_mp3_rect.bottom= %d")
TRACE_MSG(MAINAPP_2658_112_2_17_23_39_24_1667,"IdleWin_HandleMsg s_idlewin_info.is_display_mp3= %d")
TRACE_MSG(MAINAPP_2683_112_2_17_23_39_24_1668,"IdleWin_HandleMsg open mp3 player!")
TRACE_MSG(MAINAPP_2716_112_2_17_23_39_24_1669,"IDLEWIN_HandleWinMsg: MSG_STK_SET_UP_IDLE_MODE")
TRACE_MSG(MAINAPP_2746_112_2_17_23_39_24_1670,"MMI: IdleWin_HandleMsg, don't handle message, msg_id = %d")
TRACE_MSG(MAINAPP_2776_112_2_17_23_39_24_1671,"OpenDialingNumberBock, enter")
TRACE_MSG(MAINAPP_2781_112_2_17_23_39_24_1672,"[mainapp.c]: dialing number block have been opened")
TRACE_MSG(MAINAPP_3176_112_2_17_23_39_25_1673,"kick the key VT")
TRACE_MSG(MAINAPP_3272_112_2_17_23_39_25_1674,"FuncDialingWin_HandleMsg MSG_KEYLONG_HASH!")
TRACE_MSG(MAINAPP_3306_112_2_17_23_39_25_1675,"FuncDialingWin_HandleMsg:result_info_ptr->protect_type %d is error!")
TRACE_MSG(MAINAPP_3327_112_2_17_23_39_25_1676,"[mainapp.c]: close dialing number block")
TRACE_MSG(MAINAPP_3331_112_2_17_23_39_25_1677,"[mainapp.c]: dialing number block have been closed!")
TRACE_MSG(MAINAPP_3339_112_2_17_23_39_25_1678,"[mainapp.c]: BLOCK disable failure!")
TRACE_MSG(MAINAPP_3342_112_2_17_23_39_25_1679," CloseDialingNumberBlock: close block successful!")
TRACE_MSG(MAINAPP_3364_112_2_17_23_39_25_1680,"main_app_init:call_ring_type is %d!")
TRACE_MSG(MAINAPP_3459_112_2_17_23_39_25_1681,"MAIN_StopIdleTimer: the sim error timer has stoped!")
TRACE_MSG(MAINAPP_3646_112_2_17_23_39_26_1682,"OutputIdleWinWords idle_mode->len= %d")
TRACE_MSG(MAINAPP_3668_112_2_17_23_39_26_1683,"OutputIdleWinWords s_words_rect.left= %d")
TRACE_MSG(MAINAPP_3669_112_2_17_23_39_26_1684,"OutputIdleWinWords s_words_rect.right= %d")
TRACE_MSG(MAINAPP_3670_112_2_17_23_39_26_1685,"OutputIdleWinWords s_words_rect.top= %d")
TRACE_MSG(MAINAPP_3671_112_2_17_23_39_26_1686,"OutputIdleWinWords s_words_rect.bottom= %d")
TRACE_MSG(MAINAPP_3727_112_2_17_23_39_26_1687,"OutputIdleWinNetworkName point.x= %d")
TRACE_MSG(MAINAPP_3728_112_2_17_23_39_26_1688,"OutputIdleWinNetworkName point.y= %d")
TRACE_MSG(MAINAPP_3753_112_2_17_23_39_26_1689,"OutputIdleWinNetworkName, is_get_cphs_info = %d")
TRACE_MSG(MAINAPP_3768_112_2_17_23_39_26_1690,"OutputIdleWinNetworkName s_network_rect.left= %d")
TRACE_MSG(MAINAPP_3769_112_2_17_23_39_26_1691,"OutputIdleWinNetworkName s_network_rect.right= %d")
TRACE_MSG(MAINAPP_3770_112_2_17_23_39_26_1692,"OutputIdleWinNetworkName s_network_rect.top= %d")
TRACE_MSG(MAINAPP_3771_112_2_17_23_39_26_1693,"OutputIdleWinNetworkName s_network_rect.bottom= %d")
TRACE_MSG(MAINAPP_3794_112_2_17_23_39_26_1694,"OutputIdleWinNetworkName, get the network name from sim card!")
TRACE_MSG(MAINAPP_3823_112_2_17_23_39_26_1695,"OutputIdleWinNetworkName s_network_rect.left= %d")
TRACE_MSG(MAINAPP_3824_112_2_17_23_39_26_1696,"OutputIdleWinNetworkName s_network_rect.right= %d")
TRACE_MSG(MAINAPP_3825_112_2_17_23_39_26_1697,"OutputIdleWinNetworkName s_network_rect.top= %d")
TRACE_MSG(MAINAPP_3826_112_2_17_23_39_26_1698,"OutputIdleWinNetworkName s_network_rect.bottom= %d")
TRACE_MSG(MAINAPP_3837_112_2_17_23_39_26_1699,"OutputIdleWinNetworkName, get the network name using the old method!")
TRACE_MSG(MAINAPP_4229_112_2_17_23_39_27_1700,"DisplayIdleMp3Name g_idle_cur_mp3_name.length= %d")
TRACE_MSG(MAINAPP_4230_112_2_17_23_39_27_1701,"DisplayIdleMp3Name g_idle_cur_mp3_name.is_ucs2= %d")
TRACE_MSG(MAINAPP_4238_112_2_17_23_39_27_1702,"DisplayIdleMp3Name string_width= %d")
TRACE_MSG(MAINAPP_4253_112_2_17_23_39_27_1703,"DisplayIdleMp3Name s_mp3_rect.left= %d")
TRACE_MSG(MAINAPP_4254_112_2_17_23_39_27_1704,"DisplayIdleMp3Name s_mp3_rect.right= %d")
TRACE_MSG(MAINAPP_4255_112_2_17_23_39_27_1705,"DisplayIdleMp3Name s_mp3_rect.top= %d")
TRACE_MSG(MAINAPP_4256_112_2_17_23_39_27_1706,"DisplayIdleMp3Name s_mp3_rect.bottom= %d")
TRACE_MSG(MAINAPP_4315_112_2_17_23_39_27_1707,"DisplayIdleCycleMp3Name s_mp3_rect.left= %d")
TRACE_MSG(MAINAPP_4316_112_2_17_23_39_27_1708,"DisplayIdleCycleMp3Name s_mp3_rect.right= %d")
TRACE_MSG(MAINAPP_4317_112_2_17_23_39_27_1709,"DisplayIdleCycleMp3Name s_mp3_rect.top= %d")
TRACE_MSG(MAINAPP_4318_112_2_17_23_39_27_1710,"DisplayIdleCycleMp3Name s_mp3_rect.bottom= %d")
TRACE_MSG(MAINAPP_4609_112_2_17_23_39_28_1711,"DisplayStatusBarIcon start MMI_WIN_ICON_GPRS is_gprs is %d")
TRACE_MSG(MAINAPP_4612_112_2_17_23_39_28_1712,"DisplayStatusBarIcon MMI_WIN_ICON_GPRS sim is OK!")
TRACE_MSG(MAINAPP_4626_112_2_17_23_39_28_1713,"DisplayStatusBarIcon image_id = IMAGE_IDLE_TITLE_ICON_GPRS_DEACTIVE")
TRACE_MSG(MAINAPP_4793_112_2_17_23_39_28_1714,"DisplayStatusBarIcon is charge: image_id=%d")
TRACE_MSG(MAINAPP_4800_112_2_17_23_39_28_1715,"DisplayStatusBarIcon not charge: image_id=%d")
TRACE_MSG(MAINAPP_4964_112_2_17_23_39_28_1716,"MAIN_SetIdleSmsState: the line_state is same!")
TRACE_MSG(MAINAPP_4981_112_2_17_23_39_28_1717,"MAIN_SetIdleSmsState: the voice message is same!")
TRACE_MSG(MAINAPP_5090_112_2_17_23_39_29_1718,"MMIMAIN_HandleIdelTimer: handle the idle icon timer!")
TRACE_MSG(MAINAPP_5163_112_2_17_23_39_29_1719,"MMI_GetFreeSpace: free_space = %d")
TRACE_MSG(MAINAPP_5325_112_2_17_23_39_29_1720,"Handle_HW_Icon_Exec_Func : s_cur_hw_icon_index = %d")
TRACE_MSG(MMIGAME_WINTAB_135_112_2_17_23_39_30_1721,"mmigame_wintab.c:enter main menu handle function")
TRACE_MSG(PUA_UNIT_TEST_232_112_2_17_23_40_33_1722,"[ PUAUT ] Receive an signal, but do not know who sent it")
TRACE_MSG(PUA_UNIT_TEST_307_112_2_17_23_40_33_1723,"\nPUAUT: ProcAppUTTestSignal()")
TRACE_MSG(PUA_UNIT_TEST_308_112_2_17_23_40_33_1724,"\nPUAUT: Command is %s, %s, %s")
TRACE_MSG(PUA_UNIT_TEST_351_112_2_17_23_40_34_1725,"[ PUAUT ] REG parameters error")
TRACE_MSG(PUA_UNIT_TEST_528_112_2_17_23_40_34_1726,"PUAUT: Register failed, because of no resource.\n")
TRACE_MSG(PUA_UNIT_TEST_543_112_2_17_23_40_34_1727,"PUAUT: Register success")
TRACE_MSG(PUA_UNIT_TEST_545_112_2_17_23_40_34_1728,"PUAUT: Register failed, because of no resource.\n")
TRACE_MSG(PUA_UNIT_TEST_584_112_2_17_23_40_34_1729,"PUAUT[ PUB ]: Create presence source failed(%d).\n")
TRACE_MSG(PUA_UNIT_TEST_610_112_2_17_23_40_34_1730,"PUAUT[ PUB ]: Unpublish failed")
TRACE_MSG(PUA_UNIT_TEST_665_112_2_17_23_40_34_1731,"PUAUT[ WATCHER ]: Create watcher success")
TRACE_MSG(PUA_UNIT_TEST_672_112_2_17_23_40_34_1732,"PUAUT[ WATCHER ]: Create watcher failed(%d).\n")
TRACE_MSG(PUA_UNIT_TEST_703_112_2_17_23_40_34_1733,"PUAUT[ WATCHER ]: Unsubscribe success")
TRACE_MSG(PUA_UNIT_TEST_705_112_2_17_23_40_34_1734,"PUAUT[ WATCHER ]: Unsubscribe failed")
TRACE_MSG(PUA_UNIT_TEST_727_112_2_17_23_40_34_1735,"PUAUT[ WATCHER ]: close success")
TRACE_MSG(PUA_UNIT_TEST_729_112_2_17_23_40_34_1736,"PUAUT[ WATCHER ]: close failed")
TRACE_MSG(PUA_UNIT_TEST_737_112_2_17_23_40_34_1737,"PUAUT[ WATCHER ]: redirected failed")
TRACE_MSG(PUA_UNIT_TEST_741_112_2_17_23_40_34_1738,"PUAUT[ WATCHER ]: NOTIFY IND")
TRACE_MSG(PUA_UNIT_TEST_771_112_2_17_23_40_34_1739,"PUAUT[ WINFO ]: Create watcher success")
TRACE_MSG(PUA_UNIT_TEST_778_112_2_17_23_40_34_1740,"PUAUT[ WINFO ]: Create watcher failed(%d).\n")
TRACE_MSG(PUA_UNIT_TEST_809_112_2_17_23_40_34_1741,"PUAUT[ WINFO ]: Unsubscribe success")
TRACE_MSG(PUA_UNIT_TEST_811_112_2_17_23_40_34_1742,"PUAUT[ WINFO ]: Unsubscribe failed")
TRACE_MSG(PUA_UNIT_TEST_833_112_2_17_23_40_35_1743,"PUAUT[ WINFO ]: close success")
TRACE_MSG(PUA_UNIT_TEST_835_112_2_17_23_40_35_1744,"PUAUT[ WINFO ]: close failed")
TRACE_MSG(PUA_UNIT_TEST_843_112_2_17_23_40_35_1745,"PUAUT[ WINFO ]: redirected failed")
TRACE_MSG(PUA_UNIT_TEST_847_112_2_17_23_40_35_1746,"PUAUT[ WINFO ]: NOTIFY IND")
TRACE_MSG(PUA_UNIT_TEST_867_112_2_17_23_40_35_1747,"PUAUT: Registering\n")
TRACE_MSG(PUA_UNIT_TEST_887_112_2_17_23_40_35_1748,"PUAUT[ PUB ]: PUA_PRES_SRC_PUB_CNF")
TRACE_MSG(PUA_UNIT_TEST_888_112_2_17_23_40_35_1749,"PUAUT[ PUB ]: error code(%d)")
TRACE_MSG(PUA_UNIT_TEST_889_112_2_17_23_40_35_1750,"PUAUT[ PUB ]: instance id(%08x)")
TRACE_MSG(PUA_UNIT_TEST_895_112_2_17_23_40_35_1751,"PUAUT[ SIGNAL ]: unknown message")
TRACE_MSG(PUA_UNIT_TEST_1286_112_2_17_23_40_35_1752,"Parse result \"%s %s\"")
TRACE_MSG(PUA_UT_DRIVE_465_112_2_17_23_40_38_1753,"PUAUT: TestOneCase:\n")
TRACE_MSG(PUA_UT_DRIVE_499_112_2_17_23_40_38_1754,"[PUA UT DRIVER] Send signal: (%s) (%s) (%s)")
TRACE_MSG(PUA_UT_DRIVE_504_112_2_17_23_40_38_1755,"[PUA UT DRIVER] Send signal: (%d)")
TRACE_MSG(PUA_UT_DRIVE_523_112_2_17_23_40_38_1756,"[PUA UT DRIVER] Receive signal PUA_UT_STOP_TEST_IND")
TRACE_MSG(PUA_UT_DRIVE_527_112_2_17_23_40_38_1757,"[PUA UT DRIVER] Receive signal PUA_UT_SEND_CMD_CNF")
TRACE_MSG(PUA_UT_DRIVE_537_112_2_17_23_40_38_1758,"PUAUT: [FATAL ERROR] System requirement: VC WIN32\n")
TRACE_MSG(POC_ADAP_AUDIO_86_112_2_17_23_40_39_1759,"POCPS:POCADAP_AudioCreatePlayerHandle audio_handle=0x%x")
TRACE_MSG(POC_ADAP_AUDIO_145_112_2_17_23_40_39_1760,"POCPS:POCADAP_AudioStartPlay audio_handle=0x%x, dev_mode=%d, status=%d")
TRACE_MSG(POC_ADAP_AUDIO_187_112_2_17_23_40_39_1761,"POCPS:POCADAP_AudioSetPlayerParams audio_handle=0x%x, dev_mode=%d, vol=%d")
TRACE_MSG(POC_ADAP_AUDIO_231_112_2_17_23_40_39_1762,"POCPS:POCADAP_AudioSetDataAndPlay audio_handle=0x%x, buf_ptr=0x%x, buf_len=%d, status=%d")
TRACE_MSG(POC_ADAP_AUDIO_256_112_2_17_23_40_39_1763,"POCPS:POCADAP_AudioStopPlay audio_handle=0x%x")
TRACE_MSG(POC_ADAP_AUDIO_286_112_2_17_23_40_39_1764,"POCPS:POCADAP_AudioClosePlayerHandle audio_handle=0x%x")
TRACE_MSG(POC_ADAP_AUDIO_303_112_2_17_23_40_39_1765,"POCPS:POCADAP_AudioCreateRecorderHandle audio_handle=0x%x")
TRACE_MSG(POC_ADAP_AUDIO_341_112_2_17_23_40_39_1766,"POCPS:POCADAP_AudioStartRecord audio_handle=0x%x, status=%d")
TRACE_MSG(POC_ADAP_AUDIO_382_112_2_17_23_40_39_1767,"POCPS:POCADAP_AudioGetRecordData audio_handle=0x%x, status=%d, data_length=%d")
TRACE_MSG(POC_ADAP_AUDIO_417_112_2_17_23_40_39_1768,"POCPS:POCADAP_AudioStopRecord audio_handle=0x%x, status=%d")
TRACE_MSG(POC_ADAP_AUDIO_440_112_2_17_23_40_39_1769,"POCPS:POCADAP_AudioCloseRecorderHandle audio_handle=0x%x")
TRACE_MSG(POC_ADAP_AUDIO_458_112_2_17_23_40_39_1770,"POCPS:PlayerCallBack play_result=%d")
TRACE_MSG(POC_ADAP_AUDIO_479_112_2_17_23_40_39_1771,"POCPS:RecorderCallBack data_length=%d")
TRACE_MSG(POC_ADAP_OS_123_112_2_17_23_40_40_1772,"POCPS: POCADAP_OSSetupQueue start\n")
TRACE_MSG(POC_ADAP_OS_147_112_2_17_23_40_40_1773,"POCPS:Send to app   %.32s    id=0x%x\n")
TRACE_MSG(POC_ADAP_OS_157_112_2_17_23_40_40_1774,"POCPS:POCADAP_OSSendSignalToApp failed,errno:%d\n")
TRACE_MSG(POC_ADAP_OS_170_112_2_17_23_40_40_1775,"POCPS:POCADAP_OSSendSignalToApp failed\n")
TRACE_MSG(POC_ADAP_OS_195_112_2_17_23_40_40_1776,"POCPS:POCADAP_OSSendSignalToPoC failed,errno:%d\n")
TRACE_MSG(POC_ADAP_OS_208_112_2_17_23_40_40_1777,"POCPS:POCADAP_OSSendSignalToPoC failed\n")
TRACE_MSG(POC_DEJ_222_112_2_17_23_40_58_1778,"POCPS: POCDEJ_HasDataToPlay no data")
TRACE_MSG(POC_MAIN_105_112_2_17_23_40_59_1779,"POCPS: POCMAIN_Start s_poc_task_id=0x%x")
TRACE_MSG(POC_MAIN_282_112_2_17_23_41_0_1780,"POCPS:Get           %.32s    id=0x%x\n")
TRACE_MSG(AMR_UNPACK_353_112_2_17_23_41_11_1781,"RTPPL:RTPPL_AmrUnpackConstruct,input error!")
TRACE_MSG(AMR_UNPACK_370_112_2_17_23_41_11_1782,"RTPPL:RTPPL_AmrUnpackConstruct,amr clock rate error!")
TRACE_MSG(AMR_UNPACK_378_112_2_17_23_41_11_1783,"RTPPL_AmrUnpackConstruct timestamp =%x")
TRACE_MSG(AMR_UNPACK_390_112_2_17_23_41_11_1784,"RTPPL:RTPPL_AmrUnpackConstruct,create list fail for no memory!")
TRACE_MSG(AMR_UNPACK_397_112_2_17_23_41_11_1785,"RTPPL:RTPPL_AmrUnpackConstruct,init buffer error!")
TRACE_MSG(AMR_UNPACK_449_112_2_17_23_41_11_1786,"RTPPL:RTPPL_AmrUnpackConstruct,unpack toc error!")
TRACE_MSG(AMR_UNPACK_459_112_2_17_23_41_11_1787,"RTPPL:RTPPL_AmrUnpackConstruct,skip crc error!")
TRACE_MSG(AMR_UNPACK_468_112_2_17_23_41_11_1788,"RTPPL:RTPPL_AmrUnpackConstruct,get toc count error!")
TRACE_MSG(AMR_UNPACK_476_112_2_17_23_41_11_1789,"RTPPL:RTPPL_AmrUnpackConstruct,update frame block error!")
TRACE_MSG(AMR_UNPACK_497_112_2_17_23_41_11_1790,"RTPPL:RTPPL_AmrUnpackConstruct,sorted/linear copy error!")
TRACE_MSG(AMR_UNPACK_506_112_2_17_23_41_11_1791,"RTPPL:RTPPL_AmrUnpackConstruct,alloc result buffer error!")
TRACE_MSG(AMR_UNPACK_515_112_2_17_23_41_11_1792,"RTPPL:RTPPL_AmrUnpackConstruct,convert block to buffer error!")
TRACE_MSG(AMR_UNPACK_567_112_2_17_23_41_12_1793,"RTPPL:RTPPL_AmrUnpackConstruct,occur error!")
TRACE_MSG(AMR_UNPACK_613_112_2_17_23_41_12_1794,"RTPPL:RTPPL_AmrGetFrameParam,input error!")
TRACE_MSG(AMR_UNPACK_624_112_2_17_23_41_12_1795,"RTPPL:RTPPL_AmrGetFrameParam,frame count error!")
TRACE_MSG(AMR_UNPACK_656_112_2_17_23_41_12_1796,"RTPPL:RTPPL_AmrUnpackDestruct,input error!")
TRACE_MSG(AMR_UNPACK_738_112_2_17_23_41_12_1797,"RTPPL:ConvertBlockBufToList,input error!")
TRACE_MSG(AMR_UNPACK_746_112_2_17_23_41_12_1798,"RTPPL:ConvertBlockBufToList,create list error!")
TRACE_MSG(AMR_UNPACK_754_112_2_17_23_41_12_1799,"ConvertBlockBufToList timestamp =%x")
TRACE_MSG(AMR_UNPACK_769_112_2_17_23_41_12_1800,"ConvertBlockBufToList frame timestamp 1 =%x")
TRACE_MSG(AMR_UNPACK_774_112_2_17_23_41_12_1801,"ConvertBlockBufToList frame timestamp 2 =%x")
TRACE_MSG(AMR_UNPACK_789_112_2_17_23_41_12_1802,"RTPPL:ConvertBlockBufToList,alloc frame data buffer error!")
TRACE_MSG(AMR_UNPACK_805_112_2_17_23_41_12_1803,"RTPPL:ConvertBlockBufToList,insert new node error!")
TRACE_MSG(AMR_UNPACK_854_112_2_17_23_41_12_1804,"RTPPL:UpdateFrameBlockCount,input error!")
TRACE_MSG(AMR_UNPACK_879_112_2_17_23_41_12_1805,"RTPPL:BlockBufferSetBlockCount,input error!")
TRACE_MSG(AMR_UNPACK_898_112_2_17_23_41_12_1806,"RTPPL:BlockBufferSetBlockCount,alloc block buffer error!")
TRACE_MSG(AMR_UNPACK_914_112_2_17_23_41_12_1807,"RTPPL:BlockBufferSetBlockCount,alloc block size error!")
TRACE_MSG(AMR_UNPACK_1099_112_2_17_23_41_13_1808,"RTPPL:GetTocTypeByIndex,input error!")
TRACE_MSG(AMR_UNPACK_1142_112_2_17_23_41_13_1809,"RTPPL:GetTocTypeByIndex,input error!")
TRACE_MSG(H263_UNPACK_161_112_2_17_23_41_14_1810,"RTPPL:RTPPL_H263Unpack,input error!")
TRACE_MSG(H263_UNPACK_193_112_2_17_23_41_14_1811,"RTPPL:RTPPL_H263Unpack,payload header is Mode A!")
TRACE_MSG(H263_UNPACK_241_112_2_17_23_41_15_1812,"RTPPL:RTPPL_H263Unpack,payload header is Mode B!")
TRACE_MSG(H263_UNPACK_252_112_2_17_23_41_15_1813,"RTPPL:RTPPL_H263Unpack,payload header is Mode C!")
TRACE_MSG(H263_UNPACK_282_112_2_17_23_41_15_1814,"RTPPL:RTPPL_H263PlusUnpackConstruct,input error!")
TRACE_MSG(H263_UNPACK_330_112_2_17_23_41_15_1815,"RTPPL:RTPPL_H263PlusUnpackConstruct,exist VRC header!")
TRACE_MSG(H263_UNPACK_369_112_2_17_23_41_15_1816,"RTPPL:RTPPL_H263PlusUnpackConstruct,exist extra header!")
TRACE_MSG(H263_UNPACK_426_112_2_17_23_41_15_1817,"RTPPL:RTPPL_H263PlusUnpackConstruct,occur error = %d")
TRACE_MSG(H263_UNPACK_443_112_2_17_23_41_15_1818,"RTPPL:RTPPL_H263PlusGetResult,input error!")
TRACE_MSG(H263_UNPACK_466_112_2_17_23_41_15_1819,"RTPPL:RTPPL_H263PlusUnpackDestruct,input error!")
TRACE_MSG(H264_UNPACK_179_112_2_17_23_41_16_1820,"RTPPL_H264UnpackConstruct timestamp =%x")
TRACE_MSG(H264_UNPACK_507_112_2_17_23_41_16_1821,"RTPPL: RTPPL_H264UnpackConstruct,h264 payload struct error!")
TRACE_MSG(MPEG4_UNPACK_120_112_2_17_23_41_17_1822,"RTPPL RTPPL_Mpeg4UnpackConstruct input error")
TRACE_MSG(MPEG4_UNPACK_138_112_2_17_23_41_17_1823,"RTPPL RTPPL_Mpeg4UnpackConstruct have au header")
TRACE_MSG(MPEG4_UNPACK_145_112_2_17_23_41_17_1824,"RTPPL RTPPL_Mpeg4UnpackConstruct alloc error")
TRACE_MSG(MPEG4_UNPACK_155_112_2_17_23_41_17_1825,"RTPPL RTPPL_Mpeg4UnpackConstruct create list error")
TRACE_MSG(MPEG4_UNPACK_243_112_2_17_23_41_17_1826,"RTPPL RTPPL_Mpeg4UnpackConstruct cts delta value is 0")
TRACE_MSG(MPEG4_UNPACK_265_112_2_17_23_41_17_1827,"RTPPL RTPPL_Mpeg4UnpackConstruct dts delta value is 0")
TRACE_MSG(MPEG4_UNPACK_463_112_2_17_23_41_17_1828,"RTPPL RTPPL_Mpeg4GetAuNum input error")
TRACE_MSG(MPEG4_UNPACK_492_112_2_17_23_41_18_1829,"RTPPL RTPPL_Mpeg4GetAuParam input error")
TRACE_MSG(MPEG4_UNPACK_503_112_2_17_23_41_18_1830,"RTPPL RTPPL_Mpeg4GetAuParam 1 input error")
TRACE_MSG(MPEG4_UNPACK_534_112_2_17_23_41_18_1831,"RTPPL RTPPL_Mpeg4UnpackDestruct input error")
TRACE_MSG(MPEG4_UNPACK_590_112_2_17_23_41_18_1832,"RTPPL Mpeg4ShowBits input error")
TRACE_MSG(MPEG4_UNPACK_641_112_2_17_23_41_18_1833,"RTPPL InsertNewAuNode input error")
TRACE_MSG(MPEG4_UNPACK_654_112_2_17_23_41_18_1834,"RTPPL InsertNewAuNode insert error")
TRACE_MSG(MPEG4_UNPACK_672_112_2_17_23_41_18_1835,"RTPPL DestroyAuList input error")
TRACE_MSG(RTSP_ADAPTER_83_112_2_17_23_41_18_1836,"RTSP RTSPADP_AdapterMethodToSignal method =%d,status_code =%d")
TRACE_MSG(RTSP_ADAPTER_132_112_2_17_23_41_18_1837,"\n rtsp AdapterMethodToSignal param error\n")
TRACE_MSG(RTSP_ADAPTER_270_112_2_17_23_41_18_1838,"rtsp AdapterPlayMethodToSignal RTSP_HSM_PLAY_AUTH_IND ")
TRACE_MSG(RTSP_ADAPTER_398_112_2_17_23_41_19_1839,"rtsp AdapterSetParameterMethodToSignal RTSP_HSM_SETPARAM_AUTH_IND ")
TRACE_MSG(RTSP_API_51_112_2_17_23_41_19_1840,"\n rtsp RTSP_InitSession task id = %d")
TRACE_MSG(RTSP_API_68_112_2_17_23_41_19_1841,"\n rtsp RTSP_InitSession error_code = %d")
TRACE_MSG(RTSP_API_91_112_2_17_23_41_19_1842,"RTSP RTSP_OptionsReq context_id =%d")
TRACE_MSG(RTSP_API_98_112_2_17_23_41_19_1843,"\n rtsp RTSP_OptionsReq task id = %d")
TRACE_MSG(RTSP_API_115_112_2_17_23_41_19_1844,"\n rtsp RTSP_OptionsReq error_code = %d")
TRACE_MSG(RTSP_API_142_112_2_17_23_41_19_1845,"RTSP RTSP_OptionsAuthRsp context_id =%d")
TRACE_MSG(RTSP_API_149_112_2_17_23_41_19_1846,"\n rtsp RTSP_OptionsAuthRsp task id = %d")
TRACE_MSG(RTSP_API_166_112_2_17_23_41_19_1847,"\n rtsp RTSP_OptionsAuthRsp error_code = %d")
TRACE_MSG(RTSP_API_189_112_2_17_23_41_19_1848,"RTSP RTSP_DescribeReq context_id =%d")
TRACE_MSG(RTSP_API_196_112_2_17_23_41_19_1849,"\n rtsp RTSP_DescribeReq task id = %d")
TRACE_MSG(RTSP_API_213_112_2_17_23_41_19_1850,"\n rtsp RTSP_DescribeReq error_code = %d")
TRACE_MSG(RTSP_API_240_112_2_17_23_41_19_1851,"RTSP RTSP_DescribeAuthRsp context_id =%d")
TRACE_MSG(RTSP_API_247_112_2_17_23_41_19_1852,"\n rtsp RTSP_DescribeAuthRsp task id = %d")
TRACE_MSG(RTSP_API_264_112_2_17_23_41_19_1853,"\n rtsp RTSP_DescribeAuthRsp error_code = %d")
TRACE_MSG(RTSP_API_287_112_2_17_23_41_19_1854,"RTSP RTSP_PlayReq context_id =%d")
TRACE_MSG(RTSP_API_294_112_2_17_23_41_19_1855,"\n rtsp RTSP_PlayReq task id = %d")
TRACE_MSG(RTSP_API_311_112_2_17_23_41_20_1856,"\n rtsp RTSP_PlayReq error_code = %d")
TRACE_MSG(RTSP_API_334_112_2_17_23_41_20_1857,"RTSP RTSP_PauseReq context_id =%d")
TRACE_MSG(RTSP_API_341_112_2_17_23_41_20_1858,"\n rtsp RTSP_PauseReq task id = %d")
TRACE_MSG(RTSP_API_358_112_2_17_23_41_20_1859,"\n rtsp RTSP_PauseReq error_code = %d")
TRACE_MSG(RTSP_API_381_112_2_17_23_41_20_1860,"RTSP RTSP_SetupReq context_id =%d")
TRACE_MSG(RTSP_API_388_112_2_17_23_41_20_1861,"\n rtsp RTSP_SetupReq task id = %d")
TRACE_MSG(RTSP_API_405_112_2_17_23_41_20_1862,"\n rtsp RTSP_SetupReq error_code = %d")
TRACE_MSG(RTSP_API_432_112_2_17_23_41_20_1863,"RTSP RTSP_SetupAuthRsp context_id =%d")
TRACE_MSG(RTSP_API_439_112_2_17_23_41_20_1864,"\n rtsp RTSP_SetupAuthRsp task id = %d")
TRACE_MSG(RTSP_API_456_112_2_17_23_41_20_1865,"\n rtsp RTSP_SetupAuthRsp error_code = %d")
TRACE_MSG(RTSP_API_483_112_2_17_23_41_20_1866,"RTSP RTSP_PlayAuthRsp context_id =%d")
TRACE_MSG(RTSP_API_490_112_2_17_23_41_20_1867,"\n rtsp RTSP_PlayAuthRsp task id = %d")
TRACE_MSG(RTSP_API_507_112_2_17_23_41_20_1868,"\n rtsp RTSP_PlayAuthRsp error_code = %d")
TRACE_MSG(RTSP_API_530_112_2_17_23_41_20_1869,"RTSP RTSP_TeardownReq context_id =%d")
TRACE_MSG(RTSP_API_537_112_2_17_23_41_20_1870,"\n rtsp RTSP_TeardownReq task id = %d")
TRACE_MSG(RTSP_API_554_112_2_17_23_41_20_1871,"\n rtsp RTSP_TeardownReq error_code = %d")
TRACE_MSG(RTSP_API_577_112_2_17_23_41_20_1872,"RTSP RTSP_AnnounceReq context_id =%d")
TRACE_MSG(RTSP_API_584_112_2_17_23_41_20_1873,"\n rtsp RTSP_AnnounceReq task id = %d")
TRACE_MSG(RTSP_API_601_112_2_17_23_41_20_1874,"\n rtsp RTSP_AnnounceReq error_code = %d")
TRACE_MSG(RTSP_API_624_112_2_17_23_41_20_1875,"RTSP RTSP_RecordReq context_id =%d")
TRACE_MSG(RTSP_API_631_112_2_17_23_41_20_1876,"\n rtsp RTSP_RecordReq task id = %d")
TRACE_MSG(RTSP_API_648_112_2_17_23_41_20_1877,"\n rtsp RTSP_RecordReq error_code = %d")
TRACE_MSG(RTSP_API_671_112_2_17_23_41_20_1878,"RTSP RTSP_CloseSession context_id =%d")
TRACE_MSG(RTSP_API_678_112_2_17_23_41_20_1879,"\n rtsp RTSP_CloseSession task id = %d")
TRACE_MSG(RTSP_API_695_112_2_17_23_41_20_1880,"\n rtsp RTSP_CloseSession error_code = %d")
TRACE_MSG(RTSP_API_718_112_2_17_23_41_20_1881,"RTSP RTSP_SetParamReq context_id =%d")
TRACE_MSG(RTSP_API_725_112_2_17_23_41_20_1882,"\n rtsp RTSP_SetParamReq task id = %d")
TRACE_MSG(RTSP_API_742_112_2_17_23_41_20_1883,"\n rtsp RTSP_SetParamReq error_code = %d")
TRACE_MSG(RTSP_API_769_112_2_17_23_41_20_1884,"RTSP RTSP_SetParamAuthRsp context_id =%d")
TRACE_MSG(RTSP_API_776_112_2_17_23_41_20_1885,"\n rtsp RTSP_SetParamAuthRsp task id = %d")
TRACE_MSG(RTSP_API_793_112_2_17_23_41_21_1886,"\n rtsp RTSP_SetParamAuthRsp error_code = %d")
TRACE_MSG(RTSP_AUTH_119_112_2_17_23_41_21_1887,"RTSP RTSPAUTH_GetInfo input error")
TRACE_MSG(RTSP_AUTH_133_112_2_17_23_41_21_1888,"RTSP RTSPAUTH_GetInfo 1 proxy_ptr = PNULL")
TRACE_MSG(RTSP_AUTH_143_112_2_17_23_41_21_1889,"RTSP RTSPAUTH_GetInfo 1 auth_ptr = PNULL")
TRACE_MSG(RTSP_AUTH_157_112_2_17_23_41_21_1890,"RTSP RTSPAUTH_GetInfo 2 proxy_ptr = PNULL")
TRACE_MSG(RTSP_AUTH_167_112_2_17_23_41_21_1891,"RTSP RTSPAUTH_GetInfo 2 auth_ptr = PNULL")
TRACE_MSG(RTSP_AUTH_202_112_2_17_23_41_21_1892,"RTSP RTSPAUTH_DigestResponseCreate alloc fail")
TRACE_MSG(RTSP_AUTH_219_112_2_17_23_41_21_1893,"RTSP RTSPAUTH_DigestResponseDestroy input error")
TRACE_MSG(RTSP_AUTH_290_112_2_17_23_41_21_1894,"RTSP RTSPAUTH_DigestRequestCreate alloc fail")
TRACE_MSG(RTSP_AUTH_307_112_2_17_23_41_21_1895,"RTSP RTSPAUTH_FreeDigestRequest input error")
TRACE_MSG(RTSP_AUTH_416_112_2_17_23_41_21_1896,"RTSP RTSPAUTH_DigestResponseParse response_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_AUTH_503_112_2_17_23_41_22_1897,"RTSP RtspAuthenBasicEncode user_name_ptr =%d,password_ptr =%d")
TRACE_MSG(RTSP_AUTH_544_112_2_17_23_41_22_1898,"RTSP RtspAuthenDigestEncode input error")
TRACE_MSG(RTSP_AUTH_595_112_2_17_23_41_22_1899,"RTSP RtspAuthenDigestCalcHA1 input error")
TRACE_MSG(RTSP_AUTH_632_112_2_17_23_41_22_1900,"RTSP RtspAuthenDigestCalcResult input error")
TRACE_MSG(RTSP_AUTH_682_112_2_17_23_41_22_1901,"RTSP RtspAuthenDigestRequestSetParam request_ptr =%d,name_ptr =%d,password_ptr =%d")
TRACE_MSG(RTSP_AUTH_929_112_2_17_23_41_22_1902,"\n RTSP RtspAuthenDigestResponseSaveParam invalid input \n")
TRACE_MSG(RTSP_AUTH_1130_112_2_17_23_41_23_1903,"\n RTSP RTSPAuthenDigestResponseSaveParam unknown param \n")
TRACE_MSG(RTSP_AUTH_1141_112_2_17_23_41_23_1904,"\n RTSP RTSPAuthenDigestResponseSaveParam error_code=%d")
TRACE_MSG(RTSP_AUTH_1153_112_2_17_23_41_23_1905,"\n RTSP RTSPAuthenDigestResponseSaveParam destroy request_digest_ptr")
TRACE_MSG(RTSP_AUTH_1165_112_2_17_23_41_23_1906,"\n RTSP RTSPAuthenDigestResponseSaveParam CLEAR error_code=%d")
TRACE_MSG(RTSP_AUTH_1181_112_2_17_23_41_23_1907,"\n rtsp RtspAuthenDigestTokenFind input error \n")
TRACE_MSG(RTSP_AUTH_1194_112_2_17_23_41_23_1908,"\n rtsp RtspAuthenDigestTokenFind auth map type =%d")
TRACE_MSG(RTSP_AUTH_1198_112_2_17_23_41_23_1909,"\n rtsp RtspAuthenDigestTokenFind error \n")
TRACE_MSG(RTSP_AUTH_1219_112_2_17_23_41_23_1910,"\n rtsp RtspAuthenCombineResult input error \n")
TRACE_MSG(RTSP_AUTH_1230_112_2_17_23_41_23_1911,"\n rtsp RtspAuthenCombineResult alloc error \n")
TRACE_MSG(RTSP_AUTH_1245_112_2_17_23_41_23_1912,"\n rtsp RtspAuthenCombineResult 1 input error \n")
TRACE_MSG(RTSP_AUTH_1251_112_2_17_23_41_23_1913,"\n rtsp RtspAuthenCombineResult 1 alloc error \n")
TRACE_MSG(RTSP_COOKIE_52_112_2_17_23_41_23_1914,"\n rtsp RTSPCOOKIE_NodeCreate input error\n")
TRACE_MSG(RTSP_COOKIE_59_112_2_17_23_41_23_1915,"\n rtsp RTSPCOOKIE_NodeCreate alloc error\n")
TRACE_MSG(RTSP_COOKIE_95_112_2_17_23_41_24_1916,"\n RTSP RTSPCOOKIE_Parse 1 alloc error\n")
TRACE_MSG(RTSP_COOKIE_106_112_2_17_23_41_24_1917,"\n rtsp RTSPCOOKIE_Parse 2 alloc error\n")
TRACE_MSG(RTSP_COOKIE_122_112_2_17_23_41_24_1918,"\n RTSP RTSPCookieParse 3 alloc error\n")
TRACE_MSG(RTSP_COOKIE_336_112_2_17_23_41_24_1919,"\n rtsp RTSPCOOKIE_Put input error\n")
TRACE_MSG(RTSP_COOKIE_400_112_2_17_23_41_24_1920,"\n RTSP RTSPCookiePut alloc 1 error\n")
TRACE_MSG(RTSP_COOKIE_425_112_2_17_23_41_24_1921,"\n RTSP RTSPCookiePut 2 alloc error\n")
TRACE_MSG(RTSP_COOKIE_435_112_2_17_23_41_24_1922,"\n RTSP RTSPCookiePut 3 alloc error\n")
TRACE_MSG(RTSP_COOKIE_458_112_2_17_23_41_24_1923,"\n RTSP RTSPCookiePut 4 alloc error\n")
TRACE_MSG(RTSP_COOKIE_482_112_2_17_23_41_24_1924,"RTSP RTSPCOOKIE_DestroyNode cookie_node_ptr = %d")
TRACE_MSG(RTSP_COOKIE_542_112_2_17_23_41_24_1925,"\n http RTSPGetCookie input error or no cookie\n")
TRACE_MSG(RTSP_COOKIE_593_112_2_17_23_41_25_1926,"\n RTSP RTSPGetCookie mark_parent_ptr null\n")
TRACE_MSG(RTSP_COOKIE_600_112_2_17_23_41_25_1927,"\n RTSP RTSPGetCookie alloc error\n")
TRACE_MSG(RTSP_COOKIE_633_112_2_17_23_41_25_1928,"\n RTSP RTSPGetCookie temp_str_ptr null\n")
TRACE_MSG(RTSP_COOKIE_664_112_2_17_23_41_25_1929,"\n http RTSPCookieFormat input error\n")
TRACE_MSG(RTSP_COOKIE_672_112_2_17_23_41_25_1930,"\n RTSP RTSPCookieFormat alloc error\n")
TRACE_MSG(RTSP_COOKIE_694_112_2_17_23_41_25_1931,"\n RTSP RTSPCookieFormat input 1 error\n")
TRACE_MSG(RTSP_COOKIE_713_112_2_17_23_41_25_1932,"\n RTSP RTSPCookieFormat 1 alloc error\n")
TRACE_MSG(RTSP_COOKIE_726_112_2_17_23_41_25_1933,"\n RTSP RTSPCookieFormat 1 alloc error\n")
TRACE_MSG(RTSP_EVENT_32_112_2_17_23_41_25_1934,"RTSP RTSPEVENT_QueueInit input error")
TRACE_MSG(RTSP_EVENT_50_112_2_17_23_41_25_1935,"RTSP RTSPEVENT_QueueInsert input error")
TRACE_MSG(RTSP_EVENT_53_112_2_17_23_41_25_1936,"rtsp RTSPEVENT_QueueInsert event =%d")
TRACE_MSG(RTSP_EVENT_58_112_2_17_23_41_25_1937,"RTSP RTSPEVENT_QueueInsert alloc error")
TRACE_MSG(RTSP_EVENT_93_112_2_17_23_41_25_1938,"RTSP RTSPEVENT_QueueFetch input error")
TRACE_MSG(RTSP_HEADER_122_112_2_17_23_41_26_1939,"RTSP RTSPHDR_InsertHeader input error")
TRACE_MSG(RTSP_HEADER_180_112_2_17_23_41_26_1940,"RTSP RTSPHDR_FindHeader input error")
TRACE_MSG(RTSP_HEADER_368_112_2_17_23_41_26_1941,"\n rtsp RTSPHDR_SetHeaderField input error,request =%d,token =%d,value =%d\n")
TRACE_MSG(RTSP_HEADER_371_112_2_17_23_41_26_1942,"rtsp RTSPHDR_SetHeaderField token =%s")
TRACE_MSG(RTSP_HEADER_375_112_2_17_23_41_26_1943,"rtsp RTSPHDR_SetHeaderField value =%s")
TRACE_MSG(RTSP_HEADER_383_112_2_17_23_41_26_1944,"\n RTSP RTSPHDR_SetHeaderField alloc error\n")
TRACE_MSG(RTSP_HEADER_466_112_2_17_23_41_27_1945,"\n rtsp RTSPHDR_GetHeaderValue param error\n")
TRACE_MSG(RTSP_HOST_147_112_2_17_23_41_27_1946,"RTSP RTSPHOST_FreeHostList host_list_handle =%d")
TRACE_MSG(RTSP_HSM_558_112_2_17_23_41_28_1947,"RTSP RTSPHSM_DispatchSignal dispatch signal code =%d")
TRACE_MSG(RTSP_HSM_561_112_2_17_23_41_28_1948,"RTSP RTSPHSM_DispatchSignal hsm_state =%d")
TRACE_MSG(RTSP_HSM_566_112_2_17_23_41_28_1949,"RTSP RTSPHSM_DispatchSignal event =%d")
TRACE_MSG(RTSP_HSM_597_112_2_17_23_41_28_1950,"RTSP RTSPHSM_DealRequest input error")
TRACE_MSG(RTSP_HSM_753_112_2_17_23_41_29_1951,"RTSP RTSP_TransCallback input error")
TRACE_MSG(RTSP_HSM_762_112_2_17_23_41_29_1952,"RTSP RTSP_TransCallback host_ptr =%d,context_ptr =%d")
TRACE_MSG(RTSP_HSM_771_112_2_17_23_41_29_1953,"RTSP RTSP_TransCallback sig_ptr alloc error")
TRACE_MSG(RTSP_HSM_866_112_2_17_23_41_29_1954,"\n rtsp TransCallback recv NETMGR_SEND_RETURN_IND error\n")
TRACE_MSG(RTSP_HSM_894_112_2_17_23_41_29_1955,"\n rtsp TransCallback create response error\n")
TRACE_MSG(RTSP_HSM_979_112_2_17_23_41_29_1956,"\n rtsp TransCallback recv NETMGR_RECV_RETURN_IND error\n")
TRACE_MSG(RTSP_HSM_1003_112_2_17_23_41_29_1957,"\n RTSP TransCallback default error\n")
TRACE_MSG(RTSP_HSM_1050_112_2_17_23_41_29_1958,"\n rtsp init\n")
TRACE_MSG(RTSP_HSM_1065_112_2_17_23_41_29_1959,"RTSP RTSP_CreateContext alloc error")
TRACE_MSG(RTSP_HSM_1082_112_2_17_23_41_29_1960,"RTSP RTSP_CreateContext host_list_handle =%d")
TRACE_MSG(RTSP_HSM_1086_112_2_17_23_41_29_1961,"RTSP RTSP_CreateContext method_chart_list =%d")
TRACE_MSG(RTSP_HSM_1208_112_2_17_23_41_30_1962,"RTSP RTSP_BuildAndSendReq input error")
TRACE_MSG(RTSP_HSM_1218_112_2_17_23_41_30_1963,"RTSP RTSP_BuildAndSendReq alloc error")
TRACE_MSG(RTSP_HSM_1342_112_2_17_23_41_30_1964,"RTSP RtspHandleRecvData host_ptr input error")
TRACE_MSG(RTSP_HSM_1349_112_2_17_23_41_30_1965,"RTSP RtspHandleRecvData context_ptr =%d,response_ptr =%d")
TRACE_MSG(RTSP_HSM_1461_112_2_17_23_41_30_1966,"\n rtsp RtspConstructor param error\n")
TRACE_MSG(RTSP_HSM_1478_112_2_17_23_41_30_1967,"\n rtsp RtspDestructor param error\n")
TRACE_MSG(RTSP_HSM_1494_112_2_17_23_41_30_1968,"\n rtsp RtspInitial param error\n")
TRACE_MSG(RTSP_HSM_1514_112_2_17_23_41_30_1969,"RTSP QHSM_Rtsp context id =%d")
TRACE_MSG(RTSP_HSM_1516_112_2_17_23_41_30_1970,"RTSP QHSM_Rtsp context_ptr =%d")
TRACE_MSG(RTSP_HSM_1525_112_2_17_23_41_30_1971,"RTSP QHSM_Rtsp recv RTSP_HSM_TIMEOUT_IND")
TRACE_MSG(RTSP_HSM_1530_112_2_17_23_41_30_1972,"RTSP QHSM_Rtsp RTSP_HSM_TIMEOUT_IND host_ptr =%d,context_ptr =%d")
TRACE_MSG(RTSP_HSM_1559_112_2_17_23_41_30_1973,"RTSP QHSM_Rtsp recv RTSP_HSM_CLOSE_REQ")
TRACE_MSG(RTSP_HSM_1563_112_2_17_23_41_30_1974,"RTSP QHSM_Rtsp RTSP_HSM_CLOSE_REQ host_ptr =%d,context_ptr =%d")
TRACE_MSG(RTSP_HSM_1575_112_2_17_23_41_30_1975,"RTSP QHSM_Rtsp recv RTSP_HSM_PEROID_TIMER_IND")
TRACE_MSG(RTSP_HSM_1582_112_2_17_23_41_30_1976,"RTSP QHSM_Rtsp recv RTSP_HSM_SERVER_CLOSE_IND or RTSP_HSM_EXCEPTION_CLOSE_IND")
TRACE_MSG(RTSP_HSM_1628_112_2_17_23_41_30_1977,"RTSP QHSM_Rtsp recv other method")
TRACE_MSG(RTSP_HSM_1632_112_2_17_23_41_30_1978,"RTSP QHSM_Rtsp other method host_ptr =%d,context_ptr =%d")
TRACE_MSG(RTSP_HSM_1662_112_2_17_23_41_31_1979,"RTSP QHSM_Rtsp recv RTSP_HSM_DESTROY_HSM")
TRACE_MSG(RTSP_HSM_1668_112_2_17_23_41_31_1980,"RTSP QHSM_Rtsp recv RTSP_HSM_TEARDOWN_REQ")
TRACE_MSG(RTSP_HSM_1687_112_2_17_23_41_31_1981,"\n rtsp QHSM_Rtsp param error\n")
TRACE_MSG(RTSP_HSM_1704_112_2_17_23_41_31_1982,"RTSP RTSP_DealSignal input error")
TRACE_MSG(RTSP_HSM_1713_112_2_17_23_41_31_1983,"RTSP RTSP_DealSignal RTSP_SIG_INIT_SESSION_REQ")
TRACE_MSG(RTSP_HSM_1718_112_2_17_23_41_31_1984,"RTSP RTSP_DealSignal RTSP_SIG_CLOSE_SESSION_REQ")
TRACE_MSG(RTSP_HSM_1723_112_2_17_23_41_31_1985,"RTSP RTSP_DealSignal RTSP_SIG_OPTIONS_AUTH_RSP")
TRACE_MSG(RTSP_HSM_1728_112_2_17_23_41_31_1986,"RTSP RTSP_DealSignal RTSP_SIG_DESCRIBE_AUTH_RSP")
TRACE_MSG(RTSP_HSM_1733_112_2_17_23_41_31_1987,"RTSP RTSP_DealSignal RTSP_SIG_SETUP_AUTH_RSP")
TRACE_MSG(RTSP_HSM_1738_112_2_17_23_41_31_1988,"RTSP RTSP_DealSignal RTSP_SIG_PLAY_AUTH_RSP")
TRACE_MSG(RTSP_HSM_1744_112_2_17_23_41_31_1989,"RTSP RTSP_DealSignal RTSP_SIG_TEARDOWN_REQ")
TRACE_MSG(RTSP_HSM_1749_112_2_17_23_41_31_1990,"RTSP RTSP_DealSignal RTSP_SIG_PERIOD_OPTIONS_TIMEOUT_IND")
TRACE_MSG(RTSP_HSM_1753_112_2_17_23_41_31_1991,"RTSP RTSP_DealSignal RTSP_SIG_TIMEOUT_IND")
TRACE_MSG(RTSP_HSM_1757_112_2_17_23_41_31_1992,"\n RTSP RTSP_DealSignal signal = %d\n")
TRACE_MSG(RTSP_HSM_1804_112_2_17_23_41_31_1993,"RTSP RtspHandleOptionsAuthRsp input error")
TRACE_MSG(RTSP_HSM_1845_112_2_17_23_41_31_1994,"RTSP RtspHandleDescribeAuthRsp input error")
TRACE_MSG(RTSP_HSM_1886_112_2_17_23_41_31_1995,"RTSP RtspHandlePlayAuthRsp input error")
TRACE_MSG(RTSP_HSM_1927_112_2_17_23_41_31_1996,"RTSP RtspHandleSetupAuthRsp input error")
TRACE_MSG(RTSP_HSM_1971_112_2_17_23_41_31_1997,"RTSP RtspHandlePeriodOptions input error")
TRACE_MSG(RTSP_HSM_2003_112_2_17_23_41_31_1998,"RTSP RtspHandleTimeoutInd input error")
TRACE_MSG(RTSP_HSM_2044_112_2_17_23_41_31_1999,"RTSP RtspHandleTeardownReq input error")
TRACE_MSG(RTSP_HSM_2092_112_2_17_23_41_31_2000,"\n rtsp Rtsp_Init param error\n")
TRACE_MSG(RTSP_HSM_2118_112_2_17_23_41_31_2001,"RTSP Rtsp_InitIdle recv RTSP_HSM_SETUP_REQ")
TRACE_MSG(RTSP_HSM_2125_112_2_17_23_41_31_2002,"RTSP Rtsp_InitIdle recv RTSP_HSM_DESCRIBE_REQ")
TRACE_MSG(RTSP_HSM_2132_112_2_17_23_41_31_2003,"RTSP Rtsp_InitIdle recv RTSP_HSM_STOP_REQ")
TRACE_MSG(RTSP_HSM_2144_112_2_17_23_41_31_2004,"\n rtsp Rtsp_InitIdle param error\n")
TRACE_MSG(RTSP_HSM_2163_112_2_17_23_41_32_2005,"\n rtsp Rtsp_InitDescribePending param error\n")
TRACE_MSG(RTSP_HSM_2181_112_2_17_23_41_32_2006,"RTSP Rtsp_InitDescribePendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_2203_112_2_17_23_41_32_2007,"RTSP Rtsp_InitDescribePendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_2242_112_2_17_23_41_32_2008,"RTSP Rtsp_InitDescribePendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_2246_112_2_17_23_41_32_2009,"RTSP Rtsp_InitDescribePendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_2268_112_2_17_23_41_32_2010,"\n rtsp Rtsp_InitDescribePendingSending param error\n")
TRACE_MSG(RTSP_HSM_2316_112_2_17_23_41_32_2011,"RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_AUTH_IND")
TRACE_MSG(RTSP_HSM_2319_112_2_17_23_41_32_2012,"RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_AUTH_IND param error")
TRACE_MSG(RTSP_HSM_2347_112_2_17_23_41_32_2013,"RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_OK_IND")
TRACE_MSG(RTSP_HSM_2367_112_2_17_23_41_32_2014,"RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_ERROR_IND")
TRACE_MSG(RTSP_HSM_2387_112_2_17_23_41_32_2015,"RTSP Rtsp_InitDescribePendingRecving recv RTSP_HSM_DESCRIBE_REDIRECT_IND")
TRACE_MSG(RTSP_HSM_2402_112_2_17_23_41_32_2016,"\n rtsp Rtsp_InitDescribePendingRecving param error\n")
TRACE_MSG(RTSP_HSM_2423_112_2_17_23_41_32_2017,"RTSP Rtsp_InitDescribePendingAuthPending recv RTSP_HSM_DESCRIBE_AUTH_RSP")
TRACE_MSG(RTSP_HSM_2461_112_2_17_23_41_32_2018,"\n rtsp Rtsp_InitDescribePendingAuthPending param error\n")
TRACE_MSG(RTSP_HSM_2482_112_2_17_23_41_32_2019,"RTSP RtspHandleDescribePendingAuthRsp input error")
TRACE_MSG(RTSP_HSM_2545_112_2_17_23_41_32_2020,"RTSP RTSP_SaveAuthParamToContext input error")
TRACE_MSG(RTSP_HSM_2579_112_2_17_23_41_32_2021,"\n rtsp SaveAuthParamToContext error_code=%d")
TRACE_MSG(RTSP_HSM_2597_112_2_17_23_41_32_2022,"\n rtsp Rtsp_InitDescribePending param error\n")
TRACE_MSG(RTSP_HSM_2615_112_2_17_23_41_32_2023,"RTSP Rtsp_InitSetupPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_2626_112_2_17_23_41_32_2024,"RTSP Rtsp_InitSetupPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_2666_112_2_17_23_41_33_2025,"RTSP Rtsp_InitSetupPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_2670_112_2_17_23_41_33_2026,"RTSP Rtsp_InitSetupPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_2694_112_2_17_23_41_33_2027,"\n rtsp Rtsp_InitSetupPendingSending param error\n")
TRACE_MSG(RTSP_HSM_2747_112_2_17_23_41_33_2028,"RTSP Rtsp_InitSetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND")
TRACE_MSG(RTSP_HSM_2750_112_2_17_23_41_33_2029,"RTSP Rtsp_InitSetupPendingRecving param RTSP_HSM_SETUP_AUTH_IND error")
TRACE_MSG(RTSP_HSM_2777_112_2_17_23_41_33_2030,"RTSP Rtsp_InitSetupPendingRecving recv RTSP_HSM_SETUP_OK_IND")
TRACE_MSG(RTSP_HSM_2800_112_2_17_23_41_33_2031,"RTSP setup 1 sender =%d,context_id = %d")
TRACE_MSG(RTSP_HSM_2812_112_2_17_23_41_33_2032,"RTSP Rtsp_InitSetupPendingRecving recv RTSP_HSM_SETUP_ERROR_IND")
TRACE_MSG(RTSP_HSM_2835_112_2_17_23_41_33_2033,"RTSP Rtsp_InitSetupPendingRecving recv RTSP_HSM_SETUP_REDIRECT_IND")
TRACE_MSG(RTSP_HSM_2864_112_2_17_23_41_33_2034,"\n rtsp Rtsp_InitSetupPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_2886_112_2_17_23_41_33_2035,"RTSP Rtsp_InitSetupPendingAuthPending recv RTSP_HSM_SETUP_AUTH_RSP")
TRACE_MSG(RTSP_HSM_2927_112_2_17_23_41_33_2036,"\n rtsp Rtsp_InitSetupPendingAuthPending param error\n")
TRACE_MSG(RTSP_HSM_2948_112_2_17_23_41_33_2037,"RTSP RtspHandleSetupPendingAuthRsp input error")
TRACE_MSG(RTSP_HSM_3012_112_2_17_23_41_33_2038,"RTSP RtspHandlePlayPendingAuthRsp input error")
TRACE_MSG(RTSP_HSM_3073_112_2_17_23_41_33_2039,"\n rtsp Rtsp_InitDescribePending param error\n")
TRACE_MSG(RTSP_HSM_3091_112_2_17_23_41_33_2040,"RTSP Rtsp_InitTeardownPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_3112_112_2_17_23_41_33_2041,"RTSP Rtsp_InitTeardownPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_3150_112_2_17_23_41_34_2042,"RTSP Rtsp_InitTeardownPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_3154_112_2_17_23_41_34_2043,"RTSP Rtsp_InitTeardownPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_3175_112_2_17_23_41_34_2044,"\n rtsp Rtsp_InitTeardownPendingSending param error\n")
TRACE_MSG(RTSP_HSM_3221_112_2_17_23_41_34_2045,"RTSP Rtsp_InitTeardownPendingRecving recv RTSP_HSM_TEARDOWN_OK_IND")
TRACE_MSG(RTSP_HSM_3245_112_2_17_23_41_34_2046,"RTSP Rtsp_InitTeardownPendingRecving recv RTSP_HSM_TEARDOWN_ERROR_IND")
TRACE_MSG(RTSP_HSM_3275_112_2_17_23_41_34_2047,"\n rtsp Rtsp_InitTeardownPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_3301_112_2_17_23_41_34_2048,"\n rtsp Rtsp_Ready param error\n")
TRACE_MSG(RTSP_HSM_3327_112_2_17_23_41_34_2049,"RTSP Rtsp_ReadyOnready recv RTSP_HSM_PLAY_REQ")
TRACE_MSG(RTSP_HSM_3334_112_2_17_23_41_34_2050,"RTSP Rtsp_ReadyOnready recv RTSP_HSM_STOP_REQ")
TRACE_MSG(RTSP_HSM_3341_112_2_17_23_41_34_2051,"RTSP Rtsp_ReadyOnready recv RTSP_HSM_SETUP_REQ")
TRACE_MSG(RTSP_HSM_3348_112_2_17_23_41_34_2052,"RTSP Rtsp_ReadyOnready recv RTSP_HSM_ANNOUNCE_REQ")
TRACE_MSG(RTSP_HSM_3361_112_2_17_23_41_34_2053,"\n rtsp Rtsp_Ready param error\n")
TRACE_MSG(RTSP_HSM_3388_112_2_17_23_41_34_2054,"\n rtsp Rtsp_ReadyPlayPending param error\n")
TRACE_MSG(RTSP_HSM_3409_112_2_17_23_41_34_2055,"RTSP Rtsp_ReadyPlayPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_3413_112_2_17_23_41_34_2056,"RTSP Rtsp_ReadyPlayPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_3434_112_2_17_23_41_34_2057,"\n rtsp Rtsp_ReadyPlayPendingSending param error\n")
TRACE_MSG(RTSP_HSM_3482_112_2_17_23_41_34_2058,"RTSP Rtsp_ReadyPlayPendingRecving recv RTSP_HSM_PLAY_OK_IND")
TRACE_MSG(RTSP_HSM_3505_112_2_17_23_41_34_2059,"RTSP Rtsp_ReadyPlayPendingRecving recv RTSP_HSM_PLAY_ERROR_IND")
TRACE_MSG(RTSP_HSM_3527_112_2_17_23_41_34_2060,"RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_REDIRECT_IND")
TRACE_MSG(RTSP_HSM_3537_112_2_17_23_41_34_2061,"RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_AUTH_IND")
TRACE_MSG(RTSP_HSM_3543_112_2_17_23_41_34_2062,"RTSP RECV RTSP_HSM_PLAY_AUTH_IND SEND PLAY")
TRACE_MSG(RTSP_HSM_3556_112_2_17_23_41_34_2063,"RTSP RECV RTSP_HSM_PLAY_AUTH_IND SEND AUTH IND")
TRACE_MSG(RTSP_HSM_3577_112_2_17_23_41_34_2064,"\n rtsp Rtsp_ReadyPlayPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_3604_112_2_17_23_41_34_2065,"\n rtsp Rtsp_ReadyTeardownPending param error\n")
TRACE_MSG(RTSP_HSM_3625_112_2_17_23_41_34_2066,"RTSP Rtsp_ReadyTeardownPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_3629_112_2_17_23_41_34_2067,"RTSP Rtsp_ReadyTeardownPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_3650_112_2_17_23_41_35_2068,"\n rtsp Rtsp_ReadyTeardownPendingSending param error\n")
TRACE_MSG(RTSP_HSM_3698_112_2_17_23_41_35_2069,"RTSP Rtsp_ReadyTeardownPendingRecving recv RTSP_HSM_TEARDOWN_OK_IND")
TRACE_MSG(RTSP_HSM_3720_112_2_17_23_41_35_2070,"RTSP Rtsp_ReadyTeardownPendingRecving recv RTSP_HSM_TEARDOWN_ERROR_IND")
TRACE_MSG(RTSP_HSM_3748_112_2_17_23_41_35_2071,"\n rtsp Rtsp_ReadyTeardownPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_3774_112_2_17_23_41_35_2072,"\n rtsp Rtsp_ReadySetupPending param error\n")
TRACE_MSG(RTSP_HSM_3792_112_2_17_23_41_35_2073,"RTSP Rtsp_ReadyPlayPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_3810_112_2_17_23_41_35_2074,"RTSP Rtsp_ReadyPlayPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_3844_112_2_17_23_41_35_2075,"RTSP Rtsp_ReadySetupPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_3865_112_2_17_23_41_35_2076,"RTSP Rtsp_ReadySetupPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_3902_112_2_17_23_41_35_2077,"RTSP Rtsp_ReadyTeardownPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_3920_112_2_17_23_41_35_2078,"RTSP Rtsp_ReadyTeardownPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_3954_112_2_17_23_41_35_2079,"RTSP Rtsp_ReadyAnnouncePendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_3972_112_2_17_23_41_35_2080,"RTSP Rtsp_ReadyAnnouncePendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_4006_112_2_17_23_41_35_2081,"RTSP Rtsp_ReadyRecordPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_4024_112_2_17_23_41_35_2082,"RTSP Rtsp_ReadyRecordPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_4060_112_2_17_23_41_35_2083,"RTSP Rtsp_ReadySetupPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_4064_112_2_17_23_41_35_2084,"RTSP Rtsp_ReadySetupPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_4088_112_2_17_23_41_35_2085,"\n rtsp Rtsp_ReadySetupPendingSending param error\n")
TRACE_MSG(RTSP_HSM_4145_112_2_17_23_41_36_2086,"RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND")
TRACE_MSG(RTSP_HSM_4148_112_2_17_23_41_36_2087,"RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND param error")
TRACE_MSG(RTSP_HSM_4176_112_2_17_23_41_36_2088,"RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_OK_IND")
TRACE_MSG(RTSP_HSM_4199_112_2_17_23_41_36_2089,"RTSP setup 2 sender =%d,context_id = %d")
TRACE_MSG(RTSP_HSM_4212_112_2_17_23_41_36_2090,"RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_ERROR_IND")
TRACE_MSG(RTSP_HSM_4235_112_2_17_23_41_36_2091,"RTSP Rtsp_ReadySetupPendingRecving recv RTSP_HSM_SETUP_REDIRECT_IND")
TRACE_MSG(RTSP_HSM_4264_112_2_17_23_41_36_2092,"\n rtsp Rtsp_ReadySetupPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_4286_112_2_17_23_41_36_2093,"RTSP Rtsp_ReadySetupPendingAuthPending recv RTSP_HSM_SETUP_AUTH_RSP")
TRACE_MSG(RTSP_HSM_4327_112_2_17_23_41_36_2094,"\n rtsp Rtsp_ReadySetupPendingAuthPending param error\n")
TRACE_MSG(RTSP_HSM_4353_112_2_17_23_41_36_2095,"\n rtsp Rtsp_ReadyAnnouncePending param error\n")
TRACE_MSG(RTSP_HSM_4373_112_2_17_23_41_36_2096,"RTSP Rtsp_ReadyAnnouncePendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_4377_112_2_17_23_41_36_2097,"RTSP Rtsp_ReadyAnnouncePendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_4398_112_2_17_23_41_36_2098,"\n rtsp Rtsp_ReadyAnnouncePendingSending param error\n")
TRACE_MSG(RTSP_HSM_4446_112_2_17_23_41_36_2099,"RTSP Rtsp_ReadyAnnouncePendingRecving recv RTSP_HSM_ANNOUNCE_OK_IND")
TRACE_MSG(RTSP_HSM_4465_112_2_17_23_41_36_2100,"RTSP Rtsp_ReadyAnnouncePendingRecving recv RTSP_HSM_ANNOUNCE_ERROR_IND")
TRACE_MSG(RTSP_HSM_4490_112_2_17_23_41_36_2101,"\n rtsp Rtsp_ReadyAnnouncePendingRecving param error\n")
TRACE_MSG(RTSP_HSM_4510_112_2_17_23_41_36_2102,"RTSP Rtsp_ReadyAnnounced recv RTSP_HSM_STOP_REQ")
TRACE_MSG(RTSP_HSM_4518_112_2_17_23_41_36_2103,"RTSP Rtsp_ReadyAnnounced recv RTSP_HSM_RECORD_REQ")
TRACE_MSG(RTSP_HSM_4532_112_2_17_23_41_36_2104,"\n rtsp Rtsp_ReadyAnnouncd param error\n")
TRACE_MSG(RTSP_HSM_4558_112_2_17_23_41_36_2105,"\n rtsp Rtsp_ReadyRecordPending param error\n")
TRACE_MSG(RTSP_HSM_4579_112_2_17_23_41_36_2106,"RTSP Rtsp_ReadyRecordPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_4583_112_2_17_23_41_36_2107,"RTSP Rtsp_ReadyRecordPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_4604_112_2_17_23_41_36_2108,"\n rtsp Rtsp_ReadyRecordPendingSending param error\n")
TRACE_MSG(RTSP_HSM_4650_112_2_17_23_41_37_2109,"RTSP Rtsp_ReadyRecordPendingRecving recv RTSP_HSM_RECORD_OK_IND")
TRACE_MSG(RTSP_HSM_4670_112_2_17_23_41_37_2110,"RTSP Rtsp_ReadyRecordPendingRecving recv RTSP_HSM_RECORD_ERROR_IND")
TRACE_MSG(RTSP_HSM_4695_112_2_17_23_41_37_2111,"\n rtsp Rtsp_ReadyRecordPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_4721_112_2_17_23_41_37_2112,"\n rtsp Rtsp_Playing param error\n")
TRACE_MSG(RTSP_HSM_4747_112_2_17_23_41_37_2113,"RTSP Rtsp_PlayingPlay recv RTSP_HSM_PLAY_REQ")
TRACE_MSG(RTSP_HSM_4754_112_2_17_23_41_37_2114,"RTSP Rtsp_PlayingPlay recv RTSP_HSM_STOP_REQ")
TRACE_MSG(RTSP_HSM_4761_112_2_17_23_41_37_2115,"RTSP Rtsp_PlayingPlay recv RTSP_HSM_SETUP_REQ")
TRACE_MSG(RTSP_HSM_4768_112_2_17_23_41_37_2116,"RTSP Rtsp_PlayingPlay recv RTSP_HSM_PAUSE_REQ")
TRACE_MSG(RTSP_HSM_4781_112_2_17_23_41_37_2117,"\n rtsp Rtsp_Playing param error\n")
TRACE_MSG(RTSP_HSM_4800_112_2_17_23_41_37_2118,"\n rtsp Rtsp_PlayingPlayPending param error\n")
TRACE_MSG(RTSP_HSM_4818_112_2_17_23_41_37_2119,"RTSP Rtsp_PlayingPlayPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_4837_112_2_17_23_41_37_2120,"RTSP Rtsp_PlayingPlayPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_4873_112_2_17_23_41_37_2121,"RTSP Rtsp_PlayingPlayPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_4877_112_2_17_23_41_37_2122,"RTSP Rtsp_PlayingPlayPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_4898_112_2_17_23_41_37_2123,"\n rtsp Rtsp_PlayingPlayPendingSending param error\n")
TRACE_MSG(RTSP_HSM_4944_112_2_17_23_41_37_2124,"RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_OK_IND")
TRACE_MSG(RTSP_HSM_4965_112_2_17_23_41_37_2125,"RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_REDIRECT_IND")
TRACE_MSG(RTSP_HSM_4975_112_2_17_23_41_37_2126,"RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_AUTH_IND")
TRACE_MSG(RTSP_HSM_4981_112_2_17_23_41_37_2127,"RTSP RECV RTSP_HSM_PLAY_AUTH_IND SEND PLAY")
TRACE_MSG(RTSP_HSM_4996_112_2_17_23_41_37_2128,"RTSP RECV RTSP_HSM_PLAY_AUTH_IND SEND AUTH IND")
TRACE_MSG(RTSP_HSM_5009_112_2_17_23_41_37_2129,"RTSP Rtsp_PlayingPlayPendingRecving recv RTSP_HSM_PLAY_ERROR_IND")
TRACE_MSG(RTSP_HSM_5034_112_2_17_23_41_37_2130,"\n rtsp Rtsp_PlayingPlayPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_5053_112_2_17_23_41_37_2131,"\n rtsp Rtsp_PlayingPausePending param error\n")
TRACE_MSG(RTSP_HSM_5071_112_2_17_23_41_37_2132,"RTSP Rtsp_PlayingPausePendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_5090_112_2_17_23_41_37_2133,"RTSP Rtsp_PlayingPausePendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_5127_112_2_17_23_41_38_2134,"RTSP Rtsp_PlayingPausePendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_5131_112_2_17_23_41_38_2135,"RTSP Rtsp_PlayingPausePendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_5152_112_2_17_23_41_38_2136,"\n rtsp Rtsp_PlayingPausePendingSending param error\n")
TRACE_MSG(RTSP_HSM_5201_112_2_17_23_41_38_2137,"RTSP Rtsp_PlayingPausePendingRecving recv RTSP_HSM_PAUSE_OK_IND")
TRACE_MSG(RTSP_HSM_5222_112_2_17_23_41_38_2138,"RTSP Rtsp_PlayingPausePendingRecving recv RTSP_HSM_PAUSE_ERROR_IND")
TRACE_MSG(RTSP_HSM_5248_112_2_17_23_41_38_2139,"\n rtsp Rtsp_PlayingPausePendingRecving param error\n")
TRACE_MSG(RTSP_HSM_5267_112_2_17_23_41_38_2140,"\n rtsp Rtsp_PlayingTeardownPending param error\n")
TRACE_MSG(RTSP_HSM_5285_112_2_17_23_41_38_2141,"RTSP Rtsp_PlayingTeardownPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_5304_112_2_17_23_41_38_2142,"RTSP Rtsp_PlayingTeardownPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_5341_112_2_17_23_41_38_2143,"RTSP Rtsp_PlayingTeardownPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_5345_112_2_17_23_41_38_2144,"RTSP Rtsp_PlayingTeardownPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_5366_112_2_17_23_41_38_2145,"\n rtsp Rtsp_PlayingTeardownPendingSending param error\n")
TRACE_MSG(RTSP_HSM_5413_112_2_17_23_41_38_2146,"RTSP Rtsp_PlayingTeardownPendingRecving recv RTSP_HSM_TEARDOWN_OK_IND")
TRACE_MSG(RTSP_HSM_5436_112_2_17_23_41_38_2147,"RTSP Rtsp_PlayingTeardownPendingRecving recv RTSP_HSM_TEARDOWN_ERROR_IND")
TRACE_MSG(RTSP_HSM_5464_112_2_17_23_41_38_2148,"\n rtsp Rtsp_PlayingTeardownPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_5491_112_2_17_23_41_38_2149,"\n rtsp Rtsp_PlayingSetupPending param error\n")
TRACE_MSG(RTSP_HSM_5509_112_2_17_23_41_38_2150,"RTSP Rtsp_PlayingSetupPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_5531_112_2_17_23_41_38_2151,"RTSP Rtsp_PlayingSetupPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_5571_112_2_17_23_41_38_2152,"RTSP Rtsp_PlayingSetupPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_5575_112_2_17_23_41_38_2153,"RTSP Rtsp_PlayingSetupPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_5599_112_2_17_23_41_38_2154,"\n rtsp Rtsp_PlayingSetupPendingSending param error\n")
TRACE_MSG(RTSP_HSM_5655_112_2_17_23_41_39_2155,"RTSP Rtsp_PlayingSetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND")
TRACE_MSG(RTSP_HSM_5687_112_2_17_23_41_39_2156,"RTSP Rtsp_PlayingSetupPendingRecving recv RTSP_HSM_SETUP_OK_IND")
TRACE_MSG(RTSP_HSM_5710_112_2_17_23_41_39_2157,"RTSP setup 3 sender =%d,context_id = %d")
TRACE_MSG(RTSP_HSM_5722_112_2_17_23_41_39_2158,"RTSP Rtsp_PlayingSetupPendingRecving recv RTSP_HSM_SETUP_ERROR_IND")
TRACE_MSG(RTSP_HSM_5745_112_2_17_23_41_39_2159,"RTSP Rtsp_PlayingSetupPendingRecving recv RTSP_HSM_SETUP_REDIRECT_IND")
TRACE_MSG(RTSP_HSM_5772_112_2_17_23_41_39_2160,"\n rtsp Rtsp_PlayingSetupPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_5794_112_2_17_23_41_39_2161,"RTSP Rtsp_PlayingSetupPendingAuthPending recv RTSP_HSM_SETUP_AUTH_RSP")
TRACE_MSG(RTSP_HSM_5837_112_2_17_23_41_39_2162,"\n rtsp Rtsp_PlayingSetupPendingAuthPending param error\n")
TRACE_MSG(RTSP_HSM_5859_112_2_17_23_41_39_2163,"RTSP Rtsp_PlayingPlayPendingAuthPending recv RTSP_HSM_PLAY_AUTH_RSP")
TRACE_MSG(RTSP_HSM_5899_112_2_17_23_41_39_2164,"\n rtsp Rtsp_PlayingPlayPendingAuthPending param error\n")
TRACE_MSG(RTSP_HSM_5921_112_2_17_23_41_39_2165,"RTSP Rtsp_PlayingPlayPendingAuthPending recv RTSP_HSM_PLAY_AUTH_RSP")
TRACE_MSG(RTSP_HSM_5961_112_2_17_23_41_39_2166,"\n rtsp Rtsp_PlayingPlayPendingAuthPending param error\n")
TRACE_MSG(RTSP_HSM_5988_112_2_17_23_41_39_2167,"\n rtsp Rtsp_Recording param error\n")
TRACE_MSG(RTSP_HSM_6016_112_2_17_23_41_39_2168,"RTSP Rtsp_RecordingRecord recv RTSP_HSM_RECORD_REQ")
TRACE_MSG(RTSP_HSM_6023_112_2_17_23_41_39_2169,"RTSP Rtsp_RecordingRecord recv RTSP_HSM_STOP_REQ")
TRACE_MSG(RTSP_HSM_6030_112_2_17_23_41_39_2170,"RTSP Rtsp_RecordingRecord recv RTSP_HSM_SETUP_REQ")
TRACE_MSG(RTSP_HSM_6037_112_2_17_23_41_39_2171,"RTSP Rtsp_RecordingRecord recv RTSP_HSM_PAUSE_REQ")
TRACE_MSG(RTSP_HSM_6052_112_2_17_23_41_39_2172,"\n rtsp Rtsp_Recordinf param error\n")
TRACE_MSG(RTSP_HSM_6079_112_2_17_23_41_39_2173,"\n rtsp Rtsp_RecordingRecordPending param error\n")
TRACE_MSG(RTSP_HSM_6100_112_2_17_23_41_39_2174,"RTSP Rtsp_RecordingRecordPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_6104_112_2_17_23_41_39_2175,"RTSP Rtsp_RecordingRecordPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_6125_112_2_17_23_41_40_2176,"\n rtsp Rtsp_RecordingRecordPendingSending param error\n")
TRACE_MSG(RTSP_HSM_6174_112_2_17_23_41_40_2177,"RTSP Rtsp_RecordingRecordPendingRecving recv RTSP_HSM_RECORD_OK_IND")
TRACE_MSG(RTSP_HSM_6198_112_2_17_23_41_40_2178,"RTSP Rtsp_RecordingRecordPendingRecving recv RTSP_HSM_RECORD_ERROR_IND")
TRACE_MSG(RTSP_HSM_6227_112_2_17_23_41_40_2179,"\n rtsp Rtsp_RecordingRecordPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_6254_112_2_17_23_41_40_2180,"\n rtsp Rtsp_RecordingPausePending param error\n")
TRACE_MSG(RTSP_HSM_6275_112_2_17_23_41_40_2181,"RTSP Rtsp_RecordingPausePendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_6279_112_2_17_23_41_40_2182,"RTSP Rtsp_RecordingPausePendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_6300_112_2_17_23_41_40_2183,"\n rtsp Rtsp_RecordingPausePendingSending param error\n")
TRACE_MSG(RTSP_HSM_6347_112_2_17_23_41_40_2184,"RTSP Rtsp_RecordingPausePendingRecving recv RTSP_HSM_PAUSE_OK_IND")
TRACE_MSG(RTSP_HSM_6368_112_2_17_23_41_40_2185,"RTSP Rtsp_RecordingPausePendingRecving recv RTSP_HSM_PAUSE_ERROR_IND")
TRACE_MSG(RTSP_HSM_6393_112_2_17_23_41_40_2186,"\n rtsp Rtsp_RecordingPausePendingRecving param error\n")
TRACE_MSG(RTSP_HSM_6420_112_2_17_23_41_40_2187,"\n rtsp Rtsp_RecordingTeardownPending param error\n")
TRACE_MSG(RTSP_HSM_6441_112_2_17_23_41_40_2188,"RTSP Rtsp_RecordingTeardownPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_6445_112_2_17_23_41_40_2189,"RTSP Rtsp_RecordingTeardownPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_6466_112_2_17_23_41_40_2190,"\n rtsp Rtsp_RecordingTeardownPendingSending param error\n")
TRACE_MSG(RTSP_HSM_6514_112_2_17_23_41_40_2191,"RTSP Rtsp_RecordingTeardownPendingRecving recv RTSP_HSM_TEARDOWN_OK_IND")
TRACE_MSG(RTSP_HSM_6540_112_2_17_23_41_40_2192,"RTSP Rtsp_RecordingTeardownPendingRecving recv RTSP_HSM_TEARDOWN_ERROR_IND")
TRACE_MSG(RTSP_HSM_6571_112_2_17_23_41_40_2193,"\n rtsp Rtsp_RecordingTeardownPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_6598_112_2_17_23_41_40_2194,"\n rtsp Rtsp_RecordingSetupPending param error\n")
TRACE_MSG(RTSP_HSM_6620_112_2_17_23_41_41_2195,"RTSP Rtsp_RecordingSetupPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_6642_112_2_17_23_41_41_2196,"RTSP Rtsp_RecordingSetupPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_6684_112_2_17_23_41_41_2197,"RTSP Rtsp_RecordingRecordPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_6703_112_2_17_23_41_41_2198,"RTSP Rtsp_RecordingRecordPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_6742_112_2_17_23_41_41_2199,"RTSP Rtsp_RecordingTeardownPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_6761_112_2_17_23_41_41_2200,"RTSP Rtsp_RecordingTeardownPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_6800_112_2_17_23_41_41_2201,"RTSP Rtsp_RecordingPausePendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_HSM_6819_112_2_17_23_41_41_2202,"RTSP Rtsp_RecordingPausePendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_HSM_6857_112_2_17_23_41_41_2203,"RTSP Rtsp_RecordingSetupPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_HSM_6861_112_2_17_23_41_41_2204,"RTSP Rtsp_RecordingSetupPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_HSM_6885_112_2_17_23_41_41_2205,"\n rtsp Rtsp_RecordingSetupPendingSending param error\n")
TRACE_MSG(RTSP_HSM_6940_112_2_17_23_41_41_2206,"RTSP Rtsp_RecordingSetupPendingRecving recv RTSP_HSM_SETUP_AUTH_IND")
TRACE_MSG(RTSP_HSM_6972_112_2_17_23_41_41_2207,"RTSP Rtsp_RecordingSetupPendingRecving recv RTSP_HSM_SETUP_OK_IND")
TRACE_MSG(RTSP_HSM_6995_112_2_17_23_41_41_2208,"RTSP setup 4 sender =%d,context_id = %d")
TRACE_MSG(RTSP_HSM_7008_112_2_17_23_41_41_2209,"RTSP Rtsp_RecordingSetupPendingRecving recv RTSP_HSM_SETUP_ERROR_IND")
TRACE_MSG(RTSP_HSM_7031_112_2_17_23_41_41_2210,"RTSP Rtsp_RecordingSetupPendingRecving recv RTSP_HSM_SETUP_REDIRECT_IND")
TRACE_MSG(RTSP_HSM_7059_112_2_17_23_41_41_2211,"\n rtsp Rtsp_RecordingSetupPendingRecving param error\n")
TRACE_MSG(RTSP_HSM_7081_112_2_17_23_41_41_2212,"RTSP Rtsp_RecordingSetupPendingAuthPending recv RTSP_HSM_SETUP_AUTH_RSP")
TRACE_MSG(RTSP_HSM_7142_112_2_17_23_41_42_2213,"\n rtsp Rtsp_RecordingSetupPendingAuthPending param error\n")
TRACE_MSG(RTSP_HSM_7160_112_2_17_23_41_42_2214,"\n rtsp RTSP_SigErrCnf RTSP_SIG_INIT_SESSION_REQ\n")
TRACE_MSG(RTSP_HSM_7165_112_2_17_23_41_42_2215,"\n rtsp RTSP_SigErrCnf RTSP_SIG_CLOSE_SESSION_REQ\n")
TRACE_MSG(RTSP_HSM_7170_112_2_17_23_41_42_2216,"\n rtsp RTSP_SigErrCnf RTSP_SIG_OPTIONS_REQ\n")
TRACE_MSG(RTSP_HSM_7175_112_2_17_23_41_42_2217,"\n rtsp RTSP_SigErrCnf RTSP_SIG_DESCRIBE_REQ\n")
TRACE_MSG(RTSP_HSM_7180_112_2_17_23_41_42_2218,"\n rtsp RTSP_SigErrCnf RTSP_SIG_SET_PARAM_REQ\n")
TRACE_MSG(RTSP_HSM_7185_112_2_17_23_41_42_2219,"\n rtsp RTSP_SigErrCnf RTSP_SIG_SETUP_REQ\n")
TRACE_MSG(RTSP_HSM_7190_112_2_17_23_41_42_2220,"\n rtsp RTSP_SigErrCnf RTSP_SIG_PLAY_REQ\n")
TRACE_MSG(RTSP_HSM_7195_112_2_17_23_41_42_2221,"\n rtsp RTSP_SigErrCnf RTSP_SIG_PAUSE_REQ\n")
TRACE_MSG(RTSP_HSM_7200_112_2_17_23_41_42_2222,"\n rtsp RTSP_SigErrCnf RTSP_SIG_ANNOUNCE_REQ\n")
TRACE_MSG(RTSP_HSM_7205_112_2_17_23_41_42_2223,"\n rtsp RTSP_SigErrCnf RTSP_SIG_RECORD_REQ\n")
TRACE_MSG(RTSP_HSM_7210_112_2_17_23_41_42_2224,"\n rtsp RTSP_SigErrCnf RTSP_SIG_TEARDOWN_REQ\n")
TRACE_MSG(RTSP_HSM_7215_112_2_17_23_41_42_2225,"\n rtsp RTSP_SigErrCnf RTSP_SIG_OPTIONS_AUTH_RSP\n")
TRACE_MSG(RTSP_HSM_7220_112_2_17_23_41_42_2226,"\n rtsp RTSP_SigErrCnf RTSP_SIG_SET_PARAM_AUTH_RSP\n")
TRACE_MSG(RTSP_HSM_7225_112_2_17_23_41_42_2227,"\n rtsp RTSP_SigErrCnf RTSP_SIG_DESCRIBE_AUTH_RSP\n")
TRACE_MSG(RTSP_HSM_7229_112_2_17_23_41_42_2228,"\n rtsp RTSP_SigErrCnf RTSP_SIG_PLAY_AUTH_RSP\n")
TRACE_MSG(RTSP_HSM_7234_112_2_17_23_41_42_2229,"\n rtsp RTSP_SigErrCnf RTSP_SIG_SETUP_AUTH_RSP\n")
TRACE_MSG(RTSP_HSM_7243_112_2_17_23_41_42_2230,"\n HTTP RTSP_SigErrCnf unknown signal \n")
TRACE_MSG(RTSP_HSM_7647_112_2_17_23_41_43_2231,"\n rtsp RtspHandleHsmFree hsm_ptr = %d")
TRACE_MSG(RTSP_METHOD_174_112_2_17_23_41_43_2232,"RTSP RTSPMTD_FreeMethodChart method_chart_list =%d")
TRACE_MSG(RTSP_OPTIONS_HSM_137_112_2_17_23_41_44_2233,"RTSP RTSPHSM_OptionsDispatchSignal dispatch signal")
TRACE_MSG(RTSP_OPTIONS_HSM_139_112_2_17_23_41_44_2234,"RTSP RTSPHSM_OptionsDispatchSignal hsm_state =%d")
TRACE_MSG(RTSP_OPTIONS_HSM_143_112_2_17_23_41_44_2235,"RTSP RTSPHSM_OptionsDispatchSignal event =%d")
TRACE_MSG(RTSP_OPTIONS_HSM_173_112_2_17_23_41_44_2236,"\n rtsp RtspOptionsConstructor param error\n")
TRACE_MSG(RTSP_OPTIONS_HSM_190_112_2_17_23_41_44_2237,"\n rtsp RtspDestructor param error\n")
TRACE_MSG(RTSP_OPTIONS_HSM_217_112_2_17_23_41_44_2238,"\n rtsp QHSM_RtspOptions param error\n")
TRACE_MSG(RTSP_OPTIONS_HSM_235_112_2_17_23_41_44_2239,"\n rtsp Rtsp_OptionsPendingIdle RTSP_HSM_OPTIONS_REQ")
TRACE_MSG(RTSP_OPTIONS_HSM_247_112_2_17_23_41_44_2240,"\n rtsp Rtsp_OptionsPendingIdle param error\n")
TRACE_MSG(RTSP_OPTIONS_HSM_273_112_2_17_23_41_44_2241,"\n rtsp Rtsp_OptionsPendingConnecting RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_OPTIONS_HSM_279_112_2_17_23_41_44_2242,"\n rtsp Rtsp_OptionsPendingConnecting RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_OPTIONS_HSM_328_112_2_17_23_41_44_2243,"\n rtsp Rtsp_OptionsPendingSending RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_OPTIONS_HSM_333_112_2_17_23_41_44_2244,"\n rtsp Rtsp_OptionsPendingSending RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_OPTIONS_HSM_354_112_2_17_23_41_44_2245,"\n rtsp Rtsp_OptionsPendingSending param error\n")
TRACE_MSG(RTSP_OPTIONS_HSM_402_112_2_17_23_41_44_2246,"\n rtsp Rtsp_OptionsPendingRecving RTSP_HSM_OPTIONS_AUTH_IND")
TRACE_MSG(RTSP_OPTIONS_HSM_433_112_2_17_23_41_44_2247,"\n rtsp Rtsp_OptionsPendingRecving RTSP_HSM_OPTIONS_OK_IND")
TRACE_MSG(RTSP_OPTIONS_HSM_458_112_2_17_23_41_44_2248,"\n rtsp Rtsp_OptionsPendingRecving RTSP_HSM_OPTIONS_ERROR_IND")
TRACE_MSG(RTSP_OPTIONS_HSM_483_112_2_17_23_41_44_2249,"\n rtsp Rtsp_OptionsPendingRecving RTSP_HSM_OPTIONS_REDIRECT_IND")
TRACE_MSG(RTSP_OPTIONS_HSM_499_112_2_17_23_41_44_2250,"\n rtsp Rtsp_OptionsPendingRecving param error\n")
TRACE_MSG(RTSP_OPTIONS_HSM_522_112_2_17_23_41_44_2251,"\n rtsp Rtsp_OptionsPendingAuthPending RTSP_HSM_OPTIONS_AUTH_RSP")
TRACE_MSG(RTSP_OPTIONS_HSM_578_112_2_17_23_41_45_2252,"\n rtsp Rtsp_OptionsPendingAuthPending param error\n")
TRACE_MSG(RTSP_OPTIONS_HSM_658_112_2_17_23_41_45_2253,"\n rtsp RtspOptionsInitial param error\n")
TRACE_MSG(RTSP_REQUEST_147_112_2_17_23_41_45_2254,"RTSP RTSP_CreateRequest header_list_handle =%d")
TRACE_MSG(RTSP_REQUEST_193_112_2_17_23_41_45_2255,"RTSP RTSP_CreateRequest options")
TRACE_MSG(RTSP_REQUEST_219_112_2_17_23_41_45_2256,"RTSP RTSP_CreateRequest self options")
TRACE_MSG(RTSP_REQUEST_247_112_2_17_23_41_45_2257,"RTSP RTSP_CreateRequest describe")
TRACE_MSG(RTSP_REQUEST_274_112_2_17_23_41_45_2258,"RTSP RTSP_CreateRequest setup")
TRACE_MSG(RTSP_REQUEST_298_112_2_17_23_41_45_2259,"RTSP RTSP_CreateRequest self setup")
TRACE_MSG(RTSP_REQUEST_322_112_2_17_23_41_45_2260,"RTSP RTSP_CreateRequest play")
TRACE_MSG(RTSP_REQUEST_342_112_2_17_23_41_45_2261,"RTSP RTSP_CreateRequest pause")
TRACE_MSG(RTSP_REQUEST_363_112_2_17_23_41_45_2262,"RTSP RTSP_CreateRequest announce")
TRACE_MSG(RTSP_REQUEST_382_112_2_17_23_41_46_2263,"RTSP RTSP_CreateRequest record")
TRACE_MSG(RTSP_REQUEST_403_112_2_17_23_41_46_2264,"RTSP RTSP_CreateRequest stop")
TRACE_MSG(RTSP_REQUEST_424_112_2_17_23_41_46_2265,"RTSP RTSP_CreateRequest self stop")
TRACE_MSG(RTSP_REQUEST_445_112_2_17_23_41_46_2266,"RTSP RTSP_CreateRequest set param")
TRACE_MSG(RTSP_REQUEST_523_112_2_17_23_41_46_2267,"RTSP RTSP_FreeRequest header_list_handle =%d")
TRACE_MSG(RTSP_REQUEST_557_112_2_17_23_41_46_2268,"RTSP RtspCreateOptionsReq input error")
TRACE_MSG(RTSP_REQUEST_591_112_2_17_23_41_46_2269,"RTSP RtspCreateOptionsReq no auth param")
TRACE_MSG(RTSP_REQUEST_636_112_2_17_23_41_46_2270,"RTSP RtspCreateDescribeReq input error")
TRACE_MSG(RTSP_REQUEST_670_112_2_17_23_41_46_2271,"RTSP RtspCreateDescribeReq no auth param")
TRACE_MSG(RTSP_REQUEST_715_112_2_17_23_41_46_2272,"RTSP RtspCreateSetupReq input error")
TRACE_MSG(RTSP_REQUEST_749_112_2_17_23_41_46_2273,"RTSP RtspCreateSetupReq no auth param")
TRACE_MSG(RTSP_REQUEST_770_112_2_17_23_41_46_2274,"RTSP RtspCreatePlayReq input error")
TRACE_MSG(RTSP_REQUEST_827_112_2_17_23_41_46_2275,"RTSP RtspCreatePauseReq input error")
TRACE_MSG(RTSP_REQUEST_884_112_2_17_23_41_47_2276,"RTSP RtspCreateAnnounceReq input error")
TRACE_MSG(RTSP_REQUEST_947_112_2_17_23_41_47_2277,"RTSP RtspCreateRecordReq input error")
TRACE_MSG(RTSP_REQUEST_1003_112_2_17_23_41_47_2278,"RTSP RtspCreateTeardownReq input error")
TRACE_MSG(RTSP_REQUEST_1058_112_2_17_23_41_47_2279,"RTSP RtspCreateSetparamReq input error")
TRACE_MSG(RTSP_REQUEST_1147_112_2_17_23_41_47_2280,"\n rtsp SaveAuthParam error_code=%d")
TRACE_MSG(RTSP_REQUEST_1167_112_2_17_23_41_47_2281,"\n rtsp RTSP_UpdateRequest input error\n")
TRACE_MSG(RTSP_REQUEST_1191_112_2_17_23_41_47_2282,"\n rtsp RTSP_UpdateRequest \n")
TRACE_MSG(RTSP_REQUEST_1322_112_2_17_23_41_47_2283,"\n rtsp RTSP_GetInitInfo proxy host = %s")
TRACE_MSG(RTSP_REQUEST_1326_112_2_17_23_41_47_2284,"\n rtsp RTSP_GetInitInfo proxy port =%d")
TRACE_MSG(RTSP_RESPONSE_124_112_2_17_23_41_48_2285,"RTSP RTSPRSP_CreateResponse alloc error")
TRACE_MSG(RTSP_RESPONSE_144_112_2_17_23_41_48_2286,"RTSP RTSPRSP_FreeResponse input error")
TRACE_MSG(RTSP_RESPONSE_209_112_2_17_23_41_48_2287,"RTSP RTSPRSP_ParseResponseHeader input error")
TRACE_MSG(RTSP_RESPONSE_330_112_2_17_23_41_49_2288,"RTSP RTSPRSP_ParseContentLengthHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_353_112_2_17_23_41_49_2289,"RTSP RTSPRSP_ParseSetCookieHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_404_112_2_17_23_41_49_2290,"RTSP RTSPRSP_ParseLocationHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_523_112_2_17_23_41_49_2291,"RTSP RTSPRSP_ParseSessionHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_611_112_2_17_23_41_49_2292,"RTSP RTSPRSP_ParseAuthenticationWWWHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_657_112_2_17_23_41_50_2293,"RTSP RTSPRSP_ParseAuthecticationProxyHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_699_112_2_17_23_41_50_2294,"RTSP RTSPRSP_ParsePublicHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_724_112_2_17_23_41_50_2295,"RTSP RTSPRSP_ParseCseqHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_926_112_2_17_23_41_50_2296,"RTSP RtspSetHeaderHandler input error")
TRACE_MSG(RTSP_RESPONSE_1298_112_2_17_23_41_51_2297,"RTSP RTSPRSP_ParseCseqHeader input error response_ptr = %d,name_ptr =%d,value_ptr =%d")
TRACE_MSG(RTSP_RESPONSE_1304_112_2_17_23_41_51_2298,"\n rtsp RTSPRSP_ParseTransportHeader 2 alloc error\n")
TRACE_MSG(RTSP_RESPONSE_1317_112_2_17_23_41_51_2299,"\n RTSP RTSPRSP_ParseTransportHeader 3 alloc error\n")
TRACE_MSG(RTSP_RESPONSE_1356_112_2_17_23_41_51_2300,"\n RTSP RTSPRSP_ParseTransportHeader SUCCESS\n")
TRACE_MSG(RTSP_SERIALIZE_537_112_2_17_23_41_52_2301,"\n rtsp RTSPSERIALIZE_InitReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_833_112_2_17_23_41_53_2302,"\n rtsp RTSPSERIALIZE_OptionsReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_1545_112_2_17_23_41_54_2303,"\n rtsp RTSPSERIALIZE_OptionsAuthRspSigT error")
TRACE_MSG(RTSP_SERIALIZE_1641_112_2_17_23_41_54_2304,"\n rtsp RTSPSERIALIZE_DescribeReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_2018_112_2_17_23_41_55_2305,"\n rtsp RTSPSERIALIZE_DescribeAuthRspSigT error")
TRACE_MSG(RTSP_SERIALIZE_2114_112_2_17_23_41_55_2306,"\n rtsp RTSPSERIALIZE_PlayReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_2369_112_2_17_23_41_56_2307,"\n rtsp RTSPSERIALIZE_PauseReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_2624_112_2_17_23_41_56_2308,"\n rtsp RTSPSERIALIZE_SetupReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_3113_112_2_17_23_41_57_2309,"\n rtsp RTSPSERIALIZE_DescribeAuthRspSigT error")
TRACE_MSG(RTSP_SERIALIZE_3183_112_2_17_23_41_58_2310,"\n rtsp RTSPSERIALIZE_PlayAuthRspSigT error")
TRACE_MSG(RTSP_SERIALIZE_3306_112_2_17_23_41_58_2311,"\n rtsp RTSPSERIALIZE_TeardownReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_3463_112_2_17_23_41_58_2312,"\n rtsp RTSPSERIALIZE_StopReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_3656_112_2_17_23_41_59_2313,"\n rtsp RTSPSERIALIZE_AnnounceReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_3977_112_2_17_23_42_0_2314,"\n rtsp RTSPSERIALIZE_PlayReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_4233_112_2_17_23_42_0_2315,"\n rtsp RTSPSERIALIZE_SetparamReqSigT error")
TRACE_MSG(RTSP_SERIALIZE_4587_112_2_17_23_42_1_2316,"\n rtsp RTSPSERIALIZE_SetparamAuthRspSigT error")
TRACE_MSG(RTSP_SETPARAM_HSM_140_112_2_17_23_42_2_2317,"RTSP RTSPHSM_SetparamDispatchSignal dispatch signal")
TRACE_MSG(RTSP_SETPARAM_HSM_142_112_2_17_23_42_2_2318,"RTSP RTSPHSM_SetparamDispatchSignal hsm_state =%d")
TRACE_MSG(RTSP_SETPARAM_HSM_146_112_2_17_23_42_2_2319,"RTSP RTSPHSM_SetparamDispatchSignal event =%d")
TRACE_MSG(RTSP_SETPARAM_HSM_178_112_2_17_23_42_2_2320,"\n rtsp RtspSetparamConstructor param error\n")
TRACE_MSG(RTSP_SETPARAM_HSM_195_112_2_17_23_42_2_2321,"\n rtsp RtspSetparamInitial param error\n")
TRACE_MSG(RTSP_SETPARAM_HSM_211_112_2_17_23_42_2_2322,"\n rtsp RtspSetparamDestructor param error\n")
TRACE_MSG(RTSP_SETPARAM_HSM_238_112_2_17_23_42_2_2323,"\n rtsp rtsp Rtsp_SetparamPending error\n")
TRACE_MSG(RTSP_SETPARAM_HSM_257_112_2_17_23_42_2_2324,"RTSP Rtsp_SetparamPendingIdle recv RTSP_HSM_SETPARAM_REQ")
TRACE_MSG(RTSP_SETPARAM_HSM_269_112_2_17_23_42_2_2325,"\n rtsp Rtsp_SetparamPendingIdle param error\n")
TRACE_MSG(RTSP_SETPARAM_HSM_294_112_2_17_23_42_2_2326,"RTSP Rtsp_SetparamPendingConnecting recv RTSP_HSM_CONNECT_SUCC_IND")
TRACE_MSG(RTSP_SETPARAM_HSM_300_112_2_17_23_42_2_2327,"RTSP Rtsp_SetparamPendingConnecting recv RTSP_HSM_CONNECT_FAIL_IND")
TRACE_MSG(RTSP_SETPARAM_HSM_346_112_2_17_23_42_2_2328,"RTSP Rtsp_SetparamPendingSending recv RTSP_HSM_SEND_SUCC_IND")
TRACE_MSG(RTSP_SETPARAM_HSM_350_112_2_17_23_42_2_2329,"RTSP Rtsp_SetparamPendingSending recv RTSP_HSM_SEND_FAIL_IND")
TRACE_MSG(RTSP_SETPARAM_HSM_373_112_2_17_23_42_2_2330,"\n rtsp Rtsp_SetparamPendingSending param error\n")
TRACE_MSG(RTSP_SETPARAM_HSM_421_112_2_17_23_42_2_2331,"RTSP Rtsp_SetparamPendingRecving recv RTSP_HSM_SETPARAM_AUTH_IND")
TRACE_MSG(RTSP_SETPARAM_HSM_452_112_2_17_23_42_3_2332,"RTSP Rtsp_SetparamPendingRecving recv RTSP_HSM_SETPARAM_OK_IND")
TRACE_MSG(RTSP_SETPARAM_HSM_474_112_2_17_23_42_3_2333,"RTSP Rtsp_SetparamPendingRecving recv RTSP_HSM_SETPARAM_ERROR_IND or RTSP_HSM_TIMEOUT_IND")
TRACE_MSG(RTSP_SETPARAM_HSM_498_112_2_17_23_42_3_2334,"RTSP Rtsp_SetparamPendingRecving recv RTSP_HSM_SETPARAM_REDIRECT_IND")
TRACE_MSG(RTSP_SETPARAM_HSM_513_112_2_17_23_42_3_2335,"\n rtsp Rtsp_SetparamPendingRecving param error\n")
TRACE_MSG(RTSP_SETPARAM_HSM_540_112_2_17_23_42_3_2336,"RTSP Rtsp_SetparamPendingAuthPending recv RTSP_HSM_SETPARAM_AUTH_RSP")
TRACE_MSG(RTSP_SETPARAM_HSM_579_112_2_17_23_42_3_2337,"\n rtsp Rtsp_SetparamPendingAuthPending param error\n")
TRACE_MSG(RTSP_SETPARAM_HSM_600_112_2_17_23_42_3_2338,"RTSP RtspHandleSetparamPendingAuthRsp input error, %d,%d")
TRACE_MSG(RTSP_SETPARAM_HSM_608_112_2_17_23_42_3_2339,"RTSP RtspHandleSetparamPendingAuthRsp host_ptr = PNULL")
TRACE_MSG(RTSP_SETPARAM_HSM_615_112_2_17_23_42_3_2340,"RTSP RtspHandleSetparamPendingAuthRsp request_ptr = PNULL")
TRACE_MSG(RTSP_SETPARAM_HSM_625_112_2_17_23_42_3_2341,"RTSP RtspHandleSetparamPendingAuthRsp RTSP_SaveAuthParam error")
TRACE_MSG(RTSP_SETPARAM_HSM_631_112_2_17_23_42_3_2342,"RTSP RtspHandleSetparamPendingAuthRsp rtsp_context_ptr = PNULL")
TRACE_MSG(RTSP_SETPARAM_HSM_638_112_2_17_23_42_3_2343,"RTSP RtspHandleSetparamPendingAuthRsp RTSP_SaveAuthParamToContext error")
TRACE_MSG(RTSP_SETPARAM_HSM_647_112_2_17_23_42_3_2344,"RTSP RtspHandleSetparamPendingAuthRsp NETMGR_SetProxyAuthenInfo error")
TRACE_MSG(RTSP_SIGNAL_46_112_2_17_23_42_3_2345,"\n rtsp RTSPSIG_SendOptionsCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_80_112_2_17_23_42_3_2346,"\n rtsp RTSPSIG_SendOptionsAuthInd status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_114_112_2_17_23_42_3_2347,"\n rtsp RTSPSIG_SendSetparamCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_148_112_2_17_23_42_3_2348,"\n rtsp RTSPSIG_SendSetparamAuthInd status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_180_112_2_17_23_42_3_2349,"\n rtsp RTSPSIG_SendInitCnf task id = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_211_112_2_17_23_42_3_2350,"\n rtsp RTSPSIG_SendCloseCnf task id = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_244_112_2_17_23_42_4_2351,"\n rtsp RTSPSIG_SendSetparamCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_278_112_2_17_23_42_4_2352,"\n rtsp RTSPSIG_SendSetparamAuthInd status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_312_112_2_17_23_42_4_2353,"\n rtsp RTSPSIG_SendPlayCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_346_112_2_17_23_42_4_2354,"\n rtsp RTSPSIG_SendPauseCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_380_112_2_17_23_42_4_2355,"\n rtsp RTSPSIG_SendAnnounceCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_414_112_2_17_23_42_4_2356,"\n rtsp RTSPSIG_SendRecordCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_448_112_2_17_23_42_4_2357,"\n rtsp RTSPSIG_SendTeardownCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_485_112_2_17_23_42_4_2358,"\n rtsp RTSPSIG_SendSetupCnf status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_495_112_2_17_23_42_4_2359,"\n rtsp RTSPSIG_SendSetupCnf")
TRACE_MSG(RTSP_SIGNAL_520_112_2_17_23_42_4_2360,"\n rtsp RTSPSIG_SendSetupAuthInd status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_555_112_2_17_23_42_4_2361,"\n rtsp RTSPSIG_SendPlayAuthInd status code = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_564_112_2_17_23_42_4_2362,"RTSPSIG_SendPlayAuthInd SIGNAL =%x,sender =%d")
TRACE_MSG(RTSP_SIGNAL_592_112_2_17_23_42_4_2363,"RTSP RTSPSIG_StopReq context_id =%d")
TRACE_MSG(RTSP_SIGNAL_599_112_2_17_23_42_4_2364,"\n rtsp RTSPSIG_StopReq task id = %d")
TRACE_MSG(RTSP_SIGNAL_616_112_2_17_23_42_4_2365,"\n rtsp RTSPSIG_StopReq error_code = %d")
TRACE_MSG(RTSP_SIGNAL_639_112_2_17_23_42_4_2366,"RTSP RTSPSIG_StopSelfReq context_id =%d")
TRACE_MSG(RTSP_SIGNAL_646_112_2_17_23_42_5_2367,"\n rtsp RTSPSIG_StopReq task id = %d")
TRACE_MSG(RTSP_SIGNAL_663_112_2_17_23_42_5_2368,"\n rtsp RTSPSIG_StopReq error_code = %d")
TRACE_MSG(RTSP_SIGNAL_735_112_2_17_23_42_5_2369,"\n rtsp RTSPSIG_SendServerClose task id = %d,context_id =%d")
TRACE_MSG(RTSP_SIGNAL_765_112_2_17_23_42_5_2370,"RTSP RTSP_OptionsSelfReq context_id =%d")
TRACE_MSG(RTSP_SIGNAL_772_112_2_17_23_42_5_2371,"\n rtsp RTSP_OptionsReq task id = %d")
TRACE_MSG(RTSP_SIGNAL_789_112_2_17_23_42_5_2372,"\n rtsp RTSP_OptionsReq error_code = %d")
TRACE_MSG(SNTP_192_112_2_17_23_42_6_2373,"[SNTP] Command  SNTP_SIG_GET_TIME_REQ \n")
TRACE_MSG(SNTP_196_112_2_17_23_42_6_2374,"[SNTP] Command SOCKET_READ_EVENT_IND \n")
TRACE_MSG(SNTP_200_112_2_17_23_42_6_2375,"[SNTP] Command SNTP_SIG_UDP_SOCKET_TIMER_IND \n")
TRACE_MSG(SNTP_204_112_2_17_23_42_6_2376,"[SNTP] Command  SNTP_Main default \n")
TRACE_MSG(SNTP_264_112_2_17_23_42_6_2377,"[SNTP] Command  SNTP_ReqNetTimeProc ret =%d \n")
TRACE_MSG(SNTP_293_112_2_17_23_42_7_2378,"[SNTP] Command  SNTP_ReqSocketReadProc ret =%d \n")
TRACE_MSG(SNTP_321_112_2_17_23_42_7_2379,"[SNTP] Command  SNTP_NetSendHandle SNTP_HD_SOCKET_SEND_ERROR \n")
TRACE_MSG(SNTP_326_112_2_17_23_42_7_2380,"[SNTP] Command  SNTP_NetSendHandle!ret_val=%d \n")
TRACE_MSG(SNTP_371_112_2_17_23_42_7_2381,"[SNTP] Command  SNTP_NetReadHandle SNTP_HD_SOCKET_RECEIVE_ERROR\n")
TRACE_MSG(SNTP_375_112_2_17_23_42_7_2382,"[SNTP] Command  SNTP_NetReadHandle ret =%d \n")
TRACE_MSG(SNTP_400_112_2_17_23_42_7_2383,"[SNTP] Command  SNTP_NetTimeReqDnsHandle SNTP_HD_DNS_ERROR \n")
TRACE_MSG(SNTP_413_112_2_17_23_42_7_2384,"[SNTP] Command  SNTP_NetTimeReqDnsHandle SNTP_HD_SOCKET_ERROR \n")
TRACE_MSG(SNTP_419_112_2_17_23_42_7_2385,"[SNTP] Command  SNTP_NetTimeReqDnsHandle ret =%d \n")
TRACE_MSG(SNTP_444_112_2_17_23_42_7_2386,"[SNTP] Command  SNTP_NetTimeReqIPHandle SNTP_HD_SOCKET_ERROR \n")
TRACE_MSG(SNTP_450_112_2_17_23_42_7_2387,"[SNTP] Command  SNTP_NetTimeReqIPHandle ret =%d \n")
TRACE_MSG(SNTP_498_112_2_17_23_42_7_2388,"[SNTP] SNTP_NetTimeReqCNF!ret_val =%d\n")
TRACE_MSG(SNTP_558_112_2_17_23_42_7_2389,"[SNTP] SNTP_SocketTimerOutProc!ret_val =%d\n")
TRACE_MSG(SNTP_API_71_112_2_17_23_42_7_2390,"[SNTP] Command is: SNTP_GetNTPTime ret=%d\n")
TRACE_MSG(SNTP_UNIT_49_112_2_17_23_42_9_2391,"[SNTP] SNTP_SockCreat sock ERROR")
TRACE_MSG(SNTP_UNIT_60_112_2_17_23_42_9_2392,"[SNTP] SNTP SockCreat  is fail")
TRACE_MSG(SNTP_UNIT_66_112_2_17_23_42_9_2393,"[SNTP]SNTP SockCreat setopt ERROR")
TRACE_MSG(SNTP_UNIT_97_112_2_17_23_42_9_2394,"[SNTP] SNTP_SendtoData  is error")
TRACE_MSG(SNTP_UNIT_124_112_2_17_23_42_9_2395,"[SNTP] SNTP RevData fd is error")
TRACE_MSG(SNTP_UNIT_180_112_2_17_23_42_9_2396,"[SNTP] SNTP_GetHostByName!ret_val =%d\n")
TRACE_MSG(SNTP_UNIT_202_112_2_17_23_42_9_2397,"[SNTP] SNTP_GetHostByNameAsync is ERROR")
TRACE_MSG(SNTP_UNIT_290_112_2_17_23_42_10_2398,"[SNTP] SNTP_FindNoteByIndex!ret_val =%d\n")
TRACE_MSG(SNTP_UNIT_313_112_2_17_23_42_10_2399,"[SNTP] SNTP_UpdateNodeByNodeHand!ret_val =%d\n")
TRACE_MSG(SNTP_UNIT_344_112_2_17_23_42_10_2400,"[SNTP] SNTP_DeleteNoteByIndex!ret_val =%d\n")
TRACE_MSG(SNTP_UNIT_368_112_2_17_23_42_10_2401,"[SNTP] SNTP_SerializeNettimeReqSigT!\n")
TRACE_MSG(MD2_93_112_2_17_23_42_14_2402,"ssl matrixMd2Init invalid input")
TRACE_MSG(MD2_108_112_2_17_23_42_14_2403,"ssl matrixMd2Update invalid input")
TRACE_MSG(MD2_135_112_2_17_23_42_14_2404,"ssl matrixMd2Final invalid input")
TRACE_MSG(RSA_79_112_2_17_23_42_23_2405,"ssl_rsa_crypt error: mp_init_multi\n")
TRACE_MSG(RSA_84_112_2_17_23_42_23_2406,"ssl_rsa_crypt error: mp_read_unsigned_bin\n")
TRACE_MSG(RSA_97_112_2_17_23_42_23_2407,"decrypt error: mp_exptmod dP, p\n")
TRACE_MSG(RSA_102_112_2_17_23_42_23_2408,"decrypt error: mp_exptmod dQ, q\n")
TRACE_MSG(RSA_106_112_2_17_23_42_23_2409,"decrypt error: sub tmpb, tmp\n")
TRACE_MSG(RSA_110_112_2_17_23_42_23_2410,"decrypt error: mp_mulmod qP, p\n")
TRACE_MSG(RSA_114_112_2_17_23_42_23_2411,"decrypt error: mp_mul q \n")
TRACE_MSG(RSA_118_112_2_17_23_42_23_2412,"decrypt error: mp_add tmp \n")
TRACE_MSG(RSA_123_112_2_17_23_42_23_2413,"ssl_rsa_crypt error: mp_exptmod\n")
TRACE_MSG(RSA_129_112_2_17_23_42_23_2414,"ssl_rsa_crypt error: mp_exptmod\n")
TRACE_MSG(RSA_133_112_2_17_23_42_23_2415,"ssl_rsa_crypt error: invalid type param\n")
TRACE_MSG(RSA_141_112_2_17_23_42_23_2416,"ssl_rsa_crypt error: mp_unsigned_bin_size\n")
TRACE_MSG(RSA_157_112_2_17_23_42_23_2417,"ssl_rsa_crypt error: mp_to_unsigned_bin\n")
TRACE_MSG(RSA_191_112_2_17_23_42_23_2418,"RSA encryption data too large: %d\n")
TRACE_MSG(RSA_205_112_2_17_23_42_23_2419,"Error gathering RSA pad entropy\n")
TRACE_MSG(ASN1_53_112_2_17_23_42_26_2420,"Unsupported: ASN indefinite length\n")
TRACE_MSG(ASN1_93_112_2_17_23_42_26_2421,"ASN getBig failed\n")
TRACE_MSG(ASN1_99_112_2_17_23_42_26_2422,"ASN getBig failed\n")
TRACE_MSG(ASN1_120_112_2_17_23_42_26_2423,"ASN getSerialNumber failed\n")
TRACE_MSG(ASN1_126_112_2_17_23_42_26_2424,"ASN getSerialNumber failed\n")
TRACE_MSG(ASN1_240_112_2_17_23_42_26_2425,"Unsupported algorithm identifier\n")
TRACE_MSG(ASN1_396_112_2_17_23_42_26_2426,"ASN getInteger failed\n")
TRACE_MSG(ASN1_404_112_2_17_23_42_26_2427,"ASN getInteger failed\n")
TRACE_MSG(RSAPKI_74_112_2_17_23_42_27_2428,"Osdep open failure\n")
TRACE_MSG(RSAPKI_145_112_2_17_23_42_27_2429,"ssl filename is null")
TRACE_MSG(RSAPKI_160_112_2_17_23_42_27_2430,"SFS_CreateFile() error\n")
TRACE_MSG(RSAPKI_168_112_2_17_23_42_27_2431,"SFS_GetFileSize() error\n")
TRACE_MSG(RSAPKI_182_112_2_17_23_42_27_2432,"SFS_ReadFile() error\n")
TRACE_MSG(RSAPKI_197_112_2_17_23_42_27_2433,"SFS_CloseFile() error\n")
TRACE_MSG(RSAPKI_240_112_2_17_23_42_27_2434,"Error parsing private key buffer\n")
TRACE_MSG(RSAPKI_253_112_2_17_23_42_27_2435,"No password given for encrypted private key\n")
TRACE_MSG(RSAPKI_258_112_2_17_23_42_27_2436,"Unrecognized private key file encoding\n")
TRACE_MSG(RSAPKI_266_112_2_17_23_42_27_2437,"Invalid private key file salt\n")
TRACE_MSG(RSAPKI_276_112_2_17_23_42_27_2438,"3DES has been disabled for private key decrypt\n")
TRACE_MSG(RSAPKI_283_112_2_17_23_42_27_2439,"Error parsing private key buffer\n")
TRACE_MSG(RSAPKI_299_112_2_17_23_42_27_2440,"Unable to base64 decode private key\n")
TRACE_MSG(RSAPKI_377_112_2_17_23_42_27_2441,"Unable to ASN parse private key.\n")
TRACE_MSG(RSAPKI_451_112_2_17_23_42_28_2442,"ASN sequence parse error\n")
TRACE_MSG(RSAPKI_465_112_2_17_23_42_28_2443,"ASN key extract parse error\n")
TRACE_MSG(RSAPKI_584_112_2_17_23_42_28_2444,"Parsing untested DN attrib type\n")
TRACE_MSG(X509_101_112_2_17_23_42_28_2445,"Error: Calling matrixRsaReadKeys against a library\n")
TRACE_MSG(X509_102_112_2_17_23_42_28_2446,"built without USE_FILE_SYSTEM defined\n")
TRACE_MSG(X509_202_112_2_17_23_42_29_2447,"Error reading private key file: %s\n")
TRACE_MSG(X509_209_112_2_17_23_42_29_2448,"Error parsing private key file: %s\n")
TRACE_MSG(X509_230_112_2_17_23_42_29_2449,"Error reading CA cert file %s\n")
TRACE_MSG(X509_239_112_2_17_23_42_29_2450,"Error parsing CA cert %s\n")
TRACE_MSG(X509_258_112_2_17_23_42_29_2451,"No valid CA certs in %s\n")
TRACE_MSG(X509_305_112_2_17_23_42_29_2452,"Exceeded maximum cert chain length of %dn")
TRACE_MSG(X509_310_112_2_17_23_42_29_2453,"Couldn't open file %s\n")
TRACE_MSG(X509_330_112_2_17_23_42_29_2454,"Unable to base64 decode certificate\n")
TRACE_MSG(X509_381_112_2_17_23_42_29_2455,"Unable to read certificate file %s\n")
TRACE_MSG(X509_417_112_2_17_23_42_29_2456,"Error reading cert file %s\n")
TRACE_MSG(X509_511_112_2_17_23_42_29_2457,"Error reading private key mem\n")
TRACE_MSG(X509_521_112_2_17_23_42_29_2458,"Error parsing CA cert\n")
TRACE_MSG(X509_608_112_2_17_23_42_30_2459,"Initial cert parse error\n")
TRACE_MSG(X509_628_112_2_17_23_42_30_2460,"ASN sequence parse error\n")
TRACE_MSG(X509_637_112_2_17_23_42_30_2461,"ASN version parse error\n")
TRACE_MSG(X509_642_112_2_17_23_42_30_2462,"Warning: non-v3 certificate version: %dn")
TRACE_MSG(X509_648_112_2_17_23_42_30_2463,"ASN serial number parse error\n")
TRACE_MSG(X509_730_112_2_17_23_42_30_2464,"Parse error: mismatched signature type\n")
TRACE_MSG(X509_969_112_2_17_23_42_30_2465,"Error parsing cert extension\n")
TRACE_MSG(X509_979_112_2_17_23_42_30_2466,"Expecting OCTET STRING in ext parse\n")
TRACE_MSG(X509_1082_112_2_17_23_42_30_2467,"Unsupported subjectAltName type\n")
TRACE_MSG(X509_1132_112_2_17_23_42_31_2468,"Error auth key-id name type: %dn")
TRACE_MSG(X509_1215_112_2_17_23_42_31_2469,"Unknown critical ext encountered\n")
TRACE_MSG(X509_1286_112_2_17_23_42_31_2470,"Warning:  No CA to validate cert with\n")
TRACE_MSG(X509_1287_112_2_17_23_42_31_2471,"\tPerforming self-signed CA test\n")
TRACE_MSG(X509_1332_112_2_17_23_42_31_2472,"Unsupported signature algorithm\n")
TRACE_MSG(X509_1337_112_2_17_23_42_31_2473,"ssl matrixX509ValidateCertInternal wrong sigLen")
TRACE_MSG(CIPHERSUITE_376_112_2_17_23_42_32_2474,"Error: output buffer too small for NULL encrypt\n")
TRACE_MSG(CIPHERSUITE_387_112_2_17_23_42_32_2475,"Error: output buffer too small for NULL decrypt\n")
TRACE_MSG(SSL_ALERT_60_112_2_17_23_42_32_2476,"SSL_AlertSend : invalid input")
TRACE_MSG(SSL_ALERT_67_112_2_17_23_42_32_2477,"SSL_AlertSend : PDU create fail")
TRACE_MSG(SSL_ALERT_78_112_2_17_23_42_32_2478,"SSL_AlertSend :pdu pack fail")
TRACE_MSG(SSL_ALERT_85_112_2_17_23_42_32_2479,"SSL_AlertSend :payload pack fail")
TRACE_MSG(SSL_ALERT_122_112_2_17_23_42_32_2480,"SSL_AlertEventHandle : invalid input")
TRACE_MSG(SSL_ALERT_132_112_2_17_23_42_32_2481,"SSL_ALERTEVENTHANDLE :error event create fail")
TRACE_MSG(SSL_ALERT_145_112_2_17_23_42_33_2482,"SSL_ALERTEVENTHANDLE :warning event create fail")
TRACE_MSG(SSL_ALGOPROCESS_286_112_2_17_23_42_35_2483,"SSLALGOPROC_CalculateSslPrf: encounter problem\n")
TRACE_MSG(SSL_ALGOPROCESS_723_112_2_17_23_42_36_2484,"SSL_CALC: alloc space for tempout_ptr fail")
TRACE_MSG(SSL_ALGOPROCESS_818_112_2_17_23_42_36_2485,"SSL_CALC: alloc temp_output_ptr fail")
TRACE_MSG(SSL_ALGOPROCESS_1585_112_2_17_23_42_37_2486,"SSL_DECRYPT: not space for decrypt data")
TRACE_MSG(SSL_ALGOPROCESS_1635_112_2_17_23_42_37_2487,"SSL_DECRYPT: SSL_OctstrGetChar error")
TRACE_MSG(SSL_ALGOPROCESS_1640_112_2_17_23_42_37_2488,"SSL_DECRYPT: Get padding length error")
TRACE_MSG(SSL_ALGOPROCESS_1648_112_2_17_23_42_37_2489,"SSL_DECRYPT: SSL_OctstrDelete error")
TRACE_MSG(SSL_ALGOPROCESS_1728_112_2_17_23_42_38_2490,"SSL_ENCRYPT: tls calculate client mac fail")
TRACE_MSG(SSL_ALGOPROCESS_1827_112_2_17_23_42_38_2491,"SSL_ENCRYPT: NOT SPACE FOR THE ENCRYPTED DATA")
TRACE_MSG(SSL_ALGOPROCESS_1871_112_2_17_23_42_38_2492,"SSL_DataEncrypt:event create fail")
TRACE_MSG(SSL_ALGOPROCESS_1902_112_2_17_23_42_38_2493,"ssl_decryptpdulist: invalid input")
TRACE_MSG(SSL_ALGOPROCESS_1918_112_2_17_23_42_38_2494,"SSL_DecryptPDU: decrypt pdu fail")
TRACE_MSG(SSL_ALGOPROCESS_2003_112_2_17_23_42_38_2495,"SSL_VERIFYMAC: tlsCALCULATE SERVER MAC FAIL")
TRACE_MSG(SSL_ALGOPROCESS_2358_112_2_17_23_42_39_2496,"SSL_ALGOPROC: calculate tls prf fail")
TRACE_MSG(SSL_ALGOPROCESS_2389_112_2_17_23_42_39_2497,"SSL_GetRsaPublickeyincert: invalid input")
TRACE_MSG(SSL_API_121_112_2_17_23_42_39_2498,"SSL_AsyncRegCallback:the handle not exist\n")
TRACE_MSG(SSL_API_148_112_2_17_23_42_40_2499,"SSL_Init:can't support half handshake\n ")
TRACE_MSG(SSL_API_189_112_2_17_23_42_40_2500,"SSL_Create : input is invalid\n")
TRACE_MSG(SSL_API_196_112_2_17_23_42_40_2501,"ssl_create: create machine fail!\n")
TRACE_MSG(SSL_API_202_112_2_17_23_42_40_2502,"SSL_Create 0x%x")
TRACE_MSG(SSL_API_222_112_2_17_23_42_40_2503,"SSL_CLOSE 0x%x")
TRACE_MSG(SSL_API_227_112_2_17_23_42_40_2504,"SSL_CLOSE: ssl_handle is invalid\n")
TRACE_MSG(SSL_API_234_112_2_17_23_42_40_2505,"SSL_Close : handle not find in table\n")
TRACE_MSG(SSL_API_258_112_2_17_23_42_40_2506,"SSL_ProtocolChoose: the handler is invalid\n")
TRACE_MSG(SSL_API_290_112_2_17_23_42_40_2507,"SSL_Handshake: the input is invalid\n")
TRACE_MSG(SSL_API_297_112_2_17_23_42_40_2508,"SSL_Handshake: the handler not registered\n")
TRACE_MSG(SSL_API_325_112_2_17_23_42_40_2509,"SSL_Handshake: client hello event create fail\n")
TRACE_MSG(SSL_API_354_112_2_17_23_42_40_2510,"SSL_DecryptPasser: len=%d")
TRACE_MSG(SSL_API_363_112_2_17_23_42_40_2511,"SSL_DecryptPasser: the input is invalid\n")
TRACE_MSG(SSL_API_369_112_2_17_23_42_40_2512,"SSL_DecryptPasser: handler not registered\n")
TRACE_MSG(SSL_API_450_112_2_17_23_42_40_2513,"SSL_DecryptPasser : error code")
TRACE_MSG(SSL_API_473_112_2_17_23_42_40_2514,"SSL_EncryptPasser: len=%d")
TRACE_MSG(SSL_API_476_112_2_17_23_42_40_2515,"SSL_EncryptPasser: input invalid\n")
TRACE_MSG(SSL_API_482_112_2_17_23_42_40_2516,"SSL_EncryptPasser: handler not registered\n")
TRACE_MSG(SSL_API_536_112_2_17_23_42_40_2517,"SSL_EncryptPasser: error code\n")
TRACE_MSG(SSL_API_561_112_2_17_23_42_40_2518,"SSL_AsyncMessageProc: input invalid\n")
TRACE_MSG(SSL_API_567_112_2_17_23_42_40_2519,"SSL_AsyncMessageProc: handler not registered\n")
TRACE_MSG(SSL_API_605_112_2_17_23_42_40_2520,"SSL_AsyncMessageProc: no memeory\n")
TRACE_MSG(SSL_API_614_112_2_17_23_42_40_2521,"SSL_AsyncMessageProc: file open error\n")
TRACE_MSG(SSL_API_631_112_2_17_23_42_41_2522,"SSL_AsyncMessageProc: no memeory_2\n")
TRACE_MSG(SSL_API_640_112_2_17_23_42_41_2523,"SSL_AsyncMessageProc: file open error_2\n")
TRACE_MSG(SSL_API_660_112_2_17_23_42_41_2524,"SSL WILL IGNORE IT")
TRACE_MSG(SSL_API_685_112_2_17_23_42_41_2525,"SSL_EncryptPasserFile: input invalid")
TRACE_MSG(SSL_API_691_112_2_17_23_42_41_2526,"SSL_EncryptPasserFile: handler not registered")
TRACE_MSG(SSL_API_699_112_2_17_23_42_41_2527,"SSL_ENCRYPTPASSERFILE :SFS_CreateFile() error\n")
TRACE_MSG(SSL_API_706_112_2_17_23_42_41_2528,"SSL_ENCRYPTPASSERFILE:SFS_GetFileSize() error\n")
TRACE_MSG(SSL_API_722_112_2_17_23_42_41_2529,"SSL_ENCRYPTFILE: can not allocate enough space")
TRACE_MSG(SSL_API_739_112_2_17_23_42_41_2530,"SSL_ENCRYPTPASSERFILE:SFS_ReadFile() error\n")
TRACE_MSG(SSL_API_761_112_2_17_23_42_41_2531,"SSL_ENCRYPTPASSERFILE:SFS_ReadFile() error\n")
TRACE_MSG(SSL_API_785_112_2_17_23_42_41_2532,"SSL_ENCRYPTPASSERFILE:SFS_ReadFile() error\n")
TRACE_MSG(SSL_API_797_112_2_17_23_42_41_2533,"SSL_ENCRYPTPASSERFILE:error code\n")
TRACE_MSG(SSL_API_828_112_2_17_23_42_41_2534,"SSL_UserCnfCert enter!")
TRACE_MSG(SSL_API_831_112_2_17_23_42_41_2535,"SSL_UserCnfCert: input invalid\n")
TRACE_MSG(SSL_API_837_112_2_17_23_42_41_2536,"SSL_UserCnfCert: handler not registered\n")
TRACE_MSG(SSL_API_843_112_2_17_23_42_41_2537,"SSL_UserCnfCert user trust the cert")
TRACE_MSG(SSL_API_856_112_2_17_23_42_41_2538,"SSL_UserCnf: event create fail\n")
TRACE_MSG(SSL_API_863_112_2_17_23_42_41_2539,"SSL_UserCnfCert user do not trust cert")
TRACE_MSG(SSL_API_873_112_2_17_23_42_41_2540,"SSL_UserCnf: error_code \n")
TRACE_MSG(SSL_API_892_112_2_17_23_42_41_2541,"SSL_AddMachineToTable: ssl_handle is invalid")
TRACE_MSG(SSL_API_914_112_2_17_23_42_41_2542,"SSL_ADDMachinToTable: we need larger number macro\n")
TRACE_MSG(SSL_API_928_112_2_17_23_42_41_2543,"SSL_DeleteMachineFromTable: input is invalid")
TRACE_MSG(SSL_API_950_112_2_17_23_42_41_2544,"SSL_DeleteMachineFromTable:handle not found\n")
TRACE_MSG(SSL_API_986_112_2_17_23_42_41_2545,"SSL_GetMachineInTable: input is invalid\n")
TRACE_MSG(SSL_API_1023_112_2_17_23_42_41_2546,"SSL_GetCurrentCertInfo: cert is invalid\n")
TRACE_MSG(SSL_API_1029_112_2_17_23_42_41_2547,"SSL_GetCurrentCertInfo: handler not registered\n")
TRACE_MSG(SSL_API_1037_112_2_17_23_42_42_2548,"SSL_GETCURRENTINFO: not after ptr alloc fail\n")
TRACE_MSG(SSL_API_1048_112_2_17_23_42_42_2549,"SSL_GETCURRENTINFO: not before ptr alloc fail\n")
TRACE_MSG(SSL_API_1059_112_2_17_23_42_42_2550,"SSL_GETCURRENTINFO: serialnumber ptr alloc fail\n")
TRACE_MSG(SSL_API_1075_112_2_17_23_42_42_2551,"SSL_GETCURRENTINFO: subjectid ptr alloc fail\n")
TRACE_MSG(SSL_API_1086_112_2_17_23_42_42_2552,"SSL_GETCURRENTINFO: unique userid ptr alloc fail")
TRACE_MSG(SSL_API_1100_112_2_17_23_42_42_2553,"SSL_GETCURRENTINFO: issuser name ptr alloc fail\n")
TRACE_MSG(SSL_API_1111_112_2_17_23_42_42_2554,"SSL_GETCURRENTINFO: issuer country ptr alloc fail\n")
TRACE_MSG(SSL_API_1122_112_2_17_23_42_42_2555,"SSL_GETCURRENTINFO: issuer hash ptr alloc fail\n")
TRACE_MSG(SSL_API_1133_112_2_17_23_42_42_2556,"SSL_GETCURRENTINFO:issuer locality ptr alloc fail\n")
TRACE_MSG(SSL_API_1144_112_2_17_23_42_42_2557,"SSL_GETCURRENTINFO: issuer organization ptr alloc fail\n")
TRACE_MSG(SSL_API_1155_112_2_17_23_42_42_2558,"SSL_GETCURRENTINFO: issuer orgunit alloc fail\n")
TRACE_MSG(SSL_API_1166_112_2_17_23_42_42_2559,"SSL_GETCURRENTINFO: issuer state ptr alloc fail\n")
TRACE_MSG(SSL_API_1177_112_2_17_23_42_42_2560,"SSL_GETCURRENTINFO: issuer commonname alloc fail\n")
TRACE_MSG(SSL_API_1188_112_2_17_23_42_42_2561,"SSL_GETCURRENTINFO:subject country ptr alloc fail\n")
TRACE_MSG(SSL_API_1199_112_2_17_23_42_42_2562,"SSL_GETCURRENTINFO:subject hash alloc fail\n")
TRACE_MSG(SSL_API_1210_112_2_17_23_42_42_2563,"SSL_GETCURRENTINFO: subject locality ptr alloc fail\n")
TRACE_MSG(SSL_API_1221_112_2_17_23_42_42_2564,"SSL_GETCURRENTINFO: subject orgnanization ptr alloc fail\n")
TRACE_MSG(SSL_API_1232_112_2_17_23_42_42_2565,"SSL_GETCURRENTINFO: subject orgunit ptr alloc fail\n")
TRACE_MSG(SSL_API_1243_112_2_17_23_42_42_2566,"SSL_GETCURRENTINFO:subject state ptr alloc fail\n")
TRACE_MSG(SSL_API_1362_112_2_17_23_42_42_2567,"SSL_FreeCurrentCertInfo: cert is invalid")
TRACE_MSG(SSL_API_1506_112_2_17_23_42_42_2568,"SSL_SessionSave: session_ptr is invalid")
TRACE_MSG(SSL_API_1560_112_2_17_23_42_43_2569,"SSL_LoadSession: list_ptr is invalid")
TRACE_MSG(SSL_API_1617_112_2_17_23_42_43_2570,"SSL_CertHashSaveFile: input is invalid")
TRACE_MSG(SSL_API_1641_112_2_17_23_42_43_2571,"SSL_CertHashSaveFile :SFS_CreateFile() error\n")
TRACE_MSG(SSL_API_1654_112_2_17_23_42_43_2572,"SSL_CertHashSaveFile :SFS_GetFileSize() error\n")
TRACE_MSG(SSL_API_1665_112_2_17_23_42_43_2573,"SSL_CertHashSaveFile :error_code\n")
TRACE_MSG(SSL_API_1695_112_2_17_23_42_43_2574,"SSL_LoadCertHashFromFile: input is invalid\n")
TRACE_MSG(SSL_API_1703_112_2_17_23_42_43_2575,"SSL_LoadCertHashFromFile :SFS_CreateFile() error\n")
TRACE_MSG(SSL_API_1710_112_2_17_23_42_43_2576,"SSL_LoadCertHashFromFile :SFS_GetFileSize() error\n")
TRACE_MSG(SSL_API_1723_112_2_17_23_42_43_2577,"SSL_LoadCertHashFromFile :SFS_ReadFile() error\n")
TRACE_MSG(SSL_API_1730_112_2_17_23_42_43_2578,"SSL_LoadCertHashFromFile :create hash_ptr fail error\n")
TRACE_MSG(SSL_API_1741_112_2_17_23_42_43_2579,"SSL_LoadCertHashFromFile :error_code\n")
TRACE_MSG(SSL_API_1775_112_2_17_23_42_43_2580,"SSL_SaveDataToFile SFS_CreateFile error!")
TRACE_MSG(SSL_CERT_100_112_2_17_23_42_43_2581,"SSL_LoadCertificate: input invalid")
TRACE_MSG(SSL_CERT_213_112_2_17_23_42_44_2582,"SSL_IsCertUserConfirm: input invalid")
TRACE_MSG(SSL_EVENT_421_112_2_17_23_42_45_2583,"SSL_EventCreate: malloc fail")
TRACE_MSG(SSL_EVENT_471_112_2_17_23_42_45_2584,"SSL_EVENTHANDLE: event type=%d")
TRACE_MSG(SSL_EVENT_476_112_2_17_23_42_45_2585,"SSL_EVENTHANDLE: EventAppendToQueue")
TRACE_MSG(SSL_EVENT_489_112_2_17_23_42_45_2586,"SSL_EVENTHANDLE: DECRYPT FAIL HERE NEED ALERT!!!")
TRACE_MSG(SSL_EVENT_502_112_2_17_23_42_45_2587,"SSL_EVENTHANDLE: SSLEVENT_EventHandle condition=%d")
TRACE_MSG(SSL_EVENT_526_112_2_17_23_42_45_2588,"SSL_EVENTHANDLE: state jump loop")
TRACE_MSG(SSL_HANDSHAKE_202_112_2_17_23_42_46_2589,"SSL_HANDRECEIVED DATA :invalid input\n")
TRACE_MSG(SSL_HANDSHAKE_220_112_2_17_23_42_46_2590,"SSL_HANDRECEIVED DATA :pdu unpack fail\n")
TRACE_MSG(SSL_HANDSHAKE_245_112_2_17_23_42_46_2591,"SSL_HANDRECEIVED DATA :error_code\n")
TRACE_MSG(SSL_HANDSHAKE_273_112_2_17_23_42_46_2592,"SSL_EncryptUserDataToSend: invalid input\n")
TRACE_MSG(SSL_HANDSHAKE_286_112_2_17_23_42_46_2593,"SSL_EncryptUserDataToSend: PDU create fail\n")
TRACE_MSG(SSL_HANDSHAKE_304_112_2_17_23_42_46_2594,"SSL_EncryptUserDataToSend: PDU add fail\n")
TRACE_MSG(SSL_HANDSHAKE_428_112_2_17_23_42_46_2595,"SSLHAND_SendClientHello :invalid input\n")
TRACE_MSG(SSL_HANDSHAKE_492_112_2_17_23_42_46_2596,"SSLHAND_SendClientHello : encounters error\n")
TRACE_MSG(SSL_HANDSHAKE_526_112_2_17_23_42_46_2597,"SSLHAND_HandleServerHello enter")
TRACE_MSG(SSL_HANDSHAKE_600_112_2_17_23_42_47_2598,"SSLHAND_HandleServerHello : after SSLEVENT_EventHandle")
TRACE_MSG(SSL_HANDSHAKE_612_112_2_17_23_42_47_2599,"SSLHAND_HandleServerHello : after SSL_HANDSHAKE_FULL")
TRACE_MSG(SSL_HANDSHAKE_617_112_2_17_23_42_47_2600,"SSLHAND_HandleServerHello end")
TRACE_MSG(SSL_HANDSHAKE_621_112_2_17_23_42_47_2601,"SSLHAND_HandleServerHello : encounters error\n")
TRACE_MSG(SSL_HANDSHAKE_674_112_2_17_23_42_47_2602,"SSLHAND_HandleServerCert : encounters error\n")
TRACE_MSG(SSL_HANDSHAKE_702_112_2_17_23_42_47_2603,"SSLHAND_HandleServerHelloDone")
TRACE_MSG(SSL_HANDSHAKE_726_112_2_17_23_42_47_2604,"SSLHAND_HandleServerHelloDone SSL_CERT_NOTCONFIRMED")
TRACE_MSG(SSL_HANDSHAKE_737_112_2_17_23_42_47_2605,"SSLHAND_HandleServerHelloDone SSLSTATE_SERVERHELLODONE")
TRACE_MSG(SSL_HANDSHAKE_747_112_2_17_23_42_47_2606,"SSLHAND_HandleServerHelloDone end")
TRACE_MSG(SSL_HANDSHAKE_751_112_2_17_23_42_47_2607,"SSLHAND_HandleServerHelloDone : encounters error\n")
TRACE_MSG(SSL_HANDSHAKE_851_112_2_17_23_42_47_2608,"SSLHAND_HandleClientExchange")
TRACE_MSG(SSL_HANDSHAKE_907_112_2_17_23_42_47_2609,"SSLHAND_HandleClientExchange : encounters error\n")
TRACE_MSG(SSL_HANDSHAKE_989_112_2_17_23_42_48_2610,"SSLHAND_HandleServerFinished : encounters error\n")
TRACE_MSG(SSL_HANDSHAKE_1022_112_2_17_23_42_48_2611,"SendChangeCipherAndFinshedMessage")
TRACE_MSG(SSL_HANDSHAKE_1065_112_2_17_23_42_48_2612,"SendChangeCipherAndFinshedMessage end")
TRACE_MSG(SSL_HANDSHAKE_1069_112_2_17_23_42_48_2613,"SendChangeCipherAndFinshedMessage: encounters error\n")
TRACE_MSG(SSL_HANDSHAKE_1143_112_2_17_23_42_48_2614,"SSL_HANDSHAKE,master key generate fail\n")
TRACE_MSG(SSL_HANDSHAKE_1182_112_2_17_23_42_48_2615,"SSLHAND_GenerateMasterKey: encounters error\n")
TRACE_MSG(SSL_MACHINE_116_112_2_17_23_42_49_2616,"")
TRACE_MSG(SSL_MACHINE_124_112_2_17_23_42_49_2617,"")
TRACE_MSG(SSL_MACHINE_317_112_2_17_23_42_49_2618,"SSLMACHINE_Destroy 0x%x")
TRACE_MSG(SSL_PDU_253_112_2_17_23_42_50_2619,"SSL_PDUPACK: encrypt fail")
TRACE_MSG(SSL_PDU_657_112_2_17_23_42_51_2620,"SSL_PduAdd: invalid input")
TRACE_MSG(SSL_PDU_671_112_2_17_23_42_51_2621,"SSL_PduAdd: pdu pack fail")
TRACE_MSG(SSL_PDU_681_112_2_17_23_42_51_2622,"SSL_PduAdd: payload pack fail")
TRACE_MSG(SSL_PDU_690_112_2_17_23_42_51_2623,"SSL_PduAdd: insert pdu fail")
TRACE_MSG(SSL_PDU_720_112_2_17_23_42_51_2624,"SSL_PayloadPack: invalid input")
TRACE_MSG(SSL_PDU_795_112_2_17_23_42_52_2625,"SSL_PayloadPack: string create fail")
TRACE_MSG(SSL_PDU_885_112_2_17_23_42_52_2626,"SSLPDU_UnpackPDU error\n")
TRACE_MSG(SSL_PDU_1134_112_2_17_23_42_52_2627,"SSL_packrsasecret: invalid input")
TRACE_MSG(SSL_PDU_1412_112_2_17_23_42_53_2628,"SSL_DATAGRAMUNPACK: event create fail")
TRACE_MSG(SSL_PDUSUPPORT_129_112_2_17_23_42_53_2629,"SSL_version:invalid input")
TRACE_MSG(SSL_PDUSUPPORT_605_112_2_17_23_42_54_2630,"ssl:GetRandom = %s")
TRACE_MSG(SSL_PDUSUPPORT_755_112_2_17_23_42_55_2631,"SSL_CREATECIPHERSUITE LIST FAIL")
TRACE_MSG(SSL_PDUSUPPORT_847_112_2_17_23_42_55_2632,"SSL_ENCRYPTSECRETE: rsa fail")
TRACE_MSG(STRM_API_70_112_2_17_23_42_56_2633,"#STRM STRM_Init!")
TRACE_MSG(STRM_API_74_112_2_17_23_42_56_2634,"#STRM STRM_Init invalid input!")
TRACE_MSG(STRM_API_80_112_2_17_23_42_56_2635,"#STRM STRM_Init STRMHSM_CreatHsm fail!")
TRACE_MSG(STRM_API_87_112_2_17_23_42_56_2636,"#STRM STRM_Init sig_ptr alloc fail!")
TRACE_MSG(STRM_API_94_112_2_17_23_42_56_2637,"#STRM STRM_Init STRMHSM ThreadId is invalid!")
TRACE_MSG(STRM_API_130_112_2_17_23_42_56_2638,"#STRM STRM_Play strm_handle=0x%x,palyer_handle=0x%x!")
TRACE_MSG(STRM_API_134_112_2_17_23_42_56_2639,"#STRM STRM_Play invalid input!")
TRACE_MSG(STRM_API_140_112_2_17_23_42_56_2640,"#STRM STRM_Play creat signal fail!")
TRACE_MSG(STRM_API_159_112_2_17_23_42_56_2641,"#STRM STRM_Pause strm_handle=0x%x!")
TRACE_MSG(STRM_API_163_112_2_17_23_42_56_2642,"#STRM STRM_Pause invalid input!")
TRACE_MSG(STRM_API_170_112_2_17_23_42_56_2643,"#STRM STRM_Pause creat signal fail!")
TRACE_MSG(STRM_API_190_112_2_17_23_42_56_2644,"#STRM STRM_Seek strm_handle=0x%x!")
TRACE_MSG(STRM_API_194_112_2_17_23_42_56_2645,"#STRM STRM_Seek invalid input!")
TRACE_MSG(STRM_API_200_112_2_17_23_42_56_2646,"#STRM STRM_Resume creat signal fail!")
TRACE_MSG(STRM_API_220_112_2_17_23_42_56_2647,"#STRM STRM_Resume strm_handle=0x%x!")
TRACE_MSG(STRM_API_224_112_2_17_23_42_56_2648,"#STRM STRM_Resume invalid input!")
TRACE_MSG(STRM_API_230_112_2_17_23_42_56_2649,"#STRM STRM_Resume creat signal fail!")
TRACE_MSG(STRM_API_250_112_2_17_23_42_56_2650,"#STRM STRM_Stop strm_handle=0x%x!")
TRACE_MSG(STRM_API_254_112_2_17_23_42_56_2651,"#STRM STRM_Stop invalid input!")
TRACE_MSG(STRM_API_261_112_2_17_23_42_56_2652,"#STRM STRM_Stop creat signal fail!")
TRACE_MSG(STRM_API_280_112_2_17_23_42_56_2653,"#STRM STRM_Close strm_handle=0x%x!")
TRACE_MSG(STRM_API_284_112_2_17_23_42_56_2654,"#STRM STRM_Close invalid input!")
TRACE_MSG(STRM_API_291_112_2_17_23_42_56_2655,"#STRM STRM_Close creat signal fail!")
TRACE_MSG(STRM_API_312_112_2_17_23_42_57_2656,"#STRM SendAuthRsp strm_handle=0x%x!")
TRACE_MSG(STRM_API_316_112_2_17_23_42_57_2657,"#STRM SendAuthRsp invalid input!")
TRACE_MSG(STRM_API_321_112_2_17_23_42_57_2658,"#STRM SendAuthRsp username or psw cannot be NULL!")
TRACE_MSG(STRM_API_334_112_2_17_23_42_57_2659,"#STRM SendAuthRsp serialize err!, msg_id:%x")
TRACE_MSG(STRM_API_350_112_2_17_23_42_57_2660,"#STRM STRM_DescribeAuthRsp strm_handle=0x%x!")
TRACE_MSG(STRM_API_363_112_2_17_23_42_57_2661,"#STRM STRM_SetupAuthRsp strm_handle=0x%x!")
TRACE_MSG(STRM_API_375_112_2_17_23_42_57_2662,"#STRM STRM_PlayAuthRsp strm_handle=0x%x!")
TRACE_MSG(STRM_API_393_112_2_17_23_42_57_2663,"#STRM STRM_SendInitCnf strm_handle=0x%x!")
TRACE_MSG(STRM_API_396_112_2_17_23_42_57_2664,"#STRM STRM_SendInitCnf invalid input!")
TRACE_MSG(STRM_API_407_112_2_17_23_42_57_2665,"#STRM STRM_SendInitCnf serialize FAIL!")
TRACE_MSG(STRM_API_427_112_2_17_23_42_57_2666,"#STRM STRM_SendAuthInd strm_handle=0x%x!")
TRACE_MSG(STRM_API_431_112_2_17_23_42_57_2667,"#STRM STRM_SendAuthInd invalid input!")
TRACE_MSG(STRM_API_437_112_2_17_23_42_57_2668,"#STRM STRM_SendAuthInd creat signal FAIL!")
TRACE_MSG(STRM_API_455_112_2_17_23_42_57_2669,"#STRM STRM_SendPlayCnf strm_handle=0x%x!")
TRACE_MSG(STRM_API_459_112_2_17_23_42_57_2670,"#STRM STRM_SendPlayCnf invalid input!")
TRACE_MSG(STRM_API_466_112_2_17_23_42_57_2671,"#STRM STRM_SendPlayCnf creat signal FAIL!")
TRACE_MSG(STRM_API_484_112_2_17_23_42_57_2672,"#STRM STRM_SendStopCnf strm_handle=0x%x dest_id=0x%x!")
TRACE_MSG(STRM_API_488_112_2_17_23_42_57_2673,"#STRM STRM_SendStopCnf invalid input!")
TRACE_MSG(STRM_API_495_112_2_17_23_42_57_2674,"#STRM STRM_SendStopCnf creat signal FAIL!")
TRACE_MSG(STRM_API_514_112_2_17_23_42_57_2675,"#STRM STRM_SendBufferingInd strm_handle=0x%x!")
TRACE_MSG(STRM_API_518_112_2_17_23_42_57_2676,"#STRM STRM_SendBufferingInd invalid input!")
TRACE_MSG(STRM_API_524_112_2_17_23_42_57_2677,"#STRM STRM_SendBufferingInd creat signal FAIL!")
TRACE_MSG(STRM_API_543_112_2_17_23_42_57_2678,"#STRM STRM_SendGoodbyeInd strm_handle=0x%x!")
TRACE_MSG(STRM_API_547_112_2_17_23_42_57_2679,"#STRM STRM_SendGoodbyeInd invalid input!")
TRACE_MSG(STRM_API_553_112_2_17_23_42_57_2680,"#STRM STRM_SendGoodbyeInd creat signal FAIL!")
TRACE_MSG(STRM_API_572_112_2_17_23_42_57_2681,"#STRM STRM_SendBufferEndInd strm_handle=0x%x!")
TRACE_MSG(STRM_API_576_112_2_17_23_42_57_2682,"#STRM STRM_SendBufferEndInd invalid input!")
TRACE_MSG(STRM_API_582_112_2_17_23_42_57_2683,"#STRM STRM_SendBufferEndInd creat signal FAIL!")
TRACE_MSG(STRM_API_603_112_2_17_23_42_57_2684,"#STRM STRM_BufProInd strm_handle=0x%x!")
TRACE_MSG(STRM_API_607_112_2_17_23_42_57_2685,"#STRM STRM_BufProInd invalid input!")
TRACE_MSG(STRM_API_613_112_2_17_23_42_57_2686,"#STRM STRM_BufProInd creat signal FAIL!")
TRACE_MSG(STRM_API_634_112_2_17_23_42_57_2687,"#STRM STRM_SendDataTimeoutInd strm_handle=0x%x!")
TRACE_MSG(STRM_API_638_112_2_17_23_42_57_2688,"#STRM STRM_SendDataTimeoutInd invalid input!")
TRACE_MSG(STRM_API_644_112_2_17_23_42_57_2689,"#STRM STRM_SendDataTimeoutInd creat signal FAIL!")
TRACE_MSG(STRM_API_663_112_2_17_23_42_57_2690,"#STRM STRM_SendPauseCnf strm_handle=0x%x!")
TRACE_MSG(STRM_API_667_112_2_17_23_42_57_2691,"#STRM STRM_SendPauseCnf invalid input!")
TRACE_MSG(STRM_API_673_112_2_17_23_42_57_2692,"#STRM STRM_SendPauseCnf creat signal FAIL!")
TRACE_MSG(STRM_API_692_112_2_17_23_42_57_2693,"#STRM STRM_SendResumeCnf strm_handle=0x%x!")
TRACE_MSG(STRM_API_696_112_2_17_23_42_57_2694,"#STRM STRM_SendResumeCnf invalid input!")
TRACE_MSG(STRM_API_702_112_2_17_23_42_58_2695,"#STRM STRM_SendResumeCnf creat signal FAIL!")
TRACE_MSG(STRM_API_721_112_2_17_23_42_58_2696,"#STRM STRM_SendSeekCnf strm_handle=0x%x!")
TRACE_MSG(STRM_API_725_112_2_17_23_42_58_2697,"#STRM STRM_SendSeekCnf invalid input!")
TRACE_MSG(STRM_API_731_112_2_17_23_42_58_2698,"#STRM STRM_SendSeekCnf creat signal FAIL!")
TRACE_MSG(STRM_API_750_112_2_17_23_42_58_2699,"#STRM STRM_SendCloseCnf strm_handle=0x%x!")
TRACE_MSG(STRM_API_754_112_2_17_23_42_58_2700,"#STRM STRM_SendCloseCnf invalid input!")
TRACE_MSG(STRM_API_760_112_2_17_23_42_58_2701,"#STRM STRM_SendCloseCnf creat signal FAIL!")
TRACE_MSG(STRM_API_781_112_2_17_23_42_58_2702,"#STRM STRM_NotifyAppErr strm_handle=0x%x!")
TRACE_MSG(STRM_API_785_112_2_17_23_42_58_2703,"#STRM STRM_NotifyAppErr invalid input!")
TRACE_MSG(STRM_API_791_112_2_17_23_42_58_2704,"#STRM STRM_NotifyAppErr STRM_INIT_CNF")
TRACE_MSG(STRM_API_795_112_2_17_23_42_58_2705,"#STRM STRM_NotifyAppErr STRM_INIT_CNF sig_ptr CREAT fail")
TRACE_MSG(STRM_API_803_112_2_17_23_42_58_2706,"#STRM STRM_NotifyAppErr STRM_PLAY_CNF")
TRACE_MSG(STRM_API_807_112_2_17_23_42_58_2707,"#STRM STRM_NotifyAppErr STRM_PLAY_CNF sig_ptr CREAT fail")
TRACE_MSG(STRM_API_813_112_2_17_23_42_58_2708,"#STRM STRM_NotifyAppErr STRM_STOP_CNF")
TRACE_MSG(STRM_API_817_112_2_17_23_42_58_2709,"#STRM STRM_NotifyAppErr STRM_STOP_CNF sig_ptr CREAT fail")
TRACE_MSG(STRM_API_823_112_2_17_23_42_58_2710,"#STRM STRM_NotifyAppErr STRM_PAUSE_CNF")
TRACE_MSG(STRM_API_827_112_2_17_23_42_58_2711,"#STRM STRM_NotifyAppErr STRM_PAUSE_CNF sig_ptr CREAT fail")
TRACE_MSG(STRM_API_833_112_2_17_23_42_58_2712,"#STRM STRM_NotifyAppErr STRM_SEEK_CNF")
TRACE_MSG(STRM_API_837_112_2_17_23_42_58_2713,"#STRM STRM_NotifyAppErr STRM_SEEK_CNF sig_ptr CREAT fail")
TRACE_MSG(STRM_API_843_112_2_17_23_42_58_2714,"#STRM STRM_NotifyAppErr STRM_RESUME_CNF")
TRACE_MSG(STRM_API_847_112_2_17_23_42_58_2715,"#STRM STRM_NotifyAppErr STRM_RESUME_CNF sig_ptr CREAT fail")
TRACE_MSG(STRM_API_853_112_2_17_23_42_58_2716,"#STRM STRM_NotifyAppErr STRM_CLOSE_CNF")
TRACE_MSG(STRM_API_857_112_2_17_23_42_58_2717,"#STRM STRM_NotifyAppErr STRM_SEEK_CNF sig_ptr CREAT fail")
TRACE_MSG(STRM_BUFFER_282_112_2_17_23_42_59_2718,"#STRM STRMBUF_Creat, input invalid!")
TRACE_MSG(STRM_BUFFER_291_112_2_17_23_42_59_2719,"#STRM STRMBUF_Creat, creat mutex fail!")
TRACE_MSG(STRM_BUFFER_298_112_2_17_23_42_59_2720,"#STRM STRMBUF_Creat, StrmMem_CreatMemHandle fail!")
TRACE_MSG(STRM_BUFFER_306_112_2_17_23_42_59_2721,"#STRM STRMBUF_Creat, creat audio buffer fail!")
TRACE_MSG(STRM_BUFFER_312_112_2_17_23_42_59_2722,"#STRM STRMBUF_Creat, creat video buffer fail!")
TRACE_MSG(STRM_BUFFER_373_112_2_17_23_42_59_2723,"#STRM STRMBUF_ClearBufData!")
TRACE_MSG(STRM_BUFFER_376_112_2_17_23_42_59_2724,"#STRM STRMBUF_ClearBufData invalid input!")
TRACE_MSG(STRM_BUFFER_450_112_2_17_23_42_59_2725,"#STRM STRMBUF_GetNsnNun input invalid!")
TRACE_MSG(STRM_BUFFER_568_112_2_17_23_42_59_2726,"#STRM STRMBUF_ModifiyBufSize input invalid!")
TRACE_MSG(STRM_BUFFER_598_112_2_17_23_42_59_2727,"#STRM STRMBUF_InsertPacket input invalid!")
TRACE_MSG(STRM_BUFFER_607_112_2_17_23_42_59_2728,"#STRM STRMBUF_InsertPacket, buffer FULL, packet ignored")
TRACE_MSG(STRM_BUFFER_641_112_2_17_23_42_59_2729,"#STRM STRMBUF_InsertPacket, unsupported encode type")
TRACE_MSG(STRM_BUFFER_648_112_2_17_23_42_59_2730,"#STRM STRMBUF_InsertPacket, buffer FULL after inserted")
TRACE_MSG(STRM_BUFFER_673_112_2_17_23_42_59_2731,"#STRM InsertAmr input invalid!")
TRACE_MSG(STRM_BUFFER_682_112_2_17_23_42_59_2732,"#STRM InsertAmr, creat list fail!")
TRACE_MSG(STRM_BUFFER_691_112_2_17_23_42_59_2733,"#STRM InsertAmr buffer is empty")
TRACE_MSG(STRM_BUFFER_697_112_2_17_23_42_59_2734,"#STRM InsertAmr, ignore the out-of-time packet1!")
TRACE_MSG(STRM_BUFFER_705_112_2_17_23_42_59_2735,"#STRM InsertAmr, insert node fail1!")
TRACE_MSG(STRM_BUFFER_712_112_2_17_23_42_59_2736,"#STRM InsertAmr, alloc payload memory fail1!")
TRACE_MSG(STRM_BUFFER_724_112_2_17_23_42_59_2737,"#STRM InsertAmr, the first packet inserted, initial timestamp:0x%x")
TRACE_MSG(STRM_BUFFER_736_112_2_17_23_42_59_2738,"#STRM InsertAmr, ignore the out-of-time packet2!")
TRACE_MSG(STRM_BUFFER_749_112_2_17_23_42_59_2739,"#STRM InsertAmr, invalid node!")
TRACE_MSG(STRM_BUFFER_771_112_2_17_23_43_0_2740,"#STRM InsertAmr, insert node fail2!")
TRACE_MSG(STRM_BUFFER_778_112_2_17_23_43_0_2741,"#STRM InsertAmr alloc payload memory fail2!")
TRACE_MSG(STRM_BUFFER_787_112_2_17_23_43_0_2742,"#STRM InsertAmr, update initial timestamp:0x%x")
TRACE_MSG(STRM_BUFFER_860_112_2_17_23_43_0_2743,"#STRM InsertPacketAccTimestampSeq, input invalidl!")
TRACE_MSG(STRM_BUFFER_871_112_2_17_23_43_0_2744,"#STRM InsertPacketAccTimestampSeq, creat list fail!")
TRACE_MSG(STRM_BUFFER_880_112_2_17_23_43_0_2745,"#STRM InsertPacketAccTimestampSeq buffer is empty")
TRACE_MSG(STRM_BUFFER_886_112_2_17_23_43_0_2746,"#STRM InsertPacketAccTimestampSeq, ignore the out-of-time packet1!")
TRACE_MSG(STRM_BUFFER_894_112_2_17_23_43_0_2747,"#STRM InsertPacketAccTimestampSeq, creat sub_list fail1!")
TRACE_MSG(STRM_BUFFER_902_112_2_17_23_43_0_2748,"#STRM InsertPacketAccTimestampSeq, insert sub_node fail1!")
TRACE_MSG(STRM_BUFFER_910_112_2_17_23_43_0_2749,"#STRM InsertPacketAccTimestampSeq, alloc sub_node payload mem fail1!")
TRACE_MSG(STRM_BUFFER_924_112_2_17_23_43_0_2750,"#STRM InsertPacketAccTimestampSeq, insert sub_list fail1!")
TRACE_MSG(STRM_BUFFER_936_112_2_17_23_43_0_2751,"#STRM InsertPacketAccTimestampSeq, the first packet inserted, initial timestamp :0x%x")
TRACE_MSG(STRM_BUFFER_948_112_2_17_23_43_0_2752,"#STRM InsertPacketAccTimestampSeq, ignore the out-of-time packet2!")
TRACE_MSG(STRM_BUFFER_959_112_2_17_23_43_0_2753,"#STRM InsertPacketAccTimestampSeq, invalid node!")
TRACE_MSG(STRM_BUFFER_982_112_2_17_23_43_0_2754,"#STRM InsertPacketAccTimestampSeq, invalid node2!")
TRACE_MSG(STRM_BUFFER_988_112_2_17_23_43_0_2755,"#STRM InsertPacketAccTimestampSeq, invalid sub_list!")
TRACE_MSG(STRM_BUFFER_998_112_2_17_23_43_0_2756,"#STRM InsertPacketAccTimestampSeq, invalid sub_node_data_ptr!")
TRACE_MSG(STRM_BUFFER_1019_112_2_17_23_43_0_2757,"#STRM InsertPacketAccTimestampSeq, insert sub_node fail2!")
TRACE_MSG(STRM_BUFFER_1025_112_2_17_23_43_0_2758,"#STRM InsertPacketAccTimestampSeq, alloc sub_node payload mem fail2!")
TRACE_MSG(STRM_BUFFER_1038_112_2_17_23_43_0_2759,"#STRM InsertPacketAccTimestampSeq, creat sub_list fail2!")
TRACE_MSG(STRM_BUFFER_1046_112_2_17_23_43_0_2760,"#STRM InsertPacketAccTimestampSeq, insert sub_node fail1!")
TRACE_MSG(STRM_BUFFER_1055_112_2_17_23_43_0_2761,"#STRM InsertPacketAccTimestampSeq, alloc sub_node payload mem fail1!")
TRACE_MSG(STRM_BUFFER_1073_112_2_17_23_43_0_2762,"#STRM InsertPacketAccTimestampSeq, invalid node2!")
TRACE_MSG(STRM_BUFFER_1097_112_2_17_23_43_0_2763,"#STRM InsertPacketAccTimestampSeq, insert sub_list fail1!")
TRACE_MSG(STRM_BUFFER_1109_112_2_17_23_43_0_2764,"#STRM InsertPacketAccTimestampSeq, update initial timestamp :0x%x")
TRACE_MSG(STRM_BUFFER_1146_112_2_17_23_43_0_2765,"#STRM InsertPacketAccTimestampSeqIndex, input invalidl!")
TRACE_MSG(STRM_BUFFER_1156_112_2_17_23_43_0_2766,"#STRM InsertPacketAccTimestampSeqIndex, creat list fail!")
TRACE_MSG(STRM_BUFFER_1167_112_2_17_23_43_0_2767,"#STRM InsertPacketAccTimestampSeqIndex, list is empty")
TRACE_MSG(STRM_BUFFER_1173_112_2_17_23_43_0_2768,"#STRM InsertPacketAccTimestampSeqIndex, ignore the out-of-time packet!")
TRACE_MSG(STRM_BUFFER_1180_112_2_17_23_43_0_2769,"#STRM InsertPacketAccTimestampSeqIndex, AddTimestampNodeToListByPos fail1!")
TRACE_MSG(STRM_BUFFER_1189_112_2_17_23_43_0_2770,"#STRM InsertPacketAccTimestampSeqIndex, insert sub_list fail1!")
TRACE_MSG(STRM_BUFFER_1198_112_2_17_23_43_0_2771,"#STRM InsertPacketAccTimestampSeqIndex, AddPacketNodeToListByPos fail!")
TRACE_MSG(STRM_BUFFER_1207_112_2_17_23_43_0_2772,"#STRM InsertPacketAccTimestampSeqIndex, the first packet inserted, initial timestamp :0x%x")
TRACE_MSG(STRM_BUFFER_1218_112_2_17_23_43_0_2773,"#STRM InsertPacketAccTimestampSeqIndex, ignore the out-of-time packet2!")
TRACE_MSG(STRM_BUFFER_1230_112_2_17_23_43_0_2774,"#STRM InsertPacketAccTimestampSeqIndex, invalid cts_node!")
TRACE_MSG(STRM_BUFFER_1237_112_2_17_23_43_0_2775,"#STRM InsertPacketAccTimestampSeqIndex, invalid sequence_list!")
TRACE_MSG(STRM_BUFFER_1252_112_2_17_23_43_1_2776,"#STRM InsertPacketAccTimestampSeqIndex, AddPacketNodeToListByPos fail!")
TRACE_MSG(STRM_BUFFER_1262_112_2_17_23_43_1_2777,"#STRM InsertPacketAccTimestampSeqIndex, insert sub_list fail1!")
TRACE_MSG(STRM_BUFFER_1272_112_2_17_23_43_1_2778,"#STRM InsertPacketAccTimestampSeqIndex, AddPacketNodeToListByPos fail!")
TRACE_MSG(STRM_BUFFER_1285_112_2_17_23_43_1_2779,"#STRM InsertPacketAccTimestampSeqIndex, AddTimestampNodeToListByPos fail1!")
TRACE_MSG(STRM_BUFFER_1295_112_2_17_23_43_1_2780,"#STRM InsertPacketAccTimestampSeqIndex, insert sub_list fail1!")
TRACE_MSG(STRM_BUFFER_1305_112_2_17_23_43_1_2781,"#STRM InsertPacketAccTimestampSeqIndex, AddPacketNodeToListByPos fail!")
TRACE_MSG(STRM_BUFFER_1313_112_2_17_23_43_1_2782,"#STRM InsertPacketAccTimestampSeqIndex, update initial timestamp :0x%x")
TRACE_MSG(STRM_BUFFER_1340_112_2_17_23_43_1_2783,"#STRM InsertH264 input invalid!")
TRACE_MSG(STRM_BUFFER_1349_112_2_17_23_43_1_2784,"#STRM InsertH264, creat list fail!")
TRACE_MSG(STRM_BUFFER_1362_112_2_17_23_43_1_2785,"#STRM InsertH264, ignore the out-of-time packet1!")
TRACE_MSG(STRM_BUFFER_1371_112_2_17_23_43_1_2786,"#STRM InsertH264, insert node fail1!")
TRACE_MSG(STRM_BUFFER_1378_112_2_17_23_43_1_2787,"#STRM InsertH264, alloc payload memory fail1!")
TRACE_MSG(STRM_BUFFER_1391_112_2_17_23_43_1_2788,"#STRM InsertH264, the first packet inserted, initial_timestamp:0x%x")
TRACE_MSG(STRM_BUFFER_1402_112_2_17_23_43_1_2789,"#STRM InsertH264, ignore the out-of-time packet2!")
TRACE_MSG(STRM_BUFFER_1412_112_2_17_23_43_1_2790,"#STRM InsertH264, invalid node!")
TRACE_MSG(STRM_BUFFER_1462_112_2_17_23_43_1_2791,"#STRM InsertH264, insert fail2!")
TRACE_MSG(STRM_BUFFER_1469_112_2_17_23_43_1_2792,"#STRM InsertH264, alloc payload mem fail2!")
TRACE_MSG(STRM_BUFFER_1480_112_2_17_23_43_1_2793,"#STRM InsertH264, update initial_timestamp:%d")
TRACE_MSG(STRM_BUFFER_1505_112_2_17_23_43_1_2794,"#STRM InsertMPEG4!")
TRACE_MSG(STRM_BUFFER_1524_112_2_17_23_43_1_2795,"#STRM CheckIfSendPacket input invalid:first:%d, last:%d")
TRACE_MSG(STRM_BUFFER_1535_112_2_17_23_43_1_2796,"#STRM CheckIfSendPacket :first:%d, last:%d, start:%d, end:%d")
TRACE_MSG(STRM_BUFFER_1557_112_2_17_23_43_1_2797,"#STRM STRMBUF_GetPacket, input invalid!")
TRACE_MSG(STRM_BUFFER_1598_112_2_17_23_43_1_2798,"#STRM STRMBUF_GetPacket, unsupported encode type = %d!")
TRACE_MSG(STRM_BUFFER_1615_112_2_17_23_43_1_2799,"#STRM STRMBUF_FreePacket input invalid!")
TRACE_MSG(STRM_BUFFER_1638_112_2_17_23_43_1_2800,"#STRM GetAmrPacket input invalid!")
TRACE_MSG(STRM_BUFFER_1644_112_2_17_23_43_1_2801,"#STRM GetAmrPacket, list null")
TRACE_MSG(STRM_BUFFER_1652_112_2_17_23_43_1_2802,"#STRM GetAmrPacket, empty list")
TRACE_MSG(STRM_BUFFER_1671_112_2_17_23_43_1_2803,"#STRM GetAmrPacket, not enough buffer data to send")
TRACE_MSG(STRM_BUFFER_1848_112_2_17_23_43_2_2804,"#STRM GetPacketAccTimestampSeq, input invalid")
TRACE_MSG(STRM_BUFFER_1854_112_2_17_23_43_2_2805,"#STRM GetPacketAccTimestampSeq, list null")
TRACE_MSG(STRM_BUFFER_1860_112_2_17_23_43_2_2806,"#STRM GetPacketAccTimestampSeq, STRM_BUF_GET_RESULT_EMPTY!")
TRACE_MSG(STRM_BUFFER_1877_112_2_17_23_43_2_2807,"#STRM GetPacketAccTimestampSeq, not enough buffer data to send")
TRACE_MSG(STRM_BUFFER_1885_112_2_17_23_43_2_2808,"#STRM GetPacketAccTimestampSeq, not enough buffer data to send 2")
TRACE_MSG(STRM_BUFFER_1895_112_2_17_23_43_2_2809,"#STRM GetPacketAccTimestampSeq, empty list")
TRACE_MSG(STRM_BUFFER_1911_112_2_17_23_43_2_2810,"#STRM GetPacketAccTimestampSeq, sub_node_data_ptr is NULL or last")
TRACE_MSG(STRM_BUFFER_1948_112_2_17_23_43_2_2811,"#STRM GetPacketAccTimestampSeq, alloc payload mem fail or is_list_bad=%d!")
TRACE_MSG(STRM_BUFFER_2008_112_2_17_23_43_2_2812,"#STRM GetPacketAccTimestampSeq, STRM_BUF_GET_RESULT_FAIL!")
TRACE_MSG(STRM_BUFFER_2048_112_2_17_23_43_2_2813,"#STRM GetPacketAccTimestampSeqIndex, input invalid")
TRACE_MSG(STRM_BUFFER_2055_112_2_17_23_43_2_2814,"#STRM GetPacketAccTimestampSeqIndex, list null")
TRACE_MSG(STRM_BUFFER_2062_112_2_17_23_43_2_2815,"#STRM GetPacketAccTimestampSeqIndex, STRM_BUF_GET_RESULT_EMPTY!")
TRACE_MSG(STRM_BUFFER_2078_112_2_17_23_43_2_2816,"#STRM GetPacketAccTimestampSeqIndex, not enough buffer data to send")
TRACE_MSG(STRM_BUFFER_2087_112_2_17_23_43_2_2817,"#STRM GetPacketAccTimestampSeqIndex, empty list")
TRACE_MSG(STRM_BUFFER_2090_112_2_17_23_43_2_2818,"#STRM GetPacketAccTimestampSeqIndex, timestamp:0x%x")
TRACE_MSG(STRM_BUFFER_2122_112_2_17_23_43_2_2819,"#STRM GetPacketAccTimestampSeqIndex fail:alloc data_ptr!")
TRACE_MSG(STRM_BUFFER_2178_112_2_17_23_43_2_2820,"#STRM GetPacketAccTimestampSeqIndex fail:data_len > au_size!")
TRACE_MSG(STRM_BUFFER_2188_112_2_17_23_43_2_2821,"#STRM GetPacketAccTimestampSeqIndex fail:data_len > au_size!")
TRACE_MSG(STRM_BUFFER_2198_112_2_17_23_43_2_2822,"#STRM GetPacketAccTimestampSeqIndex fail:packet number != 1!")
TRACE_MSG(STRM_BUFFER_2265_112_2_17_23_43_3_2823,"#STRM GetPacketAccTimestampSeqIndex fail:data_ptr invalid!")
TRACE_MSG(STRM_BUFFER_2272_112_2_17_23_43_3_2824,"#STRM GetPacketAccTimestampSeqIndex fail:timestamp node doesn't have sequence node!")
TRACE_MSG(STRM_BUFFER_2278_112_2_17_23_43_3_2825,"#STRM GetPacketAccTimestampSeqIndex, pos:0x%x")
TRACE_MSG(STRM_BUFFER_2281_112_2_17_23_43_3_2826,"#STRM GetPacketAccTimestampSeqIndex, STRM_BUF_GET_RESULT_FAIL!")
TRACE_MSG(STRM_BUFFER_2290_112_2_17_23_43_3_2827,"#STRM GetPacketAccTimestampSeqIndex fail: data_len!=au_size!")
TRACE_MSG(STRM_BUFFER_2317_112_2_17_23_43_3_2828,"#STRM GetH264Packet input invalid!")
TRACE_MSG(STRM_BUFFER_2323_112_2_17_23_43_3_2829,"#STRM GetH264Packet, list null")
TRACE_MSG(STRM_BUFFER_2330_112_2_17_23_43_3_2830,"#STRM GetH264Packet, empty list")
TRACE_MSG(STRM_BUFFER_2337_112_2_17_23_43_3_2831,"#STRM GetH264Packet, invalid list node!")
TRACE_MSG(STRM_BUFFER_2343_112_2_17_23_43_3_2832,"#STRM GetH264Packet, invalid list node!")
TRACE_MSG(STRM_BUFFER_2354_112_2_17_23_43_3_2833,"#STRM GetH264Packet, not enough buffer data!")
TRACE_MSG(STRM_BUFFER_2365_112_2_17_23_43_3_2834,"#STRM GetH264Packet, invalid list node!")
TRACE_MSG(STRM_BUFFER_2417_112_2_17_23_43_3_2835,"#STRM GetH264Packet, invalid temp_node_data_ptr!")
TRACE_MSG(STRM_BUFFER_2453_112_2_17_23_43_3_2836,"#STRM GetH264Packet, PackH264Fragments fail!")
TRACE_MSG(STRM_BUFFER_2483_112_2_17_23_43_3_2837,"#STRM GetH264Packet, DeleteH264NodeBetween fail!")
TRACE_MSG(STRM_BUFFER_2496_112_2_17_23_43_3_2838,"#STRM GetH264Packet packet is empty STRM_BUF_GET_RESULT_FAIL!")
TRACE_MSG(STRM_BUFFER_2523_112_2_17_23_43_3_2839,"#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr is null!")
TRACE_MSG(STRM_BUFFER_2534_112_2_17_23_43_3_2840,"#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr2 is null!")
TRACE_MSG(STRM_BUFFER_2539_112_2_17_23_43_3_2841,"#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr->seq_num!=seq!")
TRACE_MSG(STRM_BUFFER_2553_112_2_17_23_43_3_2842,"#STRM CheckIfBetween2NodesSeqNumContinuous pack_ptr3 is null!")
TRACE_MSG(STRM_BUFFER_2558_112_2_17_23_43_3_2843,"#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr->seq_num!=seq!2")
TRACE_MSG(STRM_BUFFER_2568_112_2_17_23_43_3_2844,"#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr is null!")
TRACE_MSG(STRM_BUFFER_2579_112_2_17_23_43_3_2845,"#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr2 is null!")
TRACE_MSG(STRM_BUFFER_2584_112_2_17_23_43_3_2846,"#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr->seq_num!=seq!")
TRACE_MSG(STRM_BUFFER_2598_112_2_17_23_43_3_2847,"#STRM CheckIfBetween2NodesSeqNumContinuous pack_ptr3 is null!")
TRACE_MSG(STRM_BUFFER_2603_112_2_17_23_43_3_2848,"#STRM CheckIfBetween2NodesSeqNumContinuous return false:pack_ptr->seq_num!=seq!2")
TRACE_MSG(STRM_BUFFER_2634_112_2_17_23_43_3_2849,"#STRM PackH264Fragments input invalid!")
TRACE_MSG(STRM_BUFFER_2686_112_2_17_23_43_3_2850,"#STRM PackH264Fragments memcpy fatal error!")
TRACE_MSG(STRM_BUFFER_2712_112_2_17_23_43_4_2851,"#STRM DeleteH264NodeBetween input invalid!")
TRACE_MSG(STRM_BUFFER_2722_112_2_17_23_43_4_2852,"#STRM DeleteH264NodeBetween, invalid node!")
TRACE_MSG(STRM_BUFFER_2753_112_2_17_23_43_4_2853,"#STRM FindNodeInListByTimestamp, invalid input!")
TRACE_MSG(STRM_BUFFER_2764_112_2_17_23_43_4_2854,"#STRM FindNodeInListByTimestamp, invalid node!")
TRACE_MSG(STRM_BUFFER_2800_112_2_17_23_43_4_2855,"#STRM FindNodeInListBySequence, invalid input!")
TRACE_MSG(STRM_BUFFER_2811_112_2_17_23_43_4_2856,"#STRM FindNodeInListBySequence, invalid node!")
TRACE_MSG(STRM_BUFFER_2848_112_2_17_23_43_4_2857,"#STRM FindPosToInsertNodeByMpeg4Index, invalid input!")
TRACE_MSG(STRM_BUFFER_2858_112_2_17_23_43_4_2858,"#STRM FindPosToInsertNodeByMpeg4Index, invalid packet_node_data_ptr!")
TRACE_MSG(STRM_BUFFER_2891_112_2_17_23_43_4_2859,"#STRM FindPosToInsertNodeByTimestamp, invalid input!")
TRACE_MSG(STRM_BUFFER_2901_112_2_17_23_43_4_2860,"#STRM FindPosToInsertNodeByTimestamp, invalid node2!")
TRACE_MSG(STRM_BUFFER_2934_112_2_17_23_43_4_2861,"#STRM FindPosToInsertNodeBySequence, invalid input!")
TRACE_MSG(STRM_BUFFER_2944_112_2_17_23_43_4_2862,"#STRM FindPosToInsertNodeBySequence, invalid node2!")
TRACE_MSG(STRM_BUFFER_2976_112_2_17_23_43_4_2863,"#STRM AddPacketNodeToListByPos, invalid input!")
TRACE_MSG(STRM_BUFFER_2988_112_2_17_23_43_4_2864,"#STRM AddPacketNodeToListByPos, insert packet_node_data_ptr fail!")
TRACE_MSG(STRM_BUFFER_2996_112_2_17_23_43_4_2865,"#STRM AddPacketNodeToListByPos, alloc sub_node payload mem fail2!")
TRACE_MSG(STRM_BUFFER_3017_112_2_17_23_43_4_2866,"#STRM AddTimestampNodeToListByPos, invalid input!")
TRACE_MSG(STRM_BUFFER_3025_112_2_17_23_43_4_2867,"#STRM AddTimestampNodeToListByPos, creat sequence_list fail1!")
TRACE_MSG(STRM_BUFFER_3034_112_2_17_23_43_4_2868,"#STRM AddTimestampNodeToListByPos, insert sub_list fail1!")
TRACE_MSG(STRM_BUFFER_3056_112_2_17_23_43_4_2869,"#STRM AddSequenceNodeToListByPos, invalid input!")
TRACE_MSG(STRM_BUFFER_3064_112_2_17_23_43_4_2870,"#STRM AddSequenceNodeToListByPos, creat sequence_list fail1!")
TRACE_MSG(STRM_BUFFER_3070_112_2_17_23_43_4_2871,"#STRM AddSequenceNodeToListByPos, insert sub_list fail1!")
TRACE_MSG(STRM_BUFFER_3093_112_2_17_23_43_4_2872,"#STRM DestroyPacketList invalid input!")
TRACE_MSG(STRM_BUFFER_3132_112_2_17_23_43_4_2873,"STRM DestroySequenceNode invalid input!")
TRACE_MSG(STRM_BUFFER_3160_112_2_17_23_43_4_2874,"#STRM DestroySeqenceList invalid input!")
TRACE_MSG(STRM_BUFFER_3188_112_2_17_23_43_5_2875,"#STRM DestroyTimestampNode invalid input!")
TRACE_MSG(STRM_BUFFER_3215_112_2_17_23_43_5_2876,"#STRM DestroyTimestampList invalid input!")
TRACE_MSG(STRM_BUFFER_3249_112_2_17_23_43_5_2877,"#STRM DestroyBufListFor2Levels invalid input!")
TRACE_MSG(STRM_BUFFER_3286_112_2_17_23_43_5_2878,"#STRM StrmMem_CreatMemHandle!")
TRACE_MSG(STRM_BUFFER_3305_112_2_17_23_43_5_2879,"#STRM StrmMem_CreatMemHandle:Mem Handle has been initiated!")
TRACE_MSG(STRM_BUFFER_3326_112_2_17_23_43_5_2880,"#STRM StrmMem_CreatMemHandle:Mem Handle has been initiated!")
TRACE_MSG(STRM_BUFFER_3333_112_2_17_23_43_5_2881,"#STRM StrmMem_CreatMemHandle fail!")
TRACE_MSG(STRM_BUFFER_3346_112_2_17_23_43_5_2882,"#STRM StrmMem_DeleteMemHandle!")
TRACE_MSG(STRM_BUFFER_TEST_193_112_2_17_23_43_6_2883,"#STRM buffer test, amr produce, init_timestamp = %d")
TRACE_MSG(STRM_BUFFER_TEST_460_112_2_17_23_43_6_2884,"#STRM Buffer Test, insert AMR, seq=%d, timestamp=%d, result:%d")
TRACE_MSG(STRM_BUFFER_TEST_463_112_2_17_23_43_6_2885,"#STRM Buffer Test, insert AMR, init_timestamp=%d")
TRACE_MSG(STRM_BUFFER_TEST_477_112_2_17_23_43_6_2886,"#STRM Buffer Test, insert h263, seq=%d, timestamp=%d, result:%d")
TRACE_MSG(STRM_BUFFER_TEST_480_112_2_17_23_43_6_2887,"#STRM Buffer Test, insert h263, init_timestamp=%d")
TRACE_MSG(STRM_BUFFER_TEST_494_112_2_17_23_43_6_2888,"#STRM Buffer Test, insert h264, seq=%d, string:%s, result:%d")
TRACE_MSG(STRM_BUFFER_TEST_497_112_2_17_23_43_6_2889,"#STRM Buffer Test, insert h264, init_timestamp=%d")
TRACE_MSG(STRM_BUFFER_TEST_504_112_2_17_23_43_6_2890,"#STRM Buffer Test, insert h264, seq:%d, list:%s")
TRACE_MSG(STRM_BUFFER_TEST_522_112_2_17_23_43_6_2891,"#STRM Buffer Test, insert h264, seq=%d, string:%s, result:%d")
TRACE_MSG(STRM_BUFFER_TEST_525_112_2_17_23_43_6_2892,"#STRM Buffer Test, insert h264, init_timestamp=%d")
TRACE_MSG(STRM_BUFFER_TEST_532_112_2_17_23_43_6_2893,"#STRM Buffer Test, insert h264, seq:%d, list:%s")
TRACE_MSG(STRM_BUFFER_TEST_550_112_2_17_23_43_6_2894,"#STRM Buffer Test,  Mpeg4Insert, seq=%d, timestamp=%d, result:%d")
TRACE_MSG(STRM_BUFFER_TEST_553_112_2_17_23_43_6_2895,"#STRM Buffer Test, Mpeg4Insert, init_timestamp=%d")
TRACE_MSG(STRM_BUFFER_TEST_579_112_2_17_23_43_6_2896,"#STRM Buffer Test, packed amr buf_size:%d")
TRACE_MSG(STRM_BUFFER_TEST_580_112_2_17_23_43_6_2897,"#STRM Buffer Test, packed amr string:%s")
TRACE_MSG(STRM_BUFFER_TEST_608_112_2_17_23_43_6_2898,"#STRM Buffer Test, packed h263 buf_size:%d")
TRACE_MSG(STRM_BUFFER_TEST_609_112_2_17_23_43_6_2899,"#STRM Buffer Test, packed h263 string:%s")
TRACE_MSG(STRM_BUFFER_TEST_638_112_2_17_23_43_6_2900,"#STRM Buffer Test, packed h264 buf_size:%d")
TRACE_MSG(STRM_BUFFER_TEST_639_112_2_17_23_43_6_2901,"#STRM Buffer Test, packed h264 string:%s")
TRACE_MSG(STRM_BUFFER_TEST_668_112_2_17_23_43_6_2902,"#STRM Buffer Test, packed mpeg4 buf_size:%d")
TRACE_MSG(STRM_BUFFER_TEST_669_112_2_17_23_43_6_2903,"#STRM Buffer Test, packed mpeg4 string:%s")
TRACE_MSG(STRM_HSM_354_112_2_17_23_43_7_2904,"#STRM StrmHsmConstructor, ERR input!")
TRACE_MSG(STRM_HSM_373_112_2_17_23_43_7_2905,"#STRM StrmHsmDestructor, ERR input!")
TRACE_MSG(STRM_HSM_404_112_2_17_23_43_7_2906,"#STRM StrmStreaming, ERR input!")
TRACE_MSG(STRM_HSM_470_112_2_17_23_43_7_2907,"#STRM StrmIdle, ERR input!")
TRACE_MSG(STRM_HSM_491_112_2_17_23_43_7_2908,"#STRM StrmIdle init rtsp session failed!")
TRACE_MSG(STRM_HSM_516_112_2_17_23_43_7_2909,"#STRM StrmInitRtspPending, ERR input!")
TRACE_MSG(STRM_HSM_538_112_2_17_23_43_8_2910,"#STRM StrmInitRtspPending parse SDP fail!")
TRACE_MSG(STRM_HSM_549_112_2_17_23_43_8_2911,"#STRM StrmInitRtspPending no SDP buffer while using http!")
TRACE_MSG(STRM_HSM_569_112_2_17_23_43_8_2912,"#STRM StrmInitRtspPending RTSP DESCRIBE FAILED!")
TRACE_MSG(STRM_HSM_576_112_2_17_23_43_8_2913,"#STRM StrmInitRtspPending RTSP init return FAILED!")
TRACE_MSG(STRM_HSM_599_112_2_17_23_43_8_2914,"#STRM HandleDscrbCnf, ERR input!")
TRACE_MSG(STRM_HSM_618_112_2_17_23_43_8_2915,"#STRM HandleDscrbCnf creat RTP1 FAILED!")
TRACE_MSG(STRM_HSM_630_112_2_17_23_43_8_2916,"#STRM HandleDscrbCnf creat RTP2 FAILED!")
TRACE_MSG(STRM_HSM_641_112_2_17_23_43_8_2917,"#STRM HandleDscrbCnf send init cnf FAILED!")
TRACE_MSG(STRM_HSM_648_112_2_17_23_43_8_2918,"#STRM StrmDescribePending RTSP describe return FAILED!")
TRACE_MSG(STRM_HSM_668_112_2_17_23_43_8_2919,"#STRM StrmDescribePending, ERR input!")
TRACE_MSG(STRM_HSM_700_112_2_17_23_43_8_2920,"#STRM StrmDescribePending Send auth ind fail!")
TRACE_MSG(STRM_HSM_729_112_2_17_23_43_8_2921,"#STRM StrmInitiated, ERR input!")
TRACE_MSG(STRM_HSM_761_112_2_17_23_43_8_2922,"#STRM StrmDescribeAuthing, ERR input!")
TRACE_MSG(STRM_HSM_794_112_2_17_23_43_8_2923,"#STRM HandleDscrbAuthRsp input invalid!")
TRACE_MSG(STRM_HSM_800_112_2_17_23_43_8_2924,"#STRM HandleDscrbAuthRsp err input auth info!")
TRACE_MSG(STRM_HSM_809_112_2_17_23_43_8_2925,"#STRM HandleDscrbAuthRsp send auth rsp failed!")
TRACE_MSG(STRM_HSM_829_112_2_17_23_43_8_2926,"#STRM StrmSetup, ERR input!")
TRACE_MSG(STRM_HSM_898_112_2_17_23_43_8_2927,"#STRM HandleSetupFirstCnf input invalid!")
TRACE_MSG(STRM_HSM_923_112_2_17_23_43_8_2928,"#STRM StrmSetup1 RTSP SETUP FAILED!")
TRACE_MSG(STRM_HSM_940_112_2_17_23_43_8_2929,"#STRM StrmSetup1 RTSP PLAY FAILED!")
TRACE_MSG(STRM_HSM_948_112_2_17_23_43_8_2930,"#STRM StrmSetup1 RTSP setup1 return FAILED!")
TRACE_MSG(STRM_HSM_963_112_2_17_23_43_8_2931,"#STRM HandleSetupAuthInd input invalid!")
TRACE_MSG(STRM_HSM_985_112_2_17_23_43_8_2932,"#STRM HandleSetupAuthInd Send auth ind fail!")
TRACE_MSG(STRM_HSM_1001_112_2_17_23_43_8_2933,"#STRM HandleSetupAuthInd input invalid!")
TRACE_MSG(STRM_HSM_1023_112_2_17_23_43_9_2934,"#STRM HandlePlayAuthInd Send auth ind fail!")
TRACE_MSG(STRM_HSM_1043_112_2_17_23_43_9_2935,"#STRM StrmSetup1, ERR input!")
TRACE_MSG(STRM_HSM_1099_112_2_17_23_43_9_2936,"#STRM HandleSetupAuthRsp input invalid!")
TRACE_MSG(STRM_HSM_1106_112_2_17_23_43_9_2937,"#STRM HandleSetupAuthRsp err input auth info!")
TRACE_MSG(STRM_HSM_1115_112_2_17_23_43_9_2938,"#STRM HandleSetupAuthRsp auth rsp failed!")
TRACE_MSG(STRM_HSM_1136_112_2_17_23_43_9_2939,"#STRM HandlePlayAuthRsp input invalid!")
TRACE_MSG(STRM_HSM_1143_112_2_17_23_43_9_2940,"#STRM HandlePlayAuthRsp err input auth info!")
TRACE_MSG(STRM_HSM_1152_112_2_17_23_43_9_2941,"#STRM HandlePlayAuthRsp auth rsp failed!")
TRACE_MSG(STRM_HSM_1172_112_2_17_23_43_9_2942,"#STRM StrmSetup1Auth, ERR input!")
TRACE_MSG(STRM_HSM_1205_112_2_17_23_43_9_2943,"#STRM StrmSetup1Auth, ERR input!")
TRACE_MSG(STRM_HSM_1238_112_2_17_23_43_9_2944,"#STRM HandleSetup2Cnf input invalid!")
TRACE_MSG(STRM_HSM_1255_112_2_17_23_43_9_2945,"#STRM HandleSetup2Cnf RTSP PLAY FAILED!")
TRACE_MSG(STRM_HSM_1262_112_2_17_23_43_9_2946,"#STRM HandleSetup2Cnf RTSP setup2 return FAILED!")
TRACE_MSG(STRM_HSM_1281_112_2_17_23_43_9_2947,"#STRM StrmSetup2, ERR input!")
TRACE_MSG(STRM_HSM_1325_112_2_17_23_43_9_2948,"#STRM StrmSetup2Auth, ERR input!")
TRACE_MSG(STRM_HSM_1356_112_2_17_23_43_9_2949,"#STRM HandleGoodbyeInd input invalid!")
TRACE_MSG(STRM_HSM_1372_112_2_17_23_43_9_2950,"#STRM HandleGoodbyeInd wrong rtp handle!")
TRACE_MSG(STRM_HSM_1387_112_2_17_23_43_9_2951,"#STRM HandleSsrcSetInd input invalid!")
TRACE_MSG(STRM_HSM_1418_112_2_17_23_43_9_2952,"#STRM HandleSsrcSetInd, STRM_RTP_SSRC_SET_IND, wrong rtp handle")
TRACE_MSG(STRM_HSM_1431_112_2_17_23_43_9_2953,"#STRM StrmPlay!")
TRACE_MSG(STRM_HSM_1435_112_2_17_23_43_9_2954,"#STRM StrmPlay, ERR input!")
TRACE_MSG(STRM_HSM_1441_112_2_17_23_43_9_2955,"#STRM StrmPlay, STRM_STOP_REQ!")
TRACE_MSG(STRM_HSM_1457_112_2_17_23_43_9_2956,"#STRM StrmPlay STRM_RTP_SRRR_SEND_IND err!")
TRACE_MSG(STRM_HSM_1490_112_2_17_23_43_10_2957,"#STRM StrmPlayPending, ERR input!")
TRACE_MSG(STRM_HSM_1493_112_2_17_23_43_10_2958,"#STRM StrmPlayPending is_seek:%d!")
TRACE_MSG(STRM_HSM_1537_112_2_17_23_43_10_2959,"#STRM StrmPlayPending,STRM_RTSP_PLAY_CNF is_seek play_start_time:%d!")
TRACE_MSG(STRM_HSM_1551_112_2_17_23_43_10_2960,"#STRM StrmPlayPending,STRM_RTSP_PLAY_CNF play_start_time:%d!")
TRACE_MSG(STRM_HSM_1563_112_2_17_23_43_10_2961,"#STRM StrmPlayPending resume, RTSP return failed!")
TRACE_MSG(STRM_HSM_1569_112_2_17_23_43_10_2962,"#STRM StrmPlayPending seek, RTSP return failed!")
TRACE_MSG(STRM_HSM_1577_112_2_17_23_43_10_2963,"#STRM StrmPlayPending, RTSP PLAY return failed!")
TRACE_MSG(STRM_HSM_1606_112_2_17_23_43_10_2964,"#STRM StrmPlayBuffering, ERR input!")
TRACE_MSG(STRM_HSM_1609_112_2_17_23_43_10_2965,"#STRM StrmPlayBuffering hsm_handle=0x%x,sig=0x%x!")
TRACE_MSG(STRM_HSM_1622_112_2_17_23_43_10_2966,"#STRM StrmPlayBuffering, timer handle=0x%x!")
TRACE_MSG(STRM_HSM_1628_112_2_17_23_43_10_2967,"#STRM StrmPlayBuffering, STRM_RTP_GOODBYE_IND!")
TRACE_MSG(STRM_HSM_1640_112_2_17_23_43_10_2968,"#STRM StrmPlayBuffering, STRM_BUFFER_FULL_IND!")
TRACE_MSG(STRM_HSM_1700_112_2_17_23_43_10_2969,"#STRM StrmPlayBuffering, STRM_PAUSE_REQ!")
TRACE_MSG(STRM_HSM_1710_112_2_17_23_43_10_2970,"#STRM StrmPlayBuffering, STRM_SEEK_REQ!")
TRACE_MSG(STRM_HSM_1722_112_2_17_23_43_10_2971,"#STRM StrmPlayBuffering, HSM_STATE_EXIT_SIG!")
TRACE_MSG(STRM_HSM_1764_112_2_17_23_43_10_2972,"#STRM StrmGetFrame, input error: player_handle=0x%x,playload_type=%d,frame_data_ptr=0x%x!")
TRACE_MSG(STRM_HSM_1774_112_2_17_23_43_10_2973,"#STRM StrmGetFrame couldn't get valid strm_hsm!")
TRACE_MSG(STRM_HSM_1778_112_2_17_23_43_10_2974,"#STRM StrmGetFrame, no DPLAYER_GetTime under WIN32")
TRACE_MSG(STRM_HSM_1811_112_2_17_23_43_10_2975,"#STRM StrmGetFrame,hsm=0x%x,player=0x%x,type:%d,end:%d,cur:%d,start:%d, data_pos(timestamp):%d")
TRACE_MSG(STRM_HSM_1815_112_2_17_23_43_10_2976,"#STRM StrmGetFrame, type:%d, STRMBUF_GetPacket failed!")
TRACE_MSG(STRM_HSM_1829_112_2_17_23_43_10_2977,"#STRM StrmGetFrame, the buffer nearly empty.")
TRACE_MSG(STRM_HSM_1861_112_2_17_23_43_10_2978,"#STRM StrmGetFrame, play end: cur_time=0x%x, end_time:0x%x")
TRACE_MSG(STRM_HSM_1867_112_2_17_23_43_10_2979,"#STRM StrmGetFrame, send goodbye")
TRACE_MSG(STRM_HSM_1877_112_2_17_23_43_10_2980,"#STRM StrmGetFrame, is_video_valid is %d, last_video_frame_time=0x%x, play end time is 0x%x")
TRACE_MSG(STRM_HSM_1882_112_2_17_23_43_10_2981,"#STRM StrmGetFrame, the buffer is empty or nearly empty.")
TRACE_MSG(STRM_HSM_1887_112_2_17_23_43_10_2982,"#STRM StrmGetFrame, STRM_BUF_GET_RESULT_EMPTY. last_video_frame_time is %d")
TRACE_MSG(STRM_HSM_1888_112_2_17_23_43_10_2983,"#STRM StrmGetFrame, STRM_BUF_GET_RESULT_EMPTY.last_audio_frame_time is %d")
TRACE_MSG(STRM_HSM_1919_112_2_17_23_43_10_2984,"#STRM StrmTestDecode write file error!")
TRACE_MSG(STRM_HSM_1941_112_2_17_23_43_10_2985,"#STRM StrmFreeFrame input invalid!")
TRACE_MSG(STRM_HSM_1949_112_2_17_23_43_11_2986,"#STRM StrmFreeFrame couldn't get valid strm_hsm!")
TRACE_MSG(STRM_HSM_1955_112_2_17_23_43_11_2987,"#STRM StrmFreeFrame type:%d pos:0x%x data_ptr=0x%x!")
TRACE_MSG(STRM_HSM_1986_112_2_17_23_43_11_2988,"#STRM StrmPlayPlaying, ERR input!")
TRACE_MSG(STRM_HSM_1989_112_2_17_23_43_11_2989,"#STRM StrmPlayPlaying, hsm_handle=0x%x, sig=0x%x!")
TRACE_MSG(STRM_HSM_1997_112_2_17_23_43_11_2990,"#STRM StrmPlayPlaying, DPLAYER_StreamRxStart from time:%d!")
TRACE_MSG(STRM_HSM_2000_112_2_17_23_43_11_2991,"#STRM StrmPlayPlaying, no DPLAYER_StreamRxStart under WIN32")
TRACE_MSG(STRM_HSM_2010_112_2_17_23_43_11_2992,"#STRM StrmPlayPlaying MEDIA IS START timer id=%d!")
TRACE_MSG(STRM_HSM_2064_112_2_17_23_43_11_2993,"#STRM StrmPlayPlaying MEDIA timer out don't data!")
TRACE_MSG(STRM_HSM_2071_112_2_17_23_43_11_2994,"#STRM StrmPlayPlaying MEDIA timer out!")
TRACE_MSG(STRM_HSM_2076_112_2_17_23_43_11_2995,"#STRM StrmPlayPlaying, DPLAYER_StreamRxStop!")
TRACE_MSG(STRM_HSM_2079_112_2_17_23_43_11_2996,"#STRM StrmPlayPlaying, no DPLAYER_StreamRxStop under WIN32")
TRACE_MSG(STRM_HSM_2089_112_2_17_23_43_11_2997,"#STRM StrmPlayPlaying MEDIA IS STOP timer id=%d")
TRACE_MSG(STRM_HSM_2116_112_2_17_23_43_11_2998,"#STRM StrmPlayPausePending, ERR input!")
TRACE_MSG(STRM_HSM_2160_112_2_17_23_43_11_2999,"#STRM StrmPlayPausePending seek, RTSP PAUSE return failed!")
TRACE_MSG(STRM_HSM_2167_112_2_17_23_43_11_3000,"#STRM StrmPlayPausePending, RTSP PAUSE return failed!")
TRACE_MSG(STRM_HSM_2192_112_2_17_23_43_11_3001,"#STRM StrmPlayPaused, ERR input!")
TRACE_MSG(STRM_HSM_2243_112_2_17_23_43_11_3002,"#STRM StrmStopPending, ERR input!")
TRACE_MSG(STRM_HSM_2246_112_2_17_23_43_11_3003,"#STRM StrmStopPending hsm_handle=0x%x, sig=0x%x!")
TRACE_MSG(STRM_HSM_2264_112_2_17_23_43_11_3004,"#STRM StrmStopPending, RTSP STOP return failed!")
TRACE_MSG(STRM_HSM_2286_112_2_17_23_43_11_3005,"#STRM SetSeekFlag input invalid!")
TRACE_MSG(STRM_HSM_2289_112_2_17_23_43_11_3006,"#STRM SetSeekFlag to %d")
TRACE_MSG(STRM_HSM_2304_112_2_17_23_43_11_3007,"#STRM SetResumeFlag input invalid!")
TRACE_MSG(STRM_HSM_2307_112_2_17_23_43_11_3008,"#STRM SetResumeFlag to %d")
TRACE_MSG(STRM_HSM_2322_112_2_17_23_43_11_3009,"#STRM SaveAuthInfo input invalid!")
TRACE_MSG(STRM_HSM_2328_112_2_17_23_43_11_3010,"#STRM HandleAuthRsp err auth rsp param!")
TRACE_MSG(STRM_HSM_2345_112_2_17_23_43_11_3011,"#STRM HandleAuthRsp err save username!")
TRACE_MSG(STRM_HSM_2351_112_2_17_23_43_11_3012,"#STRM HandleAuthRsp err save password_ptr!")
TRACE_MSG(STRM_HSM_2367_112_2_17_23_43_11_3013,"#STRM HandlePauseReq input invalid!")
TRACE_MSG(STRM_HSM_2375_112_2_17_23_43_11_3014,"#STRM HandlePauseReq pause fail!")
TRACE_MSG(STRM_HSM_2391_112_2_17_23_43_11_3015,"#STRM HandleCloseReq input invalid!")
TRACE_MSG(STRM_HSM_2422_112_2_17_23_43_11_3016,"#STRM HandleCloseReq input invalid!")
TRACE_MSG(STRM_HSM_2433_112_2_17_23_43_11_3017,"#STRM HandleStopReq teardown failed!")
TRACE_MSG(STRM_HSM_2458_112_2_17_23_43_12_3018,"#STRM HandleRtcpSrrrSendInd input invalid!")
TRACE_MSG(STRM_HSM_2487_112_2_17_23_43_12_3019,"#STRM StrmPlay STRM_RTP_SRRR_SEND_IND, error payload type!")
TRACE_MSG(STRM_HSM_2512_112_2_17_23_43_12_3020,"#STRM HandleRtcpSrrrSendInd send NADU, free buffer:%x, nsn:%d, nun:%d")
TRACE_MSG(STRM_HSM_2526_112_2_17_23_43_12_3021,"#STRM HandleRtcpSrrrSendInd get nsn nun fail!")
TRACE_MSG(STRM_HSM_2550_112_2_17_23_43_12_3022,"#STRM HandlePlayReq input invalid!")
TRACE_MSG(STRM_HSM_2560_112_2_17_23_43_12_3023,"#STRM HandlePlayReq, no DPLAYER_StreamRegistRxDataPath under WIN32")
TRACE_MSG(STRM_HSM_2591_112_2_17_23_43_12_3024,"#STRM HandlePlayReq RTSP SETUP FAILED!")
TRACE_MSG(STRM_HSM_2616_112_2_17_23_43_12_3025,"#STRM HandleSeekReq input invalid!")
TRACE_MSG(STRM_HSM_2654_112_2_17_23_43_12_3026,"#STRM HandleSeekReq SEEK fail: invalid start_time!")
TRACE_MSG(STRM_HSM_2658_112_2_17_23_43_12_3027,"#STRM HandleSeekReq, send goodbye")
TRACE_MSG(STRM_HSM_2670_112_2_17_23_43_12_3028,"#STRM HandleSeekReq PAUSED SEEK fail!")
TRACE_MSG(STRM_HSM_2679_112_2_17_23_43_12_3029,"#STRM HandleSeekReq NON PAUSED fail!")
TRACE_MSG(STRM_HSM_2701_112_2_17_23_43_12_3030,"#STRM HandleResumeReq input invalid!")
TRACE_MSG(STRM_HSM_2723_112_2_17_23_43_12_3031,"#STRM StrmPlayBuffering RESUME fail!")
TRACE_MSG(STRM_HSM_2745_112_2_17_23_43_12_3032,"#STRM STRMHSM_InsertPacketToBuf input invalid!")
TRACE_MSG(STRM_HSM_2753_112_2_17_23_43_12_3033,"#STRM STRMBUF_InsertPacket audio strm_handle=0x%x timestamp:0x%x, now buf_size=%d")
TRACE_MSG(STRM_HSM_2759_112_2_17_23_43_12_3034,"#STRM STRMBUF_InsertPacket video strm_handle=0x%x timestamp:0x%x, now buf_size=%d")
TRACE_MSG(STRM_HSM_2763_112_2_17_23_43_12_3035,"#STRM STRMHSM_InsertPacketToBuf, wrong payload type!")
TRACE_MSG(STRM_HSM_2775_112_2_17_23_43_12_3036,"#STRM STRMHSM_InsertPacketToBuf, insert fail!")
TRACE_MSG(STRM_HSM_2781_112_2_17_23_43_12_3037,"#STRM STRMHSM_InsertPacketToBuf, buffer FULL, ignore the packet!")
TRACE_MSG(STRM_HSM_2786_112_2_17_23_43_12_3038,"#STRM STRMHSM_InsertPacketToBuf, first packet received, send STRM_RTP_SSRC_SET_IND")
TRACE_MSG(STRM_HSM_2802_112_2_17_23_43_12_3039,"#STRM SendSetRtpSsrcInd input invalid!")
TRACE_MSG(STRM_HSM_2808_112_2_17_23_43_12_3040,"#STRM SendSetRtpSsrcInd, creat signal fail!")
TRACE_MSG(STRM_HSM_2828_112_2_17_23_43_12_3041,"#STRM SendBufferEmptyInd input invalid!")
TRACE_MSG(STRM_HSM_2834_112_2_17_23_43_12_3042,"#STRM SendBufferEmptyInd creat signal FAIL!")
TRACE_MSG(STRM_HSM_2852_112_2_17_23_43_12_3043,"#STRM SendBufferFullInd input invalid!")
TRACE_MSG(STRM_HSM_2858_112_2_17_23_43_12_3044,"#STRM SendBufferEmptyInd creat signal FAIL!")
TRACE_MSG(STRM_HSM_2875_112_2_17_23_43_12_3045,"#STRM AddHsmToList input invalid!")
TRACE_MSG(STRM_HSM_2891_112_2_17_23_43_12_3046,"#STRM AddHsmToList creat list failed!")
TRACE_MSG(STRM_HSM_2898_112_2_17_23_43_12_3047,"#STRM AddHsmToList insert node failed!")
TRACE_MSG(STRM_HSM_2914_112_2_17_23_43_12_3048,"#STRM DeleteHsmFromList hsm_handle=0x%x!")
TRACE_MSG(STRM_HSM_2918_112_2_17_23_43_12_3049,"#STRM DeleteHsmFromList input invalid!")
TRACE_MSG(STRM_HSM_2923_112_2_17_23_43_13_3050,"#STRM DeleteHsmFromList no list exists!")
TRACE_MSG(STRM_HSM_2942_112_2_17_23_43_13_3051,"#STRM DeleteHsmFromList node not exists!")
TRACE_MSG(STRM_HSM_2971_112_2_17_23_43_13_3052,"#STRM CheckIfHsmValid no list exists!")
TRACE_MSG(STRM_HSM_3005_112_2_17_23_43_13_3053,"#STRM STRMHSM_GetHandleByRtspHandle no list exists!")
TRACE_MSG(STRM_HSM_3039_112_2_17_23_43_13_3054,"#STRM STRMHSM_GetRequestUrl input invalid!")
TRACE_MSG(STRM_HSM_3057_112_2_17_23_43_13_3055,"#STRM STRMHSM_GetHandleByRtpHandle no list exists!")
TRACE_MSG(STRM_HSM_3095_112_2_17_23_43_13_3056,"#STRM STRMHSM_GetHandleByRtpHandle no list exists!")
TRACE_MSG(STRM_HSM_3132_112_2_17_23_43_13_3057,"#STRM STRMHSM_GetEncType input invalid!")
TRACE_MSG(STRM_HSM_3160_112_2_17_23_43_13_3058,"#STRM STRMHSM_GetEncType input invalid!")
TRACE_MSG(STRM_HSM_3182_112_2_17_23_43_13_3059,"#STRM STRMHSM_CreatHsm!")
TRACE_MSG(STRM_HSM_3186_112_2_17_23_43_13_3060,"#STRM STRMHSM_CreatHsm input invalid!")
TRACE_MSG(STRM_HSM_3192_112_2_17_23_43_13_3061,"#STRM STRMHSM_CreatHsm hsm_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3198_112_2_17_23_43_13_3062,"#STRM STRMHSM_CreatHsm StrmHsmConstructor fail!")
TRACE_MSG(STRM_HSM_3209_112_2_17_23_43_13_3063,"#STRM STRMHSM_CreatHsm error input, url==0!")
TRACE_MSG(STRM_HSM_3212_112_2_17_23_43_13_3064,"#STRM STRMHSM_CreatHsm input, url: %s")
TRACE_MSG(STRM_HSM_3217_112_2_17_23_43_13_3065,"#STRM STRMHSM_CreatHsm url_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3234_112_2_17_23_43_13_3066,"#STRM STRMHSM_CreatHsm, unsupported URL scheme!")
TRACE_MSG(STRM_HSM_3246_112_2_17_23_43_13_3067,"#STRM STRMHSM_CreatHsm, alloc sdp buffer fail!")
TRACE_MSG(STRM_HSM_3256_112_2_17_23_43_13_3068,"#STRM STRMHSM_CreatHsm, no sdp file input while using http!")
TRACE_MSG(STRM_HSM_3267_112_2_17_23_43_13_3069,"#STRM STRMHSM_CreatHsm user_agent_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3277_112_2_17_23_43_13_3070,"#STRM STRMHSM_CreatHsm uaprof_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3287_112_2_17_23_43_13_3071,"#STRM STRMHSM_CreatHsm proxy.host_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3298_112_2_17_23_43_13_3072,"#STRM STRMHSM_CreatHsm proxy.username_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3308_112_2_17_23_43_13_3073,"#STRM STRMHSM_CreatHsm proxy.password_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3320_112_2_17_23_43_13_3074,"#STRM STRMHSM_CreatHsm auth_info.username_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3329_112_2_17_23_43_13_3075,"#STRM STRMHSM_CreatHsm auth_info.password_ptr alloc fail!")
TRACE_MSG(STRM_HSM_3338_112_2_17_23_43_13_3076,"#STRM STRMHSM_CreatHsm creat buffer fail!")
TRACE_MSG(STRM_HSM_3343_112_2_17_23_43_13_3077,"#STRM STRMHSM_CreatHsm add to list fail!")
TRACE_MSG(STRM_HSM_3361_112_2_17_23_43_13_3078,"#STRM STRMHSM_FreeHsm!")
TRACE_MSG(STRM_HSM_3365_112_2_17_23_43_13_3079,"#STRM STRMHSM_FreeHsm input invalid!")
TRACE_MSG(STRM_HSM_3481_112_2_17_23_43_14_3080,"#STRM STRMHSM_GetAudioPayload input invalid!")
TRACE_MSG(STRM_HSM_3497_112_2_17_23_43_14_3081,"#STRM STRMHSM_GetVideoPayload input invalid!")
TRACE_MSG(STRM_HSM_3513_112_2_17_23_43_14_3082,"#STRM STRMHSM_CheckIfCanRecvRtpPack input invalid!")
TRACE_MSG(STRM_HSM_3525_112_2_17_23_43_14_3083,"#STRM STRMHSM_CheckIfCanRecvRtpPack strm_handle=0x%x Fail!")
TRACE_MSG(STRM_HSM_3538_112_2_17_23_43_14_3084,"#STRM STRMHSM_GetNetId input invalid!")
TRACE_MSG(STRM_HSM_3555_112_2_17_23_43_14_3085,"#STRM STRMHSM_GetSdpInfo input invalid!")
TRACE_MSG(STRM_HSM_3589_112_2_17_23_43_14_3086,"#STRM StrmThreadEntry, strm_handle=0x%x, sig_code=0x%x")
TRACE_MSG(STRM_HSM_3621_112_2_17_23_43_14_3087,"#STRM StrmThreadEntry, invalid signal")
TRACE_MSG(STRM_HSM_3630_112_2_17_23_43_14_3088,"#STRM StrmThreadEntry, invalid strm_handle!")
TRACE_MSG(STRM_HSM_3648_112_2_17_23_43_14_3089,"#STRM STRMHSM_Dispatch err sig_ptr==0!")
TRACE_MSG(STRM_HSM_3676_112_2_17_23_43_14_3090,"#STRM STRMHSM_GetDataBufferTotalTime, input error")
TRACE_MSG(STRM_HSM_3708_112_2_17_23_43_14_3091,"#STRM STRMHSM_GetDataBufferTotalTime, STRM_SDP_ENC_MP4A_LATM audio empty list")
TRACE_MSG(STRM_HSM_3716_112_2_17_23_43_14_3092,"#STRM STRMHSM_GetDataBufferTotalTime, STRM_SDP_ENC_MP4A_LATM audio empty list")
TRACE_MSG(STRM_HSM_3728_112_2_17_23_43_14_3093,"#STRM STRMHSM_GetDataBufferTotalTime, audio empty list")
TRACE_MSG(STRM_HSM_3743_112_2_17_23_43_14_3094,"#STRM STRMHSM_GetDataBufferTotalTime, audio buf_ptr->audio_buf.initial_timestamp= %d, first_time =%d last_time =%d, sample_rate=%d")
TRACE_MSG(STRM_HSM_3770_112_2_17_23_43_14_3095,"#STRM STRMHSM_GetDataBufferTotalTime STRM_SDP_ENC_H264, video empty list")
TRACE_MSG(STRM_HSM_3788_112_2_17_23_43_14_3096,"#STRM STRMHSM_GetDataBufferTotalTime, video empty list")
TRACE_MSG(STRM_HSM_3803_112_2_17_23_43_14_3097,"#STRM STRMHSM_GetDataBufferTotalTime, video buf_ptr->video_buf.initial_timestamp= %d, packet_in_list_ptr->timestamp =%d last_pack_ptr->timestamp =%d, sample_rate=%d")
TRACE_MSG(STRM_HSM_3819_112_2_17_23_43_14_3098,"#STRM STRMHSM_GetDataBufferTotalTime,timestamp_size=%d")
TRACE_MSG(STRM_HSM_3846_112_2_17_23_43_14_3099,"#STRM STRMRTP_SRRRSendMode input invalid!")
TRACE_MSG(STRM_HSM_3856_112_2_17_23_43_14_3100,"#STRM STRM_SendRRPacket session1, get remote address fail!")
TRACE_MSG(STRM_HSM_3865_112_2_17_23_43_14_3101,"#STRM STRM_SendRRPacket session1 rtp function err!")
TRACE_MSG(STRM_HSM_3876_112_2_17_23_43_14_3102,"#STRM STRM_SendRRPacket session2, get remote address fail!")
TRACE_MSG(STRM_HSM_3885_112_2_17_23_43_14_3103,"#STRM STRM_SendRRPacket session2 rtp function err!")
TRACE_MSG(STRM_RTP_128_112_2_17_23_43_15_3104,"#STRM STRMRTP_CreatSession input invalid!")
TRACE_MSG(STRM_RTP_131_112_2_17_23_43_15_3105,"#STRM creat RTP session, net_id:%d")
TRACE_MSG(STRM_RTP_139_112_2_17_23_43_15_3106,"#STRM STRMRTP_CreatSession rtp function err!")
TRACE_MSG(STRM_RTP_147_112_2_17_23_43_15_3107,"#STRM creat RTP session, rtcp port:%d, rtp port:%d, rtp handle:0x%x")
TRACE_MSG(STRM_RTP_168_112_2_17_23_43_15_3108,"#STRM STRMRTP_CloseSession rtp function err!")
TRACE_MSG(STRM_RTP_202_112_2_17_23_43_15_3109,"#STRM STRMRTP_SendRtcpNaduPacket input invalid!")
TRACE_MSG(STRM_RTP_212_112_2_17_23_43_15_3110,"#STRM STRMRTP_SendNadu, get remote address fail!")
TRACE_MSG(STRM_RTP_248_112_2_17_23_43_15_3111,"#STRM STRMRTP_SendRtcpNaduPacket rtp function err!")
TRACE_MSG(STRM_RTP_268_112_2_17_23_43_15_3112,"#STRM RecvDataCb, error input!")
TRACE_MSG(STRM_RTP_274_112_2_17_23_43_15_3113,"#STRM RecvDataCb, error strm_handle!")
TRACE_MSG(STRM_RTP_280_112_2_17_23_43_15_3114,"#STRM RecvDataCb, wrong state, ignore packet!")
TRACE_MSG(STRM_RTP_294_112_2_17_23_43_15_3115,"#STRM RecvDataCb, error payload_type!")
TRACE_MSG(STRM_RTP_300_112_2_17_23_43_15_3116,"#STRM RecvDataCb, UnpackPayloadAndInsert error!")
TRACE_MSG(STRM_RTP_326_112_2_17_23_43_15_3117,"#STRM UnpackPayloadAndInsert, get sdp info fail!")
TRACE_MSG(STRM_RTP_334_112_2_17_23_43_15_3118,"#STRM UnpackPayloadAndInsert UnpackAmrAndInsert FAIL!")
TRACE_MSG(STRM_RTP_341_112_2_17_23_43_15_3119,"#STRM UnpackPayloadAndInsert UnpackH263AndInsert FAIL!")
TRACE_MSG(STRM_RTP_348_112_2_17_23_43_15_3120,"#STRM UnpackPayloadAndInsert UnpackH263plusAndInsert FAIL!")
TRACE_MSG(STRM_RTP_355_112_2_17_23_43_15_3121,"#STRM UnpackPayloadAndInsert UnpackH264AndInsert FAIL!")
TRACE_MSG(STRM_RTP_363_112_2_17_23_43_15_3122,"#STRM UnpackPayloadAndInsert UnpackMP4AndInsert FAIL!")
TRACE_MSG(STRM_RTP_370_112_2_17_23_43_15_3123,"#STRM UnpackPayloadAndInsert UnpackMP4AndInsert FAIL!")
TRACE_MSG(STRM_RTP_375_112_2_17_23_43_15_3124,"#STRM UnpackPayloadAndInsert unsupport enc_type!")
TRACE_MSG(STRM_RTP_403_112_2_17_23_43_15_3125,"#STRM UnpackAmrAndInsert input invalid!")
TRACE_MSG(STRM_RTP_419_112_2_17_23_43_15_3126,"#STRM UnpackAmr RTPPL_AmrUnpackConstruct fail:%d")
TRACE_MSG(STRM_RTP_435_112_2_17_23_43_15_3127,"#STRM RTPPL frame alloc fail")
TRACE_MSG(STRM_RTP_452_112_2_17_23_43_15_3128,"#STRM UnpackAmrAndInsert insert fail!")
TRACE_MSG(STRM_RTP_488_112_2_17_23_43_16_3129,"#STRM UnpackH263AndInsert input invalid!")
TRACE_MSG(STRM_RTP_496_112_2_17_23_43_16_3130,"#STRM UnpackH263AndInsert fail:%d")
TRACE_MSG(STRM_RTP_517_112_2_17_23_43_16_3131,"#STRM UnpackH263AndInsert insert fail!")
TRACE_MSG(STRM_RTP_543_112_2_17_23_43_16_3132,"#STRM UnpackH263plusAndInsert input invalid!")
TRACE_MSG(STRM_RTP_552_112_2_17_23_43_16_3133,"#STRM UnpackH263plusAndInsert RTPPL_H263PlusUnpackConstruct fail:%d")
TRACE_MSG(STRM_RTP_570_112_2_17_23_43_16_3134,"#STRM UnpackH263plusAndInsert insert fail!")
TRACE_MSG(STRM_RTP_601_112_2_17_23_43_16_3135,"#STRM UnpackH264AndInsert input invalid!")
TRACE_MSG(STRM_RTP_611_112_2_17_23_43_16_3136,"#STRM UnpackH264AndInsert RTPPL_H264UnpackConstruct fail:%d")
TRACE_MSG(STRM_RTP_627_112_2_17_23_43_16_3137,"#STRM h264 nalu param alloc fail")
TRACE_MSG(STRM_RTP_657_112_2_17_23_43_16_3138,"#STRM UnpackH264AndInsert insert fail!")
TRACE_MSG(STRM_RTP_694_112_2_17_23_43_16_3139,"#STRM UnpackMP4AndInsert input invalid!")
TRACE_MSG(STRM_RTP_711_112_2_17_23_43_16_3140,"#STRM UnpackMP4AndInsert insert fail!")
TRACE_MSG(STRM_RTP_743_112_2_17_23_43_16_3141,"#STRM UnpackMPEG4AndInsert input invalid!")
TRACE_MSG(STRM_RTP_757_112_2_17_23_43_16_3142,"#STRM UnpackMPEG4AndInsert unknown payload_type!")
TRACE_MSG(STRM_RTP_779_112_2_17_23_43_16_3143,"#STRM UnpackMPEG4AndInsert RTPPL_H264UnpackConstruct fail:%d")
TRACE_MSG(STRM_RTP_795_112_2_17_23_43_16_3144,"#STRM h264 nalu param alloc fail")
TRACE_MSG(STRM_RTP_818_112_2_17_23_43_16_3145,"#STRM UnpackMPEG4AndInsert insert fail: au_size=0x%x!")
TRACE_MSG(STRM_RTP_848_112_2_17_23_43_16_3146,"#STRM AdapSrrrSendInd input invalid!")
TRACE_MSG(STRM_RTP_855_112_2_17_23_43_16_3147,"#STRM AdapSrrrSendInd, wrong strm_handle!")
TRACE_MSG(STRM_RTP_861_112_2_17_23_43_16_3148,"#STRM AdapSrrrSendInd, error when creat signal!")
TRACE_MSG(STRM_RTP_882_112_2_17_23_43_16_3149,"#STRM AdapGoodbyeInd input invalid!")
TRACE_MSG(STRM_RTP_889_112_2_17_23_43_16_3150,"#STRM AdapGoodbyeInd, wrong strm_handle!")
TRACE_MSG(STRM_RTP_895_112_2_17_23_43_16_3151,"#STRM AdapGoodbyeInd, error when creat signal!")
TRACE_MSG(STRM_RTP_916_112_2_17_23_43_16_3152,"#STRM STRMRTP_SigAdap input invalid!")
TRACE_MSG(STRM_RTP_944_112_2_17_23_43_16_3153,"#STRM STRMRTP_SRRRSendMode input invalid!")
TRACE_MSG(STRM_RTP_949_112_2_17_23_43_17_3154,"#STRM STRMRTP_SRRRSendMode rtp_error_code =%d")
TRACE_MSG(STRM_RTSP_266_112_2_17_23_43_17_3155,"#STRM STRMRTSP_InitSessionReq input invalid!")
TRACE_MSG(STRM_RTSP_284_112_2_17_23_43_17_3156,"#STRM STRMRTSP_InitSessionReq, RTSP function error")
TRACE_MSG(STRM_RTSP_302_112_2_17_23_43_17_3157,"#STRM AdapInitCnf input invalid!")
TRACE_MSG(STRM_RTSP_308_112_2_17_23_43_17_3158,"#STRM AdapInitCnf, alloc adap signal fail!")
TRACE_MSG(STRM_RTSP_337_112_2_17_23_43_17_3159,"#STRM CombineTransportHeadValue, alloc fail!")
TRACE_MSG(STRM_RTSP_370_112_2_17_23_43_17_3160,"#STRM Combine3gppAdaptationHeadValue, alloc fail!")
TRACE_MSG(STRM_RTSP_410_112_2_17_23_43_18_3161,"#STRM CombineRangeHeadValue ALLOC fail!")
TRACE_MSG(STRM_RTSP_451_112_2_17_23_43_18_3162,"#STRM STRMRTSP_DescribeReq input invalid!")
TRACE_MSG(STRM_RTSP_475_112_2_17_23_43_18_3163,"#STRM STRMRTSP_DescribeReq, alloc head ptr fail!")
TRACE_MSG(STRM_RTSP_499_112_2_17_23_43_18_3164,"#STRM STRMRTSP_DescribeReq, RTSP function error")
TRACE_MSG(STRM_RTSP_525_112_2_17_23_43_18_3165,"#STRM GetBaseUrlFromDescribeCnf input invalid!")
TRACE_MSG(STRM_RTSP_532_112_2_17_23_43_18_3166,"#STRM GetBaseUrlFromDescribeCnf head_len==0!")
TRACE_MSG(STRM_RTSP_538_112_2_17_23_43_18_3167,"#STRM GetBaseUrlFromDescribeCnf copy head fail!")
TRACE_MSG(STRM_RTSP_565_112_2_17_23_43_18_3168,"#STRM GetBaseUrlFromDescribeCnf error head content base!")
TRACE_MSG(STRM_RTSP_592_112_2_17_23_43_18_3169,"#STRM GetBaseUrlFromDescribeCnf error head content location!")
TRACE_MSG(STRM_RTSP_624_112_2_17_23_43_18_3170,"#STRM AdapDescribeCnf input invalid!")
TRACE_MSG(STRM_RTSP_631_112_2_17_23_43_18_3171,"#STRM AdapDescribeCnf, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_647_112_2_17_23_43_18_3172,"#STRM AdapDescribeCnf, alloc sdp buffer fail!")
TRACE_MSG(STRM_RTSP_662_112_2_17_23_43_18_3173,"#STRM AdapDescribeCnf, describe cnf is STRM_RESULT_UNSUPPORTED!")
TRACE_MSG(STRM_RTSP_668_112_2_17_23_43_18_3174,"#STRM AdapDescribeCnf, describe cnf is not RTSP_SUCCESS!")
TRACE_MSG(STRM_RTSP_675_112_2_17_23_43_18_3175,"#STRM AdapDescribeCnf, serialize fail!")
TRACE_MSG(STRM_RTSP_715_112_2_17_23_43_18_3176,"#STRM STRMRTSP_SetupReq input invalid!")
TRACE_MSG(STRM_RTSP_747_112_2_17_23_43_18_3177,"#STRM STRMRTSP_SetupReq, alloc head ptr fail!")
TRACE_MSG(STRM_RTSP_775_112_2_17_23_43_18_3178,"#STRM STRMRTSP_SetupReq, RTSP function error")
TRACE_MSG(STRM_RTSP_809_112_2_17_23_43_18_3179,"#STRM AdapSetupCnf input invalid!")
TRACE_MSG(STRM_RTSP_816_112_2_17_23_43_18_3180,"#STRM AdapSetupCnf, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_859_112_2_17_23_43_18_3181,"#STRM STRMRTSP_PlayReq input invalid!")
TRACE_MSG(STRM_RTSP_885_112_2_17_23_43_18_3182,"#STRM STRMRTSP_PlayReq, alloc head ptr fail!")
TRACE_MSG(STRM_RTSP_907_112_2_17_23_43_19_3183,"#STRM STRMRTSP_PlayReq, RTSP function error")
TRACE_MSG(STRM_RTSP_937_112_2_17_23_43_19_3184,"#STRM GetPlayTimeRange input invalid")
TRACE_MSG(STRM_RTSP_952_112_2_17_23_43_19_3185,"#STRM GetPlayTimeRange alloc mem fail")
TRACE_MSG(STRM_RTSP_961_112_2_17_23_43_19_3186,"#STRM GetPlayTimeRange, Range:%s, start_time:%d, end_time:%d")
TRACE_MSG(STRM_RTSP_983_112_2_17_23_43_19_3187,"#STRM AdapPlayCnf input invalid")
TRACE_MSG(STRM_RTSP_990_112_2_17_23_43_19_3188,"#STRM AdapPlayCnf, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_997_112_2_17_23_43_19_3189,"#STRM AdapPlayCnf, error when creat signal!")
TRACE_MSG(STRM_RTSP_1029_112_2_17_23_43_19_3190,"#STRM STRMRTSP_PauseReq input invalid")
TRACE_MSG(STRM_RTSP_1045_112_2_17_23_43_19_3191,"#STRM STRMRTSP_PauseReq, alloc head ptr fail!")
TRACE_MSG(STRM_RTSP_1057_112_2_17_23_43_19_3192,"#STRM STRMRTSP_PauseReq, RTSP function error")
TRACE_MSG(STRM_RTSP_1078_112_2_17_23_43_19_3193,"#STRM AdapPauseCnf input invalid")
TRACE_MSG(STRM_RTSP_1085_112_2_17_23_43_19_3194,"#STRM AdapPauseCnf, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_1092_112_2_17_23_43_19_3195,"#STRM AdapPauseCnf, error when creat signal!")
TRACE_MSG(STRM_RTSP_1122_112_2_17_23_43_19_3196,"#STRM STRMRTSP_TearDownReq input invalid")
TRACE_MSG(STRM_RTSP_1139_112_2_17_23_43_19_3197,"#STRM STRMRTSP_SetupReq, alloc head ptr fail!")
TRACE_MSG(STRM_RTSP_1152_112_2_17_23_43_19_3198,"#STRM STRMRTSP_TearDownReq, RTSP function error")
TRACE_MSG(STRM_RTSP_1175_112_2_17_23_43_19_3199,"#STRM AdapTearDownCnf input invalid")
TRACE_MSG(STRM_RTSP_1182_112_2_17_23_43_19_3200,"#STRM AdapTearDownCnf, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_1189_112_2_17_23_43_19_3201,"#STRM AdapTearDownCnf, error when creat signal!")
TRACE_MSG(STRM_RTSP_1219_112_2_17_23_43_19_3202,"#STRM AdapSetParamCnf input invalid")
TRACE_MSG(STRM_RTSP_1226_112_2_17_23_43_19_3203,"#STRM AdapSetParamCnf, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_1233_112_2_17_23_43_19_3204,"#STRM AdapTearDownCnf, error when creat signal!")
TRACE_MSG(STRM_RTSP_1263_112_2_17_23_43_19_3205,"#STRM STRMRTSP_CloseSessionReq input invalid")
TRACE_MSG(STRM_RTSP_1276_112_2_17_23_43_19_3206,"#STRM STRMRTSP_CloseSessionReq, rtsp function err!")
TRACE_MSG(STRM_RTSP_1295_112_2_17_23_43_19_3207,"#STRM AdapCloseCnf input invalid")
TRACE_MSG(STRM_RTSP_1302_112_2_17_23_43_19_3208,"#STRM AdapCloseCnf, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_1309_112_2_17_23_43_19_3209,"#STRM AdapCloseCnf, error when creat signal!")
TRACE_MSG(STRM_RTSP_1338_112_2_17_23_43_19_3210,"#STRM AdapDescribeAuthInd input invalid")
TRACE_MSG(STRM_RTSP_1345_112_2_17_23_43_19_3211,"#STRM AdapDescribeAuthInd, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_1351_112_2_17_23_43_19_3212,"#STRM AdapDescribeAuthInd, error when creat signal!")
TRACE_MSG(STRM_RTSP_1373_112_2_17_23_43_19_3213,"#STRM STRMRTSP_DescribeAuthRsp input invalid")
TRACE_MSG(STRM_RTSP_1386_112_2_17_23_43_20_3214,"#STRM STRMRTSP_DescribeAuthRsp, RTSP function error!")
TRACE_MSG(STRM_RTSP_1404_112_2_17_23_43_20_3215,"#STRM AdapSetupAuthInd input invalid")
TRACE_MSG(STRM_RTSP_1411_112_2_17_23_43_20_3216,"#STRM AdapSetupAuthInd, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_1417_112_2_17_23_43_20_3217,"#STRM AdapSetupAuthInd, error when creat signal!")
TRACE_MSG(STRM_RTSP_1438_112_2_17_23_43_20_3218,"#STRM AdapPlayAuthInd input invalid")
TRACE_MSG(STRM_RTSP_1445_112_2_17_23_43_20_3219,"#STRM AdapPlayAuthInd, wrong strm_handle!")
TRACE_MSG(STRM_RTSP_1451_112_2_17_23_43_20_3220,"#STRM AdapPlayAuthInd, error when creat signal!")
TRACE_MSG(STRM_RTSP_1474_112_2_17_23_43_20_3221,"#STRM STRMRTSP_SetupAuthRsp input invalid")
TRACE_MSG(STRM_RTSP_1487_112_2_17_23_43_20_3222,"#STRM STRMRTSP_SetupAuthRsp, RTSP function error!")
TRACE_MSG(STRM_RTSP_1508_112_2_17_23_43_20_3223,"#STRM STRMRTSP_PlayAuthRsp input invalid")
TRACE_MSG(STRM_RTSP_1521_112_2_17_23_43_20_3224,"#STRM STRMRTSP_PlayAuthRsp, RTSP function error!")
TRACE_MSG(STRM_RTSP_1539_112_2_17_23_43_20_3225,"#STRM STRMRTSP_SigAdap input invalid")
TRACE_MSG(STRM_SDP_269_112_2_17_23_43_21_3226,"#STRM STRMSDP_Parse input invalid")
TRACE_MSG(STRM_SDP_299_112_2_17_23_43_21_3227,"#STRM STRMSDP_Parse parse 3GPP-Adaptation-Support fail!")
TRACE_MSG(STRM_SDP_305_112_2_17_23_43_21_3228,"#STRM STRMSDP_Parse parse session info fail!")
TRACE_MSG(STRM_SDP_312_112_2_17_23_43_21_3229,"#STRM STRMSDP_Parse parse session name fail!")
TRACE_MSG(STRM_SDP_322_112_2_17_23_43_21_3230,"#STRM STRMSDP_Parse GetAudioParam fail!")
TRACE_MSG(STRM_SDP_329_112_2_17_23_43_21_3231,"#STRM STRMSDP_Parse GetVideoParam fail!")
TRACE_MSG(STRM_SDP_336_112_2_17_23_43_21_3232,"#STRM STRMSDP_Parse session end:0x%x, audio_end:0x%x, video_end:0x%x")
TRACE_MSG(STRM_SDP_387_112_2_17_23_43_21_3233,"#STRM STRMSDP_CloneInfo input invalid")
TRACE_MSG(STRM_SDP_411_112_2_17_23_43_21_3234,"#STRM STRMSDP_CloneInfo clone audio_type_name_ptr FAIL!")
TRACE_MSG(STRM_SDP_422_112_2_17_23_43_21_3235,"#STRM STRMSDP_CloneInfo audio_param.config_ptr ALLOC FAIL!")
TRACE_MSG(STRM_SDP_435_112_2_17_23_43_21_3236,"#STRM STRMSDP_CloneInfo clone video_type_name_ptr FAIL!")
TRACE_MSG(STRM_SDP_446_112_2_17_23_43_21_3237,"#STRM STRMSDP_CloneInfo video_param.config_ptr ALLOC FAIL!")
TRACE_MSG(STRM_SDP_458_112_2_17_23_43_21_3238,"#STRM STRMSDP_CloneInfo audio_param.url_ptr clone FAIL!")
TRACE_MSG(STRM_SDP_467_112_2_17_23_43_21_3239,"#STRM STRMSDP_CloneInfo video_param.url_ptr clone FAIL!")
TRACE_MSG(STRM_SDP_476_112_2_17_23_43_21_3240,"#STRM STRMSDP_CloneInfo media_name_ptr clone FAIL!")
TRACE_MSG(STRM_SDP_485_112_2_17_23_43_21_3241,"#STRM STRMSDP_CloneInfo session_info_ptr clone FAIL!")
TRACE_MSG(STRM_SDP_498_112_2_17_23_43_21_3242,"#STRM STRMSDP_CloneInfo sprop_sets_ptr CFL_ALLOC  FAIL!")
TRACE_MSG(STRM_SDP_522_112_2_17_23_43_21_3243,"#STRM STRMSDP_FreeSdpInfo input invalid")
TRACE_MSG(STRM_SDP_593_112_2_17_23_43_21_3244,"#STRM GetNaduReportRate input invalid")
TRACE_MSG(STRM_SDP_604_112_2_17_23_43_21_3245,"#STRM GetSessionInfo, descr_num==0, wrong SDP!")
TRACE_MSG(STRM_SDP_625_112_2_17_23_43_21_3246,"#STRM GetNaduReportRate, doesn't support audio 3gpp adaptation")
TRACE_MSG(STRM_SDP_637_112_2_17_23_43_21_3247,"#STRM GetNaduReportRate, doesn't support video 3gpp adaptation")
TRACE_MSG(STRM_SDP_657_112_2_17_23_43_21_3248,"#STRM GetSessionInfo input invalid")
TRACE_MSG(STRM_SDP_666_112_2_17_23_43_21_3249,"#STRM GetSessionInfo, string clone fail!")
TRACE_MSG(STRM_SDP_686_112_2_17_23_43_21_3250,"#STRM GetSessionName input invalid")
TRACE_MSG(STRM_SDP_695_112_2_17_23_43_21_3251,"#STRM GetSessionName, string clone fail!")
TRACE_MSG(STRM_SDP_722_112_2_17_23_43_21_3252,"#STRM GetUrlInDescrp input invalid")
TRACE_MSG(STRM_SDP_730_112_2_17_23_43_22_3253,"#STRM GetUrlInDescrp, get URL: %s")
TRACE_MSG(STRM_SDP_734_112_2_17_23_43_22_3254,"#STRM GetUrlInDescrp, the URL is none or no control attribute")
TRACE_MSG(STRM_SDP_743_112_2_17_23_43_22_3255,"#STRM GetUrlInDescrp, CLONE URL fail!")
TRACE_MSG(STRM_SDP_752_112_2_17_23_43_22_3256,"#STRM GetUrlInDescrp, no base URL, cannot generate URL!")
TRACE_MSG(STRM_SDP_761_112_2_17_23_43_22_3257,"#STRM GetUrlInDescrp, ALLOC URL fail!")
TRACE_MSG(STRM_SDP_799_112_2_17_23_43_22_3258,"#STRM STRMSDP_GetTimeRange input invalid")
TRACE_MSG(STRM_SDP_806_112_2_17_23_43_22_3259,"#STRM STRMSDP_GetTimeRange, unsupported range format: %s")
TRACE_MSG(STRM_SDP_822_112_2_17_23_43_22_3260,"#STRM STRMSDP_GetTimeRange, clone fail")
TRACE_MSG(STRM_SDP_854_112_2_17_23_43_22_3261,"#STRM STRMSDP_GetTimeRange, illegal form of range")
TRACE_MSG(STRM_SDP_887_112_2_17_23_43_22_3262,"#STRM GetSessionRange input invalid")
TRACE_MSG(STRM_SDP_895_112_2_17_23_43_22_3263,"#STRM GetSessionRange, Range:%s, start_time:%d, end_time:%d")
TRACE_MSG(STRM_SDP_918_112_2_17_23_43_22_3264,"#STRM GetMediaRange input invalid")
TRACE_MSG(STRM_SDP_926_112_2_17_23_43_22_3265,"#STRM GetMediaRange, Range:%s, start_time:%d, end_time:%d")
TRACE_MSG(STRM_SDP_948_112_2_17_23_43_22_3266,"#STRM GetTimeInMsFromTimeString input invalid")
TRACE_MSG(STRM_SDP_960_112_2_17_23_43_22_3267,"#STRM GetTimeInMsFromTimeString, WRONG time format!")
TRACE_MSG(STRM_SDP_1004_112_2_17_23_43_22_3268,"#STRM IsAllDigit input invalid")
TRACE_MSG(STRM_SDP_1031_112_2_17_23_43_22_3269,"#STRM IsAllHex input invalid")
TRACE_MSG(STRM_SDP_1069_112_2_17_23_43_22_3270,"#STRM GetAudioParam input invalid")
TRACE_MSG(STRM_SDP_1076_112_2_17_23_43_22_3271,"#STRM GetAudioParam, descr_num==0, wrong SDP!")
TRACE_MSG(STRM_SDP_1096_112_2_17_23_43_22_3272,"#STRM GetAudioParam, unsupported protocol:%s")
TRACE_MSG(STRM_SDP_1103_112_2_17_23_43_22_3273,"#STRM GetAudioParam, wrong rtpmap number:%d")
TRACE_MSG(STRM_SDP_1108_112_2_17_23_43_22_3274,"#STRM GetAudioParam, GetRtpMapInfo fail")
TRACE_MSG(STRM_SDP_1136_112_2_17_23_43_22_3275,"#STRM GetAudioParam, GetUrlInDescrp fail")
TRACE_MSG(STRM_SDP_1144_112_2_17_23_43_22_3276,"#STRM GetAudioParam, GetFmtpInfo fail")
TRACE_MSG(STRM_SDP_1155_112_2_17_23_43_22_3277,"#STRM GetAudioParam, no audio exists!")
TRACE_MSG(STRM_SDP_1171_112_2_17_23_43_22_3278,"#STRM GetEncType input invalid")
TRACE_MSG(STRM_SDP_1224_112_2_17_23_43_23_3279,"#STRM FindStrBefChar input invalid")
TRACE_MSG(STRM_SDP_1286_112_2_17_23_43_23_3280,"#STRM GetFramesizeInfo input invalid")
TRACE_MSG(STRM_SDP_1294_112_2_17_23_43_23_3281,"#STRM GetFramesizeInfo, no framesize exists")
TRACE_MSG(STRM_SDP_1302_112_2_17_23_43_23_3282,"#STRM GetFramesizeInfo, no memory allocates")
TRACE_MSG(STRM_SDP_1373_112_2_17_23_43_23_3283,"#STRM GetFramerateInfo input invalid")
TRACE_MSG(STRM_SDP_1381_112_2_17_23_43_23_3284,"#STRM GetFramerateInfo, no framerate exists")
TRACE_MSG(STRM_SDP_1389_112_2_17_23_43_23_3285,"#STRM GetFramerateInfo, no memory allocates")
TRACE_MSG(STRM_SDP_1456_112_2_17_23_43_23_3286,"#STRM GetVideoParam input invalid")
TRACE_MSG(STRM_SDP_1463_112_2_17_23_43_23_3287,"#STRM GetVideoParam, descr_num==0, wrong SDP!")
TRACE_MSG(STRM_SDP_1483_112_2_17_23_43_23_3288,"#STRM GetVideoParam, unsupported protocol:%s")
TRACE_MSG(STRM_SDP_1490_112_2_17_23_43_23_3289,"#STRM GetVideoParam, wrong rtpmap number:%d")
TRACE_MSG(STRM_SDP_1495_112_2_17_23_43_23_3290,"#STRM GetVideoParam, GetRtpMapInfo fail")
TRACE_MSG(STRM_SDP_1510_112_2_17_23_43_23_3291,"#STRM GetVideoParam, GetUrlInDescrp fail")
TRACE_MSG(STRM_SDP_1516_112_2_17_23_43_23_3292,"#STRM GetVideoParam, GetFmtpInfo fail")
TRACE_MSG(STRM_SDP_1525_112_2_17_23_43_23_3293,"#STRM GetFramerateInfo, GetFramesizeInfo fail")
TRACE_MSG(STRM_SDP_1532_112_2_17_23_43_23_3294,"#STRM GetFramerateInfo, GetFramerateInfo fail")
TRACE_MSG(STRM_SDP_1542_112_2_17_23_43_23_3295,"#STRM GetVideoParam, no video exists!")
TRACE_MSG(STRM_SDP_1563_112_2_17_23_43_23_3296,"#STRM GetRtpMapInfo input invalid")
TRACE_MSG(STRM_SDP_1571_112_2_17_23_43_23_3297,"#STRM GetRtpMapInfo, get encode name fail!")
TRACE_MSG(STRM_SDP_1577_112_2_17_23_43_23_3298,"#STRM GetRtpMapInfo, clone encode name fail!")
TRACE_MSG(STRM_SDP_1584_112_2_17_23_43_23_3299,"#STRM GetRtpMapInfo, wrong payload num: %d")
TRACE_MSG(STRM_SDP_1591_112_2_17_23_43_23_3300,"#STRM GetRtpMapInfo, enc_name: %s, payload: %d, clock_rate: %d")
TRACE_MSG(STRM_SDP_1599_112_2_17_23_43_23_3301,"#STRM GetRtpMapInfo, clone encode param fail!")
TRACE_MSG(STRM_SDP_1616_112_2_17_23_43_23_3302,"#STRM GetRtpMapInfo, clone encode param fail!")
TRACE_MSG(STRM_SDP_1672_112_2_17_23_43_23_3303,"#STRM GetConfigInfo input invalid")
TRACE_MSG(STRM_SDP_1678_112_2_17_23_43_23_3304,"#STRM GetConfigInfo, no fmtp exists")
TRACE_MSG(STRM_SDP_1689_112_2_17_23_43_23_3305,"#STRM GetConfigInfo, no fmtp exists")
TRACE_MSG(STRM_SDP_1723_112_2_17_23_43_24_3306,"#STRM GetFmtpInfo input invalid")
TRACE_MSG(STRM_SDP_1729_112_2_17_23_43_24_3307,"#STRM GetFmtpInfo, no fmtp exists")
TRACE_MSG(STRM_SDP_1739_112_2_17_23_43_24_3308,"#STRM GetFmtpInfo GetFmtpAmrParam fail!")
TRACE_MSG(STRM_SDP_1750_112_2_17_23_43_24_3309,"#STRM GetFmtpInfo GetFmtpH264Param fail!")
TRACE_MSG(STRM_SDP_1761_112_2_17_23_43_24_3310,"#STRM GetFmtpInfo, unsupported encode type!")
TRACE_MSG(STRM_SDP_1785_112_2_17_23_43_24_3311,"#STRM GetFmtpParam input invalid")
TRACE_MSG(STRM_SDP_1795_112_2_17_23_43_24_3312,"#STRM GetFmtpParam, WRONG fmtp parameter format!")
TRACE_MSG(STRM_SDP_1819_112_2_17_23_43_24_3313,"#STRM GetFmtpH264Param input invalid")
TRACE_MSG(STRM_SDP_1839_112_2_17_23_43_24_3314,"#STRM GetFmtpAmrParam input invalid")
TRACE_MSG(STRM_SDP_1868_112_2_17_23_43_24_3315,"#STRM GetFmtpMPEG4GenericParam input invalid")
TRACE_MSG(STRM_SDP_1942_112_2_17_23_43_24_3316,"#STRM HexStr2Bin input invalid")
TRACE_MSG(STRM_SDP_1973_112_2_17_23_43_24_3317,"#STRM GetFmtpH264ProLevelId input invalid")
TRACE_MSG(STRM_SDP_1979_112_2_17_23_43_24_3318,"#STRM GetFmtpH264ProLevelId, copied_fmtp_str_ptr clone fail!")
TRACE_MSG(STRM_SDP_1987_112_2_17_23_43_24_3319,"#STRM GetFmtpH264ProLevelId, wrong profile-level-id length!")
TRACE_MSG(STRM_SDP_1996_112_2_17_23_43_24_3320,"#STRM GetFmtpH264ProLevelId, no profile level id exists!")
TRACE_MSG(STRM_SDP_2013_112_2_17_23_43_24_3321,"#STRM GetFmtpSpropSetsNum input invalid")
TRACE_MSG(STRM_SDP_2041_112_2_17_23_43_24_3322,"#STRM GetFmtpSpropSetsArr input invalid")
TRACE_MSG(STRM_SDP_2082_112_2_17_23_43_24_3323,"#STRM GetFmtpSpropSets input invalid")
TRACE_MSG(STRM_SDP_2088_112_2_17_23_43_24_3324,"#STRM GetFmtpSpropSets, copied_fmtp_str_ptr clone fail!")
TRACE_MSG(STRM_SDP_2094_112_2_17_23_43_24_3325,"#STRM GetFmtpSpropSets, no sprop-parameter-sets exists!")
TRACE_MSG(STRM_SDP_2100_112_2_17_23_43_24_3326,"#STRM GetFmtpSpropSets, error sets number: %d")
TRACE_MSG(STRM_SDP_2127_112_2_17_23_43_24_3327,"#STRM GetFmtpSpropSets, CFLBASE64_Decode fail!")
TRACE_MSG(STRM_SDP_2145_112_2_17_23_43_24_3328,"#STRM GetFmtpSpropSets, alloc all_sprop_sets_ptr fail!")
TRACE_MSG(STRM_SDP_2196_112_2_17_23_43_25_3329,"#STRM GetFmtpOctetAlign input invalid")
TRACE_MSG(STRM_SDP_2203_112_2_17_23_43_25_3330,"#STRM GetFmtpOctetAlign, copied_fmtp_str_ptr clone fail!")
TRACE_MSG(STRM_SDP_2226_112_2_17_23_43_25_3331,"#STRM GetFmtpOctetAlign, wrong octet-align value:%d, set to default!")
TRACE_MSG(STRM_SDP_2232_112_2_17_23_43_25_3332,"#STRM GetFmtpOctetAlign, wrong octet-align format, set to default value!")
TRACE_MSG(STRM_SDP_2253_112_2_17_23_43_25_3333,"#STRM GetFmtpCrc input invalid")
TRACE_MSG(STRM_SDP_2260_112_2_17_23_43_25_3334,"#STRM GetFmtpCrc, copied_fmtp_str_ptr clone fail!")
TRACE_MSG(STRM_SDP_2276_112_2_17_23_43_25_3335,"#STRM GetFmtpCrc, wrong crc value:%d, set to default!")
TRACE_MSG(STRM_SDP_2282_112_2_17_23_43_25_3336,"#STRM GetFmtpCrc, wrong crc format, set to default value!")
TRACE_MSG(STRM_SDP_2303_112_2_17_23_43_25_3337,"#STRM GetFmtpRobustSorting input invalid")
TRACE_MSG(STRM_SDP_2310_112_2_17_23_43_25_3338,"#STRM GetFmtpRobustSorting, copied_fmtp_str_ptr clone fail!")
TRACE_MSG(STRM_SDP_2326_112_2_17_23_43_25_3339,"#STRM GetFmtpRobustSorting, wrong robust_sorting value:%d, set to default!")
TRACE_MSG(STRM_SDP_2332_112_2_17_23_43_25_3340,"#STRM GetFmtpRobustSorting, wrong robust_sorting format, set to default value!")
TRACE_MSG(STRM_SDP_2353_112_2_17_23_43_25_3341,"#STRM GetFmtpInterleaving input invalid")
TRACE_MSG(STRM_SDP_2360_112_2_17_23_43_25_3342,"#STRM GetFmtpInterleaving, copied_fmtp_str_ptr clone fail!")
TRACE_MSG(STRM_SDP_2376_112_2_17_23_43_25_3343,"#STRM GetFmtpInterleaving, wrong interleaving format, set to default value!")
TRACE_MSG(STRM_SDP_2413_112_2_17_23_43_25_3344,"#STRM GetFmtpDigitalValueofPara input invalid")
TRACE_MSG(STRM_SDP_2419_112_2_17_23_43_25_3345,"#STRM GetFmtpDigitalValueofPara, copied_fmtp_str_ptr clone fail!")
TRACE_MSG(STRM_SDP_2430_112_2_17_23_43_25_3346,"#STRM GetFmtpDigitalValueofPara, no profile level id exists!")
TRACE_MSG(STRM_SDP_2453_112_2_17_23_43_25_3347,"#STRM GetFmtpMpeg4Mode input invalid")
TRACE_MSG(STRM_SDP_2459_112_2_17_23_43_25_3348,"#STRM GetFmtpMpeg4Mode, copied_fmtp_str_ptr clone fail!")
TRACE_MSG(STRM_SERIALIZE_99_112_2_17_23_43_25_3349,"#STRM SerializeAuthInfo input invalid")
TRACE_MSG(STRM_SERIALIZE_106_112_2_17_23_43_25_3350,"STRM SerializeAuthInfo, username err,len=%d")
TRACE_MSG(STRM_SERIALIZE_116_112_2_17_23_43_25_3351,"STRM SerializeAuthInfo, password_ptr err,len=%d")
TRACE_MSG(STRM_SERIALIZE_140_112_2_17_23_43_26_3352,"#STRM STRMSERIALIZE_AuthRsp input invalid")
TRACE_MSG(STRM_SERIALIZE_149_112_2_17_23_43_26_3353,"STRM STRMSERIALIZE_AuthRsp alloc fail!")
TRACE_MSG(STRM_SERIALIZE_162_112_2_17_23_43_26_3354,"STRM STRMSERIALIZE_AuthRsp serialize fail!")
TRACE_MSG(STRM_SERIALIZE_196_112_2_17_23_43_26_3355,"#STRM SerializeExtSdpInfo input invalid")
TRACE_MSG(STRM_SERIALIZE_204_112_2_17_23_43_26_3356,"#STRM SerializeExtSdpInfo, url1 error!")
TRACE_MSG(STRM_SERIALIZE_216_112_2_17_23_43_26_3357,"#STRM SerializeExtSdpInfo, url2 error!")
TRACE_MSG(STRM_SERIALIZE_229_112_2_17_23_43_26_3358,"#STRM SerializeExtSdpInfo, session_info_ptr error!")
TRACE_MSG(STRM_SERIALIZE_242_112_2_17_23_43_26_3359,"#STRM SerializeExtSdpInfo, media_name_ptr error!")
TRACE_MSG(STRM_SERIALIZE_255_112_2_17_23_43_26_3360,"#STRM SerializeExtSdpInfo, audio_param.audio_type_name_ptr error!")
TRACE_MSG(STRM_SERIALIZE_268_112_2_17_23_43_26_3361,"#STRM SerializeExtSdpInfo, audio_param.config_ptr error!")
TRACE_MSG(STRM_SERIALIZE_281_112_2_17_23_43_26_3362,"#STRM SerializeExtSdpInfo, video_param.config_ptr error!")
TRACE_MSG(STRM_SERIALIZE_294_112_2_17_23_43_26_3363,"#STRM SerializeExtSdpInfo, video_param.video_type_name_ptr error!")
TRACE_MSG(STRM_SERIALIZE_324_112_2_17_23_43_26_3364,"#STRM GetSdpExtInfoLen input invalid")
TRACE_MSG(STRM_SERIALIZE_380_112_2_17_23_43_26_3365,"#STRM STRMSERIALIZE_InitCnf input invalid")
TRACE_MSG(STRM_SERIALIZE_388_112_2_17_23_43_26_3366,"#STRM STRMSERIALIZE_InitCnf serialized_ptr alloc fail!")
TRACE_MSG(STRM_SERIALIZE_403_112_2_17_23_43_26_3367,"STRM STRMSERIALIZE_InitCnf serialize fail!")
TRACE_MSG(STRM_SERIALIZE_427_112_2_17_23_43_26_3368,"#STRM STRMSERIALIZE_RtspDscrbCnf input invalid")
TRACE_MSG(STRM_SERIALIZE_435_112_2_17_23_43_26_3369,"#STRM STRMSERIALIZE_InitCnf serialized_ptr alloc fail!")
TRACE_MSG(STRM_SERIALIZE_448_112_2_17_23_43_26_3370,"STRM STRMSERIALIZE_InitCnf serialize fail!")
TRACE_MSG(STRM_TEST_APP_75_112_2_17_23_43_27_3371,"#STRMTEST StrmTestAppTask STRM_INIT_CNF")
TRACE_MSG(STRM_TEST_APP_83_112_2_17_23_43_27_3372,"#STRMTEST StrmTestAppTask STRM_DESCRIBE_AUTH_IND")
TRACE_MSG(STRM_TEST_APP_93_112_2_17_23_43_27_3373,"#STRMTEST StrmTestAppTask STRM_SETUP_AUTH_IND")
TRACE_MSG(STRM_TEST_APP_100_112_2_17_23_43_27_3374,"#STRMTEST StrmTestAppTask STRM_PLAY_CNF")
TRACE_MSG(STRM_TEST_APP_108_112_2_17_23_43_27_3375,"#STRMTEST StrmTestAppTask STRM_PLAY_AUTH_IND")
TRACE_MSG(STRM_TEST_APP_118_112_2_17_23_43_27_3376,"#STRMTEST StrmTestAppTask STRM_PAUSE_CNF")
TRACE_MSG(STRM_TEST_APP_127_112_2_17_23_43_27_3377,"#STRMTEST StrmTestAppTask STRM_RESUME_CNF")
TRACE_MSG(STRM_TEST_APP_133_112_2_17_23_43_27_3378,"#STRMTEST StrmTestAppTask STRM_SEEK_CNF")
TRACE_MSG(STRM_TEST_APP_137_112_2_17_23_43_27_3379,"#STRMTEST StrmTestAppTask STRM_BUFFERING_IND")
TRACE_MSG(STRM_TEST_APP_140_112_2_17_23_43_27_3380,"#STRMTEST StrmTestAppTask STRM_BUFFER_END_IND")
TRACE_MSG(STRM_TEST_APP_143_112_2_17_23_43_27_3381,"#STRMTEST StrmTestAppTask STRM_STOP_CNF")
TRACE_MSG(STRM_TEST_REF_132_112_2_17_23_43_27_3382,"#STRM StrmTestDecode write file error!")
TRACE_MSG(STRM_TEST_REF_151_112_2_17_23_43_27_3383,"#STRM_TEST StrmTestRefTask alloc memory error")
TRACE_MSG(STRM_TEST_REF_173_112_2_17_23_43_27_3384,"#STRM_TEST StrmTestRefTask creat audio and video file  error")
TRACE_MSG(STRM_TEST_REF_192_112_2_17_23_43_27_3385,"#STRM TEST_REF audio is empty!")
TRACE_MSG(STRM_TEST_REF_207_112_2_17_23_43_27_3386,"#STRM TEST_REF video is empty!")
TRACE_MSG(STRM_TEST_REF_213_112_2_17_23_43_27_3387,"#STRM TEST_REF video and audio both empty!")
TRACE_MSG(STRM_TEST_REF_221_112_2_17_23_43_27_3388,"#STRM closefile error!")
TRACE_MSG(STRM_TEST_RTP_SERVER_116_112_2_17_23_43_28_3389,"#STRM Buffer Test, insert AMR, seq=%d, timestamp=%d")
TRACE_MSG(STRM_TEST_RTP_SERVER_132_112_2_17_23_43_28_3390,"#STRM Buffer Test, insert AMR, seq=%d, timestamp=%d")
TRACE_MSG(MEM_CHECK_136_112_2_17_23_43_32_3391,"tcpip memchk: RecordMalloc() - alloc mem failed!")
TRACE_MSG(MEM_CHECK_174_112_2_17_23_43_32_3392,"tcpip memchk: MEMORY NOT FREED -")
TRACE_MSG(MEM_CHECK_179_112_2_17_23_43_32_3393," - tcpip(%d): %s(%d), size = %d")
TRACE_MSG(MEM_CHECK_188_112_2_17_23_43_32_3394,"tcpip memchk: TOTAL NO FREED MEM SIZE = %d")
TRACE_MSG(MEM_CHECK_192_112_2_17_23_43_32_3395,"tcpip memchk: All MEMORY FREED!")
TRACE_MSG(OSPORT_74_112_2_17_23_43_32_3396,"tcpip: TCPIP stack start up, version: %s")
TRACE_MSG(OSPORT_122_112_2_17_23_43_33_3397,"tcpip: Priority of Task \"%s\" is %d ")
TRACE_MSG(OSPORT_128_112_2_17_23_43_33_3398,"tcpip: stack size of net stack is %d ")
TRACE_MSG(OSPORT_133_112_2_17_23_43_33_3399,"tcpip: stack size of clock stack is %d ")
TRACE_MSG(OSPORT_138_112_2_17_23_43_33_3400,"tcpip: the stack of ping stack is %d ")
TRACE_MSG(OSPORT_207_112_2_17_23_43_33_3401,"tcpip: tcpip_tasks_start() is no use any more!")
TRACE_MSG(OSPORT_217_112_2_17_23_43_33_3402,"tcpip: tcpip_tasks_end() is no use any more!")
TRACE_MSG(OSPORT_288_112_2_17_23_43_33_3403,"tcpip: error lock resource index!")
TRACE_MSG(OSPORT_295_112_2_17_23_43_33_3404,"tcpip: resource %d is not exist, lock failed!")
TRACE_MSG(OSPORT_305_112_2_17_23_43_33_3405,"tcpip: resource %d lock failed, mutex number = %d, task = %x !")
TRACE_MSG(OSPORT_324_112_2_17_23_43_33_3406,"tcpip: error unlock resource index!")
TRACE_MSG(OSPORT_335_112_2_17_23_43_33_3407,"tcpip: resource %d unlock failed, task = %x !")
TRACE_MSG(PCLINK_DECODE_54_112_2_17_23_43_33_3408,"tcpip: alloc pclink decode buffer failed!")
TRACE_MSG(PCLINK_DECODE_73_112_2_17_23_43_33_3409,"tcpip pclink decode warning: data_ptr(%p), data_len(%d), s_pclink_buff(%p)")
TRACE_MSG(PCLINK_DECODE_101_112_2_17_23_43_34_3410,"tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_CFBB: Invalid data length( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_110_112_2_17_23_43_34_3411,"tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d)\n")
TRACE_MSG(PCLINK_DECODE_121_112_2_17_23_43_34_3412,"tcpip pclink decode: 0x%x, DECS_DRCV -> DECS_CFBB: Receive too much( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_158_112_2_17_23_43_34_3413,"tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_IDLE: Invalid data length( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_167_112_2_17_23_43_34_3414,"tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d)\n")
TRACE_MSG(PCLINK_DECODE_188_112_2_17_23_43_34_3415,"tcpip pclink decode: 0x%x, DECS_CFEB, Invalid nPkt(%d), nRcv(%d)n")
TRACE_MSG(PCLINK_DECODE_197_112_2_17_23_43_34_3416,"tcpip pclink decode: Receive Link Setup Message! Set link setup flag!")
TRACE_MSG(PCLINK_DECODE_201_112_2_17_23_43_34_3417,"tcpip pclink decode: Receive Link Close Message! Reset link setup flag!")
TRACE_MSG(PCLINK_DECODE_208_112_2_17_23_43_34_3418,"tcpip pclink decode: Receive Packet from UART, len = %d!")
TRACE_MSG(PCLINK_DECODE_242_112_2_17_23_43_34_3419,"tcpip pclink decode: 0x%x, DECS_CFBE, Invalid nPkt(%d), nRcv(%d)n")
TRACE_MSG(PCLINK_DECODE_259_112_2_17_23_43_34_3420,"tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_IDLE: Invalid data length( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_270_112_2_17_23_43_34_3421,"tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d), nRcv(%d)n")
TRACE_MSG(PCLINK_DECODE_281_112_2_17_23_43_34_3422,"tcpip pclink decode: 0x%x, DECS_DRCV -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_287_112_2_17_23_43_34_3423,"tcpip pclink decode: 0x%x, DECS_CFEB -> DECS_IDLE: End flag lost!\n")
TRACE_MSG(PCLINK_DECODE_301_112_2_17_23_43_34_3424,"tcpip pclink decode: 0x%x, DECS_CFSB -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_326_112_2_17_23_43_34_3425,"tcpip pclink decode: 0x%x, DECS_CFBE, Invalid nPkt(%d), nRcv(%d)n")
TRACE_MSG(PCLINK_DECODE_343_112_2_17_23_43_34_3426,"tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_IDLE: Invalid data length( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_354_112_2_17_23_43_34_3427,"tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d), nRcv(%d)n")
TRACE_MSG(PCLINK_DECODE_365_112_2_17_23_43_34_3428,"tcpip pclink decode: 0x%x, DECS_DRCV -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_371_112_2_17_23_43_34_3429,"tcpip pclink decode: 0x%x, DECS_CFEB -> DECS_IDLE: End flag lost!\n")
TRACE_MSG(PCLINK_DECODE_389_112_2_17_23_43_34_3430,"tcpip pclink decode: 0x%x, DECS_CFCB -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_420_112_2_17_23_43_34_3431,"tcpip pclink decode: 0x%x, DECS_DLEN -> DECS_IDLE: Invalid data length( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_431_112_2_17_23_43_34_3432,"tcpip pclink decode: 0x%x, DECS_DRCV, Invalid nPkt(%d), nRcv(%d)n")
TRACE_MSG(PCLINK_DECODE_442_112_2_17_23_43_34_3433,"tcpip pclink decode: 0x%x, DECS_DRCV -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n")
TRACE_MSG(PCLINK_DECODE_449_112_2_17_23_43_34_3434,"tcpip pclink decode: 0x%x, DECS_CFEB -> DECS_IDLE: Receive too much( nPkt:%d, nRcv:%d )n")
TRACE_MSG(STARTUP_PREP_174_112_2_17_23_43_35_3435,"tcpip debug: static Dnsbuf memory - 0x%x ~ 0x%x, size %d")
TRACE_MSG(STARTUP_PREP_197_112_2_17_23_43_35_3436,"tcpip debug: static sockbuf memory - 0x%x ~ 0x%x, size %d")
TRACE_MSG(STARTUP_PREP_275_112_2_17_23_43_36_3437,"tcpip: read dns from nv error!")
TRACE_MSG(STARTUP_PREP_289_112_2_17_23_43_36_3438,"tcpip: default spare dns1: 0x%08x, dns2: 0x%08x")
TRACE_MSG(TCPIP_BEARER_92_112_2_17_23_43_36_3439,"tcpip: register netif error - config check failed")
TRACE_MSG(TCPIP_BEARER_101_112_2_17_23_43_36_3440,"tcpip: register netif error - no free netif found")
TRACE_MSG(TCPIP_BEARER_148_112_2_17_23_43_36_3441,"tcpip: update netif error - config check failed")
TRACE_MSG(TCPIP_BEARER_157_112_2_17_23_43_36_3442,"tcpip: update netif error - no netif found")
TRACE_MSG(TCPIP_BEARER_165_112_2_17_23_43_36_3443,"tcpip: update netif error - netif %x is not open")
TRACE_MSG(TCPIP_BEARER_202_112_2_17_23_43_36_3444,"tcpip: duplicate new netif error - no source netif found")
TRACE_MSG(TCPIP_BEARER_210_112_2_17_23_43_36_3445,"tcpip: duplicate new netif error - source netif is not open")
TRACE_MSG(TCPIP_BEARER_219_112_2_17_23_43_36_3446,"tcpip: duplicate new netif error - no dst netif found")
TRACE_MSG(TCPIP_BEARER_263_112_2_17_23_43_36_3447,"tcpip: duplicate old netif error - no source netif found")
TRACE_MSG(TCPIP_BEARER_272_112_2_17_23_43_36_3448,"tcpip: duplicate old netif error - no dst netif found")
TRACE_MSG(TCPIP_BEARER_281_112_2_17_23_43_36_3449,"tcpip: duplicate old netif error - netif are not open: src(%d), dst(%d)")
TRACE_MSG(TCPIP_BEARER_316_112_2_17_23_43_37_3450,"tcpip: dereg net interface - inval netid %x")
TRACE_MSG(TCPIP_BEARER_323_112_2_17_23_43_37_3451,"tcpip: dereg net interface - netif %p is not open!")
TRACE_MSG(TCPIP_BEARER_372_112_2_17_23_43_37_3452,"tcpip: netid(%x), in(%d), out(%d), in_rd(%d), out_rd(%d), in_drop(%d)")
TRACE_MSG(TCPIP_BEARER_427_112_2_17_23_43_37_3453,"tcpip: open modem service - invalid net_id %x")
TRACE_MSG(TCPIP_BEARER_436_112_2_17_23_43_37_3454,"tcpip: open modem service - inval netif(%p) state, open(%d), modem(%d)")
TRACE_MSG(TCPIP_BEARER_444_112_2_17_23_43_37_3455,"tcpip: open modem service - ppp_ifopen() failed!")
TRACE_MSG(TCPIP_BEARER_450_112_2_17_23_43_37_3456,"tcpip: open MODEM mode on netif(%x) - pppif(%p), modem_id(%d)")
TRACE_MSG(TCPIP_BEARER_483_112_2_17_23_43_37_3457,"tcpip: close modem srv - invalid netid %x")
TRACE_MSG(TCPIP_BEARER_491_112_2_17_23_43_37_3458,"tcpip: close MODEM mode on netif(%p)")
TRACE_MSG(TCPIP_BEARER_520_112_2_17_23_43_37_3459,"tcpip: set IP address error - invalid addr_ptr")
TRACE_MSG(TCPIP_BEARER_528_112_2_17_23_43_37_3460,"tcpip: set IP address error - invalid net_id %x")
TRACE_MSG(TCPIP_BEARER_559_112_2_17_23_43_37_3461,"tcpip: tx cnf - invalid net_id %x")
TRACE_MSG(TCPIP_BEARER_566_112_2_17_23_43_37_3462,"tcpip: tx cnf - error called on net_id %x")
TRACE_MSG(TCPIP_BEARER_601_112_2_17_23_43_37_3463,"tcpip: TCPIP_RxInd() - invalid netid %x")
TRACE_MSG(TCPIP_BEARER_613_112_2_17_23_43_37_3464,"tcpip: TCPIP_RxInd() - warning, len(%d) > mtu(%d)")
TRACE_MSG(TCPIP_BEARER_623_112_2_17_23_43_37_3465,"tcpip: TCPIP_RxInd() - alloc buffer failed, len %d")
TRACE_MSG(TCPIP_BEARER_653_112_2_17_23_43_37_3466,"tcpip: TCPIP_RxInd() - invalid prot pointer for pkt %p")
TRACE_MSG(TCPIP_BEARER_680_112_2_17_23_43_37_3467,"tcpip: TCPIP_RxInd() - unknown packet type %d!")
TRACE_MSG(TCPIP_BEARER_715_112_2_17_23_43_37_3468,"tcpip: TCPIP_RxInd() - no ppp supported, free pkt %x")
TRACE_MSG(TCPIP_BEARER_749_112_2_17_23_43_37_3469,"tcpip: set tx flow ctrl - net_id is NULL")
TRACE_MSG(TCPIP_BEARER_757_112_2_17_23_43_37_3470,"tcpip: set tx flow ctrl - invalid net_id %x")
TRACE_MSG(TCPIP_BEARER_764_112_2_17_23_43_37_3471,"tcpip: tx flowctrl OPENED on netid %x")
TRACE_MSG(TCPIP_BEARER_779_112_2_17_23_43_38_3472,"tcpip: tx flowctrl CLOSED on netid %x, last %d ms")
TRACE_MSG(TCPIP_BEARER_801_112_2_17_23_43_38_3473,"tcpip: set tx rate - invalid net_id %x")
TRACE_MSG(TCPIP_BEARER_811_112_2_17_23_43_38_3474,"tcpip: set tx rate %d bytes/s on netid %x")
TRACE_MSG(TCPIP_BEARER_829_112_2_17_23_43_38_3475,"tcpip: set mtu - invalid net_id %x")
TRACE_MSG(TCPIP_BEARER_849_112_2_17_23_43_38_3476,"tcpip: set mtu %d on netid %x")
TRACE_MSG(TCPIP_BEARER_872_112_2_17_23_43_38_3477,"tcpip: resume TCPIP timer")
TRACE_MSG(TCPIP_BEARER_925_112_2_17_23_43_38_3478,"tcpip: %s - invalid cfg_ptr")
TRACE_MSG(TCPIP_BEARER_933_112_2_17_23_43_38_3479,"tcpip: %s - invalid packet type")
TRACE_MSG(TCPIP_BEARER_940_112_2_17_23_43_38_3480,"tcpip: %s - invalid tx_req_fptr")
TRACE_MSG(TCPIP_BEARER_949_112_2_17_23_43_38_3481,"tcpip: %s - invalid hard address - ptr(%p), len(%d)")
TRACE_MSG(TCPIP_BEARER_957_112_2_17_23_43_38_3482,"tcpip: %s - invalid hard address for ETHERNET")
TRACE_MSG(TCPIP_BEARER_1145_112_2_17_23_43_38_3483,"tcpip: Copy Net Interface - dst_netid(%p), src_netid(%p)")
TRACE_MSG(TCPIP_PCLINK_73_112_2_17_23_43_38_3484,"tcpip pclink: get pclink flag from NV error!")
TRACE_MSG(TCPIP_PCLINK_92_112_2_17_23_43_39_3485,"tcpip pclink: no free netif found!")
TRACE_MSG(TCPIP_PCLINK_167_112_2_17_23_43_39_3486,"tcpip pclink: invalid input!")
TRACE_MSG(TCPIP_PCLINK_173_112_2_17_23_43_39_3487,"tcpip pclink: to send pkt %p, iplen(%d)")
TRACE_MSG(TCPIP_PCLINK_305_112_2_17_23_43_39_3488,"tcpip pclink: set pclink netif error - no pclink prepared!")
TRACE_MSG(TCPIP_PCLINK_320_112_2_17_23_43_39_3489,"tcpip pclink: set ip address on netid %x failed!")
TRACE_MSG(TCPIP_PCLINK_330_112_2_17_23_43_39_3490,"tcpip pclink: switch UART to DATA mode for PCLINK use!")
TRACE_MSG(TCPIP_PS_164_112_2_17_23_43_40_3491,"tcpip ps: no netif in-use on GPRS")
TRACE_MSG(TCPIP_PS_178_112_2_17_23_43_40_3492,"tcpip ps: invalid NSAPI %d")
TRACE_MSG(TCPIP_PS_186_112_2_17_23_43_40_3493,"tcpip ps: get netid - invalid index %d for nsapi %d")
TRACE_MSG(TCPIP_PS_202_112_2_17_23_43_40_3494,"tcpip ps: GetNsapiByNetid - netid is NULL")
TRACE_MSG(TCPIP_PS_226_112_2_17_23_43_40_3495,"tcpip ps: GetNsapiByNetid - netid is NULL")
TRACE_MSG(TCPIP_PS_237_112_2_17_23_43_40_3496,"tcpip ps: no cid found with netid %x")
TRACE_MSG(TCPIP_PS_251_112_2_17_23_43_40_3497,"tcpip ps: GetLinkidByNetid - netid is NULL")
TRACE_MSG(TCPIP_PS_262_112_2_17_23_43_40_3498,"tcpip ps: no link ID found with netid %x")
TRACE_MSG(TCPIP_PS_326_112_2_17_23_43_40_3499,"tcpip ps: set net interface - inval nsapi(%d, %d)")
TRACE_MSG(TCPIP_PS_396_112_2_17_23_43_40_3500,"tcpip ps: set ppp interface - inval nsapi(%d)")
TRACE_MSG(TCPIP_PS_406_112_2_17_23_43_40_3501,"tcpip ps: set ppp interface - no ppp supported")
TRACE_MSG(TCPIP_PS_421_112_2_17_23_43_40_3502,"tcpip: TCPIP_ResetPPPInterface is called")
TRACE_MSG(TCPIP_PS_445_112_2_17_23_43_41_3503,"tcpip ps: GPRSTCPIP_SetPPPService - no netif in use for GPRS")
TRACE_MSG(TCPIP_PS_453_112_2_17_23_43_41_3504,"tcpip ps: GPRSTCPIP_SetPPPService - no nsapi with netid %x")
TRACE_MSG(TCPIP_PS_460_112_2_17_23_43_41_3505,"tcpip ps: GPRSTCPIP_SetPPPService - no ppp supported")
TRACE_MSG(TCPIP_PS_506_112_2_17_23_43_41_3506,"tcpip: MPPP_SetPPPInterface - invalid input, nsapi(%d), link_id(%d), cid(%d)")
TRACE_MSG(TCPIP_PS_519_112_2_17_23_43_41_3507,"tcpip: MPPP_SetPPPInterface - nsapi(%d), link_id(%d), cid(%d)")
TRACE_MSG(TCPIP_PS_524_112_2_17_23_43_41_3508,"tcpip: MPPP_SetPPPInterface - no ppp supported")
TRACE_MSG(TCPIP_PS_540_112_2_17_23_43_41_3509,"tcpip: MPPP_ResetPPPInterface is called")
TRACE_MSG(TCPIP_PS_571_112_2_17_23_43_41_3510,"tcpip: ppp disconnect on netif %x by mux release")
TRACE_MSG(TCPIP_PS_687_112_2_17_23_43_41_3511,"tcpip ps: RawSendToPs() - invalid input")
TRACE_MSG(TCPIP_PS_711_112_2_17_23_43_41_3512,"tcpip ps: MNGPRS_SendDataReq() failed!")
TRACE_MSG(TCPIP_PS_722_112_2_17_23_43_41_3513,"tcpip ps: MNGPRS_SendDataReq() failed!")
TRACE_MSG(TCPIP_PS_750_112_2_17_23_43_41_3514,"tcpip ps: set netif - invalid index %d for nsapi %d")
TRACE_MSG(TCPIP_PS_764_112_2_17_23_43_41_3515,"tcpip ps: set net interface for nsapi(%d, %d)")
TRACE_MSG(TCPIP_PS_776_112_2_17_23_43_41_3516,"tcpip ps: update net interface for nsapi(%d, %d)")
TRACE_MSG(TCPIP_PS_789_112_2_17_23_43_41_3517,"tcpip ps: invalid index %d for parent nsapi %d")
TRACE_MSG(TCPIP_PS_794_112_2_17_23_43_41_3518,"tcpip ps: no netif for parent nsapi %d")
TRACE_MSG(TCPIP_PS_807_112_2_17_23_43_41_3519,"tcpip ps: copy net interface for nsapi(%d, %d)")
TRACE_MSG(TCPIP_PS_819_112_2_17_23_43_41_3520,"tcpip ps: update net interface by nsapi(%d, %d)")
TRACE_MSG(TCPIP_PS_846_112_2_17_23_43_41_3521,"tcpip ps: reset net interface for nsapi %d")
TRACE_MSG(TCPIP_PS_878_112_2_17_23_43_42_3522,"tcpip ps: open modem - invalid index %d for nsapi %d")
TRACE_MSG(TCPIP_PS_882_112_2_17_23_43_42_3523,"tcpip ps: to open modem - nsapi %d, modem_id %d, cid %d")
TRACE_MSG(TCPIP_PS_917_112_2_17_23_43_42_3524,"tcpip ps: close modem - invalid index %d for cid %d")
TRACE_MSG(TCPIP_PS_922_112_2_17_23_43_42_3525,"tcpip ps: to close modem - nsapi %d, modem_id %d, cid %d")
TRACE_MSG(TCPIP_VETHER_113_112_2_17_23_43_42_3526,"tcpip: Create task %s with ID : %d")
TRACE_MSG(TCPIP_VETHER_115_112_2_17_23_43_42_3527,"tcpip: Priority of task %s is : %d")
TRACE_MSG(TCPIP_VETHER_138_112_2_17_23_43_42_3528,"tcpip: TCPIPVETH_Open - no netid found with nsapi %d")
TRACE_MSG(TCPIP_VETHER_146_112_2_17_23_43_42_3529,"tcpip: TCPIPVETH_Open - netif with nsapi %d is not open")
TRACE_MSG(TCPIP_VETHER_162_112_2_17_23_43_42_3530,"tcpip: TCPIPVETH_Open - veth_open on minor %x!")
TRACE_MSG(TCPIP_VETHER_166_112_2_17_23_43_42_3531,"tcpip: TCPIPVETH_Open - veth_open failed on minor %x!")
TRACE_MSG(TCPIP_VETHER_171_112_2_17_23_43_42_3532,"tcpip: TCPIPVETH_Open - minor is in use!")
TRACE_MSG(TCPIP_VETHER_176_112_2_17_23_43_42_3533,"tcpip: TCPIPVETH_Open - vEther is not supported!")
TRACE_MSG(TCPIP_VETHER_192_112_2_17_23_43_42_3534,"tcpip: TCPIPVETH_Close - on minor %x!")
TRACE_MSG(TCPIP_VETHER_205_112_2_17_23_43_42_3535,"tcpip: TCPIPVETH_close - vEther is not supported!")
TRACE_MSG(TCPIP_VETHER_221_112_2_17_23_43_42_3536,"tcpip: TCPIPVETH_SendPacket - invalid input")
TRACE_MSG(TCPIP_VETHER_231_112_2_17_23_43_42_3537,"tcpip: TCPIPVETH_SendPacket - veth_tx failed")
TRACE_MSG(TCPIP_VETHER_235_112_2_17_23_43_42_3538,"tcpip: TCPIPVETH_SendToVeth - vEther is not supported, drop pkt %x")
TRACE_MSG(TCPIP_VETHER_321_112_2_17_23_43_42_3539,"tcpip: invalid data para from vEth, ptr %x, len %d")
TRACE_MSG(TCPIP_VETHER_332_112_2_17_23_43_42_3540,"tcpip: VethRecvTaskEntry - invalid minor")
TRACE_MSG(TCPIP_VETHER_347_112_2_17_23_43_42_3541,"tcpip: VethToBearer - invalid input")
TRACE_MSG(TCPIP_VETHER_357_112_2_17_23_43_43_3542,"tcpip: VethToBearer - alloc buffer failed, len %d")
TRACE_MSG(TCPIP_VETHER_372_112_2_17_23_43_43_3543,"tcpip: VethToBearer - unsupported packet type %d!")
TRACE_MSG(TCPIP_VETHER_403_112_2_17_23_43_43_3544,"tcpip: resume veth receive task!")
TRACE_MSG(UART_MUX_68_112_2_17_23_43_43_3545,"ppp: create MUX link - netif %x, link_id %d, ret %d")
TRACE_MSG(UART_MUX_84_112_2_17_23_43_43_3546,"ppp: release MUX link - netif %x, link_id %d, ret %d")
TRACE_MSG(UART_MUX_100_112_2_17_23_43_43_3547,"ppp: UART recv mode: 1 - normal timing check!")
TRACE_MSG(UART_MUX_103_112_2_17_23_43_43_3548,"ppp: UART recv mode: 2 - alloc buffer and wake up demux task!")
TRACE_MSG(UART_MUX_105_112_2_17_23_43_43_3549,"ppp: UART recv mode: 3 - MUX directly upcall")
TRACE_MSG(UART_MUX_127_112_2_17_23_43_43_3550,"ppp: no pppif found with link id %d")
TRACE_MSG(UART_MUX_143_112_2_17_23_43_43_3551,"ppp: UARTMUX_SendBuffCheck - invalid input len %d")
TRACE_MSG(UART_MUX_162_112_2_17_23_43_43_3552,"ppp: not enough send buffer (tot: %d, left: %d, need: %d)")
TRACE_MSG(UART_MUX_185_112_2_17_23_43_43_3553,"ppp: UARTMUX_SendBuffData - invalid buffer(%p) or data ptr")
TRACE_MSG(UART_MUX_198_112_2_17_23_43_43_3554,"ppp: send %d bytes on link id %d")
TRACE_MSG(UART_MUX_213_112_2_17_23_43_43_3555,"ppp: send %d bytes on link id %d")
TRACE_MSG(UART_MUX_222_112_2_17_23_43_43_3556,"ppp: send %d bytes on link id %d")
TRACE_MSG(UART_MUX_232_112_2_17_23_43_43_3557,"ppp: UARTMUX_SendBuffData - start point(%d) = end point(%d)!")
TRACE_MSG(UART_MUX_291_112_2_17_23_43_43_3558,"ppp: UARTMUX_PutChar - alloc send buffer failed!")
TRACE_MSG(UART_MUX_300_112_2_17_23_43_43_3559,"ppp: UARTMUX_PutChar - alloc data buffer failed!")
TRACE_MSG(UART_MUX_320_112_2_17_23_43_43_3560,"ppp: UARTMUX_PutChar - invalid buff_ptr %p or data_ptr")
TRACE_MSG(UART_MUX_338_112_2_17_23_43_43_3561,"ppp: UARTMUX_PutChar - buffer is full, can't hold char 0x%x")
TRACE_MSG(UART_MUX_370_112_2_17_23_43_44_3562,"ppp: MuxDataRecvUpcall - invalid input!")
TRACE_MSG(UART_MUX_377_112_2_17_23_43_44_3563,"tcpip test: MuxDataRecvUpcall - drop %d bytes!")
TRACE_MSG(UART_MUX_383_112_2_17_23_43_44_3564,"ppp: %d bytes received with link_id %d")
TRACE_MSG(UART_MUX_431_112_2_17_23_43_44_3565,"ppp: receive buff is not enough, space(%d), data_len(%d), link_id(%d)")
TRACE_MSG(UART_MUX_454_112_2_17_23_43_44_3566,"ppp: MuxDataRecvUpcall - invalid input!")
TRACE_MSG(UART_MUX_461_112_2_17_23_43_44_3567,"tcpip test: MuxDataRecvUpcall - drop %d bytes!")
TRACE_MSG(UART_MUX_467_112_2_17_23_43_44_3568,"ppp: %d bytes received with link_id %d")
TRACE_MSG(UART_MUX_484_112_2_17_23_43_44_3569,"ppp: MuxDataRecvUpcall - no pppif matched with link_id %d")
TRACE_MSG(UARTUTIL_142_112_2_17_23_43_44_3570,"ppp: ln_uconnect() called!")
TRACE_MSG(UARTUTIL_162_112_2_17_23_43_44_3571,"ppp error: ln_udisconnect - line is NULL!")
TRACE_MSG(UARTUTIL_169_112_2_17_23_43_44_3572,"ppp error: ln_udisconnect - pppif is NULL!")
TRACE_MSG(UARTUTIL_173_112_2_17_23_43_44_3573,"ppp: ln_udisconnect() called!")
TRACE_MSG(UARTUTIL_185_112_2_17_23_43_44_3574,"ppp: netif %p is not in modem mode!")
TRACE_MSG(UARTUTIL_199_112_2_17_23_43_44_3575,"ppp: ppp_disconnect() - invalid netif_ptr or n_local!")
TRACE_MSG(UARTUTIL_204_112_2_17_23_43_44_3576,"ppp: start pppif(%p) disconnect!")
TRACE_MSG(UARTUTIL_215_112_2_17_23_43_44_3577,"ppp: ppp_disconnect() - wait for %d ms!")
TRACE_MSG(UARTUTIL_229_112_2_17_23_43_44_3578,"ppp: switch UART to AT mode")
TRACE_MSG(UARTUTIL_247_112_2_17_23_43_44_3579,"ppp: ppp_connect() - invalid netif_ptr or n_local!")
TRACE_MSG(UARTUTIL_260_112_2_17_23_43_44_3580,"ppp: switch UART to DATA mode")
TRACE_MSG(UARTUTIL_264_112_2_17_23_43_44_3581,"ppp: start pppif(%p) - ppp lower up!")
TRACE_MSG(UARTUTIL_271_112_2_17_23_43_44_3582,"ppp: to start PPP connect - sleep %d ms ...")
TRACE_MSG(UARTUTIL_327_112_2_17_23_43_45_3583,"ppp: uart recv %d bytes")
TRACE_MSG(UARTUTIL_390_112_2_17_23_43_45_3584,"tcpip: UPM event - UPM_PLUGOUT is detected!")
TRACE_MSG(UARTUTIL_433_112_2_17_23_43_45_3585,"ppp uart: uartrcv_addq() - invalid input!")
TRACE_MSG(UARTUTIL_441_112_2_17_23_43_45_3586,"ppp uart: uart_data_add() - alloc UPACKET failed! ")
TRACE_MSG(UARTUTIL_449_112_2_17_23_43_45_3587,"ppp uart: uart_data_add() - alloc u_data failed! ")
TRACE_MSG(UARTUTIL_487_112_2_17_23_43_45_3588,"ppp uart warning: invalid upkt!")
TRACE_MSG(UARTUTIL_510_112_2_17_23_43_45_3589,"ppp uart: invalid pppif(%p) or data_ptr(%p) of upkt %p!")
TRACE_MSG(XIP_SEC_93_112_2_17_23_43_45_3590,"tcpip xip: deregister encrypt func pointer!")
TRACE_MSG(XIP_SEC_100_112_2_17_23_43_45_3591,"tcpip xip: update encrypt func - old(%p), new(%p)")
TRACE_MSG(XIP_SEC_104_112_2_17_23_43_45_3592,"tcpip xip: register encrypt func - %p")
TRACE_MSG(XIP_SEC_121_112_2_17_23_43_46_3593,"tcpip xip: TCPIP_RegIpUpFunc() - input is NULL!")
TRACE_MSG(XIP_SEC_140_112_2_17_23_43_46_3594,"tcpip xip: TCPIP_RegIpDownFunc() - input is NULL!")
TRACE_MSG(XIP_SEC_160_112_2_17_23_43_46_3595,"tcpip xip: XIP_SendToEncrypt() - input pkt is NULL!")
TRACE_MSG(XIP_SEC_172_112_2_17_23_43_46_3596,"tcpip xip: allocate XIPPACKET for UL packet %x failed!")
TRACE_MSG(XIP_SEC_213_112_2_17_23_43_46_3597,"tcpip xip: packet encrypt queue is NULL!")
TRACE_MSG(XIP_SEC_250_112_2_17_23_43_46_3598,"tcpip xip: to send encrypted pkt %x")
TRACE_MSG(XIP_SEC_270_112_2_17_23_43_46_3599,"tcpip xip: to send encrypted pkt %x ( initial %x )")
TRACE_MSG(XIP_SEC_276_112_2_17_23_43_46_3600,"tcpip xip: encrpyted pk_cpalloc() failed!")
TRACE_MSG(XIP_SEC_282_112_2_17_23_43_46_3601,"tcpip xip: encrypted data error, pkt(%p)->nb_prot(%p), ptr(%p), len(%d)")
TRACE_MSG(XIP_SEC_300_112_2_17_23_43_46_3602,"tcpip xip: pkt(%p, ID:%x) not match with encrypted data (ID:%x), drop(%d)")
TRACE_MSG(XIP_SEC_324_112_2_17_23_43_46_3603,"tcpip xip: XIP_RecvToDecrypt() - input pkt is NULL!")
TRACE_MSG(XIP_SEC_336_112_2_17_23_43_46_3604,"tcpip xip: allocate XIPPACKET for DL packet %x failed!")
TRACE_MSG(XIP_SEC_381_112_2_17_23_43_46_3605,"tcpip xip: packet decrypt queue is NULL!")
TRACE_MSG(XIP_SEC_452_112_2_17_23_43_46_3606,"tcpip xip: decrpyted pk_cpalloc() failed!")
TRACE_MSG(XIP_SEC_458_112_2_17_23_43_46_3607,"tcpip xip: decrypted data error, pkt(%p)->nb_prot(%p), ptr(%p), len(%d)")
TRACE_MSG(XIP_SEC_477_112_2_17_23_43_46_3608,"tcpip xip: pkt(%p, ID:%x) not match with decrypted data (ID:%x), drop(%d)")
TRACE_MSG(XIP_SEC_551_112_2_17_23_43_47_3609,"tcpip xip: XIPTEST_DoEncrypt() - alloc buffer failed!")
TRACE_MSG(XIP_SEC_564_112_2_17_23_43_47_3610,"tcpip xip: XIPTEST_DoEncrypt() - invalid data_ptr(%p), data_len(%d) or dataup_func(%p)")
TRACE_MSG(XIP_SEC_572_112_2_17_23_43_47_3611,"tcpip xip: XIPTEST_DoEncrypt() - invalid xipdata_ptr(%p)!")
TRACE_MSG(XIP_SEC_603_112_2_17_23_43_47_3612,"tcpip xip: XIPTEST_DoDecrypt() - alloc buffer failed!")
TRACE_MSG(XIP_SEC_616_112_2_17_23_43_47_3613,"tcpip xip: XIPTEST_DoEncrypt() - invalid data_ptr(%p), data_len(%d) or datadown_func(%p)")
TRACE_MSG(XIP_SEC_624_112_2_17_23_43_47_3614,"tcpip xip: XIPTEST_DoDecrypt() - invalid xipdata_ptr(%p)!")
TRACE_MSG(XIP_SEC_648_112_2_17_23_43_47_3615,"tcpip xip: XIPTEST_AddToQueue() - invalid data_ptr(%p) or data_len(%d)")
TRACE_MSG(XIP_SEC_667_112_2_17_23_43_47_3616,"tcpip xip: allocate XIPDATA with data ID %x failed!")
TRACE_MSG(ET_ARP_142_112_2_17_23_43_49_3617,"tcpip: et_send() - no send pointer for pkt %p")
TRACE_MSG(ET_ARP_320_112_2_17_23_43_49_3618,"tcpip: make_arp_entry - free pending pkt %p on arptabent %p")
TRACE_MSG(ET_ARP_412_112_2_17_23_43_50_3619,"tcpip: send_arp() - add pkt %p on arptabent %p")
TRACE_MSG(ET_ARP_489_112_2_17_23_43_50_3620,"tcpip: send_arp() - no send pointer for pkt %p")
TRACE_MSG(ET_ARP_519_112_2_17_23_43_50_3621,"tcpip: send_arp() - no send pointer for pkt %p")
TRACE_MSG(ET_ARP_554_112_2_17_23_43_50_3622,"tcpip: arpReply() - pk_alloc for pkt %x failed")
TRACE_MSG(ET_ARP_625_112_2_17_23_43_50_3623,"tcpip: arpReply() - no send pointer for pkt %p")
TRACE_MSG(ET_ARP_711_112_2_17_23_43_50_3624,"tcpip debug: IP conflict with MAC %02X:%02X:%02X:%02X:%02X:%02X")
TRACE_MSG(ET_ARP_854_112_2_17_23_43_51_3625,"tcpip: send_via_arp() - drop pkt %x for pending time-out")
TRACE_MSG(ET_ARP_860_112_2_17_23_43_51_3626,"tcpip: send_via_arp() - drop pkt %x for pending time-out")
TRACE_MSG(ET_ARP_891_112_2_17_23_43_51_3627,"tcpip: send_via_arp() - drop pkt %x for no more pending")
TRACE_MSG(ICMP_190_112_2_17_23_43_51_3628,"tcpip debug: icmprcv - cancel redirect in add_route for dest: %08x, hop1: %08x")
TRACE_MSG(IFACE_658_112_2_17_23_43_53_3629,"tcpip error: if_getbynum - no netif found!")
TRACE_MSG(IP_257_112_2_17_23_43_54_3630,"tcpip: ip_write_internal - pkt:%x, len:%u to %u.%u.%u.%u, can't route")
TRACE_MSG(IP_302_112_2_17_23_43_55_3631,"tcpip: pkt %x on netif %x has no NF_NBPORT")
TRACE_MSG(IP_335_112_2_17_23_43_55_3632,"tcpip: no fragment, pkt %x len %d > mtu %d on netif %x")
TRACE_MSG(IP_499_112_2_17_23_43_55_3633,"tcpip: ip_fragment - small max ip size %d on netif %x for pkt %x")
TRACE_MSG(IP_550_112_2_17_23_43_55_3634,"tcpip: ip_fragment() - pk_alloc() failed for pkt 0x%x")
TRACE_MSG(IP_697_112_2_17_23_43_55_3635,"tcpip: iproute() - invalid host or hop1!")
TRACE_MSG(IP_761_112_2_17_23_43_55_3636,"tcpip: iproute() - add_route() failed!")
TRACE_MSG(IP_798_112_2_17_23_43_56_3637,"[ TCPIPINFO ] Link-layer is already released, but APP still try to use the network.")
TRACE_MSG(IP_802_112_2_17_23_43_56_3638,"tcpip: iproute() - no route found for host %08x, netif %p!")
TRACE_MSG(IP_REASM_125_112_2_17_23_43_56_3639,"tcpip: ip_reassm() - req len %d > buff size %d, drop pkt %x")
TRACE_MSG(IP_REASM_142_112_2_17_23_43_56_3640,"tcpip: ip_reassm() - no fragment slots, drop pkt %x")
TRACE_MSG(IP_REASM_160_112_2_17_23_43_56_3641,"tcpip: ip_reassm() - alloc big buff fail, drop pkt %x")
TRACE_MSG(IP_REASM_221_112_2_17_23_43_56_3642,"tcpip: ip_reassm() - no fragment slots, drop pkt %x")
TRACE_MSG(IP_REASM_253_112_2_17_23_43_56_3643,"tcpip: ip_reassm() - fragment reassemble logic error, drop %x")
TRACE_MSG(IPCONFLICT_CHK_82_112_2_17_23_43_57_3644,"tcpip: sci_ipconflict_reg - AddNodeByTask failed on netif %x")
TRACE_MSG(IPCONFLICT_CHK_91_112_2_17_23_43_57_3645,"tcpip: sci_ipconflict_reg - handle %x on netif %x")
TRACE_MSG(IPCONFLICT_CHK_95_112_2_17_23_43_57_3646,"tcpip: sci_ipconflict_reg - no ip conflict notify!")
TRACE_MSG(IPCONFLICT_CHK_108_112_2_17_23_43_57_3647,"tcpip: sci_ipconflict_dereg - handle %x")
TRACE_MSG(IPCONFLICT_CHK_124_112_2_17_23_43_57_3648,"tcpip: sci_ipconflict_dereg - no ip conflict notify!")
TRACE_MSG(IPCONFLICT_CHK_152_112_2_17_23_43_57_3649,"tcpip debug: IPCONFLICT_Notify - less than %d seconds, do not notify.")
TRACE_MSG(IPCONFLICT_CHK_167_112_2_17_23_43_57_3650,"tcpip debug: alloc signal memory failed for IP conflict on netif %x")
TRACE_MSG(IPCONFLICT_CHK_178_112_2_17_23_43_57_3651,"tcpip debug: send IP_CONFLICT event to task %x, handle %x, netif %x")
TRACE_MSG(IPDEMUX_113_112_2_17_23_43_58_3652,"tcpip: ip_rcv() - BAD_IP_HEAD(%d): bad pkt len, pkt = %08xn")
TRACE_MSG(IPDEMUX_138_112_2_17_23_43_58_3653,"tcpip: ip_rcv() - BAD_IP_HEAD(%d): bad version num, pkt = %08xn")
TRACE_MSG(IPDEMUX_152_112_2_17_23_43_58_3654,"tcpip: ip_rcv() - BAD_IP_HEAD(%d): bad chksum, pkt %x")
TRACE_MSG(IPDEMUX_220_112_2_17_23_43_58_3655,"tcpip: ip_rcv - pkt %x len %d > mtu %d on netif %x")
TRACE_MSG(IPDEMUX_375_112_2_17_23_43_58_3656,"tcpip: pkt %x should be fragmentated, pktlen(%d) > mtu(%d)")
TRACE_MSG(IPDEMUX_395_112_2_17_23_43_58_3657,"tcpip: ip_rcv - pkt %x len %d > mtu %d on netif %x")
TRACE_MSG(IPNET_228_112_2_17_23_43_59_3658,"tcpip: fixup_subnet_mask - IP address 0x%x is bad")
TRACE_MSG(IPNET_520_112_2_17_23_44_0_3659,"tcpip: modem mode not supported, free pkt %x")
TRACE_MSG(IPNET_673_112_2_17_23_44_0_3660,"tcpip: ip2mac() - ifAdminStatus = NI_DOWN, drop pkt %x")
TRACE_MSG(IPNET_715_112_2_17_23_44_0_3661,"tcpip: ip2mac() - no send pointer for pkt %x")
TRACE_MSG(IPNET_741_112_2_17_23_44_0_3662,"tcpip error: ip2mac - unknown interface type for pkt %x")
TRACE_MSG(IPNET_785_112_2_17_23_44_0_3663,"tcpip: found matched pkt %x in tx queue, drop pkt %x")
TRACE_MSG(IPNET_804_112_2_17_23_44_0_3664,"tcpip: send pending pkt %x on netif %x")
TRACE_MSG(IPNET_815_112_2_17_23_44_0_3665,"tcpip: remove pkt %x from tx queue on netif %x")
TRACE_MSG(IPNET_827_112_2_17_23_44_1_3666,"tcpip: add pkt %x to tx queue on netif %x, qlen = %d")
TRACE_MSG(IPNET_832_112_2_17_23_44_1_3667,"tcpip: PacketSend() - drop pkt %x for error %d")
TRACE_MSG(IPNET_880_112_2_17_23_44_1_3668,"tcpip: cap output not support packet type %d")
TRACE_MSG(IPNET_916_112_2_17_23_44_1_3669,"tcpip: TxToBearer() - async tx pkt %x fail, error %d")
TRACE_MSG(IPNET_944_112_2_17_23_44_1_3670,"tcpip: TxToBearer() - sync tx pkt %x fail, error %d")
TRACE_MSG(IPNET_960_112_2_17_23_44_1_3671,"tcpip: TxToBearer() - drop pkt %x for error %d")
TRACE_MSG(IPNET_1019_112_2_17_23_44_1_3672,"tcpip: GetSubnetMask() - not ABC-class IP 0x%08x ")
TRACE_MSG(IPROUTE_138_112_2_17_23_44_2_3673,"tcpip add_route: rejected null parm, dest: %08x, mask: %08x")
TRACE_MSG(IPSTART_293_112_2_17_23_44_4_3674,"tcpip: bad netif ptr %p")
TRACE_MSG(APP_PING_120_112_2_17_23_44_8_3675,"tcpip: sci_ping_request - no ping supported")
TRACE_MSG(APP_PING_138_112_2_17_23_44_8_3676,"tcpip: sci_ping_cancel() - cancel ping %x")
TRACE_MSG(APP_PING_182_112_2_17_23_44_8_3677,"tcpip: sci_async_ping - no ping supported")
TRACE_MSG(APP_PING_222_112_2_17_23_44_8_3678,"tcpip: ping_request - invalid input faddr!")
TRACE_MSG(APP_PING_230_112_2_17_23_44_8_3679,"tcpip: ping_request - address len should < %d")
TRACE_MSG(APP_PING_240_112_2_17_23_44_8_3680,"tcpip: ping_request - no ping callback set")
TRACE_MSG(APP_PING_250_112_2_17_23_44_8_3681,"tcpip: ping_request - no ping message task set")
TRACE_MSG(APP_PING_259_112_2_17_23_44_8_3682,"tcpip: ping_request - unsupport ping type %d")
TRACE_MSG(APP_PING_268_112_2_17_23_44_8_3683,"tcpip: ping_request - no extern netif for netid %x")
TRACE_MSG(APP_PING_276_112_2_17_23_44_8_3684,"tcpip: ping_request - create ping handle failed")
TRACE_MSG(APP_PING_284_112_2_17_23_44_8_3685,"tcpip: ping_request - mem alloc failed!")
TRACE_MSG(APP_PING_325_112_2_17_23_44_8_3686,"tcpip: ping_request - handle(%x), addr(%s), tmout(%dms), netif(%x), type(%s)")
TRACE_MSG(APP_PING_336_112_2_17_23_44_8_3687,"tcpip: ping_request - resolve ping host ")
TRACE_MSG(APP_PING_356_112_2_17_23_44_8_3688,"tcpip: ping req error - ping_send() error %d")
TRACE_MSG(APP_PING_449_112_2_17_23_44_9_3689,"tcpip: ping_result_notify - res(%d), time_delay(%dms), handle(%x)")
TRACE_MSG(APP_PING_457_112_2_17_23_44_9_3690,"tcpip: ping_result_notify - no callback")
TRACE_MSG(APP_PING_472_112_2_17_23_44_9_3691,"tcpip: ping_result_notify - unsupport notify type %d")
TRACE_MSG(APP_PING_530_112_2_17_23_44_9_3692,"tcpip: delete ping %p from pingq error - %d, handle %x")
TRACE_MSG(APP_PING_673_112_2_17_23_44_9_3693,"tcpip: ping_send() - icmpEcho error %d")
TRACE_MSG(APP_PING_836_112_2_17_23_44_9_3694,"tcpip: ping_check() - resolve ping host ")
TRACE_MSG(APP_PING_882_112_2_17_23_44_10_3695,"tcpip: ping_check() - ping handle %x, state %d, do nothing")
TRACE_MSG(APP_TCP_IF_202_112_2_17_23_44_10_3696,"tcpip: sci_gethostbyname_ext() - input NULL!")
TRACE_MSG(APP_TCP_IF_207_112_2_17_23_44_10_3697,"tcpip: sci_gethostbyname_ext() - url \"%s\", netid %x")
TRACE_MSG(APP_TCP_IF_284_112_2_17_23_44_10_3698,"tcpip: sci_parse_host_ext() - url \"%s\", netid %x")
TRACE_MSG(APP_TCP_IF_390_112_2_17_23_44_11_3699,"tcpip: sci_udp_send_ext(netid %x) error - %d")
TRACE_MSG(APP_TCP_IF_461_112_2_17_23_44_11_3700,"tcpip: sci_udp_open_ext() - invalid input: handler, lport or label!")
TRACE_MSG(APP_TCP_IF_957_112_2_17_23_44_12_3701,"tcpip: app_recv_upcall() - udp data not for us!")
TRACE_MSG(APP_TCP_IF_963_112_2_17_23_44_12_3702,"tcpip: app_recv_upcall() - udp data, pkt = %p, len = %d")
TRACE_MSG(ASYNC_GETHOST_101_112_2_17_23_44_12_3703,"tcpip: sci_async_gethostbyname - invalid input!")
TRACE_MSG(ASYNC_GETHOST_109_112_2_17_23_44_12_3704,"tcpip: sci_async_gethostbyname - no netif found!")
TRACE_MSG(ASYNC_GETHOST_117_112_2_17_23_44_12_3705,"tcpip: should use sci_gethostaddress_ext() get local address")
TRACE_MSG(ASYNC_GETHOST_126_112_2_17_23_44_12_3706,"tcpip: sci_async_gethostbyname - alloc memory failed!")
TRACE_MSG(ASYNC_GETHOST_172_112_2_17_23_44_12_3707,"tcpip: sci_async_gethostbyname - url(%s), timeout(%d ms), netid(%x)")
TRACE_MSG(ASYNC_GETHOST_177_112_2_17_23_44_12_3708,"tcpip: sci_async_gethostbyname - not support!")
TRACE_MSG(ASYNC_GETHOST_222_112_2_17_23_44_12_3709,"tcpip: gethostbyname_check - success for URL(%s), addr(%s)")
TRACE_MSG(ASYNC_GETHOST_233_112_2_17_23_44_12_3710,"tcpip: gethostbyname_check - error %d for URL(%s)")
TRACE_MSG(ASYNC_GETHOST_244_112_2_17_23_44_12_3711,"tcpip: gethostbyname_check - timeout for URL(%s)")
TRACE_MSG(ASYNC_GETHOST_340_112_2_17_23_44_13_3712,"tcpip: gethostbyname_bynetid - invalid netid %x")
TRACE_MSG(ASYNC_GETHOST_394_112_2_17_23_44_13_3713,"tcpip: gethostbyname_informevent - invalid task")
TRACE_MSG(ASYNC_GETHOST_426_112_2_17_23_44_13_3714,"tcpip: gethostbyname_informevent - alloc signal failed")
TRACE_MSG(ASYNC_SOCK_243_112_2_17_23_44_14_3715,"tcpip: asyncselect_informevent - no mem for READ event")
TRACE_MSG(ASYNC_SOCK_265_112_2_17_23_44_14_3716,"tcpip: asyncselect_informevent - no mem for READ_BUFFER event")
TRACE_MSG(ASYNC_SOCK_274_112_2_17_23_44_14_3717,"tcpip debug: send READ_BUFFER_STATUS event for so %x to task %x")
TRACE_MSG(ASYNC_SOCK_287_112_2_17_23_44_14_3718,"tcpip: asyncselect_informevent - no mem for WRITE event")
TRACE_MSG(ASYNC_SOCK_308_112_2_17_23_44_14_3719,"tcpip: asyncselect_informevent - no mem for CONNECT event")
TRACE_MSG(ASYNC_SOCK_318_112_2_17_23_44_14_3720,"tcpip debug: send CONNECT event for so %x to task %x, error %d")
TRACE_MSG(ASYNC_SOCK_329_112_2_17_23_44_14_3721,"tcpip: asyncselect_informevent - no mem for CLOSE event")
TRACE_MSG(ASYNC_SOCK_339_112_2_17_23_44_14_3722,"tcpip debug: send CLOSE event for so %x to task %x, error %d")
TRACE_MSG(ASYNC_SOCK_351_112_2_17_23_44_14_3723,"tcpip: asyncselect_informevent - no mem for ACCEPT event")
TRACE_MSG(ASYNC_SOCK_360_112_2_17_23_44_14_3724,"tcpip debug: send ACCEPT event for so %x to task %x")
TRACE_MSG(ASYNC_SOCK_371_112_2_17_23_44_14_3725,"tcpip: asyncselect_informevent - no mem for FULL_CLOSE event")
TRACE_MSG(ASYNC_SOCK_381_112_2_17_23_44_14_3726,"tcpip: send FULL_CLOSED event (%d) for so %x to task %x, reason = %d")
TRACE_MSG(ASYNC_SOCK_415_112_2_17_23_44_14_3727,"tcpip: ping_inform_event - alloc signal failed")
TRACE_MSG(ASYNC_SOCK_428_112_2_17_23_44_14_3728,"tcpip debug: ping message for handle %x, res %d, delay %dms")
TRACE_MSG(ASYNC_SOCK_456_112_2_17_23_44_14_3729,"tcpip: dhcp_inform_event - alloc signal failed")
TRACE_MSG(ASYNC_SOCK_474_112_2_17_23_44_14_3730,"tcpip debug: send dhcp message to %x on netid %x, res %d")
TRACE_MSG(ASYNC_SOCK_511_112_2_17_23_44_14_3731,"tcpip: t_asyncselect() - input event is AS_NULL")
TRACE_MSG(DHCSETUP_68_112_2_17_23_44_16_3732,"tcpip: sci_async_dhcp - invalid task ID!")
TRACE_MSG(DHCSETUP_74_112_2_17_23_44_16_3733,"tcpip: sci_async_dhcp - no DHCP CLIENT supported!")
TRACE_MSG(DHCSETUP_99_112_2_17_23_44_16_3734,"tcpip: sci_dhcp_request - invalid callback pointer!")
TRACE_MSG(DHCSETUP_105_112_2_17_23_44_16_3735,"tcpip: sci_dhcp_request - no DHCP CLIENT supported!")
TRACE_MSG(DHCSETUP_125_112_2_17_23_44_16_3736,"tcpip: sci_dhcp_cancel - no matched netif!")
TRACE_MSG(DHCSETUP_133_112_2_17_23_44_16_3737,"tcpip: sci_dhcp_cancel - invalid netif index!")
TRACE_MSG(DHCSETUP_141_112_2_17_23_44_16_3738,"tcpip: cancel dhcp request on netif %x")
TRACE_MSG(DHCSETUP_171_112_2_17_23_44_16_3739,"tcpip: dhcp_request - no matched netif!")
TRACE_MSG(DHCSETUP_179_112_2_17_23_44_16_3740,"tcpip: dhcp_request - invalid netif index!")
TRACE_MSG(DHCSETUP_186_112_2_17_23_44_16_3741,"tcpip: dhcp_request - invalid hard address ptr or len %d!")
TRACE_MSG(DHCSETUP_195_112_2_17_23_44_16_3742,"tcpip: dhcp_request - dhc_init_iface() error %d")
TRACE_MSG(DHCSETUP_217_112_2_17_23_44_16_3743,"tcpip: dhcp_request - invalid dhcp type %d")
TRACE_MSG(DHCSETUP_241_112_2_17_23_44_16_3744,"tcpip: dhcp_request - dhc_second() error %d")
TRACE_MSG(DHCSETUP_248_112_2_17_23_44_16_3745,"tcpip: start DHCP request on netif %x")
TRACE_MSG(NETMAIN_352_112_2_17_23_44_25_3746,"tcpip: stop TCPIP timer!")
TRACE_MSG(NETMAIN_423_112_2_17_23_44_25_3747,"Spreadtrum TCP Echo Testing ......\n")
TRACE_MSG(RESHOST_128_112_2_17_23_44_39_3748,"tcpip: in_reshost() - no DNS on netif %x")
TRACE_MSG(TCPIP_DEBUG_284_112_2_17_23_44_46_3749,"tcpip debug: print pkt %p, len = %d")
TRACE_MSG(TCPIP_DEBUG_294_112_2_17_23_44_46_3750," - pktdata %d%d) %s")
TRACE_MSG(TCPIP_DEBUG_302_112_2_17_23_44_46_3751," - pktdata %d%d) %s")
TRACE_MSG(TCPIP_DEBUG_316_112_2_17_23_44_46_3752,"tcpip: nofreed soq_len = %d ")
TRACE_MSG(TCPIP_DEBUG_332_112_2_17_23_44_46_3753," - tcpip: so(%d) : 0x%x, tot_snd(%d), tot_rcv(%d), %s")
TRACE_MSG(TCPIP_DEBUG_339_112_2_17_23_44_46_3754," - tcpip: so(%d) : 0x%x, tot_snd(%d), tot_rcv(%d), UDP")
TRACE_MSG(TCPIP_DEBUG_344_112_2_17_23_44_46_3755," - tcpip: so(%d) : 0x%x, tot_snd(%d), tot_rcv(%d), %d")
TRACE_MSG(TCPIP_DEBUG_353_112_2_17_23_44_46_3756," - tcpip: - RCV pkt(%x): %p( %d, %d ), mbuf(%p): %s")
TRACE_MSG(TCPIP_DEBUG_363_112_2_17_23_44_46_3757," - tcpip: - SND pkt(%x): %p( %d, %d ), mbuf(%p): %s")
TRACE_MSG(TCPIP_DEBUG_368_112_2_17_23_44_46_3758,"tcpip: bigfreeq = %d, lilfreeq = %d")
TRACE_MSG(TCPIP_DEBUG_387_112_2_17_23_44_46_3759,"tcpip: inet_aton(), %s -> 0x%08x")
TRACE_MSG(TCPIP_DEBUG_391_112_2_17_23_44_46_3760,"tcpip: inet_ntoa(), %s <- 0x%08x")
TRACE_MSG(TCPIP_DEBUG_412_112_2_17_23_44_46_3761,"tcpip: no in-used net interface!")
TRACE_MSG(TCPIP_DEBUG_420_112_2_17_23_44_46_3762,"tcpip: current in-used netif num(%d)")
TRACE_MSG(TCPIP_DEBUG_427_112_2_17_23_44_46_3763," - tcpip: index(%d), iface(%d), netid(%x), desc(%s), pkttype(%s)")
TRACE_MSG(TCPIP_DEBUG_428_112_2_17_23_44_46_3764," - tcpip:  ip address   - %s")
TRACE_MSG(TCPIP_DEBUG_429_112_2_17_23_44_46_3765," - tcpip:  subnet mask  - %s")
TRACE_MSG(TCPIP_DEBUG_430_112_2_17_23_44_46_3766," - tcpip:  gateway      - %s")
TRACE_MSG(TCPIP_DEBUG_432_112_2_17_23_44_46_3767," - tcpip:  dns pri/sec  - %s, %s")
TRACE_MSG(TCPIP_DEBUG_437_112_2_17_23_44_46_3768," - tcpip:  hard addr    - %s")
TRACE_MSG(TCPIP_DEBUG_439_112_2_17_23_44_46_3769," - tcpip:  mtu          - %d Byte")
TRACE_MSG(TCPIP_DEBUG_455_112_2_17_23_44_46_3770,"tcpip: time(%04ds) - bigfreeq = %d, lilfreeq = %d")
TRACE_MSG(TCPIP_DEBUG_487_112_2_17_23_44_47_3771,"tcpip: SimulatePktSend - invalid input")
TRACE_MSG(TCPIP_DEBUG_497_112_2_17_23_44_47_3772,"tcpip: SimulatePktSend - alloc packet buffer failed, len = %d")
TRACE_MSG(TCPIP_DEBUG_517_112_2_17_23_44_47_3773,"tcpip: SimulatePktSend - iproute failed")
TRACE_MSG(TCPIP_DEBUG_557_112_2_17_23_44_47_3774,"tcpip: type %d is not supported for pkt %x")
TRACE_MSG(TCPIP_DEBUG_571_112_2_17_23_44_47_3775,"tcpip: parse pkt(%x), len(%d) failed, print data:")
TRACE_MSG(TCPIP_DEBUG_606_112_2_17_23_44_47_3776,"tcpip: PacketInfoIp() - invalid len %d < iplen %d")
TRACE_MSG(TCPIP_DEBUG_866_112_2_17_23_44_47_3777,"[ TCPIP ] %s, pkt(%x):%x, netif(%x), IPID(%x), %s, %s(%d)")
TRACE_MSG(TCPIP_DEBUG_872_112_2_17_23_44_47_3778,"[ TCPIP ] %s, pkt(%x):%x, netif(%x), IPID(%x), FRAG_OFFSET(%d), %s(%d)")
TRACE_MSG(TCPIP_DEBUG_889_112_2_17_23_44_48_3779,"[ TCPIP ] %s, pkt(%x):%x, netif(%x), %s")
TRACE_MSG(TCPIP_DEBUG_898_112_2_17_23_44_48_3780,"[ TCPIP ] %s, pkt(%x):%x, netif(%x), %s")
TRACE_MSG(TCPIP_DEBUG_905_112_2_17_23_44_48_3781,"[ TCPIP ] %s, pkt(%x):%x, netif(%x), UNKNOWN TYPE(%d)")
TRACE_MSG(CHAP_128_112_2_17_23_44_54_3782,"ppp ChapAuthWithPeer: link:%lx, our_name:%s, digest(type):%d")
TRACE_MSG(CHAP_218_112_2_17_23_44_55_3783,"ppp Peer failed to respond to CHAP challenge")
TRACE_MSG(CHAP_396_112_2_17_23_44_55_3784,"ppp ChapInput: rcvd short header.")
TRACE_MSG(CHAP_405_112_2_17_23_44_55_3785,"ppp ChapInput: rcvd illegal length.")
TRACE_MSG(CHAP_410_112_2_17_23_44_55_3786,"ppp ChapInput: rcvd short packet.")
TRACE_MSG(CHAP_433_112_2_17_23_44_55_3787,"ppp Unknown CHAP code (%d) received.")
TRACE_MSG(CHAP_468_112_2_17_23_44_55_3788,"ppp ChapReceiveChallenge: link %lx Rcvd id %d.")
TRACE_MSG(CHAP_474_112_2_17_23_44_55_3789,"ppp ChapReceiveChallenge: in state %d")
TRACE_MSG(CHAP_480_112_2_17_23_44_55_3790,"ppp ChapReceiveChallenge: rcvd short packet.")
TRACE_MSG(CHAP_488_112_2_17_23_44_55_3791,"ppp ChapReceiveChallenge: rcvd short packet.")
TRACE_MSG(CHAP_500_112_2_17_23_44_55_3792,"ppp ChapReceiveChallenge: received name field: %s")
TRACE_MSG(CHAP_508_112_2_17_23_44_55_3793,"ppp No CHAP secret found for authenticating us to %s")
TRACE_MSG(CHAP_524_112_2_17_23_44_55_3794,"ppp MD5ing; id:%d, secret:%s, chal_len:%un")
TRACE_MSG(CHAP_547_112_2_17_23_44_55_3795,"ppp MS-CHAPping; id:%d, secret:%s, chal_len:%un")
TRACE_MSG(CHAP_555_112_2_17_23_44_55_3796,"ppp unknown digest type %d")
TRACE_MSG(CHAP_590_112_2_17_23_44_55_3797,"ppp ChapReceiveResponse: link: %lx Rcvd id %d.")
TRACE_MSG(CHAP_596_112_2_17_23_44_55_3798,"ppp ChapReceiveResponse: in state %d")
TRACE_MSG(CHAP_621_112_2_17_23_44_55_3799,"ppp ChapReceiveResponse: rcvd short packet.")
TRACE_MSG(CHAP_631_112_2_17_23_44_55_3800,"ppp ChapReceiveResponse: rcvd short packet.")
TRACE_MSG(CHAP_643_112_2_17_23_44_55_3801,"ppp ChapReceiveResponse: received name field: %s")
TRACE_MSG(CHAP_654_112_2_17_23_44_55_3802,"ppp No CHAP secret found for authenticating %s")
TRACE_MSG(CHAP_666_112_2_17_23_44_55_3803,"ppp MD5ing; id:%d, secret:%s, chal_len:%un")
TRACE_MSG(CHAP_722_112_2_17_23_44_56_3804,"ppp MS-CHAP Authentication failure. Allowing retry. New ID is %d\n")
TRACE_MSG(CHAP_742_112_2_17_23_44_56_3805,"ppp unknown digest type %d")
TRACE_MSG(CHAP_768_112_2_17_23_44_56_3806,"ppp CHAP peer authentication failed")
TRACE_MSG(CHAP_789_112_2_17_23_44_56_3807,"ppp ChapReceiveSuccess: link:%x Rcvd id %d.")
TRACE_MSG(CHAP_801_112_2_17_23_44_56_3808,"ppp ChapReceiveSuccess: in state %dn")
TRACE_MSG(CHAP_830_112_2_17_23_44_56_3809,"ppp ChapReceiveFailure: link: %lx, Rcvd id %d.")
TRACE_MSG(CHAP_836_112_2_17_23_44_56_3810,"ppp ChapReceiveFailure: in state %dn")
TRACE_MSG(CHAP_842_112_2_17_23_44_56_3811,"ppp CHAP authentication failed")
TRACE_MSG(CHAP_885_112_2_17_23_44_56_3812,"ppp ChapSendChallenge: link: %lx, Sent id %d.")
TRACE_MSG(CHAP_932_112_2_17_23_44_56_3813,"ppp ChapSendStatus: Sent code %d, id %d.")
TRACE_MSG(IFPPP_212_112_2_17_23_44_57_3814,"ppp: del_ppp() - no mppp %x found")
TRACE_MSG(IFPPP_401_112_2_17_23_44_57_3815,"tcpip: prep_ppp() - new_ppp failed, index %d")
TRACE_MSG(IFPPP_501_112_2_17_23_44_58_3816,"tcpip: ppp_ifopen() error - no ppp interface found")
TRACE_MSG(IFPPP_535_112_2_17_23_44_58_3817,"ppp error: ppp_pkt_send - invalid net %x or n_local %x for pkt %x")
TRACE_MSG(IFPPP_545_112_2_17_23_44_58_3818,"ppp error: ppp_pkt_send - mppp->ifp(%x) != pkt->net(%x) for pkt %x")
TRACE_MSG(IFPPP_566_112_2_17_23_44_58_3819,"ppp: ppp_pkt_send - retry dropped")
TRACE_MSG(IFPPP_585_112_2_17_23_44_58_3820,"tcpip error: no space for MAC addr, nb_prot %x < nb_buff %x for pkt %x")
TRACE_MSG(IFPPP_597_112_2_17_23_44_58_3821,"ppp warning: ppp_pkt_send - IPCP is not OPENED!")
TRACE_MSG(IFPPP_900_112_2_17_23_44_58_3822,"ppp timeout (link %p), trys left: %dn")
TRACE_MSG(IFPPP_1011_112_2_17_23_44_59_3823,"ppp link %p Auth FAILURE: %s failed authentication")
TRACE_MSG(IFPPP_1055_112_2_17_23_44_59_3824,"ppp Updating Src IP\n")
TRACE_MSG(IFPPP_1083_112_2_17_23_44_59_3825,"ppp: ppp_set_ipaddr - end TCP checksum loop")
TRACE_MSG(IPCP_76_112_2_17_23_44_59_3826,"ppp: ipcp_resetci() - ouraddr: %u.%u.%u.%u")
TRACE_MSG(IPCP_99_112_2_17_23_44_59_3827,"ppp Disabling VJ over PPPOE link %lx\n")
TRACE_MSG(IPCP_220_112_2_17_23_44_59_3828,"ppp ipcp_addci: ouraddr==0.0.0.0\n")
TRACE_MSG(IPCP_394_112_2_17_23_45_0_3829,"ppp local IP address %u.%u.%u.%un")
TRACE_MSG(IPCP_400_112_2_17_23_45_0_3830,"ppp remote IP address %u.%u.%u.%un")
TRACE_MSG(IPCP_443_112_2_17_23_45_0_3831,"ppp ipcp rcv NAK: bad VJ length %d\n")
TRACE_MSG(IPCP_474_112_2_17_23_45_0_3832,"ppp ipcp NAK: DNS (0x%x) address %u.%u.%u.%u n")
TRACE_MSG(IPCP_479_112_2_17_23_45_0_3833,"ppp ipcp rcv NAK: unknown option 0x%x\n")
TRACE_MSG(IPCP_488_112_2_17_23_45_0_3834,"ppp ipcp odd NAK len\n")
TRACE_MSG(IPCP_617_112_2_17_23_45_0_3835,"ppp ipcp_rejci: bad Rej pkt len\n")
TRACE_MSG(IPCP_628_112_2_17_23_45_0_3836,"ppp ipcp_rejci: bad Rej pkt at opt 0x%x\n")
TRACE_MSG(IPCP_693_112_2_17_23_45_0_3837,"ppp ipcp_reqci: bad CI length on opt 0x%x")
TRACE_MSG(IPCP_708_112_2_17_23_45_0_3838,"ppp ipcp: received IPADDR - %u.%u.%u.%u")
TRACE_MSG(IPCP_751_112_2_17_23_45_0_3839,"ppp %u.%u.%u.%u")
TRACE_MSG(IPCP_768_112_2_17_23_45_1_3840,"ppp ipcp_reqci: ouraddr==0.0.0.0\n")
TRACE_MSG(IPCP_776_112_2_17_23_45_1_3841," \n")
TRACE_MSG(IPCP_780_112_2_17_23_45_1_3842,"ppp ipcp: received COMPRESSTYPE ")
TRACE_MSG(IPCP_789_112_2_17_23_45_1_3843,"ppp (%d)")
TRACE_MSG(IPCP_834_112_2_17_23_45_1_3844,"ppp ipcp: received DNSADDR(%x) - %u.%u.%u.%u")
TRACE_MSG(IPCP_878_112_2_17_23_45_1_3845,"ppp (%s)\n")
TRACE_MSG(IPCP_945_112_2_17_23_45_1_3846,"ppp ipcp: returning Configure-%s")
TRACE_MSG(IPCP_977_112_2_17_23_45_1_3847,"ppp ipcp: up")
TRACE_MSG(IPCP_985_112_2_17_23_45_1_3848,"ppp Could not determine remote IP address")
TRACE_MSG(IPCP_994_112_2_17_23_45_1_3849,"ppp Peer is not authorized to use remote address %u.%u.%u.%u.")
TRACE_MSG(IPCP_1006_112_2_17_23_45_1_3850,"ppp Could not determine local IP address")
TRACE_MSG(IPCP_1015_112_2_17_23_45_1_3851,"ppp IPCP: local  IP address %u.%u.%u.%u")
TRACE_MSG(IPCP_1018_112_2_17_23_45_1_3852,"ppp IPCP: remote IP address %u.%u.%u.%u")
TRACE_MSG(IPCP_1024_112_2_17_23_45_1_3853,"ppp setaddr failed")
TRACE_MSG(IPCP_1044_112_2_17_23_45_1_3854,"ppp sifup failed")
TRACE_MSG(IPCP_1092_112_2_17_23_45_1_3855,"ppp ipcp: down")
TRACE_MSG(LCP_179_112_2_17_23_45_2_3856,"ppp LCP down.")
TRACE_MSG(LCP_217_112_2_17_23_45_2_3857,"ppp lcp_down: to call mppp->line.ln_disconnect()")
TRACE_MSG(LCP_258_112_2_17_23_45_2_3858,"ppp lcp: Echo-Request, Rcvd id %d")
TRACE_MSG(LCP_484_112_2_17_23_45_3_3859,"ppp: ci too big for buffer\n")
TRACE_MSG(LCP_546_112_2_17_23_45_3_3860,"ppp rcvd NAK; ")
TRACE_MSG(LCP_552_112_2_17_23_45_3_3861,"ppp Bad LCP option 0x%x\n")
TRACE_MSG(LCP_555_112_2_17_23_45_3_3862,"ppp opt:%s ")
TRACE_MSG(LCP_678_112_2_17_23_45_3_3863,"ppp lcp: Bad NAK option 0x%x\n")
TRACE_MSG(LCP_692_112_2_17_23_45_3_3864,"ppp The line appears to be looped back; closing:")
TRACE_MSG(LCP_698_112_2_17_23_45_3_3865,"ppp Got LCP NAK in OPENED state.")
TRACE_MSG(LCP_700_112_2_17_23_45_3_3866,"\n")
TRACE_MSG(LCP_704_112_2_17_23_45_3_3867,"ppp lcp: rcvd bad NAK (option:0x%x)\n")
TRACE_MSG(LCP_739_112_2_17_23_45_3_3868,"ppp lcp reject of 0x%x\n")
TRACE_MSG(LCP_819_112_2_17_23_45_3_3869,"ppp lcp: REG for bad option 0x%x\n")
TRACE_MSG(LCP_832_112_2_17_23_45_3_3870,"ppp lcp_rejci: bad; len %d opt: 0x%x")
TRACE_MSG(LCP_895_112_2_17_23_45_3_3871,"ppp lcp_reqci: bad CI length!")
TRACE_MSG(LCP_907_112_2_17_23_45_3_3872,"ppp lcp_reqci: rcvd MRU")
TRACE_MSG(LCP_914_112_2_17_23_45_3_3873,"ppp (%d)")
TRACE_MSG(LCP_931_112_2_17_23_45_3_3874,"ppp lcp_reqci: rcvd ASYNCMAP")
TRACE_MSG(LCP_938_112_2_17_23_45_3_3875,"ppp (%lx)")
TRACE_MSG(LCP_972_112_2_17_23_45_3_3876,"ppp lcp_reqci: rcvd AUTHTYPE")
TRACE_MSG(LCP_1030_112_2_17_23_45_4_3877,"ppp (%x)")
TRACE_MSG(LCP_1060_112_2_17_23_45_4_3878,"ppp Bad MD5 digest type %x, NAKing\n")
TRACE_MSG(LCP_1082_112_2_17_23_45_4_3879,"ppp lcp_reqci: rcvd QUALITY")
TRACE_MSG(LCP_1093_112_2_17_23_45_4_3880,"ppp (%x %lx)")
TRACE_MSG(LCP_1108_112_2_17_23_45_4_3881,"ppp lcp_reqci: rcvd MAGICNUMBER")
TRACE_MSG(LCP_1115_112_2_17_23_45_4_3882,"ppp (%lx)")
TRACE_MSG(LCP_1134_112_2_17_23_45_4_3883,"ppp lcp_reqci: rcvd PCOMPRESSION")
TRACE_MSG(LCP_1144_112_2_17_23_45_4_3884,"ppp lcp_reqci: rcvd ACCOMPRESSION")
TRACE_MSG(LCP_1159_112_2_17_23_45_4_3885,"ppp lcp_reqci: rcvd MULTILINK_MRU")
TRACE_MSG(LCP_1170_112_2_17_23_45_4_3886,"ppp lcp_reqci: rcvd MULTILINK_SSN")
TRACE_MSG(LCP_1179_112_2_17_23_45_4_3887,"ppp lcp_reqci: rcvd MULTILINK_ENDPT")
TRACE_MSG(LCP_1199_112_2_17_23_45_4_3888,"ppp lcp_reqci: rcvd unknown option %d")
TRACE_MSG(LCP_1205_112_2_17_23_45_4_3889,"ppp (%s)\n")
TRACE_MSG(LCP_1246_112_2_17_23_45_4_3890,"ppp lcp_reqci: returning CONF%s.\n")
TRACE_MSG(LCP_1275_112_2_17_23_45_4_3891,"ppp lcp_finished: to call mppp->line.ln_disconnect()")
TRACE_MSG(LCP_1291_112_2_17_23_45_4_3892,"ppp lcp_starting: to call mppp->line.ln_connect()")
TRACE_MSG(PPP_DHCP_61_112_2_17_23_45_4_3893,"ppp_start_dhcp: ppp_timeout in %d secs\n")
TRACE_MSG(PPP_DHCP_81_112_2_17_23_45_4_3894,"ppp dhcp_timeout: iface %d\n")
TRACE_MSG(PPP_DHCP_124_112_2_17_23_45_4_3895,"DHCP assigned PPP link IP=%u.%u.%u.%un")
TRACE_MSG(PPP_LOOP_172_112_2_17_23_45_5_3896,"ppp lbxover: no server\n")
TRACE_MSG(PPP_LOOP_179_112_2_17_23_45_5_3897,"ppp lbxover: no client index\n")
TRACE_MSG(PPPCHAR_155_112_2_17_23_45_6_3898,"ppp_inchar: no bigger buffer, size %d\n")
TRACE_MSG(PPPCHAR_180_112_2_17_23_45_6_3899,"ppp_inchar: no bigger buffer, size %dn")
TRACE_MSG(PPPCHAR_203_112_2_17_23_45_6_3900,"ppp_inchar: got PPP_FLAG ")
TRACE_MSG(PPPCHAR_225_112_2_17_23_45_6_3901,"ppp_inchar: flushed, ppp:%p, flushed=%dn")
TRACE_MSG(PPPCHAR_238_112_2_17_23_45_6_3902,"ppp_inchar: pkt is null\n")
TRACE_MSG(PPPCHAR_242_112_2_17_23_45_6_3903,"ppp_inchar: bad fcs, ppp:%p, len %d, data:\n")
TRACE_MSG(PPPCHAR_255_112_2_17_23_45_6_3904,"ppp text: %s\n")
TRACE_MSG(PPPCHAR_271_112_2_17_23_45_6_3905,"ppp_inchar: pkt is null\n")
TRACE_MSG(PPPCHAR_280_112_2_17_23_45_6_3906,"ppp_inchar: ppp:%p short pkt (%d)\n")
TRACE_MSG(PPPCHAR_346_112_2_17_23_45_6_3907,"ppp_inchar: pkt is null\n")
TRACE_MSG(PPPCHAR_358_112_2_17_23_45_6_3908,"ppp %p: missing ALLSTATIONS, got 0x%x; flags %xn")
TRACE_MSG(PPPCHAR_370_112_2_17_23_45_6_3909,"ppp %p: missing UI, got 0x%x\n")
TRACE_MSG(PPPCHAR_476_112_2_17_23_45_6_3910,"ppp: send buffer check failed, discard pkt(%p), len = %d")
TRACE_MSG(PPPCHAR_488_112_2_17_23_45_6_3911,"ppp: get uart semaphore error, discard pkt %p!")
TRACE_MSG(PPPCHAR_495_112_2_17_23_45_6_3912,"ppp: uart tx buffer is not enough, discard pkt(%p), len = %d")
TRACE_MSG(PPPCHAR_510_112_2_17_23_45_6_3913,"ppp: ppp_sendchars() - mppp(%x), link_id(%d), pkt(%x), len(%d)")
TRACE_MSG(PPPCHAR_512_112_2_17_23_45_6_3914,"ppp: ppp_sendchars() - pkt(%x), len(%d) ")
TRACE_MSG(PPPCHAR_523_112_2_17_23_45_6_3915,"ppp: ln_putc error %d on link %p\n")
TRACE_MSG(PPPCHAR_598_112_2_17_23_45_7_3916,"ppp fcs not ok ??\n")
TRACE_MSG(PPPCHAR_629_112_2_17_23_45_7_3917,"ppp fcs not ok ??\n")
TRACE_MSG(PPPFSM_273_112_2_17_23_45_7_3918,"ppp link %p rcvd:")
TRACE_MSG(PPPFSM_305_112_2_17_23_45_8_3919,"ppp (%p) pkalloc failed for VJ pkt len %dn")
TRACE_MSG(PPPFSM_326_112_2_17_23_45_8_3920,"ppp (%p) vj_uncompress failed on type %xn")
TRACE_MSG(PPPFSM_367_112_2_17_23_45_8_3921,"ppp_inpkt: modem hung up\n")
TRACE_MSG(PPPFSM_399_112_2_17_23_45_8_3922,"ppp_inpkt: (link %p): unsupported prot %x\n")
TRACE_MSG(PPPFSM_420_112_2_17_23_45_8_3923,"ppp (link %p) truncated packet, lengths: hdr:%d, actual:%d\n")
TRACE_MSG(PPPFSM_429_112_2_17_23_45_8_3924,"ppp_inpkt (link %p); prot: %s, code %s, state %s, len %dn")
TRACE_MSG(PPPFSM_435_112_2_17_23_45_8_3925,"ppp (link %p) got pkt in state %sn")
TRACE_MSG(PPPFSM_491_112_2_17_23_45_8_3926,"ppp (link %p) Prot-Reject for IPCP, closing\n")
TRACE_MSG(PPPFSM_498_112_2_17_23_45_8_3927,"ppp (link %p) Prot-Reject for %x\n")
TRACE_MSG(PPPFSM_535_112_2_17_23_45_8_3928,"ppp rcvd IDENTITY: %s\n")
TRACE_MSG(PPPFSM_545_112_2_17_23_45_8_3929,"ppp (link %p) unknown code: %x")
TRACE_MSG(PPPFSM_612_112_2_17_23_45_8_3930,"ppp fsm_input (link %p): bad state: %d, event:%d.")
TRACE_MSG(PPPFSM_621_112_2_17_23_45_8_3931,"ppp_fsm (link %p): %s state: %s -> %sn")
TRACE_MSG(PPPFSM_786_112_2_17_23_45_9_3932,"ppp_fsm (link %p) sending %s, len:%dn")
TRACE_MSG(PPPFSM_811_112_2_17_23_45_9_3933,"ppp LCP (%p) failed, closing IPCP\n")
TRACE_MSG(PPPFSM_852_112_2_17_23_45_9_3934,"ppp_fsm_init (link %p)\n")
TRACE_MSG(PPPFSM_1002_112_2_17_23_45_9_3935,"ppp allpktsend - DHCP pending?\n")
TRACE_MSG(PPPFSM_1052_112_2_17_23_45_9_3936,"ppp link %p sending:")
TRACE_MSG(PPPFSM_1061_112_2_17_23_45_9_3937,"PPP LOSSY_IO dropping TX (link %p)\n")
TRACE_MSG(PPPFSM_1073_112_2_17_23_45_9_3938,"ppp_allpktsend (%p): lost line\n")
TRACE_MSG(PPPFSM_1157_112_2_17_23_45_9_3939,"ppp lowerup: (link %p) %s\n")
TRACE_MSG(PPPFSM_1235_112_2_17_23_45_9_3940,"ppp lowerdown (link %p): %s\n")
TRACE_MSG(PPPFSM_1251_112_2_17_23_45_9_3941,"ppp open (link %p): %s\n")
TRACE_MSG(PPPFSM_1256_112_2_17_23_45_9_3942,"ppp: ppp_open(), to open IPCP but LCP still not opened!")
TRACE_MSG(PPPFSM_1286_112_2_17_23_45_9_3943,"ppp close (link %p): %s\n")
TRACE_MSG(PPPFSM_1324_112_2_17_23_45_10_3944,"ppp %02x ")
TRACE_MSG(PPPFSM_1325_112_2_17_23_45_10_3945,"\n")
TRACE_MSG(PPPMENU_408_112_2_17_23_45_11_3946,"ppp menu: link up on link %p\n")
TRACE_MSG(PPPMENU_451_112_2_17_23_45_11_3947,"ppp menu: link down on link %p\n")
TRACE_MSG(PPPSYS_297_112_2_17_23_45_13_3948,"ppp: ppp_line_init - want ouraddr: %u.%u.%u.%u")
TRACE_MSG(PPPSYS_794_112_2_17_23_45_14_3949,"ppp: can't hold new addr %08x on mppp(%p), netif(%p)")
TRACE_MSG(UPAP_184_112_2_17_23_45_14_3950,"ppp link %lx: No response to PAP authenticate-requests")
TRACE_MSG(UPAP_293_112_2_17_23_45_15_3951,"ppp PAP authentication failed due to protocol-reject")
TRACE_MSG(UPAP_298_112_2_17_23_45_15_3952,"ppp PAP authentication of peer failed (protocol-reject)")
TRACE_MSG(UPAP_327_112_2_17_23_45_15_3953,"ppp upap_input: rcvd short header.")
TRACE_MSG(UPAP_336_112_2_17_23_45_15_3954,"ppp upap_input link %lx: runt, len: %d.")
TRACE_MSG(UPAP_340_112_2_17_23_45_15_3955,"ppp upap_input link %lx: code: %s, id %d, len %dn")
TRACE_MSG(UPAP_346_112_2_17_23_45_15_3956,"ppp upap_input: unknown code %d\n")
TRACE_MSG(UPAP_411_112_2_17_23_45_15_3957,"ppp upap_rauth: rcvd short packet.")
TRACE_MSG(UPAP_420_112_2_17_23_45_15_3958,"ppp upap_rauth: rcvd short packet.")
TRACE_MSG(UPAP_428_112_2_17_23_45_15_3959,"ppp upap_rauth: rcvd short packet.")
TRACE_MSG(UPAP_476_112_2_17_23_45_15_3960,"ppp upap_rauthack: rx pkt in state %d\n")
TRACE_MSG(UPAP_486_112_2_17_23_45_15_3961,"ppp upap_rauthack: runt, len: %d.")
TRACE_MSG(UPAP_530_112_2_17_23_45_15_3962,"ppp upap_rauthnak: runt, len %d\n")
TRACE_MSG(UPAP_536_112_2_17_23_45_15_3963,"ppp Link %lx: PAP auth failed\n")
TRACE_MSG(UPAP_578_112_2_17_23_45_15_3964,"ppp upap_sauth: Sent id %d.")
TRACE_MSG(UPAP_624_112_2_17_23_45_15_3965,"ppp upap_sresp: Sent code %d, id %d.")
TRACE_MSG(DHCPCLNT_149_112_2_17_23_45_17_3966,"tcpip: dcp_init_iface - invalid iface %d")
TRACE_MSG(DHCPCLNT_161_112_2_17_23_45_17_3967,"tcpip: dcp_init_iface - udp_open error on iface %d")
TRACE_MSG(DHCPCLNT_204_112_2_17_23_45_18_3968,"tcpip: dhc_callback() - invalid iface %d")
TRACE_MSG(DHCPCLNT_213_112_2_17_23_45_18_3969,"tcpip: get IP address from DHCP on netif(%p), desc(%s), pkttype(%d)")
TRACE_MSG(DHCPCLNT_216_112_2_17_23_45_18_3970,"- tcpip: haddr   - %02X:%02X:%02X:%02X:%02X:%02X")
TRACE_MSG(DHCPCLNT_217_112_2_17_23_45_18_3971,"- tcpip: ipaddr  - %s")
TRACE_MSG(DHCPCLNT_218_112_2_17_23_45_18_3972,"- tcpip: snmask  - %s")
TRACE_MSG(DHCPCLNT_219_112_2_17_23_45_18_3973,"- tcpip: gateway - %s")
TRACE_MSG(DHCPCLNT_220_112_2_17_23_45_18_3974,"- tcpip: dnssrv1 - %s")
TRACE_MSG(DHCPCLNT_221_112_2_17_23_45_18_3975,"- tcpip: dnssrv2 - %s")
TRACE_MSG(DHCPCLNT_222_112_2_17_23_45_18_3976,"- tcpip: dhcpsrv - %s")
TRACE_MSG(DHCPCLNT_254_112_2_17_23_45_18_3977,"tcpip: dhc_callback() - wrong notify type!")
TRACE_MSG(DHCPCLNT_260_112_2_17_23_45_18_3978,"tcpip: dhc_callback() - DHCP is already bound, no notify!")
TRACE_MSG(DHCPCLNT_269_112_2_17_23_45_18_3979,"tcpip: DHCP request timeout on netif %x, state %s")
TRACE_MSG(DHCPCLNT_293_112_2_17_23_45_18_3980,"tcpip: dhc_callback() - wrong notify type")
TRACE_MSG(DHCPCLNT_358_112_2_17_23_45_18_3981,"tcpip: dhc_upcall() - DHCP upcall not matched")
TRACE_MSG(DHCPCLNT_366_112_2_17_23_45_18_3982,"tcpip: dhc_upcall() - DHCP is UNUSED on netif %x")
TRACE_MSG(DHCPCLNT_377_112_2_17_23_45_18_3983,"tcpip: dhc_upcall() - invalid DHCP data fields on netif %x")
TRACE_MSG(DHCPCLNT_385_112_2_17_23_45_18_3984,"tcpip: dhc_upcall() - DHCP data on netif %x is not for me")
TRACE_MSG(DHCPCLNT_406_112_2_17_23_45_18_3985,"tcpip: dhc_upcall() - wrong DHCP type %d")
TRACE_MSG(DHCPCLNT_420_112_2_17_23_45_18_3986,"tcpip: dhc_upcall() - wrong DHCP state %s on netif %x")
TRACE_MSG(DHCPCLNT_432_112_2_17_23_45_18_3987,"tcpip: dhcpclnt.c(%d) - dhc_upcall()")
TRACE_MSG(DHCPCLNT_448_112_2_17_23_45_18_3988,"tcpip: dhc_upcall() - dhc_rx_offer() error %d")
TRACE_MSG(DHCPCLNT_471_112_2_17_23_45_18_3989,"tcpip: dhc_upcall() - receive DHCP type %d in DHCS_SELECTING on netif %x")
TRACE_MSG(DHCPCLNT_487_112_2_17_23_45_18_3990,"tcpip: dhcpclnt.c(%d) - dhc_upcall()")
TRACE_MSG(DHCPCLNT_495_112_2_17_23_45_18_3991,"tcpip: dhcpclnt.c(%d) - relay agent isn't match")
TRACE_MSG(DHCPCLNT_509_112_2_17_23_45_18_3992,"tcpip: netif(%x), DHCP renew time - INFINITE, rebind time - INFINITE")
TRACE_MSG(DHCPCLNT_516_112_2_17_23_45_18_3993,"tcpip: netif(%x), DHCP renew time - %ds, rebind time - %ds")
TRACE_MSG(DHCPCLNT_523_112_2_17_23_45_18_3994,"tcpip: dhcpclnt.c(%d) - dhc_upcall()")
TRACE_MSG(DHCPCLNT_550_112_2_17_23_45_18_3995,"tcpip: dhcpclnt.c(%d) - dhc_upcall()")
TRACE_MSG(DHCPCLNT_558_112_2_17_23_45_19_3996,"tcpip: dhc_upcall() - bad DHCP state %s on netif %x")
TRACE_MSG(DHCPCLNT_579_112_2_17_23_45_19_3997,"tcpip: DHCP state on netif %x is %s")
TRACE_MSG(DHCPCLNT_619_112_2_17_23_45_19_3998,"tcpip: dhc_buildheader - illegal netif(%d) type - %d")
TRACE_MSG(DHCPCLNT_629_112_2_17_23_45_19_3999,"tcpip: dhc_buildheader() -dhc_states[iface].rev_dhc_flag check begin...")
TRACE_MSG(DHCPCLNT_633_112_2_17_23_45_19_4000,"tcpip: dhc_buildheader() - use broadcast!")
TRACE_MSG(DHCPCLNT_638_112_2_17_23_45_19_4001,"tcpip: dhc_buildheader() -just use unicast")
TRACE_MSG(DHCPCLNT_642_112_2_17_23_45_19_4002,"tcpip: dhc_buildheader() -dhc_states[iface].rev_dhc_flag= %x")
TRACE_MSG(DHCPCLNT_647_112_2_17_23_45_19_4003,"tcpip: dhc_buildheader() -renewing use unicast")
TRACE_MSG(DHCPCLNT_657_112_2_17_23_45_19_4004,"tcpip: dhc_buildheader() - invalid netif(%d) haddr len %d!")
TRACE_MSG(DHCPCLNT_698_112_2_17_23_45_19_4005,"tcpip: dhc_discover() - udp_alloc failed!")
TRACE_MSG(DHCPCLNT_708_112_2_17_23_45_19_4006,"tcpip: dhc_discover() -xid is %x")
TRACE_MSG(DHCPCLNT_797_112_2_17_23_45_19_4007,"tcpip: dhc_rx_offer() - begin ntohl(bp->xid) =%x,ntohl(dhc_states[iface].xid)=%x")
TRACE_MSG(DHCPCLNT_801_112_2_17_23_45_19_4008,"tcpip: dhc_rx_offer() -ERROR  the xid that replied from the server is larger than the xid that client used to send...")
TRACE_MSG(DHCPCLNT_807_112_2_17_23_45_19_4009,"tcpip: dhc_rx_offer() - received DHCP type invalid %x, xid = %x")
TRACE_MSG(DHCPCLNT_818_112_2_17_23_45_19_4010,"tcpip: dhc_rx_offer() - the sending type is unicast")
TRACE_MSG(DHCPCLNT_823_112_2_17_23_45_19_4011,"tcpip: dhc_rx_offer() - the sending type is broadcast")
TRACE_MSG(DHCPCLNT_828_112_2_17_23_45_19_4012,"tcpip: dhc_rx_offer() - xid match on netif %x, ours %x, recv %x successfully")
TRACE_MSG(DHCPCLNT_833_112_2_17_23_45_19_4013,"tcpip: dhc_rx_offer() - xid not match on netif %x, ours %x, recv %x")
TRACE_MSG(DHCPCLNT_841_112_2_17_23_45_19_4014,"tcpip: dhc_rx_offer() - dhc_extract_opts error on netif %d")
TRACE_MSG(DHCPCLNT_847_112_2_17_23_45_19_4015,"tcpip: dhc_rx_offer() - server does not give our IP address")
TRACE_MSG(DHCPCLNT_913_112_2_17_23_45_19_4016,"tcpip: dhc_request() - no UDP buffer")
TRACE_MSG(DHCPCLNT_923_112_2_17_23_45_19_4017,"tcpip: dhc_request() - xids is %d")
TRACE_MSG(DHCPCLNT_931_112_2_17_23_45_19_4018,"tcpip: dhc_request() - dhc_buildheader() error %d")
TRACE_MSG(DHCPCLNT_1134_112_2_17_23_45_20_4019,"tcpip: fixup_subnet_mask - IP address 0x%x is bad")
TRACE_MSG(DHCPCLNT_1459_112_2_17_23_45_20_4020,"tcpip: dhc_second() - dhc_discover() error %d on state %d")
TRACE_MSG(DHCPCLNT_1471_112_2_17_23_45_20_4021,"tcpip: dhc_second() - dhc_reclaim() error %d on state %d")
TRACE_MSG(DHCPCLNT_1503_112_2_17_23_45_20_4022,"tcpip: dhc_second() - bogus state %d")
TRACE_MSG(DHCPCLNT_1558_112_2_17_23_45_21_4023,"tcpip: dhc_second() - dhc_reclaim() error %d on state %d")
TRACE_MSG(DHCPCLNT_1591_112_2_17_23_45_21_4024,"tcpip: dhc_second() - dhc_request() error %d on state %d")
TRACE_MSG(DHCPCLNT_1623_112_2_17_23_45_21_4025,"tcpip: dhc_halt error - invalid iface!")
TRACE_MSG(DHCPCLNT_1628_112_2_17_23_45_21_4026,"tcpip: deact dhcp client on netif %x")
TRACE_MSG(DHCPCLNT_1657_112_2_17_23_45_21_4027,"tcpip: dhc_reclaim() - ip on netif %x is NULL")
TRACE_MSG(DHCPCLNT_1678_112_2_17_23_45_21_4028,"tcpip: dhc_reclaim() - add_route for server %08x on netif %x failed")
TRACE_MSG(DHCPCLNT_1686_112_2_17_23_45_21_4029,"tcpip: dhc_reclaim() - server IP on netif %x is NULL")
TRACE_MSG(DHCPCLNT_1794_112_2_17_23_45_21_4030,"tcpip: dhc set state %s")
TRACE_MSG(DNSCLNT_175_112_2_17_23_45_22_4031,"tcpip: dnc_sendreq() - no DNS on netif %x")
TRACE_MSG(DNSCLNT_279_112_2_17_23_45_22_4032,"tcpip warning: netif of dns_qs %d is NULL")
TRACE_MSG(DNSCLNT_515_112_2_17_23_45_23_4033,"tcpip: DNS error - %s; flags %x")
TRACE_MSG(DNSCLNT_901_112_2_17_23_45_23_4034,"tcpip: t_gethostbyname p_hostent->h_addr_list is NULL!")
TRACE_MSG(DNSCLNT_920_112_2_17_23_45_23_4035,"tcpip: t_gethostbyname() - no DNS on netif %p")
TRACE_MSG(DNSCLNT_1286_112_2_17_23_45_24_4036,"tcpip: dnsquery addr(0x%x ~ 0x%x), size %d, num %d")
TRACE_MSG(EAPOL_80_112_2_17_23_45_25_4037,"tcpip: sci_eapol_tx() - netif %p should use ETHERNET packet")
TRACE_MSG(EAPOL_109_112_2_17_23_45_25_4038,"tcpip: sci_eapol_tx() - invalid pkt %p, nb_prot %p, nb_buff %p")
TRACE_MSG(EAPOL_131_112_2_17_23_45_25_4039,"tcpip: sci_eapol_tx() - no send ptr for pkt %p")
TRACE_MSG(EAPOL_167_112_2_17_23_45_25_4040,"tcpip: set eapol rx callback ptr %p on netif %p")
TRACE_MSG(EAPOL_207_112_2_17_23_45_25_4041,"tcpip: no eapol rx fptr for pkt %p on netif %p")
TRACE_MSG(NETIF_181_112_2_17_23_45_27_4042,"tcpip: use spare dns on netif %x, dns1(%08x), dns2(%08x)")
TRACE_MSG(NETIF_189_112_2_17_23_45_27_4043,"tcpip: use global spare dns, dns1(%08x), dns2(%08x)")
TRACE_MSG(NETIF_224_112_2_17_23_45_27_4044,"tcpip: use default netif %x")
TRACE_MSG(NETIF_288_112_2_17_23_45_27_4045,"tcpip warning: netif %x with seq %x is not opened")
TRACE_MSG(NETIF_405_112_2_17_23_45_27_4046,"tcpip: get throughput on netif 0x%x, in(%d), out(%d)")
TRACE_MSG(NETIF_427_112_2_17_23_45_27_4047,"tcpip: clear throughput on netif 0x%x")
TRACE_MSG(NETIF_479_112_2_17_23_45_27_4048,"tcpip: set spare dns on netif 0x%x, dns1 %s, dns2 %s")
TRACE_MSG(NETIF_517_112_2_17_23_45_27_4049,"tcpip: set global spare dns, dns1 %s, dns2 %s")
TRACE_MSG(PKTALLOC_201_112_2_17_23_45_29_4050,"tcpip: pk_alloc() fail - buffer wanted oversize, %d > %d")
TRACE_MSG(PKTALLOC_213_112_2_17_23_45_29_4051,"tcpip: pk_alloc() - bigfreeq = %d, lilfreeq = %d")
TRACE_MSG(PKTALLOC_240_112_2_17_23_45_29_4052,"tcpip: pk_alloc() - alloc fail for data len %d")
TRACE_MSG(PKTALLOC_261_112_2_17_23_45_29_4053,"tcpip: pk_alloc() - alloc buffer for pkt %p failed!")
TRACE_MSG(PKTALLOC_329_112_2_17_23_45_29_4054,"tcpip: pk_cpalloc() invalid prot_bias(%x)")
TRACE_MSG(PKTALLOC_338_112_2_17_23_45_29_4055,"tcpip: pk_cpalloc() from pkt %p with new data len %d failed!")
TRACE_MSG(PKTALLOC_449_112_2_17_23_45_29_4056,"tcpip: pk_free - buffer %p already in bigfreeq")
TRACE_MSG(PKTALLOC_458_112_2_17_23_45_29_4057,"tcpip: pk_free - buffer %p already in lilfreeq")
TRACE_MSG(PKTALLOC_536_112_2_17_23_45_29_4058,"tcpip: pkt_big(%d) - %d; pkt_lil(%d) - %d ")
TRACE_MSG(PKTALLOC_543_112_2_17_23_45_29_4059,"tcpip error: packet static buffer is not enough!")
TRACE_MSG(PKTALLOC_549_112_2_17_23_45_29_4060,"tcpip debug: no-used static net buffer size %d")
TRACE_MSG(PKTALLOC_741_112_2_17_23_45_30_4061,"tcpip: netbuff addr(0x%x ~ 0x%x), size %d, bigbufs %d, lilbufs %d")
TRACE_MSG(PKTALLOC_768_112_2_17_23_45_30_4062,"tcpip: netbuff addr 0x%x  was corrupted !")
TRACE_MSG(PKTALLOC_775_112_2_17_23_45_30_4063,"tcpip: netbuff addr 0x%x  was corrupted !")
TRACE_MSG(Q_TXCNF_143_112_2_17_23_45_30_4064,"tcpip warning: no matched info %p found in txcnfq, qlen = %d")
TRACE_MSG(TX_FLOWCTRL_79_112_2_17_23_45_32_4065,"tcpip: drop pending pkt(%x), len(%d)")
TRACE_MSG(TX_FLOWCTRL_123_112_2_17_23_45_32_4066,"tcpip: send pending pkt(%p, %d)")
TRACE_MSG(TX_FLOWCTRL_147_112_2_17_23_45_32_4067,"tcpip: empty tx pending queue(len: %d) on netif(%x)")
TRACE_MSG(TX_FLOWCTRL_261_112_2_17_23_45_33_4068,"tcpip: alloc backup pkt memory failed, %d")
TRACE_MSG(TX_FLOWCTRL_331_112_2_17_23_45_33_4069,"tcpip: no backup packet on netif %x")
TRACE_MSG(TX_FLOWCTRL_339_112_2_17_23_45_33_4070,"tcpip: invalid packet bias %d")
TRACE_MSG(TX_FLOWCTRL_350_112_2_17_23_45_33_4071,"tcpip: pk_alloc failed for backup packet tx on netif %x")
TRACE_MSG(TX_FLOWCTRL_371_112_2_17_23_45_33_4072,"tcpip: to send backup packet %x on netif %x")
TRACE_MSG(TX_FLOWCTRL_458_112_2_17_23_45_33_4073,"tcpip: delete pending pkt %x with received ack %x")
TRACE_MSG(TX_FLOWCTRL_481_112_2_17_23_45_33_4074,"tcpip: invalid packet buffer len %d")
TRACE_MSG(TX_FLOWCTRL_573_112_2_17_23_45_33_4075,"tcpip: drop pkt %x from tx pending queue on netif 0x%x")
TRACE_MSG(IN_PCB_168_112_2_17_23_45_34_4076,"tcpip: in_pcbconnect sin->sin_addr = %x, sin->sin_port = %d")
TRACE_MSG(IN_PCB_176_112_2_17_23_45_34_4077,"tcpip: in_pcbconnect() - sin->sin_port == 0")
TRACE_MSG(IN_PCB_193_112_2_17_23_45_34_4078,"tcpip: in_pcbconnect() error - inp == NULL || inp->ifp == NULL")
TRACE_MSG(IN_PCB_199_112_2_17_23_45_34_4079,"tcpip: in_pcbconnect() error - sin->sin_addr.s_addr == INADDR_BROADCAST")
TRACE_MSG(IN_PCB_214_112_2_17_23_45_34_4080,"tcpip: in_pcbconnect() error - iproute() failed!")
TRACE_MSG(NPTCP_260_112_2_17_23_45_36_4081,"tcpip: no pkt avail for mbuf MT_TXDATA, len = %d")
TRACE_MSG(NPTCP_269_112_2_17_23_45_36_4082,"tcpip: no pkt avail for mbuf %d, len = %d")
TRACE_MSG(NPTCP_304_112_2_17_23_45_36_4083,"tcpip: m_getnbuf() - invalid mbuf type %d")
TRACE_MSG(NPTCP_338_112_2_17_23_45_36_4084,"tcpip: m_getnbuf - getq fail")
TRACE_MSG(NPTCP_884_112_2_17_23_45_37_4085,"tcpip: tcp_rcv() - BAD_TCP_HEAD(%d): bad chksum, pkt %08x")
TRACE_MSG(NPTCP_1190_112_2_17_23_45_37_4086,"tcpip msslimit: change pkt(%p) mss %d -> %d")
TRACE_MSG(NPTCP_1281_112_2_17_23_45_38_4087,"tcpip debug: np_stripoptions - ihlen %d")
TRACE_MSG(NPTCP_1828_112_2_17_23_45_39_4088,"tcpip: mbuf addr(0x%x ~ 0x%x), size %d, num %d")
TRACE_MSG(SOCKCALL_88_112_2_17_23_45_40_4089,"tcpip: creat socket 0x%x, type = %d, netif = %x")
TRACE_MSG(SOCKCALL_121_112_2_17_23_45_40_4090,"tcpip: modify so %x with netif %x is not allowed")
TRACE_MSG(SOCKCALL_126_112_2_17_23_45_40_4091,"tcpip: modify so %x netif %x -> %x")
TRACE_MSG(SOCKCALL_213_112_2_17_23_45_41_4092,"tcpip: t_bind - so %x bind error %d")
TRACE_MSG(SOCKCALL_376_112_2_17_23_45_41_4093,"tcpip: t_connect() - so %x, error ENETUNREACH")
TRACE_MSG(SOCKCALL_415_112_2_17_23_45_41_4094,"tcpip: t_connect() - so %x is connected, %08x:%d -> %08x:%d")
TRACE_MSG(SOCKCALL_423_112_2_17_23_45_41_4095,"tcpip: t_connect() - refuse so %x connection, %08x:%d -> %08x:%d")
TRACE_MSG(SOCKCALL_470_112_2_17_23_45_41_4096,"tcpip: t_connect() - so %x, error ENETUNREACH")
TRACE_MSG(SOCKCALL_740_112_2_17_23_45_42_4097,"tcpip: t_recv() - so %x, error ENETUNREACH")
TRACE_MSG(SOCKCALL_760_112_2_17_23_45_42_4098,"tcpip: t_recv() - so %x received %d, total %d, left %d")
TRACE_MSG(SOCKCALL_825_112_2_17_23_45_42_4099,"tcpip: t_recvfrom() - so %x, error %d, ENETUNREACH")
TRACE_MSG(SOCKCALL_844_112_2_17_23_45_42_4100,"tcpip: t_recvfrom() - so %x received %d, total %d, left %d")
TRACE_MSG(SOCKCALL_973_112_2_17_23_45_42_4101,"tcpip: t_sendto() - so %x, error %d, ENETUNREACH")
TRACE_MSG(SOCKCALL_992_112_2_17_23_45_42_4102,"tcpip: t_sendto() - so %x sent %d, total %d, left %d")
TRACE_MSG(SOCKCALL_1120_112_2_17_23_45_42_4103,"tcpip: t_send() - so %x, error %d, ENETUNREACH")
TRACE_MSG(SOCKCALL_1132_112_2_17_23_45_42_4104,"tcpip: t_send() - so %x sent %d, total %d, left %d")
TRACE_MSG(SOCKCALL_1198_112_2_17_23_45_42_4105,"tcpip: close socket 0x%x, tot_snd %d, tot_rcv %d, snd_left %d, rcv_left %d")
TRACE_MSG(SOCKCALL_1209_112_2_17_23_45_43_4106,"tcpip: t_socketclose() - so %x, err %d")
TRACE_MSG(SOCKET_262_112_2_17_23_45_43_4107,"tcpip: sofree() - to free so 0x%x, tot_snd = %d, tot_rcv = %d")
TRACE_MSG(SOCKET_367_112_2_17_23_45_43_4108,"tcpip: soclose(), so %x with SS_NOFDREF directly return ")
TRACE_MSG(SOCKET_792_112_2_17_23_45_44_4109,"tcpip: clear so_rcv cr20961***!!!\"#")
TRACE_MSG(SOCKET_1187_112_2_17_23_45_45_4110,"tcpip: actually set so %x with TCP_ACKDELAYTIME value %d ms")
TRACE_MSG(SOCKET_1263_112_2_17_23_45_45_4111,"tcpip: actually set so %x with TCP_MAXSEG value %d")
TRACE_MSG(SOCKET_1492_112_2_17_23_45_46_4112,"tcpip: forced_sofree() - to free TCP so %p")
TRACE_MSG(SOCKET2_113_112_2_17_23_45_46_4113,"tcpip warning: soisconnected() - soqremque on so %x error")
TRACE_MSG(SOCKET2_226_112_2_17_23_45_46_4114,"tcpip: sonewconn() - new socket %x, type = %d, netif = %x")
TRACE_MSG(SOCKET2_707_112_2_17_23_45_47_4115,"tcpip:the sb->sb_cc = %d")
TRACE_MSG(SOSELECT_386_112_2_17_23_45_48_4116,"tcpip: FD_CLR() - no so %x found in list %p")
TRACE_MSG(SOSELECT_410_112_2_17_23_45_48_4117,"tcpip: FD_SET() - so %x is already set in list %p")
TRACE_MSG(SOSELECT_418_112_2_17_23_45_48_4118,"tcpip: FD_SET() - list %p is full for so %x")
TRACE_MSG(TCP_IN_355_112_2_17_23_45_49_4119,"[ TCPIPINFO ] Remote TCP sent RST(reset)")
TRACE_MSG(TCP_IN_834_112_2_17_23_45_50_4120,"[ TCPIPINFO ] socket 0x%x has establised!")
TRACE_MSG(TCP_IN_1044_112_2_17_23_45_51_4121,"tcpip debug: delay to send abnormal close event on so %x")
TRACE_MSG(TCP_IN_1130_112_2_17_23_45_51_4122,"[ TCPIPINFO ] socket 0x%x has establised!")
TRACE_MSG(TCP_IN_1599_112_2_17_23_45_52_4123,"[ TCPIPINFO ] Remote closed TCP connection by sending 'FIN'")
TRACE_MSG(TCP_IN_1610_112_2_17_23_45_52_4124,"tcpip debug: delay to send normal close event on so %x")
TRACE_MSG(TCP_IN_1822_112_2_17_23_45_52_4125,"tcpip: so %x, TCP local winscale %d, peer winscale %d")
TRACE_MSG(TCP_IN_2036_112_2_17_23_45_53_4126,"tcpip tcpoptmz: tcp_xmit_timer() - tp(%x), t_rxtcur( M:%d, A:%d )")
TRACE_MSG(TCP_OUT_237_112_2_17_23_45_54_4127,"tcpip: tcp_output(%d) - allocate memory failed!")
TRACE_MSG(TCP_OUT_304_112_2_17_23_45_54_4128,"[ TCPIPINFO ] Local sent TCP_RST(reset)")
TRACE_MSG(TCP_OUT_1000_112_2_17_23_45_55_4129,"tcpip exception: cp(%p) overflow tcp_optionbuf(%p, len:%d)")
TRACE_MSG(TCP_SUBR_316_112_2_17_23_45_56_4130,"tcpip tcpoptmz: tcp_newtcpcb() - tp(%x), t_rxtcur( M:%d, A:%d ), rxt_shift(%d)")
TRACE_MSG(TCP_TIMR_305_112_2_17_23_45_57_4131,"tcpip tcpoptmz: tcp_timers() - tp(%x), t_rxtcur( M:%d, A:%d ), rxt_shift(%d)")
TRACE_MSG(TCP_TIMR_314_112_2_17_23_45_57_4132,"[ TCPIPINFO ] Local TCP totally retransmitted %d times, %d ms.")
TRACE_MSG(TCP_USR_506_112_2_17_23_45_59_4133,"[T CPIPINFO ] Local closed TCP connection by sending 'FIN' first!")
TRACE_MSG(TCPPORT_128_112_2_17_23_46_0_4134,"tcpip: set TCP MSS = %d")
TRACE_MSG(TCPPORT_156_112_2_17_23_46_0_4135,"tcpip: set global TCP send space %d")
TRACE_MSG(UDPSOCK_88_112_2_17_23_46_2_4136,"tcpip warning: so %x recv buff not enough, drop pkt %x")
TRACE_MSG(FAKETCP_THROUGHUP_114_112_2_17_23_46_3_4137,"tcpip test: FakeTcpThroughupInit failed!")
TRACE_MSG(FAKETCP_THROUGHUP_218_112_2_17_23_46_3_4138,"tcpip test: THROUGHUP_FTCP_Test() to start!")
TRACE_MSG(FAKETCP_THROUGHUP_223_112_2_17_23_46_3_4139,"tcpip test: THROUGHUP_FTCP_Test() to end!")
TRACE_MSG(FAKETCP_THROUGHUP_248_112_2_17_23_46_3_4140,"tcpip test: fake TCP up-throughput task is already existed!")
TRACE_MSG(FAKETCP_THROUGHUP_255_112_2_17_23_46_3_4141,"tcpip test: FakeTcpThroughupInit failed!")
TRACE_MSG(FAKETCP_THROUGHUP_275_112_2_17_23_46_3_4142,"tcpip test: fake TCP throughup task ID : 0x%x ")
TRACE_MSG(FAKETCP_THROUGHUP_276_112_2_17_23_46_3_4143,"tcpip test: priority of fake TCP throughup task: %d ")
TRACE_MSG(FAKETCP_THROUGHUP_317_112_2_17_23_46_3_4144,"tcpip debug: create TCP on PORT %d <-> %d failed")
TRACE_MSG(FAKETCP_THROUGHUP_323_112_2_17_23_46_3_4145,"tcpip debug: to start FakeTcpThroughupTest - on %s:%d  <-> %s:%d")
TRACE_MSG(FAKETCP_THROUGHUP_354_112_2_17_23_46_3_4146,"tcpip debug: FakeTcpThroughupTest - totally sent %d bytes on PORT %d <-> %d")
TRACE_MSG(FAKETCP_THROUGHUP_535_112_2_17_23_46_4_4147,"tcpip: CreateTcpPacket - packet buffer not enough")
TRACE_MSG(MODEM_THROUGHUP_122_112_2_17_23_46_4_4148,"tcpip test: CreateUdpPacket() failed!")
TRACE_MSG(MODEM_THROUGHUP_134_112_2_17_23_46_4_4149,"tcpip test: MODEMTHROUGHUP_Test() to start!")
TRACE_MSG(MODEM_THROUGHUP_139_112_2_17_23_46_4_4150,"tcpip test: MODEMTHROUGHUP_Test() to end!")
TRACE_MSG(MODEM_THROUGHUP_164_112_2_17_23_46_4_4151,"tcpip test: modem up-throughput task is already existed!")
TRACE_MSG(MODEM_THROUGHUP_187_112_2_17_23_46_4_4152,"tcpip test: modem throughup task ID : 0x%x ")
TRACE_MSG(MODEM_THROUGHUP_188_112_2_17_23_46_4_4153,"tcpip test: priority of modem throughup task: %d ")
TRACE_MSG(MODEM_THROUGHUP_245_112_2_17_23_46_5_4154,"tcpip test: ModemThroughupTaskEntry - GetModemId() failed!")
TRACE_MSG(MODEM_THROUGHUP_275_112_2_17_23_46_5_4155,"tcpip test: ModemThroughupTaskEntry - CreateUdpPacket() failed!")
TRACE_MSG(MODEM_THROUGHUP_295_112_2_17_23_46_5_4156,"tcpip test: MNGPRS_GetPclinkCfg() error!")
TRACE_MSG(MODEM_THROUGHUP_303_112_2_17_23_46_5_4157,"tcpip test: ipsrc %s, ipdst %s, timegap %d")
TRACE_MSG(MODEM_THROUGHUP_335_112_2_17_23_46_5_4158,"tcpip test: found modem netif 0x%x")
TRACE_MSG(MODEM_THROUGHUP_348_112_2_17_23_46_5_4159,"tcpip test: modem_id of netif 0x%x is %d, his_asyncmap %x")
TRACE_MSG(MODEM_THROUGHUP_370_112_2_17_23_46_5_4160,"tcpip test: MuxToPPP - invalid input!")
TRACE_MSG(MODEM_THROUGHUP_374_112_2_17_23_46_5_4161,"tcpip test: %d bytes received with link_id %d")
TRACE_MSG(MODEM_THROUGHUP_382_112_2_17_23_46_5_4162,"tcpip test: MuxToPPP - no pppif matched with link_id %d")
TRACE_MSG(MODEM_THROUGHUP_414_112_2_17_23_46_5_4163,"tcpip test: CreateUdpPacket() - invalid input")
TRACE_MSG(MODEM_THROUGHUP_434_112_2_17_23_46_5_4164,"tcpip test: CreateUdpPacket() - too small buffer")
TRACE_MSG(MODEM_THROUGHUP_445_112_2_17_23_46_5_4165,"tcpip test: CreateUdpPacket() - act_ptr %x, act_max %d")
TRACE_MSG(MODEM_THROUGHUP_452_112_2_17_23_46_5_4166,"tcpip test: CreateUdpPacket() - too small packet")
TRACE_MSG(MODEM_THROUGHUP_474_112_2_17_23_46_5_4167,"tcpip test: CreateUdpPacket() - invalid type")
TRACE_MSG(MODEM_THROUGHUP_610_112_2_17_23_46_6_4168,"tcpip test: CreatePPPPacket() - invalid input")
TRACE_MSG(MODEM_THROUGHUP_677_112_2_17_23_46_6_4169,"tcpip test: ppp fcs not ok\n")
TRACE_MSG(PCLINK_UTILITY_144_112_2_17_23_46_6_4170,"tcpip test: MNGPRS_GetPclinkCfg() error!")
TRACE_MSG(CONFIG_FILE_76_112_2_17_23_46_11_4171,"%s(%d) : malloc memory failed!\n")
TRACE_MSG(CONFIG_FILE_113_112_2_17_23_46_11_4172,"%s(%d) : malloc memory failed!\n")
TRACE_MSG(ETHER_ADAPTER_117_112_2_17_23_46_12_4173,"tcpip ether: adapter name is too long!")
TRACE_MSG(ETHER_ADAPTER_140_112_2_17_23_46_12_4174,"tcpip ether: no empty position in adapter list")
TRACE_MSG(ETHER_ADAPTER_214_112_2_17_23_46_12_4175,"tcpip ether: set adapter to mode %d error!")
TRACE_MSG(ETHER_ADAPTER_221_112_2_17_23_46_12_4176,"tcpip ether: set adapter buffer size %d error!")
TRACE_MSG(ETHER_ADAPTER_228_112_2_17_23_46_12_4177,"tcpip ether: set adapter read time-out %d error!")
TRACE_MSG(ETHER_ADAPTER_283_112_2_17_23_46_12_4178,"tcpip ether: no free adapter handle found")
TRACE_MSG(ETHER_ADAPTER_401_112_2_17_23_46_13_4179,"tcpip ether: PacketSetNumWrites() failed!")
TRACE_MSG(ETHER_ADAPTER_408_112_2_17_23_46_13_4180,"tcpip ether: PacketSendPacket() failed!")
TRACE_MSG(ETHER_ADAPTER_497_112_2_17_23_46_13_4181,"tcpip ether: Open adapter \"%s\" error - 0x%x")
TRACE_MSG(ETHER_ADAPTER_501_112_2_17_23_46_13_4182,"tcpip ether: Open adapter \"%s\" OK")
TRACE_MSG(ETHER_ADAPTER_539_112_2_17_23_46_13_4183,"tcpip ether: retrieve list of adapter name failed!")
TRACE_MSG(ETHER_ADAPTER_554_112_2_17_23_46_13_4184,"tcpip ether: adapter name list is full!")
TRACE_MSG(ETHER_ADAPTER_564_112_2_17_23_46_13_4185,"tcpip ether: Current Ethernet Adapter Name List - num(%d)")
TRACE_MSG(ETHER_ADAPTER_567_112_2_17_23_46_13_4186," %d) - %s ")
TRACE_MSG(ETHER_PARAM_122_112_2_17_23_46_13_4187,"tcpip ether: IP string(%s) length is not long enough!")
TRACE_MSG(ETHER_PARAM_131_112_2_17_23_46_13_4188,"tcpip ether: not IP char %c exist in given IP string!")
TRACE_MSG(ETHER_PARAM_144_112_2_17_23_46_13_4189,"tcpip ether: there should be 3 '.' chars in IP string %s!")
TRACE_MSG(ETHER_PARAM_172_112_2_17_23_46_13_4190,"tcpip ether: %s - each number should be 3 at the most!")
TRACE_MSG(ETHER_PARAM_183_112_2_17_23_46_13_4191,"tcpip ether: %s - each number should be in 0~255!")
TRACE_MSG(ETHER_PARAM_208_112_2_17_23_46_14_4192,"tcpip ether: %s - not enough MAC string length!")
TRACE_MSG(ETHER_PARAM_218_112_2_17_23_46_14_4193,"tcpip ether: %s - none MAC char exist!")
TRACE_MSG(ETHER_PARAM_231_112_2_17_23_46_14_4194,"tcpip ether: %s - there should be 5 '-' chars in MAC string!")
TRACE_MSG(ETHER_PARAM_260_112_2_17_23_46_14_4195,"tcpip ether: %s - invalid MAC string!")
TRACE_MSG(ETHER_PARAM_323_112_2_17_23_46_14_4196,"tcpip ether: Convert IP string to long failed!")
TRACE_MSG(ETHER_PARAM_349_112_2_17_23_46_14_4197,"tcpip ether: Get IP String from file failed!")
TRACE_MSG(ETHER_PARAM_376_112_2_17_23_46_14_4198,"tcpip ether: Get MAC string from file failed!")
TRACE_MSG(ETHER_PARAM_402_112_2_17_23_46_14_4199,"tcpip ether: Get device description string from file failed!")
TRACE_MSG(ETHER_PARAM_428_112_2_17_23_46_14_4200,"tcpip ether: Get adapter name string from file failed!")
TRACE_MSG(ETHER_PARAM_494_112_2_17_23_46_14_4201,"tcpip ether: AddEtherNetif - interface list is full!")
TRACE_MSG(ETHER_PARAM_502_112_2_17_23_46_14_4202,"tcpip ether: AddEtherNetif - open adapter '%s' failed!")
TRACE_MSG(ETHER_PARAM_541_112_2_17_23_46_14_4203,"tcpip ether: load MAC in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_548_112_2_17_23_46_14_4204,"tcpip ether: load IP in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_555_112_2_17_23_46_14_4205,"tcpip ether: load subnet mask in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_562_112_2_17_23_46_14_4206,"tcpip ether: load Gateway in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_569_112_2_17_23_46_14_4207,"tcpip ether: load DNS in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_576_112_2_17_23_46_14_4208,"tcpip ether: load secondary DNS in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_582_112_2_17_23_46_14_4209,"tcpip ether: load DHCP in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_589_112_2_17_23_46_14_4210,"tcpip ether: load device desc in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_596_112_2_17_23_46_14_4211,"tcpip ether: load adapter name in section \"%s\" failed!")
TRACE_MSG(ETHER_PARAM_601_112_2_17_23_46_14_4212,"\ntcpip ether: net parameters from section \"%s\"")
TRACE_MSG(ETHER_PARAM_603_112_2_17_23_46_14_4213," < MAC . . . . . %02X:%02X:%02X:%02X:%02X:%02X >")
TRACE_MSG(ETHER_PARAM_604_112_2_17_23_46_14_4214," < IP  . . . . . %s > ")
TRACE_MSG(ETHER_PARAM_605_112_2_17_23_46_14_4215," < Gateway . . . %s > ")
TRACE_MSG(ETHER_PARAM_606_112_2_17_23_46_14_4216," < SubnetMask  . %s > ")
TRACE_MSG(ETHER_PARAM_607_112_2_17_23_46_14_4217," < DNS Pri . . . %s > ")
TRACE_MSG(ETHER_PARAM_608_112_2_17_23_46_14_4218," < DNS Sec . . . %s > ")
TRACE_MSG(ETHER_PARAM_609_112_2_17_23_46_14_4219," < DHCP  . . . . %s > ")
TRACE_MSG(ETHER_PARAM_610_112_2_17_23_46_14_4220," < DeviceDesc  . %s > ")
TRACE_MSG(ETHER_PARAM_611_112_2_17_23_46_14_4221," < NetAdapter  . %s > ")
TRACE_MSG(ETHER_PARAM_643_112_2_17_23_46_15_4222,"tcpip ether: DhcpCallBack - DHCP OK on netid %x")
TRACE_MSG(ETHER_PARAM_647_112_2_17_23_46_15_4223,"tcpip ether: DhcpCallBack - DHCP failed %d!")
TRACE_MSG(ETHER_PARAM_681_112_2_17_23_46_15_4224,"tcpip ether: get DHCP result event on netid %x")
TRACE_MSG(ETHER_PARAM_696_112_2_17_23_46_15_4225,"tcpip ether: DHCP event - DHCP OK on netid %x")
TRACE_MSG(ETHER_PARAM_700_112_2_17_23_46_15_4226,"tcpip ether: DHCP event - DHCP failed %d on netid %x!")
TRACE_MSG(ETHER_PARAM_715_112_2_17_23_46_15_4227,"tcpip ether: ptr->netid - unresolved EVENT %d ")
TRACE_MSG(ETHER_PARAM_746_112_2_17_23_46_15_4228,"tcpip ether: Get ethernet configure file failed!")
TRACE_MSG(ETHER_PARAM_835_112_2_17_23_46_15_4229,"tcpip ether: DHCP enabled on netif %x!")
TRACE_MSG(ETHER_PARAM_841_112_2_17_23_46_15_4230,"tcpip ether: set net interface for nsapi(%d, %d) failed!")
TRACE_MSG(ETHER_PARAM_849_112_2_17_23_46_15_4231,"tcpip ether: register net interface failed!")
TRACE_MSG(ETHER_PARAM_855_112_2_17_23_46_15_4232,"tcpip ether: DHCP enabled on netif %x!")
TRACE_MSG(ETHER_PARAM_876_112_2_17_23_46_15_4233,"tcpip ether: net id list is full")
TRACE_MSG(ETHER_PARAM_1065_112_2_17_23_46_15_4234,"tcpip ether: ETHERPARAM_StartDhcp - sci_async_dhcp() error %d")
TRACE_MSG(ETHER_PARAM_1067_112_2_17_23_46_15_4235,"tcpip ether: ETHERPARAM_StartDhcp - sci_dhcp_request() error %d")
TRACE_MSG(ETHER_PARAM_1096_112_2_17_23_46_15_4236,"tcpip ether: ETHERPARAM_GetFirstNetid() should be called first!")
TRACE_MSG(ETHER_PARAM_1154_112_2_17_23_46_16_4237,"tcpip ether: get ethernet state file failed!")
TRACE_MSG(ETHER_PARAM_1234_112_2_17_23_46_16_4238,"tcpip ether: Create task %s with ID : %d")
TRACE_MSG(ETHER_PARAM_1236_112_2_17_23_46_16_4239,"tcpip ether: Priority of task %s is : %d")
TRACE_MSG(ETHERNET_TEST_115_112_2_17_23_46_16_4240,"\n%s(%d) : WSAStartup() failed! ")
TRACE_MSG(ETHERNET_TEST_122_112_2_17_23_46_16_4241,"\n%s(%d) : Can't find WINSOCK dll! ")
TRACE_MSG(ETHERNET_TEST_130_112_2_17_23_46_16_4242,"\n%s(%d) : gethostname() failed - %d! ")
TRACE_MSG(ETHERNET_TEST_139_112_2_17_23_46_16_4243,"\n%s(%d) : gethostbyname() failed - %d! ")
TRACE_MSG(ETHERNET_TEST_155_112_2_17_23_46_16_4244,"\n%s(%d) : WSACleanup() failed! ")
TRACE_MSG(ETHERNET_TEST_175_112_2_17_23_46_16_4245,"\n%s(%d) : DNSTest - GetTestUrl() failed! ")
TRACE_MSG(ETHERNET_TEST_183_112_2_17_23_46_16_4246,"\n%s(%d) : DNSTest - sci_gethostbyname() failed! ")
TRACE_MSG(ETHERNET_TEST_192_112_2_17_23_46_16_4247,"\nDNSTest Result:\n <URL> - %s\n <IP> - %s\n")
TRACE_MSG(ETHERNET_TEST_215_112_2_17_23_46_16_4248,"n%s(%d) : UDPTest - sci_sock_socket() error : %d!n")
TRACE_MSG(ETHERNET_TEST_223_112_2_17_23_46_16_4249,"\nUDPTest Client IP - %s : %d  ")
TRACE_MSG(ETHERNET_TEST_230_112_2_17_23_46_16_4250,"%s(%d) : UDPTest - sci_sock_bind() error : %d!n")
TRACE_MSG(ETHERNET_TEST_247_112_2_17_23_46_16_4251,"n%s(%d) : UDPTest - sci_sock_sendto() error : %d! ")
TRACE_MSG(ETHERNET_TEST_268_112_2_17_23_46_16_4252,"\n%s(%d) : UDPTest - select() time out! ")
TRACE_MSG(ETHERNET_TEST_274_112_2_17_23_46_16_4253,"n%s(%d) : UDPTest - select() error : %d! ")
TRACE_MSG(ETHERNET_TEST_288_112_2_17_23_46_17_4254,"n%s(%d) : UDPTest - sci_sock_recvfrom() error : %d! ")
TRACE_MSG(ETHERNET_TEST_294_112_2_17_23_46_17_4255,"n%s(%d) : UDPTest - socket connection is broken : %d! ")
TRACE_MSG(ETHERNET_TEST_310_112_2_17_23_46_17_4256,"n%s(%d) : UDPTest - Reply is not from server, go on waiting! ")
TRACE_MSG(ETHERNET_TEST_317_112_2_17_23_46_17_4257,"\n%s(%d) : UDPTest - client socket is not set in read field! ")
TRACE_MSG(ETHERNET_TEST_346_112_2_17_23_46_17_4258,"n%s(%d) : TCPTest - sci_sock_socket() error : %d! ")
TRACE_MSG(ETHERNET_TEST_360_112_2_17_23_46_17_4259,"n%s(%d) : TCPTest - sci_sock_bind() error : %d! ")
TRACE_MSG(ETHERNET_TEST_373_112_2_17_23_46_17_4260,"n%s(%d) : TCPTest - sci_sock_connect() error : %d! ")
TRACE_MSG(ETHERNET_TEST_383_112_2_17_23_46_17_4261,"n%s(%d) : TCPTest - sci_sock_send() error : %d! ")
TRACE_MSG(ETHERNET_TEST_404_112_2_17_23_46_17_4262,"\n%s(%d) : TCPTest - sci_sock_select() time out! ")
TRACE_MSG(ETHERNET_TEST_410_112_2_17_23_46_17_4263,"n%s(%d) : TCPTest - sci_sock_select() error : %d! ")
TRACE_MSG(ETHERNET_TEST_422_112_2_17_23_46_17_4264,"n%s(%d) : TCPTest - sci_sock_recvfrom() error : %d! ")
TRACE_MSG(ETHERNET_TEST_428_112_2_17_23_46_17_4265,"n%s(%d) : TCPTest - socket connection is broken : %d! ")
TRACE_MSG(ETHERNET_TEST_444_112_2_17_23_46_17_4266,"n%s(%d) : TCPTest - client socket is not set in read field! ")
TRACE_MSG(ETHERNET_TEST_464_112_2_17_23_46_17_4267,"\n<TCP/IP Ethernet Test Start>\n")
TRACE_MSG(ETHERNET_TEST_505_112_2_17_23_46_17_4268,"%s(%d) : Create Ethernet Test Task with ID : %dn")
TRACE_MSG(ETHERNET_TEST_507_112_2_17_23_46_17_4269,"%s(%d) : Priority of Ethernet Test Task is : %dn")
TRACE_MSG(ETHERNET_TEST_524_112_2_17_23_46_17_4270,"%s(%d) : Close Ethernet Test Task with ID : %d\n")
TRACE_MSG(FD_OPERATION_179_112_2_17_23_46_18_4271,"\ntcpip ether: Found \"%s\" in \"%s\"\n")
TRACE_MSG(FD_OPERATION_359_112_2_17_23_46_18_4272,"tcpip ether: FILEDIR_GetConfigFilePath - _getcwd() error!\n")
TRACE_MSG(FD_OPERATION_369_112_2_17_23_46_18_4273,"tcpip ether: searching \"%s\" in \"%s\"...\n")
TRACE_MSG(FD_OPERATION_380_112_2_17_23_46_18_4274,"tcpip ether: configure file \"%s\" is not found!\n")
TRACE_MSG(FD_OPERATION_403_112_2_17_23_46_18_4275,"tcpip ether: FILEDIR_GetStateFilePath - _getcwd() error!\n")
TRACE_MSG(FD_OPERATION_437_112_2_17_23_46_18_4276,"tcpip ether: FILEDIR_GetSettingFilePath - _getcwd() error!\n")
TRACE_MSG(MUX_SIMULATE_49_112_2_17_23_46_19_4277,"MUX_DTI_Create fail due to link id(%d) execed")
TRACE_MSG(MUX_SIMULATE_65_112_2_17_23_46_19_4278,"MUX_DTI_Destroy fail due to link id(%d) execed")
TRACE_MSG(MUX_SIMULATE_81_112_2_17_23_46_19_4279,"tcpip simulator: MUX_Write() %d data with link id %d")
TRACE_MSG(TCPIP_ETHER_359_112_2_17_23_46_20_4280,"tcpip ether: Create task %s with ID : %d")
TRACE_MSG(TCPIP_ETHER_361_112_2_17_23_46_20_4281,"tcpip ether: Priority of task %s is : %d")
TRACE_MSG(TCPIP_ETHER_418_112_2_17_23_46_20_4282,"tcpip ether: EtherTaskSendEntry - pkt pointer is NULL!")
TRACE_MSG(TCPIP_ETHER_440_112_2_17_23_46_20_4283,"tcpip ether: TX buffer alloc failed!")
TRACE_MSG(TCPIP_ETHER_506_112_2_17_23_46_21_4284,"tcpip ether: Create task %s(%d) with ID : %d")
TRACE_MSG(TCPIP_ETHER_508_112_2_17_23_46_21_4285,"tcpip ether: Priority of task %s is : %d")
TRACE_MSG(TCPIP_ETHER_539_112_2_17_23_46_21_4286,"tcpip ether: bind recv task %x to ether adapter failed")
TRACE_MSG(TCPIP_ETHER_547_112_2_17_23_46_21_4287,"tcpip ether: alloc recv buffer in task %x failed")
TRACE_MSG(TCPIP_ETHER_558_112_2_17_23_46_21_4288,"tcpip ether: config adapter as buff(%d), rd_tmout(%d), type(%d) failed")
TRACE_MSG(TCPIP_ETHER_571_112_2_17_23_46_21_4289,"tcpip ether: packet receive failed!")
TRACE_MSG(MBOX_PING_400_112_2_17_23_47_44_4290,"finish timer expires\n")
TRACE_MSG(MBOX_PING_456_112_2_17_23_47_44_4291,"netbuf is used up, sleep a while\n")
TRACE_MSG(MBOX_PING_542_112_2_17_23_47_45_4292,"---- WMIPING Statistics ----\n")
TRACE_MSG(MBOX_PING_543_112_2_17_23_47_45_4293,"Packets transmitted: %d\n")
TRACE_MSG(MBOX_PING_544_112_2_17_23_47_45_4294,"Packets received: %d\n")
TRACE_MSG(MBOX_PING_561_112_2_17_23_47_45_4295,"Throughput = %.2f pkts/sec, %.2f Mbpsn")
TRACE_MSG(SPI_TESTING_262_112_2_17_23_47_46_4296,"spitest_DoPioExternalAccess %d\n")
TRACE_MSG(SPI_TESTING_273_112_2_17_23_47_46_4297,"spitest_DoPioExternalAccess %d\n")
TRACE_MSG(SPI_TESTING_276_112_2_17_23_47_46_4298,"spitest_DoPioExternalAccess %d\n")
TRACE_MSG(RTW_WLAN_UTIL_1476_112_2_17_23_49_14_4299,"------->%s, raid:%d mask:%8.8xn")
TRACE_MSG(RTL8192C_PHYCFG_1268_112_2_17_23_49_56_4300,"---->_rtl8192c_phy_set_bwmode():20M\n")
TRACE_MSG(RTL8192C_PHYCFG_1272_112_2_17_23_49_56_4301,"---->_rtl8192c_phy_set_bwmode():40M\n")
TRACE_MSG(RTL8192CS_DESC_54_112_2_17_23_50_6_4302,"---->HAL_PRIME_CHNL_OFFSET_LOWER\n")
TRACE_MSG(RTL8192CS_DESC_57_112_2_17_23_50_6_4303,"---->HAL_PRIME_CHNL_OFFSET_UPPER\n")
TRACE_MSG(RTL8192CS_DESC_60_112_2_17_23_50_6_4304,"---->HAL_PRIME_CHNL_OFFSET_DONT_CARE\n")
TRACE_MSG(RTL8192CS_DESC_63_112_2_17_23_50_6_4305,"---->HAL_PRIME_CHNL_OFFSET_WRONG\n")
TRACE_MSG(RTL8192CS_GEN_228_112_2_17_23_50_7_4306,"&&&&&&&&&&BSSID-i: %x\n")
TRACE_MSG(RTL8192CS_GEN_285_112_2_17_23_50_7_4307,"HW_VAR_AMPDU_MIN_SPACE:%x\n")
TRACE_MSG(RTL8192CS_GEN_335_112_2_17_23_50_8_4308,"HW_VAR_AMPDU_FACTOR:%x\n")
TRACE_MSG(RTL8192CS_INIT_143_112_2_17_23_50_20_4309,"---->Set RF Chip ID to RF_6052 and RF type to 1T1R.\n")
TRACE_MSG(RTL8192CS_INIT_148_112_2_17_23_50_20_4310,"rf_chip=0x%x, rf_type=0x%x\n")
TRACE_MSG(RTL8192CS_INIT_515_112_2_17_23_50_20_4311,"leonard 200 %x 204 %x\n")
TRACE_MSG(GSPI_IO_249_112_2_17_23_50_25_4312,"&&&&&leonard temp_data[0-4]: %x %x %x %xn")
TRACE_MSG(THREADX_INTFS_269_112_2_17_23_50_28_4313,"rtw_xmit_entry: rtw_if_up fail\n")
TRACE_MSG(THREADX_INTFS_362_112_2_17_23_50_28_4314,"thx_relay_xmit_entry: rtw_if_up fail\n")
TRACE_MSG(SOCKETADDR_330_112_2_17_23_51_3_4315,"FATAL: get_in_addr called on IPv6 address\n")
TRACE_MSG(GNU_GETOPT_763_112_2_17_23_51_9_4316,"digits occur in two different argv-elements.\n")
TRACE_MSG(GNU_GETOPT_765_112_2_17_23_51_9_4317,"option %c\n")
TRACE_MSG(GNU_GETOPT_769_112_2_17_23_51_9_4318,"option a\n")
TRACE_MSG(GNU_GETOPT_773_112_2_17_23_51_9_4319,"option b\n")
TRACE_MSG(GNU_GETOPT_777_112_2_17_23_51_9_4320,"option c with value `%s'\n")
TRACE_MSG(GNU_GETOPT_784_112_2_17_23_51_9_4321,"?? gnu_getopt returned character code 0%o ??\n")
TRACE_MSG(GNU_GETOPT_789_112_2_17_23_51_9_4322,"non-option ARGV-elements: ")
TRACE_MSG(GNU_GETOPT_791_112_2_17_23_51_9_4323,"%s ")
TRACE_MSG(GNU_GETOPT_792_112_2_17_23_51_9_4324,"\n")
TRACE_MSG(GNU_GETOPT_LONG_103_112_2_17_23_51_9_4325,"option %s")
TRACE_MSG(GNU_GETOPT_LONG_105_112_2_17_23_51_9_4326," with arg %s")
TRACE_MSG(GNU_GETOPT_LONG_106_112_2_17_23_51_9_4327,"\n")
TRACE_MSG(GNU_GETOPT_LONG_120_112_2_17_23_51_10_4328,"digits occur in two different argv-elements.\n")
TRACE_MSG(GNU_GETOPT_LONG_122_112_2_17_23_51_10_4329,"option %c\n")
TRACE_MSG(GNU_GETOPT_LONG_126_112_2_17_23_51_10_4330,"option a\n")
TRACE_MSG(GNU_GETOPT_LONG_130_112_2_17_23_51_10_4331,"option b\n")
TRACE_MSG(GNU_GETOPT_LONG_134_112_2_17_23_51_10_4332,"option c with value `%s'\n")
TRACE_MSG(GNU_GETOPT_LONG_138_112_2_17_23_51_10_4333,"option d with value `%s'\n")
TRACE_MSG(GNU_GETOPT_LONG_145_112_2_17_23_51_10_4334,"?? gnu_getopt returned character code 0%o ??\n")
TRACE_MSG(GNU_GETOPT_LONG_150_112_2_17_23_51_10_4335,"non-option ARGV-elements: ")
TRACE_MSG(GNU_GETOPT_LONG_152_112_2_17_23_51_10_4336,"%s ")
TRACE_MSG(GNU_GETOPT_LONG_153_112_2_17_23_51_10_4337,"\n")
TRACE_MSG(WSP_API_68_112_2_17_23_51_16_4338,"\n wsp WSP_InitRequest task id = %d")
TRACE_MSG(WSP_API_80_112_2_17_23_51_16_4339,"\n wsp WSP_InitRequest error_code = %d")
TRACE_MSG(WSP_API_129_112_2_17_23_51_16_4340,"\n wsp WSP_GetRequest error_code = %d")
TRACE_MSG(WSP_API_178_112_2_17_23_51_16_4341,"\n wsp WSP_PostRequest error_code = %d")
TRACE_MSG(WSP_API_229_112_2_17_23_51_16_4342,"\n wsp WSP_AuthResponse error_code = %d")
TRACE_MSG(WSP_API_274_112_2_17_23_51_16_4343,"\n wsp WSP_CloseRequest error_code = %d")
TRACE_MSG(WSP_API_322_112_2_17_23_51_16_4344,"\n wsp WSP_CancelRequest error_code = %d")
TRACE_MSG(WSP_API_411_112_2_17_23_51_16_4345,"\n http HTTP_CookieClear error_code = %d")
TRACE_MSG(WSP_API_442_112_2_17_23_51_17_4346,"\n wsp WSP_CacheClear 1 error_code = %d")
TRACE_MSG(WSP_API_459_112_2_17_23_51_17_4347,"\n wsp WSP_CacheClear 2 error_code = %d")
TRACE_MSG(WSP_API_478_112_2_17_23_51_17_4348,"\n http HTTP_CacheClear 3 error_code = %d")
TRACE_MSG(WSP_API_492_112_2_17_23_51_17_4349,"\n wsp WSP_CacheClear 4 error_code = %d")
TRACE_MSG(WSP_API_544_112_2_17_23_51_17_4350,"\n wsp WSP_WtlsCertResponse error_code = %d")
TRACE_MSG(WSP_AUTH_150_112_2_17_23_51_17_4351,"\n wsp WspAuthenDigestEncode input error \n")
TRACE_MSG(WSP_AUTH_156_112_2_17_23_51_17_4352,"\n wsp WspAuthenDigestEncode input error2 \n")
TRACE_MSG(WSP_AUTH_164_112_2_17_23_51_17_4353,"\n wsp WspAuthenDigestEncode alloc error \n")
TRACE_MSG(WSP_AUTH_172_112_2_17_23_51_17_4354,"\n wsp WspAuthenDigestEncode AuthenDigestRequsetSetParam error \n")
TRACE_MSG(WSP_AUTH_219_112_2_17_23_51_17_4355,"\n wsp SetAuthenDigestRequsetParam input error \n")
TRACE_MSG(WSP_AUTH_393_112_2_17_23_51_18_4356,"\n wsp SetAuthenDigestRequsetParam error_code =%d \n")
TRACE_MSG(WSP_AUTH_454_112_2_17_23_51_18_4357,"\n wsp WSP_AuthenDigestResponseParse input error \n")
TRACE_MSG(WSP_AUTH_461_112_2_17_23_51_18_4358,"\n wsp WSP_AuthenDigestResponseParse alloc error \n")
TRACE_MSG(WSP_AUTH_488_112_2_17_23_51_18_4359,"\n wsp WSP_AuthenDigestResponseParse 1 error_code =%d")
TRACE_MSG(WSP_AUTH_505_112_2_17_23_51_18_4360,"\n wsp WSP_AuthenDigestResponseParse error net data \n")
TRACE_MSG(WSP_AUTH_514_112_2_17_23_51_18_4361,"\n wsp WSP_AuthenDigestResponseParse 2 error_code=%d")
TRACE_MSG(WSP_AUTH_548_112_2_17_23_51_18_4362,"\n wsp AuthenDigestResponseSaveParam invalid input \n")
TRACE_MSG(WSP_AUTH_741_112_2_17_23_51_18_4363,"\n wsp AuthenDigestResponseSaveParam unknown param \n")
TRACE_MSG(WSP_AUTH_748_112_2_17_23_51_18_4364,"\n wsp AuthenDigestResponseSaveParam error_code=%d")
TRACE_MSG(WSP_AUTH_767_112_2_17_23_51_18_4365,"\n wsp AuthenDigestResponseSaveParam CLEAR error_code=%d")
TRACE_MSG(WSP_AUTH_783_112_2_17_23_51_18_4366,"\n wsp AuthenDigestTokenFind input error \n")
TRACE_MSG(WSP_AUTH_796_112_2_17_23_51_18_4367,"\n wsp AuthenDigestTokenFind auth map type =%d")
TRACE_MSG(WSP_AUTH_800_112_2_17_23_51_18_4368,"\n wsp AuthenDigestTokenFind error \n")
TRACE_MSG(WSP_AUTH_819_112_2_17_23_51_18_4369,"\n wsp WSP_AuthenGetInfo input error \n")
TRACE_MSG(WSP_AUTH_860_112_2_17_23_51_18_4370,"\n wsp WSP_AuthenGetInfo unknown scheme\n")
TRACE_MSG(WSP_AUTH_893_112_2_17_23_51_19_4371,"\n wsp CombineAuthenEncodedResult input error \n")
TRACE_MSG(WSP_AUTH_904_112_2_17_23_51_19_4372,"\n wsp CombineAuthenEncodedResult alloc error \n")
TRACE_MSG(WSP_AUTH_919_112_2_17_23_51_19_4373,"\n wsp CombineAuthenEncodedResult 1 input error \n")
TRACE_MSG(WSP_AUTH_925_112_2_17_23_51_19_4374,"\n wsp CombineAuthenEncodedResult 1 alloc error \n")
TRACE_MSG(WSP_CACHE_118_112_2_17_23_51_19_4375,"\n wsp WSP_HandleCacheDataToApp 1 input error \n")
TRACE_MSG(WSP_CACHE_125_112_2_17_23_51_19_4376,"\n wsp WSP_HandleCacheDataToApp 2 input error \n")
TRACE_MSG(WSP_CACHE_142_112_2_17_23_51_19_4377,"\n wsp WSP_HandleCacheDataToApp file not found error \n")
TRACE_MSG(WSP_CACHE_150_112_2_17_23_51_19_4378,"\n wsp WSP_HandleCacheDataToApp get file size error \n")
TRACE_MSG(WSP_CACHE_168_112_2_17_23_51_19_4379,"\n wsp WSP_HandleCacheDataToApp alloc error \n")
TRACE_MSG(WSP_CACHE_178_112_2_17_23_51_19_4380,"\n wsp WSP_HandleCacheDataToApp file read error \n")
TRACE_MSG(WSP_CACHE_195_112_2_17_23_51_19_4381,"\n wsp WSP_HandleCacheDataToApp send HTTP_SIG_GET_CNF")
TRACE_MSG(WSP_CACHE_202_112_2_17_23_51_19_4382,"\n wsp WSP_HandleCacheDataToApp error method\n")
TRACE_MSG(WSP_EVENT_38_112_2_17_23_51_19_4383,"\n wsp WSP_EventQueueInit input error\n")
TRACE_MSG(WSP_EVENT_56_112_2_17_23_51_19_4384,"\n wsp WSP_EventQueueInsert input error\n")
TRACE_MSG(WSP_EVENT_64_112_2_17_23_51_19_4385,"\n wsp WSP_EventQueueInsert alloc error\n")
TRACE_MSG(WSP_EVENT_99_112_2_17_23_51_20_4386,"\n wsp WSP_EventQueueFetch input error\n")
TRACE_MSG(WSP_EVENT_146_112_2_17_23_51_20_4387,"\n wsp WSP_EventQueueClear input error\n")
TRACE_MSG(WSP_EVENT_175_112_2_17_23_51_20_4388,"\n wsp WSP_EventDestroy input error\n")
TRACE_MSG(WSP_EVENT_237_112_2_17_23_51_20_4389,"WSP:WSP_EventDestroy.sig code error = %d!")
TRACE_MSG(WSP_HEADER_42_112_2_17_23_51_20_4390,"\n wsp WSP_SetAcceptHeader input error\n")
TRACE_MSG(WSP_HEADER_49_112_2_17_23_51_20_4391,"\n wsp WSP_SetAcceptHeader alloc error\n")
TRACE_MSG(WSP_HEADER_78_112_2_17_23_51_20_4392,"\n wsp WSP_SetAcceptCharsetHeader input error\n")
TRACE_MSG(WSP_HEADER_85_112_2_17_23_51_20_4393,"\n wsp WSP_SetAcceptCharsetHeader alloc error\n")
TRACE_MSG(WSP_HEADER_114_112_2_17_23_51_20_4394,"\n wsp WSP_SetAcceptEncodingHeader input error\n")
TRACE_MSG(WSP_HEADER_121_112_2_17_23_51_20_4395,"\n wsp WSP_SetAcceptEncodingHeader alloc error\n")
TRACE_MSG(WSP_HEADER_150_112_2_17_23_51_20_4396,"\n wsp WSP_SetAcceptLanguageHeader input error\n")
TRACE_MSG(WSP_HEADER_157_112_2_17_23_51_20_4397,"\n wsp WSP_SetAcceptLanguageHeader alloc error\n")
TRACE_MSG(WSP_HEADER_186_112_2_17_23_51_20_4398,"\n wsp WSP_SetRefererHeader input error\n")
TRACE_MSG(WSP_HEADER_193_112_2_17_23_51_20_4399,"\n wsp WSP_SetRefererHeader alloc error\n")
TRACE_MSG(WSP_HEADER_222_112_2_17_23_51_20_4400,"\n wsp WSP_SetUserAgentHeader input error\n")
TRACE_MSG(WSP_HEADER_229_112_2_17_23_51_20_4401,"\n wsp WSP_SetUserAgentHeader alloc error\n")
TRACE_MSG(WSP_HEADER_258_112_2_17_23_51_21_4402,"\n wsp WSP_SetContentTypeHeader input error\n")
TRACE_MSG(WSP_HEADER_265_112_2_17_23_51_21_4403,"\n wsp WSP_SetContentTypeHeader alloc error\n")
TRACE_MSG(WSP_HEADER_294_112_2_17_23_51_21_4404,"\n wsp WSP_SetUAProfileHeader input error\n")
TRACE_MSG(WSP_HEADER_301_112_2_17_23_51_21_4405,"\n wsp WSP_SetUAProfileHeader alloc error\n")
TRACE_MSG(WSP_HEADER_332_112_2_17_23_51_21_4406,"\n wsp WSP_SetHeaderField input error\n")
TRACE_MSG(WSP_HEADER_339_112_2_17_23_51_21_4407,"\n wsp WSP_SetHeaderField alloc error\n")
TRACE_MSG(WSP_HSM_374_112_2_17_23_51_22_4408,"\n wsp WSPHSM_Create host_handle =%x\n")
TRACE_MSG(WSP_HSM_383_112_2_17_23_51_22_4409,"\n wsp WSPHSM_Create  alloc  wsp_hsm_ptr error\n")
TRACE_MSG(WSP_HSM_394_112_2_17_23_51_22_4410,"\n wsp WSPHSM_Create  alloc  event_queue_ptr error\n")
TRACE_MSG(WSP_HSM_405_112_2_17_23_51_22_4411,"\n wsp WSPHSM_Create session = %d\n")
TRACE_MSG(WSP_HSM_412_112_2_17_23_51_22_4412,"\n wsp WSPHSM_Create wsp_hsm_ptr = %d")
TRACE_MSG(WSP_HSM_453_112_2_17_23_51_22_4413,"\n wsp HandleHsmDestroy hsm_ptr = %d")
TRACE_MSG(WSP_HSM_487_112_2_17_23_51_22_4414,"\n wsp WSPHSM_DispatchSignal input error\n")
TRACE_MSG(WSP_HSM_490_112_2_17_23_51_22_4415,"\n wsp WSPHSM_DispatchSignal hsm_ptr=%d,signal_code=%d\n")
TRACE_MSG(WSP_HSM_540_112_2_17_23_51_22_4416,"\n wsp WSPHSM_DealRequest input error\n")
TRACE_MSG(WSP_HSM_546_112_2_17_23_51_22_4417,"\n wsp WSPHSM_DealRequest request_ptr=%d\n")
TRACE_MSG(WSP_HSM_550_112_2_17_23_51_22_4418,"\n wsp WSPHSM_DealRequest hsm_ptr input error\n")
TRACE_MSG(WSP_HSM_624_112_2_17_23_51_22_4419,"\n wsp QHSM_Wsp input error\n")
TRACE_MSG(WSP_HSM_635_112_2_17_23_51_22_4420,"\n wsp QHSM_Wsp receive wtp destroy hsm ind! \n")
TRACE_MSG(WSP_HSM_644_112_2_17_23_51_22_4421,"\n wsp QHSM_Wsp receive wsp destroy hsm signal! \n")
TRACE_MSG(WSP_HSM_650_112_2_17_23_51_22_4422,"\n wsp QHSM_Wsp receive wtp send prog ind signal! \n")
TRACE_MSG(WSP_HSM_665_112_2_17_23_51_23_4423,"\n wsp QHSM_Wsp receive wsp error signal! \n")
TRACE_MSG(WSP_HSM_693_112_2_17_23_51_23_4424,"\n wsp Wsp_Idle input error! \n")
TRACE_MSG(WSP_HSM_700_112_2_17_23_51_23_4425,"\n wsp Wsp_Idle receive app req signal! \n")
TRACE_MSG(WSP_HSM_710_112_2_17_23_51_23_4426,"\n wsp Wsp_Idle receive connect proxy signal! \n")
TRACE_MSG(WSP_HSM_724_112_2_17_23_51_23_4427,"\n wsp Wsp_Idle receive handshake req signal! \n")
TRACE_MSG(WSP_HSM_756_112_2_17_23_51_23_4428,"\n wsp Wsp_Connecting input error! \n")
TRACE_MSG(WSP_HSM_764_112_2_17_23_51_23_4429,"\n wsp Wsp_Connecting receive seg result signal! \n")
TRACE_MSG(WSP_HSM_773_112_2_17_23_51_23_4430,"\n wsp Wsp_Connecting receive last result signal! \n")
TRACE_MSG(WSP_HSM_783_112_2_17_23_51_23_4431,"\n wsp Wsp_Connecting receive wtp abort ind signal! \n")
TRACE_MSG(WSP_HSM_789_112_2_17_23_51_23_4432,"\n wsp Wsp_Connecting receive cancel req signal! \n")
TRACE_MSG(WSP_HSM_823_112_2_17_23_51_23_4433,"\n wsp Wsp_AuthRspPending input error! \n")
TRACE_MSG(WSP_HSM_831_112_2_17_23_51_23_4434,"\n wsp Wsp_AuthRspPending receive cancel req signal! \n")
TRACE_MSG(WSP_HSM_837_112_2_17_23_51_23_4435,"\n wsp Wsp_AuthRspPending receive app auth rsp signal! \n")
TRACE_MSG(WSP_HSM_873_112_2_17_23_51_23_4436,"\n wsp Wsp_Handshaking input error! \n")
TRACE_MSG(WSP_HSM_884_112_2_17_23_51_23_4437,"\n wsp delete handshake timer = %d")
TRACE_MSG(WSP_HSM_891_112_2_17_23_51_23_4438,"\n wsp create handshake timer = %d")
TRACE_MSG(WSP_HSM_898_112_2_17_23_51_23_4439,"\n wsp delete handshake timer = %d")
TRACE_MSG(WSP_HSM_906_112_2_17_23_51_23_4440,"\n wsp delete handshake timer = %d")
TRACE_MSG(WSP_HSM_912_112_2_17_23_51_23_4441,"\n wsp Wsp_Handshaking receive wtls handshake succ signal! \n")
TRACE_MSG(WSP_HSM_916_112_2_17_23_51_23_4442,"\n wsp delete handshake timer = %d")
TRACE_MSG(WSP_HSM_931_112_2_17_23_51_23_4443,"\n wsp Wsp_Idle receive connect proxy signal! \n")
TRACE_MSG(WSP_HSM_948_112_2_17_23_51_23_4444,"\n wsp delete handshake timer = %d")
TRACE_MSG(WSP_HSM_953_112_2_17_23_51_23_4445,"\n wsp Wsp_Handshaking receive wtls cert ind signal \n")
TRACE_MSG(WSP_HSM_962_112_2_17_23_51_23_4446,"\n wsp delete handshake timer = %d")
TRACE_MSG(WSP_HSM_969_112_2_17_23_51_23_4447,"\n wsp create handshake timer = %d")
TRACE_MSG(WSP_HSM_972_112_2_17_23_51_23_4448,"\n wsp Wsp_Handshaking receive app untrust rsp signal! \n")
TRACE_MSG(WSP_HSM_981_112_2_17_23_51_23_4449,"\n wsp delete handshake timer = %d")
TRACE_MSG(WSP_HSM_984_112_2_17_23_51_23_4450,"\n wsp Wsp_Handshaking receive cancel req signal! \n")
TRACE_MSG(WSP_HSM_1009_112_2_17_23_51_23_4451,"\n wsp Wsp_Connected input error! \n")
TRACE_MSG(WSP_HSM_1016_112_2_17_23_51_23_4452,"\n wsp Wsp_Connected receive wtp invoke ind signal! \n")
TRACE_MSG(WSP_HSM_1021_112_2_17_23_51_23_4453,"\n wsp Wsp_Connected receive cancel req signal! \n")
TRACE_MSG(WSP_HSM_1027_112_2_17_23_51_23_4454,"\n wsp Wsp_Connected receive method invoke req signal! \n")
TRACE_MSG(WSP_HSM_1060_112_2_17_23_51_23_4455,"\n wsp Wsp_Waiting input error! \n")
TRACE_MSG(WSP_HSM_1068_112_2_17_23_51_23_4456,"\n wsp Wsp_Waiting receive seg result ind signal! \n")
TRACE_MSG(WSP_HSM_1077_112_2_17_23_51_23_4457,"\n wsp Wsp_Waiting receive last result ind signal! \n")
TRACE_MSG(WSP_HSM_1086_112_2_17_23_51_23_4458,"\n wsp Wsp_Waiting receive wtp abort ind signal! \n")
TRACE_MSG(WSP_HSM_1095_112_2_17_23_51_23_4459,"\n wsp Wsp_Waiting receive cancel req signal! \n")
TRACE_MSG(WSP_HSM_1130_112_2_17_23_51_23_4460,"\n wsp Wsp_Completing input error! \n")
TRACE_MSG(WSP_HSM_1137_112_2_17_23_51_23_4461,"\n wsp Wsp_Completing entry ! \n")
TRACE_MSG(WSP_HSM_1154_112_2_17_23_51_24_4462,"\n wsp Wsp_Completing destroy wsp hsm ! \n")
TRACE_MSG(WSP_HSM_1161_112_2_17_23_51_24_4463,"\n wsp Wsp_Completing receive wsp  resuest fin ind signal! \n")
TRACE_MSG(WSP_HSM_1190_112_2_17_23_51_24_4464,"\n wsp HandleCompletingRequestFinInd input error! \n")
TRACE_MSG(WSP_HSM_1208_112_2_17_23_51_24_4465,"\n wsp HandleCompletingRequestFinInd disconnect wsp ! \n")
TRACE_MSG(WSP_HSM_1216_112_2_17_23_51_24_4466,"\n wsp HandleCompletingRequestFinInd:re-handle current request! \n")
TRACE_MSG(WSP_HSM_1223_112_2_17_23_51_24_4467,"\n wsp HandleCompletingRequestFinInd:inform net mgr redirect! \n")
TRACE_MSG(WSP_HSM_1228_112_2_17_23_51_24_4468,"\n wsp HandleCompletingRequestFinInd redirect error \n")
TRACE_MSG(WSP_HSM_1235_112_2_17_23_51_24_4469,"\n wsp HandleCompletingRequestFinInd:inform net mgr deal next request! \n")
TRACE_MSG(WSP_HSM_1259_112_2_17_23_51_24_4470,"\n wsp HandleWtpDestroyedInd input error! \n")
TRACE_MSG(WSP_HSM_1274_112_2_17_23_51_24_4471,"\n wsp HandleWtpDestroyedInd: destroy active wtp hsm! \n")
TRACE_MSG(WSP_HSM_1281_112_2_17_23_51_24_4472,"\n wsp HandleWtpDestroyedInd: destroy connect wtp hsm! \n")
TRACE_MSG(WSP_HSM_1287_112_2_17_23_51_24_4473,"\n wsp HandleWtpDestroyedInd: destroy method wtp hsm! \n")
TRACE_MSG(WSP_HSM_1307_112_2_17_23_51_24_4474,"\n wsp HandleWtpDestroyedInd: to destroy wsp hsm! \n")
TRACE_MSG(WSP_HSM_1328_112_2_17_23_51_24_4475,"\n wsp HandleWspError input error! \n")
TRACE_MSG(WSP_HSM_1340_112_2_17_23_51_24_4476,"\n wsp HandleWspError abort transaction error! \n")
TRACE_MSG(WSP_HSM_1364_112_2_17_23_51_24_4477,"\n wsp HandleAuthPendingAuthRsp input error! \n")
TRACE_MSG(WSP_HSM_1495_112_2_17_23_51_24_4478,"\n wsp WSP_DealSignal recv WSP_SIG_HANDSHAKE_TIMEOUT_IND \n")
TRACE_MSG(WSP_HSM_1499_112_2_17_23_51_24_4479,"\n WSP_DealSignal unknown signal\n")
TRACE_MSG(WSP_HSM_1537_112_2_17_23_51_24_4480,"\n wsp HandleRTimeOutInd alloc error! \n")
TRACE_MSG(WSP_HSM_1563_112_2_17_23_51_24_4481,"\n wsp HandleRTimeOutInd input error! \n")
TRACE_MSG(WSP_HSM_1572_112_2_17_23_51_24_4482,"\n wsp HandleRTimeOutInd: app param error! \n")
TRACE_MSG(WSP_HSM_1581_112_2_17_23_51_24_4483,"\n wsp HandleRTimeOutInd get request fail! \n")
TRACE_MSG(WSP_HSM_1588_112_2_17_23_51_24_4484,"\n wsp HandleRTimeOutInd no active wtp hsm! \n")
TRACE_MSG(WSP_HSM_1596_112_2_17_23_51_24_4485,"\n wsp HandleRTimeOutInd alloc error! \n")
TRACE_MSG(WSP_HSM_1602_112_2_17_23_51_24_4486,"\n wsp HandleRTimeOutInd:inform wtp R timeout, handle =%x! \n")
TRACE_MSG(WSP_HSM_1626_112_2_17_23_51_24_4487,"\n wsp HandleConnectedMethodInvokeReq input error! \n")
TRACE_MSG(WSP_HSM_1645_112_2_17_23_51_24_4488,"\n wsp HandleConnectedMethodInvokeReq format request header error! \n")
TRACE_MSG(WSP_HSM_1704_112_2_17_23_51_25_4489,"\n wsp HandleConnectedMethodInvokeReq send invoke error! \n")
TRACE_MSG(WSP_HSM_1786_112_2_17_23_51_25_4490,"\n wsp HandleIdleAppReq data from cache success\n")
TRACE_MSG(WSP_HSM_2043_112_2_17_23_51_25_4491,"WSP AbortWspRequest: input invalid!")
TRACE_MSG(WSP_HSM_2051_112_2_17_23_51_25_4492,"WSP AbortWspRequest: memory alloc error!")
TRACE_MSG(WSP_HSM_2055_112_2_17_23_51_25_4493,"wsp AbortWspRequest, abort reason = %d")
TRACE_MSG(WSP_HSM_2337_112_2_17_23_51_26_4494,"WSP HandleIdleHandshake: input error fail!")
TRACE_MSG(WSP_HSM_2348_112_2_17_23_51_26_4495,"WSP HandleIdleHandshake: port = %d!")
TRACE_MSG(WSP_HSM_2353_112_2_17_23_51_26_4496,"WSP HandleIdleHandshake: create fail!")
TRACE_MSG(WSP_HSM_2364_112_2_17_23_51_26_4497,"WSP HandleIdleHandshake: host = %s!")
TRACE_MSG(WSP_HSM_2370_112_2_17_23_51_26_4498,"WSP HandleIdleHandshake: inet fail!")
TRACE_MSG(WSP_HSM_2377_112_2_17_23_51_26_4499,"WSP HandleIdleHandshake: handshake fail!")
TRACE_MSG(WSP_HSM_2417_112_2_17_23_51_26_4500,"\n wsp WSP_SecurityPostMessage recv WTLS_POSTMESSAGE_HANDSHAKE_SUCC\n")
TRACE_MSG(WSP_HSM_2422_112_2_17_23_51_26_4501,"\n wsp WSP_SecurityPostMessage recv WTLS_POSTMESSAGE_CERT_NEED_CONFIRM\n")
TRACE_MSG(WSP_HSM_2429_112_2_17_23_51_26_4502,"\n wsp WSP_SecurityPostMessage recv WTLS_POSTMESSAGE_FAIL\n")
TRACE_MSG(WSP_HSM_2434_112_2_17_23_51_26_4503,"\n wsp WSP_SecurityPostMessage recv error\n")
TRACE_MSG(WSP_HSM_2494_112_2_17_23_51_26_4504,"\n wsp HandleWaitingAbortInd: abort reason =%d! \n")
TRACE_MSG(WSP_HSM_2581_112_2_17_23_51_26_4505,"WSP IsWspHeaderAllReceived: recv all header!")
TRACE_MSG(WSP_HSM_2610_112_2_17_23_51_26_4506,"\n wsp HandleWaitingResultInd input error! \n")
TRACE_MSG(WSP_HSM_2623_112_2_17_23_51_26_4507,"WSP:HandleWaitingResultInd, active tid =%d, rsp tid = %d, data_ptr = %d, data len = %d")
TRACE_MSG(WSP_HSM_2686_112_2_17_23_51_27_4508,"WSP:HandleWaitingResultInd data error!")
TRACE_MSG(WSP_HSM_2774_112_2_17_23_51_27_4509,"\n wsp HandleWaitingResultInd occurs error = %d! \n")
TRACE_MSG(WSP_HSM_2887_112_2_17_23_51_27_4510,"WSP HandleWspContent, rsp code =%d")
TRACE_MSG(WSP_HSM_3108_112_2_17_23_51_27_4511,"\n WSP HandleDataFromCache unknown method \n")
TRACE_MSG(WSP_HSM_3235_112_2_17_23_51_28_4512,"WSP HandleCookieClearReq context_id =%d")
TRACE_MSG(WSP_HSM_3245_112_2_17_23_51_28_4513,"WSP HandleCookieClearReq HttpCookieFinalize 1 error_code = %d")
TRACE_MSG(WSP_HSM_3252_112_2_17_23_51_28_4514,"WSP HandleCookieClearReq HttpCookieFinalize 2 error_code = %d")
TRACE_MSG(WSP_HSM_3257_112_2_17_23_51_28_4515,"WSP HandleCookieClearReq HttpCookieFinalize 3 error_code = %d")
TRACE_MSG(WSP_HSM_3263_112_2_17_23_51_28_4516,"WSP HandleCookieClearReq HttpCookieFinalize 4 error_code = %d")
TRACE_MSG(WSP_HSM_3639_112_2_17_23_51_28_4517,"\n WSP_SigErrCnf unknown signal \n")
TRACE_MSG(WSP_HSM_3769_112_2_17_23_51_29_4518,"\n wsp HandleTransSendResult: error_code = %d!\n")
TRACE_MSG(WSP_HSM_3774_112_2_17_23_51_29_4519,"\n wsp HandleTransSendResult: send wsp session disconnect succ!\n")
TRACE_MSG(WSP_HSM_3790_112_2_17_23_51_29_4520,"\n wsp HandleTransSendResult: inform wsp send disconnect pdu error!\n")
TRACE_MSG(WSP_HSM_3813_112_2_17_23_51_29_4521,"\n wsp HandleTransSendResult: inform wtp send succ!\n")
TRACE_MSG(WSP_HSM_3818_112_2_17_23_51_29_4522,"\n wsp HandleTransSendResult: inform wtp send fail!\n")
TRACE_MSG(WSP_HSM_3856_112_2_17_23_51_29_4523,"WSP: HandleTransRecvSucc, data len = %d")
TRACE_MSG(WSP_HSM_3859_112_2_17_23_51_29_4524,"WSP: HandleTransRecvSucc, data len <=0, need not handle!")
TRACE_MSG(WSP_HSM_3909_112_2_17_23_51_29_4525,"WSP: HandleTransRecvSucc, fragment data len = %d")
TRACE_MSG(WSP_HSM_3952_112_2_17_23_51_29_4526,"WSP, HandleTransRecvSucc, unpack error!")
TRACE_MSG(WSP_HSM_3958_112_2_17_23_51_29_4527,"WSP, HandleTransRecvSucc,no active wtp ptr!")
TRACE_MSG(WSP_HSM_4006_112_2_17_23_51_29_4528,"wsp NotifyWspError invalid input!")
TRACE_MSG(WSP_HSM_4019_112_2_17_23_51_29_4529,"wsp NotifyWspError!")
TRACE_MSG(WSP_HSM_4052_112_2_17_23_51_29_4530,"\n wsp TransCallback NETMGR_DATA_IN_IND success\n")
TRACE_MSG(WSP_HSM_4056_112_2_17_23_51_29_4531,"\n wsp TransCallback NETTRANS_Recv error\n")
TRACE_MSG(WSP_HSM_4062_112_2_17_23_51_29_4532,"\n wsp TransCallback NETMGR_DATA_IN_IND error\n")
TRACE_MSG(WSP_HSM_4071_112_2_17_23_51_29_4533,"\n wsp TransCallback NETTRANS_SEND_RETURN_IND error\n")
TRACE_MSG(WSP_HSM_4079_112_2_17_23_51_29_4534,"\n wsp TransCallback NETTRANS_RECV_RETURN_IND success\n")
TRACE_MSG(WSP_HSM_4083_112_2_17_23_51_29_4535,"\n wsp TransCallback HandleTransRecvSucc error\n")
TRACE_MSG(WSP_HSM_4090_112_2_17_23_51_29_4536,"\n wsp TransCallback NETTRANS_RECV_RETURN_IND fail\n")
TRACE_MSG(WSP_HSM_4101_112_2_17_23_51_29_4537,"\n wsp TransCallback NETMGR_RECV_RETURN_IND error\n")
TRACE_MSG(WSP_HSM_4107_112_2_17_23_51_29_4538,"\n wsp TransCallback default error\n")
TRACE_MSG(WSP_HSM_4125_112_2_17_23_51_29_4539,"\n wsp HandleWspHsmFatalError, input error!\n")
TRACE_MSG(WSP_HSM_4129_112_2_17_23_51_29_4540,"\n wsp HandleWspHsmFatalError, error = %d \n")
TRACE_MSG(WSP_HSM_4138_112_2_17_23_51_29_4541,"\n wsp HandleWspHsmFatalError abort transaction error = %d! \n")
TRACE_MSG(WSP_REQUEST_150_112_2_17_23_51_30_4542,"\n WSP WSP_CreateRequest: header list handle = %d\n")
TRACE_MSG(WSP_REQUEST_187_112_2_17_23_51_31_4543,"\n WSP WSP_CreateRequest unknown request\n")
TRACE_MSG(WSP_REQUEST_195_112_2_17_23_51_31_4544,"\n WSP WSP_CreateRequest: request list handle =%d\n")
TRACE_MSG(WSP_REQUEST_201_112_2_17_23_51_31_4545,"\n WSP WSP_CreateRequest request id =%d\n")
TRACE_MSG(WSP_REQUEST_279_112_2_17_23_51_31_4546,"\n WSP WSP_DestroyRequest: destroy header list handle = %d\n")
TRACE_MSG(WSP_REQUEST_297_112_2_17_23_51_31_4547,"\n WSP WSP_DestroyRequest: free post body copyed by wsp\n")
TRACE_MSG(WSP_REQUEST_318_112_2_17_23_51_31_4548,"\n WSP WSP_DestroyRequest: destroy request list handle = %d\n")
TRACE_MSG(WSP_REQUEST_563_112_2_17_23_51_31_4549,"\n WSP CreatePostRequest: wsp body is file\n")
TRACE_MSG(WSP_REQUEST_586_112_2_17_23_51_32_4550,"\n WSP CreatePostRequest: wsp copy body buffer\n")
TRACE_MSG(WSP_REQUEST_591_112_2_17_23_51_32_4551,"\n WSP CreatePostRequest post buffer not copy \n")
TRACE_MSG(WSP_REQUEST_596_112_2_17_23_51_32_4552,"\n WSP CreatePostRequest post buffer null \n")
TRACE_MSG(WSP_REQUEST_1099_112_2_17_23_51_33_4553,"\n WSP_RequestFormat, need to proxyAuthenticate\n")
TRACE_MSG(WSP_REQUEST_1152_112_2_17_23_51_33_4554,"\n WSP_RequestFormat, need not to proxyAuthenticate\n")
TRACE_MSG(WSP_REQUEST_1201_112_2_17_23_51_33_4555,"\n WSP_RequestFormat, need to wwwAuthenticate\n")
TRACE_MSG(WSP_REQUEST_1249_112_2_17_23_51_33_4556,"\n WSP_RequestFormat, need not to wwwAuthenticate\n")
TRACE_MSG(WSP_REQUEST_1499_112_2_17_23_51_33_4557,"\n wsp WSP_UpdateRequest input error\n")
TRACE_MSG(WSP_REQUEST_1507_112_2_17_23_51_33_4558,"\n wsp WSP_UpdateRequest \n")
TRACE_MSG(WSP_RESPONSE_718_112_2_17_23_51_35_4559,"\n WSP ParseResponseCacheHeader unknown param \n")
TRACE_MSG(WSP_RESPONSE_1138_112_2_17_23_51_36_4560,"\n WSP WSP_ParseResponseLocationHeader request_ptr->uri_str_ptr = %s")
TRACE_MSG(WSP_SESSION_174_112_2_17_23_51_43_4561,"\n WSP WSP_DestroySession enter!\n")
TRACE_MSG(WSP_SIGNAL_85_112_2_17_23_51_43_4562,"\n WSP WSP_SendNetIndToApp send WSP_SIG_NET_PROG_IND!")
TRACE_MSG(WSP_SIGNAL_123_112_2_17_23_51_43_4563,"\n WSP WSP_SendDataIndToApp send WSP_SIG_DATA_IND!")
TRACE_MSG(WSP_SIGNAL_161_112_2_17_23_51_43_4564,"\n WSP WSP_SendHeaderIndToApp send WSP_SIG_HEADER_IND!")
TRACE_MSG(WSP_SIGNAL_196_112_2_17_23_51_43_4565,"\n WSP WSP_SendErrorIndToApp send WSP_SIG_ERROR_IND!")
TRACE_MSG(WSP_SIGNAL_231_112_2_17_23_51_44_4566,"\n WSP WSP_SendCancelCnfToApp send WSP_SIG_CANCEL_CNF!")
TRACE_MSG(WSP_SIGNAL_274_112_2_17_23_51_44_4567,"\n WSP WSP_SendGetCnfToApp send WSP_SIG_GET_CNF!")
TRACE_MSG(WSP_SIGNAL_318_112_2_17_23_51_44_4568,"\n WSP WSP_SendPostCnfToApp send WSP_SIG_POST_CNF!")
TRACE_MSG(WSP_SIGNAL_354_112_2_17_23_51_44_4569,"\n WSP WSP_SendNeedAuthToApp send WSP_SIG_NEED_AUTH_IND!")
TRACE_MSG(WSP_SIGNAL_390_112_2_17_23_51_44_4570,"\n WSP WSP_SendInitCnfToApp send WSP_SIG_INIT_CNF!")
TRACE_MSG(WSP_SIGNAL_421_112_2_17_23_51_44_4571,"\n WSP WSP_SendCloseCnfToApp send WSP_SIG_CLOSE_CNF!")
TRACE_MSG(WSP_SIGNAL_452_112_2_17_23_51_44_4572,"\n WSP WSP_SendCookieClearCnfToApp send WSP_SIG_COOKIE_CLEAR_CNF!")
TRACE_MSG(WSP_SIGNAL_484_112_2_17_23_51_44_4573,"\n WSP WSP_SendRequestIndToApp send WSP_SIG_REQUEST_ID_IND!")
TRACE_MSG(WSP_SIGNAL_520_112_2_17_23_51_44_4574,"\n WSP WSP_SendRedirectIndToApp send WSP_SIG_REDIRECT_IND")
TRACE_MSG(WSP_SIGNAL_554_112_2_17_23_51_44_4575,"\n WSP WSP_SendWtlsUntrustIndToApp send WSP_SIG_WTLS_UNTRUST_IND!")
TRACE_MSG(WSP_TEST_140_112_2_17_23_51_45_4576,"\n WSP TEST no memory \n")
TRACE_MSG(WSP_TEST_258_112_2_17_23_51_45_4577,"\n wsp TEST no memory \n")
TRACE_MSG(WSP_TEST_462_112_2_17_23_51_45_4578,"wsp get wsp init cnf")
TRACE_MSG(WSP_TEST_469_112_2_17_23_51_45_4579,"\n wsp test auth request id = %d\n")
TRACE_MSG(WSP_TEST_475_112_2_17_23_51_45_4580,"\n wsp test request id = %d\n")
TRACE_MSG(WSP_TEST_480_112_2_17_23_51_45_4581,"\n wsp test recv WSP_SIG_GET_CNF\n")
TRACE_MSG(WSP_TEST_486_112_2_17_23_51_45_4582,"\n wsp test data ind rsp code=%d,request_id =%d,len = %d\n")
TRACE_MSG(WSP_TEST_496_112_2_17_23_51_45_4583,"\n wsp test recv WSP_SIG_HEADER_IND,charset = %s,content encoding = %d, content len = %d\n")
TRACE_MSG(WSP_TEST_498_112_2_17_23_51_45_4584,"\n wsp test recv WSP_SIG_HEADER_IND, rsp_code = %d, content type =%s\n")
TRACE_MSG(WSP_TEST_502_112_2_17_23_51_45_4585,"\nwsp test recv WSP_SIG_HEADER_IND, header= %x")
TRACE_MSG(WSP_TEST_516_112_2_17_23_51_45_4586,"\n wsp test recv WSP_SIG_ERROR_IND, ERROR=%d\n")
TRACE_MSG(WSP_TEST_522_112_2_17_23_51_45_4587,"\n wsp test recv WSP_SIG_CLOSE_CNF\n")
TRACE_MSG(WSP_TEST_538_112_2_17_23_51_45_4588,"\n wsp test net prog ind,%d,%d,%d\n")
TRACE_MSG(WSP_TEST_542_112_2_17_23_51_45_4589,"\n wsp test redirect ind\n")
TRACE_MSG(WSP_TEST_546_112_2_17_23_51_45_4590,"\n wsp test recv HTTP_SIG_POST_CNF\n")
TRACE_MSG(WTP_API_113_112_2_17_23_51_46_4591,"wsp,WTP_UnpackWdpData, input error!")
TRACE_MSG(WTP_API_124_112_2_17_23_51_46_4592,"wsp,WTP_UnpackWdpData, alloc memory error!")
TRACE_MSG(WTP_API_132_112_2_17_23_51_46_4593,"wsp,WTP_UnpackWdpData, recv error pdu!")
TRACE_MSG(WTP_API_140_112_2_17_23_51_46_4594,"wsp,WTP_UnpackWdpData, ocstr create error!")
TRACE_MSG(WTP_API_148_112_2_17_23_51_46_4595,"wsp,WTP_UnpackWdpData, get char error!")
TRACE_MSG(WTP_API_167_112_2_17_23_51_46_4596,"wsp,WTP_UnpackWdpData, unpack signal pdu error!")
TRACE_MSG(WTP_HSM_319_112_2_17_23_51_47_4597,"\n wsp WTPHSM_Create wtp_hsm_ptr = %d")
TRACE_MSG(WTP_HSM_334_112_2_17_23_51_47_4598,"\n wsp Wtp_EventQueueClear input error\n")
TRACE_MSG(WTP_HSM_364_112_2_17_23_51_47_4599,"\n wsp WTPHSM_Destroy wtp_hsm_ptr = %d")
TRACE_MSG(WTP_HSM_391_112_2_17_23_51_47_4600,"\n wsp WTPHSM_Destroy: close file handle = %d")
TRACE_MSG(WTP_HSM_408_112_2_17_23_51_47_4601,"\n wsp WTPHSM_Destroy: psn list handle = %d! \n")
TRACE_MSG(WTP_HSM_415_112_2_17_23_51_47_4602,"\n wsp WTPHSM_Destroy: delete timer! \n")
TRACE_MSG(WTP_HSM_529_112_2_17_23_51_47_4603,"\n wsp QHSM_Wtp input error! \n")
TRACE_MSG(WTP_HSM_540_112_2_17_23_51_47_4604,"\n wsp QHSM_Wtp receive wtp abort request! \n")
TRACE_MSG(WTP_HSM_550_112_2_17_23_51_47_4605,"\n wsp QHSM_Wtp receive wtp rcv abort ind! \n")
TRACE_MSG(WTP_HSM_559_112_2_17_23_51_47_4606,"\n wsp QHSM_Wtp receive wtp rcv error pdu ind! \n")
TRACE_MSG(WTP_HSM_568_112_2_17_23_51_47_4607,"\n wsp QHSM_Wtp receive wtp rcv error pdu ind! \n")
TRACE_MSG(WTP_HSM_577_112_2_17_23_51_47_4608,"\n wsp QHSM_Wtp receive destroy wtp hsm req! \n")
TRACE_MSG(WTP_HSM_605_112_2_17_23_51_47_4609,"\n wsp HandleWtp_AbortReq input error! \n")
TRACE_MSG(WTP_HSM_616_112_2_17_23_51_47_4610,"\n wsp HandleWtp_AbortReq  send abort pdu error! \n")
TRACE_MSG(WTP_HSM_644_112_2_17_23_51_47_4611,"\n wsp HandleWtp_RcvAbortInd input error! \n")
TRACE_MSG(WTP_HSM_656_112_2_17_23_51_48_4612,"\n wsp HandleWtp_RcvAbortInd: destroy wtp hsm error! \n")
TRACE_MSG(WTP_HSM_664_112_2_17_23_51_48_4613,"\n wsp HandleWtp_RcvAbortInd: alloc error! \n")
TRACE_MSG(WTP_HSM_693_112_2_17_23_51_48_4614,"\n wsp HandleWtp_DestroyInd input error! \n")
TRACE_MSG(WTP_HSM_701_112_2_17_23_51_48_4615,"\n wsp HandleWtp_DestroyInd: alloc error! \n")
TRACE_MSG(WTP_HSM_732_112_2_17_23_51_48_4616,"\n wsp HandleWtp_RcvInvoke input error! \n")
TRACE_MSG(WTP_HSM_743_112_2_17_23_51_48_4617,"\n wsp HandleWtp_RcvInvoke: alloc error! \n")
TRACE_MSG(WTP_HSM_753_112_2_17_23_51_48_4618,"\n wsp HandleWtp_RcvInvoke: alloc2 error! \n")
TRACE_MSG(WTP_HSM_787_112_2_17_23_51_48_4619,"\n wsp HandleWtp_RcvErrorPdu enter! \n")
TRACE_MSG(WTP_HSM_792_112_2_17_23_51_48_4620,"\n wsp HandleWtp_RcvErrorPdu input error! \n")
TRACE_MSG(WTP_HSM_800_112_2_17_23_51_48_4621,"\n wsp HandleWtp_RcvErrorPdu: send abort error! \n")
TRACE_MSG(WTP_HSM_809_112_2_17_23_51_48_4622,"\n wsp HandleWtp_RcvErrorPdu: deatroy wtp hsm error! \n")
TRACE_MSG(WTP_HSM_817_112_2_17_23_51_48_4623,"\n wsp HandleWtp_RcvErrorPdu: alloc error! \n")
TRACE_MSG(WTP_HSM_844_112_2_17_23_51_48_4624,"\n wsp Wtp_Idle input error! \n")
TRACE_MSG(WTP_HSM_851_112_2_17_23_51_48_4625,"\n wsp Wtp_Idle: rcv invoke req! \n")
TRACE_MSG(WTP_HSM_879_112_2_17_23_51_48_4626,"\n wsp Wtp_Segsendackwait input error! \n")
TRACE_MSG(WTP_HSM_886_112_2_17_23_51_48_4627,"\n wsp Wtp_Segsendackwait: rcv wtp R timeout \n")
TRACE_MSG(WTP_HSM_895_112_2_17_23_51_48_4628,"\n wsp Wtp_Segsendackwait: rcv wtp ack ind \n")
TRACE_MSG(WTP_HSM_904_112_2_17_23_51_48_4629,"\n wsp Wtp_Segsendackwait: rcv wtp Nack ind \n")
TRACE_MSG(WTP_HSM_913_112_2_17_23_51_48_4630,"\n wsp Wtp_Segsendackwait: rcv wtp send succ cnf! \n")
TRACE_MSG(WTP_HSM_922_112_2_17_23_51_48_4631,"\n wsp Wtp_Segsendackwait: rcv wtp send succ fail! \n")
TRACE_MSG(WTP_HSM_1003_112_2_17_23_51_48_4632,"\n wsp HandleSegsendackwait_Rcvack: send ack pdu to verify tid,timer handle = %x! \n")
TRACE_MSG(WTP_HSM_1007_112_2_17_23_51_48_4633,"\n wsp HandleSegsendackwait_Rcvack: tid does not match! \n")
TRACE_MSG(WTP_HSM_1127_112_2_17_23_51_48_4634,"\n wsp HandleSegsendackwait_Rcvack: new timer handle = %x! \n")
TRACE_MSG(WTP_HSM_1159_112_2_17_23_51_49_4635,"\n wsp HandleSegsendackwait_Rcvack: 2 send all packets! \n")
TRACE_MSG(WTP_HSM_1166_112_2_17_23_51_49_4636,"\n wsp HandleSegsendackwait_Rcvack: send all packets! \n")
TRACE_MSG(WTP_HSM_1190_112_2_17_23_51_49_4637,"\n wsp HandleSegsendackwait_Rcvack: error = %d! \n")
TRACE_MSG(WTP_HSM_1226_112_2_17_23_51_49_4638,"\n wsp HandleSegsendackwait_RcvNack: missing num = %d\n")
TRACE_MSG(WTP_HSM_1236_112_2_17_23_51_49_4639,"\n wsp HandleSegsendackwait_RcvNack: pack missed psn = %d\n")
TRACE_MSG(WTP_HSM_1402_112_2_17_23_51_49_4640,"\n wsp HandleSegsendackwait_RcvNack: new timer handle = %x\n")
TRACE_MSG(WTP_HSM_1483_112_2_17_23_51_49_4641,"wsp HandleSegsendackwait_Rtimeout: new timer handle =%x")
TRACE_MSG(WTP_HSM_1678_112_2_17_23_51_50_4642,"\n wsp HandleSegsendackwait_Sendfail enter! \n")
TRACE_MSG(WTP_HSM_1737_112_2_17_23_51_50_4643,"\n wsp Wtp_Resultwait input error! \n")
TRACE_MSG(WTP_HSM_1744_112_2_17_23_51_50_4644,"\n wsp Wtp_Resultwait: rcv R timeout ind! \n")
TRACE_MSG(WTP_HSM_1753_112_2_17_23_51_50_4645,"\n wsp Wtp_Resultwait: rcv ack ind! \n")
TRACE_MSG(WTP_HSM_1762_112_2_17_23_51_50_4646,"\n wsp Wtp_Resultwait: rcv result cnf! \n")
TRACE_MSG(WTP_HSM_1771_112_2_17_23_51_50_4647,"\n wsp Wtp_Resultwait: rcv seg result cnf! \n")
TRACE_MSG(WTP_HSM_1780_112_2_17_23_51_50_4648,"\n wsp Wtp_Resultwait: rcv send succ cnf! \n")
TRACE_MSG(WTP_HSM_1789_112_2_17_23_51_50_4649,"\n wsp Wtp_Resultwait: rcv send fail cnf! \n")
TRACE_MSG(WTP_HSM_1861_112_2_17_23_51_50_4650,"wsp HandleResultwait_Rtimeout: new timer handle =%x")
TRACE_MSG(WTP_HSM_1873_112_2_17_23_51_50_4651,"\n wsp HandleResultwait_Rtimeout: rcr is max! \n")
TRACE_MSG(WTP_HSM_1945_112_2_17_23_51_50_4652,"wsp HandleResultwait_Rcvack: new timer handle = %x")
TRACE_MSG(WTP_HSM_2055_112_2_17_23_51_50_4653,"\n WSP HandleResultwait_RcvSegresult: psn list handle = %d\n")
TRACE_MSG(WTP_HSM_2069_112_2_17_23_51_50_4654,"WSP:HandleResultwait_RcvSegresult, packet offset = %d")
TRACE_MSG(WTP_HSM_2075_112_2_17_23_51_50_4655,"WSP:HandleResultwait_RcvSegresult, recv new seg result!")
TRACE_MSG(WTP_HSM_2086_112_2_17_23_51_50_4656,"WSP:HandleResultwait_RcvSegresult, sended = %d, data_len = %d, insert_pos=%d")
TRACE_MSG(WTP_HSM_2128_112_2_17_23_51_50_4657,"\n wsp HandleResultwait_RcvSegresult: gtr ttr = %d, timer handle = %x\n")
TRACE_MSG(WTP_HSM_2163_112_2_17_23_51_51_4658,"WSP:HandleResultwait_RcvSegresult, result to app send len =%d")
TRACE_MSG(WTP_HSM_2174_112_2_17_23_51_51_4659,"\n wsp HandleResultwait_RcvSegresult: packet is not continue send Nack\n")
TRACE_MSG(WTP_HSM_2226_112_2_17_23_51_51_4660,"\n wsp HandleResultwait_RcvSegresult: packet is not continue send Nack\n")
TRACE_MSG(WTP_HSM_2239_112_2_17_23_51_51_4661,"\n wsp HandleResultwait_RcvSegresult: has receive last packet\n")
TRACE_MSG(WTP_HSM_2243_112_2_17_23_51_51_4662,"\n wsp HandleResultwait_RcvSegresult: event is not last packet,but last packet has received already!\n")
TRACE_MSG(WTP_HSM_2327_112_2_17_23_51_51_4663,"\n wsp SendResultIndtoWsp input error! \n")
TRACE_MSG(WTP_HSM_2331_112_2_17_23_51_51_4664,"WSP SendResultIndtoWsp, send result to wsp ,len = %d")
TRACE_MSG(WTP_HSM_2336_112_2_17_23_51_51_4665,"\n wsp SendResultIndtoWsp alloc error! \n")
TRACE_MSG(WTP_HSM_2354_112_2_17_23_51_51_4666,"\n wsp SendResultIndtoWsp alloc 2 error! \n")
TRACE_MSG(WTP_HSM_2400_112_2_17_23_51_51_4667,"\n wsp HandleResultwait_Rcvresult input error! \n")
TRACE_MSG(WTP_HSM_2411_112_2_17_23_51_51_4668,"\n wsp HandleResultwait_Rcvresult: tid does not match! \n")
TRACE_MSG(WTP_HSM_2424_112_2_17_23_51_51_4669,"\n WSP HandleResultwait_Rcvresult: psn list handle = %d\n")
TRACE_MSG(WTP_HSM_2435_112_2_17_23_51_51_4670,"\n wsp HandleResultwait_Rcvresult:NOT_LAST_PACKET,has exist 0 psn! \n")
TRACE_MSG(WTP_HSM_2446_112_2_17_23_51_51_4671,"WSP:HandleResultwait_Rcvresult, packet offset = %d")
TRACE_MSG(WTP_HSM_2463_112_2_17_23_51_51_4672,"WSP:HandleResultwait_Rcvresult, inser= %d, sended = %d, datalen=%d")
TRACE_MSG(WTP_HSM_2484_112_2_17_23_51_51_4673,"\n wsp HandleResultwait_Rcvresult: has receive last packet\n")
TRACE_MSG(WTP_HSM_2488_112_2_17_23_51_51_4674,"\n wsp HandleResultwait_Rcvresult: event is not last packet,but last packet has received already!\n")
TRACE_MSG(WTP_HSM_2545_112_2_17_23_51_51_4675,"\n wsp HandleResultwait_Rcvresult:NOT_LAST_PACKET,new timer handle = %x! \n")
TRACE_MSG(WTP_HSM_2615_112_2_17_23_51_51_4676,"\n WSP HandleResultwait_Rcvresult: psn 2 list handle = %d\n")
TRACE_MSG(WTP_HSM_2626_112_2_17_23_51_51_4677,"\n wsp HandleResultwait_Rcvresult:LAST_PACKET_OF_GROUP,has exist 0 psn! \n")
TRACE_MSG(WTP_HSM_2724_112_2_17_23_51_52_4678,"\n wsp HandleResultwait_Rcvresult:LAST_PACKET_of_group,new timer handle = %x! \n")
TRACE_MSG(WTP_HSM_2769_112_2_17_23_51_52_4679,"\n wsp HandleResultwait_Rcvresult: error_code =%d! \n")
TRACE_MSG(WTP_HSM_2797_112_2_17_23_51_52_4680,"\n wsp WTP_SendDisconnectPdu input error! \n")
TRACE_MSG(WTP_HSM_2805_112_2_17_23_51_52_4681,"\n wsp WTP_SendDisconnectPdu: pack invoke pdu header error! \n")
TRACE_MSG(WTP_HSM_2814_112_2_17_23_51_52_4682,"\n wsp WTP_SendDisconnectPdu: alloc error! \n")
TRACE_MSG(WTP_HSM_3020_112_2_17_23_51_52_4683,"\n wsp HandleWtpFatalError input error! \n")
TRACE_MSG(WTP_HSM_3024_112_2_17_23_51_52_4684,"\n wsp HandleWtpFatalError enter, error_code = %d! \n")
TRACE_MSG(WTP_HSM_3036_112_2_17_23_51_52_4685,"\n wsp HandleWtpFatalError,alloc error!\n")
TRACE_MSG(WTP_HSM_3092_112_2_17_23_51_52_4686,"\n wsp BeginSarInvoke: last packet psn = %d! \n")
TRACE_MSG(WTP_HSM_3096_112_2_17_23_51_52_4687,"\n wsp BeginSarInvoke: user data are too long,max than 255! \n")
TRACE_MSG(WTP_HSM_3224_112_2_17_23_51_53_4688,"\n wsp BeginSarInvoke: timer handle  = %x! \n")
TRACE_MSG(WTP_HSM_3623_112_2_17_23_51_53_4689,"\n wsp SendAllInvoke: timer handle  = %x! \n")
TRACE_MSG(WTP_HSM_3740_112_2_17_23_51_54_4690,"WSP:IsPackageContinue, list handle is pnull!")
TRACE_MSG(WTP_HSM_3750_112_2_17_23_51_54_4691,"WSP:IsPackageContinue, first node value =%d!")
TRACE_MSG(WTP_HSM_3762_112_2_17_23_51_54_4692,"WSP:IsPackageContinue, compare, first =%d, second = %d!")
TRACE_MSG(WTP_HSM_3814_112_2_17_23_51_54_4693,"\n wsp InsertPsnToList:enter psn = %d! num=%d \n")
TRACE_MSG(WTP_HSM_3820_112_2_17_23_51_54_4694,"\n wsp InsertPsnToList fist psn")
TRACE_MSG(WTP_HSM_3837_112_2_17_23_51_54_4695,"\n wsp InsertPsnToList: 0!=current node")
TRACE_MSG(WTP_HSM_3840_112_2_17_23_51_54_4696,"\n wsp InsertPsnToList:current psn = %d! ,insert = %d\n")
TRACE_MSG(WTP_HSM_3866_112_2_17_23_51_54_4697,"\n wsp InsertPsnToList, insert to next node!\n")
TRACE_MSG(WTP_HSM_3875_112_2_17_23_51_54_4698,"\n WSP InsertPsnToList, insert to last node!\n")
TRACE_MSG(WTP_HSM_3904_112_2_17_23_51_54_4699,"\n WSP InsertPsnToList, insert to first node!\n")
TRACE_MSG(WTP_HSM_3917_112_2_17_23_51_54_4700,"\n wsp InsertPsnToList:over psn = %d! \n")
TRACE_MSG(WTP_HSM_3927_112_2_17_23_51_54_4701,"\n wsp InsertPsnToList:over2 psn = %d, error_code = %d \n")
TRACE_MSG(WTP_HSM_3951_112_2_17_23_51_54_4702,"WSP:WTP_IsPackageExist, packet is existed! psn = %d")
TRACE_MSG(WTP_HSM_4269_112_2_17_23_51_55_4703,"\n wsp WTP_EventDestroy input error! \n")
TRACE_MSG(WTP_HSM_4329_112_2_17_23_51_55_4704,"WSP:WSP_EventDestroy.sig code error!")
TRACE_MSG(WTP_HSM_4363_112_2_17_23_51_55_4705,"\n wsp SendAbortPdu: abort tid =%d! \n")
TRACE_MSG(WTP_HSM_4368_112_2_17_23_51_55_4706,"\n wsp SendAbortPdu: pack abort pdu error! \n")
TRACE_MSG(WTP_HSM_4401_112_2_17_23_51_55_4707,"\n wsp SendAbortPdu:pack for trans error! \n")
TRACE_MSG(WTP_HSM_4418_112_2_17_23_51_55_4708,"\n wsp SendAbortPdu: trans send error! \n")
TRACE_MSG(WTP_HSM_4543_112_2_17_23_51_55_4709,"WSP:WTP_HandleExistPsn, not gtr ttr, need not handle!")
TRACE_MSG(WTP_HSM_4550_112_2_17_23_51_55_4710,"WSP:WTP_HandleExistPsn, last packet of message!!")
TRACE_MSG(WTP_HSM_4556_112_2_17_23_51_55_4711,"WSP:WTP_HandleExistPsn, last packet of group!!")
TRACE_MSG(WTP_HSM_4579_112_2_17_23_51_55_4712,"WSP:WTP_HandleExistPsn, rcv gtr ttr, send result to wsp.len = %d")
TRACE_MSG(WTP_HSM_4590_112_2_17_23_51_55_4713,"WSP:WTP_HandleExistPsn, last packet of message, destroy wtp hsm!!")
TRACE_MSG(WTP_PDU_596_112_2_17_23_51_57_4714,"\n wsp WTP_PackNack: node psn = %d, i = %d\n")
TRACE_MSG(WTP_PDU_603_112_2_17_23_51_57_4715,"\n wsp WTP_PackNack: missed num = %d! \n")
TRACE_MSG(WTP_PDU_774_112_2_17_23_51_57_4716,"WTP WTP_PackAck : error , tpi len is too long and large than 3.")
TRACE_MSG(WTP_PDU_1815_112_2_17_23_51_59_4717,"UnpackInvokePdu : The CON is not 0 in the arrived PUD head.There are TPIs but you have no procedure")
TRACE_MSG(WTP_PDU_2186_112_2_17_23_52_0_4718,"WSP:UnpackSegResultPdu, exist psn num = %d")
TRACE_MSG(WTP_PDU_2190_112_2_17_23_52_0_4719,"WSP:UnpackSegResultPdu, exist 1 psn num = %d")
TRACE_MSG(WTP_PDU_2324_112_2_17_23_52_0_4720,"wsp: WTP_UnpackSignalPdu, receive unkonwn pdu type")
TRACE_MSG(WSP_HEADERS_913_112_2_17_23_52_3_4721,"unpack_well_known_field, error")
TRACE_MSG(WSP_HEADERS_919_112_2_17_23_52_3_4722,"unpack_well_known_field, name is null error")
TRACE_MSG(WSP_HEADERS_1113_112_2_17_23_52_3_4723,"wsp_headers_unpack: there is no content type!")
TRACE_MSG(WSP_HEADERS_1731_112_2_17_23_52_4_4724,"WSP: pack_short_integer, integer invalid!")
TRACE_MSG(WSP_LIST_124_112_2_17_23_52_7_4725,"WSP: list_insert, pos invalid!")
TRACE_MSG(WSP_LIST_382_112_2_17_23_52_8_4726,"WSP: delete_items_from_list, input invalid!")
TRACE_MSG(WSP_LIST_388_112_2_17_23_52_8_4727,"WSP: delete_items_from_list 3333, input invalid!")
TRACE_MSG(WSP_OCTSTR_219_112_2_17_23_52_8_4728,"WSP: wspheader_octstr_append_char, input invalid!")
TRACE_MSG(WSP_OCTSTR_381_112_2_17_23_52_9_4729,"WSP: wspheader_octstr_copy_real, input invalid!")
TRACE_MSG(WSP_OCTSTR_422_112_2_17_23_52_9_4730,"WSP: wspheader_octstr_search_char, input invalid!")
TRACE_MSG(WSP_PARSE_232_112_2_17_23_52_10_4731,"WSP: parse_set_error, input invalid!")
TRACE_MSG(WTLS_ALERT_67_112_2_17_23_52_12_4732,"WTLS_AlertSend : invalid input")
TRACE_MSG(WTLS_ALERT_74_112_2_17_23_52_12_4733,"WTLS_AlertSend : PDU create fail")
TRACE_MSG(WTLS_ALERT_87_112_2_17_23_52_12_4734,"WTLS_AlertSend :pdu pack fail")
TRACE_MSG(WTLS_ALERT_94_112_2_17_23_52_12_4735,"WTLS_AlertSend :payload pack fail")
TRACE_MSG(WTLS_ALERT_126_112_2_17_23_52_12_4736,"WTLS_AlertEventHandle : invalid input")
TRACE_MSG(WTLS_ALERT_137_112_2_17_23_52_12_4737,"WTLS_ALERTEVENTHANDLE :error event create fail")
TRACE_MSG(WTLS_ALERT_150_112_2_17_23_52_12_4738,"WTLS_ALERTEVENTHANDLE :warning event create fail")
TRACE_MSG(WTLS_ALGOPROCESS_192_112_2_17_23_52_13_4739,"WTLSALGOPROC_HASH: encounter problem =%d\n")
TRACE_MSG(WTLS_ALGOPROCESS_319_112_2_17_23_52_14_4740,"WtlsHmacHash: encounter problem=%d\n")
TRACE_MSG(WTLS_ALGOPROCESS_422_112_2_17_23_52_14_4741,"WtlsPHash: encounter problem =%d\n")
TRACE_MSG(WTLS_ALGOPROCESS_500_112_2_17_23_52_14_4742,"WTLSALGOPROC_CalculateWTLSPrf: encounter problem=%d\n")
TRACE_MSG(WTLS_ALGOPROCESS_559_112_2_17_23_52_14_4743,"WTLSALGOPROC_DataEncrypt:encounter problem =%d \n")
TRACE_MSG(WTLS_ALGOPROCESS_696_112_2_17_23_52_14_4744,"WTLSALGOPROC_CalculateServerKeyBlock: encounter problem=%d\n")
TRACE_MSG(WTLS_ALGOPROCESS_838_112_2_17_23_52_15_4745,"WTLSALGOPROC_CalculateClientKeyBlock: encounter problem=%d\n")
TRACE_MSG(WTLS_ALGOPROCESS_1184_112_2_17_23_52_15_4746,"WTLSALGOPROC_Encrypt: encounter error=%d \n")
TRACE_MSG(WTLS_ALGOPROCESS_1505_112_2_17_23_52_16_4747,"WTLSALGOPROC_Decrypt: encounter problem=%d\n")
TRACE_MSG(WTLS_ALGOPROCESS_1627_112_2_17_23_52_16_4748,"WTLSALGOPROC_DecryptPduList: encounter problem=%d\n")
TRACE_MSG(WTLS_ALGOPROCESS_1771_112_2_17_23_52_17_4749,"WtlsVerifyMac: encounter problem= %d\n")
TRACE_MSG(WTLS_ALGOPROCESS_1838_112_2_17_23_52_17_4750,"WTLSALGOPROC_FinishedVerify: encounter problem= %d\n")
TRACE_MSG(WTLS_API_119_112_2_17_23_52_17_4751,"WTLS_AsyncRegCallback:the handle not exist\n")
TRACE_MSG(WTLS_API_126_112_2_17_23_52_17_4752,"WTLS_Handshake: the handler not registered\n")
TRACE_MSG(WTLS_API_176_112_2_17_23_52_17_4753,"WTLS_Create : input is invalid\n")
TRACE_MSG(WTLS_API_184_112_2_17_23_52_17_4754,"WTLS_create: create machine fail!\n")
TRACE_MSG(WTLS_API_212_112_2_17_23_52_17_4755,"WTLS_AsyncRegCallback:the handle not exist\n")
TRACE_MSG(WTLS_API_220_112_2_17_23_52_17_4756,"WTLS_Handshake: the handler not registered\n")
TRACE_MSG(WTLS_API_228_112_2_17_23_52_17_4757,"WTLS_Close : destroy machine handle\n")
TRACE_MSG(WTLS_API_251_112_2_17_23_52_17_4758,"WTLS_Handshake: the input is invalid\n")
TRACE_MSG(WTLS_API_259_112_2_17_23_52_17_4759,"WTLS_Handshake: the handler not registered\n")
TRACE_MSG(WTLS_API_283_112_2_17_23_52_17_4760,"WTLS_Handshake: client hello event create fail\n")
TRACE_MSG(WTLS_API_319_112_2_17_23_52_17_4761,"WTLS_DecryptPasser: the input is invalid\n")
TRACE_MSG(WTLS_API_327_112_2_17_23_52_17_4762,"WTLS_DecryptPasser: handler not registered\n")
TRACE_MSG(WTLS_API_392_112_2_17_23_52_18_4763,"WTLS_DecryptPasser : encounter problem\n")
TRACE_MSG(WTLS_API_422_112_2_17_23_52_18_4764,"WTLS_EncryptPasser: input invalid\n")
TRACE_MSG(WTLS_API_430_112_2_17_23_52_18_4765,"WTLS_EncryptPasser: handler not registered\n")
TRACE_MSG(WTLS_API_473_112_2_17_23_52_18_4766,"WTLS_EncryptPasser: error code\n")
TRACE_MSG(WTLS_API_498_112_2_17_23_52_18_4767,"WTLS_UserCnfCert: input invalid\n")
TRACE_MSG(WTLS_API_506_112_2_17_23_52_18_4768,"WTLS_UserCnfCert: handler not registered\n")
TRACE_MSG(WTLS_API_518_112_2_17_23_52_18_4769,"WTLS_UserCnf: event create fail\n")
TRACE_MSG(WTLS_API_534_112_2_17_23_52_18_4770,"WTLS_UserCnf: error_code \n")
TRACE_MSG(WTLS_API_556_112_2_17_23_52_18_4771,"WTLS_AddMachineToTable: WTLS_handle is invalid")
TRACE_MSG(WTLS_API_587_112_2_17_23_52_18_4772,"WTLS_DeleteMachineFromTable: input is invalid\n")
TRACE_MSG(WTLS_API_665_112_2_17_23_52_18_4773,"WTLS_DeleteMachineFromTable: input is invalid\n")
TRACE_MSG(WTLS_API_827_112_2_17_23_52_18_4774,"WTLS_LoadSession: list_ptr is invalid")
TRACE_MSG(WTLS_CERT_117_112_2_17_23_52_19_4775,"WTLSCERT_LoadCert:encounter problem\n")
TRACE_MSG(WTLS_EVENT_370_112_2_17_23_52_20_4776,"WTLS_EventCreate: malloc fail")
TRACE_MSG(WTLS_HANDSHAKE_261_112_2_17_23_52_21_4777,"get_DhKeys:encounter problem=%d\n")
TRACE_MSG(WTLS_HANDSHAKE_442_112_2_17_23_52_21_4778,"WTLSHAND_GetDhPubKey:encounter problem=%d\n")
TRACE_MSG(WTLS_HANDSHAKE_616_112_2_17_23_52_21_4779,"WTLS_HANDRECEIVED DATA :error_code =%d\n")
TRACE_MSG(WTLS_HANDSHAKE_683_112_2_17_23_52_21_4780,"WTLS_EncryptUserDataToSend: encounter problem=%d\n")
TRACE_MSG(WTLS_HANDSHAKE_843_112_2_17_23_52_22_4781,"WTLSHAND_SendClientHello : encounters error =%d\n")
TRACE_MSG(WTLS_HANDSHAKE_1218_112_2_17_23_52_22_4782,"WTLSHAND_HandleServerHello : encounters error =%d\n")
TRACE_MSG(WTLS_HANDSHAKE_1383_112_2_17_23_52_23_4783,"WTLSHAND_HandleClientExchange : encounters erro =%d\n")
TRACE_MSG(WTLS_HANDSHAKE_1516_112_2_17_23_52_23_4784,"SendChangeCipherAndFinshedMessage: encounters error=%d\n")
TRACE_MSG(WTLS_HANDSHAKE_1810_112_2_17_23_52_24_4785,"WTLSHAND_HandleServerHelloFinish: encounter problem=%d\n")
TRACE_MSG(WTLS_HANDSHAKE_1966_112_2_17_23_52_24_4786,"WTLSHAND_SendClientFinish: encounter problem =%d\n")
TRACE_MSG(WTLS_HANDSHAKE_2109_112_2_17_23_52_24_4787,"WTLSHAND_HandleServerCCFinish: encounter problem=%d \n")
TRACE_MSG(WTLS_MACHINE_122_112_2_17_23_52_25_4788,"WTLSMACHINE_MachineInitialize:encounter problem \n")
TRACE_MSG(WTLS_MACHINE_159_112_2_17_23_52_25_4789,"WTLSMACHINE_Create: initial fail \n")
TRACE_MSG(WTLS_MACHINE_165_112_2_17_23_52_25_4790,"WTLSMACHINE_Create: alloc fail \n")
TRACE_MSG(WTLS_PDU_116_112_2_17_23_52_26_4791,"WTLSPDU_CreatePdu :encounter problem\n")
TRACE_MSG(WTLS_PDU_280_112_2_17_23_52_26_4792,"WTLSPDU_PackPdu: encounter problem =%d\n")
TRACE_MSG(WTLS_PDU_524_112_2_17_23_52_27_4793,"WTLSPDU_PackWtlsHandShakePdu:encounter problem =%d\n")
TRACE_MSG(WTLS_PDU_811_112_2_17_23_52_27_4794,"WTLSPDU_AddPdu: encounter problem=%d\n")
TRACE_MSG(WTLS_PDU_910_112_2_17_23_52_27_4795,"WTLS_PayloadPack: encounter problem =%d\n")
TRACE_MSG(WTLS_PDU_1055_112_2_17_23_52_28_4796,"WTLSPDU_UnpackPDU :error= %d\n")
TRACE_MSG(WTLS_PDU_1220_112_2_17_23_52_28_4797,"WTLSPDU_UnpackWtlsHandshakePdu : encounter problem=%d\n")
TRACE_MSG(WTLS_PDU_1345_112_2_17_23_52_28_4798,"WTLSPDU_PayloadUnpack encounter problem =%d\n")
TRACE_MSG(WTLS_PDU_1530_112_2_17_23_52_29_4799,"WTLS_DATAGRAMUNPACK: event create fail")
TRACE_MSG(WTLS_PDU_1537_112_2_17_23_52_29_4800,"WTLS_DATAGRAMUNPACK: payloadlist unpack fail")
TRACE_MSG(WTLS_PDUSUPPORT_442_112_2_17_23_52_29_4801,"WTLSPDUS_UnpackServerHello : encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_562_112_2_17_23_52_30_4802,"WTLSPDUS_PackKeyexSuiteList: encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_646_112_2_17_23_52_30_4803,"WTLSPDUS_PackBulkCipherSuiteList:encounter problem =%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_695_112_2_17_23_52_30_4804,"WTLSPDUS_PackCompressionMethodList : encounter problem =%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_752_112_2_17_23_52_30_4805,"WTLSPDUS_PackRandom : encounter problem =%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_914_112_2_17_23_52_30_4806,"WTLSPDUS_WtlsGetRandom : encounter error=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_979_112_2_17_23_52_31_4807,"WTLSPDUS_CreateRandom: encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_1142_112_2_17_23_52_31_4808,"WTLSPDUS_GetEncryptedSecret: encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_1244_112_2_17_23_52_31_4809,"WTLSPDUS_CreateClientKeyexSuite : encounter problem =%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_1313_112_2_17_23_52_31_4810,"WTLS_CREATECIPHERSUITE LIST FAIL")
TRACE_MSG(WTLS_PDUSUPPORT_1475_112_2_17_23_52_31_4811,"WTLSPDUS_UnPackWCert:encounter problem =%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_1565_112_2_17_23_52_32_4812,"WTLSPDUS_Unpack_Signature : encounter problem=%d \n")
TRACE_MSG(WTLS_PDUSUPPORT_1674_112_2_17_23_52_32_4813,"WTLSPDUS_UnpackECPubKey:encounter problem =%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_1748_112_2_17_23_52_32_4814,"WTLSPDUS_UnpackPubKey : encounter problem =%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_1843_112_2_17_23_52_32_4815,"WTLSPDUS_UnpackParamSpec : encounter problem =%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_1931_112_2_17_23_52_32_4816,"WTLSPDUS_PackOctstr16: encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_1974_112_2_17_23_52_32_4817,"WTLSPDUS_UnpackOctstr16: encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_2025_112_2_17_23_52_33_4818,"WTLSPDUS_UnpackDHParams : encounter problem=%d \n")
TRACE_MSG(WTLS_PDUSUPPORT_2139_112_2_17_23_52_33_4819,"WTLSPDUS_UnpackECParams encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_2273_112_2_17_23_52_33_4820,"WTLSPDUS_UnpackWtlsIdentify:encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_2745_112_2_17_23_52_34_4821,"WtlsPackDhParams:encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_2981_112_2_17_23_52_34_4822,"WTLSPDUS_PackParamSpec:encounter problem=%d\n")
TRACE_MSG(WTLS_PDUSUPPORT_3044_112_2_17_23_52_35_4823,"WtlsPackKeyExSuite: encounter problem\n")
TRACE_MSG(XMLEXPATIF_384_112_2_17_23_52_36_4824,"\n\nXML expat: (%d):%s\n")
TRACE_MSG(XMLEXPATIF_680_112_2_17_23_52_36_4825,"XML ExpatRealloc len err, len:%d, new_size:%d")
TRACE_MSG(XMLEXPATIF_686_112_2_17_23_52_36_4826,"XML ExpatRealloc, malloc p2 err!")
TRACE_MSG(XMLEXPATIF_905_112_2_17_23_52_37_4827,"XML ConvertUtf8WithLenToUCS2 malloc try_buff_ptr fail!")
TRACE_MSG(XMLEXPATIF_918_112_2_17_23_52_37_4828,"XML ConvertUtf8WithLenToUCS2 malloc dest_ptr fail!")
TRACE_MSG(XMLEXPATIF_967_112_2_17_23_52_37_4829,"XML ConvertUtf8ArrayToUCS2Array malloc ucs_arr fail!")
TRACE_MSG(XMLPARSE_6341_112_2_17_23_52_50_4830,"XML ExpatRealloc len err, len:%d, new_size:%d")
TRACE_MSG(XMLPARSE_6347_112_2_17_23_52_50_4831,"XML ExpatRealloc, malloc p2 err!")
TRACE_MSG(XMLTEST_52_112_2_17_23_52_53_4832,"\n\nSAX.startElementNs")
TRACE_MSG(XMLTEST_90_112_2_17_23_52_53_4833,"\n\nSAX.startElementNs")
END_TRACE_MAP(DAPS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _DAPS_TRC_H_


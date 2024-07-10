/******************************************************************************
 ** File Name:      rtos_trc.h                                         *
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
//trace_id:14
#ifndef _RTOS_TRC_H_
#define _RTOS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define SCI_API_227_112_2_18_0_49_28_0 "ERROR: The SDL TMID pointer is invalid !"
#define SCI_API_316_112_2_18_0_49_28_1 "ERROR: The SDL TMID pointer is invalid. !"
#define SCI_API_369_112_2_18_0_49_28_2 "ERROR: The SDL TMID pointer is invalid. !"
#define SCI_API_419_112_2_18_0_49_28_3 "ERROR: The SDL TMID pointer is invalid. !"
#define SCI_BLKMEM_101_112_2_18_0_49_29_4 "BLKMEM_Register: Module %d"
#define SCI_MEM_226_112_2_18_0_49_34_5 "Memory memory_ptr 0x%8x has been destroied! It be malloced by %s on %d line ,size is %d"
#define SCI_MEM_642_112_2_18_0_49_35_6 "SCI_GetByteHeapAvailableSpace:No Base Heap\r\n"
#define SCI_MEM_648_112_2_18_0_49_35_7 "SCI_GetByteHeapAvailableSpace:No this Heap\r\n"
#define SCI_MEM_745_112_2_18_0_49_36_8 "\nCould not get information until define macro __MEMORY_DEBUG__\n"
#define SCI_MEM_756_112_2_18_0_49_36_9 "\nAllocated memory info:"
#define SCI_MEM_758_112_2_18_0_49_36_10 "\n\tNo.      Size     FileName (Line)"
#define SCI_MEM_782_112_2_18_0_49_36_11 "nt%-8ld %-8ld %s (Line %ld)"
#define SCI_MEM_796_112_2_18_0_49_36_12 "\nCould not get SCI_GetMemInfoCount until define macro __MEMORY_DEBUG__\n"
#define SCI_MEM_811_112_2_18_0_49_36_13 "SHELL_memLeakCheck: Memory Debug List Error\r\n"
#define SCI_MEM_848_112_2_18_0_49_36_14 "SCI_MemInfoGet:item_offset=%d\r\n"
#define SCI_MEM_875_112_2_18_0_49_36_15 "SCI_MemInfoGet: Is List End\r\n"
#define SCI_MEM_914_112_2_18_0_49_36_16 "SCI_MemInfoGet:MEM_INFO_END\r\n"
#define SCI_MEM_924_112_2_18_0_49_36_17 "SCI_MemInfoGet:Buffer Len:0x%X\r\n"
#define SCI_MEM_930_112_2_18_0_49_36_18 "\nCould not get information until define macro __MEMORY_DEBUG__\n"
#define SERVER_480_112_2_18_0_49_43_19 "Server: time to delete the server(%d)!\n"
#define SERVER_502_112_2_18_0_49_43_20 "Server: time to delete the client(0x%x)!\n"
#define SERVER_604_112_2_18_0_49_43_21 "Server: server link list error!\n"
#define SERVER_619_112_2_18_0_49_43_22 "Server: Delete the server(%d) immediately!\n"
#define SERVER_635_112_2_18_0_49_43_23 "Server: server(%d) deletion postponed!\n"
#define SERVER_872_112_2_18_0_49_44_24 "Server: Delete the Client(0x%x) immediately!\n"
#define SERVER_875_112_2_18_0_49_44_25 "Server: Client(0x%x) deletion postponed!\n"
#define THREADX_APPMEM_889_112_2_18_0_49_46_26 "[SCI_CreateMemPool]: name:%s,  addr=0x%08X, size=0x%08X, unit size = 0x%X,  pool size=0x%X, watermark=0x%08Xrn"
#define THREADX_APPMEM_920_112_2_18_0_49_46_27 "[SCI_CreateMemPool]: Get a vaild Entry:0x%08X\r\n"
#define THREADX_APPMEM_943_112_2_18_0_49_46_28 "[SCI_CreateMemPool]: Create Pool,addr=0x%08X, size=0x%X\r\n"
#define THREADX_APPMEM_956_112_2_18_0_49_46_29 "[SCI_CreateMemPool]: Create Heap,addr=0x%08X, size=0x%X\r\n"
#define THREADX_APPMEM_971_112_2_18_0_49_46_30 "[SCI_CreateMemPool]: Mem handle=0x%08X\r\n"
#define THREADX_APPMEM_990_112_2_18_0_49_46_31 "[SCI_DeleteMemPool]: Mem handle=0x%08X, is_force_delete=0x%X\r\n"
#define THREADX_APPMEM_1016_112_2_18_0_49_47_32 "[SCI_DeleteMemPool]: Mem Pool Is using,delete fail, Total=0x%08X, avalable=0x%08Xrn"
#define THREADX_APPMEM_1032_112_2_18_0_49_47_33 "[SCI_DeleteMemPool]: Mem Heap Is using, delete fail, Total=0x%08X, avalable=0x%08Xrn"
#define THREADX_APPMEM_1041_112_2_18_0_49_47_34 "[SCI_DeleteMemPool]: Start Delete Mem Pool\r\n"
#define THREADX_APPMEM_1053_112_2_18_0_49_47_35 "[SCI_DeleteMemPool]: Start Delete Mem Heap\r\n"
#define THREADX_APPMEM_1064_112_2_18_0_49_47_36 "[SCI_DeleteMemPool]: Delete Complete\r\n"
#define THREADX_APPMEM_1241_112_2_18_0_49_47_37 "Memory memory_ptr 0x%8x has been destroied! It be malloced by %s on %d line ,size is %d"
#define THREADX_APPMEM_1399_112_2_18_0_49_48_38 "\nCould not get information until define macro SCI_MEMORY_DEBUG\n"
#define THREADX_MEM_782_112_2_18_0_49_49_39 "Memory memory_ptr 0x%8x has been destroied! It be malloced by %s on %d line ,size is %d"
#define THREADX_MEM_1082_112_2_18_0_49_50_40 "\nCould not get information until define macro SCI_MEMORY_DEBUG\n"
#define THREADX_MEM_1093_112_2_18_0_49_50_41 "\nAllocated memory info:"
#define THREADX_MEM_1095_112_2_18_0_49_50_42 "\n\tNo.      Size     FileName (Line)"
#define THREADX_MEM_1119_112_2_18_0_49_50_43 "nt%-8ld %-8ld %s (Line %ld)"
#define THREADX_MEM_1134_112_2_18_0_49_50_44 "\nCould not get information until define macro SCI_MEMORY_DEBUG\n"
#define THREADX_MEM_1150_112_2_18_0_49_50_45 "SHELL_memLeakCheck: Memory Debug List Error\r\n"
#define THREADX_MEM_1187_112_2_18_0_49_50_46 "SCI_MemInfoGet:item_offset=%d\r\n"
#define THREADX_MEM_1214_112_2_18_0_49_50_47 "SCI_MemInfoGet: Is List End\r\n"
#define THREADX_MEM_1253_112_2_18_0_49_50_48 "SCI_MemInfoGet:MEM_INFO_END\r\n"
#define THREADX_MEM_1263_112_2_18_0_49_50_49 "SCI_MemInfoGet:Buffer Len:0x%X\r\n"
#define THREADX_MEM_1269_112_2_18_0_49_50_50 "\nCould not get information until define macro SCI_MEMORY_DEBUG\n"
#define THREADX_MEM_1289_112_2_18_0_49_50_51 "\nCould not get information until define macro SCI_MEMORY_DEBUG\n"
#define THREADX_MEM_1293_112_2_18_0_49_50_52 "\nMemory Pool Information:"
#define THREADX_MEM_1296_112_2_18_0_49_50_53 "\n\tPool_Name           Start_Addr   End_Addr     Total_Num  Max_Used   Avail_Num"
#define THREADX_MEM_1306_112_2_18_0_49_50_54 "nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld"
#define THREADX_OS_1492_112_2_18_0_49_54_55 "RTOS:SCI_DeleteThread dest thread is not exist! ID:%d"
#define THREADX_OS_1744_112_2_18_0_49_55_56 "RTOS:SCI_TerminateThread dest thread is not exist! ID:%d"
#define THREADX_OS_1825_112_2_18_0_49_55_57 "RTOS:SCI_ChangeThreadPriority dest thread isnot exist! ID:%d"
#define THREADX_OS_1833_112_2_18_0_49_55_58 "RTOS:SCI_ChangeThreadPriority dest thread isnot exist! ID:%d"
#define THREADX_OS_2210_112_2_18_0_49_56_59 "RTOS:SCI_SetThreadEvent dest thread is not exist! ID:%d"
#define THREADX_OS_2216_112_2_18_0_49_56_60 "RTOS:SCI_SetThreadEvent dest thread is not exist! ID:%d"
#define THREADX_OS_2989_112_2_18_0_49_57_61 "SCI_ActiveTimer: The timer is active now, Could not active again!!!"
#define SCI_API_227_112_2_18_0_50_2_62 "ERROR: The SDL TMID pointer is invalid !"
#define SCI_API_316_112_2_18_0_50_2_63 "ERROR: The SDL TMID pointer is invalid. !"
#define SCI_API_369_112_2_18_0_50_2_64 "ERROR: The SDL TMID pointer is invalid. !"
#define SCI_API_419_112_2_18_0_50_2_65 "ERROR: The SDL TMID pointer is invalid. !"
#define SERVER_480_112_2_18_0_50_9_66 "Server: time to delete the server(%d)!\n"
#define SERVER_502_112_2_18_0_50_9_67 "Server: time to delete the client(0x%x)!\n"
#define SERVER_604_112_2_18_0_50_9_68 "Server: server link list error!\n"
#define SERVER_619_112_2_18_0_50_9_69 "Server: Delete the server(%d) immediately!\n"
#define SERVER_635_112_2_18_0_50_9_70 "Server: server(%d) deletion postponed!\n"
#define SERVER_872_112_2_18_0_50_10_71 "Server: Delete the Client(0x%x) immediately!\n"
#define SERVER_875_112_2_18_0_50_10_72 "Server: Client(0x%x) deletion postponed!\n"
#define THREADX_APPMEM_895_112_2_18_0_50_12_73 "[SCI_CreateMemPool]: name:%s,  addr=0x%08X, size=0x%08X, unit size = 0x%X,  pool size=0x%X, watermark=0x%08Xrn"
#define THREADX_APPMEM_924_112_2_18_0_50_12_74 "[SCI_CreateMemPool]: Get a vaild Entry:0x%08X\r\n"
#define THREADX_APPMEM_947_112_2_18_0_50_12_75 "[SCI_CreateMemPool]: Create Pool,addr=0x%08X, size=0x%X\r\n"
#define THREADX_APPMEM_960_112_2_18_0_50_12_76 "[SCI_CreateMemPool]: Create Heap,addr=0x%08X, size=0x%X\r\n"
#define THREADX_APPMEM_975_112_2_18_0_50_12_77 "[SCI_CreateMemPool]: Mem handle=0x%08X\r\n"
#define THREADX_APPMEM_994_112_2_18_0_50_12_78 "[SCI_DeleteMemPool]: Mem handle=0x%08X, is_force_delete=0x%X\r\n"
#define THREADX_APPMEM_1019_112_2_18_0_50_12_79 "[SCI_DeleteMemPool]: Mem Pool Is using,delete fail, Total=0x%08X, avalable=0x%08Xrn"
#define THREADX_APPMEM_1035_112_2_18_0_50_12_80 "[SCI_DeleteMemPool]: Mem Heap Is using, delete fail, Total=0x%08X, avalable=0x%08Xrn"
#define THREADX_APPMEM_1044_112_2_18_0_50_12_81 "[SCI_DeleteMemPool]: Start Delete Mem Pool\r\n"
#define THREADX_APPMEM_1056_112_2_18_0_50_12_82 "[SCI_DeleteMemPool]: Start Delete Mem Heap\r\n"
#define THREADX_APPMEM_1067_112_2_18_0_50_12_83 "[SCI_DeleteMemPool]: Delete Complete\r\n"
#define THREADX_APPMEM_1226_112_2_18_0_50_12_84 "Memory memory_ptr 0x%8x has been destroied! It be malloced by %s on %d line ,size is %d"
#define THREADX_APPMEM_1383_112_2_18_0_50_13_85 "\nCould not get information until define macro SCI_MEMORY_DEBUG\n"
#define THREADX_MEM_983_112_2_18_0_50_15_86 "\nCould not get information until define macro DEBUG_KAL\n"
#define THREADX_MEM_997_112_2_18_0_50_15_87 "\nAllocated memory info:"
#define THREADX_MEM_999_112_2_18_0_50_15_88 "\n\tNo.      Size     FileName (Line)"
#define THREADX_MEM_1007_112_2_18_0_50_15_89 "\nmemory is corrupted, abnormal termination\n"
#define THREADX_MEM_1021_112_2_18_0_50_15_90 "nt%-8ld %-8ld %-12s File code: %ld  (Line %ld)"
#define THREADX_MEM_1032_112_2_18_0_50_15_91 "nt%-8ld %-8ld %-12s File code: %ld  (Line %ld)"
#define THREADX_MEM_1064_112_2_18_0_50_15_92 "nt%-8ld %-8ld %-12s %s (Line %ld)"
#define THREADX_MEM_1089_112_2_18_0_50_15_93 "\nmemory is corrupted, abnormal termination\n"
#define THREADX_MEM_1105_112_2_18_0_50_15_94 "nt%-8ld %-8ld OSA_ByteHeap %s (Line %ld)"
#define THREADX_MEM_1126_112_2_18_0_50_15_95 "\nCould not get information until define macro DEBUG_KAL\n"
#define THREADX_MEM_1134_112_2_18_0_50_15_96 "\nMemory Pool Information:"
#define THREADX_MEM_1136_112_2_18_0_50_15_97 "\n\tPool_Name           Start_Addr   End_Addr     Total_Num  Max_Used   Avail_Num"
#define THREADX_MEM_1151_112_2_18_0_50_15_98 "nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld"
#define THREADX_MEM_1160_112_2_18_0_50_15_99 "nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld"
#define THREADX_MEM_1168_112_2_18_0_50_15_100 "nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld"
#define THREADX_MEM_1176_112_2_18_0_50_15_101 "nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld"
#define THREADX_MEM_1378_112_2_18_0_50_16_102 "\nCould not get information until define macro DEBUG_KAL\n"
#define THREADX_MEM_1490_112_2_18_0_50_16_103 "\nCould not get information until define macro SCI_MEMORY_DEBUG\n"
#define THREADX_MEM_1802_112_2_18_0_50_17_104 "SHELL_memLeakCheck: Memory Debug List Error\r\n"
#define THREADX_MEM_1818_112_2_18_0_50_17_105 "SHELL_memLeakCheck: Memory Debug List Error\r\n"
#define THREADX_MEM_1849_112_2_18_0_50_17_106 "SCI_MemInfoGet:item_offset=%d\r\n"
#define THREADX_MEM_1888_112_2_18_0_50_17_107 "SCI_MemInfoGet: Some Error Happend\r\n"
#define THREADX_MEM_1983_112_2_18_0_50_17_108 "SCI_MemInfoGet: Is the list end\r\n"
#define THREADX_MEM_1997_112_2_18_0_50_17_109 "SCI_MemInfoGet:Buffer Len:0x%X\r\n"
#define THREADX_MEM_2003_112_2_18_0_50_17_110 "\nCould not get information until define macro SCI_MEMORY_DEBUG\n"
#define THREADX_OS_2232_112_2_18_0_50_22_111 "RTOS:SCI_ChangeThreadPriority dest thread isnot exist! ID:%d"
#define THREADX_OS_2605_112_2_18_0_50_23_112 "RTOS:SCI_SetThreadEvent dest thread is not exist! ID:%d"
#define THREADX_OS_3427_112_2_18_0_50_24_113 "SCI_ActiveTimer: The timer is active now, Could not active again!!!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(RTOS_TRC)
TRACE_MSG(SCI_API_227_112_2_18_0_49_28_0,"ERROR: The SDL TMID pointer is invalid !")
TRACE_MSG(SCI_API_316_112_2_18_0_49_28_1,"ERROR: The SDL TMID pointer is invalid. !")
TRACE_MSG(SCI_API_369_112_2_18_0_49_28_2,"ERROR: The SDL TMID pointer is invalid. !")
TRACE_MSG(SCI_API_419_112_2_18_0_49_28_3,"ERROR: The SDL TMID pointer is invalid. !")
TRACE_MSG(SCI_BLKMEM_101_112_2_18_0_49_29_4,"BLKMEM_Register: Module %d")
TRACE_MSG(SCI_MEM_226_112_2_18_0_49_34_5,"Memory memory_ptr 0x%8x has been destroied! It be malloced by %s on %d line ,size is %d")
TRACE_MSG(SCI_MEM_642_112_2_18_0_49_35_6,"SCI_GetByteHeapAvailableSpace:No Base Heap\r\n")
TRACE_MSG(SCI_MEM_648_112_2_18_0_49_35_7,"SCI_GetByteHeapAvailableSpace:No this Heap\r\n")
TRACE_MSG(SCI_MEM_745_112_2_18_0_49_36_8,"\nCould not get information until define macro __MEMORY_DEBUG__\n")
TRACE_MSG(SCI_MEM_756_112_2_18_0_49_36_9,"\nAllocated memory info:")
TRACE_MSG(SCI_MEM_758_112_2_18_0_49_36_10,"\n\tNo.      Size     FileName (Line)")
TRACE_MSG(SCI_MEM_782_112_2_18_0_49_36_11,"nt%-8ld %-8ld %s (Line %ld)")
TRACE_MSG(SCI_MEM_796_112_2_18_0_49_36_12,"\nCould not get SCI_GetMemInfoCount until define macro __MEMORY_DEBUG__\n")
TRACE_MSG(SCI_MEM_811_112_2_18_0_49_36_13,"SHELL_memLeakCheck: Memory Debug List Error\r\n")
TRACE_MSG(SCI_MEM_848_112_2_18_0_49_36_14,"SCI_MemInfoGet:item_offset=%d\r\n")
TRACE_MSG(SCI_MEM_875_112_2_18_0_49_36_15,"SCI_MemInfoGet: Is List End\r\n")
TRACE_MSG(SCI_MEM_914_112_2_18_0_49_36_16,"SCI_MemInfoGet:MEM_INFO_END\r\n")
TRACE_MSG(SCI_MEM_924_112_2_18_0_49_36_17,"SCI_MemInfoGet:Buffer Len:0x%X\r\n")
TRACE_MSG(SCI_MEM_930_112_2_18_0_49_36_18,"\nCould not get information until define macro __MEMORY_DEBUG__\n")
TRACE_MSG(SERVER_480_112_2_18_0_49_43_19,"Server: time to delete the server(%d)!\n")
TRACE_MSG(SERVER_502_112_2_18_0_49_43_20,"Server: time to delete the client(0x%x)!\n")
TRACE_MSG(SERVER_604_112_2_18_0_49_43_21,"Server: server link list error!\n")
TRACE_MSG(SERVER_619_112_2_18_0_49_43_22,"Server: Delete the server(%d) immediately!\n")
TRACE_MSG(SERVER_635_112_2_18_0_49_43_23,"Server: server(%d) deletion postponed!\n")
TRACE_MSG(SERVER_872_112_2_18_0_49_44_24,"Server: Delete the Client(0x%x) immediately!\n")
TRACE_MSG(SERVER_875_112_2_18_0_49_44_25,"Server: Client(0x%x) deletion postponed!\n")
TRACE_MSG(THREADX_APPMEM_889_112_2_18_0_49_46_26,"[SCI_CreateMemPool]: name:%s,  addr=0x%08X, size=0x%08X, unit size = 0x%X,  pool size=0x%X, watermark=0x%08Xrn")
TRACE_MSG(THREADX_APPMEM_920_112_2_18_0_49_46_27,"[SCI_CreateMemPool]: Get a vaild Entry:0x%08X\r\n")
TRACE_MSG(THREADX_APPMEM_943_112_2_18_0_49_46_28,"[SCI_CreateMemPool]: Create Pool,addr=0x%08X, size=0x%X\r\n")
TRACE_MSG(THREADX_APPMEM_956_112_2_18_0_49_46_29,"[SCI_CreateMemPool]: Create Heap,addr=0x%08X, size=0x%X\r\n")
TRACE_MSG(THREADX_APPMEM_971_112_2_18_0_49_46_30,"[SCI_CreateMemPool]: Mem handle=0x%08X\r\n")
TRACE_MSG(THREADX_APPMEM_990_112_2_18_0_49_46_31,"[SCI_DeleteMemPool]: Mem handle=0x%08X, is_force_delete=0x%X\r\n")
TRACE_MSG(THREADX_APPMEM_1016_112_2_18_0_49_47_32,"[SCI_DeleteMemPool]: Mem Pool Is using,delete fail, Total=0x%08X, avalable=0x%08Xrn")
TRACE_MSG(THREADX_APPMEM_1032_112_2_18_0_49_47_33,"[SCI_DeleteMemPool]: Mem Heap Is using, delete fail, Total=0x%08X, avalable=0x%08Xrn")
TRACE_MSG(THREADX_APPMEM_1041_112_2_18_0_49_47_34,"[SCI_DeleteMemPool]: Start Delete Mem Pool\r\n")
TRACE_MSG(THREADX_APPMEM_1053_112_2_18_0_49_47_35,"[SCI_DeleteMemPool]: Start Delete Mem Heap\r\n")
TRACE_MSG(THREADX_APPMEM_1064_112_2_18_0_49_47_36,"[SCI_DeleteMemPool]: Delete Complete\r\n")
TRACE_MSG(THREADX_APPMEM_1241_112_2_18_0_49_47_37,"Memory memory_ptr 0x%8x has been destroied! It be malloced by %s on %d line ,size is %d")
TRACE_MSG(THREADX_APPMEM_1399_112_2_18_0_49_48_38,"\nCould not get information until define macro SCI_MEMORY_DEBUG\n")
TRACE_MSG(THREADX_MEM_782_112_2_18_0_49_49_39,"Memory memory_ptr 0x%8x has been destroied! It be malloced by %s on %d line ,size is %d")
TRACE_MSG(THREADX_MEM_1082_112_2_18_0_49_50_40,"\nCould not get information until define macro SCI_MEMORY_DEBUG\n")
TRACE_MSG(THREADX_MEM_1093_112_2_18_0_49_50_41,"\nAllocated memory info:")
TRACE_MSG(THREADX_MEM_1095_112_2_18_0_49_50_42,"\n\tNo.      Size     FileName (Line)")
TRACE_MSG(THREADX_MEM_1119_112_2_18_0_49_50_43,"nt%-8ld %-8ld %s (Line %ld)")
TRACE_MSG(THREADX_MEM_1134_112_2_18_0_49_50_44,"\nCould not get information until define macro SCI_MEMORY_DEBUG\n")
TRACE_MSG(THREADX_MEM_1150_112_2_18_0_49_50_45,"SHELL_memLeakCheck: Memory Debug List Error\r\n")
TRACE_MSG(THREADX_MEM_1187_112_2_18_0_49_50_46,"SCI_MemInfoGet:item_offset=%d\r\n")
TRACE_MSG(THREADX_MEM_1214_112_2_18_0_49_50_47,"SCI_MemInfoGet: Is List End\r\n")
TRACE_MSG(THREADX_MEM_1253_112_2_18_0_49_50_48,"SCI_MemInfoGet:MEM_INFO_END\r\n")
TRACE_MSG(THREADX_MEM_1263_112_2_18_0_49_50_49,"SCI_MemInfoGet:Buffer Len:0x%X\r\n")
TRACE_MSG(THREADX_MEM_1269_112_2_18_0_49_50_50,"\nCould not get information until define macro SCI_MEMORY_DEBUG\n")
TRACE_MSG(THREADX_MEM_1289_112_2_18_0_49_50_51,"\nCould not get information until define macro SCI_MEMORY_DEBUG\n")
TRACE_MSG(THREADX_MEM_1293_112_2_18_0_49_50_52,"\nMemory Pool Information:")
TRACE_MSG(THREADX_MEM_1296_112_2_18_0_49_50_53,"\n\tPool_Name           Start_Addr   End_Addr     Total_Num  Max_Used   Avail_Num")
TRACE_MSG(THREADX_MEM_1306_112_2_18_0_49_50_54,"nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld")
TRACE_MSG(THREADX_OS_1492_112_2_18_0_49_54_55,"RTOS:SCI_DeleteThread dest thread is not exist! ID:%d")
TRACE_MSG(THREADX_OS_1744_112_2_18_0_49_55_56,"RTOS:SCI_TerminateThread dest thread is not exist! ID:%d")
TRACE_MSG(THREADX_OS_1825_112_2_18_0_49_55_57,"RTOS:SCI_ChangeThreadPriority dest thread isnot exist! ID:%d")
TRACE_MSG(THREADX_OS_1833_112_2_18_0_49_55_58,"RTOS:SCI_ChangeThreadPriority dest thread isnot exist! ID:%d")
TRACE_MSG(THREADX_OS_2210_112_2_18_0_49_56_59,"RTOS:SCI_SetThreadEvent dest thread is not exist! ID:%d")
TRACE_MSG(THREADX_OS_2216_112_2_18_0_49_56_60,"RTOS:SCI_SetThreadEvent dest thread is not exist! ID:%d")
TRACE_MSG(THREADX_OS_2989_112_2_18_0_49_57_61,"SCI_ActiveTimer: The timer is active now, Could not active again!!!")
TRACE_MSG(SCI_API_227_112_2_18_0_50_2_62,"ERROR: The SDL TMID pointer is invalid !")
TRACE_MSG(SCI_API_316_112_2_18_0_50_2_63,"ERROR: The SDL TMID pointer is invalid. !")
TRACE_MSG(SCI_API_369_112_2_18_0_50_2_64,"ERROR: The SDL TMID pointer is invalid. !")
TRACE_MSG(SCI_API_419_112_2_18_0_50_2_65,"ERROR: The SDL TMID pointer is invalid. !")
TRACE_MSG(SERVER_480_112_2_18_0_50_9_66,"Server: time to delete the server(%d)!\n")
TRACE_MSG(SERVER_502_112_2_18_0_50_9_67,"Server: time to delete the client(0x%x)!\n")
TRACE_MSG(SERVER_604_112_2_18_0_50_9_68,"Server: server link list error!\n")
TRACE_MSG(SERVER_619_112_2_18_0_50_9_69,"Server: Delete the server(%d) immediately!\n")
TRACE_MSG(SERVER_635_112_2_18_0_50_9_70,"Server: server(%d) deletion postponed!\n")
TRACE_MSG(SERVER_872_112_2_18_0_50_10_71,"Server: Delete the Client(0x%x) immediately!\n")
TRACE_MSG(SERVER_875_112_2_18_0_50_10_72,"Server: Client(0x%x) deletion postponed!\n")
TRACE_MSG(THREADX_APPMEM_895_112_2_18_0_50_12_73,"[SCI_CreateMemPool]: name:%s,  addr=0x%08X, size=0x%08X, unit size = 0x%X,  pool size=0x%X, watermark=0x%08Xrn")
TRACE_MSG(THREADX_APPMEM_924_112_2_18_0_50_12_74,"[SCI_CreateMemPool]: Get a vaild Entry:0x%08X\r\n")
TRACE_MSG(THREADX_APPMEM_947_112_2_18_0_50_12_75,"[SCI_CreateMemPool]: Create Pool,addr=0x%08X, size=0x%X\r\n")
TRACE_MSG(THREADX_APPMEM_960_112_2_18_0_50_12_76,"[SCI_CreateMemPool]: Create Heap,addr=0x%08X, size=0x%X\r\n")
TRACE_MSG(THREADX_APPMEM_975_112_2_18_0_50_12_77,"[SCI_CreateMemPool]: Mem handle=0x%08X\r\n")
TRACE_MSG(THREADX_APPMEM_994_112_2_18_0_50_12_78,"[SCI_DeleteMemPool]: Mem handle=0x%08X, is_force_delete=0x%X\r\n")
TRACE_MSG(THREADX_APPMEM_1019_112_2_18_0_50_12_79,"[SCI_DeleteMemPool]: Mem Pool Is using,delete fail, Total=0x%08X, avalable=0x%08Xrn")
TRACE_MSG(THREADX_APPMEM_1035_112_2_18_0_50_12_80,"[SCI_DeleteMemPool]: Mem Heap Is using, delete fail, Total=0x%08X, avalable=0x%08Xrn")
TRACE_MSG(THREADX_APPMEM_1044_112_2_18_0_50_12_81,"[SCI_DeleteMemPool]: Start Delete Mem Pool\r\n")
TRACE_MSG(THREADX_APPMEM_1056_112_2_18_0_50_12_82,"[SCI_DeleteMemPool]: Start Delete Mem Heap\r\n")
TRACE_MSG(THREADX_APPMEM_1067_112_2_18_0_50_12_83,"[SCI_DeleteMemPool]: Delete Complete\r\n")
TRACE_MSG(THREADX_APPMEM_1226_112_2_18_0_50_12_84,"Memory memory_ptr 0x%8x has been destroied! It be malloced by %s on %d line ,size is %d")
TRACE_MSG(THREADX_APPMEM_1383_112_2_18_0_50_13_85,"\nCould not get information until define macro SCI_MEMORY_DEBUG\n")
TRACE_MSG(THREADX_MEM_983_112_2_18_0_50_15_86,"\nCould not get information until define macro DEBUG_KAL\n")
TRACE_MSG(THREADX_MEM_997_112_2_18_0_50_15_87,"\nAllocated memory info:")
TRACE_MSG(THREADX_MEM_999_112_2_18_0_50_15_88,"\n\tNo.      Size     FileName (Line)")
TRACE_MSG(THREADX_MEM_1007_112_2_18_0_50_15_89,"\nmemory is corrupted, abnormal termination\n")
TRACE_MSG(THREADX_MEM_1021_112_2_18_0_50_15_90,"nt%-8ld %-8ld %-12s File code: %ld  (Line %ld)")
TRACE_MSG(THREADX_MEM_1032_112_2_18_0_50_15_91,"nt%-8ld %-8ld %-12s File code: %ld  (Line %ld)")
TRACE_MSG(THREADX_MEM_1064_112_2_18_0_50_15_92,"nt%-8ld %-8ld %-12s %s (Line %ld)")
TRACE_MSG(THREADX_MEM_1089_112_2_18_0_50_15_93,"\nmemory is corrupted, abnormal termination\n")
TRACE_MSG(THREADX_MEM_1105_112_2_18_0_50_15_94,"nt%-8ld %-8ld OSA_ByteHeap %s (Line %ld)")
TRACE_MSG(THREADX_MEM_1126_112_2_18_0_50_15_95,"\nCould not get information until define macro DEBUG_KAL\n")
TRACE_MSG(THREADX_MEM_1134_112_2_18_0_50_15_96,"\nMemory Pool Information:")
TRACE_MSG(THREADX_MEM_1136_112_2_18_0_50_15_97,"\n\tPool_Name           Start_Addr   End_Addr     Total_Num  Max_Used   Avail_Num")
TRACE_MSG(THREADX_MEM_1151_112_2_18_0_50_15_98,"nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld")
TRACE_MSG(THREADX_MEM_1160_112_2_18_0_50_15_99,"nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld")
TRACE_MSG(THREADX_MEM_1168_112_2_18_0_50_15_100,"nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld")
TRACE_MSG(THREADX_MEM_1176_112_2_18_0_50_15_101,"nt%-19s 0x%08lx   0x%08lx   %-8ld   %-8ld   %-8ld")
TRACE_MSG(THREADX_MEM_1378_112_2_18_0_50_16_102,"\nCould not get information until define macro DEBUG_KAL\n")
TRACE_MSG(THREADX_MEM_1490_112_2_18_0_50_16_103,"\nCould not get information until define macro SCI_MEMORY_DEBUG\n")
TRACE_MSG(THREADX_MEM_1802_112_2_18_0_50_17_104,"SHELL_memLeakCheck: Memory Debug List Error\r\n")
TRACE_MSG(THREADX_MEM_1818_112_2_18_0_50_17_105,"SHELL_memLeakCheck: Memory Debug List Error\r\n")
TRACE_MSG(THREADX_MEM_1849_112_2_18_0_50_17_106,"SCI_MemInfoGet:item_offset=%d\r\n")
TRACE_MSG(THREADX_MEM_1888_112_2_18_0_50_17_107,"SCI_MemInfoGet: Some Error Happend\r\n")
TRACE_MSG(THREADX_MEM_1983_112_2_18_0_50_17_108,"SCI_MemInfoGet: Is the list end\r\n")
TRACE_MSG(THREADX_MEM_1997_112_2_18_0_50_17_109,"SCI_MemInfoGet:Buffer Len:0x%X\r\n")
TRACE_MSG(THREADX_MEM_2003_112_2_18_0_50_17_110,"\nCould not get information until define macro SCI_MEMORY_DEBUG\n")
TRACE_MSG(THREADX_OS_2232_112_2_18_0_50_22_111,"RTOS:SCI_ChangeThreadPriority dest thread isnot exist! ID:%d")
TRACE_MSG(THREADX_OS_2605_112_2_18_0_50_23_112,"RTOS:SCI_SetThreadEvent dest thread is not exist! ID:%d")
TRACE_MSG(THREADX_OS_3427_112_2_18_0_50_24_113,"SCI_ActiveTimer: The timer is active now, Could not active again!!!")
END_TRACE_MAP(RTOS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _RTOS_TRC_H_


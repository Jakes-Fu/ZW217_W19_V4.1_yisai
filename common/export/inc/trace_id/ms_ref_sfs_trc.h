/******************************************************************************
 ** File Name:      ms_ref_sfs_trc.h                                         *
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
//trace_id:68
#ifndef _MS_REF_SFS_TRC_H_
#define _MS_REF_SFS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define FFS_API_OLD_34_112_2_18_1_52_49_0 "FFS %s name is:"
#define FFS_API_OLD_38_112_2_18_1_52_49_1 "Zero"
#define FFS_API_OLD_43_112_2_18_1_52_49_2 "Too long"
#define FFS_API_OLD_58_112_2_18_1_52_49_3 "%s"
#define FFS_API_OLD_64_112_2_18_1_52_49_4 "%s"
#define FFS_DEV_CLUSTERBUF_MANAGER_1025_112_2_18_1_52_59_5 "FFS,-------counter = %d"
#define FFS_DEV_MANAGER_242_112_2_18_1_53_0_6 "FFS,The device has not been registed,so you can not unregisted it"
#define FFS_FAT_MANAGER_1013_112_2_18_1_53_4_7 "FFS chek info time is %d"
#define FFS_FAT_MANAGER_1014_112_2_18_1_53_4_8 "bad_cluster_counts = %x,used_cluster_counts = %x,free_cluster_counts = %x,first_free_cluster_id = %x"
#define FFS_FCB_49_112_2_18_1_53_4_9 "%s"
#define FFS_FCB_55_112_2_18_1_53_5_10 "%s"
#define FFS_FCB_68_112_2_18_1_53_5_11 "FFS the file opened:"
#define FFS_FCB_72_112_2_18_1_53_5_12 "entry_cluster_id = 0x%x,entry_offset = 0x%x,mode = 0x%x"
#define FFS_FCB_77_112_2_18_1_53_5_13 "FFS the findhanler opened:"
#define FFS_FCB_86_112_2_18_1_53_5_14 "FFS 0x%x handles"
#define FFS_PAL_79_112_2_18_1_53_10_15 "FFS,FFS_OpenLLD,BSD_Open begin"
#define FFS_PAL_85_112_2_18_1_53_10_16 "FFS,FFS_OpenLLD Image open fail"
#define FFS_PAL_89_112_2_18_1_53_10_17 "FFS,FFS_OpenLLD,BSD_Open suc!"
#define FFS_PAL_102_112_2_18_1_53_10_18 "FFS,FFS_OpenLLD,SectorSize=%d,TotScts=%d"
#define FFS_PAL_108_112_2_18_1_53_10_19 "FFS_CloseLLD!!!"
#define FFS_PAL_154_112_2_18_1_53_10_20 "FFS,FFS_OpenLLD,BSD_Open begin"
#define FFS_PAL_161_112_2_18_1_53_10_21 "FFS,OpenLLD,%s"
#define FFS_PAL_166_112_2_18_1_53_10_22 "FFS,FFS_OpenLLD BSD open fail"
#define FFS_PAL_170_112_2_18_1_53_10_23 "FFS,FFS_OpenLLD,BSD_Open suc!"
#define FFS_PAL_179_112_2_18_1_53_10_24 "FFS,FFS_OpenLLD,SectorSize=%d,TotScts=%d"
#define FFS_PAL_186_112_2_18_1_53_10_25 "FFS_CloseLLD!!!"
#define FFS_PAL_243_112_2_18_1_53_11_26 "FFS,FFS_OpenLLD,BSD_Open begin"
#define FFS_PAL_250_112_2_18_1_53_11_27 "FFS,OpenLLD,%s"
#define FFS_PAL_255_112_2_18_1_53_11_28 "FFS,FFS_OpenLLD BSD open fail"
#define FFS_PAL_259_112_2_18_1_53_11_29 "FFS,FFS_OpenLLD,BSD_Open suc!"
#define FFS_PAL_270_112_2_18_1_53_11_30 "FFS,FFS_OpenLLD,SectorSize=%d,TotScts=%d"
#define FFS_PAL_277_112_2_18_1_53_11_31 "FFS_CloseLLD!!!"
#define UDISKDEV_49_112_2_18_1_53_21_32 "FFS,UdiskReadSector fail,%d"
#define UDISKDEV_62_112_2_18_1_53_21_33 "FFS,UdiskDeleteSector fail,%d"
#define UDISKDEV_78_112_2_18_1_53_21_34 "FFS,UdiskWriteSector fail,%d"
#define UDISKDEV_239_112_2_18_1_53_21_35 "FFS , error high address of entry in NONE FAT32 is not zero,it is wrong entry,this disk is not correctly write before"
#define UDISKDEV_270_112_2_18_1_53_21_36 "FFS,fat_update_cur_dir_cluster,id<2 or > Totclu+1,fail!"
#define UDISKDEV_316_112_2_18_1_53_21_37 "FFS,malloc root buffer fail"
#define UDISKDEV_323_112_2_18_1_53_21_38 "FFS,read root sector fail"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_SFS_TRC)
TRACE_MSG(FFS_API_OLD_34_112_2_18_1_52_49_0,"FFS %s name is:")
TRACE_MSG(FFS_API_OLD_38_112_2_18_1_52_49_1,"Zero")
TRACE_MSG(FFS_API_OLD_43_112_2_18_1_52_49_2,"Too long")
TRACE_MSG(FFS_API_OLD_58_112_2_18_1_52_49_3,"%s")
TRACE_MSG(FFS_API_OLD_64_112_2_18_1_52_49_4,"%s")
TRACE_MSG(FFS_DEV_CLUSTERBUF_MANAGER_1025_112_2_18_1_52_59_5,"FFS,-------counter = %d")
TRACE_MSG(FFS_DEV_MANAGER_242_112_2_18_1_53_0_6,"FFS,The device has not been registed,so you can not unregisted it")
TRACE_MSG(FFS_FAT_MANAGER_1013_112_2_18_1_53_4_7,"FFS chek info time is %d")
TRACE_MSG(FFS_FAT_MANAGER_1014_112_2_18_1_53_4_8,"bad_cluster_counts = %x,used_cluster_counts = %x,free_cluster_counts = %x,first_free_cluster_id = %x")
TRACE_MSG(FFS_FCB_49_112_2_18_1_53_4_9,"%s")
TRACE_MSG(FFS_FCB_55_112_2_18_1_53_5_10,"%s")
TRACE_MSG(FFS_FCB_68_112_2_18_1_53_5_11,"FFS the file opened:")
TRACE_MSG(FFS_FCB_72_112_2_18_1_53_5_12,"entry_cluster_id = 0x%x,entry_offset = 0x%x,mode = 0x%x")
TRACE_MSG(FFS_FCB_77_112_2_18_1_53_5_13,"FFS the findhanler opened:")
TRACE_MSG(FFS_FCB_86_112_2_18_1_53_5_14,"FFS 0x%x handles")
TRACE_MSG(FFS_PAL_79_112_2_18_1_53_10_15,"FFS,FFS_OpenLLD,BSD_Open begin")
TRACE_MSG(FFS_PAL_85_112_2_18_1_53_10_16,"FFS,FFS_OpenLLD Image open fail")
TRACE_MSG(FFS_PAL_89_112_2_18_1_53_10_17,"FFS,FFS_OpenLLD,BSD_Open suc!")
TRACE_MSG(FFS_PAL_102_112_2_18_1_53_10_18,"FFS,FFS_OpenLLD,SectorSize=%d,TotScts=%d")
TRACE_MSG(FFS_PAL_108_112_2_18_1_53_10_19,"FFS_CloseLLD!!!")
TRACE_MSG(FFS_PAL_154_112_2_18_1_53_10_20,"FFS,FFS_OpenLLD,BSD_Open begin")
TRACE_MSG(FFS_PAL_161_112_2_18_1_53_10_21,"FFS,OpenLLD,%s")
TRACE_MSG(FFS_PAL_166_112_2_18_1_53_10_22,"FFS,FFS_OpenLLD BSD open fail")
TRACE_MSG(FFS_PAL_170_112_2_18_1_53_10_23,"FFS,FFS_OpenLLD,BSD_Open suc!")
TRACE_MSG(FFS_PAL_179_112_2_18_1_53_10_24,"FFS,FFS_OpenLLD,SectorSize=%d,TotScts=%d")
TRACE_MSG(FFS_PAL_186_112_2_18_1_53_10_25,"FFS_CloseLLD!!!")
TRACE_MSG(FFS_PAL_243_112_2_18_1_53_11_26,"FFS,FFS_OpenLLD,BSD_Open begin")
TRACE_MSG(FFS_PAL_250_112_2_18_1_53_11_27,"FFS,OpenLLD,%s")
TRACE_MSG(FFS_PAL_255_112_2_18_1_53_11_28,"FFS,FFS_OpenLLD BSD open fail")
TRACE_MSG(FFS_PAL_259_112_2_18_1_53_11_29,"FFS,FFS_OpenLLD,BSD_Open suc!")
TRACE_MSG(FFS_PAL_270_112_2_18_1_53_11_30,"FFS,FFS_OpenLLD,SectorSize=%d,TotScts=%d")
TRACE_MSG(FFS_PAL_277_112_2_18_1_53_11_31,"FFS_CloseLLD!!!")
TRACE_MSG(UDISKDEV_49_112_2_18_1_53_21_32,"FFS,UdiskReadSector fail,%d")
TRACE_MSG(UDISKDEV_62_112_2_18_1_53_21_33,"FFS,UdiskDeleteSector fail,%d")
TRACE_MSG(UDISKDEV_78_112_2_18_1_53_21_34,"FFS,UdiskWriteSector fail,%d")
TRACE_MSG(UDISKDEV_239_112_2_18_1_53_21_35,"FFS , error high address of entry in NONE FAT32 is not zero,it is wrong entry,this disk is not correctly write before")
TRACE_MSG(UDISKDEV_270_112_2_18_1_53_21_36,"FFS,fat_update_cur_dir_cluster,id<2 or > Totclu+1,fail!")
TRACE_MSG(UDISKDEV_316_112_2_18_1_53_21_37,"FFS,malloc root buffer fail")
TRACE_MSG(UDISKDEV_323_112_2_18_1_53_21_38,"FFS,read root sector fail")
END_TRACE_MAP(MS_REF_SFS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_SFS_TRC_H_


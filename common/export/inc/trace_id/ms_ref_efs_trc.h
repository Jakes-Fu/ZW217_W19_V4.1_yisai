/******************************************************************************
 ** File Name:      ms_ref_efs_trc.h                                         *
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
//trace_id:46
#ifndef _MS_REF_EFS_TRC_H_
#define _MS_REF_EFS_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define EFS_DELETE_62_112_2_18_1_30_1_0 "[DRV_EFS]<ERROR> invalid parameters"
#define EFS_DUMMY_374_112_2_18_1_30_3_1 "NVITEM_UpdateProductInfo : erase slave address failed\n"
#define EFS_DUMMY_382_112_2_18_1_30_3_2 "NVITEM_UpdateProductInfo_Protect : write slave address failed\n"
#define EFS_DUMMY_393_112_2_18_1_30_3_3 "NVITEM_UpdateProductInfo : erase product_info_addr failed\n"
#define EFS_DUMMY_411_112_2_18_1_30_3_4 "NVITEM_UpdateProductInfo : erase slave address failed\n"
#define EFS_DUMMY_504_112_2_18_1_30_3_5 "NVITEM_UpdateProductInfo_Protect : erase slave address failed\n"
#define EFS_DUMMY_512_112_2_18_1_30_3_6 "NVITEM_UpdateProductInfo_Protect : write slave address failed\n"
#define EFS_DUMMY_523_112_2_18_1_30_3_7 "NVITEM_UpdateProductInfo_Protect : erase slave address failed\n"
#define EFS_DUMMY_554_112_2_18_1_30_3_8 "PhaseCheck Itemno %d \n"
#define EFS_DUMMY_557_112_2_18_1_30_3_9 "NVITEM_UpdateProductInfo_Protect: write protect info address failed!!!\n"
#define EFS_DUMMY_567_112_2_18_1_30_3_10 "NVITEM_UpdateProductInfo_Protect : erase slave address after failed\n"
#define EFS_DUMMY_584_112_2_18_1_30_3_11 "RETURN SUCCESS\n"
#define EFS_EVENT_96_112_2_18_1_30_4_12 "[DRV_EFS]<ERROR> Invalid parameter."
#define EFS_IRP_204_112_2_18_1_30_6_13 "[DRV_EFS]<ERROR> Invalid parameters.."
#define EFS_ITEM_59_112_2_18_1_30_7_14 "System Error : %s(%d)"
#define EFS_ITEM_491_112_2_18_1_30_7_15 "EFS_TraceSectorSpaceInfo:  efs is internal do garbage collect ,please waiting 10s and try call this function again!!!"
#define EFS_ITEM_497_112_2_18_1_30_7_16 "EFS_TraceSectorSpaceInfo: sec_id=0x%x is not  efs sector:(sec_min=0x%x, sec_max=0x%x)"
#define EFS_ITEM_506_112_2_18_1_30_8_17 "EFS_TraceTotalSpaceInfo: sec_id=0x%x is not good sector, maybe is being erasing!!!"
#define EFS_ITEM_517_112_2_18_1_30_8_18 "EFS_TraceSectorSpaceInfo:sec_addr = 0x%x, sector.free_space=0x%x,sector.dirty_space=0x%x,sector.used_space=0x%x"
#define EFS_ITEM_539_112_2_18_1_30_8_19 "EFS_TraceSectorSpaceInfo:  efs is internal do garbage collect ,please waiting 10s and try call this function again!!!"
#define EFS_ITEM_550_112_2_18_1_30_8_20 "EFS_TraceTotalSpaceInfo: sec_id=0x%x is being erasing"
#define EFS_ITEM_560_112_2_18_1_30_8_21 "EFS_TraceTotalSpaceInfo: total.free_space=0x%x,total.dirty_space=0x%x,total.used_space=0x%x"
#define EFS_ITEM_752_112_2_18_1_30_8_22 "EFS_FindAvailSectorInRefreshStatus:  find no avail sector!!!"
#define EFS_ITEM_759_112_2_18_1_30_8_23 "EFS_FindAvailSectorInRefreshStatus:  find  avail sector num=0x%x"
#define EFS_ITEM_942_112_2_18_1_30_8_24 "check_sum 0x%x  item_head[1] 0x%x\n"
#define EFS_ITEM_2240_112_2_18_1_30_11_25 "[DRV_EFS]<Item Type ERROR>"
#define EFS_ITEM_2494_112_2_18_1_30_12_26 "[DRV_EFS]<Init Flag ERROR>"
#define EFS_ITEM_2562_112_2_18_1_30_12_27 "[DRV_EFS]<Init Flag ERROR>"
#define EFS_ITEM_2653_112_2_18_1_30_12_28 "[DRV_EFS]<Init Flag ERROR>"
#define EFS_ITEM_3179_112_2_18_1_30_13_29 "[efs_task1]:sector %d erase_count %d dirty 0x%x free 0x%x\n"
#define EFS_ITEM_3250_112_2_18_1_30_13_30 "[DRV_EFS]<efs sector ERROR>\n"
#define EFS_ITEM_3323_112_2_18_1_30_13_31 "[DRV_EFS]<refresh status ERROR >\n"
#define EFS_TASK_366_112_2_18_1_30_19_32 "[efs_task1]: write when refresh %d id %d count 0x%xn"
#define EFS_TASK_479_112_2_18_1_30_19_33 "[efs_task1]: begin to sleep 300ms :0x%x"
#define NV_SYS_484_112_2_18_1_30_21_34 "efs:write nvitem to cache failed. %d"
#define NV_SYS_713_112_2_18_1_30_21_35 "Now Update Nvitem "
#define NVITEM_CACHE_269_112_2_18_1_30_22_36 "verify_cache_data failed: ptr = 0x%x\n"
#define NVITEM_CACHE_277_112_2_18_1_30_22_37 "invalid length %d ptr = 0x%x\n"
#define NVITEM_CACHE_380_112_2_18_1_30_22_38 "[DRV_EFS]<WARNING> invalid size "
#define NVITEM_CACHE_457_112_2_18_1_30_23_39 "[DRV_EFS]<WARNING> invalid size"
#define NVITEM_CACHE_480_112_2_18_1_30_23_40 "[DRV_EFS]<ERROR> invalid size.."
#define NVITEM_CACHE_509_112_2_18_1_30_23_41 "EFS: write from cache failed. %d"
#define NVITEM_CACHE_664_112_2_18_1_30_23_42 "verify_cache_data failed: id %d ptr = 0x%x\n"
#define NVITEM_CACHE_669_112_2_18_1_30_23_43 "EFS: Has flush %d, is writting now\n"
#define NVITEM_CACHE_721_112_2_18_1_30_23_44 "EFS: Has flush %d, but cant free it(%d/%d)"
#define NVITEM_CACHE_735_112_2_18_1_30_23_45 "efs: flush all cache"
#define UPDATE_FIXNV_634_112_2_18_1_30_25_46 "[Verify_fixnv] Both Fixnv and Fixnv Copy are bad!Now just goahead For Luck\r\n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_EFS_TRC)
TRACE_MSG(EFS_DELETE_62_112_2_18_1_30_1_0,"[DRV_EFS]<ERROR> invalid parameters")
TRACE_MSG(EFS_DUMMY_374_112_2_18_1_30_3_1,"NVITEM_UpdateProductInfo : erase slave address failed\n")
TRACE_MSG(EFS_DUMMY_382_112_2_18_1_30_3_2,"NVITEM_UpdateProductInfo_Protect : write slave address failed\n")
TRACE_MSG(EFS_DUMMY_393_112_2_18_1_30_3_3,"NVITEM_UpdateProductInfo : erase product_info_addr failed\n")
TRACE_MSG(EFS_DUMMY_411_112_2_18_1_30_3_4,"NVITEM_UpdateProductInfo : erase slave address failed\n")
TRACE_MSG(EFS_DUMMY_504_112_2_18_1_30_3_5,"NVITEM_UpdateProductInfo_Protect : erase slave address failed\n")
TRACE_MSG(EFS_DUMMY_512_112_2_18_1_30_3_6,"NVITEM_UpdateProductInfo_Protect : write slave address failed\n")
TRACE_MSG(EFS_DUMMY_523_112_2_18_1_30_3_7,"NVITEM_UpdateProductInfo_Protect : erase slave address failed\n")
TRACE_MSG(EFS_DUMMY_554_112_2_18_1_30_3_8,"PhaseCheck Itemno %d \n")
TRACE_MSG(EFS_DUMMY_557_112_2_18_1_30_3_9,"NVITEM_UpdateProductInfo_Protect: write protect info address failed!!!\n")
TRACE_MSG(EFS_DUMMY_567_112_2_18_1_30_3_10,"NVITEM_UpdateProductInfo_Protect : erase slave address after failed\n")
TRACE_MSG(EFS_DUMMY_584_112_2_18_1_30_3_11,"RETURN SUCCESS\n")
TRACE_MSG(EFS_EVENT_96_112_2_18_1_30_4_12,"[DRV_EFS]<ERROR> Invalid parameter.")
TRACE_MSG(EFS_IRP_204_112_2_18_1_30_6_13,"[DRV_EFS]<ERROR> Invalid parameters..")
TRACE_MSG(EFS_ITEM_59_112_2_18_1_30_7_14,"System Error : %s(%d)")
TRACE_MSG(EFS_ITEM_491_112_2_18_1_30_7_15,"EFS_TraceSectorSpaceInfo:  efs is internal do garbage collect ,please waiting 10s and try call this function again!!!")
TRACE_MSG(EFS_ITEM_497_112_2_18_1_30_7_16,"EFS_TraceSectorSpaceInfo: sec_id=0x%x is not  efs sector:(sec_min=0x%x, sec_max=0x%x)")
TRACE_MSG(EFS_ITEM_506_112_2_18_1_30_8_17,"EFS_TraceTotalSpaceInfo: sec_id=0x%x is not good sector, maybe is being erasing!!!")
TRACE_MSG(EFS_ITEM_517_112_2_18_1_30_8_18,"EFS_TraceSectorSpaceInfo:sec_addr = 0x%x, sector.free_space=0x%x,sector.dirty_space=0x%x,sector.used_space=0x%x")
TRACE_MSG(EFS_ITEM_539_112_2_18_1_30_8_19,"EFS_TraceSectorSpaceInfo:  efs is internal do garbage collect ,please waiting 10s and try call this function again!!!")
TRACE_MSG(EFS_ITEM_550_112_2_18_1_30_8_20,"EFS_TraceTotalSpaceInfo: sec_id=0x%x is being erasing")
TRACE_MSG(EFS_ITEM_560_112_2_18_1_30_8_21,"EFS_TraceTotalSpaceInfo: total.free_space=0x%x,total.dirty_space=0x%x,total.used_space=0x%x")
TRACE_MSG(EFS_ITEM_752_112_2_18_1_30_8_22,"EFS_FindAvailSectorInRefreshStatus:  find no avail sector!!!")
TRACE_MSG(EFS_ITEM_759_112_2_18_1_30_8_23,"EFS_FindAvailSectorInRefreshStatus:  find  avail sector num=0x%x")
TRACE_MSG(EFS_ITEM_942_112_2_18_1_30_8_24,"check_sum 0x%x  item_head[1] 0x%x\n")
TRACE_MSG(EFS_ITEM_2240_112_2_18_1_30_11_25,"[DRV_EFS]<Item Type ERROR>")
TRACE_MSG(EFS_ITEM_2494_112_2_18_1_30_12_26,"[DRV_EFS]<Init Flag ERROR>")
TRACE_MSG(EFS_ITEM_2562_112_2_18_1_30_12_27,"[DRV_EFS]<Init Flag ERROR>")
TRACE_MSG(EFS_ITEM_2653_112_2_18_1_30_12_28,"[DRV_EFS]<Init Flag ERROR>")
TRACE_MSG(EFS_ITEM_3179_112_2_18_1_30_13_29,"[efs_task1]:sector %d erase_count %d dirty 0x%x free 0x%x\n")
TRACE_MSG(EFS_ITEM_3250_112_2_18_1_30_13_30,"[DRV_EFS]<efs sector ERROR>\n")
TRACE_MSG(EFS_ITEM_3323_112_2_18_1_30_13_31,"[DRV_EFS]<refresh status ERROR >\n")
TRACE_MSG(EFS_TASK_366_112_2_18_1_30_19_32,"[efs_task1]: write when refresh %d id %d count 0x%xn")
TRACE_MSG(EFS_TASK_479_112_2_18_1_30_19_33,"[efs_task1]: begin to sleep 300ms :0x%x")
TRACE_MSG(NV_SYS_484_112_2_18_1_30_21_34,"efs:write nvitem to cache failed. %d")
TRACE_MSG(NV_SYS_713_112_2_18_1_30_21_35,"Now Update Nvitem ")
TRACE_MSG(NVITEM_CACHE_269_112_2_18_1_30_22_36,"verify_cache_data failed: ptr = 0x%x\n")
TRACE_MSG(NVITEM_CACHE_277_112_2_18_1_30_22_37,"invalid length %d ptr = 0x%x\n")
TRACE_MSG(NVITEM_CACHE_380_112_2_18_1_30_22_38,"[DRV_EFS]<WARNING> invalid size ")
TRACE_MSG(NVITEM_CACHE_457_112_2_18_1_30_23_39,"[DRV_EFS]<WARNING> invalid size")
TRACE_MSG(NVITEM_CACHE_480_112_2_18_1_30_23_40,"[DRV_EFS]<ERROR> invalid size..")
TRACE_MSG(NVITEM_CACHE_509_112_2_18_1_30_23_41,"EFS: write from cache failed. %d")
TRACE_MSG(NVITEM_CACHE_664_112_2_18_1_30_23_42,"verify_cache_data failed: id %d ptr = 0x%x\n")
TRACE_MSG(NVITEM_CACHE_669_112_2_18_1_30_23_43,"EFS: Has flush %d, is writting now\n")
TRACE_MSG(NVITEM_CACHE_721_112_2_18_1_30_23_44,"EFS: Has flush %d, but cant free it(%d/%d)")
TRACE_MSG(NVITEM_CACHE_735_112_2_18_1_30_23_45,"efs: flush all cache")
TRACE_MSG(UPDATE_FIXNV_634_112_2_18_1_30_25_46,"[Verify_fixnv] Both Fixnv and Fixnv Copy are bad!Now just goahead For Luck\r\n")
END_TRACE_MAP(MS_REF_EFS_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_EFS_TRC_H_


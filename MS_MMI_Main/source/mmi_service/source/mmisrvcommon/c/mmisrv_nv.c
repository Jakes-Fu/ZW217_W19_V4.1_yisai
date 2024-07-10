/*****************************************************************************
** File Name:      mmisrv_nv.c                                              *
** Author:                                                                   *
** Date:           08/13/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/13/2012        bin.ji              Creat
******************************************************************************/
#define MMISRV_NV_C


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mn_type.h"
#include "mmisrv_module.h"
#include "mmisrv_nv.h"
#include "mmisrv_nv_internal.h"
#include "mmicom_data.h"
#include "nvitem.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL BOOLEAN       s_is_mmisrv_nv_reg_done = FALSE;
LOCAL uint16        s_mmisrv_nv_max_item[MMISRV_MAX_MODU_NUM] = {0};  // 每个模块最大的NV的数目
LOCAL const uint16  *s_mmisrv_nv_len[MMISRV_MAX_MODU_NUM];            //每个模块各个nv的长度

#define MMISRV_NV_DEF(nv_id, nv_item_size)          nv_item_size,

const uint16 s_mmisrv_sample_nv_len[] =
{
	1, //MMISRVSAMPLE_NV_START
#include "mmisrvsample_nv_def.h"
};

#undef MMISRV_NV_DEF

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get module max nv item num 
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL uint16 GetMMISRVNVMAXItem(uint16 module_id)
{
    SCI_ASSERT(MMISRV_MAX_MODU_NUM > module_id); /*assert verified*/

    return s_mmisrv_nv_max_item[module_id];
}

/*****************************************************************************/
//  Description : Read NV info by id  
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMISRV_ReadNVItem(uint32 item_id, void * buffer_ptr)
{
    uint16 module_id = 0;
    uint16 nv_offset = 0;
    uint16 true_item_id = MMI_NV_USER_BASE;
    uint16 i = 0;
    NVITEM_ERROR_E reslut = NVERR_NONE;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_SUCCESS;

    SCI_ASSERT(s_is_mmisrv_nv_reg_done); /*assert verified*/

    module_id = MMICOM_GET_WORD_HB(item_id);
    nv_offset = MMICOM_GET_WORD_LB(item_id);

    SCI_ASSERT(MMISRV_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(GetMMISRVNVMAXItem(module_id) > nv_offset); /*assert verified*/

    if (0 != module_id) /*NV base is MMI_NV_USER_BASE*/
    {
        for (i = 1; i <= module_id; i++)
        {
            true_item_id = true_item_id + GetMMISRVNVMAXItem(i - 1); /*calculate module NV base*/
        }
    }

    true_item_id = true_item_id + nv_offset;    /*calculate true NV id*/

    SCI_ASSERT((true_item_id <= (MMI_NV_USER_BASE + MAX_MMI_NV_USER_ITEM_NUM)) && (true_item_id >= MMI_NV_USER_BASE)); /*MAX NV item*/ /*assert verified*/

    reslut = EFS_NvitemRead( true_item_id, s_mmisrv_nv_len[module_id][nv_offset], buffer_ptr );

    if( NVERR_NONE != reslut )
    {
        ret_value = MN_RETURN_FAILURE;
    }

    return ret_value;
}

/*****************************************************************************/
//  Description : write NV info by id  
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_WriteNVItem(uint32 item_id, void * buffer_ptr)
{
    uint16 module_id = 0;
    uint16 nv_offset = 0;
    uint16 true_item_id = MMI_NV_USER_BASE;
    uint16 i = 0;

    SCI_ASSERT(s_is_mmisrv_nv_reg_done); /*assert verified*/

    module_id = MMICOM_GET_WORD_HB(item_id);
    nv_offset = MMICOM_GET_WORD_LB(item_id);

    SCI_ASSERT(MMISRV_MAX_MODU_NUM > module_id); /*assert verified*/
    SCI_ASSERT(GetMMISRVNVMAXItem(module_id) > nv_offset);  /*assert verified*/

    if (0 != module_id)/*NV base is MMI_NV_USER_BASE*/
    {
        for (i = 1; i <= module_id; i++)
        {
            true_item_id = true_item_id + GetMMISRVNVMAXItem(i - 1); /*calculate module NV base*/
        }
    }

    true_item_id = true_item_id + nv_offset; /*calculate module NV base*/

    SCI_ASSERT((true_item_id <= MMI_NV_MAX_NUM) && (true_item_id >= MMI_NV_USER_BASE)); /*MAX NV item*/ /*assert verified*/
    SCI_ASSERT(s_mmisrv_nv_len[module_id][nv_offset] != 0); /*assert verified*/

    EFS_NvitemWrite( true_item_id, s_mmisrv_nv_len[module_id][nv_offset], buffer_ptr, FALSE);
}

/*****************************************************************************/
//  Description : get max id
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISRV_GetMaxId(void)
{
	return MMI_NV_MAX_NUM;
}

/*****************************************************************************/
//  Description : get end id
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISRV_GetEndId(void)
{
    uint16 true_item_id = MMI_NV_USER_BASE;
    uint16 i = 0;

	SCI_ASSERT(s_is_mmisrv_nv_reg_done); /*assert verified*/

    for (i = 0; i <= MMISRV_MODULE_MAX_NUM; i++)
    {
        true_item_id = true_item_id + GetMMISRVNVMAXItem(i); /*calculate module NV base*/
    }

    SCI_ASSERT((true_item_id <= MMI_NV_MAX_NUM) && (true_item_id >= MMI_NV_USER_BASE)); /*MAX NV item*/ /*assert verified*/

    return true_item_id;
}

/*****************************************************************************/
//  Description : register sample module nv len and  max item
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_RegSampleNv(void)
{    
    MMISRV_RegModuleNv(MMISRV_MODULE_SAMPLE, s_mmisrv_sample_nv_len, ARR_SIZE(s_mmisrv_sample_nv_len));
}

/*****************************************************************************/
//  Description : test sample nv
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_TestSampleNVItem(void)
{  
	uint32 test_read_value = 0;
	uint32 test_write_value = 0;
	MN_RETURN_RESULT_E  nv_return = MN_RETURN_FAILURE;

	MMISRVNV_READ(MMISRVSAMPLE_NV_TEST, &test_read_value, nv_return);
	test_write_value = 0x12345;
	MMISRVNV_WRITE(MMISRVSAMPLE_NV_TEST, &test_write_value);
	MMISRVNV_READ(MMISRVSAMPLE_NV_TEST, &test_read_value, nv_return);
	test_write_value = 0x6666;
	MMISRVNV_WRITE(MMISRVSAMPLE_NV_TEST, &test_write_value);
	MMISRVNV_READ(MMISRVSAMPLE_NV_TEST, &test_read_value, nv_return);
/*
	MMISRV_ReadNVItem(MMISRVSAMPLE_NV_TEST, &test_read_value);
	MMISRV_WriteNVItem(MMISRVSAMPLE_NV_TEST, &test_write_value);
	MMISRV_ReadNVItem(MMISRVSAMPLE_NV_TEST, &test_read_value);
	*/
}

/*****************************************************************************/
//  Description : set reg modle nv start flag
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_RegModuleNvStart (void)
{
    s_is_mmisrv_nv_reg_done = FALSE;

    return TRUE;
}

/*****************************************************************************/
//  Description : set reg modle nv end flag
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISRV_RegModuleNvEnd (void)
{
    s_is_mmisrv_nv_reg_done = TRUE;
    return TRUE;
}

/*****************************************************************************/
//  Description : register module nv len and max nv item 
//  Global resource dependence : none
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISRV_RegModuleNv(uint16 module_id, const uint16 nv_len[], uint16 nv_max_item)
{
    SCI_ASSERT(MMISRV_MAX_MODU_NUM > module_id && !s_is_mmisrv_nv_reg_done); /*assert verified*/

    s_mmisrv_nv_len[module_id] = (const uint16 *)nv_len;
    s_mmisrv_nv_max_item[module_id] = nv_max_item;
   // s_mmi_nv_num += nv_max_item ;   //count mmi nv item num
}

/*****************************************************************************/
//     Description : get sum size of all modules nv size
//    Global resource dependence : 
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMISRV_GetAllModuleSumNvSize(void)
{
    uint32 all_module_sum_size = 0;
    uint32 cur_module_sum_size = 0;
//    uint32 all_nv_item_num = 0;
    uint16 module_id = 0;
    uint16 i = 0;
    
    SCI_ASSERT(s_is_mmisrv_nv_reg_done); /*assert verified*/

    for(module_id = 0; module_id < MMISRV_MAX_MODU_NUM; module_id ++)
    {
        /* get sum nv item */
//        all_nv_item_num = all_nv_item_num + GetMMISRVNVMAXItem(module_id); 

        /* get sum nv size */
        cur_module_sum_size = 0;

        for(i = 0; i < GetMMISRVNVMAXItem(module_id); i ++)
        {
            /* sum every item nv size */
            cur_module_sum_size += s_mmisrv_nv_len[module_id][i];
//            SCI_TRACE_LOW("[MMISRV_NV.c]:NV_ID=%d size=%d", i,s_mmisrv_nv_len[module_id][i]);
        }

        /* sum current module nv sum size */
        all_module_sum_size += cur_module_sum_size;
    }
//    SCI_TRACE_LOW("[MMISRV_NV.c]: total module nv size = %d, all nv item num = %d", all_module_sum_size, all_nv_item_num);
    SCI_TRACE_LOW("[MMISRV_NV.c]: total module nv size = %d", all_module_sum_size);

    return all_module_sum_size;
}

/*****************************************************************************/
//  Description :  get nv item num of all mmi mudule 
//  Global resource dependence : none
//  Author: 		bin.ji
//  Note:		
/*****************************************************************************/
PUBLIC uint32 MMISRV_GetAllMMINVItem(void)
{   
    uint32  mmi_nv_num  = 0;
    uint32  module_id = 0 ;

    SCI_ASSERT(s_is_mmisrv_nv_reg_done); /*assert verified*/

	for(module_id = 0; module_id < MMISRV_MAX_MODU_NUM; module_id ++)
    {
        /* get sum nv item */
        mmi_nv_num += GetMMISRVNVMAXItem(module_id); 
	}

	return mmi_nv_num;
}
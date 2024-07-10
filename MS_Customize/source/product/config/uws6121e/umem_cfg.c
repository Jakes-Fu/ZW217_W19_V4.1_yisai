/******************************************************************************
 ** File Name:      Umem_main.c                                                             *
 ** Author:           tao.feng                                            *
 ** DATE:            05/07/2009                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    MMCCard Driver virtual interface layer.      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 05/07/2009     tao.feng         Create.                                   *
 ******************************************************************************/

#include "sci_api.h"
#include "umem_cfg.h"
#include "flash.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/**---------------------------------------------------------------------------*
 **                        Customer Dedicated Macro Definition                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                        Customer  Option Macro Definition                         *
 **---------------------------------------------------------------------------*/
#define  UMEM_HIDEN_SIZE          ((uint32)FLASH_GetUmemSize()/5)       //customer define udisk rsv sec num.should be modified according to your need!!!!
#undef   UMEM_STATIC_MEM_EN                        						//customer define udisk mem source,if undef will use dynamic mem,recommand to use dynamic mem

/**---------------------------------------------------------------------------*
 **                        Dedicated  Macro Definition                                   *
 **---------------------------------------------------------------------------*/

 
/**---------------------------------------------------------------------------*
 **                         LOCAL Variables                                  *
 **---------------------------------------------------------------------------*/
#ifdef UMEM_STATIC_MEM_EN
#define UMEM_PHY_SEC_SIZE   	   0x40000      //customer should config phy sec size if you want to use static mem!!!! 
LOCAL uint8   umem_phy_sec[UMEM_PHY_SEC_SIZE] ;
#endif

LOCAL UMEM_CUSTDEFINE_T s_umem_cust_info;


/**---------------------------------------------------------------------------*
 **                         LOCAL Variables                                  *
 **---------------------------------------------------------------------------*/
//const uint32 g_umem_base_addr = UMEM_BASE_ADDR;
//const uint32 g_umem_size = UMEM_SIZE;


/********************************************************************
*     Description: This function is used to get customer defined info
*	Dependence: none
*	Author: tao.feng
*	Param:
*          none
*	Result:
*	    customer defined info.
********************************************************************/
PUBLIC UMEM_CUSTDEFINE_T  *Umem_GetCustInfo(void)
{
    s_umem_cust_info.umem_attr[UMEM_TYPE_FS]   = UMEM_ATTR_RW;

    s_umem_cust_info.umem_garbage_collect_en = SCI_FALSE;
#ifdef UMEM_STATIC_MEM_EN
    s_umem_cust_info.umem_phy_buf_ptr    = (uint8*)&umem_phy_sec[0];
    s_umem_cust_info.umem_static_mem_en = SCI_TRUE;
#else
    s_umem_cust_info.umem_static_mem_en = SCI_FALSE;
#endif
    s_umem_cust_info.umem_hidden_part_ration = 5;//5:20%, 1:100%(full use)

#ifdef UMEM_NEW_MECHNISM
    s_umem_cust_info.umem_new_scheme = SCI_TRUE;
#endif
    return (UMEM_CUSTDEFINE_T*)&s_umem_cust_info;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif



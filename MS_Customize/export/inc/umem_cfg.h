/******************************************************************************
 ** File Name:    umem_cfg.h                                                    *
 ** Author:       tao.feng                                                 *
 ** DATE:         5/05/2005                                                    *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 5/05/2009      tao.feng       Create.                                     *
 ******************************************************************************/
#ifndef UMEM_CFG_H
#define UMEM_CFG_H
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#undef   UMEM_STATIC_MEM_EN                      //customer define udisk mem source,if undef will use dynamic mem,recommand to use dynamic mem
#undef   UMEM_CTRL_SEC_EN                         //if define ctrl info will stor in ctrl sect, if undef ctrl info will stor in running nv.


/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

typedef enum
{
    UMEM_ATTR_RW=0,//enabel read write
    UMEM_ATTR_R,//only read 
    UMEM_ATTR_MAX
}SEC_UMEM_ATTR_E;

typedef enum
{
    UMEM_TYPE_FS =0,
    UMEM_TYPE_HFS,
    UMEM_TYPE_MAX
}UMEM_TYPE_E;

typedef struct UMEM_CUSTDEFINE_Tag
{
    uint8    *umem_phy_buf_ptr;
    uint32   umem_hidden_part_ration;
    BOOLEAN  umem_garbage_collect_en;
    BOOLEAN  umem_static_mem_en;
#ifdef UMEM_NEW_MECHNISM
	BOOLEAN  umem_new_scheme;
#endif
    SEC_UMEM_ATTR_E umem_attr[UMEM_TYPE_MAX];
}UMEM_CUSTDEFINE_T;


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/********************************************************************
*     Description: This function is used to get customer defined info
*	Dependence: none
*	Author: tao.feng
*	Param:
*          none
*	Result:
*	    customer defined info.
********************************************************************/
PUBLIC UMEM_CUSTDEFINE_T  *Umem_GetCustInfo(void);


#endif
// End 


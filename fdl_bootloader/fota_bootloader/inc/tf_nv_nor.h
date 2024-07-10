/******************************************************************************
 ** File Name:      TF_nv.h                                                *
 ** Author:           tao.feng                                                 *
 ** DATE:            20/12/2010                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces of TF download process  *
            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 20/12/2010     tao.feng         Create.                                   *
 **                                                                           *
 ******************************************************************************/


#ifndef _TF_NV_H_
#define _TF_NV_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "tf_cfg.h"


/**---------------------------------------------------------------------------*
 **                         Macro Defination                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Struct Defination                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         	Export Func                                   *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Discription: TF init nv status
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void TF_Initialize_NV_Status(void);

/*****************************************************************************/
//  Discription: TF update fixnv
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Fixnv(char * fix_nv_address,uint32 fix_nv_size);

/*****************************************************************************/
//  Discription: TF update product info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Productinfo(char * prod_address,uint32 write_size);

/*****************************************************************************/
//  Discription: TF update product info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Productinfo(char * udisk_address, uint32 image_size);

/*****************************************************************************/
//  Discription: TF common update func, will be called 
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Common(char * buf,uint32 write_size,  FILE_ID_TYPE type);

/*****************************************************************************/
//  Discription: TF update product info
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Udisk_Image(uint32 addr, char * buf, uint32 size, uint32 total_file_size);

/*****************************************************************************/
//  Discription: TF update sector by slave
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC TFLOAD_ERR_TYPE TF_Update_Sector_By_Slave(void);


#endif //_TF_NV_H_
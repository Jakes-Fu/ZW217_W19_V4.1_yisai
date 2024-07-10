/*****************************************************************************
 **  File Name:    effuse_drv.h                                                 *
 **  Author:       Jenny Deng                                                * 
 **  Date:         20/10/2009                                                *
 **  Copyright:    2009 Spreadtrum, Incorporated. All Rights Reserved.       *
 **  Description:  This file defines the basic operation interfaces of       *
 **                EFuse initilize and operation. It provides read and         *
 **                writer interfaces of 0~5 efuse. Efuse 0 for Sn block.     *
 **                Efuse 1 to 4 for Hash blocks. Efuse 5 for control block.  *
 *****************************************************************************
 *****************************************************************************
 **  Edit History                                                            *
 **--------------------------------------------------------------------------*
 **  DATE               Author              Operation                        *
 **  20/10/2009         Jenny.Deng          Create.                          *
 **  26/10/2009         Yong.Li             Update.                          *
 **  30/10/2009         Yong.Li             Update after review.             *
 *****************************************************************************/
 
#ifndef _EFuse_DRV_H
#define _EFuse_DRV_H

#include "sci_types.h"

/***********************structure define**************************************/

// the function return value
typedef enum
{
    EFuse_RESULT_SUCCESS        	= 0x00,
    EFuse_READ_FAIL             	= 0x01,
    EFuse_WRITE_FAIL            	= 0x02,
    EFuse_WRITE_SOFT_COMPARE_FAIL   = 0x03, // compare write data with read data after write
    EFuse_WRITE_VERIFY_FAIL     	= 0x04,
    EFuse_PARAMETER_ERROR       	= 0x05,
    EFuse_ID_ERROR              	= 0x06,    //block id is not between 0 and 5
    EFuse_HAS_WRITEED_ERROR			= 0x07,
    EFuse_PTR_LENGTH_ERROR      	= 0x08,
    EFuse_WRITE_HARD_COMPARE_FAIL   = 0x09, // compare write data use hardware compare      
    EFuse_WRITE_SOFT_HARD_COMPARE_FAIL   = 0x0A, // compare write data use hardware compare    
    EFuse_BIST_FAIL					= 0xB,
    EFuse_BIST_TIMEOUT			    = 0xC,
    EFuse_NOT_LOCK				    = 0xD,
    EFuse_LOCKED				    = 0xE
}EFuse_RETURN_E;

// the EFuse read mode value, bit [6:5] of efuse_ctrl
typedef enum
{
    EFuse_USER_MODE     = 0,
    EFuse_MARGIN_MODE_2 = 2,
    EFuse_MARGIN_MODE_1 = 3    
}EFuse_READ_MODE_E;

// the control bit of control block, bit 1 for secure_boot
typedef enum 
{
    UNDEFINED        = 0,
    EFuse_SECURE_BOOT  = 1,
    EFuse_MAX_CTRL_BIT = 2
}EFuse_CONTROL_BIT_E;

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
// EFuse blocks of efuse
typedef enum
{
    EFuse_MIN_ID        = 0,
    EFuse_SN_HIGH_ID    = 0,
    EFuse_SN_LOW_ID    =  1,
    EFuse_BOUND_ID      = 2,
    EFuse_HASH_STARTID  = 9,
    EFuse_HASH_ENDID    = 13,
    EFuse_JTAG_SET      = 15,
    EFuse_MAX_ID        = 15
}EFuse_BLOCK_ID_E;
#else
// EFuse blocks of efuse
typedef enum
{
    EFuse_MIN_ID        = 0,
    EFuse_SN_ID         = EFuse_MIN_ID,
    EFuse_HASH_STARTID  = 2,
    EFuse_HASH_ENDID    = 6,
    EFuse_CONTRL_ID     = 7,
    EFuse_MAX_ID        = 8
}EFuse_BLOCK_ID_E;
#endif
/***********************function declaration**********************************/


//*****************************************************************************
// Description : initilize EFuse
// Global resource dependence :
// Author: Jenny Deng
// Note:   
//*****************************************************************************
PUBLIC void EFuseInitilize(void);

//*****************************************************************************
// Description : close EFuse
// Global resource dependence :
// Author: Jenny Deng
// Note:   
//*****************************************************************************
PUBLIC void EFuseClose(void);

//*****************************************************************************
// Description : read data from EFuse block
// Global resource dependence :
// Author: Jenny Deng
// Note:   
// OUTPUT: parameter r_data_ptr is the read data pointer
// RETURN VALUE: EFuse_ID_ERROR         when block_id is out of value 0~5
//               EFuse_READ_FAIL        when fail to read block data
//               EFuse_RESULT_SUCCESS   when read successfully
//*****************************************************************************
PUBLIC EFuse_RETURN_E EFuseRead(            
        unsigned int block_id,                 // the selected effuse block id
        unsigned int* r_data_ptr               // pointer of block data read from EFuse block
        );



//*****************************************************************************
// Description : read EFuse Hash blocks data (block 1~4)
// Global resource dependence :
// Author: Jenny Deng
// Note:   
// OUTPUT: parameter des_data_ptr is the read data pointer
// RETURN VALUE: EFuse_ID_ERROR        when block_id is out of value 0~5
//               EFuse_READ_FAIL       when fail to read block data
//               EFuse_RESULT_SUCCESS  when read successfully
//*****************************************************************************
PUBLIC EFuse_RETURN_E EFuseReadHash(      
        unsigned int* des_data_ptr        //the pointer of destination data                      
        );



       

#endif

/*****************************************************************************
 **  File Name:    efuse_drv.c                                                 *
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
#include "sci_types.h"
#include "efuse_drv.h"
#include "chip_plf_export.h"

/***********************macro define******************************************/
#define EFUSE_DATA_RD            (EFUSE_REG_BASE + 0x0000)
#define EFUSE_DATA_WR			 (EFUSE_REG_BASE + 0x0004)

#define EFUSE_BLOCK_INDEX		 (EFUSE_REG_BASE + 0x0008)
#define EFUSE_MODE_CTRL			 (EFUSE_REG_BASE + 0x000c)
#define EFUSE_PGM_PARA			 (EFUSE_REG_BASE + 0x0010)
#define EFUSE_STATUS			 (EFUSE_REG_BASE + 0x0014)
#define EUSE_MEM_BLOCK_FLAGS  	 (EFUSE_REG_BASE + 0x0018)
#define EUSE_MEM_BLOCK_FLAGS_CLR (EFUSE_REG_BASE + 0x001c)
#define EFUSE_MAGIC_NUMBER		 (EFUSE_REG_BASE + 0x0020)

#define EFuse_LOCK_BIT           BIT_31
//efuse mode ctrl register bit define
#define EFUSE_PG_START			 BIT_0
#define EFUSE_RD_START			 BIT_1
#define EFUSE_BIST_START		 BIT_31

//efuse block index bit define 
#define READ_INDEX_OFFSET		 (0)
#define PGM_INDEX_OFFSET		 (16)
#define BIST_START_INDEX_OFFSET	 (26)
#define BIST_SIZE	 (29)

//PGM_PARA register bit define
#define EFUSE_AUTO_TEST_EN		 BIT_16
#define CLK_EFS_EN				 BIT_28
#define EFS_VDD_ON				 BIT_29
#define PGM_EN					 BIT_31

//efuse status register bit define
#define EFUSE_PRG_BUSY    	 	 BIT_0
#define EFUSE_READ_BUSY    	 	 BIT_1
#define EFUSE_BIST_FAIL			 BIT_4
#define EFUSE_BIST_BUSY			 BIT_5

//Mem_block_flags register bit define
#define EFUSE_BLOCK_ERR_FLAG	 BIT_8

//timeout define 
#define EFUSE_READ_TIMEOUT       2 //ms
#define EFUSE_WRITE_TIMEOUT      3 //ms

/***********************function declaration**********************************/

//*****************************************************************************
// Description : initilize EFuse
// Global resource dependence :
// Author: Jenny Deng
// Note:   
//*****************************************************************************
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

//*****************************************************************************
// Description : EFuse init, before use efuse must call this function to init efuse
// Global resource dependence
// Author: Junqiang.wang
// Note:   
//*****************************************************************************
PUBLIC void EFuseInitilize(void)
{
    uint32 i=0;
	uint32 old_tick = 0;
    uint32 new_tick = 0;
	
    /*Purpose:To enable Efuse Module*/
    REG_WRITE(APB_EB0_SET, EFUSE_EB_SET);	
  	
    /*Purpose:Reset efuse */
    REG_WRITE(APB_RST0_SET, EFUSE_SOFT_RST_SET);
    
    /* Delay some time */
    for (i = 0; i < 100; i++);
    
    /*Purpose:Reset efuse */    
    REG_WRITE(APB_RST0_CLR, EFUSE_SOFT_RST_CLR);    	
    
    /*Purpose:To power on Efuse Module*/
    REG_OR(EFUSE_PGM_PARA,EFS_VDD_ON);
    
    old_tick = fdl_GetTickCount();
	new_tick = fdl_GetTickCount();
	
	/*Purpose:To clk on Efuse Moudle*/
	REG_OR(EFUSE_PGM_PARA,CLK_EFS_EN);
	
	/*Purpose:To enable program operation to Efuse*/
	REG_OR(EFUSE_PGM_PARA,PGM_EN);
	
    while(((new_tick - old_tick) < 2))   //delay 1ms
    {
        new_tick = fdl_GetTickCount();
    }
      
        
}
//*****************************************************************************
// Description : close EFuse
// Global resource dependence :
// Author: Junqiang.wang
// Note:   
//*****************************************************************************
PUBLIC void EFuseClose(void)
{
	
	/*Purpose:To close efuse clk*/
	REG_AND(EFUSE_PGM_PARA,~CLK_EFS_EN);
	
	/*Purpose:To disable efuse program*/
	REG_AND(EFUSE_PGM_PARA,~PGM_EN);
	
	/*Purpose:To shut down the efuse power */
	REG_AND(EFUSE_PGM_PARA,~EFS_VDD_ON);

	/*Purpose:To disable Efuse Moudle*/
	REG_WRITE(APB_EB0_CLR, EFUSE_EB_CLR);
}

//*****************************************************************************
// Description : read data from EFuse block
// Global resource dependence :
// Author: Junqiang.wang
// Note:   
// OUTPUT: parameter r_data_ptr is the read data pointer
// RETURN VALUE: EFuse_ID_ERROR         when block_id is out of value 0~5
//               EFuse_READ_FAIL        when fail to read block data
//               EFuse_RESULT_SUCCESS   when read successfully
//*****************************************************************************
PUBLIC EFuse_RETURN_E EFuseRead(            
        unsigned int block_id,                 // the selected efuse block id
        unsigned int* r_data_ptr               // pointer of block data read from EFuse block
        )
{
    uint32 old_tick = 0;
    uint32 new_tick = 0;
    uint32 sts = 0; //states of efuse
    EFuse_RETURN_E result = EFuse_RESULT_SUCCESS;  // return value
    
    /* check the block_id */
    if(((int32)block_id < EFuse_MIN_ID) || ((int32)block_id > EFuse_MAX_ID))
    {
        return EFuse_ID_ERROR;
    }
	
	/*Purpose:To config Efuse block index register,
	  read index must be the same of the write index
	*/
	REG_WRITE(EFUSE_BLOCK_INDEX,(block_id << READ_INDEX_OFFSET) | (block_id << PGM_INDEX_OFFSET));
	
	/*Purpose:To send read commmand to Efuse Module*/
	REG_OR(EFUSE_MODE_CTRL,EFUSE_RD_START);
	
	/*Purpose:Polling the Efuse status,to see if Efuse is in Reading-Busy state*/
	REG_READ(EFUSE_STATUS,sts);
	sts = sts & EFUSE_READ_BUSY;
	
	old_tick = fdl_GetTickCount();
	while((EFUSE_READ_BUSY == sts))  
    {
    
		/*Purpose:Polling the Efuse status,to see if Efuse is in Reading-Busy state*/
		REG_READ(EFUSE_STATUS,sts);
		sts = sts & EFUSE_READ_BUSY;
		
        new_tick = fdl_GetTickCount();
        if((new_tick - old_tick) > EFUSE_READ_TIMEOUT)
        {
        	break;
        }
    }  
	if(sts == EFUSE_READ_BUSY)
	{
		result =  EFuse_READ_FAIL;
	}
	else 
	{
		*r_data_ptr = *(volatile uint32 *)(EFUSE_DATA_RD);
	}
	
    return result;
}



//*****************************************************************************
// Description : read EFuse Hash blocks data (block 1~4)
// Global resource dependence :
// Author: junqiang wang
// Note:   
// OUTPUT: parameter des_data_ptr is the read data pointer
// RETURN VALUE: EFuse_ID_ERROR        when block_id is out of value 0~5
//               EFuse_READ_FAIL       when fail to read block data
//               EFuse_RESULT_SUCCESS  when read successfully
//*****************************************************************************
PUBLIC EFuse_RETURN_E EFuseReadHash(      
        unsigned int* des_data_ptr              // the pointer of destination data                      
        )
{
    uint32 id  = 0;
    EFuse_RETURN_E res = EFuse_RESULT_SUCCESS;
    
    for(id = EFuse_HASH_STARTID; id <= EFuse_HASH_ENDID; id++)
    {
        res = EFuseRead(id, des_data_ptr);  // read data 
        
        if (EFuse_RESULT_SUCCESS != res)
        {
            return res;
        }
        
        //*des_data_ptr &= (unsigned int)(~EFuse_LOCK_BIT);    // hash BIT_31 set 0
        des_data_ptr++;        
    }

    return res;
}


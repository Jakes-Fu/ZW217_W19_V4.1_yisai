/******************************************************************************
 ** File Name:      normcp_spec.c							                                                   *
 ** Author:         	Yiyue.he										                                                   *
 ** DATE:           16/12/2008                                                									*
 ** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.     							*
 ** Description:    NORMCP--|_ Norflash   Info  Descriptions 			                  					*					
 **                                   |_ Psram         													*
 **     			The info in this file should be indepedent with all the platform.						*
 **				They are only abstracted from the attribute borned in the flash and psram, 			*
 **                 	which can get from the spec released by the manufactory.						*
 **                                                                           										*
 **Function:		The content in the file are used by the platform memory interface 's Driver			*
 **			      [called EMC OR EMI OR MC etc.], and the flash/Sram config to platform code 		*
 **				[called custom mem cfg etc.]. The accessing way  can be variable as the designer's 	*
 **				reasonable plan.        														*
 **																						*
 **Use:			This file is expected as developing trend, which is builded up from different normcps 	*
 **				one by one, when the mcp item has been described, just use it,if not ,you can refer 	*
 **				to mcp spec,then write a new mcp item like a existed one.						*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           		NAME             DESCRIPTION                               							*
 ** 16/12/2008     	Yiyue.He         	 Create for New Emc-Mem code struct .                                   							*
 ******************************************************************************/
 


/**---------------------------------------------------------------------------	*
 **                         				Dependencies                                      			   	*
 **---------------------------------------------------------------------------	*/
#include "normcp_spec.h"


/**---------------------------------------------------------------------------	*
 **                        				 Compiler Flag                                     				*
 **---------------------------------------------------------------------------	*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


//normcp spec info pool
#include "normcp_k5l2833ata_af66.c" 
#include "normcp_s71ws128pc0hf3sr.c"
#include "normcp_m36l0r7050u3.c"
#include "normcp_m36l0r7050t4zaq.c"
#include "normcp_k5n2833atm_aq11.c"
#include "normcp_PF38F3050M0Y3Q.c"
#include "normcp_PF38F3050M0Y0YE.c"
#include "normcp_m36l0r8060u0.c"
#include "normcp_PF38F3040L0YUQ3.c"
#include "normcp_burst_not_support.c"  
#include "normcp_k5l5666.c" 
#include "normcp_k5n5629atb.c" 

//@add new one "normcp_xxxx_xxx.c"at here           
 









#include "flash.h"
/*****************************************************************************/
// 	Description :
//  By manu_id,devc_id and extend_id,find correct g_normcp_entity_table item.
/*****************************************************************************/
PUBLIC NORMCP_ENTITY_PTR NORMCP_GetEntity(
									uint32 manu_id, 
									uint32 devc_id, 
									uint32 extend_id,
									uint32 *index)
{
	uint32 table_index 				 = 0;
	uint32 table_num 				 = 0; 
	NORMCP_ENTITY_PTR entity_ptr 	 = PNULL;
	NORMCP_ENTITY_PTR entity_tab_ptr = NORMCP_GetEntityTab(&table_num);
	
	for(table_index=0; table_index < table_num; ++table_index)
	{
		if( (  manu_id   == entity_tab_ptr[table_index].normcp_spec_ptr->manu_id)
			&&(devc_id   == entity_tab_ptr[table_index].normcp_spec_ptr->devc_id)
			&&(extend_id == entity_tab_ptr[table_index].normcp_spec_ptr->extend_id))
		{
			//Find Nor Type			
			entity_ptr = (NORMCP_ENTITY_PTR)&entity_tab_ptr[table_index];
			break;
		}
	}

	if(PNULL != index)
	{
		if(table_index == table_num) 
		{
			*index = 0xFFFFFFFF; 
		}
		else
		{
			*index = table_index;
		}
	}
	
	return entity_ptr;
}


#pragma arm section code = "NORMCP_SPEC_FUNC_IN_SRAM"
/*****************************************************************************/
// 	Description :
//  Get extend id to distinguish different flash with same manu_id and dev_id
//  Using CFI in autoselect mode.
/*****************************************************************************/
uint32 FLASH_GetExtID(uint32 manu_id,uint32 dev_id)  
{
    FLASH_PTR_TYPE  ptr = (FLASH_PTR_TYPE)0x0;
    uint32          b_1, b_2, b_3;
    uint32          ext_id;
    
    ext_id = 0x0;
    //distinguish spansion PL127N between PL127J 
    if((0x7E2000 == dev_id)&&(FLASH_AMD_ID == manu_id))
    {
        RESET_FLASH;    						/*lint !e413*/
        // enter autoselect mode
        *(ptr + 0x55) = 0x98;  					/*lint !e413*/

        b_1 = *(ptr + 0x20);   					/*lint !e413*/
        b_2 = *(ptr + 0x2a);  					/*lint !e413*/
        b_3 = *(ptr + 0x58);  					/*lint !e413*/
        if( (0x9 == b_1) && (0x6 == b_2) && (0xB == b_3) )
            ext_id = 0x1; //PL127N mirrorbit 
        else
            ext_id = 0x0;
    }
        //distinguish  ST T2 between ST T1 
    if((0x88C4 == dev_id)&&(FLASH_ST_ID == manu_id))
    {
        RESET_FLASH;       						/*lint !e413*/
        // enter autoselect mode
        *(ptr + 0x55) = 0x98;    				/*lint !e413*/

        b_1 = *(ptr + 0x127);  					/*lint !e413*/
        
        if(0x3 == b_1)
            ext_id = 0x1; //page len_4
        else if(0x4 == b_1)
            ext_id = 0x0; //page len_8
    }
    //distinguish  SAMSUNG K5N2731 and K5N6331 
    if((0x257e == dev_id)&&(FLASH_SAMSUNG_ID == manu_id))
    {
        RESET_FLASH;       						/*lint !e413*/
        // enter autoselect mode
        *(ptr + 0x555) = 0xAA;  				/*lint !e413*/
        *(ptr + 0x2AA) = 0x55;  				/*lint !e413*/
    
        // enter autoselect mode
        *(ptr + 0x555) = 0x90;   				/*lint !e413*/
    
         *(ptr + 0x00);  						/*lint !e413*/
         *(ptr + 0x01);  						/*lint !e413*/
      
         b_1 = *(ptr + 0xE);  					/*lint !e413*/
        
        if(0x2506 == b_1)
            ext_id = 0x1; //K5L6331CAA
        else 
            ext_id = 0x0; //_K5L2731CAM
    }

    // distinguish  SAMSUNG K5N2866-BQ12 and K5N2833-AF66 
    if((0x2404 == dev_id)&&(FLASH_SAMSUNG_ID == manu_id))
    {
         RESET_FLASH;               /*lint !e413*/			

        // enter CFI mode
         *(ptr + 0x55) = 0x98;    /*lint !e413*/
        
        //read max clk freq in CFI mode
        b_1 = *(ptr + 0x4E);      /*lint !e413*/
        
        if(0x6c == b_1)
        {
        	ext_id = 0x1; //K5N2866-BQ12, 108M
        }
        else
        {
        	ext_id = 0x0; //K5N2833-AF66
        }
        
    }
	
    return ext_id;
}
#pragma arm section code

/**---------------------------------------------------------------------------	*
 **                         				Compiler Flag                                     				*
 **---------------------------------------------------------------------------	*/
#ifdef   __cplusplus
    }
#endif 
// end  normcp_spec.c


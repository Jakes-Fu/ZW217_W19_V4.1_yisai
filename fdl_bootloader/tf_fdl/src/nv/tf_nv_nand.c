
#include "nv_item_id.h"
#include "nv_productionparam_type.h"
#include "sci_ftl_api.h"
#include "tf_cfg.h"

//#include "nand_partitionid.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**
#	search nvitem mode: 
#					0: search specified nv id in nv partition
#					1: search the first nv id in nv partition
#					2: search the nv after the specified nv id in nv partition
**/
#define		SEARCH_BY_NVID		0
#define		SEARCH_FIRST_NVID		1
#define		SEARCH_NEXT_NVID		2

/**
#			define the max buffer used to update nv parameters.
**/
#define		MAX_FIXNV_LENGTH		(64*1024)

extern uint16 tf_backup_nvitems[];

LOCAL BOOLEAN tf_findnvoffset(uint16 wId, uint8* pReadbuff,uint32 ReadSize,
                  uint32* pOffset,uint32* pLength)
{
    uint32 dwOffset = 0;
    uint32 dwLength = 0;
    uint8* pTemp = NULL;
    uint16 dwId;
    BOOLEAN bRet = FALSE;
    uint32 dwAlignOffset;
    
    if(pReadbuff == NULL)
    {
        return FALSE;
    }
    
    dwOffset = 4;     // Skip first four bytes,that is time stamp
    pTemp = pReadbuff + dwOffset;
    
    while(dwOffset < ReadSize)
    {
        dwId = ( *pTemp << 8 ) + *(pTemp + 1);
        pTemp += 2;
        dwLength = ( *pTemp << 8 ) + *(pTemp + 1);
        pTemp += 2;
        dwOffset += 4;
        
        // Must be four byte aligned
        dwAlignOffset = (dwLength % 4);
        if(dwAlignOffset != 0)
        {
            dwLength += 4 - dwAlignOffset;
        }
        
        if(dwId == wId)
        {          
             bRet = TRUE;
             break;
        }
        else
        {
            dwOffset += dwLength;
            pTemp += dwLength;
        }
    }

    *pOffset = dwOffset;
    *pLength = dwLength;
    
    return bRet;
}


LOCAL BOOLEAN tf_savenvitem(  uint16   wID, uint8* pReadBuff, uint32 ReadSize, 
					  uint8* pSaveBuff, uint32 SaveSize)

{   
    uint32 dwReadOffset,dwReadLength;
    uint32 dwWriteOffset,dwWriteLength;
    PROD_PARAM_T* 		p_read_prod_param;
    PROD_PARAM_T* 		p_write_prod_param;
	PROD_PERIPHERAL_T* param_read_ptr;
  	PROD_PERIPHERAL_T* param_write_ptr;

    if(tf_findnvoffset( wID, pReadBuff, ReadSize, &dwReadOffset, &dwReadLength))
    {
        if(tf_findnvoffset( wID, pSaveBuff,SaveSize,&dwWriteOffset,&dwWriteLength))
        {            
            if(dwReadLength == dwWriteLength)
            {    
                /* For the NV_PROD_PARAM, we only need backup the lcd ID */
                if(wID == NV_PROD_PARAM)
                {
                      p_read_prod_param = (PROD_PARAM_T*)(pReadBuff + dwReadOffset);
                      param_read_ptr = &(p_read_prod_param->peripheral_param);

                      p_write_prod_param = (PROD_PARAM_T*)(pSaveBuff + dwWriteOffset);
                      param_write_ptr = &(p_write_prod_param->peripheral_param);

                      memcpy(param_write_ptr, param_read_ptr, sizeof(PROD_PERIPHERAL_T));

                }
                else
                {                
                    memcpy( pSaveBuff + dwWriteOffset,
                        pReadBuff + dwReadOffset,
                        dwWriteLength);
                }
                
                return TRUE;
            }
            //else:如果在保留过程中发现某一项在手机中
            //和文件中的长度不一样，则不予保留，直接用文件中的替换。
        }        
    }
    
    return FALSE;    
}

BOOLEAN tf_savebackupnv(uint8* pReadBuff, uint32 ReadSize, 
					  uint8* pSaveBuff, uint32 SaveSize)
{
    uint32 i = 0;
    BOOLEAN bRet = TRUE;

    while(TRUE)
    {
        if(tf_backup_nvitems[i] == INVALID_NVID)
            break;
        
        
        bRet = tf_savenvitem(tf_backup_nvitems[i], pReadBuff, ReadSize, \
            pSaveBuff, SaveSize);
        if(!bRet)
        {
            break;
        }
        
        i++;
    }

    return bRet;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


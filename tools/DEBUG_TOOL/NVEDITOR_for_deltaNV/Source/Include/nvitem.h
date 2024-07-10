#ifndef NVITEM_H
#define NVITEM_H

#include "sci_types.h"

/******************************************************************************
 * Parameter Attribute
 ******************************************************************************/
#define INPUT
#define OUTPUT
#define INOUT
#define	MAGIC_SP09				(0x39305053)
//#define	MAGIC_SP05				(0x35305053) // not used
/******************************************************************************
 * Interface Prototype   
 ******************************************************************************/
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 * Return values of NVITEM_XXX()   
 ******************************************************************************/
typedef enum _NVITEM_ERROR {
    NVERR_NONE  = 0,   			/* Success */    
    NVERR_SYSTEM,      			/* System error, e.g. hardware failure */
    NVERR_INVALID_PARAM,
    NVERR_NO_ENOUGH_RESOURCE,
    NVERR_NOT_EXIST,
    NVERR_ACCESS_DENY,
    NVERR_INCOMPATIBLE,
    NVERR_NOT_OPENED
}NVITEM_ERROR_E;

/**
    NVITEM_Init
**/
#ifndef UEIT_MOCOR
NVITEM_ERROR_E NVITEM_Init(void);
#endif
/**
    NVITEM_Uninit
**/
void NVITEM_Uninit(void);

/**
    NVITEM_MarkReset
    
    Descirption:
        Make a mark that NV should be reset but it will not take effect until the next power on.
        Caution: All changes made to NV after the invocation of this function will be invalidated 
        when the phone powers on again.
    
    Parameters:
        None.
    
    Return values:
        NVERR_NONE -
        Other      -
        
**/
NVITEM_ERROR_E NVITEM_MarkReset(void);

/**
	NVITEM_Read
	
    Description:
        Read an item from the medium.
       
    Paramters:
    	ItemID   - Identifier of the NV item to be read.
    	cchSize  - Size in bytes to be read.
    	pBuf     - Pointer to the buffer to hold the data read.
    
    Return values:
        NVERR_NONE          - The item is read successfully.
        NVERR_NOT_EXIST     - The nvitem does not exist.
        NVERR_INVALID_PARAM - Parameters are invalid, e.g. buf_ptr is NULL, count 
                              is larger than the size of this NV item or the specified
                              Identifier does not exist.
    
**/
NVITEM_ERROR_E NVITEM_Read(INPUT uint16 ItemID, INPUT uint16 cchSize, 
	OUTPUT void *pBuf);

/**
	NVITEM_Write
	    
    Description:
        Write an item to the medium. If the item does not exist, it will be created.
        
    Parameters:
    	ItemID  - Identifier of the item to be written.
    	cchSize - Size in byte of this item.
    	pBuf    - Pointer to the buffer holding the data of the item.
        
    Return values:
        NVERR_NONE               - The item is written successfully.
        NVERR_NO_ENOUGH_RESOURCE - There is no enough resource to complete this 
                                   operation, e.g. no enough space on the medium.
            
        NVERR_INVALID_PARAM      - Parameters are invalid, e.g. buf_ptr is NULL or 
        						   Identifier is invalid.
    
**/
NVITEM_ERROR_E NVITEM_Write(INPUT uint16 ItemID, INPUT uint16 cchSize,  
	INPUT const void *pBuf); 

/** 
	NVITEM_Delete
	
  	Description:
    	Delete the items in the given range. If From is larger than To, NVERR_NONE
    	will be returned, but no item is deleted. If From is equal to To and NV_ERROR
    	is returned, only one item is deleted. If From is less than To, any item in
    	the range, if exists, will be deleted.
    	
    Parameters:
    	From - Identifier of the first item to be deleted.
    	To   - Identifier of the last item to be deleted.
    	
  	Return values:
  		NVERR_NONE - The operation is successful.
  		Other      - 
**/                         
NVITEM_ERROR_E NVITEM_Delete(INPUT uint16 From, INPUT uint16 To);  

/**
	NVITEM_UpdateCali
	
	Description:
		Update calibration parameters both in the running area and in the download area. This
		function should be called by production-relative module ONLY to update RF calibratoin
		parameter, IMEI and so on. Other modules, which try to update their own NV items should
		call NVITEM_Write instead.

**/ 
NVITEM_ERROR_E NVITEM_UpdateCali(INPUT uint16 ItemID, INPUT uint16 cchSize,  
	INPUT const void *pBuf); 

/**
 	NVITEM_GetLength
 	
    Description:
        Get the length of NV item.
        
    Return values:
        Length of the NV item in bytes. If 0 is returned, the item does not exist
        or is empty.
**/
uint16 NVITEM_GetLength(INPUT uint16 ItemID);
	
/**
	NVITEM_IsSimulate
	
	Description:
		Return whether NVITEM is simulated.
	
	Return values:
		TRUE  - Use RAM to simulate.
		FALSE - Figure it out on yourself ;-). 
**/		
BOOLEAN NVITEM_IsSimulate(void);
                        
void 	*NVITEM_GetClassmarkAddr(void);
void 	*NVITEM_GetSystemParamAddr(void);
void 	*NVITEM_GetCalibratedParamAddr(void);
uint16 	NVITEM_GetCalibratedParamSize(void);
void 	*NVITEM_GetDSPCodeAddr(void);


/*
* alloc buffer and read data from nv partition sector
* return the beginning address of alloced buffer
*/
PUBLIC void* NVITEM_MallocCaliBuf(void);

/*
* update the nv buffer content according nvitem id
* always return NVERR_NONE
*/
PUBLIC NVITEM_ERROR_E NVITEM_UpdateCaliBuf(uint16 ItemID, uint16 cchSize, const void *pBuf, void * srcbuf);
PUBLIC NVITEM_ERROR_E NVITEM_ReadCaliBuf(uint16 ItemID, uint16 cchSize, void *pBuf, const void * srcbuf);
/*
* first do update nv action, then read to make sure handle is valid
* SCI_TRUE: write success, else write error
*
*/
BOOLEAN  NVITEM_UpdateNvParam(void *pBuf);

/* Sorry, I don't know what this function is for
// 
// set the item as protected, system will store it in the special space.
// Return:
//   If succ, return TRUE, otherwise return FALSE
//
#define  NVITEM_SetProtectedItem( _id )   EFS_SetProtectedItem( _id )
//
BOOLEAN  NVITEM_SetProtectedItem( uint16   item_id ); 
*/

/******************************************************************************
 * Functions for back-compatibility
 * These functions are for back-compatibility only and new written codes
 * should not use them.
 ******************************************************************************/
NVITEM_ERROR_E EFS_NvitemRead(INPUT uint16 ItemID, INPUT uint16 cchSize, 
	OUTPUT uint8 *pBuf);

NVITEM_ERROR_E EFS_NvitemWrite(INPUT uint16 ItemID, INPUT uint16 cchSize, 
	INPUT const uint8 *pBuf, INPUT uint8 bAsync);

NVITEM_ERROR_E EFS_NvitemDelete(INPUT uint16 From, INPUT uint16 To);

PUBLIC uint8  NVITEM_UpdateProductInfo(uint32 start_addr, uint8 *pSrc, uint32 len);
PUBLIC void  NVITEM_GetProductInfo(uint8 *pdes,uint32 len);

PUBLIC uint8  NVITEM_UpdateProductInfo_Protect(uint32 start_addr, uint8 *pSrc, uint32 len);
PUBLIC void  NVITEM_GetProductInfo_Protect(uint8 *pdes,uint32 len);

/******************************************************************************
 * Functions for update the corrupted nvitem in running nv area by
 * the one recorded in fixnv partition.
 ******************************************************************************/
NVITEM_ERROR_E	EFS_RestoreCorruptedNVItem(uint16  identifier, uint16  count, uint8 * buf_ptr);
/******************************************************************************
 * Functions for read phasecheck info from production partition.
 ******************************************************************************/
int32  EFS_ReadPhaseCheck(uint8 *buffer,uint16  buffer_length);
/*cr168988 start*/
uint16 NVITEM_FixedNV_GetLength(INPUT uint16 ItemID);
NVITEM_ERROR_E NVITEM_FixedNV_Read(INPUT uint16 ItemID, INPUT uint16 cchSize, OUTPUT void *pBuf);
NVITEM_ERROR_E NVITEM_FixedNV_Write(INPUT uint16 ItemID, INPUT uint16 cchSize, INPUT const void *pBuf);	
/*cr168988 end*/

BOOLEAN  NVITEM_UpdateNvParam_Protect(uint16 ItemID, uint16 cchSize, const void *pBuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* NVITEM_H */

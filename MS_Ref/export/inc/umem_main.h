/******************************************************************************
 ** File Name:    umem_main.h                                                    *
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
#ifndef UMEM_MAIN_H
    #define UMEM_MAIN_H
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */


/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
typedef enum UMEM_ERROR_enum
{
	UMEM_SUCCESS,
	UMEM_ERROR
}UMEM_ERROR_E;

typedef struct SEC_CTRL_Tag
{
	uint8    sec_ctrl_start; //used to indicate  start of ctrl sec.
	uint8    *sec_state;//this array contain real sec(512 byte sector) state info.
	uint16   *sec_mapper;//this array contain virtual sec(512 byte sector) to real sec info(512 byte sector).
	uint16   *sec_erasenum;//this array contain phy sec erase num.
	uint16   *sec_exception_state;//uesd to indicate unexpected shutdown
	uint32   sec_ctrl_indicator;//indicate  the ctrl sector loacte in which phy sector.
	uint32   sec_ctrl_index;//indicate the ctrl structure position withing sec.
	uint8     sec_ctrl_stop; //used to indicate end of ctrl sec
}SEC_CTRL_T;

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/********************************************************************
*     Description: This function is used to get flag used to indicate whether we should
*     update ctrl sec.
*     umem operation be mutual exclusion
*	Dependence: should be called after write and erase operation.
*	Author: tao.feng
*	Param:
*          none
*	Result:
*	      flag used to indicate whether we should update ctrl sec.
*           TRUE: need update;FALSE:no need update
********************************************************************/
PUBLIC BOOLEAN  UMEM_GetUpdateCtrlSecFlag(void);


/********************************************************************
*     Description: This function is used to set flag used to indicate whether we should
*     update ctrl sec.
*     umem operation be mutual exclusion
*	Dependence: should be called after write and erase operation.
*	Author: tao.feng
*	Param:
*           updata:TRUE:need update;FALSE: no need update!
*	Result:
*	      none
********************************************************************/
PUBLIC void UMEM_SetUpdateCtrlSecFlag(BOOLEAN  update);


/********************************************************************
*	Description: free ctrl sec pointer
*	Dependence:  should be called after Umem_Initialize has been called before
*	Author: tao.feng
*	Param:
*		none
*	Result:
*		return ctrl secctor pointer
********************************************************************/
PUBLIC void Umem_FreeCtrlSec(void);


/********************************************************************
*	Description: get the capacity of norflash capacity which used by udik
*	Dependence:  should be called after Umem_Initialize has been called before
*	Author: tao.feng
*	Param:
*		none
*	Result:
*		None zero:the Capacity, unit is byte
*		0: Fail
********************************************************************/
PUBLIC uint32 Umem_GetCapacity(void);


/********************************************************************
*	Description: This function used to terminate umem garbage collection task
*	Dependence: should be called after Umem_Initialize has been called before.
*	Author: tao.feng
*	Param:
*		   none :
*	Result:
*		 none
********************************************************************/
PUBLIC void Umem_DeleteGarbageCollectThread(void);

/********************************************************************
*	Description: This function is used to erase the block in StorageCARD
*	Dependence:  should be called after Umem_Initialize has been called before
*	Author: tao.feng
*	Param:
*			hDev   : the partition id occupied by norflash udisk
*		secId	: the first virtual sector id (512 byte sector)that will be erased.
*		num    : the num of virtual sector that will be earased
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC UMEM_ERROR_E Umem_EraseSector(uint32 secid,uint32 num);


/********************************************************************
*	Description: This function is used to read serval number continue sectors from
*	  nor flash.each block has 512 bytes.
*	Dependence: should be called after Umem_Initialize has been called before.
*	Author: tao.feng
*	Param:
*		BlockId: that the first virtual sector(512 byte sector) you want to read
*		num:	 the number of	sectors that you want to read from norflash
*		readBuf: the space used to save the values that readout from norflash
*	Result:
*		TRUE:  Success
*		FALSE: Fail
********************************************************************/
PUBLIC UMEM_ERROR_E Umem_ReadSect(uint32 secid,uint32 num, void * readBuf);


/********************************************************************
*	Description: This function is used to write serval number continue blocks to nor flash
*	Dependence:  should be called after Umem_Initialize has been called before
*	Author: tao.feng
*	Param:
*		hDev   : the partition id occupied by norflash udisk
*		BlockId: that the first block Id you want to write
*		num: the number blocks that you want to write to StorageCARD
*		writeBuf: the space used to save the value that will be writen to StorageCARD
*	Result:
*		TRUE: Success
*		FALSE: Fail
********************************************************************/
PUBLIC UMEM_ERROR_E Umem_WriteSect(uint32 secid,uint32 num,const void * writeBuf);


/********************************************************************
*	Description: This function is used to initialize a nor flash udisk internal function
*	Dependence: 
*	Author: tao.feng
*	Param:
*		 none
*	Result:
*		TRUE:  Success
*		FALSE: Fail
********************************************************************/
PUBLIC UMEM_ERROR_E Umem_Initialize(void);


/********************************************************************
*	Description: This function is used to update ctrl phy sec
*	Dependence: should be called after Umem_Initialize has been called before.
*	Author: tao.feng
*	Param:
*          none :
*	Result:
*	     none
********************************************************************/
PUBLIC UMEM_ERROR_E Umem_UpdateCtrlSec(void);

/********************************************************************
*	Description: This function is used to reset ctrl sec info
*	Dependence: should be called after Umem_Initialize has been called before.
*	Author: tao.feng
*	Param:
*          none :
*	Result:
*	     UMEM_ERROR_E
********************************************************************/
PUBLIC void  Umem_CtrlSecInfoReset(void);

/********************************************************************
*     Description: This function is used to get exception state
*	Dependence: none
*	Author: tao.feng
*	Param:
*           none
*	Result:
*	     BOOLEAN
********************************************************************/
PUBLIC BOOLEAN Umem_GetExceptionState(void);


 /********************************************************************
*     Description: This function is used to handle exception state when enter erase or write operation!!!
*     if shutdown power during erase or write operation,when open power we will erase all ctrl sec.
*     so we  should format device again!!!
*	Dependence: none
*	Author: tao.feng
*	Param:
*           none
*	Result:
*	     UMEM_ERROR_E
********************************************************************/
PUBLIC UMEM_ERROR_E Umem_ExceptionHandler(void);

#endif
// End 

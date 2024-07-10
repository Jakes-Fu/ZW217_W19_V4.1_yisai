/******************************************************************************
 ** File Name:      fota.h                                                    *
 ** Author:         jiayong Yang                                              *
 ** DATE:                                                                     *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/

#ifndef _FOTA_H
#define _FOTA_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

 typedef enum _FOTA_ERROR_E {
    FOTA_ERR_NONE  = 0,   			/* Success */    
    FOTA_ERR_SYSTEM			/* System error, e.g. hardware failure */
}FOTA_ERROR_E;

typedef	enum	_FOTA_RESULT_E{
	E_FOTA_NONE = 0,	// not run update process
	E_FOTA_SUCCESS,   // update successully
	E_FOTA_UNCOMPATIBLE // failed for version uncompatible
} FOTA_RESULT_E;

typedef	enum	_FOTA_FLAG_OPERATION_E{
	FOTA_OP_SET_UPDATE_FLAG,
	FOTA_OP_CLEAR_FLAG
}FOTA_FLAG_OPERATION_E;

/*****************************************************************************/
// Description :    Initialize fota  device,must first call before 
//					any other fota operation
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/
FOTA_ERROR_E  FOTA_ImageArea_Open(void);

/*****************************************************************************/
// Description :    Used to write data to fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

uint32  FOTA_ImageArea_Write(uint8 *buffer,uint32 size);

/*****************************************************************************/
// Description :    Used to read data from fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

uint32  FOTA_ImageArea_read(uint8 *buffer,uint32 size);

/*****************************************************************************/
// Description :    Used to erase fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

uint32  FOTA_ImageArea_Erase(void);

/*****************************************************************************/
// Description :    Used to close fota device 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

void  FOTA_ImageArea_Close(void);

/*****************************************************************************/
// Description :    Used to modify flag field of fota device.
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

BOOLEAN  FOTA_SetUpdateFlag(FOTA_FLAG_OPERATION_E op);

/*****************************************************************************/
// Description :    Used to query update result 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

FOTA_RESULT_E  FOTA_GetUpdateResult(void);

/*****************************************************************************/
// Description :    Used to query fota device space size 
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

uint32  FOTA_GetSpaceSize(void);

/*****************************************************************************/
// Description :    Used to Set user flag value
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

BOOLEAN  FOTA_SetUserFlag(uint32 flag);

/*****************************************************************************/
// Description :   Used to get user flag value
// Global resource dependence : 
// Author :        
// Note :
/*****************************************************************************/

BOOLEAN  FOTA_GetUserFlag(uint32 *flag);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /* _FOTA_H */


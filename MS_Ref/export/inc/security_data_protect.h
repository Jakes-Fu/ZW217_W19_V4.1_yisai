/******************************************************************************
 ** File Name:      security_data_protect.h                                                    *
 ** Author:                                                       *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE          	NAME             	DESCRIPTION                               *
 **                                    *                                *
 ******************************************************************************/
#ifndef SECURITY_DATA_PROTECT_H
#define SECURITY_DATA_PROTECT_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "nv_item_id.h"
#include "bsc.h"
#include "diag.h"
#include "cmddef.h"
#include "efuse_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum _ERR_STATUS
{
	ERR_NONE = 0,
	ERR_IMEI_ADDRESS,
	ERR_BT_ADDRESS,
	ERR_RF_PARAMETER,
	ERR_BSC
} ERR_STATUS;

typedef enum _SECURITYDATA_RW_MODE
{
	SECURITYDATA_READ_MODE = 0,
	SECURITYDATA_WRITE_MODE
} SECURITYDATA_RW_MODE;

typedef struct _DIAG_MSG_SECURITYDATA_T
{
    MSG_HEAD_T   msg_head;    
}DIAG_MSG_SECURITYDATA_T;

typedef struct _MSG_SECURITYDATA_RES_T
{
	MSG_HEAD_T   msg_head;    
	uint16        status; 
	uint16	length;
}MSG_SECURITYDATA_RES_T;

typedef enum
{
	SIMLOCK_SELFTEST=0,
	SIMLOCK_WRITE                   = 1

}SIMLOCK_E;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC void Security_Data_Init(void);
PUBLIC BOOLEAN Security_Data_WriteAndSelftest();
PUBLIC BOOLEAN Security_Data_Write();
uint32 HandleSecurityDataReq(uint8       **dest_ptr,
                           uint16       *dest_len_ptr,
                           const uint8  *src_ptr,
                           uint16        src_len);
PUBLIC uint8 Security_Data_Protection_Result();
#ifdef SECURE_BOOT_SUPPORT
PUBLIC EFUSE_RETURN_E WriteBscHashValue();
#endif
PUBLIC BOOLEAN Write_SecurityFlag(void);
PUBLIC BOOLEAN Read_SecurityFlag(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#endif /* SECURITY_DATA_PROTECT_H */

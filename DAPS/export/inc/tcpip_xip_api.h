/*************************************************************************
 ** File Name:      tcpip_xip_api.h                                      *
 ** Author:         yifeng.wang                                          *
 ** Date:           2009/09/16                                           *
 ** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    ip security external interface for POSTCOM(XinYou)   *
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                    *
 ** 2009/09/16     yifeng.wang            Create                         *
 *************************************************************************/
 
#ifndef TCPIP_XIP_API_H
#define TCPIP_XIP_API_H


/**----------------------------------------------------------------------*
 **                        TYPE DEFINITION                               *
 **----------------------------------------------------------------------*/
/* IP encrypt/decrypt function pointer */
typedef int32 (*IP_ENCRYPT_FUNC_PTR)(
    uint8*  data_ptr,   // data pointer
    uint32  data_len,   // data length (BYTES)
    uint32  data_id,    // data ID
    BOOLEAN is_encrypt  // encrypt flag ( TRUE - encrypt; FALSE - decrypt )
    );

/* IP data up function pointer, use after encrypt */
typedef int32 (*IP_DATAUP_FUNC_PTR)(
    uint8*  data_ptr,   // data pointer
    uint32  data_len,   // data length (BYTES)
    uint32  data_id     // data ID
    );

/* IP data down function pointer, use after decrypt */
typedef int32 (*IP_DATADOWN_FUNC_PTR)(
    uint8*  data_ptr,   // data pointer
    uint32  data_len,   // data length (BYTES)
    uint32  data_id     // data ID
    );

 
/**----------------------------------------------------------------------*
 **                        FUNCTION DECLARE                              *
 **----------------------------------------------------------------------*/
/*************************************************************************/
// Description: register IP encrypt/decrypt function pointer
// Global resource : 
// Author: yifeng.wang
// Note: this function is called by POSTCOM to register its IP 
//       encrypt/decrypt function to TCPIP
/*************************************************************************/
PUBLIC int32 TCPIP_RegIpEncryptFunc(IP_ENCRYPT_FUNC_PTR func_ptr);

/*************************************************************************/
// Description: register data up function for IP encrypt module calling
// Global resource : 
// Author: yifeng.wang
// Note: this function is called by POSTCOM to get data-up function
//       for data send after IP encrypted
/*************************************************************************/
PUBLIC int32 TCPIP_RegIpUpFunc(IP_DATAUP_FUNC_PTR* func_pptr);

/*************************************************************************/
// Description: register data down function for IP decrypt module calling
// Global resource : 
// Author: yifeng.wang
// Note: this function is called by POSTCOM to get data-down function
//       for data send after IP decrypted
/*************************************************************************/
PUBLIC int32 TCPIP_RegIpDownFunc(IP_DATADOWN_FUNC_PTR* func_pptr);

#endif /* TCPIP_XIP_API_H */

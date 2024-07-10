/*****************************************************************************
** File Name:      cfl_md5.h                                                 *
** Author:         undiso cheng                                              *
** Date:           2009/02/13                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    DAPS MD5 HASH ALGORITHM                                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/02/13     undiso.cheng     Create                                    *
** 2010/03/18     yifeng.wang      Format Amend                              *
******************************************************************************/

#ifndef _CFL_MD5_H_   
#define _CFL_MD5_H_

/*---------------------------------------------------------------------------*
/*                          Include Files                                    *
/*---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"

/*---------------------------------------------------------------------------*
/*                          Compiler Flag                                    *
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

 
/*---------------------------------------------------------------------------*
/*                          MACRO DEFINITION                                 *
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
/*                          TYPE DEFINITION                                  *
/*---------------------------------------------------------------------------*/
/* typedef a 32-bit type */
typedef unsigned long int UINT4;

/* Data structure for MD5 (Message-Digest) computation */
typedef struct {
    UINT4 i[2];                   /* number of _bits_ handled mod 2^64 */
    UINT4 buf[4];                 /* scratch buffer */
    unsigned char in[64];         /* input buffer */
    unsigned char digest[16];     /* actual digest after MD5Final call */
} CFL_MD5_CTX;


/*---------------------------------------------------------------------------*
/*                          EXTERNAL FUNCTION DECLARE                        *
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : md5 hash
// Global resource dependence : none
// Param : src_ptr: the data to be hashed
//         src_length: data length
//         dest_ptr : the hash result
// Author: undiso.cheng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN CFL_MD5Hash(
    unsigned char *src_ptr, uint32 src_length, uint8 *dest_ptr);

/*****************************************************************************/
// Description : md5 init state
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
// Author : undiso.cheng
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Init(CFL_MD5_CTX *mdcontext);

/*****************************************************************************/
// Description : updates the message-digest context to account for the
//               presence of each of the characters inBuf[0..inLen-1]
//               in the message whose digest is being computed.
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
//         inbuf[IN]: the buf to be computed
//         inlen : the length of the buf
// Author : undiso.cheng
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Update(
    CFL_MD5_CTX *mdcontext, unsigned char *inbuf, unsigned int inlen);

/*****************************************************************************/
// Description : The routine MD5Final terminates the message-digest 
//               computation and ends with the desired message digest 
//               in mdContext->digest[0...15].
// Global resource dependence : none
// Param : md_state[IN/OUT]: state record
// Author : undiso.cheng
// Note :
/*****************************************************************************/
PUBLIC void CFL_MD5Final(CFL_MD5_CTX *mdcontext);

/*****************************************************************************/
// Description : To test the algorithm result
// Global resource dependence : none
// Param :
// Dependency : tests 
// Author : undiso.cheng
// Note :
/*****************************************************************************/
PUBLIC uint32 CFL_Md5Test(void);


/*---------------------------------------------------------------------------*
/*                          Compiler Flag                                    *
/*---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif // _CFL_MD5_H_


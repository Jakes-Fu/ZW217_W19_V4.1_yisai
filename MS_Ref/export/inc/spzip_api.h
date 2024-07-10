/******************************************************************************
 ** File Name:      spzip_api.h                                               *
 ** Author:                                                                   *
 ** DATE:           12/08/2011                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 LZMA           .                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/08/2011                      Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifndef __SPZIP_API_H__
#define __SPZIP_API_H__

#include "sci_types.h"

typedef uint32  LZMA_STATUS;

#define LZMA_SUCCESS     0
#define LZMA_ERROR       1
#define LZMA_ERROR_DATA  2
#define LZMA_ERROR_MEM   3
#define LZMA_ERROR_READ  4
#define LZMA_ERROR_WRITE 5

typedef uint32 LZMA_BZP_IDX_T;

enum{
    LZMA_BZP_IMG,
    LZMA_BZP_RES,
    LZMA_BZP_NV,
    LZMA_BZP_DSP,
    LZMA_BZP_CODE,
    LZMA_BZP_EX00,
    LZMA_BZP_EX01,
    LZMA_BZP_EX02,
    LZMA_BZP_EX03,
    LZMA_BZP_EX04,
    LZMA_BZP_EX05,
    LZMA_BZP_EX06,
    LZMA_BZP_EX07,
    LZMA_BZP_EX08,
    LZMA_BZP_EX09,
    LZMA_BZP_EX10,
    LZMA_BZP_EX11,
    LZMA_BZP_EX12,
    LZMA_BZP_EX13,
    LZMA_BZP_EX14,
    LZMA_BZP_EX15,
    LZMA_BZP_EX16,
    LZMA_BZP_EX17,
    LZMA_BZP_EX18,
    LZMA_BZP_EX19,
    LZMA_BZP_EX20,
    LZMA_BZP_EX21,
    LZMA_BZP_EX22,
    LZMA_BZP_EX23,    
    LZMA_BZP_MAX
};

typedef struct LZMA_INPUT_Tag
{
  uint8* zip_buf_addr;
  uint32 zip_buf_len;
  uint8* unzip_buf_addr;
  uint32 unzip_buf_len;
}LZMA_INPUT_T;

typedef struct LZMA_OUTPUT_Tag
{
  uint32 unzip_size;
}LZMA_OUTPUT_T;

/*****************************************************************************/
//  Description:    The function is used to decompress Bin which has been 
//                  compressed with LZMA arithmetic.
//  Global resource dependence: DSP Bin of LZMA  
//  Input para:     in_param
//                      in_param->zip_buf_addr: Bin data pointer
//                      in_param->zip_buf_len:  Bin data length
//                      in_param->unzip_buf_addr: unzipped Bin data pointer
//                      in_param->unzip_buf_len:  unzipped Bin data length    
//  Output para:    out_param
//                      out_param->unzip_size:  actual unzipped bin data size
//  Return val:     LZMA_SUCCESS or LZMA_ERROR
//  Author:         
//  Note:
/*****************************************************************************/
LZMA_STATUS LZMA_UnZip(LZMA_OUTPUT_T* out_param, LZMA_INPUT_T* in_param);

/*****************************************************************************/
//  Description:    The function is used to get bin pointer.
//  Global resource dependence: Bin of LZMA  
//  Input para:     type    
//  Output para:    None
//  Return val:     Bin data pointer which is used to decompressed
//  Author:         
//  Note:
/*****************************************************************************/
uint8* LZMA_GetBzpBinPtr(LZMA_BZP_IDX_T idx);

/*****************************************************************************/
//  Description:    The function is used to get bin source data size.
//  Global resource dependence: Bin of LZMA  
//  Input para:     type
//  Output para:    None
//  Return val:     Bin data size which is used to decompressed
//  Author:         
//  Note:
/*****************************************************************************/
uint32 LZMA_GetBzpBinSize(LZMA_BZP_IDX_T idx);

/*****************************************************************************/
//  Description:    The function is used to get bin unzipped data buffer length.
//  Global resource dependence: Bin of LZMA  
//  Input para:     type
//  Output para:    None
//  Return val:     Bin data unzipped size
//  Author:         
//  Note:
/*****************************************************************************/
uint32 LZMA_GetBzpDecSize(LZMA_BZP_IDX_T idx);

/*****************************************************************************/
//  Description:    The function is used to get Bzp file pointer address.
//  Global resource dependence: BZP tool compresses the bin files.  
//  Input para:     unzip
//  Output para:    None
//  Return val:     Bzp file pointer address
//  Author:         
//  Note:
/*****************************************************************************/
uint32 LZMA_GetBzpAddr(LZMA_BZP_IDX_T idx);

/*****************************************************************************/
//  Description:    The function is used to get Bzp hardware LZMA module
//              decompress status         
//  Global resource dependence: Hardware LZMA module has been initialized.  
//  Input para:     NONE
//  Output para:    None
//  Return val:     TRUE or FALSE
//  Author:         
//  Note:
/*****************************************************************************/
LZMA_STATUS LZMA_GetUnZipStatus(void);

#endif
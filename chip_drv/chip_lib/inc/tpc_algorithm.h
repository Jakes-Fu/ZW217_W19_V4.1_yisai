/*****************************************************************************
 **  File Name:    tpc_algorithm.h                                           *
 **  Author:       changde                                                   * 
 **  Date:         20/10/2011                                                *
 **  Copyright:    2011 Spreadtrum, Incorporated. All Rights Reserved.       *
 **  Description:  
 *****************************************************************************
 *****************************************************************************
 **  Edit History                                                            *
 **--------------------------------------------------------------------------*
 **  DATE               Author              Operation                        *
 **                                                                          *
 *****************************************************************************/
 
#ifndef _REG_ALGORITHM_H
#define _REG_ALGORITHM_H

#include "sci_types.h"
#include "tpc_drvapi.h"

/**--------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **--------------------------------------------------------------------------*/
#define ARRAY_SIZE( x ) sizeof(x)/sizeof(x[0]) 

#define TP_ABS(x) ((x)>=0)?((x)):(-(x))

/**--------------------------------------------------------------------------*
 **                         Public Declare                                   *
 **--------------------------------------------------------------------------*/
FLOAT_TYPE adc2ohm(FLOAT_TYPE adc_data, FLOAT_TYPE adc_ref, FLOAT_TYPE r_ref);
FLOAT_TYPE pixel2len(FLOAT_TYPE pixels, FLOAT_TYPE pixel_max, FLOAT_TYPE len_max);
uint32     len2pixel(FLOAT_TYPE len, FLOAT_TYPE pixel_max, FLOAT_TYPE len_max);

uint32 sqrt_16(unsigned long M);
FLOAT_TYPE TPC_GetRtouch(TPC_SEQ_DATA_PTR tpc_seq_ptr) ;
uint32 tpc_algorithm (TPC_SEQ_DATA_PTR tpc_seq_ptr,TPC_DATA_U *data0, uint32 size) ;
                            
#endif

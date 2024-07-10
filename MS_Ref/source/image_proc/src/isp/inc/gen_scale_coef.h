/******************************************************************************
 ** File Name:      gen_scale_coef.h                                          *
 ** Author:         shan.he                                                   *
 ** DATE:           2011-02-12                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ** Note:                                                      				  *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE              NAME            DESCRIPTION                             * 
 *****************************************************************************/
#ifndef _GEN_SCALE_COEF_H_
#define _GEN_SCALE_COEF_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

/**---------------------------------------------------------------------------*
 **                         Macros                                            *
 **---------------------------------------------------------------------------*/
 #if (defined(PLATFORM_SC8800G) || defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E))
#define SCALER_COEF_TAP_NUM_HOR			48
#define SCALER_COEF_TAP_NUM_VER			68

//temp buffer size used for coefficient calculation
#define SCALER_TEMP_BUF_SIZE			(6 * 1024)

//coefficient size of horizontal
#define SCALER_COEF_HOR_SIZE				(SCALER_COEF_TAP_NUM_HOR * sizeof(uint32))
//coefficient size of vertical
#define SCALER_COEF_VER_SIZE				((SCALER_COEF_TAP_NUM_VER + 1) * sizeof(uint32))
//max coefficient size
#define SCALER_COEF_MAX_SIZE				SCALER_COEF_VER_SIZE

#elif defined(PLATFORM_SC6800H)

#define SCALER_COEF_TAP_NUM_HOR			48
#define SCALER_COEF_TAP_NUM_VER			36

//temp buffer size used for coefficient calculation
#define SCALER_TEMP_BUF_SIZE			(12 * 1024)

//coefficient size of horizontal
#define SCALER_COEF_HOR_SIZE				(SCALER_COEF_TAP_NUM_HOR * sizeof(uint32))
//coefficient size of vertical
#define SCALER_COEF_VER_SIZE				((SCALER_COEF_TAP_NUM_VER + 1) * sizeof(uint32))
//max coefficient size
#define SCALER_COEF_MAX_SIZE				SCALER_COEF_HOR_SIZE
#else
	"error ! undefined platform"
#endif



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Public Functions                                  *
 **---------------------------------------------------------------------------*/
/****************************************************************************/
/* Purpose:	generate scale factor           							    */
/* Author:																	*/
/* Input:                                                                   */
/*          i_w:	source image width                                      */
/*          i_h:	source image height                                  	*/
/*          o_w:    target image width                                      */
/*          o_h:    target image height                						*/
/* Output:	    															*/
/*          coeff_h_ptr: pointer of horizontal coefficient buffer, the size of which must be at  */
/*					   least SCALER_COEF_TAP_NUM_HOR * 4 bytes				*/
/*					  the output coefficient will be located in coeff_h_ptr[0], ......,   */	
/*						coeff_h_ptr[SCALER_COEF_TAP_NUM_HOR-1]				*/
/*			coeff_v_ptr: pointer of vertical coefficient buffer, the size of which must be at      */
/*					   least (SCALER_COEF_TAP_NUM_VER + 1) * 4 bytes	    */
/*					  the output coefficient will be located in coeff_v_ptr[0], ......,   */	
/*					  coeff_h_ptr[SCALER_COEF_TAP_NUM_VER-1] and the tap number */
/*					  will be located in coeff_h_ptr[SCALER_COEF_TAP_NUM_VER] */
/*          temp_buf_ptr: temp buffer used while generate the coefficient   */
/*          temp_buf_size: temp buffer size, 6k is the suggest size         */
/* Return:					                    							*/  
/* Note:                                                                    */
/****************************************************************************/
PUBLIC BOOLEAN GenScaleCoeff(int16	i_w, int16 i_h, int16 o_w,  int16 o_h, 
					       uint32* coeff_h_ptr, uint32* coeff_v_ptr,
                           void *temp_buf_ptr, uint32 temp_buf_size);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif

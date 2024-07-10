/******************************************************************************
 ** File Name:      aud_comm.h                                                *
 ** Author:         Cherry.Liu                                             *
 ** DATE:           2014.01.20                                               *
 ** Copyright:      2014 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    common function which can be shared by various algorithm moduls.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2014.01.20     Cherry.Liu       Create.                                  *
 ******************************************************************************/

#ifndef _AUD_COMM_H
#define _AUD_COMM_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "os_api.h"


//#define SCI_TRUE                    1       // Boolean true value
//#define SCI_FALSE                   0       // Boolean false value
//#define SCI_NULL                    0       // null pointer


typedef unsigned char		BOOLEAN;
#ifndef WIN32
typedef signed long long	int64;
#endif



/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                      public  Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \brief  Description:  This function is to find the maxive value and index of the array
//! 
//! \author Author:  cherry.liu 
/*****************************************************************************/
 void COMM_Maxfind(
    int32* dataPtr,
    int32  dataCount,
    int32* MaxValue,//out
    int32* MaxIndex //out
    );

/*****************************************************************************/
//! \brief  Description:  This function is to caculate 200*log10(x)
//! \range of x [1,32767]
//! \author Author:  cherry.liu 
/*****************************************************************************/
 int16 COMM_F200log10(
    int32 input
    );

/*****************************************************************************/
//! \brief  Description:  This function is to caculate 32768*10^(-x/200)
//! \range of x [0,1023]
//! \author Author:  cherry.liu 
/*****************************************************************************/
 int16 COMM_F32768power10(
    int32 input
    );

/*****************************************************************************/
//! \brief  Description:  This function is to caculate 100*log10(x)
//! \range of x [1,2^31-1]
//! \author Author:  cherry.liu 
/*****************************************************************************/
 int16 COMM_F100log10(
    int32 input
    );

/*****************************************************************************/
//! \brief  Description:  This function is to caculate 512-fft
//! \author Author:  cherry.liu 
/*****************************************************************************/
 void COMM_FFT(
    int16* input,//input ptr
    int32* output_r,//out N point
    int32* output_i,//out N point
    int32* spec//out N/2 point
    );



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _AUD_COMM_H

// End of aud_comm.h




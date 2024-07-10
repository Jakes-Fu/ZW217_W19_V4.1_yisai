/******************************************************************************
 ** File Name:      simat_outport.h                                           *
 ** Author:         Raistlin.kong                                             *
 ** Date:           10/19/2001                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This is the header file define that data that will used to* 
 **                 to implement the proactive that can be done by sim        *
 **                 application itself 										  *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2001.10        Raistlin.kong    Create                                    *
 ******************************************************************************/

#ifndef _SIMAT_OUTPORT_H_
#define _SIMAT_OUTPORT_H_
#include "sig_code.h"
#include "mn_type.h"   //need by simat_data_object.h
#include "simat_data_object.h"
#include "simat_proactive.h"
#include "simat_api.h"
/* +cr110036 , 2.5g(dual sim)/3g integration */
#ifndef PRODUCT_DM
#include "simat_signal.h"
#else
/* +cr97157 */
/* #include "simat_signal.h" */ /* george comment for atc 10.09*/
/* -cr97157 */
#endif
/* -cr110036 */
#include "simat_callback.h"


#ifdef __cplusplus
    extern "C"
    {
#endif

#ifdef __cplusplus
    }
#endif
#endif  //_SIMAT_OUTPORT_H_
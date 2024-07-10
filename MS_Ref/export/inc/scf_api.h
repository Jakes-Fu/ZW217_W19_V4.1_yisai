/*******************************************************************
**  File Name:     SCF_api.h									   *
**  Author:        Tan LI										   *
**  Date:         04/09/2011                                       *
**  Copyright:    2011 Spreadtrum, Incorporated. All Rights Reserved.*
**  Description:   .											   *
********************************************************************
********************************************************************
**  Edit History                                                   *
**-----------------------------------------------------------------*
**  DATE            NAME            DESCRIPTION                    *
**  04/09/2011     Tan LI        Create.						   *
*******************************************************************/
#ifndef SCF_API_H
#define SCF_API_H

#include "sci_types.h"
//#include "SCF_typeDefs.h"

/*malloc memory*/
int32 SCF_construct(void **h_scf_ptr);

int32 SCF_init(void *scf_ptr);
int32 SCF_process(void *scf_ptr, int16 *data_in, int16 *data_out, int32 scale);

/*release memory*/
int32 SCF_deconstruct(void **h_scf_ptr);

#endif
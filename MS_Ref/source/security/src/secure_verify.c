/******************************************************************************
** File Name:     	secure_verify.c                                           *
 ** Author:                                                       			  *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/

#include "secure.h"
#include "sci_api.h"	

static unsigned char sprd_magic[]	= "SPRD-SECUREFLAG";

/*
*	verify the signed image header is ok or not.
*	0 is OK, !0 is no signed image
*/
PUBLIC int Verify_SecureHeader(const file_header_t *pHeader)
{
	return strcmp(pHeader->magic, sprd_magic);
}



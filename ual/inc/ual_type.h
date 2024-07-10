/*****************************************************************************
** Copyright 2022 Unisoc(Shanghai) Technologies Co.,Ltd                      *
** Licensed under the Unisoc General Software License,                       *
** version 1.0 (the License);                                                *
** you may not use this file except in compliance with the License.          *
** You may obtain a copy of the License at                                   *
** https://www.unisoc.com/en_us/license/UNISOC_GENERAL_LICENSE_V1.0-EN_US    *
** Software distributed under the License is distributed on an "AS IS" BASIS,*
** WITHOUT WARRANTIES OF ANY KIND, either express or implied.                *
** See the Unisoc General Software License, version 1.0 for more details.    *
******************************************************************************/

#ifndef _UAL_TYPE_H
#define _UAL_TYPE_H
#include "mn_type.h"

#undef TRUE
#define TRUE 1

#undef FALSE
#define FALSE 0

typedef unsigned char        boolean;
typedef unsigned char        uint8;
typedef unsigned short       uint16;
typedef unsigned long int    uint32;//TODO: typedef unsigned long int uint32
typedef signed char          int8;
typedef signed short         int16;
typedef signed long int      int32;//TODO: typedef signed long int int32

#undef PNULL
#define PNULL 0

#ifndef local
#define local static
#endif

#ifndef wchar
typedef unsigned short   wchar;
#endif

#endif  /*_UAL_TYPE_H*/

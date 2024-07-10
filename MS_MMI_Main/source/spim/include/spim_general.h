/*==============================================================================
File Name: spim_general.h
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description: spimģ����ϵͳ��صĲ��֣�Ψһ����ϵͳ��ϵ�ļ�
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/
#ifndef _SPIM_GENERAL_H_
#define _SPIM_GENERAL_H_

#include "os_api.h"
#include "caf.h"


//ϵͳ�궨��
#define SPIM_ASSERT(_EXP) SCI_ASSERT(_EXP)  /*assert verified*/

#define SPIM_MALLOC(_SIZE) SCI_ALLOC(_SIZE)
#define SPIM_FREE(_PTR) SCI_FREE(_PTR)
#define SPIM_MEMCPY(_DST, _SRC, _CNT) SCI_MEMCPY(_DST, _SRC, _CNT)
#define SPIM_MEMSET(_ADDR, _VAL, _CNT) SCI_MEMSET(_ADDR, _VAL, _CNT)

//���ú궨��
#define SPIM_ARR_SIZE(_a) ARR_SIZE(_a)
#define SPIM_ISLOWER(_c) ('a' <= (_c) && (_c) <= 'z')

//type
typedef int8 SPIM_INT8;
typedef int16 SPIM_INT16;
typedef int32 SPIM_INT32;
typedef uint8 SPIM_UINT8;
typedef uint16 SPIM_UINT16;
typedef uint32 SPIM_UINT32;
typedef BOOLEAN SPIM_BOOLEAN;


#endif //_SPIM_GENERAL_H_




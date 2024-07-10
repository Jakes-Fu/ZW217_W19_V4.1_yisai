/******************************************************************************
 ** File Name:      spiflash_spec.h                                                 *
 ** Author:         Yong.Li                                                *
 ** DATE:           10/20/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of SPI Flash       *
 ******************************************************************************/
#ifndef _SPIFLASH_SPEC_H_
#define _SPIFLASH_SPEC_H_

#include "sci_types.h"
//#include "flash.h"
#include "flash_drvapi.h"

typedef void * EXT_CFG_PTR;

 //SPI Flash Spec and Configuration for platform application
typedef struct  SPI_flash_spec_s
{
	uint16		nMID;			/* Manufacture ID*/
	uint16		nDID1;			/* Device ID1*/
	uint16		nDID2;			/* Device ID2*/
	uint16		nQEPos;			/* QE bit Poisition, 0xFF indicates no QE bit*/
       uint16            nWIPPos;         /* WIP bit Poisition, 0xFF indicates no nWIPPos bit*/
	uint16		EnterQPI;		/* Command to enter QPI mode*/
	uint16		ExitQPI;			/* Command to exit QPI mode,  0x00 indicates not support*/
	uint16		write_4pp;		/* Command to write in 4 I/O mode */
       uint16            cmd_4read;       /* Command to read in 4 I/O mode */
	uint16		CMD_PES;		/* Program/Erase Suspend Command */
	uint16		CMD_PER;		/* Program/Erase Resume Command */
	BOOLEAN		reset_support;	/* sw reset support*/
	BOOLEAN		QPI_support;	/* QPI mode support*/
       NOR_FLASH_CONFIG_PTR        spiflash_cfg;
       EXT_CFG_PTR    pExtcfg;
} SPIFLASH_SPEC_T, *SPIFLASH_SPEC_PTR;


#ifdef defined(FLASH_SIZE_128MBITX32MBIT)  //6121E 16MB+4MB  FLASH_SIZE_128MBITX32MBIT
   #define MAX_HW_FLASH_SIZE              0x01000000
   #define MAX_HW_FLASH_SIZE_B            0x00400000
#elif FLASH_SIZE_16MBIT
   #define MAX_HW_FLASH_SIZE              0x00200000
#elif defined(FLASH_SIZE_32MBIT)
   #define MAX_HW_FLASH_SIZE              0x00400000
#elif defined(FLASH_SIZE_48MBIT)
   #define MAX_HW_FLASH_SIZE              0x00600000
#elif defined(FLASH_SIZE_64MBIT)
   #define MAX_HW_FLASH_SIZE              0x00800000
#elif defined(FLASH_SIZE_80MBIT)
   #define MAX_HW_FLASH_SIZE              0x00A00000
#elif defined(FLASH_SIZE_128MBIT)
   #define MAX_HW_FLASH_SIZE              0x01000000
#elif defined(FLASH_SIZE_144MBIT)
   #define MAX_HW_FLASH_SIZE              0x01200000
#elif defined(FLASH_SIZE_160MBIT)
   #define MAX_HW_FLASH_SIZE              0x01400000
#elif defined(FLASH_SIZE_192MBIT)
   #define MAX_HW_FLASH_SIZE              0x01800000
#elif (defined(FLASH_SIZE_128MBITX64MBIT_OLD) ||  defined(FLASH_SIZE_128MBITX64MBIT_NEW))
   #define MAX_HW_FLASH_SIZE              0x01000000
   #define MAX_HW_FLASH_SIZE_B            0x00800000
#elif defined(FLASH_SIZE_256MBIT)
   #define MAX_HW_FLASH_SIZE              0x02000000
#else
   #define MAX_HW_FLASH_SIZE              0x01000000
#endif

#if defined(TRACE_INFO_SUPPORT) || defined(MEMORY_DEBUG_SUPPORT)
	#define DEBUG_FLASH_SIZE           0x00000     //Include trace or debug information, the code size will be more large
#else
	#define DEBUG_FLASH_SIZE           0x0
#endif

#endif    //_SPIFLASH_SPEC_H_


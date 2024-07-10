/*===================================================================*/
/*                                                                   */
/*  K6502_RW.h : 6502 Reading/Writing Operation for NES              */
/*               This file is included in K6502.cpp                  */
/*                                                                   */
/*  2000/5/23   InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

#ifndef K6502_RW_H_INCLUDED
#define K6502_RW_H_INCLUDED

/*-------------------------------------------------------------------*/
/*  Include files                                                    */
/*-------------------------------------------------------------------*/

#include "InfoNES.h"
#include "InfoNES_System.h"
#include "InfoNES_pAPU.h"

/*===================================================================*/
/*                                                                   */
/*            K6502_ReadZp() : Reading from the zero page            */
/*                                                                   */
/*===================================================================*/
static __inline BYTE K6502_ReadZp(Nes_Global_T *pGData, BYTE byAddr )
{
/*
 *  Reading from the zero page
 *
 *  Parameters
 *    BYTE byAddr              (Read)
 *      An address inside the zero page
 *
 *  Return values
 *    Read Data
 */

  return pGData->RAM[ byAddr ];
}

/*===================================================================*/
/*                                                                   */
/*               K6502_Read() : Reading operation                    */
/*                                                                   */
/*===================================================================*/

extern  BYTE**	K6502_Read_mem_map[];


extern BYTE	K6502_Read_raw(Nes_Global_T *pGData, WORD wAddr );
extern void	K6502_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );

static __inline BYTE K6502_Read(Nes_Global_T *pGData, WORD wAddr )
{

	BYTE** p = K6502_Read_mem_map[ wAddr >> 13 ];
	
	if( NULL != p )
	{
		return (*p)[ wAddr & 0x1fff ];
	}

	return K6502_Read_raw(pGData, wAddr );
}

// Reading/Writing operation (WORD version)
static __inline WORD K6502_ReadW(Nes_Global_T *pGData, WORD wAddr )
{ 
	return (WORD)((WORD)K6502_Read(pGData, wAddr ) | (WORD)((WORD)K6502_Read(pGData, wAddr + 1 ) << 8)); 
}
static __inline void K6502_WriteW(Nes_Global_T *pGData, WORD wAddr, WORD wData )
{ 
	K6502_Write(pGData, wAddr, wData & 0xff ); K6502_Write(pGData, wAddr + 1, wData >> 8 );
}
static __inline WORD K6502_ReadZpW(Nes_Global_T *pGData, BYTE byAddr )
{ 
	return K6502_ReadZp(pGData, byAddr ) | ( K6502_ReadZp(pGData, byAddr + 1 ) << 8 ); 
}

// 6502's indirect absolute jmp(opcode: 6C) has a bug (added at 01/08/15 )
static __inline WORD K6502_ReadW2(Nes_Global_T *pGData, WORD wAddr )
{ 
  if ( 0x00ff == ( wAddr & 0x00ff ) )
  {
    return K6502_Read(pGData, wAddr ) | (WORD)K6502_Read(pGData, wAddr - 0x00ff ) << 8;
  } else {
    return K6502_Read(pGData, wAddr ) | (WORD)K6502_Read(pGData, wAddr + 1 ) << 8;
  }
}

#endif /* !K6502_RW_H_INCLUDED */

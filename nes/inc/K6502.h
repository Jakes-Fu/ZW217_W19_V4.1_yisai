/*===================================================================*/
/*                                                                   */
/*  K6502.h : Header file for K6502                                  */
/*                                                                   */
/*  2000/05/29  InfoNES Project ( based on pNesX )                   */
/*                                                                   */
/*===================================================================*/

#ifndef K6502_H_INCLUDED
#define K6502_H_INCLUDED

// Type definition
#ifndef DWORD
typedef unsigned long  DWORD;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef BYTE
typedef unsigned char  BYTE;
#endif

#ifndef NULL
#define NULL 0
#endif

/* 6502 Flags */
#define FLAG_C 0x01
#define FLAG_Z 0x02
#define FLAG_I 0x04
#define FLAG_D 0x08
#define FLAG_B 0x10
#define FLAG_R 0x20
#define FLAG_V 0x40
#define FLAG_N 0x80

/* Stack Address */
#define BASE_STACK   0x100

/* Interrupt Vectors */
#define VECTOR_NMI   0xfffa
#define VECTOR_RESET 0xfffc
#define VECTOR_IRQ   0xfffe

// NMI Request
#define NMI_REQ  NMI_State = 0;

// IRQ Request
#define IRQ_REQ  IRQ_State = 0;

#include "InfoNES.h"
// Emulator Operation
int K6502_Init(void);
void k6502_finalize(void);

void    K6502_init_mem_map( Nes_Global_T *pGData );

void K6502_Reset(Nes_Global_T *pGData);
void K6502_Set_Int_Wiring( BYTE byNMI_Wiring, BYTE byIRQ_Wiring );
void K6502_Step(Nes_Global_T *pGData, register WORD wClocks );

// I/O Operation (User definition)
static __inline BYTE K6502_Read(Nes_Global_T *pGData, WORD wAddr );
static __inline WORD K6502_ReadW(Nes_Global_T *pGData, WORD wAddr );
static __inline WORD K6502_ReadW2(Nes_Global_T *pGData, WORD wAddr );
static __inline BYTE K6502_ReadZp(Nes_Global_T *pGData, BYTE byAddr );
static __inline WORD K6502_ReadZpW(Nes_Global_T *pGData, BYTE byAddr );
static __inline BYTE K6502_ReadAbsX(Nes_Global_T *pGData);
static __inline BYTE K6502_ReadAbsY(Nes_Global_T *pGData);
static __inline BYTE K6502_ReadIY(Nes_Global_T *pGData);

extern void K6502_Write(Nes_Global_T *pGData, WORD wAddr, BYTE byData );
static __inline void K6502_WriteW(Nes_Global_T *pGData, WORD wAddr, WORD wData );

// The state of the IRQ pin
extern BYTE IRQ_State;

// The state of the NMI pin
extern BYTE NMI_State;

// The number of the clocks that it passed
extern WORD g_wPassedClocks;

#endif /* !K6502_H_INCLUDED */

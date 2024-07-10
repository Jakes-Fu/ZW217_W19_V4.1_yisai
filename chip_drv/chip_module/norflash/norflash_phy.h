/******************************************************************************
 ** File Name:      norflash_phy.h                                               *
 ** Author:         windy.wang                                                  *
 ** DATE:           01/03/2012                                                *
 ** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 Manager AMD flash.                                        *
 **                 This driver support Am29DL322GT                           *
 **                 If change flash, need modify sector number etc.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 03-01-2012     windy.wang       Create.                                   *
 ******************************************************************************/

#ifndef _NORFLASH_PHY_H
#define _NORFLASH_PHY_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//defined in MS_Customize
extern BOOLEAN SPIFLASH_ReadID(int cs, uint8 *pMID, uint8 *pDID1, uint8 *pDID2);
extern SPIFLASH_SPEC_T *SPIFLASH_SPEC_Get(uint16 nMID, uint16 nDID1, uint16 nDID2);
extern void SPIFLASH_Init(int cs);
extern void SPIFLASH_ResetRegister(int cs);
extern uint32 SPIFLASH_LockRegion(uint32 addr, uint32 size);
extern  void SPIFLASH_ResetRegister(int cs);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_NORFLASH_PHY_H


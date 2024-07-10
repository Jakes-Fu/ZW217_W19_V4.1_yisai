/******************************************************************************
 ** File Name:    cpu_endian.h                                                    *
 ** Author:       weihua.wang                                                 *
 ** DATE:         11/05/2011                                                    *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 11/05/2011     weihua.wang     Create.                                     *
 ******************************************************************************/
#ifndef _CPU_ENDIAN_H_
#define _CPU_ENDIAN_H_
#include "sci_types.h"
/*****************************************************************************/
//  Description:    Clear CSW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 cpu_to_be32 (uint32 dword);
/*****************************************************************************/
//  Description:    Clear CSW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 cpu_to_le32 (uint32 dword);
/*****************************************************************************/
//  Description:    Clear CSW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC  uint32 le32_to_cpu (uint32 dword);
/*****************************************************************************/
//  Description:    Clear CSW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC  uint16 cpu_to_le16 (uint16 word);
/*****************************************************************************/
//  Description:    Clear CSW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC  uint16 le16_to_cpu (uint16 word);

#endif
// End 
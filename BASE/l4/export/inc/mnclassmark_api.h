/******************************************************************************
 ** File Name:      mnclassmark_api.h                                         *
 ** Author:         Fancier fan                                               *
 ** Date:           09/18/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *                                                      
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 09/18/20022     Fancier fan      Create.                                  *
 ******************************************************************************/
 
#if !defined (_MNCLASSMARK_H)
#define _MNCLASSMARK_H

#include "nv_classmark.h"
           
/*****************************************************************************/
// 	Description : read classmark point
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern PS_CLASSMARK_T* MNNV_GetClassmarkPtr( // struct of CLASSMARK
              void
              );

/*****************************************************************************/
// 	Description : read ModelType
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
int16 CLASSMARK_GetModelType(	// return Model name
            void
            );
            
/*****************************************************************************/
// 	Description : read ManufacterName
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern char* CLASSMARK_GetManufacterName(  // return 
            void
            );

/*****************************************************************************/
// 	Description : read ModelName
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern char* CLASSMARK_GetModelName(	// return Model name
            void
            );

/*****************************************************************************/
// 	Description : read software version
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
/*
extern uint8 CLASSMARK_GetSWVersion(   // return software version
			void
			);
*/
/*****************************************************************************/
// 	Description : read p_gsm_supported value
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern BOOLEAN CLASSMARK_GetPGsmSupport(   // return software version
			void
			);

/*****************************************************************************/
// 	Description : read e_gsm_supported value
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern BOOLEAN CLASSMARK_GetEGsmSupport(   // return software version
			void
			);

/*****************************************************************************/
// 	Description : read dcs_supported value
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern BOOLEAN CLASSMARK_GetDcsSupport(   // return software version
			void
			);

/*****************************************************************************/
// 	Description : read pcs_supported value
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern BOOLEAN CLASSMARK_GetPcsSupport(   // return software version
			void
			);

/*****************************************************************************/
// 	Description : read pcs_supported value
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern BOOLEAN CLASSMARK_GetGsm850Support(   // return software version
			void
			);
			
#ifndef _BASE_COMPACT_CODE_
/*****************************************************************************/
// 	Description : write classmark
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
extern void CLASSMARK_WriteClassMark(
              MN_DUAL_SYS_E dual_sys,
              const PS_CLASSMARK_T* classmark_ptr // struct of CLASSMARK
              );
#endif

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
// 	Description : write classmark Gprs Class
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:gprs_multislot_class (mustbe: 1- 12)
//  If Application Calls CLASSMARK_WriteGprsClassEx, The system should re-boot up
//  OR should
//  Call MNPHONE_PowerOffPsEx(dual_sys);
//  and after receiving APP_MN_PS_POWER_OFF_CNF,
//  Call MNPHONE_PowerOnPsEx(dual_sys);
/*****************************************************************************/
void CLASSMARK_WriteGprsClassEx(uint8 gprs_multislot_class);

/*****************************************************************************/
// 	Description : read gprs multislot class
//	Global resource dependence : none
//  Author:       William Qian
//	Note:
/*****************************************************************************/
extern uint8 CLASSMARK_GetGPRSMultislotClass(   // return gprs multislot class
			void
			);
#endif
/*****************************************************************************/
// 	Description : get band filter value
//	Global resource dependence : none
//  Author:      feng.shi
//	Note:
/*****************************************************************************/
extern BOOLEAN CLASSMARK_GetBandFilterValue(  
                                  void
			                      );

/*****************************************************************************/
// 	Description : get band filter start arfcn
//	Global resource dependence : none
//  Author:      feng.shi
//	Note:
/*****************************************************************************/
extern uint16 CLASSMARK_GetStartArfcn(  
                             void
                             );

/*****************************************************************************/
// 	Description : get band filter end arfcn
//	Global resource dependence : none
//  Author:      feng.shi
//	Note:
/*****************************************************************************/
extern uint16 CLASSMARK_GetEndArfcn(  
                           void
			               );

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
// 	Description : read card mask that decides which card recv paging in tbf
//	Global resource dependence : none
//  Author:       ken.kong
//	Note:
/*****************************************************************************/
extern uint8 CLASSMARK_GetPagingtbfMask(// return mask of card which recv paging in tbf
            void
            );

#endif
#endif /* end of _MNCLASSMARK_H */

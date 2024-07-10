/******************************************************************************
 ** File Name:      l3func.h                                                  *
 ** Author:         Fancier.fan                                               *
 ** Date:           07/23/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file contains general types and macros that   *
 **         		are of use to all modules.The values or definitions are   *
 ** 				dependent on the specified target.                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 07/23/2001     fancier fan      Create.                                   *
 ******************************************************************************/
#ifndef L3_FUNC_H
#define L3_FUNC_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/
// ps define

#include "sci_types.h"
#include "gsm_gprs.h"
#include "dal_time.h"
#include "ps_eng.h"

typedef struct MM_REJ_INFO_T_s
{
    uint8       domain;             /* 0:CS, 1:PS or 2:Cm_serv_rej */
    uint8       cause_sel;          /* 0:Nw rej or 1:Local rej */
    uint8       rej_cause;
    PLMN_T      plmn;               /* include mcc and mnc */
} MM_REJ_INFO_T;

typedef struct MM_REJ_RECORD_T_s
{
    MM_REJ_INFO_T   rej_info;
    SCI_TM_T        current_time;
} MM_REJ_RECORD_T;

typedef struct MM_REJ_INFO_TABLE_T_s
{
    uint8               total_count;    /* 0 - 8 */
    MM_REJ_RECORD_T     rej_record[MM_MAX_REJ_INFO_NUM];
} MM_REJ_INFO_TABLE_T;

/*****************************************************************************/
// 	Description : update plmn list 
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_UpdatePlmnList( // true means update ok,false means no operate
            BOOLEAN         del_flag, // false means add, true means delete
            PLMN_T          plmn, 	  // specified item be added or be deleted
            PLMN_LIST_T     *list_ptr // plmn list
            );

/*****************************************************************************/
// 	Description : update forbidden plmn list 
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_UpdateFplmnList( // true means update ok,false means no operate
            BOOLEAN         del_flag, // false means add, true means delete
            PLMN_T          plmn, 	  // specified item be added or be deleted
            FPLMN_LIST_T    *list_ptr // forbidden plmn list
            );

/*****************************************************************************/
// 	Description : update long forbidden plmn list 
//	Global resource dependence : none
//  Author:       Yu.Sun
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_UpdateLongFplmnList( // true means update ok,false means no operate
            BOOLEAN           del_flag, // false means add, true means delete
            PLMN_T            plmn, 	// specified item be added or be deleted
            FPLMN_LONG_LIST_T *list_ptr // long forbidden plmn list
            );

/*****************************************************************************/
// 	Description : change LAI_T type  to 5 bytes array
//	Global resource dependence : none
//  Author:       fancier.fan
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_EncodeLai(           // true means ok,false means fail
           	LAI_T        lai, 		// in:  LAI
            uint8*       bytes_ptr // out: encode data
            );

/*****************************************************************************/
// 	Description : change 5 bytes array to LAI_T type
//	Global resource dependence : none
//  Author:       Xueli Lv
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_DecodeLai(           // true means ok,false means fail
           	LAI_T*        lai_ptr,  // out:  decoded LAI
            uint8*        bytes_ptr // in:   5 data array
            );

/*****************************************************************************/
// 	Description : This function is used to add or delete the special LAI item from
//                the FLAI lsit
//	Global resource dependence : none
//  Author:       Xueli Lv
//	Note:
/*****************************************************************************/
BOOLEAN  L3FUNC_UpdateFlailist(// TRUE: update successfully; FALSE: failed
					BOOLEAN         del_flag,  // true: delete this item;false:add this item to the list
					LAI_T           lai,       // the special item to be deleted or added
					FLAI_LIST_T     *list_ptr  // pointer the flai list
					);

/*****************************************************************************/
// 	Description : This function is used to find the special PLMN in the FPLMN list
//	Global resource dependence : none
//  Author:       Xueli Lv
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_FindPlmnInFplmslist(
					PLMN_T            plmn,     // the special PLMN to be found
					FPLMN_LIST_T      *list_ptr // pointer to the fplmn list
					);

/*****************************************************************************/
// 	Description : This function is used to find the special PLMN in the long FPLMN list
//	Global resource dependence : none
//  Author:       Yu.Sun
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_FindPlmnInLongFplmnlist(
					const PLMN_T            plmn,     // the special PLMN to be found
					const FPLMN_LONG_LIST_T *list_ptr // pointer to the long fplmn list
					);

/*****************************************************************************/
// 	Description : This function is used to find the special PLMN in the PLMN list
//	Global resource dependence : none
//  Author:       Xueli Lv
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_FindPlmnInPlmnlist(
					const PLMN_T      plmn,     // the special plmn to be found 
					const PLMN_LIST_T *list_ptr // pointer the plmn list
					);

/*****************************************************************************/
// 	Description : This function is used to find the special lai in the FLAI list
//	Global resource dependence : none
//  Author:       Xueli Lv
//	Note:
/*****************************************************************************/
BOOLEAN  L3FUNC_FindLaiInFlailist(
					LAI_T        lai,      // the special lai to be found
					FLAI_LIST_T  *list_ptr // pointer the flai list
					);

/*****************************************************************************/
// 	Description : decode equivalent plmn list
//	Global resource dependence : none
//  Author: Tracy Zhang
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_DecodeEPlmnList(
                               PLMN_LIST_T  *eplmn_list_ptr,
                               uint8        *bytes_ptr,
                               uint8        length
                               );

/*****************************************************************************/
// 	Description : Add current PLMN to the equivalent PLMN list
//	Global resource dependence : none
//  Author: Tracy Zhang
//	Note: The mobile station shall store the list, as provided by the network, 
//        except that any PLMN code that is already in the "forbidden PLMN list" 
//        shall be removed from the "equivalent PLMNs" list before it is stored 
//        by the mobile station. In addition the mobile station shall add to the 
//        stored list the PLMN code of the network that sent the list. 
//        The maximum number of possible entries in the stored list is six.
/*****************************************************************************/
void L3FUNC_UpdateEPlmnList(
                            PLMN_LIST_T              *eplmn_list_ptr, //IN/OUT:
                            const FPLMN_LONG_LIST_T  *fplmn_long_list_ptr, //IN:
                            const PLMN_T             *cur_plmn_ptr    //IN:
                            );

/*****************************************************************************/
// 	Description: check the PLMN is in the EPLMN list
//	Global resource dependence : 
//  Author: Tracy Zhang
//	Note: 
/*****************************************************************************/
BOOLEAN L3FUNC_IsEPLMN(
                       const PLMN_T       plmn,             //IN:
                       const PLMN_LIST_T  *eplmn_list_ptr   //IN:
                       );

/*****************************************************************************/
// 	Description: check the two PLMNs is equivalent or not
//	Global resource dependence : 
//  Author: Tracy Zhang
//	Note: 
/*****************************************************************************/
BOOLEAN L3FUNC_IsTwoPlmnsEquivalent( 
                                    PLMN_T          plmn1,          //IN:
                                    PLMN_T          plmn2,          //IN:
                                    PLMN_LIST_T     *eplmn_list_ptr //IN:
                                    );

/*****************************************************************************/
// 	Description : This function is used to find the special PLMN in the Select PLMN list
//	Global resource dependence : none
//  Author:       Yu.Sun
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_FindPlmnInPlmnSelectlist(
					PLMN_T      plmn,     // the special plmn to be found 
					SELECT_PLMN_LIST_T *list_ptr // pointer the plmn list
					);

/*****************************************************************************/
// 	Description : This function is used to find the special PLMN in the Available PLMN list
//	Global resource dependence : none
//  Author:       Yu.Sun
//	Note:
/*****************************************************************************/
BOOLEAN L3FUNC_FindPlmnInAvailablePlmnlist(
					PLMN_T      plmn,     // the special plmn to be found 
					AVAILABLE_PLMN_LA_LIST_T *list_ptr // pointer the plmn list
					);

/*****************************************************************************/
//  Description: 
//  Global resource dependence :
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
void L3FUNC_SaveMMRejInfo(
                          const MM_REJ_INFO_T   *rej_info_ptr,  //IN:
                          MM_REJ_INFO_TABLE_T   *rej_list_ptr   //IN/OUT:
                          );

/*****************************************************************************/
//  Description: 
//  Global resource dependence :
//  Author: Andy Huang
//  Note:
/*****************************************************************************/
void L3FUNC_RecordOneMMRejInfo(
    uint8       domain,             /* 0:CS, 1:PS or 2:Cm_serv_rej */
    uint8       cause_sel,          /* 0:Nw rej or 1:Local rej */
    uint8       rej_cause,
    PLMN_T      *plmn_ptr,              /* include mcc and mnc */
    MM_REJ_INFO_TABLE_T   *rej_list_ptr   //IN/OUT:
    );

#endif  /* L3_FUNC_H */

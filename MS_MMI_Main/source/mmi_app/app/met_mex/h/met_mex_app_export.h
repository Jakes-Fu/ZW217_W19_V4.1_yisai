#ifndef __MET_MEX_APP_EXPORT_H__
#define __MET_MEX_APP_EXPORT_H__
#ifdef MET_MEX_SUPPORT

#include "mmisms_export.h"

#ifdef __cplusplus
extern   "C"
{
#endif

PUBLIC BOOLEAN MMIMEX_PhsSaveSmsCB(APP_MN_WRITE_SMS_CNF_T *sig_ptr,
											MMISMS_SEND_T  *send_info_ptr
											 );

PUBLIC int32 MMIMex_Aq_SmsWall(MN_DUAL_SYS_E dual_sys,MN_CALLED_NUMBER_T* orig_address_ptr);

int32 MexInterface_aq_PhoneWall(MN_DUAL_SYS_E dual_sys,MN_CALLING_NUMBER_T* call_num_ptr);


int32 MexInterface_aq_CheckIPPhone(const uint8 *tele_in, uint8 *tele_out, int32 tele_out_len);
int32 MexInterface_aq_SmsWall(MN_DUAL_SYS_E dual_sys,MN_CALLED_NUMBER_T* orig_address_ptr);

#ifdef __cplusplus
extern   "C"
}
#endif

#endif   /* MET_MEX_SUPPORT */

#endif   /* __MET_MEX_APP_EXPORT_H__ */


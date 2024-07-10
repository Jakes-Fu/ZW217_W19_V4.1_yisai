#ifndef HALO_FACTORY_H
#define HALO_FACTORY_H

#include "system_hal.h"

#if(PLAY_ROLE == PLAY_AP)
#include "halo_factory_api.h"

#elif(PLAY_ROLE == PLAY_CP)

#endif
PUBLIC uint8 HALO_factory_CheckLicense(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len);         
PUBLIC uint8 HALO_factory_ReadUid(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len); 
PUBLIC uint8 HALO_factory_WriteLicense(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len); 

extern BOOLEAN HALO_SignWrite(NV_EFUSE_SIGN_DATA_T sign);
extern BOOLEAN HALO_SignRead( NV_EFUSE_SIGN_DATA_T* sign);
extern uint8 HALO_ReadUid(uint8 *uid);
#endif

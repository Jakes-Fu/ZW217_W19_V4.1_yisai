#include "system_hal.h"
#if 0
#define HALO_factory_CheckLicense(dest_ptr,dest_len,src_ptr,src_len)
#define HALO_factory_ReadUid(dest_ptr,dest_len,src_ptr,src_len)
#define HALO_factory_WriteLicense(dest_ptr,dest_len,src_ptr,src_len)
#define AcknowledgeCPshakehands()
#else
PUBLIC uint8 HALO_factory_CheckLicense(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len)
{
	return 0;
}        
PUBLIC uint8 HALO_factory_ReadUid(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len)
{
	return 0;
}    
PUBLIC uint8 HALO_factory_WriteLicense(uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len)
{
	return 0;
}

uint8 AcknowledgeCPshakehands(void)
{
	return 0;
}
#endif
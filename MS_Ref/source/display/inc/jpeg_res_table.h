#ifndef __JPEG_RES_TABLE_H__
#define __JPEG_RES_TABLE_H__

#include "sci_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*default vaule for luminance component*/
extern const uint8 jpg_lum_dc_bits_default[];
extern const uint8 jpg_lum_dc_huffvalue_default[];
extern const uint8 jpg_lum_ac_bits_default[];
extern const uint8 jpg_lum_ac_huffvalue_default[];
/*default vaule for chroma component*/
extern const uint8 jpg_chr_dc_bits_default[];
extern const uint8 jpg_chr_dc_huffvalue_default[];
extern const uint8 jpg_chr_ac_bits_default[];
extern const uint8 jpg_chr_ac_huffvalue_default[];
extern const uint8 jpg_fw_lum_quant_tbl_default[5][64];
extern const uint8 jpg_fw_chr_quant_tbl_default[5][64];
/*ZigZag table*/
extern const uint8 jpg_zigzag_order[];

#ifdef __cplusplus
}
#endif

#endif

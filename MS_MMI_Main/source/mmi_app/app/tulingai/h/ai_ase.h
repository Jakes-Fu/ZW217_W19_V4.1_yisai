#ifndef _AI_AES_H_
#define _AI_AES_H_
#ifdef  TULING_AI_SUPPORT

//#include <stdint.h>
#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "lcd_cfg.h"
#include "mmk_app.h"
#include "mmiphone_export.h"
#include "mmi_filemgr.h"

#include "gui_ucs2b_converter.h"
#include "mmisms_export.h"
#include "mmisms_export.h"
#include "os_param.h"


// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// CBC enables AES128 encryption in CBC-mode of operation and handles 0-padding.
// ECB enables the basic ECB 16-byte block algorithm. Both can be enabled simultaneously.

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#ifndef CBC
  #define CBC 1
#endif

#ifndef ECB
  #define ECB 1
#endif



#if defined(ECB) && defined(ECB)

extern void AES128_ECB_encrypt(const uint8* input, const uint8* key, uint8 *output);
extern void AES128_ECB_decrypt(const uint8* input, const uint8* key, uint8 *output);

#endif // #if defined(ECB) && ECB


#if defined(CBC) && defined(CBC)

extern void AES128_CBC_encrypt_buffer(uint8* output, uint8* input, uint32 length, const uint8* key, const uint8* iv);
extern void AES128_CBC_decrypt_buffer(uint8* output, uint8* input, uint32 length, const uint8* key, const uint8* iv);

#endif // #if defined(CBC) && CBC


#endif
#endif //_AI_AES_H_

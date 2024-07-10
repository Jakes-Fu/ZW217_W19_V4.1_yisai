#ifndef _OD_GAME_AES_H_
#define _OD_GAME_AES_H_

#include "od_game_menu.h"

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

typedef signed char int8_t;
typedef unsigned char   uint8_t;
typedef short  int16_t;
typedef unsigned short  uint16_t;
typedef int  int32_t;
typedef unsigned   uint32_t;

#if defined(ECB) && ECB

void AES128_ECB_encrypt(uint8_t* input, const uint8_t* key, uint8_t *output);
#ifdef AES_DECRYPT
void AES128_ECB_decrypt(uint8_t* input, const uint8_t* key, uint8_t *output);
#endif // #ifdef AES_DECRYPT

#endif // #if defined(ECB) && ECB


#if defined(CBC) && CBC

void AES128_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
void AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);

#endif // #if defined(CBC) && CBC

char *od_game_base64_encode(const unsigned char *data, int32_t input_length, int32_t *output_length);
#ifdef OD_DEBUG
unsigned char *od_game_base64_decode(const char *data, int32_t input_length, int32_t *output_length);
void build_decoding_table();
void base64_cleanup();
#endif	// #ifdef OD_DEBUG

#endif //_AES_H_

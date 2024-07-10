/**
 * \file sm3.h
 * thanks to Xyssl
 * author:goldboar
 * email:goldboar@163.com
 * 2011-10-26
 */
#ifndef _SM3_H_
#define _SM3_H_

#include "anti.h"

#define    SM3_DIGEST_LENGTH    32
#define    SM3_CBLOCK        32

/**
 * \brief          SM3 context structure
 */
typedef struct
{
    unsigned long total[2];     /*!< number of bytes processed  */
    unsigned long state[8];     /*!< intermediate digest state  */
    uint8_t buffer[64];   /*!< data block being processed */

    uint8_t ipad[64];     /*!< HMAC: inner padding        */
    uint8_t opad[64];     /*!< HMAC: outer padding        */

}sm3_context;

typedef sm3_context SM3_CTX;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          SM3 context setup
 *
 * \param ctx      context to be initialized
 */
int  SM3_Init( sm3_context *ctx );

/**
 * \brief          SM3 process buffer
 *
 * \param ctx      SM3 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
int SM3_Update( sm3_context *ctx, const uint8_t *input, int ilen );

/**
 * \brief          SM3 final digest
 *
 * \param ctx      SM3 context
 */
int SM3_Final( sm3_context *ctx, uint8_t *output);

/**
 * \brief          Output = SM3( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SM3 checksum result
 */
void SM3(const uint8_t *input, uint32_t ilen,
           uint8_t output[32]);

/**
 * \brief          Output = SM3( file contents )
 *
 * \param path     input file name
 * \param output   SM3 checksum result
 *
 * \return         0 if successful, 1 if fopen failed,
 *                 or 2 if fread failed
 */
int sm3_file( char *path, uint8_t output[32] );

/**
 * \brief          SM3 HMAC context setup
 *
 * \param ctx      HMAC context to be initialized
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 */
void sm3_hmac_starts( sm3_context *ctx, uint8_t *key, int keylen);

/**
 * \brief          SM3 HMAC process buffer
 *
 * \param ctx      HMAC context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void sm3_hmac_update( sm3_context *ctx, uint8_t *input, int ilen );

/**
 * \brief          SM3 HMAC final digest
 *
 * \param ctx      HMAC context
 * \param output   SM3 HMAC checksum result
 */
void sm3_hmac_finish( sm3_context *ctx, uint8_t output[32] );

/**
 * \brief          Output = HMAC-SM3( hmac key, input buffer )
 *
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   HMAC-SM3 result
 */
void sm3_hmac( uint8_t *key, int keylen,
                uint8_t *input, int ilen,
                uint8_t output[32] );


#ifdef __cplusplus
}
#endif

#endif /* sm3.h */

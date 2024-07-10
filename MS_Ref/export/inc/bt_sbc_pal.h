#ifndef SBC_PAL_H
#define SBC_PAL_H

#include "sci_types.h"

#define AV_FIXED_MEDIA_PACKET_HDR_SIZE    12


// sbc_api.h
#define SBC_MAX_BLOCKS                  16
#define SBC_MAX_CHANNELS                2
#define SBC_MAX_SUBBANDS                8

#define MAX_SBC_AUDIO_FRAME_SIZE_BYTES  512
#define MAX_SBC_FRAME_SIZE_BYTES        512

typedef enum
{
    SBC_MONO = 0,
    SBC_DUAL,
    SBC_STEREO,
    SBC_JOINT_STEREO
} SbcChannelMode_t;

typedef enum
{
    SBC_METHOD_LOUDNESS = 0,
    SBC_METHOD_SNR
} SbcAllocMethod_t;


/****************************************************************************
  SBC_CalcBitPool - calculate the bitpool value

  Returns:
    * the main bitpool
    * the alternative bitpool (via the first argument)
    * the toggle period (via the second argument)

  Notes:
    If the bitpool needed to give a more accurate bit rate
    (i.e. closer to that required by the user) is not too close
    to a whole number [1],  then the toggle period is set to
    greater than zero and an alternate bitpool is specified.
    On the application level the main bitpool can then be made to
    switch to the alternative bitpool after every
    'toggle period'/2  frames.  The application may alternatively
    choose not to use a variable bitpool.

    [1] In this version of the codec the algoritm used in working
    out whether the bitpool falls 'close to a whole number' is
    very simple. When the bitpool is calculated, if it falls in
    the middle half     between two whole numbers (i.e.>1/4 & <3/4),
    the toggle period is set to two and the alternative bitpool
    is 'the main bitpool'+1.
****************************************************************************/
/*uint8 SBC_CalcBitPool(uint8 *bitPoolAlt, uint8 *togglePeriod,
                      SbcChannelMode_t channel_mode,
                      uint16 sample_freq,
                      uint8 nrof_blocks,
                      uint8 nrof_subbands,
                      uint16 bitrate);*/


/****************************************************************************
  SBC_Config - configure the settings required before encoding

  Returns:
    frame size in bytes if inputs are valid else returns zero.

  Notes:
    * sample_freq must be either 16000, 32000, 44100 or 48000.
    * nrof_subbands must either 4 or 8.
    * nrof_blocks must be either 4, 8, 12 or 16.
    * bitpool must be greater than 2 and no greater than
       16 * nrof_subbands for MONO and DUAL_CHANNEL channel modes
    or 32 * nrof_subbands for STEREO and JOINT_STEREO channel modes

    If all inputs are correct, all the appropriate settings will be set.
    This function does not need to be called before decoding.
****************************************************************************/
/*uint16 SBC_Config(SbcChannelMode_t channel_mode,
                  SbcAllocMethod_t alloc_method,
                  uint16 sample_freq,
                  uint8 nrof_blocks,
                  uint8 nrof_subbands,
                  uint8 bitpool);*/


/****************************************************************************
  SBC_Encode - encodes a single sbc frame

  Inputs:
    * pointer to the input wav samples casted to a (int16 (*)[2])
    * char* to the frame where sbc frame will be written
    * bitpool to be used for encoding the frame

  Returns:
    frame size in bytes of the sbc frame just encoded
****************************************************************************/
uint16 SBC_Encode(const int16 audio_samples[][2],
                  uint8 *frame,
                  uint8 bit_pool);


/****************************************************************************
  SBC_Init_Decoder - initialise the decoder ready to accept another file.

  Input:
    none

  Returns:
    nothing
****************************************************************************/
void SBC_Init_Decoder(void);


/****************************************************************************
  SBC_Read_Header - verify the sbc frame header before attempting to decode
                        the frame

  Input:
    * char* to the frame to be read

  Returns:
    * frame size if frame is valid else returns zero.
****************************************************************************/
uint16 SBC_Read_Header(uint8 *frame);


/****************************************************************************
  SBC_Decode - decodes a single sbc frame

  Inputs:
    * char* to the frame where sbc frame to be decoded begins
    * pointer to the buffer storing decoded wav samples
****************************************************************************/
void SBC_Decode(
    uint8 *frame,
    int16 audio_samples[SBC_MAX_BLOCKS][SBC_MAX_CHANNELS][SBC_MAX_SUBBANDS]);



typedef struct
{
    SbcChannelMode_t    channel_mode;
    SbcAllocMethod_t    alloc_method;
    uint16            sample_freq;
    uint8             blocks;
    uint8             subbands;
    uint8             minBitpool;
    uint8             maxBitpool;
    uint8             bitPool;
    uint16            framesize;
    uint8             channels;
    uint16            frames_per_payload;
    uint16            bytes_per_payload;  /* @ming.zhang, sbc data */
    /* uint16         bytes_per_encoding; */
    uint16            data_per_encoding;  /* @ming.zhang, 16-bit raw data */
    uint8             remoteMinBitpool; /* ensure proper reconfiguration */
    uint8             remoteMaxBitpool; /* ensure proper reconfiguration */
} SbcConfig_t;


void     SBC_Init(void);
void     SBC_Deinit(void);
BOOLEAN   SBC_prepareSbc(SbcConfig_t *config, uint16 maxFrameSize, uint16 bitrate);
void     SBC_getConfig(SbcConfig_t *config);
void     SBC_startEncode(void);
void     SBC_stopEncode(void);
BOOLEAN   SBC_getPacket(void **packet, uint16 *packetSize, uint16 *frames);
uint16 SBC_getPacketCount(void);
void     SBC_SetTaskPriority(uint32 priority);
void     SBC_Encode_For_DSPCodec(uint16 source_samplerate, uint16 frame_address, uint16 frame_size, void *callback);
void     A2DP_Output_SBC_Enable(BOOLEAN flag);


#endif /* SBC_PAL_H */

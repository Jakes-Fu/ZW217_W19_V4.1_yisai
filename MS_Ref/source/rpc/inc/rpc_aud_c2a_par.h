/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

// AUTO GENERATED

#define TAG_AUD_ToneStopEnd 0x10ead86d
#define TAG_AUD_SetCodecOpStatus 0xc070ad19
struct IPAR_AUD_SetCodecOpStatus
{
    AUD_CP_STATUS_E Status;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_AUD_SetCodecOpStatus));

#define TAG_audevPlayVoiceTone 0x8ec00598
struct IPAR_audevPlayVoiceTone
{
    uint8_t ToneType;
    uint16_t Duration;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_audevPlayVoiceTone));
struct OPAR_audevPlayVoiceTone
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_audevPlayVoiceTone));


// AUTO GENERATED END

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

#ifndef DISABLE_RPC_AUD_ToneStopEnd
void RPC_AUD_ToneStopEnd(void *in, void *out)
{


    AUD_ToneStopEnd();
}
#endif

#ifndef DISABLE_RPC_AUD_SetCodecOpStatus
void RPC_AUD_SetCodecOpStatus(void *in, void *out)
{
    struct IPAR_AUD_SetCodecOpStatus *ipar = (struct IPAR_AUD_SetCodecOpStatus *)in;
    AUD_CP_STATUS_E Status;
    Status = ipar->Status;
    AUD_SetCodecOpStatus(Status);
}
#endif

#ifndef DISABLE_RPC_AUD_VoiceCoderChangeInd
void RPC_AUD_VoiceCoderChangeInd(void *in, void *out)
{


    AUD_VoiceCoderChangeInd();
}
#endif

#ifndef DISABLE_RPC_audevPlayVoiceTone
void RPC_audevPlayVoiceTone(void *in, void *out)
{
    struct IPAR_audevPlayVoiceTone *ipar = (struct IPAR_audevPlayVoiceTone *)in;
    struct OPAR_audevPlayVoiceTone *opar = (struct OPAR_audevPlayVoiceTone *)out;

    opar->result =  audevPlayVoiceTone(ipar->ToneType, ipar->Duration);
}
#endif



// AUTO GENERATED END

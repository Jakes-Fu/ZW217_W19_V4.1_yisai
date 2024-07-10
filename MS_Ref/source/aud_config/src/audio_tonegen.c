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
#include "sci_api.h"
#include "os_api.h"

#include "audio_tonegen.h"

#define COSTAB_COUNT (16)
#define SUBPHASE_BITS (16)
#define SUBPHASE_MASK ((1 << SUBPHASE_BITS) - 1)
#define PHASE_BITS (SUBPHASE_BITS + 6)
#define PHASE_MASK ((1 << PHASE_BITS) - 1)
#define PHASE_1 (1 << PHASE_BITS)
#define PHASE_1_4 (PHASE_1 >> 2)
#define PHASE_1_2 (PHASE_1 >> 1)
#define PHASE_3_4 (PHASE_1_4 + PHASE_1_2)
#define GAIN_BITS (14)
#define GAIN_1 (1 << 14)

#define TONE_DIV_ROUND(p, m, n) ((p > 0)?(((p * m) + ((n) >> 1)) / (n)):p)

//Apply gain(1.14) on sample
#define TONE_APPLY_GAIN(sample, gain) (((int16)sample * (int16)gain) >> GAIN_BITS)

LOCAL const int16 gToneCos[COSTAB_COUNT + 2] = {
    32610, 32767, 32610, 32138, 31357, 30273, 28898, 27245,
    25330, 23170, 20787, 18204, 15446, 12539, 9512, 6392, 3211, 0, //
};


/**
 * Calculate cos value based on phase [0, 0.25]
 */
LOCAL int16 prvCosVal(uint32 phase)
{
    // It is verified there are no overflow
    int loc = (phase >> SUBPHASE_BITS) + 1;
    int pos = phase & SUBPHASE_MASK;

    int a = (gToneCos[loc + 1] + gToneCos[loc - 1] - 2 * gToneCos[loc]);
    int b = (gToneCos[loc + 1] - gToneCos[loc - 1]);
    int pa = ((uint32)pos * pos) >> SUBPHASE_BITS;
    int pb = pos;
    int val = ((a * pa + b * pb) >> (SUBPHASE_BITS + 1)) + gToneCos[loc];
    return val;
}

/**
 * Calculate sin value based on phase [0, 1.0]
 */
LOCAL int16 prvSinVal(uint32 phase)
{
    if (phase <= PHASE_1_2)
        return (phase < PHASE_1_4) ? prvCosVal(PHASE_1_4 - phase) : prvCosVal(phase - PHASE_1_4);
    else
        return (phase <= PHASE_3_4) ? -prvCosVal(PHASE_3_4 - phase) : -prvCosVal(phase - PHASE_3_4);
}

/**
 * Calculate phase step. It is required freq < rate.
 */
LOCAL uint32 prvPhaseStep(uint32 freq, uint32 rate)
{
    uint64 a = (uint64)freq << PHASE_BITS;
    uint32 b = (a + (rate >> 1)) / rate;
    return b & PHASE_MASK;
}

/**
 * Create the tonegen
 */
ARMTONE_GEN_RES_T *auToneGenCreate(uint32 sample_rate)
{
    ARMTONE_GEN_RES_T *d;
    if (sample_rate == 0)
        return PNULL;

    d = (ARMTONE_GEN_RES_T *)SCI_ALLOC(sizeof(ARMTONE_GEN_RES_T));
    if (d == PNULL)
        return PNULL;

    d->sample_rate = sample_rate;
    d->gain = 0;
    SCI_TRACE_LOW("[tone_adp][%s] ARMTONE_GEN_RES_T 0x%x, rate %d",
        __FUNCTION__, d, sample_rate);
    return d;
}

/**
 * Delete the tonegen
 */
void auToneGenDelete(ARMTONE_GEN_RES_T *pd)
{
    if(PNULL != pd) {
        SCI_FREE(pd);
    }
}

/**
 * Start tonegen
 */
uint32 auToneGenStart(
    ARMTONE_GEN_RES_T *d,
    uint32 gain,
    uint32 freq1,
    uint32 freq2,
    uint32 period,
    uint32 silence)
{
    if (d == NULL || (freq1 == 0 && freq2 == 0) ||
        freq1 >= d->sample_rate ||
        freq2 >= d->sample_rate) {
        SCI_TRACE_LOW("auToneGenStart invalid param %d %d %d",
            freq1, freq1, d->sample_rate);
        return SCI_FALSE;
    }

    if (freq1 == 0 || freq2 == 0) {
        freq1 = freq1>0?freq1:freq2;
        freq2 = freq1;
    }

    if(gain > GAIN_1)
        gain = GAIN_1;

    d->sample_count = 0;
    d->gain = gain;
    d->freq1 = freq1;
    d->freq2 = freq2;
    d->tone_sample_count = TONE_DIV_ROUND(period, d->sample_rate, 1000);
    d->silence_sample_count = TONE_DIV_ROUND(silence, d->sample_rate, 1000);
    d->phase1 = 0;
    d->phase2 = 0;
    d->phase_step1 = prvPhaseStep(freq1, d->sample_rate);
    d->phase_step2 = prvPhaseStep(freq2, d->sample_rate);
    return SCI_TRUE;
}

/**
 * Generate one sample, and advance phase
 */
LOCAL int16 prvGenSample(ARMTONE_GEN_RES_T *d)
{
    int16 sample1, sample2;

    sample1 = prvSinVal(d->phase1);
    d->phase1 = (d->phase1 + d->phase_step1) & PHASE_MASK;
    if (d->freq1 == d->freq2)
    {
        d->phase2 = d->phase1;
        return sample1;
    }

    sample2 = prvSinVal(d->phase2);
    d->phase2 = (d->phase2 + d->phase_step2) & PHASE_MASK;
    return (sample1 + sample2) >> 1;
}


/**
 * Generate samples
 */
uint32 auToneGenSamples(ARMTONE_GEN_RES_T *d, int16 *samples, uint32 bytes)
{
    uint32 filled = 0;

    if (d == NULL || samples == NULL)
        return -1;

    if (d->sample_count >= d->tone_sample_count) {
        return 0;
    }

    while (filled + sizeof(int16) <= bytes)
    {
        if (d->sample_count < d->tone_sample_count || -1 == d->tone_sample_count) {
            *samples++ = TONE_APPLY_GAIN(prvGenSample(d), d->gain);
            d->sample_count++;
            filled += sizeof(int16);
        } else {
            break;
        }
    }

    SCI_TRACE_LOW("[tone_adp][%s] ARMTONE_GEN_RES_T 0x%x, bytes out %d, d->sample_count %d",
        __FUNCTION__, d, filled, d->sample_count);

    return filled;
}

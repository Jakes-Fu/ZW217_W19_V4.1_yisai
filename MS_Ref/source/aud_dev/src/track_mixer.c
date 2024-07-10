/******************************************************************************
 ** File Name:      track_mixer.c                                                                *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           01/07/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    Voiceband(ARM Controlled) audioservice adapter layer.*
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/07/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "ms_ref_aud_dev_trc.h"
#include "os_api.h"
#include "adm.h"
#include "track_mixer.h"
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define VB_MAX_DA_VALUE (32767)
#define VB_MIN_DA_VALUE (-32768)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

LOCAL void ARMVB_AddrAdjust(
    uint32 uiTrackNum,
    int16 **const ppsLeftData,
    int16 **const ppsRightData,
    uint32 uiDataOffset
)
{
    switch (g_adm_dec_output_buf[uiTrackNum].channel_choise_e)
    {
    case DEV_DATA_OUTPUT_CHANNEL_CHOICE_NORMAL:
        {
            *ppsLeftData = (int16 *) (& (g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr[uiDataOffset]));
            *ppsRightData = (int16 *) (& (g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr_r[uiDataOffset]));
        }
        break;
    case DEV_DATA_OUTPUT_CHANNEL_CHOICE_LEFT:
        {
            *ppsLeftData = (int16 *) (& (g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr[uiDataOffset]));
            *ppsRightData = *ppsLeftData;
        }
        break;
    case DEV_DATA_OUTPUT_CHANNEL_CHOICE_RIGHT:
        {
            *ppsLeftData = (int16 *) (& (g_adm_dec_output_buf[uiTrackNum].data_buffer_ptr_r[uiDataOffset]));
            *ppsRightData = *ppsLeftData;
        }
        break;
    default:
        {
            SCI_PASSERT(SCI_FALSE, ("channel_choise_e:%d.", g_adm_dec_output_buf[uiTrackNum].channel_choise_e));/*assert verified*/
        }
        break;
    }
}

static void _Track_Mixer_Inner(ADM_TRACK_T *track_ptr,
                  int32 *mix_buf_left,
                  int32 *mix_buf_right,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned samples_per_block,
                  int downmix,
                  int is_mute,
                  int dg_level);

void _Track_Mixer(ADM_TRACK_T *track_ptr,
                  int32 *mix_buf_left,
                  int32 *mix_buf_right,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned samples_per_block,
                  unsigned num_of_blocks,
                  int downmix,
                  int is_mute,
                  int dg_level)
{
    while (num_of_blocks)
    {
        _Track_Mixer_Inner(track_ptr,
                  mix_buf_left,
                  mix_buf_right,
                  dest_buf_left,
                  dest_buf_right,
                  samples_per_block,
                  downmix,
                  is_mute,
                  dg_level);
        dest_buf_left += samples_per_block;
        //dest_buf_right += samples_per_block; //lint !e613
        num_of_blocks--;
    }
}

void _Track_Mixer_Inner(ADM_TRACK_T *track_ptr,
                  int32 *mix_buf_left,
                  int32 *mix_buf_right,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned samples_per_block,
                  int downmix,
                  int is_mute,
                  int dg_level)
{
    int16 *left_data_ptr = SCI_NULL;
    int16 *right_data_ptr = SCI_NULL;
    uint32 i;
    int32 n = ADM_MAX_TRACK_NUM;
    int track_count = 0;
    int32 *vb_da0_multitrack = SCI_NULL;
    int32 *vb_da1_multitrack = SCI_NULL;

    // mix every tracks that is on
    while (track_ptr != SCI_NULL)
    {
        n = track_ptr->uiTrackNum;
        SCI_ASSERT (n < ADM_MAX_TRACK_NUM); /*assert verified*/

        track_ptr = track_ptr->ptNextTrack;

        if (g_adm_track_controller[n].track_on)
        {
            if (g_adm_dec_output_buf[n].data_count < samples_per_block)
            {
                SCI_TRACE_LOW("[TRACK][%d] undder run (%d < %d)",
                    __LINE__,
                    g_adm_dec_output_buf[n].data_count,
                    samples_per_block);
                continue;
            }
            vb_da0_multitrack = mix_buf_left;
            vb_da1_multitrack = mix_buf_right;
            {
                SCI_ASSERT(g_adm_dec_output_buf[n].data_count <= g_adm_dec_output_buf[n].buffer_size);/*assert verified*/
                // get input buffer pointer
                SCI_ASSERT( (g_adm_dec_output_buf[n].data_out_pos + samples_per_block) <= g_adm_dec_output_buf[n].buffer_size); /*assert verified*/
                ARMVB_AddrAdjust(n, &left_data_ptr, &right_data_ptr, g_adm_dec_output_buf[n].data_out_pos);
                if (!is_mute)
                {
                    if (track_count == 0)
                    {
                        if (g_adm_dec_output_buf[n].data_count >= samples_per_block)
                        {
                            for (i = 0; i < (samples_per_block-1); i += 2)
                            {
                                vb_da0_multitrack[i] = * (left_data_ptr + i);
                                vb_da0_multitrack[i + 1] = * (left_data_ptr + i + 1);

                                vb_da1_multitrack[i] = * (right_data_ptr + i);
                                vb_da1_multitrack[i + 1] = * (right_data_ptr + i + 1);
                            }
                        }
                        else
                        {
                            SCI_MEMSET(vb_da0_multitrack, 0, samples_per_block*4);
                            SCI_MEMSET(vb_da1_multitrack, 0, samples_per_block*4);
                        }
                    }
                    else
                    {
                        if (g_adm_dec_output_buf[n].data_count >= samples_per_block)
                        {
                            for (i = 0; i < (samples_per_block-1); i += 2)
                            {
                                vb_da0_multitrack[i] += * (left_data_ptr + i);
                                vb_da0_multitrack[i + 1] += * (left_data_ptr + i + 1);

                                vb_da1_multitrack[i] += * (right_data_ptr + i);
                                vb_da1_multitrack[i + 1] += * (right_data_ptr + i + 1);
                            }
                        }
                    }
                    vb_da0_multitrack += samples_per_block;
                    vb_da1_multitrack += samples_per_block;
                }
                if (g_adm_dec_output_buf[n].data_count >= samples_per_block)
                {
                    SCI_DisableIRQ();
                    g_adm_dec_output_buf[n].data_count -= samples_per_block;
                    SCI_PASSERT( (g_adm_dec_output_buf[n].data_count <= g_adm_dec_output_buf[n].buffer_size), ("%d", g_adm_dec_output_buf[n].data_count)); /*assert verified*/
                    g_adm_dec_output_buf[n].data_out_pos += samples_per_block;
                    SCI_PASSERT( (g_adm_dec_output_buf[n].data_out_pos <=  g_adm_dec_output_buf[n].buffer_size), ("%d", g_adm_dec_output_buf[n].data_out_pos)); /*assert verified*/
                    if (g_adm_dec_output_buf[n].data_out_pos ==  g_adm_dec_output_buf[n].buffer_size)
                    {
                        g_adm_dec_output_buf[n].data_out_pos = 0;
                    }
                    SCI_RestoreIRQ();
                }
            }
            if (!is_mute)
            {
                track_count++;
            }
        }
    }

    if (is_mute || track_count==0 /*no track outputs data*/)
    {
        SCI_TRACE_LOW("[TRACK]track_count==0");
        SCI_MEMSET(dest_buf_left, 0, samples_per_block<<2);//mute force stereo
        //if (!downmix)
            //SCI_MEMSET(dest_buf_right, 0, samples_per_block<<1);
    }
    else
    {
        // clip PCM samples
        if (!downmix)
        {
            for (i = 0; i < samples_per_block; i++)
            {
                int32 sample_l, sample_r;
                sample_l = mix_buf_left[i];
                sample_r = mix_buf_right[i];
                if (sample_l > VB_MAX_DA_VALUE)
                    sample_l = VB_MAX_DA_VALUE;
                else if (sample_l < VB_MIN_DA_VALUE)
                    sample_l = VB_MIN_DA_VALUE;
                if (sample_r > VB_MAX_DA_VALUE)
                    sample_r = VB_MAX_DA_VALUE;
                else if (sample_r < VB_MIN_DA_VALUE)
                    sample_r = VB_MIN_DA_VALUE;
                dest_buf_left[2*i] = MULT16_16_P14((int16)sample_l, dg_level);
                //dest_buf_right[i] = (int16)sample_r;
                dest_buf_left[2*i+1] = MULT16_16_P14((int16)sample_r, dg_level);
            }
        }
        else
        {
            for (i = 0; i < samples_per_block; i++)
            {
                int32 sample;
                sample = mix_buf_left[i]+mix_buf_right[i];
                sample >>= 1;
                if (sample > VB_MAX_DA_VALUE)
                    sample = VB_MAX_DA_VALUE;
                else if (sample < VB_MIN_DA_VALUE)
                    sample = VB_MIN_DA_VALUE;
                dest_buf_left[i] = MULT16_16_P14((int16)sample, dg_level);
            }
        }
    }
}

void _Read_SingleTrack(ADM_TRACK_T *track_ptr,
                  int16 *dest_buf_left,
                  int16 *dest_buf_right,
                  unsigned sample_count,
                  BOOLEAN is_mute)
{
    int track_found = 0;
    uint32 uiCount = sample_count;
    int16 *destLeft = dest_buf_left;
    int16 *destRight = dest_buf_right;

    // mix every tracks that is on
//        SCI_MEMSET(dest_buf_left, 0, sample_count*2);
//        SCI_MEMSET(dest_buf_right, 0, sample_count*2);
    while (track_ptr != SCI_NULL)
    {
        int32 n = track_ptr->uiTrackNum;
        SCI_ASSERT (n < ADM_MAX_TRACK_NUM); /*assert verified*/

        track_ptr = track_ptr->ptNextTrack;

        if (g_adm_track_controller[n].track_on)
        {
            track_found = 1;
            while(sample_count)
            {
                uint32 copy_count;
                SCI_ASSERT(g_adm_dec_output_buf[n].data_count <= g_adm_dec_output_buf[n].buffer_size); /*assert verified*/
                SCI_ASSERT(g_adm_dec_output_buf[n].data_out_pos < g_adm_dec_output_buf[n].buffer_size); /*assert verified*/
                if (g_adm_dec_output_buf[n].data_out_pos + sample_count <= g_adm_dec_output_buf[n].buffer_size)
                {
                    copy_count = sample_count;
                }
                else
                {
                    copy_count = g_adm_dec_output_buf[n].buffer_size - g_adm_dec_output_buf[n].data_out_pos;
                }
                if (copy_count > g_adm_dec_output_buf[n].data_count)
                {
                    copy_count = g_adm_dec_output_buf[n].data_count;
                }

                // get input buffer pointer
                if (copy_count > 0)
                {
                    int16 *left_data_ptr = SCI_NULL;
                    int16 *right_data_ptr = SCI_NULL;
                    ARMVB_AddrAdjust(n, &left_data_ptr, &right_data_ptr, g_adm_dec_output_buf[n].data_out_pos);
                    SCI_MEMCPY(dest_buf_left, left_data_ptr, copy_count*2);
                    SCI_MEMCPY(dest_buf_right, right_data_ptr, copy_count*2);
                }
                else
                {
                    SCI_MEMSET(dest_buf_left, 0, sample_count*2);
                    SCI_MEMSET(dest_buf_right, 0, sample_count*2);
                    break;
                }
                dest_buf_left += copy_count;
                dest_buf_right += copy_count;
                sample_count -= copy_count;
                SCI_DisableIRQ();
                g_adm_dec_output_buf[n].data_count -= copy_count;
                g_adm_dec_output_buf[n].data_out_pos += copy_count;
                if (g_adm_dec_output_buf[n].data_out_pos ==  g_adm_dec_output_buf[n].buffer_size)
                {
                    g_adm_dec_output_buf[n].data_out_pos = 0;
                }
                SCI_RestoreIRQ();
            }
            break;
        }
    }

    //SCI_TRACE_LOW("_Read_SingleTrack %d, %d, %d.", is_mute, sample_count, uiCount);

    if (!track_found || is_mute)
    {
        if(dest_buf_left)
        {
            SCI_MEMSET(destLeft, 0, (uiCount<<1));
        }
        if(dest_buf_right)
        {
            SCI_MEMSET(destRight, 0, (uiCount<<1));
        }

        return;
    }
}

uint32 _GetDataCount_SingleTrack(ADM_TRACK_T *track_ptr)
{
    while (track_ptr != SCI_NULL)
    {
        int32 n = track_ptr->uiTrackNum;
        SCI_ASSERT (n < ADM_MAX_TRACK_NUM); /*assert verified*/

        track_ptr = track_ptr->ptNextTrack;

        if (g_adm_track_controller[n].track_on)
        {
            return g_adm_dec_output_buf[n].data_count;
        }
    }
    return 0;
}


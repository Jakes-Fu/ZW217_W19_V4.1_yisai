/*******************************************************************************

  ;******************************************************************************
  ;*                         Edit History                                       *
  ;* -------------------------------------------------------------------------- *
  ;*    DATE                NAME             DESCRIPTION                        *
  ;*  2/14/2008             qiang huang           Create.                       *
  ;* 
;******************************************************************************/

#include <stdio.h>
#include "fir_eq.h"
#pragma warning(disable: 4514)
#include <math.h>
void audio_fir_eq(
                  int * frequency_set,//频率设置
                  double * origin_eq,//原始频响
                  double * target_eq,//目标频响
                  fireqstate * fir_state,
                  int * fir_coef, //fir_eq系数
                  int * fir_gain
                  )
{
    double eq_design[maxlength];
    double fir_design[8001];
    double temp_fir[65];
    int i,j,frequency_set_length;
    int high_freq;
    double ap1,ap2,temp;
    int mid_tap,temp_coef,max;
    double pi=3.1415926;
    //step1:get eq_design values in frequency domain
    
    frequency_set_length=fir_state->frequency_set_length;
    if (fir_state->scale_flag==1)//dB scale
    {
        for (i=0;i<frequency_set_length;i++)
        {
            eq_design[i]=target_eq[i]-origin_eq[i];
            eq_design[i]=pow(10,eq_design[i]/20);
        }
    }
    else
    {
        for (i=0;i<frequency_set_length;i++)
        {
            eq_design[i]=target_eq[i]/origin_eq[i];
        } 
    }
    
    if (fir_state->bandwidth==8000)
    {
        high_freq=4000;
    }
    else
    {
        high_freq=8000;
    }
    
    
    if (eq_design[frequency_set_length-2]>1)
    {
        eq_design[frequency_set_length-2]=1;
    }
    if (eq_design[frequency_set_length-1]>1)
    {
        eq_design[frequency_set_length-1]=1;
    }
    
    //step2: interpolate to 4001_point frequency response
    
    j=1;
    for (i=frequency_set[0];i<frequency_set[frequency_set_length-1]+1;i++)
    {
        while (i>frequency_set[j])
        {
            j++;
        }
        ap1=(double)(frequency_set[j]-i)/(frequency_set[j]-frequency_set[j-1]);
        ap2=(double)(-frequency_set[j-1]+i)/(frequency_set[j]-frequency_set[j-1]);
        fir_design[i]=ap1*eq_design[j-1]+ap2*eq_design[j];
    }
    if (frequency_set[0]>100)
    {
        for (i=0;i<frequency_set[0];i++)
            fir_design[i]=eq_design[frequency_set[0]];
    }
    else
    {
        for (i=0;i<100;i++)
            fir_design[i]=fir_design[100];
    }
    if (frequency_set[frequency_set_length-1]<high_freq)
    {
        for (i=frequency_set[frequency_set_length-1]+1;i<4001;i++)
            fir_design[i]=eq_design[frequency_set[frequency_set_length-1]];
    }
    
    
    //step3: get fir coef
    for (i=32;i<65;i++)
    {
        temp=0;
        for (j=0;j<high_freq+1;j++)
        {
            temp=temp+fir_design[j]*cos(2*pi*j*(i-32)/2/high_freq);
        }
        temp_fir[i]=temp/high_freq;
        temp_fir[64-i]= temp/high_freq;
    }
    mid_tap=(int) ceil((double)fir_state->fir_taps/2);
    for (i=0;i<fir_state->fir_taps;i++)
    {
        temp_coef=(int) floor(temp_fir[33-mid_tap+i]*4096);
        fir_coef[i]=temp_coef;
    }
    * fir_gain=4096;
    //step3: get fir gain
    if (fir_state->chip_type!=0)
    {
        max=abs(fir_coef[0]);
        for (i=1;i<fir_state->fir_taps;i++)
        {
            if (abs(fir_coef[i])>max)
            {
                max=abs(fir_coef[i]);
            }
        }
        
        while (max<16384)
        {
            max = max * 2;
            * fir_gain= (* fir_gain)/2;
        }
        
        for (i=0;i<fir_state->fir_taps;i++)
        {
            fir_coef[i]=(int)floor(temp_fir[33-mid_tap+i]*4096*4096/(* fir_gain));
        }
    }
    
    
}

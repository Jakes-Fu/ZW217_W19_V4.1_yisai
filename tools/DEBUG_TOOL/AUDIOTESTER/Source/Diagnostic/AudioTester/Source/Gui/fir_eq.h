/*******************************************************************************

;******************************************************************************
;*                         Edit History                                       *
;* -------------------------------------------------------------------------- *
;*    DATE                NAME             DESCRIPTION                        *
;*  2/14/2008             qiang huang           Create.                       *
;* 
;******************************************************************************/
#ifndef fir_eq_h
#define fir_eq_h

#define maxlength 400

typedef struct
{
	 int frequency_set_length;//频率设置的长度
	 int chip_type;//6600D=0,else =1
     int fir_taps;//fir步长
	 int scale_flag;//单位标志符，取log的DB值＝1，不取log的绝对值＝0
	 int bandwidth;//8000,16000
} fireqstate;

void audio_fir_eq(
      int * frequency_set,//频率设置
      double * origin_eq,//原始频响
	  double * target_eq,//目标频响
	  fireqstate * fir_state,
	  int * fir_coef, //fir_eq系数
	  int * fir_gain
	  );





#endif
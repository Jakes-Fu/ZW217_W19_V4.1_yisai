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
	 int frequency_set_length;//Ƶ�����õĳ���
	 int chip_type;//6600D=0,else =1
     int fir_taps;//fir����
	 int scale_flag;//��λ��־����ȡlog��DBֵ��1����ȡlog�ľ���ֵ��0
	 int bandwidth;//8000,16000
} fireqstate;

void audio_fir_eq(
      int * frequency_set,//Ƶ������
      double * origin_eq,//ԭʼƵ��
	  double * target_eq,//Ŀ��Ƶ��
	  fireqstate * fir_state,
	  int * fir_coef, //fir_eqϵ��
	  int * fir_gain
	  );





#endif
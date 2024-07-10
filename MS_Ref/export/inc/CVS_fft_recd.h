#ifndef CVS_FFT_H_recd
#define CVS_FFT_H_recd

#define fftMll_recd(x,w) ( (x>>16)*w + (((x&0xffff)*w)>>16) )

typedef struct FFTINFO_recd
{
	long    N;
	long    M;

	short  *map;
	short  *fftw;
	short  *rft_win;
	short  *ana_win;
	short  *syn_win;
	short  *adf_win;
	//short  *adf_win_h;
}FFTI_INFO_recd;



//int32 bitinv(long k,long Log2N);
void  rfft_win_recd (long *cx,long *dx,short *win,FFTI_INFO_recd *fftInfo);
void irfft_map_recd (long *cx,long *dx,FFTI_INFO_recd *fftInfo);

#endif


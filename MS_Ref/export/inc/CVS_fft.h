#ifndef CVS_FFT_H
#define CVS_FFT_H

//#define fftMll(x,w) (long)((((__int64)x)*w)>>31)
//#define fftMll(x,w) ( (x>>15)*w + (((x&0x7fff)*w)>>15) )
#define fftMll(x,w) ( (((int32)(x&0xffff0000))>>16)*w + (((x&0xffff)*w)>>16) )
#define fftMll2(x,w) ( (((int32)(x&0xffff0000))>>15)*w + (((x&0xffff)*w)>>15) )

typedef struct FFTINFO
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
}FFTI_INFO;



int32 bitinv(long k,long Log2N);

//void init_fft_map(FFTI_INFO **fft_ptr_h,long N,long winLen,long isReal);

void  rfft_win (long *cx,long *dx,short *win,FFTI_INFO *fftInfo);
void irfft_map (long *cx,long *dx,FFTI_INFO *fftInfo);

//void DCONV_SUB(int32 *x1,int32 *x2,int32 *CX,int32 *DX,int32 *W,FFTX_INFO *fftInfo,int32 *y1,int32 *y2,int32 *mean_max);
//void DCONV_MAP(int32 *x1,int32 *x2,int32 *CX,int32 *DX,int32 *W,FFTX_INFO *fftInfo,int32 *y1,int32 *y2,int32 *mean_max);

//void FFTX_DUAL_WIN(int32 *x1,int32 *x2,int32 *CX,int16 *win, FFTI_INFO *fftInfo);
//void iFFTX_DUAL(int32 *CX,int32 *DX, FFTI_INFO *fftInfo);

//#if (defined WIN32)||(defined TUREC)
//void FFTX_DUAL_MAP(long* CX,long *PX1,long *PX2,long N);
//#else
//#define FFTX_DUAL_MAP asm_FFTX_DUAL_MAP
//void asm_FFTX_DUAL_MAP(long* CX,long *PX1,long *PX2,long N);
//#endif

#endif

/*
void  rfft (long *cx,FFTI_INFO *fftInfo);
void irfft (long *cx,FFTI_INFO *fftInfo);

long  fft4 (long *cx,FFTI_INFO *fftInfo);
long ifft4 (long *cx,FFTI_INFO *fftInfo);
*/
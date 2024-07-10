


#ifndef HX_FRU_H__
#define HX_FRU_H__

#include "rn_frumv.h"

/* add dot to interpolated frames */
#define ADD_DOT 0

/* color text blocks */
#define COLOR_TEXT 0

/* color logo blocks */
#define COLOR_LOGO 0

#define mmax(a, b)        ((a) > (b) ? (a) : (b))
#define mmin(a, b)        ((a) < (b) ? (a) : (b))
#define mswap(a, b)       {temp=(a);(a)=(b);(b)=temp;}

#define FRAME_COPY(f1, f2)	f1[0]=f2[0];f1[1]=f2[1];f1[2]=f2[2]

/* padding on luma and chroma */
#define LUMA_PADDING 0
#define CHROMA_PADDING 0

/* possible entry states */
#define LOW_ACTION 0
#define MODERATE_ACTION 1
#define HIGH_ACTION 2
#define ABORT 3
#define TIMED_KEY_FRAME 4
#define SCENECHANGE 5
#define NO_CHANGE 6
#define BIG_CHANGE 7

/* number of key frames text must persist to classify block as "logo" */
#define KEYS_FOR_LOGO 1

/* global tables */
#ifndef GLOBAL_TABLES
#ifdef HELIX_CONFIG_NOSTATICS
extern const int signedsquare[512];
extern const int squared[512];
extern const int absvalue[512];
extern const int div3[768];
#else
extern int signedsquare[512];
extern int squared[512];
extern int absvalue[512];
extern int div3[768];
#endif
#endif

#define SIGNEDSQUARE(i) (signedsquare[255+(i)])
#define SQUARED(i) (squared[255+(i)])
#define ABSVALUE(i) (absvalue[255+(i)])
#define DIV3(i) (div3[(i)])

typedef struct tag_FRU_STATS
{
	HXBOOL		temporalInit;
	LONG32*		MV;
	LONG32*		MVblock;
	ULONG32*	MV_SSD;
	ULONG32		totalSSD;
	HXBOOL*		moveMode;
	ULONG32		numIBlocks;
	ULONG32		state1;
	ULONG32		state2;
	UCHAR*		A[3];
	UCHAR*		B[3];
	UCHAR*		dst[3];
	LONG32		init_pels;
	LONG32		init_lines;
	LONG32		pels;
	LONG32		lines;
	LONG32		lumPitch;
	LONG32		chromPitch;
	LONG32		blockPitch;
	LONG32		numbx;
	LONG32		numby;
	ULONG32		quant;
	UCHAR*		textPersistence;
	UCHAR*		logoPersistence;
	HXBOOL*		edgeFound;
} FRU_STATS;

int TemporalInterpolateInit(
	FRU_STATS *fru_stats,
	ULONG32 pels, ULONG32 lines,
	ULONG32 luma_pitch, ULONG32 chroma_pitch);

int TemporalInterpolateReInit(
	FRU_STATS *fru_stats,
	ULONG32 pels, ULONG32 lines,
	ULONG32 luma_pitch, ULONG32 chroma_pitch);

int TemporalInterpolateFree(FRU_STATS *fru_stats);

void MVCopy(FRU_MV_INFO *fru_mv_info, FRU_STATS *fru_stats);

void MVFilter(
	FRU_MV_INFO *fru_mv_info, 
	UCHAR *A, UCHAR *B, 
	ULONG32 quant, 
	FRU_STATS *fru_stats);

int TemporalSetup(
	ULONG32 mode, UCHAR* A[3], UCHAR* B[3], ULONG32 quant, ULONG32 pict_type,
	ULONG32 deltaTime, FRU_MV_INFO *fru_mv_info, FRU_STATS *fru_stats);

int GetFrame(
	ULONG32 timePlacement,
	UCHAR *dst[3],
	FRU_STATS *fru_stats);

int ResetStats(FRU_STATS *fru_stats);

int TestState(ULONG32 mode, ULONG32 quant, ULONG32 pict_type, ULONG32 deltaTime,
			  FRU_MV_INFO *fru_mv_info, FRU_STATS *fru_stats);

void InterpolateMVs(FRU_STATS *fru_stats);

int FindEdges(FRU_STATS *fru_stats,ULONG32 state);

void MoveBlock(
	ULONG32 timePlacement,
	int bx,int by,int *a_tab,int *b_tab,
	UCHAR *A_frame[3],UCHAR *B_frame[3],UCHAR *dst[3],
	FRU_STATS *fru_stats);

void MoveAndAvgBlock(
	ULONG32 timePlacement,
	int bx,int by,int *alpha,int *beta,
	UCHAR *A[3],UCHAR *B[3],UCHAR *dst[3],
	FRU_STATS *fru_stats);

int FixRightAndBottom(
	ULONG32 timePlacement,
	UCHAR *dst_frame[3],
	int *alpha,int *beta,
	FRU_STATS *fru_stats);

void ColorBlocks(
	UCHAR *dst[3],
	FRU_STATS *fru_stats);

HXBOOL IsSceneChange(FRU_STATS *fru_stats);

int Median(int n,LONG32 *v);
int SSD8x8Full(UCHAR *s1,UCHAR *s2,int pitch,int exitSSD);
int SSD8x8Half(UCHAR *s1,UCHAR *s2,int pitch,int exitSSD);
int SSD16x16Half(UCHAR *s1,UCHAR *s2,int pitch,int exitSSD);
int SSD16x16Low(UCHAR *s1,UCHAR *s2,int pitch,int exitSSD);

int PIA_Status_to_HX_RESULT(int ps);


#endif // HX_FRU_H__


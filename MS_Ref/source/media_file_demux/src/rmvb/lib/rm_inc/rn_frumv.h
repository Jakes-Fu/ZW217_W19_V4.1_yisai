


#ifndef HX_FRUMV_H__
#define HX_FRUMV_H__

#define FRAME_TYPE_I	0
#define FRAME_TYPE_P	1
#define FRAME_TYPE_B	2

#define FRU_MV_TYPE_INTRA 0
#define FRU_MV_TYPE_INTER 1

#include "hxtypes.h"
#include "hxresult.h"  // for HX_RESULT

#ifdef __cplusplus
extern "C" {
#endif


typedef struct tag_FRU_MV
{
	LONG32		iMVx2;
	LONG32		iMVy2;
} FRU_MV;

typedef struct tag_FRU_MV_INFO
{
	ULONG32		ulVersion;
	ULONG32		ulNumberOfElementsX;
	ULONG32		ulNumberOfElementsY;
	UCHAR		*pMBType;
	FRU_MV		*pMV;
} FRU_MV_INFO;

// different struct versions
#define FRU_VERSION_16x16_BLOCKS 0

/////////////////////////////////////////////////////////
//
// Exported FRU functions
// TBD: these should not be needed for Tromso
//
/////////////////////////////////////////////////////////
#if defined(_WIN32)
// On Windows rv90 is compiled with __stdcall by default
// We just changed rv2000 and rn_fru2 to be compiled with __cdecl
// and this is why we now explicitly need to define the calling
// convention for these functions.
extern HX_RESULT __cdecl HX_FRU_Init(
	void **HX_FRU_ref, 
	ULONG32 pels, ULONG32 lines,
	ULONG32 luma_pitch, ULONG32 chroma_pitch);

extern HX_RESULT __cdecl HX_FRU_Setup(
	void *HX_FRU_ref,
	ULONG32 mode,
	UCHAR* A[3],
	UCHAR* B[3],
	ULONG32 quant,
	ULONG32 pict_type,
	ULONG32 dt,
	FRU_MV_INFO* fru_mv_info);

extern HX_RESULT __cdecl HX_FRU_GetFrame(
	void *HX_FRU_ref,ULONG32 timePlacement,UCHAR* dst[3]);

extern HX_RESULT __cdecl HX_FRU_Free (void *HX_FRU_ref);

#else

extern HX_RESULT HX_FRU_Init(
	void **HX_FRU_ref, 
	ULONG32 pels, ULONG32 lines,
	ULONG32 luma_pitch, ULONG32 chroma_pitch);

extern HX_RESULT HX_FRU_Setup(
	void *HX_FRU_ref,
	ULONG32 mode,
	UCHAR* A[3],
	UCHAR* B[3],
	ULONG32 quant,
	ULONG32 pict_type,
	ULONG32 dt,
	FRU_MV_INFO* fru_mv_info);

extern HX_RESULT HX_FRU_GetFrame (
	void *HX_FRU_ref,ULONG32 timePlacement,UCHAR* dst[3]);

extern HX_RESULT HX_FRU_Free (void *HX_FRU_ref);
#endif

#ifdef __cplusplus
}
#endif


#endif

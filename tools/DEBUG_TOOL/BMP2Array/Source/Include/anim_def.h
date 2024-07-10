#ifndef __ANIM_DEF_H__
#define __ANIM_DEF_H__

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



typedef enum _ANIM_CMP_TYPE_E
{
	ANIM_CMP_BMP_MBMP = 0,
	ANIM_CMP_BMP_MJPG,
	ANIM_CMP_BMP_ARGB, // SMOV ARGB565
	ANIM_CMP_PNG_ARGB, // SMOV ARGB565
	ANIM_CMP_BMP_ARGB888, // SMOV ARGB888
	ANIM_CMP_PNG_ARGB888, // SMOV ARGB888
} ANIM_CMP_TYPE_E;

// big endian
typedef struct _SMOV_ARGB_HEADER_T
{
	DWORD   dwMainMark;
	DWORD   dwSubMark;
	WORD    wInterval;
	WORD    wFrameNum;
	WORD    wReserved1;
	WORD    wReserved2;
	_SMOV_ARGB_HEADER_T()
	{
		dwMainMark= 0x564F4D53; //"SMOV"
		dwSubMark = 0x42475241; //"ARGB"
		wInterval = 0x1000;     //16
		wFrameNum = 0;
		wReserved1 = 0;
		wReserved2 = 0;
	}
}SMOV_ARGB_HDR_T,*	SMOV_ARGB_HDR_PTR;


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif // __ANIM_DEF_H__
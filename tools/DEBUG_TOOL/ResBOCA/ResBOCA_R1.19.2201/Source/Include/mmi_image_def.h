#ifndef _MMI_IMAGE_DEF_H__
#define _MMI_IMAGE_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// 图片资源的信息
///////////////////////////////////////////////////////////////////////////////

// 图片的类型
typedef enum _IMG_TYPE_E
{
	IT_BMP_16_565_RAW = 0, // 16位565格式的原始位图
	IT_BMP_16_565_RLE,     // 16位565格式，采用行压缩的位图
	IT_BMP_16_565_OSD,    // 16位565格式，采用OSD的位图
	IT_BMP_16_565_JPG,     // 16位565格式，采用JPEG的位图
} IMG_TYPE_E;

typedef struct _IMG_FLAG_T
{
	uint16 nWidth;
	uint16 nHeight;
	uint16 nType;
	uint16 nOtherInfo;

    uint32 nSize;
} IMG_FLAG_T, * PIMG_FLAG_T;

typedef struct _MMI_IMG_DATA_T
{
	PIMG_FLAG_T pFlag;       // 图片的附件标识，如长、宽、类型等信息
    uint32    * pImgData;
} MMI_IMG_DATA_T, * PMMI_IMG_DATA_T;


#ifdef __cplusplus
}
#endif

#endif // _MMI_IMAGE_DEF_H__
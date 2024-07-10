#ifndef _MMI_ANIM_DEF_H__
#define _MMI_ANIM_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// 动画资源的信息
///////////////////////////////////////////////////////////////////////////////

typedef enum _ANIM_TYPE_E
{
    AT_COMPRESS_STREAM = 0,
	AT_COMPRESS_MJPEG,     // 采用Motion JPEG 压缩
} ANIM_TYPE_E;

typedef struct _ANIM_FLAG_T
{
    uint16 nWidth;
    uint16 nHeight;
	uint16 nType;
    uint16 nFrameNum;

	uint32 nSize;
} ANIM_FLAG_T, * PANIM_FLAG_T;

typedef struct _MMI_ANIM_DATA_T
{
    PANIM_FLAG_T pFlag;
    uint32     * pAnimData;
} MMI_ANIM_DATA_T, * PMMI_ANIM_DATA_T;

//MJPEG文件头的信息
typedef struct _MMIMPEG4_HEAD_INFO_T
{
	uint8  id[8];  		// 标识，是一个"sci_vd"的字符串
	uint16 rate;  		// 每秒的帧数
	uint16 reserved;	// 保留字，默认值为0
	uint16 width;  		// JPG图片的宽
	uint16 height; 		// JPG图片的高
	uint32 frame_num;  	// 总共的帧数，即每秒的帧数 * 总时间（秒）
} MMIMPEG4_HEAD_INFO_T; 

#ifdef __cplusplus
}
#endif

#endif // _MMI_ANIM_DEF_H__
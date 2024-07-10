#ifndef _MMI_ANIM_DEF_H__
#define _MMI_ANIM_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// ������Դ����Ϣ
///////////////////////////////////////////////////////////////////////////////

typedef enum _ANIM_TYPE_E
{
    AT_COMPRESS_STREAM = 0,
	AT_COMPRESS_MJPEG,     // ����Motion JPEG ѹ��
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

//MJPEG�ļ�ͷ����Ϣ
typedef struct _MMIMPEG4_HEAD_INFO_T
{
	uint8  id[8];  		// ��ʶ����һ��"sci_vd"���ַ���
	uint16 rate;  		// ÿ���֡��
	uint16 reserved;	// �����֣�Ĭ��ֵΪ0
	uint16 width;  		// JPGͼƬ�Ŀ�
	uint16 height; 		// JPGͼƬ�ĸ�
	uint32 frame_num;  	// �ܹ���֡������ÿ���֡�� * ��ʱ�䣨�룩
} MMIMPEG4_HEAD_INFO_T; 

#ifdef __cplusplus
}
#endif

#endif // _MMI_ANIM_DEF_H__
#ifndef _MMI_IMAGE_DEF_H__
#define _MMI_IMAGE_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// ͼƬ��Դ����Ϣ
///////////////////////////////////////////////////////////////////////////////

// ͼƬ������
typedef enum _IMG_TYPE_E
{
	IT_BMP_16_565_RAW = 0, // 16λ565��ʽ��ԭʼλͼ
	IT_BMP_16_565_RLE,     // 16λ565��ʽ��������ѹ����λͼ
	IT_BMP_16_565_OSD,    // 16λ565��ʽ������OSD��λͼ
	IT_BMP_16_565_JPG,     // 16λ565��ʽ������JPEG��λͼ
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
	PIMG_FLAG_T pFlag;       // ͼƬ�ĸ�����ʶ���糤�������͵���Ϣ
    uint32    * pImgData;
} MMI_IMG_DATA_T, * PMMI_IMG_DATA_T;


#ifdef __cplusplus
}
#endif

#endif // _MMI_IMAGE_DEF_H__
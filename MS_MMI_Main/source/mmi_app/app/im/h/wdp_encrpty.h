#ifndef WDP_ENCRPTY__h__
#define WDP_ENCRPTY__h__

#include "sci_types.h"

#define MAX_NAME_LIST_COUNT							1024

/*****************************************************************************/
//  Description : ��ȡƤ������
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC unsigned char MMIAPIIM_parse_skinbin_get_count(unsigned short index, unsigned short* startPos, unsigned char *skinNameListBuf);

/*****************************************************************************/
//  Description : ��ȡƤ������
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIIM_parse_skinbin_get_type(unsigned short index, unsigned char *skinNameListBuf);

/*****************************************************************************/
//  Description : ��ȡƤ������
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC unsigned char MMIAPIIM_parse_skinbin_get_string(unsigned short index, unsigned short* nameList, unsigned short *name_id, unsigned char *skinNameListBuf);
#endif

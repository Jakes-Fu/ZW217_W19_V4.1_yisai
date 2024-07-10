#ifndef WDP_ENCRPTY_C
#define WDP_ENCRPTY_C
#include "wdp_encrpty.h"

/*****************************************************************************/
//  Description : 获取皮肤总数
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC unsigned char MMIAPIIM_parse_skinbin_get_count(unsigned short index, unsigned short* startPos, unsigned char *skinNameListBuf)
{
    unsigned char *ptr = (unsigned char*)skinNameListBuf + 6;
    unsigned char idx = 1, count = 0;
    
    if (index > MAX_NAME_LIST_COUNT)
    {
        return 0;
    }
	
    *startPos = 0;  
    while (1) /*lint !e716*/
    {
        if(*(ptr + 1) == index)
        {
            if(!*startPos) 
            {
				*startPos = idx;               
            }
            count++;
        }
        else if(count > 0)
        {
            break;
        }
        ptr += ((*ptr) >> 1);
        idx++;
    }

    return count;
}

/*****************************************************************************/
//  Description : 获取皮肤类型
//  Global resource dependence : none
//  Author:
//  Note:
//  Return value: 1 : 目录 0 : pic
/*****************************************************************************/
PUBLIC uint8 MMIAPIIM_parse_skinbin_get_type(unsigned short index, unsigned char *skinNameListBuf)
{
    unsigned char *ptr = (unsigned char*)skinNameListBuf + 6;
    unsigned char idx = 1;
    
    if (index > MAX_NAME_LIST_COUNT) 
    {
        return 0xff;
    }
    while (1) /*lint !e716*/
    {
        if (idx == index) 
        {
            return *ptr & 0x01;
        }
        ptr += (*ptr >> 1);
        idx++;
    }
    //return 0;     //pclint消除，该语句无法执行
}

/*****************************************************************************/
//  Description : 获取皮肤名称
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC unsigned char MMIAPIIM_parse_skinbin_get_string(unsigned short index, unsigned short* nameList, unsigned short *name_id, unsigned char *skinNameListBuf)
{
    unsigned char *ptr = (unsigned char*)skinNameListBuf + 6;
    unsigned char idx = 1;
	unsigned char len = 0;
    unsigned short tmp;
    unsigned char is_pic = 0; 
    
    if (index > MAX_NAME_LIST_COUNT)
    {
        return 0;
    }
	
    while(*ptr)
    {
        if(idx == index)
        {
			is_pic = !(*ptr & 0x01);
            len = (*ptr >> 1) - 2;
			if (is_pic)
			{
				len -= 10;
			}
            ptr += 2;
            idx = 0;
            while(idx < len)
            {
                tmp = *ptr << 8;
                tmp += *(ptr + 1);
                if (nameList)
                {
                    *(nameList + (idx / 2)) = tmp;
                }
                idx += 2;
                ptr += 2;
            }
			
            if (name_id != NULL && is_pic)
            {
                len = 8;
                ptr += 2;
                idx = 0;
                while(idx < len)
                {
                    tmp = *ptr << 8;
                    tmp += *(ptr + 1);
                    *(name_id + (idx / 2)) = tmp;
                    idx += 2;
                    ptr += 2;
                }				
            }
            break;
        }
        ptr += (*ptr >> 1);
        idx++;
    }

    return len;
}

#endif //WDP_ENCRPTY_C

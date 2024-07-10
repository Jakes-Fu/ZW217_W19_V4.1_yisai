/*****************************************************************************
** File Name:      mmivc_common.c                                                   *
** Author:                                                                   *
** Date:           02/05/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi vcard               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        renyi hu         Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_vcard_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmifmm_export.h"
#include "mmi_appmsg.h"
#include "vf_iface.h"
#ifdef MMI_VCALENDAR_SUPPORT
#include "mmivcalendar.h"
#endif
#include "mmisd_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "vf_strings.h"
#include "vf_malloc.h"
#include "gui_ucs2b_converter.h"
#include "vf_malloc_stdlib.h"
#include "mmipub.h"
#include "vf_internals.h"
#include "guiurl.h"
#include "mmivc_common.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define VC_PROP_VALUE_BUF_SIZE  500 //表示用于存放属性值的buffer的大小
/*---------------------------------------------------------------------------*/
/*                          STATIC DEFINITION                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL DECLARE                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          GLOBAL DEFINITION                                */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT
/*****************************************************************************/
// Description : mem functions for vcard and vcalendar parser
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL uint32_t AllocationSize(void *p);

/*****************************************************************************/
// Description : 获取属性编码方式
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL VF_CHARSET_E vc_getcharset(VF_PROP_T    *p_prop);
//#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// Description : Get Name Prop Value
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL void GetNamePropValue(
                            VF_PROP_T *p_prop,//[IN]
                            VC_NAME_PROPERTY_TYPE_E type_index,//[IN]
                            MMIVF_NAME_T *vf_name //[OUT]
                            );
/*****************************************************************************/
// Description : 重新进行排序用于显示，按照"prefix+family+middle+given+suffix"排序方式
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL void VcardSort(char *PropValueBuffer, //[IN][OUT]
                        uint16 buf_size, //[IN]
                        MMIVF_NAME_T *vf_name //[IN]
                        );
/*****************************************************************************/
//  Description : 将数据从vcard存储格式转换为Wstr类型的通用格式
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void VcardDataToWstr(
						   BOOLEAN       is_utf8,	//[IN]
						   const uint8   *src_ptr,	//[IN]
						   MMI_STRING_T  *des_str	//[OUT]
						   );
//#endif
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : mem functions for vcard and vcalendar parser
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void *MMIVC_Malloc(uint32_t s)
{
    char *p = PNULL; 
    
    if(s > 0)
    {
#ifndef WIN32
        //SCI_TRACE_LOW:"mmivcard:MMIVC_Malloc, size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVC_COMMON_72_112_2_18_3_4_30_0,(uint8*)"d", s);
#endif
        p = (char *)SCI_ALLOC_APP(s+8);
        
        //if( p )
        {
            *(uint32_t *)p = s;
            p += 8;
        }    
    }
    //s_malloc_count ++;
    return (void *)p;
}

/*****************************************************************************/
// Description : mem functions for vcard and vcalendar parser
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void *MMIVC_Realloc(void *p, uint32_t ns)
{
    char      *p2 = PNULL;
    uint32_t   len = 0;

    if(ns == 0)
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIVC_Realloc ns == 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVC_COMMON_97_112_2_18_3_4_30_1,(uint8*)"");
        return p2;
    }
    if (PNULL == p)
    {
        p2 = MMIVC_Malloc(ns);
        return (void *)p2;
    }
    p2 = ((char *)p - 8);
    len = AllocationSize(p);
    SCI_ASSERT(len>0);/*assert verified*/
    SCI_ASSERT(ns>=len);/*assert verified*/
    p2 = (char*)SCI_ALLOC_APP(ns+8);
    SCI_MEMSET( p2, 0, (ns+8));
    *(uint32_t *)p2 = ns;
    p2 += 8;
    SCI_MEMCPY(p2,p,len);
    MMIVC_Free(p);
    //s_realloc_count ++;
    return (void *)p2;
}
/*****************************************************************************/
// Description : mem functions for vcard and vcalendar parser
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVC_Free(void *p)
{
    void *rp = (void *)(((char*)p)-8);

    if( PNULL != p)
    {
        SCI_FREE(rp);
    }
}
/*****************************************************************************/
// Description : mem functions for vcard and vcalendar parser
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL uint32_t AllocationSize(void *p)
{
    return p ? *(uint32_t *)((char *)p - 8) : 0;
}
/*****************************************************************************/
// Description : 获取属性编码方式
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL VF_CHARSET_E vc_getcharset(VF_PROP_T    *p_prop)
{
    VF_CHARSET_E     charset = VF_CHARSET_UTF8;
    const char      *pszCharset = PNULL;

    if(PNULL == p_prop)
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_getcharset p_prop 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVC_COMMON_150_112_2_18_3_4_30_2,(uint8*)"d", p_prop);
        return charset;
    }
    pszCharset = vf_find_charset(p_prop);
    if ( PNULL != pszCharset)
    {
        if ( 0 == p_stricmp(pszCharset, "UTF-8"))
        {
            charset = VF_CHARSET_UTF8;
        }
        else if ( 0 == p_stricmp(pszCharset, "GB2312"))
        {
            charset = VF_CHARSET_GB2312;
        }
        else
        {
            charset = VF_CHARSET_UTF8;
        }
    }

    return charset;
}
//#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// Description : 获取Name属性值
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIVC_GetNamePropVaule(
								   VF_PROP_T   *p_prop,		//[IN]
								   uint32		nValue,			//[IN]
								   MMI_STRING_T    *p_value)	//[OUT]
{
    char         PropValueBuffer[VC_PROP_VALUE_BUF_SIZE + 1] = "\0";
    uint16       buf_len = 0, buf_size = 0;
    uint16       n;
    uint16       propValueBufferLen;
    char        *p_valuestr;
    MMIVF_NAME_T   vf_name = {0};
    VC_NAME_PROPERTY_TYPE_E type_index = 0;
    BOOLEAN is_utf8_charset = FALSE;
    
    if((PNULL == p_prop) || (PNULL == p_value))
    {
        SCI_TRACE_LOW("[MMIVCARD] MMIVC_GetNamePropVaule error param");
        return;
    }

	//获取各个组成部分的值:family, given, middle, prefix, suffix
    SCI_MEMSET(&vf_name, 0x00, sizeof(MMIVF_NAME_T));
    for (n = 0; n < nValue; n++)
    {
        type_index = n;
        GetNamePropValue(p_prop, type_index, &vf_name);     
    }

    if(NULL != p_value->wstr_ptr)
    {
        SCI_FREE(p_value->wstr_ptr);
    }

	//重新组织排序
    buf_size = sizeof(PropValueBuffer);
    VcardSort(PropValueBuffer,buf_size, &vf_name);
	
    propValueBufferLen = (uint16)p_strlen(PropValueBuffer);
    buf_len = MIN(propValueBufferLen, VC_PROP_VALUE_BUF_SIZE);
    p_valuestr = SCI_ALLOC_APP(buf_len + 1);
    if(PNULL == p_valuestr)
    {
        SCI_TRACE_LOW("[MMIVCARD] MMIVC_GetNamePropVaule p_valuestr alloc fail");
        return;
    }
    SCI_MEMSET(p_valuestr, 0, buf_len + 1);
    memcpy(p_valuestr, PropValueBuffer, buf_len);

    //判断是否UTF8编码
    if ( VF_CHARSET_UTF8 == vc_getcharset(p_prop))
    {
        is_utf8_charset = TRUE;
    }

	//数据格式转换
    VcardDataToWstr(is_utf8_charset, p_valuestr, p_value);
    SCI_FREE(p_valuestr);
	
}
/*****************************************************************************/
// Description : Get Name Prop Value
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL void GetNamePropValue(
                            VF_PROP_T *p_prop,//[IN]
                            VC_NAME_PROPERTY_TYPE_E type_index,//[IN]
                            MMIVF_NAME_T *vf_name //[OUT]
                            )
{
    char        *p_valuestr;
    uint16       value_len = 0;
	
    if((PNULL == p_prop) || (PNULL == vf_name) || (VC_NAME_PROPERTY_TYPE_MAX <= type_index))
    {
        SCI_TRACE_LOW("[MMIVCARD] GetNamePropValue error param");
        return;
    }
    
    p_valuestr = vf_get_prop_value_string(p_prop, type_index);
    if(PNULL == p_valuestr)  
    {
        SCI_TRACE_LOW("[MMIVCARD] GetNamePropValue vf_get_prop_value_string return null");
        return;
    }
    
    value_len = p_strlen(p_valuestr);
    switch(type_index)
    {
    case VC_NAME_PROPERTY_TYPE_FAMILY:
        vf_name->family_name.alpha_len = MIN(value_len, MMIPB_NAME_MAX_LEN);
        memcpy(vf_name->family_name.alpha, p_valuestr, vf_name->family_name.alpha_len);
        SCI_TRACE_BUF("family_name: ", vf_name->family_name.alpha, vf_name->family_name.alpha_len);
        break;
    case VC_NAME_PROPERTY_TYPE_GIVEN:
        vf_name->given_name.alpha_len = MIN(value_len,MMIPB_NAME_MAX_LEN);
        memcpy(vf_name->given_name.alpha, p_valuestr, vf_name->given_name.alpha_len);
        SCI_TRACE_BUF("given_name: ", vf_name->given_name.alpha, vf_name->given_name.alpha_len);
        break;
    case VC_NAME_PROPERTY_TYPE_MIDDLE:
        vf_name->middle_name.alpha_len = MIN(value_len,MMIPB_NAME_MAX_LEN);
        memcpy(vf_name->middle_name.alpha, p_valuestr, vf_name->middle_name.alpha_len);
        SCI_TRACE_BUF("middle_name: ", vf_name->middle_name.alpha, vf_name->middle_name.alpha_len);
        break;
    case VC_NAME_PROPERTY_TYPE_PREFIX:
        vf_name->prefix.alpha_len = MIN(value_len,MMIPB_NAME_MAX_LEN);
        memcpy(vf_name->prefix.alpha, p_valuestr, vf_name->prefix.alpha_len);
        SCI_TRACE_BUF("prefix: ", vf_name->prefix.alpha, vf_name->prefix.alpha_len);
        break;
    case VC_NAME_PROPERTY_TYPE_SUFFIX:
        vf_name->suffix.alpha_len = MIN(value_len,MMIPB_NAME_MAX_LEN);
        memcpy(vf_name->suffix.alpha, p_valuestr, vf_name->suffix.alpha_len);
        SCI_TRACE_BUF("suffix: ", vf_name->suffix.alpha, vf_name->suffix.alpha_len);
        break;
    default:
        break;
    }
    
    SCI_TRACE_LOW("[MMIVCARD] GetNamePropValue index:%d, len:%d", type_index, value_len);
}

LOCAL BOOLEAN IsFamilyNameInFront(MMIVF_NAME_T *vf_name)
{
    uint32 i;

    if(0 < vf_name->prefix.alpha_len)
    {
        for(i=0;i<strlen(vf_name->prefix.alpha);i++)
        {
            if(*(vf_name->prefix.alpha+i) & 0x80)
                return TRUE;
        }
    }
    else if(0 < vf_name->family_name.alpha_len)
    {
        for(i=0;i<strlen(vf_name->family_name.alpha);i++)
        {
            if(*(vf_name->family_name.alpha+i) & 0x80)
                return TRUE;
        }
    }
    else if(0 < vf_name->middle_name.alpha_len)
    {
        for(i=0;i<strlen(vf_name->middle_name.alpha);i++)
        {
            if(*(vf_name->middle_name.alpha+i) & 0x80)
                return TRUE;
        }
    }
    else if(0 < vf_name->given_name.alpha_len)
    {
        for(i=0;i<strlen(vf_name->given_name.alpha);i++)
        {
            if(*(vf_name->given_name.alpha+i) & 0x80)
                return TRUE;
        }
    }
    else if(0 < vf_name->suffix.alpha_len)
    {
        for(i=0;i<strlen(vf_name->suffix.alpha);i++)
        {
            if(*(vf_name->suffix.alpha+i) & 0x80)
                return TRUE;
        }
    }
    return FALSE;
}
/*****************************************************************************/
// Description : 重新进行排序用于显示，按照"prefix+family+middle+given+suffix"排序方式
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL void VcardSort(char *PropValueBuffer, //[IN][OUT]
					 uint16 buf_size, //[IN]
					 MMIVF_NAME_T *vf_name //[IN]
					 )
{
    BOOLEAN is_family_name_in_front = FALSE;
    uint16 buf_len = p_strlen(PropValueBuffer);
    
    if(PNULL == PropValueBuffer || PNULL == vf_name)
    {
        SCI_TRACE_LOW("[MMIVCARD] VcardSort error param");
        return;
    }
    is_family_name_in_front = IsFamilyNameInFront(vf_name);
    
    //first filed: prefix
    if ((buf_len + vf_name->prefix.alpha_len) < buf_size)
    {
        if(0 < vf_name->prefix.alpha_len)
        {
            p_strcat(PropValueBuffer, vf_name->prefix.alpha);
            buf_len = p_strlen(PropValueBuffer);
        }
    }
    else
    {
        if((buf_size > (buf_len - 1)) && ((buf_size - buf_len + 1) <= MMIPB_NAME_MAX_LEN))
        {
            vf_name->prefix.alpha[buf_size - (buf_len - 1)] = 0;
            p_strcat(PropValueBuffer, vf_name->prefix.alpha);
        }                
        return;
    }
    
    if(is_family_name_in_front)
    {
    //second filed: family name
    if((buf_len + vf_name->family_name.alpha_len) < buf_size) 
    {
        if(0 < vf_name->family_name.alpha_len)
        {
            p_strcat(PropValueBuffer, vf_name->family_name.alpha);
            buf_len = p_strlen(PropValueBuffer);
        }
    }
    else
    {        
        if((buf_size > (buf_len - 1)) && ((buf_size - buf_len + 1) <= MMIPB_NAME_MAX_LEN))
        {
            vf_name->family_name.alpha[buf_size - (buf_len - 1)] = 0;
            p_strcat(PropValueBuffer, vf_name->family_name.alpha);
        }                
        return;
    }
    }
    else
    {
        //second filed: given name
        if((buf_len + vf_name->given_name.alpha_len) < buf_size) 
        {        
            if(0 < vf_name->given_name.alpha_len)
            {
                p_strcat(PropValueBuffer, vf_name->given_name.alpha);
                buf_len = p_strlen(PropValueBuffer);
            }
        }
        else
        {        
            if((buf_size > (buf_len - 1)) && ((buf_size - buf_len + 1) <= MMIPB_NAME_MAX_LEN))
            {
                vf_name->given_name.alpha[buf_size - (buf_len - 1)] = 0;
			    p_strcat(PropValueBuffer, vf_name->given_name.alpha);
            }                
            return;
        }
    }
    
    //third filed: middle name        
    if((buf_len + vf_name->middle_name.alpha_len) < buf_size) 
    {   
        if(0 < vf_name->middle_name.alpha_len)
        {        
            p_strcat(PropValueBuffer, vf_name->middle_name.alpha);
            buf_len = p_strlen(PropValueBuffer);
        }
    }
    else
    {        
        if((buf_size > (buf_len - 1)) && ((buf_size - buf_len + 1) <= MMIPB_NAME_MAX_LEN))
        {
            vf_name->middle_name.alpha[buf_size - (buf_len - 1)] = 0;
			p_strcat(PropValueBuffer, vf_name->middle_name.alpha);
        }                
        return;
    }
    
    if(is_family_name_in_front)
    {
    //forth filed:given name
    if((buf_len + vf_name->given_name.alpha_len) < buf_size) 
    {        
        if(0 < vf_name->given_name.alpha_len)
        {
            p_strcat(PropValueBuffer, vf_name->given_name.alpha);
            buf_len = p_strlen(PropValueBuffer);
        }
    }
    else
    {        
        if((buf_size > (buf_len - 1)) && ((buf_size - buf_len + 1) <= MMIPB_NAME_MAX_LEN))
        {
            vf_name->given_name.alpha[buf_size - (buf_len - 1)] = 0;
			p_strcat(PropValueBuffer, vf_name->given_name.alpha);
        }                
        return;
    }
    }
    else
    {
        //forth filed: family name
        if((buf_len + vf_name->family_name.alpha_len) < buf_size) 
        {
            if(0 < vf_name->family_name.alpha_len)
            {
                p_strcat(PropValueBuffer, vf_name->family_name.alpha);
                buf_len = p_strlen(PropValueBuffer);
            }
        }
        else
        {        
            if((buf_size > (buf_len - 1)) && ((buf_size - buf_len + 1) <= MMIPB_NAME_MAX_LEN))
            {
                vf_name->family_name.alpha[buf_size - (buf_len - 1)] = 0;
                p_strcat(PropValueBuffer, vf_name->family_name.alpha);
            }                
            return;
        }
    }

	
    //fifth filed:suffix
	if((buf_len + vf_name->suffix.alpha_len) < buf_size) 
    {
        if(0 < vf_name->suffix.alpha_len)
        {
            p_strcat(PropValueBuffer, vf_name->suffix.alpha);
            buf_len = p_strlen(PropValueBuffer);
        }
    }
    else
    {        
        if((buf_size > (buf_len - 1)) && ((buf_size - buf_len + 1) <= MMIPB_NAME_MAX_LEN))
        {
            vf_name->suffix.alpha[buf_size - (buf_len - 1)] = 0;
			p_strcat(PropValueBuffer, vf_name->suffix.alpha);
        }                
        return;
    }
}
/*****************************************************************************/
//  Description : 将数据从vcard存储格式转换为Wstr类型的通用格式
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void VcardDataToWstr(
						   BOOLEAN       is_utf8,	//[IN]
						   const uint8   *src_ptr,	//[IN]
						   MMI_STRING_T  *des_str	//[OUT]
						   )
{
    
    if (PNULL == src_ptr || PNULL == des_str)
    {
        SCI_TRACE_LOW("[MMIVCARD] VcardDataToWstr Error param");
        return;
    }
    
    if (is_utf8)
    {
        uint16    length;
        uint16    src_len = p_strlen(src_ptr);
        length = MIN(src_len, VC_PROP_VALUE_BUF_SIZE);
        des_str->wstr_ptr = (wchar *)SCI_ALLOC_APP(length * sizeof(wchar));
        des_str->wstr_len = GUI_UTF8ToWstr(des_str->wstr_ptr,length,
			(const uint8*)src_ptr, length);
    }
    else
    {
        MMIAPICOM_GBToSTRING((const uint8 *)src_ptr, des_str);
    }
    
}
//#endif
/*****************************************************************************/
// Description : 获取属性值
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVC_GetPropValue(VF_PROP_T     *p_prop,
                           uint32         nValue,
                           MMI_STRING_T  *p_value)
{
    char         PropValueBuffer[500] = "\0";
    uint16       buf_len = 0, buf_size = 0;
    uint16       n;
    char        *p_valuestr;
    uint16       value_len = 0;

    if((PNULL == p_prop) || (PNULL == p_value))
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_getpropvalue p_prop 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVC_COMMON_188_112_2_18_3_4_30_3,(uint8*)"d", p_prop);
        return;
    }
    buf_size = sizeof(PropValueBuffer);
    for (n = 0;n < nValue;n++)
    {
        p_valuestr = vf_get_prop_value_string(p_prop, n);
        if (NULL != p_valuestr)
        {
            buf_len   = p_strlen(PropValueBuffer);
            value_len = p_strlen(p_valuestr);
            if ((buf_len + value_len) < buf_size)
            {
                p_strcat(PropValueBuffer, p_valuestr);
            }
            else
            {
				if(buf_size > buf_len - 1 )
				{
				   p_valuestr[buf_size - buf_len - 1] = 0;
				   p_strcat(PropValueBuffer, p_valuestr);
				}                
				break;
            }
        }
    }
    p_valuestr = PropValueBuffer;
    if(NULL != p_value->wstr_ptr)
    {
        SCI_FREE(p_value->wstr_ptr);
    }
    if ( VF_CHARSET_UTF8 == vc_getcharset(p_prop))
    {
        uint16    length;
        length = p_strlen(p_valuestr);
        //p_value->is_ucs2 = TRUE;
        p_value->wstr_ptr = (wchar *)SCI_ALLOC_APP(length * sizeof(wchar));
        p_value->wstr_len = GUI_UTF8ToWstr(p_value->wstr_ptr,length,
                                         (const uint8*)p_valuestr, length);
    }
    else
    {
        MMIAPICOM_GBToSTRING((const uint8 *)p_valuestr, p_value);
    }
    return;
}

/*****************************************************************************/
// Description : free MMI_STRING_T
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIVC_freeData( MMI_STRING_T  *p_data)
{

    if( PNULL == p_data)
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_freedata PNULL == p_data"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVC_COMMON_240_112_2_18_3_4_30_4,(uint8*)"");
        return;
    }

    //p_data->is_ucs2 = FALSE;
    p_data->wstr_len  = 0;
    //Free buf
    if (PNULL != p_data->wstr_ptr)
    {
        SCI_FREE(p_data->wstr_ptr);
    }

    p_data->wstr_ptr = PNULL;

    return;
}

/*****************************************************************************/
// Description : 将时间字符串转换为日期时间结构
// Global resource dependence :
// Author:renyi.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIVC_StringToDateTime( char* string,//19960401T033000
										MMIVC_DATE_TIME_T	 *date_time)
{
    BOOLEAN ret = TRUE;
	uint32 str_len=0;

    if (PNULL!=string && PNULL!=date_time)
    {
        SCI_MEMSET(date_time, 0, sizeof(MMIVC_DATE_TIME_T)); //modify for coverity
		str_len=SCI_STRLEN(string);

		if ( str_len<8)
		{
			return FALSE;
		}
        
        date_time->year = (string[0]-'0')*1000
                +(string[1]-'0')*100
                +(string[2]-'0')*10
                +(string[3]-'0');
	
        date_time->mon= (string[4]-'0')*10
                +(string[5]-'0');
		
        date_time->mday= (string[6]-'0')*10
                +(string[7]-'0');
		
		if(str_len>10 && MMIVC_TOKEN_TIME == string[8])
		{
	        date_time->hour= (string[9]-'0')*10
	                +(string[10]-'0');
		}
		else
		{
			return FALSE;
		}
		
		if(str_len>12)
		{
	        date_time->min= (string[11]-'0')*10
	                +(string[12]-'0');
		}
		
		if(str_len>14)
		{
			date_time->sec= (string[13]-'0')*10
	                +(string[14]-'0');
		}

    }

    return ret;
}

/****************************************************************************/
//  Description:MMIVC_GetFirstValidLetterPos
//  Global resource dependence
//  Auther:
//  Note
/****************************************************************************/
PUBLIC uint16 MMIVC_GetFirstValidLetterPos(wchar *name)
{
    // BOOLEAN result = FALSE;
    wchar   *str_ptr = name;
    uint16  pos = 0;

    if(PNULL == name)
    {
        //SCI_TRACE_LOW:"[MMIVCARD] GetFirstValidLetterPos (PNULL == name) "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVC_COMMON_324_112_2_18_3_4_30_5,(uint8*)"");
        return pos;
    }
	
    while(*str_ptr)
    {
       if((' ' != *str_ptr) &&  ('.' != *str_ptr))
       {
          break;
       }
       pos++;
       str_ptr++;
    }
    return pos;
}

/*****************************************************************************/
//  Description : 保存文件的回调函数
//  Global resource dependence : 
//  Author:Bill.Ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIVC_WriteFile_CallBack(SFS_ERROR_E  error, 
                                     uint32       param,
                                     uint32       result,
                                     uint32       param2)
{
    VCARD_FILE_CALLBACK_T    ffs_result = {0};
    VCARD_FILE_CALLBACK_T   *callback_ptr = PNULL;

    //SCI_TRACE_LOW:"MMIVC_WriteFile_CallBack() error=%d, result=%d, param=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVC_COMMON_353_112_2_18_3_4_30_6,(uint8*)"ddd",error, result, param);
    SCI_MEMSET(&ffs_result, 0, sizeof(ffs_result));
    callback_ptr = (VCARD_FILE_CALLBACK_T *)param;
    if(PNULL == callback_ptr)
    {
        //SCI_TRACE_LOW:"MMIVC_WriteFile_CallBack (PNULL == callback_ptr)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVC_COMMON_358_112_2_18_3_4_30_7,(uint8*)"");
        return;
    }
    //写文件成功
    if ( SFS_NO_ERROR == error && callback_ptr->buf_len == result)
    {
        ffs_result.result = VCARD_FS_WRITE_SUCCEED;
    }
    else
    {
        ffs_result.result = VCARD_FS_WRITE_FAIL;
    }
    //post message
    ffs_result.handle    = callback_ptr->handle;
    ffs_result.win_id    = callback_ptr->win_id;
    ffs_result.msg_id    = callback_ptr->msg_id;
    ffs_result.buf_len   = callback_ptr->buf_len;
    ffs_result.buf_ptr   = callback_ptr->buf_ptr;
    ffs_result.isWaiting = callback_ptr->isWaiting;
    ffs_result.name_ptr  = callback_ptr->name_ptr;
    MMK_PostMsg(ffs_result.win_id, ffs_result.msg_id, &ffs_result,
                sizeof(VCARD_FILE_CALLBACK_T));
    //free buf
    if (PNULL != callback_ptr->buf_ptr)
    {
        SCI_FREE(callback_ptr->buf_ptr);
    }
    //free param
    SCI_FREE(callback_ptr);
    return;
}

#endif //#if defined MMI_VCARD_SUPPORT || defined MMI_VCALENDAR_SUPPORT



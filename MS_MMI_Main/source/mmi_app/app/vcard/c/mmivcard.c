/*****************************************************************************
** File Name:      mmivcard.c                                                   *
** Author:                                                                   *
** Date:           18/05/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi vcard               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2006       Jenny Sun      Create
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_vcard_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmipb_export.h"
//#include "mmipb_internal.h"
#include "mmipb_id.h"
#include "mmisms_id.h"
#include "mmifmm_export.h"
#include "mmi_appmsg.h"
//#include "mmidisplay_data.h"
//#include "mmipub.h"
#include "vf_iface.h"
#include "mmivcard_export.h"
#include "mmisd_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "vf_strings.h"
#include "vf_malloc.h"
#include "gui_ucs2b_converter.h"
#include "mmisms_export.h"
#include "mmiidle_export.h"
//#include "mmk_app.h"
#include "vf_malloc_stdlib.h"
#include "mmipub.h"
#include "vf_internals.h"
#include "guiurl.h"
#include "mmivc_common.h"
#include "mmimms_text.h"
#if defined MMI_VCARD_SUPPORT
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          STATIC DEFINITION                                */
/*---------------------------------------------------------------------------*/
LOCAL VF_FIELDINFO_T s_vf_fields[] =
{
    {VF_PROPERTY_N,     VFP_NAME},
    {VF_PROPERTY_TEL,   VFP_TELEPHONE},
    {VF_PROPERTY_EMAIL, VFP_EMAILADDRESS},
    {VF_PROPERTY_TITLE, VFP_TITLE},
    {VF_PROPERTY_ORG,   VFP_ORG},
    {VF_PROPERTY_ADR,   VFP_ADR},
    {VF_PROPERTY_NOTE,  VFP_COMMENT},
    {VF_PROPERTY_FN,    VFP_FULLNAME},
    {VF_PROPERTY_BDAY,  VFP_BIRTHDATE},
    {VF_PROPERTY_URL,   VFP_URL},
    {VF_PROPERTY_PHOTO, VFP_PHOTO},
    {VF_PROPERTY_TIMESTAMP,    VFP_TIMESTAMP},
    {VF_PROPERTY_INVALID,   ""}
};

/*LOCAL VF_FIELDINFO_T s_vf_fields30[] =
{
    {VF_PROPERTY_N,     VFP_FULLNAME},
    {VF_PROPERTY_TEL,   VFP_TELEPHONE},
    {VF_PROPERTY_EMAIL, VFP_EMAILADDRESS},
    {VF_PROPERTY_TITLE, VFP_TITLE},
    {VF_PROPERTY_ORG,   VFP_ORG},
    {VF_PROPERTY_ADR,   VFP_ADR},
    {VF_PROPERTY_NOTE,  VFP_COMMENT},
    {VF_PROPERTY_INVALID,   ""}
};*/

//VFP_HOME 与 VFP_WORK 调换顺序，为与phonebook下电话号码类型一致，否则生成的vcard文件中号码类型与号码错误
LOCAL const char *s_TelephoneTypes[] =//注意和下面的s_num_TelephoneTypes对齐
{
    VFP_CELLULAR,
    VFP_HOME,
    VFP_WORK,
    VFP_FAX,
    VFP_VOICE,
    NULL
};

//MMIPB_HOME_NUMBER 与 MMIPB_OFFICE_NUMBER 调换顺序，为与phonebook下电话号码类型一致，否则解析后的号码类型与号码错误
LOCAL uint8 s_num_TelephoneTypes[]=
{
    MMIPB_ADN_NUMBER,      //移动号码  
    MMIPB_HOME_NUMBER,     //家庭号码
    MMIPB_OFFICE_NUMBER,   //办公号码
    MMIPB_FAX_NUMBER,      //传真号码
    MMIPB_FIXED_NUMBER,    //电话号码
    MMIPB_UNKNOWN_NUMBER  //未知号码
};

LOCAL const char *s_LogDateTimeTypes[] =//注意和下面的s_num_TelephoneTypes对齐
{
    VFP_MISSED,
    VFP_RECEIVED,
    VFP_DIALED,
    VFP_INCOMING,
    VFP_OUTGOING,
    NULL
};

LOCAL uint8 s_num_LogDateTimeTypes[]=
{
#ifdef BT_DIALER_SUPPORT
    MMICL_BT_CALL_MISSED,
    MMICL_BT_CALL_RECEIVED,
    MMICL_BT_CALL_DIALED,
#else
    MMICL_CALL_MISSED,
    MMICL_CALL_RECEIVED,
    MMICL_CALL_DIALED,
#endif
    //for Huawei special types
#ifdef BT_DIALER_SUPPORT   
    MMICL_BT_CALL_RECEIVED,
    MMICL_BT_CALL_DIALED,
#else
    MMICL_CALL_RECEIVED,
    MMICL_CALL_DIALED,
#endif
    MMICL_CALL_TYPE_MAX
};


LOCAL uint8   *s_vCard_ptr = PNULL;
//LOCAL VF_MULTI_ITEM_CONTEXT  s_multi_item_context={0};
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          EXTERNAL DECLARE                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          GLOBAL DEFINITION                                */
/*---------------------------------------------------------------------------*/
LOCAL wchar             *g_vCard_name_ptr = PNULL;
//uint32 s_malloc_count=0;
//uint32 s_realloc_count=0;
//const wchar g_mmivcard_default_dir_wstr[] = { 'v', 'C', 'a', 'r', 'd', 0 }; //#define VCARD_DEFAULT_DIR          "\x00\x76\x00\x43\x00\x61\x00\x72\x00\x64\x00\x00"
//const wchar g_mmivcard_phone_dir_wstr[] = { 'v', 'C', 'a', 'r', 'd', '\\', 'p','h','o','n','e',0 };
//const wchar g_mmivcard_sim1_dir_wstr[] = { 'v', 'C', 'a', 'r', 'd', '\\', 's','i','m','1',0 };
//const wchar g_mmivcard_sim2_dir_wstr[] = { 'v', 'C', 'a', 'r', 'd', '\\', 's','i','m','2',0 };

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/****************************************************************************/
//  Description:是否是只包含空格，回车换行的字符串
//  Global resource dependence
//  Auther:
//  Note
/****************************************************************************/
LOCAL BOOLEAN isspacelfstr(uint8 *buffer_ptr);

/*****************************************************************************/
// Description : 创建vCard文件
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL SFS_ERROR_E vc_creatfile(const uint16 *file_name, 
                                  uint32       mode,
								  SFS_HANDLE* handle);

/*****************************************************************************/
// Description : 保存文件
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN VCARD_WritedFile(
                               wchar            *filename,
                               uint8            *buf_ptr,
                               uint32            buflen);

/****************************************************************************/
//  Description: Parses a vCard file
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
LOCAL VCARD_ERROR_E  VCARD_ReadFile(VF_OBJECT_T **pp_object, uint16       *name_ptr);

/*****************************************************************************/
// Description : 判断保存文件所需空间是否不足
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckDiskSpaceForSaveVCard(MMIFILE_DEVICE_E file_dev, uint32 data_size);

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
LOCAL VF_PROPERTY_TYPE_E vc_getproptype(const char      *p_name,
                                        VF_FIELDINFO_T  *p_field);


LOCAL void vc_setdatatopb( VF_PROPERTY_TYPE_E       proptype,
                           uint16                   max_len,
                           void                    *p_data,
                           MMIPB_CONTACT_T *contact_ptr);

LOCAL void vc_setteltopb(char                *p_telstr, 
                         MMIPB_BCD_NUMBER_T  *p_telnum);


LOCAL void vc_cleanuptelnum(char  *p_org_num,
                            char  *p_dst_num);


/*BEGIN*****ADD By LiuXiaohua********BEGIN**/

void MMIVC_GetTelePhoneNumber(VF_OBJECT_T               *p_Object,
                        const char                *p_PropName,
                        const char               **pp_Array,
                        MMIPB_CONTACT_T *contact_ptr);

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: init vcard module
// Global resource dependence:
// Auther: Jenny.Sun
// Note: 
/*****************************************************************************/
void MMIAPIVCARD_Init(void)
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    
	do
	{
	    if(MMIAPIFMM_GetDeviceTypeStatus(dev))
	    {
		    MMIAPIVCARD_InitFolder(
		                    MMIAPIFMM_GetDevicePath(dev),
		                    MMIAPIFMM_GetDevicePathLen(dev),
						    MMIMULTIM_DIR_VCARD, 
						    MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VCARD));	    
		}
	}while(++dev < MMI_DEVICE_NUM);
}

/*****************************************************************************/
// Description: Get the familyname of one entry in the phonebook. If it is in 
//              Chinese, the default value is the first Chinese character of
//              the name. If it is in English, the default value is the name
//              string.
// Global resource dependence:
// Auther: Jenny.Sun
// Note: This need further consideration. Especially in English.
/*****************************************************************************/
LOCAL uint16 VCARD_GetFamilyname
(
    MMIPB_CONTACT_T  *contact_ptr,  //IN
    uint8 *familyname_ptr,  //IN/OUT
    uint16 max_familyname_length
)
{
    //uint8 *temp_familyname_ptr = PNULL;
    //uint8 gbname[MMIPB_NAME_MAX_LEN*3 + 1];
    uint16 gbname_length = 0;
    
    if((PNULL == contact_ptr) || (PNULL == familyname_ptr))
    {
        //SCI_TRACE_LOW:"[MMIVCARD] VCARD_GetFamilyname contact_ptr 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_288_112_2_18_3_4_36_43,(uint8*)"d", contact_ptr);
        return gbname_length;
    }

    //SCI_MEMSET(gbname,0,(MMIPB_NAME_MAX_LEN*3 + 1));

    //temp_familyname_ptr = familyname_ptr;
    //if (entry_ptr ->entry_name.is_ucs2)
    {
        gbname_length = GUI_WstrToUTF8(familyname_ptr,((MMIPB_NAME_MAX_LEN >> 1)* 3 + 1), contact_ptr->name.wstr ,contact_ptr->name.wstr_len);
        gbname_length = MIN(gbname_length, max_familyname_length);
        if (1 <= gbname_length)
        {
			//SCI_MEMCPY(temp_familyname_ptr, gbname, gbname_length);			
            //*(temp_familyname_ptr++) = gbname[0];
            //*(temp_familyname_ptr++) = gbname[1];
            //*temp_familyname_ptr = '\0';

            //gbname_length = 2;
        }
        else
        {
            *familyname_ptr = '\0';
        }
    }      
       
    return gbname_length;
}

/*****************************************************************************/
// Description : Init vcard global var
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVCARD_BuildInit(void)
{
    //uint16 len = 0;
    MMIAPIVCARD_Init();
    MMIAPIVCARD_BuildFree();
    s_vCard_ptr = (uint8*)SCI_ALLOC_APP(VCARD_MAX_FILE_LEN*sizeof(uint8));
    SCI_ASSERT( PNULL != s_vCard_ptr );/*assert verified*/
    SCI_MEMSET(s_vCard_ptr,0,VCARD_MAX_FILE_LEN);
    return;
}

/*****************************************************************************/
// Description : Free vcard global var
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVCARD_BuildFree(void)
{
    if ( PNULL != s_vCard_ptr )
    {
        SCI_FREE( s_vCard_ptr );
        s_vCard_ptr = PNULL;
    }
    return;
}

/*****************************************************************************/
// Description : Get vcard data ptr
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 * MMIAPIVCARD_GetBuildData(void)
{
    return s_vCard_ptr;
}

/****************************************************************************/
//   Description: Build one vCard file following the protocol vCard 2.1
//   Global resource dependence
//   Auther: Jenny.Sun
//   Note
/****************************************************************************/
PUBLIC void MMIAPIVCARD_BuildVCard( MMIPB_CONTACT_T* contact_ptr)
{
	SCI_MEMSET(s_vCard_ptr, 0x00, (VCARD_MAX_FILE_LEN*sizeof(uint8)));
	MMIAPIVCARD_BuildVCardToBuffer(contact_ptr, s_vCard_ptr, VCARD_MAX_FILE_LEN);
}

/****************************************************************************/
//   Description: Build one vCard file following the protocol vCard 2.1
//   Global resource dependence
//   Auther: 
//   Note:add the build property Birth,Url of the vCard
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_BuildVCardToBuffer(MMIPB_CONTACT_T* contact_ptr, 
									 uint8 *vcard_data_ptr, 
									 uint32 vcard_max_len
									 )
{
    uint8           familyname[((MMIPB_NAME_MAX_LEN >> 1)* 3 + 1)] = {0};  // family (last)
    uint8           personal_tel[MMIPB_NUMBER_MAX_STRING_LEN +1] = "\0";
    //uint8           rev[VCARD_REV_MAX_LEN] = "\0";
    uint8           *str_buf = PNULL;
    uint8           str_buf_len = 0;	
    uint8          *vcard_ptr = vcard_data_ptr;
    //SCI_DATE_T      date= {0};
    //SCI_TIME_T      time= {0};
    uint16 familynamelength = 0;
    uint16 vcard_length = 0;
	uint8 number_index = 0;
	uint16 i = 0;	

	if((PNULL == contact_ptr) 
	   || (PNULL == vcard_data_ptr)
	   || (VCARD_MIM_LEN > vcard_max_len)
	   )
	{
		//SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_BuildVCardToBuffer parameter error !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_458_112_2_18_3_4_37_45,(uint8*)"");
         return FALSE;
	}

    SCI_MEMSET(vcard_ptr, 0x00, (vcard_max_len*sizeof(uint8)));
    strcat ((char*)vcard_ptr, "BEGIN:VCARD\r\n");
    strcat ((char*)vcard_ptr, "VERSION:2.1\r\n");
    //>>> build the property N of the vCard 
    //get the familyname string from the entry
    familynamelength= VCARD_GetFamilyname(contact_ptr,
                                    familyname,
                                    ((MMIPB_NAME_MAX_LEN >> 1)* 3 + 1));   

       
    if(familynamelength > 0)
	{
		strcat ((char*)vcard_ptr, "N;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:;");
		vcard_length = strlen((char*)vcard_ptr);
		if(vcard_length >= vcard_max_len)
		{
			return FALSE;
		}
        vf_vcard_qprint_encode(familyname, strlen((char*)familyname),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
        vcard_length = strlen((char *)vcard_ptr);
        
        strcat ((char*)vcard_ptr, ";;;\r\n");

		strcat ((char*)vcard_ptr, "FN;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:");
		vcard_length = strlen((char*)vcard_ptr);
		if(vcard_length >= vcard_max_len)
		{
			return FALSE;
		}
        vf_vcard_qprint_encode(familyname, strlen((char*)familyname),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
        vcard_length = strlen((char *)vcard_ptr);
        
        strcat ((char*)vcard_ptr, "\r\n");
    }



    //>>> build the property TEL of the vCard 
	while(NULL != s_TelephoneTypes[i] && (i < VCARD_MAX_NV_PHONE_NUM))
   {
        if(MMIAPIPB_IsValidPhoneNumber(contact_ptr->number[i].npi_ton, contact_ptr->number[i].number_len))/*lint !e64*/
        {
            //convert the personal BCD number to number string
			MMIAPICOM_GenDispNumber(((MN_NUMBER_TYPE_E)(contact_ptr->number[i].npi_ton >> 4)),
									(uint8)(contact_ptr->number[i].number_len),
				                     contact_ptr->number[i].number,
									 personal_tel,
									 MMIPB_MAX_STRING_LEN);
            //build the property TEL of the vCard  CELL
			number_index = i;				
			vcard_length = strlen((char *)vcard_ptr) + strlen((char *)s_TelephoneTypes[number_index]) + strlen((char *)personal_tel);
			if(vcard_length + 7 >= vcard_max_len)
			{
				return FALSE;
			}
            strcat ((char*)vcard_ptr, "TEL;");
            strcat ((char*)vcard_ptr,s_TelephoneTypes[number_index]);
            strcat ((char*)vcard_ptr, ":");
            strcat ((char*)vcard_ptr, (char*)personal_tel);
            strcat ((char*)vcard_ptr, "\r\n");
        }
        i ++;
    }  
#ifdef MMIPB_MAIL_SUPPORT
    //>>> build the property EMAIL of the vCard
    if ( 0 != contact_ptr->mail.wstr_len)
    {
		str_buf_len = contact_ptr->mail.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			GUI_WstrToUTF8_vCard(str_buf, str_buf_len, contact_ptr->mail.wstr, contact_ptr->mail.wstr_len);
			vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
			if(vcard_length + strlen("EMAIL;PREF;INTERNET:") + 2>= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}
			//MMIAPICOM_UCS2ToGB(&str_tmp, str_buf);        
			strcat ((char*)vcard_ptr, "EMAIL;PREF;INTERNET:");
			strcat ((char*)vcard_ptr,(char*)str_buf);
			strcat ((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    //>>>build the property Meno of the vCard
    if (0!=contact_ptr->memo.wstr_len) 
    {
		str_buf_len = contact_ptr->memo.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			//if(entry_ptr->custom_text.is_ucs2)
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, contact_ptr->memo.wstr, contact_ptr->memo.wstr_len);
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("NOTE;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:") + 2>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}
				strcat ((char*)vcard_ptr, "NOTE;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:");
			}
			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}			
			vf_vcard_qprint_encode(str_buf, strlen((char*)str_buf),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
			strcat((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}

    }
    
	//Bug 160831
    //>>>build the property Birth of the vCard
    if (0!=contact_ptr->birth.wstr_len) 
    {
		str_buf_len = contact_ptr->birth.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, contact_ptr->birth.wstr, contact_ptr->birth.wstr_len);
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("BDAY;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:") + 2>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}
				strcat ((char*)vcard_ptr, "BDAY;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:");
			}
			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}			
			strcat ((char*)vcard_ptr,(char*)str_buf);
            strcat((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }

    //>>>build the property Url of the vCard
    if (0!=contact_ptr->url.wstr_len) 
    {
        str_buf_len = contact_ptr->url.wstr_len * 3 + 1;
        str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
        if(PNULL != str_buf)
        {
            {
                GUI_WstrToUTF8_vCard(str_buf, str_buf_len, contact_ptr->url.wstr, contact_ptr->url.wstr_len);
                vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
                if(vcard_length + strlen("URL;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:") + 2>= vcard_max_len)
                {
                    SCI_FREE(str_buf);
                    str_buf = PNULL;
                    return FALSE;
                }
                strcat ((char*)vcard_ptr, "URL;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:");
            }
            vcard_length = strlen((char*)vcard_ptr);
            if(vcard_length >= vcard_max_len)
            {
                SCI_FREE(str_buf);
                str_buf = PNULL;
                return FALSE;
            }           
            vf_vcard_qprint_encode(str_buf, strlen((char*)str_buf),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
            strcat((char*)vcard_ptr, "\r\n");
            SCI_FREE(str_buf);
            str_buf = PNULL;
        }
    }

    
#if 0	
    //>>> build the property TITLE of the vCard
    if ( 0 != contact_ptr->title.wstr_len )
    {
		str_buf_len = contact_ptr->title.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			//if(entry_ptr->title.is_ucs2)
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, contact_ptr->title.wstr, contact_ptr->title.wstr_len); 
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("TITLE;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:") + 2>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}		
				strcat((char*)vcard_ptr, "TITLE;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:");
			}
			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}
			vf_vcard_qprint_encode(str_buf, strlen((char*)str_buf),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
			strcat((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }
    //>>> build the property ORG of the vCard
    if ( 0 != contact_ptr->company.wstr_len )
    {
		str_buf_len = contact_ptr->company.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			//	if(entry_ptr->org.is_ucs2)
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, contact_ptr->company.wstr, contact_ptr->company.wstr_len);
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("ORG;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:") + 2>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}
				strcat((char*)vcard_ptr, "ORG;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:");
			}
       
			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}			
			vf_vcard_qprint_encode(str_buf, strlen((char*)str_buf),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
			strcat((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }

    //>>> build the property ADR of the vCard
    if ( 0 != contact_ptr->address.wstr_len )
    {
		str_buf_len = contact_ptr->address.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			//if(entry_ptr->work_adr.is_ucs2)
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, contact_ptr->address.wstr, contact_ptr->address.wstr_len);
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("ADR;WORK;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:") + 7>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}
				strcat((char*)vcard_ptr, "ADR;WORK;CHARSET=UTF-8;ENCODING=QUOTED-PRINTABLE:");
			}

			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}
			vf_vcard_qprint_encode(str_buf, strlen((char*)str_buf),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
			//strcat((char*)vcard_ptr,(char*)str_buf);
			strcat((char*)vcard_ptr,";;;");
			strcat((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }
#endif
#endif
//    //get the last revsion
//    TM_GetSysDate( &date );
//    TM_GetSysTime( &time );
//    VCARD_GetRev(&date, &time, rev);
//	if(vcard_length + strlen("REV:") + strlen((char *)rev) + 2>= vcard_max_len)
//	{
//		return FALSE;
//	}
//    //build the property REV of the vCard
//    strcat ((char*)vcard_ptr, "REV:");
//    strcat ((char*)vcard_ptr, (char*)rev);
//     strcat ((char*)vcard_ptr, "\r\n");
	if(vcard_length + strlen("END:VCARD\r\n") >= vcard_max_len)
	{
		return FALSE;
	}
    strcat ((char*)vcard_ptr, "END:VCARD\r\n");
    return TRUE;      
}

/****************************************************************************/
//   Description: Build one vCard file following the protocol vCard 3.0
//   Global resource dependence
//   Auther: 
//   Note
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_BuildVCard30ToBuffer(MMIPB_CONTACT_T  *entry_ptr, 
                                          uint8 *vcard_data_ptr, 
                                          uint32 vcard_max_len
                                          )
{
    uint8           familyname[((MMIPB_NAME_MAX_LEN >> 1)* 3 + 1)] = {0};  // family (last)
    uint8           personal_tel[MMIPB_NUMBER_MAX_STRING_LEN +1] = "\0";
    //uint8           rev[VCARD_REV_MAX_LEN] = "\0";
    uint8           *str_buf = PNULL;
    uint8           str_buf_len = 0;	
    uint8          *vcard_ptr = vcard_data_ptr;
    //SCI_DATE_T      date= {0};
    //SCI_TIME_T      time= {0};
    uint16 familynamelength = 0;
    uint16 vcard_length = 0;
	uint8 number_index = 0;
	uint16 i = 0;	

	if((PNULL == entry_ptr) 
	   || (PNULL == vcard_data_ptr)
	   || (VCARD_MIM_LEN > vcard_max_len)
	   )
	{
		//SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_BuildVCardToBuffer parameter error !"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_727_112_2_18_3_4_37_46,(uint8*)"");
         return FALSE;
	}

    SCI_MEMSET(vcard_ptr, 0x00, (vcard_max_len*sizeof(uint8)));
    strcat ((char*)vcard_ptr, "BEGIN:VCARD\r\n");
    strcat ((char*)vcard_ptr, "VERSION:3.0\r\n");
    //>>> build the property N of the vCard 
    //get the familyname string from the entry
    familynamelength= VCARD_GetFamilyname(entry_ptr,
                                    familyname,
                                    ((MMIPB_NAME_MAX_LEN >> 1)* 3 + 1));

       
    if(familynamelength > 0)
	{
		strcat ((char*)vcard_ptr, "FN:");
		vcard_length = strlen((char*)vcard_ptr);
		if(vcard_length >= vcard_max_len)
		{
			return FALSE;
		}
        
        strcat ((char*)vcard_ptr, (char*)familyname);
        vcard_length = strlen((char *)vcard_ptr);
        
        strcat ((char*)vcard_ptr, "\r\n");

        strcat ((char*)vcard_ptr, "N:;");
		vcard_length = strlen((char*)vcard_ptr);
		if(vcard_length >= vcard_max_len)
		{
			return FALSE;
		}
        strcat ((char*)vcard_ptr, (char*)familyname);
        vcard_length = strlen((char *)vcard_ptr);
        
        strcat ((char*)vcard_ptr, ";;;\r\n");
    }



    //>>> build the property TEL of the vCard 
	while(NULL != s_TelephoneTypes[i] && (i < VCARD_MAX_NV_PHONE_NUM))
   {
        if(MMIAPIPB_IsValidPhoneNumber(entry_ptr->number[i].npi_ton, entry_ptr->number[i].number_len))/*lint !e64*/
        {
            //convert the personal BCD number to number string
			MMIAPICOM_GenDispNumber(((MN_NUMBER_TYPE_E)(entry_ptr->number[i].npi_ton >> 4)),
									(uint8)(entry_ptr->number[i].number_len),
				                     entry_ptr->number[i].number,
									 personal_tel,
									 MMIPB_MAX_STRING_LEN);
            //build the property TEL of the vCard  CELL
			number_index = i;				
			vcard_length = strlen((char *)vcard_ptr) + strlen((char *)s_TelephoneTypes[number_index]) + strlen((char *)personal_tel);
			if(vcard_length + 7 >= vcard_max_len)
			{
				return FALSE;
			}
            strcat ((char*)vcard_ptr, "TEL;");
            strcat ((char*)vcard_ptr, "TYPE=");
            strcat ((char*)vcard_ptr,s_TelephoneTypes[number_index]);
            strcat ((char*)vcard_ptr, ":");
            strcat ((char*)vcard_ptr, (char*)personal_tel);
            strcat ((char*)vcard_ptr, "\r\n");
        }
        i ++;
    }  
#ifdef MMIPB_MAIL_SUPPORT
    //>>> build the property EMAIL of the vCard
    if ( 0 != entry_ptr->mail.wstr_len)
    {
		str_buf_len = entry_ptr->mail.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			GUI_WstrToUTF8_vCard(str_buf, str_buf_len, entry_ptr->mail.wstr, entry_ptr->mail.wstr_len);
			vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
			if(vcard_length + strlen("EMAIL;TYPE=PREF,INTERNET:") + 2>= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}
			//MMIAPICOM_UCS2ToGB(&str_tmp, str_buf);        
			strcat ((char*)vcard_ptr, "EMAIL;TYPE=PREF,INTERNET:");
			strcat ((char*)vcard_ptr,(char*)str_buf);
			strcat ((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }
#endif
#ifndef PB_SUPPORT_LOW_MEMORY
    //>>>build the property Meno of the vCard
    if (0 != entry_ptr->memo.wstr_len)
    {
		str_buf_len = entry_ptr->memo.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			//if(entry_ptr->custom_text.is_ucs2)
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, entry_ptr->memo.wstr, entry_ptr->memo.wstr_len);
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("NOTE:") + 2>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}
				strcat ((char*)vcard_ptr, "NOTE:");
			}
			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}			
            strcat((char*)vcard_ptr, (char*)str_buf);
			strcat((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}

    }
#if 0
    //>>> build the property TITLE of the vCard
    if ( 0 != entry_ptr->title.wstr_len )
    {
		str_buf_len = entry_ptr->title.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			//if(entry_ptr->title.is_ucs2)
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, entry_ptr->title.wstr, entry_ptr->title.wstr_len); 
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("TITLE:") + 2>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}		
				strcat((char*)vcard_ptr, "TITLE:");
			}
			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}
            strcat((char*)vcard_ptr, (char*)str_buf);
            strcat((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }
    //>>> build the property ORG of the vCard
    if ( 0 != entry_ptr->company.wstr_len )
    {
		str_buf_len = entry_ptr->company.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			//	if(entry_ptr->org.is_ucs2)
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, entry_ptr->company.wstr, entry_ptr->company.wstr_len);
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("ORG:") + 2>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}
				strcat((char*)vcard_ptr, "ORG:");
			}
       
			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}			
			//vf_vcard_qprint_encode(str_buf, strlen((char*)str_buf),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
            strcat((char*)vcard_ptr, (char*)str_buf);
            strcat((char*)vcard_ptr, ";;\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }

    //>>> build the property ADR of the vCard
    if ( 0 != entry_ptr->address.wstr_len )
    {
		str_buf_len = entry_ptr->address.wstr_len * 3 + 1;
		str_buf = (uint8 *)SCI_ALLOCAZ(str_buf_len); 
		if(PNULL != str_buf)
		{
			//SCI_MEMSET(str_buf, 0, str_buf_len);
			//if(entry_ptr->work_adr.is_ucs2)
			{
				GUI_WstrToUTF8_vCard(str_buf, str_buf_len, entry_ptr->address.wstr, entry_ptr->address.wstr_len);
				vcard_length = strlen((char *)vcard_ptr) + strlen((char *)str_buf);
				if(vcard_length + strlen("ADR;TYPE=WORK:") + 7>= vcard_max_len)
				{
                    SCI_FREE(str_buf);
			        str_buf = PNULL;
					return FALSE;
				}
				strcat((char*)vcard_ptr, "ADR;TYPE=WORK:");
			}

			vcard_length = strlen((char*)vcard_ptr);
			if(vcard_length >= vcard_max_len)
			{
                SCI_FREE(str_buf);
			    str_buf = PNULL;
				return FALSE;
			}
			//vf_vcard_qprint_encode(str_buf, strlen((char*)str_buf),(char*)(vcard_ptr+ vcard_length), (vcard_max_len - vcard_length), TRUE);
			strcat((char*)vcard_ptr,(char*)str_buf);
			strcat((char*)vcard_ptr,";;;;;;");
			strcat((char*)vcard_ptr, "\r\n");
			SCI_FREE(str_buf);
			str_buf = PNULL;
		}
    }
#endif
#endif

	if(vcard_length + strlen("END:VCARD\r\n") >= vcard_max_len)
	{
		return FALSE;
	}
    strcat ((char*)vcard_ptr, "END:VCARD\r\n");
    return TRUE;      
}

/****************************************************************************/
//   Description: Get Vcard File Name For PB
//   Global resource dependence
//   Auther: 
//   Note
/****************************************************************************/
PUBLIC void MMIAPIVCARD_GetVcardFileName( MMIPB_CONTACT_T  *contact_ptr,
                                    PHONEBOOK_STORAGE_E       entry_storage,
                                    wchar*file_name)
{
    uint8            name_str[VCARD_NAME_MAX_LEN] = {0};
    uint16           name_len = 0;
	uint16           max_len = 0;
    uint16           valid_pos = 0;
    const char      *p_ext=VCARD_DEFAULT_SUBFIX;
     
    if(PNULL == contact_ptr ||PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIVCARD] VCARD_GetVcardFileName PNULL == contact_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_1014_112_2_18_3_4_38_47,(uint8*)"");
        return;
    }

    switch(entry_storage)
    {
      case PHONEBOOK_SIM_STORAGE :
         sprintf((char*)name_str, "%s%d%s",
                 VCARD_SIM_DEFAULT_PREFIX,
                 contact_ptr->contact_id,
                 VCARD_DEFAULT_SUBFIX);
         break;
      case PHONEBOOK_NV_STORAGE:
         sprintf((char*)name_str, "%s%d%s",
                 VCARD_NV_DEFAULT_PREFIX,
                 contact_ptr->contact_id,
                 VCARD_DEFAULT_SUBFIX);
        break; 
      default:
        break;
    }
    if(0<contact_ptr->name.wstr_len)
    {

        if (MMIAPICOM_IsIncludeInvalidChar(contact_ptr->name.wstr,contact_ptr->name.wstr_len))
        {
            name_len = MIN(strlen((char*)name_str), VCARD_NAME_MAX_LEN); /*lint !e666*/
            MMI_STRNTOWSTR((wchar *)file_name, VCARD_NAME_MAX_LEN, name_str, name_len, name_len);
        }
        else 
        {
            valid_pos = MMIVC_GetFirstValidLetterPos(contact_ptr->name.wstr);

            max_len = MIN(MMIAPICOM_Wstrlen(contact_ptr->name.wstr),(VCARD_NAME_MAX_LEN - VCARD_DEFAULT_SUBFIX_LEN - MMIAPICOM_Wstrlen(MMIMULTIM_DIR_VCARD) -1));/*lint !e666*/
            max_len = MIN(max_len,MMIPB_NAME_MAX_LEN);/*lint !e831*/

            if((valid_pos >= max_len) )/*lint !e831*/
            {
                name_len = MIN(strlen((char*)name_str), VCARD_NAME_MAX_LEN); /*lint !e666*/
                MMI_STRNTOWSTR((wchar *)file_name, VCARD_NAME_MAX_LEN, name_str, name_len, name_len);
            }
            else
            {
                max_len = max_len - valid_pos;
                MMIAPICOM_Wstrncpy(file_name, &contact_ptr->name.wstr[valid_pos], max_len);/*lint !e831*/
                MMI_STRNTOWSTR(&file_name[max_len], VCARD_DEFAULT_SUBFIX_LEN,(uint8*)p_ext, VCARD_DEFAULT_SUBFIX_LEN, VCARD_DEFAULT_SUBFIX_LEN);
                file_name[max_len + VCARD_DEFAULT_SUBFIX_LEN] = 0;
            }
        }
    }
    else
    {
         name_len = MIN(strlen((char*)name_str), VCARD_NAME_MAX_LEN); /*lint !e666*/
         MMI_STRNTOWSTR((wchar *)file_name, VCARD_NAME_MAX_LEN, name_str, name_len, name_len);

       //GUI_GB2UCS((uint16*)vCard_name_ptr,(const uint8 *)name_str,name_len);
    }
}

/****************************************************************************/
//   Description: Get Vcard default full path
//   Global resource dependence
//   Auther: 
//   Note
/****************************************************************************/
PUBLIC void MMIAPIVCARD_GetVcardDefaultFileName( 
                                      const wchar *file_path_ptr,//IN
                                      wchar*file_full_name,//OUT
                                      uint16 name_max_len//IN
                                      )
{
    uint16 str_len = 0;
    uint16 name_len = 0;

    if(file_full_name != PNULL && file_path_ptr != PNULL)
    {
        name_len = MMIAPICOM_Wstrlen(file_path_ptr);
        //folder
        if(name_len < name_max_len)
        {
            MMIAPICOM_Wstrcpy(file_full_name,file_path_ptr);
        }
        if(name_len + 1 < name_max_len) 
        {
            MMIAPICOM_Wstrcat(file_full_name, L"\\");
            name_len += 1;
        }
        //name前缀
        str_len = strlen(VCARD_NV_DEFAULT_PREFIX);
        if(name_len + str_len < name_max_len)
        {
            MMI_STRNTOWSTR(
                &file_full_name[name_len],
                (name_max_len - name_len),
                (uint8 *)VCARD_NV_DEFAULT_PREFIX,
                str_len,
                str_len
                );
            name_len += str_len;
        }
        //添加时间时间戳
        if(name_len < name_max_len)
        {
            MMIAPICOM_GetDataTimeString(&file_full_name[name_len], (name_max_len - name_len));
        }
        //添加后缀
        name_len = MMIAPICOM_Wstrlen(file_full_name);
        str_len = strlen(VCARD_DEFAULT_SUBFIX);
        if(name_len + str_len < name_max_len)
        {
            MMI_STRNTOWSTR(
                &file_full_name[name_len],
                (name_max_len - name_len),
                (uint8 *)VCARD_DEFAULT_SUBFIX,
                str_len,
                str_len
                );
            name_len += str_len;
        }
        MMIAPICOM_GetNoExistFileName(file_full_name, MMIAPICOM_Wstrlen(file_full_name));
    }
}
/*****************************************************************************/
// Description : 创建vCard目录
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_InitFolder(const wchar   *device_ptr,
                                uint16   device_len,
                                const wchar   *dir_ptr,
                                uint16   dir_len)
{
    BOOLEAN    result = FALSE;
    wchar     full_path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    wchar     full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;

    if((PNULL == device_ptr) || (PNULL == dir_ptr) || (0 == device_len) || (0 == dir_len))
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_InitFolder device_ptr 0x%x dir_ptr 0x%x device_len %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_1191_112_2_18_3_4_38_49,(uint8*)"ddd", device_ptr, dir_ptr, device_len);
        return FALSE;
    }
    vf_stdlib_init_mem();
    if(MMIAPIFMM_CombineFullPath((wchar *)device_ptr, device_len,
                            (wchar *)dir_ptr, dir_len, 
                            NULL, 0, 
                            full_path_name, &full_path_len))
    {
	    if(!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
	    {
	        result = MMIAPIFMM_CreateDir(full_path_name, full_path_len);
	    }
	    else
	    {
	        result = TRUE;
	    }
    }
    return result;
}

/*****************************************************************************/
// Description : 创建vCard文件
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL SFS_ERROR_E vc_creatfile(const uint16 *file_name, 
                                  uint32       mode,
								  SFS_HANDLE* handle)
{
    uint16    path_name[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    uint16    path_len = 0;
	uint16    full_path_len = 0;
	SFS_ERROR_E ret_val = SFS_ERROR_INVALID_PARAM;

    //SCI_TRACE_LOW:"mmivcard:vc_creatfile()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_1225_112_2_18_3_4_38_50,(uint8*)"");
    if (PNULL == file_name || PNULL == handle)
    {
        return SFS_ERROR_INVALID_PARAM;
    }
	full_path_len = MMIAPICOM_Wstrlen(file_name);
	MMIAPICOM_GetFilePath(file_name, full_path_len, path_name, &path_len);
	if(full_path_len > path_len + 2)
	{
		ret_val = SFS_CreateFileAsyn(path_name, &(file_name[path_len + 1]), mode, NULL, NULL, handle, PNULL);
	}
    return ret_val;
}

/*****************************************************************************/
// Description : 判断保存文件所需空间是否不足
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckDiskSpaceForSaveVCard(MMIFILE_DEVICE_E file_dev, uint32 data_size)
{
    BOOLEAN    bRet = TRUE;
    uint32     space = 0;
    wchar*     dev_path_ptr = PNULL;
    uint16     dev_len = 0;

    if(MMI_DEVICE_NUM ==file_dev)
    {
        return FALSE;
    }
    dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));
    dev_path_ptr =  MMIAPIFMM_GetDevicePath((file_dev));

    if(!MMIAPIFMM_GetDeviceStatus(dev_path_ptr, dev_len))
    {
        return FALSE;
    }
    
    space = MMI_GetFreeSpace(file_dev);
    if ((data_size >>10) > space)
    {
        bRet = FALSE;
    }
    return bRet;
}

/*****************************************************************************/
// Description : 保存文件
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCard_SaveVCardByData(
                                wchar           *file_name,//[IN/OUT]: vcard file name to show
                                uint8            *data_ptr,
                                uint32            data_len)
{
    //delete unused variable for coverity
    //wchar*          dev_path_ptr = PNULL;
    //uint32          dev_len = 0;
    MMIFILE_HANDLE  file_handle = SFS_INVALID_HANDLE;
    VCARD_ERROR_E   result = VCARD_FILE_ERROR;
    MMIFILE_DEVICE_E    fs_dev = MMI_DEVICE_NUM;
    wchar                      * file_ptr = PNULL;
    if(PNULL == data_ptr || PNULL == file_name)
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCard_SaveVCardByData data_ptr 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_1619_112_2_18_3_4_39_55,(uint8*)"d", data_ptr);
        return result;
    }
    if (0 == data_len)
    {
        //SCI_TRACE_LOW:"MMIAPIVCard_SaveVCardByData data_len %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_1624_112_2_18_3_4_39_56,(uint8*)"d", data_len);
        result = VCARD_EMPTY_FILE;
    }
    else
    {
        {           
            fs_dev = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);

            if(CheckDiskSpaceForSaveVCard(fs_dev, data_len))
            {                
                //delete unused variable for coverity
                //dev_len = MMIAPIFMM_GetDevicePathLen((fs_dev));
                //dev_path_ptr =  MMIAPIFMM_GetDevicePath((fs_dev));
                file_ptr = SCI_ALLOCA((MMIFILE_FULL_PATH_MAX_LEN +1)*sizeof(wchar));
                if(file_ptr != PNULL)
                {
                    SCI_MEMSET(file_ptr, 0x00, (MMIFILE_FULL_PATH_MAX_LEN +1)*sizeof(wchar)); 
                    MMIAPICOM_Wstrncpy(file_ptr, file_name, MMIAPICOM_Wstrlen(file_name));
                    MMIAPICOM_CombineDefaultFullPathByFilename(fs_dev, file_ptr, MMIAPICOM_Wstrlen(file_ptr), 
                                                       file_name, MMIFILE_FULL_PATH_MAX_LEN);
                    SCI_FREE(file_ptr);
                }
                {
                    //创建文件

                    //MMIAPIFMM_CloseFile(file_handle);
                    MMIAPICOM_GetNoExistFileName(file_name, MMIAPICOM_Wstrlen(file_name));
                    result = vc_creatfile((const uint16 *)file_name,
                        SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, &file_handle);/*lint !e64 !e655*/
                    if(SFS_INVALID_HANDLE != file_handle)
                    {
                        MMIAPIFMM_CloseFile(file_handle);
                    }
                    if(VCARD_WritedFile(file_name, data_ptr, data_len))
                    {
                        result = VCARD_NO_ERROR;
                    }
                    else
                    {
                        result = VCARD_FILE_ERROR;
                    }
                }
            }//memory enough
            else
            {
                result = VCARD_TOBIG_FILE;
            }//no memory
        }//if(PNULL != vcard_buf)
    }//(0 < data_len)
    //SCI_TRACE_LOW:"MMIAPIVCard_SaveVCardByData result %d !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_1694_112_2_18_3_4_39_57,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
// Description : 保存文件
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN VCARD_WritedFile(
                               wchar            *filename,
                               uint8            *buf_ptr,
                               uint32            buflen)
{
    BOOLEAN                  result = FALSE;
    uint32                   transmitted = 0;
    MMIFILE_ERROR_E          ffs_error = SFS_NO_ERROR;
    MMIFILE_HANDLE           file_handle = SFS_INVALID_HANDLE;

    //入口参数检查
    if((PNULL == filename) || (PNULL == buf_ptr))
    {
        //SCI_TRACE_LOW:"[MMIVCARD]VCARD_WritedFile filename 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_1719_112_2_18_3_4_39_58,(uint8*)"d", filename);
    }
    else
    { //创建文件
	    ffs_error = vc_creatfile((const uint16 *)filename,
	                               SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, &file_handle);/*lint !e655*/	   
	    //异步写文件数据
	    if(SFS_INVALID_HANDLE != file_handle)
	    {
	        //传递参数的赋值
			ffs_error = MMIAPIFMM_SetFileSize(file_handle, buflen);
			if(ffs_error == SFS_NO_ERROR)
			{
                           
				ffs_error = MMIAPIFMM_WriteFile(file_handle,
					buf_ptr,
					buflen,
					&transmitted, PNULL);
				//SCI_TRACE_LOW:"VCARD_WritedFile: ffs_error=%d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_1748_112_2_18_3_4_39_59,(uint8*)"d", ffs_error);
				if (ffs_error == SFS_NO_ERROR)
				{					
					result = TRUE;
				}
			}
			else
			{
				MMIAPIFMM_CloseFile(file_handle);
				MMIAPIFMM_DeleteFile((const uint16 *)filename, PNULL);							
			}
            MMIAPIFMM_CloseFile(file_handle);
	    }
	}
    return result;
}

/****************************************************************************/
//  Description: Vcard Find version
//  Global resource dependence
//  Auther:renyi.hu
//  Note: 在字符串中找另一个字符串，num为找到的数量，返回最后找到的指针
/****************************************************************************/
LOCAL uint32 VcardFindVersion(const char* buff )
{
    uint32 ver=0;
    char *version2_1="VERSION:2.1\r\n";
    char *version3_0="VERSION:3.0\r\n";
    
    char *version=p_stristr(buff,version2_1);
    if(version!=PNULL)
    {

        ver=VCARD_VERSION_21;
    }
    else
    {
        version=p_stristr(buff,version3_0);
        if(version!=PNULL)
        {
            ver=VCARD_VERSION_30;
        }
    }

    return ver;
}

/****************************************************************************/
//  Description: Parses a vCard file
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
LOCAL VCARD_ERROR_E VCARD_ReadFile(VF_OBJECT_T **pp_object,
                        uint16       *name_ptr)
{
    VCARD_ERROR_E     ret = VCARD_FORMAT_ERROR;
	BOOLEAN           parse_ret = FALSE;
    MMIFILE_HANDLE    file_handle = SFS_INVALID_HANDLE;
    char              *buffer=PNULL;
    uint32            charsread = 0;
    VF_PARSER_T      *p_parser;
    MMIFILE_ERROR_E   ffs_error;
    uint16            file_size = 0;
    uint32 ver=0;
    Vcard_parse_text_fn parse_fn=vf_parse_text;
    
    if((PNULL== pp_object) || (PNULL == name_ptr))
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_ReadFile pp_object 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2181_112_2_18_3_4_40_64,(uint8*)"d", pp_object);
        return ret;
    }

    //打开文件
    ffs_error = vc_creatfile((const uint16 *)name_ptr,
                               SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, &file_handle);/*lint !e655*/
    //SCI_TRACE_LOW:"mmivcard:MMIAPIVCARD_ReadFile() file_handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2188_112_2_18_3_4_40_65,(uint8*)"d", file_handle);
    if (file_handle)
    {
        //判断文件是否超长
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        if (0 == file_size)
        {
            //if(SFS_INVALID_HANDLE != file_handle)
            {
                MMIAPIFMM_CloseFile(file_handle);
            }
			//if(0 == file_size)
			//{
				ret = VCARD_EMPTY_FILE;
			//}
			//else
			//{
            //    ret = VCARD_TOBIG_FILE;
			//}
            return ret;
        }
        buffer=(char*)SCI_ALLOCAZ(VCARD_READ_BUFFER_LEN+1);
        if(PNULL ==buffer)
        {
            return ret;
        }
        //Set the memory allocation functions
        vf_set_mem_functions(MMIVC_Malloc, MMIVC_Realloc, MMIVC_Free);
        if (vf_parse_init(&p_parser, pp_object))
        {
            file_size = 0;
            do
            {
                //同步读文件
                SCI_MEMSET(buffer, 0, VCARD_READ_BUFFER_LEN+1);
                ffs_error = MMIAPIFMM_ReadFile(file_handle,buffer,VCARD_READ_BUFFER_LEN,&charsread,PNULL);
                file_size += charsread;
                if(0==ver)
                {
                    ver=VcardFindVersion(buffer);
                    if(0==ver)
                    {
                        ret = VCARD_FORMAT_ERROR; 
                        MMIAPIFMM_CloseFile(file_handle);
                        SCI_FREE(buffer);
                        return ret; 
                    }
                    else
                    {
                        if(VCARD_VERSION_30==ver)
                        {
                            parse_fn=vf_parse_text_v30;
                        }
                    }
                }
                if (0 < charsread && SFS_NO_ERROR == ffs_error)
                {
                    parse_ret = parse_fn(p_parser, buffer, charsread);
                }
            }
            while (parse_ret && (0 < charsread) && (file_size <= VCARD_MAX_FILE_LEN));

            if (vf_parse_end(p_parser))
            {
                if(*pp_object!=PNULL)
                {
                    VOBJECT_T * tmp=(VOBJECT_T *)(*pp_object);
                    tmp->version=ver;
                }
                ret = VCARD_NO_ERROR;
            }
        }
        
        MMIAPIFMM_CloseFile(file_handle);
        SCI_FREE(buffer);
    }
	else
	{
		ret = VCARD_FILE_ERROR;
	}
    return ret;
}

/****************************************************************************/
//  Description: Parses a vCard data
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_ParseByData(VF_OBJECT_T **pp_object, uint8 *data_ptr, uint32 data_size)
{
    BOOLEAN ret = TRUE;
    VF_PARSER_T      *p_parser;
    uint32 ver=0;
    Vcard_parse_text_fn parse_fn=vf_parse_text;
    char *buffer=PNULL;

    if((PNULL== pp_object) || (PNULL == data_ptr))
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_ParseByData pp_object 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2283_112_2_18_3_4_40_66,(uint8*)"d", pp_object);
        return FALSE;
    }
    buffer=(char*)SCI_ALLOCA(data_size+1);
    if(PNULL!=buffer)
    {
       SCI_MEMSET(buffer,0,data_size+1);
       SCI_MEMCPY(buffer,data_ptr,data_size);
    }
    else
    {
       return FALSE; 
    }
    
    vf_set_mem_functions(MMIVC_Malloc, MMIVC_Realloc, MMIVC_Free);
    if (vf_parse_init(&p_parser, pp_object))
    {
        
        ver=VcardFindVersion(buffer);
        if(0==ver)
        {
            ret = VCARD_FORMAT_ERROR; 
            SCI_FREE(buffer);
            return ret; 
        }
        else
        {
            if(VCARD_VERSION_30==ver)
            {
                parse_fn=vf_parse_text_v30;
            }
        }
        ret = parse_fn(p_parser, (char *)data_ptr, data_size);

        if(*pp_object!=PNULL)
        {
            VOBJECT_T * tmp=(VOBJECT_T *)(*pp_object);
            tmp->version=ver;
        }
        
        if (!vf_parse_end(p_parser))
        {
            ret = FALSE;
        }
    }
    SCI_FREE(buffer);
    return ret;
}
/****************************************************************************/
//  Description: Read vCard file to global variable
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCARD_GetDataByFile(uint16    *name_ptr)
{
    VCARD_ERROR_E     ret = VCARD_FILE_ERROR;
    MMIFILE_HANDLE    file_handle = SFS_INVALID_HANDLE;
    uint32            charsread = 0;
    MMIFILE_ERROR_E   ffs_error = SFS_NO_ERROR;
    uint16            file_size = 0;
    uint8            *p_data = PNULL;
    
    if(PNULL== name_ptr)
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_GetDataByFile name_ptr 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2346_112_2_18_3_4_41_67,(uint8*)"d", name_ptr);
        return ret;
    }
    //打开文件
    ffs_error = vc_creatfile((const uint16 *)name_ptr,
                               SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, &file_handle);/*lint !e655*/
    //SCI_TRACE_LOW:"mmivcard:MMIAPIVCARD_GetDataByFile() file_handle=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2352_112_2_18_3_4_41_68,(uint8*)"d", file_handle);
    if (file_handle)
    {
        //文件长度检查
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        if ( 0 == file_size )
        {
            ret = VCARD_EMPTY_FILE;
        }
        else if ( file_size > VCARD_MAX_FILE_LEN)
        {
            ret = VCARD_TOBIG_FILE;
        }
        else
        {
            //初始化缓冲区
            MMIAPIVCARD_BuildInit();
            p_data = MMIAPIVCARD_GetBuildData();
            if(p_data != PNULL)
            {
                //同步读文件
                ffs_error = MMIAPIFMM_ReadFile(file_handle,p_data,file_size,&charsread,PNULL);
                if (0 < charsread && SFS_NO_ERROR == ffs_error)
                {
                    ret = VCARD_NO_ERROR;
                }                
            }
        }

    }
    if(SFS_INVALID_HANDLE != file_handle)
    {
        MMIAPIFMM_CloseFile(file_handle);
    }
    return ret;
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
LOCAL VF_PROPERTY_TYPE_E vc_getproptype(const char      *p_name,
                                        VF_FIELDINFO_T  *p_field)
{
    uint32               i = 0;
    char                *p_tmp;
    VF_PROPERTY_TYPE_E   ret = VF_PROPERTY_INVALID;


    if (NULL == p_name || ( PNULL == p_field)) 
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_getproptype p_name 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2403_112_2_18_3_4_41_69,(uint8*)"d", p_name);
        return ret;
    }

    while(VF_PROPERTY_INVALID != p_field[i].vf_type)
    {
        p_tmp = p_field[i].pszPropName;
        if ( 0 == p_strcmp(p_name, p_tmp))
        {
            ret = p_field[i].vf_type;
            break;
        }
        i++;
    }
    return ret;
}


LOCAL void vc_setdatatopb( VF_PROPERTY_TYPE_E       proptype,
                           uint16                   max_len,
                           void                    *p_data,
                           MMIPB_CONTACT_T *contact_ptr)
{
    BOOLEAN       is_ready = FALSE;
    MMI_STRING_T *p_datastr = NULL;
    uint16        *p_len = NULL;
    wchar        *p_text = NULL;
    
    if((PNULL == p_data) || (PNULL == contact_ptr))
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_setdatatopb p_data 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2512_112_2_18_3_4_41_71,(uint8*)"d", p_data);
        return;
    }
    switch(proptype)
    {
      case VF_PROPERTY_N:
        //p_isucs2 = &entry_ptr->entry_name.is_ucs2;
        p_len    = &contact_ptr->name.wstr_len;
        p_text   = contact_ptr->name.wstr;
        is_ready = TRUE;
        break;
      case VF_PROPERTY_EMAIL:
#ifdef MMIPB_MAIL_SUPPORT
        //p_isucs2 = &entry_ptr->mail.is_ucs2;
        p_len    = &contact_ptr->mail.wstr_len;
        p_text   = contact_ptr->mail.wstr;
        is_ready = TRUE;
#endif
        break;
#ifndef PB_SUPPORT_LOW_MEMORY  
#if 0		
      case VF_PROPERTY_TITLE:
        //p_isucs2 = &entry_ptr->title.is_ucs2;
        p_len    = &contact_ptr->title.wstr_len;
        p_text   = contact_ptr->title.wstr;
        is_ready = TRUE;
        break;

      case VF_PROPERTY_ORG:
        //p_isucs2 = &entry_ptr->org.is_ucs2;
        p_len    = &contact_ptr->company.wstr_len;
        p_text   = contact_ptr->company.wstr;
        is_ready = TRUE;
        break;
      case VF_PROPERTY_ADR:
        //p_isucs2 = &entry_ptr->work_adr.is_ucs2;
        p_len    = &contact_ptr->address.wstr_len;
        p_text   = contact_ptr->address.wstr;
        is_ready = TRUE;
        break;
#endif
   case VF_PROPERTY_NOTE:
	    //entry_ptr->custom_text.is_valid = FALSE;
	    //p_isvalid = &entry_ptr->custom_text.is_valid;
        //p_isucs2 = &entry_ptr->custom_text.is_ucs2;
        p_len    = &contact_ptr->memo.wstr_len;
        p_text   = contact_ptr->memo.wstr;
        is_ready = TRUE;              
       break;

   case VF_PROPERTY_BDAY:
        p_len = &contact_ptr->birth.wstr_len;
        p_text = contact_ptr->birth.wstr;
        is_ready = TRUE;
        break;
   case VF_PROPERTY_URL:
        p_len = &contact_ptr->url.wstr_len;
        p_text = contact_ptr->url.wstr;
        is_ready = TRUE;
        break;
#endif
      default:
        return;
    }
    //Set data
    p_datastr = (MMI_STRING_T *)p_data;
    if (p_len != NULL && p_text != PNULL && p_datastr->wstr_len > 0)    /*lint !e774 */
    {
        //*p_isucs2 = p_datastr->is_ucs2;
        if (max_len < p_datastr->wstr_len)
        {
            *p_len = max_len;
        }
        else
        {
            *p_len = p_datastr->wstr_len;
        }
        
        MMIAPICOM_Wstrncpy(p_text, p_datastr->wstr_ptr, *p_len);
    }
    return;
}
void MMIVC_GetTelePhoneNumberEx(VF_OBJECT_T               *p_Object,
                        const char                *p_PropName,
                        const char               **pp_Array,
                        MMIPB_CONTACT_T *contact_ptr,
                        MMIPB_BCD_NUMBER_T *number_array,
                        uint8             *num_type_array,
                        uint8 num_array_len
                        )
{
    VF_PROP_T    *pProp;
    char         *pTel;
    //VOBJECT_T * object=(VOBJECT_T *)p_Object;
    int32 i=0;
    int32 n=0;
    char **x_tel = PNULL;
    uint8 x_tel_max = 0;
    int32 x_tel_num=0;
    int32      cur_index = 0;

    if((PNULL == p_Object) || (PNULL == p_PropName) ||(PNULL == pp_Array) ||(PNULL == contact_ptr))
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_gettelephonenum p_Object 0x%x p_PropName 0x%x pp_Array 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2599_112_2_18_3_4_41_72,(uint8*)"ddd", p_Object, p_PropName, pp_Array);
        return;
    }

    x_tel_max = MAX(num_array_len,VCARD_MAX_NV_PHONE_NUM);
    x_tel = SCI_ALLOC_APPZ(x_tel_max*sizeof(char *));

    if(PNULL == x_tel)
    {
        SCI_TRACE_LOW("MMIVC_GetTelePhoneNumberEx x_tel alloc failed!!!");
        return;
    }
    
    if (vf_get_property(&pProp, p_Object, VFGP_FIND, NULL, p_PropName, NULL))
    {
        do
        {
            VPROP_T *p_vprop = (VPROP_T *)pProp;
            BOOLEAN is_set=FALSE;
            pTel = vf_get_prop_value_string(pProp, 0);
            if (pTel)
            {
                for(i=0;(pp_Array[i]!=PNULL);i++)
                {
                    for (n = 0;n < p_vprop->name.n_strings;n++)/*lint !e574 !e737*/
                    {
                        if(p_stristr(p_vprop->name.pp_strings[n],pp_Array[i]))
                        {
                            if(cur_index<VCARD_MAX_NV_PHONE_NUM)
                            {
                                contact_ptr->num_type[cur_index] = s_num_TelephoneTypes[i];
                                vc_setteltopb(pTel, &contact_ptr->number[cur_index]);
                                cur_index++;
                                is_set=TRUE;
                            }
                            break;
                        }
                    }
                }
                if((!is_set) && (x_tel_max > x_tel_num))  //bug 75026
                {
                    x_tel[x_tel_num]=pTel;
                    x_tel_num++;
                }
            }
        }
        while (vf_get_next_property(&pProp));

        for(i=0;i<x_tel_num;i++)
        {
            if(cur_index<VCARD_MAX_NV_PHONE_NUM)
            {
                contact_ptr->num_type[cur_index] = MMIPB_UNKNOWN_NUMBER;
				vc_setteltopb(x_tel[i], &contact_ptr->number[cur_index]);
                cur_index++;
            }
        }

        if(0 != num_array_len)
        {
            SCI_MEMCPY(number_array,contact_ptr->number,MIN(num_array_len,VCARD_MAX_NV_PHONE_NUM)*sizeof(MMIPB_BCD_NUMBER_T));
            SCI_MEMCPY(num_type_array,contact_ptr->num_type,MIN(num_array_len,VCARD_MAX_NV_PHONE_NUM)*sizeof(uint8));

            cur_index = MIN(num_array_len,VCARD_MAX_NV_PHONE_NUM);
            
            for(i=0;i<x_tel_num;i++)
            {
                if(cur_index<num_array_len)
                {
                    num_type_array[cur_index] = MMIPB_UNKNOWN_NUMBER;
    				vc_setteltopb(x_tel[i], &number_array[cur_index]);
                    cur_index++;
                }
            }
        }
    }
    
    SCI_FREE(x_tel);
    return;
}

/****************************************************************************/
//  Description:MMIVC_GetCallDatetimeEx
//  Global resource dependence
//  Auther: Cheney Wu
//  Note
/****************************************************************************/
void MMIVC_GetCallDatetimeEx(VF_OBJECT_T            *p_Object,
                             const char             *p_PropName,
                             const char             **pp_Array,
                             MMICL_CALL_INFO_T      *calllog_ptr
                             )
{
    VF_PROP_T       *pProp;
    char            *pTimeStamp;
    int32           i=0;
    int32           j=0;
    int32           n=0;
    uint8           timestamp_buff[10] = {0};
    uint8           tm_offset[VF_TIMESTAMP_MAX_FIELD] = {
                                        VF_TIMESTAMP_YEAR_OFFSET, 
                                        VF_TIMESTAMP_MON_OFFSET, 
                                        VF_TIMESTAMP_DAY_OFFSET, 
                                        VF_TIMESTAMP_T_OFFSET, 
                                        VF_TIMESTAMP_HOUR_OFFSET, 
                                        VF_TIMESTAMP_MIN_OFFSET, 
                                        VF_TIMESTAMP_SEC_OFFSET
                                    };
    uint8           tm_len[VF_TIMESTAMP_MAX_FIELD] = {
                                        VF_TIMESTAMP_MON_OFFSET  - VF_TIMESTAMP_YEAR_OFFSET,/*lint !e656*/
                                        VF_TIMESTAMP_DAY_OFFSET  - VF_TIMESTAMP_MON_OFFSET,/*lint !e656*/
                                        VF_TIMESTAMP_T_OFFSET    - VF_TIMESTAMP_DAY_OFFSET,/*lint !e656*/
                                        VF_TIMESTAMP_HOUR_OFFSET - VF_TIMESTAMP_T_OFFSET,/*lint !e656*/
                                        VF_TIMESTAMP_MIN_OFFSET  - VF_TIMESTAMP_HOUR_OFFSET,/*lint !e656*/
                                        VF_TIMESTAMP_SEC_OFFSET  - VF_TIMESTAMP_MIN_OFFSET,/*lint !e656*/
                                        VF_TIMESTAMP_MAX_OFFSET  - VF_TIMESTAMP_SEC_OFFSET};/*lint !e656*/
    uint16          tm_value[VF_TIMESTAMP_MAX_FIELD]   = {0};

    if((PNULL == p_Object) || (PNULL == p_PropName) ||(PNULL == calllog_ptr))
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_gettelephonenum p_Object 0x%x p_PropName 0x%x pp_Array 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2599_112_2_18_3_4_41_72,(uint8*)"ddd", p_Object, p_PropName, pp_Array);
        return;
    }

    if (vf_get_property(&pProp, p_Object, VFGP_FIND, NULL, p_PropName, NULL))
    {
        do
        {
            VPROP_T *p_vprop = (VPROP_T *)pProp;
            pTimeStamp = vf_get_prop_value_string(pProp, 0);
            if (pTimeStamp)
            {
                for(i=0;(pp_Array[i]!=PNULL);i++)
                {
                    for (n = 0;n < p_vprop->name.n_strings;n++)/*lint !e574 !e737*/
                    {
                        if(p_stristr(p_vprop->name.pp_strings[n],pp_Array[i]))
                        {
                            calllog_ptr->call_type = (CC_CALL_MODE_E)s_num_LogDateTimeTypes[i];
                            for(j=0; j<VF_TIMESTAMP_MAX_FIELD; j++)
                            {
                                memset(timestamp_buff, 0x00, sizeof(timestamp_buff));
                                memcpy(timestamp_buff, &pTimeStamp[tm_offset[j]], tm_len[j]);
                                tm_value[j] = (uint16)atoi((char*)timestamp_buff);
                            }
                            
                            calllog_ptr->call_time.start_time = MMIAPICOM_Tm2Second(tm_value[VF_TIMESTAMP_SEC_FIELD], tm_value[VF_TIMESTAMP_MIN_FIELD], tm_value[VF_TIMESTAMP_HOUR_FIELD], tm_value[VF_TIMESTAMP_DAY_FIELD], tm_value[VF_TIMESTAMP_MON_FIELD], tm_value[VF_TIMESTAMP_YEAR_FIELD]);
                            calllog_ptr->call_time.stop_time  = calllog_ptr->call_time.start_time;
                            break;
                        }
                    }
                }
            }
        }
        while (vf_get_next_property(&pProp));
    }
    else
    {
         calllog_ptr->call_type = (CC_CALL_MODE_E)MMICL_GetLogListType();//fix bug928829,按照6531E方案修改
         SCI_TraceLow("MMIVC_GetCallDatetimeEx there is no call type");
    }
    return;
}

void MMIVC_GetTelePhoneNumber(VF_OBJECT_T               *p_Object,
                        const char                *p_PropName,
                        const char               **pp_Array,
                        MMIPB_CONTACT_T *contact_ptr)
{
    MMIVC_GetTelePhoneNumberEx(p_Object, 
                                                        p_PropName, 
                                                        pp_Array, 
                                                        contact_ptr,
                                                        PNULL,
                                                        PNULL,
                                                        0
                                                        );
}

LOCAL void vc_setteltopb(char                *p_telstr, 
                         MMIPB_BCD_NUMBER_T  *p_telnum)
{
    int16                tel_len = 0;
    char                *tel_ptr = PNULL;
    MMI_PARTY_NUMBER_T   party_num;

    if((PNULL == p_telstr) || (PNULL == p_telnum) )
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_setteltopb p_telstr 0x%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2718_112_2_18_3_4_41_73,(uint8*)"d", p_telstr);
        return;
    }
    SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));
    tel_len = strlen(p_telstr);
    tel_ptr = (char *)SCI_ALLOC_APP(tel_len + 1);
    SCI_MEMSET(tel_ptr, 0, (tel_len + 1));
    vc_cleanuptelnum(p_telstr, tel_ptr);
    
    MMIAPIPB_StringToBCDNumber(tel_ptr, p_telnum);/*lint !e64*/
    SCI_FREE(tel_ptr);
    return;
}

LOCAL void vc_cleanuptelnum(char  *p_org_num,
                            char  *p_dst_num)
{    
    uint16  length = 0;
    uint16  i = 0,j = 0;

    if((PNULL == p_org_num) || (PNULL == p_dst_num) )
    {
        //SCI_TRACE_LOW:"[MMIVCARD] vc_cleanuptelnum p_org_num 0x%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2740_112_2_18_3_4_41_74,(uint8*)"d", p_org_num);
        return;
    }
    length = strlen(p_org_num);
    if('+' == p_org_num[0])
    {
        p_dst_num[0] = '+';
        j = 1;
    }
    for(i = j;i < length;i++)
    {
        switch( p_org_num[i] )
        {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
          case '+':			  
          case '*':
          case '#':
          case 'P':
          case 'p':
          case 'W':
          case 'w':
            p_dst_num[j] = p_org_num[i];
            break;
          case 'x':
          case 'X':
            p_dst_num[j] = 'p';
            break;
          default:
            continue;
            //break;
        }
        j ++;
    }
    return;
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note:add the parse property Birth,Url of the vCard
/****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCARD_VcardDataToContactEx(                                    
                                           uint8 *data_ptr,//[IN]
                                           uint32  data_len,//[IN]
                                           MMIPB_CONTACT_T *contact_ptr,
                                           MMIPB_CUSTOM_PHOTO_T *photo_ptr,
                                            MMIPB_BCD_NUMBER_T *number_array,
                                            uint8             *num_type_array,
                                            uint8 num_array_len
                                            )
{
    VCARD_ERROR_E   result = VCARD_FILE_ERROR;
    VF_PROP_T       *pProp = PNULL;
    const char      *p_PropName = NULL;
    vf_encoding_t    enc = 0;
    void            *pValue = PNULL;
    uint32_t         nValue = 0 ;
    MMI_STRING_T     tmp_str = {NULL};
    uint16           max_len = 0;
    VF_OBJECT_T      *p_object = NULL;
    VOBJECT_T * object=PNULL;
    BOOLEAN is_set_fn=FALSE;
#ifdef MMI_CONTACT_PHOTO_SUPPORT
    char         *pBase64Prop = PNULL;
    MMIFILE_DEVICE_E    file_dev = MMI_DEVICE_SDCARD;
    const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    static wchar        *photo_file = PNULL;
    wchar               *photo_file_extension = L".jpg";
    uint16              photofilelen = 0;
    uint16              full_path_len = 256;
    wchar               full_path[256 +1] = {0};
    uint32              bytes_written=0;
    MMIFILE_HANDLE      file_handle = 0;	
#endif

    if((PNULL == data_ptr) || 0 == data_len || (PNULL == contact_ptr) )
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_AddObjectToPB pObject 0x%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2833_112_2_18_3_4_41_75,(uint8*)"d", data_len);
        return VCARD_FILE_ERROR;
    }
    //开始解析
    if (MMIAPIVCARD_ParseByData(&p_object, data_ptr, data_len) && 
        PNULL != p_object)
    {
        object=(VOBJECT_T *)p_object;
        if (!MMIAPIVCARD_IsEmpty(p_object))
        {            
            MMIVC_GetTelePhoneNumberEx(p_object, VFP_TELEPHONE, s_TelephoneTypes, contact_ptr,number_array,num_type_array,num_array_len);
            
            if (vf_get_property(&pProp, p_object, VFGP_FIND, NULL, "*", NULL))
            {               
                do
                {
                    p_PropName = vf_get_prop_name_string(pProp, 0);
                    vf_get_prop_value(pProp, &pValue, &nValue, &enc);
                    switch(vc_getproptype(p_PropName,s_vf_fields))
                    {
                    case VF_PROPERTY_N:
                        if(is_set_fn)
                        {
                            break;
                        }
                        MMIVC_GetNamePropVaule(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_N, MMIPB_NAME_MAX_LEN,
                                       (void *)&tmp_str, contact_ptr);
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                        if(PNULL != photo_file)
                            SCI_FREE(photo_file);
                        photofilelen = (MMIAPICOM_Wstrlen(tmp_str.wstr_ptr)+MMIAPICOM_Wstrlen(photo_file_extension)+1) * sizeof(wchar);
                        photo_file = SCI_ALLOCAZ(photofilelen);
                        SCI_MEMSET(photo_file,0,photofilelen);
                        MMIAPICOM_Wstrncpy(photo_file, tmp_str.wstr_ptr, tmp_str.wstr_len);
                        MMIAPICOM_Wstrcat(photo_file, photo_file_extension);
#endif
                        MMIVC_freeData(&tmp_str);
                        break;
                    case VF_PROPERTY_FN:
                        is_set_fn=TRUE;
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_N, MMIPB_NAME_MAX_LEN,
                                       (void *)&tmp_str, contact_ptr);
                        MMIVC_freeData(&tmp_str);
                        break;

                        
                    case VF_PROPERTY_EMAIL:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_EMAIL, MMIPB_MAX_MAIL_LEN,
                            (void *)&tmp_str, contact_ptr);
                        
                        MMIVC_freeData(&tmp_str);
                        break;
                    case VF_PROPERTY_NOTE:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        if(MMIAPICOM_IsASCIIString(tmp_str.wstr_ptr, tmp_str.wstr_len))
                        {
                            max_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
                        }
                        else
                        {
                            max_len = MMIPB_MAX_CUSTOM_TEXT_LEN >>1;
                        }
                        vc_setdatatopb(VF_PROPERTY_NOTE, max_len,
                            (void *)&tmp_str, contact_ptr);
                        MMIVC_freeData(&tmp_str);               
                        break;				
	
                    case VF_PROPERTY_BDAY:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_BDAY, MMIPB_SNS_BIRTH_LEN, 
                            (void *)&tmp_str, contact_ptr);
                        MMIVC_freeData(&tmp_str);
                        break;
                    case VF_PROPERTY_URL:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_URL, MMIPB_URL_MAX_LEN, 
                            (void *)&tmp_str, contact_ptr);
                        MMIVC_freeData(&tmp_str);
                        break;
                    case VF_PROPERTY_PHOTO:
                    {
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                        uint16 path_len = 0;
                        if(PNULL == photo_ptr)
                            break;
                        pBase64Prop = ((VPROP_T *)pProp)->value.v.b.p_buffer;

                        file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev);
                        file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);

                        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
                        photo_file, (uint16)MMIAPICOM_Wstrlen(photo_file), full_path, &full_path_len);
                            
                        file_handle = MMIAPIFMM_CreateFile((const wchar*)full_path, SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
                        if( SFS_INVALID_HANDLE != file_handle )
                        {				
                            if (SFS_NO_ERROR == MMIAPIFMM_WriteFile(file_handle, pBase64Prop, nValue, &bytes_written, PNULL))
                            {
                                MMIAPIFMM_CloseFile(file_handle);
                                path_len = MIN(full_path_len, MMIFILE_FULL_PATH_MAX_LEN);
                                contact_ptr->photo_id_flag = (contact_ptr->photo_id_flag&MMIPB_PHOTO_ID_CLEAN)+MMIPB_PHOTO_ID_EXIST;
                                SCI_MEMSET(photo_ptr->file_name,0,sizeof(photo_ptr->file_name));
                                photo_ptr->is_valid = TRUE;
                                photo_ptr->file_name_len = path_len;
                                MMIAPICOM_Wstrncpy(photo_ptr->file_name,full_path,path_len);
                            }
                        }
#endif
                        break;
                    }
                    default:
                        break;
                    }
                }while (vf_get_next_property(&pProp));

                result = VCARD_NO_ERROR;
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                SCI_FREE(photo_file);
#endif
            }            
        }
        vf_delete_object(p_object, TRUE);
    }
    else
    {
        result = VCARD_FORMAT_ERROR;
    }

    return result;
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther: Bill.Ji
//  Ex by cheney.wu, for the PBAP parser
//  Note
/****************************************************************************/

PUBLIC VCARD_ERROR_E MMIAPIVCARD_VcardDataToCalllogEx(                                    
                                                      uint8 *data_ptr,//[IN]
                                                      uint32  data_len,//[IN]
                                                      MMICL_CALL_INFO_T *calllog_ptr
                                                      )
{
    VCARD_ERROR_E    result = VCARD_FILE_ERROR;
    VF_OBJECT_T      *p_object = NULL;
    VOBJECT_T        *object=PNULL;
    MMIPB_CONTACT_T  contact;

    if((PNULL == data_ptr) || 0 == data_len || (PNULL == calllog_ptr) )
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_AddObjectToPB pObject 0x%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2833_112_2_18_3_4_41_75,(uint8*)"d", data_len);
        return VCARD_FILE_ERROR;
    }
    //开始解析
    if (MMIAPIVCARD_ParseByData(&p_object, data_ptr, data_len) && 
        PNULL != p_object)
    {
        object=(VOBJECT_T *)p_object;
        if (!MMIAPIVCARD_IsEmpty(p_object))
        {            
            MMIVC_GetTelePhoneNumberEx(p_object, VFP_TELEPHONE, s_TelephoneTypes, &contact, NULL, NULL, 0);
            MMIVC_GetCallDatetimeEx(p_object, VFP_TIMESTAMP, s_LogDateTimeTypes, calllog_ptr);

            calllog_ptr->call_num.is_num_present = TRUE;

            calllog_ptr->call_num.number.num_len = contact.number->number_len;
            calllog_ptr->call_num.number.number_type = MMIPB_GetNumberTypeFromUint8(contact.number->npi_ton);
            memcpy(calllog_ptr->call_num.number.party_num, contact.number->number, MMIPB_BCD_NUMBER_MAX_LEN);
        }
        result = VCARD_NO_ERROR;
        vf_delete_object(p_object, TRUE);
    }
    else
    {
        result = VCARD_FORMAT_ERROR;
    }

    return result;
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note:
/****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCARD_VcardDataToContact(                                    
                                           uint8 *data_ptr,//[IN]
                                           uint32  data_len,//[IN]
                                           MMIPB_CONTACT_T *contact_ptr
                                            )
{
    return MMIAPIVCARD_VcardDataToContactEx(data_ptr, 
                                                                data_len, 
                                                                contact_ptr,
                                                                PNULL,
                                                                PNULL,
                                                                PNULL,
                                                                0);
}
/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note:add the parse property Birth,Url of the vCard
/****************************************************************************/
//no use
PUBLIC VCARD_ERROR_E MMIAPIVCARD_VcardFileToContact(                                    
                                                    wchar * file_name,//[IN]
                                                    uint32  file_name_len,//[IN]
                                                    MMIPB_CONTACT_T *contact_ptr,//[OUT]
                                                    MMIPB_CUSTOM_PHOTO_T *photo_ptr//[OUT]
                                                    )
{
    VCARD_ERROR_E   result = VCARD_FILE_ERROR;
    VF_PROP_T       *pProp = PNULL;
    const char      *p_PropName = NULL;
    vf_encoding_t    enc = 0;
    void            *pValue = PNULL;
    uint32_t         nValue = 0 ;
    MMI_STRING_T     tmp_str = {NULL};
    uint16           max_len = 0;
    VF_OBJECT_T      *p_object = NULL;
    VOBJECT_T * object=PNULL;
#ifdef MMI_CONTACT_PHOTO_SUPPORT
    char         *pBase64Prop = PNULL;
    MMIFILE_DEVICE_E    file_dev = MMI_DEVICE_SDCARD;
    const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    static wchar        *photo_file = PNULL;
    wchar               *photo_file_extension = L".jpg";
    uint16              photofilelen = 0;
    uint16              full_path_len = 256;
    wchar               full_path[256 +1] = {0};
    uint32              bytes_written=0;
    MMIFILE_HANDLE      file_handle = 0;	
#endif

    if((PNULL == file_name) || 0 == file_name_len || (PNULL == contact_ptr) )
    {
        //SCI_TRACE_LOW:"[MMIVCARD] MMIAPIVCARD_AddObjectToPB pObject 0x%x "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIVCARD_2833_112_2_18_3_4_41_75,(uint8*)"d", file_name_len);
        return VCARD_FILE_ERROR;
    }
    //开始解析
    if (VCARD_NO_ERROR == VCARD_ReadFile(&p_object, file_name) && 
        PNULL != p_object)
    {
        object=(VOBJECT_T *)p_object;
        if (!MMIAPIVCARD_IsEmpty(p_object))
        {            
            MMIVC_GetTelePhoneNumber(p_object, VFP_TELEPHONE, s_TelephoneTypes, contact_ptr);
            
            if (vf_get_property(&pProp, p_object, VFGP_FIND, NULL, "*", NULL))
            {               
                do
                {
                    p_PropName = vf_get_prop_name_string(pProp, 0);
                    vf_get_prop_value(pProp, &pValue, &nValue, &enc);
                    switch(vc_getproptype(p_PropName,s_vf_fields))
                    {
                    case VF_PROPERTY_N:
                        if(object->version==VCARD_VERSION_30)
                        {
                            break;
                        }
                        MMIVC_GetNamePropVaule(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_N, MMIPB_NAME_MAX_LEN,
                                       (void *)&tmp_str, contact_ptr);
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                        if(PNULL != photo_file)
                            SCI_FREE(photo_file);
                        photofilelen = (MMIAPICOM_Wstrlen(tmp_str.wstr_ptr)+MMIAPICOM_Wstrlen(photo_file_extension)+1) * sizeof(wchar);
                        photo_file = SCI_ALLOCAZ(photofilelen);
                        SCI_MEMSET(photo_file,0,photofilelen);
                        MMIAPICOM_Wstrncpy(photo_file, tmp_str.wstr_ptr, tmp_str.wstr_len);
#endif
                        MMIVC_freeData(&tmp_str);
                        break;
                    case VF_PROPERTY_FN:
                        if(object->version==VCARD_VERSION_21)
                        {
                            break;
                        }
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_N, MMIPB_NAME_MAX_LEN,
                                       (void *)&tmp_str, contact_ptr);
                        MMIVC_freeData(&tmp_str);
                        break;
                        
                    case VF_PROPERTY_EMAIL:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_EMAIL, MMIPB_MAX_MAIL_LEN,
                            (void *)&tmp_str, contact_ptr);
                        
                        MMIVC_freeData(&tmp_str);
                        break;
						
                    case VF_PROPERTY_PHOTO:
                    {
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                        uint16 path_len = 0;
                        if(PNULL == photo_ptr)
                            break;
                        pBase64Prop = ((VPROP_T *)pProp)->value.v.b.p_buffer;

                        file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev);
                        file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
                        MMIAPICOM_Wstrcat(photo_file, photo_file_extension);
                        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
                        photo_file, (uint16)MMIAPICOM_Wstrlen(photo_file), full_path, &full_path_len);
                            
                        file_handle = MMIAPIFMM_CreateFile((const wchar*)full_path, SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
                        if( SFS_INVALID_HANDLE != file_handle )
                        {				
                            if (SFS_NO_ERROR == MMIAPIFMM_WriteFile(file_handle, pBase64Prop, nValue, &bytes_written, PNULL))
                            {
                                MMIAPIFMM_CloseFile(file_handle);
                                path_len = MIN(full_path_len, MMIFILE_FULL_PATH_MAX_LEN);
                                contact_ptr->photo_id_flag = (contact_ptr->photo_id_flag&MMIPB_PHOTO_ID_CLEAN)+MMIPB_PHOTO_ID_EXIST;
                                SCI_MEMSET(photo_ptr->file_name,0,sizeof(photo_ptr->file_name));
                                photo_ptr->is_valid = TRUE;
                                photo_ptr->file_name_len = path_len;
                                MMIAPICOM_Wstrncpy(photo_ptr->file_name,full_path,path_len);
                            }
                        }
#endif
                        break;
                    }
                    case VF_PROPERTY_TIMESTAMP:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_TIMESTAMP, MMIPB_MAX_MAIL_LEN,
                            (void *)&tmp_str, contact_ptr);
                        
                        MMIVC_freeData(&tmp_str);
                        break;

                    case VF_PROPERTY_NOTE:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        if(MMIAPICOM_IsASCIIString(tmp_str.wstr_ptr, tmp_str.wstr_len))
                        {
                            max_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
                        }
                        else
                        {
                            max_len = MMIPB_MAX_CUSTOM_TEXT_LEN >>1;
                        }
                        vc_setdatatopb(VF_PROPERTY_NOTE, max_len,
                            (void *)&tmp_str, contact_ptr);
                        MMIVC_freeData(&tmp_str);               
                        break;				
                    
                    case VF_PROPERTY_BDAY:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_BDAY, MMIPB_SNS_BIRTH_LEN,
                            (void *)&tmp_str, contact_ptr);                      
                        MMIVC_freeData(&tmp_str);
                        break;                
                    case VF_PROPERTY_URL:
                        MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                        vc_setdatatopb(VF_PROPERTY_URL, MMIPB_URL_MAX_LEN,
                            (void *)&tmp_str, contact_ptr);                
                        MMIVC_freeData(&tmp_str);
                        break;
                    default:
                        break;
                    }
                }while (vf_get_next_property(&pProp));

                result = VCARD_NO_ERROR;
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                SCI_FREE(photo_file);
#endif
            }            
        }
        vf_delete_object(p_object, TRUE);
    }
    else
    {
        result = VCARD_FORMAT_ERROR;
    }

    return result;
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_ContactToVcard(
                                     MMIPB_CONTACT_T *contact_ptr,  //[IN]  
                                     uint32          data_max_size,//[IN]
                                     uint8 *data_ptr,//[OUT]
                                     uint32 *data_len_ptr//[OUT]
                                    )
{
    BOOLEAN result = FALSE;	

    result = MMIAPIVCARD_BuildVCardToBuffer(contact_ptr, data_ptr, data_max_size);
    if(result && data_len_ptr != PNULL && data_ptr != PNULL)
    {
         *data_len_ptr = strlen((char *)data_ptr);
    }    
    
    return result;      
}

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_IsEmpty(VF_OBJECT_T    *pObject)
{
    BOOLEAN       bRet = TRUE;
    VF_PROP_T    *pProp;

    if(PNULL == pObject)
	{
		return bRet;
	}
    if (vf_get_property(&pProp, pObject, VFGP_FIND, NULL, "*", NULL))
    {
        bRet = FALSE;
    }
    return bRet;
}
/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note:add the parse property Birth,Url of the vCard
/****************************************************************************/
LOCAL VCARD_ERROR_E vc_ObjectToContact(VF_OBJECT_T  *p_object,MMIPB_CONTACT_T *contact_ptr, MMIPB_CUSTOM_PHOTO_T *photo_ptr)
{
    VCARD_ERROR_E   result = VCARD_FILE_ERROR;
    VF_PROP_T       *pProp = PNULL;
    const char      *p_PropName = NULL;
    vf_encoding_t    enc = 0;
    void            *pValue = PNULL;
    uint32_t         nValue = 0 ;
    MMI_STRING_T     tmp_str = {NULL};
    uint16           max_len = 0;
    VOBJECT_T * object=(VOBJECT_T *)p_object;
#ifdef MMI_CONTACT_PHOTO_SUPPORT
    char         *pBase64Prop = PNULL;
    MMIFILE_DEVICE_E    file_dev = MMI_DEVICE_SDCARD;
    const wchar*        file_dev_ptr = PNULL;
    uint16              file_dev_len = 0;
    static wchar        *photo_file = PNULL;
    wchar               *photo_file_extension = L".jpg";
    uint16              photofilelen = 0;
    uint16              full_path_len = 256;
    wchar               full_path[256 +1] = {0};
    uint32              bytes_written=0;
    MMIFILE_HANDLE      file_handle = 0;	
#endif

    if (!MMIAPIVCARD_IsEmpty(p_object))
    {            
        MMIVC_GetTelePhoneNumber(p_object, VFP_TELEPHONE, s_TelephoneTypes, contact_ptr);
        
        if (vf_get_property(&pProp, p_object, VFGP_FIND, NULL, "*", NULL))
        {               
            do
            {
                p_PropName = vf_get_prop_name_string(pProp, 0);
                vf_get_prop_value(pProp, &pValue, &nValue, &enc);
                switch(vc_getproptype(p_PropName,s_vf_fields))
                {
                case VF_PROPERTY_N:
                    if(object->version==VCARD_VERSION_30)
                    {
                        break;
                    }
                    MMIVC_GetNamePropVaule(pProp, nValue, &tmp_str);
                    vc_setdatatopb(VF_PROPERTY_N, MMIPB_NAME_MAX_LEN,
                                   (void *)&tmp_str, contact_ptr);
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                        if(PNULL != photo_file)
                            SCI_FREE(photo_file);
                        photofilelen = (MMIAPICOM_Wstrlen(tmp_str.wstr_ptr)+MMIAPICOM_Wstrlen(photo_file_extension)+1) * sizeof(wchar);
                        photo_file = SCI_ALLOCAZ(photofilelen);
                        SCI_MEMSET(photo_file,0,photofilelen);
                        MMIAPICOM_Wstrncpy(photo_file, tmp_str.wstr_ptr, tmp_str.wstr_len);
#endif
                    MMIVC_freeData(&tmp_str);
                    break;
                case VF_PROPERTY_FN:
                    if(object->version==VCARD_VERSION_21)
                    {
                        break;
                    }
                    MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                    vc_setdatatopb(VF_PROPERTY_N, MMIPB_NAME_MAX_LEN,
                                   (void *)&tmp_str, contact_ptr);
                    MMIVC_freeData(&tmp_str);
                    break;
                    
                case VF_PROPERTY_EMAIL:
                    MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                    vc_setdatatopb(VF_PROPERTY_EMAIL, MMIPB_MAX_MAIL_LEN,
                        (void *)&tmp_str, contact_ptr);
                    
                    MMIVC_freeData(&tmp_str);
                    break;
			
                case VF_PROPERTY_PHOTO:
                    {
#ifdef MMI_CONTACT_PHOTO_SUPPORT
                        uint16 path_len = 0;
                        if(PNULL == photo_ptr)
                            break;
                        pBase64Prop = ((VPROP_T *)pProp)->value.v.b.p_buffer;

                        file_dev_ptr = MMIAPIFMM_GetDevicePath(file_dev);
                        file_dev_len = MMIAPIFMM_GetDevicePathLen(file_dev);
                        MMIAPICOM_Wstrcat(photo_file, photo_file_extension);
                        MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len, MMIMULTIM_DIR_PICTURE, MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
                        photo_file, (uint16)MMIAPICOM_Wstrlen(photo_file), full_path, &full_path_len);
                            
                        file_handle = MMIAPIFMM_CreateFile((const wchar*)full_path, SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
                        if( SFS_INVALID_HANDLE != file_handle )
                        {				
                            if (SFS_NO_ERROR == MMIAPIFMM_WriteFile(file_handle, pBase64Prop, nValue, &bytes_written, PNULL))
                            {
                                MMIAPIFMM_CloseFile(file_handle);
                                path_len = MIN(full_path_len, MMIFILE_FULL_PATH_MAX_LEN);
                                contact_ptr->photo_id_flag = (contact_ptr->photo_id_flag&MMIPB_PHOTO_ID_CLEAN)+MMIPB_PHOTO_ID_EXIST;
                                SCI_MEMSET(photo_ptr->file_name,0,sizeof(photo_ptr->file_name));
                                photo_ptr->is_valid = TRUE;
                                photo_ptr->file_name_len = path_len;
                                MMIAPICOM_Wstrncpy(photo_ptr->file_name,full_path,path_len);
                            }
                        }
#endif
                        break;
                    }
                case VF_PROPERTY_TIMESTAMP:
                    MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                    vc_setdatatopb(VF_PROPERTY_TIMESTAMP, MMIPB_MAX_MAIL_LEN,
                        (void *)&tmp_str, contact_ptr);
                    
                    MMIVC_freeData(&tmp_str);
                    break;
		
                case VF_PROPERTY_NOTE:
                    MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                    if(MMIAPICOM_IsASCIIString(tmp_str.wstr_ptr, tmp_str.wstr_len))
                    {
                        max_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
                    }
                    else
                    {
                        max_len = MMIPB_MAX_CUSTOM_TEXT_LEN >>1;
                    }
                    vc_setdatatopb(VF_PROPERTY_NOTE, max_len,
                        (void *)&tmp_str, contact_ptr);
                    MMIVC_freeData(&tmp_str);               
                    break;	 
					     
                case VF_PROPERTY_BDAY:
                    MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                    vc_setdatatopb(VF_PROPERTY_BDAY, MMIPB_SNS_BIRTH_LEN,
                        (void *)&tmp_str, contact_ptr);                   
                    MMIVC_freeData(&tmp_str);
                    break;
                case VF_PROPERTY_URL:
                    MMIVC_GetPropValue(pProp, nValue, &tmp_str);
                    vc_setdatatopb(VF_PROPERTY_URL, MMIPB_URL_MAX_LEN,
                        (void *)&tmp_str, contact_ptr);                   
                    MMIVC_freeData(&tmp_str);               
                    break;				
                default:
                    break;
                }
            }while (vf_get_next_property(&pProp));

            result = VCARD_NO_ERROR;
#ifdef MMI_CONTACT_PHOTO_SUPPORT
            SCI_FREE(photo_file);
#endif
        }            
    }
    else
    {
        result=VCARD_EMPTY_FILE;
    }
    vf_delete_object(p_object, TRUE);

    return result;
}
/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
//no relation with bt, not change or change the caller at the same time 
VCARD_ERROR_E MMIAPIVCARD_ReadContact(
                                            MMIFILE_HANDLE file_handle,//[IN]
                                            MMIPB_CONTACT_T *contact_ptr,//[OUT]
                                            MMIPB_CUSTOM_PHOTO_T *photo_ptr//[OUT]
                                           )
{
    VCARD_ERROR_E     ret = VCARD_FORMAT_ERROR;
	BOOLEAN           parse_ret = FALSE;
    char              *buffer=PNULL;
    uint32            charsread = 0;
    VF_PARSER_T      *p_parser;
    MMIFILE_ERROR_E   ffs_error;
    uint16            file_size = 0;
    char *vcard_end="END:VCARD\r\n";
    char *p=PNULL;
    uint32 ver=0;
    Vcard_parse_text_fn parse_fn=vf_parse_text;
    VF_OBJECT_T *p_object=PNULL;
    VF_OBJECT_T **pp_object=&p_object;
    uint32 read_byte=0;
    int32 file_offset=-1;
    
    if (file_handle)
    {
        file_size = MMIAPIFMM_GetFileSize(file_handle);
        if (0 == file_size)
        {
		    ret = VCARD_EMPTY_FILE;
            return ret;
        }
        file_offset=MMIAPIFMM_GetFilePointer(file_handle,MMIFILE_SEEK_BEGIN);
        if(file_offset==file_size)
        {
            ret = VCARD_FILE_TAIL;
            return ret;
        }
        read_byte=file_offset;
        buffer=(char*)SCI_ALLOCAZ(VCARD_READ_BUFFER_LEN+1);
        if(PNULL ==buffer)
        {
            return ret;
        }

        vf_set_mem_functions(MMIVC_Malloc, MMIVC_Realloc, MMIVC_Free);
        
        if (vf_parse_init(&p_parser, pp_object))
        {
            do
            {
                SCI_MEMSET(buffer, 0, VCARD_READ_BUFFER_LEN+1);
                ffs_error = MMIAPIFMM_ReadFile(file_handle,buffer,VCARD_READ_BUFFER_LEN,&charsread,PNULL);
                read_byte+=charsread;
                if(0==ver)
                {
                    ver=VcardFindVersion(buffer);
                    if(0==ver)
                    {
                        if(isspacelfstr(buffer))/*lint !e64*/
                        {
                            ret = VCARD_FILE_TAIL; 
                        }
                        else
                        {
                            ret = VCARD_FORMAT_ERROR; 
                        }
                        //SCI_FREE(buffer);
                        break; 
                    }
                    else
                    {
                        if(VCARD_VERSION_30==ver)
                        {
                            parse_fn=vf_parse_text_v30;
                        }
                    }
                }
                p=p_stristr(buffer,vcard_end);
                if(PNULL!=p)
                {
                    parse_ret = parse_fn(p_parser, buffer, (p-buffer)+SCI_STRLEN(vcard_end));/*lint !e737*/
                    if (parse_ret && vf_parse_end(p_parser))
                    {
                        ret = VCARD_NO_ERROR;
                        read_byte=read_byte-(charsread-(p-buffer)-SCI_STRLEN(vcard_end));/*lint !e737*/
                        MMIAPIFMM_SetFilePointer(file_handle, read_byte, MMIFILE_SEEK_BEGIN);

                        if(*pp_object!=PNULL)
                        {
                           VOBJECT_T * tmp=(VOBJECT_T *)(*pp_object);
                           tmp->version=ver;
                        }
                        ret=vc_ObjectToContact(*pp_object,contact_ptr, photo_ptr);
                        //SCI_FREE(buffer);
                        break;
                    }
                    else
                    {
                      //MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);
                       ret = VCARD_FORMAT_ERROR; 
                       //SCI_FREE(buffer);
                       break;
                    }
                }
                else
                {
                    if (0 < charsread && SFS_NO_ERROR == ffs_error)
                    {
                        parse_ret = parse_fn(p_parser, buffer, charsread);
                    }
                }
            }
            while (parse_ret && (0 < charsread));
        }
        SCI_FREE(buffer);
        if(VCARD_NO_ERROR != ret)
        {
            MMIAPIFMM_SetFilePointer(file_handle, 0, MMIFILE_SEEK_BEGIN);
            if(file_offset > 0)
            {
                //exist valid contacts in vcard file
                ret = VCARD_FILE_TAIL;
            }
        }
    }
    else
    {
        ret = VCARD_FILE_ERROR;
    }
    return ret;
}

/****************************************************************************/
//  Description:是否是只包含空格，回车换行的字符串
//  Global resource dependence
//  Auther:
//  Note
/****************************************************************************/
LOCAL BOOLEAN isspacelfstr(uint8 *buffer_ptr)
{
    BOOLEAN ret = TRUE;
    uint32 i  = 0;

    for(i = 0; buffer_ptr != PNULL && i < strlen(buffer_ptr); i++)/*lint !e64*/
    {
        if(buffer_ptr[i] != ' ' 
          ||buffer_ptr[i] != '\r'
          ||buffer_ptr[i] != '\n'
          )
        {
            ret = FALSE;
            break;
        }
    }
    return ret;
}
#endif //#if defined MMI_VCARD_SUPPORT



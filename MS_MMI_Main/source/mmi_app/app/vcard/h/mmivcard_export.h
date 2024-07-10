/*****************************************************************************
** File Name:      mmivcard_export.h                                                   *
** Author:                                                                   *
** Date:           18/05/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi vcard                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2006      Jenny Sun       Create
******************************************************************************/

#ifndef _MMIVCARD_H_
#define _MMIVCARD_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mn_type.h"
#include "vf_iface.h"
#include "mmipb_export.h"
#include "mmicl_export.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//#define VCARD_MAX_LEN 1000
#define VCARD_NAME_MAX_LEN         MMIFILE_FILE_NAME_MAX_LEN       //vCard文件名的最大长度
#define VCARD_REV_MAX_LEN          20       //vCard 文件中REV值的最大长度
#define VCARD_MAX_FILE_LEN         (5*1024) //所支持vCard文件的最大长度
#define VCARD_MIM_LEN             100 //Vcard数据的最小长度
#define VCARD_SIM_DEFAULT_PREFIX   "SIM_"   //vCard文件前缀名(SIM条目)
#define VCARD_NV_DEFAULT_PREFIX    "VCARD_"//"NV_"    //vCard文件前缀名(NV条目) maryxiao for cr108402
#define VCARD_DEFAULT_SUBFIX       ".vcf"   //vCard文件后缀名
#define VCARD_DEFAULT_SUBFIX_LEN   4        //vCard文件后缀名长度
//#define VCARD_DEFAULT_DIR          (g_mmivcard_default_dir_wstr)//"\x00\x76\x00\x43\x00\x61\x00\x72\x00\x64\x00\x00"
//#define VCARD_DEFAULT_DIR_LEN      5

#define VCARD_MIN_NEED_SPACE        2048    //保存Vcard最小需要空间

#define VCARD_VERSION_21            21
#define VCARD_VERSION_30            30
#define VCARD_MAX_NV_PHONE_NUM      MMIPB_MAX_NV_PHONE_NUM
#define VCARD_READ_BUFFER_LEN       1024
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum VF_PROPERTY_TYPE_E 
{
    VF_PROPERTY_INVALID,
    VF_PROPERTY_N,
    VF_PROPERTY_TEL,
    VF_PROPERTY_EMAIL,
    VF_PROPERTY_TITLE,
    VF_PROPERTY_ORG,
    VF_PROPERTY_ADR,
	VF_PROPERTY_NOTE,
	VF_PROPERTY_FN,
	VF_PROPERTY_BDAY,
	VF_PROPERTY_URL,
	VF_PROPERTY_PHOTO,
    VF_PROPERTY_TIMESTAMP,
    VF_PROPERTY_MAX
}VF_PROPERTY_TYPE_E;


typedef struct VF_FIELDINFO_T
{
    VF_PROPERTY_TYPE_E    vf_type;
    char                 *pszPropName;
}VF_FIELDINFO_T;

typedef enum VCARD_ERROR_E 
{
    VCARD_NO_ERROR,
    VCARD_EMPTY_FILE,
    VCARD_TOBIG_FILE,
    VCARD_FORMAT_ERROR,
    VCARD_FILE_ERROR,
    VCARD_PARAM_ERROR,
    VCARD_MULTI_ITEM,
    VCARD_SPACE_FULL,
    VCARD_FILE_TAIL,
}VCARD_ERROR_E;

typedef struct _VF_MULTI_ITEM_CONTEXT_
{
    uint32 read_byte;
    MMIFILE_HANDLE    file_handle;
    uint32 total_item_num;
}VF_MULTI_ITEM_CONTEXT;

typedef BOOLEAN (*Vcard_parse_text_fn)(VF_PARSER_T *p_parser,char *p_chars,int numchars);


//extern const wchar g_mmivcard_default_dir_wstr[];
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_IsEmpty(VF_OBJECT_T    *pObject);

/****************************************************************************/
//   Description: Build one vCard file following the protocol vCard 2.1
//   Global resource dependence
//   Auther: Jenny.Sun
//   Note
/****************************************************************************/
PUBLIC void MMIAPIVCARD_BuildVCard(MMIPB_CONTACT_T  *entry_ptr);

/****************************************************************************/
//   Description: Build one vCard file following the protocol vCard 2.1
//   Global resource dependence
//   Auther: 
//   Note
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_BuildVCardToBuffer(MMIPB_CONTACT_T *contact_ptr, 
									 uint8 *vcard_data_ptr, 
									 uint32 vcard_max_len
									 );

/****************************************************************************/
//   Description: Build one vCard file following the protocol vCard 3.0
//   Global resource dependence
//   Auther: 
//   Note
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_BuildVCard30ToBuffer(MMIPB_CONTACT_T  *entry_ptr, 
									 uint8 *vcard_data_ptr, 
									 uint32 vcard_max_len
									 );

/*****************************************************************************/
// Description : 创建vCard目录
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_InitFolder(const wchar   *device_ptr,
                                uint16   device_len,
                                const wchar   *dir_ptr,
                                uint16   dir_len);

/*****************************************************************************/
// Description : Init vcard global var
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVCARD_BuildInit(void);

/*****************************************************************************/
// Description : Get vcard data ptr
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC uint8 * MMIAPIVCARD_GetBuildData(void);

/**BEGIN*******add by liuxiaohua *********begin*****/

/****************************************************************************/
//   Description: Get Vcard File Name For PB
//   Global resource dependence
//   Auther: 
//   Note
/****************************************************************************/
PUBLIC void MMIAPIVCARD_GetVcardFileName( MMIPB_CONTACT_T  *contact_ptr,
                                    PHONEBOOK_STORAGE_E       entry_storage,
                                    wchar*name);

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
                                      );

/*****************************************************************************/
// Description : pb联系人转换成vcard
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_ContactToVcard(
                                     MMIPB_CONTACT_T *contact_ptr,  //[IN]  
                                     uint32          data_max_size,//[IN]
                                     uint8 *data_ptr,//[OUT]
                                     uint32 *data_len_ptr//[OUT]
                                    );

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCARD_VcardDataToContact(                                    
                                           uint8 *data_ptr,//[IN]
                                           uint32  data_len,//[IN]
                                           MMIPB_CONTACT_T *contact_ptr);
/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
// Ex by yao.chen,for the PBAP parser
//  Note
/****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCARD_VcardDataToContactEx(                                    
                                           uint8 *data_ptr,//[IN]
                                           uint32  data_len,//[IN]
                                           MMIPB_CONTACT_T *contact_ptr,
                                           MMIPB_CUSTOM_PHOTO_T *photo_ptr, //[OUT]
                                            MMIPB_BCD_NUMBER_T *number_array,
                                            uint8             *num_type_array,
                                            uint8 num_array_len);
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
                                                      );

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCARD_VcardFileToContact(                                    
                                                    wchar * file_name,//[IN]
                                                    uint32  file_name_len,//[IN]
                                                    MMIPB_CONTACT_T *contact_ptr,//[OUT]
                                                    MMIPB_CUSTOM_PHOTO_T *photo_ptr//[OUT]
                                                    );

/*****************************************************************************/
// Description : 保存文件
// Global resource dependence : 
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCard_SaveVCardByData(
                                wchar           *file_name,//[IN/OUT]: vcard file name to show
                                uint8            *data_ptr,
                                uint32            data_len);

/****************************************************************************/
//  Description: Read vCard file to global variable
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC VCARD_ERROR_E MMIAPIVCARD_GetDataByFile(uint16    *name_ptr);

/*****************************************************************************/
// Description: init vcard module
// Global resource dependence:
// Auther: Jenny.Sun
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVCARD_Init(void);

/*****************************************************************************/
// Description : Free vcard global var
// Global resource dependence :
// Author:Bill.Ji
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIVCARD_BuildFree(void);

/****************************************************************************/
//  Description: Parses a vCard data
//  Global resource dependence
//  Auther:Bill.Ji
//  Note
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIVCARD_ParseByData(VF_OBJECT_T **pp_object, uint8 *data_ptr, uint32 data_size);

/****************************************************************************/
//  Description:
//  Global resource dependence
//  Auther:renyi.hu
//  Note
/****************************************************************************/
VCARD_ERROR_E MMIAPIVCARD_ReadContact( 
                                      MMIFILE_HANDLE file_handle,//[IN]
                                      MMIPB_CONTACT_T *contact_ptr,//[OUT]
                                      MMIPB_CUSTOM_PHOTO_T *photo_ptr//[OUT]
                                      );
  
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define 	VCARD_IsEmpty	   		        MMIAPIVCARD_IsEmpty
#define 	VCARD_BuildVCard			        MMIAPIVCARD_BuildVCard
#define 	VCARD_BuildVCardToBuffer		MMIAPIVCARD_BuildVCard30ToBuffer
//#define 	VCARD_GetVcardFileName		MMIAPIVCARD_GetVcardFileName
//#define 	VCARD_GetExportVcardFileName		MMIAPIVCARD_GetExportVcardFileName
//#define 	VCARD_SaveVcardFileSync		MMIAPIVCARD_SaveVcardFileSync
#define 	VCARD_InitFolder			        MMIAPIVCARD_InitFolder
#define 	VCARD_BuildInit		                MMIAPIVCARD_BuildInit
#define 	VCARD_GetBuildData		        MMIAPIVCARD_GetBuildData
#define 	MMIVCard_SaveVCardByData	MMIAPIVCard_SaveVCardByData
#define 	VCARD_GetDataByFile		        MMIAPIVCARD_GetDataByFile
#define 	VCARD_parse_by_data		        MMIAPIVCARD_ParseByData
//#define 	VCARD_read_file_Ex		        MMIAPIVCARD_ReadFileEx
//#define 	VCARD_read_next_item		MMIAPIVCARD_ReadNextItem
//#define 	VCARD_get_multi_item_context		MMIAPIVCARD_GetMultiItemContext

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/****************************************************************************
** File Name:      cafmodule.h                                             *
** Author:         Xiaoqing.Lu                                            *
** Date:           05/26/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        Xiaoqing.Lu       Create
** 
****************************************************************************/

#ifndef  _MMK_MODULEINFO_H_    
#define  _MMK_MODULEINFO_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "cafmodule.h"
//#include "mmk_modinfo.h"
#include "mmi_link.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define CIF_FILE_NAME_ID        1000 // cif file name id
#define ARM_RES_FILE_NAME_ID    1001 // arm resource file name id
#define WIN_RES_FILE_NAME_ID    1002 // win resource file name id
#define ARM_BIN_FILE_NAME_ID    1003 // arm bin file name id
#define WIN_DLL_FILE_NAME_ID    1004 // window dll file name id
#define MOD_AUTHOR_NAME_ID      2000 // author name id
#define MOD_COPYRIGHT_STR_ID    2001 // copy right id
#define MOD_VER_STR_ID          2002 // version right id
#define MOD_APPLET_NAME_MIN_ID  3000 // applet name min id
#define MOD_APPLET_NAME_MAX_ID  3999 // applet name max id
#define MOD_OBJ_MIN_ID          4000 // object min id
#define MOD_OBJ_MAX_ID          4999 // object max id

#define MMK_SZIP_IDENT_MAX_LEN  0x10 // file zip

#define CAF_FILENAME_MAX_LEN    0x100 // file name length

#define MOD_CIF_VALID_HANDLE 0
#define MOD_CIF_FILE_MAX_SIZE (10*1024)
#define MMK_UNPACK_BUFFER_SIZE (5*1024)
//#define INT32_SIZE sizeof(int32)
#define MOD_CIF_DEVICE_NAME_D      (g_mod_cif_dev_name_d_wstr) // "D"
#define MOD_CIF_DEVICE_NAME_D_LEN  2
#define MOD_CIF_DEVICE_NAME_E      (g_mod_cif_dev_name_e_wstr)  // "E"
#define MOD_CIF_DEVICE_NAME_E_LEN  2
#define MOD_CIF_FOLDER             (g_mod_cif_folder_wstr)  // "Com"
#define MOD_CIF_FOLDER_LEN         3
#define MOD_CIF_EXPNAME_CIF        (g_mod_cif_exp_name_wstr)// "cif"
#define MOD_CIF_EXPNAME_CIF_LEN    3

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef enum
{
    // applet type
    CAF_APPLET_APPLICATION,  //应用程序
    CAF_APPLET_TOOLS,
    CAF_APPLET_PIM,
    CAF_APPLET_CONTROL_DLL, //控件动态库
    CAF_APPLET_COMMON_DLL, //通用动态库

    // next number is extension type
    CAF_EXTENSION,

    CAF_APPLET_MAX=0xFFFF
}CAF_APPLET_TYPE_E;

// applets' struct
typedef struct
{
    CAF_GUID_T          guid;
    uint32              text_id;
    uint32              icon_id;
    CAF_APPLET_TYPE_E   type;
    uint32              flag;
    uint32              pad1;
    uint32              pad2;
}CAF_APP_INFO_T;

// define the type of class
typedef enum 
{
    CAF_CLASS_APPLET,
    CAF_CLASS_EXT,
    CAF_CLASS_MAX
} CAF_CLASS_TYPE_E;


// define the type of class
typedef enum 
{
    CAF_APPLET_POPUP,
    CAF_APPLET_SSAVER,
    CAF_APPLET_PHONE,
    CAF_APPLET_SHOW_MENU,
    CAF_APPLET_HIDDEN_MENU,
    CAF_APPLET_FLAG_MAX
} CAF_APPLET_FLAG_E;

// 定义扩展接口类是否是受保护状态
typedef uint32 CAF_EXTENSION_TYPE;
#define CAF_EXTENSION_UNPROTECTED       0
#define CAF_EXTENSION_PROTECTED         1

// module type
typedef enum
{
    CAF_MOD_STATIC,
    CAF_MOD_DYNAMIC,
    CAF_MOD_TYPE_MAX
} CAF_MODULE_TYPE_E;


typedef enum
{
    MMI_QUERY_BY_MODID = 0,
    MMI_QUERY_BY_GUID,
    MMI_QUERY_MAX
}MMI_MODINFO_QUERY_TYPE_E;

//RiancyZhang 2009-04-12
//模块安装错误定义
typedef enum
{
	CAF_MOD_SETUP_OK,
	CAF_MOD_SETUP_GUIDERR,//guid 冲突
	CAF_MOD_SETUP_NOMEM,//没有空间
	CAF_MOD_SETUP_XMLERR,//xml解析错误
	CAF_MOD_SETUP_FILEERR,//文件系统错误
	CAF_MOD_SETUP_PARAMERR,//参数错误
	CAF_MOD_SETUP_UNKNOWN	//未知错误
	
}CAF_MOD_SETUP_ERR_E;
/************************************************************************/
/* start xml parse struct define                                        */
/************************************************************************/
/************************************************************************/
/* to define the xml tag                                                */
/************************************************************************/
typedef enum 
{
    XML_CIFINFO_TAG = 0,
    XML_STRINGS_TAG,
    XML_STRING_TAG,
    XML_OBJECTS_TAG,
    XML_OBJECT_TAG,
    XML_EXPORTED_TAG,
    XML_CLASS_TAG,
    XML_IMPORTED_TAG,
    XML_APPLETS_TAG,
    XML_APPLET_TAG,
    XML_CLASSID_TAG,
    XML_FLAGS_TAG,
    XML_FLAG_TAG,
    XML_EFS_TAG,
	XML_CRC_TAG,
	XML_INVALID_TAG
}MMI_MODINFO_XML_TAG_E;

typedef enum
{
    XML_STR_UTF8,
    XML_STR_ASCII,
    XML_OBJ_BMP,
    XML_OBJ_JPG,
    XML_ENCODE_MAX
}MMI_XML_ENCODE_E;

typedef enum
{
    XML_EXPORTED_CLASS,
    XML_IMPORTED_CLASS,
    XML_CLASS_MAX
}MMI_XML_CLASS_TYPE_E;

typedef enum
{
    XML_APPLET_CLSID,
    XML_APPLET_TEXTID,
    XML_APPLET_ICONID,
    XML_APPLET_TYPE,
    XML_APPLET_ABBRIB_MAX
}MMI_XML_APPLET_ABBRIB_E;

/*added by chenpeng for my application 09/25/08*/
typedef enum
{
	CAF_MOD_STORE_PHONE,
	CAF_MOD_STORE_SD,

	CAF_MOD_STORE_MAX
}CAF_MOD_STORE_TYPE_E;

// 定义Object基本类型
typedef struct 
{
    uint32  id;
    uint32  pad;
    uint8  *name;
    CAF_STRING_T content;
}MMI_OBJECT_T;

typedef struct 
{
    MMI_XML_ENCODE_E type;
    uint32 num;
    MMI_LINK_NODE_T *text_list_ptr; // 节点为MMI_OBJECT_T类型
}MMI_CIF_STRING_T;

// 定义文本类型,存放文本
typedef MMI_CIF_STRING_T MMI_MODINFO_TEXT_T;

// 定义对象类型,存放图片等对象
typedef MMI_CIF_STRING_T MMI_MODINFO_OBJECT_T;

// 定义导出接口类结构
typedef struct 
{
    CAF_GUID_T guid;
    CAF_EXTENSION_TYPE is_protected;
}MMI_EXPORTED_CLASS_T;

// 定义导入接口类
typedef struct 
{
    CAF_GUID_T guid;
}MMI_IMPORTED_CLASS_T;


typedef struct 
{
    MMI_MODINFO_XML_TAG_E   cur_tag;            // 解析状态
    CAF_STRING_T            version_data;       // 版本信息
	uint32                  crc_data;			// CRC信息
    MMI_MODINFO_TEXT_T      cif_str_data;       // 字符串信息
    MMI_MODINFO_OBJECT_T    cif_obj_data;       // 图标对象信息
    MMI_LINK_NODE_T         *exported_list_ptr; // 导出接口类信息, 节点为MMI_EXPORTED_CLASS_T类型
    MMI_LINK_NODE_T         *imported_list_ptr; // 倒入接口类信息, 节点为MMI_IMPORTED_CLASS_T类型
    MMI_LINK_NODE_T         *applet_list_ptr;   // 小程序信息, 节点为CAF_CLASS_INFO_T类型
}MMI_CIF_STRUCT_T;

/************************************************************************/
/* end parse xml struct define                                          */
/************************************************************************/
typedef uint32 CAF_MODULE_FLAG_T;
#define CAF_MOD_FLAG_INVALID_VER 0x00000001

// static module info
// typedef struct
// {
//     MODULEENTRY             entry;
//     CAF_APP_INFO_T*         applet_info_ptr;    // class information
//     uint32                  applet_num;
// }CAF_STATIC_MODULE_T;

// get static module info
// typedef struct
// {
//     const CAF_STATIC_MODULE_T** module_pptr;
//     uint32                      module_num;
// }CAF_STATIC_MODULE_INFO_T;

// module information struct
typedef struct
{
    MODULEENTRY             entry;
    wchar                   cif_file_name[CAF_FILENAME_MAX_LEN + 1];
    wchar                   res_file_name[CAF_FILENAME_MAX_LEN + 1];
    wchar                   dll_file_name[CAF_FILENAME_MAX_LEN + 1];
    CAF_MODULE_FLAG_T       flag;               // 平台版本是否支持
    CAF_STRING_T            version_data;       // 版本信息
	uint32                  crc_data;       // crc
    MMI_MODINFO_TEXT_T      cif_str_data;       // 字符串信息
    MMI_MODINFO_OBJECT_T    cif_obj_data;       // 图标对象信息
    uint32                  class_num;
    MMI_LINK_NODE_T         *class_info_ptr;    // class information
    uint32                  dep_num; 
    MMI_LINK_NODE_T         *dep_guid_ptr;      // dependeces
    uint32                  exp_num;
    MMI_LINK_NODE_T         *exported_list_ptr; // 导出接口类信息, 节点为MMI_EXPORTED_CLASS_T类型
	CAF_MOD_STORE_TYPE_E	store;			//存贮位置
	uint32					create_time;	//创建时间
	uint32					size;	//resource file size + bin file size
}CAF_MODULE_INFO_T;

// module info node
typedef struct mmi_modinfo_node_tag
{
    CAF_MODULE_TYPE_E  mod_type;
    uint32             mod_id;    
    CAF_MODULE_INFO_T  module_info;
    struct mmi_modinfo_node_tag *next_ptr;
    struct mmi_modinfo_node_tag *prev_ptr;
}MMI_MODINFO_NODE_T;

// module information list
typedef struct mmi_modinfo_list_tag
{
    uint32 mod_num;
    MMI_MODINFO_NODE_T *modinfo_list_ptr;
}MMI_MODINFO_LIST_T;

//文件打包
typedef struct
{
    uint8   ident[MMK_SZIP_IDENT_MAX_LEN]; //文件标示
    uint32  file_num;                      //文件个数
    uint32  total_size;                    //所有文件大小
    uint32  crc_data;                      //crc校验
    uint32  reserved;                      //保留
}MMK_SZIP_HEAD_INFO;

typedef struct
{
    uint32  file_name_len;                //文件名长度
    uint32  file_size;                    //文件大小
    uint32  zip_size;                     //文件压缩后大小
    uint32  reserved;                     //保留

}MMK_SZIP_FILE_INFO;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern const wchar g_mod_cif_dev_name_d_wstr[];
extern const wchar g_mod_cif_dev_name_e_wstr[];
extern const wchar g_mod_cif_folder_wstr[] ;
extern const wchar g_mod_cif_exp_name_wstr[];
/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : load all files' info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T *MMK_LoadStaticModInfo(void);

/*****************************************************************************/
// 	Description : load all files' info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T *MMK_LoadDynamicModInfo(void);

/*****************************************************************************/
// 	Description : load file info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_NODE_T *MMK_SearchModInfo (
                                              uint32 id,
                                              MMI_MODINFO_QUERY_TYPE_E type
                                              );

/*****************************************************************************/
// 	Description : parse xml
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CafXmlParser(
                                uint16 *file_name_ptr,
                                MMI_MODINFO_NODE_T *mod_info_ptr
                                );

/*****************************************************************************/
// 	Description : update all files' info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T *MMK_UpdateDynamicModList(void);

/*****************************************************************************/
// 	Description : unload file's info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMK_UnLoadDynModInfo(uint32 mod_id);

/*****************************************************************************/
// 	Description : unload all files' info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMK_UnLoadAllDynModInfo(void);

/*****************************************************************************/
// 	Description : Get the point of s_dyn_modinfo_list
//	Global resource dependence : 
//  Author:peng.chen
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T * MMK_GetDynModInfoList(void);

/*****************************************************************************/
// 	Description : Get the point of s_static_modinfo_list
//	Global resource dependence : 
//  Author:peng.chen
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T * MMK_GetStaticModInfoList(void);

/*****************************************************************************/
// 	Description : Parse the caf XML file interface 
//	Global resource dependence : 
//  Author:RiancyZhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_MOD_SETUP_ERR_E MMK_CafGUIDIsValid(uint16 *file_name_ptr);

/*****************************************************************************/
// 	Description : Search object by object id
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC MMI_OBJECT_T *MMK_SearchObject(
                                      MMI_LINK_NODE_T *head_node_ptr, 
                                      uint32 obj_id
                                      );

/*****************************************************************************/
// 	Description : search applet type
//	Global resource dependence : 
//  Author:Andrew.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetAppletType(CAF_GUID_T guid, CAF_APPLET_TYPE_E *applet_type_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif // _MMK_MODULEINFO_H_

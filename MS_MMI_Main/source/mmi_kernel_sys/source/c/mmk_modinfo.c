/****************************************************************************
** File Name:      mmk_modinfo.c                                           *
** Author:         Xiaoqing.Lu                                             *
** Date:           05/07/2008                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2008        Xiaoqing.Lu       Create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "mmi_kernel_sys_trc.h"
#include "cafshell.h"
#include "mmk_app.h"

#include "mmi_theme.h"
#include "guicommon.h"
#include "mmk_modinfo.h"
#include "sfs.h"

#include "mmi_link.h"
#include "xml_sap.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
// 定义uint32类型的数据最大长度为16,实际是不会超过10的
#define CIF_XML_MAX_NUM_LEN     16 //
// 定义最大长度字符串
#define MMI_MAX_TEXT_LEN        1024 
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

typedef struct  
{
    MMI_CIF_STRUCT_T      cif_data;
    MMI_OBJECT_T*         cur_text_ptr;
    wchar                 text_arr[MMI_MAX_TEXT_LEN + 1];
    MMI_XML_CLASS_TYPE_E  cur_class_type;
    CAF_STRING_T          obj_data;
}MMI_XML_PARSE_TEMP_T;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#ifdef DYNAMIC_MODULE_SUPPORT
LOCAL MMI_MODINFO_LIST_T s_dyn_modinfo_list = {0}; // 动态模块
#endif
LOCAL MMI_MODINFO_LIST_T s_static_modinfo_list = {0}; // 静态模块
LOCAL uint32            s_com_id_count = 0;
LOCAL BOOLEAN           s_is_comid_cyc = FALSE; // 判断com id是否已经经过了一个循环，已经达到过一次0xFFFFFFFF

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
// 根据全文件名获取路径名
extern BOOLEAN MMIAPICOM_GetFilePath(
                                     const uint16 *file_name_ptr, // [IN]
                                     uint16 file_name_len,  // [IN]
                                     uint16 *file_path_ptr, // [OUT]
                                     uint16 *file_path_len  // [OUT]
                                     );
// 邦定路径名
extern BOOLEAN MMIAPICOM_CombinePath(
                                     const wchar     *dir_ptr,           //in
                                     uint16           dir_len,            //in, 双字节为单位
                                     const wchar     *file_name_ptr,     //in, can be NULL
                                     uint16           file_name_len,      //in, 双字节为单位
                                     BOOLEAN          is_extname,         //in, 是否为扩展名
                                     wchar            *full_path_ptr,     //out
                                     uint16           *full_path_len_ptr  //out, 双字节为单位
                                     );
#ifdef DYNAMIC_MODULE_SUPPORT
extern uint32 MMIAPICOM_Tm2Second(uint32 tm_sec, uint32 tm_min, uint32 tm_hour, uint32 tm_mday, uint32 tm_mon,  uint32 tm_year);
#endif
extern void MMI_GetStaticModuleInfo( CAF_STATIC_MODULE_T* static_module_ptr );

/************************************************************************/
/* to define the xml tag string array                                   */
/************************************************************************/
LOCAL const char* const s_tag_arr[] = 
{
	"CIFInfo",
	"Strings",
	"String",
	"Objects",
	"Object",
	"Exported",
	"Class",
	"Imported",
    "Applets",
    "Applet",
    "ClassID",
    "Flags",
    "Flag",
    "EFS",
    "CRC"
};

/************************************************************************/
/* to define the xml tag string array                                   */
/************************************************************************/
LOCAL const char* const s_xml_encode_arr[] = 
{
	"UTF-8",
    "ASCII",
	"image/BMP",
    "image/JPG"
};

/************************************************************************/
/* to define the xml tag string array                                   */
/************************************************************************/
LOCAL const char* const s_applet_type_arr[] = 
{
	"Game",
    "Tools",
	"PIM",
    "Web",
    "Hidden"
};

/************************************************************************/
/* to define the applet's attribs array                                 */
/************************************************************************/
LOCAL const char* const s_applet_attrib_arr[] = 
{
	"clsid",
    "textid",
	"iconid",
    "type"
};

/************************************************************************/
/* include some versions of the platform, from a func to decide witch   */
/* module does not run under the platform.                              */
/************************************************************************/
LOCAL const uint32 s_versino_info_arr[3] =
{
    1, 0, 0
};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
const wchar g_mod_cif_dev_name_d_wstr[] = { 'D', ':', 0 };      //MOD_CIF_DEVICE_NAME_D "\x00\x44\x00\x3a\x00\x00"  // "D"
const wchar g_mod_cif_dev_name_e_wstr[] = { 'E', ':', 0 };      //MOD_CIF_DEVICE_NAME_E "\x00\x45\x00\x3a\x00\x00"  // "E"
const wchar g_mod_cif_folder_wstr[]     = { 'C', 'o', 'm', 0 }; //MOD_CIF_FOLDER          "\x00\x43\x00\x6f\x00\x6d\x00\x00"  // "Com"
const wchar g_mod_cif_exp_name_wstr[] = { 'c', 'i', 'f', 0 }; //MOD_CIF_EXPNAME_CIF     "\x00\x63\x00\x69\x00\x66\x00\x00" // "cif"

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : init module info
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GenerateComID(void);

/*****************************************************************************/
// 	Description : init module info
//	Global resource dependence : 
//  Author:xiaoqinglu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadStaticCIF(MMI_MODINFO_LIST_T * module_list_head_ptr);
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : load module information
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN LoadDynamicXmlCIF(
                                MMI_MODINFO_LIST_T *modinfo_list_ptr,
                                const wchar *dev_ptr,
                                uint16       dev_wstr_len
                                );
#endif
/*****************************************************************************/
// 	Description : delete module information from list by module id
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMI_MODINFO_NODE_T *SearchModInfoByModId (
                                                MMI_MODINFO_LIST_T *modinfo_list_ptr,
                                                uint32 mod_id
                                                );

/*****************************************************************************/
// 	Description : search in list  
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchInList(MMI_LINK_NODE_T *head_node_ptr, CAF_GUID_T guid);

/*****************************************************************************/
// 	Description : search module information from list by guid
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMI_MODINFO_NODE_T *SearchModInfoByGuid (
                                                MMI_MODINFO_LIST_T *modinfo_list_ptr,
                                                uint32 guid
                                                );

/*****************************************************************************/
// 	Description : to set element's attributes
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMI_XML_ENCODE_E XmlGetObjectEncodeType(
                                              const uint16 *attributes_pptr
                                              );

/*****************************************************************************/
// 	Description : Get Cif Version Attrib 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetCifVersionAttrib( 
                                  MMI_XML_PARSE_TEMP_T* temp_ptr,
                                  const uint16 **attributes_pptr
                                  );

/*****************************************************************************/
// 	Description : uint16转换 int32数字 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL int32 Uint16_2_Int32(uint16 const *src_ptr) ;

/*****************************************************************************/
// 	Description : Get Cif String content
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetCifStringAttribs(
                                  MMI_XML_PARSE_TEMP_T* temp_ptr,
                                  const uint16 **attributes_pptr
                                  );

/*****************************************************************************/
// 	Description : Get Cif Object content
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetCifObjectAttribs(
                                  MMI_XML_PARSE_TEMP_T* temp_ptr,
                                  const uint16 **attributes_pptr
                                  );

/*****************************************************************************/
// 	Description : uint16转换 int32数字
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL int32 GetX16ToInt32(uint16 const *src_ptr) ;

/*****************************************************************************/
// 	Description : Get Exported class attribs
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetClassAttribs(
                              MMI_XML_PARSE_TEMP_T* temp_ptr,
                              const uint16 **attributes_pptr, 
                              MMI_XML_CLASS_TYPE_E type
                              );

/*****************************************************************************/
// 	Description : Get Applet Attribs 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetAppletAttribs( 
                               MMI_XML_PARSE_TEMP_T* temp_ptr,
                               const uint16 **attributes_pptr
                               );

/*****************************************************************************/
// 	Description : to set element's attributes
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlSetElementAttribs(
                                MMI_XML_PARSE_TEMP_T* temp_ptr,
                                MMI_MODINFO_XML_TAG_E tag, 
                                const uint16 **attributes_pptr
                                );

/*****************************************************************************/
// 	Description : to set element's attributes
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlSetElementContext( 
                                MMI_XML_PARSE_TEMP_T* temp_ptr
                                );

/*****************************************************************************/
// 	Description : callback, Start to parse xml element and attributes
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlStartParseElementSax(
                                   void *user_data_ptr,
                                   const uint16 *localname_ptr,
                                   const uint16 **attributes_pptr
                                   );

/*****************************************************************************/
// 	Description : callback, End to parse xml element 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlEndParseElementSax(
                                 void *user_data_ptr,
                                 const uint16 *localname_ptr
                                 );

/*****************************************************************************/
// 	Description : callback, to parse xml content of element 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlParseCharactersSAX(
                                 void *user_data_ptr,
                                 const uint16 *ch_ptr 
                                 );

/*****************************************************************************/
// 	Description : Read Xml file information function
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint8* ReadXmlFileInfo(
                             uint16 *file_name_ptr,
                             uint32 *file_size_out
                             );

/*****************************************************************************/
// 	Description : Free the struct of MMI_OBJECT_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void FreeObjectNode(
                          MMI_LINK_NODE_T const * node_ptr
                          );

/*****************************************************************************/
// 	Description : Free the struct of MMI_OBJECT_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void FreeExportedNode(
                            MMI_LINK_NODE_T const * node_ptr
                            );

/*****************************************************************************/
// 	Description : Free the struct of MMI_OBJECT_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void FreeImportedNode(
                            MMI_LINK_NODE_T const * node_ptr
                            );

/*****************************************************************************/
// 	Description : Free the struct of MMI_OBJECT_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void FreeAppInfoNode(
                           MMI_LINK_NODE_T const * node_ptr
                           );

/*****************************************************************************/
// 	Description : Release the struct of MMI_CIF_STRUCT_T 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void ReleaseCifInfo(
                          MMI_CIF_STRUCT_T cif_data
                          );

/*****************************************************************************/
// 	Description : 查找字符串 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T * FindFileNameInStrList(
                                              MMI_LINK_NODE_T *head_node_ptr, 
                                              uint32 id
                                              );

/*****************************************************************************/
// 	Description : 根据解析xml所得数据,转化成CAF_MODULE_INFO_T结构 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN XmlGetCafInformation(
                                   const MMI_CIF_STRUCT_T *xml_data_ptr,
                                   CAF_MODULE_INFO_T *cif_data_ptr,
                                   uint16 *file_name_ptr
                                   );

/*****************************************************************************/
// 	Description : 验证版本信息
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN VersionInfoIsValid( CAF_STRING_T* ver_ptr );

/*****************************************************************************/
// 	Description : Parse the caf XML file interface 
//	Global resource dependence : 
/************************************************************************/
/* 把ucs2的数据转化成bmp数据。                                          */
/************************************************************************/
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN XmlConvertUcs2BufToBmpData(
                                         const uint8 *data_ptr, 
                                         uint32 indata_len, 
                                         char *bmp_ptr, 
                                         uint32 outdata_len
                                         );


/*****************************************************************************/
// 	Description : delete all module information and module information list 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteAllModInfo (
                                MMI_MODINFO_LIST_T* modinfo_list_ptr
                                );
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : Parse the caf XML file interface 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CafXmlParser(
                           uint16 *file_name_ptr,
                           MMI_MODINFO_NODE_T *mod_info_ptr
                           );
#endif
/*****************************************************************************/
// 	Description : search mod info by cif name
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMI_MODINFO_NODE_T *SearchModInfoByCifName (
                                                  MMI_MODINFO_LIST_T *modinfo_list_ptr,
                                                  uint16 *cif_file_name
                                                  );
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : init list 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitDynamicModInfoList(void);
#endif
/*****************************************************************************/
// 	Description : init list 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitStaticModInfoList(void);
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : delete module information from list by module id
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteModInfo (
                             MMI_MODINFO_LIST_T *modinfo_list_ptr,
                             uint32 mod_id
                             );
#endif
/*****************************************************************************/
// 	Description : free module information node
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeModNodeDetailInfo(
                                    MMI_MODINFO_NODE_T *node_ptr
                                    );

/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init module info
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint32 GenerateComID(void)
{
    if (s_com_id_count == 0xFFFFFFFF || s_is_comid_cyc)
    {
        s_com_id_count++;
        if (s_com_id_count == 0) // 确保com id不能为0
        {
            s_com_id_count = 1;
        }
        while (MMK_SearchModInfo(s_com_id_count, MMI_QUERY_BY_MODID))
        {
            s_com_id_count++;
        } 
    }
    else
    {
        s_com_id_count++;
        if(s_com_id_count == 0xFFFFFFFF) // 达到了一个循环，设置状态
        {
            s_is_comid_cyc = TRUE;
        }
    }

    return s_com_id_count;
}
/*****************************************************************************/
// 	Description : init module info
//	Global resource dependence : 
//  Author:xiaoqinglu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN LoadStaticCIF(MMI_MODINFO_LIST_T * module_list_head_ptr)
{
    uint32 i = 0;
    uint32 j = 0;

    MMI_MODINFO_NODE_T* modinfo_node_ptr = PNULL;
    CAF_STATIC_MODULE_T static_module = {0};

    MMI_GetStaticModuleInfo( &static_module );
    
    SCI_ASSERT( PNULL != module_list_head_ptr );/*assert verified*/
    SCI_ASSERT( PNULL != static_module.entry && PNULL != static_module.applet_info_pptr && 0 != static_module.applet_info_num );/*assert verified*/
    
    modinfo_node_ptr = SCI_ALLOC_APP(sizeof(MMI_MODINFO_NODE_T));
    SCI_ASSERT(PNULL != modinfo_node_ptr);/*assert verified*/
    SCI_MEMSET(modinfo_node_ptr, 0, sizeof(MMI_MODINFO_NODE_T) );
    
    //set applet info
    modinfo_node_ptr->module_info.entry = static_module.entry;
    
    //add applet info to node
    for ( i = 0; i < static_module.applet_info_num; i++ )
    {
        CAF_STATIC_APPLET_INFO_T* static_info_ptr = static_module.applet_info_pptr[i];

        if ( PNULL != static_info_ptr )
        {
            for ( j = 0; j < static_info_ptr->num_of_applet; j++ )
            {
                SCI_ASSERT( 0 != static_info_ptr->applet_ptr[j].guid && 0 != static_info_ptr->applet_ptr[j].size && PNULL != static_info_ptr->applet_ptr[j].func );/*assert verified*/
                
                modinfo_node_ptr->module_info.class_num++;
                
                if ( PNULL == modinfo_node_ptr->module_info.class_info_ptr )
                {
                    modinfo_node_ptr->module_info.class_info_ptr = MMILINK_CreateHead((uint32)&static_info_ptr->applet_ptr[j]);
                }
                else
                {
                    MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)&static_info_ptr->applet_ptr[j]);
                    
                    MMILINK_AddNodeBeforeBaseNode( modinfo_node_ptr->module_info.class_info_ptr, tmp_node_ptr );
                }
            }
        }
    }

    SCI_ASSERT( 0 != modinfo_node_ptr->module_info.class_num );/*assert verified*/
    
    modinfo_node_ptr->mod_type = CAF_MOD_STATIC;
    modinfo_node_ptr->mod_id = GenerateComID();
    // add this node to list
    if(0 < module_list_head_ptr->mod_num)
    {
        modinfo_node_ptr->next_ptr = module_list_head_ptr->modinfo_list_ptr;
        modinfo_node_ptr->prev_ptr = module_list_head_ptr->modinfo_list_ptr->prev_ptr;
        module_list_head_ptr->modinfo_list_ptr->prev_ptr->next_ptr = modinfo_node_ptr;
        module_list_head_ptr->modinfo_list_ptr->prev_ptr = modinfo_node_ptr;
    }
    else
    {
        module_list_head_ptr->modinfo_list_ptr = modinfo_node_ptr;
        module_list_head_ptr->modinfo_list_ptr->prev_ptr = modinfo_node_ptr;
        module_list_head_ptr->modinfo_list_ptr->next_ptr = modinfo_node_ptr;
    }
    module_list_head_ptr->mod_num ++;

#if 0
    //get module func table
    extern const CAF_STATIC_MODULE_INFO_T g_caf_static_module_info;

    SCI_ASSERT(PNULL != module_list_head_ptr);/*assert verified*/

    for(i = 0; i < g_caf_static_module_info.module_num; i ++)
    {
        const CAF_STATIC_MODULE_T* static_ptr = g_caf_static_module_info.module_pptr[i];
        
        if( PNULL != static_ptr )
        {
            MMI_MODINFO_NODE_T* modinfo_node_ptr = PNULL;

            SCI_ASSERT( PNULL != static_ptr->entry && PNULL != static_ptr->applet_info_ptr && 0 != static_ptr->applet_num );/*assert verified*/

            modinfo_node_ptr = SCI_ALLOC_APP(sizeof(MMI_MODINFO_NODE_T));
            SCI_ASSERT(PNULL != modinfo_node_ptr);/*assert verified*/
            SCI_MEMSET(modinfo_node_ptr, 0, sizeof(MMI_MODINFO_NODE_T) );
            
            //set applet info
            modinfo_node_ptr->module_info.entry = static_ptr->entry;
            modinfo_node_ptr->module_info.class_num = static_ptr->applet_num;
            
            //add applet info to node
            for (j = 0; j < modinfo_node_ptr->module_info.class_num; j++)
            {
                CAF_APP_INFO_T* applet_info_ptr = &static_ptr->applet_info_ptr[j];

                SCI_ASSERT( PNULL != applet_info_ptr );/*assert verified*/

                if ( modinfo_node_ptr->module_info.class_info_ptr == PNULL )
                {
                    modinfo_node_ptr->module_info.class_info_ptr = MMILINK_CreateHead((uint32)applet_info_ptr);
                }
                else
                {
                    MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)applet_info_ptr);
                    MMILINK_AddNodeBeforeBaseNode( modinfo_node_ptr->module_info.class_info_ptr, tmp_node_ptr );
                }
            }
            
            modinfo_node_ptr->mod_type = CAF_MOD_STATIC;
            modinfo_node_ptr->mod_id = GenerateComID();
            // add this node to list
            if(0 < module_list_head_ptr->mod_num)
            {
                modinfo_node_ptr->next_ptr = module_list_head_ptr->modinfo_list_ptr;
                modinfo_node_ptr->prev_ptr = module_list_head_ptr->modinfo_list_ptr->prev_ptr;
                module_list_head_ptr->modinfo_list_ptr->prev_ptr->next_ptr = modinfo_node_ptr;
                module_list_head_ptr->modinfo_list_ptr->prev_ptr = modinfo_node_ptr;
            }
            else
            {
                module_list_head_ptr->modinfo_list_ptr = modinfo_node_ptr;
                module_list_head_ptr->modinfo_list_ptr->prev_ptr = modinfo_node_ptr;
                module_list_head_ptr->modinfo_list_ptr->next_ptr = modinfo_node_ptr;
            }

            module_list_head_ptr->mod_num ++;

        }
    }
#endif

    return TRUE;
}

/*****************************************************************************/
// 	Description : load all files' info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T *MMK_LoadStaticModInfo(void)
{
    BOOLEAN result = FALSE;

    InitStaticModInfoList();
    // static cif
    result = LoadStaticCIF(&s_static_modinfo_list);

    if (result)
    {
        return  &s_static_modinfo_list;
    }
    else
    {
        return PNULL;
    }
}
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : load module information
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN LoadDynamicXmlCIF(
                                MMI_MODINFO_LIST_T *modinfo_list_ptr,
                                const wchar *dev_ptr,
                                uint16       dev_wstr_len
                                )
{
    BOOLEAN             result                              = TRUE;
    SFS_FIND_DATA_T     find_data                           = {0};
    SFS_HANDLE          sfs_handle                          = 0;
    uint16              file_name_len                       = 0;
    uint16              file_name1_len                      = 0;  // file_name1 的长度
    wchar               *file_name1                         = PNULL;// 保存 "D:\Com"
    wchar               *file_name2                         = PNULL; 
    wchar               *file_name3                         = PNULL;
    MMI_MODINFO_NODE_T  *trail_nod_ptr = PNULL;
    MMI_MODINFO_NODE_T  *mod_nod_ptr = PNULL;
    wchar                start_char = '*';
    uint16               mem_size = (CAF_FILENAME_MAX_LEN + 1) * sizeof(wchar);
    
    file_name1 = SCI_ALLOC_APP( mem_size );
    file_name2 = SCI_ALLOC_APP( mem_size );
    file_name3 = SCI_ALLOC_APP( mem_size );
    SCI_MEMSET(file_name1, 0, mem_size );
    SCI_MEMSET(file_name2, 0, mem_size );
    SCI_MEMSET(file_name3, 0, mem_size );
    
    // combine the device name
    MMIAPICOM_CombinePath( dev_ptr, dev_wstr_len, MOD_CIF_FOLDER, MOD_CIF_FOLDER_LEN, FALSE, file_name1, &file_name1_len); // D:\Com
    // combine the device name
    MMIAPICOM_CombinePath(file_name1, file_name1_len, &start_char, 1, FALSE, file_name2, &file_name_len); // D:\Com\*
    
    // find first file in folder of "D:\Com"
    sfs_handle = SFS_FindFirstFile(file_name2, &find_data);
    SCI_ASSERT(PNULL != modinfo_list_ptr);/*assert verified*/
    trail_nod_ptr = modinfo_list_ptr->modinfo_list_ptr;
    // go to trail node
    while (trail_nod_ptr != PNULL && trail_nod_ptr->next_ptr != PNULL) // get trail node ptr
    {
        trail_nod_ptr = trail_nod_ptr->next_ptr;
    }
    
    do // load new information
    {
        if ((MOD_CIF_VALID_HANDLE != sfs_handle) && (find_data.attr & SFS_ATTR_DIR)) 
        {            
            if(0 < (uint16)MMIAPICOM_Wstrlen(find_data.name))
            {                  
                SFS_FIND_DATA_T     find_data_1                           = {0};
                SFS_HANDLE          sfs_handle_1                          = 0;
                
                // combine full path with file name
                file_name1[file_name1_len] = 0;
                SCI_ASSERT(0 < (uint16)MMIAPICOM_Wstrlen(find_data.name)); /*assert verified*/
                SCI_MEMSET( file_name2, 0, mem_size);
                MMIAPICOM_CombinePath(file_name1, file_name1_len, find_data.name, (uint16)MMIAPICOM_Wstrlen(find_data.name), FALSE, file_name3, &file_name_len); // D:\Com\Test1
                
                MMIAPICOM_CombinePath(file_name3, file_name_len, &start_char, 1, FALSE, file_name2, &file_name_len); // D:\Com\Test1\*
                
                sfs_handle_1 = SFS_FindFirstFile(file_name2, &find_data_1);
                
                //装入目录下的所有.cif文件
                do
                {                    
                    if ((MOD_CIF_VALID_HANDLE != sfs_handle_1) && ((find_data_1.attr & SFS_ATTR_DIR) == 0)) 
                    {                        
                        uint16 len = 0;
                        wchar *p1 = PNULL;
                        BOOLEAN is_cif_file = FALSE;
                        
                        //判断是否是.cif文件
                        len = (uint16)MMIAPICOM_Wstrlen(find_data_1.name);
                        if(len > MOD_CIF_EXPNAME_CIF_LEN)
                        {
                            p1 = find_data_1.name + len - MOD_CIF_EXPNAME_CIF_LEN;
                            
                            if(MMIAPICOM_Wstrcmp(p1,(const wchar *)MOD_CIF_EXPNAME_CIF) == 0)
                            {
                                is_cif_file = TRUE;
                            }
                        }
                        
                        if(is_cif_file == TRUE)
                        {                            
                            MMIAPICOM_CombinePath(file_name1, file_name1_len, find_data.name, (uint16)MMIAPICOM_Wstrlen(find_data.name), FALSE, file_name3, &file_name_len); // D:\Com\Test1
                            MMIAPICOM_CombinePath(file_name3, file_name_len, find_data_1.name, (uint16)MMIAPICOM_Wstrlen(find_data_1.name), FALSE, file_name2, &file_name_len);  // D:\Com\Test1\Test1.cif
                            
                            file_name2[file_name_len] = 0;
                            
                            // 判断是否已经load
                            if(PNULL == SearchModInfoByCifName(&s_dyn_modinfo_list, file_name2))
                            {
                                if (mod_nod_ptr == PNULL)
                                {
                                    mod_nod_ptr = SCI_ALLOC_APP(sizeof(MMI_MODINFO_NODE_T));
                                    SCI_MEMSET(mod_nod_ptr, 0, sizeof(MMI_MODINFO_NODE_T));
                                }
                                
                                if(CafXmlParser(file_name2, mod_nod_ptr)) // successful
                                {
                                    //						uint32 len = 0;
                                    uint32 size = 0;
                                    /*get code size*/
                                    SFS_HANDLE f_handle = SFS_CreateFile(mod_nod_ptr->module_info.dll_file_name,
                                        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
                                    if(0 != f_handle)
                                    {
                                        /* get file size */
                                        SFS_GetFileSize(f_handle, &size);
                                        SFS_CloseFile(f_handle);
                                    }						
                                    /*get resource size*/
                                    f_handle = SFS_CreateFile(mod_nod_ptr->module_info.res_file_name, 
                                        SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
                                    if(0 != f_handle)
                                    {
                                        /* get file size */
                                        SFS_GetFileSize(f_handle, &mod_nod_ptr->module_info.size);
                                        SFS_CloseFile(f_handle);
                                    }
                                    /*get the total size of bin and res*/
                                    mod_nod_ptr->module_info.size += size;
                                    
                                    /*set location info*/
                                    if (!MMIAPICOM_Wstrcmp( dev_ptr, MOD_CIF_DEVICE_NAME_D ) )
                                    {
                                        mod_nod_ptr->module_info.store = CAF_MOD_STORE_PHONE;						
                                    }
                                    else if(!MMIAPICOM_Wstrcmp( dev_ptr, MOD_CIF_DEVICE_NAME_E ) )
                                    {
                                        mod_nod_ptr->module_info.store = CAF_MOD_STORE_SD;
                                    }
                                    else
                                    {
                                        SCI_PASSERT(0, ("MMIDYNA: Store location is err!"));/*assert verified*/
                                    }
                                    
                                    /*set create date*/
                                    mod_nod_ptr->module_info.create_time = MMIAPICOM_Tm2Second(
                                        find_data.create_time.sec,
                                        find_data.create_time.min,
                                        find_data.create_time.hour, 
                                        find_data.create_Date.mday, 
                                        find_data.create_Date.mon, 
                                        find_data.create_Date.year);
                                    if (0 == modinfo_list_ptr->mod_num) // the first node
                                    {
                                        modinfo_list_ptr->modinfo_list_ptr = mod_nod_ptr; // first node
                                        trail_nod_ptr = mod_nod_ptr;
                                        modinfo_list_ptr->mod_num = 1; // num is 1
                                    }
                                    else // other node
                                    {
                                        modinfo_list_ptr->mod_num++; // add num
                                        SCI_ASSERT(PNULL != trail_nod_ptr);/*assert verified*/
                                        trail_nod_ptr->next_ptr = mod_nod_ptr; // add next ptr
                                        trail_nod_ptr = trail_nod_ptr->next_ptr;// move trail poiter
                                    }
                                    mod_nod_ptr = PNULL;
                                }
                                else // read fail then free memory
                                {
                                    SCI_FREE(mod_nod_ptr);
                                    mod_nod_ptr = PNULL;
                                } 
                            } //if(PNULL == SearchModInfoByCifName(&s_dyn_modinfo_list, file_name2))
                        } //if(is_cif_file == TRUE)
                    } //if ((MOD_CIF_VALID_HANDLE != sfs_handle_1) && ((find_data_1.attr & SFS_ATTR_DIR) == 0))
                    
                    SCI_MEMSET(&find_data_1, 0, sizeof(find_data_1));
                }while(SFS_ERROR_NONE == SFS_FindNextFile(sfs_handle_1, &find_data_1));
            } //if(0 < (uint16)MMIAPICOM_Wstrlen(find_data.name))
        } //if ((MOD_CIF_VALID_HANDLE != sfs_handle) && (find_data.attr & SFS_ATTR_DIR)) 
        
        SCI_MEMSET(&find_data, 0, sizeof(find_data));
        
    } while(SFS_ERROR_NONE == SFS_FindNextFile(sfs_handle, &find_data)); // find next file in folder of "D:\Com"
    
    SFS_FindClose(sfs_handle);
    
    SCI_FREE(file_name1);
    SCI_FREE(file_name2);
    SCI_FREE(file_name3);
    
    return (result);
}
#endif
/*****************************************************************************/
// 	Description : delete module information from list by module id
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMI_MODINFO_NODE_T *SearchModInfoByModId (
                                                MMI_MODINFO_LIST_T *modinfo_list_ptr,
                                                uint32 mod_id
                                                )
{
    BOOLEAN result = FALSE;
    uint16 i = 0;
    MMI_MODINFO_NODE_T *node_ptr = PNULL;
    MMI_MODINFO_NODE_T *find_node_ptr = PNULL;

    SCI_ASSERT(PNULL != modinfo_list_ptr);/*assert verified*/
    
    node_ptr = modinfo_list_ptr->modinfo_list_ptr;
    find_node_ptr = PNULL;

    for (i = 0; ((i < modinfo_list_ptr->mod_num) && (PNULL != node_ptr)); i++)
    {
        if (node_ptr->mod_id == mod_id)
        {
            find_node_ptr = node_ptr;
            result = TRUE;
            break;
        }
        node_ptr = node_ptr->next_ptr;
    }
    return find_node_ptr;
}

/*****************************************************************************/
// 	Description : search in list  
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchInList(MMI_LINK_NODE_T *head_node_ptr, CAF_GUID_T guid)
{
    MMI_LINK_NODE_T *tmp_ptr = PNULL;
    BOOLEAN is_find = FALSE;
    tmp_ptr = head_node_ptr;

    if (head_node_ptr == PNULL)
    {
        return FALSE;
    }

    do 
    {
        //applet或扩展类的结构体都以guid开头
        if ( *(uint32*)tmp_ptr->data == guid )
        {
            is_find = TRUE;
            break;
        }
        else
        {
            tmp_ptr = tmp_ptr->next_ptr;
        }
    } while(tmp_ptr != head_node_ptr);

    return is_find;    
}

/*****************************************************************************/
// 	Description : search module information from list by guid
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMI_MODINFO_NODE_T *SearchModInfoByGuid (
                                               MMI_MODINFO_LIST_T *modinfo_list_ptr,
                                               uint32 guid
                                               )
{
    BOOLEAN result = FALSE;
    uint16 i = 0;
    MMI_LINK_NODE_T *tmp_ptr = PNULL;
    MMI_MODINFO_NODE_T *node_ptr = PNULL;
    MMI_MODINFO_NODE_T *find_node_ptr = PNULL;
    
    SCI_ASSERT(PNULL != modinfo_list_ptr);/*assert verified*/
    
    node_ptr = modinfo_list_ptr->modinfo_list_ptr;
    find_node_ptr = PNULL;
	//RiancyZhang 2009-04-14
	if(node_ptr == NULL) return NULL;

    tmp_ptr = node_ptr->module_info.class_info_ptr;
    for (i = 0; ((i < modinfo_list_ptr->mod_num) && (PNULL != node_ptr)); i++)
    {
        result = SearchInList(node_ptr->module_info.class_info_ptr, guid);
        if(!result)
        {
            result = SearchInList(node_ptr->module_info.exported_list_ptr, guid);
        }
        
        if (result)
        {
            find_node_ptr = node_ptr;
            break;
        }

        if (node_ptr != PNULL) /*lint !e774 */
        {
            node_ptr = node_ptr->next_ptr;
        }
    }

    return find_node_ptr;
}

/*****************************************************************************/
// 	Description : to set element's attributes
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMI_XML_ENCODE_E XmlGetObjectEncodeType(
                                              const uint16 *attributes_pptr
                                              )
{
    uint32 i = 0;
    MMI_XML_ENCODE_E type = XML_ENCODE_MAX;

    if (attributes_pptr == PNULL)
    {
        return type;
    }
    // 设置解析的属性
    for( i = 0; i < XML_ENCODE_MAX; i++ )
	{
		if( 0 == MMIAPICOM_WstrStrncmp((const wchar*)attributes_pptr, (const uint8*)s_xml_encode_arr[i], strlen(s_xml_encode_arr[i])))
		{
			type = i;
            break;
		}
	}
    
    return type;
}

/*****************************************************************************/
// 	Description : Get Cif Version Attrib 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetCifVersionAttrib(
                                  MMI_XML_PARSE_TEMP_T* temp_ptr,
                                  const uint16 **attributes_pptr
                                  )
{
    uint16 str_len = 0;

    if (attributes_pptr == PNULL || *attributes_pptr == PNULL)
    {
        return;
    }
    
    str_len = MMIAPICOM_Wstrlen(attributes_pptr[1]);
    //s_cif_data.version_data.is_ucs2 = TRUE;
    temp_ptr->cif_data.version_data.wstr_ptr = SCI_ALLOC_APP((str_len + 1)*sizeof(wchar));
    SCI_MEMSET(temp_ptr->cif_data.version_data.wstr_ptr, 0, ((str_len+1)*sizeof(wchar)));

    // 取最小值
//     str_len = MIN(str_len*sizeof(wchar), MMI_MAX_TEXT_LEN);
//     SCI_MEMCPY(s_cif_data.version_data.str_ptr, attributes_pptr[1], str_len);
//     s_cif_data.version_data.length = (uint16)(str_len/sizeof(uint16));

    str_len = MIN(str_len, MMI_MAX_TEXT_LEN);
    MMIAPICOM_Wstrncpy( temp_ptr->cif_data.version_data.wstr_ptr, attributes_pptr[1], str_len );
    temp_ptr->cif_data.version_data.wstr_len = str_len;

    return;
}

/*****************************************************************************/
// 	Description : uint16转换 int32数字 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL int32 Uint16_2_Int32(uint16 const *src_ptr) 
{
    char  num_buf[CIF_XML_MAX_NUM_LEN] = {0};

    if (src_ptr == PNULL)
    {
        return 0;
    }

    MMIAPICOM_WstrToStr( src_ptr, (uint8*)num_buf );

    return atoi((const char*)num_buf);
}

/*****************************************************************************/
// 	Description : Get Cif String content
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetCifStringAttribs(
                                  MMI_XML_PARSE_TEMP_T* temp_ptr,
                                  const uint16 **attributes_pptr
                                  )
{
    MMI_OBJECT_T *text_ptr = PNULL;

    if (attributes_pptr == PNULL || *attributes_pptr == PNULL)
    {
        return;
    }

    text_ptr = SCI_ALLOC_APP(sizeof(MMI_OBJECT_T));
    SCI_MEMSET(text_ptr, 0, sizeof(MMI_OBJECT_T));

    // id
    text_ptr->id = Uint16_2_Int32(attributes_pptr[1]);

    // 解析内容时使用
    temp_ptr->cur_text_ptr = (MMI_OBJECT_T*)text_ptr;
    if ( temp_ptr->cif_data.cif_str_data.text_list_ptr == PNULL)
    {
        temp_ptr->cif_data.cif_str_data.text_list_ptr = MMILINK_CreateHead((uint32)text_ptr);
    }
    else
    {
        MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)text_ptr);
        MMILINK_AddNodeBeforeBaseNode( temp_ptr->cif_data.cif_str_data.text_list_ptr, tmp_node_ptr);
    }

    return;
}

/*****************************************************************************/
// 	Description : Get Cif Object content
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetCifObjectAttribs(
                                  MMI_XML_PARSE_TEMP_T* temp_ptr,
                                  const uint16 **attributes_pptr
                                  )
{
    MMI_OBJECT_T *text_ptr = PNULL;

    if (attributes_pptr == PNULL || *attributes_pptr == PNULL)
    {
        return;
    }

    text_ptr = SCI_ALLOC_APP(sizeof(MMI_OBJECT_T));
    SCI_MEMSET(text_ptr, 0, sizeof(MMI_OBJECT_T));

    // id
    text_ptr->id = Uint16_2_Int32(attributes_pptr[1]);

    if (PNULL != attributes_pptr[4] && PNULL != attributes_pptr[5])
    {
        text_ptr->pad = XmlGetObjectEncodeType(attributes_pptr[5]);
    }
    
    // 解析内容时使用
    temp_ptr->cur_text_ptr = (MMI_OBJECT_T*)text_ptr;
    if ( temp_ptr->cif_data.cif_obj_data.text_list_ptr == PNULL)
    {
        temp_ptr->cif_data.cif_obj_data.text_list_ptr = MMILINK_CreateHead((uint32)text_ptr);
    }
    else
    {
        MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)text_ptr);
        MMILINK_AddNodeBeforeBaseNode( temp_ptr->cif_data.cif_obj_data.text_list_ptr, tmp_node_ptr);
    }

    return;
}

/*****************************************************************************/
// 	Description : uint16转换 int32数字
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL int32 GetX16ToInt32(uint16 const *src_ptr) 
{
	int32 nLength		= 0;
	int32 i             = 0;
    int32 num           = 0;
    int32 out_num       = 0;
    char  num_buf[CIF_XML_MAX_NUM_LEN + 1] = {0};

    SCI_ASSERT(src_ptr != PNULL);/*assert verified*/

    MMIAPICOM_WstrToStr( src_ptr, (uint8*)num_buf );
    nLength = SCI_STRLEN(num_buf);

    // 从2开始，0和1分别存0和x
    for (i = 2; i < nLength; i++)
    {
        if (num_buf[i] == 'F' || num_buf[i] == 'f')
        {
            num = 15;
        }
        else if(num_buf[i] == 'E' || num_buf[i] == 'e')
        {
            num = 14;
        }
        else if (num_buf[i] == 'D' || num_buf[i] == 'd')
        {
            num = 13;
        }
        else if (num_buf[i] == 'C' || num_buf[i] == 'c')
        {
            num = 12;
        }
        else if (num_buf[i] == 'B' || num_buf[i] == 'b')
        {
            num = 11;
        }
        else if (num_buf[i] == 'A' || num_buf[i] == 'a')
        {
            num = 10;
        }
        else if (num_buf[i] <= '9' || num_buf[i] >= '0')
        {
            num = num_buf[i] - '0';
        }
        else
        {
            SCI_ASSERT(0);/*assert verified*/
        }

        out_num = out_num*16 + num;
    }
    
	return out_num;
}

/*****************************************************************************/
// 	Description : Get Exported class attribs
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetClassAttribs(
                              MMI_XML_PARSE_TEMP_T* temp_ptr,
                              const uint16 **attributes_pptr, 
                              MMI_XML_CLASS_TYPE_E type
                              )
{
    MMI_EXPORTED_CLASS_T *exported_class_ptr = PNULL;
    MMI_IMPORTED_CLASS_T *imported_class_ptr = PNULL;
    
    if (attributes_pptr == PNULL || *attributes_pptr == PNULL)
    {
        return;
    }
    switch(type)
    {
    case XML_EXPORTED_CLASS:
        {
            exported_class_ptr = SCI_ALLOC_APP(sizeof(MMI_EXPORTED_CLASS_T));
            exported_class_ptr->guid = GetX16ToInt32(attributes_pptr[1]);
    
            if ( 0 == MMIAPICOM_WstrStrncmp((const wchar*)attributes_pptr[3], (const uint8*)"Yes", 3))
            {
                exported_class_ptr->is_protected = CAF_EXTENSION_PROTECTED;
            }
            else
            {
                exported_class_ptr->is_protected = CAF_EXTENSION_UNPROTECTED;
            }
            // add to list
            if ( temp_ptr->cif_data.exported_list_ptr == PNULL)
            {
                temp_ptr->cif_data.exported_list_ptr = MMILINK_CreateHead((uint32)exported_class_ptr);
            }
            else
            {
                MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)exported_class_ptr);
                MMILINK_AddNodeBeforeBaseNode(temp_ptr->cif_data.exported_list_ptr, tmp_node_ptr);
            }
        }
    	break;
        
    case XML_IMPORTED_CLASS:
        {
            imported_class_ptr = SCI_ALLOC_APP(sizeof(MMI_EXPORTED_CLASS_T));
            imported_class_ptr->guid = GetX16ToInt32(attributes_pptr[1]);
            // add to list
            if (temp_ptr->cif_data.imported_list_ptr == PNULL)
            {
                temp_ptr->cif_data.imported_list_ptr = MMILINK_CreateHead((uint32)imported_class_ptr);
            }
            else
            {
                MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)imported_class_ptr);
                MMILINK_AddNodeBeforeBaseNode(temp_ptr->cif_data.imported_list_ptr, tmp_node_ptr);
            }
        }
        break;

    default:
        //SCI_TRACE_LOW:"XmlGetClassAttribs current type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODINFO_1213_112_2_18_3_23_14_20,(uint8*)"d", type);
        break;
    }

    return;
}

/*****************************************************************************/
// 	Description : Get Applet Attribs 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlGetAppletAttribs(
                               MMI_XML_PARSE_TEMP_T* temp_ptr,
                               const uint16 **attributes_pptr
                               )
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 k = 0;
    CAF_APP_INFO_T *app_info_ptr = PNULL;

    if (attributes_pptr == PNULL || *attributes_pptr == PNULL)
    {
        return;
    }
    
    app_info_ptr = SCI_ALLOC_APP(sizeof(CAF_APP_INFO_T));
    SCI_MEMSET(app_info_ptr, 0, sizeof(CAF_APP_INFO_T));

    // 目前只用刀下面四个参数
    while( attributes_pptr != PNULL && attributes_pptr[i] )
    {
        for (j = 0; j < XML_APPLET_ABBRIB_MAX; j++)
        {
		    if( 0 == MMIAPICOM_WstrStrncmp((const wchar*)attributes_pptr[i], (const uint8*)s_applet_attrib_arr[j], strlen(s_applet_attrib_arr[j])))
		    {
                switch(j) 
                {
                case XML_APPLET_CLSID:
                    app_info_ptr->guid = GetX16ToInt32(attributes_pptr[i+1]);
                	break;
                case XML_APPLET_TEXTID:
                    app_info_ptr->text_id = Uint16_2_Int32(attributes_pptr[i+1]);
                	break;
                case XML_APPLET_ICONID:
                    app_info_ptr->icon_id = Uint16_2_Int32(attributes_pptr[i+1]);
                	break;
                case XML_APPLET_TYPE:
                    // 设置解析的属性
                    for( k = 0; k < CAF_EXTENSION; k++ )
	                {
		                if( 0 == MMIAPICOM_WstrStrncmp((const wchar*)attributes_pptr[i+1], (const uint8*)s_applet_type_arr[k], strlen( s_applet_type_arr[k] ) )  )
		                {
                            app_info_ptr->type = k;
                            break;
		                }
	                }
                	break;
                default:
                    break;
                }
                break;
		    }
        }   
        i++;
    } 

    if (temp_ptr->cif_data.applet_list_ptr == PNULL)
    {
        temp_ptr->cif_data.applet_list_ptr = MMILINK_CreateHead((uint32)app_info_ptr);
    }
    else
    {
        MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode((uint32)app_info_ptr);
        MMILINK_AddNodeBeforeBaseNode(temp_ptr->cif_data.applet_list_ptr, tmp_node_ptr);
    }

    return;
}

/*****************************************************************************/
// 	Description : to set element's attributes
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlSetElementAttribs(
                                MMI_XML_PARSE_TEMP_T* temp_ptr,
                                MMI_MODINFO_XML_TAG_E tag, 
                                const uint16 **attributes_pptr
                                )
{
    switch(tag)
    {
    case XML_CIFINFO_TAG:
        XmlGetCifVersionAttrib( temp_ptr, attributes_pptr);
    	break;

    case XML_STRINGS_TAG:
        if (attributes_pptr == PNULL || *attributes_pptr == PNULL)
        {
            return;
        }
        temp_ptr->cif_data.cif_str_data.type = XmlGetObjectEncodeType(attributes_pptr[1]);
    	break;

    case XML_STRING_TAG:
        temp_ptr->cif_data.cif_str_data.num++;
        XmlGetCifStringAttribs( temp_ptr, attributes_pptr);
    	break;

    case XML_OBJECTS_TAG:
        if (attributes_pptr == PNULL || *attributes_pptr == PNULL)
        {
            return;
        }
        temp_ptr->cif_data.cif_obj_data.type = XmlGetObjectEncodeType(attributes_pptr[1]);
    	break;

    case XML_OBJECT_TAG:
        temp_ptr->cif_data.cif_obj_data.num++;
        XmlGetCifObjectAttribs( temp_ptr, attributes_pptr);
    	break;

    case XML_EXPORTED_TAG:
        temp_ptr->cur_class_type = XML_EXPORTED_CLASS;
    	break;
        
    case XML_CLASS_TAG:
        XmlGetClassAttribs( temp_ptr, attributes_pptr, temp_ptr->cur_class_type);
    	break;
    case XML_IMPORTED_TAG:
        temp_ptr->cur_class_type = XML_IMPORTED_CLASS;
    	break;
        
    case XML_APPLET_TAG:
        XmlGetAppletAttribs( temp_ptr, attributes_pptr);
    	break;

    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : to set element's attributes
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlSetElementContext(
                                MMI_XML_PARSE_TEMP_T* temp_ptr
                                )
{
    uint32 str_len = 0;
    
    if ( temp_ptr->cur_text_ptr != PNULL)
    {
        // 取得字符串的长度
        str_len = MMIAPICOM_Wstrlen( temp_ptr->text_arr );
        // 设置字符串属性,根据定义为uint16
//        s_cur_text_ptr->content.is_ucs2 = TRUE;
        // 分配内存
        temp_ptr->cur_text_ptr->content.wstr_ptr = SCI_ALLOC_APP((str_len + 1)*sizeof(wchar));
        SCI_MEMSET( temp_ptr->cur_text_ptr->content.wstr_ptr, 0, ((str_len + 1 )*sizeof(wchar)));
        // 取最小值
//         str_len = MIN(str_len*sizeof(uint16), MMI_MAX_TEXT_LEN*2);
//         SCI_MEMCPY(s_cur_text_ptr->content.str_ptr, s_text_arr, str_len);
//         s_cur_text_ptr->content.length = (uint16)(str_len/sizeof(uint16));

        str_len = MIN(str_len, MMI_MAX_TEXT_LEN);
        MMIAPICOM_Wstrncpy( temp_ptr->cur_text_ptr->content.wstr_ptr, temp_ptr->text_arr, str_len );
        temp_ptr->cur_text_ptr->content.wstr_len = str_len;

        //SCI_MEMSET(s_text_arr, 0, sizeof(s_text_arr));
        // 把全局变量置NULL
        temp_ptr->cur_text_ptr = PNULL;
    }
}

/*****************************************************************************/
// 	Description : callback, Start to parse xml element and attributes
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlStartParseElementSax(
                                   void *user_data_ptr,
                                   const uint16 *localname_ptr,
                                   const uint16 **attributes_pptr
                                   )
{
    int i = 0;
    MMI_XML_PARSE_TEMP_T* temp_ptr = (MMI_XML_PARSE_TEMP_T*)user_data_ptr;
    
    SCI_ASSERT( PNULL != temp_ptr );/*assert verified*/

    // 设置解析的属性
    for( i = 0; i < XML_INVALID_TAG; i++ )
	{
		if( 0 == MMIAPICOM_WstrStrncmp((const wchar*)localname_ptr, (const uint8*)s_tag_arr[i], strlen(s_tag_arr[i]) )  )
		{
            temp_ptr->cif_data.cur_tag = i;
            break;
		}
	}

    XmlSetElementAttribs( temp_ptr, temp_ptr->cif_data.cur_tag, attributes_pptr);
}

/*****************************************************************************/
// 	Description : callback, End to parse xml element 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlEndParseElementSax(
                                 void *user_data_ptr,
                                 const uint16 *localname_ptr
                                 )
{
    MMI_XML_PARSE_TEMP_T* temp_ptr = (MMI_XML_PARSE_TEMP_T*)user_data_ptr;

    SCI_ASSERT( PNULL != temp_ptr );/*assert verified*/

    switch( temp_ptr->cif_data.cur_tag)
    {
    case XML_STRING_TAG:
        // 设置元素的内容
        XmlSetElementContext( temp_ptr );
        SCI_MEMSET( temp_ptr->text_arr, 0, sizeof(temp_ptr->text_arr) );
    	break;

    case XML_OBJECT_TAG:
        if ( temp_ptr->cur_text_ptr != PNULL)
        {
//            s_cur_text_ptr->content.is_ucs2 = TRUE;
            // 分配内存
//             s_cur_text_ptr->content.str_ptr = s_obj_data.str_ptr;
//             s_cur_text_ptr->content.length = s_obj_data.length;
            temp_ptr->cur_text_ptr->content.wstr_ptr = temp_ptr->obj_data.wstr_ptr;
            temp_ptr->cur_text_ptr->content.wstr_len = temp_ptr->obj_data.wstr_len;
            SCI_MEMSET(&temp_ptr->obj_data, 0, sizeof(CAF_STRING_T));
            // 把全局变量置NULL，后面解析的时候会继续用到
            temp_ptr->cur_text_ptr = PNULL;
        }
        break;

    case XML_CRC_TAG:
		{	
            CAF_STRING_T string = {0};

//            string.is_ucs2 = TRUE;
            string.wstr_ptr = temp_ptr->text_arr;
            string.wstr_len = MMIAPICOM_Wstrlen( temp_ptr->text_arr );
            temp_ptr->cif_data.crc_data = Uint16_2_Int32( string.wstr_ptr );
            SCI_MEMSET( temp_ptr->text_arr, 0, sizeof(temp_ptr->text_arr) );
		}
		break;

    default:
        break;
    }

    // 解析一个元素结束的时候,状态设置成无效
    temp_ptr->cif_data.cur_tag = XML_INVALID_TAG;
}

/************************************************************************/
/* 获取字符对应的实际数据                                               */
/************************************************************************/
LOCAL uint32 GetNumOfChar(char nChar)
{
    uint32 num = 0;
    if (nChar >= '0' && nChar <= '9')
    {
        num = nChar - '0';
    }
    else if (nChar == 'F' || nChar == 'f')
    {
        num = 15;
    }
    else if(nChar == 'E' || nChar == 'e')
    {
        num = 14;
    }
    else if (nChar == 'D' || nChar == 'd')
    {
        num = 13;
    }
    else if (nChar == 'C' || nChar == 'c')
    {
        num = 12;
    }
    else if (nChar == 'B' || nChar == 'b')
    {
        num = 11;
    }
    else if (nChar == 'A' || nChar == 'a')
    {
        num = 10;
    }
    else
    {
        num = 0; // 如果不在这个范围之内，则num=0，如果走到这一步，那么转化出来的数据就已经是错误的了
    }

    return num;
}

/************************************************************************/
/* 把ucs2的数据转化成bmp数据。                                          */
/************************************************************************/
LOCAL BOOLEAN XmlConvertUcs2BufToBmpData(
                                         const uint8 *data_ptr, 
                                         uint32 indata_len, 
                                         char *bmp_ptr, 
                                         uint32 outdata_len
                                         )
{
    uint32 i = 0;
    uint32 j = 0;
    uint32 num1 = 0;
    uint32 num2 = 0;

    if (PNULL == data_ptr || PNULL == bmp_ptr)
    {
        return FALSE;
    }
#ifdef WIN32
    i = 0;
#else
    i = 1;
#endif
    for (; i < indata_len; i++)
    {
        num1 = GetNumOfChar(data_ptr[i]); // 获取first字节
        i += 2; // 跳过second字节
        if (i < indata_len && j < outdata_len)
        {
            num2 = GetNumOfChar(data_ptr[i]); // 获取third字节
            bmp_ptr[j] = ((uint8)num1<<4) + num2; // num1 num2合并为一个字节
        }
        i++; // 跳过fourth字节
        j++;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : callback, to parse xml content of element 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL void XmlParseCharactersSAX(
                                 void *user_data_ptr,
                                 const uint16 *ch_ptr 
                                 )
{
    uint16 len = 0;
    
    MMI_XML_PARSE_TEMP_T* temp_ptr = (MMI_XML_PARSE_TEMP_T*)user_data_ptr;

    SCI_ASSERT( PNULL != temp_ptr );/*assert verified*/

    switch(temp_ptr->cif_data.cur_tag)
    {
    case XML_STRING_TAG:
        MMIAPICOM_Wstrcat( temp_ptr->text_arr, ch_ptr);
    	break;

    case XML_CRC_TAG:
		MMIAPICOM_Wstrcat( temp_ptr->text_arr, ch_ptr);
		break;

    case XML_OBJECT_TAG:
        {
            wchar *data_buf_ptr = PNULL;

            len = MMIAPICOM_Wstrlen(ch_ptr) / sizeof(wchar);

            data_buf_ptr = SCI_ALLOC_APP( ( len + 1 ) * sizeof(wchar) );
            SCI_MEMSET( data_buf_ptr, 0, (( len + 1 ) * sizeof(wchar)) );

            //将uint16压缩成uint8
            XmlConvertUcs2BufToBmpData( (uint8*)ch_ptr, len * sizeof(wchar) * sizeof(wchar), (char *)data_buf_ptr, len * sizeof(wchar) );

            if ( temp_ptr->obj_data.wstr_ptr == PNULL )
            {
                temp_ptr->obj_data.wstr_ptr = data_buf_ptr;
                temp_ptr->obj_data.wstr_len = len;
            }
            else
            {
                wchar *data_ptr = SCI_ALLOC_APP( ( temp_ptr->obj_data.wstr_len + len + 1 ) * sizeof(wchar) );

                SCI_MEMSET( data_ptr, 0, (( temp_ptr->obj_data.wstr_len + len + 1 ) * sizeof(wchar)) );

                //不是string, 所以不能使用wstrncpy
                SCI_MEMCPY( data_ptr, temp_ptr->obj_data.wstr_ptr, temp_ptr->obj_data.wstr_len * sizeof(wchar) );
                
                SCI_MEMCPY( data_ptr + temp_ptr->obj_data.wstr_len, data_buf_ptr, len * sizeof(wchar));

                SCI_FREE( temp_ptr->obj_data.wstr_ptr );

                temp_ptr->obj_data.wstr_ptr = data_ptr;
                temp_ptr->obj_data.wstr_len = temp_ptr->obj_data.wstr_len + len;

                // free
                SCI_FREE(data_buf_ptr);
            }
        }
    	break;

    default:
        //SCI_TRACE_LOW:"XmlParseCharactersSAX current tag %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_MODINFO_1616_112_2_18_3_23_15_21,(uint8*)"d", temp_ptr->cif_data.cur_tag);
        break;
    }
    
    return;
}

/*****************************************************************************/
// 	Description : Free the struct of MMI_OBJECT_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void FreeObjectNode(MMI_LINK_NODE_T const * node_ptr)
{
    MMI_OBJECT_T *obj_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }
    obj_ptr = (MMI_OBJECT_T*)node_ptr->data;

    if (obj_ptr != PNULL)
    {
        SCI_FREE(obj_ptr->name);
        SCI_FREE(obj_ptr->content.wstr_ptr);
        SCI_FREE(obj_ptr);
    }
}

/*****************************************************************************/
// 	Description : Free the struct of MMI_OBJECT_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void FreeExportedNode(MMI_LINK_NODE_T const * node_ptr)
{
    MMI_EXPORTED_CLASS_T *obj_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }
    obj_ptr = (MMI_EXPORTED_CLASS_T*)node_ptr->data;

    if (obj_ptr != PNULL)
    {
        SCI_FREE(obj_ptr);
    }
}

/*****************************************************************************/
// 	Description : Free the struct of MMI_OBJECT_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void FreeImportedNode(MMI_LINK_NODE_T const * node_ptr)
{
    MMI_IMPORTED_CLASS_T *obj_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }
    obj_ptr = (MMI_IMPORTED_CLASS_T*)node_ptr->data;

    if (obj_ptr != PNULL)
    {
        SCI_FREE(obj_ptr);
    }
}

/*****************************************************************************/
// 	Description : Free the struct of MMI_OBJECT_T
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void FreeAppInfoNode(MMI_LINK_NODE_T const * node_ptr)
{
    CAF_APP_INFO_T *obj_ptr = PNULL;

    if (node_ptr == PNULL)
    {
        return;
    }
    obj_ptr = (CAF_APP_INFO_T*)node_ptr->data;

    if (obj_ptr != PNULL)
    {
        SCI_FREE(obj_ptr);
    }
}

/*****************************************************************************/
// 	Description : Release the struct of MMI_CIF_STRUCT_T 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL void ReleaseCifInfo(MMI_CIF_STRUCT_T cif_data)
{
    if (PNULL != cif_data.version_data.wstr_ptr)
    {
        SCI_FREE(cif_data.version_data.wstr_ptr);
    }
    if (PNULL != cif_data.cif_str_data.text_list_ptr)
    {
        MMILINK_DestroyLink(cif_data.cif_str_data.text_list_ptr, FreeObjectNode);
    }
    if (PNULL != cif_data.cif_obj_data.text_list_ptr)
    {
        MMILINK_DestroyLink(cif_data.cif_obj_data.text_list_ptr, FreeObjectNode);
    }
    if (cif_data.exported_list_ptr != PNULL)
    {
        MMILINK_DestroyLink(cif_data.exported_list_ptr, FreeExportedNode);
    }
    if (cif_data.imported_list_ptr != PNULL)
    {
        MMILINK_DestroyLink(cif_data.imported_list_ptr, FreeImportedNode);
    }
    if (cif_data.applet_list_ptr != PNULL)
    {
        MMILINK_DestroyLink(cif_data.applet_list_ptr, FreeAppInfoNode);
    }
//     if (cif_data.crc_data.str_ptr != PNULL)
// 	{
// 		SCI_FREE(cif_data.crc_data.str_ptr);
// 	}

}

/*****************************************************************************/
// 	Description : 查找字符串 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL MMI_LINK_NODE_T * FindFileNameInStrList(
                                              MMI_LINK_NODE_T *head_node_ptr,
                                              uint32 id
                                              )
{
    BOOLEAN is_find = FALSE;
    MMI_LINK_NODE_T *obj_ptr = PNULL;

    obj_ptr = head_node_ptr;
    if (head_node_ptr == PNULL)
    {
        return PNULL;
    }
    
    do {
    	if (((MMI_OBJECT_T*)obj_ptr->data)->id == id)
    	{
            is_find = TRUE;
            break;
    	}
        obj_ptr = obj_ptr->next_ptr;
    } while(obj_ptr != head_node_ptr);

    if (is_find)
    {
        return obj_ptr;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
// 	Description : 根据解析xml所得数据,转化成CAF_MODULE_INFO_T结构 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN XmlGetCafInformation(
                                   const MMI_CIF_STRUCT_T *xml_data_ptr,
                                   CAF_MODULE_INFO_T *cif_data_ptr,
                                   uint16 *file_name_ptr
                                   )
{
    BOOLEAN result = FALSE;
    uint16 file_name_len = 0;
    uint16 file_path_len = 0;
    uint16 file_path_name[CAF_FILENAME_MAX_LEN + 1] = {0};
    MMI_OBJECT_T *obj_ptr = PNULL;
    MMI_LINK_NODE_T *list_node_ptr = PNULL;
    
    if (PNULL == xml_data_ptr || PNULL == cif_data_ptr)
    {
        return result;
    }

    file_name_len = (uint16)MMIAPICOM_Wstrlen(file_name_ptr);
    MMIAPICOM_GetFilePath(file_name_ptr, file_name_len, file_path_name, &file_path_len);

    list_node_ptr = FindFileNameInStrList(xml_data_ptr->cif_str_data.text_list_ptr, CIF_FILE_NAME_ID);
        
    if ( PNULL != list_node_ptr 
        && PNULL != ( obj_ptr = (MMI_OBJECT_T*)list_node_ptr->data ) )
    {
        MMIAPICOM_CombinePath(file_path_name, file_path_len, (uint16*)obj_ptr->content.wstr_ptr, (uint16)obj_ptr->content.wstr_len, 
            FALSE, cif_data_ptr->cif_file_name, &file_name_len);   
    }

    list_node_ptr = FindFileNameInStrList(xml_data_ptr->cif_str_data.text_list_ptr, 
#ifdef WIN32
        WIN_RES_FILE_NAME_ID
#else
        ARM_RES_FILE_NAME_ID
#endif
        );
    
    if ( PNULL != list_node_ptr  
        && PNULL != ( obj_ptr = (MMI_OBJECT_T*)list_node_ptr->data ) )
    {
        MMIAPICOM_CombinePath(file_path_name, file_path_len, (uint16*)obj_ptr->content.wstr_ptr, (uint16)obj_ptr->content.wstr_len, 
                FALSE, cif_data_ptr->res_file_name, &file_name_len);
    }

    list_node_ptr = FindFileNameInStrList(xml_data_ptr->cif_str_data.text_list_ptr, 
#ifdef WIN32
        WIN_DLL_FILE_NAME_ID
#else
        ARM_BIN_FILE_NAME_ID
#endif
        );
    
    if ( PNULL != list_node_ptr  
        && PNULL != ( obj_ptr = (MMI_OBJECT_T*)list_node_ptr->data ) )
    {
        MMIAPICOM_CombinePath(file_path_name, file_path_len, (uint16*)obj_ptr->content.wstr_ptr, (uint16)obj_ptr->content.wstr_len, 
                FALSE, cif_data_ptr->dll_file_name, &file_name_len);
    }

    // version data
    //cif_data_ptr->version_data.is_ucs2 = xml_data_ptr->version_data.is_ucs2;
    cif_data_ptr->version_data = xml_data_ptr->version_data;
    // string data
    cif_data_ptr->cif_str_data.num = xml_data_ptr->cif_str_data.num; // 所有的string数
    cif_data_ptr->cif_str_data.text_list_ptr = xml_data_ptr->cif_str_data.text_list_ptr; // string列表
    cif_data_ptr->cif_str_data.type = xml_data_ptr->cif_str_data.type; // 编码类型
    // object data
    cif_data_ptr->cif_obj_data.num = xml_data_ptr->cif_obj_data.num;
    cif_data_ptr->cif_obj_data.text_list_ptr = xml_data_ptr->cif_obj_data.text_list_ptr;
    cif_data_ptr->cif_obj_data.type = xml_data_ptr->cif_obj_data.type;
    // Applet information
    cif_data_ptr->class_info_ptr = xml_data_ptr->applet_list_ptr;
    cif_data_ptr->class_num = MMILINK_GetListNodesNum(cif_data_ptr->class_info_ptr);
    // exported class information
    cif_data_ptr->exported_list_ptr = xml_data_ptr->exported_list_ptr;
    cif_data_ptr->exp_num = MMILINK_GetListNodesNum(cif_data_ptr->exported_list_ptr);
    // importes class information
    cif_data_ptr->dep_guid_ptr = xml_data_ptr->imported_list_ptr;
    cif_data_ptr->dep_num = MMILINK_GetListNodesNum(cif_data_ptr->dep_guid_ptr);
    
    // crc
// 	cif_data_ptr->crc_data.is_ucs2 = xml_data_ptr->crc_data.is_ucs2;
// 	cif_data_ptr->crc_data.length = xml_data_ptr->crc_data.length;
// 	cif_data_ptr->crc_data.str_ptr = xml_data_ptr->crc_data.str_ptr;
    cif_data_ptr->crc_data = xml_data_ptr->crc_data;
    result = TRUE;

    return result;
}
                                   
/*****************************************************************************/
// 	Description : Read Xml file information function
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL uint8* ReadXmlFileInfo(
                             uint16 *file_name_ptr,
                             uint32 *file_size_out
                             )
{
    SFS_HANDLE sfs_handle = 0;
    uint32 file_size = 0;
    uint32 file_read = 0;
    uint8  *file_data_ptr = PNULL;

    sfs_handle = SFS_CreateFile(file_name_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    if(MOD_CIF_VALID_HANDLE != sfs_handle)
    {
        SFS_GetFileSize(sfs_handle, &file_size);

        // CIF文件的大小大于10k时，不读取该文件
        if (file_size != 0 && MOD_CIF_FILE_MAX_SIZE > file_size)
        {
            file_data_ptr = SCI_ALLOC_APP(file_size);
            SCI_MEMSET(file_data_ptr, 0, file_size);
            SFS_ReadFile(sfs_handle, (void*)file_data_ptr, file_size, &file_read, PNULL);
        }
    }
    SFS_CloseFile(sfs_handle);
    *file_size_out = file_size;

    return (file_data_ptr);
}
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : Parse the caf XML file interface 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CafXmlParser(
                           uint16 *file_name_ptr,
                           MMI_MODINFO_NODE_T *mod_info_ptr
                           )
{
    CFLXML_PARSER_T* parser_ptr = PNULL;
    uint32 file_size = 0;
    uint8  *buffer = PNULL;
    BOOLEAN result = FALSE;
    MMI_XML_PARSE_TEMP_T* temp_ptr = PNULL;
    
#ifdef XML_SUPPORT
    if (file_name_ptr == PNULL || mod_info_ptr == PNULL)
    {
        return FALSE;
    }
    
    buffer = ReadXmlFileInfo(file_name_ptr, &file_size);

    if ((PNULL == buffer) || (0 == file_size) || (MOD_CIF_FILE_MAX_SIZE <= file_size))
    {
        if (buffer != PNULL)
        {
            SCI_FREE(buffer);
        }
        return FALSE;
    }

    temp_ptr = SCI_ALLOC_APP( sizeof(MMI_XML_PARSE_TEMP_T) );
    SCI_MEMSET( temp_ptr, 0, sizeof(MMI_XML_PARSE_TEMP_T) );

    // 创建 解析器
    parser_ptr = CFLXML_CreateParser(XmlStartParseElementSax, XmlEndParseElementSax, XmlParseCharactersSAX, NULL, temp_ptr );

    if( parser_ptr )
    {                
        // 开始解析
        uint32 recode = CFLXML_Parse(parser_ptr, buffer, file_size, 1);
        if (CFLXML_ERR_OK == recode)
        {
            mod_info_ptr->mod_id = GenerateComID();
            mod_info_ptr->mod_type = CAF_MOD_DYNAMIC;
            result = XmlGetCafInformation(&temp_ptr->cif_data, &mod_info_ptr->module_info, file_name_ptr);
            if ( !VersionInfoIsValid( &mod_info_ptr->module_info.version_data ) )
            {
                mod_info_ptr->module_info.flag |= CAF_MOD_FLAG_INVALID_VER;
            }
            
            SCI_MEMSET(&temp_ptr->cif_data, 0, sizeof(MMI_CIF_STRUCT_T)); // 因为已经把数据设置到mod_info_ptr->module_info中了，此处可以初始化s_cif_data
        }
        else
        {
            // free s_cif_data
            ReleaseCifInfo(temp_ptr->cif_data);
        }
        // 解析结束
        CFLXML_FreeParser( parser_ptr );
    }   

    SCI_FREE( temp_ptr );

    SCI_FREE(buffer);
#endif

    return result;
}
/*****************************************************************************/
// 	Description : Parse the caf XML file interface 
//	Global resource dependence : 
//  Author:RiancyZhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_MOD_SETUP_ERR_E MMK_CafGUIDIsValid(uint16 *file_name_ptr)
{
	CFLXML_PARSER_T* parser_ptr = PNULL;
	uint32 file_size = 0;
	uint8  *buffer = PNULL;
	BOOLEAN result = FALSE;
	MMI_XML_PARSE_TEMP_T* temp_ptr = PNULL;
	MMI_MODINFO_NODE_T mod_info = {0};
#ifdef XML_SUPPORT	
	if (file_name_ptr == PNULL)
	{
		return CAF_MOD_SETUP_PARAMERR;
	}
		
	buffer = ReadXmlFileInfo(file_name_ptr, &file_size);
	if ((PNULL == buffer) || (0 == file_size) || (MOD_CIF_FILE_MAX_SIZE <= file_size))
	{
		if (buffer != PNULL)
		{
			SCI_FREE(buffer);
		}
		return CAF_MOD_SETUP_FILEERR;
	}
	
	temp_ptr = SCI_ALLOC_APP( sizeof(MMI_XML_PARSE_TEMP_T) );
	SCI_MEMSET( temp_ptr, 0, sizeof(MMI_XML_PARSE_TEMP_T) );
	// 创建 解析器
	parser_ptr = CFLXML_CreateParser(XmlStartParseElementSax, XmlEndParseElementSax, XmlParseCharactersSAX, NULL, temp_ptr );
	
	if( parser_ptr )
	{				 
		// 开始解析
		uint32 recode = CFLXML_Parse(parser_ptr, buffer, file_size, 1);
		if (CFLXML_ERR_OK == recode)
		{
			//mod_info.mod_id = GenerateComID();
			//mod_info.mod_type = CAF_MOD_DYNAMIC;
			result = XmlGetCafInformation(&temp_ptr->cif_data, &mod_info.module_info, file_name_ptr);
			if ( !VersionInfoIsValid( &mod_info.module_info.version_data ) )
			{
				mod_info.module_info.flag |= CAF_MOD_FLAG_INVALID_VER;
			}
			
			SCI_MEMSET(&temp_ptr->cif_data, 0, sizeof(MMI_CIF_STRUCT_T)); // 因为已经把数据设置到mod_info_ptr->module_info中了，此处可以初始化s_cif_data
		}
		else
		{
			// free s_cif_data
			ReleaseCifInfo(temp_ptr->cif_data);
		}
		// 解析结束
		CFLXML_FreeParser( parser_ptr );
	}	
	
    // 释放内存
    SCI_FREE(temp_ptr);
    SCI_FREE(buffer);
	
	//guid check
	if( NULL !=MMK_SearchModInfo(((CAF_APP_INFO_T*)(mod_info.module_info.class_info_ptr->data))->guid,MMI_QUERY_BY_GUID))
	{
		return CAF_MOD_SETUP_GUIDERR;
	}
	
	return (result?CAF_MOD_SETUP_OK:CAF_MOD_SETUP_XMLERR);
#else
	return result;
#endif		
}
/*****************************************************************************/
// 	Description : Search object by object id
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL MMI_OBJECT_T *SearchObject(
                                 MMI_LINK_NODE_T *head_node_ptr, 
                                 uint32 obj_id
                                 )
{
    BOOLEAN is_find = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;
    MMI_OBJECT_T *ret_ptr = PNULL;

    if (head_node_ptr == PNULL)
    {
        return PNULL;
    }

    node_ptr = head_node_ptr;

    do {
    	if (((MMI_OBJECT_T*)node_ptr->data)->id == obj_id)
    	{
            is_find = TRUE;
            ret_ptr = (MMI_OBJECT_T*)node_ptr->data;
            break;
    	}
        node_ptr = node_ptr->next_ptr;
    } while(node_ptr != head_node_ptr);
    
    return ret_ptr;
}
#endif
// s_versino_info_arr
/*****************************************************************************/
// 	Description : 验证版本信息
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN VersionInfoIsValid( CAF_STRING_T* ver_ptr )
{
    BOOLEAN result  = FALSE;
    uint32  ver_num = 0;
    uint32  i = 0;

    SCI_ASSERT( PNULL != ver_ptr && PNULL != ver_ptr->wstr_ptr );/*assert verified*/

    for (i = 0; i < ver_ptr->wstr_len; i++ )
    {
        if ( ver_ptr->wstr_ptr[i] >='0' && ver_ptr->wstr_ptr[i] <= '9')
        {
            ver_num = ver_num*10 + (ver_ptr->wstr_ptr[i] - '0');
        }
        else
        {
            break;
        }
    }

    //向下兼容, cif的版本低于平台的版本, 允许其运行
    //James, 目前版本控制机制不完善, 暂时改成 == , 避免assert, 完善后改回<=
    if( ver_num == s_versino_info_arr[0] )
    {
        result = TRUE;
    }

    return result;
}
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : Search object by object id
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC MMI_OBJECT_T *MMK_SearchObject(
                                      MMI_LINK_NODE_T *head_node_ptr, 
                                      uint32 obj_id
                                      )
{
    return SearchObject(head_node_ptr, obj_id);
}

/*****************************************************************************/
// 	Description : Parse the caf XML file interface 
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CafXmlParser(
                                uint16 *file_name_ptr,
                                MMI_MODINFO_NODE_T *mod_info_ptr
                                )
{
    return CafXmlParser(file_name_ptr, mod_info_ptr);
}

/*****************************************************************************/
// 	Description : load all files' info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T *MMK_LoadDynamicModInfo(void)
{
    BOOLEAN result = FALSE;
    
    InitDynamicModInfoList();
    // load all cif infomartion
    result = LoadDynamicXmlCIF(&s_dyn_modinfo_list, MOD_CIF_DEVICE_NAME_D, MOD_CIF_DEVICE_NAME_D_LEN );
    result = LoadDynamicXmlCIF(&s_dyn_modinfo_list, MOD_CIF_DEVICE_NAME_E, MOD_CIF_DEVICE_NAME_E_LEN );
    
    if (result)
    {
        return  &s_dyn_modinfo_list;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
// 	Description : check whether file is exist
//	Global resource dependence : 
//  Author:Xiaoqing.Lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsFileExist(
                          const uint16  *full_path_ptr,     //in
                          uint16        full_path_len       //in, 双字节为单位
                          )
{
    BOOLEAN         result = FALSE;
    SFS_HANDLE      sfs_handle = 0;
    SFS_FIND_DATA_T find_data = {0};

    SCI_ASSERT(NULL != full_path_ptr);/*assert verified*/

    sfs_handle = SFS_FindFirstFile(full_path_ptr, &find_data);

    if(MOD_CIF_VALID_HANDLE != sfs_handle)
    {
        if(!(SFS_ATTR_DIR&find_data.attr))
        {
            result = TRUE;
        }
    }

    SFS_FindClose(sfs_handle);

    return result;
}

/*****************************************************************************/
// 	Description : update all files' info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T *MMK_UpdateDynamicModList(void)
{
    BOOLEAN result = FALSE;
    MMI_MODINFO_NODE_T *modinfo_list_ptr = PNULL;
    MMI_MODINFO_NODE_T *pre_list_ptr = PNULL;
    MMI_MODINFO_NODE_T *head_list_ptr = PNULL;
    
    // load all cif infomartion
    result = LoadDynamicXmlCIF(&s_dyn_modinfo_list, MOD_CIF_DEVICE_NAME_D, MOD_CIF_DEVICE_NAME_D_LEN );
    result = LoadDynamicXmlCIF(&s_dyn_modinfo_list, MOD_CIF_DEVICE_NAME_E, MOD_CIF_DEVICE_NAME_E_LEN );
    
    head_list_ptr = s_dyn_modinfo_list.modinfo_list_ptr;
    pre_list_ptr = head_list_ptr;
    modinfo_list_ptr = head_list_ptr;
    
    while(modinfo_list_ptr != PNULL)
    {
        if (!IsFileExist(modinfo_list_ptr->module_info.cif_file_name, (uint16)MMIAPICOM_Wstrlen(modinfo_list_ptr->module_info.cif_file_name))
            ||
            !IsFileExist(modinfo_list_ptr->module_info.dll_file_name, (uint16)MMIAPICOM_Wstrlen(modinfo_list_ptr->module_info.cif_file_name))
            )
        {
            if (modinfo_list_ptr == head_list_ptr) // 删除头
            {
                head_list_ptr = modinfo_list_ptr->next_ptr; // head
                pre_list_ptr = modinfo_list_ptr->next_ptr;  // pre
                FreeModNodeDetailInfo(modinfo_list_ptr);
                SCI_FREE(modinfo_list_ptr);
                s_dyn_modinfo_list.mod_num--;
                modinfo_list_ptr = head_list_ptr;           // cur
            }
            else
            {
                pre_list_ptr->next_ptr = modinfo_list_ptr->next_ptr; // pre->next
                FreeModNodeDetailInfo(modinfo_list_ptr);
                SCI_FREE(modinfo_list_ptr);
                s_dyn_modinfo_list.mod_num--;
                modinfo_list_ptr = pre_list_ptr->next_ptr; // cur
            }
            
        }
        else
        {
            pre_list_ptr = modinfo_list_ptr;
            modinfo_list_ptr = modinfo_list_ptr->next_ptr;
        }
    }

    s_dyn_modinfo_list.modinfo_list_ptr = head_list_ptr;
    
    if (result)
    {
        return  &s_dyn_modinfo_list;
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
// 	Description : Get the point of s_dyn_modinfo_list
//	Global resource dependence : 
//  Author:peng.chen
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T * MMK_GetDynModInfoList(void)
{
	return &s_dyn_modinfo_list;
}
#endif
/*****************************************************************************/
// 	Description : Get the point of s_static_modinfo_list
//	Global resource dependence : 
//  Author:peng.chen
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_LIST_T * MMK_GetStaticModInfoList(void)
{
    return &s_static_modinfo_list;
}
/*****************************************************************************/
// 	Description : load file info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC MMI_MODINFO_NODE_T *MMK_SearchModInfo (
                                              uint32 id,
                                              MMI_MODINFO_QUERY_TYPE_E type
                                              )
{
    MMI_MODINFO_NODE_T *mod_info_ptr = PNULL;
    
    switch(type)
    {
    case MMI_QUERY_BY_MODID:
        // 首先在静态列表中搜索
        mod_info_ptr = SearchModInfoByModId(&s_static_modinfo_list, id);
#ifdef DYNAMIC_MODULE_SUPPORT
        if (PNULL == mod_info_ptr)
        {
            // 在动态列表中搜索
            mod_info_ptr = SearchModInfoByModId(&s_dyn_modinfo_list, id);
        }
#endif
        break;
    case MMI_QUERY_BY_GUID:
        // 首先在静态列表中搜索
        mod_info_ptr = SearchModInfoByGuid(&s_static_modinfo_list, id);
#ifdef DYNAMIC_MODULE_SUPPORT
        if (PNULL == mod_info_ptr)
        {
            // 在动态连表中搜索
            mod_info_ptr = SearchModInfoByGuid(&s_dyn_modinfo_list, id);
        }
#endif
        break;
    default:
        //SCI_ASSERT(0);
        break;
    }

    return mod_info_ptr;
}

/*****************************************************************************/
// 	Description : free module information node
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN FreeModNodeDetailInfo(
                                    MMI_MODINFO_NODE_T *node_ptr
                                    )
{
    BOOLEAN result = TRUE;

    if (node_ptr != PNULL)
    {        
        if (node_ptr->module_info.version_data.wstr_ptr != PNULL)
        {
            SCI_FREE(node_ptr->module_info.version_data.wstr_ptr);
        }
        if (node_ptr->module_info.cif_str_data.text_list_ptr != PNULL)
        {
            MMILINK_DestroyLink(node_ptr->module_info.cif_str_data.text_list_ptr, FreeObjectNode);
        }
        if (node_ptr->module_info.cif_obj_data.text_list_ptr != PNULL)
        {
            MMILINK_DestroyLink(node_ptr->module_info.cif_obj_data.text_list_ptr, FreeObjectNode);
        }
        if (node_ptr->module_info.class_info_ptr != PNULL)
        {
            MMILINK_DestroyLink(node_ptr->module_info.class_info_ptr, NULL);
        }
        if (node_ptr->module_info.dep_guid_ptr != PNULL)
        {
            MMILINK_DestroyLink(node_ptr->module_info.dep_guid_ptr, NULL);
        }
        if (node_ptr->module_info.exported_list_ptr != PNULL)
        {
            MMILINK_DestroyLink(node_ptr->module_info.exported_list_ptr, NULL);
        }
//         if (node_ptr->module_info.crc_data.str_ptr != PNULL)
//         {
//             SCI_FREE(node_ptr->module_info.crc_data.str_ptr);
//         }
    }

    return result;
}
#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : init list 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitDynamicModInfoList(void)
{
    if (DeleteAllModInfo(&s_dyn_modinfo_list))
    {
        SCI_MEMSET(&s_dyn_modinfo_list, 0, sizeof(s_dyn_modinfo_list));
    }
    return TRUE;
}
#endif
/*****************************************************************************/
// 	Description : init list 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitStaticModInfoList(void)
{
    if (DeleteAllModInfo(&s_static_modinfo_list))
    {
        SCI_MEMSET(&s_static_modinfo_list, 0, sizeof(s_static_modinfo_list));
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : delete all module information and module information list 
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteAllModInfo (
                                MMI_MODINFO_LIST_T* modinfo_list_ptr
                                )
{
    BOOLEAN result = FALSE;
    uint32 i = 0;
    uint32 mod_num = 0;
    MMI_MODINFO_NODE_T *node_ptr = PNULL;
    MMI_MODINFO_NODE_T *cur_node_ptr = PNULL;

    if (modinfo_list_ptr == PNULL)
    {
        return result;
    }

    mod_num = modinfo_list_ptr->mod_num;
    node_ptr = modinfo_list_ptr->modinfo_list_ptr;

    for (i = 0; ((i < mod_num) && (node_ptr != PNULL)); i++)
    {
        cur_node_ptr = node_ptr;
        node_ptr = node_ptr->next_ptr;
        FreeModNodeDetailInfo(cur_node_ptr);
        SCI_FREE(cur_node_ptr);
        cur_node_ptr = PNULL;
        result = TRUE;
    }
    modinfo_list_ptr->mod_num = 0;

    return (result);
}

#ifdef DYNAMIC_MODULE_SUPPORT
/*****************************************************************************/
// 	Description : delete module information from list by module id
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN DeleteModInfo (
                             MMI_MODINFO_LIST_T *modinfo_list_ptr,
                             uint32 mod_id
                             )
{
    BOOLEAN result = FALSE;
    uint16 i = 0;
    MMI_MODINFO_NODE_T *headnode_ptr = PNULL;
    MMI_MODINFO_NODE_T *pre_node_ptr = PNULL;
    MMI_MODINFO_NODE_T *nxtnode_ptr = PNULL;
    MMI_MODINFO_NODE_T *delnode_ptr = PNULL;

    if (PNULL == modinfo_list_ptr) // check param pointer
    {
        return result;
    }

    headnode_ptr = modinfo_list_ptr->modinfo_list_ptr;
    pre_node_ptr = modinfo_list_ptr->modinfo_list_ptr;
    nxtnode_ptr = modinfo_list_ptr->modinfo_list_ptr;
    for (i = 0; ((i < modinfo_list_ptr->mod_num) && (PNULL != nxtnode_ptr)); i++)
    {
        if ((nxtnode_ptr != NULL) && (nxtnode_ptr->mod_id == mod_id)) /*lint !e774 */
        {
            if (nxtnode_ptr == modinfo_list_ptr->modinfo_list_ptr) // delete head pointer
            {
                delnode_ptr = modinfo_list_ptr->modinfo_list_ptr;
                headnode_ptr = headnode_ptr->next_ptr;
            }
            else if(i == modinfo_list_ptr->mod_num - 1) // tail pointer
            {
                delnode_ptr = nxtnode_ptr;
                pre_node_ptr->next_ptr = PNULL;
            }
            else // middle pointer
            {
                delnode_ptr = nxtnode_ptr;
                pre_node_ptr->next_ptr = nxtnode_ptr->next_ptr;
            }

            FreeModNodeDetailInfo(delnode_ptr);
            SCI_FREE(delnode_ptr);
            delnode_ptr = PNULL;
            result = TRUE;
            modinfo_list_ptr->mod_num--;
            break;
        }
        pre_node_ptr = nxtnode_ptr;

        if (PNULL != nxtnode_ptr) /*lint !e774 */
        {
            nxtnode_ptr = nxtnode_ptr->next_ptr;
        }
    }

    modinfo_list_ptr->modinfo_list_ptr = headnode_ptr;

    return result;
}

/*****************************************************************************/
// 	Description : unload file's info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMK_UnLoadDynModInfo(uint32 mod_id)
{
    return DeleteModInfo(&s_dyn_modinfo_list, mod_id);
}

/*****************************************************************************/
// 	Description : unload all files' info
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMK_UnLoadAllDynModInfo(void)
{
    if (DeleteAllModInfo(&s_dyn_modinfo_list))
    {
        SCI_MEMSET(&s_dyn_modinfo_list, 0, sizeof(s_dyn_modinfo_list));
    }
    return TRUE;
}
#endif
/*****************************************************************************/
// 	Description : search mod info by cif name
//	Global resource dependence : 
//  Author:Xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL MMI_MODINFO_NODE_T *SearchModInfoByCifName (
                                                  MMI_MODINFO_LIST_T *modinfo_list_ptr,
                                                  uint16 *cif_file_name
                                                  )
{
    BOOLEAN result = FALSE;
    uint16 i = 0;
    MMI_MODINFO_NODE_T *node_ptr = PNULL;
    MMI_MODINFO_NODE_T *find_node_ptr = PNULL;

    SCI_ASSERT(PNULL != modinfo_list_ptr);/*assert verified*/
    
    node_ptr = modinfo_list_ptr->modinfo_list_ptr;
    find_node_ptr = PNULL;

    for (i = 0; ((i < modinfo_list_ptr->mod_num) && (PNULL != node_ptr)); i++)
    {
        if ((node_ptr != NULL) && (MMIAPICOM_Wstrcmp(node_ptr->module_info.cif_file_name, cif_file_name) == 0)) /*lint !e774 */
        {
            find_node_ptr = node_ptr;
            result = TRUE;
            break;
        }
        if (node_ptr != PNULL) /*lint !e774 */
        {
            node_ptr = node_ptr->next_ptr;
        }
    }
    
    return find_node_ptr;
}


/*****************************************************************************/
// 	Description : search applet type
//	Global resource dependence : 
//  Author:Andrew.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetAppletType(CAF_GUID_T guid, CAF_APPLET_TYPE_E *applet_type_ptr)
{
    BOOLEAN result = FALSE;
    MMI_MODINFO_NODE_T *modinfo_node_ptr = PNULL;
    
    modinfo_node_ptr = MMK_SearchModInfo (guid, MMI_QUERY_BY_GUID);
    
    if(modinfo_node_ptr == PNULL)
    {
        return result;
    }
    
    if(modinfo_node_ptr->mod_type == CAF_MOD_DYNAMIC) //只有动态模块才有类型
    {
        MMI_LINK_NODE_T *node_ptr = PNULL;
        
        node_ptr = modinfo_node_ptr->module_info.class_info_ptr;
        
        if (node_ptr != PNULL)
        {
            do {
                if(((CAF_APP_INFO_T*)(modinfo_node_ptr->module_info.class_info_ptr->data))->guid == guid)
                {
                    *applet_type_ptr = (CAF_APPLET_TYPE_E)(((CAF_APP_INFO_T*)(modinfo_node_ptr->module_info.class_info_ptr->data))->type);
                    result = TRUE;
                    break;
                }
                node_ptr = node_ptr->next_ptr;
                
            } while(node_ptr != modinfo_node_ptr->module_info.class_info_ptr);
        }
    }
    
    return result;
}

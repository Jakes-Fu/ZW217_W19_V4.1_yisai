/*****************************************************************************
** File Name:      mmidcd_file.c                                              *
** Author:                                                                   *
** Date:           10/2008                                                   *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mp3 player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2008        byte.guo          Create                                   *
******************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_dcd_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
//#include <stdlib.h>
//#include <string.h>
//#include "sci_types.h"
#include "mmifmm_export.h"
#include "os_api.h"
#include "Guifont.h"
#include "mmisd_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "gui_ucs2b_converter.h"
#ifdef MMI_ENABLE_DCD	//!�궨�忪ͷ
#include "mmi_appmsg.h"
#include "mmidcd_export.h"
#include "mmidcd_main.h"
#include "mmidcd_file.h"

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
// jun.hu 2009/12/18 for ��DCD�ļ�����Ϊϵͳ�ļ����еĲ�����������....
#define MMIDCD_DEFAULT_DIR_LEN             3 
#define MMIDCD_DEFAULT_DIR                (s_mmidcd_dir_wstr) 
#define MMIDCD_PICTURE_DIR_LEN            12     
#define MMIDCD_PICTURE_DEFAULT_DIR        (s_mmidcd_picture_dir_wstr)
#define MMIDCD_LOCAL_PICTURE_DIR_LEN      13     
#define MMIDCD_LOCAL_PICTURE_DEFAULT_DIR  (s_mmidcd_lpicture_dir_wstr) 
#define MMIDCD_INDEX_NAME_LEN              9      
#define MMIDCD_INDEX_NAME                 (s_mmidcd_index_file_name)  
#define MMIDCD_NOD_NAME_LEN                9      
#define MMIDCD_NOD_NAME                   (s_mmidcd_nod_file_name)  
#define MMIDCD_OLD_NAME_LEN                7      
#define MMIDCD_OLD_NAME                   (s_mmidcd_old_file_name)
#define MMIDCD_LOCAL_NAME_LEN              9      
#define MMIDCD_LOCAL_NAME                 (s_mmidcd_local_file_name)

#define MMIDCD_PURE_PICTURE_DIR_LEN            11     // ����û���̷���·������ 
#define MMIDCD_PURE_LOCAL_PICTURE_DIR_LEN      12     // ����û���̷���·������ 
        
// Ĭ�ϵ�DCD����Ŀ¼
static const wchar s_mmidcd_dir_wstr[]          = L"DCD";
// index.xml
static const wchar s_mmidcd_index_file_name[]   = L"index.xml";
// index.nod
static const wchar s_mmidcd_nod_file_name[]     = L"index.nod";
// old.xml
static const wchar s_mmidcd_old_file_name[]     = L"old.xml";
// local.xml
static const wchar s_mmidcd_local_file_name[]   = L"local.xml";
// dcd ����ͼƬ��Ŀ¼
static const wchar s_mmidcd_picture_dir_wstr[]  = L"DCD\\picture\\";
// dcd ���ر�����Ŀ��ͼƬ�ı���Ŀ¼
static const wchar s_mmidcd_lpicture_dir_wstr[] = L"DCD\\lpicture\\";
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
 **                          TYPE AND CONSTANT                               *
 **--------------------------------------------------------------------------*/
const uint8 g_mmidcd_update_info_file_path[] = {"D:\\DCD\\dcd_info.txt"};
/*
dcd default xml file
ttl: 0 ; disable ttl update
*/
const char default_dcd_xml_chinese[] =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<sync-pkg>\
  <meta>\
    <generator url=\"http:////dcd.monternet.com:80/dp\" version=\"S3_CMCC_1.2\">Dynamic Portal Media Gateway</generator>\
    <global-meta anchor=\"globalanchor\" ttl=\"00000\" server-time=\"1197034234\" />\
    <feed-meta ctxt-id=\"y7dspk9gcwv\" anchor=\"a7h0.f9wdacyp.xf.0.jaqpf6y0.8\" target=\"container1\" />\
  </meta>\
  <feed ctxt-id=\"y7dspk9g1bp\" entry-type=\"cs\" template-id=\"default\">\
    <title>﻿欢迎使用快讯 </title>\
    <summary>服务说明\
    </summary>\
    <link rel=\"alternate\" href=\"http:////wapnews.i139.cn/dcd/dcd.do?j=queryTempl&amp;f=1107&amp;n=channelPara\" type=\"application/xhtml+xml\" />\
    <entry ctxt-id=\"y7dspk9g2gh\" entry-type=\"cs\" template-id=\"default\">\
      <title>服务说明 \
      </title>\
      <summary>“快讯”是中国移动提供的一项资讯类业务，提供该服务的手机可以实时接收快讯的最新资讯并在待机屏幕上滚动显示。使用快讯业务时，浏览资讯的标题和摘要在手机终端本地就可以完成，浏览资讯全文需要连接服务器在WAP页面上完成。  \
  </summary>\
      <link rel=\"alternate\" href=\"http://dcd.monternet.com/service/help/help.im\" type=\"application/xhtml+xml\" />\
      <issued>1196981880000</issued>\
      <author>\
        <name>unknown</name>\
      </author>\
    </entry>\
  </feed>\
</sync-pkg>";
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
//extern int stricmp(char * s1, char * s2);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*the flag for dcd item state arr ..zhuo wang*/
static MMIDCD_ITEM_FLAG s_xml_node_state[MMIDCD_MAX_PAGE_NUM][MMIDCD_MAX_ITEM_NUM_PER_PAGE] = {0};

static uint16  s_new_items_num = 0; //jun.hu for dcd���µ��µ���Ŀ��.....
/********************************************************************************
 NAME:			�������ֵ�����
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhuo.wang
 DATE:			
********************************************************************************/
LOCAL uint32 DCD_calexp(uint32 num);
/********************************************************************************
 NAME:			�ַ���ת��Ϊ����
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhuo.wang
 DATE:			
********************************************************************************/
LOCAL uint32 DCDConvertString2Digit(
                                 uint8 *str_ptr,    //in
                                 uint16 length  //in
                                 );

/********************************************************************************
 NAME:			����ת��Ϊ�ַ���
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhuo.wang
 DATE:			
********************************************************************************/
LOCAL void DCDConvertDigit2String(
                               uint8 *str_ptr,        //out
                               uint16 *length_ptr,    //out
                               uint32 num //in
                               );
/*****************************************************************************/
//  Description : get dcd index file full path for read
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/12/18
/*****************************************************************************/
LOCAL const wchar * GetDCDFileName( 
                                   uint16 *  filename_len_ptr,     //[OUT]
                                   MMIDCD_FILE_PATH_TYPE_E file_path_type
                                   );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author: 
//	Note: jun.hu 2009/10/13 �޸�,�����õ���wchar�͵�
/*****************************************************************************/
LOCAL uint16 MMIDCD_CombineFileName(
                                    wchar* file_full_name_ptr, // out 
                                    const char*  file_name_ptr,
                                    const char*  file_extend_name_ptr
                                    );
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note: modified by jun.hu 2009/12/18 for ȥ�������Ĳ�����ֱ���ں����ڲ���ȡ�ļ���
/*****************************************************************************/
LOCAL void SaveAllItemsState(void);
/*****************************************************************************/
//  Description : ȥ�����Ƿ��ѱ����˸�Ƶ����.....
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/10
/*****************************************************************************/
LOCAL BOOLEAN hasSavedTheSameDCDItem(char * content_id_ptr);
/*****************************************************************************/
// 	Description :��DCD��ĳһ��ITEM����ȡ����Ϣд�����ر���...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
LOCAL BOOLEAN WriteItemTextToLocal(MMIDCD_LOCAL_FILE_NODE_T save_item);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN InitalizeLocalFileHeaderInfo(MMIDCD_LOCAL_FILE_HEADER_T * hear_ptr,
										   MMIFILE_HANDLE file_handle
										   );
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : ��ʼ��s_xml_node_state������0 node state
// Global resource dependence : None
// Author: jun.hu , 2009/8/27
// Note: 
/*****************************************************************************/
PUBLIC void MMIDCD_Init_XML_Node_State(void)
{
    SCI_MEMSET(s_xml_node_state, 0, sizeof(s_xml_node_state));
    // ����ֱ���������������д�ļ�......
    SaveAllItemsState();
}
/*****************************************************************************/
// Description : �õ�ָ��λ�õ�ITEM �Ƿ��Ѷ�
// Global resource dependence : None
// Author: jun.hu , 2009/8/27
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_GetItemAlreadyRead(uint16 pageIndex, uint16 itemIndex)
{
    if(MMIDCD_MAX_PAGE_NUM > pageIndex && MMIDCD_MAX_ITEM_NUM_PER_PAGE > itemIndex)
    {
        return s_xml_node_state[pageIndex][itemIndex].is_readed;
    }
    else
    {
        //SCI_TRACE_MID:"MMIDCD pageIndex = %d, item index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_214_112_2_18_2_12_41_3,(uint8*)"dd", pageIndex, itemIndex);
        return FALSE;
    }
}
/*****************************************************************************/
// Description : ����ָ��λ�õ�ITEM �Ѷ��ı��
// Global resource dependence : None
// Author: jun.hu 
// Note:  2009/10/13 
/*****************************************************************************/
PUBLIC void MMIDCD_SetItemAlreadyRead(
                                      uint16 pageIndex, 
                                      uint16 itemIndex
                                      )
{
    if(MMIDCD_MAX_PAGE_NUM > pageIndex && MMIDCD_MAX_ITEM_NUM_PER_PAGE > itemIndex)
    {
        s_xml_node_state[pageIndex][itemIndex].is_readed = TRUE;
        SaveAllItemsState();
    }
    else
    {
        //SCI_TRACE_MID:"MMIDCD SetItemAlreadyRead pageIndex = %d, item index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_234_112_2_18_2_12_41_4,(uint8*)"dd", pageIndex, itemIndex);
    }
}
/*****************************************************************************/
// Description : ��ȡ��¼�ĸ��µ�Ƶ������....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetUpDateNewItemsCount(void)
{
    return  s_new_items_num;
}
/*****************************************************************************/
// Description : ���ü�¼�ĸ��µ�Ƶ������....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC void MMIDCDFILE_SetUpDateNewItemsCount(uint16 count)
{
     s_new_items_num = count;
}
/*****************************************************************************/
// Description : ��ȡxml�ļ����ܵ�Ƶ������....
// Global resource dependence : None
// Author: jun.hu 
// Note: 2009/12/2
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetXMLItemsCount(xmlDocPtr xml_ptr)
{
    uint16 items_count = 0;
    uint16 xml_feed_num = 0;       // �ļ��е�Ƶ����
    uint16 feed_item_count = 0;    // Ƶ���е�Ƶ������
    uint16 feed_index = 0;
    if(PNULL != xml_ptr)
    {
        // �õ��ļ���Ƶ����
        xml_feed_num = MMIDCD_GetFeedNum(xml_ptr);
        for(feed_index = 0; feed_index < xml_feed_num; feed_index++)
        {
             feed_item_count = MMIDCD_GetEntryNumInFeed(xml_ptr, feed_index);
             items_count+=feed_item_count;
        }
    }
    else
    {
        //SCI_TRACE_MID:"MMIDCD GetXMLItemsCountPNULL != xml_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_275_112_2_18_2_12_41_5,(uint8*)"");
    }

    return items_count;
}
/*****************************************************************************/
// Description : ����ĳһƵ��ID�ڸ������ļ����Ƿ����....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_HasFeedIdInXmlFile(
                                             xmlDocPtr search_xml_ptr,
                                             const char * feedId_ptr,
                                             uint16 * find_feedIndex_ptr // out �����ҵ���feedindex
                                             )
{
    uint16 xml_feed_num = 0; // �ļ��е�Ƶ����
    uint16 feedindex = 0;    // �ļ��е�Ƶ�����ļ��е�Ƶ������
    char * xml_feedId_ptr = PNULL; // �ļ���Ƶ��idָ��
    BOOLEAN findFlag = FALSE;
    
    if(PNULL == feedId_ptr || PNULL == search_xml_ptr || PNULL == find_feedIndex_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:MMIDCDFILE_HasFeedIdInXmlFile ptr PNULL error!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_297_112_2_18_2_12_41_6,(uint8*)"");
        return FALSE;
    }
    
    // �õ��ļ���Ƶ����
    xml_feed_num = MMIDCD_GetFeedNum(search_xml_ptr);
    //SCI_TRACE_MID:"[MMIDCD] xml_feed_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_303_112_2_18_2_12_41_7,(uint8*)"d", xml_feed_num);
    for(feedindex = 0; feedindex < xml_feed_num; feedindex++)
    {
         xml_feedId_ptr = MMIDCD_GetFeedCTXTId(search_xml_ptr, feedindex);
         if(0 == strcmp(feedId_ptr, xml_feedId_ptr))
         {
            findFlag = TRUE;
            *find_feedIndex_ptr = feedindex;
            break;
         }
    }

    return findFlag;
}
/*****************************************************************************/
// Description : ����ĳһƵ����ID�ڸ������ļ��ƶ���Ƶ�����Ƿ����,�����Ƿ��Ѷ�....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_HasReadEntryIdInXmlFile(
                                                  xmlDocPtr search_xml_ptr,
                                                  const uint16 feedIndex,
                                                  const char * entryId_ptr,
                                                  BOOLEAN *  is_exist_ptr     // out ��������Ŀ�Ƿ����.
                                                  )
{
    uint16 feed_entry_num = 0; // �ļ��е�Ƶ����
    uint16 entryindex = 0;    // �ļ��е�Ƶ�����ļ��е�Ƶ������
    char * xml_entryId_ptr = PNULL; // �ļ���Ƶ��idָ��
    BOOLEAN findFlag = FALSE;

    if(PNULL == entryId_ptr || PNULL == search_xml_ptr || PNULL == is_exist_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:MMIDCDFILE_HasReadEntryIdInXmlFile ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_335_112_2_18_2_12_41_8,(uint8*)"");
        return FALSE;
    }

    // �õ��ļ�Ƶ����Ƶ������
    feed_entry_num = MMIDCD_GetEntryNumInFeed(search_xml_ptr, feedIndex);
    //SCI_TRACE_MID:"[MMIDCD] feed_entry_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_341_112_2_18_2_12_41_9,(uint8*)"d", feed_entry_num);
    //SCI_TRACE_MID:"[MMIDCD] entryId_ptr = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_342_112_2_18_2_12_41_10,(uint8*)"s", entryId_ptr);
    for(entryindex = 0; entryindex < feed_entry_num; entryindex++)
    {
        xml_entryId_ptr = MMIDCD_GetEntryContentCTXTId(search_xml_ptr, feedIndex, entryindex);
        //SCI_TRACE_MID:"[MMIDCD] old entryId_ptr = %s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_346_112_2_18_2_12_41_11,(uint8*)"s", xml_entryId_ptr);
        if(0 == strcmp(entryId_ptr, xml_entryId_ptr))
        {
            // ����Ŀ����....
            *is_exist_ptr = TRUE;
            // ���Ҹ�ID�Ѷ�....
            if(MMIAPIDCD_GetItemAlreadyRead(feedIndex, entryindex)) 
            {
                //SCI_TRACE_MID:"[MMIDCD] find readed item"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_354_112_2_18_2_12_41_12,(uint8*)"");
                findFlag = TRUE;
                break;
            }
            //SCI_TRACE_MID:"[MMIDCD]  same entry id "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_358_112_2_18_2_12_41_13,(uint8*)"");
        }
    }
    
    return findFlag;
}
/*****************************************************************************/
// Description : �����ǰ���ļ����Ƚ�....
// Global resource dependence : None
// Author: jun.hu
// Note:   2009/10/13
/*****************************************************************************/
PUBLIC void MMIDCDFILE_CmpTwoXmlFile(
                                     xmlDocPtr new_xml_ptr, 
                                     xmlDocPtr older_xml_ptr
                                     )
{
    uint16 new_xml_feed_num = 0; // ���ļ��е�Ƶ����
    uint16 new_feedindex = 0;    // ���ļ��е�Ƶ�����ļ��е�Ƶ������
    uint16 new_entryindex = 0;   // ���ļ��е�ĳһƵ�����ڸ�Ƶ���еĴ���
    uint16 new_feed_entry_num = 0; // ���ļ���ĳһƵ����Ƶ������
    char * new_feedId_ptr = PNULL; // ���ļ���Ƶ��idָ��
    char * new_contentId_ptr = PNULL; // ���ļ���ĳһƵ�����Ӧ������idָ��
    
    MMIDCD_ITEM_FLAG new_xml_node_state[MMIDCD_MAX_PAGE_NUM][MMIDCD_MAX_ITEM_NUM_PER_PAGE] = {0};
    SCI_MEMSET(new_xml_node_state, 0, sizeof(new_xml_node_state));

    if(PNULL == new_xml_ptr || PNULL == older_xml_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD MMIDCDFILE_CmpTwoXmlFile has file null !!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_385_112_2_18_2_12_41_14,(uint8*)"");
        return;
    }
    
    // �Ƚϴ˴εõ���xml�ļ������֮ǰ���ļ�֮��Ĳ��죬������ЩƵ�����Ǹ���ǰ���е�...
    // ��ĳһ�θ�����˵�����������һ�Σ����ܴ���Ƶ���������仯�����Ҫ��ĳ��Ƶ����
    // �Ƿ��ڸ���ǰ���еģ�������Ҫ�����е�Ƶ������бȽϣ��漰������һ�����⣬�Ϳ���
    // ����Ƶ����id���ȫ��Ψһ��һ������������Ƶ����id����һ���ٱȽ�.......

    //SCI_TRACE_MID:"[MMIDCD] DCD_CmpTwoXmlFile enterd time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_394_112_2_18_2_12_41_15,(uint8*)"d", SCI_GetTickCount());
    // �õ����ļ���Ƶ����
    new_xml_feed_num = MMIDCD_GetFeedNum(new_xml_ptr);
    //SCI_TRACE_MID:"[MMIDCD] new_fend_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_397_112_2_18_2_12_41_16,(uint8*)"d", new_xml_feed_num);
    
    for(new_feedindex = 0; new_feedindex < new_xml_feed_num; new_feedindex++)
    {   
        // �õ��ƶ���Ƶ��ID
        new_feedId_ptr = MMIDCD_GetFeedCTXTId(new_xml_ptr, new_feedindex);
        if(PNULL != new_feedId_ptr)
        {   
            uint16 find_feed_idx = 0;
            // �õ���һƵ����Ƶ������...
            new_feed_entry_num = MMIDCD_GetEntryNumInFeed(new_xml_ptr, new_feedindex);
            // �����һƵ��ID�ھɵ��ļ��д���
            if(MMIDCDFILE_HasFeedIdInXmlFile(older_xml_ptr, new_feedId_ptr, &find_feed_idx))
            {   
                for(new_entryindex = 0; new_entryindex < new_feed_entry_num; new_entryindex++)
                {   // �õ���һƵ����һƵ����Ķ�Ӧ������ID...
                    new_contentId_ptr = MMIDCD_GetEntryContentCTXTId(new_xml_ptr, new_feedindex, new_entryindex);
                    if(PNULL != new_contentId_ptr)
                    {   
                        BOOLEAN item_is_exist = FALSE;
                        //�����һƵ�����Ӧ������ID��ID�ھ��ļ��д���,�����Ƿ��Ѷ�....
                        if(MMIDCDFILE_HasReadEntryIdInXmlFile(older_xml_ptr, find_feed_idx, new_contentId_ptr, &item_is_exist))
                        {
                            // �ҵ��ˣ����ָ��λ�õ��Ѷ���־��ΪTRUE....
                            new_xml_node_state[new_feedindex][new_entryindex].is_readed = TRUE;
                            //SCI_TRACE_MID:"[MMIDCD] find entry exist"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_422_112_2_18_2_12_41_17,(uint8*)"");
                        }
                        if(!item_is_exist)
                        {
                            s_new_items_num++;
                        }
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIDCD DCD_CmpTwoXmlFile find entry id = NULL !!!!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_431_112_2_18_2_12_41_18,(uint8*)"");
                    }
                }
            }
            else
            {   //��һƵ���е�����Ƶ������µ�....
                s_new_items_num+= new_feed_entry_num;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCD new feedId = Null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_442_112_2_18_2_12_41_19,(uint8*)"");
        }
    }// for 
    // ������õĽ��״̬��������...
    SCI_MEMCPY(s_xml_node_state, new_xml_node_state, sizeof(s_xml_node_state));
    SaveAllItemsState();
    //SCI_TRACE_MID:"[MMIDCD] DCD_CmpTwoXmlFile EXIT time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_448_112_2_18_2_12_41_20,(uint8*)"d", SCI_GetTickCount());
}
/*****************************************************************************/
// Description : �õ�ָ��λ�õ�ITEM �Ƿ���ɾ��
// Global resource dependence : None
// Author: jun.hu , 2009/8/27
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCD_IS_Node_State_Deleted(uint16 pageIndex, uint16 itemIndex)
{
    if(MMIDCD_MAX_PAGE_NUM > pageIndex && MMIDCD_MAX_ITEM_NUM_PER_PAGE > itemIndex)
    {
        return s_xml_node_state[pageIndex][itemIndex].is_deleted;
    }
    else
    {
        //SCI_TRACE_MID:"MMIDCD pageIndex = %d, item index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_462_112_2_18_2_12_41_21,(uint8*)"dd", pageIndex, itemIndex);
        return FALSE;
    }
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note: modified by jun.hu 2009/12/18 for ȥ�������Ĳ�����ֱ���ں����ڲ���ȡ�ļ���
/*****************************************************************************/
LOCAL void SaveAllItemsState(void)
{
    SFS_HANDLE sfs_handle = 0;
    wchar xml_node_state_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    // ����size ��ʱ��0!!!
    if(MMIDCDFILE_GetFilePathForWrite(xml_node_state_full_path , &path_len, MMIDCD_NOD_FILE_PATH, 0))
    {
        sfs_handle = MMIAPIFMM_CreateFile(xml_node_state_full_path, SFS_MODE_WRITE|SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);
        if(SFS_INVALID_HANDLE != sfs_handle)
        {
            uint32 data_len = sizeof(s_xml_node_state);
            
            MMIAPIFMM_WriteFile(sfs_handle, (const void*)s_xml_node_state, data_len, &data_len, NULL);
            MMIAPIFMM_CloseFile(sfs_handle);
        }
        else
        {
            //SCI_TRACE_LOW:"Create xml File Fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_489_112_2_18_2_12_41_22,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"WriteXMLNodeState get file path fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_494_112_2_18_2_12_41_23,(uint8*)"");
    }
}
/*****************************************************************************/
//Description :  Create Default XML File
//Global resource dependence : 
//Author:
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFFS_ParseDefaultXMLFile(void)
{    
    uint32 len = strlen(default_dcd_xml_chinese);
    return DCDXML_ParseSysFile(default_dcd_xml_chinese,len);   
}
/*****************************************************************************/
//Description : Rename dcd xml file 
//Global resource dependence : 
//Author: jun.hu 
//Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDCDFILE_RenameXMLFile(
                                        MMIDCD_FILE_PATH_TYPE_E src_file_type, //[in]
                                        MMIDCD_FILE_PATH_TYPE_E dest_file_type //[in]
                                        )
{
    BOOLEAN ret = FALSE;
    
    wchar    old_file_full_name_arr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0}; 
    wchar    new_file_full_name_arr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0}; 
    
    uint16   old_file_full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint16   new_file_full_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    if(!MMIDCDFILE_GetFilePathForRead(old_file_full_name_arr , &old_file_full_name_len, src_file_type))
    {
        //SCI_TRACE_LOW:"MMIDCD MMIDCDFILE_RenameXMLFile, index xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_525_112_2_18_2_12_42_24,(uint8*)"");
        return FALSE;
    }
    
    if(!MMIDCDFILE_GetFilePathForRead(new_file_full_name_arr , &new_file_full_name_len, dest_file_type))
    {
        //SCI_TRACE_LOW:"MMIDCD MMIDCDFILE_RenameXMLFile, ole xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_531_112_2_18_2_12_42_25,(uint8*)"");
        return FALSE;
    }
    
    if(0 != MMIAPICOM_Wstrcmp(old_file_full_name_arr, new_file_full_name_arr))
    {
        if(MMIAPIFMM_IsFileExist(old_file_full_name_arr, old_file_full_name_len))
        {
            if(MMIAPIFMM_IsFileExist(new_file_full_name_arr, new_file_full_name_len))
            {
                MMIAPIFMM_DeleteFile(new_file_full_name_arr, PNULL);
            }

            if(MMIDCD_OLD_FILE_PATH == dest_file_type)
            {
                ret = MMIAPIFMM_RenameFile(old_file_full_name_arr, old_file_full_name_len, (wchar *)MMIDCD_OLD_NAME, new_file_full_name_len);  
            }
            else if(MMIDCD_INDEX_FILE_PATH == dest_file_type)
            {
                ret = MMIAPIFMM_RenameFile(old_file_full_name_arr, old_file_full_name_len, (wchar *)MMIDCD_INDEX_NAME, new_file_full_name_len);  
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDCD Rename error dest file type!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_554_112_2_18_2_12_42_26,(uint8*)"");
            }
            
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCD Rename old file is not esist"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_560_112_2_18_2_12_42_27,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD Rename two file name same !!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_565_112_2_18_2_12_42_28,(uint8*)"");
    }
    

    return  ret;
        
}
/*****************************************************************************/
//  Description :  Convert FileName to ctxt_id
//  Global resource dependence : 
//  %
//   file_name_ptr:  usc2 file name
//   file_name_len: file name length
//   ctxt_id :    gb code of file name(not include suffix)
//  %
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIDCDFFS_FileName2CtxtId(
										const uint16* full_file_name_ptr,
										uint16 full_file_name_len,
										uint8* ctxt_id)
{  
    uint16       file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};          
    uint16      file_name_len=0;      
    uint16       suffix[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};       
    uint16      suffix_len=0;    

    if(PNULL==full_file_name_ptr || PNULL==ctxt_id)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:MMIDCDFFS_FileName2CtxtId ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_593_112_2_18_2_12_42_29,(uint8*)"");
        return 0;
    }
    
	suffix_len = MMIFILE_FILE_NAME_MAX_LEN;
    MMIAPIFMM_SplitFileName(
				(uint16 *)full_file_name_ptr,
				full_file_name_len,
				file_name,
				&file_name_len,
				suffix,
				&suffix_len
				);
    MMI_WSTRNTOSTR(ctxt_id,MMIFILE_FULL_PATH_MAX_LEN,file_name,MMIFILE_FILE_NAME_MAX_LEN,file_name_len);
    
    return file_name_len*2;
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author: 
//	Note: jun.hu 2009/10/13 �޸�,�����õ���wchar�͵�
/*****************************************************************************/
LOCAL uint16 MMIDCD_CombineFileName(
                                    wchar* file_full_name_ptr, // out 
                                    const char*  file_name_ptr,
                                    const char*  file_extend_name_ptr
                                    )
{
    uint16  str_len = 0;
    char    file_path[MMIFILE_FULL_PATH_MAX_LEN + 2] = {0};
    uint32  wlen = 0;   

    if( (PNULL != file_name_ptr) && (PNULL !=file_extend_name_ptr) && (PNULL !=file_full_name_ptr))
    {
        str_len = strlen((char*)file_name_ptr) + strlen((char*)".") + strlen((char*)file_extend_name_ptr);
        if(str_len < MMIFILE_FULL_PATH_MAX_LEN + 1)
        {
            SCI_MEMCPY(file_path,file_name_ptr,SCI_STRLEN(file_name_ptr)); /*lint !e666*/ 
            strncat(file_path,".",strlen((char*)"."));
            strncat(file_path,(char*)file_extend_name_ptr,strlen((char*)file_extend_name_ptr));
            MMI_STRNTOWSTR(file_full_name_ptr, (MMIFILE_FULL_PATH_MAX_LEN + 1), (const uint8*)file_path,str_len,str_len);
            wlen = str_len;
        }
    }
    
    return wlen;
}
/*****************************************************************************/
//  Description :��ȡһ��ͼƬ������·�� ...
//  Global resource dependence : 
//  Author: 
//  Note: jun.hu 2009/12/21 
/*****************************************************************************/
PUBLIC void MMIDCDFILE_GetOnePictureFullPath(
                                             wchar * full_path_ptr,   // [out: full path]
                                             const char * picture_name_ptr, // [in: picture name]
                                             const char * extend_name_ptr,  // [in: extend name]
                                             MMIDCD_DIR_PATH_TYPE_E dir_path_type
                                             )
{
    wchar   picture_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  picture_name_len = 0;
    uint16  cat_len = 0;
    uint16  dir_name_len = MMIFILE_FULL_PATH_MAX_LEN;

    if(PNULL == full_path_ptr)
    {
        return;
    }
    
    if(PNULL != picture_name_ptr && PNULL != extend_name_ptr)
    {
        picture_name_len = MMIDCD_CombineFileName(picture_name, picture_name_ptr, extend_name_ptr);
    }

    // ��ȡͼƬ���ڵ�·��....
    if(!MMIDCDFILE_GetDirPathForRead(full_path_ptr , &dir_name_len, dir_path_type))
    {
        //SCI_TRACE_LOW:"MMIDCD GetOnePictureFullPath get path fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_668_112_2_18_2_12_42_30,(uint8*)"");
    }

    if(MMIFILE_FULL_PATH_MAX_LEN > dir_name_len)
    {
        cat_len = MIN(picture_name_len, (MMIFILE_FULL_PATH_MAX_LEN - dir_name_len));
        MMIAPICOM_Wstrncat(full_path_ptr, picture_name, cat_len);
    }
    
    
}
/*****************************************************************************/
//  Description :�ҵ������õ�ͼƬ�����Ѷ�Ӧ��CONGTENT��ͼƬ��Ч��־��ΪTRUE
//               �����û�õ�ͼƬ����������.....
//  Global resource dependence : 
//  Author: jun.hu 
//  Note: 2010/1/26 for������¹����а�����������
/*****************************************************************************/
PUBLIC void MMIDCDFILE_FindUseablePictures(void)
{
    MMIFILE_FIND_DATA_T    find_result   =  {0};
    MMIFILE_HANDLE         handle_result =   0;    
    wchar   picutre_dir_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar   picutre_dir_all_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar   picutre_full_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  dir_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint16  dir_all_name_len = 0;
    uint16  find_file_name_len = 0;
    uint16  cat_len = 0;
    const   wchar  suffix[]= L"*.*"; 
    
    if(!MMIDCDFILE_GetDirPathForRead(picutre_dir_name , &dir_name_len, MMIDCD_PICTURE_DIR_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD DeletePictures Get path fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_699_112_2_18_2_12_42_31,(uint8*)"");
    }

    dir_name_len = MIN(MMIFILE_FULL_PATH_MAX_LEN, dir_name_len);

    //���γɱ���·��
    MMIAPICOM_Wstrncat(picutre_dir_all_name, picutre_dir_name, dir_name_len);
    // �γ�·�� ��d:\system\dcd\picture\*.* ���ڲ���
    MMIAPICOM_Wstrncat(picutre_dir_all_name, suffix, sizeof(suffix));
    dir_all_name_len = MMIAPICOM_Wstrlen(picutre_dir_all_name);

    handle_result = MMIAPIFMM_FindFirstFile(picutre_dir_all_name, dir_all_name_len,&find_result);
    
    if(SFS_INVALID_HANDLE != handle_result)
    {

        uint8        file_name_ctxt_id[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};
        uint16       file_name_ctxt_id_len = 0;
        
        do
        {
            SCI_MEMSET(file_name_ctxt_id, 0, sizeof(file_name_ctxt_id));
            SCI_MEMSET(picutre_full_name, 0, sizeof(picutre_full_name));
            MMIAPICOM_Wstrncat(picutre_full_name, picutre_dir_name,  dir_name_len);
            find_file_name_len = (uint16)(MMIAPICOM_Wstrlen((wchar const *)find_result.name));
            
            file_name_ctxt_id_len = MMIDCDFFS_FileName2CtxtId((uint16*)find_result.name, find_file_name_len, file_name_ctxt_id);
            
            //���ͼƬ�ļ�����ʹ��,��ôɾ��
            if(!MMIDCD_IsContentExist(MMIAPIDCD_GetXmlDoc_Ptr(), (char *)file_name_ctxt_id))       
            {   
                cat_len = MIN(MMIFILE_FULL_PATH_MAX_LEN - dir_name_len, find_file_name_len);
                MMIAPICOM_Wstrncat(picutre_full_name, find_result.name, cat_len);          
                
                MMIAPIFMM_DeleteFile(picutre_full_name, PNULL);
                
            }
            //������һ���ļ�
            
        }while(SFS_ERROR_NONE==MMIAPIFMM_FindNextFile(handle_result, &find_result));
        
        SFS_FindClose(handle_result);
    }      

}
/*****************************************************************************/
//  Description :delete 
//  Global resource dependence : 
//  Author: 
//  Note: jun.hu 2009/12/21
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeleteOnePicture(
                                        char * picture_name_ptr, // [in: picture name]
                                        char * extend_name_ptr,   // [in: extend name]
                                        BOOLEAN is_local // [in,�Ƿ���ɾ�����ص�ͼƬ]
                                        )
{
    wchar   picutre_full_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    if(is_local)
    {
        MMIDCDFILE_GetOnePictureFullPath(picutre_full_name, picture_name_ptr, extend_name_ptr, MMIDCD_LOCAL_PICTURE_DIR_PATH);
    }
    else
    {
        MMIDCDFILE_GetOnePictureFullPath(picutre_full_name, picture_name_ptr, extend_name_ptr, MMIDCD_PICTURE_DIR_PATH);
    }

    MMIAPIFMM_DeleteFile(picutre_full_name, PNULL);
}
/*****************************************************************************/
//  Description : create dcd dir 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:2009/12/18 for ��DCD���ļ�����Ϊϵͳ�ļ�����������....
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_CreateDCDDir(void)
{
    const wchar   picture_pure_dir_wstr[]  = L"DCD\\picture";
    const wchar   local_picture_pure_dir_wstr[] = L"DCD\\lpicture";
    
    BOOLEAN return_val = FALSE;
    //  ������Ŀ¼...
    if(MMIAPIFMM_CreateSysFileDir(MMIAPIFMM_GetDefaultDisk(), MMIDCD_DEFAULT_DIR, MMIDCD_DEFAULT_DIR_LEN,PNULL))
    {
        // ����ͼƬ��Ŀ¼...
        return_val = MMIAPIFMM_CreateSysFileDir(MMIAPIFMM_GetDefaultDisk(), picture_pure_dir_wstr, MMIDCD_PURE_PICTURE_DIR_LEN,PNULL);
        if(!return_val)
        {
            //SCI_TRACE_LOW:"MMIDCD create pitcute dir fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_784_112_2_18_2_12_42_32,(uint8*)"");
        }
        // �ٴ������ص�ͼƬĿ¼....
        return_val =   MMIAPIFMM_CreateSysFileDir(MMIAPIFMM_GetDefaultDisk(), local_picture_pure_dir_wstr, MMIDCD_PURE_LOCAL_PICTURE_DIR_LEN,PNULL);
        //SCI_TRACE_LOW:"MMIDCD create local pictute dir = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_788_112_2_18_2_12_42_33,(uint8*)"d", return_val);
        //init index.xml file
        MMIDCDFILE_CreateDefaultXMLFile();
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD create dcd main dir fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_794_112_2_18_2_12_42_34,(uint8*)"");
    }
    
    return return_val;
}
/*****************************************************************************/
//  Description : get dcd index file full path for read
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/12/18
/*****************************************************************************/
LOCAL const wchar * GetDCDFileName( 
                                   uint16 *  filename_len_ptr,     //[OUT]
                                   MMIDCD_FILE_PATH_TYPE_E file_path_type
                                   )
{
    const wchar * file_name_ptr = PNULL;

    if(PNULL != filename_len_ptr)
    {
        switch(file_path_type)
        {
            case MMIDCD_INDEX_FILE_PATH:
            {
                file_name_ptr = MMIDCD_INDEX_NAME;
                *filename_len_ptr = MMIDCD_INDEX_NAME_LEN;
            }
            break;
  
            case MMIDCD_NOD_FILE_PATH:
            {
                file_name_ptr = MMIDCD_NOD_NAME;
                *filename_len_ptr = MMIDCD_NOD_NAME_LEN;
            }
            break;
            
            case MMIDCD_OLD_FILE_PATH:
            {
                file_name_ptr = MMIDCD_OLD_NAME;
                *filename_len_ptr = MMIDCD_OLD_NAME_LEN;
            }
            break;

            case MMIDCD_LOCAL_FILE_PATH:
            {
                file_name_ptr = MMIDCD_LOCAL_NAME;
                *filename_len_ptr = MMIDCD_LOCAL_NAME_LEN;
            }
            break;

            default:
            {
                //SCI_TRACE_LOW:"[MMIDCD]:GetDCDFileName default ERROR!!!!!!!!!!!!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_844_112_2_18_2_12_42_35,(uint8*)"");
            }
            break;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDCD]:GetDCDFileName ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_851_112_2_18_2_12_42_36,(uint8*)"");
    }

    return file_name_ptr;
}
/*****************************************************************************/
//  Description : get dcd index file full path for read
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/12/18
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetFilePathForRead(
                                             wchar  *  full_path_name_ptr,    //[OUT]
                                             uint16 *  full_path_len_ptr,     //[OUT]
                                             MMIDCD_FILE_PATH_TYPE_E file_path_type
                                             )
{
    BOOLEAN   result_val = FALSE;
    const wchar *   file_name_ptr = PNULL;
    uint16    file_name_len = 0;
    if((PNULL != full_path_name_ptr) && (PNULL != full_path_len_ptr))
    {
        file_name_ptr = GetDCDFileName(&file_name_len, file_path_type);
        if(PNULL != file_name_ptr)
        {
            result_val = MMIAPIFMM_GetSysFileFullPathForRead(
                                                           TRUE,
                                                           MMIDCD_DEFAULT_DIR,
                                                           MMIDCD_DEFAULT_DIR_LEN,
                                                           file_name_ptr,
                                                           file_name_len,
                                                           full_path_name_ptr,
                                                           full_path_len_ptr
                                                          );
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCD GetindexFilePathForRead file name is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_886_112_2_18_2_12_42_37,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD PARA error!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_891_112_2_18_2_12_42_38,(uint8*)"");
    }

    return result_val;  
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/12/21 ��ע�⣺Ŀǰ����DCD������ͼƬĿ¼��Ҫ�õ�!!
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetDirPathForRead(
                                            wchar  *  dir_path_name_ptr,    //[OUT]
                                            uint16 *  dir_path_len_ptr,     //[OUT]
                                            MMIDCD_DIR_PATH_TYPE_E dir_path_type
                                            )
{
    BOOLEAN   result_val = FALSE;

    if((PNULL != dir_path_name_ptr) && (PNULL != dir_path_len_ptr))
    {
        
        if(MMIDCD_PICTURE_DIR_PATH == dir_path_type)
        {
            result_val = MMIAPIFMM_GetSysFileFullPathForRead(TRUE,
                                                           MMIDCD_PICTURE_DEFAULT_DIR,
                                                           MMIDCD_PICTURE_DIR_LEN,
                                                           PNULL,
                                                           PNULL,
                                                           dir_path_name_ptr,
                                                           dir_path_len_ptr
                                                          );
        }
        else
        {
            result_val = MMIAPIFMM_GetSysFileFullPathForRead(TRUE,
                                                           MMIDCD_LOCAL_PICTURE_DEFAULT_DIR,
                                                           MMIDCD_LOCAL_PICTURE_DIR_LEN,
                                                           PNULL,
                                                           PNULL,
                                                           dir_path_name_ptr,
                                                           dir_path_len_ptr
                                                          );
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD PARA error!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_936_112_2_18_2_12_42_39,(uint8*)"");
    }

    return result_val;  
}
/*****************************************************************************/
//  Description : get dcd index file full path for write
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/12/18
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetFilePathForWrite(
                                              wchar  *  full_path_name_ptr,    //[OUT]
                                              uint16 *  full_path_len_ptr,     //[OUT]
                                              MMIDCD_FILE_PATH_TYPE_E file_path_type, //[IN]
                                              uint32    size                    //[IN]
                                              )
{
    BOOLEAN   result_val = FALSE;
    const wchar *   file_name_ptr = PNULL;
    uint16    file_name_len = 0;
    if((PNULL != full_path_name_ptr) && (PNULL != full_path_len_ptr))
    {
        file_name_ptr = GetDCDFileName(&file_name_len, file_path_type);
        if(PNULL != file_name_ptr)
        {
            result_val = MMIAPIFMM_GetSysFileFullPathForWrite(
                                                           TRUE,
                                                           MMIDCD_DEFAULT_DIR,
                                                           MMIDCD_DEFAULT_DIR_LEN,
                                                           file_name_ptr,
                                                           file_name_len,
                                                           size,
                                                           full_path_name_ptr,
                                                           full_path_len_ptr
                                                           );
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCD GetindexFilePathForWrite file name is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_973_112_2_18_2_12_42_40,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD FilePathForWrite PARA error!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_978_112_2_18_2_12_42_41,(uint8*)"");
    }

    return result_val;  
}
/*****************************************************************************/
//Description :  Create Default XML File
//Global resource dependence : 
//Author:
//Note: jun.hu 2009/12/19 �����Ӿɺ����������
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_CreateDefaultXMLFile(void)
{    
    MMIFILE_HANDLE  file_handle = SFS_INVALID_HANDLE;
    wchar xml_file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16   file_name_len = MMIFILE_FULL_PATH_MAX_LEN;     
    uint32   bytes_written = 0;
    BOOLEAN  ret = TRUE;
    
    if(!MMIDCDFILE_GetFilePathForWrite(xml_file_name, &file_name_len, MMIDCD_INDEX_FILE_PATH, 0))
    {
        //SCI_TRACE_LOW:"MMIDCD get file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_997_112_2_18_2_12_42_42,(uint8*)"");
        return FALSE;
    }

    if(!MMIAPIFMM_IsFileExist(xml_file_name, file_name_len))
    {
        file_handle = MMIAPIFMM_CreateFile(xml_file_name, SFS_MODE_CREATE_NEW|SFS_MODE_WRITE, PNULL, PNULL);
        
        if (SFS_INVALID_HANDLE != file_handle)               
        {           
            ret = MMIAPIFMM_WriteFile(file_handle,
						            default_dcd_xml_chinese,
						            (uint32)strlen(default_dcd_xml_chinese),
						            &bytes_written,
						            PNULL
						            );

            if (bytes_written != (uint32)strlen(default_dcd_xml_chinese))
            {
                //SCI_TRACE_LOW:"MMIDCDFFS_CreateDefaultXMLFile bytes_written != default_dcd_xml_len"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1016_112_2_18_2_12_43_43,(uint8*)"");
                ret = FALSE;
            }

            MMIAPIFMM_CloseFile(file_handle);             
        }
        else
        {
            //SCI_TRACE_LOW:"Create XML File Failed\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1024_112_2_18_2_12_43_44,(uint8*)"");
            ret = FALSE;
        }   
    }

    return ret;   
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//	Note: jun.hu 2009/12/21 ������MMIDCD_CopyContentFile�������......
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_SaveOnePicture(
                                         const char *file_name_ptr,
                                         const char *file_extend_name_ptr,
                                         MMI_WIN_ID_T win_id
                                         )
{

    wchar  src_file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    wchar  dst_file_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16 src_file_path_len = 0;
    uint16 dst_file_path_len = 0;
    BOOLEAN result = FALSE;
    MMIDCDFILE_GetOnePictureFullPath(src_file_path, file_name_ptr, file_extend_name_ptr, MMIDCD_PICTURE_DIR_PATH);
    MMIDCDFILE_GetOnePictureFullPath(dst_file_path, file_name_ptr, file_extend_name_ptr, MMIDCD_LOCAL_PICTURE_DIR_PATH);
    dst_file_path_len = MMIAPICOM_Wstrlen(dst_file_path);
    src_file_path_len = MMIAPICOM_Wstrlen(src_file_path);
        
    result = MMIAPIFMM_CopyOneFile(
                                src_file_path, 
                                src_file_path_len, 
                                dst_file_path, 
                                dst_file_path_len, 
                                win_id, 
                                MSG_MULTIM_COPY_RESULT_CNF
                                );
    return result;
}
/*****************************************************************************/
//  Description : ȥ�����Ƿ��ѱ����˸�Ƶ����.....
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/10
/*****************************************************************************/
LOCAL BOOLEAN hasSavedTheSameDCDItem(char * content_id_ptr)
{
    BOOLEAN retValue = FALSE;
    uint16 item_num = 0;
    uint16 i= 0;
    MMIDCD_LOCAL_FILE_NODE_T cur_node = {0};
  
    item_num = MMIDCDFILE_GetLocalItemCount();
    
    for(i = 0; i < item_num; i++)
    {
        if(MMIDCDFILE_ReadLocalItemInfo(&cur_node, i))
        {
            if(0 == strcmp(content_id_ptr, cur_node.ctxt_id))
            {
                retValue = TRUE;
                break;
            }
        }
        
    }

    return retValue;
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 2010/1/21
/*****************************************************************************/
PUBLIC MMIDCD_FILE_SAVE_ITEM_RESULT_E MMIDCDFILE_SaveOneItemText(
                                                                 uint16 page_index,
                                                                 uint16 real_item_index
                                                                 )
{
    MMIDCD_LOCAL_FILE_NODE_T cur_node = {0};
    xmlDocPtr doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();
    char  *content_id_ptr = PNULL;
    uint8 *title_ptr      = PNULL;
    uint8 *summary_ptr    = PNULL;
    uint16 text_len       = 0;
    MMIDCD_FILE_SAVE_ITEM_RESULT_E result  = MMIDCD_ITEM_SAVE_FAIL;
    
    // ���ָ��Ƶ�������ݵ�id
    content_id_ptr = MMIDCD_GetEntryContentCTXTId(doc_ptr, page_index, real_item_index);
    if(PNULL != content_id_ptr)
    {
        if(!hasSavedTheSameDCDItem(content_id_ptr))
        {
            SCI_MEMSET(&cur_node, 0, sizeof(MMIDCD_LOCAL_FILE_NODE_T));
            
            strncpy(cur_node.ctxt_id, content_id_ptr, DCD_MAX_TXT_VALUE_LEN);
            title_ptr = MMIDCD_GetEntryStrTitle(&text_len, page_index, real_item_index);
            if(PNULL != title_ptr)
            {
                text_len = MIN(text_len, MMIDCD_MAX_TITLE_STR_LEN);//added by jun.hu 2009/11/26 for ����
                SCI_MEMCPY(cur_node.title_str, title_ptr, text_len);
            }
            cur_node.title_len = text_len;
            summary_ptr = MMIDCD_GetEntryStrSummary(&text_len, page_index, real_item_index);
            if(PNULL != summary_ptr)
            {
                text_len = MIN(text_len, MMIDCD_MAX_SUMMARY_STR_LEN);//added by jun.hu 2009/11/26 for ����
                cur_node.summary_len = text_len;
                SCI_MEMCPY(cur_node.summary_str, summary_ptr, text_len);
            }
            // ��������������Ϣ
            strcpy((char*)cur_node.content_type_str, MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"type"));
            strcpy((char*)cur_node.content_text_id_str, MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, (char*)"ctxt-id"));
            strcpy((char*)cur_node.link_str, MMIDCD_GetEntryLink(page_index, real_item_index, (char*)"href"));
            if(WriteItemTextToLocal(cur_node))
            {
                result = MMIDCD_ITEM_SAVE_SUCESS;
            }
            else
            {
                result = MMIDCD_ITEM_SAVE_FAIL;
            }
        }
        else
        {
            result = MMIDCD_ITEM_ALREADY_EXIST;
        }
    }

    return result;
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author: 
//	Note: jun.hu 2009/12/21 ��MMIDCD_LocalDeleteFiles�������.....
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeleteLocalPictures(void)
{
    MMIFILE_FIND_DATA_T     find_result = {0};
    MMIFILE_HANDLE          handle_result = 0;    
    wchar file_dir_name[MMIFILE_FULL_PATH_MAX_LEN + 1]  = {0};
    wchar file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
    wchar dir_all_file[MMIFILE_FULL_PATH_MAX_LEN + 1]   = {0};  
    const wchar suffix[]= L"*.*"; 
    uint16 dir_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint16  dir_all_file_len = MMIFILE_FULL_PATH_MAX_LEN;

    if(!MMIDCDFILE_GetDirPathForRead(file_dir_name , &dir_len, MMIDCD_LOCAL_PICTURE_DIR_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD DeleteLocalPictures Get path fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1168_112_2_18_2_12_43_45,(uint8*)"");
    }

    //���γ�·��
    MMIAPICOM_Wstrncat(dir_all_file, file_dir_name, dir_len);

    // �γ�·�� ��d:\system\dcd\lpicture\*.* ���ڲ���
    MMIAPICOM_Wstrncat(dir_all_file, suffix, sizeof(suffix));
    dir_all_file_len = MMIAPICOM_Wstrlen(dir_all_file);
    handle_result = MMIAPIFMM_FindFirstFile(dir_all_file, dir_all_file_len, &find_result);
    
    if (SFS_INVALID_HANDLE != handle_result)
    {
        do
        {     
            uint16 cat_len = 0;

            SCI_MEMSET(file_full_path, 0, sizeof(file_full_path));
            
            MMIAPICOM_Wstrncpy(file_full_path, file_dir_name, MIN(MMIFILE_FULL_PATH_MAX_LEN, dir_len));
            if(MMIFILE_FULL_PATH_MAX_LEN > dir_len)
            {
                cat_len = MIN((uint16)(MMIFILE_FULL_PATH_MAX_LEN - dir_len), MMIAPICOM_Wstrlen(find_result.name)); /*lint !e666*/
                MMIAPICOM_Wstrncat(file_full_path, find_result.name, cat_len);
            }

            MMIAPIFMM_DeleteFile(file_full_path, PNULL);
            
            //������һ���ļ�
            
        }while(SFS_ERROR_NONE==MMIAPIFMM_FindNextFile(handle_result, &find_result));
        
        SFS_FindClose(handle_result);
    }      
}
/*****************************************************************************/
// 	Description :�Ժ��ֱ�ӵ��ô˺�������
//	Global resource dependence : none
//  Author:jun.hu
//	Note: 2009/11/3
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeleteRealIndexItem(
                                           uint16 page_index,
                                           uint16 real_item_index
                                           )
{   
    if(MMIDCD_MAX_PAGE_NUM > page_index && MMIDCD_MAX_ITEM_NUM_PER_PAGE > real_item_index)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:pageindex = %d, real_item_index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1214_112_2_18_2_12_43_46,(uint8*)"dd", page_index, real_item_index);
        
        s_xml_node_state[page_index][real_item_index].is_deleted= TRUE;
        //write xml node state
        SaveAllItemsState();
    }
    else
    {
        //SCI_TRACE_MID:"[MMIDCD]:DeleteRealIndexItem pageindex = %d, real_item_index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1222_112_2_18_2_12_43_47,(uint8*)"dd", page_index, real_item_index);
    }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeletePageSingleItem(
                                            const uint16 page_index,
                                            const uint16 real_item_index                                                    
                                            )
{

    MMIDCDFILE_DeleteOnePicture(
                                MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, "ctxt-id"),
                                MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0, "type"),
                                FALSE
                                );
    //save the real deleted item node info
    MMIDCDFILE_DeleteRealIndexItem(page_index, real_item_index);
}
/*****************************************************************************/
//  Description : ������������þ���һ�ο���ɾ������������㵥����ɾ�����¼DEL  
//                ��FLAG,������ɾ�ڶ���ʱ���real index���ܲ�����...
//  Global resource dependence : none
//  Author: jun.hu
//  Note:2009/11/3
/*****************************************************************************/
PUBLIC void MMIDCDFILE_DeletePageMultItem(
                                          const uint16 page_index,
                                          const uint16 * real_idx_arr, // ע�⣺�����洫���Ǿ���ֵ
                                          const uint16 del_num   // Ҫɾ��������
                                          )
{
 
    uint16 real_item_index = 0;
    uint16 i = 0;
    
    if(PNULL == real_idx_arr)
    {
        return;
    }
    
    if(MMIDCD_MAX_PAGE_NUM > page_index && MMIDCD_MAX_ITEM_NUM_PER_PAGE >= del_num)
    {
        for(i = 0; i < del_num; i++)
        {
            real_item_index = real_idx_arr[i];
            // ɾ��ͼƬ.....
            MMIDCDFILE_DeleteOnePicture(
                                        MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0,"ctxt-id"),
                                        MMIAPIDCD_GetEntryContentInfo(page_index, real_item_index, 0,"type"),
                                        FALSE
                                        );
            //д���
            s_xml_node_state[page_index][real_item_index].is_deleted= TRUE;
        }
        
        //write xml node state
        SaveAllItemsState();
    }
    else
    {
        //SCI_TRACE_MID:"[MMIDCD]:DeletePageMultItem pageindex = %d, del_num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1283_112_2_18_2_12_43_48,(uint8*)"dd", page_index, del_num);
    }
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:z.f.
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDCD_GetRealItemIndex(                                 
                                     uint16 page_index,
                                     uint16 item_index,
                                     BOOLEAN is_disable_read
                                     )
{
    int8 display_index = -1;
    int32 i=0;

    for (i = 0; i < MMIDCD_MAX_ITEM_NUM_PER_PAGE; i ++)
    {
        if(is_disable_read || MMIAPIDCD_GeSettingInfo()->is_show_readed)
        {
            if (!s_xml_node_state[page_index][i].is_deleted)
            {
                display_index ++;
            }
        }
        else
        {
            if ((! s_xml_node_state[page_index][i].is_deleted) && (!s_xml_node_state[page_index][i].is_readed))
            {
                display_index ++;
            }
        }
        
        if (display_index == item_index)
        {
            return (uint8)i;
        }
    }

    //SCI_TRACE_LOW:"[MMIDCD]:MMIDCD_GetRealItemIndex NOT find item"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1322_112_2_18_2_12_43_49,(uint8*)"");

    return 0;    /*lint !e527*/
}

/*****************************************************************************/
// 	Description : ����������ȡItem
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_GetUseableItemFromIndex(                                 
                                              uint16 page_index,
                                              uint16 item_index,
                                              uint16 * out_item_index_ptr //out
                                              )
{
    BOOLEAN result = FALSE;
    uint16  item_num = MMIDCD_GetFormalEntryNumInFeed(page_index);
    uint16  i  =  0;
    
    if(PNULL == out_item_index_ptr)
    {
        return FALSE;
    }

    for (i = item_index; i < item_num; i ++)
    {
        if(MMIAPIDCD_GeSettingInfo()->is_show_readed)
        {
            if(!s_xml_node_state[page_index][i].is_deleted)
            {
                result = TRUE;
                *out_item_index_ptr = i;
                break;
            }
           
        }
        else
        {
            if((! s_xml_node_state[page_index][i].is_deleted) && (!s_xml_node_state[page_index][i].is_readed))
            {
                result = TRUE;
                *out_item_index_ptr = i;
                break;
            }
           
        }
    }
    
    return result;
}

/*****************************************************************************/
// 	Description :��ȡͼƬ����Ϣ....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:���ͼƬ�����ݣ�������ڴ�ռ䣬������ָ��ͼƬ���ݵ�ָ�룬�����򷵻ؿ�ָ�룬
//       �������ֵ��Ϊ��, ��Ҫ���øú����ĵط��ͷŸ��ڴ�ռ� 2009/12/17
/*****************************************************************************/
PUBLIC uint8 *  MMIDCDFILE_GetPictureInfo(wchar * full_file_name_ptr, uint32 * file_size_ptr)
{
    MMIFILE_HANDLE   sfs_handle = 0;
    uint32   file_size          = 0;
    uint32   readed_size        = 0;
    uint8 *  data_info_ptr      = PNULL;

    if(PNULL == full_file_name_ptr || PNULL == file_size_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:MMIDCDFILE_GetPictureInfo ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1387_112_2_18_2_12_43_50,(uint8*)"");
        return PNULL;
    }

    sfs_handle = MMIAPIFMM_CreateFile(full_file_name_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != sfs_handle) 
    {
        file_size = MMIAPIFMM_GetFileSize(sfs_handle);
        *file_size_ptr = file_size;
        if(0 < file_size)// ���ȥ����Щ��û����������ͼƬ�����....
        {
            data_info_ptr = MMIDCD_ALLOC(file_size);
            if(PNULL != data_info_ptr)
            {
                if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(sfs_handle, data_info_ptr, file_size ,&readed_size,NULL))
                {
                    //SCI_TRACE_LOW:"MMIDCDFILE_GetPictureInfo readed_size = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1404_112_2_18_2_12_43_51,(uint8*)"d",readed_size);
                }
                else
                {
                    MMIDCD_FreeMem(data_info_ptr);
                    data_info_ptr = PNULL;
                    //SCI_TRACE_LOW:"MMIDCDFILE_GetPictureInfo read file error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1410_112_2_18_2_12_43_52,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDCDFILE_GetPictureInfo get picture alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1415_112_2_18_2_12_43_53,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCDFILE_GetPictureInfo get picture size is zero , no data"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1420_112_2_18_2_12_43_54,(uint8*)"");
        }
        
        MMIAPIFMM_CloseFile(sfs_handle);
    }
    
    return data_info_ptr;
}
/*****************************************************************************/
// 	Description :��ȡͼƬ�ĵĴ�С....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMIDCDFILE_GetPictureSize(wchar * full_file_name_ptr)
{
    MMIFILE_HANDLE   sfs_handle = 0;
    uint32   file_size          = 0;
    if(PNULL != full_file_name_ptr)
    {
        sfs_handle = MMIAPIFMM_CreateFile(full_file_name_ptr, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
        
        if(SFS_INVALID_HANDLE != sfs_handle) 
        {
            file_size = MMIAPIFMM_GetFileSize(sfs_handle);
            if(0 == file_size)
            {
                if(MMIDCD_IsIdle())
                {
                    MMIAPIFMM_DeleteFile(full_file_name_ptr, PNULL);
                }
            }
        }
        
        MMIAPIFMM_CloseFile(sfs_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD MMIDCDFILE_GetPictureSize para error!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1456_112_2_18_2_12_43_55,(uint8*)"");
    }
    
    return file_size;
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note: ��ָ����real index ת����show index,(������Ŀǰû�е��øýӿڵ�û��
//        �õ������Ѷ���Ҫ�󣬹�û������һ����...)
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_RealIndexToShowIndex(                                 
                                              uint16 page_index,
                                              uint16 real_item_index
                                              )
{
    uint16 display_index = 0;
    uint16 i=0;
    uint16 show_item_count = 0;
    
    if(page_index >= MMIDCD_MAX_PAGE_NUM || real_item_index >= MMIDCD_MAX_ITEM_NUM_PER_PAGE)
    {
        //SCI_TRACE_MID:"MMIDCD RealIndexToShowIndex page_index = %d, real_item_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1477_112_2_18_2_12_43_56,(uint8*)"dd", page_index, real_item_index);
        return 0;
    }

    for (i = 0; i < real_item_index; i++)
    {
        if (!s_xml_node_state[page_index][i].is_deleted)
        {
            display_index ++;
        }
    }
    
    show_item_count = MMIAPIDCDFILE_GetFeedUseableEntryCount(page_index, TRUE);
    if(display_index >= show_item_count)
    {
        //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_RealIndexToShowIndex: not find !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1492_112_2_18_2_12_43_57,(uint8*)"");
        display_index = 0;
    }
    
    //SCI_TRACE_LOW:"[MMIDCD]: display_index =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1496_112_2_18_2_12_43_58,(uint8*)"d", display_index);
    return display_index;
   
}

/*****************************************************************************/
// 	Description :��ȡĳһƵ���Ŀ��õ�Ƶ������....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/10/17
/*****************************************************************************/
PUBLIC uint16 MMIAPIDCDFILE_GetFeedUseableEntryCount(                                 
                                                  uint16 feed_index,   //Ƶ����index
                                                  BOOLEAN is_show_read  // �Ƿ���ʾ�Ѷ�
                                                  )
{
    uint16 entryCount = 0;
    uint16 totalEntryNum = 0;
    uint16 entryIndex = 0;
    xmlDocPtr xml_doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();
    if(xml_doc_ptr)
    {   
        // ��feedindex���м��Ĺ����ŵ���MMIDCD_GetEntryNumInFeed ��������.....
        totalEntryNum = MMIDCD_GetEntryNumInFeed(xml_doc_ptr, feed_index);
        // �����Ҫ���Ƿ��Ѷ���....
        if(is_show_read)
        {
            for(entryIndex = 0; entryIndex < totalEntryNum; entryIndex++)
            {
                // ֻҪ�ýڵ�û��ɾ������...
                if (!MMIDCD_IS_Node_State_Deleted(feed_index, entryIndex))
                {
                    entryCount++;
                }
            }

        }
        else
        {
            for(entryIndex = 0; entryIndex < totalEntryNum; entryIndex++)
            {
                if ((!MMIDCD_IS_Node_State_Deleted(feed_index, entryIndex) )&& (! MMIAPIDCD_GetItemAlreadyRead(feed_index, entryIndex)))
                {
                    entryCount++;
                }
            }
        }//else
            
     }//
     
     if(MMIDCD_MAX_ITEM_NUM_PER_PAGE < entryCount)
     {
        entryCount = MMIDCD_MAX_ITEM_NUM_PER_PAGE;
     }
     
     return entryCount;
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN InitalizeLocalFileHeaderInfo(MMIDCD_LOCAL_FILE_HEADER_T * hear_ptr,
										   MMIFILE_HANDLE file_handle
										   )
{
	BOOLEAN  result     = FALSE;
	uint32   write_size = 0;
	uint16   i = 0;
	
	if(PNULL == hear_ptr)
    {
        return FALSE;
    }

	hear_ptr->total_num = 0;
    for( ; i < DCD_LOCAL_MAX_ITEM_NUM; i++)
    {
        hear_ptr->vaild_arr[i] = TRUE;
    }

	if( (uint32)(sizeof(MMIDCD_LOCAL_FILE_HEADER_T)) > MMIAPIFMM_GetFileSize(file_handle) )
	{
		MMIAPIFMM_SetFileSize(file_handle, (uint32)(sizeof(MMIDCD_LOCAL_FILE_HEADER_T)));
	}

	if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(file_handle, hear_ptr,(uint32)sizeof(MMIDCD_LOCAL_FILE_HEADER_T),&write_size, NULL))
	{
		//SCI_TRACE_LOW:"MMIDCD InitalizeLocalFileHeaderInfo write_size = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1581_112_2_18_2_12_44_59,(uint8*)"d", write_size);
		result = TRUE;
	}
	else
	{
		//SCI_TRACE_LOW:"MMIDCD InitalizeLocalFileHeaderInfo write file error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1586_112_2_18_2_12_44_60,(uint8*)"");
	}

	return result;
}
/*****************************************************************************/
// 	Description :��ȡ�Ѵ��ļ���ͷ��Ϣ...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_GetLocalFileHeaderInfo(MMIDCD_LOCAL_FILE_HEADER_T * hear_ptr)
{
    wchar    file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    MMIFILE_HANDLE   sfs_handle          = 0;
    uint32   readed_size         = 0;
    BOOLEAN  result              = FALSE;
    uint16   path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    if(PNULL == hear_ptr)
    {
        return FALSE;
    }

    if(!MMIDCDFILE_GetFilePathForRead(file_name , &path_len, MMIDCD_LOCAL_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1610_112_2_18_2_12_44_61,(uint8*)"");
    }
    
    if(!MMIAPIFMM_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name)))
    {
        if(!MMIDCDFILE_InitLocalFileHeaderInfo())
        {
            //SCI_TRACE_LOW:"MMIDCD init header fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1617_112_2_18_2_12_44_62,(uint8*)"");
            return FALSE;
        }
    }
    // ��������ļ���С���Ϸ�ʱ����InitalizeLocalFileHeaderInfoʱ�п�����Ҫд....
    sfs_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE |SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != sfs_handle) 
    {
        // jun.hu 2009/11/28 for cr162653, ����ļ���Ϣͷ������Ϣ...
        if((uint32)sizeof(MMIDCD_LOCAL_FILE_HEADER_T) > MMIAPIFMM_GetFileSize(sfs_handle))
        {
            if(InitalizeLocalFileHeaderInfo(hear_ptr, sfs_handle))
            {
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDCD init header fail less size than normal"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1635_112_2_18_2_12_44_63,(uint8*)"");
            }
        }
        else
        {
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(sfs_handle, hear_ptr,(uint32)sizeof(MMIDCD_LOCAL_FILE_HEADER_T),&readed_size,NULL))
            {
                //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo readed_size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1642_112_2_18_2_12_44_64,(uint8*)"d",readed_size);
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo read file error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1647_112_2_18_2_12_44_65,(uint8*)"");
            }
            
        }
        
        MMIAPIFMM_CloseFile(sfs_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo create header fail, will init it"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1656_112_2_18_2_12_44_66,(uint8*)"");
        MMIDCDFILE_InitLocalFileHeaderInfo();
    }
    return result;
}
/*****************************************************************************/
// 	Description : д�Ѵ��ļ���ͷ��Ϣ...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_WriteLocalFileHeaderInfo(MMIDCD_LOCAL_FILE_HEADER_T * hear_ptr)
{
    wchar    file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    MMIFILE_HANDLE   file_handle          = 0;
    uint32   write_size         = 0;
    BOOLEAN  result             = FALSE;
    uint16   path_len           = MMIFILE_FULL_PATH_MAX_LEN;

    if(PNULL == hear_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDCD]:MMIDCDFILE_WriteLocalFileHeaderInfo ptr PNULL ERROR!!!!!!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1675_112_2_18_2_12_44_67,(uint8*)"");
        return FALSE;
    }
    
    if(!MMIDCDFILE_GetFilePathForWrite(file_name , &path_len, MMIDCD_LOCAL_FILE_PATH, 0))
    {
        //SCI_TRACE_LOW:"MMIDCD WriteLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1681_112_2_18_2_12_44_68,(uint8*)"");
    }
    //��ȡ���ر����ļ�����Ϣͷ����....//���ﲻ��ҪSFS_MODE_CREATE_NEW,�����������....
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle) 
    {
        if(0 == (uint32)MMIAPIFMM_GetFileSize(file_handle))
        {
            MMIAPIFMM_SetFileSize(file_handle, (uint32)(sizeof(MMIDCD_LOCAL_FILE_HEADER_T)));
        }
        if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(file_handle, hear_ptr,(uint32)sizeof(MMIDCD_LOCAL_FILE_HEADER_T),&write_size, NULL))
        {
            //SCI_TRACE_LOW:"MMIDCD WriteLocalFileHeaderInfo write_size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1694_112_2_18_2_12_44_69,(uint8*)"d", write_size);
            result = TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDCD WriteLocalFileHeaderInfo write file error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1699_112_2_18_2_12_44_70,(uint8*)"");
        }
        
        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
    		
    		//SCI_TRACE_LOW:"MMIDCD WriteLocalFileHeaderInfo create handle = %d"
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1707_112_2_18_2_12_44_71,(uint8*)"d", file_handle);
    }
    
    return result;
}
/*****************************************************************************/
// 	Description : ��ʼ���Ѵ��ļ���ͷ��Ϣ...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/7
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_InitLocalFileHeaderInfo(void)
{
    MMIDCD_LOCAL_FILE_HEADER_T header = {0};
    uint16 i = 0;
    BOOLEAN  result        = FALSE;

    header.total_num = 0;
    
    for( ; i < DCD_LOCAL_MAX_ITEM_NUM; i++)
    {
        header.vaild_arr[i] = TRUE;
    }

    if(MMIDCDFILE_WriteLocalFileHeaderInfo(&header))
    {
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD init HeaderInfo write file error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1735_112_2_18_2_12_44_72,(uint8*)"");
    }

    return result;
}
/*****************************************************************************/
// 	Description :�õ��Ѵ����Ŀ��...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC uint16 MMIDCDFILE_GetLocalItemCount(void)
{
    uint16 itemcount = 0;
    MMIDCD_LOCAL_FILE_HEADER_T header = {0};
    
    if(MMIDCDFILE_GetLocalFileHeaderInfo(&header))
    {
        itemcount = header.total_num;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD MMIDCDFILE_GetLocalItemCount fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1755_112_2_18_2_12_44_73,(uint8*)"");
    }
    
    if(DCD_LOCAL_MAX_ITEM_NUM < itemcount)
    {
        itemcount = DCD_LOCAL_MAX_ITEM_NUM;
    }

    return itemcount;
}
/*****************************************************************************/
// 	Description :��DCD��ĳһ��ITEM����ȡ����Ϣд�����ر���...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
LOCAL BOOLEAN WriteItemTextToLocal(MMIDCD_LOCAL_FILE_NODE_T save_item)
{
    wchar    file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    MMIFILE_HANDLE   file_handle      = 0;
    MMIDCD_LOCAL_FILE_HEADER_T header = {0};
    
    uint32   offset          = 0;
    uint32   write_size      = 0;  
    uint32   readed_size     = 0;  
    BOOLEAN  result          =  FALSE;
    BOOLEAN  index_vaild     =  FALSE;  // �Ƿ��ҵ���һ����Ч��λ��
    uint16   path_len        =  MMIFILE_FULL_PATH_MAX_LEN;
    uint16   real_index      = 0;
    uint16   i               = 0;

    if(!MMIDCDFILE_GetFilePathForRead(file_name , &path_len, MMIDCD_LOCAL_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD GetLocalFileHeaderInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1786_112_2_18_2_12_44_74,(uint8*)"");
        return FALSE;
    }

    // д��������.....
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);
    // �������Ч�ľ��...
    if(SFS_INVALID_HANDLE == file_handle)
    {
        // �����ļ�����ʼ��һ��
        MMIDCDFILE_InitLocalFileHeaderInfo();
        // �ٴ���һ��...
        file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);
    }

    if(SFS_INVALID_HANDLE != file_handle)
    {
        // ��λ��ͷ, ����ͷ��Ϣ....
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN))
        {
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, &header,(uint32)sizeof(MMIDCD_LOCAL_FILE_HEADER_T),&readed_size,NULL))
            {
                for(i = 0; i < DCD_LOCAL_MAX_ITEM_NUM; i++)
                {
                    // �����λ���ǿ��õ�....
                    if(header.vaild_arr[i])
                    {
                        real_index = i;
                        //SCI_TRACE_LOW:"MMIDCD GetFirstFreeIndex = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1814_112_2_18_2_12_44_75,(uint8*)"d", i);
                        index_vaild = TRUE;
                        break;
                    }
                }
            }
        }

        if(!index_vaild)
        {
            MMIAPIFMM_CloseFile(file_handle);
            return FALSE;
        }
        
        MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN);
        offset = sizeof(MMIDCD_LOCAL_FILE_HEADER_T) + real_index * sizeof(MMIDCD_LOCAL_FILE_NODE_T);
        if (offset > (uint32)MMIAPIFMM_GetFileSize(file_handle))
        {
            MMIAPIFMM_SetFileSize(file_handle,(uint32)(offset+sizeof(MMIDCD_LOCAL_FILE_HEADER_T)));
        }
        
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN))
        {        
            if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(file_handle, &save_item, (uint32)sizeof(MMIDCD_LOCAL_FILE_NODE_T),&write_size,NULL))
            {
                //SCI_TRACE_LOW:"WriteItemInfoToLocal write_size = %d,offset = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1839_112_2_18_2_12_44_76,(uint8*)"dd",write_size,offset);
                
                
                header.total_num++;
                if(DCD_LOCAL_MAX_ITEM_NUM < header.total_num)
                {
                    //SCI_TRACE_LOW:"MMIDCD WriteItemInfoToLocal !!!= %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1845_112_2_18_2_12_44_77,(uint8*)"d", header.total_num);
                    header.total_num = DCD_LOCAL_MAX_ITEM_NUM;
                }
                header.vaild_arr[real_index] = FALSE;
                // offset �ָ�.....
                offset = 0;
                // �ٴζ�λ��ͷ����ͷ��Ϣ...
                if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN))
                {
                    if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(file_handle, &header,(uint32)sizeof(MMIDCD_LOCAL_FILE_HEADER_T),&write_size, NULL))
                    {
                        //SCI_TRACE_LOW:"MMIDCD WriteItemTextToLocal write_size = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1856_112_2_18_2_12_44_78,(uint8*)"d", write_size);
                        result = TRUE;
                    }
                }
            }
        }
        
        MMIAPIFMM_CloseFile(file_handle);
        
    }
    
    return result;

}
/*****************************************************************************/
// 	Description :��ȡָ��λ�õ��Ѵ���Ŀ...
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5  2010/1/14 ����������ĳ���Ե�ֵ....
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_ReadLocalItemInfo( 
                                            MMIDCD_LOCAL_FILE_NODE_T * read_item_ptr,
                                            uint16  item_index   // [ in ,�������õ����ֵ]
                                            )
{
    wchar    file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    MMIFILE_HANDLE   file_handle          = 0;
    BOOLEAN  result          =  FALSE;
    uint32   readed_size     = 0;
    uint32   offset          = 0;
    uint16   path_len        = MMIFILE_FULL_PATH_MAX_LEN;
    uint16   real_index      = 0;

    if(PNULL == read_item_ptr)
    {
        return FALSE;
    }

    
    if(!MMIDCDFILE_LocalIndexRelativeToAbs(&real_index, item_index))
    {
        //SCI_TRACE_LOW:"MMIDCD MMIDCDFILE_ReadLocalItemInfo, item_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1895_112_2_18_2_12_44_79,(uint8*)"d", item_index);
        return FALSE;
    }

    if(DCD_LOCAL_MAX_ITEM_NUM <= real_index)
    {
        return FALSE;
    }

    if(!MMIDCDFILE_GetFilePathForRead(file_name , &path_len, MMIDCD_LOCAL_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD MMIDCDFILE_ReadLocalItemInfo, local xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1906_112_2_18_2_12_44_80,(uint8*)"");
        return FALSE;
    }
    
    file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if (SFS_INVALID_HANDLE != file_handle)
    {

        offset = sizeof(MMIDCD_LOCAL_FILE_HEADER_T) + real_index  * sizeof(MMIDCD_LOCAL_FILE_NODE_T);

        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN))
        {        
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(file_handle, read_item_ptr, (uint32)sizeof(MMIDCD_LOCAL_FILE_NODE_T),&readed_size,NULL))
            {
                //SCI_TRACE_LOW:"MMIDCD ReadLocalItemInfo readed_size = %d,offset = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1920_112_2_18_2_12_44_81,(uint8*)"dd",readed_size, offset);
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDCD ReadLocalItemInfo read file error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1925_112_2_18_2_12_44_82,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ReadLocalItemInfo set file pointer error offset = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1930_112_2_18_2_12_44_83,(uint8*)"d",offset);
        }
        
        MMIAPIFMM_CloseFile(file_handle);
    }

    return result;
}
/*****************************************************************************/
// 	Description :ɾ��ָ��λ�õĵ�����Ŀ
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5  
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_DeleteLocalSingleItem(uint16  real_index)
{
    MMIDCD_LOCAL_FILE_HEADER_T header = {0};
    BOOLEAN   result         =  FALSE;
    if(DCD_LOCAL_MAX_ITEM_NUM <= real_index)
    {
        return FALSE;
    }

    if(MMIDCDFILE_GetLocalFileHeaderInfo(&header))
    {
        if(0 < header.total_num)
        {
            header.total_num--;
        }
        
        header.vaild_arr[real_index] = TRUE;
        if(! MMIDCDFILE_WriteLocalFileHeaderInfo(&header))
        {
            //SCI_TRACE_LOW:"MMIDCD DeleteLocalItem write file error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1961_112_2_18_2_12_44_84,(uint8*)"");
        }
        else
        {
            result = TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD DeleteLocalItem read file error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_1970_112_2_18_2_12_44_85,(uint8*)"");
    }

    return result;
}
/*****************************************************************************/
// 	Description :ɾ�������Ŀ���Ѵ�..
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_DeleteLocalMultItem(
                                              uint16 * real_index_ptr,
                                              uint16 del_num
                                              )
{
    MMIDCD_LOCAL_FILE_HEADER_T header = {0};
    BOOLEAN   result         =  FALSE;
    uint16    i  = 0;
    uint16    real_index = 0;

    if(PNULL == real_index_ptr)
    {
        return FALSE;
    }
    
    if(MMIDCDFILE_GetLocalFileHeaderInfo(&header))
    {
        if(del_num > header.total_num)
        {
            return FALSE;
        }

        header.total_num -= del_num;
        for(i = 0; i < del_num; i++ )
        {  
            real_index = real_index_ptr[i];
            if(real_index < DCD_LOCAL_MAX_ITEM_NUM)
            {
                header.vaild_arr[real_index] = TRUE;
            }
        }
        if(FALSE == MMIDCDFILE_WriteLocalFileHeaderInfo(&header))
        {
            //SCI_TRACE_LOW:"MMIDCD DeleteLocalItem write file error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2012_112_2_18_2_12_44_86,(uint8*)"");
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDCD DeleteLocalItem read file error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2017_112_2_18_2_12_45_87,(uint8*)"");
    }

    return result;
}
/*****************************************************************************/
// 	Description :�ѱ��ص���Ե�indexת���ɾ��Ե�index....
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/11/5
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_LocalIndexRelativeToAbs(
                                                  uint16 * real_index_ptr,
                                                  uint16 item_index     // ��Ե�index
                                                  )
{
    MMIDCD_LOCAL_FILE_HEADER_T header = {0};
    BOOLEAN   result         =  FALSE;
    uint16    i  = 0;
    uint16    index  = 0;

    if(PNULL == real_index_ptr)
    {
        return FALSE;
    }
    
    // ��Ե�index Ҳ����ҪС��DCD_LOCAL_MAX_ITEM_NUM
    if(DCD_LOCAL_MAX_ITEM_NUM < item_index)
    {
        return FALSE;
    }

    if(MMIDCDFILE_GetLocalFileHeaderInfo(&header))
    {
        for(i = 0; i < DCD_LOCAL_MAX_ITEM_NUM; i++)
        {
            // �����λ���ѱ��õ�....
            if(FALSE == header.vaild_arr[i])
            {
                if(item_index == index)
                {
                    *real_index_ptr = i;
                    result = TRUE;
                    break;
                }
                
                index++;
            }
        }
    }    
    else
    {
         //SCI_TRACE_LOW:"MMIDCD LocalIndexRelativeToAbs read file error"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2067_112_2_18_2_12_45_88,(uint8*)"");
    }

    return result;
}
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:jun.hu
//	Note:2009/12/19 �Ӿɵ�ReadXMLNodeFlag�������..
/*****************************************************************************/
PUBLIC void MMIDCDFILE_ReadItemsState(void)
{
    SFS_HANDLE file_handle = 0;
    wchar xml_node_state_full_path[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 path_len = MMIFILE_FULL_PATH_MAX_LEN;
    
    if(!MMIDCDFILE_GetFilePathForRead(xml_node_state_full_path , &path_len, MMIDCD_NOD_FILE_PATH))
    {
        //SCI_TRACE_LOW:"MMIDCD ReadItemsState, node xml file name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2084_112_2_18_2_12_45_89,(uint8*)"");
    }

    file_handle = MMIAPIFMM_CreateFile(xml_node_state_full_path, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    
    if(SFS_INVALID_HANDLE != file_handle)
    {
        uint32 data_len = sizeof(s_xml_node_state);
        MMIAPIFMM_ReadFile(file_handle, s_xml_node_state, data_len, &data_len, NULL);
        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
        // for cr170635, ��¼״̬���ļ���ɾ���Ժ��ٸ���ʱ�������ڵ��״̬�� ����������ݣ�������
        // û����� s_xml_node_state�� ����s_xml_node_state����Ļ���ԭ����״̬
        // ����û�в��������������ͷ���һ�µ�������������ELSE�����������԰�״̬д���ļ���ȥ
        MMIAPIFMM_CloseFile(file_handle);
        MMIDCD_Init_XML_Node_State();
    }
    
}
/********************************************************************************
 NAME:			�������ֵ�����
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhuo.wang
 DATE:			
********************************************************************************/
LOCAL uint32 DCD_calexp(uint32 num)
{
    uint32 c = num;
    uint32 i = 1;

    while (c >= 10)
    {
        c = c / 10;
        i++;
    }
    return i;
}

/********************************************************************************
 NAME:			�ַ���ת��Ϊ����
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhuo.wang
 DATE:			
********************************************************************************/
LOCAL uint32 DCDConvertString2Digit(
                                 uint8 *str_ptr,    //in
                                 uint16 length  //in
                                 )
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < length; i++)
    {
        num = (num * 10 + (str_ptr[i] - '0'));
    }
    return num;
}

/********************************************************************************
 NAME:			����ת��Ϊ�ַ���
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhuo.wang
 DATE:			
********************************************************************************/
LOCAL void DCDConvertDigit2String(
                               uint8 *str_ptr,        //out
                               uint16 *length_ptr,    //out
                               uint32 num //in
                               )
{
    uint32 exp = 0, a = 0, b = 0, c = num, i = 0;
    int32 offset = 0;
    
    exp = DCD_calexp(num);
    (*length_ptr) = exp;

    for (i = 0; i < exp; i++)
    {
        a = c / 10;
        b = (c - a * 10);
        offset = exp - 1 - i;
        if(offset>=0 &&offset<*length_ptr)
        {
            str_ptr[exp - 1 - i] = (b + '0');
        }
        else
        {
            break;
        }
        c = a;
    }
}

/*****************************************************************************/
// 	Description :����ģʽdcd ɾ����¼�ӿ�...
//	Global resource dependence : none
//  Author: zhuo.wang
//	Note:2009/11/23
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_DeleteUpdateInfo( void )
{
    wchar    file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    MMIFILE_ERROR_E  file_error=	SFS_NO_ERROR;
    BOOLEAN   result = FALSE;

    //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo enter.."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2193_112_2_18_2_12_45_90,(uint8*)"");
    MMI_STRNTOWSTR(file_name, MMIFILE_FULL_PATH_MAX_LEN, g_mmidcd_update_info_file_path, MMIFILE_FULL_PATH_MAX_LEN, SCI_STRLEN((char*)g_mmidcd_update_info_file_path));
    file_error = MMIAPIFMM_DeleteFile(file_name,PNULL);
    if(SFS_NO_ERROR == file_error)
    {
        //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo; the local.txt file delete ok!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2198_112_2_18_2_12_45_91,(uint8*)"");
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_DeleteUpdateInfo; the local.txt file do not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2203_112_2_18_2_12_45_92,(uint8*)"");
    }
    
    return result;
}

/*****************************************************************************/
// 	Description :����ģʽdcd ���¼�¼�ӿ�...
//	Global resource dependence : none
//  Author: zhuo.wang
//	Note:2009/11/23
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_WriteUpdateInfo( MMIDCD_REQ_TYPE_E req_type,
                                          MMIDCD_ERROR_CODE_E s_dcd_error 
                                          )
{
    wchar    file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    MMIFILE_HANDLE   file_handle          = 0;
    uint16 info_total_num = 0;
    uint8  str_str[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint8  num_str[4] = {0};
    uint16 len=0;
    uint32   offset          = 0;
    uint32   readed_size         = 0;	
    uint32   write_size      = 0;    
    BOOLEAN  result          =  FALSE;
	
    SCI_DATE_T                 cur_date       = {0};
    SCI_TIME_T                  cur_time       = {0};
	
    // ���Ӷ��ļ��Ƿ���ڵ��ж��ʹ���
    MMI_STRNTOWSTR(file_name, MMIFILE_FILE_NAME_MAX_LEN, g_mmidcd_update_info_file_path,\
        MMIFILE_FILE_NAME_MAX_LEN, SCI_STRLEN((char*)g_mmidcd_update_info_file_path));
    
    //  1    �ļ�������ʱ,����
    if(FALSE == MMIAPIFMM_IsFileExist(file_name, MMIAPICOM_Wstrlen(file_name)))
    {        
        file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS, PNULL, PNULL);    
        if(SFS_INVALID_HANDLE != file_handle) 
        {
            DCDConvertDigit2String(num_str,&len,info_total_num);            
            if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, num_str,len,&write_size, PNULL))
            {
                //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo write file error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2244_112_2_18_2_12_45_93,(uint8*)"");
            }
            else
            {
                result = TRUE;
            }
        }        
    }
    else
    {        
        file_handle = MMIAPIFMM_CreateFile(file_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL); 
        if(SFS_INVALID_HANDLE == file_handle) 
        {
            //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo open file error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2257_112_2_18_2_12_45_94,(uint8*)"");
        }
        else	
        {
            result = TRUE;
        }
    }
    
    if(result)
    {
        //�ж��ļ�ͷ����
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_BEGIN))
        {
            for(len=0; len<3;len++)
            {
                MMIAPIFMM_GetChar(file_handle, num_str);
                if( (num_str[0]<'0') || (num_str[0]>'9'))                
                {
                    break;
                }    
            }	    
            
            //  2    ����ͷ��Ϣ������    
            if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_BEGIN))
            {
                if(SFS_ERROR_NONE != MMIAPIFMM_ReadFile(file_handle, &num_str,len,&readed_size,NULL))
                {
                    //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo read file error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2284_112_2_18_2_12_45_95,(uint8*)"");
                    result = FALSE;	
                }     
                else
                {          
                    info_total_num=DCDConvertString2Digit(num_str,strlen((char*)num_str));        
                    info_total_num++;
                    if(DCD_UPDATE_INFO_NUM > info_total_num)
                    {
                        if(SFS_INVALID_HANDLE != file_handle) 
                        {
                            
                            if(SFS_ERROR_NONE ==MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_BEGIN))
                            {
                                DCDConvertDigit2String(num_str,&len,info_total_num);                
                                if(SFS_ERROR_NONE != MMIAPIFMM_WriteFile(file_handle, &num_str,len,&write_size, NULL))
                                {
                                    //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo write file error"
                                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2301_112_2_18_2_12_45_96,(uint8*)"");
                                }
                            }
                            else
                            {
                                //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo SetFilePointer error!!"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2306_112_2_18_2_12_45_97,(uint8*)"");
                            }
                        }
                        
                        //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo !!!= %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2310_112_2_18_2_12_45_98,(uint8*)"d", info_total_num);
                        
                        //  3  ���¼�¼����.....     
                        if(SFS_ERROR_NONE ==MMIAPIFMM_SetFilePointer(file_handle, 0, SFS_SEEK_END))           
                        {
                            TM_GetSysDate(&cur_date);
				TM_GetSysTime(&cur_time);
                            if(s_dcd_error == DCD_ERROR_NULL)
                            {
                                sprintf((char*)str_str, "  \r\n%04d/%02d/%02d   %02d:%02d:%02d  end %02d,  %02d:%02d:%02d  update type: %d,  success!",MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_year,MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_mon,\
                                    MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_mday, MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_hour,\
                                    MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_min, MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_sec,\
                                    cur_date.mday, cur_time.hour, cur_time.min, cur_time.sec,
                                    req_type);
                            }
                            else
                            {
                                sprintf((char*)str_str, "  \r\n%04d/%02d/%02d   %02d:%02d:%02d  end %02d,  %02d:%02d:%02d  update type: %d,  error type: %d",MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_year,MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_mon,\
                                    MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_mday, MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_hour,\
                                    MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_min, MMIAPIDCD_GeSettingInfo()->dcd_update_time.tm_sec,\
                                    cur_date.mday, cur_time.hour, cur_time.min, cur_time.sec,
                                    req_type,s_dcd_error);
                            }
                            
                            if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(file_handle, str_str, (uint32)strlen((char*)str_str),&write_size,NULL))
                            {
                                //SCI_TRACE_LOW:"WriteItemInfoToLocal write_size = %d,offset = %d"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2336_112_2_18_2_12_45_99,(uint8*)"dd",write_size,offset);
                                result = TRUE;
                            }
                            else
                            {
                                //SCI_TRACE_LOW:"WriteItemInfoToLocal write_size file error"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2341_112_2_18_2_12_45_100,(uint8*)"");
                                result = FALSE;		
                            }
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo SetFilePointer error!!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2347_112_2_18_2_12_45_101,(uint8*)"");
                        }
                    }
                    else
                    {
                        //info_total_num %= DCD_UPDATE_INFO_NUM;
                        /*��Ҫ������λ���� */ 
                        //SCI_TRACE_LOW:"[MMIDCD]: MMIDCDFILE_WriteUpdateInfo total info num out of range !!!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_FILE_2354_112_2_18_2_12_45_102,(uint8*)"");
                        MMIAPIFMM_CloseFile(file_handle);   		
                        MMIDCDFILE_DeleteUpdateInfo();
                        result = FALSE;	 
                    }
                }
            }
        }       
        
    }
    
    MMIAPIFMM_CloseFile(file_handle);      
    
    return result;
    
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCDFILE_IsDefaultXML(uint16 page_index )
{
    BOOLEAN result=FALSE;
    char * current_feed_Id_ptr =PNULL;
    char   str_str[]="y7dspk9g1bp";	
    xmlDocPtr xml_doc_ptr = MMIAPIDCD_GetXmlDoc_Ptr();
    if(PNULL != xml_doc_ptr)
    {
        current_feed_Id_ptr = MMIDCD_GetFeedCTXTId(xml_doc_ptr, page_index);
        // �ص�����Ҫ��������ж�...................
        if(PNULL != current_feed_Id_ptr)
        {
            if(0 == strcmp(str_str, current_feed_Id_ptr))
            {
                result = TRUE;
            }
        }
    }

    return result; 	
}	
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*#ifdef MMI_ENABLE_DCD*/

/*****************************************************************************
** File Name:      mmipb_search.h                                             *
** Author:         Lin Lin                                                   *
** Date:           23/11/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe mmi phonebook search        *
**                 function .                                                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2004        Lin Lin      Create
******************************************************************************/

#ifndef _MMIPB_SEARCH_H_
#define _MMIPB_SEARCH_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

//#include "mmk_type.h"
//#include "mmipb_mnapi.h"
#include "mmipb_interface.h"
#include "mmipb_adapt.h"
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


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

        //��Ҫ��ʾ�����м�¼����
typedef struct
{
    uint16 max_count;//���������
    uint16 used_count;//����ʹ�õĸ���
    uint32 *index_table;//���е�16ÿ����¼�����ݿ�����ݱ��е���������16λ��Ӧ�ü�¼���ĸ����뼴�������index
}MMIPB_VISIBLE_INDEX_TABLE_T;

typedef struct _SEARCH_PHONE_NUMBER
{
    //MMIPB_BCD_NUMBER_T	number_t;
    uint32  low;     //�����8λ�ĺ���
    uint16  high;    //�����3λ�ĺ���
	BOOLEAN is_plus_in_head; //����ǰ���Ƿ��"+"
} SEARCH_PHONE_NUMBER_T;

//����ƥ���ڵ�
typedef struct 
{
    SEARCH_PHONE_NUMBER_T   phone_number;  
    uint16                  entry_id;         
    uint16                  storage_id;    
    uint8                   npi_ton;
    uint8                   number_len;         
    uint8                   original_len;        
    uint8                   num_index;         
} MMIPB_SEARCH_ITEM_T;

//����ƥ��������Ϣ
typedef struct _MMIPB_SEARCH_LIST_INFO_T
{   
    uint16               count;  //�������        
	uint16               max_count;//����������
} MMIPB_SEARCH_LIST_INFO_T;

//����ƥ���ṹ
typedef struct _MMIPB_SEARCH_LIST_T
{
    MMIPB_SEARCH_ITEM_T* item_ptr; // �����б�      
    MMIPB_SEARCH_LIST_INFO_T list_inof;  //�����Ļ�����Ϣ          
} MMIPB_SEARCH_LIST_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��ʼ�����������б�
//  Global resource dependence : g_mmipb_search_list_ptr
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_InitNumberList(uint32 num_max_count);
#ifdef MMIPB_MOST_USED_SUPPORT
/*****************************************************************************/
//  Description : ��ʼ�����������б�
//  Global resource dependence : s_pb_number_list, s_pb_mostused_number_list
//  Author: baokun
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_CleanAllMostusedNumber(void);
#endif
/*****************************************************************************/
//  Description : add phone number to search list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertNumberToList(
                                const MMIPB_CONTACT_T* entry_ptr
                                );                              

#ifdef CMCC_VCARD_SUPPORT

/*****************************************************************************/
//  Description : add phone number to search list
//  Global resource dependence : 
//  Author: yao.chen
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_InsertAddtionNumberToList(
                                const MMIPB_CONTACT_T* entry_ptr
                                );                              
#endif

/*****************************************************************************/
//  Description : delete phone number from search list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_DelNumberFromList(
                                        uint16 entry_id,	//IN: ��ɾ����¼�ļ�¼��
                                        uint16 storage_id,  //IN: ��ɾ����¼��storage_id                                        
                                        uint8* num_index_ptr //IN: ɾ���ĺ���index,���Ϊ�գ���ʾɾ���ü�¼��Ӧ�����к���
                                        );

/*****************************************************************************/
//  Description : �绰�����ְ��ƥ��
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: 0:���������ܹ�ְ��ƥ�䣬��������ƥ��
/*****************************************************************************/
PUBLIC int32 MMIPB_BcdSmartCmp(const MMIPB_BCD_NUMBER_T *bcd_number1, 
                               const MMIPB_BCD_NUMBER_T *bcd_number2);

/*****************************************************************************/
//  Description : ��ô洢λ������Ӧ��������Ⱥ��ID��ͼƬID
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNameByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
                                     MMI_STRING_T *name_ptr,
                                     uint16 max_name_len,
                                     uint16* entry_id,
                                     uint16* storage_id,
                                     uint32* group,
                                     BOOLEAN is_use_default_name,
                                     BOOLEAN is_protect);

/*****************************************************************************/
//  Description : ��ȡ���������Ӧ�ĺ�������
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              type_ptr:  ��������[OUT]
//              max_name_len:���������ַ�������󳤶�(��wchar��)  [IN]
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetNumberTypeByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
                                           wchar *type_ptr,
                                           uint16 max_type_len);

/*****************************************************************************/
//  Description : ����������ַ������Һ����а����ô��ļ�¼�������а�������ĸƥ��ļ�¼
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: �ҵ��ĺ���ĸ���
/*****************************************************************************/
PUBLIC uint32 MMIPB_SearchList(MMI_STRING_T *search_str, 
                               uint32 field_type, 
                               MMIPB_SEARCH_TYPE_T search_type, 
                               MMIPB_HANDLE_T *handle_ptr//[OUT]
                               );

/*****************************************************************************/
//  Description : ����������ַ������Һ����а����ô��ļ�¼�������а�������ĸƥ��ļ�¼
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: �ҵ��ĺ���ĸ���
/*****************************************************************************/
PUBLIC void MMIPB_ExitSearchList(MMIPB_HANDLE_T handle);


/*****************************************************************************/
//  Description : �ú���ʵ��ģ������
// str_input���û����ַ���
// group��Ҫװ�صļ�¼��group;
// exclude_group:��Ҫ�ų���group���飬���Ҫ���ҵļ�¼�����group�����У�����ʾ
/*****************************************************************************/
PUBLIC uint32 MMIPB_MultiSearch
(
     MMI_STRING_T *str_input,//IN
     uint32 group,//IN
     uint32 exclude_group,
     uint32 search_type
);

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼���������ͺ���
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSearchEntry(MMIPB_HANDLE_T handle,
                                       MMI_STRING_T *name_str,//[OUT] 
                                       uint16 max_name_len, 
                                       MMIPB_BCD_NUMBER_T *bcd_num,//[OUT]
                                       uint32* group,
                                       uint32 index);

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼��contact_id and storage id
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: Ϊ�ⲿ���н�pbѡ���
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_GetSearchEntryID(MMIPB_HANDLE_T handle,                                       
                                       uint32 index,
                                       uint16 *contact_id_ptr,
                                       uint16 *storage_id_ptr);

/*****************************************************************************/
//  Description : Search phone number in list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_SearchPhoneNumber(
                                MMIPB_BCD_NUMBER_T  *bcd_num_ptr,   //IN: Դ����
                                uint16* entry_id,                   //OUT: entry_id
                                uint16* storage_id,                 //OUT: entry_id 
                                uint8*  num_index_ptr               //OUT: num_index 
                                );

/*****************************************************************************/
//  Description : Search phone number in list
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
BOOLEAN MMIPB_SearchPhoneNumberInMostUsedlist(
                                MMIPB_BCD_NUMBER_T  *bcd_num_ptr,   //IN: Դ����
                                uint16* entry_id,                   //OUT: entry_id
                                uint16* storage_id                    //OUT: storage
                                );

/*****************************************************************************/
//  Description : transfer bcd phone number to hex number
//  Global resource dependence : 
//  Author: Lin.Lin
//  Note: 
/*****************************************************************************/
uint8 PhoneNumberToHex                          //return phone number length
(
    const MMIPB_BCD_NUMBER_T	*bcd_num_ptr,	//IN:
    SEARCH_PHONE_NUMBER_T       *phone_number_ptr //OUT
);
///*****************************************************************************/
////  Description :�ָ�Ĭ��
////  Global resource dependence : none
////  Author: 
////Return: 
///*****************************************************************************/
// PUBLIC void MMIPB_SetDefaultRange(uint8 group_id);


/*****************************************************************************/
//  Description :check two number is partial matched
//  Global resource dependence : none
//  Author: 
//Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsNumberPartialMatch(MMIPB_SEARCH_ITEM_T* base_item_ptr, 
										  MMIPB_SEARCH_ITEM_T* table_item_ptr);

/*****************************************************************************/
//  Description :�жϸú����Ƿ���Ҫ��ȷƥ��
//  Global resource dependence : none
//  Author: 
//Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsExactMatchNumber(SEARCH_PHONE_NUMBER_T phone_number, uint16 num_len);

/*****************************************************************************/
//  Description : ����tab�л�ʱ�Ƿ���Ҫ�޸�contactҳ���style
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIPB_SetPermitChangeType(BOOLEAN ispermit);

/*****************************************************************************/
//  Description : �ú���ʵ�����ֲ���
// str_input���û����ַ���
// group��Ҫ���ҵļ�¼������group;
// exclude_group:��Ҫ�ų���group���飬���Ҫ���ҵļ�¼�����group�����У�����ʾ
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_IsNameExist(MMI_STRING_T *str_input, uint32 storage_id, uint32* pos);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

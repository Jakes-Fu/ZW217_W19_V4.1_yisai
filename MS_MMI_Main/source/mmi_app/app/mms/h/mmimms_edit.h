/****************************************************************************
** File Name:      mmimms_edit.h                                           *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_EDIT_H
#define _MMIMMS_EDIT_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mMmsApi.h"
#include "guianim.h"
#include "mmifmm_export.h"
#include "mmimms_export.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_MAX_SLIDE                            20 
#define MMIMMS_SINGLE_SLIDE_SIZE                    100 
#define MMIMMS_SLIDE_PLAY_DEFAULT_TIME              10000
#define MMIMMS_PIC_REDUCED_WIDTH_MAX                240//ͼƬѹ���������
#define MMIMMS_PIC_REDUCED_HEIGHT_MAX               320//ͼƬѹ�������߶�
#define MMIMMS_SIZE_LEFT                            200
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIMMS_ADD_TO_CONTACT,  //����TO ��ϵ��
#ifdef MMIMMS_SMS_CC_SUPPORT    
    MMIMMS_ADD_CC_CONTACT,    //����CC��ϵ��
    MMIMMS_ADD_BCC_CONTACT,   //����BCC��ϵ��
#endif
    MMIMMS_ADD_CONTENT_CONTACT, //����MMS�����е���ϵ�ˣ�ͬ���ŵĹ���
    MMIMMS_ADD_CONTACT_OP_MAX
}MMIMMS_EDIT_ADD_CONTACT_TYPE;   

typedef struct MMIMMS_EDIT_NAME_LIST_T_tag
{
    wchar   *file_name_ptr;
    uint16  file_name_len;
    uint8   *vfname_ptr;
    uint16  vfname_len;
    struct MMIMMS_EDIT_NAME_LIST_T_tag *next;
    
}MMIMMS_EDIT_NAME_LIST_T;

typedef struct MMIMMS_EDIT_NUMBER_LIST_T_tag
{
    BOOLEAN is_object;       //�Ƿ�����pb���룬�����Ƶĺ��룬�˴��޸���ʱ��������ƥ��
    wchar   *number_name_ptr;
    uint16  number_name_len;
    wchar   *number_ptr;
    uint16  number_len;
    struct MMIMMS_EDIT_NUMBER_LIST_T_tag *next;
    
}MMIMMS_EDIT_NUMBER_LIST_T;


typedef struct MMIMMS_EDIT_DOCUMENT_T_tag
{
    P_MMS_EDIT_BODY_T     editbody_ptr;
    MMIMMS_EDIT_NAME_LIST_T *namelist_header_ptr;
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_header_ptr;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef MMIMMS_SMS_CC_SUPPORT    
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_cc_header_ptr;
    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_bcc_header_ptr;
#endif
#endif
    MMI_STRING_T        edit_to;
    MMI_STRING_T        edit_cc;
    MMI_STRING_T        edit_bcc;
    MMI_STRING_T        edit_subject;
    MMI_STRING_T        msg_id;
    int32		        record_id;
//    uint8               *url;
    int32               pritority;
    int32               read_report;
    int32               expiry;
    int32               message_class;
    int32               sender_visibility;
    int32               delivery_report;
    MMS_TEXT_IMAGE_ORDER_E	textimage_order;        //�Ե���MMS����֡��Ч
}MMIMMS_EDIT_DOCUMENT_T;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��������
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetSubject(
                              const MMI_STRING_T *subject_ptr, 
                              MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                              );

/*****************************************************************************/
//  Description : �����ռ���
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSTo(
                            const MMI_STRING_T *string_to_ptr, 
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            );

/*****************************************************************************/
//  Description : �����ռ��������б�
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSToNameList(
                                    MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr, 
                                    MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                    );

/*****************************************************************************/
//  Description : ������ǰ����ִ���Ϣ 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_AddStringBeforeSubject(MMI_STRING_T *subject, 
                                          const wchar * add_subject, 
                                          const uint16 add_subject_len);

/*****************************************************************************/
//  Description : ���õ�ǰҳ��Ĳ���ʱ��,��λMS
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetCurSlideDuration(
                                       uint32 delay,
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       );

/*****************************************************************************/
//  Description : get edit document 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
//PUBLIC MMIMMS_EDIT_DOCUMENT_T * MMIMMS_GetCurrentEditDoc(void);

/*****************************************************************************/
//  Description : add a new slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_AddSlide(
                                      MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr,
                                      uint32  slide_direct
                                      );

/*****************************************************************************/
//  Description : add text to slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_AddText(
                                     MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr,
                                     const MMI_STRING_T* str_info_ptr
                                     );

/*****************************************************************************/
//  Description : del slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelSlide(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            );

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelImage(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            );

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelMusic(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            );

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelVideo(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            );

/*****************************************************************************/
//  Description : del text in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DelText(
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                            );

/*****************************************************************************/
//  Description : del attachment
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_DelAttachment(
                                 uint8 * attachment_name_ptr,
                                 MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                 );

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_HeadSlide(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       );

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_PrevSlide(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       );

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_NextSlide(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       );

/*****************************************************************************/
//  Description : del image in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_TailSlide(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetCurrentSlideText(
                                          MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr,
                                          MMI_STRING_T             *string_ptr
                                           );

/*****************************************************************************/
//  Description : get current slide index
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GetSlideIndex(
                                   MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                   );

/*****************************************************************************/
//  Description : get slide count
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GetSlideCount(
                                   MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                   );

/*****************************************************************************/
//  Description : get image,music,video count
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GetAllContentCount(
                                        MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                        );

/*****************************************************************************/
//  Description : get image,music,video in slide
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetContentByName(
                                       uint8	*filename, 
                                       uint8 	**data,
                                       uint32	*data_len
                                       );


/*****************************************************************************/
//  Description : edit mms read report  ���Էŵ�UI��
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_DOCUMENT_T * EditMMSReadReport(
                                                  MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                                  );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetCurrentSlideImage(uint32                   *type, 
                                           uint32                   *multim_buf_len,
                                           void                     **multim_buf_ptr,
                                           MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                           );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetCurrentSlideAudio(MMISRVAUD_RING_FMT_E          *fmt, 
                                           uint32                   *multim_buf_len,
                                           void                     **multim_buf_ptr,
                                           MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                           );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetCurrentSlideVideo(uint32                   *type, 
                                           uint32                   *multim_buf_len,
                                           void                     **multim_buf_ptr,
                                           MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                           );

/*****************************************************************************/
//  Description : create mms edit doc
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_DOCUMENT_T *MMIMMS_CreateEditDocument(void);

/*****************************************************************************/
//  Description : destroy mms edit doc
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_DestroyEditDocument(MMIMMS_EDIT_DOCUMENT_T *editdoc_ptr);

/*****************************************************************************/
//  Description : ����Ƿ���Խ�����ű༭ģ��
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIMMS_EnterEditCheck(void);

/*****************************************************************************/
//  Description : edit mms from file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_EditMMSFromFile(
                                   MMIFILE_FILE_INFO_T   *file_info_ptr,
                                   MMIFMM_FILE_TYPE_E   file_type
                                   );

/*****************************************************************************/
//  Description : add file to mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_AddFile(
                              MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                              MMIFILE_FILE_INFO_T   *file_info_ptr,
                              MMIFMM_FILE_TYPE_E   file_type,
                              BOOLEAN is_replace_multifile,
                              uint32 replace_multifile_size
                              );


/*****************************************************************************/
//  Description : add and replace attachment to mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_AddAndReplaceAttachment(
                                              MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                              MMIFILE_FILE_INFO_T   *file_info_ptr,
                                              uint8 *replaced_name_ptr,
                                              uint8   *file_data_ptr,
                                              uint32 file_data_len,
                                              MMIFMM_FILE_TYPE_E   file_type,
                                              MMI_STRING_T *vacard_str
                                              );

/*****************************************************************************/
//  Description :get attachment number of the edit body
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetAttachmentNumber(
                                          P_MMS_EDIT_BODY_T  pDocument,          //[IN]the edit doc
                                          uint32             *attachment_num_ptr //[OUT]the total number of attachment
                                          );

/*****************************************************************************/
//  Description :Get appointed attachment file size
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetAttachmentSize(
                                        char               *pFilename,     //[IN]the filename of content
                                        P_MMS_EDIT_BODY_T  pDocument,      //[IN]the edit doc
                                        uint32             *file_size_ptr  //[OUT]appointed attachment file size
                                        );

/*****************************************************************************/
//  Description : �����û����ã���ȡMMS head��ֵ
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetDefaultHeadValue(
                                       MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr
                                       );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideAudioInfo(uint8 *fname,
                                              uint32 fname_max_len,
                                              MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr);


/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideImageInfo(uint8 *fname,
                                              uint32 fname_max_len,
                                              MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideVideoInfo(uint8 *fname,
                                              uint32 fname_max_len,
                                              MMIMMS_EDIT_DOCUMENT_T   *editdoc_ptr);

/*****************************************************************************/
//  Description : ��������ͼƬ���Ű淽ʽ���Ե���MMS����֡��Ч
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetTextImageOrder(
                                      MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                      MMS_TEXT_IMAGE_ORDER_E  order_type  
                                      );

/*****************************************************************************/
//  Description : ��ȡ����ͼƬ���Ű淽ʽ���Ե���MMS����֡��Ч
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMS_TEXT_IMAGE_ORDER_E MMIMMS_GetTextImageOrder(
                                                       MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr  
                                                       );

/*****************************************************************************/
//  Description : ��ȡ����ͼƬ���Ű淽ʽ���Ե���MMS����֡��Ч
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsWithAttachment(
                                       MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr 
                                       );

/*****************************************************************************/
//  Description : ���������ļ�����ȡ��Ӧ��ʵ���ļ���
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GetFileNameFromVfname(
                                           MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr,
                                           wchar      *file_name_ptr,          //[OUT]
                                           uint16     max_file_name_len,       //[OUT]
                                           uint8      *vfname_ptr,             //[IN]
                                           uint16     vfname_len               //[IN]
                                           );

/*****************************************************************************/
//  Description :get current slide music time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideMusicTime(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description :get current slide video time
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentSlideVideoTime(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description : �����ռ��������б�,����CC,BCC
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSToNameListEx(
                            MMIMMS_EDIT_NUMBER_LIST_T *numberlist_ptr,  //[IN]
                            MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr, //[IN]
                            MMIMMS_EDIT_ADD_CONTACT_TYPE contact_type //[IN]
                            );
#endif                            
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMMS_EDIT_H*/

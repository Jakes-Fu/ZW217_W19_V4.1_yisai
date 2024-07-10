/*! @file guires.h
@brief  ���ļ����ڶ����ͼ����Դ��صķ���������
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 8/2007       Jassmine              Creat
*******************************************************************************/

/*! @page page_Res Res

(����)

*******************************************************************************/

/*! @addtogroup resGrp Res
@brief resģ��
@details
��ģ�����ڶ����ͼ����Դ��صķ���������
@{
*******************************************************************************/

#ifndef _GUIRES_H_
#define _GUIRES_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guianim.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @struct  GUIRES_IMG_SRC_T 
@brief  ����ͼƬbuffer�����ݽṹ
*/
typedef struct
{
    uint8       *data_ptr;              /*!< ͼƬ���ݵ�ַ */
    uint32      data_size;              /*!< ͼƬ���ݵĴ�С */
} GUIRES_IMG_SRC_T;


/*! @struct  GUIRES_IMG_INFO_T 
@brief  ����ͼƬ��Ϣ�����ݽṹ
*/
typedef struct 
{
    BOOLEAN         is_argb;            /*!< �Ƿ��͸����Ϣ *///is argb
    uint16          width;              /*!< ͼƬ��� *///image width
    uint16          height;             /*!< ͼƬ�߶� *///image height
    uint16          frame_num;          /*!< ͼƬ֡�� *///frame number
    GUIANIM_TYPE_E  img_type;           /*!< ͼƬ���� *///image type
} GUIRES_IMG_INFO_T;


/*! @struct  GUIRES_DISPLAY_T 
@brief  ͼƬ����ʾ����
*/
typedef struct 
{
    IMGREF_DATA_FORMAT_E    format;             /*!< ͼƬ���ݸ�ʽ */
    IMG_RES_BG_TYPE_E       bg_type;            /*!< ������ͣ���ʾʱ��ֱ�Ӹ��ǣ������뱳����ϡ�@note ����argb888����Ч *///only for argb888
    BOOLEAN                 is_res_bg_color;    /*!<�Ƿ�֧����Դ������ɫ�滻 */
    BOOLEAN                 is_exchange_color;  /*!<�Ƿ�����ɫ�滻ͼƬ*/
    uint32                  res_bg_color;       /*!<��Դ�����滻��ɫֵ */
    uint32                  exchange_color;     /*!<ͼƬ�滻��ɫֵ */
} GUIRES_DISPLAY_T;


/*! @struct  GUIRES_ANIM_DIS_T 
@brief  ������ʾ��Ϣ
*/
typedef struct 
{
    BOOLEAN         is_dis_ordinal;     /*!< �Ƿ�˳����ʾͼƬ,gif����˳����ʾ */
    BOOLEAN         is_next_frame;      /*!< �Ƿ�gif������һ֡,������ʾ��ǰ֡ */
    uint16          frame_index;        /*!< ָ����ʾ������ĳһ֡,bmp,argb���� */
    uint32          anim_handle;        /*!< ˳����ʾͼƬ,���봫��ͼƬhandle */
} GUIRES_ANIM_DIS_T;


/*! @struct  GUIRES_ANIM_INFO_T 
@brief  ������Ϣ
*/
typedef struct 
{
    BOOLEAN         is_decode_all;      /*!< �Ƿ�ȫ�������� *///is decode all frame
    uint16          cur_frame;          /*!< ��ǰ֡���� *///current frame index
    uint32          delay_time;         /*!< �����ӳ�ʱ�� *///delay time
} GUIRES_ANIM_INFO_T;

/*---------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ��ȡͼƬ��Ϣ
@author Jassmine
@param src_ptr [in] ָ��ͼƬԴ���ݵ�ָ��
@param info_ptr [in/out] ָ��ͼƬ��Ϣ��ָ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgInfo(
                                 GUIRES_IMG_SRC_T  *src_ptr,   //in
                                 GUIRES_IMG_INFO_T *info_ptr   //in/out
                                 );


/***************************************************************************//*!
@brief ��ȡָ����ͼƬ�Ƿ��͸����Ϣ
@author Jassmine
@param img_id [in] ͼƬID
@param win_id [in] ����ID
@return ͼƬ�Ƿ����͸����Ϣ
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_IsArgb(
                             MMI_IMAGE_ID_T     img_id, //in
                             MMI_WIN_ID_T       win_id  //in
                             );


/***************************************************************************//*!
@brief ͨ��ͼƬID��ȡ��Ⱥ͸߶�
@author Jassmine
@param width_ptr [in/out] ָ��ͼƬ��ȵ�ָ��
@param height_ptr [in/out] ָ��ͼƬ�߶ȵ�ָ��
@param img_id [in] ͼƬID
@param win_id [in] ����ID
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgWidthHeight(
                                        uint16          *width_ptr, //may PNULL
                                        uint16          *height_ptr,//may PNULL
                                        MMI_IMAGE_ID_T  img_id,     //in
                                        MMI_WIN_ID_T    win_id      //in
                                        );


/***************************************************************************//*!
@brief ͨ��ͼƬbuffer��ȡ��Ⱥ͸߶�
@author Jassmine
@param data_ptr [in] ͼƬbuffer
@param width_ptr [in] ָ��ͼƬ��ȵ�ָ��
@param height_ptr [in] ָ��ͼƬ�߶ȵ�ָ��
@param data_size [in] ͼƬbuffer�Ĵ�С
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetImgWidthHeightByPtr(
                                             uint8         *data_ptr,  //in:
                                             uint16        *width_ptr, //may PNULL
                                             uint16        *height_ptr,//may PNULL
                                             uint32        data_size   //in:
                                             );


/***************************************************************************//*!
@brief ��ͼƬ����ָ����LCD��
@author Jassmine
@param display_point_ptr [in] ָ��ͼƬ��LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��ͼƬ��LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ��ͼƬ�������Ϊ�գ�����ʾ����ͼƬ��
@param win_id [in] ����ID
@param img_id [in] ͼƬID
@param dev_info_ptr [in] ͼ����Ϣ
@return �Ƿ���óɹ�
@note 
- display_point_ptr��display_rect_ptr����ͬʱΪ�գ����ͬʱ���ڣ�����display_point_ptr
- ͼƬ��ʾ��λ��Ϊdisplay_point_ptr��display_rect_ptr����image_rect_ptr�Ľ���
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImg(
                                 GUI_POINT_T            *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                 GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                 GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                 MMI_WIN_ID_T           win_id,             //in:win id
                                 MMI_IMAGE_ID_T         img_id,             //in:image id
                                 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                 );

/***************************************************************************//*!
@brief ��sabm��ʽ��ͼƬ����ָ����LCD��, ����sabm��ͼƬ, ��������ʾ, ��������
@author James.Zhang
@param display_rect_ptr [in] ָ��ͼƬ��LCD�ϵ���ʾ���򡣲���Ϊ��
@param full_rect_ptr  [in] ָ��ͼƬ��ԭʼ�������Ϊ�գ�����ʾ����ͼƬ��
@param image_rect_ptr [in] ָ����Ҫ��ʾ��ͼƬ�������Ϊ�գ�����ʾ����ͼƬ��
@param win_id [in] ����ID
@param img_id [in] ͼƬID
@param dev_info_ptr [in] ͼ����Ϣ
@return �Ƿ���óɹ�
@note 
- display_point_ptr��display_rect_ptr����ͬʱΪ�գ����ͬʱ���ڣ�����display_point_ptr
- ͼƬ��ʾ��λ��Ϊdisplay_point_ptr��display_rect_ptr����image_rect_ptr�Ľ���
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplaySabmImg(
                                     GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                     GUI_RECT_T             *full_rect_ptr,     //in:may PNULL����ָ��rect������ʾ
                                     GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                     MMI_WIN_ID_T           win_id,             //in:win id
                                     MMI_IMAGE_ID_T         img_id,             //in:image id
                                     const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                     );


/***************************************************************************//*!
@brief ��ͼƬ����ָ����LCD��
@author Jassmine
@param display_point_ptr [in] ָ��ͼƬ��LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��ͼƬ��LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ��ͼƬ�������Ϊ�գ�����ʾ����ͼƬ��
@param win_id [in] ����ID
@param img_id [in] ͼƬID
@param dev_info_ptr [in] ͼ����Ϣ
@param display_ptr [in] ͼƬ��ʾ����
@return �Ƿ���óɹ�
@note 
- display_point_ptr��display_rect_ptr����ͬʱΪ�գ����ͬʱ���ڣ�����display_point_ptr
- ͼƬ��ʾ��λ��Ϊdisplay_point_ptr��display_rect_ptr����image_rect_ptr�Ľ���
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgEx(
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   GUIRES_DISPLAY_T         *display_ptr        //in:display info
                                   );

/***************************************************************************//*!
@brief ��ͼƬ����ָ����LCD��,����ָ����ɫ�滻ͼƬ��ɫ
@author kevin.lou
@param display_point_ptr [in] ָ��ͼƬ��LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��ͼƬ��LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ��ͼƬ�������Ϊ�գ�����ʾ����ͼƬ��
@param win_id [in] ����ID
@param img_id [in] ͼƬID
@param dev_info_ptr [in] ͼ����Ϣ
@param is_exchange_color [in] �Ƿ���ɫ�滻
@param color [in] �滻Ŀ����ɫֵ
@return �Ƿ���óɹ�
@note 
- display_point_ptr��display_rect_ptr����ͬʱΪ�գ����ͬʱ���ڣ�����display_point_ptr
- ͼƬ��ʾ��λ��Ϊdisplay_point_ptr��display_rect_ptr����image_rect_ptr�Ľ���
- 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgWitchColorExchange(
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   BOOLEAN                  is_exchange_color,  //in:�Ƿ���ɫ�滻
                                   uint32                   color               //in:�滻Ŀ����ɫֵ
                                   );


/***************************************************************************//*!
@brief ͨ��ͼƬbuffer��ʾ��ָ����LCD��
@author Jassmine
@param data_ptr [in] ͼƬbuffer
@param data_size [in] ͼƬbuffer�Ĵ�С
@param display_point_ptr [in] ָ��ͼƬ��LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��ͼƬ��LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ��ͼƬ�������Ϊ�գ�����ʾ����ͼƬ��
@param dev_info_ptr [in] ͼ����Ϣ
@return �Ƿ���óɹ�
@note 
- display_point_ptr��display_rect_ptr����ͬʱΪ�գ����ͬʱ���ڣ�����display_point_ptr
- ͼƬ��ʾ��λ��Ϊdisplay_point_ptr��display_rect_ptr����image_rect_ptr�Ľ���
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgByPtr(
                                      uint8                     *data_ptr,          //in:
                                      uint32                    data_size,          //in:
                                      GUI_POINT_T               *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                      GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                      GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                      const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                      );


/***************************************************************************//*!
@brief ͨ��ͼƬbuffer��ʾ��ָ����LCD��
@author Jassmine
@param data_ptr [in] ͼƬbuffer
@param data_size [in] ͼƬbuffer�Ĵ�С
@param display_point_ptr [in] ָ��ͼƬ��LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��ͼƬ��LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ��ͼƬ�������Ϊ�գ�����ʾ����ͼƬ��
@param dev_info_ptr [in] ͼ����Ϣ
@param display_ptr [in] ͼƬ��ʾ����
@return �Ƿ���óɹ�
@note 
- display_point_ptr��display_rect_ptr����ͬʱΪ�գ����ͬʱ���ڣ�����display_point_ptr
- ͼƬ��ʾ��λ��Ϊdisplay_point_ptr��display_rect_ptr����image_rect_ptr�Ľ���
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgByPtrEx(
                                        uint8                   *data_ptr,          //in:
                                        uint32                  data_size,          //in:
                                        GUI_POINT_T             *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                        GUI_RECT_T              *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                        GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                        const GUI_LCD_DEV_INFO  *dev_info_ptr,      //in:
                                        GUIRES_DISPLAY_T        *display_ptr        //in:display info
                                        );


/***************************************************************************//*!
@brief ��ȡ�����Ŀ�Ⱥ͸߶�
@author Jassmine
@param width_ptr [in] ָ�򶯻���ȵ�ָ�룬����Ϊ��
@param height_ptr [in] ָ�򶯻��߶ȵ�ָ�룬����Ϊ��
@param frame_num_ptr [in] ָ�򶯻�֡����ָ�룬����Ϊ��
@param anim_id [in] ����ID
@param win_id [in] ����ID
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetAnimWidthHeight(
                                         uint16         *width_ptr,     //in/out:may PNULL
                                         uint16         *height_ptr,    //in/out:may PNULL
                                         uint16         *frame_num_ptr, //in/out:may PNULL
                                         MMI_ANIM_ID_T  anim_id,        //in
                                         MMI_WIN_ID_T   win_id          //in
                                         );


/***************************************************************************//*!
@brief ͨ������buffer��ȡ��Ⱥ͸߶�
@author Jassmine
@param data_ptr [in] ָ�򶯻�buffer
@param width_ptr [in] ָ�򶯻���ȵ�ָ�룬����Ϊ��
@param height_ptr [in] ָ�򶯻��߶ȵ�ָ�룬����Ϊ��
@param frame_num_ptr [in] ָ�򶯻�֡����ָ�룬����Ϊ��
@param data_size [in] ����buffer�Ĵ�С
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_GetAnimWidthHeightByPtr(
                                              uint8          *data_ptr,      //in:may PNULL
                                              uint16         *width_ptr,     //in/out:may PNULL
                                              uint16         *height_ptr,    //in/out:may PNULL
                                              uint16         *frame_num_ptr, //in/out:may PNULL
                                              uint32         data_size       //in:
                                              );


/***************************************************************************//*!
@brief ��������ʾ��ָ����LCD��
@author Jassmine
@param frame_index [in] ָ���ڼ�֡����0��ʼ����
@param display_point_ptr [in] ָ��������LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��������LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ�Ķ����������Ϊ�գ�����ʾ���Ŷ�����
@param win_id [in] ����ID
@param anim_id [in] ����ID
@param dev_info_ptr [in] ͼ����Ϣ
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnim(
                                  uint16                    frame_index,        //in:from 0
                                  GUI_POINT_T               *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                  GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                  GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                  MMI_WIN_ID_T              win_id,             //in:win id
                                  MMI_ANIM_ID_T             anim_id,            //in:anim id
                                  const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                  );

/***************************************************************************//*!
@brief ��������ʾ��ָ����LCD��
@author Jassmine
@param frame_index [in] ָ���ڼ�֡����0��ʼ����
@param display_point_ptr [in] ָ��������LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��������LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ�Ķ����������Ϊ�գ�����ʾ���Ŷ�����
@param win_id [in] ����ID
@param anim_id [in] ����ID
@param dev_info_ptr [in] ͼ����Ϣ
@param display_ptr [in] ������ʾ����
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimEx(
                                    uint16                  frame_index,        //in:from 0
                                    GUI_POINT_T             *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                    GUI_RECT_T              *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                    GUI_RECT_T              *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                    MMI_WIN_ID_T            win_id,             //in:win id
                                    MMI_ANIM_ID_T           anim_id,            //in:anim id
                                    const GUI_LCD_DEV_INFO  *dev_info_ptr,      //in:
                                    GUIRES_DISPLAY_T        *display_ptr        //in:display info
                                    );


/***************************************************************************//*!
@brief ͨ������buffer��ʾ��ָ����LCD��
@author Jassmine
@param data_ptr [in] ָ�򶯻�buffer
@param frame_index [in] ָ���ڼ�֡����0��ʼ����
@param data_size [in] ����buffer�Ĵ�С
@param display_point_ptr [in] ָ��������LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��������LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ�Ķ����������Ϊ�գ�����ʾ���Ŷ�����
@param dev_info_ptr [in] ͼ����Ϣ
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimByPtr(
                                       uint8                     *data_ptr,          //in:
                                       uint16                    frame_index,        //in:from 0
                                       uint32                    data_size,          //in:
                                       GUI_POINT_T               *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                       GUI_RECT_T                *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                       GUI_RECT_T                *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                       const GUI_LCD_DEV_INFO    *dev_info_ptr       //in:
                                       );


/***************************************************************************//*!
@brief ͨ������buffer��ʾ��ָ����LCD��
@author Jassmine
@param data_ptr [in] ָ�򶯻�buffer
@param frame_index [in] ָ���ڼ�֡����0��ʼ����
@param data_size [in] ����buffer�Ĵ�С
@param display_point_ptr [in] ָ��������LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��������LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ�Ķ����������Ϊ�գ�����ʾ���Ŷ�����
@param dev_info_ptr [in] ͼ����Ϣ
@param display_ptr [in] ������ʾ����
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayAnimByPtrEx(
                                         uint8                  *data_ptr,          //in:
                                         uint16                 frame_index,        //in:from 0
                                         uint32                 data_size,          //in:
                                         GUI_POINT_T            *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                         GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                         GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                         const GUI_LCD_DEV_INFO *dev_info_ptr,      //in:
                                         GUIRES_DISPLAY_T       *display_ptr        //in:display info
                                         );


/***************************************************************************//*!
@brief ��ͼƬ������ָ�����ڴ�
@author Jassmine
@param src_data_ptr [in] ָ��ԴͼƬbuffer
@param dest_width [in] Ŀ��buffer�Ŀ��
@param dest_height [in] Ŀ��buffer�ĸ߶�
@param dest_buf_size [in] Ŀ��buffer�Ĵ�С
@param src_data_size [in] ԴͼƬbuffer�Ĵ�С
@param src_point_ptr [in] ָ��ԴͼƬ�����λ�ã�����Ϊ��
@param src_rect_ptr [in] ָ��ԴͼƬ�����򣬿���Ϊ��
@param dest_buf_ptr [in/out] ָ��Ŀ��buffer
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_CopyImgBuf(
                                 const uint8   *src_data_ptr,  //in:src data buffer
                                 uint16        dest_width,     //in:
                                 uint16        dest_height,    //in:
                                 uint32        dest_buf_size,  //in:
                                 uint32        src_data_size,  //in:
                                 GUI_POINT_T   *src_point_ptr, //in:may PNULL����ָ��rect����
                                 GUI_RECT_T    *src_rect_ptr,  //in:may PNULL����ָ����ʼ��
                                 GUI_COLOR_T   *dest_buf_ptr   //out:dest buffer
                                 );


/***************************************************************************//*!
@brief ��ͼƬ������ָ�����ڴ�
@author Jassmine
@param src_data_ptr [in] ָ��ԴͼƬbuffer
@param dest_width [in] Ŀ��buffer�Ŀ��
@param dest_height [in] Ŀ��buffer�ĸ߶�
@param dest_buf_size [in] Ŀ��buffer�Ĵ�С
@param src_data_size [in] ԴͼƬbuffer�Ĵ�С
@param src_point_ptr [in] ָ��ԴͼƬ�����λ�ã�����Ϊ��
@param src_rect_ptr [in] ָ��ԴͼƬ�����򣬿���Ϊ��
@param dest_buf_ptr [in/out] ָ��Ŀ��buffer
@param display_ptr [in] ͼƬ��ʾ����
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_CopyImgBufEx(
                                   const uint8          *src_data_ptr,  //in:src data buffer
                                   uint16               dest_width,     //in:
                                   uint16               dest_height,    //in:
                                   uint32               dest_buf_size,  //in:
                                   uint32               src_data_size,  //in:
                                   GUI_POINT_T          *src_point_ptr, //in:may PNULL����ָ��rect����
                                   GUI_RECT_T           *src_rect_ptr,  //in:may PNULL����ָ����ʼ��
                                   void                 *dest_buf_ptr,  //out:dest buffer,888����32bit
                                   GUIRES_DISPLAY_T     *display_ptr    //in:display info
                                   );


/***************************************************************************//*!
@brief ����ͼƬ�ľ��
@author Jassmine
@param handle_ptr [in/out] ͼƬ�ľ��
@param win_id [in] ����ID
@param img_id [in] ͼƬID
@param dev_info_ptr [in] ͼ����Ϣ(�ѷ���)
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_CreatHandle(
                                  uint32            *handle_ptr,//in/out:
                                  MMI_WIN_ID_T      win_id,     //in:win id
                                  MMI_IMAGE_ID_T    img_id,      //in:image id
                                  GUI_LCD_DEV_INFO  *dev_info_ptr
                                  );


/***************************************************************************//*!
@brief  �ͷ�ͼƬ�ľ��
@author Jassmine
@param handle [in] ͼƬ�ľ��
@return �Ƿ���óɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_FreeHandle(
                                 uint32     handle//in/out:
                                 );


/***************************************************************************//*!
@brief  ��ʾgif����
@author Jassmine
@param is_next_frame [in] �Ƿ�gif������һ֡,������ʾ��ǰ֡
@param handle [in] ͼƬ�ľ��
@param display_point_ptr [in] ָ��������LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��������LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ�Ķ����������Ϊ�գ�����ʾ���Ŷ�����
@param win_id [in] ����ID
@param img_id [in] ͼƬID
@param gif_info_ptr [in/out] ���ض�����ʾ�����Ϣ
@param dev_info_ptr [in] ͼ����Ϣ
@return �Ƿ���óɹ�
@note Gif����ֻ����֡��ʾ,����ָ����ǰ֡
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayGif(
                                 BOOLEAN                is_next_frame,      //in:
                                 uint32                 handle,             //in:
                                 GUI_POINT_T            *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                 GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                 GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                 MMI_WIN_ID_T           win_id,             //in:win id
                                 MMI_IMAGE_ID_T         img_id,             //in:image id
                                 GUIRES_ANIM_INFO_T     *gif_info_ptr,      //in/out:may PNULL
                                 const GUI_LCD_DEV_INFO *dev_info_ptr       //in:
                                 );


/***************************************************************************//*!
@brief ��ʾgif����
@author Jassmine
@param is_next_frame [in] �Ƿ�gif������һ֡,������ʾ��ǰ֡
@param handle [in] ͼƬ�ľ��
@param display_point_ptr [in] ָ��������LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��������LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ�Ķ����������Ϊ�գ�����ʾ���Ŷ�����
@param win_id [in] ����ID
@param img_id [in] ͼƬID
@param gif_info_ptr [in/out] ���ض�����ʾ�����Ϣ
@param dev_info_ptr [in] ͼ����Ϣ
@param display_ptr [in] ͼƬ��ʾ����
@return �Ƿ���óɹ�
@note Gif����ֻ����֡��ʾ,����ָ����ǰ֡
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayGifEx(
                                   BOOLEAN                  is_next_frame,      //in:
                                   uint32                   handle,             //in:
                                   GUI_POINT_T              *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
                                   GUI_RECT_T               *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
                                   GUI_RECT_T               *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
                                   MMI_WIN_ID_T             win_id,             //in:win id
                                   MMI_IMAGE_ID_T           img_id,             //in:image id
                                   GUIRES_ANIM_INFO_T       *gif_info_ptr,      //in/out:may PNULL
                                   const GUI_LCD_DEV_INFO   *dev_info_ptr,      //in:
                                   GUIRES_DISPLAY_T         *display_ptr    //in:display info
                                   );

/***************************************************************************//*!
@brief ��ȡָ����ͼƬ�Ƿ������
@author Jassmine
@param img_id [in] ͼƬID
@param win_id [in] ����ID
@return ͼƬ�Ƿ������
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_IsSabm(
                             MMI_IMAGE_ID_T     img_id, //in
                             MMI_WIN_ID_T       win_id  //in
                             );

/***************************************************************************//*!
@brief ��ͼƬ����ָ����LCD��
@author lianxiang.zhou
@param display_point_ptr [in] ָ��ͼƬ��LCD�ϵ����λ�ã����Ϊ����LCD��ȡ����Ϊ�գ�����display_rect_ptr������
@param display_rect_ptr [in] ָ��ͼƬ��LCD�ϵ���ʾ�������Ϊ�գ�����display_point_ptr������
@param image_rect_ptr [in] ָ����Ҫ��ʾ��ͼƬ�������Ϊ�գ�����ʾ����ͼƬ��
@param win_id [in] ����ID
@param img_id [in] ͼƬID
@param dc_data_ptr [in] ͼ����Ϣ
@return �Ƿ���óɹ�
@note 
- display_point_ptr��display_rect_ptr����ͬʱΪ�գ����ͬʱ���ڣ�����display_point_ptr
- ͼƬ��ʾ��λ��Ϊdisplay_point_ptr��display_rect_ptr����image_rect_ptr�Ľ���
- ֱ�ӽ�ͼƬ��ʾ�ڲ�����λ���ϣ���ȥ�м�λ��ת��
- ͬʱ��ȥupdate rect��ͼ���alphaЧ������
*******************************************************************************/
PUBLIC BOOLEAN GUIRES_DisplayImgDst(
    GUI_POINT_T            *display_point_ptr, //in:may PNULL����ָ��rect������ʾ
    GUI_RECT_T             *display_rect_ptr,  //in:lcd display rect,may PNULL����ָ����ʼ����ʾ
    GUI_RECT_T             *image_rect_ptr,    //in:image rect,may PNULL�����ͼƬ(0,0)����ʾ
    MMI_WIN_ID_T           win_id,
    MMI_IMAGE_ID_T         img_id,
    GUILCD_DRAW_DC_T const *dc_data_ptr
);

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/

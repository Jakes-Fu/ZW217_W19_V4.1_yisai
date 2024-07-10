/*****************************************************************************
** File Name:               mmiap_musicmark.h                                *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifndef _MMIAP_MUSICARRAY_H_
#define _MMIAP_MUSICARRAY_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmivirtualarray.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
//��������ʽ
typedef enum
{
    APLAYER_LSIT_SORT_NULL,
    APLAYER_LSIT_SORT_NAME,
    APLAYER_LSIT_SORT_TIME,
    APLAYER_LSIT_SORT_SIZE,
    APLAYER_LSIT_SORT_TYPE,
    APLAYER_LSIT_SORT_MAX
}
APLAYER_LIST_SORT_TYPE_E;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create handle
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void * MMIAPLAYERARRAY_CreateArray(uint32 data_size,
        const char *file,
        uint32   line
                                         );


/*****************************************************************************/
//  Description : destory handle
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPLAYERARRAY_DestroyArray(void ** music_array);

/*****************************************************************************/
//  Description : Get the total number of music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPLAYERARRAY_GetTotalNum(void * music_array);


/*****************************************************************************/
//  Description : Get music mark detail info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Read(
    void * music_array,
    uint32 index,                                       //[IN]
    void *data_info_ptr      //[OUT]
);


/*****************************************************************************/
//  Description : load all music mark brief info.
//  Global resource dependence : s_music_mark_info_ptr
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_LoadFromFile(void * music_array,
        uint16     *full_path_ptr,
        uint16     full_path_len);


/*****************************************************************************/
//  Description : Add a new music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Add(void * music_array,
                                   void * data_info_ptr);

/*****************************************************************************/
//  Description : Delete music mark.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Delete(void * music_array,
                                      uint16 data_index);

/*****************************************************************************/
//  Description : Update music mark info.
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Modify(void * music_array,
                                      uint16  data_index,         //[IN]mark index
                                      void * data_info_ptr ) ;

/*****************************************************************************/
//  Description : ��filearray���鱣�浽ָ���ļ��У��ػ���Ҳ���ᶪʧ
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIAPLAYERARRAY_SaveArrayToFile(
    void*  music_array,     //in, filearrayֵ
    uint32  data_size,
    uint16     *full_path_ptr,     //Ҫ���浽���ļ���, �ļ�һ��Ҫ����
    uint16     full_path_len
);

/*****************************************************************************/
//  Description : ��filearray�����������
//  Global resource dependence :
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_Sort(  //����True�ɹ���Falseʧ��
    void *       music_array, //in, filearrayֵ
    APLAYER_LIST_SORT_TYPE_E sort_type,   //in, ��������
    MMIVIRTUALARRAY_COMPARE  compare_func
);

/*****************************************************************************/
//  Description : set buffer size for array
//  Global resource dependence :
//  Author: ying.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPLAYERARRAY_SetBufferSize(
    void*    music_array,
    uint32          size           //counted in KB
);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif



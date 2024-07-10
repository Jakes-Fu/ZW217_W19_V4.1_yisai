/******************************************************************************
** File Name:      mmi_resource_def.h                                         *
** Author:                                                                    *
** Date:           2006                                         		      *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.          *
** Description:    This file is used to describe mmi resource info            *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 2006       jibin          Create							 				  *
******************************************************************************/
/*!
 *  \addtogroup mmk_resource_group_label
 *  @{
 */


/*! 
 *  \file 	mmi_resource_def.h
 *  \date	2006
 *  \brief 	This file is used to describe mmi resource info
 */


#ifndef _MMI_RESOURCE_DEF_H__
#define _MMI_RESOURCE_DEF_H__

#include "sci_types.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif

///////////////////////////////////////////////////////////////////////////////
// The whole information of resource
///////////////////////////////////////////////////////////////////////////////

#define MAX_PACKAGE_RESOURCE_NUM	32 // �������������SkinMerge�����á�


#define MAX_RES_VERSION_LENGTH    128
#define MAX_RES_NAME_LENGTH 	128 

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*! \brief resource info needed by download tool */    
typedef struct _TOOL_USED_INFO_T
{
    uint8  flag[4];         // �����ܵ���ԴbinΪ "sres"��theme��ԴbinΪ"tres"
    uint8  version[4];      // "0001"
    uint32 size;            // �ṹ��size,���ڽṹ��չ��MMI�����ӹ�bin��ͷ��size��С��
    uint8  bigendian_flag;  // arm binΪ 1�� win binΪ0
    uint8  reserved1;       // ĿǰΪ0
    //uint16 reserved2;     // ĿǰΪ0,���Ϊres_flag��language_id2��uint8 
	uint8  res_flag;		// 	
	uint8  language_id;
} TOOL_USED_INFO_T, * PTOOL_USED_INFO_T;



/*! \brief mmi resource type */    
typedef enum _MMI_RES_TYPE_E
{
	MMI_RT_IMAGE = 0,								/*!< image resource */
    MMI_RT_ANIM,									/*!< animate resource */
	MMI_RT_RING,									/*!< ring resource */
	MMI_RT_TEXT,									/*!< text resource */
	MMI_RT_FONT,									/*!< font resource */    
    MMI_RT_DATA,									/*!< data resource */        
    MMI_RT_COLOR,                                   /*!< color resource */
    MMI_RT_RECT,                                    /*!< rect resource */
	MMI_RT_MAX_NUM									/*!< max resource num */
} MMI_RES_TYPE_E;



/*! \brief mmi resource data */
typedef struct _MMI_RES_DATA_T
{
	const void 					*pData; 			/*!< resource address */			
 	uint32  					uCount;     		/*!< resource count */				
	uint32  					uSize;       		/*!< resource size */				
} MMI_RES_DATA_T, * PMMI_RES_DATA_T;



/*! \brief mmi resource struct */
typedef struct
{	
	MMI_RES_DATA_T   			mmi_data_table[ MMI_RT_MAX_NUM ];	/*!< mmi data table */
} MMIRES_MODULE_INFO_T, * PMMIRES_MODULE_INFO_T;


typedef struct  
{
    uint32 offset;
    uint32 size;
}PACKAGE_INFO_T;

typedef struct 
{
    TOOL_USED_INFO_T bin_tool_info;
    PACKAGE_INFO_T default_info;
    PACKAGE_INFO_T addition_info[MAX_PACKAGE_RESOURCE_NUM];
    uint8  addition_num;
}RESOURCE_PACKAGE_INFO_T, *PRESOURCE_PACKAGE_INFO_T;


typedef struct 
{
    uint32 color;    // store 16bits 565 RGB
    uint8  validate; // 0,invalidate; 1, validate
    uint8  reserved[3]; // reserved,not used now
}MMIRES_COLOR_T;

typedef struct 
{
    int16 left;
    int16 top;
    int16 right;
    int16 bottom;
    uint8  validate; // 0,invalidate; 1, validate
    uint8  reserved[3]; // reserved,not used now
}MMIRES_RECT_T;

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
}
#endif



#endif // _MMI_RESOURCE_DEF_H__
/*! @} */
 

/*****************************************************************************
** File Name:      mmipb_common.h                                          *
** Author:                                                                 *
** Date:           11/09/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file defines common struct, common emnu, common MACRO *
**                 定义了电话本模块的公共的结构，枚举，宏等
**                 注：严禁在本文件中定义非共用的结构，枚举，宏，尽量少的在这里定义！！！
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        baokun yin       Create
******************************************************************************/

#ifndef _MMIPB_COMMON_H_
#define _MMIPB_COMMON_H_

//#define MMIPB_FUNCTION_TEST_MODE
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mn_type.h"

#if defined(MMIPB_FUNCTION_TEST_MODE) && defined (WIN32)
#include "mmipb_testcase.h"
#endif
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
//#define CMCC_VCARD_SUPPORT
#ifdef MMI_PB_NAME_LEN_20_BYTES
#define MMIPB_NAME_MAX_LEN   (20)
#elif !defined(PB_SUPPORT_LOW_MEMORY)
#define MMIPB_NAME_MAX_LEN   (PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
#else
#define MMIPB_NAME_MAX_LEN   (30)
#endif   
#define MMIPB_BCD_NUMBER_NORMAL_LEN (uint8)PHONEBOOK_MAX_NORMAL_DN_NUMBER_LEN//sim卡的非扩展号码长度：10
#define MMIPB_BCD_NUMBER_MAX_LEN    (uint8)(PHONEBOOK_MAX_DN_NUMBER_LEN)
#define MMIPB_GROUP_NAME_MAX_LEN          (20)
        
// 最大、最少组个数
#ifndef MMIPB_GROUP_MANAGE_SUPPORT
#define MMIPB_USER_GROUP_NUM        (uint8)(0)//允许用户新增5个分组
#else
#ifdef MMI_PDA_SUPPORT
#define MMIPB_USER_GROUP_NUM        (uint8)(5)//允许用户新增5个分组
#else
#define MMIPB_USER_GROUP_NUM        (uint8)(10)//允许用户新增10个分组
#endif
#endif
#ifdef SNS_SUPPORT
#define MMIPB_SNS_GROUP_NUM         (uint8)(3)
#define MMIPB_MAX_GROUP_NUM		    (uint8)(MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM + MMIPB_SNS_GROUP_NUM)  // 10
#else
#define MMIPB_MAX_GROUP_NUM		    (uint8)(MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM)  // 10
#endif
#define MMIPB_SYSTEM_GROUP_NUM      (uint8)PB_GROUP_MAX

#define MMIPN_MAX_SNS_NUM           (uint8)(3000)
#define MMIPB_USER_GROUP_START      MMIPB_SYSTEM_GROUP_NUM

#define PB_NOT_EXIST                       0XFFFF//允许输入字符的最大个数

// 在 index_of_num 中有效的值

#define MMIPB_INDEX_OF_PHONE_0      0X00
#define MMIPB_INDEX_OF_PHONE_1      0X01
#define MMIPB_INDEX_OF_PHONE_2      0X02
#define MMIPB_INDEX_OF_PHONE_3      0X03
#define MMIPB_INDEX_OF_PHONE_4      0X04
#define MMIPB_INDEX_OF_MAIL         0X07
#define MMIPB_INDEX_OF_MAX          0X08  

// 为了获得手机中记录对应的某一个具体号码
#define MMIPB_FEILD_MASK_SINGLE_NUMBER    0X01


// 为了获得手机中记录对应的邮箱地址
#define MMIPB_FEILD_FLAG_MASK_MAIL		        0X80

// 记录姓名对应的域
#define MMIPB_FEILD_FLAG_MASK_NAME		        0X40
// 获得手机中记录对应的所有域
#define MMIPB_FEILD_FLAG_MASK_ALL		        0XFF
// 为了获得手机中记录对应的所有号码
#define MMIPB_FEILD_FLAG_MASK_ALL_NUM	        0X1F

// 所有分组
#define MMIPB_ALL_GROUP_MASK		        0XFFFFFFFF

//#define MMIPB_GROUP_FAVROUTE            31//计算分组的时候不计算此群组， 此群组只用于固定的保存收藏的联系人


#define MMIPB_MAX_NUM_TYPE              5
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//主要的PB的所有错误类型
typedef enum
{
	MMIPB_ERROR_SUCCESS,          //成功
	MMIPB_ERROR_ERROR         =1 ,//失败
	MMIPB_ERROR_PENDING       =2, //操作中
	MMIPB_ERROR_BUSY          =3,//上一个操作还没有结束
	MMIPB_ERROR_FILE_NOT_EXIST=4,//文件不存在
	MMIPB_ERROR_INVALID_PARAM =5,//参数错误
	   
	MMIPB_ERROR_INVALID_FORMAT=6,//格式错误
	MMIPB_ERROR_PART_SUCCESS  =7,//部分成功
    MMIPB_ERROR_NO_INITIAL    =8,//没有初始化
	MMIPB_ERROR_NO_ENTRY      =9,//没有记录
	MMIPB_ERROR_NO_NUMBER     =10,//没有号码
	MMIPB_ERROR_NO_MSISDN     =11,//没有本机号码
	MMIPB_ERROR_BLACKLIST_FULL=12,//黑名单满
	MMIPB_ERROR_NUMBER_ALERDY_EXIST=13,//号码已经存在
	MMIPB_ERROR_MAIL_LEN=14,//邮件长度错误
    MMIPB_ERROR_SIM_ERROR= 15,//sim卡错误
    MMIPB_ERROR_FDN_ENABLE = 16,//fdn开启
    MMIPB_ERROR_UDISK_ISRUN = 17,//U盘开启中
    MMIPB_ERROR_NO_DEV = 18, //磁盘不存在   

    MMIPB_ERROR_SFS_ERROR = 19,//文件系统错误
    MMIPB_ERROR_SMS_TOO_LARGE = 20,
    MMIPB_ERROR_INVALID_NUMBER = 21,//无效号码
    MMIPB_ERROR_NO_SPACE      =22,//空间不足 
    MMIPB_ERROR_PHONE_NO_SPACE =23, 
    MMIPB_ERROR_SIM1_NO_SPACE=24,
    MMIPB_ERROR_SIM2_NO_SPACE = 25,
    MMIPB_ERROR_SIM3_NO_SPACE = 26,
    MMIPB_ERROR_SIM4_NO_SPACE = 27,
    MMIPB_ERROR_FMM_NO_SPACE=28,
    MMIPB_ERROR_CANCEL=29,
    MMIPB_ERROR_BREAK=30,
    MMIPB_ERROR_MAX_LIMIT=31,
    MMIPB_ERROR_NO_SELECT_ENTRY=32,
    MMIPB_ERROR_INVALID_CONTACT = 33,
    MMIPB_ERROR_MAX
}MMIPB_ERROR_E;

typedef enum
{
    PB_GROUP_ALL,			//全部
//    PB_GROUP_UNCLASS,     //未分类  
    PB_GROUP_SIM_1,         //SIM卡1
    #ifndef MMI_MULTI_SIM_SYS_SINGLE
    PB_GROUP_SIM_2,             //SIM卡2
    #if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    PB_GROUP_SIM_3,             //SIM卡3
    #endif
    #if defined(MMI_MULTI_SIM_SYS_QUAD)
    PB_GROUP_SIM_4,             //SIM卡4
    #endif
    #endif
    PB_GROUP_HOME,			//家庭
    PB_GROUP_FRIEND,		//朋友
    PB_GROUP_COLLEAGUE,		//同事
    PB_GROUP_SCHOOLMATE,	//同学
#ifdef MMIPB_MOST_USED_SUPPORT    
    PB_GROUP_MOSTUSED,		//常用联系人
    PB_GROUP_FAVROUTE,
#endif
#ifdef BT_PBAP_SUPPORT
    PB_GROUP_BT,   //蓝牙联系人
#endif
    PB_GROUP_MAX
}PB_GROUP_LIST_E;

#ifdef SNS_SUPPORT
typedef enum
{
 MMIPB_GROUP_SNS_SINA = MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM,//预留给sns的3个群组位置
 MMIPB_GROUP_SNS_TIWTTER, 
 MMIPB_GROUP_SNS_FACEBOOK,
 MMIPB_GROUP_SNS_MAX
}PB_SNS_GROUP_LIST_E;
#endif
//AAS Type
typedef enum _MMI_AAS_TYPE_E
{
	MMI_AAS_OFFICE_PHONE,
#ifndef PB_SUPPORT_LOW_MEMORY
	MMI_AAS_HOME_PHONE,
	MMI_AAS_FAX_PHONE,
#endif
	MMI_AAS_PHONE_NUM
 } MMI_AAS_TYPE_E;

//SIM Type
typedef enum _MMI_SIM_TYPE_E
{
	MMI_SIM_TYPE_SIM,
	MMI_SIM_TYPE_USIM,
	MMI_SIM_TYPE_NUM
} MMI_SIM_TYPE_E;
//号码类型
#define MMIPB_INVLID_NUMBER   0x0 //无效号码或者号码不存在
#define MMIPB_ADN_NUMBER      0X01 //移动号码
#define MMIPB_HOME_NUMBER     0X02 //家庭号码
#define MMIPB_OFFICE_NUMBER   0X03 //办公号码
#define MMIPB_FAX_NUMBER      0X04 //传真号码
#define MMIPB_FIXED_NUMBER    0X05 //电话号码
#define MMIPB_UNKNOWN_NUMBER  0XFF //未知号码
//可以通过字符串查找的类型
typedef uint32 MMIPB_SEARCH_TYPE_T;

#define MMIPB_SEARCH_NONE   0x00 //不能进行字符串查找
#define MMIPB_SEARCH_NAME   0x01 //可以对名称进行字符串查找
#define MMIPB_SEARCH_NUMBER 0x02 //可以对号码进行字符串查找
#define MMIPB_SEARCH_EMAIL  0x04 //可以对email进行字符串查找
#ifdef MMIPB_SEARCH_CONTACT_CONTENT 
#define MMIPB_SEARCH_OTHER  0x08 //可以对ALL OTHER FIELD进行字符串查找
#endif

// SIM卡中最多可保存的记录数
#ifdef MMI_SIM_CONTACT_SUPPORT
#ifndef WIN32
#ifdef PB_SUPPORT_LOW_MEMORY
#define MMIPB_SIM_MAX_RECORD_NUM		(uint16)(300)   //500
#define MMIPB_SNS_MAX_RECORD_NUM		(uint16)(100)   //500
#else
#define MMIPB_SIM_MAX_RECORD_NUM		(uint16)(500)   //1000
#define MMIPB_SNS_MAX_RECORD_NUM		(uint16)(500)   //500
#endif
#else
#define MMIPB_SIM_MAX_RECORD_NUM		(uint16)(50) 
#define MMIPB_SNS_MAX_RECORD_NUM		(uint16)(50)   //500
#endif
#else
#define MMIPB_SIM_MAX_RECORD_NUM		(uint16)(0)
#define MMIPB_SNS_MAX_RECORD_NUM		(uint16)(0)
#endif

// 手机中电话簿记录的最大数
#ifndef WIN32
#if 1 //100  modifyed bao 优化内存
    #define MMIPB_NV_MAX_RECORD_NUM     10
#else
#ifdef PB_SUPPORT_LOW_MEMORY
//LOW MEMORY Platform
    #define MMIPB_NV_MAX_RECORD_NUM     300 //500
#elif NANDBOOT_SUPPORT
    #define MMIPB_NV_MAX_RECORD_NUM     1000
#else
    #define MMIPB_NV_MAX_RECORD_NUM     500
#endif
#endif
#else
    #define MMIPB_NV_MAX_RECORD_NUM     50
#endif
#ifdef MMIPB_MOST_USED_SUPPORT
//for cr NEWMS00172066
#ifdef CMCC_UI_STYLE

#ifdef MMIPB_NV_MAX_RECORD_NUM
#undef MMIPB_NV_MAX_RECORD_NUM
#endif

#define MMIPB_NV_MAX_RECORD_NUM     1500
#endif

#ifdef WIN32
#define MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS  4
#else
#define MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS  20//20个常用联系人
#endif
#endif

#ifdef TCARD_PB_SUPPORT
#ifndef WIN32
#define MMIPB_MAX_CONTACT_COUNT       1500
#else
#define MMIPB_MAX_CONTACT_COUNT       200
#endif
#define MMIPB_TCARD_MAX_RECORD_NUM  (MMIPB_MAX_CONTACT_COUNT - MMIPB_NV_MAX_RECORD_NUM)
#endif

// 电话簿中的号码
typedef struct MMIPB_BCD_NUMBER_T
{
    uint8   number_len;                         // number数组的BYTE个数
    uint8   npi_ton;                            // 具体意义请查看GSM 04.08
	uint8   number[MMIPB_BCD_NUMBER_MAX_LEN];	// 具体意义请查看GSM 04.08 
} MMIPB_BCD_NUMBER_T;

// 电话簿中的姓名
typedef struct _MMIPB_NAME_T
{
    uint16   wstr_len;                   
	wchar   wstr[MMIPB_NAME_MAX_LEN + 1];   
} MMIPB_NAME_T;

#endif






/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ′óBYTE?D??μ?NUMBER TYPE
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC MN_NUMBER_TYPE_E MMIPB_GetNumberTypeFromUint8(uint8   npi_ton );


#ifdef   __cplusplus
    }
#endif


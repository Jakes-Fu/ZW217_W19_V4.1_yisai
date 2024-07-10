/****************************************************************************
** File Name:      mmk_app.h                                               *
** Author:                                                                 *
** Date:           17/12/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2003        Great.Tian       Create
** 
****************************************************************************/
#ifndef MMI_COMMON_H
#define MMI_COMMON_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/


#include "sci_types.h"
#include "ref_common.h"

//#include "mmi_modu_main.h"
#include "mmicom_string.h"
#include "mmicom_data.h"
#include "mmicom_time.h"
//#include "guianim.h"
#include "mmk_type.h"
//#include "guiwin.h"
#include "freq_cfg.h"
//#include "mmipb_adapt.h"
#include "mmi_filemgr.h"
#include "mmisrvaud_api.h"
#include "mmipb_common.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMICOM_READ_BYTE(ptr, position)   (ptr[position])
#define MMICOM_READ_2BYTE(ptr, position)   (ptr[position] + (ptr[position+1] << 8))
#define MMICOM_MAKELONG( a, b ) \
        ((uint32)(((uint32)(a)&0x0000ffff) | ((uint32)(b)) << 16 ))
#define MMICOM_HIWORD( l ) \
        ((uint16)(((uint32)( l ) >> 16 ) & 0xFFFF ))
#define MMICOM_LOWORD( l ) \
        ((uint16)( l ))

/*
#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
             MMI_WriteNVItem(_item_id, _addr)
			 */

#define ALLOCMEM(men_ptr,mem_len)  \
    if( PNULL != men_ptr )\
    {\
        SCI_FREE(men_ptr);\
    }\
    men_ptr  = SCI_ALLOC_APP(mem_len);\
    SCI_MEMSET(men_ptr, 0, mem_len);\
//========================
#define RGB32_2_RGB16(TXTcolor, color)  { \
    uint8 red   = color&0x000000FF; \
    uint8 green = (color&0x0000FF00) >> 8; \
    uint8 blue  = (color&0x00FF0000) >> 16; \
    \
    TXTcolor = (GUI_COLOR_T)((((red/8)<<11)+((green/4)<<5)+(blue/8))); \
    }

/*lint -e767*/
#ifdef ABS
#undef ABS
#endif

#define ABS(x) (((x) >= 0) ? (x) : (-(x)))
/*lint +e767*/
//#define UNICODE_ELLIPSIS                  "\x00\x2e\x00\x2e\x00\x2e"    //"..."
//#define UNICODE_ELLIPSIS_LEN              6
#define IMAGE_NULL          COMMON_IMAGE_NULL

#define MMI_100MSECONDS 100
#define MMI_200MSECONDS 200
#define MMI_300MSECONDS 300
#define MMI_500MSECONDS 500// 500 ms
#define MMI_1SECONDS    1000//3 second
#define MMI_2SECONDS    2000//2 second  //@xingdong.li
#define MMI_3SECONDS    3000//3 second
#define MMI_10SECONDS    10000//3 second
#ifdef MMI_ETWS_SUPPORT
#define MMI_30SECONDS    30000//30 second
#define MMI_5SECONDS    5000//5 second
#define MMI_10500MSECONDS    10500//10.5 second
#define MMI_5MINUTES    300000//5 minutes
#endif

#define MMI_DEADLOOP_CHECK_ENABLE

//记录死循环之前最后执行到的文件和行号
#ifdef MMI_DEADLOOP_CHECK_ENABLE
    #define __MMI_START_DEADLOOP_CHECK__(expired_time)      MMIAPICOM_StartDeadLoopCheck(expired_time)
    #define __MMI_END_DEADLOOP_CHECK__()                    MMIAPICOM_EndDeadLoopCheck()
    #define __MMI_PRINT_DEADLOOP_CHECK__()                  MMIAPICOM_PrintDeadLoopCheck(__FILE__, __LINE__)
#else
    #define __MMI_START_DEADLOOP_CHECK__(expired_time)
    #define __MMI_END_DEADLOOP_CHECK__()
    #define __MMI_PRINT_DEADLOOP_CHECK__()
#endif

#define mmi_crc16  REF_Crc16

#define MODEM_VERSION_STR_LEN 256

typedef struct _MMI_PARTY_NUMBER_T 
{
    MN_NUMBER_TYPE_E  number_type;
    uint16            num_len;
    uint8             bcd_num[MN_MAX_ADDR_BCD_LEN];
} MMI_PARTY_NUMBER_T;

typedef struct
{
    BOOLEAN                 is_valid;       //该项有效
    MMI_WIN_ID_T            win_id;         //window id
    MMI_MENU_ID_T           menu_id;        //组内索引
    MMI_MENU_GROUP_ID_T     group_id;       //菜单组id
} MMI_MENU_GRAYED_T;

//转换窗口位置类型

typedef struct {
    uint32 state[4];                                   /* state (ABCD) */
    uint32 count[2];        /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];                         /* input buffer */
} MD5_CONTEXT;
typedef enum
{
    MMICOM_WINPOS_WIN2LCD,          //相对于窗口位置转换为相对于lcd位置
    MMICOM_WINPOS_LCD2WIN,          //相对于lcd位置转换为相对于窗口位置
    MMICOM_WINPOS_WIN2DISP,         //相对于窗口位置转换为相对于窗口所在图层位置
    MMICOM_WINPOS_DISP2WIN,         //相对于窗口所在图层位置转换为相对于窗口位置

    MMICOM_WINPOS_CONVERT_MAX
}MMICOM_WINPOS_CONVERT_E;

typedef enum
{
    MMICOM_RETURN_NONE = -1,       // no option
    MMICOM_RETURN_DECREASE,    // decrease
    MMICOM_RETURN_INCREASE   // increase  
} MMICOM_RETURN_OPTION_E;

typedef enum
{
    MMICOM_VIDEO_TYPE_3GP,  //0x00
    MMICOM_VIDEO_TYPE_MP4,  //0x01
    MMICOM_VIDEO_TYPE_AVI,  //0x02
    MMICOM_VIDEO_TYPE_FLV,  //0x03
    MMICOM_VIDEO_TYPE_RMVB, //0x04
#ifdef DRM_SUPPORT
    MMICOM_VIDEO_TYPE_DRM,           //DRM文件后缀名
    MMICOM_VIDEO_TYPE_DCF,           //DCF文件后缀名
#endif
#ifdef MMI_KING_MOVIE_SUPPORT
    MMICOM_VIDEO_TYPE_KMV,
    MMICOM_VIDEO_TYPE_SMV,  //smv_support
#ifdef MMI_KING_MOVIE_SUPPORT_HMV
    MMICOM_VIDEO_TYPE_HMV,  //hmv_support
#endif
#endif
    MMICOM_VIDEO_TYPE_MAX
}MMICOM_VIDEO_TYPE_E;

//二分法查找的类型
typedef enum
{
    MMICOM_SEARCH_FIRST_EQUAL,//查找最前面的一个相同的退出
    MMICOM_SEARCH_LAST_EQUAL,//查找最后的一个相同的退出
    MMICOM_SEARCH_ANY_EQUAL,//查找任意一个相同的退出
}MMI_BIN_SEARCH_TYPE_E;
/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/

typedef int (* BIN_COMPARE_FUNC)(uint32 base_index, void *compare_base_data, void *list);

// typedef struct _MMI_COMPARE_FUNC_T
// {
//     BIN_COMPARE_FUNC func;
// }MMI_COMPARE_FUNC_T;

typedef struct _MMI_BIN_SEARCH_INFO_T
{
    int32  start_pos;
	int32  end_pos;
	void    *compare_base_data;
	MMI_BIN_SEARCH_TYPE_E search_type;
}MMI_BIN_SEARCH_INFO_T;

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : using bin search to find the record in list
//  Global resource dependence : 
//  Parameter: search_info:(IN) search_info->start_pos:the start index to search;
//                              search_info->end_pos:the end index of search;
//                              search_info->b_is_backward:search from ahead or not;  
//             func_t:(IN)      func_t->func: compared function,this is a callback function,
//                                            be defined by every caller;
//                              func_t->compare_data_2:(it can be null) the data(string or value) to be compare with list
//                              func_t->list_ptr:(it can be null)search list
//             pos:             if find,it is the postion of searched record; 
//                              if not find, it is the postion of the first larger to func_t->compare_data_2  
//  Note: 
//  return : 0: find this record in func_t->list_ptr, !=0 not find in func_t->list_ptr      
/*****************************************************************************/
PUBLIC int MMIAPICOM_BinSearch(MMI_BIN_SEARCH_INFO_T *search_info, //[IN]
                           BIN_COMPARE_FUNC func, //[IN]
                           uint32 *pos, //[OUT]
                           void *list//[IN]
                           );

/*****************************************************************************/
//  Description : Generate to the string number
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_GenDispNumber(
                         MN_NUMBER_TYPE_E  number_type,
                         uint8             party_len,
                         uint8             *party_num,
                         uint8             *tele_num,
                         uint16            max_tele_len //!!!!!!!!!可显示的号码的最大长度 + 2(/0 字符串结束符号)
                         );

/*****************************************************************************/
//  Description : for telenumber to translate the string to bcd
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_ConvertWStr2Num(
                                        wchar *wnum_str,
										uint16 wstr_len,
                                        MMIPB_BCD_NUMBER_T *bcdnum_ptr
                                        );

/*****************************************************************************/
//  Description : convert bcd number to wchar
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_BcdToWchar(
                                    wchar *wnum_str, //[OUT]
                                    uint16 *wstr_len_ptr,//[OUT]
                                    uint16 max_wstr_len,//[IN]
                                    MMIPB_BCD_NUMBER_T *bcdnum_ptr//[IN]                                                                        
                                    );

/*****************************************************************************/
// Description: The MMIAPICOM_bcdsstr function returns a pointer to the first occurrence of 
//              bcd_strset in bcd_string. 
// Parameters:  bcd_number: bcd number string to search 
//              bcd_numset:bcd number string to search for 
// Return:      TRUE: find bcd_numset in bcd_number; FALSE:not find bcd_numset in bcd_number
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_bcdsstr(const MMIPB_BCD_NUMBER_T *bcd_number, //IN:
								 MMIPB_BCD_NUMBER_T *bcd_numset//IN:
								 );

/*****************************************************************************/
//  Description : Generate to the string number
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_GenNetDispNumber(
                         MN_NUMBER_TYPE_E  number_type,
                         uint8             party_len,
                         uint8             *party_num,
                         uint8              *tele_num,
                         uint8              max_tele_len //!!!!!!!!!可显示的号码的最大长度 + 2(/0 字符串结束符号)
                         );
/*****************************************************************************/
//  Description : for telenumber to translate the string to bcd
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_GenPartyNumber(
                             uint8              *tele_num_ptr,
                             int16              tele_len,
                             MMI_PARTY_NUMBER_T *party_num
                             );
/*****************************************************************************/
//  Description : Delete all fils in dir and dir
//  Global resource dependence : none
//  Author:louis.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_DelDirAll(
                                   const wchar* path_ptr, //dir path
                                   uint32        path_buf_len //Buffer len  
                                   );
/*****************************************************************************/
//  Description :change enum type and plan of number to uint8 npi_ton,请参考PB
//  Global resource dependence : none
//  Author: jassmine.meng
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICOM_ChangeTypePlanToUint8(
                                MN_NUMBER_TYPE_E    number_type,
                                MN_NUMBER_PLAN_E    number_plan
                                );

/*****************************************************************************/
//  Description : get nosuffix_name and suffix
//  Global resource dependence :                                
//      Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_SplitSuffix(
                        wchar   *nosuffix_name_ptr,    //out
                        uint16  wlen,
                        wchar    *suffix_ptr,           //out
                        uint16  clen,
                        wchar   *addsuffix_name_ptr    //in
                        );

/*****************************************************************************/
//  Description : set menu gray
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_EnableGrayed(
                                      MMI_WIN_ID_T          win_id,
                                      MMI_MENU_GROUP_ID_T   group_id,
                                      MMI_MENU_ID_T         menu_id,
                                      BOOLEAN               is_gray //TRUE:变灰
                                      );

/*****************************************************************************/
//  Description : menu is grayed
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsGrayed(
                                  MMI_WIN_ID_T          win_id,
                                  MMI_MENU_GROUP_ID_T   group_id,
                                  MMI_MENU_ID_T         menu_id
                                  );

/*****************************************************************************/
//  Description : free all menu grayed by window id
//  Global resource dependence : 
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_FreeGrayed(
                                 MMI_WIN_ID_T    win_id
                                 );

/******************************************************************************/
/*! \brief get the invalid ctrl id
 *  \author jibin 
 *  \return invalid ctrl id
 */
/******************************************************************************/
PUBLIC MMI_CTRL_ID_T MMIAPICOM_GetInvalidCtrlId(void);

/******************************************************************************/
/*! \brief get the invalid window id
 *  \author jibin 
 *  \return invalid window id
 */
/******************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPICOM_GetInvalidWinId(void);

/*****************************************************************************/
//  Description : 其他task向mmi发送消息
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:使用MMK_PostMsg和MMIAPICOM_TriggerMMITask存在风险, 用此函数代替
/*****************************************************************************/
PUBLIC void MMIAPICOM_OtherTaskToMMI( 
                                     MMI_HANDLE_T     handle,
                                     MMI_MESSAGE_ID_E msg_id,
                                     DPARAM           param_ptr,
                                     uint32           size_of_param
                                     );

/*****************************************************************************/
//  Description : trigger the mmi task
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_TriggerMMITask(void);

/******************************************************************************
 * 函数定义:    MMIAPICOM_IsPointValid 判断一个point是否有效
 * 函数说明：   
 * 参数说明：   坐标点
 * 返回值：     BOOLEAN
 * NOTE:        xiaoqing.lu
********************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsPointValid(
                           GUI_POINT_T point
                           );


/*****************************************************************************/
//  Description : trigger the mmi task
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MMI_TriggerMMITask(void);

/*****************************************************************************/
//  Discription: This function is used to send signal from mmi to mmi
//   
//  Global resource dependence: None
//  Author: baokun yin
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPICOM_SendSignalToMMITask(uint32 sig_id);

/******************************************************************************/
/*! \brief  turn on lcd back light and keyboard light
 *  \author Bruce.Chi  
 *  \param[in]  void
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_TurnOnBackLight(void);

/******************************************************************************/
/*! \brief  turn on lcd back light
 *  \author Bruce.Chi  
 *  \param[in]  void
 */
/******************************************************************************/
PUBLIC void MMIDEFAULT_TurnOnLCDBackLight(void);
/*****************************************************************************/
//  Description : get lcd device infomation 
//  Global resource dependence : 
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetLcdDevInfo(GUI_LCD_DEV_INFO *lcd_dev_info);

/*****************************************************************************/
//  Description : display backgroud of input screen for time inoput of ALARM
//  Global resource dependence : 
//  Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPICOM_DisplayBackground(
                                MMI_WIN_ID_T    win_id, 
                                uint32         x,
                                uint32         y,
                                uint32          icon_id
                               );

/*****************************************************************************/
//  Discription:get file name suffix from the full file name 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetLongFileNameAndSuffix( 
                                               const wchar *ucs_full_name_ptr,      //IN
                                               wchar        *ucs_suffix_ptr,        //OUT
                                               wchar        *ucs_name_ptr           //OUT
                                               );

/*****************************************************************************/
//  Description : get the full path according to the full file name
//  Global resource dependence : 
//  Author:Xiaoqing.lu
//  Note: 本接口的作用是获取文件名（包含全路径）中的路径名
//  Example: file_name_ptr[d:\com\test\test.exe],输出参数file_path_ptr[d:\com\test]
/*****************************************************************************/
BOOLEAN MMIAPICOM_GetFilePath(
                              const wchar *file_name_ptr, // [IN]
                              uint16 file_name_len,  // [IN]
                              wchar *file_path_ptr, // [OUT]
                              uint16 *file_path_len  // [OUT]
                              );
/*****************************************************************************/
// Description : get valid file name 
// Global resource dependence : 
// Author: jian.ma
// Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPICOM_GetValidFileName(
										  BOOLEAN	is_complete_name,	//in:名字后面是否还有字符,比如后缀等
										  wchar		*file_name,			//in
										  uint32	file_len,			//in
										  wchar		*new_file_name,		//out
										  uint32	new_file_len		//in
										  );

/*****************************************************************************/
//  Discription:get full path name by file name with suffix 
//  Global resource dependence: none 
//  Parameter : filedev: file system staroge
//              file_name_ptr: file name, if this file name include "/" or "\\", the full_path_ptr will return file_name_ptr
//                             if  this file name not include "/", the full_path_ptr will be combine by this file type
//              file_name_len: file name length
//              full_path_ptr : file full path name, it will return to user
//              full_path_max_len: file full path name max. length, if the combined file full path name will be limited full_path_max_len  
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_CombineDefaultFullPathByFilename(
                                                MMIFILE_DEVICE_E filedev,            //IN
                                                const wchar     *file_name_ptr,  //IN
                                                uint16           file_name_len,  //IN
                                                wchar *full_path_ptr,             //OUT
                                                uint16 full_path_max_len         //IN
                                                );

/*****************************************************************************/
//  Discription: check full_path_name exist, auto rename full_path_name till file name not exist 
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetNoExistFileName(wchar *full_path_name,
                                        uint16 file_name_len);

/*****************************************************************************/
//  Discription: check full_path_name exist, auto rename full_path_name till file name not exist 
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_AutoRenameFileName(wchar *full_path_name,
                                           uint16 file_name_len);
										   
/*****************************************************************************/
//  Description : get the full path according to the full file name
//  Global resource dependence : 
//  Author:Xiaoqing.lu
//  Note: 
//  Example: file_name_ptr[d:\com\test\test.exe],输出参数file_path_ptr[test.exe]
/*****************************************************************************/
BOOLEAN MMIAPICOM_GetFileName(
                              const uint16 *src_name_ptr, // [IN]
                              uint16 src_name_len,  // [IN]
                              uint16 *des_path_ptr, // [OUT]
                              uint16 *des_path_len  // [OUT]
                              );


/*****************************************************************************/
//  Description : combine pathname with two name of dir_ptr and filename
//  Global resource dependence : 
//  Author:Xiaoqing.lu
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPICOM_CombinePath(
                              const wchar      *dir_ptr,           //in
                              uint16           dir_len,            //in, 双字节为单位
                              const wchar      *file_name_ptr,     //in, can be NULL
                              uint16           file_name_len,      //in, 双字节为单位
                              BOOLEAN          is_extname,         //in, 是否为扩展名
                              wchar            *full_path_ptr,     //out
                              uint16           *full_path_len_ptr  //out, 双字节为单位
                              );

/*****************************************************************************/
//  Discription: If the file is the same type with the type array
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsSameFileType( 
                                        uint16 * file_suffix_ptr,        //new file suffix
                                        uint8  * type_suffix_ptr        //file type suffix array
                                        );

/*****************************************************************************/
//  Discription: If the file is the same type with the type array
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsKingMovieType(
                                             const wchar *file_name_ptr,
                                             uint16      name_len
                                             );

/*****************************************************************************/
//  Description : 获得没有后缀名的文件名称，以及后缀名
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC void MMIAPICOM_GetNameAndSuffix(
                                       const wchar     *name_suffix_ptr,   //文件名
                                       uint16          name_suffix_len,    //文件名长度
                                       MMI_STRING_T    *name_ptr,          //没有后缀名的文件名
                                       MMI_STRING_T    *suffix_ptr         //后缀名
                                       );

/*****************************************************************************/
//  Description : 文件名是否有效
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIAPICOM_FileNameIsValid(
                                         MMI_STRING_T    *file_name_ptr  //in:
                                         );

/*****************************************************************************/
//  Description : get image file type
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC GUIANIM_TYPE_E MMIAPICOM_GetImgaeType(
                                             const wchar  *full_path_ptr, //in
                                             uint16       full_path_len   //in
                                             );
                                      
/*****************************************************************************/
//  Description : get music file type
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E  MMIAPICOM_GetMusicType(
                                               const wchar *file_name_ptr,
                                               uint16    name_len
                                               );

/*****************************************************************************/
//  Description : get music file type(including drm)
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E  MMIAPICOM_GetMusicTypeEx(
                                               const wchar    *file_name_ptr,
                                               uint16    name_len,
                                               BOOLEAN need_check_drm
                                               );

/*****************************************************************************/
//  Description : get movie file type
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC MMICOM_VIDEO_TYPE_E  MMIAPICOM_GetMovieType(
                                             const wchar *file_name_ptr,
                                             uint16    name_len
                                             );

/*****************************************************************************/
//  Discription:get file path by full path name 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetFilePathNameByFullPath( 
                                                const wchar *ucs_full_name_ptr,     //IN
                                                wchar       *ucs_name_ptr           //OUT
                                                );

/*****************************************************************************/
//  Discription:get file path by full path name 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetLongFilePathNameByFullPath( 
                                                    const wchar *ucs_full_name_ptr,     //IN
                                                    wchar *ucs_name_ptr         //OUT
                                                    );

/*****************************************************************************/
//  Discription:get file with suffix by full path name 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICOM_GetLongFileNameByFullPath( 
                                                const wchar     *ucs_full_name_ptr,     //IN
                                                wchar       *ucs_name_ptr           //OUT
                                                );

/*****************************************************************************/
//  Description : check the string is phone number or not
//  Global resource dependence : none
//  Return : TRUE: the string is phone number; FALSE: the string is not phone number
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsPhoneNumber(
                             uint8              *tele_num_ptr,
                             int16              tele_len
                             );

/*****************************************************************************/
//  Description : 获得随机文件名字
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_CreateRandFile(
                                         const wchar        *path_ptr,  //in，路径
                                         uint16     path_len,   //in, 双字节单位
                                         uint8      *prefix_str_ptr,    //in，文件名前缀，可以是NULL
                                         uint8      *suffix_str_ptr,    //in，文件名后缀，可以是NULL
                                         wchar      *file_name_ptr,     //out，输出文件名
                                         uint16     *file_name_len_ptr  //out，双字节单位
                                         );

/*****************************************************************************/
//  Description : 创建一个随机名字的文件
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIAPICOM_CreateRandFileAsyn(
                                        const wchar     *path_ptr,  //in，路径
                                        uint16      path_len,   //in, 双字节单位
                                        uint8       *prefix_str_ptr,    //in，文件名前缀，可以是NULL
                                        uint8       *suffix_str_ptr,    //in，文件名后缀，可以是NULL
                                        wchar       *file_name_ptr,     //out，输出文件名
                                        uint16      *file_name_len_ptr  //out，双字节单位
                                        );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:将wstr trace输出
/*****************************************************************************/
PUBLIC void MMIAPICOM_WstrTraceOut(
                                   const wchar* src,
                                   size_t count
                                   );

#ifdef MMI_DEADLOOP_CHECK_ENABLE
/*****************************************************************************/
//  Description : start deadloop check
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_StartDeadLoopCheck(
                                         uint32 expired_time
                                         );

/*****************************************************************************/
//  Description : end deadloop check
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_EndDeadLoopCheck(void);

/*****************************************************************************/
//  Description : print deadloop check
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_PrintDeadLoopCheck(
                                         char *file,    //文件名
                                         uint32 line    //行号
                                         );
/**********************************************************************
//    Description: the function to judge whether increase or decrease
//    Global resource dependence :
//    Author: jian.ma
//    Note:when *# align Left/Right，Left * decrease and right # increase;
//         when *# align up/down，up * increase and down # decrease;
***********************************************************************/
PUBLIC MMICOM_RETURN_OPTION_E MMIAPICOM_IsIncrease(uint32 key);

/*****************************************************************************/
//  Description : Set systerm frequency for mmi app
//  Global resource dependence :                     
//  Author: Xiaoqing.Lu
//  Note: 
/*****************************************************************************/
//PUBLIC uint32 MMI_SetFreq(PROD_FREQ_INDEX_E freq);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: (!!!) this function return free space the unit is K bytes not byte
//  return: return space size by K bytes
/*****************************************************************************/
PUBLIC uint32  MMI_GetFreeSpace(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:zhaohui
//  Note: get user agent
//  return: 
/*****************************************************************************/
PUBLIC char* MMIAPICOM_GetUserAgent(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:kelly.li
//  Note: get user agent profile info
//  return: function caller should not free the buffer
/*****************************************************************************/
PUBLIC char* MMIAPICOM_GetUserAgentProfile(void);
#endif
 
/*****************************************************************************/
//  Description : Get systerm frequency for mmi app
//  Global resource dependence :                     
//  Author: Xiaoqing.Lu
//  Note: 
/*****************************************************************************/ 
PUBLIC uint32 MMI_RestoreFreq(void);

/*****************************************************************************/
//     Description : convert win rect
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMI_ConvertWinRect( 
                              MMICOM_WINPOS_CONVERT_E convert_type,
                              MMI_HANDLE_T   win_handle,
                              GUI_RECT_T rect
                              );

/*****************************************************************************/
//     Description : convert win point
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMI_ConvertWinPoint( 
                              MMICOM_WINPOS_CONVERT_E convert_type,
                              MMI_HANDLE_T   win_handle,
                              GUI_POINT_T point
                              );


/*****************************************************************************/
// Description: The MMIAPICOM_bcdInbcdHead function number2 in number1 from head. 
// Parameters:  number1: bcd number string to cmp 
//              number2:bcd number string to cmp 
// Return:      TRUE:number2 in number1 from head; FALSE: number2 not in number1 from head
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_bcdInbcdHead(const MMIPB_BCD_NUMBER_T *number1,//IN:
                                      MMIPB_BCD_NUMBER_T *number2//IN:
                                      );
/*****************************************************************************/
//     Description : 将原始图像数据输出为bmp格式图片
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICOM_WriteBMPFile(const wchar *u16filename, uint32 w, uint32 h, uint8 *pix, uint32 ColorDepth);

#ifdef MMI_DEBUGLAYER_SUPPORT
/*****************************************************************************/
//  Description : create debug layer
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_CreateDebugLayer(void);

/*****************************************************************************/
//  Description : output debug text
//  Global resource dependence : 
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICOM_OutputDebugText(
                           uint32 line_index,
                           const MMI_STRING_T *str_ptr
                           );
#endif

/*****************************************************************************/
//  Description : change freq by quick key
//  Global resource dependence :                          
//  Author: bin.ji
//  Modify: wei.zhou, for local to public
//  Note: 
/*****************************************************************************/
void MMI_ChangeFreqByQuickKey(void);

/*****************************************************************************/
//  Description : Get the audio priority by audio type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_PRI_T MMIAPICOM_GetAudioPri(char *audio_type_str);

/*****************************************************************************/
//  Description : get data and time string.
//  Global resource dependence : 
//  Author:baokun.yin
//  Note: yearmonday_hourminsec
/*****************************************************************************/
PUBLIC wchar *MMIAPICOM_GetDataTimeString(wchar *datatime_str,//OUT
                                          uint16 str_max_len
                                          );

/*****************************************************************************/
// 	Description : convert qwertykey msg to typicalkey msg
//	Global resource dependence : 
//  Author: aoke.hu
//	Note:   
/*****************************************************************************/
PUBLIC MMI_MESSAGE_ID_E MMIAPICOM_ConvertKeyMsg(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : Convert the stream to MD5 context, use MD5 arithmetic
//	Global resource dependence : 
//  Author:juan.wu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPICOM_ProduceSgnMD5Str(uint8 *secreat, int32 scrlen, uint8 *sign);

/*****************************************************************************/
//  Description : get modem version 
//  [IN] :the pointer for copy modem version string
//  retrun: the modem version string length
//  Author:xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPI_GetModemVersion(uint8 *pModeVersion);


#undef EXT
#endif  /* FILENAME_H */

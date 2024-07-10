/****************************************************************************
** File Name:      mmiunitconversion.c                              
** Author:           renyi.hu                                                    
** Date:            2010/04/21
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to describe functions of unit conversion
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 21/04/2010     renyi.hu          Create

****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_accessory_trc.h"
#include "sci_types.h" 
#ifdef  MMI_UNITCONVERSION_SUPPORT
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmk_msg.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_theme.h"
#include "guicommon.h"
#include "guitext.h"
#include "guiedit.h"
#include "guimenu.h"
#include "mmipub.h"
#include "mmi_menutable.h"
#include "mmiacc_id.h"
#include "guilabel.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "guires.h"
#include "mmiacc_position.h"
#include "mmiunitconversion_export.h"
#include "guiform.h"
#include "guisetlist.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_128X160
#define  MMIUC_INPUT_MAX_LEN  4  //编辑框输入最大字符数
#define  MMIUC_OUTPUT_MAX_LEN 9    //最大输出长度
#else
#define  MMIUC_INPUT_MAX_LEN  7  //编辑框输入最大字符数
#define  MMIUC_OUTPUT_MAX_LEN 13    //最大输出长度
#endif
#define  MMIUC_PRECISION_NUM  9  //换算结果保留小数位数
#define  MMIUC_MAX_EXPAND_NUM  10000000  //数值扩大的上限
#define  MMIUC_DECIMAL_NUM 10  
#define  MMIUC_MAX_STR_BUFF_LEN 30    //字符buff大小
#define  MMIUC_INTEGER_BUFF_LEN 10    //存放转换率整数部分buff大小
#define  MMIUC_DECIMAL_BUFF_LEN 10   //存放转换率小数部分buff大小
#define  MMIUC_MOD_BUFF_LEN  15  //存放余数buff大小

#define  MMIUC_DOT_CHAR  		'.'
#define  MMIUC_ZERO_CHAR  		'0'
#define  MMIUC_FIVE_CHAR  		'5'
#define  MMIUC_NINE_CHAR  		'9'

#define 	MMIUC_LENGTH_TYPE_NUM  4   //长度转换类型数量
#define 	MMIUC_WEIGHT_TYPE_NUM  2   //重量转换类型数量

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
//如果需要扩展该枚举,注意是否需要
//修改宏:MMIUC_WEIGHT_TYPE_NUM
typedef enum
{
    //重量转换类型
    MMIUC_TYPE_KG_POUND ,
    MMIUC_TYPE_POUND_KG ,
    MMIUC_TYPE_KG_OUNCE , 
    MMIUC_TYPE_OUNCE_KG  ,

    //长度转换类型
    MMIUC_TYPE_KM_MILE , 
    MMIUC_TYPE_MILE_KM  ,
    MMIUC_TYPE_M_YARD ,
    MMIUC_TYPE_YARD_M   ,
    MMIUC_TYPE_M_FOOT, 
    MMIUC_TYPE_FOOT_M ,   
    MMIUC_TYPE_CM_INCH , 
    MMIUC_TYPE_INCH_CM  ,

    MMIUC_TYPE_MAX
}UC_CONV_TYPE_E;

typedef enum
{
    UC_TYPE_WEIGHT,
    UC_TYPE_LENGTH,
    UC_TYPE_MAX
}UC_TYPE_E;

typedef enum
{
    UC_WEIGHT_KILOGRAM,
    UC_WEIGHT_POUND,
    UC_WEIGHT_OUNCE,
    UC_WEIGHT_MAX
}UC_WEIGHT_E;

typedef enum
{
    UC_LENGTH_KILOMETER,
    UC_LENGTH_METER,
    UC_LENGTH_CENTIMETER,
    UC_LENGTH_MILE,
    UC_LENGTH_YARD,
    UC_LENGTH_FOOT,
    UC_LENGTH_INCH,
    UC_LENGTH_MAX
}UC_LENGTH_E;

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                               */
/*---------------------------------------------------------------------------*/
LOCAL UC_TYPE_E s_uc_type=UC_TYPE_MAX;
LOCAL BOOLEAN s_type_pressed = FALSE;

#ifdef MMI_PDA_SUPPORT
LOCAL uint16 s_picker1_selected = 0;
LOCAL uint16 s_picker2_selected = 0;
LOCAL wchar s_result_buffer[MMIUC_OUTPUT_MAX_LEN + 1] = {0};  

LOCAL const char  *s_weight_conversion_rate[UC_WEIGHT_MAX][UC_WEIGHT_MAX]=
{
	"1.0",           "2.2046",      "35.2740",	                
	"0.4536",    "1.0",             "16.0",
	"0.0283",    "0.0625",      "1.0",
};

LOCAL const char  *s_length_conversion_rate[UC_LENGTH_MAX][UC_LENGTH_MAX]=//CR147456
{    
	"1.0",                "1000.0",     "100000.0",    "0.62137",             "1093.6",          "3280.8",           "39370.0",                  
	"0.001",	         "1.0",            "100.0",          "0.00062137",      "1.0936",          "3.2808",            "39.37",                                   
        "0.00001",       "0.01",          "1.0",                "0.0000062137",  "0.010936",     "0.032808",        "0.3937",               
        "1.6093",         "1609.3",      "160934.4",     "1.0",                    "1760.0",         "5280.0",            "63360.0",                          
	"0.0009144",   "0.9144",    "91.44",           "0.0005682",       "1.0",                "3.0",                   "36.0",                   
	"0.0003048",   "0.3048",    "30.48",            "0.0001894",      "0.3333",         "1.0",                   "12.0",                      
	"0.0000254",   "0.0254",    "2.5400",         "0.00001578",      "0.02778",     "0.08333",          "1.0",                  
};

const  uint32 s_weight_text_id[UC_WEIGHT_MAX] =
{
    TXT_UC_KILOGRAM,
    TXT_UC_POUND,
    TXT_UC_OUNCE,
};

const  uint32 s_length_text_id[UC_LENGTH_MAX] =
{    
    TXT_UC_KILOMETER,
    TXT_UC_METER,    
    TXT_UC_CENTIMETER,    
    TXT_UC_MILE,
    TXT_UC_YARD,
    TXT_UC_FOOT,
    TXT_UC_INCH,
};

#else
LOCAL UC_CONV_TYPE_E s_conversion_type=MMIUC_TYPE_MAX;//转换类型
LOCAL BOOLEAN s_conversion_type_pressed = FALSE;

//不同单位之间的换算关系，对应关系参看枚举UC_CONV_TYPE_E；
LOCAL const char  *s_conversion_rate[]=
{
	"2.2046",	
	"0.4536",
	"35.2740",
	"0.0283",
	
	"0.62137",//CR147456
	"1.6093",
	"1.0936",
	"0.9144",
	"3.2808",
	"0.3048",
	"0.3937",
	"2.5400",
};

const  uint32 s_weight_text_id[MMIUC_WEIGHT_TYPE_NUM] =
{
    TXT_UC_KILOGRAM_POUND,
    TXT_UC_KILOGRAM_OUNCE,
};

const  uint32 s_length_text_id[MMIUC_LENGTH_TYPE_NUM] =
{
    TXT_UC_KILOMETER_MILE,
    TXT_UC_METER_YARD,
    TXT_UC_METER_FOOT,
    TXT_UC_CENTIMETER_INCH,
};
#endif

//LOCAL uint32 s_last_setlist_index=0;//记录上次setlist index的值
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN InputIsValid(
							MMI_STRING_T const *in_str_ptr //in
							);

/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN StrIsValid(
							MMI_STRING_T const *in_str_ptr //in
							);

/*****************************************************************************/
//  Description : display uc type
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayUcType(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : handle owndraw tp down
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpDown(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                );

/*****************************************************************************/
//  Description : handle owndraw tp up
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpUp(
                              MMI_HANDLE_T ctrl_handle,
                              GUI_POINT_T tp_point
                              );

/*****************************************************************************/
//  Description : handle owndraw tp move
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpMove(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                );

/*****************************************************************************/
//  Discription:  handle change type win
//  Global resource dependence: none 
//  Author:paul.huang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleChangeTypeWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );

#ifdef MMI_PDA_SUPPORT
 /*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  UnitConversion (
							uint16  selected1,//in
							uint16  selectde2,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							);

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 LOCAL BOOLEAN  UnitConversionByRate(
									 const MMI_STRING_T  *in_str, //in 
									 const char *conv_rate, // in 
									 MMI_STRING_T  *out_str //out
									 );

/*****************************************************************************/
//  Description : display result
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayResultString(GUIOWNDRAW_INFO_T *owner_draw_ptr);
/*****************************************************************************/
//  Description : Display H Line
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayLine(void);
#else
/*****************************************************************************/
// 	Description : 设置单位换算显示时各种ctrl、字体等的属性-》只用于计算自适应str使用
//	Global resource dependence :
//  Author:liyan.zhu
//	Note:CR152753
/*****************************************************************************/
LOCAL MMI_RESULT_E ComputeUnitCoversionAdjustSet(
												 MMI_STRING_T    *in_str_ptr,
												 MMI_STRING_T    *out_str_ptr   //可显示下的宽度            
												 );

/*****************************************************************************/
// 	Description : 获取一个字符串显示时的自适应string
//	Global resource dependence :
//  Author:liyan.zhu
//	Note:CR152753
/*****************************************************************************/
LOCAL MMI_RESULT_E ComputAdjustStr(
								   MMI_STRING_T    *in_str_ptr,
								   uint16          ctrl_width,//string显示要小于的宽度
								   GUISTR_STYLE_T  font_style,//字体大小等描述
								   GUISTR_STATE_T  state,//字符串类型
								   MMI_STRING_T    *out_str_ptr   //可显示下的宽度            
);
/*****************************************************************************/
//  Description : 转换字符串到整数
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 若字符串为小数形式，则扩大成整数形式，并记录
//        下扩大的倍数
/*****************************************************************************/
LOCAL void  StrToInt32(
                        const MMI_STRING_T  *in_str_ptr, //in 
                        UC_STR_TO_NUM_T  *out_num  //out
                        );

/*****************************************************************************/
// 	Description : 去掉小数点后无效0
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void  DiscardStringInvalidZero(
  										MMI_STRING_T *in_str  //in & out
  										);  

/*****************************************************************************/
// 	Description : 对字符串4舍5入
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void  DiscardStringRedundance(
										  MMI_STRING_T *in_str,//in & out
										  uint32 num  // in,最多保留小数位数
										  );
 
/*****************************************************************************/
// 	Description : 根据扩大倍数移动小数点位置
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL  void  MoveDotByExpand(
								MMI_STRING_T *in_str,//in & out
								uint32 expand  //in
								);

/*****************************************************************************/
// 	Description : 转换整数结果到字符串。
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 如果整数经过放大了的，则加上小数点
/*****************************************************************************/
LOCAL void ConversionResultToString(
										UC_STR_TO_NUM_T * in_num, // in 
										MMI_STRING_T  *out_str_ptr  //out
										);

/*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL void  UnitConversion (
							UC_CONV_TYPE_E type,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							);

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 LOCAL void  UnitConversionByRate(
									 UC_STR_TO_NUM_T *in_num, //in 
									 const char *conv_rate, // in 
									 MMI_STRING_T  *out_str //out
									 );

/*****************************************************************************/
// 	Description : 根据转换类型设置文本显示标签文本
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void SetTextLabelText(
							  UC_CONV_TYPE_E type // in 
							  );

/*****************************************************************************/
//  Description : display conversion type
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayUcConversionType(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Discription:  handle change conversion type win
//  Global resource dependence: none 
//  Author:paul.huang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleChangeConversionTypeWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );
#endif

/*****************************************************************************/
// 	Description : 长度重量单位换算窗口消息处理函数
//	Global resource dependence :
//  Author:renyi.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUnitConvertWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*---------------------------------------------------------------------------*/
/*                          GLOBE DATA DECLARE                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                               */
/*---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
WINDOW_TABLE(UNITCONV_CHANGE_TYPE_WIN_TAB) = 
{ 
    WIN_FUNC((uint32) HandleChangeTypeWinMsg ),
    WIN_ID(MMIUNITCONV_CHANGE_TYPE_WIN_ID),
    WIN_TITLE(TXT_UC_SWITCH_UNIT),  
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIUNITCONV_CHANGE_TYPE_CTRL_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN    
 };

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(UNITCONV_MIAN_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleUnitConvertWinMsg),
    WIN_ID(MMIUNITCONV_MAIN_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_UC_UNIT_CONVERSION),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),    
        CHILD_OWNDRAW_CTRL(TRUE, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, (uint32)DisplayUcType),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE, MMIUC_INPUT_MAX_LEN, MMIUNITCONV_EDIT1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIUNITCONV_UNIT_LABEL1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID),
            CHILD_OWNDRAW_CTRL(FALSE, MMIUNITCONV_OWNDRAW_RESULT_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, (uint32)DisplayResultString),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),
            CHILD_EDIT_PICKER_CTRL(TRUE, MMIUNITCONV_EDIT_PICKER1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID),
            CHILD_EDIT_PICKER_CTRL(TRUE, MMIUNITCONV_EDIT_PICKER2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID),
            
    END_WIN    
};
#else
WINDOW_TABLE(UNITCONV_CHANGE_CONVERSION_TYPE_WIN_TAB) = 
{ 
    WIN_FUNC((uint32) HandleChangeConversionTypeWinMsg ),
    WIN_ID(MMIUNITCONV_CHANGE_CONVERSION_TYPE_WIN_ID),
    WIN_TITLE(TXT_UC_SWITCH_UNIT),  
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIUNITCONV_CHANGE_TYPE_CTRL_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN    
 };

WINDOW_TABLE(UNITCONV_MIAN_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleUnitConvertWinMsg),
    WIN_ID(MMIUNITCONV_MAIN_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_UC_UNIT_CONVERSION),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),    
        CHILD_OWNDRAW_CTRL(TRUE, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, (uint32)DisplayUcType),
        CHILD_OWNDRAW_CTRL(TRUE, MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, (uint32)DisplayUcConversionType),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIUNITCONV_UNIT_LABEL1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE, MMIUC_MAX_STR_BUFF_LEN, MMIUNITCONV_EDIT1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIUNITCONV_UNIT_LABEL2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE, MMIUC_MAX_STR_BUFF_LEN, MMIUNITCONV_EDIT2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN    
};
#endif

/**---------------------------------------------------------------------------*
 **                       FUNCTION BODY 
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN InputIsValid(
							MMI_STRING_T const *in_str_ptr //in
							)
{
	int32 loop=0;
	int32 dot_count=0;

	if(PNULL==in_str_ptr)
	{
		return FALSE;
	}

	if(0==in_str_ptr->wstr_len)
	{
		return FALSE;
	}

	//第一个字符为'0'，但第2个字符不是'.'
	if(in_str_ptr->wstr_len>1)
	{
		if(MMIUC_ZERO_CHAR==in_str_ptr->wstr_ptr[0] && MMIUC_DOT_CHAR!=in_str_ptr->wstr_ptr[1])
		{
			return FALSE;
		}
	}

	//第一个字符为'.'，或者最后一个字符为'.'
	if(MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[0] || MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[in_str_ptr->wstr_len-1])
	{
		return FALSE;
	}

	for(loop=0;loop<in_str_ptr->wstr_len;loop++)
	{
		//不是'0'~'9'或者'.'
		if( (in_str_ptr->wstr_ptr[loop]<MMIUC_ZERO_CHAR || in_str_ptr->wstr_ptr[loop]>MMIUC_NINE_CHAR)
			&&MMIUC_DOT_CHAR!=in_str_ptr->wstr_ptr[loop])
		{
			return FALSE;
		}

		if(MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[loop])
		{
			dot_count++;
		}
	}

	if(dot_count>1)//小数点个数大于1
	{
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN StrIsValid(
							MMI_STRING_T const *in_str_ptr //in
							)
{
	int32 loop=0;
	int32 dot_count=0;

	if(PNULL==in_str_ptr)
	{
		return TRUE;
	}

	if(0==in_str_ptr->wstr_len)
	{
		return TRUE;
	}

	//第一个字符为'0'，但第2个字符不是'.'
	if(in_str_ptr->wstr_len>1)
	{
		if(MMIUC_ZERO_CHAR==in_str_ptr->wstr_ptr[0] && MMIUC_DOT_CHAR!=in_str_ptr->wstr_ptr[1])
		{
			return FALSE;
		}
	}

	//第一个字符为'.'
	if(MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[0])
	{
		return FALSE;
	}

	for(loop=0;loop<in_str_ptr->wstr_len;loop++)
	{
		//不是'0'~'9'或者'.'
		if( (in_str_ptr->wstr_ptr[loop]<MMIUC_ZERO_CHAR || in_str_ptr->wstr_ptr[loop]>MMIUC_NINE_CHAR)
			&&MMIUC_DOT_CHAR!=in_str_ptr->wstr_ptr[loop])
		{
			return FALSE;
		}

		if(MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[loop])
		{
			dot_count++;
		}
	}

	if(dot_count>1)//小数点个数大于1
	{
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
//  Description : display uc type
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayUcType(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_RECT_T    rect = {0};
    GUI_RECT_T    display_rect = {0};
    GUI_LCD_DEV_INFO    dev_info = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;  
    MMI_STRING_T        type_string = {0};
    uint16 width = 0;
    uint16 height = 0;
    MMITHEME_FORM_T    form_theme = {0};
    MMI_HANDLE_T    win_handle = 0;
    
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID), &rect);

    if(s_type_pressed)
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENU_PRESSED,
                (const GUI_LCD_DEV_INFO*)&dev_info);
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENU_DEFAULT,
                (const GUI_LCD_DEV_INFO*)&dev_info);

    }

    //画边框
    if(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID == MMK_GetActiveCtrlId(owner_draw_ptr->win_handle))  
    {
        MMITHEME_GetFormTheme(&form_theme);
        GUI_DisplayBorder(rect, rect, &form_theme.active_child.border, &dev_info);   
    }

    switch(s_uc_type)
    {
        case UC_TYPE_WEIGHT:
            MMI_GetLabelTextByLang(TXT_UC_WEIGHT, &type_string);
            break;

        case UC_TYPE_LENGTH:
            MMI_GetLabelTextByLang(TXT_UC_LENGTH, &type_string);
            break;

        default:
            break;
    }

//@leon.wang modify for QQVGA and QCIF
#ifdef MMI_UNITCONV_MINI_SUPPORT
    text_style.align = ALIGN_LVMIDDLE;
    display_rect = rect;
    display_rect.left += MMIUC_OWNDRAW_TITLE_MARGIN;
#else
    text_style.align = ALIGN_HVMIDDLE;
    display_rect = rect;
#endif
    text_style.font = MMIUC_FONT_SIZE;
    text_style.font_color = MMI_WHITE_COLOR;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info,
        (const GUI_RECT_T      *)&display_rect,       //the fixed display area
        (const GUI_RECT_T      *)&display_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&type_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_CONVERTER_MENUICON_DEFAULT, owner_draw_ptr->win_handle);
    rect.top += (rect.bottom - rect.top + 1 -height)/2;
#ifdef MMI_UNITCONV_MINI_SUPPORT
    rect.left = rect.right - width;
#else
    rect.left = rect.right - width - 7;
#endif

    if(s_type_pressed)
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENUICON_PRESSED,
                (const GUI_LCD_DEV_INFO*)&dev_info);   
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENUICON_DEFAULT,
                (const GUI_LCD_DEV_INFO*)&dev_info);    
    }
    //modify for NEWM00226567
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    GUIWIN_SetSoftkeyTextId(owner_draw_ptr->win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
    GUIWIN_SeSoftkeytButtonIconId(owner_draw_ptr->win_handle, IMAGE_NULL, 1, TRUE);
#else
    GUIWIN_SetSoftkeyTextId(owner_draw_ptr->win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
#endif
    win_handle = MMK_GetFocusWinHandle();
    if(MMK_IsWinIncludeTitle(win_handle))
    {
        GUIWIN_UpdateTitle(win_handle);
    }
}

/*****************************************************************************/
//  Description : handle owndraw tp down
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpDown(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                )
{
    if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID)) == ctrl_handle)
    {
        s_type_pressed = TRUE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID)) == ctrl_handle)
    {
        s_conversion_type_pressed = TRUE;
    }
#endif

    MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle owndraw tp up
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpUp(
                              MMI_HANDLE_T ctrl_handle,
                              GUI_POINT_T tp_point
                              )
{
    GUI_RECT_T    rect = {0};

    if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID)) == ctrl_handle)
    {
        s_type_pressed = FALSE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID)) == ctrl_handle)
    {
        s_conversion_type_pressed = FALSE;
    }   
#endif

    MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

    IGUICTRL_GetRect(MMK_GetCtrlPtr(ctrl_handle), &rect);
    
    if (GUI_PointIsInRect(tp_point, rect))
    {
        if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID)) == ctrl_handle)
        {
            MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_TYPE_WIN_TAB, PNULL);
        }
#ifndef MMI_PDA_SUPPORT
        else if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID)) == ctrl_handle)
        {
            MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_CONVERSION_TYPE_WIN_TAB, PNULL);
        }    
#endif
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle owndraw tp move
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpMove(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                )
{
    GUI_RECT_T    rect = {0};
    
    IGUICTRL_GetRect(MMK_GetCtrlPtr(ctrl_handle), &rect);
    
    if (!GUI_PointIsInRect(tp_point, rect))
    {
        if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID)) == ctrl_handle)
        {
            s_type_pressed = FALSE;
        }
#ifndef MMI_PDA_SUPPORT
        else if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID)) == ctrl_handle)
        {
            s_conversion_type_pressed = FALSE;
        }   
#endif
        
        MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Discription:  handle change type win
//  Global resource dependence: none 
//  Author:paul.huang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleChangeTypeWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id  = MMIUNITCONV_CHANGE_TYPE_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:        
            GUILIST_SetMaxItem(ctrl_id,UC_TYPE_MAX, FALSE );
            MMIAPISET_AppendListItemByTextId( TXT_UC_WEIGHT, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            MMIAPISET_AppendListItemByTextId( TXT_UC_LENGTH, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            GUILIST_SetSelectedItem(ctrl_id, s_uc_type, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id,s_uc_type);        
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            {
                uint16  cur_select_item = 0;
                cur_select_item = GUILIST_GetCurItemIndex(ctrl_id);
                switch (cur_select_item)
                {
                    case UC_TYPE_WEIGHT:
                        s_uc_type = UC_TYPE_WEIGHT;
                        break;
                    
                case UC_TYPE_LENGTH:
                    s_uc_type = UC_TYPE_LENGTH;
                    break;
                    
                    default:
                        break;
                }            
                MMK_CloseWin(win_id);
            }
            break;
        
        case MSG_GET_FOCUS: 
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

#ifdef MMI_PDA_SUPPORT
 /*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  UnitConversion (
							uint16  selected1,//in
							uint16  selectde2,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							)
{
    BOOLEAN result = FALSE;
    SCI_ASSERT(in_str_ptr!=PNULL&&out_str_ptr!=PNULL);/*assert verified*/
    SCI_ASSERT(out_str_ptr->wstr_ptr!=PNULL);/*assert verified*/

    switch(s_uc_type)
    {
        case UC_TYPE_WEIGHT:
            result = UnitConversionByRate(in_str_ptr,s_weight_conversion_rate[selected1][selectde2],out_str_ptr);  
            break;

        case UC_TYPE_LENGTH:
            result = UnitConversionByRate(in_str_ptr,s_length_conversion_rate[selected1][selectde2],out_str_ptr);  
            break;

        default:
            break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 LOCAL BOOLEAN  UnitConversionByRate(
									 const MMI_STRING_T  *in_str, //in 
									 const char *conv_rate, // in 
									 MMI_STRING_T  *out_str //out
									 )
{
    double  dResult = 0;
    double    dOp1 = 0;
    double    dOp2 = 0;
    char * strStop = PNULL;
    char in_buf[MMIUC_INPUT_MAX_LEN + 1]={0};
    char out_buf[MMIUC_OUTPUT_MAX_LEN + 1]={0};
    char buf[MMIUC_OUTPUT_MAX_LEN*2 + 1]={0};

    SCI_ASSERT(in_str!=PNULL && conv_rate!=PNULL && out_str!=PNULL);/*assert verified*/
    SCI_ASSERT(out_str->wstr_ptr!=PNULL);/*assert verified*/

    MMIAPICOM_WstrToStr(in_str->wstr_ptr, (uint8*)&in_buf);

    dOp1 = strtod((char*)conv_rate, (char**)&strStop);
    dOp2 = strtod((char*)in_buf, (char**)&strStop);
    if(dOp1<1.0e-100&&dOp1>-1.0e-100)
    {
        dOp1 = 0;
    }
    if(dOp2<1.0e-100 && dOp2>-1.0e-100)
    {
        dOp2 = 0;
    }

    dResult = dOp1*dOp2;
    if(dResult<1.0e-100 && dResult>-1.0e-100)//add for CR177013
    {
        dResult = 0;   
    }
    
    if (dResult > 9.9e100||dResult < -(9.9e100))/*错误处理*/
    {
        return FALSE;					
    }
    else
    {
        if( dResult <= -1e13 || dResult >= 1e13 )
        {    
            sprintf((char*)out_buf, "%e", dResult);      
        }
        else if( dResult <= 1e-11 && dResult >= -1e-11 )     //去0和小数点的位置
        {
            if(dResult<1.0e-100 && dResult>-1.0e-100)   //0
            {
                out_buf[0] = '0';
            }
            else
            {
                sprintf((char*)out_buf, "%e", dResult);  
            }
            
        }
        else
        {     
            SCI_MEMSET(buf, 0, MMIUC_OUTPUT_MAX_LEN*2 + 1);       
            // 尽可能长的打印，去除小数点的位置
            sprintf ( (char*)buf, "%-.*f", (MMIUC_OUTPUT_MAX_LEN - 2), dResult);    
            if( buf[0] == '-')
            {
                SCI_MEMCPY((char*)out_buf, (char*)buf, MMIUC_OUTPUT_MAX_LEN + 1) ; 
            }
            else
            {
                SCI_MEMCPY((char*)out_buf, (char*)buf, MMIUC_OUTPUT_MAX_LEN) ;     
            }   
            //去除result尾部的0和‘.’
            {
                int32 i=0;
                int32 j=0;
                // 寻找‘.’
                for(i = MMIUC_OUTPUT_MAX_LEN; i >= 0 ; i--)
                {
                    if(out_buf[i] == '.')
                    {   
                        break;
                    }          
                }
                
                //找到‘.’,处理尾部的‘0’
                if(i != -1)
                {
                    for(j = MMIUC_OUTPUT_MAX_LEN; j > i ; j--)
                    {
                        if(out_buf[j] == '0'|| out_buf[j] == 0)
                        {                    
                            out_buf[j] = 0;                           
                        }       
                        else
                        {
                            break;
                        }
                    }
                }
                
                //判断是否处理‘.’
                if(i == j)
                {
                    out_buf[i] = 0;
                }
                
            }
        }   
    }

    MMIAPICOM_StrToWstr((const uint8*)out_buf, out_str->wstr_ptr);
    out_str->wstr_len=MMIAPICOM_Wstrlen(out_str->wstr_ptr);

    return TRUE;
}

LOCAL void  InitPickerContent(MMI_STRING_T* weight_string, MMI_STRING_T* height_string)
{
    uint8 i = 0;

    for(i=0; i<UC_WEIGHT_MAX; i++)
    {
        MMI_GetLabelTextByLang(s_weight_text_id[i], &weight_string[i]);
    }

    for(i=0; i<UC_LENGTH_MAX; i++)
    {
        MMI_GetLabelTextByLang(s_length_text_id[i], &height_string[i]);
    }

    return;
}

/*****************************************************************************/
//  Description : display result
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayResultString(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_RECT_T    rect = {0};
    GUI_LCD_DEV_INFO    dev_info = {0};
    MMITHEME_FORM_T    form_theme = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;  
    MMI_STRING_T        result_string = {0};
    uint16 string_len = 0;
    
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_RESULT_CTRL_ID), &rect);
    //边框与EDIT控件一致
    MMITHEME_GetFormTheme(&form_theme);
    GUI_FillRect(&dev_info,rect,MMI_WINDOW_BACKGROUND_COLOR);
    GUI_DisplayBorder(rect, rect, &form_theme.child.border, &dev_info);   
    //与EDIT控件坐标一致
    rect.left += MMIUC_EDIT_MARGIN + form_theme.child.border.width;
    rect.right -= MMIUC_EDIT_MARGIN + form_theme.child.border.width;
    rect.top += MMIUC_EDIT_MARGIN + form_theme.child.border.width;
    rect.bottom -= MMIUC_EDIT_MARGIN + form_theme.child.border.width;

    string_len = MMIAPICOM_Wstrlen(s_result_buffer);

    if(string_len)
    {
        text_style.align = ALIGN_LVMIDDLE;
        text_style.font = MMIUC_FONT_SIZE;
        text_style.font_color = MMI_BLACK_COLOR;

        result_string.wstr_ptr = s_result_buffer;
        result_string.wstr_len = string_len;

        while(GUI_GetStringWidth(text_style.font, result_string.wstr_ptr,result_string.wstr_len) > (rect.right - rect.left))
        {           
            text_style.font-= 1;   
        } 

        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&dev_info,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&result_string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );    
    }
}
/*****************************************************************************/
//  Description : Display H Line
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayLine(void)
{
    GUI_RECT_T    picker1_rect = {0};
    GUI_RECT_T    picker2_rect = {0};
    GUI_RECT_T    edit_rect = {0};
    GUI_LCD_DEV_INFO    dev_info = {0};
    uint16 x1=0;
    uint16 x2=0;
    uint16 y1=0;
    uint16 y2=0;
    //GUIFORM_ForceLayout(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID);
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_EDIT_PICKER1_CTRL_ID), &picker1_rect);
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_EDIT_PICKER2_CTRL_ID), &picker2_rect);
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_EDIT1_CTRL_ID), &edit_rect);
    x1=picker1_rect.left;
    x2=picker2_rect.right;
    y1=edit_rect.bottom+(picker1_rect.top-edit_rect.bottom)/2;
    LCD_DrawHLine(&dev_info,x1,y1,x2,MMI_DARK_GRAY_COLOR);
    x1=picker1_rect.right+1;
    y2=picker1_rect.bottom;
    LCD_DrawVLine(&dev_info,x1,y1,y2,MMI_DARK_GRAY_COLOR);
    
}

/*****************************************************************************/
// 	Description : 长度重量单位换算窗口消息处理函数
//	Global resource dependence :
//  Author:renyi.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUnitConvertWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    wchar  dot_str[] = {'.'};
    static wchar last_valid_input_str[MMIUC_INPUT_MAX_LEN + 1];    
    MMI_STRING_T  text_label  = {0};
    char     str[2] = {0};
    wchar    wstr[2] = {0};
    uint16   wstr_length = 0;
    MMI_STRING_T  weight_string[UC_WEIGHT_MAX] = {0};
    MMI_STRING_T  height_string[UC_LENGTH_MAX] = {0};
    static UC_TYPE_E current_type = UC_TYPE_MAX;     
    GUIFORM_CHILD_WIDTH_T width = {(45), GUIFORM_CHILD_WIDTH_PERCENT};   
    GUIFORM_CHILD_HEIGHT_T height = {0};
    uint16 picker_height = 0;
    uint16 picker_item_height = 0;
    uint16 v_space = MMIUC_FORM_VSAPCE;
    uint16 v_space_h = MMIUC_FORM_HSAPCE;
    uint16 margin = MMIUC_FORM_MARGIN;
    GUI_BOTH_RECT_T client_rect = {0};
    GUI_FONT_ALL_T active_font = {0};
    GUI_FONT_ALL_T normal_font = {0};
    GUIOWNDRAW_TP_HANDLER_T     tp_handler = {0};
    uint16 picker_space_h=1;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            wchar           forbid_char_value[]= {0x0d, 0x0a};
            SCI_MEMSET(last_valid_input_str, 0, (MMIUC_INPUT_MAX_LEN + 1)*sizeof(wchar));
            GUIFORM_IsSlide(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, FALSE);
            GUIFORM_SetMargin(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID,margin);  
            if(MMITHEME_IsMainScreenLandscape())
            {
                GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space_h);
            }
            else
            {
                GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space);                
            }            
            GUIFORM_PermitChildBg(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, FALSE);
            
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_OWNDRAW_HEIGHT;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, &height);
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_EDIT_FORM_HEIGHT;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, &height);
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
#ifndef MAINLCD_DEV_SIZE_160X128 
            height.add_data = MMIUC_EDIT_FORM_HEIGHT - 4;
#else
            height.add_data = MMIUC_EDIT_FORM_HEIGHT - 3;
#endif
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_EDIT1_CTRL_ID, &height);
            client_rect = MMITHEME_GetWinClientBothRect(win_id);
            if(MMITHEME_IsMainScreenLandscape())
            {
                picker_height = client_rect.h_rect.bottom - client_rect.h_rect.top +1 - 2*v_space_h - 2*margin;
                picker_item_height = MMIUC_PICKER_HEIGHT_H;
            }
            else
            {
                picker_height = client_rect.v_rect.bottom - client_rect.v_rect.top +1 - 2*v_space - 2*margin;
                picker_item_height = MMIUC_PICKER_HEIGHT_V;
            }
            picker_height -= MMIUC_OWNDRAW_HEIGHT + MMIUC_EDIT_FORM_HEIGHT;
            GUIEDIT_SetPickerHeight(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, picker_height);
            GUIEDIT_SetPickerHeight(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, picker_height);

            tp_handler.tp_down  = HandleOwnDrawTpDown;
            tp_handler.tp_move = HandleOwnDrawTpMove;
            tp_handler.tp_up = HandleOwnDrawTpUp;

            GUIOWNDRAW_SetCallback(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, &tp_handler);   
        
            sprintf(str, "=");
            wstr_length = SCI_STRLEN(str);
            MMIAPICOM_StrToWstr((uint8*)str, wstr);
            text_label.wstr_ptr = wstr;
            text_label.wstr_len = wstr_length;
            GUILABEL_SetText(MMIUNITCONV_UNIT_LABEL1_CTRL_ID, &text_label, FALSE);        
            GUILABEL_SetFont(MMIUNITCONV_UNIT_LABEL1_CTRL_ID, MMIUC_FONT_SIZE, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
            //set edit style
            GUIEDIT_SetStyle(MMIUNITCONV_EDIT1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetAlign(MMIUNITCONV_EDIT1_CTRL_ID, ALIGN_LVMIDDLE);
			
            GUIEDIT_SetIm(MMIUNITCONV_EDIT1_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_ConfigDisableChar(MMIUNITCONV_EDIT1_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            GUIEDIT_ConfigImCustomKey(MMIUNITCONV_EDIT1_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            GUIEDIT_SetImTag(MMIUNITCONV_EDIT1_CTRL_ID, GUIIM_TAG_DOT_NUM);
			
            GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT1_CTRL_ID,MMIUC_INPUT_MAX_LEN);
            GUIEDIT_SetFont(MMIUNITCONV_EDIT1_CTRL_ID, MMIUC_FONT_SIZE);
            GUIEDIT_SetMargin(MMIUNITCONV_EDIT1_CTRL_ID, MMIUC_EDIT_MARGIN, MMIUC_EDIT_MARGIN);

            GUIFORM_SetChildWidth(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_EDIT1_CTRL_ID, &width);
            GUIFORM_SetChildWidth(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_OWNDRAW_RESULT_CTRL_ID, &width);

            current_type = UC_TYPE_WEIGHT;
            s_uc_type = UC_TYPE_WEIGHT;
            s_picker1_selected = 0;
            s_picker2_selected = 0;
            
            InitPickerContent(weight_string, height_string);
            
            GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, weight_string, UC_WEIGHT_MAX);
            GUIEDIT_SetPickerLoop(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, TRUE);
            GUIEDIT_SetPickerLineHeight(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, picker_item_height);            
            GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, s_picker1_selected);  

            GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, weight_string, UC_WEIGHT_MAX);
            GUIEDIT_SetPickerLoop(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, TRUE);
            GUIEDIT_SetPickerLineHeight(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, picker_item_height);            
            GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, s_picker2_selected);  

            normal_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_NORMAL);
            normal_font.font = MMIUC_FONT_SIZE;            
            active_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_CUR);
            active_font.font = MMIUC_FONT_SIZE;
            GUIEDIT_SetPickerFont(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, &normal_font, &active_font);
            GUIEDIT_SetPickerFont(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, &normal_font, &active_font);

            GUIEDIT_SetClipboardEnabled(MMIUNITCONV_EDIT1_CTRL_ID,FALSE);

            GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID,&picker_space_h,PNULL);
            
            MMK_SetAtvCtrl(win_id, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID);

            s_type_pressed = FALSE;
            SCI_MEMSET(s_result_buffer, 0, sizeof(s_result_buffer));
        }
        break;

   case MSG_LCD_SWITCH:
       client_rect = MMITHEME_GetWinClientBothRect(win_id);
       if(MMITHEME_IsMainScreenLandscape())
       {
           picker_height = client_rect.h_rect.bottom - client_rect.h_rect.top +1 - 2*v_space_h - 2*margin;
           picker_item_height = MMIUC_PICKER_HEIGHT_H;
       }
       else
       {
           picker_height = client_rect.v_rect.bottom - client_rect.v_rect.top +1 - 2*v_space - 2*margin;
           picker_item_height = MMIUC_PICKER_HEIGHT_V;
       }
       picker_height -= MMIUC_OWNDRAW_HEIGHT + MMIUC_EDIT_FORM_HEIGHT;
       GUIEDIT_SetPickerHeight(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, picker_height);
       GUIEDIT_SetPickerHeight(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, picker_height);       
       GUIEDIT_SetPickerLineHeight(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, picker_item_height);           
       GUIEDIT_SetPickerLineHeight(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, picker_item_height);            

       if(MMITHEME_IsMainScreenLandscape())
       {
           GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space_h);
       }
       else
       {
           GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space);                
       }
       break;

   case MSG_LOSE_FOCUS:
        s_type_pressed = FALSE;
        break;
        
   case MSG_GET_FOCUS:
        if(current_type != s_uc_type)
        {
            wchar  out_buffer[1] = {0}; 
            
            current_type = s_uc_type;            
            InitPickerContent(weight_string, height_string);
            s_picker1_selected = 0;
            s_picker2_selected = 0;

            switch (s_uc_type)
            {
                case UC_TYPE_WEIGHT:
                    GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, weight_string, UC_WEIGHT_MAX);
                    GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, s_picker1_selected);  
                    GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, weight_string, UC_WEIGHT_MAX);
                    GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, s_picker2_selected);  
                    break;

                case UC_TYPE_LENGTH:
                    GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, height_string, UC_LENGTH_MAX);
                    GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, s_picker1_selected);  
                    GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, height_string, UC_LENGTH_MAX);
                    GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, s_picker2_selected);  
                    break;

                default:
                    break;
            }              
            GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_buffer, 1);
            SCI_MEMSET(s_result_buffer, 0, sizeof(s_result_buffer));
        }
        break;

    case MSG_NOTIFY_EDITBOX_PICKER_SELECTED:
        {
            MMI_STRING_T in_str_ptr = {0};              
            MMI_STRING_T out_str_ptr = {0};                
            wchar        out_buffer[MMIUC_OUTPUT_MAX_LEN + 1] = {0};  
            MMI_STRING_T        text = {0};
            
            out_str_ptr.wstr_ptr = out_buffer;
            if(MMIUNITCONV_EDIT_PICKER1_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                s_picker1_selected = GUIEDIT_GetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER1_CTRL_ID);
            } 
            else if(MMIUNITCONV_EDIT_PICKER2_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                s_picker2_selected = GUIEDIT_GetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER2_CTRL_ID);
            }
            GUIEDIT_GetString(MMIUNITCONV_EDIT1_CTRL_ID, &in_str_ptr);
            if(0 < in_str_ptr.wstr_len)
            {
                if(UnitConversion(s_picker1_selected, s_picker2_selected, &in_str_ptr, &out_str_ptr))        
                {
                    SCI_MEMSET(s_result_buffer, 0, sizeof(s_result_buffer));
                    MMIAPICOM_Wstrncpy(s_result_buffer, out_buffer, sizeof(s_result_buffer));
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_ERROR, &text);
                    MMIAPICOM_Wstrncpy(s_result_buffer, text.wstr_ptr, sizeof(s_result_buffer));
                }
                MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
            }
        }
        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T in_str_ptr = {0};              
            MMI_STRING_T out_str_ptr = {0};        
            wchar        out_buffer[MMIUC_OUTPUT_MAX_LEN + 1] = {0};
            wchar        in_buffer[2] = {0}; 
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            static BOOLEAN is_recovery_last_input = FALSE;            
            MMI_STRING_T        text = {0};
            
            if(PNULL == param)
            {
                break;
            }
            if(((MMI_NOTIFY_T*)param)->src_id != ctrl_id)
            {
                break;
            }
            //if  recovering last input,do nothing
            if(is_recovery_last_input)    
            {
                is_recovery_last_input = FALSE;
                break;
            }      
            
            out_str_ptr.wstr_ptr = out_buffer;
            GUIEDIT_GetString(ctrl_id, &in_str_ptr);

            if(!StrIsValid(&in_str_ptr))
            {
                uint16 length = MMIAPICOM_Wstrlen(last_valid_input_str);
	            if((in_str_ptr.wstr_len == 2)
                 &&(MMIUC_ZERO_CHAR==in_str_ptr.wstr_ptr[0])
                 &&(in_str_ptr.wstr_ptr[1]>=MMIUC_ZERO_CHAR && in_str_ptr.wstr_ptr[1]<=MMIUC_NINE_CHAR))
	            {
		            
                    MMIAPICOM_Wstrncpy(in_buffer,in_str_ptr.wstr_ptr,in_str_ptr.wstr_len);
                    in_buffer[0] = in_str_ptr.wstr_ptr[1];
                    in_buffer[1] = 0;
                    GUIEDIT_SetString( ctrl_id, in_buffer, 1); 
                    GUIEDIT_GetString(ctrl_id, &in_str_ptr);
	            }
                else if((in_str_ptr.wstr_len == 1)
                 &&(MMIUC_DOT_CHAR==in_str_ptr.wstr_ptr[0])
                 )
	            {
                    in_buffer[0] = '0';
                    in_buffer[1] = in_str_ptr.wstr_ptr[0];
                    GUIEDIT_SetString( ctrl_id, in_buffer, 2); 
                    GUIEDIT_GetString(ctrl_id, &in_str_ptr);
	            }
                else if(length != 0)
                {
                    in_str_ptr.wstr_ptr = last_valid_input_str;
                    in_str_ptr.wstr_len = length;
                    GUIEDIT_SetString( ctrl_id, in_str_ptr.wstr_ptr, in_str_ptr.wstr_len);  
                    is_recovery_last_input = TRUE;
                }
                else if(in_str_ptr.wstr_len!=0)
                {
                    GUIEDIT_ClearAllStr(ctrl_id);
                    is_recovery_last_input = TRUE;
                }
            }
            if (InputIsValid(&in_str_ptr))
            {     
                if(!UnitConversion(s_picker1_selected, s_picker2_selected, &in_str_ptr, &out_str_ptr))        
                {
                    MMI_GetLabelTextByLang(TXT_ERROR, &text);
                    MMIAPICOM_Wstrncpy(out_buffer, text.wstr_ptr, sizeof(out_buffer));
                }
            }
            else    //invalid input, clear result
            {
                out_str_ptr.wstr_ptr[0] = 0;
                out_str_ptr.wstr_len = 1;
            }
            
            SCI_MEMSET(last_valid_input_str, 0, (MMIUC_INPUT_MAX_LEN + 1)*sizeof(wchar));
            GUIEDIT_GetString(ctrl_id, &in_str_ptr);
            if(in_str_ptr.wstr_ptr!=PNULL && in_str_ptr.wstr_len>0)
            {
                MMIAPICOM_Wstrncpy(last_valid_input_str, in_str_ptr.wstr_ptr,
                    in_str_ptr.wstr_len <= MMIUC_INPUT_MAX_LEN ? in_str_ptr.wstr_len : MMIUC_INPUT_MAX_LEN);
            }
                
            if(MMIUNITCONV_EDIT1_CTRL_ID == ctrl_id)
            {
                SCI_MEMSET(s_result_buffer, 0, sizeof(s_result_buffer));
                MMIAPICOM_Wstrncpy(s_result_buffer, out_buffer, sizeof(s_result_buffer));
            }
            else
            {}
            MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
        }
        break;

    case MSG_END_FULL_PAINT:
        DisplayLine();
        break;

    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            
            if(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID == ctrl_id)
            {
                MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_TYPE_WIN_TAB, PNULL);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

#else
 /*****************************************************************************/
 //  Description : 转换字符串到整数
 //  Global resource dependence : 
 //  Author:renyi.hu
 //  Note: 若字符串为小数形式，则扩大成整数形式，并记录
 //        下扩大的倍数
 /*****************************************************************************/
 LOCAL void  StrToInt32(
                         const MMI_STRING_T  *in_str_ptr, //in 
                         UC_STR_TO_NUM_T  *out_num  //out
                         )
 {
     wchar dot=MMIUC_DOT_CHAR;
     int16 dot_position=-1;
     int32 loop=0;
     wchar buff[MMIUC_MAX_STR_BUFF_LEN + 1]={0};
     MMI_STRING_T  str_ptr={0};
     str_ptr.wstr_ptr=buff;
 
     SCI_ASSERT(in_str_ptr!=PNULL&&out_num!=PNULL);/*assert verified*/  /*lint !e831*/
     if(!InputIsValid(in_str_ptr))
     {
         return ;
     }
 
     MMIAPICOM_CopyStr2Buf(buff,in_str_ptr,MMIUC_MAX_STR_BUFF_LEN);
     str_ptr.wstr_len=in_str_ptr->wstr_len;  /*lint !e613*/
 
     //找出小数点位置
     for(loop=0;loop<str_ptr.wstr_len;loop++)
     {
         if(str_ptr.wstr_ptr[loop]==dot)
         {
             dot_position=loop;
             break;
         }
     }
     
     if(dot_position>0)//有小数点则去掉，并记下去掉小数点后数值扩大的倍数
     {
         for(loop=dot_position+1;loop<str_ptr.wstr_len;loop++)
         {
             str_ptr.wstr_ptr[loop-1]=str_ptr.wstr_ptr[loop];
         }
         str_ptr.wstr_len-=1;
         str_ptr.wstr_ptr[str_ptr.wstr_len]=0;
         MMIAPICOM_WstrToNum(str_ptr.wstr_ptr,str_ptr.wstr_len,&out_num->num);  /*lint !e613*/
         
         for(loop=0;loop<str_ptr.wstr_len-dot_position;loop++)  
         {
             out_num->expand=out_num->expand*MMIUC_DECIMAL_NUM;  /*lint !e613*/
         }
     }
     else
     {//没有小数点则直接转成整数。扩大倍数则为1.
         MMIAPICOM_WstrToNum(str_ptr.wstr_ptr,str_ptr.wstr_len,&out_num->num);   /*lint !e613*/
         out_num->expand=1;   /*lint !e613*/
     }
 
     //SCI_TRACE_LOW:"StrToInt32:num=[%d],expand=[%d]"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1477_112_2_18_2_2_24_103,(uint8*)"dd",out_num->num,out_num->expand);   
 }

 /*****************************************************************************/
// 	Description : 去掉小数点后无效0
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void  DiscardStringInvalidZero(
  										MMI_STRING_T *in_str  //in & out
  										)
 {
 	int32 loop=0;
	
	if(PNULL==in_str)
	{
		return ;
	}

	//找到小数点位置
	for(loop=0;loop<in_str->wstr_len;loop++)
	{
		if(MMIUC_DOT_CHAR==in_str->wstr_ptr[loop])
		{
			break;
		}
	}

	//没有找到小数点
	if(loop==in_str->wstr_len)
	{
		return ;
	}

	//去掉最后面无效的'0'
	loop=in_str->wstr_len-1;
	while(in_str->wstr_ptr[loop]==MMIUC_ZERO_CHAR)
	{
		in_str->wstr_ptr[loop]=0;
		in_str->wstr_len--;
		loop--;
	}
	
	//如果最后的字符是'.'，则去掉。
	if(MMIUC_DOT_CHAR==in_str->wstr_ptr[loop])
	{
		in_str->wstr_ptr[loop]=0;
		in_str->wstr_len--;
	}
 }
  
/*****************************************************************************/
// 	Description : 对字符串4舍5入
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void  DiscardStringRedundance(
										  MMI_STRING_T *in_str,//in & out
										  uint32 num  // in,最多保留小数位数
										  )
  {
  	int32 loop=0;
	uint16 dot_position=0;
	wchar validChar=0;

	if(PNULL==in_str)
	{
		return ;
	}

	//找到小数点位置
	for(loop=0;loop<in_str->wstr_len;loop++)
	{
		if(MMIUC_DOT_CHAR==in_str->wstr_ptr[loop])
		{
			break;
		}
	}
	dot_position=loop;
	
	if(dot_position==in_str->wstr_len)//没有找到小数点,不用取舍
	{
		return ;
	}

	if(in_str->wstr_len-dot_position-1<=num)/*lint !e574*///小数位数小于等于num,不用取舍
	{
		return ;
	}

	validChar=in_str->wstr_ptr[dot_position+num+1];//validChar大小决定是否进位
	//SCI_TRACE_LOW:"UC:pre_validChar=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1567_112_2_18_2_2_24_104,(uint8*)"d",in_str->wstr_ptr[dot_position+num]);
	//SCI_TRACE_LOW:"UC:validChar=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1568_112_2_18_2_2_24_105,(uint8*)"d",validChar);
	
	//该条件下直接把后面的丢弃
	if(validChar<MMIUC_FIVE_CHAR&&validChar>=MMIUC_ZERO_CHAR)
	{
		in_str->wstr_ptr[dot_position+num+1]=0;
		in_str->wstr_len=dot_position+1+num;
	}
	//该条件下需要进位
	else if(validChar<=MMIUC_NINE_CHAR&&validChar>=MMIUC_FIVE_CHAR)
	{
		in_str->wstr_ptr[dot_position+num+1]=0;
		in_str->wstr_len=dot_position+1+num;
		
		for(loop=in_str->wstr_len-1;loop>=0;loop--)
		{
			if(MMIUC_DOT_CHAR != in_str->wstr_ptr[loop])
			{
				in_str->wstr_ptr[loop]+=1;
			}
			else
			{
				continue;
			}
			
			if(in_str->wstr_ptr[loop]>MMIUC_NINE_CHAR)//进为后大于9，则需减去10
			{
				in_str->wstr_ptr[loop]-=MMIUC_DECIMAL_NUM;
			}
			else
			{//小于或等于9无需向前进位了，退出循环
				break;
			}
		}
	}
	
	//最高位进位了
	if(-1==loop)
	{
		for(loop=in_str->wstr_len-1;loop>=0;loop--)
		{
			in_str->wstr_ptr[loop+1]=in_str->wstr_ptr[loop];
		}
		in_str->wstr_ptr[0]='1';
		in_str->wstr_len++;
	}

	//SCI_TRACE_LOW:"UC:lastchar=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1615_112_2_18_2_2_24_106,(uint8*)"d",in_str->wstr_ptr[in_str->wstr_len-1]);
  }

/*****************************************************************************/
// 	Description : 根据扩大倍数移动小数点位置
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL  void  MoveDotByExpand(
								MMI_STRING_T *in_str,//in & out
								uint32 expand  //in
								)
{
	int32 dot_position=0;
	int32 count=0;
	int32 loop=0;

	if(PNULL==in_str)
	{
		return ;
	}

	//找出需要移动的位数
	while(expand>1)
	{
		expand=expand/MMIUC_DECIMAL_NUM;
		count++;
	}
	
	if(0==count)
	{
		return ;
	}

	//找出小数点位置
	for(dot_position=0;dot_position<in_str->wstr_len;dot_position++)
	{
		if(MMIUC_DOT_CHAR==in_str->wstr_ptr[dot_position])
		{
			break;
		}
	}

	//没有找到小数点则在最后添加一个。
	if(dot_position==in_str->wstr_len)
	{
		in_str->wstr_ptr[dot_position]=MMIUC_DOT_CHAR;
		in_str->wstr_len+=1;
	}
	
	
	while(count>0)
	{
		if(dot_position>1)//该条件下无需在前面补0
		{
			in_str->wstr_ptr[dot_position]=in_str->wstr_ptr[dot_position-1];
			dot_position--;
			count--;
		}
		else
		{
			break;
		}
	}
	
	if(0==count)
	{
		in_str->wstr_ptr[dot_position]=MMIUC_DOT_CHAR;
	}
	else
	{//需要在前面补0
		in_str->wstr_ptr[dot_position]=in_str->wstr_ptr[dot_position-1];
		
		for(loop=in_str->wstr_len;loop>0;loop--)
		{
			in_str->wstr_ptr[loop-1+count]=in_str->wstr_ptr[loop-1];
		}
		for(loop=0;loop<count+1;loop++)
		{
			in_str->wstr_ptr[loop]=MMIUC_ZERO_CHAR;
		}
		in_str->wstr_ptr[1]=MMIUC_DOT_CHAR;
		in_str->wstr_len+=count;
	}
}

/*****************************************************************************/
// 	Description : 转换整数结果到字符串。
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 如果整数经过放大了的，则加上小数点
/*****************************************************************************/
LOCAL void ConversionResultToString(
                                        UC_STR_TO_NUM_T * in_num, // in
                                        MMI_STRING_T  *out_str_ptr  //out
                                        )
{
    uint8 str[MMIUC_MAX_STR_BUFF_LEN + 1]={0};
    uint8 str_len=0;
    int32 loop =0;
    int32 diff=0;
    int32 precision=0;
    uint32 count=1;
    uint32 decimal_count=0;
    uint32 dot_position=0;
    uint32 num=0;
    uint32 expand=1;

    //SCI_ASSERT(in_num!=PNULL && out_str_ptr!=PNULL);/*assert verified*/
    //SCI_ASSERT(out_str_ptr->wstr_ptr!=PNULL);/*assert verified*/
    if(in_num == PNULL || out_str_ptr == PNULL || out_str_ptr->wstr_ptr == PNULL)
    {
        return;
    }

    num=in_num->num;
    expand=in_num->expand;

    //计算出小数点后位数
    while(expand>1)
    {
        expand=expand/MMIUC_DECIMAL_NUM;
        precision++;
    }

    //该条件下这里先进行 4舍5入，
    //将整数后面的无效位变为0
    if(precision>MMIUC_PRECISION_NUM)
    {
        for(loop=0;loop<precision-MMIUC_PRECISION_NUM;loop++)
        {
            count=count*MMIUC_DECIMAL_NUM;
        }

        if( (num%count) >=count/2)
        {
            num=num-(num%count)+count;//进位后的数，无效位全是0了。
        }
        else
        {
            num=num-(num%count);
        }
    }

    MMIAPICOM_Int2Str(num,str,MMIUC_MAX_STR_BUFF_LEN);

    str_len=SCI_STRLEN((char*)str);
    diff=str_len-precision;

    if(diff>0)
    {//字符串长度大于小数点位数，直接在相应位置添加'.'
        for(loop=str_len;loop>str_len-precision;loop--)
        {
            str[loop]=str[loop-1];
        }
        str[loop]=MMIUC_DOT_CHAR;
        str_len=SCI_STRLEN((char*)str);
        out_str_ptr->wstr_len=str_len;
        for(loop=0;loop<str_len;loop++)
        {
            out_str_ptr->wstr_ptr[loop]=str[loop];
        }
    }
    else
    {//前面需要补0了
        diff=precision-str_len;
        out_str_ptr->wstr_ptr[0]=MMIUC_ZERO_CHAR;
        out_str_ptr->wstr_ptr[1]=MMIUC_DOT_CHAR;
        out_str_ptr->wstr_len+=2;
        for(loop=0;loop<diff;loop++)
        {
            out_str_ptr->wstr_ptr[2+loop]=MMIUC_ZERO_CHAR;
            out_str_ptr->wstr_len++;
        }

        for(loop=0;loop<str_len;loop++)
        {
            out_str_ptr->wstr_ptr[2+diff+loop]=str[loop];
            out_str_ptr->wstr_len++;
        }
    }

    //下面的代码将结果后面的0去掉；
    loop=out_str_ptr->wstr_len-1;
    //不用担心将整数后面的0也去掉了,
    //上面的转换在整数后加了'.'
    while(out_str_ptr->wstr_ptr[loop]==MMIUC_ZERO_CHAR)
    {
        out_str_ptr->wstr_ptr[loop]=0;
        out_str_ptr->wstr_len--;
        loop--;
    }
    if(MMIUC_DOT_CHAR==out_str_ptr->wstr_ptr[loop])//如果最后的字符是'.'，则去掉。
    {
        out_str_ptr->wstr_ptr[loop]=0;
        out_str_ptr->wstr_len--;
    }

    //最多显示UNITCONV_PRECISION 位小数
    for(loop=0;loop<out_str_ptr->wstr_len;loop++)
    {
        if(MMIUC_DOT_CHAR==out_str_ptr->wstr_ptr[loop])
        {
            break;
        }
        dot_position++;
    }

    loop++;
    for(;loop<out_str_ptr->wstr_len;loop++)
    {
        decimal_count++;
    }
    if(decimal_count>MMIUC_PRECISION_NUM)
    {
        out_str_ptr->wstr_ptr[dot_position+MMIUC_PRECISION_NUM+1]=0;
        out_str_ptr->wstr_len=out_str_ptr->wstr_len-(decimal_count-MMIUC_PRECISION_NUM);
    }
}

/*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL void  UnitConversion (
                            UC_CONV_TYPE_E  type,//in
                            const MMI_STRING_T  *in_str_ptr, //in
                            MMI_STRING_T  *out_str_ptr//out
                            )
{
    UC_STR_TO_NUM_T out_num={0};
// 	uint32 conv_result=0;
    out_num.expand=1;

    //SCI_ASSERT(in_str_ptr!=PNULL&&out_str_ptr!=PNULL);/*assert verified*/
    //SCI_ASSERT(out_str_ptr->wstr_ptr!=PNULL);/*assert verified*/
    if(in_str_ptr == PNULL || out_str_ptr == PNULL || out_str_ptr->wstr_ptr == PNULL)
    {
        return;
    }

    StrToInt32(in_str_ptr,&out_num);

    UnitConversionByRate(&out_num,s_conversion_rate[type],out_str_ptr);

}

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 LOCAL void  UnitConversionByRate(
									 UC_STR_TO_NUM_T *in_num, //in 
									 const char *conv_rate, // in 
									 MMI_STRING_T  *out_str //out
									 )
{
	int32 loop=0;
	int32 i=0;
	UC_STR_TO_NUM_T out;
	uint8 rate_len=0;
	wchar integer_buff[MMIUC_INTEGER_BUFF_LEN]={0};
	uint8 interer_len=0;
	wchar dot_buff[MMIUC_DECIMAL_BUFF_LEN]={0};
	uint8 dot_len=0;
	uint32 count=1;
	uint32 mod_array[MMIUC_MOD_BUFF_LEN]={0};
	uint8 mod_array_num=0;
	uint32 remian_1=0;
	uint32 remian_gene_1=1;
	UC_STR_TO_NUM_T result_interger={0};
	UC_STR_TO_NUM_T result_decimal={0};
	wchar result_decimal_buff[MMIUC_MAX_STR_BUFF_LEN + 1]={0};
	wchar result_interger_buff[MMIUC_MAX_STR_BUFF_LEN + 1]={0};
	MMI_STRING_T  result_decimal_str={0};
	MMI_STRING_T  result_interger_str={0};
	uint32 num=0;
	uint32 expand=1;
	

	SCI_ASSERT(in_num!=PNULL && conv_rate!=PNULL && out_str!=PNULL);/*assert verified*/
	SCI_ASSERT(out_str->wstr_ptr!=PNULL);/*assert verified*/
		
	num=in_num->num;
	expand=in_num->expand;
	rate_len=SCI_STRLEN(conv_rate);
	result_decimal_str.wstr_ptr=result_decimal_buff;
	result_decimal_str.wstr_len=0;
	result_decimal.expand=1;
	result_interger_str.wstr_ptr=result_interger_buff;
	result_interger_str.wstr_len=0;
	result_interger.expand=1;

	//将转换率的整数部分和小数部分分开
	while(conv_rate[loop]!=MMIUC_DOT_CHAR)
	{
		integer_buff[loop]=conv_rate[loop];
		loop++;
		interer_len++;
	}
	
	loop++;
	for(;loop<rate_len;loop++)
	{
		dot_buff[i]=conv_rate[loop];
		i++;
		dot_len++;
	}

	//将输入扩大较大值，注意不能越界，运算时可以提高精度
	while(num<MMIUC_MAX_EXPAND_NUM&&expand<MMIUC_MAX_EXPAND_NUM)
	{
		num=num*MMIUC_DECIMAL_NUM;
		expand=expand*MMIUC_DECIMAL_NUM;
	}


	MMIAPICOM_WstrToNum(integer_buff,interer_len,&out.num);

	out.num=out.num*num;
	out.expand=expand;

	//计算出整数部分并记录下余数部分。
	for(loop=0;loop<dot_len;loop++)
	{
		count=count*MMIUC_DECIMAL_NUM;
		out.num=out.num+num*(dot_buff[loop]-MMIUC_ZERO_CHAR)/count;
		mod_array[loop]=(num*(dot_buff[loop]-MMIUC_ZERO_CHAR))%count;
		mod_array_num++;
	}
	//剩余部分为:remian_1/remian_gene_1；
	remian_gene_1=count;
	for(loop=mod_array_num;loop>0;loop--)
	{
		remian_1+=mod_array[loop-1]*(remian_gene_1/count);
		count=count/MMIUC_DECIMAL_NUM;
	}

	result_interger.num=out.num+remian_1/remian_gene_1;//最后得到的整数
	result_interger.expand=1;
	result_decimal.num=remian_1%remian_gene_1;//最后得到的小数，不过是扩大成整数后的
	result_decimal.expand=remian_gene_1;//扩大倍数
	
	ConversionResultToString(&result_decimal,&result_decimal_str);
	ConversionResultToString(&result_interger,&result_interger_str);

	//SCI_TRACE_LOW:"UC:result_interger.num=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1958_112_2_18_2_2_25_107,(uint8*)"d",result_interger.num);
	//SCI_TRACE_LOW:"UC:result_decimal.num=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1959_112_2_18_2_2_25_108,(uint8*)"d",result_decimal.num);
	//SCI_TRACE_LOW:"UC:result_decimal.expand=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1960_112_2_18_2_2_25_109,(uint8*)"d",result_decimal.expand);

	for(loop=0;loop<result_decimal_str.wstr_len;loop++)
	{
		if(MMIUC_DOT_CHAR==result_decimal_str.wstr_ptr[loop])
		{
			break;
		}
	}
	
	//将整数和小数连接到一起
	if(loop!=result_decimal_str.wstr_len)
	{
		for(;loop<result_decimal_str.wstr_len;loop++)
		{
			result_interger_str.wstr_ptr[result_interger_str.wstr_len++]=result_decimal_str.wstr_ptr[loop];
		}
	}
	
	//还不是最后结果，result_interger_str被扩大了out.expand倍，还原之
	MoveDotByExpand(&result_interger_str,out.expand);
	
	MMIAPICOM_Wstrncpy(out_str->wstr_ptr,result_interger_str.wstr_ptr,result_interger_str.wstr_len);
	out_str->wstr_len=result_interger_str.wstr_len;
	
	DiscardStringRedundance(out_str,MMIUC_PRECISION_NUM);
	DiscardStringInvalidZero(out_str);
}

/*****************************************************************************/
// 	Description : 根据转换类型设置label 控件文本
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void SetTextLabelText(
							  UC_CONV_TYPE_E type // in 
							  )
 {
    //  	MMI_STRING_T  text_type  = {0};
    //MMI_STRING_T  text_label0  = {0};
    MMI_STRING_T  text_label1  = {0};
    MMI_STRING_T  text_label2  = {0};

    switch(type)
    {
        case MMIUC_TYPE_KG_POUND:
            {
                MMI_GetLabelTextByLang(TXT_UC_KILOGRAM, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_POUND, &text_label2);            
            }
            break;

        case MMIUC_TYPE_POUND_KG:
            {
                MMI_GetLabelTextByLang(TXT_UC_POUND, &text_label2);
                MMI_GetLabelTextByLang(TXT_UC_KILOGRAM, &text_label1);
            }
            break;
            
        case MMIUC_TYPE_KG_OUNCE:
            {            
                MMI_GetLabelTextByLang(TXT_UC_KILOGRAM, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_OUNCE, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_OUNCE_KG:
            {
                MMI_GetLabelTextByLang(TXT_UC_OUNCE, &text_label2);
                MMI_GetLabelTextByLang(TXT_UC_KILOGRAM, &text_label1);
            }
            break;
            
        case MMIUC_TYPE_KM_MILE:
            {            
                MMI_GetLabelTextByLang(TXT_UC_KILOMETER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_MILE, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_MILE_KM:
            {
                MMI_GetLabelTextByLang(TXT_UC_KILOMETER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_MILE, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_M_YARD:
            {
                MMI_GetLabelTextByLang(TXT_UC_METER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_YARD, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_YARD_M:
            {
                MMI_GetLabelTextByLang(TXT_UC_METER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_YARD, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_M_FOOT:
            {
                MMI_GetLabelTextByLang(TXT_UC_METER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_FOOT, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_FOOT_M:
            {
                MMI_GetLabelTextByLang(TXT_UC_METER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_FOOT, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_CM_INCH:
            {
                MMI_GetLabelTextByLang(TXT_UC_CENTIMETER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_INCH, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_INCH_CM:
            {
                MMI_GetLabelTextByLang(TXT_UC_CENTIMETER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_INCH, &text_label2);
            }
            break;
            
        default:
            break;
    }

    GUILABEL_SetText(MMIUNITCONV_UNIT_LABEL1_CTRL_ID, &text_label1, FALSE);
    GUILABEL_SetText(MMIUNITCONV_UNIT_LABEL2_CTRL_ID, &text_label2, FALSE);
 }

/*****************************************************************************/
//  Description : display conversion type
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayUcConversionType(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_RECT_T    rect = {0};
    GUI_RECT_T    display_rect ={0};
    GUI_LCD_DEV_INFO    dev_info = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;  
    MMI_STRING_T        type_string = {0};
    uint16 width = 0;
    uint16 height = 0;
    MMITHEME_FORM_T    form_theme = {0};
    
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID), &rect);
    
    if(s_conversion_type_pressed)
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENU_PRESSED,
                (const GUI_LCD_DEV_INFO*)&dev_info);
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENU_DEFAULT,
                (const GUI_LCD_DEV_INFO*)&dev_info);

    }

    //画边框
    if(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID == MMK_GetActiveCtrlId(owner_draw_ptr->win_handle))  
    {
        MMITHEME_GetFormTheme(&form_theme);
        GUI_DisplayBorder(rect, rect, &form_theme.active_child.border, &dev_info);   
    }

    switch(s_conversion_type)
    {
        case MMIUC_TYPE_KG_POUND: 
        case MMIUC_TYPE_POUND_KG:            
            MMI_GetLabelTextByLang(TXT_UC_KILOGRAM_POUND, &type_string);
            break;
            
        case MMIUC_TYPE_KG_OUNCE:
        case MMIUC_TYPE_OUNCE_KG:            
            MMI_GetLabelTextByLang(TXT_UC_KILOGRAM_OUNCE, &type_string);
            break;

        case MMIUC_TYPE_KM_MILE:
        case MMIUC_TYPE_MILE_KM:            
            MMI_GetLabelTextByLang(TXT_UC_KILOMETER_MILE, &type_string);
            break;
            
        case MMIUC_TYPE_M_YARD:
        case MMIUC_TYPE_YARD_M:            
            MMI_GetLabelTextByLang(TXT_UC_METER_YARD, &type_string);
            break;
            
        case MMIUC_TYPE_M_FOOT:
        case MMIUC_TYPE_FOOT_M:              
            MMI_GetLabelTextByLang(TXT_UC_METER_FOOT, &type_string);
            break;
            
        case MMIUC_TYPE_CM_INCH:
        case MMIUC_TYPE_INCH_CM:            
            MMI_GetLabelTextByLang(TXT_UC_CENTIMETER_INCH, &type_string);
            break;

        default:
            break;
    }

    //@leon.wang modify for QQVGA and QCIF
#ifdef MMI_UNITCONV_MINI_SUPPORT
    text_style.align = ALIGN_LVMIDDLE;
    display_rect = rect;
    display_rect.left += MMIUC_OWNDRAW_TITLE_MARGIN;
#else
    text_style.align = ALIGN_HVMIDDLE;
    display_rect = rect;
#endif
    text_style.font = MMIUC_FONT_SIZE;
    text_style.font_color = MMI_WHITE_COLOR;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info,
        (const GUI_RECT_T      *)&display_rect,       //the fixed display area
        (const GUI_RECT_T      *)&display_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&type_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_CONVERTER_MENUICON_DEFAULT, owner_draw_ptr->win_handle);
    rect.top += (rect.bottom - rect.top + 1 -height)/2;
#ifdef MMI_UNITCONV_MINI_SUPPORT
    rect.left = rect.right - width;
#else
    rect.left = rect.right - width - 7;
#endif

    if(s_conversion_type_pressed)
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENUICON_PRESSED,
                (const GUI_LCD_DEV_INFO*)&dev_info);   
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENUICON_DEFAULT,
                (const GUI_LCD_DEV_INFO*)&dev_info);    
    }
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    GUIWIN_SetSoftkeyTextId(owner_draw_ptr->win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
    GUIWIN_SeSoftkeytButtonIconId(owner_draw_ptr->win_handle, IMAGE_NULL, 1, TRUE);
#else
    GUIWIN_SetSoftkeyTextId(owner_draw_ptr->win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
#endif
}

/*****************************************************************************/
//  Discription:  handle change conversion type win
//  Global resource dependence: none 
//  Author:paul.huang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleChangeConversionTypeWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id  = MMIUNITCONV_CHANGE_TYPE_CTRL_ID;
    uint16        cur_select_item = 0;
    uint8    i = 0;
    uint16    pos = 0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:      
            switch (s_uc_type)
            {
                case UC_TYPE_WEIGHT:
                    GUILIST_SetMaxItem(ctrl_id,MMIUC_WEIGHT_TYPE_NUM, FALSE );
                    for(i = 0; i< MMIUC_WEIGHT_TYPE_NUM; i++)
                    {
                        MMIAPISET_AppendListItemByTextId( s_weight_text_id[i], TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
                    }
                    pos = (s_conversion_type - MMIUC_TYPE_KG_POUND)/2;/*lint !e656 */
                    GUILIST_SetSelectedItem(ctrl_id, pos, TRUE);
                    GUILIST_SetCurItemIndex(ctrl_id,pos);        
                    break;

                case UC_TYPE_LENGTH:
                    GUILIST_SetMaxItem(ctrl_id,MMIUC_LENGTH_TYPE_NUM, FALSE );
                    for(i = 0; i< MMIUC_LENGTH_TYPE_NUM; i++)
                    {
                        MMIAPISET_AppendListItemByTextId( s_length_text_id[i], TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
                    }
                    pos = (s_conversion_type - MMIUC_TYPE_KM_MILE)/2;/*lint !e656 */
                    GUILIST_SetSelectedItem(ctrl_id, pos, TRUE);
                    GUILIST_SetCurItemIndex(ctrl_id,pos);        
                    break;

                default:
                    break;
            }     
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            cur_select_item = GUILIST_GetCurItemIndex(ctrl_id);

            switch (s_uc_type)
            {
                case UC_TYPE_WEIGHT:
                    s_conversion_type=MMIUC_TYPE_KG_POUND+2*cur_select_item;
                    break;

                case UC_TYPE_LENGTH:                    
                    s_conversion_type=MMIUC_TYPE_KM_MILE+2*cur_select_item;
                    break;

                default:
                    break;
            }        
            MMK_CloseWin(win_id);
            break;
        
        case MSG_GET_FOCUS: 
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}
/*****************************************************************************/
// 	Description : 设置单位换算显示时各种ctrl、字体等的属性-》只用于计算自适应str使用
//	Global resource dependence :
//  Author:liyan.zhu
//	Note:CR152753
/*****************************************************************************/
LOCAL MMI_RESULT_E ComputeUnitCoversionAdjustSet(
												 MMI_STRING_T    *in_str_ptr,
												 MMI_STRING_T    *out_str_ptr   //可显示下的宽度            
)
{
	GUISTR_STYLE_T  font_style = {0};
	GUISTR_STATE_T  state = GUISTR_STATE_ALIGN|GUISTR_STATE_ANGLE;
	uint16 ctrl_width = (float)(MMI_MAINSCREEN_RIGHT_MAX_PIXEL) * (1.00 - (float)MMIUC_LABEL_WIDTH_PERCENT/100.00) 
		                 - MMIUC_FORM_VSAPCE - MMIUC_FORM_MARGIN * 6 ;
	font_style.font = MMIUC_FONT_SIZE;
    return ComputAdjustStr(in_str_ptr,ctrl_width,font_style,state,out_str_ptr);
}

/*****************************************************************************/
// 	Description : 获取一个字符串显示时的自适应string
//	Global resource dependence :
//  Author:liyan.zhu
//	Note:CR152753
/*****************************************************************************/
LOCAL MMI_RESULT_E ComputAdjustStr(
										   MMI_STRING_T    *in_str_ptr,
										   uint16          ctrl_width,//string显示要小于的宽度
										   GUISTR_STYLE_T  font_style,//字体大小等描述
										   GUISTR_STATE_T  state,//字符串类型
										   MMI_STRING_T    *out_str_ptr   //可显示下的宽度            
)
{
	MMI_RESULT_E     result = MMI_RESULT_TRUE;
	uint16 str_width = GUISTR_GetStringWidth(&font_style, in_str_ptr, state);
	if (PNULL == in_str_ptr)
	{
		return MMI_RESULT_FALSE;
	}
    
	while(str_width > ctrl_width)				
	{//计算自适应str					
		in_str_ptr->wstr_len  = in_str_ptr->wstr_len - 1;
		str_width = GUISTR_GetStringWidth(&font_style, in_str_ptr, state);
	}				

	out_str_ptr = in_str_ptr;

	return result;
}
/*****************************************************************************/
// 	Description : 长度重量单位换算窗口消息处理函数
//	Global resource dependence :
//  Author:renyi.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUnitConvertWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    wchar  dot_str[] = {'.'};
    static wchar last_valid_input_str[MMIUC_MAX_STR_BUFF_LEN + 1];
    static UC_TYPE_E current_type = UC_TYPE_MAX;
    static UC_CONV_TYPE_E current_conversion_type = MMIUC_TYPE_MAX;
    GUIFORM_CHILD_WIDTH_T width = {MMIUC_LABEL_WIDTH_PERCENT, GUIFORM_CHILD_WIDTH_PERCENT};   
    GUIFORM_CHILD_HEIGHT_T height = {0};
    uint16 v_space = MMIUC_FORM_VSAPCE;
    uint16 v_space_h = MMIUC_FORM_HSAPCE;
    uint16 margin = MMIUC_FORM_MARGIN;
    GUIOWNDRAW_TP_HANDLER_T     tp_handler = {0};
    static MMI_CTRL_ID_T  pre_active_ctrl_id = 0;
    BOOLEAN         is_disp_im_icon = FALSE;
    BOOLEAN         is_disp_num_info = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            wchar           forbid_char_value[]= {0x0d, 0x0a, 0x40, 0x26,0x3f, 0x23, 0x20, 0x27, 0x2c};
            SCI_MEMSET(last_valid_input_str, 0, (MMIUC_MAX_STR_BUFF_LEN + 1)*sizeof(wchar));
                
            current_type = UC_TYPE_WEIGHT;
            s_uc_type = UC_TYPE_WEIGHT;
            current_conversion_type = MMIUC_TYPE_KG_POUND;
            s_conversion_type = MMIUC_TYPE_KG_POUND;            
            s_type_pressed = FALSE;
            s_conversion_type_pressed = FALSE;

            GUIFORM_IsSlide(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, FALSE);
            GUIFORM_SetMargin(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID,margin);  
            if(MMITHEME_IsMainScreenLandscape())
            {
                GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space_h);
            }
            else
            {
                GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space);                
            }            
            GUIFORM_SetMargin(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, 0);
            GUIFORM_SetMargin(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, 0);
            GUIFORM_PermitChildBg(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, FALSE);
            GUIFORM_SetChildWidth( MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID,  MMIUNITCONV_UNIT_LABEL1_CTRL_ID,  &width );
            GUIFORM_SetChildWidth( MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID,  MMIUNITCONV_UNIT_LABEL2_CTRL_ID,  &width );  

            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_OWNDRAW_HEIGHT;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID, &height);
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_EDIT_FORM_HEIGHT;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, &height);
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_EDIT_FORM_HEIGHT - 4;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_EDIT1_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_UNIT_LABEL1_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, MMIUNITCONV_EDIT2_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_UNIT_LABEL2_CTRL_ID, &height);

            tp_handler.tp_down  = HandleOwnDrawTpDown;
            tp_handler.tp_move = HandleOwnDrawTpMove;
            tp_handler.tp_up = HandleOwnDrawTpUp;

            GUIOWNDRAW_SetCallback(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, &tp_handler);   
            GUIOWNDRAW_SetCallback(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID, &tp_handler);   

            SetTextLabelText(s_conversion_type);
                
            //set edit style
            GUIEDIT_SetStyle(MMIUNITCONV_EDIT1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetAlign(MMIUNITCONV_EDIT1_CTRL_ID, ALIGN_LVMIDDLE);
            GUIEDIT_SetStyle(MMIUNITCONV_EDIT2_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetAlign(MMIUNITCONV_EDIT2_CTRL_ID, ALIGN_LVMIDDLE);

            GUIEDIT_SetIm(MMIUNITCONV_EDIT1_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_ConfigDisableChar(MMIUNITCONV_EDIT1_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            GUIEDIT_ConfigImCustomKey(MMIUNITCONV_EDIT1_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            GUIEDIT_SetImTag(MMIUNITCONV_EDIT1_CTRL_ID, GUIIM_TAG_DOT_NUM);

            GUIEDIT_SetIm(MMIUNITCONV_EDIT2_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_ConfigDisableChar(MMIUNITCONV_EDIT2_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            GUIEDIT_ConfigImCustomKey(MMIUNITCONV_EDIT2_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            GUIEDIT_SetImTag(MMIUNITCONV_EDIT2_CTRL_ID, GUIIM_TAG_DOT_NUM);
            //modify for NEWM00226567
            GUIEDIT_SetSoftkey(MMIUNITCONV_EDIT1_CTRL_ID, 0, 1, TXT_NULL,TXT_NULL, PNULL);
            GUIEDIT_SetSoftkey(MMIUNITCONV_EDIT2_CTRL_ID, 0, 1, TXT_NULL,TXT_NULL, PNULL);
            //over
            GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT1_CTRL_ID,MMIUC_INPUT_MAX_LEN);
            is_disp_im_icon = TRUE;
            is_disp_num_info = TRUE;
            GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT1_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
            GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT2_CTRL_ID,MMIUC_MAX_STR_BUFF_LEN);
            is_disp_im_icon = FALSE;
            is_disp_num_info = FALSE;
            GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT2_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);

            GUIEDIT_SetFont(MMIUNITCONV_EDIT1_CTRL_ID, MMIUC_FONT_SIZE);            
            GUIEDIT_SetFont(MMIUNITCONV_EDIT2_CTRL_ID, MMIUC_FONT_SIZE);            
            GUIEDIT_SetMargin(MMIUNITCONV_EDIT1_CTRL_ID, MMIUC_EDIT_MARGIN, MMIUC_EDIT_MARGIN);            
            GUIEDIT_SetMargin(MMIUNITCONV_EDIT2_CTRL_ID, MMIUC_EDIT_MARGIN, MMIUC_EDIT_MARGIN);
            GUILABEL_SetFont(MMIUNITCONV_UNIT_LABEL1_CTRL_ID, MMIUC_FONT_SIZE, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
            GUILABEL_SetFont(MMIUNITCONV_UNIT_LABEL2_CTRL_ID, MMIUC_FONT_SIZE, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));

            GUIEDIT_SetClipboardEnabled(MMIUNITCONV_EDIT1_CTRL_ID,FALSE);
            GUIEDIT_SetClipboardEnabled(MMIUNITCONV_EDIT2_CTRL_ID,FALSE);
			GUIFORM_PermitChildFont(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID,FALSE);
			GUIFORM_PermitChildFont(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID,FALSE);
            MMK_SetAtvCtrl(win_id, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID);
            pre_active_ctrl_id = MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID;
        }
        break;   

    case MSG_LOSE_FOCUS:
        s_type_pressed = FALSE;
        s_conversion_type_pressed = FALSE;
         break;

    case MSG_GET_FOCUS:
        if(current_type != s_uc_type)
        {
            wchar  out_buffer[1] = {0}; 

            current_type = s_uc_type;           
            
            switch (s_uc_type)
            {
                case UC_TYPE_WEIGHT:
                    current_conversion_type = MMIUC_TYPE_KG_POUND;
                    s_conversion_type = MMIUC_TYPE_KG_POUND;   
                    break;

                case UC_TYPE_LENGTH:                    
                    current_conversion_type = MMIUC_TYPE_KM_MILE;
                    s_conversion_type = MMIUC_TYPE_KM_MILE;   
                    break;

                default:
                    break;
            }   
            GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_buffer, 1);             
            GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_buffer, 1);
        }
        else
        {  
            if (current_conversion_type !=s_conversion_type)
            {
                MMI_STRING_T in_str_ptr = {0};
                MMI_STRING_T out_str_ptr = {0};
                wchar        out_buffer[MMIUC_MAX_STR_BUFF_LEN + 1] = {0};
                current_conversion_type = s_conversion_type;
                out_str_ptr.wstr_ptr = out_buffer;
                GUIEDIT_GetString(MMIUNITCONV_EDIT1_CTRL_ID, &in_str_ptr);
                if (0 != in_str_ptr.wstr_len)
                {
                   UnitConversion(s_conversion_type, &in_str_ptr, &out_str_ptr);
				   ComputeUnitCoversionAdjustSet(&out_str_ptr,&out_str_ptr);
                   GUIEDIT_SetString(MMIUNITCONV_EDIT2_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);
                }
                else
                {
                   GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_buffer, 1);
                }  
            }
        }
        SetTextLabelText(s_conversion_type); 
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        {
            wchar  out_buffer[1] = {0};  
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            if(MMIUNITCONV_EDIT1_CTRL_ID == pre_active_ctrl_id 
                ||MMIUNITCONV_EDIT2_CTRL_ID == pre_active_ctrl_id
                ||MMIUNITCONV_EDIT1_CTRL_ID == ctrl_id 
                ||MMIUNITCONV_EDIT2_CTRL_ID == ctrl_id )
            {
                if(MMIUNITCONV_EDIT1_CTRL_ID == ctrl_id )
                {
                    if(1 == (s_conversion_type%2) )
                    {
                        s_conversion_type -= 1;          
                        current_conversion_type -= 1;
                    }                 
                    //clear edit box
                    GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT1_CTRL_ID,MMIUC_INPUT_MAX_LEN);
                    is_disp_im_icon = TRUE;
                    is_disp_num_info = TRUE;
                    GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT1_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
                    GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT2_CTRL_ID,MMIUC_MAX_STR_BUFF_LEN);
                    is_disp_im_icon = FALSE;
                    is_disp_num_info = FALSE;
                    GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT2_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
                    GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_buffer, 1);                
                    GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_buffer, 1);
                    SCI_MEMSET(last_valid_input_str, 0, (MMIUC_MAX_STR_BUFF_LEN + 1)*sizeof(wchar));
                    MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
                }
                else if( MMIUNITCONV_EDIT2_CTRL_ID == ctrl_id)
                {
                    if(0 == (s_conversion_type%2) )
                    {
                        s_conversion_type += 1;          
                        current_conversion_type += 1;
                    }                
                    //clear edit box
                    GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT2_CTRL_ID,MMIUC_INPUT_MAX_LEN);
                    is_disp_im_icon = TRUE;
                    is_disp_num_info = TRUE;
                    GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT2_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
                    GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT1_CTRL_ID,MMIUC_MAX_STR_BUFF_LEN);
                    is_disp_im_icon = FALSE;
                    is_disp_num_info = FALSE;
                    GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT1_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
                    GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_buffer, 1);                
                    GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_buffer, 1);
                    SCI_MEMSET(last_valid_input_str, 0, (MMIUC_MAX_STR_BUFF_LEN + 1)*sizeof(wchar));                
                    MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
                }
            }
            pre_active_ctrl_id = ctrl_id;
       }
       break;

   case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
    {
        MMI_STRING_T in_str_ptr = {0};              
        MMI_STRING_T out_str_ptr = {0};        
        wchar        out_buffer[MMIUC_MAX_STR_BUFF_LEN + 1] = {0};  
        wchar        in_buffer[2] = {0}; 
        MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
        static BOOLEAN is_recovery_last_input = FALSE;
        if(PNULL == param)
        {
            break;
        }
        if(((MMI_NOTIFY_T*)param)->src_id != ctrl_id)
        {
            break;
        }
        //if  recovering last input,do nothing
        if(is_recovery_last_input)    
        {
            is_recovery_last_input = FALSE;
            break;
        }      
        
        out_str_ptr.wstr_ptr = out_buffer;
        GUIEDIT_GetString(ctrl_id, &in_str_ptr);

        if(!StrIsValid(&in_str_ptr))
        {
            uint16 length = MMIAPICOM_Wstrlen(last_valid_input_str);
            if((in_str_ptr.wstr_len == 2)
             &&(MMIUC_ZERO_CHAR==in_str_ptr.wstr_ptr[0])
             &&(in_str_ptr.wstr_ptr[1]>=MMIUC_ZERO_CHAR && in_str_ptr.wstr_ptr[1]<=MMIUC_NINE_CHAR))
            { 
                MMIAPICOM_Wstrncpy(in_buffer,in_str_ptr.wstr_ptr,in_str_ptr.wstr_len);
                in_buffer[0] = in_str_ptr.wstr_ptr[1];
                in_buffer[1] = 0;
                GUIEDIT_SetString( ctrl_id, in_buffer, 1); 
                GUIEDIT_GetString( ctrl_id, &in_str_ptr);
	        }
            else if((in_str_ptr.wstr_len == 1)
             &&(MMIUC_DOT_CHAR==in_str_ptr.wstr_ptr[0])
             )
            {
                in_buffer[0] = '0';
                in_buffer[1] = in_str_ptr.wstr_ptr[0];
                GUIEDIT_SetString( ctrl_id, in_buffer, 2); 
                GUIEDIT_GetString(ctrl_id, &in_str_ptr);
            }
            else if(length != 0)
            {
                in_str_ptr.wstr_ptr = last_valid_input_str;
                in_str_ptr.wstr_len = length;
                GUIEDIT_SetString( ctrl_id, in_str_ptr.wstr_ptr, in_str_ptr.wstr_len);  
                is_recovery_last_input = TRUE;
            }
            else if(in_str_ptr.wstr_len!=0)
            {
                GUIEDIT_ClearAllStr(ctrl_id);
                is_recovery_last_input = TRUE;
            }
        }
        if (InputIsValid(&in_str_ptr))
        {                  
            UnitConversion(s_conversion_type, &in_str_ptr, &out_str_ptr);
        }
        else    //invalid input, clear result
        {
            out_str_ptr.wstr_ptr[0] = 0;
            out_str_ptr.wstr_len = 1;
        }
        
        SCI_MEMSET(last_valid_input_str, 0, (MMIUC_MAX_STR_BUFF_LEN + 1)*sizeof(wchar));
        GUIEDIT_GetString(ctrl_id, &in_str_ptr);
        if(in_str_ptr.wstr_ptr!=PNULL && in_str_ptr.wstr_len>0)
        {
            MMIAPICOM_Wstrncpy(last_valid_input_str, in_str_ptr.wstr_ptr,
                in_str_ptr.wstr_len <= MMIUC_MAX_STR_BUFF_LEN ? in_str_ptr.wstr_len : MMIUC_MAX_STR_BUFF_LEN);
        }
            
        if(MMIUNITCONV_EDIT1_CTRL_ID == ctrl_id)
        {
			ComputeUnitCoversionAdjustSet(&out_str_ptr,&out_str_ptr);
            GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);                
        }
        else if(MMIUNITCONV_EDIT2_CTRL_ID == ctrl_id)
        {
			ComputeUnitCoversionAdjustSet(&out_str_ptr,&out_str_ptr);
            GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);
        }
        else
        {}
        MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
    }
    break;

    case MSG_FULL_PAINT:
        break;

    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            
            if(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID == ctrl_id)
            {
                MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_TYPE_WIN_TAB, PNULL);
            }
            else if(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID == ctrl_id)
            {
                MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_CONVERSION_TYPE_WIN_TAB, PNULL);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif

/*****************************************************************************/
// 	Description : the entry of unit conversion
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUNITCONV_OpenMainWin(void)
{
    MMK_CreateWin((uint32*)UNITCONV_MIAN_WIN_TAB, PNULL);
}
#else
/*****************************************************************************/
// 	Description : the entry of unit conversion
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 PUBLIC void MMIAPIUNITCONV_OpenMainWin(void)
 {
 }
#endif//MMI_UNITCONVERSION_SUPPORT
 


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527

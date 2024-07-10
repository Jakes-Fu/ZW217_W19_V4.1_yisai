/*! @file mmi_font.h
@brief 此文件介绍了字体相关的信息
@author Lianxiang
@version 1.0
@date 07/2007
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 
*******************************************************************************/

/*! @page page_Mmi_font Mmi_font

(暂无)

*******************************************************************************/

/*! @addtogroup mmi_fontGrp Mmi_font
@brief mmi_font模块
@details mmi_font模块
@{
*******************************************************************************/

#ifndef _MMI_FONT_H__
#define _MMI_FONT_H__

#include "sci_types.h"
#include "cafdc.h"
#include "mmi_adapt_screen.h"
#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Font Resource 
///////////////////////////////////////////////////////////////////////////////

// Font Type定义的规则
//0号字体的值必须为0, 表示字体高度为0
//16号字体的值必须为0, 表示字体高度为16
//目前支持的最大字体为64, 如要支持更大字号的矢量字体, 需要调大s_vector_buffer
/*! @typedef GUI_FONT_T
@brief 字号
*/
typedef CAF_FONT_TYPE_E     GUI_FONT_T;
/*! @def SONG_FONT_0
@brief 0号字
*/
#define SONG_FONT_0         CAF_FONT_0
/*! @def SONG_FONT_1
@brief 1号字
*/
#define SONG_FONT_1         CAF_FONT_1
/*! @def SONG_FONT_2
@brief 2号字
*/
#define SONG_FONT_2         CAF_FONT_2
/*! @def SONG_FONT_3
@brief 3号字
*/
#define SONG_FONT_3         CAF_FONT_3
/*! @def SONG_FONT_4
@brief 4号字
*/
#define SONG_FONT_4         CAF_FONT_4
/*! @def SONG_FONT_5
@brief 5号字
*/
#define SONG_FONT_5         CAF_FONT_5
/*! @def SONG_FONT_6
@brief 6号字
*/
#define SONG_FONT_6         CAF_FONT_6
/*! @def SONG_FONT_7
@brief 7号字
*/
#define SONG_FONT_7         CAF_FONT_7
/*! @def SONG_FONT_8
@brief 8号字
*/
#define SONG_FONT_8         CAF_FONT_8
/*! @def SONG_FONT_9
@brief 9号字
*/
#define SONG_FONT_9         CAF_FONT_9
/*! @def SONG_FONT_10
@brief 10号字
*/
#define SONG_FONT_10        CAF_FONT_10
/*! @def SONG_FONT_11
@brief 11号字
*/
#define SONG_FONT_11        CAF_FONT_11
/*! @def SONG_FONT_12
@brief 12号字
*/
#define SONG_FONT_12        CAF_FONT_12
/*! @def SONG_FONT_13
@brief 13号字
*/
#define SONG_FONT_13        CAF_FONT_13
/*! @def SONG_FONT_14
@brief 14号字
*/
#define SONG_FONT_14        CAF_FONT_14
/*! @def SONG_FONT_15
@brief 15号字
*/
#define SONG_FONT_15        CAF_FONT_15
/*! @def SONG_FONT_16
@brief 16号字
*/
#define SONG_FONT_16        CAF_FONT_16
/*! @def SONG_FONT_17
@brief 17号字
*/
#define SONG_FONT_17        CAF_FONT_17
/*! @def SONG_FONT_18
@brief 18号字
*/
#define SONG_FONT_18        CAF_FONT_18
/*! @def SONG_FONT_19
@brief 19号字
*/
#define SONG_FONT_19        CAF_FONT_19
/*! @def SONG_FONT_20
@brief 20号字
*/
#define SONG_FONT_20        CAF_FONT_20
/*! @def SONG_FONT_21
@brief 21号字
*/
#define SONG_FONT_21        CAF_FONT_21
/*! @def SONG_FONT_22
@brief 22号字
*/
#define SONG_FONT_22        CAF_FONT_22
/*! @def SONG_FONT_23
@brief 23号字
*/
#define SONG_FONT_23        CAF_FONT_23
/*! @def SONG_FONT_24
@brief 24号字
*/
#define SONG_FONT_24        CAF_FONT_24
/*! @def SONG_FONT_25
@brief 25号字
*/
#define SONG_FONT_25        CAF_FONT_25
/*! @def SONG_FONT_26
@brief 26号字
*/
#define SONG_FONT_26        CAF_FONT_26
/*! @def SONG_FONT_27
@brief 27号字
*/
#define SONG_FONT_27        CAF_FONT_27
/*! @def SONG_FONT_28
@brief 28号字
*/
#define SONG_FONT_28        CAF_FONT_28
/*! @def SONG_FONT_29
@brief 29号字
*/
#define SONG_FONT_29        CAF_FONT_29
/*! @def SONG_FONT_30
@brief 30号字
*/
#define SONG_FONT_30        CAF_FONT_30
/*! @def SONG_FONT_31
@brief 31号字
*/
#define SONG_FONT_31        CAF_FONT_31
/*! @def SONG_FONT_32
@brief 32号字
*/
#define SONG_FONT_32        CAF_FONT_32
/*! @def SONG_FONT_33
@brief 33号字
*/
#define SONG_FONT_33        CAF_FONT_33
/*! @def SONG_FONT_34
@brief 34号字
*/
#define SONG_FONT_34        CAF_FONT_34
/*! @def SONG_FONT_35
@brief 35号字
*/
#define SONG_FONT_35        CAF_FONT_35
/*! @def SONG_FONT_36
@brief 36号字
*/
#define SONG_FONT_36        CAF_FONT_36
/*! @def SONG_FONT_37
@brief 37号字
*/
#define SONG_FONT_37        CAF_FONT_37
/*! @def SONG_FONT_38
@brief 38号字
*/
#define SONG_FONT_38        CAF_FONT_38
/*! @def SONG_FONT_39
@brief 39号字
*/
#define SONG_FONT_39        CAF_FONT_39
/*! @def SONG_FONT_40
@brief 40号字
*/
#define SONG_FONT_40        CAF_FONT_40
/*! @def SONG_FONT_41
@brief 41号字
*/
#define SONG_FONT_41        CAF_FONT_41
/*! @def SONG_FONT_42
@brief 42号字
*/
#define SONG_FONT_42        CAF_FONT_42
/*! @def SONG_FONT_43
@brief 43号字
*/
#define SONG_FONT_43        CAF_FONT_43
/*! @def SONG_FONT_44
@brief 44号字
*/
#define SONG_FONT_44        CAF_FONT_44
/*! @def SONG_FONT_45
@brief 45号字
*/
#define SONG_FONT_45        CAF_FONT_45
/*! @def SONG_FONT_46
@brief 46号字
*/
#define SONG_FONT_46        CAF_FONT_46
/*! @def SONG_FONT_47
@brief 47号字
*/
#define SONG_FONT_47        CAF_FONT_47
/*! @def SONG_FONT_48
@brief 48号字
*/
#define SONG_FONT_48        CAF_FONT_48
/*! @def SONG_FONT_49
@brief 49号字
*/
#define SONG_FONT_49        CAF_FONT_49
/*! @def SONG_FONT_50
@brief 50号字
*/
#define SONG_FONT_50        CAF_FONT_50
/*! @def SONG_FONT_51
@brief 51号字
*/
#define SONG_FONT_51        CAF_FONT_51
/*! @def SONG_FONT_52
@brief 52号字
*/
#define SONG_FONT_52        CAF_FONT_52
/*! @def SONG_FONT_53
@brief 53号字
*/
#define SONG_FONT_53        CAF_FONT_53
/*! @def SONG_FONT_54
@brief 54号字
*/
#define SONG_FONT_54        CAF_FONT_54
/*! @def SONG_FONT_55
@brief 55号字
*/
#define SONG_FONT_55        CAF_FONT_55
/*! @def SONG_FONT_56
@brief 56号字
*/
#define SONG_FONT_56        CAF_FONT_56
/*! @def SONG_FONT_57
@brief 57号字
*/
#define SONG_FONT_57        CAF_FONT_57
/*! @def SONG_FONT_58
@brief 58号字
*/
#define SONG_FONT_58        CAF_FONT_58
/*! @def SONG_FONT_59
@brief 59号字
*/
#define SONG_FONT_59        CAF_FONT_59
/*! @def SONG_FONT_60
@brief 60号字
*/
#define SONG_FONT_60        CAF_FONT_60
/*! @def SONG_FONT_61
@brief 61号字
*/
#define SONG_FONT_61        CAF_FONT_61
/*! @def SONG_FONT_62
@brief 62号字
*/
#define SONG_FONT_62        CAF_FONT_62
/*! @def SONG_FONT_63
@brief 63号字
*/
#define SONG_FONT_63        CAF_FONT_63
/*! @def SONG_FONT_64
@brief 64号字
*/
#define SONG_FONT_64        CAF_FONT_64
/*! @def GUI_MAX_FONT_NUM
@brief 
*/
#define GUI_MAX_FONT_NUM    CAF_MAX_FONT_NUM
// Font Type定义的规则
//0号字体的值必须为0, 表示字体高度为0
//16号字体的值必须为0, 表示字体高度为16
//目前支持的最大字体为64, 如要支持更大字号的矢量字体, 需要调大s_vector_buffer

#define DP_FONT_0 DP2PX_FONT(SONG_FONT_0)
#define DP_FONT_1 DP2PX_FONT(SONG_FONT_1)
#define DP_FONT_2  DP2PX_FONT(SONG_FONT_2)
#define DP_FONT_3  DP2PX_FONT(SONG_FONT_3)
#define DP_FONT_4  DP2PX_FONT(SONG_FONT_4)
#define DP_FONT_5  DP2PX_FONT(SONG_FONT_5)
#define DP_FONT_6  DP2PX_FONT(SONG_FONT_6)
#define DP_FONT_7  DP2PX_FONT(SONG_FONT_7)
#define DP_FONT_8  DP2PX_FONT(SONG_FONT_8)
#define DP_FONT_9  DP2PX_FONT(SONG_FONT_9)
#define DP_FONT_10  DP2PX_FONT(SONG_FONT_10)
#define DP_FONT_11  DP2PX_FONT(SONG_FONT_11)
#define DP_FONT_12  DP2PX_FONT(SONG_FONT_12)
#define DP_FONT_13  DP2PX_FONT(SONG_FONT_13)
#define DP_FONT_14  DP2PX_FONT(SONG_FONT_14)
#define DP_FONT_15  DP2PX_FONT(SONG_FONT_15)
#define DP_FONT_16  DP2PX_FONT(SONG_FONT_16)
#define DP_FONT_17  DP2PX_FONT(SONG_FONT_17)
#define DP_FONT_18  DP2PX_FONT(SONG_FONT_18)
#define DP_FONT_19  DP2PX_FONT(SONG_FONT_19)
#define DP_FONT_20  DP2PX_FONT(SONG_FONT_20)
#define DP_FONT_21  DP2PX_FONT(SONG_FONT_21)
#define DP_FONT_22  DP2PX_FONT(SONG_FONT_22)
#define DP_FONT_23  DP2PX_FONT(SONG_FONT_23)
#define DP_FONT_24  DP2PX_FONT(SONG_FONT_24)
#define DP_FONT_25  DP2PX_FONT(SONG_FONT_25)
#define DP_FONT_26  DP2PX_FONT(SONG_FONT_26)
#define DP_FONT_27  DP2PX_FONT(SONG_FONT_27)
#define DP_FONT_28  DP2PX_FONT(SONG_FONT_28)
#define DP_FONT_29  DP2PX_FONT(SONG_FONT_29)
#define DP_FONT_30  DP2PX_FONT(SONG_FONT_30)
#define DP_FONT_31  DP2PX_FONT(SONG_FONT_31)
#define DP_FONT_32  DP2PX_FONT(SONG_FONT_32)
#define DP_FONT_33  DP2PX_FONT(SONG_FONT_33)
#define DP_FONT_34  DP2PX_FONT(SONG_FONT_34)
#define DP_FONT_35  DP2PX_FONT(SONG_FONT_35)
#define DP_FONT_36  DP2PX_FONT(SONG_FONT_36)
#define DP_FONT_37  DP2PX_FONT(SONG_FONT_37)
#define DP_FONT_38  DP2PX_FONT(SONG_FONT_38)
#define DP_FONT_39  DP2PX_FONT(SONG_FONT_39)
#define DP_FONT_40  DP2PX_FONT(SONG_FONT_40)
#define DP_FONT_41  DP2PX_FONT(SONG_FONT_41)
#define DP_FONT_42  DP2PX_FONT(SONG_FONT_42)
#define DP_FONT_43  DP2PX_FONT(SONG_FONT_43)
#define DP_FONT_44  DP2PX_FONT(SONG_FONT_44)
#define DP_FONT_45  DP2PX_FONT(SONG_FONT_45)
#define DP_FONT_46  DP2PX_FONT(SONG_FONT_46)
#define DP_FONT_47  DP2PX_FONT(SONG_FONT_47)
#define DP_FONT_48  DP2PX_FONT(SONG_FONT_48)
#define DP_FONT_49  DP2PX_FONT(SONG_FONT_49)
#define DP_FONT_50  DP2PX_FONT(SONG_FONT_50)
#define DP_FONT_51  DP2PX_FONT(SONG_FONT_51)
#define DP_FONT_52  DP2PX_FONT(SONG_FONT_52)
#define DP_FONT_53  DP2PX_FONT(SONG_FONT_53)
#define DP_FONT_54  DP2PX_FONT(SONG_FONT_54)
#define DP_FONT_55  DP2PX_FONT(SONG_FONT_55)
#define DP_FONT_56  DP2PX_FONT(SONG_FONT_56)
#define DP_FONT_57  DP2PX_FONT(SONG_FONT_57)
#define DP_FONT_58  DP2PX_FONT(SONG_FONT_58)
#define DP_FONT_59  DP2PX_FONT(SONG_FONT_59)
#define DP_FONT_60  DP2PX_FONT(SONG_FONT_60)
#define DP_FONT_61  DP2PX_FONT(SONG_FONT_61)
#define DP_FONT_62  DP2PX_FONT(SONG_FONT_62)
#define DP_FONT_63  DP2PX_FONT(SONG_FONT_63)
#define DP_FONT_64  DP2PX_FONT(SONG_FONT_64)




/*! @struct _GUI_FONT_INFO_T
@brief 
*/
typedef struct _GUI_FONT_INFO_T
{
	GUI_FONT_T   type;                //font type
	uint32       ngb_db_size;
	uint8      * gb_database_ptr;     //font GB DB
	uint32       nascii_db_size;
	uint8      * pascii_ptr;          //font ASCII DB
} GUI_FONT_INFO_T, * P_GUI_FONT_INFO_T;

/*! @struct _GUI_FONT_DB_T
@brief 
*/
typedef struct _GUI_FONT_DB_T
{
	uint32       db_size;
	uint8      * pdb;                 //font DB
} GUI_FONT_DB_T, * P_GUI_FONT_DB_T;

/*! @struct _GUI_FONT_INFO_EX_T
@brief 
*/
typedef struct _GUI_FONT_INFO_EX_T
{
	GUI_FONT_T   type;                //font type
	uint8      * pdbtt;               //DBs,point to GUI_FONT_DB_T
} GUI_FONT_INFO_EX_T, * P_GUI_FONT_INFO_EX_T;


#ifdef __cplusplus
}
#endif

#endif // _MMI_FONT_DEF_H__

/*@}*/

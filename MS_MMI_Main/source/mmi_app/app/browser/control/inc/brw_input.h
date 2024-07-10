/*****************************************************************
 **  File Name: 	brw_input.h    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:header file of history module                   *
 **  note:如果修改BRW_VISIT_HIS_ITEM_T或者BRW_VISIT_HIS_INFO_T结 *
 **  构,修改BRW_HISTORY_FILE_NAME中的版本标识                  *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  07/04/2009		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef BRW_INPUT_H
#define BRW_INPUT_H

#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
#include "brw_control_api.h"
#include "brw_control.h"
//001标识版本号,当BRW_INPUT_HIS_INFO_T或BRW_INPUT_HIS_ITEM_T修改的时候,更改版本标识,避免文件兼容的问题
#define BRW_INPUT_HISTORY_FILE_NAME   "inputhistory001.dat"

typedef struct tagBRW_INPUT_HIS_ITEM_T
{
    BRW_INPUT_HIS_INFO_T input_his_info;
    BOOLEAN is_used;
}BRW_INPUT_HIS_ITEM_T;

typedef struct tagBRW_INPUT_HIS_T
{
    BRW_INPUT_HIS_ITEM_T input_his_item[BRW_MAX_INPUT_HIS_ITEM_NUM];
    uint16 his_num;
}BRW_INPUT_HIS_T;
#endif

#endif


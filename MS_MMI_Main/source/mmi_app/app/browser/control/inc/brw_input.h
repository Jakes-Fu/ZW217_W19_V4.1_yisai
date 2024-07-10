/*****************************************************************
 **  File Name: 	brw_input.h    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:header file of history module                   *
 **  note:����޸�BRW_VISIT_HIS_ITEM_T����BRW_VISIT_HIS_INFO_T�� *
 **  ��,�޸�BRW_HISTORY_FILE_NAME�еİ汾��ʶ                  *
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
//001��ʶ�汾��,��BRW_INPUT_HIS_INFO_T��BRW_INPUT_HIS_ITEM_T�޸ĵ�ʱ��,���İ汾��ʶ,�����ļ����ݵ�����
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


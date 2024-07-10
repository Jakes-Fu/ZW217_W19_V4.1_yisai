/******************************************************************************
** File Name:    	Thermal_phy.c                                                				*
** Author:         	Tianyu.Yang                                                   				*
** DATE:           	22/02/2023                                               					*
** Copyright:      	2023 Spreadtrum, Incoporated. All Rights Reserved.         		*
** Description:    	This file defines the basic thermalr manage operation process.   	*
******************************************************************************

******************************************************************************
**                        Edit History                                       						*
** -------------------------------------------------------------------------
** DATE           	NAME             	DESCRIPTION                               			*
** 22/02/2023    	Tianyu.Yang     	Create.                                  				*
******************************************************************************/

#ifndef _Thermal_H_
#define _Thermal_H_

#include "sci_api.h"

typedef struct THERMALSVR_SIG_tag {
	SIGNAL_VARS
	uint32  sig_param;
}THERMALSVR_SIG_T;

typedef enum
{
    THERMAL_CLOSE_BUSINESS = 0x1,
    THERMAL_REOPEN_BUSINESS,
    THERMAL_POWER_OFF,
    THERMAL_MSG_MAX_NUM
} THERMAL_SVR_MSG_SERVICE_E;

typedef enum {
	THERMAL_MONITOR_MSG = 1 ,
	THERMAL_MAX_MSG
} THERMAL_MSG_E;

typedef enum THERMAL_StateCode_enum
{
	IDLE_TEMP = 0,
	SKIN_TEMP,
	BOARD_TEMP,
	RF_TEMP,
	STOP_TERMAL
} THERMAL_STATE_E;

typedef struct
{
    THERMAL_STATE_E          thermal_state;
    int32                  		skin_temp;
    int32                  		board_temp;
    int32                  		rf_temp;
}THERMAL_STATE_INFO_T;

LOCAL THERMAL_STATE_INFO_T thermal_module_state =
{
    IDLE_TEMP,        //thermal_state
    25,                  //skin_temp
    0,                  //board_temp
    0,                  //rf_temp
};

LOCAL const int16 skin_temp_table[][2]=
{
	{961	,  -30 },
	{951	,  -29 },
	{941	,  -28 },
	{930	,  -27 },
	{918	,  -26 },
	{907	,  -25 },
	{895	,  -24 },
	{883	,  -23 },
	{870	,  -22 },
	{857	,  -21 },
	{844	,  -20 },
	{831	,  -19 },
	{817	,  -18 },
	{803	,  -17 },
	{789	,  -16 },
	{775	,  -15 },
	{760	,  -14 },
	{746	,  -13 },
	{731	,  -12 },
	{716	,  -11 },
	{701	,  -10 },
	{686	,  -9  },
	{671	,  -8  },
	{656	,  -7  },
	{641	,  -6  },
	{625	,  -5  },
	{610	,  -4  },
	{595	,  -3  },
	{580	,  -2  },
	{565	,  -1  },
	{551	,  0   },
	{536	,  1   },
	{521	,  2   },
	{507	,  3   },
	{493	,  4   },
	{479	,  5   },
	{465	,  6   },
	{451	,  7   },
	{438	,  8   },
	{425	,  9   },
	{412	,  10  },
	{399	,  11  },
	{387	,  12  },
	{375	,  13  },
	{363	,  14  },
	{352	,  15  },
	{340	,  16  },
	{329	,  17  },
	{319	,  18  },
	{308	,  19  },
	{298	,  20  },
	{288	,  21  },
	{279	,  22  },
	{269	,  23  },
	{260	,  24  },
	{251	,  25  },
	{243	,  26  },
	{235	,  27  },
	{227	,  28  },
	{219	,  29  },
	{211	,  30  },
	{204	,  31  },
	{197	,  32  },
	{190	,  33  },
	{184	,  34  },
	{177	,  35  },
	{171	,  36  },
	{165	,  37  },
	{160	,  38  },
	{154	,  39  },
	{149	,  40  },
	{144	,  41  },
	{139	,  42  },
	{134	,  43  },
	{129	,  44  },
	{125	,  45  },
	{121	,  46  },
	{116	,  47  },
	{112	,  48  },
	{109	,  49  },
	{105	,  50  },
	{101	,  51  },
	{98		,  52  },
	{95		,  53  },
	{91		,  54  },
	{88		,  55  },
	{85		,  56  },
	{82   	,  57  },
	{80		,  58  },
	{77		,  59  },
	{74		,  60  },
	{72		,  61  },
	{70		,  62  },
	{67		,  63  },
	{65		,  64  },
	{63		,  65  },
	{61		,  66  },
	{59		,  67  },
	{57		,  68  },
	{55		,  69  },
	{53		,  70  },
	{52		,  71  },
	{50		,  72  },
	{48		,  73  },
	{47		,  74  },
	{45		,  75  },
	{44		,  76  },
	{42		,  77  },
	{41		,  78  },
	{40		,  79  },
	{39		,  80  },
	{37		,  81  },
	{36		,  82  },
	{35		,  83  },
	{34		,  84  },
	{33		,  85  },
	{32		,  86  },
	{31		,  87  },
	{30		,  88  },
	{29		,  89  },
	{28		,  90  },
	{27		,  91  },
	{27		,  92  },
	{26		,  93  },
	{25		,  94  },
	{24		,  95  },
	{24		,  96  },
	{23		,  97  },
	{22		,  98  },
	{22		,  99  },
	{21		,  100 },
};

PUBLIC void thermal_init(void);
LOCAL void _THERMAL_SrvThread (uint32 argc, void *argv);
LOCAL uint32 _THERMAL_TimerHandler (uint32 param);
PUBLIC void THERMAL_SendMsgToTHMTask(THERMAL_MSG_E sig,uint32 sig_param);
PUBLIC int32 THERMAL_Skin_Temp_Read(void);
PUBLIC uint32 THERMAL_Ctrl_Get (void);
PUBLIC void THERMAL_Ctrl_Set (uint32 value);
LOCAL void THERMAL_Read_Temperature (void);
LOCAL void THERMAL_SrvDefault (BLOCK_ID id, uint32 argc, void *argv);
LOCAL void THERMAL_Skin_Temp_Action (int32 skin_temp);
#endif
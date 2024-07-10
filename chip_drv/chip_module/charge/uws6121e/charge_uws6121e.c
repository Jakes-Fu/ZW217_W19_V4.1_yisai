/******************************************************************************
 ** File Name:      charge.c                                                 *
 ** Author:         Benjamin.Wang                                                   *
 ** DATE:           24/11/2004                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic charging and power manage operation process.   *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                               *
 ** 24/11/2004      Benjamin.Wang       Create.                                   *
 ** 26/08/2011      Mingwei.Zhang       Modify
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "chip_plf_export.h"
#include "tasks_id.h"
#include "chg_drvapi.h"
#include "dal_power.h"
#include "doidleonpage.h"
#include "charge_phy.h"
#include "priority_system.h"
#include "efuse_phy.h"
#include "nv_productionparam_type.h"
#include "nv_item_id.h"
#include "prod_param.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
#define DEBUG_CHGMNG

#ifdef DEBUG_CHGMNG
#define CHGMNG_PRINT( _format_string )   SCI_TRACE_LOW _format_string
#else
#define CHGMNG_PRINT( _format_string )
#endif

#define TASK_CHGMNG_SRV_PRI    PRI_CHGMNG_SRV_TASK   //SCI_PRIORITY_NORMAL
/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
//when the phone is staying in busy state(for example, talking, play games or play music, etc.),we will stop
//the state timer until it is not busy.
#define BUSYSTATE               1
//#define CHARGING_TIMER_INTERVAL 1000

#define CHARGING_ON_TIMER_INTERVAL  (9000)
#define CHARGING_OFF_TIMER_INTERVAL (1000)

/* ISENSE�����ĵ�ѹֵ���Ի�׼1.2V�����ʱ������ռ�趨�������İٷֱȣ�
   ISENSE�����ĵ�ѹֵ�ٳ���0.48�͸�AUXADC
   Auxadc�Ǳ�Ӧ��ʹ��С���������������ѹ,����1��1
*/
#define VOL_TO_CUR_PARAM           (576)
#define VBAT_VOL_DIV_P1            (1000)
#define VBAT_VOL_DIV_P2            1000

#define VCHG_DIV_P1             (74)         ///voltage divider 0.0739, 9.2v -> 0.68v
#define VCHG_DIV_P2             (1000)

#define DISCHG_VBAT_BUFF_SIZE    32
#define CHG_VBAT_BUFF_SIZE          16
#define CHG_CURRENT_BUFF_SIZE   (CHG_VBAT_BUFF_SIZE >> 1)

#define CHG_TEMP_BUFF_SIZE      5        //mingwei
#define CHG_NV_TEMP_OFFSET        1000    //NV value must be positive,e.g. nv = -5C + CHG_NV_TEMP_OFFSET
#define VBAT_RESULT_NUM         7       //ADC sampling number
#define VPROG_RESULT_NUM        7       //ADC sampling number

#define CHGMNG_STACK_SIZE       (1024*2)
#define CHGMNG_QUEUE_NUM        24
#define BAT_CAP_ALMOST_FULL (99)
#define PULSE_POS_LEVEL_WIDTH    (9)
#define PULSE_NEG_LEVEL_WIDTH    (1)
#define PULSE_PERIOD            (PULSE_POS_LEVEL_WIDTH + PULSE_NEG_LEVEL_WIDTH)

#define CHGMNG_CALI_MODE_END_VOLT 4500
#define CHGMNG_CALI_MODE_BAT_OVP_VOLT 4500


#define OVP_DETECT_VALID_TIMES  (3)
extern adcPmicEfuse_data_t adcEffusePara;

#define _CHGMNG_COUNTER_INIT(c, def)    (c = def)
#define _CHGMNG_COUNTER_ADD(c)            (c++)
#define _CHGMNG_COUNTER_ROUND(c, round)    { if (c == round) c = 0; }
#define _CHGMNG_COUNTER_ADD_AND_AUTO_ROUND(c, round)    {\
    _CHGMNG_COUNTER_ADD(c);                                 \
    _CHGMNG_COUNTER_ROUND(c, round);                     \
}

#define _CHGMNG_COUNTER_DEC(c)                (c--)
#define _CHGMNG_COUNTER_RELOAD(c, reload)    { if (c == 0) c = reload; }
#define _CHGMNG_COUNTER_DEC_AND_AUTO_RELOAD(c, round)    {\
    _CHGMNG_COUNTER_DEC(c);                                 \
    _CHGMNG_COUNTER_RELOAD(c, round);                     \
}

typedef enum {
    CHGMNG_ADP_PLUGIN,
    CHGMNG_RECHARGE,
    CHGMNG_FAULT_RESUME
}CHGMNG_CHG_START_TYPE_E;
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL BLOCK_ID chg_task_id;

/*CHGMNG_CHARGING state use*/
/* IN 8910, Ϊ���͹����޸�Ϊ��9s��ʱ����ֹͣ�������1s��ʱ�� */
LOCAL SCI_TIMER_PTR g_charging_timer = NULL;      //charge state timer
LOCAL uint32        g_charge_plus = 0;
LOCAL BOOLEAN ischgmng_start = SCI_FALSE;     //charge manager start flag.
LOCAL BOOLEAN isshutting_down = SCI_FALSE;    //shutdowe flag
LOCAL CHG_SWITPOINT_E hw_switch_point = CHG_SWITPOINT_15; //The lowest switchover point between cc and cv modes.
LOCAL uint16 cv_status_counter = 0;
LOCAL uint16 charge_endtime_counter = 0;
LOCAL uint16 warning_counter = 0;
//LOCAL uint16 pulse_counter = 0;
LOCAL uint16 ovp_detect_counter = 0;
LOCAL uint16 ovp_detect_bat_cnt = 0;
/* recent_message_flag: Record the recent message which has been send before client registes.*/
LOCAL uint32 recent_message[10] = {0};     // Ten is enough!

LOCAL CHGMNG_ADC_CAL_T adc_cal = {3505,4200,1207,1000,CAL_TYPR_NO};
uint8 count_paging = 3; // Init value is max to avoid display error when system start up

extern uint8_t prvAdcScale_23_calibrated_get(void);

#define BAT_CAPACITY_STEP   12
LOCAL uint16 dischg_bat_capacity_table[BAT_CAPACITY_STEP][2] =
{
    {4164,  100},
    {4058,  90},
    {3980,  80},
    {3919,  70},
    {3858,  60},
    {3804,  50},
    {3780,  40},
    {3763,  30},
    {3726,  20},
    {3694,  15},
    {3544,  5},
    {3400,  0},
};

#if defined(CHGMNG_PSE_SUPPORT)
LOCAL uint16 dischg_bat_capacity_high_table[BAT_CAPACITY_STEP][2] =
{
    {4100,  100},
    {4030,  90},
    {3960,  80},
    {3890,  70},
    {3820,  60},
    {3750,  50},
    {3680,  40},
    {3610,  30},
    {3540,  20},
    {3505,  15},
    {3435,  5},
    {3400,  0},
};
LOCAL uint16 dischg_bat_capacity_low_table[BAT_CAPACITY_STEP][2] =
{
    {4100,  100},
    {4030,  90},
    {3960,  80},
    {3890,  70},
    {3820,  60},
    {3750,  50},
    {3680,  40},
    {3610,  30},
    {3540,  20},
    {3505,  15},
    {3435,  5},
    {3400,  0},

};
#endif

LOCAL uint16 chg_bat_capacity_table[BAT_CAPACITY_STEP][2]=
{
    {4200,  100},
    {4180,  90},
    {4119,  80},
    {4080,  70},
    {4020,  60},
    {3970,  50},
    {3920,  40},
    {3880,  30},
    {3860,  20},
    {3830,  15},
    {3730,  5},
    {3251,  0},
};

LOCAL CHGMNG_DISCHARGE_PARAM_T dischg_param =
{
    3440,     //warning_vol
    3380,     //shutdown_vol
    3000,     //deadline_vol
    24,       //warning_count,warning interval
};

LOCAL CHGMNG_CHARGE_PARAM_T chg_param =
{
    4120,       //rechg_vol
    4200,         //chg_end_vol
    4330,
    CHARGER_CURRENT_400MA,      //standard_chg_current
    CHARGER_CURRENT_300MA,      //usb_chg_current
    CHARGER_CURRENT_300MA,  //nonstandard_current_sel,0:usb charge current,1:normal charge current
    18000,      //18000S
};

LOCAL CHGMNG_OVP_PARAM_T ovp_param =
{
    0,                  //ovp_type
    0,                  //ovp_over_vol
    0,                  //ovp_resume_vol
};

PUBLIC CHGMNG_STATE_INFO_T module_state =
{
    CHGMNG_IDLE,        //chgmng_state
    0,                  //bat_statistic_vol
    0,                  //bat_cur_vol
    0,                  //bat_remain_cap
    0,                  //charging_current
    0,                  //charging_temperature
    CHGMNG_ADP_UNKNOW,  //adp_type
    CHGMNG_INVALIDREASON, //charging_stop_reason
    0,                  //chg_vol
};
#if defined(CHGMNG_PSE_EVT_SUPPORT)
LOCAL const int16 vbat_temp_table[][2]=
{
    {1143,  -40},
    {1139,  -35},
    {1135,  -30},
    {1129,  -25},
    {1121,  -20},
    {1112,  -15},
    {1100,  -10},
    {1097,  -9},
    {1094,  -8},
    {1091,  -7},
    {1088,  -6},
    {1085,  -5},
    {1082,  -4},
    {1079,  -3},
    {1076,  -2},
    {1073,  -1},
    {1067,  0},
    {1063,  1},
    {1059,  2},
    {1055,  3},
    {1051,  4},
    {1044,  5},
    {1039,  6},
    {1034,  7},
    {1029,  8},
    {1024,  9},
    {1018,  10},
    {1012,  11},
    {1006,  12},
    {1000,  13},
    {994,  14},
    {987,  15},
    {980,  16},
    {973,  17},
    {966,  18},
    {959,  19},
    {951,  20},
    {943,  21},
    {935,  22},
    {927,  23},
    {919,  24},
    {911,  25},
    {902,  26},
    {893,  27},
    {884,  28},
    {875,  29},
    {866,  30},
    {864,  31},
    {862,  32},
    {861,  33},
    {860,  34},
    {817,  35},
    {806,  36},
    {795,  37},
    {785,  38},
    {775,  39},
    {765,  40},
    {755,  41},
    {744,  42},
    {733,  43},
    {722,  44},
    {711,  45},
    {791,  46},
    {782,  47},
    {773,  48},
    {764,  49},
    {655,  50},
    {644,  51},
    {633,  52},
    {622,  53},
    {611,  54},
    {600,  55},
    {589,  56},
    {578,  57},
    {567,  58},
    {556,  59},
    {546,  60},
    {535,  61},
    {524,  62},
    {513,  63},
    {502,  64},
    {493,  65},
    {483,  66},
    {473,  67},
    {463,  68},
    {453,  69},
    {444,  70},
    {435,  71},
    {426,  72},
    {417,  73},
    {408,  74},
    {397,  75},
    {354,  80},
    {314,  85},
    {278,  90},
    {246,  95},
    {217,  100},
    {192,  105},
    {168,  110},
    {150,  115},
    {133,  120},
    {118,  125},
};
#else
LOCAL const int16 vbat_temp_table[][2]=
{
    {1451,  -40},
    {1435,  -39},
    {1419,  -38},
    {1402,  -37},
    {1384,  -36},
    {1367,  -35},
    {1348,  -34},
    {1330,  -33},
    {1311,  -32},
    {1292,  -31},
    {1272,  -30},
    {1253,  -29},
    {1233,  -28},
    {1212,  -27},
    {1192,  -26},
    {1171,  -25},
    {1150,  -24},
    {1129,  -23},
    {1108,  -22},
    {1087,  -21},
    {1066,  -20},
    {1045,  -19},
    {1023,  -18},
    {1002,  -17},
    {981,  -16},
    {960,  -15},
    {938,  -14},
    {917,  -13},
    {896,  -12},
    {875,  -11},
    {854,  -10},
    {834,  -9},
    {814,  -8},
    {794,  -7},
    {773,  -6},
    {754,  -5},
    {735,  -4},
    {716,  -3},
    {697,  -2},
    {678,  -1},
    {660,  0},
    {642,  1},
    {625,  2},
    {608,  3},
    {591,  4},
    {574,  5},
    {558,  6},
    {542,  7},
    {527,  8},
    {512,  9},
    {497,  10},
    {483,  11},
    {468,  12},
    {456,  13},
    {441,  14},
    {428,  15},
    {416,  16},
    {403,  17},
    {391,  18},
    {379,  19},
    {368,  20},
    {357,  21},
    {346,  22},
    {335,  23},
    {326,  24},
    {316,  25},
    {306,  26},
    {297,  27},
    {288,  28},
    {279,  29},
    {271,  30},
    {262,  31},
    {254,  32},
    {247,  33},
    {239,  34},
    {231,  35},
    {225,  36},
    {218,  37},
    {211,  38},
    {205,  39},
    {199,  40},
    {193,  41},
    {187,  42},
    {181,  43},
    {176,  44},
    {170,  45},
    {165,  46},
    {160,  47},
    {156,  48},
    {151,  49},
    {146,  50},
    {142,  51},
    {138,  52},
    {134,  53},
    {130,  54},
    {126,  55},
    {122,  56},
    {118,  57},
    {115,  58},
    {112,  59},
    {108,  60},
    {105,  61},
    {102,  62},
    {99,  63},
    {97,  64},
    {94,  65},
    {91,  66},
    {89,  67},
    {86,  68},
    {84,  69},
    {81,  70},
    {79,  71},
    {77,  72},
    {75,  73},
    {73,  74},
    {71,  75},
    {69,  76},
    {67,  77},
    {65,  78},
    {63,  79},
    {62,  80},
    {60,  81},
    {58,  82},
    {57,  83},
    {55,  84},
    {54,  85},
    {52,  86},
    {51,  87},
    {50,  88},
    {49,  89},
    {47,  90},
    {46,  91},
    {45,  92},
    {44,  93},
    {43,  94},
    {42,  95},
};
#endif


LOCAL const int16 ntc_temp_table[][2]=
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





LOCAL CHGMNG_STATE_INFO_T module_state_to_app;

typedef struct
{
    uint32 queue[DISCHG_VBAT_BUFF_SIZE];
    uint32 pointer;
    uint32 sum;
    uint32 queue_len;
} VBAT_QUEUE_INFO_T;
LOCAL VBAT_QUEUE_INFO_T vbat_queue;

typedef struct
{
    uint32 queue[CHG_CURRENT_BUFF_SIZE];
    uint32 pointer;
    uint32 sum;
} CURRENT_QUEUE_INFO_T;
LOCAL CURRENT_QUEUE_INFO_T current_queue;

#ifdef CHGMNG_PSE_SUPPORT
typedef struct
{
    uint32 queue[CHG_TEMP_BUFF_SIZE];
    uint32 pointer;
    uint32 sum;
} TEMP_QUEUE_INFO_T;
LOCAL TEMP_QUEUE_INFO_T temp_queue;
#endif

#ifdef CHGMNG_PSE_SUPPORT
#define CHGMNG_POWERON_VAL 3200
#define HOT_TEMP 1071
uint32 pse_once = SCI_FALSE;

LOCAL CHGMNG_TEMP_PARAM_T temp_param =
{
    990,     // cold_temp;          -10C, cold_temp = 1000 + temperatue
    1000,     // low_temp;             0C, low_temp = 1000 + temperatue
    1010,     // specific_temp;       10C  specific_temp = 1000 + temperatue
    1022,     // standard_temp;       22C  standard_temp = 1000 + temperatue
    1045,     // high_temp;         45C  high_temp = 1000 + temperatue
    1055,     // hot_temp;          50C  hot_temp = 1000 + temperatue

};

LOCAL CHGMNG_PSE_CHARGE_PARAM_T pse_chg_param =
{
    4100,                  // hot_chg_end_vol;
    4000,                  // hot_rechg_vol;
    4100,                   // high_chg_end_vol;
    4000,                   // high_rechg_vol;
    4200,                   // standard_chg_end_vol;
    4100,                   // standard_rechg_vol;
    4200,                   // specific_chg_end_vol;
    4100,                   // specific_rechg_vol;
    4100,                   // low_chg_end_vol;
    3900,                   // low_rechg_vol;
    4100,                   // cold_chg_end_vol;
    3900,                   // cold_rechg_vol;
    CHARGER_CURRENT_300MA, //hot_chg_current;
    CHARGER_CURRENT_500MA, // high_chg_current;
    CHARGER_CURRENT_500MA, // standard_chg_current;
    CHARGER_CURRENT_500MA, // specific_chg_current;
    CHARGER_CURRENT_350MA, // low_chg_current;
    CHARGER_CURRENT_300MA, // cold_chg_current;
    18000,                    // chg_timeout;
      120,                  // chg_end_current;
};

LOCAL CHGMNG_PSE_PARAM_T pse_param[6];

static uint16 temp_region = PSE_BATTERY_TEMP_REGION__STANDARD;
static uint16 temp_bat_region = PSE_BATTERY_TEMP_REGION__STANDARD;

#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
LOCAL void _CHGMNG_SrvDefault (BLOCK_ID id, uint32 argc, void *argv);
LOCAL uint32 _CHGMNG_VoltagetoPercentum (uint32 voltage, CHGMNG_STATE_E  is_charging, BOOLEAN update);
LOCAL void _CHGMNG_StartCharge (CHGMNG_CHG_START_TYPE_E start_type);
LOCAL void _CHGMNG_SendMessagetoClient (CHR_SVR_MSG_SERVICE_E  msg, uint32 param);
LOCAL uint32 _CHGMNG_GetPhoneState (void);
#ifdef BATTERY_DETECT_SUPPORT
LOCAL uint32 _CHGMNG_BatteryOffRoutine(void);
#endif
LOCAL void _CHGMNG_VbatMonitorRoutine (void);
LOCAL void _CHGMNG_VBatQueueInit (uint32 vbat_vol,uint32 queue_len);
LOCAL void _CHGMNG_ChargeTimerHandler (uint32 state);
LOCAL uint32 _CHGMNG_VBatTimerHandler (uint32 param);
LOCAL void _CHGMNG_StopCharge (CHGMNG_STOPREASON_E reason);
LOCAL uint32 _CHGMNG_CheckVbatPresent (void);
LOCAL void _CHGMNG_ChargeMonitorRoutine(void);
LOCAL void _CHGMNG_SrvThread (uint32 argc, void *argv);
LOCAL void _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_E fsm_event,uint32 condition);
LOCAL void _CHGMNG_SetChargeCurrent (CHGMNG_ADAPTER_TYPE_E mode);
LOCAL uint32 _CHGMNG_GetVBATADCResult (void);
LOCAL uint32 _CHGMNG_GetVbatVol(void);
LOCAL uint32 _CHGMNG_GetCurrentTemp(void);
LOCAL void _CHGMNG_TempQueueInit(uint32 temp);
LOCAL void _CHGMNG_TempQueueUpdate(uint32 temp);

#ifdef DUAL_BATTERY_SUPPORT
LOCAL void _CHGMNG_ModuleReset(uint32 voltage);
#endif

#ifdef CHGMNG_PSE_SUPPORT
PUBLIC uint16 CHGMNG_AdjustTempRegion(void);
PUBLIC void  CHGMNG_InitPseChgParam (void);
#endif

/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                        *
 **---------------------------------------------------------------------------*/
#if 0
LOCAL BOOLEAN _CHGMNG_IsNegativeLevel(void) {
    return (pulse_counter >= PULSE_POS_LEVEL_WIDTH);
}
#endif
/*****************************************************************************/
//  Description:    Charger module task entry.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_PrintLogMsg(void)
{
    //CHGMNG_PRINT:"CHGMNG:module_state.chgmng_state:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_236_112_2_17_23_3_25_387,(uint8*)"d",module_state.chgmng_state);
    //CHGMNG_PRINT:"CHGMNG:module_state.bat_statistic_vol:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_237_112_2_17_23_3_25_388,(uint8*)"d",module_state.bat_statistic_vol);
    //CHGMNG_PRINT:"CHGMNG:module_state.bat_cur_vol:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_238_112_2_17_23_3_25_389,(uint8*)"d",module_state.bat_cur_vol);
    //CHGMNG_PRINT:"CHGMNG:module_state.bat_remain_cap:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_239_112_2_17_23_3_25_390,(uint8*)"d",module_state.bat_remain_cap);
    //CHGMNG_PRINT:"CHGMNG:x0_adc:%d,y0_vol:%d,slope:%d,zoom_in:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_240_112_2_17_23_3_25_391,(uint8*)"dddd",adc_cal.x0_adc,adc_cal.y0_vol,adc_cal.slope,adc_cal.zoom_in);

//    SCI_TRACE_LOW("CHGMNG:Ctl0=0x%x, Ctl1=0x%x, status=0x%x", CHG_PHY_ChgCtl0(), CHG_PHY_ChgCtl1(), CHG_PHY_ChgStatus());

    if (CHG_PHY_IsChargerPresent())
    {
        //CHGMNG_PRINT:"CHGMNG:module_state.charging_current:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_244_112_2_17_23_3_25_392,(uint8*)"d",module_state.charging_current);
        //CHGMNG_PRINT:"CHGMNG:module_state.adp_type:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_245_112_2_17_23_3_25_393,(uint8*)"d",module_state.adp_type);
        //CHGMNG_PRINT:"CHGMNG:module_state.charging_stop_reason:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_246_112_2_17_23_3_25_394,(uint8*)"d",module_state.charging_stop_reason);
        //CHGMNG_PRINT:"CHGMNG:hw_switch_point:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_247_112_2_17_23_3_25_395,(uint8*)"d",hw_switch_point);
        //CHGMNG_PRINT:"CHGMNG:chg_end_vol:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_248_112_2_17_23_3_25_396,(uint8*)"d",chg_param.chg_end_vol);
#ifdef CHGMNG_PSE_SUPPORT
       CHGMNG_PRINT(("CHGMNG: temperature region:%d",temp_region));
       CHGMNG_PRINT(("CHGMNG: chg_end_vol:%d",pse_param[CHGMNG_AdjustTempRegion()].chg_end_vol));
       CHGMNG_PRINT(("CHGMNG: rechg_vol:%d",pse_param[CHGMNG_AdjustTempRegion()].rechg_vol));
       CHGMNG_PRINT(("CHGMNG: chg_end_current:%d",pse_chg_param.chg_end_current));
#endif
    }
    else
    {
        //CHGMNG_PRINT:"CHGMNG:module_state.bat_remain_cap:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_257_112_2_17_23_3_25_398,(uint8*)"d",module_state.bat_remain_cap);
        //CHGMNG_PRINT:"CHGMNG:dischg_param.shutdown_vol:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_258_112_2_17_23_3_25_399,(uint8*)"d",dischg_param.shutdown_vol);
        //CHGMNG_PRINT:"CHGMNG:dischg_param.deadline_vol:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_259_112_2_17_23_3_25_400,(uint8*)"d",dischg_param.deadline_vol);
        //CHGMNG_PRINT:"CHGMNG:dischg_param.warning_vol:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_260_112_2_17_23_3_25_401,(uint8*)"d",dischg_param.warning_vol);
    }
    //CHGMNG_PRINT:"CHGMNG:....................vbat_queue.pointer:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_262_112_2_17_23_3_25_402,(uint8*)"d",vbat_queue.pointer);

}

uint8 FlagCHGThreadReady = 0;
uint8 CHGMNG_ThreadReady(void)
{
    return FlagCHGThreadReady;
}

/*****************************************************************************/
//  Description:    charge task entry.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_SrvThread (uint32 argc, void *argv)
{
    CHGMNGSVR_SIG_T *    signal;

    SCI_SLEEP(1500);    //wait for some client

    if (module_state.chgmng_state == CHGMNG_IDLE)
    {
        if (CHG_PHY_IsChargerPresent())         //If we hadn't detected charger is pluged in in interrupts, this operator can ensure charging is started.
        {
            // If in calibration mode, charger should be closed at the beginning. And then, response open and close command.
            if (POWER_IsCalibrationResetMode() && RM_GetDloaderMode()) {
                CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PESUDO_PLUG_OUT_MSG, 0);
            } else {
                CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_IN_MSG, 0);
            }
        }
    }

    _CHGMNG_COUNTER_INIT(warning_counter, 0);
    DoIdle_RegisterCallback ((DOIDLE_CALLBACK_FUNC)_CHGMNG_VBatTimerHandler);

    for(;;)
    {
        signal = (CHGMNGSVR_SIG_T *) SCI_GetSignal (chg_task_id);

        SCI_TRACE_LOW("_CHGMNG_SrvThread SignalCode=%d, chgmng_state = %d", signal->SignalCode, module_state.chgmng_state);

        switch(signal->SignalCode)
        {
            case CHGMNG_CHARGER_PLUG_IN_MSG:
                module_state.adp_type = CHGMNG_ADP_STANDARD;
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG, 0);
            break;
            case CHGMNG_CHARGER_PLUG_OUT_MSG:
                module_state.adp_type = CHGMNG_ADP_UNKNOW;
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_CHARGERUNPLUG);
            break;
            case CHGMNG_CHARGER_MONITOR_MSG:
                _CHGMNG_ChargeMonitorRoutine();
            break;
            case CHGMNG_VBAT_MONITOR_MSG:
                _CHGMNG_VbatMonitorRoutine ();
            break;
#ifdef DUAL_BATTERY_SUPPORT
            case CHGMNG_MODULE_RESET_MSG:
                _CHGMNG_ModuleReset(signal->sig_param);
            break;
#endif
            case CHGMNG_CHARGER_PESUDO_PLUG_OUT_MSG:
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_PESUDO_CHARGERUNPLUG);
                CHG_PHY_SetChargeEndVolt(CHGMNG_CALI_MODE_END_VOLT);
                CHG_PHY_SetVBatOvpVolt(CHGMNG_CALI_MODE_BAT_OVP_VOLT);
                FlagCHGThreadReady = 1;
            break;
#ifdef BATTERY_DETECT_SUPPORT
            case CHGMNG_BATTERY_OFF_MSG:
                _CHGMNG_BatteryOffRoutine ();
            break;
#endif
            case CHGMNG_USB_HOST_PLUG_IN_MSG:
                CHGMNG_SetChargeAdapter(CHGMNG_ADP_USB);
            break;
            default:
                SCI_PASSERT (0,("Error msg!!!"));   /*assert verified*/
            break;
        }

        SCI_FREE(signal);
        signal = NULL;
    }
}

/*****************************************************************************/
//  Description:    This function is used to process Finite State Machine of charge module.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_E fsm_event, uint32 condition)
{
    //CHGMNG_PRINT:"CHGMNG:_CHGMNG_FSMProcess fsm_event:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_321_112_2_17_23_3_25_403,(uint8*)"d",fsm_event);

    switch(fsm_event)
    {
        case CHGMNG_FSM_EVENT_INIT:
            module_state.chgmng_state= CHGMNG_IDLE;
        break;

        case CHGMNG_FSM_EVENT_START_CHG:
            _CHGMNG_StartCharge(condition);
            module_state.chgmng_state = CHGMNG_CHARGING; ///should be changed in future
        break;

        case CHGMNG_FSM_EVENT_STOP_CHG:
            module_state.chgmng_state = CHGMNG_IDLE;
            _CHGMNG_StopCharge (condition);
        break;

        default:
            SCI_PASSERT (0,("Error CHGMNG_FSM_EVENT_E!!!"));   /*assert verified*/
        break;
    }
}

//end modify by paul for charge manage

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    vbat present
//  Author:
/*****************************************************************************/
LOCAL uint32 _CHGMNG_CheckVbatPresent (void)
{
#if 0
    uint32 old_time,new_time;

    CHG_PHY_ShutDown();

    ////wait 10ms, if vbat don't present,should poweroff by hardware
    old_time = new_time = SCI_GetTickCount();

    while ( (new_time - old_time) < 10)
    {
        new_time = SCI_GetTickCount();
    }

    CHG_PHY_TurnOn();
    CHG_PHY_SetRecharge();
#else
#ifdef ZDT_PCBA_ZW201_SUPPORT
    return 1;
#else
    uint32  batdet_ok = CHG_PHY_IsBatDetOK();

    return batdet_ok;
#endif
#endif
}
/*****************************************************************************/
//  Description:    convert adc to Vbus voltage
//  Author:         Mingwei.Zhang
//  Modify:         Tony
/*****************************************************************************/
PUBLIC uint32 _CHGMNG_AdcValueToVChgVol(uint32 adc)
{
    uint32 result;
    uint32 vchg_vol = CHGMNG_AdcvalueToVoltageforSmallScale(adc);

    result = vchg_vol * 900 / 68;
    SCI_TRACE_LOW("_CHGMNG_AdcValueToVChgVol result=%d", result);

    return result;
}

/*****************************************************************************/
//  Description:    This function is used to get the result of Vcharge ADC value.
//                  Return: the Vcharge adc value.
//  Author:         paul.luo
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetVChgVol (void)
{
    return _CHGMNG_AdcValueToVChgVol(ADC_GetResultDirectly (ADC_CHANNEL_VCHG, ADC_SCALE_1V250));
}

/*****************************************************************************/
//  Description:    This function is used to convert VProg ADC value to charge current value.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL uint16 _CHGMNG_ADCValueToCurrent(uint32 adcvalue)
{
    uint32 current_type= 300;

    uint32 voltage = (uint32)CHGMNG_AdcvalueToVoltageforSmallScale (adcvalue);

    uint32 temp;

    voltage = voltage * 100 / 48;
    if(module_state.adp_type == CHGMNG_ADP_STANDARD)
    {
        current_type = chg_param.standard_chg_current;
    }
    else if(module_state.adp_type == CHGMNG_ADP_USB)
    {
        current_type = chg_param.usb_chg_current;
    }
    else if(module_state.adp_type == CHGMNG_ADP_NONSTANDARD)
    {
        current_type = chg_param.nonstandard_chg_current;
    }
    else if(module_state.adp_type == CHGMNG_ADP_UNKNOW)
    {
        current_type = CHARGER_CURRENT_450MA;
    }

    /*
        1.internal chip voltage for ADC measure:v1---->v1 = voltage*0.241 = voltage*VOL_DIV_P1/VOL_DIV_P2
        2.current convert voltage expressions: VOL_TO_CUR_PARAM*current/curret_type = v1 = voltage*VOL_DIV_P1/VOL_DIV_P2
           ---->current = ((current_type*voltage*VOL_DIV_P1)/VOL_TO_CUR_PARAM)/VOL_DIV_P2;
    */
    temp = current_type * voltage / 1200;

    CHGMNG_PRINT(("_CHGMNG_ADCValueToCurrent current_type= %d, current:%d\n", current_type, temp));

    return (uint16)temp;

}

/*****************************************************************************/
//  Description:    This function gets the virtual Vprog value. Because the Iprog is not steady
//                  enough when it is larger than 500mA, we have to calculate its average. The
//                  sampling times perhaps will be adjusted in the eventual project.
//                  Return: the virtual result.
//  Author:         Benjamin.Wang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetVprogADCResult (void)
{
    int i,j, temp;
    int vprog_result[VPROG_RESULT_NUM];

    for (i = 0; i < VPROG_RESULT_NUM; i++)
    {
        vprog_result[i] =
            ADC_GetResultDirectly (16, ADC_SCALE_1V250);
    }

    for (j=1; j<=VPROG_RESULT_NUM-1; j++)
    {
        for (i=0; i<VPROG_RESULT_NUM -j; i++)
        {
            if (vprog_result[i] > vprog_result[i+1])
            {
                temp = vprog_result[i];
                vprog_result[i] = vprog_result[i+1];
                vprog_result[i+1] = temp;
            }
        }
    }

    return vprog_result[VPROG_RESULT_NUM/2];
}
//#define CURRENT_FROM_ISENSE
#define ADC_CHANNEL_ISENSE  1
/*****************************************************************************/
//  Description:    Get charging current
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetChgCurrent(void)
{
    uint32 progAdcResult = 0;
    uint32 chgCurrent = 0;

    progAdcResult = _CHGMNG_GetVprogADCResult();
    chgCurrent =  _CHGMNG_ADCValueToCurrent(progAdcResult);

    SCI_TRACE_LOW("_CHGMNG_GetChgCurrent:progAdcResult:%d,chgCurrent:%d\n",progAdcResult,chgCurrent);

    return chgCurrent;
}

/*****************************************************************************/
//  Description:    This function is used to get the result of VBAT ADC.
//                  Return: the VBAT value.
//  Author:         Benjamin.Wang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetVBATADCResult (void)
{
    int i,j, temp;

    int vbat_result[VBAT_RESULT_NUM];

    for (i = 0; i < VBAT_RESULT_NUM; i++)
    {
        vbat_result[i] =
            ADC_GetResultDirectly (ADC_CHANNEL_VBAT, ADC_SCALE_1V250);
        SCI_TRACE_LOW("_CHGMNG_GetVBATADCResult i:%d adc:%d", i, vbat_result[i]);
    }

    for (j=1; j<=VBAT_RESULT_NUM-1; j++)
    {
        for (i=0; i<VBAT_RESULT_NUM -j; i++)
        {
            if (vbat_result[i] > vbat_result[i+1])
            {
                temp = vbat_result[i];
                vbat_result[i] = vbat_result[i+1];
                vbat_result[i+1] = temp;
            }
        }
    }


    //CHGMNG_PRINT:"CHGMNG:_CHGMNG_GetVBATADCResult:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_533_112_2_17_23_3_26_406,(uint8*)"d",vbat_result[VBAT_RESULT_NUM/2]);
    return vbat_result[VBAT_RESULT_NUM/2];
}

/*****************************************************************************/
//  Description:    Get Vbat voltage
//  Author:         Tianyu.Yang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetVbatVol(void)
{
    uint32 vol = CHGMNG_AdcvalueToVoltage (_CHGMNG_GetVBATADCResult());
    SCI_TRACE_LOW("_CHGMNG_GetVbatVol vol:%d", vol);
    //CHGMNG_PRINT:"CHGMNG:_CHGMNG_GetVbatVol:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_542_112_2_17_23_3_26_407,(uint8*)"d",vol);
    return vol;
}

/*****************************************************************************/
//  Description:    The function initializes the Vbat queue
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_VBatQueueInit (uint32 vbat_vol,uint32 queue_len)
{
    uint32 i;

    SCI_DisableIRQ();   //must do it
    vbat_queue.sum = vbat_vol * queue_len;
    vbat_queue.pointer = 0;
    vbat_queue.queue_len = queue_len;
    SCI_RestoreIRQ();

    for (i = 0; i < queue_len ; i++)               //init vbat queue
    {
        vbat_queue.queue[i] = vbat_vol;
    }

    module_state.bat_statistic_vol = vbat_vol;
}
/*****************************************************************************/
//  Description:    update vbat queue
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_VbatQueueUpdate (uint32 vbat)
{
    SCI_DisableIRQ();   //must do it
    vbat_queue.sum += vbat;
    vbat_queue.sum -= vbat_queue.queue[vbat_queue.pointer];
    module_state.bat_statistic_vol = vbat_queue.sum/vbat_queue.queue_len;
//    SCI_TraceLow("module_state.bat_statistic_vol = %d", module_state.bat_statistic_vol);
    SCI_RestoreIRQ();

    vbat_queue.queue[vbat_queue.pointer++] = vbat;

    if (vbat_queue.pointer == vbat_queue.queue_len)
    {
        vbat_queue.pointer = 0;
    }
}
/*****************************************************************************/
//  Description:    This function return current temperature.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetCurrentTemp(void)
{
    int32 result = 0;

#ifdef CHGMNG_PSE_SUPPORT
    result =CHG_NV_TEMP_OFFSET+ CHGMNG_GetVbatTempVolValue();
//    SCI_TraceLow("_CHGMNG_GetCurrentTemp = %d", result);
#endif
    return (uint32)result;
}
/*****************************************************************************/
//  Description:    Update Statistic Temperature.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_TempQueueInit(uint32 temp)
{
#ifdef CHGMNG_PSE_SUPPORT
    uint32 i;

    temp_queue.sum = temp * CHG_TEMP_BUFF_SIZE;
    temp_queue.pointer = 0;

    for (i = 0; i < CHG_TEMP_BUFF_SIZE ; i++)
    {
        temp_queue.queue[i] = temp;
    }
    module_state.charging_temperature = temp;
#endif
}
/*****************************************************************************/
//  Description:    Update Statistic Temperature.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_TempQueueUpdate(uint32 temp)
{
#ifdef CHGMNG_PSE_SUPPORT
    temp_queue.sum += temp;
    temp_queue.sum -= temp_queue.queue[temp_queue.pointer];
    module_state.charging_temperature = temp_queue.sum/CHG_TEMP_BUFF_SIZE;
//    SCI_TraceLow("_CHGMNG_TempQueueUpdate = %d", module_state.charging_temperature);

    temp_queue.queue[temp_queue.pointer++] = temp;

    if (temp_queue.pointer == CHG_TEMP_BUFF_SIZE)
    {
        temp_queue.pointer = 0;
    }
#endif
}


/*****************************************************************************/
//  Description:    Charge current queue init
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_CurrentQueueInit (uint32 current)
{
    uint32 i;

    current_queue.sum = current * CHG_CURRENT_BUFF_SIZE;
    current_queue.pointer = 0;

    for (i = 0; i < CHG_CURRENT_BUFF_SIZE ; i++)
    {
        current_queue.queue[i] = current;
    }
    module_state.charging_current = current;
}
/*****************************************************************************/
//  Description:    update charge current queue
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_CurrentQueueUpdate (uint32 current)
{
    current_queue.sum += current;
    current_queue.sum -= current_queue.queue[current_queue.pointer];
    module_state.charging_current = current_queue.sum/CHG_CURRENT_BUFF_SIZE;
//    SCI_TraceLow("_CHGMNG_CurrentQueueUpdate = %d", module_state.charging_current);

    current_queue.queue[current_queue.pointer++] = current;

    if (current_queue.pointer == CHG_CURRENT_BUFF_SIZE)
    {
        current_queue.pointer = 0;
    }
}

/*****************************************************************************/
//  Description:    Message send function.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHGMNG_SendMessagetoClient (CHR_SVR_MSG_SERVICE_E  msg, uint32 param)
{
    static uint32 msg_count = 1;

    if (CHR_MSG_MAX_NUM == recent_message[0])
    {
        int32 i = 1;

        while (i < 10)
        {
            if ( (recent_message[i] != 0))
            {
                if (SCI_SUCCESS != SCI_SendEventToClient (CHR_SERVICE, recent_message[i], 0))
                {
                    if ( (msg != CHR_CAP_IND) && (msg != CHR_WARNING_IND))
                    {
                        recent_message[msg_count++] = msg;

                        if (10 == msg_count)
                        {
                            msg_count = 1;
                        }
                    }

                    return;
                }
                else
                {
                    //CHGMNG_PRINT:"CHGMNG:%d has been send000!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_699_112_2_17_23_3_26_408,(uint8*)"d", recent_message[i]);
                    recent_message[i] = 0;
                }
            }

            i++;
        }

        msg_count = 1;
        recent_message[0] = 0;
    }

    // send to client.
    // The percent is valid only when msg == CHR_CAP_IND
    //CHGMNG_PRINT:"CHGMNG:%d has been send2222!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_713_112_2_17_23_3_26_409,(uint8*)"d", msg);

    if (SCI_SUCCESS != SCI_SendEventToClient (CHR_SERVICE, (uint32) msg, (void *) param))
    {
        //CHGMNG_PRINT:"CHGMNG:%d has been saved111!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_717_112_2_17_23_3_26_410,(uint8*)"d", msg);

        if ( (msg != CHR_CAP_IND) && (msg != CHR_WARNING_IND))
        {
            recent_message[0] = CHR_MSG_MAX_NUM;        //This is used to indicate that we have had messages to handle.
            recent_message[msg_count++] = msg;

            if (10 == msg_count)
            {
                msg_count = 1;
            }
        }
    }
}

/*****************************************************************************/
//  Description:    Convert ADCVoltage to percentrum.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
uint32 _CHGMNG_VoltagetoPercentum (uint32 voltage, CHGMNG_STATE_E  is_charging, BOOLEAN update)
{
    uint16 percentum;
    int32 temp = 0;
    int pos = 0;
    static uint16 pre_percentum = 0xffff;
    static BOOLEAN init_chg_cap = SCI_FALSE;

    if(update)
    {
        pre_percentum = 0xffff;
        return 0;
    }

    if (((is_charging == CHGMNG_CHARGING) || (is_charging == CHGMNG_PULSECHARGING)) && (init_chg_cap == SCI_FALSE))
    {
        if(pre_percentum == 0xffff)
        {
            CHG_PHY_ShutDown();
            init_chg_cap = SCI_TRUE;
            return PROD_Capacity_Get_NV();
        }

        for (pos = 0; pos < BAT_CAPACITY_STEP -1; pos++)
        {
            if(voltage > chg_bat_capacity_table[pos][0])
                break;
        }

        if (pos == 0)
        {
            percentum = 100;
        }
        else
        {
            temp = chg_bat_capacity_table[pos][1]-chg_bat_capacity_table[pos-1][1];
            temp = temp*(int32)(voltage - chg_bat_capacity_table[pos][0]);
            temp = temp/(chg_bat_capacity_table[pos][0] - chg_bat_capacity_table[pos-1][0]);
            temp = temp + chg_bat_capacity_table[pos][1];
            if (temp < 0)
            {
                temp = module_state.bat_remain_cap;
            }
            percentum = temp;
        }

        if (pre_percentum > percentum)
        {
            percentum = pre_percentum;
        }
        else
        {
            pre_percentum = percentum;
        }
        if(percentum > BAT_CAP_ALMOST_FULL)
            percentum = BAT_CAP_ALMOST_FULL;

    }
    else
    {
        if(init_chg_cap == SCI_TRUE)
        {
            voltage = _CHGMNG_GetVbatVol();
            _CHGMNG_VBatQueueInit(voltage,DISCHG_VBAT_BUFF_SIZE);
            voltage = module_state.bat_statistic_vol;
            if((is_charging == CHGMNG_CHARGING) || (is_charging == CHGMNG_PULSECHARGING))
                CHG_PHY_TurnOn();
            init_chg_cap = SCI_FALSE;
        }
        else if(pre_percentum == 0xffff)
        {
            _CHGMNG_VBatQueueInit(module_state.bat_statistic_vol,DISCHG_VBAT_BUFF_SIZE);
            voltage = module_state.bat_statistic_vol;
        }

        if(is_charging == CHGMNG_STOPPING &&
        (module_state.charging_stop_reason == CHGMNG_VBATEND ||
        module_state.charging_stop_reason == CHGMNG_CHARGEDONE))
        {
            percentum = 100;
            pre_percentum = percentum;
            return percentum;
        }

        for (pos = 0; pos < BAT_CAPACITY_STEP -1; pos++)
        {
            if (voltage > dischg_bat_capacity_table[pos][0])
            break;
        }

        if (pos == 0)
        {
            percentum = 100;
        }
        else
        {
            temp = dischg_bat_capacity_table[pos][1]-dischg_bat_capacity_table[pos-1][1];
            temp = temp*(int32)(voltage - dischg_bat_capacity_table[pos][0]);
            temp = temp/(dischg_bat_capacity_table[pos][0] - dischg_bat_capacity_table[pos-1][0]);
            temp = temp + dischg_bat_capacity_table[pos][1];
            if (temp < 0)
            {
                temp = module_state.bat_remain_cap;
            }
            percentum = temp;
        }

        if (pre_percentum == 0xffff)
        {
            pre_percentum = percentum;
        }
        else if (pre_percentum < percentum)
        {
            percentum = pre_percentum;
        }
        else
        {
            pre_percentum = percentum;
        }
    }

    return percentum;
}

/*****************************************************************************/
//  Description:    Get phone state(busy or no busy).
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 _CHGMNG_GetPhoneState (void)
{
    /*Improve later.*/
    return 0;
}

#ifdef BATTERY_DETECT_SUPPORT
LOCAL uint32 _CHGMNG_BatteryOffRoutine()
{
    MCU_MODE_E reset_mode = POWER_GetResetMode();
    if (reset_mode == NORMAL_MODE) {
        _CHGMNG_SendMessagetoClient (CHR_BATTERY_OFF_IND, 0);

        if (recent_message[0] == 0) { ///msg is send
            isshutting_down = SCI_TRUE;
        }
    }

    return 0;
}
#endif

/*****************************************************************************/
//  Description:    The function monitor vbat status,Be called by DoIdle_Callback.
//  Author:         Mingwei.Zhang
//    Note:
/*****************************************************************************/
LOCAL uint32 _CHGMNG_VBatTimerHandler (uint32 param)
{
    CHGMNG_SendMsgToChgTask(CHGMNG_VBAT_MONITOR_MSG, 0);

    return 0;
}

/*****************************************************************************/
//  Description:    The function calculates the vbat every 2 seconds.
//  Author:         Benjamin.Wang
//    Note:
/*****************************************************************************/
LOCAL void _CHGMNG_VbatMonitorRoutine ()
{
#if defined(CHGMNG_PSE_SUPPORT)
    POWER_RESTART_CONDITION_E   restart_condition = POWER_GetRestartCondition();
    SCI_TRACE_LOW("_CHGMNG_VbatMonitorRoutine_restart_condition=%d",restart_condition);
#endif
    CHIP_Enable7sRstMode_ALL();
    /*If we had inform the upper layer to shutdown, we will not send any other messages
    because too many messages can block the message queue.*/
    if (isshutting_down)
    {
//        SCI_TRACE_LOW("_CHGMNG_VbatMonitorRoutine is shutdown");
        return;
    }

    if ( _CHGMNG_CheckVbatPresent() == 0)
    {
        SCI_TRACE_LOW("_CHGMNG_VbatMonitorRoutine NO BAT");
        return;
    }

#if defined(CHGMNG_PSE_SUPPORT)
    _CHGMNG_TempQueueUpdate(_CHGMNG_GetCurrentTemp());
    SCI_TraceLow("_CHGMNG_VbatMonitorRoutine_module_state.charging_temperature = %d", module_state.charging_temperature);
    if( module_state.charging_temperature >= HOT_TEMP)
    {
        if(RESTART_BY_CHARGE == restart_condition)
        {
            if(module_state.charging_stop_reason != CHGMNG_OVERTEMP)
            {
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_OVERTEMP);
            }
            return;
        }
        else
        {
            SCI_TraceLow("_CHGMNG_VbatMonitorRoutine_POWER_PowerOff");
            POWER_PowerOff();
        }
    }
#endif

    if (module_state.chgmng_state == CHGMNG_IDLE) {
        if (count_paging >= 3)
        {
            count_paging = 0;
            SCI_TRACE_LOW("_CHGMNG_VbatMonitorRoutine count_paging run!");
        }
        else
        {
            count_paging++;
            SCI_TRACE_LOW("_CHGMNG_VbatMonitorRoutine count_paging jump");
            return;
        }
        _CHGMNG_PrintLogMsg();

        module_state.bat_cur_vol = _CHGMNG_GetVbatVol();
        _CHGMNG_VbatQueueUpdate (module_state.bat_cur_vol);

        if (module_state.bat_statistic_vol < dischg_param.warning_vol) {
            if (0 == warning_counter) {
                //CHGMNG_PRINT:"CHGMNG:CHR_WARNING_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_831_112_2_17_23_3_26_411,(uint8*)"");
                _CHGMNG_SendMessagetoClient (CHR_WARNING_IND, 0);
            }
            _CHGMNG_COUNTER_ADD_AND_AUTO_ROUND(warning_counter, dischg_param.warning_count);

            if ((module_state.bat_statistic_vol < dischg_param.shutdown_vol) ||
                (module_state.bat_cur_vol < dischg_param.deadline_vol)) {
                if (0 == warning_counter) {
                    //CHGMNG_PRINT:"CHGMNG:CHR_WARNING_IND Before shutdown!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_847_112_2_17_23_3_26_412,(uint8*)"");
                    _CHGMNG_SendMessagetoClient (CHR_WARNING_IND, 0);
                    _CHGMNG_COUNTER_ADD(warning_counter);
                } else {
                    //CHGMNG_PRINT:"CHGMNG:CHR_SHUTDOWN_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
                    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_853_112_2_17_23_3_26_413,(uint8*)"");
                    _CHGMNG_SendMessagetoClient (CHR_SHUTDOWN_IND, 0);

                    if (recent_message[0] == 0) { ///msg is send
                        isshutting_down = SCI_TRUE;
                        return ;
                    }
                }
            }
        } else {
            _CHGMNG_COUNTER_INIT(warning_counter, 0);
        }
    }

    module_state.bat_remain_cap = _CHGMNG_VoltagetoPercentum(
        module_state.bat_statistic_vol,
        module_state.chgmng_state, SCI_FALSE);
    _CHGMNG_SendMessagetoClient (
        CHR_CAP_IND, module_state.bat_remain_cap);
}


LOCAL uint32 CHGMNG_SoftCheckIsChargerOverVol(uint32 chgVol)
{
    if (chgVol >= ovp_param.ovp_over_vol)
    {
        ovp_detect_counter ++;
    }
    else
    {
        ovp_detect_counter = 0;
    }

    if (ovp_detect_counter >= OVP_DETECT_VALID_TIMES)
    {
        ovp_detect_counter = OVP_DETECT_VALID_TIMES;

        return TRUE;
    }

    return FALSE;
}

LOCAL uint32 CHGMNG_SoftCheckIsBatteryOverVol(uint32 batChargingVol)
{
    if (batChargingVol >= (chg_param.chg_end_vol + 30))
    {
        ovp_detect_bat_cnt ++;
    }
    else
    {
        ovp_detect_bat_cnt = 0;
    }

    if (ovp_detect_bat_cnt >= OVP_DETECT_VALID_TIMES)
    {
        ovp_detect_bat_cnt = OVP_DETECT_VALID_TIMES;

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description:    timeout function of charge timer
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_ChargeTimerHandler (uint32 state)
{
    CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_MONITOR_MSG,0);
}

#if 0
/**
 @details     : ��ΪOSTIMER˯�ߺ󲻹������ṩһ����ȡ��綨ʱ����timer��Ϣ�ӿڣ�
                ˯�ߺ󽫸ö�ʱ��ʣ��ʱ��copy��һ��sleep���ܹ����Ķ�ʱ���У��Ա�֤������������
 @param       : void
 @return      : uint32 ʣ��tick��
*/
PUBLIC uint32 CHGMNG_GetChargeTimerInfo(void)
{
    uint32          status          = 0;
    BOOLEAN         active          = 0;
    uint32          remaining_time  = 0;
    uint32          reschedule_time    = 0;
    char            name[SCI_MAX_NAME_SIZE];

    if (NULL == g_charging_timer)
    {
        return 0;
    }

    status = SCI_GetTimerInfoTick(g_charging_timer, name, &active, &remaining_time, &reschedule_time);

    SCI_ASSERT(SCI_SUCCESS == status);  /*assert verified*/

    if ( TRUE == active )
    {
        return remaining_time;
    }

    return 0;

}
#endif


/*****************************************************************************/
//  Description:    When charger connect start charger monitor,Be called by charger task.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_ChargeMonitorRoutine(void)
{
    uint32 chgVol = 0;
    uint32 isChgOVP = FALSE;

    uint32 batVol = 0;
    uint32 isBatOVP = FALSE;

    _CHGMNG_PrintLogMsg();

    // switch positive and negative level to charge
    if ( 1 == g_charge_plus )
    {
        SCI_ChangeTimer (g_charging_timer, _CHGMNG_ChargeTimerHandler, CHARGING_OFF_TIMER_INTERVAL);
        if (SCI_SUCCESS != SCI_ActiveTimer (g_charging_timer))
        {
            //CHGMNG_PRINT:"CHGMNG:Invalid application timer pointer!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1167_112_2_17_23_3_27_425,(uint8*)"");
        }

        /* ostimer2 sleep�󲻹�����֪ͨsleepģ�齫chg timer ���ݵ���һ����ʱ��*/
        SLEEP_SetChgOsw2Timer(CHARGING_OFF_TIMER_INTERVAL+100);

        g_charge_plus = 0;
    }
    else
    {
        SCI_ChangeTimer (g_charging_timer, _CHGMNG_ChargeTimerHandler, CHARGING_ON_TIMER_INTERVAL);
        if (SCI_SUCCESS != SCI_ActiveTimer (g_charging_timer))
        {
            //CHGMNG_PRINT:"CHGMNG:Invalid application timer pointer!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1167_112_2_17_23_3_27_425,(uint8*)"");
        }

        /* ostimer2 sleep�󲻹�����֪ͨsleepģ�齫chg timer ���ݵ���һ����ʱ��*/
        SLEEP_SetChgOsw2Timer(CHARGING_ON_TIMER_INTERVAL+100);

        g_charge_plus = 1;
    }
#if 0//defined(CHGMNG_PSE_SUPPORT)
    if( module_state.charging_temperature >= HOT_TEMP)
    {
        if(RESTART_BY_CHARGE == restart_condition)
        {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_OVERTEMP);
            return;
        }
        else
        {
            POWER_PowerOff();
        }
    }
#endif

    if ( _CHGMNG_CheckVbatPresent() == 0)
    {
        SCI_TRACE_LOW("_CHGMNG_StartCharge NO BAT");

        _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_VBATEND);
        return;
    }

    chgVol = _CHGMNG_GetVChgVol();
    isChgOVP = CHGMNG_SoftCheckIsChargerOverVol(chgVol);

    batVol = _CHGMNG_GetVbatVol();
    isBatOVP = CHGMNG_SoftCheckIsBatteryOverVol(batVol);

    SCI_TRACE_LOW("_CHGMNG_ChargeMonitorRoutine chgVol=%d, rechg_vol=%d, ovp_resume_vol=%d, isChgOVP=%d, isBatOVP=%d",
        chgVol, chg_param.rechg_vol, ovp_param.ovp_resume_vol, isChgOVP, isBatOVP);

    // check if over voltage
#ifdef CHGMNG_PSE_SUPPORT
    if (CHG_PHY_IsVBatOv()
     || CHG_PHY_IsVChgOv()
     || (isChgOVP == TRUE)
     || (isBatOVP== TRUE))
    {
        if ((module_state.charging_stop_reason != CHGMNG_OVERVOLTAGE)
            &&(module_state.charging_stop_reason != CHGMNG_OVERTEMP))
        {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_OVERVOLTAGE);
            return ;
        }
        return ;
    }
    else
    {
        if (module_state.charging_stop_reason == CHGMNG_OVERTEMP)
        {
            if( module_state.charging_temperature >= HOT_TEMP)
            {
                 return;
            }
            if((CHGMNG_AdjustTempRegion() != PSE_BATTERY_TEMP_REGION__HOT) && (CHGMNG_AdjustTempRegion() != PSE_BATTERY_TEMP_REGION__COLD))
            {
                if (module_state.bat_statistic_vol < pse_param[CHGMNG_AdjustTempRegion()].rechg_vol)
                {
                    _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG, 0);
                }
            }
            return;
        }
        if (module_state.charging_stop_reason == CHGMNG_OVERVOLTAGE)
        {
            if (chgVol < ovp_param.ovp_resume_vol)
            {
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG, 0);
            }
            return;
        }
    }

#else
    if (CHG_PHY_IsVBatOv()
     || CHG_PHY_IsVChgOv()
     || (isChgOVP == TRUE)
     || (isBatOVP== TRUE)) {
        if (module_state.charging_stop_reason != CHGMNG_OVERVOLTAGE) {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_OVERVOLTAGE);
            return ;
        }
        return ;
    } else {
        if (module_state.charging_stop_reason == CHGMNG_OVERVOLTAGE) {
            if (chgVol < ovp_param.ovp_resume_vol)
            {
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG, 0);
            }

            return ;
        }
    }
#endif


    SCI_TRACE_LOW("_CHGMNG_ChargeMonitorRoutine chgmng_state=%d, g_charge_plus =%d", module_state.chgmng_state, g_charge_plus);

    /*if (module_state.chgmng_state == CHGMNG_IDLE) {

        // check if need to recharge
        if (module_state.bat_statistic_vol <= chg_param.rechg_vol) {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG, 0);
        }

        return ;
    }*/
    if (module_state.chgmng_state == CHGMNG_IDLE) {
        // check if need to recharge
#if defined(CHGMNG_PSE_SUPPORT)
        if (module_state.bat_statistic_vol <= pse_param[CHGMNG_AdjustTempRegion()].rechg_vol) {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG,CHGMNG_RECHARGE);
        }
#else
        if (module_state.bat_statistic_vol <= chg_param.rechg_vol) {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_START_CHG,CHGMNG_RECHARGE);
        }
#endif
        return ;
    }

    // switch positive and negative level to charge
    if ( 0 == g_charge_plus )
    {
       _CHGMNG_CurrentQueueUpdate(_CHGMNG_GetChgCurrent());

        CHG_PHY_ShutDown();
    }
    else
    {
    //        module_state.bat_cur_vol = _CHGMNG_GetVbatVol();
        module_state.bat_cur_vol = batVol;
        _CHGMNG_VbatQueueUpdate (module_state.bat_cur_vol);

        /* ��ͣ1s���Ҽ���ѹ�����´򿪳�磬��ʱ���л�Ϊ9s */
        CHG_PHY_TurnOn();
    }

#if defined(CHGMNG_PSE_SUPPORT)
    if (module_state.bat_statistic_vol <= CHGMNG_POWERON_VAL)
    {
        CHG_PHY_SetChgCurrent (CHARGER_CURRENT_450MA);
        CHG_PHY_TurnOn();
        return ;
    }

    //check temperature
    if(CHGMNG_AdjustTempRegion() == PSE_BATTERY_TEMP_REGION__HOT || CHGMNG_AdjustTempRegion() == PSE_BATTERY_TEMP_REGION__COLD)
    {
        if(module_state.charging_stop_reason != CHGMNG_OVERTEMP)
        {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_OVERTEMP);
        }
        return;
    }
//    module_state.adp_type = CHGMNG_ADP_STANDARD;
    _CHGMNG_SetChargeCurrent(module_state.adp_type);
// adjust cc-cv point
    if((hw_switch_point != pse_param[CHGMNG_AdjustTempRegion()].switch_point)&&(module_state.bat_statistic_vol < pse_param[CHGMNG_AdjustTempRegion()].chg_end_vol))
    {
        hw_switch_point = pse_param[CHGMNG_AdjustTempRegion()].switch_point;
        CHG_PHY_SetSwitchoverPoint (hw_switch_point);
        //CHGMNG_PRINT:"CHGMNG:UpdateSwitchoverPoint!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1039_112_2_17_23_3_27_419,(uint8*)"");
        _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol,CHG_VBAT_BUFF_SIZE);
    }

/*  if (module_state.bat_statistic_vol < pse_param[CHGMNG_AdjustTempRegion()].chg_end_vol)
    {
        if(hw_switch_point != pse_param[CHGMNG_AdjustTempRegion()].switch_point)
        {
            hw_switch_point = pse_param[CHGMNG_AdjustTempRegion()].switch_point;
            CHG_PHY_SetSwitchoverPoint (hw_switch_point);
            //CHGMNG_PRINT:"CHGMNG:UpdateSwitchoverPoint!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1039_112_2_17_23_3_27_419,(uint8*)"");
            _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol,CHG_VBAT_BUFF_SIZE);
        }
   if(module_state.charging_current < CHGMNG_SWITCH_CV_VPROG)    //CC-CVת��������С�ˣ���������
        {
            hw_switch_point = (CHG_SWITPOINT_E) CHG_PHY_UpdateSwitchoverPoint (TRUE);
            pse_param[CHGMNG_AdjustTempRegion()].switch_point = hw_switch_point;
            //CHGMNG_PRINT:"CHGMNG:UpdateSwitchoverPoint!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1039_112_2_17_23_3_27_419,(uint8*)"");

            _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol,CHG_VBAT_BUFF_SIZE);
        }
    }
    else
    {
        if (module_state.charging_current  < pse_chg_param.chg_end_current)
        {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG,CHGMNG_CHARGEDONE);
            return;
        }
    }*/
#endif
#if 0
    _CHGMNG_COUNTER_ADD_AND_AUTO_ROUND(pulse_counter, PULSE_PERIOD);
    if (_CHGMNG_IsNegativeLevel()) {
       _CHGMNG_CurrentQueueUpdate(_CHGMNG_GetChgCurrent());

        CHG_PHY_ShutDown();

//        SCI_TRACE_LOW("_CHGMNG_ChargeMonitorRoutine stop---- ctr0=0x%x", CHG_PHY_ChgCtl0());
    } else {
        if (pulse_counter == 0) {
            // we can read accurate battery voltage at this time
            // since charger has been shut down after a while
            module_state.bat_cur_vol = _CHGMNG_GetVbatVol();
            _CHGMNG_VbatQueueUpdate (module_state.bat_cur_vol);

            CHG_PHY_TurnOn();

//            SCI_TRACE_LOW("_CHGMNG_ChargeMonitorRoutine start------ctr0=0x%x", CHG_PHY_ChgCtl0());
        }
    }
#endif

#if defined(CHGMNG_PSE_SUPPORT)
    /* cv-status �ڹ��ĸ�ʱ���ܽ�ֹ��磬�ĳ�����жϽ�ֹ��� */
    if (module_state.bat_statistic_vol >= (pse_param[CHGMNG_AdjustTempRegion()].chg_end_vol - 30))//4.18v
    {
        _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_CHARGEDONE);
        return ;
    }
#else
    /* cv-status �ڹ��ĸ�ʱ���ܽ�ֹ��磬�ĳ�����жϽ�ֹ��� */
    if (module_state.bat_statistic_vol >= (chg_param.chg_end_vol -30))//4.2v
    {
        _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_CHARGEDONE);
        return ;
    }
#endif


#if defined(PRODUCT_CONFIG_uis8910ff_refphone)
    // check cv-status
    SCI_TraceLow("CHG_PHY_IsChargerCvStatus() = %d, cv_status_counter=%d", CHG_PHY_IsChargerCvStatus(), cv_status_counter);
    if (CHG_PHY_IsChargerCvStatus()) {
        _CHGMNG_COUNTER_ADD_AND_AUTO_ROUND(cv_status_counter, PULSE_PERIOD);
        if (cv_status_counter == 0) {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_CHARGEDONE);
            return ;
        }
    }
#endif

    // check total charger time
    if ( 0 == g_charge_plus )
    {
        if (_CHGMNG_GetPhoneState() != BUSYSTATE) {
            _CHGMNG_COUNTER_DEC(charge_endtime_counter);
            if (charge_endtime_counter == 0) {
                _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_TIMEOUT);
                return ;
            }
        }
    }

}

/*****************************************************************************/
//  Description:    Enter Charge start state.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
LOCAL void _CHGMNG_StartCharge (CHGMNG_CHG_START_TYPE_E start_type)
{
    //to detect charge unplug.
    //CHGMNG_PRINT:"CHGMNG:CHGMNG_ChargeStartHandler!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1134_112_2_17_23_3_27_422,(uint8*)"");

	if ( _CHGMNG_CheckVbatPresent() == 0)
	{
		CHG_PHY_ShutDown();

		SCI_TRACE_LOW("_CHGMNG_StartCharge NO BAT");
		return;
	}

	_CHGMNG_VBatQueueInit(module_state.bat_statistic_vol, CHG_VBAT_BUFF_SIZE);

#if 0//CHGMNG_PSE_SUPPORT
     _CHGMNG_TempQueueInit(_CHGMNG_GetCurrentTemp());
    if( module_state.charging_temperature >= HOT_TEMP)
    {
        if(RESTART_BY_CHARGE == POWER_GetRestartCondition())
        {
            _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_STOP_CHG, CHGMNG_OVERTEMP);
        }
        else
        {
            POWER_PowerOff();
        }
    }
#endif
//    SCI_TRACE_LOW("module_state.adp_type = %d",module_state.adp_type);
	_CHGMNG_SetChargeCurrent (module_state.adp_type);
	CHG_PHY_SetChargeEndCurrent(CHGMNG_STOP_VPROG);
	_CHGMNG_CurrentQueueInit (_CHGMNG_GetChgCurrent());

	CHG_PHY_TurnOn();       //Ensure that charge module can be turned on always.

	if (start_type != CHGMNG_RECHARGE && _CHGMNG_CheckVbatPresent()) 
	{
		/*If it is not a restart recharging, send start message.*/
		//CHGMNG_PRINT:"CHGMNG:CHR_CHARGE_START_IND!!!!!!!!!!!!!!!!!!!!!!!!!!!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1145_112_2_17_23_3_27_423,(uint8*)"");

		_CHGMNG_SendMessagetoClient (CHR_CHARGE_START_IND, 0);
	}

	if (!SCI_IsTimerActive(g_charging_timer)) 
	{
		SCI_ChangeTimer (g_charging_timer, _CHGMNG_ChargeTimerHandler, CHARGING_ON_TIMER_INTERVAL);
		if (SCI_SUCCESS != SCI_ActiveTimer (g_charging_timer))
		{
			//CHGMNG_PRINT:"CHGMNG:Invalid application timer pointer!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1167_112_2_17_23_3_27_425,(uint8*)"");

			g_charge_plus = 0;
		}
		/* ostimer2 sleep�󲻹�����֪ͨsleepģ�齫chg timer ���ݵ���һ����ʱ��*/
		SLEEP_SetChgOsw2Timer(CHARGING_ON_TIMER_INTERVAL+100);
		g_charge_plus = 1;
	}

    	module_state.charging_stop_reason = CHGMNG_INVALIDREASON;

#ifdef CHGMNG_PSE_SUPPORT
        charge_endtime_counter = (pse_chg_param.chg_timeout* 1000)/(CHARGING_ON_TIMER_INTERVAL+CHARGING_OFF_TIMER_INTERVAL);     //set end time
#else
        charge_endtime_counter = (chg_param.chg_timeout * 1000) / (CHARGING_ON_TIMER_INTERVAL+CHARGING_OFF_TIMER_INTERVAL);     //set end time
#endif

	cv_status_counter = 0;
	ovp_detect_counter = 0;
	ovp_detect_bat_cnt = 0;
#ifdef CHGMNG_PSE_SUPPORT
    	_CHGMNG_COUNTER_INIT(charge_endtime_counter,
        (pse_chg_param.chg_timeout * 1000) / (CHARGING_ON_TIMER_INTERVAL+CHARGING_OFF_TIMER_INTERVAL));
#else
     	_CHGMNG_COUNTER_INIT(charge_endtime_counter,
        (chg_param.chg_timeout * 1000) / (CHARGING_ON_TIMER_INTERVAL+CHARGING_OFF_TIMER_INTERVAL));
#endif
    	_CHGMNG_COUNTER_INIT(cv_status_counter, 0);
}


/*****************************************************************************/
//  Description:    Stop charge.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_StopCharge (CHGMNG_STOPREASON_E reason)
{
    module_state.charging_stop_reason = reason;
    if (CHGMNG_CHARGERUNPLUG == reason) {
        module_state.adp_type = CHGMNG_ADP_UNKNOW;
    }

    CHG_PHY_ShutDown();
    _CHGMNG_VBatQueueInit (module_state.bat_statistic_vol, DISCHG_VBAT_BUFF_SIZE);
    _CHGMNG_COUNTER_INIT(warning_counter, 0);
    _CHGMNG_CurrentQueueInit (0);

#if 0
    /* TIMER2 ��sleep�󲻹��� */
    SCI_CHR_EnableDeepSleep(1);
#endif

    switch (reason)
    {
        case CHGMNG_CHARGERUNPLUG:
        case CHGMNG_PESUDO_CHARGERUNPLUG:
            if (SCI_SUCCESS != SCI_DeactiveTimer(g_charging_timer))
            {
                    //CHGMNG_PRINT:"CHGMNG:Invalid application timer pointer!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1200_112_2_17_23_3_27_426,(uint8*)"");
            }

            //CHGMNG_PRINT:"CHGMNG:CHR_CHARGE_DISCONNECT1!!!!!!!!!!!!!!!!!!!!!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1213_112_2_17_23_3_27_428,(uint8*)"");
            _CHGMNG_SendMessagetoClient (CHR_CHARGE_DISCONNECT, 0);
            break;

        case CHGMNG_TIMEOUT:
        case CHGMNG_VBATEND:
        case CHGMNG_CHARGEDONE:
            //CHGMNG_PRINT:"CHGMNG:CHR_CHARGE_FINISH!!!!!!!!!!!!!!!!!!!!!!!!!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1220_112_2_17_23_3_27_429,(uint8*)"");
            _CHGMNG_SendMessagetoClient (CHR_CHARGE_FINISH, 0);
            break;

        case CHGMNG_OVERVOLTAGE:
        case CHGMNG_OVERTEMP:
            _CHGMNG_SendMessagetoClient (CHR_CHARGE_FAULT, 0); //add by paul
            break;
        default:
            break;
    }
}

/*****************************************************************************/
//  Description:    This function is used to set charge current and charger mode.
//  Author:         Benjamin.Wang
/*****************************************************************************/
LOCAL void _CHGMNG_SetChargeCurrent (CHGMNG_ADAPTER_TYPE_E mode)
{
    switch(mode)
    {
        case CHGMNG_ADP_STANDARD:
#ifdef CHGMNG_PSE_SUPPORT
            CHG_PHY_SetChgCurrent(pse_param[CHGMNG_AdjustTempRegion()].chg_current);
#else
            CHG_PHY_SetChgCurrent (chg_param.standard_chg_current);
#endif
        break;
        case CHGMNG_ADP_NONSTANDARD:
#ifdef CHGMNG_PSE_SUPPORT
            CHG_PHY_SetChgCurrent(pse_param[CHGMNG_AdjustTempRegion()].chg_current);
#else
            CHG_PHY_SetChgCurrent (chg_param.nonstandard_chg_current);
#endif
        break;
        case CHGMNG_ADP_USB:
            CHG_PHY_SetChgCurrent (chg_param.usb_chg_current);
        break;
        case CHGMNG_ADP_UNKNOW:
        default:
            CHG_PHY_SetChgCurrent (CHARGER_CURRENT_450MA);
    }
}
/*****************************************************************************/
//  Description:    Default callback function. Called if client has no callback function..
//                      This is a dummy function.
//  Author:         Benjamin.Wang
/*****************************************************************************/
LOCAL void _CHGMNG_SrvDefault (BLOCK_ID id, uint32 argc, void *argv)
{
}

/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Definitions                           *
 **---------------------------------------------------------------------------*/
PUBLIC void  _CHGMNG_EIC_Init (void)
{
    ANA_REG_AND(ANA_CHGR_STATUS_RTC, ~ANA_CHGR_INT_EN);
    ANA_REG_OR(ANA_CHGR_STATUS_RTC, ANA_DCP_SWITCH_EN);
}
/*****************************************************************************/
//  Description:    This function initialize the Charge manager. Before starting charge
//                      manager, you'd better set the essential parameters according to your need.
//                      Otherwise, it will use the default parameters.
//  Return:
//  Author:         Benjamin.Wang
//  Note:           This module should be initialized after GPIO_Init.
/*****************************************************************************/
PUBLIC uint32  CHGMNG_Init (void)
{
    // create client list.
    SCI_CreateClientList (CHR_SERVICE, (CHR_MSG_MAX_NUM & 0x0ff),
                          (REG_CALLBACK) _CHGMNG_SrvDefault);

    CHG_PHY_Init();     //Charger control initial setup

    CHG_PHY_SetSwitchoverPoint (hw_switch_point);   //Set hardware cc-cv switch point
    CHG_PHY_SetChargeEndVolt(chg_param.chg_end_vol);
#ifdef CHGMNG_PSE_SUPPORT
    CHGMNG_InitPseChgParam();
    _CHGMNG_TempQueueInit(_CHGMNG_GetCurrentTemp());
#endif
    CHG_PHY_SetChgCurrent(CHARGER_CURRENT_450MA);   // Set default current
    CHG_PHY_SetVBatOvpVolt(chg_param.bat_safety_vol);
    CHG_PHY_SetVChgOvpVolt(ovp_param.ovp_over_vol); // soft control

    /*Registe timers*/
    if (NULL == g_charging_timer)    //stop state timer
    {
        /* ��9s��ʱ�����Ϊ1s�� �������Զ�ʱ�� */
        g_charging_timer = SCI_CreateTimer ("CHGTIMER",
                           _CHGMNG_ChargeTimerHandler,
                           module_state.chgmng_state,
                           CHARGING_ON_TIMER_INTERVAL,
                           SCI_NO_ACTIVATE);
    }
    else
    {
        SCI_PASSERT (SCI_FALSE, ("CHGMNG:Stop timer has been created!"));/*assert verified*/
    }

    _CHGMNG_FSMProcess(CHGMNG_FSM_EVENT_INIT,0);

    chg_task_id  = SCI_CreateThread("CHGMNG_SRV",
                                    "Q_CHGMNG_SRV",
                                    _CHGMNG_SrvThread ,
                                    0,
                                    0,
                                    CHGMNG_STACK_SIZE ,
                                    CHGMNG_QUEUE_NUM,
                                    TASK_CHGMNG_SRV_PRI ,
                                    SCI_PREEMPT,
                                    SCI_AUTO_START);

    if (0 == _CHGMNG_CheckVbatPresent())
    {
        CHG_PHY_ShutDown();
    }

    module_state.bat_cur_vol = _CHGMNG_GetVbatVol();
    _CHGMNG_VBatQueueInit(module_state.bat_cur_vol, DISCHG_VBAT_BUFF_SIZE);     //init statistic program
    _CHGMNG_EIC_Init();

    //Tianyu.Yang
#if 0
#ifndef DUAL_BATTERY_SUPPORT
    if (module_state.bat_cur_vol < dischg_param.shutdown_vol)
    {
        if (!CHG_PHY_IsChargerPresent())
        {
            POWER_PowerOff();
        }
    }
#endif
#endif
    ischgmng_start = SCI_TRUE;      //Charge management has started.

    return 0;
}

/*****************************************************************************/
//  Description:    This function is used to close charge manager.
//  Author:         Benjamin.Wang
//  Note:           It can't be called in int handler!
/*****************************************************************************/
PUBLIC void CHGMNG_Close (void)
{
    if (g_charging_timer != NULL)
    {
        SCI_DeactiveTimer (g_charging_timer);
    }

    DoIdle_UnRegisterCallback((DOIDLE_CALLBACK_FUNC) _CHGMNG_VBatTimerHandler);

    CHG_PHY_ShutDown();

    if (ischgmng_start)
    {
        ischgmng_start = SCI_FALSE;
    }
}

/*****************************************************************************/
//  Description:    This function sets charge otp parameter.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC void CHGMNG_SetOtpParam(CHGMNG_OTP_PARAM_T* otp )
{
}
/*****************************************************************************/
//  Description:    This function set the charger type.
//  Retrun:
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetOvpParam (CHGMNG_OVP_PARAM_T* ovp)
{
    SCI_PASSERT ((ovp != NULL),  ("CHGMNG_SetOvpParam error"));    /*assert verified*/

    ovp_param = *ovp;
}


/*****************************************************************************/
//  Description:    This function get the charger type.
//  Retrun:
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_OVP_PARAM_T CHGMNG_GetOvpParam (void)
{
    return ovp_param;
}

/*****************************************************************************/
//  Description:    This function sets the charging parameter.
//  Retrun:
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetChgParam (CHGMNG_CHARGE_PARAM_T* chg)
{
    SCI_PASSERT ((chg != NULL), ("CHGMNG_SetChgParam err!"));   /*assert verified*/

    chg_param = *chg;
}

/*****************************************************************************/
//  Description:    This function gets the charging parameter.
//  Retrun:         recharge_voltage value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_CHARGE_PARAM_T CHGMNG_GetChgParam (void)
{
    return chg_param;
}

/*****************************************************************************/
//  Description:    Set the discharge parameter.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetDischgParam (CHGMNG_DISCHARGE_PARAM_T* dischg)
{
    SCI_PASSERT ((dischg != NULL), ("WarningVoltage Setting is too low!"));/*assert verified*/

    dischg_param = *dischg;
}

/*****************************************************************************/
//  Description:    Get the discharge parameter.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC CHGMNG_DISCHARGE_PARAM_T CHGMNG_GetDischgParam (void)
{
    return dischg_param;
}

/*****************************************************************************/
//  Description:    Get the hardware switch point which is from cc to cv.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_GetHWSwitchPoint (void)
{
    return hw_switch_point;
}

/*****************************************************************************/
//  Description:    Set the hardware switch point which is from cc to cv.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetHWSwitchPoint (CHG_SWITPOINT_E value)
{
    hw_switch_point = value;
    CHG_PHY_SetSwitchoverPoint (value);
}

/*****************************************************************************/
//  Description:    Set the vbat calibration table.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetVBatAdcCal(CHGMNG_ADC_CAL_T *cal_info)
{
    if(cal_info != NULL)
    {
        adc_cal = *cal_info;
    }
}

/*****************************************************************************/
//  Description:    Set the voltage capacity table.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetVBatCapTable (uint16 *dischg_ptable, uint16 *chg_ptable)
{
    uint32 i;
    SCI_PASSERT (((dischg_ptable != NULL) && (chg_ptable != NULL)), ("dischg_ptable != NULL!"));/*assert verified*/
    SCI_PASSERT (((dischg_ptable[0] > 0) && (chg_ptable[0] > 0)), ("dischg_ptable[0] > 0"));/*assert verified*/

    for(i = 0; i < BAT_CAPACITY_STEP; i++)
    {
        dischg_bat_capacity_table[i][0] = dischg_ptable[i];
        chg_bat_capacity_table[i][0] = chg_ptable[i];
    }

    CHGMNG_PRINT(("dischg_bat_capacity_table[5][0]:%d\n",dischg_bat_capacity_table[5][0] ));
    CHGMNG_PRINT(("dischg_bat_capacity_table[6][0]:%d\n",dischg_bat_capacity_table[6][0] ));
    CHGMNG_PRINT(("chg_bat_capacity_table[5][0]:%d\n",chg_bat_capacity_table[5][0] ));
    CHGMNG_PRINT(("chg_bat_capacity_table[6][0]:%d\n",chg_bat_capacity_table[6][0] ));
}

/*****************************************************************************/
//  Description:    This function is used to set the adapter mode, adapter or usb.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_SetChargeAdapter (CHGMNG_ADAPTER_TYPE_E mode)
{
    module_state.adp_type = mode;
#ifdef CHGMNG_PSE_SUPPORT
    if (module_state.bat_statistic_vol <= CHGMNG_POWERON_VAL)
    {
        CHG_PHY_SetChgCurrent (CHARGER_CURRENT_450MA);
    }
    else
#endif
    {
        _CHGMNG_SetChargeCurrent (mode);
    }
}

/*****************************************************************************/
//  Description:    This function is used to convert voltage value to ADC value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_VoltageToAdcvalue (uint16 votage)
{
    int32 result;

    result = ((votage - adc_cal.y0_vol)*adc_cal.zoom_in)/adc_cal.slope + adc_cal.x0_adc;
    if(result < 0)
    {
        result = 0;
    }

    //CHGMNG_PRINT:"CHGMNG:CHGMNG_VoltageToAdcvalue votage:%d adc:%d"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1510_112_2_17_23_3_28_430,(uint8*)"dd",votage,(uint16) (result));
    return (uint16) (result);
}

/*****************************************************************************/
//  Description:    This function is used to convert ADC value to voltage value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_AdcvalueToVoltage (uint32 adc)
{
    int32 result;
    int32 adcvalue = (int32)adc;

    result = (adcvalue - adcEffusePara.deltaB) * (4200 - 3600) / (adcEffusePara.deltaA - adcEffusePara.deltaB) + 3600;

    if(result < 0)
    {
        result = 0;
    }

    return result;
}

/*****************************************************************************/
//  Description:    This function is used to convert ADC value to voltage value,use small scale(0.1v-1v).
//  Author:         Tony
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_AdcvalueToVoltageforSmallScale (uint32 adc)
{
    int32 result;
    int32 adcvalue = (int32)adc;

    SCI_TRACE_LOW("adc: scale0 adcvalue=%d,deltaE %d deltaF %d", adcvalue, adcEffusePara.deltaE, adcEffusePara.deltaF);
    result = (adcvalue - adcEffusePara.deltaE) * (1000 - 100) / (adcEffusePara.deltaF - adcEffusePara.deltaE) + 100;

    if(result < 0)
    {
        result = 0;
    }

    SCI_TRACE_LOW("CHGMNG_AdcvalueToVoltageforSmallScale result=%d", result);

    return result;
}

/*****************************************************************************/
//  Description:    This function is used to convert ADC value to voltage value,use scale.
//  Author:         Tao.shi
//  Note:
/*****************************************************************************/
PUBLIC uint16 CHGMNG_AdcvalueToVoltage_baseScale(uint32 adc_value, ADC_SCALE_TYPE_E scale)
{
    int32 result;
    int32 adcvalue = (int32)adc_value;

    switch(scale)
    {
        case ADC_SCALE_1V250:
            result = (adcvalue - adcEffusePara.deltaE) * (1000 - 100) / (adcEffusePara.deltaF - adcEffusePara.deltaE) + 100;
            result = result * 1 / 1;
            /*
            //Vch1=(Dch1-DE)*(1.0-0.1)/(DF-DE) + 0.1
            //volt range 0.1-1V  use for adc1-4
            */
        break;
        case ADC_SCALE_2V444:
            result = (adcvalue - adcEffusePara.deltaG) * (1400 - 200) / (adcEffusePara.deltaH - adcEffusePara.deltaG) + 200; 	//Tianyu.Yang
            /*
            //Vch1=(Dch1-DG)*(1.40-0.2)/(DH-DG) + 0.2
            //volt range 0.2-1.4V used for bat det
            */
        break;
        case ADC_SCALE_3V233:

            if(prvAdcScale_23_calibrated_get() != 0 )
            {
                result = (adcvalue - adcEffusePara.deltaC) * (3000 - 300) / (adcEffusePara.deltaD - adcEffusePara.deltaC) + 300;
            }
            else
            {
                result = (adcvalue - adcEffusePara.deltaE) * (1000 - 100) / (adcEffusePara.deltaF - adcEffusePara.deltaE) + 100;
                result = result * 26 / 10;
            }
            /*
            //Vch1=[(Dch1-DE)*(1.0-0.1)/(DF-DE) + 0.1]*2.6
            //volt range 0.26-2.6v use for adc1-4
            */
        break;
        case ADC_SCALE_5V000:

            if(prvAdcScale_23_calibrated_get() != 0 )
            {
                result = (adcvalue - adcEffusePara.deltaJ) * (4000 - 500) / (adcEffusePara.deltaK - adcEffusePara.deltaJ) + 500;
            }
            else
            {
                result = (adcvalue - adcEffusePara.deltaE) * (1000 - 100) / (adcEffusePara.deltaF - adcEffusePara.deltaE) + 100;
                result = result * 406 / 100;
            }
            /*
            //Vch1=[(Dch1-DE)*(1.0-0.1)/(DF-DE) + 0.1]*2.6
            //volt range 0.406-4.06V
            */
        break;
        default:
        /* assert "sacle error!" */
        SCI_ASSERT(0);
        break;
    }
    result = result < 0 ? 0:result;
    return (uint16)result;
}


/*****************************************************************************/
//  Description:    When charger pulgin/unplug interrupt happened, this function is called.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC void CHGMNG_ChargerPlugInHandler (uint32 gpio_id, uint32 gpio_state)
{
    SCI_TRACE_LOW("CHGMNG_ChargerPlugInHandler ischgmng_start=%d", ischgmng_start);

    if (!ischgmng_start)
    {
        return;
    }

    if (CHG_PHY_IsChargerPresent())
    {
        CHG_PHY_SetChgCurrent(CHARGER_CURRENT_450MA);
        // If in calibration mode, charger should be closed at the beginning. And then, response open and close command.
        if (POWER_IsCalibrationResetMode() && RM_GetDloaderMode()) {
            CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PESUDO_PLUG_OUT_MSG, 0);
        } else {
            CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_IN_MSG, 0);
        }
    }
    else
    {
        CHGMNG_SendMsgToChgTask(CHGMNG_CHARGER_PLUG_OUT_MSG,0);
    }

    module_state.charging_stop_reason = CHGMNG_INVALIDREASON;// when charge plug out ,to make sure charge can check voltage right
}

/*****************************************************************************/
//  Description:    This function is used to get VBAT ADC value.
//  Author:         Benjamin.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHGMNG_GetVBATADCValue (void)
{
    return _CHGMNG_GetVBATADCResult();
}
/*****************************************************************************/
//  Description:    Check charger connect.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
BOOLEAN CHGMNG_IsChargeConnect (void)
{
#if defined(FPGA_SUPPORT_SC6530) || defined(FPGA_SUPPORT_SC6531) || defined(FPGA_SUPPORT_SC6531EFM)
    return SCI_FALSE;
#else
    return CHG_PHY_IsChargerPresent();
#endif
}
/*****************************************************************************/
//  Description:    identify charger type.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC CHGMNG_ADAPTER_TYPE_E CHGMNG_IdentifyAdpType(void)
{
    CHGMNG_ADAPTER_TYPE_E type_e = CHG_PHY_IdentifyAdpType();

    if(type_e == CHGMNG_ADP_UNKNOW)
    {
        return module_state.adp_type;
    }

    return type_e;
}

#if 1
LOCAL uint32 _CHGMNG_GetADCResultByChannel (
    ADC_E                               adc_channel,
    ADC_SCALE_TYPE_E                    scale)
{
    int i,j, temp;
    int vbat_result[VBAT_RESULT_NUM];

    for (i = 0; i < VBAT_RESULT_NUM; i++)
    {
        vbat_result[i] = ADC_GetResultDirectly (adc_channel, scale);
        SCI_TRACE_LOW("_CHGMNG_GetADCResult i:%d adc:%d", i, vbat_result[i]);
    }

    for (j=1; j<=VBAT_RESULT_NUM-1; j++)
    {
        for (i=0; i<VBAT_RESULT_NUM -j; i++)
        {
            if (vbat_result[i] > vbat_result[i+1])
            {
                temp = vbat_result[i];
                vbat_result[i] = vbat_result[i+1];
                vbat_result[i+1] = temp;
            }
        }
    }


    //CHGMNG_PRINT:"CHGMNG:_CHGMNG_GetVBATADCResult:%d"
//    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_533_112_2_17_23_3_26_406,(uint8*)"d",vbat_result[VBAT_RESULT_NUM/2]);
    return vbat_result[VBAT_RESULT_NUM/2];
}


uint32 _CHGMNG_GetChannelVol (
    ADC_E                               adc_channel,
    ADC_SCALE_TYPE_E                    scale)
{
    uint32 adc_val = 0;
    uint32 vol = 0;

    if ( adc_channel >= ADC_MAX )
    {
        SCI_TRACE_LOW("_CHGMNG_GetChannelVol adc_channel:%d err!", adc_channel);
        return 0;
    }

    adc_val = _CHGMNG_GetADCResultByChannel(adc_channel, scale);

#if 0 /* old code */
    if ( ADC_SCALE_1V250 == scale )
    {
        vol = CHGMNG_AdcvalueToVoltageforSmallScale(adc_val);
    }
    else
    {
        vol = CHGMNG_AdcvalueToVoltage(adc_val);
    }
#else /* new code add by 2023.01.16@tao.shi*/
    vol = CHGMNG_AdcvalueToVoltage_baseScale(adc_val, scale);
#endif

    SCI_TRACE_LOW("_CHGMNG_GetChannelVol adc:%d scale:%d adc_val:%d, vol=%d", adc_channel,scale, adc_val, vol);

    return vol;
}


PUBLIC int32 CHGMNG_GetVbatTempVolValue (void)
{
    uint32          vol = 0;
    uint32          i = 0;
    int32           temp = 0;
    uint32          table_size = sizeof(vbat_temp_table)/sizeof(vbat_temp_table[0]);
#if defined(CHGMNG_PSE_EVT_SUPPORT)
    vol =  _CHGMNG_GetChannelVol(ADC_CHANNEL_TEMP_DET, ADC_SCALE_1V250);
#else
    vol =  _CHGMNG_GetChannelVol(ADC_CHANNEL_BAT_DET, ADC_SCALE_1V250);
#endif

    for  (i = 0; i < table_size; i++)
    {
        if (vol >= vbat_temp_table[i][0])
        {
            break;
        }
    }

    if ( i <  table_size)
    {
        temp = vbat_temp_table[i][1];
    }
    else
    {
        temp = vbat_temp_table[table_size-1][1];
    }

    SCI_TRACE_LOW("CHGMNG_GetVbatTempVolValue temp=%d, vol=%d", temp, vol);

    return temp;

}
#endif

PUBLIC int32 CHGMNG_GetNTC_TempVolValue(void)
{
    uint32          vol = 0;
    uint32          i = 0;
    int32           temp = 0;
    uint32          table_size = sizeof(ntc_temp_table)/sizeof(ntc_temp_table[0]);

    vol =  _CHGMNG_GetChannelVol(ADC_CHANNEL_ADCI2, ADC_SCALE_2V444);  //Tianyu.Yang

    for  (i = 0; i < table_size; i++)
    {
        if (vol >= ntc_temp_table[i][0])
        {
            break;
        }
    }

    if ( i <  table_size)
    {
        temp = ntc_temp_table[i][1];
    }
    else
    {
        temp = ntc_temp_table[table_size-1][1];
    }

    SCI_TRACE_LOW("CHGMNG_GetNTC_TempVolValue temp=%d, vol=%d", temp, vol);

    return temp;
}

//#define NTC_FUNC_TEST
#ifdef NTC_FUNC_TEST
//---- TEST NTC -----
void adc_test_function(void)
{
    _CHGMNG_GetChannelVol(ADC_CHANNEL_ADCI1, ADC_SCALE_1V250);
    _CHGMNG_GetChannelVol(ADC_CHANNEL_ADCI1, ADC_SCALE_3V233);
    _CHGMNG_GetChannelVol(ADC_CHANNEL_ADCI2, ADC_SCALE_1V250);
    _CHGMNG_GetChannelVol(ADC_CHANNEL_ADCI2, ADC_SCALE_3V233);
}

LOCAL void CHGMNG_GetNTC_ThreadEntry(uint32 argc, void *argv)
{
    while(1)
    {
        SCI_Sleep(1000);
        CHGMNG_GetNTC_TempVolValue();
        //adc_test_function();
    }
}

//extern void CHGMNG_GetNTC_TestThread_Init(void);
void CHGMNG_GetNTC_TestThread_Init(void)
{
    SCI_CreateThread("T_GETNTC_TEST_HANDLER",
                                    "Q_GETNTC",
                                    CHGMNG_GetNTC_ThreadEntry,
                                    0,
                                    0,
                                    0x1000,
                                    0x0010,
                                    20,
                                    SCI_PREEMPT,
                                    SCI_AUTO_START);
}

//-------------------
#endif


/*****************************************************************************/
//  Description:    Get charge module state information.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC CHGMNG_STATE_INFO_T* CHGMNG_GetModuleState(void)
{
#if 0
    uint32 channel0_val = 0;
    uint32 channel1_val = 0;
    uint32 chgCurrent = 0;
    uint32 vbat_vol = 0;
#endif

    SCI_DisableIRQ();
    module_state.chg_vol = _CHGMNG_GetVChgVol();
    module_state_to_app = module_state;
    SCI_RestoreIRQ();

    if(!ischgmng_start)
    {
        module_state_to_app.bat_cur_vol = _CHGMNG_GetVbatVol();
    }


#if 0
    vbat_vol = _CHGMNG_GetVbatVol();
    chgCurrent  = _CHGMNG_GetChgCurrent();
    channel0_val = _CHGMNG_GetChannelVol(ADC_CHANNEL_BAT_DET);
    channel1_val = _CHGMNG_GetChannelVol(ADC_CHANNEL_ADCI1);

    SCI_TRACE_LOW("CHGMNG_GetModuleState chg_vol(ch14)=%d, vbat_vol(ch5)=%d, chgCurrent(ch16)=%d, channel0_val=%d, channel1_val=%d",
        module_state.chg_vol, vbat_vol, chgCurrent, channel0_val, channel1_val);
#endif

    return &module_state_to_app;
}
/*****************************************************************************/
//  Description:    send massage to charge task.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
PUBLIC void CHGMNG_SendMsgToChgTask(CHGMNG_MSG_E sig,uint32 sig_param)
{
    CHGMNGSVR_SIG_T *signal = PNULL;


    SCI_TRACE_LOW("CHGMNG_SendMsgToChgTask sig=%d", sig);

    SCI_CREATE_SIGNAL(
        signal,
        sig,
        sizeof (CHGMNGSVR_SIG_T),
        SCI_IDENTIFY_THREAD());

    signal->sig_param = sig_param;

    SCI_SEND_SIGNAL((xSignalHeaderRec *)signal, chg_task_id);
}

#ifdef DUAL_BATTERY_SUPPORT

////dual battery
/*****************************************************************************/
//  Description:    reset charge module for dual battery module.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
LOCAL void _CHGMNG_ModuleReset(uint32 voltage)
{

    //CHGMNG_PRINT:"CHGMNG:CHGMNG_ModuleReset!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    //SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_1619_112_2_17_23_3_28_431,(uint8*)"");

    if (CHGMNG_CHARGING == module_state.chgmng_state)
    {
        _CHGMNG_VBatQueueInit (voltage, CHG_VBAT_BUFF_SIZE);
    }
    else
    {
        _CHGMNG_VBatQueueInit (voltage,DISCHG_VBAT_BUFF_SIZE);
    }

     module_state.bat_remain_cap = _CHGMNG_VoltagetoPercentum (module_state.bat_statistic_vol,module_state.chgmng_state,SCI_TRUE);

    _CHGMNG_COUNTER_INIT(warning_counter, 0);
    _CHGMNG_SetChargeCurrent (module_state.adp_type);

    _CHGMNG_CurrentQueueInit (_CHGMNG_GetChgCurrent());
    isshutting_down = SCI_FALSE;
}

/*****************************************************************************/
//  Description:    Convert ADCVoltage to percentrum for dual battery.
//  Author:         Mingwei.Zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 CHGMNG_VoltagetoPercentumDBat (uint32 bat, uint32 voltage,BOOLEAN init)
{
    uint16 percentum;
    int32 temp;
    int pos = 0;

    for(pos = 0; pos < BAT_CAPACITY_STEP -1; pos++){
        if(voltage > dischg_bat_capacity_table[pos][0])
            break;
    }
    if(pos == 0) {
        percentum = 100;
    }else{
        temp = dischg_bat_capacity_table[pos][1]-dischg_bat_capacity_table[pos-1][1];
        temp = temp*(voltage - dischg_bat_capacity_table[pos][0]);
        temp = temp/(dischg_bat_capacity_table[pos][0] - dischg_bat_capacity_table[pos-1][0]);
        temp = temp + dischg_bat_capacity_table[pos][1];
        if(temp < 0){
            temp = 0;
        }
        percentum = temp;
    }

    return percentum;
}

#endif

#ifdef CHGMNG_PSE_SUPPORT
PUBLIC uint16 CHGMNG_AdjustTempRegion(void)
{
     if(module_state.charging_temperature >= temp_param.hot_temp)
     {    // Hot Temperature
         temp_bat_region = PSE_BATTERY_TEMP_REGION__HIGH;
         temp_region = PSE_BATTERY_TEMP_REGION__HOT;
     }else if((module_state.charging_temperature < temp_param.hot_temp)
        && (module_state.charging_temperature >= temp_param.high_temp))
     {    // High Temperature
         temp_bat_region = PSE_BATTERY_TEMP_REGION__HIGH;
         temp_region = PSE_BATTERY_TEMP_REGION__HIGH;
     } else if((module_state.charging_temperature < temp_param.high_temp)
         && (module_state.charging_temperature >= temp_param.standard_temp))
     {    // Standard Temperature
         temp_bat_region = PSE_BATTERY_TEMP_REGION__STANDARD;
         temp_region = PSE_BATTERY_TEMP_REGION__STANDARD;
     }else if((module_state.charging_temperature < temp_param.standard_temp)
        && (module_state.charging_temperature >= temp_param.specific_temp))
     {    // Specific Temperature
         temp_bat_region = PSE_BATTERY_TEMP_REGION__STANDARD;
         temp_region = PSE_BATTERY_TEMP_REGION__SPECIFIC;
     }else if((module_state.charging_temperature < temp_param.specific_temp)
        && (module_state.charging_temperature >= temp_param.low_temp))
     {    // Low Temperature
         temp_bat_region = PSE_BATTERY_TEMP_REGION__LOW;
         temp_region = PSE_BATTERY_TEMP_REGION__LOW;
     }else if(module_state.charging_temperature < temp_param.low_temp)
     {  // Cold Temperature
         temp_bat_region = PSE_BATTERY_TEMP_REGION__LOW;
         temp_region = PSE_BATTERY_TEMP_REGION__COLD;
     }

    return temp_region;
}

PUBLIC void  CHGMNG_InitPseChgParam (void)
{
    pse_param[PSE_BATTERY_TEMP_REGION__COLD].chg_end_vol = pse_chg_param.cold_chg_end_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__COLD].rechg_vol = pse_chg_param.cold_rechg_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__COLD].chg_current = pse_chg_param.cold_chg_current;

    pse_param[PSE_BATTERY_TEMP_REGION__LOW].chg_end_vol = pse_chg_param.low_chg_end_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__LOW].rechg_vol = pse_chg_param.low_rechg_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__LOW].chg_current = pse_chg_param.low_chg_current;
    if(hw_switch_point >= 13)
        pse_param[PSE_BATTERY_TEMP_REGION__LOW].switch_point = hw_switch_point-13;
    else
        pse_param[PSE_BATTERY_TEMP_REGION__LOW].switch_point = CHG_SWITPOINT_15;

    pse_param[PSE_BATTERY_TEMP_REGION__SPECIFIC].chg_end_vol = pse_chg_param.specific_chg_end_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__SPECIFIC].rechg_vol = pse_chg_param.specific_rechg_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__SPECIFIC].chg_current = pse_chg_param.specific_chg_current;
    pse_param[PSE_BATTERY_TEMP_REGION__SPECIFIC].switch_point = hw_switch_point;

    pse_param[PSE_BATTERY_TEMP_REGION__STANDARD].chg_end_vol = pse_chg_param.standard_chg_end_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__STANDARD].rechg_vol = pse_chg_param.standard_rechg_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__STANDARD].chg_current = pse_chg_param.standard_chg_current;
    pse_param[PSE_BATTERY_TEMP_REGION__STANDARD].switch_point = hw_switch_point;

    pse_param[PSE_BATTERY_TEMP_REGION__HIGH].chg_end_vol = pse_chg_param.high_chg_end_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__HIGH].rechg_vol = pse_chg_param.high_rechg_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__HIGH].chg_current = pse_chg_param.high_chg_current;
    if(hw_switch_point >= 13)
        pse_param[PSE_BATTERY_TEMP_REGION__HIGH].switch_point = hw_switch_point-13;
    else
        pse_param[PSE_BATTERY_TEMP_REGION__HIGH].switch_point = CHG_SWITPOINT_15;

    pse_param[PSE_BATTERY_TEMP_REGION__HOT].chg_end_vol = pse_chg_param.hot_chg_end_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__HOT].rechg_vol = pse_chg_param.hot_rechg_vol;
    pse_param[PSE_BATTERY_TEMP_REGION__HOT].chg_current = pse_chg_param.hot_chg_current;
}

#endif

PUBLIC void CHGMNG_FSM_TAKE_THERMAL(CHGMNG_FSM_EVENT_E event, uint32 condition)
{
	_CHGMNG_FSMProcess(event, condition);
}
/////old function dummy
uint32 CHR_GetVoltage (void){return 0;}
uint32 CHR_GetBatCapacity (void){ return 0; }
uint32 CHR_GetBattState (void){ return 0;}
uint32 CHR_SetBattIntRes (uint32 level){ return 0;}
uint32 CHR_SetTPulseOutCharge (uint32 level){ return 0;}
uint32 CHR_SetTPulseInCharge (uint32 level){ return 0;}
uint32 CHR_SetBattLevelMax (uint32 level){ return 0;}
uint32 CHR_SetBattLevelMin (uint32 level){ return 0;}
uint32 CHR_CheckBatteryStaus (void){ return 0;}
void CHR_StopCharge (void){}
void CHR_StartCharge (void){}
uint32 CHR_GetCurVoltage (void)
{
    CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();

    return p_chgmng_info->bat_cur_vol / 10;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of charge.c


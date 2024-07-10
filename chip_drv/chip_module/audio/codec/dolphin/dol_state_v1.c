/******************************************************************************
 ** File Name:      dol_state_v1.c                                            *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Realizing a state machine to stablize the audio codec, and*
 **                 exporting interfaces to audio device HAL layer.           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "sci_api.h"
#include "priority_system.h"

#include "chip_plf_export.h"
#include "ldo_drvapi.h"

#include "dol_state.h"
#include "dol_phy.h"
#include "audio_hal.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
/*
 * when need close codecphin , we wait some time 
 *  then , it will quickly open.
 *  CONFIG MOCRO DOL_CLOSE_WAIT_TIME for you system.
*/
#define CLOSE_WAIT_TEST         1

/*
 * use task event process ATOM op.
*/
#define USE_EVENT_TRIGGER       1

/*
 * MUST SET IF USE DOLPHIN.
*/
#define HP_DAC_MUTE_NEED_DAC_BUG    1

#ifdef DOL_DEBUG
#define DOL_STATE_DGB           SCI_TRACE_LOW
#define DOL_CHECK_MAX_CNT_ASS   SCI_ASSERT /*assert verified*/
#else
#define DOL_STATE_DGB(...)
#define DOL_CHECK_MAX_CNT_ASS(x)
#endif
#define DOL_STATE_PRT           SCI_TRACE_LOW

#define DOL_G_LOCK()            do{ SCI_DisableIRQ(); }while(0)
#define DOL_G_UNLOCK()          do{ SCI_RestoreIRQ(); }while(0)


//uint (x) ms
#define DOL_LDO_OPEN_TIME       (5)
#define DOL_RESET_TIME          (5)

//ken.kuang this delay maybe in 10-120.
//this use max value for most chip.
#define DOL_TSBYU_TIME          (120) //sb-->sb_sleep
#define DOL_TSHADC_TIME         (250)
#define DOL_TSHDAC_TIME         (2)
#define DOL_TSBOUT_TIME         (290)
#define DOL_TDACMUTE_TIME       (20)


#define DOL_RUPD_DET_TIME       (10)
#define DOL_UPD_DET_TIME        (5)

#define DOL_RUPD_DET_MAX_CNT    (26)  //(290+26*10)=550 ms
#define DOL_UPD_DET_MAX_CNT     (16)  //(20+16*5)=100 ms


#if CLOSE_WAIT_TEST
#define DOL_CLOSE_WAIT_TIME     (1125)
#endif

#define DOL_TASK_NAME           "DOL_TASK"
#define DOL_QNAME_NAME          "Q_DOL_TASK"
#define DOL_STACK_SIZE          (2*1024)
#define DOL_QUEUE_NUM           (30)
#define DOL_TASK_PRIV           (PRI_CODEC_DRV_TASK) //ken.kuang: this pri < AUDIO task pri.

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef uint32 (*RUN_DOL_S_FUN)(uint32);

/*
 * STATE FUNTION.
*/
LOCAL uint32 _run_off_s(uint32 param);
LOCAL uint32 _run_pwr_s(uint32 param);
LOCAL uint32 _run_slp_s(uint32 param);
LOCAL uint32 _run_comm_on_s(uint32 param);
LOCAL uint32 _run_on_stop_s(uint32 param);
LOCAL uint32 _run_ad_s(uint32 param);
LOCAL uint32 _run_lin_rec_s(uint32 param);
LOCAL uint32 _run_da_s(uint32 param);
LOCAL uint32 _run_dac_lin_s(uint32 param);
LOCAL uint32 _run_mix_s(uint32 param);
LOCAL uint32 _run_lout_hp_s(uint32 param);
LOCAL uint32 _run_mute_s(uint32 param);
#if CLOSE_WAIT_TEST
LOCAL uint32 _run_close_wait_s(uint32 param);
#endif

//1: ORDER IMPORTANT!!
typedef enum
{
    OFF_S = 0,
    PWR_S,
    SLP_S,
    COMM_ON_S,
    ON_STOP_S,
    //ad-capture
    AD_S,
    LIN_REC_S,
    //da-playback
    DA_S,
    DAC_LIN_S,
    MIX_S,
    LOUT_HP_S,
    MUTE_S,
#if CLOSE_WAIT_TEST
    CLOSE_WAIT_S,
#endif
    MAX_S
} DOL_DOL_STATE_E;
//1: ORDER IMPORTANT!!
//2: same order like DOL_DOL_STATE_E
LOCAL const RUN_DOL_S_FUN _dol_s_fun_tbl[MAX_S] = 
{
    (RUN_DOL_S_FUN) _run_off_s,
    (RUN_DOL_S_FUN) _run_pwr_s,
    (RUN_DOL_S_FUN) _run_slp_s,
    (RUN_DOL_S_FUN) _run_comm_on_s,
    (RUN_DOL_S_FUN) _run_on_stop_s,
    (RUN_DOL_S_FUN) _run_ad_s,
    (RUN_DOL_S_FUN) _run_lin_rec_s,
    (RUN_DOL_S_FUN) _run_da_s,
    (RUN_DOL_S_FUN) _run_dac_lin_s,
    (RUN_DOL_S_FUN) _run_mix_s,
    (RUN_DOL_S_FUN) _run_lout_hp_s,
    (RUN_DOL_S_FUN) _run_mute_s,
#if CLOSE_WAIT_TEST
    (RUN_DOL_S_FUN) _run_close_wait_s,
#endif
};

#ifdef DOL_DEBUG
//1: ORDER IMPORTANT!!
LOCAL char* _dol_s_str_tbl[MAX_S] = 
{
    "OFF_S",
    "PWR_S",
    "SLP_S",
    "COMM_ON_S",
    "ON_STOP_S",
    "AD_S",
    "LIN_REC_S",
    "DA_S",
    "DAC_LIN_S",
    "MIX_S",
    "LOUT_HP_S",
    "MUTE_S",
#if CLOSE_WAIT_TEST
    "CLOSE_WAIT_S",
#endif
};
#endif


typedef enum
{
    START_S2 = 0,
    PWR_RESET,
    PWR_UP,
    PWR_DONE,
    DAC_DELAY_DONE,
    HP_POP_U_DONE_CHECK,
    HP_POP_U_DONE,
    HP_POP_D_DONE_CHECK,
    HP_POP_D_DONE,
    GAIN_U_DONE_CHECK,
    GAIN_U_DONE,
    GAIN_D_DONE_CHECK,
    GAIN_D_DONE,
    END_S2=START_S2
}DOL_STATE2_E;

/*
 * TASK MSG.
*/
typedef enum {
    DOL_START_RUN = 1,
    DOL_MAX_MSG
} DOL_TASK_MSG_E;

typedef enum
{
    D_OFF = 0,
    D_ON
} DOL_STATE_ONOFF_E;

typedef enum
{
    D_NOTHING = 0,
    D_OPEN,
    D_CLOSE
} DOL_STATE_OC_E;

//ORDER IMPORTANT!!
typedef enum
{
    I_DA = 0,
    I_LIN,
    I_DAC,
    I_HP,
    I_LO,
    I_DAC_MUTE,
    
    I_LIR,
    I_AD,
    I_MAX
} DOL_STATE_OC_INDEX_E;


#if (USE_EVENT_TRIGGER)
enum{
    DOL_RUNING     =   0x1,
    DOL_DELAING    =   0x2,
    DOL_WAITING    =   0x4,
    DOL_LOCKING    =   0x8,
    DOL_MAX_EVENT
};
#endif

typedef struct
{
    //DA
    DOL_STATE_ONOFF_E       st;            // DA
    DOL_STATE_ONOFF_E       lin_st;        // LINEIN
    DOL_STATE_ONOFF_E       dac_st;        // DAC
    DOL_STATE_ONOFF_E       hp_st;         // hp path flag.
    DOL_STATE_ONOFF_E       lo_st;         // lo path flag.
    DOL_STATE_ONOFF_E       unmute_st;     // DAC MUTE.
}DOL_DA_STATE_T;

typedef struct
{
    //AD
    DOL_STATE_ONOFF_E       st;            // AD
    DOL_STATE_ONOFF_E       liR_st;        // LINEINREC
}DOL_AD_STATE_T;


typedef struct
{
    DOL_STATE_OC_E          oc[I_MAX];
    DOL_DA_STATE_T          da;
    DOL_AD_STATE_T          ad;
    // aux val
#if !(USE_EVENT_TRIGGER)
    uint32                  locking;
    uint32                  runing;
    uint32                  delaing;
    uint32                  waiting;
#endif
    DOL_DOL_STATE_E         cur_st;
    DOL_DOL_STATE_E         pre_st;
    uint32                  st_2;
    uint32                  delay_cnt;

}DOL_STATE_RUN_T;

typedef struct{
    uint32                  func_flag;
    DOL_STATE_OC_E          dol_oc[I_MAX];
}DOL_STATE_TBL_T;


typedef struct
{
    char                    magic_str[AUDIO_MAGIC_STR_MAX_LEN];

    DOL_STATE_RUN_T         st_run;
    DOL_STATE_OC_E          ch[I_MAX];
#if (USE_EVENT_TRIGGER)
    SCI_EVENT_GROUP_PTR     event;
#endif
    SCI_TIMER_PTR           timer;
    BLOCK_ID                task_id;
    BOOLEAN                 is_ready;

    uint32                  magic_num;
} DOL_STATE_T;

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

LOCAL DOL_STATE_T s_dol_state = {0};

LOCAL const DOL_STATE_TBL_T _dol_state_tbl[]=
{//         fun_flg     , {I_DA     ,I_LIN  ,I_DAC  ,I_HP   ,I_LO   ,I_DAC_MUTE ,I_LIR  ,I_AD   }
    {DOL_FUN_PLAYBACK   , {D_OPEN   ,0      ,D_OPEN ,0      ,0      ,0          ,0      ,0      }},
    {DOL_FUN_CAPTURE    , {0        ,0      ,0      ,0      ,0      ,0          ,0      ,D_OPEN }},
    {DOL_FUN_LINEIN     , {D_OPEN   ,D_OPEN ,0      ,0      ,0      ,0          ,0      ,0      }},
    {DOL_FUN_LINEIN_REC , {0        ,0      ,0      ,0      ,0      ,0          ,D_OPEN ,D_OPEN }},
    {DOL_FUN_HP         , {0        ,0      ,0      ,D_OPEN ,0      ,0          ,0      ,0      }},
    {DOL_FUN_LOUT       , {0        ,0      ,0      ,0      ,D_OPEN ,0          ,0      ,0      }},
    {DOL_FUN_DAC_UNMUTE , {0        ,0      ,0      ,0      ,0      ,D_OPEN     ,0      ,0      }},
};
#define _dol_state_tbl_len (sizeof(_dol_state_tbl)/sizeof(_dol_state_tbl[0]))

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/*
 * AUX FUNTION.
*/
LOCAL uint32 _is_runing(void);
LOCAL void   _set_runing(uint32 set);
LOCAL uint32 _is_locking(void);
LOCAL void   _set_locking(uint32 set);
LOCAL uint32 _is_delaing(void);
LOCAL void   _set_delaing(uint32 set);

/*
 * CODEC LOCAL FUNTION.
*/
LOCAL __inline uint32 _dol_is_oc(DOL_STATE_OC_E *pSrc,uint32 oc);
LOCAL __inline void   _dol_oc_set(DOL_STATE_OC_E *pSrc,uint32 oc);
LOCAL __inline void   _dol_on_off_set(DOL_STATE_ONOFF_E *pSrc,uint32 onoff);
LOCAL void   _dol_apply_change(void);
LOCAL void   _dol_change_state(DOL_DOL_STATE_E st);
LOCAL void   _dol_init_state(void);
LOCAL uint32 _dol_timer_wait(uint32 time);
LOCAL void   _dol_timer_wait_cancel(void);
LOCAL uint32 _dol_timer_delay(uint32 time);
LOCAL void   _dol_notify_data(uint32 need);
LOCAL void   _dol_stop_all(uint32 is_open);
LOCAL void   _dol_merge_change(void *to,const void* from);

LOCAL  void _dol_send_msg(uint32 msg);

LOCAL void  _dol_get_oc(DOL_STATE_OC_E *p_dol_oc,uint32 func_flag);
LOCAL void  _dol_change_oc_flag(DOL_STATE_OC_E * p_dol_oc);
LOCAL void  _dol_mute_all(void);


#define IS_OPEN(v)      (_dol_is_oc((DOL_STATE_OC_E*)&(v),D_OPEN))
#define IS_CLOSE(v)     (_dol_is_oc((DOL_STATE_OC_E*)&(v),D_CLOSE))
#define IS_ON(v)        (v==D_ON)
#define IS_OFF(v)       (v==D_OFF)

#define S_DOL_M         (s_dol_state.st_run)

//ALL CONDITION

//COMM
#define O_CONDI          ((AD_O) || (DA_O))                      //T01
//#define C_CONDI        ((AD_C) && (DA_C))                      //T02

//AD
#define AD_O             (IS_OPEN(S_DOL_M.oc[I_AD]))             //T03
#define AD_C             (IS_CLOSE(S_DOL_M.oc[I_AD]))            //T04
#define LIN_REC_O        (IS_OPEN(S_DOL_M.oc[I_LIR]))            //T05
#define LIN_REC_C        (IS_CLOSE(S_DOL_M.oc[I_LIR]))           //T06

//DA
#define DA_O             ((IS_OPEN(S_DOL_M.oc[I_DA])))           //T07
#define DA_C             ((IS_CLOSE(S_DOL_M.oc[I_DA])))          //T08

#define DAC_O            ((IS_OPEN(S_DOL_M.oc[I_DAC])))          //T09
#define DAC_C            ((IS_CLOSE(S_DOL_M.oc[I_DAC])))         //T10
#define LIN_O            ((IS_OPEN(S_DOL_M.oc[I_LIN])))          //T11
#define LIN_C            ((IS_CLOSE(S_DOL_M.oc[I_LIN])))         //T12
#define LOUT_O           ((IS_OPEN(S_DOL_M.oc[I_LO])))           //T13
#define LOUT_C           ((IS_CLOSE(S_DOL_M.oc[I_LO])))          //T14
#define HP_O             ((IS_OPEN(S_DOL_M.oc[I_HP])))           //T15
#define HP_C             ((IS_CLOSE(S_DOL_M.oc[I_HP])))          //T16

#define DACUNMUTE        ((IS_OPEN(S_DOL_M.oc[I_DAC_MUTE])))     //T17
#define DACMUTE          ((IS_CLOSE(S_DOL_M.oc[I_DAC_MUTE])))    //T18


#define AD_ON            (IS_ON(S_DOL_M.ad.st))                  //T22
#define AD_OF            (IS_OFF(S_DOL_M.ad.st))                 //T23
#define LIN_REC_ON       (IS_ON(S_DOL_M.ad.liR_st))              //T24
#define LIN_REC_OF       (IS_OFF(S_DOL_M.ad.liR_st))             //T25

#define DA_ON            (IS_ON(S_DOL_M.da.st))                  //T26
#define DA_OF            (IS_OFF(S_DOL_M.da.st))                 //T27
#define HP_ON            (IS_ON(S_DOL_M.da.hp_st))               //T28
#define HP_OF            (IS_OFF(S_DOL_M.da.hp_st))              //T29
#define LOUT_ON          (IS_ON(S_DOL_M.da.lo_st))               //T30
#define LOUT_OF          (IS_OFF(S_DOL_M.da.lo_st))              //T31
#define DAC_ON           (IS_ON(S_DOL_M.da.dac_st))              //T32
#define DAC_OF           (IS_OFF(S_DOL_M.da.dac_st))             //T33
#define LIN_ON           (IS_ON(S_DOL_M.da.lin_st))              //T34
#define LIN_OF           (IS_OFF(S_DOL_M.da.lin_st))             //T35
#define FROM_COMM_STATE  (S_DOL_M.pre_st==COMM_ON_S)             //T36
#define DAC_UNMUTE       (IS_ON(S_DOL_M.da.unmute_st))           //T37
#define DAC_MUTE         (IS_OFF(S_DOL_M.da.unmute_st))          //T38

#define A_CONDI ((!(LIN_C && DAC_C)) && ((DAC_ON && ((LIN_O && LIN_OF) ||(LIN_C && LIN_ON))) || (LIN_ON && ((DAC_O && DAC_OF) ||(DAC_C && DAC_ON)))))
#define B_CONDI ((!(DA_C)) && (HP_O || LOUT_O || (HP_C || LOUT_C)))
//#define B_CONDI ((!(HP_C && LOUT_C)) && ((HP_ON && ((LOUT_O && LOUT_OF) ||(LOUT_C && LOUT_ON))) || (LOUT_ON && ((HP_O && HP_OF) ||(HP_C && HP_ON)))))
//ALL CONDITION END.

#define EXIT_RUNNING    (0)
#define RUN_TO_NEXT     (1)
#define NO_OPRATE       (0xFFA5)


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description :    codecphin state funtion.
// Author :         ken.kuang
// Return :       TRUE -- should be run next state.
//                FALSE-- DO NOT run next state. maybe complete, or need delay.
// Note :
/*****************************************************************************/
PUBLIC void DOL_SM_Process(void)
{
    //DOL_STATE_DGB:"[DOLST]DOL_SM_Process"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_412_112_2_17_23_2_42_128,(uint8*)"");
    AUDIO_HAL_PRTMEM ( 0xAAAAFFFF);
    if( (!__dol_is_arm_ctl()) )
    {
        //DOL_STATE_PRT:"[DOLST]arm not run"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_416_112_2_17_23_2_42_129,(uint8*)"");
        return;
    }
    //send msg to task for start.
    _dol_send_msg(DOL_START_RUN);
}

PUBLIC BOOLEAN DOL_IsReady(void)
{
    BOOLEAN ret = 0;
    DOL_G_LOCK();
    ret = s_dol_state.is_ready;
    DOL_G_UNLOCK();
    return ret;
}

PUBLIC BOOLEAN DOL_IsOnStop(void)
{
    BOOLEAN ret = 0;
    DOL_G_LOCK();
    ret = ((s_dol_state.st_run.cur_st==ON_STOP_S));
    DOL_G_UNLOCK();
    return ret;
}

PUBLIC BOOLEAN DOL_IsPwrOn(void)
{
    BOOLEAN ret = 0;
    DOL_G_LOCK();
    ret = ((s_dol_state.st_run.cur_st>SLP_S));
    DOL_G_UNLOCK();
    return ret;
}

PUBLIC BOOLEAN DOL_IsRuning(void)
{
    BOOLEAN ret = 0;
    DOL_G_LOCK();
    ret = !((s_dol_state.st_run.cur_st==OFF_S) || (s_dol_state.st_run.cur_st==ON_STOP_S));
    DOL_G_UNLOCK();
    return ret;
}

PUBLIC void DOL_ChangeSt(uint32 func_flag,uint32 is_close)
{
    DOL_STATE_OC_E  use_dol_oc[I_MAX];
    
    _dol_get_oc(use_dol_oc,func_flag);
    
    if(is_close)
    {
        _dol_change_oc_flag(use_dol_oc);
    }
    
    DOL_G_LOCK();
    _dol_merge_change(&(s_dol_state.ch),use_dol_oc);
    DOL_G_UNLOCK();
    
    //DOL_STATE_DGB:"[DOLST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_474_112_2_17_23_2_42_130,(uint8*)"dddddddd",s_dol_state.ch[I_AD],s_dol_state.ch[I_LIR],s_dol_state.ch[I_DA],s_dol_state.ch[I_DAC],s_dol_state.ch[I_LIN],s_dol_state.ch[I_HP],s_dol_state.ch[I_LO],s_dol_state.ch[I_DAC_MUTE]);
    AUDIO_HAL_PRTMEM ( (0xCCC50000 
        | ((s_dol_state.ch[I_AD ]<<14) & 0x0000C000)
        | ((s_dol_state.ch[I_LIR]<<12) & 0x00003000)
        | ((s_dol_state.ch[I_DA ]<<10) & 0x00000C00)
        | ((s_dol_state.ch[I_DAC]<<8)  & 0x00000300)
        | ((s_dol_state.ch[I_LIN]<<6)  & 0x000000C0)
        | ((s_dol_state.ch[I_HP ]<<4)  & 0x00000030)
        | ((s_dol_state.ch[I_LO ]<<2)  & 0x0000000C)
        | ((s_dol_state.ch[I_DAC_MUTE ]<<0)  & 0x00000003)
        ));
}

LOCAL void _run_dol_state(void)
{
    RUN_DOL_S_FUN fun = 0;
    if( _is_runing() || _is_delaing() )
    {
        return;
    }
    _dol_timer_wait_cancel();

    SCI_ASSERT( __dol_is_arm_ctl() ); /*assert verified*/

    _set_runing(1);
    //if return  FALSE means s_dol_state state off
    //   or need delay , when delay complete will start this.
    // if return TRUE means s_dol_state state should run next state.
    do{
        _dol_apply_change();
        fun = (RUN_DOL_S_FUN)(_dol_s_fun_tbl[(s_dol_state.st_run.cur_st)]);
    }while( fun((uint32)&(s_dol_state.st_run)) );
    _set_runing(0);
}


LOCAL uint32 _run_off_s(uint32 param)
{
    if( O_CONDI )
    {
        _dol_change_state(PWR_S);
    }
    else //if( C_CONDI )
    {
        return EXIT_RUNNING;
    }
    return RUN_TO_NEXT;
}

LOCAL void _test_reg_rw(void)
{
    SCI_ASSERT(__dol_reg_rw_test() && "some old plat dolphin's reset share");/*assert verified*/
}


LOCAL uint32 _run_pwr_s(uint32 param)
{
    static uint32 is_o = 0;
    if( O_CONDI )
    {
        if( !is_o )
        {
            switch(s_dol_state.st_run.st_2)
            {
                case START_S2:
                {
                    __dol_mclk(1);
                    LDO_TurnOnLDO (LDO_LDO_AVBO);
                    LDO_AutoCloseInDeepSleep (SLP_LDO_VBO, SCI_FALSE);
                    s_dol_state.st_run.st_2 = PWR_RESET;
                    return _dol_timer_delay(DOL_LDO_OPEN_TIME);
                }break;/*lint !e527 comfirmed by windy.wang*/
                case PWR_RESET:
                {
                    __dol_reset();
                    s_dol_state.st_run.st_2 = PWR_UP;
                    return _dol_timer_delay(DOL_RESET_TIME);
                }break;/*lint !e527 comfirmed by windy.wang*/
                case PWR_UP:
                {
                    //after reset.
                    _test_reg_rw();
                    __dol_open();
                    __dol_fs_setting();
                    __dol_sb(1);
                    s_dol_state.st_run.st_2 = PWR_DONE;
                    return _dol_timer_delay(DOL_TSBYU_TIME);
                }break;/*lint !e527 comfirmed by windy.wang*/
                case PWR_DONE:
                {
                    __dol_pga_bug(); //ken.kuang fixed.
                    s_dol_state.st_run.st_2 = END_S2;
                    is_o = 1;
                }break;
                default:break;
            }
        }
        _dol_change_state(SLP_S);
    }
    else// if( C_CONDI )
    {
        is_o = 0;
        __dol_sb(0);
        LDO_AutoCloseInDeepSleep (SLP_LDO_VBO, SCI_TRUE);
        LDO_TurnOffLDO (LDO_LDO_AVBO);
        __dol_mclk(0);
        s_dol_state.st_run.st_2 = END_S2;
        _dol_stop_all(0);
        _dol_change_state(OFF_S);
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_slp_s(uint32 param)
{
    static uint32 is_o = 0;
    if( DA_O )
    {
        if( !is_o )
        {
            __dol_sb_sleep(1);
            is_o = 1;
        }
        _dol_change_state(DA_S);
    }
    else if( AD_O )
    {
        if( !is_o )
        {
            __dol_sb_sleep(1);
            _dol_change_state(AD_S);
            is_o = 1;
            return _dol_timer_delay(DOL_TSHADC_TIME);
        }
        else
        {
            _dol_change_state(AD_S);
        }
    }
    else //if( C_CONDI )
    {
        if( is_o )
        {
            __dol_sb_sleep(0);
            is_o = 0;
        }
        _dol_change_state(PWR_S);
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_comm_on_s(uint32 param)
{
    if( (LIN_REC_C && LIN_REC_ON) || (LIN_REC_O && LIN_REC_OF && AD_ON ) )
    {
        _dol_change_state(LIN_REC_S);
    }
    else if( (AD_C && AD_ON) || (AD_O && AD_OF ) || (LIN_REC_O && AD_OF) )
    {
        _dol_change_state(AD_S);
    }
    else if( (DA_O && DA_OF ) )
    {
        _dol_change_state(DA_S);
    }
    else if( DA_ON && (A_CONDI) )
    {
        _dol_change_state(DAC_LIN_S);
    }
    else if( DA_ON && (B_CONDI) )
    {
        _dol_change_state(LOUT_HP_S);
    }
    else if( ((DACMUTE || DACUNMUTE)&& DAC_ON && DA_ON) || (DA_C && DA_ON ) )
    {
        _dol_change_state(MUTE_S);
    }
    else
    {
#if HP_DAC_MUTE_NEED_DAC_BUG
        if( DAC_OF )
        {
            __dol_sb_dac(0);
        }
        if( LIN_ON )
        {
            __dol_sb_lin(1);
        }
#endif
        _dol_change_state(ON_STOP_S);
        _dol_stop_all(1);
        _dol_notify_data(1);
    }
    return RUN_TO_NEXT;
}

#if CLOSE_WAIT_TEST
LOCAL uint32 _run_close_wait_s(uint32 param)
{
    if( (DA_C && DA_ON ) )
    {
        _dol_change_state(LOUT_HP_S);
        return _dol_timer_wait(DOL_CLOSE_WAIT_TIME);
    }
    else if (DA_O)
    {
        _dol_change_state(COMM_ON_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}
#endif


LOCAL uint32 _run_on_stop_s(uint32 param)
{
#ifdef DOL_DEBUG
    //DOL_STATE_DGB:"[DOLST]on_stop pre=%s,cur=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_697_112_2_17_23_2_43_131,(uint8*)"ss",_dol_s_str_tbl[s_dol_state.st_run.pre_st],_dol_s_str_tbl[s_dol_state.st_run.cur_st]);
#endif
    AUDIO_HAL_PRTMEM ( (0x555A0000 
        | ((s_dol_state.st_run.pre_st<<8) & 0x0000FF00)
        | ((s_dol_state.st_run.cur_st<<0) & 0x000000FF)
        ));
    if( (__dol_is_arm_ctl()) && (((O_CONDI || AD_C || DA_C || DAC_O || DAC_C || LIN_O || LIN_C || HP_O || HP_C || LOUT_O || LOUT_C || LIN_REC_O || LIN_REC_C) 
        && (AD_ON || DA_ON )) || DACMUTE || DACUNMUTE)  )
    {
#if HP_DAC_MUTE_NEED_DAC_BUG
        if( DAC_OF )
        {
            __dol_sb_dac(1);
        }
        if( LIN_C && LIN_ON )
        {
            __dol_sb_lin(0);
        }
#endif
        _dol_change_state(COMM_ON_S);
    }
    else
    {
        SCI_MEMSET(&(s_dol_state.st_run.oc),0,sizeof(s_dol_state.st_run.oc));
        return EXIT_RUNNING;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_ad_s(uint32 param)
{
    //static uint32 is_o = 0;
    if( AD_O )
    {
        __dol_sb_adc(1);
        _dol_on_off_set(&(s_dol_state.st_run.ad.st),D_ON);
        if( LIN_REC_O )
        {
            _dol_change_state(LIN_REC_S);
        }
        else
        {
            _dol_oc_set(&(s_dol_state.st_run.oc[I_AD]),D_NOTHING);
            _dol_change_state(COMM_ON_S);
        }
    }
    else if( AD_C )
    {
        __dol_sb_adc(0);
        _dol_on_off_set(&(s_dol_state.st_run.ad.st),D_OFF);
        _dol_oc_set(&(s_dol_state.st_run.oc[I_AD]),D_NOTHING);
        if( DA_ON )
        {
            _dol_change_state(COMM_ON_S);
        }
        else if( DA_O )
        {
            _dol_change_state(DA_S);
        }
        else //if( !(DA_O || liO) )
        {
            _dol_change_state(SLP_S);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_lin_rec_s(uint32 param)
{
    //static uint32 is_o = 0;
    if( LIN_REC_O )
    {
        __dol_linein_rec(1);
        _dol_on_off_set(&(s_dol_state.st_run.ad.liR_st),D_ON);
        s_dol_state.st_run.oc[I_AD] = s_dol_state.st_run.oc[I_LIR] = D_NOTHING;//_dol_oc_set
        _dol_change_state(COMM_ON_S);
    }
    else if( LIN_REC_C )
    {
        __dol_linein_rec(0);
        _dol_on_off_set(&(s_dol_state.st_run.ad.liR_st),D_OFF);
        _dol_oc_set(&(s_dol_state.st_run.oc[I_LIR]),D_NOTHING);
        _dol_change_state(AD_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_da_s(uint32 param)
{
    //static uint32 is_o = 0;
    _dol_on_off_set(&(s_dol_state.st_run.da.st),D_OFF);
    if( DA_O )
    {
        _dol_mute_all();
        _dol_change_state(DAC_LIN_S);
    }
    else if( DA_C  )
    {
        _dol_oc_set(&(s_dol_state.st_run.oc[I_DA]),D_NOTHING);
        if( AD_ON )
        {
            _dol_change_state(COMM_ON_S);
        }
        else if( AD_O )
        {
            _dol_change_state(AD_S);
        }
        else
        {
            _dol_change_state(SLP_S);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}


LOCAL uint32 _run_dac_s(uint32 param)
{
    static uint32 is_o = 0;
    if( DAC_O )
    {
        if( !is_o )
        {
            switch( s_dol_state.st_run.st_2 )
            {
                case START_S2:
                {
                    if( !FROM_COMM_STATE )
                    {
                        s_dol_state.st_run.st_2 = DAC_DELAY_DONE;
                        return _dol_timer_delay(DOL_TSHDAC_TIME);
                    }
                }break;
                case DAC_DELAY_DONE:
                {
                    s_dol_state.st_run.st_2 = END_S2;
                }break;
                default:break;
            }
            __dol_sb_dac(1);
            _dol_on_off_set(&(s_dol_state.st_run.da.dac_st),D_ON);
            _dol_oc_set(&(s_dol_state.st_run.oc[I_DAC]),D_NOTHING);
            is_o = 1;
        }
        else
        {
            _dol_oc_set(&(s_dol_state.st_run.oc[I_DAC]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else if( DAC_C )
    {
        if( is_o )
        {
            __dol_sb_dac(0);
            _dol_on_off_set(&(s_dol_state.st_run.da.dac_st),D_OFF);
            _dol_oc_set(&(s_dol_state.st_run.oc[I_DAC]),D_NOTHING);
            is_o = 0;
        }
        else
        {
            _dol_oc_set(&(s_dol_state.st_run.oc[I_DAC]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else
    {
        _dol_oc_set(&(s_dol_state.st_run.oc[I_DAC]),D_NOTHING);
        return NO_OPRATE;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_lin_s(uint32 param)
{
    static uint32 is_o = 0;
    if( LIN_O )
    {
        if( !is_o )
        {
#if HP_DAC_MUTE_NEED_DAC_BUG
            if( DAC_OF )
            {
                __dol_sb_dac(1);
            }
#else
            __dol_sb_lin(1);
#endif
            _dol_on_off_set(&(s_dol_state.st_run.da.lin_st),D_ON);
            _dol_oc_set(&(s_dol_state.st_run.oc[I_LIN]),D_NOTHING);
            is_o = 1;
        }
        else
        {
            _dol_oc_set(&(s_dol_state.st_run.oc[I_LIN]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else if( LIN_C )
    {
        if( is_o )
        {
#if HP_DAC_MUTE_NEED_DAC_BUG
            if( DAC_OF )
            {
                __dol_sb_dac(0);
            }
#endif
            __dol_sb_lin(0);
            _dol_on_off_set(&(s_dol_state.st_run.da.lin_st),D_OFF);
            _dol_oc_set(&(s_dol_state.st_run.oc[I_LIN]),D_NOTHING);
            is_o = 0;
        }
        else
        {
            _dol_oc_set(&(s_dol_state.st_run.oc[I_LIN]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else
    {
        _dol_oc_set(&(s_dol_state.st_run.oc[I_LIN]),D_NOTHING);
        return NO_OPRATE;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_dac_lin_s(uint32 param)
{
    uint32 ret = 0;
    ret = _run_dac_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_dol_state.st_run.st_2) )
    {
        return ret;
    }
    ret = _run_lin_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_dol_state.st_run.st_2) )
    {
        return ret;
    }

    if( DA_ON && FROM_COMM_STATE  )
    {
        s_dol_state.st_run.oc[I_DA] = s_dol_state.st_run.oc[I_DAC] =s_dol_state.st_run.oc[I_LIN] = D_NOTHING;//_dol_oc_set
        _dol_change_state(COMM_ON_S);
    }
    else if( DA_O )
    {
        _dol_change_state(MIX_S);
    }
    else if( DA_C )
    {
        _dol_change_state(DA_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}


LOCAL uint32 _run_mix_s(uint32 param)
{
    if( DA_O )
    {
        __dol_sb_mix(1);
        _dol_on_off_set(&(s_dol_state.st_run.da.st),D_ON);
        _dol_change_state(LOUT_HP_S);
    }
    else if( DA_C  )
    {
        __dol_sb_mix(0);
        _dol_change_state(DAC_LIN_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_lout_s(uint32 param)
{
    static uint32 is_o = 0;
    if( LOUT_O )
    {
        if( !is_o )
        {
            __dol_sb_lout(1);
            _dol_on_off_set(&(s_dol_state.st_run.da.lo_st),D_ON);
            _dol_oc_set(&(s_dol_state.st_run.oc[I_LO]),D_NOTHING);
            is_o = 1;
        }
        else
        {
            _dol_oc_set(&(s_dol_state.st_run.oc[I_LO]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else if( LOUT_C )
    {
        if( is_o )
        {
            __dol_sb_lout(0);
            _dol_on_off_set(&(s_dol_state.st_run.da.lo_st),D_OFF);
            _dol_oc_set(&(s_dol_state.st_run.oc[I_LO]),D_NOTHING);
            is_o = 0;
        }
        else
        {
            _dol_oc_set(&(s_dol_state.st_run.oc[I_LO]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else
    {
        _dol_oc_set(&(s_dol_state.st_run.oc[I_LO]),D_NOTHING);
        return NO_OPRATE;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_hp_s(uint32 param)
{
    if( HP_O )
    {
        switch(s_dol_state.st_run.st_2)
        {
            case START_S2:
            case HP_POP_D_DONE://ramp down state.
            {
#if HP_DAC_MUTE_NEED_DAC_BUG
                if( DAC_OF )
                {
                    __dol_sb_dac(1);
                }
#endif
                _set_locking(1);
                if( __dol_sb_out(1) )
                {
                    s_dol_state.st_run.st_2 = HP_POP_U_DONE_CHECK;
                    s_dol_state.st_run.delay_cnt=0;
                    return _dol_timer_delay(DOL_TSBOUT_TIME);
                }
                else
                {
                    s_dol_state.st_run.st_2 = HP_POP_U_DONE;
                    //DOL_STATE_PRT:"[AUDDRV][DOL]SB_OUT is on"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1057_112_2_17_23_2_43_132,(uint8*)"");
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
            case HP_POP_U_DONE_CHECK:
            {
                if( __dol_ramp_done(1) )
                {
                    __dol_clr_ramp(1);
                }
                else if(s_dol_state.st_run.delay_cnt>DOL_RUPD_DET_MAX_CNT)
                {
                    //DOL_STATE_PRT:"[AUDDRV][DOL]RAMP is UP"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1068_112_2_17_23_2_43_133,(uint8*)"");
                    __dol_reg_print();
                    DOL_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                }
                else
                {
                    s_dol_state.st_run.delay_cnt++;
                    return _dol_timer_delay(DOL_RUPD_DET_TIME);
                }
                s_dol_state.st_run.delay_cnt = 0;
                s_dol_state.st_run.st_2 = HP_POP_U_DONE;
            }break;/*lint !e527 comfirmed by windy.wang*/
            case HP_POP_U_DONE:
            {
                s_dol_state.st_run.st_2 = END_S2;
                _dol_on_off_set(&(s_dol_state.st_run.da.hp_st),D_ON);
                _dol_oc_set(&(s_dol_state.st_run.oc[I_HP]),D_NOTHING);
                _set_locking(0);
            }break;
            default:break;
        }
    }
    else if( HP_C )
    {
        switch(s_dol_state.st_run.st_2)
        {
            case START_S2:
            case HP_POP_U_DONE_CHECK://ramp up state
            {
#if HP_DAC_MUTE_NEED_DAC_BUG
                if( DAC_OF )
                {
                    __dol_sb_dac(1);
                }
#endif
                _set_locking(1);
                _dol_on_off_set(&(s_dol_state.st_run.da.hp_st),D_OFF);
                if( __dol_sb_out(0) )
                {
                    s_dol_state.st_run.st_2 = HP_POP_D_DONE_CHECK;
                    s_dol_state.st_run.delay_cnt=0;
                    return _dol_timer_delay(DOL_TSBOUT_TIME);
                }
                else
                {
                    s_dol_state.st_run.st_2 = HP_POP_D_DONE;
                    //DOL_STATE_PRT:"[AUDDRV][DOL]SB_OUT is off"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1114_112_2_17_23_2_43_134,(uint8*)"");
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
            case HP_POP_D_DONE_CHECK:
            {
                if( __dol_ramp_done(0) )
                {
                    __dol_clr_ramp(0);
                }
                else if(s_dol_state.st_run.delay_cnt>DOL_RUPD_DET_MAX_CNT)
                {
                    //DOL_STATE_PRT:"[AUDDRV][DOL]RAMP is DOWN"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1125_112_2_17_23_2_43_135,(uint8*)"");
                    __dol_reg_print();
                    DOL_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                }
                else
                {
                    s_dol_state.st_run.delay_cnt++;
                    return _dol_timer_delay(DOL_RUPD_DET_TIME);
                }
                s_dol_state.st_run.delay_cnt = 0;
                s_dol_state.st_run.st_2 = HP_POP_D_DONE;
            }break;/*lint !e527 comfirmed by windy.wang*/
            case HP_POP_D_DONE:
            {
                s_dol_state.st_run.st_2 = END_S2;
                _dol_oc_set(&(s_dol_state.st_run.oc[I_HP]),D_NOTHING);
                _set_locking(0);
            }break;
            default:break;
        }
    }
    else
    {
        _dol_oc_set(&(s_dol_state.st_run.oc[I_HP]),D_NOTHING);
        return NO_OPRATE;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_lout_hp_s(uint32 param)
{
    uint32 ret = 0;
    ret = _run_lout_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_dol_state.st_run.st_2) )
    {
        return ret;
    }
    ret = _run_hp_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_dol_state.st_run.st_2) )
    {
        return ret;
    }

    if( DA_O || (DA_ON && FROM_COMM_STATE)  )
    {
        s_dol_state.st_run.oc[I_DA] = s_dol_state.st_run.oc[I_HP] =s_dol_state.st_run.oc[I_LO] = D_NOTHING;//_dol_oc_set
        _dol_change_state(COMM_ON_S);
    }
    else if( DA_C )
    {
        _dol_change_state(MIX_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_mute_s(uint32 param)
{
    if( DACUNMUTE )
    {
        switch(s_dol_state.st_run.st_2)
        {
            case START_S2:
            {
#if HP_DAC_MUTE_NEED_DAC_BUG
                if( DAC_OF )
                {
                    __dol_sb_dac(1);
                }
#endif
                if( DAC_ON && __dol_dac_mute(0) )
                {
                    _set_locking(1);
                    s_dol_state.st_run.st_2 = GAIN_U_DONE_CHECK;
                    s_dol_state.st_run.delay_cnt=0;
                    return _dol_timer_delay(DOL_TDACMUTE_TIME);
                }
                else
                {
                    //DOL_STATE_PRT:"[AUDDRV][DOL]UnMuted"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1207_112_2_17_23_2_44_136,(uint8*)"");
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
            case GAIN_U_DONE_CHECK:
            {
                if( __dol_gain_done(1) )
                {
                    __dol_clr_gain(1);
                }
                else if(s_dol_state.st_run.delay_cnt>DOL_UPD_DET_MAX_CNT)
                {
                    //DOL_STATE_PRT:"[AUDDRV][DOL]GAIN is UP"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1218_112_2_17_23_2_44_137,(uint8*)"");
                    __dol_reg_print();
                    DOL_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                }
                else
                {
                    s_dol_state.st_run.delay_cnt++;
                    return _dol_timer_delay(DOL_UPD_DET_TIME);
                }
                s_dol_state.st_run.delay_cnt = 0;
            }break;/*lint !e527 comfirmed by windy.wang*/
            default:break;
        }
        s_dol_state.st_run.st_2 = END_S2;
        _dol_on_off_set(&(s_dol_state.st_run.da.unmute_st),D_ON);
        _dol_oc_set(&(s_dol_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
        _set_locking(0);
        _dol_change_state(COMM_ON_S);
    }
    else if( DACMUTE || DA_C  )
    {
        switch(s_dol_state.st_run.st_2)
        {
            case START_S2:
            {
#if HP_DAC_MUTE_NEED_DAC_BUG
                if( DAC_OF )
                {
                    __dol_sb_dac(1);
                }
#endif
                if( DAC_ON && __dol_dac_mute(1) )
                {
                    _set_locking(1);
                    s_dol_state.st_run.st_2 = GAIN_D_DONE_CHECK;
                    s_dol_state.st_run.delay_cnt=0;
                    return _dol_timer_delay(DOL_TDACMUTE_TIME);
                }
                else
                {
                    //DOL_STATE_PRT:"[AUDDRV][DOL]Muted"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1258_112_2_17_23_2_44_138,(uint8*)"");
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
            case GAIN_D_DONE_CHECK:
            {
                if( __dol_gain_done(0) )
                {
                    __dol_clr_gain(0);
                }
                else if(s_dol_state.st_run.delay_cnt>DOL_UPD_DET_MAX_CNT)
                {
                    //DOL_STATE_PRT:"[AUDDRV][DOL]GAIN is DOWN"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1269_112_2_17_23_2_44_139,(uint8*)"");
                    __dol_reg_print();
                    DOL_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                }
                else
                {
                    s_dol_state.st_run.delay_cnt++;
                    return _dol_timer_delay(DOL_UPD_DET_TIME);
                }
                s_dol_state.st_run.delay_cnt = 0;
            }break;/*lint !e527 comfirmed by windy.wang*/
            default:break;
        }
        s_dol_state.st_run.st_2 = END_S2;
        _dol_notify_data(0);
        _dol_on_off_set(&(s_dol_state.st_run.da.unmute_st),D_OFF);
        if( DA_C )
        {
            _dol_mute_all();
            _dol_oc_set(&(s_dol_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
            _set_locking(0);
#if CLOSE_WAIT_TEST
            _dol_change_state(CLOSE_WAIT_S);
#else
            _dol_change_state(LOUT_HP_S);
#endif
        }
        else if(DACMUTE)
        {
            _dol_oc_set(&(s_dol_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
            _set_locking(0);
            _dol_change_state(COMM_ON_S);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}



#if (USE_EVENT_TRIGGER)
LOCAL uint32 _is_event_set(uint32 flag)
{
    uint32 ret = 0;
    uint32 actual_event;
    if( s_dol_state.event )
    {
        ret = SCI_GetEvent(s_dol_state.event,
            flag,
            SCI_OR, //do not clear!
            &actual_event,
            SCI_NO_WAIT);
        if( SCI_SUCCESS == ret )
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
    }
    return ret;
}

LOCAL void _set_event(uint32 set,uint32 flag)
{
    if( s_dol_state.event )
    {
        if( set )
        {
            SCI_SetEvent(s_dol_state.event, flag, SCI_OR);
        }
        else
        {
            SCI_SetEvent(s_dol_state.event, ~(flag), SCI_AND);
        }
    }
}

#endif



LOCAL uint32 _is_runing()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(DOL_RUNING);
#else
    uint32 ret = SCI_ERROR;
    DOL_G_LOCK();
    ret = s_dol_state.st_run.runing;
    DOL_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_runing(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,DOL_RUNING);
#else
    DOL_G_LOCK();
    s_dol_state.st_run.runing = set;
    DOL_G_UNLOCK();
#endif
}

LOCAL uint32 _is_locking()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(DOL_LOCKING);
#else
    uint32 ret = SCI_ERROR;
    DOL_G_LOCK();
    ret = s_dol_state.st_run.locking;
    DOL_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_locking(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,DOL_LOCKING);
#else
    DOL_G_LOCK();
    s_dol_state.st_run.locking = set;
    DOL_G_UNLOCK();
#endif
}


LOCAL uint32 _is_delaing()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(DOL_DELAING);
#else
    uint32 ret = 0;
    DOL_G_LOCK();
    ret = s_dol_state.st_run.delaing;
    DOL_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_delaing(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,DOL_DELAING);
#else
    DOL_G_LOCK();
    s_dol_state.st_run.delaing = set;
    DOL_G_UNLOCK();
#endif
}

LOCAL uint32 _is_waiting()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(DOL_WAITING);
#else
    uint32 ret = 0;
    DOL_G_LOCK();
    ret = s_dol_state.st_run.waiting;
    DOL_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_waiting(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,DOL_WAITING);
#else
    DOL_G_LOCK();
    s_dol_state.st_run.waiting = set;
    DOL_G_UNLOCK();
#endif
}



LOCAL void _dol_stag1_dispel_change(void *to)
{
    DOL_STATE_OC_E * pDst = (DOL_STATE_OC_E *)to;

    if( (DAC_OF && (pDst[I_DAC] == D_CLOSE))
        || (DAC_ON && (pDst[I_DAC] == D_OPEN))
    )
    {
        pDst[I_DAC] = D_NOTHING;
    }
    if( (LIN_OF && (pDst[I_LIN] == D_CLOSE))
        || (LIN_ON && (pDst[I_LIN] == D_OPEN))
    )
    {
        pDst[I_LIN] = D_NOTHING;
    }
    if( (HP_OF && (pDst[I_HP] == D_CLOSE))
        || (HP_ON && (pDst[I_HP] == D_OPEN))
    )
    {
        pDst[I_HP] = D_NOTHING;
    }
    if( (LOUT_OF && (pDst[I_LO] == D_CLOSE))
        || (LOUT_ON && (pDst[I_LO] == D_OPEN))
    )
    {
        pDst[I_LO] = D_NOTHING;
    }
    if( (DAC_MUTE && (pDst[I_DAC_MUTE] == D_CLOSE))
        || (DAC_UNMUTE && (pDst[I_DAC_MUTE] == D_OPEN))
    )
    {
        pDst[I_DAC_MUTE] = D_NOTHING;
    }
}

LOCAL void _dol_stag2_dispel_change(void *to)
{
    DOL_STATE_OC_E * pDst = (DOL_STATE_OC_E *)to;

    if( D_CLOSE == pDst[I_DA] )
    {
        if( DAC_ON )
        {
            pDst[I_DAC] = D_CLOSE;
        }
        if( LIN_ON )
        {
            pDst[I_LIN] = D_CLOSE;
        }
        if( HP_ON )
        {
            pDst[I_HP] = D_CLOSE;
        }
        if( LOUT_ON )
        {
            pDst[I_LO] = D_CLOSE;
        }
        if( DAC_UNMUTE )
        {
            pDst[I_DAC_MUTE] = D_CLOSE;
        }
    }
}

LOCAL void _dol_stag1_merge_change(void *to,const void* from)
{
    uint32 i=0;
    DOL_STATE_OC_E * pSrc = (DOL_STATE_OC_E *)from;
    DOL_STATE_OC_E * pDst = (DOL_STATE_OC_E *)to;
    for(i=I_LIN;i<=I_LIR;i++)
    {
        if( D_NOTHING != pSrc[i] )
        {
            if( (D_NOTHING != pDst[i]) )
            {
                if(pDst[i] != pSrc[i])
                {
                    pDst[i] = D_NOTHING;
                }
            }
            else
            {
                pDst[i] = pSrc[i];
            }
            
        }
    }
}

LOCAL void _dol_stag2_merge_change(void *to,const void* from)
{
    DOL_STATE_OC_E * pSrc = (DOL_STATE_OC_E *)from;
    DOL_STATE_OC_E * pDst = (DOL_STATE_OC_E *)to;
    if( D_NOTHING != pSrc[I_AD] )
    {
        pDst[I_AD] = pSrc[I_AD];
    }
    
    if( D_NOTHING != pSrc[I_DA] )
    {
        pDst[I_DA] = pSrc[I_DA];
    }
    
    //if( D_NOTHING != pSrc[I_DAC_MUTE] )
    //{
    //    pDst[I_DAC_MUTE] = pSrc[I_DAC_MUTE];
    //}
}


LOCAL void _dol_merge_change(void *to,const void* from)
{
    _dol_stag1_merge_change(to,from);
    _dol_stag2_merge_change(to,from);
}


LOCAL void _dol_merge_st_change(void *to,const void* from)
{
    _dol_stag1_merge_change(to,from);
    
    _dol_stag1_dispel_change(to);

    //DOL_STATE_DGB:"[DOLST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1587_112_2_17_23_2_44_140,(uint8*)"dddddddd",s_dol_state.st_run.oc[I_AD],s_dol_state.st_run.oc[I_LIR],s_dol_state.st_run.oc[I_DA],s_dol_state.st_run.oc[I_DAC],s_dol_state.st_run.oc[I_LIN],s_dol_state.st_run.oc[I_HP],s_dol_state.st_run.oc[I_LO],s_dol_state.st_run.oc[I_DAC_MUTE]);
    
    _dol_stag2_merge_change(to,from);

    _dol_stag2_dispel_change(to);
}

LOCAL void _dol_apply_change(void)
{
    DOL_STATE_OC_E  use_dol_oc[I_MAX];
    if( _is_locking() )
    {
        return;
    }
    //DOL_STATE_DGB:"[DOLST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1610_112_2_17_23_2_44_141,(uint8*)"dddddddd",s_dol_state.ch[I_AD],s_dol_state.ch[I_LIR],s_dol_state.ch[I_DA],s_dol_state.ch[I_DAC],s_dol_state.ch[I_LIN],s_dol_state.ch[I_HP],s_dol_state.ch[I_LO],s_dol_state.ch[I_DAC_MUTE]);
    DOL_G_LOCK();
    SCI_MEMCPY(&(use_dol_oc),&(s_dol_state.ch),sizeof(use_dol_oc));//copy all change.
    SCI_MEMSET(&(s_dol_state.ch),0,sizeof(s_dol_state.ch)); //clear all change.
    DOL_G_UNLOCK();
    _dol_merge_st_change(&(s_dol_state.st_run.oc),&(use_dol_oc));
    //DOL_STATE_DGB:"[DOLST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1625_112_2_17_23_2_44_142,(uint8*)"dddddddd",s_dol_state.st_run.oc[I_AD],s_dol_state.st_run.oc[I_LIR],s_dol_state.st_run.oc[I_DA],s_dol_state.st_run.oc[I_DAC],s_dol_state.st_run.oc[I_LIN],s_dol_state.st_run.oc[I_HP],s_dol_state.st_run.oc[I_LO],s_dol_state.st_run.oc[I_DAC_MUTE]);
    AUDIO_HAL_PRTMEM ( (0xCCCC0000 
        | ((s_dol_state.st_run.oc[I_AD ]<<14) & 0x0000C000)
        | ((s_dol_state.st_run.oc[I_LIR]<<12) & 0x00003000)
        | ((s_dol_state.st_run.oc[I_DA ]<<10) & 0x00000C00)
        | ((s_dol_state.st_run.oc[I_DAC]<<8)  & 0x00000300)
        | ((s_dol_state.st_run.oc[I_LIN]<<6)  & 0x000000C0)
        | ((s_dol_state.st_run.oc[I_HP ]<<4)  & 0x00000030)
        | ((s_dol_state.st_run.oc[I_LO ]<<2)  & 0x0000000C)
        | ((s_dol_state.st_run.oc[I_DAC_MUTE ]<<0)  & 0x00000003)
        ));
    
}

////////// START OF codec STATE FUNTION //////////////////////////////////////////
LOCAL void _dol_change_state(DOL_DOL_STATE_E st)
{
    SCI_ASSERT(st<MAX_S); /*assert verified*/
#ifdef DOL_DEBUG
    //DOL_STATE_DGB:"[DOLST]change state pre=%s,cur=%s,set=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1647_112_2_17_23_2_44_143,(uint8*)"sss",_dol_s_str_tbl[s_dol_state.st_run.pre_st],_dol_s_str_tbl[s_dol_state.st_run.cur_st],_dol_s_str_tbl[st]);
#endif
    AUDIO_HAL_PRTMEM ( (0x55550000 
        | ((s_dol_state.st_run.pre_st<<10) & 0x00007C00)
        | ((s_dol_state.st_run.cur_st<<5) & 0x00000370)
        | ((st<<0) & 0x0000001F)
        ));
    s_dol_state.st_run.pre_st = s_dol_state.st_run.cur_st;
    DOL_G_LOCK();
    s_dol_state.st_run.cur_st = st;
    DOL_G_UNLOCK();
}

LOCAL void _dol_init_state(void)
{
    SCI_MEMSET(&(s_dol_state.st_run),0,sizeof(s_dol_state.st_run));
}

LOCAL void _dol_timer_wait_callback(uint32 tmp)
{
    SCI_DeactiveTimer(s_dol_state.timer);
    _set_waiting(0);
    DOL_SM_Process();
}

LOCAL uint32 _dol_timer_wait(uint32 time)
{
    if( time )
    {
        _set_waiting(1);
        // start timer.
        SCI_ChangeTimer(s_dol_state.timer, _dol_timer_wait_callback, time);
        SCI_ActiveTimer(s_dol_state.timer);
        return EXIT_RUNNING;
    }
    return RUN_TO_NEXT;
}

LOCAL void _dol_timer_wait_cancel()
{
    if( _is_waiting() )
    {
        SCI_DeactiveTimer(s_dol_state.timer);
        _set_waiting(0);
    }
}



///timer task call this ! need protect.
LOCAL void _dol_timer_delay_callback(uint32 tmp)
{
    SCI_DeactiveTimer(s_dol_state.timer);
    _set_delaing(0);
    DOL_SM_Process();
}


LOCAL uint32 _dol_timer_delay(uint32 time)
{
    if( time )
    {
        _set_delaing(1);
        // start timer.
        SCI_ChangeTimer(s_dol_state.timer, _dol_timer_delay_callback, time);
        SCI_ActiveTimer(s_dol_state.timer);
        return EXIT_RUNNING;
    }
    return RUN_TO_NEXT;
}

LOCAL __inline void _dol_on_off_set(DOL_STATE_ONOFF_E *pSrc,uint32 onoff)
{
    *pSrc = onoff;
}

LOCAL  __inline void _dol_oc_set(DOL_STATE_OC_E *pSrc,uint32 oc)
{
    *pSrc = oc;
}

LOCAL  __inline uint32 _dol_is_oc(DOL_STATE_OC_E *pSrc,uint32 oc)
{
    return (*pSrc == oc);
}



LOCAL void _dol_notify_data(uint32 need)
{
    DOL_G_LOCK();
    if(need)
    {
        ///TODO: need send data.
        s_dol_state.is_ready = 1;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_SEND_READY);
    }
    else
    {
        ///TODO: maybe can sotp send data.
        s_dol_state.is_ready = 0;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_SEND_Z);
    }
    DOL_G_UNLOCK();
}

LOCAL void _dol_stop_all(uint32 is_open)
{
    ///TODO: STOP ALL.
    SCI_DeactiveTimer(s_dol_state.timer);
    if( !is_open )
    {
        _dol_init_state();
    }
    else
    {
        SCI_MEMSET(&(s_dol_state.st_run.oc),0,sizeof(s_dol_state.st_run.oc));
    }


    __run_callback(is_open);
    __dol_reg_print();
}

LOCAL void _dol_task_init(void)
{
    if( AUDIO_MAGIC_NUM != s_dol_state.magic_num )
    {
        s_dol_state.magic_num = AUDIO_MAGIC_NUM;
        SCI_MEMCPY(s_dol_state.magic_str,AUDIO_MAGIC_STR,AUDIO_MAGIC_STR_MAX_LEN);
        
        _dol_init_state();
        s_dol_state.timer = SCI_CreatePeriodTimer ("_DOL_SM_TIMER",
                           _dol_timer_delay_callback, 0, 5, SCI_NO_ACTIVATE);
#if (USE_EVENT_TRIGGER)
        s_dol_state.event = SCI_CreateEvent("_DOL_SM_EVENT");
#endif
    }
}

LOCAL  void _dol_send_msg(uint32 msg)
{
    xSignalHeader   signal;

    SCI_CREATE_SIGNAL(
        signal,
        msg,
        sizeof(*signal),
        SCI_IDENTIFY_THREAD());

    SCI_SEND_SIGNAL(signal,s_dol_state.task_id);
}

/*****************************************************************************/
//  Description:    Dolphin task entry.
//  Author:         ken.kuang
//  Note:
/*****************************************************************************/
LOCAL void _dol_thread (uint32 argc, void *argv)
{
    xSignalHeader	signal;
    xSignalHeader	peek_signal;

    _dol_task_init();

    for(;;)
    {
        SCI_RECEIVE_SIGNAL(signal, s_dol_state.task_id);
        //**PEEK MSG**
        SCI_PEEK_SIGNAL(peek_signal, s_dol_state.task_id);
        while( peek_signal )
        {
            SCI_FREE_SIGNAL(peek_signal);
            SCI_PEEK_SIGNAL(peek_signal, s_dol_state.task_id);
        }
        //**PEEK MSG**
        switch(signal->SignalCode)
        {
            case DOL_START_RUN:
            {
                _run_dol_state();
            }break;
            //// DOT NOT ADD MSG!!!!
            //// OR, TACK CARE **PEEK MSG**
            default:
            {
                SCI_PASSERT (0,("Error msg!!!"));   /*assert verified*/
            }break;

        }
        SCI_FREE_SIGNAL(signal);
        signal = SCI_NULL;
    }
}


PUBLIC void __dol_create_task(void)
{
    static uint32 do_this = 1;
    if( do_this )
    {
        s_dol_state.task_id  = SCI_CreateThread(DOL_TASK_NAME,
            DOL_QNAME_NAME,
            _dol_thread ,
            0,
            0,
            DOL_STACK_SIZE ,
            DOL_QUEUE_NUM,
            DOL_TASK_PRIV,
            SCI_PREEMPT,
            SCI_AUTO_START);
        do_this = 0;
    }
}

LOCAL void _dol_get_oc(DOL_STATE_OC_E *p_dol_oc,uint32 func_flag)
{
    uint32 index = 0;
    for(index=0;index<_dol_state_tbl_len;index++)
    {
        if( func_flag==_dol_state_tbl[index].func_flag )
        {
            //DOL_STATE_DGB:"[DOLST]index=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOL_STATE_V1_1861_112_2_17_23_2_45_144,(uint8*)"d",index);
            SCI_MEMCPY(p_dol_oc,&(_dol_state_tbl[index].dol_oc),sizeof(_dol_state_tbl[index].dol_oc));
            return;
        }
    }
    SCI_ASSERT(0); /*assert verified*/
}

LOCAL void _dol_change_oc_flag(DOL_STATE_OC_E * p_dol_oc)
{
    uint32 index = 0;
    for(index=0;index<I_MAX;index++)
    {
        if( p_dol_oc[index] == D_OPEN )
        {
            p_dol_oc[index] = D_CLOSE;
        }
        else if( p_dol_oc[index] == D_CLOSE )
        {
            SCI_ASSERT(0); /*assert verified*/
        }
    }
}

LOCAL void _dol_mute_all(void)
{
    __dol_ext_pa_ctl(0);
    __dol_hp_mute(1);
    __dol_ep_mute(1);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


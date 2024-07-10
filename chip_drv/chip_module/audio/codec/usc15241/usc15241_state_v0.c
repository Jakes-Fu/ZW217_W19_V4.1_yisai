/******************************************************************************
 ** File Name:      usc15241_state_v0.c                                       *
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

#include "usc15241_state.h"
#include "usc15241.h"
#include "sprd_codec_dp_phy.h"
#include "audio_drvapi.h"
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
 *  CONFIG MOCRO USC15241_CLOSE_WAIT_TIME for you system.
*/
#define CLOSE_WAIT_TEST         1

/*
 * use task event process ATOM op.
*/
#define USE_EVENT_TRIGGER       1


#ifdef CODEC_DEBUG
#define USC15241_STATE_DGB           SCI_TRACE_LOW
#define USC15241_CHECK_MAX_CNT_ASS   SCI_ASSERT /*assert verified*/
#else
#define USC15241_STATE_DGB(...)
#define USC15241_CHECK_MAX_CNT_ASS(x)
#endif
#define USC15241_STATE_PRT           SCI_TRACE_LOW

#define USC15241_G_LOCK()            do{ SCI_DisableIRQ(); }while(0)
#define USC15241_G_UNLOCK()          do{ SCI_RestoreIRQ(); }while(0)


//uint (x) ms
#define USC15241_LDO_OPEN_TIME       (5)
#define USC15241_RESET_TIME          (5)

//ken.kuang this delay maybe in 10-120.
//this use max value for most chip.
#define USC15241_TSBYU_TIME          (1) //sb-->sb_sleep
#define USC15241_TSHADC_TIME         (1)
#define USC15241_TSHDAC_TIME         (1)
#define USC15241_TSBOUT_TIME         (1)
#define USC15241_TDACMUTE_TIME       (1)


#define USC15241_RUPD_DET_TIME       (1)
#define USC15241_UPD_DET_TIME        (1)

#define USC15241_RUPD_DET_MAX_CNT    (1)  //(290+26*10)=550 ms
#define USC15241_UPD_DET_MAX_CNT     (1)  //(20+16*5)=100 ms


#if CLOSE_WAIT_TEST
#define USC15241_CLOSE_WAIT_TIME     (1125)
#endif

#define USC15241_TASK_NAME           "USC15241_TASK"
#define USC15241_QNAME_NAME          "Q_USC15241_TASK"
#define USC15241_STACK_SIZE          (2*1024)
#define USC15241_QUEUE_NUM           (50)
#define USC15241_TASK_PRIV           (PRI_CODEC_DRV_TASK) //ken.kuang: this pri < AUDIO task pri.

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef uint32 (*RUN_USC15241_S_FUN)(uint32);

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
LOCAL uint32 _run_da_clk_s(uint32 param);
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
    DA_CLK_S,
    DAC_LIN_S,
    MIX_S,
    LOUT_HP_S,
    MUTE_S,
#if CLOSE_WAIT_TEST
    CLOSE_WAIT_S,
#endif
    MAX_S
} USC15241_USC15241_STATE_E;
//1: ORDER IMPORTANT!!
//2: same order like USC15241_USC15241_STATE_E
LOCAL const RUN_USC15241_S_FUN _usc15241_s_fun_tbl[MAX_S] = 
{
    (RUN_USC15241_S_FUN) _run_off_s,
    (RUN_USC15241_S_FUN) _run_pwr_s,
    (RUN_USC15241_S_FUN) _run_slp_s,
    (RUN_USC15241_S_FUN) _run_comm_on_s,
    (RUN_USC15241_S_FUN) _run_on_stop_s,
    (RUN_USC15241_S_FUN) _run_ad_s,
    (RUN_USC15241_S_FUN) _run_lin_rec_s,
    (RUN_USC15241_S_FUN) _run_da_s,
    (RUN_USC15241_S_FUN) _run_da_clk_s,
    (RUN_USC15241_S_FUN) _run_dac_lin_s,
    (RUN_USC15241_S_FUN) _run_mix_s,
    (RUN_USC15241_S_FUN) _run_lout_hp_s,
    (RUN_USC15241_S_FUN) _run_mute_s,
#if CLOSE_WAIT_TEST
    (RUN_USC15241_S_FUN) _run_close_wait_s,
#endif
};

#ifdef CODEC_DEBUG
//1: ORDER IMPORTANT!!
LOCAL char* _usc15241_s_str_tbl[MAX_S] = 
{
    "OFF_S",
    "PWR_S",
    "SLP_S",
    "COMM_ON_S",
    "ON_STOP_S",
    "AD_S",
    "LIN_REC_S",
    "DA_S",
    "DA_CLK_S",
    "DAC_LIN_S",
    "MIX_S",
    "LOUT_HP_S",
    "MUTE_S",
#if CLOSE_WAIT_TEST
    "CLOSE_WAIT_S",
#endif
};
#endif


/*
 * TASK MSG.
*/
typedef enum {
    USC15241_START_RUN = 1,
    USC15241_MAX_MSG
} USC15241_TASK_MSG_E;

typedef enum
{
    D_OFF = 0,
    D_ON
} USC15241_STATE_ONOFF_E;

typedef enum
{
    D_NOTHING = 0,
    D_OPEN,
    D_CLOSE
} USC15241_STATE_OC_E;

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
} USC15241_STATE_OC_INDEX_E;


#if (USE_EVENT_TRIGGER)
enum{
    USC15241_RUNING     =   0x1,
    USC15241_DELAING    =   0x2,
    USC15241_WAITING    =   0x4,
    USC15241_LOCKING    =   0x8,
    USC15241_MAX_EVENT
};
#endif

typedef struct
{
    //DA
    USC15241_STATE_ONOFF_E       st;            // DA
    USC15241_STATE_ONOFF_E       lin_st;        // LINEIN
    USC15241_STATE_ONOFF_E       dac_st;        // DAC
    USC15241_STATE_ONOFF_E       hp_st;         // hp path flag.
    USC15241_STATE_ONOFF_E       lo_st;         // lo path flag.
    USC15241_STATE_ONOFF_E       unmute_st;     // DAC MUTE.
}USC15241_DA_STATE_T;

typedef struct
{
    //AD
    USC15241_STATE_ONOFF_E       st;            // AD
    USC15241_STATE_ONOFF_E       liR_st;        // LINEINREC
}USC15241_AD_STATE_T;


typedef struct
{
    USC15241_STATE_OC_E          ch[I_MAX];
    USC15241_STATE_OC_E          oc[I_MAX];
    USC15241_DA_STATE_T          da;
    USC15241_AD_STATE_T          ad;
    // aux val
#if !(USE_EVENT_TRIGGER)
    uint32                  locking;
    uint32                  runing;
    uint32                  delaing;
    uint32                  waiting;
#endif
    USC15241_USC15241_STATE_E         cur_st;
    USC15241_USC15241_STATE_E         pre_st;
    uint32                  st_2;
    uint32                  delay_cnt;

}USC15241_STATE_RUN_T;

typedef struct{
    uint32                  func_flag;
    USC15241_STATE_OC_E          usc15241_oc[I_MAX];
}USC15241_STATE_TBL_T;


typedef struct
{
    USC15241_STATE_RUN_T         st_run;
#if (USE_EVENT_TRIGGER)
    SCI_EVENT_GROUP_PTR     event;
#endif
    SCI_TIMER_PTR           timer;
    BLOCK_ID                task_id;
    BOOLEAN                 is_ready;
} USC15241_STATE_T;

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

LOCAL USC15241_STATE_T s_usc15241_state = {0};

LOCAL const USC15241_STATE_TBL_T _usc15241_state_tbl[]=
{//         fun_flg     , {I_DA     ,I_LIN  ,I_DAC  ,I_HP   ,I_LO   ,I_DAC_MUTE ,I_LIR  ,I_AD   }
    {USC15241_FUN_PLAYBACK   , {D_OPEN   ,0      ,D_OPEN ,0      ,0      ,0          ,0      ,0      }},
    {USC15241_FUN_CAPTURE    , {0        ,0      ,0      ,0      ,0      ,0          ,D_OPEN ,D_OPEN }},
    {USC15241_FUN_LINEIN     , {D_OPEN   ,D_OPEN ,0      ,0      ,0      ,0          ,0      ,0      }},
    {USC15241_FUN_LINEIN_REC , {0        ,0      ,0      ,0      ,0      ,0          ,D_OPEN ,D_OPEN }},
    {USC15241_FUN_HP         , {0        ,0      ,0      ,D_OPEN ,0      ,0          ,0      ,0      }},
    {USC15241_FUN_LOUT       , {0        ,0      ,0      ,0      ,D_OPEN ,0          ,0      ,0      }},
    {USC15241_FUN_DAC_UNMUTE , {0        ,0      ,0      ,0      ,0      ,D_OPEN     ,0      ,0      }},
};
#define _usc15241_state_tbl_len (sizeof(_usc15241_state_tbl)/sizeof(_usc15241_state_tbl[0]))

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
LOCAL __inline uint32 _usc15241_is_oc(USC15241_STATE_OC_E *pSrc,uint32 oc);
LOCAL __inline void   _usc15241_oc_set(USC15241_STATE_OC_E *pSrc,uint32 oc);
LOCAL __inline void   _usc15241_on_off_set(USC15241_STATE_ONOFF_E *pSrc,uint32 onoff);
LOCAL void   _usc15241_apply_change(void);
LOCAL void   _usc15241_change_state(USC15241_USC15241_STATE_E st);
LOCAL void   _usc15241_init_state(void);
LOCAL uint32 _usc15241_timer_wait(uint32 time);
LOCAL void   _usc15241_timer_wait_cancel(void);
LOCAL uint32 _usc15241_timer_delay(uint32 time);
LOCAL void   _usc15241_notify_data(uint32 need);
LOCAL void   _usc15241_stop_all(uint32 is_open);
LOCAL void   _usc15241_merge_change(void *to,const void* from);

LOCAL  void _usc15241_send_msg(uint32 msg);

LOCAL void  _usc15241_get_oc(USC15241_STATE_OC_E *p_usc15241_oc,uint32 func_flag);
LOCAL void  _usc15241_change_oc_flag(USC15241_STATE_OC_E * p_usc15241_oc);
LOCAL void  _usc15241_mute_all(void);


#define IS_OPEN(v)      (_usc15241_is_oc((USC15241_STATE_OC_E*)&(v),D_OPEN))
#define IS_CLOSE(v)     (_usc15241_is_oc((USC15241_STATE_OC_E*)&(v),D_CLOSE))
#define IS_ON(v)        (v==D_ON)
#define IS_OFF(v)       (v==D_OFF)

#define S_USC15241_M         (s_usc15241_state.st_run)

//ALL CONDITION

//COMM
#define O_CONDI          ((AD_O) || (DA_O))                      //T01
//#define C_CONDI        ((AD_C) && (DA_C))                      //T02

//AD
#define AD_O             (IS_OPEN(S_USC15241_M.oc[I_AD]))             //T03
#define AD_C             (IS_CLOSE(S_USC15241_M.oc[I_AD]))            //T04
#define LIN_REC_O        (IS_OPEN(S_USC15241_M.oc[I_LIR]))            //T05
#define LIN_REC_C        (IS_CLOSE(S_USC15241_M.oc[I_LIR]))           //T06

//DA
#define DA_O             ((IS_OPEN(S_USC15241_M.oc[I_DA])))           //T07
#define DA_C             ((IS_CLOSE(S_USC15241_M.oc[I_DA])))          //T08

#define DAC_O            ((IS_OPEN(S_USC15241_M.oc[I_DAC])))          //T09
#define DAC_C            ((IS_CLOSE(S_USC15241_M.oc[I_DAC])))         //T10
#define LIN_O            ((IS_OPEN(S_USC15241_M.oc[I_LIN])))          //T11
#define LIN_C            ((IS_CLOSE(S_USC15241_M.oc[I_LIN])))         //T12
#define LOUT_O           ((IS_OPEN(S_USC15241_M.oc[I_LO])))           //T13
#define LOUT_C           ((IS_CLOSE(S_USC15241_M.oc[I_LO])))          //T14
#define HP_O             ((IS_OPEN(S_USC15241_M.oc[I_HP])))           //T15
#define HP_C             ((IS_CLOSE(S_USC15241_M.oc[I_HP])))          //T16

#define DACUNMUTE        ((IS_OPEN(S_USC15241_M.oc[I_DAC_MUTE])))     //T17
#define DACMUTE          ((IS_CLOSE(S_USC15241_M.oc[I_DAC_MUTE])))    //T18


#define AD_ON            (IS_ON(S_USC15241_M.ad.st))                  //T22
#define AD_OF            (IS_OFF(S_USC15241_M.ad.st))                 //T23
#define LIN_REC_ON       (IS_ON(S_USC15241_M.ad.liR_st))              //T24
#define LIN_REC_OF       (IS_OFF(S_USC15241_M.ad.liR_st))             //T25

#define DA_ON            (IS_ON(S_USC15241_M.da.st))                  //T26
#define DA_OF            (IS_OFF(S_USC15241_M.da.st))                 //T27
#define HP_ON            (IS_ON(S_USC15241_M.da.hp_st))               //T28
#define HP_OF            (IS_OFF(S_USC15241_M.da.hp_st))              //T29
#define LOUT_ON          (IS_ON(S_USC15241_M.da.lo_st))               //T30
#define LOUT_OF          (IS_OFF(S_USC15241_M.da.lo_st))              //T31
#define DAC_ON           (IS_ON(S_USC15241_M.da.dac_st))              //T32
#define DAC_OF           (IS_OFF(S_USC15241_M.da.dac_st))             //T33
#define LIN_ON           (IS_ON(S_USC15241_M.da.lin_st))              //T34
#define LIN_OF           (IS_OFF(S_USC15241_M.da.lin_st))             //T35
#define FROM_COMM_STATE  (S_USC15241_M.pre_st==COMM_ON_S)             //T36
#define DAC_UNMUTE       (IS_ON(S_USC15241_M.da.unmute_st))           //T37
#define DAC_MUTE         (IS_OFF(S_USC15241_M.da.unmute_st))          //T38

#define A_CONDI ((!(LIN_C && DAC_C)) && ((DAC_ON && ((LIN_O && LIN_OF) ||(LIN_C && LIN_ON))) || (LIN_ON && ((DAC_O && DAC_OF) ||(DAC_C && DAC_ON)))))
#define B_CONDI ((!(DA_C)) && (HP_O || LOUT_O || (HP_C || LOUT_C)))
//#define B_CONDI ((!(HP_C && LOUT_C)) && ((HP_ON && ((LOUT_O && LOUT_OF) ||(LOUT_C && LOUT_ON))) || (LOUT_ON && ((HP_O && HP_OF) ||(HP_C && HP_ON)))))
//ALL CONDITION END.

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
PUBLIC void USC15241_SM_Process(void)
{
    //USC15241_STATE_DGB:"[USC15241ST]USC15241_SM_Process"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_414_112_2_17_23_3_6_287,(uint8*)"");
    AUDIO_HAL_PRTMEM ( 0xAAAAFFFF);
    if( (!__usc15241_is_arm_ctl()) )
    {
        //USC15241_STATE_PRT:"[USC15241ST]arm not run"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_418_112_2_17_23_3_6_288,(uint8*)"");
        return;
    }
    //send msg to task for start.
    _usc15241_send_msg(USC15241_START_RUN);
}

PUBLIC BOOLEAN USC15241_IsReady(void)
{
    BOOLEAN ret = 0;
    USC15241_G_LOCK();
    ret = s_usc15241_state.is_ready;
    USC15241_G_UNLOCK();
    return ret;
}

PUBLIC BOOLEAN USC15241_IsOnStop(void)
{
    BOOLEAN ret = 0;
    USC15241_G_LOCK();
    ret = ((s_usc15241_state.st_run.cur_st==ON_STOP_S));
    USC15241_G_UNLOCK();
    return ret;
}

PUBLIC BOOLEAN USC15241_IsRuning(void)
{
    BOOLEAN ret = 0;
    USC15241_G_LOCK();
    ret = !((s_usc15241_state.st_run.cur_st==OFF_S) || (s_usc15241_state.st_run.cur_st==ON_STOP_S));
    USC15241_G_UNLOCK();
    return ret;
}

PUBLIC void USC15241_ChangeSt(uint32 func_flag,uint32 is_close)
{
    USC15241_STATE_OC_E  use_usc15241_oc[I_MAX];
    
    _usc15241_get_oc(use_usc15241_oc,func_flag);
    
    if(is_close)
    {
        _usc15241_change_oc_flag(use_usc15241_oc);
    }
    
    USC15241_G_LOCK();
    _usc15241_merge_change(&(s_usc15241_state.st_run.ch),use_usc15241_oc);
    USC15241_G_UNLOCK();
    
    //USC15241_STATE_DGB:"[USC15241ST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_476_112_2_17_23_3_6_289,(uint8*)"dddddddd",s_usc15241_state.st_run.ch[I_AD],s_usc15241_state.st_run.ch[I_LIR],s_usc15241_state.st_run.ch[I_DA],s_usc15241_state.st_run.ch[I_DAC],s_usc15241_state.st_run.ch[I_LIN],s_usc15241_state.st_run.ch[I_HP],s_usc15241_state.st_run.ch[I_LO],s_usc15241_state.st_run.ch[I_DAC_MUTE]);
    AUDIO_HAL_PRTMEM ( (0xCCC50000 
        | ((s_usc15241_state.st_run.ch[I_AD ]<<14) & 0x0000C000)
        | ((s_usc15241_state.st_run.ch[I_LIR]<<12) & 0x00003000)
        | ((s_usc15241_state.st_run.ch[I_DA ]<<10) & 0x00000C00)
        | ((s_usc15241_state.st_run.ch[I_DAC]<<8)  & 0x00000300)
        | ((s_usc15241_state.st_run.ch[I_LIN]<<6)  & 0x000000C0)
        | ((s_usc15241_state.st_run.ch[I_HP ]<<4)  & 0x00000030)
        | ((s_usc15241_state.st_run.ch[I_LO ]<<2)  & 0x0000000C)
        | ((s_usc15241_state.st_run.ch[I_DAC_MUTE ]<<2)  & 0x00000003)
        ));
}

LOCAL void _run_usc15241_state(void)
{
    RUN_USC15241_S_FUN fun = 0;
    if( _is_runing() || _is_delaing() )
    {
        return;
    }
    _usc15241_timer_wait_cancel();

    SCI_ASSERT( __usc15241_is_arm_ctl() ); /*assert verified*/

    _set_runing(1);
    //if return  FALSE means s_usc15241_state state off
    //   or need delay , when delay complete will start this.
    // if return TRUE means s_usc15241_state state should run next state.
    do{
        _usc15241_apply_change();
        fun = (RUN_USC15241_S_FUN)(_usc15241_s_fun_tbl[(s_usc15241_state.st_run.cur_st)]);
    }while( fun((uint32)&(s_usc15241_state.st_run)) );
    _set_runing(0);
}


LOCAL uint32 _run_off_s(uint32 param)
{
    if( O_CONDI )
    {
        _usc15241_change_state(PWR_S);
    }
    else //if( C_CONDI )
    {
        return SCI_FALSE;
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_pwr_s(uint32 param)
{
    static uint32 is_o = 0;
    if( O_CONDI )
    {
        if( !is_o )
        {
            switch(s_usc15241_state.st_run.st_2)
            {
                case 0:
                {
                    //LDO_TurnOnLDO (LDO_LDO_AVBO);
                    s_usc15241_state.st_run.st_2 = 1;
                    return _usc15241_timer_delay(USC15241_LDO_OPEN_TIME);
                }break;/*lint !e527 comfirmed by windy.wang*/
                case 1:
                {
                    __usc15241_en(1);
                    s_usc15241_state.st_run.st_2 = 2;
                    return _usc15241_timer_delay(USC15241_RESET_TIME);
                }break;/*lint !e527 comfirmed by windy.wang*/
                case 2:
                {
                    s_usc15241_state.st_run.st_2 = 3;
                    return _usc15241_timer_delay(USC15241_TSBYU_TIME);
                }break;/*lint !e527 comfirmed by windy.wang*/
                case 3:
                {
                    s_usc15241_state.st_run.st_2 = 0;
                    is_o = 1;
                }break;
                default:break;
            }
        }
        _usc15241_change_state(SLP_S);
    }
    else// if( C_CONDI )
    {
        if( is_o )
        {
            __usc15241_en(0);
            //LDO_TurnOffLDO (LDO_LDO_AVBO);
            is_o = 0;
        }
        s_usc15241_state.st_run.st_2 = 0;
        _usc15241_stop_all(0);
        _usc15241_change_state(OFF_S);
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_slp_s(uint32 param)
{
    if( DA_O )
    {
        _usc15241_change_state(DA_S);
    }
    else if( AD_O )
    {
        _usc15241_change_state(AD_S);
    }
    else //if( C_CONDI )
    {
        _usc15241_change_state(PWR_S);
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_comm_on_s(uint32 param)
{
    if( (LIN_REC_C && LIN_REC_ON) || (LIN_REC_O && LIN_REC_OF && AD_ON ) )
    {
        _usc15241_change_state(LIN_REC_S);
    }
    else if( (AD_C && AD_ON) || (AD_O && AD_OF ) || (LIN_REC_O && AD_OF) )
    {
        _usc15241_change_state(AD_S);
    }
    else if( (DA_O && DA_OF ) )
    {
        _usc15241_change_state(DA_S);
    }
    else if( DA_ON && (A_CONDI) )
    {
        _usc15241_change_state(DAC_LIN_S);
    }
    else if( DA_ON && (B_CONDI) )
    {
        _usc15241_change_state(LOUT_HP_S);
    }
    else if( ((DACMUTE || DACUNMUTE)&& DAC_ON && DA_ON) || (DA_C && DA_ON ) )
    {
        _usc15241_change_state(MUTE_S);
    }
    else
    {
        _usc15241_change_state(ON_STOP_S);
        _usc15241_stop_all(1);
        _usc15241_notify_data(1);
    }
    return SCI_TRUE;
}

#if CLOSE_WAIT_TEST
LOCAL uint32 _run_close_wait_s(uint32 param)
{
    if( (DA_C && DA_ON ) )
    {
        _usc15241_change_state(LOUT_HP_S);
        return _usc15241_timer_wait(USC15241_CLOSE_WAIT_TIME);
    }
    else if (DA_O)
    {
        _usc15241_change_state(COMM_ON_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}
#endif


LOCAL uint32 _run_on_stop_s(uint32 param)
{
#ifdef CODEC_DEBUG
    //USC15241_STATE_DGB:"[USC15241ST]on_stop pre=%s,cur=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_655_112_2_17_23_3_6_290,(uint8*)"ss",_usc15241_s_str_tbl[s_usc15241_state.st_run.pre_st],_usc15241_s_str_tbl[s_usc15241_state.st_run.cur_st]);
#endif
    AUDIO_HAL_PRTMEM ( (0x555A0000 
        | ((s_usc15241_state.st_run.pre_st<<8) & 0x0000FF00)
        | ((s_usc15241_state.st_run.cur_st<<0) & 0x000000FF)
        ));
    if( (__usc15241_is_arm_ctl()) && (((O_CONDI || AD_C || DA_C || DAC_O || DAC_C || LIN_O || LIN_C || HP_O || HP_C || LOUT_O || LOUT_C || LIN_REC_O || LIN_REC_C) 
        && (AD_ON || DA_ON )) || DACMUTE || DACUNMUTE)  )
    {
        _usc15241_change_state(COMM_ON_S);
    }
    else
    {
        SCI_MEMSET(&(s_usc15241_state.st_run.oc),0,sizeof(s_usc15241_state.st_run.oc));
        return SCI_FALSE;
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_ad_s(uint32 param)
{
    //static uint32 is_o = 0;
    if( AD_O )
    {
        __usc15241_adc(1,AUDIO_ALL_CHAN);
        _usc15241_on_off_set(&(s_usc15241_state.st_run.ad.st),D_ON);
        if( LIN_REC_O )
        {
            _usc15241_change_state(LIN_REC_S);
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_AD]),D_NOTHING);
            _usc15241_change_state(COMM_ON_S);
        }
    }
    else if( AD_C )
    {
        __usc15241_adc(0,AUDIO_ALL_CHAN);
        _usc15241_on_off_set(&(s_usc15241_state.st_run.ad.st),D_OFF);
        _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_AD]),D_NOTHING);
        if( DA_ON )
        {
            _usc15241_change_state(COMM_ON_S);
        }
        else if( DA_O )
        {
            _usc15241_change_state(DA_S);
        }
        else //if( !(DA_O || liO) )
        {
            _usc15241_change_state(SLP_S);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_lin_rec_s(uint32 param)
{
    //static uint32 is_o = 0;
    if( LIN_REC_O )
    {
        __usc15241_linein_rec(1,AUDIO_ALL_CHAN);
        _usc15241_on_off_set(&(s_usc15241_state.st_run.ad.liR_st),D_ON);
        s_usc15241_state.st_run.oc[I_AD] = s_usc15241_state.st_run.oc[I_LIR] = D_NOTHING;//_usc15241_oc_set
        _usc15241_change_state(COMM_ON_S);
    }
    else if( LIN_REC_C )
    {
        __usc15241_linein_rec(0,AUDIO_ALL_CHAN);
        _usc15241_on_off_set(&(s_usc15241_state.st_run.ad.liR_st),D_OFF);
        _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LIR]),D_NOTHING);
        _usc15241_change_state(AD_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_da_s(uint32 param)
{
    //static uint32 is_o = 0;
    _usc15241_on_off_set(&(s_usc15241_state.st_run.da.st),D_OFF);
    if( DA_O )
    {
        _usc15241_mute_all();
        _usc15241_change_state(DA_CLK_S);
    }
    else if( DA_C  )
    {
        _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DA]),D_NOTHING);
        if( AD_ON )
        {
            _usc15241_change_state(COMM_ON_S);
        }
        else if( AD_O )
        {
            _usc15241_change_state(AD_S);
        }
        else
        {
            _usc15241_change_state(SLP_S);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}


LOCAL uint32 _run_da_clk_s(uint32 param)
{
    if( DA_O )
    {
        __usc15241_dac_clk_en(1);
        _usc15241_change_state(DAC_LIN_S);
    }
    else if( DA_C  )
    {
        __usc15241_dac_clk_en(0);
        _usc15241_change_state(DA_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}


LOCAL uint32 _run_dac_s(uint32 param)
{
    static uint32 is_o = 0;
    if( DAC_O )
    {
        if( !is_o )
        {
            __usc15241_dac(1,AUDIO_ALL_CHAN);
            _usc15241_on_off_set(&(s_usc15241_state.st_run.da.dac_st),D_ON);
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DAC]),D_NOTHING);
            is_o = 1;
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DAC]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else if( DAC_C )
    {
        if( is_o )
        {
            __usc15241_dac(0,AUDIO_ALL_CHAN);
            _usc15241_on_off_set(&(s_usc15241_state.st_run.da.dac_st),D_OFF);
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DAC]),D_NOTHING);
            is_o = 0;
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DAC]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else
    {
        _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DAC]),D_NOTHING);
        return NO_OPRATE;
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_lin_s(uint32 param)
{
    static uint32 is_o = 0;
    if( LIN_O )
    {
        if( !is_o )
        {
            //__usc15241_lin(1,0);
            _usc15241_on_off_set(&(s_usc15241_state.st_run.da.lin_st),D_ON);
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LIN]),D_NOTHING);
            is_o = 1;
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LIN]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else if( LIN_C )
    {
        if( is_o )
        {
            //__usc15241_lin(0,0);
            _usc15241_on_off_set(&(s_usc15241_state.st_run.da.lin_st),D_OFF);
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LIN]),D_NOTHING);
            is_o = 0;
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LIN]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else
    {
        _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LIN]),D_NOTHING);
        return NO_OPRATE;
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_dac_lin_s(uint32 param)
{
    uint32 ret = 0;
    ret = _run_dac_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_usc15241_state.st_run.st_2) )
    {
        return ret;
    }
    ret = _run_lin_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_usc15241_state.st_run.st_2) )
    {
        return ret;
    }

    if( DA_ON && FROM_COMM_STATE  )
    {
        s_usc15241_state.st_run.oc[I_DA] = s_usc15241_state.st_run.oc[I_DAC] =s_usc15241_state.st_run.oc[I_LIN] = D_NOTHING;//_usc15241_oc_set
        _usc15241_change_state(COMM_ON_S);
    }
    else if( DA_O )
    {
        _usc15241_change_state(MIX_S);
    }
    else if( DA_C )
    {
        _usc15241_change_state(DA_CLK_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}


LOCAL uint32 _run_mix_s(uint32 param)
{
    if( DA_O )
    {
        _usc15241_on_off_set(&(s_usc15241_state.st_run.da.st),D_ON);
        _usc15241_change_state(LOUT_HP_S);
    }
    else if( DA_C  )
    {
        _usc15241_change_state(DAC_LIN_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_lout_s(uint32 param)
{
    static uint32 is_o = 0;
    if( LOUT_O )
    {
        if( !is_o )
        {
            __usc15241_lout(1,AUDIO_ALL_CHAN,(uint32)DAC_ON,(uint32)LIN_ON);
            _usc15241_on_off_set(&(s_usc15241_state.st_run.da.lo_st),D_ON);
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LO]),D_NOTHING);
            is_o = 1;
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LO]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else if( LOUT_C )
    {
        if( is_o )
        {
            __usc15241_lout(0,AUDIO_ALL_CHAN,(uint32)DAC_ON,(uint32)LIN_ON);
            _usc15241_on_off_set(&(s_usc15241_state.st_run.da.lo_st),D_OFF);
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LO]),D_NOTHING);
            is_o = 0;
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LO]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else
    {
        _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_LO]),D_NOTHING);
        return NO_OPRATE;
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_hp_s(uint32 param)
{
    static uint32 is_o = 0;
    if( HP_O )
    {
        if( !is_o )
        {
            __usc15241_hp(1,AUDIO_ALL_CHAN,(uint32)DAC_ON,(uint32)LIN_ON);
            _usc15241_on_off_set(&(s_usc15241_state.st_run.da.hp_st),D_ON);
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_HP]),D_NOTHING);
            is_o = 1;
            
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_HP]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else if( HP_C )
    {
        if( is_o )
        {
            _usc15241_on_off_set(&(s_usc15241_state.st_run.da.hp_st),D_OFF);
            __usc15241_hp(0,AUDIO_ALL_CHAN,(uint32)DAC_ON,(uint32)LIN_ON);
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_HP]),D_NOTHING);
            is_o = 0;
        }
        else
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_HP]),D_NOTHING);
            return NO_OPRATE;
        }
    }
    else
    {
        _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_HP]),D_NOTHING);
        return NO_OPRATE;
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_lout_hp_s(uint32 param)
{
    uint32 ret = 0;
    ret = _run_lout_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_usc15241_state.st_run.st_2) )
    {
        return ret;
    }
    ret = _run_hp_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_usc15241_state.st_run.st_2) )
    {
        return ret;
    }

    if( DA_O || (DA_ON && FROM_COMM_STATE)  )
    {
        s_usc15241_state.st_run.oc[I_DA] = s_usc15241_state.st_run.oc[I_HP] =s_usc15241_state.st_run.oc[I_LO] = D_NOTHING;//_usc15241_oc_set
        _usc15241_change_state(COMM_ON_S);
    }
    else if( DA_C )
    {
        _usc15241_change_state(MIX_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}

LOCAL uint32 _run_mute_s(uint32 param)
{
    if( DACUNMUTE )
    {
        switch(s_usc15241_state.st_run.st_2)
        {
            case 0:
            {
                if( DAC_ON && __sprd_codec_dp_dac_mute(0) )
                {
                    _set_locking(1);
                    s_usc15241_state.st_run.st_2 = 1;
                    s_usc15241_state.st_run.delay_cnt=0;
                    return _usc15241_timer_delay(USC15241_TDACMUTE_TIME);
                }
                else
                {
                    //USC15241_STATE_PRT:"[AUDDRV][USC15241]UnMuted"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1058_112_2_17_23_3_7_291,(uint8*)"");
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
            case 1:
            {
                if( __sprd_codec_dp_gain_done(1) )
                {
                    __sprd_codec_dp_clr_gain(1);
                }
                else if(s_usc15241_state.st_run.delay_cnt>USC15241_UPD_DET_MAX_CNT)
                {
                    //USC15241_STATE_PRT:"[AUDDRV][USC15241]GAIN is UP"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1069_112_2_17_23_3_7_292,(uint8*)"");
                    __usc15241_reg_print();
                    USC15241_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                }
                else
                {
                    s_usc15241_state.st_run.delay_cnt++;
                    return _usc15241_timer_delay(USC15241_UPD_DET_TIME);
                }
                s_usc15241_state.st_run.delay_cnt = 0;
            }break;/*lint !e527 comfirmed by windy.wang*/
            default:break;
        }
        s_usc15241_state.st_run.st_2 = 0;
        _usc15241_on_off_set(&(s_usc15241_state.st_run.da.unmute_st),D_ON);
        _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
        _set_locking(0);
        _usc15241_change_state(COMM_ON_S);
    }
    else if( DACMUTE || DA_C  )
    {
        switch(s_usc15241_state.st_run.st_2)
        {
            case 0:
            {
                if( DAC_ON && __sprd_codec_dp_dac_mute(1)   )
                {
                    _set_locking(1);
                    s_usc15241_state.st_run.st_2 = 3;
                    s_usc15241_state.st_run.delay_cnt=0;
                    return _usc15241_timer_delay(USC15241_TDACMUTE_TIME);
                }
                else
                {
                    //USC15241_STATE_PRT:"[AUDDRV][USC15241]Muted"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1103_112_2_17_23_3_7_293,(uint8*)"");
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
            case 3:
            {
                if( __sprd_codec_dp_gain_done(0) )
                {
                    __sprd_codec_dp_clr_gain(0);
                }
                else if(s_usc15241_state.st_run.delay_cnt>USC15241_UPD_DET_MAX_CNT)
                {
                    //USC15241_STATE_PRT:"[AUDDRV][USC15241]GAIN is DOWN"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1114_112_2_17_23_3_7_294,(uint8*)"");
                    __usc15241_reg_print();
                    USC15241_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                }
                else
                {
                    s_usc15241_state.st_run.delay_cnt++;
                    return _usc15241_timer_delay(USC15241_UPD_DET_TIME);
                }
                s_usc15241_state.st_run.delay_cnt = 0;
            }break;/*lint !e527 comfirmed by windy.wang*/
            default:break;
        }
        s_usc15241_state.st_run.st_2 = 0;
        _usc15241_notify_data(0);
        _usc15241_on_off_set(&(s_usc15241_state.st_run.da.unmute_st),D_OFF);
        if( DA_C )
        {
            _usc15241_mute_all();
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
            _set_locking(0);
#if CLOSE_WAIT_TEST
            _usc15241_change_state(CLOSE_WAIT_S);
#else
            _usc15241_change_state(LOUT_HP_S);
#endif
        }
        else if(DACMUTE)
        {
            _usc15241_oc_set(&(s_usc15241_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
            _set_locking(0);
            _usc15241_change_state(COMM_ON_S);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return SCI_TRUE;
}



#if (USE_EVENT_TRIGGER)
LOCAL uint32 _is_event_set(uint32 flag)
{
    uint32 ret = 0;
    uint32 actual_event;
    if( s_usc15241_state.event )
    {
        ret = SCI_GetEvent(s_usc15241_state.event,
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
    if( s_usc15241_state.event )
    {
        if( set )
        {
            SCI_SetEvent(s_usc15241_state.event, flag, SCI_OR);
        }
        else
        {
            SCI_SetEvent(s_usc15241_state.event, ~(flag), SCI_AND);
        }
    }
}

#endif



LOCAL uint32 _is_runing()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(USC15241_RUNING);
#else
    uint32 ret = SCI_ERROR;
    USC15241_G_LOCK();
    ret = s_usc15241_state.st_run.runing;
    USC15241_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_runing(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,USC15241_RUNING);
#else
    USC15241_G_LOCK();
    s_usc15241_state.st_run.runing = set;
    USC15241_G_UNLOCK();
#endif
}

LOCAL uint32 _is_locking()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(USC15241_LOCKING);
#else
    uint32 ret = SCI_ERROR;
    USC15241_G_LOCK();
    ret = s_usc15241_state.st_run.locking;
    USC15241_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_locking(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,USC15241_LOCKING);
#else
    USC15241_G_LOCK();
    s_usc15241_state.st_run.locking = set;
    USC15241_G_UNLOCK();
#endif
}


LOCAL uint32 _is_delaing()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(USC15241_DELAING);
#else
    uint32 ret = 0;
    USC15241_G_LOCK();
    ret = s_usc15241_state.st_run.delaing;
    USC15241_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_delaing(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,USC15241_DELAING);
#else
    USC15241_G_LOCK();
    s_usc15241_state.st_run.delaing = set;
    USC15241_G_UNLOCK();
#endif
}

LOCAL uint32 _is_waiting()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(USC15241_WAITING);
#else
    uint32 ret = 0;
    USC15241_G_LOCK();
    ret = s_usc15241_state.st_run.waiting;
    USC15241_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_waiting(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,USC15241_WAITING);
#else
    USC15241_G_LOCK();
    s_usc15241_state.st_run.waiting = set;
    USC15241_G_UNLOCK();
#endif
}



LOCAL void _usc15241_stag1_dispel_change(void *to)
{
    USC15241_STATE_OC_E * pDst = (USC15241_STATE_OC_E *)to;

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

LOCAL void _usc15241_stag2_dispel_change(void *to)
{
    USC15241_STATE_OC_E * pDst = (USC15241_STATE_OC_E *)to;

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

LOCAL void _usc15241_stag1_merge_change(void *to,const void* from)
{
    uint32 i=0;
    USC15241_STATE_OC_E * pSrc = (USC15241_STATE_OC_E *)from;
    USC15241_STATE_OC_E * pDst = (USC15241_STATE_OC_E *)to;
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

LOCAL void _usc15241_stag2_merge_change(void *to,const void* from)
{
    USC15241_STATE_OC_E * pSrc = (USC15241_STATE_OC_E *)from;
    USC15241_STATE_OC_E * pDst = (USC15241_STATE_OC_E *)to;
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


LOCAL void _usc15241_merge_change(void *to,const void* from)
{
    _usc15241_stag1_merge_change(to,from);
    _usc15241_stag2_merge_change(to,from);
}


LOCAL void _usc15241_merge_st_change(void *to,const void* from)
{
    _usc15241_stag1_merge_change(to,from);
    
    _usc15241_stag1_dispel_change(to);

    //USC15241_STATE_DGB:"[USC15241ST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1432_112_2_17_23_3_8_295,(uint8*)"dddddddd",s_usc15241_state.st_run.oc[I_AD],s_usc15241_state.st_run.oc[I_LIR],s_usc15241_state.st_run.oc[I_DA],s_usc15241_state.st_run.oc[I_DAC],s_usc15241_state.st_run.oc[I_LIN],s_usc15241_state.st_run.oc[I_HP],s_usc15241_state.st_run.oc[I_LO],s_usc15241_state.st_run.oc[I_DAC_MUTE]);
    
    _usc15241_stag2_merge_change(to,from);

    _usc15241_stag2_dispel_change(to);
}

LOCAL void _usc15241_apply_change(void)
{
    USC15241_STATE_OC_E  use_usc15241_oc[I_MAX];
    if( _is_locking() )
    {
        return;
    }
    //USC15241_STATE_DGB:"[USC15241ST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1455_112_2_17_23_3_8_296,(uint8*)"dddddddd",s_usc15241_state.st_run.ch[I_AD],s_usc15241_state.st_run.ch[I_LIR],s_usc15241_state.st_run.ch[I_DA],s_usc15241_state.st_run.ch[I_DAC],s_usc15241_state.st_run.ch[I_LIN],s_usc15241_state.st_run.ch[I_HP],s_usc15241_state.st_run.ch[I_LO],s_usc15241_state.st_run.ch[I_DAC_MUTE]);
    USC15241_G_LOCK();
    SCI_MEMCPY(&(use_usc15241_oc),&(s_usc15241_state.st_run.ch),sizeof(use_usc15241_oc));//copy all change.
    SCI_MEMSET(&(s_usc15241_state.st_run.ch),0,sizeof(s_usc15241_state.st_run.ch)); //clear all change.
    USC15241_G_UNLOCK();
    _usc15241_merge_st_change(&(s_usc15241_state.st_run.oc),&(use_usc15241_oc));
    //USC15241_STATE_DGB:"[USC15241ST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1470_112_2_17_23_3_8_297,(uint8*)"dddddddd",s_usc15241_state.st_run.oc[I_AD],s_usc15241_state.st_run.oc[I_LIR],s_usc15241_state.st_run.oc[I_DA],s_usc15241_state.st_run.oc[I_DAC],s_usc15241_state.st_run.oc[I_LIN],s_usc15241_state.st_run.oc[I_HP],s_usc15241_state.st_run.oc[I_LO],s_usc15241_state.st_run.oc[I_DAC_MUTE]);
    AUDIO_HAL_PRTMEM ( (0xCCCC0000 
        | ((s_usc15241_state.st_run.oc[I_AD ]<<14) & 0x0000C000)
        | ((s_usc15241_state.st_run.oc[I_LIR]<<12) & 0x00003000)
        | ((s_usc15241_state.st_run.oc[I_DA ]<<10) & 0x00000C00)
        | ((s_usc15241_state.st_run.oc[I_DAC]<<8)  & 0x00000300)
        | ((s_usc15241_state.st_run.oc[I_LIN]<<6)  & 0x000000C0)
        | ((s_usc15241_state.st_run.oc[I_HP ]<<4)  & 0x00000030)
        | ((s_usc15241_state.st_run.oc[I_LO ]<<2)  & 0x0000000C)
        | ((s_usc15241_state.st_run.oc[I_DAC_MUTE ]<<2)  & 0x00000003)
        ));
    
}

////////// START OF codec STATE FUNTION //////////////////////////////////////////
LOCAL void _usc15241_change_state(USC15241_USC15241_STATE_E st)
{
    SCI_ASSERT(st<MAX_S); /*assert verified*/
#ifdef CODEC_DEBUG
    //USC15241_STATE_DGB:"[USC15241ST]change state pre=%s,cur=%s,set=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1492_112_2_17_23_3_8_298,(uint8*)"sss",_usc15241_s_str_tbl[s_usc15241_state.st_run.pre_st],_usc15241_s_str_tbl[s_usc15241_state.st_run.cur_st],_usc15241_s_str_tbl[st]);
#endif
    AUDIO_HAL_PRTMEM ( (0x55550000 
        | ((s_usc15241_state.st_run.pre_st<<10) & 0x00007C00)
        | ((s_usc15241_state.st_run.cur_st<<5) & 0x00000370)
        | ((st<<0) & 0x0000001F)
        ));
    s_usc15241_state.st_run.pre_st = s_usc15241_state.st_run.cur_st;
    USC15241_G_LOCK();
    s_usc15241_state.st_run.cur_st = st;
    USC15241_G_UNLOCK();
}

LOCAL void _usc15241_init_state(void)
{
    SCI_MEMSET(&(s_usc15241_state.st_run),0,sizeof(s_usc15241_state.st_run));
}

LOCAL void _usc15241_timer_wait_callback(uint32 tmp)
{
    SCI_DeactiveTimer(s_usc15241_state.timer);
    _set_waiting(0);
    USC15241_SM_Process();
}

LOCAL uint32 _usc15241_timer_wait(uint32 time)
{
    if( time )
    {
        _set_waiting(1);
        // start timer.
        SCI_ChangeTimer(s_usc15241_state.timer, _usc15241_timer_wait_callback, time);
        SCI_ActiveTimer(s_usc15241_state.timer);
        return SCI_FALSE;
    }
    return SCI_TRUE;
}

LOCAL void _usc15241_timer_wait_cancel()
{
    if( _is_waiting() )
    {
        SCI_DeactiveTimer(s_usc15241_state.timer);
        _set_waiting(0);
    }
}



///timer task call this ! need protect.
LOCAL void _usc15241_timer_delay_callback(uint32 tmp)
{
    SCI_DeactiveTimer(s_usc15241_state.timer);
    _set_delaing(0);
    USC15241_SM_Process();
}


LOCAL uint32 _usc15241_timer_delay(uint32 time)
{
    if( time )
    {
        _set_delaing(1);
        // start timer.
        SCI_ChangeTimer(s_usc15241_state.timer, _usc15241_timer_delay_callback, time);
        SCI_ActiveTimer(s_usc15241_state.timer);
        return SCI_FALSE;
    }
    return SCI_TRUE;
}

LOCAL __inline void _usc15241_on_off_set(USC15241_STATE_ONOFF_E *pSrc,uint32 onoff)
{
    *pSrc = onoff;
}

LOCAL  __inline void _usc15241_oc_set(USC15241_STATE_OC_E *pSrc,uint32 oc)
{
    *pSrc = oc;
}

LOCAL  __inline uint32 _usc15241_is_oc(USC15241_STATE_OC_E *pSrc,uint32 oc)
{
    return (*pSrc == oc);
}



LOCAL void _usc15241_notify_data(uint32 need)
{
    USC15241_G_LOCK();
    if(need)
    {
        ///TODO: need send data.
        s_usc15241_state.is_ready = 1;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_SEND_READY);
    }
    else
    {
        ///TODO: maybe can sotp send data.
        s_usc15241_state.is_ready = 0;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_SEND_Z);
    }
    USC15241_G_UNLOCK();
}

LOCAL void _usc15241_stop_all(uint32 is_open)
{
    ///TODO: STOP ALL.
    SCI_DeactiveTimer(s_usc15241_state.timer);
    if( !is_open )
    {
        _usc15241_init_state();
    }
    else
    {
        SCI_MEMSET(&(s_usc15241_state.st_run.oc),0,sizeof(s_usc15241_state.st_run.oc));
    }


    __run_callback(is_open);
    __usc15241_reg_print();
}

LOCAL void _usc15241_task_init(void)
{
    _usc15241_init_state();
    s_usc15241_state.timer = SCI_CreatePeriodTimer ("_USC15241_SM_TIMER",
                       _usc15241_timer_delay_callback, 0, 5, SCI_NO_ACTIVATE);
#if (USE_EVENT_TRIGGER)
    s_usc15241_state.event = SCI_CreateEvent("_USC15241_SM_EVENT");
#endif
}

LOCAL  void _usc15241_send_msg(uint32 msg)
{
    xSignalHeader   signal;

    SCI_CREATE_SIGNAL(
        signal,
        msg,
        sizeof(*signal),
        SCI_IDENTIFY_THREAD());

    SCI_SEND_SIGNAL(signal,s_usc15241_state.task_id);
}

/*****************************************************************************/
//  Description:    usc15241 task entry.
//  Author:         ken.kuang
//  Note:
/*****************************************************************************/
LOCAL void _usc15241_thread (uint32 argc, void *argv)
{
    xSignalHeader	signal;
    xSignalHeader	peek_signal;

    _usc15241_task_init();

    for(;;)
    {
        SCI_RECEIVE_SIGNAL(signal, s_usc15241_state.task_id);
        //**PEEK MSG**
        SCI_PEEK_SIGNAL(peek_signal, s_usc15241_state.task_id);
        while( peek_signal )
        {
            SCI_FREE_SIGNAL(peek_signal);
            SCI_PEEK_SIGNAL(peek_signal, s_usc15241_state.task_id);
        }
        //**PEEK MSG**
        switch(signal->SignalCode)
        {
            case USC15241_START_RUN:
            {
                _run_usc15241_state();
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


PUBLIC void __usc15241_create_task(void)
{
    static uint32 do_this = 1;
    if( do_this )
    {
        s_usc15241_state.task_id  = SCI_CreateThread(USC15241_TASK_NAME,
            USC15241_QNAME_NAME,
            _usc15241_thread ,
            0,
            0,
            USC15241_STACK_SIZE ,
            USC15241_QUEUE_NUM,
            USC15241_TASK_PRIV,
            SCI_PREEMPT,
            SCI_AUTO_START);
        do_this = 0;
    }
}

LOCAL void _usc15241_get_oc(USC15241_STATE_OC_E *p_usc15241_oc,uint32 func_flag)
{
    uint32 index = 0;
    for(index=0;index<_usc15241_state_tbl_len;index++)
    {
        if( func_flag==_usc15241_state_tbl[index].func_flag )
        {
            //USC15241_STATE_DGB:"[USC15241ST]index=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,USC15241_STATE_V0_1706_112_2_17_23_3_9_299,(uint8*)"d",index);
            SCI_MEMCPY(p_usc15241_oc,&(_usc15241_state_tbl[index].usc15241_oc),sizeof(_usc15241_state_tbl[index].usc15241_oc));
            return;
        }
    }
    SCI_ASSERT(0); /*assert verified*/
}

LOCAL void _usc15241_change_oc_flag(USC15241_STATE_OC_E * p_usc15241_oc)
{
    uint32 index = 0;
    for(index=0;index<I_MAX;index++)
    {
        if( p_usc15241_oc[index] == D_OPEN )
        {
            p_usc15241_oc[index] = D_CLOSE;
        }
        else if( p_usc15241_oc[index] == D_CLOSE )
        {
            SCI_ASSERT(0); /*assert verified*/
        }
    }
}

LOCAL void _usc15241_mute_all(void)
{
    __usc15241_ext_pa_ctl(0);
    __usc15241_ext_hp_pa_ctl(0);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


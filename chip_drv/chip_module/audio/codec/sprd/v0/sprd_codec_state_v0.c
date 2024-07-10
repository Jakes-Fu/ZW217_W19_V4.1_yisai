/******************************************************************************
 ** File Name:      sprd_codec_state_v0.c                                     *
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

#include "audio_drvapi.h"

#include "../sprd_codec_state.h"
#include "../sprd_codec_phy.h"
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
   for external headphone pa close delay
*/
#define CLOSE_WAIT_FOR_EXT_HP_PA_TEST         0

/*
 * when need close codecphin , we wait some time 
 *  then , it will quickly open.
 *  CONFIG MOCRO SPRD_CODEC_CLOSE_WAIT_TIME for you system.
*/
#define CLOSE_WAIT_TEST         0

/*
 * use task event process ATOM op.
*/
#define USE_EVENT_TRIGGER       1



#ifdef CODEC_DEBUG
#define SPRD_CODEC_STATE_DGB           SCI_TRACE_LOW
#define SPRD_CODEC_CHECK_MAX_CNT_ASS   SCI_ASSERT /*assert verified*/
#else
#define SPRD_CODEC_STATE_DGB(...)
#define SPRD_CODEC_CHECK_MAX_CNT_ASS(x)
#endif
#define SPRD_CODEC_STATE_PRT           SCI_TRACE_LOW

#define SPRD_CODEC_G_LOCK()            do{ SCI_DisableIRQ(); }while(0)
#define SPRD_CODEC_G_UNLOCK()          do{ SCI_RestoreIRQ(); }while(0)


//uint (x) ms
#define SPRD_CODEC_TIME_CONT           (0xFFFFFFFF)

#define SPRD_CODEC_RESET_TIME          (5)
#define SPRD_CODEC_VCOM_TIME           (5)

#if (USE_INT_DELAY)
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define SPRD_CODEC_THP_TIMEOUT         (10)
#else
#define SPRD_CODEC_THP_TIMEOUT         (1000)
#endif
#define SPRD_CODEC_TDACMUTE_TIMEOUT    (600)

#define SPRD_CODEC_RUPD_DET_MAX_CNT    (1)

#else
#define SPRD_CODEC_THP_TIME            (320)
#define SPRD_CODEC_TDACMUTE_TIME       (45)


#define SPRD_CODEC_RUPD_DET_TIME       (10)

#define SPRD_CODEC_RUPD_DET_MAX_CNT    (26)  //(320+26*10)=580 ms
#endif

#if CLOSE_WAIT_FOR_EXT_HP_PA_TEST
#define SPRD_CODEC_CLOSE_WAIT_FOR_EXT_HP_TIME     (4000)
#endif
#if CLOSE_WAIT_TEST
#define SPRD_CODEC_CLOSE_WAIT_TIME     (1125)
#endif

#define SPRD_CODEC_TASK_NAME           "SPRD_CODEC_TASK"
#define SPRD_CODEC_QNAME_NAME          "Q_SPRD_CODEC_TASK"
#define SPRD_CODEC_STACK_SIZE          (2*1024)
#define SPRD_CODEC_QUEUE_NUM           (30)
#define SPRD_CODEC_TASK_PRIV           (PRI_CODEC_DRV_TASK) //ken.kuang: this pri < AUDIO task pri.

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

typedef uint32 (*RUN_SPRD_CODEC_S_FUN)(uint32);

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
LOCAL uint32 _run_hp_pop_s(uint32 param);
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
    HP_POP_S,
    LOUT_HP_S,
    MUTE_S,
#if CLOSE_WAIT_TEST
    CLOSE_WAIT_S,
#endif
    MAX_S
}SPRD_CODEC_STATE_E;
//1: ORDER IMPORTANT!!
//2: same order like SPRD_CODEC_STATE_E
LOCAL const RUN_SPRD_CODEC_S_FUN _sprd_codec_s_fun_tbl[MAX_S] = 
{
    (RUN_SPRD_CODEC_S_FUN) _run_off_s,
    (RUN_SPRD_CODEC_S_FUN) _run_pwr_s,
    (RUN_SPRD_CODEC_S_FUN) _run_slp_s,
    (RUN_SPRD_CODEC_S_FUN) _run_comm_on_s,
    (RUN_SPRD_CODEC_S_FUN) _run_on_stop_s,
    (RUN_SPRD_CODEC_S_FUN) _run_ad_s,
    (RUN_SPRD_CODEC_S_FUN) _run_lin_rec_s,
    (RUN_SPRD_CODEC_S_FUN) _run_da_s,
    (RUN_SPRD_CODEC_S_FUN) _run_da_clk_s,
    (RUN_SPRD_CODEC_S_FUN) _run_dac_lin_s,
    (RUN_SPRD_CODEC_S_FUN) _run_mix_s,
    (RUN_SPRD_CODEC_S_FUN) _run_hp_pop_s,
    (RUN_SPRD_CODEC_S_FUN) _run_lout_hp_s,
    (RUN_SPRD_CODEC_S_FUN) _run_mute_s,
#if CLOSE_WAIT_TEST
    (RUN_SPRD_CODEC_S_FUN) _run_close_wait_s,
#endif
};

#ifdef CODEC_DEBUG
//1: ORDER IMPORTANT!!
LOCAL char* _sprd_codec_s_str_tbl[MAX_S] = 
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
    "HP_POP_S",
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
    HP_POP_U_DONE_CHECK,
    HP_POP_U_DONE,
    HP_POP_D_DONE_CHECK,
    HP_POP_D_DONE,
    GAIN_U_DONE_CHECK,
    GAIN_U_DONE,
    GAIN_D_DONE_CHECK,
    GAIN_D_DONE,
    LIN_OPEN,
    END_S2=START_S2
}SPRD_CODEC_STATE2_E;


/*
 * TASK MSG.
*/
typedef enum {
    SPRD_CODEC_START_RUN = 1,
    SPRD_CODEC_MAX_MSG
} SPRD_CODEC_TASK_MSG_E;

typedef enum
{
    D_OFF = 0,
    D_ON
} SPRD_CODEC_STATE_ONOFF_E;

typedef enum
{
    D_NOTHING = 0,
    D_OPEN,
    D_CLOSE
} SPRD_CODEC_STATE_OC_E;

//ORDER IMPORTANT!!
typedef enum
{
    I_DA = 0,
    I_LIN,
    I_DAC,
    I_HP,
    I_LO,
    I_EP,
    I_DAC_MUTE,
    
    I_LIR,
    I_AD,
    I_MAX
} SPRD_CODEC_STATE_OC_INDEX_E;


#if (USE_EVENT_TRIGGER)
enum{
    SPRD_CODEC_RUNING     =   0x1,
    SPRD_CODEC_DELAING    =   0x2,
    SPRD_CODEC_WAITING    =   0x4,
    SPRD_CODEC_LOCKING    =   0x8,
    SPRD_CODEC_BLOCKING   =   0x10,
    SPRD_CODEC_MAX_EVENT
};
#endif

typedef struct
{
    //DA
    SPRD_CODEC_STATE_ONOFF_E        st;            // DA
    SPRD_CODEC_STATE_ONOFF_E        lin_st;        // LINEIN
    SPRD_CODEC_STATE_ONOFF_E        dac_st;        // DAC
    SPRD_CODEC_STATE_ONOFF_E        hp_st;         // hp path flag.
    SPRD_CODEC_STATE_ONOFF_E        lo_st;         // lo path flag.
    SPRD_CODEC_STATE_ONOFF_E        ep_st;         // ep path flag.
    SPRD_CODEC_STATE_ONOFF_E        unmute_st;     // DAC MUTE.
}SPRD_CODEC_DA_STATE_T;

typedef struct
{
    //AD
    SPRD_CODEC_STATE_ONOFF_E        st;            // AD
    SPRD_CODEC_STATE_ONOFF_E        liR_st;        // LINEINREC
}SPRD_CODEC_AD_STATE_T;


typedef struct{
    uint32                          dac;
    uint32                          adc;
    uint32                          lin;
}SPRD_CODEC_CHAN_T;

typedef struct
{
    SPRD_CODEC_STATE_OC_E           oc[I_MAX];
    SPRD_CODEC_DA_STATE_T           da;
    SPRD_CODEC_AD_STATE_T           ad;
    // aux val
#if !(USE_EVENT_TRIGGER)
    uint32                          locking;
    uint32                          runing;
    uint32                          delaing;
    uint32                          waiting;
    uint32                          blocking;
#endif
    SPRD_CODEC_STATE_E              cur_st;
    SPRD_CODEC_STATE_E              pre_st;
    uint32                          st_2;
    uint32                          delay_cnt;
#if (USE_INT_DELAY)
    uint32                          timeout;
#endif
}SPRD_CODEC_STATE_RUN_T;

typedef struct{
    uint32                          func_flag;
    SPRD_CODEC_STATE_OC_E           sprd_codec_oc[I_MAX];
}SPRD_CODEC_STATE_TBL_T;


typedef struct
{
    char                            magic_str[AUDIO_MAGIC_STR_MAX_LEN];

    SPRD_CODEC_STATE_RUN_T          st_run;
    SPRD_CODEC_STATE_OC_E           ch[I_MAX];
    SPRD_CODEC_CHAN_T               chan;
#if (USE_EVENT_TRIGGER)
    SCI_EVENT_GROUP_PTR             event;
#endif
    SCI_TIMER_PTR                   timer;
    BLOCK_ID                        task_id;
    BOOLEAN                         is_ready;

    uint32                          magic_num;
} SPRD_CODEC_STATE_T;

/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local variables                                   *
 **---------------------------------------------------------------------------*/

LOCAL SPRD_CODEC_STATE_T s_sprd_codec_state = {0};

LOCAL const SPRD_CODEC_STATE_TBL_T _sprd_codec_state_tbl[]=
{//         fun_flg            , {I_DA     ,I_LIN  ,I_DAC  ,I_HP   ,I_LO   ,I_EP   ,I_DAC_MUTE ,I_LIR  ,I_AD   }
    {SPRD_CODEC_FUN_PLAYBACK   , {D_OPEN   ,0      ,D_OPEN ,0      ,0      ,0      ,0          ,0      ,0      }},
    {SPRD_CODEC_FUN_CAPTURE    , {0        ,0      ,0      ,0      ,0      ,0      ,0          ,D_OPEN ,D_OPEN }},
    {SPRD_CODEC_FUN_LINEIN     , {D_OPEN   ,D_OPEN ,0      ,0      ,0      ,0      ,0          ,0      ,D_OPEN }},
    {SPRD_CODEC_FUN_LINEIN_REC , {0        ,0      ,0      ,0      ,0      ,0      ,0          ,D_OPEN ,0      }},
    {SPRD_CODEC_FUN_HP         , {0        ,0      ,0      ,D_OPEN ,0      ,0      ,0          ,0      ,0      }},
    {SPRD_CODEC_FUN_LOUT       , {0        ,0      ,0      ,0      ,D_OPEN ,0      ,0          ,0      ,0      }},
    {SPRD_CODEC_FUN_EP         , {0        ,0      ,0      ,0      ,0      ,D_OPEN ,0          ,0      ,0      }},
    {SPRD_CODEC_FUN_DAC_UNMUTE , {0        ,0      ,0      ,0      ,0      ,0      ,D_OPEN     ,0      ,0      }},
    {SPRD_CODEC_FUN_LININ_DIGTAL, {D_OPEN  ,0      ,D_OPEN ,0      ,0      ,0      ,0          ,D_OPEN, D_OPEN }},     
};
#define _sprd_codec_state_tbl_len (sizeof(_sprd_codec_state_tbl)/sizeof(_sprd_codec_state_tbl[0]))

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
LOCAL uint32 _is_blocking(void);
LOCAL void   _set_blocking(uint32 set);

/*
 * CODEC LOCAL FUNTION.
*/
LOCAL __inline uint32 _sprd_codec_is_oc(SPRD_CODEC_STATE_OC_E *pSrc,uint32 oc);
LOCAL __inline void   _sprd_codec_oc_set(SPRD_CODEC_STATE_OC_E *pSrc,uint32 oc);
LOCAL __inline void   _sprd_codec_on_off_set(SPRD_CODEC_STATE_ONOFF_E *pSrc,uint32 onoff);
LOCAL void   _sprd_codec_apply_change(void);
LOCAL void   _sprd_codec_change_state(SPRD_CODEC_STATE_E st);
LOCAL void   _sprd_codec_init_state(void);
LOCAL uint32 _sprd_codec_timer_wait(uint32 time);
LOCAL void   _sprd_codec_timer_wait_cancel(void);
LOCAL uint32 _sprd_codec_timer_delay(uint32 time);
LOCAL void   _sprd_codec_notify_data(uint32 need);
LOCAL void   _sprd_codec_stop_all(uint32 is_open);
LOCAL void   _sprd_codec_merge_change(void *to,const void* from);

LOCAL  void _sprd_codec_send_msg(uint32 msg);

LOCAL void  _sprd_codec_get_oc(SPRD_CODEC_STATE_OC_E *p_sprd_codec_oc,uint32 func_flag);
LOCAL void  _sprd_codec_change_oc_flag(SPRD_CODEC_STATE_OC_E * p_sprd_codec_oc);
LOCAL void  _sprd_codec_mute_all(void);

#if (USE_INT_DELAY)

LOCAL void  _sprd_codec_ap_irq_dis(void);
LOCAL void  _sprd_codec_ap_irq_en(void);
LOCAL ISR_EXE_T _sprd_codec_ap_isr(uint32 tmp);
LOCAL void _sprd_codec_ap_hisr(uint32 count, void *data);
LOCAL void  _sprd_codec_dacmute_irq_dis(void);
LOCAL void  _sprd_codec_dacmute_irq_en(void);
LOCAL ISR_EXE_T _sprd_codec_dacmute_isr(uint32 tmp);
LOCAL void _sprd_codec_dacmute_hisr(uint32 count, void *data);

#endif


#define IS_OPEN(v)      (_sprd_codec_is_oc((SPRD_CODEC_STATE_OC_E*)&(v),D_OPEN))
#define IS_CLOSE(v)     (_sprd_codec_is_oc((SPRD_CODEC_STATE_OC_E*)&(v),D_CLOSE))
#define IS_ON(v)        (v==D_ON)
#define IS_OFF(v)       (v==D_OFF)

#define S_SPRD_CODEC_M         (s_sprd_codec_state.st_run)

//ALL CONDITION

//COMM
#define O_CONDI          ((AD_O) || (DA_O))                      //T01
//#define C_CONDI        ((AD_C) && (DA_C))                      //T02

//AD
#define AD_O             (IS_OPEN(S_SPRD_CODEC_M.oc[I_AD]))             //T03
#define AD_C             (IS_CLOSE(S_SPRD_CODEC_M.oc[I_AD]))            //T04
#define LIN_REC_O        (IS_OPEN(S_SPRD_CODEC_M.oc[I_LIR]))            //T05
#define LIN_REC_C        (IS_CLOSE(S_SPRD_CODEC_M.oc[I_LIR]))           //T06

//DA
#define DA_O             ((IS_OPEN(S_SPRD_CODEC_M.oc[I_DA])))           //T07
#define DA_C             ((IS_CLOSE(S_SPRD_CODEC_M.oc[I_DA])))          //T08

#define DAC_O            ((IS_OPEN(S_SPRD_CODEC_M.oc[I_DAC])))          //T09
#define DAC_C            ((IS_CLOSE(S_SPRD_CODEC_M.oc[I_DAC])))         //T10
#define LIN_O            ((IS_OPEN(S_SPRD_CODEC_M.oc[I_LIN])))          //T11
#define LIN_C            ((IS_CLOSE(S_SPRD_CODEC_M.oc[I_LIN])))         //T12
#define LOUT_O           ((IS_OPEN(S_SPRD_CODEC_M.oc[I_LO])))           //T13
#define LOUT_C           ((IS_CLOSE(S_SPRD_CODEC_M.oc[I_LO])))          //T14
#define EP_O             ((IS_OPEN(S_SPRD_CODEC_M.oc[I_EP])))           //TXX
#define EP_C             ((IS_CLOSE(S_SPRD_CODEC_M.oc[I_EP])))          //TXX
#define HP_O             ((IS_OPEN(S_SPRD_CODEC_M.oc[I_HP])))           //T15
#define HP_C             ((IS_CLOSE(S_SPRD_CODEC_M.oc[I_HP])))          //T16

#define DACUNMUTE        ((IS_OPEN(S_SPRD_CODEC_M.oc[I_DAC_MUTE])))     //T17
#define DACMUTE          ((IS_CLOSE(S_SPRD_CODEC_M.oc[I_DAC_MUTE])))    //T18


#define AD_ON            (IS_ON(S_SPRD_CODEC_M.ad.st))                  //T22
#define AD_OF            (IS_OFF(S_SPRD_CODEC_M.ad.st))                 //T23
#define LIN_REC_ON       (IS_ON(S_SPRD_CODEC_M.ad.liR_st))              //T24
#define LIN_REC_OF       (IS_OFF(S_SPRD_CODEC_M.ad.liR_st))             //T25

#define DA_ON            (IS_ON(S_SPRD_CODEC_M.da.st))                  //T26
#define DA_OF            (IS_OFF(S_SPRD_CODEC_M.da.st))                 //T27
#define HP_ON            (IS_ON(S_SPRD_CODEC_M.da.hp_st))               //T28
#define HP_OF            (IS_OFF(S_SPRD_CODEC_M.da.hp_st))              //T29
#define LOUT_ON          (IS_ON(S_SPRD_CODEC_M.da.lo_st))               //T30
#define LOUT_OF          (IS_OFF(S_SPRD_CODEC_M.da.lo_st))              //T31
#define EP_ON            (IS_ON(S_SPRD_CODEC_M.da.ep_st))               //Txx
#define EP_OF            (IS_OFF(S_SPRD_CODEC_M.da.ep_st))              //Txx
#define DAC_ON           (IS_ON(S_SPRD_CODEC_M.da.dac_st))              //T32
#define DAC_OF           (IS_OFF(S_SPRD_CODEC_M.da.dac_st))             //T33
#define LIN_ON           (IS_ON(S_SPRD_CODEC_M.da.lin_st))              //T34
#define LIN_OF           (IS_OFF(S_SPRD_CODEC_M.da.lin_st))             //T35
#define FROM_COMM_STATE  (S_SPRD_CODEC_M.pre_st==COMM_ON_S)             //T36
#define DAC_UNMUTE       (IS_ON(S_SPRD_CODEC_M.da.unmute_st))           //T37
#define DAC_MUTE         (IS_OFF(S_SPRD_CODEC_M.da.unmute_st))          //T38

#define A_CONDI ((!(LIN_C && DAC_C)) && ((DAC_ON && ((LIN_O && LIN_OF) ||(LIN_C && LIN_ON))) || (LIN_ON && ((DAC_O && DAC_OF) ||(DAC_C && DAC_ON)))))
#define B_CONDI ((!(DA_C)) && (HP_O || LOUT_O || EP_O|| (HP_C || LOUT_C || EP_C)))
//ALL CONDITION END.

#define EXIT_RUNNING        (0)
#define RUN_TO_NEXT         (1)
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
PUBLIC void SPRD_CODEC_SM_Process(void)
{
    //SPRD_CODEC_STATE_DGB:"[AUDST]SPRD_CODEC_SM_Process"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_425_112_2_17_23_2_57_225,(uint8*)"");
    AUDIO_HAL_PRTMEM ( 0xAAAAFFFF);
    if( (!__sprd_codec_is_arm_ctl()) )
    {
        //SPRD_CODEC_STATE_PRT:"[AUDST]arm not run"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_429_112_2_17_23_2_57_226,(uint8*)"");
        return;
    }
    //send msg to task for start.
    _sprd_codec_send_msg(SPRD_CODEC_START_RUN);
}

PUBLIC BOOLEAN SPRD_CODEC_IsReady(void)
{
    BOOLEAN ret = 0;
    SPRD_CODEC_G_LOCK();
    ret = s_sprd_codec_state.is_ready;
    SPRD_CODEC_G_UNLOCK();
    return ret;
}

PUBLIC BOOLEAN SPRD_CODEC_IsOnStop(void)
{
    BOOLEAN ret = 0;
    SPRD_CODEC_G_LOCK();
    ret = ((s_sprd_codec_state.st_run.cur_st==ON_STOP_S));
    SPRD_CODEC_G_UNLOCK();
    return ret;
}

PUBLIC BOOLEAN SPRD_CODEC_IsPwrOn(void)
{
    BOOLEAN ret = 0;
    SPRD_CODEC_G_LOCK();
    ret = ((s_sprd_codec_state.st_run.cur_st>SLP_S));
    SPRD_CODEC_G_UNLOCK();
    return ret;
}

PUBLIC BOOLEAN SPRD_CODEC_IsRuning(void)
{
    BOOLEAN ret = 0;
    SPRD_CODEC_G_LOCK();
    ret = !((s_sprd_codec_state.st_run.cur_st==OFF_S) || (s_sprd_codec_state.st_run.cur_st==ON_STOP_S));
    SPRD_CODEC_G_UNLOCK();
    return ret;
}

PUBLIC void SPRD_CODEC_ChangeSt(uint32 func_flag,uint32 is_close)
{
    SPRD_CODEC_STATE_OC_E  use_sprd_codec_oc[I_MAX];
    
    _sprd_codec_get_oc(use_sprd_codec_oc,func_flag);
    
    if(is_close)
    {
        _sprd_codec_change_oc_flag(use_sprd_codec_oc);
    }
    
    SPRD_CODEC_G_LOCK();
    _sprd_codec_merge_change(&(s_sprd_codec_state.ch),use_sprd_codec_oc);
    SPRD_CODEC_G_UNLOCK();
    
    //SPRD_CODEC_STATE_DGB:"[AUDST]CHANGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_488_112_2_17_23_2_57_227,(uint8*)"ddddddddd",s_sprd_codec_state.ch[I_AD],s_sprd_codec_state.ch[I_LIR],s_sprd_codec_state.ch[I_DA],s_sprd_codec_state.ch[I_DAC],s_sprd_codec_state.ch[I_LIN],s_sprd_codec_state.ch[I_HP],s_sprd_codec_state.ch[I_LO],s_sprd_codec_state.ch[I_EP],s_sprd_codec_state.ch[I_DAC_MUTE]);
    AUDIO_HAL_PRTMEM ( (0xCC500000 
        | ((s_sprd_codec_state.ch[I_EP]<<16) & 0x00030000)
        | ((s_sprd_codec_state.ch[I_AD ]<<14) & 0x0000C000)
        | ((s_sprd_codec_state.ch[I_LIR]<<12) & 0x00003000)
        | ((s_sprd_codec_state.ch[I_DA ]<<10) & 0x00000C00)
        | ((s_sprd_codec_state.ch[I_DAC]<<8)  & 0x00000300)
        | ((s_sprd_codec_state.ch[I_LIN]<<6)  & 0x000000C0)
        | ((s_sprd_codec_state.ch[I_HP ]<<4)  & 0x00000030)
        | ((s_sprd_codec_state.ch[I_LO ]<<2)  & 0x0000000C)
        | ((s_sprd_codec_state.ch[I_DAC_MUTE ]<<0)  & 0x00000003)
        ));
}

PUBLIC void SPRD_CODEC_Change_Chan(uint32 fun_flg,uint32 is_close)
{
    if( is_close )
    {
        if( (AUDIO_FUN_LINEIN & fun_flg)    )
        {
            if( AUDIO_FUN_DAC0 & fun_flg )
            {
                s_sprd_codec_state.chan.lin &= ~AUDIO_CHAN_L; 
                s_sprd_codec_state.chan.adc &= ~AUDIO_CHAN_L; 
            }
            if( AUDIO_FUN_DAC1 & fun_flg )
            {
                s_sprd_codec_state.chan.lin &= ~AUDIO_CHAN_R; 
                s_sprd_codec_state.chan.adc &= ~AUDIO_CHAN_R; 
            }
        }
        else
        {
            if( AUDIO_FUN_DAC0 & fun_flg )
            {
                s_sprd_codec_state.chan.dac &= ~AUDIO_CHAN_L; 
            }
            if( AUDIO_FUN_DAC1 & fun_flg )
            {
                s_sprd_codec_state.chan.dac &= ~AUDIO_CHAN_R; 
            }
        }
        if( AUDIO_FUN_ADC0 & fun_flg )
        {
            s_sprd_codec_state.chan.adc &= ~AUDIO_CHAN_L; 
        }
        if( AUDIO_FUN_ADC1 & fun_flg )
        {
            s_sprd_codec_state.chan.adc &= ~AUDIO_CHAN_R; 
        }
    }
    else
    {
        if( (AUDIO_FUN_LINEIN & fun_flg)    )
        {
            if( AUDIO_FUN_DAC0 & fun_flg )
            {
                s_sprd_codec_state.chan.lin |= AUDIO_CHAN_L; 
                s_sprd_codec_state.chan.adc |= AUDIO_CHAN_L; 
            }
            if( AUDIO_FUN_DAC1 & fun_flg )
            {
                s_sprd_codec_state.chan.lin |= AUDIO_CHAN_R; 
                s_sprd_codec_state.chan.adc |= AUDIO_CHAN_R; 
            }
        }
        else
        {
            if( AUDIO_FUN_DAC0 & fun_flg )
            {
                s_sprd_codec_state.chan.dac |= AUDIO_CHAN_L; 
            }
            if( AUDIO_FUN_DAC1 & fun_flg )
            {
                s_sprd_codec_state.chan.dac |= AUDIO_CHAN_R; 
            }
        }
        if( AUDIO_FUN_ADC0 & fun_flg )
        {
            s_sprd_codec_state.chan.adc |= AUDIO_CHAN_L; 
        }
        if( AUDIO_FUN_ADC1 & fun_flg )
        {
            s_sprd_codec_state.chan.adc |= AUDIO_CHAN_R; 
        }
    }
}


LOCAL void _run_sprd_codec_state(void)
{
    RUN_SPRD_CODEC_S_FUN fun = 0;
    if( _is_runing() || _is_delaing() )
    {
        return;
    }
    _sprd_codec_timer_wait_cancel();

    SCI_ASSERT( __sprd_codec_is_arm_ctl() ); /*assert verified*/

    _set_runing(1);
    //if return  FALSE means s_sprd_codec_state state off
    //   or need delay , when delay complete will start this.
    // if return TRUE means s_sprd_codec_state state should run next state.
    do{
        _sprd_codec_apply_change();
        fun = (RUN_SPRD_CODEC_S_FUN)(_sprd_codec_s_fun_tbl[(s_sprd_codec_state.st_run.cur_st)]);
    }while( fun((uint32)&(s_sprd_codec_state.st_run)) );
    _set_runing(0);
}


LOCAL uint32 _run_off_s(uint32 param)
{
    if( O_CONDI )
    {
        _sprd_codec_change_state(PWR_S);
    }
    else //if( C_CONDI )
    {
        return EXIT_RUNNING;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_pwr_s(uint32 param)
{
    static uint32 is_o = 0;
    if( O_CONDI )
    {
        if( !is_o )
        {
            switch(s_sprd_codec_state.st_run.st_2)
            {
                case START_S2:
                {
                    __sprd_codec_register(1);
                    s_sprd_codec_state.st_run.st_2 = PWR_RESET;
                    return RUN_TO_NEXT;
                }break;/*lint !e527 comfirmed by windy.wang*/
                case PWR_RESET:
                {
                    if( !__sprd_codec_ap_is_ldo_mic_bias() ) //lint !e506 !e774
                    {
                        __sprd_codec_reset();
                    }
                    s_sprd_codec_state.st_run.st_2 = PWR_UP;
                    return _sprd_codec_timer_delay(SPRD_CODEC_RESET_TIME);
                }break;/*lint !e527 comfirmed by windy.wang*/
                case PWR_UP:
                {
                    //after reset.
                    __sprd_codec_open();
                    __sprd_codec_fs_setting();
                    __sprd_codec_en(1);
                    s_sprd_codec_state.st_run.st_2 = PWR_DONE;
                    return _sprd_codec_timer_delay(SPRD_CODEC_VCOM_TIME);
                }break;/*lint !e527 comfirmed by windy.wang*/
                case PWR_DONE:
                {
                    s_sprd_codec_state.st_run.st_2 = END_S2;
                    is_o = 1;
                }break;
                default:break;
            }
        }
        _sprd_codec_change_state(SLP_S);
    }
    else// if( C_CONDI )
    {
        is_o = 0;
        __sprd_codec_en(0);
        if( !__sprd_codec_ap_is_ldo_mic_bias() ) //lint !e506 !e774
        {
            __sprd_codec_register(0);
        }
        s_sprd_codec_state.st_run.st_2 = START_S2;
        _sprd_codec_stop_all(0);
        _sprd_codec_change_state(OFF_S);
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_slp_s(uint32 param)
{
    if( AD_O )
    {
        _sprd_codec_change_state(AD_S);
    }
    else if( DA_O )
    {
        _sprd_codec_change_state(DA_S);
    }
    else //if( C_CONDI )
    {
        __sprd_codec_mclk(0);
        _sprd_codec_change_state(PWR_S);
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_comm_on_s(uint32 param)
{
    if( (LIN_REC_C && LIN_REC_ON) || (LIN_REC_O && LIN_REC_OF && AD_ON ) )
    {
        _sprd_codec_change_state(LIN_REC_S);
    }
    else if( (AD_O && AD_OF ) || (LIN_REC_O && AD_OF) )
    {
        _sprd_codec_change_state(AD_S);
    }
    else if( (DA_O && DA_OF ) )
    {
        _sprd_codec_change_state(DA_S);
    }
    else if( DA_ON && (A_CONDI) )
    {
        _sprd_codec_change_state(DAC_LIN_S);
    }
    else if( DA_ON && (B_CONDI) )
    {
        _sprd_codec_change_state(HP_POP_S);
    }
    else if( ((DACMUTE || DACUNMUTE)&& DAC_ON && DA_ON) || (DA_C && DA_ON ) )
    {
        _sprd_codec_change_state(MUTE_S);
    }
    else if( (AD_C && AD_ON) )
    {
        _sprd_codec_change_state(AD_S);
    }
    else
    {
        _sprd_codec_change_state(ON_STOP_S);
        _sprd_codec_stop_all(1);
        _sprd_codec_notify_data(1);
    }
    return RUN_TO_NEXT;
}

#if CLOSE_WAIT_TEST
LOCAL uint32 _run_close_wait_s(uint32 param)
{
    if( (DA_C && DA_ON ) )
    {
        _sprd_codec_change_state(HP_POP_S);
#if CLOSE_WAIT_FOR_EXT_HP_PA_TEST
        if( HP_C && __sprd_codec_is_use_ext_hp() )
            return _sprd_codec_timer_wait(SPRD_CODEC_CLOSE_WAIT_TIME-1000);
        else
            return _sprd_codec_timer_wait(SPRD_CODEC_CLOSE_WAIT_TIME);
#else
        return _sprd_codec_timer_wait(SPRD_CODEC_CLOSE_WAIT_TIME);
#endif
    }
    else if (DA_O)
    {
        _sprd_codec_change_state(LOUT_HP_S);
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
#ifdef CODEC_DEBUG
    //SPRD_CODEC_STATE_DGB:"[AUDST]on_stop pre=%s,cur=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_707_112_2_17_23_2_58_228,(uint8*)"ss",_sprd_codec_s_str_tbl[s_sprd_codec_state.st_run.pre_st],_sprd_codec_s_str_tbl[s_sprd_codec_state.st_run.cur_st]);
#endif
    AUDIO_HAL_PRTMEM ( (0x555A0000 
        | ((s_sprd_codec_state.st_run.pre_st<<8) & 0x0000FF00)
        | ((s_sprd_codec_state.st_run.cur_st<<0) & 0x000000FF)
        ));
    if( (__sprd_codec_is_arm_ctl()) && (((O_CONDI || AD_C || DA_C || DAC_O || DAC_C || LIN_O || LIN_C 
        || HP_O || HP_C || LOUT_O || LOUT_C || EP_O || EP_C || LIN_REC_O || LIN_REC_C) 
        && (AD_ON || DA_ON )) || DACMUTE || DACUNMUTE)  )
    {
        _sprd_codec_change_state(COMM_ON_S);
    }
    else
    {
        SCI_MEMSET(&(s_sprd_codec_state.st_run.oc),0,sizeof(s_sprd_codec_state.st_run.oc));
        return EXIT_RUNNING;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_ad_s(uint32 param)
{
    if( AD_O )
    {
        __sprd_codec_mclk(1);
#if (PGA_SMOOTH_ADJUST)
        __sprd_codec_ap_adc_pga_gain_clr_mute();
#endif
        __sprd_codec_ap_adc(1,s_sprd_codec_state.chan.adc);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.ad.st),D_ON);
        if( LIN_REC_O )
        {
            _sprd_codec_change_state(LIN_REC_S);
        }
        else
        {
            _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_AD]),D_NOTHING);
            _sprd_codec_change_state(COMM_ON_S);
        }
    }
    else if( AD_C )
    {
#if (PGA_SMOOTH_ADJUST)
        __sprd_codec_ap_adc_mute();
#else
        __sprd_codec_ap_adc(0,AUDIO_ALL_CHAN);
#endif
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.ad.st),D_OFF);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_AD]),D_NOTHING);
        if( DA_ON )
        {
            _sprd_codec_change_state(COMM_ON_S);
        }
        else if( DA_O )
        {
            _sprd_codec_change_state(DA_S);
        }
        else //if( !(DA_O || liO) )
        {
            _sprd_codec_change_state(SLP_S);
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
    if( LIN_REC_O )
    {
        __sprd_codec_adc_en(1, s_sprd_codec_state.chan.adc);
        __sprd_codec_ap_linein_rec(1,s_sprd_codec_state.chan.adc);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.ad.liR_st),D_ON);
        s_sprd_codec_state.st_run.oc[I_AD] = s_sprd_codec_state.st_run.oc[I_LIR] = D_NOTHING;//_sprd_codec_oc_set
        _sprd_codec_change_state(COMM_ON_S);
    }
    else if( LIN_REC_C )
    {
        __sprd_codec_adc_en(0, s_sprd_codec_state.chan.adc);
        __sprd_codec_ap_linein_rec(0,s_sprd_codec_state.chan.adc);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.ad.liR_st),D_OFF);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_LIR]),D_NOTHING);
        if( AD_C )
        {
            _sprd_codec_change_state(AD_S);
        }
        else if( AD_ON )
        {
            _sprd_codec_change_state(COMM_ON_S);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_da_s(uint32 param)
{
    _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.st),D_OFF);
    if( DA_O )
    {
        _sprd_codec_mute_all();
        _sprd_codec_change_state(DA_CLK_S);
    }
    else if( DA_C  )
    {
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_DA]),D_NOTHING);
        if( AD_ON )
        {
            _sprd_codec_change_state(COMM_ON_S);
        }
        else if( AD_O )
        {
            _sprd_codec_change_state(AD_S);
        }
        else
        {
            _sprd_codec_change_state(SLP_S);
        }
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_da_clk_s(uint32 param)
{
    if( DA_O )
    {
        __sprd_codec_ap_dac_clk_en(1);
        __sprd_codec_ap_drv_clk_en(1);
        _sprd_codec_change_state(DAC_LIN_S);
    }
    else if( DA_C  )
    {
        __sprd_codec_ap_drv_clk_en(0);
        __sprd_codec_ap_dac_clk_en(0);
        _sprd_codec_change_state(DA_S);
    }
    else
    {
        SCI_ASSERT(0); /*assert verified*/
    }
    return RUN_TO_NEXT;
}


LOCAL uint32 _run_dac_s(uint32 param)
{
    if( DAC_O )
    {
        __sprd_codec_mclk(1);
        __sprd_codec_dac_en(1,s_sprd_codec_state.chan.dac);
        __sprd_codec_ap_dac(1,s_sprd_codec_state.chan.dac);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.dac_st),D_ON);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_DAC]),D_NOTHING);
    }
    else if( DAC_C && ( (DA_C && !(HP_C || HP_O) && !(LOUT_C || LOUT_O) && !(EP_C || EP_O)) ))
    {
        __sprd_codec_dac_en(0,AUDIO_ALL_CHAN);
        __sprd_codec_ap_dac(0,AUDIO_ALL_CHAN);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.dac_st),D_OFF);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_DAC]),D_NOTHING);
    }
    else
    {
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_DAC]),D_NOTHING);
        return NO_OPRATE;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_lin_s(uint32 param)
{
    static uint32 mute_change = 0;
    if( LIN_O )
    {
        switch(s_sprd_codec_state.st_run.st_2)
        {
            case START_S2:
            {
                _set_locking(1);
                s_sprd_codec_state.st_run.st_2 = LIN_OPEN;
                __sprd_codec_ap_lin(1,s_sprd_codec_state.chan.lin);
                mute_change = __sprd_codec_dp_dac_mute(1);
                return _sprd_codec_timer_delay(250);
            }break;/*lint !e527 comfirmed by windy.wang*/
            case LIN_OPEN:
            {
                s_sprd_codec_state.st_run.st_2 = END_S2;
                _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.lin_st),D_ON);
                _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_LIN]),D_NOTHING);
                _set_locking(0);
            }break;
            default:break;
        }
    }
    else if( LIN_C && ( (DA_C && !(HP_C || HP_O) && !(LOUT_C || LOUT_O) && !(EP_C || EP_O)) ) )
    {
        __sprd_codec_ap_lin(0,AUDIO_ALL_CHAN);
        if(mute_change)
        {
            __sprd_codec_dp_dac_mute(0);
        }
        s_sprd_codec_state.st_run.st_2 = END_S2;
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.lin_st),D_OFF);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_LIN]),D_NOTHING);
        _set_blocking(0);
    }
    else
    {
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_LIN]),D_NOTHING);
        return NO_OPRATE;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_dac_lin_s(uint32 param)
{
    uint32 ret = 0;
    ret = _run_dac_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_sprd_codec_state.st_run.st_2) )
    {
        return ret;
    }
    ret = _run_lin_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_sprd_codec_state.st_run.st_2) )
    {
        return ret;
    }

    if( !(DA_C && !(HP_C || HP_O) && !(LOUT_C || LOUT_O) && !(EP_C || EP_O)) )
    {
        _sprd_codec_change_state(MIX_S);
    }
    else
    {
        _sprd_codec_change_state(DA_CLK_S);
    }
    return RUN_TO_NEXT;
}


LOCAL uint32 _run_mix_s(uint32 param)
{
    if( DA_ON && (A_CONDI) )
    {
        _sprd_codec_change_state(COMM_ON_S);
    }
    else if( !(DA_C && !(HP_C || HP_O) && !(LOUT_C || LOUT_O) && !(EP_C || EP_O)) )
    {
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.st),D_ON);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_DA]),D_NOTHING);
        _sprd_codec_change_state(HP_POP_S);
    }
    else
    {
        _sprd_codec_change_state(DAC_LIN_S);
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_hp_s(uint32 param);

LOCAL uint32 _run_hp_pop_s(uint32 param)
{
    static uint32 is_o = 0;
    if( DA_ON && (A_CONDI) )
    {
        s_sprd_codec_state.st_run.st_2 = END_S2;
        _sprd_codec_change_state(COMM_ON_S);
    }
    else if( !DA_C )
    {
        if( HP_O && !is_o )
        {
            //ramp up state.
            switch(s_sprd_codec_state.st_run.st_2)
            {
                case START_S2:
                case HP_POP_D_DONE_CHECK:
                case HP_POP_D_DONE:
                {
                    _set_locking(1);
                    if(__sprd_codec_is_use_ext_hp())
                    {
                        s_sprd_codec_state.st_run.st_2 = HP_POP_U_DONE;
                    }
                    else if( __sprd_codec_ap_hp_pop_en(1) )
                    {
                        s_sprd_codec_state.st_run.st_2 = HP_POP_U_DONE_CHECK;
                        s_sprd_codec_state.st_run.delay_cnt=0;
#if (USE_INT_DELAY)
                        _sprd_codec_ap_irq_en();
                        return _sprd_codec_timer_delay(SPRD_CODEC_THP_TIMEOUT);
#else
                        return _sprd_codec_timer_delay(SPRD_CODEC_THP_TIME);
#endif
                    }
                    else
                    {
                        s_sprd_codec_state.st_run.st_2 = HP_POP_U_DONE;
                        //SPRD_CODEC_STATE_PRT:"[AUDDRV][SPRD]hp pop is up"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_972_112_2_17_23_2_59_229,(uint8*)"");
                    }
                    return RUN_TO_NEXT;
                }break;/*lint !e527 comfirmed by windy.wang*/
                case HP_POP_U_DONE_CHECK:
                {
                    SCI_Sleep(2); //analog flag maybe unstable.
                    if( __sprd_codec_ap_hp_ramp_done(1) )
                    {
                        __sprd_codec_ap_clr_ramp(1);
                    }
                    else if( (s_sprd_codec_state.st_run.delay_cnt>SPRD_CODEC_RUPD_DET_MAX_CNT)
#if (USE_INT_DELAY)
                        || (s_sprd_codec_state.st_run.timeout)
#endif
                    )
                    {
                        //SPRD_CODEC_STATE_PRT:"[AUDDRV][SPRD]RAMP UP"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_984_112_2_17_23_2_59_230,(uint8*)"");
                        __sprd_codec_ap_reg_print();
                        SPRD_CODEC_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                    }
                    else
                    {
                        s_sprd_codec_state.st_run.delay_cnt++;
#if (USE_INT_DELAY)
                        _sprd_codec_ap_irq_en();
                        return _sprd_codec_timer_delay(SPRD_CODEC_TIME_CONT);
#else
                        return _sprd_codec_timer_delay(SPRD_CODEC_RUPD_DET_TIME);
#endif
                    }
                    s_sprd_codec_state.st_run.delay_cnt = 0;
                    s_sprd_codec_state.st_run.st_2 = HP_POP_U_DONE;
                    return RUN_TO_NEXT;
                }break;/*lint !e527 comfirmed by windy.wang*/
                case HP_POP_U_DONE:
                {
                    s_sprd_codec_state.st_run.st_2 = END_S2;
                    is_o = 1;
                    _run_hp_s(param); //HP MUST LOCK OP
                    _set_locking(0);
                }break;
                default:break;
            }
            if(__sprd_codec_is_use_ext_hp())
            {
                _sprd_codec_change_state(LOUT_HP_S);
                return _sprd_codec_timer_delay(__sprd_codec_get_ext_hp_delay_time());
            }
        }
        _sprd_codec_change_state(LOUT_HP_S);
    }
    else
    {
        if( is_o )
        {
            //ramp down state.
            switch(s_sprd_codec_state.st_run.st_2)
            {
                case START_S2:
                {
                    _set_locking(1);
                    _run_hp_s(param); //HP MUST LOCK OP
                    if( (!__sprd_codec_is_use_ext_hp())
                        && __sprd_codec_ap_hp_pop_en(0) 
                    )
                    {
                        s_sprd_codec_state.st_run.st_2 = HP_POP_D_DONE_CHECK;
                        s_sprd_codec_state.st_run.delay_cnt=0;
#if (USE_INT_DELAY)
                        _sprd_codec_ap_irq_en();
                        return _sprd_codec_timer_delay(SPRD_CODEC_THP_TIMEOUT);
#else
                        return _sprd_codec_timer_delay(SPRD_CODEC_THP_TIME);
#endif
                    }
                    else
                    {
                        s_sprd_codec_state.st_run.st_2 = HP_POP_D_DONE;
                        //SPRD_CODEC_STATE_PRT:"[AUDDRV][SPRD]hp pop is down"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1030_112_2_17_23_2_59_231,(uint8*)"");
                    }
                    return RUN_TO_NEXT;
                }break;/*lint !e527 comfirmed by windy.wang*/
                case HP_POP_D_DONE_CHECK:
                {
                    SCI_Sleep(2); //analog flag maybe unstable.
                    if( __sprd_codec_ap_hp_ramp_done(0) )
                    {
                        __sprd_codec_ap_clr_ramp(0);
                    }
                    else if( (s_sprd_codec_state.st_run.delay_cnt>SPRD_CODEC_RUPD_DET_MAX_CNT)
#if (USE_INT_DELAY)
                        || (s_sprd_codec_state.st_run.timeout)
#endif
                    )
                    {
                        //SPRD_CODEC_STATE_PRT:"[AUDDRV][SPRD]RAMP DOWN"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1042_112_2_17_23_2_59_232,(uint8*)"");
                        __sprd_codec_ap_reg_print();
                        SPRD_CODEC_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                    }
                    else
                    {
                        s_sprd_codec_state.st_run.delay_cnt++;
#if (USE_INT_DELAY)
                        _sprd_codec_ap_irq_en();
                        return _sprd_codec_timer_delay(SPRD_CODEC_TIME_CONT);
#else
                        return _sprd_codec_timer_delay(SPRD_CODEC_RUPD_DET_TIME);
#endif
                    }
                    s_sprd_codec_state.st_run.delay_cnt = 0;
                    s_sprd_codec_state.st_run.st_2 = HP_POP_D_DONE;
                    return RUN_TO_NEXT;
                }break;/*lint !e527 comfirmed by windy.wang*/
                case HP_POP_D_DONE:
                {
                    s_sprd_codec_state.st_run.st_2 = END_S2;
                    is_o = 0;
                    _set_locking(0);
                }break;
                default:break;
            }
        }
        _sprd_codec_change_state(MIX_S);
    }
    return RUN_TO_NEXT;
}




LOCAL uint32 _run_lout_s(uint32 param)
{
    if( LOUT_O )
    {
#if (PGA_SMOOTH_ADJUST)
        __sprd_codec_ap_pa_pga_gain_clr_mute();
#endif
        __sprd_codec_ap_lout(1);
        if( DAC_ON )
        {
            __sprd_codec_ap_lout_mixer(1,s_sprd_codec_state.chan.dac);
        }
        else
        {
            __sprd_codec_ap_lout_mixer(0,AUDIO_ALL_CHAN);
        }
        if( LIN_ON )
        {
            __sprd_codec_ap_lout_mixer_adcbuf(1,s_sprd_codec_state.chan.lin);
        }
        else
        {
            __sprd_codec_ap_lout_mixer_adcbuf(0,AUDIO_ALL_CHAN);
        }
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.lo_st),D_ON);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_LO]),D_NOTHING);
    }
    else if( LOUT_C )
    {
        __sprd_codec_ap_lout_mixer(0,AUDIO_ALL_CHAN);
        __sprd_codec_ap_lout_mixer_adcbuf(0,AUDIO_ALL_CHAN);
#ifdef SKIP_SMOOTH_ADJ
        __sprd_codec_ap_lout(0);
#endif
        __sprd_codec_pa_mute(1);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.lo_st),D_OFF);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_LO]),D_NOTHING);
#if (PGA_SMOOTH_ADJUST) //ken.kuang FOR AUDIO CLOSE NOISE!!
        return EXIT_RUNNING;
#endif
    }
    else
    {
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_LO]),D_NOTHING);
        return NO_OPRATE;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_ep_s(uint32 param)
{
    if( EP_O )
    {
#if (PGA_SMOOTH_ADJUST)
        __sprd_codec_ap_ep_pga_gain_clr_mute();
#endif
        __sprd_codec_ap_ear(1);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.ep_st),D_ON);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_EP]),D_NOTHING);
    }
    else if( EP_C )
    {
#ifdef SKIP_SMOOTH_ADJ
        __sprd_codec_ap_ear(0);
#endif
        __sprd_codec_ep_mute(1);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.ep_st),D_OFF);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_EP]),D_NOTHING);
#if (PGA_SMOOTH_ADJUST) //ken.kuang FOR AUDIO CLOSE NOISE!!
        return EXIT_RUNNING;
#endif
    }
    else
    {
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_EP]),D_NOTHING);
        return NO_OPRATE;
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_hp_s(uint32 param)
{
    if( HP_O )
    {
#if (PGA_SMOOTH_ADJUST)
        __sprd_codec_ap_hp_pga_gain_clr_mute();
#endif
        __sprd_codec_ap_hp(1,AUDIO_ALL_CHAN);
        if(!__sprd_codec_is_use_ext_hp())
        {
            __sprd_codec_ap_hp_pop(HP_POP_CTL_DIS);
        }
        if( DAC_ON )
        {
            __sprd_codec_ap_hp_mixer(1,s_sprd_codec_state.chan.dac);
        }
        else
        {
            __sprd_codec_ap_hp_mixer(0,AUDIO_ALL_CHAN);
        }
        if( LIN_ON )
        {
            __sprd_codec_ap_hp_mixer_adcbuf(1,s_sprd_codec_state.chan.lin);
        }
        else
        {
            __sprd_codec_ap_hp_mixer_adcbuf(0,AUDIO_ALL_CHAN);
        }
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.hp_st),D_ON);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_HP]),D_NOTHING);
    }
    else if( HP_C )
    {
        if(!__sprd_codec_is_use_ext_hp())
        {
            __sprd_codec_ap_hp_pop(HP_POP_CTL_HOLD);
        }
        __sprd_codec_ap_hp_mixer(0,AUDIO_ALL_CHAN);
        __sprd_codec_ap_hp_mixer_adcbuf(0,AUDIO_ALL_CHAN);
#ifdef SKIP_SMOOTH_ADJ
        __sprd_codec_ap_hp(0,AUDIO_ALL_CHAN);
#endif
        __sprd_codec_hp_mute(1);
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.hp_st),D_OFF);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_HP]),D_NOTHING);
#if (PGA_SMOOTH_ADJUST) //ken.kuang FOR AUDIO CLOSE NOISE!!
        return EXIT_RUNNING;
#endif
    }
    else
    {
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_HP]),D_NOTHING);
        return NO_OPRATE;
    }

    return RUN_TO_NEXT;
}

LOCAL uint32 _run_lout_hp_s(uint32 param)
{
    uint32 ret = 0;
    if( DA_ON && (A_CONDI) )
    {
        _sprd_codec_change_state(COMM_ON_S);
        return RUN_TO_NEXT;
    }
    _set_locking(1);
    ret = _run_lout_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_sprd_codec_state.st_run.st_2))
    {
        return ret;
    }
    ret = _run_ep_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_sprd_codec_state.st_run.st_2))
    {
        return ret;
    }
    ret = _run_hp_s(param);
    if( (NO_OPRATE!=ret) && (0!= s_sprd_codec_state.st_run.st_2))
    {
        return ret;
    }
    _set_locking(0);

    if( !DA_C )
    {
        _sprd_codec_change_state(MUTE_S);
    }
    else
    {
#if CLOSE_WAIT_TEST
        if( LIN_C )
        {
            _sprd_codec_change_state(HP_POP_S);
        }
        else
        {
            _sprd_codec_change_state(CLOSE_WAIT_S);
        }
#else
        _sprd_codec_change_state(HP_POP_S);
#endif
    }
    return RUN_TO_NEXT;
}

LOCAL uint32 _run_mute_s(uint32 param)
{
    if( DA_ON && (A_CONDI) )
    {
        s_sprd_codec_state.st_run.st_2 = END_S2;
        _sprd_codec_change_state(COMM_ON_S);
    }
    else if( DACUNMUTE )
    {
        switch(s_sprd_codec_state.st_run.st_2)
        {
            case START_S2:
            case GAIN_D_DONE_CHECK:
            {
                if( DAC_ON && __sprd_codec_dp_dac_mute(0) )
                {
                    _set_locking(1);
                    s_sprd_codec_state.st_run.st_2 = GAIN_U_DONE_CHECK;
#if (USE_INT_DELAY)
                    _sprd_codec_dacmute_irq_en();
                    return _sprd_codec_timer_delay(SPRD_CODEC_TDACMUTE_TIMEOUT);
#else
                    return _sprd_codec_timer_delay(SPRD_CODEC_TDACMUTE_TIME);
#endif
                }
                else
                {
                    //SPRD_CODEC_STATE_PRT:"[AUDDRV][SPRD]UnMuted"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1239_112_2_17_23_2_59_233,(uint8*)"");
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
            case GAIN_U_DONE_CHECK:
            {
                if( __sprd_codec_dp_gain_done(1) )
                {
                    __sprd_codec_dp_clr_gain(1);
                }
                else
                {
                    //SPRD_CODEC_STATE_PRT:"[AUDDRV][SPRD]GAIN is UP"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1250_112_2_17_23_2_59_234,(uint8*)"");
                    __sprd_codec_dp_reg_print();
                    SPRD_CODEC_CHECK_MAX_CNT_ASS(0);/*assert verified*/
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
            default:break;
        }
        s_sprd_codec_state.st_run.st_2 = END_S2;
        _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.unmute_st),D_ON);
        _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
        _set_locking(0);
    }
    else if( DACMUTE || DA_C  )
    {
        switch(s_sprd_codec_state.st_run.st_2)
        {
            case START_S2:
            {
                if( DAC_ON && __sprd_codec_dp_dac_mute(1) )
                {
                    _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
                    _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.unmute_st),D_OFF);
                    s_sprd_codec_state.st_run.st_2 = GAIN_D_DONE_CHECK;
#if (USE_INT_DELAY)
                    _sprd_codec_dacmute_irq_en();
                    return _sprd_codec_timer_wait(SPRD_CODEC_TDACMUTE_TIMEOUT);
#else
                    return _sprd_codec_timer_wait(SPRD_CODEC_TDACMUTE_TIME);
#endif
                }
                else
                {
                    _sprd_codec_oc_set(&(s_sprd_codec_state.st_run.oc[I_DAC_MUTE]),D_NOTHING);
                    _sprd_codec_on_off_set(&(s_sprd_codec_state.st_run.da.unmute_st),D_OFF);
                    //SPRD_CODEC_STATE_PRT:"[AUDDRV][SPRD]Muted"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1283_112_2_17_23_2_59_235,(uint8*)"");
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
#if CLOSE_WAIT_FOR_EXT_HP_PA_TEST
            case GAIN_D_DONE_CHECK:
            {
                s_sprd_codec_state.st_run.st_2 = END_S2;
                _sprd_codec_notify_data(0);
                if( DA_C )
                {
                    __sprd_codec_pa_mute(1);
                    __sprd_codec_ep_mute(1);
                    if( HP_C && __sprd_codec_is_use_ext_hp() )
                    {
                        return _sprd_codec_timer_wait(SPRD_CODEC_CLOSE_WAIT_FOR_EXT_HP_TIME);
                    }
                }
            }break;/*lint !e527 comfirmed by windy.wang*/
#endif
            default:break;
        }
        s_sprd_codec_state.st_run.st_2 = END_S2;
        _sprd_codec_notify_data(0);
        if( DA_C )
        {
            _sprd_codec_mute_all();
            _sprd_codec_change_state(LOUT_HP_S);
            if( LIN_C )
            {
                _set_blocking(1);
            }
#if (PGA_SMOOTH_ADJUST) //ken.kuang FOR AUDIO CLOSE NOISE!!
#ifndef SKIP_SMOOTH_ADJ
            return EXIT_RUNNING;
#endif
#endif
        }
        else if(DACMUTE)
        {
            _sprd_codec_change_state(COMM_ON_S);
        }
    }
    if( !DA_C )
    {
        s_sprd_codec_state.st_run.st_2 = END_S2;
        _sprd_codec_change_state(COMM_ON_S);
    }
    return RUN_TO_NEXT;
}



#if (USE_EVENT_TRIGGER)
LOCAL uint32 _is_event_set(uint32 flag)
{
    uint32 ret = 0;
    uint32 actual_event;
    if( s_sprd_codec_state.event )
    {
        ret = SCI_GetEvent(s_sprd_codec_state.event,
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
    if( s_sprd_codec_state.event )
    {
        if( set )
        {
            SCI_SetEvent(s_sprd_codec_state.event, flag, SCI_OR);
        }
        else
        {
            SCI_SetEvent(s_sprd_codec_state.event, ~(flag), SCI_AND);
        }
    }
}

#endif



LOCAL uint32 _is_runing()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(SPRD_CODEC_RUNING);
#else
    uint32 ret = SCI_ERROR;
    SPRD_CODEC_G_LOCK();
    ret = s_sprd_codec_state.st_run.runing;
    SPRD_CODEC_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_runing(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,SPRD_CODEC_RUNING);
#else
    SPRD_CODEC_G_LOCK();
    s_sprd_codec_state.st_run.runing = set;
    SPRD_CODEC_G_UNLOCK();
#endif
}

LOCAL uint32 _is_locking()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(SPRD_CODEC_LOCKING);
#else
    uint32 ret = SCI_ERROR;
    SPRD_CODEC_G_LOCK();
    ret = s_sprd_codec_state.st_run.locking;
    SPRD_CODEC_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_locking(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,SPRD_CODEC_LOCKING);
#else
    SPRD_CODEC_G_LOCK();
    s_sprd_codec_state.st_run.locking = set;
    SPRD_CODEC_G_UNLOCK();
#endif
}

LOCAL uint32 _is_blocking()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(SPRD_CODEC_BLOCKING);
#else
    uint32 ret = SCI_ERROR;
    SPRD_CODEC_G_LOCK();
    ret = s_sprd_codec_state.st_run.blocking;
    SPRD_CODEC_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_blocking(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,SPRD_CODEC_BLOCKING);
#else
    SPRD_CODEC_G_LOCK();
    s_sprd_codec_state.st_run.blocking = set;
    SPRD_CODEC_G_UNLOCK();
#endif
}

LOCAL uint32 _is_delaing()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(SPRD_CODEC_DELAING);
#else
    uint32 ret = 0;
    SPRD_CODEC_G_LOCK();
    ret = s_sprd_codec_state.st_run.delaing;
    SPRD_CODEC_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_delaing(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,SPRD_CODEC_DELAING);
#else
    SPRD_CODEC_G_LOCK();
    s_sprd_codec_state.st_run.delaing = set;
    SPRD_CODEC_G_UNLOCK();
#endif
}

LOCAL uint32 _is_waiting()
{
#if (USE_EVENT_TRIGGER)
    return _is_event_set(SPRD_CODEC_WAITING);
#else
    uint32 ret = 0;
    SPRD_CODEC_G_LOCK();
    ret = s_sprd_codec_state.st_run.waiting;
    SPRD_CODEC_G_UNLOCK();
    return ret;
#endif
}

LOCAL void _set_waiting(uint32 set)
{
#if (USE_EVENT_TRIGGER)
    _set_event(set,SPRD_CODEC_WAITING);
#else
    SPRD_CODEC_G_LOCK();
    s_sprd_codec_state.st_run.waiting = set;
    SPRD_CODEC_G_UNLOCK();
#endif
}



LOCAL void _sprd_codec_stag1_dispel_change(void *to)
{
    SPRD_CODEC_STATE_OC_E * pDst = (SPRD_CODEC_STATE_OC_E *)to;

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
    //for SPRD CODEC HP/SPK open need open DAC->HP or LIN->HP SW .
    if( (HP_OF && (pDst[I_HP] == D_CLOSE))
    //for SPRD CODEC    || (HP_ON && (pDst[I_HP] == D_OPEN))
    )
    {
        pDst[I_HP] = D_NOTHING;
    }
    if( (LOUT_OF && (pDst[I_LO] == D_CLOSE))
    //for SPRD CODEC    || (LOUT_ON && (pDst[I_LO] == D_OPEN))
    )
    {
        pDst[I_LO] = D_NOTHING;
    }
    if( (EP_OF && (pDst[I_EP] == D_CLOSE))
        || (EP_ON && (pDst[I_EP] == D_OPEN))
    )
    {
        pDst[I_EP] = D_NOTHING;
    }
    if( (DAC_MUTE && (pDst[I_DAC_MUTE] == D_CLOSE))
        || (DAC_UNMUTE && (pDst[I_DAC_MUTE] == D_OPEN))
    )
    {
        pDst[I_DAC_MUTE] = D_NOTHING;
    }
}

LOCAL void _sprd_codec_stag2_dispel_change(void *to)
{
    SPRD_CODEC_STATE_OC_E * pDst = (SPRD_CODEC_STATE_OC_E *)to;

    if( D_CLOSE == pDst[I_DA] )
    {
        if( DAC_ON || (D_OPEN==pDst[I_DAC]) )
        {
            pDst[I_DAC] = D_CLOSE;
        }
        if( LIN_ON || (D_OPEN==pDst[I_LIN]) )
        {
            pDst[I_LIN] = D_CLOSE;
        }
        if( HP_ON || (D_OPEN==pDst[I_HP]) )
        {
            pDst[I_HP] = D_CLOSE;
        }
        if( LOUT_ON || (D_OPEN==pDst[I_LO]) )
        {
            pDst[I_LO] = D_CLOSE;
        }
        if( EP_ON || (D_OPEN==pDst[I_EP]) )
        {
            pDst[I_EP] = D_CLOSE;
        }
        if( DAC_UNMUTE || (D_OPEN==pDst[I_DAC_MUTE]) )
        {
            pDst[I_DAC_MUTE] = D_CLOSE;
        }
    }

    //for SPRD CODEC , when AD close, need close Record.
    if( D_CLOSE == pDst[I_AD] )
    {
        if( LIN_REC_ON || (D_OPEN==pDst[I_LIR]) )
        {
            pDst[I_LIR] = D_CLOSE;
        }
    }
}

//for SPRD CODEC HP/SPK open need open DAC->HP or LIN->HP SW .
LOCAL uint32 _sprd_codec_open_filter_merge(uint32 index)
{
    uint32 i;
    uint32 filter_index[] = {
        I_HP,
        I_LO
    };
    for(i=0;i<sizeof(filter_index)/sizeof(filter_index[0]);i++)
    {
        if( index == filter_index[i] )
        {
            //is filter.
            return 1;
        }
    }
    return 0;
}


LOCAL void _sprd_codec_stag1_merge_change(void *to,const void* from)
{
    uint32 i=0;
    SPRD_CODEC_STATE_OC_E * pSrc = (SPRD_CODEC_STATE_OC_E *)from;
    SPRD_CODEC_STATE_OC_E * pDst = (SPRD_CODEC_STATE_OC_E *)to;
    for(i=I_LIN;i<=I_LIR;i++)
    {
        if( D_NOTHING != pSrc[i] )
        {
            if( (D_NOTHING != pDst[i]) && !_sprd_codec_open_filter_merge(i))
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

LOCAL void _sprd_codec_stag2_merge_change(void *to,const void* from)
{
    SPRD_CODEC_STATE_OC_E * pSrc = (SPRD_CODEC_STATE_OC_E *)from;
    SPRD_CODEC_STATE_OC_E * pDst = (SPRD_CODEC_STATE_OC_E *)to;
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


LOCAL void _sprd_codec_merge_change(void *to,const void* from)
{
    _sprd_codec_stag1_merge_change(to,from);
    _sprd_codec_stag2_merge_change(to,from);
}


LOCAL void _sprd_codec_merge_st_change(void *to,const void* from)
{
    _sprd_codec_stag1_merge_change(to,from);
    
    _sprd_codec_stag1_dispel_change(to);

    //SPRD_CODEC_STATE_DGB:"[AUDST]MERGED ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1649_112_2_17_23_3_0_237,(uint8*)"ddddddddd",s_sprd_codec_state.st_run.oc[I_AD],s_sprd_codec_state.st_run.oc[I_LIR],s_sprd_codec_state.st_run.oc[I_DA],s_sprd_codec_state.st_run.oc[I_DAC],s_sprd_codec_state.st_run.oc[I_LIN],s_sprd_codec_state.st_run.oc[I_HP],s_sprd_codec_state.st_run.oc[I_LO],s_sprd_codec_state.st_run.oc[I_EP],s_sprd_codec_state.st_run.oc[I_DAC_MUTE]);
    
    _sprd_codec_stag2_merge_change(to,from);

    _sprd_codec_stag2_dispel_change(to);
}

LOCAL void _sprd_codec_apply_change(void)
{
    SPRD_CODEC_STATE_OC_E  use_sprd_codec_oc[I_MAX] = {0};
    if( _is_locking() )
    {
        return;
    }
    if( !_is_blocking() )
    {
        //SPRD_CODEC_STATE_DGB:"[AUDST] MERGE ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1673_112_2_17_23_3_0_238,(uint8*)"ddddddddd",s_sprd_codec_state.ch[I_AD],s_sprd_codec_state.ch[I_LIR],s_sprd_codec_state.ch[I_DA],s_sprd_codec_state.ch[I_DAC],s_sprd_codec_state.ch[I_LIN],s_sprd_codec_state.ch[I_HP],s_sprd_codec_state.ch[I_LO],s_sprd_codec_state.ch[I_EP],s_sprd_codec_state.ch[I_DAC_MUTE]);
        SPRD_CODEC_G_LOCK();
        SCI_MEMCPY(&(use_sprd_codec_oc),&(s_sprd_codec_state.ch),sizeof(use_sprd_codec_oc));//copy all change.
        SCI_MEMSET(&(s_sprd_codec_state.ch),0,sizeof(s_sprd_codec_state.ch)); //clear all change.
        SPRD_CODEC_G_UNLOCK();
    }
    _sprd_codec_merge_st_change(&(s_sprd_codec_state.st_run.oc),&(use_sprd_codec_oc));
    //SPRD_CODEC_STATE_DGB:"[AUDST] APPLY ad=%d,liR=%d,da=%d,dac=%d,lin=%d,hp=%d,lo=%d,ep=%d,mu=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1689_112_2_17_23_3_0_239,(uint8*)"ddddddddd",s_sprd_codec_state.st_run.oc[I_AD],s_sprd_codec_state.st_run.oc[I_LIR],s_sprd_codec_state.st_run.oc[I_DA],s_sprd_codec_state.st_run.oc[I_DAC],s_sprd_codec_state.st_run.oc[I_LIN],s_sprd_codec_state.st_run.oc[I_HP],s_sprd_codec_state.st_run.oc[I_LO],s_sprd_codec_state.st_run.oc[I_EP],s_sprd_codec_state.st_run.oc[I_DAC_MUTE]);
    AUDIO_HAL_PRTMEM ( (0xCCC00000 
        | ((s_sprd_codec_state.st_run.oc[I_EP]<<16) & 0x00030000)
        | ((s_sprd_codec_state.st_run.oc[I_AD ]<<14) & 0x0000C000)
        | ((s_sprd_codec_state.st_run.oc[I_LIR]<<12) & 0x00003000)
        | ((s_sprd_codec_state.st_run.oc[I_DA ]<<10) & 0x00000C00)
        | ((s_sprd_codec_state.st_run.oc[I_DAC]<<8)  & 0x00000300)
        | ((s_sprd_codec_state.st_run.oc[I_LIN]<<6)  & 0x000000C0)
        | ((s_sprd_codec_state.st_run.oc[I_HP ]<<4)  & 0x00000030)
        | ((s_sprd_codec_state.st_run.oc[I_LO ]<<2)  & 0x0000000C)
        | ((s_sprd_codec_state.st_run.oc[I_DAC_MUTE ]<<0)  & 0x00000003)
        ));
    
}

////////// START OF codec STATE FUNTION //////////////////////////////////////////
LOCAL void _sprd_codec_change_state(SPRD_CODEC_STATE_E st)
{
    SCI_ASSERT(st<MAX_S); /*assert verified*/
#ifdef CODEC_DEBUG
    //SPRD_CODEC_STATE_DGB:"[AUDST]change state pre=%s,cur=%s,set=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1712_112_2_17_23_3_0_240,(uint8*)"sss",_sprd_codec_s_str_tbl[s_sprd_codec_state.st_run.pre_st],_sprd_codec_s_str_tbl[s_sprd_codec_state.st_run.cur_st],_sprd_codec_s_str_tbl[st]);
    AUDIO_HAL_TICK_DBG("[AUDTICK] set st=%s %d",_sprd_codec_s_str_tbl[st],SCI_GetTickCount());
#endif
    AUDIO_HAL_PRTMEM ( (0x55550000 
        | ((s_sprd_codec_state.st_run.pre_st<<10) & 0x00007C00)
        | ((s_sprd_codec_state.st_run.cur_st<<5) & 0x00000370)
        | ((st<<0) & 0x0000001F)
        ));
    s_sprd_codec_state.st_run.pre_st = s_sprd_codec_state.st_run.cur_st;
    SPRD_CODEC_G_LOCK();
    s_sprd_codec_state.st_run.cur_st = st;
    SPRD_CODEC_G_UNLOCK();
}

LOCAL void _sprd_codec_init_state(void)
{
    SCI_MEMSET(&(s_sprd_codec_state.st_run),0,sizeof(s_sprd_codec_state.st_run));
}

LOCAL void _sprd_codec_timer_wait_callback(uint32 tmp)
{
    SCI_DeactiveTimer(s_sprd_codec_state.timer);
    _set_waiting(0);
    SPRD_CODEC_SM_Process();
}

LOCAL uint32 _sprd_codec_timer_wait(uint32 time)
{
    if( time )
    {
        _set_waiting(1);
        // start timer.
        SCI_ChangeTimer(s_sprd_codec_state.timer, _sprd_codec_timer_wait_callback, time);
        SCI_ActiveTimer(s_sprd_codec_state.timer);
        return EXIT_RUNNING;
    }
    return RUN_TO_NEXT;
}

LOCAL void _sprd_codec_timer_wait_cancel()
{
    if( _is_waiting() )
    {
        SCI_DeactiveTimer(s_sprd_codec_state.timer);
        _set_waiting(0);
    }
}



///timer task call this ! need protect.
LOCAL void _sprd_codec_timer_delay_callback(uint32 tmp)
{
    SCI_DeactiveTimer(s_sprd_codec_state.timer);
    _set_delaing(0);
    SPRD_CODEC_SM_Process();
}


LOCAL uint32 _sprd_codec_timer_delay(uint32 time)
{
    if( time )
    {
        _set_delaing(1);
        // start timer.
        if( SPRD_CODEC_TIME_CONT != time )
        {
            SCI_ChangeTimer(s_sprd_codec_state.timer, _sprd_codec_timer_delay_callback, time);
        }
        SCI_ActiveTimer(s_sprd_codec_state.timer);
        return EXIT_RUNNING;
    }
    return RUN_TO_NEXT;
}

LOCAL __inline void _sprd_codec_on_off_set(SPRD_CODEC_STATE_ONOFF_E *pSrc,uint32 onoff)
{
    *pSrc = onoff;
}

LOCAL  __inline void _sprd_codec_oc_set(SPRD_CODEC_STATE_OC_E *pSrc,uint32 oc)
{
    *pSrc = oc;
}

LOCAL  __inline uint32 _sprd_codec_is_oc(SPRD_CODEC_STATE_OC_E *pSrc,uint32 oc)
{
    return (*pSrc == oc);
}



LOCAL void _sprd_codec_notify_data(uint32 need)
{
    SPRD_CODEC_G_LOCK();
    if(need)
    {
        ///TODO: need send data.
        s_sprd_codec_state.is_ready = 1;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_SEND_READY);
    }
    else
    {
        ///TODO: maybe can sotp send data.
        s_sprd_codec_state.is_ready = 0;
        __AUDIO_HAL_Codec_Notify_Msg(AUDIO_HAL_CODEC_SEND_Z);
    }
    SPRD_CODEC_G_UNLOCK();
}

LOCAL void _sprd_codec_stop_all(uint32 is_open)
{
    ///TODO: STOP ALL.
    SCI_DeactiveTimer(s_sprd_codec_state.timer);
    if( !is_open )
    {
        _sprd_codec_init_state();
    }
    else
    {
        SCI_MEMSET(&(s_sprd_codec_state.st_run.oc),0,sizeof(s_sprd_codec_state.st_run.oc));
    }


    __run_callback(is_open);
    __sprd_codec_ap_reg_print();
}

LOCAL void _sprd_codec_task_init(void)
{
    if( AUDIO_MAGIC_NUM != s_sprd_codec_state.magic_num )
    {
        s_sprd_codec_state.magic_num = AUDIO_MAGIC_NUM;
        SCI_MEMCPY(s_sprd_codec_state.magic_str,AUDIO_MAGIC_STR,AUDIO_MAGIC_STR_MAX_LEN);
        
        _sprd_codec_init_state();
        s_sprd_codec_state.timer = SCI_CreatePeriodTimer ("_SPRD_CODEC_SM_TIMER",
                           _sprd_codec_timer_delay_callback, 0, 5, SCI_NO_ACTIVATE);
#if (USE_EVENT_TRIGGER)
        s_sprd_codec_state.event = SCI_CreateEvent("_SPRD_CODEC_SM_EVENT");
#endif
#if (USE_INT_DELAY)
    {
        uint32 status;
        #if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		#else
        status = ISR_RegHandler_Ex(SPRD_CODEC_HP_POP_INT, _sprd_codec_ap_isr, 
            _sprd_codec_ap_hisr, CHIPDRV_HISR_PRIO_2, NULL);
        SCI_PASSERT ( (TB_SUCCESS == status),/*assert to do*/
                      ("%s, %d hp_pop_isr", __MODULE__, __LINE__));
        #endif
        status = ISR_RegHandler_Ex(SPRD_CODEC_DACMUTE_INT, _sprd_codec_dacmute_isr, 
            _sprd_codec_dacmute_hisr, CHIPDRV_HISR_PRIO_2, NULL);
        SCI_PASSERT ( (TB_SUCCESS == status),/*assert to do*/
                      ("%s, %d dacmute_isr", __MODULE__, __LINE__));
    }
#endif
    }
}

LOCAL  void _sprd_codec_send_msg(uint32 msg)
{
    xSignalHeader   signal;

    SCI_CREATE_SIGNAL(
        signal,
        msg,
        sizeof(*signal),
        SCI_IDENTIFY_THREAD());

    SCI_SEND_SIGNAL(signal,s_sprd_codec_state.task_id);
}

/*****************************************************************************/
//  Description:    sprd_codec task entry.
//  Author:         ken.kuang
//  Note:
/*****************************************************************************/
LOCAL void _sprd_codec_thread (uint32 argc, void *argv)
{
    xSignalHeader	signal;
    xSignalHeader	peek_signal;

    _sprd_codec_task_init();

    for(;;)
    {
        SCI_RECEIVE_SIGNAL(signal, s_sprd_codec_state.task_id);
        //**PEEK MSG**
        SCI_PEEK_SIGNAL(peek_signal, s_sprd_codec_state.task_id);
        while( peek_signal )
        {
            SCI_FREE_SIGNAL(peek_signal);
            SCI_PEEK_SIGNAL(peek_signal, s_sprd_codec_state.task_id);
        }
        //**PEEK MSG**
        switch(signal->SignalCode)
        {
            case SPRD_CODEC_START_RUN:
            {
#if (PGA_SMOOTH_ADJUST)
                __sprd_codec_pga_adj_get_mutex();
#endif
                _run_sprd_codec_state();
#if (PGA_SMOOTH_ADJUST)
                __sprd_codec_pga_adj_put_mutex();
#endif
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


PUBLIC void __sprd_codec_create_task(void)
{
    static uint32 do_this = 1;
    if( do_this )
    {
        s_sprd_codec_state.task_id  = SCI_CreateThread(SPRD_CODEC_TASK_NAME,
            SPRD_CODEC_QNAME_NAME,
            _sprd_codec_thread ,
            0,
            0,
            SPRD_CODEC_STACK_SIZE ,
            SPRD_CODEC_QUEUE_NUM,
            SPRD_CODEC_TASK_PRIV,
            SCI_PREEMPT,
            SCI_AUTO_START);
        do_this = 0;
    }
}

LOCAL void _sprd_codec_get_oc(SPRD_CODEC_STATE_OC_E *p_sprd_codec_oc,uint32 func_flag)
{
    uint32 index = 0;
    for(index=0;index<_sprd_codec_state_tbl_len;index++)
    {
        if( func_flag==_sprd_codec_state_tbl[index].func_flag )
        {
            //SPRD_CODEC_STATE_DGB:"[AUDST]index=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPRD_CODEC_STATE_V0_1926_112_2_17_23_3_0_241,(uint8*)"d",index);
            SCI_MEMCPY(p_sprd_codec_oc,&(_sprd_codec_state_tbl[index].sprd_codec_oc),sizeof(_sprd_codec_state_tbl[index].sprd_codec_oc));
            return;
        }
    }
    SCI_ASSERT(0); /*assert verified*/
}

LOCAL void _sprd_codec_change_oc_flag(SPRD_CODEC_STATE_OC_E * p_sprd_codec_oc)
{
    uint32 index = 0;
    for(index=0;index<I_MAX;index++)
    {
        if( p_sprd_codec_oc[index] == D_OPEN )
        {
            p_sprd_codec_oc[index] = D_CLOSE;
        }
        else if( p_sprd_codec_oc[index] == D_CLOSE )
        {
            SCI_ASSERT(0); /*assert verified*/
        }
    }
}

LOCAL void _sprd_codec_mute_all(void)
{
    __sprd_codec_pa_mute(1);
    __sprd_codec_hp_mute(1);
    __sprd_codec_ep_mute(1);
}

#if (USE_INT_DELAY)
INPUT_BUFFER_INIT(int32,64)

LOCAL void _sprd_codec_ap_irq_dis(void)
{
    __sprd_codec_ap_int_set(0,AUDIO_POP_IRQ_MSK);
    #if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    #else
    CHIPDRV_DisableIRQINT(SPRD_CODEC_HP_POP_INT);
    #endif
}

LOCAL void _sprd_codec_ap_irq_en(void)
{
    s_sprd_codec_state.st_run.timeout = 1;
    #if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    #else
    CHIPDRV_EnableIRQINT(SPRD_CODEC_HP_POP_INT);
    #endif
    __sprd_codec_ap_int_clr(AUDIO_POP_IRQ_MSK);
    __sprd_codec_ap_int_set(1,AUDIO_POP_IRQ_MSK);
    AUDIO_HAL_TICK_DBG("[AUDTICK]hp start %d",SCI_GetTickCount());
}

LOCAL ISR_EXE_T _sprd_codec_ap_isr(uint32 tmp)
{
    uint32 msk = __sprd_codec_ap_int_mask();
    IsrWriteBuffer(msk);
    _sprd_codec_ap_irq_dis();
    return CALL_HISR;
}

LOCAL void _sprd_codec_ap_hisr(uint32 count, void *data)
{
    uint32 msk;
    threadReadBuffer ( (int32 *) (&msk));
    if( AUDIO_POP_IRQ_MSK & msk )
    {
        __sprd_codec_ap_int_clr(AUDIO_POP_IRQ_MSK);
        s_sprd_codec_state.st_run.timeout = 0;
        _sprd_codec_timer_delay_callback(0);
        AUDIO_HAL_TICK_DBG("[AUDTICK]hp end %d",SCI_GetTickCount());
    }
}

LOCAL void _sprd_codec_dacmute_irq_dis(void)
{
    CHIPDRV_DisableIRQINT(SPRD_CODEC_DACMUTE_INT);
    __sprd_codec_dp_int_en_u(0);
    __sprd_codec_dp_int_en_d(0);
}

LOCAL void _sprd_codec_dacmute_irq_en(void)
{
    CHIPDRV_EnableIRQINT(SPRD_CODEC_DACMUTE_INT);
    __sprd_codec_dp_int_clr_u();
    __sprd_codec_dp_int_clr_d();
    __sprd_codec_dp_int_en_u(1);
    __sprd_codec_dp_int_en_d(1);
    AUDIO_HAL_TICK_DBG("[AUDTICK]dacmute start %d",SCI_GetTickCount());
}

enum {
    DP_INT_STS_U = 0x1,
    DP_INT_STS_D = 0x2,

    DP_INT_STS_MAX = 0xFFFFFFFF
};

LOCAL ISR_EXE_T _sprd_codec_dacmute_isr(uint32 tmp)
{
    uint32 flg = 0;
    if( __sprd_codec_dp_int_sts_u() )
    {
        flg |= (DP_INT_STS_U);
    }
    else if( __sprd_codec_dp_int_sts_d() )
    {
        flg |= (DP_INT_STS_D);
    }
    IsrWriteBuffer(flg);
    _sprd_codec_dacmute_irq_dis();
    return CALL_HISR;
}

LOCAL void _sprd_codec_dacmute_hisr(uint32 count, void *data)
{
    uint32 flg;
    threadReadBuffer ( (int32 *) (&flg));
    if( DP_INT_STS_U & flg )
    {
        __sprd_codec_dp_int_clr_u();
        _sprd_codec_timer_delay_callback(0);
    }
    else if( DP_INT_STS_D & flg )
    {
        __sprd_codec_dp_int_clr_d();
        _sprd_codec_timer_wait_cancel();
        SPRD_CODEC_SM_Process();
    }
    AUDIO_HAL_TICK_DBG("[AUDTICK]dacmute end %d",SCI_GetTickCount());
}

#endif


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


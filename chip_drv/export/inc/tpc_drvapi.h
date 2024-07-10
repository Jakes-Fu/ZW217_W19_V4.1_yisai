/******************************************************************************
 ** File Name:      Tpc_drvapi.h                                                *
 ** Author:         tao.feng                                            *
 ** DATE:           06/10/2008                                              *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define tpc module reg infomation*
 **                                                               *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/10/2008     tao.feng         Create.                                  *                                              *
 ******************************************************************************/

#ifndef _TPC_DRVAPI_H_
#define _TPC_DRVAPI_H_

typedef float FLOAT_TYPE;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

// Touch panel constant.
#define TB_TP_CONST_BASE       0x90
#define TB_TP_DOWN           TB_TP_CONST_BASE
#define TB_TP_MOVE          (TB_TP_CONST_BASE + 1)
#define TB_TP_UP            (TB_TP_CONST_BASE + 2)


#define  TP_SCI_ASSERT(_EXP)        SCI_ASSERT(_EXP)
#define  TP_IRQ                     24
#define  ADC_IRQ                    14
#define  TMP_BUF_LEN_MAX            16
#define  DOWN_IRQ                   (uint8)0x01
#define  UP_IRQ                     (uint8)0x02
#define  DONE_IRQ                   (uint8)0x03
#define  NVALID_IRQ                 (uint8)0xff
#define  TP_CAL_SCALAR              0x08
#define  TP_QUEFUL_THRESHOLD        0x0a

typedef enum
{
    TPC_WORK_STATE_INVALID,     //Invalid state
    TPC_WORK_STATE_ACTIVE,      //The device is active
    TPC_WORK_STATE_OPEN,            //The device is open
    TPC_WORK_STATE_STOP,                 //The device is suspend
    TPC_WORK_STATE_CLOSE,       //The device is close
    TPC_WORK_MODE_MAX
} TPC_WORK_STATE_E;

typedef struct TP_DATA
{
    uint16 x;
    uint16 y;
} TP_DATA_TYPE_T;

typedef struct
{
    uint16  x_slope;
    uint16  x_offset;
    uint16  y_slope;
    uint16  y_offset;
    BOOLEAN cal_en;// cal enable :will read coord value throgh TPC_BUF_DATA;cal disable :will read adc value throgh TPC_BUF_DATA
} TP_CALI_T ;

typedef struct
{
    TP_CALI_T   cal_info;// used to store the cal info
    uint8       buf_len;// tpc fifo depth
} TP_INFO;

typedef enum
{
    TP_DONE_IRQ_200US,
    TP_DONE_IRQ_1MS,
    TP_DONE_IRQ_5MS,
    TP_DONE_IRQ_10MS,
    TP_DONE_IRQ_20MS
} TP_DONE_IRQ_PERIOD_E;

typedef enum
{
    TP_DOWN_IRQ_MASK,
    TP_DONE_IRQ_MASK,
    TP_UP_IRQ_MASK,
    TP_IRQ_MASK_ALL
} TP_IRQ_MASK_E;

typedef enum
{
    ADC_LARGE_RANGE = 0,
    ADC_SMALL_RANGE = 1
} ADC_RANGE_E;

typedef struct
{
    void (*tp_drv_init) (void);
    void (*tp_drv_start_scan) (void);
    void (*tp_drv_get_data) (void);
    void (*tp_drv_stop) (void);
    void (*tp_drv_pause) (void);
    void (*tp_drv_resume) (void);
    void (*tp_irq_mask) (TP_IRQ_MASK_E irq);
    void (*tp_irq_en) (void);
    void (*tp_flush_buf) (void);
    void (*tp_conf_done_irq_period) (TP_DONE_IRQ_PERIOD_E period);
    void (*_tp_set_adc_range) (ADC_RANGE_E range);
    void (*_tp_clr_irq_status) (void);
    void (*tp_conf_cal_parameter) (void);
    TP_INFO * (*tp_drv_get_info) (void);
    void * (*_tp_get_data_buf) (void);
    void (*_tp_enable) (void);
    void (*_tp_disable) (void);
    BOOLEAN (*_tp_detect_up) (void);
    void (*_tp_drv_clac_enable) (void);
    void (*_tp_drv_clac_disable) (void);
} TP_DRV_FUNC_T;

/*****************************************************************************/
//  Function name:  tp_drv_init
//  Description:    tpc module driver init
//  Global resource dependence:
//  Author: tao.feng
//  Note:should be called before active TPC module
/*****************************************************************************/
PUBLIC void tp_drv_init (void);

/*****************************************************************************/
//  Function name:  TP_GetDrvFunc
//  Description:this function get the pointer of the driver function
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TP_DRV_FUNC_T *TP_GetDrvFunc (void);
/*****************************************************************************/
//  Function name:  TP_XL_Deshaking_Callback
//  Description:this function send DOWN_IRQ signal to TP task
//  Global resource dependence:
//  Author: Younger.yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 TP_XL_Deshaking_Callback (BOOLEAN param);

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         DATA STRUCTURE Definition                         *
 **---------------------------------------------------------------------------*/
/*
    Sequence name for a sampling
*/
typedef enum
{
	TPC_SEQ_Y = 0,
	TPC_SEQ_Y1,
	TPC_SEQ_X,
	TPC_SEQ_X1,	
	TPC_SEQ_RY0,
	TPC_SEQ_RX0,
	TPC_SEQ_RY,	
	TPC_SEQ_RX,
	TPC_SEQ_RT,
	TPC_SEQ_RT_X,	
	TPC_SEQ_RT_Y,	
	TPC_SEQ_RS,
    TPC_SEQ_VDDIO_X	,
    TPC_SEQ_VDDIO_Y	,    
	TPC_SEQ_MAX,	
} TPC_SEQ_TYPE_E;

typedef struct _tpc_seq_data_t
{
    FLOAT_TYPE      data;    
}   TPC_SEQ_DATA_T, *TPC_SEQ_DATA_PTR;

/*
  The information getting from calibrating process
*/
typedef struct _calibrated_data_t
{
    FLOAT_TYPE Rx0;
    FLOAT_TYPE Ry0; 
    FLOAT_TYPE Rs0;
    FLOAT_TYPE Rt0;    
} CALIBRATED_DATA_T, *CALIBRATED_DATA_PTR;

/*
  The information for NV struct
*/
typedef struct nv_tp_param
{
    uint32    Rx0;
    uint32    Ry0;
    uint32    Rt0;
    uint32    tp_width;
    uint32    tp_height;
    uint32    valid_flag;
}  NV_TP_PARAM_T, *NV_TP_PARAM_PTR;

typedef union
{
    struct
    {
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
        uint16 data_x;
        uint16 data_y;
#else
        uint16 data_y;
        uint16 data_x;
#endif

    } data;
    uint32 dwValue;
} TPC_DATA_U;

/**---------------------------------------------------------------------------*
 **                         PUBLIC DECLARATION                                *
 **---------------------------------------------------------------------------*/
extern  void TPDRV_Init (void);
extern  void TPDRV_Enable (void);
extern  void TPDRV_Disable (void);
extern  void TPDRV_EnableCoord (void);
extern  void TPDRV_DisableCoord (void);
extern  void TPDRV_EnterCalibration (void);
extern  void TPDRV_EnterMulti (void);
extern  void TPDRV_ConfigCoord (int x_slop, int x_off, int y_slop,
                                int y_offset,BOOLEAN xy_exchange);

extern  void               TPDRV_SaveCalibratedData(CALIBRATED_DATA_T data);
extern  void               TPDRV_SetCalibratedData(CALIBRATED_DATA_T data);
extern  CALIBRATED_DATA_T* TPDRV_GetCalibratedData( void );

#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
PUBLIC int TP_Calibrantion_Prepare(void);
#else
LOCAL __inline int TP_Calibrantion_Prepare(void)
{
    return 0;
}
#endif


#ifdef __cplusplus
}
#endif

#endif //_TPC_DRVAPI_H_



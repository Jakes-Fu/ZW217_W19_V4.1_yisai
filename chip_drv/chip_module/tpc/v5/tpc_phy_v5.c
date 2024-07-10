/******************************************************************************
 ** File Name:        Tpc_phy_v5.c                                               *
 ** Author:           Yi.Qiu                                                  *
 ** DATE:             09/16/2009                                              *
 ** Copyright:        2009 Spreadtrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE            NAME            DESCRIPTION                               *
 ** 06/10/2008      Tao.Feng        Create                                    *
 ** 09/16/2009      Yi.Qiu          Reconstruct TPC driver                    *
 ******************************************************************************/

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "clock_drvapi.h"
#include "adi_hal_internal.h"
#include "task_monitor.h"
#include "tpc_reg_v5.h"
#include "pwm_drvapi.h"
#include "deepsleep_drvapi.h"
#include "tpc_drvapi.h"
#include "adc_drvapi.h"
#include "adc_internal.h"

#include "lcd_cfg.h"
#include "Tp_cfg.h"
#include "tpc_algorithm.h"
#include "../../adc/V5/Adc_reg_v5.h"
#include "tpc_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/

#define TPC_MODE_NULL           0
#define TPC_CALIBRATION_MODE    1
#define TPC_MULTITOUCH_MODE     2
/*
    Each adc channel has different range.
    ADC_TPC_SCALE is the ratio of adc0/adc1 to tpc channel.
*/
 FLOAT_TYPE ADC_TPC_SCALE=1.72;


uint32                s_tpc_mode = TPC_CALIBRATION_MODE;
//uint32                s_tpc_mode = TPC_MULTITOUCH_MODE;

#define _TPDRV_DEBUG
#ifdef _TPDRV_DEBUG
#include "sci_log.h"

#define TPDRV_ASSERT                 SCI_ASSERT
#define TPDRV_LOG_TRACE              SCI_TRACE_LOW
/*
#define TPDRV_ASSERT(_x_)                SCI_ASSERT(_x_)
#define TPDRV_LOG_TRACE(_x_)            SCI_TraceLow _x_*/
#define TPDRV_VERIFY(_x_)                SCI_ASSERT(_x_)
#define TPDRV_ERR_TRACE(_x_)            SCI_TraceLow _x_
#define TPDRV_INF_TRACE(_x_)            SCI_TraceLow _x_

#else
#define TPDRV_ASSERT(_x_)                ((void)0)
#define TPDRV_VERIFY(_x_)                ((void)(_x_))
#define TPDRV_ERR_TRACE(_x_)            ((void)0)
#define TPDRV_LOG_TRACE(_x_)            ((void)0)
#define TPDRV_INF_TRACE(_x_)            ((void)0)
#endif /* _TPC_DEBUG */

#define TPC_REG_OR       ANA_REG_OR
#define TPC_REG_AND      ANA_REG_AND
#define TPC_REG_GET      ANA_REG_GET
#define TPC_REG_SET      ANA_REG_SET
#define TPC_REG_MSK_OR(addr, value, mask) \
                         TPC_REG_AND(addr, ~(mask));\
                         TPC_REG_OR (addr, (value))
#define CLEAR_TPC_INT(x) TPC_REG_OR(TPC_INT_CLR, x)

#define TPC_BUF_LENGTH          1


#define TPC_INT_DOWN            1
#define TPC_INT_UP              2
#define TPC_INT_DONE            4

INPUT_BUFFER_INIT (int32,64)

/******************************************************************************
 * Structure definitions
 ******************************************************************************/
typedef struct _tpc_seq_index_t
{
    uint32         index;  
    TPC_SEQ_TYPE_E seq_type;  
}   TPC_SEQ_INDEX_T, *TPC_SEQ_INDEX_PTR;

typedef struct _tpc_coordinate_coef_t
{
    int x_slop;
    int x_offset;
    int y_slop;
    int y_offset; 
    uint32 is_coordinated;	
}   TPC_COORDINATE_COEF_T, *TPC_COORDINATE_COEF_PTR;

typedef struct _tpc_cmd_sequence_t
{
    TPC_SEQ_TYPE_E seq_type;

    uint32 out_yu;  // 1--xx pad output; 0--xx pad z
    uint32 out_yd;
    uint32 out_xl;
    uint32 out_xr;
	
    uint32 oe_yu;  // 1--xx pad oe; 0--xx pad z
    uint32 oe_yd;
    uint32 oe_xl;
    uint32 oe_xr;
	
    uint32 ch_yu;  // 1--xx channe is selected; 0--NULL
    uint32 ch_yd;
    uint32 ch_xl;
    uint32 ch_xr;

    uint32 res_y;  // 1--y resistence selection; 0-NULL
    uint32 res_x;  // 1--x resistence selection; 0-NULL
    uint32 swt_on; // 1--switch on; 0--switch off.	
}   TPC_CMD_SEQ_T, *TPC_CMD_SEQ_PTR;

TPC_COORDINATE_COEF_T s_coordinate_coef = {0};
TPC_SEQ_DATA_T        s_tpc_seq_data[TPC_SEQ_MAX] ={0};
TPC_SEQ_INDEX_T       s_tpc_seq_index[8] ={0};
LOCAL uint32          s_seq_x_index=SCI_FALSE;
LOCAL uint32          s_seq_y_index=SCI_FALSE;
// cmd sequence for test.(Static mode)
TPC_CMD_SEQ_T s_tp_SeqTest[] =
{
//---OUT--------------OE-------------CH------------RES---------SWT-------- //
// YU-YD-XL-XR      YU-YD-XL-XR    YU-YD-XL-XR   Yres:Xres   is_on
{ TPC_SEQ_RY,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  },          
{ TPC_SEQ_RX,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },
{ TPC_SEQ_VDDIO_Y,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   0,   0,     0,  },   
{ TPC_SEQ_VDDIO_X,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   0,     0,  },

{ TPC_SEQ_RT_Y,
  1, 1, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   1,     0,  },  
};

// Cmd sequence for single point 
// Y->X->delta_Ry->delta_Rx->R1
TPC_CMD_SEQ_T s_tp_SeqYx[] =
#ifndef TP_EXCHANGE_XY
{
//---OUT--------------OE-------------CH------------RES---------SWT-------- //
// YU-YD-XL-XR      YU-YD-XL-XR    YU-YD-XL-XR   Yres:Xres   is_on
{ TPC_SEQ_Y,
  1, 0, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   0,     0,  },          
{ TPC_SEQ_X,
  0, 0, 1, 0,      0, 0, 1, 1,    0, 1, 0, 0,   0,   0,     0,  },
{ TPC_SEQ_RY,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  },  
{ TPC_SEQ_RX,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },

{ TPC_SEQ_RT_Y,
  1, 1, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   1,     0,  },  
};
#else
{
//---OUT--------------OE-------------CH------------RES---------SWT-------- //
// YU-YD-XL-XR      YU-YD-XL-XR    YU-YD-XL-XR   Yres:Xres   is_on
{ TPC_SEQ_X,
  1, 0, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   0,     0,  },          
{ TPC_SEQ_Y,
  0, 0, 1, 0,      0, 0, 1, 1,    0, 1, 0, 0,   0,   0,     0,  },
{ TPC_SEQ_RX0,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  },  
{ TPC_SEQ_RY0,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },

{ TPC_SEQ_RT_X,
  1, 1, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   1,     0,  },  
};
#endif

// Y->X->Ry0->Rx0->RT_y->RT_x->delta_Ry->delta_Rx
TPC_CMD_SEQ_T s_tp_SeqCalibrate[] =
#ifndef TP_EXCHANGE_XY
{
//---OUT--------------OE-------------CH------------RES---------SWT-------- //
// YU-YD-XL-XR      YU-YD-XL-XR    YU-YD-XL-XR   Yres:Xres   is_on
{ TPC_SEQ_Y,
  1, 0, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   0,     0,  },          
{ TPC_SEQ_X,
  0, 0, 1, 0,      0, 0, 1, 1,    0, 1, 0, 0,   0,   0,     0,  },
{ TPC_SEQ_RY0,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  },  
{ TPC_SEQ_RX0,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },

{ TPC_SEQ_RT_Y,
  1, 1, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   1,     0,  },  
{ TPC_SEQ_RT_X,
  0, 0, 1, 1,      0, 0, 1, 1,    0, 1, 0, 0,   1,   0,     0,  },
{ TPC_SEQ_RY,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  }, 
{ TPC_SEQ_RX,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },
};
#else
{
//---OUT--------------OE-------------CH------------RES---------SWT-------- //
// YU-YD-XL-XR      YU-YD-XL-XR    YU-YD-XL-XR   Yres:Xres   is_on
{ TPC_SEQ_X,
  1, 0, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   0,     0,  },          
{ TPC_SEQ_Y,
  0, 0, 1, 0,      0, 0, 1, 1,    0, 1, 0, 0,   0,   0,     0,  },
{ TPC_SEQ_RX0,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  },  
{ TPC_SEQ_RY0,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },

{ TPC_SEQ_RT_X,
  1, 1, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   1,     0,  },  
{ TPC_SEQ_RT_Y,
  0, 0, 1, 1,      0, 0, 1, 1,    0, 1, 0, 0,   1,   0,     0,  },
{ TPC_SEQ_RX,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  }, 
{ TPC_SEQ_RY,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },
};
#endif

// Cmd sequence for Multi touch
// Y->Y1->X->X1->Ry->Rx->RT
TPC_CMD_SEQ_T s_tp_SeqMultitouch[] =
#ifndef TP_EXCHANGE_XY
{
//PinConnect Map
//TP_XL <--> X+;  TP_XR <--> X-
//TP_YU <--> Y+;  TP_YD <--> Y-
//-OUT--------------OE-------------CH------------RES---------SWT-------- //
//YU-YD-XL-XR      YU-YD-XL-XR    YU-YD-XL-XR   Yres-Xres   on/off
{ TPC_SEQ_Y,
  1, 0, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   0,     0,  },          
{ TPC_SEQ_Y1,
  1, 0, 0, 0,      1, 1, 0, 0,    0, 0, 1, 0,   0,   0,     0,  },
{ TPC_SEQ_X,
  0, 0, 1, 0,      0, 0, 1, 1,    0, 1, 0, 0,   0,   0,     0,  },
{ TPC_SEQ_X1,
  0, 0, 1, 0,      0, 0, 1, 1,    1, 0, 0, 0,   0,   0,     0,  },

{ TPC_SEQ_RY,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  },  
{ TPC_SEQ_RX,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },
{ TPC_SEQ_RT,
  1, 1, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   1,     0,  },
};

#else
{
//PinConnect Map
//TP_XL <--> Y+;  TP_XR <--> Y-
//TP_YU <--> X+;  TP_YD <--> X-
// X1->X->Y1->Y->Rx->Ry->RT
//-OUT--------------OE-------------CH------------RES---------SWT-------- //
//YU-YD-XL-XR      YU-YD-XL-XR    YU-YD-XL-XR   Yres-Xres   on/off
{ TPC_SEQ_X,
  1, 0, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   0,     0,  },          
{ TPC_SEQ_X1,
  1, 0, 0, 0,      1, 1, 0, 0,    0, 0, 1, 0,   0,   0,     0,  },
{ TPC_SEQ_Y,
  0, 0, 1, 0,      0, 0, 1, 1,    0, 1, 0, 0,   0,   0,     0,  },
{ TPC_SEQ_Y1,
  0, 0, 1, 0,      0, 0, 1, 1,    1, 0, 0, 0,   0,   0,     0,  },

{ TPC_SEQ_RX,
  1, 0, 0, 0,      1, 0, 0, 0,    0, 1, 0, 0,   1,   0,     0,  },  
{ TPC_SEQ_RY,
  0, 0, 1, 0,      0, 0, 1, 0,    0, 0, 0, 1,   0,   1,     0,  },
{ TPC_SEQ_RT,
  1, 1, 0, 0,      1, 1, 0, 0,    0, 0, 0, 1,   0,   1,     0,  },
};
#endif

/******************************************************************************
 * LOCAL fucntion declaration
 ******************************************************************************/
// TPC device handle
LOCAL DEVICE_HANDLE s_dev_tpc = SCI_NULL;

LOCAL TP_DP_CFG_PTR s_tp_dp_cfg_ptr;
LOCAL FLOAT_TYPE TP_WIDTH,TP_HEIGHT, TP_REF_U,TP_REF_R;
LOCAL BOOLEAN is_first_down = SCI_TRUE;

LOCAL ISR_EXE_T _IrqHandler (void);
LOCAL void _Run (void);
LOCAL void _Stop (void);
LOCAL uint32 _FetchData (TPC_DATA_U *data, uint32 size);
LOCAL void _TPCHisrFunc (uint32 i, void *d);
LOCAL void _HandleTPCHisr (int int_type);

void ADIE_TPC_REGLIST( char* cstring )
{
    //SCI_TraceLow:"\r\n %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_255_112_2_17_23_7_28_1358,(uint8*)"s", cstring);
    //SCI_TraceLow:" --->:ADC_HW_CH_DELAY =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_256_112_2_17_23_7_28_1359,(uint8*)"d", TPC_REG_GET(ADC_HW_CH_DELAY) );
    //SCI_TraceLow:" --->:ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL) =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_257_112_2_17_23_7_28_1360,(uint8*)"d", TPC_REG_GET(ADC_SLOW_HW_CHx_CFG(TPC_YD_REQ_CHANNEL)) );
    //SCI_TraceLow:" --->:ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL) =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_258_112_2_17_23_7_28_1361,(uint8*)"d", TPC_REG_GET(ADC_SLOW_HW_CHx_CFG(TPC_XR_REQ_CHANNEL)) );
    //SCI_TraceLow:" --->:ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL) =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_259_112_2_17_23_7_28_1362,(uint8*)"d", TPC_REG_GET(ADC_SLOW_HW_CHx_CFG(TPC_YU_REQ_CHANNEL)) );
    //SCI_TraceLow:" --->:ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL) =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_260_112_2_17_23_7_28_1363,(uint8*)"d", TPC_REG_GET(ADC_SLOW_HW_CHx_CFG(TPC_XL_REQ_CHANNEL)) );
                         
    //SCI_TraceLow:" --->:TPC_SAMPLE_CTRL0 =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_262_112_2_17_23_7_28_1364,(uint8*)"d", TPC_REG_GET(TPC_SAMPLE_CTRL0) );
    //SCI_TraceLow:" --->:TPC_SAMPLE_CTRL1 =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_263_112_2_17_23_7_28_1365,(uint8*)"d", TPC_REG_GET(TPC_SAMPLE_CTRL1) );
    //SCI_TraceLow:" --->:TPC_SAMPLE_CTRL2 =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_264_112_2_17_23_7_28_1366,(uint8*)"d", TPC_REG_GET(TPC_SAMPLE_CTRL2) );
    //SCI_TraceLow:" --->:TPC_BOUNCE_CTRL   =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_265_112_2_17_23_7_28_1367,(uint8*)"d", TPC_REG_GET(TPC_BOUNCE_CTRL) );
    //SCI_TraceLow:" --->:TPC_FILTER_CTRL   =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_266_112_2_17_23_7_28_1368,(uint8*)"d", TPC_REG_GET(TPC_FILTER_CTRL) );
    //SCI_TraceLow:" --->:TPC_CALC_CTRL    =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_267_112_2_17_23_7_28_1369,(uint8*)"d", TPC_REG_GET(TPC_CALC_CTRL) );
    //SCI_TraceLow:" --->:TPC_BUF_CTRL    =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_268_112_2_17_23_7_28_1370,(uint8*)"d", TPC_REG_GET(TPC_BUF_CTRL) );

    //SCI_TraceLow:" --->:TPC_CTRL         =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_270_112_2_17_23_7_28_1371,(uint8*)"d", TPC_REG_GET(TPC_CTRL) );
    //SCI_TraceLow:" --->:TPC_DATA_MIN     =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_271_112_2_17_23_7_28_1372,(uint8*)"d", TPC_REG_GET(TPC_DATA_MIN) );
    //SCI_TraceLow:" --->:TPC_DATA_MAX     =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_272_112_2_17_23_7_28_1373,(uint8*)"d", TPC_REG_GET(TPC_DATA_MAX) );
    //SCI_TraceLow:" --->:TPC_CALC_X_COEF_A =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_273_112_2_17_23_7_28_1374,(uint8*)"d", TPC_REG_GET(TPC_CALC_X_COEF_A) );
    //SCI_TraceLow:" --->:TPC_CALC_X_COEF_B =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_274_112_2_17_23_7_28_1375,(uint8*)"d", TPC_REG_GET(TPC_CALC_X_COEF_B) );
    //SCI_TraceLow:" --->:TPC_CALC_Y_COEF_A =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_275_112_2_17_23_7_28_1376,(uint8*)"d", TPC_REG_GET(TPC_CALC_Y_COEF_A) );
    //SCI_TraceLow:" --->:TPC_CALC_Y_COEF_B =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_276_112_2_17_23_7_28_1377,(uint8*)"d", TPC_REG_GET(TPC_CALC_Y_COEF_B) );

    //SCI_TraceLow:" --->:TPC_INT_EN         =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_278_112_2_17_23_7_28_1378,(uint8*)"d", TPC_REG_GET(TPC_INT_EN) );
    //SCI_TraceLow:" --->:TPC_INT_RAW         =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_279_112_2_17_23_7_28_1379,(uint8*)"d", TPC_REG_GET(TPC_INT_RAW) );
    //SCI_TraceLow:" --->:TPC_INT_CLR         =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_280_112_2_17_23_7_28_1380,(uint8*)"d", TPC_REG_GET(TPC_INT_CLR) );
    
    //SCI_TraceLow:" --->:TPC_OUT_SEQ1     =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_282_112_2_17_23_7_28_1381,(uint8*)"d", TPC_REG_GET(TPC_OUT_SEQ1) );
    //SCI_TraceLow:" --->:TPC_OUT_SEQ0     =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_283_112_2_17_23_7_28_1382,(uint8*)"d", TPC_REG_GET(TPC_OUT_SEQ0) );
    //SCI_TraceLow:" --->:TPC_OE_SEQ1      =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_284_112_2_17_23_7_28_1383,(uint8*)"d", TPC_REG_GET(TPC_OE_SEQ1) );
    //SCI_TraceLow:" --->:TPC_OE_SEQ0      =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_285_112_2_17_23_7_28_1384,(uint8*)"d", TPC_REG_GET(TPC_OE_SEQ0) );
    //SCI_TraceLow:" --->:TPC_CH_SEQ       =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_286_112_2_17_23_7_28_1385,(uint8*)"d", TPC_REG_GET(TPC_CH_SEQ) );
    //SCI_TraceLow:" --->:TPC_RES_SEQ      =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_287_112_2_17_23_7_28_1386,(uint8*)"d", TPC_REG_GET(TPC_RES_SEQ) );
    //SCI_TraceLow:" --->:TPC_SWT_SEQ      =0x%8x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_288_112_2_17_23_7_28_1387,(uint8*)"d", TPC_REG_GET(TPC_SWT_SEQ) );
    
    //SCI_TraceLow:"\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_290_112_2_17_23_7_28_1388,(uint8*)"");
}

LOCAL void _seq_set_addr( uint32 seq_num )
{
    TPC_REG_SET(TPC_SEQ_ADDR, (seq_num<<TPC_SEQ_ADDR_OFFSET));
}

LOCAL uint32 _seq_get_data()
{
    return (TPC_REG_GET(TPC_SEQ_DATA));
}

/******************************************************************************/
//  Description:  Validate the sequence confiure table
//  Author:         
//  Note:  
/******************************************************************************/
#define ANA_INPUT_YU  3
#define ANA_INPUT_YD  2
#define ANA_INPUT_XL  1
#define ANA_INPUT_XR  0
LOCAL void _seq_validate(TPC_CMD_SEQ_PTR seq_ptr, uint32 seq_size)
{
    uint32 index=0;
	uint32 temp=0;

    SCI_ASSERT(seq_ptr != SCI_NULL);/*assert verified*/	
    SCI_ASSERT(seq_size <= 8);/*assert verified*/

    TPC_REG_AND(TPC_SAMPLE_CTRL0, ~(TPC_SAMPLE_CTRL0_SEQ_NUM_MSK) );
    TPC_REG_OR (TPC_SAMPLE_CTRL0, (seq_size<<TPC_SAMPLE_CTRL0_SEQ_NUM_OFFSET) );

    TPC_REG_SET(TPC_OUT_SEQ0, 0x00);  
	TPC_REG_SET(TPC_OUT_SEQ1, 0x00);
    TPC_REG_SET(TPC_OE_SEQ0, 0x00); 
	TPC_REG_SET(TPC_OE_SEQ1, 0x00); 
    TPC_REG_SET(TPC_CH_SEQ, 0x00);  
    TPC_REG_SET(TPC_RES_SEQ, 0x00);      
    TPC_REG_SET(TPC_SWT_SEQ, 0x00);  

    for(index=0; index<seq_size; index++)
    {
        // OUT_PAT
        if(index < 4)
        {
            temp = TPC_REG_GET(TPC_OUT_SEQ0); 
			if(seq_ptr->out_yu == 1)    temp |= (1<< ((index<<2)+3));
			if(seq_ptr->out_yd == 1)    temp |= (1<< ((index<<2)+2));
			if(seq_ptr->out_xl == 1)    temp |= (1<< ((index<<2)+1));
			if(seq_ptr->out_xr == 1)    temp |= (1<< ((index<<2)+0));
			
			TPC_REG_SET(TPC_OUT_SEQ0, temp);
		}
		else
		{
            temp = TPC_REG_GET(TPC_OUT_SEQ1); 
			if(seq_ptr->out_yu == 1)    temp |= (1<< (((index-4)<<2)+3));
			if(seq_ptr->out_yd == 1)    temp |= (1<< (((index-4)<<2)+2));
			if(seq_ptr->out_xl == 1)    temp |= (1<< (((index-4)<<2)+1));
			if(seq_ptr->out_xr == 1)    temp |= (1<< (((index-4)<<2)+0));
			
			TPC_REG_SET(TPC_OUT_SEQ1, temp);
		}

        // OE_PAT
        if(index < 4)
        {
            temp = TPC_REG_GET(TPC_OE_SEQ0); 
			if(seq_ptr->oe_yu == 1)    temp |= (1<< ((index<<2)+3));
			if(seq_ptr->oe_yd == 1)    temp |= (1<< ((index<<2)+2));
			if(seq_ptr->oe_xl == 1)    temp |= (1<< ((index<<2)+1));
			if(seq_ptr->oe_xr == 1)    temp |= (1<< ((index<<2)+0));
			
			TPC_REG_SET(TPC_OE_SEQ0, temp);
		}
		else
		{
            temp = TPC_REG_GET(TPC_OE_SEQ1); 
			if(seq_ptr->oe_yu == 1)    temp |= (1<< (((index-4)<<2)+3));
			if(seq_ptr->oe_yd == 1)    temp |= (1<< (((index-4)<<2)+2));
			if(seq_ptr->oe_xl == 1)    temp |= (1<< (((index-4)<<2)+1));
			if(seq_ptr->oe_xr == 1)    temp |= (1<< (((index-4)<<2)+0));
			
			TPC_REG_SET(TPC_OE_SEQ1, temp);
		}

        // CH_PAT
        {
            temp = TPC_REG_GET(TPC_CH_SEQ); 
			if(seq_ptr->ch_yu == 1)    temp |= (ANA_INPUT_YU<< (index<<1));
			if(seq_ptr->ch_yd == 1)    temp |= (ANA_INPUT_YD<< (index<<1));
			if(seq_ptr->ch_xl == 1)    temp |= (ANA_INPUT_XL<< (index<<1));
			if(seq_ptr->ch_xr == 1)    temp |= (ANA_INPUT_XR<< (index<<1));
			
			TPC_REG_SET(TPC_CH_SEQ, temp);
		}

        // RES_PAT
        {
            temp = TPC_REG_GET(TPC_RES_SEQ); 
			if(seq_ptr->res_x== 1)    temp |= (1<< ((index<<1)+0));
			if(seq_ptr->res_y== 1)    temp |= (1<< ((index<<1)+1));
			
			TPC_REG_SET(TPC_RES_SEQ, temp);
		}

        // SWT_PAT
        {
            temp = TPC_REG_GET(TPC_SWT_SEQ); 
			if(seq_ptr->swt_on== 1)    temp |= (1<< (index));
			
			TPC_REG_SET(TPC_SWT_SEQ, temp);
		}

        // Init s_tpc_seq_index   
        s_tpc_seq_index[index].index =index;    
		s_tpc_seq_index[index].seq_type = seq_ptr->seq_type;
		
		seq_ptr++;
	}

}

/******************************************************************************/
//  Description:  Get the sequence number of Y/X data you specified.
//  Author:         
//  Note:  it starts from 1,
//         SCI_FALSE is invalid.
/******************************************************************************/
LOCAL uint32 _seq_get_index(TPC_CMD_SEQ_PTR seq_ptr, uint32 seq_size, TPC_SEQ_TYPE_E seq_type)
{
    uint32 index=0;
	uint32 result=SCI_FALSE;

    for(index=0; index<seq_size; index++)
    {
        if(seq_ptr[index].seq_type == seq_type)  break;
    }

    if(index < seq_size)
    {
        result = index+1;
    }
    return result;
}

//CALIBRATED_DATA_T g_calibrated_data = {332.0, 1070.00, 235.00, 900.00};//For MetalFix sc6530_HX8352c 
CALIBRATED_DATA_T g_calibrated_data = {433.0, 748.00, 235.00, 900.00};//For MetalFix NewTP 
//LOCAL CALIBRATED_DATA_T g_calibrated_data = {0};    
LOCAL CALIBRATED_DATA_T s_sum_calibratedData = {0,0,0, 900};
LOCAL uint32            s_sum_DataCnt = 0;

/******************************************************************************/
//  Description:  Set calibration data in 3-points calibration.
//  Author:         
//  Note:  Rt0 is not correct in calibration, so you must set it before.
//         Also you must not change it in other process.
/******************************************************************************/
PUBLIC void TPDRV_SetCalibratedData(CALIBRATED_DATA_T data)
{
    char buf[128]={0};
    sprintf(buf,"TPDRV: _SetCalibratedData: Rx0=%f,Ry0=%f,Rt0=%f",data.Rx0,data.Ry0,data.Rt0);
    TPDRV_LOG_TRACE(("TPDRV: _SetCalibratedData: %s", buf));
    
    g_calibrated_data.Rx0 = data.Rx0;
    g_calibrated_data.Ry0 = data.Ry0;
    //g_calibrated_data.Rt0 = data.Rt0;   
}

/******************************************************************************/
//  Description:  Save calibration data in 3-points calibration.
//  Author:         
//  Note:  
/******************************************************************************/
PUBLIC void TPDRV_SaveCalibratedData(CALIBRATED_DATA_T data)
{
    char buf[128]={0};
    sprintf(buf,"TPDRV:_SaveCalibratedData: Rx0=%f,Ry0=%f,Rt0=%f s_sum_DataCnt=%d",\
                                                      data.Rx0,data.Ry0,data.Rt0,\
                                                      s_sum_DataCnt);
    TPDRV_LOG_TRACE (("TPDRV:_SaveCalibratedData: %s", buf));

    if(s_sum_DataCnt == 0)
    {
        s_sum_calibratedData.Rx0 = data.Rx0;
        s_sum_calibratedData.Ry0 = data.Ry0;
        //s_sum_calibratedData.Rt0 = data.Rt0;
        s_sum_DataCnt++;
    }
    else
    {
        s_sum_calibratedData.Rx0 += data.Rx0;
        s_sum_calibratedData.Ry0 += data.Ry0;
        //s_sum_calibratedData.Rt0 += data.Rt0;
        s_sum_DataCnt++;        
    }  

    g_calibrated_data.Rx0 = s_sum_calibratedData.Rx0/s_sum_DataCnt;
    g_calibrated_data.Ry0 = s_sum_calibratedData.Ry0/s_sum_DataCnt;
    //g_calibrated_data.Rt0 = s_sum_calibratedData.Rt0/s_sum_DataCnt;    
}

LOCAL void _ClearCalibratedData( void )
{
    TPDRV_LOG_TRACE (("TPDRV:========_ClearCalibratedData"));
    //SCI_MEMSET(&g_calibrated_data, 0, sizeof(g_calibrated_data));
    SCI_MEMSET(&s_sum_calibratedData,0, sizeof(s_sum_calibratedData));
    s_sum_DataCnt = 0;
}

/******************************************************************************
 * _Run
 ******************************************************************************/
LOCAL void _Run (void)
{
    //Disable APB sleep
    SCI_TPC_EnableDeepSleep (DISABLE_APB_SLEEP);

    //RUN TPC module
    TPC_REG_OR(TPC_CTRL, TPC_CTRL_RUN);

    CLEAR_TPC_INT(TPC_DONE_IRQ_MSK);

    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_Run ]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_650_112_2_17_23_7_29_1393,(uint8*)"");
}

/******************************************************************************
 * _Stop
 ******************************************************************************/
LOCAL void _Stop (void)
{
    //Enable APB sleep
    SCI_TPC_EnableDeepSleep (ENABLE_APB_SLEEP);

    //Disable TPC module
    TPC_REG_OR (TPC_CTRL, TPC_CTRL_STOP);
    //Close ADC channel
    //ADC_CloseTPC();

    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_Stop]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_665_112_2_17_23_7_29_1394,(uint8*)"");
}

/******************************************************************************
 * _FetchYxData
 ******************************************************************************/
LOCAL uint32 _FetchYxData (TPC_DATA_U *data0, uint32 size)
{
    uint8 i;
    BOOLEAN result = SCI_TRUE;
    TPC_DATA_U cur_data, pre_data;

    pre_data.dwValue = 0;
    s_tpc_seq_data[s_tpc_seq_index[0].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[1].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[2].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[3].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[4].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[5].seq_type].data =0;	
    s_tpc_seq_data[s_tpc_seq_index[6].seq_type].data =0;	
    s_tpc_seq_data[s_tpc_seq_index[7].seq_type].data =0;
 
    //Fecth data from TPC data buffer
    _seq_set_addr(0);  //for y_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        //Get data from buffer
        s_tpc_seq_data[(uint32)(s_tpc_seq_index[0].seq_type)].data  = _seq_get_data();
    }
 
    _seq_set_addr(1);  //for x_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        //Get data from buffer
        s_tpc_seq_data[s_tpc_seq_index[1].seq_type].data  = _seq_get_data();
    }

    _seq_set_addr(2);  //for x_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[2].seq_type].data = _seq_get_data();
    }
        
    _seq_set_addr(3);  //for x1_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[3].seq_type].data = _seq_get_data();
    }

    _seq_set_addr(4);  //for Ry_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[4].seq_type].data = _seq_get_data();
    }

    cur_data.data.data_y = (uint16)(s_tpc_seq_data[TPC_SEQ_Y].data);
    cur_data.data.data_x = (uint16)(s_tpc_seq_data[TPC_SEQ_X].data);

    TPDRV_LOG_TRACE("TPDRV:_FetchYxData x=%d y=%d", cur_data.data.data_x,cur_data.data.data_y);
    TPDRV_LOG_TRACE("[TPDRV:_FetchYxData] x_index=%d y_index=%d",\
                                              s_seq_x_index,s_seq_y_index);
    //Get the last data
    data0->dwValue = cur_data.dwValue;

    // clear the data buffer
    TPC_REG_OR(TPC_BUF_CTRL, TPC_BUF_CTRL_CLR);
    
    return result;

}


/******************************************************************************
 * TPDRV_FetchMultiData
 ******************************************************************************/
//7_step:Y->Y1->X->X1->Ry->Rx->Rt
LOCAL uint32 _FetchMultiData (TPC_DATA_U *data0, uint32 size)
{
    uint8 i;
    uint32 result = 0;

    FLOAT_TYPE x1_pixels=0;
    FLOAT_TYPE y1_pixels=0;    

    s_tpc_seq_data[s_tpc_seq_index[0].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[1].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[2].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[3].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[4].seq_type].data =0;
    s_tpc_seq_data[s_tpc_seq_index[5].seq_type].data =0;	
    s_tpc_seq_data[s_tpc_seq_index[6].seq_type].data =0;	
    s_tpc_seq_data[s_tpc_seq_index[7].seq_type].data =0;
	
    //TPDRV_LOG_TRACE:"TPDRV:_FetchMultiData:"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_692_112_2_17_23_7_29_1395,(uint8*)"" );
 
    //Fecth data from TPC data buffer
    _seq_set_addr(0);  //for y_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        //Get data from buffer
        s_tpc_seq_data[(uint32)(s_tpc_seq_index[0].seq_type)].data  = _seq_get_data();
    }
 
    _seq_set_addr(1);  //for y1_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        //Get data from buffer
        s_tpc_seq_data[s_tpc_seq_index[1].seq_type].data  = _seq_get_data();
    }

    _seq_set_addr(2);  //for x_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[2].seq_type].data = _seq_get_data();
    }
        
    _seq_set_addr(3);  //for x1_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[3].seq_type].data = _seq_get_data();
    }

    _seq_set_addr(4);  //for Ry_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[4].seq_type].data = _seq_get_data();
    }

    _seq_set_addr(5);  //for Rx_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[5].seq_type].data = _seq_get_data();	
    }

		
    _seq_set_addr(6);  
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[6].seq_type].data = _seq_get_data();
    }

    //TPDRV_LOG_TRACE:"rnTPDRV:Original_bef: Y=%df Y1=%df X=%df X1=%df Ry=%df Rx=%df RT=%df"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_748_112_2_17_23_7_29_1396,(uint8*)"fffffff",(uint32)(s_tpc_seq_data[TPC_SEQ_Y].data*10000),(uint32)(s_tpc_seq_data[TPC_SEQ_Y1].data*10000),(uint32)(s_tpc_seq_data[TPC_SEQ_X].data*10000),(uint32)(s_tpc_seq_data[TPC_SEQ_X1].data*10000),(uint32)(s_tpc_seq_data[TPC_SEQ_RY].data*10000),(uint32)(s_tpc_seq_data[TPC_SEQ_RX].data*10000),(uint32)(s_tpc_seq_data[TPC_SEQ_RT].data*10000) );
	
    x1_pixels = s_tpc_seq_data[TPC_SEQ_X1].data*(s_coordinate_coef.x_slop)*ADC_TPC_SCALE/256+s_coordinate_coef.x_offset;
    y1_pixels = s_tpc_seq_data[TPC_SEQ_Y1].data*(s_coordinate_coef.y_slop)*ADC_TPC_SCALE/256+s_coordinate_coef.y_offset;   

    s_tpc_seq_data[TPC_SEQ_X].data = pixel2len(s_tpc_seq_data[TPC_SEQ_X].data, (FLOAT_TYPE)LCD_WIDTH,TP_WIDTH);
    s_tpc_seq_data[TPC_SEQ_Y].data = pixel2len(s_tpc_seq_data[TPC_SEQ_Y].data, (FLOAT_TYPE)LCD_HEIGHT,TP_HEIGHT); 
    s_tpc_seq_data[TPC_SEQ_X1].data= pixel2len(x1_pixels, (FLOAT_TYPE)LCD_WIDTH,TP_WIDTH);
    s_tpc_seq_data[TPC_SEQ_Y1].data= pixel2len(y1_pixels, (FLOAT_TYPE)LCD_HEIGHT,TP_HEIGHT);     
    s_tpc_seq_data[TPC_SEQ_RX].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RX].data, TP_REF_U, TP_REF_R);
    s_tpc_seq_data[TPC_SEQ_RY].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RY].data, TP_REF_U, TP_REF_R);

    s_tpc_seq_data[TPC_SEQ_RT].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RT].data, TP_REF_U, TP_REF_R);
    s_tpc_seq_data[TPC_SEQ_RT_X].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RT_X].data, TP_REF_U, TP_REF_R);
    s_tpc_seq_data[TPC_SEQ_RT_Y].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RT_Y].data, TP_REF_U, TP_REF_R);

    //TPDRV_LOG_TRACE:"TPDRV:Original_ohm:x1_pixels=%f y1_pixels=%f RT_Y=%f RT_X=%f RT=%f"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_768_112_2_17_23_7_29_1397,(uint8*)"fffff",x1_pixels, y1_pixels,             s_tpc_seq_data[TPC_SEQ_RT_Y].data,s_tpc_seq_data[TPC_SEQ_RT_X].data,s_tpc_seq_data[TPC_SEQ_RT].data );
  
    //TPDRV_LOG_TRACE:"TPDRV:Original_aft: Y=%f Y1=%f X=%f X1=%f Ry=%f Rx=%f RT=%f"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_777_112_2_17_23_7_29_1398,(uint8*)"fffffff",s_tpc_seq_data[TPC_SEQ_Y].data,s_tpc_seq_data[TPC_SEQ_Y1].data,s_tpc_seq_data[TPC_SEQ_X].data,s_tpc_seq_data[TPC_SEQ_X1].data,s_tpc_seq_data[TPC_SEQ_RY].data,s_tpc_seq_data[TPC_SEQ_RX].data,s_tpc_seq_data[TPC_SEQ_RT].data );
        
    result = tpc_algorithm(&(s_tpc_seq_data[0]), data0, size);

//OUT:    // clear the data buffer
    TPC_REG_OR(TPC_BUF_CTRL, TPC_BUF_CTRL_CLR);
    
    return result;
}


//X->Y->Rx0->Ry0->Rt->Rt->delta_Rx->delta_Ry
LOCAL uint32 _FetchCalibrateData (TPC_DATA_U *data0, uint32 size)
{
    uint8 i;
    BOOLEAN result = 0;
    FLOAT_TYPE Rtouch=0;
    char log_buffer[128];
    CALIBRATED_DATA_T calibrated_data={0};

    //TPDRV_LOG_TRACE:"TPDRV:_FetchCalibrateData:\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_803_112_2_17_23_7_29_1399,(uint8*)"" );

    //Fecth data from TPC data buffer
    _seq_set_addr(0);  
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[0].seq_type].data  = _seq_get_data();
    }
    
    _seq_set_addr(1);  //for x_data
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[1].seq_type].data  = _seq_get_data();
    }
    
    _seq_set_addr(2);  //Rx0
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[2].seq_type].data  = _seq_get_data();
    }
        
    _seq_set_addr(3);  //Ry0
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[3].seq_type].data  = _seq_get_data();
    }

    _seq_set_addr(4);  
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[4].seq_type].data  = _seq_get_data();
    }
    _seq_set_addr(5);  
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[5].seq_type].data  = _seq_get_data();
    }
    
    _seq_set_addr(6);  
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[6].seq_type].data  = _seq_get_data();
    }
    _seq_set_addr(7);  
    for (i=0; i<TPC_BUF_LENGTH; i++)
    {
        s_tpc_seq_data[s_tpc_seq_index[7].seq_type].data  = _seq_get_data();        
    }
	
    sprintf(log_buffer, "TPDRV:Adc_ x=%f y=%f Rx0=%f Ry0=%f delta_Rx=%f delta_Ry=%f",\
                                          s_tpc_seq_data[TPC_SEQ_X].data,\
                                          s_tpc_seq_data[TPC_SEQ_Y].data, \
                                          s_tpc_seq_data[TPC_SEQ_RX0].data,\
                                          s_tpc_seq_data[TPC_SEQ_RY0].data,\
                                          s_tpc_seq_data[TPC_SEQ_RX].data,\
                                          s_tpc_seq_data[TPC_SEQ_RY].data ); 
    //TPDRV_LOG_TRACE:"%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_859_112_2_17_23_7_29_1400,(uint8*)"s", log_buffer);
    
	if(SCI_FALSE== s_coordinate_coef.is_coordinated)
	{// Get adc raw data
		data0[0].data.data_x = (uint16)(s_tpc_seq_data[TPC_SEQ_X].data);
		data0[0].data.data_y = (uint16)(s_tpc_seq_data[TPC_SEQ_Y].data);
	    //TPDRV_LOG_TRACE:"TPDRV: adc raw data(%d,%d)\r\n"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_865_112_2_17_23_7_29_1401,(uint8*)"dd", data0[0].data.data_x,data0[0].data.data_y );
		result = 1;
		goto OUT;
	}
	else
	{
	    s_tpc_seq_data[TPC_SEQ_X].data = pixel2len(s_tpc_seq_data[TPC_SEQ_X].data, (FLOAT_TYPE)LCD_WIDTH,TP_WIDTH);
        s_tpc_seq_data[TPC_SEQ_Y].data = pixel2len(s_tpc_seq_data[TPC_SEQ_Y].data, (FLOAT_TYPE)LCD_HEIGHT,TP_HEIGHT); 
		result = 1;
	}

{	
    s_tpc_seq_data[TPC_SEQ_RX0].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RX0].data, TP_REF_U, TP_REF_R);
    s_tpc_seq_data[TPC_SEQ_RY0].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RY0].data, TP_REF_U, TP_REF_R);
    s_tpc_seq_data[TPC_SEQ_RT_X].data= adc2ohm(s_tpc_seq_data[TPC_SEQ_RT_X].data, TP_REF_U, TP_REF_R);
    s_tpc_seq_data[TPC_SEQ_RT_Y].data= adc2ohm(s_tpc_seq_data[TPC_SEQ_RT_Y].data, TP_REF_U, TP_REF_R);    
    s_tpc_seq_data[TPC_SEQ_RX].data= adc2ohm(s_tpc_seq_data[TPC_SEQ_RX].data, TP_REF_U, TP_REF_R);
    s_tpc_seq_data[TPC_SEQ_RY].data= adc2ohm(s_tpc_seq_data[TPC_SEQ_RY].data, TP_REF_U, TP_REF_R);    
}
	
    Rtouch =TPC_GetRtouch(&(s_tpc_seq_data[0]));         
    
    sprintf(log_buffer, "TPDRV: x=%f y=%f Rx0=%f Ry0=%f delta_Rx=%f delta_Ry=%f Rtouch=%f",\
                                          s_tpc_seq_data[TPC_SEQ_X].data,\
                                          s_tpc_seq_data[TPC_SEQ_Y].data, \
                                          s_tpc_seq_data[TPC_SEQ_RX0].data,\
                                          s_tpc_seq_data[TPC_SEQ_RY0].data,\
                                          s_tpc_seq_data[TPC_SEQ_RX].data,\
                                          s_tpc_seq_data[TPC_SEQ_RY].data,\
                                          Rtouch );  
    TPDRV_LOG_TRACE (("TPDRV:%s", log_buffer));

OUT:
	if(SCI_FALSE== s_coordinate_coef.is_coordinated)
	{
        s_tpc_seq_data[TPC_SEQ_RX0].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RX0].data, TP_REF_U, TP_REF_R);
        s_tpc_seq_data[TPC_SEQ_RY0].data = adc2ohm(s_tpc_seq_data[TPC_SEQ_RY0].data, TP_REF_U, TP_REF_R);
        
        calibrated_data.Rx0 = s_tpc_seq_data[TPC_SEQ_RX0].data;
        calibrated_data.Ry0 = s_tpc_seq_data[TPC_SEQ_RY0].data;
        calibrated_data.Rt0 = Rtouch;
        TPDRV_SaveCalibratedData( calibrated_data);
	}
    
    // clear the data buffer
    TPC_REG_OR(TPC_BUF_CTRL, TPC_BUF_CTRL_CLR);
    
    return result;
}

LOCAL uint32 _FetchData (TPC_DATA_U *data0, uint32 size)
{
#ifdef TPC_TYPE_DOUBLE
	if(s_tpc_mode == TPC_MULTITOUCH_MODE) 
	{
        return (_FetchMultiData (data0, size));
	}

	if(s_tpc_mode == TPC_CALIBRATION_MODE) 
	{
        return (_FetchCalibrateData (data0, size));
	}
#else //TPC_TYPE_SINGLE
    return _FetchYxData(data0, size);
#endif
}

/******************************************************************************
 * TPDRV_GetCalibratedData
 ******************************************************************************/
PUBLIC CALIBRATED_DATA_T* TPDRV_GetCalibratedData( void )
{
    char buf[128]={0};
    sprintf(buf,"TPDRV:_GetCalibratedData: Rx0=%f,Ry0=%f,Rt0=%f",g_calibrated_data.Rx0,\
                                                                 g_calibrated_data.Ry0,\
                                                                 g_calibrated_data.Rt0);
    TPDRV_LOG_TRACE (("TPDRV:_GetCalibratedData: %s", buf));
    
    return (&g_calibrated_data);
}

/******************************************************************************
 * TPDRV_TestModeEn(uint32 is_en)
 ******************************************************************************/
PUBLIC void TPDRV_TestModeEn(uint32 is_en)
{
    if(is_en)
    {
		CHIP_REG_OR (TPC_CTRL, BIT_4);
    }
	else
	{
		CHIP_REG_AND (TPC_CTRL, ~BIT_4);
	}
}

/******************************************************************************
 * TPC_Open
 ******************************************************************************/
PUBLIC void TPDRV_Open (void)
{
    /* Enable TPC device */	
    SCI_ASSERT (DEVICE_Find ("TPC", &s_dev_tpc) == CLOCK_SUCCESS); /*assert verified*/    
    DEVICE_Open(s_dev_tpc);

    DEVICE_Reset(s_dev_tpc);
    
    //Enable TPC module
    TPC_REG_OR(TPC_CTRL, TPC_CTRL_EN);
}

/******************************************************************************
 * TPDRV_IntEnable
 ******************************************************************************/
PUBLIC void TPDRV_IntEnable (void)
{
    TPC_REG_OR (TPC_INT_EN, (TPC_DONE_IRQ_MSK | TPC_UP_IRQ_MSK |TPC_DOWN_IRQ_MSK));

    // enable the bit in a-die int controller
    TPC_REG_OR(ANA_INT_EN, ANA_TPC_IRQ);
}

/******************************************************************************
 * TPDRV_IntDisable
 ******************************************************************************/
PUBLIC void TPDRV_IntDisable (void)
{
    TPC_REG_AND (TPC_INT_EN, ~ (TPC_DONE_IRQ_MSK | TPC_UP_IRQ_MSK |TPC_DOWN_IRQ_MSK));

    // disable the bit in a-die int controller
    TPC_REG_AND(ANA_INT_EN, ~ANA_TPC_IRQ);
}

PUBLIC void TPDRV_Enable (void)
{
	TPDRV_IntEnable();
    SCI_TPC_EnableDeepSleep(DISABLE_APB_SLEEP);
}

PUBLIC void TPDRV_Disable (void)
{
	TPDRV_IntDisable();
    SCI_TPC_EnableDeepSleep(ENABLE_APB_SLEEP);
}
	
/******************************************************************************
 * TPDRV_Init
 ******************************************************************************/
PUBLIC void TPDRV_Init (void)
{

	TP_CFG_INFO_PTR tp_cfg_ptr = TP_GetCfgInfo();
	s_tp_dp_cfg_ptr = tp_cfg_ptr->dpoint_cfg_ptr;	

    TP_WIDTH = (FLOAT_TYPE)s_tp_dp_cfg_ptr->tp_width;
    TP_HEIGHT= (FLOAT_TYPE)s_tp_dp_cfg_ptr->tp_height;
    TP_REF_U = (FLOAT_TYPE)s_tp_dp_cfg_ptr->ref_v;
    TP_REF_R = (FLOAT_TYPE)s_tp_dp_cfg_ptr->ref_r;	
	
    //Enable TPC module
    TPDRV_Open();
    //Config pen request polarity
    TPC_REG_AND (TPC_CTRL, ~TPC_CTRL_PEN_REQ_POL);

    //TPC sample properity is equal to 1/2ms
    TPC_REG_MSK_OR (TPC_CTRL, (((s_tp_dp_cfg_ptr->data_period)*2)<< TPC_CTRL_PRESCALE_OFFSET), TPC_CTRL_PRESCALE_MSK);
	
    //Enable TPC interrupt
    //ANA_REG_OR(TPC_INT_EN, (TPC_DONE_IRQ_MSK_BIT | TPC_UP_IRQ_MSK_BIT |TPC_DOWN_IRQ_MSK_BIT));

    //Config ADC channel
    // * 0--little scale,1.2V * 1--big scale,   3.0v 
    TPC_SetScale ( ADC_SCALE_3V );

    //Config TPC sample properity
	//Without pclk prescale, the data below is regard as 0.5ms period sending data to server.
	//Example for 7step: TotalTime=(0.052*7+0.2)*2*10=11ms
    TPC_REG_SET (TPC_SAMPLE_CTRL1,
				 (0x32 <<TPC_SAMPLE_CTRL1_POINT_INTERVAL_OFFSET) |
				 (0x14 << TPC_SAMPLE_CTRL1_SAMPLE_INTERVAL_OFFSET) ); //52us for each data
    TPC_REG_SET (TPC_SAMPLE_CTRL2,
                 (0x520 << TPC_SAMPLE_CTRL2_DATA_INTERVAL_OFFSET) );  //0.2ms
    TPC_REG_OR (TPC_SAMPLE_CTRL0,
                 (0x5 << TPC_SAMPLE_CTRL0_SAMPLE_NUM_OFFSET) );       

    //Config TPC debounce properity
    TPC_REG_SET (TPC_BOUNCE_CTRL,
                 TPC_DEBOUNCE_EN_BIT | (20 << TPC_DEBOUNCE_NUM_OFFSET));

#ifdef TPC_TYPE_DOUBLE
	if(s_tpc_mode == TPC_MULTITOUCH_MODE)
	{    //Config tpc mode 
        _seq_validate(s_tp_SeqMultitouch, ARRAY_SIZE(s_tp_SeqMultitouch));
        s_seq_x_index=_seq_get_index(s_tp_SeqMultitouch, ARRAY_SIZE(s_tp_SeqMultitouch), TPC_SEQ_X);
        s_seq_y_index=_seq_get_index(s_tp_SeqMultitouch, ARRAY_SIZE(s_tp_SeqMultitouch), TPC_SEQ_Y);
	}
	if(s_tpc_mode == TPC_CALIBRATION_MODE)
	{
	    _seq_validate(s_tp_SeqCalibrate, ARRAY_SIZE(s_tp_SeqCalibrate));
	}
#else //TPC_TYPE_SINGLE
    _seq_validate(s_tp_SeqYx, ARRAY_SIZE(s_tp_SeqYx));
    s_seq_x_index=_seq_get_index(s_tp_SeqYx, ARRAY_SIZE(s_tp_SeqYx), TPC_SEQ_X);
    s_seq_y_index=_seq_get_index(s_tp_SeqYx, ARRAY_SIZE(s_tp_SeqYx), TPC_SEQ_Y);
#endif
	
    //Config TPC buffer length
    TPC_REG_MSK_OR (TPC_BUF_CTRL, (TPC_BUF_LENGTH << TPC_BUF_LENGTH_OFFSET), TPC_BUF_LENGTH_MSK);
    //Clear TPC interrupt
    CLEAR_TPC_INT ( (TPC_DONE_IRQ_MSK | TPC_UP_IRQ_MSK |TPC_DOWN_IRQ_MSK));

    //Open ADC channel
    ADC_OpenTPC();

    //Register TPC ISR handler
    ISR_RegHandler_Ex (TB_TPC_INT, (TB_ISR) _IrqHandler, _TPCHisrFunc, CHIPDRV_HISR_PRIO_2, NULL);

    //Disable tpc interrupt
    TPDRV_IntDisable();

    //TPDRV_LOG_TRACE:"[TPDRV: TPDRV_Init]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_1062_112_2_17_23_7_30_1404,(uint8*)"");
}

/******************************************************************************
 * TPC_EnableCoord
 ******************************************************************************/
PUBLIC void TPDRV_EnableCoord (void)
{
#ifdef CHIP_VER_6530
    // Start from 1
    TPC_REG_OR (TPC_CALC_CTRL, (s_seq_y_index<<TPC_CALC_SEQY_OFFSET));
    TPC_REG_OR (TPC_CALC_CTRL, (s_seq_x_index<<TPC_CALC_SEQX_OFFSET));
#else
    TPC_REG_OR (TPC_CALC_SEQ, (1<<TPC_CALC_SEQY0_OFFSET));
    TPC_REG_OR (TPC_CALC_SEQ, (2<<TPC_CALC_SEQX0_OFFSET));
    TPC_REG_OR (TPC_CALC_SEQ, (3<<TPC_CALC_SEQY1_OFFSET));    
    TPC_REG_OR (TPC_CALC_SEQ, (4<<TPC_CALC_SEQX1_OFFSET));
#endif

    TPC_REG_OR (TPC_CALC_CTRL, TPC_CALC_CTRL_EN);
    s_coordinate_coef.is_coordinated= SCI_TRUE;	
	
    TPDRV_LOG_TRACE("[TPDRV:TPDRV_EnableCoord] x_index=%d y_index=%d",\
                                              s_seq_x_index,s_seq_y_index);
}

/******************************************************************************
 * TPC_DisableCoord
 ******************************************************************************/
PUBLIC void TPDRV_DisableCoord (void)
{
    TPC_REG_AND (TPC_CALC_CTRL, ~TPC_CALC_CTRL_EN);
    s_coordinate_coef.is_coordinated= SCI_FALSE;	
	
    //TPDRV_LOG_TRACE:"[TPDRV:TPDRV_DisableCoord]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_1087_112_2_17_23_7_30_1406,(uint8*)"");
}

/******************************************************************************
 * TPC_ConfigCoord
 ******************************************************************************/
PUBLIC void TPDRV_ConfigCoord (
    int x_slop,
    int x_offset,
    int y_slop,
    int y_offset,
    BOOLEAN xy_exchange)
{
    //Config x coordinate coef
    TPC_REG_MSK_OR (TPC_CALC_X_COEF_A, x_slop, TPC_X_COEF_A_MSK);
    TPC_REG_MSK_OR (TPC_CALC_X_COEF_B, x_offset, TPC_X_COEF_B_MSK);
    //Config y coordinate coef
    TPC_REG_MSK_OR (TPC_CALC_Y_COEF_A, y_slop, TPC_Y_COEF_A_MSK);
    TPC_REG_MSK_OR (TPC_CALC_Y_COEF_B, y_offset, TPC_Y_COEF_B_MSK);

    s_coordinate_coef.x_slop   = x_slop;
    s_coordinate_coef.x_offset = x_offset;	
    s_coordinate_coef.y_slop   = y_slop;
    s_coordinate_coef.y_offset = y_offset;
}

/******************************************************************************
 * TP_EnterCalibrate
 ******************************************************************************/
PUBLIC void TPDRV_EnterCalibration ( void )
{
    s_tpc_mode = TPC_CALIBRATION_MODE;

#ifdef TPC_TYPE_DOUBLE
	_seq_validate(s_tp_SeqCalibrate, ARRAY_SIZE(s_tp_SeqCalibrate));	
#else
	//_seq_validate(s_tp_SeqYx, ARRAY_SIZE(s_tp_SeqYx));	
#endif

    //Disable coordinate convertion
    TPDRV_DisableCoord();    
    _ClearCalibratedData();
    
    TPDRV_LOG_TRACE ( ("[TPDRV:TPDRV_EnterCalibration]"));
}

/******************************************************************************
 * TP_EnterMulti
 ******************************************************************************/
PUBLIC void TPDRV_EnterMulti ( void )
{
    s_tpc_mode = TPC_MULTITOUCH_MODE;
	
#ifdef TPC_TYPE_DOUBLE
    _seq_validate(s_tp_SeqMultitouch, ARRAY_SIZE(s_tp_SeqMultitouch));	
    s_seq_x_index=_seq_get_index(s_tp_SeqMultitouch, ARRAY_SIZE(s_tp_SeqMultitouch), TPC_SEQ_X);
    s_seq_y_index=_seq_get_index(s_tp_SeqMultitouch, ARRAY_SIZE(s_tp_SeqMultitouch), TPC_SEQ_Y);
#else
	//_seq_validate(s_tp_SeqYx, ARRAY_SIZE(s_tp_SeqYx));
#endif

    //Enable coordinate convertion
    TPDRV_EnableCoord();
    TPDRV_LOG_TRACE ( ("[TPDRV:TPDRV_EnterMulti]"));
}

/******************************************************************************
 * TPC_IrqHandler
 ******************************************************************************/
LOCAL ISR_EXE_T _IrqHandler (void)
{
    uint16 int_status;

    int_status = TPC_REG_GET (TPC_INT_STS);

    //Pen Down interrtup
    if (int_status&TPC_DOWN_IRQ_MSK)
    {
        // clear the data buffer
        TPC_REG_OR(TPC_BUF_CTRL, TPC_BUF_CTRL_CLR);
        
        IsrWriteBuffer (TPC_INT_DOWN);
        //Clear down interrupt*/
        CLEAR_TPC_INT (TPC_DOWN_IRQ_MSK);
    }
    //Pen Up interrupt
    else if (int_status&TPC_UP_IRQ_MSK)
    {
        IsrWriteBuffer (TPC_INT_UP);
        //disable interrupt
        TPC_REG_AND (TPC_INT_EN, ~TPC_DONE_IRQ_MSK);
        //Clear up interrupt*/
        CLEAR_TPC_INT ( (TPC_UP_IRQ_MSK|TPC_DONE_IRQ_MSK));
    }
    //Sample Done interrupt
    else if (int_status&TPC_DONE_IRQ_MSK)
    {
        IsrWriteBuffer (TPC_INT_DONE);
        //Clear done interrupt*/
        CLEAR_TPC_INT (TPC_DONE_IRQ_MSK);
    }

    //Error occured
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     ("[TPDRV:TPDRV_IrqHandler] Undefined irq"));
    }

    return CALL_HISR;
}

/******************************************************************************
 * _TPCHisrFunc
 ******************************************************************************/
LOCAL void _TPCHisrFunc (uint32 i, void *d)
{
    int32  int_type = 0;

    while (!threadReadBuffer (&int_type))
    {
        _HandleTPCHisr (int_type);
    }
}

/******************************************************************************
 * _HandleTPCHisr
 ******************************************************************************/
uint32  data_x0 = 0;
uint32  data_y0 = 0;
uint32  data_x1 = 0;
uint32  data_y1 = 0;

uint32           points_num=0;
TPC_DATA_U tpc_data[2]={0};
TB_MSG        msg_ptr ={0};
LOCAL void _HandleTPCHisr (int int_type)
{

    //Pen Down interrtup
    if (int_type == TPC_INT_DOWN)
    {
        is_first_down = SCI_TRUE;
        //Run TPC
        _Run();
        //enable done interrupt
        TPC_REG_OR (TPC_INT_EN, TPC_DONE_IRQ_MSK);
        return ;
    }
    //Pen Up interrupt
    else if (int_type == TPC_INT_UP)
    {
        //Stop TPC
        _Stop();
        CLEAR_TPC_INT (TPC_DONE_IRQ_MSK);

        msg_ptr.wparam = 0;
        msg_ptr.message = TB_TP_UP;
    }
    //Sample Done interrupt
    else if (int_type == TPC_INT_DONE)
    {
        //Get data from TP buffer
        points_num =_FetchData (&(tpc_data[0]), 2);
        //TPDRV_LOG_TRACE:"[TPDRV:_FetchData] (%d,%d),(%d,%d),size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_1263_112_2_17_23_7_30_1414,(uint8*)"ddddd",tpc_data[0].data.data_x,tpc_data[0].data.data_y,tpc_data[1].data.data_x,tpc_data[1].data.data_y,points_num);
        //Filter TP_UP invalid point
        data_x0 = tpc_data[0].data.data_x;             
        data_y0 = tpc_data[0].data.data_y;   
        data_x1 = tpc_data[1].data.data_x;             
        data_y1 = tpc_data[1].data.data_y;   
        if (points_num==1)
        {         
            if((data_x0 == 1023) && (data_y0 == 0))
            {
                //TPDRV_LOG_TRACE:"[TPDRV:_FetchData] return--1"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_1273_112_2_17_23_7_30_1415,(uint8*)"" );
                return;                
            }
        }
        else if(points_num ==2)
        {
            if((data_x0 == 1023) && (data_y0 == 0))
            {
                //TPDRV_LOG_TRACE:"[TPDRV:_FetchData] return--2"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_1281_112_2_17_23_7_30_1416,(uint8*)"" );
                return;
            }
            
            if((data_x1 == 1023) && (data_y1 == 0))
            {
                //TPDRV_LOG_TRACE:"[TPDRV:_FetchData] return--3"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_1287_112_2_17_23_7_30_1417,(uint8*)"" );
                return; 
            }
        }
        else
        {
                //TPDRV_LOG_TRACE:"[TPDRV:_FetchData] return--4"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPC_PHY_V5_1293_112_2_17_23_7_30_1418,(uint8*)"" );
            return;
        }
        
        if (points_num)
        {
            msg_ptr.wparam = points_num;
            msg_ptr.lparam = &(tpc_data[0]);
			
            if (is_first_down == SCI_TRUE)
            {
                msg_ptr.message = TB_TP_DOWN;
                is_first_down = SCI_FALSE;
            }
            else
            {
                msg_ptr.message = TB_TP_MOVE;
            }
        }
        else
        {
            return ;
        }
    }
    //Error occured
    else
    {
        SCI_PASSERT (SCI_FALSE,  /*assert verified*/
                     ("[TPDRV:TPDRV_IrqHandler] Undefined irq"));
    }

    //Send message to TP service layer
    ISR_Callback (TB_TPC_INT,  &msg_ptr);
    return ;
}

/******************************************************************************/
//  Description:    Get Buf addr&size used for points
//  Author:         
//  Note:           *buf_addr: return buffer address
//                  *size    : return data len in unit of word
/******************************************************************************/
uint32 g_ParamBuf[12]={0};
PUBLIC uint32 TPDRV_GetPoints (uint32* buf_addr, uint32* size)
{
    uint32 data_size=0;
    *buf_addr = (uint32)(&(g_ParamBuf[0]));
    
    if(points_num == 2)
    {
        g_ParamBuf[0] =  (uint32)(tpc_data[0].data.data_x);     data_size++;
        g_ParamBuf[1] =  (uint32)(tpc_data[0].data.data_y);     data_size++;      
        g_ParamBuf[2] =  (uint32)(tpc_data[1].data.data_x);     data_size++; 
        g_ParamBuf[3] =  (uint32)(tpc_data[1].data.data_y);     data_size++;  
    }
    else
    {
        g_ParamBuf[0] =  (uint32)(tpc_data[0].data.data_x);     data_size++;
        g_ParamBuf[1] =  (uint32)(tpc_data[0].data.data_y);     data_size++; 
    }
    *size = data_size;

    TPDRV_LOG_TRACE ( "[TPDRV:_GetPoints] (%d,%d)size=%d,point_num=%d", g_ParamBuf[0],\
		                                                    g_ParamBuf[1],\
		                                                   *size, points_num);
    return SCI_TRUE;
}

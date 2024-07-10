/******************************************************************************
 ** File Name:    isp_drv.h                                               *
 ** Author:       Benny.zou                                                   *
 ** DATE:         6/25/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  SD6800 ISP service, more comments will be added later.                                                            *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 6/25/2006     Benny.zou       Create.                                     *
 ** 8/31/2006     patric.fang     change                                      *
 *****************************************************************************/

#ifndef _ISP_DRIVER_H_
#define _ISP_DRIVER_H_

#include "sci_types.h"
#include "os_api.h"
#include "ref_tasks_def.h"
#include "dc_cfg.h"

#define ISP_ERROR_XXX                   -1

#undef  CHIP_TEST

// the isp_state::mode field bit[3:0] define
#define ISP_MODE_IDLE                   0
#define ISP_MODE_PREVIEW                1
#define ISP_MODE_MPEG                   2
#define ISP_MODE_JPEG1                  3
#define ISP_MODE_JPEG2                  4
#define ISP_MODE_VP                     5
#define ISP_MODE_EXTERNAL0              6
#define ISP_MODE_EXTERNAL1              7
#define ISP_MODE_EXTERNAL2              8
#define ISP_MODE_EXTERNAL3              9
#define ISP_MODE_EXTERNAL4              10
#define ISP_MODE_MANUAL                 15

#define ISP_MODE_EXTERNAL               5

#define ISP_PORTAB_EXCHANGE             0x100   //porta portb path exchange.
#define ISP_YUV422_VIA_CCE              0x200   //input yuv422 force via cce module.

#define ISP_PORTA                       1UL
#define ISP_PORTB                       2UL
#define ISP_PORTC                       3UL
#define ISP_PORTAWBOUT                  4UL
#define ISP_CAPSTART                    5UL
#define ISP_PORTAOVF                    6
#define ISP_PORTBOVF                    7
#define ISP_PORTA_FRAME_SYNC            1



// the isp_state::format field define
#define ISP_FORMAT_YUV422               0   //only for CAP
#define ISP_FORMAT_YUV420               0     //only for PORT C
#define ISP_FORMAT_RAWRGB               1
#define ISP_FORMAT_FULLRGB888           2
#define ISP_FORMAT_FULLRGB565           3

//the isp_state::formatb field define
#define ISP_FORMATB_RGB565              0
#define ISP_FORMATB_RGB666              2
#define ISP_FORMATB_RGB888              3

// the isp_state::bayer field define
#define ISP_BAYER_GR                    0
#define ISP_BAYER_R                     1
#define ISP_BAYER_B                     2
#define ISP_BAYER_GB                    3

// the isp_state::yuv sequence relatived field define
#define ISP_VIDEO_YUYV                  0
#define ISP_VIDEO_YVYU                  1
#define ISP_VIDEO_UYVY                  2
#define ISP_VIDEO_VYUY                  3

// the isp_state::tv frame odd/even relatived field define
#define ISP_TV_FRAME_ODD                0
#define ISP_TV_FRAME_EVEN               1
#define ISP_TV_FRAME_BOTH               2

// for ioctl ISP_CFG_SC1_ENABLE flags
#define ISP_SCALE1_ENABLE_XUP           1
#define ISP_SCALE1_ENABLE_YUP           2
#define ISP_SCALE1_XYSAME               4
#define ISP_SCALE1_ENABLE               8 //default to x scale down, y scale down

//for ioctl SC1 and SC2 work mode
#define ISP_SC_MODE_SCALE               0
#define ISP_SC_MODE_DEBLOCK             1

//for porta and portb trimming position
#define ISP_TRAM_LEFT_ALIGN             0
#define ISP_TRAM_IN_CENTER              1
#define ISP_TRAM_RIGHT_ALIGN            2

#define ISP_CFG_MODE                    1        // overall control
#define ISP_CFG_SUB_MODULE              2
#define ISP_CFG_INPUT_FORMAT            3        // format control
#define ISP_CFG_INPUT_FPS               4
#define ISP_CFG_PRE_SKIP_FRAMES         5
#define ISP_CFG_RAWRGB_BAYER            6
#define ISP_CFG_YUV_SEQUENCE            7
#define ISP_CFG_HSYNC_POLARITY          8
#define ISP_CFG_VSYNC_POLARITY          9
#define ISP_CFG_ENABLE_CCIR656          10        // camera interface
#define ISP_CFG_CAP_FRAME_DEL           11
#define ISP_CFG_CAP_LINE_DEL            12
#define ISP_CFG_CUT_FIRST_Y0            13
#define ISP_CFG_TV_FRAME_SEL            14
#define ISP_CFG_YCBCR_TO_YUV            15        //input yuv format
#define ISP_CFG_Y_SHIFT128              16
#define ISP_CFG_UV_SHIFT128             17
#define ISP_CFG_PORTC_PITCH             18
#define ISP_CFG_INPUT_SIZE              19        // position and size
#define ISP_CFG_SCALE1_IN               20
#define ISP_CFG_SCALE1_OUT              21
#define ISP_CFG_SCALE2_IN               22
#define ISP_CFG_SCALE2_OUT              23
#define ISP_CFG_ENCODER_RANGE           24
#define ISP_CFG_DISPLAY_RANGE           25
#define ISP_CFG_PORTA_Y_SHIFT128        26        // port a output format
#define ISP_CFG_PORTA_UV_SHIFT128       27
#define ISP_CFG_PORTB_FORMAT            28        // port b output format
#define ISP_CFG_PORTB_INVERTED          29
#define ISP_CFG_PORTA_BASE              30        // address
#define ISP_CFG_PORTB_BASE              31
#define ISP_CFG_PORTC_BASE              32
#define ISP_CFG_PORTA_FRAME             33
#define ISP_CFG_PORTB_FRAME             34
#define ISP_CFG_PORTC_FRAME             35
#define ISP_CFG_PREFILTER_THRESHOLD     36        // image process
#define ISP_CFG_BLC_CALIBRATION         37
#define ISP_CFG_AWB_START               38
#define ISP_CFG_AWB_DIRECT              39
#define ISP_CFG_AWB_CALCULATE           40
#define ISP_CFG_AWB_READY               41
#define ISP_CFG_AE_CALCULATE            42
#define ISP_CFG_AF_CALCULATE            43
#define ISP_CFG_NSR_WEIGHT              44
#define ISP_CFG_NSR_LIMIT               45
#define ISP_CFG_LCN_CENTER              46
#define ISP_CFG_LCN_PARA                47
#define ISP_CFG_LCN_SQR                 48
#define ISP_CFG_GAMMA_TABLE             49
#define ISP_CFG_CFA                     50
#define ISP_CFG_CCE                     51
#define ISP_CFG_YUV2RGB                 52
#define ISP_CFG_SCALE1_DOWN_COEF        53        // scale 1
#define ISP_CFG_SC1_ENABLE              54
#define ISP_CFG_SC2_ENABLE              55
#define ISP_CFG_BAYER_DEBUG             56
#define ISP_CFG_SC1_REL_FACTORS         57
#define ISP_CFG_SC1_ABS_FACTORS         58
#define ISP_CFG_SC1_GET_ABS_FACTORS     59
#define ISP_CFG_GET_SC1_ENABLE          60
#define ISP_CFG_GET_AWB_MONITOR         61
#define ISP_CFG_MANUAL_START            62
#define ISP_CFG_FRAME_MANUAL            63
#define ISP_CFG_DEFAULT_SCALING         64
#define ISP_CFG_INT_ENABLE              65
//#define ISP_CFG_INT_DISABLE               66
#define ISP_CFG_CAP_INPUT_RANGE         67
#define ISP_CFG_CCE_MATRIX              68
#define ISP_CFG_AWBAE_ENABLE            69
#define ISP_CFG_PCLK_POLARITY           70
#define ISP_CFG_VP_PORTC                71
#define ISP_CFG_VP_REVIEW_SIZE          72
#define ISP_CFG_PORTA_EOL_SKIP          73
#define ISP_CFG_PORTB_EOL_SKIP          74
#define ISP_CFG_PORTC_EOL_SKIP          75
#define ISP_CFG_TRIM_POSITION           76
#define ISP_CFG_CLOCK                   77
#define ISP_SWITCH_BUFFER               78
#define ISP_CFG_DISP_BUFF_OFFSET        79

#define ISP_CFG_VP_DISP_REQ         80
#define ISP_CFG_PORTA_CUR_ADDR          81

#define ISP_DEFAULT_CLK                 48

#define PA_NR_PINGPONG_FRAME                DC_ENCODER_ROUND_BUFFER
#define PB_NR_PINGPONG_FRAME                DC_DISPLAY_ROUND_BUFFER

#define MAX_DEBUG_NUM           500


//#ifndef ISP_SERVICE_THREAD
#define ISP_SERVICE_THREAD ISP //maybe a bastard if not registered to system
//#endif


typedef struct _isp_signal_t
{
    xSignalHeaderRec                    sig;
    uint32                              command;
    uint32                              param;
    void                                *ptr;
} ISP_SIGNAL_T;



typedef struct
{
    uint32  int_id;
    uint32  time;

} ISP_INT_RECORD_T;

typedef struct af_operations_tag
{
    uint32 (*af_ContinuousAutoFocus) (uint32 af_value);
    uint32 (*af_SingleModeAutoFocus) (uint32 af_value);
    void (*af_ManualAdjustMotor) (uint8 step_number);
    void (*af_Open) ();
    void (*af_Close) ();
    void (*af_PreviewAutoFocus) ();
    void (*af_CaptureAutoFocus) ();
} AF_OPERATIONS_T;


typedef struct _mn115_table
{
    // m, n range: 1<=m,n<=15, m>=n
    // this table gives all the combination of m/n,
    // except 15/14, because 15/14==14/13==1.07
    uint32          m;                  //fill line_d, column_d
    uint32          n;                  //fill line_i, column_i
    const           uint32 *up_coef;    //scale down coefficient
    const           uint32 *down_coef;  //scale down coefficient

    //add any quick find item here.
    uint32          up100;              //size of scale up 100
    uint32          down1024;           //size of scale down 1024

} COEF_MN115_TABLE_T;




typedef struct _rect_t //rectangle
{
    unsigned long                       x;
    unsigned long                       y;
    unsigned long                       w;
    unsigned long                       h;
} ISP_RECT_T;

typedef struct _rsize_t //rectangle's size
{
    unsigned long                       w;
    unsigned long                       h;
} ISP_SIZE_T;
typedef struct _isp_frame_t
{
    unsigned long                       type;
    unsigned long                       lock;
    unsigned long                       flags;
    unsigned long                       fid;
    unsigned long                       afval;
    unsigned long                       width;
    unsigned long                       height;
    unsigned long                       yaddr;
    unsigned long                       uaddr;
    unsigned long                       vaddr;
    unsigned long                       rgbaddr;
    struct _isp_frame_t                 *prev;
    struct _isp_frame_t                 *next;
} ISP_FRAME_T;



typedef struct _isp_driver_t
{
    int (*probe) (void);
    int (*reset) (void);
    int (*poweron) (void);
    int (*poweroff) (void);
    int (*suspend) (void);
    int (*resume) (void);
    int (*start) (void);
    int (*pause) (void);
    int (*play) (void);
    int (*stop) (void);
    int (*ioctl) (unsigned long item, void *param);
    int (*lock) (ISP_FRAME_T *f);
    int (*unlock) (ISP_FRAME_T *f);
    int (*getframe) (unsigned long port, ISP_FRAME_T **f);
    int (*setframe) (ISP_FRAME_T *f);
    int (*regevent) (unsigned long type, void (*func) (void *));
    int (*unregevent) (unsigned long type);
} ISP_DRIVER_T;


int     ISP_ModuleInit (void);
void    ISP_ModuleExit (void);
void   *ISP_GetDriver (int major, int minor);

typedef enum
{
    ISP_INT_SENSOR_SOF,             //Sensor's start of frame indication to Camera Interface module
    ISP_INT_CAP_SOF,                //Camera Interface's start of frame indication to the next module.
    ISP_INT_CAP_EOF,                //Camera Interface's end of frame indication to the next module.
    ISP_INT_PORT_A_SUC,             //A frame for encoding is transferred to external memory successfully.
    ISP_INT_PORT_B_SUC,             //A frame for display is transferred external memory successfully.
    ISP_INT_PORT_A_OVF,             //The FIFO in Port A of AHB master is overflowed.
    ISP_INT_PORT_B_OVF,             //The FIFO in Port B of AHB master is overflowed.
    ISP_INT_SC1_FIFO_OVF,           //The FIFO for scaling-up in SC1 is overflowed.
    ISP_INT_CAP_FIFO_OVF,           //The FIFO in camera interface is overflowed.
    ISP_INT_AF_COMP,                //AF monitor complete a frame.
    ISP_INT_LCDC_SOF,               //LCD controller's start of frame indication to SC2.
    ISP_INT_PORT_C_SUC              //A frame is transferred from external memory successfully.
} ISP_INT_E;




/*****************************************************************************/
//  Description:    start AF
//  Global resource dependence: none
//  Author:         Benny.Zou
//  Note:
/*****************************************************************************/
void ISP_StartAF (void);
void ISP_StopAF (void);
void ISP_BeforeGetdataFromLCDC (void *param_ptr);
void ISP_AfterGetdataFromLCDC (void);
void _ISP_Reset (void);
void ISP_SetAFPosition (ISP_RECT_T af_window);
void ISP_SetAFShift (uint8 af_shift);



#endif


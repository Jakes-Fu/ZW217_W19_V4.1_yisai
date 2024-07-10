#ifndef __pah_800x_main_h__
#define __pah_800x_main_h__

// pah
#include "pah_800x.h"
#include "pah_unit.h"
#include "pah_800x_config.h"



//static hr algotirhm
#include "pxialg.h"

// spo2 algorithm
#include "pxialg_spo2.h"

// Stress algorithm
#ifdef PPG_MEMS_Synchronize
//MEMS-INT
#include "pxi_interp.h"
#endif
//============================================================================
// Functions
//============================================================================
#ifdef __cplusplus
extern  "C" {
#endif 


//============================================================================
// Definition
//============================================================================
#ifdef PPG_MEMS_Synchronize
#define FIFO_SIZE 50  //FIFO Size is dependent on the CPU performance
#else
#define FIFO_SIZE 8  //FIFO Size is dependent on the CPU performance
#endif

//============================================================================
// Type Definition
//============================================================================
#pragma pack(1)
typedef struct raw_data_13bytes_t
{
    uint8_t loss;
    uint32_t rawData;
    uint8_t non_zero1;
    uint8_t non_zero2;
    uint8_t non_zero3;
    uint8_t frame_count;
    uint8_t time;
    uint8_t rsv;
    uint8_t touch;
    uint8_t rsv2;
} sRaw_Data_13Bytes;

typedef enum {
    MSG_ALG_NOCHANGE = 0,
    MSG_ALG_ENABLE,
    MSG_ALG_DISABLE,
} alg_msg_code_t;
#pragma pack()

typedef enum
{
    pc_4mm_2g_ir_hrd,           // PC 4.25mm for HRD
    diw_4mm_g_ir_hrd,           // DI+WALL 4.25mm for HRD
    diw_4mm_ir_r_ir_spo2,       // DI+WALL 4.25mm for SPO2
    pc_22mm_2g_ir_hrd,          // PC 2.2mm for HRD
    di_18mm_g_g_hrd,            // DI 1.8mm for HRD

} reg_config_type;

typedef struct {
    
    volatile bool       has_interrupt;
    volatile uint64_t   interrupt_timestamp;
    
    // pah
    pah_800x            pah_800x;
    
    bool                is_Auto_mode;
    // ppg
    bool                ppg_is_enabled;
    //bool                ppg_only_is_enabled;
    bool                ppg_has_prev_report_timestamp;
    uint64_t            ppg_prev_report_timestamp;
    uint8_t             ppg_frame_count;

    // touch
    bool                touch_is_enabled;
    //bool                touch_only_is_enabled;
    int                 touch_last_is_touched;
    
    // alg
    uint8_t             alg_is_enabled;
    bool                hr_is_enabled;
    bool                spo2_is_enabled;
    bool                stress_is_enable;
    // debug
    bool                debug_print_header;
    uint8_t             debug_frame_count;
    
} main_state;

typedef struct {    
    float                 MEMS_Data[3] ;
    pah_800x_ppg_data     ppg_data;
    uint8_t               touch_flag;
    uint8_t               duration;
    uint8_t               ppg_frame_count;
    //pah_800x_touch_data touch_data;
} ppg_mems_data_t;





//============================================================================
// Public function
//============================================================================
void pah_sensor_init(reg_config_type type);
void touch_only_sensor_start(void);
void ppg_only_sensor_start(void);
void auto_mode_sensor_start(void);
void pah_sensor_stop(void);
void ppg_interrupt_handle(void);
void ppg_sensor_task_interrupt(void);
void ppg_sensor_task_polling(void);


bool isFIFOEmpty(void);
bool pah_push(ppg_mems_data_t *data);
bool pah_pop(ppg_mems_data_t *data);


uint8_t Check_alg_enable(void);
void Clear_alg_enable(void);

//HRD
void hr_algorithm_enable(void);
void hr_algorithm_disable(void);
void hr_alg_task(void);
void hr_alg_task2(uint8_t ppg_num);

//SPO2
void spo2_algorithm_enable(void);
void spo2_algorithm_disable(void);
void spo2_alg_task(void);
void spo2_alg_task2(uint8_t ppg_num);

//Stress
void stress_algorithm_enable(void);
void stress_algorithm_disable(void);
//void stress_alg_task(void);
void stress_alg_task(int grade);

#ifdef PPG_MEMS_Synchronize
//MEMS  Interpolation
void mems_interpolation(uint8_t ppg_num);
uint8_t  get_ppg_data_num(void);
void  clear_ppg_data_num(void);
#endif

#ifdef __cplusplus
}
#endif

#endif  // header guard

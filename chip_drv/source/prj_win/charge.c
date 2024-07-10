/********************************************************************************
 ** File Name:      Charge.h                                                    *
 ** Author:         Lin.liu                                                     *
 ** DATE:           08/21/2003                                                  *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:    Manager charge (Li-Battery) and Send event to client        *
 **                                                                             *
 ** Physical Model:                                                             *
 **                 the range of voltage is 3.2V to 4.2V.                       *
 **                 the range of Vprog is 1.5V to 0V                            *
 **                 SM5100B use two ADC. one for voltage of battery. one for    *
 **                 Vprog.                                                      *
 **                 Use the charge int to check disconnect/connect, and GPIO to *
 **                 check charge state.                                         *
 **                                                                             *
 **                 If the charge is not connect, only report the capacity to   *
 **                    client( capacity, worning, shutdow )                     *
 **                 If the charge is connect, when the battery is not enough high, *
 **                    or the Vprog is not enough low, start charge. and don't  *
 **                    report worning/shutdown message to client.               *
 ** Note :                                                                      *
 **                 The battery couldn't be removed while phone is working      *
 ********************************************************************************

 ********************************************************************************
 **                        Edit History                                         *
 ** -------------------------------------------------------------------------   *
 ** DATE           NAME             DESCRIPTION                                 *
 ** 08/21/2003     Lin.liu          Create.                                     *
 ********************************************************************************/

/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "chip_drv_trc.h"
#include "os_api.h"
#include "tb_dal.h"
#include "tb_hal.h"
#include "ref_import.h"
#include "ref_param.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Debug Flag                                        *
 **---------------------------------------------------------------------------*/
//#define DEBUG_CHARGE

#ifdef DEBUG_CHARGE
#define DEBUG_PRINT( _format_string )   SCI_TRACE_LOW _format_string
#else
#define DEBUG_PRINT( _format_string )
#endif


/**---------------------------------------------------------------------------*
 **             The const data define                                         *
 **---------------------------------------------------------------------------*/

// Charge Init-ed flag
#define CHR_INIT_MAGIC          0x43485249      // CHRI
static  uint32                  chr_init_flag;

//
// These are used by AT Command
//

// the max level range and default value
#define CHR_MAX_LEVEL_HIGH                      4200
#define CHR_MAX_LEVEL_LOW                       4000
#define CHR_MAX_LEVEL_DEFAULT                   4100

// the min level range and default value
#define CHR_MIN_LEVEL_HIGH                      3300
#define CHR_MIN_LEVEL_LOW                       3200
#define CHR_MIN_LEVEL_DEFAULT                   3300


static uint32  chr_batt_level_max  = CHR_MAX_LEVEL_DEFAULT;
static uint32  chr_batt_level_min  = CHR_MIN_LEVEL_DEFAULT;

// When lower than it, Should shutdown the phone.
#define CHR_VOLTAGE_SHUTDOWN    3300

// When lower than it, Should notify Client that need charge.
#define CHR_VOLTAGE_WARNING     3500

// When higher than this, the capacity that report to client is 100%
#define CHR_VOLTAGE_100         3900

// When higher than this, the capacity that report to client is 80%
#define CHR_VOLTAGE_80          3800

// When higher than this, the capacity that report to client is 60%
#define CHR_VOLTAGE_60          3700

// When higher than this, the capacity that report to client is 40%
#define CHR_VOLTAGE_40          3600

// When higher than this, the capacity that report to client is 20%
// and notify client that need charge.
#define CHR_VOLTAGE_20          CHR_VOLTAGE_WARNING

// When higher than this, the capacity that report to client is 0%
// and notify client that need power off.
#define CHR_VOLTAGE_0           CHR_VOLTAGE_SHUTDOWN

//
// the Lowest voltage of Vprogram, when lower than this, stop charge
//
#define CHR_PROGRAM_ADC_MIN         0x10

// When lower than this, we start check the program voltage waveform.
// For first, the Li-battery is consand current and the Vprogram is
// 1.5V.
#define CHR_PROGRAM_ADC_START_CHECK   700

// When the program voltage don't falling in the given time slot,
// we think should stop charge.
#define CHR_MAX_VP_TIME_SLOT            (30 * 60 * 1000)  // 30 minutes

// The max charge time. when charge more than it continuely, stop charge.
#define CHR_MAX_CHARGE_TIME             (4 * 3600 * 1000) // 4 Hour


//
//  Charge State & Event define
//
typedef enum
{
    CHR_IDLE,                       // Not in charge
    CHR_CHARGING                    // being charging.
} CHR_STATE;

typedef enum
{
    CHR_START_CHARGE,               // start charge command
    CHR_STOP_CHARGE,                // stop charge  command

    CHR_UPDATE_BATTERY,             // update the battery voltage
    CHR_UPDATE_PROGRAM,

    CHR_DISCONNECT,                 // the charge being disconnect
    CHR_CONNECT                     // the charge being connect.
} CHR_EVENT;

// the state of charge.
static  CHR_STATE   chr_state;

// indicate that the capacity is full, and needn't charge.
//
static uint32  bCapacityFull = 0;

//
// the current voltage(m-voltage) of battery.
//

static volatile uint32  current_battery_voltage;

//
// The current adc of program
static volatile uint32  current_program_adc;

//
//  Used to average of Vprogram
//
static uint32  program_adc_total = 0;
static uint32  program_adc_count = 0;
static uint32  program_adc_max   = 0;
static uint32  program_adc_min   = 0;

static uint32  program_adc_saved = 0xFFFF;
static uint32  program_time_saved    = 0;

// the tick when start charge.
static uint32  start_charge_time     = 0;

static int is_charge_connect (void);
static void reset_program_adc_var (void);

//
// Calculate the time slot between the two given time
//
#define get_time_slot( _start, _end ) \
    ( (_end) >= (_start) ) ? ( (_end) - (_start) ) : (0xFFFFFFFF - (_start) + (_end) )

/*
static uint32 get_time_slot( uint32  start, uint32  end )
{
    uint32    ret;

    return (end >= start)  ?
                (end - start) : (0xFFFFFFFF - start + end);
}
*/

#ifdef WIN32
#define  SCI_ASSERT(x)
#endif
static uint32 voltage_to_percentum (uint32 voltage)
{
    if (voltage >= CHR_VOLTAGE_100)
    {
        return 100;
    }
    else if (voltage >= CHR_VOLTAGE_80)
    {
        return 80;
    }
    else if (voltage >= CHR_VOLTAGE_60)
    {
        return 60;
    }
    else if (voltage >= CHR_VOLTAGE_40)
    {
        return 40;
    }
    else if (voltage >= CHR_VOLTAGE_20)
    {
        return 20;
    }
    else
    {
        return 0;
    }
}

//
// Following is for Calibration.
//

// Support the ADC calibration after this version.
#define CHR_PM_VER_SUPPORT                        0x0204

// The invalid calibration value
#define CHR_INVALID_CALIBRATION                   0xFFFFFFFF

#define CHR_GET_ADC( _v )       ( ((uint32)(_v)) >> 16)
#define CHR_GET_VOLTAGE( _v )   ( ((uint32)(_v)) & 0xFFFF )

#define CHR_MAKE_CALIBRATION( _adc, _voltage )  \
    ( (((uint32)(_adc)) << 16) + (((uint32)(_voltage)) & 0xFFFF) )

static uint32 battery_calibration[2];
static uint32 program_calibration[2];

// The average table of Battery
#define ADC_COMPENSATE    1
static const uint32 battery_table[] =
{
    243 - ADC_COMPENSATE,    // 3.0V
    253 - ADC_COMPENSATE,    // 3.1V
    261 - ADC_COMPENSATE,    // 3.2V
    271 - ADC_COMPENSATE,    // 3.3V
    280 - ADC_COMPENSATE,    // 3.4V
    289 - ADC_COMPENSATE,    // 3.5V
    299 - ADC_COMPENSATE,    // 3.6V
    308 - ADC_COMPENSATE,    // 3.7V
    318 - ADC_COMPENSATE,    // 3.8V
    327 - ADC_COMPENSATE,    // 3.9V
    336 - ADC_COMPENSATE,    // 4.0V
    346 - ADC_COMPENSATE,    // 4.1V
    355 - ADC_COMPENSATE,    // 4.2V

    // tail
    0xFFFFFFFF
};

#define CHR_BATTERY_3_6    battery_table[6];

static uint32 adc_to_battery_voltage (uint32 adc)
{
    uint32    *adc_table = (uint32 *) battery_table;
    uint32   vol, tmp;
    uint32   compensate;

    if (adc >= 0x3FF)
    {
        return 4200;
    }

    if (adc <= 100)
    {
        return 0;
    }

#if 0    // If Voltage calibration is invalid, disable following code.
    compensate = (0x7FFFFFFF >= calibration_3_6)
                 ?   0  : (calibration_3_6 - CHR_BATTERY_3_6;
                           adc += compensate;
#endif

                           if (adc > *adc_table)
{
    // locate
    while (adc > *adc_table)
        {
            adc_table++;
        }

        // Now the *(adc_table - 1) < adc <= *adc_table
        if ( (adc - * (adc_table - 1))
                < (*adc_table - adc))
        {
            adc_table--;
        }

        tmp = adc_table - (uint32 *) (battery_table);

        vol =  3000 + tmp * 100;
    }
    else
    {
        vol = 3000;
    }

    return vol;
}

static const uint32 adc_value_table[] =
{
    31,     // 0.1V
    101,    // 0.2V
    174,    // 0.3V
    247,    // 0.4V
    320,    // 0.5V
    392,    // 0.6V
    466,    // 0.7V
    541,    // 0.8V
    612,    // 0.9V
    686,    // 1.0V
    761,    // 1.1V
    834,    // 1.2V
    910,    // 1.3V
    979,    // 1.4V
    1023,   // 1.5V

    // Tail
    0xFFFFFFFF
};


//
// Send message and capacity to client
//
static void send_msg_to_client (CHR_SVR_MSG_SERVICE_E  msg, uint32 voltage)
{
    static uint32  warning_count  = 0;
    static uint32  shutdown_count = 0;

    uint32     percent = 0;

    //DEBUG_PRINT:"battery Try send message(%d, %d) to Client"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_349_112_2_17_23_8_22_1501,(uint8*)"dd", msg & 0xFF, voltage);

    if (CHR_CAP_IND == msg)
    {
        // maybe need re-map the message
        percent = voltage_to_percentum (voltage);

        if (!is_charge_connect())
        {
#if 0

            // need notify client these message only when charge is not connect
            if (voltage <= CHR_VOLTAGE_SHUTDOWN)  //chr_batt_level_min )
            {
                if (++shutdown_count >= 2)
                {
                    shutdown_count = 0;
                    msg = CHR_SHUTDOWN_IND;
                }
            }
            else
#endif
            {
                shutdown_count = 0;

                if (voltage <= CHR_VOLTAGE_WARNING)
                {
                    if (++warning_count >= 3)
                    {
                        warning_count = 0;
                        msg = CHR_WARNING_IND;
                    }
                }
                else
                {
                    warning_count  = 0;
                }
            }
        }
    }

    // send to client.
    // The percent is valid only when msg == CHR_CAP_IND
    SCI_SendEventToClient (CHR_SERVICE, (uint32) msg, (void *) percent);

    //DEBUG_PRINT:"charge : has sent to client. (%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_394_112_2_17_23_8_22_1502,(uint8*)"d", msg);
}



/**---------------------------------------------------------------------------*
 **             physical interface                                            *
 **---------------------------------------------------------------------------*/
//
// Enable/Disable charge
//
static void enable_charge (int  bEnable)
{
#ifndef WIN32
    GPIO_EnableCharge (bEnable ? TRUE : FALSE);
#endif
}


//
//  check the charge status.
//
void PowerButton_SetMode (BOOLEAN  bGPIO_Mode);
//
static int is_charge_connect (void) // If being connect, return non-zero, otherwise return 0;
{
#ifndef WIN32
    uint32   status;

    SCI_DisableIRQ();
    PowerButton_SetMode (0);
    status = * (volatile uint32 *) (KPD_INT_MSK);
    PowerButton_SetMode (1);
    SCI_RestoreIRQ();

    //DEBUG_PRINT:"is_charge_connect: 0x%X"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_428_112_2_17_23_8_22_1503,(uint8*)"d", status);
    status &= 0x4;   // get charge int status

    return (status) ? 0 : 1;
#else  // WIN32
    // On win32, no charge.
    return 0;
#endif
}


//
// start the charge process and send message to client.
//
static void start_charge (void)
{
    // When start charge, need reset the vars, so when charge again, the value is correct.
    reset_program_adc_var();
    enable_charge (1);
    send_msg_to_client (CHR_CHARGE_START_IND, 0);

    start_charge_time  =  SCI_GetTickCount();
}

//
// stop the charge and send message to MMI
//
static void stop_charge (void)
{
    enable_charge (0);
    send_msg_to_client (CHR_CHARGE_END_IND, 0);
}


//
//   see phycical model in this file header to get more information.
//   if battery voltage is not enough high, or Vprog is not enough low,
//   need charge and return non-zero;
//   otherwise return zero;
//
uint32 need_charge (void)
{
    if (is_charge_connect() && !bCapacityFull)
    {
#if 0

        // check reach the max level first(AT Command set it).
        // Maybe needn't check it.
        if (current_battery_voltage >= chr_batt_level_max)
        {
            //DEBUG_PRINT:"CHR : Has reach max level"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_478_112_2_17_23_8_22_1504,(uint8*)"");
            return 0;
        }

#endif

        if (current_program_adc > CHR_PROGRAM_ADC_MIN)
        {
            //DEBUG_PRINT:"CHR : need charge"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_486_112_2_17_23_8_22_1505,(uint8*)"");
            return 1;
        }
    }

    //DEBUG_PRINT:"CHR : needn't charge"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_491_112_2_17_23_8_22_1506,(uint8*)"");
    return 0;
}

//
//  Charge State Machine.
//
static void charge_state_machine (CHR_EVENT    event)
{
    uint32  tick, time_slot;

    // Has been init-ed ?
    if (CHR_INIT_MAGIC != chr_init_flag)
    {
        return ;
    }

    //DEBUG_PRINT:"chr_stm( %d, %d )"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_508_112_2_17_23_8_22_1507,(uint8*)"dd", chr_state, event);

    switch (chr_state)
    {
        case CHR_IDLE:   // Not in charge

            switch (event)
            {
                case CHR_STOP_CHARGE:               // Nothing need to do
                case CHR_DISCONNECT:                // Nothing need to do
                case CHR_UPDATE_PROGRAM:    // Nothing need to do
                    break;
                case CHR_UPDATE_BATTERY:
                    enable_charge (0);    // Make sure in idle state, the charge is disabled.
                    send_msg_to_client (CHR_CAP_IND, current_battery_voltage);

                    if (current_battery_voltage <= CHR_VOLTAGE_80)
                    {
                        // the capacity is not full.
                        bCapacityFull = 0;
                    }

                case CHR_START_CHARGE:
                case CHR_CONNECT:

                    if (need_charge())
                    {
                        chr_state = CHR_CHARGING;
                        start_charge();
                    }

                default:  // invalid event, ignore it
                    break;
            }

            break;  // CHR_IDLE
        case CHR_CHARGING:

            switch (event)
            {
                case CHR_START_CHARGE:  // Nothing need to do
                case CHR_CONNECT:       // Nothing need to do
                    break;

                case CHR_UPDATE_PROGRAM:

                    if (current_program_adc < CHR_PROGRAM_ADC_START_CHECK)
                    {
                        tick = SCI_GetTickCount();

                        if (current_program_adc < program_adc_saved)
                        {
                            program_adc_saved       = current_program_adc;
                            program_time_saved      = tick;
                        }
                        else
                        {
                            time_slot = get_time_slot (program_time_saved, tick);

                            if (time_slot > CHR_MAX_VP_TIME_SLOT)
                            {
                                //DEBUG_PRINT:"time slot is too long and need stop charge"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_569_112_2_17_23_8_22_1508,(uint8*)"");
                                bCapacityFull   = 1;
                                chr_state       = CHR_IDLE;
                                stop_charge();
                            }
                        }
                    }

                    break;
                case CHR_UPDATE_BATTERY:
                    // Has reach the max charge time ?
                    tick        = SCI_GetTickCount();
                    time_slot   = get_time_slot (start_charge_time, tick);
                    //DEBUG_PRINT:"CHR: time_slot : %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_582_112_2_17_23_8_22_1509,(uint8*)"d", time_slot);

                    if (need_charge() && (time_slot < CHR_MAX_CHARGE_TIME))
                    {
                        send_msg_to_client (CHR_CAP_IND, current_battery_voltage);
                    }
                    else
                    {
                        bCapacityFull   = is_charge_connect() ?  1 : 0;
                        chr_state       = CHR_IDLE;
                        stop_charge();
                    }

                    break;
                case CHR_STOP_CHARGE:
                case CHR_DISCONNECT:
                    bCapacityFull       = 0;
                    chr_state           = CHR_IDLE;
                    stop_charge();
                    break;
                default:  // invalid event, ignore it
                    break;
            }

            break;
        default: // Invalid state, ignore it.
            break;
    } // end switch( chr_state )
}


/**---------------------------------------------------------------------------*
 **             ISR(charge/ADC) handler                                       *
 **---------------------------------------------------------------------------*/

//
//
//  ADC Process.
//
//
// If bFirstTime == 1, update the capacity, so can display the current voltage quickly
static uint32  bFirstTime = 1;
static void adc_battery_voltage_handler (uint32 adc)
{
    // these var used to average the input data.
#define CHR_BATT_ADC_SHIFT_NUM    4
    static uint32  count = 0, total = 0;
    static uint32  max = 0x0, min = 0xFFFF;

    if (bFirstTime)
    {
        current_battery_voltage = adc_to_battery_voltage (adc);
        charge_state_machine (CHR_UPDATE_BATTERY);
        bFirstTime = 0;
    }

    if (count < ( (0x1 << CHR_BATT_ADC_SHIFT_NUM) + 2))
    {
        count++;
        total  += adc;

        if (adc > max)
        {
            max = adc;
        }

        if (adc < min)
        {
            min = adc;
        }
    }
    else
    {
        // remove the max/min value, than average.
        total   -= (min + max);
        total  >>= CHR_BATT_ADC_SHIFT_NUM;
        //DEBUG_PRINT:"The Batt ADC( %d/%d,  %d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_657_112_2_17_23_8_22_1510,(uint8*)"ddd", max, min, total);
        current_battery_voltage = adc_to_battery_voltage (total);

        // reset these var
        count = 0;
        total = 0;
        max   = 0;
        min   = 0xFFFF;

        charge_state_machine (CHR_UPDATE_BATTERY);
    }
}


static void reset_program_adc_var (void)
{
    program_adc_total   = 0;
    program_adc_count   = 0;
    program_adc_max     = 0x0;
    program_adc_min     = 0xFFFF;

    program_adc_saved   = CHR_PROGRAM_ADC_START_CHECK;
    program_time_saved  = 0;
}

#define CHR_PROGRAM_ADC_SHIFT_NUM   4
static void adc_program_voltage_handler (uint32 adc)
{
    if (program_adc_count < ( (0x1 << CHR_PROGRAM_ADC_SHIFT_NUM) + 2))
    {
        program_adc_count++;
        program_adc_total  += adc;

        if (adc > program_adc_max)
        {
            program_adc_max = adc;
        }

        if (adc < program_adc_min)
        {
            program_adc_min = adc;
        }
    }
    else
    {
        program_adc_total  -= (program_adc_max + program_adc_min);
        program_adc_total >>= CHR_PROGRAM_ADC_SHIFT_NUM;
        current_program_adc = program_adc_total;
        reset_program_adc_var();

        charge_state_machine (CHR_UPDATE_PROGRAM);
    }
}


//
// the source of charge.
// 0x1, 0x2, 0x4, 0x8, 0x10
// 5-channel analog inputs are connected to a 10-bit ADC
// through an analog multiplexor.
//
#define ADC_SOURCE_CHARGE    (1u << 1)   // ADIN1
#define ADC_SOURCE_BATTERY   (1u << 4)   // ADIN4

//
// is ADC busy ?
// ADC need a little time, we set the isr handler to handle it.
//
static volatile uint32  is_charge_busy  = 0;
static volatile uint32  adc_sour        = 0;
static volatile uint32  current_adc     = 0;

//
// Handle the ADC isr, read the result.
//
//@Daniel.Ding,CR:MS00004239,9/11/2003,begin
#ifdef USE_NUCLEUS
static void adc_isr_handler (void)
#else
static ISR_EXE_T adc_isr_handler (uint32 tmp)
#endif
{
}

//
// start the process of ADC, the result will be handled in
// ADC_isr_handler.
//
static void start_read_battery (void)
{
#ifndef WIN32
    SCI_DisableIRQ();

    if (0 == is_charge_busy)
    {
        is_charge_busy  = 1;
        adc_sour        = ADC_SOURCE_BATTERY;

        //start ADC and handler the result in isr
        * (volatile uint32 *) (GR_ADCC) = ADC_SOURCE_BATTERY;
    }

    SCI_RestoreIRQ();
#endif
}

static void start_read_program (void)
{
#ifndef WIN32
    SCI_DisableIRQ();

    if (0 == is_charge_busy)
    {
        is_charge_busy  = 1;
        adc_sour        = ADC_SOURCE_CHARGE;

        //start ADC and handler the result in isr
        * (volatile uint32 *) (GR_ADCC) = ADC_SOURCE_CHARGE;
    }

    SCI_RestoreIRQ();
#endif
}


//
//  Report the capacity of battery periodically.
//  called by timer callback or when layer1 process one page( save battery capacity ).
//
void Charge_ReportCapacity (void)
{
    static uint32  bSour = 0;  // we use it to read battery/program voltage by turns.

    static uint32  dwChargeCount = 0;

    if (CHR_INIT_MAGIC == chr_init_flag)
    {
        if ( (CHR_CHARGING == chr_state))
        {
            //DEBUG_PRINT:"In charge_State and try check connect"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_796_112_2_17_23_8_23_1511,(uint8*)"");

            if (!is_charge_connect())
            {
                //DEBUG_PRINT:"charge not connect, and STM"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,CHARGE_800_112_2_17_23_8_23_1512,(uint8*)"");
                charge_state_machine (CHR_DISCONNECT);
            }
        }

        if ( (CHR_IDLE     == chr_state) && is_charge_connect())
        {
            // Notify Client.
            if (dwChargeCount > 1)
            {
                charge_state_machine (CHR_CONNECT);
                dwChargeCount = 0;
            }
            else
            {
                dwChargeCount++;
            }

            return ;
        }

        if (bSour && (CHR_CHARGING == chr_state))
        {
            // Only be in charge state, need check the program voltage
            start_read_program();
        }
        else
        {
            start_read_battery();
        }

        bSour = !bSour;
    }
}


/**---------------------------------------------------------------------------*
 **             API define                                                    *
 **---------------------------------------------------------------------------*/


//
// Close the charge, release the resource.
//
void CHR_Close (void)
{
#ifdef _PUBLIC_TELEPHONE_  // the charge is valid only when defined _PUBLIC_TELEPHONE_    

    if (CHR_INIT_MAGIC == chr_init_flag)
    {
        chr_init_flag = 0;

        stop_charge();
    }

#endif
}

// This is a dummy function.
static void CHRSVR_default (BLOCK_ID id, uint32 argc, void *argv)
{
}

//
// Init the charge, set the charge_state = CHR_IDLE;
// the GPIO has been config.
//
void    CHR_Init (void)
{
    CHR_PARAM_T  *chr_param_ptr;
    chr_init_flag = 0;

    // create client list.
    SCI_CreateClientList (CHR_SERVICE, (CHR_MSG_MAX_NUM & 0x0ff),
                          (REG_CALLBACK) CHRSVR_default);


    // init the var
    bFirstTime              = 1;
    current_battery_voltage = 4200;
    current_program_adc     = 0x3FF;

    reset_program_adc_var();

    chr_state               = CHR_IDLE;

    chr_batt_level_max      = CHR_MAX_LEVEL_DEFAULT;
    chr_batt_level_min      = CHR_MIN_LEVEL_DEFAULT;

    chr_param_ptr = REFPARAM_GetChargeParam();
    //    REF_GetAdcCalibrationPara(battery_calibration,program_calibration,
    //                            CHR_INVALID_CALIBRATION,CHR_PM_VER_SUPPORT);

#ifndef WIN32
    // enable adc interrupt and register handler
    //@Daniel.Ding,CR:MS00004239,9/18/2003,begin
#ifdef HTW_INTEGRATION
    //
#else
    ISR_RegHandler (TB_ADC_INT, adc_isr_handler);
    CHIPDRV_EnableIRQINT (TB_ADC_INT);
#endif
    //Daniel .end
#endif

    // All has done.
    chr_init_flag = CHR_INIT_MAGIC;
}

//
//  Check the battery status.
//
//  Return:
//     The current status of battery.
uint32 CHR_CheckBatteryStaus (void)
{
    if (CHR_INIT_MAGIC != chr_init_flag)
    {
        // this function should be called after init the chr.
        // Pls call CHR_Init first.
        SCI_ASSERT (0);/*assert to do*/
        return CHR_BATTERY_NONE_S;
    }

    switch (chr_state)
    {
        case CHR_IDLE:
            return CHR_BATTERY_NORMAL_S;
            //break;
        case CHR_CHARGING:
            return CHR_BATTERY_CHARGING_S;
            break;
        default:  // the state is not defined.
            SCI_ASSERT (0);/*assert to do*/
            return CHR_BATTERY_NORMAL_S;
            break;
    }
}

//
// Get the battery capacity
//
// Return :
//      the capacity( 0--100 ).
//          0:   no battery.
//          1 -- 99 : capacity
//          >=100   : battery full
//
uint32 CHR_GetBatCapacity (void)
{
    if (CHR_INIT_MAGIC != chr_init_flag)
    {
        // this function should be called after init the chr.
        // Pls call CHR_Init first.
        SCI_ASSERT (0);/*assert to do*/
        return 0;
    }
    else
    {
        return voltage_to_percentum (current_battery_voltage);
    }
}


/* @jim.zhang CR:MS8131 2004-03-25 */

/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the voltage of battery from ADC result.                           *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the voltage of battery.  The units is 0.01 V.                         *
 **---------------------------------------------------------------------------*/
uint32 CHR_GetVoltage (void)
{
    return CHR_GetBattVoltage();
}


/**---------------------------------------------------------------------------*
 ** DESCRIPTION                                                               *
 **     Get the current voltage of battery from ADC result.                   *
 ** Input Parameter                                                           *
 **     None                                                                  *
 ** Retrun                                                                    *
 **     the current voltage of battery.  The units is 0.01 V.                 *
 **---------------------------------------------------------------------------*/
uint32 CHR_GetCurVoltage (void)
{
    return adc_to_battery_voltage (current_adc);
}

/* end CR:MS8131 2004-03-25 */

/*****************************************************************************/
//  Description:    Get charge module state information.
//  Author:         Mingwei.Zhang
/*****************************************************************************/
static CHGMNG_STATE_INFO_T module_state_to_app;
PUBLIC CHGMNG_STATE_INFO_T* CHGMNG_GetModuleState(void)
{
    module_state_to_app.chgmng_state = 0;
    module_state_to_app.bat_statistic_vol = CHR_GetVoltage ();
    module_state_to_app.bat_cur_vol = CHR_GetCurVoltage ();
    module_state_to_app.bat_remain_cap = CHR_GetBatCapacity ();
    
    return &module_state_to_app;
}

/*****************************************************************************
 ** Charge Manager Module ( AT Command )

Charge Manager Module Requirement:
    1£ºstop the battery charge.
    2£ºStart the battery charge.
    3£ºGet the current battery voltage.
    4£ºSet the battery charge parameters.
        The below paramters:
        (1)£ºlevel max for the battery voltage.When it is reached,
             the battery is considered as charged.
             (give the allowed range and the default value)
        (2)£ºlevel min for the battery voltage.When it is reached ,
             the battery is considered as discharged, and the module is turn off.
             (give the allowed range and the default value)
        (3)£ºTime between pulses for the pulsed charge.
             (give the allowed range and the default value)
        (4)£ºtime pulse out charge.
             (give the allowed range and the default value)
        (5)£ºbattery internal resistor.
             (give the allowed range and the default value)
    5:  Get the current battery state. The state will include:
        (0):  Minimum battery level reached - the battery is considered as discharged.
              The product is turned off (as with the +CPOF command).
        (1):  Maximum battery level reached - the battery is considered as charged.
              The battery charging is stopped.
        (2):  Battery currently in charge.
        (3):  Battery currently out of charge.


 *****************************************************************************/


#define CHR_TPULSE_IN_CHARGE_HIGH               10000
#define CHR_TPULSE_IN_CHARGE_LOW                100
#define CHR_TPULSE_IN_CHARGE_DEFAULT            100

#define CHR_TPULSE_OUT_CHARGE_HIGH              10000
#define CHR_TPULSE_OUT_CHARGE_LOW               100
#define CHR_TPULSE_OUT_CHARGE_DEFAULT           5000

#define CHR_BATT_INTERNAL_RESISTOR_HIGH         255
#define CHR_BATT_INTERNAL_RESISTOR_LOW          1
#define CHR_BATT_INTERNAL_RESISTOR_DEFAULT      1


void CHR_GetBattIntResParam (uint32 *low, uint32 *high, uint32 *deft)
{
    * low  = CHR_BATT_INTERNAL_RESISTOR_LOW;
    * high = CHR_BATT_INTERNAL_RESISTOR_HIGH;
    * deft = CHR_BATT_INTERNAL_RESISTOR_DEFAULT;
}

uint32 CHR_SetBattIntRes (uint32 level)
{
    if ( (level <= CHR_BATT_INTERNAL_RESISTOR_HIGH)
            && (level >= CHR_BATT_INTERNAL_RESISTOR_LOW))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void CHR_GetTPulseOutChargeParam (uint32 *low, uint32 *high, uint32 *deft)
{
    * low  = CHR_TPULSE_OUT_CHARGE_LOW;
    * high = CHR_TPULSE_OUT_CHARGE_HIGH;
    * deft = CHR_TPULSE_OUT_CHARGE_DEFAULT;
}

uint32 CHR_SetTPulseOutCharge (uint32 level)
{
    if ( (level <= CHR_TPULSE_OUT_CHARGE_HIGH)
            && (level >= CHR_TPULSE_OUT_CHARGE_LOW))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void CHR_GetTPulseInChargeParam (uint32 *low, uint32 *high, uint32 *deft)
{
    * low  = CHR_TPULSE_IN_CHARGE_LOW;
    * high = CHR_TPULSE_IN_CHARGE_HIGH;
    * deft = CHR_TPULSE_IN_CHARGE_DEFAULT;
}

uint32 CHR_SetTPulseInCharge (uint32 level)
{
    if ( (level <= CHR_TPULSE_IN_CHARGE_HIGH) && (level >= CHR_TPULSE_IN_CHARGE_LOW))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void CHR_GetBattLevelMaxParam (uint32 *low, uint32 *high, uint32 *deft)
{
    * low  = CHR_MAX_LEVEL_LOW;
    * high = CHR_MAX_LEVEL_HIGH;
    * deft = CHR_MAX_LEVEL_DEFAULT;
}

uint32 CHR_SetBattLevelMax (uint32 level)
{
    if ( (level <= CHR_MAX_LEVEL_HIGH) && (level >= CHR_MAX_LEVEL_LOW))
    {
        chr_batt_level_max = level;
        return 1;
    }
    else
    {
        // Invalid param.
        return 0;
    }
}

void CHR_GetBattLevelMinParam (uint32 *low, uint32 *high, uint32 *deft)
{
    * low  = CHR_MIN_LEVEL_LOW;
    * high = CHR_MIN_LEVEL_HIGH;
    * deft = CHR_MIN_LEVEL_DEFAULT;
}

uint32 CHR_SetBattLevelMin (uint32 level)
{
    if ( (level <= CHR_MIN_LEVEL_HIGH) && (level >= CHR_MIN_LEVEL_LOW))
    {
        chr_batt_level_min = level;
        return 1;
    }
    else
    {
        return 0;
    }
}

void CHR_StopCharge (void)
{
    //charge_state_machine( CHR_STOP_CHARGE );
}
void CHR_StartCharge (void)
{
    //charge_state_machine( CHR_START_CHARGE );
}

// return the m-Voltage of current battery voltage
uint32 CHR_GetBattVoltage (void)
{
    return  current_battery_voltage;
}

CHR_BATT_STATE_E   CHR_GetBattState()
{
    CHR_BATT_STATE_E     state;

    if (CHR_IDLE == chr_state)
    {
        if (current_battery_voltage >= chr_batt_level_max)
        {
            state = CHR_BATT_MAX_VOLTAGE;
        }
        else if (current_battery_voltage <= chr_batt_level_min)
        {
            state = CHR_BATT_MIN_VOLTAGE;
        }
        else
        {
            state = CHR_BATT_OUT_OF_CHARGE;
        }
    }
    else
    {
        state = CHR_BATT_IN_CHARGE;
    }

    return state;
}

//apple gao : add for handle charge
#ifdef WIN32
void SetCurrentBattery (uint8 batlevel)
{
    switch (batlevel)
    {
        case 0:
            current_battery_voltage = 2000;
            break;
        case 1:
            current_battery_voltage = 3500;
            break;
        case 2:
            current_battery_voltage = 3600;
            break;
        case 3:
            current_battery_voltage = 3700;
            break;
        case 4:
            current_battery_voltage = 3800;
            break;
        case 5:
            current_battery_voltage = 4000;
            break;
        default:
            break;
    }
}

#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

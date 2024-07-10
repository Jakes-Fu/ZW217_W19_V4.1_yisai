/******************************************************************************
 ** File Name:      production_test.h                                            *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           09/18/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 09/18/2004      Lin.liu         Create.                                   *
 ** 12/17/2008      weihua.wang     modify for tools struct, add GUID element *
 ******************************************************************************/

#ifndef PRODUCTION_TEST_H
#define PRODUCTION_TEST_H
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "tb_dal.h"
#include "tb_hal.h"
#include "adc_drvapi.h"


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Stop the current "CURRENT" test.
//  Author:         Lin.liu
//  Note:           This function will be called befeore test next item.
/*****************************************************************************/
void SCI_ProductTest_Current_Stop (void);

/*****************************************************************************/
//  Description:    Prepare the deepsleep current test.
//  Author:         Lin.liu
//  Note:           This function will be called befeore test deepsleep current.
/*****************************************************************************/
void SCI_ProductTest_Current_Prepare_DeepSleep (void);

/*****************************************************************************/
//  Description:    Test the LED on current.
//  Author:         Lin.liu
//  Note:
/*****************************************************************************/
void SCI_ProductTest_Current_LED_On (void);

/*****************************************************************************/
//  Description:    Test the vibrator on current
//  Author:         Lin.liu
//  Note:
/*****************************************************************************/
void SCI_ProductTest_Current_Vibrator_On (void);

/*****************************************************************************/
//  Description:    Test the charging current
//  Author:         Lin.liu
//  Note:
/*****************************************************************************/
void SCI_ProductTest_Current_Charge (void);


/*****************************************************************************/
//  Description:    Test the Audio module.
//  Input:
//                  failed_pin_list : the buffer that store the failed pin list.
//                                      0xFF means the tail.
//                  max_failed_pin_list : the max buffer of failed pin list buffer.
//  Return:
//                  return non-zero means that the audio test pass. zero indicate failed.
//  Author:         Lin.liu
//  Note:
/*****************************************************************************/
uint32 SCI_ProductTest_Audio (uint8 *failed_pin_list, uint32  max_failed_pin_list);


/*****************************************************************************/
//  Description:    Test the LCM module.
//  Input:
//                  failed_pin_list : the buffer that store the failed pin list.
//                                      0xFF means the tail.
//                  max_failed_pin_list : the max buffer of failed pin list buffer.
//  Return:
//                  return non-zero means that the LCM test pass. zero indicate failed.
//  Author:         Lin.liu
//  Note:           This function will be called befeore test next item.
/*****************************************************************************/
uint32 SCI_ProductTest_LCM (uint8 *failed_pin_list, uint32  max_failed_pin_list);


/*****************************************************************************/
//  Description:    Test the vibrator
//  Input:
//                  failed_pin_list : the buffer that store the failed pin list.
//                                      0xFF means the tail.
//                  max_failed_pin_list : the max buffer of failed pin list buffer.
//  Return:
//                  return non-zero means that the test pass. zero indicate failed.
//  Author:         Lin.liu
//  Note:
/*****************************************************************************/
uint32 SCI_ProductTest_Vibrator (uint8 *failed_pin_list, uint32  max_failed_pin_list);


/*****************************************************************************/
//  Description:    Config the ADC input voltage.
//  Input Param:
//                  adc_src : the channel of the adc. please see ADC_E type
//                  bHightVoltage : If it is non-zero, the input the ADC should be
//                                  config as high voltage. low voltage if it is zero.
//  Author:         Lin.liu
//  Note:
/*****************************************************************************/
void SCI_Calibration_ADC_Config (ADC_E  adc_src,  BOOLEAN  bHighVoltage);


/*****************************************************************************/
//  Description:    Productin Test extend interface.
//  Author:         Lin.liu
//  Note:           This interface only do the data transfer between the customer's
//                  test code and the PC tools.
//                  The length of response to PC tools should be limited to max_rsp_len.
//                  the typically value of it is 1Kbytes.
/*****************************************************************************/
uint32 SCI_ProductTest_Extend (uint8 *src_ptr, uint32 src_len, uint8 *rsp_ptr, uint32 max_rsp_len);


//
// Following function used to help Audio test.
//
/*****************************************************************************/
//  Description:    Switch the audio test channel.
//  Author:         Lin.liu
//  Note:           There are two input channel and two output channed, when
//                  call SCI_ProductTest_Audio_Enter_Loopback_Mode(), the system
//                  use one channel pair, this function will switch to another channel.
/*****************************************************************************/
void    SCI_ProductTest_Audio_SwitchChannel (void);

/*****************************************************************************/
//  Description:    Let Audio enter loopback mode
//  Author:         Lin.liu
//  Return:
//                  return non-zero means that the audio has been enter loopback mode
//                  return zero
//  Note:
/*****************************************************************************/
BOOLEAN SCI_ProductTest_Audio_Enter_Loopback_Mode (void);


/*****************************************************************************/
//  Description:    Exit the audio loopback mode
//  Author:         Lin.liu
//  Note:
/*****************************************************************************/
BOOLEAN SCI_ProductTest_Audio_Exit_Loopback_Mode (void);





/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define PRODUCTION_TEST_SUCCESS         0
#define PRODUCTION_TEST_NOT_DONE        0xFFFFFFFF

#define PRODUCTION_TEST_ITEM_MAX_NUM    12


#define MAX_SN_LEN              24
#define MAX_STATION_NUM          15
#define MAX_STATION_NAME_LEN        10
#define MAX_LAST_DESCRIPTION_LEN    32
enum _ARRAY_LEN
{
    TSN_LEN = 16,
    TT_LEN  = 4,
    TS_LEN  = 16,
    FD_LEN  = 32,
    LTT_LEN = 24,
    MAG_LEN = 4,
    SN_LEN  = 24,
    VER_LEN = 24,
    FAC_LEN = 24,
    PL_LEN  = FAC_LEN,
    RES_LEN1= 32,
    GUID_LEN= 16,
    RES_LEN2= 48
};

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
// Interface between Ref and MMI
//-----------------------------------------------------------------------------
//Production test result information.
typedef struct
{
    char    item_name[20];
    char    test_state[20];
    uint32  error_code;
} _TEST_ITEM_INFO_T;

typedef struct PRODUCTION_TEST_INFO_tag
{
    char    serial_num[32];
    uint16  total_item_num;
    uint16  reserved1;
    _TEST_ITEM_INFO_T   item_info[PRODUCTION_TEST_ITEM_MAX_NUM];
} PRODUCTION_TEST_INFO_T;

//-----------------------------------------------------------------------------
// Interface of MobilePhone program and WIN TOOLS.
//-----------------------------------------------------------------------------
// Description:
//
// The track data comprises a TRACK_DATA_HEADER structure followed by a list of
// TEST_DATA structure. The number of items of the list is determined by the number
// of test steps. The first test step, Write S/N or Download should initialzie
// the whole data block.
//
// TRACE_DATA_HEADER comprises following fields:
// Magic       - Magic[0] = 's', Magic[1] = 't', Magic[2] = '0', Magic = '2', i.e. the
//               string "st02" without the terminal NIL. Under Windows, Magic can be cast
//               as DWORD and compared with 0x32305453 directly.
// BigEndian   - Flag indicates whether values in the block is big-endian or little-endian.
//               Value 0 indicates little-endian, otherwise big-endian.
// Reserved1   - Provided to enforce alignment on 4-bytes boundary.
// SN          - The SN of phone. It must be in ASCII code and with a termianl NIL.
// PcbVersion  - An ASCII string with a terminal NIL describing the version of PCB.
// Factory     - An ASCII string with a terminal NIL describing the name of the factory.
// ProductLine - The name of the product line. An ASCII string with a terminal NIL.
// Reserved2   - Provided to enforce 4-bytes alignment.
// TimeStamp   - An ASCII string with a terminal NIL. When the last test step sets the Status field
//               to 1, it should also set the field to current time.
// StationNum  - The number of the total test steps, i.e. the number of items in the TEST_DATA
//               list.
// SoftwareVersion - An ASCII string with a terminal NIL describing the version of software.
// HardwareVersion - An ASCII string with a terminal NIL describing the version of hardware.
// All the fields in this structure must be initialized by the first test step.
//
// TEST_DATA structure comprises followint fields:
// TestName     - Textual description of the teststation item, set by the first test step.
// TestTimes    - The times of this test has been done on this phone. The first test step
//                must set this field to 0.
// FailureTimes - The times of failed test. The first test step must set this field to 0.
// TestState    - Test result in ascii string format "PASS", "FAIL" or "DCS_TXP", etc"
// FailDesciption-the description of failed reason
// LastTestTime - Textual representation of the time of the test. The first test step must
//                set this field to empty string.
// The first test step knows how many steps there are and their sequence and initialze the
// track data block accordingly. Successive test steps know its own test ID. After reading out
// the TEST_DATA list, it finds its element in the list by item ID and then check the ErrorCode
// field of the element just before it to judge what to do.
//
typedef struct _PHASE_CHECK_ITEM
{
    uint8  TestStationName[TSN_LEN];      /* "CFT" or "AT", etc               */
    uint32 TestTimes;                     /* "012"                            */
    uint32 FailureTimes;                  /* "001"                            */
    uint8  TestState[TS_LEN];             /* "PASS", "FAIL" or "DCS_TXP", etc.*/
    uint8  FailDesciption[FD_LEN];        /* the description of failed reason */
    uint8  LastTestTime[LTT_LEN];         /* "2005/06/01 15:30:07"            */
    uint8  Reserve[RES_LEN1];             /* ""                               */
} TEST_TRACK_DATA_T ;

#if 0
typedef struct _PHASE_CHECK_HEADER
{
    uint32 Magic;                    /*  "SP05"      */
    uint8  BigEndian;                /*  1           */
    uint8  StationNum;               /* the test station number of the testing */
    uint8  Reserved1[2];
    uint8  SN[SN_LEN];               /* SN           */
    uint8  PcbVersion[VER_LEN];      /* PCB version  */
    uint8  HardwareVersion[VER_LEN]; /* HW version   */
    uint8  SoftwareVersion[VER_LEN]; /* SW version   */
    uint8  FactoryName[FAC_LEN];     /* Factory name of the manufactor         */
    uint8  ProductLine[PL_LEN];      /* Product line */
    uint8  Guid[GUID_LEN];
    uint8  Reserve[RES_LEN2];        /* ""           */
} TEST_TRACK_HEADER_T;
#else
typedef struct _PHASE_CHECK_HEADER
{
    uint32  Magic;              // "SP09"   (老接口为SP05)
    int8    SN[MAX_SN_LEN];         // SN , SN_LEN=24
    int8    SN2[MAX_SN_LEN];            // add for Mobile

    int32       StationNum;             // the test station number of the testing
    int8    StationName[MAX_STATION_NUM][MAX_STATION_NAME_LEN];
//    uint8   Reserved[48];               // 固定为0
//    uint16  iItem;                  // part1: Bit0~ Bit_14 indicate test Station,1表示Pass,
    int8    Reserved[13];               // 固定为0
    int8    SignFlag;                   // internal flag 
    char    szLastFailDescription[MAX_LAST_DESCRIPTION_LEN];
    uint16  iTestSign;          // Bit0~Bit14 ---> station0~station 14  if tested. 0: tested, 1: not tested 
    uint16  iItem;              // part1: Bit0~ Bit_14 indicate test Station, 0: Pass, 1: fail    
                                        // part2: Bit15 标志位置成0 	


} TEST_TRACK_HEADER_T;
#endif
typedef struct _PHASE_CHECK_S
{
    TEST_TRACK_HEADER_T header;
    // TEST_TRACK_DATA_T   items[PRODUCTION_TEST_ITEM_MAX_NUM];
} TEST_DATA_INFO_T ;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to get production test result
//                  information.
//  Author:         Xueliang.Wang
//  Note:           This function does nothing if it is called when playing
//                  GenericTone or Ring.
/*****************************************************************************/
PUBLIC uint32 SCI_GetProductionTestInfo ( // If succeed, return SCI_SUCCESS,
    // else return error value
    PRODUCTION_TEST_INFO_T  *info_ptr   // Used to save test result.
);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif  // PRODUCTION_TEST_H



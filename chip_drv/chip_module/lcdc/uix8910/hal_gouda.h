/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/




#ifndef _HAL_GOUDA_H_
#define _HAL_GOUDA_H_

#include "sci_types.h"

/// @defgroup gouda HAL GOUDA Driver
/// This document describes the characteristics of the GOUDA module and how
/// to use them via its Hardware Abstraction Layer API.
///
/// The GOUDA is a dedicated controller that is capable of driving
/// a parallel LCD module with a multi layer engine.
///
/// @{


// =============================================================================
// MACROS
// =============================================================================

/// Number of different set of timings available for the GOUDA LCD
/// configuration. This number should be equal to the number
/// of different system frequencies, so that the GOUDA LCD interface
/// can be configured optimaly for every system frequency.
#define HAL_GOUDA_LCD_TIMINGS_QTY   12


// =============================================================================
// TYPES
// =============================================================================


// =============================================================================
// HAL_GOUDA_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// Type of the user handler function called when a GOUDA related interrupt
/// is triggered.
// =============================================================================
typedef void (*HAL_GOUDA_IRQ_HANDLER_T)(void);

typedef uint32 (*HAL_SYS_FREQ_CHANGE_HANDLER_T)(uint32 sysFreq);

typedef enum
{
    /// No error occured
    HAL_ERR_NO,

    /// A resource reset is required
    HAL_ERR_RESOURCE_RESET,

    /// An attempt to access a busy resource failed
    HAL_ERR_RESOURCE_BUSY,

    /// Timeout while trying to access the resource
    HAL_ERR_RESOURCE_TIMEOUT,

    /// An attempt to access a resource that is not enabled
    HAL_ERR_RESOURCE_NOT_ENABLED,

    /// Invalid parameter
    HAL_ERR_BAD_PARAMETER,

    /// Uart RX FIFO overflowed
    HAL_ERR_UART_RX_OVERFLOW,

    /// Uart TX FIFO overflowed
    HAL_ERR_UART_TX_OVERFLOW,
    HAL_ERR_UART_PARITY,
    HAL_ERR_UART_FRAMING,
    HAL_ERR_UART_BREAK_INT,
    HAL_ERR_TIM_RTC_NOT_VALID,
    HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED,
    HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED,

    /// Communication failure
    HAL_ERR_COMMUNICATION_FAILED,

    HAL_ERR_QTY
} HAL_ERR_T;

// =============================================================================
// HAL_GOUDA_SPI_LINE_T
// -----------------------------------------------------------------------------
/// Define the type of the SLCD SPI line.
// =============================================================================
typedef enum
{
    HAL_GOUDA_SPI_LINE_4 = 0,
    HAL_GOUDA_SPI_LINE_3 = 1,
    HAL_GOUDA_SPI_LINE_4_START_BYTE = 2,
    HAL_GOUDA_SPI_LINE_3_2_LANE =3 ,
} HAL_GOUDA_SPI_LINE_T;


// =============================================================================
// HAL_GOUDA_LCD_TIMINGS_T
// -----------------------------------------------------------------------------
/// This types describes the timing used by the GOUDA LCD interface.
/// An array os those will be needed to describe the GOUDA LCD
/// timings for each system frequency.
// =============================================================================
#pragma anon_unions
typedef union
{
    struct
    {
        /// Address setup time (RS to WR, RS to RD) in clock number
        uint32 tas:3;
        uint32 :1;
        /// Adress hold time in clock number
        uint32 tah:3;
        uint32 :1;
        /// Control  pulse width low, in clock number
        uint32 pwl:6;
        uint32 :2;
        /// Control pulse width high, in clock number
        uint32 pwh:6;
        uint32 :10;
    };

    /// Use this 'reg' field of the union
    /// to access this timing configuration as
    /// a 32 bits value than can be written to
    /// a register.
    uint32 reg;
} HAL_GOUDA_LCD_TIMINGS_T;


// =============================================================================
// HAL_GOUDA_LCD_CS_T
// -----------------------------------------------------------------------------
/// Defines the Chip Select use to select the LCD screen connected to the
/// GOUDA_LCD controller.
// =============================================================================
typedef enum
{
    HAL_GOUDA_LCD_CS_0 = 0,
    HAL_GOUDA_LCD_CS_1 = 1,

    HAL_GOUDA_LCD_CS_QTY,

    HAL_GOUDA_LCD_MEMORY_IF = 2,
    HAL_GOUDA_LCD_IN_RAM = 3

} HAL_GOUDA_LCD_CS_T;


// =============================================================================
// HAL_GOUDA_LCD_OUTPUT_FORMAT_T
// -----------------------------------------------------------------------------
/// Describes the Output format of the LCD bus.
/// The MSB select also the AHB access size (8-bit or 16-bit) when Memory destination is selected.
/// Must set to RGB565 when RAM type destination selected
// =============================================================================
typedef enum
{
    /// 8-bit - RGB3:3:2 - 1cycle/1pixel - RRRGGGBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_8_BIT_RGB332    = 0,
    /// 8-bit - RGB4:4:4 - 3cycle/2pixel - RRRRGGGG/BBBBRRRR/GGGGBBBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_8_bit_RGB444    = 1,
    /// 8-bit - RGB5:6:5 - 2cycle/1pixel - RRRRRGGG/GGGBBBBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_8_bit_RGB565    = 2,
    /// 16-bit - RGB3:3:2 - 1cycle/2pixel - RRRGGGBBRRRGGGBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB332   = 4,
    /// 16-bit - RGB4:4:4 - 1cycle/1pixel - XXXXRRRRGGGGBBBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB444   = 5,
    /// 16-bit - RGB5:6:5 - 1cycle/1pixel - RRRRRGGGGGGBBBBB
    HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565   = 6

} HAL_GOUDA_LCD_OUTPUT_FORMAT_T;

// =============================================================================
// HAL_GOUDA_LCD_CONFIG_T
// -----------------------------------------------------------------------------
/// Describes the configuration of the GOUDA LCD interface.
// =============================================================================
typedef union
{
    struct
    {
        HAL_GOUDA_LCD_CS_T              cs:2;
        uint32 :2;
        HAL_GOUDA_LCD_OUTPUT_FORMAT_T   outputFormat:3;
        BOOLEAN                            highByte:1;
        /// \c FALSE is active low, \c TRUE is active high.
        BOOLEAN                            cs0Polarity:1;
        /// \c FALSE is active low, \c TRUE is active high.
        BOOLEAN                            cs1Polarity:1;
        /// \c FALSE is active low, \c TRUE is active high.
        BOOLEAN                            rsPolarity:1;
        /// \c FALSE is active low, \c TRUE is active high.
        BOOLEAN                            wrPolarity:1;
        /// \c FALSE is active low, \c TRUE is active high.
        BOOLEAN                            rdPolarity:1;
        uint32 :3;
        /// Number of command to be send to the LCD command (up to 31)
        uint32                          nbCmd:5;
        uint32 :3;
        /// Start command transfer only. Autoreset
        BOOLEAN                            startCmdOnly:1;
        BOOLEAN                            resetb:1;
        uint32 :6;
    }b;

    /// Use this 'reg' field of the union
    /// to access this timing configuration as
    /// a 32 bits value than can be written to
    /// a register.
    uint32 reg;
} HAL_GOUDA_LCD_CONFIG_T;

// =============================================================================
// HAL_GOUDA_SERIAL_LCD_CONFIG_T
// -----------------------------------------------------------------------------
/// Describes the configuration of the GOUDA serial LCD interface.
// =============================================================================
typedef union
{
    struct
    {
        BOOLEAN isSlcd:1;
        uint32 deviceId:6;
        uint32 clkDivider:8;
        uint32 dummyCycle:3;
        uint32 lineType:2;
        uint32 rxByte:3;
        BOOLEAN isRead:1;
    } b;

    uint32 reg;
} HAL_GOUDA_SERIAL_LCD_CONFIG_T;

// =============================================================================
// HAL_GOUDA_LCD_CMD_T
// -----------------------------------------------------------------------------
/// Describes a command send to LCD
// =============================================================================
typedef struct
{
    /// \FALSE for a command, \TRUE for a data
    BOOLEAN    isData;
    /// value of data or command
    uint16  value;
} HAL_GOUDA_LCD_CMD_T;

// =============================================================================
// HAL_GOUDA_WINDOW_T
// -----------------------------------------------------------------------------
/// Describes a window by a top left point
/// and a bottom right point
// =============================================================================
typedef struct
{
    /// top left point
    uint16  tlPX;
    uint16  tlPY;
    /// bottom right point
    uint16  brPX;
    uint16  brPY;

} HAL_GOUDA_WINDOW_T;

// =============================================================================
// HAL_GOUDA_IMG_FORMAT_T
// -----------------------------------------------------------------------------
/// Describes the image input format
// =============================================================================
typedef enum
{
    HAL_GOUDA_IMG_FORMAT_RGB565,
    HAL_GOUDA_IMG_FORMAT_RGBA,
    HAL_GOUDA_IMG_FORMAT_UYVY,
    HAL_GOUDA_IMG_FORMAT_YUYV,
    HAL_GOUDA_IMG_FORMAT_IYUV
} HAL_GOUDA_IMG_FORMAT_T;

// =============================================================================
// HAL_GOUDA_CKEY_MASK_T
// -----------------------------------------------------------------------------
/// Describes the color mask for Chroma Keying.
/// This allows a range of color as transparent.
// =============================================================================
typedef enum
{
    /// exact color match
    HAL_GOUDA_CKEY_MASK_OFF = 0,
    /// disregard 1 LSBit of each color component for matching
    HAL_GOUDA_CKEY_MASK_1LSB = 1,
    /// disregard 2 LSBit of each color component for matching
    HAL_GOUDA_CKEY_MASK_2LSB = 3,
    /// disregard 3 LSBit of each color component for matching
    HAL_GOUDA_CKEY_MASK_3LSB = 7
} HAL_GOUDA_CKEY_MASK_T;

// =============================================================================
// HAL_GOUDA_OVL_LAYER_ID_T
// -----------------------------------------------------------------------------
/// Describes the overlay layers Ids.
// =============================================================================
typedef enum
{
    HAL_GOUDA_OVL_LAYER_ID0,
    HAL_GOUDA_OVL_LAYER_ID1,
    HAL_GOUDA_OVL_LAYER_ID2,

    HAL_GOUDA_OVL_LAYER_ID_QTY
} HAL_GOUDA_OVL_LAYER_ID_T;

// =============================================================================
// HAL_GOUDA_OVL_LAYER_DEF_T
// -----------------------------------------------------------------------------
/// Describes the configuration of an overlay layer.
// =============================================================================
typedef struct
{
    ///@todo check with uint16* (?)
    uint32* addr;
    HAL_GOUDA_IMG_FORMAT_T fmt;
    /// buffer width in bytes including optional padding, when 0 will be calculated
    uint16  stride;
    HAL_GOUDA_WINDOW_T pos;
    uint8   alpha;
    BOOLEAN    cKeyEn;
    uint16  cKeyColor;
    HAL_GOUDA_CKEY_MASK_T cKeyMask;
} HAL_GOUDA_OVL_LAYER_DEF_T;


// =============================================================================
// HAL_GOUDA_VID_LAYER_DEPTH_T
// -----------------------------------------------------------------------------
/// Describes the video layer depth
// =============================================================================
typedef enum
{
    /// Video layer behind all Overlay layers
    HAL_GOUDA_VID_LAYER_BEHIND_ALL = 0,
    /// Video layer between Overlay layers 1 and 0
    HAL_GOUDA_VID_LAYER_BETWEEN_1_0 ,
    /// Video layer between Overlay layers 2 and 1
    HAL_GOUDA_VID_LAYER_BETWEEN_2_1 ,
    /// Video layer on top of all Overlay layers
    HAL_GOUDA_VID_LAYER_OVER_ALL

} HAL_GOUDA_VID_LAYER_DEPTH_T;



// =============================================================================
// HAL_GOUDA_VID_LAYER_DEF_T
// -----------------------------------------------------------------------------
/// Describes the configuration of the video layer.
// =============================================================================
typedef struct
{
    uint32* addrY;
    uint32* addrU;
    uint32* addrV;
    HAL_GOUDA_IMG_FORMAT_T fmt;
    uint16  height;
    uint16  width;
    uint16  stride;
    HAL_GOUDA_WINDOW_T pos;
    uint8   alpha;
    BOOLEAN    cKeyEn;
    uint16  cKeyColor;
    HAL_GOUDA_CKEY_MASK_T cKeyMask;
    HAL_GOUDA_VID_LAYER_DEPTH_T depth;
    uint8 rotation;
} HAL_GOUDA_VID_LAYER_DEF_T;


// =============================================================================
// FUNCTIONS
// =============================================================================


// =============================================================================
// hal_GoudaOpen
// -----------------------------------------------------------------------------
/// Open the LCD controller of Gouda
/// Parameters are used to describe the screen connexion configuration and
/// the controller timings to use depending on the system clock frequency.
/// @param config Pointer to the controller configuration.
/// @param timings Controller timing array (must be provided and valid,
///   even if unused in case of #HAL_GOUDA_LCD_MEMORY_IF)
/// @param ebcCsAddr address of ebc CS for #HAL_GOUDA_LCD_MEMORY_IF,
///   can be 0 for gouda CS.
// =============================================================================
PUBLIC void hal_GoudaOpen(CONST HAL_GOUDA_LCD_CONFIG_T* config, CONST HAL_GOUDA_LCD_TIMINGS_T* timings, uint32 ebcCsAddr);


// =============================================================================
// hal_GoudaSerialOpen
// -----------------------------------------------------------------------------
/// Open the serial LCD controller of Gouda
/// Parameters are used to describe the screen connexion configuration and
/// the controller timings to use depending on the system clock frequency.
/// @param spiLineType The SPI line type of the serial LCD.
/// @param spiFreq The SPI frequency of the serial LCD.
/// @param config Pointer to the controller configuration.
/// @param ebcCsAddr address of ebc CS for #HAL_GOUDA_LCD_MEMORY_IF,
///   can be 0 for gouda CS.
// =============================================================================
PUBLIC void hal_GoudaSerialOpen(HAL_GOUDA_SPI_LINE_T spiLineType, uint32 spiFreq, CONST HAL_GOUDA_LCD_CONFIG_T* config, uint32 ebcCsAddr);

PUBLIC void hal_GoudaSerialMode(HAL_GOUDA_SPI_LINE_T spiLineType);

// =============================================================================
// hal_GoudaClose
// -----------------------------------------------------------------------------
/// Close Gouda.
// =============================================================================
PUBLIC void hal_GoudaClose(void);


// =============================================================================
// hal_GoudaWriteReg
// -----------------------------------------------------------------------------
/// Write a command and then a data to the LCD screen (Or a value at a register
/// address, depending on the point of view).
///
/// @param addr Address of the register to write, or command to send
/// to the LCD screen
/// @param data Corresponding data to write to the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteReg(uint16 addr, uint16 data);


// =============================================================================
// hal_GoudaReadData
// -----------------------------------------------------------------------------
/// Read a data from the LCD screen (Or a value at a register
/// address, depending on the point of view).
///
/// @param addr Address of the register to read
/// @param pData Pointer of the corresponding data to read from the LCD.
/// @param len Data length to read from the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaReadData(uint16 addr, uint8 *pData, uint32 len);


// =============================================================================
// hal_GoudaReadReg
// -----------------------------------------------------------------------------
/// Read a data from the LCD screen (Or a value at a register
/// address, depending on the point of view).
///
/// @param addr Address of the register to read
/// @param pData Pointer of the corresponding data to read from the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaReadReg(uint16 addr, uint16 *pData);


// =============================================================================
// hal_GoudaWriteCmd
// -----------------------------------------------------------------------------
/// Write a command to the LCD.
///
/// @param addr Command to write.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteCmd(uint16 addr);


// =============================================================================
// hal_GoudaWriteData
// -----------------------------------------------------------------------------
/// Write a data to the LCD screen.
///
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteData(uint16 data);

// =============================================================================
// hal_GoudaBlitRoi
// -----------------------------------------------------------------------------
/// Send a block of pixel data to the LCD screen.
///
/// @param pRoi Pointer to the roi description to send to the screen.
/// @param nbCmd number of commands to send to LCD before pixel data
/// @param pCmds commands to send to LCD before pixel data can be \c NULL if \p nbCmd is 0
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaBlitRoi(CONST HAL_GOUDA_WINDOW_T* pRoi, CONST uint32 nbCmd, CONST HAL_GOUDA_LCD_CMD_T* pCmds, HAL_GOUDA_IRQ_HANDLER_T handler);

// =============================================================================
// hal_GoudaBlitRoi2Ram
// -----------------------------------------------------------------------------
/// Send a block of pixel data to a flat ram buffer in RGB565 format
/// WARNING: only even pixel number are supported, that is to say size must
/// be a multiple of 4 bytes.
///
/// @param pBuffer Pointer to the ram buffer
/// @param width Buffer width in pixel, if 0 the buffer is considered to have
/// the same width as the roi.
/// @param pRoi Pointer to the roi description to send to the screen.
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaBlitRoi2Ram(uint32* pBuffer, CONST uint16 width, CONST HAL_GOUDA_WINDOW_T* pRoi, HAL_GOUDA_IRQ_HANDLER_T handler);


// =============================================================================
// hal_GoudaIsActive
// -----------------------------------------------------------------------------
/// Get the activity status
/// @return \c TRUE when active
// =============================================================================
PUBLIC BOOLEAN hal_GoudaIsActive(void);


// =============================================================================
// hal_GoudaSetBgColor
// -----------------------------------------------------------------------------
/// Set the background color
/// @param color in RGB565 format
// =============================================================================
PUBLIC void hal_GoudaSetBgColor(uint16 color);

// =============================================================================
// hal_GoudaGetBgColor
// -----------------------------------------------------------------------------
/// Get the background color
/// @param color in RGB565 format
// =============================================================================
PUBLIC uint16 hal_GoudaGetBgColor(void);

// =============================================================================
// hal_GoudaOvlLayerOpen
// -----------------------------------------------------------------------------
/// Open and configure an overlay layer.
///
/// @param id the layer Id
/// @param def the configuration
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the layer is already enabled.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaOvlLayerOpen(HAL_GOUDA_OVL_LAYER_ID_T id, HAL_GOUDA_OVL_LAYER_DEF_T* def);

// =============================================================================
// hal_GoudaOvlLayerClose
// -----------------------------------------------------------------------------
/// Close an overlay layer.
// =============================================================================
PUBLIC void hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID_T id);


// =============================================================================
// hal_GoudaVidLayerOpen
// -----------------------------------------------------------------------------
/// Open and configure the video layer.
///
/// @param def the configuration
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the layer is already enabled.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaVidLayerOpen(HAL_GOUDA_VID_LAYER_DEF_T* def);


// =============================================================================
// hal_GoudaVidLayerClose
// -----------------------------------------------------------------------------
/// Close the video layer.
// =============================================================================
PUBLIC void hal_GoudaVidLayerClose(void);

// =============================================================================
// hal_GoudaVidLayerState
// -----------------------------------------------------------------------------
/// Get an video layer state.
// =============================================================================
PUBLIC uint32 hal_GoudaVidLayerState(void);

// =============================================================================
// hal_GoudaInitResetPin
// -----------------------------------------------------------------------------
/// Initialize the status of LCD reset pin.
// =============================================================================
PUBLIC void hal_GoudaInitResetPin(void);

// =============================================================================
// hal_GoudaGetTiming
// -----------------------------------------------------------------------------
/// Get lcd timing.
// =============================================================================
PUBLIC uint32 hal_GoudaGetTiming(void);

// =============================================================================
// hal_GoudaSetTiming
// -----------------------------------------------------------------------------
/// Set lcd timing.
// =============================================================================
PUBLIC void hal_GoudaSetTiming(uint32 lcd_timing);

// =============================================================================
// hal_GoudaSetBlock
// -----------------------------------------------------------------------------
/// Indicate whether to wait until gouda blit operations finish.
/// @param block The block flag, with a non-zero value to wait, 0 otherwise.
// =============================================================================
PUBLIC void hal_GoudaSetBlock(uint32 block);

// =============================================================================
// hal_GoudaSetSpiFreq
// -----------------------------------------------------------------------------
/// Set SPI Frequency timing.
// =============================================================================
PUBLIC void hal_GoudaSetSpiFreq(uint32 spiFreq);
extern void hal_GoudaSetFmarkBusy(BOOLEAN busy);
/// @} end of group
#endif // _HAL_GOUDA_H_



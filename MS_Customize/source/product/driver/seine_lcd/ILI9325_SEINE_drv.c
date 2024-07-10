#include "ms_customize_trc.h"
#include "sci_types.h"
#include "mtv_cfg.h"
#include "os_api.h"


//replace ILI9325 with new lcd driver if new lcd is used
#ifdef LCD_DRV_IC
#undef LCD_DRV_IC
#endif
#define LCD_DRV_IC ILI9325

#define MCU_FMARK_MODE                          1

//DO NOT MODIFY MACRO DIFINITION BELOW
#define _SEINE_LCD_INIT(x)              x##_OP_Init
#define _SEINE_LCD_ENTERSLEEP(x)        x##_OP_EnterSleep
#define _SEINE_LCD_SETCONTRAST(x)       x##_OP_SetContrast
#define _SEINE_LCD_SETBRIGHTNESS(x)     x##_OP_SetBrightness
#define _SEINE_LCD_SETWINDOW(x)         x##_OP_SetDisplayWindow
#define _SEINE_LCD_GETINFO(x)           x##_OP_GetInfo
#define _SEINE_LCD_SETFRAMERATE(x)      x##_OP_SetFrameRate
#define _SEINE_LCD_CLEAR(x)             x##_OP_Clear
#define _SEINE_LCD_CLOSE(x)             x##_OP_Close
#define _SEINE_LCD_OPEN(x)              x##_OP_Open
#define _SEINE_LCD_SETDIRECTION(x)      x##_OP_SetDirection
#define _SEINE_LCD_GET_CONFIG(x)        LCD_##x##_GetSeineCfg
#define _SEINE_LCD_MODE(x)              x##_mode
#define _SEINE_LCD_TIMING(x)            x##_timing
#define _SEINE_LCD_GAMMA_TABLE(x)       x##_gamma_table

#define SEINE_LCD_INIT(x)              _SEINE_LCD_INIT(x)
#define SEINE_LCD_ENTERSLEEP(x)        _SEINE_LCD_ENTERSLEEP(x)
#define SEINE_LCD_SETCONTRAST(x)       _SEINE_LCD_SETCONTRAST(x)
#define SEINE_LCD_SETBRIGHTNESS(x)     _SEINE_LCD_SETBRIGHTNESS(x)
#define SEINE_LCD_SETWINDOW(x)         _SEINE_LCD_SETWINDOW(x)
#define SEINE_LCD_GETINFO(x)           _SEINE_LCD_GETINFO(x)
#define SEINE_LCD_SETFRAMERATE(x)      _SEINE_LCD_SETFRAMERATE(x) 
#define SEINE_LCD_CLEAR(x)             _SEINE_LCD_CLEAR(x) 
#define SEINE_LCD_CLOSE(x)             _SEINE_LCD_CLOSE(x)
#define SEINE_LCD_OPEN(x)              _SEINE_LCD_OPEN(x)  
#define SEINE_LCD_SETDIRECTION(x)      _SEINE_LCD_SETDIRECTION(x) 
#define SEINE_LCD_GET_CONFIG(x)        _SEINE_LCD_GET_CONFIG(x)
#define SEINE_LCD_MODE(x)              _SEINE_LCD_MODE(x)
#define SEINE_LCD_TIMING(x)            _SEINE_LCD_TIMING(x)
#define SEINE_LCD_GAMMA_TABLE(x)       _SEINE_LCD_GAMMA_TABLE(x)

//modify this struct according hardware setting and lcd driver spec if new lcd used
const LCD_MCU_MODE_PARAM_T SEINE_LCD_MODE(LCD_DRV_IC) = 
{
    LCD_MCU_SYNC_MODE_VSYNC,
    LCD_MCU_BUS_MODE_8080,
    LCD_SIGNAL_POL_HIGH,
    LCD_SIGNAL_POL_LOW,
    LCD_MCU_DATA_TYPE_RGB565
};

//modify this struct according lcd driver spec if new lcd used
/*
vsync width: let it be 1000 if you don't know what is it.
whp: write high pluse width, normally can be found in lcd driver spec ac timing
wlp: write low pluse width, normally can be found in lcd driver spec ac timing
wcs: write cs setup timing, normally can be found in lcd driver spec ac timing
rhp: read high pluse width, normally can be found in lcd driver spec ac timing
rlp: read low pluse width, normally can be found in lcd driver spec ac timing
rcs: read cs setup timing, normally can be found in lcd driver spec ac timing
*/
const LCD_MCU_TIMING_PARAM_T SEINE_LCD_TIMING(LCD_DRV_IC) = 
{
    1000,        //vsync output width if lcd is used as vsync mode
    50,50,10,    //whp, wlp, wcs
    150,150,5  //rhp, rlp, rcs
};


//modify the gamma table according lcm type
/*
this setting will change the video brightness just only when playing tv.

the parameter should be no more than 0xff

there is two test setting value for make sure the gamma enhancement function worked.
when the setting is 
{0x00, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 
 0x48, 0x50, 0x58, 0x60 ,0x68, 0x70, 0x78, 0x80 ,0x85}
the video on lcd will be more darker than normal;

when the setting is 
{0x00, 0x80, 0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 
 0xb8, 0xc0, 0xc8, 0xd0 ,0xd8, 0xe0, 0xe8, 0xf0 ,0xf5}
the video on lcd will be more brighter than normal; 

howerver, it needs setting all 17 members of SEINE_LCD_GAMMA_TABLE(LCD_DRV_IC) carefully to 
get certain birghtness you want.

if you want keep the original video brightness, just setting the value below:
{0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 
 0x90, 0xa0, 0xb0 ,0xc0, 0xd0, 0xe0, 0xf0 ,0xff}
*/
const uint16 SEINE_LCD_GAMMA_TABLE(LCD_DRV_IC)[17] = 
{
    0x0, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 
    0x90, 0xa0, 0xb0 ,0xc0, 0xd0, 0xe0, 0xf0 ,0xff
};

//modify config sequence below according lcd driver spec if new lcd uesd


//lcd init sequence
/*
if lcd reset pin is connected to BB lcm reset pin, 
this sequence may contain only an empty item {0, 0, 0, 0}
*/
const LCD_OP_CELL SEINE_LCD_INIT(LCD_DRV_IC)[] = 
{
    {0, 0, 0x0000, 0x0000}
};

//lcd enter sleep sequence
/*
this sequence may contain only an empty item {0, 0, 0, 0}
*/
const LCD_OP_CELL SEINE_LCD_ENTERSLEEP(LCD_DRV_IC)[] = 
{
    {0, 0, 0x0000, 0x0000}
};

//lcd set contrast sequence
/*
this sequence may contain only an empty item {0, 0, 0, 0}
*/
const LCD_OP_CELL SEINE_LCD_SETCONTRAST(LCD_DRV_IC)[] = 
{
    {0, 0, 0x0000, 0x0000}
};

//lcd set brightness sequence
/*
this sequence may contain only an empty item {0, 0, 0, 0}
*/
const LCD_OP_CELL SEINE_LCD_SETBRIGHTNESS(LCD_DRV_IC)[] = 
{
    {0, 0, 0x0000, 0x0000}
};

//lcd set refresh(display) window sequence
/*
this config should setting the lcd refresh(display) window with full windows size,
and set start refresh(display) address at left, top point, and leave the lcd in receiving 
refresh(display) data state
*/
const LCD_OP_CELL SEINE_LCD_SETWINDOW(LCD_DRV_IC)[] = 
{
    {3, 0, 0x0050, 0  },	    // Horizontal GRAM Start Address
    {3, 0, 0x0051, 239},	    // Horizontal GRAM End Address
    {3, 0, 0x0052, 0  },	    // Vertical GRAM Start Address
    {3, 0, 0x0053, 319},	    // Vertical GRAM End Address
    
    {3, 0, 0x0020, 0x0000},     // GRAM horizontal Address
    {3, 0, 0x0021, 0x0000},     // GRAM Vertical Address
    
	{1, 0, 0x0022, 0x0000},		// send data.
	{0, 0, 0x0000, 0x0000}
};

//lcd info sequence
/*
this config supply the lcd width and height info
240 is lcd width, 320 is lcd height, replace them with new lcd width and height
*/
const LCD_OP_CELL SEINE_LCD_GETINFO(LCD_DRV_IC)[] = 
{
    {0, 0, 240, 320}            
};

//lcd set frame rate sequence
/*
this config sets the fmark output frequency about 30HZ if lcd's fmark
signal is used
*/
const LCD_OP_CELL SEINE_LCD_SETFRAMERATE(LCD_DRV_IC)[] = 
{
#if MCU_FMARK_MODE
    {3, 0, 0x000a, 0x000B},  //frequency of fmark should be about 30
#endif
    {1, 0, 0x0022, 0x0000},
    {0, 0, 0x0000, 0x0000}
};

//lcd clear sequence
/*
this sequence may contain only an empty item {0, 0, 0, 0}
*/
const LCD_OP_CELL SEINE_LCD_CLEAR(LCD_DRV_IC)[] = 
{
    {0, 0, 0x0000, 0x0000}
};

//lcd close sequence
/*
this sequence may stop lcd's fmark to output if lcd is drived with fmark mode
or this sequence may sets lcd in normal display mode if lcd is drived width vsync mode
*/
const LCD_OP_CELL SEINE_LCD_CLOSE(LCD_DRV_IC)[] = 
{
    {1, 0, 0x0022, 0x0000},
    {0, 0, 0x0000, 0x0000}
};

//lcd open sequence
/*
this sequence may sets lcd's fmark signal output if lcd is drived with fmark mode
or this sequence may sets lcd in vsync display mode if lcd is drived width vsync mode
*/
const LCD_OP_CELL SEINE_LCD_OPEN(LCD_DRV_IC)[] = 
{
    {1, 0, 0x0022, 0x0000},
    {0, 0, 0x0000, 0x0000}
};

//lcd set display direction sequence
/*
this sequence ensure lcd display direction angle is 0
this sequence may contain only an empty item {0, 0, 0, 0} if default angle is 0
*/
const LCD_OP_CELL SEINE_LCD_SETDIRECTION(LCD_DRV_IC)[] = 
{
    {0, 0, 0x0000, 0x0000}
};




static uint32 lcd_com_cfg_buf[256];


LOCAL uint32 *LCDCOM_GetLCDCfgBuf(uint32 *len)
{
    *len = sizeof(lcd_com_cfg_buf);
    return lcd_com_cfg_buf;
}

/*
DO NOT MODIFY THE FUNCTION BELOW
THE FUNCTION CAN BE CALLED WITH NAME LCD_LGDP4531_GetSeineCfg.
FOR THE MACRO DEFINITION "LCD_DRV_IC" IS DEFINED LGDP4531
*/

PUBLIC void SEINE_LCD_GET_CONFIG(LCD_DRV_IC)(uint32 **cfg_pptr, uint32 *len_ptr)
{
    uint32 cfg_buf_len;
    LCD_COM_CFG *lcd_cfg_ptr      = (LCD_COM_CFG*)LCDCOM_GetLCDCfgBuf(&cfg_buf_len);

    LCD_OP_CELL *cfg_addr[11];
    uint32      len[11];
    uint32 i, total_len, *offset_ptr;
    uint8  *cfg_offset_ptr;
    
    lcd_cfg_ptr->ctrl_mode     = LCD_CTRL_MODE_MCU;
    lcd_cfg_ptr->mode_param    = SEINE_LCD_MODE(LCD_DRV_IC);
    lcd_cfg_ptr->timing_param  = SEINE_LCD_TIMING(LCD_DRV_IC);
    SCI_MEMCPY(lcd_cfg_ptr->lcd_gamma_table, SEINE_LCD_GAMMA_TABLE(LCD_DRV_IC), \
                sizeof(SEINE_LCD_GAMMA_TABLE(LCD_DRV_IC)));
    
    offset_ptr     = (uint32*)(&(lcd_cfg_ptr->lcd_init_offset));/*lint !e662*/
    cfg_offset_ptr = (uint8*)(&(lcd_cfg_ptr[1]));
    
    len[0]  = sizeof(SEINE_LCD_INIT(LCD_DRV_IC));
    len[1]  = sizeof(SEINE_LCD_ENTERSLEEP(LCD_DRV_IC));
    len[2]  = sizeof(SEINE_LCD_SETCONTRAST(LCD_DRV_IC));
    len[3]  = sizeof(SEINE_LCD_SETBRIGHTNESS(LCD_DRV_IC));
    len[4]  = sizeof(SEINE_LCD_SETWINDOW(LCD_DRV_IC));
    len[5]  = sizeof(SEINE_LCD_GETINFO(LCD_DRV_IC));
    len[6]  = sizeof(SEINE_LCD_SETFRAMERATE(LCD_DRV_IC));
    len[7]  = sizeof(SEINE_LCD_CLEAR(LCD_DRV_IC));
    len[8]  = sizeof(SEINE_LCD_CLOSE(LCD_DRV_IC));
    len[9]  = sizeof(SEINE_LCD_OPEN(LCD_DRV_IC));
    len[10] = sizeof(SEINE_LCD_SETDIRECTION(LCD_DRV_IC));
    
    cfg_addr[0] = (LCD_OP_CELL *)(SEINE_LCD_INIT(LCD_DRV_IC));                 
    cfg_addr[1] = (LCD_OP_CELL *)(SEINE_LCD_ENTERSLEEP(LCD_DRV_IC));   
    cfg_addr[2] = (LCD_OP_CELL *)(SEINE_LCD_SETCONTRAST(LCD_DRV_IC));  
    cfg_addr[3] = (LCD_OP_CELL *)(SEINE_LCD_SETBRIGHTNESS(LCD_DRV_IC));
    cfg_addr[4] = (LCD_OP_CELL *)(SEINE_LCD_SETWINDOW(LCD_DRV_IC));    
    cfg_addr[5] = (LCD_OP_CELL *)(SEINE_LCD_GETINFO(LCD_DRV_IC));      
    cfg_addr[6] = (LCD_OP_CELL *)(SEINE_LCD_SETFRAMERATE(LCD_DRV_IC)); 
    cfg_addr[7] = (LCD_OP_CELL *)(SEINE_LCD_CLEAR(LCD_DRV_IC));        
    cfg_addr[8] = (LCD_OP_CELL *)(SEINE_LCD_CLOSE(LCD_DRV_IC));        
    cfg_addr[9] = (LCD_OP_CELL *)(SEINE_LCD_OPEN(LCD_DRV_IC));         
    cfg_addr[10]= (LCD_OP_CELL *)(SEINE_LCD_SETDIRECTION(LCD_DRV_IC)); 
 
    total_len = 0;
    for(i=0; i<11; i++)/*lint !e662*/
    {   
        offset_ptr[i] = total_len;/*lint !e662 !e661*/
        
        SCI_MEMCPY(cfg_offset_ptr + total_len, cfg_addr[i], len[i]);
        
        total_len += len[i];
    }

    for(i=0; i<11; i++)
    {
        //SCI_TRACE_LOW:"INDEX %u, LEN %u"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ILI9325_SEINE_DRV_289_112_2_18_0_34_51_2059,(uint8*)"uu", i, len[i]);
    }

    //SCI_TRACE_LOW:"Total len %u, index len %u"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ILI9325_SEINE_DRV_292_112_2_18_0_34_51_2060,(uint8*)"uu", total_len, sizeof(LCD_COM_CFG));

    total_len += sizeof(LCD_COM_CFG);
    
    if(total_len > cfg_buf_len)
    {
        /*
        this assert means you should increase lcd config
        buffer lcd_com_cfg_buf which defined above, but 
        no more than 15k
        */
        SCI_ASSERT(0);/*assert verified*/
    }
 
    *cfg_pptr = (uint32*)lcd_cfg_ptr;
    *len_ptr  = (((total_len + 1) >> 1) << 1);
}



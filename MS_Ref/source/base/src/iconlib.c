#define  ICONLIB_C

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_ClrAntenna[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
    18, 1, 0x00, 0x00, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Antenna1[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0x00,0x41,0x00,0x2a,0x00,0x2a,0x00,0x41,
    0x00,0x08,0x00,0x08,0x00,0x1c,0x00,0x7f
#else
    18, 1, 0x90, 0x00, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Antenna2[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0x40,0x41,0x80,0x2a,0x00,0x2a,0x00,0x41,
    0x00,0x08,0x00,0x08,0x00,0x1c,0x00,0x7f
#else
    18, 1, 0x92, 0x00, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Antenna3[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0x50,0x41,0x90,0x2a,0x20,0x2a,0xc0,0x41,
    0x00,0x08,0x00,0x08,0x00,0x1c,0x00,0x7f
#else
    18, 1, 0x92, 0x40, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Antenna4[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0x54,0x41,0x94,0x2a,0x24,0x2a,0xc8,0x41,
    0x10,0x08,0xe0,0x08,0x00,0x1c,0x00,0x7f
#else
	18, 1, 0x92, 0x44, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Antenna5[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0x55,0x41,0x95,0x2a,0x25,0x2a,0xc9,0x41,
    0x12,0x08,0xe4,0x08,0x08,0x1c,0xf0,0x7f
#else
	18, 1, 0x92, 0x44, 0x80
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_ClrBattery[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
	8, 1, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Battery1[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0xff,0xfe,0x80,0x02,0x80,0x03,0x80,0x03,
    0x80,0x03,0x80,0x03,0x80,0x02,0xff,0xfe
#else
	8, 1, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Battery2[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0xff,0xfe,0x80,0x02,0xb0,0x03,0xb0,0x03,
    0xb0,0x03,0xb0,0x03,0x80,0x02,0xff,0xfe
#else
   8, 1, 0x80
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Battery3[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0xff,0xfe,0x80,0x02,0xb6,0x03,0xb6,0x03,
    0xb6,0x03,0xb6,0x03,0x80,0x02,0xff,0xfe
#else
    8, 1, 0x88
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Battery4[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0xff,0xfe,0x80,0x02,0xb6,0xc3,0xb6,0xc3,
    0xb6,0xc3,0xb6,0xc3,0x80,0x02,0xff,0xfe
#else
	8, 1, 0x89
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_Battery5[]=
{
#ifndef _FIXED_ICON
    0x10,0x08,0xff,0xfe,0x80,0x02,0xb6,0xdb,0xb6,0xdb,
    0xb6,0xdb,0xb6,0xdb,0x80,0x02,0xff,0xfe
#else
	8, 1, 0x8E
#endif    
};

/*************************************************************/
/*      The format of following icon array is 12X8           */
/*************************************************************/
const unsigned char lcd_ClrMessage[]=
{
#ifndef _FIXED_ICON
    0x0c,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
    1, 1, 0x00
#endif
};

/*************************************************************/
/*      The format of following icon array is 12X8           */
/*************************************************************/
const unsigned char lcd_IconMessage[]=
{
#ifndef _FIXED_ICON
    0x0c,0x08,0xff,0xf0,0x80,0x10,0x80,0x10,0xe0,0x70,
    0x99,0x90,0x86,0x10,0x80,0x10,0xff,0xf0
#else
    1, 1, 0x80
#endif
};


/*************************************************************/
/*      The format of following icon array is 8X8          */
/*************************************************************/
const unsigned char lcd_ClrRing[]=
{
#ifndef _FIXED_ICON
    0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
    1, 1, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 8X8           */
/*************************************************************/
const unsigned char lcd_IconRing[]=
{
#ifndef _FIXED_ICON
    0x08,0x08,0x0d,0x12,0xe1,0x41,0x22,0x74,0x2c,0x04
#else
    1, 1, 0x80
#endif    
};

/*************************************************************/
/*      The format of following icon array is 8X8          */
/*************************************************************/
const unsigned char lcd_IconNoRing[]=
{
#ifndef _FIXED_ICON
    0x08,0x08,0x66,0x99,0x00,0x66,0x99,0x00,0x66,0x99
#else
    1, 1, 0x0
#endif    
};


/*************************************************************/
/*      The format of following icon array is 13X8          */
/*************************************************************/
const unsigned char lcd_ClrPhoneMem[]=
{
   0x0d,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/*************************************************************/
/*      The format of following icon array is 13X8          */
/*************************************************************/
const unsigned char lcd_IconPhoneMem[]=
{
   0x0d,0x08,0x38,0xe0,0xe5,0x00,0xa2,0x10,0xa2,0x28,
   0xa2,0x08,0xba,0x90,0x87,0x20,0xff,0xb8
};


/*************************************************************/
/*      The format of following icon array is 10X8          */
/*************************************************************/
const unsigned char lcd_ClrRoaming[]=
{
#ifndef _FIXED_ICON
    0x0a,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#else
    1, 1, 0x00
#endif    
};

/*************************************************************/
/*      The format of following icon array is 16X8          */
/*************************************************************/
const unsigned char lcd_IconRoaming[]=
{
#ifndef _FIXED_ICON
    0x0a,0x08,0x09,0xc0,0x1c,0xc0,0x3d,0x40,0x7a,0x00,
    0xc5,0x80,0x49,0x00,0x41,0x00,0x7f,0x00
#else
    1, 1, 0x80
#endif    
};


/*************************************************************/
/*      The format of following icon array is 13X8          */
/*************************************************************/
const unsigned char lcd_ClrSimMem[]=
{
   0x0d,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/*************************************************************/
/*      The format of following icon array is 13X8          */
/*************************************************************/
const unsigned char lcd_IconSimMem[]=
{
   0x0d,0x08,0x38,0xe0,0xe5,0x00,0xa2,0x10,0xa2,0x30,
   0xa2,0x10,0xba,0x90,0x87,0x10,0xff,0xb8
};

/*************************End Of File*************************/


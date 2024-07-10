/******************************************************************************
 ** File Name:     flash_cfg.c                                                *
 ** Description:                                                              *
 **        norflash configuration                                             *
 ** Author:         Younger.yang                                              *
 ** DATE:           11/23/2006                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/23/2006      Younger.yang       Create for Jiuyang                     *
 ******************************************************************************/

#include "flash_cfg.h"
#include "fdl_main.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif



/**---------------------------------------------------------------------------*
 **                         Macro defines
 **---------------------------------------------------------------------------*/
#define FLASH_START_ADDR     g_flash_start_addr


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern  uint32  g_flash_start_addr;

/**---------------------------------------------------------------------------*
 **                         Functions
 **---------------------------------------------------------------------------*/


// 64k * 31 + 32K + 8K + 8K + 16K
static uint32 S29AL016D_Top_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (addr < 0x1F0000)
    {
        return 64 * 1024;
    }
    else if (addr < (0x1F0000 + 32 * 1024))
    {
        return 32 * 1024;
    }
    else if (addr < (0x1F0000 + 32 * 1024 + 8*1024 * 2))
    {
        return 8 * 1024;
    }
    else
    {
        return 16 * 1024;
    }
}

// S29AL016D (Bottom boot block) : 16K + 8K + 8K + 32K + 64K * 31
static uint32 S29AL016D_Bottom_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (addr >= 0x10000)
    {
        return 64 * 1024;
    }
    else if (addr < (16 * 1024))
    {
        return 16 * 1024;
    }
    else if (addr < (16 * 1024 + 8*1024 * 2))
    {
        return 8 * 1024;
    }
    else
    {
        return 32 * 1024;
    }
}



static uint32 AMD32_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0x3F0000)
        {
            return 8 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x10000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}



static uint32 AMD64_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0x7F0000)
        {
            return 8 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x10000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}


static uint32 Samsung64_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0x7F0000)
        {
            return 8 * 1024;
        }
    }
    else
    {
        if (addr <  0x10000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}


static uint32 AMD128_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0xFF0000)
        {
            return 8 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x10000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}

static uint32 AMD256_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0x1FE0000)
        {
            return 32 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x20000)
        {
            return 32 * 1024;
        }
    }

    return 128 * 1024;
}


static uint32 PL127N_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0xFC0000)
        {
            return 64 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x40000)
        {
            return 64 * 1024;
        }
    }

    return (256 * 1024);
}


//U can realize the following function by yourself.
static uint32 AMD128_DualCS_GetSectorSize (uint32 bootflag, uint32 addr)
{
    return 0;
}


static uint32 SST_GetSectorSize (uint32 bootflag, uint32 addr)
{

    addr -= FLASH_START_ADDR;

    if (addr < 0x3F0000)
    {
        return (64 * 1024);
    }

    else if (addr < 0x3FC000)
    {
        return (48 * 1024);
    }
    else
    {
        return (8 * 1024);
    }
}


static uint32 AMD128_GetS71GLSectorSize (uint32 bootflag, uint32 addr)
{
    return (128 * 1024);
}


//@David.Jia 2006.3.13  cr43532 begin
//Deal with address from 0x00000000 to 0x007FFFFF and from 0x54000000 to 0x547FFFFF
//For each one, first 64k is 8k sector size, all other is 64k sector size.
static uint32 AMD64_64_DualCS_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr &= 0xFFFFFF;

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x10000)
        {
            return 8 * 1024;
        }
    }
    else
    {
        if (addr >= 0x7F0000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}
//@David.Jia 2006.3.13  cr43532 end



static uint32 INTEL128MLC_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0xFE0000)
        {
            return 32 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x20000)
        {
            return 32 * 1024;
        }
    }

    return 128 * 1024;
}
/*start added by victorxu*/
/*start added by feng tao*/
static uint32 Numonyx128_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;
    return 256* 1024;
}

static uint32 Numonyx256_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;
    return 256 * 1024;
}

static uint32 Numonyx512_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;
    return 256 * 1024;
}
/*end added by feng tao*/
/*end added by victorxu*/

static uint32 INTEL128_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0xFF0000)
        {
            return 8 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x10000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}


static uint32 INTEL64_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0x7E0000)
        {
            return 8 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x10000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}


static uint32 INTEL32_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0x3F0000)
        {
            return 8 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x8000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}

static uint32 S29ws128p_Bottom_Top_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (addr < 0x20000)
    {
        return 32 * 1024;
    }
    else if (addr >= 0xfe0000)
    {
        return 32 * 1024;
    }

    else
    {
        return 128 * 1024;
    }
}

static uint32 TSB128_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0xfe0000)
        {
            return 16 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x20000)
        {
            return 16 * 1024;
        }
    }

    return 128 * 1024;
}
static uint32 Numonyx256_L18_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0xFF0000)
        {
            return 32 * 1024;  //16 k word
        }
        else
        {
            return 128 * 1024; //64 k word
        }
    }

    return 128 * 1024;
}

static uint32 CFEON128_GetSectorSize (uint32 bootflag, uint32 addr)
{
    return 128 * 1024;
}

static uint32 Intel_L18_GetSectorSize (uint32 bootflag,uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0xfe0000)
        {
            return 32 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x20000)
        {
            return 32 * 1024;
        }
    }

    return 128 * 1024;
}
static uint32 TSB64_GetSectorSize (uint32 bootflag, uint32 addr)
{
    addr -= FLASH_START_ADDR;

    if (TOP_BOOT_FLAG & bootflag)
    {
        if (addr >= 0x7F0000)
        {
            return 8 * 1024;
        }
    }

    if (BOTTOM_BOOT_FLAG & bootflag)
    {
        if (addr < 0x10000)
        {
            return 8 * 1024;
        }
    }

    return 64 * 1024;
}


//All kinds of nor flash configuration defined here.
const NORFLASH_INFO_T  g_norflash_info[] =
{
    { FLASH_AMD_ID,      0x7E4400,    0,   64,  BOTTOM_BOOT_FLAG | TOP_BOOT_FLAG,   S29ws128p_Bottom_Top_GetSectorSize},  //SPANSION burst mem s71ws128pc0hf3sr @add by hyy
    { FLASH_TSB_ID,      0x0049,     0,   0,   TOP_BOOT_FLAG,                     AMD128_GetSectorSize}, //TOSIBA FLASH
    { FLASH_AMD_ID,      0x7E2000,  1,   64,  BOTTOM_BOOT_FLAG | TOP_BOOT_FLAG,  PL127N_GetSectorSize},  //PL127N
    { FLASH_AMD_ID,      0x7E2000,  0,   0,   BOTTOM_BOOT_FLAG | TOP_BOOT_FLAG,  AMD128_GetSectorSize},   // Am29PDL127.   16M + 48M + 48M + 16M
    { FLASH_AMD_ID,      0x7E2100,  0,   0,   BOTTOM_BOOT_FLAG | TOP_BOOT_FLAG,  AMD128_DualCS_GetSectorSize}, // Am29PDL129
    { FLASH_AMD_ID,      0x7E0201,  0,   0,   BOTTOM_BOOT_FLAG | TOP_BOOT_FLAG,  AMD64_GetSectorSize},  // Am29DL640G.  8M + 24M + 24M + 8M
    { FLASH_AMD_ID,      0x7E1A01,  0,   32,  TOP_BOOT_FLAG,                     AMD32_GetSectorSize},  //S71GL032A
    { FLASH_AMD_ID,      0x7E1000,  0,   32,  BOTTOM_BOOT_FLAG,                  AMD64_GetSectorSize},  //S71GL064A
    { FLASH_AMD_ID,      0x7E1001,  0,   32,  TOP_BOOT_FLAG,                     AMD64_GetSectorSize},  //S71GL064A
    { FLASH_AMD_ID,      0x7E2101,  0,   32,  0,                                 AMD128_GetS71GLSectorSize},  //71GL128NB0
    { FLASH_AMD_ID,      0x7E0A01,  0,   0,   BOTTOM_BOOT_FLAG | TOP_BOOT_FLAG,  AMD32_GetSectorSize},  //MB84VD22181FM(T), four bank: 4M + 12M + 12M + 4M
    { FLASH_AMD_ID,      0x7E0A00,  0,   0,   BOTTOM_BOOT_FLAG,                  AMD32_GetSectorSize},  //MB84VD22191FM(B), four bank: 4M + 12M + 12M + 4M

    { FLASH_AMD_ID,      0x55,      0,   0,   TOP_BOOT_FLAG,                     AMD32_GetSectorSize},  //Am29DL322(T)
    { FLASH_AMD_ID,      0x50,      0,   0,   TOP_BOOT_FLAG,                     AMD32_GetSectorSize},  //Am29DL323(T)
    { FLASH_AMD_ID,      0x5C,      0,   0,   TOP_BOOT_FLAG,                     AMD32_GetSectorSize},  //Am29DL324(T)
    { FLASH_AMD_ID,      0x56,      0,   0,   BOTTOM_BOOT_FLAG,                  AMD32_GetSectorSize},  //Am29DL322(B)
    { FLASH_AMD_ID,      0x53,      0,   0,   BOTTOM_BOOT_FLAG,                  AMD32_GetSectorSize},  //Am29DL323(B)
    { FLASH_AMD_ID,      0x5F,      0,   0,   BOTTOM_BOOT_FLAG,                  AMD32_GetSectorSize},  //Am29DL324(B)
    { FLASH_AMD_ID,      0xC4,      0,   0,   TOP_BOOT_FLAG,                     S29AL016D_Top_GetSectorSize},  //S29AL016D
    { FLASH_AMD_ID,      0x49,      0,   0,   BOTTOM_BOOT_FLAG,                  S29AL016D_Bottom_GetSectorSize},  //S29AL016D
    { FLASH_AMD_ID,      0x57,      0,   0,   TOP_BOOT_FLAG,                     AMD64_GetSectorSize},  //TH50VPF5682CDSB
    { FLASH_AMD_ID,      0xE0,      0,   0,   TOP_BOOT_FLAG,                     Samsung64_GetSectorSize},
    { FLASH_AMD_ID,      0xE2,      0,   0,   BOTTOM_BOOT_FLAG,                  Samsung64_GetSectorSize},

    { FLASH_SAMSUNG_ID,  0x2206,    0,   0,   TOP_BOOT_FLAG,                     AMD256_GetSectorSize},  //K5L5666CAM  burst mem@add by yong.li
    { FLASH_SAMSUNG_ID,  0x2208,    0,   64,   TOP_BOOT_FLAG,                     AMD256_GetSectorSize},  //K5N5629ATB  burst mem@add by yong.li
    { FLASH_SAMSUNG_ID,  0x22E0,    0,   0,   TOP_BOOT_FLAG,                     AMD64_64_DualCS_GetSectorSize},
    { FLASH_SAMSUNG_ID,  0x22E2,    0,   0,   BOTTOM_BOOT_FLAG,                  AMD64_64_DualCS_GetSectorSize},
    { FLASH_SAMSUNG_ID,  0x257E,    1,   0,BOTTOM_BOOT_FLAG | TOP_BOOT_FLAG,  AMD64_64_DualCS_GetSectorSize}, //K5 L 63 31 C A A
    { FLASH_SAMSUNG_ID,  0x257E,    0,   0,   BOTTOM_BOOT_FLAG | TOP_BOOT_FLAG,  AMD128_GetSectorSize}, //K5L2731CAM-D770

    { FLASH_SAMSUNG_ID,  0x2402,      0,   0,  TOP_BOOT_FLAG,                 AMD128_GetSectorSize},  //K5L2833ATA-AF66 ATB-AQ11 2866-AF66 2866-AQ11 T burst mem@add by hyy
    { FLASH_SAMSUNG_ID,  0x2403,      0,   0,  BOTTOM_BOOT_FLAG,              AMD128_GetSectorSize},  //K5L2833ATA-AF66 ATB-AQ11 2866-AF66 2866-AQ11 B burst mem@add by hyy
    { FLASH_SAMSUNG_ID,  0x2404,      0,   0,  TOP_BOOT_FLAG,                 AMD128_GetSectorSize},  //K5N2833ATM-AQ11 AF66  T burst mem@add by hyy
    { FLASH_SAMSUNG_ID,  0x2404,	  1,   0,  TOP_BOOT_FLAG,			AMD128_GetSectorSize},  //K5N2866ATE-BQ12  T burst mem@add by yong.li
    { FLASH_SAMSUNG_ID,  0x2405,      0,   0,  BOTTOM_BOOT_FLAG,              AMD128_GetSectorSize},  //K5N2833ATM-AQ11 AF66  B burst mem@add by hyy


    { FLASH_INTEL_ID,    0x880C,    0,   64,  TOP_BOOT_FLAG,                     INTEL128MLC_GetSectorSize}, //MLC L18
    { FLASH_INTEL_ID,    0x880F,    0,   64,  BOTTOM_BOOT_FLAG,                  INTEL128MLC_GetSectorSize}, //MLC L18
    { FLASH_INTEL_ID,    0x8812,    0,   64,  TOP_BOOT_FLAG,                     INTEL128MLC_GetSectorSize}, //MLC L30
    { FLASH_INTEL_ID,    0x8813,    0,   64,  BOTTOM_BOOT_FLAG,                  INTEL128MLC_GetSectorSize}, //MLC L30
    { FLASH_INTEL_ID,    0x8815,    0,   64,  BOTTOM_BOOT_FLAG,                  INTEL128MLC_GetSectorSize}, //MLC L30
    { FLASH_INTEL_ID,    0x88C5,    0,   64,  BOTTOM_BOOT_FLAG,                  INTEL128MLC_GetSectorSize},
    { FLASH_INTEL_ID,    0x8852,    0,   0,   TOP_BOOT_FLAG,                     INTEL32_GetSectorSize}, //SLC W30
    { FLASH_INTEL_ID,    0x8853,    0,   0,   BOTTOM_BOOT_FLAG,                  INTEL32_GetSectorSize}, //SLC W30
    { FLASH_INTEL_ID,    0x8854,    0,   0,   TOP_BOOT_FLAG,                     INTEL64_GetSectorSize},
    { FLASH_INTEL_ID,    0x8855,    0,   0,   BOTTOM_BOOT_FLAG,                  INTEL64_GetSectorSize},
    { FLASH_INTEL_ID,    0x8856,    0,   0,   TOP_BOOT_FLAG,                     INTEL128_GetSectorSize},
    { FLASH_INTEL_ID,    0x8857,    0,   0,   BOTTOM_BOOT_FLAG,                  INTEL128_GetSectorSize},
    { FLASH_INTEL_ID,    0x8862,    0,   0,   TOP_BOOT_FLAG,                     INTEL32_GetSectorSize}, //SLC W18
    { FLASH_INTEL_ID,    0x8863,    0,   0,   BOTTOM_BOOT_FLAG,                  INTEL32_GetSectorSize}, //SLC W18
    { FLASH_INTEL_ID,    0x8864,    0,   0,   TOP_BOOT_FLAG,                     INTEL64_GetSectorSize},
    { FLASH_INTEL_ID,    0x8865,    0,   0,   BOTTOM_BOOT_FLAG,                  INTEL64_GetSectorSize},
    { FLASH_INTEL_ID,    0x8866,    0,   0,   TOP_BOOT_FLAG,                     INTEL128_GetSectorSize},
    { FLASH_INTEL_ID,    0x8867,    0,   0,   BOTTOM_BOOT_FLAG,                  INTEL128_GetSectorSize},

    { FLASH_INTEL_ID,    0x8811,    0,   0,   BOTTOM_BOOT_FLAG,                  INTEL64_GetSectorSize},
    { FLASH_INTEL_ID,    0x8835,    0,   64,   BOTTOM_BOOT_FLAG,                   Intel_L18_GetSectorSize},//L18 ADMUX
    { FLASH_INTEL_ID,    0x880A,    0,   64,   TOP_BOOT_FLAG,            Numonyx256_L18_GetSectorSize}, //
    { FLASH_INTEL_ID,    0x8836,    0,   64,   BOTTOM_BOOT_FLAG,            Numonyx256_L18_GetSectorSize}, //

    { FLASH_INTEL_ID,    0x8809,    0,   64,   TOP_BOOT_FLAG,                   Intel_L18_GetSectorSize},//L18 ADMUX

    /*start added by victorxu*/
    { FLASH_INTEL_ID,    0x8903,    0,   1024,   0,                                Numonyx128_GetSectorSize}, //MLC M18 128M
    { FLASH_INTEL_ID,    0x8900,    0,   1024,   0,                                Numonyx128_GetSectorSize}, //hanjun.liu
    /*end added by victorxu*/
    { FLASH_SST_ID,      0x7351,    0,   0,   TOP_BOOT_FLAG,                     SST_GetSectorSize },
    { FLASH_SST_ID,      0x7353,    0,   0,   TOP_BOOT_FLAG,                     SST_GetSectorSize },

    { FLASH_ST_ID,       0x88C4,    0,   64,  TOP_BOOT_FLAG,                     INTEL128MLC_GetSectorSize}, //nymonyx adp burst mem m36l0r7050T4zaQ @add by hyy
    { FLASH_ST_ID,       0x88C4,    1,   64,  TOP_BOOT_FLAG,                     INTEL128MLC_GetSectorSize}, //nymonyx adp burst mem m36l0r7050T4zaQ @add by hyy
    { FLASH_ST_ID,       0x88C5,    0,   64,  BOTTOM_BOOT_FLAG,               INTEL128MLC_GetSectorSize}, //nymonyx adp burst mem m36l0r7050B4zaQ @add by hyy
    { FLASH_ST_ID,       0x88C5,    1,   64,  BOTTOM_BOOT_FLAG,               INTEL128MLC_GetSectorSize}, //nymonyx adp burst mem m36l0r7050B4zaQ @add by hyy

    { FLASH_ST_ID,       0x8810,    0,   0,   TOP_BOOT_FLAG,                     INTEL64_GetSectorSize},  //ST WR18
    { FLASH_ST_ID,       0x883C,    0,   0,   TOP_BOOT_FLAG,                      INTEL128MLC_GetSectorSize},     //ST WR18

    { FLASH_ST_ID,       0x882E,    0,   64,  TOP_BOOT_FLAG,                      INTEL128MLC_GetSectorSize}, //nymonyx adm burst mem m36l0r7050u3zam @add by hyy
    { FLASH_ST_ID,       0x882F,    0,   64,  BOTTOM_BOOT_FLAG,                INTEL128MLC_GetSectorSize},//nymonyx adm burst mem m36l0r7050l3zam @add by hyy

    { FLASH_TSB_ID,      0x0003,   0,   0,   TOP_BOOT_FLAG,                    TSB128_GetSectorSize},
    { FLASH_TSB_ID,      0x0096,   0,   0,   TOP_BOOT_FLAG,                    TSB64_GetSectorSize},
    { FLASH_TSB_ID,      0x0097,   0,   0,   BOTTOM_BOOT_FLAG,              TSB64_GetSectorSize},
    { FLASH_CFEON_ID,    0x227E,    0,   64,  TOP_BOOT_FLAG,                        CFEON128_GetSectorSize},
    { FLASH_CFEON_ID,    0x7E4300,    0,   64,  TOP_BOOT_FLAG,                        CFEON128_GetSectorSize},

    { NULL,              NULL,      NULL,NULL,NULL,                              NULL}

};


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/******************************************************************************
 ** File Name:     mem_test.c                                                *
 ** Author:        guangqiao.she                                                  *
 ** DATE:           10/15/2022                                                *
 ** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/15/2022        guangqiao.she          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mem_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define TEST_PATTERN  0x5A5A5A5A
#define TEST_PATTERN_2  0xA5A5A5A5
#if  defined(MEM_STRESS_TEST_NORFLASH)
#define FLASH_BLOCK_SIZE   128*1024
#endif
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                          Variables                                *
 **---------------------------------------------------------------------------*/
uint32 total_test_cnt=0;
uint32 pattern1_failure_cnt=0;
uint32 pattern2_failure_cnt=0;

#if  defined(MEM_STRESS_TEST_NORFLASH)
uint32  test_flash_block[FLASH_BLOCK_SIZE>>2]; 
uint32  test_flash_block2[FLASH_BLOCK_SIZE>>2]; 
#endif
/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
#if  defined(MEM_STRESS_TEST_NORFLASH)
extern uint32 FDL_FlashWriteArea (uint32 addr, char *buf, uint32 size);
extern uint32 FDL_FlashErase(uint32 start_addr,  uint32 size);
extern uint32 FDL_FlashCheckSectorSize (uint32 start_addr);
#endif
extern void rprintf(const char *fmt, ...);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/


#if defined(MEM_STRESS_TEST_PSRAM) || defined(MEM_STRESS_TEST_NORFLASH)

void test_write_pattern(uint32 addr, uint32 len, uint32 pattern)
{
    uint32 i=0, pattern_data = 0;
    
    //write pattern to addr
    for(i = 0; i < len; i += 4)
    {
        *((uint32*)(addr + i)) = pattern;
    }
    return;
}

int _test_compare_pattern(uint32 addr, uint32 len, uint32 pattern)
{
    uint32 i=0, pattern_data = 0;
         uint32 err_cnt = 0;
        //compare
    for(i = 0; i < len; i += 4)
    {
        if(pattern != *((uint32*)(addr + i)))
        {
            rprintf("test 0x%x failure, data 0x%x != pattern 0x%x\n",addr + i,*((uint32*)(addr + i)),pattern);
            err_cnt++;
        }
    }
    
    if(err_cnt)
    {
        rprintf("%d counter data error\n",err_cnt);
        return 0;
    }
    else
    {
        rprintf("test OK \n");
        return 1;
    }

}

/*
//  first write "pattern" from "addr" to "addr+len"
//  read data from "addr" to "addr+len", and compare with "pattern"
*/
int  test_compare_pattern(uint32 addr, uint32 len, uint32 pattern)
{
    test_write_pattern(addr, len, pattern);

    return _test_compare_pattern(addr, len, pattern);

}
#endif

#if defined(MEM_STRESS_TEST_PSRAM) 
void  psram_test(void)
{
    rprintf("psram_test start \n");
    total_test_cnt = 0;
    while(1)
    {
        total_test_cnt++;
        rprintf("test from 0x%x to 0x%x \n",PHY_RAM_BASE_ADDR,(PHY_RAM_BASE_ADDR+MEM_RAM_SIZE));
        rprintf("test pattern1 0x%x \n",TEST_PATTERN);
        if(!test_compare_pattern(PHY_RAM_BASE_ADDR,MEM_RAM_SIZE,TEST_PATTERN))
            pattern1_failure_cnt++;

        rprintf("psram_test : test %d, failure %d(p1 fail %d, p2 fail %d) \n\n",  \
            total_test_cnt,(pattern1_failure_cnt+pattern2_failure_cnt),pattern1_failure_cnt,pattern2_failure_cnt);

        total_test_cnt++;
        rprintf("test pattern2 0x%x \n",TEST_PATTERN_2);
        if(!test_compare_pattern(PHY_RAM_BASE_ADDR,MEM_RAM_SIZE,TEST_PATTERN_2))
            pattern2_failure_cnt++;

        rprintf("psram_test : test %d, failure %d(p1 fail %d, p2 fail %d) \n\n",  \
            total_test_cnt,(pattern1_failure_cnt+pattern2_failure_cnt),pattern1_failure_cnt,pattern2_failure_cnt);
    }
} 
#endif


#if  defined(MEM_STRESS_TEST_NORFLASH)

void  norflash_test(uint32 start, uint32 end)
{
    uint32 size  =0, i =0;
    uint32 test_pattern = 0; 
    uint32 flash_block_size = 0, block_size = 0;
    rprintf("norflash_test start \n");
    
    flash_block_size =FDL_FlashCheckSectorSize(start);
    total_test_cnt = 0;
    size = end - start;


    block_size = (flash_block_size > FLASH_BLOCK_SIZE ? FLASH_BLOCK_SIZE:flash_block_size);

    rprintf("flash_block_size 0x%x, block size 0x%x \n",flash_block_size,block_size);

    //fill block area with TEST_PATTERN
    for(i = 0; i <= (block_size >>2); i++)
    {
        test_flash_block[i] = TEST_PATTERN;
    }

    //fill block area with TEST_PATTERN_2
    for(i = 0; i <= (block_size >>2); i++)
    {
        test_flash_block2[i] = TEST_PATTERN_2;
    }
    
    while(1)
    {
        if(flash_block_size > FLASH_BLOCK_SIZE)
        {
           rprintf("flash_block_size(0x%x) > FLASH_BLOCK_SIZE(0x%x), please enlarge FLASH_BLOCK_SIZE \n",flash_block_size,FLASH_BLOCK_SIZE);
        }
        
        total_test_cnt++;
        rprintf("test from 0x%x to 0x%x \n",start,end);
        test_pattern = (TEST_PATTERN) ;
        rprintf("test pattern1 0x%x \n",test_pattern);

        rprintf("erase flash: start 0x%x, size 0x%x\n", start, size);
        FDL_FlashErase (start, size);
        
        rprintf("write flash with 0x%x \n",test_pattern);
        for(i = 0; i <= size; i += block_size)
        {
            FDL_FlashWriteArea(start+i, (char*)&test_flash_block,block_size);
        }
        rprintf("read and compare \n"); 
        if(!_test_compare_pattern(start,size,test_pattern))
            pattern1_failure_cnt++;

        rprintf("norflash_test : test %d, failure %d(p1 fail %d, p2 fail %d) \n\n",  \
            total_test_cnt,(pattern1_failure_cnt+pattern2_failure_cnt),pattern1_failure_cnt,pattern2_failure_cnt);

        total_test_cnt++;
        rprintf("test from 0x%x to 0x%x \n",start,end);
        test_pattern = (TEST_PATTERN_2) ;
        rprintf("test pattern2 0x%x \n",test_pattern);

        rprintf("erase flash: start 0x%x, size 0x%x\n", start, size);
        FDL_FlashErase (start, size);
        
        rprintf("write flash with 0x%x \n",test_pattern);
        for(i = 0; i <= size; i += block_size)
        {
            FDL_FlashWriteArea(start+i, (char*)&test_flash_block2,block_size);
        }
        
        rprintf("read and compare \n");     
        if(!_test_compare_pattern(start,size,test_pattern))
            pattern2_failure_cnt++;

        rprintf("norflash_test : test %d, failure %d(p1 fail %d, p2 fail %d) \n\n",  \
            total_test_cnt,(pattern1_failure_cnt+pattern2_failure_cnt),pattern1_failure_cnt,pattern2_failure_cnt);
    }
} 

#endif



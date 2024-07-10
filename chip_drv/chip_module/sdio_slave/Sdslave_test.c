/******************************************************************************
 ** File Name:      Sdio_master_test.c                                               *
 ** Author:         Andy.Chen                                                 *
 ** DATE:           21/02/2010                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    define trace interface just for testing usage             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 21/02/2010     Andy.Chen          Create                                  *
 ******************************************************************************/
#include "sci_api.h"
#include "sdslave_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define MAX_BLOCK_NUM   101 // 512// 
#define MAX_BLOCK_SIZE  512
#define MAX_BUF_SIZE    (MAX_BLOCK_NUM*MAX_BLOCK_SIZE)
#define TEST_MAX_SLOT_NO    1

/**---------------------------------------------------------------------------*
 **                          Variables                                        *
 **---------------------------------------------------------------------------*/
LOCAL __align (32) uint32 s_src_buf[MAX_BUF_SIZE/4];
LOCAL __align (32) uint32 s_des_buf[MAX_BUF_SIZE/4];
PUBLIC uint8 *s_des_buf1 = s_des_buf;//(uint8*)0xb00000;//(uint8 *)0x33fc002;//(uint8*)0xa00000;//
LOCAL uint8 *s_src_buf1 = s_src_buf;//(uint8*)0xa00000;//(uint8*)0x40005000;//(uint8*)0xa05000;
LOCAL uint32 p_buf[1024] = {0};

LOCAL SDSLAVE_HAL_HANDLE s_cardHandle[TEST_MAX_SLOT_NO] = {PNULL};

PUBLIC uint32 test_sdio_dma_type = 0;
/**---------------------------------------------------------------------------*
 **                     Public Function Prototypes                            *
 **---------------------------------------------------------------------------*/
PUBLIC uint32 sdslave_test_entry (void)
{
    uint32 total_size = 0, blk_cnt = 0, blk_len = 0;
    volatile uint32 event, dma_flag;
    uint32 i,num;
    SDSLAVE_HAL_HANDLE cardHandle;
    extern uint32 card_int_flag;

    // Step1: Open
    cardHandle = SDSLAVE_HAL_Open();

    if (NULL == cardHandle)
    {
        SCI_ASSERT (0); /*assert to do*/

        while (1);
    }

    while (1)
    {
#include <string.h>

        char  *str_load               = (char *) s_des_buf1;
        static char str_card_int[50]  = "test card int";
        static char str_hst_start[50] = "host ask slave start buff-port test";
        static char str_hst_end[50]   = "host ask slave end buff-port test";
        static char str_slv_ready[50] = "slave ready to test buff-port";
        static char str_slv_end[50]   = "slave end buff-port test";

        while (1)
        {
            SCI_MEMSET (s_des_buf, 0, 512);

            if (TRUE != SDSLAVE_HAL_Read (cardHandle, s_des_buf, 16))
            {
                SCI_ASSERT (0); /*assert verified*/
            }

            if (TRUE != SDSLAVE_HAL_Write (cardHandle, s_des_buf, 16))
            {
                SCI_ASSERT (0); /*assert verified*/
            }
        }
    }
}


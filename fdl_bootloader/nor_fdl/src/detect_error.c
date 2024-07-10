/******************************************************************************
 ** File Name:      BB_main.c                                                 *
 ** Author:         Leo.Feng                                                  *
 ** DATE:           27/06/2002                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 27/06/2002     Leo.Feng         Create.                                   *
 ** 11/12/2003     Zhemin.Lin       Enhanced according to cr6198              *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

#define SAMPLE_SIZE (1)

#define SAMPLE_POOL_SIZE    (1024*32/SAMPLE_SIZE)
/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/

uint32  sample_value_pool[SAMPLE_POOL_SIZE],sample_count =0;

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

void    record_samples (uint32 base_address,uint32 write_address,uint32 data_len,char *data)
{
    uint32 step = 1024*SAMPLE_SIZE;
    uint32 i;
    uint32 curren_sample_count;

    if (data_len %step)
    {
        return;
    }

    curren_sample_count = (data_len/step);

    if (sample_count + curren_sample_count >= SAMPLE_POOL_SIZE)
    {
        return;
    }

    for (i=0; i<curren_sample_count; i++)
    {
        sample_value_pool[ (write_address-base_address) /step] = * (uint32 *) data;
        write_address+= step;
        data +=step;
    }

    sample_count +=curren_sample_count;
}
uint8   check_complete_sector (uint32 base_address,uint32 write_address,uint32 sector_size)
{
    uint32 step = 1024*SAMPLE_SIZE;
    uint32 offset;
    uint32 flash_value32;
    uint32 index=0;

    offset = 0;

    while (offset < sector_size)
    {
        flash_value32 = * (uint32 *) (write_address);
        index = (write_address-base_address) /step;

        if (index >= sample_count)
        {
            break;
        }

        if (flash_value32 != sample_value_pool[index])
        {
            return 0;
        }

        write_address+= step;
        offset += step;
    }

    return 1;
}

uint8   check_one_patition (uint32 base_address,uint32 write_address)
{
    uint32 step = 1024*SAMPLE_SIZE;
    uint32 i=0;
    uint32 offset=0;
    uint32 flash_value32;
    uint32 index=0;

    while (i < sample_count)
    {
        flash_value32 = * (uint32 *) (write_address);
        index = (write_address-base_address) /step;

        if (index >= sample_count)
        {
            break;
        }

        if (flash_value32 != sample_value_pool[index])
        {
            sample_count = 0;
            return 0;
        }

        write_address+= step;
        i++;

    }

    sample_count = 0;
    return 1;
}


PUBLIC void SCI_Assert_NoDebug (void)
{
   while(1);
}


/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/27/2016                      Create.                                   *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#if 1
#include "spzip_api.h"
#include "efs_comm.h"
#include "nv_type.h"
#include "load_modem.h"
#include "lzma_reg_8910.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


#define _CHECK_CRC_


uint8  load_use_lzma(void)
{
#ifdef USE_LZMA_UZIP_BIN
    return 1;
#else
    return 0;
#endif
};


#ifdef USE_LZMA_UZIP_BIN   ////loading modem cp.bin  by lzma

uint32 g_lzma_count;

#define _USE_MALLOC_BUFF_

#ifdef _USE_MALLOC_BUFF_
uint32 lzma_get_block_cnt(LZMA_INPUT_T * in)
{
    uint32 ptr_b;
    uint32 date[4];
    uint32 count,setp;
    g_lzma_count = 0;
    mon_Event(0x10ad8001);
    mon_Event((uint32)in->zip_buf_addr);
    ptr_b =  (uint32)in->zip_buf_addr;
    mon_Event(ptr_b);
    memcpy((void*)date,(const void*)ptr_b,12);
    while(date[0]!= 0xffffffff)
    {

        setp =date[2];
        setp += 20;
        ptr_b = ptr_b +setp;
        g_lzma_count++;
        //mon_Event(ptr_b);
        memcpy((void*)date,(const void*)ptr_b,12);
        // mon_Event(date[0]);


    }
    SCI_TraceLow("[INFO] lzma :  lzma_get_block_cnt  %d  \n",g_lzma_count);
    mon_Event(0x10ad8013);
    mon_Event(g_lzma_count);
    return g_lzma_count;
}

uint32 *block_info;
uint32 *dict_info;
uint32 *stream_info;
uint32 *in_crc_info;
uint32 *out_crc_info;
void lzma_init_info_array(LZMA_INPUT_T * in)
{
    uint32 blckcount;
    uint32 ptr_b;
    uint32 date[4];
    ptr_b =  (uint32)in->zip_buf_addr;
    for(blckcount = 0; blckcount<g_lzma_count; blckcount++)
    {
        memcpy((void*)date,(const void*)ptr_b,12);
        block_info[blckcount]=date[0];
        ptr_b+=4;
        dict_info[blckcount]=date[1];
        ptr_b+=4;
        stream_info[blckcount]=date[2];
        ptr_b+=4;
        ptr_b+=stream_info[blckcount];
        //mon_Event(0x10ad8a13); mon_Event(ptr_b); mon_Event(stream_info[blckcount]);
        memcpy((void*)date,(const void*)ptr_b,8);
#ifdef _CHECK_CRC_
        in_crc_info[blckcount]=date[0];
#endif
        ptr_b+=4;
#ifdef _CHECK_CRC_
        out_crc_info[blckcount]=date[1];
#endif
        ptr_b+=4;
    }
    mon_Event(0x10ad8fff);
}

uint8* src_buff;
uint8* dst_buff;
uint8* uzip_ptr;

void get_uzip_buff(LZMA_INPUT_T * in)
{
    uint8 * ptr;
    uint8 count;
    lzma_get_block_cnt(in);
    uzip_ptr =  (uint8 *)SCI_ALLOC(1024*64*2+32+g_lzma_count*5*4);
    count = (uint32)uzip_ptr%8;
    src_buff =(uint8 *) ((uint32)uzip_ptr+(8-count));
    mon_Event(0x10adfafd);
    mon_Event(src_buff);
    dst_buff = (uint8 *)((uint32)src_buff+1024*64);
    mon_Event(0x10adfaff);
    mon_Event(dst_buff);
    block_info =  (uint32 *)((uint32)dst_buff+1024*64);
    dict_info =  (uint32 *)((uint32)block_info+g_lzma_count*4);
    stream_info =  (uint32 *)((uint32)dict_info+g_lzma_count*4);
    in_crc_info =  (uint32 *)((uint32)stream_info+g_lzma_count*4);
    out_crc_info =  (uint32 *)((uint32)in_crc_info+g_lzma_count*4);
    lzma_init_info_array(in);

}
void free_uzip_buff(void)
{

    SCI_FREE(uzip_ptr);
}


#else
__align (32)  uint8 src_buff[1024*64];

__align (32)  uint8 dst_buff[1024*64];
#endif
void yy3(void) {};

//LZMA_BZP_EX01:cp.bin
//LZMA_BZP_EX02:zsp.bin

/*the format of bin file is
[blocksize-32bit][dictsize-32bit][streamlen-32bits][date.......][inputcrc-32bits][outputcrc-32bits]
[blocksize-32bit][dictsize-32bit][streamlen-32bits][date.......][inputcrc-32bits][outputcrc-32bits]
[blocksize-32bit][dictsize-32bit][streamlen-32bits][date.......][inputcrc-32bits][outputcrc-32bits]
*/
#if 0  // decrease the decode time.
#define _STEP_DEC_LEN_  2048
void lzma_uzip(LZMA_BZP_IDX_T idx,uint8*load_addr,LZMA_INPUT_T * in)
{
    uint32 BlockNum,BlockNumTotal;
    uint32 temp,kk,sumzip,sumorign,count_step,dec_error,dec_len,dec_offset,dec_len2,len_s;
    uint8 refbyte_en;
    uint8 cabac_totalbits;
    uint8 cabac_movebits;
    uint32 input_crc_hw;
    uint32 output_crc_hw;
    uint32 rdma_base_addr;
    uint32 wdma_base_addr;
    uint8 data;
    uint32 * ptr_block_size;
    uint32 * ptr_dict_size;
    uint32 * ptr_stream_len;
#ifdef _CHECK_CRC_
    uint32 * ptr_input_crc;
    uint32 * ptr_output_crc;
#endif
    uint8 *dec_data1;
    uint8 *dec_data_ref1;
    uint32 ii;
    uint32 tick,tick2;
    tick = timer_TimRealTickGet(OS_TIMER);
#ifdef _USE_MALLOC_BUFF_
    get_uzip_buff(in);
#endif

    sumzip = 0;

    SCI_TraceLow("[INFO] lzma :  bin id =%d \n",idx);
    ptr_block_size = block_info;
    ptr_dict_size = dict_info;
    ptr_stream_len = stream_info;
#ifdef _CHECK_CRC_
    ptr_input_crc = in_crc_info;
    ptr_output_crc=out_crc_info;
#endif
    BlockNumTotal = g_lzma_count;

    sumorign = 0;
//uint32 rdma_base_addr_cpoy;
    for(kk=0; kk<BlockNumTotal; kk++)
    {
        sumorign+=ptr_block_size[kk];

    }
    sumzip=0;
    for(kk=0; kk<BlockNumTotal; kk++)
    {
        sumzip+=ptr_stream_len[kk];
    }
    SCI_TraceLow("[INFO] lzma :BlockNum=%d, size %d ,fliesize=%dKbytes, zip fliesize  %d Kbytes\n",BlockNumTotal,ptr_block_size[0],sumorign/1024,sumzip/1024);


    hwp_lzma->lzma_cmd_reg = 0;
    hwp_lzma->lzma_status_reg = 0;
    temp=1;
    while(temp!=0)
    {
        temp = hwp_lzma->lzma_cmd_reg;
        SCI_TraceLow("[INFO] lzma : cmd_reg: 0x%x !\n", temp);
    }

    temp = 1;
    while(temp != 0)
    {
        temp = hwp_lzma->lzma_status_reg;
        SCI_TraceLow("[INFO] lzma : status_reg: 0x%x !\n", temp);
    }
    hwp_lzma->lzma_irq_mask = 0x07;
    refbyte_en = 0;
    cabac_totalbits = 0;
    cabac_movebits = 1;
    wdma_base_addr = (uint32)load_addr;
    rdma_base_addr = (uint32)in->zip_buf_addr;
    dst_buff = wdma_base_addr;
    SCI_TraceLow("[INFO] lzma : load_addr ddr: 0x%x !\n", load_addr);

    for(BlockNum = 0; BlockNum < BlockNumTotal; BlockNum++)
    {
        rdma_base_addr+=12;
        count_step = 0;
        dec_offset= 0;
        dec_len =0;
        dec_error=0;
        len_s = ptr_stream_len[BlockNum];
    DEC_REPEAT:

        if((dec_error ==0) && (len_s>_STEP_DEC_LEN_*2))
        {
            dec_len =_STEP_DEC_LEN_;

            memcpy((void *)src_buff, (const void*)rdma_base_addr, dec_len);
            MMU_DmaCacheSync((uint32)src_buff, dec_len,DMABUFFER_BIDIRECTIONAL);


        }
        else
        {
            memcpy((void *)src_buff, (const void*)rdma_base_addr, ptr_stream_len[BlockNum]);
            MMU_DmaCacheSync((uint32)src_buff, ptr_stream_len[BlockNum],DMABUFFER_BIDIRECTIONAL);
        }

        //SCI_TraceLow("[INFO] lzma : memcpy rdma_base_addr:0x%x ,len=0x%x  ,src_buff=0x%x\n", rdma_base_addr, ptr_stream_len[BlockNum],src_buff);
        hwp_lzma->lzma_dma_raddr_reg = (uint32)src_buff;//rdma_base_addr_cpoy;
        hwp_lzma->lzma_dma_waddr_reg =  (uint32)dst_buff;//(uint32)wdma_base_addr;
        hwp_lzma->lzma_config_reg1 = LZMA_REG_DICT_SIZE(ptr_dict_size[BlockNum]) + LZMA_REG_BLOCK_SIZE(ptr_block_size[BlockNum]);
        hwp_lzma->lzma_config_reg2 = LZMA_REG_STREAM_LEN(ptr_stream_len[BlockNum]);
        hwp_lzma->lzma_config_reg3 = (refbyte_en << 2) + (cabac_movebits << 1) + cabac_totalbits;
        hwp_lzma->lzma_cmd_reg = 1;
        temp = 0;
        if((dec_error ==0) && (len_s>_STEP_DEC_LEN_*2))
        {
            while(1)
            {
                dec_offset = dec_offset+_STEP_DEC_LEN_;
                if(dec_offset + _STEP_DEC_LEN_ <= len_s)
                {
                    dec_len = _STEP_DEC_LEN_;
                }
                else
                {
                    dec_len = len_s - dec_offset;
                }
                memcpy((void *)((uint32)src_buff+dec_offset), (const void*)(rdma_base_addr+dec_offset), dec_len);
                MMU_DmaCacheSync(((uint32)src_buff+dec_offset), dec_len,DMABUFFER_BIDIRECTIONAL);
                //mon_Event(0x10cca669); mon_Event((dec_offset +_STEP_DEC_LEN_)); mon_Event( len_s);

                if((dec_offset +_STEP_DEC_LEN_)  >= len_s)
                {
                    break;
                }

            }
        }
        // SCI_TraceLow("[INFO] lzma : rdmaaddr:0x%x, wdmaaddr:0x%x!\n", (uint32 )rdma_base_addr, (uint32 )wdma_base_addr);
        while (temp==0)
        {
            temp = hwp_lzma->lzma_status_reg;
        }

        if (temp==1)
        {
            input_crc_hw = hwp_lzma->lzma_input_crc;
            output_crc_hw = hwp_lzma->lzma_output_crc;
            //SCI_TraceLow("[INFO] lzma : check crc!\n");
#ifdef _CHECK_CRC_
            if (ptr_input_crc[BlockNum] != input_crc_hw)
            {
                SCI_TraceLow("[ERROR] lzma : BlockNum = %08d, InputCrc = %08x, Expected = %08x!\n", BlockNum, input_crc_hw, ptr_input_crc[BlockNum]);
                goto ERROR;
            }
            else if (ptr_output_crc[BlockNum] != output_crc_hw)
            {
                SCI_TraceLow("[ERROR] lzma : BlockNum = %08d, OutputCrc = %08x, Expected = %08x!\n", BlockNum, output_crc_hw, ptr_output_crc[BlockNum]);
                mon_Event(0x10ada014);
                goto ERROR;
            }

#endif
            hwp_lzma->lzma_cmd_reg = 0;
            hwp_lzma->lzma_status_reg = 0;
            rdma_base_addr += ptr_stream_len[BlockNum];
            rdma_base_addr+=8;

            MMU_DmaCacheSync((uint32)dst_buff, ptr_block_size[BlockNum],DMABUFFER_BIDIRECTIONAL);

            wdma_base_addr+=ptr_block_size[BlockNum];
            dst_buff = wdma_base_addr;


        }
        else
        {
            dec_error = 1;
            _LzmaReset();
            mon_Event(0x10adfff5);
            mon_Event(len_s);
            goto DEC_REPEAT;
        }


    }

#ifdef _USE_MALLOC_BUFF_
    free_uzip_buff();
#endif
    // SCI_ASSERT(SCI_FALSE); SCI_ASSERT(SCI_TRUE);
    tick2 = timer_TimRealTickGet(OS_TIMER);
    SCI_TraceLow("[INFO] lzma : Decode Finished  BlockNum =%d, load_addr =0x%x ,len=%d ,time= %d ms\n",BlockNum,load_addr,sumorign,((tick2-tick)*1000)/16384);

    mon_Event(0x10ada886);
    return;
ERROR:
    hwp_lzma->lzma_cmd_reg = 0;
    hwp_lzma->lzma_status_reg = 0;
    SCI_TraceLow("[INFO] lzma : Decode ERROR!\n");
    mon_Event(0x10adacff);
#ifdef _USE_MALLOC_BUFF_
    free_uzip_buff();
#endif

}

#else
void lzma_uzip(LZMA_BZP_IDX_T idx,uint8*load_addr,LZMA_INPUT_T * in)
{
    uint32 BlockNum,BlockNumTotal;
    uint32 temp,kk,sumzip,sumorign;
    uint8 refbyte_en;
    uint8 cabac_totalbits;
    uint8 cabac_movebits;
    uint32 input_crc_hw;
    uint32 output_crc_hw;
    uint32 rdma_base_addr;
    uint32 wdma_base_addr;
    uint8 data;
    uint32 * ptr_block_size;
    uint32 * ptr_dict_size;
    uint32 * ptr_stream_len;
#ifdef _CHECK_CRC_
    uint32 * ptr_input_crc;
    uint32 * ptr_output_crc;
#endif
    uint8 *dec_data1;
    uint8 *dec_data_ref1;
    uint32 ii;
    uint32 tick,tick2;
    tick = timer_TimRealTickGet(OS_TIMER);
#ifdef _USE_MALLOC_BUFF_
    get_uzip_buff(in);
#endif
    mon_Event(0x10ada011);
    sumzip = 0;

    SCI_TraceLow("[INFO] lzma :  bin id =%d \n",idx);
    ptr_block_size = block_info;
    ptr_dict_size = dict_info;
    ptr_stream_len = stream_info;
#ifdef _CHECK_CRC_
    ptr_input_crc = in_crc_info;
    ptr_output_crc=out_crc_info;
#endif
    BlockNumTotal = g_lzma_count;

    sumorign = 0;
//uint32 rdma_base_addr_cpoy;
    for(kk=0; kk<BlockNumTotal; kk++)
    {
        sumorign+=ptr_block_size[kk];

    }
    //SCI_TraceLow("[INFO] lzma :  BlockNumTotal =%d,the origin fliesize is about %d Kbytes\n",BlockNumTotal,sumorign/(1024));
    sumzip=0;
    for(kk=0; kk<BlockNumTotal; kk++)
    {
        sumzip+=ptr_stream_len[kk];
    }
    //SCI_TraceLow("[INFO] lzma : block size %d       the encode zip fliesize is about %d Kbytes\n\n\n\n",ptr_block_size[0],sumzip/(1024));


    hwp_lzma->lzma_cmd_reg = 0;
    hwp_lzma->lzma_status_reg = 0;
    temp=1;
    while(temp!=0)
    {
        temp = hwp_lzma->lzma_cmd_reg;
        //SCI_TraceLow("[INFO] lzma : cmd_reg: 0x%x !\n", temp);
    }

    temp = 1;
    while(temp != 0)
    {
        temp = hwp_lzma->lzma_status_reg;
        //SCI_TraceLow("[INFO] lzma : status_reg: 0x%x !\n", temp);
    }
    hwp_lzma->lzma_irq_mask = 0x07;
    refbyte_en = 0;
    cabac_totalbits = 0;
    cabac_movebits = 1;
    wdma_base_addr = (uint32)load_addr;
    rdma_base_addr = (uint32)in->zip_buf_addr;
    SCI_TraceLow("[INFO] lzma : load_addr ddr: 0x%x !\n", load_addr);
    for(BlockNum = 0; BlockNum < BlockNumTotal; BlockNum++)
    {
        rdma_base_addr+=12;
        memcpy((void *)src_buff, (const void*)rdma_base_addr, ptr_stream_len[BlockNum]);
        MMU_DmaCacheSync((uint32)src_buff, ptr_stream_len[BlockNum],DMABUFFER_BIDIRECTIONAL);

        //SCI_TraceLow("[INFO] lzma : memcpy rdma_base_addr:0x%x ,len=0x%x  ,src_buff=0x%x\n", rdma_base_addr, ptr_stream_len[BlockNum],src_buff);
        hwp_lzma->lzma_dma_raddr_reg = (uint32)src_buff;//rdma_base_addr_cpoy;
        hwp_lzma->lzma_dma_waddr_reg =  (uint32)dst_buff;//(uint32)wdma_base_addr;
        hwp_lzma->lzma_config_reg1 = LZMA_REG_DICT_SIZE(ptr_dict_size[BlockNum]) + LZMA_REG_BLOCK_SIZE(ptr_block_size[BlockNum]);
        hwp_lzma->lzma_config_reg2 = LZMA_REG_STREAM_LEN(ptr_stream_len[BlockNum]);
        hwp_lzma->lzma_config_reg3 = (refbyte_en << 2) + (cabac_movebits << 1) + cabac_totalbits;
        hwp_lzma->lzma_cmd_reg = 1;
        temp = 0;

        // SCI_TraceLow("[INFO] lzma : rdmaaddr:0x%x, wdmaaddr:0x%x!\n", (uint32 )rdma_base_addr, (uint32 )wdma_base_addr);
        while (temp==0)
        {
            temp = hwp_lzma->lzma_status_reg;
        }

        if (temp==1)
        {
            input_crc_hw = hwp_lzma->lzma_input_crc;
            output_crc_hw = hwp_lzma->lzma_output_crc;
            //SCI_TraceLow("[INFO] lzma : check crc!\n");
#ifdef _CHECK_CRC_
            if (ptr_input_crc[BlockNum] != input_crc_hw)
            {
                SCI_TraceLow("[ERROR] lzma : BlockNum = %08d, InputCrc = %08x, Expected = %08x!\n", BlockNum, input_crc_hw, ptr_input_crc[BlockNum]);
                goto ERROR;
            }
            else if (ptr_output_crc[BlockNum] != output_crc_hw)
            {
                SCI_TraceLow("[ERROR] lzma : BlockNum = %08d, OutputCrc = %08x, Expected = %08x!\n", BlockNum, output_crc_hw, ptr_output_crc[BlockNum]);
                mon_Event(0x10ada014);
                goto ERROR;
            }
            //SCI_TraceLow("[INFO] lzma : check crc ok \n");
#endif
            //MMU_DmaCacheSync((uint32)dst_buff, (uint32)(ptr_block_size[BlockNum]),DMABUFFER_BIDIRECTIONAL);

            // SCI_TraceLow("[INFO] lzma : BlockNum = %08d, Deocde Correct!\n", BlockNum);
            //  if(bzp_stream_len[BlockNum] % 8 == 0)
            //  rdma_base_addr += bzp_stream_len[BlockNum];
            // else
            //   rdma_base_addr += bzp_stream_len[BlockNum] + 8 - bzp_stream_len[BlockNum] % 8;

            hwp_lzma->lzma_cmd_reg = 0;
            hwp_lzma->lzma_status_reg = 0;
            rdma_base_addr += ptr_stream_len[BlockNum];
            rdma_base_addr+=8;

            MMU_DmaCacheSync((uint32)dst_buff, ptr_block_size[BlockNum],DMABUFFER_BIDIRECTIONAL);

            memcpy((void *)wdma_base_addr, (const void*)dst_buff, ptr_block_size[BlockNum]);
            //MMU_DmaCacheSync((uint32)wdma_base_addr, ptr_block_size[BlockNum],DMABUFFER_BIDIRECTIONAL);


#if 0

            for(ii = 0; ii < ptr_block_size[BlockNum]; ii++)
            {
                dec_data1 = *((volatile uint8*)wdma_base_addr + ii);
                dec_data_ref1 = *((volatile uint8*)dst_buff + ii);
                if(dec_data1 != dec_data_ref1)
                {
                    SCI_TraceLow("[ERROR] lzma : BlockNum = %08d, Deocde Data Error, addr_offset = %08x, data = %02x, expected = %02x!\n", \
                                 BlockNum, ii, dec_data1, dec_data_ref1);

                    goto ERROR;
                }
            }
            //  if(BlockNum == 0)



            dec_data1 = ((volatile uint8*)load_addr);
            dec_data_ref1 = ((volatile uint8*)dst_buff);

            SCI_TraceLow("y lzma  load_addr1 =0x%x : %x, %x, %x, %8x, %x, %x, %x, %x,\n",(uint32) load_addr,
                         dec_data1[0],dec_data1[1],dec_data1[2],dec_data1[3],dec_data1[4],dec_data1[5],dec_data1[6],dec_data1[7]);
            SCI_TraceLow("y  lzma  dst_buff1=0x%x : %x, %x, %x, %8x, %x, %x, %x, %x,\n", (uint32)dst_buff,
                         dec_data_ref1[0],dec_data_ref1[1],dec_data_ref1[2],dec_data_ref1[3],dec_data_ref1[4],dec_data_ref1[5],dec_data_ref1[6],dec_data_ref1[7]);

#endif
            wdma_base_addr+=ptr_block_size[BlockNum];


        }
        else if((temp & 0x02) == 0x02)
        {
            temp = hwp_lzma->lzma_error_type;
            SCI_TraceLow("[ERROR] lzma : BlockNum = %08d, lzma_status_reg = 0x%x, lzma_status_reg2 = 0x%x, lzma_status_reg3 = 0x%x, Decoder Error, Error_type = %02x!\n", \
                         BlockNum, hwp_lzma->lzma_status_reg, hwp_lzma->lzma_status_reg2,  hwp_lzma->lzma_status_reg3, temp);
            mon_Event(0x10ada016);
            goto ERROR;
        }
        else if((temp & 0x04) == 0x04)
        {
            SCI_TraceLow("[ERROR] lzma : BlockNum = %08d, AXI BUS Error!\n");
            mon_Event(0x10ada017);
            goto ERROR;
        }
        else
        {
            SCI_TraceLow("[ERROR] lzma : BlockNum = %08d, Unknown Error!\n");
            mon_Event(0x10ada018);
            goto ERROR;
        }
    }


#ifdef _USE_MALLOC_BUFF_
    free_uzip_buff();
#endif
    // SCI_ASSERT(SCI_FALSE); SCI_ASSERT(SCI_TRUE);

    //MMU_PHY_DCacheSync();
    //MMU_PHY_InvalideDCACHEALL();
    tick2 = timer_TimRealTickGet(OS_TIMER);
    SCI_TraceLow("[INFO] lzma : time =%dms\n",(tick2-tick)*1000/16384);
    SCI_TraceLow("[INFO] lzma : Finished  BlockNum =%d, load_addr =0x%x ,len=%d\n",BlockNum,load_addr,sumorign);

    mon_Event(0x10ada086);
    return;
ERROR:
    hwp_lzma->lzma_cmd_reg = 0;
    hwp_lzma->lzma_status_reg = 0;
    SCI_TraceLow("[INFO] lzma : Decode ERROR!\n");
    mon_Event(0x10ada0ff);
#ifdef _USE_MALLOC_BUFF_
    free_uzip_buff();
#endif

}
#endif
#else
void lzma_uzip(LZMA_BZP_IDX_T idx,uint8*load_addr,LZMA_INPUT_T * in)
{
}
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif



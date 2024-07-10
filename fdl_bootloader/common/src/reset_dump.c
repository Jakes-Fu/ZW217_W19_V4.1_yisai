/******************************************************************************
 ** File Name:      reset_dump.c                                               *
 ** Author:         yang.li3                                             *
 ** DATE:           10/11/2022                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain reset dump function.                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/11/2022     yang.li3         Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "global_macros.h"
#include "sys_ctrl_6121e.h"
#include "serial.h"
#include "uart_reg_uws6121e.h"
#include "mem_cfg.h"
#include "mem_dump.h"
#include "cmddef.h"
#include "idle_lps.h"
#include "uws6121e_reg_analog.h"
#include "adi_hal_internal.h"
#include "reset_dump.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

LOCAL uint32 s_mem_seq_num_reset = 0;

const uint8 reset_dump_mem_data[8] = {0, 0, 0, 0, // seq num and dummy word
                                           0, 0,
                                           MSG_BOARD_ASSERT, DUMP_MEM_DATA};

const uint8 reset_dump_mem_end[8] = {0, 0, 0, 0, // seq num and dummy word
                                          0, 0,
                                          MSG_BOARD_ASSERT, DUMP_MEM_END};

struct FDL_ChannelHandler *gResrtDumpUsbChannel;


/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*########################### UART ###########################*/
int RD_UartPutChar (const unsigned char ch)
{
    REG_UART_STATUS_T status;
    HWP_UART_T *Regbase;
    Regbase = (HWP_UART_T *)(RESET_DUMP_UART_REGBASE);
    status.v = Regbase->status;

    do{

        status.v = Regbase->status;

    }while(status.b.tx_fifo_space == 0);

    Regbase->rxtx_buffer = ch;

    return 0;
}

int RD_UartGetSingleChar(void)
{

    REG_UART_STATUS_T status;
    HWP_UART_T *Regbase;
    Regbase = (HWP_UART_T *)(RESET_DUMP_UART_REGBASE);
    status.v = Regbase->status;

    if(status.b.rx_fifo_level == 0)
    {
        return -1;
    }

    return ((Regbase->rxtx_buffer) & 0xff);
}

uint8 RD_UartPutUint8(uint8 data)
{
    if ((0x7E == data) || (0x7D == data))
    {
        RD_UartPutChar(0x7D);
        RD_UartPutChar((data ^ 0x20));
        return 2;
    }
    else
    {
        RD_UartPutChar(data);
        return 1;
    }
}

void RD_UartPutUint32(uint32 data)
{
    int i;

    for (i = 0; i < 4; ++i)
    {
        uint8 c = (data & 0xFF000000) >> 24;

        RD_UartPutUint8(c);

        data <<= 8;
    }
}


uint32 RD_UartPutUint8Chars(const int8* dataptr, int32 len)
{
    int i=0;
    uint8 mem_data = 0;
    uint32 total_len=0;
    for(i=0;i<len;i++){
        mem_data = *dataptr;
        total_len += RD_UartPutUint8(mem_data);
        dataptr++;
    }
    return total_len;
}

void RD_UartPrintHeaderInfo(void)
{
   int32 i;

   for (i = 0; i < (int32)sizeof(reset_dump_mem_data); i++)
   {
       RD_UartPutUint8(reset_dump_mem_data[i]);
   }
}


void RD_UartUpdateDumpHeader(uint32 seq_num, uint16 len)
{
   MSG_HEAD_T* data_ptr = NULL;

   data_ptr = (MSG_HEAD_T*)(reset_dump_mem_data);

#if defined(CHIP_ENDIAN_LITTLE) && defined(PLATFORM_SC8800H)
   data_ptr->seq_num = DWORD_ENDIAN_SWAP(seq_num);
   data_ptr->len     = WORD_ENDIAN_SWAP(len);
#else
   data_ptr->seq_num = seq_num;
   data_ptr->len     = len;
#endif

   RD_UartPrintHeaderInfo();
}

uint32 RD_UartDumpMemSection(uint32 data, uint32 len)
{
  uint32  seq_num;
  const uint8* p_data = (const uint8*)data;
  uint32 size = len;
  uint32 m = 0;
  uint32 n = 0;
  int i=0;
  uint32 total_len = 0;

  m = (size) / RESET_DUMP_UNIT;
  n = (size) % RESET_DUMP_UNIT;

  seq_num=s_mem_seq_num_reset;
  for(i= 0; i < m; i++)
  {

      RD_UartPutChar(0x7E);

      // update and print Head info.
      RD_UartUpdateDumpHeader(++seq_num, RESET_DUMP_UNIT);

      total_len += RD_UartPutUint8Chars(p_data, RESET_DUMP_UNIT);

      p_data = (const uint8*)(p_data + RESET_DUMP_UNIT);

      RD_UartPutChar(0x7E);
  }

  if(n != 0)
  {
      RD_UartPutChar(0x7E);

      RD_UartUpdateDumpHeader(++seq_num, n);

      total_len += RD_UartPutUint8Chars(p_data,n);

      RD_UartPutChar(0x7E);
  }

  s_mem_seq_num_reset = seq_num;

  return total_len;
}

uint32 RD_UartDumpRegisterGroup(void)
{
    uint32  i = 0;
    uint32 reg_group_num = CHIP_GetNumCtlRegGrp();
    CTL_REG_INFO_T  *IpRegInfoPtr = (CTL_REG_INFO_T *)CHIP_GetInfoOfCtlReg();
    uint32 dump_count =0;
    uint32 ip_reg_size = 0;

    //SCI_PrintAssertString("\n Dump IpRegInfoPtr->magic = %s", IpRegInfoPtr->magic);
    for(i=0; i<reg_group_num; i++)
    {
        if(IpRegInfoPtr[i].reg_address_begin != 0xffffffff)
        {
            ip_reg_size = IpRegInfoPtr[i].reg_address_end - IpRegInfoPtr[i].reg_address_begin;
            //SCI_PrintAssertString("\n Region:Offset=0x%08X, Length=0x%08X",
                    //CtlRegInfoPtr_temp[i].reg_address_begin,
                    //ip_reg_size);
            RD_UartDumpMemSection(IpRegInfoPtr[i].reg_address_begin, ip_reg_size);
            dump_count+=ip_reg_size;
        }
    }

    return dump_count;
}

uint32  ResetUartDump2Logel(void)
{
    uint32 m = 0;
    uint32 n = 0;
    uint32 total_len = 0;
    uint32 dump_len = 0;
    uint32 size_of_dump_mem = 0;
    uint16 num_of_dump_mem = 0;
    uint16 i = 0;


    CTL_MEM_INFO_T *CtlMemInfoPtr = NULL;
    CtlMemInfoPtr = (CTL_MEM_INFO_T *)CHIP_GetInfoOfCtlMem();
    num_of_dump_mem = CHIP_GetNumCtlMemGrp();

    s_mem_seq_num_reset = 0;

    for(i=0;i<num_of_dump_mem;i++){
       if(CtlMemInfoPtr[i].mem_address_begin != 0xffffffff){
           //SCI_PrintAssertString("\n Region:Offset=0x%08X, Length=0x%08X",CtlMemInfoPtr[i].mem_address_begin,CtlMemInfoPtr[i].mem_size);
           RD_UartDumpMemSection(CtlMemInfoPtr[i].mem_address_begin, CtlMemInfoPtr[i].mem_size);
           size_of_dump_mem += CtlMemInfoPtr[i].mem_size;
       }
    }

    // Dump registers
    dump_len = RD_UartDumpRegisterGroup();

    //SCI_PrintAssertString("\nRegister Data:Offset=0x%08X, Length=0x%08X", (int)size_of_dump_mem, (int)dump_len);
    size_of_dump_mem = size_of_dump_mem + dump_len;

    // Dump end packet.
    RD_UartPutChar(0x7E);

    for (i = 0; i < sizeof(reset_dump_mem_end); i++)
    {
        RD_UartPutUint8(reset_dump_mem_end[i]);
    }

    RD_UartPutChar(0x9);

    RD_UartPutChar(0x7E);

    return size_of_dump_mem;
}


uint32  ResetUartDumpBin(void*data, uint32 len)
{

    const uint8* p_data = (const uint8*)data;

    uint32 size = len;

    uint32 m = 0;
    uint32 n = 0;
    int i=0;
    uint32 total_len=0;

    m = (size) / RESET_DUMP_UNIT;
    n = (size) % RESET_DUMP_UNIT;

    for(i= 0; i < m; i++)
    {

      total_len += Download_SIO_Write(NULL,p_data,RESET_DUMP_UNIT);

      p_data = (const uint8*)(p_data+RESET_DUMP_UNIT);

    }

    if(n!=0)
    {

      total_len += Download_SIO_Write(NULL,p_data,n);

    }


return total_len;
}



/*########################### USB ###########################*/

int RD_UsbPutChar (const unsigned char ch)
{
    gResrtDumpUsbChannel = ResetDumpUsbChannelGet();
    gResrtDumpUsbChannel->PutChar(NULL,ch);
    return 0;
}

uint8 RD_UsbPutUint8(uint8 data)
{
    if ((0x7E == data) || (0x7D == data))
    {
        RD_UsbPutChar(0x7D);
        RD_UsbPutChar((data ^ 0x20));
        return 2;
    }
    else
    {
        RD_UsbPutChar(data);
        return 1;
    }
}

void RD_UsbPutUint32(uint32 data)
{
    int i;

    for (i = 0; i < 4; ++i)
    {
        uint8 c = (data & 0xFF000000) >> 24;

        RD_UsbPutUint8(c);

        data <<= 8;
    }
}

uint32 RD_UsbPutUint8Chars(const int8* dataptr, int32 len)
{
    int i=0;
    uint8 mem_data = 0;
    uint32 total_len=0;
    for(i=0;i<len;i++){
        mem_data = *dataptr;
        total_len += RD_UsbPutUint8(mem_data);
        dataptr++;
    }
    return total_len;
}

void RD_UsbPrintHeaderInfo(void)
{
   int32 i;

   for (i = 0; i < (int32)sizeof(reset_dump_mem_data); i++)
   {
       RD_UsbPutUint8(reset_dump_mem_data[i]);
   }
}

void RD_UsbUpdateDumpHeader(uint32 seq_num, uint16 len)
{
   MSG_HEAD_T* data_ptr = NULL;

   data_ptr = (MSG_HEAD_T*)(reset_dump_mem_data);

#if defined(CHIP_ENDIAN_LITTLE) && defined(PLATFORM_SC8800H)
   data_ptr->seq_num = DWORD_ENDIAN_SWAP(seq_num);
   data_ptr->len     = WORD_ENDIAN_SWAP(len);
#else
   data_ptr->seq_num = seq_num;
   data_ptr->len     = len;
#endif

   RD_UsbPrintHeaderInfo();
}

uint32 RD_UsbDumpMemSection(uint32 data, uint32 len)
{
  uint32  seq_num;
  const uint8* p_data = (const uint8*)data;
  uint32 size = len;
  uint32 m = 0;
  uint32 n = 0;
  int i=0;
  uint32 total_len = 0;

  m = (size) / RESET_DUMP_UNIT;
  n = (size) % RESET_DUMP_UNIT;

  seq_num=s_mem_seq_num_reset;
  for(i= 0; i < m; i++)
  {

      RD_UsbPutChar(0x7E);

      // update and print Head info.
      RD_UsbUpdateDumpHeader(++seq_num, RESET_DUMP_UNIT);

      total_len += RD_UsbPutUint8Chars(p_data, RESET_DUMP_UNIT);

      p_data = (const uint8*)(p_data + RESET_DUMP_UNIT);

      RD_UsbPutChar(0x7E);
  }

  if(n != 0)
  {
      RD_UsbPutChar(0x7E);

      RD_UsbUpdateDumpHeader(++seq_num, n);

      total_len += RD_UsbPutUint8Chars(p_data,n);

      RD_UsbPutChar(0x7E);
  }

  s_mem_seq_num_reset = seq_num;

  return total_len;
}


uint32 RD_UsbDumpRegisterGroup(void)
{
    uint32  i = 0;
    uint32 reg_group_num = CHIP_GetNumCtlRegGrp();
    CTL_REG_INFO_T  *IpRegInfoPtr = (CTL_REG_INFO_T *)CHIP_GetInfoOfCtlReg();
    uint32 dump_count =0;
    uint32 ip_reg_size = 0;

    //SCI_PrintAssertString("\n Dump IpRegInfoPtr->magic = %s", IpRegInfoPtr->magic);
    for(i=0; i<reg_group_num; i++)
    {
        if(IpRegInfoPtr[i].reg_address_begin != 0xffffffff)
        {
            ip_reg_size = IpRegInfoPtr[i].reg_address_end - IpRegInfoPtr[i].reg_address_begin;
            //SCI_PrintAssertString("\n Region:Offset=0x%08X, Length=0x%08X",
                    //CtlRegInfoPtr_temp[i].reg_address_begin,
                    //ip_reg_size);
            RD_UsbDumpMemSection(IpRegInfoPtr[i].reg_address_begin, ip_reg_size);
            dump_count+=ip_reg_size;
        }
    }

    return dump_count;
}


uint32  ResetUsbDump2Logel(void)
{
    uint32 m = 0;
    uint32 n = 0;
    uint32 total_len = 0;
    uint32 dump_len = 0;
    uint32 size_of_dump_mem = 0;
    uint16 num_of_dump_mem = 0;
    uint16 i = 0;


    CTL_MEM_INFO_T *CtlMemInfoPtr = NULL;
    CtlMemInfoPtr = (CTL_MEM_INFO_T *)CHIP_GetInfoOfCtlMem();
    num_of_dump_mem = CHIP_GetNumCtlMemGrp();

    s_mem_seq_num_reset = 0;

    usb_boot(1);//boot 2 times

    gResrtDumpUsbChannel = ResetDumpUsbChannelGet();


    for(i=0;i<num_of_dump_mem;i++){
       if(CtlMemInfoPtr[i].mem_address_begin != 0xffffffff){
           //SCI_PrintAssertString("\n Region:Offset=0x%08X, Length=0x%08X",CtlMemInfoPtr[i].mem_address_begin,CtlMemInfoPtr[i].mem_size);
           RD_UsbDumpMemSection(CtlMemInfoPtr[i].mem_address_begin, CtlMemInfoPtr[i].mem_size);
           size_of_dump_mem += CtlMemInfoPtr[i].mem_size;
       }
    }

    // Dump registers
    dump_len = RD_UsbDumpRegisterGroup();

    //SCI_PrintAssertString("\nRegister Data:Offset=0x%08X, Length=0x%08X", (int)size_of_dump_mem, (int)dump_len);
    size_of_dump_mem = size_of_dump_mem + dump_len;

    // Dump end packet.
    RD_UsbPutChar(0x7E);

    for (i = 0; i < sizeof(reset_dump_mem_end); i++)
    {
        RD_UsbPutUint8(reset_dump_mem_end[i]);
    }

    RD_UsbPutChar(0x9);

    RD_UsbPutChar(0x7E);

    gResrtDumpUsbChannel->Close(gResrtDumpUsbChannel);//flush

return size_of_dump_mem;
}


/*########################### COMMON ###########################*/

uint32 RD_GetCur32kNum(void)
{
    return hwp_idleLps->lps_32k_ref;
}


uint32 RD_GetCur32kNum2Second(void)
{
     uint32 sec = RD_GetCur32kNum() * 30 / 1000000 ;
     return sec;
}

void RD_DelaySecond(uint32 second)
{
    uint32 sec = RD_GetCur32kNum();
    while( RD_GetCur32kNum() < sec + 32768 * second);
}

void RD_ResetMCUSoft(void)
{
       //REG_ADI_CHANGE1(hwp_pmicPsm->por_timer_thd, psm_por_timer_thd, ext_rst_timer_thd, 0x3FF);
       ANA_REG_OR(0x700+0x78, 0x3FF);
       //halAdiBusWrite(&PMU_BOOT_MODE_REG, mode);
       ANA_REG_AND(ANA_POR_RST_MONITOR, 0);
       //swrst_ctrl0.b.reg_rst_en = 1;
       ANA_REG_OR(ANA_SWRST_CTRL0, (0x1 << 4));
       //swrst_ctrl0.b.reg_rst_pd_en = 1;
       ANA_REG_OR(ANA_SWRST_CTRL0, (0x1 << 8));
       //soft_rst_hw.b.reg_soft_rst_sw = 1;
       ANA_REG_OR(ANA_SOFT_RST_HW, 0x1);
       while(1);
}

void RD_PowerOff (void)
{

    CHIP_PHY_SetANAReg (ANA_POWER_PD_HW, PWR_OFF_SEQ_EN);

}


uint32 After7sResetActionType(void)
{
    char ch_usb;
    char ch_uart;
    uint32  old_tick, new_tick;

    old_tick = new_tick = RD_GetCur32kNum2Second();
    RESETDUMP_LOG("After7sResetActionType enter new_tick: %d\n", new_tick);
    usb_boot(1);
    gResrtDumpUsbChannel = ResetDumpUsbChannelGet();

    for ( ; ; )
     {

           ch_usb = gResrtDumpUsbChannel->GetSingleChar(gResrtDumpUsbChannel);
           ch_uart = RD_UartGetSingleChar();

           if (0x7e == ch_usb)
           {

              return CHANNEL_USB;
           }
           if (0x7e == ch_uart)
           {

              return CHANNEL_UART;
           }

           new_tick = RD_GetCur32kNum2Second();
           //RESETDUMP_LOG("new_tick: %d  old_tick: %d\n", new_tick,old_tick);
           if ((new_tick - old_tick) > RESET_DUMP_WAIT_TIME)//10
           {

               break;
           }

     }
    RESETDUMP_LOG("After7sResetActionType leave new_tick: %d\n", new_tick);
    RESETDUMP_LOG("RESET_DUMP_WAIT_TIME: %d--new_tick-old_tick: %d\n",RESET_DUMP_WAIT_TIME, new_tick-old_tick);

    return CHANNEL_NO_RESET;

}

void After7sResetActionStart(DumpChannel channel)
{

    switch (channel)
        {
            case CHANNEL_USB:
                ResetUsbDump2Logel();
                rprintf("USB DUMP FINISH RESET\n");//Also used for delay
                RD_DelaySecond(1);
                RD_ResetMCUSoft();
                break;

            case CHANNEL_UART:
                ResetUartDump2Logel();
                rprintf("UART DUMP FINISH RESET\n");//Also used for delay
                RD_DelaySecond(1);
                RD_ResetMCUSoft();
                break;

            case CHANNEL_NO_POWEROFF:
                RD_PowerOff();
                break;

            case CHANNEL_NO_RESET:
                RD_ResetMCUSoft();
                break;

            default:
                RD_ResetMCUSoft();
                break;

        }

}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

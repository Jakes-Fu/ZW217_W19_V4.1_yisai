/******************************************************************************
 ** File Name:      spi_lm49370_v3.c                                              *
 ** Author:                                                            *
 ** DATE:                                                           *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    .                                         *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/02/2009               Create.                                   *
 *****************************************************************************/
#include "chip_drv_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "spi_drvapi.h"
//#include "../../../../spi/spi_phy.h"

#include "LM49370_v3.h"

#define SPI_LOGIC_ID 2


BOOLEAN  SPI_LM49370_Init (void)
{
    //uint32 index;
    SPI_DEV SPIConfig = {0};

    SPIConfig.id = SPI_LOGIC_ID;
    SPIConfig.freq = 200000; /* Start out with a slow SPI clock, 400kHz*/
    SPIConfig.tx_bit_length = 8;
    SPIConfig.mode = CPOL0_CPHA0;


    if ( SPI_HAL_Open (&SPIConfig) )
    {
        //SCI_TraceLow:"SCM: This SPI Port Has been occupied!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_39_112_2_17_23_2_46_157,(uint8*)"");
        //SCI_ASSERT(0);
        return FALSE;
    }

    return TRUE;
}

void SPI_LM49370_WriteByte (uint8 *WAddrData, uint8 len)
{
    uint32 id = SPI_LOGIC_ID;
    uint32 cs_sel;

    cs_sel = SPI_CS_LOW;
    SPI_HAL_Ioctl(id, SPI_CTL_S_CS, &cs_sel);

    SPI_HAL_Write(id, WAddrData, len);

    cs_sel = SPI_CS_HIGH;
    SPI_HAL_Ioctl(id, SPI_CTL_S_CS, &cs_sel);

}

uint8 SPI_LM49370_ReadByte (uint8 *WAddrData, uint8 wlen, uint8 *RData, uint8 rlen)
{
    uint8 data = 0;
    uint32 id = SPI_LOGIC_ID;
    uint32 cs_sel;

    cs_sel = SPI_CS_LOW;
    SPI_HAL_Ioctl(id, SPI_CTL_S_CS, &cs_sel);

    SPI_HAL_Write(id, WAddrData, 1);

    SPI_HAL_Read (id, &data, 1);

    cs_sel = SPI_CS_HIGH;
    SPI_HAL_Ioctl(id, SPI_CTL_S_CS, &cs_sel);


    return data;
}

void LM49370RegTest(void)
{
#define MAX_ADDR        0x1E

    uint8 idata;
    uint32 i;
    uint8 addrdata[2];
    uint8 readdata[2] = {0, 0};

    SPI_LM49370_Init();

    // Select GPIO2 for SPI_OUT
    addrdata[0] = 0x1F;
    addrdata[1] = 0x08;
    SPI_LM49370_WriteByte (addrdata, 2);


    for (i = 0; i <= MAX_ADDR; i++)
    {
        if (i == 0x18)
        {
            continue;
        }

        // Write data to Register
        addrdata[0] = i;
        addrdata[1] = (i + 1) & ~0xF0;
        SPI_LM49370_WriteByte (addrdata, 2);

        // Read data from Register
        addrdata[0] |= 0x80;
        //addrdata[1] = 0x00;
        idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);

        if (idata != addrdata[1])
        {
            //SCI_TraceLow:"Error in LM49370 Register 0x%02XrnValue Write In : 0x%02XrnValue Read Out : 0x%02Xrnrn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_121_112_2_17_23_2_46_158,(uint8*)"ddd",addrdata[0] & ~0x80,addrdata[1],idata);

            return;
        }
    }

    //SCI_TraceLow:"\r\nLM49370 Register Test Done, OK!\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_127_112_2_17_23_2_46_159,(uint8*)"");
}

#define LM49370_REG_READ_BACK

void LM49370_IIS_Slave_RegSet()
{

    uint8 idata;
    //uint32 i;
    uint8 addrdata[2];
    uint8 readdata[2] = {0, 0};

    SPI_LM49370_Init();

#ifdef LM49370_REG_READ_BACK
    // Select GPIO2 for SPI_OUT
    addrdata[0] = 0x1F;
    addrdata[1] = 0x08;
    SPI_LM49370_WriteByte (addrdata, 2);
#endif

    // Basic Configuration, 0x00
    addrdata[0] = 0x00;
    addrdata[1] = 0x06;                                 // PLL Enalbe
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_M, 0x02
    addrdata[0] = 0x02;
    addrdata[1] = ( (13 * 2) - 1);                      // PLL_M
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N, 0x03
    addrdata[0] = 0x03;
    addrdata[1] = 60;                                   // PLL_N
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N_MOD, 0x05
    addrdata[0] = 0x05;
    addrdata[1] = (PLL_IN_MCLK << 5) | (0x00 << 0);     // PLL_N_MOD = 0/32, PLL_IN = MCLK
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PLL_P, 0x04
    addrdata[0] = 0x04;
    addrdata[1] = (0x6 << 4) | ( ( (5 * 2) - 1) << 0);  // Q_DIV = 6, PLL_P = 9
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // CLOCKS, 0x01
    addrdata[0] = 0x01;
    addrdata[1] = 0x2D;                                 // R_DIV = 11, DAC_CLK_IN = PLL_OUT
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // ADC_2, 0x07
    addrdata[0] = 0x07;
    addrdata[1] = 0x20;                                 // ADC_CLK_IN = PLL_OUT, ADC OverSampling = 125
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // IIS Clock, 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x00;                                 // PCM_SYNC_WIDTH = 1 bit
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Mode, 0x1C
    addrdata[0] = 0x1C;
    addrdata[1] = 0x00;                                 // PCM_CLK = DAC, PCM_IN/OUT = Disable, PCM Slave
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Clock, 0x1D
    addrdata[0] = 0x1D;
    addrdata[1] = 0x56;                                 // PCM_CLK Divided by : 16, SYNC Divided by : 128
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // LS_OUTPUT, 0x14
    addrdata[0] = 0x14;
    addrdata[1] = 0x08;                                 // LEFT, RIGHT Selected
    SPI_LM49370_WriteByte (addrdata, 2);


    // HP_OUTPUT, 0x15
    addrdata[0] = 0x15;
    addrdata[1] = 0x0C;                                 // LEFT, RIGHT Enable
    //addrdata[1] = 0x00;                                   // LEFT, RIGHT Disable
    SPI_LM49370_WriteByte (addrdata, 2);


    // AUX_LEFT, 0x0F
    addrdata[0] = 0x0F;
    addrdata[1] = 0x1F;                                 // Left level
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x10
    addrdata[0] = 0x10;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1A
    addrdata[0] = 0x1A;
    addrdata[1] = 0x87;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x00;                                 // IIS Clock : Slave, Clock Source : DAC
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1E
    addrdata[0] = 0x1E;
    addrdata[1] = 0x00;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x11
    addrdata[0] = 0x11;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);
}

void LM49370_PCM_Slave_RegSet()
{
    uint8 idata;
    //uint32 i;
    uint8 addrdata[2];
    uint8 readdata[2] = {0, 0};

    SPI_LM49370_Init();

#ifdef LM49370_REG_READ_BACK
    // Select GPIO2 for SPI_OUT
    addrdata[0] = 0x1F;
    addrdata[1] = 0x08;
    SPI_LM49370_WriteByte (addrdata, 2);
#endif

    // Basic Configuration, 0x00
    addrdata[0] = 0x00;
    addrdata[1] = 0x06;                                 // PLL Enalbe
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_M, 0x02
    addrdata[0] = 0x02;
    addrdata[1] = ( (13 * 2) - 1);                      // PLL_M
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N, 0x03
    addrdata[0] = 0x03;
    addrdata[1] = 60;                                   // PLL_N
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N_MOD, 0x05
    addrdata[0] = 0x05;
    addrdata[1] = (PLL_IN_MCLK << 5) | (0x00 << 0);     // PLL_N_MOD = 0/32, PLL_IN = MCLK
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PLL_P, 0x04
    addrdata[0] = 0x04;
    addrdata[1] = (0x6 << 4) | ( ( (5 * 2) - 1) << 0);  // Q_DIV = 6, PLL_P = 9
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // CLOCKS, 0x01
    addrdata[0] = 0x01;
    addrdata[1] = 0x2D;                                 // R_DIV = 11, DAC_CLK_IN = PLL_OUT
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // ADC_2, 0x07
    addrdata[0] = 0x07;
    addrdata[1] = 0x20;                                 // ADC_CLK_IN = PLL_OUT, ADC OverSampling = 125
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // IIS Clock, 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x00;                                 // PCM_SYNC_WIDTH = 1 bit
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Mode, 0x1C
    addrdata[0] = 0x1C;
    addrdata[1] = 0x03;                                 // PCM_CLK = DAC, PCM_IN/OUT = Enable, PCM Slave
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Clock, 0x1D
    addrdata[0] = 0x1D;
    addrdata[1] = 0x56;                                 // PCM_CLK Divided by : 16, SYNC Divided by : 128
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // LS_OUTPUT, 0x14
    addrdata[0] = 0x14;
    addrdata[1] = 0x08;                                 // LEFT, RIGHT Selected
    SPI_LM49370_WriteByte (addrdata, 2);

    // LS_OUTPUT, 0x14
    addrdata[0] = 0x15;
    addrdata[1] = 0x2C;                                 // LEFT, RIGHT Selected
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_LEFT, 0x0F
    addrdata[0] = 0x0F;
    addrdata[1] = 0x1F;                                 // Left level
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x10
    addrdata[0] = 0x10;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x11
    addrdata[0] = 0x11;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);
}

void LM49370_IIS_Master_RegSet()
{
    uint8 idata;
    //uint32 i;
    uint8 addrdata[2];
    uint8 readdata[2] = {0, 0};

    SPI_LM49370_Init();

#ifdef LM49370_REG_READ_BACK
    // Select GPIO2 for SPI_OUT
    addrdata[0] = 0x1F;
    addrdata[1] = 0x08;
    SPI_LM49370_WriteByte (addrdata, 2);
#endif

    // Basic Configuration, 0x00
    addrdata[0] = 0x00;
    addrdata[1] = 0x06;                                 // PLL Enalbe
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_M, 0x02
    addrdata[0] = 0x02;
    addrdata[1] = ( (13 * 2) - 1);                      // PLL_M
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N, 0x03
    addrdata[0] = 0x03;
    addrdata[1] = 60;                                   // PLL_N
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N_MOD, 0x05
    addrdata[0] = 0x05;
    addrdata[1] = (PLL_IN_MCLK << 5) | (0x00 << 0);     // PLL_N_MOD = 0/32, PLL_IN = MCLK
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PLL_P, 0x04
    addrdata[0] = 0x04;
    addrdata[1] = (0x6 << 4) | ( ( (5 * 2) - 1) << 0);  // Q_DIV = 6, PLL_P = 9
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // CLOCKS, 0x01
    addrdata[0] = 0x01;
    addrdata[1] = 0x2D;                                 // R_DIV = 11, DAC_CLK_IN = PLL_OUT
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // ADC_2, 0x07
    addrdata[0] = 0x07;
    addrdata[1] = 0x20;                                 // ADC_CLK_IN = PLL_OUT, ADC OverSampling = 125
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // IIS Clock, 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x00;                                 // PCM_SYNC_WIDTH = 1 bit
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Mode, 0x1C
    addrdata[0] = 0x1C;
    addrdata[1] = 0x00;                                 // PCM_CLK = DAC, PCM_IN/OUT = Disable, PCM Slave
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Clock, 0x1D
    addrdata[0] = 0x1D;
    addrdata[1] = 0x56;                                 // PCM_CLK Divided by : 16, SYNC Divided by : 128
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // LS_OUTPUT, 0x14
    addrdata[0] = 0x14;
    addrdata[1] = 0x06;                                 // LEFT, RIGHT Selected
    SPI_LM49370_WriteByte (addrdata, 2);


    // HP_OUTPUT, 0x15
    addrdata[0] = 0x15;
    addrdata[1] = 0x2C;                                 // LEFT, RIGHT Enable
    SPI_LM49370_WriteByte (addrdata, 2);


    // AUX_LEFT, 0x0F
    addrdata[0] = 0x0F;
    addrdata[1] = 0x1F;                                 // Left level
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x10
    addrdata[0] = 0x10;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1A
    addrdata[0] = 0x1A;
    addrdata[1] = 0x17;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x31;                                 // IIS Clock : Master, Clock Source : DAC, Divider : 7.815
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1E
    addrdata[0] = 0x1E;
    addrdata[1] = 0x20;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1F
    addrdata[0] = 0x1F;
    addrdata[1] = 0x00;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x11
    addrdata[0] = 0x11;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);

}

void LM49370_IIS_Master_RegSet_VBC()
{

    uint8 idata;
    //uint32 i;
    uint8 addrdata[2];
    uint8 readdata[2] = {0, 0};

    SPI_LM49370_Init();

#ifdef LM49370_REG_READ_BACK
    // Select GPIO2 for SPI_OUT
    addrdata[0] = 0x1F;
    addrdata[1] = 0x08;
    SPI_LM49370_WriteByte (addrdata, 2);
#endif

    // Basic Configuration, 0x00
    addrdata[0] = 0x00;
    addrdata[1] = 0x06;                                 // PLL Enalbe
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_M, 0x02
    addrdata[0] = 0x02;
    addrdata[1] = ( (13 * 2) - 1);                      // PLL_M
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N, 0x03
    addrdata[0] = 0x03;
    addrdata[1] = 60;                                   // PLL_N
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N_MOD, 0x05
    addrdata[0] = 0x05;
    addrdata[1] = (PLL_IN_MCLK << 5) | (0x00 << 0);     // PLL_N_MOD = 0/32, PLL_IN = MCLK
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PLL_P, 0x04
    addrdata[0] = 0x04;
    addrdata[1] = (0x6 << 4) | ( ( (5 * 2) - 1) << 0);  // Q_DIV = 6, PLL_P = 9
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // CLOCKS, 0x01
    addrdata[0] = 0x01;
    addrdata[1] = 0x2D;                                 // R_DIV = 11, DAC_CLK_IN = PLL_OUT
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // ADC_2, 0x07
    addrdata[0] = 0x07;
    addrdata[1] = 0x20;                                 // ADC_CLK_IN = PLL_OUT, ADC OverSampling = 125
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // IIS Clock, 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x00;                                 // PCM_SYNC_WIDTH = 1 bit
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Mode, 0x1C
    addrdata[0] = 0x1C;
    addrdata[1] = 0x00;                                 // PCM_CLK = DAC, PCM_IN/OUT = Disable, PCM Slave
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Clock, 0x1D
    addrdata[0] = 0x1D;
    addrdata[1] = 0x56;                                 // PCM_CLK Divided by : 16, SYNC Divided by : 128
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // LS_OUTPUT, 0x14
    addrdata[0] = 0x14;
    addrdata[1] = 0x08;                                 // LEFT, RIGHT Selected
    SPI_LM49370_WriteByte (addrdata, 2);


    // HP_OUTPUT, 0x15
    addrdata[0] = 0x15;
    addrdata[1] = 0x2C;                                 // LEFT, RIGHT Enable
    SPI_LM49370_WriteByte (addrdata, 2);


    // AUX_LEFT, 0x0F
    addrdata[0] = 0x0F;
    addrdata[1] = 0x1F;                                 // Left level
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x10
    addrdata[0] = 0x10;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1A
    addrdata[0] = 0x1A;
    addrdata[1] = 0x13;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x31;                                 // IIS Clock : Master, Clock Source : DAC, Divider : 7.815
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1E
    addrdata[0] = 0x1E;
    addrdata[1] = 0x20;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1F
    addrdata[0] = 0x1F;
    addrdata[1] = 0x00;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x11
    addrdata[0] = 0x11;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);


}


void LM49370_PCM_Master_RegSet()
{
    //uint8 idata, temp;
    //uint32 i;
    uint8 addrdata[2];
    //uint8 readdata[2] = {0, 0};

    SPI_LM49370_Init();

    // Basic Configuration, 0x00
    addrdata[0] = 0x00;
    addrdata[1] = 0x06;                                 // PLL Enalbe
    SPI_LM49370_WriteByte (addrdata, 2);

    // PLL_M, 0x02
    addrdata[0] = 0x02;
    addrdata[1] = ( (13 * 2) - 1);                      // PLL_M
    SPI_LM49370_WriteByte (addrdata, 2);

    // PLL_N, 0x03
    addrdata[0] = 0x03;
    addrdata[1] = 60;                                   // PLL_N
    SPI_LM49370_WriteByte (addrdata, 2);

    // PLL_N_MOD, 0x05
    addrdata[0] = 0x05;
    addrdata[1] = (PLL_IN_MCLK << 5) | (0x00 << 0);     // PLL_N_MOD = 0/32, PLL_IN = MCLK
    SPI_LM49370_WriteByte (addrdata, 2);

    // PLL_P, 0x04
    addrdata[0] = 0x04;
    addrdata[1] = (0x6 << 4) | ( ( (5 * 2) - 1) << 0);  // Q_DIV = 6, PLL_P = 9
    SPI_LM49370_WriteByte (addrdata, 2);

    // CLOCKS, 0x01
    addrdata[0] = 0x01;
    addrdata[1] = 0x2D;                                 // R_DIV = 11, DAC_CLK_IN = PLL_OUT
    SPI_LM49370_WriteByte (addrdata, 2);

    // ADC_2, 0x07
    addrdata[0] = 0x07;
    addrdata[1] = 0x20;                                 // ADC_CLK_IN = PLL_OUT, ADC OverSampling = 125
    SPI_LM49370_WriteByte (addrdata, 2);

    // IIS Clock, 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x00;                                 // PCM_SYNC_WIDTH = 1 bit
    SPI_LM49370_WriteByte (addrdata, 2);

    // PCM Mode, 0x1C
    addrdata[0] = 0x1C;
    addrdata[1] = 0x17;                                 // PCM_CLK = DAC, PCM_IN/OUT = Enable, PCM Master
    SPI_LM49370_WriteByte (addrdata, 2);

    // PCM Clock, 0x1D
    addrdata[0] = 0x1D;
    addrdata[1] = 0x56;                                 // PCM_CLK Divided by : 16, SYNC Divided by : 128
    SPI_LM49370_WriteByte (addrdata, 2);
}

void LM49370_IIS_Master_RegSet_ADC()
{

    uint8 idata;
    //uint32 i;
    uint8 addrdata[2];
    uint8 readdata[2] = {0, 0};

    SPI_LM49370_Init();

#ifdef LM49370_REG_READ_BACK
    // Select GPIO2 for SPI_OUT
    addrdata[0] = 0x1F;
    addrdata[1] = 0x08;
    SPI_LM49370_WriteByte (addrdata, 2);
#endif

    // Basic Configuration, 0x00
    addrdata[0] = 0x00;
    addrdata[1] = 0x06;                                 // PLL Enalbe
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_M, 0x02
    addrdata[0] = 0x02;
    addrdata[1] = ( (13 * 2) - 1);                      // PLL_M
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N, 0x03
    addrdata[0] = 0x03;
    addrdata[1] = 60;                                   // PLL_N
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);
#endif

    // PLL_N_MOD, 0x05
    addrdata[0] = 0x05;
    addrdata[1] = (PLL_IN_MCLK << 5) | (0x00 << 0);     // PLL_N_MOD = 0/32, PLL_IN = MCLK
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PLL_P, 0x04
    addrdata[0] = 0x04;
    addrdata[1] = (0x6 << 4) | ( ( (5 * 2) - 1) << 0);  // Q_DIV = 6, PLL_P = 9
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // ADC_1, 0x06
    addrdata[0] = 0x06;
    addrdata[1] = 0x01;                                 // MIC_SELECT
    SPI_LM49370_WriteByte (addrdata, 2);

    // CLOCKS, 0x01
    addrdata[0] = 0x01;
    addrdata[1] = 0x2D;                                 // R_DIV = 11, DAC_CLK_IN = PLL_OUT
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // ADC_2, 0x07
    addrdata[0] = 0x07;
    addrdata[1] = 0x20;                                 // ADC_CLK_IN = PLL_OUT, ADC OverSampling = 125
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // IIS Clock, 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x31;                                 // PCM_SYNC_WIDTH = 1 bit
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Mode, 0x1C
    addrdata[0] = 0x1C;
    addrdata[1] = 0x00;                                 // PCM_CLK = DAC, PCM_IN/OUT = Disable, PCM Slave
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // PCM Clock, 0x1D
    addrdata[0] = 0x1D;
    addrdata[1] = 0x56;                                 // PCM_CLK Divided by : 16, SYNC Divided by : 128
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK

#endif

    // LS_OUTPUT, 0x14
    addrdata[0] = 0x14;
    addrdata[1] = 0x06;                                 // LEFT, RIGHT Selected
    SPI_LM49370_WriteByte (addrdata, 2);


    // HP_OUTPUT, 0x15
    addrdata[0] = 0x15;
    addrdata[1] = 0x2C;                                 // LEFT, RIGHT Enable
    SPI_LM49370_WriteByte (addrdata, 2);


    // AUX_LEFT, 0x0F
    addrdata[0] = 0x0F;
    addrdata[1] = 0x1F;                                 // Left level
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x10
    addrdata[0] = 0x10;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1A
    addrdata[0] = 0x1A;
    addrdata[1] = 0x17;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = 0x31;                                 // IIS Clock : Master, Clock Source : DAC, Divider : 7.815
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1E
    addrdata[0] = 0x1E;
    addrdata[1] = 0x00;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x1F
    addrdata[0] = 0x1F;
    addrdata[1] = 0x00;                                 //
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x11
    addrdata[0] = 0x11;
    addrdata[1] = 0x1F;                                 // Right level
    SPI_LM49370_WriteByte (addrdata, 2);

    // 0x0B
    addrdata[0] = 0x0B;
    addrdata[1] = 0x4F;                                 // EXT MIC, Maximum
    SPI_LM49370_WriteByte (addrdata, 2);
}

void LM49370_Config_Sample()
{
    INTERFACE_PROPERTY InterfacePro;
    IIS_CONFIG IisConfig;
    PCM_CONFIG PcmConfig;

    InterfacePro.pIisConfig = &IisConfig;
    InterfacePro.pPcmConfig = &PcmConfig;

    /////////////////////////////////////////////
    // Configuration of Codec
    //
    //InterfacePro.Interface = INTERFACE_PCM;
    InterfacePro.SampleRate = SAM_8000;

    // ----- InterfacePro.pIisConfig
    InterfacePro.pIisConfig->IisWSMode = BITS_PER_WORD_16;
    InterfacePro.pIisConfig->IisGrouping = IIS_GROUPING_RL;

    InterfacePro.pIisConfig->IisInputEnable = TRUE;
    InterfacePro.pIisConfig->IisOutputEnable = TRUE;

    InterfacePro.pIisConfig->IisMode = IIS_MODE_LEFT_JUSTIFIED;

    InterfacePro.pIisConfig->IisClockMaster = TRUE;
    InterfacePro.pIisConfig->IisWSMaster = TRUE;

    InterfacePro.pIisConfig->IntOSCEnable = FALSE;              // Default = FALSE
    InterfacePro.pIisConfig->IisStereoReverse = FALSE;              // Default = FALSE

    // ----- InterfacePro.pPcmConfig
    InterfacePro.pPcmConfig->PcmBitPerChannel = PCM_BIT_16;

    InterfacePro.pPcmConfig->PcmClockMaster = TRUE;
    InterfacePro.pPcmConfig->PcmSyncMaster = TRUE;

    InterfacePro.pPcmConfig->PcmInputEnable = TRUE;
    InterfacePro.pPcmConfig->PcmOutputEnable = TRUE;

    InterfacePro.pPcmConfig->PcmSyncWidth = PCM_SYNC_WIDTH_1BIT;
    // InterfacePro.pPcmConfig->PcmSdoLSBHZEnable = FALSE;
    // InterfacePro.pPcmConfig->PcmMode = PCM_MODE_ALAW
    // InterfacePro.pPcmConfig->PcmCompandOn = FALSE;

    // ----- InterfacePro.BasicConfig
    InterfacePro.BasicConfig.ChipMode = ACTIVE_WTO_HP_DET;

    InterfacePro.BasicConfig.AUXLeft = VOLUME_LEVEL_MAX;
    InterfacePro.BasicConfig.AUXRight = VOLUME_LEVEL_MAX;
    InterfacePro.BasicConfig.DACLevel = VOLUME_LEVEL_MAX;

    InterfacePro.BasicConfig.LeftSrc = LEFT_SRC_DAC;
    InterfacePro.BasicConfig.RightSrc = RIGHT_SRC_DAC;

    InterfacePro.BasicConfig.EnableLS = TRUE;
    InterfacePro.BasicConfig.EnableHP = TRUE;
    InterfacePro.BasicConfig.ADCEnable = TRUE;

    InterfacePro.BasicConfig.StereoModeEnable = TRUE;

    // ----- InterfacePro.HeadsetMic
    InterfacePro.HeadsetMic.MICSelct = MIC_SEL_EXT;

    // ----- Bridge Configurations
    InterfacePro.IisPcmBridge.DacInputSel = DAC_INPUT_SEL_IIS_RECEIVER;


}

PUBLIC uint32 __codec_dp_dac_fs_mode_calc(uint32 sample_rate,uint32 *fs_mode)
{
    uint32 sample_rate_set;
    uint32 fs_mode_ret =0;

    // 6000, 8000,11025,12000,16000,22050,24000,32000,44100,48000
    if (sample_rate < 6600)
    {
        sample_rate_set = 6000;
    }
    else if (sample_rate < 8800)
    {
        sample_rate_set = 8000;
    }
    else if (sample_rate < 11512)
    {
        sample_rate_set = 11025;
    }
    else if (sample_rate  < 14000)
    {
        sample_rate_set = 12000;
    }
    else if (sample_rate < 19025)
    {
        sample_rate_set = 16000;
    }
    else if (sample_rate < 23025)
    {
        sample_rate_set = 22050;
    }
    else if (sample_rate < 28000)
    {
        sample_rate_set = 24000;
    }
    else if (sample_rate < 38050)
    {
        sample_rate_set = 32000;
    }
    else if (sample_rate < 46050)
    {
        sample_rate_set = 44100;
    }
    else if (sample_rate < 72000)
    {
        sample_rate_set = 48000;
    }
    else
    {
        sample_rate_set = 8000;
    }
    if( fs_mode )
    {
        *fs_mode = fs_mode_ret;
    }
    return sample_rate_set;
}




uint8 LM49370_SetMode (PINTERFACE_PROPERTY pInterface)
{

    uint8 idata;
    //uint32 i;
    uint8 addrdata[2];
    uint8 readdata[2] = {0, 0};

    ////////////////////////////////////////////////
    //  Defination of variable
    //////

    // ----- Clock relationed configurations
    uint8 PLL_M = 0, PLL_N = 0, PLL_N_MOD = 0, PLL_P = 0;
    uint8 R_DIV = 0, Q_DIV = 0;
    uint8 IIS_CLK_DIV = 0;              // 0x1B, bit5:2
    uint8 PCM_CLK_DIV = 0;              // 0x1D, bit3:0
    uint8 PCM_SYN_DIV = 0;              // 0x1D, bit6:4
    BOOLEAN PllEnable = TRUE;               // 0x00, bit2, default = 1
    DAC_OVERSAMPLING DacOverSampling = DAC_OVERSAM_125;               // 0x00, bit7:6
    ADC_OVERSAMPLING AdcOverSamling = ADC_OVERSAM_125;                // 0x07, bit0
    CLK_SRC ADCClkSrc = PLL_OUTPUT;             // 0x07, bit6:5, default = PLL_OUTPUT
    CLK_SRC DACClkSrc = PLL_OUTPUT;             // 0x01, bit1:0, default = PLL_OUTPUT

    // ----- Bridge Configurations
    PCM_TX_SEL PcmTxSel;            // 0x1E, bit0
    //BOOLEAN AdcSrcMode = FALSE;         // 0x1F, bit6, default = 0
    //BOOLEAN DacSrcMode = FALSE;         // 0x1F, bit7, default = 0

    IIS_TX_SEL IisTxSel;                // 0x1E, bit2:1
    DAC_INPUT_SEL DacInputSel;      // 0x1E, bit4:3

    MONO_SUM_SEL MonoSumSel = MONO_SUM_SEL_DAC;     // 0x1E, bit5, default = 0
    MONO_SUM_MODE MonoSum = MONO_SUM_MODE_LR;       // 0x1E, bit7:6, default = 0x00

    // ----- Interface Configurations
    // IIS
    BOOLEAN IisOutputEnable;        // 0x1A, bit0
    BOOLEAN IisInputEnable;         // 0x1A, bit1
    BOOLEAN IisClockMaster;         // 0x1B, bit0
    BOOLEAN IisWSMaster;            // 0x1A, bit4
    BOOLEAN IntOSCEnable;           // 0x00, bit3
    BOOLEAN IisStereoReverse;       // 0x1A, bit3

    IIS_GROUPING IisGrouping;       // 0x1A, bit7
    IIS_WS_MODE IisWSMode;          // 0x1A, bit6:5
    IIS_CLK_SRC IisClkSrc = IIS_CLK_SRC_DAC;          // 0x1B, bit1
    //IIS_BIT_PER_CHANNAL IisBitPerChannel;
    IIS_MODE IisMode;               // 0x1A, bit2

    // PCM
    BOOLEAN PcmOutputEnable;        // 0x1C, bit0
    BOOLEAN PcmInputEnable;     // 0x1C, bit1
    BOOLEAN PcmClockMaster;     // 0x1C, bit2
    BOOLEAN PcmSyncMaster;          // 0x1C, bit4
    BOOLEAN PcmCompandOn = FALSE;           // 0x1C, bit6, default = 0
    BOOLEAN PcmSdoLSBHZEnable = FALSE;      // 0x1C, bit5

    PCM_CLK_SRC PcmClkSrc = PCM_CLK_SRC_ADC;          // 0x1C, bit3
    PCM_SYNC_WIDTH PcmSyncWidth;    // 0x1B, bit7:6
    //PCM_BIT_PER_CHANNEL PcmBitPerChannel;
    PCM_MODE PcmMode = PCM_MODE_uLAW;               // 0x1C, bit7, default = alaw

    // ----- HeadSet/MIC Configurations
    BOOLEAN HPOCLMode = FALSE;                  // 0x15, bit6, default = 0
    OCL_VCM_VOLTAGE OCLVCMVoltage = OCL_VCM_VOLTAGE_12; // 0x0C, bit0, default = 0

    MIC_BIAS_VOLTAGE MicBiasVoltage = MIC_BIAS_VOLTAGE_20;  // 0x0C, bit2:1, default = 0
    MIC_SELECT MICSelct;                    // 0x0B, bit6

    INTERNAL_MIC InternalMic = INT_MIC_DIFF;            // 0x0B, bit5, default = 0

    // ----- Basic Configurations
    CHIP_MODE ChipMode;             // 0x00, bit1:0
    BYPASS_CAP_SIZE BypassCapSize = BYPASS_CAP_SIZE_01UF;       // 0x00, bit5:4

    BOOLEAN StereoModeEnable = TRUE;            // 0x15, bit5

    LEFT_SRC LeftSrc;                   // 0x0F, bit7
    RIGHT_SRC RightSrc;                 // 0x10, bit7

    HP_OUTPUT HpOutput;
    LS_OUTPUT LsOutput;
    ADC_INPUT AdcInput;

    VOLUME_CONTROL AUXLeft;         // 0x0F, bit4:0
    VOLUME_CONTROL AUXRight;            // 0x10, bit4:0
    VOLUME_CONTROL DACLevel;            // 0x11, bit4:0

    ////////////////////////////////////////////////
    //  Evaluation of variable
    //////
    // ----- Interface Configurations
    // IIS
    IisOutputEnable = pInterface->pIisConfig->IisOutputEnable;
    IisInputEnable = pInterface->pIisConfig->IisInputEnable;
    IisClockMaster = pInterface->pIisConfig->IisClockMaster;
    IisWSMaster = pInterface->pIisConfig->IisWSMaster;
    IntOSCEnable = pInterface->pIisConfig->IntOSCEnable;
    IisStereoReverse = pInterface->pIisConfig->IisStereoReverse;

    IisGrouping = pInterface->pIisConfig->IisGrouping;
    IisWSMode = pInterface->pIisConfig->IisWSMode;
    IisMode = pInterface->pIisConfig->IisMode;

    // PCM
    PcmOutputEnable = pInterface->pPcmConfig->PcmOutputEnable;
    PcmInputEnable = pInterface->pPcmConfig->PcmInputEnable;
    PcmClockMaster = pInterface->pPcmConfig->PcmClockMaster;
    PcmSyncMaster = pInterface->pPcmConfig->PcmSyncMaster;
    PcmSyncWidth = pInterface->pPcmConfig->PcmSyncWidth;

    // ----- HeadSet/MIC Configurations
    MICSelct = pInterface->HeadsetMic.MICSelct;

    // ----- Bridge Configurations
    PcmTxSel = PCM_TX_SEL_ADC;

    //IisTxSel = IIS_TX_SEL_DISABLE;
    //ken.kuang modify.
    IisTxSel = IIS_TX_SEL_ADC;

    DacInputSel = pInterface->IisPcmBridge.DacInputSel;

    // ----- Basic configuration
    StereoModeEnable = pInterface->BasicConfig.StereoModeEnable;

    ChipMode = pInterface->BasicConfig.ChipMode;
    LeftSrc = pInterface->BasicConfig.LeftSrc;
    RightSrc = pInterface->BasicConfig.RightSrc;

    AUXLeft = pInterface->BasicConfig.AUXLeft;
    AUXRight = pInterface->BasicConfig.AUXRight;
    DACLevel = pInterface->BasicConfig.DACLevel;

    if (pInterface->BasicConfig.EnableHP)
    {
        HpOutput.LeftSel = TRUE;
        HpOutput.RightSel = TRUE;

        HpOutput.CPISel = FALSE;
        HpOutput.HpMute = FALSE;
        HpOutput.SideToneSel = FALSE;
    }
    else
    {
        HpOutput.LeftSel = FALSE;
        HpOutput.RightSel = FALSE;

        HpOutput.CPISel = FALSE;
        HpOutput.HpMute = TRUE;
        HpOutput.SideToneSel = FALSE;
    }

    if (pInterface->BasicConfig.EnableLS)
    {
        LsOutput.LeftSel = TRUE;
        LsOutput.RightSel = TRUE;

        LsOutput.CPISel = FALSE;
        LsOutput.LsMute = FALSE;
    }
    else
    {
        LsOutput.LeftSel = FALSE;
        LsOutput.RightSel = FALSE;

        LsOutput.CPISel = FALSE;
        LsOutput.LsMute = TRUE;
    }

    if (pInterface->BasicConfig.ADCEnable)
    {
        AdcInput.MICSel = TRUE;

        AdcInput.ADCMute = FALSE;
        AdcInput.CPISel = FALSE;
        AdcInput.LeftSel = FALSE;
        AdcInput.RightSel = FALSE;
    }
    else
    {
        AdcInput.MICSel = FALSE;

        AdcInput.ADCMute = FALSE;
        AdcInput.CPISel = FALSE;
        AdcInput.LeftSel = FALSE;
        AdcInput.RightSel = FALSE;
    }

    /*****************************************/

    // Clock configuration
    switch (pInterface->SampleRate)
    {
        case SAM_44100:
        case SAM_11025:
        case SAM_22050:

            if (pInterface->pIisConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = (13 * 2) - 1;
                PLL_N = 55;
                PLL_P = (5 * 2) -  1;

                PLL_N_MOD = 4;

                if (pInterface->SampleRate == SAM_44100)
                {
                    R_DIV = (1 * 2) - 1;            // Divider for DAC Clock, DAC_CLK = 11.025Mhz
                    Q_DIV = 0;                      // Divider for ADC Clock, Divide value : 2, ADC_CLK = 5.5125MHz
                }
                else if (pInterface->SampleRate == SAM_22050)
                {
                    R_DIV = (2 * 2) - 1;            // Divider for DAC Clock, DAC_CLK = 5.513Mhz
                    Q_DIV = 2;                      // Divider for ADC Clock, Divide value : 4, ADC_CLK = 2.756MHz
                }
                else if (pInterface->SampleRate == SAM_11025)
                {
                    R_DIV = (4 * 2) - 1;            // Divider for DAC Clock, DAC_CLK = 2.756Mhz
                    Q_DIV = 4;                      // Divider for ADC Clock, Divide value : 8, ADC_CLK = 1.378MHz
                }

                DacOverSampling = DAC_OVERSAM_125;      // 0x00, bit7:6, DAC SampleRate = 44.1KHz
                AdcOverSamling = ADC_OVERSAM_125;       // 0x07, bit0, ADC SampleRate = 44.1KHz

                // ----- In case of Master
                if (pInterface->pIisConfig->IisClockMaster)
                {
                    switch (pInterface->pIisConfig->IisWSMode)
                    {
                        case BITS_PER_WORD_25:

                            //SCI_TraceLow:"Do not support 25bit per channel in IIS mode!(44.1KHz)\r\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1361_112_2_17_23_2_49_160,(uint8*)"");
                            return LM49370_ERR_INVALID_PARA;

                            //break;

                        case BITS_PER_WORD_16:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0xC;      // 0x1B, bit5:2, Divider = 7.8125

                            break;

                        case BITS_PER_WORD_32:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0xA;      // 0x1B, bit5:2, Divider = 3.90625

                            break;

                        default:
                            break;
                    }
                }
            }

            if (pInterface->pPcmConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = (13 * 2) - 1;
                PLL_N = 55;
                PLL_P = (5 * 2) -  1;

                PLL_N_MOD = 4;

                //  R_DIV = (1 * 2) - 1;            // Divider for DAC Clock, DAC_CLK = 11.025Mhz
                //  Q_DIV = 0;                      // Divider for ADC Clock, Divide value : 2, ADC_CLK = 5.5125MHz

                DacOverSampling = DAC_OVERSAM_125;      // 0x00, bit7:6, DAC SampleRate = 44.1KHz
                AdcOverSamling = ADC_OVERSAM_125;       // 0x07, bit0, ADC SampleRate = 44.1KHz

                // ----- In case of Master
                if (pInterface->pPcmConfig->PcmClockMaster)
                {
                    switch (pInterface->pPcmConfig->PcmBitPerChannel)
                    {
                        case PCM_BIT_8:

                            //SCI_TraceLow:"Codec LM49370 do not support 8bit per channel in PCM mode!(44.1KHz)\r\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1408_112_2_17_23_2_49_161,(uint8*)"");
                            return LM49370_ERR_INVALID_PARA;

                            //break;

                        case PCM_BIT_16:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0xC;              // 0x1D, bit3:0, Divider = 7.8125
                            PCM_SYN_DIV = 0x1;              // 0x1D, bit6:4, mono = 16

                            break;

                        case PCM_BIT_32:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0xA;              // 0x1D, bit3:0, Divider = 3.90625
                            PCM_SYN_DIV = 0x3;              // 0x1D, bit6:4, mono = 32

                            break;

                        default:
                            break;
                    }
                }
            }

            break;

        case SAM_8000:

            if (pInterface->pIisConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = 52;//(26.5 * 2) - 1;
                PLL_N = 112;
                PLL_P = 8;//(4.5 * 2) -  1;

                PLL_N_MOD = 23;             // PLL_OUT

                R_DIV = (6 * 2) - 1;            // Divider for DAC Clock, DAC_CLK = 2.048Mhz
                Q_DIV = 6;                      // Divider for ADC Clock, Divide value : 12, ADC_CLK = 1.024MHz

                DacOverSampling = DAC_OVERSAM_128;      // 0x00, bit7:6
                AdcOverSamling = ADC_OVERSAM_128;       // 0x07, bit0

                // ----- In case of Master
                if (pInterface->pIisConfig->IisClockMaster)
                {
                    switch (pInterface->pIisConfig->IisWSMode)
                    {
                        case BITS_PER_WORD_25:

                            //SCI_TraceLow:"Do not support 25bit per channel in IIS mode!\r\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1461_112_2_17_23_2_49_162,(uint8*)"");
                            return LM49370_ERR_INVALID_PARA;

                            //break;

                        case BITS_PER_WORD_16:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0x2;      // 0x1B, bit5:2, Divider = 8

                            break;

                        case BITS_PER_WORD_32:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0x1;      // 0x1B, bit5:2, Divider = 4

                            break;

                        default:
                            break;
                    }
                }

                /*
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                    PLL_M = (13 * 2) - 1;
                    PLL_N = 60;
                    PLL_P = (5 * 2) -  1;
                    PLL_N_MOD = 0;              // PLL_OUT = 12MHz

                    R_DIV = (6 * 2) - 1;            // Divider for DAC Clock, Divide value = 6, DAC_CLK = 2Mhz
                    Q_DIV = 6;                      // Divider for ADC Clock, Divide value = 12, ADC_CLK = 1MHz

                    DacOverSampling = DAC_OVERSAM_125;      // 0x00, bit7:6, DAC SampleRate = 8KHz
                    AdcOverSamling = ADC_OVERSAM_125;       // 0x07, bit0, ADC SampleRate = 8KHz

                // ----- In case of Master
                    if(pInterface->pIisConfig->IisClockMaster)
                    {
                        switch(pInterface->pIisConfig->IisWSMode)
                        {
                            case BITS_PER_WORD_25:

                                //SCI_TraceLow:"Do not support 25bit per channel in IIS mode!\r\n"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1505_112_2_17_23_2_49_163,(uint8*)"");
                                return LM49370_ERR_INVALID_PARA;

                                break;

                            case BITS_PER_WORD_16:

                                IisClkSrc = IIS_CLK_SRC_DAC;
                                IIS_CLK_DIV = 0xC;      // 0x1B, bit5:2, Divider = 7.8125

                                break;

                            case BITS_PER_WORD_32:

                                IisClkSrc = IIS_CLK_SRC_DAC;
                                IIS_CLK_DIV = 0xA;      // 0x1B, bit5:2, Divider = 3.90625

                                break;
                        }
                    }
                */

            }

            if (pInterface->pPcmConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = 52;//(26.5 * 2) - 1;
                PLL_N = 112;
                PLL_P = 8;//(4.5 * 2) -  1;

                PLL_N_MOD = 23;             // PLL_OUT

                //  R_DIV = (6 * 2) - 1;            // Divider for DAC Clock, DAC_CLK = 2.048Mhz
                //  Q_DIV = 6;                      // Divider for ADC Clock, Divide value : 12, ADC_CLK = 1.024MHz

                DacOverSampling = DAC_OVERSAM_128;      // 0x00, bit7:6
                AdcOverSamling = ADC_OVERSAM_128;       // 0x07, bit0

                // ----- In case of Master
                if (pInterface->pPcmConfig->PcmClockMaster)
                {
                    switch (pInterface->pPcmConfig->PcmBitPerChannel)
                    {
                        case PCM_BIT_8:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x6;              // 0x1D, bit3:0, Divider = 16
                            PCM_SYN_DIV = 0x0;              // 0x1D, bit6:4, mono = 8

                            break;

                        case PCM_BIT_16:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x4;              // 0x1D, bit3:0, Divider = 8
                            PCM_SYN_DIV = 0x1;              // 0x1D, bit6:4, mono = 16

                            break;

                        case PCM_BIT_32:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x2;              // 0x1D, bit3:0, Divider = 4
                            PCM_SYN_DIV = 0x3;              // 0x1D, bit6:4, mono = 32

                            break;

                        case PCM_BIT_128:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x0;              // 0x1D, bit3:0, Divider = 1
                            PCM_SYN_DIV = 0x5;              // 0x1D, bit6:4, mono = 128

                            break;

                        default:
                            break;
                    }
                }
            }

            break;
#if 0
        case SAM_6000:

            if (pInterface->pIisConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = 52;//(26.5 * 2) - 1;
                PLL_N = 112;
                PLL_P = 8;//(4.5 * 2) -  1;

                PLL_N_MOD = 23;             // PLL_OUT

                R_DIV = (8 * 2) - 1;            // Divider for DAC Clock, DAC_CLK = 1.536Mhz
                Q_DIV = 6;                      // Divider for ADC Clock, Divide value : 12, ADC_CLK = 1.024MHz

                DacOverSampling = DAC_OVERSAM_128;      // 0x00, bit7:6
                AdcOverSamling = ADC_OVERSAM_128;       // 0x07, bit0

                // ----- In case of Master
                if (pInterface->pIisConfig->IisClockMaster)
                {
                    switch (pInterface->pIisConfig->IisWSMode)
                    {
                        case BITS_PER_WORD_25:

                            //SCI_TraceLow:"Do not support 25bit per channel in IIS mode!\r\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1613_112_2_17_23_2_49_164,(uint8*)"");
                            return LM49370_ERR_INVALID_PARA;

                            break;

                        case BITS_PER_WORD_16:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0x2;      // 0x1B, bit5:2, Divider = 8

                            break;

                        case BITS_PER_WORD_32:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0x1;      // 0x1B, bit5:2, Divider = 4

                            break;
                    }
                }

            }

            if (pInterface->pPcmConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = 52;//(26.5 * 2) - 1;
                PLL_N = 112;
                PLL_P = 8;//(4.5 * 2) -  1;

                PLL_N_MOD = 23;             // PLL_OUT

                R_DIV = (8 * 2) - 1;            // Divider for DAC Clock, DAC_CLK = 2.048Mhz
                Q_DIV = 6;                      // Divider for ADC Clock, Divide value : 12, ADC_CLK = 1.024MHz

                DacOverSampling = DAC_OVERSAM_128;      // 0x00, bit7:6
                AdcOverSamling = ADC_OVERSAM_128;       // 0x07, bit0

                // ----- In case of Master
                if (pInterface->pPcmConfig->PcmClockMaster)
                {
                    switch (pInterface->pPcmConfig->PcmBitPerChannel)
                    {
                        case PCM_BIT_8:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x6;              // 0x1D, bit3:0, Divider = 16
                            PCM_SYN_DIV = 0x0;              // 0x1D, bit6:4, mono = 8

                            break;

                        case PCM_BIT_16:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x4;              // 0x1D, bit3:0, Divider = 8
                            PCM_SYN_DIV = 0x1;              // 0x1D, bit6:4, mono = 16

                            break;

                        case PCM_BIT_32:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x2;              // 0x1D, bit3:0, Divider = 4
                            PCM_SYN_DIV = 0x3;              // 0x1D, bit6:4, mono = 32

                            break;

                        case PCM_BIT_128:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x0;              // 0x1D, bit3:0, Divider = 1
                            PCM_SYN_DIV = 0x5;              // 0x1D, bit6:4, mono = 128

                            break;
                    }
                }
            }

            break;
#endif
        case SAM_16000:

            if (pInterface->pIisConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = 52;//(26.5 * 2) - 1;
                PLL_N = 112;
                PLL_P = 8;//(4.5 * 2) -  1;

                PLL_N_MOD = 23;             // PLL_OUT

                R_DIV = 5;          // Divider for DAC Clock, DAC_CLK = 4.096Mhz
                Q_DIV = 3;                      // Divider for ADC Clock, Divide value : 6, ADC_CLK = 2.048MHz

                DacOverSampling = DAC_OVERSAM_128;      // 0x00, bit7:6, DAC SampleRate = 32KHz
                AdcOverSamling = ADC_OVERSAM_128;       // 0x07, bit0, DAC SampleRate = 32KHz

                // ----- In case of Master
                if (pInterface->pIisConfig->IisClockMaster)
                {
                    switch (pInterface->pIisConfig->IisWSMode)
                    {
                        case BITS_PER_WORD_25:

                            //SCI_TraceLow:"Do not support 25bit per channel in IIS mode!(16KHz)\r\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1717_112_2_17_23_2_50_165,(uint8*)"");
                            return LM49370_ERR_INVALID_PARA;

                            //break;

                        case BITS_PER_WORD_16:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0x2;      // 0x1B, bit5:2, Divider = 8

                            break;

                        case BITS_PER_WORD_32:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0x1;      // 0x1B, bit5:2, Divider = 4

                            break;

                        default:
                            break;
                    }
                }
            }

            if (pInterface->pPcmConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = 52;//(26.5 * 2) - 1;
                PLL_N = 112;
                PLL_P = 8;//(4.5 * 2) -  1;

                PLL_N_MOD = 23;             // PLL_OUT

                //  R_DIV = 5;          // Divider for DAC Clock, DAC_CLK = 4.096Mhz
                //  Q_DIV = 3;                      // Divider for ADC Clock, Divide value : 6, ADC_CLK = 2.048MHz

                DacOverSampling = DAC_OVERSAM_128;      // 0x00, bit7:6, DAC SampleRate = 32KHz
                AdcOverSamling = ADC_OVERSAM_128;       // 0x07, bit0, DAC SampleRate = 32KHz

                // ----- In case of Master
                if (pInterface->pPcmConfig->PcmClockMaster)
                {
                    switch (pInterface->pPcmConfig->PcmBitPerChannel)
                    {
                        case PCM_BIT_8:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x6;              // 0x1D, bit3:0, Divider = 16
                            PCM_SYN_DIV = 0x0;              // 0x1D, bit6:4, mono = 8

                            break;

                        case PCM_BIT_16:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x4;              // 0x1D, bit3:0, Divider = 8
                            PCM_SYN_DIV = 0x1;              // 0x1D, bit6:4, mono = 16

                            break;

                        case PCM_BIT_32:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x2;              // 0x1D, bit3:0, Divider = 4
                            PCM_SYN_DIV = 0x3;              // 0x1D, bit6:4, mono = 32

                            break;

                        case PCM_BIT_128:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x0;              // 0x1D, bit3:0, Divider = 1
                            PCM_SYN_DIV = 0x5;              // 0x1D, bit6:4, mono = 128

                            break;

                        default:
                            break;
                    }
                }
            }

            break;

        case SAM_32000:

            if (pInterface->pIisConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = 52;//(26.5 * 2) - 1;
                PLL_N = 112;
                PLL_P = 8;//(4.5 * 2) -  1;

                PLL_N_MOD = 23;             // PLL_OUT

                R_DIV = 2;          // Divider for DAC Clock, DAC_CLK = 8.192Mhz
                Q_DIV = 1;                      // Divider for ADC Clock, Divide value : 3, ADC_CLK = 4.096MHz

                DacOverSampling = DAC_OVERSAM_128;      // 0x00, bit7:6, DAC SampleRate = 32KHz
                AdcOverSamling = ADC_OVERSAM_128;       // 0x07, bit0, DAC SampleRate = 32KHz

                // ----- In case of Master
                if (pInterface->pIisConfig->IisClockMaster)
                {
                    switch (pInterface->pIisConfig->IisWSMode)
                    {
                        case BITS_PER_WORD_25:

                            //SCI_TraceLow:"Do not support 25bit per channel in IIS mode!(32KHz)\r\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1826_112_2_17_23_2_50_166,(uint8*)"");
                            return LM49370_ERR_INVALID_PARA;

                            //break;

                        case BITS_PER_WORD_16:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0x2;      // 0x1B, bit5:2, Divider = 8

                            break;

                        case BITS_PER_WORD_32:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0x1;      // 0x1B, bit5:2, Divider = 4

                            break;

                        default:
                            break;
                    }
                }
            }

            if (pInterface->pPcmConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = 52;//(26.5 * 2) - 1;
                PLL_N = 112;
                PLL_P = 8;//(4.5 * 2) -  1;

                PLL_N_MOD = 23;             // PLL_OUT

                //  R_DIV = 2;          // Divider for DAC Clock, DAC_CLK = 8.192Mhz
                //  Q_DIV = 1;                      // Divider for ADC Clock, Divide value : 3, ADC_CLK = 4.096MHz

                DacOverSampling = DAC_OVERSAM_128;      // 0x00, bit7:6, DAC SampleRate = 32KHz
                AdcOverSamling = ADC_OVERSAM_128;       // 0x07, bit0, DAC SampleRate = 32KHz

                // ----- In case of Master
                if (pInterface->pPcmConfig->PcmClockMaster)
                {
                    switch (pInterface->pPcmConfig->PcmBitPerChannel)
                    {
                        case PCM_BIT_8:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x6;              // 0x1D, bit3:0, Divider = 16
                            PCM_SYN_DIV = 0x0;              // 0x1D, bit6:4, mono = 8

                            break;

                        case PCM_BIT_16:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x4;              // 0x1D, bit3:0, Divider = 8
                            PCM_SYN_DIV = 0x1;              // 0x1D, bit6:4, mono = 16

                            break;

                        case PCM_BIT_32:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0x2;              // 0x1D, bit3:0, Divider = 4
                            PCM_SYN_DIV = 0x3;              // 0x1D, bit6:4, mono = 32

                            break;

                        default:
                            break;
                    }
                }
            }

            break;

        case SAM_48000:
        case SAM_24000:
        case SAM_12000:

            if (pInterface->pIisConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = (13 * 2) - 1;
                PLL_N = 60;
                PLL_P = (5 * 2) -  1;
                PLL_N_MOD = 0;              // PLL_OUT = 12MHz

                if (pInterface->SampleRate == SAM_48000)
                {
                    R_DIV = (1 * 2) - 1;            // Divider for DAC Clock, Divide value = 1, DAC_CLK = 12Mhz
                    Q_DIV = 0;                      // Divider for ADC Clock, Divide value = 2, ADC_CLK = 6MHz
                }
                else if (pInterface->SampleRate == SAM_24000)
                {
                    R_DIV = (2 * 2) - 1;            // Divider for DAC Clock, Divide value = 1, DAC_CLK = 12Mhz
                    Q_DIV = 2;                      // Divider for ADC Clock, Divide value = 4, ADC_CLK = 3MHz
                }
                else if (pInterface->SampleRate == SAM_12000)
                {
                    R_DIV = (4 * 2) - 1;            // Divider for DAC Clock, Divide value = 1, DAC_CLK = 12Mhz
                    Q_DIV = 4;                      // Divider for ADC Clock, Divide value = 8, ADC_CLK = 1.5MHz
                }

                DacOverSampling = DAC_OVERSAM_125;      // 0x00, bit7:6, DAC SampleRate = 48KHz
                AdcOverSamling = ADC_OVERSAM_125;       // 0x07, bit0, ADC SampleRate = 48KHz

                // ----- In case of Master
                if (pInterface->pIisConfig->IisClockMaster)
                {
                    switch (pInterface->pIisConfig->IisWSMode)
                    {
                        case BITS_PER_WORD_25:

                            //SCI_TraceLow:"Do not support 25bit per channel in IIS mode!(48KHz)\r\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1941_112_2_17_23_2_50_167,(uint8*)"");
                            return LM49370_ERR_INVALID_PARA;

                            //break;

                        case BITS_PER_WORD_16:

                            IisClkSrc = IIS_CLK_SRC_ADC;
                            IIS_CLK_DIV = 0xA;      // 0x1B, bit5:2, Divider = 3.90625

                            break;

                        case BITS_PER_WORD_32:

                            IisClkSrc = IIS_CLK_SRC_DAC;
                            IIS_CLK_DIV = 0xA;      // 0x1B, bit5:2, Divider = 3.90625

                            break;

                        default:
                            break;
                    }
                }
            }

            if (pInterface->pPcmConfig != NULL)
            {
                // ----- In case of Slave, only need to config DAC&ADC sample rate
                PLL_M = (13 * 2) - 1;
                PLL_N = 60;
                PLL_P = (5 * 2) -  1;
                PLL_N_MOD = 0;              // PLL_OUT = 12MHz

                //  R_DIV = (1 * 2) - 1;            // Divider for DAC Clock, Divide value = 1, DAC_CLK = 12Mhz
                //  Q_DIV = 0;                      // Divider for ADC Clock, Divide value = 2, ADC_CLK = 6MHz

                DacOverSampling = DAC_OVERSAM_125;      // 0x00, bit7:6, DAC SampleRate = 48KHz
                AdcOverSamling = ADC_OVERSAM_125;       // 0x07, bit0, ADC SampleRate = 48KHz

                // ----- In case of Master
                if (pInterface->pPcmConfig->PcmClockMaster)
                {
                    switch (pInterface->pPcmConfig->PcmBitPerChannel)
                    {
                        case PCM_BIT_8:

                            //SCI_TraceLow:"Codec LM49370 do not support 8bit per channel in PCM mode!(48KHz)\r\n"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,SPI_LM49370_V3_1987_112_2_17_23_2_50_168,(uint8*)"");
                            return LM49370_ERR_INVALID_PARA;

                            //break;

                        case PCM_BIT_16:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0xC;              // 0x1D, bit3:0, Divider = 7.8125
                            PCM_SYN_DIV = 0x1;              // 0x1D, bit6:4, mono = 16

                            break;

                        case PCM_BIT_32:

                            PcmClkSrc = PCM_CLK_SRC_ADC;
                            PCM_CLK_DIV = 0xA;              // 0x1D, bit3:0, Divider = 3.90625
                            PCM_SYN_DIV = 0x3;              // 0x1D, bit6:4, mono = 32

                            break;

                        default:
                            break;
                    }
                }
            }

            break;

        default:
            break;

    }

    // Initialize SPI1 port
    SPI_LM49370_Init();

    // Select GPIO2 for SPI_OUT
    addrdata[0] = 0x1F;
    addrdata[1] = 0x08;
    SPI_LM49370_WriteByte (addrdata, 2);

    // Basic, 0x00
    addrdata[0] = 0x00;
    addrdata[1] = (ChipMode << 0) | (PllEnable << 2) | (IntOSCEnable << 3) |
                  (BypassCapSize << 4) | (DacOverSampling << 6) ;
    SPI_LM49370_WriteByte (addrdata, 2);

#ifdef LM49370_REG_READ_BACK
    // Read data from Register
    addrdata[0] |= 0x80;
    //addrdata[1] = 0x00;
    idata = SPI_LM49370_ReadByte (addrdata, 2, readdata, 2);

    // If read back INVALID data, return FALSE
    if (idata != addrdata[1])
    {
        return LM49370_ERR_INVALID_READBACK;
    }
#endif

    // ----- Clock Configuration
    // PLL_M, 0x02
    addrdata[0] = 0x02;
    addrdata[1] = PLL_M;                                    // PLL_M
    SPI_LM49370_WriteByte (addrdata, 2);

    // PLL_N, 0x03
    addrdata[0] = 0x03;
    addrdata[1] = PLL_N;                                    // PLL_N
    SPI_LM49370_WriteByte (addrdata, 2);

    // PLL_N_MOD, 0x05
    addrdata[0] = 0x05;
    addrdata[1] = (PLL_IN_MCLK << 5) | (PLL_N_MOD << 0);    // PLL_IN = MCLK, PLL_N_MOD = 0/32
    SPI_LM49370_WriteByte (addrdata, 2);

    // PLL_P, 0x04
    addrdata[0] = 0x04;
    addrdata[1] = (Q_DIV << 4) | (PLL_P << 0);              // Q_DIV = 6, PLL_P = 9
    SPI_LM49370_WriteByte (addrdata, 2);

    // ADC_2, 0x07
    addrdata[0] = 0x07;
    addrdata[1] = (ADCClkSrc << 5) | (AdcOverSamling << 0) | (AdcInput.ADCMute << 1);
    SPI_LM49370_WriteByte (addrdata, 2);

    // CLOCKS, 0x01
    addrdata[0] = 0x01;
    addrdata[1] = (R_DIV << 2) | (DACClkSrc << 0);          // R_DIV = 11, DAC_CLK_IN = PLL_OUT
    SPI_LM49370_WriteByte (addrdata, 2);

    // ----- Interface Configuration
    // IIS Mode, 0x1A
    addrdata[0] = 0x1A;
    addrdata[1] = (IisOutputEnable << 0) | (IisInputEnable << 1) | (IisMode << 2) |
                  (IisStereoReverse << 3) | (IisWSMaster << 4) | (IisWSMode << 5) |
                  (IisGrouping << 7);
    SPI_LM49370_WriteByte (addrdata, 2);

    // IIS Clock, 0x1B
    addrdata[0] = 0x1B;
    addrdata[1] = (IisClockMaster << 0) | (IisClkSrc << 1) | (IIS_CLK_DIV << 2) |
                  (PcmSyncWidth << 6);
    SPI_LM49370_WriteByte (addrdata, 2);

    // PCM Mode, 0x1C
    addrdata[0] = 0x1C;
    addrdata[1] = (PcmOutputEnable << 0) | (PcmInputEnable << 1) | (PcmClkSrc << 3) |
                  (PcmSyncMaster << 4) | (PcmClockMaster << 2) | (PcmSdoLSBHZEnable << 5) |
                  (PcmCompandOn << 6) | (PcmMode << 7);
    SPI_LM49370_WriteByte (addrdata, 2);

    // PCM Clock, 0x1D
    addrdata[0] = 0x1D;
    addrdata[1] = (PCM_CLK_DIV << 0) | (PCM_SYN_DIV << 4);
    SPI_LM49370_WriteByte (addrdata, 2);

    // ----- Basic Configuration
    // LS_OUTPUT, 0x14
    addrdata[0] = 0x14;
    addrdata[1] = (LsOutput.CPISel << 0) | (LsOutput.RightSel << 1) |
                  (LsOutput.LeftSel << 2) | (LsOutput.LsMute);
    SPI_LM49370_WriteByte (addrdata, 2);

    // HP_OUTPUT, 0x15
    addrdata[0] = 0x15;
    addrdata[1] = (HpOutput.SideToneSel << 0) | (HpOutput.CPISel << 1) |
                  (HpOutput.RightSel << 2) | (HpOutput.LeftSel << 3) | (HpOutput.HpMute << 4) |
                  (StereoModeEnable << 5) | (HPOCLMode << 6);
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_LEFT, 0x0F
    addrdata[0] = 0x0F;
    addrdata[1] = (AUXLeft << 0) | (LeftSrc << 7);
    SPI_LM49370_WriteByte (addrdata, 2);

    // AUX_RIGHT, 0x10
    addrdata[0] = 0x10;
    addrdata[1] = (AUXRight << 0) | (RightSrc << 7);
    SPI_LM49370_WriteByte (addrdata, 2);

    // DAC, 0x11
    addrdata[0] = 0x11;
    addrdata[1] = (DACLevel << 0);
    SPI_LM49370_WriteByte (addrdata, 2);

    // ADC_1, 0x06
    addrdata[0] = 0x06;
    addrdata[1] = (AdcInput.MICSel << 0) | (AdcInput.CPISel << 1) |
                  (AdcInput.LeftSel << 2) | (AdcInput.RightSel << 3);
    SPI_LM49370_WriteByte (addrdata, 2);

    // ----- HeadSet/MIC Configurations
    // MIC_1, 0x0B
    addrdata[0] = 0x0B;
    addrdata[1] = (InternalMic << 5) | (MICSelct << 6) | (0xF << 0);
    SPI_LM49370_WriteByte (addrdata, 2);

    // MIC_2, 0x0C, Button type & Button debounce_time not configed
    addrdata[0] = 0x0C;
    addrdata[1] = (OCLVCMVoltage << 0) | (MicBiasVoltage << 1);
    SPI_LM49370_WriteByte (addrdata, 2);

    // ----- Bridge Configurations
    // Bridge, 0x1E
    addrdata[0] = 0x1E;
    addrdata[1] = (PcmTxSel << 0) | (IisTxSel << 1) | (DacInputSel << 3) |
                  (MonoSumSel << 5) | (MonoSum << 6);
    SPI_LM49370_WriteByte (addrdata, 2);

    /*
    // GPIO control, 0x1F
    addrdata[0] = 0x1F;
    addrdata[1] = (AdcSrcMode << 6) | (DacSrcMode << 7);
    SPI_LM49370_WriteByte(addrdata, 2);
    */

    SPI_HAL_Close (SPI_LOGIC_ID);
    return LM49370_SUCCESS;
}







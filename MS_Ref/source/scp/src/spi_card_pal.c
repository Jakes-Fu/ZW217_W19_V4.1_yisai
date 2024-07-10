



#include "sci_types.h"
#include "SCI_api.h"
#include "spi_card_pal.h"
#include "spi_drv.h"

/*-----------------------------------------*/
//	Debug Function
/*-----------------------------------------*/
#define CARD_SPI_PAL_ASSERT SCI_ASSERT
/*-----------------------------------------*/
//	IRQ Function
/*-----------------------------------------*/
#if defined(OS_SELF_SIMULATOR)
#define CARD_SPI_PAL_IRQ_DECLARE OS_CPU_SR  cpu_sr
#define CARD_SPI_PAL_DISABLE_IRQ OS_ENTER_CRITICAL
#define CARD_SPI_PAL_ENABLE_IRQ OS_EXIT_CRITICAL
#else
#define CARD_SPI_PAL_IRQ_DECLARE
#define CARD_SPI_PAL_DISABLE_IRQ SCI_DisableIRQ
#define CARD_SPI_PAL_ENABLE_IRQ SCI_RestoreIRQ
#endif
/*-----------------------------------------*/



typedef struct
{
	BOOLEAN flag;
	SPI_CARD_PORT_E spiId;
	SPI_CARD_CS_E csId;
}CARD_SPI_PAL_struct;

#define CARD_SPI_PAL_SUPPORT_NUM 3
LOCAL CARD_SPI_PAL_struct s_cardSpiPalHd[CARD_SPI_PAL_SUPPORT_NUM] = {{FALSE,0,0},{FALSE,0,0},{FALSE,0,0}};
/*
	Function: 打开Spi驱动容器，返回驱动的handle，其余功能都通过handle进行访问
	Param:
		SpiId: 指定 spi端口
		CsId:  指定该端口的CS编号
*/
PUBLIC SPI_CARD_PAL_HANDLE Spi_Card_Pal_Open(SPI_CARD_PORT_E spiId, SPI_CARD_CS_E csId)
{
	uint32 i;
	SPI_CONFIG_T spiConfig;
	SPI_CARD_PAL_HANDLE handle;
	CARD_SPI_PAL_IRQ_DECLARE;

	handle = CARD_SPI_PAL_SUPPORT_NUM;

	// 检查是否有有效的handle存在
	CARD_SPI_PAL_DISABLE_IRQ();
	//---
	for(i = 0; i < CARD_SPI_PAL_SUPPORT_NUM; i++)
	{
		if(TRUE == s_cardSpiPalHd[i].flag)
		{
			if(
				(spiId == s_cardSpiPalHd[i].spiId)
				&&(csId == s_cardSpiPalHd[i].csId)
			)
			{
				CARD_SPI_PAL_ENABLE_IRQ();
				CARD_SPI_PAL_ASSERT(0);
			}
		}
		else
		{
			handle = i;
		}
	}
	if(CARD_SPI_PAL_SUPPORT_NUM <= handle)
	{
		CARD_SPI_PAL_ENABLE_IRQ();
		CARD_SPI_PAL_ASSERT(0);
		return handle;  //lint !e527
	}
	s_cardSpiPalHd[handle].flag = TRUE;
	CARD_SPI_PAL_ENABLE_IRQ();
	s_cardSpiPalHd[handle].spiId = spiId;
	s_cardSpiPalHd[handle].csId = csId;

	spiConfig.dev_type      = SPI_SDCARD;
	spiConfig.freq          = 400000;
    
	spiConfig.lsb_first     = FALSE;
#ifdef CHIP_DRV_SC6600L
	spiConfig.rx_shift_edge = TRUE;
	spiConfig.tx_shift_edge = FALSE;
#else
	spiConfig.rx_shift_edge = FALSE;
	spiConfig.tx_shift_edge = TRUE;
#endif
	spiConfig.tx_bit_length = 8;

	spiConfig.sck_reverse   = TRUE;
    switch(csId)
    {
	case SPI_CARD_CS0:
        spiConfig.cs_id         = SPI_CS_ID0;
    break;
	case SPI_CARD_CS1:
        spiConfig.cs_id         = SPI_CS_ID1;
    break;
	case SPI_CARD_CS2:
        spiConfig.cs_id         = SPI_CS_ID2;
    break;
	case SPI_CARD_CS3:
        spiConfig.cs_id         = SPI_CS_ID3;
    break;
    default:
		CARD_SPI_PAL_ASSERT(0);
    break;
    }
    
	//---
	if(SCI_FALSE == SPI_Open(spiId, &spiConfig))
	{
		CARD_SPI_PAL_ASSERT(0);
	}

	return handle;
}

/*
	Function: 初始化Spi驱动容器。
	SPI初始化成CLK上升沿采样，8bit宽，MSB
*/
PUBLIC BOOLEAN Spi_Card_Pal_PwrHook(SPI_CARD_PAL_HANDLE handle, SPI_CARD_PAL_PWR_E onOrOff)
{
	// 此模块只提供给Card_spi.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
	CARD_SPI_PAL_ASSERT(
		(handle < CARD_SPI_PAL_SUPPORT_NUM)
		&&(TRUE == s_cardSpiPalHd[handle].flag)
	);

	switch(onOrOff)
	{
		case SPI_CARD_PAL_PWR_ON_PRE:
			if(SCI_FALSE == SPI_Init(s_cardSpiPalHd[handle].spiId))
			{
				CARD_SPI_PAL_ASSERT(0);
			}
		break;

		case SPI_CARD_PAL_PWR_OFF_AFTER:
			if(SCI_FALSE == SPI_Deinit(s_cardSpiPalHd[handle].spiId))
			{
				CARD_SPI_PAL_ASSERT(0);
			}
		break;

		default:
			CARD_SPI_PAL_ASSERT(0);
		break;
	}
	return TRUE;

}

/*
	Function: 从Spi端口读取相应字节的数据
*/
PUBLIC BOOLEAN Spi_Card_Pal_Read(SPI_CARD_PAL_HANDLE handle,uint8* buf, uint32 len)
{

// 此模块只提供给Card_spi.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
//	CARD_SPI_PAL_ASSERT(
//		(handle < CARD_SPI_PAL_SUPPORT_NUM)
//		&&(TRUE == s_cardSpiPalHd[handle].flag)
//	);

	return SPI_Read(s_cardSpiPalHd[handle].spiId, buf, len);

}

/*
	Function: 向Spi端口写入相应字节的数据
*/
PUBLIC BOOLEAN Spi_Card_Pal_Write(SPI_CARD_PAL_HANDLE handle,uint8* buf, uint32 len)
{

// 此模块只提供给Card_spi.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
//	CARD_SPI_PAL_ASSERT(
//		(handle < CARD_SPI_PAL_SUPPORT_NUM)
//		&&(TRUE == s_cardSpiPalHd[handle].flag)
//	);

	return SPI_Write(s_cardSpiPalHd[handle].spiId, buf, len);

}

/*
	Function: 拉高或拉低Cs线
*/
PUBLIC BOOLEAN Spi_Card_Pal_SetCs(SPI_CARD_PAL_HANDLE handle,CARD_SPI_CS_E highOrLow)
{
// 此模块只提供给Card_spi.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
//	CARD_SPI_PAL_ASSERT(
//		(handle < CARD_SPI_PAL_SUPPORT_NUM)
//		&&(TRUE == s_cardSpiPalHd[handle].flag)
//	);

	switch(highOrLow)
	{
		case SPI_CARD_PAL_CS_HIGH:
			SPI_SetCSSignal(s_cardSpiPalHd[handle].spiId,SPI_CS_HIGH);
		break;

		case SPI_CARD_PAL_CS_LOW:
			SPI_SetCSSignal(s_cardSpiPalHd[handle].spiId,SPI_CS_LOW);
		break;

		default:
			CARD_SPI_PAL_ASSERT(0);
		break;
	}

	return TRUE;
}

/*
	Function: 指定SPI的工作频率
*/
PUBLIC BOOLEAN Spi_Card_Pal_SetClk(SPI_CARD_PAL_HANDLE handle,uint32 clk)
{

// 此模块只提供给Card_spi.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
//	CARD_SPI_PAL_ASSERT(
//		(handle < CARD_SPI_PAL_SUPPORT_NUM)
//		&&(TRUE == s_cardSpiPalHd[handle].flag)
//	);

	SPI_SetClkFreq(s_cardSpiPalHd[handle].spiId, clk);

	return TRUE;
}

/*
	Function: 关闭Spi驱动容器，handle随即失效
*/
PUBLIC BOOLEAN Spi_Card_Pal_Close(SPI_CARD_PAL_HANDLE handle)
{
	CARD_SPI_PAL_IRQ_DECLARE;

	// 此模块只提供给Card_spi.c这个协议使用，协议代码本身已经保证该handle的有效性，因此在这里不做特别严的检查
	CARD_SPI_PAL_ASSERT(
		(handle < CARD_SPI_PAL_SUPPORT_NUM)
		&&(TRUE == s_cardSpiPalHd[handle].flag)
	);

	CARD_SPI_PAL_DISABLE_IRQ();
	s_cardSpiPalHd[handle].flag = FALSE;
	CARD_SPI_PAL_ENABLE_IRQ();

	SPI_Close(s_cardSpiPalHd[handle].spiId);

	return TRUE;
}




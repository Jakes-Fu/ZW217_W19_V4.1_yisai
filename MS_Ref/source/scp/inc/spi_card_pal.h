

#ifndef _SPI_CARD_PAL_H_
#define _SPI_CARD_PAL_H_

#include "sci_types.h"

/**************************************************************************
Jason.wu

SD/MMC 关于SPI接口协议代码 所使用的SPI抽象层，无论底层驱动如何变化请保持
该抽象层不变。
另外该文件不允许被修改

该抽象层定义了一个半双工的SPI端口.
**************************************************************************/

typedef uint32 SPI_CARD_PAL_HANDLE;

/*
	Function: 打开Spi驱动容器，返回驱动的handle，其余功能都通过handle进行访问
	Param:
		SpiId: 指定 spi端口
		CsId:  指定该端口的CS编号
*/
typedef enum SPI_Card_Port_enum
{
	SPI_CARD_PORT_0 = 0,
	SPI_CARD_PORT_1,
	SPI_CARD_PORT_2,
	SPI_CARD_PORT_MAX_NUM
}
SPI_CARD_PORT_E;

typedef enum SPI_Card_Cs_enum
{
	SPI_CARD_CS0 = 0,
	SPI_CARD_CS1,
	SPI_CARD_CS2,
	SPI_CARD_CS3,
	SPI_CARD_CS_MAX_NUM
}
SPI_CARD_CS_E;
PUBLIC SPI_CARD_PAL_HANDLE Spi_Card_Pal_Open(SPI_CARD_PORT_E spiId, SPI_CARD_CS_E csId);

/*
	Function: 初始化Spi驱动容器。
	SPI初始化成CLK上升沿采样，8bit宽，MSB
*/
typedef enum
{
	SPI_CARD_PAL_PWR_OFF_AFTER,
	SPI_CARD_PAL_PWR_ON_PRE
}
SPI_CARD_PAL_PWR_E;
PUBLIC BOOLEAN Spi_Card_Pal_PwrHook(SPI_CARD_PAL_HANDLE handle, SPI_CARD_PAL_PWR_E onOrOff);

/*
	Function: 从Spi端口读取相应字节的数据
*/
PUBLIC BOOLEAN Spi_Card_Pal_Read(SPI_CARD_PAL_HANDLE handle,uint8* buf, uint32 len);

/*
	Function: 向Spi端口写入相应字节的数据
*/
PUBLIC BOOLEAN Spi_Card_Pal_Write(SPI_CARD_PAL_HANDLE handle,uint8* buf, uint32 len);

/*
	Function: 拉高或拉低Cs线
*/
typedef enum
{
	SPI_CARD_PAL_CS_HIGH,
	SPI_CARD_PAL_CS_LOW
}CARD_SPI_CS_E;
PUBLIC BOOLEAN Spi_Card_Pal_SetCs(SPI_CARD_PAL_HANDLE handle,CARD_SPI_CS_E highOrLow);

/*
	Function: 指定SPI的工作频率
*/
PUBLIC BOOLEAN Spi_Card_Pal_SetClk(SPI_CARD_PAL_HANDLE handle,uint32 clk);

/*
	Function: 关闭Spi驱动容器，handle随即失效
*/
PUBLIC BOOLEAN Spi_Card_Pal_Close(SPI_CARD_PAL_HANDLE handle);




#endif



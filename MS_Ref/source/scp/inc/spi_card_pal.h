

#ifndef _SPI_CARD_PAL_H_
#define _SPI_CARD_PAL_H_

#include "sci_types.h"

/**************************************************************************
Jason.wu

SD/MMC ����SPI�ӿ�Э����� ��ʹ�õ�SPI����㣬���۵ײ�������α仯�뱣��
�ó���㲻�䡣
������ļ��������޸�

�ó���㶨����һ����˫����SPI�˿�.
**************************************************************************/

typedef uint32 SPI_CARD_PAL_HANDLE;

/*
	Function: ��Spi��������������������handle�����๦�ܶ�ͨ��handle���з���
	Param:
		SpiId: ָ�� spi�˿�
		CsId:  ָ���ö˿ڵ�CS���
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
	Function: ��ʼ��Spi����������
	SPI��ʼ����CLK�����ز�����8bit��MSB
*/
typedef enum
{
	SPI_CARD_PAL_PWR_OFF_AFTER,
	SPI_CARD_PAL_PWR_ON_PRE
}
SPI_CARD_PAL_PWR_E;
PUBLIC BOOLEAN Spi_Card_Pal_PwrHook(SPI_CARD_PAL_HANDLE handle, SPI_CARD_PAL_PWR_E onOrOff);

/*
	Function: ��Spi�˿ڶ�ȡ��Ӧ�ֽڵ�����
*/
PUBLIC BOOLEAN Spi_Card_Pal_Read(SPI_CARD_PAL_HANDLE handle,uint8* buf, uint32 len);

/*
	Function: ��Spi�˿�д����Ӧ�ֽڵ�����
*/
PUBLIC BOOLEAN Spi_Card_Pal_Write(SPI_CARD_PAL_HANDLE handle,uint8* buf, uint32 len);

/*
	Function: ���߻�����Cs��
*/
typedef enum
{
	SPI_CARD_PAL_CS_HIGH,
	SPI_CARD_PAL_CS_LOW
}CARD_SPI_CS_E;
PUBLIC BOOLEAN Spi_Card_Pal_SetCs(SPI_CARD_PAL_HANDLE handle,CARD_SPI_CS_E highOrLow);

/*
	Function: ָ��SPI�Ĺ���Ƶ��
*/
PUBLIC BOOLEAN Spi_Card_Pal_SetClk(SPI_CARD_PAL_HANDLE handle,uint32 clk);

/*
	Function: �ر�Spi����������handle�漴ʧЧ
*/
PUBLIC BOOLEAN Spi_Card_Pal_Close(SPI_CARD_PAL_HANDLE handle);




#endif



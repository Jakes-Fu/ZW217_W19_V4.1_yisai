ifeq ($(strip $(CMMB_SUPPORT)), TRUE)
MCFLAG_OPT   	=  -DSPRD_SUPPORT_MCEX
endif

MINCPATH =  MS_Ref/source/scp/inc MS_Ref/source/scp/inc
MINCPATH += chip_drv/export/inc/outdated
MINCPATH += chip_drv/chip_module/sdio/uws6121e

MSRCPATH =  MS_Ref/source/scp/src

SOURCES = 

ifeq ($(strip $(PLATFORM)),	NILE)
MCFLAG_OPT 		+= -DCHIP_DRV_SC6600M -DCARD_DMA_TRANSFER
SOURCES             		+=  card_spi.c storagecard_crc.c spi_card_pal.c 
endif

ifeq ($(strip $(PLATFORM)),	SC6600R)
MCFLAG_OPT 		+= -DCHIP_DRV_SC6600R -DCARD_DMA_TRANSFER
SOURCES             		+=  card_spi.c storagecard_crc.c spi_card_pal.c 
endif

ifeq ($(strip $(PLATFORM)),	HIMALAYA)
MCFLAG_OPT 		+= -DCHIP_DRV_SC6600H -DCARD_DMA_TRANSFER
SOURCES             		+=  card_spi.c storagecard_crc.c spi_card_pal.c 
endif

ifeq ($(strip $(PLATFORM)),	SC6600L)
MCFLAG_OPT 		+= -DCHIP_DRV_SC6600L -DCARD_DMA_TRANSFER
SOURCES             		+=  card_spi.c storagecard_crc.c spi_card_pal.c 
endif

ifeq ($(strip $(PLATFORM)),	SC6800H)
SOURCES     += sdio_card_pal.c card_sdio.c 
endif

ifeq ($(strip $(PLATFORM)),	SC6530)
SOURCES     += sdio_card_pal.c card_sdio.c 
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
SOURCES     += sdio_card_pal.c card_sdio.c 
endif

ifeq ($(strip $(PLATFORM)),	UWS6121E)
SOURCES     += sdio_card_pal.c card_sdio.c mcd_sdmmc.c
endif

ifeq ($(strip $(PLATFORM)),	SC8800H)
SOURCES             +=         	sdio_card_pal.c card_sdio.c 
endif

ifeq ($(strip $(PLATFORM)),	SC8800G)
SOURCES             +=         	sdio_card_pal.c card_sdio.c 
endif

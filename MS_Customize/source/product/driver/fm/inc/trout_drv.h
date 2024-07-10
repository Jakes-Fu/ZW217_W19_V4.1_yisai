#ifndef _TROUT_DRV_H_
#define _TROUT_DRV_H_

//#include "trout_inc.h"
//#include "trout_com_reg_def.h"
//#include "trout_sys_reg_def.h"
#include "sci_types.h"

#define SPI_RX_FIFO_REALLY_FULL     (BIT_4)
#define SPI_RX_FIFO_REALLY_EMPTY    (BIT_5)
    
#define SPI_TX_FIFO_REALLY_FULL     (BIT_6)
#define SPI_TX_FIFO_REALLY_EMPTY    (BIT_7)
#define SPI_TX_BUSY                 (BIT_8)
#define SPI_TIME_OUT        	(0x10)

    /********************************************************/
    /* this GPIO Control function is defined temp           */
    /* get more information from sc8810 device spec         */
    /********************************************************/
typedef struct
{
    uint32  reg_addr;
    uint32  reg_data;
} trout_reg_cfg_t;

PUBLIC BOOLEAN trout_write_reg(uint32 reg_addr, uint32 reg_data);
PUBLIC BOOLEAN mac_write_reg(uint32 reg_addr, uint32 reg_data);
int32 trout_write_reg_cfg(trout_reg_cfg_t *reg_cfg, uint32 cfg_cnt);
int32 trout_read_reg(uint32 reg_addr, uint32 *reg_data);
int32 trout_read_reg_cfg(trout_reg_cfg_t *reg_cfg, uint32 cfg_cnt);
int32 spi_write_waitforidle(void);
int32 spi_read_waitforidle(void);
PUBLIC BOOLEAN trout_read_rf_reg(uint32 addr, uint32 *data);
PUBLIC BOOLEAN trout_write_rf_reg(uint32 addr, uint32 data);

void fm_calibration_main(void);
void fm_calibration_main_keep(void);
void fm_calibration_main_test(void);

#endif


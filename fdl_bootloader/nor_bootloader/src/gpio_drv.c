#include "common.h"
#include "sio_drv.h"
#include "gpio_reg_v5.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif
                                       
/* Pinmap ctrl register Bit field value
------------------------------------------------------------------------------------------------------------
|                |               |                 |            |             |             |              |
| Reserved[31:8] | Func Sel[7:6] | Drv Strght[5:4] | Pull Up[3] | Pull Down[2]| Input EN[1] | Output EN[0] |
|                |               |                 |            |             |             |              |
------------------------------------------------------------------------------------------------------------
*/
/*GPIO_PROD_AP_RST_BB_ID GPIO number, pls refer to MS_Customize's gpio_cfg.c 
GPIO_PROD_AP_RST_BB_ID Pin configuration*/
#if (defined(MODEM_TYPE_ZBX))
    #define GPIO_PROD_AP_RST_BB_ID_NUM  (88)
#elif defined MODEM_TYPE_V2
    #define GPIO_PROD_AP_RST_BB_ID_NUM  (0)
#else
    #error "MODEM_TYPE config ERROR!"
#endif

#define GPIO_PROD_AP_RST_BB_ID_NUM_BIT  (1<<(GPIO_PROD_AP_RST_BB_ID_NUM & 0xF))
#define GET_GPIO_BASE_ADDR(NUM)         ((((NUM)&0x7F)>>4)*0x80 + GPIO_PG_BASE)

uint32 _GPIO_GetValue(uint32 gpio_id)
{
   #if 0
   uint32 page, bit_num, gpio_pg_base;
    
    // set initial state
    page        = gpio_id >> 4;
    bit_num     = gpio_id & 0xF;
    gpio_pg_base= page * 0x80 + GPIO_PG_BASE;
    
    page  = *(volatile uint32 *)(gpio_pg_base + GPIO_DATA);
    SIO_TRACE("_GPIO_GetValue = 0x%x\r\n", page);
    return ((page >> bit_num) & 1 );
   #endif 
}

uint32 GPIO_GetAPRstBBValue(void)
{
	return 1;//return _GPIO_GetValue(GPIO_PROD_AP_RST_BB_ID_NUM);
}

void GPIO_GetAPRstBBValueInit(void)
{
#if 0
REG32(APB_EB0_SET) |= (GPIO_EB_SET | GPIO_RTC_EB_SET | PIN_REG_EB_SET);	

#if (defined(MODEM_TYPE_ZBX))
    /*GPIO_50 Default configuration*/
	REG32(PIN_KEYOUT2_REG) |= ( PIN_I_EN | PIN_SPU_EN | PIN_FUNC_3 | PIN_FPU_EN | PIN_DS_1);
#elif defined MODEM_TYPE_V2
	/*GPIO_0 Default configuration*/
	REG32(PIN_GPIO0_REG) |= ( PIN_I_EN | PIN_SPU_EN | PIN_FUNC_DEF | PIN_FPU_EN | PIN_DS_1);
#else
    #error "MODEM_TYPE config ERROR!"
#endif
    REG32(GPIO_IE + (GET_GPIO_BASE_ADDR(GPIO_PROD_AP_RST_BB_ID_NUM))) &= (~GPIO_PROD_AP_RST_BB_ID_NUM_BIT);
	REG32(GPIO_IC + (GET_GPIO_BASE_ADDR(GPIO_PROD_AP_RST_BB_ID_NUM))) |= GPIO_PROD_AP_RST_BB_ID_NUM_BIT;
	REG32(GPIO_DIR + (GET_GPIO_BASE_ADDR(GPIO_PROD_AP_RST_BB_ID_NUM))) &= (~GPIO_PROD_AP_RST_BB_ID_NUM_BIT);
	REG32(GPIO_DMSK + (GET_GPIO_BASE_ADDR(GPIO_PROD_AP_RST_BB_ID_NUM))) |= GPIO_PROD_AP_RST_BB_ID_NUM_BIT;
    
    SIO_TRACE("REG32(PIN_KEYOUT2_REG) |= ( PIN_I_EN | PIN_SPU_EN | PIN_FUNC_3 | PIN_FPU_EN | PIN_DS_1) = 0x%x \r\n",  REG32(PIN_KEYOUT2_REG));
	SIO_TRACE("REG32(GPIO_IE) = 0x%x \r\n",  REG32(GPIO_IE + (GET_GPIO_BASE_ADDR(GPIO_PROD_AP_RST_BB_ID_NUM))));
	SIO_TRACE("REG32(GPIO_IC) = 0x%x \r\n",  REG32(GPIO_IC + (GET_GPIO_BASE_ADDR(GPIO_PROD_AP_RST_BB_ID_NUM))));
	SIO_TRACE("REG32(GPIO_DIR) = 0x%x \r\n",  REG32(GPIO_DIR + (GET_GPIO_BASE_ADDR(GPIO_PROD_AP_RST_BB_ID_NUM))));
	SIO_TRACE("REG32(GPIO_DMSK) = 0x%x \r\n",  REG32(GPIO_DMSK + (GET_GPIO_BASE_ADDR(GPIO_PROD_AP_RST_BB_ID_NUM))));
	#endif
}

#ifdef __cplusplus
    }
#endif



#ifndef __VCHR02_COMMON_H__
#define __VCHR02_COMMON_H__

#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "os_api.h"
#include "gpio_prod_api.h"
#include "gpio_prod_cfg.h"
#include "Power_cfg.h"
#include "zdt_hsensor.h"

#include "priority_system.h"
#include "ldo_drvapi.h"
#include "dal_time.h"




#define HRS_VCHR02_ENABLE  1

#define HRS_I2C_ERROR                0
#define HRS_I2C_SUCCESS              1


#define v_abs(x)  ((x) > 0 ? (x) : (-(x)))


typedef enum
{
    HRS_HEARTRATE,
    HRS_BLOODPRESS,

    HRS_BLOODOXYGEN,
}HRS_DATA_TYPE;


#endif /* VCHR02_COMMON_H_ */




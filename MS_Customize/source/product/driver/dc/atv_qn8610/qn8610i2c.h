#ifndef _QN8610I2C_DRV_H_
#define _QN8610I2C_DRV_H_

#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

uint8 QMA_WriteReg(uint8 Regis_Addr,uint8 Data);

uint8 QMA_ReadReg(uint8 Regis_Addr); 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  // _QN8610I2C_DRV_H_

// End of qn8610i2c.h


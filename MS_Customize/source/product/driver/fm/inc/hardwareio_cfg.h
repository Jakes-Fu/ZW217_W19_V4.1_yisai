#ifndef _HARDWARE_IO_CFG_H_
#define _HARDWARE_IO_CFG_H_

void Init3wHardwareIo_PowerOn(void);

void Init3wHardwareIo(void);

void Close3wHardwareIo(void);

void ShutDownLv2400xChip(void);

/*****************************************************************************/
//  Description:    This function is used to open or close ldo    
//  Author:         huazhong.yue
//  Note:    user can finish it according to hardware design if need.       
/*****************************************************************************/
void CtlPower(BOOLEAN is_open);

/*****************************************************************************/
//  Description:    This function is used to open or close ldo    
//  Author:         huazhong.yue
//  Note:    user can finish it according to hardware design if need.       
/*****************************************************************************/
void OpenFMPA(BOOLEAN is_open);

/*****************************************************************************/
//  Description:    This function is used to open 32k clock.    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
void Open32kClock(BOOLEAN is_on);

#endif // End of _HARDWARE_IO_CFG_H_
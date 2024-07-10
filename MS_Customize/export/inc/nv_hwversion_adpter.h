#ifndef  _NV_HWVERSION_ADPTER_H_
#define _NV_HWVERSION_ADPTER_H_


PUBLIC BOOLEAN NV_Force_Change(char* fileName, uint16 version);

PUBLIC BOOLEAN get_deltanv_info(uint32* deltanv_addr, uint32* p_size);

PUBLIC BOOLEAN get_rf_deltanv_info(uint32* deltanv_addr, uint32* p_size);

PUBLIC void REFPARAM_GetFileVersion(uint8* rf_version, uint8* pa_version);

PUBLIC uint16 NV_Force_Change_Ex(char* fileName, uint8* deltanv_addr, uint32 deltanv_size, uint16 version);

#endif

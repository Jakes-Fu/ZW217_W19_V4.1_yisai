#ifndef HARDWARE_MOCOR_API_H
#define HARDWARE_MOCOR_API_H


#include "nvitem.h"
#include "nv_item_id.h"
extern NVITEM_ERROR_E EFS_NvitemRead(INPUT uint16 ItemID, INPUT uint16 cchSize, OUTPUT uint8 *pBuf);
extern NVITEM_ERROR_E EFS_NvitemWrite(INPUT uint16 ItemID, INPUT uint16 cchSize, INPUT const uint8 *pBuf, INPUT uint8 bAsync);
#define HALO_ReadEfuseUID(UID) EFUSE_HAL_ReadUid(UID)

#endif 
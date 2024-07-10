#ifndef BSD_H
#define BSD_H

#include "sci_types.h"

/******************************************************************************
 * Return codes of BSD_XXX() 
 ******************************************************************************/
typedef uint32 BSD_STATUS;
#define BSD_RETURN_VALUE(err, maj, min)	(uint32)(((uint32)((err) & 0x00000001) << 31) | \
											     ((uint32)((maj) & 0x00007FFF) << 16) | \
											      (uint32)((min) & 0x0000FFFF))
#define BSD_RETURN_MAJOR(err)			(uint32)(((err) & 0x7FFF0000) >> 16)
#define	BSD_RETURN_MINOR(err)			(uint32)((err) & 0x0000FFFF)
#define BSD_SUCCEEDED(err)				(0 == ((uint32)(err) >> 31))

#define BSD_SUCCESS						BSD_RETURN_VALUE(0, 0x0000, 0x0000)
#define BSD_ERROR						BSD_RETURN_VALUE(1, 0x0000, 0x0000)
#define BSD_SYSTEM_ERROR				BSD_RETURN_VALUE(1, 0x0001, 0x0000)
#define BSD_INVALID_PARAM				BSD_RETURN_VALUE(1, 0x0002, 0x0000)
#define BSD_INVALID_HANDLE				BSD_RETURN_VALUE(1, 0x0002, 0x0001)
#define BSD_NO_MORE_RESOURCE			BSD_RETURN_VALUE(1, 0x0003, 0x0000)
#define BSD_NOT_ENOUGH_MEMORY			BSD_RETURN_VALUE(1, 0x0004, 0x0000)
#define BSD_TOO_MANY_OPEN_HANDLES		BSD_RETURN_VALUE(1, 0x0005, 0x0000)
#define BSD_ACCESS_DENIED				BSD_RETURN_VALUE(1, 0x0006, 0x0000)
#define BSD_SHARING_VIOLATION			BSD_RETURN_VALUE(1, 0x0007, 0x0000)
#define BSD_NOT_SUPPORTED				BSD_RETURN_VALUE(1, 0x0008, 0x0000)
#define BSD_ALREADY_EXISTS				BSD_RETURN_VALUE(1, 0x0009, 0x0000)
#define BSD_DOES_NOT_EXIST				BSD_RETURN_VALUE(1, 0x000A, 0x0000)
#define BSD_HAS_OPEN_HANDLES			BSD_RETURN_VALUE(1, 0x000B, 0x0000)
#define BSD_TO_BE_REMOVED				BSD_RETURN_VALUE(1, 0x000C, 0x0000)



/******************************************************************************
 * BSD handle 
 ******************************************************************************/
typedef uint32 BSD_HANDLE;


/******************************************************************************
 * BSD Device Types 
 ******************************************************************************/
#define BSD_RAM					1
#define BSD_ROM					2
#define BSD_NORFLASH			3
#define BSD_NANDFLASH			4
#define BSD_SD					5
#define BSD_BML					6
#define BSD_STL					7
#define BSD_OTHER				254	
#define BSD_UNKNOWN				255


/******************************************************************************
 * BSD Device Attributes
 ******************************************************************************/
#define BSD_DEVICE_NORMAL				0x0
#define BSD_DEVICE_READONLY				0x1
#define BSD_DEVICE_WRITABLE_ON_DEMAND	0x2
#define BSD_DEVICE_EXCLUSIVE			0x4
#define BSD_DEVICE_SYNC_ACCESS			0x8

/******************************************************************************
 * BSD Open Flags
 ******************************************************************************/
#define BSD_GENERIC_READ		0x1
#define BSD_GENERIC_WRITE		0x2
#define BSD_SHARING_READ		0x4
#define BSD_SHARING_WRITE		0x8

/******************************************************************************
 * BSD Partition Size Flags
 ******************************************************************************/
/* If the value of Offset of structure BSD_PARTITION is BSD_PART_SIZE_FULL, 
 * BSD_Mount will extend the size of this partition to the end of its device.
 */
#define BSD_PART_SIZE_FULL		0xFFFFFFFF 

/******************************************************************************
 * Configurations of BSD 
 ******************************************************************************/
#define MAX_HANDLE_NUM			MAX_OBJECT_NUM

/******************************************************************************
 * Prototypes of BSD callback functions
 ******************************************************************************/
struct _BSD_DEVICE;

typedef BSD_STATUS (*BSD_READ)(struct _BSD_DEVICE *pDev, uint32 Start, 
	uint32 NumOfSectors, void *pBuf);
typedef BSD_STATUS (*BSD_WRITE)(struct _BSD_DEVICE *pDev, uint32 Start, 
	uint32 NumOfSectors, const void *pBuf);
typedef BSD_STATUS (*BSD_READOOB)(struct _BSD_DEVICE *pDev, uint32 Start,
	uint32 NumOfSectors, const void *pBuf);
typedef BSD_STATUS (*BSD_WRITEOOB)(struct _BSD_DEVICE *pDev, uint32 Start,
	uint32 NumOfSectors, const void *pBuf);
typedef BSD_STATUS (*BSD_ERASE)(struct _BSD_DEVICE *pDev, uint32 Start, 
	uint32 NumOfEraseUnit);
typedef BSD_STATUS (*BSD_IOCTL)(struct _BSD_DEVICE *pDev, uint32 Code, 
	const void *pIn, uint32 InSize, void *pOut, uint32 OutSize, uint32 *pBytesReturned);
typedef BSD_STATUS (*BSD_SYNC)(struct _BSD_DEVICE *pDev);
	
typedef BSD_STATUS (*BSD_LOCK)(struct _BSD_DEVICE *pDev);
typedef BSD_STATUS (*BSD_UNLOCK)(struct _BSD_DEVICE *pDev);
	
typedef BSD_STATUS (*BSD_SUSPEND)(struct _BSD_DEVICE *pDev);
typedef BSD_STATUS (*BSD_RESUME)(struct _BSD_DEVICE *pDev);
	
typedef BSD_STATUS (*BSD_MOUNT)(struct _BSD_DEVICE *pDev);
typedef BSD_STATUS (*BSD_UNMOUNT)(struct _BSD_DEVICE *pDev);
	
/******************************************************************************
 * BSD structures
 ******************************************************************************/
#define MAX_DEVICE_NAME_LEN		128

typedef struct _BSD_DEVICE {
	char 	Name[MAX_DEVICE_NAME_LEN];
	uint32	Index;
	uint8	Type;
	BOOLEAN bMaster;
	uint32	Attribute;
	
	uint32 	SectorSize; 		/* in bytes */
	uint32  OobSize; 			/* in bytes */
	
	uint32 	EraseUnitSize; 		/* in sectors */
	uint32	TotScts; 			/* in sectors */
	uint32	Offset; 			/* in sectors */
	
	BSD_READ 		Read;
	BSD_WRITE 		Write;
	BSD_READOOB 	ReadOob;
	BSD_WRITEOOB	WriteOob;
	BSD_ERASE 		Erase;
	BSD_IOCTL 		IOCtl;
	BSD_SYNC 		Sync;
	
	BSD_LOCK 		Lock;
	BSD_UNLOCK 		Unlock;
	
	BSD_SUSPEND 	Suspend;
 	BSD_RESUME  	Resume;
	
	BSD_MOUNT   	Mount;
	BSD_UNMOUNT 	Unmount;
	
	void *DeviceExtension;
	
} BSD_DEVICE, *PBSD_DEVICE;

typedef struct _BSD_PARTITION {	
	char 	Name[MAX_DEVICE_NAME_LEN];
	uint32  Index;
	uint32	TotScts;
	uint32	Offset;
	uint32 	AttrMask;
} BSD_PARTITION, *PBSD_PARTITION;

typedef struct _BSD_NOTIFIER {
	void (*OnMount)(struct _BSD_DEVICE *pDev);
	void (*OnUnmount)(struct _BSD_DEVICE *pDev);
	struct _BSD_NOTIFIER *pNext;
} BSD_NOTIFIER, *PBSD_NOTIFIER;


/******************************************************************************
 * Prototype of BSD functions
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

BSD_STATUS BSD_Initialize(void);
BSD_STATUS BSD_Uninitialize(void);
BSD_STATUS BSD_RegUser(BSD_NOTIFIER *pUser);
BSD_STATUS BSD_UnregUser(BSD_NOTIFIER *pUser);

BSD_STATUS BSD_AddDevice(const BSD_DEVICE *pDev);
BSD_STATUS BSD_RemoveDevice(const char *Name, BOOLEAN bForceRemove);
BSD_STATUS BSD_Mount(BSD_HANDLE hDev, BSD_PARTITION *pPart, uint32 Num);
BSD_STATUS BSD_Unmount(const char *Name, BOOLEAN bForceUnmount);

BSD_STATUS BSD_Open(const char *Name, uint32 Flag, BSD_HANDLE *pHandle);
BSD_STATUS BSD_Close(BSD_HANDLE hDev);

BSD_STATUS BSD_Read(BSD_HANDLE hDev, uint32 Start, uint32 NumOfSectors, void *pBuf);
BSD_STATUS BSD_Write(BSD_HANDLE hDev, uint32 Start, uint32 NumOfSectors, 
	const void *pBuf);
BSD_STATUS BSD_ReadOob(BSD_HANDLE hDev, uint32 Start, uint32 NumOfSectors, void *pBuf);
BSD_STATUS BSD_WriteOob(BSD_HANDLE hDev, uint32 Start, uint32 NumOfSectors,
	const void *pBuf);
BSD_STATUS BSD_Erase(BSD_HANDLE hDev, uint32 Start, uint32 NumOfSectors);
BSD_STATUS BSD_IOCtl(BSD_HANDLE hDev, uint32 Code, const void *pIn, uint32 InSize,
	void *pOut, uint32 OutSize, uint32 *pBytesReturned);
BSD_STATUS BSD_Sync(BSD_HANDLE hDev);
BSD_STATUS BSD_Lock(BSD_HANDLE hDev);
BSD_STATUS BSD_Unlock(BSD_HANDLE hDev);
BSD_STATUS BSD_Suspend(BSD_HANDLE hDev);
BSD_STATUS BSD_Resume(BSD_HANDLE hDev);

BSD_STATUS BSD_GetDevice(const char *Name, BSD_DEVICE *pDev);
BSD_STATUS BSD_GetDeviceByHandle(BSD_HANDLE hDev, BSD_DEVICE *pDev);

#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#endif /* BSD_H */


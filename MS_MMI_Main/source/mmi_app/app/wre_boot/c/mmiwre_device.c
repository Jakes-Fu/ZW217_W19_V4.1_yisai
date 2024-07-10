#ifdef WRE_SUPPORT
/****************************************************************************
** File Name:      mmiwre_export.c
** Author:                                                                  
** Date:            2011/08/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define game module outside API .
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                         
** 01/09/2012       wei.zhou           Create
** 
****************************************************************************/
#include "os_api.h"
#include "msensor_drv.h"
#include "Mmk_app.h"
//#include "mmiwre_other.h"

#define MAX_DEVICE_NAME     16

#define DEVICE_BT_NAME              L"BLUETOOTH"
#define DEVICE_GPS_NAME             L"GPS"
#define DEVICE_GSENSOR_NAME         L"GSENSOR"

#define FILE_DEVICE_BT                  0x00000037
#define FILE_DEVICE_GSENSOR             0x00000038
#define FILE_DEVICE_GPS                 0x00000039
#define FILE_DEVICE_UNKNOWN             0x00000022

#define FILE_ANY_ACCESS                 0
#define FILE_READ_ACCESS          ( 0x0001 )    // file & pipe
#define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe

/* method codes */
#define  METHOD_BUFFERED     0
#define  METHOD_IN_DIRECT    1
#define  METHOD_OUT_DIRECT   2
#define  METHOD_NEITHER      3

typedef enum
{
    GSENSOR_STATUS_NOT_INIT = 0,
    GSENSOR_STATUS_INIT,
    GSENSOR_STATUS_OPEN,	
    GSENSOR_STATUS_CLOSE,
    GSENSOR_STATUS_MAX
}GSENSOR_STATUS_E;

typedef struct devicectrl
{
    int     (*open)(void);
    int     (*close)(void);
    int     (*read)(void *buffer, unsigned int count, unsigned int *pcount);
    int     (*write)(const void *buffer, unsigned int count, unsigned int *pcount);
    int     (*ioctrl)(uint32 IoControlCode, void *InBuffer_ptr, uint32 nInBufferSize, void *OutBuffer_ptr, uint32 nOutBufferSize, uint32 *BytesReturned_ptr);
    uint16  DeviceName[MAX_DEVICE_NAME];
    uint32  DeviceNo;
    uint32  ref;
}DEVICE_CTRL, *PDEVICE_CTRL;

typedef struct _DEVICE_USERDATA_T
{
    PDEVICE_CTRL    pDeviceCtrl;
    union
    {
        struct
        {
            int k;
        } bluetoothdev;
        struct
        {
            int k;
        } gpsdev;
        struct
        {
            int status;
        } gsensordev;
    }DEVICE_USERDATA_U;
}DEVICE_USERDATA_T ,*PDEVICE_USERDATA_T;

/* blue tooth apis */
LOCAL int bt_open(void);
LOCAL int bt_close(void);
LOCAL int bt_read(void *buffer, unsigned int count, unsigned int *pcount);
LOCAL int bt_write(const void *buffer, unsigned int count, unsigned int *pcount);
LOCAL int bt_ioctrl(uint32 IoControlCode, void *InBuffer_ptr, uint32 nInBufferSize, void *OutBuffer_ptr, uint32 nOutBufferSize, uint32 *BytesReturned_ptr);

/* gsensor apis */
LOCAL int gs_open(void);
LOCAL int gs_close(void);
LOCAL int gs_read(void *buffer, unsigned int count, unsigned int *pcount);
LOCAL int gs_write(const void *buffer, unsigned int count, unsigned int *pcount);
LOCAL int gs_ioctrl(uint32 IoControlCode, void *InBuffer_ptr, uint32 nInBufferSize, void *OutBuffer_ptr, uint32 nOutBufferSize, uint32 *BytesReturned_ptr);


LOCAL const DEVICE_CTRL s_Device_ctrl[] =
{
    {
        bt_open, 
        bt_close, 
        bt_read, 
        bt_write, 
        bt_ioctrl, 
        DEVICE_BT_NAME, 
        FILE_DEVICE_BT, 
        0,
    },
    {
        gs_open, 
        gs_close, 
        gs_read, 
        gs_write, 
        gs_ioctrl, 
        DEVICE_GSENSOR_NAME, 
        FILE_DEVICE_GSENSOR, 
        0,
    }
};


LOCAL int Is_WreDevice(uint16 *deviceName_ptr)
{
    int i;
    for(i = 0; i < sizeof(s_Device_ctrl)/sizeof(DEVICE_CTRL); i++)
    {
        if(MMIAPICOM_Wstrcmp(deviceName_ptr, (uint16 *)s_Device_ctrl[i].DeviceName) == 0)
        {
            return i;
        }
    }
    return -1;
}

/*  need CreateFile call  */
void *Wre_CreateDevice(uint16 *deviceName_ptr)
{
    int                 deviceno;
    PDEVICE_USERDATA_T  pDevUser;

    deviceno = Is_WreDevice(deviceName_ptr);
    if(deviceno == -1)
    {
        return PNULL;
    }
    SCI_TRACE_LOW("Wre_CreateDevice");
    pDevUser = (PDEVICE_USERDATA_T)SCI_ALLOCA(sizeof(DEVICE_USERDATA_T));
    memset(pDevUser, 0, sizeof(DEVICE_USERDATA_T));
    pDevUser->pDeviceCtrl = &s_Device_ctrl[deviceno];
    pDevUser->pDeviceCtrl->open();

    return pDevUser;
}


/*  need CloseFile call  */
void Wre_DestroyDevice(void *device_ptr, BOOLEAN CFlag)
{
    PDEVICE_USERDATA_T  pDevUser = (PDEVICE_USERDATA_T)device_ptr;

    if(CFlag)
    {
       pDevUser->pDeviceCtrl->close();
    }
    
    if(pDevUser != NULL)
    {
       SCI_TRACE_LOW("Wre_DestroyDevice");
       SCI_FREE(pDevUser);
    }
}


/*  need ReadFile call  */
BOOLEAN Wre_ReadDevice(void *device_ptr, void *buffer, unsigned int count, unsigned int *poutcnt)
{
    PDEVICE_USERDATA_T  pDevUser = (PDEVICE_USERDATA_T)device_ptr;

    SCI_TRACE_LOW("Wre_ReadDevice");
    pDevUser->pDeviceCtrl->read(buffer, count, poutcnt);
    return TRUE;
}

/*  need WriteFile call  */
BOOLEAN Wre_WriteDevice(void *device_ptr, void *buffer, unsigned int count, unsigned int *poutcnt)
{
    PDEVICE_USERDATA_T  pDevUser = (PDEVICE_USERDATA_T)device_ptr;

    SCI_TRACE_LOW("Wre_WriteDevice");
    pDevUser->pDeviceCtrl->write(buffer, count, poutcnt);
    return TRUE;
}

BOOLEAN Wre_DeviceIoControl(
    void   *device_ptr,
    uint32 IoControlCode,
    void   *InBuffer_ptr,
    uint32 nInBufferSize,
    void   *OutBuffer_ptr,
    uint32 nOutBufferSize,
    uint32 *nBytesReturned,
    uint32 *BytesReturned_ptr)
{
    PDEVICE_USERDATA_T  pDevUser = (PDEVICE_USERDATA_T)device_ptr;

    if(device_ptr == NULL)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("Wre_DeviceIoControl");
    pDevUser->pDeviceCtrl->ioctrl(IoControlCode, InBuffer_ptr, nInBufferSize, OutBuffer_ptr, nOutBufferSize, BytesReturned_ptr);
    return TRUE;
}

int bt_open(void)
{
    return 0;
}

int bt_close(void)
{
    return 0;
}

int bt_read(void *buffer, unsigned int count, unsigned int *pcount)
{
    return 0;
}

int bt_write(const void *buffer, unsigned int count, unsigned int *pcount)
{
    return 0;
}


#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define IOCTL_BT_SENDFILE     CTL_CODE(FILE_DEVICE_BT, 0x0103, METHOD_BUFFERED, FILE_READ_ACCESS)

typedef struct tagBTSendFile
{
    wchar_t    *filepath_name;
    uint16      filepath_len;
    uint32      file_size;
    BOOLEAN     is_temp_file;
}BT_SENDFILE, *PBT_SENDFILE;

int bt_ioctrl(uint32 IoControlCode, void *InBuffer_ptr, uint32 nInBufferSize, void *OutBuffer_ptr, uint32 nOutBufferSize, uint32 *BytesReturned_ptr)
{
    switch(IoControlCode)
    {
        case IOCTL_BT_SENDFILE:
            {
                PBT_SENDFILE p;
                p = (PBT_SENDFILE)InBuffer_ptr;
                //MMIBT_SendFile(p->filepath_name, p->filepath_len, p->file_size, p->is_temp_file);
            }
            break;
        
        default:
            break;
    }
    return 0;
}


int gs_open(void)
{
    BOOLEAN     result = FALSE;

#ifdef MOTION_SENSOR_TYPE
#ifndef WIN32
    if ( MSENSOR_SUCCESS == MSensor_Open(MSENSOR_TYPE_ACCELEROMETER) )
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
#else
    result = TRUE;
#endif
#endif

    return result;
}

int gs_close(void)
{
    BOOLEAN     result = FALSE;

#ifdef MOTION_SENSOR_TYPE
#ifndef WIN32
    if ( MSENSOR_SUCCESS == MSensor_Close(MSENSOR_TYPE_ACCELEROMETER) )
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
#else
    result = TRUE;
#endif
#endif

    return result;
}

int gs_read(void *buffer, unsigned int count, unsigned int *pcount)
{
    return 0;
}

int gs_write(const void *buffer, unsigned int count, unsigned int *pcount)
{
    return 0;
}

int gs_ioctrl(uint32 IoControlCode, void *InBuffer_ptr, uint32 nInBufferSize, void *OutBuffer_ptr, uint32 nOutBufferSize, uint32 *BytesReturned_ptr)
{
    BOOLEAN     result = FALSE;

#ifdef MOTION_SENSOR_TYPE
#ifndef WIN32
    //MSENSOR_MOTION_DATA_T *msensor_data = (MSENSOR_MOTION_DATA_T *)OutBuffer_ptr;

    if(IoControlCode >= MSENSOR_IOCTL_MAX || OutBuffer_ptr == NULL)
    {
        return FALSE;
    }

    if ( MSENSOR_SUCCESS == MSensor_ioctl( MSENSOR_TYPE_ACCELEROMETER, IoControlCode, (void *)InBuffer_ptr ) )
    {
        OutBuffer_ptr = InBuffer_ptr;
        nOutBufferSize = sizeof(OutBuffer_ptr);
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
#else
    result = TRUE;
#endif
#endif
    
    return result;
}

#endif

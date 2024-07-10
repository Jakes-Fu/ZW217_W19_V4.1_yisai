#ifndef _IMEI_H
#define _IMEI_H
#include "sci_types.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifndef  WIN32 
#define	__PACKED	__packed
#else 
#define	 __PACKED
#endif

#define  RM_CALI_NV_IMEI1_MSK   0x01
#define  RM_CALI_NV_IMEI2_MSK   0x02
#define  RM_CALI_NV_BT_MSK      0x04
#define  RM_CALI_NV_GPS_MSK     0x08
#define  RM_CALI_NV_GUID_MSK    0x10
#define  RM_CALI_NV_SN_MSK      0x20
#define  RM_CALI_NV_CHECK_MSK      0x40

#define  RM_CALI_NV_IMEI3_MSK  RM_CALI_NV_GUID_MSK

#define  RM_CALI_NV_SN_MSK      0x20
#define  RM_CALI_NV_IMEI4_MSK  RM_CALI_NV_SN_MSK

#define  RM_CALI_NV_WIFI_MSK      0x40


#define  RM_VALID_CMD_MSK	0x7f

#define	MAX_MSG_SIZE		(8*1024)
#define	RW_MASK			(BIT_7)
#define	WRITE_MODE		0
#define	MSG_NACK		0
#define	MSG_ACK			1

#define	INVALID_IMEI_CMD	(0xff)

#define	DATATYPE(x)		(x&0x7f)

typedef BOOLEAN (*NV_Update_Func)(uint8 *data, uint32 length);
typedef uint32  (*NV_Read_Func)(uint8 *data, uint32 max_length);

typedef enum{
	IMEI_ERR_NONE=0,
	IMEI_CRC_ERR = 1,
	IMEI_CMD_ERR,
	IMEI_SAVE_ERR,
	IMEI_READ_ERR
		
}ERR_IMEI_E;

typedef __PACKED struct tag_Command
{
	uint32 reserved;
	uint16 frameSize;
	uint8  command;
	uint8  subCommand;
}COMMAND_T;

typedef struct
{
    uint8 engine_sn[24];
    uint8 map_version[4];
    uint8 activate_code[16];
} gpsNVInfo_t;

typedef struct _REF_NVWriteDirect {
	uint8			imei1[MN_MAX_IMEI_LENGTH];    /*imei1 number 8byte*/
	uint8			imei2[MN_MAX_IMEI_LENGTH];    /*imei2 number 8byte*/
	BT_ADDRESS    	bt_add;
	gpsNVInfo_t     gps_info;
       uint8            		wifi_add[6];
       uint8            		reserve1[2]; 
       uint8            		imei3[MN_MAX_IMEI_LENGTH];    /*imei3 number 8byte*/
       uint8            		imei4[MN_MAX_IMEI_LENGTH];    /*imei4 number 8byte*/     
       uint8            		reserved[16];
}REF_NVWriteDirect_T,*pREF_NVWriteDirect_T;
typedef struct TOOL_Callback_TAG
{
	NV_Update_Func	update;
	NV_Read_Func	read;
}IMEI_TOOL_E;


typedef struct _REF_IMEI_CMD_INF {
	uint16	cmd_id;
	uint16	data_len;
	uint8	*data;
}ImeiCmdInf_T;

PUBLIC uint32 IMEI_EfuseFlushTempBuf(void);
PUBLIC uint32 IMEI_EfuseWriteTempBuf(uint16 ItemID, uint8 *data,uint32 length);
PUBLIC uint32 IMEI_EfuseRead(uint16 ItemID, uint8 *data,uint32 length);
#ifdef __cplusplus
    }
#endif

#endif /*End init.h*/

/******************************************************************************
 ** File Name:      SCM_api.h                                                     *
 ** Author:                                                           *
 ** DATE:           10/26/2007                                                *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 this file define the struct and API of SCM.               *
 **                 (Storage card manager                                    *
 **                                                                           * 
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/26/2007               Create.                   Jason.wu                *
 ******************************************************************************/


#ifndef _SCM_API_H_
#define _SCM_API_H_

#include "sci_types.h"

//-------------------------------------
//	The param of Read command
//-------------------------------------
typedef struct
{
/*IN*/
	uint32 startBlock;	// the first sect id that will be read
	uint32 num;		// the count of sects that will be read
	uint8* buf;		// the data that be read will be stored in this place
}
SCM_READ_PARM_T;

//-------------------------------------
//	The param of Write command
//-------------------------------------
typedef struct
{
/*IN*/
	uint32 startBlock;	// the first sect id that will be write
	uint32 num;		// the count of sects that will be write
	uint8* buf;		// the data that be write is stored in this place
}
SCM_WRITE_PARM_T;

/*

*/
typedef struct
{
	uint32	num;
	uint8*	buf;
}SCM_SCTBUF_ITEM_INFO_T;

typedef struct
{
	uint32				cnt;
	SCM_SCTBUF_ITEM_INFO_T*		item;
}
SCM_SCTBUF_INFO_T;

//-------------------------------------
//	The param of ReadExt command
//-------------------------------------
typedef struct
{
/*IN*/
	uint32			startBlock;	// the first sect id that will be read
	SCM_SCTBUF_INFO_T*	sctInfo;
}
SCM_READEXT_PARM_T;

//-------------------------------------
//	The param of WriteExt command
//-------------------------------------
typedef struct
{
/*IN*/
	uint32			startBlock;	// the first sect id that will be write
	SCM_SCTBUF_INFO_T*	sctInfo;
}
SCM_WRITEEXT_PARM_T;

//-------------------------------------
//	The param of Erase command
//-------------------------------------
typedef struct
{
/*IN*/
	uint32 startBlock;	// the first sect id that will be erased
	uint32 endBlock;	// the last sect id that will be erased
}
SCM_ERASE_PARM_T;

//-------------------------------------
//	The param of GetCapacity command
//-------------------------------------
typedef struct
{
/*OUT*/
	uint32 capcity;	// when command is performed , capacity of card will be stored in this variable
}
SCM_GETCAPCITY_T;

#if defined(SPRD_SUPPORT_MCEX)
typedef struct
{
/*IN*/
	BOOLEAN* ifEnterMcex;
}SCM_ENTERMCEX_T;

typedef struct
{
	uint32 dummy;
}SCM_RESET_TRM_T;

typedef struct
{
/*IN*/
	uint8* buf;	// length of buffer must be 512
}SCM_READ_REC_CMD_T;

typedef struct
{
/*IN*/
	uint8* buf;	// length of buffer must be 512
}SCM_WRITE_REC_CMD_T;

typedef struct
{
/*IN*/
	uint32 arg;
	uint8* buf;	// length of buffer must be 32
}SCM_SEND_PSI_T;

typedef struct
{
	uint32 dummy;
}SCM_EXITMCEX_T;

#endif
//-------------------------------------
//	The command of IO Ctrl
//-------------------------------------
typedef enum
{
	SCM_FUNC_REINIT = 0,
	SCM_FUNC_READ = 1,
	SCM_FUNC_WRITE = 2,
	SCM_FUNC_ERASE =3,
	SCM_FUNC_GETCAPCITY = 4,
#if defined(SPRD_SUPPORT_MCEX)
	SCM_FUNC_ENTERMCEX = 5,
	SCM_FUNC_RESETTRM = 6,
	SCM_FUNC_READ_REC_CMD = 7,
	SCM_FUNC_WRITE_REC_CMD = 8,
	SCM_FUNC_SEND_PSI = 9,
	SCM_FUNC_EXITMCEX = 10,
#endif
	SCM_FUNC_READEXT = 11,
	SCM_FUNC_WRITEEXT = 12,

	SCM_FUNC_DUMMY
}
SCM_FUN_E;

//-------------------------------------
//	The Param of command
//-------------------------------------
typedef union
{
	SCM_READ_PARM_T readParam;
	SCM_WRITE_PARM_T writeParam;
	SCM_READEXT_PARM_T readExtParam;
	SCM_WRITEEXT_PARM_T writeExtParam;
	SCM_ERASE_PARM_T eraseParam;
	SCM_GETCAPCITY_T getCapcityParam;
#if defined(SPRD_SUPPORT_MCEX)
	SCM_ENTERMCEX_T enterMcex;
	SCM_RESET_TRM_T resetTrm;
	SCM_READ_REC_CMD_T readRecCmd;
	SCM_WRITE_REC_CMD_T writeRecCmd;
	SCM_SEND_PSI_T	sendPsi;
	SCM_EXITMCEX_T exitMcex;
#endif
}
SCM_PARAM_T;
//==========
typedef enum
{
	SLOT_PWRON,
	SLOT_PWROFF
}
SLOT_PWR_SWITCH_E;
//==========

//-------------------------------------
//	The Device NO. of system
//-------------------------------------
typedef enum
{
	SCM_SPI_0 = 0,
	SCM_SPI_1,
	SCM_SPI_2,
	SCM_SDIO_0,
	SCM_SDIO_1,
	SCM_SDIO_2,
	SCM_SDIO_3,
	SCM_SDIO_4,
	SCM_SDIO_5,
	SCM_SDIO_6,
	SCM_SDIO_7,
	MAX_DEVICE_COUNT
}
SCM_DEVICE_NAME_E;

//-------------------------------------
//	The Slot NO. of system
//-------------------------------------
typedef enum
{
	SCM_SLOT_0,
	SCM_SLOT_1,
	SCM_SLOT_2,
	SCM_SLOT_3,
	SCM_SLOT_4,
	SCM_SLOT_5,
	SCM_SLOT_6,
	SCM_SLOT_7,
	SCM_SLOT_8,
	SCM_MAX_SLOT_NO
}
SCM_SLOT_NAME_E;

//-------------------------------------
//	Follow function must be realized by card protocol
//-------------------------------------
typedef BOOLEAN (*SCM_OPEN)(void);		// open protocol
typedef BOOLEAN (*SCM_CLOSE)(void);	// close protocol
typedef BOOLEAN (*SCM_INIT)(void);		// init protocol
typedef BOOLEAN (*SCM_READ)(uint32 /*startBlock*/,uint32 /*num*/,uint8* /*buf*/);	// read card
typedef BOOLEAN (*SCM_WRITE)(uint32 /*startBlock*/,uint32 /*num*/,uint8* /*buf*/);	// write card
typedef BOOLEAN (*SCM_READEXT)(uint32 /*startBlock*/,SCM_SCTBUF_INFO_T* /*sectInfo*/);
typedef BOOLEAN (*SCM_WRITEEXT)(uint32 /*startBlock*/,SCM_SCTBUF_INFO_T* /*sectInfo*/);
typedef BOOLEAN (*SCM_ERASE)(uint32 /*startBlock*/,uint32 /*endBlock*/);			// erase card
typedef uint32 (*SCM_GETCAPCITY)(void);		// get capacity of card
#if defined(SPRD_SUPPORT_MCEX)
typedef BOOLEAN (*SCM_ENTERMCEX)(BOOLEAN* ifEnterMcex);
typedef BOOLEAN (*SCM_RESETTRM)(void);
typedef BOOLEAN (*SCM_SENDPSI)(uint32 arg,uint8* rspBuf);
typedef BOOLEAN (*SCM_READSECCMD)(uint8* rspBuf);
typedef BOOLEAN (*SCM_WRITESECCMD)(uint8* rspBuf);
typedef BOOLEAN (*SCM_EXITMCEX)(void);
#endif
typedef BOOLEAN (*SCM_ERRPROC)(SCM_FUN_E/*cmd*/,SCM_PARAM_T* /*param*/);	// when error happend this function is used to 
/*TRUE card exist,FALSE ,card not exist*/
typedef BOOLEAN (*SCM_ISEXIST)(void);
typedef BOOLEAN (*SCM_PWR)(SCM_SLOT_NAME_E,SLOT_PWR_SWITCH_E);

//-------------------------------------
//	Slot description
//-------------------------------------
typedef struct
{
	SCM_SLOT_NAME_E slot_name;
	SCM_OPEN	OpenProtocol;
	SCM_CLOSE	CloseProtocol;
	SCM_INIT	InitCard;
	SCM_ISEXIST	IsExist;
	SCM_PWR	pwr;

	SCM_READ	Read;
	SCM_WRITE	Write;
	SCM_READEXT	ReadExt;
	SCM_WRITEEXT	WriteExt;
	SCM_ERASE	Erase;
	SCM_GETCAPCITY GetCapcity;
#if defined(SPRD_SUPPORT_MCEX)
	SCM_ENTERMCEX	EnterMcex;
	SCM_RESETTRM	ResetTrm;
	SCM_SENDPSI	SendPsi;
	SCM_READSECCMD	ReadRecCmd;
	SCM_WRITESECCMD	WriteRecCmd;
	SCM_EXITMCEX	ExitMcex;
#endif
	SCM_ERRPROC	ErrProc;

	SCM_DEVICE_NAME_E DeviceName;
}
SCM_SLOT_DESCRIPTION_T;
//==========


//==========
/*****************************************************************************/
//  Description:   Initialize SCM system
//  Author: Jason.wu
//  Param
//       NONE
//  Return:
//       TRUE: success
//       FALSE : fail
//  Note: 
//       This function will be reserved in the future.
/*****************************************************************************/
PUBLIC BOOLEAN SCM_Init(void);

/*****************************************************************************/
//  Description:   Create slot in SCM system
//  Author: Jason.wu
//  Param
//       description : the information of slot
//  Return:
//       TRUE: success
//       FALSE : fail
//  Note: 
//       This function will be reserved in the future.
/*****************************************************************************/
//PUBLIC BOOLEAN SCM_RegSlot(SCM_SLOT_DESCRIPTION_T description);

/*****************************************************************************/
//  Description:   Remove slot in SCM system
//  Author: Jason.wu
//  Param
//       slot_name : the NO. of slot
//  Return:
//       TRUE: success
//       FALSE : fail
//  Note: 
//      This function will be reserved in the future.
/*****************************************************************************/
//PUBLIC BOOLEAN SCM_UnRegSlot(SCM_SLOT_NAME_E slot_name);

/*****************************************************************************/
//  Description:
//      scm has 3 service background.
//      1 power san and power saving
//      2 error process
//      3 event indicate
//	    this api can start or stop service.
//  Author: Jason.wu
//  Param
//      slotNO		: slot number where you want to set
//      serviceId	: service index
//      onOf		: start or stop
//  Return:
//       none
//  Note: 
//
/*****************************************************************************/
typedef enum
{
	SCM_PWR_SAVING		= 0
//	SCM_EVT_IND			= 1,
//	SCM_ERR_PROC		= 2
}SCM_SERVICE_ID;

typedef enum
{
	SCM_SERVICE_ON		= 0,
	SCM_SERVICE_OFF	= 1
}SCM_SERVICE_ONOFF;

PUBLIC void SCM_SetService(SCM_SLOT_NAME_E slot_name, SCM_SERVICE_ID serviceId, SCM_SERVICE_ONOFF onOff);

/*****************************************************************************/
//  Description:   Regist event in slot,when the event happend ,slot will inform to the task
//  Author: Jason.wu
//  Param
//       slot_name : the NO. of slot
//       clientId : the block id of task that want to recieve the event
//       scmEvent : the event that the task want to know
//       callback : when the event happened , SCM will call this function to infom the task
//  Return:
//       TRUE: success
//       FALSE : fail
//  Note: 
/*****************************************************************************/
//======for SCM Event====
typedef enum
{
	SCM_NONE_EVT = 0,
	SCM_ERROR_EVT = 0x1,
	SCM_PLUG_IN_EVT = 0x2,
	SCM_PLUG_OUT_EVT = 0x4,
	SCM_ALL_EVT = SCM_ERROR_EVT|SCM_PLUG_IN_EVT|SCM_PLUG_OUT_EVT
}
SCM_EVENT_E;
typedef void (*SCM_EVENT_CALLBACK)(SCM_SLOT_NAME_E,SCM_EVENT_E);

PUBLIC BOOLEAN SCM_RegEvent(SCM_SLOT_NAME_E slot_name,BLOCK_ID clientId,SCM_EVENT_E scmEvent,SCM_EVENT_CALLBACK callback);

/*****************************************************************************/
//  Description:   Get the status of slot
//  Author: Jason.wu
//  Param
//       slot_name : the NO. of slot
//  Return:
//       SCM_STATUS_NORMAL : card is inserted and in good condition
//       SCM_STATUS_ERROR : card is inserted and in error condition
//       SCM_NOT_EXIST : card is not inserted
//       SCM_SYSTEMERR : systemError such as slot is not registed
//  Note: 
/*****************************************************************************/
typedef enum
{
	SCM_STATUS_NORMAL,	// card is inserted and in good condition
	SCM_STATUS_ERROR,		// card is inserted and in error condition
	SCM_NOT_EXIST,	// card is not inserted
	SCM_SYSTEMERR	// systemError such as slot is not registed
}
SCM_STATUS;
PUBLIC SCM_STATUS SCM_GetSlotStatus(SCM_SLOT_NAME_E slot_name);

/*****************************************************************************/
//  Description:   call the command of slot
//  Author: Jason.wu
//  Param
//       slot_name : the NO. of slot
//       cmd : the function that will be performed
//       param : the param of cmd
//  Return:
//       TRUE: success
//       FALSE : fail
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SCM_IOCTL(SCM_SLOT_NAME_E slot_name,SCM_FUN_E cmd,SCM_PARAM_T* param);

/*****************************************************************************/
//  Description:   When card is pluged into slot ,this function must be called to informed SCM
//  Author: Jason.wu
//  Param
//       slot_name : the NO. of slot
//  Return:
//       NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SCM_PlugIn(SCM_SLOT_NAME_E slot_name);

/*****************************************************************************/
//  Description:   When card is pluged out from slot ,this function must be called to informed SCM
//  Author: Jason.wu
//  Param
//       slot_name : the NO. of slot
//  Return:
//       NONE
//  Note: 
/*****************************************************************************/
PUBLIC void SCM_PlugOut(SCM_SLOT_NAME_E slot_name);

#endif

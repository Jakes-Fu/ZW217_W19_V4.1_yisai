
/******************************************************************************
 ** File Name:      tp_multi_drv.h                                            *
 ** DATE:           2011.03.19                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2011.03.19                         Create.                               *
 ******************************************************************************/
/******************************************************************************/
#ifndef _TP_MULTI_DRV_H_
#define _TP_MULTI_DRV_H_

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_types.h"
#include "tp_srv.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" 
{
#endif

#define TP_MULTI_DRV_DEBUG
#ifdef TP_MULTI_DRV_DEBUG
    #define TP_MULTI_DRV_PRINT	SCI_TRACE_LOW
#else
    #define TP_MULTI_DRV_PRINT	
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/

#define TP_MULTI_ERROR                SCI_ERROR
#define TP_MULTI_SUCCESS              SCI_SUCCESS

/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
typedef struct
{
   uint32 	(*init)(void);
   int    	(*open)(void);
   int      (*write)(void);
   uint32   (*read)(TPDSVR_SIG_T *data);
   uint32   (*ioctl)(uint32 cmd, void *arg);
   void     (*suspend)(void);
   void     (*resume)(void);
   int    	(*close)(void);
   int      (*deinit)(void);  
}TPC_OPERATION_T;

/**---------------------------------------------------------------------------*
 **                             Variables                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/******************************************************************************/
//  FUNCTION:     TPC_Init
// Description:   This function is used to init Capactive TouchPanel.
// Dependence: 
// Note:   If touchscreen isn't capacitive style, internal touchscreen chip is used.
/******************************************************************************/
PUBLIC uint32 TPC_Init(void);

/*****************************************************************************/
//  FUNCTION:     TPC_Event_Mode
//  Description:  touch message from capacitive touchscreen chip will be got.
//	Note:
/*****************************************************************************/
PUBLIC uint32 TPC_Event_Mode(TP_MSG_E *event_mode);
/*****************************************************************************/
//  FUNCTION:     TPC_Suspend
//  Description:  The function will suspend capacitive touchscreen chip.
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN TPC_Suspend(void);

/*****************************************************************************/
//  FUNCTION:     TPC_Resume
//  Description:  The function will resume from capacitive touchscreen chip.
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN TPC_Resume(void);

/*****************************************************************************/
//  FUNCTION:     TPC_Ioctl
//  Description:  Data from capacitive touchpanel chip.
//	Note:
/*****************************************************************************/
PUBLIC uint32 TPC_Ioctl(uint32 cmd, void *arg);
/*****************************************************************************/
//  FUNCTION:     HandleTPMessage
//  Description:  This functoion to handle multi-TP gpio message
//                  
//	Note:         This function will be called in P_TP task in tp_multi_srv.c file
/*****************************************************************************/
PUBLIC void HandleTPMessage(void);

#ifdef __cplusplus
}
#endif

/******************************************************************************/
#endif //#ifndef _TP_MULTI_DRV_H_
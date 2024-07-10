/******************************************************************************
 ** File Name:    UPCC_drv.h                                                  *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/22/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/22/2006     yuhua.shi       Create.                                     *
 ******************************************************************************/
#ifndef _UPCC_DRV_H_
    #define _UPCC_DRV_H_


/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */    
#include "upcc_common.h"    
#include "dal_dcamera.h"
#include "upcc_camera.h"

/**---------------------------------------------------------------------------*
 **                         COMPILER FLAG                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {    
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/    

#define GET_CUR							0x81
#define GET_MIN							0x82
#define GET_MAX							0x83
#define GET_RES							0x84
#define GET_LEN							0x85
#define GET_INF							0x86
#define GET_DEF							0x87
#define SET_CUR							0x01

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_EP0Config  (void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_EP1Config  (void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_EP3Config  (void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_GetDevDescriptor (uint8 bDirection,uint16 wValue,\
                            uint16 wIndex ,uint16 wLen);
                            
/*****************************************************************************/
//  Description:    Call this function to send data to special end point.
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           pBuf  must be aligned  4  ;
//                  len   must be less than 64 ;
//                  ep_id must be less than 6  ;
/*****************************************************************************/
PUBLIC int UPCC_EPxSendData (uint8 ep_id ,void * pBuf,uint8 len);

/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_EPxSendNextPacket(void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_ResetCache (void);

/*****************************************************************************/
//  Description:    set the lock flag of cache 
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_SetCacheLockFlag(BOOLEAN flag);

/*****************************************************************************/
//  Description:    get the lock flag of cache 
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_GetCacheLockFlag(void);

/*****************************************************************************/
//  Description: make the JPEG data the store in cache
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_MakeCacheData(UPCC_JPG_FRAME_T *p_frame);

/*****************************************************************************/
//  Description: send the cache data           
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SendCache(void);

/*****************************************************************************/
//  Description: get a frame data and send it to pc
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_SendAFrame(UPCC_JPG_FRAME_T *p_frame);

/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_SetCur(uint8 selector, uint8 length);

/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_SendCommand (uint8 request,uint8 fea,uint8 param);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_InitFeaValArray(void);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_Fea_Control(uint8 request, uint8 selector, uint8 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_StreamingInterface_GetCur(uint16 selector,uint16 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_StreamingInterface_SetCur(uint16 selector,uint16 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetCur(uint8 selector,uint8 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetMin(uint8 selector,uint8 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetMax(uint8 selector,uint8 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetRes(uint8 selector,uint8 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetLen(uint8 selector,uint8 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetInf(uint8 selector,uint8 length);
/*****************************************************************************/
//  Description:
//	Global resource 
//  Author:
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_PU_GetDef(uint8 selector,uint8 length);
/**---------------------------------------------------------------------------*
**                          COMPILER FLAG                                     *
**----------------------------------------------------------------------------*/    
#ifdef __cplusplus
    }
#endif    


/**---------------------------------------------------------------------------*/   
#endif //_UPCC_DRV_H_
//end UPCC_drv.h

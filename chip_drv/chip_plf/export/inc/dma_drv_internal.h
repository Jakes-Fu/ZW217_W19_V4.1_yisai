/******************************************************************************
 ** File Name:    dma_drv_internal.h                                          *
 ** Author:       Guofu.Huang                                                 *
 ** DATE:         2010-2-28                                                   *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:  dma driver infterface specfication                          *
 **               Note:In DMA7, data blcok in DMA9 was named busrt,           *
 **               Here we all used block.*
 **               any advice, please fell free to contact me.                 *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 2010-2-9      Guofu.Huang     Created, Rewrite dma driver                 *
 ******************************************************************************/
#ifndef _DMA_DRV_INTERNAL_H_
#define _DMA_DRV_INTERNAL_H_

#include "mmu_drvapi.h"

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define DMA_CHANNELID_INVALID               (0xFFFFFFFF)
#define DMA_LINKLIST_HANDLE_INVALID         (0xFFFFFFFF)
#define DMA_UID_SOFT                        (0)
#define DMA_UID_MAX                         (32)

//---------------------------------------------------------------------------
// INT type macros
//
//int status BIT: 3bits value
#define DMA_INT_BIT_BLOCK  0   // bit 0
#define DMA_INT_BIT_TRANS  1   // bit 1
#define DMA_INT_BIT_LIST   2   // bit 2
#define DMA_INT_BIT_FRAG   3   // bit 3
#define DMA_INT_BIT_LISR   31   // bit 31
#define DMA_CHN_INT_TYPE_NONE               0
#define DMA_CHN_INT_TYPE_BLOCKDONE          (1<<DMA_INT_BIT_BLOCK)
#define DMA_CHN_INT_TYPE_TRANSDONE          (1<<DMA_INT_BIT_TRANS)
#define DMA_CHN_INT_TYPE_FRAGDONE           (1<<DMA_INT_BIT_FRAG)
#define DMA_CHN_INT_TYPE_LISTDONE           (1<<DMA_INT_BIT_LIST)
#define DMA_CHN_INT_TYPE_LISR               (1<<DMA_INT_BIT_LISR)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
//int status type: 3bits value
//DMA_CHN_INT_TYPE_BLOCKDONE | DMA_CHN_INT_TYPE_TRANSDONE | DMA_CHN_INT_TYPE_LISTDONE
typedef void (*DMA_HAL_ISR_HANDLER) (uint32 inttype);

typedef uint32 DMA_LINKLIST_HANDLE;

//dma channel type
typedef enum _DMA_CHN_TYPE
{
    DMA_CHN_TYPE_HARD = 0,
    DMA_CHN_TYPE_SOFT,
    DMA_CHN_TYPE_MAX
} DMA_CHN_TYPE_E;
typedef enum _DMA_CHN_TRSF_TYPE
{
    DMA_CHN_TRSF_TYPE_STANDARD = 0,
    DMA_CHN_TRSF_TYPE_FULL,
    DMA_CHN_TRSF_TYPE_MAX
} DMA_CHN_TRSF_TYPE_E;

//DMA priority
typedef enum _DMA_CHN_PRIORITY
{
    DMA_CHN_PRIORITY_0  = 0, // LOWEST PRI
    DMA_CHN_PRIORITY_1,
    DMA_CHN_PRIORITY_2,
    DMA_CHN_PRIORITY_3,      // HIGHIST
    DMA_CHN_PRIORITY_MAX
} DMA_CHN_PRIORITY_E;

//endian type
typedef enum _DMA_ENDIANTYPE
{
    DMA_ENDIANTYPE_BIG = 0,
    DMA_ENDIANTYPE_LITTLE,
    DMA_ENDIANTYPE_MAX
} DMA_ENDIANTYPE_E;

//endian switch mode
typedef enum _DMA_ENDIANSWITCHMODE
{
    DMA_ENDIANSWITCHMODE_UN = 0,
    DMA_ENDIANSWITCHMODE_FULL,
    DMA_ENDIANSWITCHMODE_MODE0,
    DMA_ENDIANSWITCHMODE_MODE1,
    DMA_ENDIANSWITCHMODE_MAX
} DMA_ENDIANSWITCHMODE_E;

//data width
typedef enum _DMA_DATAWIDTH
{
    DMA_DATAWIDTH_BYTE = 0,
    DMA_DATAWIDTH_HALFWORD,
    DMA_DATAWIDTH_WORD,
    DMA_DATAWIDTH_MAX
} DMA_DATAWIDTH_E;

//work mode
typedef enum _DMA_CHN_WORKMODE
{
    DMA_CHN_WORKMODE_NORMAL = 0,
    DMA_CHN_WORKMODE_LINKLIST,
    DMA_CHN_WORKMODE_SOFTLIST,
    DMA_CHN_WORKMODE_MAX
} DMA_CHN_WORKMODE_E;

//request mode
typedef enum _DMA_CHN_REQMODE
{
    DMA_CHN_REQMODE_NORMAL = 0,
    DMA_CHN_REQMODE_TRASACTION,
    DMA_CHN_REQMODE_LIST,
    DMA_CHN_REQMODE_INFINITE,
	DMA_CHN_REQMODE_FRAGMENT,    
    DMA_CHN_REQMODE_MAX
} DMA_CHN_REQMODE_E;

//burst mode
typedef enum _DMA_BLOCKMODE
{
    DMA_BLOCKMODE_SINGLETRANS  = 0,
    DMA_BLOCKMODE_INCR,
    DMA_BLOCKMODE_INCR4,
    DMA_BLOCKMODE_INCR8,
    DMA_BLOCKMODE_INCR16,
    DMA_BLOCKMODE_MAX
} DMA_BLOCKMODE_E;

typedef struct _DMA_CACHE_CONFIG
{
    uint32 bufferAddr;
    uint32 bufferSize;
    DMABUFFER_DIRECTION_E dir;
} DMA_CAHCECONFIG_T;

#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E) 
typedef enum _DMA_SECURITY_MODE
{
    DMA_SECURITY  = 0,
    DMA_UNSECURITY,    
} DMA_SECURITYMODE_E;
#endif

//dma channel config struct
typedef struct _DMA_ChnCfg_Struct
{
    // normal config param, these param maybe used in most scence
    DMA_CHN_TYPE_E          type;              /* hardware or software         */
    DMA_CHN_PRIORITY_E      priority;          /* dma channel priority         */
    DMA_CHN_WORKMODE_E      workmode;          /* normal/linklist/soflist      */
    DMA_DATAWIDTH_E         src_datawidth;     /* source data width            */
    DMA_DATAWIDTH_E         dest_datawidth;    /* dest data width              */
    uint32                  src_address;       /* src start address            */
    uint32                  dest_address;      /* des start address            */
    DMA_CHN_REQMODE_E       requestmode;       /* request mode                 */

    uint16                  frag_length;       /* block length                 */
    uint16                  block_length;      /* block length                 */
    uint32                  total_length;      /* total length                 */
    int32                   src_step;          /* SrcElementPost-modification  */
    int32                   dest_step;         /* DestElementPost-modification */
	int32                   src_fragstep ;     /* SrcFragment-modification     */
    int32                   dest_fragstep;    /* DestFragmentPost-modification*/    
    DMA_BLOCKMODE_E         src_blockmode;     /* INCR4,8m16                   */
    DMA_BLOCKMODE_E         dest_blockmode;    /* INCR4,8m16                   */
    int32                   src_blockstep ;    /* SrcBlockPost-modification    */
    int32                   dest_blockstep;    /* SrcBlockPost-modification    */
    BOOLEAN                 is_srcwrapen;      /* src wrap enable              */
    BOOLEAN                 is_destwrapen;     /* dst wrap enable              */
    uint32                  wrapstart_addr;    /* wrap start addr              */
    uint32                  wrapend_addr;      /* wrap end addr                */
    BOOLEAN                 is_ll_end;         /* linklist end flag            */
    uint32                  ll_next;           /* point to next link list node */
    DMA_ENDIANSWITCHMODE_E  endian_switchmode; /* endian switch mode           */
    BOOLEAN                 is_noautoclose;    /* no auto close                */
    DMA_ENDIANTYPE_E        endian_type;       /* endian selection             */
    //uint8                   uid;               /* dma channel uid  0-31        */
#if defined(CHIP_VER_UIX8910) || defined(CHIP_VER_UIS8910C) || defined(CHIP_VER_UIS8910A) || defined(CHIP_VER_UWS6121E) 
    DMA_SECURITYMODE_E      dma_securitymode;    /*dma security mode sel        */
#endif
} DMA_CHNCFG_T, *DMA_CHNCFG_PTR;

//DMA error type
typedef enum _DMA_ERR_TYPE_E
{
    DMA_ERR_TYPE_NONE = 0,
    DMA_ERR_TYPE_INVALID_CHNID,
    DMA_ERR_TYPE_INVALID_PARAM,
    DMA_ERR_TYPE_CHNTYPE_MISMATCHED,
    DMA_ERR_TYPE_NOT_AVAILABLE,
    DMA_ERR_TYPE_LIST_OVERRUN,
    DMA_ERR_TYPE_LIST_NODATA,
    DMA_ERR_TYPE_LIST_APPENDFAIL,
    DMA_ERR_TYPE_MAX
} DMA_ERR_TYPE_E;

typedef enum _DMA_CHN_REQ_LOGIC_ID_
{
	DMA_UART0_TX_CHN_REQ = 0x0,
	DMA_UART0_RX_CHN_REQ,
	DMA_UART1_TX_CHN_REQ,
	DMA_UART1_RX_CHN_REQ,
	DMA_UART2_TX_CHN_REQ,
	DMA_UART2_RX_CHN_REQ,
#if defined(PLATFORM_UWS6121E)
	DMA_ZSPUART_TX_CHN_REQ,
	DMA_ZSPUART_RX_CHN_REQ,
#endif
	DMA_SPI0_TX_CHN_REQ,
	DMA_SPI0_RX_CHN_REQ,
	DMA_SPI1_TX_CHN_REQ,
	DMA_SPI1_RX_CHN_REQ,
	DMA_IIS0_TX_CHN_REQ,
	DMA_IIS0_RX_CHN_REQ,
	DMA_IIS1_TX_CHN_REQ,
	DMA_IIS1_RX_CHN_REQ,
	DMA_SIM0_TX_CHN_REQ,
	DMA_SIM0_RX_CHN_REQ,
	DMA_SIM1_TX_CHN_REQ,
	DMA_SIM1_RX_CHN_REQ,
	DMA_VBDA0_CHN_REQ,
	DMA_VBDA1_CHN_REQ,
	DMA_VBAD0_CHN_REQ,
	DMA_VBAD1_CHN_REQ,
	DMA_USBD0_CHN_REQ,
	DMA_USBD1_CHN_REQ,
	DMA_USBD2_CHN_REQ,
	DMA_USBD3_CHN_REQ,
	DMA_USBD4_CHN_REQ,
	DMA_USBD5_CHN_REQ,
	DMA_USBD6_CHN_REQ,
	DMA_USBD7_CHN_REQ,
	DMA_USBD8_CHN_REQ,
	DMA_USBD9_CHN_REQ,
	DMA_USBD10_CHN_REQ,
	DMA_USBD11_CHN_REQ,
	DMA_USBD12_CHN_REQ,
	DMA_USBD13_CHN_REQ,
	DMA_EPT_DIN_CHN_REQ,
	DMA_EPT_DOUT_CNH_REQ,
	DMA_FMARK_CHN_REQ,
	DMA_NFCRX_CHN_REQ,
	DMA_NFCTX_CHN_REQ,
	DMA_ROTATION_CHN_REQ,
	DMA_GEA_CHN_REQ,
	DMA_MIDI0_CHN_REQ,
	DMA_MIDI1_CHN_REQ,
	DMA_SDIO_WR_CHN_REQ,
	DMA_SDIO_RD_CHN_REQ,
	DMA_CHN_REQ_MAX
}DMA_CHN_REQ_LOGIC_ID_E;

typedef struct _DMA_CHN_REQ_INFO
{
	DMA_CHN_REQ_LOGIC_ID_E logic_id;
	uint32 				   req_id;
}DMA_CHN_REQ_INFO_T;

/**----------------------------------------------------------------------------*
**                         Public Function Prototype                          **
**----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    dma driver init
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_Init (void);


/*****************************************************************************/
//  Description:    alloc soft dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           if success return channel id, otherwise return DMA_CHANNELID_INVALID
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_E type);

/*****************************************************************************/
//  Description:    release channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_FreeChn(uint32 channel);

/*****************************************************************************/
//  Description:    set dma pause
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           s_pause:SCI_TRUE,pause dma; SCI_FALSE,resume dma
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_SetPause (BOOLEAN is_pause);

/*****************************************************************************/
//  Description:    alloc soft dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           if success return channel id, otherwise return DMA_CHANNELID_INVALID
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_AllocSoftChn (void);

/*****************************************************************************/
//  Description:    release channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_FreeSoftChn (uint32 channel);

/*****************************************************************************/
//  Description:    configure a dma channel
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:channel to be configured
//                  chncfgstruct:channel configure struct
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnConfig (uint32 channel, DMA_CHNCFG_PTR chnparam);

/*****************************************************************************/
//  Description:    set dma priority
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//                  priority:channel priority,0-3
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_SetChnPriority (uint32 channel,DMA_CHN_PRIORITY_E priority);

/*****************************************************************************/
//  Description:    dma request enable/disable
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel :  channel No
//                  is_enable: enable/disable
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnEnable (uint32 channel, BOOLEAN is_enable);

typedef void (*DMA_Enable_CacheCallback) (void);

PUBLIC void DMA_HAL_ChnEnable_Ex (uint32 channel, BOOLEAN is_enable,
                                  DMA_CAHCECONFIG_T cfgArray[], uint32 cfgCnt);

/*****************************************************************************/
//  Description:    get dma channel transdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingTransDone (uint32 channel);

/*****************************************************************************/
//  Description:    get dma channel blockdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingBlockDone (uint32 channel);

/*****************************************************************************/
//  Description:    get dma channel listdone interrupt raw status
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//  Return value:   return SCI_TRUE if the channel transfer finished,
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_PollingListDone (uint32 channel);

/*****************************************************************************/
//  Description:    configure dma channel interrupt
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel number,0-31
//                  inttype:interrupt type,including all transfer done,burst done and link done.
//                  is_enable:SCI_TRUE,enable the interrupt, SCI_FALSE,disable the interrupt
//                  callback:interrupt service,if is_enable=SCI_FALSE, it is unuseful.
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC void DMA_HAL_ChnIntConfig (uint32 channel, uint32 inttype, DMA_HAL_ISR_HANDLER callback);

//*****************************************************************************/
//  Description:    return std chn block transmited length
//  Note:           channel: chn index
/*****************************************************************************/
PUBLIC uint32 DMA_HAL_StdChn_Transfered_Data (uint32 channel);

/*****************************************************************************/
//  Description:    init a link transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           channel:dma channel id
//  Return value:   if success return link list handle, otherwise return  DMA_LINKLIST_HANDLE_INVALID
/*****************************************************************************/
PUBLIC DMA_LINKLIST_HANDLE DMA_HAL_InitLinkList (uint32 channel);

/*****************************************************************************/
//  Description:    append a link element
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           linklisthandle:link list handle
//                  linklistelement:element to append
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC DMA_ERR_TYPE_E DMA_HAL_AppendInitLinkList (DMA_LINKLIST_HANDLE linklisthandle,
        DMA_CHNCFG_PTR param);

/*****************************************************************************/
//  Description:    submit a link list and start transfer
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           linklisthandle: link list handle
//                  is_int_used:polling or interrupt,if SCI_TRUE, intterrupt used
//                  callback: if interrupt used,callback need
//  Return Value:   if success return DMA_ERR_TYPE_NONE
/*****************************************************************************/
PUBLIC DMA_ERR_TYPE_E DMA_HAL_SubmitInitLinkList (DMA_LINKLIST_HANDLE linklisthandle);

/*****************************************************************************/
//  Description:    free link list
//  Global resource dependence:
//  Author:         Guofu.Huang
//  Note:           linklisthandle:link list to release
/*****************************************************************************/
PUBLIC void DMA_HAL_FreeLinkList (DMA_LINKLIST_HANDLE linklisthandle);

/*****************************************************************************/
// Description :    This function is used to enable or disable dma pause function.
// Global resource dependence : s_dma_pause_disable_cnt
// Author :
// Note : is_enable :   TURE: enable dma pause function; FALSE: disable dma pause function
/*****************************************************************************/
PUBLIC void DMA_HAL_EnableDMAPause (BOOLEAN is_enable);

/*****************************************************************************/
// Description :    This function is used to get the function status of dma pause
// Global resource dependence : s_dma_pause_disable_cnt
// Author :
// Note : TURE: dma pause function is enabled; FALSE: dma pause function is disabled
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_IsEnableDMAPause (void);

/*****************************************************************************/
// Description :    This function is used to get the dma pause status
// Global resource dependence : s_is_dma_pause
// Author :
// Note : TURE: dma pause function is enabled; FALSE: dma pause function is disabled
/*****************************************************************************/
PUBLIC BOOLEAN DMA_HAL_IsDMAPause (void);

//*****************************************************************************/
//  Description:    setting dma channnel user id
//  Global resource dependence:
//  Author:         
//  Note:           channel: chn index
//                  uid:channel request logic id
/*****************************************************************************/
PUBLIC void DMA_HAL_SetChnUID (uint32 channel, DMA_CHN_REQ_LOGIC_ID_E chn_req_id);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif //_DMA_DRV_INTERNAL_H_
// End of File


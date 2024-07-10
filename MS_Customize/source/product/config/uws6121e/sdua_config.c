/******************************************************************************
 ** File Name:      sdua_config.c                                             *
 ** Author:         Zhang Hanbing                                             *
 ** DATE:           25/11/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    used for mbbms ca sd card rw function set                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 25/11/2009     Zhang Hanbing        Create                                *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 
#include "os_api.h"
#include "ref_outport.h"
#include "bsd.h"
#include "sdua_config.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                  *
 **---------------------------------------------------------------------------*/
#define SD_CA_PNM_NUM 5
 
/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
//extern  BOOLEAN slot0_Read(uint32 startBlock,uint32 num,uint8* buf);
//extern  BOOLEAN slot0_Write(uint32 startBlock,uint32 num,uint8* buf);
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN s_is_sd_init = TRUE;
//LOCAL uint8 sd_ca_pnm[SD_CA_PNM_NUM] = {0x4D,0x42,0x42,0x4D,0x53,0x20};
LOCAL uint8 sd_ca_pnm[SD_CA_PNM_NUM] = {0x4D,0x42,0x42,0x4D,0x53}; //MBBMS
LOCAL uint8 sd_pnm[SD_CA_PNM_NUM] = {0};
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Local Function Definitions                        *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN SDUACFG_Is_Init(void)
//! \param void
//! \return TRUE init 
//! \brief  Description: This Function is used to get ca sd power init status
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC BOOLEAN SDUACFG_Is_Init(void)
{
    return s_is_sd_init;
}

/*****************************************************************************/
//! \fn PUBLIC void SDUACFG_Set_Init_Status(BOOLEAN is_init )
//! \param is_init
//! \return void
//! \brief  Description: This Function is used to set ca sd power init status
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC void SDUACFG_Set_Init_Status(BOOLEAN is_init )
{
    s_is_sd_init = is_init;
}

/*****************************************************************************/
//! \fn BOOLEAN SDUACFG_IsMbbmsca(void)
//! \param void
//! \return TRUE is ca card
//! \brief  Description: This Function is used to judeg whether is mbbms ca card
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC BOOLEAN SDUACFG_IsMbbmsca(void)
{
    if ( 0 == memcmp(sd_pnm,sd_ca_pnm,SD_CA_PNM_NUM))
    {
        SCI_TRACE_LOW("sd is mbbms ca card");
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//! \fn PUBLIC void SDUACFG_GetPnm(uint8* pnm)
//! \param pnm
//! \return void
//! \brief  Description: This Function is used to get sd pnm
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC void SDUACFG_GetPnm(uint8* pnm)
{
    if(pnm)
    {
        SCI_MEMCPY(sd_pnm,pnm,SD_CA_PNM_NUM);
    }
}

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN SDUACFG_IOCTL(SCM_SLOT_NAME_E slot_name,SCM_FUN_E cmd,SCM_PARAM_T* param)
//! \param is_init
//! \return void
//! \brief  Description: This Function is used to sd IO before init
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC BOOLEAN SDUACFG_IOCTL(SCM_SLOT_NAME_E slot_name,SCM_FUN_E cmd,SCM_PARAM_T* param)
{
    //SCI_ASSERT(0);
    SCI_TRACE_LOW("[DRV_SDUA]<ERROR> SDUACFG_IOCTL is wrong\r\n" );
#if 0    
    if (SCM_FUNC_READ==cmd)
    {
        if (slot0_Read( param->readParam.startBlock,param->readParam.num,
                        param->readParam.buf) )
        {
            return TRUE;
        }
    }
    else if (SCM_FUNC_WRITE == cmd)
    {
        if (slot0_Write( param->readParam.startBlock, param->readParam.num,
                         param->readParam.buf) )
        {
            return TRUE;
        }
    }
    else
    {
    }
#endif    
    return FALSE;    
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


/****************************************************************************
** File Name:      mmiad_internal.h                                        *
** Author:                                                                 *
** Date:           03/19/2012                                             *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the mmi ad internal api   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 19/03/2012       chunjie.liu         Create
****************************************************************************/
#ifndef  _MMIAUTODEMO_INTERNAL_H_
#define  _MMIAUTODEMO_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmicc_export.h"
#include "mmicc_position.h"
#include "mmi_custom_define.h"
#include "cafapplet.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C" {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define  MMI_AUTO_DEMO_LIST_NUM         100
#define  MMI_AUTO_DEMO_CMD_TOREAD_SIZE  256
#define  MMI_AUTO_DEMO_SUFFIX_LEN       10


//#define CC_INVALID_ID						255		//·Ç·¨ID


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef   struct   FileList 
    { 
        char   filename[64]; 
        struct   FileList   *next; 
    }FILENODE;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : load cmd
//  Global resource dependence : 
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/     
 PUBLIC BOOLEAN MMIAPIAUTODEMO_LoadCmd(
                                    uint8* cmd_buf,
									uint32 *cmd_buf_len_ptr
									//BOOLEAN* is_readfile_over_ptr									
                                 );

/*****************************************************************************/
//  Description : is read  the file over
//  Global resource dependence : 
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/     
PUBLIC BOOLEAN MMIAPIAUTODEMO_IsFileOver(void);

/*****************************************************************************/
//  Description : get file handle
//  Global resource dependence : s_file_handle
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/  
PUBLIC MMIFILE_HANDLE MMIAPIAUTODEMO_GetFileHandle(void);

/*****************************************************************************/
//  Description : set file handle
//  Global resource dependence : s_file_handle
//  Author:chunjie.liu
//  Note: 
/*****************************************************************************/  
PUBLIC void MMIAPIAUTODEMO_SetFileHandle(MMIFILE_HANDLE file_handle);

/*****************************************************************************/
//  Description : free cmd buf
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIAD_FreeCmdBuf(void);

/*****************************************************************************/
//  Description : create autodemo task
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAD_CreateADTask(void);
/*****************************************************************************/
//  Description : delete Autodemo task
//  Global resource dependence : 
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAD_DeleteADTask(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif//_MMIAUTODEMO_INTERNAL_H_

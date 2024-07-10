/****************************************************************************
** File Name:      mmiphs_app.h                                                *
** Author:                                                                 *
** Date:           03/22/2006                                              *
** Copyright:      2006 TLT, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshengqi         Create
** 
****************************************************************************/
#ifndef  _MMI_ZDTHTTP_APP_H_    
#define  _MMI_ZDTHTTP_APP_H_   

#ifdef __cplusplus
    extern "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "zdthttp_common.h"

#include "zdthttp_id.h"
#include "zdthttp_menutable.h"
#include "zdthttp_nv.h"
#include "zdthttp_text.h"
#include "zdthttp_image.h"
#include "zdthttp_anim.h"
#include "zdthttp_net.h"
#include "zdthttp_api.h"

#define ZDTHTTP_MAX_URL_LEN 1024

typedef struct
{
       uint8 *        str;
	uint32         len; 
} MMI_ZDTHTTP_DATA_T;

typedef struct
{
	SIGNAL_VARS
       MMI_ZDTHTTP_DATA_T data;
} MMI_ZDTHTTP_SIG_T ;

typedef struct
{
    uint32                  refresh_time;           //refresh time: seconds
    uint8  url_arr[ZDTHTTP_MAX_URL_LEN+1];
}ZDTHTTP_REFRESH_IND_T;

typedef struct
{
    uint8  * url_arr;
    uint32   refresh_time;           //refresh time: seconds
}ZDTHTTP_REFRESH_DATA_T;

typedef struct
{
    SIGNAL_VARS
    ZDTHTTP_REFRESH_DATA_T * p_http_data;
} ZDTHTTP_REFRESH_SIG_T;


extern void MMIZDTHTTP_AppInit(void);
extern void MMIZDTHTTP_RegWinIdNameArr(void);
extern void MMIZDTHTTP_InitModule(void);
extern void MMIZDTHTTP_RegMenuGroup(void);
extern void MMIZDTHTTP_RegNv(void);
extern BOOLEAN  MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len);

#ifdef __cplusplus
    }
#endif

#endif

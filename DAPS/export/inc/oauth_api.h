/*****************************************************************************
** File Name:       oauth_api.h                                              *
** Author:          Jiaoyou.wu                                               *
** Date:            03/26/2015                                               *
** Copyright:       2007 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:     This file is used to describe the interface              *
*****************************************************************************/

#ifndef _oauth_api_H_
#define _oauth_api_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "os_api.h"
#include "sci_types.h"

 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define OAUTH_MAX_TOKEN_LEN                      1024

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    OAUTH_SUCCESS = 0,                  
    OAUTH_ERROR_FAILED,                  
    OAUTH_ERROR_INVALID_INPUT,           
    OAUTH_ERROR_NO_MEMORY,               
    OAUTH_ERROR_UNKNOWN,                

    OAUTH_ERROR_MAX
} OAUTH_ERROR_E;

typedef enum
{
    OAUTH_REPONSE_TYPE_CODE = 0,                  

    OAUTH_REPONSE_TYPE_MAX
} OAUTH_REPONSE_TYPE_E;

typedef enum
{
    OAUTH_DISPLAY_TOUCH = 0,                  

    OAUTH_DISPLAY_MAX
} OAUTH_DISPLAY_E;

typedef struct _OAUTH_TOKEN_IND_SIG
{
    _SIGNAL_VARS
    int32 module_id;
    int32 result;
    char *access_token_data;
    int32 access_token_data_len;
    char *refresh_token_data;
    int32 refresh_token_data_len;	
}OAUTH_TOKEN_IND_SIG_T;

typedef struct _OAUTH_REFRESH_TOKEN_IND_SIG
{
    _SIGNAL_VARS
    int32 module_id;
    int32 result;
    char *access_token_data;
    int32 access_token_data_len;
    char *refresh_token_data;
    int32 refresh_token_data_len;	
}OAUTH_REFRESH_TOKEN_IND_SIG_T;


/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void OAUTH_CheckAccessCodeInd(const char *url, uint32 len);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void OAUTH_CheckAccessCode(const char *url);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void OAUTH_SignOffByAPP(int32 context);
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_CreateContext(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/

PUBLIC int32 OAUTH_CreateContextByURL(const char *url);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void OAUTH_DestoryContext(int32 context);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetModuleID(int32 context, uint32 module_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetNetID(int32 context, uint32 net_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetClientID(int32 context, const char *client_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetRequestURL(int32 context, const char *request_url);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetTokenURL(int32 context, const char *token_url);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetRefreshTokenURL(int32 context, const char *refresh_token_url);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetRefreshRedirectURL(int32 context, const char *redirect_url);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetScope(int32 context, const char *scope);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetResponseType(int32 context, OAUTH_REPONSE_TYPE_E reponse_type);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetDisplay(int32 context, OAUTH_DISPLAY_E display);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_SetTaskID(int32 context, BLOCK_ID task_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_GetAccessToken(int32 context);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC int32 OAUTH_RefreshToken(int32 context);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

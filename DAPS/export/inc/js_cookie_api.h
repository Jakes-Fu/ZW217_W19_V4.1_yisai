#ifndef _COOKIE_API_H
#define _COOKIE_API_H

#include "os_api.h"
#include "sci_types.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif
/*****************************************************************************/
//  Description : get http cookie for js use
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
 PUBLIC char* HttpGetCookieByJS(char* url_ptr, uint32 context_id);

/*****************************************************************************/
//  Description : set http cookie by JS
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  HttpSetCookeByJS(char* value_ptr, char* url_ptr, uint32 context_id);

/*****************************************************************************/
//  Description : tell JS if the cookie is enable
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  HttpIsCookieEnableQeuryByJS(uint32 context_id);

/*****************************************************************************/
//  Description : return the cookie list of the indicated url for js use
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
//PUBLIC HTTP_COOKIE_LIST_T*  HttpGetCookieListByJS(char* url_ptr, uint32 context_id);
/*****************************************************************************/
//  Description : delete the cookie of indecated url by JS
//  Global resource dependence : 
//  Author: zhaodi.chen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpDeleteCookieByJS(char* name_ptr, char* url_ptr,  uint32 context_id);

#ifdef   __cplusplus
    }
#endif

#endif //_COOKIE_API_H

/****************************************************************************
** File Name:      ssl_test.h                                              *                                           *
** Date:           2020/09/03                                              *
** Copyright:      2020 uinsoc, Incorporated. All Rights Reserved.         *
** Description:                                                            *
****************************************************************************/

#ifndef _HTTP_TEST_H_
#define _HTTP_TEST_H_

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#define MAX_RESULT_BUFFER_LEN       (100)
typedef struct _HTTPTESTPARAM_T
{
    uint8 method;
    uint32 break_start;
    uint32 break_end;
}HTTPTESTPARAM_T;
typedef enum
{
    HTTPTEST_METHOD_GET = 0,
    HTTPTEST_METHOD_DELETE,
    HTTPTEST_METHOD_HEAD,
    HTTPTEST_METHOD_MAX
}HTTPTEST_METHOD_E;
 /*****************************************************************************/
//  Description :  set atcmd link id
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void http_test_setLinkId(uint8 link_id);

/*****************************************************************************/
//  Description :  set net id
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC void http_test_setnetid(uint32 netid);
/*****************************************************************************/

/*****************************************************************************/
//  Description :  exit function
//  This API must called in MMI thread.(May be User press some key)
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC int  http_test_exit(void);

/*****************************************************************************/
//  Description :  get at test result function
//  Note: 
/*****************************************************************************/
PUBLIC int  http_test_get_result(char *at_rsp);

#ifdef   __cplusplus
    }
#endif

#endif 

/****************************************************************************
** File Name:      query_traffic.h                                                         
** Date:           2022/10/19                                              
** Copyright:      2020 uinsoc, Incorporated. All Rights Reserved.         
** Description:                                                            
****************************************************************************/

#ifndef _QUERY_TRAFFIC_H_
#define _QUERY_TRAFFIC_H_

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
PUBLIC int query_get_traffic(void);
PUBLIC int query_clear_traffic(void);
PUBLIC int query_traffic_get_result(char *at_rsp);

#ifdef   __cplusplus
    }
#endif

#endif 

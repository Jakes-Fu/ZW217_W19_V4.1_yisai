/*******************************************************************************
 ** File Name:      atc_mux.h                                                      *
 ** Author:         wanghui                                                   *
 ** Date:           09/10/2008                                                 *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.          *
 ** Description:    This file contains types and MICROS that are of use to all *
 **                 other ATC modules.                                         *
 *******************************************************************************
                                                                               
 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                *
 ** 09/10/2008     wanghui           Created.                                   *
 ******************************************************************************/
#ifndef _ATC_MUX_H_
#define _ATC_MUX_H_
/*******************************************************************************
 **                        Common Header Files                                 *
 ******************************************************************************/

typedef struct
{
    SIGNAL_VARS
    uint8           link_id;
    uint8           align[3];
}ATC_MUX_LINK_SETUP_REQ_T ;

typedef struct
{
    SIGNAL_VARS
    uint8           link_id;
    uint8           align[3];
}ATC_MUX_LINK_RELEASE_REQ_T ;

typedef struct
{
    SIGNAL_VARS
}ATC_MUX_CLOSE_REQ_T ;


#endif /*_ATC_MUX_H_*/
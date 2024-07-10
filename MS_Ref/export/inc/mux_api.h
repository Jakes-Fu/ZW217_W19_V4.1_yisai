/******************************************************************************
 ** File Name:      mux_api.h                                      *
 ** Author:         yayan.xu                                              *
 ** DATE:           07/10/2008                                              *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    All header info for external interface*
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                       NAME             DESCRIPTION                               *
 ** 07/10/2008     yayan.xu                 Create.                                   *
 ******************************************************************************/
#ifndef _MUX_API_H
#define _MUX_API_H

#include "sci_types.h"

#ifndef _WINDOWS
#include "os_api.h"
#endif

#include "mux_all.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

    /*------------------------------------------------------------------------------
    Defination struct: External SIGNAL interface
    ------------------------------------------------------------------------------*/
    /*MUX notify External upper layer modules recieve data.*/
    typedef struct
    {
        SIGNAL_VARS      uint8  link_id;
        uint8                   reserved[3];
        uint32                  data_length;
        uint8                  *data_ptr;
    }MUX_ATC_RECV_DATA_SIG_T;

    /*------------------------------------------------------------------------------
    Defination :External API function for data transfer
    ------------------------------------------------------------------------------*/
    PUBLIC MUX_RETURN_E     MUX_Write(uint8 *data_ptr, uint32 data_length, uint8 link_id);
    /*------------------------------------------------------------------------------
    Defination :External API function for MUX control  and DLC control
    ------------------------------------------------------------------------------*/
    PUBLIC MUX_RETURN_E     MUX_Startup(MUX_SIDE_E side, MUX_SYS_PARA_T para);
    PUBLIC MUX_RETURN_E     MUX_Exit(void);

    PUBLIC MUX_RETURN_E     MUX_Close_Link(uint8 link_id);
    PUBLIC MUX_RETURN_E     MUX_Open_Link(MUX_PN_DATA_T *sys_param, uint8 *link_id, MUX_CALLBACK recv_func);


    PUBLIC MUX_RETURN_E     MUX_ReadSysPara(MUX_SYS_PARA_T *param);
    /*------------------------------------------------------------------------------
    Defination :External API function for DTI
    ------------------------------------------------------------------------------*/
    PUBLIC MUX_RETURN_E     MUX_DTI_Create(uint8 link_id, MUX_ENTITY_T entity_info);
    PUBLIC MUX_RETURN_E     MUX_DTI_Destroy(uint8 link_id);
    PUBLIC MUX_RETURN_E     MUX_DTI_Query_Entity(uint8 link_id, MUX_ENTITY_T *entity);
    PUBLIC MUX_RETURN_E     MUX_Set_Atc_CallbackFunc(uint8 link_id, MUX_CALLBACK at_recv_func);
    /*------------------------------------------------------------------------------
    Defination :Special interface PPP.
    ------------------------------------------------------------------------------*/
    MUX_DLC_T              *MUX_GetPPPPort(uint8 link_id);
    long                    MUX_Getbaud_rate(void);

    typedef enum
    {
        MUX_CTRL                            = 0,
        /*MUX control channel*/
        MUX_ATC,
        MUX_VT = 2,
        MUX_PPP1,
        MUX_PPP2,
        MUX_PPP3,
        MUX_PPP4,/*In  8800S, max num of Pdp contentis 4*/
        MUX_SERVICE_NUM_MAX,
        MUX_INVALID_SERVICE = 0xff,
    }       MUX_SERVICE_E;

    uint8   MUX_GetLinkid(MUX_SERVICE_E type);


typedef struct _MUX_ATCIPSEND_T_
{
    uint32      atc_cmdline_maxlen;
    uint32      cur_atc_frame_size; 
    BOOLEAN     atc_is_max_mode;
    uint8       reserved[3];
}MUX_ATCIPSEND_T;
PUBLIC MUX_ATCIPSEND_T * MUX_ATC_RtnGblCtl(void);
PUBLIC void MUX_ATC_SetCmdLineMaxLen(BOOLEAN is_max, uint32 max_len);

#ifdef   __cplusplus
}
#endif
#endif/*_MUX_API_H*/




/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/

#ifndef _DM_ENDPOINT_H_
#define _DM_ENDPOINT_H_

typedef struct _Options
{
    char szCMEI_IMEI[64];  //CMEI/IMEI
    char szCMEI_IMEI2[64]; //CMEI/IMEI
    char szIMSI[64];       //IMSI
    char szDMv[16];        //DM
    char szAppKey[64];     //appkey
    char szPwd[64];        //pwd
    int nAddressFamily;    //ipv4,ipv6
} Options;

int prv_getDmUpdateQueryLength(void *contextP, void *server);
int prv_getDmUpdateQuery(void *contextP, void *server, uint8_t *buffer, size_t length);
int genDmUpdateEndpointName(char *type, char **data, void *dmconfig);
int genDmRegEndpointName(char *type, char **data, void *dmconfig);
int dm_read_info_encode(char *szin, char **szout, char *pwd);

#endif

/*************************************************************************
** File Name:      mmidm_pl_comm.c                                      *
** Author:         jian.ma                                              *
** Date:           2009/07/03                                           *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file defines the function about dm function     *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 2009/07/03     jian.ma          Create.                              *
*************************************************************************/
#define _MMIDM_PL_COMM_C_
//#include "std_header.h"
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmi_app_dm_trc.h"
#ifdef DM_SUPPORT
#include "vdm_pl_types.h"
#include "app_tcp_if.h"
#include "vdm_error.h"
#include "vdm_comm_pl_types.h"
#ifdef DM_SUPPORT_REDBENCH
#include "vdm_comm_pl_conn.h"
#include "vdm_comm_pl_http.h"
#endif
#include "vdm_utl_utf8.h"
#include "os_api.h"
#include "vdm_pl_stdlib.h"
#include "vdm_types.h"
#include "mmidm_app.h"
#include "mmidm_config.h"
#ifdef DM_SUPPORT_DFRESH
#include "mmidm_parsexml.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define RDM_DM_USERNAME_MAX_LEN             MMIFMM_FILE_FILE_NAME_MAX_LEN
#define RDM_DM_MAC_MAX_LEN                  MMIFMM_FILE_FILE_NAME_MAX_LEN
#define RDM_DM_ALGORITHM_LEN                MMIFMM_FILE_FILE_NAME_MAX_LEN
#define RDM_DM_SERVER_URL_LEN               MMIFMM_FILE_FILE_NAME_MAX_LEN
#define RDM_MAX_SEND_PACKAGE                (1024*15)
#define RDM_SOCKET_DM_RECEV_BUFFER_LEN      (8*1024)

#define DM_CMWAP_PORT_INFO                  80     //define dm cmwap port info
#define INVALID_SOCKET                      (-1)


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef void (*RDM_GET_SOCKET_ID_cbFunc)(unsigned long socketID);
typedef struct
{
    unsigned long     v_ip_addr[RDM_DM_SERVER_URL_LEN];    /* ip address */  
    unsigned long     v_socket_id;
    unsigned short    v_port;
    RDM_GET_SOCKET_ID_cbFunc v_cbFunc;
}RDM_SOCKET_INFO_T;

typedef struct
{
    char username[RDM_DM_USERNAME_MAX_LEN];
    char mac[RDM_DM_MAC_MAX_LEN];
    char algorithm[RDM_DM_ALGORITHM_LEN];
    char server_url[RDM_DM_SERVER_URL_LEN];
}RDM_CONNECT_INFO_T;

/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern unsigned long inet_addr(const char * cp);


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
#ifdef DM_SUPPORT_REDBENCH
LOCAL VDM_CommObserver_t*       s_dm_observer = PNULL;
LOCAL VDM_CommObserver_t*       s_dl_observer = PNULL;
#endif
LOCAL int32                     s_dm_Socket_Id = 0;
LOCAL int32                     s_dl_Socket_Id = 0;
LOCAL unsigned char*            s_dm_OutPackage_ptr = PNULL;
#if !defined (DM_SUPPORT_DFRESH) && !defined (DM_SUPPORT_REDBENCH)
LOCAL int16                     s_dm_isWBXML = 1; 
#endif
LOCAL char *                    s_recvBuffer_ptr = PNULL;
LOCAL uint32                    s_recvActualLen = 0;
LOCAL uint32                    s_recvpos= 0;
#ifdef DM_SUPPORT_REDBENCH
LOCAL char *                    s_loc_algorithm = NULL;
LOCAL char *                    s_loc_username  = NULL;
LOCAL char *                    s_loc_mac = NULL;
#endif
LOCAL RDM_SOCKET_INFO_T         s_rdm_socket_info = {0};
LOCAL RDM_CONNECT_INFO_T *      s_connect_info_ptr = {0};
#ifdef DM_SUPPORT_REDBENCH
LOCAL VDM_Comm_HMAC_t*          s_com_hmac_ptr ={0};
#endif

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : error process function
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/

/*
********************************************************************************
//  Description : get socketID.
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
********************************************************************************
*/
LOCAL void RDM_COMMS_GetDMSocketID_cbFunc(int32 socketID);

/*********************************************************************************
//  Description : get socketID
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
*********************************************************************************/
LOCAL void RDM_COMMS_GetDLSocketID_cbFunc(int32 socketID);

/*
********************************************************************************
//  Description : construct http
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
********************************************************************************
*/

LOCAL int RDM_COMMS_Construct_http(unsigned char* destbuf, int maxlen, const unsigned char* body, int bodylen);

/*
********************************************************************************
//  Description : find substring
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
********************************************************************************
*/
LOCAL uint8* RDM_COMMS_findSubStringN( uint8* str,uint8* subStr,int N);

/*
********************************************************************************
//  Description : memory compare
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
********************************************************************************
*/
LOCAL BOOLEAN RDM_COMMS_memcmpN(uint8* str1, uint8* str2, int N);

/*
********************************************************************************
//  Description : find next char
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
********************************************************************************
*/
LOCAL uint8* RDM_COMMS_findNextCharN(uint8* str,char ch,int N);
/*
********************************************************************************
//  Description : parse syncml
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
********************************************************************************
*/
#ifdef DM_SUPPORT_REDBENCH
LOCAL RDM_Error RDM_parse_syncml(char* infoStr, char** username, char** mac, char** algorithm);
#endif
/*!
*******************************************************************************
* send data
*
* \param   socketId        socket id
* \param    buffer      the pointer of the buffer load the data want to be sent
* \param    write_len             the length of the data want to be sent
*
* \return         how many bytes have been sent in fact
*******************************************************************************/
LOCAL unsigned int RDM_Socket_Send (unsigned long socketId,const char * buffer, unsigned int write_len);


/*!
*******************************************************************************
* connect DM server
*
* \param   url            address of DM server
* \param           getSocketIDcbFunc      a callback function, pass the socket id to lib
*
* \return         a value
*******************************************************************************/
LOCAL IS32 RDM_Socket_open(const char * url, RDM_GET_SOCKET_ID_cbFunc getSocketIDcbFunc);


LOCAL int RDM_Socket_Connect_Server(const char * serverNameURL);

LOCAL int32 RDM_Socket_Create(int af, int type, int protocol);

LOCAL int RDM_Socket_Connect_by_ip(unsigned long socketId,const char* ip, unsigned int port, unsigned int timeout);

LOCAL int RDM_Socket_Connect_by_name(unsigned long socketId,const char* server_name, unsigned int port, unsigned int timeout);

LOCAL BOOLEAN RDM_UrlIsAnIpAddress(const char* url);

LOCAL int RDM_isWholeMsgArrived(void);

LOCAL void RDM_COMMS_Data_Arrive(uint32 socket_id);
/*********************************************************************************
//  Description : to lower
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
LOCAL char toLower(char c);
/*********************************************************************************
//  Description : find next lower char
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
LOCAL UTF8CStr FindNextLowerChar(UTF8CStr str, char ch);

/*********************************************************************************
//  Description : find sub string
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
LOCAL UTF8CStr FindSubString(UTF8CStr str, UTF8CStr subStr);

/*********************************************************************************
//  Description : get dec number
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
LOCAL IU32 GetDecNumber(UTF8CStr str);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*********************************************************************************
//  Description : get socketID
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
*********************************************************************************/
LOCAL void RDM_COMMS_GetDMSocketID_cbFunc(int32 socketID)
{
    //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_getDMSocketID_cbFunc] socketID = 0x%0x \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_241_112_2_18_2_14_8_438,(uint8*)"d",socketID);
    if (s_dm_Socket_Id!=0 && s_dm_Socket_Id != socketID)
    {
        sci_sock_socketclose(s_dm_Socket_Id);
    }
    s_dm_Socket_Id = socketID;
}

/*********************************************************************************
//  Description : get socketID
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
*********************************************************************************/
LOCAL void RDM_COMMS_GetDLSocketID_cbFunc(int32 socketID)
{
    //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_GetDLSocketID_cbFunc] socketID = 0x%x \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_256_112_2_18_2_14_8_439,(uint8*)"d",socketID);
    if (s_dl_Socket_Id!=0 && s_dl_Socket_Id != socketID)
    {
        sci_sock_socketclose(s_dl_Socket_Id);
    }
    s_dl_Socket_Id = socketID;
}

/*********************************************************************************
//  Description : construct http
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
*********************************************************************************/

LOCAL int RDM_COMMS_Construct_http(unsigned char* destbuf, int maxlen, const unsigned char* body, int bodylen)
{
    int ret = 0;
    int pos = 0;
    char serverName[RDM_DM_USERNAME_MAX_LEN] = {0};
    uint16 port = 80; 

    //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_Construct_http] bodylen = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_277_112_2_18_2_14_8_440,(uint8*)"d", bodylen);
    
    if (PNULL == destbuf)
    {
        return ret;
    }
    SCI_MEMSET(destbuf,0,maxlen);
    pos = sprintf((char*)destbuf, "POST %s HTTP/1.1\r\nCache-Control: private\r\nConnection: close\r\nUser-Agent: RedBend\r\nAccept: application/vnd.syncml+xml, application/vnd.syncml+wbxml, */*\r\nAccept-Language: en\r\nAccept-Charset: utf-8\r\n", s_connect_info_ptr->server_url);
    ret = sscanf((const char*)(s_connect_info_ptr->server_url +7), "%[^:/]:%hu", serverName, (uint16*)&port);
    if(ret < 1)
    {
        //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_Construct_http] failed to extract host.\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_288_112_2_18_2_14_8_441,(uint8*)"");
        pos += sprintf((char*)(destbuf+pos), "Host: %s\r\n", s_connect_info_ptr->server_url);
    }
    else
    {
        pos += sprintf((char*)(destbuf+pos), "Host: %s:%d\r\n", serverName, port);
    }
    //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_Construct_http] serverName = %s, port = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_295_112_2_18_2_14_8_442,(uint8*)"sd",serverName,port);
#ifdef DM_SUPPORT_REDBENCH  //根据库文件类型
    pos += sprintf((char*)(destbuf+pos), "Content-Type: %s\r\n", MMIDM_Config_GetEncodeWBXMLMsg()? DM_MIME_TYPE_WXML : DM_MIME_TYPE_XML);
#elif DM_SUPPORT_DFRESH  //DFRESH 只支持XML
    pos += sprintf((char*)(destbuf+pos), "Content-Type: %s\r\n", DM_MIME_TYPE_XML);
#else
    pos += sprintf((char*)(destbuf+pos), "Content-Type: %s\r\n", s_dm_isWBXML? DM_MIME_TYPE_WXML : DM_MIME_TYPE_XML);
#endif
    
    if(strlen(s_connect_info_ptr->username) > 0 && strlen(s_connect_info_ptr->mac) >0)
    {
        pos += sprintf((char*)(destbuf+pos), "x-syncml-hmac: username=\"%s\", mac=%s", s_connect_info_ptr->username, s_connect_info_ptr->mac);
        if(strlen(s_connect_info_ptr->algorithm) > 0)
        {
            pos += sprintf((char*)(destbuf+pos), " algorithm=\"%s\"\r\n", s_connect_info_ptr->algorithm);
        }
        else
        {
            pos += sprintf((char*)(destbuf+pos), "\r\n"); 
        }
    }  
    
    pos += sprintf((char*)(destbuf+pos), "Content-Length: %d\r\n\r\n", bodylen);
    
    if( (pos+bodylen) >= maxlen )
    {
        //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_Construct_http] len=%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_315_112_2_18_2_14_8_443,(uint8*)"d",(pos+bodylen));
        //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_Construct_http]Message too long to be hold.maxlen=%d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_316_112_2_18_2_14_8_444,(uint8*)"d",maxlen);
        return  -1;
    }
    
    //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_Construct_http] head lenght =%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_320_112_2_18_2_14_8_445,(uint8*)"d", pos);
    //SCI_TRACE_LOW("MMIDM==> [RDM_COMMS_Construct_http] strlen(header)=%d\n", strlen(destbuf));
    SCI_MEMCPY(destbuf+pos, body, bodylen);
    pos += bodylen;
    
    destbuf[pos]=0;
    //SCI_TRACE_LOW("MMIDM==> [RDM_COMMS_Construct_http] destbuf =%s\n", destbuf);
    //SCI_TRACE_LOW:"MMIDM==> [RDM_COMMS_Construct_http] message constructed total len = %d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_327_112_2_18_2_14_8_446,(uint8*)"d", pos);
    return pos;

}

/*********************************************************************************
//  Description : find substring
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
*********************************************************************************/
LOCAL uint8* RDM_COMMS_findSubStringN( uint8* str,uint8* subStr,int N)
{
    
    BOOLEAN is_same = FALSE;
    uint8* curr_ptr = PNULL; 
    uint8* next_ptr = PNULL;
    uint8* tmp_ptr = PNULL;
    const char fc = subStr[0] ;
    uint32 len = strlen((char*)subStr);
    uint8* strN = str+N-4;
    //SCI_TRACE_LOW:"MMIDM==> [findSubStringN] was called len= 0x%x,N= 0x%x,strN=0x%x\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_347_112_2_18_2_14_8_447,(uint8*)"ddd",len,N,strN);
    
    
    for (curr_ptr=str;curr_ptr<strN;curr_ptr++)
    {
        next_ptr= RDM_COMMS_findNextCharN(curr_ptr,fc,N-(uint32)curr_ptr-4+(int)str);/*lint !e737 */
        
        if (PNULL==next_ptr)
        {
            return NULL;
        }
        // print out the following 4 char
        tmp_ptr= next_ptr;
        
        is_same = RDM_COMMS_memcmpN(next_ptr,subStr,len);
        
        if (!is_same)
        {
            //SCI_TRACE_LOW:"MMIDM==> [findSubStringN]found the whole string,next =0x%x\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_365_112_2_18_2_14_8_448,(uint8*)"d",next_ptr);
            return next_ptr;
        }
        else
        { 
            curr_ptr=next_ptr;
        }
    }
    return NULL;
}

/*********************************************************************************
//  Description : memory compare
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
*********************************************************************************/
LOCAL BOOLEAN RDM_COMMS_memcmpN(uint8* str1, uint8* str2, int N)
{
    uint16 i = 0;
    
    if (PNULL == str1 || PNULL == str2)
    {
        return FALSE;
    }
    for (i=0;i<N;i++)
    {
        if(str1[i]!=str2[i]) 
        {
            return TRUE;   
        }
    }
    return FALSE;
}

/*********************************************************************************
//  Description : find next char
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
*********************************************************************************/
LOCAL uint8* RDM_COMMS_findNextCharN(uint8* str,char ch,int N)
{
    
    uint8* strN = str+N;
    uint8* strtmp = PNULL;
    
    for (strtmp= str;strtmp<strN;strtmp++)
    {
        if (*strtmp == ch)
        {
            return strtmp ;
        }
    }
    return PNULL;
}

#ifdef DM_SUPPORT_REDBENCH
/*********************************************************************************
//  Description : parse syncml
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
*********************************************************************************/
LOCAL RDM_Error RDM_parse_syncml(char* infoStr, char** username, char** mac, char** algorithm)
{
    char* syncml_hdr_ptr = PNULL;
    char * first_occ_str = strstr((char*)infoStr, "x-syncml-hmac:");
    //SCI_TRACE_LOW:"MMIDM==> [RDM_parse_syncml]  begin\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_430_112_2_18_2_14_9_449,(uint8*)"");
    if(PNULL!=first_occ_str)
    {
        char* end_syncml_hdr_ptr = strstr((char*)first_occ_str, "\r\n");/* add for pclint e613*/
        char* username_occ_ptr = PNULL;
        char* username_end_ptr = PNULL;
        char* mac_occ_ptr = PNULL;
        char* mac_end_ptr = PNULL;
        char* algorithm_occ_ptr = PNULL;
        char* algorithm_end_ptr = PNULL;
        int32 username_len = 0;
        int32 mac_len = 0;
        int32 algorithm_len = 0;
	 int32 syncml_hdr_len = 0;
	if(PNULL!= end_syncml_hdr_ptr)
	{
	        syncml_hdr_len = end_syncml_hdr_ptr - first_occ_str;
	}
	else
	{
		syncml_hdr_len = 0;
	}
        syncml_hdr_ptr = SCI_ALLOCA((syncml_hdr_len + 1) * sizeof(char));/*lint !e737 */
        if(PNULL == syncml_hdr_ptr)
        {
            //SCI_TRACE_LOW:"MMIDM==> [RDM_parse_syncml]  syncml_hdr_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_455_112_2_18_2_14_9_450,(uint8*)"d",syncml_hdr_len);
            return RDM_ERR_MEMORY;
        }
        
        SCI_MEMCPY(syncml_hdr_ptr, first_occ_str, syncml_hdr_len * sizeof(char));/*lint !e737 */
        end_syncml_hdr_ptr = syncml_hdr_ptr + syncml_hdr_len;
        username_occ_ptr = strstr(syncml_hdr_ptr, (char*)"username=");
        if (PNULL !=username_occ_ptr)
        {
            username_end_ptr = strstr(username_occ_ptr, (char*)",");
        }
	else
	{
		//SCI_TRACE_LOW:"MMIDM==>RDM_parse_syncml  PNULL ==username_occ_ptr"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_468_112_2_18_2_14_9_451,(uint8*)"");
		SCI_FREE(syncml_hdr_ptr);
		syncml_hdr_ptr = PNULL;
		return RDM_ERR_MEMORY;
	}
        mac_occ_ptr = strstr(syncml_hdr_ptr, (char*)"mac=");
        if (PNULL!=mac_occ_ptr)
        {
            mac_end_ptr = strstr(mac_occ_ptr, (char*)",");
        }
	else
	{
		//SCI_TRACE_LOW:"MMIDM==>RDM_parse_syncml  PNULL ==mac_occ_ptr"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_480_112_2_18_2_14_9_452,(uint8*)"");
		SCI_FREE(syncml_hdr_ptr);
		syncml_hdr_ptr = PNULL;
		return RDM_ERR_MEMORY;
	}
        algorithm_occ_ptr = strstr(syncml_hdr_ptr, (char*)"algorithm=");/*lint !e64*/
        if (PNULL!=algorithm_occ_ptr)/*lint !e64*/
        {
            algorithm_end_ptr = strstr(algorithm_occ_ptr, (char*)",");
        }
	else
	{
		//SCI_TRACE_LOW:"MMIDM==>RDM_parse_syncml  PNULL ==algorithm_occ_ptr"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_492_112_2_18_2_14_9_453,(uint8*)"");
		SCI_FREE(syncml_hdr_ptr);
		syncml_hdr_ptr = PNULL;
		return RDM_ERR_MEMORY;
	}
//        if(!username_occ_ptr || !mac_occ_ptr || !algorithm_occ_ptr)/*lint !e774 */
//        {
//            SCI_TRACE_LOW("MMIDM==> [RDM_parse_syncml]2 RDM_ERR_COMMS_NON_FATAL");
//            return RDM_ERR_COMMS_NON_FATAL;
//         }
        
        username_occ_ptr+=9;
        mac_occ_ptr+=4;
	   	
        algorithm_occ_ptr+=10;
        if(PNULL != username_end_ptr)
        {
            username_len = username_end_ptr - username_occ_ptr;
        }
        else
        {
            username_len = end_syncml_hdr_ptr - username_occ_ptr;
        }   
        if(PNULL != algorithm_end_ptr)
        {
            algorithm_len = algorithm_end_ptr - algorithm_occ_ptr;
        }       
        else
        {
            algorithm_len = end_syncml_hdr_ptr - algorithm_occ_ptr;
        }       
        if(PNULL != mac_end_ptr)
        {
            mac_len = mac_end_ptr - mac_occ_ptr;
        }   
        else
        {
            mac_len = end_syncml_hdr_ptr  - mac_occ_ptr;
        }           
        s_loc_username = SCI_ALLOC_APP((username_len + 1) * sizeof(char));/*lint !e737 */
        s_loc_mac = SCI_ALLOC_APP((mac_len + 1) * sizeof(char));/*lint !e737 */
        s_loc_algorithm = SCI_ALLOC_APP((algorithm_len + 1) * sizeof(char));/*lint !e737 */
        if(PNULL == s_loc_algorithm || PNULL == s_loc_algorithm ||PNULL == s_loc_username)
        {
            //SCI_TRACE_LOW:"MMIDM==> [RDM_parse_syncml]3 RDM_ERR_MEMORY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_536_112_2_18_2_14_9_454,(uint8*)"");
		SCI_FREE(syncml_hdr_ptr);
		syncml_hdr_ptr = PNULL;
            return RDM_ERR_MEMORY;
        }           
        username_occ_ptr[username_len] = '\0';
        mac_occ_ptr[mac_len] = '\0';
        algorithm_occ_ptr[algorithm_len] = '\0';
        SCI_MEMCPY(s_loc_username, username_occ_ptr, username_len + 1);
        SCI_MEMCPY(s_loc_mac, mac_occ_ptr, mac_len + 1);
        SCI_MEMCPY(s_loc_algorithm, algorithm_occ_ptr, algorithm_len + 1);
        *username = s_loc_username;
        //SCI_TRACE_LOW:"MMIDM==> [RDM_parse_syncml] s_loc_username =%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_548_112_2_18_2_14_9_455,(uint8*)"s",s_loc_username);
        *mac = s_loc_mac;
        //SCI_TRACE_LOW:"MMIDM==> [RDM_parse_syncml] s_loc_mac =%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_550_112_2_18_2_14_9_456,(uint8*)"s",s_loc_mac);
        *algorithm = s_loc_algorithm;
        //SCI_TRACE_LOW:"MMIDM==> [RDM_parse_syncml] s_loc_algorithm =%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_552_112_2_18_2_14_9_457,(uint8*)"s",s_loc_algorithm);
        //if(PNULL != syncml_hdr_ptr)
        {
            SCI_FREE(syncml_hdr_ptr);
            syncml_hdr_ptr = PNULL;
        }
        //VDM_PL_free(syncml_hdr);
    }
    else
    {
        *username = NULL;
        *mac = NULL;
        *algorithm = NULL;
        //SCI_TRACE_LOW:"MMIDM==> [RDM_parse_syncml]  no mac\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_565_112_2_18_2_14_9_458,(uint8*)"");
    }
    return RDM_ERR_OK;
}
//This for the part of vdm_comm_pl_conn

/********************************************************************************
* Initialize the Raw Connection API. 
*
* \param  outContext      VDM_Comm_PL_Conn's opaque context.
* \param  inObserver      Structure containing notification callbacks.
*
* \return VDM_ERR_OK on success, or an error code on failure.
********************************************************************************/
VDM_Error VDM_Comm_PL_Conn_init(
                                void**              outContext,
                                VDM_CommObserver_t* inObserver)
{
    VDM_Error   result = VDM_ERR_OK;
    
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_init "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_584_112_2_18_2_14_9_459,(uint8*)"");
    
    s_dl_observer = SCI_ALLOC_APP(sizeof(VDM_CommObserver_t));
    if (PNULL == s_dl_observer)
    {
        result = VDM_ERR_MEMORY;
        //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_init result = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_590_112_2_18_2_14_9_460,(uint8*)"d",result);
        return result;
    }
    SCI_MEMSET(s_dl_observer,0x00,sizeof(VDM_CommObserver_t));
    SCI_MEMCPY(s_dl_observer, inObserver, sizeof(VDM_CommObserver_t));
    *outContext = s_dl_observer;
    
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_init result = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_597_112_2_18_2_14_9_461,(uint8*)"d",result);
    
    return result;
}

/********************************************************************************
* Open a connection to the specified URL.
*
* \param   inContext   Conn context.
* \param   outConnId   A reference to the connection
* \param   inURL       The URL to open
*
* \return  VDM_ERR_OK on success, or an error code on failure.
********************************************************************************/
VDM_Error VDM_Comm_PL_Conn_open(
                                void*       inContext, 
                                IS32*       outConnId, 
                                UTF8CStr    inURL)
{
    VDM_Error           result = VDM_ERR_OK;
    char                serverName[RDM_DM_USERNAME_MAX_LEN] = {0};
   // int32               rc = 0;
   // long                ip = 0;
    uint16              port = 0;
    //struct sci_sockaddr      addr = {0};
    VDM_CommObserver_t* observer = (VDM_CommObserver_t*)inContext;
   // int                 socktid = 0;
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_open "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_623_112_2_18_2_14_9_462,(uint8*)"");

    if (PNULL == inURL)
    {
        //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_open PNULL == inURL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_627_112_2_18_2_14_9_463,(uint8*)"");
        return VDM_ERR_INVALID_CALL;
    }
    *outConnId = VDM_SessionType_DL;

    if(!MMIDM_ActivePdpConnect())
    {
        //MMIDM_ErrorAlert(0,VDM_SessionType_DL);
        //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_open:MMIDM_ActivePdpConnect not opened"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_635_112_2_18_2_14_9_464,(uint8*)"");
        result = observer->notifyCommBroken(observer->context, *outConnId, FALSE);
    }
    else
    {
        if(0 !=RDM_Socket_open(inURL, RDM_COMMS_GetDLSocketID_cbFunc)) /*lint !e64*/
        {
            //MMIDM_ErrorAlert(sci_sock_errno(s_dl_Socket_Id),VDM_SessionType_DL);
            //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_open8:connection not opened to %s:%hu\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_643_112_2_18_2_14_9_465,(uint8*)"sd", serverName, port);
            result = observer->notifyCommBroken(observer->context, *outConnId, FALSE);
        }
    }

    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_open9 (connId = %d) returns 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_648_112_2_18_2_14_9_466,(uint8*)"dd", *outConnId, result);
    return result;
}


/********************************************************************************
* Send the request content to the server
*
* \param   inContext       Conn context.
* \param   inConnId        Reference to the connection, as returned by 
*                          \ref VDM_Comm_PL_Conn_open().
* \param   inData          The buffer which holds the request content
* \param   inDataLength    The request length
*
* \return  VDM_ERR_OK on success, or an error code on failure.
********************************************************************************/
VDM_Error VDM_Comm_PL_Conn_send(
    void* inContext, 
    IS32  inConnId, 
    IU8*  inData, 
    IU32  inDataLen)
{
    VDM_Error   result = VDM_ERR_OK;
    uint32      actualSent = 0;
    VDM_CommObserver_t* observer = (VDM_CommObserver_t*)inContext;
    
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_send "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_673_112_2_18_2_14_9_467,(uint8*)"");
    actualSent = RDM_Socket_Send(s_dl_Socket_Id,inData,inDataLen);  /*lint !e64*/
    
    //需要改成异步  
    if (inDataLen == actualSent)
    {
        //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_Conn_send success  actualSent = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_679_112_2_18_2_14_9_468,(uint8*)"d", actualSent);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_send :send attempt failed\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_683_112_2_18_2_14_9_469,(uint8*)"");
        //MMIDM_ErrorAlert(sci_sock_errno(s_dl_Socket_Id),VDM_SessionType_DL);
        result = observer->notifyCommBroken(observer->context, VDM_SessionType_DL, FALSE);
    }   
    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_Conn_send (connId = %d) returns 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_687_112_2_18_2_14_9_470,(uint8*)"dd", inConnId, result);
    return result;
}

/********************************************************************************
* Read the content of the servers reply into a pre-allocated buffer of the
* indicated length. This function can be called multiple times until all the
* content has been read.
*
* \param   inContext   Conn context.
* \param   inConnId    Reference to the connection, as returned by 
*                      \ref VDM_Comm_PL_Conn_open().
* \param   ioBuf       Pre-allocated buffer to receive the content
* \param   inBufSize   Size of the pre-allocated buffer.
* \param   outDataLen  The actual length of the data read. Set to -1 
*                      if there is no more data.
*
* \return  VDM_ERR_OK on success, or an error code on failure.
********************************************************************************/
VDM_Error VDM_Comm_PL_Conn_receive(
    void*   inContext, 
    IS32    inConnId, 
    IU8 *   inBuf,
    IU32    inBufSize, 
    IS32 *  outDataLen)
{
    VDM_Error   result = VDM_ERR_OK;
    int left_data_len = 0;
    VDM_CommObserver_t* observer = (VDM_CommObserver_t*)inContext;

    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_receive  inBufSize = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_716_112_2_18_2_14_9_471,(uint8*)"d",inBufSize);
    *outDataLen = sci_sock_asyncrecv(s_dl_Socket_Id, (char*)inBuf, inBufSize,0,&left_data_len);

    if (0 >*outDataLen)
    {
        if(EWOULDBLOCK != sci_sock_errno(s_dl_Socket_Id))
        {
            //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_receive: recv attempt failed sock_errno =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_723_112_2_18_2_14_9_472,(uint8*)"d",sci_sock_errno(s_dl_Socket_Id));
            result = observer->notifyCommBroken(observer->context, VDM_SessionType_DL, FALSE);
        }
    }
    else if( 0 == *outDataLen)
    {
        //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_receive: outDataLen = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_729_112_2_18_2_14_9_473,(uint8*)"");
        result = observer->notifyCommBroken(observer->context, VDM_SessionType_DL, FALSE);
    }
    else 
    {
        if( left_data_len > 0 )
        {
            observer->notifyTransport(observer->context,VDM_SessionType_DL);
            //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_receive notifyTransport left_data_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_737_112_2_18_2_14_9_474,(uint8*)"d",left_data_len);
        }
    }
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_receive (connId = %d) returns 0x%x, *outDataLen=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_740_112_2_18_2_14_9_475,(uint8*)"ddd", inConnId, result,*outDataLen);

    return result;
}

/********************************************************************************
* Close the connection previously opened with VDM_Comm_PL_Conn_open().
*
* \param   inContext   Conn context.
* \param   inConnId    Reference to the connection, as returned by 
*                      VDM_Comm_PL_Conn_open().
********************************************************************************/
void VDM_Comm_PL_Conn_close(
    void*   inContext, 
    IS32    inConnId)
{
    sci_sock_socketclose(s_dl_Socket_Id);
    s_dl_Socket_Id = 0;
    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_Conn_close (connId = %d) returns \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_757_112_2_18_2_14_9_476,(uint8*)"d", inConnId);
}

/********************************************************************************
* Terminate the Conn Connection system and free any resources in use.
*
* \param   inContext   Conn context.
********************************************************************************/
VDM_Error VDM_Comm_PL_Conn_term(void* inContext)
{
    //VDM_CommObserver_t* observer = (VDM_CommObserver_t*)inContext;
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_term "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_767_112_2_18_2_14_9_477,(uint8*)"");
    
    if(PNULL != s_dl_observer)
    {
        SCI_FREE(s_dl_observer);
        s_dl_observer = PNULL;
    }
    
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_Conn_term returns \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_775_112_2_18_2_14_9_478,(uint8*)"");
    return VDM_ERR_OK;
}
#endif



//This for the part of vdm_comm_pl_http
/********************************************************************************
* Initialize the http communication support code.
*
* Provide the AddrType to be used for the SyncMLDM access. 
*
* \param   outContext VDM_Comm_PL_HTTP's opaque context.
*
* \param   inAddrType  The type of the address specified. From the DMStdObj
*                      specification the possible values are:"URI","IPv4","IPv6".
*
* \param   inMaxMsgSize    Maximum size allowed for a message.
*
* \param   inObserver      A structure containing callback functions that 
*                          must be invoked at the end of each async operation
*
*
* \return  \ref VDM_ERR_defs "An error code" (VDM_ERR_OK if no error).
********************************************************************************/  
#ifdef DM_SUPPORT_DFRESH                              
PUBLIC VDM_Error VDM_Comm_PL_HTTP_init_dfresh(void)
{
    VDM_Error result = VDM_ERR_OK;

    if (PNULL == s_dm_OutPackage_ptr)
    {
        s_dm_OutPackage_ptr =SCI_ALLOC_APP(RDM_MAX_SEND_PACKAGE);
    }
    SCI_MEMSET(s_dm_OutPackage_ptr,0,RDM_MAX_SEND_PACKAGE);

    if (PNULL == s_connect_info_ptr)
    {
        s_connect_info_ptr = SCI_ALLOC_APP(sizeof(RDM_CONNECT_INFO_T));
    }
    SCI_MEMSET(s_connect_info_ptr,0,sizeof(RDM_CONNECT_INFO_T));

    if (PNULL == s_recvBuffer_ptr)
    {
        s_recvBuffer_ptr =SCI_ALLOCA(RDM_SOCKET_DM_RECEV_BUFFER_LEN+1);
    }
    if (PNULL != s_recvBuffer_ptr)
    {
        SCI_MEMSET(s_recvBuffer_ptr,0,(RDM_SOCKET_DM_RECEV_BUFFER_LEN+1));
    }
    else
    {
        result = VDM_ERR_MEMORY;
    }
    s_recvpos = 0;   
    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_HTTP_init returns 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_830_112_2_18_2_14_9_479,(uint8*)"d", result);
    
    return result;
}
#elif defined(DM_SUPPORT_REDBENCH)
PUBLIC VDM_Error VDM_Comm_PL_HTTP_init(
    void**              outContext,
    UTF8CStr            inAddrType,
    IU32                inMaxMsgSize,
    VDM_CommObserver_t* inObserver)
{
    VDM_Error result = VDM_ERR_OK;
    
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_HTTP_init called (inAddrType=%s)\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_843_112_2_18_2_14_9_480,(uint8*)"s", inAddrType);
    UNUSED(inAddrType);
    UNUSED(inMaxMsgSize);
    
    if (PNULL == s_dm_observer)
    {
        s_dm_observer = SCI_ALLOC_APP(sizeof(VDM_CommObserver_t));
    }
    SCI_MEMSET(s_dm_observer,0x00,sizeof(VDM_CommObserver_t))
    SCI_MEMCPY(s_dm_observer, inObserver, sizeof(VDM_CommObserver_t)); 

    if (PNULL == s_dm_OutPackage_ptr)
    {
        s_dm_OutPackage_ptr =SCI_ALLOC_APP(RDM_MAX_SEND_PACKAGE);
    }
    SCI_MEMSET(s_dm_OutPackage_ptr,0,RDM_MAX_SEND_PACKAGE);

    if (PNULL == s_connect_info_ptr)
    {
        s_connect_info_ptr = SCI_ALLOC_APP(sizeof(RDM_CONNECT_INFO_T));
    }
    SCI_MEMSET(s_connect_info_ptr,0,sizeof(RDM_CONNECT_INFO_T));

    if (PNULL == s_recvBuffer_ptr)
    {
        s_recvBuffer_ptr =SCI_ALLOCA(RDM_SOCKET_DM_RECEV_BUFFER_LEN+1);
    }
    if (PNULL != s_recvBuffer_ptr)
    {
        SCI_MEMSET(s_recvBuffer_ptr,0,(RDM_SOCKET_DM_RECEV_BUFFER_LEN+1));
    }
    else
    {
        result = VDM_ERR_MEMORY;
    }
    s_recvpos = 0;
   
    *outContext = NULL;
    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_HTTP_init called s_dm_observer=%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_881_112_2_18_2_14_10_481,(uint8*)"d", s_dm_observer);
 
    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_HTTP_init returns 0x%x\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_883_112_2_18_2_14_10_482,(uint8*)"d", result);
    
    return result;
}
#endif   

/********************************************************************************
* Mark the start of a new message to the server.
*
* If HMAC authentication is to be sent then both username and mac must be
* supplied but algorithm need only be supplied if not MD5.
*
* \param   inContext   The context previously set by VDM_Comm_PL_HTTP_init().
*
* \param   outConnId   Opaque application data, used to identify the
*                      connection. every subsequent event/API regarding
*                      this message should pass this value
*
* \param   inAddr      The address of the SyncML Server.
*
* \param   inMsgLen    The length of the message to be sent.
*
* \param   inMimeType  Whether the data is in DM-WBXML, DM-XML, DS-WBXML, DS-XML format.
*
* \param   inHMACinfo  Information used for HMAC authentication or NULL.
*
* \return  \ref VDM_ERR_defs "An error code" (VDM_ERR_OK if no error)
********************************************************************************/
PUBLIC VDM_Error VDM_Comm_PL_HTTP_open(
    void*            inContext, 
    IS32*            outConnId, 
    UTF8Str          inAddr, 
    IS32             inMsgLen,
    IU32             inMimeType,
    VDM_Comm_HMAC_t* inHMACinfo)
{
    const char *ipA = PNULL;
    //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_open] begin, *outConnId = %d, addr = %s \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_919_112_2_18_2_14_10_483,(uint8*)"ds", *outConnId, inAddr);
    //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_open]   inMimeType = %d, msgLen = %d,addr = %d \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_920_112_2_18_2_14_10_484,(uint8*)"ddd", inMimeType, inMsgLen);
    
    
    SCI_MEMSET(s_connect_info_ptr->username,0x00,RDM_DM_USERNAME_MAX_LEN);
    SCI_MEMSET(s_connect_info_ptr->mac,0x00,RDM_DM_MAC_MAX_LEN);
    SCI_MEMSET(s_connect_info_ptr->algorithm,0x00,RDM_DM_ALGORITHM_LEN);  
    SCI_MEMSET(s_connect_info_ptr->server_url,0x00, RDM_DM_SERVER_URL_LEN);
#if !defined (DM_SUPPORT_DFRESH) && !defined (DM_SUPPORT_REDBENCH)
    s_dm_isWBXML = (int16)(inMimeType ? 1:0) ;
#endif
    if(inAddr)
    {
        strcpy(s_connect_info_ptr->server_url, (const char*)inAddr);
    }
    if(inHMACinfo)
    {
        //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_open]   username %x = %s, mac %x = %s, algorithm %x = %s \r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_935_112_2_18_2_14_10_485,(uint8*)"dsdsds", inHMACinfo->username,inHMACinfo->username, inHMACinfo->mac,inHMACinfo->mac,inHMACinfo->algorithm, inHMACinfo->algorithm);
        if(inHMACinfo->username)
        {
            strcpy(s_connect_info_ptr->username, (char*)inHMACinfo->username);
        }
        if(inHMACinfo->mac)
        {
            strcpy(s_connect_info_ptr->mac, (char*)inHMACinfo->mac);
        }
        if(inHMACinfo->algorithm)
        {
            strcpy(s_connect_info_ptr->algorithm, (char*)inHMACinfo->algorithm);
        }
    }
    ipA = (const char *)inAddr;
    //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_open] s_connect_info_ptr->server_url = %s\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_950_112_2_18_2_14_10_486,(uint8*)"s",s_connect_info_ptr->server_url);
    *outConnId = VDM_SessionType_DM;
    
    if(!MMIDM_ActivePdpConnect())
    {
        //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_open] ActivePdp fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_955_112_2_18_2_14_10_487,(uint8*)"");
#ifdef DM_SUPPORT_REDBENCH
        s_dm_observer->notifyCommBroken(s_dm_observer->context,VDM_SessionType_DM,FALSE);
#endif        
    }
    else
    {
        if(0 !=RDM_Socket_open(ipA, RDM_COMMS_GetDMSocketID_cbFunc)) /*lint !e64*/
        {
            //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_open] open fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_964_112_2_18_2_14_10_488,(uint8*)"");
#ifdef DM_SUPPORT_REDBENCH
            s_dm_observer->notifyCommBroken(s_dm_observer->context,*outConnId,FALSE);
#endif            
        }
    }
    return VDM_ERR_OK;
}

/********************************************************************************
* Send the data for the next msg to the specified address. This function is
* not expected to return until all the data has been sent.
*
* \param   inContext   The context previously set by VDM_Comm_PL_HTTP_init().
*
* \param   inConnId    Reference to the connection, as returned by 
*                      VDM_Comm_PL_HTTP_open().
*
* \param   inMsg       Pointer to the data to be sent.
*
* \param   inMsgLen    The length of the data to send.
*
* \return  \ref VDM_ERR_defs "An error code" (VDM_ERR_OK if no error)
********************************************************************************/
PUBLIC VDM_Error VDM_Comm_PL_HTTP_send(
    void*   inContext, 
    IS32    inConnId, 
    IU8 *   inMsg, 
    IS32    inMsgLength)
{
    RDM_Error   result = RDM_ERR_OK;
    int16         total = 0;
    unsigned int sentLen = 0;
#ifdef DM_SUPPORT_REDBENCH   
    VDM_CommObserver_t* observer = (VDM_CommObserver_t*)inContext;//mary
#endif    
    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_HTTP_send\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_999_112_2_18_2_14_10_489,(uint8*)"");
    
    total = (int16)RDM_COMMS_Construct_http(s_dm_OutPackage_ptr, RDM_MAX_SEND_PACKAGE, inMsg, inMsgLength);
    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_HTTP_send total %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1002_112_2_18_2_14_10_490,(uint8*)"d", total);
    if(total > 0)
    {
        sentLen = RDM_Socket_Send(s_dm_Socket_Id,s_dm_OutPackage_ptr,total); /*lint !e64*/
        //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_HTTP_send sentLen %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1006_112_2_18_2_14_10_491,(uint8*)"d", sentLen);
        if (sentLen!=total)/*lint !e737 */
        {
#ifdef DM_SUPPORT_REDBENCH            
            result = observer->notifyCommBroken(observer->context,VDM_SessionType_DM,FALSE);//mary
#endif            
        }
    }
    else
    {
#ifdef DM_SUPPORT_REDBENCH       
        result = observer->notifyCommBroken(observer->context,VDM_SessionType_DM,FALSE);//mary
#endif        
    }

    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_HTTP_send, total = %d, sentLen = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1021_112_2_18_2_14_10_492,(uint8*)"dd",total, sentLen);
    return result;
}
/********************************************************************************
* Receive a reply. The data is expected to be written to the buffer supplied
* in the argument 'ioMsgBuf', which can hold a maximum of 'inMsgBufSize' bytes of data.
* The length of the data read in is returned in 'outMsgLen'. This function is
* expected to read the full content of the reply before returning. If the
* buffer is not large enough, VDM_ERR_BUFFER_OVERFLOW should be returned and
* the required buffer size returned in outMsgLen. If the mime-type of the reply does
* not match the mime-type for the document sent (ie if XML is received when
* WBXML was sent) then an error (VDM_ERR_COMMS_MIME_MISMATCH) should be
* returned.
*
* \param   inContext    The context previously set by VDM_Comm_PL_HTTP_init().
*
* \param   inConnId     Reference to the connection, as returned by 
*                       VDM_Comm_PL_HTTP_open().
*
* \param   ioMsgBuf     Pointer to a buffer to receive the incoming message.
*
* \param   inMsgBufSize The maximum number of bytes which can be held in the
*                       supplied buffer.
*
* \param   outMsgLen    The number of bytes read.
*
* \param   outHMACinfo  Structure that will be filled with information from the
*                       HMAC credentials or NULL if no HMAC credentials were
*                       supplied. This pointer need only remain valid until
*                       VDM_Comm_PL_DM_endMessage() is called.
*
* \return  \ref VDM_ERR_defs "An error code" (VDM_ERR_OK if no error)
********************************************************************************/
#ifdef DM_SUPPORT_REDBENCH
VDM_Error VDM_Comm_PL_HTTP_receive(
    void*            inContext, //dataPtr
    IS32             inConnId,
    IU8 *            ioMsgBuf, 
    IS32             inMsgBufSize,
    IS32*            outMsgLen,
    VDM_Comm_HMAC_t* outHMACinfo)   //username, mac, algorithm
{ 
    RDM_Error ret = RDM_ERR_OK;   
    
    //int32   totalRead = 0;
    int32 LenHTTPbody = 0;
    uint8* t1_ptr = PNULL;   
    //uint8* findMac = NULL;
    // The actual recv was done inside RDM_COMMS_sendMessage and stored locally.
    // Here we just copy the stored data.

    //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_receive]  bufSize = %d, s_recvActualLen = %d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1071_112_2_18_2_14_10_493,(uint8*)"dd",inMsgBufSize, s_recvActualLen);
    
    if ((uint32)inMsgBufSize < s_recvActualLen)
    {
        //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_receive]  Error:RDM_ERR_BUFFER_OVERFLOW!\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1075_112_2_18_2_14_10_494,(uint8*)"");
        ret = RDM_ERR_BUFFER_OVERFLOW;
    }
    
    t1_ptr = (uint8*)RDM_COMMS_findSubStringN((uint8*)s_recvBuffer_ptr,(uint8*)"\r\n\r\n",(int)s_recvActualLen);
    
    if (PNULL == t1_ptr)
    {  
        //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_receive]  return error when cut the http header, t1_ptr=0x%x\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1083_112_2_18_2_14_10_495,(uint8*)"d",t1_ptr);
        ret =  RDM_ERR_BAD_INPUT;
        return ret;
    }
    else
    {
        LenHTTPbody = (uint32)s_recvBuffer_ptr+ s_recvActualLen-(IU32)t1_ptr-4;
        //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_receive]  LenHTTPbody=0x%x\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1090_112_2_18_2_14_10_496,(uint8*)"d",LenHTTPbody);
        
        SCI_MEMCPY(ioMsgBuf,t1_ptr+4,LenHTTPbody);
        *outMsgLen = LenHTTPbody;
    }


    if(PNULL != s_loc_algorithm)
    {
        SCI_FREE(s_loc_algorithm);
        s_loc_algorithm = PNULL;
    }
    if(PNULL != s_loc_username)
    {
        SCI_FREE(s_loc_username);
        s_loc_username = PNULL;
    }
    if(PNULL != s_loc_mac)
    {
        SCI_FREE(s_loc_mac);
        s_loc_mac = PNULL;
    }

    ret = RDM_parse_syncml((char*)s_recvBuffer_ptr, (char**)&(outHMACinfo->username), (char**)&(outHMACinfo->mac), (char**)&(outHMACinfo->algorithm));
  
    //SCI_TRACE_LOW:"MMIDM==> RDM_COMMS_recvMessage return ret =%d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1115_112_2_18_2_14_10_497,(uint8*)"d", ret);
    return ret;
    
}
#elif defined(DM_SUPPORT_DFRESH)
VDM_Error VDM_Comm_PL_HTTP_receive_dfresh(
    void*            inContext, //dataPtr
    IS32             inConnId,
    IU8 *            ioMsgBuf, 
    IS32             inMsgBufSize,
    IS32*            outMsgLen)
   
{ 
    BOOL ret = TRUE;   
    
    //int32   totalRead = 0;
    int32 LenHTTPbody = 0;
    uint8* t1_ptr = PNULL;   
    //uint8* findMac = NULL;
    // The actual recv was done inside RDM_COMMS_sendMessage and stored locally.
    // Here we just copy the stored data.

    //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_receive]  bufSize = %d, s_recvActualLen = %d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1137_112_2_18_2_14_10_498,(uint8*)"dd",inMsgBufSize, s_recvActualLen);
    
    if ((uint32)inMsgBufSize < s_recvActualLen)
    {
        //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_receive]  Error:RDM_ERR_BUFFER_OVERFLOW!\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1141_112_2_18_2_14_10_499,(uint8*)"");
        ret = RDM_ERR_BUFFER_OVERFLOW;
    }
    
    t1_ptr = (uint8*)RDM_COMMS_findSubStringN((uint8*)s_recvBuffer_ptr,(uint8*)"\r\n\r\n",(int)s_recvActualLen);
    
    if (PNULL == t1_ptr)
    {  
        //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_receive]  return error when cut the http header, t1_ptr=0x%x\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1149_112_2_18_2_14_10_500,(uint8*)"d",t1_ptr);
        ret =  RDM_ERR_BAD_INPUT;
        return ret;
    }
    else
    {
        LenHTTPbody = (uint32)s_recvBuffer_ptr+ s_recvActualLen-(uint32)t1_ptr-4;
        //SCI_TRACE_LOW:"MMIDM==> [VDM_Comm_PL_HTTP_receive]  LenHTTPbody=0x%x\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1156_112_2_18_2_14_10_501,(uint8*)"d",LenHTTPbody);
        
        SCI_MEMCPY(ioMsgBuf,t1_ptr+4,LenHTTPbody);
        *outMsgLen = LenHTTPbody;
    }

    MMIDM_ParseReceiveData(s_recvBuffer_ptr);  
    

    //SCI_TRACE_LOW:"MMIDM==> RDM_COMMS_recvMessage return ret =%d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1165_112_2_18_2_14_10_502,(uint8*)"d", ret);
    return ret;
    
}
#endif 
/********************************************************************************
* End the request/response exchange for the current message.
*
* \param   inContext   The context previously set by VDM_Comm_PL_HTTP_init().
*
* \param   inConnId    Reference to the connection, as returned by 
*                      VDM_Comm_PL_HTTP_open().
*******************************************************************************/
PUBLIC void VDM_Comm_PL_HTTP_close(
    void*   inContext, 
    IS32    inConnId)
{
    sci_sock_socketclose(s_dm_Socket_Id);
    s_dm_Socket_Id = 0;
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_HTTP_close (connId = %d) returns\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1183_112_2_18_2_14_10_503,(uint8*)"d", inConnId);
}
/********************************************************************************
* Terminate the connection.
*
* Terminate the communications support code and perform any cleanup required.
*
* \param   inContext   The context previously set by VDM_Comm_PL_HTTP_init().
*
* \return  \ref VDM_ERR_defs "An error code" (VDM_ERR_OK if no error)
********************************************************************************/
PUBLIC VDM_Error VDM_Comm_PL_HTTP_term(void* inContext)
{
    //SCI_TRACE_LOW:"MMIDM==>  VDM_Comm_PL_HTTP_term called\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1195_112_2_18_2_14_10_504,(uint8*)"");
#ifdef DM_SUPPORT_REDBENCH
    if(PNULL != s_loc_algorithm)
    {
        SCI_FREE(s_loc_algorithm);
        s_loc_algorithm = PNULL;
    }
    if(PNULL != s_loc_username)
    {
        SCI_FREE(s_loc_username);
        s_loc_username = PNULL;
    }
    if(PNULL != s_loc_mac)
    {
        SCI_FREE(s_loc_mac);
        s_loc_mac = PNULL;
    }
#endif    
    
    if (PNULL != s_dm_OutPackage_ptr)
    {
        SCI_FREE(s_dm_OutPackage_ptr);
        s_dm_OutPackage_ptr = PNULL;
    }
#ifdef DM_SUPPORT_REDBENCH
    if(PNULL != s_dm_observer)
    {
        SCI_FREE(s_dm_observer);
        s_dm_observer = PNULL;
    }
#endif

    if (PNULL!=s_connect_info_ptr)
    {
        SCI_FREE(s_connect_info_ptr);
        s_connect_info_ptr = PNULL;
    }
    //SCI_TRACE_LOW:"5"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1232_112_2_18_2_14_10_505,(uint8*)"");
    if (PNULL != s_recvBuffer_ptr)
    {
        SCI_FREE(s_recvBuffer_ptr);
        s_recvBuffer_ptr = PNULL;
    }

    //SCI_TRACE_LOW:"MMIDM==> VDM_Comm_PL_HTTP_term returns\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1239_112_2_18_2_14_10_506,(uint8*)"");
    return VDM_ERR_OK;
}

/********************************************************************************
* send data
*
* \param   socketId        socket id
* \param    buffer      the pointer of the buffer load the data want to be sent
* \param    write_len             the length of the data want to be sent
*
* \return         how many bytes have been sent in fact
*******************************************************************************/
LOCAL unsigned int RDM_Socket_Send (unsigned long socketId,const char * buffer, unsigned int write_len)
{
  //  unsigned int i = 0;
    unsigned int sent_len = 0;
    unsigned int total_len = write_len;
    //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Send], total_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1256_112_2_18_2_14_10_507,(uint8*)"d",total_len);
    while( total_len > 0)
    {
        unsigned int length = 0;
        length = sci_sock_send(socketId, (char *)buffer + sent_len, total_len, 0);
        if( length > 0 )
        {
            // 发送成功，记录发送长度
            total_len -= length;
            sent_len += length;
        }
        else
        {
            // 发送失败，进行异常处理
            //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Send], length = 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1270_112_2_18_2_14_10_508,(uint8*)"");
            break;
        }
        
    }
    //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Send], sent_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1275_112_2_18_2_14_10_509,(uint8*)"d",sent_len);

    return sent_len;
}

/********************************************************************************
* connect DM server
*
* \param   url            address of DM server
* \param           getSocketIDcbFunc      a callback function, pass the socket id to lib
*
* \return         a value
*******************************************************************************/
IS32 RDM_Socket_open(const char * url, RDM_GET_SOCKET_ID_cbFunc getSocketIDcbFunc)
{
    IS32 retVal = -1;
    int len =0;
    
    if(PNULL == url)
    {
        return retVal;
    }
    s_rdm_socket_info.v_cbFunc = getSocketIDcbFunc;/*lint !e527*/
    memset(s_rdm_socket_info.v_ip_addr, 0x00, RDM_DM_SERVER_URL_LEN);
    
    if(CONNECT_CMWAP == MMIDM_GetPdpConnectType())//cmwap
    {
        len = strlen(MMIDM_GetServerUrl());
        memcpy(s_rdm_socket_info.v_ip_addr,MMIDM_GetServerUrl(),len);
    }
    else if(CONNECT_CMNET == MMIDM_GetPdpConnectType())
    {
    	 len = strlen(url);
        memcpy(s_rdm_socket_info.v_ip_addr, url, MIN(RDM_DM_SERVER_URL_LEN, len));
    }
    else
    {
         len = strlen(url);
        memcpy(s_rdm_socket_info.v_ip_addr, url, MIN(RDM_DM_SERVER_URL_LEN, len));
    }
    
    //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_open] url = %s \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1315_112_2_18_2_14_10_510,(uint8*)"s", url);
    
    retVal = RDM_Socket_Connect_Server(s_rdm_socket_info.v_ip_addr); /*lint !e64*/
    return retVal;
}


LOCAL int RDM_Socket_Connect_Server(const char * serverNameURL)
{
    int ret = -1;
    const char * server_name = PNULL;
    int32 gv_Rdm_Socket = 0;
    //int error = 0;
    char *temp = NULL;
    char *colonSign = NULL;
    BOOLEAN hasPort = FALSE;
    unsigned int portNumber =0;
    char ServerAddress[RDM_DM_SERVER_URL_LEN] = {0};   
    uint32   max_len = 0;
    uint32   len= 0;
    gv_Rdm_Socket = RDM_Socket_Create(AF_INET, SOCK_STREAM, 0);//IPPROTO_TCP
    //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Connect_Server] gv_Rdm_Socket = 0x%x \r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1336_112_2_18_2_14_10_511,(uint8*)"d", gv_Rdm_Socket);
    if( gv_Rdm_Socket == INVALID_SOCKET) 
    {
        return ret;
    }
    s_rdm_socket_info.v_cbFunc(gv_Rdm_Socket);
    s_rdm_socket_info.v_socket_id = gv_Rdm_Socket;
    
    server_name = serverNameURL;//s_rdm_socket_info.v_ip_addr;
    
    memset(ServerAddress, 0x00, RDM_DM_SERVER_URL_LEN);
    
    if(strstr(server_name, "http://"))
    {
    	len = (uint32)strlen(server_name)-7;
        max_len = MIN(len,RDM_DM_SERVER_URL_LEN-7);///*lint !e506 */
        //SCI_TRACE_LOW:"MMIDM==> RDM_Socket_Connect_Server, has http max_len= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1352_112_2_18_2_14_10_512,(uint8*)"d",max_len);
        memcpy(ServerAddress, server_name+7, max_len);///*lint !e420 */
    }
    else if(strstr(server_name, "https://"))
    {
    	 len = (uint32)strlen(server_name)-8;
        max_len = MIN(len,RDM_DM_SERVER_URL_LEN-8);/*lint !e64*/
        //SCI_TRACE_LOW:"MMIDM==> RDM_Socket_Connect_Server, has https max_len= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1359_112_2_18_2_14_10_513,(uint8*)"d",max_len);
        memcpy(ServerAddress, server_name+8, max_len);///*lint !e420 */
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> RDM_Socket_Connect_Server, has not http and https\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1364_112_2_18_2_14_10_514,(uint8*)"");
	len = (uint32)strlen(server_name);
        memcpy(ServerAddress, server_name, MIN(len, sizeof(ServerAddress)));/*lint !e64*/
    }
    //SCI_TRACE_LOW:"MMIDM==> RDM_Socket_Connect_Server, ServerAddress = %s\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1368_112_2_18_2_14_11_515,(uint8*)"s", ServerAddress);
    
    temp = strstr(ServerAddress,"/");
    if(temp)
    {
        *temp = '\0';
    }
    //SCI_TRACE_LOW:"MMIDM==> RDM_Socket_Connect_Server, ServerAddress delete / = %s\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1375_112_2_18_2_14_11_516,(uint8*)"s", ServerAddress);
    
    colonSign = strstr(ServerAddress, ":");
    if(PNULL!=colonSign)
    {
        IBOOL result;
        
        portNumber = VDM_PL_atoIU32(colonSign+1,10, &result);
        hasPort = TRUE;
        colonSign[0] = '\0';
        s_rdm_socket_info.v_port = portNumber;
        //SCI_TRACE_LOW:"MMIDM==> RDM_Socket_Connect_Server, ServerAddress delete : = %s, portNumber = %d\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1386_112_2_18_2_14_11_517,(uint8*)"sd", ServerAddress,portNumber);
    }
    else
    {
        hasPort = FALSE;
        portNumber = DM_CMWAP_PORT_INFO;
        s_rdm_socket_info.v_port = DM_CMWAP_PORT_INFO;
    }
    
    if(RDM_UrlIsAnIpAddress(ServerAddress))
    {
        ret = RDM_Socket_Connect_by_ip(gv_Rdm_Socket, ServerAddress, portNumber, 1000);
        
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> RDM_Socket_Connect_Server no ip[%s],port=[%d]\r\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1402_112_2_18_2_14_11_518,(uint8*)"sd",ServerAddress,portNumber);
        ret = RDM_Socket_Connect_by_name(gv_Rdm_Socket, ServerAddress, portNumber, 1000);  
        
    }
    //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Connect_Server], ret = %x\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1406_112_2_18_2_14_11_519,(uint8*)"d", ret);
    return ret;
}

LOCAL int32 RDM_Socket_Create(int af, int type, int protocol)
{
    int32 socket_id = INVALID_SOCKET;
    int iError = 0;
    socket_id  = sci_sock_socket(af, type, protocol, MMIDM_GetPdpNetID());
    //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Create], socket_id = %x, netid = %drn"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1416_112_2_18_2_14_11_520,(uint8*)"dd", socket_id, MMIDM_GetPdpNetID());
    if(INVALID_SOCKET != socket_id)
    {
        iError = sci_sock_asyncselect(socket_id,
            MMIDM_GetDmTaskID(),
            AS_READ|AS_WRITE|AS_CONNECT
            );
        if(iError < 0)
        {
            sci_sock_socketclose(socket_id);
            return -1;
        }
        return socket_id;
    }
    else
    {
        //iError = WSAGetLastError();
        //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Create], iError = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1433_112_2_18_2_14_11_521,(uint8*)"d", iError);
    }
    return socket_id;
}

LOCAL BOOLEAN RDM_UrlIsAnIpAddress(const char* url)
{
    int v_len=0;
    int i=0;
    int index= 0;
    char IPaddress[16]={0};
    
    if (PNULL == url)
    {
        return FALSE;
    }

    v_len=strlen(url);
    if(v_len > 15)
    {
        v_len = 15;
    }

    memset(IPaddress, 0x00, sizeof(IPaddress));
    for(index = 0; index <= v_len; index++)
    {
        IPaddress[index] = url[index];
        if( ':' == url[index] || '/' == url[index])
        {
            IPaddress[index] = '\0';
            break;
        }
    }
    //SCI_TRACE_LOW:"MMIDM==> RDM_UrlIsAnIpAddress, url = %s\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1466_112_2_18_2_14_11_522,(uint8*)"s", url);
    //SCI_TRACE_LOW:"MMIDM==> RDM_UrlIsAnIpAddress, IPaddress = %s\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1467_112_2_18_2_14_11_523,(uint8*)"s", IPaddress);
    v_len = strlen(IPaddress);
    //SCI_TRACE_LOW:"MMIDM==> RDM_UrlIsAnIpAddress, v_len = %d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1469_112_2_18_2_14_11_524,(uint8*)"d", v_len);
    for( i = 0 ; i < v_len; i++)
    {
        
        if( ( IPaddress[i] > '9'  || IPaddress[i] < '0'  ) && (IPaddress[i] != '.' ))
        {     
            //SCI_TRACE_LOW:"MMIDM==> RDM_UrlIsAnIpAddress ---url is a domain name\r\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1475_112_2_18_2_14_11_525,(uint8*)"");
            return FALSE;
        }
    }
    //SCI_TRACE_LOW:"RDM_UrlIsAnIpAddress ---url is an dot decimal ip\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1479_112_2_18_2_14_11_526,(uint8*)"");
    return TRUE;
}


LOCAL int RDM_Socket_Connect_by_ip(unsigned long socketId,const char* ip, unsigned int port, unsigned int timeout)
{
    int ret = -1;
    struct sci_sockaddr target_socket_address;
    int error = 0;
    //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Connect_by_ip], ip = %s,port = %d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1489_112_2_18_2_14_11_527,(uint8*)"sd", ip, port);
    target_socket_address.family = AF_INET;
    target_socket_address.port = htons(port);
    target_socket_address.ip_addr = inet_addr(ip);

    ret =  sci_sock_connect(socketId, (struct sci_sockaddr*)&target_socket_address, sizeof(target_socket_address));
    error = sci_sock_errno(socketId);
    if (EINPROGRESS == error)
    {
        ret = 0;
    }
    //SCI_TRACE_LOW:"[RDM_Socket_Connect_by_ip], ret = %d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1500_112_2_18_2_14_11_528,(uint8*)"d", ret);
    return ret;
}


LOCAL int RDM_Socket_Connect_by_name(unsigned long socketId,const char* server_name, unsigned int port, unsigned int timeout)
{
    struct sci_hostent *host = NULL;
    struct sci_sockaddr target_socket_address;
    unsigned long *pAddr;
    int ret = -1;   
    int error = 0;
    //SCI_TRACE_LOW:"MMIDM==> [RDM_Socket_Connect_by_name], server_name = %s,port = %d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1512_112_2_18_2_14_11_529,(uint8*)"sd", server_name, port);
    
    host = sci_gethostbyname(server_name);/*lint !e605*/
    
    if(NULL == host)
    {
        ret = -1;
    }
    else
    {
        target_socket_address.family = AF_INET;
        target_socket_address.port = htons(port);
        pAddr = (unsigned long *)host->h_addr_list[0];
        target_socket_address.ip_addr = *pAddr;
        ret =  sci_sock_connect(socketId, (struct sci_sockaddr*)&target_socket_address, sizeof(target_socket_address));
        error = sci_sock_errno(socketId);
        if (EINPROGRESS == error)
        {
            ret = 0;
        }
    }
    //SCI_TRACE_LOW:"[RDM_Socket_Connect_by_name], ret = %d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1533_112_2_18_2_14_11_530,(uint8*)"d", ret);
    return ret;
}

void VDM_PL_DD_field_value(UTF8CStr inField, UTF8CStr inValue)
{
    //SCI_TRACE_LOW:"MMIDM==> VDM_PL_DD_field_value: inField=%s, inValue=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1539_112_2_18_2_14_11_531,(uint8*)"ss", inField, inValue);
    //  if (!VDM_UTL_utf8cmp("size", inField))
    //  {
    //      g_pkgSize = (unsigned int)VDM_PL_atoIU32((const char*)inValue, 10, NULL);
    //      DM_LOG("g_pkgSize = %u", g_pkgSize);
    //  }
}

/*****************************************************************************/
// 	Description :handle socket message
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_HandleSocketMessage(MMIDM_SOCKET_STATE type,int32 sock_id)
{
    //SCI_TRACE_LOW:"MMIDM==> VDM_HandleSocketMessage type 0x%x ,sock_id = 0x%x dm=0x%x,dl=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1553_112_2_18_2_14_11_532,(uint8*)"dddd", type,sock_id,s_dm_Socket_Id,s_dl_Socket_Id);
    switch(type)
    {
        case SOCKET_STATE_READ:
            //SCI_TRACE_LOW:"MMIDM==> SOCKET_STATE_READ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1557_112_2_18_2_14_11_533,(uint8*)"");
            if (sock_id == s_dm_Socket_Id)
            {
                RDM_COMMS_Data_Arrive(sock_id);
            }
            else if (sock_id == s_dl_Socket_Id)
            {
#ifdef DM_SUPPORT_REDBENCH               
                s_dl_observer->notifyTransport(s_dl_observer->context,VDM_SessionType_DL);
#endif                
            }
            break;
        case SOCKET_STATE_WRITE:
            //SCI_TRACE_LOW:"MMIDM==> SOCKET_STATE_WRITE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1570_112_2_18_2_14_11_534,(uint8*)"");
            if (sock_id ==s_dm_Socket_Id)
            {
#ifdef DM_SUPPORT_REDBENCH               
                s_dm_observer->notifyTransport(s_dm_observer->context,VDM_SessionType_DM);
#endif                

                //SCI_TRACE_LOW:"begin to write xml"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1577_112_2_18_2_14_11_535,(uint8*)"");
            }
            else if (sock_id ==s_dl_Socket_Id)
            {
#ifdef DM_SUPPORT_REDBENCH               
                s_dl_observer->notifyTransport(s_dl_observer->context,VDM_SessionType_DL);
#endif                
            }
            break;
        case SOCKET_STATE_CONNECT:
            //SCI_TRACE_LOW:"MMIDM==> SOCKET_STATE_CONNECT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1587_112_2_18_2_14_11_536,(uint8*)"");
            if (sock_id ==s_dm_Socket_Id)
            {
#ifdef DM_SUPPORT_REDBENCH
                s_dm_observer->notifyCommOpen(s_dm_observer->context,VDM_SessionType_DM);
#elif defined(DM_SUPPORT_DFRESH)               
                if(DM_CANCEL != MMIDM_GetDmState())
                {
                    //SCI_TRACE_LOW:"MMIDM_HandleSocketMessage DM_CANCEL == MMIDM_GetDmState()"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1595_112_2_18_2_14_11_537,(uint8*)"");
                     MMIDM_SendSigToDmTask(DM_TASK_RUN_MESSAGE,MMIDM_GetDmTaskID(),PNULL);
                }   
#endif                            
                

            }
            else if (sock_id ==s_dl_Socket_Id)
            {
#ifdef DM_SUPPORT_REDBENCH               
                s_dl_observer->notifyCommOpen(s_dl_observer->context,VDM_SessionType_DL);
#endif                
            }

            break;
        case SOCKET_STATE_CLOSE:
            //SCI_TRACE_LOW:"MMIDM==> SOCKET_STATE_CLOSE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1611_112_2_18_2_14_11_538,(uint8*)"");
            if (sock_id ==s_dm_Socket_Id)
            {
#ifdef DM_SUPPORT_REDBENCH               
                s_dm_observer->notifyCommBroken(s_dm_observer->context,VDM_SessionType_DM,FALSE);
#endif                
            }
            else if (sock_id ==s_dl_Socket_Id)
            {
#ifdef DM_SUPPORT_REDBENCH                
                s_dl_observer->notifyCommBroken(s_dl_observer->context,VDM_SessionType_DL,FALSE);
#endif                
            }
            break;
        default:
            break;
    }
    
}
/*********************************************************************************
//  Description : get dec number
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
LOCAL IU32 GetDecNumber(UTF8CStr str)
{
	char     c;
	UTF8CStr t;
	IU32     val;
	IBOOL    ok;


    if (PNULL == str)
    {
        return 0;
    }
	/* skip space and tab char */
	while ((*str == ' ') || (*str == '\t'))
		str++;
	/* Now find the next space char if exist */
	for (t = str; *t && (*t != ' ') && (*t != '\t') && (*t != '\r') && (*t != '\n'); t++)
		;
	/* temporarily make it NULL and do atoiu32 */
	c = *t;
	*((char*)t) = '\0';
	val = VDM_PL_atoIU32((const char*)str, 10, &ok);
	*((char*)t) = c;
	return ok ? val : 0;
}

LOCAL int RDM_isWholeMsgArrived(void)
{
    UTF8CStr t1 = PNULL; 
    UTF8CStr t2 = PNULL;
	IU32     contentLen = 0;
    char*   recvBuffer = s_recvBuffer_ptr;
    uint32  inMsgPos = s_recvpos;
    if (PNULL == recvBuffer)
    {
        return 1;
    }
	//SCI_TRACE_LOW:"MMIDM==> RDM_isWholeMsgArrived: starting (0x%x, %d)\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1672_112_2_18_2_14_11_539,(uint8*)"dd", recvBuffer, inMsgPos);
	recvBuffer[inMsgPos] = 0;
	if (!(t1 = FindSubString((UTF8CStr)recvBuffer, (UTF8CStr)"\r\n\r\n")))/*lint !e820 */		/* header not full yet */
		return 0;
	//SCI_TRACE_LOW:"MMIDM==> RDM_isWholeMsgArrived: found EOM (0x%x)\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1676_112_2_18_2_14_11_540,(uint8*)"d", t1);

	/* find content length if available; make sure it is inside the header!!! */
	if ((t2 = FindSubString((UTF8CStr)recvBuffer, (UTF8CStr)"Content-length:")) && (t2 < t1)) /*lint !e820 */
    {
		contentLen = GetDecNumber(t2+15);
		//SCI_TRACE_LOW:"MMIDM==> RDM_isWholeMsgArrived: found ContentLength (%d)\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1682_112_2_18_2_14_11_541,(uint8*)"d", contentLen);
	}
	else {
		//SCI_TRACE_LOW:"MMIDM==> RDM_isWholeMsgArrived: no ContentLength\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1685_112_2_18_2_14_11_542,(uint8*)"");
		return 1;
	}

	if ((t1-(UTF8CStr)recvBuffer)+4+contentLen > inMsgPos)/*lint !e737 */
    {
        SCI_TRACE_LOW("MMIDM==> RDM_isWholeMsgArrived: (t1-(UTF8CStr)recvBuffer)+4+contentLen %d", (t1-(UTF8CStr)recvBuffer)+4+contentLen);/*lint !e737*/
        //SCI_TRACE_LOW:"MMIDM==> RDM_isWholeMsgArrived: inMsgPos %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1691_112_2_18_2_14_11_543,(uint8*)"d", inMsgPos);
        return 0;
    }

	//SCI_TRACE_LOW:"MMIDM==> RDM_isWholeMsgArrived: found Whole message\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1695_112_2_18_2_14_11_544,(uint8*)"");
	return 1;
}

LOCAL void RDM_COMMS_Data_Arrive(uint32 socket_id)
{
#ifdef DM_SUPPORT_REDBENCH
    VDM_CommObserver_t* observer = NULL;
    uint32   type = 0;
#elif defined(DM_SUPPORT_DFRESH)
     unsigned char*    buf=PNULL;
     int      read_len= 0;
    int connid = -1;
#endif      

#ifdef DM_SUPPORT_DFRESH  
    buf = SCI_ALLOC_APP(9000);
    if(PNULL == buf)
    {
        //SCI_TRACE_LOW:"RDM_COMMS_Data_Arrive PNULL == buf"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1714_112_2_18_2_14_11_545,(uint8*)"");
        SCI_Free(buf);
        buf =PNULL;
        return;
    }
    memset(buf, 0, 9000);
#endif    
	//SCI_TRACE_LOW:"MMIDM==> RDM_COMMS_Data_Arrive socket_id=0x%x\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1721_112_2_18_2_14_11_546,(uint8*)"d",socket_id);
 
    s_recvActualLen = sci_sock_recv(socket_id, (char*)s_recvBuffer_ptr+s_recvpos, RDM_SOCKET_DM_RECEV_BUFFER_LEN-s_recvpos,0);
    //SCI_TRACE_LOW:"MMIDM==> RDM_COMMS_Data_Arrive s_recvActualLen=%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1724_112_2_18_2_14_11_547,(uint8*)"d",s_recvActualLen);
#ifdef DM_SUPPORT_REDBENCH
    if ((int32)socket_id ==s_dm_Socket_Id)
    {
        observer = s_dm_observer;
        type = VDM_SessionType_DM;
    }
    else
    {
        observer = s_dl_observer;
        type = VDM_SessionType_DL;
    }
#endif    
    if (s_recvActualLen > 0 && s_recvActualLen != 0xffffffff)
    {
		s_recvpos += s_recvActualLen;
		s_recvActualLen = s_recvpos;
		if (RDM_isWholeMsgArrived()) 
		{
			s_recvpos = 0;			                   
#ifdef DM_SUPPORT_REDBENCH
            observer->notifyTransport(observer->context, type);
#elif defined(DM_SUPPORT_DFRESH)
            //SCI_TRACE_LOW:"RDM_COMMS_Data_Arrive the receiver content"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1747_112_2_18_2_14_11_548,(uint8*)"");
            MMIDM_PrintTrace(s_recvBuffer_ptr, s_recvActualLen);
            VDM_Comm_PL_HTTP_receive_dfresh(PNULL, connid, buf, 9000, &read_len);
#endif            
	    }
	} 
#ifdef DM_SUPPORT_DFRESH	
	SCI_Free(buf);
    buf = PNULL; 
#endif    
}

/*********************************************************************************
//  Description : to lower
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
LOCAL char toLower(char c)
{
	if ((c >= 'A') && (c <= 'Z'))
    {
        c = (c - 'A') + 'a';
    }
	return c;
}
/*********************************************************************************
//  Description : find next lower char
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
LOCAL UTF8CStr FindNextLowerChar(UTF8CStr str, char ch)
{
    if (PNULL == str)
    {
        return NULL;
    }
	for (; *str; str++)
    {
        if (toLower(*str) == ch)
        {
            return str;
        }
    }
	return NULL;
}
/*********************************************************************************
//  Description : find sub string
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
LOCAL UTF8CStr FindSubString(UTF8CStr str, UTF8CStr subStr)
{
	UTF8CStr   curr, next;
    const char fc  = toLower(subStr[0]);
	IU32       len = 0;

    if (PNULL == subStr)
    {
        return NULL;
    }

	len = VDM_PL_strlen((char*)subStr);

	for (curr = str; ; curr = next+1)
    {
        next = FindNextLowerChar(curr, fc);
		if (0 == VDM_PL_strncasecmp((const char*) next, (const char*)subStr, len))
        {
            return next;
        }
	}
	//return NULL;
}
/*********************************************************************************
//  Description : broken dm session
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
PUBLIC void MMIDM_BrokenDMSession(void)
{
#ifdef DM_SUPPORT_REDBENCH   
    if (PNULL != s_dl_observer )
    {
         //SCI_TRACE_LOW:"MMIDM==> MMIDM_BrokenDMSession dl socket session "
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1830_112_2_18_2_14_11_549,(uint8*)"");
        s_dl_observer->notifyCommBroken(s_dl_observer->context,VDM_SessionType_DL,TRUE);
         MMIDM_SetBrokenType(DM_BROKEN_TYPE_DL);
     }
    else if (PNULL != s_dm_observer)
    {
         //SCI_TRACE_LOW:"MMIDM==> MMIDM_BrokenDMSession dm socket session "
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_PL_COMM_1836_112_2_18_2_14_11_550,(uint8*)"");
        s_dm_observer->notifyCommBroken(s_dm_observer->context,VDM_SessionType_DM,TRUE);
         MMIDM_SetBrokenType(DM_BROKEN_TYPE_DM);
     }
#endif
//     else
//     {
//         SCI_TRACE_LOW("MMIDM==> MMIDM_BrokenDMSession no socket session "); 
//         MMIDM_ExitDM();
//         MMIDM_SetBrokenType(DM_BROKEN_TYPE_NONE);   
//     }    
}
#endif
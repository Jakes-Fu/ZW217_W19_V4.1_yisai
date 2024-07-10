/****************************************************************************
** File Name:      MMIDM_buildXML.C                                               *
** Author:                                                                 *
** Date:           15/08/2012                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the MMI DM XML PARSE             *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2011       maryxiao       组建syncxml协议dm部分xml      *
****************************************************************************/
#define MMIBUILDXML_C

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmidm_app.h"
#include "mmi_id.h"
#include "mmidm_text.h"
//#include "mmi_appmsg.h"
#include "mmidm_internal.h"
//#include "mmidm_config.h"
#include "mmidm_parsexml.h"

extern DM_STATE_DATA_T g_state_data;
extern DM_PARSE_DATA_T g_parse_data;
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_STRCAT(char*des_buf, int buf_size, char* src_buf);
/*****************************************************************************/
//  Description :  根据tag产生tag的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_CreateTag(DMXML_TAG_T* tag_info, char* buf, uint16 buf_size, BOOL if_free);

/*****************************************************************************/
//  Description :  NONCE的加密
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_B64_Md5(char* outbuf, int buf_size, int len);
/*****************************************************************************/
//  Description :  产生鉴权字符串
//  Global resource dependence : b64(md5(b64(md5(name:pwd):nonce)))
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_GetCreddata(char* outbuf, uint16 buf_size);
/*****************************************************************************/
//  Description :  初始话和产生xml的header部分xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN InitDMXMLHeader(char* headerbuf, uint16 buf_size);



/*****************************************************************************/
//  Description : 根据alert节点内容产生alert部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildAlert(char* alert_buf, uint16 buf_size);
/*****************************************************************************/
//  Description : 根据source节点内容产生source部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildISource(DMXML_TAG_SOURCE_T source_info, char* source_buf, uint16 buf_size);
/*****************************************************************************/
//  Description : 根据meta节点内容产生meta部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildIMeta(DMXML_TAG_META_T meta_info, char* meta_buf, uint16 buf_size);
/*****************************************************************************/
//  Description : 根据item节点内容产生item部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildItem(DMXML_TAG_ITEM_T item_info, char* item_buf, uint16 buf_size);
/*****************************************************************************/
//  Description : 根据replace节点内容产生replace部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildReplace(char* replace_buf, uint16 buf_size);
/*****************************************************************************/
//  Description : 根据CHAL节点内容产生CHAL部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildIChal(DMXML_TAG_CHAL_T chal_info, char* chal_buf, int buf_size);
/*****************************************************************************/
//  Description : 根据status节点内容产生status部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildStatus(char* status_buf, uint16 buf_size);
/*****************************************************************************/
//  Description : 根据result节点内容产生result部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildResult(char* result_buf, uint16 buf_size);
/*****************************************************************************/
//  Description : generate the body of the xml msg
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_CodecXmlBody(char* bodybuf, uint16 buf_size);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_STRCAT(char*des_buf, int buf_size, char* src_buf)
{
	if(SCI_STRLEN(des_buf)+ SCI_STRLEN(src_buf) < buf_size) 
	{
		strcat(des_buf, src_buf);
		return TRUE;
	}
	else
	{
		SCI_TRACE_LOW("[MMIDM]MMIDM_STRCAT BUF SIZE SMALL buf_size des_buf %s src_buf %s",buf_size, des_buf, src_buf);
		return FALSE;
		
	}
}


/*****************************************************************************/
//  Description :  根据tag产生tag的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_CreateTag(DMXML_TAG_T* tag_info, char* buf, uint16 buf_size, BOOL if_free)
{
    uint16  size = 0;
    BOOLEAN ret = TRUE;
    do {  
        if(PNULL == buf || PNULL == tag_info)
        {
            SCI_TRACE_LOW("MMIDM_CreateTag buf %0x, tag_info %0x", buf, tag_info);
           ret = FALSE;
           break;
        }
    
        SCI_MEMSET(buf, 0, buf_size);
    
        SCI_STRCPY(buf,(char*)"<");       
        if(!MMIDM_STRCAT(buf , buf_size, MMIDM_GetTagStr(tag_info->tagId)))
        {
        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
            ret = FALSE;
            break;        	
        }
        if(strlen(tag_info->tagArr))
        {
	        if(!MMIDM_STRCAT(buf , buf_size, " "))
	        {
	        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        }	
	        if(!MMIDM_STRCAT(buf , buf_size, tag_info->tagArr))
	        {
	        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        }			
        }
        if(!MMIDM_STRCAT(buf , buf_size,">"))
        {
        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
            ret = FALSE;
            break;        	
        }    

        if(tag_info->hasChildTag)
        {
	        if(!MMIDM_STRCAT(buf , buf_size,tag_info->tagContent))
	        {
	        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        } 			
        }
        else
        {
	        if(!MMIDM_STRCAT(buf , buf_size,"<![CDATA["))
	        {
	        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        } 	
	        if(!MMIDM_STRCAT(buf , buf_size,tag_info->tagContent))
	        {
	        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        }
	        if(!MMIDM_STRCAT(buf , buf_size,"]]>"))
	        {
	        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        }			
        }
        if(!MMIDM_STRCAT(buf , buf_size,"</"))
        {
        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
            ret = FALSE;
            break;        	
        }		
        if(!MMIDM_STRCAT(buf , buf_size,MMIDM_GetTagStr(tag_info->tagId)))
        {
        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
            ret = FALSE;
            break;        	
        }		
        if(!MMIDM_STRCAT(buf , buf_size,">"))
        {
        	SCI_TRACE_LOW("MMIDM_CreateTag bufsize is too small %d", buf_size);
            ret = FALSE;
            break;        	
        }		
        size = strlen(buf);
        buf[size]='\0';//Add NUL terminator
    
        if(PNULL!=tag_info->tagContent && if_free)
        {    
            SCI_FREE(tag_info->tagContent);
            tag_info->tagContent = PNULL;
        }
    }while(0);
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;
}  


/*****************************************************************************/
//  Description :  NONCE的加密
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_B64_Md5(char* outbuf, int buf_size, int len)
{
    char  buf[128]={0};
	int copy_len =0;
    
    if(PNULL == outbuf)
    {
        SCI_TRACE_LOW("MMIDM_GetCreddata PNULL == outbuf");
        return;
    }
   SCI_MEMCPY(buf, outbuf, len);
   SCI_MEMSET(outbuf, 0, buf_size);    

   // hs_calc_md5_cred(buf);
    mmidm_calc_md5_cred(buf); 
    mmidm_calc_b64_cred(buf,B64_CRED_LEN);  
	copy_len =MIN(buf_size, SCI_STRLEN(buf));/*lint !e666*/
    strncpy(outbuf,buf, copy_len);
}
/*****************************************************************************/
//  Description :  产生鉴权字符串
//  Global resource dependence : b64(md5(b64(md5(name:pwd):nonce)))
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL void MMIDM_GetCreddata(char* outbuf, uint16 buf_size)
{
    char  nonce_buf[MAX_NONCE_LEN]={0};
    char  buf[DM_COMMON_BUFSIZE]={0};
//    char  temp[4]={0};
    long  len=0;
    long  len1=MAX_NONCE_LEN;
    long  len2=MAX_NONCE_LEN;
//    int i=0;
    
    if(PNULL == outbuf)
    {
        SCI_TRACE_LOW("MMIDM_GetCreddata PNULL == outbuf");
        return;
    }
    SCI_MEMSET(outbuf, 0, buf_size);

    sprintf(buf, "%s%s%s", "mvpdm",":","mvpdm"); 
    MMIDM_B64_Md5(buf, DM_COMMON_BUFSIZE, strlen(buf));
    SCI_TRACE_LOW("MMIDM_GetCreddata after MMIDM_B64_Md51 %s",buf);

    len = strlen(g_state_data.s_g_nonce);
    SCI_TRACE_LOW("[MMIDM] MMIDM_GetCreddata g_state_data.s_g_nonce %s", g_state_data.s_g_nonce);
    if(!len)
    {
         SCI_STRCPY(g_state_data.s_g_nonce,(char*)"JE48dmgiOyogPzxXPkdUJQ==");
         len = strlen(g_state_data.s_g_nonce);
    }
#if 0 //直接使用CFL BASE64的
    Base64_decode(g_state_data.s_g_nonce, len,nonce_buf, &len1); /*lint !e718 !e746 */
#else
    len1= CFLBASE64_Decode(nonce_buf, MAX_NONCE_LEN, g_state_data.s_g_nonce, len);/*lint !e718*/
#endif
    sprintf(outbuf, "%s%s", buf,":");
    SCI_MEMCPY(outbuf+strlen(outbuf), nonce_buf,len1)

    len2=len+1+len1;
    MMIDM_B64_Md5(outbuf, buf_size, len2);
}

/*****************************************************************************/
//  Description :  初始话和产生xml的header部分xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN InitDMXMLHeader(char* headerbuf, uint16 buf_size)
{
    char* buf=PNULL;
    char* buf2=PNULL;
    char str[64] ={0};
    char imei_str[64]={0};
    char creddata[DM_COMMON_BUFSIZE];
    DMXML_TAG_T * ptr = PNULL;
    BOOLEAN   ret = TRUE;

 do {
 	  
        if(PNULL == headerbuf)
        {
           SCI_TRACE_LOW("[MMIDM]InitDMXMLHeader PNULL == headerbuf"); 
           ret =FALSE;
           break;
        }
        SCI_MEMSET(headerbuf, 0, buf_size);
    
        buf = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("[MMIDM] InitDMXMLHeader PNULL == buf");
            ret =FALSE;
            break;
        } 
        buf2 = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf2)
        {
            SCI_TRACE_LOW("[MMIDM] InitDMXMLHeader PNULL == buf2");
            ret =FALSE;
            break;
        }
        ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
        if(PNULL== ptr)
        {
            SCI_TRACE_LOW("[MMIDM] InitDMXMLHeader PNULL== ptr");
            ret =FALSE;
            break;
        }

        if(MMIDM_ConstrucTag(ptr,TAG_VERDTD,PNULL, DM_VERDTD_VER, FALSE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            MMIDM_STRCAT(headerbuf,buf_size, buf);
        }
        else
        {
            ret = FALSE;
            break;
        }//VERDTD
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_VERPROTO,PNULL, DM_VERPROTO_VER, FALSE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            MMIDM_STRCAT(headerbuf,buf_size, buf);
        }
        else
        {
            ret = FALSE;
            break;
        }//VERPROTO
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        MMIDM_IU32toa(g_state_data.s_g_SessionID, str, 32);
        if(MMIDM_ConstrucTag(ptr,TAG_SESSIONID,PNULL, str, FALSE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            MMIDM_STRCAT(headerbuf,buf_size, buf);
        }
        else
        {
            ret = FALSE;
            break;
        }//SESSIONID
        MMIDM_IU32toa(g_state_data.s_g_MsgID, str, 32);
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_MSGID,PNULL, str, FALSE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            MMIDM_STRCAT(headerbuf,buf_size, buf);
        }
        else
        {
            ret = FALSE;
            break;
        }//msgid
        
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_LOCURIID,PNULL, MMIDM_getResUri(), FALSE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
        }
        else
        {
            ret = FALSE;
            break;
        }//LOCURI
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_TARGETID,PNULL, buf, TRUE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            MMIDM_STRCAT(headerbuf,buf_size, buf);
        }
        else
        {
            ret = FALSE;
            break;
        }//TARGETID
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        MMIDM_GetDmParaInfo(DM_IMEI_SET, imei_str);
        SCI_STRCPY(str, "IMEI:");/*lint !e779*/
        MMIDM_STRCAT(str, 64, imei_str);
        if(MMIDM_ConstrucTag(ptr,TAG_LOCURIID,PNULL, str, FALSE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
        }
        else
        {
            ret = FALSE;
            break;
        }//locuri
   	 	if(g_state_data.s_g_step != STEP_CREDED)
    	{
            SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
            if(MMIDM_ConstrucTag(ptr,TAG_LOCNAMEID,PNULL, DM_LOCNAME, FALSE))
            {
                MMIDM_CreateTag(ptr, buf2, MAX_TAG_BUF_SIZE, TRUE);
                MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2);
            }          
   	 	}//LOCNAME
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_SOURCEID,PNULL, buf, TRUE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            MMIDM_STRCAT(headerbuf , buf_size,buf);
        }
        else
        {
            ret = FALSE;
            break;
        }//SOURCE
        if(g_state_data.s_g_step != STEP_CREDED)
        {
            SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
            if(MMIDM_ConstrucTag(ptr,TAG_FORMATID,ARR_METINFO, DM_CRED_FORMAT, FALSE))
            {
                MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            }
            else
            {
                ret = FALSE;
                break;
            }//CREDFORMAT
            SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
            if(MMIDM_ConstrucTag(ptr,TAG_TYPEID,ARR_METINFO, DM_CRED_TYPE, FALSE))
            {
                MMIDM_CreateTag(ptr, buf2, MAX_TAG_BUF_SIZE, TRUE);
                MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2);
            }
            else
            {
                ret = FALSE;
                break;
            }//CREDTYPE
            SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
            if(MMIDM_ConstrucTag(ptr,TAG_METAID,PNULL, buf, TRUE))
            {
                MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            }
            else
            {
                ret = FALSE;
                break;
            }//META
            SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
            MMIDM_GetCreddata(creddata, 128);
            if(MMIDM_ConstrucTag(ptr,TAG_DATAID,PNULL, creddata, FALSE))
            {
                MMIDM_CreateTag(ptr, buf2, MAX_TAG_BUF_SIZE, TRUE);
	            MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2);
            }
            else
            {
                ret = FALSE;
                break;
            }//DATA

            SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
            if(MMIDM_ConstrucTag(ptr,TAG_CREDID,PNULL, buf, TRUE))
            {
                MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
                MMIDM_STRCAT(headerbuf , buf_size,buf);
            }
            else
            {
                ret = FALSE;
                break;
            }//CRED
        }
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_MAXMSGID,ARR_METINFO, DM_MAX_MSGID, FALSE))
        {
             MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
        }
        else
        {
            ret = FALSE;
            break;
        }//MSXMSGID
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_MAXOBJID,ARR_METINFO, DM_MAXOBJ_SIZE, FALSE))
        {
             MMIDM_CreateTag(ptr, buf2, MAX_TAG_BUF_SIZE, TRUE);
	         MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2);
        }
        else
        {
            ret = FALSE;
            break;
        }//MAXOBJ
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_METAID,PNULL, buf, TRUE))
        {
            MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
	        MMIDM_STRCAT(headerbuf , buf_size,buf);
        }
        else
        {
            ret = FALSE;
            break;
        }//META
        SCI_MEMSET(ptr, 0X0, sizeof(DMXML_TAG_T));
        if(MMIDM_ConstrucTag(ptr,TAG_SYNCHDRID,PNULL, headerbuf, TRUE))
        {
            MMIDM_CreateTag(ptr, headerbuf, MAX_XML_BUF_SIZE, TRUE);
        }
        else
        {
            ret = FALSE;
            break;
        }//SYNCHDR
        
  } while(0);  
    if(PNULL!=ptr)
    {    
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(PNULL!=buf)
    {
        SCI_FREE(buf);
        buf =PNULL;
    }
    if(PNULL!=buf2)
    {
        SCI_FREE(buf2);
        buf2 =PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;
}


/*****************************************************************************/
//  Description : 根据alert节点内容产生alert部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildAlert(char* alert_buf, uint16 buf_size)
{

    char* buf=PNULL;
    char* buf2=PNULL;
    DMXML_TAG_ALERT_T* cur_tag = PNULL;
    DMXML_TAG_T * ptr = PNULL;
    BOOLEAN ret = TRUE;

    do{  
        if(PNULL == alert_buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildAlert PNULL == alert_buf");
            ret = FALSE;
            break;
        }
        buf = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildItem PNULL == buf");
            ret = FALSE;
            break;
        } 
        buf2 = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf2)
        {
            SCI_TRACE_LOW("MMIDM_BuildItem PNULL == buf2");
            ret = FALSE;
            break;
        } 
        ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
        if(PNULL== ptr)
        {
            SCI_TRACE_LOW("MMIDM_BuildAlert PNULL== ptr");
            ret = FALSE;
            break;
        }
    
        for(cur_tag=g_parse_data.s_alerTag_head;cur_tag!=PNULL;cur_tag = cur_tag->next)
        {
            if(cur_tag->CmdId.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->CmdId), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildAlert bufsize is too small %d", buf_size);
		            ret = FALSE;
		            break;        	
		        }  
            }
            if(cur_tag->DATA.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->DATA), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildAlert bufsize is too small %d", buf_size);
		            ret = FALSE;
		            break;        	
		        }  
            }
            if(strlen(buf))
            {
                if(MMIDM_ConstrucTag(ptr,TAG_ALERTID,PNULL, buf, TRUE))
                {
                    MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
		            if(!MMIDM_STRCAT(alert_buf , buf_size,buf))
		            {
		        	    SCI_TRACE_LOW("[MMIDM] MMIDM_BuildAlert bufsize is too small %d", buf_size);
		                ret = FALSE;
		                break;        	
		            } 
                }
                else
                {
                    ret = FALSE;
                    break;
                }//SYNCHDR
            }
        }
    }while(0);
        
    while(PNULL!=g_parse_data.s_alerTag_head)
    {
        cur_tag = g_parse_data.s_alerTag_head->next;
        SCI_FREE(g_parse_data.s_alerTag_head);
        g_parse_data.s_alerTag_head= cur_tag;
    }
    if(PNULL!=ptr)
    {
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(PNULL!=buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }
    if(PNULL!=buf2)
    {
        SCI_FREE(buf2);
        buf2 = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;
}
/*****************************************************************************/
//  Description : 根据source节点内容产生source部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildISource(DMXML_TAG_SOURCE_T source_info, char* source_buf, uint16 buf_size)
{
    char* buf=PNULL;
    char* buf2=PNULL;
    DMXML_TAG_T * ptr = PNULL;
    BOOLEAN    ret = TRUE;
    
    do {

        if(PNULL == source_buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildISource PNULL == source_buf");
            ret = FALSE;
            break;
        }
        SCI_MEMSET(source_buf, 0X0, buf_size);
        buf = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildISource PNULL == buf");
            ret = FALSE;
            break;
        } 
        buf2 = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf2)
        {
            SCI_TRACE_LOW("MMIDM_BuildISource PNULL == buf2");
            ret = FALSE;
            break;
        } 
        if(source_info.locuri.tagContent)
        {
            MMIDM_CreateTag(&(source_info.locuri), buf2, MAX_TAG_BUF_SIZE, TRUE);
	        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildISource bufsize is too small");
	            ret = FALSE;
	            break;        	
	        } 
        }
        if(source_info.locname.tagContent)
        {
            MMIDM_CreateTag(&(source_info.locname), buf2, MAX_TAG_BUF_SIZE, TRUE);
	        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildISource bufsize is too small");
	            ret = FALSE;
	            break;        	
	        } 
        } 
        if(strlen(buf))
        {
            ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
            if(PNULL== ptr)
            {
                SCI_TRACE_LOW("MMIDM_BuildIsource PNULL== ptr");
                ret = FALSE;
                break;
            }
            if(MMIDM_ConstrucTag(ptr,TAG_SOURCEID,PNULL, buf, TRUE))
            {
                MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            }
            else
            {
                ret = FALSE;
                break;
            }//source
 	        if(!MMIDM_STRCAT(source_buf , buf_size,buf))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildISource bufsize is too small");
	            ret = FALSE;
	            break;        	
	        }            
           //SCI_STRCPY(source_buf, buf);

        }
    	
    } while(0);
    if(PNULL != buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }
    if(PNULL != buf2)
    {
        SCI_FREE(buf2);
        buf2 = PNULL;
    }
    if(PNULL != ptr)
    {    
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;
}
/*****************************************************************************/
//  Description : 根据meta节点内容产生meta部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildIMeta(DMXML_TAG_META_T meta_info, char* meta_buf, uint16 buf_size)
{
    char* buf=PNULL;
    char* buf2=PNULL;
    DMXML_TAG_T * ptr = PNULL;
    BOOLEAN ret   = TRUE;
    do{
        
        if(PNULL == meta_buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildIMeta PNULL == meta_buf"); 
            ret = FALSE;
            break;
        }
        SCI_MEMSET(meta_buf, 0, buf_size);
    
        buf = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildIMeta PNULL == buf");
            ret = FALSE;
            break;
        } 
    
        buf2 = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf2)
        {
            SCI_TRACE_LOW("MMIDM_BuildIMeta PNULL == buf2");
            ret = FALSE;
            break;
        } 

        if(meta_info.format.tagContent)
        {
            MMIDM_CreateTag(&(meta_info.format), buf2, MAX_TAG_BUF_SIZE, TRUE);
	        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildISource bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        } 

        }//formate
        if(meta_info.type.tagContent)
        {
            MMIDM_CreateTag(&(meta_info.type), buf2, MAX_TAG_BUF_SIZE, TRUE);
	        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildIMeta bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        } 
        } 
        if(meta_info.nextnonce.tagContent)
        {
            MMIDM_CreateTag(&(meta_info.nextnonce), buf2, MAX_TAG_BUF_SIZE, TRUE);
	        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildIMeta bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        } 

        }    
        if(strlen(buf))
        {
            ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
            if(PNULL== ptr)
            {
                SCI_TRACE_LOW("MMIDM_BuildIMeta PNULL== ptr");
                ret = FALSE;
                break;
            }   
            if(MMIDM_ConstrucTag(ptr,TAG_METAID,PNULL, buf, TRUE))
            {
                MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            }
            else
            {
                ret = FALSE;
                break;
            }//meta
 	        if(!MMIDM_STRCAT(meta_buf , buf_size,buf))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildISource bufsize is too small");
	            ret = FALSE;
	            break;        	
	        }   
        }
    }while(0);
        
    if(PNULL != buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }
    if(PNULL != buf2)
    {
        SCI_FREE(buf2);
        buf2 = PNULL;
    }
    if(PNULL != ptr)
    {    
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;
}


/*****************************************************************************/
//  Description : 根据item节点内容产生item部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildItem(DMXML_TAG_ITEM_T item_info, char* item_buf, uint16 buf_size)
{
    char* buf=PNULL;
    char* buf2 =PNULL;
    DMXML_TAG_T * ptr = PNULL;
    BOOLEAN  ret = TRUE;
    
    do {
        if(PNULL == item_buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildItem PNULL == item_buf"); 
            ret = FALSE;
            break;
        }
        SCI_MEMSET(item_buf, 0, buf_size);   
     
        buf = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildItem PNULL == buf");
            ret = FALSE;
            break;
        } 
        buf2 = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf2)
        {
            SCI_TRACE_LOW("MMIDM_BuildItem PNULL == buf2");
            ret = FALSE;
            break;
        } 
        
        MMIDM_BuildISource(item_info.source, buf, MAX_TAG_BUF_SIZE);
        MMIDM_BuildIMeta(item_info.meta, buf2, MAX_TAG_BUF_SIZE);
        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
        {
        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildItem bufsize is too small %d", buf_size);
            ret = FALSE;
            break;        	
        } 

        if(item_info.data.tagContent)
        {
            MMIDM_CreateTag(&(item_info.data), buf2, MAX_TAG_BUF_SIZE, TRUE);
	        if(!MMIDM_STRCAT(buf , MAX_TAG_BUF_SIZE,buf2))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildItem bufsize is too small %d", buf_size);
	            ret = FALSE;
	            break;        	
	        }
        }

        if(strlen(buf))
        {
            ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
            if(PNULL== ptr)
            {
                SCI_TRACE_LOW("MMIDM_BuildItem PNULL== ptr");
                ret = FALSE;
                break;
            }
            if(MMIDM_ConstrucTag(ptr,TAG_ITEMID,PNULL, buf, TRUE))
            {
                MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            }
            else
            {
                ret = FALSE;
                break;
            }//meta
 	        if(!MMIDM_STRCAT(item_buf , buf_size,buf))
	        {
	        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildISource bufsize is too small");
	            ret = FALSE;
	            break;        	
	        }
        }  	
    } while(0);
    if(PNULL != buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }
    if(PNULL != buf2)
    {
        SCI_FREE(buf2);
        buf2 = PNULL;
    }
    if(PNULL != ptr)
    {    
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;
}

/*****************************************************************************/
//  Description : 根据replace节点内容产生replace部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildReplace(char* replace_buf, uint16 buf_size)
{
     
    char *buf = PNULL;
    char* buf2 = PNULL;
    DMXML_TAG_REPLACE_T* cur_tag = PNULL;
    DMXML_TAG_ITEM_T*       item_tag = PNULL;
    DMXML_TAG_T * ptr = PNULL;
    char * replace_ptr = PNULL;
    BOOLEAN ret = TRUE;   
    
    do {
        if(PNULL == replace_buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildReplace PNULL == replace_buf");
            ret =FALSE;
            break;
        }
        replace_ptr = replace_buf;
        SCI_MEMSET(replace_ptr, 0, buf_size);
    
        buf = SCI_ALLOC_APPZ(MAX_XML_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildReplace PNULL == buf");
            ret =FALSE;
            break;
        }
    
        buf2 = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf2)
        {
            SCI_TRACE_LOW("MMIDM_BuildReplace PNULL == buf2");
            ret =FALSE;
            break;
        }
        ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
        if(PNULL== ptr)
        {
            SCI_TRACE_LOW("MMIDM_BuildReplace PNULL== ptr");
            ret =FALSE;
            break;
        }
        
        for(cur_tag=g_parse_data.s_replaceTag_head;cur_tag!=PNULL;cur_tag = cur_tag->next)
        {
            SCI_MEMSET(buf, 0, MAX_XML_BUF_SIZE);
            if(cur_tag->CmdId.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->CmdId), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildReplace bufsize is too small %d", buf_size);
		            ret = FALSE;
		            break;        	
		        }
            }
            for(item_tag = cur_tag->item_ptr; item_tag!= PNULL; item_tag=item_tag->next)
            {
                MMIDM_BuildItem(*item_tag, buf2, MAX_TAG_BUF_SIZE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildReplace bufsize is too small %d", buf_size);
		            ret = FALSE;
		            break;        	
		        }
            }
            if(strlen(buf))
            {
                SCI_MEMSET(ptr, 0, sizeof(DMXML_TAG_T));
                if(MMIDM_ConstrucTag(ptr,TAG_REPLACEID,PNULL, buf, TRUE))
                {
                    MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
                }
                else
                {
                    ret = FALSE;
                    break;
                }//replace
		        if(!MMIDM_STRCAT(replace_buf , buf_size,buf))
		        {
		        	SCI_TRACE_LOW("[MMIDM] MMIDM_BuildReplace replace_buf is too small %d", buf_size);
		            ret = FALSE;
		            break;        	
		        }
            }
        }

    	
    } while(0);


    
    while(PNULL!=g_parse_data.s_replaceTag_head)
    {
        while(PNULL!=g_parse_data.s_replaceTag_head->item_ptr)
        {
            item_tag = g_parse_data.s_replaceTag_head->item_ptr->next;
            SCI_FREE(g_parse_data.s_replaceTag_head->item_ptr);
            g_parse_data.s_replaceTag_head->item_ptr= item_tag;
        }
        cur_tag = g_parse_data.s_replaceTag_head->next;
        SCI_FREE(g_parse_data.s_replaceTag_head);
        g_parse_data.s_replaceTag_head= cur_tag;
    }
    
    if(PNULL != buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }
    if(PNULL != buf2)
    {
        SCI_FREE(buf2);
        buf2 = PNULL;
    }
    if(PNULL != ptr)
    {    
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;
    
}

/*****************************************************************************/
//  Description : 根据CHAL节点内容产生CHAL部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/

LOCAL BOOLEAN MMIDM_BuildIChal(DMXML_TAG_CHAL_T chal_info, char* chal_buf, int buf_size)
{
    char* buf=PNULL;
    DMXML_TAG_T * ptr = PNULL;
    BOOLEAN ret = TRUE;

    do {

        buf = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildISource PNULL == buf");
            ret = FALSE;
            break;
        } 
        
        MMIDM_BuildIMeta(chal_info.meta, buf, MAX_TAG_BUF_SIZE);
    
        if(strlen(buf))
        {
            ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
            if(PNULL== ptr)
            {
                SCI_TRACE_LOW("MMIDM_BuildIChal PNULL== ptr");
                ret = FALSE;
                break;
            }
            if(MMIDM_ConstrucTag(ptr,TAG_CHALID,PNULL, buf, TRUE))
            {
                MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
            }
            else
            {
                ret = FALSE;
                break;
            }//chal
		    if(!MMIDM_STRCAT(chal_buf , buf_size,buf))
		    {
		        SCI_TRACE_LOW("[MMIDM] MMIDM_BuildReplace replace_buf is too small %d", buf_size);
		        ret = FALSE;
		        break;        	
		    }
        }
    	
    } while(0);
    if(PNULL != buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }
    if(PNULL != ptr)
    {    
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;

}

/*****************************************************************************/
//  Description : 根据status节点内容产生status部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildStatus(char* status_buf, uint16 buf_size)
{
    char* buf = PNULL;
    char* buf2= PNULL;
    //unsigned char str[200] ={0};
    DMXML_TAG_STATUS_T* cur_tag = PNULL;
    DMXML_TAG_T * ptr = PNULL;
    BOOLEAN  ret = TRUE;
    int i = 0; 
    
    do {

        if(PNULL == status_buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildStatus PNULL == status_buf");
            ret = FALSE;
            break;
        }
        SCI_MEMSET(status_buf, 0, buf_size);

        buf = SCI_ALLOC_APPZ(MAX_XML_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildStatus PNULL == buf");
            ret = FALSE;
            break;
        }
    
        buf2 = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf2)
        {
            SCI_TRACE_LOW("MMIDM_BuildReplace PNULL == buf2");
            ret = FALSE;
            break;
        }
        ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
        if(PNULL== ptr)
        {
            SCI_TRACE_LOW("MMIDM_BuildStatus PNULL== ptr");
            ret = FALSE;
            break;
        }

        for(cur_tag=g_parse_data.s_statusTag_head;cur_tag!=PNULL;cur_tag = cur_tag->next)
        {
            SCI_MEMSET(buf, 0, MAX_XML_BUF_SIZE);
            if(cur_tag->CmdId.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->CmdId), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildStatus buf is too smal CmdId [%lu]", (strlen(buf)+strlen(buf2)));
		            ret = FALSE;
		            break;        	
		        }				
            }
            if(cur_tag->msgRef.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->msgRef), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildStatus buf is too smal msgRef [%lu]", (strlen(buf)+strlen(buf2)));
		            ret = FALSE;
		            break;        	
		        }				
            }
            if(cur_tag->cmdRef.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->cmdRef), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildStatus buf is too small cmdRef [%lu]", (strlen(buf)+strlen(buf2)));
		            ret = FALSE;
		            break;        	
		        }					
            }
            if(cur_tag->cmd.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->cmd), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildStatus buf is too small cmd [%lu]", (strlen(buf)+strlen(buf2)));
		            ret = FALSE;
		            break;        	
		        }					

            }
            for (i =0; i<DM_MAX_TARGETREF_NUM ; i++)
            {
             if(PNULL != cur_tag->targetRef[i].tagContent)
	            {
	                MMIDM_CreateTag(&(cur_tag->targetRef[i]), buf2, MAX_TAG_BUF_SIZE, TRUE);
		            if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		            {
			            SCI_TRACE_LOW("MMIDM_BuildStatus buf is too small");
			            ret = FALSE;
			            break;        	
		            }					
	            }
            }
            if(PNULL != cur_tag->sourceRef.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->sourceRef), buf2, MAX_TAG_BUF_SIZE, TRUE);
	            if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
	            {
		            SCI_TRACE_LOW("MMIDM_BuildStatus buf is too small sourceRef [%lu]", (strlen(buf)+strlen(buf2)));
		            ret = FALSE;
		            break;        	
	            }				

            }
	        SCI_MEMSET(buf2, 0, MAX_TAG_BUF_SIZE);
            MMIDM_BuildIChal(cur_tag->chal, buf2, MAX_TAG_BUF_SIZE);
	        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
	        {
	        	SCI_TRACE_LOW("MMIDM_BuildStatus buf is too small");
	            ret = FALSE;
	            break;        	
	        }			
            if(cur_tag->data.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->data), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildStatus buf is too small data [%lu]", (strlen(buf)+strlen(buf2)));
		            ret = FALSE;
		            break;        	
		        }				
            }
		    if(strlen(buf))
            {
                SCI_MEMSET(ptr, 0, sizeof(DMXML_TAG_T));
                if(MMIDM_ConstrucTag(ptr,TAG_STATUSID,PNULL, buf, TRUE))
                {
                    MMIDM_CreateTag(ptr, buf, MAX_TAG_BUF_SIZE, TRUE);
                }
                else
                {
                    ret = FALSE;
                    break;
                }//chal
		        if(!MMIDM_STRCAT(status_buf , buf_size,buf))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildStatus buf is too small data [%lu]", (strlen(buf)+strlen(buf2)));
		            ret = FALSE;
		            break;        	
		        }				
            }        
        }
    	
    } while(0);
    while(PNULL!=g_parse_data.s_statusTag_head)
    {
        cur_tag = g_parse_data.s_statusTag_head->next;
        SCI_FREE(g_parse_data.s_statusTag_head);
        g_parse_data.s_statusTag_head= cur_tag;
    }

    if(PNULL != buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }

    if(PNULL != buf2)
    {
        SCI_FREE(buf2);
        buf2 = PNULL;
    }

    if(PNULL != ptr)
    {    
        SCI_FREE(ptr);
        ptr = PNULL;
    }

    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }

    return ret;
}


/*****************************************************************************/
//  Description : 根据result节点内容产生result部分的xml
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_BuildResult(char* result_buf, uint16 buf_size)
{
    char* buf = PNULL;
    char* buf2=PNULL;
   // unsigned char str[200] ={0};
    DMXML_TAG_RESULT_T* cur_tag = PNULL;
    DMXML_TAG_T * ptr = PNULL;
    DMXML_TAG_ITEM_T*       item_tag = PNULL;
    BOOLEAN ret = TRUE;
    
    do {

        if(PNULL == result_buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildResult PNULL == result_buf");
            ret = FALSE;
            break;
        }
        SCI_MEMSET(result_buf, 0, buf_size);
    
        buf = SCI_ALLOC_APPZ(MAX_XML_BUF_SIZE);
        if(PNULL == buf)
        {
            SCI_TRACE_LOW("MMIDM_BuildResult  PNULL == buf");
            ret = FALSE;
            break;
        }
        buf2 = SCI_ALLOC_APPZ(MAX_TAG_BUF_SIZE);
        if(PNULL == buf2)
        {
            SCI_TRACE_LOW("MMIDM_BuildResult PNULL == buf2");
            ret = FALSE;
            break;
        }

        ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
        if(PNULL== ptr)
        {
            SCI_TRACE_LOW("MMIDM_BuildReplace PNULL== ptr");
            ret = FALSE;
            break;
        }

        for(cur_tag=g_parse_data.s_resultTag_head;cur_tag!=PNULL;cur_tag = cur_tag->next)
        {
            SCI_MEMSET(buf, 0, MAX_XML_BUF_SIZE);
            if(cur_tag->CmdId.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->CmdId), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildResult buf is too small CmdId [%lu]", (strlen(buf)+strlen(buf2)));
		            ret = FALSE;
		            break;        	
		        }					
            }
            if(cur_tag->msgRef.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->msgRef), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildResult buf is too small");
		            ret = FALSE;
		            break;        	
		        }				
            }			
            if(cur_tag->cmdRef.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->cmdRef), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildResult buf is too small");
		            ret = FALSE;
		            break;        	
		        }
            }
            if(cur_tag->cmd.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->cmd), buf2,MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildResult buf is too small");
		            ret = FALSE;
		            break;        	
		        }
            }
        
            for(item_tag = cur_tag->item_ptr; item_tag!= PNULL; item_tag=item_tag->next)
            {
                MMIDM_BuildItem(*item_tag, buf2, MAX_TAG_BUF_SIZE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildResult buf is too small");
		            ret = FALSE;
		            break;        	
		        }
            }

            if(cur_tag->sourceRef.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->sourceRef), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildResult buf is too small");
		            ret = FALSE;
		            break;        	
		        }
            }
            if(cur_tag->targetRef.tagContent)
            {
                MMIDM_CreateTag(&(cur_tag->targetRef), buf2, MAX_TAG_BUF_SIZE, TRUE);
		        if(!MMIDM_STRCAT(buf , MAX_XML_BUF_SIZE,buf2))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildResult buf is too small");
		            ret = FALSE;
		            break;        	
		        }
            }
            if(strlen(buf))
            {
                if(MMIDM_ConstrucTag(ptr,TAG_RESULTID,PNULL, buf, TRUE))
                {
                    MMIDM_CreateTag(ptr, buf, MAX_XML_BUF_SIZE, TRUE);
                }
                else
                {
                    ret = FALSE;
                    break;
                }
		        if(!MMIDM_STRCAT(result_buf , buf_size,buf))
		        {
		        	SCI_TRACE_LOW("MMIDM_BuildResult result_buf is too small");
		            ret = FALSE;
		            break;        	
		        }
            }
        }
    	
    } while(0);
    
    while(PNULL!=g_parse_data.s_resultTag_head)
    {
        while(PNULL!=g_parse_data.s_resultTag_head->item_ptr)
        {
            item_tag = g_parse_data.s_resultTag_head->item_ptr->next;
            SCI_FREE(g_parse_data.s_resultTag_head->item_ptr);
            g_parse_data.s_resultTag_head->item_ptr= item_tag;
        }
        cur_tag = g_parse_data.s_resultTag_head->next;
        SCI_FREE(g_parse_data.s_resultTag_head);
        g_parse_data.s_resultTag_head= cur_tag;
    }
    if(PNULL != buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }
    if(PNULL != buf2)
    {
        SCI_FREE(buf2);
        buf2 = PNULL;
    }
    if(PNULL != ptr)
    {    
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;
}



/*****************************************************************************/
//  Description : generate the body of the xml msg
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CODEC_InitXmlBody(char* bodybuf, uint16 buf_size, char* buf)
{
    char str[50] ={0};
    char str2[50] ={0};
    DMXML_TAG_ALERT_T* alert_tag = PNULL;
    DMXML_TAG_REPLACE_T*    replace_tag = PNULL;
    DMXML_TAG_ITEM_T*    item_tag = PNULL;
    DMXML_TAG_ITEM_T*    item_tag_tail = PNULL;
    BOOLEAN ret =TRUE;
    do {
        alert_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ALERT_T)) ;
        if(PNULL== alert_tag)
        {
            SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== alert_tag");
            ret = FALSE;
            break;
        }

        MMIDM_BuildStatus(buf, MAX_XML_BUF_SIZE);
	    if(!MMIDM_STRCAT(bodybuf , buf_size,buf))
	    {
	        SCI_TRACE_LOW("MMIDM_CodecXmlBody bodybuf is too small");
	        ret = FALSE;
	        break;        	
	    }//STATUS			

        MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
        g_state_data.s_g_CmdID++;
        if(!MMIDM_ConstrucTag(&(alert_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
        {
            ret = FALSE;
            break;
        }//CMDID
        if(!MMIDM_ConstrucTag(&(alert_tag->DATA),TAG_DATAID,PNULL, CONNECT_DATA, FALSE))
        {
            ret = FALSE;
            break;
        }//DATA
        alert_tag->next = (DMXML_TAG_ALERT_T*)PNULL;
        g_parse_data.s_alerTag_head = alert_tag;
        MMIDM_BuildAlert(bodybuf, MAX_XML_BUF_SIZE);

        replace_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_REPLACE_T)) ;
        if(PNULL== replace_tag)
        {
            SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== replace_tag");
            ret = FALSE;
            break;
        }

        MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
        g_state_data.s_g_CmdID++;
        if(!MMIDM_ConstrucTag(&(replace_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
        {
            ret = FALSE;
            break;
        }//CMDID

        item_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ITEM_T)) ;
        if(PNULL== item_tag)
        {
            SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== item_tag");
            ret = FALSE;
            break;
        }

        if(!MMIDM_ConstrucTag(&(item_tag->source.locuri),TAG_LOCURIID,PNULL, DM_DEV_MOD, FALSE))
        {
            ret = FALSE;
            break;
        }//DM_DEV_MODE
        MMIDM_GetDmParaInfo(DM_MON_SET, str);
        if(!MMIDM_ConstrucTag(&(item_tag->data),TAG_DATAID,PNULL, str, FALSE))
        {
            ret = FALSE;
            break;
        }//DATA
        replace_tag->item_ptr = item_tag;
        item_tag_tail = item_tag;

        item_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ITEM_T));
        if(PNULL== item_tag)
        {
            SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== item_tag");
            ret = FALSE;
            break;
        }
        if(!MMIDM_ConstrucTag(&(item_tag->source.locuri),TAG_LOCURIID,PNULL, DM_DEV_MAN, FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_MAN_LOCRUI
        MMIDM_GetDmParaInfo(DM_MAN_SET, str);
        if(!MMIDM_ConstrucTag(&(item_tag->data),TAG_DATAID,PNULL, str, FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_MAN_DATA
        item_tag_tail->next = item_tag;
        item_tag_tail = item_tag_tail->next;


        item_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ITEM_T)) ;
        if(PNULL== item_tag)
        {
            SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== item_tag");
            ret = FALSE;
            break;
        }
        if(!MMIDM_ConstrucTag(&(item_tag->source.locuri),TAG_LOCURIID,PNULL, DM_DEV_LANG, FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_LANG_LOCRUI
        if(!MMIDM_ConstrucTag(&(item_tag->data),TAG_DATAID,PNULL, "en-US", FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_LANG_LOCRUI
        item_tag_tail->next = item_tag;
        item_tag_tail = item_tag_tail->next;

        item_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ITEM_T)) ;
        if(PNULL== item_tag)
        {
            SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== item_tag");
            ret = FALSE;
            break;
        }
        if(!MMIDM_ConstrucTag(&(item_tag->source.locuri),TAG_LOCURIID,PNULL, DM_DEV_VESION, FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_VESION_LOCRUI
        if(!MMIDM_ConstrucTag(&(item_tag->data),TAG_DATAID,PNULL, "1.2", FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_VESION_LOCRUI
        item_tag_tail->next = item_tag;
        item_tag_tail = item_tag_tail->next;

        item_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ITEM_T)) ;
        if(PNULL== item_tag)
        {
            SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== item_tag");
            ret = FALSE;
            break;
        }
        if(!MMIDM_ConstrucTag(&(item_tag->source.locuri),TAG_LOCURIID,PNULL, DM_DEV_DEVID, FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_DEVID_LOCRUI
        if(!MMIDM_ConstrucTag(&(item_tag->meta.format),TAG_FORMATID,ARR_METINFO, "chr", FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_DEVID_META_FORMAT
        MMIDM_GetDmParaInfo(DM_IMEI_SET, str2);
        SCI_STRCPY(str, (char*)"IMEI:");
	    if(!MMIDM_STRCAT(str , 50, str2))
	    {
	        ret = FALSE;
	        break;        	
	    }
        if(!MMIDM_ConstrucTag(&(item_tag->data),TAG_DATAID,PNULL, str, FALSE))
        {
            ret = FALSE;
            break;
        }//DEV_DEVID_DATA
        item_tag_tail->next = item_tag;
        item_tag_tail = item_tag_tail->next;
        item_tag_tail->next =PNULL;

        g_parse_data.s_replaceTag_head = replace_tag;
        MMIDM_BuildReplace(buf, MAX_XML_BUF_SIZE);

	    if(!MMIDM_STRCAT(bodybuf , buf_size,buf))
	    {
	        SCI_TRACE_LOW("MMIDM_BuildBODY bodybuf is too smal  %s", bodybuf);
	        ret = FALSE;
	        break;        	
	    }			
	    if(!MMIDM_STRCAT(bodybuf , buf_size,"<Final/>"))
	    {
	        SCI_TRACE_LOW("MMIDM_BuildBODY bodybuf is too smal  %s", bodybuf);
	        ret = FALSE;
	        break;        	
	    }
    }while(0);

    return ret;
}



/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CODEC_CrededXmlBody(char* bodybuf, uint16 buf_size, char* buf)
{
    BOOLEAN ret = TRUE;
    do{

        SCI_MEMSET(bodybuf, 0, MAX_XML_BUF_SIZE);
        MMIDM_BuildAlert(buf, MAX_XML_BUF_SIZE);
	    if(!MMIDM_STRCAT(bodybuf , buf_size,buf))
	    {
	        SCI_TRACE_LOW("MMIDM_BuildBODY bodybuf is too smal  %s", bodybuf);
	        ret = FALSE;
	        break;        	
	    }				

        SCI_MEMSET(buf, 0, MAX_XML_BUF_SIZE);
        MMIDM_BuildStatus(buf, MAX_XML_BUF_SIZE);
	    if(!MMIDM_STRCAT(bodybuf , buf_size,buf))
	    {
	        SCI_TRACE_LOW("MMIDM_BuildBODY bodybuf is too smal  %s", bodybuf);
	        ret = FALSE;
	        break;        	
	    }
        SCI_MEMSET(buf, 0, MAX_XML_BUF_SIZE);
        MMIDM_BuildReplace(buf, MAX_XML_BUF_SIZE);
	    if(!MMIDM_STRCAT(bodybuf , buf_size,buf))
	    {
	        SCI_TRACE_LOW("MMIDM_BuildBODY bodybuf is too smal  %s", bodybuf);
	        ret = FALSE;
	        break;        	
	    }
        SCI_MEMSET(buf, 0, MAX_XML_BUF_SIZE);
        MMIDM_BuildResult(buf, MAX_XML_BUF_SIZE);
	    if(!MMIDM_STRCAT(bodybuf , buf_size,buf))
	    {
	        SCI_TRACE_LOW("MMIDM_BuildBODY bodybuf is too smal  %s", bodybuf);
	        ret = FALSE;
	        break;        	
	    }
	    if(!MMIDM_STRCAT(bodybuf , buf_size,"<Final/>"))
	    {
	        SCI_TRACE_LOW("MMIDM_BuildBODY bodybuf is too smal  %s", bodybuf);
	        ret = FALSE;
	        break;        	
	    }
    }while(0);
    return ret;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CODEC_AbortXmlBody(char* bodybuf, uint16 buf_size, char* buf)
{
     DMXML_TAG_ALERT_T* alert_tag = PNULL;
     BOOLEAN ret = TRUE;
     char str[50] ={0};
     do{
           alert_tag = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_ALERT_T)) ;
            if(PNULL== alert_tag)
            {
                SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== alert_tag");
                ret = FALSE;
                break;
            }
            MMIDM_IU32toa(g_state_data.s_g_CmdID, str, 32);
            g_state_data.s_g_CmdID++;
            if(!MMIDM_ConstrucTag(&(alert_tag->CmdId),TAG_CMDIDID,PNULL, str, FALSE))
            {
                ret = FALSE;
                break;
            }//cmdid
            
            if(!MMIDM_ConstrucTag(&(alert_tag->DATA),TAG_DATAID,PNULL, EXIT_DATA, FALSE))
            {
                ret = FALSE;
                break;
            }//data

            alert_tag->next = (DMXML_TAG_ALERT_T*)PNULL;
            g_parse_data.s_alerTag_head = alert_tag;
            MMIDM_BuildAlert(bodybuf, MAX_XML_BUF_SIZE);  
	        if(!MMIDM_STRCAT(bodybuf , buf_size,"<Final/>"))
	        {
	        	SCI_TRACE_LOW("MMIDM_BuildBODY bodybuf is too smal");
	            ret = FALSE;
	            break;        	
	        }         
     }while(0);
     return ret;
}

/*****************************************************************************/
//  Description : generate the body of the xml msg
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIDM_CodecXmlBody(char* bodybuf, uint16 buf_size)
{
    char* buf =PNULL;
    DMXML_TAG_T* ptr = PNULL;
    BOOLEAN ret = TRUE;
    	
    if(PNULL == bodybuf)
    {
        SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL == bodybuf");
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
        return FALSE;
    }
    SCI_MEMSET(bodybuf, 0, buf_size);    

    buf = SCI_ALLOC_APPZ(MAX_XML_BUF_SIZE);
    if(PNULL == buf)/*lint !e831*/
    {
        SCI_TRACE_LOW("MMIDM_BuildResult  PNULL == buf");
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
        return FALSE;
    }

    if(g_state_data.s_g_step == STEP_GETNONCE || g_state_data.s_g_step == STEP_GETNONCE_AGAIN)
    {
	    ret = CODEC_InitXmlBody(bodybuf, buf_size, buf);
    }
    else if(g_state_data.s_g_step == STEP_CREDED)
    {	
	    ret = CODEC_CrededXmlBody(bodybuf, buf_size, buf);
    }
    else if(g_state_data.s_g_step == STEP_ABORT)
    {
	    ret = CODEC_AbortXmlBody(bodybuf, buf_size, buf);
    }    
    if(ret)
    {

        ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
        if(PNULL== ptr)
        {
            SCI_TRACE_LOW("MMIDM_CodecXmlBody PNULL== ptr");
           // if(PNULL != buf)
            {
                SCI_FREE(buf);
                buf = PNULL;
            }
            MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
            return FALSE;
        }
        if(!MMIDM_ConstrucTag(ptr,TAG_SYNCBODYID,PNULL, bodybuf, TRUE))
        {
           // if(PNULL != buf)
            {
                SCI_FREE(buf);
                buf = PNULL;
            }
            MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
            return FALSE;
        }//BODY
        MMIDM_CreateTag(ptr, bodybuf, MAX_XML_BUF_SIZE, TRUE);  
    }

    //if(PNULL != buf)
    {
        SCI_FREE(buf);
        buf = PNULL;
    }
    if(PNULL != ptr)
    {
        if(PNULL!=ptr->tagContent)
        {
            SCI_FREE(ptr->tagContent);
            ptr->tagContent = PNULL;
        }
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;

}


/*****************************************************************************/
//  Description : GET THE CONTENT OF THE TAG
//  Global resource dependence : 
//  Author:MARY.XIAO
//  xmlbuf: CONNTENT NEED TO PARSE  
//  tagid: THE FIRST TAG WHICH CONTENT WANT TO GET
//  content: OUT: THE TAG CONTENT
//  return:  return the left content 
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_generateXMLData(char* sendbuf, int buf_size)
{
    char *bodybuf=PNULL;
    char* sendContent=PNULL;
    char* head_ptr ="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    DMXML_TAG_T * ptr = PNULL;
    BOOLEAN  ret = TRUE;
    
    do {

        bodybuf = SCI_ALLOCA(MAX_XML_BUF_SIZE) ;
        if(PNULL== bodybuf)
        {
            SCI_TRACE_LOW("MMIDM_generateXMLData PNULL== bodybuf");
            ret = FALSE;
            break;
        }
         SCI_MEMSET(bodybuf, 0X0, MAX_XML_BUF_SIZE);
     
        sendContent = SCI_ALLOCA(MAX_XML_BUF_SIZE) ;
        if(PNULL== sendContent)
        {
            SCI_TRACE_LOW("MMIDM_generateXMLData PNULL== sendContent");
            ret = FALSE;
            break;
        }
         SCI_MEMSET(sendContent, 0X0, MAX_XML_BUF_SIZE);

        InitDMXMLHeader(sendContent, MAX_XML_BUF_SIZE); 

        MMIDM_CodecXmlBody(bodybuf, MAX_XML_BUF_SIZE);

        if(!MMIDM_STRCAT(sendContent , MAX_XML_BUF_SIZE,bodybuf))
        {
        	SCI_TRACE_LOW("MMIDM_generateXMLData sendContent is too smal");
            ret = FALSE;
            break;        	
        }
        ptr = SCI_ALLOC_APPZ(sizeof(DMXML_TAG_T));
        if(PNULL== ptr)
        {
            SCI_TRACE_LOW("MMIDM_generateXMLData PNULL== ptr");
            ret = FALSE;
            break;
        }   
        if(!MMIDM_ConstrucTag(ptr,TAG_SYNCMLID,ARR_SYNCML_VER, sendContent, TRUE))
        {
            ret = FALSE;
            break;
        }

        MMIDM_CreateTag(ptr, sendContent, MAX_XML_BUF_SIZE, TRUE);
        SCI_STRCPY(sendbuf, head_ptr);

        if(!MMIDM_STRCAT(sendbuf , buf_size,sendContent))
        {
        	SCI_TRACE_LOW("MMIDM_generateXMLData sendbuf is too smal");
            ret = FALSE;
            break;        	
        }		

    	
    } while(0);

    if(PNULL != sendContent)
    {
        SCI_FREE(sendContent);
        sendContent = PNULL;
    }
    if(PNULL != bodybuf)
    {
        SCI_FREE(bodybuf);
        bodybuf = PNULL;
    }
    if(PNULL != ptr)
    {    
        if(PNULL != ptr->tagContent)
        {
            SCI_FREE(ptr->tagContent);
            ptr->tagContent = PNULL;
        }
        SCI_FREE(ptr);
        ptr = PNULL;
    }
    if(!ret)
    {
        MMIDM_SendSigToDmTask(DM_TASK_DM_CLOSE,MMIDM_GetDmTaskID(),PNULL);
    }
    return ret;

}



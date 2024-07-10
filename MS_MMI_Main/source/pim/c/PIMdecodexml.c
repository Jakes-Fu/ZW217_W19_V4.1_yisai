/******************************************************************************
                                   海信WCDMA  自主业务模块
        由于3G  网络的逐渐成熟，大量的业务需求成为第三代
        移动终端的亮点，逐渐成为不可或缺的一部分。本
        模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现在多平台、多操作系统中的PIM业务。目标
        是要达到任意平台的可移植性良好的功能稳定的模块。
        本模块的实现分为三个步骤:
        1.实现Data sync功能
        2.实现Device Management功能
        3.实现多平台的平滑移植
        模块开发参考OMA成员共同开发的SRT_c。
        目前模块计划开发支持OMA syncML 协议1.1


        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:刘刚
        开发日期:2005年4月11日。
******************************************************************************/


/******************************************************************************
文件名称:PIMdecodexml.c
文件描述:xml文档解码主文件
文件说明:由codec.c调用，负责xml文档的解码。
开发人员:刘刚
首次开发日期:2005-4-11
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2004-4-11              刘刚                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/

  /*  包含头文件*/
#ifdef WIN32
   #include "std_header.h"
#endif
  #define _PIMDECODEXML_C_
  
#include "../h/PIMdecodexml.h"
#include "../h/PIMsmlerr.h"
#include "../h/PIMlibmem.h"
  /*  数据结构声明*/
  
  /*  全局变量声明*/

  /*  功能函数声明*/

  /**
 * Public methods of the scanner interface.
 *
 * Description see XLTDecCom.h.
 */
static short hs_destroy(XltDecScannerPtr_t);
static short hs_nextTok(const char* ,  int , XltDecScannerPtr_t); // chenxiang_memory 20080313
static short hs_pushTok(XltDecScannerPtr_t);
static void hs_setBuf(XltDecScannerPtr_t, unsigned char* , unsigned char*); //liugang delete the const symbol for reduce compiling warnings!
static unsigned char* hs_getPos(XltDecScannerPtr_t); 

/**
 * FUNCTION: hs_readBytes
 *
 * Advance the current position pointer after checking whether the end of
 * the buffer has been reached. If the end of the buffer has been reached
 * the scanner's finished flag is set.
 *
 * PRE-Condition:
 * POST-Condition:
 *
 * IN:             bytes, read this many bytes
 *
 * IN/OUT:         pScanner, the scanner
 *
 * RETURNS:        1, if end of buffer has not been reached
 *                 0 otherwise
 */
static unsigned char hs_readBytes(xmlScannerPrivPtr_t pScanner, long bytes);

/**
 * Skip whitespaces.
 */
static void hs_skipS(xmlScannerPrivPtr_t pScanner);

static short hs_xmlTag(xmlScannerPrivPtr_t pScanner, unsigned char endtag);
static short hs_xmlName(xmlScannerPrivPtr_t pScanner, char* *name);
static short hs_xmlCharData(xmlScannerPrivPtr_t pScanner);
static short hs_xmlProlog(xmlScannerPrivPtr_t pScanner);
static short hs_xmlDocTypeDecl(xmlScannerPrivPtr_t pScanner);
static short hs_xmlXMLDecl(xmlScannerPrivPtr_t pScanner);
static short hs_xmlAttribute(xmlScannerPrivPtr_t pScanner, char* *name, char* *value);
static short hs_xmlStringConst(xmlScannerPrivPtr_t pScanner, char* *value);

static short hs_xmlSkipPCDATA(const char * file,  int line, xmlScannerPrivPtr_t pScanner); // chenxiang_memory 20080313
static short hs_xmlSkipComment(xmlScannerPrivPtr_t pScanner);
static short hs_xmlSkipAttributes(xmlScannerPrivPtr_t pScanner);
static short hs_xmlSkipPI(xmlScannerPrivPtr_t pScanner);
static short hs_xmlCDATA(xmlScannerPrivPtr_t pScanner);
static unsigned char hs_isPcdata(XltTagID_t tagid);

  /*功能函数*/

/*************************************************************************/
/* External Functions                                                    */
/*************************************************************************/


short hs_xltDecXmlInit( unsigned char* pBufEnd, unsigned char* *ppBufStart, 
            XltDecScannerPtr_t *ppScanner)//liugang delete the const symbol for reduce compiling warnings! 
{
        xmlScannerPrivPtr_t pScanner;
        short rc;

        pScanner = (xmlScannerPrivPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(xmlScannerPriv_t));
        if (pScanner == NULL) {
 			*ppScanner = NULL;
 			return HS_SML_ERR_NOT_ENOUGH_SPACE;
 		}

        pScanner->finished = 0;
        pScanner->pos = *ppBufStart;
        pScanner->bufend = pBufEnd;
        pScanner->curtok = (XltDecTokenPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(XltDecToken_t));
        if (pScanner->curtok == NULL) {
 			hs_smlLibFree(pScanner);
 			*ppScanner = NULL;
 			return HS_SML_ERR_NOT_ENOUGH_SPACE;
 		}
        
        pScanner->curtok->pcdata = NULL;
        pScanner->curtok->tagid = TN_UNDEF;
        pScanner->pubID = 0;
        pScanner->pubIDStr = NULL;
        pScanner->charset = 0;
        pScanner->charsetStr = NULL;
	 pScanner->ext          = SML_EXT_UNDEFINED;
	 pScanner->prev_ext     = (SmlPcdataExtension_t)255;
	 pScanner->ext_tag      = TN_UNDEF;
	 pScanner->prev_ext_tag = TN_UNDEF;
	 pScanner->nsprelen     = 0;
	 pScanner->nsprefix     = NULL;


        /* point public/private methods to the right implementation */
        pScanner->nextTok = hs_nextTok;
        pScanner->destroy = hs_destroy;
        pScanner->pushTok = hs_pushTok;
        pScanner->setBuf = hs_setBuf;
        pScanner->getPos = hs_getPos;

        if ((rc = hs_xmlProlog(pScanner)) != HS_SML_ERR_OK) {
                hs_smlLibFree((pScanner->curtok));
                hs_smlLibFree(pScanner);
                *ppScanner = NULL;
                return rc;
        }

        *ppScanner = (XltDecScannerPtr_t)pScanner;

        return HS_SML_ERR_OK;
}

/**
 * FUNCTION: destroy
 * 
 * Free memory. Description see XltDecAll.h.
 */
static short hs_destroy(XltDecScannerPtr_t pScanner)
{
        xmlScannerPrivPtr_t pScannerPriv;

        if (pScanner == NULL)
            return HS_SML_ERR_OK;

        pScannerPriv = (xmlScannerPrivPtr_t)pScanner;
#if 0 /* chenxiang_memory 20080219 */
        if (NULL != pScannerPriv->curtok) 
        {
            hs_smlFreePcdata(__FILE__, __LINE__, pScannerPriv->curtok->pcdata);
            pScannerPriv->curtok->pcdata = NULL;
      //      hs_smlLibFree(pScannerPriv->curtok->start);
        }
#endif
    //SCI_TRACE_MID_PIM("MMIPIM chenxiang hs_destroy pcdata = %d start = %d", pScannerPriv->curtok->pcdata, pScannerPriv->curtok->start); // chenxiang_20080215
        hs_smlLibFree((pScannerPriv->curtok));
        hs_smlLibFree((pScannerPriv->charsetStr));
        hs_smlLibFree((pScannerPriv->pubIDStr));
        hs_smlLibFree(pScannerPriv);

        return HS_SML_ERR_OK;
}

/**
 * FUNCTION: nextTok
 *
 * Get next token. Description see XltDecAll.h.
 */
static short hs_nextTok(const char * file,  int line, XltDecScannerPtr_t pScanner) // chenxiang_memory 20080313
{
        xmlScannerPrivPtr_t pScannerPriv;
        short rc;
        pScannerPriv = (xmlScannerPrivPtr_t)pScanner;
        pScannerPriv->curtok->start = pScannerPriv->pos;
        hs_skipS(pScannerPriv);
        /* skip unsupported elements until we find a supported one */
        rc = 0;


        while (!rc) {
            if (hs_smlLibStrncmp((char*)pScannerPriv->pos, "<!--", 4) == 0) {
                rc = hs_xmlSkipComment(pScannerPriv);
            } else if (hs_smlLibStrncmp((char*)pScannerPriv->pos, "<?", 2) == 0) {
                rc = hs_xmlSkipPI(pScannerPriv);
            } else if (hs_smlLibStrncmp((char*)pScannerPriv->pos, "</", 2) == 0) {
                rc = hs_xmlTag(pScannerPriv, 1);
                break;
            } else if (hs_smlLibStrncmp((char*)pScannerPriv->pos, "<![CDATA[", 9) == 0) {
                rc = hs_xmlCDATA(pScannerPriv);
                break;
            } else if ((hs_isPcdata(pScannerPriv->curtok->tagid)) && (pScannerPriv->curtok->type != TOK_TAG_END)) {
                rc = hs_xmlSkipPCDATA(file, line, pScannerPriv); // chenxiang_memory 20080313
                break;
            } else if (hs_smlLibStrncmp((char*)pScannerPriv->pos, "<", 1) == 0) {
                rc = hs_xmlTag(pScannerPriv, 0);
                break;
            } else {
                rc = hs_xmlCharData(pScannerPriv);
                break;
            }
        }
        if (rc)
            return rc;

        return HS_SML_ERR_OK;
}

/**
 * FUNCTION: pushTok
 *
 * Reset the scanner to the starting position of the current token within
 * the buffer. Description see XltDecAll.h.
 */
static short hs_pushTok(XltDecScannerPtr_t pScanner)
{
        xmlScannerPrivPtr_t pScannerPriv;       

        pScannerPriv = (xmlScannerPrivPtr_t)pScanner;
        pScannerPriv->pos = pScannerPriv->curtok->start;

        /* invalidate curtok */
        /* T.K. Possible Error. pScannerPriv->curtok is of type XltDecToken_t NOT ...Ptr_t */
        // OrigLine:
        // hs_smlLibMemset(pScannerPriv->curtok, 0, sizeof(XltDecTokenPtr_t));
        pScannerPriv->curtok->type = (XltTokType_t)0;

        return HS_SML_ERR_OK;
}

/**
 * FUNCTION: setBuf
 *
 * Set the working buffer of the scanner.
 */
static void hs_setBuf(XltDecScannerPtr_t pScanner, unsigned char* pBufStart,
        unsigned char* pBufEnd)//liugang delete the const symbol for reduce compiling warnings!
{
    xmlScannerPrivPtr_t pScannerPriv = (xmlScannerPrivPtr_t)pScanner;
    pScannerPriv->pos = pBufStart;
    pScannerPriv->bufend = pBufEnd;
}

/**
 * FUNCTION: getPos
 *
 * Get the current position of the scanner within its working buffer.
 */
static unsigned char* hs_getPos(XltDecScannerPtr_t pScanner)
{
    return ((xmlScannerPrivPtr_t)pScanner)->pos;
}

/*************************************************************************/
/* Internal Functions                                                    */
/*************************************************************************/

/**
 * FUNCTION: hs_readBytes
 *
 * Advance the position pointer. Description see above.
 */
static unsigned char hs_readBytes(xmlScannerPrivPtr_t pScanner, long bytes)
{
        if (pScanner->pos + bytes > pScanner->bufend) {
                pScanner->finished = 1;
                return 0;
        }
        pScanner->pos += bytes;
        return 1;
}

/**
 * FUNCTION: hs_skipS
 *
 * Skip whitespace.
 */
static void hs_skipS(xmlScannerPrivPtr_t pScanner)
{
    for (;;) {
        switch (*pScanner->pos) {
            case  9: /* tab stop */
            case 10: /* line feed */
            case 13: /* carriage return */
            case 32: /* space */
                // %%% luz: 2001-07-03: added exit from loop if no more bytes
                if (!hs_readBytes(pScanner, 1)) return;
                break;
            default:
                return;
        }
    }
}

/**
 * FUNCTION: hs_xmlTag
 *
 * Handle XML Tags
 */
static short hs_xmlTag(xmlScannerPrivPtr_t pScanner, unsigned char endtag)
{
    short rc;
    char* name;
    char* attname=NULL;
    char* value=NULL;
    char* nsprefix=NULL;
    unsigned char nsprelen = 0;
    XltTagID_t tagid;
    SmlPcdataExtension_t ext;



    if (endtag) {
        if (!hs_readBytes(pScanner, 2))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
    } else {
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
    }

    if ((rc = hs_xmlName(pScanner, &name)) != HS_SML_ERR_OK)
    {
        if (rc != HS_SML_ERR_NOT_ENOUGH_SPACE)
        {
            return HS_SML_ERR_XLT_INVAL_XML_DOC;
        }
        else
        {
            return rc;
        }
        
    }

	ext = pScanner->ext;
	if (!endtag) {
		/* Namespaces can only be defined on start, never on endtags
		 * but we have to make sure we close a namespace on the corrsponding endtag.
		 * Thats why we a) only open a namespace when it differs from the previous one, and
		 * b) record the tag_id that opend the namespace so we can close it when the 
		 * corresponding endtag is reached.
		 */
               
        if ((rc = hs_xmlAttribute(pScanner, &attname, &value)) == HS_SML_ERR_OK)
        {
			if (hs_smlLibStrncmp(attname, "xmlns", 5) == 0) {
				/* Heureka we found a Namespace :-) */
				/* It's save to check attname[5] here, as it contains at least the terminating '\000' */
				if (attname[5] == ':') { /* we found a namespace prefixdefinition */
                    nsprelen = (unsigned char)hs_smlLibStrlen(&attname[6]);
					nsprefix = hs_smlLibMalloc(__FILE__, __LINE__, nsprelen+1);
                    if (nsprefix == NULL) {
                        hs_smlLibFree(attname);
                        hs_smlLibFree(value);
                        hs_smlLibFree(name);
                        return HS_SML_ERR_NOT_ENOUGH_SPACE;
                    }
					hs_smlLibStrcpy(nsprefix,&attname[6]);						
				}
				ext = hs_getExtByName(value);
				if (ext == SML_EXT_MAX) {/*lint !e650 !e774 */
					hs_smlLibFree(nsprefix); /* doesn't harm, even when empty */
					hs_smlLibFree(attname);
					hs_smlLibFree(value);
					hs_smlLibFree(name);
					return  HS_SML_ERR_XLT_INVALID_CODEPAGE;
				}
			} else {/*lint !e774*/
#if 0//del by wangxiaolin   2009.12.19  moidfy for warning
                if (rc == HS_SML_ERR_NOT_ENOUGH_SPACE){/*lint !e774*/
                    hs_smlLibFree(attname);
                    hs_smlLibFree(value);
                    hs_smlLibFree(name);
                    return HS_SML_ERR_NOT_ENOUGH_SPACE;
                }
                else
#endif
                    {
				/* we found an unknown attribute -> bail out */
				hs_smlLibFree(attname);
				hs_smlLibFree(value);
				/* nsprefix is empty here so we save us a function call */
				hs_smlLibFree(name);
				return HS_SML_ERR_XLT_INVAL_XML_DOC;
			}
		}
            
        }
        else if ( rc != HS_SML_ERR_XLT_MISSING_CONT)
        {
            /* hs_xmlAttribute returns an HS_SML_ERR_XLT_MISSING_CONT error when
             * no attribute was found. This is not an error, but everything else is.
             */
            hs_smlLibFree(value);
            hs_smlLibFree(name);
            return rc;
        }
        
        
	}


	if (pScanner->ext == ext) {
		/* no new Namespace found - lets proceed with the active one */

		/* first lets check wether a tag is in the right namespace, in case
		 * we are using namespaces with prefix notation ('mi:Format' instead of
		 * 'Format nsattr="..."). 
		 * If we are and the token is not in this namespace -> bail out
		 */
		if (pScanner->nsprelen > 0 && hs_smlLibStrlen(name) > pScanner->nsprelen+1) {
			if (name[pScanner->nsprelen] != ':' || hs_smlLibStrncmp(name,pScanner->nsprefix, pScanner->nsprelen) != 0) {
				hs_smlLibFree(name);
				hs_smlLibFree(attname);
				hs_smlLibFree(value);
				hs_smlLibFree(nsprefix);
				return HS_SML_ERR_XLT_NO_MATCHING_CODEPAGE;
			}
		}
		/* Strip off namespace prefixes and ':' to find the tag.
		 * If no prefix is defined (pScanner->nsprelen == 0) take the whole tagname.
		 */
		if (pScanner->nsprelen > 0)
 			rc = hs_getTagIDByStringAndExt(&name[0+pScanner->nsprelen+1], pScanner->ext, &tagid);
		else
			rc = hs_getTagIDByStringAndExt(name, pScanner->ext, &tagid);
	} else {
		/* we have a new Namespace */
		if (nsprelen > 0 && hs_smlLibStrlen(name) > nsprelen+1) {
			if (name[nsprelen] != ':' || hs_smlLibStrncmp(name,nsprefix, nsprelen) != 0) {
				hs_smlLibFree(name);
				hs_smlLibFree(attname);
				hs_smlLibFree(value);
				hs_smlLibFree(nsprefix);
				return HS_SML_ERR_XLT_NO_MATCHING_CODEPAGE;
			}
		}
		/* Strip off namespace prefixes and ':' to find the tag.
		 * If no prefix is defined (pScanner->nsprelen == 0) take the whole tagname.
		 */
		if (nsprelen > 0)
 			rc = hs_getTagIDByStringAndExt(&name[nsprelen+1], ext, &tagid);
		else
			rc = hs_getTagIDByStringAndExt(name, ext, &tagid);
	}
	/* free temporary buffers */
	hs_smlLibFree(name);
	hs_smlLibFree(attname);
	hs_smlLibFree(value);

	if ((tagid == TN_UNDEF) || (rc != HS_SML_ERR_OK)) {
		hs_smlLibFree(nsprefix);
	return rc;
	}

	/* remember the old extension including the corresponding start tag if we found a new one */
	if (ext != pScanner->ext) { /* namespace changed */
		pScanner->prev_ext     = pScanner->ext;     /* remember the old ext */
		pScanner->prev_ext_tag = pScanner->ext_tag; /* and the corresponding start tag */
		pScanner->ext          = ext;
		pScanner->ext_tag      = tagid;
		hs_smlLibFree((pScanner->nsprefix));
		pScanner->nsprefix    = nsprefix;
		pScanner->nsprelen    = nsprelen;
	}


	pScanner->curtok->tagid = tagid;
	pScanner->curtok->ext   = pScanner->ext;
    hs_skipS(pScanner);

    if (endtag) {
        /* found end tag */
        if (hs_smlLibStrncmp((char*)pScanner->pos, ">", 1) != 0)
            return HS_SML_ERR_XLT_INVAL_XML_DOC;
        pScanner->curtok->type = TOK_TAG_END;
        hs_readBytes(pScanner, 1);
    	/* in case of an endtag we might need to close the current CP */
		if (tagid == pScanner->ext_tag) {
			pScanner->ext_tag = pScanner->prev_ext_tag;
			pScanner->ext     = pScanner->prev_ext;
			pScanner->prev_ext     = SML_EXT_UNDEFINED;
			pScanner->prev_ext_tag = TN_UNDEF;
			pScanner->nsprelen    = 0;
			hs_smlLibFree((pScanner->nsprefix));
			pScanner->nsprefix    = NULL;
		}
    } else {
        /* Attributes are not supported in SyncML -> skip them*/
        if ((rc = hs_xmlSkipAttributes(pScanner)) != HS_SML_ERR_OK) return rc;

        if (hs_smlLibStrncmp((char*)pScanner->pos, "/>", 2) == 0) {
            /* found empty tag */
            pScanner->curtok->type = TOK_TAG_EMPTY;
            hs_readBytes(pScanner, 2);
        } else if (hs_smlLibStrncmp((char*)pScanner->pos, ">", 1) == 0) {
            pScanner->curtok->type = TOK_TAG_START;
            hs_readBytes(pScanner, 1);
        } else {
            return HS_SML_ERR_XLT_INVAL_XML_DOC;
        }
    }

    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_xmlName
 *
 * Handle Name Elements
 */
static short hs_xmlName(xmlScannerPrivPtr_t pScanner, char* *name)
{
    unsigned char* begin;
    char* tmp;
    int len;

    begin = pScanner->pos;
    while (((*pScanner->pos >= 'a') && (*pScanner->pos <= 'z')) ||
           ((*pScanner->pos >= 'A') && (*pScanner->pos <= 'Z')) ||
           ((*pScanner->pos >= '0') && (*pScanner->pos <= '9')) ||
           (*pScanner->pos == '.') || (*pScanner->pos == '-') ||
           (*pScanner->pos == '_') || (*pScanner->pos == ':'))
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        
    len = pScanner->pos - begin;
		/* T.K. bail out if len is zero without modifying name */
		if (len == 0) return HS_SML_ERR_OK;


    tmp = (char*)hs_smlLibMalloc(__FILE__, __LINE__, len + 1);
    if (tmp == NULL) {
        *name = NULL;
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    hs_smlLibMemset(tmp, 0, len + 1);
    hs_smlLibStrncpy(tmp, (char*)begin, len);
    *name = tmp;
    return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_xmlCharData
 *
 * Handle Pcdata character data content
 */
static short hs_xmlCharData(xmlScannerPrivPtr_t pScanner)
{
    SmlPcdataPtr_t pPCData;
    unsigned char* begin;
    int len;


    pPCData = (SmlPcdataPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlPcdata_t));
    if (pPCData == NULL)
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    pPCData->contentType = SML_PCDATA_UNDEFINED;
    pPCData->length = 0;
    pPCData->content = NULL;

    begin = pScanner->pos;
    
    if (*pScanner->pos >= *pScanner->bufend) {
        pPCData->content     = NULL;
        pPCData->contentType = SML_PCDATA_UNDEFINED;
        pPCData->extension   = SML_EXT_UNDEFINED;
        pPCData->length      = 0;
   	    pScanner->curtok->type = TOK_CONT;  
        pScanner->curtok->pcdata = pPCData; 
        //hs_smlLibFree(pPCData);
        return HS_SML_ERR_XLT_END_OF_BUFFER;
    }

    while (*pScanner->pos != '<') /* && (*pScanner->pos != '&') */
    {
      if (pScanner->pos >= pScanner->bufend)
      {
        hs_smlLibFree(pPCData);
        return HS_SML_ERR_XLT_INVAL_SYNCML_DOC;
      }
      if (!hs_readBytes(pScanner, 1)) {
          hs_smlLibFree(pPCData);
        return HS_SML_ERR_XLT_END_OF_BUFFER;
      }
	  
    }
    len = pScanner->pos - begin;
    pPCData->content = hs_smlLibMalloc(__FILE__, __LINE__, len + 1);
    if (pPCData->content == NULL){
        hs_smlLibFree(pPCData);
		return HS_SML_ERR_NOT_ENOUGH_SPACE;
	}
    hs_smlLibMemset(pPCData->content, 0, len + 1);
    hs_smlLibMemcpy(pPCData->content, begin, len);
    pPCData->contentType = SML_PCDATA_STRING;
    pPCData->length = len;

    pScanner->curtok->type = TOK_CONT;
    pScanner->curtok->pcdata = pPCData;

    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_xmlProlog
 *
 * Scan the XML prolog (might be empty...).
 */
static short hs_xmlProlog(xmlScannerPrivPtr_t pScanner)
{
    short rc;

    if (pScanner->pos + 5 > pScanner->bufend)
        return HS_SML_ERR_OK;
    if (hs_smlLibStrncmp((char*)pScanner->pos, "<?xml", 5) == 0)
        if ((rc = hs_xmlXMLDecl(pScanner)) != HS_SML_ERR_OK)
            return rc;

    hs_skipS(pScanner);

    while ((pScanner->pos + 4 <= pScanner->bufend) &&
           ((hs_smlLibStrncmp((char*)pScanner->pos, "<!--", 4) == 0) ||
            (hs_smlLibStrncmp((char*)pScanner->pos, "<?", 2) == 0))) {
        if (hs_smlLibStrncmp((char*)pScanner->pos, "<!--", 4) == 0)
            rc = hs_xmlSkipComment(pScanner);
        else
            rc = hs_xmlSkipPI(pScanner);
        if (rc != HS_SML_ERR_OK)
            return rc;
        hs_skipS(pScanner);
    }

    if ((pScanner->pos + 9 <= pScanner->bufend) &&
        (hs_smlLibStrncmp((char*)pScanner->pos, "<!DOCTYPE", 9) == 0))
        if ((rc = hs_xmlDocTypeDecl(pScanner)) != HS_SML_ERR_OK)
            return rc;

    hs_skipS(pScanner);

    return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_xmlDocTypeDecl
 *
 * Part of the Prolog scanning
 */
static short hs_xmlDocTypeDecl(xmlScannerPrivPtr_t pScanner)
{
    short rc;
    char* name = NULL;
    char* syslit = NULL;
    char* publit = NULL;

    hs_readBytes(pScanner, 9);
    hs_skipS(pScanner);
    if ((rc = hs_xmlName(pScanner, &name)) != HS_SML_ERR_OK) {
        hs_smlLibFree(name);
        return rc;
    }
    hs_skipS(pScanner);

    /* parse ExternalID */
    if ((pScanner->pos + 6 <= pScanner->bufend) &&
        (hs_smlLibStrncmp((char*)pScanner->pos, "SYSTEM", 6) == 0)) {
        hs_readBytes(pScanner, 6);
        hs_skipS(pScanner);
        if ((rc = hs_xmlStringConst(pScanner, &syslit)) != HS_SML_ERR_OK) {
            hs_smlLibFree(name);
            hs_smlLibFree(syslit);
            return rc;
        }
    } else if ((pScanner->pos + 6 <= pScanner->bufend) &&
         (hs_smlLibStrncmp((char*)pScanner->pos, "PUBLIC", 6) == 0)) {
        hs_readBytes(pScanner, 6);
        hs_skipS(pScanner);
        if ((rc = hs_xmlStringConst(pScanner, &publit)) != HS_SML_ERR_OK) {
            hs_smlLibFree(name);
            hs_smlLibFree(publit);
            return rc;
        }
        hs_skipS(pScanner);
        if ((rc = hs_xmlStringConst(pScanner, &syslit)) != HS_SML_ERR_OK) {
            hs_smlLibFree(name);
            hs_smlLibFree(syslit);
            hs_smlLibFree(publit);
            return rc;
        }
    }

    hs_smlLibFree(name); 
    hs_smlLibFree(syslit); 
    hs_smlLibFree(publit); 

    hs_skipS(pScanner);

    if (*pScanner->pos != '>')
        return HS_SML_ERR_XLT_INVAL_XML_DOC;
    hs_readBytes(pScanner, 1);

    return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_xmlXMLDecl
 *
 * Part of the Prolog scanning
 */
static short hs_xmlXMLDecl(xmlScannerPrivPtr_t pScanner)
{
    char* name;
    char* value;
    short rc;

    hs_readBytes(pScanner, 5);
    hs_skipS(pScanner);
    
    /* mandatory version info */
    if ((rc = hs_xmlAttribute(pScanner, &name, &value)) != HS_SML_ERR_OK)
        return rc;
    if (hs_smlLibStrcmp(name, "version") != 0) {
        hs_smlLibFree(name);
        hs_smlLibFree(value);
        return HS_SML_ERR_XLT_INVAL_XML_DOC;
    }
    hs_smlLibFree(name);
    hs_smlLibFree(value);

    hs_skipS(pScanner);

    /* optional attributes are encoding and standalone */
    while ((pScanner->pos + 2 <= pScanner->bufend) &&
        (hs_smlLibStrncmp((char*)pScanner->pos, "?>", 2) != 0)) {
        if ((rc = hs_xmlAttribute(pScanner, &name, &value)) != HS_SML_ERR_OK)
            return rc;
        hs_smlLibFree(name);
        hs_smlLibFree(value);
        hs_skipS(pScanner);
    }

    if (pScanner->pos + 2 > pScanner->bufend)
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    hs_readBytes(pScanner, 2);

    return HS_SML_ERR_OK;
}



/**
 * FUNCTION: hs_xmlAttribute
 *
 * Handle Attributes //function can be used if attributes get necessary
 */
static short hs_xmlAttribute(xmlScannerPrivPtr_t pScanner, char**name, char**value)
{
    short rc;
    
    hs_skipS(pScanner);

    if ((rc = hs_xmlName(pScanner, name)) != HS_SML_ERR_OK)
        return rc;

    hs_skipS(pScanner);

		/* no attributes found, because this tag has none -> bail out */
		if (*pScanner->pos == '>') { 
			return HS_SML_ERR_XLT_MISSING_CONT;
		}
		if (hs_smlLibStrncmp((char*)pScanner->pos, "/>", 2) == 0) {
			return HS_SML_ERR_XLT_MISSING_CONT;
		}

    if (*pScanner->pos != '=') {
        hs_smlLibFree((*name));
        *name = NULL;
        *value = NULL;
        return HS_SML_ERR_XLT_INVAL_XML_DOC;
    }
    hs_readBytes(pScanner, 1);

    hs_skipS(pScanner);

    if ((rc = hs_xmlStringConst(pScanner, value)) != HS_SML_ERR_OK) {
        hs_smlLibFree((*name));
        *name = NULL;
        *value = NULL;
        return rc;
    }
    
    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_xmlStringConst
 *
 * Handle Pcdata String Constants
 */
static short hs_xmlStringConst(xmlScannerPrivPtr_t pScanner, char* *value)
{
    char* end;
    int len;
    char del;

    if ((*pScanner->pos != '"') && (*pScanner->pos != '\'')) {
        *value = NULL;
        return HS_SML_ERR_XLT_INVAL_XML_DOC;
    }
    del = *pScanner->pos;
    hs_readBytes(pScanner, 1);
        
    if ((end = hs_smlLibStrchr((char*)pScanner->pos, del)) == NULL) {
        *value = NULL;
		
        return HS_SML_ERR_XLT_END_OF_BUFFER;
    }
    len = end - (char*)pScanner->pos;
    if ((*value = (char*)hs_smlLibMalloc(__FILE__, __LINE__, len + 1)) == NULL)
	{
       
		return HS_SML_ERR_NOT_ENOUGH_SPACE;
	}
    hs_smlLibMemset(*value, 0, len + 1);
    hs_smlLibStrncpy(*value, (char*)pScanner->pos, len);
    hs_readBytes(pScanner, len + 1);

    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_xmlSkipPCDATA
 *
 * Read over a Pcdata content
 */
static short hs_xmlSkipPCDATA(const char * file,  int line, xmlScannerPrivPtr_t pScanner) // chenxiang_memory 20080313
{
    SmlPcdataPtr_t pPCData;
    unsigned char* begin;
    int len;
    short rc;
    char* _tagString = NULL;
    char* _tagString2 = NULL;

		/* Check wether this PCData might contain a subdtd.
    ** We assume a Sub DTD starts with '<' as first char.
    ** If this char is present start further processing else
    ** take it as pure String data. If the scanning returns an
    ** error we reject the file, as '<' is not a valid char inside
    ** PCData elements.
		 */
    if (hs_smlLibStrncmp((char*)pScanner->pos, "<", 1) == 0) {
      rc = hs_xmlTag(pScanner, 0);
        return rc;
    }

    _tagString = hs_smlLibMalloc(__FILE__, __LINE__, XML_MAX_TAGLEN);
    if (_tagString == NULL) return HS_SML_ERR_NOT_ENOUGH_SPACE;
    if ((rc = hs_getTagString(pScanner->curtok->tagid, _tagString, pScanner->curtok->ext)) != HS_SML_ERR_OK)
    {
        hs_smlLibFree(_tagString);
        return rc;
    }
    
    _tagString2 = hs_smlLibMalloc(__FILE__, __LINE__, hs_smlLibStrlen(_tagString) + 4 + (pScanner->nsprelen +1));

    // build a end tag String to compate (e.g. </Meta>)
		// beware of possible namespace prefixes
    if (_tagString2 == NULL) 
    {
        hs_smlLibFree(_tagString);
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    
    _tagString2 = hs_smlLibStrcpy(_tagString2,"</");
		if (pScanner->nsprelen > 0) {
			_tagString2 = hs_smlLibStrcat(_tagString2,pScanner->nsprefix);
			_tagString2 = hs_smlLibStrcat(_tagString2,":");
		}
    _tagString2 = hs_smlLibStrcat(_tagString2,_tagString);
    _tagString2 = hs_smlLibStrcat(_tagString2,">");
    hs_smlLibFree(_tagString);

    pPCData = (SmlPcdataPtr_t)hs_smlLibMalloc(file, line, sizeof(SmlPcdata_t));
    
    
    
    if (pPCData == NULL)
    {
        hs_smlLibFree(_tagString2);
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    pPCData->contentType = SML_PCDATA_UNDEFINED;
    pPCData->extension = SML_EXT_UNDEFINED;
    pPCData->length = 0;
    pPCData->content = NULL;

    begin = pScanner->pos;

    // read Pcdata content until end tag appears
    while (hs_smlLibStrncmp((char*)pScanner->pos, _tagString2, hs_smlLibStrlen(_tagString2)) != 0)
    {
      if (pScanner->pos >= pScanner->bufend)
      {
        hs_smlLibFree(_tagString2);
        return HS_SML_ERR_XLT_INVAL_SYNCML_DOC;
      }
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        
    }
    
    hs_smlLibFree(_tagString2);
    
    len = pScanner->pos - begin;
    pPCData->content = hs_smlLibMalloc(__FILE__, __LINE__, len + 1); 
    if (pPCData->content == NULL) 
    {
        hs_smlLibFree(pPCData);
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    
    
    hs_smlLibMemset(pPCData->content, 0, len + 1);
    hs_smlLibMemcpy(pPCData->content, begin, len);
    pPCData->contentType = SML_PCDATA_STRING;
    pPCData->length = len;

    pScanner->curtok->type = TOK_CONT;
  //  if (pScanner->curtok->pcdata) //  chenxiang_memory 20080222      
  //  { 
         //SCI_TRACE_MID_PIM("MMIPIM chenxiang hs_xmlSkipPCDATA --  1 file: %s , line: %d , pcdata->content = %d", __FILE__, __LINE__, pScanner->curtok->pcdata->content);
  //      hs_smlLibFree(pScanner->curtok->pcdata->content);
  //      hs_smlLibFree(pScanner->curtok->pcdata);
   //     pScanner->curtok->pcdata = NULL;
   //  }
    pScanner->curtok->pcdata = pPCData;


    return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_xmlSkipComment
 *
 * Skip comments
 */
static short hs_xmlSkipComment(xmlScannerPrivPtr_t pScanner)
{
    hs_readBytes(pScanner, 4);

    while ((pScanner->pos + 3 <= pScanner->bufend) &&
           (hs_smlLibStrncmp((char*)pScanner->pos, "-->", 3) != 0))
		   if (!hs_readBytes(pScanner, 1))
           return HS_SML_ERR_XLT_END_OF_BUFFER;
        

    if (pScanner->pos + 3 > pScanner->bufend)
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    if (!hs_readBytes(pScanner, 3))
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    hs_skipS(pScanner);

    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_xmlSkipAttributes
 *
 * Skip attributes -> they are not supported in SyncML
 */
static short hs_xmlSkipAttributes(xmlScannerPrivPtr_t pScanner)
{ 

    while ((pScanner->pos + 1 <= pScanner->bufend) &&
           (hs_smlLibStrncmp((char*)pScanner->pos, ">", 1)) && (hs_smlLibStrncmp((char*)pScanner->pos, "/>", 2)))
		   if (!hs_readBytes(pScanner, 1))
           return HS_SML_ERR_XLT_END_OF_BUFFER;

    
    if (pScanner->pos + 1 > pScanner->bufend)
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_xmlSkipPI
 *
 * Skip PI elements
 */
static short hs_xmlSkipPI(xmlScannerPrivPtr_t pScanner)
{
    if (pScanner) { /* Get rid of warning, this should not be called anyway */
 	}

    return HS_SML_ERR_UNSPECIFIC;
}

/**
 * FUNCTION: hs_xmlCDATA
 *
 * Handle a CDATA content
 */
static short hs_xmlCDATA(xmlScannerPrivPtr_t pScanner)
{
    SmlPcdataPtr_t pPCData;
    unsigned char* begin;
    int len;
    
    hs_readBytes(pScanner, 9);

    pPCData = (SmlPcdataPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlPcdata_t));
    if (pPCData == NULL)
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    pPCData->contentType = SML_PCDATA_UNDEFINED;
    pPCData->length = 0;
    pPCData->content = NULL;

    begin = pScanner->pos;
    while (!((pScanner->pos[0] == ']') && (pScanner->pos[1] == ']') && (pScanner->pos[2] == '>')))
        if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;
		
    len = pScanner->pos - begin;
    pPCData->content = hs_smlLibMalloc(__FILE__, __LINE__, len + 1);
    if (pPCData->content == NULL) {
		hs_smlLibFree(pPCData);
 		return HS_SML_ERR_NOT_ENOUGH_SPACE;
 	}

    hs_smlLibMemset(pPCData->content, 0, len + 1);
    hs_smlLibMemcpy(pPCData->content, begin, len);
    pPCData->contentType = SML_PCDATA_CDATA;
    pPCData->length = len;

    pScanner->curtok->type = TOK_CONT;
//    if (pScanner->curtok->pcdata) //  chenxiang_memory 20080222      
//    { 
         //SCI_TRACE_MID_PIM("MMIPIM chenxiang hs_xmlCDATA --  2 file: %s , line: %d , pcdata->content = %d", __FILE__, __LINE__, pScanner->curtok->pcdata->content);
  //      hs_smlLibFree(pScanner->curtok->pcdata->content);
  //      hs_smlLibFree(pScanner->curtok->pcdata);
  //      pScanner->curtok->pcdata = NULL;
  //   }
    pScanner->curtok->pcdata = pPCData;

    hs_readBytes(pScanner, 3);

    return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_isPcdata
 *
 * Check if the current tag id represents a Pcdata element
 */
static unsigned char hs_isPcdata(XltTagID_t tagid)
{
  switch (tagid)
  {
    case TN_CMD:
    case TN_CMDID:
    case TN_CMDREF:
    case TN_LANG:
    case TN_LOCNAME:      
    case TN_LOCURI:   
    case TN_MSGID:   
    case TN_MSGREF:   
    case TN_RESPURI:   
    case TN_SESSIONID:   
    case TN_SOURCEREF:   
    case TN_TARGETREF:   
    case TN_VERSION:   
    case TN_PROTO:   
    case TN_DATA:
    case TN_META:
	case TN_NUMBEROFCHANGES:
#ifdef __HS_USE_METINF__
	case TN_METINF_EMI:
	case TN_METINF_FORMAT:
	case TN_METINF_FREEID:
	case TN_METINF_FREEMEM:
	case TN_METINF_LAST:
	case TN_METINF_MARK:
	case TN_METINF_MAXMSGSIZE:
	/* SCTSTK - 18/03/2002 S.H. 2002-04-05 : SyncML 1.1 */
	case TN_METINF_MAXOBJSIZE:
	case TN_METINF_NEXT:
	case TN_METINF_NEXTNONCE:		
	case TN_METINF_SIZE:
	case TN_METINF_TYPE:
	case TN_METINF_VERSION:
#endif
#ifdef __HS_USE_DEVINF__
    case TN_DEVINF_MAN:
    case TN_DEVINF_MOD:
    case TN_DEVINF_OEM:
    case TN_DEVINF_FWV:
    case TN_DEVINF_SWV:
    case TN_DEVINF_HWV:
    case TN_DEVINF_DEVID:
    case TN_DEVINF_DEVTYP:
    case TN_DEVINF_MAXGUIDSIZE:
    case TN_DEVINF_SOURCEREF:
    case TN_DEVINF_DISPLAYNAME:
    case TN_DEVINF_CTTYPE:
    case TN_DEVINF_DATATYPE:
    case TN_DEVINF_SIZE:
    case TN_DEVINF_PROPNAME:
    case TN_DEVINF_VALENUM:
    case TN_DEVINF_PARAMNAME:
    case TN_DEVINF_SYNCTYPE:
    case TN_DEVINF_XNAM:
    case TN_DEVINF_XVAL:
    case TN_DEVINF_MAXMEM:
    case TN_DEVINF_MAXID:
    case TN_DEVINF_VERCT:
    case TN_DEVINF_VERDTD:
#endif
      return 1;
    default:
      return 0;
  }
}


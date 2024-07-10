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
文件名称:PIMdecodewbxml.c
文件描述:wbxml文档解码主文件
文件说明:由codec.c调用，负责wbxml文档的解码。
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
  #define _PIMDECODEWBXML_C_
  
#include "../h/PIMdecodewbxml.h"
#include "../h/PIMsmlerr.h"
#include "../h/PIMlibmem.h"

#ifdef __HS_SML_WBXML__
  /*  数据结构声明*/

/* various checks about wbxml token */
#define HAS_ATTRIBUTES(tag) (*tag & 0x80)
#define HAS_CONTENT(tag) (*tag & 0x40)
#define IDENTITY(tag) (*tag & 0x3F)

#define IS_SWITCH(tok)  (*(tok) == 0x00)
#define IS_END_(tok)     (*(tok) == 0x01)  
#define IS_ENTITY(tok)  (*(tok) == 0x02)
#define IS_STR_I(tok)   (*(tok) == 0x03)
#define IS_LITERAL(tok) (IDENTITY(tok) == 0x04)
// Note: gcc cannot parse multi-line macros when file has DOS line ends
#define IS_EXT_I(tok)  ((*(tok) == 0x40) || (*(tok) == 0x41) || (*(tok) == 0x42))
#define IS_PI(tok)      (*(tok) == 0x43)
#define IS_EXT_T(tok)  ((*(tok) == 0x80) || (*(tok) == 0x81) || (*(tok) == 0x82))
#define IS_STR_T(tok)   (*(tok) == 0x83)
#define IS_EXT(tok)    ((*(tok) == 0xC0) || (*(tok) == 0xC1) || (*(tok) == 0xC2))
#define IS_OPAQUE(tok)  (*(tok) == 0xC3)
#define IS_STRING(tok) (IS_STR_I(tok) || IS_STR_T(tok))
#define IS_EXTENSION(tok) (IS_EXT_I(tok) || IS_EXT_T(tok) || IS_EXT(tok))

#define IS_ATTRIBUTE_VALUE(tok) (*(tok) & 0x80)
#define IS_ATTRIBUTE_START(tok) (~IS_ATTRIBUTE_VALUE(tok))


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
static void hs_setBuf(XltDecScannerPtr_t pScanner, unsigned char* , unsigned char*);  //liugang delete the const symbol for reduce compiling warnings! 
static unsigned char* hs_getPos(XltDecScannerPtr_t); 

/**
 * FUNCTION: hs_wbxmlHeader, hs_wbxmlVersion, hs_wbxmlPublicID, hs_wbxmlCharset
 *
 * These functions are used for decoding the WBXML document header.
 * hs_wbxmlHeader is a short wrapper that calls the other four functions in
 * the right order to scan the header. hs_wbxmlStrtbl makes a copy of the
 * string table.
 */
static short hs_wbxmlHeader(wbxmlScannerPrivPtr_t pScanner);
static short hs_wbxmlVersion(wbxmlScannerPrivPtr_t pScanner);
static short hs_wbxmlPublicID(wbxmlScannerPrivPtr_t pScanner);
static short hs_wbxmlCharset(wbxmlScannerPrivPtr_t pScanner);
static short hs_wbxmlStrtbl(wbxmlScannerPrivPtr_t pScanner);

/**
 * FUNCTION: hs_readBytes
 *
 * Advance the current position pointer after checking whether the end of
 * the buffer has been reached. If the end of the buffer has been reached
 * the scanner's finished flag is set.

 * RETURNS:        0, if end of buffer has been reached
 *                 1 otherwise
 */
static unsigned char hs_readBytes(wbxmlScannerPrivPtr_t pScanner, long bytes);

/**
 * FUNCTION: hs_parseInt
 *
 * Decodes multi-byte integers.
 *
 * PRE-Condition:
 *                 pScanner->pos points to the first byte of the mb_int.
 *
 * POST-Condition:
 *                 pScanner->pos points to the last byte of the mb_int.
 */
static short hs_parseInt(wbxmlScannerPrivPtr_t pScanner, long *mbi);

/**
 * FUNCTION: wbxmlXXXToken
 *
 * WBXML extensions, entities, processing instructions and attributes are
 * not supported by this scanner. If one is found it is skipped and
 * processing continues afterwards.
 */
static short hs_wbxmlSkipExtension(wbxmlScannerPrivPtr_t pScanner);
static short hs_wbxmlSkipEntity(wbxmlScannerPrivPtr_t pScanner);
static short hs_wbxmlSkipPI(wbxmlScannerPrivPtr_t);
static short hs_wbxmlSkipAttribute(wbxmlScannerPrivPtr_t);

/**
 * FUNCTION: wbxmlXXXToken
 *
 * Scan the document for the next valid XML/WBXML token as defined in the
 * XLTDecCom header file (e.g. TOK_TAG_START).
 *
 * PRE-Condition:
 *                 pScanner->pos points to the first byte of a valid WBXML
 *                 element (String, Tag, etc.)
 *
 * POST-Condition:
 *                 pScanner->pos points to the last byte of the WBXML
 *                 element;
 *                 pScanner->curtok contains type and tagid or pcdata of
 *                 the token
 */
static short hs_wbxmlStringToken(wbxmlScannerPrivPtr_t pScanner);
static short hs_wbxmlOpaqueToken(wbxmlScannerPrivPtr_t pScanner);
static short hs_wbxmlTagToken(wbxmlScannerPrivPtr_t pScanner);

/**
 * FUNCTION: hs_wbxmlSwitchPage
 *
 * Switch WBXML code page
 */
static short hs_wbxmlSwitchPage(wbxmlScannerPrivPtr_t pScanner);
	
  /*  功能函数*/

/**
 * FUNCTION: XltDecWbxmlInit
 *
 * Create and initialize a new WBXML scanner. Description see XLTDec.h.
 */
short hs_xltDecWbxmlInit(unsigned char* pBufEnd, unsigned char* *ppBufPos,
        XltDecScannerPtr_t *ppScanner) //liugang delete the const symbol for reduce compiling warnings! 
{
    wbxmlScannerPrivPtr_t pScanner;
    short rc;

    /* initialize new WBXML scanner */
    if ((pScanner = (wbxmlScannerPrivPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(wbxmlScannerPriv_t))) == NULL)
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    hs_smlLibMemset(pScanner, 0, sizeof(wbxmlScannerPriv_t));
    pScanner->bufend = pBufEnd;
    pScanner->pos = *ppBufPos;
    if ((pScanner->curtok = (XltDecTokenPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(XltDecToken_t))) == NULL) {
        hs_smlLibFree(pScanner);
        *ppScanner = NULL;
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    pScanner->curtok->pcdata = NULL;
    if ((rc = hs_xltUtilCreateStack(&pScanner->tagstack, 10)) != HS_SML_ERR_OK) {
        hs_smlLibFree((pScanner->curtok));
        hs_smlLibFree(pScanner);
        *ppScanner = NULL;
        return rc;
    }
    pScanner->state = TAG_STATE;

    /* point public/private methods to the right implementation */
    pScanner->nextTok = hs_nextTok;
    pScanner->destroy = hs_destroy;
    pScanner->pushTok = hs_pushTok;
    pScanner->setBuf = hs_setBuf;
    pScanner->getPos = hs_getPos;

    /* decode WBXML header */
    if ((rc = hs_wbxmlHeader(pScanner)) != HS_SML_ERR_OK) {
	      pScanner->destroy((XltDecScannerPtr_t)pScanner);
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
    wbxmlScannerPrivPtr_t pScannerPriv;

    if (pScanner == NULL)
        return HS_SML_ERR_OK;

    pScannerPriv = (wbxmlScannerPrivPtr_t)pScanner;
    if (pScannerPriv->tagstack != NULL)
        pScannerPriv->tagstack->destroy(pScannerPriv->tagstack);
    hs_smlLibFree((pScannerPriv->curtok));
    hs_smlLibFree((pScannerPriv->strtbl));
    hs_smlLibFree(pScannerPriv);

    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: nextTok
 *
 * Get next token.
 */
static short hs_nextTok(const char * file,  int line, XltDecScannerPtr_t pScanner) // chenxiang_memory 20080313
{
    wbxmlScannerPrivPtr_t pScannerPriv;
    short rc;

    pScannerPriv = (wbxmlScannerPrivPtr_t)pScanner;
    // T.K.: chanched Ptr_t to _t
    hs_smlLibMemset(pScanner->curtok, 0, sizeof(XltDecToken_t));
    pScannerPriv->curtok->start = pScannerPriv->pos;

    /* keep going until we find a "supported" element */
    rc = HS_SML_ERR_OK;
    while (rc == HS_SML_ERR_OK) {
        /* skip PIs, extensions and entities... */
        if (IS_PI(pScannerPriv->pos)) {
            rc = hs_wbxmlSkipPI(pScannerPriv);
        } else if (IS_EXTENSION(pScannerPriv->pos)) {
            rc = hs_wbxmlSkipExtension(pScannerPriv);
        } else if (IS_ENTITY(pScannerPriv->pos)) {
            rc = hs_wbxmlSkipEntity(pScannerPriv);

        /* ... decode strings, opaque data and tags */
        } else if (IS_STRING(pScannerPriv->pos)) {
            rc = hs_wbxmlStringToken(pScannerPriv);
            break;
        } else if (IS_OPAQUE(pScannerPriv->pos)) {
            rc = hs_wbxmlOpaqueToken(pScannerPriv);
            break;
        } else {
            rc = hs_wbxmlTagToken(pScannerPriv);
            break;
        }
    }

    return rc;
}

/**
 * FUNCTION: pushTok
 *
 * Reset the scanner to the starting position of the current token within
 * the buffer. 
 */
static short hs_pushTok(XltDecScannerPtr_t pScanner)
{
    wbxmlScannerPrivPtr_t pScannerPriv;
    XltUtilStackPtr_t pTagStack;
    XltTagID_t tagid;
    short rc = 0;

    pScannerPriv = (wbxmlScannerPrivPtr_t)pScanner;
    pTagStack = pScannerPriv->tagstack;

    if (pScannerPriv->curtok->start == NULL)
        return HS_SML_ERR_WRONG_USAGE;

    /* reset scanner to position where tok begins */
    pScannerPriv->pos = pScannerPriv->curtok->start;

    /* correct the tag stack */
    if (pScannerPriv->curtok->type == TOK_TAG_START) {
        rc = pTagStack->pop(pTagStack, &tagid);
    } else if (pScannerPriv->curtok->type == TOK_TAG_END) {
        tagid = pScannerPriv->curtok->tagid;
        rc = pTagStack->push(pTagStack, tagid);
    }
    if (rc) return rc;

    /* invalidate curtok */
    /* T.K. Possible Error. pScannerPriv->curtok is of type XltDecToken_t NOT ...Ptr_t */
    // OrigLine:
    // hs_smlLibMemset(pScannerPriv->curtok, 0, sizeof(XltDecTokenPtr_t));
    pScannerPriv->curtok->type = (XltTokType_t)0;
    
    return HS_SML_ERR_OK;
}

static void hs_setBuf(XltDecScannerPtr_t pScanner, unsigned char* pBufStart,
        unsigned char* pBufEnd) //liugang delete the const symbol for reduce compiling warnings! 
{
    wbxmlScannerPrivPtr_t pScannerPriv = (wbxmlScannerPrivPtr_t)pScanner;
    pScannerPriv->pos = pBufStart;
    pScannerPriv->bufend = pBufEnd;
}

static unsigned char* hs_getPos(XltDecScannerPtr_t pScanner)
{
    return ((wbxmlScannerPrivPtr_t)pScanner)->pos;
}

/**
 * FUNCTION: hs_wbxmlHeader
 *
 * PIM_Decode the WBXML header containing version number, document public
 * identifier, character set and a string table.
 */
static short hs_wbxmlHeader(wbxmlScannerPrivPtr_t pScanner)
{
    short rc;

    /* decode the WBXML header */
    if ((rc = hs_wbxmlVersion(pScanner)) != HS_SML_ERR_OK)
        return rc;
    if ((rc = hs_wbxmlPublicID(pScanner)) != HS_SML_ERR_OK)
        return rc;
    if ((rc = hs_wbxmlCharset(pScanner)) != HS_SML_ERR_OK)
        return rc;
    if ((rc = hs_wbxmlStrtbl(pScanner)) != HS_SML_ERR_OK)
        return rc;
    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wbxmlVersion
 *
 * PIM_Decode WBXML version. The scanner returns an error if the major version
 * of the document differs from the major version this scanner supports or
 * if the minor version of the document is larger than the minor version
 * the scanner supports.
 */
static short hs_wbxmlVersion(wbxmlScannerPrivPtr_t pScanner)
{
    unsigned char major, minor;

    minor = ((unsigned char)(*pScanner->pos & 0x0F));
     major = ((unsigned char)((*pScanner->pos >> 4) + 1));
   


    if (major != _MAJOR_VERSION || minor > _MINOR_VERSION)
        return HS_SML_ERR_XLT_INCOMP_WBXML_VERS;

    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wbxmlPublicID
 *
 * Decodes WBXML Document Public Identifier.
 */
static short hs_wbxmlPublicID(wbxmlScannerPrivPtr_t pScanner)
{
    long tmp;
    short rc;

    if (*pScanner->pos != 0) {
        /* pre-defined numeric identifier */
        if ((rc = hs_parseInt(pScanner, &tmp)) != HS_SML_ERR_OK)
            return rc;
        pScanner->pubID = tmp;
        pScanner->pubIDIdx = 0;
    } else {
        /* public id is given as string table entry (which we
           haven't read at this point so we'll save the reference
           for later) */
        if  (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        if ((rc = hs_parseInt(pScanner, &tmp)) != HS_SML_ERR_OK)
            return rc;
        pScanner->pubID = 0;
        pScanner->pubIDIdx = tmp;
    }
    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;
    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wbxmlCharset
 *
 * PIM_Decode WBXML Charset.
 */
static short hs_wbxmlCharset(wbxmlScannerPrivPtr_t pScanner)
{
    /* TODO: if charset iformation has to be processed
       it can be done here. For the moment only UTF-8 is used by SyncML */
    long mibenum;
    short rc;

    /* charset is given as a single IANA assigned MIBEnum value */
    if ((rc = hs_parseInt(pScanner, &mibenum)) != HS_SML_ERR_OK)
        return rc;
    pScanner->charset = mibenum;

    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wbxmlStrtbl
 *
 * Keep a copy of the string table.
 */
static short hs_wbxmlStrtbl(wbxmlScannerPrivPtr_t pScanner)
{
    long len;
    short rc;

    if ((rc = hs_parseInt(pScanner, &len)) != HS_SML_ERR_OK)
        return rc;
    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;
    pScanner->strtbllen = len;
    if (len > 0) {
        if (pScanner->pos + len > pScanner->bufend)
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        if ((pScanner->strtbl = hs_smlLibMalloc(__FILE__, __LINE__, len)) == NULL)
        {
            return HS_SML_ERR_NOT_ENOUGH_SPACE;
        }
        hs_smlLibMemcpy(pScanner->strtbl, pScanner->pos, len);
        hs_readBytes(pScanner, len);
    } else {
        pScanner->strtbl = NULL;
    }

    /* if the public ID was given as a string table reference save a
       reference to the corresponding string for later */
    if (pScanner->pubID == 0) {
        if (pScanner->pubIDIdx > pScanner->strtbllen)
            return HS_SML_ERR_XLT_INVAL_WBXML_DOC;
        pScanner->pubIDStr = (char*)(pScanner->strtbl + pScanner->pubIDIdx);
    }

    return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_readBytes
 *
 * Advance the position pointer. Description see above.
 */
static unsigned char hs_readBytes(wbxmlScannerPrivPtr_t pScanner, long bytes)
{
    if (pScanner->pos + bytes > pScanner->bufend) {
        pScanner->finished = 1;
        return 0;
    }
    pScanner->pos += bytes;
    return 1;
}

static short hs_parseInt(wbxmlScannerPrivPtr_t pScanner, long *mbi)
{
    *mbi = 0;
    /* accumulate byte value until continuation flag (MSB) is zero */
    for (;;)  {
        *mbi = *mbi << 7;
        *mbi += *(pScanner->pos) & 0x7F;
        if (!(*pScanner->pos & 0x80)) break;
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
    }
    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wbxmlSkipPI
 *
 * Handle XML processing instructions. PIs are not supported but the
 * scanner recognizes and skips over them.
 */
static short hs_wbxmlSkipPI(wbxmlScannerPrivPtr_t pScanner)
{
    /* PIs are just like tag attributes with a special PI token instead
     * of the attribute start token */
    return hs_wbxmlSkipAttribute(pScanner);
}

/**
 * FUNCTION: hs_wbxmlSkipAttribute
 *
 * Handle attributes. Attributes are not supported but the
 * scanner recognizes and skips over them.
 */
static short hs_wbxmlSkipAttribute(wbxmlScannerPrivPtr_t pScanner)
{
    XltDecTokenPtr_t oldtok;
    long tmp;
    short rc = 0;

    /* skipping attributes shouldn't change the current token so we
       make a copy... */
    if ((oldtok = (XltDecTokenPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(XltDecToken_t))) == NULL)
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    hs_smlLibMemcpy(oldtok, pScanner->curtok, sizeof(XltDecToken_t));

    /* ... skip until attribute end tag... */
    while (!IS_END_(pScanner->pos)) {
        if (IS_STRING(pScanner->pos)) {
            rc = hs_wbxmlStringToken(pScanner);
            /* avoid memory leak due to this ugly workaround of
               skipping attributes */
            hs_smlLibFree((pScanner->curtok->pcdata));
            pScanner->curtok->pcdata = NULL; // chenxiang_memory 20080222
        } else if (IS_EXTENSION(pScanner->pos)) {
            rc = hs_wbxmlSkipExtension(pScanner);
        } else if (IS_ENTITY(pScanner->pos)) {
            rc = hs_wbxmlSkipEntity(pScanner);
        } else if (IS_OPAQUE(pScanner->pos)) {
            rc = hs_wbxmlOpaqueToken(pScanner);
            /* avoid memory leak due to this ugly workaround of
               skipping attributes */
            hs_smlLibFree((pScanner->curtok->pcdata));
            pScanner->curtok->pcdata = NULL; // chenxiang_memory 20080222
        } else if (IS_LITERAL(pScanner->pos)) {
            if (!hs_readBytes(pScanner, 1))
                return HS_SML_ERR_XLT_END_OF_BUFFER;
            rc = hs_parseInt(pScanner, &tmp);
            if (!hs_readBytes(pScanner, 1))
                return HS_SML_ERR_XLT_END_OF_BUFFER;
        } else if (IS_SWITCH(pScanner->pos)) {
            rc = hs_wbxmlSwitchPage(pScanner);
        } else {
            if (!hs_readBytes(pScanner, 1))
                return HS_SML_ERR_XLT_END_OF_BUFFER;
        }

        if (rc != HS_SML_ERR_OK) {
            hs_smlLibFree(oldtok);
            return rc;
        }

    }
    /* ... then skip the end tag itself... */
    hs_readBytes(pScanner, 1);

    /* ... and finaly restore our copy of curtok */
    hs_smlLibMemcpy(pScanner->curtok, oldtok, sizeof(XltDecToken_t));
    hs_smlLibFree(oldtok);

    return HS_SML_ERR_OK;
}


/**
 * FUNCTION: hs_wbxmlSkipExtension
 *
 * PIM_Decode WBXML extensions. Skips the extension but doesn't do anything
 * useful with it.
 */
static short hs_wbxmlSkipExtension(wbxmlScannerPrivPtr_t pScanner)
{
    long tmp;
    short rc;

    if (IS_EXT(pScanner->pos)) {
        /* single byte extension token */
        if (!hs_readBytes(pScanner, 1))
          return HS_SML_ERR_XLT_END_OF_BUFFER;
    } else if (IS_EXT_I(pScanner->pos)) {
        /* inline string extension token */
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        if (!hs_readBytes(pScanner, hs_smlLibStrlen((char*)pScanner->pos) + 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
    } else {
        /* inline integer extension token */
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        if ((rc = hs_parseInt(pScanner, &tmp)) != HS_SML_ERR_OK)
            return rc;
        if (!hs_readBytes(pScanner, tmp + 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
    }
    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wbxmlSkipEntity
 *
 * Skips entities but doesn't do anything useful yet.
 */
static short hs_wbxmlSkipEntity(wbxmlScannerPrivPtr_t pScanner)
{
    long tmp;
    short rc;

    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;
    if ((rc = hs_parseInt(pScanner, &tmp)) != HS_SML_ERR_OK)
        return rc;
    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    return HS_SML_ERR_OK;
}

static short hs_wbxmlStringToken(wbxmlScannerPrivPtr_t pScanner)
{
    SmlPcdataPtr_t pPcdata;
    short rc;

    if ((pPcdata = (SmlPcdataPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlPcdata_t))) == NULL)
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    /* copy the string into the new PCdata struct */
    if (IS_STR_I(pScanner->pos)) {
        /* inline string */
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        pPcdata->extension   = SML_EXT_UNDEFINED;
        pPcdata->contentType = SML_PCDATA_STRING;
        pPcdata->length = hs_smlLibStrlen((char*)pScanner->pos);
        if (pScanner->pos + pPcdata->length + 1 > pScanner->bufend) {
            hs_smlLibFree(pPcdata);
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        }
        if ((pPcdata->content = hs_smlLibMalloc(__FILE__, __LINE__, pPcdata->length + 1)) == NULL) {
            hs_smlLibFree(pPcdata);
            return HS_SML_ERR_NOT_ENOUGH_SPACE;
        }
        hs_smlLibStrncpy(pPcdata->content, (char*)pScanner->pos, pPcdata->length + 1);
        hs_readBytes(pScanner, pPcdata->length + 1);

    } else {
        /* string table reference */
        long offset; /* offset into string table */
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        if ((rc = hs_parseInt(pScanner, &offset)) != HS_SML_ERR_OK) {
            hs_smlLibFree(pPcdata);
            return rc;
        }
        if (offset >= pScanner->strtbllen) {
            hs_smlLibFree(pPcdata);
            return HS_SML_ERR_XLT_INVAL_WBXML_DOC;
        }
        pPcdata->contentType = SML_PCDATA_STRING;
        pPcdata->length = hs_smlLibStrlen((char*)(pScanner->strtbl + offset));
        if ((pPcdata->content = hs_smlLibMalloc(__FILE__, __LINE__, pPcdata->length + 1)) == NULL) {
            hs_smlLibFree(pPcdata);
            return HS_SML_ERR_NOT_ENOUGH_SPACE;
        }
        hs_smlLibStrncpy(pPcdata->content, (char*)(pScanner->strtbl + offset), pPcdata->length + 1);
        hs_readBytes(pScanner, 1);
    }

    pScanner->curtok->pcdata = pPcdata;

    pScanner->curtok->type = TOK_CONT;

    return HS_SML_ERR_OK;
}

static short hs_wbxmlOpaqueToken(wbxmlScannerPrivPtr_t pScanner)
{
    SmlPcdataPtr_t pPcdata = NULL;
    long len;
    short rc;

    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    /* a mbi indicates the length of the opaque data block that we'll
       copy into new PCdata struct */
    if ((rc = hs_parseInt(pScanner, &len)) != HS_SML_ERR_OK)
        return rc;
    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;
    if (pScanner->pos + len > pScanner->bufend)
        return HS_SML_ERR_XLT_END_OF_BUFFER;
    if ((pPcdata = (SmlPcdataPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlPcdata_t))) == NULL)
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    pPcdata->extension   = SML_EXT_UNDEFINED;
    pPcdata->contentType = SML_PCDATA_OPAQUE;
    pPcdata->length = len;
    /* Modification 2001-07-03 by Luz %%%%%:
     * made sure that content is one null byte longer
     * than indicated opaque content, such that strings that are coded as
     * opaque (happens to be the case with Nokia 9210) can still be read
     * as C-string without need for an intermediate buffer
     */
    /* original:
    if ((pPcdata->content = hs_smlLibMalloc(__FILE__, __LINE__, len)) == NULL) {
        hs_smlLibFree(pPcdata);
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    */
    /* modified: */
    if ((pPcdata->content = hs_smlLibMalloc(__FILE__, __LINE__, len+1)) == NULL) {
        hs_smlLibFree(pPcdata);
        return HS_SML_ERR_NOT_ENOUGH_SPACE;
    }
    ((char *)pPcdata->content)[len]=0; /* make sure there is a c-string terminator */
    /* end modification */
    
    hs_smlLibMemcpy(pPcdata->content, pScanner->pos, len);
    pScanner->curtok->pcdata = pPcdata;

    hs_readBytes(pScanner, len);

    pScanner->curtok->type = TOK_CONT;

    return HS_SML_ERR_OK;
}

/**
 * FUNCTION: hs_wbxmlSwitchPage
 *
 * Switch WBXML code page.
 */
/* T.K. 06.02.01
 * We need to enhance this as soon as we introduce 
 * Sub DTD's with more than one WBXML codepage. But till then
 * there is only one case where WBXML codepages can occure, and 
 * this is the MetInf Sub DTD. So in case we find a codepage switch
 * to something other than codepage zero, we set the active extension 
 * to metinf.
 * In future versions the pScanner needs to be enhanced, to translate
 * codepageswitches context sensitive to the active extension.
 */
static short hs_wbxmlSwitchPage(wbxmlScannerPrivPtr_t pScanner)
{
    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;
    if (pScanner->state == TAG_STATE)
        pScanner->cptag = (SmlPcdataExtension_t)*pScanner->pos;
    else 
        pScanner->cpattr = *pScanner->pos;
    hs_readBytes(pScanner, 1);
    /* T.K. this needs to be adjusted as described above */
    if (pScanner->cpattr != 0 || pScanner->cptag != 0)
        pScanner->activeExt = SML_EXT_METINF;
    else
        pScanner->activeExt = SML_EXT_UNDEFINED;
    return HS_SML_ERR_OK;
}


static short hs_wbxmlTagToken(wbxmlScannerPrivPtr_t pScanner)
{
    XltTagID_t tagid;
    unsigned char has_cont, has_attr;
    short rc;

    if (IS_SWITCH(pScanner->pos)) {
        if ((rc = hs_wbxmlSwitchPage(pScanner)) != HS_SML_ERR_OK)
            return rc;
    }

    /* we have to look at the top of the tagstack to see which
       start tag an end tag belongs to */
    if (IS_END_(pScanner->pos)) {
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        pScanner->curtok->type = TOK_TAG_END;
        rc = pScanner->tagstack->pop(pScanner->tagstack, &tagid);
        if (rc == HS_SML_ERR_WRONG_USAGE)
            return HS_SML_ERR_XLT_INVAL_WBXML_DOC;
        else if (rc)
            return rc;
        pScanner->curtok->tagid = tagid;
        return HS_SML_ERR_OK;
    } 


    /* look at the two MSB: does this tag have content or attributes? */
    
     has_cont = ((unsigned char)(HAS_CONTENT(pScanner->pos)));
     has_attr = ((unsigned char)(HAS_ATTRIBUTES(pScanner->pos)));


    /* look up tag ID either by string or by number */
    if (IS_LITERAL(pScanner->pos)) {
        long offset; /* offset into the string table */
        if (!hs_readBytes(pScanner, 1))
            return HS_SML_ERR_XLT_END_OF_BUFFER;
        if ((rc = hs_parseInt(pScanner, &offset)) != HS_SML_ERR_OK)
            return rc;
        if (offset > pScanner->strtbllen)
            return HS_SML_ERR_XLT_INVAL_WBXML_DOC;

        rc = (short)hs_getTagIDByStringAndExt((char*)(pScanner->strtbl + offset), pScanner->activeExt, &tagid);
        if ((tagid == TN_UNDEF) || (rc != HS_SML_ERR_OK)) return rc;

    } else {
        rc = (short)hs_getTagIDByByteAndExt((unsigned char)IDENTITY(pScanner->pos), pScanner->activeExt, &tagid);
        if ((tagid == TN_UNDEF) || (rc != HS_SML_ERR_OK)) return rc;

    }

    /* we know everything we need to know */
    pScanner->curtok->tagid = tagid;
    pScanner->curtok->type = has_cont ? TOK_TAG_START : TOK_TAG_EMPTY;
#ifdef __HS_USE_METINF__    
    pScanner->curtok->ext = pScanner->cptag == 0 ? SML_EXT_UNDEFINED : SML_EXT_METINF;
#else
	pScanner->curtok->ext = SML_EXT_UNDEFINED;
#endif

    if (!hs_readBytes(pScanner, 1))
        return HS_SML_ERR_XLT_END_OF_BUFFER;

    /* push tag onto tagstack unless this tag is empty */
    if (has_cont) {
        if ((rc = pScanner->tagstack->push(pScanner->tagstack, tagid)) != HS_SML_ERR_OK)
            return rc;
    }

    /* skip attributes */
    if (has_attr) {
        pScanner->state = ATTRIBUTE_STATE;
        if ((rc = hs_wbxmlSkipAttribute(pScanner)) != HS_SML_ERR_OK)
            return rc;
        pScanner->state = TAG_STATE;
    }

    return HS_SML_ERR_OK;
}

#ifdef __HS_USE_EXTENSIONS__
/*
 * This function tries to decode an inlined WBXML document inside
 * an PCDATA element.
 * In case of failing to decode it the PCDATA element isn't changed
 * at all.
 */
 
void hs_subdtdDecodeWbxml(XltDecoderPtr_t pDecoder,SmlPcdataPtr_t *ppPcdata)
{
    short                 _err         = HS_SML_ERR_OK;
    unsigned char*              pSubBuf      = NULL;
    SmlPcdataPtr_t        pSubPcdata   = NULL;
    XltDecoderPtr_t       pSubDecoder  = NULL;
#ifdef __HS_USE_DEVINF__ 
    wbxmlScannerPrivPtr_t pScannerPriv = NULL;
#endif   

    /* some sanity checks at first */    
   
    if (*ppPcdata == NULL) {
 		if (pDecoder) /* use this rare case to remove warning */
 		{
 		}
 		return;
 	}

    if ((*ppPcdata)->contentType != SML_PCDATA_OPAQUE) return;

    // now create a sub buffer
    pSubBuf = (unsigned char*)hs_smlLibMalloc(__FILE__, __LINE__, (*ppPcdata)->length);
    if (pSubBuf == NULL) return;
    hs_smlLibMemset(pSubBuf, 0x00, (*ppPcdata)->length);
    hs_smlLibMemmove(pSubBuf, (*ppPcdata)->content, (*ppPcdata)->length);
    
    /* ok looks fine sofar - now lets decode the rest */
    /* now lets create a decoder, but without parsing the SyncML
     * start tags (because it's not there) and skip the XML
     * part as we don't need it.
     */
    pSubDecoder = (XltDecoderPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(XltDecoder_t));
    if (pSubDecoder == NULL) {
        hs_smlLibFree(pSubBuf);
        return;
    }
    pSubDecoder->finished = 0;
    pSubDecoder->final    = 0;
    pSubDecoder->scanner  = NULL;
    if (hs_xltUtilCreateStack(&pSubDecoder->tagstack, 10) != HS_SML_ERR_OK) {
        hs_smlLibFree(pSubDecoder);
        hs_smlLibFree(pSubBuf);
        return;
    }
    if (hs_xltDecWbxmlInit(pSubBuf+(*ppPcdata)->length,&pSubBuf, &pSubDecoder->scanner) != HS_SML_ERR_OK) {
        hs_xltDecTerminate(pSubDecoder);
        hs_smlLibFree(pSubBuf);
        return;
    }
    pSubDecoder->charset    = pSubDecoder->scanner->charset;
    pSubDecoder->charsetStr = NULL;

    pSubPcdata = (SmlPcdataPtr_t)hs_smlLibMalloc(__FILE__, __LINE__, sizeof(SmlPcdata_t));
    if (pSubPcdata == NULL) {
        hs_xltDecTerminate(pSubDecoder);
        hs_smlLibFree(pSubPcdata);
        hs_smlLibFree(pSubBuf);
        return;
    }
    /* T.K.
     * In the future we need to check the WBXML stringtable and
     * switch into the right Sub DTD. But sofar only DevInf is
     * supported so we can save time and space
     */
    /* T.K.
     * To prevent buffer corruption when __HS_USE_DEVINF__ is not used
     * we initialize _err with any errorcode != OK, and this way
     * force the function to exit without modifying the ppPcdata
     */
    _err = HS_SML_ERR_UNSPECIFIC;
#ifdef __HS_USE_DEVINF__    
    pSubPcdata->contentType   = SML_PCDATA_EXTENSION;
    pSubPcdata->extension     = SML_EXT_DEVINF;
    pSubPcdata->length        = 0;
    pSubPcdata->content       = NULL;

    pScannerPriv = (wbxmlScannerPrivPtr_t)pSubDecoder->scanner;
    pScannerPriv->activeExt  = SML_EXT_DEVINF;
    pScannerPriv->cpattr     = 0;
    pScannerPriv->cptag      = (SmlPcdataExtension_t)0;
    hs_smlLibMemset(pScannerPriv->curtok, 0,sizeof(XltDecToken_t)); 

    _err = hs_buildDevInfDevInfCmd(pSubDecoder, (void*)&pSubPcdata->content);
#endif

    if (_err != HS_SML_ERR_OK) {
        hs_xltDecTerminate(pSubDecoder);
        hs_smlLibFree(pSubPcdata);
        hs_smlLibFree(pSubBuf);
        return;
    }
    
    /* parsing is done, now lets anchor it within the original PCDATA element */
    hs_smlFreePcdata( __FILE__, __LINE__, *ppPcdata);
    *ppPcdata = pSubPcdata;

    /* we are done */
    hs_xltDecTerminate(pSubDecoder);
    hs_smlLibFree(pSubBuf);

    return;
}
    
#endif

#endif

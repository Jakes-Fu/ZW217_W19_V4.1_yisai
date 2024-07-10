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
文件名称:PIMcodec.h
文件描述:编解码头文件
文件说明:用于进行生成数据的编解码。编解码包括两种
                       XML/WBXML。
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
/*加入此FEATURE防止头文件重复包含*/
  #ifndef   HEADER_FILE_CODEC
  #define  HEADER_FILE_CODEC
  
  /*  包含头文件*/
  #include "PIMcallback_handler.h"
  //#include "../h/mmipim.h"

  /*  数据结构声明*/

  // XML version
#define XML_VERSION "1.0"

// XML encoding
#define XML_ENCODING "UTF-8"

// byte for XML tag begin parentheses
#define XML_BEGPAR '<'

// byte for XML tag end parentheses
#define XML_ENDPAR '>'

// byte for XML tag del
#define XML_TAGDEL '/'

// XML namespaceattribute
#define XML_NSSTART " xmlns='"
#define XML_NSEND   "'"

#define XML_MAX_TAGLEN 35
  
  // byte for WBXML Version Number
#define XLT_WBXMLVER 0x02

// byte for WBXML charset - not yet implemented - default UTF-8
#define XLT_CHARSET 0x6A

// public identifier 0x00,0x00 -> unknown, use stringtable
#define XLT_PUBIDENT1 0x00
#define XLT_PUBIDENT2 0x00

// switch page tag 0x00
#define XLT_SWITCHPAGE 0x00

// termination character for certain WBXML element types (e.g. STR_I)
#define XLT_TERMSTR 0x00

// byte to add to a tag if a content follows
#define XLT_CONTBYTE 0x40

#ifdef __HS_SML_XML__
#define _TOKEN(id, wbxml, xml) (id), (wbxml), (xml)
#else
#define _TOKEN(id, wbxml, xml) (id), (wbxml), ""
#endif


// Tag IDs
typedef enum {
    TN_UNDEF = 0,

    TN_ADD,
    TN_ALERT,
    TN_ARCHIVE,
    TN_ATOMIC,
    TN_ATOMIC_END, /* 5 */
    TN_CHAL,
    TN_CMD,
    TN_CMDID,
    TN_CMDREF,
    TN_COPY,     /* 10 */
    TN_CRED,
    TN_DATA,
    TN_DELETE,
    TN_EXEC,
    TN_FINAL,    /* 15 */
    TN_GET,
    TN_ITEM,
    TN_LANG,
    TN_LOCNAME,
    TN_LOCURI,  /* 20 */
    TN_MAP,
    TN_MAPITEM,
    TN_META,
    TN_MSGID,
    TN_MSGREF,  /* 25 */
    TN_NORESP,
    TN_NORESULTS,
    TN_PUT,
    TN_REPLACE,
    TN_RESPURI, /* 30 */
    TN_RESULTS,
    TN_SEARCH,
    TN_SEQUENCE,
    TN_SEQUENCE_END,
    TN_SESSIONID, /* 35 */
    TN_SFTDEL,
    TN_SOURCE,
    TN_SOURCEREF,
    TN_STATUS,
    TN_SYNC, /* 40 */
    TN_SYNCBODY,
    TN_SYNCHDR,
    TN_SYNCML,
    TN_SYNC_END,
    TN_TARGET, /* 45 */
    TN_TARGETREF,
    TN_VERSION,
    TN_PROTO,
    TN_METINF_ANCHOR,
    TN_METINF_EMI, /* 50 */
    TN_METINF_FORMAT, 
    TN_METINF_FREEID,
    TN_METINF_FREEMEM,		
    TN_METINF_LAST,
    TN_METINF_MARK, /* 55 */
    TN_METINF_MAXMSGSIZE,
    TN_METINF_MEM,
    TN_METINF_METINF,
    TN_METINF_NEXT,
    TN_METINF_NEXTNONCE, /* 60 */
    TN_METINF_SHAREDMEM,
    TN_METINF_SIZE,
    TN_METINF_TYPE,
    TN_METINF_VERSION,
    TN_DEVINF_CTCAP,    /* 65 */
    TN_DEVINF_CTTYPE,
    TN_DEVINF_DATASTORE,
    TN_DEVINF_DATATYPE,
    TN_DEVINF_DEVID,
    TN_DEVINF_DEVINF, /* 70 */
    TN_DEVINF_DEVTYP,
    TN_DEVINF_DISPLAYNAME,
    TN_DEVINF_DSMEM,
    TN_DEVINF_EXT,
    TN_DEVINF_FWV, /* 75 */
    TN_DEVINF_HWV,
    TN_DEVINF_MAN,
    TN_DEVINF_MAXGUIDSIZE,
    TN_DEVINF_MAXID,
    TN_DEVINF_MAXMEM, /* 80 */
    TN_DEVINF_MOD,
    TN_DEVINF_OEM,
    TN_DEVINF_PARAMNAME,
    TN_DEVINF_PROPNAME,
    TN_DEVINF_RX, /* 85 */
    TN_DEVINF_RXPREF,
    TN_DEVINF_SHAREDMEM,
    TN_DEVINF_SIZE,
    TN_DEVINF_SOURCEREF,
    TN_DEVINF_SWV, /* 90 */
    TN_DEVINF_SYNCCAP,
    TN_DEVINF_SYNCTYPE,
    TN_DEVINF_TX,
    TN_DEVINF_TXPREF,
    TN_DEVINF_VALENUM, /* 95 */
    TN_DEVINF_VERCT,
    TN_DEVINF_VERDTD,
    TN_DEVINF_XNAM,
    TN_DEVINF_XVAL,
    /* SCTSTK - 18/03/2002, S.H. 2002-04-05 : SyncML 1.1 */
    TN_NUMBEROFCHANGES, /* 100 */
    TN_MOREDATA,
    TN_METINF_MAXOBJSIZE,
    TN_DEVINF_UTC,
    TN_DEVINF_NOFM,
    TN_DEVINF_LARGEOBJECT
} XltTagID_t;

/* Tag Types (TT) - begin tags, end tags, ...
 */
typedef enum {
  TT_END = 0,
  TT_BEG,
  TT_ALL
} XltTagType_t;

/* Structure elements which are not directly relied to a tag
*/
typedef enum {
  ITEM_LIST,
  TARGET_LIST,
  SOURCE_LIST,
  META_LIST,
  MAPITEM_LIST,
  TARGETREF_LIST,
  SOURCEREF_LIST
} XltListType_t;

#if 0
  /** * Type of used encoding **/
typedef enum {
	SML_UNDEF = 0,
        SML_WBXML,
        SML_XML
} SmlEncoding_t;
#endif
 /* *
 *SyncML version
 **/
typedef enum {
  SML_VERS_UNDEF = 0,
  SML_VERS_1_0,
  SML_VERS_1_1,
  SML_NUM_VERS
} SmlVersion_t;
  
  typedef struct Tag_s
{
    XltTagID_t  id;
    unsigned char      wbxml;
    char*    xml;
} Tag_t, *TagPtr_t;

  // global tokens in WBXML
typedef enum {
  UNDEF = 0,
  END,
  STR_I,
  _OPAQUE,   //在VC编译下重复定义，加了下划线  note by wangxiaolin
  TAG
} XltElementType_t;

 // flags if a (WB)XML block is required or optional
typedef enum {
  _OPTIONAL = 0,  //在VC编译下重复定义，加了下划线  note by wangxiaolin
  REQUIRED
} XltRO_t;

 typedef struct PEEnc_s
{
    XltTagID_t     tagid;
    SmlProtoElement_t type;
} PEEnc_t, *PEEncPtr_t;


// Type for storing free mem size evaluation information
typedef struct XltSpaceEvaluation_s
{
  long written_bytes;
  long end_tag_size;
  // %%% luz 2002-09-03: evaluation needs private shadow copies of these:
  SmlPcdataExtension_t cur_ext;
  SmlPcdataExtension_t last_ext;
} XltSpaceEvaluation_t, *XltSpaceEvaluationPtr_t;
  
  //Type for storing encoder information
typedef struct XltEncoder_s
{
  SmlEncoding_t enc;
  SmlVersion_t vers; // %%% luz 2003-07-31: added SyncML version here
  SmlPcdataExtension_t cur_ext;
  SmlPcdataExtension_t last_ext;
  unsigned char final;
  XltSpaceEvaluationPtr_t space_evaluation;
  long end_tag_size;
} XltEncoder_t, *XltEncoderPtr_t;

typedef struct Dtd_s
{
	char*	         name;
	SmlPcdataExtension_t ext;
} Dtd_t, *DtdPtr_t;

  typedef struct bufferMgmt_s
{
  unsigned char* smlXltBufferP;
  unsigned char* smlXltStoreBufP;
  long smlXltWrittenBytes;
  long smlXltBufferLen;
  XltTagID_t           switchExtTag;
  SmlPcdataExtension_t smlCurExt;
  SmlPcdataExtension_t smlLastExt;
  SmlPcdataExtension_t smlActiveExt;
  long endTagSize;
  unsigned char spaceEvaluation;
  // %%% luz:2003-04-24: added syncmlvers
  // %%% luz:2003-07-31: made it an enum, now called vers
  SmlVersion_t vers;
} BufferMgmt_t, *BufferMgmtPtr_t;

/* Token types returned by the scanner */
typedef enum {
    TOK_UNDEF = 0,
    TOK_TAG_START,
    TOK_TAG_END,
    TOK_TAG_EMPTY,
    TOK_CONT
} XltTokType_t;

/**
 * Struct containing a token returned by the scanner
 */
typedef struct
{
    XltTokType_t type;       /* XLT_TOK_TAG_START, etc. */
    XltTagID_t tagid;        /* TN_SYNCHDR, etc. - valid for token of type
				XLT_TOK_TAG_xxx */
    SmlPcdataExtension_t ext;   /* type of extension this tag belongs to -
				valid for token of type XLT_TOK_TAG_xxx */
    SmlPcdataPtr_t pcdata;      /* valid for token of type XLT_TOK_CONT_xxx */
    unsigned char* start;          /* pointer to the start of this token within
				the document - needed for pushback */
} XltDecToken_t, *XltDecTokenPtr_t;

/**
 * Public interface for the XML/WBXML scanner components.
 *
 * The struct used for holding the XML and WBXML scanner state information
 * are the same - function pointers are used to map the "objects" public
 * methods to the right functions for scanning XML and WBXML documents.
 * This object-oriented interface enables the parser component to use the
 * two scanners interchangeably.
 * The interface contains serveral public methods and attributes:
 *
 * FUNCTION: nextTok
 *
 * Decodes the next valid token at the current position within the
 * document. Information about this token is placed into the curtok
 * attribute of the scanner object.
 *
 * PRE-Condition:
 * 
 * POST-Condition:
 *                 curtok contains the last valid token.
 *
 * IN:
 *
 * OUT:
 *
 * IN/OUT:         the scanner
 *
 * RETURNS:        HS_SML_ERR_OK or an appropriate error code
 *
 * 
 * FUNCTION: destroy
 *
* Free the memory allocated by the scanner.
*
* PRE-Condition:
* POST-Condition:
*
* IN:
*
* OUT:
*
* IN/OUT:         the scanner
*
* RETURNS:        HS_SML_ERR_OK or an appropriate error code
*
*
* FUNCTION: pushTok
*
* Resets the scanner to the beginning position within the document of the
* last valid token stored in curtok. Only the last found token can be
* pushed back. It is not possible to go back more than one token.
*
* PRE-Condition:
*                 curtok contains a valid token.
*                 
* POST-Condition:
*                 the next call of nextTok will find the token that was
*                 pushed back.
*
* IN/OUT:         the scanner
*
* RETURNS:        HS_SML_ERR_OK or an appropriate error code
*
* FUNCITON: setBuf
*
* Set the buffer the scanner works on.
*
* FUNCTION: getPos
*
* Get the current position of the scanner within the working buffer.
*
* ATTRIBUTE: curtok
*
* Contains the last valid token found by a call to nextTok.
*
* ATTRIBUTE: charset
*
* The charset information as specified in the XML/WBXML document. This is
* the IANA assigned MIBEnum value.

* ATTRIBUTE: charsetStr
*
* String representation of the charset. This attribute is valid only when
* charset equals zero. Otherwise charsetStr will be NULL.
*
* ATTRIBUTE: pubID
*
* The document public identifier as specified in the XML/WBXML document.
* This is the numeric identifier assigned by the WAP Forum. If this value
* is zero, the public ID is instead specified as a string contained in the
* pubIDStr public attribute.
*
* ATTRIBUTE: pubIDStr
*
* The string representation of the document public identifier as specified
* in the XML/WBXML document (e.g. "-//WAPFORUM//DTD WML 1.0//EN"). This
* attribute is valid only when pubID equals zero. Otherwise pubIDStr will
* be NULL.
*
* ATTRIBUTE: finished
*
* This flag is set by the nextTok method when the scanner reaches the end
* of the buffer.
*/
typedef struct XltDecScanner_s XltDecScanner_t, *XltDecScannerPtr_t;
struct XltDecScanner_s
{
    /* public methods */
    short (*nextTok)(const char * file,  int line, XltDecScannerPtr_t pScanner); // chenxiang_memory 20080313
    short (*destroy)(XltDecScannerPtr_t pScanner);
    short (*pushTok)(XltDecScannerPtr_t pScanner);
    void (*setBuf)(XltDecScannerPtr_t pScanner, const unsigned char* pBufStart, const unsigned char* pBufEnd); 
    unsigned char* (*getPos)(XltDecScannerPtr_t pScanner); 

    /* public attributes */
    XltDecTokenPtr_t curtok;
    long charset;
    char* charsetStr;
    long pubID;
    char* pubIDStr;
    unsigned int finished;
};

/* type for stack elements */
typedef XltTagID_t XltUtilStackItem_t;

/**
 * XLTUtilStack interface
 *
 * Like the WBXML/XML scanner, this stack implementation tries to emulate
 * an object-oriented interface. It consist of one stack structure that
 * contains the public methods and attributes and another private stack
 * structure that is not visible to the users of the stack. A stack object
 * has the following public methods:
 *
 * FUNCTION: top
 *
 * Returns the top element of the stack.
 *
 * PRE-Condition:
 *                 the stack contains at least one element
 *
 * POST-Condition:
 *
 * IN:             the stack
 *
 * OUT:            the top element of the stack
 *
 * IN/OUT:
 *
 * RETURNS:        HS_SML_ERR_WRONG_USAGE, if the stack is empty
 *                 HS_SML_ERR_OK, else
 *
 * FUNCTION: pop
 *
 * Returns the top element and takes it off the stack.
 *
 * PRE-Condition:
*                 the stack contains at least one element
*
* POST-Condition:
*                 the top element of the stack is removed
*
* IN:
*
* OUT:            the top element of the stack
*
* IN/OUT:         the stack
*
* RETURNS:        HS_SML_ERR_WRONG_USAGE, if the stack is empty
*                 HS_SML_ERR_NOT_ENOUGH_SPACE, if memory reallocation failed
*                 HS_SML_ERR_OK, else
*
* FUNCTION: push
*
* Put a new element on top of the stack.
*
* PRE-Condition:
*
* POST-Condition:
*                 popping the stack yields the same stack as before
*                 the push
*
* IN:             the new stack element
*
* OUT:
*
* IN/OUT:         the stack
*
* RETURNS:        HS_SML_ERR_NOT_ENOUGH_SPACE, if memory reallocation failed
*                 HS_SML_ERR_OK, else
*
* FUNCTION: destroy
*
* Free the memory used by the stack.
*
* PRE-Condition:
* POST-Condition:
*
* IN:
*
* OUT:
*
* IN/OUT:         the stack
*
* RETURNS:        HS_SML_ERR_OK
*/
typedef struct XltUtilStack_s *XltUtilStackPtr_t, XltUtilStack_t;
struct XltUtilStack_s
{
        short (*top)(const XltUtilStackPtr_t, XltUtilStackItem_t *);
        short (*pop)(XltUtilStackPtr_t, XltUtilStackItem_t *);
        short (*push)(XltUtilStackPtr_t, const XltUtilStackItem_t);
        short (*destroy)(XltUtilStackPtr_t);
};

/**
 * The XLT Decoder Interface consists of a single XltDecoder "object"
 * (struct) and an creation/initialization function. The XltDecoder
 * object contains all "public" methods and data structures. The first
 * parameter for any public method is the object of which the method is
 * called.
 * 
 * The decoder's public methods/attributes are:
 * 
 * ATTRIBUTE: charset
 *
 * Character set used in the document - this is the MIBEnum value assigned
 * by the IANA for the character encoding, e.g. "3" for US-ASCII.
 *
 * ATTRIBUTE: charsetStr
 * 
 * Name of the character set, e.g. "US-ASCII" - valid only when charset == 0.
 *
 * ATTRIBUTE: finished
 *
 * Indicates whether the decoder has reached the end of the buffer during
 * the last call to hs_xltDecNext.
 *
 * ATTRIBUTE: scanner
 *
 * Pointer to the scanner status object used by this decoder. The scanner
 * will be created during the initialization of the decoder as either a XML
 * or WBXML scanner.
 *
 * 
 * ATTRIBUTE: tagstack
 *
 * The decoder uses an internal stack to check that for every start tag
 * there is a corresponding end tag.

 */
typedef struct XltDecoder_s
{
    long charset;
    char* charsetStr;
    unsigned int finished;
    unsigned char final;
    XltDecScannerPtr_t scanner;
    XltUtilStackPtr_t tagstack;

} XltDecoder_t, *XltDecoderPtr_t;

  /*  全局变量声明*/

  /*  功能函数声明*/
  /**
 * FUNCTION: smlXltEncInit
 *
 * Initializes an XML buffer; Creates XML code for the SyncHdr
 * and appends it to the buffer.
 * Returns 0 if operation was successful.
 *
 * PRE-Condition:   no memory should be allocated for ppEncoder (should be NULL)
 *                  pHeader has to contain a valid SyncHdr structure
 *                  pBufEnd must point to the end of the (WB)XML buffer
 *                  ppBufPos has to be initialized to the start point of the
 *                  (WB)XML buffer.
 *                  
 *
 * POST-Condition:  After the function call ppBufPos points to the
 *                  first free byte in the buffer behind the (WB)XML document
 *
 * IN:              enc, the encoding constant (SML_WBXML or SML_XML)
 *                  pHeader, the SyncML header structure
 *                  pBufEnd, pointer to the end of the buffer to write on
 * 
 * IN/OUT:          ppBufPos, current position of the bufferpointer
 *                  ppEncoder, the encoder object       
 *
 * RETURN:          shows error codes of function, 
 *                  0, if OK
 *                  Possible Error Codes:
 *                  HS_SML_ERR_XLT_MISSING_CONT            
 *                  HS_SML_ERR_XLT_BUF_ERR                 
 *                  SML_ERR_XLT_INVAL_ELEM_TYPE         
 *                  HS_SML_ERR_XLT_INVAL_LIST_TYPE         
 *                  HS_SML_ERR_XLT_INVAL_TAG_TYPE          
 *                  HS_SML_ERR_XLT_ENC_UNK	               
 *                  HS_SML_ERR_XLT_INVAL_PROTO_ELEM
 */
short hs_xltEncInit(SmlEncoding_t enc,
                 const SmlSyncHdrPtr_t pHeader,
                 const unsigned char* pBufEnd,
                 unsigned char* *ppBufPos,
                 XltEncoderPtr_t *ppEncoder,
                 SmlVersion_t vers);

 
short hs_xltEncReset(XltEncoderPtr_t pEncoder);

short hs_xltEncAppend(const XltEncoderPtr_t pEncoder, 
                   SmlProtoElement_t pe, 
                   const unsigned char* pBufEnd,
                   const void* pContent,
                   unsigned char* *ppBufPos);

short hs_xltEncTerminate(const XltEncoderPtr_t pEncoder,
                      const unsigned char* pBufEnd,
                      unsigned char* *ppBufPos);

short hs_xltDecInit(const SmlEncoding_t enc,
        unsigned char* pBufEnd,  //liugang delete the const symbol for reduce compiling warnings! 
        unsigned char **ppBufPos,
        XltDecoderPtr_t *ppDecoder, 
        SmlSyncHdrPtr_t *ppSyncHdr);

short hs_xltDecNext(XltDecoderPtr_t pDecoder,
        const unsigned char* pBufEnd,
        unsigned char* *ppBufPos,
        SmlProtoElement_t *pe,
        void* *ppContent);

short hs_xltDecTerminate(XltDecoderPtr_t pDecoder);

short hs_xltDecReset(XltDecoderPtr_t pDecoder);

  
/**
 * FUNCTION: hs_xltAddToBuffer
 *
 * Add a string to the global buffer
 *
 * PRE-Condition:  pContent contains some content bytes to write to the (WB) XML buffer
 *
 * POST-Condition: content is written to the buffer 
 *
 * IN:             pContent, the character pointer referencing the content to
 *                           write to the buffer
 *                 size, the content length
 * 
 * IN/OUT:         pBufMgr, pointer to a structure containing buffer management elements
 * 
 * RETURN:         shows error codes of function, 
 *                 0, if OK
 */
short hs_xltAddToBuffer(const unsigned char* pContent, long size, BufferMgmtPtr_t pBufMgr);

/**
 * FUNCTION: hs_xltUtilCreateStack
 *
 * Creates a new stack. The size parameter indicates for how many elements
 * memory should be allocated initially. This does _not_ mean that you can
 * not push more than that many element onto the stack - in that case
 * memory for another size elements is allocated.
 *
 * PRE-Condition:
 * POST-Condition:
 *                 the stack pointer points to a new, empty stack
 *
 * IN:             size, the initial size of the stack
 *
 * OUT:            a new stack
 *
 * IN/OUT:
 *
 * RETURNS:        HS_SML_ERR_NOT_ENOUGH_SPACE, if memory allocation failed
 *                 HS_SML_ERR_OK, else
 */
short hs_xltUtilCreateStack(XltUtilStackPtr_t *ppStack, const long size);

#ifdef __HS_USE_EXTENSIONS__
/* Entrypoint for SubDTD's. If we reached this point we already know 
 * a) we have data fora sub-DTD to encode and
 * b) we know which sub-DTD should be encoded.
 * So just call the appropriate sub-DTD encoder and thats it.
 */
	#ifdef __HS_USE_METINF__
	
	short hs_metinfEncBlock(XltTagID_t tagId, XltRO_t reqOptFlag, const void* pContent, SmlEncoding_t enc, BufferMgmtPtr_t pBufMgr, SmlPcdataExtension_t attFlag);

	short hs_buildMetInfMetInfCmd(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildMetInfAnchorCmd(XltDecoderPtr_t pDecoder, void* *ppElem);
	
       short hs_buildMetInfMemCmd(XltDecoderPtr_t pDecoder, void* *ppElem);
	   
	#endif
	#ifdef __HS_USE_DEVINF__
	
	short hs_devinfEncBlock(XltTagID_t tagId, XltRO_t reqOptFlag, const void* pContent, SmlEncoding_t enc, BufferMgmtPtr_t pBufMgr, SmlPcdataExtension_t attFlag);

	short hs_buildDevInfDevInfCmd(XltDecoderPtr_t pDecoder, void* *ppElem); 

	short hs_buildDevInfDevInfContent(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfDataStoreList(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfDataStoreCmd(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfXmitCmd(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfXmitList(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfDSMemCmd(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfSyncCapCmd(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfCtcap(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfExtList(XltDecoderPtr_t pDecoder, void* *ppElem);

	short hs_buildDevInfExtCmd(XltDecoderPtr_t pDecoder, void* *ppElem);

	#endif
#endif



 /*****************************codec base functions**************************/



 
 /**
 * FUNCTION: hs_getCodePage
 *
 * Returns the code page which belongs to a certain PCDATA extension type. 
 *
 * PRE-Condition:   valid PCDATA extension type
 *
 * POST-Condition:  the code page is returned
 *
 * IN:              ext, the PCDATA extension type
 * 
 * RETURN:          the code page
 */
unsigned char hs_getCodePage(SmlPcdataExtension_t ext);

/**
 * FUNCTION: getTagIDByStringAndCodepage
 *
 * Returns the tag ID which belongs to a tag string in a certain codepage
 *
 * PRE-Condition:   valid tag string, valid code page
 *
 * POST-Condition:  tag id is returned
 *
 * IN:              tag, the string representation of the tag 
 *                  cp, code page group for the tag 
 *                  pTagID, the tag id of the tag
 * 
 * RETURN:          0, if OK
 */

short hs_getTagIDByStringAndExt(char* tag, SmlPcdataExtension_t ext, XltTagID_t *pTagID);

/**
 * FUNCTION: getTagIDByByteAndCodepage
 *
 * Returns the tag ID which belongs to a tag byte in a certain codepage
 *
 * PRE-Condition:   valid tag byte, valid code page
 *
 * POST-Condition:  tag id is returned
 *
 * IN:              tag, the byte representation of the tag 
 *                  cp, code page group for the tag  
 *                  pTagID, the tag id of the tag
 * 
 * RETURN:          0, if OK
 */
short hs_getTagIDByByteAndExt(unsigned char tag, SmlPcdataExtension_t ext, XltTagID_t *pTagID);


#ifdef __HS_SML_XML__ 
/**
 * FUNCTION: hs_getTagString
 *
 * Returns a tag string which belongs to a tag ID. 
 * This function is needed for the XML encoding
 *
 * PRE-Condition:   valid tag ID, the tagSring has to be allocated 
 *
 * POST-Condition:  tag string is returned
 *
 * IN:              tagId, the ID for the tag 
 *
 * IN/OUT:          tagString, allocated string into which the XML 
 *                             tag string will be written
 * 
 * RETURN:          0,if OK
 */
short hs_getTagString(XltTagID_t tagID, char* tagString, SmlPcdataExtension_t ext);
#endif


/**
 * FUNCTION: getCpByName
 *
 * Returns the codepage constant assoziated with the name stored in 'ns'
 *
 * RETURN:             a SmlPcdataExtension_t representing the corresponding codepage id.
 *                     If no corresponding codepage is found -1 is returned.
 */
SmlPcdataExtension_t hs_getExtByName(char* ns);




 /**
 * FUNCTION: hs_wbxmlWriteTypeToBuffer
 *
 * Write a content of a certain WBXML element type (e.g. STR_I) to the global buffer
 *
 * PRE-Condition:   valid parameters 
 *
 * POST-Condition:  the content is written to the wbxml buffer with the leading
 *                  bytes for the opaque data type or the STR_I data type
 *
 * IN:              pContent, the character pointer referencing the content to
 *                            write to the buffer
 *                  elType, the element type to write to the buffer (e.g. STR_I)
 *                  size, the content length
 * 
 * IN/OUT:          pBufMgr, pointer to a structure containing buffer management elements
 * 
 * RETURN:          shows error codes of function, 
 *                  0, if OK
 */
short hs_wbxmlWriteTypeToBuffer(const unsigned char* pContent, XltElementType_t elType, long size, BufferMgmtPtr_t pBufMgr);




/**
 * FUNCTION: hs_wbxmlGenerateTag
 *
 * Generates a tag for a given tag ID and a given tag type
 *
 * PRE-Condition:   valid parameters 
 *
 * POST-Condition:  a new wbxml tag is written to the buffer
 *
 * IN:              tagId, the ID for the tag to generate (TN_ADD, ...)
 *                  tagType, the tag type (e.g. Begin Tag -> TT_BEG, ...)
 * 
 * IN/OUT:          pBufMgr, pointer to a structure containing buffer management elements
 * 
 * RETURN:          shows error codes of function, 
 *                  0, if OK
 */
short hs_wbxmlGenerateTag(XltTagID_t tagId, XltTagType_t tagType, BufferMgmtPtr_t pBufMgr );

 /**
 * FUNCTION: hs_xmlGenerateTag
 *
 * Generates a XML tag
 *
 * PRE-Condition:   valid parameters 
 *
 * POST-Condition:  the XML tag is written to the XML buffer
 *
 * IN:              tagId, the ID for the tag to generate (TN_ADD, ...)
 *                  tagType, the tag type (e.g. Begin Tag -> TT_BEG, ...)
 *                  attFlag, indicates if the encoded tag contain Attributes in namespace extensions
 *
 * IN/OUT:          pBufMgr, pointer to a structure containing buffer management elements
 * 
 * RETURN:          shows error codes of function, 
 *                  0, if OK
 */
short hs_xmlGenerateTag(XltTagID_t tagId, XltTagType_t tagType, BufferMgmtPtr_t pBufMgr, SmlPcdataExtension_t attFlag);


short hs_discardToken(XltDecoderPtr_t pDecoder);

short hs_buildPCData(const char * file,  int line, XltDecoderPtr_t pDecoder, void* *ppPCData); // chenxiang_memory 20080312

short hs_buildPCDataList(XltDecoderPtr_t pDecoder, void* *ppPCData);

short hs_buildTargetOrSource(XltDecoderPtr_t pDecoder, void* *ppTarget);

short hs_buildCred(XltDecoderPtr_t pDecoder, void* *ppCred);

short hs_buildItem(XltDecoderPtr_t pDecoder, void* *ppElem);

short hs_buildSyncHdr(XltDecoderPtr_t pDecoder, void* *ppSyncHdr);


short hs_buildGenericCmd(XltDecoderPtr_t pDecoder, void* *ppElem);

short hs_buildAlert(XltDecoderPtr_t pDecoder, void* *ppElem);

short hs_buildSync(XltDecoderPtr_t pDecoder, void* *ppElem);

short hs_buildStatus(XltDecoderPtr_t pDecoder, void* *ppElem);

short hs_buildChal(XltDecoderPtr_t pDecoder, void* *ppChal);

short hs_buildPutOrGet(XltDecoderPtr_t pDecoder, void**ppElem);

#if (defined HS_ATOMIC_RECEIVE || defined HS_SEQUENCE_RECEIVE)  
short hs_buildAtomOrSeq(XltDecoderPtr_t pDecoder, void* *ppElem);
#endif

#ifdef HS_EXEC_RECEIVE
short hs_buildExec(XltDecoderPtr_t pDecoder, void* *ppElem);
#endif

#ifdef HS_MAP_RECEIVE
short hs_buildMap(XltDecoderPtr_t pDecoder, void* *ppElem);
#endif

#ifdef HS_RESULT_RECEIVE
short hs_buildResults(XltDecoderPtr_t pDecoder, void* *ppResults);
#endif

#endif 

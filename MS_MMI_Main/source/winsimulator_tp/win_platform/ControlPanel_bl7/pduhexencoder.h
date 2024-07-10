/***************************************************************************************
** FileName    :  PduHexEncoder.h                                                     **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  22/10/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  interface for the CPduHexEncoder class.                             **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  22/10/2010     Mingxuan.Zhu         Created                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#if !defined(AFX_PDUHEXENCODER_H__66646CEE_D384_42C3_AA9F_93B25425781B__INCLUDED_)
#define AFX_PDUHEXENCODER_H__66646CEE_D384_42C3_AA9F_93B25425781B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExcelApplication.h"
#include <comutil.h>
#include <afxtempl.h>

#import <MSXML2.DLL>            // parser xml file
using namespace MSXML2;

/////////////////////////////////////////////////////////////////////////////
// Provisioning Document

/* WBXML Lib basic types redefinition */
#define WB_UTINY unsigned char
#define WB_TINY char
/** Use OMA PROV 1.1 Tables (only 'sv_prov10_attr_value_table' changed) */
#define WBXML_SUPPORT_PROV_11

/**
 * @brief The WBXML Application Token types
 */
typedef enum WBXMLTokenType_e {
    WBXML_TAG_TOKEN,        /**< Tag token */
    WBXML_ATTR_TOKEN        /**< Attribute token */
} WBXMLTokenType;

/**
 * @brief WBXML Application Token structure: Tag token
 */
typedef struct WBXMLTagEntry_s
{    
    const WB_TINY *xmlName;       /**< XML Tag Name */    
    WB_UTINY       wbxmlCodePage; /**< WBXML Code Page */
    WB_UTINY       wbxmlToken;    /**< WBXML Tag Token */
} WBXMLTagEntry;

/**
 * @brief Name Space
 * @note For SyncML, where a WBXML Code Page is associated to an XML Name Space
 */
typedef struct WBXMLNameSpaceEntry_s
{    
    const WB_TINY *xmlNameSpace;  /**< XML Name Space */   
    WB_UTINY       wbxmlCodePage; /**< WBXML Code Page */
} WBXMLNameSpaceEntry;

/**
 * @brief WBXML Application Token structure: Attribute token
 */
typedef struct WBXMLAttrEntry_s
{
    const WB_TINY *xmlName;       /**< XML Attribute Name */
    const WB_TINY *xmlValue;      /**< XML Attribute Value (may be NULL) */
    WB_UTINY       wbxmlCodePage; /**< WBXML Code Page */
    WB_UTINY       wbxmlToken;    /**< WBXML Attribute Token */
} WBXMLAttrEntry;


/**
 * @brief WBXML Application Token structure: Attribute Value token
 */
typedef struct WBXMLAttrValueEntry_s
{
    const WB_TINY *xmlName;       /**< XML Attribute Value */
    WB_UTINY       wbxmlCodePage; /**< WBXML Code Page */
    WB_UTINY       wbxmlToken;    /**< WBXML Attribute Value Token */
} WBXMLAttrValueEntry;

// WBXML keyword definition.
static const CString CHARACTERISTIC_ELEMENT = "characteristic";
static const CString PARM_ELEMENT = "parm";
static const CString CHARACTERISTIC_ELEMENT_CAC_TOKEN = "C6";  // contains attribute and content : 11xx xxxx
static const CString TOKEN_OF_UNKNOWN_TYPE_ATTRIBUTE = "50";
static const CString PARM_ELEMENT_NCAC_TOKEN = "87";           // not contains attribute and content : 10xx xxxx
static const CString TOKEN_OF_NAME_ATTRIBUTE = "05";
static const CString TOKEN_OF_VALUE_ATTRIBUTE = "06";
static const CString START_TOKEN_OF_INLINE_STR_CONTENT = "03";
static const CString END_TOKEN_OF_INLINE_STR_CONTENT = "00";
static const CString END_TOKEN_OF_ATTRIBUTE = "01";
static const CString CODE_PAGE_ONE_TO_CODE_PAGE_ZERO = "0000";
static const CString CODE_PAGE_ZERO_TO_CODE_PAGE_ONE = "0001";
static const CString MAX_HEX_LENGTH_OF_TP_USER_DATA = "8c";
static const _bstr_t WAP_PROVISIONINGDOC_ATTR_VALUE = "version";
static const _bstr_t CHARACTERISTIC_ATTR_VALUE = "type";
static const _bstr_t PARM_NAME_ATTR_VALUE = "name";
static const _bstr_t PARM_VALUE_ATTR_VALUE = "value";
static const int MAX_LENGTH_OF_TP_USER_DATA = 128; // by octet
static const int WBXML_TOKEN_LENGTH = 2;

// the .xls file's settings
static const CString OTA_TEST_CASE_SHEET_NAME = "OTA";
static const CString THE_COL_NAME_OF_EXPECTED_RESULT = "Expected Results";
static const CString THE_COL_NAME_OF_ACTUAL_RESULT = "Actual Result";
static const CString PRE_PROCESSED_XML_FILE = ".\\currentTemp.xml";
static const CString WBXML_TEST_CASE_FILE_NAME = "wbxml_test_cases";
static const CString CASE_ID_COL_NAME = "Case ID";
static const CString XML_FILE_NAME_COL_NAME = "XML FileName";
static const CString TYPE_COL_NAME = "Type";
static const CString PDU_COL_NAME = "Pdu";
static const CString CONCLUSION_COL_NAME = "Conclusion";
static const int PDU_MAX_LENGTH_COL_NAME = 12;
static const short TITLE_RANGE_BG_COLOR = 6;  // yellow
static const short ERROR_RANGE_BG_COLOR = 3;  // red

// RP-User data constant definition
/* RP-User data element Sample
                                                               140
                                                                |
  Integer representation of useful semi-octets                  |                               TP User Data
          |                        TP data coding scheme   TP User data Length                         |
          |          BCD Number         |                       |         TP User data header          |
          |     |-----------------|     |                       |     |-----------------------------|  |
   a0 40 0d 91 68 31 67 81 94 34 f8 00 f5 01 90 20 41 62 25 23 8c 0b 05 04 0b 84 23 f0 00 03 65 03 01 65 
    |        |                       |     |-----------------|     |                    |-----------|
   160       |                       |      TP SC time stamp   TP UD Header Length        long SMS concatenate
             |              TP protocol identifier
             |
            91: International Number
            80: Unknown Number
   
   Comment1: TP User data Length is volatile. 
   Comment2: 65 03 01 is is volatile. 
*/
static const CString HEADER_TOKEN_OF_RP_USER_DATA_CONTENT = "400d91683119325476f800f501909090819123";
static const CString HEADER_TOKEN_OF_TP_USER_DATA_CONTENT = "0b05040b8423f0000365";
static const CString HEADER_TOKEN_OF_WBXML_DATA = "650601B6030B6A0045";
static const int HEADER_LENGTH_OF_TP_USER_DATA_CONTENT = 24; // contains TP UD Header Length.

/**********************************************************
 *    PROV 1.0
 *      WAP 2.0: "WAP-183-PROVCONT-20010724-a.pdf"
 *      OMA: "OMA-WAP-ProvCont-v1_1-20021112-C.PDF"
 *
 *    PROV 1.1
 *      OMA: OMA-WAP-ProvCont-v1_1-20050428-Cchangebars.doc
 *      There is no new Public ID defined for this new version,
 *      so how should we handle this ??
 */

const WBXMLTagEntry sv_prov10_tag_table[] = {
    { "wap-provisioningdoc",        0x00, 0x05 },
    { "characteristic",             0x00, 0x06 },
    { "parm",                       0x00, 0x07 },
    
    { "characteristic",             0x01, 0x06 }, /* OMA */
    { "parm",                       0x01, 0x07 }, /* OMA */
    { NULL,                         0x00, 0x00 }
};


const WBXMLAttrEntry sv_prov10_attr_table[] = {
    /* Wap-provisioningdoc */
    { "version",    NULL,               0x00, 0x45 },
    { "version",    "1.0",              0x00, 0x46 },

    /* Characteristic */
    { "type",        NULL,                  0x00, 0x50 },
    { "type",        "PXLOGICAL",           0x00, 0x51 },
    { "type",        "PXPHYSICAL",          0x00, 0x52 },
    { "type",        "PORT",                0x00, 0x53 },
    { "type",        "VALIDITY",            0x00, 0x54 },
    { "type",        "NAPDEF",              0x00, 0x55 },
    { "type",        "BOOTSTRAP",           0x00, 0x56 },
    { "type",        "VENDORCONFIG",        0x00, 0x57 },
    { "type",        "CLIENTIDENTITY",      0x00, 0x58 },
    { "type",        "PXAUTHINFO",          0x00, 0x59 },
    { "type",        "NAPAUTHINFO",         0x00, 0x5a },
    { "type",        "ACCESS",              0x00, 0x5b }, /* OMA */
    
    { "type",        NULL,                  0x01, 0x50 }, /* OMA */
    { "type",        "PORT",                0x01, 0x53 }, /* OMA */
    { "type",        "CLIENTIDENTITY",      0x01, 0x58 }, /* OMA */
    { "type",        "APPLICATION",         0x01, 0x55 }, /* OMA */
    { "type",        "APPADDR",             0x01, 0x56 }, /* OMA */
    { "type",        "APPAUTH",             0x01, 0x57 }, /* OMA */
    { "type",        "RESOURCE",            0x01, 0x59 }, /* OMA */

    /* Parm */
    { "name",        NULL,                  0x00, 0x05 },
    { "value",       NULL,                  0x00, 0x06 },
    { "name",        "NAME",                0x00, 0x07 },
    { "name",        "NAP-ADDRESS",         0x00, 0x08 },
    { "name",        "NAP-ADDRTYPE",        0x00, 0x09 },
    { "name",        "CALLTYPE",            0x00, 0x0a },
    { "name",        "VALIDUNTIL",          0x00, 0x0b },
    { "name",        "AUTHTYPE",            0x00, 0x0c },
    { "name",        "AUTHNAME",            0x00, 0x0d },
    { "name",        "AUTHSECRET",          0x00, 0x0e },
    { "name",        "LINGER",              0x00, 0x0f },
    { "name",        "BEARER",              0x00, 0x10 },
    { "name",        "NAPID",               0x00, 0x11 },
    { "name",        "COUNTRY",             0x00, 0x12 },
    { "name",        "NETWORK",             0x00, 0x13 },
    { "name",        "INTERNET",            0x00, 0x14 },
    { "name",        "PROXY-ID",            0x00, 0x15 },
    { "name",        "PROXY-PROVIDER-ID",   0x00, 0x16 },
    { "name",        "DOMAIN",              0x00, 0x17 },
    { "name",        "PROVURL",             0x00, 0x18 },
    { "name",        "PXAUTH-TYPE",         0x00, 0x19 },
    { "name",        "PXAUTH-ID",           0x00, 0x1a },
    { "name",        "PXAUTH-PW",           0x00, 0x1b },
    { "name",        "STARTPAGE",           0x00, 0x1c },
    { "name",        "BASAUTH-ID",          0x00, 0x1d },
    { "name",        "BASAUTH-PW",          0x00, 0x1e },
    { "name",        "PUSHENABLED",         0x00, 0x1f },
    { "name",        "PXADDR",              0x00, 0x20 },
    { "name",        "PXADDRTYPE",          0x00, 0x21 },
    { "name",        "TO-NAPID",            0x00, 0x22 },
    { "name",        "PORTNBR",             0x00, 0x23 },
    { "name",        "SERVICE",             0x00, 0x24 },
    { "name",        "LINKSPEED",           0x00, 0x25 },
    { "name",        "DNLINKSPEED",         0x00, 0x26 },
    { "name",        "LOCAL-ADDR",          0x00, 0x27 },
    { "name",        "LOCAL-ADDRTYPE",      0x00, 0x28 },
    { "name",        "CONTEXT-ALLOW",       0x00, 0x29 },
    { "name",        "TRUST",               0x00, 0x2a },
    { "name",        "MASTER",              0x00, 0x2b },
    { "name",        "SID",                 0x00, 0x2c },
    { "name",        "SOC",                 0x00, 0x2d },
    { "name",        "WSP-VERSION",         0x00, 0x2e },
    { "name",        "PHYSICAL-PROXY-ID",   0x00, 0x2f },
    { "name",        "CLIENT-ID",           0x00, 0x30 },
    { "name",        "DELIVERY-ERR-SDU",    0x00, 0x31 },
    { "name",        "DELIVERY-ORDER",      0x00, 0x32 },
    { "name",        "TRAFFIC-CLASS",       0x00, 0x33 },
    { "name",        "MAX-SDU-SIZE",        0x00, 0x34 },
    { "name",        "MAX-BITRATE-UPLINK",  0x00, 0x35 },
    { "name",        "MAX-BITRATE-DNLINK",  0x00, 0x36 },
    { "name",        "RESIDUAL-BER",        0x00, 0x37 },
    { "name",        "SDU-ERROR-RATIO",     0x00, 0x38 },
    { "name",        "TRAFFIC-HANDL-PRIO",  0x00, 0x39 },
    { "name",        "TRANSFER-DELAY",      0x00, 0x3a },
    { "name",        "GUARANTEED-BITRATE-UPLINK",   0x00, 0x3b },
    { "name",        "GUARANTEED-BITRATE-DNLINK",   0x00, 0x3c },
    { "name",        "PXADDR-FQDN",         0x00, 0x3d }, /* OMA */
    { "name",        "PROXY-PW",            0x00, 0x3e }, /* OMA */
    { "name",        "PPGAUTH-TYPE",        0x00, 0x3f }, /* OMA */
    { "name",        "PULLENABLED",         0x00, 0x47 }, /* OMA */
    { "name",        "DNS-ADDR",            0x00, 0x48 }, /* OMA */
    { "name",        "MAX-NUM-RETRY",       0x00, 0x49 }, /* OMA */
    { "name",        "FIRST-RETRY-TIMEOUT", 0x00, 0x4a }, /* OMA */
    { "name",        "REREG-THRESHOLD",     0x00, 0x4b }, /* OMA */
    { "name",        "T-BIT",               0x00, 0x4c }, /* OMA */
    { "name",        "AUTH-ENTITY",         0x00, 0x4e }, /* OMA */
    { "name",        "SPI",                 0x00, 0x4f }, /* OMA */
    
    { "name",        NULL,                  0x01, 0x05 }, /* OMA */
    { "value",       NULL,                  0x01, 0x06 }, /* OMA */
    { "name",        "NAME",                0x01, 0x07 }, /* OMA */
    { "name",        "INTERNET",            0x01, 0x14 }, /* OMA */
    { "name",        "STARTPAGE",           0x01, 0x1c }, /* OMA */
    { "name",        "TO-NAPID",            0x01, 0x22 }, /* OMA */
    { "name",        "PORTNBR",             0x01, 0x23 }, /* OMA */
    { "name",        "SERVICE",             0x01, 0x24 }, /* OMA */
    { "name",        "AACCEPT",             0x01, 0x2e }, /* OMA */
    { "name",        "AAUTHDATA",           0x01, 0x2f }, /* OMA */
    { "name",        "AAUTHLEVEL",          0x01, 0x30 }, /* OMA */
    { "name",        "AAUTHNAME",           0x01, 0x31 }, /* OMA */
    { "name",        "AAUTHSECRET",         0x01, 0x32 }, /* OMA */
    { "name",        "AAUTHTYPE",           0x01, 0x33 }, /* OMA */
    { "name",        "ADDR",                0x01, 0x34 }, /* OMA */
    { "name",        "ADDRTYPE",            0x01, 0x35 }, /* OMA */
    { "name",        "APPID",               0x01, 0x36 }, /* OMA */
    { "name",        "APROTOCOL",           0x01, 0x37 }, /* OMA */
    { "name",        "PROVIDER-ID",         0x01, 0x38 }, /* OMA */
    { "name",        "TO-PROXY",            0x01, 0x39 }, /* OMA */
    { "name",        "URI",                 0x01, 0x3a }, /* OMA */
    { "name",        "RULE",                0x01, 0x3b }, /* OMA */
    
    { NULL,          NULL,                  0x00, 0x00 }
};

//
const WBXMLAttrValueEntry sv_prov10_attr_value_table[] = {
    /* ADDRTYPE */
    { "IPV4",                   0x00, 0x85 },
    { "IPV6",                   0x00, 0x86 },
    { "E164",                   0x00, 0x87 },
    { "ALPHA",                  0x00, 0x88 },
    { "APN",                    0x00, 0x89 },
    { "SCODE",                  0x00, 0x8a },
    { "TETRA-ITSI",             0x00, 0x8b },
    { "MAN",                    0x00, 0x8c },
    
    { "IPV6",                   0x01, 0x86 }, /* OMA */
    { "E164",                   0x01, 0x87 }, /* OMA */
    { "ALPHA",                  0x01, 0x88 }, /* OMA */
    { "APPSRV",                 0x01, 0x8d }, /* OMA */
    { "OBEX",                   0x01, 0x8e }, /* OMA */

    /* CALLTYPE */
    { "ANALOG-MODEM",           0x00, 0x90 },
    { "V.120",                  0x00, 0x91 },
    { "V.110",                  0x00, 0x92 },
    { "X.31",                   0x00, 0x93 },
    { "BIT-TRANSPARENT",        0x00, 0x94 },
    { "DIRECT-ASYNCHRONOUS-DATA-SERVICE",    0x00, 0x95 },

    /* AUTHTYPE/PXAUTH-TYPE */
    { "PAP",                    0x00, 0x9a },
    { "CHAP",                   0x00, 0x9b },
    { "HTTP-BASIC",             0x00, 0x9c },
    { "HTTP-DIGEST",            0x00, 0x9d },
    { "WTLS-SS",                0x00, 0x9e },
    { "MD5",                    0x00, 0x9f }, /* OMA */

    /* BEARER */
    { "GSM-USSD",               0x00, 0xa2 },
    { "GSM-SMS",                0x00, 0xa3 },
    { "ANSI-136-GUTS",          0x00, 0xa4 },
    { "IS-95-CDMA-SMS",         0x00, 0xa5 },
    { "IS-95-CDMA-CSD",         0x00, 0xa6 },
    { "IS-95-CDMA-PACKET",      0x00, 0xa7 },
    { "ANSI-136-CSD",           0x00, 0xa8 },
    { "ANSI-136-GPRS",          0x00, 0xa9 },
    { "GSM-CSD",                0x00, 0xaa },
    { "GSM-GPRS",               0x00, 0xab },
    { "AMPS-CDPD",              0x00, 0xac },
    { "PDC-CSD",                0x00, 0xad },
    { "PDC-PACKET",             0x00, 0xae },
    { "IDEN-SMS",               0x00, 0xaf },
    { "IDEN-CSD",               0x00, 0xb0 },
    { "IDEN-PACKET",            0x00, 0xb1 },
    { "FLEX/REFLEX",            0x00, 0xb2 },
    { "PHS-SMS",                0x00, 0xb3 },
    { "PHS-CSD",                0x00, 0xb4 },
    { "TRETRA-SDS",             0x00, 0xb5 },
    { "TRETRA-PACKET",          0x00, 0xb6 },
    { "ANSI-136-GHOST",         0x00, 0xb7 },
    { "MOBITEX-MPAK",           0x00, 0xb8 },
    { "CDMA2000-1X-SIMPLE-IP",  0x00, 0xb9 }, /* OMA */
    { "CDMA2000-1X-MOBILE-IP",  0x00, 0xba }, /* OMA */

    /* LINKSPEED */
    { "AUTOBAUDING",            0x00, 0xc5 },

    /* SERVICE */
    { "CL-WSP",                 0x00, 0xca },
    { "CO-WSP",                 0x00, 0xcb },
    { "CL-SEC-WSP",             0x00, 0xcc },
    { "CO-SEC-WSP",             0x00, 0xcd },
    { "CL-SEC-WTA",             0x00, 0xce },
    { "CO-SEC-WTA",             0x00, 0xcf },
    { "OTA-HTTP-TO",            0x00, 0xd0 }, /* OMA */
    { "OTA-HTTP-TLS-TO",        0x00, 0xd1 }, /* OMA */
    { "OTA-HTTP-PO",            0x00, 0xd2 }, /* OMA */
    { "OTA-HTTP-TLS-PO",        0x00, 0xd3 }, /* OMA */
    
    /* AAUTHTYPE */
#if defined( WBXML_SUPPORT_PROV_11 )
    
    { ",",                      0x01, 0x90 }, /* OMA */
    { "HTTP-",                  0x01, 0x91 }, /* OMA */
    { "BASIC",                  0x01, 0x92 }, /* OMA */
    { "DIGEST",                 0x01, 0x93 }, /* OMA */
    
#else

    { ",",                      0x01, 0x80 }, /* OMA */
    { "HTTP-",                  0x01, 0x81 }, /* OMA */
    { "BASIC",                  0x01, 0x82 }, /* OMA */
    { "DIGEST",                 0x01, 0x83 }, /* OMA */
    
#endif /* WBXML_SUPPORT_PROV_11 */
    
    /* AUTH-ENTITY */
    { "AAA",                    0x00, 0xe0 }, /* OMA */
    { "HA",                     0x00, 0xe1 }, /* OMA */

    { NULL,                     0x00, 0x00 }
};


/////////////////////////////////////////////////////////////////////////////
// CPduHexEncoder interface
class CPduHexEncoder  
{
public:
	CPduHexEncoder();
	virtual ~CPduHexEncoder();

    // 
    void LoadAllXmlFile(CString &sFolderName);
    CString GetTimeStamp();
    int GetErrorFormatSwitch();

private:
    void PreProcessXmlFile(CString &sXmlFilePath);
    void SetStrTableOffsetValue(CString &sStrTableMapKey);
    BOOLEAN ProcessXmlFile( CString &sXmlFilePath, 
                            CStringArray &aHexCode );
    CString ParseXMLTree( MSXML2::IXMLDOMNodePtr pXmlNode, 
                          CString &sWBXMLCode );
    BOOLEAN IsFixedAttrValue(const CString &parmName);
    CString EncodeCharacteristicTypeHexValue(CString &characteristicType);
    CString EncodeParmNameHexValue(CString &parmName);
    CString EncodeParmValueHexValue( const CString &parmName, 
                                     CString &parmValue);
    CString GenerateXlsFileName(void);
    void SetXlsTestCaseFileTitle(void);
    void GenerateXlsTestCaseFile( const CStringArray &sHexPduSet, 
                                  const CString &sXmlFileName
                                  //const CString &sInternetAccountInfo 
                                );
    CString GetSeqHexNumberOfSMS( int seqNumberOfSMS, 
                                  int maxNumberofSMS );
    CString ConvertDecNum2DecStr(int decimalNumber);
    CString ConvertDecNum2HexStr(int decimalNumber);
    CString ConvertStr2HexStr(CString &sAttrValueString);
    CString ConcatenateSMS( CString &sSeqNoPdu, 
                            CString &tpUserData );
    BOOLEAN IsInternetAccountNAPType(const CString &CharTypeAttrValue);
    BOOLEAN IsInternetAccountPXLType(const CString &CharTypeAttrValue);
    void InitInternetAccountInfo(int nPxLogicalNumber);
    void GetInternetAccountNAPSetting( const CString &CharTypeAttrValue, 
                                       const CString &ParmNameAttrValue, 
                                       const CString &ParmValueAttrValue );
    void GetInternetAccountPXLSetting( const CString &CharTypeAttrValue, 
                                       const CString &ParmNameAttrValue, 
                                       const CString &ParmValueAttrValue );
    CString GetInternetAccountInfo(void);

private:
    CExcelApplication m_pExcelApp;
    CMap<CString, LPCTSTR, CString, LPCTSTR> m_mInternetAccountKeySettingInfo;
    CStringArray m_aInternetAccountInfo;
    CStringArray m_aInternetAccountPXLInfo;
    int m_iCodePage;
    int m_iCharacteristicNodeSwitch;
    int m_nXmlFileCount;
    int m_nPxLogicalNumber;
    int m_iErrorFormatSwitch;
    long m_lPduStartColNo;
    long m_lExpectedResultColNo;
    long m_iSheetRowNo;
    long m_iSheetColNo;
};

#endif // !defined(AFX_PDUHEXENCODER_H__66646CEE_D384_42C3_AA9F_93B25425781B__INCLUDED_)
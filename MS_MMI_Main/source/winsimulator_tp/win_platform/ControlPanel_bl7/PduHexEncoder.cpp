/***************************************************************************************
** FileName    :  PduHexEncoder.cpp                                                   **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  22/10/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  implementation of the CPduHexEncoder class.                         **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  22/10/2010     Mingxuan.Zhu         Created                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#include "stdafx.h"
#include "PduHexEncoder.h"
#include "ConPanCfgAccess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPduHexEncoder::CPduHexEncoder()
{
    // member variable initialization.
    m_iCodePage = 0;
    m_nXmlFileCount = 1;
    m_iCharacteristicNodeSwitch = 0;
    m_iSheetRowNo = 1;  // start row
    m_iSheetColNo = 1;  // start column
    m_lPduStartColNo = 0;
    m_lExpectedResultColNo = 0;
    m_nPxLogicalNumber = 0;
    m_iErrorFormatSwitch = 0;
}

CPduHexEncoder::~CPduHexEncoder()
{

}

/**************************************************************************************/
// Description      : load all .xml file.
//--------------------------------------------------------------------------------------
// Input Parameter  : CString &sFolderName  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CPduHexEncoder::LoadAllXmlFile(CString &sFolderName)  // IN:Param
{
    CStdioFile pProcessedXmlFile;
    CFileFind pXmlFileList;
    CString sXmlFileName ="";
    CString sXmlFilePath = sFolderName + "\\" + "*.xml";
    CString sPreprocessXmlFilePath = PRE_PROCESSED_XML_FILE;
    BOOLEAN bIsExistence = pXmlFileList.FindFile(sXmlFilePath);
    CStringArray aHexCodeOfPdu;
    CString sInternetAccountInfoSetting = "";

    m_nXmlFileCount = 1; // initialize xml file number.
    m_pExcelApp.CreateExcelObject();  // start excel server.
    // get the start column of the pdu in the test case file that format is .xls
    SetXlsTestCaseFileTitle();

    while(bIsExistence)
    {      
        bIsExistence = pXmlFileList.FindNextFile();
        sXmlFileName = pXmlFileList.GetFileName();
        sXmlFilePath = sFolderName + "\\" + sXmlFileName;
        //TRACE("==== FileName = %s ==== START ====\n",sXmlFileName);
        PreProcessXmlFile(sXmlFilePath);
        ProcessXmlFile(sPreprocessXmlFilePath,aHexCodeOfPdu);
        pProcessedXmlFile.Remove(sPreprocessXmlFilePath);
        //sInternetAccountInfoSetting = GetInternetAccountInfo();
        GenerateXlsTestCaseFile(aHexCodeOfPdu,sXmlFileName/*,sInternetAccountInfoSetting*/);
        m_nXmlFileCount++;
        aHexCodeOfPdu.RemoveAll();
        //TRACE("==== FileName = %s ==== END ====\n",sXmlFileName);
    }

    m_pExcelApp.ReleaseExcelObjects();  // close excel server.
    //m_mInternetAccountKeySettingInfo.RemoveAll();
    //m_aInternetAccountPXLInfo.RemoveAll();
    //m_aInternetAccountInfo.RemoveAll();
    
}

/**************************************************************************************/
// Description      : pre process the xml file ( filter the comments ).
//--------------------------------------------------------------------------------------
// Input Parameter  : the path of the file that format is xml  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CPduHexEncoder::PreProcessXmlFile(CString &sXmlFilePath)
{
    CString sModifiedXmlFile = "";
    CString sXmlContentPerLine = "";
    CStdioFile pXmlFile;
    CStdioFile pProcessedXmlFile;

    pProcessedXmlFile.Open(PRE_PROCESSED_XML_FILE,CFile::modeCreate|CFile::modeWrite,NULL);
    m_nPxLogicalNumber = 0;

    if(pXmlFile.Open(sXmlFilePath,CFile::modeRead,NULL))
    {
        while(pXmlFile.ReadString(sXmlContentPerLine))
        {
            if(-1 == sXmlContentPerLine.Find("<!"))
            {   
                sXmlContentPerLine = sXmlContentPerLine + "\n";
                pProcessedXmlFile.WriteString(sXmlContentPerLine);
                // count the number of the keyword named PXLOGICAL.
                if(sXmlContentPerLine.Find("PXLOGICAL") >0)
                {
                    m_nPxLogicalNumber++;
                }
            }
            else
            {
                pProcessedXmlFile.WriteString("\n");
            }
        }
        pXmlFile.Close();       
    }
    else
    {
        AfxMessageBox("This xml file dose not exist.");
        return;
    }

    pProcessedXmlFile.Close();
    // initialize the Internet Account Information.
    //InitInternetAccountInfo(m_nPxLogicalNumber);
}

/**************************************************************************************/
// Description      : set string table offset value.
//--------------------------------------------------------------------------------------
// Input Parameter  : the key of the string table map  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CPduHexEncoder::SetStrTableOffsetValue(CString &sStrTableMapKey)
{
    CString sStrMapKey = sStrTableMapKey;
    CString sStrMapValue = "";
    // TODO : ADD CODE HERE
}

/**************************************************************************************/
// Description      : process .xml file.
//--------------------------------------------------------------------------------------
// Input Parameter  : the path of the processed xml file
// Output Parameter : the encoded pdu sequence which is divided.
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOLEAN CPduHexEncoder::ProcessXmlFile(
                                        CString &sXmlFilePath,  // IN:Param
                                        CStringArray &aHexCode  // OUT:Param
                                      )
{
    MSXML2::IXMLDOMDocument2Ptr pXMLDoc;
    MSXML2::IXMLDOMNodeListPtr pXmlNodeList;
    MSXML2::IXMLDOMNodePtr pXmlRootNode;
    MSXML2::IXMLDOMNodePtr pRootNodeAttr;
    MSXML2::IXMLDOMNodePtr pXmlCurNode;
    MSXML2::IXMLDOMNamedNodeMapPtr pXmlRootNodeAttr;
    
    _bstr_t sRootName = "";
    _bstr_t sRootAttrValue = "";
    
    CString sRootNodeAttrValue = "";
    CString sHexCode = "";
    CString sNumberOfSMS = "";
    CString sConcatedSMS = "";
    
    int nPduNumber = 0;
    int nOctetNumber = 0;
    
    HRESULT hr = pXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
    if(!SUCCEEDED(hr))
    {
        AfxMessageBox("fail to open XML file!");
    }
    
    // load xml file and select root node.
    pXMLDoc->load((_variant_t)sXmlFilePath);
    
    if(NULL == pXMLDoc->documentElement)
    {
        return FALSE;
    }
    else
    {
        sRootName = pXMLDoc->documentElement->GetbaseName();  // LABEL = wap-provisioningdoc
        pXmlNodeList = pXMLDoc->selectNodes(sRootName);
        pXmlRootNode = pXmlNodeList->nextNode();
        pXmlRootNodeAttr = pXmlRootNode->Getattributes();
        pRootNodeAttr = pXmlRootNodeAttr->getNamedItem(WAP_PROVISIONINGDOC_ATTR_VALUE);
        
        if(NULL != pRootNodeAttr)
        {
            sRootAttrValue = pRootNodeAttr->Gettext();
            sRootNodeAttrValue = sRootAttrValue.operator char*();
        }
        
        pXmlCurNode = pXmlRootNode->GetfirstChild();
        
        ParseXMLTree(pXmlCurNode, sHexCode);
        
        sHexCode = HEADER_TOKEN_OF_WBXML_DATA + sHexCode + END_TOKEN_OF_ATTRIBUTE; // the end of wap-provisioningdoc
        nOctetNumber = sHexCode.GetLength() / 2;
        if((nOctetNumber % MAX_LENGTH_OF_TP_USER_DATA))
        {
            nPduNumber = nOctetNumber / MAX_LENGTH_OF_TP_USER_DATA + 1;
        }
        else
        {
            nPduNumber = nOctetNumber / MAX_LENGTH_OF_TP_USER_DATA;
        }
        
        for(int iPduNo=0; iPduNo < nPduNumber; iPduNo++)
        {        
            sNumberOfSMS = GetSeqHexNumberOfSMS((iPduNo+1),nPduNumber);
            sConcatedSMS = ConcatenateSMS(sNumberOfSMS, sHexCode.Left(MAX_LENGTH_OF_TP_USER_DATA*2));
            aHexCode.Add(sConcatedSMS);
            //TRACE("==>%s\n",aHexCode.GetAt(iPduNo));
            sHexCode = sHexCode.Right(sHexCode.GetLength() - MAX_LENGTH_OF_TP_USER_DATA*2);
        }
        return TRUE;
    }
}

/**************************************************************************************/
// Description      : get the concatenated hex header of the pdu sequence.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. int seqNumberOfSMS  
//                    2. int maxNumberofSMS
// Output Parameter : NONE
// Return Value     : the concatenated hex header of the pdu sequence.
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note: the position of the sequence
//
//             TP User data header
//      |-----------------------------|
//     05 04 0b 84 23 f0 00 03 65 03 01
//                                 |--| --> the concatenated hex header
/**************************************************************************************/
CString CPduHexEncoder::GetSeqHexNumberOfSMS(int seqNumberOfSMS, int maxNumberofSMS)
{
    CString sMaxHexNumOfSMS = "";
    CString sSeqHexNumOfSMS = "";
    CString sHexNumberOfSMS = "";

    sMaxHexNumOfSMS = ConvertDecNum2HexStr(maxNumberofSMS); // the max length of the pdu sequence 
    sSeqHexNumOfSMS = ConvertDecNum2HexStr(seqNumberOfSMS); // the order No of the pdu sequence 
    sHexNumberOfSMS = sMaxHexNumOfSMS + sSeqHexNumOfSMS;
    
    return sHexNumberOfSMS;
}

/**************************************************************************************/
// Description      : convert decimal number to the decimal string.
//--------------------------------------------------------------------------------------
// Input Parameter  : int decimalNumber  
// Output Parameter : NONE
// Return Value     : decimal string
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::ConvertDecNum2DecStr(int decimalNumber)
{
    char *sPreDecNumber = NULL;
    CString sDecNumber = "";
    
    sPreDecNumber = (char*)malloc(4*sizeof(char));
    if(NULL == sPreDecNumber)
    {
        AfxMessageBox("The heap memory is not available!");
    }
    else
    {
        memset(sPreDecNumber,0,4*sizeof(char));
        
        itoa(decimalNumber,sPreDecNumber,10);
        sDecNumber.Format("%s",sPreDecNumber);
        
        free(sPreDecNumber);
    }  
    return sDecNumber;
}

/**************************************************************************************/
// Description      : convert the decimal number to the hexadecimal string.
//--------------------------------------------------------------------------------------
// Input Parameter  : int decimalNumber  
// Output Parameter : NONE
// Return Value     : the hexadecimal string
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::ConvertDecNum2HexStr(int decimalNumber)
{
    char *sPreHexNumber = NULL;
    CString sHexNumber = "";
    
    sPreHexNumber = (char*)malloc(4*sizeof(char));

    if(NULL == sPreHexNumber)
    {
        AfxMessageBox("The heap memory is not available!");
    }
    else
    {
        memset(sPreHexNumber,0,4*sizeof(char));
        itoa(decimalNumber,sPreHexNumber,16);
        sHexNumber.Format("%s",sPreHexNumber);
        
        free(sPreHexNumber);

        if(sHexNumber.GetLength() < 2)
        {
            sHexNumber = "0" + sHexNumber;
        }
    }
    return sHexNumber;
}

/**************************************************************************************/
// Description      : concatenate the user data header and the divided user data.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. the user data header
//                    2. the divided user data  
// Output Parameter : NONE
// Return Value     : the concatenated the pdu sequence.
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::ConcatenateSMS( CString &sSeqNoPdu,  // IN:Param
                                        CString &tpUserData  // IN:Param
                                      )
{
    CString sPreUnconcatPdu = tpUserData;
    CString sSeqNumberPdu = sSeqNoPdu;
    
    CString sConcatedPdu = "";

    if(sPreUnconcatPdu.GetLength()==(MAX_LENGTH_OF_TP_USER_DATA*2))
    {
        sConcatedPdu = HEADER_TOKEN_OF_RP_USER_DATA_CONTENT + MAX_HEX_LENGTH_OF_TP_USER_DATA 
                     + HEADER_TOKEN_OF_TP_USER_DATA_CONTENT + sSeqNumberPdu + sPreUnconcatPdu;        
    }
    else
    {
        CString sLengthOfLastPdu = "";
        int nLengthOfLastPdu = (sPreUnconcatPdu.GetLength()+HEADER_LENGTH_OF_TP_USER_DATA_CONTENT) / 2;
        
        sLengthOfLastPdu = ConvertDecNum2HexStr(nLengthOfLastPdu);
        sConcatedPdu = HEADER_TOKEN_OF_RP_USER_DATA_CONTENT + sLengthOfLastPdu 
                     + HEADER_TOKEN_OF_TP_USER_DATA_CONTENT + sSeqNumberPdu + sPreUnconcatPdu;
    }
    return sConcatedPdu;
}

/**************************************************************************************/
// Description      : parser the tree of the xml file.
//--------------------------------------------------------------------------------------
// Input Parameter  : MSXML2::IXMLDOMNodePtr pXmlNode  
// Output Parameter : CString &sWBXMLCode
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::ParseXMLTree( 
                                      MSXML2::IXMLDOMNodePtr pCurNode, // IN:Param
                                      CString &sWBXMLCode              // OUT:Param
                                    )
{
    // variable initialization
    MSXML2::IXMLDOMNamedNodeMapPtr pNodeAttr;
    MSXML2::IXMLDOMNodePtr pCharacteristicNodeTypeAttr;
    MSXML2::IXMLDOMNodePtr pParmNodeNameAttr;
    MSXML2::IXMLDOMNodePtr pParmNodeValueAttr;
    MSXML2::IXMLDOMNodePtr pSibNode;
    MSXML2::IXMLDOMNodePtr pSubNode;
    
    _bstr_t sSibName = "";
    _bstr_t sSubName = "";
    _bstr_t sSibAttrValue = "";
    _bstr_t sSubNameAttrValue = "";
    _bstr_t sSubValueAttrValue = "";

    CString sSibNodeName = "";
    CString sSubNodeName = "";
    CString sSibNodeAttrValue = "";
    CString sSubNodeNameAttrValue = "";
    CString sSubNodeValueAttrValue = "";
    CString sSibNodeAttrHexValue = "";
    CString sSubNodeNameAttrHexValue = "";
    CString sSubNodeValueAttrHexValue = "";

    //
    pSibNode = pCurNode;

    while(NULL != pSibNode)
    {
        sSibName = pSibNode->GetnodeName();
        sSibNodeName = sSibName.operator char*();

        if(sSibNodeName.Compare(CHARACTERISTIC_ELEMENT)==0)
        {
            sWBXMLCode = sWBXMLCode + CHARACTERISTIC_ELEMENT_CAC_TOKEN;
            //TRACE("-->:<%s ",sSibNodeName);
            
            pNodeAttr = pSibNode->Getattributes();
            // get the value of the type attribute in characteristic element. 
            pCharacteristicNodeTypeAttr = pNodeAttr->getNamedItem(CHARACTERISTIC_ATTR_VALUE);
            if(NULL != pCharacteristicNodeTypeAttr)
            {
                sSibAttrValue = pCharacteristicNodeTypeAttr->Gettext();
                sSibNodeAttrValue = sSibAttrValue.operator char*();
                sSibNodeAttrHexValue = EncodeCharacteristicTypeHexValue(sSibNodeAttrValue);
                sWBXMLCode = sWBXMLCode + sSibNodeAttrHexValue + END_TOKEN_OF_ATTRIBUTE;
                //TRACE("type=\"%s\">\n",sSibNodeAttrValue);
                //TRACE("typeHex = %s\n",sSibNodeAttrHexValue);
            }
            else
            {
                AfxMessageBox("This characteristic does not contain the attribute of the type!\n");      
            }
            pSubNode = pSibNode->GetfirstChild();
        }
        else
        {
            pSubNode = pSibNode;
            //pSibNode = pSibNode->GetnextSibling();
            m_iCharacteristicNodeSwitch = 1;
        }     

        while(NULL != pSubNode)
        {
            sSubName = pSubNode->GetnodeName();
            sSubNodeName = sSubName.operator char*();
            
            if(sSubNodeName.Compare(PARM_ELEMENT)==0)
            {
                if(m_iCharacteristicNodeSwitch == 1) // count parm number
                {
                    pSibNode = pSibNode->GetnextSibling();
                }
                
                // get the pointer of the attributes in param element.
                pNodeAttr = pSubNode->Getattributes();
                //TRACE("-->:<%s",sSubNodeName);

                // get the value of the name attribute in param element. 
                pParmNodeNameAttr = pNodeAttr->getNamedItem(PARM_NAME_ATTR_VALUE);
                if(NULL != pParmNodeNameAttr)
                {
                    sSubNameAttrValue = pParmNodeNameAttr->Gettext();
                    sSubNodeNameAttrValue = sSubNameAttrValue.operator char*();
                    sSubNodeNameAttrHexValue = EncodeParmNameHexValue(sSubNodeNameAttrValue);  // xml to wbxml format.
                    sWBXMLCode = sWBXMLCode + PARM_ELEMENT_NCAC_TOKEN + sSubNodeNameAttrHexValue;
                    //TRACE(" name=\"%s\"",sSubNodeNameAttrValue);
                }
                else // the name attribute does not exist in param element.
                {
                    AfxMessageBox("This param does not contain the attribute of the name!\n");
                }

                // get the value of the value attribute in param element.
                pParmNodeValueAttr = pNodeAttr->getNamedItem(PARM_VALUE_ATTR_VALUE);
                if(NULL != pParmNodeValueAttr)
                {
                    sSubValueAttrValue = pParmNodeValueAttr->Gettext();
                    sSubNodeValueAttrValue = sSubValueAttrValue.operator char*();
                    sSubNodeValueAttrHexValue = EncodeParmValueHexValue(sSubNodeNameAttrValue,sSubNodeValueAttrValue);   // xml to wbxml format.
                    sWBXMLCode = sWBXMLCode + TOKEN_OF_VALUE_ATTRIBUTE + sSubNodeValueAttrHexValue;
                    //TRACE(" value=\"%s\"/>\n",sSubNodeValueAttrValue);
                    //TRACE("nameHex = %s\t valueHex = %s\n",sSubNodeNameAttrHexValue,sSubNodeValueAttrHexValue);
                }
                else // the value attribute does not exist in param element.
                {
                    sSubNodeValueAttrHexValue = END_TOKEN_OF_ATTRIBUTE;
                    sWBXMLCode = sWBXMLCode + sSubNodeValueAttrHexValue;
                    //TRACE("/>\n");
                    //TRACE("nameHex = %s\t valueHex = %s\n",sSubNodeNameAttrHexValue,sSubNodeValueAttrHexValue);
                    //TRACE("This param does not contain the attribute of the value.\n");
                }
//                 if(IsInternetAccountNAPType(sSibNodeAttrValue))
//                 {
//                     GetInternetAccountNAPSetting(sSibNodeAttrValue,sSubNodeNameAttrValue,sSubNodeValueAttrValue);
//                 }
//                 if(IsInternetAccountPXLType(sSibNodeAttrValue))
//                 {
//                     GetInternetAccountPXLSetting(sSibNodeAttrValue,sSubNodeNameAttrValue,sSubNodeValueAttrValue);
//                 }
                pSubNode = pSubNode->GetnextSibling();
            }
            else
            {   
                ParseXMLTree(pSubNode, sWBXMLCode);
                m_iCharacteristicNodeSwitch = 0;
                pSubNode = NULL;
            }
        }

        if(m_iCharacteristicNodeSwitch == 0)
        {
            // a characteristic element is over.
            sWBXMLCode = sWBXMLCode + END_TOKEN_OF_ATTRIBUTE;
            //TRACE("-->:</characteristic> \n");  
            
            if(NULL != pSibNode)
            {
                pSibNode = pSibNode->GetnextSibling();
            }
        }
        else
        {
            pSibNode = NULL;
        }
    }
    return sWBXMLCode;
}

/**************************************************************************************/
// Description      : check whether the attribute named name contains fixed attribute 
//                    value or not in the parm element.
//--------------------------------------------------------------------------------------
// Input Parameter  : the value of the attribute named name in the parm element.  
// Output Parameter : NONE
// Return Value     : TRUE / FALSE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOLEAN CPduHexEncoder::IsFixedAttrValue(const CString &parmName)
{
    /**************************************** 
    **   the attributes which contains fixed attributes keywords list: 
    **   1. ADDRTYPE
    **   2. CALLTYPE
    **   3. AUTHTYPE
    **   4. PXAUTH-TYPE
    **   5. BEARER
    **   6. LINKSPEED
    **   7. SERVICE
    **   8. AAUTHTYPE
    **   9. AUTH-ENTITY
    ****************************************/
    if( parmName.Find("ADDRTYPE")>=0
        || parmName.Compare("CALLTYPE")==0 
        || parmName.Compare("AUTHTYPE")==0
        || parmName.Compare("PXAUTH-TYPE")==0
        || parmName.Compare("BEARER")==0
        || parmName.Compare("LINKSPEED")==0
        || parmName.Compare("SERVICE")==0
        || parmName.Compare("AAUTHTYPE")==0
        || parmName.Compare("AUTH-ENTITY")==0 )
    {   // is attribute which contains fixed attribute.
        return TRUE;
    }
    else
    {   // is not attribute which contains fixed attribute.
        return FALSE;
    }
}

/**************************************************************************************/
// Description      : encode characteristic type Hex value.
//--------------------------------------------------------------------------------------
// Input Parameter  : CString &characteristicType  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::EncodeCharacteristicTypeHexValue(CString &characteristicType)
{
    CString preCharacteristicType = characteristicType;
    CString characteristicHexType = "";
    char *typeHexCode = NULL;
    int nSizeOfAttrTable = sizeof(sv_prov10_attr_table)/sizeof(WBXMLAttrEntry);
    int iNamePos = 0;

    typeHexCode = preCharacteristicType.GetBuffer(preCharacteristicType.GetLength());
    preCharacteristicType.ReleaseBuffer();

    for(iNamePos=0; iNamePos < nSizeOfAttrTable; iNamePos++)
    {
        if(sv_prov10_attr_table[iNamePos].xmlName != NULL)
        {
            if(strcmp(sv_prov10_attr_table[iNamePos].xmlName,"type")==0)
            {
                if(sv_prov10_attr_table[iNamePos].xmlValue != NULL)
                {
                    if(strcmp(sv_prov10_attr_table[iNamePos].xmlValue,typeHexCode)==0)
                    {
                        itoa(sv_prov10_attr_table[iNamePos].wbxmlToken,typeHexCode,16); // transform to hexadecimal
                        characteristicHexType.Format("%s",typeHexCode);
                        if(characteristicHexType.GetLength() < WBXML_TOKEN_LENGTH)
                        {
                            characteristicHexType = "0" + characteristicHexType;
                        }
                        break;
                    }
                }
            }
        }
    }
    
    if(iNamePos < nSizeOfAttrTable)
    {
        if(sv_prov10_attr_table[iNamePos].wbxmlCodePage)
        {
            characteristicHexType = CODE_PAGE_ZERO_TO_CODE_PAGE_ONE + characteristicHexType;
            m_iCodePage = 1;
        }
    }
    else
    {
        characteristicHexType = TOKEN_OF_UNKNOWN_TYPE_ATTRIBUTE + START_TOKEN_OF_INLINE_STR_CONTENT 
            + END_TOKEN_OF_INLINE_STR_CONTENT + ConvertStr2HexStr(preCharacteristicType);
        //TRACE("the type does not exist in the attribute table.");
    }
    return characteristicHexType;
}

/**************************************************************************************/
// Description      : convert the normal string to the hex string.
//--------------------------------------------------------------------------------------
// Input Parameter  : the value of the attribute named value in the parm element.    
// Output Parameter : NONE
// Return Value     : the hex value of the attribute named value in the parm element.
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::ConvertStr2HexStr(CString &sAttrValueString)
{
    CString sPreAttrValueString = sAttrValueString;
    CString ASCIICodeStrOfChar = "";
    CString sAttrValueHexString = "";
    int ASCIICodeValue = 0;
    char ASCIICodeString[2]={0};

    for(int iCharPos=0;iCharPos < sPreAttrValueString.GetLength();iCharPos++)
    {
        ASCIICodeValue = sPreAttrValueString.GetAt(iCharPos);
        itoa(ASCIICodeValue,ASCIICodeString,16); // transform to hexadecimal
        ASCIICodeStrOfChar.Format("%s",ASCIICodeString);
        sAttrValueHexString = sAttrValueHexString + ASCIICodeStrOfChar;
    }
    return sAttrValueHexString;
}

/**************************************************************************************/
// Description      : encode the hex value of the attribute named name in the parm element.
//--------------------------------------------------------------------------------------
// Input Parameter  : the value of the attribute named name in the parm element.  
// Output Parameter : NONE
// Return Value     : the hex value of the attribute named name in the parm element.
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::EncodeParmNameHexValue(CString &parmName)
{
    CString preParmName = parmName;
    CString parmHexName = "";
    char *nameHexCode = NULL;
    int nSizeOfAttrTable = sizeof(sv_prov10_attr_table)/sizeof(WBXMLAttrEntry);

    nameHexCode = preParmName.GetBuffer(preParmName.GetLength());
    preParmName.ReleaseBuffer();

    for(int iNamePos=0; iNamePos < nSizeOfAttrTable; iNamePos++)
    {
        if(sv_prov10_attr_table[iNamePos].xmlName != NULL)
        {
            if(strcmp(sv_prov10_attr_table[iNamePos].xmlName,"name")==0)
            {
                if(sv_prov10_attr_table[iNamePos].xmlValue != NULL)
                {
                    if(strcmp(sv_prov10_attr_table[iNamePos].xmlValue,nameHexCode)==0)
                    {
                        itoa(sv_prov10_attr_table[iNamePos].wbxmlToken,nameHexCode,16); // transform to hexadecimal
                        parmHexName.Format("%s",nameHexCode);
                        if(parmHexName.GetLength() < WBXML_TOKEN_LENGTH)
                        {
                            parmHexName = "0" + parmHexName;
                        }
                        break;
                    }
                }
            }
        }
    }

    if(iNamePos == nSizeOfAttrTable)
    {
        parmHexName = ConvertStr2HexStr(preParmName);

        if(0 == m_iCodePage)
        {
            parmHexName = TOKEN_OF_NAME_ATTRIBUTE + START_TOKEN_OF_INLINE_STR_CONTENT 
                + parmHexName + END_TOKEN_OF_INLINE_STR_CONTENT;
        }
        else
        {
            parmHexName = CODE_PAGE_ONE_TO_CODE_PAGE_ZERO + TOKEN_OF_NAME_ATTRIBUTE 
                + START_TOKEN_OF_INLINE_STR_CONTENT + parmHexName + END_TOKEN_OF_INLINE_STR_CONTENT;
        }
    }
    else
    {
        if(0 == m_iCodePage)
        {
            if(sv_prov10_attr_table[iNamePos].wbxmlCodePage)
            {
                parmHexName = CODE_PAGE_ZERO_TO_CODE_PAGE_ONE + parmHexName + CODE_PAGE_ONE_TO_CODE_PAGE_ZERO;
            }
        }
        else
        {
            parmHexName = parmHexName + CODE_PAGE_ONE_TO_CODE_PAGE_ZERO;
            m_iCodePage = 0;
        }
    }

    return parmHexName;
}

/**************************************************************************************/
// Description      : encode the hex value of the attribute named value in the parm element.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. the value of the attribute named name in the parm element.
//                    2. the value of the attribute named value in the parm element.  
// Output Parameter : NONE
// Return Value     : the hex value of the attribute named value in the parm element.
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::EncodeParmValueHexValue(const CString &parmName, CString &parmValue)
{
    CString preParmValue = parmValue;
    CString parmHexValue = "";
    int iAttrValuePos=0;
    char ASCIICodeString[2]={0};

    if(IsFixedAttrValue(parmName))
    {   // is the attribute which contains fixed attributes.
        char *parmValueAttrValue = NULL;
        int nSizeOfAttrValueTable = sizeof(sv_prov10_attr_value_table)/sizeof(WBXMLAttrValueEntry); 
        
        parmValueAttrValue = preParmValue.GetBuffer(preParmValue.GetLength());
        preParmValue.ReleaseBuffer();  
        
        for(iAttrValuePos=0; iAttrValuePos < nSizeOfAttrValueTable; iAttrValuePos++)
        {
            if(sv_prov10_attr_value_table[iAttrValuePos].xmlName != NULL)
            {
                if(strcmp(sv_prov10_attr_value_table[iAttrValuePos].xmlName,parmValueAttrValue)==0)
                {
                    itoa(sv_prov10_attr_value_table[iAttrValuePos].wbxmlToken,ASCIICodeString,16); // transform to hexadecimal
                    parmHexValue.Format("%s",ASCIICodeString);
                    
                    if(sv_prov10_attr_value_table[iAttrValuePos].wbxmlCodePage)
                    {
                        parmHexValue = CODE_PAGE_ZERO_TO_CODE_PAGE_ONE + parmHexValue + END_TOKEN_OF_ATTRIBUTE;
                        m_iCodePage = 1;
                    }
                    else
                    {
                        parmHexValue = parmHexValue + END_TOKEN_OF_ATTRIBUTE;
                    }
                    break;
                }
            }
        }
        // the fixed attribute value does not exist.
        if(iAttrValuePos == nSizeOfAttrValueTable)
        {
            parmHexValue = ConvertStr2HexStr(preParmValue);
            parmHexValue = START_TOKEN_OF_INLINE_STR_CONTENT + parmHexValue + END_TOKEN_OF_INLINE_STR_CONTENT + END_TOKEN_OF_ATTRIBUTE; 
        }
    }
    else
    {   // is the attribute which does not contain fixed attributes.
        parmHexValue = ConvertStr2HexStr(preParmValue);
        parmHexValue = START_TOKEN_OF_INLINE_STR_CONTENT + parmHexValue + END_TOKEN_OF_INLINE_STR_CONTENT + END_TOKEN_OF_ATTRIBUTE; 
    }

    return parmHexValue;
}

/**************************************************************************************/
// Description      : generate .XLS test case file by hex pdu.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. the set of the pdu sequence
//                    2. the name of the xml file  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CPduHexEncoder::GenerateXlsTestCaseFile( 
                                              const CStringArray &sHexPduSet, 
                                              const CString &sXmlFileName 
                                              //const CString &sInternetAccountInfo
                                            )
{
    CString sCaseID = ConvertDecNum2DecStr(m_nXmlFileCount);
    int nSizeOfHexPduSet = sHexPduSet.GetSize();
    
    m_pExcelApp.GetRange(m_iSheetRowNo,m_iSheetColNo);
    m_pExcelApp.SetRangeValue(sCaseID);
    m_pExcelApp.GetRange(m_iSheetRowNo,(m_iSheetColNo+1));
    m_pExcelApp.SetRangeValue(sXmlFileName);
    //m_pExcelApp.GetRange(m_iSheetRowNo,m_lExpectedResultColNo);
    //m_pExcelApp.SetRangeValue(sInternetAccountInfo);

    if(nSizeOfHexPduSet)
    {
        for(long iPduNo=0; iPduNo < nSizeOfHexPduSet; iPduNo++)
        {
            m_pExcelApp.GetRange(m_iSheetRowNo,(m_lPduStartColNo+iPduNo));
            m_pExcelApp.SetRangeFormat(WRAP_TEXT,TRUE);
            m_pExcelApp.SetRangeValue(sHexPduSet.GetAt(iPduNo));
        }
    }
    else
    {
        m_pExcelApp.GetRange(m_iSheetRowNo, m_lPduStartColNo);
        m_pExcelApp.SetRangeFormat(WRAP_TEXT,TRUE);
        m_pExcelApp.SetRangeBgColor(ERROR_RANGE_BG_COLOR);
        m_pExcelApp.SetRangeValue("The format of the current xml file is not correct.");
        m_iErrorFormatSwitch = 1;
    }
    // reset
    m_iSheetColNo = 1;
    m_iSheetRowNo++;

    m_pExcelApp.SaveExcelFile();
}

/**************************************************************************************/
// Description      : set the title content in the sheet named of the test case file.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CPduHexEncoder::SetXlsTestCaseFileTitle(void)
{
    CString sNoOfPdu = "";
    CString sTestCaseName = GenerateXlsFileName();
    m_iSheetRowNo = 1;
    m_iSheetColNo = 1;

    m_pExcelApp.CreateNewWorkbook();
    m_pExcelApp.GetWorksheets();   
    m_pExcelApp.SetSheetName(1,OTA_TEST_CASE_SHEET_NAME);
    m_pExcelApp.GetSelectedSheet(OTA_TEST_CASE_SHEET_NAME);
    
    m_pExcelApp.GetRange(m_iSheetRowNo,m_iSheetColNo);
    m_pExcelApp.SetRangeValue(CASE_ID_COL_NAME);
    m_pExcelApp.SetRangeBgColor(TITLE_RANGE_BG_COLOR);
    m_iSheetColNo++;
    // set the name of the xml file name column.
    m_pExcelApp.GetRange(m_iSheetRowNo,m_iSheetColNo);
    m_pExcelApp.SetRangeValue(XML_FILE_NAME_COL_NAME);
    m_pExcelApp.SetRangeBgColor(TITLE_RANGE_BG_COLOR);
    m_iSheetColNo++;
    m_pExcelApp.GetRange(m_iSheetRowNo,m_iSheetColNo);
    m_pExcelApp.SetRangeValue(TYPE_COL_NAME);
    m_pExcelApp.SetRangeBgColor(TITLE_RANGE_BG_COLOR);
    m_iSheetColNo++;
    // set the name of the pdu data column
    m_lPduStartColNo = m_iSheetColNo;
    for(long iPduNo=0; iPduNo < PDU_MAX_LENGTH_COL_NAME; iPduNo++)
    {
        sNoOfPdu = PDU_COL_NAME + ConvertDecNum2DecStr((iPduNo+1));
        m_pExcelApp.GetRange(m_iSheetRowNo,(m_iSheetColNo+iPduNo));
        m_pExcelApp.SetRangeValue(sNoOfPdu);
        m_pExcelApp.SetRangeBgColor(TITLE_RANGE_BG_COLOR);
    }
    m_iSheetColNo = m_iSheetColNo + PDU_MAX_LENGTH_COL_NAME;
    // set the name of the expected result column.
    m_pExcelApp.GetRange(m_iSheetRowNo,m_iSheetColNo);
    m_pExcelApp.SetRangeValue(THE_COL_NAME_OF_EXPECTED_RESULT);
    m_pExcelApp.SetRangeBgColor(TITLE_RANGE_BG_COLOR);
    m_lExpectedResultColNo = m_iSheetColNo;
    m_iSheetColNo++;
    // set the name of the actual result column.
    m_pExcelApp.GetRange(m_iSheetRowNo,m_iSheetColNo);
    m_pExcelApp.SetRangeValue(THE_COL_NAME_OF_ACTUAL_RESULT);
    m_pExcelApp.SetRangeBgColor(TITLE_RANGE_BG_COLOR);
    m_iSheetColNo++;
    m_pExcelApp.GetRange(m_iSheetRowNo,m_iSheetColNo);
    m_pExcelApp.SetRangeValue(CONCLUSION_COL_NAME);
    m_pExcelApp.SetRangeBgColor(TITLE_RANGE_BG_COLOR);

    m_iSheetRowNo = m_iSheetRowNo + 1;
    m_iSheetColNo = 1;

    m_pExcelApp.SaveExcelFile(sTestCaseName);
}

/**************************************************************************************/
// Description      : generate .XLS test case file name.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : the name of the file that format is .XLS.
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::GenerateXlsFileName(void)
{
    CString sTestCaseFilePath = "";
    CString sTestCaseFileName = "";
    CString sXlsFilePath = "";
    CString sTimeStamp = "";

    sTimeStamp = GetTimeStamp();
    // get the main path of the OTA test case file.
    sTestCaseFilePath = CConPanCfgAccess::GetOTATestCasePath();
    sTestCaseFilePath = sTestCaseFilePath.Left(sTestCaseFilePath.ReverseFind('\\')+1);
    sTestCaseFileName = WBXML_TEST_CASE_FILE_NAME + "_" + sTimeStamp + ".xls";
    sXlsFilePath = sTestCaseFilePath + sTestCaseFileName;
    CConPanCfgAccess::SetOTAXmlTestCasePath(sXlsFilePath);

    return sXlsFilePath;
}

/**************************************************************************************/
// Description      : generate the time stamp string of the current time.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE  
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
CString CPduHexEncoder::GetTimeStamp()
{
    // Initialize Time Object
    COleDateTime sTime;
    COleDateTime sDate;
    CString sTimeStr = "";
    CString sDateStr = "";
    CString sTimeStamp = "";
    
    sTime = COleDateTime::GetCurrentTime();
    sDate = COleDateTime::GetCurrentTime();
    sTimeStr = sTime.Format("%H%M%S");
    sDateStr = sDate.Format("%Y%m%d");
    
    sTimeStamp = sDateStr + "_" + sTimeStr;  // timeStamp

    return sTimeStamp;
}

/**************************************************************************************/
// Description      : check whether the type attr value is NAP keyword or not.
//--------------------------------------------------------------------------------------
// Input Parameter  : the value of the attribute named type in the characteristic element.  
// Output Parameter : NONE
// Return Value     : TRUE / FALSE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note: NAP -- Network Access Point
/**************************************************************************************/
BOOLEAN CPduHexEncoder::IsInternetAccountNAPType(const CString &CharTypeAttrValue)
{
    /**************************************** 
    **   NAP keywords list: 
    **   1. NAPDEF
    **   2. NAPAUTHINFO
    ****************************************/   
    if( CharTypeAttrValue.Compare("NAPDEF")==0
        || CharTypeAttrValue.Compare("NAPAUTHINFO")==0)
    {   // is NAP's keywords.
        return TRUE;
    }
    else
    {   // is not NAP's keywords.
        return FALSE;
    }
}

/**************************************************************************************/
// Description      : check whether the type attr value is logical proxy keyword or not.
//--------------------------------------------------------------------------------------
// Input Parameter  : the value of the attribute named type in the characteristic element.
// Output Parameter : NONE
// Return Value     : TRUE / FALSE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
BOOLEAN CPduHexEncoder::IsInternetAccountPXLType(const CString &CharTypeAttrValue)
{
    /**************************************** 
    **   logical proxy's keywords list: 
    **   1. PXLOGICAL
    **   2. PXPHYSICAL
    **   3. PORT
    ****************************************/
    if( CharTypeAttrValue.Compare("PXLOGICAL")==0
        || CharTypeAttrValue.Compare("PXPHYSICAL")==0    
        || CharTypeAttrValue.Compare("PORT")==0 )
    {   // is logical proxy's keywords.
        return TRUE;
    }
    else
    {   // is not logical proxy's keywords.
        return FALSE;
    }
}

/**************************************************************************************/
// Description      : get the setting information of the Internet account NAP (Network Access Point).
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. the value of the attribute named type in the characteristic element.
//                    2. the value of the attribute named name in the parm element.
//                    3. the value of the attribute named value in the parm element.
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note: NAP -- Network Access Point
/**************************************************************************************/
void CPduHexEncoder::GetInternetAccountNAPSetting( 
                                                   const CString &CharTypeAttrValue,  // IN:Param
                                                   const CString &ParmNameAttrValue,  // IN:Param
                                                   const CString &ParmValueAttrValue  // IN:Param
                                                 )
{
    // get NAP's setting information.
    if(CharTypeAttrValue.Compare("NAPDEF")==0 && ParmNameAttrValue.Compare("NAP-ADDRESS")==0)
    {   // get 
        m_mInternetAccountKeySettingInfo.SetAt("[Access Points]:",ParmValueAttrValue);
    }
    else if(CharTypeAttrValue.Compare("NAPAUTHINFO")==0)
    {
        if(ParmNameAttrValue.Compare("AUTHTYPE")==0)
        {
            if(ParmValueAttrValue.Compare("CHAP")==0)
            {
                m_mInternetAccountKeySettingInfo.SetAt("[Security Connect]:","Enable");
            }
            else
            {
                m_mInternetAccountKeySettingInfo.SetAt("[Security Connect]:","Forbidden");
            }
        }
        else if(ParmNameAttrValue.Compare("AUTHNAME")==0)
        {
            m_mInternetAccountKeySettingInfo.SetAt("[User Name]:",ParmValueAttrValue);
        }
        else if(ParmNameAttrValue.Compare("AUTHSECRET")==0)
        {
            m_mInternetAccountKeySettingInfo.SetAt("[Password]:",ParmValueAttrValue);
        }
    }
}

/**************************************************************************************/
// Description      : get the setting information in the label named PXLOGICAL.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1. the value of the attribute named type in the characteristic element.
//                    2. the value of the attribute named name in the parm element.
//                    3. the value of the attribute named value in the parm element.
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CPduHexEncoder::GetInternetAccountPXLSetting( 
                                                   const CString &CharTypeAttrValue, 
                                                   const CString &ParmNameAttrValue, 
                                                   const CString &ParmValueAttrValue 
                                                 )
{
    if(CharTypeAttrValue.Compare("PXLOGICAL")==0)
    {
        if(ParmNameAttrValue.Compare("NAME")==0)
        {
            m_aInternetAccountPXLInfo.Add("[Change Name]:"+ParmValueAttrValue);
        }
        else if(ParmNameAttrValue.Compare("STARTPAGE")==0)
        {
            m_aInternetAccountPXLInfo.Add("[Set Homepage]:"+ParmValueAttrValue);
        }
    }
    else if(CharTypeAttrValue.Compare("PXPHYSICAL")==0)
    {
        if(ParmNameAttrValue.Compare("PXADDR")==0)
        {
            m_aInternetAccountPXLInfo.Add("[Gateway]:"+ParmValueAttrValue);
        }
    }
    else if(CharTypeAttrValue.Compare("PORT")==0)
    {
        if(ParmNameAttrValue.Compare("PORTNBR")==0)
        {
            m_aInternetAccountPXLInfo.Add("[Port]:"+ParmValueAttrValue);
        }
        else if(ParmNameAttrValue.Compare("SERVICE")==0)
        {
            CString sNetType = "[Net Type]:";
            if(ParmValueAttrValue.Find("HTTP")==-1)
            {
                m_aInternetAccountPXLInfo.Add(sNetType+"WAP1.2");
            }
            else
            {
                m_aInternetAccountPXLInfo.Add(sNetType+"WAP2.0/HTTP");             
            }
        }
    }
}

/**************************************************************************************/
// Description      : initialize the Internet account information.
//--------------------------------------------------------------------------------------
// Input Parameter  : the number of the label named PXLOGICAL
// Output Parameter : NONE
// Return Value     : NONE
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
void CPduHexEncoder::InitInternetAccountInfo(int nPxLogicalNumber)
{
    // initialize the Internet account information.
    for(int iPxLogicalNo=0; iPxLogicalNo < nPxLogicalNumber; iPxLogicalNo++)
    {
        m_aInternetAccountInfo.Add("[Change Name]:");
        m_aInternetAccountInfo.Add("[Set Homepage]:");
        m_aInternetAccountInfo.Add("[Gateway]:");
        m_aInternetAccountInfo.Add("[Port]:");
        m_aInternetAccountInfo.Add("[User Name]:");
        m_aInternetAccountInfo.Add("[Password]:");
        m_aInternetAccountInfo.Add("[Access Points]:");
        m_aInternetAccountInfo.Add("[Security Connect]:");
        m_aInternetAccountInfo.Add("[Net Type]:");
    }
}

/**************************************************************************************/
// Description      : get Internet account information.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE 
// Output Parameter : NONE
// Return Value     : the string which contains the key information of the IA
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note: IA -- Internet Account
/**************************************************************************************/
CString CPduHexEncoder::GetInternetAccountInfo(void)
{
    CString sInternetAccountInfo = "";
    
    for(int itemNo=0; itemNo < m_aInternetAccountInfo.GetSize(); itemNo++)
    {
        CString sInternetAccountMapKey = "";
    }
    return sInternetAccountInfo;
}

/**************************************************************************************/
// Description      : get the error format switch.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE 
// Output Parameter : NONE
// Return Value     : m_iErrorFormatSwitch
//-------------------------------------------------------------------------------------- 
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note: 
/**************************************************************************************/
int CPduHexEncoder::GetErrorFormatSwitch()
{
    return m_iErrorFormatSwitch;
}
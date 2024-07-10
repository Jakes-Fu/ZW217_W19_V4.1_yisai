
/******************************************************************************
 ** File Name:      DPSrv.h                                                   *
 ** Author:         Leo.Feng                                                  *
 ** DATE:           28/05/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 28/05/2002     Leo.Feng         Create.                                   *
 ******************************************************************************/

#ifndef _DPSRV_H_
#define _DPSRV_H_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DPSVR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DPSVR_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef DPSRV_EXPORTS
#define DPSRV_API __declspec(dllexport)
#else
#define DPSRV_API __declspec(dllimport)
#endif

#include "CDFException.h"
#include "ErrorString.h"

#define MAX_VAR_NAME_SIZE          64
#define INVALID_MSG_ID             0xFFFFFFFF
#define INVALID_SIZE_FIELD_INDEX   -1

/*****************************************************************************/
//Start data type define
typedef enum
{
    LITTLE_ENDIAN,
    BIG_ENDIAN
}EndianType;

typedef enum
{
    NONE_NODE_TYPE,
    INVISIBLE_NODE_TYPE,                //节点没有值
    VISIBLE_NODE_TYPE                   //节点有值
}ENodeType;

typedef enum
{
    NONE_DATA_TYPE,
    CHAR_DATA_TYPE,                     //节点为字符类型，节点的值为整型
    SHORT_DATA_TYPE,                    //节点为短整型类型，节点的值为整型
    INT_DATA_TYPE = 4,                  //节点为整型类型，节点的值为整型
    ENUM_DATA_TYPE,                     //节点为枚举类型，节点的值为字符串
	UNION_DATA_TYPE	,					// node is union
	LONGLONG_DATA_TYPE = 8				// 64 bit integer type
}EDataType;

typedef struct TreeNode_tag
{
    ENodeType     m_eNodeType;          //节点显示类型 
    EDataType     m_eDataType;          //节点数据类型
    int           m_iLevel;             //节点级别
    int           m_iOffset;            //节点偏移量
    __int64       m_iValue;             //节点的整型值
    CString       m_szValue;            //节点的字符值
    CString       m_szLeftName;         //节点的类型名称
    CString       m_szRightName;        //节点的名称
	__int64		  m_iBitMask;			//如果不为0，那么说明节点为一个位域，其值为该节点的掩码标志
	int			  m_iCommentAddr;		//注释字符串指针
}TreeNode,*TreeNodePtr;

//Added for GPRS test
typedef struct EmbeddedMsgInfo_tag
{
    int       nChannel;
    int       nType;
    BOOL      bEmbeddedPtrType;
    int       nEmbeddedFieldIndex;
    int       nEmbeddedSizeFieldIndex;
    int       nEmbeddedMemBlockSize;
    void      *pEmbeddedMemBlock;    
}EmbeddedMsgInfo,*EmbeddedMsgInfo_PTR;
//End

//End of data type define
/*****************************************************************************/



class IDP
{
public:
    //Initialize DPSrv(Data Processor Server)
	virtual HRESULT Init(void) = 0;
    
    //Free Memory
    virtual HRESULT Free(void **ppMemBlock) = 0;

    //Start the interface for bits stream
    //Convert a memory block to a Name_to_Value table according to some defined structure
    virtual HRESULT Decode(
        unsigned int   nMsgChannelID,
        unsigned int   nMsgTypeID,         
        void           *ppMemBlock,
        int            nMemBlockSize
        ) = 0;

    //Convert a message description to a Memory Block according to some defined structure
    virtual HRESULT Encode(
        unsigned int   nMsgChannelID,
        unsigned int   nMsgTypeID, 
        char           *szMsgScript, 
        void           **ppMemBlock,        
        int            *pMemBlockSize
        ) = 0;  

    //End of the interface for bytes-to-bits converting

    //Start the interface for bytes stream
    //Convert a memory block to a Name_to_Value table according to some defined structure
    virtual HRESULT Decode(
        unsigned short nMsgTypeID,         
        void           *ppMemBlock,
        int            nMemBlockSize
        ) = 0;
    
    virtual HRESULT Decode(
        char           *szMsgTypeName,         
        void           *ppMemBlock,        
        int            nMemBlockSize
        ) = 0;   

    //Convert a message description to a Memory Block according to some defined structure
    virtual HRESULT Encode(
        unsigned short nMsgTypeID, 
        char           *szMsgScript, 
        void           **ppMemBlock,
        int            *pMemBlockSize
        ) = 0;
    
    virtual HRESULT Encode(           
        char           *szMsgTypeName, 
        char           *szMsgScript, 
        void           **ppMemBlock,
        int            *pMemBlockSize
        ) = 0;

    virtual HRESULT Encode(
        unsigned short nMsgTypeID,         
        void           **ppMemBlock,
        int            *pMemBlockSize
        ) = 0;
    
    virtual HRESULT Encode(           
        char           *szMsgTypeName,         
        void           **ppMemBlock,
        int            *pMemBlockSize
        ) = 0;

    //Start the interface for bytes stream
    
    //Added at 2002-07-15 For GPRS test
    virtual HRESULT Decode(
        unsigned short      nMsgTypeID,         
        void                *ppMemBlock,        
        int                 nMemBlockSize,
        EmbeddedMsgInfo_PTR pEmbeddedMsgInfo,
        int                 nEmbeddedMsgNum
        ) = 0;

    virtual HRESULT Decode(
        char                *szMsgTypeName,         
        void                *ppMemBlock,        
        int                 nMemBlockSize,        
        EmbeddedMsgInfo_PTR pEmbeddedMsgInfo,
        int                 nEmbeddedMsgNum
        ) = 0;

    virtual HRESULT Encode(           
        unsigned short      nMsgTypeID,         
        char                *szMsgScript, 
        void                **ppMemBlock,
        int                 *pMemBlockSize,
        EmbeddedMsgInfo_PTR pEmbeddedMsgInfo,
        int                 nEmbededdMsgNum
        ) = 0;
    
    virtual HRESULT Encode(           
        char                *szMsgTypeName,
        char                *szMsgScript, 
        void                **ppMemBlock,
        int                 *pMemBlockSize,
        EmbeddedMsgInfo_PTR pEmbeddedMsgInfo,
        int                 nEmbeddedMsgNum
        ) = 0;   
    //End of added For GPRS test

    //Start the interface for getting result
    virtual DWORD GetValue(char *szMsgScript )= 0;    
    
    //Added at 2002-07-15 For GPRS test
    virtual DWORD GetBitsValue(char *szMsgScript) = 0;

    //End of the interface for getting result
    
    //Start the interface for old version structparser
    //获取节点
	virtual TreeNode& GetByteTreeNode(int iIndex) = 0;

    //Added for the air message tree
    virtual TreeNode& GetBitsTreeNode(int iIndex) = 0;

    //获取节点数目
	virtual int  GetByteStreamSize() = 0;
    virtual int  GetBitsStreamSize() = 0;

    virtual int  GetByteNodeCount() = 0;
    
    virtual int  GetBitsNodeCount() = 0;
    
    virtual int  GetFieldOffset(int iFieldIndex) = 0;
    virtual int  GetFieldOffset(int nMsgCode, int nFieldIndex) = 0;
    virtual int  GetFieldOffset(char *szMsgName, int nFieldIndex) = 0;

    virtual int  GetFieldValue(int iFieldIndex) = 0;

    //设置二进制数据转换为可显示的线形表时的endian
	virtual HRESULT SetEndian(EndianType eEndianType) = 0;

    //设置二进制数据转换为可显示的线形表时的字节对齐方式
	virtual HRESULT SetAlignment(int iAlignValue) = 0;

    //设置结构定义描述文件的路径
	virtual HRESULT SetFile(char * szFileName) = 0;
    //End of the interface for old version structparser

    //Start the interface for old version EndianConvert
    virtual HRESULT ConvertToBigEndian(LPBYTE lpSourceData, DWORD dwDataSize) = 0;
	
    virtual HRESULT ConvertToLittleEndian(LPBYTE lpSourceData, DWORD dwDataSize) = 0;
    //End of the interface for old version EndianConvert

    virtual const char * GetErrorMessage(CCDFException *err) = 0;

    //CppToCOM: Need to free an object in the DLL since it was allocatd here
    virtual ULONG   Release() = 0;
//Add Comment for samsung
	virtual const char * GetValueCommentString(__int64 value,int nCommentAddr){
		UNUSED_ALWAYS(value);
		UNUSED_ALWAYS(nCommentAddr);
		return NULL;
	};
};

class IDPSrvFactory
{
public:
    virtual HRESULT CreateDP(IDP **ppObject) = 0;
    virtual ULONG   Release(void) = 0;
};

HRESULT DPSRV_API DllGetClassFactoryObject(void **ppObject);

#endif /* _DPSVR_H_ */


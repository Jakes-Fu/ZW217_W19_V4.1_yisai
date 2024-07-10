#ifndef _CHFINTERFACE__H_
#define _CHFINTERFACE__H_

#pragma pack(push, 1)

//////////////////////////////////////////////////////////////////////////////////
#define INVALID_ENUM_VALUE   0xFFFFFFFF
#define INVALID_ITEM_TYPE    0xFFFFFFFF
#define INVALID_MSG_ID       0xFFFFFFFF

#define MAX_ITEM_NAME        64
#define MAX_SYMBOL_NAME_SIZE 64

//////////////////////////////////////////////////////////////////////////////////
//  �����ֵ���ײ�����
//////////////////////////////////////////////////////////////////////////////////
typedef enum DD_Item_Type_Tag
{
    DD_ITEM_NONE_TYPE      ,
    DD_ITEM_CHAR_TYPE      ,
    DD_ITEM_SHORT_TYPE     ,
    DD_ITEM_INT_TYPE    = 4,
    DD_ITEM_LONG_TYPE      ,   
    DD_ITEM_VOID_TYPE      ,   
	DD_ITEM_FLOAT_TYPE  ,
    DD_ITEM_LONGLONG_TYPE  ,
    DD_ITEM_DOUBLE_TYPE  ,

    DD_ITEM_SIMPLE_TYPE,
    DD_ITEM_ARRAY_TYPE,
    DD_ITEM_POINTER_TYPE,

    DD_ITEM_STRUCT_TYPE,
    DD_ITEM_ENUM_TYPE,
    DD_ITEM_UNION_TYPE,
    DD_ITEM_UNKNOW_TYPE,

	DD_ITEM_BITFIELD_TYPE,

    DD_ITEM_MAX_TYPE
}DD_Item_Type;

typedef struct DD_Head_Item_tag
{
    /* ���ⲿ����ʱ�Ľṹ��ʶ */
    int            iExternalMsgID;
    /* �����ֵ��ڲ�ʹ�õĽṹ��ʶ */
    int            iInternalMsgID;
    /* �ṹ��ԭʼ�����ʶ */
    int            iParentMsgID;
    /* �ṹ������ */
    DD_Item_Type   eItemType;
    /* �ṹʵ���ŵĿ�ʼλ�� */
    int            iStartAddr;
    /* �ṹʵ��Ĵ�С */
    int            iSize;
    int            iArraySize;
    /* �ṹʵ�����Ŀ */  
    short          iItemNum;
    short          iTotalFieldSize;

    /* �ṹ�������� */
    char           szTypeName[MAX_SYMBOL_NAME_SIZE];
    /* �ṹ������ */
    char           szName[MAX_SYMBOL_NAME_SIZE];    
    DD_Head_Item_tag()
    {
        memset( this,0,sizeof( *this ) );
    }
}DD_Head_Item, *DD_Head_Item_PTR;

//////////////////////////////////////////////////////////////////////////////////
typedef struct DD_Item_Body_tag
{   
    int               m_iLevel;
    DD_Item_Type      m_eType;       
    DD_Item_Type      m_eRightType;
    int               m_iParentID;
    int               m_iArraySize; 
    int               m_iEnumValue;
    /*�����ֽ�*/
    int               m_iOffset;
    int               m_iBitMask;	/* used by bit field */

    char              m_szLeftName[MAX_SYMBOL_NAME_SIZE-4];
	/*�����ṹ��ע����������TypeName�н�ȡ4bytes*/
	int				  m_iCommentAddr;    
    char              m_szRightName[MAX_SYMBOL_NAME_SIZE];
}DD_Item_Body,*DD_Item_Body_PTR;


#define DD_Head_Item_Size   sizeof(DD_Head_Item)   //0xa0
#define DD_Item_Body_Size     sizeof(DD_Item_Body)

#pragma pack(pop)

#endif //_CHFINTERFACE__H_
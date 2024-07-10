#ifndef _NVITEMDEF_H
#define _NVITEMDEF_H


#define MAX_NAME_LENGTH                 (0x200)
#define MAX_ITEM_CONTENT                (0x100)
#define INVALID_MODULE_ID               ((INT)0xFFFFFFFF)
#define INVALID_MODULE_ITEM_INDEX       ((INT)0xFFFFFFFF)
#define MAX_TOTAL_ITEM_CONTENT          (0x80000)
#define DSP_CODE_SIZE                   (0xC000)

//Define Item type
typedef enum ITEMTYPE_ENUM 
{ 
    NONE_ITEM_TYPE,
    CHAR_ITEM_TYPE  , 
    SHORT_ITEM_TYPE , 
    LONG_ITEM_TYPE = 4 , 
    ARRAY_ITEM_TYPE,
    ASTRING_ITEM_TYPE,
    STRUCT_ITEM_TYPE,
	WSTRING_ITEM_TYPE
}ITEMTYPE;


#endif // _NVITEMDEF_H
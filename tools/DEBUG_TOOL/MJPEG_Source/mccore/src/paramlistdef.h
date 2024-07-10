#ifndef PARAMLISTDEF_H
#define PARAMLISTDEF_H

#define PT_DEFAULT 0
#define PT_INT 1
#define PT_STRING 2
#define PT_ENUM 3
#define PT_BOOL 4
#define PT_FLOAT 5
#define PT_PTR 6
#define PT_FILE 7
#define PT_DIR 8
#define PT_SEPERATOR 0xe
#define PT_CATALOG 0xf
#define PF_NODE 0x8
#define PF_NOSAVE 0x20
#define PF_READONLY 0x40
#define PF_MODIFIED 0x80
#define PF_GENERATED PF_NOSAVE|PF_READONLY

//typedef TCHAR int8;
//typedef unsigned TCHAR uint8;
//typedef short int int16;
//typedef unsigned short int WORD;
//typedef int int32;
//typedef unsigned int uint32;

typedef struct {
	unsigned char type;
	unsigned char flags;
	short int min;
	int max;
	char* pchParamName;
	char* pchKeyName;
	char* pchEnums;
	void* valueDefault;
	void* value;
} PARAM_DATA;

typedef struct {
	char* pchParamName;
	char* pchEnums;
} PARAM_ENTRY;

typedef struct {
	int id;
	char* desc;
} PARAM_DESC;

typedef int (*PARAM_NOTIFY)(int, void*);

#endif
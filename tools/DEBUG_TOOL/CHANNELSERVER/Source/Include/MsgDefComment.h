#ifndef _MSGDEF_COMMENT_H_
#define _MSGDEF_COMMENT_H_

#pragma pack(push,1)


typedef struct Comment_File_Header_tag
{
	int HiVersion;
	int LoVersion;
	int nReserve;
	char desp[32];
}CommentFileHeader;

typedef enum Comment_Type_tag
{
	Comment_Header_Type=0x55AA5A5A,
	Comment_Value_type=1,
	Comment_Range_type,
	Comment_Default_type,
	Comment_Tial_Type=-1
}CommentItemType;

typedef struct Comment_Common_Header_tag
{
	CommentItemType eItemType;
	int				nItemSize;
}CommentCommonHeader;

typedef struct Comment_Value_Header_tag
{
	CommentCommonHeader header;
	int					nValue;
}CommentValueHeader;

typedef struct Comment_Range_Header_tag
{
	CommentCommonHeader header;
	int					nMin;
	int					nMax;
}CommentRangeHeader;

typedef struct{
	CommentValueHeader  header;
	char				comment[0];
}VisitorValueHeader;

typedef struct{
	CommentRangeHeader  header;
	char				comment[0];
}VisitorRangeHeader;

typedef struct{
	CommentCommonHeader  header;
	char				comment[0];
}VisitorDefaultHeader;

#pragma pack(pop)

#endif
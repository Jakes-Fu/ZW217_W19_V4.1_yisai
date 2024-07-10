#ifndef _MMI_FONT_DEF_H__
#define _MMI_FONT_DEF_H__

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// �ֿ���Դ����Ϣ
///////////////////////////////////////////////////////////////////////////////
const int MAX_FONT_NUM = 32;

// �ֿ������
typedef enum _GUI_FONT_T
{
	SONG_FONT_8 = 0,
	SONG_FONT_10,
	SONG_FONT_12,
	SONG_FONT_14,
	SONG_FONT_16,
	ASC_ME_FONT_38,
	ARI_NARROW_FONT_29,
	GUI_MAX_FONT_NUM,
} GUI_FONT_T;

typedef struct _GUI_FONT_INFO_T
{
	int			 type;	    	    //�ֿ�����
	uint32       ngb_db_size;
	uint8      * gb_database_ptr;	//��Ӧ�����ݿ�
	uint32       nascii_db_size;
	uint8      * pascii_ptr;		//��Ӧ��ASCII�����ݿ�
} GUI_FONT_INFO_T, * PGUI_FONT_INFO_T;

typedef struct _GUI_FONT_DB_T
{
	uint32       db_size;
	uint8      * pdb;
} GUI_FONT_DB_T,*PGUI_FONT_DB_T;

typedef struct _GUI_FONT_DB_TABLE_T
{
	GUI_FONT_DB_T db[MAX_FONT_NUM];
} GUI_FONT_DB_TABLE_T,*PGUI_FONT_DB_TABLE_T;

typedef struct _GUI_FONT_INFO_EX_T
{
	int			 type;	    	    //�ֿ�����
	uint8       *pDbtt;
} GUI_FONT_INFO_EX_T, * PGUI_FONT_INFO_EX_T;

#ifdef __cplusplus
}
#endif

#endif // _MMI_FONT_DEF_H__
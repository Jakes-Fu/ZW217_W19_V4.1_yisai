/******************************************************************************
 ** File Name:      se_table_gen.c                                                                              *
 ** Author:           Shan.He                                                                                            *
 ** DATE:             2009-6-9                                                                                         *
 ** Copyright:       2008 Spreatrum, Incoporated. All Rights Reserved.                                  *
 ** Description:     scaling up algorithm                                                                   *
 ** Note:                                                     *
 *****************************************************************************/

#include <stdio.h>
#include "se_table_gen.h"
#include "sci_types.h"
#include "os_api.h"


#define MAX_TABLE_SIZE 640
#define MICRO_SHIFT_BITS 9
#define MICOR_PIXEL 512

#define MAX_FILE_PATH_LEN 512

char g_szTmpPath[MAX_FILE_PATH_LEN] = {0};

/******************************************************************************
// Description: write a micro definition to a file
// Author:		shan.he
//              pFile:	file pointer
//              szMicroName: micro name
//              szMicroContext:	CONTEXT of micro
// Output:		
// Return:		none
// Note:		none
******************************************************************************/
LOCAL void WriteMicro(FILE *pFile, const char *szMicroName, const char *szMicroContext)
{
    char szTemp[1024] = "";
    SCI_ASSERT (NULL != pFile);
    SCI_ASSERT (NULL != szMicroName);
    SCI_ASSERT (NULL != szMicroContext);

    fwrite("\n", sizeof(char), 1, pFile);
    sprintf(szTemp, "#define %s %s", szMicroName, szMicroContext);
    fwrite(szTemp, sizeof(char), strlen(szTemp), pFile);
}

/******************************************************************************
// Description: write a string to a file
// Author:		shan.he
//              pFile:	file pointer
//              szString: string
// Output:		
// Return:		none
// Note:		none
******************************************************************************/
LOCAL void WriteString(FILE *pFile, const char *szString)
{
    char szTemp[1024] = "";
    SCI_ASSERT (NULL != pFile);
    SCI_ASSERT (NULL != szString);

    fwrite("\n", sizeof(char), 1, pFile);
    fwrite(szString, sizeof(char), strlen(szString), pFile);
}

/******************************************************************************
// Description: write table to a file
// Author:		shan.he
//              pFile:	file pointer
//              szTableName: table name
//              pTable:	pointer of table
//              uTableSize: size of table
// Output:		
// Return:		none
// Note:		none
******************************************************************************/
LOCAL uint32 WriteTable(FILE *pFile, const char *szTableName, const uint32 *pTable, uint32 uTableSize)
{
    char szNum[16] = "";
    char szLine[256] = "";
    uint32 i = 0, j = 0;

    SCI_ASSERT (NULL != szTableName);
    SCI_ASSERT (NULL != pTable);

    fwrite("\n", sizeof(char), 1, pFile);
    fwrite("\n", sizeof(char), 1, pFile);
    strcpy(szLine, "PUBLIC const uint32 ");
    fwrite(szLine, sizeof(char), strlen(szLine), pFile);
    fwrite(szTableName, sizeof(char), strlen(szTableName), pFile);
    fwrite("[]", sizeof(char), 2, pFile);
    fwrite("=", sizeof(char), 1, pFile);
    fwrite("\n", sizeof(char), 1, pFile);
    fwrite("{", sizeof(char), 1, pFile);
    fwrite("\n", sizeof(char), 1, pFile);

    if (uTableSize > 8)
    {
        for (i=0; i<uTableSize - 8; i+=8)
        {
            memset(szLine, 0, 256);

            strcat(szLine, "\t");
            for (j=0; j<8; j++)
            {
                sprintf(szNum, "%d,\t", *pTable++);
                strcat(szLine, szNum);
            }

            strcat(szLine, "\n");
            fwrite(szLine, sizeof(char), strlen(szLine), pFile);
        }    

        memset(szLine, 0, 256);
        strcat(szLine, "\t");
    }
    else
    {
        i = 0;
        memset(szLine, 0, 256);
        strcat(szLine, "\t");
    }

    while (i < uTableSize)
    {
        sprintf(szNum, "%d,\t", *pTable++);
        strcat(szLine, szNum);
        i++;    
    }

    strcat(szLine, "\n");
    fwrite(szLine, sizeof(char), strlen(szLine), pFile);
    fwrite("};", sizeof(char), 2, pFile);

    return i;
}

/******************************************************************************
// Description: create table of slide move special effect
// Author:		shan.he
//              uWidth:	icon width
//              uHeight: icon height
//              pTable: output table pointer
// Output:		
// Return:		none
// Note:		the table will be output to the file named "SE_SLIDE_MOVE_table"
******************************************************************************/
PUBLIC BOOLEAN CreateSlideMoveTable(int32 uWidth, int32 uHeight)
{
    //char szFileName[256] = "SE_SLIDE_MOVE_table";
	char szFileName[MAX_FILE_PATH_LEN] = {0};	
    uint32 TransTable[MAX_TABLE_SIZE + 1];	
    uint32 uTableSize = 0;
    __int64 w = uWidth;
    __int64 h = uHeight;
    __int64 h1 = h;
    __int64 h2 = h / 4;
    __int64 inc_h = abs(h1 - h2);
    __int64 min_h = h1 < h2 ? h1 : h2;
    __int64 min_w = 0;
    __int64 w1 = 0;
    __int64 w2 = 0;
    __int64 inc_w = 0;
    uint32 i = 0;
    FILE *pFile = NULL;


	sprintf(szFileName,"%sSE_SLIDE_MOVE_table",g_szTmpPath);

    pFile = fopen(szFileName, "w+t");
    if (NULL == pFile)
    {
        return FALSE;
    }

    memset(TransTable, 0, sizeof(uint32) * MAX_TABLE_SIZE);

    /*
        horizental table
    */
    uTableSize = w;
    
    for (i=0; i<uTableSize; i++)
    {
        TransTable[uTableSize - i - 1] = (uint32)((h * w * w << MICRO_SHIFT_BITS) / ((h - min_h) * w * w - i * i * inc_h));       
    }

    WriteTable(pFile, "g_slide_move_horz_major_tbl_0", TransTable, uTableSize);

    h1 = h * 3 / 4; 
    h2 = h / 8;
    inc_h = abs(h1 - h2);
    min_h = h1 < h2 ? h1 : h2;

    for (i=0; i<uTableSize; i++)
    {
        TransTable[uTableSize - i - 1] = (uint32)((h * w * w << MICRO_SHIFT_BITS) / ((h - min_h) * w * w - i * i * inc_h));       
    }

    WriteTable(pFile, "g_slide_move_horz_minor_tbl_0", TransTable, uTableSize);

    h1 = h / 2; 
    h2 = h / 8;
    inc_h = abs(h1 - h2);
    min_h = h1 < h2 ? h1 : h2;

    for (i=0; i<uTableSize; i++)
    {
        TransTable[uTableSize - i - 1] = (uint32)((h * w * w << MICRO_SHIFT_BITS) / ((h - min_h) * w * w - i * i * inc_h));       
    }
    
    WriteTable(pFile, "g_slide_move_horz_minor_tbl_1", TransTable, uTableSize);

    /*
        vertical table
    */
    uTableSize = h;

    w1 = w;
    w2 = w / 4;
    min_w = w1 < w2 ? w1 : w2;
    inc_w = abs(w1 - w2);

    for (i=0; i<uTableSize; i++)
    {
        TransTable[uTableSize - i - 1] = (uint32)((w * h * h << MICRO_SHIFT_BITS) / ((w - min_w) * h * h - i * i * inc_w));       
    }

    WriteTable(pFile, "g_slide_move_vert_major_tbl_0", TransTable, uTableSize);

    w1 = w * 3 / 4;
    w2 = w / 8;
    min_w = w1 < w2 ? w1 : w2;
    inc_w = abs(w1 - w2);

    for (i=0; i<uTableSize; i++)
    {
        TransTable[uTableSize - i - 1] = (uint32)((w * h * h << MICRO_SHIFT_BITS) / ((w - min_w) * h * h - i * i * inc_w));       
    }

    WriteTable(pFile, "g_slide_move_vert_minor_tbl_0", TransTable, uTableSize);

    w1 = w / 2;
    w2 = w / 8;
    min_w = w1 < w2 ? w1 : w2;
    inc_w = abs(w1 - w2);

    for (i=0; i<uTableSize; i++)
    {
        TransTable[uTableSize - i - 1] = (uint32)((w * h * h << MICRO_SHIFT_BITS) / ((w - min_w) * h * h - i * i * inc_w));       
    }

    WriteTable(pFile, "g_slide_move_vert_minor_tbl_1", TransTable, uTableSize);

    fclose(pFile);
    return TRUE;
}

/******************************************************************************
// Description: create table of desktop rotation special effect
// Author:		shan.he
//              uWidth:	desktop width
//              uHeight: desktop height
// Output:		
// Return:		none
// Note:		the table will be output to the file named "SE_DESKTOP_ROTATE_table"
******************************************************************************/
PUBLIC BOOLEAN CreateRotDesktopTable(int32 uWidth, int32 uHeight)
{
    //char szFileName[256] = "SE_DESKTOP_ROTATE_table";
	char szFileName[MAX_FILE_PATH_LEN] = {0};
	char szText[256] = "";
    uint32 TransTable[MAX_TABLE_SIZE + 1];	
    uint32 uTableSize = 0;
    __int64 llWidth = uWidth;
    __int64 llHeight = uHeight;
    __int64 lHeightInc = 0;
    uint32 i = 0;
    FILE *pFile = NULL;

	sprintf(szFileName,"%sSE_DESKTOP_ROTATE_table",g_szTmpPath);
	
    pFile = fopen(szFileName, "w+t");
    if (NULL == pFile)
    {
        return FALSE;
    }

	////////////////////////////////////////////////////////////////////////////
    llWidth = uWidth;
    llHeight = uHeight;
    lHeightInc = 0;

	if (llWidth > MAX_TABLE_SIZE)
	{
		llWidth = MAX_TABLE_SIZE;
	}

    sprintf(szText, "PUBLIC const uint32 g_desktop_tbl_0_size = %d;", llWidth);
    WriteString(pFile, szText);

    lHeightInc = llHeight / 8;
    if (0 == lHeightInc)
    {
        lHeightInc = 1;
    }
    
    uTableSize = llWidth;
	memset(TransTable, 0, sizeof(uint32) * MAX_TABLE_SIZE);

    for (i=1; i<=llWidth; i++)
    {
        TransTable[i-1] = (llWidth * llHeight << 7)  / (lHeightInc * i);
    }   

    WriteTable(pFile, "g_desktop_rotate_small_inc_tbl_0", TransTable, uTableSize);


    lHeightInc = llHeight / 4;
    if (0 == lHeightInc)
    {
        lHeightInc = 1;
    }

	uTableSize = llWidth;
	memset(TransTable, 0, sizeof(uint32) * MAX_TABLE_SIZE);

    for (i=1; i<=llWidth; i++)
    {
        TransTable[i-1] = (llWidth * llHeight << 7)  / (lHeightInc * i);
    }   

    WriteTable(pFile, "g_desktop_rotate_large_inc_tbl_0", TransTable, uTableSize);

	////////////////////////////////////////////////////////////////////////////
    llWidth = uHeight;
    llHeight = uWidth;
    lHeightInc = 0;

	if (llWidth > MAX_TABLE_SIZE)
	{
		llWidth = MAX_TABLE_SIZE;
	}

    sprintf(szText, "PUBLIC const uint32 g_desktop_tbl_1_size = %d;", llWidth);
    WriteString(pFile, szText);

    lHeightInc = llHeight / 8;
    if (0 == lHeightInc)
    {
        lHeightInc = 1;
    }

    uTableSize = llWidth;
	memset(TransTable, 0, sizeof(uint32) * MAX_TABLE_SIZE);

    for (i=1; i<=llWidth; i++)
    {
        TransTable[i-1] = (llWidth * llHeight << 7)  / (lHeightInc * i);
    }   

    WriteTable(pFile, "g_desktop_rotate_small_inc_tbl_1", TransTable, uTableSize);

    lHeightInc = llHeight / 4;
    if (0 == lHeightInc)
    {
        lHeightInc = 1;
    }

	uTableSize = llWidth;
	memset(TransTable, 0, sizeof(uint32) * MAX_TABLE_SIZE);

    for (i=1; i<=llWidth; i++)
    {
        TransTable[i-1] = (llWidth * llHeight << 7)  / (lHeightInc * i);
    }   

    WriteTable(pFile, "g_desktop_rotate_large_inc_tbl_1", TransTable, uTableSize);

	////////////////////////////////////////////////////////////////////////////
    fclose(pFile);
    return TRUE;
}

/******************************************************************************
// Description: create table of distort move special effect
// Author:		shan.he
//              uWidth:	icon width
//              uHeight: icon height
// Output:		
// Return:		none
// Note:		the table will be output to the file named "SE_DISTORT_MOVE_table"
******************************************************************************/
PUBLIC BOOLEAN CreateDistortMoveTable(int32 uWidth, int32 uHeight)
{
    //char szFileName[256] = "SE_DISTORT_MOVE_table";
	char szFileName[MAX_FILE_PATH_LEN] = {0};

    uint32 TransTable[MAX_TABLE_SIZE + 1];	
    uint32 uTableSize = 0;
    uint32 *pTransTbl = NULL;
    __int64 llWidth = uWidth;
    __int64 llHeight = uHeight;
    
    __int64 llHeightLeft = 0;
    __int64 llHeightRight = 0;
    __int64 lHeightInc = 0;
    __int64 lWidthLeft = 0;
    __int64 lWidthRight = 0;
    __int64 lWidthBottom = 0;
    __int64 lWidthTop = 0;
    __int64 i = 0;
    __int64 lWidthInc = 0;
    __int64 lHeightTop = 0;
    FILE *pFile = NULL;

	sprintf(szFileName,"%sSE_DISTORT_MOVE_table",g_szTmpPath);

    pFile = fopen(szFileName, "w+t");
    if (NULL == pFile)
    {
        return FALSE;
    }

    memset(TransTable, 0, sizeof(uint32) * MAX_TABLE_SIZE);

    /*
        horizental table
    */
    uTableSize = llWidth;

    llHeightLeft = llHeight;
    llHeightRight = llHeight * 7 / 8;
    lHeightInc = llHeightLeft - llHeightRight;

    TransTable[0] = llHeight << MICRO_SHIFT_BITS;
     for (i=1; i<llWidth; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lHeightInc);
    }   

    WriteTable(pFile, "s_distort_horz_tbl_0", TransTable, uTableSize);

    llHeightLeft = llHeight;
    llHeightRight = llHeight * 3 / 4;
    lHeightInc = llHeightLeft - llHeightRight;

    TransTable[0] = llHeight << MICRO_SHIFT_BITS;
     for (i=1; i<llWidth; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lHeightInc);
    }   

    WriteTable(pFile, "s_distort_horz_tbl_1", TransTable, uTableSize);

    llHeightLeft = llHeight;
    llHeightRight = llHeight * 2 / 4;
    lHeightInc = llHeightLeft - llHeightRight;

    TransTable[0] = llHeight << MICRO_SHIFT_BITS;
     for ( i=1; i<llWidth; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lHeightInc);
    }   

    WriteTable(pFile, "s_distort_horz_tbl_2", TransTable, uTableSize);

    llHeightLeft = llHeight;
    llHeightRight = llHeight * 1 / 4;
    lHeightInc = llHeightLeft - llHeightRight;

    TransTable[0] = llHeight << MICRO_SHIFT_BITS;
     for ( i=1; i<llWidth; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lHeightInc);
    }   

    WriteTable(pFile, "s_distort_horz_tbl_3", TransTable, uTableSize);

    llHeightLeft = llHeight;
    llHeightRight = 0;
    lHeightInc = llHeightLeft - llHeightRight;

    TransTable[0] = llHeight << MICRO_SHIFT_BITS;
     for ( i=1; i<llWidth; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lHeightInc);
    }   

    WriteTable(pFile, "s_distort_horz_tbl_4", TransTable, uTableSize);

    llHeightLeft = llHeight * 3 / 4;
    llHeightRight = 0;
    lHeightInc = llHeightLeft - llHeightRight;
    lWidthLeft = llWidth * 3 / 4;
    
    pTransTbl = TransTable;
    for ( i=0; i<lWidthLeft; i++)
    {
        *pTransTbl++ = (llHeight * lWidthLeft << MICRO_SHIFT_BITS)  / (lWidthLeft * llHeight - lWidthLeft * llHeightLeft + i * lHeightInc);
    }   

    llHeightLeft = llHeight / 4;
    llHeightRight = 0;
    lHeightInc = abs(llHeightLeft - llHeightRight);
    lWidthLeft = llWidth / 4;

    pTransTbl = TransTable + llWidth - 1;
    for (i=0; i<lWidthLeft; i++)
    {
        *pTransTbl-- = (llHeight * lWidthLeft << MICRO_SHIFT_BITS)  / (lWidthLeft * llHeight - lWidthLeft * llHeightLeft + i * lHeightInc);
    }   

    WriteTable(pFile, "s_distort_horz_tbl_5", TransTable, uTableSize);

    llHeightLeft = llHeight * 2 / 4;
    llHeightRight = 0;
    lHeightInc = llHeightLeft - llHeightRight;
    lWidthLeft = llWidth * 2 / 4;
    
    pTransTbl = TransTable;
    for ( i=0; i<lWidthLeft; i++)
    {
        *pTransTbl++ = (llHeight * lWidthLeft << MICRO_SHIFT_BITS)  / (lWidthLeft * llHeight - lWidthLeft * llHeightLeft + i * lHeightInc);
    }   

    llHeightLeft = llHeight * 2 / 4;
    llHeightRight = 0;
    lHeightInc = llHeightLeft - llHeightRight;
    lWidthLeft = llWidth * 2 / 4;

    pTransTbl = TransTable + llWidth - 1;
    for (i=0; i<lWidthLeft; i++)
    {
        *pTransTbl-- = (llHeight * lWidthLeft << MICRO_SHIFT_BITS)  / (lWidthLeft * llHeight - lWidthLeft * llHeightLeft + i * lHeightInc);
    }   

    WriteTable(pFile, "s_distort_horz_tbl_6", TransTable, uTableSize);

    /*
    vertical table
    */
    uTableSize = llHeight;

    lWidthTop = llWidth;
    lWidthBottom = llWidth * 7 / 8;
    lWidthInc = lWidthTop - lWidthBottom;

    TransTable[0] = llWidth << MICRO_SHIFT_BITS;
     for (i=1; i<llHeight; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lWidthInc);
    }   

    WriteTable(pFile, "s_distort_vert_tbl_0", TransTable, uTableSize);

    lWidthTop = llWidth;
    lWidthBottom = llWidth * 3 / 4;
    lWidthInc = lWidthTop - lWidthBottom;

    TransTable[0] = llWidth << MICRO_SHIFT_BITS;
     for (i=1; i<llHeight; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lWidthInc);
    }   

    WriteTable(pFile, "s_distort_vert_tbl_1", TransTable, uTableSize);

    lWidthTop = llWidth;
    lWidthBottom = llWidth * 2 / 4;
    lWidthInc = lWidthTop - lWidthBottom;

    TransTable[0] = llWidth << MICRO_SHIFT_BITS;
     for (i=1; i<llHeight; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lWidthInc);
    }   

    WriteTable(pFile, "s_distort_vert_tbl_2", TransTable, uTableSize);

    lWidthTop = llWidth;
    lWidthBottom = llWidth * 1 / 4;
    lWidthInc = lWidthTop - lWidthBottom;

    TransTable[0] = llWidth << MICRO_SHIFT_BITS;
     for (i=1; i<llHeight; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lWidthInc);
    }   

    WriteTable(pFile, "s_distort_vert_tbl_3", TransTable, uTableSize);

    lWidthTop = llWidth;
    lWidthBottom = 0;
    lWidthInc = lWidthTop - lWidthBottom;

    TransTable[0] = llWidth << MICRO_SHIFT_BITS;
     for (i=1; i<llHeight; i++)
    {
        TransTable[i] = (llHeight * llWidth << MICRO_SHIFT_BITS)  / (i * lWidthInc);
    }   

    WriteTable(pFile, "s_distort_vert_tbl_4", TransTable, uTableSize);

    lWidthTop = llWidth * 3 / 4;
    lWidthBottom = 0;
    lWidthInc = lWidthTop - lWidthBottom;
    lHeightTop = llHeight * 3 / 4;
    
    pTransTbl = TransTable;
    for ( i=0; i<lHeightTop; i++)
    {
        *pTransTbl++ = (llWidth * lHeightTop << MICRO_SHIFT_BITS)  / (lHeightTop * (llWidth - lWidthTop) + i * lWidthInc);
    }   

    lWidthTop = llWidth / 4;
    lWidthBottom = 0;
    lWidthInc = abs(lWidthTop - lWidthBottom);
    lHeightTop = llHeight / 4;

    pTransTbl = TransTable + llHeight - 1;
    for (i=0; i<lHeightTop; i++)
    {
        *pTransTbl-- = (llWidth * lHeightTop << MICRO_SHIFT_BITS)  / (lHeightTop * (llWidth - lWidthTop) + i * lWidthInc);
    }   

    WriteTable(pFile, "s_distort_vert_tbl_5", TransTable, uTableSize);

    lWidthTop = llWidth * 2 / 4;
    lWidthBottom = 0;
    lWidthInc = lWidthTop - lWidthBottom;
    lHeightTop = llHeight * 2 / 4;
    
    pTransTbl = TransTable;
    for ( i=0; i<lHeightTop; i++)
    {
        *pTransTbl++ = (llWidth * lHeightTop << MICRO_SHIFT_BITS)  / (lHeightTop * (llWidth - lWidthTop) + i * lWidthInc);
    }   

    lWidthTop = llWidth * 2 / 4;
    lWidthBottom = 0;
    lWidthInc = lWidthTop - lWidthBottom;
    lHeightTop = llHeight * 2 / 4;

    pTransTbl = TransTable + llHeight - 1;
    for (i=0; i<lHeightTop; i++)
    {
        *pTransTbl-- = (llWidth * lHeightTop << MICRO_SHIFT_BITS)  / (lHeightTop * (llWidth - lWidthTop) + i * lWidthInc);
    }   

    WriteTable(pFile, "s_distort_vert_tbl_6", TransTable, uTableSize);

    fclose(pFile);

    return TRUE;
}

/******************************************************************************
// Description: create file for micros
// Author:		shan.he
//              uIconWidth:	icon width
//              uIconHeight: icon height
//              uDesktopWidth: desktop width
//              uDesktopHeight: desktop height
// Output:		
// Return:		none
// Note:		the table will be output to the file named "SE_DISTORT_MOVE_table"
******************************************************************************/
PUBLIC BOOLEAN CreateMicros(int32 uIconWidth, int32 uIconHeight, int32 uDesktopWidth, int32 uDesktopHeight)
{
    //char szFileName[256] = "SE_MICORS";
	char szFileName[MAX_FILE_PATH_LEN] = {0};
    char szMicroContext[512] = "";
    FILE *pFile = NULL;

	sprintf(szFileName,"%sSE_MICORS",g_szTmpPath);

    pFile = fopen(szFileName, "w+t");
    if (NULL == pFile)
    {
        return FALSE;
    }

    sprintf(szMicroContext, "%d", uDesktopWidth);
    WriteMicro(pFile, "DESKTOP_WIDTH", szMicroContext);
    sprintf(szMicroContext, "%d", uDesktopHeight);
    WriteMicro(pFile, "DESKTOP_HEIGHT", szMicroContext);

    sprintf(szMicroContext, "%d", uIconWidth);
    WriteMicro(pFile, "ICON_WIDTH", szMicroContext);
    sprintf(szMicroContext, "%d", uIconHeight);
    WriteMicro(pFile, "ICON_HEIGHT", szMicroContext);

    fclose(pFile);

	return TRUE;
}

PUBLIC void SetTempPath(char * pPath)
{
	int nLen =0;
	memset(g_szTmpPath,0,sizeof(g_szTmpPath));

	if(pPath != NULL)
	{	
		nLen = strlen(pPath);
		if(nLen >= (MAX_FILE_PATH_LEN-30) || nLen == 0)
		{
			return;
		}
		strcpy(g_szTmpPath,pPath);		
		if(g_szTmpPath[nLen-1] != '\\')
		{
			strcat(g_szTmpPath,"\\");		
		}	
	}
}

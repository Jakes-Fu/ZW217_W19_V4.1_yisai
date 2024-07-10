/**************************************************************************
; 
;  Centel INC.  Beijing                                                         
;  
;***************************************************************************
;    
;      Centel CONFIDENTIAL AND PROPRIETARY                                       
;      
;        This source is the sole property of Centel Inc. Reproduction or           
;        utilization of this source in whole or in part is forbidden without    
;        the written consent of Centel Inc.                                        
;        
;          (c) Copyright Centel Inc.,         2007.  All Rights Reserved.            
;          
;***************************************************************************
;            
;             tf3rd_for_hz.h                     
;              
;***************************************************************************
;                
;      Date                            Author                                               Reference              
; =============        =============                 ====================================
;   2007.05.29                  Zhuxiaoqi                                             Creation
;*****************************************************************************/
#ifndef TF3RD_FOR_HZ_FUN_H
#define TF3RD_FOR_HZ_FUN_H

typedef struct St_tm
{
  int tm_sec;       /* seconds after the minute (from 0) */
  int tm_min;       /* minutes after the hour (from 0) */
  int tm_hour;      /* hour of the day (from 0) */
  int tm_mday;      /* day of the month (from 1) */
  int tm_mon;       /* month of the year (from 0) */
  int tm_year;      /* years since 1900 (from 0) */
  int tm_wday;      /* days since Sunday (from 0) */
  int tm_yday;      /* day of the year (from 0) */
  int tm_isdst;     /* daylight saving time flag */
}St_tm;

typedef struct
{
	
  	unsigned short   nYear;
	unsigned char	  nMonth;
	unsigned char    nDay;
	unsigned char	  nHour;
	unsigned char	  nMin;
	unsigned char	  nSec;
	unsigned char	  DayIndex; /* 0=Sunday */
} TimeType;

typedef enum
{
    TF3RD_HZ_SEMAPHORE_1 = 0, //use for iwap interface 
    TF3RD_HZ_SEMAPHORE_MAX
}TF3RD_HZ_SEMAPHORE_TYPE;
//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                     //
//                                                      FSS interface                                                                              //
//                                                                                                                                                     //     
/////////////////////////////////////////////////////////////////////////////////////////
void *TS_WAPMMS_MALLOC(unsigned int allocSize, const char * pFileName,int dwLine);
void TS_WAPMMS_FREE(void *pv, const char * pFileName,int dwLine);

//file system functions
extern int     Tf3rd0_1fclose(void* f);
extern void*   Tf3rd0_2fopen(const char *filename, int mode);
extern void*   Tf3rd0_2fopenEx(const char *filename, const unsigned char * mode);
extern int     Tf3rd0_3fread(void* f, void* buf, unsigned int size);
extern int     Tf3rd0_4fseek(void* f, int offset, int whence);
extern int     Tf3rd0_5ftell(void* f);
extern int     Tf3rd0_6feof(void* f);
extern int     Tf3rd0_7ferror(void* f);
extern int     Tf3rd0_8fflush(void* f);
extern int     Tf3rd0_9fwrite(void* f, void* buf, unsigned int size);
extern int     Tf3rd0_21CheckFileExist(char * f);
extern int     Tf3rd0_22CreatFile(char * f);
extern int     Tf3rd0_10chdir(const char* path);
extern char*   Tf3rd0_11getcwd(char* buf, unsigned int size);
extern int     Tf3rd0_12makedir(const char* path);
extern int     Tf3rd0_13remove(const char* filename);
extern int     Tf3rd0_14rename(const char* pOldName, const char* pNewName);
extern int     Tf3rd0_15removedir(const char* path);
extern int     Tf3rd0_16stat(void* f, unsigned int* dwSize);
extern int     Tf3rd0_16statEx(const char* filename,  unsigned int* dwSize);
extern int     Tf3rd0_16StatX(const char* filename,  unsigned int* dwSize,unsigned int* fileTime, unsigned char* fileMode);

extern int     Tf3rd0_17truncate(const char* filename, int length);
extern int     Tf3rd0_17truncateEx(void* f,  int length);

extern void*   Tf3rd0_18opendir(const char* pDirName, unsigned char bDir);
extern char*   Tf3rd0_19readdir(void* pDirPtr);
extern int     Tf3rd0_20closedir(void* pDirPtr);
extern int     Tf3rd0_26fAccess(char* filename);

extern unsigned int  Tf3rd0_30Rand(void);
extern unsigned int  Tf3rd0_31ClockZone(void);
extern unsigned int  Tf3rd0_32Time(void);

extern unsigned int  Tf3rd0_33FreeSize(int vVolumeID);
extern int           Tf3rd0_34fsize(void* f);
extern unsigned int  Tf3rd0_34MKTime(TimeType* pTM);

extern int           Tf3rd0_37fitfreespace(unsigned int *pdwHighSize, unsigned int *pdwLowSize);
extern int           Tf3rd0_36sdcard1Vailed(void);
extern int           Tf3rd0_36sdcard2Vailed(void);
extern int           Tf3rd0_36udiskVailed(void);
extern int           Tf3rd0_37sdcard1Freespace(unsigned int *pdwHighSize, unsigned int *pdwLowSize);
extern int           Tf3rd0_37sdcard2Freespace(unsigned int *pdwHighSize, unsigned int *pdwLowSize);
extern int           Tf3rd0_37udiskFreespace(unsigned int *pdwHighSize, unsigned int *pdwLowSize);
extern void          Tf3rd0_50Lock(TF3RD_HZ_SEMAPHORE_TYPE vSemaPhoreID);
extern void          Tf3rd0_51UnLock(TF3RD_HZ_SEMAPHORE_TYPE vSemaPhoreID);

/*************************************************************/
/***********************Timer Interface*************************/
/*************************************************************/
extern unsigned int  Tf3rd0_21SetTimer(unsigned char appId, unsigned int ms);
extern unsigned int  Tf3rd0_22KillTimer(unsigned int timer_id);
extern unsigned int  Tf3rd2_78GetTickCount(void);
extern void          Tf3rd2_79ConvertTC2Date(unsigned int vTC,unsigned short* Year, unsigned char* Month, unsigned char* Day, unsigned char* Hour, unsigned char* Minute, unsigned char* Second);
extern void          Tf3rd2_GetDateTime(TimeType *pstJulDate);
extern void          Tf3rd2_GetJulDate(unsigned long uiSenconds, TimeType *pstJulDate);
//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                     //
//                                                      OS interface                                                                              //
//                                                                                                                                                     //     
/////////////////////////////////////////////////////////////////////////////////////////
//Utility functions
extern void Tf3rd2_PrintMemory(void);
extern void Tf3rd2_DeleteMemory(void);
extern void*      Tf3rd2_0MallocMemory(unsigned int allocSize, char *file, unsigned int line);
extern int        Tf3rd2_1MemCmp( const void *buf1, const void *buf2, unsigned int  count );
extern void*      Tf3rd2_2MemCpy(void *dst, const void *src, unsigned int len);
extern void*      Tf3rd2_3MemMove(void *dst, const void *src, unsigned int n);
extern void*      Tf3rd2_4MemSet(void *s, int c, unsigned int n);
extern char*      Tf3rd2_5MemStr( void *p1, const void *p2, unsigned int count );
extern void*      Tf3rd2_6Realloc(void *ptr, unsigned int size);
extern void       Tf3rd2_7MemFree(void *memBlock);
extern int        Tf3rd2_8StrAtoi(const unsigned char *str);
extern char*      Tf3rd2_9StrCat(char *dst, const char*src);
extern char*      Tf3rd2_10StrChr(const char *str, char ch);
extern int        Tf3rd2_11StrCmp(const char *str1, const char *str2);
extern char*      Tf3rd2_12StrCpy(char *dst, const char *src);
extern char*      Tf3rd2_13StrDup(const char *stringToDuplicate);
extern int        Tf3rd2_14StriCmp( const char *str1, const char *str2);
extern char*      Tf3rd2_15StriStr( const char *pszHaystack, const char *pszNeedle);
extern char*      Tf3rd2_16StrlCat(char * dst, const char * src, unsigned int nSize);
extern char*      Tf3rd2_17StrlCpy(char * dst, const char * src, unsigned int nSize);
extern int        Tf3rd2_18StrLen(const char * str);
extern char*      Tf3rd2_19StrLower(char *psz);
extern  int       Tf3rd2_20StrnCmp( const char *str1, const char *str2 , unsigned int length);
extern char*      Tf3rd2_21StrnCpy( char *strDest, const char *strSource, unsigned int count );
extern int        Tf3rd2_22StrniCmp( const char *str1, const char *str2 , unsigned int length);
extern char*      Tf3rd2_23StrrChr( const char *string, char c);
extern char*      Tf3rd2_24StrStr(const char *str1, const char *str2);
extern double     Tf3rd2_25StrTod(const char *pszFloat, char ** ppszEnd);
extern unsigned long    Tf3rd2_26StrToul( const char *nptr, char **endptr, int base );
extern unsigned short*  Tf3rd2_27Str2Wstr(char *p_AscStr, unsigned short *p_Ucs2, int nSize);
extern char*            Tf3rd2_28StrUpper(char *psz);
extern unsigned int     Tf3rd2_29Utf8ToWstr(const unsigned char *pSrc,int nLen, unsigned short *pDst, int nSize);
extern void             Tf3rd2_30Wsprintf(unsigned short *pDest, int nSize, unsigned short *pFormat,...);
extern unsigned short*  Tf3rd2_31WstrCat(unsigned short *pDest, unsigned short *pSrc);
extern unsigned short*  Tf3rd2_32WstrChr( unsigned short *s1, unsigned short c );
extern int              Tf3rd2_33WstrCmp(unsigned short *s1, unsigned short *s2);
extern void             Tf3rd2_34WstrCompress(const unsigned short *pSrc, int nLen,unsigned char *pDest, int nSize);
extern unsigned short*  Tf3rd2_35WstrCpy(unsigned short *pDest, unsigned short *pSrc);
extern unsigned short*  Tf3rd2_36WstrDup(const unsigned short *pIn);
extern unsigned int     Tf3rd2_37WstrlCat(unsigned short * dst, const unsigned short * src, unsigned int nSize);
extern unsigned int     Tf3rd2_38WstrlCpy(unsigned short * dst, const unsigned short * src, unsigned int nSize);
extern int              Tf3rd2_39WstrLen(unsigned short * str);
extern void             Tf3rd2_40WstrLower(unsigned short *pszDest);
extern int              Tf3rd2_41WstrnCmp(unsigned short *s1, unsigned short *s2, unsigned int length);
extern char*            Tf3rd2_42Wstr2Str(unsigned short *pIn, char *pszDest, int nSize);
extern unsigned int     Tf3rd2_43Wstr2Utf8(const unsigned short *pSrc,int nLen, unsigned char *pDst, int nSize);
extern void             Tf3rd2_44WstrUpper(unsigned short *pszDest);
extern double           Tf3rd2_45FCeil(double x);
extern double           Tf3rd2_47FFloor(double x);
extern unsigned char    Tf3rd2_48Float2Wstr(double v, unsigned short *psz, int nSize);
extern unsigned long    Tf3rd2_49StrCspn(char* a,char *b);
extern char*            Tf3rd2_50StrPbrk(char* a,char *b);
extern char*            Tf3rd2_51StrTok(char* a,char *b);

extern long    Tf3rd2_52StrTol(char* a,char* *b,int c);
extern void    Tf3rd2_56MSleep(unsigned int msecs);
extern void    Tf3rd2_59DbgPrintf(const char *pszFormat,...);
extern void    Tf3rd2_62CurrentTime(unsigned short* Year, unsigned char* Month, unsigned char* Day, unsigned char* Hour, unsigned char* Minute, unsigned char* Second);

extern int     Tf3rd2_60SNPrintf(char *s, unsigned int n, const char *format, ...);
extern int     Tf3rd2_60VSNPrintf(char *s, unsigned int n, const char *format, void* args);
extern int     Tf3rd2_61SPrintf(char *s, const char *format, ...);
extern int     Tf3rd2_61VSPrintf(char *s, const char *format, void* args);
extern void*   Tf3rd2_63MemChr(const void *s, int c, unsigned int n);
extern void*   Tf3rd2_64MemRChr(const void *s, int c, unsigned int n);


/////////////////////////////////////Math function////////////////////////////////////
extern int     Tf3rd2_70Abs(int param);
extern double  Tf3rd2_71Fabs(double param);
extern double  Tf3rd2_72Sqrt(double param);
extern double  Tf3rd2_73Sin(double param);
extern double  Tf3rd2_74Cos(double param);
extern double  Tf3rd2_75Tan(double param);
extern double  Tf3rd2_76AtoF(const char * param);
extern double  Tf3rd2_77Pow(double param, double exp);

///////////////////////////////Play sound////////////////////////////////////////////
extern void    Tf3rd3_20PlayMusic(const char* pFileName, unsigned char bRepeat, unsigned int vDuration );
extern void    Tf3rd3_21StopMusic(void);


//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                     //
//                                                      UI interface                                                                              //
//                                                                                                                                                     //     
/////////////////////////////////////////////////////////////////////////////////////////

/*------------------------------image decode interface-----------------------------------------*/

extern void         Tf3rd3_9CompressPic(unsigned char  *Src_data,unsigned short srcWidth, unsigned short srcHeight,unsigned short Src_x,unsigned short  Src_y,unsigned short  Srcimg_width,unsigned short  Srcimg_height,unsigned short  Displaywidth,unsigned short Displayheight,unsigned short  Des_x,unsigned short  Des_y,unsigned short desWidth, unsigned short desHeight,unsigned char  *Des_data);
extern unsigned int Tf3rd3_12GetImageInfo(void *pImageBuffer,int enumImageType,void *pImageInfo );          		
extern unsigned int Tf3rd3_13DecodeImageToBmp(void *pImageBuffer, 
                                              unsigned long BufferSize,
                                              int enumImageType,
                                              int  FrameIndex, // 如果是动画，这里表示取用第几帧，不是动画，这个值无意义
                                              unsigned int pGifHandle,
                                              int *pDelayTime,
                                              void *pBmpBuffer,
                                              int vBmpWidth,
                                              int vBmpHeight);

extern int Tf3rd3_22GetCurFrameInfo(unsigned int pGifHandle , int *dx, int*dy, int* dWidth,int * dHeight);
extern int Tf3rd3_23CheckGIFIsLastFrame(unsigned int pGifHandle);


/*-------------------------------displaying interface---------------------------------*/

extern void Tf3rd4_0DrawPoint(unsigned char *Des_buf, unsigned short vp_Width, unsigned short vp_Height, unsigned short x , unsigned short y, unsigned short vp_Color, unsigned char vp_Mode);
extern void Tf3rd4_01DrawLine(unsigned char * Des_buf,unsigned short Area_width,unsigned short Area_height,unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned short vp_Color,unsigned char vp_Mode);
extern void Tf3rd4_02DrawRect( unsigned char * Des_buf,unsigned short Area_width,unsigned short Area_height,unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,const unsigned short v_ColorIdx,const unsigned char v_Mode);
extern void Tf3rd4_04FillRect(unsigned char * Des_buf,unsigned short Area_width,unsigned short Area_height,unsigned short vl_x, unsigned short vl_y,unsigned short  vl_Width, unsigned short vl_Height,unsigned short v_BackgroudColor);
extern void Tf3rd4_05DrawArc(unsigned short cx, unsigned short cy, unsigned short w, unsigned short h, unsigned short s, unsigned short e, const unsigned short v_ColorIdx,const unsigned char v_Mode);
extern void Tf3rd4_06ReverseZone(unsigned char * Des_buf,unsigned short Area_width,unsigned short Area_height,unsigned short vl_x, unsigned short vl_y,unsigned short  vl_Width, unsigned short vl_Height);
extern void Tf3rd4_07RefreshAreaScreen(unsigned short v_x1,unsigned short v_y1,unsigned short v_x2,unsigned short v_y2);
extern void Tf3rd4_08DisplayImage(unsigned char * vp_BmpData, unsigned short ImgWidth,unsigned short ImgHeight,unsigned short v_x, unsigned short v_y, unsigned char v_Mode);
extern unsigned short*  Tf3rd4_09Get_ScreenBuffer(void);
extern unsigned short   Tf3rd4_10Get_ScreenBuffer_Width(void);
extern unsigned short   Tf3rd4_11Get_ScreenBuffer_Height(void);
extern void             Tf3rd4_12BitBlt(unsigned char * pbmDest,unsigned short desWidth, unsigned short desHeight, unsigned short xDest,unsigned short yDest,unsigned short cxDest,unsigned short cyDest,unsigned char * pbmSource,unsigned short srcWidth,unsigned short srcHeight, unsigned short xSrc, unsigned short ySrc, unsigned short cxSrc,unsigned short cySrc,int  v_mode,unsigned short v_Transpantcolor);

/*-------------------------------string displaying---------------------------------*/

extern unsigned short Tf3rd5_0GetPixelNumofString(unsigned char v_CharSet,unsigned char * p_Str, unsigned short v_StrLen);
extern unsigned short Tf3rd5_1GetCharNumofCertainPixel(unsigned char v_CharSet, unsigned char * p_Str, unsigned short v_StrLen, unsigned short v_PixelNum);
extern unsigned char  Tf3rd5_2GetCharactorWidth(unsigned short v_CharCode);
extern unsigned char  Tf3rd5_3GetCharactorHeight(void);
extern unsigned short Tf3rd5_4DrawStringinBuffer(unsigned short *p_AreaBuf, unsigned short v_Width, unsigned short v_Height, 
                                                 unsigned short v_x, unsigned short v_y, unsigned char *p_String, unsigned short v_StrLen, 
                                                 unsigned char v_CharSet, unsigned char v_Mode, unsigned short v_Color);					
extern unsigned short Tf3rd5_5DrawStringinBufferEx(unsigned short *p_AreaBuf, unsigned short v_Width, unsigned short v_Height, 
                                                   short v_x, short v_y, unsigned char *p_String, unsigned short  v_StrLen, 
                                                   unsigned char v_CharSet,unsigned char v_Mode, unsigned short v_Color, 
                                                   unsigned short v_clipLeft, unsigned short v_clipTop,   unsigned short v_clipRight, unsigned short v_clipBottom);

																					
#endif

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
#ifndef TF3RD_FOR_HZ_H
#define TF3RD_FOR_HZ_H
#include "tf3rd_for_hz_fun.h"

#define TS_SYS_C_DISK               "C:\\\\"			/*sys disk*/
#define TS_SYS_D_DISK               "D:\\\\"			/*mms&wap sys disk*/
#define TS_USER_DISK                "E:\\\\"			/*mms&wap user data disk*/

#define TS_SYSDISK_C_SPACE          "C:\\"			/*system disk space path: C */
#define TS_SYSDISK_D_SPACE          "D:\\"			/*system disk space path: D */
#define TS_USERDISK_SPACE           "E:\\"			/*User disk space path: T-flash*/

#define TS_ROOT_PATH                TS_SYSDISK_D_SPACE"TechSoft"

#define TS_MMS_DIR_PATH             TS_ROOT_PATH"\\mms"		/*mms data directory*/
#define TS_PUSH_DIR_PATH            TS_ROOT_PATH"\\push"		/*mms data directory*/
#define TS_WAP_DIR_PATH             TS_ROOT_PATH"\\wap"		/* WAP browser data directory*/

#define TS_MMS_SETTINGS_DIR_PATH	   TS_MMS_DIR_PATH"\\settings"		/*mms data directory*/

#define TS_TIMER_MAX	               10
#define TS_TIMER_NAME	            "TS_TIMER"

#define TS_MMS_INBOX                "Inbox"
#define TS_MMS_OUTBOX               "Outbox"
#define TS_MMS_SENT                 "Sent"
#define TS_MMS_DRAFTS               "Drafts"
#define TS_MMS_TEMPLATES            "Templates"

#define TS_MMS_INBOX_ID             1
#define TS_MMS_OUTBOX_ID		      2
#define TS_MMS_SENT_ID			      3
#define TS_MMS_DRAFTS_ID		      4
#define TS_MMS_TEMPLATES_ID	      5

// #if defined(__MTK_TARGET__)
// #include "kal_release.h"
// #include "stack_timer.h" 
// 
// //#ifdef TF3RD_TARGET_DBG_INFO
// #ifndef TF3RD_TARGET_DBG_INFO
// #define TF3RD_TARGET_DBG_INFO		/*print debug info on catcher tools*/
// #endif
// 
// typedef struct
// {
// 	stack_timer_struct 		stTimerStruct;
// 	int             			iIsUsed;	//0 for nost,1 for used
// }St_TS_Timer;
// 
// #endif

typedef struct tagSt_TS_Msg_Struct
{
   //LOCAL_PARA_HDR
   /*MMS TM*/
   unsigned int uiTMResult;  //response from tm
   unsigned int uiMsgId;
   unsigned int uiTransType;
   void         *pvQMETM;      

   /*Push*/
   void         *pvPushBuff;  // freed when receive

   /* User data */
   void         *pvUserData; // freed when receive
}St_TS_Msg_Struct;

/*****************************************************/
/*****************Be careful!!!*************************/
/********please don't use function directly******************/
/*****************************************************/
enum
{
  TF3RD_VOLUME_NOR,
  TF3RD_VOLUME_NAN,
  TF3RD_VOLUME_TFLASH
};

enum
{
  TF3RD_SEEK_SET,
  TF3RD_SEEK_CUR,
  TF3RD_SEEK_END
};

enum
{
   TF3RD_OFM_READ,
   TF3RD_OFM_READWRITE,
   TF3RD_OFM_CREATE,
   TF3RD_OFM_APPEND
};

typedef enum
{
    TF3RD_HZ_APP_WAP_STACK = 1,  
    TF3RD_HZ_APP_WAP_ENG,  
    TF3RD_HZ_APP_MMS_ENG,  
    TF3RD_HZ_APP_MAX
}TF3RD_HZ_APP_TYPE;

/*
Add by jade for support set directory path
08/09/2008
*/
typedef enum
{
   /*mms type*/
   E_PATH_TYPE_ROOT_PATH,
//   E_PATH_TYPE_MMS_PATH,
//   E_PATH_TYPE_MMS_CARD_PATH,
//   E_PATH_TYPE_MMS_FOLDER_PATH,
//   E_PATH_TYPE_MMS_TRASH_PATH,
//   E_PATH_TYPE_MMS_INFO_PATH,
//   E_PATH_TYPE_MMS_MEDIA_PATH,
   /*push type*/
//   E_PATH_TYPE_PUSH_SMSSTATE_PATH,
//   E_PATH_TYPE_PUSH_PATH,
//   E_PATH_TYPE_PUSH_SMS_PATH,
//   E_PATH_TYPE_PUSH_DM_PATH,
//   E_PATH_TYPE_PUSH_PDU_PATH,
   /*email type*/
   E_PATH_TYPE_EMAIL_PATH,
   E_PATH_TYPE_EMAIL_EML_PATH,
   E_PATH_TYPE_EMAIL_EML_CARD_PATH,
   E_PATH_TYPE_EMAIL_ACCOUNT_PATH,
   E_PATH_TYPE_EMAIL_INDEX_PATH,
   E_PATH_TYPE_EMAIL_TEMP_PATH,
   E_PATH_TYPE_EMAIL_TEMP_CARD_PATH,

   E_PATH_TYPE_EMAIL_ACTIVE_ACCOUNT,

   E_PATH_TYPE_NUMBER_MAX
}E_PathType;

typedef enum
{
   E_ACTIVE_PATH_IS_PHONE,
   E_ACTIVE_PATH_IS_CARD
}E_ActivePathType;

extern char* BRWENG10_GetPathByType(E_PathType eType);
extern void BRWENG10_FreeDirPath(void);
extern int BRWENG10_SetSaveAsPath(char* pcPath);
extern void BREWNG10_SetCardIsActive(void);
extern void BREWNG10_SetPhoneIsActive(void);
extern E_ActivePathType BRWENG10_GetActivePathType(void);

#define BRWENG10_SETDIRPATH         BRWENG10_SetMMSEDirPath
#define BREWNG10_SETSCARDPATH       BRWENG10_SetSaveAsPath
#define BRWENG10_GETDIRPATH         BRWENG10_GetPathByType
#define BRWENG10_DELDIRPATH         BRWENG10_FreeDirPath
#define BRWENG10_SETCARDACTIVE      BREWNG10_SetCardIsActive
#define BRWENG10_SETPHONEACTIVE     BREWNG10_SetPhoneIsActive
#define BRWENG10_GETACTIVEPATHTYPE  BRWENG10_GetActivePathType

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                     //
//                                                      FSS interface                                                                              //
//                                                                                                                                                     //     
/////////////////////////////////////////////////////////////////////////////////////////
//file system macro defination
#define TF3RD_FCLOSE                Tf3rd0_1fclose
#define TF3RD_FOPEN                 Tf3rd0_2fopen
#define TF3RD_FREAD                 Tf3rd0_3fread
#define TF3RD_FSEEK                 Tf3rd0_4fseek
#define TF3RD_FTELL                 Tf3rd0_5ftell
#define TF3RD_TEOF                  Tf3rd0_6feof
#define TF3RD_FERROR                Tf3rd0_7ferror
#define TF3RD_FFLUSH                Tf3rd0_8fflush
#define TF3RD_FWRITE                Tf3rd0_9fwrite
#define TF3RD_CHDIR                 Tf3rd0_10chdir
#define TF3RD_GETCWD                Tf3rd0_11getcwd
#define TF3RD_MAKEDIR               Tf3rd0_12makedir
#define TF3RD_REMOVE                Tf3rd0_13remove
#define TF3RD_RENAME                Tf3rd0_14rename
#define TF3RD_REMOVEDIR             Tf3rd0_15removedir
#define TF3RD_STAT                  Tf3rd0_16stat
#define TF3RD_STATX                 Tf3rd0_16StatX
#define TF3RD_FSTAT                 Tf3rd0_16statEx
#define TF3RD_TRUNCATE              Tf3rd0_17truncate
#define TF3RD_FTRUNCATE             Tf3rd0_17truncateEx
#define TF3RD_OPENDIR               Tf3rd0_18opendir
#define TF3RD_READDIR               Tf3rd0_19readdir
#define TF3RD_CLOSEDIR              Tf3rd0_20closedir
#define TF3RD_SETTIMER              Tf3rd0_21SetTimer
#define TF3RD_KILLTIMER             Tf3rd0_22KillTimer
#define TF3RD_FITFREESPACE          Tf3rd0_37fitfreespace
#define TF3RD_SD1VAILED             Tf3rd0_36sdcard1Vailed     /*SDCard1 vailed*/
#define TF3RD_SD2VAILED             Tf3rd0_36sdcard2Vailed     /*SDCard2 vailed*/
#define TF3RD_UDVAILED              Tf3rd0_36udiskVailed       /*UDisk vailed*/
#define TF3RD_SD1FREESPACE          Tf3rd0_37sdcard1Freespace  /*SDCard1 free space*/
#define TF3RD_SD2FREESPACE          Tf3rd0_37sdcard2Freespace  /*SDCard2 free space*/
#define TF3RD_UDFREESPACE           Tf3rd0_37udiskFreespace    /*UDisk free space*/

#define TF3RD_FACCESS               Tf3rd0_26fAccess

#define TF3RD_RAND                  Tf3rd0_30Rand
#define TF3RD_CLOCKZONE             Tf3rd0_31ClockZone
#define TF3RD_TIME                  Tf3rd0_32Time
#define TF3RD_MKTIME                Tf3rd0_34MKTime
#define TF3RD_TICKCOUNT             Tf3rd2_78GetTickCount
#define TF3RD_GETDATETIME				Tf3rd2_GetDateTime
#define TF3RD_GETJULDATE            Tf3rd2_GetJulDate
#define TF3RD_SEMAPHORE_LOCK        Tf3rd0_50Lock
#define TF3RD_SEMAPHORE_UNLOCK      Tf3rd0_51UnLock

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                     //
//                                                      OS interface                                                                              //
//                                                                                                                                                     //     
/////////////////////////////////////////////////////////////////////////////////////////
//Utility macro
#define TF3RD_PRINTMEMORY                     Tf3rd2_PrintMemory
#define TF3RD_DELETEMEMORY                   Tf3rd2_DeleteMemory
#define TF3RD_MALLOCMEMORY(size)          Tf3rd2_0MallocMemory(size,__FILE__,__LINE__)
#define TF3RD_MEMCMP                Tf3rd2_1MemCmp
#define TF3RD_MEMCPY                Tf3rd2_2MemCpy
#define TF3RD_MEMMOVE               Tf3rd2_3MemMove
#define TF3RD_MEMSET                Tf3rd2_4MemSet
#define TF3RD_MEMSTR                Tf3rd2_5MemStr
#define TF3RD_REALLOC               Tf3rd2_6Realloc
#define TF3RD_MEMFREE               Tf3rd2_7MemFree
#define TF3RD_STRATOI               Tf3rd2_8StrAtoi
#define TF3RD_STRCAT                Tf3rd2_9StrCat
#define TF3RD_STRCHR                Tf3rd2_10StrChr 
#define TF3RD_STRCMP                Tf3rd2_11StrCmp
#define TF3RD_STRCPY                Tf3rd2_12StrCpy
#define TF3RD_STRDUP                Tf3rd2_13StrDup
#define TF3RD_STRICMP               Tf3rd2_14StriCmp
#define TF3RD_STRISTR               Tf3rd2_15StriStr
#define TF3RD_STRLCAT               Tf3rd2_16StrlCat
#define TF3RD_STRLCPY               Tf3rd2_17StrlCpy
#define TF3RD_STRLEN                Tf3rd2_18StrLen
#define TF3RD_STRLOWER              Tf3rd2_19StrLower
#define TF3RD_STRNCMP               Tf3rd2_20StrnCmp
#define TF3RD_STRNCPY               Tf3rd2_21StrnCpy
#define TF3RD_STRNICMP              Tf3rd2_22StrniCmp
#define TF3RD_STRRCHR               Tf3rd2_23StrrChr
#define TF3RD_STRSTR                Tf3rd2_24StrStr
#define TF3RD_STRTOD                Tf3rd2_25StrTod
#define TF3RD_STRTOUL               Tf3rd2_26StrToul
#define TF3RD_STR2WSTR              Tf3rd2_27Str2Wstr
#define TF3RD_STRUPPER              Tf3rd2_28StrUpper
#define TF3RD_UTF8TOWSTR            Tf3rd2_29Utf8ToWstr
#define TF3RD_WSPRINTF              Tf3rd2_30Wsprintf
#define TF3RD_WSTRCAT               Tf3rd2_31WstrCat
#define TF3RD_WSTRCHR               Tf3rd2_32WstrChr
#define TF3RD_WSTRCMP               Tf3rd2_33WstrCmp
#define TF3RD_WSTRCOMPRESS          Tf3rd2_34WstrCompress
#define TF3RD_WSTRCPY               Tf3rd2_35WstrCpy
#define TF3RD_WSTRDUP               Tf3rd2_36WstrDup
#define TF3RD_WSTRLCA               Tf3rd2_37WstrlCat
#define TF3RD_WSTRLCPY              Tf3rd2_38WstrlCpy
#define TF3RD_WSTRLEN               Tf3rd2_39WstrLen
#define TF3RD_WSTRLOWER             Tf3rd2_40WstrLower
#define TF3RD_WSTRNCMP              Tf3rd2_41WstrnCmp
#define TF3RD_WSTR2STR              Tf3rd2_42Wstr2Str
#define TF3RD_WSTR2UTF8             Tf3rd2_43Wstr2Utf8
#define TF3RD_WSTRUPPER             Tf3rd2_44WstrUpper
#define TF3RD_FCEIL                 Tf3rd2_45FCeil
#define TF3RD_FFLOOR                Tf3rd2_47FFloor
#define TF3RD_FLOAT2WSTR            Tf3rd2_48Float2Wstr
#define TF3RD_MSLEEP                Tf3rd2_56MSleep

#define TF3RD_DBGPRINTF             Tf3rd2_59DbgPrintf("%s, %d", (TF3RD_STRRCHR(__FILE__, '/') + 1), __LINE__);Tf3rd2_59DbgPrintf

#define TF3RD_CURRENTTIME           Tf3rd2_62CurrentTime
#define TF3RD_TC2DATE               Tf3rd2_79ConvertTC2Date

#define TF3RD_SPRINTF               Tf3rd2_61SPrintf
#define TF3RD_VSPRINTF              Tf3rd2_61VSPrintf
#define TF3RD_SNPRINTF              Tf3rd2_60SNPrintf
#define TF3RD_VSNPRINTF             Tf3rd2_60VSNPrintf

#define TF3RD_MEMCHR                Tf3rd2_63MemChr
#define TF3RD_MEMRCHR               Tf3rd2_64MemRChr

#define TF3RD_VOLUME_FREESIZE       Tf3rd0_33FreeSize

/////////////////////////////////////Math function////////////////////////////////////
#define TF3RD_ABS                   Tf3rd2_70Abs
#define TF3RD_FABS                  Tf3rd2_71Fabs
#define TF3RD_SQRT                  Tf3rd2_72Sqrt
#define TF3RD_SIN                   Tf3rd2_73Sin
#define TF3RD_COS                   Tf3rd2_74Cos
#define TF3RD_TAN                   Tf3rd2_75Tan
#define TF3RD_ATOF                  Tf3rd2_76AtoF

#define TF3RD_POW                   Tf3rd2_77Pow
////////////////////////////////sound interface//////////////////////////////////////////////
#define TF3RD_PLAYMUSIC             Tf3rd3_20PlayMusic
#define TF3RD_STOPMUSIC             Tf3rd3_21StopMusic

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                     //
//                                                      UI interface                                                                              //
//                                                                                                                                                     //     
/////////////////////////////////////////////////////////////////////////////////////////
enum
{
   TF3RD_RO_OR,
   TF3RD_RO_XOR,
   TF3RD_RO_COPY,
   TF3RD_RO_NOT,
   TF3RD_RO_MERGENOT,
   TF3RD_RO_ANDNOT,
   TF3RD_RO_TRANSPARENT,
   TF3RD_RO_AND,
   TF3RD_RO_BLEND,
   TF3RD_RO_TOTAL
};
#define TF3RD_RO_MASK           				TF3RD_RO_TRANSPARENT
#define TF3RD_RO_MASKNOT        				TF3RD_RO_ANDNOT

enum
{													
    TF3RD_IMAGE_FORMAT_BMP,													
    TF3RD_IMAGE_FORMAT_WBMP,													
    TF3RD_IMAGE_FORMAT_GIF,													
    TF3RD_IMAGE_FORMAT_JPG,													
    TF3RD_IMAGE_FORMAT_PNG,													
    TF3RD_IMAGE_FORMAT_ICO,													
    TF3RD_IMAGE_FORMAT_TIF													
};	

enum
{
    TF3RD_CHAR_SET_UCS2,
    TF3RD_CHAR_SET_GSM0338,
    TF3RD_CHAR_SET_ASCII,
    TF3RD_CHAR_SET_UTF8
};

/**********现只能获得width和height*******************/
typedef struct TF3RD_IMAGEINFO													
{													
   unsigned short   Width;													
   unsigned short   Height;													
   unsigned short   Colors;		//保留项											
   unsigned short   FrameCount;  // 如果是动画，这里表示有多少帧													
   unsigned int   srcBufferSize; //用于传入输入的buffer大小
} t_TF3RD_ImageInfo;

/*------------------------------image decode interface-----------------------------------------*/

#define TF3RD_FOR_HZ_COMPRESS_PICTURE(p_Buffer, Displaywidth, Displayheight) \
         Tf3rd3_9CompressPic( p_Buffer, Displaywidth, Displayheight)

#define TF3RD_FOR_HZ_GET_IMAGE_INFO(pImageBuffer, enumImageType,pImageInfo ) \
         Tf3rd3_12GetImageInfo(pImageBuffer, enumImageType,pImageInfo )

#define TF3RD_FOR_HZ_DECODE_IMAGE_TO_BMP(pImageBuffer,BufferSize,enumImageType,FrameIndex, pGifHandle,pDelayTime,pBmpBuffer,vBmpWidth,vBmpHeight) \
         Tf3rd3_13DecodeImageToBmp(pImageBuffer,BufferSize,enumImageType,FrameIndex, pGifHandle,pDelayTime,pBmpBuffer,vBmpWidth,vBmpHeight)

#define TF3RD_FOR_HZ_GET_CURGIFFRAME_INFO    Tf3rd3_22GetCurFrameInfo

#define TF3RD_FOR_HZ_CHECK_GIF_ISLAST_FRAME  Tf3rd3_23CheckGIFIsLastFrame

/*----------------------------------displaying interface---------------------------------*/


#define MC_TF3RD_HZ_DRAW_DOT(Des_buf, Area_width, Area_height, x , y,  vp_Color, vp_Mode) \
			Tf3rd4_0DrawPoint(Des_buf, Area_width, Area_height, x , y,  vp_Color, vp_Mode)

#define MC_TF3RD_HZ_DRAW_LINE(Des_buf,Area_width,Area_height,x1, y1, x2, y2,vp_Color,vp_Mode) \
			Tf3rd4_01DrawLine(Des_buf,Area_width,Area_height,x1, y1, x2, y2,vp_Color,vp_Mode)

#define MC_TF3RD_DRAW_RECT(Des_buf, Area_width,Area_height,x1, y1,  x2, y2, v_ColorIdx, v_Mode) \
			Tf3rd4_02DrawRect(Des_buf, Area_width,Area_height,x1, y1,  x2, y2, v_ColorIdx, v_Mode)

#define MC_TF3RD_HZ_FILL_RECT(Des_buf,Area_width,Area_height,vl_x,  vl_y,  vl_Width,  vl_Height, v_BackgroudColor) \
			Tf3rd4_04FillRect(Des_buf,Area_width,Area_height,vl_x,  vl_y,  vl_Width,  vl_Height, v_BackgroudColor)
			
#define MC_TF3RD_HZ_DRAW_ARC( vl_x, vl_y, vl_Width, vl_Height, vl_Start_angle, vl_End_angle,vl_Color,vl_Mode) \
			Tf3rd4_05rawArc( vl_x, vl_y, vl_Width, vl_Height, vl_Start_angle, vl_End_angle,vl_Color,vl_Mode)	

#define MC_TF3RD_HZ_REVERSE_ZONE(Des_buf, Area_width,Area_height,vl_x, vl_y,vl_Width, vl_Height) \
			Tf3rd4_06ReverseZone(Des_buf, Area_width,Area_height,vl_x, vl_y,vl_Width, vl_Height)

#define MC_TF3RD_HZ_REFRESH_AREA_SCREEN(v_x1,v_y1, v_x2, v_y2) \
         Tf3rd4_07RefreshAreaScreen(v_x1,v_y1, v_x2, v_y2)

#define MC_TF3RD_HZ_DISPLAY_IMAGE(vp_BmpData, ImgWidth,ImgHeight,v_x, v_y,  v_Mode) \
         Tf3rd4_08DisplayImage(vp_BmpData, ImgWidth,ImgHeight,v_x, v_y,  v_Mode)

#define MC_TF3RD_HZ_GET_SCREENBUFFER() \
         Tf3rd4_09Get_ScreenBuffer()
                    
#define MC_TF3RD_HZ_GET_SCREENBUFFER_WIDTH()\
         Tf3rd4_10Get_ScreenBuffer_Width()
                    
#define MC_TF3RD_HZ_GET_SCREENBUFFER_HEIGHT()\
         Tf3rd4_11Get_ScreenBuffer_Height()

#define MC_TF3RD_HZ_BitBlt(pbmDest, desWidth,desHeight,  xDest, yDest, cxDest,cyDest, pbmSource,srcWidth, srcHeight, xSrc, ySrc,  cxSrc, cySrc,v_mode,v_color) \
         Tf3rd4_12BitBlt(pbmDest, desWidth,desHeight,  xDest, yDest, cxDest,cyDest, pbmSource,srcWidth, srcHeight, xSrc, ySrc,  cxSrc, cySrc,v_mode,v_color)


/*-------------------------------string displaying---------------------------------*/

#define MC_TF3RD_HZ_GET_PXL_NUM_OF_STR(v_CharSet, p_Str, v_StrLen)\
         Tf3rd5_0GetPixelNumofString(v_CharSet, , p_Str, v_StrLen)
 			
#define MC_TF3RD_HZ_GET_CHAR_NUM_OF_CERTAIN_PXL(v_CharSet, p_Str, v_StrLen, v_PixelNum)\
         Tf3rd5_1GetCharNumofCertainPixel(v_CharSet, p_Str, v_StrLen, v_PixelNum)
 			
#define MC_TF3RD_HZ_GET_CHAR_WIDTH(v_CharCode)\
         Tf3rd5_2GetCharactorWidth(v_CharCode)
 			
#define MC_TF3RD_HZ_GET_CHAR_HEIGHT()\
         Tf3rd5_3GetCharactorHeight()
 			
#define MC_TF3RD_HZ_DRAW_STR_IN_BUFFER(p_AreaBuf, \
                                       v_Width, v_Height,\
                                       v_x, v_y, \
                                       p_String, v_StrLen,\
                                       v_CharSet,\
                                       v_Mode, \
                                       v_Color) \
         Tf3rd5_4DrawStringinBuffer(p_AreaBuf, \
                                    v_Width, v_Height,\
                                    v_x, v_y, \
                                    p_String, v_StrLen,\
                                    v_CharSet, \
                                    v_Mode, \
                                    v_Color)
 			
#define MC_TF3RD_HZ_DRAW_STR_IN_BUFFER_EX(p_AreaBuf, \
                                          v_Width, v_Height,\
                                          v_x, v_y, \
                                          p_String, v_StrLen,\
                                          v_CharSet, \
                                          v_Mode, \
                                          v_Color,\
                                          v_clipLeft,v_clipTop, v_clipRight, v_clipBottom) \
         Tf3rd5_5DrawStringinBufferEx(p_AreaBuf, \
                                      v_Width, v_Height,\
                                      v_x, v_y, \
                                      p_String, v_StrLen,\
                                      v_CharSet, \
                                      v_Mode, \
                                      v_Color,\
                                      v_clipLeft,v_clipTop, v_clipRight, v_clipBottom)


#endif

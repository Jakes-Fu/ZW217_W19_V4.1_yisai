
/*************************************************************************
 ** File Name:      met_mex_aq.h                                    *
** Author:         zhenmu(zhenjie.shen)                                           *
** Date:           2011/03/15                                        *
** Copyright:      2011 METEK. All Rights Reserved.  *
** Description:    This file is the interface of area query               *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
** 2011/03/15      zhenmu(zhenjie.shen)       Create.                              *
*************************************************************************/
 
#ifndef __MET_MEX_AQ_H__
#define __MET_MEX_AQ_H__

#include "mexlib.h"
#include "window_parse.h"
#include "mmicc_export.h"
#include "met_mex_export.h"


#define MET_MEX_AQ_DEBUG



#define MET_MAXQUHAO 350			//最大城市数即是实际城市数
#define MET_MAXCITY	     352			//实际城市数+未知城市+sixnum
#define MET_MAXSIXNUM 280			//最大的特殊号码段
#define MET_MAXPHONENUM	100000	//最大手机号码个数
#define MET_TOSIXNUMINDEX 350

typedef struct 
{
	char phonenum[10];
	short index;
}MET_SIXNUMINDEX;
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)

PUBLIC void MMIMEX_AQIPPhNum(MMI_PARTY_NUMBER_T party_num,
											CC_CALL_CONTEXT_T *p_content, 
											uint8 copy_len,
											uint8 *tele_num,
											uint8 *p_tele_len);

PUBLIC int32 MMIMEX_AQPhoneWall(MN_DUAL_SYS_E dual_sys,MN_CALLING_NUMBER_T *calling_num);

PUBLIC BOOLEAN MMIMEX_AQCCFindLocalInfo(uint8 *origin_number_arr,	
									    int16  origin_number_len ,	
										MMI_STRING_T* mmi_str,
									    uint16* str
										);



PUBLIC int32 MMIMEX_CheckSetDivertCall(int32 is_true);

PUBLIC int32 MMIMEX_CheckCancelDivertCall(int32 is_true);

PUBLIC int32 MMIMEX_CheckQueryDivertCall(int32 is_true,int32 status);

#endif


#if 0
const  MET_SIXNUMINDEX met_sixnumindexarray[MET_MAXSIXNUM]
={
{"13500920",24},{"13500921",24},{"13500924",24}
,{"13500925",24},{"13500926",24},{"13500927",24}
,{"13500928",24},{"13500971",26},{"13514350",26}
,{"13514352",26},{"13514354",26},{"13514357",26}
,{"13514358",26},{"13630680",24},{"13630681",24}
,{"13630682",24},{"13630684",24},{"13630685",24}
,{"13630686",24},{"13630687",24},{"13630688",24}
,{"13630689",24},{"13630690",24},{"13630691",24}
,{"13630692",24},{"13630693",24},{"13630694",24}
,{"13630699",24},{"13634330",24},{"13634332",24}
,{"13634333",24},{"13634334",24},{"13634336",24}
,{"13634337",24},{"13634338",24},{"13634350",26}
,{"13634356",26},{"13634358",26},{"13634430",24}
,{"13634431",24},{"13634433",24},{"13634434",24}
,{"13634435",24},{"13634436",24},{"13634437",24}
,{"13634438",24},{"13634450",26},{"13634451",26}
,{"13634453",26},{"13634454",26},{"13634455",26}
,{"13634456",26},{"13634457",26},{"13634458",26}
,{"13704431",24},{"13704432",24},{"13704433",24}
,{"13704434",24},{"13704435",24},{"13704436",24}
,{"13704437",24},{"13704438",24},{"13704457",26}
,{"13704460",24},{"13704461",24},{"13704462",24}
,{"13704463",24},{"13704464",24},{"13704465",24}
,{"13704467",24},{"13704468",24},{"13704469",24}
,{"13709700",315},{"13709701",315},{"13709702",314}
,{"13709703",315},{"13709704",314},{"13709705",315}
,{"13709706",315},{"13709707",315},{"13709708",315}
,{"13709709",315},{"13709930",332},{"13709931",332}
,{"13709932",332},{"13709933",332},{"13709934",332}
,{"13709935",338},{"13709936",338},{"13709937",338}
,{"13709938",338},{"13709939",338},{"13804431",24}
,{"13804432",24},{"13804433",24},{"13804434",24}
,{"13804435",24},{"13804436",24},{"13804437",24}
,{"13804438",24},{"13804450",26},{"13804454",26}
,{"13804457",26},{"13804458",26},{"13804459",26}
,{"13809980",343},{"13809981",343},{"13809982",343}
,{"13809983",343},{"13809984",343},{"13809985",342}
,{"13809986",342},{"13809987",342},{"13809988",342}
,{"13809989",342},{"13843330",24},{"13843331",24}
,{"13843332",24},{"13843333",24},{"13843334",24}
,{"13843335",24},{"13843336",24},{"13843338",24}
,{"13843339",24},{"13903670",13},{"13903671",13}
,{"13903672",13},{"13903673",13},{"13903674",13}
,{"13903675",12},{"13903676",12},{"13903677",12}
,{"13903678",12},{"13903679",12},{"13904450",26}
,{"13904451",26},{"13904454",26},{"13904457",26}
,{"13904458",26},{"13904459",26},{"13904474",24}
,{"13904475",24},{"13904476",24},{"13904477",24}
,{"13904478",24},{"13904670",13},{"13904671",13}
,{"13904672",13},{"13904673",13},{"13904674",13}
,{"13904675",12},{"13904676",12},{"13904677",12}
,{"13904678",12},{"13904679",12},{"13904680",14}
,{"13904681",14},{"13904682",14},{"13904683",14}
,{"13904684",14},{"13904685",15},{"13904686",15}
,{"13904687",15},{"13904688",15},{"13904689",15}
,{"13904880",14},{"13904881",14},{"13904882",14}
,{"13904883",14},{"13904884",14},{"13904885",15}
,{"13904886",15},{"13904887",15},{"13904888",15}
,{"13904889",15},{"13909720",315},{"13909721",315}
,{"13909722",314},{"13909723",315},{"13909724",314}
,{"13909725",315},{"13909726",315},{"13909727",315}
,{"13909728",315},{"13909729",315},{"13943340",26}
,{"13943341",26},{"13943342",26},{"13943343",26}
,{"13943349",26},{"13943380",24},{"13943381",24}
,{"13943382",24},{"13943383",24},{"13943384",24}
,{"13943385",24},{"13943530",26},{"13943531",26}
,{"13943532",26},{"13943537",26},{"13943538",26}
,{"13943539",26},{"13500922",28},{"13500923",28}
,{"13500929",28},{"13500970",27},{"13500972",27}
,{"13500973",27},{"13500974",27},{"13500975",27}
,{"13500976",27},{"13500977",27},{"13500978",27}
,{"13500979",27},{"13514351",27},{"13514353",27}
,{"13514355",27},{"13514356",27},{"13514359",27}
,{"13630683",28},{"13630695",28},{"13630696",28}
,{"13630697",28},{"13630698",28},{"13634331",28}
,{"13634335",28},{"13634339",28},{"13634351",27}
,{"13634352",27},{"13634353",27},{"13634354",27}
,{"13634355",27},{"13634357",27},{"13634359",27}
,{"13634432",28},{"13634439",28},{"13634452",27}
,{"13634459",27},{"13704430",28},{"13704439",28}
,{"13704450",27},{"13704451",27},{"13704452",27}
,{"13704453",27},{"13704454",27},{"13704455",27}
,{"13704456",27},{"13704458",27},{"13704459",27}
,{"13704466",28},{"13804430",28},{"13804439",28}
,{"13804451",27},{"13804452",27},{"13804453",27}
,{"13804455",27},{"13804456",27},{"13843337",28}
,{"13904452",27},{"13904453",27},{"13904455",27}
,{"13904456",27},{"13904470",28},{"13904471",28}
,{"13904472",28},{"13904473",28},{"13904479",28}
,{"13943344",27},{"13943345",27},{"13943346",27}
,{"13943347",27},{"13943348",27},{"13943386",28}
,{"13943387",28},{"13943388",28},{"13943389",28}
,{"13943533",27},{"13943534",27},{"13943535",27}
,{"13943536",27}
};
#endif

#define MET_MAXCITYNAMEBUF                26

#define MET_AQ_FS_HANDLE  SFS_HANDLE

/*******************************file ***********/
#define MET_AQ_FS_READ_WRITE            0x00000000L
#define MET_AQ_FS_READ_ONLY             0x00000100L
#define MET_AQ_FS_OPEN_SHARED           0x00000200L
#define MET_AQ_FS_OPEN_NO_DIR           0x00000400L
#define MET_AQ_FS_OPEN_DIR              0x00000800L
#define MET_AQ_FS_CREATE                0x00010000L
#define MET_AQ_FS_CREATE_ALWAYS         0x00020000L 
#define MET_AQ_FS_COMMITTED		         0x01000000L
#define MET_AQ_FS_CACHE_DATA	         0x02000000L
#define MET_AQ_FS_LAZY_DATA		         0x04000000L
#define MET_AQ_FS_NONBLOCK_MODE			0x10000000L
#define MET_AQ_FS_PROTECTION_MODE		   0x20000000L
#define MET_AQ_FS_RW_AND_CREATE	0x00010000L
/*******************************file ***********/




// 标志编译器是否为高端优先
#ifdef MEX_BIGEND
#define VERSION_HIGH_FIRST
#endif


#ifdef VERSION_HIGH_FIRST
// 八字节类型的字节转换
#define HIGH_CHANGE_EIGHT(data) 	{ \
 		uint8 * p=&(data); \
		uint8 t; \
		int32 i; \
		for(i=0;i<4;i++) \
		{ \
			t=p[i]; \
			p[i]=p[7-i]; \
			p[7-i]=t; \
		} \
	} 
// 四字节类型的字节转换
#define HIGH_CHANGE_FOUR(data) ( (((data)&0xff)<<24)	\
	| ((uint8)(((data)&0xff00)>>8)<<16)\
	| ((uint8)(((data)&0xff0000)>>16)<<8)	\
	| ((uint8)(((data)&0xff000000)>>24))	\
	)
// 双字节类型的字节转换
#define HIGH_CHANGE_TWO(data)	( (((data)&0xff)<<8)	\
	| ((uint8)(((data)&0xff00)>>8))	\
	)

extern void met_mex_aq_swapWchar(char * str,int32 strlen);

#define MET_AQ_CHANGE_WSTR(str,strlen) met_mex_aq_swapWchar((char *)str,strlen)
#else
#define HIGH_CHANGE_EIGHT(data) (data)
#define HIGH_CHANGE_FOUR(data) (data)
#define HIGH_CHANGE_TWO(data)	(data)
#define MET_AQ_CHANGE_WSTR(str,strlen) 
#endif


wchar_t* mex_wcscpy( wchar_t *strDest, const wchar_t *strSource );
char* mex_twcs2str( const wchar_t *wcsSrc );


extern int32 met_aq_showLocationStatus(void);
extern int8* met_mex_aq_UnicodeStrCpy(int8* target,const int8* source);
extern uint16  met_mex_aq_UnicodeStrlen(const int8* source);
extern int8 *met_mex_aq_UnicodeStrCat(int8 *target ,  const int8 *source);
extern void MET_MEX_AQ_GB2UCS2(char * src,char *dest);
extern BOOLEAN met_mex_aq_get_dat_path(int8 * drv_buf,int8 * filename);
/*******************文件操作部分***************************/
extern int32 met_aq_fs_read(MET_AQ_FS_HANDLE filehandler,void * dataptr,uint length,uint * read);
extern int32 met_aq_fs_open(const wchar * filename,uint flag);
extern int32 met_aq_fs_close(MET_AQ_FS_HANDLE file);
extern int32 met_aq_fs_seek(MET_AQ_FS_HANDLE file,int32 offset,int32 whence);
extern int32 met_aq_fs_delete(const wchar* file);
extern int32 met_aq_fs_rename(const wchar* file ,const wchar* newfile);
extern int32 MET_AQ_FS_GetFileSize(MET_AQ_FS_HANDLE FileHandle, uint *Size);
extern uint32 MET_AQ_FS_GetRomoveableFree(void);
//////////////////////mem///////////////////////////
extern int32 met_aq_getCity(char * pNumber);
extern size_t mex_strlen( const char *str );

extern void * met_mex_aq_malloc(uint32 size);
extern void met_mex_aq_free(void **ptr);
///////////////////////mem.//////////////////////////

//=====================================================================

extern int32 met_aq_GetIPPhone(const uint8 *tele_in, uint8 *tele_out, int32 tele_out_len); 
extern void met_mex_aq_GetAttachedAreaNameAndReset(char* tempBuf);
extern const wchar_t* mex_GetAppRootDir(void);
extern int32 MexConfig_GetPhoneConfig(E_Mex_Phone_Config cfg);
extern int8 met_mex_aq_NumInq_App_Flag;
extern uint8 met_mex_aq_strCityNameUnicode[];

#if defined(MEX_PLATFORM_SPREAD) || defined(MEX_PLATFORM_SIMULATOR)
extern int32 met_aq_SmsWall(
        MN_NUMBER_PLAN_E    number_plan,
        MN_NUMBER_TYPE_E    number_type,
        uint16              num_len,
        uint8              *party_num);

extern int32 met_aq_PhoneWall(
        MN_NUMBER_PLAN_E    number_plan,
        MN_NUMBER_TYPE_E    number_type,
        uint16              num_len,
        uint8              *party_num);

#endif

extern int32  met_mex_aq_get_drv_status(void);

extern void  Met_Mex_Aq_ResetCityName(void);
extern void Met_Mex_Aq_CityNotFound(void);

extern BOOLEAN MET_Aq_Check_DataFile_exist(void);

extern void met_mex_aq_trace(const char *atext,...);

#endif

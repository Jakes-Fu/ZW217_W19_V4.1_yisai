
/*************************************************************************
 ** File Name:      met_mex_aq.c                                    *
** Author:         zhenmu(zhenjie.shen)                                           *
** Date:           2011/03/15                                        *
** Copyright:      2011 METEK. All Rights Reserved.  *
** Description:    This file is the interface code of area query               *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
** 2011/03/15      zhenmu(zhenjie.shen)       Create.                              *
*************************************************************************/
#include "mmi_app_met_mex_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef MET_MEX_AQ_SUPPORT
#include <stdarg.h>
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
#include "mmiphone_export.h"
#else
#include "mmiphone.h"
#endif
#include "mmi_filemgr.h"
#if(MEX_SPRD_CODE_VERSION == 0x10A6530)
#include "mmifmm_export.h"
#else
#include "mmifmm.h"
#endif
#include "mmipub.h"

#include "mexlib.h"
#include "met_mex_aq.h"


/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                                                           *
**--------------------------------------------------------------------------*/
extern void met_mex_aq_reset_confBuf(void);

/**---------------------------------------------------------------------------*
**						 LOCAL DATA DECLARATION
**---------------------------------------------------------------------------*/

int8 met_mex_aq_NumInq_App_Flag;	//查询状态标志位


/**---------------------------------------------------------------------------*
**                       function bodies  -- 
**---------------------------------------------------------------------------*/
	

#ifdef VERSION_HIGH_FIRST
void met_mex_aq_swapWchar(char * str,int32 strlen)
{
	int32 i;
	char temp;
	for(i=0;i<strlen*2;i+=2)
	{
		temp=str[i];
		str[i]=str[i+1];
		str[i+1]=temp;
	}
}
#endif

int8* met_mex_aq_UnicodeStrCpy(int8* target,const int8* source)
{
    return (int8*)mex_wcscpy((wchar_t*)target,(const wchar_t*)source);
}

uint16  met_mex_aq_UnicodeStrlen(const int8* source)
{
#if 0
	UnicodeStrLen(source);
#else
	uint16 nlength=0;
	uint16 ncount=0;

	if(source)
	{
		while(source[ncount]!='\0' || source[ncount+1]!='\0')
		{
			++nlength;
			ncount+=2;
		}
	}
	return nlength;
#endif
}


int8 *met_mex_aq_UnicodeStrCat(int8 *target ,  const int8 *source)
{
	int8 *temp ;
	temp= target;
	
	temp = temp + met_mex_aq_UnicodeStrlen(source) * 2;
	//temp+= met_UnicodeStrlen(source) ; //// * 2;
	met_mex_aq_UnicodeStrCpy(temp, source);
	return target;
}


void MET_MEX_AQ_GB2UCS2(char * src,char *dest)
{
	GUI_GBToWstr((wchar*)dest,(const uint8*)src,strlen(src));
}



//==============================================================


//获取工作路径

BOOLEAN met_mex_aq_get_dat_path(int8 * drv_buf,int8 * filename)
{
	char buf[SFS_MAX_PATH];
	sprintf(buf, "%s%s%s",mex_twcs2str(mex_GetAppRootDir()) ,"\\AreaInquire\\",filename);
	
#ifdef MET_MEX_AQ_DEBUG
	met_mex_aq_trace("get vm path:%s.",buf);
#endif

	MET_MEX_AQ_GB2UCS2(buf, drv_buf);

	return TRUE;
}



/*******************文件操作部分***************************/
int32 met_aq_fs_read(MET_AQ_FS_HANDLE filehandler,void * dataptr,uint length,uint * read)
{
	SFS_ERROR_E result = SFS_ReadFile(filehandler,dataptr,length,(uint32*)read,PNULL);
	if(result != SFS_NO_ERROR)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int32 met_aq_fs_open(const wchar * filename,uint flag)	// == 0则表示打开失败
{
	uint32 access_mode=0;
	switch(flag)
	{
		case MET_AQ_FS_READ_ONLY:
			access_mode = SFS_MODE_READ | SFS_MODE_OPEN_EXISTING;
			break;
		case MET_AQ_FS_READ_WRITE:
			access_mode = SFS_MODE_READ |SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS;
			break;
		case MET_AQ_FS_CREATE:
			access_mode = SFS_MODE_READ |SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS;
			break;
		case MET_AQ_FS_CREATE_ALWAYS:
			access_mode = SFS_MODE_READ | SFS_MODE_WRITE| SFS_MODE_CREATE_ALWAYS;
			break;
		default:
			access_mode = SFS_MODE_READ |SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS;
			break;
	}
	return SFS_CreateFile(filename, access_mode, PNULL, PNULL);
}

int32 met_aq_fs_close(MET_AQ_FS_HANDLE file)
{
	return SFS_CloseFile(file);
}
int32 met_aq_fs_write(MET_AQ_FS_HANDLE file,void * data,uint length,uint * wtitten)
{
	SFS_ERROR_E result = SFS_WriteFile(file, data, length, (uint32*)wtitten, PNULL);
	if(result != SFS_NO_ERROR)
		return -1;
	else
		return 0;
	
}
//#define max_seek  1024
int32 met_aq_fs_seek(MET_AQ_FS_HANDLE file,int32 offset,int32 whence)
{
	return SFS_SetFilePointer(file, offset, (uint32) whence);   /*lint !e747*/
}

int32 met_aq_fs_delete(const wchar* file)
{
	return SFS_DeleteFile(file, PNULL);
}

int32 met_aq_fs_rename(const wchar* file ,const wchar* newfile)
{
	return SFS_RenameFile(file,newfile,PNULL);
}

int32 MET_AQ_FS_GetFileSize(MET_AQ_FS_HANDLE FileHandle, uint *Size)
{
	return SFS_GetFileSize(FileHandle,(uint32*)Size); 
}



//////////////////////mem///////////////////////////

void * met_mex_aq_malloc(uint32 size)
{
#ifdef WIN32
	return malloc(size);
#else
	void *ret=SCI_ALLOC(size);
	if(ret == PNULL)
	{
	#ifdef MET_MEX_AQ_DEBUG
		//SCI_TraceLow:"Met_aq trace met_aq_malloc failed "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MET_MEX_AQ_217_112_2_18_2_35_52_0,(uint8*)"");
	#endif
	}
	else
	{
		memset(ret, 0, size);
	}
	return ret;
#endif
}

void met_mex_aq_free(void **ptr)
{
	if(*ptr)
	{
#ifdef WIN32
		free(*ptr);
#else
		SCI_FREE(*ptr);
		*ptr=PNULL;
	#ifdef MET_MEX_AQ_DEBUG
		//SCI_TraceLow:"Met_aq trace  succeed "
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MET_MEX_AQ_238_112_2_18_2_35_52_1,(uint8*)"");
	#endif
#endif
	}
}
///////////////////////mem.//////////////////////////


//=====================================================================



int32  met_mex_aq_get_drv_status()
{
#if MEX_PATH_ADAPTIVE
	return TRUE;
#else
	return MexConfig_GetPhoneConfig(E_MPC_Tcard_State);
#endif
}

void met_mex_aq_trace(const char *atext,...)
{
	char temptext[1024];
	va_list args;
	
	va_start(args,atext);
	vsprintf(temptext,atext,args);
	va_end(args);

	mex_log(1,temptext);	
}


extern uint8 met_mex_aq_strCityNameUnicode[MET_MAXCITYNAMEBUF];
extern char met_mex_aq_strCityQuhao[6];
void  Met_Mex_Aq_ResetCityName(void)
{
	memset(met_mex_aq_strCityNameUnicode, 0, sizeof(met_mex_aq_strCityNameUnicode));
    memset(met_mex_aq_strCityQuhao,0,sizeof(met_mex_aq_strCityQuhao));
}


void Met_Mex_Aq_CityNotFound(void)
{
#ifdef MEX_PLATFORM_SPREAD
	met_mex_aq_UnicodeStrCpy((int8 *)met_mex_aq_strCityNameUnicode,(const int8 *)"\x67\x2a\x77\xe5\x57\xce\x5e\x02\x00\x00");
#else
	met_mex_aq_UnicodeStrCpy((int8 *)met_mex_aq_strCityNameUnicode,(const int8 *)"\x2a\x67\xe5\x77\xce\x57\x02\x5e\x00\x00");
#endif
    memset(met_mex_aq_strCityQuhao,0,sizeof(met_mex_aq_strCityQuhao));
}

extern wchar g_datafilepath[];

BOOLEAN MET_Aq_Check_DataFile_exist()
{
	MET_AQ_FS_HANDLE fp;
	BOOLEAN file_exist = FALSE;
	
	met_mex_aq_get_dat_path((int8 *)g_datafilepath, "Met_areaInq.dat");
	
	if((fp=met_aq_fs_open(g_datafilepath, MET_AQ_FS_READ_ONLY)) > 0)
	{
		uint Size=0;
		MET_AQ_FS_GetFileSize(fp,&Size);
		if(Size>50)
		{
			file_exist=TRUE;
#ifdef MET_MEX_AQ_DEBUG
			met_mex_aq_trace("MET_Aq_Check_DataFile_exist  file exist==%d",file_exist);
#endif
		}
		met_aq_fs_close(fp);
	}
	else
	{
#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("file exist");
#endif
	}
	return file_exist;
}


int32 met_mex_aq_GetAttachedAreaName(char *tempbuf)
{
	int32 ret;
	
	Met_Mex_Aq_ResetCityName();
	if(met_mex_aq_get_drv_status()==0)
	{
#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("met_get_drv_status return 0 fail");
#endif
		return -1;
	}
	if(!(MET_Aq_Check_DataFile_exist()))
	{
#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("met_aq_datafile check failed");
#endif
		return -2;
	}


	ret=met_aq_getCity(tempbuf);
	if(ret==0)
	{
		extern int32 met_isshortlocalnumber;
		//met_UnicodeStrCpy((int8*)MET_strCityNameUnicode, "\x2a\x67\xe5\x77\xce\x57\x02\x5e\x00\x00");
		Met_Mex_Aq_CityNotFound();
		if(met_isshortlocalnumber !=1)
		{
			return -2;
		}
	}
	if(ret==-1)
	{				//在来去电界面数据包已过期
		Met_Mex_Aq_ResetCityName();
		return -4;
	}
	if(ret==-2)
	{
	//在查询界面数据包已过期
		//MET_AreaInq_Update_monthly_alert();
		Met_Mex_Aq_ResetCityName();
		return -5;
	}
	if(ret==10)
	{
		Met_Mex_Aq_CityNotFound();
	}
return 1;
}

void met_mex_aq_GetAttachedAreaNameAndReset(char* tempBuf)
{    
	met_mex_aq_NumInq_App_Flag = met_mex_aq_GetAttachedAreaName(tempBuf);
	met_mex_aq_reset_confBuf();
}

void met_mex_aq_save_log(char *pNumber, char *log_file_name)
{
    typedef struct
    {
    	uint16 year;		//年
    	uint8 month;		//月
    	uint8 day;			//日
        uint8 dayIndex;	// 星期 0=Sunday
    	uint8 hour;			//小时
    	uint8 minute;		//分
    	uint8 second;		//秒
    }mexDateTime;
    void mex_GetDateTime(mexDateTime *pDt);
    mexDateTime cur_dateTime = {0}; 
	MET_AQ_FS_HANDLE file_handler = 0;
    uint result = 0;
    wchar log_file_path[SFS_MAX_PATH] = {0};
    char log_line[64] = {0};
    mex_GetDateTime(&cur_dateTime);
    met_mex_aq_get_dat_path((int8*)log_file_path,log_file_name);
    if((file_handler = met_aq_fs_open((const wchar*)log_file_path,MET_AQ_FS_READ_WRITE)) <= 0)  /*lint !e775*/
    {
        if((file_handler = met_aq_fs_open(log_file_path,MET_AQ_FS_CREATE_ALWAYS)) <= 0)  /*lint !e775*/
        {
            mex_log(1,"%s%s","[met_mex_aq_save_log]","create file error");
            return;
        }
    }
    sprintf(log_line,"%d-%d-%d %d:%d %s\r\n",
        cur_dateTime.year,
        cur_dateTime.month,
        cur_dateTime.day,
        cur_dateTime.hour,
        cur_dateTime.minute,
        pNumber);
    met_aq_fs_seek(file_handler,0,SFS_SEEK_END);
    met_aq_fs_write(file_handler,log_line,mex_strlen(log_line),&result);
    met_aq_fs_close(file_handler);
}

#endif


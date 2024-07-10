
/*************************************************************************
 ** File Name:      met_mex_aq_lib.h                                    *
** Author:         zhenmu(zhenjie.shen)                                           *
** Date:           2011/03/15                                        *
** Copyright:      2011 METEK. All Rights Reserved.  *
** Description:    This file is the code of area query     needs to be hidden          *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
** 2011/03/15      zhenmu(zhenjie.shen)       Create.                              *
*************************************************************************/
#include "mmi_app_met_mex_trc.h"
#include "std_header.h"
#ifdef MET_MEX_AQ_SUPPORT
#include "window_parse.h"
#include "met_mex_aq.h"
	

/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

typedef enum 
{
	BLOCK_ID_PHONENUMLIST=0,//�ֻ������Ӧ����������	//�汾��ʷ��60�ֽڿ��ַ�
	BLOCK_ID_AUTODELFILEFLAG,//ÿ���Զ�ɾ����ʶλ
	BLOCK_ID_MAXCITYNUM,//����������
	BLOCK_ID_MAXQUHAONUM,//�����������
	BLOCK_ID_PHONENUMPREFIX,//�ֻ�����ǰ׺����
	BLOCK_ID_PHONENUMPRELIST,//�ֻ�ǰ׺���� ��"131518"����13,15,18��ͷ�ĺ���ǰ׺�б�
	BLOCK_ID_CITYNAMEDATA,//����������
	BLOCK_ID_QUHAODATA,//������������
	BLOCK_ID_CITYNAMEDATA_GBK,//�������������Ĺ�����GBK
	BLOCK_ID_MAXNUM
}MMIAPI_DEV_GET_CTRL_ACTION;


typedef struct 
{
	int32 block_id;
	int32 block_size;
	int32 blockfilepos;
}BlockMappstr;

#define PREFIXFILESIZE 200000
#define  BLOCKMAPPSTART "BlockMapping start:"
#define  BLOCKMAPPEND    "BlockMapping end"


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                                                           *
**--------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
**						 LOCAL DATA DECLARATION
**---------------------------------------------------------------------------*/
	
static MET_AQ_FS_HANDLE g_datafilehandle;

unsigned char met_mex_aq_strCityNameUnicode[MET_MAXCITYNAMEBUF];
char met_mex_aq_strCityQuhao[6] = {0}; // ����Ϊ5,��Ϊ6��֤����
static unsigned char  config_buffer[1124];
wchar g_datafilepath[SFS_MAX_PATH];
static int32 g_maxCityNameNum=0;
static int32 g_maxquhaonum=0;
static int32 g_maxphoneprefix=0;
static char prelist[400];

static int32 pt_filepos_PhoneNumData=0;
static int32 pt_filepos_CityNameUnicData=0;
static int32 pt_filepos_QuHaoData=0;


/**---------------------------------------------------------------------------*
**                       function bodies  -- 
**---------------------------------------------------------------------------*/
	

void met_mex_aq_reset_confBuf(void)
{
	g_maxCityNameNum=0;
	memset(config_buffer,0,sizeof(config_buffer));	

}

static void met_readblockbyid(void *data,int32 blockid,BlockMappstr *outdata)
{
	char *tempdata=(char *)data;
	//aq_ntoh((wchar *)tempdata);
	memcpy(outdata, tempdata+sizeof(BlockMappstr)*blockid, sizeof(BlockMappstr));
	outdata->block_id = HIGH_CHANGE_FOUR(outdata->block_id);
	outdata->block_size = HIGH_CHANGE_FOUR(outdata->block_size);
	outdata->blockfilepos = HIGH_CHANGE_FOUR(outdata->blockfilepos);
	//SCI_MEMCPY(outdata, tempdata+sizeof(BlockMappstr)*blockid, sizeof(BlockMappstr));
}

static int32 OpenDataFile(wchar *ori_path)
{
	int32 i,j,size;
	if((g_datafilehandle=met_aq_fs_open(ori_path, MET_AQ_FS_READ_ONLY)) == 0)
	{
#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("OpenDataFile,open data file fail");
#endif
		g_datafilehandle=0;
		return -3;
	}
	return 1;
}

static int32 CloseDataFile(void)
{
	met_aq_fs_close(g_datafilehandle);
	g_datafilehandle=0;
	return 0;
}
static int32 readMaxCityNumFromConf(MET_AQ_FS_HANDLE filehandle)
{
	BlockMappstr temp_blockmappstr;
	int32 size,temp;
	met_readblockbyid(config_buffer, BLOCK_ID_MAXCITYNUM, &temp_blockmappstr);

	{
			char temptest[100]={0x00};
			char temptest2[100]={0x00};
			sprintf(temptest,"%d",temp_blockmappstr.blockfilepos);
			sprintf(temptest2,"%d",temp_blockmappstr.block_size);
		}
	
	met_aq_fs_seek(filehandle, temp_blockmappstr.blockfilepos, SFS_SEEK_BEGIN);
	met_aq_fs_read(filehandle, &temp, sizeof(temp), (uint *) &size);
	temp  = HIGH_CHANGE_FOUR(temp);
	return temp;
}

static int32 readMAXQUHAONUMFromConf(MET_AQ_FS_HANDLE filehandle)
{
	BlockMappstr temp_blockmappstr;
	int32 size,temp;
	met_readblockbyid(config_buffer, BLOCK_ID_MAXQUHAONUM, &temp_blockmappstr);

{
			char temptest[100]={0x00};
			char temptest2[100]={0x00};
			sprintf(temptest,"%d",temp_blockmappstr.blockfilepos);
			sprintf(temptest2,"%d",temp_blockmappstr.block_size);
		}
	
	met_aq_fs_seek(filehandle, temp_blockmappstr.blockfilepos, SFS_SEEK_BEGIN);
	met_aq_fs_read(filehandle, &temp, sizeof(temp), (uint *) &size);
	temp  = HIGH_CHANGE_FOUR(temp);
	return temp;
}

static int32 readPHONENUMPREFIXFromconf(MET_AQ_FS_HANDLE filehandle)
{
	BlockMappstr temp_blockmappstr;
	int32 size,temp;
	met_readblockbyid(config_buffer, BLOCK_ID_PHONENUMPREFIX, &temp_blockmappstr);
	met_aq_fs_seek(filehandle, temp_blockmappstr.blockfilepos, SFS_SEEK_BEGIN);
	met_aq_fs_read(filehandle, &temp, sizeof(temp), (uint *) &size);
	temp  = HIGH_CHANGE_FOUR(temp);
	return temp;
}

//ȡ���ֻ���ǰ׺���ִ��б�
static int32 readprefixStrListFromConf(MET_AQ_FS_HANDLE fp,char * perfixStr)
{
	int32 size;
	BlockMappstr temp_blockmappstr;
	met_readblockbyid(config_buffer,BLOCK_ID_PHONENUMPRELIST,&temp_blockmappstr);
	met_aq_fs_seek(fp, temp_blockmappstr.blockfilepos,SFS_SEEK_BEGIN);
		{
			char temptest[100]={0x00};
			char temptest2[100]={0x00};
			sprintf(temptest,"%d",temp_blockmappstr.blockfilepos);
			sprintf(temptest2,"%d",temp_blockmappstr.block_size);
		}
	met_aq_fs_read(fp,perfixStr,temp_blockmappstr.block_size,(uint *)&size);
	return 1;
}
//

static int32 readPhoneNumDataFromConf_pos(MET_AQ_FS_HANDLE fp)
{
	int32 size;
	BlockMappstr temp_blockmappstr;
	met_readblockbyid(config_buffer, BLOCK_ID_PHONENUMLIST, &temp_blockmappstr);
	return temp_blockmappstr.blockfilepos;
}
//
static int32 readCityNameUnicDataFromConf_pos(MET_AQ_FS_HANDLE fp)
{
	int32 size;
	BlockMappstr temp_blockmappstr;
	met_readblockbyid(config_buffer, BLOCK_ID_CITYNAMEDATA, &temp_blockmappstr);
	return temp_blockmappstr.blockfilepos;
}
//

static int32 readQuHaoDataFromConf_pos(MET_AQ_FS_HANDLE fp)
{
	int32 size;
	BlockMappstr temp_blockmappstr;
	met_readblockbyid(config_buffer, BLOCK_ID_QUHAODATA, &temp_blockmappstr);
	return temp_blockmappstr.blockfilepos;
}
#if 0
uint8 MET_GetCityIndexFormFile(int32 index,char filenameindex,void * rtdata)
{
	MET_AQ_FS_HANDLE fs_handle;
	U16 drv=(U16)*MET_AQ_APP_DRV;
	char Met_Unic_datafilepath[SFS_MAX_PATH];
	char met_datafiledrv[4];
	sprintf(met_datafiledrv,"%c:\\",&drv);

	met_bra_AsciiToUnicodeString((S8 *) Met_Unic_datafilepath, (s8 *) met_datafiledrv);
	met_UnicodeStrCat((int8 *) Met_Unic_datafilepath, (const int8 *) MET_G_DATAFILEPATH);
}
#endif


uint8 MET_GetCityIndexByPhoneNum(MET_AQ_FS_HANDLE file_handle_rf,int32 phonenum_index,int32 prefixindex,void *rtdata)
{
	int32 seek_length,read_size,size;
	seek_length=pt_filepos_PhoneNumData+sizeof(short)*phonenum_index+prefixindex*PREFIXFILESIZE;
	read_size=sizeof(short);
	#ifdef MET_MEX_AQ_DEBUG
	met_mex_aq_trace("MET_GetCityIndexByPhoneNum   seek_length ===%d,read_size===%d",seek_length,read_size);
	#endif

	met_aq_fs_seek(file_handle_rf, seek_length, SFS_SEEK_BEGIN);
	met_aq_fs_read(file_handle_rf, rtdata, read_size, (uint *) &size);
	#ifdef MET_MEX_AQ_DEBUG
	met_mex_aq_trace("rtdata===%d",rtdata);
	#endif
}
int32  MET_returnCityName(MET_AQ_FS_HANDLE fs_handle_rt,unsigned char* outCityName,int32 cityIndex)
{
	int32 seek_length,read_size,size;
	seek_length=pt_filepos_CityNameUnicData+sizeof(unsigned char)*20*cityIndex;
	read_size=sizeof(unsigned char)*20;
	#ifdef MET_MEX_AQ_DEBUG
	met_mex_aq_trace(" MET_returnCityName  seek_length ===%d,read_size===%d",seek_length,read_size);
	#endif

	met_aq_fs_seek(fs_handle_rt,seek_length,SFS_SEEK_BEGIN);
	met_aq_fs_read(fs_handle_rt, outCityName, read_size, (uint *)&size);
	//UNICODE �ߵ�λ��ת��
	MET_AQ_CHANGE_WSTR(outCityName,read_size/2);
	return 1;
}

int32 MET_returnCityQuhao(MET_AQ_FS_HANDLE fs_handle_rt,unsigned char* outQuHao, int32 cityIndex)
{
	int32 seek_length,read_size,size;
	seek_length=pt_filepos_QuHaoData + sizeof(char)*5*cityIndex;
	read_size=sizeof(char)*5;
#ifdef MET_MEX_AQ_DEBUG
	met_mex_aq_trace(" MET_returnCityQuhao seek_length = %d,read_size = %d",seek_length,read_size);
#endif

	met_aq_fs_seek(fs_handle_rt,seek_length,SFS_SEEK_BEGIN);
	met_aq_fs_read(fs_handle_rt, outQuHao, read_size, (uint *)&size);
	return 1;
}

int32 met_GetCityIndexByQuHao(MET_AQ_FS_HANDLE fs_handle_rt,unsigned char* QuHaoData,int32 pos,int32 readSize,unsigned char* outCityName)
{
	int32 seek_length,read_size,size,find_flag;
	char *pt_QuHaoList=PNULL;
	char *pt_temp=PNULL;

	int32 i;
	seek_length=pos;
	read_size=sizeof(char)*5*readSize;

	pt_QuHaoList=(char*)met_mex_aq_malloc(read_size);
	if(pt_QuHaoList==PNULL)
	{
	#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("met_GetCityIndexByQuHao  pt_QuHaoList malloc fail");
	#endif
		return -1;
	}
	else
	{
		pt_temp=pt_QuHaoList;
		
	#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("met_GetCityIndexByQuHao  seek_length ==%d",seek_length);
	#endif
		met_aq_fs_seek(fs_handle_rt,seek_length, SFS_SEEK_BEGIN);
		met_aq_fs_read(fs_handle_rt,pt_QuHaoList, read_size, (uint *)&size);

		for(i=0;i<read_size;i++)
		{
			if(pt_temp!=PNULL)
			{
				if(memcmp(pt_temp, QuHaoData,sizeof(char)*5)==0)
				{
					find_flag=1;
					break;
				}
				else
				{
					pt_temp+=5;
				}
			}
			if(i==(readSize-1))
			{
				if(QuHaoData[3]=='\0')
				{
					break;
				}
				else
				{
					QuHaoData[3]='\0';
					pt_temp=pt_QuHaoList;
					i=-1;
				}
			}
		}
		if(find_flag==1)
		{
			MET_returnCityName(fs_handle_rt,  outCityName,i);
			met_mex_aq_free(&pt_QuHaoList);
			return 1;
		}
		else
		{
			met_mex_aq_free(&pt_QuHaoList);
			return -2;
		}
	}
}

/***********************************************/
int32 met_isshortlocalnumber=0;									//���غ���

static int32 met_need_skip_ip_header(char * pNumber,int32 * skipLen)
{
	char ipstr[200];
	char *pt1;
	char *pt2;
	int32 find_86 = 0;
	int32 find_iphead= 0;
	pt1 = pt2 = PNULL;

	if(SCI_STRLEN(pNumber)<3)
	{
		return -1;
	}

	pt1=pNumber;
	
	if((SCI_STRLEN(pNumber)>= 7) && (SCI_STRLEN(pNumber)<= 8))		//���غ���
	{
		if(((*pt1)!='+') && ((*pt1)!='1') && ((*pt1)!='0'))
		met_isshortlocalnumber=1;
		return -2;
	}
	// skip +86
	if((*pt1)=='+')
	{
		pt2=pt1+1;
		if((*pt2)=='8')
			return 0;
		pt2+=1;
		if((*pt2)=='6')
			return 0;

		find_86=1;
	}
	//skip 12593 17951 17911
	if(SCI_STRLEN(pNumber)>= 8)
	{
		SCI_MEMSET(ipstr,0,sizeof(ipstr));
		if(find_86==0)
		{
			SCI_MEMCPY(ipstr,pNumber,SCI_STRLEN(pNumber));
		}
		else
		{
			SCI_MEMCPY(ipstr,pNumber+3,(SCI_STRLEN(pNumber)-3));
		}
		#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("ipstr === %s",ipstr);
		#endif
		if((memcmp(ipstr,"12593", 5)==0)
		||(memcmp(ipstr,"17951", 5)==0)
		||(memcmp(ipstr,"17911", 5)==0))
		{
			find_iphead=1;
		}
		if(memcmp(ipstr,"86", 2)==0)
		{
			find_iphead=2;
		}
	}

	if((find_iphead==1) && (find_86 ==1))
	{
		(*skipLen)=8;
		return 1;
	}
	else if((find_86==0) && (find_iphead==1))
	{
		(*skipLen)=5;
		return 1;
	}
	else if((find_86==1) && (find_iphead==0))
	{
		(*skipLen)=3;
		return 1;
	}
	else if(find_86==2)
	{
		(*skipLen)=2;
		return 1;
	}
	return 0;
}


static  int32 readblockconfigtobuffer(wchar * ori_path,void * outdata,int32 * autodelfileflag)
{
	MET_AQ_FS_HANDLE fp;
	int32 size;
	if((fp=met_aq_fs_open(ori_path, MET_AQ_FS_READ_ONLY)) == 0)
	{
	#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("readblockconfigtobuffer,file open error failed");
	#endif
		return -1;
	}

	met_aq_fs_seek(fp,-(1024+20+20), SFS_SEEK_END);

	memset(outdata, 0, sizeof(outdata));

	met_aq_fs_read(fp,outdata,20,(uint *)&size);
	if(memcmp(outdata, BLOCKMAPPSTART,SCI_STRLEN(outdata))==0)
	{
		SCI_MEMSET(outdata, 0, sizeof(outdata));
		met_aq_fs_seek(fp,-20,SFS_SEEK_END);
		met_aq_fs_read(fp,outdata,20,(uint *)&size);
		if(memcmp(outdata, BLOCKMAPPEND,SCI_STRLEN(outdata))==0)
		{
			BlockMappstr temp_blockmappstr={0};
			SCI_MEMSET(outdata, 0, sizeof(outdata));
			met_aq_fs_seek(fp,-(1024+20),SFS_SEEK_END);
			met_aq_fs_read(fp,outdata,1024,(uint *)&size);
			//ȡ��ÿ���Զ�ɾ���ı�־λ
			{		
				met_readblockbyid(outdata, BLOCK_ID_AUTODELFILEFLAG, &temp_blockmappstr);
				met_aq_fs_seek(fp, temp_blockmappstr.blockfilepos,SFS_SEEK_BEGIN);
				met_aq_fs_read(fp,autodelfileflag, sizeof(autodelfileflag),(uint *)&size);
				

			}
			met_aq_fs_close(fp);
			return 1;
		}
		else
		{
			met_aq_fs_close(fp);
			return -1;
		}
	}
	else
	{
		met_aq_fs_close(fp);
		return -2;
	}
}

/*******************************************************************************
��ѯ����,�����ѯ���ַ��������ú���ȫ������strCityName������鵽�ĳ�����
������pNumber: Ϊ����ĺ����ַ�����ָ��
����ֵ��
1���������أ���������strCityName������д��������δ�鵽��дδ֪����
2��˵����ǰ����ŶΣ�6λ���������أ��Ŷ�������8λ������ǰ��λ�����ܲ鵽������
0��˵�������λ������
3��˵��������ַ����к��Ƿ��ַ�
4:    ˵��������ַ���Ϊ��
ע�⣺����Ĳ�ѯ�ַ���Ҫ��֤��'\0'��β
********************************************************************************/
 int32 met_aq_getCity(char * pNumber)
{
	int32 skipLen=0;
	int32 n=0;
	char * pDeNumber;
	char *ps1;
	char *ps2;
	char temchars[3];
	int32 i=0;
	char strNum[10];//����ֻ��ŵ�1*�����λ����λ���룬����ѯ��
	int32 mobileNum;//strNum  int32 type
	char *pt_Prefix=PNULL;
	short cityindex;
	int32 prefixindex=-1;
	char numNotFound=0;
	int32 cityindexfromquhao=0;
	
	int32 g_autodelfileflag=0;

	if(pNumber==PNULL || *pNumber =='\0')
		return 0;

#ifdef MET_MEX_AQ_DEBUG
	met_mex_aq_trace("met_aq_getCityName begin ");
#endif
	
	
	{
		//int32 m=SCI_STRLEN((const char *)config_buffer);
		///return m value=0; 
	}//  add for test config_buffer;
	
	if(met_need_skip_ip_header(pNumber,&skipLen)<0)
	{
	#ifdef MET_MEX_AQ_DEBUG
		met_mex_aq_trace("met_aq_getcityname met_need_skip_ip_header < 0 ");
	#endif

		return 0;
	}


	pDeNumber=pNumber+skipLen;
	#ifdef MET_MEX_AQ_DEBUG
	met_mex_aq_trace("pDeNumber===%s ",pDeNumber);
	#endif

	ps1=pDeNumber;
	ps2=ps1+1;
	n=SCI_STRLEN((const char *) pDeNumber);
	#ifdef MET_MEX_AQ_DEBUG
	met_mex_aq_trace("StrLen((const char *) pDeNumber)===%d ",n);
	#endif

	if(readblockconfigtobuffer(g_datafilepath, config_buffer, &g_autodelfileflag)==1)
 	{
	#ifdef MET_MEX_AQ_DEBUG
 		met_mex_aq_trace("getcityname  g_datafilepath==%s",g_datafilepath);
	#endif
 		if( g_autodelfileflag==1 )
 		{
 		 	int32 rs_tmp=0;
			rs_tmp= -100;//Met_mex_aq_CheckAndDeleteDataFile();
			if(rs_tmp == 3)		
			{					//calling screen
				return -1;
			}
			else if(rs_tmp==1)
			{					//main menu screen
				return -2;
			}

 		}
		if(OpenDataFile((wchar*)g_datafilepath)==1)
		{
			g_maxCityNameNum=readMaxCityNumFromConf(g_datafilehandle);
			g_maxquhaonum=readMAXQUHAONUMFromConf(g_datafilehandle);
			g_maxphoneprefix=readPHONENUMPREFIXFromconf(g_datafilehandle);
			//g_maxquhaonum  value==371
			SCI_MEMSET(prelist, 0, sizeof(prelist));
			readprefixStrListFromConf(g_datafilehandle, (char *) prelist);
			pt_filepos_PhoneNumData=readPhoneNumDataFromConf_pos(g_datafilehandle);
			pt_filepos_CityNameUnicData=readCityNameUnicDataFromConf_pos(g_datafilehandle);
			pt_filepos_QuHaoData=readQuHaoDataFromConf_pos(g_datafilehandle);
	#ifdef MET_MEX_AQ_DEBUG
			met_mex_aq_trace("g_maxCity=%d g_maxQuHao=%d g_maxPrefix=%d pt_FilePos_PhoneNum=%d",g_maxCityNameNum,g_maxquhaonum,g_maxphoneprefix,pt_filepos_PhoneNumData);
	#endif
		}
		else
		{
	#ifdef MET_MEX_AQ_DEBUG
			met_mex_aq_trace("readblockconfigto buffer met_aq_getcity failed!!!");
	#endif
			return 0;
		}
	
 	}
	
	else
	{
		return 0;
	}
	SCI_MEMSET(temchars, 0, sizeof(temchars));
	temchars[0] = (*ps1);
	temchars[1]=  (*ps2);
	

	//if call is cellphone number
	if(temchars[0]=='1')		
	{
		if(n<5)
		{
			CloseDataFile();
			return 0;
		}
		ps1++;ps1++;
		for(i;i<5;i++)
			strNum[i]=*(ps1++);
		strNum[i]='\0';
		mobileNum=atoi(&strNum);
		pt_Prefix=strstr((char *)prelist, (char *) temchars);
		if(pt_Prefix!=PNULL)
		{
			prefixindex=pt_Prefix-prelist+1;
			prefixindex=(int32)(prefixindex / 2);
		}
		else
		{
			CloseDataFile();
			return 10;
		}
		MET_GetCityIndexByPhoneNum(g_datafilehandle, mobileNum, prefixindex , &cityindex);
		cityindex = HIGH_CHANGE_TWO(cityindex);
		if(cityindex==MET_TOSIXNUMINDEX)
		{
			if(n<8)
			{
				CloseDataFile();
				return 0;
			}

			#if 1
			cityindex=MET_MAXCITY-1;
			numNotFound=1;
			#else		//@TODO:��̫���������ʲô�ģ���ʱע����ʡ�ռ�
			ps1=pDeNumber;
			for(i=0;i<8;i++)
				strNum[i]=*ps1++;
			strNum[i]='\0';
			for(i=0;i<MET_MAXSIXNUM;i++)
			{
				if(strcmp(strNum, met_sixnumindexarray[i].phonenum)==0)
				{
					cityindex=met_sixnumindexarray[i].index;
					break;
				}
			}
			if(i=MET_MAXSIXNUM)
			{
				cityindex=MET_MAXCITY-1;
				numNotFound=1;
			}
			#endif
		}
    	MET_returnCityName(g_datafilehandle,met_mex_aq_strCityNameUnicode,cityindex);
        MET_returnCityQuhao(g_datafilehandle,met_mex_aq_strCityQuhao,cityindex);
    	ps1=pDeNumber;
    	CloseDataFile();
	}
	else if(*ps1 == '0')
	{
		if(n<3)
		{
			CloseDataFile();
			return 0;
		}
		for(i=0;i<4;i++)
		{
			strNum[i] = *ps1;
			ps1++;
		}
		strNum[i]='\0';
        memcpy(met_mex_aq_strCityQuhao,strNum,5);
		cityindexfromquhao=met_GetCityIndexByQuHao(g_datafilehandle, (unsigned char *) strNum, pt_filepos_QuHaoData, g_maxquhaonum, met_mex_aq_strCityNameUnicode);
		if(cityindexfromquhao!=1)
		{
			CloseDataFile();
			numNotFound=1;
			return 10;
		}
		
	}
	else
	{
		cityindex=MET_MAXCITY-1;
		CloseDataFile();
		return 0;
	}
	if(numNotFound==1)
	{
		CloseDataFile();
		return 10;
	}
	//return ok;
	CloseDataFile();
	return 1;
	
}

typedef enum _AQ_LIST_TYPE_T    // �ڰ���������
{
    PHONE_WHITE_LIST,           // ���������
    PHONE_BLACK_LIST,           // ���������
    SMS_WHITE_LIST,             // ���Ű�����
    SMS_BLACK_LIST,             // ���ź�����

    AQ_LIST_MAX
}AQ_LIST_E;

#define PHONE_NUM_LEN           16			// �����ֽڳ���
#define CONTACT_NAME_LEN        16			// �����ֽڳ���
#define AQ_BUFFER_LEN           20			// Ԥ������ /mex�㲻ʹ��/
#define MAGIC_NUMBER            0x1111			// ����,ȷ���ļ���ȷ��

typedef struct _Number_Entry		// ÿ����¼�Ľṹ��
{
    char number[PHONE_NUM_LEN];		// �洢����
    char name[CONTACT_NAME_LEN];	// �洢����
}NumberEntry_T;

typedef struct _FILE_HEADER_BLOCK	// �ڰ������ļ�ͷ
{
    int32 magic_number;		// ����,ȷ���ļ���ȷ��
    int32 entry_count;		// ��¼����
    int32 pad[6];			// �հ�,�����Ժ�ʹ��
}FILE_HEADER_BLOCK;

typedef struct _AQ_SETTINGS	// �����ļ��ṹ
{
    int32 magic_number;		// ����,ȷ���ļ���ȷ
    int32 bShowLocation;		// �Ƿ���ʾ������
    int16 ePhoneBlockType;	// �绰��������
    int16 eSmsBlockType;	// ������������
    int32 bPhoneBlockAll;		// �Ƿ���������
    int32 bPhoneRedirect;		// �Ƿ�����ת��
    char PhoneNumber[16];	// �洢ת�ӵ��ĺ���
    int32 bEnableIPCall;    // �Ƿ�֧��IP����
    char LocalNumber[8];    // ��������
    char IpCallPreix[8];    // IP�绰ǰ׺
}AQ_SETTING_T;

typedef enum _aq_setting{	// ��������
    AQ_BLOCK_NONE,		// ������
    AQ_BLOCK_UNKNOWN,		// ����δ֪����
    AQ_BLOCK_BLACK,		// ���غ�����
    AQ_ALLOW_WHITE,		// ֻ���������

    AQ_SETTING_MAX
}AQ_SETTING_E;

// �������,�õ�ֱ�ӿɱȽϵĺ���
char *met_aq_parse_string(char *Phone_Number)
{    
    if(NULL == Phone_Number)
    {
        return NULL;
    }
    if('8' == Phone_Number[0] && '6' == Phone_Number[1])
    {
        return &Phone_Number[2];
    }
    if(('0' == Phone_Number[0] || '+' == Phone_Number[0]) && '8' == Phone_Number[1] && '6' == Phone_Number[2])
    {
        return &Phone_Number[3];
    }
    return Phone_Number;
}
int32 met_aq_compare_string(char *p1,char *p2)
{
	if(NULL == p1 || NULL == p2)
	{
		return 0;
	}
    p1 = met_aq_parse_string(p1);
    p2 = met_aq_parse_string(p2);
	while(0 != *p1 || 0 != *p2)
	{
		if(*p1 > *p2)
		{
			return 1;
		}
		else if(*p1 < *p2)
		{
			return -1;
		}
		p1++;
		p2++;
	}
	return 0;
}

int32 met_aq_getIndex(char *pNumber,AQ_LIST_E type)
{
    wchar file_path[SFS_MAX_PATH] = {0};    
	MET_AQ_FS_HANDLE file_handler = 0;
    int32 index = 0;
    switch(type)
    {
        case PHONE_WHITE_LIST:
            met_mex_aq_get_dat_path(file_path,"phone_white_list.dat");
            break;
        case PHONE_BLACK_LIST:
            met_mex_aq_get_dat_path(file_path,"phone_black_list.dat");
            break;
        case SMS_WHITE_LIST:
            met_mex_aq_get_dat_path(file_path,"sms_white_list.dat");
            break;
        case SMS_BLACK_LIST:
            met_mex_aq_get_dat_path(file_path,"sms_black_list.dat");
            break;
        default:
            met_mex_aq_trace("%s Invalid Index type","[met_aq_getIndex]");
            return -1;
    }
            
    if((file_handler = met_aq_fs_open(file_path, MET_AQ_FS_READ_ONLY)) > 0)
    {
        FILE_HEADER_BLOCK file_header = {0};
        uint size = 0;
        NumberEntry_T *pEntry = NULL;
        met_aq_fs_read(file_handler,&file_header,sizeof(FILE_HEADER_BLOCK),&size);
        if(MAGIC_NUMBER == file_header.magic_number)
        {
            int32 left_size = file_header.entry_count;
            uint read_size = 0, i = 0, bfound = 0;
            pEntry = met_mex_aq_malloc(AQ_BUFFER_LEN * sizeof(NumberEntry_T));
            if(NULL == pEntry)
            {
                met_aq_fs_close(file_handler);
                met_mex_aq_trace("%s malloc mem error","[met_aq_getIndex]");
                return -1;                
            }
            while(left_size > 0 && 1 != bfound)
            {
                if(left_size > AQ_BUFFER_LEN)
                {
                    read_size = AQ_BUFFER_LEN;
                }
                else
                {
                    read_size = left_size;
                }
                met_aq_fs_read(file_handler,pEntry,sizeof(NumberEntry_T) * read_size,&size);
                if(size != sizeof(NumberEntry_T) * read_size)
                {
                    met_mex_aq_trace("%s want %d read %d","[met_aq_getIndex]",sizeof(NumberEntry_T) * read_size,size);
                    break;
                }
                left_size -= read_size;
                for(i = 0; i < read_size;i++)
                {
                    if(0 == met_aq_compare_string(pEntry[i].number,pNumber))
                    {
                        bfound = 1;
                        break;
                    }
                }
            }
            if(bfound)
            {
                index = file_header.entry_count - left_size - read_size + i;                
            }
            else
            {
                met_mex_aq_trace("%s can't find number %s","[met_aq_getIndex]",pNumber);
                index = -1;
            }
            met_mex_aq_free(&pEntry);
            met_aq_fs_close(file_handler);
            return index;
        }
        else
        {
            met_aq_fs_close(file_handler);
            met_mex_aq_trace("%s file damaged!","[met_aq_getIndex]");
            return -1;
        }
    }
    else
    {
        met_mex_aq_trace("%s file open error!","[met_aq_getIndex]");
        return -1;
    }
}

int32 met_aq_getSetting(AQ_SETTING_T *block)
{
	MET_AQ_FS_HANDLE file_handler = 0;   
    int32 result = 0,size = 0;
    wchar path[256] = {0};   
    met_mex_aq_get_dat_path(path,"settings.dat");
    file_handler = met_aq_fs_open(path,MET_AQ_FS_READ_ONLY);
    if(file_handler > 0)
    {
        met_aq_fs_read(file_handler,block,sizeof(AQ_SETTING_T),&size);
        met_aq_fs_close(file_handler);
        if(MAGIC_NUMBER == block->magic_number)
        {
            if(sizeof(AQ_SETTING_T) == size)
            {
                result = 1;
            }
            else if(sizeof(AQ_SETTING_T) > size)
            {
                memset(((char *)block) + size, 0 ,sizeof(AQ_SETTING_T) - size);
                met_mex_aq_trace("%s file size %d","[met_aq_get_Setting]",size);                
                result = 2;
            }
            else
            {
                // should never come here.
                met_mex_aq_trace("%s read more than want","[met_aq_get_Setting]");                
            }
        }
        else
        {
            met_mex_aq_trace("%s file damaged","[met_aq_get_Setting]");
            result = 0;
        }
    }
    else
    {
        met_mex_aq_trace("%s file open error!","[met_aq_get_Setting]");
        result = 0;
    }
    return result;
}

int32 met_aq_showLocationStatus(void)
{
    AQ_SETTING_T settings = {0};
    if(met_aq_getSetting(&settings))
    {
        return settings.bShowLocation;
    }
    return 0;
}
    
int32 met_aq_dealPhoneBlock(char *pNumber)
{
    AQ_SETTING_T settings = {0};
    int32 result = 0;
    if(met_aq_getSetting(&settings))
    {
        int32 index = 0;
        if(settings.bPhoneBlockAll)
        {
            result = 1;
        }
        else
        {
            switch(settings.ePhoneBlockType)
            {
                case AQ_ALLOW_WHITE:
                    index = met_aq_getIndex(pNumber,PHONE_WHITE_LIST);
                    result = (-1 != index) ? 0 : 1;
                    break;
                case AQ_BLOCK_BLACK:
                    index = met_aq_getIndex(pNumber,PHONE_BLACK_LIST);
                    result = (-1 != index) ? 1 : 0;
                    break;
                case AQ_BLOCK_UNKNOWN:
                    // ��Ҫ���绰��
                    result = 2;                
                    break;
                case AQ_BLOCK_NONE:
                default:
                    result = 0;
                    break;
            }
        }
    }
    return result;
}

int32 met_aq_dealSmsBlock(char *pNumber)
{
    AQ_SETTING_T settings = {0};
    int32 result = 0;
    if(met_aq_getSetting(&settings))
    {
        int32 index = 0;
        switch(settings.eSmsBlockType)
        {
            case AQ_ALLOW_WHITE:
                index = met_aq_getIndex(pNumber,SMS_WHITE_LIST);
                result = (-1 != index) ? 0 : 1;
                break;
            case AQ_BLOCK_BLACK:
                index = met_aq_getIndex(pNumber,SMS_BLACK_LIST);
                result = (-1 != index) ? 1 : 0;
                break;
            case AQ_BLOCK_UNKNOWN:
                // ��Ҫ���绰��
                result = 2;                
                break;
            case AQ_BLOCK_NONE:
            default:
                result = 0;
                break;
        }
    }
    return result;
}


int32 met_aq_PhoneWall(
    MN_NUMBER_PLAN_E    number_plan,
    MN_NUMBER_TYPE_E    number_type,
    uint16              num_len,
    uint8              *party_num)
{
    int32 result = 0, ret = 0;
#ifdef MET_MEX_AQ_DEBUG
    int32 ticks = 0;
#endif    
    char Number[25] = {0};
    if(PNULL == party_num)
    {
        return result;
    }
#ifdef MET_MEX_AQ_DEBUG
    ticks = SCI_GetTickCount();
#endif
//  MN_CALLING_NUMBER_T�ṹ���е�party_numΪshort��,����ʵ������Ϊchar���õ�,���Դ˴�ֱ��ʹ��num_len����2
    MexConfig_ConvertBCD2Ansi(Number,25,party_num,num_len,0);

    if(1 == (ret = met_aq_dealPhoneBlock(Number)))
    {
        result = 1;
    }
    else if(2 == ret)
    {
        if(0 == MexInterface_InPhoneBook(number_plan,number_type,num_len,party_num))
        {
            result = 1;
        }
    }
#ifdef MET_MEX_AQ_DEBUG
    ticks = SCI_GetTickCount() - ticks;
    mex_log(1,"%s cost time %d","[met_mex_aq_PhoneWall]",ticks);
    mex_log(1,"%s number %s, block_status %d","[met_mex_aq_PhoneWall]",Number,result);
#endif
    if(1 == result)
    {
        met_mex_aq_save_log(Number,"phone_blocked.txt");
    }

    //SCI_TraceLow:"%s number %s, block_status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MET_MEX_AQ_LIB_1042_112_2_18_2_35_55_2,(uint8*)"ssd",Number,result);
    return result;
}


int32 met_aq_SmsWall(
    MN_NUMBER_PLAN_E    number_plan,
    MN_NUMBER_TYPE_E    number_type,
    uint16              num_len,
    uint8              *party_num)
{
    int32 result = 0, ret = 0;
#ifdef MET_MEX_AQ_DEBUG
    int32 ticks = 0;
#endif    
    char Number[25] = {0};
    if(PNULL == party_num)
    {
        return result;
    }
#ifdef MET_MEX_AQ_DEBUG
    ticks = SCI_GetTickCount();
#endif
    MexConfig_ConvertBCD2Ansi(Number,25,party_num,num_len,0);
    if(1 == (ret = met_aq_dealSmsBlock(Number)))
    {
        result = 1;
    }
    else if(2 == ret)
    {
        if(0 == MexInterface_InPhoneBook(number_plan,number_type,num_len,party_num))
        {
            result = 1;
        }
    }
#ifdef MET_MEX_AQ_DEBUG
    ticks = SCI_GetTickCount() - ticks;
    mex_log(1,"%s cost time %d","[met_mex_aq_SmsWall]",ticks);
    mex_log(1,"%s number %s, block_status %d","[met_mex_aq_SmsWall]",Number,result);
#endif
    if(1 == result)
    {
        met_mex_aq_save_log(Number,"sms_blocked.txt");
    }
    //SCI_TraceLow:"%s number %s, block_status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MET_MEX_AQ_LIB_1086_112_2_18_2_35_55_3,(uint8*)"ssd",Number,result);
    return result;
}

int32 met_aq_GetIPPhone(const uint8 *tele_in, uint8 *tele_out, int32 tele_out_len)
{
    int32 result = 0;    
	if(1 == met_mex_aq_GetAttachedAreaName(tele_in))
	{
        AQ_SETTING_T settings = {0};
        int32 skip_len = 0;
        if(met_aq_getSetting(&settings))
        {
            if(met_need_skip_ip_header(tele_in,&skip_len))
            {
                if(skip_len >= 5)
                {
                    // ����ip ǰ׺��������
                    return 0;
                }
            }
            if( settings.bEnableIPCall
                && 0 != mex_strlen(settings.LocalNumber)
                && 0 != mex_strlen(met_mex_aq_strCityQuhao)
                && 0 != mex_strcmp(settings.LocalNumber,met_mex_aq_strCityQuhao)
                )
            {
                mex_strncpy(tele_out,settings.IpCallPreix,tele_out_len - 1);
                mex_strncat(tele_out,tele_in + skip_len,tele_out_len - strlen(tele_out) - 1);
                result = 1;
            }
        }
	}
	met_mex_aq_reset_confBuf();
    return result;
}

#endif


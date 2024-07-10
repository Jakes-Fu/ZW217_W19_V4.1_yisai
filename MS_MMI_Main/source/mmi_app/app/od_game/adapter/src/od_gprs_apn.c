#include "od_adapter_gprs.h"
#include "od_adapter_log.h"
#include "img_gzip.h"

int Od_DecodeGzip(
                uint8 *src_buf_ptr, //[IN]
                uint32    src_len,//[IN]
                uint8 **dest_buf_ptr,//[IN/OUT]*dest_buf_ptr need to free outside
                uint32    *dest_len_ptr,//[IN/OUT]
                uint32  max_size
                )
{
#if (defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
    IMGGZIP_DECODE_INPUT_PARAM_T	decode_input = {0};
    IMGGZIP_DECODE_OUTPUT_T			decode_output = {0};
    IMGGZIP_DECODE_SRC_PARAM_T		src_param = {0};
    IMGGZIP_DECODE_INFO_PARAM_T		dec_info = {0};
    uint32  i = 0;
    int brw_result = 0;
    IMGGZIP_DECODE_RESULT_E decode_result = IMGGZIP_DECODE_SUCCESS;
        
    if ((PNULL == src_buf_ptr) ||(0 == src_len) ||(PNULL == dest_buf_ptr) ||(PNULL == dest_len_ptr))
    {
        return 5;
    }

    do
    {
        OD_LOG("Od_DecodeGzip: src_len=%d", src_len);

        src_param.src_buf_ptr = src_buf_ptr;

        //MS00217600:避免http buf的尾部带有额外字符
        for (i = src_len -1; i > 0; i--)
        {
            if ((0x0D != src_buf_ptr[i]) && (0x0A != src_buf_ptr[i]))
            {
                break;
            }
            src_len--;
        }
        
        src_param.src_size = src_len;
        IMGGZIP_Get_Info(&src_param, &dec_info);
		OD_LOG("Od_DecodeGzip: src_len=%d,src_img_size", src_len,dec_info.src_img_size);
        if (0 == dec_info.src_img_size)
        {
            brw_result = 1;
            break;
        }
        
        //protect
        if (dec_info.src_img_size > max_size)
        {
            brw_result = 2;
            break;
        }
        else
        {
             *dest_len_ptr = dec_info.src_img_size;
        }

        *dest_buf_ptr = malloc(*dest_len_ptr);
        if (PNULL == *dest_buf_ptr )
        {
            OD_LOG("Od_DecodeGzip ALLOC FAIL");
            brw_result = 3;
            break;
        }
        memset(*dest_buf_ptr , 0x00, *dest_len_ptr);

        if(0 != gzip_initialize(0, 0))
        {	
            OD_LOG("Od_DecodeGzip:gzip_initialize fail");
            brw_result = 4;
            break;
        }

        decode_input.dst_buf_ptr = *dest_buf_ptr;
        decode_input.dst_buf_size = *dest_len_ptr;
        decode_input.src_buf_ptr = src_buf_ptr;
        decode_input.src_size = src_len;
        decode_result = IMGGZIP_Decode(&decode_input,&decode_output);
        
        OD_LOG("[BRW]Od_DecodeGzip:decode_result = %d", decode_result);
        if (IMGGZIP_DECODE_SUCCESS != decode_result)
        {
            OD_LOG("Od_DecodeGzip:IMGGZIP_Decode FAIL");
            brw_result = 5;
            break;
        }
    }while(0);

    if (0 != brw_result)//error, free the dest_buf_ptr
    {
        od_game_free(*dest_buf_ptr);
        *dest_buf_ptr = PNULL;
    }
    
    gzip_release();

    return brw_result;
#else
    return 2;
#endif
}
extern OD_U32 get_od_apn_data_len();
extern OD_U8* get_od_apn_data();

OD_U8* Od_ReplaceChr(OD_U8*buf,OD_U8 old_chr,OD_U8 new_chr)
{
	int i,len;
	if(buf==NULL) return NULL;
	len = strlen(buf);
	for(i=0;i<len;i++)
	{
		if(buf[i] == old_chr)
		{
			buf[i] = new_chr;
		}
	}

	return buf;
}

OD_U8** Od_Split(OD_U8* srcbuf,OD_U8 ch,OD_U16 *arraylen)
{
	OD_U8 *start,*find,*temp;
	OD_U8 **strArray;
	OD_U16 len,count,i;
	OD_U16 *chPoint;

	if(srcbuf==NULL||arraylen==NULL) return NULL;
	*arraylen = 0;
	len = strlen(srcbuf);
	if(len==0) return NULL;
	chPoint = od_game_malloc((len+2)*sizeof(OD_U16));
	if(chPoint==NULL) return NULL;
	start = srcbuf;
	count = 0;
	chPoint[count++]=0;
	
	while(start!=NULL)
	{
		start = strchr(start, ch);
		if(start == NULL)
		{
			break;
		}
		++start;
		chPoint[count++]=start - srcbuf;
		
	}
	if(count==1)// 没有分割符
	{
		od_game_free(chPoint);
		return NULL;
	}
	
	*arraylen = count;
	chPoint[count++]=len+1;
	strArray = od_game_malloc(count*sizeof(OD_U8 **));
	if(strArray==NULL) return NULL;
	for(i=0;i<count-1;i++)
	{
		find = srcbuf + chPoint[i];
		len = chPoint[i+1] -chPoint[i];
		if(len==0) len = 1;
		temp = od_game_malloc(len);
		if(temp==NULL) break;
		memcpy(temp,find,len-1);
		strArray[i] = temp;
	}
	od_game_free(chPoint);
	return strArray;
}

void Od_GetApnFromGzipString(OD_ACCOUNT_DATA_STRUCT *account,char *plmn )
{
	char *outbuf=NULL;
	char *find,*end,**content=NULL;
	int len;
	OD_U16 count,i;
	
	len = 20*1024;
	if(plmn==NULL||account==NULL)
		return;
	if(Od_DecodeGzip(get_od_apn_data(),get_od_apn_data_len(),&outbuf,&len,len)!=0)
	{
		return;
	}
	OD_LOG("Od_GetApnFromGzipString: len=%d",len);
	if(outbuf!=NULL)
	{
		find = strstr(outbuf, plmn);
		end = NULL;
		if(find)
		{
			OD_LOG("Od_GetApnFromGzipString: find OK");
			end = strchr(find, '}');
		}
		
		if(end!=NULL)
		{
			*end = '\0';
			content = Od_Split(find,',',&count);
			if(content!=NULL&&count==6)
			{
				strcpy(account->plmn,content[0]);
				strcpy(account->name,Od_ReplaceChr(content[1],'$','\0'));
				account->auth_type = atoi(content[2]);
				strcpy(account->apn,Od_ReplaceChr(content[3],'$','\0'));
				strcpy(account->username,Od_ReplaceChr(content[4],'$','\0'));
				strcpy(account->password,Od_ReplaceChr(content[5],'$','\0'));
			}
		}
		od_game_free(outbuf);
	}
	if(content!=NULL)
	{
		for(i=0;i<count;i++)
		{
			od_game_free(content[i]);
		}
		od_game_free(content);
	}
}


OD_ACCOUNT_DATA_STRUCT*	od_get_gprs_account_data(char*plmn)
{
	int	i,len,count;
	OD_U32 iPlmn;
	static OD_ACCOUNT_DATA_STRUCT acountData;
	
	OD_LOG("od_get_gprs_account_data plmn:%s",plmn);
	memset(&acountData,0,sizeof(acountData));

	Od_GetApnFromGzipString(&acountData,plmn);
	OD_LOG("OD_ACCOUNT_DATA_STRUCT: \n");
	OD_LOG("acountData.plmn=%s\n",acountData.plmn);
	OD_LOG("acountData.name=%s\n",acountData.name);
	OD_LOG("acountData.apn=%s\n", acountData.apn);
	OD_LOG("acountData.username=%s\n",acountData.username);
	OD_LOG("acountData.password=%s\n",acountData.password);
	OD_LOG("acountData.auth_type=%d\n",acountData.auth_type);
	if(strlen(acountData.plmn)==0)
	{
		return NULL;
	}
	return (OD_ACCOUNT_DATA_STRUCT*)&acountData;
}

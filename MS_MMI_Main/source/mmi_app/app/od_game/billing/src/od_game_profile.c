#include "od_game_profile.h"
#include "mn_api.h"
#include "mn_type.h"
#include "img_gzip.h"


OD_U8 ** od_game_split(char* str, const char* spl,OD_U16 max_count,OD_U16* count)
{
    OD_U8 **strArray;
    char* strPtr = str;
    OD_U16 n = 0;
    char* end;
    OD_U16 spl_len = strlen(spl);
    OD_U16 str_len = strlen(str);
    OD_U16 check_len =0;

    strArray = (OD_U8 **)od_game_malloc(max_count*sizeof(OD_U8 **));

    while(check_len < str_len)
    {
        strPtr = str + check_len;
        end = strstr(strPtr,spl);
        if(end)
        {
            *end = '\0';
        }
        check_len+= strlen(strPtr)+spl_len;
        strArray[n] = (OD_U8*)od_game_malloc(strlen(strPtr)+1);
        strcpy(strArray[n++], strPtr);        
    }
    * count = n;
    return strArray;
} 


OD_S16 od_game_decode_gzip(
                uint8 *src_buf_ptr, //[IN]
                uint32    src_len,//[IN]
                uint8 **dest_buf_ptr,//[IN/OUT]*dest_buf_ptr need to free outside
                uint32    *dest_len_ptr,//[IN/OUT]
                uint32  max_size
                )
{
#if 1//(defined BROWSER_GZIP_SUPPORT) && (defined GZIP_DEC_SUPPORT_GZIP_SW)
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
        src_param.src_buf_ptr = src_buf_ptr;        
        src_param.src_size = src_len;
        IMGGZIP_Get_Info(&src_param, &dec_info);
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

        *dest_buf_ptr = od_game_malloc(*dest_len_ptr);
        if (PNULL == *dest_buf_ptr )
        {
            brw_result = 3;
            break;
        }
        memset(*dest_buf_ptr , 0x00, *dest_len_ptr);

        if(0 != gzip_initialize(0, 0))
        {	
            brw_result = 4;
            break;
        }

        decode_input.dst_buf_ptr = *dest_buf_ptr;
        decode_input.dst_buf_size = *dest_len_ptr;
        decode_input.src_buf_ptr = src_buf_ptr;
        decode_input.src_size = src_len;
        decode_result = IMGGZIP_Decode(&decode_input,&decode_output);
        if (IMGGZIP_DECODE_SUCCESS != decode_result)
        {
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


const OD_U8 g_od_game_profiles_compressed_data[] ={
	#include "od_game_profile_config.data"
};

OD_BOOL od_game_get_profile_info(OD_U32 plmn,od_game_profile_struct* profile)
{
    char *outbuf=NULL;
    OD_U32 len;
    OD_U8 plmnStr[7] = {0};
    OD_U8 *find,*end;
    OD_U8** content = NULL;
    OD_U16 count;
    OD_BOOL ret = OD_FALSE;
    memset(profile,0,sizeof(od_game_profile_struct));

    sprintf(plmnStr,"%d",plmn);

    if(strlen(plmnStr)<5 ||strlen(plmnStr) >6)
    {
        return ret;
    }


    len = 40*1024;

    if(od_game_decode_gzip(g_od_game_profiles_compressed_data,sizeof(g_od_game_profiles_compressed_data),&outbuf,&len,len)!=0)
    {
    	return ret;
    }

    find = strstr(outbuf,plmnStr);

    if(find)
    {
        find = strchr(find,'\t');
        find+=1;
        if(find)
        {
            end = strstr(find,"\r\n");
            *end = '\0';
            content = od_game_split(find,"\t",10,&count);

            if(content!=NULL && count == 9)
            {
                OD_U8 index =0;
                /*
                OD_U32 profile_id;
                OD_U8 short_code[20];
                OD_U8 keyword[20];
                OD_U8 price[10];
                OD_U8 currency_code[10];  
                OD_U8 no_mo;
                OD_BOOL vat_text_is_needed;
                OD_BOOL sms_count_text_is_needed;
                OD_U16 provider_id;
                */
                profile->profile_id = atoi(content[index++]);
                strcpy(profile->short_code,content[index++]);
                strcpy(profile->keyword,content[index++]);
                strcpy(profile->price,content[index++]);
                strcpy(profile->currency_code,content[index++]);
                profile->no_mo = atoi(content[index++]);
                profile->vat_text_is_needed = atoi(content[index++]);
                profile->sms_count_text_is_needed = atoi(content[index++]);
                profile->provider_id = atoi(content[index++]);
                ret = OD_TRUE;
            }
            
            if(content!=NULL)
            {
                OD_U16 i;
                for(i=0;i<count;i++)
                {
                	od_game_free(content[i]);
                }
                od_game_free(content);
            }

        }
    }
    od_game_free(outbuf);
    return ret;    
}


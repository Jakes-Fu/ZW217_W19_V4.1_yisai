/*
和盈互联智能穿戴通讯协议2.0兼容GSM网.pdf
*/
#include "ai_app.h"
#include "ai_func.h"
//#include "ai_ase.h"


void get_rand_str(char s[], int number)
{
    char *str = "567895678945678994567893456789345678834567893456";
    int i,lstr;
    char ss[2] = {0};
	 uint32 time = SCI_GetTickCount();
    lstr = strlen(str);
    
       srand(time);
    for(i = 1; i <= number; i++){
       sprintf(ss,"%c",str[(rand()%lstr)]);//rand()%lstr 
       strcat(s,ss);//
    }
}
//char *user_id	= "ai11224562548762";//for test 
char user_id[16+1]	= {0};//for release

LOCAL void AI_GenerateUID(char *user_id)
{
 
    int len = 0 ;
    int i = 0;
    char imei[15+1] = {0};
    MMIZDT_Net_GetIMEI(MN_DUAL_SYS_1, imei);
    len = strlen(imei); 
    if(len < 15)
    {

         for(i = len ; i < 15 ; i++)
         {
             imei[i] = '6';
         }
    }
    user_id[0] = 'A';
    //user_id[1] = 'i';
    //SCI_MEMCPY(user_id+2,  imei+1 , 14);
	SCI_MEMCPY(user_id+1,  imei , 15);

    AI_LOG("AI__func AI_GenerateUID user_id = %s",user_id);
 

}


 int CTAIHttp_WritePostBody(uint8 * ptr)
{
	uint8_t in[17] = {"0"};  
	uint8_t out[64] = {'0'};	
	uint8_t aes_key_1[17];	  
	uint8_t iv[17]={0}; 
	uint8_t outStr[64] = {'0'};
	int i,aseLen=0;
	char *parameter_data = NULL;
	char aes_key[30] ="k6j48bR66PgQF2yS";
    char api_key[50] ="86804674ce864045b696d5a58bb32988";
	char   m_string[20]  = {"123"}; 
	
	char token[64]={0};
	
	cJSON	*pJsonRoot = NULL;
	char* str_js  = NULL;

	pJsonRoot = cJSON_CreateObject();
	if(NULL == pJsonRoot)
	{
		AI_LOG(" [tuling ai]:makeJson NULL == pJsonRoot");
		return PNULL;
	}
	cJSON_AddStringToObject(pJsonRoot,"ak",api_key);
	cJSON_AddNumberToObject(pJsonRoot,"asr", 2);
	cJSON_AddNumberToObject(pJsonRoot,"tts", 3);
	cJSON_AddNumberToObject(pJsonRoot,"type", 0);
	cJSON_AddNumberToObject(pJsonRoot,"tone", 21);  
	cJSON_AddNumberToObject(pJsonRoot,"volume",6); 
	cJSON_AddNumberToObject(pJsonRoot,"flag", 3);
	AI_LOG(" [tuling ai]:token=%s", g_ai_app.token);
    if(SCI_STRLEN(g_ai_app.token) > 0)
    {
        cJSON_AddStringToObject(pJsonRoot,"token", g_ai_app.token);
    }
    else
    {
        cJSON_AddStringToObject(pJsonRoot,"token", "");
    }
	//cJSON_AddStringToObject(pJsonRoot,"textStr", m_string);
#if 0
	if(identify)
	{
	cJSON_AddStringToObject(root,"identify",identify);
	}
	if(realtime)
	{
	cJSON_AddNumberToObject(root,"realTime", 1);
	}
	if(index)
	{
	cJSON_AddNumberToObject(root,"index", index);
	}
#endif
    AI_GenerateUID(user_id);
	if(strlen(user_id) != 0)
	{
	    memcpy(in, user_id, strlen(user_id));	
	}
	memcpy(aes_key_1, aes_key, strlen(aes_key));	
	memcpy(iv, api_key, 16);		
	AES128_CBC_encrypt_buffer(out, in, 16, aes_key_1, iv);
	for(i=0;i < 16;i++)
	{
		aseLen+=_snprintf(outStr+aseLen,64,"%.2x",out[i]);	 
	}
	cJSON_AddStringToObject(pJsonRoot,"uid", outStr);
	//cJSON_AddStringToObject(pJsonRoot,"token", token);
	str_js = cJSON_Print(pJsonRoot);
	cJSON_Delete(pJsonRoot);
	AI_LOG("[tuling ai]:parameters is :%s\n",str_js);

#if 0
	parameter_data = SCI_ALLOCA(strlen(str_js)+ strlen(upload_parameters) + strlen(boundary) + strlen(end)*2 + strlen(twoHyphens) +1);
	sprintf(parameter_data, upload_parameters, str_js);
	strcat(parameter_data, secondBoundary);

	strcat(tmp_ptr, firstBoundary);
	strcat(tmp_ptr, parameter_data);
	strcat(tmp_ptr, upload_speech);
	#else
	//parameter_data = SCI_ALLOCA(strlen(str_js));
	//memset(parameter_data,0,strlen(str_js));
	strcpy(ptr,str_js);	
	
#endif

}



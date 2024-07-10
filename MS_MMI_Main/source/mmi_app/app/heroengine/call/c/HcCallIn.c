#ifdef HERO_ENGINE_HMGS_SUPPORT
/*-------------------------------------------------------------------------------*

--------------------------------------------------------------------------------*/
#include "HcDataType.h" 
#include "HcFileAdapter.h"
#include "heroEngineApi.h"

//#include "hero_util.h"

//section
char   g_HCNumber[HCNUMBER_LEN];
char   g_HCStrLocation[HCSTRLOCATION_LEN];
char   g_HCStrGsdLocation[HCSTRLOCATION_LEN];
char   g_HcStr_Number[HCNUMBER_LEN];

static U16 Hc_func_setting_item = 0;

#define ELSE else
#define IF_STRCMP(des,des_len) 	if(strncmp((const char*)g_HCNumber+pos, des, des_len) == 0) \
{ \
pos += des_len; \
} \

void InitHcCallApp(void);
void deInitHcCallApp(void);

#if 0
int hr_str_wstrlen(char * txt)
{
	int lenth=0;
	unsigned char * ss=(unsigned char*)txt;

	while(ss[0] | ss[1])
	{
		lenth+=2;
		ss+=2;
	}
	return lenth;
}
#endif
char *hero_ucs2_strncpy(char *strDestination, const char *strSource, uint32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    uint16 count = 1;
    uint32 count1 = 0;
    char *temp = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    size = size * 2;

    /* MMI_ASSERT(!(strDestination == NULL)); */

    while (!((*(strSource + count) == 0) && (*(strSource + count - 1) == 0)) && (count1 < size))
    {

        *(strDestination + count - 1) = *(strSource + count - 1);
        *(strDestination + count) = *(strSource + count);
        count += 2;
        count1 += 2;
    }

    *(strDestination + count - 1) = '\0';
    *(strDestination + count) = '\0';

    return temp;
}

char *hero_ucs2_strcpy(char *strDestination, const char *strSource)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    uint16 count = 1;
    char *temp = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (strSource == NULL)
    {
        if (strDestination)
        {
            *(strDestination + count - 1) = '\0';
            *(strDestination + count) = '\0';
        }
        return temp;

    }

    if (strDestination == NULL || strSource == NULL)
    {
        return NULL;
    }
    while (!((*(strSource + count) == 0) && (*(strSource + count - 1) == 0)))
    {

        *(strDestination + count - 1) = *(strSource + count - 1);
        *(strDestination + count) = *(strSource + count);
        count += 2;
    }

    *(strDestination + count - 1) = '\0';
    *(strDestination + count) = '\0';

    return temp;
}

char *hero_ucs2_strcat(char *strDestination, const char *strSource)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    char *dest = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dest = dest + hr_str_wstrlen(strDestination);

    hero_ucs2_strcpy(dest, strSource);
    return strDestination;

}

char *hero_ucs2_strncat(char *strDestination, const char *strSource, uint32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    char *dest = strDestination;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    dest = dest + hr_str_wstrlen(strDestination);

    hero_ucs2_strncpy(dest, strSource, size);
    return strDestination;

}

void dumpUcs2Byte(char *p)
{
	int len = hr_str_wstrlen(p);
	int i = 0;
	char buff[128] = {0};
	int pos = 0;
	char *pBuff = buff;

	for(i = 0; i < len; i++)
	{
		pos += sprintf(pBuff+pos, "0X%c", p[i]);
	}
}
/********************************************************************
Function:		HcCall_hist_populate_call_list
Description: 	get the city's name from a phone number
Input:		city_number: cellphone or telephone number	
Output:		city_name: the city name
Return:		the length of the city's name
Notice:
*********************************************************************/
int HcCall_hist_populate_call_list(char *city_name, int city_name_size, char *city_number)
{
	int ret_len=0;
	hccall_printf("HCCALLUI :: HcCall_hist_populate_call_list-----start");
	
	if (city_name==NULL || city_number==NULL)
	{
		return 0;
	}

	InitHcCallApp();
	hccall_printf("HCCALLUI :: HcCall_hist_populate_call_list-----city_number=%s", city_number);
	memset(city_name, 0, city_name_size);
	//if(0 == g_hccall_confg.functionset)
	{
		int pos = 0;
		int len = 0;

       	memset(g_HCNumber, 0, sizeof(g_HCNumber));
		memset(g_HCStrLocation,0,sizeof(g_HCStrLocation));
   		
		if( strlen( ( const char * )city_number) > 2 && strlen( ( const char * )city_number ) < 62 )
		{
			strcpy(g_HCNumber, ( const char * )city_number);
			
			IF_STRCMP("17951",5)
			ELSE IF_STRCMP("12593",5)
			ELSE IF_STRCMP("17969",5)
			ELSE IF_STRCMP("17909",5)
			
			IF_STRCMP("+86",3)
			IF_STRCMP("0086",4)
			//·Ö¸ô·û
			hero_ucs2_strcat((S8 *)city_name , (const S8 *) L" ");
			len = GetLocation_ex( g_HCStrLocation,sizeof(g_HCStrLocation),g_HCNumber + pos );
			hccall_printf("HCCALLUI :: HcCall_hist_populate_call_list----ret = %d, len = %d", len, hr_str_wstrlen(g_HCStrLocation));
			if(hr_str_wstrlen(g_HCStrLocation))
			{			
				hero_ucs2_strncat((S8 *)city_name , (const S8 *)g_HCStrLocation, city_name_size-4);
				ret_len = hr_str_wstrlen((S8 *)city_name);
			}
            memset(g_HCStrLocation,0,sizeof(g_HCStrLocation));
			memset(g_HCNumber, 0, sizeof(g_HCNumber));
		}
	}

	{
		int temp_buff_len = 128;
		uint8 temp_buff[128] = {0};
		GUI_WstrToUTF8(temp_buff, 128, (wchar *)city_name, MMIAPICOM_Wstrlen(city_name));
		hccall_printf("[inkleak.zhao] HcCall_hist_populate_call_list  temp_buff = %s", temp_buff);
	}

	hccall_printf("HCCALLUI :: HcCall_hist_populate_call_list----city_name=%s", city_name);
	hccall_printf("HCCALLUI :: HcCall_hist_populate_call_list----ret_len=%d", ret_len);
	deInitHcCallApp();
	return ret_len;
}

/********************************************************************
Function:		InitHcCallApp
Description: 	the entrence function of the hccall application
Input:		void
Output:		void
Return:		void
Notice:		this function is called when the phone is powered on
*********************************************************************/
extern void LocationData_Init_ex(void);
extern void LocationData_deInit_ex(void);
static int inited = 0;
void InitHcCallApp(void)
{
	//hccall_printf("HCCALLUI :: InitHcCallApp inited=%d", inited);
	//if (inited) return;
	hccall_printf("HCCALL :: InitHcCallApp");
	LocationData_Init_ex();
	//inited = 1;
	//gui_start_timer(1000, LocationData_Init_ex); 
}

void deInitHcCallApp(void)
{
	//hccall_printf("HCCALLUI :: InitHcCallApp inited=%d", inited);
	//if (inited) return;
	hccall_printf("HCCALL :: deInitHcCallApp");
	LocationData_deInit_ex();
	//inited = 1;
	//gui_start_timer(1000, LocationData_Init_ex); 
}

int32 hero_get_phone_number_location_ex(uint8* input, int32 input_len, uint8** output, int32* output_len, HR_PLAT_EX_CB *cb)
{
	//hccall_printf("HCCALLUI :: hero_get_phone_number_location_ex inited=%d", inited);
	//InitHcCallApp();
	return HcCall_hist_populate_call_list((char *)(*output), *output_len, (char *)input);
}

#endif // FEATURE_SUPPORT_HERO_HCCALL

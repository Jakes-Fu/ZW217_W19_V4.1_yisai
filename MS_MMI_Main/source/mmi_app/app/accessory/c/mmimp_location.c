/*****************************************************************************
** File Name:		mmifindmp_infor.c                                                
** Author:			zhenhao.huang
** Date:            2007/10
** Copyright:		2007 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:		This file is used to describe mmi stopwatch common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE        	NAME             	DESCRIPTION                               *
** 2007/10/29	zhenhao.huang		Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h" 
#endif

#if defined(HOME_LOCATION_INQUIRE_SUPPORT) && (defined IM_SIMP_CHINESE_SUPPORT|| defined IM_TRAD_CHINESE_SUPPORT)
//#include "sci_types.h"
#include "window_parse.h"
#include "mmk_app.h"
#include "mmifmm_export.h"
#include "mmimp_location.h"
#include "mmi_data.h"
#include "os_api.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guifont.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define RECORD_SIZE					4						// MP Phone number for 2 bytes, city ID 2 bytes
#define MMI_HL_130          130
#define MMI_HL_131          131
#define MMI_HL_132          132
#define MMI_HL_133          133
#define MMI_HL_134          134
#define MMI_HL_135          135
#define MMI_HL_136          136
#define MMI_HL_137          137
#define MMI_HL_138          138
#define MMI_HL_139          139
#define MMI_HL_145          145
#define MMI_HL_147          147
#define MMI_HL_150          150
#define MMI_HL_151          151
#define MMI_HL_152          152
#define MMI_HL_153          153
#define MMI_HL_154          154
#define MMI_HL_155          155
#define MMI_HL_156          156
#define MMI_HL_157          157
#define MMI_HL_158          158
#define MMI_HL_159          159
#define MMI_HL_180          180
#define MMI_HL_181          181
#define MMI_HL_182          182
#define MMI_HL_183          183
#define MMI_HL_185          185
#define MMI_HL_186          186
#define MMI_HL_187          187
#define MMI_HL_188          188
#define MMI_HL_189          189

#define LOCATION_FILE_MAX_NUM_INDEX 10000
#define LOCATION_FILE_NAME_PTR_LEN 3
#define LOCATION_FILE_PATH "E:\\location\\R_ACC_LOCATION_"

/*****************************************************************************/
// 	Description : get home location data by phone string
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL void GetHomeLocationData(
                                  uint8* phone_str,  //in
                                  uint32  *rela_nb,  //out
                                  uint16  *mid_nb,   //out
                                  MMIAUD_RING_DATA_INFO_T *file_info_ptr,  //in/out
                                  BOOLEAN   *is_need_free //out
                                  );

/*****************************************************************************/
// 	Description : find the phone's city text by its city ID
//	Global resource dependence : none
//  Author: zhenhao.huang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPI_FindMPLocationText(	uint8* city_name,	// output in GB2313, buffer length is MMIFINDMP_CITYNAME_LENGTH
										uint16 city			// input city ID
										)
{
	MMIAUD_RING_DATA_INFO_T file_info = {0}; /*lint !e64*/
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD    
    uint8 src_file_path[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
#endif
	uint8* ptr = PNULL;
        
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    strncpy( src_file_path,R_ACC_MPLOCSTR_TCARD,strlen((char*)R_ACC_MPLOCSTR_TCARD));/*lint !e64*/
	GetAreaCodeData(src_file_path,&file_info);
#else
    MMI_GetDataInfo(R_ACC_MPLOCSTR, &file_info);
#endif
	ptr = file_info.data_ptr + (city-1)*MMIFINDMP_CITYNAME_LENGTH;
    if(file_info.data_ptr+file_info.data_len <= ptr)
    {
        return;
    }
	strncpy((char*)city_name, (const char*)ptr, MMIFINDMP_CITYNAME_LENGTH);
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
    if(PNULL != file_info.data_ptr)
    {
        SCI_FREE(file_info.data_ptr);
        file_info.data_ptr = PNULL;
    }
#endif
}
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
/*****************************************************************************/
// 	Description : get home location data by phone string
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void GetAreaCodeData( 
                                  uint8* file_path,  //in
                                  MMIAUD_RING_DATA_INFO_T *file_info_ptr  //in/out
                                  )
{
    uint16	file_path_w[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    MMIFILE_HANDLE                  file_handle;
    uint16  mid_nb = 0;
        
    if (PNULL == file_path ||PNULL == file_info_ptr)
    {
        return;
    }

	MMI_STRNTOWSTR( file_path_w, MMIFILE_FULL_PATH_MAX_LEN, file_path, MMIFILE_FULL_PATH_MAX_LEN, MMIFILE_FULL_PATH_MAX_LEN );/*lint !e64*/
	file_handle = MMIAPIFMM_CreateFile((const uint16*)file_path_w,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);

     if(SFS_INVALID_HANDLE != file_handle)
     { 
         file_info_ptr->type = 0;
         file_info_ptr->data_len = MMIAPIFMM_GetFileSize(file_handle);
         file_info_ptr->data_ptr = SCI_ALLOC_APP(file_info_ptr->data_len);
         
         if(file_info_ptr->data_ptr != PNULL)
         {
             MMIAPIFMM_ReadFile(file_handle, file_info_ptr->data_ptr, file_info_ptr->data_len, &mid_nb, PNULL);/*lint !e64*/
         }
         
         MMIAPIFMM_CloseFile(file_handle);
     }
}
#endif
/*****************************************************************************/
// 	Description : get home location data by phone string
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
LOCAL void GetHomeLocationData(
                                  uint8* phone_str,  //in
                                  uint32  *rela_nb,  //out
                                  uint16  *mid_nb,   //out
                                  MMIAUD_RING_DATA_INFO_T *file_info_ptr,  //in/out
                                  BOOLEAN   *is_need_free //out
                                  )
{
    uint16	location_file_path_w[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16	location_file_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16	name_patr[LOCATION_FILE_NAME_PTR_LEN] = {0};
    MMIFILE_HANDLE                  file_handle; 
    uint32 phoneNB = 0;     /* lint !e529*/
    if (PNULL == phone_str 
        ||PNULL == rela_nb 
        ||PNULL == mid_nb
        ||PNULL == file_info_ptr
        ||PNULL == is_need_free)
    {
        return;
    }
#if defined HOME_LOCATION_INQUIRE_SUPPORT_TCARD
	phoneNB = 0;
    strncpy( location_file_path,LOCATION_FILE_PATH,strlen((char*)LOCATION_FILE_PATH));/*lint !e64*/
    strncpy( name_patr,phone_str,LOCATION_FILE_NAME_PTR_LEN);/*lint !e64*/
    strcat(location_file_path,name_patr);/*lint !e64*/
    strcat(location_file_path,".pnl");/*lint !e64*/

	 MMI_STRNTOWSTR( location_file_path_w, MMIFILE_FULL_PATH_MAX_LEN, location_file_path, MMIFILE_FULL_PATH_MAX_LEN, MMIFILE_FULL_PATH_MAX_LEN );/*lint !e64*/
	 file_handle = MMIAPIFMM_CreateFile((const uint16*)location_file_path_w,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ,0,0);

     if(SFS_INVALID_HANDLE != file_handle)
     { 
         *rela_nb = atoi((char*)phone_str+LOCATION_FILE_NAME_PTR_LEN);			
         file_info_ptr->type = 0;
         file_info_ptr->data_len = MMIAPIFMM_GetFileSize(file_handle);
         file_info_ptr->data_ptr = SCI_ALLOC_APP(file_info_ptr->data_len);
         
         if(file_info_ptr->data_ptr != PNULL)
         {
             MMIAPIFMM_ReadFile(file_handle, file_info_ptr->data_ptr, file_info_ptr->data_len, mid_nb, PNULL);/*lint !e64*/
         }
         
         MMIAPIFMM_CloseFile(file_handle);
        *is_need_free = TRUE;
     }
     else
     {
        *is_need_free = FALSE;
     }
#elif defined HOME_LOCATION_INQUIRE_SUPPORT_CODE
	phoneNB = atoi((char*)phone_str);
	// open the buffer based on the phone number
    switch(phoneNB/LOCATION_FILE_MAX_NUM_INDEX) 
    {
    case MMI_HL_130:
		MMI_GetDataInfo(R_ACC_LOCATION_130, file_info_ptr);
    	break;
    case MMI_HL_131:
		MMI_GetDataInfo(R_ACC_LOCATION_131, file_info_ptr);
    	break;
    case MMI_HL_132:
		MMI_GetDataInfo(R_ACC_LOCATION_132, file_info_ptr);
    	break;
    case MMI_HL_133:
		MMI_GetDataInfo(R_ACC_LOCATION_133, file_info_ptr);
    	break;
    case MMI_HL_134:
		MMI_GetDataInfo(R_ACC_LOCATION_134, file_info_ptr);
    	break;
    case MMI_HL_135:
		MMI_GetDataInfo(R_ACC_LOCATION_135, file_info_ptr);
    	break;
    case MMI_HL_136:
		MMI_GetDataInfo(R_ACC_LOCATION_136, file_info_ptr);
    	break;
    case MMI_HL_137:
		MMI_GetDataInfo(R_ACC_LOCATION_137, file_info_ptr);
    	break;
    case MMI_HL_138:
		MMI_GetDataInfo(R_ACC_LOCATION_138, file_info_ptr);
    	break;
    case MMI_HL_139:
		MMI_GetDataInfo(R_ACC_LOCATION_139, file_info_ptr);
    	break;
    case MMI_HL_145:
		MMI_GetDataInfo(R_ACC_LOCATION_145, file_info_ptr);
    	break;
    case MMI_HL_147:
		MMI_GetDataInfo(R_ACC_LOCATION_147, file_info_ptr);
    	break;
    case MMI_HL_150:
		MMI_GetDataInfo(R_ACC_LOCATION_150, file_info_ptr);
    	break;
    case MMI_HL_151:
		MMI_GetDataInfo(R_ACC_LOCATION_151, file_info_ptr);
    	break;
    case MMI_HL_152:
		MMI_GetDataInfo(R_ACC_LOCATION_152, file_info_ptr);
    	break;
    case MMI_HL_153:
		MMI_GetDataInfo(R_ACC_LOCATION_153, file_info_ptr);
    	break;
    case MMI_HL_155:
		MMI_GetDataInfo(R_ACC_LOCATION_155, file_info_ptr);
    	break;
    case MMI_HL_156:
		MMI_GetDataInfo(R_ACC_LOCATION_156, file_info_ptr);
    	break;
    case MMI_HL_157:
		MMI_GetDataInfo(R_ACC_LOCATION_157, file_info_ptr);
    	break;
    case MMI_HL_158:
		MMI_GetDataInfo(R_ACC_LOCATION_158, file_info_ptr);
    	break;
    case MMI_HL_159:
		MMI_GetDataInfo(R_ACC_LOCATION_159, file_info_ptr);
    	break;
    case MMI_HL_180:
		MMI_GetDataInfo(R_ACC_LOCATION_180, file_info_ptr);
    	break;
    case MMI_HL_181:
		MMI_GetDataInfo(R_ACC_LOCATION_181, file_info_ptr);
    	break;    
    case MMI_HL_182:
		MMI_GetDataInfo(R_ACC_LOCATION_182, file_info_ptr);
    	break;
    case MMI_HL_183:
		MMI_GetDataInfo(R_ACC_LOCATION_183, file_info_ptr);
    	break;
    case MMI_HL_185:
		MMI_GetDataInfo(R_ACC_LOCATION_185, file_info_ptr);
    	break;    
    case MMI_HL_186:
		MMI_GetDataInfo(R_ACC_LOCATION_186, file_info_ptr);
    	break;
    case MMI_HL_187:
		MMI_GetDataInfo(R_ACC_LOCATION_187, file_info_ptr);
    	break;
    case MMI_HL_188:
		MMI_GetDataInfo(R_ACC_LOCATION_188, file_info_ptr);
    	break;
    case MMI_HL_189:
		MMI_GetDataInfo(R_ACC_LOCATION_189, file_info_ptr);
    	break;
    default:
        file_info_ptr->data_ptr =PNULL;
        file_info_ptr->data_len = 0;
    	break;
    }
	*rela_nb = atoi((char*)phone_str+LOCATION_FILE_NAME_PTR_LEN);
    *mid_nb = file_info_ptr->data_len;
    *is_need_free = FALSE;
#endif
}
/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//	Note: return the ID of location, if not found return 0
/*****************************************************************************/
PUBLIC uint16 MMIAPI_FindMPLocationID(	uint8* tel_num_str,		// input phone number string in ASCII
										int32  tel_num_length   // string length
									)
{
	MMIAUD_RING_DATA_INFO_T file_info = {0}; /*lint !e64*/
	uint32 curpos = 0;

	// the first 7 digits of mobile phone number in ASCII, 13XXXXX
	uint8  phone_str[MPNB_USE_LENGTH+1] = {0};
	// the first 7 digits of mobile phone number in integer
	//uint32 phoneNB = 0;      /* lint !e529*/
	// the relative phone number to be found
	uint32 rela_nb = 0;
	// the finally found city ID, used to retrieve the city strings
	uint16 city = 0;
	
	// the phone number stored in the mid record(referred by mid)
	uint16 mid_nb  = 0;
	// the phone number stored in the record after mid
	uint16 next_nb = 0;

	// the record position with the data files
	uint32 start = 0;
	uint32 end   = 0;
	uint32 mid   = 0;
    BOOLEAN is_need_free_data = FALSE;
    
    if(PNULL == tel_num_str)
    {
	    //MMI_ASSERT_WARNING("tel_num_str != PNULL");/*assert verified*/
        return 0;
    }

	// it is not a mobile phone
	if( tel_num_length < MPNB_TOTAL_LENGTH )
	{
//		SCI_TRACE_LOW("MMIAPI_FindMPLocation: NUMBER %s IS TOO SHORT FOR A VALID MP",tel_num_str);
		return 0;
	}

	// extract the first MPNB_USE_LENGTH digits of a mobile phone number
	strncpy( (char*)phone_str, (char*)(tel_num_str+tel_num_length-MPNB_TOTAL_LENGTH), MPNB_USE_LENGTH );
	// convert the string to integer
    GetHomeLocationData(phone_str,&rela_nb,&mid_nb,&file_info,&is_need_free_data);
	// it is not a mobile phone or fail to open file
	if( PNULL == file_info.data_ptr )
	{
        if (is_need_free_data)
        {
	        SCI_FREE(file_info.data_ptr); 
        }
		return 0;
	}

	// record numbers
	end = file_info.data_len/RECORD_SIZE -1;
	
	while(1)/*lint !e716*/
	{
		// get the mid record
		mid = (start + end )/2;

		// move the file pointer and read the data
		curpos = mid*RECORD_SIZE;
		mid_nb = MMICOM_READ_2BYTE(file_info.data_ptr, curpos);

		// skip the city data to get the next record
		if( (curpos+RECORD_SIZE) == file_info.data_len )
		{
			next_nb = LOCATION_FILE_MAX_NUM_INDEX;
		}
		else
		{
			curpos = (mid+1)*RECORD_SIZE;
			next_nb = MMICOM_READ_2BYTE(file_info.data_ptr, curpos);
		}

		if(rela_nb < mid_nb)
		{
			if( mid > start )
				end = mid-1;
			else
				end = start;
		}
		else if((rela_nb >= mid_nb) && (rela_nb < next_nb))
		{
			// move the file pointer and read the data
			curpos = mid*RECORD_SIZE+RECORD_SIZE/2;
			city = MMICOM_READ_2BYTE(file_info.data_ptr, curpos);
			break;
		}
		else
		{
			if( mid < end )
				start = mid+1;
			else
				start = end;
		}
	}
    if (is_need_free_data)
    {
	    SCI_FREE(file_info.data_ptr); 
    }
	return city;
}

/*****************************************************************************/
// 	Description : find the phone's location by its number
//	Global resource dependence : none
//  Author: zhenhao.huang
//  Mender: Mingxuan.Zhu
//	Note: return length of location_str, if not found return 0
//        this function is invoked by atest, which is added a outputed param named phone_location.
/*****************************************************************************/
PUBLIC uint16 MMIAPI_FindMPLocation(	uint8*   tel_num_str,		// input phone number string in ASCII
                                        int32    tel_num_length,    // string length
                                        uint16*  location_str,	    // output string, length is MMIFINDMP_CITYNAME_LENGTH
                                        char*    phone_location     // output param : get the name of the phone location
                                    )
{
    // the finally found city ID, used to retrieve the city strings
    uint16 city = 0;
    // temp text to store city name in gb2313
    uint8  city_str[MMIFINDMP_CITYNAME_LENGTH] = {0};
    
    city = MMIAPI_FindMPLocationID( tel_num_str, tel_num_length );
    // get the city text
    if( city )
    {
        MMIAPI_FindMPLocationText(city_str, city);
#ifdef _ATEST_
#ifdef WIN32
        SCI_STRCPY(phone_location, (char*)city_str); //  get the name of the phone location. - added by Mingxuan.Zhu
#endif//WIN32
#endif//_ATEST_
        return GUI_GBToWstr(location_str, city_str, strlen((char*)city_str));
    }
    else
        return 0;
}

#endif//HOME_LOCATION_INQUIRE_SUPPORT



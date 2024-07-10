// ============================================================================
// File Name:       ConPanLayer.cpp                                                  
// Author:          Jianyong Gao
// DATE:            2002-12-18
// Copyright:       2002 Spreadtrum, Incoporated. All Rights Reserved.         
// Description:     provide functions to get call number string from call number 
//                  struct or otherwise and some folk functions to pass link.
// ============================================================================

// ============================================================================
//                        Edit History
// ----------------------------------------------------------------------------
// DATE             NAME                DESCRIPTION
// 2002-12-18       Jianyong Gao        Create.
// ============================================================================


#include "stdafx.h"
#include "math.h"
#include "sci_types.h"
#include "ConPanLayer.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif


/*****************************************************************************/
// 	Description : This function transform the BCD number to telephone string
//	Global resource dependence : none
//  Author:       Jianyong Gao
//	Note:
/*****************************************************************************/
 int CPL_TransformBcdToStr(
	         uint8                 *bcd_ptr, // in the BCD array
             int32                 length,      // in the length of BCD array
			 uint8                 *tel_str,    // out the telephone number string
	         uint8					strlength)
 {
	 int32        i;
	 uint8        temp;

	 ASSERT(PNULL != tel_str);
	 ASSERT(ADDR_BCD_MAX_LEN >= length);
	 for (i=0; i<length; i++)
	 {
		 if (i>=strlength-1)
		 {
			 return -1;	//length of tel_str is too short.
		 }
		 temp = (uint8)(*bcd_ptr & 0x0f);
		 switch (temp)
		 {
		 case 10:
			 *tel_str++ = '*';
			 break;
		 case 11:
			 *tel_str++ = '#';
			 break;
		 case 12:
			 *tel_str++ = 'a';
			 break;
		 case 13:
			 *tel_str++ = 'b';
		 case 14:
			 *tel_str++ = 'c';
		 case 15:
			 ASSERT(FALSE);
			 break;
		 default:
			 *tel_str++ = (uint8)(temp + '0');
			 break;
		 }

		 if ((*bcd_ptr & 0xf0) != 0xf0)
		 {
			 *tel_str++ = (uint8)((*bcd_ptr >> 4) + '0');
		 }
		 bcd_ptr++;
	 }
	 *tel_str = '\0';
	 return 0;
}

/*****************************************************************************/
// 	Description : This function transform telephone string to the BCD number. 
//	Global resource dependence : none
//  Author:       Jianyong Gao
//	Note:
/*****************************************************************************/
int CPL_TransformStrToBcd(
                uint8             *bcd_ptr,  // out: called BCD number array
				uint16            *length_ptr, // out: the BCD array length
				uint8             *tel_str,     // in:  telephone number string
				int32             str_length   // in: telephone number string length
				)  
{
	int32         i = 0;

	ASSERT(PNULL != length_ptr);
	ASSERT(PNULL != tel_str);

	*length_ptr = (uint8)((str_length + 1) / 2);

	for(i=0; i < *length_ptr; i++)
	{
		switch (*tel_str)
		{
		case '*':
			bcd_ptr[i] = 0x0A;
			break;
		case '#':
			bcd_ptr[i] = 0x0B;
			break;
		case 'a':
		case 'A':
			bcd_ptr[i] = 0x0C;
			break;
		case 'b':
		case 'B':
			bcd_ptr[i] = 0x0D;
			break;
		case 'c':
		case 'C':
			bcd_ptr[i] = 0x0E;
			break;
		default:
			if (*tel_str >= '0' && *tel_str <= '9')
			{
				bcd_ptr[i] = (uint8)(*tel_str - '0');
			}
			else
			{
				return -1;
			}
			break;
		}

		tel_str++;

		switch (*tel_str)
		{
		case '*':
			bcd_ptr[i] += 0xA0;
			break;
		case '#':
			bcd_ptr[i] += 0xB0;
			break;
		case 'a':
		case 'A':
			bcd_ptr[i] += 0xC0;
			break;
		case 'b':
		case 'B':
			bcd_ptr[i] += 0xD0;
			break;
		case 'c':
		case 'C':
			bcd_ptr[i] += 0xE0;
			break;
		case '\0':
			bcd_ptr[i] += 0xF0;
			break;
		default:
			if ((*tel_str >= '0') && 
				(*tel_str <= '9'))
			{
				bcd_ptr[i] = (uint8)(bcd_ptr[i] + ((*tel_str - '0') << 4));
			}
			else
			{
				return -1;
			}
			break;
		}
		tel_str++;
	}
	return 0;
}

/*****************************************************************************/
// 	Description : This function get telephone string from call number struct. 
//	Global resource dependence : none
//  Author:       Jianyong Gao
//	Note:
/*****************************************************************************/
int GetStrFromCall(
				CALLED_NUMBER_T * pcall,	//in 
				unsigned char *pcallnostr,			//out
				uint8 strlength)			//in,memory of pcallnostr 
{
			if (pcall==NULL||pcallnostr==NULL)
				return -1;

			return CPL_TransformBcdToStr(pcall->party_num,
									pcall->num_len,
									pcallnostr,
									strlength);					
}

/*****************************************************************************/
// 	Description : This function transform telephone string to call number 
//                struct. 
//	Global resource dependence : none
//  Author:       Jianyong Gao
//	Note:
/*****************************************************************************/
int SetStrToCall(
				 CALLED_NUMBER_T * pcall,	//out 
				unsigned char *pcallnostr			//in
				)			
{
	if (pcall == NULL || pcallnostr == NULL)
    {
		return -1;
    }
	pcall->number_type = 0;
	pcall->number_plan = 1;
	return CPL_TransformStrToBcd(
                pcall->party_num,  
				&pcall->num_len,
				(uint8 *) pcallnostr,   
				strlen((const char *)pcallnostr));

}
/*****************************************************************************/
// 	Description : This function transform telephone string to call number 
//                struct. 
//	Global resource dependence : none
//  Author:       Jianyong Gao
//	Note:
/*****************************************************************************/
int SetStrToCalling(
				 CALLING_NUMBER_T * pcall,	//out 
				unsigned char *pcallnostr			//in
				)			
{
	if (pcall == NULL || pcallnostr == NULL)
    {
		return -1;
    }
	pcall->number_type = 0;
	pcall->number_plan = 1;
	return CPL_TransformStrToBcd(
                pcall->party_num,  
				&pcall->num_len,
				(uint8 *) pcallnostr,   
				strlen((const char *)pcallnostr));

}
/*****************************************************************************/
// 	Description : This function transform telephone string to the BCD number. 
//	Global resource dependence : none
//  Author:       Jianyong Gao
//	Note:
/*****************************************************************************/
int CPL_TransformCharToBcd(
                uint8             *bcd_ptr,  // out: called BCD number array
				uint8             char_str     // in:  telephone number string
				)  
{
	switch (char_str)
	{
		case '*':
			*bcd_ptr = 0x0A;
			break;
		case '#':
			*bcd_ptr = 0x0B;
			break;
		case 'a':
		case 'A':
			*bcd_ptr = 0x0C;
			break;
		case 'b':
		case 'B':
			*bcd_ptr = 0x0D;
			break;
		case 'c':
		case 'C':
			*bcd_ptr = 0x0E;
			break;
		default:
			if (char_str >= '0' && char_str <= '9')
			{
				*bcd_ptr = (uint8)(char_str - '0');
			}
			else
			{
				return -1;
			}
			break;
	}
}

/*****************************************************************************/
// 	Description : This function transform time string to struct MN_SMS_TIME_STAMP_T 
//                struct. 
//	Global resource dependence : none
//  Author:       Sunsome.Ju
//	Note:
/*****************************************************************************/
int SetStrToTime(
				 MN_SMS_TIME_STAMP_T * pcall,	//out 
				unsigned char *pcallnostr			//in
				)			
{
	int32 length = 0;
	uint8 bcd_ptr = 0;
	if (pcall == NULL || pcallnostr == NULL)
    {
		return -1;
    }

	length = strlen((const char *)pcallnostr);

	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->year =  bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->year =  pcall->year * 10 + bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->month =  bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->month =  pcall->month * 10 + bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->day =  bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->day =  pcall->day * 10 + bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->hour =  bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->hour =  pcall->hour * 10 + bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->minute =  bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->minute =  pcall->minute * 10 + bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->second =  bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->second =  pcall->second * 10 + bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->timezone =  bcd_ptr;
	CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
	pcallnostr++;
	pcall->timezone =  pcall->timezone * 10 + bcd_ptr;

}


/*****************************************************************************/
// 	Description : This function transform TP_MR string to TP_MR 
//                struct. 
//	Global resource dependence : none
//  Author:       Sunsome.Ju
//	Note:
/*****************************************************************************/
int SetStrToMR(
				 MN_SMS_TP_MR_T * pcall,	//out 
				unsigned char *pcallnostr			//in
				)			
{
	int i = 0;
	int32 length = 0;
	uint8 bcd_ptr = 0;

	if (pcall == NULL || pcallnostr == NULL)
    {
		return -1;
    }
	*pcall = 0;

	length = strlen((const char *)pcallnostr);

	for(i=0;i<length;i++)
	{
		*pcall = *pcall * 10;
		CPL_TransformCharToBcd(&bcd_ptr,*pcallnostr);
		*pcall =  *pcall + bcd_ptr;
		pcallnostr++;
	}
}

/*****************************************************************************/
// 	Description : This function transform telephone string to mn call number 
//                struct. 
//	Global resource dependence : none
//  Author:       Jianyong Gao
//	Note:
/*****************************************************************************/

int SetStrToMNCall(MN_CALLED_NUMBER_T *pcall,
			   unsigned char *pcallnostr)
{
    if (pcall == NULL || pcallnostr == NULL)
    {
		return -1;
    }  
    pcall->number_type = MN_NUM_TYPE_UNKNOW;
    pcall->number_plan = MN_NUM_PLAN_UNKNOW;
   	return CPL_TransformStrToBcd(
        pcall->party_num,  
        &pcall->num_len,
        (uint8 *) pcallnostr,   
        strlen((const char *)pcallnostr));

}
#ifdef   __cplusplus
    }
#endif



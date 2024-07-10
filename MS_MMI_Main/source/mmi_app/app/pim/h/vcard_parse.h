/*******************************************************************************
Copyright(c) 2005 - 2008 Hisense Mobile Commnunications Technology Co,Ltd
All Rights Reserved. By using this module you agree to the terms of the
Hisense Mobile Commnunications Technology Co,Ltd License Agreement for it.
*******************************************************************************/
/******************************************************************************
*Filename    : Vcard_parse.h
*
*Description: 
*
*Notes        :N/A
*
*---------------------------------------------------------------------------
*Change History: 
*----------------------------------------------------------------------------
*
*2009-7-7 ,wangxiaolin,create originally.
*
*******************************************************************************/

#ifndef __VCARD_PARSE_H__
#define __VCARD_PARSE_H__

#include "vcard.h"
#include "vcal.h"
#include "mmipb_export.h"
#include "mmifmm_export.h"
#include "ffs.h"



//add by wxl 2007.04.26
/*convert the utf8 bytes stream to Unicode string*/
void	VmByteToUnicode (const char *UTF8Start, uint32 UTF8Size,
			uint16 *UCS2Start, uint32 UCS2Size);

//add by wxl 2007.04.26
/* Convert wide character string (Unicode) to UTF8 byte stream. */
void VmUnicodeToByte(const uint16 *UCS2Start, uint32 UCS2Size,
			uint8 *UTF8Start, uint32 UTF8Size);
int pim_unicode_strlen(unsigned short *src);

int EncodeQuoted(const unsigned char* pSrc, char* pDst, int nSrcLen );

int DecodeQuoted(const char* pSrc, unsigned char* pDst, int nSrcLen);

void uim_tk_test_print_data(uint8* data,uint8 data_len);

BOOLEAN FileIsVcard(char *inFile);
char *IdentifySemicolon(char *inFile, char *found);
signed int IdentifySemiColonOrDelimiter(char *inFile);
char *IdentifyDelimiters(char *inFile, char *eol);
signed int IdentifyEncoding(char *inFile, IRMC_ENCODING_TYPE *encoding);

signed int IdentifyCharSet(char *inFile, char **charSet);
signed int IdentifyLanguage(char *inFile, char **language);
signed int IdentifyValueLocation(char *inFile, IRMC_VALUE_LOCATION_TYPE *valueLocation);
signed int IdentifyImageType(char *inFile, VCARD_IMAGE_CONSTANT *imageType);
signed int IdentifyAdrType(char *inFile, VCARD_ADR_TYPE *adrType);
signed int IdentifyTelType(char *inFile, VCARD_TEL_TYPE *telType);
signed int IdentifyEmailType(char *inFile, VCARD_EMAIL_CONSTANT *emailType);
signed int IdentifyAdrValue(char *inFile, VCARD_ADR_PROPERTY_TYPE *adr);
signed int IdentifyLabelValue(char *inFile, VCARD_LABEL_PROPERTY_TYPE *label);
signed int IdentifyTelValue(char *inFile, VCARD_TEL_PROPERTY_TYPE *tel);
signed int IdentifyEmailValue(char *inFile, VCARD_EMAIL_PROPERTY_TYPE *email);
signed int IdentifySoundType(char *inFile, VCARD_SOUND_CONSTANT *soundType);
signed int IdentifyKeyType(char *inFile, VCARD_KEY_CONSTANT *keyType);
signed int ExtractVersion(const char *inFile, IRMC_VERSION_PORPERTY_TYPE *irmcVer);
signed int 	IdentifyPropertyValue(char *headPtr, char *tailPtr, char **propertyValue);
signed int ExtractNameFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractFNFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractPhotoFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractBirthDateFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractAdrFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractLabelFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractTelFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractEmailFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractMailerFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractTimezone(const char *inFile, IRMC_TIME_ZONE_PROPERTY_TYPE *timezone);
signed int ExtractGeographic(const char *inFile, IRMC_GEOGRAPHIC_PROPERTY_TYPE *geographic);
signed int ExtractTitleFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractRoleFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractLogoFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractOrgFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractNoteFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractLastRevFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractSoundFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractUrlFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractUidFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ExtractKeyFrVcard(const char *inFile, VCARD_TYPE *vcard);
signed int ReleaseVcardMem(VCARD_TYPE *vcard);
//int WriteEncoding(IRMC_PROPERTY_TYPE *property, char *dataPtr);

//signed int ReleaseVcalendarMem(VCALENDAR_TYPE *vcalendar);
//BOOLEAN FileIsVcalendar(char *inFile);
//signed int ExtractProductId(const char *inFile, VCALENDAR_PRODID_PROPERTY_TYPE *prodid);
//signed int ExtractDtStart(const char *inFile, char *endFile, VCALENDAR_DTSTART_PROPERTY_TYPE *dtStart);
//signed int ExtractDtEnd(const char *inFile, char *endFile, VCALENDAR_DTEND_PROPERTY_TYPE *dtEnd);
//signed int ExtractDescription(const char *inFile, char *endFile, VCALENDAR_DESCRIPTION_PROPERTY_TYPE *description);
//signed int ExtractLocation(const char *inFile, char *endFile, VCALENDAR_LOCATION_PROPERTY_TYPE *location);
//signed int ExtractSummary(const char *inFile, char *endFile, VCALENDAR_SUMMARY_PROPERTY_TYPE *summary);
//signed int ExtractPriority(const char *inFile, char *endFile, VCALENDAR_PRIORITY_PROPERTY_TYPE *priority);
//signed int ExtractUid(const char *inFile, char *endFile, VCALENDAR_UID_PROPERTY_TYPE *uid);
//signed int ExtractEventFrVcalendar(const char *inFile, VCALENDAR_TYPE *vcalendar);
//signed int VcardParser(char *dataPtr, VCARD_TYPE *vcard);
signed int VcardParser(char *dataPtr, VCARD_TYPE *vcard,MMIPB_CONTACT_T *pb_entry);

#ifdef HS_PIM_SUPPORT_ALL_FIELD  //Added by wangxiaolin  2009.7.9 for 电话簿其他字段  
   #ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for 大对象
   int PhotoFile2Base64String(const wchar* spFileName,char * base64_buffer);
   BOOL Base64String2PhotoFile(char* base64_str_ptr,const wchar* spFileName);
   #endif
#endif
 
#endif

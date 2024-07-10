/*******************************************************************************
Copyright(c) 2005 - 2008 Hisense Mobile Commnunications Technology Co,Ltd
All Rights Reserved. By using this module you agree to the terms of the
Hisense Mobile Commnunications Technology Co,Ltd License Agreement for it.
*******************************************************************************/
/******************************************************************************
*Filename    : D:\T3901\t3901_0706\MS_MMI\source\mmi_app\app\pim\h\vCard.h
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

#ifndef _VCARD_
#define _VCARD_
//#ifndef WIN32
#include "List.h"
#include "sci_types.h"
//#endif

/***********根据规范，定义各字段允许上传的最大长度***************/
#define PIM_MAX_FULL_NAME_LEN                       30
#define PIM_MAX_NUM_LEN                       30
#define PIM_MAX_EMAIL_LEN                       50

#define BEGIN_VCARD_DELIMITER		"BEGIN:VCARD"
#define END_VCARD_DELIMITER			"END:VCARD"
#define VERSION_PROPERTY			"\r\nVERSION"
#define NAME_PROPERTY				"\r\nN"
#define FN_PROPERTY					"\r\nFN"
#define PHOTO_PROPERTY				"\r\nPHOTO"
#define BDAY_PROPERTY 				"\r\nBDAY"
#define ADR_PROPERTY				"\r\nADR"
#define LABEL_PROPERTY				"\r\nLABEL"
#define TEL_PROPERTY				"\r\nTEL"
#define EMAIL_PROPERTY				"\r\nEMAIL"
#define MAILER_PROPERTY				"\r\nMAILER"
#define TIMEZONE_PROPERTY			"\r\nTZ"
#define GEOGRAPHIC_PROPERTY			"\r\nGEO"
#define TITLE_PROPERTY				"\r\nTITLE"
#define ROLE_PROPERTY				"\r\nROLE"
#define LOGO_PROPERTY				"\r\nLOGO"
#define ORG_PROPERTY				"\r\nORG"
#define NOTE_PROPERTY				"\r\nNOTE"
#define LAST_REV_PROPERTY			"\r\nREV"
#define SOUND_PROPERTY				"\r\nSOUND"
#define URL_PROPERTY				"\r\nURL"
#define UID_PROPERTY				"\r\nUID"
#define KEY_PROPERTY				"\r\nKEY"
#define ENCODING					"ENCODING="
#define CHARSET						"CHARSET="
#define LANGUAGE					"LANGUAGE="
#define VALUE						"VALUE="
#define TYPE						"TYPE="
#define BASE_64_ENCODING			"BASE64"
#define QUOTED_PRINTABLE_ENCODING	"QUOTED-PRINTABLE"
#define EIGHT_BIT_ENCODING			"8BIT"
#define CONTENT_ID_VL				"CONTENT-ID"
#define URL_VL						"URL"
#define DELIMITERS					";"
#define LFCR						"\r\n"
#define SEMICOLON					":"
#define LFCRLWSP					"\r\n "
					
enum
{
	ADR_DOM=0,
	ADR_INTL,
	ADR_POSTAL,
	ADR_PARCEL,
	ADR_HOME,
	ADR_WORK,
	TOTAL_ADR
};


enum
{
	TEL_PREF=0,
	TEL_WORK,
	TEL_HOME,
	TEL_VOICE,
	TEL_FAX,
	TEL_MSG,
	TEL_CELL,
	TEL_PAGER,
	TEL_BBS,
	TEL_MODEM,
	TEL_CAR,
	TEL_ISDN,
	TEL_VIDEO,
	TOTAL_TEL
};

typedef enum
{
	EMAIL_AOL=0,
	EMAIL_APPLELINK,
	EMAIL_ATTMAIL,
	EMAIL_CIS,
	EMAIL_EWORLD,
	EMAIL_INTERNET,
	EMAIL_IBMMAIL,
	EMAIL_MCIMAIL,
	EMAIL_POWERSHARE,
	EMAIL_PRODIGY,
	EMAIL_TLX,
	EMAIL_X400,
	TOTAL_EMAIL
}VCARD_EMAIL_CONSTANT;

typedef enum
{
	IMAGE_GIF_TYPE=0,
	IMAGE_CGM_TYPE,
	IMAGE_WMF_TYPE,
	IMAGE_BMP_TYPE,
	IMAGE_MET_TYPE,
	IMAGE_PMB_TYPE,
	IMAGE_DIB_TYPE,
	IMAGE_PICT_TYPE,
	IMAGE_TIFF_TYPE,
	IMAGE_PS_TYPE,
	IMAGE_PDF_TYPE,
	IMAGE_JPEG_TYPE,
	IMAGE_MPEG_TYPE,
	IMAGE_MPEG2_TYPE,
	IMAGE_AVI_TYPE,
	IMAGE_QTIME_TYPE,
	TOTAL_IMAGE_TYPE
}VCARD_IMAGE_CONSTANT;

typedef enum
{
	SOUND_WAVE=0,
	SOUND_PCM,
	SOUND_AIFF,
	TOTAL_SOUND
}VCARD_SOUND_CONSTANT;

enum
{
	QUOTED_PRINTABLE_ENCODING_FORMAT,
	BASE_64_ENCODING_FORMAT,
	EIGHT_BIT_ENCODING_FORMAT
};

enum
{
	CONTENT_ID,
	URL
};

typedef enum
{
	KEY_X509=0,
	KEY_PGP,
	TOTAL_KEY
}VCARD_KEY_CONSTANT;


typedef struct
{
	char quotedPrintable;
	char eightBit;
	char base64;
}VCARD_ENCODING_TYPE;

typedef struct
{
	char cid;	// Content-ID
	char url;	// URL
}VCARD_VALUE_LOCATION_TYPE;

typedef struct
{
	VCARD_ENCODING_TYPE			encoding;
	char						*charSet;
	char						*language;
	VCARD_VALUE_LOCATION_TYPE	valueLocation;
}VCARD_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*verNo;
}VCARD_VERSION_PROPERTY_TYPE;

// chenxiang_vcard 20071207
typedef struct 
{
    uint8						* str_ptr;
    uint16 						str_len;
    BOOLEAN 					        is_ucs2;
}
VCARD_STRING_INFO_T;

typedef struct
{
	VCARD_PROPERTY_TYPE	property;
//	char *nameField[5];
    VCARD_STRING_INFO_T nameField[5]; // chenxiang_vcard 20071207
	// familyName;	// 1st field
	// givenName;	// 2nd field
	// middleName;	// 3rd field
	// prefixName;	// 4th field
	// suffixName;	// 5th field
}VCARD_NAME_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*fn;
}VCARD_FORMATTED_NAME_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	VCARD_IMAGE_CONSTANT	formatType;
	char					*photo;
	int						photoLen;
}VCARD_PHOTO_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*birthDate;
}VCARD_BDATE_PROPERTY_TYPE;

typedef struct
{
	char	domType;
	char	intlType;
	char	postalType;
	char	parcelType;
	char	homeType;
	char	workType;
}VCARD_ADR_TYPE;

//请不要修改下面这个结构体中各字段的顺序
//因为解析的时候，按照该顺序做了枚举 wangxiaolin 2009.07.13
typedef struct
{
	char	prefType;
	char	workType;
	char	homeType;
	char	voiceType;
	char	faxType;
	char	msgType;
	char	cellType;
	char	pagerType;
	char	bbsType;
	char	modemType;
	char	carType;
	char	isdnType;
	char	videoType;
}VCARD_TEL_TYPE;

typedef struct
{
	NodeType				node;
	VCARD_PROPERTY_TYPE		property;
	VCARD_ADR_TYPE			adrType;
	char					*adrField[7];
	// first field = Post Office Address
	// second field = Extended Address
	// third field = Street
	// fourth field = Locality
	// fifth field = Region
	// six field = Postal Code
	// seventh field = Country
}VCARD_ADR_PROPERTY_TYPE;

typedef struct
{
	NodeType				node;
	VCARD_PROPERTY_TYPE		property;
	VCARD_ADR_TYPE			labelType;
	char					*label;
}VCARD_LABEL_PROPERTY_TYPE;

typedef struct
{
	NodeType				node;
	VCARD_PROPERTY_TYPE		property;
	VCARD_TEL_TYPE			telType;
	char					*tel;
}VCARD_TEL_PROPERTY_TYPE;

typedef struct
{
	NodeType				node;
	VCARD_PROPERTY_TYPE		property;
	VCARD_EMAIL_CONSTANT	formatType;
	char					*email;
}VCARD_EMAIL_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*mailer;
}VCARD_MAILER_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*timeZone;
}VCARD_TIME_ZONE_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*geographic;
}VCARD_GEOGRAPHIC_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*title;
}VCARD_TITLE_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*role;
}VCARD_ROLE_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	VCARD_IMAGE_CONSTANT	formatType;
	char					*logo;
	int						logoLen;
}VCARD_LOGO_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*org[3];
	// Organization Name - first field
	// Organization Unit - second field
	// Additional positional - third field
}VCARD_ORG_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*note;
}VCARD_NOTE_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*lastRev;
}VCARD_LAST_REV_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	VCARD_SOUND_CONSTANT	formatType;
	char					*sound;
	int						soundLen;
}VCARD_SOUND_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*url;
}VCARD_URL_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	char					*uid;
}VCARD_UID_PROPERTY_TYPE;

typedef struct
{
	VCARD_PROPERTY_TYPE		property;
	VCARD_KEY_CONSTANT		formatType;
	char					*key;
	int						keyLen;
}VCARD_KEY_PROPERTY_TYPE;

typedef struct
{
	VCARD_VERSION_PROPERTY_TYPE			vCardVer;
	VCARD_NAME_PROPERTY_TYPE			vCardName;
	VCARD_FORMATTED_NAME_PROPERTY_TYPE	vCardFn;
	VCARD_PHOTO_PROPERTY_TYPE			vCardPhoto;
	VCARD_BDATE_PROPERTY_TYPE			vCardBDate;
	ListType							vCardAdr;
	ListType							vCardLabel;
	ListType							vCardTel;
	ListType							vCardEmail;
	VCARD_MAILER_PROPERTY_TYPE			vCardMailer;
	VCARD_TIME_ZONE_PROPERTY_TYPE		vCardTimeZone;
	VCARD_GEOGRAPHIC_PROPERTY_TYPE		vCardGeographic;
	VCARD_TITLE_PROPERTY_TYPE			vCardTitle;
	VCARD_ROLE_PROPERTY_TYPE			vCardRole;	// Business Category
	VCARD_LOGO_PROPERTY_TYPE			vCardLogo;
	VCARD_ORG_PROPERTY_TYPE				vCardOrg;
	VCARD_NOTE_PROPERTY_TYPE			vCardNote;
	VCARD_LAST_REV_PROPERTY_TYPE		vCardLastRev;
	VCARD_SOUND_PROPERTY_TYPE			vCardSound;
	VCARD_URL_PROPERTY_TYPE				vCardUrl;
	VCARD_UID_PROPERTY_TYPE				vCardUid;
	VCARD_KEY_PROPERTY_TYPE				vCardKey;
}VCARD_TYPE;

typedef enum
{
    VCARD_NUM_TYPE_MOBILE  = 0,
    VCARD_NUM_TYPE_HOME,
    VCARD_NUM_TYPE_OFFICE,
    VCARD_NUM_TYPE_NUM,
    VCARD_NUM_TYPE_FAX,
    VCARD_NUM_TYPE_MAX
} T_VCARD_NUMBER_TYPE;



#endif  // #ifndef _VCARD_

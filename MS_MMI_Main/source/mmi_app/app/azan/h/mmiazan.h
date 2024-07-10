/****************************************************************************
** File Name:      mmiazan.h                                           *
** Author:         Jesse.Yu                                                *
** Date:           10/01/2011                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 10/01/2011     Jesse.Yu         Create
** 
****************************************************************************/
#ifndef MMIAZAN_H
#define MMIAZAN_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
//#include "cafstdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include <math.h>
#include "dal_time.h"
/*---------------------------------------------------------------------------*
**                          DEPENDANCY                                       *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

typedef	double   _FP64;
#define MMIAZAN_MAX_COUNTRY_COUNT        (82)  // max country count in database
#define MMIAZAN_MAX_CITY_COUNT           (57)  // max city count in database
#define MMIAZAN_MAX_STR_LEN              (128) // max string length
#define MMIAZAN_CONTAINER_CTR_COUNT      (4)   //Control count in container
#define MMIAZAN_OPERATION_COUNT          (5)
#define MMIAZAN_STATUS_COUNT             (2)   //On,Off
#define MMIAZAN_DB_HEADER_LEN	         (264) 
#define MMIAZAN_HOUR_MIN_LEN             (12)   // format as "18 : 30"
#define MMIAZAN_DIRECTION_MAX_LEN        (3)

#define MMIAZAN_COUNTRY_CITY_STR_LEN     (20)  // max country len:20, max city len:19 in cureent database, 
                                               // do care this macro when updating database.

#define MMIAZAN_MAX_CALCULATION_METHOD   (5)   // max calculation method
#define	HStartYear	(1420)
#define HEndYear	(1450)
#define MMIAZAN_MAX_RING_COUNT           (14)
#define MMIAZAN_REPEAT_TIMEOUT           (60*1000)  // Prayer repeat timeout(ms)
#define MMIAZAN_PLAY_NEXT_RING_TIMEOUT   (200)      // Play next ring timeout(ms)

#define FABS(a)						((a) > 0 ? (a) : -(a))
#define FMOD(a, b)					((a) - floor((a) / (b)) * (b))
#define DEG2RAD(degree)				((_FP64)((_FP64)(degree) * (RPD)))
#define DM2DEG(degree, minute)		((_FP64)((_FP64)(degree) + (_FP64)(minute) / 60.0))
#define DMS2DEG(degree, min, sec)	((_FP64)((_FP64)(degree) + (_FP64)(min) / 60.0 + (_FP64)(sec) / 3600.0))
#define HMS2H(hour, min, sec)		((_FP64)((_FP64)(hour) + (_FP64)(min) / 60.0 + (_FP64)(sec) / 3600.0))
#define IS_LEAP_YEAR(year)			(!((0 != year % 4) || ((0 == year % 100) && (0 != year % 400))))

#define PI				3.1415926535897932
#define EARTH_RADIUS	6378140.0
#define DPR				(57.29577951308230876799)	/* degree per radian (180/pi) */
#define RPD				(0.01745329251994329577)	/* radians per degree (pi/180) */
#define HPR				(3.81971863420548805845)	/* hours per radian (12/pi) */
#define DMLONG			(DEG2RAD(36000.7698231 / 36525.0))
#define DPERIGEE		(DEG2RAD(1.7195269 / 36525.0))
#define DELSID			((HMS2H(2400, 3, 4.812866)) / 36525.0)
#define RPD_EXT		    (0.01745329251994329577)

typedef enum
{
	MMIAZAN_SUCCESS = 0,
	MMIAZAN_FAILED  = 1
}AZAN_RESULT_E;

typedef enum
{
    AZAN_CMP_EQUAL =0,
    AZAN_CMP_FIRST_SMALL,
    AZAN_CMP_SECOND_SMALL
}AZAN_ALM_CMP_E;

typedef enum
{
	AZAN_ALARM_Fajr = 0,
	AZAN_ALARM_Sunrise,
	AZAN_ALARM_Dhuhr,
	AZAN_ALARM_Asr,
	AZAN_ALARM_Maghrib,
	AZAN_ALARM_Isha,
	AZAN_ALARM_MAX
}AZAN_ALARM_Type;

typedef struct tagGeoType
{
	uint8	latd;         //纬度
	uint8	latm;           //
	uint8	latIsN;         //
	uint8	longd;        //经度
	uint8	longm;          //
	uint8	longIsW;        //
	uint8	timeZoneH;    //时区
	uint8	timeZoneM;      //
	uint8	timeZoneIsE;    //
	int8	temperature;  //
	int16	seaLevel;       //
	uint16	atmosphericPressure; // 大气压
}_GeoType;

typedef struct 
{
	uint16 wstr_len;
	uint16 wstr_buf[MMIAZAN_MAX_STR_LEN+1];	
}AzanStrInfo;

typedef struct tagTimeType
{
	uint8	hour;
	uint8	min;
	uint8	second;
}_TimeType;

typedef struct tagDateType
{
	uint16	year;
	uint8	month;
	uint8	day;
}_DateType;

typedef	enum tagMainMethodType
{
	MUSLIM_WORLD_LEAGUE = 0,
	EGYPTIAN_GENERAL_AUTHORITY_OF_SURVEY,
	UNIVERSITY_OF_ISLAMIC_SCIENCES_KARACHI,
	UMM_ALQURA_UNIVERSITY,
	ISLAMIC_SOCIETY_OF_NORTH_AMERICA,
	USER,// reserved
	MAX_MAIN_METHODS
}_MainMethodType;

typedef	enum tagSubMethodType
{
	SHAFI = 0,
	HANAFI,
	MAX_SUB_METHODS
}_SubMethodType;

typedef	enum tagUserMethodType
{
	ANGLE = 0,
	TIME,
	MAX_USER_METHODS
}_UserMethodType;

typedef struct tagUserMethodAngleType
{
	uint8	angled;
	uint8	anglem;
}_UserMethodAngleType;

typedef struct tagUserMethodValueType
{
	_UserMethodType	        fajrMethod;
	_UserMethodAngleType	fajrAngle;
	uint8					fajrTimeMin;
	_UserMethodType	        ishaMethod;
	_UserMethodAngleType	ishaAngle;
	uint8					ishaTimeMin;
}_UserMethodValueType;

typedef struct tagMethodType
{
	_MainMethodType	mainMethod;
	_SubMethodType	subMethod;
	void			*pExtData;
}_MethodType;

typedef struct tagSalaTimeType
{
	uint16		nCountry;
	uint16		nCity;
	uint8		isDST;
	_DateType	date;
}_SalaTimeType;

typedef struct
{
    uint8	  countryName[MMIAZAN_COUNTRY_CITY_STR_LEN+1];
}SET_COUNTRY;

typedef struct
{
    uint8	 cityName[MMIAZAN_COUNTRY_CITY_STR_LEN+1];
}SET_CITY;


PUBLIC void MMIAZAN_InitDB(const uint8 *pDB);
PUBLIC const uint8 *MMIAZAN_GetCharSet(void);
PUBLIC uint16 MMIAZAN_GetCountrys(void);
PUBLIC uint16 MMIAZAN_GetCitys(uint16 nCountry);
PUBLIC uint16 MMIAZAN_GetCountryNameMaxLength(void);
PUBLIC uint16 MMIAZAN_GetCityNameMaxLength(void);
PUBLIC uint32 MMIAZAN_GetCountryName(uint16 nCountry, uint8 *pOut, uint16 bSize);
PUBLIC uint32 MMIAZAN_GetCityName(uint16 nCountry, uint16 nCity, uint8 *pOut, uint16 bSize);
PUBLIC uint32 MMIAZAN_GetCityGeo(uint16 nCountry, uint16 nCity, _GeoType *pGeo);
PUBLIC _FP64 MMIAZAN_QiblaDirection(const _GeoType *pGeo);
PUBLIC uint32 MMIAZAN_SetMethod(_MethodType *pMethod);
PUBLIC uint32 MMIAZAN_GetMethod(_MethodType *pMethod);
PUBLIC uint32 MMIAZAN_DSTime(_TimeType pPrayerTimes[AZAN_ALARM_MAX]);
PUBLIC uint32 MMIAZAN_SalatTime(const _GeoType *pGeo, const _DateType *pDate, const _MethodType *pMethod, _TimeType pPrayerTimes[6]);
PUBLIC uint32 MMIAZAN_CaclPrayerTimes(const _SalaTimeType *pSalaTime, _TimeType pPrayerTimes[AZAN_ALARM_MAX]);
PUBLIC uint8 MMIAZAN_HijriDate(_DateType *pGDate, _DateType *pHDate);

// Get extra data
PUBLIC void *MMIAZAN_GetExtData(_MainMethodType eCalMethod);
// Free extra memory
PUBLIC void MMIAZAN_FreeExtraMem(void);

#endif  /* MMIAZAN_H */



/****************************************************************************
** File Name:      mmiazan.c                                               *
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
#ifndef MMIAZAN_C
#define MMIAZAN_C

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "mmiazan.h"
	
/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL const uint8	   *g_pDataBase;

LOCAL _FP64   g_latitude;  // Î³¶È
LOCAL _FP64   g_longitude; // ¾­¶È

LOCAL _MethodType	    g_Method;

LOCAL const uint16	    ndmnthcum[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
LOCAL const _FP64		dmlong = ((36000.7698231 / 36525.0) * RPD);
LOCAL const _FP64		dperigee = ((1.7195269 / 36525.0) * RPD);
LOCAL const _FP64		delsid = DELSID;

LOCAL _FP64	timeZone;
LOCAL _FP64	latitude, longitude;
LOCAL int16	seaLevel;
LOCAL uint8	fajrByInterval, asrShadowRatio;
LOCAL _FP64	fajrInterval, fajrDepr;
LOCAL _FP64	cosobl, sinobl, perigee0, mlong0, sidtm0, c1, c2;
LOCAL _DateType  *g_pExtraDate = NULL;       // extra data date pointer
LOCAL _UserMethodValueType  *g_pExtraUserMethod = NULL;// extra data user method pointer
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/* makes it circular m[0]=m[12] & m[13]=m[1] */
const uint8 gmonth[14]  = {31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31};
const uint16 monthMap[] =
{
	19410, 19396, 19337, 19093, 13613, 13741, 15210, 18132, 19913, 19858,
	19110, 18774, 12974, 13677, 13162, 15189, 19114, 14669, 13469, 14685,
	12986, 13749, 17834, 15701, 19098, 14638, 12910, 13661, 15066, 18132,18085
};

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
LOCAL _FP64 MMIAZAN_Qibla(void);
LOCAL void	 MMIAZAN_ComputeConstants(uint16 year);
LOCAL _FP64 MMIAZAN_NoonTime(uint16 nday, _FP64* colatn);
LOCAL _FP64 MMIAZAN_Tempus(uint16 nday, _FP64 colat, _FP64 time0, uint8 am);
LOCAL void	MMIAZAN_ComputeHours(uint16 year, uint16 first, /*uint16 last,*/ _FP64 [AZAN_ALARM_MAX]);
LOCAL void	MMIAZAN_DaySchedule(uint16 year, uint8 month, uint8 day, _TimeType pPrayerTimes[AZAN_ALARM_MAX]);

/*******************************************************************************/
/*  Name: mod																   */
/*  Purpose: The mod operation for doubles  x mod y							   */
/*******************************************************************************/
LOCAL int32 mod(_FP64 x, _FP64 y);

LOCAL void GDateAjust(uint16 *yg, uint8 *mg, uint8 *dg);

/*******************************************************************************/
/* Name:    GCalendarToJD													   */
/* Type:    Function                                                           */
/* Purpose: convert Gdate(year,month,day) to Julian Day            			   */
/* Arguments:                                                                  */
/* Input : Gregorian date: year:yy, month:mm, day:dd                           */
/* Output:  The Julian Day: JD                                                 */
/*******************************************************************************/
/* it does not take care of 1582correction assumes correct calender from the past */
LOCAL _FP64 GCalendarToJD(uint16 yy, uint8 mm, _FP64 dd);

/*******************************************************************************/
/* Name:    JDToHCalendar													   */
/* Type:    Procedure                                                          */
/* Purpose: convert Julian Day to estimated Hdate(year,month,day)			   */
/* Arguments:                                                                  */
/* Input:  The Julian Day: JD                                                  */
/* Output : Hijrah date: year:yh, month:mh, day:dh                             */
/*******************************************************************************/
LOCAL void JDToHCalendar(_FP64 JD,uint16 *yh, uint8 *mh, uint8 *dh);

/*******************************************************************************/
/* Name:    HCalendarToJD													   */
/* Type:    Function                                                           */
/* Purpose: convert Hdate(year,month,day) to estimated Julian Day     	       */
/* Arguments:                                                                  */
/* Input : Hijrah  date: year:yh, month:mh, day:dh                             */
/* Output:  The Estimated Julian Day: JD                                       */
/*******************************************************************************/
LOCAL uint32 HCalendarToJD(uint16 yh, uint8 mh, uint8 dh);

/*******************************************************************************/
/* Name:    HCalendarToJDA													   */
/* Type:    Function                                                           */
/* Purpose: convert Hdate(year,month,day) to Exact Julian Day     			   */
/* Arguments:                                                                  */
/* Input : Hijrah  date: year:yh, month:mh, day:dh                             */
/* Output:  The Exact Julian Day: JD                                           */
/*******************************************************************************/
LOCAL uint32 HCalendarToJDA(uint16 yh, uint8 mh, uint8 dh);

/*******************************************************************************/
/* Name:    HMonthLength													   */
/* Type:    Function                                                           */
/* Purpose: Obtains the month length            		     				   */
/* Arguments:                                                                  */
/* Input : Hijrah  date: year:yh, month:mh                                     */
/* Output:  Month Length                                                       */
/*******************************************************************************/
LOCAL uint8 HMonthLength(uint16 yh, uint8 mh);

/*******************************************************************************/
/* Name:    JDToGCalendar													   */
/* Type:    Procedure                                                          */
/* Purpose: convert Julian Day  to Gdate(year,month,day)                       */
/* Arguments:                                                                  */
/* Input:  The Julian Day: JD                                                  */
/* Output: Gregorian date: year:yy, month:mm, day:dd                           */
/*******************************************************************************/
LOCAL void JDToGCalendar(uint32 JD, uint16 *yy, uint8 *mm, uint8 *dd);

/*******************************************************************************/
/* Name:    BH2GA                                                              */
/* Type:    Procedure                                                          */
/* Purpose: Finds Gdate(year,month,day) for Hdate(year,month,day=1)  	       */
/* Arguments:                                                                  */
/* Input: Hijrah  date: year:yh, month:mh                                      */
/* Output: Gregorian date: year:yg, month:mg, day:dg , day of week:wDay     */
/*       and returns flag found:1 not found:0                                  */
/*******************************************************************************/
LOCAL uint8  BH2GA(uint16 yh, uint8 mh, uint16 *yg, uint8 *mg, uint8 *dg, uint8 *wDay);

/*******************************************************************************/
/* Name:    MMIAZAN_G2HA                                                               */
/* Type:    Procedure                                                          */
/* Purpose: convert Gdate(year,month,day) to Hdate(year,month,day)             */
/* Arguments:                                                                  */
/* Input: Gregorian date: year:yg, month:mg, day:dg                            */
/* Output: Hijrah  date: year:yh, month:mh, day:dh, day of week:wDay        */
/*       and returns flag found:1 not found:0                                  */
/*******************************************************************************/
LOCAL uint8  MMIAZAN_G2HA(uint16 yg, uint8 mg, uint8 dg, uint16 *yh, uint8 *mh, uint8 *dh, uint8 *wDay);



PUBLIC void MMIAZAN_InitDB(const uint8 *pDB)
{
	SCI_ASSERT(NULL != pDB);	/*assert verified*/
	g_pDataBase = pDB;
}

PUBLIC const uint8* MMIAZAN_GetCharSet(void)
{
	if (NULL != g_pDataBase)
	{
		return	g_pDataBase + 4 + 2 + 2;
	}
	
	return	NULL;
}

PUBLIC uint16 MMIAZAN_GetCountrys(void)
{
	if (NULL != g_pDataBase)
	{
		const uint8	*p;
		uint32			ofst;
		
		p = g_pDataBase + MMIAZAN_DB_HEADER_LEN + 4;
		ofst = (uint32)*p++;
		ofst += (uint32)(*p++) << 8;
		ofst += (uint32)(*p++) << 16;
		ofst += (uint32)(*p) << 24;
		return	((0xffffffff - ofst - MMIAZAN_DB_HEADER_LEN) >> 4);
	}
	
	return 0;
}

PUBLIC uint16 MMIAZAN_GetCitys(uint16 nCountry)
{
	if ((NULL != g_pDataBase) && (nCountry < MMIAZAN_GetCountrys()))
	{
		const uint8	*p;
		uint32			ofst1, ofst2;
		
		p = g_pDataBase + MMIAZAN_DB_HEADER_LEN + (nCountry << 4) + 4;
		ofst1 = *p++;
		ofst1 += (uint32)(*p++) << 8;
		ofst1 += (uint32)(*p++) << 16;
		ofst1 += (uint32)(*p++) << 24;

		p += 4;
		ofst2 = *p++;
		ofst2 += (uint32)(*p++) << 8;
		ofst2 += (uint32)(*p++) << 16;
		ofst2 += (uint32)(*p) << 24;
		
		return	(uint16)((ofst1 - ofst2) >> 4);
	}
	
	return 0;
}

PUBLIC uint16 MMIAZAN_GetCountryNameMaxLength(void)
{
	if (NULL != g_pDataBase)
	{
		const uint8	*p;
		uint16			len;
		
		p = g_pDataBase + 4;
		len = *p++;
		len += (uint16)((uint32)(*p++) << 8);
		
		return	0xffff - len;
	}
	
	return 0;
}

PUBLIC uint16 MMIAZAN_GetCityNameMaxLength(void)
{
	if (NULL != g_pDataBase)
	{
		const uint8	*p;
		uint16	len;
		
		p = g_pDataBase + 4 + 2;
		len = *p++;
		len += (uint16)((uint32)(*p++) << 8);
		
		return	0xffff - len;
	}

	return 0;
}

PUBLIC uint32 MMIAZAN_GetCountryName(uint16 nCountry, uint8 *pOut, uint16 bSize)
{
	if ((NULL != g_pDataBase) && (nCountry < MMIAZAN_GetCountrys()))
	{
		const uint8	*p;
		uint16			len;
		uint32			ofst1, ofst2;

		p = g_pDataBase + MMIAZAN_DB_HEADER_LEN + (nCountry << 4);
		ofst1 = *p++;
		ofst1 += (uint32)(*p++) << 8;
		ofst1 += (uint32)(*p++) << 16;
		ofst1 += (uint32)(*p++) << 24;
		
		p += 4;
		ofst2 = *p++;
		ofst2 += (uint32)(*p++) << 8;
		ofst2 += (uint32)(*p++) << 16;
		ofst2 += (uint32)(*p) << 24;

		len = (uint16)(ofst1 - ofst2);
		if (len > bSize)
		{
			len = bSize;
		}

		p = g_pDataBase + (0xffffffff - ofst1);
		while (len--)
		{
			*pOut++ = *p++;
		}
		
		*(pOut ) = '\0';

		return	MMIAZAN_SUCCESS;
	}
	
	return MMIAZAN_FAILED;
}

PUBLIC uint32 MMIAZAN_GetCityName(uint16 nCountry, uint16 nCity, uint8 *pOut, uint16 bSize)
{
	if ((NULL != g_pDataBase) && (nCity < MMIAZAN_GetCitys(nCountry)))
	{
		const uint8	*p;
		uint16			len;
		uint32			ofst1, ofst2;

		p = g_pDataBase + MMIAZAN_DB_HEADER_LEN + (nCountry << 4) + 4;
		ofst1 = *p++;
		ofst1 += (uint32)(*p++) << 8;
		ofst1 += (uint32)(*p++) << 16;
		ofst1 += (uint32)(*p) << 24;
		
		p = g_pDataBase + (0xffffffff - ofst1) + (nCity << 4);
		ofst1 = *p++;
		ofst1 += (uint32)(*p++) << 8;
		ofst1 += (uint32)(*p++) << 16;
		ofst1 += (uint32)(*p++) << 24;

		p += 4;
		ofst2 = *p++;
		ofst2 += (uint32)(*p++) << 8;
		ofst2 += (uint32)(*p++) << 16;
		ofst2 += (uint32)(*p) << 24;

		len = (uint16)(ofst1 - ofst2);
		if (len > bSize)
		{
			len = bSize;
		}

		p = g_pDataBase + (0xffffffff - ofst1);
		while (len--)
		{
			*pOut++ = *p++;
		}
		
		*(pOut - 1) = '\0';

		return	MMIAZAN_SUCCESS;
	}
	
	return	MMIAZAN_FAILED;
}

PUBLIC uint32 MMIAZAN_GetCityGeo(uint16 nCountry, uint16 nCity, _GeoType *pGeo)
{
	if ((NULL != g_pDataBase) && (nCity < MMIAZAN_GetCitys(nCountry)))
	{
		const uint8	*p;
		uint8			tmp;
		uint16			tmp1;
		uint32			ofst;

		p = g_pDataBase + MMIAZAN_DB_HEADER_LEN + (nCountry << 4) + 4;
		ofst = *p++;
		ofst += (uint32)(*p++) << 8;
		ofst += (uint32)(*p++) << 16;
		ofst += (uint32)(*p) << 24;
		
		p = g_pDataBase + (0xffffffff - ofst) + (nCity << 4) + 4;
		pGeo->latd = 0xff - (*p++);
		tmp = 0xff - (*p++);
		pGeo->latm = tmp & 0x7f;
		pGeo->latIsN = tmp >> 7;
		pGeo->longd = 0xff - (*p++);
		tmp = 0xff - (*p++);
		pGeo->longm = tmp & 0x7f;
		pGeo->longIsW = tmp >> 7;
		pGeo->timeZoneH = 0xff - (*p++);
		tmp = 0xff - (*p++);
		pGeo->timeZoneM = tmp & 0x7f;
		pGeo->timeZoneIsE = tmp >> 7;
		pGeo->temperature = 0xff - (*p++);
		tmp1 = (*p++);
		tmp1 += (uint16)((*p++)) << 8;
		pGeo->seaLevel = 0xffff - tmp1;
		tmp1 = (*p++);
		tmp1 += (uint16)((*p++)) << 8;
		pGeo->atmosphericPressure = 0xffff - tmp1;

		return	MMIAZAN_SUCCESS;
	}
	
	return	MMIAZAN_FAILED;
}

PUBLIC _FP64 MMIAZAN_QiblaDirection(const _GeoType *pGeo)
{
	_FP64		drect;
	
	g_latitude = DEG2RAD(DM2DEG(pGeo->latd, pGeo->latm));
	if (!pGeo->latIsN)
	{
		g_latitude = -g_latitude;
	}

	g_longitude = DEG2RAD(DM2DEG(pGeo->longd, pGeo->longm));
	if (!pGeo->longIsW)
	{
		g_longitude = -g_longitude;
	}

	drect = MMIAZAN_Qibla() * DPR;

	while (drect < 0.0)
	{
		drect += 360.0;
	}
	while (drect >= 360.0)
	{
		drect -= 360.0;
	}

	return	drect;
}

LOCAL _FP64 MMIAZAN_Qibla(void)
{
    /*  lat0, long0 are Makkah's latitude and longitude in radians */
    _FP64	dflong;
    
    dflong = g_longitude - (-0.69504828);
	/*lint -e718 -e746 -e790 -e747 -e628*/
    return	atan2(sin(dflong), cos(g_latitude) * tan(0.3739077) - sin(g_latitude) * cos(dflong));/*lint -esym(628,cos) -esym(628,sin)*/
	/*lint +e718 +e746 +e790 +e747 +e628*/
}


/*******************************************************************************/
LOCAL void MMIAZAN_ComputeConstants(uint16 year)
{
/*  t = Time from 12 hr(noon), Jan 1, 2000 to 0 hr, Jan 0 of year */
/*           measured in julian centuries (units of 36525 days) */
/*  obl = obliquity of ecliptic */
/*  eccy = earth's eccentricity */
/*  dmlong, dperigee, delsid = daily motion (change) in */
/*             sun's mean longitude, longitude of sun's perigee, sidereal time */
/*  mlong0, perigee0, sidtm0 = Values (at 0h, Jan 0 of year) of: */
/*       sun's mean longitude, longitude of sun's perigee, sidereal time */
/*  c1, c2 = coefficients in equation of center */
	_FP64	t = 0.0;
    _FP64	obl, eccy;
    
    t = (((year - 1) / 4 + (year - 1) / 400 - (year - 1) / 100) + 365.0 * year - 730485.5) / 36525.0;/*lint !e653*/
    obl = DEG2RAD(DMS2DEG(23, 26, 21.448) - DMS2DEG(0, 0, 46.815) * t);
    cosobl = cos(obl);
    sinobl = sin(obl);
    eccy = 0.016708617 - 4.2037e-5 * t - 1.236e-7 * t * t;
	
	/*lint -e718 -e746 -e747 -e628 -e666*/
    mlong0 = DEG2RAD(FMOD(280.466449 - 3.0368e-4 * t * t + FMOD(36000.7698231 * t, 360.0), 360.0));/*lint -esym(628,atan) -esym(628,floor)*/
	/*lint +e718 +e746 +e747 +e628 +e666*/
	
	perigee0 = DEG2RAD(FMOD(282.937348 + 1.7195269 * t + 4.5962e-4 * t * t, 360.0));
    sidtm0 = FMOD(HMS2H(6, 41, 50.54841) + FMOD(HMS2H(2400, 3, 4.812866) * t, 24.0), 24.0);/*lint !e666*/
    c1 = eccy * (2 - eccy * eccy / 4);
    c2 = 5 * eccy * eccy / 4;
}


LOCAL _FP64 MMIAZAN_NoonTime(uint16 nday, _FP64 *colatn)
{
/*  slong, mlong =  sun's true, mean longitude at noon */
/*  perigee = longitude of sun's perigee */
/*  ra = sun's right ascension, decl = sun's declination */
/*  ha = sun's hour angle west */
/*  locmt = local mean time of phenomenon */

    _FP64	t;
    _FP64	longh, days, mlong, perigee, anomaly, slong, sinslong, ra, decl, locmt;

    longh = longitude * HPR;
    days = nday + (12.0 + longh) / 24.0;
    mlong = mlong0 + dmlong * days;
    perigee = perigee0 + dperigee * days;
    anomaly = mlong - perigee;
    slong = mlong + c1 * sin(anomaly) + c2 * sin(anomaly * 2);
    sinslong = sin(slong);
    ra = atan2(cosobl * sinslong, cos(slong)) * HPR;/*lint !e747*/
    if (ra < 0.0)
	{
		ra += 24.0;
	}

    decl = asin(sinobl * sinslong);
    locmt = ra - delsid * days - sidtm0;
    t = locmt + longh + timeZone;
    if (t < 0.0)
	{
		t += 24.0;
	}
    else if (t > 24.0)
	{
		t -= 24.0;
	}

    *colatn = FABS(latitude - decl);

    return	t;
}


LOCAL _FP64 MMIAZAN_Tempus(uint16 nday, _FP64 colat, _FP64 time0, uint8 am)
{
/*  slong, mlong =  sun's true, mlong longitude */
/*  perigee = longitude of sun's perigee */
/*  ra = sun's right ascension, sindcl = sin(sun's declination) */
/*  ha = sun's hour angle west */
/*  locmt = local mean time of phenomenon */

    _FP64	t;
	_FP64	longh, days, mlong, perigee, anomaly, slong, sinslong, ra, sindcl, cosha, ha, locmt;
    _FP64	seaLevelCorrect;

    longh = longitude * HPR;
    days = nday + (time0 + longh) / 24.0;
    mlong = mlong0 + dmlong * days;
    perigee = perigee0 + dperigee * days;
    anomaly = mlong - perigee;
    slong = mlong + c1 * sin(anomaly) + c2 * sin(anomaly * 2);/*lint !e747*/
    sinslong = sin(slong);
    ra = atan2(cosobl * sinslong, cos(slong)) * HPR;/*lint !e747*/
    if (ra < 0.0)
	{
		ra += 24.0;
	}

	sindcl = sinobl * sinslong;
    cosha = (cos(colat) - sindcl * sin(latitude)) / (sqrt(1.0 - sindcl * sindcl) * cos(latitude));
	
	/*  if cos(ha)>1, then time cannot be evaluated */
    if (FABS(cosha) > 1.0)
	{
		return	(1.0e7);
	}

	if (DEG2RAD(90.83333) != colat)/*lint !e777*/
	{
		seaLevelCorrect = 0;
	}
	else
	{
		seaLevelCorrect = acos(EARTH_RADIUS / (EARTH_RADIUS + seaLevel));
		seaLevelCorrect = cos(colat - seaLevelCorrect);
		seaLevelCorrect = seaLevelCorrect - sindcl * sin(latitude) / (sqrt(1.0 - sindcl * sindcl) * cos(latitude));
		if (FABS(seaLevelCorrect)> 1.0)
		{
			seaLevelCorrect = 0;
		}
		else
		{
			seaLevelCorrect = (acos(cosha) - acos(seaLevelCorrect)) * HPR;
		}
	}

    ha = acos(cosha) * HPR + seaLevelCorrect;
    if (am)
	{
		ha = 24.0 - ha;
	}

	/*if (am)
	{
		ha = 24.0 - ha;
	}*/

    locmt = ha + ra - delsid * days - sidtm0;
    t = locmt + longh + timeZone;
    if (t < 0.0)
	{
		t += 24.0;
	}
    else if (t > 24.0)
	{
		t -= 24.0;
	}

    return	t;
}

/* Computes times for range of days first..last-1. */
LOCAL void MMIAZAN_ComputeHours(uint16 year, uint16 first, /*uint16 last,*/ _FP64 tim[AZAN_ALARM_MAX])
{
    _FP64	colatn, time0[5], colat[2];
    _FP64	t;
    uint16	last;
    
	/* Approximate times of fajr, shuruq, asr, maghrib, isha */
    time0[0] = 4.0;
    time0[1] = 6.0;
    time0[3 - 1] = 15.0;
    time0[4 - 1] = 18.0;
    time0[5 - 1] = 20.0;
	
    /* Colatitudes of sun at fajr, shuruq, maghrib, isha */
    colat[0] = DEG2RAD((_FP64)(90.0 + fajrDepr));
    
    /* Get approximate times for the first day specified. */
    /* Later on, each day's times used as approximate times for next day */
	/* Adjust approximate times for high altitudes */
    MMIAZAN_NoonTime(first, &colatn);
    colat[3 - 2] = atan((_FP64)asrShadowRatio + tan(colatn));/*lint !e746 !e718*/
	
    time0[1] = (((t = MMIAZAN_Tempus(first, DEG2RAD(90.83333), time0[1], 1)) < 24.0) ? t : 5.0);
    time0[3 - 1] = (((t = MMIAZAN_Tempus(first, colat[3 - 2], time0[3 - 1], 0)) < 24.0) ? t : 15.0);
    time0[4 - 1] = (((t = MMIAZAN_Tempus(first, DEG2RAD(90.83333), time0[4 - 1], 0)) < 24.0) ? t : 21.0);
    if (fajrByInterval)
	{
        time0[0] = time0[1] - fajrInterval / 60.0;
        time0[0] = (time0[0] < time0[1]) ? time0[0] : 1.0;
        time0[5 - 1] = time0[4 - 1] + fajrInterval / 60.0;
        time0[5 - 1] = (time0[5 - 1] > time0[4 - 1]) ? time0[5 - 1] : 23.0;
    }
    else
	{
        time0[0] = (((t = MMIAZAN_Tempus(first, colat[0], time0[0], 1)) < 24.0) ? t : 1.0);
        time0[0] = (time0[0] < time0[1]) ? time0[0] : 1.0;
        time0[5 - 1] = (((t = MMIAZAN_Tempus(first, colat[0], time0[5 - 1], 0)) < 24.0) ? t : 23.0);
        time0[5 - 1] = (time0[5 - 1] > time0[4 - 1]) ? time0[5 - 1] : 23.0;
    }

	/*  compute times for the whole range of days */
    last = first + 1;
    tim[2] = (_FP64)(MMIAZAN_NoonTime(last, &colatn));
    colat[3 - 2] = atan((_FP64)asrShadowRatio + tan(colatn));

	/* Adjust approximate times for high altitudes */
    time0[1] = (_FP64)(((tim[1] = (_FP64)(t = MMIAZAN_Tempus(last, DEG2RAD(90.83333),
    										time0[1], 1))) < 24.0) ? t : 5.0);
    tim[3] = (_FP64)(((t = MMIAZAN_Tempus(last, colat[3 - 2], time0[3 - 1], 0)) < 24.0) ? t : 15.0);
    time0[4 - 1] = (_FP64)(((tim[4] = (_FP64)(t = MMIAZAN_Tempus(last, DEG2RAD(90.83333),
    										time0[4 - 1], 0))) < 24.0) ? t : 21.0);
    if (fajrByInterval)
    {
        tim[0] = (_FP64)(time0[1] - fajrInterval / 60.0);
        tim[0] = (tim[0] < tim[1]) ? tim[0] : 1.0;
        tim[5] = (_FP64)(time0[4 - 1] + fajrInterval / 60.0);
        tim[5] = (tim[5] > tim[4]) ? tim[5] : 23.0;
    }
    else
    {
		tim[0] = (((t = MMIAZAN_Tempus(last, colat[0], time0[0], 1)) < 24.0) ? t : 1.0);
        tim[0] = (tim[0] < tim[1]) ? tim[0] : 1.0;
        tim[5] = (((t = MMIAZAN_Tempus(last, colat[0], time0[5 - 1], 0)) < 24.0) ? t : 23.0);
        tim[5] = (tim[5] > tim[4]) ? tim[5] : 23.0;
    }
}


LOCAL void MMIAZAN_DaySchedule (uint16 year, uint8 month, uint8 day, _TimeType pPrayerTimes[6])
{
	uint8	i, hour, minute;
	uint16	ndays0;
	_FP64   tim[AZAN_ALARM_MAX] = {0.000, 0.000, 0.000, 0.000, 0.000, 0.000};
	
	month--;
	ndays0 = (uint16)(day - 1 + (month > 1 ? IS_LEAP_YEAR(year) : 0) + ndmnthcum[month]);

	MMIAZAN_ComputeHours(year, ndays0, /*ndays0 + 1,*/ tim);

	for (i = 0; i < 6; i++)
	{
		while (tim[i] < 0.0)
		{
			tim[i] += 24.0;
		}
		while (tim[i] >= 24.0)
		{
			tim[i] -= 24.0;
		}
	}

	for (i = 2; i > 0; i--)
	{
		if (tim[i] <= tim[i - 1])
		{
			if (2 == i)
			{
				tim[i - 1] = 5.0;
			}
			else
			{
				if (tim[i] <= 1.0)
				{
					tim[i] = 5.0;
				}
				tim[i - 1] = 1.0;
			}
		}
	}
	for (i = 3; i < 6; i++)
	{
		if (tim[i] <= tim[i - 1])
		{
			if (3 == i)
			{
				tim[i] = 15.0;
			}
			else if (4 == i)
			{
				tim[i] = 21.0;
			}
			else
			{
				tim[i] = 23.0;
			}
		}
	}

	for (i = 0; i < 6; i++)
	{
        hour = (uint8)floor(tim[i]);
        minute = (uint8)(60.0 * (tim[i] - hour));
        if (minute >= 60)
        {
        	minute = 0;
          	hour += 1;
        }

       	pPrayerTimes[i].hour = (uint8)hour;
       	pPrayerTimes[i].min = (uint8)minute;
		pPrayerTimes[i].second = 0;
	}
}

PUBLIC uint32 MMIAZAN_SalatTime(const _GeoType *pGeo, const _DateType *pDate, const _MethodType *pMethod, _TimeType pPrayerTimes[6])
{
	_TimeType	tmpPrayerTimes[6];

	timeZone = (_FP64)pGeo->timeZoneH + (_FP64)((_FP64)pGeo->timeZoneM / (_FP64)60.0);
	if (USER != pMethod->mainMethod)
	{
		if (!pGeo->timeZoneIsE)
		{
			timeZone = -timeZone;
		}
	}
	else
	{
		if (pGeo->longIsW)
		{
			timeZone = -timeZone;
		}
	}
	
	latitude = DEG2RAD(DM2DEG(pGeo->latd, pGeo->latm));
	if (!pGeo->latIsN)
	{
		latitude = -latitude;
	}
	
	seaLevel = pGeo->seaLevel;
	
	longitude = DEG2RAD(DM2DEG(pGeo->longd, pGeo->longm));
	if (!pGeo->longIsW)
	{
		longitude = -longitude;
	}

	MMIAZAN_ComputeConstants(pDate->year);

	fajrByInterval = 0;
	fajrInterval = 90.0;
	if (UNIVERSITY_OF_ISLAMIC_SCIENCES_KARACHI == pMethod->mainMethod)
	{
		fajrDepr = 18.0;
	}
	else
	{
		fajrDepr = 15.0;
	}
	if (SHAFI == pMethod->subMethod)
	{
		asrShadowRatio = 1;
	}
	else
	{
		asrShadowRatio = 2;
	}
	
	MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, pPrayerTimes);

	switch (pMethod->mainMethod)
	{
	case USER:
		{
		_UserMethodValueType	*p;
		
		p = (_UserMethodValueType*)pMethod->pExtData;

		if (ANGLE == p->fajrMethod)
		{
			fajrDepr = p->fajrAngle.angled + (_FP64)p->fajrAngle.anglem / 10.0;
		}
		else
		{
			fajrInterval = p->fajrTimeMin;

			if ((SHAFI == pMethod->subMethod) && (fajrInterval != 90))
			{
				fajrByInterval = 1;
			}
		}

		MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, tmpPrayerTimes);
		pPrayerTimes[0].hour = tmpPrayerTimes[0].hour;
		pPrayerTimes[0].min = tmpPrayerTimes[0].min;
		pPrayerTimes[0].second = tmpPrayerTimes[0].second;

		if (ANGLE == p->ishaMethod)
		{
			fajrDepr = p->ishaAngle.angled + (_FP64)p->ishaAngle.anglem / 10.0;
			fajrByInterval = 0;
			fajrInterval = 90.0;
		}
		else
		{
			fajrInterval = p->ishaTimeMin;
			if (fajrInterval == 90)
			{
				fajrByInterval = 0;
			}
			else
			{
				fajrByInterval = 1;
			}

			fajrDepr = 15.0;
		}

		MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, tmpPrayerTimes);
		pPrayerTimes[5].hour = tmpPrayerTimes[5].hour;
		pPrayerTimes[5].min = tmpPrayerTimes[5].min;
		pPrayerTimes[5].second = tmpPrayerTimes[5].second;
		}
	break;

	case UMM_ALQURA_UNIVERSITY:
		fajrDepr = 19.0;

		MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, tmpPrayerTimes);
		pPrayerTimes[0].hour = tmpPrayerTimes[0].hour;
		pPrayerTimes[0].min = tmpPrayerTimes[0].min;
		pPrayerTimes[0].second = tmpPrayerTimes[0].second;

		fajrDepr = 17.0;
		MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, tmpPrayerTimes);
		pPrayerTimes[5].hour = tmpPrayerTimes[5].hour;
		pPrayerTimes[5].min = tmpPrayerTimes[5].min;
		pPrayerTimes[5].second = tmpPrayerTimes[5].second;
		
		break;
	case MUSLIM_WORLD_LEAGUE:
		fajrDepr = 18.0;
		MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, tmpPrayerTimes);
		pPrayerTimes[0].hour = tmpPrayerTimes[0].hour;
		pPrayerTimes[0].min = tmpPrayerTimes[0].min;
		pPrayerTimes[0].second = tmpPrayerTimes[0].second;

		fajrDepr = 17.0;
		MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, tmpPrayerTimes);
		pPrayerTimes[5].hour = tmpPrayerTimes[5].hour;
		pPrayerTimes[5].min = tmpPrayerTimes[5].min;
		pPrayerTimes[5].second = tmpPrayerTimes[5].second;
		
		break;
	case EGYPTIAN_GENERAL_AUTHORITY_OF_SURVEY:
		fajrDepr = 19.5;	//20	//19.5(standard degree)
		MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, tmpPrayerTimes);
		pPrayerTimes[0].hour = tmpPrayerTimes[0].hour;
		pPrayerTimes[0].min = tmpPrayerTimes[0].min;
		pPrayerTimes[0].second = tmpPrayerTimes[0].second;
		
		fajrDepr = 17.5;	//18	//17.5(standard degree)
		MMIAZAN_DaySchedule(pDate->year, pDate->month, pDate->day, tmpPrayerTimes);
		pPrayerTimes[5].hour = tmpPrayerTimes[5].hour;
		pPrayerTimes[5].min = tmpPrayerTimes[5].min;
		pPrayerTimes[5].second = tmpPrayerTimes[5].second;
	
		break;
	default:
		break;
	}
    return	MMIAZAN_SUCCESS;
}

PUBLIC uint32 MMIAZAN_SetMethod(_MethodType *pMethod)
{
	g_Method.mainMethod = pMethod->mainMethod;
	g_Method.subMethod = pMethod->subMethod;
	g_Method.pExtData = pMethod->pExtData;
	
	return	MMIAZAN_SUCCESS;
}

PUBLIC uint32 MMIAZAN_GetMethod(_MethodType *pMethod)
{
	pMethod->mainMethod = g_Method.mainMethod;
	pMethod->subMethod = g_Method.subMethod;
	pMethod->pExtData = g_Method.pExtData;
	
	return	MMIAZAN_SUCCESS;
}

PUBLIC uint32 MMIAZAN_DSTime(_TimeType pPrayerTimes[6])
{
	pPrayerTimes[0].hour += 1;
	pPrayerTimes[1].hour += 1;
	pPrayerTimes[2].hour += 1;
	pPrayerTimes[3].hour += 1;
	pPrayerTimes[4].hour += 1;
	pPrayerTimes[5].hour += 1;

	return	MMIAZAN_SUCCESS;
}

PUBLIC uint32 MMIAZAN_CaclPrayerTimes(const _SalaTimeType *pSalaTime, _TimeType pPrayerTimes[6])
{
	_GeoType	GEO;
	
	MMIAZAN_GetCityGeo(pSalaTime->nCountry, pSalaTime->nCity, &GEO);
	MMIAZAN_SalatTime(&GEO, &pSalaTime->date, &g_Method, pPrayerTimes);
	
	if (pSalaTime->isDST)
	{
		MMIAZAN_DSTime(pPrayerTimes);
	}
	
	return	MMIAZAN_SUCCESS;
}

/*******************************************************************************/
/*  Name: mod																   */
/*  Purpose: The mod operation for doubles  x mod y							   */
/*******************************************************************************/
LOCAL int32 mod(_FP64 x, _FP64 y)
{
	int32	r;

	r = (int32)(x / y);

	if (r < 0)
	{
		r--;
	}

	return	(int32)(x - y * r);
}

LOCAL void GDateAjust(uint16 *yg, uint8 *mg, uint8 *dg)
{
	int32 dys;

	/* Make sure that dates are within the correct values */
	/*  Underflow  */
	if (*mg < 1)			/* months underflow */
	{
		*mg = 12 + *mg;		/* plus as the underflow months is negative */
		*yg = *yg - 1;
	}

	if (*dg < 1)			/* days underflow */
	{
		*mg = *mg - 1;		/* month becomes the previous month */
		*dg = gmonth[*mg] + *dg;	/* number of days of the month less the underflow days (it is plus as the sign of the day is negative) */
		
		if (*mg < 1)		/* months underflow */
		{
			*mg = 12 + *mg;	/* plus as the underflow months is negative */
			*yg = *yg - 1;
		}
		else if (2 == *mg)
		{
			//*dg = *dg + IS_LEAP_YEAR(*yg);
			if(IS_LEAP_YEAR(*yg))
			{
				*dg = *dg+1;
			}
		}
	}

	/* Overflow  */
	if (*mg > 12)	/* months */
	{
		*mg = *mg - 12;
		*yg = *yg + 1;
	}

	dys = gmonth[*mg];
	if (2 == *mg)
	{
		//dys = dys + IS_LEAP_YEAR(*yg);	/* number of days in the current month */
		if(IS_LEAP_YEAR(*yg))
		{
			dys = dys +1;
		}
	}
	
	if (*dg > dys)	/* days overflow */
	{
		*dg = (uint8)(*dg - dys);
		*mg = *mg + 1;
		
		if (*mg > 12)	/* months */
		{
			*mg = *mg - 12;
			*yg = *yg + 1;
		}
		else if (2 == *mg)
		{
			//dys = gmonth[*mg] + IS_LEAP_YEAR(*yg);	/* number of days in the current month */
			if(IS_LEAP_YEAR(*yg))
			{
				dys = gmonth[*mg] +1;
			}
			else
			{
				dys = gmonth[*mg];
			}
			if (*dg > dys)
			{
				*dg = (uint8)(*dg - dys);
				*mg = *mg + 1;
			}
		}
	}
}

/*******************************************************************************/
/* Name:    GCalendarToJD													   */
/* Type:    Function                                                           */
/* Purpose: convert Gdate(year,month,day) to Julian Day            			   */
/* Arguments:                                                                  */
/* Input : Gregorian date: year:yy, month:mm, day:dd                           */
/* Output:  The Julian Day: JD                                                 */
/*******************************************************************************/
/* it does not take care of 1582correction assumes correct calender from the past */
LOCAL _FP64 GCalendarToJD(uint16 yy, uint8 mm, _FP64 dd)
{
	uint8	century;
	
	if (mm <= 2)
	{
		yy = yy - 1;
		mm = mm + 12;
	}

	century = yy / 100;

	return	(_FP64)((uint32)(365.25 * (_FP64)(yy + 4716)) + (uint32)(30.6001 * (_FP64)(mm + 1))
			+ (_FP64)dd + (_FP64)(2 + century / 4 - century) - 1524.5);/*lint !e653*/
}


/*******************************************************************************/
/* Name:    JDToHCalendar													   */
/* Type:    Procedure                                                          */
/* Purpose: convert Julian Day to estimated Hdate(year,month,day)			   */
/* Arguments:                                                                  */
/* Input:  The Julian Day: JD                                                  */
/* Output : Hijrah date: year:yh, month:mh, day:dh                             */
/*******************************************************************************/
LOCAL void JDToHCalendar(_FP64 JD,uint16 *yh, uint8 *mh, uint8 *dh)
{
	/* Estimating the hijrah date from JD */
	_FP64	md, yd;

	yd = (_FP64)JD - 1948439.0;	/* subtract JD for 18/7/622 first Hijrah date */
	md = mod(yd, 354.367068);
	
	*dh = mod(md + 0.5, 29.530589) + 1;
	*mh = (uint8)((md / 29.530589) + 1);
	*yh = (uint16)((yd - md) / 354.367068 + 1);

	if (*dh > 30)
	{
		*dh = *dh - 30;
		(*mh)++;
	}

	if (*mh > 12)
	{
		*mh = *mh - 12;
		(*yh)++;
	}
}


/*******************************************************************************/
/* Name:    HCalendarToJD													   */
/* Type:    Function                                                           */
/* Purpose: convert Hdate(year,month,day) to estimated Julian Day     	       */
/* Arguments:                                                                  */
/* Input : Hijrah  date: year:yh, month:mh, day:dh                             */
/* Output:  The Estimated Julian Day: JD                                       */
/*******************************************************************************/
LOCAL uint32 HCalendarToJD(uint16 yh, uint8 mh, uint8 dh)
{
	/* Estimating The JD for hijrah dates
	   this is an approximate JD for the given hijrah date */

	/*  add JD for 18/7/622 first Hijrah date 1948439.0 */
	return	(uint32)(((_FP64)yh - 1.0) * 354.367068 + ((_FP64)mh - 1.0) * 29.530589
			+ (_FP64)dh - 1.0 + 1948439.0);
}

/*******************************************************************************/
/* Name:    HCalendarToJDA													   */
/* Type:    Function                                                           */
/* Purpose: convert Hdate(year,month,day) to Exact Julian Day     			   */
/* Arguments:                                                                  */
/* Input : Hijrah  date: year:yh, month:mh, day:dh                             */
/* Output:  The Exact Julian Day: JD                                           */
/*******************************************************************************/
LOCAL uint32 HCalendarToJDA(uint16 yh, uint8 mh, uint8 dh)
{
	uint8	m;
	uint16	dy, b;
	uint32	GJD;

	/* correct the JD value from stored tables */
	GJD = HCalendarToJD(yh, 1, 1) + (monthMap[yh - HStartYear] >> 12) - 3;
	b = monthMap[yh - HStartYear];

	for (m = 1; m < mh; m++)
	{
		if (b & 0x01)
		{
			dy = 30;
		}
		else
		{
			dy = 29;
		}

		GJD = GJD + dy;		/* Add the months lengths before mh */
		b = b >> 1;
	}

	return	GJD + dh - 1;
}


/*******************************************************************************/
/* Name:    HMonthLength													   */
/* Type:    Function                                                           */
/* Purpose: Obtains the month length            		     				   */
/* Arguments:                                                                  */
/* Input : Hijrah  date: year:yh, month:mh                                     */
/* Output:  Month Length                                                       */
/*******************************************************************************/
LOCAL uint8 HMonthLength(uint16 yh, uint8 mh)
{
	uint8	dy;

	if ((yh < HStartYear) || (yh > HEndYear))
	{
		dy = 0;
	}
	else
	{
		if ((monthMap[yh - HStartYear] >> (mh - 1)) & 0x01)
		{
			dy = 30;
		}
		else
		{
			dy = 29;
		}
	}

	return	(uint8)dy;
}

/*******************************************************************************/
/* Name:    JDToGCalendar													   */
/* Type:    Procedure                                                          */
/* Purpose: convert Julian Day  to Gdate(year,month,day)                       */
/* Arguments:                                                                  */
/* Input:  The Julian Day: JD                                                  */
/* Output: Gregorian date: year:yy, month:mm, day:dd                           */
/*******************************************************************************/
LOCAL void JDToGCalendar(uint32 JD, uint16 *yy, uint8 *mm, uint8 *dd)
{
	_FP64	F;
	uint32	alpha, A, B, C, D, E, Z;

	Z = (uint32)floor((_FP64)JD + 0.5);
	F = ((_FP64)JD + 0.5) - (_FP64)Z;
	alpha = (uint32)(((_FP64)Z - 1867216.25) / 36524.25);
	A = Z + 1 + alpha - alpha / 4;
	B = A + 1524;
	C = (uint32) (((_FP64)B - 122.1) / 365.25);
	D = (uint32) (365.25 * (_FP64)C);
	E = (uint32) ((_FP64)(B - D) / 30.6001);

	*dd = (uint8)((B - D - floor(30.6001 * (_FP64)E)) + F);/*lint !e737*/
	
	if (E < 14)
	{
		*mm = (uint8)(E - 1);
	}
	else
	{
		*mm = (uint8)(E - 13);
	}
	
	if (*mm > 2)
	{
		*yy = (uint16)(C - 4716);
	}
	else
	{
		*yy = (uint16)(C - 4715);
	}
}

/*******************************************************************************/
/* Name:    BH2GA                                                              */
/* Type:    Procedure                                                          */
/* Purpose: Finds Gdate(year,month,day) for Hdate(year,month,day=1)  	       */
/* Arguments:                                                                  */
/* Input: Hijrah  date: year:yh, month:mh                                      */
/* Output: Gregorian date: year:yg, month:mg, day:dg , day of week:wDay     */
/*       and returns flag found:1 not found:0                                  */
/*******************************************************************************/
LOCAL uint8  BH2GA(uint16 yh, uint8 mh, uint16 *yg, uint8 *mg, uint8 *dg, uint8 *wDay)
{
	uint32	GJD;

	/* Make sure that the date is within the range of the tables */
	if (mh < 1)
	{
		mh = 12;
	}
	else if (mh > 12)
	{
		mh = 1;
	}

	if (yh < HStartYear)
	{
		yh = HStartYear;
	}
	else if (yh > HEndYear)
	{
		yh = HEndYear;
	}

	GJD = HCalendarToJDA(yh, mh, 1);
	JDToGCalendar(GJD, yg, mg, dg);
	*wDay = (uint8)((GJD + 1) % 7);

	return	1;
}


/*******************************************************************************/
/* Name:    MMIAZAN_G2HA                                                               */
/* Type:    Procedure                                                          */
/* Purpose: convert Gdate(year,month,day) to Hdate(year,month,day)             */
/* Arguments:                                                                  */
/* Input: Gregorian date: year:yg, month:mg, day:dg                            */
/* Output: Hijrah  date: year:yh, month:mh, day:dh, day of week:wDay        */
/*       and returns flag found:1 not found:0                                  */
/*******************************************************************************/
LOCAL uint8  MMIAZAN_G2HA(uint16 yg, uint8 mg, uint8 dg, uint16 *yh, uint8 *mh, uint8 *dh, uint8 *wDay)
{
	uint16	yh1, yh2;
	uint8	mh1, mh2, dh1, dw2;
	uint16	yg1;
	uint8	mg1, dg1;
	int32	df;
	uint32	HJD;
	_FP64	GJD;

	GJD = GCalendarToJD(yg, mg, dg + 0.5);		/* find JD of Gdate */
	JDToHCalendar(GJD, &yh1, &mh1, &dh1);	/* estimate the Hdate that correspond to the Gdate */
	HJD = HCalendarToJDA(yh1, mh1, dh1);	// get the exact Julian Day

	*wDay = (uint8)((uint32)(GJD + 2) % 7);
	df = (int32)(GJD - HJD) + dh1;
	
	while (df > 30)
	{
		df = df - HMonthLength(yh1, mh1);
		mh1++;
		if (mh1 > 12)
		{
			yh1++;
			mh1 = 1;
		}
	}

	dh1 = (uint8)df;
	if (30 == dh1)
	{
		mh2 = mh1 + 1;
		yh2 = yh1;
		if (mh2 > 12)
		{
			mh2 = 1;
			yh2++;
		}

		BH2GA(yh2, mh2, &yg1, &mg1, &dg1, &dw2);
		
		if (dg == dg1)	/* Make sure that the month is 30days if not make adjustment */
		{
			yh1 = yh2;
			mh1 = mh2;
			dh1 = 1;
		}
	}

	*yh = yh1;
	*mh = mh1;
	*dh = dh1;

	return	1;
}

PUBLIC uint8 MMIAZAN_HijriDate(_DateType *pGDate, _DateType *pHDate)
{
	uint8	wDay;

	GDateAjust(&pGDate->year, &pGDate->month, &pGDate->day);
	MMIAZAN_G2HA(pGDate->year, pGDate->month, pGDate->day,
		&pHDate->year, &pHDate->month, &pHDate->day, &wDay);

	return	wDay;
}

// Get extra data
PUBLIC void *MMIAZAN_GetExtData(_MainMethodType eCalMethod)
{
	if (UMM_ALQURA_UNIVERSITY == eCalMethod)
	{
		if (NULL != g_pExtraDate)
		{
			SCI_FREE(g_pExtraDate);
			g_pExtraDate = NULL;
		}
		g_pExtraDate = (_DateType *)SCI_ALLOC_APP(sizeof(_DateType));
		if (NULL != g_pExtraDate)
		{
			SCI_DATE_T sys_date = {0};
			TM_GetSysDate(&sys_date);
			g_pExtraDate->year  = sys_date.year;
			g_pExtraDate->month = sys_date.mon;
			g_pExtraDate->day   = sys_date.mday;

			return (void *)g_pExtraDate;
		}
		else
		{
			return NULL;
		}
	}
	else if (USER == eCalMethod)
	{
		if (NULL != g_pExtraUserMethod)
		{
			SCI_FREE(g_pExtraUserMethod);
			g_pExtraUserMethod = NULL;
		}
		g_pExtraUserMethod = (_UserMethodValueType *)SCI_ALLOC_APP(sizeof(_UserMethodValueType));
		if (NULL != g_pExtraUserMethod)
		{
			// Fill content...
			return (void *)g_pExtraUserMethod;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

// Free extra memory
PUBLIC void MMIAZAN_FreeExtraMem(void)
{
    if (NULL != g_pExtraDate)
	{
		SCI_FREE(g_pExtraDate);
		g_pExtraDate = NULL;
	}
	if (NULL != g_pExtraUserMethod)
	{
		SCI_FREE(g_pExtraUserMethod);
		g_pExtraUserMethod = NULL;
	}
}

#endif/*MMIAZAN_C*/


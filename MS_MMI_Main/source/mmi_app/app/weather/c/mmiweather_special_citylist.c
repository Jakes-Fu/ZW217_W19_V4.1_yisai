/******************************************************************************
** File Name:      mmiweather_special_citylist.c                                               *
** Author:                                                                    *
** Date:           04/2011                                                    *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to add special citylist                       *
*******************************************************************************/
#ifndef _MMIWEATHER_SPECIAL_CITYLIST_H_
#define _MMIWEATHER_SPECIAL_CITYLIST_H_ 

#include "mmiweather.h"

const MMIWEATHER_CITY_TAB_T s_special_city_table[] =
{
    /*某语言城市名Unicode码*/     /*英文名*/              /*中文城市名*/
#ifdef WEATHER_SUPPORT_GOOGLE
    {L"\x6fb3\x95e8",           "Macau"         },     /*lint !e64*/      //澳门/
    {L"\x91cd\x5e86",           "chongqing"     },    //重庆
#elif defined(MMI_WEATHER_SUPPORT_WUNDERGROUND) || defined(MMIWEATHER_SUPPORT_SPRD_SRV)
    {L"\x91cd\x5e86",           "chongqing"     },    //重庆
    {L"\x54c8\x5c14\x6ee8",     "harbin"},//哈尔滨
    {L"\x4e4c\x9c81\x6728\x9f50", "urumqi"},//乌鲁木齐
    {L"\x957f\x6c99",           "zgha"},//长沙
    {L"changsha",               "zgha"},//长沙
    {L"\x62c9\x8428",           "lhasa"},//拉萨
    {L"\x4e1c\x4eac",           "tokyo"},//东京
    {L"lucknow",                 "vilk"},//lucknow
    {L"pondicherry","M.O. Pondicherry"},//Pondicherry
    {L"\x9f50\x9f50\x54c8\x5c14",    "qiqihar"},//齐齐哈尔
    {L"\x9042\x5b81",    "sichuan/suining"},//遂宁
    {L"\x53a6\x95e8",    "xiamen"},//厦门
    {L"\x5fb7\x5dde",    "dezhou"},//德州
    {L"\x5409\x5b89",    "jiangxi/ji'an"},//吉安
    {L"\x6cf0\x5b89",    "tai'an"},//泰安
    {L"\x53f0\x5317",    "taipei"},//台北
    {L"\x4eb3\x5dde",    "bozhou"},//亳州
    {L"\x5409\x6797",    "jilin"},//吉林
    {L"\x6dee\x5b89",    "huai'an"},//淮安
    {L"\x96c5\x5b89",    "ya'an"},//雅安

#endif
};

PUBLIC MMIWEATHER_CITY_TAB_T * MMIWEATHER_GetSpecialCityTable(void)
{
    return s_special_city_table;
}
PUBLIC uint32 MMIWEATHER_GetSpecialCityNum(void)
{
    return ARR_SIZE(s_special_city_table);
}
#endif


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
    /*ĳ���Գ�����Unicode��*/     /*Ӣ����*/              /*���ĳ�����*/
#ifdef WEATHER_SUPPORT_GOOGLE
    {L"\x6fb3\x95e8",           "Macau"         },     /*lint !e64*/      //����/
    {L"\x91cd\x5e86",           "chongqing"     },    //����
#elif defined(MMI_WEATHER_SUPPORT_WUNDERGROUND) || defined(MMIWEATHER_SUPPORT_SPRD_SRV)
    {L"\x91cd\x5e86",           "chongqing"     },    //����
    {L"\x54c8\x5c14\x6ee8",     "harbin"},//������
    {L"\x4e4c\x9c81\x6728\x9f50", "urumqi"},//��³ľ��
    {L"\x957f\x6c99",           "zgha"},//��ɳ
    {L"changsha",               "zgha"},//��ɳ
    {L"\x62c9\x8428",           "lhasa"},//����
    {L"\x4e1c\x4eac",           "tokyo"},//����
    {L"lucknow",                 "vilk"},//lucknow
    {L"pondicherry","M.O. Pondicherry"},//Pondicherry
    {L"\x9f50\x9f50\x54c8\x5c14",    "qiqihar"},//�������
    {L"\x9042\x5b81",    "sichuan/suining"},//����
    {L"\x53a6\x95e8",    "xiamen"},//����
    {L"\x5fb7\x5dde",    "dezhou"},//����
    {L"\x5409\x5b89",    "jiangxi/ji'an"},//����
    {L"\x6cf0\x5b89",    "tai'an"},//̩��
    {L"\x53f0\x5317",    "taipei"},//̨��
    {L"\x4eb3\x5dde",    "bozhou"},//����
    {L"\x5409\x6797",    "jilin"},//����
    {L"\x6dee\x5b89",    "huai'an"},//����
    {L"\x96c5\x5b89",    "ya'an"},//�Ű�

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


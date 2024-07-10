/**
 * Copyright(C),2020 - 2100, UNISOC Technologies Co.,Ltd.
 * FileName:       mmi_adapt_screen.c
 * Author:         Fulu Song
 * Version:  
 * Date:           2021-1-6
 * Description:    lcd resolution auto adaption .
 *
 * History:  
 *--------------------------------------------------------------------------
 * 2020-12-4  create
*/
#include "mmi_adapt_screen.h"

#ifdef ADAPT_SCREEN_ENABLE
PUBLIC int16 MMIAdaptScreen_Dp2Px_Value(int16 dp)
{
    return (int16)(ROUND((float)dp * DP2PX_K));
}

PUBLIC GUI_POINT_T MMIAdaptScreen_Dp2Px_Point(GUI_POINT_T point)
{
    GUI_POINT_T pt = {0};
    pt.x = (int16)(ROUND((float)point.x * DP2PX_K));
    pt.y = (int16)(ROUND((float)point.y * DP2PX_K));
    return pt;
}

PUBLIC GUI_RECT_T  MMIAdaptScreen_Dp2Px_Rect(GUI_RECT_T rect)
{
    GUI_RECT_T r = {0};
    r.left = (int16)(ROUND((float)rect.left * DP2PX_K));
    r.top = (int16)(ROUND((float)rect.top * DP2PX_K));
    r.right = (int16)(ROUND((float)rect.right * DP2PX_K)) + 1;
    r.left = (int16)(ROUND((float)rect.bottom* DP2PX_K)) + 1;

    return r;
}

PUBLIC int16 MMIAdaptScreen_Px2Dp_Value(int16 px)
{
    return (int16)(ROUND((float)px * PX2DP_K));
}

PUBLIC GUI_POINT_T MMIAdaptScreen_Px2Dp_Point(GUI_POINT_T point)
{
    GUI_POINT_T pt = {0};
    pt.x = (int16)(ROUND((float)point.x * PX2DP_K));
    pt.y = (int16)(ROUND((float)point.y * PX2DP_K));
    return pt;
}

PUBLIC GUI_RECT_T  MMIAdaptScreen_Px2Dp_Rect(GUI_RECT_T rect)
{
    GUI_RECT_T r = {0};
    r.left = (int16)(ROUND((float)rect.left * PX2DP_K));
    r.top = (int16)(ROUND((float)rect.top * PX2DP_K));
    r.right = (int16)(ROUND((float)rect.right * PX2DP_K)) + 1;
    r.left = (int16)(ROUND((float)rect.bottom* PX2DP_K)) + 1;

    return r;
}

#else

PUBLIC int16 MMIAdaptScreen_Dp2Px_Value(int16 dp)
{
    return dp;
}

PUBLIC GUI_POINT_T MMIAdaptScreen_Dp2Px_Point(GUI_POINT_T point)
{
    return point;
}

PUBLIC GUI_RECT_T  MMIAdaptScreen_Dp2Px_Rect(GUI_RECT_T rect)
{
    return rect;
}

PUBLIC int16 MMIAdaptScreen_Px2Dp_Value(int16 px)
{
    return px;
}

PUBLIC GUI_POINT_T MMIAdaptScreen_Px2Dp_Point(GUI_POINT_T point)
{
    return point;
}

PUBLIC GUI_RECT_T  MMIAdaptScreen_Px2Dp_Rect(GUI_RECT_T rect)
{
    return rect;
}
#endif

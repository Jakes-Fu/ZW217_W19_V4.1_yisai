
/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "mmk_type.h"
#include "dal_time.h"
#include "mmiacc_event.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef enum
{
	ZTEWEATHER_CLOUD_SUN_TYPE = 0,
	ZTEWEATHER_MORE_CLOUD_TYPE,
	ZTEWEATHER_MORE_RAIN_TYPE,
	ZTEWEATHER_SUN_TYPE,
	ZTEWEATHER_SUN_CLOUD_TYPE,
	ZTEWEATHER_SUN_RAIN_TYPE
}ZTEWEATHER_TEMP_TYPE_E;

typedef struct
{
	MMI_IMAGE_ID_T bg;
	MMI_TEXT_ID_T txt;
}ZTEWEATHER_FORECAST_T;

typedef struct
{
	uint16 city_name[101]; //地址
    uint16 content[101];    //天气描述多云
    uint16 date_str[21];    //日期2020-04-08
    uint16 time_str[21];    //时间11:52:32
    uint16 num; //天气编号1——晴  2——阴   3——雨  4——雪
    int32 cur_degree;       //当前温度
    int32 min_degree;      //最低温
    int32 max_degree;   	//最高温
}ZTEWEATHER_TEMP_INFO_T;

typedef struct
{
	int status;//-1:not init,0:error,1:success,2:请求超时,3:请求天气间隔时间短
	uint32 last_time;
}ZTEWEATHER_NORMAL_INFO_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

PUBLIC void ZTEWEATHER_CreateMainWin();
PUBLIC void ZTEWEATHER_UpdateWin();
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

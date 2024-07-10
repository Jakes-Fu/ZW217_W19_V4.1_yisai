/*****************************************************************************
** File Name:      mmiidle_screensaver_seed.c                                     *
** Author:                                                                   *
** Date:           05/15/2008                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe special screensaver                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/15/2008       bin.ji             Create
******************************************************************************/

#define _MMIIDLE_SCREENSAVER_SEED_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef SCREENSAVER_SUPPORT
#include "window_parse.h"
#include "mmidisplay_data.h"
//#include "mmiss_export.h"
#include "mmiss_special.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmk_timer.h"
#include "mmi_default.h"
#include "mmi_appmsg.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define SEED_COLOR             MMI_YELLOW_COLOR     //seed����ɫ
//#define SEED_INTERVAL_WIDTH    11       //seed֮���϶��
//#define SEED_INTERVAL_HEIGHT   15       //seed֮���϶��
#define SEED_WIDTH             3        //seed��
#define SEED_HEIGHT            3        //seed��
#define SEED_GROUP_NUM         4        //seedȺ����Ŀ��2x2��
#define ENV_WIDTH   9       //������
#define ENV_HEIGHT   9      //������

#define WIDTH_UNIT      ((ENV_WIDTH+2)/8)           //ÿ���ֽ���
#define HEIGHT_UNIT     (ENV_HEIGHT+2)              //����
#define MAP_SIZE        (WIDTH_UNIT*HEIGHT_UNIT)    //seedȺ���ͼ��С

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

//ĳ��seed���ڵ�λ��
typedef struct
{
    uint8 x;    //x����
    uint8 y;    //y����
}SEED_POS_T; 

//������ʾ��ʼseed�ֲ�λ��
typedef struct
{
    SEED_POS_T const *seed_pos_map;
    uint32 seed_count;  //seed��Ŀ
}SEED_POS_MAP_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

 /**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : ��ʼ������
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitSeedScr(void);

/*****************************************************************************/
// 	Description : �ػ���Ļ
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RedrawScr(void);

/*****************************************************************************/
// 	Description : ������ʱ���¼�����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void TriggerSeedScr(void);

/*****************************************************************************/
// 	Description : ����seed
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void AddSeed(
                    uint32 i,
                    uint16 x,
                    uint16 y
                    );

/*****************************************************************************/
// 	Description : �Ƿ����λ��
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAtCorner(
                         uint16 x,
                         uint16 y
                         );

/*****************************************************************************/
// 	Description : �Ƿ��λ��
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAtBorder(
                         uint16 x,
                         uint16 y
                         );

/*****************************************************************************/
// 	Description : ����¼����λ��copy��s_seed_map
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void CopySeedMap(
                        uint32 i
                        );

/*****************************************************************************/
// 	Description : ��ʼ��seed�ķֲ��ͻ���
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitSeedMap(void);

/*****************************************************************************/
// 	Description : �����µ�seed�ֲ�
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void GenNewSeed(void);

/*****************************************************************************/
// 	Description : ��seed�ֲ���ʾ����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawSeedMap(void);

/*****************************************************************************/
// 	Description : seed�Ƿ��Ҵ桱����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSurvival(
                         uint32 i,
                         uint32 x,
                         uint32 y
                         );

/*****************************************************************************/
// 	Description : ĳȺ���ĳλ���Ƿ���seed����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSeedExist(
                          uint32 group_index,
                          uint16 x,
                          uint16 y
                          );

/*****************************************************************************/
// 	Description : �ж�ĳ��λ���Ҵ����������¼��newseedmap��
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void SetNewSeed(
                       uint16 x,
                       uint16 y,
                       BOOLEAN is_survival
                       );

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
//��������Ϸ������
PUBLIC MMIIDLE_SPESCR_T g_seed_scr =
{
    InitSeedScr,
    RedrawScr,
    TriggerSeedScr        
};
//4��seedȺ��ĳ�ʼ�ֲ�ͼ
const SEED_POS_T s_seed_pos_map1[] = 
{
    {4,3},{5,3},{6,3},
    {3,4},{4,4},{6,4},
    {4,5},{5,5},{6,5},
    {2,7},
    {8,9}
};
const SEED_POS_T s_seed_pos_map2[] = 
{
    {4,3},{4,4}
};
const SEED_POS_T s_seed_pos_map3[] = 
{
    {5,3},{6,3},
    {3,4},{4,4},{6,4},
    {4,5},{6,5},
    {2,7},
    {8,9}
};
const SEED_POS_T s_seed_pos_map4[] = 
{
    {4,3},{5,3},{6,3},
    {3,4},
    {4,5},{5,5},{6,5},
    {2,7},
    {8,9}
};

SEED_POS_MAP_T s_seed_pos_map_group[] =
{
    s_seed_pos_map1, sizeof(s_seed_pos_map1)/sizeof(SEED_POS_T),
    s_seed_pos_map2, sizeof(s_seed_pos_map2)/sizeof(SEED_POS_T),
    s_seed_pos_map3, sizeof(s_seed_pos_map3)/sizeof(SEED_POS_T),
    s_seed_pos_map4, sizeof(s_seed_pos_map4)/sizeof(SEED_POS_T)
};


//��������seedȺ��Ķ�̬�ֲ�ͼ
uint8 s_seed_map[SEED_GROUP_NUM][HEIGHT_UNIT][WIDTH_UNIT] = {0};
//��������ͱ�����µ�seedȺ��ֲ�
uint8 s_new_seed_map[HEIGHT_UNIT][WIDTH_UNIT] = {0};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : ��ʼ������
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitSeedScr(void)
{
    InitSeedMap();
}

/*****************************************************************************/
// 	Description : �ػ���Ļ
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void RedrawScr(void)
{
    DrawSeedMap();
}

/*****************************************************************************/
// 	Description : ������ʱ���¼�����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void TriggerSeedScr(void)
{
    GenNewSeed();
    DrawSeedMap();
}

/*****************************************************************************/
// 	Description : seed�Ƿ��Ҵ桱����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSurvival(
                         uint32 i,
                         uint32 x,
                         uint32 y
                         )
{
    BOOLEAN     result      = FALSE;
    uint32      seed_sum    = 0;

    if (IsSeedExist(i, x-1, y-1))
    {
        seed_sum++;
    }
    if (IsSeedExist(i, x, y-1))
    {
        seed_sum++;
    }
    if (IsSeedExist(i, x+1, y-1))
    {
        seed_sum++;
    }
    if (IsSeedExist(i, x-1, y))
    {
        seed_sum++;
    }
    if (IsSeedExist(i, x+1, y))
    {
        seed_sum++;
    }
    if (IsSeedExist(i, x-1, y+1))
    {
        seed_sum++;
    }
    if (IsSeedExist(i, x, y+1))
    {
        seed_sum++;
    }
    if (IsSeedExist(i, x+1, y+1))
    {
        seed_sum++;
    }
    if (IsAtCorner(x, y))
    {
        if (1 == seed_sum)
        {
            result = TRUE;
        }
    }
    else if (IsAtBorder(x, y))
    {
        if ((1 == seed_sum)
            || (2 == seed_sum))
        {
            result = TRUE;
        }
    }
    else
    {
        if (seed_sum>=2
            &&seed_sum<=6)
        {
            result = TRUE;
        }
    }
    return result;
}
 
/*****************************************************************************/
// 	Description : ��seed�ֲ���ʾ����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void DrawSeedMap(void)
{
/*, @cr235885 renwei modify begin*/
    GUI_LCD_DEV_INFO    lcd_dev_info    = {0, 0};
    GUI_RECT_T          rect            = {0};
    uint32              x               = 0;
    uint32              y               = 0;
    uint32              i               = 0;
    uint32              start_x         = 0;
    uint32              start_y         = 0;
    uint32              seed_interval_width  = 0;
    uint32              seed_interval_height = 0;
    uint32              start_x_group[SEED_GROUP_NUM] = {0, 0, 0, 0};//4��seedȺ������Ļ�ϵ�λ��
    uint32              start_y_group[SEED_GROUP_NUM] = {0, 0, 0, 0};//4��seedȺ������Ļ�ϵ�λ��

    rect = MMITHEME_GetFullScreenRect();
    start_x_group[1]     = (rect.right/2);
    start_x_group[3]     = (rect.right/2);
    start_y_group[2]     = (rect.bottom/2);
    start_y_group[3]     = (rect.bottom/2);
    
    seed_interval_width  = (((rect.right/2))/(ENV_WIDTH+2));
    seed_interval_height = (((rect.bottom/2))/(ENV_HEIGHT+2));
    
    LCD_FillRect(&lcd_dev_info, rect, 0x00);
    for (i = 0; i < SEED_GROUP_NUM; i++)
    {
        start_x = start_x_group[i];
        start_y = start_y_group[i];
        for (y = 1; y <= ENV_HEIGHT; y++)
        {
            for (x = 1; x <= ENV_WIDTH; x++)
            {
                if (IsSeedExist(i, x, y))
                {
                    rect.left = start_x + x*seed_interval_width;
                    rect.top = start_y + y*seed_interval_height;
                    rect.right = start_x + x*seed_interval_width+SEED_WIDTH;
                    rect.bottom = start_y + y*seed_interval_height+SEED_HEIGHT;
                    LCD_FillRect(&lcd_dev_info, rect, SEED_COLOR);
                }
            }
        }
    }
/*, @cr235885 renwei modify end*/
}

/*****************************************************************************/
// 	Description : �����µ�seed�ֲ�
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void GenNewSeed(void)
{
    uint32      x   = 0;
    uint32      y   = 0;
    uint32      i   = 0;

    for (i = 0; i < SEED_GROUP_NUM; i++)
    {
        for (y = 1; y <= ENV_HEIGHT; y++)
        {
            for (x = 1; x <= ENV_WIDTH; x++)
            {
                if (3 == i)
                {
                    if (IsSeedExist(2, x, y) != IsSurvival(3, x, y))
                    {
                        i = i;
                    }
                }
                SetNewSeed(x, y, IsSurvival(i, x, y));
            }
        }
        CopySeedMap(i);
    }
}

/*****************************************************************************/
// 	Description : ����seed
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void AddSeed(
                    uint32 i,
                    uint16 x,
                    uint16 y
                    )
{
    uint16      x_high  = 0;
    uint16      x_low   = 0;

    x_high = x>>3;
    x_low = x & 0x7;
    s_seed_map[i][y][x_high] |= (0x80>>x_low);
    if (s_seed_map[i][0][0] == 0x80)
    {
        i = i;
    }
}

/*****************************************************************************/
// 	Description : ��¼�µ�seedλ��
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void SetNewSeed(
                       uint16 x,
                       uint16 y,
                       BOOLEAN is_survival
                       )
{
    uint16      x_high  = 0;
    uint16      x_low   = 0;

    x_high = x>>3;
    x_low = x & 0x7;

    if (is_survival)
    {
        s_new_seed_map[y][x_high] |= (0x80>>x_low);
    }
    else
    {
        s_new_seed_map[y][x_high] &= ~(0x80>>x_low);
    }
}

/*****************************************************************************/
// 	Description : ��ʼ��seed�ķֲ��ͻ���
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void InitSeedMap(void)
{
    uint32          i           = 0;
    uint32          seed_count  = 0;
    SEED_POS_T const *temp_ptr  = PNULL;
    uint32          j           = 0;
    
    SCI_MEMSET(s_seed_map, 0x00, sizeof(s_seed_map));
    for (i = 0; i < SEED_GROUP_NUM; i++)
    {
        seed_count = s_seed_pos_map_group[i].seed_count;
        temp_ptr = s_seed_pos_map_group[i].seed_pos_map;
        for (j = 0; j < seed_count; j++)
        {
            AddSeed(i, temp_ptr->x, temp_ptr->y);        
            temp_ptr++;
        }
    }
}

/*****************************************************************************/
// 	Description : ����¼����λ��copy��s_seed_map
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void CopySeedMap(
                        uint32 i
                        )
{
    MMI_MEMCPY(s_seed_map[i], MAP_SIZE, s_new_seed_map, MAP_SIZE, MAP_SIZE);
}

/*****************************************************************************/
// 	Description : �Ƿ����λ��
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAtCorner(
                         uint16 x,
                         uint16 y
                         )
{
    BOOLEAN     result  = FALSE;

    if ((1 == x
        || ENV_WIDTH == x)
        && (1 == y
        || ENV_HEIGHT == y))
    {
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
// 	Description : �Ƿ��λ��
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAtBorder(
                         uint16 x,
                         uint16 y
                         )
{
    BOOLEAN     result  = FALSE;

    if (1 == x
        || ENV_WIDTH == x
        || 1 == y
        || ENV_HEIGHT == y)
    {
        if (!IsAtCorner(x, y))
        {
            result = TRUE;
        }
    }
    return result;
}

/*****************************************************************************/
// 	Description : ĳȺ���ĳλ���Ƿ���seed����
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsSeedExist(
                          uint32 group_index,
                          uint16 x,
                          uint16 y
                          )
{
    uint16      x_high  = 0;
    uint16      x_low   = 0;
    uint32      c       = 0;

    x_high = x>>3;
    x_low = x & 0x7;
    c = s_seed_map[group_index][y][x_high];

    if (0 == x
        && 0 == y)
    {
        if ((c << x_low) & 0x80)
        {
            c = c;
        }
    }
    return (c << x_low) & 0x80;
}
#endif

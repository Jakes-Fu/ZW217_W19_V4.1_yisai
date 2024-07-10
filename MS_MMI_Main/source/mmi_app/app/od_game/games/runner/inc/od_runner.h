#ifdef OD_RUNNER_SUPPORT
#ifndef __OD_RUNNER_H__
#define __OD_RUNNER_H__


#define OD_DASH_COLOR_COUNT (4)
#define OD_DASH_GAP (25)
#define OD_DASH_COIN_GAP (5)

#define OD_DASH_H   (10)
#define OD_DASH_W   (50)
#define OD_COIN_GAP (8)

#define OD_HUMAN_JUMP_HIGH (12) 
#define OD_HUMAN_MOVE_Y_OFFSET (2)
typedef enum
{
	OD_TYPE_COIN,
	OD_TYPE_HUMAN,
	OD_TYPE_MAX
}OD_RES_TYPE_E;

typedef enum
{
	DASH_TYPE_LOW,
	DASH_TYPE_NORMAL,
	DASH_TYPE_HIGH,
	DASH_TYPE_MAX
}OD_DASH_TYPE_E;



typedef enum
{ 
	OD_COIN_VISIBLE,
	OD_COIN_INVISIBLE,
	OD_COIN_GONE,
	OD_COIN_MAX
}OD_COIN_STATE_E;

typedef enum
{
	OD_HUMAN_RUN,
	OD_HUMAN_RUN2,
	OD_HUMAN_RUN3,
	OD_HUMAN_JUMP_DOWN,
	OD_HUMAN_DJUMP_DOWN,
	OD_HUMAN_JUMP_UP,
	OD_HUMAN_DJUMP,
	OD_HUMAN_DROP,
	OD_HUMAN_DIED,
	OD_HUMAN_MAX
}OD_HUMAN_STATE_E;

typedef enum
{
	OD_DASH_VISIBLE,
	OD_DASH_INVISIBLE,
	OD_DASH_GONE,
	OD_DASH_MAX
}OD_DASH_STATE_E;

typedef enum
{
	OD_RUNNER_GAME_START,
	OD_RUNNER_GAME_RUNNING,
	OD_RUNNER_GAME_PAUSE,
	OD_RUNNER_GAME_STOP,
	OD_RUNNER_GAME_MAX
}OD_RUNNER_STATE_E;

typedef enum
{
	OD_COIN_TYPE_COIN,
	OD_COIN_TYPE_BOMB,
	OD_COIN_TYPE_MAX
}OD_COIN_TYPE_E;

typedef struct
{
    OD_FLOAT x;
    OD_FLOAT y;
}OD_POINT_T;

typedef struct
{
    OD_U16 w;
    OD_U16 h;
    OD_U32 image_id;
}OD_RES_T;



typedef struct{
	OD_POINT_T       point;
	OD_RES_T         *resPtr;
	OD_POINT_T       org_point;
	OD_U8            high;
	OD_S16            count;
	OD_U8            flag;
	OD_HUMAN_STATE_E state;

}OD_HUMAN_T;

typedef struct{
	OD_POINT_T point;
	OD_RES_T   *resPtr;
	OD_COIN_STATE_E state;
	OD_COIN_TYPE_E type;
}OD_COIN_T;


typedef struct{
	OD_POINT_T point;
	OD_U16 w;
	OD_U16 h;
	OD_U8  c;
	OD_DASH_STATE_E state;
	OD_DASH_TYPE_E type;
}OD_DASH_T;

typedef struct{
	od_game_color colors[OD_DASH_COLOR_COUNT];
	OD_U8 count;
}OD_DASH_RES_T;

typedef struct{
	OD_U16   scrW;
	OD_U16   scrH;
	OD_RES_T res[OD_TYPE_MAX];
	OD_RES_T dash;
	OD_FLOAT scale;
	OD_U8    level;
	OD_U8    time;
	OD_FLOAT hoffsetY;
}OD_RUNNER_CFG_T;

#define OD_RUNNER_COINS_MAX (12)
#define OD_RUNNER_DASH_MAX  (4)


typedef struct{
	OD_RUNNER_CFG_T *cfgPtr;
	OD_HUMAN_T human;
    OD_DASH_T  dashes[OD_RUNNER_DASH_MAX];
    OD_COIN_T  coins[OD_RUNNER_COINS_MAX];
	OD_U16     scores;
	OD_U16     speed;
	OD_BOOL    isPause;	
	OD_BOOL    isCoin;
	OD_RUNNER_STATE_E state;
}OD_RUNNER_T;



#endif
#endif

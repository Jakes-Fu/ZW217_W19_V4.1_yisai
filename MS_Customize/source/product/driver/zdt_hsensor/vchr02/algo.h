
#ifndef APP_ALGO_ALGO_H
#define APP_ALGO_ALGO_H

#include "vchr02_common.h"



typedef struct
{
	int32 x;
	int32 y;
	int32 z;
}AlgoAxesData_t;

typedef struct
{
	int32 		hrData;
	int32			reliability;
}AlgoOutputData_t;

typedef struct
{
	AlgoAxesData_t	axes;
	int32 		ppgSample;
	int32			envSample;
}AlgoInputData_t;


typedef enum
{
	
    SPORT_TYPE_NORMAL = 0x00,        // ¨¨?3¡ê
    SPORT_TYPE_RUNNING = 0x01,       // ?¨¹2?
    SPORT_TYPE_RIDE_BIKE = 0X02,     // ??DD
    SPORT_TYPE_JUMP_ROPE = 0X03,     // ¨¬?¨¦t
    SPORT_TYPE_SWIMMING = 0X04,      // ¨®?¨®?
    SPORT_TYPE_BADMINTON = 0X05,     // ¨®e???¨°
    SPORT_TYPE_TABLE_TENNIS = 0X06,  // ?1?¨°?¨°
    SPORT_TYPE_TENNIS = 0X07,        // ¨ª??¨°
    SPORT_TYPE_CLIMBING = 0X08,      // ?¨¤¨¦?
    SPORT_TYPE_WALKING = 0X09,       // ¨ª?2?
    SPORT_TYPE_BASKETBALL = 0X0A,    // ¨¤o?¨°
    SPORT_TYPE_FOOTBALL = 0X0B,      // ¡Á??¨°
    SPORT_TYPE_BASEBALL = 0X0C,      // ¡ã??¨°
    SPORT_TYPE_VOLLEYBALL = 0X0D,    // ???¨°
    SPORT_TYPE_CRICKET = 0X0E,       // ¡ã??¨°
    SPORT_TYPE_RUGBY = 0X0F,         // ¨¦?¨¦-?¨°
    SPORT_TYPE_HOCKEY = 0X10,        // ?¨²1¡Â?¨°
    SPORT_TYPE_DANCE = 0X11,         // ¨¬??¨¨
    SPORT_TYPE_SPINNING = 0X12,      // ?¡¥?D¦Ì£¤3¦Ì
    SPORT_TYPE_YOGA = 0X13,          // ¨¨¡è¨´¡è
    SPORT_TYPE_SIT_UP = 0X14,        // ?????e¡Á?
    SPORT_TYPE_TREADMILL = 0X15,     // ?¨¹2??¨²
    SPORT_TYPE_GYMNASTICS = 0X16,    // ¨¬?2¨´
    SPORT_TYPE_BOATING = 0X17,       // ??¡ä?
    SPORT_TYPE_JUMPING_JACK = 0X18,  // ?ao?¨¬?
    SPORT_TYPE_FREE_TRAINING = 0X19, // ¡Á?¨®¨¦?¦Ì¨¢¡¤

} AlgoSportMode;

void Algo_Init(void);
void Algo_Input(AlgoInputData_t *pInputData,int32 gaptime,int16 sportMode);
void Algo_Output(AlgoOutputData_t *pOutputData);
void Algo_Version(char *pVersionOutput);



#endif /* APP_ALGO_ALGO_H_ */




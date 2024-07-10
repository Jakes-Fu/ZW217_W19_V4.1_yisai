#include "tb_drv.h"
#include "tb_dal.h"

//#include "tpd_drv.h"

typedef enum TPD_TO_TPSVR_MSG_ENUM
{
	TP_NONE_MESSAGE = 007,
	TP_ENTER_INTO_GPIO_HANDLER,
	TP_ENTER_INTO_TIMER_HANDLER
} TPD_TO_TPSVR_MSG_E;
typedef struct TPD_TO_TPSVR_MSG_TAG
{
	SIGNAL_VARS
	TPD_TO_TPSVR_MSG_E  tpd_msg_valu;
} TPD_TO_TPSVR_MSG_T;
typedef struct TPDSVR_SIG_tag
{
	//xSignalHeaderRec signal_head;
	SIGNAL_VARS
    int     x_key;
    int		y_key;
} TPDSVR_SIG_T;

#define 	TP_MSG_MAX_NUM     		   (TP_MSG_MAX & 0x00FF)

//#define     TP_ADC_MIN_X               152
//#define     TP_ADC_MAX_X               352
//#define     TP_ADC_MIN_Y               304
//#define     TP_ADC_MAX_Y               856
#define     FLOAT32_PRE                1000        //
//#define     TP_X_FACTOR                1000
//#define     TP_Y_FACTOR                1000
#define     CALIBRATION_FLAG           0x5555aaaa
//#define 	TP_WIDTH		           176
//#define	    TP_HEIGHT		           220


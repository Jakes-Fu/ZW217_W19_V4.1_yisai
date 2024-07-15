/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"
#include "guistring.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#define FORMULA_LINE_WIDTH MMI_MAINSCREEN_WIDTH/6
#define FORMULA_LINE_HIGHT MMI_MAINSCREEN_HEIGHT/10
#define FORMULA_LIST_ITEM_HIGHT MMI_MAINSCREEN_HEIGHT/12

#define FORMULA_APP_ID "00000003"
#define FORMULA_APP_SECRET "ZmtFormulaSec"
#define FORMULA_HEADER_PATH "http://8.130.95.8:8866/"
#define FORMULA_BASE_TEXT_PATH "formulas/getFormulasAudio?%s"

#define FORMULA_AUDIO_USE_OFF_DATA 0

#define FORMULA_APP_COUNT_MAX 2
#define FORMULA_COUNT_ITEM_MAX 45

//#define FORMULA_RING_START_ID RING_FORMULA_R1

typedef struct
{
    char text[20];
}FORMULA_MULTI_TEXT_T;

typedef enum
{
    FORMULA_ACTION_NONE = 0,
    FORMULA_ACTION_PLAY,
    FORMULA_ACTION_STOP,
    FORMULA_ACTION_RESET,
    FORMULA_ACTION_END,
}FORMULA_ACTION_STATUS_TYPE;

typedef struct
{
    uint8 play_idx;
    FORMULA_ACTION_STATUS_TYPE play_status;
}FORMULA_PLAY_INFO_T;

typedef struct
{
    char * audio_uri;
    char * audio_data;
    uint16 audio_len;
}FORMULA_AUDIO_INFO_T;

PUBLIC void MMI_CreateFormulaTableWin(void);
PUBLIC void MMIZMT_CloseFormulaPlayer(void);
PUBLIC void MMI_CreateFormulaTableTipWin(uint8 idx);

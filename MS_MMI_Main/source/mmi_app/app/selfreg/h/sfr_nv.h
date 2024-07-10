
#ifndef _MMISFR_NV_H_
#define _MMISFR_NV_H_

typedef enum
{
    MMINV_SFR_SIM_ICCID = MMI_MODULE_SFR << 16,
    MMINV_SFR_EVENT_ID,
#ifdef SFR_SUPPORT_CUCC
    MMINV_SFR_CUCC_TYPE,
    MMINV_SFR_CUCC_COLLECT,
    MMINV_SFR_CUCC_CRICLE,
    MMINV_SFR_CUCC_SERVER,
    MMINV_SFR_CUCC_HISTORY,
#endif
     MMINV_SFR_ITEM_NUM
} MMISFR_NV_ITEM_E;

#ifdef SFR_SUPPORT_CUCC
//B ?a??¡ä??a?¨²¡ä£¤¡¤¡é
//T ?a?¨¹??¨®??¡ì?¡§¡ä£¤¡¤¡é
// S ?a?¨²¡Á??¨¹?¨²D?¡Á¡é2¨¢¨¬??t¡ä£¤¡¤¡é
// F ?a¨º¡Á¡ä??a?¨²¡ä£¤¡¤¡é

typedef enum
{
    MMISFR_CUCC_TYPE_FIRST = 0,
    MMISFR_CUCC_TYPE_BOOT,
    MMISFR_CUCC_TYPE_CHANG,
    MMISFR_CUCC_TYPE_CIRCLE,
    MMISFR_CUCC_TYPE_OFF,
} MMISFR_CUCC_TYPE_E;

typedef enum
{
    MMISFR_CUCC_CIRCLE_5MIN = 0,
    MMISFR_CUCC_CIRCLE_HOUR,
    MMISFR_CUCC_CIRCLE_DAY,
    MMISFR_CUCC_CIRCLE_WEEK,
    MMISFR_CUCC_CIRCLE_MONTH,
} MMISFR_CUCC_CIRCLE_E;

typedef enum
{
    MMISFR_CUCC_SERVER_REG = 0,
    MMISFR_CUCC_SERVER_TEST,
} MMISFR_CUCC_SERVER_E;

#define CUCC_HIS_COUNT (4)

typedef struct {
    MMISFR_CUCC_TYPE_E his_type;
    char his_time[24];
} MMI_SFR_CUCC_HIS_ITEM;

#endif

PUBLIC void MMISFR_RegNv(void);
PUBLIC char * MMISFR_NVGetICCID(uint8 sim);
PUBLIC void MMISFR_NVSetICCID(uint8 sim, char *iccid);
PUBLIC BOOLEAN MMISFR_IsICCIDChanged(MN_DUAL_SYS_E sim);
PUBLIC uint32 MMISFR_GetEventID(void);
PUBLIC void MMISFR_SetEventID(uint32 evid);

#ifdef SFR_SUPPORT_CUCC
PUBLIC uint8 MMISFR_GetCUCCType(void);
PUBLIC void MMISFR_SetCUCCType(uint8 type);
PUBLIC BOOLEAN  MMISFR_GetCollectInfoMode(void);
PUBLIC void MMISFR_SetCollectInfoMode(BOOLEAN collect_info_mode);
#endif

#endif

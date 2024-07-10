/******************************************************************************
 ** File Name:      nmea_log.h                                                     *
 ** Author:         Richard Yang                                              *
 ** DATE:           11/20/2017                                              *
 ** Copyright:       Incoporated. All Rights Reserved.         *
 ** Description:    This driver is for development board, Integrator/AP.      *
 ******************************************************************************

 ******************************************************************************/

#ifndef _NMEA_LOG_H_
#define _NMEA_LOG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sio.h"
//#include "com_drvapi.h"
//#include "chip.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


typedef enum
{
    NMEA_LOG_SUC = 0,
    NMEA_LOG_NOSPACE = 1,
    NMEA_LOG_FAIL = 2
} NMEA_WRITELOG_RESULT_E;

typedef enum
{
    LOG_SAVE_NMEALOG,
    LOG_SAVE_CIRCLETEST,
    LOG_SAVE_TYPE_MAX
}LOG_SAVE_TYPE;

/*****************************************************************************/
// Description: stop save file
// Global resource dependence: BTlogfile_num, sfs_BT_LOG
// Author: jackey.ling
// Note:
/*****************************************************************************/
PUBLIC void MMIENG_LogClose (LOG_SAVE_TYPE log_type);

/*****************************************************************************/
// Description: open log, create file
// Global resource dependence: sfs_BT_LOG
// Author: jackey.ling
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENG_LogOpen (uint8 *newLog_fileName,LOG_SAVE_TYPE log_type);

/*begin add cm4 check bug:620209*/
/*****************************************************************************/
// Description: write log
// Global resource dependence: 
// Author:
// Note:
/*****************************************************************************/
PUBLIC uint16 MMIENG_WriteLOGFile(
                uint8*              write_buff_ptr,
                uint32              write_length,
                LOG_SAVE_TYPE       log_type
                );


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /*End BT_LOG.h*/
#endif



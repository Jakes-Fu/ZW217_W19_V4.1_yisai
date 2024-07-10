/******************************************************************************
 ** File Name:      atc_public_phone.h                                        *
 ** Author:         Xueli.lv                                                  *
 ** DATE:           07/02/2003                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 7/2/2003       Xueli.lv          Create.                                  *
 ******************************************************************************/
#ifndef _ATC_PUBLIC_PHONE
#define _ATC_PUBLIC_PHONE

#include "atc.h"
#include "mmi_pt.h"

/*****************************************************************************/
//  Description : This function handles the AT+1001 command.This command is used
//                to enable or diable the feature of locking CELL ID.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process1001);

/*****************************************************************************/
//  Description : This function handles the AT+1002 command.This command is used
//                to set or read the amount of the locked cellid.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process1002);

/*****************************************************************************/
//  Description : This function handles the AT+1100 command.This command is used
//                to set or read the locked cellid.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process1100);

/*****************************************************************************/
//  Description : This function handles the AT+2001 command.This command is used
//                to enable or diable the feature of locking LAI.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process2001);

/*****************************************************************************/
//  Description : This function handles the AT+2002 command.This command is used
//                to enable or diable the feature of locking LAI.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process2002);

/*****************************************************************************/
//  Description : This function handles the AT+3001 command.This command is used
//                to set or read the directly dialing number.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process3001);

/*****************************************************************************/
//  Description : This function handles the AT+4001 command.This command is used
//                to enable or disable the feature of locking SIM card.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process4001);

/*****************************************************************************/
//  Description : This function handles the AT+5001 command.This command is used
//                to set or read the first PIN code.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process5001);

/*****************************************************************************/
//  Description : This function handles the AT+5002 command.This command is used
//                to set or read the second PIN code.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process5002);

/*****************************************************************************/
//  Description : This function handles the AT+6001 command.This command is used
//                to enable or disable the function of locking  PLMN card.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process6001);

/*****************************************************************************/
//  Description : This function handles the AT+6101 command.This command is used
//                to set or read the PLMN code.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process6101);

/*****************************************************************************/
//  Description : This function handles the AT+7001 command.This command is used
//                to enable or disable the function of locking IP.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process7001);

/*****************************************************************************/
//  Description : This function handles the AT+7101 command.This command is used
//                to set or read the specific IP number.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process7101);

/*****************************************************************************/
//  Description : This function handles the AT+8101 command.This command is used
//                to set or read the specific number type and relative IP index.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process8101);

/*****************************************************************************/
//  Description : This function handles the AT+9101 command.This command is used
//                to set or read the network management center number.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process9101);

/*****************************************************************************/
//  Description : This function handles the AT+A001 command.This command is used
//                to set or read the type of barrier incoming calls.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessA001);

/*****************************************************************************/
//  Description : This function handles the AT+A101 command.This command is used
//                to set or read the specific barrier incoming call number.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessA101);

/*****************************************************************************/
//  Description : This function handles the AT+B001 command.This command is used
//                to enable or diable the feature of locking LAI.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessB001);

/*****************************************************************************/
//  Description : This function handles the AT+B101 command.This command is used
//                to set or read the specific barrier outgoing call number.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessB101);

/*****************************************************************************/
//  Description : This function handles the AT+C101 command.This command is used
//                to set or read the first dialing number.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessC101);

/*****************************************************************************/
//  Description : This function handles the AT+C102 command.This command is used
//                to set or read the first interval time.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessC102);

/*****************************************************************************/
//  Description : This function handles the AT+C103 command.This command is used
//                to set or read the second dialing number.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessC103);

/*****************************************************************************/
//  Description : This function handles the AT+C104 command.This command is used
//                to set or read the second interval time.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessC104);

/*****************************************************************************/
//  Description : This function handles the AT+C105 command.This command is used
//                to set or read the third dialing number.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessC105);

/*****************************************************************************/
//  Description : This function handles the AT+E101 command.This command is used
//                to set or read the specific band for the MS to campon on.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessE101);

/*****************************************************************************/
//  Description : This function handles the AT+F001 command.This command is used
//                to set or read the first interval time.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessF001);

/*****************************************************************************/
//  Description : This function handles the AT+2002 command.This command is used
//                to set or read the amount of the available of LAC.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process2003);

/*****************************************************************************/
//  Description : This function handles the AT+4002 command.This command is used
//                to enable or disable the feature of locking phone module.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process4002);

/*****************************************************************************/
//  Description : This function handles the AT+5003 command.This command is used
//                to set or read the random pin code.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process5003);

/*****************************************************************************/
//  Description : This function handles the AT+5004 command.This command is used
//                to set or read the password of cryptogram.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Process5004);

/*****************************************************************************/
//  Description : This function handles the AT+G001 command.This command is used
//                to set or read the IMSI string.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessG001);

/*****************************************************************************/
//  Description : This function handles the AT+H001 command.This command is used
//                to set or read the specific server number
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessH001);

/*****************************************************************************/
//  Description : This function handles the AT+I002 command.This command is used
//                to set or read the repeated flag.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI002);

/*****************************************************************************/
//  Description : This function handles the AT+I001 command.This command is used
//                to set or read the functional flag.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI001);

/*****************************************************************************/
//  Description : This function handles the AT+I003 command.This command is used
//                to set or read the repeated time.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI003);

/*****************************************************************************/
//  Description : This function handles the AT+I004 command.This command is used
//                to set or read the client identity.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI004);

/*****************************************************************************/
//  Description : This function handles the AT+I005 command.This command is used
//                to set or read the server identity.
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessI005);
#endif
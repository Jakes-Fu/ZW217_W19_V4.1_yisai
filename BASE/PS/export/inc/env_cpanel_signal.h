/******************************************************************************
 ** File Name:      env_cpanel_signal.h                                       *
 ** Author:         Ivan Ren                                                  *
 ** Date:           05/23/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file define CPanel signals                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/23/2002     Ivan Ren         Create.                                   *
 ******************************************************************************/

#ifndef _ENV_CPANEL_SIGNAL_H
#define _ENV_CPANEL_SIGNAL_H

#define ENV_CPANEL_SIGNAL_LIST	\
		MN_SETUP_REQ = (( P_CPANEL << 8 ) | 1 )+1, \
		MN_SETUP_CNF, \
		MN_DISC_IND, \
		MN_SETUP_IND,\
		MN_SETUP_RSP,\
		MN_DISC_REQ,\
		MN_CELL_INFO_IND,\
		MN_CELL_INFO_REQ,\
		MN_SMS_REQ,\
		MN_SMS_IND,\
		MN_POWER_OFF_REQ,\
		END_CPANEL_SIGNAL  \

#endif

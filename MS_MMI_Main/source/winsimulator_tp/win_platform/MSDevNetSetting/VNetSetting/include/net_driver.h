/*****************************************************************************
 ** File Name:     net_driver.h                                              *
 ** Author:        yifeng.wang                                               *
 ** Date:          2008/02/27                                                *
 ** Copyright:     2008 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:   interfaces for winpcap driver interfaces.                 *
 *****************************************************************************
 **                        Edit History                                      *
 ** -------------------------------------------------------------------------*
 ** DATE           NAME                   DESCRIPTION                        *
 ** 2008/02/27     yifeng.wang            Create                             *
 ** 2010/08/27     yifeng.wang            Amend for simulater check          *
 *****************************************************************************/

#ifndef _NET_DRIVER_H_
#define _NET_DRIVER_H_

/*****************************************************************************/
// Description : ��� Winpcap �����Ƿ��Ѿ���װ
// Global : 
// Author : yifeng.wang
// Note : RETURN: TRUE - driver is installed; FALSE - not installed
/*****************************************************************************/
PUBLIC BOOL NETDRIVER_IsInstalled( void );

/*****************************************************************************/
// Description : ��װ Winpcap ����
// Global : 
// Author : yifeng.wang
// Note : RETURN: TRUE - install driver successfully; FALSE - failed
/*****************************************************************************/
PUBLIC BOOL NETDRIVER_InstallDriver( void );



#endif /* _NET_DRIVER_H_ */
/*******************************************************************************
 ** File Name:     atc_gsmonly_def.h                                                   *
 ** Author:        Steven.Yao                                                  *
 ** DATE:          04/11/2002                                                  *
 ** Copyright:     2001 Spreadtrum, Incoporated. All Rights Reserved.          *
 ** Description:   This file defines the AT commands, including basic commands *
 **                a extend commands.                                          *
 *******************************************************************************
                                                                               *
 *******************************************************************************
 **                         Edit History                                       *
 **----------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                                *
 ** 04/11/2002     Steven.Yao       Create.                                    *
 ******************************************************************************/
#ifndef _ATC_GSMONLY_DEF_H
#define _ATC_GSMONLY_DEF_H

/* Define AT command index */

#define AT_CMD_AT       0
#define AT_CMD_SHARP_SHARP       1
#define AT_CMD_D       2
#define AT_CMD_A       3
#define AT_CMD_H       4
#define AT_CMD_E       5
#define AT_CMD_CPIN       6
#define AT_CMD_CLIP       7
#define AT_CMD_CCWA       8
#define AT_CMD_VTS       9
#define AT_CMD_SIND      10
#define AT_CMD_CSMP      11
#define AT_CMD_COLP      12
#define AT_CMD_CCUG      13
#define AT_CMD_VGR      14
#define AT_CMD_CMUT      15
#define AT_CMD_SSEA      16
#define AT_CMD_SBAND      17
#define AT_CMD_CLCK      18
#define AT_CMD_CPWD      19
#define AT_CMD_SSAM      20
#define AT_CMD_SSAP      21
#define AT_CMD_SADM      22
#define AT_CMD_SADMDSP      23
#define AT_CMD_CCED      24
#define AT_CMD_SRWD      25
#define AT_CMD_SDMUT      26
#define AT_CMD_SLOG      27
#define AT_CMD_SUPS      28
#define AT_CMD_SCPFRE      29
#define AT_CMD_SCINF      30
#define AT_CMD_SPEQPARA      31
#define AT_CMD_SPREF      32
#define AT_CMD_SYTIN      33
#define AT_CMD_SFTIN      34
#define AT_CMD_SPENHA      35
#define AT_CMD_SPADLVVE      36
#define AT_CMD_HEADINFO      37
#define AT_CMD_SYSSLEEP      38
#define AT_CMD_MAXPOWER      39
#define AT_CMD_FCEPTEST      40
#define AT_CMD_SPKSTEST      41
#define AT_CMD_KEYSHORT      42
#define AT_CMD_READRSSI      43
#define AT_CMD_GETTESTNV      44
#define AT_CMD_SETTESTNV      45
#define AT_CMD_GETFULLHISTNV      46
#define AT_CMD_LOGERASE      47
#define AT_CMD_GETFULLTESTNV      48
#define AT_CMD_TEMPTEST      49
#define AT_CMD_BATTTEST      50
#define AT_CMD_IMEITEST      51
#define AT_CMD_LVOFLOCK      52
#define AT_CMD_DETALOCK      53
#define AT_CMD_RSTVERIF      54
#define AT_CMD_LOOPTEST      55
#define AT_CMD_LOCKREAD      56
#define AT_CMD_CVHU      57
#define AT_CMD_MSLSECUR      58
#define AT_CMD_SERIALNO      59
#define AT_CMD_FACTORST      60
#define AT_CMD_KSTRINGB      61
#define AT_CMD_AKSEEDNO      62
#define AT_CMD_CALIDATE      63
#define AT_CMD_SIMDETEC      64
#define AT_CMD_SMS      65
#define AT_CMD_MISCD      66
#define AT_CMD_EMEMTEST      67
#define AT_CMD_FCMPTEST      68
#define AT_CMD_FCBTTEST      69
#define AT_CMD_SETFDATA      70
#define AT_CMD_SETFULLFDATA      71
#define AT_CMD_GETFDATA      72
#define AT_CMD_GETFULLFDATA      73
#define AT_CMD_CPOL      74
#define AT_CMD_CPBS      75
#define AT_CMD_CREG      76
#define AT_CMD_COPS      77
#define AT_CMD_CMOD      78
#define AT_CMD_CPBR      79
#define AT_CMD_CPBF      80
#define AT_CMD_CMGL      81
#define AT_CMD_CSMS      82
#define AT_CMD_CSDH      83
#define AT_CMD_CPMS      84
#define AT_CMD_CMGF      85
#define AT_CMD_CPBW      86
#define AT_CMD_CPBP      87
#define AT_CMD_CPBN      88
#define AT_CMD_CNUM      89
#define AT_CMD_CCFC      90
#define AT_CMD_CSCS      91
#define AT_CMD_CAOC      92
#define AT_CMD_CMGR      93
#define AT_CMD_CSCA      94
#define AT_CMD_CNMI      95
#define AT_CMD_CMGD      96
#define AT_CMD_CMGS      97
#define AT_CMD_CMGW      98
#define AT_CMD_SMSC      99
#define AT_CMD_CLCC     100
#define AT_CMD_CUSD     101
#define AT_CMD_CLIR     102
#define AT_CMD_CSQ     103
#define AT_CMD_CFUN     104
#define AT_CMD_CCLK     105
#define AT_CMD_CIND     106
#define AT_CMD_CRSL     107
#define AT_CMD_CMMS     108
#define AT_CMD_SSYS     109
#define AT_CMD_CPAS     110
#define AT_CMD_XX     111
#define AT_CMD_SIME     112
#define AT_CMD_CIMI     113
#define AT_CMD_CCID     114
#define AT_CMD_SPLO     115
#define AT_CMD_PSORGMOD     116
#define AT_CMD_SFSMSM     117
#define AT_CMD_CMGC     118
#define AT_CMD_CMSS     119
#define AT_CMD_S0     120
#define AT_CMD_CSCB     121

#endif /* End of #ifndef _ATC_GSMONLY_DEF_H */

/*******************************************************************************
 ** File Name:     atc_def.h                                                   *
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
#ifndef _ATC_DEF_H
#define _ATC_DEF_H

/* Define AT command index */

#define AT_CMD_AT       0
#define AT_CMD_SHARP_SHARP       1
#define AT_CMD_D       2
#define AT_CMD_Q       3
#define AT_CMD_V       4
#define AT_CMD_A       5
#define AT_CMD_H       6
#define AT_CMD_Z       7
#define AT_CMD_E       8
#define AT_CMD_and_F       9
#define AT_CMD_and_D      10
#define AT_CMD_and_C      11
#define AT_CMD_CPAS      12
#define AT_CMD_CPIN      13
#define AT_CMD_CLIP      14
#define AT_CMD_CLIR      15
#define AT_CMD_CCFC      16
#define AT_CMD_CCWA      17
#define AT_CMD_CREG      18
#define AT_CMD_VTS      19
#define AT_CMD_CHLD      20
#define AT_CMD_CLCC      21
#define AT_CMD_COPS      22
#define AT_CMD_SIND      23
#define AT_CMD_CSQ      24
#define AT_CMD_CIMI      25
#define AT_CMD_CGMI      26
#define AT_CMD_CGMM      27
#define AT_CMD_CGSN      28
#define AT_CMD_CGMR      29
#define AT_CMD_CMEE      30
#define AT_CMD_CSCS      31
#define AT_CMD_CPOF      32
#define AT_CMD_CPOL      33
#define AT_CMD_S0      34
#define AT_CMD_CGDCONT      35
#define AT_CMD_CGPCO      36
#define AT_CMD_CGQREQ      37
#define AT_CMD_CGQMIN      38
#define AT_CMD_CGATT      39
#define AT_CMD_CGACT      40
#define AT_CMD_CGCMOD      41
#define AT_CMD_CGPADDR      42
#define AT_CMD_CGDATA      43
#define AT_CMD_CGEREP      44
#define AT_CMD_CGREG      45
#define AT_CMD_CGSMS      46
#define AT_CMD_CSMS      47
#define AT_CMD_CSDH      48
#define AT_CMD_CRES      49
#define AT_CMD_CPMS      50
#define AT_CMD_CMGF      51
#define AT_CMD_CMGR      52
#define AT_CMD_CMGL      53
#define AT_CMD_CMGD      54
#define AT_CMD_CMGS      55
#define AT_CMD_CMSS      56
#define AT_CMD_CSMP      57
#define AT_CMD_CMGC      58
#define AT_CMD_CMGW      59
#define AT_CMD_CSCA      60
#define AT_CMD_CSAS      61
#define AT_CMD_SMSC      62
#define AT_CMD_CSCB      63
#define AT_CMD_CNMI      64
#define AT_CMD_CPBR      65
#define AT_CMD_CPBF      66
#define AT_CMD_CPBW      67
#define AT_CMD_CPBP      68
#define AT_CMD_CPBN      69
#define AT_CMD_CNUM      70
#define AT_CMD_SSMP      71
#define AT_CMD_CPBS      72
#define AT_CMD_CCUG      73
#define AT_CMD_CUSD      74
#define AT_CMD_CAOC      75
#define AT_CMD_CCLK      76
#define AT_CMD_XX      77
#define AT_CMD_CCID      78
#define AT_CMD_VGR      79
#define AT_CMD_CRSL      80
#define AT_CMD_CMUT      81
#define AT_CMD_SSEA      82
#define AT_CMD_SBAND      83
#define AT_CMD_SDTMF      84
#define AT_CMD_CLCK      85
#define AT_CMD_CPWD      86
#define AT_CMD_SSAM      87
#define AT_CMD_SSAP      88
#define AT_CMD_SADM      89
#define AT_CMD_SADMDSP      90
#define AT_CMD_SPLO      91
#define AT_CMD_CFUN      92
#define AT_CMD_CIND      93
#define AT_CMD_CCED      94
#define AT_CMD_STONE      95
#define AT_CMD_SSST      96
#define AT_CMD_SATT      97
#define AT_CMD_SAUTOATT      98
#define AT_CMD_SGPRSDATA      99
#define AT_CMD_SDMUT     100
#define AT_CMD_SUPS     101
#define AT_CMD_SSYS     102
#define AT_CMD_SCPFRE     103
#define AT_CMD_CGEQREQ     104
#define AT_CMD_CGEQMIN     105
#define AT_CMD_SCINF     106
#define AT_CMD_SFSMSM     107
#define AT_CMD_SPEQPARA     108
#define AT_CMD_SPREF     109
#define AT_CMD_SYTIN     110
#define AT_CMD_SFTIN     111
#define AT_CMD_CMUX     112
#define AT_CMD_SIME     113
#define AT_CMD_SCMUX     114
#define AT_CMD_PSORGMOD     115
#define AT_CMD_SPENHA     116
#define AT_CMD_SPADLVVE     117
#define AT_CMD_CMMS     118
#define AT_CMD_CMER     119
#define AT_CMD_CMOD     120
#define AT_CMD_CHUP     121
#define AT_CMD_HEADINFO     122
#define AT_CMD_SYSSLEEP     123
#define AT_CMD_MAXPOWER     124
#define AT_CMD_FCEPTEST     125
#define AT_CMD_SPKSTEST     126
#define AT_CMD_KEYSHORT     127
#define AT_CMD_READRSSI     128
#define AT_CMD_GETTESTNV     129
#define AT_CMD_SETTESTNV     130
#define AT_CMD_GETFULLHISTNV     131
#define AT_CMD_LOGERASE     132
#define AT_CMD_GETFULLTESTNV     133
#define AT_CMD_TEMPTEST     134
#define AT_CMD_BATTTEST     135
#define AT_CMD_IMEITEST     136
#define AT_CMD_LVOFLOCK     137
#define AT_CMD_DETALOCK     138
#define AT_CMD_RSTVERIF     139
#define AT_CMD_LOOPTEST     140
#define AT_CMD_LOCKREAD     141
#define AT_CMD_CVHU     142
#define AT_CMD_MSLSECUR     143
#define AT_CMD_SERIALNO     144
#define AT_CMD_FACTORST     145
#define AT_CMD_KSTRINGB     146
#define AT_CMD_AKSEEDNO     147
#define AT_CMD_CALIDATE     148
#define AT_CMD_SIMDETEC     149
#define AT_CMD_SMS     150
#define AT_CMD_MISCD     151
#define AT_CMD_EMEMTEST     152
#define AT_CMD_FCMPTEST     153
#define AT_CMD_FCBTTEST     154
#define AT_CMD_SETFDATA     155
#define AT_CMD_SETFULLFDATA     156
#define AT_CMD_GETFDATA     157
#define AT_CMD_GETFULLFDATA     158
#define AT_CMD_SPAUDVOLTYPE     159

#endif /* End of #ifndef _ATC_DEF_H */

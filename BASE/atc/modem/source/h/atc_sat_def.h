/*******************************************************************************
 ** File Name:     atc_sat_def.h                                 *
 ** Author:        Steven.Yao                                                  *
 ** DATE:          04/10/2012                                                  *
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
#ifndef _ATC_SAT_DEF_H
#define _ATC_SAT_DEF_H

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
#define AT_CMD_and_W      12
#define AT_CMD_W      13
#define AT_CMD_THREE_PERCENT      14
#define AT_CMD_S0      15
#define AT_CMD_S2      16
#define AT_CMD_S3      17
#define AT_CMD_S4      18
#define AT_CMD_S5      19
#define AT_CMD_S6      20
#define AT_CMD_S7      21
#define AT_CMD_S8      22
#define AT_CMD_S10      23
#define AT_CMD_CPAS      24
#define AT_CMD_CPIN      25
#define AT_CMD_CCFC      26
#define AT_CMD_CCWA      27
#define AT_CMD_CREG      28
#define AT_CMD_CRC      29
#define AT_CMD_VTS      30
#define AT_CMD_CHLD      31
#define AT_CMD_CLCC      32
#define AT_CMD_COPS      33
#define AT_CMD_CSSN      34
#define AT_CMD_SIND      35
#define AT_CMD_CSQ      36
#define AT_CMD_CIMI      37
#define AT_CMD_CGMI      38
#define AT_CMD_CGMM      39
#define AT_CMD_CGSN      40
#define AT_CMD_CGMR      41
#define AT_CMD_CMEE      42
#define AT_CMD_CSCS      43
#define AT_CMD_CPOF      44
#define AT_CMD_CPOL      45
#define AT_CMD_CGDCONT      46
#define AT_CMD_CGPCO      47
#define AT_CMD_CGQREQ      48
#define AT_CMD_CGQMIN      49
#define AT_CMD_CGATT      50
#define AT_CMD_CGACT      51
#define AT_CMD_CGPADDR      52
#define AT_CMD_CGDATA      53
#define AT_CMD_CGAUTO      54
#define AT_CMD_CGEREP      55
#define AT_CMD_CGREG      56
#define AT_CMD_CSMS      57
#define AT_CMD_CSDH      58
#define AT_CMD_CRES      59
#define AT_CMD_CPMS      60
#define AT_CMD_CMGF      61
#define AT_CMD_CMGR      62
#define AT_CMD_CMGL      63
#define AT_CMD_CMGD      64
#define AT_CMD_CMGS      65
#define AT_CMD_CMSS      66
#define AT_CMD_CSMP      67
#define AT_CMD_CMGC      68
#define AT_CMD_CMGW      69
#define AT_CMD_CSCA      70
#define AT_CMD_CSAS      71
#define AT_CMD_SMSC      72
#define AT_CMD_SUSS      73
#define AT_CMD_CNMI      74
#define AT_CMD_CPBR      75
#define AT_CMD_CPBF      76
#define AT_CMD_CPBW      77
#define AT_CMD_CPBP      78
#define AT_CMD_CPBN      79
#define AT_CMD_CNUM      80
#define AT_CMD_CSVM      81
#define AT_CMD_CPBS      82
#define AT_CMD_CPUC      83
#define AT_CMD_CUSD      84
#define AT_CMD_CACM      85
#define AT_CMD_CAMM      86
#define AT_CMD_CCLK      87
#define AT_CMD_XX      88
#define AT_CMD_VTD      89
#define AT_CMD_CCID      90
#define AT_CMD_VGR      91
#define AT_CMD_CRSL      92
#define AT_CMD_CMUT      93
#define AT_CMD_VGT      94
#define AT_CMD_SPEAKER      95
#define AT_CMD_SSEA      96
#define AT_CMD_SBAND      97
#define AT_CMD_SDTMF      98
#define AT_CMD_ECHO      99
#define AT_CMD_CLCK     100
#define AT_CMD_CPWD     101
#define AT_CMD_SSAM     102
#define AT_CMD_SSAP     103
#define AT_CMD_SADM     104
#define AT_CMD_SPLO     105
#define AT_CMD_CICB     106
#define AT_CMD_CFUN     107
#define AT_CMD_IPR     108
#define AT_CMD_CIND     109
#define AT_CMD_CCED     110
#define AT_CMD_STONE     111
#define AT_CMD_SAC     112
#define AT_CMD_SATT     113
#define AT_CMD_CEER     114
#define AT_CMD_CR     115
#define AT_CMD_SDCP     116
#define AT_CMD_SNVM     117
#define AT_CMD_SBOOT     118
#define AT_CMD_SDMUT     119
#define AT_CMD_SLOG     120
#define AT_CMD_SUPS     121
#define AT_CMD_SSYS     122
#define AT_CMD_SCPFRE     123
#define AT_CMD_CGEQREQ     124
#define AT_CMD_CGEQMIN     125
#define AT_CMD_CRSM     126
#define AT_CMD_SCINF     127
#define AT_CMD_SPEQPARA     128
#define AT_CMD_SPREF     129
#define AT_CMD_CMUX     130
#define AT_CMD_SIME     131
#define AT_CMD_SCMGS     132
#define AT_CMD_SCMGW     133
#define AT_CMD_PSORGMOD     134
#define AT_CMD_SCREG     135
#define AT_CMD_CIEV     136
#define AT_CMD_NETAPN     137
#define AT_CMD_SOCKET     138
#define AT_CMD_IPOPEN     139
#define AT_CMD_IPSEND     140
#define AT_CMD_IPCLOSE     141
#define AT_CMD_IPDEACT     142
#define AT_CMD_GSMSTATUS     143
#define AT_CMD_FIRBOOT     144
#define AT_CMD_VGSMSW     145
#define AT_CMD_POSI     146
#define AT_CMD_SMMSWAP     147
#define AT_CMD_SADMDSP     148
#define AT_CMD_SPADLVVE     149
#define AT_CMD_SPAUDVOLTYPE     150
#define AT_CMD_SPUSIMTEST     151

#endif /* End of #ifndef _ATC_SAT_DEF_H */

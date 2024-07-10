/******************************************************************************
 ** File Name:      ms_ref_scp_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:67
#ifndef _MS_REF_SCP_TRC_H_
#define _MS_REF_SCP_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define CARD_SDIO_832_112_2_18_1_52_28_0 "SD20 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
#define CARD_SDIO_839_112_2_18_1_52_28_1 "CARD_SD_V2_0_HIGHCAP:"
#define CARD_SDIO_840_112_2_18_1_52_28_2 "CSD_STRUCTURE = %x"
#define CARD_SDIO_841_112_2_18_1_52_28_3 "reserved1= %x"
#define CARD_SDIO_842_112_2_18_1_52_28_4 "TAAC= %x"
#define CARD_SDIO_843_112_2_18_1_52_28_5 "NSAC= %x"
#define CARD_SDIO_844_112_2_18_1_52_28_6 "TRAN_SPEED= %x"
#define CARD_SDIO_845_112_2_18_1_52_28_7 "CCC= %x"
#define CARD_SDIO_846_112_2_18_1_52_28_8 "READ_BL_LEN= %x"
#define CARD_SDIO_847_112_2_18_1_52_28_9 "READ_BL_PARTIAL= %x"
#define CARD_SDIO_848_112_2_18_1_52_28_10 "WRITE_BLK_MISALIGN= %x"
#define CARD_SDIO_849_112_2_18_1_52_28_11 "READ_BLK_MISALIGN= %x"
#define CARD_SDIO_850_112_2_18_1_52_28_12 "DSR_IMP= %x"
#define CARD_SDIO_852_112_2_18_1_52_28_13 "reserved2= %x"
#define CARD_SDIO_853_112_2_18_1_52_28_14 "C_SIZE= %x"
#define CARD_SDIO_854_112_2_18_1_52_28_15 "reserved3= %x"
#define CARD_SDIO_856_112_2_18_1_52_28_16 "ERASE_BLK_EN= %x"
#define CARD_SDIO_857_112_2_18_1_52_28_17 "SECTOR_SIZE= %x"
#define CARD_SDIO_858_112_2_18_1_52_28_18 "WP_GRP_SIZE= %x"
#define CARD_SDIO_859_112_2_18_1_52_28_19 "WP_GRP_ENABLE= %x"
#define CARD_SDIO_860_112_2_18_1_52_28_20 "reserved4= %x"
#define CARD_SDIO_862_112_2_18_1_52_28_21 "R2W_FACTOR= %x"
#define CARD_SDIO_863_112_2_18_1_52_28_22 "WRITE_BL_LEN= %x"
#define CARD_SDIO_864_112_2_18_1_52_28_23 "WRITE_BL_PARTIAL= %x"
#define CARD_SDIO_865_112_2_18_1_52_28_24 "reserved5= %x"
#define CARD_SDIO_867_112_2_18_1_52_28_25 "FILE_FORMAT_GRP= %x"
#define CARD_SDIO_868_112_2_18_1_52_28_26 "COPY= %x"
#define CARD_SDIO_869_112_2_18_1_52_28_27 "PERM_WRITE_PROTECT= %x"
#define CARD_SDIO_870_112_2_18_1_52_28_28 "TMP_WRITE_PROTECT= %x"
#define CARD_SDIO_871_112_2_18_1_52_28_29 "FILE_FORMAT= %x"
#define CARD_SDIO_872_112_2_18_1_52_28_30 "reserved6= %x"
#define CARD_SDIO_884_112_2_18_1_52_28_31 "CARD_SD_V2_0_STANDARD||CARD_SD_V1_X:"
#define CARD_SDIO_885_112_2_18_1_52_28_32 "CSD_STRUCTURE = %x"
#define CARD_SDIO_886_112_2_18_1_52_28_33 "reserved1= %x"
#define CARD_SDIO_887_112_2_18_1_52_28_34 "TAAC= %x"
#define CARD_SDIO_888_112_2_18_1_52_28_35 "NSAC= %x"
#define CARD_SDIO_889_112_2_18_1_52_28_36 "TRAN_SPEED= %x"
#define CARD_SDIO_890_112_2_18_1_52_28_37 "CCC= %x"
#define CARD_SDIO_891_112_2_18_1_52_28_38 "READ_BL_LEN= %x"
#define CARD_SDIO_892_112_2_18_1_52_28_39 "READ_BL_PARTIAL= %x"
#define CARD_SDIO_893_112_2_18_1_52_28_40 "WRITE_BLK_MISALIGN= %x"
#define CARD_SDIO_894_112_2_18_1_52_28_41 "READ_BLK_MISALIGN= %x"
#define CARD_SDIO_895_112_2_18_1_52_28_42 "DSR_IMP= %x"
#define CARD_SDIO_897_112_2_18_1_52_28_43 "reserved2= %x"
#define CARD_SDIO_898_112_2_18_1_52_28_44 "C_SIZE= %x"
#define CARD_SDIO_899_112_2_18_1_52_28_45 "VDD_R_CURR_MIN = %x"
#define CARD_SDIO_900_112_2_18_1_52_28_46 "VDD_R_CURR_MAX = %x"
#define CARD_SDIO_901_112_2_18_1_52_28_47 "VDD_W_CURR_MIN = %x"
#define CARD_SDIO_902_112_2_18_1_52_28_48 "VDD_W_CURR_MAX = %x"
#define CARD_SDIO_903_112_2_18_1_52_28_49 "C_SIZE_MULT = %x"
#define CARD_SDIO_905_112_2_18_1_52_28_50 "ERASE_BLK_EN= %x"
#define CARD_SDIO_906_112_2_18_1_52_28_51 "SECTOR_SIZE= %x"
#define CARD_SDIO_907_112_2_18_1_52_28_52 "WP_GRP_SIZE= %x"
#define CARD_SDIO_908_112_2_18_1_52_28_53 "WP_GRP_ENABLE= %x"
#define CARD_SDIO_909_112_2_18_1_52_28_54 "reserved3= %x"
#define CARD_SDIO_911_112_2_18_1_52_28_55 "R2W_FACTOR= %x"
#define CARD_SDIO_912_112_2_18_1_52_28_56 "WRITE_BL_LEN= %x"
#define CARD_SDIO_913_112_2_18_1_52_28_57 "WRITE_BL_PARTIAL= %x"
#define CARD_SDIO_914_112_2_18_1_52_28_58 "reserved4= %x"
#define CARD_SDIO_916_112_2_18_1_52_28_59 "FILE_FORMAT_GRP= %x"
#define CARD_SDIO_917_112_2_18_1_52_28_60 "COPY= %x"
#define CARD_SDIO_918_112_2_18_1_52_28_61 "PERM_WRITE_PROTECT= %x"
#define CARD_SDIO_919_112_2_18_1_52_28_62 "TMP_WRITE_PROTECT= %x"
#define CARD_SDIO_920_112_2_18_1_52_28_63 "FILE_FORMAT= %x"
#define CARD_SDIO_921_112_2_18_1_52_28_64 "reserved5= %x"
#define CARD_SDIO_931_112_2_18_1_52_28_65 "CARD_MMC_331:"
#define CARD_SDIO_932_112_2_18_1_52_28_66 "CSD_STRUCTURE\t\t  = 0x%x"
#define CARD_SDIO_933_112_2_18_1_52_28_67 "SPEC_VERS\t\t  = 0x%x"
#define CARD_SDIO_934_112_2_18_1_52_28_68 "reserved1\t\t  = 0x%x"
#define CARD_SDIO_935_112_2_18_1_52_28_69 "TAAC\t\t\t  = 0x%x"
#define CARD_SDIO_936_112_2_18_1_52_28_70 "NSAC\t\t\t  = 0x%x"
#define CARD_SDIO_937_112_2_18_1_52_28_71 "TRAN_SPEED\t\t  = 0x%x"
#define CARD_SDIO_938_112_2_18_1_52_28_72 "CCC\t\t\t  = 0x%x"
#define CARD_SDIO_939_112_2_18_1_52_28_73 "READ_BL_LEN\t\t  = 0x%x"
#define CARD_SDIO_940_112_2_18_1_52_28_74 "READ_BL_PARTIAL\t\t  = 0x%x"
#define CARD_SDIO_941_112_2_18_1_52_28_75 "WRITE_BLK_MISALIGN\t  = 0x%x"
#define CARD_SDIO_942_112_2_18_1_52_28_76 "READ_BLK_MISALIGN\t  = 0x%x"
#define CARD_SDIO_943_112_2_18_1_52_28_77 "DSR_IMP\t\t\t  = 0x%x"
#define CARD_SDIO_944_112_2_18_1_52_28_78 "reserved2\t\t  = 0x%x"
#define CARD_SDIO_945_112_2_18_1_52_28_79 "C_SIZE\t\t\t  = 0x%x"
#define CARD_SDIO_946_112_2_18_1_52_28_80 "VDD_R_CURR_MIN\t\t  = 0x%x"
#define CARD_SDIO_947_112_2_18_1_52_28_81 "VDD_R_CURR_MAX\t\t  = 0x%x"
#define CARD_SDIO_948_112_2_18_1_52_28_82 "VDD_W_CURR_MIN\t\t  = 0x%x"
#define CARD_SDIO_949_112_2_18_1_52_28_83 "VDD_W_CURR_MAX\t\t  = 0x%x"
#define CARD_SDIO_950_112_2_18_1_52_28_84 "C_SIZE_MULT\t\t  = 0x%x"
#define CARD_SDIO_951_112_2_18_1_52_28_85 "ERASE_GRP_SIZE\t\t  = 0x%x"
#define CARD_SDIO_952_112_2_18_1_52_28_86 "ERASE_GRP_MULT\t\t  = 0x%x"
#define CARD_SDIO_953_112_2_18_1_52_28_87 "WP_GRP_SIZE\t\t  = 0x%x"
#define CARD_SDIO_954_112_2_18_1_52_28_88 "WP_GRP_ENABLE\t\t  = 0x%x"
#define CARD_SDIO_955_112_2_18_1_52_28_89 "DEFAULT_ECC\t\t  = 0x%x"
#define CARD_SDIO_956_112_2_18_1_52_28_90 "R2W_FACTOR\t\t  = 0x%x"
#define CARD_SDIO_957_112_2_18_1_52_28_91 "WRITE_BL_LEN\t\t  = 0x%x"
#define CARD_SDIO_958_112_2_18_1_52_28_92 "WRITE_BL_PARTIAL\t  = 0x%x"
#define CARD_SDIO_959_112_2_18_1_52_28_93 "reserved3\t\t  = 0x%x"
#define CARD_SDIO_960_112_2_18_1_52_28_94 "CONTENT_PROT_APP\t  = 0x%x"
#define CARD_SDIO_961_112_2_18_1_52_28_95 "----RW bit---"
#define CARD_SDIO_962_112_2_18_1_52_28_96 "FILE_FORMAT_GRP\t\t  = 0x%x"
#define CARD_SDIO_963_112_2_18_1_52_28_97 "COPY\t\t\t  = 0x%x"
#define CARD_SDIO_964_112_2_18_1_52_28_98 "PERM_WRITE_PROTECT\t  = 0x%x"
#define CARD_SDIO_965_112_2_18_1_52_28_99 "TMP_WRITE_PROTECT\t  = 0x%x"
#define CARD_SDIO_966_112_2_18_1_52_28_100 "FILE_FORMAT\t\t  = 0x%x"
#define CARD_SDIO_967_112_2_18_1_52_28_101 "ECC\t\t\t  = 0x%x"
#define CARD_SDIO_1002_112_2_18_1_52_29_102 "SD20 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
#define CARD_SDIO_1011_112_2_18_1_52_29_103 "-----CID value---\r\n"
#define CARD_SDIO_1012_112_2_18_1_52_29_104 "MID\t\t = 0x%x\r\n"
#define CARD_SDIO_1013_112_2_18_1_52_29_105 "OID\t\t = 0x%x\r\n"
#define CARD_SDIO_1014_112_2_18_1_52_29_106 "PNM\t\t = %c,%c,%c,%c,%c\r\n"
#define CARD_SDIO_1015_112_2_18_1_52_29_107 "PRV\t\t = 0x%x\r\n"
#define CARD_SDIO_1016_112_2_18_1_52_29_108 "PSN\t\t = 0x%x\r\n"
#define CARD_SDIO_1017_112_2_18_1_52_29_109 "MDT\t\t = 0x%x\r\n"
#define CARD_SDIO_1027_112_2_18_1_52_29_110 "-----MMC CID value---\r\n"
#define CARD_SDIO_1028_112_2_18_1_52_29_111 "MID\t\t = 0x%x\r\n"
#define CARD_SDIO_1029_112_2_18_1_52_29_112 "OID\t\t = 0x%x\r\n"
#define CARD_SDIO_1030_112_2_18_1_52_29_113 "PNM\t\t = %c,%c,%c,%c,%c,%c\r\n"
#define CARD_SDIO_1031_112_2_18_1_52_29_114 "PRV\t\t = 0x%x\r\n"
#define CARD_SDIO_1032_112_2_18_1_52_29_115 "PSN\t\t = 0x%x\r\n"
#define CARD_SDIO_1033_112_2_18_1_52_29_116 "MDT\t\t = 0x%x\r\n"
#define CARD_SDIO_1121_112_2_18_1_52_29_117 "SD20 RCA = %x"
#define CARD_SDIO_1235_112_2_18_1_52_29_118 "SD20 Block Length of SD V2.0 High capacity is fixed to 512,you cannot set to value 0x%x\r\n"
#define CARD_SDIO_1313_112_2_18_1_52_29_119 "currentVal = 0x%x"
#define CARD_SDIO_1315_112_2_18_1_52_29_120 "grp6Supported = 0x%x"
#define CARD_SDIO_1316_112_2_18_1_52_29_121 "grp5Supported = 0x%x"
#define CARD_SDIO_1317_112_2_18_1_52_29_122 "grp4Supported = 0x%x"
#define CARD_SDIO_1318_112_2_18_1_52_29_123 "grp3Supported = 0x%x"
#define CARD_SDIO_1319_112_2_18_1_52_29_124 "grp2Supported = 0x%x"
#define CARD_SDIO_1320_112_2_18_1_52_29_125 "grp1Supported = 0x%x"
#define CARD_SDIO_1322_112_2_18_1_52_29_126 "grp6SwitchResult = 0x%x"
#define CARD_SDIO_1323_112_2_18_1_52_29_127 "grp5SwitchResult = 0x%x"
#define CARD_SDIO_1324_112_2_18_1_52_29_128 "grp4SwitchResult = 0x%x"
#define CARD_SDIO_1325_112_2_18_1_52_29_129 "grp3SwitchResult = 0x%x"
#define CARD_SDIO_1326_112_2_18_1_52_29_130 "grp2SwitchResult = 0x%x"
#define CARD_SDIO_1327_112_2_18_1_52_29_131 "grp1SwitchResult = 0x%x"
#define CARD_SDIO_1329_112_2_18_1_52_29_132 "status version = 0x%x"
#define CARD_SDIO_1331_112_2_18_1_52_29_133 "grp6BusyStatus = 0x%x"
#define CARD_SDIO_1332_112_2_18_1_52_29_134 "grp5BusyStatus = 0x%x"
#define CARD_SDIO_1333_112_2_18_1_52_29_135 "grp4BusyStatus = 0x%x"
#define CARD_SDIO_1334_112_2_18_1_52_29_136 "grp3BusyStatus = 0x%x"
#define CARD_SDIO_1335_112_2_18_1_52_29_137 "grp2BusyStatus = 0x%x"
#define CARD_SDIO_1336_112_2_18_1_52_29_138 "grp1BusyStatus = 0x%x"
#define CARD_SDIO_1420_112_2_18_1_52_30_139 "SD20 enter high speed mode!!!"
#define CARD_SDIO_1537_112_2_18_1_52_30_140 "SD20 %x,%x,%x,%x"
#define CARD_SDIO_1548_112_2_18_1_52_30_141 "SD20 vertion_flag = %x"
#define CARD_SDIO_1583_112_2_18_1_52_30_142 "SD20 %x,%x,%x,%x"
#define CARD_SDIO_1642_112_2_18_1_52_30_143 "SD20 %x,%x,%x,%x"
#define CARD_SDIO_1706_112_2_18_1_52_30_144 "SD20 type is %x"
#define CARD_SDIO_1812_112_2_18_1_52_30_145 "[Card_sdio] CARD_SDIO_InitCard: Disable HighSpeed Mode !"
#define CARD_SDIO_1871_112_2_18_1_52_31_146 "[Card_sdio] _IsCardReady timeout !"
#define CARD_SDIO_2367_112_2_18_1_52_32_147 "card exit mcex success"
#define CARD_SPI_528_112_2_18_1_52_33_148 "card Read busy 1B signal timeout"
#define CARD_SPI_536_112_2_18_1_52_33_149 "card Read busy 1B signal timeout"
#define CARD_SPI_630_112_2_18_1_52_33_150 "card Read busy signal timeout"
#define CARD_SPI_668_112_2_18_1_52_33_151 "card CRC read error"
#define CARD_SPI_737_112_2_18_1_52_33_152 "card write error1 0x%x"
#define CARD_SPI_762_112_2_18_1_52_33_153 "card write error1 0x%x"
#define CARD_SPI_777_112_2_18_1_52_33_154 "card write busy signal timeout"
#define CARD_SPI_1377_112_2_18_1_52_34_155 "Block Length of SD V2.0 High capacity is fixed to 512,you cannot set to value 0x%x"
#define CARD_SPI_1491_112_2_18_1_52_35_156 "Response = %x"
#define CARD_SPI_1493_112_2_18_1_52_35_157 "-----CID value---"
#define CARD_SPI_1494_112_2_18_1_52_35_158 "MID\t\t = 0x%x"
#define CARD_SPI_1495_112_2_18_1_52_35_159 "OID\t\t = 0x%x"
#define CARD_SPI_1496_112_2_18_1_52_35_160 "PNM\t\t = %c,%c,%c,%c,%c"
#define CARD_SPI_1497_112_2_18_1_52_35_161 "PRV\t\t = 0x%x"
#define CARD_SPI_1498_112_2_18_1_52_35_162 "PSN\t\t = 0x%x"
#define CARD_SPI_1499_112_2_18_1_52_35_163 "MDT\t\t = 0x%x"
#define CARD_SPI_1500_112_2_18_1_52_35_164 "CRC\t\t = 0x%x"
#define CARD_SPI_1501_112_2_18_1_52_35_165 "lastBit\t\t = 0x%x"
#define CARD_SPI_1506_112_2_18_1_52_35_166 "Response = %x"
#define CARD_SPI_1508_112_2_18_1_52_35_167 "-----CID value---"
#define CARD_SPI_1509_112_2_18_1_52_35_168 "MID\t\t = 0x%x"
#define CARD_SPI_1510_112_2_18_1_52_35_169 "OID\t\t = 0x%x"
#define CARD_SPI_1511_112_2_18_1_52_35_170 "PNM\t\t = %c,%c,%c,%c,%c,%c"
#define CARD_SPI_1512_112_2_18_1_52_35_171 "PRV\t\t = 0x%x"
#define CARD_SPI_1513_112_2_18_1_52_35_172 "PSN\t\t = 0x%x"
#define CARD_SPI_1514_112_2_18_1_52_35_173 "MDT\t\t = 0x%x"
#define CARD_SPI_1515_112_2_18_1_52_35_174 "CRC\t\t = 0x%x"
#define CARD_SPI_1516_112_2_18_1_52_35_175 "lastBit\t\t = 0x%x"
#define CARD_SPI_1746_112_2_18_1_52_35_176 "Response = %x"
#define CARD_SPI_1748_112_2_18_1_52_35_177 "CSD_STRUCTURE = %x"
#define CARD_SPI_1749_112_2_18_1_52_35_178 "reserved1= %x"
#define CARD_SPI_1750_112_2_18_1_52_35_179 "TAAC= %x"
#define CARD_SPI_1751_112_2_18_1_52_35_180 "NSAC= %x"
#define CARD_SPI_1752_112_2_18_1_52_35_181 "TRAN_SPEED= %x"
#define CARD_SPI_1753_112_2_18_1_52_35_182 "CCC= %x"
#define CARD_SPI_1754_112_2_18_1_52_35_183 "READ_BL_LEN= %x"
#define CARD_SPI_1755_112_2_18_1_52_35_184 "READ_BL_PARTIAL= %x"
#define CARD_SPI_1756_112_2_18_1_52_35_185 "WRITE_BLK_MISALIGN= %x"
#define CARD_SPI_1757_112_2_18_1_52_35_186 "READ_BLK_MISALIGN= %x"
#define CARD_SPI_1758_112_2_18_1_52_35_187 "DSR_IMP= %x"
#define CARD_SPI_1760_112_2_18_1_52_35_188 "reserved2= %x"
#define CARD_SPI_1761_112_2_18_1_52_35_189 "C_SIZE= %x"
#define CARD_SPI_1762_112_2_18_1_52_35_190 "reserved3= %x"
#define CARD_SPI_1764_112_2_18_1_52_35_191 "ERASE_BLK_EN= %x"
#define CARD_SPI_1765_112_2_18_1_52_35_192 "SECTOR_SIZE= %x"
#define CARD_SPI_1766_112_2_18_1_52_35_193 "WP_GRP_SIZE= %x"
#define CARD_SPI_1767_112_2_18_1_52_35_194 "WP_GRP_ENABLE= %x"
#define CARD_SPI_1768_112_2_18_1_52_35_195 "reserved4= %x"
#define CARD_SPI_1770_112_2_18_1_52_35_196 "R2W_FACTOR= %x"
#define CARD_SPI_1771_112_2_18_1_52_35_197 "WRITE_BL_LEN= %x"
#define CARD_SPI_1772_112_2_18_1_52_35_198 "WRITE_BL_PARTIAL= %x"
#define CARD_SPI_1773_112_2_18_1_52_35_199 "reserved5= %x"
#define CARD_SPI_1775_112_2_18_1_52_35_200 "FILE_FORMAT_GRP= %x"
#define CARD_SPI_1776_112_2_18_1_52_35_201 "COPY= %x"
#define CARD_SPI_1777_112_2_18_1_52_35_202 "PERM_WRITE_PROTECT= %x"
#define CARD_SPI_1778_112_2_18_1_52_35_203 "TMP_WRITE_PROTECT= %x"
#define CARD_SPI_1779_112_2_18_1_52_35_204 "FILE_FORMAT= %x"
#define CARD_SPI_1780_112_2_18_1_52_35_205 "reserved6= %x"
#define CARD_SPI_1781_112_2_18_1_52_35_206 "CRC= %x"
#define CARD_SPI_1782_112_2_18_1_52_35_207 "LastBit= %x"
#define CARD_SPI_1783_112_2_18_1_52_35_208 "---------"
#define CARD_SPI_1796_112_2_18_1_52_35_209 "Response = %x"
#define CARD_SPI_1798_112_2_18_1_52_35_210 "CSD_STRUCTURE = %x"
#define CARD_SPI_1799_112_2_18_1_52_35_211 "reserved1= %x"
#define CARD_SPI_1800_112_2_18_1_52_35_212 "TAAC= %x"
#define CARD_SPI_1801_112_2_18_1_52_35_213 "NSAC= %x"
#define CARD_SPI_1802_112_2_18_1_52_35_214 "TRAN_SPEED= %x"
#define CARD_SPI_1803_112_2_18_1_52_35_215 "CCC= %x"
#define CARD_SPI_1804_112_2_18_1_52_35_216 "READ_BL_LEN= %x"
#define CARD_SPI_1805_112_2_18_1_52_35_217 "READ_BL_PARTIAL= %x"
#define CARD_SPI_1806_112_2_18_1_52_35_218 "WRITE_BLK_MISALIGN= %x"
#define CARD_SPI_1807_112_2_18_1_52_35_219 "READ_BLK_MISALIGN= %x"
#define CARD_SPI_1808_112_2_18_1_52_35_220 "DSR_IMP= %x"
#define CARD_SPI_1810_112_2_18_1_52_35_221 "reserved2= %x"
#define CARD_SPI_1811_112_2_18_1_52_35_222 "C_SIZE= %x"
#define CARD_SPI_1812_112_2_18_1_52_35_223 "VDD_R_CURR_MIN = %x"
#define CARD_SPI_1813_112_2_18_1_52_35_224 "VDD_R_CURR_MAX = %x"
#define CARD_SPI_1814_112_2_18_1_52_35_225 "VDD_W_CURR_MIN = %x"
#define CARD_SPI_1815_112_2_18_1_52_35_226 "VDD_W_CURR_MAX = %x"
#define CARD_SPI_1816_112_2_18_1_52_35_227 "C_SIZE_MULT = %x"
#define CARD_SPI_1818_112_2_18_1_52_35_228 "ERASE_BLK_EN= %x"
#define CARD_SPI_1819_112_2_18_1_52_35_229 "SECTOR_SIZE= %x"
#define CARD_SPI_1820_112_2_18_1_52_35_230 "WP_GRP_SIZE= %x"
#define CARD_SPI_1821_112_2_18_1_52_35_231 "WP_GRP_ENABLE= %x"
#define CARD_SPI_1822_112_2_18_1_52_35_232 "reserved3= %x"
#define CARD_SPI_1824_112_2_18_1_52_35_233 "R2W_FACTOR= %x"
#define CARD_SPI_1825_112_2_18_1_52_35_234 "WRITE_BL_LEN= %x"
#define CARD_SPI_1826_112_2_18_1_52_35_235 "WRITE_BL_PARTIAL= %x"
#define CARD_SPI_1827_112_2_18_1_52_35_236 "reserved4= %x"
#define CARD_SPI_1829_112_2_18_1_52_35_237 "FILE_FORMAT_GRP= %x"
#define CARD_SPI_1830_112_2_18_1_52_35_238 "COPY= %x"
#define CARD_SPI_1831_112_2_18_1_52_35_239 "PERM_WRITE_PROTECT= %x"
#define CARD_SPI_1832_112_2_18_1_52_35_240 "TMP_WRITE_PROTECT= %x"
#define CARD_SPI_1833_112_2_18_1_52_35_241 "FILE_FORMAT= %x"
#define CARD_SPI_1834_112_2_18_1_52_35_242 "reserved5= %x"
#define CARD_SPI_1835_112_2_18_1_52_35_243 "CRC= %x"
#define CARD_SPI_1836_112_2_18_1_52_35_244 "LastBit= %x"
#define CARD_SPI_1837_112_2_18_1_52_35_245 "---------"
#define CARD_SPI_1848_112_2_18_1_52_35_246 "Response = %x"
#define CARD_SPI_1850_112_2_18_1_52_35_247 "----Read only bit---"
#define CARD_SPI_1851_112_2_18_1_52_35_248 "CSD_STRUCTURE\t\t  = 0x%x"
#define CARD_SPI_1852_112_2_18_1_52_35_249 "SPEC_VERS\t\t  = 0x%x"
#define CARD_SPI_1853_112_2_18_1_52_35_250 "reserved1\t\t  = 0x%x"
#define CARD_SPI_1854_112_2_18_1_52_35_251 "TAAC\t\t\t  = 0x%x"
#define CARD_SPI_1855_112_2_18_1_52_35_252 "NSAC\t\t\t  = 0x%x"
#define CARD_SPI_1856_112_2_18_1_52_35_253 "TRAN_SPEED\t\t  = 0x%x"
#define CARD_SPI_1857_112_2_18_1_52_35_254 "CCC\t\t\t  = 0x%x"
#define CARD_SPI_1858_112_2_18_1_52_35_255 "READ_BL_LEN\t\t  = 0x%x"
#define CARD_SPI_1859_112_2_18_1_52_35_256 "READ_BL_PARTIAL\t\t  = 0x%x"
#define CARD_SPI_1860_112_2_18_1_52_35_257 "WRITE_BLK_MISALIGN\t  = 0x%x"
#define CARD_SPI_1861_112_2_18_1_52_35_258 "READ_BLK_MISALIGN\t  = 0x%x"
#define CARD_SPI_1862_112_2_18_1_52_35_259 "DSR_IMP\t\t\t  = 0x%x"
#define CARD_SPI_1863_112_2_18_1_52_35_260 "reserved2\t\t  = 0x%x"
#define CARD_SPI_1864_112_2_18_1_52_35_261 "C_SIZE\t\t\t  = 0x%x"
#define CARD_SPI_1865_112_2_18_1_52_35_262 "VDD_R_CURR_MIN\t\t  = 0x%x"
#define CARD_SPI_1866_112_2_18_1_52_35_263 "VDD_R_CURR_MAX\t\t  = 0x%x"
#define CARD_SPI_1867_112_2_18_1_52_35_264 "VDD_W_CURR_MIN\t\t  = 0x%x"
#define CARD_SPI_1868_112_2_18_1_52_35_265 "VDD_W_CURR_MAX\t\t  = 0x%x"
#define CARD_SPI_1869_112_2_18_1_52_35_266 "C_SIZE_MULT\t\t  = 0x%x"
#define CARD_SPI_1870_112_2_18_1_52_35_267 "ERASE_GRP_SIZE\t\t  = 0x%x"
#define CARD_SPI_1871_112_2_18_1_52_35_268 "ERASE_GRP_MULT\t\t  = 0x%x"
#define CARD_SPI_1872_112_2_18_1_52_35_269 "WP_GRP_SIZE\t\t  = 0x%x"
#define CARD_SPI_1873_112_2_18_1_52_35_270 "WP_GRP_ENABLE\t\t  = 0x%x"
#define CARD_SPI_1874_112_2_18_1_52_35_271 "DEFAULT_ECC\t\t  = 0x%x"
#define CARD_SPI_1875_112_2_18_1_52_35_272 "R2W_FACTOR\t\t  = 0x%x"
#define CARD_SPI_1876_112_2_18_1_52_35_273 "WRITE_BL_LEN\t\t  = 0x%x"
#define CARD_SPI_1877_112_2_18_1_52_35_274 "WRITE_BL_PARTIAL\t  = 0x%x"
#define CARD_SPI_1878_112_2_18_1_52_35_275 "reserved3\t\t  = 0x%x"
#define CARD_SPI_1879_112_2_18_1_52_35_276 "CONTENT_PROT_APP\t  = 0x%x"
#define CARD_SPI_1880_112_2_18_1_52_35_277 "----RW bit---"
#define CARD_SPI_1881_112_2_18_1_52_35_278 "FILE_FORMAT_GRP\t\t  = 0x%x"
#define CARD_SPI_1882_112_2_18_1_52_35_279 "COPY\t\t\t  = 0x%x"
#define CARD_SPI_1883_112_2_18_1_52_35_280 "PERM_WRITE_PROTECT\t  = 0x%x"
#define CARD_SPI_1884_112_2_18_1_52_35_281 "TMP_WRITE_PROTECT\t  = 0x%x"
#define CARD_SPI_1885_112_2_18_1_52_35_282 "FILE_FORMAT\t\t  = 0x%x"
#define CARD_SPI_1886_112_2_18_1_52_35_283 "ECC\t\t\t  = 0x%x"
#define CARD_SPI_1887_112_2_18_1_52_35_284 "CRC\t\t\t  = 0x%x"
#define CARD_SPI_1888_112_2_18_1_52_35_285 "LastBit\t\t\t  = 0x%x"
#define CARD_SPI_1889_112_2_18_1_52_35_286 "---------"
#define CARD_SPI_1900_112_2_18_1_52_35_287 "Nac_Max = 0x%x %x"
#define CARD_SPI_1901_112_2_18_1_52_35_288 "Nac_T = 0x%x"
#define CARD_SPI_1902_112_2_18_1_52_35_289 "clock 0x%x"
#define CARD_SPI_1972_112_2_18_1_52_35_290 "currentVal = 0x%x"
#define CARD_SPI_1974_112_2_18_1_52_35_291 "grp6Supported = 0x%x"
#define CARD_SPI_1975_112_2_18_1_52_35_292 "grp5Supported = 0x%x"
#define CARD_SPI_1976_112_2_18_1_52_35_293 "grp4Supported = 0x%x"
#define CARD_SPI_1977_112_2_18_1_52_35_294 "grp3Supported = 0x%x"
#define CARD_SPI_1978_112_2_18_1_52_35_295 "grp2Supported = 0x%x"
#define CARD_SPI_1979_112_2_18_1_52_35_296 "grp1Supported = 0x%x"
#define CARD_SPI_1981_112_2_18_1_52_35_297 "grp6SwitchResult = 0x%x"
#define CARD_SPI_1982_112_2_18_1_52_35_298 "grp5SwitchResult = 0x%x"
#define CARD_SPI_1983_112_2_18_1_52_35_299 "grp4SwitchResult = 0x%x"
#define CARD_SPI_1984_112_2_18_1_52_35_300 "grp3SwitchResult = 0x%x"
#define CARD_SPI_1985_112_2_18_1_52_35_301 "grp2SwitchResult = 0x%x"
#define CARD_SPI_1986_112_2_18_1_52_35_302 "grp1SwitchResult = 0x%x"
#define CARD_SPI_1988_112_2_18_1_52_35_303 "status version = 0x%x"
#define CARD_SPI_1990_112_2_18_1_52_36_304 "grp6BusyStatus = 0x%x"
#define CARD_SPI_1991_112_2_18_1_52_36_305 "grp5BusyStatus = 0x%x"
#define CARD_SPI_1992_112_2_18_1_52_36_306 "grp4BusyStatus = 0x%x"
#define CARD_SPI_1993_112_2_18_1_52_36_307 "grp3BusyStatus = 0x%x"
#define CARD_SPI_1994_112_2_18_1_52_36_308 "grp2BusyStatus = 0x%x"
#define CARD_SPI_1995_112_2_18_1_52_36_309 "grp1BusyStatus = 0x%x"
#define CARD_SPI_2080_112_2_18_1_52_36_310 "CARD_SPI_InitCard"
#define CARD_SPI_2108_112_2_18_1_52_36_311 "Card waitting for CMD0's response is time out"
#define CARD_SPI_2126_112_2_18_1_52_36_312 "SD2.0 not response to CMD8,SD2.0 not surpport current operation voltage range"
#define CARD_SPI_2134_112_2_18_1_52_36_313 "not SD2.0 Card ,Maybe it is SD1.x Card or MMC card"
#define CARD_SPI_2140_112_2_18_1_52_36_314 "VCA = 0, SD2.0 not surpport current operation voltage range"
#define CARD_SPI_2145_112_2_18_1_52_36_315 "SD2.0 waitting for CMD8's response is time out"
#define CARD_SPI_2164_112_2_18_1_52_36_316 "It maybe SD1.x or MMC But it is Not response to CMD58 "
#define CARD_SPI_2170_112_2_18_1_52_36_317 "It look CMD58 as illegal command so it is not SD card"
#define CARD_SPI_2178_112_2_18_1_52_36_318 "OCR is 0x%x"
#define CARD_SPI_2181_112_2_18_1_52_36_319 "SD 1.x But not surpport current voltage"
#define CARD_SPI_2193_112_2_18_1_52_36_320 "Not SD card1"
#define CARD_SPI_2199_112_2_18_1_52_36_321 "Not SD card2 , may be MMC"
#define CARD_SPI_2205_112_2_18_1_52_36_322 "Not SD card3"
#define CARD_SPI_2211_112_2_18_1_52_36_323 "Not SD card4 , may be MMC"
#define CARD_SPI_2218_112_2_18_1_52_36_324 "It is SD1.x Card!!!"
#define CARD_SPI_2224_112_2_18_1_52_36_325 "SD2.0 waitting for ACMD41's response is time out"
#define CARD_SPI_2251_112_2_18_1_52_36_326 "MMC CMD0 response is time out"
#define CARD_SPI_2272_112_2_18_1_52_36_327 "MMC CMD1 response is time out"
#define CARD_SPI_2287_112_2_18_1_52_36_328 "It maybe SD2.0 But it is Not response to CMD58 "
#define CARD_SPI_2293_112_2_18_1_52_36_329 "It look CMD58 as illegal command so it is not SD card"
#define CARD_SPI_2304_112_2_18_1_52_36_330 "SD 2.0 But not surpport current voltage"
#define CARD_SPI_2315_112_2_18_1_52_36_331 "Not SD card5"
#define CARD_SPI_2321_112_2_18_1_52_36_332 "Not SD card6"
#define CARD_SPI_2327_112_2_18_1_52_36_333 "Not SD card7"
#define CARD_SPI_2333_112_2_18_1_52_36_334 "Not SD card8"
#define CARD_SPI_2339_112_2_18_1_52_36_335 "SD2.0 ACMD41's response is time out"
#define CARD_SPI_2350_112_2_18_1_52_36_336 "It maybe SD2.0 But it is Not response to 2nd CMD58 "
#define CARD_SPI_2356_112_2_18_1_52_36_337 "It look 2nd CMD58 as illegal command so it is not SD card"
#define CARD_SPI_2368_112_2_18_1_52_36_338 "It is SD2.0 high capacity Card!!!"
#define CARD_SPI_2373_112_2_18_1_52_36_339 "It is SD2.0 standard capacity Card!!!"
#define CARD_SPI_2387_112_2_18_1_52_36_340 "Turn on CRC fail 1"
#define CARD_SPI_2393_112_2_18_1_52_36_341 "Turn on CRC fail 2"
#define CARD_SPI_2727_112_2_18_1_52_37_342 "card write stop busy signal timeout"
#define CARD_SPI_3523_112_2_18_1_52_38_343 "card exit mcex success"
#define SDIO_CARD_PAL_223_112_2_18_1_52_39_344 "SCM Wait event error!!! eRet = 0x%x"
#define SDIO_CARD_PAL_817_112_2_18_1_52_40_345 "SDIO_Card may be erase R1b is too long"
#define SDIO_CARD_PAL_839_112_2_18_1_52_40_346 "SDIO_Card error = 0x%x"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MS_REF_SCP_TRC)
TRACE_MSG(CARD_SDIO_832_112_2_18_1_52_28_0,"SD20 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(CARD_SDIO_839_112_2_18_1_52_28_1,"CARD_SD_V2_0_HIGHCAP:")
TRACE_MSG(CARD_SDIO_840_112_2_18_1_52_28_2,"CSD_STRUCTURE = %x")
TRACE_MSG(CARD_SDIO_841_112_2_18_1_52_28_3,"reserved1= %x")
TRACE_MSG(CARD_SDIO_842_112_2_18_1_52_28_4,"TAAC= %x")
TRACE_MSG(CARD_SDIO_843_112_2_18_1_52_28_5,"NSAC= %x")
TRACE_MSG(CARD_SDIO_844_112_2_18_1_52_28_6,"TRAN_SPEED= %x")
TRACE_MSG(CARD_SDIO_845_112_2_18_1_52_28_7,"CCC= %x")
TRACE_MSG(CARD_SDIO_846_112_2_18_1_52_28_8,"READ_BL_LEN= %x")
TRACE_MSG(CARD_SDIO_847_112_2_18_1_52_28_9,"READ_BL_PARTIAL= %x")
TRACE_MSG(CARD_SDIO_848_112_2_18_1_52_28_10,"WRITE_BLK_MISALIGN= %x")
TRACE_MSG(CARD_SDIO_849_112_2_18_1_52_28_11,"READ_BLK_MISALIGN= %x")
TRACE_MSG(CARD_SDIO_850_112_2_18_1_52_28_12,"DSR_IMP= %x")
TRACE_MSG(CARD_SDIO_852_112_2_18_1_52_28_13,"reserved2= %x")
TRACE_MSG(CARD_SDIO_853_112_2_18_1_52_28_14,"C_SIZE= %x")
TRACE_MSG(CARD_SDIO_854_112_2_18_1_52_28_15,"reserved3= %x")
TRACE_MSG(CARD_SDIO_856_112_2_18_1_52_28_16,"ERASE_BLK_EN= %x")
TRACE_MSG(CARD_SDIO_857_112_2_18_1_52_28_17,"SECTOR_SIZE= %x")
TRACE_MSG(CARD_SDIO_858_112_2_18_1_52_28_18,"WP_GRP_SIZE= %x")
TRACE_MSG(CARD_SDIO_859_112_2_18_1_52_28_19,"WP_GRP_ENABLE= %x")
TRACE_MSG(CARD_SDIO_860_112_2_18_1_52_28_20,"reserved4= %x")
TRACE_MSG(CARD_SDIO_862_112_2_18_1_52_28_21,"R2W_FACTOR= %x")
TRACE_MSG(CARD_SDIO_863_112_2_18_1_52_28_22,"WRITE_BL_LEN= %x")
TRACE_MSG(CARD_SDIO_864_112_2_18_1_52_28_23,"WRITE_BL_PARTIAL= %x")
TRACE_MSG(CARD_SDIO_865_112_2_18_1_52_28_24,"reserved5= %x")
TRACE_MSG(CARD_SDIO_867_112_2_18_1_52_28_25,"FILE_FORMAT_GRP= %x")
TRACE_MSG(CARD_SDIO_868_112_2_18_1_52_28_26,"COPY= %x")
TRACE_MSG(CARD_SDIO_869_112_2_18_1_52_28_27,"PERM_WRITE_PROTECT= %x")
TRACE_MSG(CARD_SDIO_870_112_2_18_1_52_28_28,"TMP_WRITE_PROTECT= %x")
TRACE_MSG(CARD_SDIO_871_112_2_18_1_52_28_29,"FILE_FORMAT= %x")
TRACE_MSG(CARD_SDIO_872_112_2_18_1_52_28_30,"reserved6= %x")
TRACE_MSG(CARD_SDIO_884_112_2_18_1_52_28_31,"CARD_SD_V2_0_STANDARD||CARD_SD_V1_X:")
TRACE_MSG(CARD_SDIO_885_112_2_18_1_52_28_32,"CSD_STRUCTURE = %x")
TRACE_MSG(CARD_SDIO_886_112_2_18_1_52_28_33,"reserved1= %x")
TRACE_MSG(CARD_SDIO_887_112_2_18_1_52_28_34,"TAAC= %x")
TRACE_MSG(CARD_SDIO_888_112_2_18_1_52_28_35,"NSAC= %x")
TRACE_MSG(CARD_SDIO_889_112_2_18_1_52_28_36,"TRAN_SPEED= %x")
TRACE_MSG(CARD_SDIO_890_112_2_18_1_52_28_37,"CCC= %x")
TRACE_MSG(CARD_SDIO_891_112_2_18_1_52_28_38,"READ_BL_LEN= %x")
TRACE_MSG(CARD_SDIO_892_112_2_18_1_52_28_39,"READ_BL_PARTIAL= %x")
TRACE_MSG(CARD_SDIO_893_112_2_18_1_52_28_40,"WRITE_BLK_MISALIGN= %x")
TRACE_MSG(CARD_SDIO_894_112_2_18_1_52_28_41,"READ_BLK_MISALIGN= %x")
TRACE_MSG(CARD_SDIO_895_112_2_18_1_52_28_42,"DSR_IMP= %x")
TRACE_MSG(CARD_SDIO_897_112_2_18_1_52_28_43,"reserved2= %x")
TRACE_MSG(CARD_SDIO_898_112_2_18_1_52_28_44,"C_SIZE= %x")
TRACE_MSG(CARD_SDIO_899_112_2_18_1_52_28_45,"VDD_R_CURR_MIN = %x")
TRACE_MSG(CARD_SDIO_900_112_2_18_1_52_28_46,"VDD_R_CURR_MAX = %x")
TRACE_MSG(CARD_SDIO_901_112_2_18_1_52_28_47,"VDD_W_CURR_MIN = %x")
TRACE_MSG(CARD_SDIO_902_112_2_18_1_52_28_48,"VDD_W_CURR_MAX = %x")
TRACE_MSG(CARD_SDIO_903_112_2_18_1_52_28_49,"C_SIZE_MULT = %x")
TRACE_MSG(CARD_SDIO_905_112_2_18_1_52_28_50,"ERASE_BLK_EN= %x")
TRACE_MSG(CARD_SDIO_906_112_2_18_1_52_28_51,"SECTOR_SIZE= %x")
TRACE_MSG(CARD_SDIO_907_112_2_18_1_52_28_52,"WP_GRP_SIZE= %x")
TRACE_MSG(CARD_SDIO_908_112_2_18_1_52_28_53,"WP_GRP_ENABLE= %x")
TRACE_MSG(CARD_SDIO_909_112_2_18_1_52_28_54,"reserved3= %x")
TRACE_MSG(CARD_SDIO_911_112_2_18_1_52_28_55,"R2W_FACTOR= %x")
TRACE_MSG(CARD_SDIO_912_112_2_18_1_52_28_56,"WRITE_BL_LEN= %x")
TRACE_MSG(CARD_SDIO_913_112_2_18_1_52_28_57,"WRITE_BL_PARTIAL= %x")
TRACE_MSG(CARD_SDIO_914_112_2_18_1_52_28_58,"reserved4= %x")
TRACE_MSG(CARD_SDIO_916_112_2_18_1_52_28_59,"FILE_FORMAT_GRP= %x")
TRACE_MSG(CARD_SDIO_917_112_2_18_1_52_28_60,"COPY= %x")
TRACE_MSG(CARD_SDIO_918_112_2_18_1_52_28_61,"PERM_WRITE_PROTECT= %x")
TRACE_MSG(CARD_SDIO_919_112_2_18_1_52_28_62,"TMP_WRITE_PROTECT= %x")
TRACE_MSG(CARD_SDIO_920_112_2_18_1_52_28_63,"FILE_FORMAT= %x")
TRACE_MSG(CARD_SDIO_921_112_2_18_1_52_28_64,"reserved5= %x")
TRACE_MSG(CARD_SDIO_931_112_2_18_1_52_28_65,"CARD_MMC_331:")
TRACE_MSG(CARD_SDIO_932_112_2_18_1_52_28_66,"CSD_STRUCTURE\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_933_112_2_18_1_52_28_67,"SPEC_VERS\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_934_112_2_18_1_52_28_68,"reserved1\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_935_112_2_18_1_52_28_69,"TAAC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_936_112_2_18_1_52_28_70,"NSAC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_937_112_2_18_1_52_28_71,"TRAN_SPEED\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_938_112_2_18_1_52_28_72,"CCC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_939_112_2_18_1_52_28_73,"READ_BL_LEN\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_940_112_2_18_1_52_28_74,"READ_BL_PARTIAL\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_941_112_2_18_1_52_28_75,"WRITE_BLK_MISALIGN\t  = 0x%x")
TRACE_MSG(CARD_SDIO_942_112_2_18_1_52_28_76,"READ_BLK_MISALIGN\t  = 0x%x")
TRACE_MSG(CARD_SDIO_943_112_2_18_1_52_28_77,"DSR_IMP\t\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_944_112_2_18_1_52_28_78,"reserved2\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_945_112_2_18_1_52_28_79,"C_SIZE\t\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_946_112_2_18_1_52_28_80,"VDD_R_CURR_MIN\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_947_112_2_18_1_52_28_81,"VDD_R_CURR_MAX\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_948_112_2_18_1_52_28_82,"VDD_W_CURR_MIN\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_949_112_2_18_1_52_28_83,"VDD_W_CURR_MAX\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_950_112_2_18_1_52_28_84,"C_SIZE_MULT\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_951_112_2_18_1_52_28_85,"ERASE_GRP_SIZE\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_952_112_2_18_1_52_28_86,"ERASE_GRP_MULT\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_953_112_2_18_1_52_28_87,"WP_GRP_SIZE\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_954_112_2_18_1_52_28_88,"WP_GRP_ENABLE\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_955_112_2_18_1_52_28_89,"DEFAULT_ECC\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_956_112_2_18_1_52_28_90,"R2W_FACTOR\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_957_112_2_18_1_52_28_91,"WRITE_BL_LEN\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_958_112_2_18_1_52_28_92,"WRITE_BL_PARTIAL\t  = 0x%x")
TRACE_MSG(CARD_SDIO_959_112_2_18_1_52_28_93,"reserved3\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_960_112_2_18_1_52_28_94,"CONTENT_PROT_APP\t  = 0x%x")
TRACE_MSG(CARD_SDIO_961_112_2_18_1_52_28_95,"----RW bit---")
TRACE_MSG(CARD_SDIO_962_112_2_18_1_52_28_96,"FILE_FORMAT_GRP\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_963_112_2_18_1_52_28_97,"COPY\t\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_964_112_2_18_1_52_28_98,"PERM_WRITE_PROTECT\t  = 0x%x")
TRACE_MSG(CARD_SDIO_965_112_2_18_1_52_28_99,"TMP_WRITE_PROTECT\t  = 0x%x")
TRACE_MSG(CARD_SDIO_966_112_2_18_1_52_28_100,"FILE_FORMAT\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_967_112_2_18_1_52_28_101,"ECC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SDIO_1002_112_2_18_1_52_29_102,"SD20 %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x")
TRACE_MSG(CARD_SDIO_1011_112_2_18_1_52_29_103,"-----CID value---\r\n")
TRACE_MSG(CARD_SDIO_1012_112_2_18_1_52_29_104,"MID\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1013_112_2_18_1_52_29_105,"OID\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1014_112_2_18_1_52_29_106,"PNM\t\t = %c,%c,%c,%c,%c\r\n")
TRACE_MSG(CARD_SDIO_1015_112_2_18_1_52_29_107,"PRV\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1016_112_2_18_1_52_29_108,"PSN\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1017_112_2_18_1_52_29_109,"MDT\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1027_112_2_18_1_52_29_110,"-----MMC CID value---\r\n")
TRACE_MSG(CARD_SDIO_1028_112_2_18_1_52_29_111,"MID\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1029_112_2_18_1_52_29_112,"OID\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1030_112_2_18_1_52_29_113,"PNM\t\t = %c,%c,%c,%c,%c,%c\r\n")
TRACE_MSG(CARD_SDIO_1031_112_2_18_1_52_29_114,"PRV\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1032_112_2_18_1_52_29_115,"PSN\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1033_112_2_18_1_52_29_116,"MDT\t\t = 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1121_112_2_18_1_52_29_117,"SD20 RCA = %x")
TRACE_MSG(CARD_SDIO_1235_112_2_18_1_52_29_118,"SD20 Block Length of SD V2.0 High capacity is fixed to 512,you cannot set to value 0x%x\r\n")
TRACE_MSG(CARD_SDIO_1313_112_2_18_1_52_29_119,"currentVal = 0x%x")
TRACE_MSG(CARD_SDIO_1315_112_2_18_1_52_29_120,"grp6Supported = 0x%x")
TRACE_MSG(CARD_SDIO_1316_112_2_18_1_52_29_121,"grp5Supported = 0x%x")
TRACE_MSG(CARD_SDIO_1317_112_2_18_1_52_29_122,"grp4Supported = 0x%x")
TRACE_MSG(CARD_SDIO_1318_112_2_18_1_52_29_123,"grp3Supported = 0x%x")
TRACE_MSG(CARD_SDIO_1319_112_2_18_1_52_29_124,"grp2Supported = 0x%x")
TRACE_MSG(CARD_SDIO_1320_112_2_18_1_52_29_125,"grp1Supported = 0x%x")
TRACE_MSG(CARD_SDIO_1322_112_2_18_1_52_29_126,"grp6SwitchResult = 0x%x")
TRACE_MSG(CARD_SDIO_1323_112_2_18_1_52_29_127,"grp5SwitchResult = 0x%x")
TRACE_MSG(CARD_SDIO_1324_112_2_18_1_52_29_128,"grp4SwitchResult = 0x%x")
TRACE_MSG(CARD_SDIO_1325_112_2_18_1_52_29_129,"grp3SwitchResult = 0x%x")
TRACE_MSG(CARD_SDIO_1326_112_2_18_1_52_29_130,"grp2SwitchResult = 0x%x")
TRACE_MSG(CARD_SDIO_1327_112_2_18_1_52_29_131,"grp1SwitchResult = 0x%x")
TRACE_MSG(CARD_SDIO_1329_112_2_18_1_52_29_132,"status version = 0x%x")
TRACE_MSG(CARD_SDIO_1331_112_2_18_1_52_29_133,"grp6BusyStatus = 0x%x")
TRACE_MSG(CARD_SDIO_1332_112_2_18_1_52_29_134,"grp5BusyStatus = 0x%x")
TRACE_MSG(CARD_SDIO_1333_112_2_18_1_52_29_135,"grp4BusyStatus = 0x%x")
TRACE_MSG(CARD_SDIO_1334_112_2_18_1_52_29_136,"grp3BusyStatus = 0x%x")
TRACE_MSG(CARD_SDIO_1335_112_2_18_1_52_29_137,"grp2BusyStatus = 0x%x")
TRACE_MSG(CARD_SDIO_1336_112_2_18_1_52_29_138,"grp1BusyStatus = 0x%x")
TRACE_MSG(CARD_SDIO_1420_112_2_18_1_52_30_139,"SD20 enter high speed mode!!!")
TRACE_MSG(CARD_SDIO_1537_112_2_18_1_52_30_140,"SD20 %x,%x,%x,%x")
TRACE_MSG(CARD_SDIO_1548_112_2_18_1_52_30_141,"SD20 vertion_flag = %x")
TRACE_MSG(CARD_SDIO_1583_112_2_18_1_52_30_142,"SD20 %x,%x,%x,%x")
TRACE_MSG(CARD_SDIO_1642_112_2_18_1_52_30_143,"SD20 %x,%x,%x,%x")
TRACE_MSG(CARD_SDIO_1706_112_2_18_1_52_30_144,"SD20 type is %x")
TRACE_MSG(CARD_SDIO_1812_112_2_18_1_52_30_145,"[Card_sdio] CARD_SDIO_InitCard: Disable HighSpeed Mode !")
TRACE_MSG(CARD_SDIO_1871_112_2_18_1_52_31_146,"[Card_sdio] _IsCardReady timeout !")
TRACE_MSG(CARD_SDIO_2367_112_2_18_1_52_32_147,"card exit mcex success")
TRACE_MSG(CARD_SPI_528_112_2_18_1_52_33_148,"card Read busy 1B signal timeout")
TRACE_MSG(CARD_SPI_536_112_2_18_1_52_33_149,"card Read busy 1B signal timeout")
TRACE_MSG(CARD_SPI_630_112_2_18_1_52_33_150,"card Read busy signal timeout")
TRACE_MSG(CARD_SPI_668_112_2_18_1_52_33_151,"card CRC read error")
TRACE_MSG(CARD_SPI_737_112_2_18_1_52_33_152,"card write error1 0x%x")
TRACE_MSG(CARD_SPI_762_112_2_18_1_52_33_153,"card write error1 0x%x")
TRACE_MSG(CARD_SPI_777_112_2_18_1_52_33_154,"card write busy signal timeout")
TRACE_MSG(CARD_SPI_1377_112_2_18_1_52_34_155,"Block Length of SD V2.0 High capacity is fixed to 512,you cannot set to value 0x%x")
TRACE_MSG(CARD_SPI_1491_112_2_18_1_52_35_156,"Response = %x")
TRACE_MSG(CARD_SPI_1493_112_2_18_1_52_35_157,"-----CID value---")
TRACE_MSG(CARD_SPI_1494_112_2_18_1_52_35_158,"MID\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1495_112_2_18_1_52_35_159,"OID\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1496_112_2_18_1_52_35_160,"PNM\t\t = %c,%c,%c,%c,%c")
TRACE_MSG(CARD_SPI_1497_112_2_18_1_52_35_161,"PRV\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1498_112_2_18_1_52_35_162,"PSN\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1499_112_2_18_1_52_35_163,"MDT\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1500_112_2_18_1_52_35_164,"CRC\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1501_112_2_18_1_52_35_165,"lastBit\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1506_112_2_18_1_52_35_166,"Response = %x")
TRACE_MSG(CARD_SPI_1508_112_2_18_1_52_35_167,"-----CID value---")
TRACE_MSG(CARD_SPI_1509_112_2_18_1_52_35_168,"MID\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1510_112_2_18_1_52_35_169,"OID\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1511_112_2_18_1_52_35_170,"PNM\t\t = %c,%c,%c,%c,%c,%c")
TRACE_MSG(CARD_SPI_1512_112_2_18_1_52_35_171,"PRV\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1513_112_2_18_1_52_35_172,"PSN\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1514_112_2_18_1_52_35_173,"MDT\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1515_112_2_18_1_52_35_174,"CRC\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1516_112_2_18_1_52_35_175,"lastBit\t\t = 0x%x")
TRACE_MSG(CARD_SPI_1746_112_2_18_1_52_35_176,"Response = %x")
TRACE_MSG(CARD_SPI_1748_112_2_18_1_52_35_177,"CSD_STRUCTURE = %x")
TRACE_MSG(CARD_SPI_1749_112_2_18_1_52_35_178,"reserved1= %x")
TRACE_MSG(CARD_SPI_1750_112_2_18_1_52_35_179,"TAAC= %x")
TRACE_MSG(CARD_SPI_1751_112_2_18_1_52_35_180,"NSAC= %x")
TRACE_MSG(CARD_SPI_1752_112_2_18_1_52_35_181,"TRAN_SPEED= %x")
TRACE_MSG(CARD_SPI_1753_112_2_18_1_52_35_182,"CCC= %x")
TRACE_MSG(CARD_SPI_1754_112_2_18_1_52_35_183,"READ_BL_LEN= %x")
TRACE_MSG(CARD_SPI_1755_112_2_18_1_52_35_184,"READ_BL_PARTIAL= %x")
TRACE_MSG(CARD_SPI_1756_112_2_18_1_52_35_185,"WRITE_BLK_MISALIGN= %x")
TRACE_MSG(CARD_SPI_1757_112_2_18_1_52_35_186,"READ_BLK_MISALIGN= %x")
TRACE_MSG(CARD_SPI_1758_112_2_18_1_52_35_187,"DSR_IMP= %x")
TRACE_MSG(CARD_SPI_1760_112_2_18_1_52_35_188,"reserved2= %x")
TRACE_MSG(CARD_SPI_1761_112_2_18_1_52_35_189,"C_SIZE= %x")
TRACE_MSG(CARD_SPI_1762_112_2_18_1_52_35_190,"reserved3= %x")
TRACE_MSG(CARD_SPI_1764_112_2_18_1_52_35_191,"ERASE_BLK_EN= %x")
TRACE_MSG(CARD_SPI_1765_112_2_18_1_52_35_192,"SECTOR_SIZE= %x")
TRACE_MSG(CARD_SPI_1766_112_2_18_1_52_35_193,"WP_GRP_SIZE= %x")
TRACE_MSG(CARD_SPI_1767_112_2_18_1_52_35_194,"WP_GRP_ENABLE= %x")
TRACE_MSG(CARD_SPI_1768_112_2_18_1_52_35_195,"reserved4= %x")
TRACE_MSG(CARD_SPI_1770_112_2_18_1_52_35_196,"R2W_FACTOR= %x")
TRACE_MSG(CARD_SPI_1771_112_2_18_1_52_35_197,"WRITE_BL_LEN= %x")
TRACE_MSG(CARD_SPI_1772_112_2_18_1_52_35_198,"WRITE_BL_PARTIAL= %x")
TRACE_MSG(CARD_SPI_1773_112_2_18_1_52_35_199,"reserved5= %x")
TRACE_MSG(CARD_SPI_1775_112_2_18_1_52_35_200,"FILE_FORMAT_GRP= %x")
TRACE_MSG(CARD_SPI_1776_112_2_18_1_52_35_201,"COPY= %x")
TRACE_MSG(CARD_SPI_1777_112_2_18_1_52_35_202,"PERM_WRITE_PROTECT= %x")
TRACE_MSG(CARD_SPI_1778_112_2_18_1_52_35_203,"TMP_WRITE_PROTECT= %x")
TRACE_MSG(CARD_SPI_1779_112_2_18_1_52_35_204,"FILE_FORMAT= %x")
TRACE_MSG(CARD_SPI_1780_112_2_18_1_52_35_205,"reserved6= %x")
TRACE_MSG(CARD_SPI_1781_112_2_18_1_52_35_206,"CRC= %x")
TRACE_MSG(CARD_SPI_1782_112_2_18_1_52_35_207,"LastBit= %x")
TRACE_MSG(CARD_SPI_1783_112_2_18_1_52_35_208,"---------")
TRACE_MSG(CARD_SPI_1796_112_2_18_1_52_35_209,"Response = %x")
TRACE_MSG(CARD_SPI_1798_112_2_18_1_52_35_210,"CSD_STRUCTURE = %x")
TRACE_MSG(CARD_SPI_1799_112_2_18_1_52_35_211,"reserved1= %x")
TRACE_MSG(CARD_SPI_1800_112_2_18_1_52_35_212,"TAAC= %x")
TRACE_MSG(CARD_SPI_1801_112_2_18_1_52_35_213,"NSAC= %x")
TRACE_MSG(CARD_SPI_1802_112_2_18_1_52_35_214,"TRAN_SPEED= %x")
TRACE_MSG(CARD_SPI_1803_112_2_18_1_52_35_215,"CCC= %x")
TRACE_MSG(CARD_SPI_1804_112_2_18_1_52_35_216,"READ_BL_LEN= %x")
TRACE_MSG(CARD_SPI_1805_112_2_18_1_52_35_217,"READ_BL_PARTIAL= %x")
TRACE_MSG(CARD_SPI_1806_112_2_18_1_52_35_218,"WRITE_BLK_MISALIGN= %x")
TRACE_MSG(CARD_SPI_1807_112_2_18_1_52_35_219,"READ_BLK_MISALIGN= %x")
TRACE_MSG(CARD_SPI_1808_112_2_18_1_52_35_220,"DSR_IMP= %x")
TRACE_MSG(CARD_SPI_1810_112_2_18_1_52_35_221,"reserved2= %x")
TRACE_MSG(CARD_SPI_1811_112_2_18_1_52_35_222,"C_SIZE= %x")
TRACE_MSG(CARD_SPI_1812_112_2_18_1_52_35_223,"VDD_R_CURR_MIN = %x")
TRACE_MSG(CARD_SPI_1813_112_2_18_1_52_35_224,"VDD_R_CURR_MAX = %x")
TRACE_MSG(CARD_SPI_1814_112_2_18_1_52_35_225,"VDD_W_CURR_MIN = %x")
TRACE_MSG(CARD_SPI_1815_112_2_18_1_52_35_226,"VDD_W_CURR_MAX = %x")
TRACE_MSG(CARD_SPI_1816_112_2_18_1_52_35_227,"C_SIZE_MULT = %x")
TRACE_MSG(CARD_SPI_1818_112_2_18_1_52_35_228,"ERASE_BLK_EN= %x")
TRACE_MSG(CARD_SPI_1819_112_2_18_1_52_35_229,"SECTOR_SIZE= %x")
TRACE_MSG(CARD_SPI_1820_112_2_18_1_52_35_230,"WP_GRP_SIZE= %x")
TRACE_MSG(CARD_SPI_1821_112_2_18_1_52_35_231,"WP_GRP_ENABLE= %x")
TRACE_MSG(CARD_SPI_1822_112_2_18_1_52_35_232,"reserved3= %x")
TRACE_MSG(CARD_SPI_1824_112_2_18_1_52_35_233,"R2W_FACTOR= %x")
TRACE_MSG(CARD_SPI_1825_112_2_18_1_52_35_234,"WRITE_BL_LEN= %x")
TRACE_MSG(CARD_SPI_1826_112_2_18_1_52_35_235,"WRITE_BL_PARTIAL= %x")
TRACE_MSG(CARD_SPI_1827_112_2_18_1_52_35_236,"reserved4= %x")
TRACE_MSG(CARD_SPI_1829_112_2_18_1_52_35_237,"FILE_FORMAT_GRP= %x")
TRACE_MSG(CARD_SPI_1830_112_2_18_1_52_35_238,"COPY= %x")
TRACE_MSG(CARD_SPI_1831_112_2_18_1_52_35_239,"PERM_WRITE_PROTECT= %x")
TRACE_MSG(CARD_SPI_1832_112_2_18_1_52_35_240,"TMP_WRITE_PROTECT= %x")
TRACE_MSG(CARD_SPI_1833_112_2_18_1_52_35_241,"FILE_FORMAT= %x")
TRACE_MSG(CARD_SPI_1834_112_2_18_1_52_35_242,"reserved5= %x")
TRACE_MSG(CARD_SPI_1835_112_2_18_1_52_35_243,"CRC= %x")
TRACE_MSG(CARD_SPI_1836_112_2_18_1_52_35_244,"LastBit= %x")
TRACE_MSG(CARD_SPI_1837_112_2_18_1_52_35_245,"---------")
TRACE_MSG(CARD_SPI_1848_112_2_18_1_52_35_246,"Response = %x")
TRACE_MSG(CARD_SPI_1850_112_2_18_1_52_35_247,"----Read only bit---")
TRACE_MSG(CARD_SPI_1851_112_2_18_1_52_35_248,"CSD_STRUCTURE\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1852_112_2_18_1_52_35_249,"SPEC_VERS\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1853_112_2_18_1_52_35_250,"reserved1\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1854_112_2_18_1_52_35_251,"TAAC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1855_112_2_18_1_52_35_252,"NSAC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1856_112_2_18_1_52_35_253,"TRAN_SPEED\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1857_112_2_18_1_52_35_254,"CCC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1858_112_2_18_1_52_35_255,"READ_BL_LEN\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1859_112_2_18_1_52_35_256,"READ_BL_PARTIAL\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1860_112_2_18_1_52_35_257,"WRITE_BLK_MISALIGN\t  = 0x%x")
TRACE_MSG(CARD_SPI_1861_112_2_18_1_52_35_258,"READ_BLK_MISALIGN\t  = 0x%x")
TRACE_MSG(CARD_SPI_1862_112_2_18_1_52_35_259,"DSR_IMP\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1863_112_2_18_1_52_35_260,"reserved2\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1864_112_2_18_1_52_35_261,"C_SIZE\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1865_112_2_18_1_52_35_262,"VDD_R_CURR_MIN\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1866_112_2_18_1_52_35_263,"VDD_R_CURR_MAX\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1867_112_2_18_1_52_35_264,"VDD_W_CURR_MIN\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1868_112_2_18_1_52_35_265,"VDD_W_CURR_MAX\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1869_112_2_18_1_52_35_266,"C_SIZE_MULT\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1870_112_2_18_1_52_35_267,"ERASE_GRP_SIZE\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1871_112_2_18_1_52_35_268,"ERASE_GRP_MULT\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1872_112_2_18_1_52_35_269,"WP_GRP_SIZE\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1873_112_2_18_1_52_35_270,"WP_GRP_ENABLE\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1874_112_2_18_1_52_35_271,"DEFAULT_ECC\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1875_112_2_18_1_52_35_272,"R2W_FACTOR\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1876_112_2_18_1_52_35_273,"WRITE_BL_LEN\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1877_112_2_18_1_52_35_274,"WRITE_BL_PARTIAL\t  = 0x%x")
TRACE_MSG(CARD_SPI_1878_112_2_18_1_52_35_275,"reserved3\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1879_112_2_18_1_52_35_276,"CONTENT_PROT_APP\t  = 0x%x")
TRACE_MSG(CARD_SPI_1880_112_2_18_1_52_35_277,"----RW bit---")
TRACE_MSG(CARD_SPI_1881_112_2_18_1_52_35_278,"FILE_FORMAT_GRP\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1882_112_2_18_1_52_35_279,"COPY\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1883_112_2_18_1_52_35_280,"PERM_WRITE_PROTECT\t  = 0x%x")
TRACE_MSG(CARD_SPI_1884_112_2_18_1_52_35_281,"TMP_WRITE_PROTECT\t  = 0x%x")
TRACE_MSG(CARD_SPI_1885_112_2_18_1_52_35_282,"FILE_FORMAT\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1886_112_2_18_1_52_35_283,"ECC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1887_112_2_18_1_52_35_284,"CRC\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1888_112_2_18_1_52_35_285,"LastBit\t\t\t  = 0x%x")
TRACE_MSG(CARD_SPI_1889_112_2_18_1_52_35_286,"---------")
TRACE_MSG(CARD_SPI_1900_112_2_18_1_52_35_287,"Nac_Max = 0x%x %x")
TRACE_MSG(CARD_SPI_1901_112_2_18_1_52_35_288,"Nac_T = 0x%x")
TRACE_MSG(CARD_SPI_1902_112_2_18_1_52_35_289,"clock 0x%x")
TRACE_MSG(CARD_SPI_1972_112_2_18_1_52_35_290,"currentVal = 0x%x")
TRACE_MSG(CARD_SPI_1974_112_2_18_1_52_35_291,"grp6Supported = 0x%x")
TRACE_MSG(CARD_SPI_1975_112_2_18_1_52_35_292,"grp5Supported = 0x%x")
TRACE_MSG(CARD_SPI_1976_112_2_18_1_52_35_293,"grp4Supported = 0x%x")
TRACE_MSG(CARD_SPI_1977_112_2_18_1_52_35_294,"grp3Supported = 0x%x")
TRACE_MSG(CARD_SPI_1978_112_2_18_1_52_35_295,"grp2Supported = 0x%x")
TRACE_MSG(CARD_SPI_1979_112_2_18_1_52_35_296,"grp1Supported = 0x%x")
TRACE_MSG(CARD_SPI_1981_112_2_18_1_52_35_297,"grp6SwitchResult = 0x%x")
TRACE_MSG(CARD_SPI_1982_112_2_18_1_52_35_298,"grp5SwitchResult = 0x%x")
TRACE_MSG(CARD_SPI_1983_112_2_18_1_52_35_299,"grp4SwitchResult = 0x%x")
TRACE_MSG(CARD_SPI_1984_112_2_18_1_52_35_300,"grp3SwitchResult = 0x%x")
TRACE_MSG(CARD_SPI_1985_112_2_18_1_52_35_301,"grp2SwitchResult = 0x%x")
TRACE_MSG(CARD_SPI_1986_112_2_18_1_52_35_302,"grp1SwitchResult = 0x%x")
TRACE_MSG(CARD_SPI_1988_112_2_18_1_52_35_303,"status version = 0x%x")
TRACE_MSG(CARD_SPI_1990_112_2_18_1_52_36_304,"grp6BusyStatus = 0x%x")
TRACE_MSG(CARD_SPI_1991_112_2_18_1_52_36_305,"grp5BusyStatus = 0x%x")
TRACE_MSG(CARD_SPI_1992_112_2_18_1_52_36_306,"grp4BusyStatus = 0x%x")
TRACE_MSG(CARD_SPI_1993_112_2_18_1_52_36_307,"grp3BusyStatus = 0x%x")
TRACE_MSG(CARD_SPI_1994_112_2_18_1_52_36_308,"grp2BusyStatus = 0x%x")
TRACE_MSG(CARD_SPI_1995_112_2_18_1_52_36_309,"grp1BusyStatus = 0x%x")
TRACE_MSG(CARD_SPI_2080_112_2_18_1_52_36_310,"CARD_SPI_InitCard")
TRACE_MSG(CARD_SPI_2108_112_2_18_1_52_36_311,"Card waitting for CMD0's response is time out")
TRACE_MSG(CARD_SPI_2126_112_2_18_1_52_36_312,"SD2.0 not response to CMD8,SD2.0 not surpport current operation voltage range")
TRACE_MSG(CARD_SPI_2134_112_2_18_1_52_36_313,"not SD2.0 Card ,Maybe it is SD1.x Card or MMC card")
TRACE_MSG(CARD_SPI_2140_112_2_18_1_52_36_314,"VCA = 0, SD2.0 not surpport current operation voltage range")
TRACE_MSG(CARD_SPI_2145_112_2_18_1_52_36_315,"SD2.0 waitting for CMD8's response is time out")
TRACE_MSG(CARD_SPI_2164_112_2_18_1_52_36_316,"It maybe SD1.x or MMC But it is Not response to CMD58 ")
TRACE_MSG(CARD_SPI_2170_112_2_18_1_52_36_317,"It look CMD58 as illegal command so it is not SD card")
TRACE_MSG(CARD_SPI_2178_112_2_18_1_52_36_318,"OCR is 0x%x")
TRACE_MSG(CARD_SPI_2181_112_2_18_1_52_36_319,"SD 1.x But not surpport current voltage")
TRACE_MSG(CARD_SPI_2193_112_2_18_1_52_36_320,"Not SD card1")
TRACE_MSG(CARD_SPI_2199_112_2_18_1_52_36_321,"Not SD card2 , may be MMC")
TRACE_MSG(CARD_SPI_2205_112_2_18_1_52_36_322,"Not SD card3")
TRACE_MSG(CARD_SPI_2211_112_2_18_1_52_36_323,"Not SD card4 , may be MMC")
TRACE_MSG(CARD_SPI_2218_112_2_18_1_52_36_324,"It is SD1.x Card!!!")
TRACE_MSG(CARD_SPI_2224_112_2_18_1_52_36_325,"SD2.0 waitting for ACMD41's response is time out")
TRACE_MSG(CARD_SPI_2251_112_2_18_1_52_36_326,"MMC CMD0 response is time out")
TRACE_MSG(CARD_SPI_2272_112_2_18_1_52_36_327,"MMC CMD1 response is time out")
TRACE_MSG(CARD_SPI_2287_112_2_18_1_52_36_328,"It maybe SD2.0 But it is Not response to CMD58 ")
TRACE_MSG(CARD_SPI_2293_112_2_18_1_52_36_329,"It look CMD58 as illegal command so it is not SD card")
TRACE_MSG(CARD_SPI_2304_112_2_18_1_52_36_330,"SD 2.0 But not surpport current voltage")
TRACE_MSG(CARD_SPI_2315_112_2_18_1_52_36_331,"Not SD card5")
TRACE_MSG(CARD_SPI_2321_112_2_18_1_52_36_332,"Not SD card6")
TRACE_MSG(CARD_SPI_2327_112_2_18_1_52_36_333,"Not SD card7")
TRACE_MSG(CARD_SPI_2333_112_2_18_1_52_36_334,"Not SD card8")
TRACE_MSG(CARD_SPI_2339_112_2_18_1_52_36_335,"SD2.0 ACMD41's response is time out")
TRACE_MSG(CARD_SPI_2350_112_2_18_1_52_36_336,"It maybe SD2.0 But it is Not response to 2nd CMD58 ")
TRACE_MSG(CARD_SPI_2356_112_2_18_1_52_36_337,"It look 2nd CMD58 as illegal command so it is not SD card")
TRACE_MSG(CARD_SPI_2368_112_2_18_1_52_36_338,"It is SD2.0 high capacity Card!!!")
TRACE_MSG(CARD_SPI_2373_112_2_18_1_52_36_339,"It is SD2.0 standard capacity Card!!!")
TRACE_MSG(CARD_SPI_2387_112_2_18_1_52_36_340,"Turn on CRC fail 1")
TRACE_MSG(CARD_SPI_2393_112_2_18_1_52_36_341,"Turn on CRC fail 2")
TRACE_MSG(CARD_SPI_2727_112_2_18_1_52_37_342,"card write stop busy signal timeout")
TRACE_MSG(CARD_SPI_3523_112_2_18_1_52_38_343,"card exit mcex success")
TRACE_MSG(SDIO_CARD_PAL_223_112_2_18_1_52_39_344,"SCM Wait event error!!! eRet = 0x%x")
TRACE_MSG(SDIO_CARD_PAL_817_112_2_18_1_52_40_345,"SDIO_Card may be erase R1b is too long")
TRACE_MSG(SDIO_CARD_PAL_839_112_2_18_1_52_40_346,"SDIO_Card error = 0x%x")
END_TRACE_MAP(MS_REF_SCP_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MS_REF_SCP_TRC_H_


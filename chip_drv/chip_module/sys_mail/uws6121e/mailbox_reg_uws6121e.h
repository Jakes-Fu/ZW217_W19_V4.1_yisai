/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _CP_MAILBOX_H_
#define _CP_MAILBOX_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'cp_mailbox'."
#endif

#define REG_MAILBOX_BASE (0x51506000)

//#undef REG32
//#define REG32 volatile unsigned int
typedef volatile unsigned int REG32;


typedef volatile struct
{
    REG32                          INTGR0;                       //0x00000000
    REG32                          INTSTR0;                      //0x00000004
    REG32                          INTCR0;                       //0x00000008
    REG32                          INTMR0;                       //0x0000000C
    REG32                          INTSR0;                       //0x00000010
    REG32                          INTMSR0;                      //0x00000014
    REG32 Reserved_00000018[2];                 //0x00000018
    REG32                          INTGR1;                       //0x00000020
    REG32                          INTSTR1;                      //0x00000024
    REG32                          INTCR1;                       //0x00000028
    REG32                          INTMR1;                       //0x0000002C
    REG32                          INTSR1;                       //0x00000030
    REG32                          INTMSR1;                      //0x00000034
    REG32 Reserved_00000038[2];                 //0x00000038
    REG32                          INTGR2;                       //0x00000040
    REG32                          INTSTR2;                      //0x00000044
    REG32                          INTCR2;                       //0x00000048
    REG32                          INTMR2;                       //0x0000004C
    REG32                          INTSR2;                       //0x00000050
    REG32                          INTMSR2;                      //0x00000054
    REG32 Reserved_00000058[2];                 //0x00000058
    REG32                          INTGR3;                       //0x00000060
    REG32                          INTSTR3;                      //0x00000064
    REG32                          INTCR3;                       //0x00000068
    REG32                          INTMR3;                       //0x0000006C
    REG32                          INTSR3;                       //0x00000070
    REG32                          INTMSR3;                      //0x00000074
    REG32 Reserved_00000078[2];                 //0x00000078
    REG32                          INTGR4;                       //0x00000080
    REG32                          INTSTR4;                      //0x00000084
    REG32                          INTCR4;                       //0x00000088
    REG32                          INTMR4;                       //0x0000008C
    REG32                          INTSR4;                       //0x00000090
    REG32                          INTMSR4;                      //0x00000094
    REG32 Reserved_00000098[2];                 //0x00000098
    REG32                          INTGR5;                       //0x000000A0
    REG32                          INTSTR5;                      //0x000000A4
    REG32                          INTCR5;                       //0x000000A8
    REG32                          INTMR5;                       //0x000000AC
    REG32                          INTSR5;                       //0x000000B0
    REG32                          INTMSR5;                      //0x000000B4
    REG32 Reserved_000000B8[18];                //0x000000B8
    REG32                          SYSMAIL0;                     //0x00000100
    REG32                          SYSMAIL1;                     //0x00000104
    REG32                          SYSMAIL2;                     //0x00000108
    REG32                          SYSMAIL3;                     //0x0000010C
    REG32                          SYSMAIL4;                     //0x00000110
    REG32                          SYSMAIL5;                     //0x00000114
    REG32                          SYSMAIL6;                     //0x00000118
    REG32                          SYSMAIL7;                     //0x0000011C
    REG32                          SYSMAIL8;                     //0x00000120
    REG32                          SYSMAIL9;                     //0x00000124
    REG32                          SYSMAIL10;                    //0x00000128
    REG32                          SYSMAIL11;                    //0x0000012C
    REG32                          SYSMAIL12;                    //0x00000130
    REG32                          SYSMAIL13;                    //0x00000134
    REG32                          SYSMAIL14;                    //0x00000138
    REG32                          SYSMAIL15;                    //0x0000013C
    REG32                          SYSMAIL16;                    //0x00000140
    REG32                          SYSMAIL17;                    //0x00000144
    REG32                          SYSMAIL18;                    //0x00000148
    REG32                          SYSMAIL19;                    //0x0000014C
    REG32                          SYSMAIL20;                    //0x00000150
    REG32                          SYSMAIL21;                    //0x00000154
    REG32                          SYSMAIL22;                    //0x00000158
    REG32                          SYSMAIL23;                    //0x0000015C
    REG32                          SYSMAIL24;                    //0x00000160
    REG32                          SYSMAIL25;                    //0x00000164
    REG32                          SYSMAIL26;                    //0x00000168
    REG32                          SYSMAIL27;                    //0x0000016C
    REG32                          SYSMAIL28;                    //0x00000170
    REG32                          SYSMAIL29;                    //0x00000174
    REG32                          SYSMAIL30;                    //0x00000178
    REG32                          SYSMAIL31;                    //0x0000017C
    REG32 Reserved_00000180[32];                //0x00000180
    REG32                          SYSMAIL32;                    //0x00000200
    REG32                          SYSMAIL33;                    //0x00000204
    REG32                          SYSMAIL34;                    //0x00000208
    REG32                          SYSMAIL35;                    //0x0000020C
    REG32                          SYSMAIL36;                    //0x00000210
    REG32                          SYSMAIL37;                    //0x00000214
    REG32                          SYSMAIL38;                    //0x00000218
    REG32                          SYSMAIL39;                    //0x0000021C
    REG32                          SYSMAIL40;                    //0x00000220
    REG32                          SYSMAIL41;                    //0x00000224
    REG32                          SYSMAIL42;                    //0x00000228
    REG32                          SYSMAIL43;                    //0x0000022C
    REG32                          SYSMAIL44;                    //0x00000230
    REG32                          SYSMAIL45;                    //0x00000234
    REG32                          SYSMAIL46;                    //0x00000238
    REG32                          SYSMAIL47;                    //0x0000023C
    REG32                          SYSMAIL48;                    //0x00000240
    REG32                          SYSMAIL49;                    //0x00000244
    REG32                          SYSMAIL50;                    //0x00000248
    REG32                          SYSMAIL51;                    //0x0000024C
    REG32                          SYSMAIL52;                    //0x00000250
    REG32                          SYSMAIL53;                    //0x00000254
    REG32                          SYSMAIL54;                    //0x00000258
    REG32                          SYSMAIL55;                    //0x0000025C
    REG32                          SYSMAIL56;                    //0x00000260
    REG32                          SYSMAIL57;                    //0x00000264
    REG32                          SYSMAIL58;                    //0x00000268
    REG32                          SYSMAIL59;                    //0x0000026C
    REG32                          SYSMAIL60;                    //0x00000270
    REG32                          SYSMAIL61;                    //0x00000274
    REG32                          SYSMAIL62;                    //0x00000278
    REG32                          SYSMAIL63;                    //0x0000027C
    REG32 Reserved_00000280[32];                //0x00000280
    REG32                          SYSMAIL64;                    //0x00000300
    REG32                          SYSMAIL65;                    //0x00000304
    REG32                          SYSMAIL66;                    //0x00000308
    REG32                          SYSMAIL67;                    //0x0000030C
    REG32                          SYSMAIL68;                    //0x00000310
    REG32                          SYSMAIL69;                    //0x00000314
    REG32                          SYSMAIL70;                    //0x00000318
    REG32                          SYSMAIL71;                    //0x0000031C
    REG32                          SYSMAIL72;                    //0x00000320
    REG32                          SYSMAIL73;                    //0x00000324
    REG32                          SYSMAIL74;                    //0x00000328
    REG32                          SYSMAIL75;                    //0x0000032C
    REG32                          SYSMAIL76;                    //0x00000330
    REG32                          SYSMAIL77;                    //0x00000334
    REG32                          SYSMAIL78;                    //0x00000338
    REG32                          SYSMAIL79;                    //0x0000033C
    REG32                          SYSMAIL80;                    //0x00000340
    REG32                          SYSMAIL81;                    //0x00000344
    REG32                          SYSMAIL82;                    //0x00000348
    REG32                          SYSMAIL83;                    //0x0000034C
    REG32                          SYSMAIL84;                    //0x00000350
    REG32                          SYSMAIL85;                    //0x00000354
    REG32                          SYSMAIL86;                    //0x00000358
    REG32                          SYSMAIL87;                    //0x0000035C
    REG32                          SYSMAIL88;                    //0x00000360
    REG32                          SYSMAIL89;                    //0x00000364
    REG32                          SYSMAIL90;                    //0x00000368
    REG32                          SYSMAIL91;                    //0x0000036C
    REG32                          SYSMAIL92;                    //0x00000370
    REG32                          SYSMAIL93;                    //0x00000374
    REG32                          SYSMAIL94;                    //0x00000378
    REG32                          SYSMAIL95;                    //0x0000037C
    REG32                          SYSMAIL191;                   //0x0000067C
} HWP_CP_MAILBOX_T;

#define hwp_mailbox                ((HWP_CP_MAILBOX_T*) REG_ACCESS_ADDRESS(REG_MAILBOX_BASE))





#endif


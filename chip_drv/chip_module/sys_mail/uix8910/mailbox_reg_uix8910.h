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



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CP_MAILBOX_T
// -----------------------------------------------------------------------------
///
// =============================================================================
// #if defined(REG_ADDRESS_FOR_GGE)
// #define REG_MAILBOX_BASE           0x05082000
// #else
#if defined(CHIP_VER_UIX8910MPW)
#define REG_MAILBOX_BASE           0x50082000
#else
#define REG_MAILBOX_BASE           0x50104000
#endif
// #endif

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
    REG32 Reserved_00000380[32];                //0x00000380
    REG32                          SYSMAIL96;                    //0x00000400
    REG32                          SYSMAIL97;                    //0x00000404
    REG32                          SYSMAIL98;                    //0x00000408
    REG32                          SYSMAIL99;                    //0x0000040C
    REG32                          SYSMAIL100;                   //0x00000410
    REG32                          SYSMAIL101;                   //0x00000414
    REG32                          SYSMAIL102;                   //0x00000418
    REG32                          SYSMAIL103;                   //0x0000041C
    REG32                          SYSMAIL104;                   //0x00000420
    REG32                          SYSMAIL105;                   //0x00000424
    REG32                          SYSMAIL106;                   //0x00000428
    REG32                          SYSMAIL107;                   //0x0000042C
    REG32                          SYSMAIL108;                   //0x00000430
    REG32                          SYSMAIL109;                   //0x00000434
    REG32                          SYSMAIL110;                   //0x00000438
    REG32                          SYSMAIL111;                   //0x0000043C
    REG32                          SYSMAIL112;                   //0x00000440
    REG32                          SYSMAIL113;                   //0x00000444
    REG32                          SYSMAIL114;                   //0x00000448
    REG32                          SYSMAIL115;                   //0x0000044C
    REG32                          SYSMAIL116;                   //0x00000450
    REG32                          SYSMAIL117;                   //0x00000454
    REG32                          SYSMAIL118;                   //0x00000458
    REG32                          SYSMAIL119;                   //0x0000045C
    REG32                          SYSMAIL120;                   //0x00000460
    REG32                          SYSMAIL121;                   //0x00000464
    REG32                          SYSMAIL122;                   //0x00000468
    REG32                          SYSMAIL123;                   //0x0000046C
    REG32                          SYSMAIL124;                   //0x00000470
    REG32                          SYSMAIL125;                   //0x00000474
    REG32                          SYSMAIL126;                   //0x00000478
    REG32                          SYSMAIL127;                   //0x0000047C
    REG32 Reserved_00000480[32];                //0x00000480
    REG32                          SYSMAIL128;                   //0x00000500
    REG32                          SYSMAIL129;                   //0x00000504
    REG32                          SYSMAIL130;                   //0x00000508
    REG32                          SYSMAIL131;                   //0x0000050C
    REG32                          SYSMAIL132;                   //0x00000510
    REG32                          SYSMAIL133;                   //0x00000514
    REG32                          SYSMAIL134;                   //0x00000518
    REG32                          SYSMAIL135;                   //0x0000051C
    REG32                          SYSMAIL136;                   //0x00000520
    REG32                          SYSMAIL137;                   //0x00000524
    REG32                          SYSMAIL138;                   //0x00000528
    REG32                          SYSMAIL139;                   //0x0000052C
    REG32                          SYSMAIL140;                   //0x00000530
    REG32                          SYSMAIL141;                   //0x00000534
    REG32                          SYSMAIL142;                   //0x00000538
    REG32                          SYSMAIL143;                   //0x0000053C
    REG32                          SYSMAIL144;                   //0x00000540
    REG32                          SYSMAIL145;                   //0x00000544
    REG32                          SYSMAIL146;                   //0x00000548
    REG32                          SYSMAIL147;                   //0x0000054C
    REG32                          SYSMAIL148;                   //0x00000550
    REG32                          SYSMAIL149;                   //0x00000554
    REG32                          SYSMAIL150;                   //0x00000558
    REG32                          SYSMAIL151;                   //0x0000055C
    REG32                          SYSMAIL152;                   //0x00000560
    REG32                          SYSMAIL153;                   //0x00000564
    REG32                          SYSMAIL154;                   //0x00000568
    REG32                          SYSMAIL155;                   //0x0000056C
    REG32                          SYSMAIL156;                   //0x00000570
    REG32                          SYSMAIL157;                   //0x00000574
    REG32                          SYSMAIL158;                   //0x00000578
    REG32                          SYSMAIL159;                   //0x0000057C
    REG32 Reserved_00000580[32];                //0x00000580
    REG32                          SYSMAIL160;                   //0x00000600
    REG32                          SYSMAIL161;                   //0x00000604
    REG32                          SYSMAIL162;                   //0x00000608
    REG32                          SYSMAIL163;                   //0x0000060C
    REG32                          SYSMAIL164;                   //0x00000610
    REG32                          SYSMAIL165;                   //0x00000614
    REG32                          SYSMAIL166;                   //0x00000618
    REG32                          SYSMAIL167;                   //0x0000061C
    REG32                          SYSMAIL168;                   //0x00000620
    REG32                          SYSMAIL169;                   //0x00000624
    REG32                          SYSMAIL170;                   //0x00000628
    REG32                          SYSMAIL171;                   //0x0000062C
    REG32                          SYSMAIL172;                   //0x00000630
    REG32                          SYSMAIL173;                   //0x00000634
    REG32                          SYSMAIL174;                   //0x00000638
    REG32                          SYSMAIL175;                   //0x0000063C
    REG32                          SYSMAIL176;                   //0x00000640
    REG32                          SYSMAIL177;                   //0x00000644
    REG32                          SYSMAIL178;                   //0x00000648
    REG32                          SYSMAIL179;                   //0x0000064C
    REG32                          SYSMAIL180;                   //0x00000650
    REG32                          SYSMAIL181;                   //0x00000654
    REG32                          SYSMAIL182;                   //0x00000658
    REG32                          SYSMAIL183;                   //0x0000065C
    REG32                          SYSMAIL184;                   //0x00000660
    REG32                          SYSMAIL185;                   //0x00000664
    REG32                          SYSMAIL186;                   //0x00000668
    REG32                          SYSMAIL187;                   //0x0000066C
    REG32                          SYSMAIL188;                   //0x00000670
    REG32                          SYSMAIL189;                   //0x00000674
    REG32                          SYSMAIL190;                   //0x00000678
    REG32                          SYSMAIL191;                   //0x0000067C
} HWP_CP_MAILBOX_T;

#define hwp_mailbox                ((HWP_CP_MAILBOX_T*) REG_ACCESS_ADDRESS(REG_MAILBOX_BASE))


//INTGR0
#define CP_MAILBOX_INTERRUPT0GENERATE(n) (((n)&0xFFFFFFFF)<<0)

//INTSTR0
#define CP_MAILBOX_INTERRUPT0BITSET(n) (((n)&0xFFFFFFFF)<<0)

//INTCR0
#define CP_MAILBOX_INTERRUPT0BITCLEAN(n) (((n)&0xFFFFFFFF)<<0)

//INTMR0
#define CP_MAILBOX_INTERRUPT0MASK(n) (((n)&0xFFFFFFFF)<<0)

//INTSR0
#define CP_MAILBOX_INTERRUPT0STATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTMSR0
#define CP_MAILBOX_INTERRUPT0MASKSTATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTGR1
#define CP_MAILBOX_INTERRUPT1GENERATE(n) (((n)&0xFFFFFFFF)<<0)

//INTSTR1
#define CP_MAILBOX_INTERRUPT1BITSET(n) (((n)&0xFFFFFFFF)<<0)

//INTCR1
#define CP_MAILBOX_INTERRUPT1BITCLEAN(n) (((n)&0xFFFFFFFF)<<0)

//INTMR1
#define CP_MAILBOX_INTERRUPT1MASK(n) (((n)&0xFFFFFFFF)<<0)

//INTSR1
#define CP_MAILBOX_INTERRUPT1STATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTMSR1
#define CP_MAILBOX_INTERRUPT1MASKSTATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTGR2
#define CP_MAILBOX_INTERRUPT2GENERATE(n) (((n)&0xFFFFFFFF)<<0)

//INTSTR2
#define CP_MAILBOX_INTERRUPT2BITSET(n) (((n)&0xFFFFFFFF)<<0)

//INTCR2
#define CP_MAILBOX_INTERRUPT2BITCLEAN(n) (((n)&0xFFFFFFFF)<<0)

//INTMR2
#define CP_MAILBOX_INTERRUPT2MASK(n) (((n)&0xFFFFFFFF)<<0)

//INTSR2
#define CP_MAILBOX_INTERRUPT2STATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTMSR2
#define CP_MAILBOX_INTERRUPT2MASKSTATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTGR3
#define CP_MAILBOX_INTERRUPT3GENERATE(n) (((n)&0xFFFFFFFF)<<0)

//INTSTR3
#define CP_MAILBOX_INTERRUPT3BITSET(n) (((n)&0xFFFFFFFF)<<0)

//INTCR3
#define CP_MAILBOX_INTERRUPT3BITCLEAN(n) (((n)&0xFFFFFFFF)<<0)

//INTMR3
#define CP_MAILBOX_INTERRUPT3MASK(n) (((n)&0xFFFFFFFF)<<0)

//INTSR3
#define CP_MAILBOX_INTERRUPT3STATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTMSR3
#define CP_MAILBOX_INTERRUPT3MASKSTATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTGR4
#define CP_MAILBOX_INTERRUPT4GENERATE(n) (((n)&0xFFFFFFFF)<<0)

//INTSTR4
#define CP_MAILBOX_INTERRUPT4BITSET(n) (((n)&0xFFFFFFFF)<<0)

//INTCR4
#define CP_MAILBOX_INTERRUPT4BITCLEAN(n) (((n)&0xFFFFFFFF)<<0)

//INTMR4
#define CP_MAILBOX_INTERRUPT4MASK(n) (((n)&0xFFFFFFFF)<<0)

//INTSR4
#define CP_MAILBOX_INTERRUPT4STATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTMSR4
#define CP_MAILBOX_INTERRUPT4MASKSTATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTGR5
#define CP_MAILBOX_INTERRUPT5GENERATE(n) (((n)&0xFFFFFFFF)<<0)

//INTSTR5
#define CP_MAILBOX_INTERRUPT5BITSET(n) (((n)&0xFFFFFFFF)<<0)

//INTCR5
#define CP_MAILBOX_INTERRUPT5BITCLEAN(n) (((n)&0xFFFFFFFF)<<0)

//INTMR5
#define CP_MAILBOX_INTERRUPT5MASK(n) (((n)&0xFFFFFFFF)<<0)

//INTSR5
#define CP_MAILBOX_INTERRUPT5STATUS(n) (((n)&0xFFFFFFFF)<<0)

//INTMSR5
#define CP_MAILBOX_INTERRUPT5MASKSTATUS(n) (((n)&0xFFFFFFFF)<<0)

//SYSMAIL0
#define CP_MAILBOX_SYSMAIL0(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL1
#define CP_MAILBOX_SYSMAIL1(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL2
#define CP_MAILBOX_SYSMAIL2(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL3
#define CP_MAILBOX_SYSMAIL3(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL4
#define CP_MAILBOX_SYSMAIL4(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL5
#define CP_MAILBOX_SYSMAIL5(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL6
#define CP_MAILBOX_SYSMAIL6(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL7
#define CP_MAILBOX_SYSMAIL7(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL8
#define CP_MAILBOX_SYSMAIL8(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL9
#define CP_MAILBOX_SYSMAIL9(n)     (((n)&0xFFFFFFFF)<<0)

//SYSMAIL10
#define CP_MAILBOX_SYSMAIL10(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL11
#define CP_MAILBOX_SYSMAIL11(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL12
#define CP_MAILBOX_SYSMAIL12(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL13
#define CP_MAILBOX_SYSMAIL13(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL14
#define CP_MAILBOX_SYSMAIL14(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL15
#define CP_MAILBOX_SYSMAIL15(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL16
#define CP_MAILBOX_SYSMAIL16(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL17
#define CP_MAILBOX_SYSMAIL17(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL18
#define CP_MAILBOX_SYSMAIL18(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL19
#define CP_MAILBOX_SYSMAIL19(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL20
#define CP_MAILBOX_SYSMAIL20(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL21
#define CP_MAILBOX_SYSMAIL21(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL22
#define CP_MAILBOX_SYSMAIL22(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL23
#define CP_MAILBOX_SYSMAIL23(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL24
#define CP_MAILBOX_SYSMAIL24(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL25
#define CP_MAILBOX_SYSMAIL25(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL26
#define CP_MAILBOX_SYSMAIL26(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL27
#define CP_MAILBOX_SYSMAIL27(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL28
#define CP_MAILBOX_SYSMAIL28(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL29
#define CP_MAILBOX_SYSMAIL29(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL30
#define CP_MAILBOX_SYSMAIL30(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL31
#define CP_MAILBOX_SYSMAIL31(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL32
#define CP_MAILBOX_SYSMAIL32(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL33
#define CP_MAILBOX_SYSMAIL33(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL34
#define CP_MAILBOX_SYSMAIL34(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL35
#define CP_MAILBOX_SYSMAIL35(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL36
#define CP_MAILBOX_SYSMAIL36(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL37
#define CP_MAILBOX_SYSMAIL37(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL38
#define CP_MAILBOX_SYSMAIL38(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL39
#define CP_MAILBOX_SYSMAIL39(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL40
#define CP_MAILBOX_SYSMAIL40(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL41
#define CP_MAILBOX_SYSMAIL41(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL42
#define CP_MAILBOX_SYSMAIL42(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL43
#define CP_MAILBOX_SYSMAIL43(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL44
#define CP_MAILBOX_SYSMAIL44(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL45
#define CP_MAILBOX_SYSMAIL45(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL46
#define CP_MAILBOX_SYSMAIL46(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL47
#define CP_MAILBOX_SYSMAIL47(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL48
#define CP_MAILBOX_SYSMAIL48(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL49
#define CP_MAILBOX_SYSMAIL49(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL50
#define CP_MAILBOX_SYSMAIL50(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL51
#define CP_MAILBOX_SYSMAIL51(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL52
#define CP_MAILBOX_SYSMAIL52(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL53
#define CP_MAILBOX_SYSMAIL53(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL54
#define CP_MAILBOX_SYSMAIL54(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL55
#define CP_MAILBOX_SYSMAIL55(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL56
#define CP_MAILBOX_SYSMAIL56(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL57
#define CP_MAILBOX_SYSMAIL57(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL58
#define CP_MAILBOX_SYSMAIL58(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL59
#define CP_MAILBOX_SYSMAIL59(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL60
#define CP_MAILBOX_SYSMAIL60(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL61
#define CP_MAILBOX_SYSMAIL61(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL62
#define CP_MAILBOX_SYSMAIL62(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL63
#define CP_MAILBOX_SYSMAIL63(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL64
#define CP_MAILBOX_SYSMAIL64(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL65
#define CP_MAILBOX_SYSMAIL65(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL66
#define CP_MAILBOX_SYSMAIL66(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL67
#define CP_MAILBOX_SYSMAIL67(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL68
#define CP_MAILBOX_SYSMAIL68(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL69
#define CP_MAILBOX_SYSMAIL69(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL70
#define CP_MAILBOX_SYSMAIL70(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL71
#define CP_MAILBOX_SYSMAIL71(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL72
#define CP_MAILBOX_SYSMAIL72(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL73
#define CP_MAILBOX_SYSMAIL73(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL74
#define CP_MAILBOX_SYSMAIL74(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL75
#define CP_MAILBOX_SYSMAIL75(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL76
#define CP_MAILBOX_SYSMAIL76(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL77
#define CP_MAILBOX_SYSMAIL77(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL78
#define CP_MAILBOX_SYSMAIL78(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL79
#define CP_MAILBOX_SYSMAIL79(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL80
#define CP_MAILBOX_SYSMAIL80(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL81
#define CP_MAILBOX_SYSMAIL81(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL82
#define CP_MAILBOX_SYSMAIL82(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL83
#define CP_MAILBOX_SYSMAIL83(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL84
#define CP_MAILBOX_SYSMAIL84(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL85
#define CP_MAILBOX_SYSMAIL85(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL86
#define CP_MAILBOX_SYSMAIL86(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL87
#define CP_MAILBOX_SYSMAIL87(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL88
#define CP_MAILBOX_SYSMAIL88(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL89
#define CP_MAILBOX_SYSMAIL89(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL90
#define CP_MAILBOX_SYSMAIL90(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL91
#define CP_MAILBOX_SYSMAIL91(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL92
#define CP_MAILBOX_SYSMAIL92(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL93
#define CP_MAILBOX_SYSMAIL93(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL94
#define CP_MAILBOX_SYSMAIL94(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL95
#define CP_MAILBOX_SYSMAIL95(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL96
#define CP_MAILBOX_SYSMAIL96(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL97
#define CP_MAILBOX_SYSMAIL97(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL98
#define CP_MAILBOX_SYSMAIL98(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL99
#define CP_MAILBOX_SYSMAIL99(n)    (((n)&0xFFFFFFFF)<<0)

//SYSMAIL100
#define CP_MAILBOX_SYSMAIL100(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL101
#define CP_MAILBOX_SYSMAIL101(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL102
#define CP_MAILBOX_SYSMAIL102(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL103
#define CP_MAILBOX_SYSMAIL103(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL104
#define CP_MAILBOX_SYSMAIL104(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL105
#define CP_MAILBOX_SYSMAIL105(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL106
#define CP_MAILBOX_SYSMAIL106(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL107
#define CP_MAILBOX_SYSMAIL107(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL108
#define CP_MAILBOX_SYSMAIL108(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL109
#define CP_MAILBOX_SYSMAIL109(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL110
#define CP_MAILBOX_SYSMAIL110(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL111
#define CP_MAILBOX_SYSMAIL111(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL112
#define CP_MAILBOX_SYSMAIL112(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL113
#define CP_MAILBOX_SYSMAIL113(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL114
#define CP_MAILBOX_SYSMAIL114(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL115
#define CP_MAILBOX_SYSMAIL115(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL116
#define CP_MAILBOX_SYSMAIL116(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL117
#define CP_MAILBOX_SYSMAIL117(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL118
#define CP_MAILBOX_SYSMAIL118(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL119
#define CP_MAILBOX_SYSMAIL119(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL120
#define CP_MAILBOX_SYSMAIL120(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL121
#define CP_MAILBOX_SYSMAIL121(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL122
#define CP_MAILBOX_SYSMAIL122(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL123
#define CP_MAILBOX_SYSMAIL123(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL124
#define CP_MAILBOX_SYSMAIL124(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL125
#define CP_MAILBOX_SYSMAIL125(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL126
#define CP_MAILBOX_SYSMAIL126(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL127
#define CP_MAILBOX_SYSMAIL127(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL128
#define CP_MAILBOX_SYSMAIL128(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL129
#define CP_MAILBOX_SYSMAIL129(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL130
#define CP_MAILBOX_SYSMAIL130(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL131
#define CP_MAILBOX_SYSMAIL131(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL132
#define CP_MAILBOX_SYSMAIL132(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL133
#define CP_MAILBOX_SYSMAIL133(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL134
#define CP_MAILBOX_SYSMAIL134(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL135
#define CP_MAILBOX_SYSMAIL135(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL136
#define CP_MAILBOX_SYSMAIL136(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL137
#define CP_MAILBOX_SYSMAIL137(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL138
#define CP_MAILBOX_SYSMAIL138(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL139
#define CP_MAILBOX_SYSMAIL139(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL140
#define CP_MAILBOX_SYSMAIL140(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL141
#define CP_MAILBOX_SYSMAIL141(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL142
#define CP_MAILBOX_SYSMAIL142(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL143
#define CP_MAILBOX_SYSMAIL143(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL144
#define CP_MAILBOX_SYSMAIL144(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL145
#define CP_MAILBOX_SYSMAIL145(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL146
#define CP_MAILBOX_SYSMAIL146(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL147
#define CP_MAILBOX_SYSMAIL147(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL148
#define CP_MAILBOX_SYSMAIL148(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL149
#define CP_MAILBOX_SYSMAIL149(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL150
#define CP_MAILBOX_SYSMAIL150(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL151
#define CP_MAILBOX_SYSMAIL151(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL152
#define CP_MAILBOX_SYSMAIL152(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL153
#define CP_MAILBOX_SYSMAIL153(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL154
#define CP_MAILBOX_SYSMAIL154(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL155
#define CP_MAILBOX_SYSMAIL155(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL156
#define CP_MAILBOX_SYSMAIL156(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL157
#define CP_MAILBOX_SYSMAIL157(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL158
#define CP_MAILBOX_SYSMAIL158(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL159
#define CP_MAILBOX_SYSMAIL159(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL160
#define CP_MAILBOX_SYSMAIL160(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL161
#define CP_MAILBOX_SYSMAIL161(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL162
#define CP_MAILBOX_SYSMAIL162(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL163
#define CP_MAILBOX_SYSMAIL163(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL164
#define CP_MAILBOX_SYSMAIL164(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL165
#define CP_MAILBOX_SYSMAIL165(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL166
#define CP_MAILBOX_SYSMAIL166(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL167
#define CP_MAILBOX_SYSMAIL167(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL168
#define CP_MAILBOX_SYSMAIL168(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL169
#define CP_MAILBOX_SYSMAIL169(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL170
#define CP_MAILBOX_SYSMAIL170(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL171
#define CP_MAILBOX_SYSMAIL171(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL172
#define CP_MAILBOX_SYSMAIL172(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL173
#define CP_MAILBOX_SYSMAIL173(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL174
#define CP_MAILBOX_SYSMAIL174(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL175
#define CP_MAILBOX_SYSMAIL175(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL176
#define CP_MAILBOX_SYSMAIL176(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL177
#define CP_MAILBOX_SYSMAIL177(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL178
#define CP_MAILBOX_SYSMAIL178(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL179
#define CP_MAILBOX_SYSMAIL179(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL180
#define CP_MAILBOX_SYSMAIL180(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL181
#define CP_MAILBOX_SYSMAIL181(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL182
#define CP_MAILBOX_SYSMAIL182(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL183
#define CP_MAILBOX_SYSMAIL183(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL184
#define CP_MAILBOX_SYSMAIL184(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL185
#define CP_MAILBOX_SYSMAIL185(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL186
#define CP_MAILBOX_SYSMAIL186(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL187
#define CP_MAILBOX_SYSMAIL187(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL188
#define CP_MAILBOX_SYSMAIL188(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL189
#define CP_MAILBOX_SYSMAIL189(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL190
#define CP_MAILBOX_SYSMAIL190(n)   (((n)&0xFFFFFFFF)<<0)

//SYSMAIL191
#define CP_MAILBOX_SYSMAIL191(n)   (((n)&0xFFFFFFFF)<<0)





#endif


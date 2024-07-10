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


#ifndef _USBC_H_
#define _USBC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'usbc'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// USBC_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_USBC_BASE              0x09040000

typedef volatile struct
{
    REG32                          GOTGCTL;                      //0x00000000
    REG32                          GOTGINT;                      //0x00000004
    REG32                          GAHBCFG;                      //0x00000008
    REG32                          GUSBCFG;                      //0x0000000C
    REG32                          GRSTCTL;                      //0x00000010
    REG32                          GINTSTS;                      //0x00000014
    REG32                          GINTMSK;                      //0x00000018
    REG32                          GRXSTSR;                      //0x0000001C
    REG32                          GRXSTSP;                      //0x00000020
    REG32                          GRXFSIZ;                      //0x00000024
    REG32                          GNPTXFSIZ;                    //0x00000028
    REG32                          GNPTXSTS;                     //0x0000002C
    REG32 Reserved_00000030[2];                 //0x00000030
    REG32                          GGPIO;                        //0x00000038
    REG32                          GUID;                         //0x0000003C
    REG32                          GSNPSID;                      //0x00000040
    REG32                          GHWCFG1;                      //0x00000044
    REG32                          GHWCFG2;                      //0x00000048
    REG32                          GHWCFG3;                      //0x0000004C
    REG32                          GHWCFG4;                      //0x00000050
    REG32 Reserved_00000054;                    //0x00000054
    REG32                          GPWRDN;                       //0x00000058
    REG32                          GDFIFOCFG;                    //0x0000005C
    REG32                          GADPCTL;                      //0x00000060
    REG32 Reserved_00000064[39];                //0x00000064
    REG32                          HPTXFSIZ;                     //0x00000100
    REG32                          DIEPTXF1;                     //0x00000104
    REG32                          DIEPTXF2;                     //0x00000108
    REG32                          DIEPTXF3;                     //0x0000010C
    REG32                          DIEPTXF4;                     //0x00000110
    REG32                          DIEPTXF5;                     //0x00000114
    REG32                          DIEPTXF6;                     //0x00000118
    REG32                          DIEPTXF7;                     //0x0000011C
    REG32                          DIEPTXF8;                     //0x00000120
    REG32 Reserved_00000124[183];               //0x00000124
    REG32                          HCFG;                         //0x00000400
    REG32                          HFIR;                         //0x00000404
    REG32                          HFNUM;                        //0x00000408
    REG32 Reserved_0000040C;                    //0x0000040C
    REG32                          HPTXSTS;                      //0x00000410
    REG32                          HAINT;                        //0x00000414
    REG32                          HAINTMSK;                     //0x00000418
    REG32                          HFLBAddr;                     //0x0000041C
    REG32 Reserved_00000420[8];                 //0x00000420
    REG32                          HPRT;                         //0x00000440
    REG32 Reserved_00000444[47];                //0x00000444
    REG32                          HCCHAR0;                      //0x00000500
    REG32                          HCSPLT0;                      //0x00000504
    REG32                          HCINT0;                       //0x00000508
    REG32                          HCINTMSK0;                    //0x0000050C
    REG32                          HCTSIZ0;                      //0x00000510
    REG32                          HCDMA0;                       //0x00000514
    REG32 Reserved_00000518;                    //0x00000518
    REG32                          HCDMAB0;                      //0x0000051C
    REG32                          HCCHAR1;                      //0x00000520
    REG32                          HCSPLT1;                      //0x00000524
    REG32                          HCINT1;                       //0x00000528
    REG32                          HCINTMSK1;                    //0x0000052C
    REG32                          HCTSIZ1;                      //0x00000530
    REG32                          HCDMA1;                       //0x00000534
    REG32 Reserved_00000538;                    //0x00000538
    REG32                          HCDMAB1;                      //0x0000053C
    REG32                          HCCHAR2;                      //0x00000540
    REG32                          HCSPLT2;                      //0x00000544
    REG32                          HCINT2;                       //0x00000548
    REG32                          HCINTMSK2;                    //0x0000054C
    REG32                          HCTSIZ2;                      //0x00000550
    REG32                          HCDMA2;                       //0x00000554
    REG32 Reserved_00000558;                    //0x00000558
    REG32                          HCDMAB2;                      //0x0000055C
    REG32                          HCCHAR3;                      //0x00000560
    REG32                          HCSPLT3;                      //0x00000564
    REG32                          HCINT3;                       //0x00000568
    REG32                          HCINTMSK3;                    //0x0000056C
    REG32                          HCTSIZ3;                      //0x00000570
    REG32                          HCDMA3;                       //0x00000574
    REG32 Reserved_00000578;                    //0x00000578
    REG32                          HCDMAB3;                      //0x0000057C
    REG32                          HCCHAR4;                      //0x00000580
    REG32                          HCSPLT4;                      //0x00000584
    REG32                          HCINT4;                       //0x00000588
    REG32                          HCINTMSK4;                    //0x0000058C
    REG32                          HCTSIZ4;                      //0x00000590
    REG32                          HCDMA4;                       //0x00000594
    REG32 Reserved_00000598;                    //0x00000598
    REG32                          HCDMAB4;                      //0x0000059C
    REG32                          HCCHAR5;                      //0x000005A0
    REG32                          HCSPLT5;                      //0x000005A4
    REG32                          HCINT5;                       //0x000005A8
    REG32                          HCINTMSK5;                    //0x000005AC
    REG32                          HCTSIZ5;                      //0x000005B0
    REG32                          HCDMA5;                       //0x000005B4
    REG32 Reserved_000005B8;                    //0x000005B8
    REG32                          HCDMAB5;                      //0x000005BC
    REG32                          HCCHAR6;                      //0x000005C0
    REG32                          HCSPLT6;                      //0x000005C4
    REG32                          HCINT6;                       //0x000005C8
    REG32                          HCINTMSK6;                    //0x000005CC
    REG32                          HCTSIZ6;                      //0x000005D0
    REG32                          HCDMA6;                       //0x000005D4
    REG32 Reserved_000005D8;                    //0x000005D8
    REG32                          HCDMAB6;                      //0x000005DC
    REG32                          HCCHAR7;                      //0x000005E0
    REG32                          HCSPLT7;                      //0x000005E4
    REG32                          HCINT7;                       //0x000005E8
    REG32                          HCINTMSK7;                    //0x000005EC
    REG32                          HCTSIZ7;                      //0x000005F0
    REG32                          HCDMA7;                       //0x000005F4
    REG32 Reserved_000005F8;                    //0x000005F8
    REG32                          HCDMAB7;                      //0x000005FC
    REG32                          HCCHAR8;                      //0x00000600
    REG32                          HCSPLT8;                      //0x00000604
    REG32                          HCINT8;                       //0x00000608
    REG32                          HCINTMSK8;                    //0x0000060C
    REG32                          HCTSIZ8;                      //0x00000610
    REG32                          HCDMA8;                       //0x00000614
    REG32 Reserved_00000618;                    //0x00000618
    REG32                          HCDMAB8;                      //0x0000061C
    REG32                          HCCHAR9;                      //0x00000620
    REG32                          HCSPLT9;                      //0x00000624
    REG32                          HCINT9;                       //0x00000628
    REG32                          HCINTMSK9;                    //0x0000062C
    REG32                          HCTSIZ9;                      //0x00000630
    REG32                          HCDMA9;                       //0x00000634
    REG32 Reserved_00000638;                    //0x00000638
    REG32                          HCDMAB9;                      //0x0000063C
    REG32                          HCCHAR10;                     //0x00000640
    REG32                          HCSPLT10;                     //0x00000644
    REG32                          HCINT10;                      //0x00000648
    REG32                          HCINTMSK10;                   //0x0000064C
    REG32                          HCTSIZ10;                     //0x00000650
    REG32                          HCDMA10;                      //0x00000654
    REG32 Reserved_00000658;                    //0x00000658
    REG32                          HCDMAB10;                     //0x0000065C
    REG32                          HCCHAR11;                     //0x00000660
    REG32                          HCSPLT11;                     //0x00000664
    REG32                          HCINT11;                      //0x00000668
    REG32                          HCINTMSK11;                   //0x0000066C
    REG32                          HCTSIZ11;                     //0x00000670
    REG32                          HCDMA11;                      //0x00000674
    REG32 Reserved_00000678;                    //0x00000678
    REG32                          HCDMAB11;                     //0x0000067C
    REG32                          HCCHAR12;                     //0x00000680
    REG32                          HCSPLT12;                     //0x00000684
    REG32                          HCINT12;                      //0x00000688
    REG32                          HCINTMSK12;                   //0x0000068C
    REG32                          HCTSIZ12;                     //0x00000690
    REG32                          HCDMA12;                      //0x00000694
    REG32 Reserved_00000698;                    //0x00000698
    REG32                          HCDMAB12;                     //0x0000069C
    REG32                          HCCHAR13;                     //0x000006A0
    REG32                          HCSPLT13;                     //0x000006A4
    REG32                          HCINT13;                      //0x000006A8
    REG32                          HCINTMSK13;                   //0x000006AC
    REG32                          HCTSIZ13;                     //0x000006B0
    REG32                          HCDMA13;                      //0x000006B4
    REG32 Reserved_000006B8;                    //0x000006B8
    REG32                          HCDMAB13;                     //0x000006BC
    REG32                          HCCHAR14;                     //0x000006C0
    REG32                          HCSPLT14;                     //0x000006C4
    REG32                          HCINT14;                      //0x000006C8
    REG32                          HCINTMSK14;                   //0x000006CC
    REG32                          HCTSIZ14;                     //0x000006D0
    REG32                          HCDMA14;                      //0x000006D4
    REG32 Reserved_000006D8;                    //0x000006D8
    REG32                          HCDMAB14;                     //0x000006DC
    REG32                          HCCHAR15;                     //0x000006E0
    REG32                          HCSPLT15;                     //0x000006E4
    REG32                          HCINT15;                      //0x000006E8
    REG32                          HCINTMSK15;                   //0x000006EC
    REG32                          HCTSIZ15;                     //0x000006F0
    REG32                          HCDMA15;                      //0x000006F4
    REG32 Reserved_000006F8;                    //0x000006F8
    REG32                          HCDMAB15;                     //0x000006FC
    REG32 Reserved_00000700[64];                //0x00000700
    REG32                          DCFG;                         //0x00000800
    REG32                          DCTL;                         //0x00000804
    REG32                          DSTS;                         //0x00000808
    REG32 Reserved_0000080C;                    //0x0000080C
    REG32                          DIEPMSK;                      //0x00000810
    REG32                          DOEPMSK;                      //0x00000814
    REG32                          DAINT;                        //0x00000818
    REG32                          DAINTMSK;                     //0x0000081C
    REG32 Reserved_00000820[2];                 //0x00000820
    REG32                          DVBUSDIS;                     //0x00000828
    REG32                          DVBUSPULSE;                   //0x0000082C
    REG32                          DTHRCTL;                      //0x00000830
    REG32                          DIEPEMPMSK;                   //0x00000834
    REG32 Reserved_00000838[50];                //0x00000838
    REG32                          DIEPCTL0;                     //0x00000900
    REG32 Reserved_00000904;                    //0x00000904
    REG32                          DIEPINT0;                     //0x00000908
    REG32 Reserved_0000090C;                    //0x0000090C
    REG32                          DIEPTSIZ0;                    //0x00000910
    REG32                          DIEPDMA0;                     //0x00000914
    REG32                          DTXFSTS0;                     //0x00000918
    REG32                          DIEPDMAB0;                    //0x0000091C
    REG32                          DIEPCTL1;                     //0x00000920
    REG32 Reserved_00000924;                    //0x00000924
    REG32                          DIEPINT1;                     //0x00000928
    REG32 Reserved_0000092C;                    //0x0000092C
    REG32                          DIEPTSIZ1;                    //0x00000930
    REG32                          DIEPDMA1;                     //0x00000934
    REG32                          DTXFSTS1;                     //0x00000938
    REG32                          DIEPDMAB1;                    //0x0000093C
    REG32                          DIEPCTL2;                     //0x00000940
    REG32 Reserved_00000944;                    //0x00000944
    REG32                          DIEPINT2;                     //0x00000948
    REG32 Reserved_0000094C;                    //0x0000094C
    REG32                          DIEPTSIZ2;                    //0x00000950
    REG32                          DIEPDMA2;                     //0x00000954
    REG32                          DTXFSTS2;                     //0x00000958
    REG32                          DIEPDMAB2;                    //0x0000095C
    REG32                          DIEPCTL3;                     //0x00000960
    REG32 Reserved_00000964;                    //0x00000964
    REG32                          DIEPINT3;                     //0x00000968
    REG32 Reserved_0000096C;                    //0x0000096C
    REG32                          DIEPTSIZ3;                    //0x00000970
    REG32                          DIEPDMA3;                     //0x00000974
    REG32                          DTXFSTS3;                     //0x00000978
    REG32                          DIEPDMAB3;                    //0x0000097C
    REG32                          DIEPCTL4;                     //0x00000980
    REG32 Reserved_00000984;                    //0x00000984
    REG32                          DIEPINT4;                     //0x00000988
    REG32 Reserved_0000098C;                    //0x0000098C
    REG32                          DIEPTSIZ4;                    //0x00000990
    REG32                          DIEPDMA4;                     //0x00000994
    REG32                          DTXFSTS4;                     //0x00000998
    REG32                          DIEPDMAB4;                    //0x0000099C
    REG32                          DIEPCTL5;                     //0x000009A0
    REG32 Reserved_000009A4;                    //0x000009A4
    REG32                          DIEPINT5;                     //0x000009A8
    REG32 Reserved_000009AC;                    //0x000009AC
    REG32                          DIEPTSIZ5;                    //0x000009B0
    REG32                          DIEPDMA5;                     //0x000009B4
    REG32                          DTXFSTS5;                     //0x000009B8
    REG32                          DIEPDMAB5;                    //0x000009BC
    REG32                          DIEPCTL6;                     //0x000009C0
    REG32 Reserved_000009C4;                    //0x000009C4
    REG32                          DIEPINT6;                     //0x000009C8
    REG32 Reserved_000009CC;                    //0x000009CC
    REG32                          DIEPTSIZ6;                    //0x000009D0
    REG32                          DIEPDMA6;                     //0x000009D4
    REG32                          DTXFSTS6;                     //0x000009D8
    REG32                          DIEPDMAB6;                    //0x000009DC
    REG32                          DIEPCTL7;                     //0x000009E0
    REG32 Reserved_000009E4;                    //0x000009E4
    REG32                          DIEPINT7;                     //0x000009E8
    REG32 Reserved_000009EC;                    //0x000009EC
    REG32                          DIEPTSIZ7;                    //0x000009F0
    REG32                          DIEPDMA7;                     //0x000009F4
    REG32                          DTXFSTS7;                     //0x000009F8
    REG32                          DIEPDMAB7;                    //0x000009FC
    REG32                          DIEPCTL8;                     //0x00000A00
    REG32 Reserved_00000A04;                    //0x00000A04
    REG32                          DIEPINT8;                     //0x00000A08
    REG32 Reserved_00000A0C;                    //0x00000A0C
    REG32                          DIEPTSIZ8;                    //0x00000A10
    REG32                          DIEPDMA8;                     //0x00000A14
    REG32                          DTXFSTS8;                     //0x00000A18
    REG32                          DIEPDMAB8;                    //0x00000A1C
    REG32                          DIEPCTL9;                     //0x00000A20
    REG32 Reserved_00000A24;                    //0x00000A24
    REG32                          DIEPINT9;                     //0x00000A28
    REG32 Reserved_00000A2C;                    //0x00000A2C
    REG32                          DIEPTSIZ9;                    //0x00000A30
    REG32                          DIEPDMA9;                     //0x00000A34
    REG32                          DTXFSTS9;                     //0x00000A38
    REG32                          DIEPDMAB9;                    //0x00000A3C
    REG32                          DIEPCTL10;                    //0x00000A40
    REG32 Reserved_00000A44;                    //0x00000A44
    REG32                          DIEPINT10;                    //0x00000A48
    REG32 Reserved_00000A4C;                    //0x00000A4C
    REG32                          DIEPTSIZ10;                   //0x00000A50
    REG32                          DIEPDMA10;                    //0x00000A54
    REG32                          DTXFSTS10;                    //0x00000A58
    REG32                          DIEPDMAB10;                   //0x00000A5C
    REG32                          DIEPCTL11;                    //0x00000A60
    REG32 Reserved_00000A64;                    //0x00000A64
    REG32                          DIEPINT11;                    //0x00000A68
    REG32 Reserved_00000A6C;                    //0x00000A6C
    REG32                          DIEPTSIZ11;                   //0x00000A70
    REG32                          DIEPDMA11;                    //0x00000A74
    REG32                          DTXFSTS11;                    //0x00000A78
    REG32                          DIEPDMAB11;                   //0x00000A7C
    REG32                          DIEPCTL12;                    //0x00000A80
    REG32 Reserved_00000A84;                    //0x00000A84
    REG32                          DIEPINT12;                    //0x00000A88
    REG32 Reserved_00000A8C;                    //0x00000A8C
    REG32                          DIEPTSIZ12;                   //0x00000A90
    REG32                          DIEPDMA12;                    //0x00000A94
    REG32                          DTXFSTS12;                    //0x00000A98
    REG32                          DIEPDMAB12;                   //0x00000A9C
    REG32 Reserved_00000AA0[24];                //0x00000AA0
    REG32                          DOEPCTL0;                     //0x00000B00
    REG32 Reserved_00000B04;                    //0x00000B04
    REG32                          DOEPINT0;                     //0x00000B08
    REG32 Reserved_00000B0C;                    //0x00000B0C
    REG32                          DOEPTSIZ0;                    //0x00000B10
    REG32                          DOEPDMA0;                     //0x00000B14
    REG32 Reserved_00000B18;                    //0x00000B18
    REG32                          DOEPDMAB0;                    //0x00000B1C
    REG32                          DOEPCTL1;                     //0x00000B20
    REG32 Reserved_00000B24;                    //0x00000B24
    REG32                          DOEPINT1;                     //0x00000B28
    REG32 Reserved_00000B2C;                    //0x00000B2C
    REG32                          DOEPTSIZ1;                    //0x00000B30
    REG32                          DOEPDMA1;                     //0x00000B34
    REG32 Reserved_00000B38;                    //0x00000B38
    REG32                          DOEPDMAB1;                    //0x00000B3C
    REG32                          DOEPCTL2;                     //0x00000B40
    REG32 Reserved_00000B44;                    //0x00000B44
    REG32                          DOEPINT2;                     //0x00000B48
    REG32 Reserved_00000B4C;                    //0x00000B4C
    REG32                          DOEPTSIZ2;                    //0x00000B50
    REG32                          DOEPDMA2;                     //0x00000B54
    REG32 Reserved_00000B58;                    //0x00000B58
    REG32                          DOEPDMAB2;                    //0x00000B5C
    REG32                          DOEPCTL3;                     //0x00000B60
    REG32 Reserved_00000B64;                    //0x00000B64
    REG32                          DOEPINT3;                     //0x00000B68
    REG32 Reserved_00000B6C;                    //0x00000B6C
    REG32                          DOEPTSIZ3;                    //0x00000B70
    REG32                          DOEPDMA3;                     //0x00000B74
    REG32 Reserved_00000B78;                    //0x00000B78
    REG32                          DOEPDMAB3;                    //0x00000B7C
    REG32                          DOEPCTL4;                     //0x00000B80
    REG32 Reserved_00000B84;                    //0x00000B84
    REG32                          DOEPINT4;                     //0x00000B88
    REG32 Reserved_00000B8C;                    //0x00000B8C
    REG32                          DOEPTSIZ4;                    //0x00000B90
    REG32                          DOEPDMA4;                     //0x00000B94
    REG32 Reserved_00000B98;                    //0x00000B98
    REG32                          DOEPDMAB4;                    //0x00000B9C
    REG32                          DOEPCTL5;                     //0x00000BA0
    REG32 Reserved_00000BA4;                    //0x00000BA4
    REG32                          DOEPINT5;                     //0x00000BA8
    REG32 Reserved_00000BAC;                    //0x00000BAC
    REG32                          DOEPTSIZ5;                    //0x00000BB0
    REG32                          DOEPDMA5;                     //0x00000BB4
    REG32 Reserved_00000BB8;                    //0x00000BB8
    REG32                          DOEPDMAB5;                    //0x00000BBC
    REG32                          DOEPCTL6;                     //0x00000BC0
    REG32 Reserved_00000BC4;                    //0x00000BC4
    REG32                          DOEPINT6;                     //0x00000BC8
    REG32 Reserved_00000BCC;                    //0x00000BCC
    REG32                          DOEPTSIZ6;                    //0x00000BD0
    REG32                          DOEPDMA6;                     //0x00000BD4
    REG32 Reserved_00000BD8;                    //0x00000BD8
    REG32                          DOEPDMAB6;                    //0x00000BDC
    REG32                          DOEPCTL7;                     //0x00000BE0
    REG32 Reserved_00000BE4;                    //0x00000BE4
    REG32                          DOEPINT7;                     //0x00000BE8
    REG32 Reserved_00000BEC;                    //0x00000BEC
    REG32                          DOEPTSIZ7;                    //0x00000BF0
    REG32                          DOEPDMA7;                     //0x00000BF4
    REG32 Reserved_00000BF8;                    //0x00000BF8
    REG32                          DOEPDMAB7;                    //0x00000BFC
    REG32                          DOEPCTL8;                     //0x00000C00
    REG32 Reserved_00000C04;                    //0x00000C04
    REG32                          DOEPINT8;                     //0x00000C08
    REG32 Reserved_00000C0C;                    //0x00000C0C
    REG32                          DOEPTSIZ8;                    //0x00000C10
    REG32                          DOEPDMA8;                     //0x00000C14
    REG32 Reserved_00000C18;                    //0x00000C18
    REG32                          DOEPDMAB8;                    //0x00000C1C
    REG32                          DOEPCTL9;                     //0x00000C20
    REG32 Reserved_00000C24;                    //0x00000C24
    REG32                          DOEPINT9;                     //0x00000C28
    REG32 Reserved_00000C2C;                    //0x00000C2C
    REG32                          DOEPTSIZ9;                    //0x00000C30
    REG32                          DOEPDMA9;                     //0x00000C34
    REG32 Reserved_00000C38;                    //0x00000C38
    REG32                          DOEPDMAB9;                    //0x00000C3C
    REG32                          DOEPCTL10;                    //0x00000C40
    REG32 Reserved_00000C44;                    //0x00000C44
    REG32                          DOEPINT10;                    //0x00000C48
    REG32 Reserved_00000C4C;                    //0x00000C4C
    REG32                          DOEPTSIZ10;                   //0x00000C50
    REG32                          DOEPDMA10;                    //0x00000C54
    REG32 Reserved_00000C58;                    //0x00000C58
    REG32                          DOEPDMAB10;                   //0x00000C5C
    REG32                          DOEPCTL11;                    //0x00000C60
    REG32 Reserved_00000C64;                    //0x00000C64
    REG32                          DOEPINT11;                    //0x00000C68
    REG32 Reserved_00000C6C;                    //0x00000C6C
    REG32                          DOEPTSIZ11;                   //0x00000C70
    REG32                          DOEPDMA11;                    //0x00000C74
    REG32 Reserved_00000C78;                    //0x00000C78
    REG32                          DOEPDMAB11;                   //0x00000C7C
    REG32                          DOEPCTL12;                    //0x00000C80
    REG32 Reserved_00000C84;                    //0x00000C84
    REG32                          DOEPINT12;                    //0x00000C88
    REG32 Reserved_00000C8C;                    //0x00000C8C
    REG32                          DOEPTSIZ12;                   //0x00000C90
    REG32                          DOEPDMA12;                    //0x00000C94
    REG32 Reserved_00000C98;                    //0x00000C98
    REG32                          DOEPDMAB12;                   //0x00000C9C
    REG32 Reserved_00000CA0[88];                //0x00000CA0
    REG32                          PCGCCTL;                      //0x00000E00
} HWP_USBC_T;

#define hwp_usbc                   ((HWP_USBC_T*) REG_ACCESS_ADDRESS(REG_USBC_BASE))


//GOTGCTL
typedef union {
    REG32 v;
    struct {
        REG32 sesreqscs : 1; // [0], read only
        REG32 sesreq : 1; // [1]
        REG32 vbvalidoven : 1; // [2]
        REG32 vbvalidovval : 1; // [3]
        REG32 avalidoven : 1; // [4]
        REG32 avalidovval : 1; // [5]
        REG32 bvalidoven : 1; // [6]
        REG32 bvalidovval : 1; // [7]
        REG32 hstnegscs : 1; // [8], read only
        REG32 hnpreq : 1; // [9]
        REG32 hstsethnpen : 1; // [10]
        REG32 devhnpen : 1; // [11]
        REG32 ehen : 1; // [12]
        REG32 __14_13 : 2;
        REG32 dbncefltrbypass : 1; // [15]
        REG32 conidsts : 1; // [16], read only
        REG32 dbnctime : 1; // [17], read only
        REG32 asesvld : 1; // [18]
        REG32 bsesvld : 1; // [19], read only
        REG32 otgver : 1; // [20]
        REG32 curmod : 1; // [21], read only
        REG32 __31_22 : 10;
    } b;
} REG_USBC_GOTGCTL_T;

//GOTGINT
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 sesenddet : 1; // [2], write clear
        REG32 __7_3 : 5;
        REG32 sesreqsucstschng : 1; // [8], write clear
        REG32 hstnegsucstschng : 1; // [9], write clear
        REG32 __16_10 : 7;
        REG32 hstnegdet : 1; // [17], write clear
        REG32 adevtoutchg : 1; // [18], write clear
        REG32 dbncedone : 1; // [19], write clear
        REG32 __31_20 : 12;
    } b;
} REG_USBC_GOTGINT_T;

//GAHBCFG
typedef union {
    REG32 v;
    struct {
        REG32 glblintrmsk : 1; // [0]
        REG32 hbstlen : 4; // [4:1]
        REG32 dmaen : 1; // [5]
        REG32 __6_6 : 1;
        REG32 nptxfemplvl : 1; // [7]
        REG32 ptxfemplvl : 1; // [8]
        REG32 __20_9 : 12;
        REG32 remmemsupp : 1; // [21]
        REG32 notialldmawrit : 1; // [22]
        REG32 ahbsingle : 1; // [23]
        REG32 invdescendianess : 1; // [24]
        REG32 __31_25 : 7;
    } b;
} REG_USBC_GAHBCFG_T;

//GUSBCFG
typedef union {
    REG32 v;
    struct {
        REG32 toutcal : 3; // [2:0]
        REG32 phyif : 1; // [3], read only
        REG32 ulpi_utmi_sel : 1; // [4]
        REG32 fsintf : 1; // [5]
        REG32 physel : 1; // [6]
        REG32 __7_7 : 1;
        REG32 srpcap : 1; // [8]
        REG32 hnpcap : 1; // [9]
        REG32 usbtrdtim : 4; // [13:10]
        REG32 __14_14 : 1;
        REG32 phylpwrclksel : 1; // [15]
        REG32 __21_16 : 6;
        REG32 termseldlpulse : 1; // [22]
        REG32 __25_23 : 3;
        REG32 ic_usbcap : 1; // [26], read only
        REG32 __27_27 : 1;
        REG32 txenddelay : 1; // [28]
        REG32 forcehstmode : 1; // [29]
        REG32 forcedevmode : 1; // [30]
        REG32 corrupttxpkt : 1; // [31]
    } b;
} REG_USBC_GUSBCFG_T;

//GRSTCTL
typedef union {
    REG32 v;
    struct {
        REG32 csftrst : 1; // [0]
        REG32 piufssftrst : 1; // [1]
        REG32 frmcntrrst : 1; // [2]
        REG32 __3_3 : 1;
        REG32 rxfflsh : 1; // [4]
        REG32 txfflsh : 1; // [5]
        REG32 txfnum : 5; // [10:6]
        REG32 __29_11 : 19;
        REG32 dmareq : 1; // [30], read only
        REG32 ahbidle : 1; // [31], read only
    } b;
} REG_USBC_GRSTCTL_T;

//GINTSTS
typedef union {
    REG32 v;
    struct {
        REG32 curmod : 1; // [0], read only
        REG32 modemis : 1; // [1], write clear
        REG32 otgint : 1; // [2]
        REG32 sof : 1; // [3]
        REG32 rxflvl : 1; // [4], read only
        REG32 nptxfemp : 1; // [5], read only
        REG32 ginnakeff : 1; // [6], read only
        REG32 goutnakeff : 1; // [7], read only
        REG32 __9_8 : 2;
        REG32 erlysusp : 1; // [10], write clear
        REG32 usbsusp : 1; // [11], write clear
        REG32 usbrst : 1; // [12], write clear
        REG32 enumdone : 1; // [13], write clear
        REG32 isooutdrop : 1; // [14], write clear
        REG32 eopf : 1; // [15], write clear
        REG32 __16_16 : 1;
        REG32 epmis : 1; // [17], write clear
        REG32 iepint : 1; // [18], read only
        REG32 oepint : 1; // [19], read only
        REG32 incompisoin : 1; // [20], write clear
        REG32 incomplp : 1; // [21], write clear
        REG32 fetsusp : 1; // [22], write clear
        REG32 resetdet : 1; // [23], write clear
        REG32 prtint : 1; // [24], read only
        REG32 hchint : 1; // [25], read only
        REG32 ptxfemp : 1; // [26]
        REG32 __27_27 : 1;
        REG32 conidstschng : 1; // [28]
        REG32 disconnint : 1; // [29], write clear
        REG32 sessreqint : 1; // [30], write clear
        REG32 wkupint : 1; // [31], write clear
    } b;
} REG_USBC_GINTSTS_T;

//GINTMSK
typedef union {
    REG32 v;
    struct {
        REG32 __0_0 : 1;
        REG32 modemismsk : 1; // [1]
        REG32 otgintmsk : 1; // [2]
        REG32 sofmsk : 1; // [3]
        REG32 rxflvlmsk : 1; // [4]
        REG32 nptxfempmsk : 1; // [5]
        REG32 ginnakeffmsk : 1; // [6]
        REG32 goutnakeffmsk : 1; // [7]
        REG32 __9_8 : 2;
        REG32 erlysuspmsk : 1; // [10]
        REG32 usbsuspmsk : 1; // [11]
        REG32 usbrstmsk : 1; // [12]
        REG32 enumdonemsk : 1; // [13]
        REG32 isooutdropmsk : 1; // [14]
        REG32 eopfmsk : 1; // [15]
        REG32 __16_16 : 1;
        REG32 epmismsk : 1; // [17]
        REG32 iepintmsk : 1; // [18]
        REG32 oepintmsk : 1; // [19]
        REG32 __20_20 : 1;
        REG32 incomplpmsk : 1; // [21]
        REG32 fetsuspmsk : 1; // [22]
        REG32 resetdetmsk : 1; // [23]
        REG32 prtintmsk : 1; // [24]
        REG32 hchintmsk : 1; // [25]
        REG32 ptxfempmsk : 1; // [26]
        REG32 __27_27 : 1;
        REG32 conidstschngmsk : 1; // [28]
        REG32 disconnintmsk : 1; // [29]
        REG32 sessreqintmsk : 1; // [30]
        REG32 wkupintmsk : 1; // [31]
    } b;
} REG_USBC_GINTMSK_T;

//GRXSTSR
typedef union {
    REG32 v;
    struct {
        REG32 chnum : 4; // [3:0]
        REG32 bcnt : 11; // [14:4], read only
        REG32 dpid : 2; // [16:15], read only
        REG32 pktsts : 4; // [20:17]
        REG32 fn : 4; // [24:21], read only
        REG32 __31_25 : 7;
    } b;
} REG_USBC_GRXSTSR_T;

//GRXSTSP
typedef union {
    REG32 v;
    struct {
        REG32 chnum : 4; // [3:0]
        REG32 bcnt : 11; // [14:4], read only
        REG32 dpid : 2; // [16:15], read only
        REG32 pktsts : 4; // [20:17]
        REG32 fn : 4; // [24:21], read only
        REG32 __31_25 : 7;
    } b;
} REG_USBC_GRXSTSP_T;

//GRXFSIZ
typedef union {
    REG32 v;
    struct {
        REG32 rxfdep : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_USBC_GRXFSIZ_T;

//GNPTXFSIZ
typedef union {
    REG32 v;
    struct {
        REG32 nptxfstaddr : 12; // [11:0]
        REG32 __15_12 : 4;
        REG32 nptxfdep : 12; // [27:16]
        REG32 __31_28 : 4;
    } b;
} REG_USBC_GNPTXFSIZ_T;

//GNPTXSTS
typedef union {
    REG32 v;
    struct {
        REG32 nptxfspcavail : 16; // [15:0], read only
        REG32 nptxqspcavail : 8; // [23:16], read only
        REG32 nptxqtop : 7; // [30:24], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_GNPTXSTS_T;

//GGPIO
typedef union {
    REG32 v;
    struct {
        REG32 gpi : 16; // [15:0], read only
        REG32 gpo : 16; // [31:16]
    } b;
} REG_USBC_GGPIO_T;

//GHWCFG2
typedef union {
    REG32 v;
    struct {
        REG32 otgmode : 3; // [2:0], read only
        REG32 otgarch : 2; // [4:3], read only
        REG32 singpnt : 1; // [5], read only
        REG32 hsphytype : 2; // [7:6], read only
        REG32 fsphytype : 2; // [9:8], read only
        REG32 numdeveps : 4; // [13:10], read only
        REG32 numhstchnl : 4; // [17:14], read only
        REG32 periosupport : 1; // [18], read only
        REG32 dynfifosizing : 1; // [19], read only
        REG32 multiprocintrpt : 1; // [20], read only
        REG32 __21_21 : 1;
        REG32 nptxqdepth : 2; // [23:22], read only
        REG32 ptxqdepth : 2; // [25:24], read only
        REG32 tknqdepth : 5; // [30:26], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_GHWCFG2_T;

//GHWCFG3
typedef union {
    REG32 v;
    struct {
        REG32 xfersizewidth : 4; // [3:0], read only
        REG32 pktsizewidth : 3; // [6:4], read only
        REG32 otgen : 1; // [7], read only
        REG32 i2cintsel : 1; // [8], read only
        REG32 vndctlsupt : 1; // [9], read only
        REG32 optfeature : 1; // [10], read only
        REG32 rsttype : 1; // [11], read only
        REG32 adpsupport : 1; // [12], read only
        REG32 hsicmode : 1; // [13], read only
        REG32 bcsupport : 1; // [14], read only
        REG32 lpmmode : 1; // [15], read only
        REG32 dfifodepth : 16; // [31:16], read only
    } b;
} REG_USBC_GHWCFG3_T;

//GHWCFG4
typedef union {
    REG32 v;
    struct {
        REG32 numdevperioeps : 4; // [3:0], read only
        REG32 partialpwrdn : 1; // [4], read only
        REG32 ahbfreq : 1; // [5], read only
        REG32 hibernation : 1; // [6], read only
        REG32 extendedhibernation : 1; // [7], read only
        REG32 __11_8 : 4;
        REG32 acgsupt : 1; // [12], read only
        REG32 enhancedlpmsupt : 1; // [13], read only
        REG32 phydatawidth : 2; // [15:14], read only
        REG32 numctleps : 4; // [19:16], read only
        REG32 iddgfltr : 1; // [20], read only
        REG32 vbusvalidfltr : 1; // [21], read only
        REG32 avalidfltr : 1; // [22], read only
        REG32 bvalidfltr : 1; // [23], read only
        REG32 sessendfltr : 1; // [24], read only
        REG32 dedfifomode : 1; // [25], read only
        REG32 ineps : 4; // [29:26], read only
        REG32 descdmaenabled : 1; // [30], read only
        REG32 descdma : 1; // [31], read only
    } b;
} REG_USBC_GHWCFG4_T;

//GPWRDN
typedef union {
    REG32 v;
    struct {
        REG32 pmuintsel : 1; // [0]
        REG32 pmuactv : 1; // [1]
        REG32 __2_2 : 1;
        REG32 pwrdnclmp : 1; // [3]
        REG32 pwrdnrst_n : 1; // [4]
        REG32 pwrdnswtch : 1; // [5]
        REG32 disablevbus : 1; // [6]
        REG32 __14_7 : 8;
        REG32 srpdetect : 1; // [15], write clear
        REG32 srpdetectmsk : 1; // [16]
        REG32 stschngint : 1; // [17], write clear
        REG32 stschngintmsk : 1; // [18]
        REG32 linestate : 2; // [20:19], read only
        REG32 iddig : 1; // [21], read only
        REG32 bsessvld : 1; // [22], read only
        REG32 adpint : 1; // [23], write clear
        REG32 __31_24 : 8;
    } b;
} REG_USBC_GPWRDN_T;

//GDFIFOCFG
typedef union {
    REG32 v;
    struct {
        REG32 gdfifocfg : 16; // [15:0]
        REG32 epinfobaseaddr : 16; // [31:16]
    } b;
} REG_USBC_GDFIFOCFG_T;

//GADPCTL
typedef union {
    REG32 v;
    struct {
        REG32 prbdschg : 2; // [1:0]
        REG32 prbdelta : 2; // [3:2]
        REG32 prbper : 2; // [5:4]
        REG32 rtim : 11; // [16:6], read only
        REG32 enaprb : 1; // [17]
        REG32 enasns : 1; // [18]
        REG32 adpres : 1; // [19]
        REG32 adpen : 1; // [20]
        REG32 adpprbint : 1; // [21], write clear
        REG32 adpsnsint : 1; // [22], write clear
        REG32 adptoutint : 1; // [23], write clear
        REG32 adpprbintmsk : 1; // [24]
        REG32 adpsnsintmsk : 1; // [25]
        REG32 adptoutmsk : 1; // [26]
        REG32 ar : 2; // [28:27]
        REG32 __31_29 : 3;
    } b;
} REG_USBC_GADPCTL_T;

//HPTXFSIZ
typedef union {
    REG32 v;
    struct {
        REG32 ptxfstaddr : 13; // [12:0]
        REG32 __15_13 : 3;
        REG32 ptxfsize : 12; // [27:16]
        REG32 __31_28 : 4;
    } b;
} REG_USBC_HPTXFSIZ_T;

//DIEPTXF1
typedef union {
    REG32 v;
    struct {
        REG32 inepntxfstaddr : 12; // [11:0]
        REG32 __15_12 : 4;
        REG32 inepntxfdep : 10; // [25:16]
        REG32 __31_26 : 6;
    } b;
} REG_USBC_DIEPTXF1_T;

//DIEPTXF2
typedef union {
    REG32 v;
    struct {
        REG32 inepntxfstaddr : 13; // [12:0]
        REG32 __15_13 : 3;
        REG32 inepntxfdep : 10; // [25:16]
        REG32 __31_26 : 6;
    } b;
} REG_USBC_DIEPTXF2_T;

//DIEPTXF3
typedef union {
    REG32 v;
    struct {
        REG32 inepntxfstaddr : 13; // [12:0]
        REG32 __15_13 : 3;
        REG32 inepntxfdep : 9; // [24:16]
        REG32 __31_25 : 7;
    } b;
} REG_USBC_DIEPTXF3_T;

//DIEPTXF4
typedef union {
    REG32 v;
    struct {
        REG32 inepntxfstaddr : 13; // [12:0]
        REG32 __15_13 : 3;
        REG32 inepntxfdep : 9; // [24:16]
        REG32 __31_25 : 7;
    } b;
} REG_USBC_DIEPTXF4_T;

//DIEPTXF5
typedef union {
    REG32 v;
    struct {
        REG32 inepntxfstaddr : 13; // [12:0]
        REG32 __15_13 : 3;
        REG32 inepntxfdep : 9; // [24:16]
        REG32 __31_25 : 7;
    } b;
} REG_USBC_DIEPTXF5_T;

//DIEPTXF6
typedef union {
    REG32 v;
    struct {
        REG32 inepntxfstaddr : 13; // [12:0]
        REG32 __15_13 : 3;
        REG32 inepntxfdep : 9; // [24:16]
        REG32 __31_25 : 7;
    } b;
} REG_USBC_DIEPTXF6_T;

//DIEPTXF7
typedef union {
    REG32 v;
    struct {
        REG32 inepntxfstaddr : 13; // [12:0]
        REG32 __15_13 : 3;
        REG32 inepntxfdep : 9; // [24:16]
        REG32 __31_25 : 7;
    } b;
} REG_USBC_DIEPTXF7_T;

//DIEPTXF8
typedef union {
    REG32 v;
    struct {
        REG32 inepntxfstaddr : 13; // [12:0]
        REG32 __15_13 : 3;
        REG32 inepntxfdep : 9; // [24:16]
        REG32 __31_25 : 7;
    } b;
} REG_USBC_DIEPTXF8_T;

//HCFG
typedef union {
    REG32 v;
    struct {
        REG32 fslspclksel : 2; // [1:0]
        REG32 fslssupp : 1; // [2]
        REG32 __6_3 : 4;
        REG32 ena32khzs : 1; // [7]
        REG32 resvalid : 8; // [15:8]
        REG32 __22_16 : 7;
        REG32 descdma : 1; // [23]
        REG32 frlisten : 2; // [25:24]
        REG32 perschedena : 1; // [26]
        REG32 __30_27 : 4;
        REG32 modechtimen : 1; // [31]
    } b;
} REG_USBC_HCFG_T;

//HFIR
typedef union {
    REG32 v;
    struct {
        REG32 frint : 16; // [15:0]
        REG32 hfirrldctrl : 1; // [16]
        REG32 __31_17 : 15;
    } b;
} REG_USBC_HFIR_T;

//HFNUM
typedef union {
    REG32 v;
    struct {
        REG32 frnum : 16; // [15:0], read only
        REG32 frrem : 16; // [31:16], read only
    } b;
} REG_USBC_HFNUM_T;

//HPTXSTS
typedef union {
    REG32 v;
    struct {
        REG32 ptxfspcavail : 16; // [15:0], read only
        REG32 ptxqspcavail : 8; // [23:16], read only
        REG32 ptxqtop : 8; // [31:24], read only
    } b;
} REG_USBC_HPTXSTS_T;

//HAINT
typedef union {
    REG32 v;
    struct {
        REG32 haint : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_HAINT_T;

//HAINTMSK
typedef union {
    REG32 v;
    struct {
        REG32 haintmsk : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_USBC_HAINTMSK_T;

//HPRT
typedef union {
    REG32 v;
    struct {
        REG32 prtconnsts : 1; // [0], read only
        REG32 prtconndet : 1; // [1], write clear
        REG32 prtena : 1; // [2], write clear
        REG32 prtenchng : 1; // [3], write clear
        REG32 prtovrcurract : 1; // [4], read only
        REG32 prtovrcurrchng : 1; // [5], write clear
        REG32 prtres : 1; // [6]
        REG32 prtsusp : 1; // [7]
        REG32 prtrst : 1; // [8]
        REG32 __9_9 : 1;
        REG32 prtlnsts : 2; // [11:10], read only
        REG32 prtpwr : 1; // [12]
        REG32 prttstctl : 4; // [16:13]
        REG32 prtspd : 2; // [18:17], read only
        REG32 __31_19 : 13;
    } b;
} REG_USBC_HPRT_T;

//HCCHAR0
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR0_T;

//HCSPLT0
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT0_T;

//HCINT0
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT0_T;

//HCINTMSK0
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK0_T;

//HCTSIZ0
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ0_T;

//HCCHAR1
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR1_T;

//HCSPLT1
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT1_T;

//HCINT1
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT1_T;

//HCINTMSK1
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK1_T;

//HCTSIZ1
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ1_T;

//HCCHAR2
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR2_T;

//HCSPLT2
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT2_T;

//HCINT2
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT2_T;

//HCINTMSK2
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK2_T;

//HCTSIZ2
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ2_T;

//HCCHAR3
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR3_T;

//HCSPLT3
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT3_T;

//HCINT3
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT3_T;

//HCINTMSK3
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK3_T;

//HCTSIZ3
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ3_T;

//HCCHAR4
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR4_T;

//HCSPLT4
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT4_T;

//HCINT4
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT4_T;

//HCINTMSK4
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK4_T;

//HCTSIZ4
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ4_T;

//HCCHAR5
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR5_T;

//HCSPLT5
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT5_T;

//HCINT5
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT5_T;

//HCINTMSK5
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK5_T;

//HCTSIZ5
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ5_T;

//HCCHAR6
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR6_T;

//HCSPLT6
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT6_T;

//HCINT6
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT6_T;

//HCINTMSK6
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK6_T;

//HCTSIZ6
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ6_T;

//HCCHAR7
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR7_T;

//HCSPLT7
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT7_T;

//HCINT7
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT7_T;

//HCINTMSK7
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK7_T;

//HCTSIZ7
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ7_T;

//HCCHAR8
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR8_T;

//HCSPLT8
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT8_T;

//HCINT8
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT8_T;

//HCINTMSK8
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK8_T;

//HCTSIZ8
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ8_T;

//HCCHAR9
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR9_T;

//HCSPLT9
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT9_T;

//HCINT9
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT9_T;

//HCINTMSK9
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK9_T;

//HCTSIZ9
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ9_T;

//HCCHAR10
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR10_T;

//HCSPLT10
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT10_T;

//HCINT10
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT10_T;

//HCINTMSK10
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK10_T;

//HCTSIZ10
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ10_T;

//HCCHAR11
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR11_T;

//HCSPLT11
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT11_T;

//HCINT11
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT11_T;

//HCINTMSK11
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK11_T;

//HCTSIZ11
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ11_T;

//HCCHAR12
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR12_T;

//HCSPLT12
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT12_T;

//HCINT12
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT12_T;

//HCINTMSK12
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK12_T;

//HCTSIZ12
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ12_T;

//HCCHAR13
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR13_T;

//HCSPLT13
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT13_T;

//HCINT13
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT13_T;

//HCINTMSK13
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK13_T;

//HCTSIZ13
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ13_T;

//HCCHAR14
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR14_T;

//HCSPLT14
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT14_T;

//HCINT14
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT14_T;

//HCINTMSK14
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK14_T;

//HCTSIZ14
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ14_T;

//HCCHAR15
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 epnum : 4; // [14:11]
        REG32 epdir : 1; // [15]
        REG32 __16_16 : 1;
        REG32 lspddev : 1; // [17]
        REG32 eptype : 2; // [19:18]
        REG32 ec : 2; // [21:20]
        REG32 devaddr : 7; // [28:22]
        REG32 oddfrm : 1; // [29]
        REG32 chdis : 1; // [30]
        REG32 chena : 1; // [31]
    } b;
} REG_USBC_HCCHAR15_T;

//HCSPLT15
typedef union {
    REG32 v;
    struct {
        REG32 prtaddr : 7; // [6:0]
        REG32 hubaddr : 7; // [13:7]
        REG32 xactpos : 2; // [15:14]
        REG32 compsplt : 1; // [16]
        REG32 __30_17 : 14;
        REG32 spltena : 1; // [31]
    } b;
} REG_USBC_HCSPLT15_T;

//HCINT15
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 chhltd : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 stall : 1; // [3], write clear
        REG32 nak : 1; // [4], write clear
        REG32 ack : 1; // [5], write clear
        REG32 nyet : 1; // [6], write clear
        REG32 xacterr : 1; // [7], write clear
        REG32 bblerr : 1; // [8], write clear
        REG32 frmovrun : 1; // [9], write clear
        REG32 datatglerr : 1; // [10], write clear
        REG32 bnaintr : 1; // [11], write clear
        REG32 xcs_xact_err : 1; // [12], write clear
        REG32 desc_lst_rollintr : 1; // [13], write clear
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINT15_T;

//HCINTMSK15
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 chhltdmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 __10_3 : 8;
        REG32 bnaintrmsk : 1; // [11]
        REG32 __12_12 : 1;
        REG32 desc_lst_rollintrmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_HCINTMSK15_T;

//HCTSIZ15
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 pid : 2; // [30:29]
        REG32 dopng : 1; // [31]
    } b;
} REG_USBC_HCTSIZ15_T;

//DCFG
typedef union {
    REG32 v;
    struct {
        REG32 devspd : 2; // [1:0]
        REG32 nzstsouthshk : 1; // [2]
        REG32 ena32khzsusp : 1; // [3]
        REG32 devaddr : 7; // [10:4]
        REG32 perfrint : 2; // [12:11]
        REG32 endevoutnak : 1; // [13]
        REG32 xcvrdly : 1; // [14]
        REG32 erraticintmsk : 1; // [15]
        REG32 __22_16 : 7;
        REG32 descdma : 1; // [23]
        REG32 perschintvl : 2; // [25:24]
        REG32 resvalid : 6; // [31:26]
    } b;
} REG_USBC_DCFG_T;

//DCTL
typedef union {
    REG32 v;
    struct {
        REG32 rmtwkupsig : 1; // [0]
        REG32 sftdiscon : 1; // [1]
        REG32 gnpinnaksts : 1; // [2], read only
        REG32 goutnaksts : 1; // [3], read only
        REG32 tstctl : 3; // [6:4]
        REG32 sgnpinnak : 1; // [7]
        REG32 cgnpinnak : 1; // [8]
        REG32 sgoutnak : 1; // [9]
        REG32 cgoutnak : 1; // [10]
        REG32 pwronprgdone : 1; // [11]
        REG32 __12_12 : 1;
        REG32 gmc : 2; // [14:13]
        REG32 ignrfrmnum : 1; // [15]
        REG32 nakonbble : 1; // [16]
        REG32 encontonbna : 1; // [17]
        REG32 __31_18 : 14;
    } b;
} REG_USBC_DCTL_T;

//DSTS
typedef union {
    REG32 v;
    struct {
        REG32 suspsts : 1; // [0], read only
        REG32 enumspd : 2; // [2:1], read only
        REG32 errticerr : 1; // [3], read only
        REG32 __7_4 : 4;
        REG32 soffn : 14; // [21:8]
        REG32 devlnsts : 2; // [23:22], read only
        REG32 __31_24 : 8;
    } b;
} REG_USBC_DSTS_T;

//DIEPMSK
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 epdisbldmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 timeoutmsk : 1; // [3]
        REG32 intkntxfempmsk : 1; // [4]
        REG32 intknepmismsk : 1; // [5]
        REG32 inepnakeffmsk : 1; // [6]
        REG32 __7_7 : 1;
        REG32 txfifoundrnmsk : 1; // [8]
        REG32 bnainintrmsk : 1; // [9]
        REG32 __12_10 : 3;
        REG32 nakmsk : 1; // [13]
        REG32 __31_14 : 18;
    } b;
} REG_USBC_DIEPMSK_T;

//DOEPMSK
typedef union {
    REG32 v;
    struct {
        REG32 xfercomplmsk : 1; // [0]
        REG32 epdisbldmsk : 1; // [1]
        REG32 ahberrmsk : 1; // [2]
        REG32 setupmsk : 1; // [3]
        REG32 outtknepdismsk : 1; // [4]
        REG32 stsphsercvdmsk : 1; // [5]
        REG32 back2backsetup : 1; // [6]
        REG32 __7_7 : 1;
        REG32 outpkterrmsk : 1; // [8]
        REG32 bnaoutintrmsk : 1; // [9]
        REG32 __11_10 : 2;
        REG32 bbleerrmsk : 1; // [12]
        REG32 nakmsk : 1; // [13]
        REG32 nyetmsk : 1; // [14]
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DOEPMSK_T;

//DAINT
typedef union {
    REG32 v;
    struct {
        REG32 inepint0 : 1; // [0], read only
        REG32 inepint1 : 1; // [1], read only
        REG32 inepint2 : 1; // [2], read only
        REG32 inepint3 : 1; // [3], read only
        REG32 inepint4 : 1; // [4], read only
        REG32 inepint5 : 1; // [5], read only
        REG32 inepint6 : 1; // [6], read only
        REG32 inepint7 : 1; // [7], read only
        REG32 inepint8 : 1; // [8], read only
        REG32 inepint9 : 1; // [9], read only
        REG32 inepint10 : 1; // [10], read only
        REG32 inepint11 : 1; // [11], read only
        REG32 inepint12 : 1; // [12], read only
        REG32 __15_13 : 3;
        REG32 outepint0 : 1; // [16], read only
        REG32 outepint1 : 1; // [17], read only
        REG32 outepint2 : 1; // [18], read only
        REG32 outepint3 : 1; // [19], read only
        REG32 outepint4 : 1; // [20], read only
        REG32 outepint5 : 1; // [21], read only
        REG32 outepint6 : 1; // [22], read only
        REG32 outepint7 : 1; // [23], read only
        REG32 outepint8 : 1; // [24], read only
        REG32 outepint9 : 1; // [25], read only
        REG32 outepint10 : 1; // [26], read only
        REG32 outepint11 : 1; // [27], read only
        REG32 outepint12 : 1; // [28], read only
        REG32 __31_29 : 3;
    } b;
} REG_USBC_DAINT_T;

//DAINTMSK
typedef union {
    REG32 v;
    struct {
        REG32 inepmsk0 : 1; // [0]
        REG32 inepmsk1 : 1; // [1]
        REG32 inepmsk2 : 1; // [2]
        REG32 inepmsk3 : 1; // [3]
        REG32 inepmsk4 : 1; // [4]
        REG32 inepmsk5 : 1; // [5]
        REG32 inepmsk6 : 1; // [6]
        REG32 inepmsk7 : 1; // [7]
        REG32 inepmsk8 : 1; // [8]
        REG32 inepmsk9 : 1; // [9]
        REG32 inepmsk10 : 1; // [10]
        REG32 inepmsk11 : 1; // [11]
        REG32 inepmsk12 : 1; // [12]
        REG32 __15_13 : 3;
        REG32 outepmsk0 : 1; // [16]
        REG32 outepmsk1 : 1; // [17]
        REG32 outepmsk2 : 1; // [18]
        REG32 outepmsk3 : 1; // [19]
        REG32 outepmsk4 : 1; // [20]
        REG32 outepmsk5 : 1; // [21]
        REG32 outepmsk6 : 1; // [22]
        REG32 outepmsk7 : 1; // [23]
        REG32 outepmsk8 : 1; // [24]
        REG32 outepmsk9 : 1; // [25]
        REG32 outepmsk10 : 1; // [26]
        REG32 outepmsk11 : 1; // [27]
        REG32 outepmsk12 : 1; // [28]
        REG32 __31_29 : 3;
    } b;
} REG_USBC_DAINTMSK_T;

//DVBUSDIS
typedef union {
    REG32 v;
    struct {
        REG32 dvbusdis : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DVBUSDIS_T;

//DVBUSPULSE
typedef union {
    REG32 v;
    struct {
        REG32 dvbuspulse : 12; // [11:0]
        REG32 __31_12 : 20;
    } b;
} REG_USBC_DVBUSPULSE_T;

//DTHRCTL
typedef union {
    REG32 v;
    struct {
        REG32 nonisothren : 1; // [0]
        REG32 isothren : 1; // [1]
        REG32 txthrlen : 9; // [10:2]
        REG32 ahbthrratio : 2; // [12:11]
        REG32 __15_13 : 3;
        REG32 rxthren : 1; // [16]
        REG32 rxthrlen : 9; // [25:17]
        REG32 __26_26 : 1;
        REG32 arbprken : 1; // [27]
        REG32 __31_28 : 4;
    } b;
} REG_USBC_DTHRCTL_T;

//DIEPEMPMSK
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfempmsk : 16; // [15:0]
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DIEPEMPMSK_T;

//DIEPCTL0
typedef union {
    REG32 v;
    struct {
        REG32 mps : 2; // [1:0]
        REG32 __14_2 : 13;
        REG32 usbactep : 1; // [15], read only
        REG32 __16_16 : 1;
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18], read only
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 __29_28 : 2;
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL0_T;

//DIEPINT0
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT0_T;

//DIEPTSIZ0
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 7; // [6:0]
        REG32 __18_7 : 12;
        REG32 pktcnt : 2; // [20:19]
        REG32 __31_21 : 11;
    } b;
} REG_USBC_DIEPTSIZ0_T;

//DTXFSTS0
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS0_T;

//DIEPCTL1
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL1_T;

//DIEPINT1
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT1_T;

//DIEPTSIZ1
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ1_T;

//DTXFSTS1
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS1_T;

//DIEPCTL2
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL2_T;

//DIEPINT2
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT2_T;

//DIEPTSIZ2
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ2_T;

//DTXFSTS2
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS2_T;

//DIEPCTL3
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL3_T;

//DIEPINT3
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT3_T;

//DIEPTSIZ3
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ3_T;

//DTXFSTS3
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS3_T;

//DIEPCTL4
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL4_T;

//DIEPINT4
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT4_T;

//DIEPTSIZ4
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ4_T;

//DTXFSTS4
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS4_T;

//DIEPCTL5
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL5_T;

//DIEPINT5
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT5_T;

//DIEPTSIZ5
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ5_T;

//DTXFSTS5
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS5_T;

//DIEPCTL6
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL6_T;

//DIEPINT6
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT6_T;

//DIEPTSIZ6
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ6_T;

//DTXFSTS6
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS6_T;

//DIEPCTL7
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL7_T;

//DIEPINT7
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT7_T;

//DIEPTSIZ7
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ7_T;

//DTXFSTS7
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS7_T;

//DIEPCTL8
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL8_T;

//DIEPINT8
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT8_T;

//DIEPTSIZ8
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ8_T;

//DTXFSTS8
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS8_T;

//DIEPCTL9
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL9_T;

//DIEPINT9
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT9_T;

//DIEPTSIZ9
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ9_T;

//DTXFSTS9
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS9_T;

//DIEPCTL10
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL10_T;

//DIEPINT10
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT10_T;

//DIEPTSIZ10
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ10_T;

//DTXFSTS10
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS10_T;

//DIEPCTL11
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL11_T;

//DIEPINT11
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT11_T;

//DIEPTSIZ11
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ11_T;

//DTXFSTS11
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS11_T;

//DIEPCTL12
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 __20_20 : 1;
        REG32 stall : 1; // [21]
        REG32 txfnum : 4; // [25:22]
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DIEPCTL12_T;

//DIEPINT12
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 timeout : 1; // [3], write clear
        REG32 intkntxfemp : 1; // [4], write clear
        REG32 intknepmis : 1; // [5], write clear
        REG32 inepnakeff : 1; // [6], write clear
        REG32 txfemp : 1; // [7], read only
        REG32 txfifoundrn : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 __31_15 : 17;
    } b;
} REG_USBC_DIEPINT12_T;

//DIEPTSIZ12
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 mc : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DIEPTSIZ12_T;

//DTXFSTS12
typedef union {
    REG32 v;
    struct {
        REG32 ineptxfspcavail : 16; // [15:0], read only
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DTXFSTS12_T;

//DOEPCTL0
typedef union {
    REG32 v;
    struct {
        REG32 mps : 2; // [1:0], read only
        REG32 __14_2 : 13;
        REG32 usbactep : 1; // [15], read only
        REG32 __16_16 : 1;
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18], read only
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 __29_28 : 2;
        REG32 epdis : 1; // [30], read only
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL0_T;

//DOEPINT0
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT0_T;

//DOEPTSIZ0
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 7; // [6:0]
        REG32 __18_7 : 12;
        REG32 pktcnt : 1; // [19]
        REG32 __28_20 : 9;
        REG32 supcnt : 2; // [30:29]
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ0_T;

//DOEPCTL1
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL1_T;

//DOEPINT1
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT1_T;

//DOEPTSIZ1
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ1_T;

//DOEPCTL2
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL2_T;

//DOEPINT2
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT2_T;

//DOEPTSIZ2
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ2_T;

//DOEPCTL3
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL3_T;

//DOEPINT3
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT3_T;

//DOEPTSIZ3
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ3_T;

//DOEPCTL4
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL4_T;

//DOEPINT4
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT4_T;

//DOEPTSIZ4
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ4_T;

//DOEPCTL5
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL5_T;

//DOEPINT5
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT5_T;

//DOEPTSIZ5
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ5_T;

//DOEPCTL6
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL6_T;

//DOEPINT6
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT6_T;

//DOEPTSIZ6
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ6_T;

//DOEPCTL7
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL7_T;

//DOEPINT7
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT7_T;

//DOEPTSIZ7
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ7_T;

//DOEPCTL8
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL8_T;

//DOEPINT8
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT8_T;

//DOEPTSIZ8
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ8_T;

//DOEPCTL9
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL9_T;

//DOEPINT9
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT9_T;

//DOEPTSIZ9
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ9_T;

//DOEPCTL10
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL10_T;

//DOEPINT10
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT10_T;

//DOEPTSIZ10
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ10_T;

//DOEPCTL11
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL11_T;

//DOEPINT11
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT11_T;

//DOEPTSIZ11
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ11_T;

//DOEPCTL12
typedef union {
    REG32 v;
    struct {
        REG32 mps : 11; // [10:0]
        REG32 __14_11 : 4;
        REG32 usbactep : 1; // [15]
        REG32 dpid : 1; // [16], read only
        REG32 naksts : 1; // [17], read only
        REG32 eptype : 2; // [19:18]
        REG32 snp : 1; // [20]
        REG32 stall : 1; // [21]
        REG32 __25_22 : 4;
        REG32 cnak : 1; // [26]
        REG32 snak : 1; // [27]
        REG32 setd0pid : 1; // [28]
        REG32 setd1pid : 1; // [29]
        REG32 epdis : 1; // [30]
        REG32 epena : 1; // [31]
    } b;
} REG_USBC_DOEPCTL12_T;

//DOEPINT12
typedef union {
    REG32 v;
    struct {
        REG32 xfercompl : 1; // [0], write clear
        REG32 epdisbld : 1; // [1], write clear
        REG32 ahberr : 1; // [2], write clear
        REG32 setup : 1; // [3], write clear
        REG32 outtknepdis : 1; // [4], write clear
        REG32 stsphsercvd : 1; // [5], write clear
        REG32 back2backsetup : 1; // [6], write clear
        REG32 __7_7 : 1;
        REG32 outpkterr : 1; // [8], write clear
        REG32 bnaintr : 1; // [9], write clear
        REG32 __10_10 : 1;
        REG32 pktdrpsts : 1; // [11], write clear
        REG32 bbleerr : 1; // [12], write clear
        REG32 nakintrpt : 1; // [13], write clear
        REG32 nyetintrpt : 1; // [14], write clear
        REG32 stuppktrcvd : 1; // [15], write clear
        REG32 __31_16 : 16;
    } b;
} REG_USBC_DOEPINT12_T;

//DOEPTSIZ12
typedef union {
    REG32 v;
    struct {
        REG32 xfersize : 19; // [18:0]
        REG32 pktcnt : 10; // [28:19]
        REG32 rxdpid : 2; // [30:29], read only
        REG32 __31_31 : 1;
    } b;
} REG_USBC_DOEPTSIZ12_T;

//PCGCCTL
typedef union {
    REG32 v;
    struct {
        REG32 stoppclk : 1; // [0]
        REG32 __2_1 : 2;
        REG32 rstpdwnmodule : 1; // [3]
        REG32 __5_4 : 2;
        REG32 physleep : 1; // [6], read only
        REG32 l1suspended : 1; // [7], read only
        REG32 __31_8 : 24;
    } b;
} REG_USBC_PCGCCTL_T;


//GOTGCTL
#define USBC_GOTGCTL_RESERVED_1(n) (((n)&0x3FF)<<22)
#define USBC_CURMOD                (1<<21)
#define USBC_OTGVER                (1<<20)
#define USBC_BSESVLD               (1<<19)
#define USBC_ASESVLD               (1<<18)
#define USBC_DBNCTIME              (1<<17)
#define USBC_CONIDSTS              (1<<16)
#define USBC_DBNCEFLTRBYPASS       (1<<15)
#define USBC_GOTGCTL_RESERVED_0(n) (((n)&3)<<13)
#define USBC_EHEN                  (1<<12)
#define USBC_DEVHNPEN              (1<<11)
#define USBC_HSTSETHNPEN           (1<<10)
#define USBC_HNPREQ                (1<<9)
#define USBC_HSTNEGSCS             (1<<8)
#define USBC_BVALIDOVVAL           (1<<7)
#define USBC_BVALIDOVEN            (1<<6)
#define USBC_AVALIDOVVAL           (1<<5)
#define USBC_AVALIDOVEN            (1<<4)
#define USBC_VBVALIDOVVAL          (1<<3)
#define USBC_VBVALIDOVEN           (1<<2)
#define USBC_SESREQ                (1<<1)
#define USBC_SESREQSCS             (1<<0)

//GOTGINT
#define USBC_GOTGINT_RESERVED_3(n) (((n)&0xFFF)<<20)
#define USBC_DBNCEDONE             (1<<19)
#define USBC_ADEVTOUTCHG           (1<<18)
#define USBC_HSTNEGDET             (1<<17)
#define USBC_GOTGINT_RESERVED_2(n) (((n)&0x7F)<<10)
#define USBC_HSTNEGSUCSTSCHNG      (1<<9)
#define USBC_SESREQSUCSTSCHNG      (1<<8)
#define USBC_GOTGINT_RESERVED_1(n) (((n)&31)<<3)
#define USBC_SESENDDET             (1<<2)
#define USBC_GOTGINT_RESERVED_0(n) (((n)&3)<<0)

//GAHBCFG
#define USBC_GAHBCFG_RESERVED_2(n) (((n)&0x7F)<<25)
#define USBC_INVDESCENDIANESS      (1<<24)
#define USBC_AHBSINGLE             (1<<23)
#define USBC_NOTIALLDMAWRIT        (1<<22)
#define USBC_REMMEMSUPP            (1<<21)
#define USBC_GAHBCFG_RESERVED_1(n) (((n)&0xFFF)<<9)
#define USBC_PTXFEMPLVL            (1<<8)
#define USBC_NPTXFEMPLVL           (1<<7)
#define USBC_GAHBCFG_RESERVED_0    (1<<6)
#define USBC_DMAEN                 (1<<5)
#define USBC_HBSTLEN(n)            (((n)&15)<<1)
#define USBC_GLBLINTRMSK           (1<<0)

//GUSBCFG
#define USBC_CORRUPTTXPKT          (1<<31)
#define USBC_FORCEDEVMODE          (1<<30)
#define USBC_FORCEHSTMODE          (1<<29)
#define USBC_TXENDDELAY            (1<<28)
#define USBC_GUSBCFG_RESERVED_4    (1<<27)
#define USBC_IC_USBCAP             (1<<26)
#define USBC_GUSBCFG_RESERVED_3(n) (((n)&7)<<23)
#define USBC_TERMSELDLPULSE        (1<<22)
#define USBC_GUSBCFG_RESERVED_2(n) (((n)&0x3F)<<16)
#define USBC_PHYLPWRCLKSEL         (1<<15)
#define USBC_GUSBCFG_RESERVED_1    (1<<14)
#define USBC_USBTRDTIM(n)          (((n)&15)<<10)
#define USBC_HNPCAP                (1<<9)
#define USBC_SRPCAP                (1<<8)
#define USBC_GUSBCFG_RESERVED_0    (1<<7)
#define USBC_PHYSEL                (1<<6)
#define USBC_FSINTF                (1<<5)
#define USBC_ULPI_UTMI_SEL         (1<<4)
#define USBC_PHYIF                 (1<<3)
#define USBC_TOUTCAL(n)            (((n)&7)<<0)

//GRSTCTL
#define USBC_AHBIDLE               (1<<31)
#define USBC_DMAREQ                (1<<30)
#define USBC_GRSTCTL_RESERVED_1(n) (((n)&0x7FFFF)<<11)
#define USBC_TXFNUM(n)             (((n)&31)<<6)
#define USBC_TXFFLSH               (1<<5)
#define USBC_RXFFLSH               (1<<4)
#define USBC_GRSTCTL_RESERVED_0    (1<<3)
#define USBC_FRMCNTRRST            (1<<2)
#define USBC_PIUFSSFTRST           (1<<1)
#define USBC_CSFTRST               (1<<0)

//GINTSTS
#define USBC_WKUPINT               (1<<31)
#define USBC_SESSREQINT            (1<<30)
#define USBC_DISCONNINT            (1<<29)
#define USBC_CONIDSTSCHNG          (1<<28)
#define USBC_GINTSTS_RESERVED_2    (1<<27)
#define USBC_PTXFEMP               (1<<26)
#define USBC_HCHINT                (1<<25)
#define USBC_PRTINT                (1<<24)
#define USBC_RESETDET              (1<<23)
#define USBC_FETSUSP               (1<<22)
#define USBC_INCOMPLP              (1<<21)
#define USBC_INCOMPISOIN           (1<<20)
#define USBC_OEPINT                (1<<19)
#define USBC_IEPINT                (1<<18)
#define USBC_EPMIS                 (1<<17)
#define USBC_GINTSTS_RESERVED_1    (1<<16)
#define USBC_EOPF                  (1<<15)
#define USBC_ISOOUTDROP            (1<<14)
#define USBC_ENUMDONE              (1<<13)
#define USBC_USBRST                (1<<12)
#define USBC_USBSUSP               (1<<11)
#define USBC_ERLYSUSP              (1<<10)
#define USBC_GINTSTS_RESERVED_0(n) (((n)&3)<<8)
#define USBC_GOUTNAKEFF            (1<<7)
#define USBC_GINNAKEFF             (1<<6)
#define USBC_NPTXFEMP              (1<<5)
#define USBC_RXFLVL                (1<<4)
#define USBC_SOF                   (1<<3)
#define USBC_OTGINT                (1<<2)
#define USBC_MODEMIS               (1<<1)
#define USBC_CURMOD_X1             (1<<0)

//GINTMSK
#define USBC_WKUPINTMSK            (1<<31)
#define USBC_SESSREQINTMSK         (1<<30)
#define USBC_DISCONNINTMSK         (1<<29)
#define USBC_CONIDSTSCHNGMSK       (1<<28)
#define USBC_GINTMSK_RESERVED_4    (1<<27)
#define USBC_PTXFEMPMSK            (1<<26)
#define USBC_HCHINTMSK             (1<<25)
#define USBC_PRTINTMSK             (1<<24)
#define USBC_RESETDETMSK           (1<<23)
#define USBC_FETSUSPMSK            (1<<22)
#define USBC_INCOMPLPMSK           (1<<21)
#define USBC_GINTMSK_RESERVED_3    (1<<20)
#define USBC_OEPINTMSK             (1<<19)
#define USBC_IEPINTMSK             (1<<18)
#define USBC_EPMISMSK              (1<<17)
#define USBC_GINTMSK_RESERVED_2    (1<<16)
#define USBC_EOPFMSK               (1<<15)
#define USBC_ISOOUTDROPMSK         (1<<14)
#define USBC_ENUMDONEMSK           (1<<13)
#define USBC_USBRSTMSK             (1<<12)
#define USBC_USBSUSPMSK            (1<<11)
#define USBC_ERLYSUSPMSK           (1<<10)
#define USBC_GINTMSK_RESERVED_1(n) (((n)&3)<<8)
#define USBC_GOUTNAKEFFMSK         (1<<7)
#define USBC_GINNAKEFFMSK          (1<<6)
#define USBC_NPTXFEMPMSK           (1<<5)
#define USBC_RXFLVLMSK             (1<<4)
#define USBC_SOFMSK                (1<<3)
#define USBC_OTGINTMSK             (1<<2)
#define USBC_MODEMISMSK            (1<<1)
#define USBC_GINTMSK_RESERVED_0    (1<<0)

//GRXSTSR
#define USBC_GRXSTSR_RESERVED_0(n) (((n)&0x7F)<<25)
#define USBC_FN(n)                 (((n)&15)<<21)
#define USBC_PKTSTS(n)             (((n)&15)<<17)
#define USBC_DPID(n)               (((n)&3)<<15)
#define USBC_BCNT(n)               (((n)&0x7FF)<<4)
#define USBC_CHNUM(n)              (((n)&15)<<0)

//GRXSTSP
#define USBC_GRXSTSP_RESERVED_0(n) (((n)&0x7F)<<25)
//#define USBC_FN(n)               (((n)&15)<<21)
//#define USBC_PKTSTS(n)           (((n)&15)<<17)
//#define USBC_DPID(n)             (((n)&3)<<15)
//#define USBC_BCNT(n)             (((n)&0x7FF)<<4)
//#define USBC_CHNUM(n)            (((n)&15)<<0)

//GRXFSIZ
#define USBC_GRXFSIZ_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define USBC_RXFDEP(n)             (((n)&0xFFF)<<0)

//GNPTXFSIZ
#define USBC_GNPTXFSIZ_RESERVED_1(n) (((n)&15)<<28)
#define USBC_NPTXFDEP(n)           (((n)&0xFFF)<<16)
#define USBC_GNPTXFSIZ_RESERVED_0(n) (((n)&15)<<12)
#define USBC_NPTXFSTADDR(n)        (((n)&0xFFF)<<0)

//GNPTXSTS
#define USBC_GNPTXSTS_RESERVED_0   (1<<31)
#define USBC_NPTXQTOP(n)           (((n)&0x7F)<<24)
#define USBC_NPTXQSPCAVAIL(n)      (((n)&0xFF)<<16)
#define USBC_NPTXFSPCAVAIL(n)      (((n)&0xFFFF)<<0)

//GGPIO
#define USBC_GPO(n)                (((n)&0xFFFF)<<16)
#define USBC_GPI(n)                (((n)&0xFFFF)<<0)

//GUID
#define USBC_GUID(n)               (((n)&0xFFFFFFFF)<<0)

//GSNPSID
#define USBC_SYNOPSYSID(n)         (((n)&0xFFFFFFFF)<<0)

//GHWCFG1
#define USBC_EPDIR(n)              (((n)&0xFFFFFFFF)<<0)

//GHWCFG2
#define USBC_GHWCFG2_RESERVED_1    (1<<31)
#define USBC_TKNQDEPTH(n)          (((n)&31)<<26)
#define USBC_PTXQDEPTH(n)          (((n)&3)<<24)
#define USBC_NPTXQDEPTH(n)         (((n)&3)<<22)
#define USBC_GHWCFG2_RESERVED_0    (1<<21)
#define USBC_MULTIPROCINTRPT       (1<<20)
#define USBC_DYNFIFOSIZING         (1<<19)
#define USBC_PERIOSUPPORT          (1<<18)
#define USBC_NUMHSTCHNL(n)         (((n)&15)<<14)
#define USBC_NUMDEVEPS(n)          (((n)&15)<<10)
#define USBC_FSPHYTYPE(n)          (((n)&3)<<8)
#define USBC_HSPHYTYPE(n)          (((n)&3)<<6)
#define USBC_SINGPNT               (1<<5)
#define USBC_OTGARCH(n)            (((n)&3)<<3)
#define USBC_OTGMODE(n)            (((n)&7)<<0)

//GHWCFG3
#define USBC_DFIFODEPTH(n)         (((n)&0xFFFF)<<16)
#define USBC_LPMMODE               (1<<15)
#define USBC_BCSUPPORT             (1<<14)
#define USBC_HSICMODE              (1<<13)
#define USBC_ADPSUPPORT            (1<<12)
#define USBC_RSTTYPE               (1<<11)
#define USBC_OPTFEATURE            (1<<10)
#define USBC_VNDCTLSUPT            (1<<9)
#define USBC_I2CINTSEL             (1<<8)
#define USBC_OTGEN                 (1<<7)
#define USBC_PKTSIZEWIDTH(n)       (((n)&7)<<4)
#define USBC_XFERSIZEWIDTH(n)      (((n)&15)<<0)

//GHWCFG4
#define USBC_DESCDMA               (1<<31)
#define USBC_DESCDMAENABLED        (1<<30)
#define USBC_INEPS(n)              (((n)&15)<<26)
#define USBC_DEDFIFOMODE           (1<<25)
#define USBC_SESSENDFLTR           (1<<24)
#define USBC_BVALIDFLTR            (1<<23)
#define USBC_AVALIDFLTR            (1<<22)
#define USBC_VBUSVALIDFLTR         (1<<21)
#define USBC_IDDGFLTR              (1<<20)
#define USBC_NUMCTLEPS(n)          (((n)&15)<<16)
#define USBC_PHYDATAWIDTH(n)       (((n)&3)<<14)
#define USBC_ENHANCEDLPMSUPT       (1<<13)
#define USBC_ACGSUPT               (1<<12)
#define USBC_GHWCFG4_RESERVED_0(n) (((n)&15)<<8)
#define USBC_EXTENDEDHIBERNATION   (1<<7)
#define USBC_HIBERNATION           (1<<6)
#define USBC_AHBFREQ               (1<<5)
#define USBC_PARTIALPWRDN          (1<<4)
#define USBC_NUMDEVPERIOEPS(n)     (((n)&15)<<0)

//GPWRDN
#define USBC_GPWRDN_RESERVED_2(n)  (((n)&0xFF)<<24)
#define USBC_ADPINT                (1<<23)
#define USBC_BSESSVLD              (1<<22)
#define USBC_IDDIG                 (1<<21)
#define USBC_LINESTATE(n)          (((n)&3)<<19)
#define USBC_STSCHNGINTMSK         (1<<18)
#define USBC_STSCHNGINT            (1<<17)
#define USBC_SRPDETECTMSK          (1<<16)
#define USBC_SRPDETECT             (1<<15)
#define USBC_GPWRDN_RESERVED_1(n)  (((n)&0xFF)<<7)
#define USBC_DISABLEVBUS           (1<<6)
#define USBC_PWRDNSWTCH            (1<<5)
#define USBC_PWRDNRST_N            (1<<4)
#define USBC_PWRDNCLMP             (1<<3)
#define USBC_GPWRDN_RESERVED_0     (1<<2)
#define USBC_PMUACTV               (1<<1)
#define USBC_PMUINTSEL             (1<<0)

//GDFIFOCFG
#define USBC_EPINFOBASEADDR(n)     (((n)&0xFFFF)<<16)
#define USBC_GDFIFOCFG(n)          (((n)&0xFFFF)<<0)

//GADPCTL
#define USBC_GADPCTL_RESERVED_0(n) (((n)&7)<<29)
#define USBC_AR(n)                 (((n)&3)<<27)
#define USBC_ADPTOUTMSK            (1<<26)
#define USBC_ADPSNSINTMSK          (1<<25)
#define USBC_ADPPRBINTMSK          (1<<24)
#define USBC_ADPTOUTINT            (1<<23)
#define USBC_ADPSNSINT             (1<<22)
#define USBC_ADPPRBINT             (1<<21)
#define USBC_ADPEN                 (1<<20)
#define USBC_ADPRES                (1<<19)
#define USBC_ENASNS                (1<<18)
#define USBC_ENAPRB                (1<<17)
#define USBC_RTIM(n)               (((n)&0x7FF)<<6)
#define USBC_PRBPER(n)             (((n)&3)<<4)
#define USBC_PRBDELTA(n)           (((n)&3)<<2)
#define USBC_PRBDSCHG(n)           (((n)&3)<<0)

//HPTXFSIZ
#define USBC_HPTXFSIZ_RESERVED_1(n) (((n)&15)<<28)
#define USBC_PTXFSIZE(n)           (((n)&0xFFF)<<16)
#define USBC_HPTXFSIZ_RESERVED_0(n) (((n)&7)<<13)
#define USBC_PTXFSTADDR(n)         (((n)&0x1FFF)<<0)

//DIEPTXF1
#define USBC_DIEPTXF1_RESERVED_1(n) (((n)&0x3F)<<26)
#define USBC_INEPNTXFDEP(n)        (((n)&0x3FF)<<16)
#define USBC_DIEPTXF1_RESERVED_0(n) (((n)&15)<<12)
#define USBC_INEPNTXFSTADDR(n)     (((n)&0xFFF)<<0)

//DIEPTXF2
#define USBC_DIEPTXF2_RESERVED_1(n) (((n)&0x3F)<<26)
//#define USBC_INEPNTXFDEP(n)      (((n)&0x3FF)<<16)
#define USBC_DIEPTXF2_RESERVED_0(n) (((n)&7)<<13)
#define USBC_INEPNTXFSTADDR_X1(n)  (((n)&0x1FFF)<<0)

//DIEPTXF3
#define USBC_DIEPTXF3_RESERVED_1(n) (((n)&0x7F)<<25)
#define USBC_INEPNTXFDEP_X1(n)     (((n)&0x1FF)<<16)
#define USBC_DIEPTXF3_RESERVED_0(n) (((n)&7)<<13)
//#define USBC_INEPNTXFSTADDR_X1(n) (((n)&0x1FFF)<<0)

//DIEPTXF4
#define USBC_DIEPTXF4_RESERVED_1(n) (((n)&0x7F)<<25)
//#define USBC_INEPNTXFDEP_X1(n)   (((n)&0x1FF)<<16)
#define USBC_DIEPTXF4_RESERVED_0(n) (((n)&7)<<13)
//#define USBC_INEPNTXFSTADDR_X1(n) (((n)&0x1FFF)<<0)

//DIEPTXF5
#define USBC_DIEPTXF5_RESERVED_1(n) (((n)&0x7F)<<25)
//#define USBC_INEPNTXFDEP_X1(n)   (((n)&0x1FF)<<16)
#define USBC_DIEPTXF5_RESERVED_0(n) (((n)&7)<<13)
//#define USBC_INEPNTXFSTADDR_X1(n) (((n)&0x1FFF)<<0)

//DIEPTXF6
#define USBC_DIEPTXF6_RESERVED_1(n) (((n)&0x7F)<<25)
//#define USBC_INEPNTXFDEP_X1(n)   (((n)&0x1FF)<<16)
#define USBC_DIEPTXF6_RESERVED_0(n) (((n)&7)<<13)
//#define USBC_INEPNTXFSTADDR_X1(n) (((n)&0x1FFF)<<0)

//DIEPTXF7
#define USBC_DIEPTXF7_RESERVED_1(n) (((n)&0x7F)<<25)
//#define USBC_INEPNTXFDEP_X1(n)   (((n)&0x1FF)<<16)
#define USBC_DIEPTXF7_RESERVED_0(n) (((n)&7)<<13)
//#define USBC_INEPNTXFSTADDR_X1(n) (((n)&0x1FFF)<<0)

//DIEPTXF8
#define USBC_DIEPTXF8_RESERVED_1(n) (((n)&0x7F)<<25)
//#define USBC_INEPNTXFDEP_X1(n)   (((n)&0x1FF)<<16)
#define USBC_DIEPTXF8_RESERVED_0(n) (((n)&7)<<13)
//#define USBC_INEPNTXFSTADDR_X1(n) (((n)&0x1FFF)<<0)

//HCFG
#define USBC_MODECHTIMEN           (1<<31)
#define USBC_HCFG_RESERVED_2(n)    (((n)&15)<<27)
#define USBC_PERSCHEDENA           (1<<26)
#define USBC_FRLISTEN(n)           (((n)&3)<<24)
#define USBC_DESCDMA_X1            (1<<23)
#define USBC_HCFG_RESERVED_1(n)    (((n)&0x7F)<<16)
#define USBC_RESVALID(n)           (((n)&0xFF)<<8)
#define USBC_ENA32KHZS             (1<<7)
#define USBC_HCFG_RESERVED_0(n)    (((n)&15)<<3)
#define USBC_FSLSSUPP              (1<<2)
#define USBC_FSLSPCLKSEL(n)        (((n)&3)<<0)

//HFIR
#define USBC_HFIR_RESERVED_0(n)    (((n)&0x7FFF)<<17)
#define USBC_HFIRRLDCTRL           (1<<16)
#define USBC_FRINT(n)              (((n)&0xFFFF)<<0)

//HFNUM
#define USBC_FRREM(n)              (((n)&0xFFFF)<<16)
#define USBC_FRNUM(n)              (((n)&0xFFFF)<<0)

//HPTXSTS
#define USBC_PTXQTOP(n)            (((n)&0xFF)<<24)
#define USBC_PTXQSPCAVAIL(n)       (((n)&0xFF)<<16)
#define USBC_PTXFSPCAVAIL(n)       (((n)&0xFFFF)<<0)

//HAINT
#define USBC_HAINT_RESERVED_0(n)   (((n)&0xFFFF)<<16)
#define USBC_HAINT(n)              (((n)&0xFFFF)<<0)

//HAINTMSK
#define USBC_HAINTMSK_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define USBC_HAINTMSK(n)           (((n)&0xFFFF)<<0)

//HFLBAddr
#define USBC_HFLBADDR(n)           (((n)&0xFFFFFFFF)<<0)

//HPRT
#define USBC_HPRT_RESERVED_1(n)    (((n)&0x1FFF)<<19)
#define USBC_PRTSPD(n)             (((n)&3)<<17)
#define USBC_PRTTSTCTL(n)          (((n)&15)<<13)
#define USBC_PRTPWR                (1<<12)
#define USBC_PRTLNSTS(n)           (((n)&3)<<10)
#define USBC_HPRT_RESERVED_0       (1<<9)
#define USBC_PRTRST                (1<<8)
#define USBC_PRTSUSP               (1<<7)
#define USBC_PRTRES                (1<<6)
#define USBC_PRTOVRCURRCHNG        (1<<5)
#define USBC_PRTOVRCURRACT         (1<<4)
#define USBC_PRTENCHNG             (1<<3)
#define USBC_PRTENA                (1<<2)
#define USBC_PRTCONNDET            (1<<1)
#define USBC_PRTCONNSTS            (1<<0)

//HCCHAR0
#define USBC_CHENA                 (1<<31)
#define USBC_CHDIS                 (1<<30)
#define USBC_ODDFRM                (1<<29)
#define USBC_DEVADDR(n)            (((n)&0x7F)<<22)
#define USBC_EC(n)                 (((n)&3)<<20)
#define USBC_EPTYPE(n)             (((n)&3)<<18)
#define USBC_LSPDDEV               (1<<17)
#define USBC_HCCHAR0_RESERVED_0    (1<<16)
#define USBC_EPDIR_X1              (1<<15)
#define USBC_EPNUM(n)              (((n)&15)<<11)
#define USBC_MPS(n)                (((n)&0x7FF)<<0)

//HCSPLT0
#define USBC_SPLTENA               (1<<31)
#define USBC_HCSPLT0_RESERVED_0(n) (((n)&0x3FFF)<<17)
#define USBC_COMPSPLT              (1<<16)
#define USBC_XACTPOS(n)            (((n)&3)<<14)
#define USBC_HUBADDR(n)            (((n)&0x7F)<<7)
#define USBC_PRTADDR(n)            (((n)&0x7F)<<0)

//HCINT0
#define USBC_HCINT0_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
#define USBC_DESC_LST_ROLLINTR     (1<<13)
#define USBC_XCS_XACT_ERR          (1<<12)
#define USBC_BNAINTR               (1<<11)
#define USBC_DATATGLERR            (1<<10)
#define USBC_FRMOVRUN              (1<<9)
#define USBC_BBLERR                (1<<8)
#define USBC_XACTERR               (1<<7)
#define USBC_NYET                  (1<<6)
#define USBC_ACK                   (1<<5)
#define USBC_NAK                   (1<<4)
#define USBC_STALL                 (1<<3)
#define USBC_AHBERR                (1<<2)
#define USBC_CHHLTD                (1<<1)
#define USBC_XFERCOMPL             (1<<0)

//HCINTMSK0
#define USBC_HCINTMSK0_RESERVED_2(n) (((n)&0x3FFFF)<<14)
#define USBC_DESC_LST_ROLLINTRMSK  (1<<13)
#define USBC_HCINTMSK0_RESERVED_1  (1<<12)
#define USBC_BNAINTRMSK            (1<<11)
#define USBC_HCINTMSK0_RESERVED_0(n) (((n)&0xFF)<<3)
#define USBC_AHBERRMSK             (1<<2)
#define USBC_CHHLTDMSK             (1<<1)
#define USBC_XFERCOMPLMSK          (1<<0)

//HCTSIZ0
#define USBC_DOPNG                 (1<<31)
#define USBC_PID(n)                (((n)&3)<<29)
#define USBC_PKTCNT(n)             (((n)&0x3FF)<<19)
#define USBC_XFERSIZE(n)           (((n)&0x7FFFF)<<0)

//HCDMA0
#define USBC_DMAADDR(n)            (((n)&0xFFFFFFFF)<<0)

//HCDMAB0
#define USBC_HCDMAB(n)             (((n)&0xFFFFFFFF)<<0)

//HCCHAR1
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR1_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT1
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT1_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT1
#define USBC_HCINT1_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK1
#define USBC_HCINTMSK1_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK1_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK1_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ1
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA1
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB1
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR2
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR2_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT2
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT2_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT2
#define USBC_HCINT2_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK2
#define USBC_HCINTMSK2_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK2_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK2_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ2
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA2
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB2
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR3
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR3_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT3
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT3_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT3
#define USBC_HCINT3_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK3
#define USBC_HCINTMSK3_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK3_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK3_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ3
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA3
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB3
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR4
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR4_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT4
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT4_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT4
#define USBC_HCINT4_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK4
#define USBC_HCINTMSK4_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK4_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK4_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ4
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA4
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB4
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR5
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR5_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT5
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT5_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT5
#define USBC_HCINT5_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK5
#define USBC_HCINTMSK5_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK5_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK5_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ5
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA5
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB5
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR6
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR6_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT6
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT6_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT6
#define USBC_HCINT6_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK6
#define USBC_HCINTMSK6_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK6_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK6_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ6
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA6
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB6
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR7
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR7_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT7
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT7_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT7
#define USBC_HCINT7_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK7
#define USBC_HCINTMSK7_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK7_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK7_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ7
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA7
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB7
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR8
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR8_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT8
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT8_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT8
#define USBC_HCINT8_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK8
#define USBC_HCINTMSK8_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK8_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK8_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ8
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA8
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB8
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR9
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR9_RESERVED_0    (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT9
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT9_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT9
#define USBC_HCINT9_RESERVED_0(n)  (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK9
#define USBC_HCINTMSK9_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK9_RESERVED_1  (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK9_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ9
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA9
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB9
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR10
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR10_RESERVED_0   (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT10
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT10_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT10
#define USBC_HCINT10_RESERVED_0(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK10
#define USBC_HCINTMSK10_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK10_RESERVED_1 (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK10_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ10
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA10
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB10
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR11
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR11_RESERVED_0   (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT11
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT11_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT11
#define USBC_HCINT11_RESERVED_0(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK11
#define USBC_HCINTMSK11_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK11_RESERVED_1 (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK11_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ11
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA11
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB11
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR12
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR12_RESERVED_0   (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT12
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT12_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT12
#define USBC_HCINT12_RESERVED_0(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK12
#define USBC_HCINTMSK12_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK12_RESERVED_1 (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK12_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ12
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA12
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB12
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR13
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR13_RESERVED_0   (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT13
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT13_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT13
#define USBC_HCINT13_RESERVED_0(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK13
#define USBC_HCINTMSK13_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK13_RESERVED_1 (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK13_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ13
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA13
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB13
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR14
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR14_RESERVED_0   (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT14
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT14_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT14
#define USBC_HCINT14_RESERVED_0(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK14
#define USBC_HCINTMSK14_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK14_RESERVED_1 (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK14_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ14
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA14
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB14
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//HCCHAR15
//#define USBC_CHENA               (1<<31)
//#define USBC_CHDIS               (1<<30)
//#define USBC_ODDFRM              (1<<29)
//#define USBC_DEVADDR(n)          (((n)&0x7F)<<22)
//#define USBC_EC(n)               (((n)&3)<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_LSPDDEV             (1<<17)
#define USBC_HCCHAR15_RESERVED_0   (1<<16)
//#define USBC_EPDIR_X1            (1<<15)
//#define USBC_EPNUM(n)            (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//HCSPLT15
//#define USBC_SPLTENA             (1<<31)
#define USBC_HCSPLT15_RESERVED_0(n) (((n)&0x3FFF)<<17)
//#define USBC_COMPSPLT            (1<<16)
//#define USBC_XACTPOS(n)          (((n)&3)<<14)
//#define USBC_HUBADDR(n)          (((n)&0x7F)<<7)
//#define USBC_PRTADDR(n)          (((n)&0x7F)<<0)

//HCINT15
#define USBC_HCINT15_RESERVED_0(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTR   (1<<13)
//#define USBC_XCS_XACT_ERR        (1<<12)
//#define USBC_BNAINTR             (1<<11)
//#define USBC_DATATGLERR          (1<<10)
//#define USBC_FRMOVRUN            (1<<9)
//#define USBC_BBLERR              (1<<8)
//#define USBC_XACTERR             (1<<7)
//#define USBC_NYET                (1<<6)
//#define USBC_ACK                 (1<<5)
//#define USBC_NAK                 (1<<4)
//#define USBC_STALL               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_CHHLTD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//HCINTMSK15
#define USBC_HCINTMSK15_RESERVED_2(n) (((n)&0x3FFFF)<<14)
//#define USBC_DESC_LST_ROLLINTRMSK (1<<13)
#define USBC_HCINTMSK15_RESERVED_1 (1<<12)
//#define USBC_BNAINTRMSK          (1<<11)
#define USBC_HCINTMSK15_RESERVED_0(n) (((n)&0xFF)<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_CHHLTDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//HCTSIZ15
//#define USBC_DOPNG               (1<<31)
//#define USBC_PID(n)              (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//HCDMA15
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//HCDMAB15
//#define USBC_HCDMAB(n)           (((n)&0xFFFFFFFF)<<0)

//DCFG
#define USBC_RESVALID_X1(n)        (((n)&0x3F)<<26)
#define USBC_PERSCHINTVL(n)        (((n)&3)<<24)
//#define USBC_DESCDMA_X1          (1<<23)
#define USBC_DCFG_RESERVED_0(n)    (((n)&0x7F)<<16)
#define USBC_ERRATICINTMSK         (1<<15)
#define USBC_XCVRDLY               (1<<14)
#define USBC_ENDEVOUTNAK           (1<<13)
#define USBC_PERFRINT(n)           (((n)&3)<<11)
#define USBC_DEVADDR_X1(n)         (((n)&0x7F)<<4)
#define USBC_ENA32KHZSUSP          (1<<3)
#define USBC_NZSTSOUTHSHK          (1<<2)
#define USBC_DEVSPD(n)             (((n)&3)<<0)

//DCTL
#define USBC_DCTL_RESERVED_1(n)    (((n)&0x3FFF)<<18)
#define USBC_ENCONTONBNA           (1<<17)
#define USBC_NAKONBBLE             (1<<16)
#define USBC_IGNRFRMNUM            (1<<15)
#define USBC_GMC(n)                (((n)&3)<<13)
#define USBC_DCTL_RESERVED_0       (1<<12)
#define USBC_PWRONPRGDONE          (1<<11)
#define USBC_CGOUTNAK              (1<<10)
#define USBC_SGOUTNAK              (1<<9)
#define USBC_CGNPINNAK             (1<<8)
#define USBC_SGNPINNAK             (1<<7)
#define USBC_TSTCTL(n)             (((n)&7)<<4)
#define USBC_GOUTNAKSTS            (1<<3)
#define USBC_GNPINNAKSTS           (1<<2)
#define USBC_SFTDISCON             (1<<1)
#define USBC_RMTWKUPSIG            (1<<0)

//DSTS
#define USBC_DSTS_RESERVED_1(n)    (((n)&0xFF)<<24)
#define USBC_DEVLNSTS(n)           (((n)&3)<<22)
#define USBC_SOFFN(n)              (((n)&0x3FFF)<<8)
#define USBC_DSTS_RESERVED_0(n)    (((n)&15)<<4)
#define USBC_ERRTICERR             (1<<3)
#define USBC_ENUMSPD(n)            (((n)&3)<<1)
#define USBC_SUSPSTS               (1<<0)

//DIEPMSK
#define USBC_DIEPMSK_RESERVED_2(n) (((n)&0x3FFFF)<<14)
#define USBC_NAKMSK                (1<<13)
#define USBC_DIEPMSK_RESERVED_1(n) (((n)&7)<<10)
#define USBC_BNAININTRMSK          (1<<9)
#define USBC_TXFIFOUNDRNMSK        (1<<8)
#define USBC_DIEPMSK_RESERVED_0    (1<<7)
#define USBC_INEPNAKEFFMSK         (1<<6)
#define USBC_INTKNEPMISMSK         (1<<5)
#define USBC_INTKNTXFEMPMSK        (1<<4)
#define USBC_TIMEOUTMSK            (1<<3)
//#define USBC_AHBERRMSK           (1<<2)
#define USBC_EPDISBLDMSK           (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//DOEPMSK
#define USBC_DOEPMSK_RESERVED_2(n) (((n)&0x1FFFF)<<15)
#define USBC_NYETMSK               (1<<14)
//#define USBC_NAKMSK              (1<<13)
#define USBC_BBLEERRMSK            (1<<12)
#define USBC_DOEPMSK_RESERVED_1(n) (((n)&3)<<10)
#define USBC_BNAOUTINTRMSK         (1<<9)
#define USBC_OUTPKTERRMSK          (1<<8)
#define USBC_DOEPMSK_RESERVED_0    (1<<7)
#define USBC_BACK2BACKSETUP        (1<<6)
#define USBC_STSPHSERCVDMSK        (1<<5)
#define USBC_OUTTKNEPDISMSK        (1<<4)
#define USBC_SETUPMSK              (1<<3)
//#define USBC_AHBERRMSK           (1<<2)
//#define USBC_EPDISBLDMSK         (1<<1)
//#define USBC_XFERCOMPLMSK        (1<<0)

//DAINT
#define USBC_DAINT_RESERVED_1(n)   (((n)&7)<<29)
#define USBC_OUTEPINT12            (1<<28)
#define USBC_OUTEPINT11            (1<<27)
#define USBC_OUTEPINT10            (1<<26)
#define USBC_OUTEPINT9             (1<<25)
#define USBC_OUTEPINT8             (1<<24)
#define USBC_OUTEPINT7             (1<<23)
#define USBC_OUTEPINT6             (1<<22)
#define USBC_OUTEPINT5             (1<<21)
#define USBC_OUTEPINT4             (1<<20)
#define USBC_OUTEPINT3             (1<<19)
#define USBC_OUTEPINT2             (1<<18)
#define USBC_OUTEPINT1             (1<<17)
#define USBC_OUTEPINT0             (1<<16)
#define USBC_DAINT_RESERVED_0(n)   (((n)&7)<<13)
#define USBC_INEPINT12             (1<<12)
#define USBC_INEPINT11             (1<<11)
#define USBC_INEPINT10             (1<<10)
#define USBC_INEPINT9              (1<<9)
#define USBC_INEPINT8              (1<<8)
#define USBC_INEPINT7              (1<<7)
#define USBC_INEPINT6              (1<<6)
#define USBC_INEPINT5              (1<<5)
#define USBC_INEPINT4              (1<<4)
#define USBC_INEPINT3              (1<<3)
#define USBC_INEPINT2              (1<<2)
#define USBC_INEPINT1              (1<<1)
#define USBC_INEPINT0              (1<<0)

//DAINTMSK
#define USBC_DAINTMSK_RESERVED_1(n) (((n)&7)<<29)
#define USBC_OUTEPMSK12            (1<<28)
#define USBC_OUTEPMSK11            (1<<27)
#define USBC_OUTEPMSK10            (1<<26)
#define USBC_OUTEPMSK9             (1<<25)
#define USBC_OUTEPMSK8             (1<<24)
#define USBC_OUTEPMSK7             (1<<23)
#define USBC_OUTEPMSK6             (1<<22)
#define USBC_OUTEPMSK5             (1<<21)
#define USBC_OUTEPMSK4             (1<<20)
#define USBC_OUTEPMSK3             (1<<19)
#define USBC_OUTEPMSK2             (1<<18)
#define USBC_OUTEPMSK1             (1<<17)
#define USBC_OUTEPMSK0             (1<<16)
#define USBC_DAINTMSK_RESERVED_0(n) (((n)&7)<<13)
#define USBC_INEPMSK12             (1<<12)
#define USBC_INEPMSK11             (1<<11)
#define USBC_INEPMSK10             (1<<10)
#define USBC_INEPMSK9              (1<<9)
#define USBC_INEPMSK8              (1<<8)
#define USBC_INEPMSK7              (1<<7)
#define USBC_INEPMSK6              (1<<6)
#define USBC_INEPMSK5              (1<<5)
#define USBC_INEPMSK4              (1<<4)
#define USBC_INEPMSK3              (1<<3)
#define USBC_INEPMSK2              (1<<2)
#define USBC_INEPMSK1              (1<<1)
#define USBC_INEPMSK0              (1<<0)

//DVBUSDIS
#define USBC_DVBUSDIS_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define USBC_DVBUSDIS(n)           (((n)&0xFFFF)<<0)

//DVBUSPULSE
#define USBC_DVBUSPULSE_RESERVED_0(n) (((n)&0xFFFFF)<<12)
#define USBC_DVBUSPULSE(n)         (((n)&0xFFF)<<0)

//DTHRCTL
#define USBC_DTHRCTL_RESERVED_2(n) (((n)&15)<<28)
#define USBC_ARBPRKEN              (1<<27)
#define USBC_DTHRCTL_RESERVED_1    (1<<26)
#define USBC_RXTHRLEN(n)           (((n)&0x1FF)<<17)
#define USBC_RXTHREN               (1<<16)
#define USBC_DTHRCTL_RESERVED_0(n) (((n)&7)<<13)
#define USBC_AHBTHRRATIO(n)        (((n)&3)<<11)
#define USBC_TXTHRLEN(n)           (((n)&0x1FF)<<2)
#define USBC_ISOTHREN              (1<<1)
#define USBC_NONISOTHREN           (1<<0)

//DIEPEMPMSK
#define USBC_DIEPEMPMSK_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define USBC_INEPTXFEMPMSK(n)      (((n)&0xFFFF)<<0)

//DIEPCTL0
#define USBC_EPENA                 (1<<31)
#define USBC_EPDIS                 (1<<30)
#define USBC_DIEPCTL0_RESERVED_3(n) (((n)&3)<<28)
#define USBC_SNAK                  (1<<27)
#define USBC_CNAK                  (1<<26)
#define USBC_TXFNUM_X1(n)          (((n)&15)<<22)
#define USBC_STALL_X1              (1<<21)
#define USBC_DIEPCTL0_RESERVED_2   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
#define USBC_NAKSTS                (1<<17)
#define USBC_DIEPCTL0_RESERVED_1   (1<<16)
#define USBC_USBACTEP              (1<<15)
#define USBC_DIEPCTL0_RESERVED_0(n) (((n)&0x1FFF)<<2)
#define USBC_MPS_X1(n)             (((n)&3)<<0)

//DIEPINT0
#define USBC_DIEPINT0_RESERVED_1(n) (((n)&0x1FFFF)<<15)
#define USBC_NYETINTRPT            (1<<14)
#define USBC_NAKINTRPT             (1<<13)
#define USBC_BBLEERR               (1<<12)
#define USBC_PKTDRPSTS             (1<<11)
#define USBC_DIEPINT0_RESERVED_0   (1<<10)
#define USBC_BNAINTR_X1            (1<<9)
#define USBC_TXFIFOUNDRN           (1<<8)
#define USBC_TXFEMP                (1<<7)
#define USBC_INEPNAKEFF            (1<<6)
#define USBC_INTKNEPMIS            (1<<5)
#define USBC_INTKNTXFEMP           (1<<4)
#define USBC_TIMEOUT               (1<<3)
//#define USBC_AHBERR              (1<<2)
#define USBC_EPDISBLD              (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ0
#define USBC_DIEPTSIZ0_RESERVED_1(n) (((n)&0x7FF)<<21)
#define USBC_PKTCNT_X1(n)          (((n)&3)<<19)
#define USBC_DIEPTSIZ0_RESERVED_0(n) (((n)&0xFFF)<<7)
#define USBC_XFERSIZE_X1(n)        (((n)&0x7F)<<0)

//DIEPDMA0
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS0
#define USBC_DTXFSTS0_RESERVED_0(n) (((n)&0xFFFF)<<16)
#define USBC_INEPTXFSPCAVAIL(n)    (((n)&0xFFFF)<<0)

//DIEPDMAB0
#define USBC_DMABUFFERADDR(n)      (((n)&0xFFFFFFFF)<<0)

//DIEPCTL1
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
#define USBC_SETD1PID              (1<<29)
#define USBC_SETD0PID              (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL1_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
#define USBC_DPID_X1               (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL1_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT1
#define USBC_DIEPINT1_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT1_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ1
#define USBC_DIEPTSIZ1_RESERVED_0  (1<<31)
#define USBC_MC(n)                 (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA1
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS1
#define USBC_DTXFSTS1_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB1
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL2
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL2_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL2_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT2
#define USBC_DIEPINT2_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT2_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ2
#define USBC_DIEPTSIZ2_RESERVED_0  (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA2
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS2
#define USBC_DTXFSTS2_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB2
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL3
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL3_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL3_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT3
#define USBC_DIEPINT3_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT3_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ3
#define USBC_DIEPTSIZ3_RESERVED_0  (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA3
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS3
#define USBC_DTXFSTS3_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB3
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL4
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL4_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL4_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT4
#define USBC_DIEPINT4_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT4_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ4
#define USBC_DIEPTSIZ4_RESERVED_0  (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA4
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS4
#define USBC_DTXFSTS4_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB4
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL5
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL5_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL5_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT5
#define USBC_DIEPINT5_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT5_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ5
#define USBC_DIEPTSIZ5_RESERVED_0  (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA5
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS5
#define USBC_DTXFSTS5_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB5
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL6
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL6_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL6_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT6
#define USBC_DIEPINT6_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT6_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ6
#define USBC_DIEPTSIZ6_RESERVED_0  (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA6
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS6
#define USBC_DTXFSTS6_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB6
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL7
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL7_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL7_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT7
#define USBC_DIEPINT7_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT7_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ7
#define USBC_DIEPTSIZ7_RESERVED_0  (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA7
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS7
#define USBC_DTXFSTS7_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB7
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL8
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL8_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL8_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT8
#define USBC_DIEPINT8_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT8_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ8
#define USBC_DIEPTSIZ8_RESERVED_0  (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA8
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS8
#define USBC_DTXFSTS8_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB8
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL9
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL9_RESERVED_1   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL9_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT9
#define USBC_DIEPINT9_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT9_RESERVED_0   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ9
#define USBC_DIEPTSIZ9_RESERVED_0  (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA9
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS9
#define USBC_DTXFSTS9_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB9
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL10
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL10_RESERVED_1  (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL10_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT10
#define USBC_DIEPINT10_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT10_RESERVED_0  (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ10
#define USBC_DIEPTSIZ10_RESERVED_0 (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA10
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS10
#define USBC_DTXFSTS10_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB10
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL11
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL11_RESERVED_1  (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL11_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT11
#define USBC_DIEPINT11_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT11_RESERVED_0  (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ11
#define USBC_DIEPTSIZ11_RESERVED_0 (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA11
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS11
#define USBC_DTXFSTS11_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB11
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DIEPCTL12
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
//#define USBC_TXFNUM_X1(n)        (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_DIEPCTL12_RESERVED_1  (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DIEPCTL12_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DIEPINT12
#define USBC_DIEPINT12_RESERVED_1(n) (((n)&0x1FFFF)<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DIEPINT12_RESERVED_0  (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_TXFIFOUNDRN         (1<<8)
//#define USBC_TXFEMP              (1<<7)
//#define USBC_INEPNAKEFF          (1<<6)
//#define USBC_INTKNEPMIS          (1<<5)
//#define USBC_INTKNTXFEMP         (1<<4)
//#define USBC_TIMEOUT             (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DIEPTSIZ12
#define USBC_DIEPTSIZ12_RESERVED_0 (1<<31)
//#define USBC_MC(n)               (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DIEPDMA12
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DTXFSTS12
#define USBC_DTXFSTS12_RESERVED_0(n) (((n)&0xFFFF)<<16)
//#define USBC_INEPTXFSPCAVAIL(n)  (((n)&0xFFFF)<<0)

//DIEPDMAB12
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL0
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
#define USBC_DOEPCTL0_RESERVED_3(n) (((n)&3)<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL0_RESERVED_2(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
#define USBC_SNP                   (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
#define USBC_DOEPCTL0_RESERVED_1   (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL0_RESERVED_0(n) (((n)&0x1FFF)<<2)
//#define USBC_MPS_X1(n)           (((n)&3)<<0)

//DOEPINT0
#define USBC_DOEPINT0_RESERVED_2(n) (((n)&0xFFFF)<<16)
#define USBC_STUPPKTRCVD           (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT0_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
#define USBC_OUTPKTERR             (1<<8)
#define USBC_DOEPINT0_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
#define USBC_STSPHSERCVD           (1<<5)
#define USBC_OUTTKNEPDIS           (1<<4)
#define USBC_SETUP                 (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ0
#define USBC_DOEPTSIZ0_RESERVED_2  (1<<31)
#define USBC_SUPCNT(n)             (((n)&3)<<29)
#define USBC_DOEPTSIZ0_RESERVED_1(n) (((n)&0x1FF)<<20)
#define USBC_PKTCNT_X2             (1<<19)
#define USBC_DOEPTSIZ0_RESERVED_0(n) (((n)&0xFFF)<<7)
//#define USBC_XFERSIZE_X1(n)      (((n)&0x7F)<<0)

//DOEPDMA0
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB0
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL1
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL1_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL1_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT1
#define USBC_DOEPINT1_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT1_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT1_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ1
#define USBC_DOEPTSIZ1_RESERVED_0  (1<<31)
#define USBC_RXDPID(n)             (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA1
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB1
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL2
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL2_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL2_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT2
#define USBC_DOEPINT2_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT2_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT2_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ2
#define USBC_DOEPTSIZ2_RESERVED_0  (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA2
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB2
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL3
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL3_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL3_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT3
#define USBC_DOEPINT3_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT3_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT3_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ3
#define USBC_DOEPTSIZ3_RESERVED_0  (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA3
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB3
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL4
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL4_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL4_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT4
#define USBC_DOEPINT4_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT4_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT4_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ4
#define USBC_DOEPTSIZ4_RESERVED_0  (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA4
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB4
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL5
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL5_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL5_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT5
#define USBC_DOEPINT5_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT5_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT5_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ5
#define USBC_DOEPTSIZ5_RESERVED_0  (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA5
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB5
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL6
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL6_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL6_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT6
#define USBC_DOEPINT6_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT6_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT6_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ6
#define USBC_DOEPTSIZ6_RESERVED_0  (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA6
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB6
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL7
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL7_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL7_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT7
#define USBC_DOEPINT7_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT7_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT7_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ7
#define USBC_DOEPTSIZ7_RESERVED_0  (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA7
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB7
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL8
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL8_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL8_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT8
#define USBC_DOEPINT8_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT8_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT8_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ8
#define USBC_DOEPTSIZ8_RESERVED_0  (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA8
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB8
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL9
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL9_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL9_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT9
#define USBC_DOEPINT9_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT9_RESERVED_1   (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT9_RESERVED_0   (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ9
#define USBC_DOEPTSIZ9_RESERVED_0  (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA9
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB9
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL10
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL10_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL10_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT10
#define USBC_DOEPINT10_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT10_RESERVED_1  (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT10_RESERVED_0  (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ10
#define USBC_DOEPTSIZ10_RESERVED_0 (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA10
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB10
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL11
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL11_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL11_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT11
#define USBC_DOEPINT11_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT11_RESERVED_1  (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT11_RESERVED_0  (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ11
#define USBC_DOEPTSIZ11_RESERVED_0 (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA11
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB11
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//DOEPCTL12
//#define USBC_EPENA               (1<<31)
//#define USBC_EPDIS               (1<<30)
//#define USBC_SETD1PID            (1<<29)
//#define USBC_SETD0PID            (1<<28)
//#define USBC_SNAK                (1<<27)
//#define USBC_CNAK                (1<<26)
#define USBC_DOEPCTL12_RESERVED_1(n) (((n)&15)<<22)
//#define USBC_STALL_X1            (1<<21)
//#define USBC_SNP                 (1<<20)
//#define USBC_EPTYPE(n)           (((n)&3)<<18)
//#define USBC_NAKSTS              (1<<17)
//#define USBC_DPID_X1             (1<<16)
//#define USBC_USBACTEP            (1<<15)
#define USBC_DOEPCTL12_RESERVED_0(n) (((n)&15)<<11)
//#define USBC_MPS(n)              (((n)&0x7FF)<<0)

//DOEPINT12
#define USBC_DOEPINT12_RESERVED_2(n) (((n)&0xFFFF)<<16)
//#define USBC_STUPPKTRCVD         (1<<15)
//#define USBC_NYETINTRPT          (1<<14)
//#define USBC_NAKINTRPT           (1<<13)
//#define USBC_BBLEERR             (1<<12)
//#define USBC_PKTDRPSTS           (1<<11)
#define USBC_DOEPINT12_RESERVED_1  (1<<10)
//#define USBC_BNAINTR_X1          (1<<9)
//#define USBC_OUTPKTERR           (1<<8)
#define USBC_DOEPINT12_RESERVED_0  (1<<7)
//#define USBC_BACK2BACKSETUP      (1<<6)
//#define USBC_STSPHSERCVD         (1<<5)
//#define USBC_OUTTKNEPDIS         (1<<4)
//#define USBC_SETUP               (1<<3)
//#define USBC_AHBERR              (1<<2)
//#define USBC_EPDISBLD            (1<<1)
//#define USBC_XFERCOMPL           (1<<0)

//DOEPTSIZ12
#define USBC_DOEPTSIZ12_RESERVED_0 (1<<31)
//#define USBC_RXDPID(n)           (((n)&3)<<29)
//#define USBC_PKTCNT(n)           (((n)&0x3FF)<<19)
//#define USBC_XFERSIZE(n)         (((n)&0x7FFFF)<<0)

//DOEPDMA12
//#define USBC_DMAADDR(n)          (((n)&0xFFFFFFFF)<<0)

//DOEPDMAB12
//#define USBC_DMABUFFERADDR(n)    (((n)&0xFFFFFFFF)<<0)

//PCGCCTL
#define USBC_PCGCCTL_RESERVED_2(n) (((n)&0xFFFFFF)<<8)
#define USBC_L1SUSPENDED           (1<<7)
#define USBC_PHYSLEEP              (1<<6)
#define USBC_PCGCCTL_RESERVED_1(n) (((n)&3)<<4)
#define USBC_RSTPDWNMODULE         (1<<3)
#define USBC_PCGCCTL_RESERVED_0(n) (((n)&3)<<1)
#define USBC_STOPPCLK              (1<<0)





#endif


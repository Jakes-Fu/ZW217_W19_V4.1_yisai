/*************************************************************************
 *
 * MODULE NAME:    bt_platform.c
 * PROJECT CODE:   rda5868
 * DESCRIPTION:    rda5868 platform file.
 * MAINTAINER:     lugongyu
 * CREATION DATE:  10/12/09
 * VERSION:        1.9.7
 * LICENSE:
 *     This source code is copyright (c) 2008-2009 rda.
 *     All rights reserved.
 *
 * NOTES TO USERS:
 *   None.
 *
 * MODIFICATION LOG
 *     
 *				 
 *************************************************************************/
#include "bt_rda.h"

#define patch_bunch0_addr 0x10005d98                                     
#define patch_bunch0_size (sizeof(patch_bunch0)/sizeof(patch_bunch0[0])) 
                                                                         
#define patch_bunch1_addr 0x10000050                                     
#define patch_bunch1_size (sizeof(patch_bunch1)/sizeof(patch_bunch1[0])) 
                                                                         
#define patch_bunch2_addr 0x10005ff0                                     
#define patch_bunch2_size (sizeof(patch_bunch2)/sizeof(patch_bunch2[0])) 
                                                                         
#define patch_bunch3_addr 0x10005bcc                                     
#define patch_bunch3_size (sizeof(patch_bunch3)/sizeof(patch_bunch3[0])) 
                                                                         
#define patch_bunch4_addr 0x1000000c                                     
#define patch_bunch4_size (sizeof(patch_bunch4)/sizeof(patch_bunch4[0])) 
                                                                         
#define patch_bunch5_addr 0x100060ac                                     
#define patch_bunch5_size (sizeof(patch_bunch5)/sizeof(patch_bunch5[0])) 
                                                                         
#define patch_bunch6_addr 0x1000610c                                     
#define patch_bunch6_size (sizeof(patch_bunch6)/sizeof(patch_bunch6[0])) 
   
#define patch_bunch7_addr 0x10005ea4                                     
#define patch_bunch7_size (sizeof(patch_bunch7)/sizeof(patch_bunch7[0])) 


                                 
#define pskey_share_clock_size (sizeof(pskey_share_clock)/sizeof(pskey_share_clock[0]))   
#define pskey_misc_size (sizeof(pskey_misc)/sizeof(pskey_misc[0]))  
#define patch_flag_size (sizeof(patch_flag)/sizeof(patch_flag[0]))  

void HwdMsDelay(uint32 delay)
{
	;
}

//0x10005d98~0x10005e24   35 //C0 Patch+ init patch + same address patch(part)
uint32 patch_bunch0[]=                    
{                                         
  0xe3a04022,//C0 Patch                             

  0xe247fc2a,
  0xe59f3040,
  0xe3a00000,
  0xe5c30000,
  0xe3a00000, 
  0xeb00000d,
  0xe3a01000,
  0xe3a00005,
  0xeb00000c,
  0xe59f3020,
  0xe5d30000,
  0xe3500002,
  0x1afffff6,
  0xe59f3024,
  0xe59f0024,
  0xe5830004,
  0xe59f0020,
  0xe5830024,
  0xe8bd8008,
  0x100092F6,
  0xe51ff004,
  0x0002ac68,
  0xe51ff004,
  0x000060cc,
  0x40180000,
  0x00011878,
  0x10005ff0,
  0xe3a0f2c1, 	
  0x13a00000,
  0x11a0f00e,
  0xe20e00ff,
  0xe35000a4,
  0x03a00000,
  0x13a00001,
  0xe1a0f00e       
};  


//0x1000050~0x1000046C  264   //big patch
uint32 patch_bunch1[]=
{
  
  0xe1a0f36f,
  0xe1a01006,
  0xe1a00004,
  0xea000013,
  0xe51ff004,
  0x00008d24,
  0xe51ff004,
  0x00007f2C,
  0xe51ff004,
  0x00013a88,
  0xe51ff004,
  0x0000e18c,
  0xe51ff004,
  0x000044b0,
  0xe51ff004,
  0x000047f8,
  0xe51ff004,
  0x0000494c,
  0xe51ff004,
  0x00001e90,
  0xe51ff004,
  0x000016f4,
  0xe51ff004,
  0x00000e84,
  0xe92d43f0,
  0xe1a07000,
  0xe59f0390, 
  0xe1a09001,
  0xe0801287,
  0xe3570002,
  0x11a00289,
  0x03a00000,
  0xe0814000,
  0xe5d4001c,
  0xe24dd024,
  0xe3a08000,
  0xe35000ff,
  0x05c4801c,
  0x01d401ba,
  0x02400001,
  0x01c401ba,
  0xe1d401ba,
  0xe3500000,
  0x0a0000c4, 
  0xe357000b,
  0x8a00009f, 
  0xe1a00009,
  0xebffffd3,
  0xe1a06000,
  0xe5d4001e,
  0xe3500000,
  0x1594100c,
  0x10800080,
  0x10810180,
  0x0594000c,
  0x05d4101f,
  0x00811081,
  0x00800181,
  0xe2405018,
  0xe59f0310, 
  0xe3a01003, 
  0xe3570002,
  0x1a000028, 
  0xe5952000, 
  0xe5d23000, 
  0xe15100a3, 
  0x05d21001, 
  0x03510017, 
  0x05901000,
  0x03811a40,
  0x0a00005b, 
  0xe3a00017,
  0xe15000a3, 
  0x1a000013,
  0xe5d20004, 
  0xe5d21005, 
  0xe0800401,
  0xe3500fc8,
  0x259f12c8, 
  0x23a0000d,
  0x218100b6,
  0x2a00008e, 
  0xe3a0201e,
  0xe3a01017,
  0xe1a00006,
  0xebffffaf, 
  0xe3a03001,
  0xe58d3000,
  0xe3a03004,
  0xe3a02fc8,
  0xe3a01fc8,
  0xe1a00006,
  0xebffffaa, 
  0xea000062, 
  0xe3a0000f,
  0xe15000a3, 
  0x05d20001, 
  0x03500000,
  0x1a00007d, 
  0xe3a0201a,
  0xe3a0100f,
  0xe1a00006,
  0xebffff9e, 
  0xea000058, 
  0xe3570000,
  0x1a000088, 
  0xe5952000, 
  0xe5d23000, 
  0xe5d2c001, 
  0xe083340c, 
  0xe3c36b40, 
  0xe246ce40,
  0xe25cc007,
  0x03a00080,
  0x05c20005, 
  0x0a00006c, 
  0xe246ce40,
  0xe25cc028,
  0xe3a0e060, 
  0x05c2e00f, 
  0x05950000,
  0xe3a03004, 
  0x05c08010,
  0x05950000,
  0x05c03012, 
  0x0a000062, 
  0xe246ce40,
  0xe25cc029,
  0x05c2e013, 
  0x05950000,
  0x05c08014,
  0x05950000,
  0x05c03016, 
  0x0a00005a, 
  0xe246ce80,
  0xe25cc003,
  0x1a000011,
  0xe3a00020, 
  0xe5c20005, 
  0xe5952000, 
  0xe5c21006, 
  0xe5952000, 
  0xe5c20007, 
  0xe5950000, 
  0xe5c01008, 
  0xe5950000,
  0xe5c03009, 
  0xe5950000,
  0xe5c0800a,
  0xe5951000,
  0xe3a00001,
  0xe5c1000b,
  0xe5950000,
  0xe5c0800c,
  0xea000045, 
  0xe246cec0, 
  0xe25cc01a, 
  0x1246cec0, 
  0x125cc019, 
  0x1a000004, 
  0xe5901000, 
  0xe3c11ec0, 
  0xe3c11018, 
  0xe5801000, 
  0xea00003b,
  0xe246ce40,
  0xe25cc002,
  0x1a000008,
  0xe59f1164,
  0xe3a00002,
  0xe5c10000,
  0xebffff5d, 
  0xe5cd8018,
  0xe1cd60bc,
  0xe28d000c,
  0xebffff5b, 
  0xea00000f,
  0xe246ce40,
  0xe25cc01f,
  0x1a00002c,
  0xe1a01006,
  0xe3a00000,
  0xebffff56, 
  0xe5cd8000,
  0xe5950000,
  0xe5d01003,
  0xe5d00004,
  0xe0810400,
  0xe1cd00b6,
  0xe1cd80b8,
  0xe3a0001c,
  0xe1a0100d,
  0xebffff4e, 
  0xe5d4001e,
  0xe3500000,
  0x05d4001f,
  0xe2400001,
  0xe5c4001e,
  0xe1d400b2,
  0xe1d510bc,
  0xe0400001,
  0xe1c400b2,
  0xe1d401ba,
  0xe2400001,
  0xe1c401ba,
  0xe5858000,
  0xe3570015,
  0x1a000010,
  0xe59f00c8,
  0xe5900000,
  0xe5d01002,
  0xe35100c0,
  0x15d00000,
  0x135000c0,
  0x0a000009,
  0xe59f10b0,
  0xe3a02001,  
  0xe5d10000,
  0xe2400001,
  0xe5c10000,
  0xe1a01009,
  0xe1a00007,
  0xebffff32, 
  0xe5940010,
  0xe1c080bc,
  0xe59f008c,
  0xe5d00051,
  0xe3500000,
  0x1a000007,
  0xe3a01000,
  0xe3a0000c,
  0xebffff2b, 
  0xe3500000,
  0x11d020bc,
  0x13a0000c,
  0x13a01000,
  0x1bffff28, 
  0xe28dd024,
  0xe8bd43f0,
  0xe59d0004,
  0xe3a0fd54, 
  0xe28dd024, 
  0xe8bd83f0, 
  0xe3570001,
  0x1affffd8,
  0xe5950000,
  0xe5d01000,
  0xe351000e,
  0x05d01003,
  0x03510003,
  0x05d00004,
  0x0350000c,
  0x059f1024,
  0x03a0000b,
  0x01c100b6,
  0xeaffffe0,
  0x10005074,
  0x100084f0,
  0x0000020e,
  0x1000968C,  
  0x10009300,
  0x100092f7,
  0x100084f4,
  0x10005514,   
  
}; 

//0x10005ff0~0x1000601C   12    //sleep patch
uint32 patch_bunch2[]=
{
  0xe247fc25,
  0xe0800084,
  0xe59710ac,
  0xe3c004f0,
  0xe1500001,
  0x81d70bb0,
  0x82800010,
  0x80866080,
  0x81500004,
  0x80444000,
  0xe3a01b46,
  0xe281f098,  
};


//0x10005bcc~0x10005C18   20   //bccmd
uint32 patch_bunch3[]=
{                
  0xe249fd48,
  0xe3a06450,
  0xe3a07000,
  0xe5867004,
  0xe3a07001,
  0xe59f8030,
  0xe5d89009,
  0xe3590005,
  0x0a000006,
  0xe3500080,//
  0x8a000004,//
  0xe3a09c4d,
  0xe1a0e00f,
  0xe289f0dc,
  0xe5867004,
  0xe3a0fc53,
  0xe5867004,
  0xe3a09c53,
  0xe289f044,
  0x100092ec,  
};          

         
//0x1000000c~0x10000014 3   //misc         
uint32 patch_bunch4[]=
{
  0xea00177E, 
  0xe1a00000,
  0xe5c17012,//bccmd resend
};  


//0x100060ac~0x100060d4 11  //deep sleep
uint32 patch_bunch5[]=   
{                        
  0xe240fdcc,     
  0xe24e0c7b,     
  0xe24000c4,     
  0xe3500000,     
  0x13a00001,     
  0x059f0008,   
  0x05d00000,   
  0x02200001,   
  0xe59ff000,   
  0x1000927a,   
  0x00022d6c,   
};       

//0x1000610C~0x1000618c   33 //wake ,resend error ,infinite times resend
uint32 patch_bunch6[]=
{   
  0xe246fc32,
  0xe3a01000,
  0xe3a00013,
  0xeb000016,
  0xe59f5050,
  0xe5950010,
  0xe3c00a80,
  0xe5850010,
  0xe5950004,
  0xe3c00a80,
  0xe5850004,
  0xe3a01000,
  0xe3a00014,
  0xeb00000e,
  0xe5950004,
  0xe3800a80,
  0xe5850004,
  0xe3a01000,
  0xe3a00003,
  0xeb000008,
  0xe5d4000d,
  0xe5d4100e,
  0xe1500001,
  0x1b000006,
  0xe3a02b91,
  0xe282f024,
  0x40200000,
  0xe51ff004,
  0x000228D0,
  0xe51ff004,
  0x000060CC,
  0xe51ff004,
  0x0002AD78,
};                   


//0x10005ea4~0x10005f14  29
uint32 patch_bunch7[]=   //12.22
{
  
  0xe241fe90,
  0xe595400c,
  0xe3500015,
  0x1a000011,
  0xe1a06000,
  0xe1a00006,
  0xe1d470bc,
  0xe3570000,
  0x1a00000c,
  0xe3a02000,
  0xe1a0100b,
  0xeb00000b,
  0xe1a00006,
  0xe3a02001,
  0xe1a0100b,
  0xeb000009,
  0xe595400c,
  0xe3540000,
  0x01a00004,
  0x03a04d4c,
  0x0284f054,
  0xeaffffee,
  0xe59f1010,
  0xe3a0fd46,
  0xe51ff004,
  0x00000e84,
  0xe51ff004,
  0x00001e90,
  0x100067a8,
};


uint32 patch_flag[][2]=
{
  {0x40180044,0x0000000A},
  {0x40180004,0x0002ac5c},//init patch   
  {0x40180024,0x10005d9c},               
  {0x40180008,0x000014fc},//bunch patch  
  {0x40180028,0x10000050}, 
  {0x4018000C,0x000249f8},//0xC0 issue patch
  {0x4018002C,0x10005d98},
  {0x40180010,0x0001ce44},//same address patch
  {0x40180030,0x10005e08},
  {0x40180014,0x000052fc},//bccmd patch patch
  {0x40180034,0x10005bcc},
  {0x40180018,0x00022d68},//deep sleep
  {0x40180038,0x100060ac},
  {0x4018001C,0x00024af8},//bccmd resend patch       
  {0x4018003C,0x10000014},
  {0x40180020,0x000243ec},//wake host patch + resend error enter patch+ infinite times' send patch                            
  {0x40180040,0x1000610c},  
  {0x40180000,0x000000ff}
};              
                

uint32 pskey_misc[][2]=
{

  {0x100067cc,0x788dffff},
  {0x1000A2B8,0x00008A00},  
//  {0x1000682C,0x00b400e0},//

  {0x40240000,0x0004F398},                               
  {0x10000080,0x00000007},                              
  {0x10000084,0x00470d37},                              
  {0x10000088,0x00482a57},                              
  {0x1000008c,0x00520400},                              
  {0x10000090,0x00530511},                              
  {0x10000094,0x00437f58},                              
  {0x10000098,0x00300029},                              
  {0x1000009c,0x0030002b},                              
  {0x10000038,0x0F055000},                              
  {0x1000003C,0xA0A0BF40},                              
  {0x10000034,0xa5025010},                              
  {0x10000030,0x1000401A},//
  {0x10000044,0x101E0007},                              
  {0X10000058,0x81010206},//Lu added 
  {0x10000000,0x1042f000},// 
  {0x100092f0,0x09008101},
//  {0x100091bc,0x101E0007},//PCM
  {0x40240000,0x0000F298},
  {0x40240018,0x201F1000},
  {0x4024002c,0x00501000},//
};

uint32 pskey_share_clock[][2]=
{
  {0x10000030,0x0000411A},////Lu added for share crystal
  {0x4024001C,0x00408155},
  {0x10000000,0x00001000}, 
};     


static void rda5868_bunch_write(uint32 start_address,uint32* patch,uint32 size)
{
   RDA5868_WriteData(start_address,&patch[0],size);   
   OS_TickDelay(10);
}

static void rda5868_misc_write(uint32 pskey[][2],uint32 size)                                   
{                                                                   
   uint32 i = 0;                                                      
   for(i=0;i<size; i++)    
   {                                                                 
      RDA5868_WriteData(pskey[i][0],&pskey[i][1],1);	
   }	   
   OS_TickDelay(10);                  
}

void RDA5868_Initialization()
{

rda5868_misc_write(pskey_misc,pskey_misc_size);

rda5868_misc_write(pskey_share_clock,pskey_share_clock_size);	            

rda5868_bunch_write(patch_bunch0_addr,&patch_bunch0[0],patch_bunch0_size);
rda5868_bunch_write(patch_bunch1_addr,&patch_bunch1[0],patch_bunch1_size);
rda5868_bunch_write(patch_bunch2_addr,&patch_bunch2[0],patch_bunch2_size);
rda5868_bunch_write(patch_bunch3_addr,&patch_bunch3[0],patch_bunch3_size);
rda5868_bunch_write(patch_bunch4_addr,&patch_bunch4[0],patch_bunch4_size); 
rda5868_bunch_write(patch_bunch5_addr,&patch_bunch5[0],patch_bunch5_size); 
rda5868_bunch_write(patch_bunch6_addr,&patch_bunch6[0],patch_bunch6_size);
rda5868_misc_write(patch_flag,patch_flag_size);

}  
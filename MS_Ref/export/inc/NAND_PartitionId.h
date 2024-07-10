/******************************************************************************
 ** File Name:      Nand_PartI.h                                              *
 ** Author:         rick zhang                                                *
 ** DATE:           06/09/2005                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file contains partition information about     *
 **                 NAND Flash which we used to store code & data according   *
 **                 to XSR BML spec.                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/09/2005     rick zhang       Create.                                      *
 ******************************************************************************/
#ifndef NAND_PARTI_H
#define NAND_PARTI_H


#define XSR_VOLUME_NUM		0

#define	SCI_PARTITION_SLAVE				0x0FFFFFFF			// reserved by SCI_FTL_MntEngin, don't modify it
#define	SCI_PARTITION_SYSUSER_DEF_BASE	0x10000000


#define SCI_PARTITION_NBL  				0
#define SCI_PARTITION_OS    				3 
#define SCI_PARTITION_DEMANDONOS			4
#define SCI_PARTITION_ID_FILESYSTEM		8

#define SCI_PARTITION_OSB    				SCI_PARTITION_OS 
#define SCI_PARTITION_OSA    				SCI_PARTITION_DEMANDONOS 
#define SCI_PARTITION_FILESYSTEM   		SCI_PARTITION_ID_FILESYSTEM                                
#define SCI_PARTITION_DOWNLOADED_NV    	SCI_PARTITION_SYSUSER_DEF_BASE + 0x1
#define SCI_PARTITION_CALIPARAM   		SCI_PARTITION_SYSUSER_DEF_BASE + 0x2

#define SCI_PARTITION_NV					SCI_PARTITION_SYSUSER_DEF_BASE + 0x3
#define SCI_PARTITION_MMI_RESOURCE		SCI_PARTITION_SYSUSER_DEF_BASE + 0x4
#define SCI_PARTITION_DSPCODE		    	SCI_PARTITION_SYSUSER_DEF_BASE + 0x5
#define SCI_PARTITION_GSMDSPCODE			SCI_PARTITION_SYSUSER_DEF_BASE + 0x6
#define SCI_PARTITION_HIDDEN_FILESYSTEM SCI_PARTITION_SYSUSER_DEF_BASE + 0x7

/* added for Device Manager func*/
#define SCI_PARTITION_UA					SCI_PARTITION_SYSUSER_DEF_BASE + 0x8  
#define SCI_PARTITION_UBB					SCI_PARTITION_SYSUSER_DEF_BASE + 0x9
#define SCI_PARTITION_DU					SCI_PARTITION_SYSUSER_DEF_BASE + 0xa
#define SCI_PARTITION_UAEB				SCI_PARTITION_SYSUSER_DEF_BASE + 0xb
#define SCI_PARTITION_UPDATED_NV			SCI_PARTITION_SYSUSER_DEF_BASE + 0xc

/*add for demand paging */
#define SCI_PARTITION_OSC					SCI_PARTITION_SYSUSER_DEF_BASE + 0xd
/*add for usb com driver auto install*/
#define SCI_PARTITION_ROMDISK_FILESYSTEM	SCI_PARTITION_SYSUSER_DEF_BASE + 0xe
#define SCI_PARTITION_PROTECT_NV	SCI_PARTITION_SYSUSER_DEF_BASE + 0xf

#endif  /* NAND_PARTI_H */


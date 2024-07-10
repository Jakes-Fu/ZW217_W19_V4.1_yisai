#ifndef XSR_PARTITION_H
#define XSR_PARTITION_H

/******************************************************************************
 * STL Partition Name
 ******************************************************************************/
#define STL_NV_FS_PART						"A"
//"B" "C" Reserved for other hidden partitions in the future 
#define STL_UDISK_HIDDEN_FS_PART            "C"
#define STL_UDISK_FS_PART					"D"
#define STL_SD_DEVICE_NAME                  "E"
#define STL_SD_1_DEVICE_NAME                "F"
#define STL_DU_PART                         "G"
#define STL_SDRAM_DEVICE_NAME               "H"
/******************************************************************************
 * BML Partition Name
 ******************************************************************************/
#define BML_OS_PART_NAME					"BML OS Partition"
#define BML_PRODUCT_PART_NAME				"BML Product Partition"
#define BML_DOWNLOADED_NV_PART_NAME			"BML Downloaded NV Partition"
#define BML_MMIRESOURCE_NAME                "BML MMI Resource Partition"
#define BML_DSPCODE_PART_NAME				"BML DSP Code Partition"
#define BML_GSMDSPCODE_PART_NAME            "BML GSM DSP Code Partition"
#define BML_DEMANDONOS_NAME					"BML DEMANDONOS Partition"
#define BML_KERNELOS2_NAME				    "BML KERNEL OS2 Partition"
#define BML_ROMDISK_FS_PART                 "BML ROMDISK Partition"
#ifdef POWER_OFF_PROTECT_SUPPORT
#define BML_PROTECT_NV_NAME                "BML PROTECT NV Partition"
#endif


/******************************************************************************
 * IOCTL Code
 ******************************************************************************/
#define IOCTL_STL_COMPACTION				((1 << 16) | 1)
#define IOCTL_BML_CHANGE_ATTRI				((2 << 16) | 1)

#endif /* XSR_PARTITION_H */

/******************************************************************************
 ** File Name:      comm_def.h                                                *
 ** Author:         Richard Yang                                              *
 ** DATE:           12/07/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/07/2002     Richard.Yang     Create.                                   *
 ******************************************************************************/

#ifndef _COMM_DEF_H
#define _COMM_DEF_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "comm_id.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
// 所有消息的头的结构
#define  STRUCT_HEAD \
unsigned short SignalCode;\
unsigned short Size;

// 键盘消息
typedef uint32  KEY_CODE_T;
typedef struct
{
	STRUCT_HEAD
	KEY_CODE_T  key_code;   // 键盘扫描码
} COMM_SOFTKEY_T, *COMM_SOFTKEY_PTR;


/* MAX cell number monitored by RRC module */
#define DIAG_CELL_MAX 7

// @Xueliang.Wang add(2003-01-02)
#define DIAG_ACCESS_MCU_MAX_LENGTH  1000

#ifndef _PS_BL7
typedef struct
{
    uint16           mcc;             
    uint16           mnc;             
    uint16           lac;              
    uint16           cell_id;          
    uint16           pm;               
    unsigned char    pwrc;             
    unsigned char    dtx;              
    uint8            rlt;              
    unsigned char    att;              
    uint8            bsag;             
    uint16           ccch;             
    uint16           cbq2;             
    uint8            bspamf;           
    uint8            maxre;            
    uint8            txint;             
    unsigned char    cba;               
    uint16           re;               
    uint16           ac;               
    uint8            crh;              
    uint8            tpmxcch;          
    uint8            ram;              
    unsigned char    osp_present;      
    uint8            cbp;              
    uint8            cro;              
    uint8            tmpo;             
    uint8            ptm;              
    unsigned char    cbch_present;     
    uint16           cbch_chan;        
    uint8            cbch_tn;               
    uint8            cbch_tsc;              
    unsigned char    cbch_is_hopping;       
    uint8            cbch_maio;             
    uint8            cbch_hsn;              
    uint16           cbch_arfcn;            

} DIAG_SERVING_CELL_INFO_T;

typedef struct
{
    unsigned char  cell_present;                  
    uint16         arfcn;                         
    uint8          bsic;                          
    uint16         lac;                           
    uint8          rxlev;                         
    int16          c1;                            
    int16          c2;                            
    unsigned char  gprs_supported;                
    int16          c31;                           
    int16          c32;                           

} DIAG_CELL_PRIMARY_INFO_T;

typedef DIAG_CELL_PRIMARY_INFO_T DIAG_CELL_PRIMARY_INFO_ARRAY[DIAG_CELL_MAX];

typedef struct
{
    STRUCT_HEAD
    DIAG_CELL_PRIMARY_INFO_ARRAY cell_list;    // The first record for serving cell
    DIAG_SERVING_CELL_INFO_T scell_info;      // The detailed information of serving cell
}DIAG_CELL_INFO_LOG_T;
#endif

typedef enum
{
	BYTE_ACCESS,
	WORD_ACCESS, 
	DWORD_ACCESS
} ADDR_ACESS_MODE_E;

typedef enum
{
    MCU_ACCESS_SUCCESS,
	MCU_ACCESS_MODE_ERR,
	MCU_ACCESS_ADDR_ERR,
	MCU_ACCESS_SIZE_ERR,
	MCU_ACCESS_RANGE_ERR
} DIAG_MCU_ACCESS_ERR_E;

typedef struct
{
    STRUCT_HEAD
	uint32  mode;
	void *  addr;   // In request packet, it is the access address
	                // In response packet, it is error code.
	uint32  access_size;    // Number of BYTE, WORD or DWORD according to mode.
}DIAG_MCU_READ_T, *DIAG_MCU_READ_PTR;

typedef struct
{
    STRUCT_HEAD
	uint32  mode;
	void *  addr;   // In request packet, it is the access address
	                // In response packet, it is error code.
	uint32  access_size;    // Number of BYTE, WORD or DWORD according to mode.
}DIAG_MCU_WRITE_T, *DIAG_MCU_WRITE_PTR;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*End comm_def.h*/
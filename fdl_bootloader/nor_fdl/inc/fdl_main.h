/******************************************************************************
 ** File Name:      fdl_main.h                                                *
 ** Author:         Leo.Feng                                                  *
 ** DATE:           30/11/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the interfaces of  the FIFO for serial  *
 **                 I/O operation. The FIFO is a clic queue.Writing operation *
 *                  can overlap the data has not be reading.                  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 30/11/2001     Leo.Feng         Create.                                   *
 **                                                                           *
 ******************************************************************************/


#ifndef _FDL_MAIN__H_
#define _FDL_MAIN__H_


/*!
        \file  sio_fifo.h
        \brief Contains the high level c api for the sio fifo.

*/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "cmd_def.h"
#include "packet.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Defination                                  *
 **---------------------------------------------------------------------------*/
typedef enum
{
    BOOT_MODE_UART = 0x0,
    BOOT_MODE_USB,
    BOOT_MODE_MAX
}
FDL_BOOT_MODE_E;

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef int (*pkt_proc_fun) (PACKET_T *, void *);

typedef struct pkt_proc_tab_tag
{
    cmd_pkt_type    cmd_type;
    pkt_proc_fun    pkt_proc;
} pkt_proc_tab, * pkt_proc_tab_ptr;

typedef struct pkt_rep_tab_tag
{
    char           *table;
    unsigned long  size;
} pkt_rep_tab, * pkt_rep_tab_ptr;

typedef enum
{
    SYS_STAGE_NONE,
    SYS_STAGE_CONNECTED,
    SYS_STAGE_START,
    SYS_STAGE_GATHER,
    SYS_STAGE_END,
    SYS_STAGE_ERROR
} down_flag_s;

typedef struct data_download_tag
{
    down_flag_s      stage;
    unsigned long    file_size;
    unsigned long    recv_size;
    unsigned long    start_addr;
    unsigned long    write_addr;
} data_download, *data_download_ptr;

typedef struct sys_stage_tag
{
    //BYTE
    ret_status       data_verify;
    data_download    data;
} sys_stage_s, *sys_stage_ptr;

typedef struct flash_partition_s
{
    uint32 NvAdress;
    uint32 PsAdress;
    uint32 ResAdress;
    uint32 EfsAdress;
    uint32 ProdinfoAdress;
    uint32 UmemAdress;
    uint32 Spload_Addr;       /*lcd  and sensor bin load address*/
#ifdef SIM_LOCK_SUPPORT
    uint32 SimlockAddress;     //simlock Start Addr
#endif
#ifdef NOR_BOOTLOADER_SUPPORT
    uint32 NorBootloaderAdress;
#endif
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    uint32 FotaAdress;
    uint32 FotaBootloaderAdress;
#endif
#ifdef DSP_DOWNLOAD_SUPPORT
	uint32 DspCodeAddress;
#endif
    uint32 OperatorDataAdress;

    uint32 CpCodeAddress;
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    uint32 apn_addr;
#endif
    uint32 config_nv_addr;
    uint32 ims_toggle_addr;

    uint32 EfsSize;
    uint32 PSSize;                /*ps partition size             */
    uint32 ResSize;               /*res partiton size            */
    uint32 NvSize ;               /*fixed nv partiton size     */
    uint32 FlashSize;
    uint32 UmemSize;
    uint32 SploadSize;
#ifdef SIM_LOCK_SUPPORT
    uint32 SimlockSize;     //simlock size
#endif
#ifdef NOR_BOOTLOADER_SUPPORT
    uint32 NorBootloaderSize;
#endif
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    uint32 FotaSize;
    uint32 FotaBootloaderSize;
#endif
#ifdef DSP_DOWNLOAD_SUPPORT
	uint32 DspCodeSize;
#endif
    uint32 OperatorDataSize;

    uint32 CpCodeSize;
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    uint32 apn_size;
#endif
    uint32 config_nv_size;
    uint32 ims_toggle_size;

} FLASH_PARTITION_T,*FLASH_PARTITION_PTR;


/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
/* This macro write to a coprocessor register */
#define         ESAL_TS_RTE_CP_WRITE(cp, op1, cp_value, crn, crm, op2)              \
                {                                                                   \
                    __asm                                                           \
                    {                                                               \
                        MCR     cp, op1, (cp_value), crn, crm, op2                  \
                    }                                                               \
                }

/**---------------------------------------------------------------------------*
**                         External Variables                                *
**---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         External Functions                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
int main (void);

/* Command process */
//void sys_reset(char *pkt_buf, int pkt_len);
//ret_status sys_cfg(char *pkt_buf, int pkt_len);
//ret_status sys_ver_req(char *pkt_buf, int pkt_len);

//ret_status sys_stop(char *pkt_buf, int pkt_len);
int FDL_SysConnect (PACKET_T *packet_ptr , void *arg);

int FDL_DataStart (PACKET_T *packet_ptr , void *arg);
int FDL_DataMidst (PACKET_T *packet_ptr , void *arg);
int FDL_DataEnd (PACKET_T *packet_ptr , void *arg);
//ret_status data_check(char *pkt_buf, int pkt_len);
int FDL_DataExec (PACKET_T *packet_ptr , void *arg);

int FDL_McuResetNormal (PACKET_T *packet_ptr, void *arg);
int FDL_McuReadFlash (PACKET_T *packet_ptr , void *arg);
int FDL_McuReadChipType (PACKET_T *packet_ptr , void *arg);

//@lin.liu. add lookup nvitem support.
int FDL_LookupNvitem (PACKET_T *packet_ptr, void *arg);

int FDL_SetBaudrate (PACKET_T *packet_ptr , void *arg);
int FDL_EraseFlash (PACKET_T *packet_ptr , void *arg);
int FDL_PowerOff (PACKET_T *packet_ptr ,void *arg);

//unsigned long system_count_get(void);
//void system_delay(unsigned long delay_ms);

//uint32 fdl_getbootmode(void);

BOOLEAN  get_flash_partition_info (FLASH_PARTITION_PTR ptr);
BOOLEAN  lookup_valid_nv_address(uint32 *valid_nv_addr_out );


uint32 Get_productinfo_offset(void);

uint32 Get_fdl_file_end_addr(void);

BOOLEAN Get_phasecheck_flag(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_SIO_FIFO__H_

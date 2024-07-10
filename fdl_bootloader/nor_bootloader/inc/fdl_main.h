

#ifndef _FDL_MAIN__H_
#define _FDL_MAIN__H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "bdl_cmd_def.h"
#include "bdl_packet.h"
#include "integer.h"

/* C header file */

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Defination                                  *
 **---------------------------------------------------------------------------*/
typedef enum
{
	BOOT_MODE_UART = 0x0,
	BOOT_MODE_USB,
	BOOT_MODE_MAX
}FDL_BOOT_MODE_E;

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef DLSTATUS (*pkt_proc_fun) (PACKET_T *);
    
typedef struct pkt_proc_tab_tag
{
    cmd_pkt_type    cmd_type;
    pkt_proc_fun    pkt_proc;
}pkt_proc_tab, * pkt_proc_tab_ptr;

typedef struct pkt_rep_tab_tag
{
    char           *table;
    unsigned long  size;
}pkt_rep_tab, * pkt_rep_tab_ptr;

typedef struct flash_partition_s
{
    uint32 NvAdress;
    uint32 	PsAdress;
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
#ifdef DSP_DOWNLOAD_SUPPORT
	uint32 DspCodeAddress;
#endif
    uint32 EfsSize;
    uint32 PSSize;                /*ps partition size             */
    uint32 ResSize;	              /*res partiton size            */
    uint32 NvSize ;               /*fixed nv partiton size     */    
    uint32 FlashSize;
    uint32 UmemSize;
    uint32 SploadSize;
#ifdef SIM_LOCK_SUPPORT    
    uint32 SimlockSize;     //simlock Size
#endif     
#ifdef NOR_BOOTLOADER_SUPPORT   	
    uint32 NorBootloaderSize;           
#endif	
#ifdef DSP_DOWNLOAD_SUPPORT
	uint32 DspCodeSize;
#endif
} FLASH_PARTITION_T,*FLASH_PARTITION_PTR;

typedef enum 
{
    DL_STAGE_SYNC,
    DL_STAGE_DL_BEGIN,    
    DL_STAGE_DATA_TRANSFER,
    DL_STAGE_DATA_END,
    DL_STAGE_RUN_GSMSW
} DL_STAGE;

typedef struct _DL_STATUS {
	DL_STAGE     stage;
	uint32       dl_packet_id;
	uint32		 data_verify;	/* Record the result of the latest file download 
	 						 * operation. */
} DL_STATUS, *PDL_STATUS;

 /*.pac include file  structure*/
 typedef struct _FILE_T
 {
     DWORD  dwSize;              // size of this struct itself
     WCHAR  szFileID[256];        // file ID,such as FDL,Fdl2,NV and etc.
     WCHAR  szFileName[256];     // file name,in the packet bin file,it only stores file name
                                 // but after unpacketing, it stores the full path of bin file
     WCHAR  szFileVersion[256];  // Reserved now
     int    nFileSize;           // file size
     int    nFileFlag;           // if "0", means that it need not a file, and
                                             // it is only an operation or a list of operations, such as file ID is "FLASH"
                                             // if "1", means that it need a file
     DWORD  nCheckFlag;          // if "1", this file must be downloaded;
                                             // if "0", this file can not be downloaded;
     DWORD  dwDataOffset;        // the offset from the packet file header to this file data
     DWORD  dwCanOmitFlag;       // if "1", this file can not be downloaded and not check it as "All files"
                                 //   in download and spupgrade tool.
     DWORD  dwReserved[255];     // Reserved for future,not used now
 }FILE_T, *PFILE_T;

 /*.pac file  structure*/
 typedef struct _BIN_PACKET_HEADER_T
 {
     WCHAR  szVersion[24];        // packet struct version
     DWORD  dwSize;              // the whole packet size;
     WCHAR  szPrdName[256];       // product name
     WCHAR  szPrdVersion[256];   // product version
     int    nFileCount;          // the number of files that will be downloaded, the file may be an operation
     DWORD  dwFileOffset;        // the offset from the packet file header to the array of FILE_T struct buffer
     DWORD  dwMode;
     DWORD  dwFlashType;
     DWORD  dwNandStrategy;
     DWORD  dwIsNvBackup;
     DWORD  dwNandPageType;
     WCHAR  szPrdAlias[100];    // product alias
     DWORD  dwOmaDmProductFlag;
     DWORD  dwIsOmaDM;
     DWORD  dwIsPreload;
     DWORD  dwReserved[202];
 }BIN_PACKET_HEADER_T,*PBIN_PACKET_HEADER_T;

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
int main(void);
ret_status process_packets(void);
void  send_ack( cmd_pkt_type pkt_type );

/* Command process */
//void sys_reset(char *pkt_buf, int pkt_len);
//ret_status sys_cfg(char *pkt_buf, int pkt_len);
//ret_status sys_ver_req(char *pkt_buf, int pkt_len);

//ret_status sys_stop(char *pkt_buf, int pkt_len);
DLSTATUS sys_connect( PACKET_T * packet_ptr );

DLSTATUS data_start( PACKET_T * packet_ptr );
DLSTATUS data_midst( PACKET_T * packet_ptr );
DLSTATUS data_end( PACKET_T * packet_ptr );
//ret_status data_check(char *pkt_buf, int pkt_len);
DLSTATUS data_exec( PACKET_T * packet_ptr );

void mcu_reset_normal( PACKET_T * packet_ptr );
void mcu_read_flash( PACKET_T * packet_ptr );
void mcu_read_chip_type( PACKET_T * packet_ptr );

//@lin.liu. add lookup nvitem support.
void lookup_nvitem( PACKET_T * packet_ptr );

BOOLEAN  get_flash_partition_info(FLASH_PARTITION_PTR ptr);
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

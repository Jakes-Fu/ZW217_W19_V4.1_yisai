/******************************************************************************
 ** File Name:      flash_amd.c                                               *
 ** Author:         lin.liu                                                   *
 ** DATE:           10/01/2003                                                *
 ** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 Manager AMD flash.                                        *
 **                 This driver support Am29DL322GT                           *
 **                 If change flash, need modify sector number etc.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01-10-2003     lin.liu          Create.                                   *
 ** 04-19-2003     Lin.liu          Move the config to the memory config file *
 ******************************************************************************/


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
/*
#include "os_api.h"
#include "sci_types.h"
#include "com_drvapi.h"
#include "sci_types.h"
#include "com_drvapi.h"
#include "chip_drvapi.h"
#include "emc_drvapi.h"
#include "normcp_spec.h"
#include "flash_drvapi.h"

#include "sc6600b_reg.h"

#ifndef USE_NUCLEUS
#include "mem_cfg.h"
#endif

#ifndef WIN32
#include "tx_port_thumb.h"
#endif
*/
#include "os_api.h"
#include "sci_types.h"
#include "com_drvapi.h"
#include "chip_drvapi.h"
#include "emc_drvapi.h"
#include "normcp_spec.h"
#include "flash_drvapi.h"
#include "flash_cfg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#define FLASH_INIT_MAGIC           0xFAACCAAF
static  uint32 s_flash_init_flag = 0x0;

//
// invert the high/low byte of the given word.
#define INVERT_WORD( _w )           _w = ((_w) >> 8) + ((_w) << 8)

#ifndef WIN32  // ARM target, Big-Endian
#define FIRST_BYTE( _word )         (uint8)( (_word) >> 8 )
#define SECOND_BYTE( _word )        (uint8)( (_word) & 0xFF )
#endif

// ARM target, big-endian
#define MAKE_WORD( _fst, _snd )     ((uint16)(_fst) << 8) + (uint16)(_snd)


/*
    Flash type define .
*/
typedef VOLATILE uint16   *FLASH_PTR_TYPE;

/*
    Flash Device Information
*/

#define FLASH_MAX_PROGRAM_TIME   100    // 100 ms
#define FLASH_MAX_ERASE_TIME     5500   // 5000 ms




// @lin.liu(2003-12-04). CR:MS5446
// Only be used when use SRAM for simulate, so don't define it in WIN32 Env.
//#define FLASH_EXT_SECTOR     3    // we use these sector to store fixed nvitem
// @CR : 6446

#ifdef WIN32 // Total 2Mbyte

#include "windows.h"

static char     flash_sim[]="c:\\flash_sim.dat";  // flash emu file
static HANDLE   hFile = NULL;  // the handle for open flash_sim
static HANDLE   hMap  = NULL;  // the handle for viewmap

static uint8   *emu_buffer = NULL;  // simulator point
#else
// Use RAM for simulate
#define FLASH_SECTOR_NUM    4
#define FLASH_SECTOR_SIZE   4096  // The sector size must > classmark | system param    

#define FLASH_EXT_SECTOR     3    // we use these sector to store fixed nvitem

#define FLASH_FIX_ITEM_SIZE  (FLASH_EXT_SECTOR * FLASH_SECTOR_SIZE)

#define FLASH_EMU_ADDRESS    FLASH_EmuOffsetAddr
static uint8 *emu_buffer;
#endif

static BOOLEAN      flash_is_simulate;

static uint32       flash_sector_num = 0;
static uint32       flash_sector_size = 0x10000;
static uint32       flash_start_addr = 0x0;
static uint32       flash_bank_addr;
static uint32       flash_end_addr;
static uint32       flash_fixed_nvitem_addr;
static uint32       flash_size;
static uint32       flash_product_info_addr;

void FLASH_GetSemaphore (void);
void FLASH_ReleaseSemaphore (void);

static const uint32 probe_addr = 0x0;
#define FLASH_PROBE_ADDR   (&probe_addr)
#define FLASH_PROBE_MAGIC  0x46504D41    // FPMA

//
// Set the flash param automatic,
// please set emu_buffer before call this function.
//
static uint flash_probe (void) // return 0 : use flash, other use SRAM or file for simulate.
{
#ifndef WIN32  // need probe we use flash / 
    volatile uint32  probe, tmp;

    // read the old value;
    probe = * (volatile uint32 *) (FLASH_PROBE_ADDR);
    // try modify it
    * (volatile uint32 *) (FLASH_PROBE_ADDR) = FLASH_PROBE_MAGIC;
    // read back
    tmp = * (volatile uint32 *) (FLASH_PROBE_ADDR);

    // compare it
    if (FLASH_PROBE_MAGIC != tmp)
    {
        // the content can't be modified,
        flash_is_simulate = FALSE;
    }
    else
    {
        // Yes, use ram for simulate
        // restore the old content
        * (volatile uint32 *) (FLASH_PROBE_ADDR) = probe;

        flash_is_simulate = TRUE;
    }

    //flash_is_simulate = TRUE;  // If you enable it, force to use SRAM.

#else // WIN32 Env.
    //@lin.liu(2003-12-04. CR : MS5446.
    // in WIN32 Env, it is same with target, so defined as "FALSE"
    flash_is_simulate = FALSE;
    // @End CR : 5446
#endif

    // compare it
    if (flash_is_simulate)
    {
        //@lin.liu(2003-12-04. CR : MS5446.
#ifndef WIN32
        // set flash param.
        flash_sector_num = FLASH_SECTOR_NUM;
        flash_sector_size = FLASH_SECTOR_SIZE;

        //@lin.liu(2003-12-10).  CR: MS6187
        // move the emu buffer space to high address to reduce the RW size.
        {
            uint32   sram_base = (uint32) (&emu_buffer) & 0xFF000000;

#ifndef USE_NUCLEUS
            emu_buffer  = (uint8 *) (sram_base + FLASH_EMU_ADDRESS);
#else
            emu_buffer  = (uint8 *) (sram_base + 0x81000);
#endif
        }
        // End. CR : 6187

        flash_start_addr = (uint32) emu_buffer;
        flash_bank_addr  = flash_start_addr;
        flash_size       = flash_sector_num * flash_sector_size;
        flash_end_addr   = flash_start_addr + flash_size;

        flash_fixed_nvitem_addr = flash_end_addr;
        flash_product_info_addr = flash_end_addr - 256;
#else // WIN32
        SCI_ASSERT (0);/*assert to do*/
#endif
        // @ End CR: 5446
    }
    else
    {
#ifdef USE_NUCLEUS  //this allocation is for HTW
        flash_sector_num = 2;   // the sectors that EFS managed
        //Daniel.ding modify it for HTW .2003.10.14
        flash_bank_addr         =  0x7D0000;   //
        flash_fixed_nvitem_addr = 0x3F0000;

        flash_sector_size       = 0x10000;
        flash_start_addr        = 0x0;

        flash_product_info_addr = 0;
        flash_size              = 0x800000;
        flash_end_addr          = flash_start_addr + flash_size;
#else
        //Raistlin.Kong 2004-1-13
        //the code will allocated in bank1,bank2,bank3(1 + 3 + 3 Mbytes) and EFS will manage bank4(1Mbytes)
        flash_sector_num        = FLASH_SectorNum;   // the sectors that EFS managed

        flash_bank_addr         = FLASH_BankAddr;   //
        flash_sector_size       = FLASH_SectorSize;
        flash_start_addr        = FLASH_StartAddrr;

        flash_fixed_nvitem_addr = FLASH_FixedNvitemAddr;
        flash_product_info_addr = FLASH_ProductInfoAddr;

        flash_size              = FLASH_Size;   // 8MBits
        flash_end_addr          = flash_start_addr + flash_size;
#endif
    }

    return flash_is_simulate;
}


//#define INTERRUPT_SAVE_AREA     SCI_INTERRUPT_SAVE_AREA
static BOOLEAN b_in_disable_irq_state = FALSE;

#define FLASH_DISABLE_IRQ         if( !b_in_disable_irq_state ) { SCI_DisableIRQ(); }
#define FLASH_RESTORE_IRQ         if( !b_in_disable_irq_state ) { SCI_RestoreIRQ(); }


/******************** AMD Flash Optr **********************************/
// following two cycle
static const FLASH_PTR_TYPE     flash_start_ptr = (FLASH_PTR_TYPE) (0x0);

#define UNLOCK_CYCLE            *(flash_start_ptr + 0x555) = 0xAA; \
    *(flash_start_ptr + 0x2AA) = 0x55

#define PROGRAM_SETUP_CYCLE     *(flash_start_ptr + 0x555) = 0xA0

#define ERASE_SETUP_CYCLE       *(flash_start_ptr + 0x555) = 0x80; \
    UNLOCK_CYCLE

#define ERASE_CHIP_CYCLE        *(flash_start_ptr + 0x555) = 0x10

#define RESET_CYCLE             *(flash_start_ptr) = 0xF0
/**********************************************************************/


#define SR_0      0x01
#define SR_1      0x02
#define SR_2      0x04
#define SR_3      0x08
#define SR_4      0x10
#define SR_5      0x20
#define SR_6      0x40
#define SR_7      0x80

// @lin.liu(2003-12-04). CR: MS5446.
// Not use this struct now.
/*
*/
typedef union BYTE_ORDER_TAG
{
    struct
    {
        uint8       b1;
        uint8       b2;
        uint8       b3;
        uint8       b4;
    } byte_struct;

    struct
    {
        uint16      low_word;
        uint16      high_word;
    } word_struct;

    uint32          dw;

} BYTE_ORDER_U;
// @ End CR : 5446


/**---------------------------------------------------------------------------*
 **                        flash information define and macro                 *
 **---------------------------------------------------------------------------*/
/*
   FLASH memory . ( all sector have the same size. )

   |--------------------|  ---> low addr( start addr )
   | Code or const data |
   |--------------------|
   | .................. |
   |                    |
   |--------------------|  ---> addr ( bank addr )
   | sector  0          |
   |--------------------|
   |  ...............   |
   |--------------------|
   | max sector no. - 1 |
   ----------------------  ---> high addr ( end addr )
*/

/**---------------------------------------------------------------------------*
 **              variable declare                                             *
 **---------------------------------------------------------------------------*/
/*
    erase status. see efs_erase_status_enum(efs_base.h) to get more info
*/
LOCAL volatile FLASH_ERASE_STATUS_E        s_erase_status;

/*
    erase addr, the base addr of the block being erased.
    we store it, so needn't the address when resume/suspend .
*/
LOCAL volatile uint32                      s_erase_addr;


/**---------------------------------------------------------------------------*
 **              optr macro  ( like function )                                *
 **---------------------------------------------------------------------------*/

/*
    check the addr is in range of flash device.
    We define like this to avoid compile warning.
*/
#define FLASH_ADDR_IS_VALID( _addr )  \
    ( ((_addr) >= flash_start_addr) && ((_addr) <= flash_end_addr) )



//
// add error trace information
//
#define FLASH_OPTR_ERASE   0xB7
#define FLASH_OPTR_PROGRAM 0xCF
#define FLASH_OPTR_RESUME  0xDE
#define FLASH_OPTR_SUSPEND 0xE3
//

static FLASH_TRACE_INFOR_T  flash_trace_infor;

#define FLASH_SAVE_TRACE_INFOR_INIT     flash_trace_infor.flag = 0x46544946  // FTIF   

#define FLASH_SAVE_TRACE_INFOR_ERASE    flash_trace_infor.optr = FLASH_OPTR_ERASE
#define FLASH_SAVE_TRACE_INFOR_PROGRAM( _data ) \
    flash_trace_infor.optr = FLASH_OPTR_PROGRAM; \
    flash_trace_infor.data = _data

#define FLASH_SAVE_TRACE_INFOR_RESUME   flash_trace_infor.optr = FLASH_OPTR_RESUME
#define FLASH_SAVE_TRACE_INFOR_SUSPEND  flash_trace_infor.optr = FLASH_OPTR_SUSPEND


#define FLASH_SAVE_TRACE_INFOR_CHECK_STATUS

#if 0
\
flash_trace_infor.address  = (volatile uint32) (addr_ptr);
\
flash_trace_infor.tick_1   = old_tick;
\
flash_trace_infor.tick_2   = cur_tick;
\
flash_trace_infor.max_tick = tick_count;
\
flash_trace_infor.status_1 = d;
\
flash_trace_infor.status_2 = tmp;
\
flash_trace_infor.content  = *addr_ptr
#endif

                             /*****************************************************************************/
                             //  Description :
                             //      Init the flash device.
                             //
                             //  Global resource dependence :
                             //      None
                             //  Author:
                             //      Lin.liu
                             //  Note:
                             //      This function should be called after system start.
                             /*****************************************************************************/

                             // @lin.liu(2003-12-04). CR: MS5446.
                             // Use new method for simulate on WIN32 Env.
#ifdef WIN32
                             static BOOL init_win32_file (const char *flash_file, DWORD flashSize,
                                     BOOL bCopyNvitem, const char *nv_file)
{
    DWORD   err,  dwFileSize;
    BOOL    newFile;

    hFile = CreateFile (flash_file,
                        GENERIC_READ | GENERIC_WRITE,   /*  Access : read / write    */
                        FILE_SHARE_READ,                /*  Share Mode : share-read  */
                        NULL,                           /*  Security : NONE          */
                        OPEN_ALWAYS,                    /*  Create the file always   */
                        0,
                        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        /*
            can't create/open the file.
        */
        hFile = NULL;
        return FALSE;
    }

    err = GetLastError();
    newFile = (ERROR_ALREADY_EXISTS != err) ? TRUE : FALSE;

    dwFileSize = GetFileSize (hFile, NULL);

    if (dwFileSize != flashSize)
    {
        // The length of the file has been changed, so force init the file
        newFile = TRUE;
    }

    /*
        try to mapping the file
    */
    hMap = CreateFileMapping (hFile,
                              NULL,
                              PAGE_READWRITE,     /*  Access : read / write  */
                              0,
                              flashSize,  //
                              NULL);  /* no name  */

    if (NULL == hMap)
    {
        err = GetLastError();
        CloseHandle (hFile);
        hFile = NULL;
        return FALSE;
    }

    emu_buffer = (uint8 *) MapViewOfFile (hMap,
                                          FILE_MAP_WRITE,
                                          0,
                                          0,
                                          0); /* map whole file  */

    if (NULL == emu_buffer)
    {
        err = GetLastError();
        CloseHandle (hMap);
        hMap = NULL;
        CloseHandle (hFile);
        hFile = NULL;
        return FALSE;
    }

    if (newFile)
    {
        if (!SetFilePointer (hFile,
                             flash_size,
                             NULL,
                             FILE_BEGIN))
        {
            err = GetLastError();
            CloseHandle (hFile);
            hFile = NULL;
            return FALSE;
        }

        memset (emu_buffer, 0xFF, flash_size);
    }

    if (bCopyNvitem)
    {
        HANDLE  NVITEM_HANDLE;
        DWORD   read_count;

        // copy nvitem
        NVITEM_HANDLE = CreateFile (nv_file,
                                    GENERIC_READ,                   /*  Access : read / write    */
                                    FILE_SHARE_READ,                /*  Share Mode : share-read  */
                                    NULL,                           /*  Security : NONE          */
                                    OPEN_EXISTING,
                                    0,
                                    NULL);

        if (NVITEM_HANDLE == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }

        memset (&emu_buffer[flash_fixed_nvitem_addr], 0xFF, 0x10000);

        if (!ReadFile (NVITEM_HANDLE, emu_buffer + flash_fixed_nvitem_addr,
                       0x10000, &read_count, NULL))
        {
            CloseHandle (NVITEM_HANDLE);
            return FALSE;
        }

        CloseHandle (NVITEM_HANDLE);
    }

    return TRUE;
}

//@lin.liu(2003-12-08). CR6121.

//
// Check the given fils is exist ?
//
static BOOL is_file_exist (const char *name)
{
    HANDLE hFind;
    WIN32_FIND_DATA findData;

    hFind = FindFirstFile (name, &findData);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        // OK, has found, use the default path.
        FindClose (hFind);
        return TRUE;
    }

    return FALSE;
}

//
// Try get the default path( ...\ms_code\common\NV_Parameters\nvitem.bin )
//
static BOOL get_default_path (char *flash_sim, char *nv_item)
{
    char   path[ MAX_PATH + 2];
    char *file_root;
    int    len;

    //  try parse flash file
    len =  sprintf (path, "%s", __FILE__);
    _strlwr (path);

    file_root =  strstr (path, "ms_code");

    if (NULL == file_root)
    {
        return FALSE;
    }

    file_root   += strlen ("ms_code");
    *file_root   = '\0';
    sprintf (flash_sim, "%s\\flash_sim.dat", path);
    return TRUE;
    //sprintf( nv_item, "%s\\common\\NV_Parameters\\nvitem.bin", path );

    // check the nvitem.bin has exist ?
    //return is_file_exist( nv_item );
}

#include "windows.h"
#include "shlobj.h"

//
// Get the current module path.
//
static char *get_module_directory (char *directory)
{
    char *ptemp, *pend;

    *directory = '\0';
    GetModuleFileName (NULL, directory,  MAX_PATH);

    // Remove the module name
    ptemp = strchr (directory, '\\');

    while (ptemp)
    {
        ptemp += 1;
        pend   = ptemp;
        ptemp  = strchr (ptemp, '\\');
    }

    *pend = '\0';
    return directory;
}

//
// Read the base directory from the Config File.
//
static int get_base_directory_from_ini_file (// If Success, return 1 and the base_directory is valid
    char *ini_file,
    char *base_directory)
{
    FILE   *pf = fopen (ini_file, "r");
    char   *ptr;

    if (NULL == pf)
    {
        return 0;
    }

    ptr = fgets (base_directory, MAX_PATH, pf);
    fclose (pf);

    if (NULL != ptr)
    {
        ptr += strlen (base_directory);

        if ('\\' != * (ptr - 1))
        {
            *ptr++ = '\\';
            *ptr   = '\0';
        }
    }

    return (NULL != ptr) ? 1 : 0;
}

//
// Write the base directory to the Config File.
//
static int write_base_directory_to_ini_file (
    char *ini_file,
    char *base_directory)
{
    FILE *pf = fopen (ini_file, "w");

    if (NULL == pf)
    {
        return 0;
    }

    fputs (base_directory, pf);

    fclose (pf);
    return 1;
}


//
// If can find the file on the current path, use it.
// Otherwise let user to select one
//
static BOOL select_path (char *flash_sim, char *nv_item)
{
    static const char flash_ini_file[]  = "FlashCfg.ini";
    static const char flash_sim_file[]  = "flash_sim.dat";
    static const char nv_item_file[]    = "NvItem.bin";
    static const char btitle[]          = "请选择 NvItem 参数的数据文件路径(NvItem.Bin)";

    char            ModuleDirectory[ MAX_PATH ];

    char            base_directory[ MAX_PATH ];
    char            ini_file[MAX_PATH];

    BROWSEINFO      binfo;
    LPITEMIDLIST    lpid;


    // First, Get Current module directory
    get_module_directory (ModuleDirectory);

    // Try use the file on the current module path
    sprintf (flash_sim, "%s%s", ModuleDirectory, flash_sim_file);
    sprintf (nv_item,   "%s%s", ModuleDirectory, nv_item_file);

    // If it is exist, use these file.
    if (is_file_exist (nv_item))
    {
        return TRUE;
    }

    // Get path from INI file or let user to select one.
    sprintf (ini_file, "%s%s", ModuleDirectory, flash_ini_file);

    while (1)  // Until user click the "CANCEL" button.
    {
        // Try get path from INI file
        if (get_base_directory_from_ini_file (ini_file, base_directory))
        {
            sprintf (flash_sim, "%s%s", base_directory, flash_sim_file);
            sprintf (nv_item,   "%s%s", base_directory, nv_item_file);

            if (is_file_exist (nv_item))
            {
                return TRUE;
            }
        }

        // Select one
        memset (&binfo, 0x0, sizeof (BROWSEINFO));
        binfo.pszDisplayName = base_directory;
        binfo.lpszTitle      = btitle;

        if (NULL != (lpid = SHBrowseForFolder (&binfo)))
        {
            if (SHGetPathFromIDList (lpid, base_directory))
            {
                // Write the file to
                write_base_directory_to_ini_file (ini_file, base_directory);
            }
        }
        else
        {
            // Use select "Cancel"
            return FALSE;
        }
    }
}

static BOOL get_flash_sim_file (char *flash_sim)
{
    static const char flash_sim_file[]  = "flash_sim.dat";
    char          ModuleDirectory[ MAX_PATH ];

    get_module_directory (ModuleDirectory);

    // Try use the file on the current module path
    sprintf (flash_sim, "%s%s", ModuleDirectory, flash_sim_file);

    return TRUE;
}

static BOOL try_get_flash_and_nvitem_path (char *flash_sim, char *nv_item)
{
    // For the nvitem.bin can't be used in WIn32 directly.
    return get_flash_sim_file (flash_sim);

    // First, Try get the default path.
    if (get_default_path (flash_sim, nv_item))
    {
        return TRUE;
    }

    // If it's not exist, please select one.
    return select_path (flash_sim, nv_item);
}
//@End. CR6121.

static BOOLEAN init_win32 (void)
{
    // we must call flash_probe to set flash param
    flash_probe();

    //#define _EFS_TEST
#ifdef _EFS_TEST
    {
        const char flash_test[]="c:\\efs_test\\nvitem_1104_1.dat";

        if (!init_win32_file (flash_test, 0x400000, FALSE, NULL))
        {
            return FALSE;
        }
    }
#else // Not define _EFS_TEST
    {
        char  flash_sim[256], nv_item[256];

        //@lin.liu(2003-12-08). CR : MS6121
        //      if( !get_flash_and_nvitem_path( flash_sim, nv_item ) )
        if (!try_get_flash_and_nvitem_path (flash_sim, nv_item))
            //End. @lin.liu(6121).
        {
            return FALSE;
        }

        if (!init_win32_file (flash_sim, flash_size, FALSE, nv_item))
        {
            return FALSE;
        }
    }
#endif // _EFS_TEST
    return TRUE;
}

#else // Not define WIN32

static void init_target (void)
{
    /*
        probe : we use flash ?
    */
    if (flash_probe())
    {
        /* Init the flash space.  */
        memset (emu_buffer, 0xFF, flash_size);
        memset (&emu_buffer[flash_fixed_nvitem_addr],
                0,
                FLASH_FIX_ITEM_SIZE);
    }
    else
    {
#if 0   //@lin.liu, needn't erase these banks now.     
        // For test, we erase the content of flash.
        // so we can debug it easily.
        // but need 2 seconds.
        uint32   sector, addr;

        addr = flash_bank_addr;

        for (sector = 0; sector < flash_sector_num; sector++)
        {
            FLASH_Erase (addr);
            addr += flash_sector_size;
        }

        // this sector store the product control information
        // If you want clean it, please enable it.
        //FLASH_Erase( 0x3FE000 );
#endif // #if 0        
    }
}

#endif // WIN32

//
// Init the device layer
//
BOOLEAN FLASH_Init (void) /* If Init success, return true, else return false */
{
#ifdef WIN32
    init_win32();
#else // Not define WIN32
    init_target();

    FLASH_SAVE_TRACE_INFOR_INIT;

#endif /* WIN32 */

    b_in_disable_irq_state = FALSE;

    // At least two block, sector size >= 4096
    SCI_ASSERT (flash_sector_num >= 2);/*assert verified*/
    SCI_ASSERT (flash_sector_size >= 4096);/*assert verified*/

    s_erase_status = FLASH_ERASE_COMPLETED_S;

    s_flash_init_flag = FLASH_INIT_MAGIC;

    return TRUE;
}

// @ End CR: 5446.


/*****************************************************************************/
//  Description :
//      Close the flash device.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      This function should be called when power down.
/*****************************************************************************/
void FLASH_Close (void)
{
#ifdef WIN32

    /*
        Close the opened resource.
    */
    if (NULL != emu_buffer)
    {
        UnmapViewOfFile (emu_buffer);
        emu_buffer = NULL;
    }

    if (NULL != hMap)
    {
        CloseHandle (hMap);
        hMap = NULL;
    }

    if (NULL != hFile)
    {
        CloseHandle (hFile);
        hFile = NULL;
    }

#endif // WIN32

    s_flash_init_flag = 0;
}

/*****************************************************************************/
//  Description :
//      Get sector size and sector number of the device.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_GetDeviceInfo (
    uint32   *sector_size_ptr, // output the sector size
    uint16   *sector_num_ptr,  // output the sector number
    uint16   *file_system_sector_num_ptr)  // output the file system sector num.
{
    *sector_size_ptr            = (uint32) flash_sector_size;
    *sector_num_ptr             = (uint16) flash_sector_num;
    *file_system_sector_num_ptr = (uint16) FLASH_FileSystemNum;
}

/*****************************************************************************/
//  Description :
//      Convert address to sector number.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      sector no. 0 is the first sector we managered
/*****************************************************************************/
uint16  FLASH_AddrToSector ( // return the no. of the sector, If addr isn't in flash, return 0xFFFF
    uint32 addr)   // physical address  that will be converted
{
    if (FLASH_ADDR_IS_VALID (addr))
    {
        SCI_ASSERT (addr >= flash_bank_addr);/*assert verified*/
        addr       -= (uint32) flash_bank_addr; // get offset

        return (uint16) (addr / flash_sector_size);
    }
    else
    {
        // return the invalid sector no.
        return FLASH_INVALID_SECTOR;
    }
}

/*****************************************************************************/
//  Description :
//      Convert sector no. to physical address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
/*****************************************************************************/
uint32 FLASH_SectorToAddr (  /* start physical address of the sector */
    uint16 sector_no)    /* sector's no.                         */
{
    return (sector_no < flash_sector_num)
           ?   flash_bank_addr + (uint32) sector_no * flash_sector_size
           :   FLASH_INVALID_ADDR;
}


#define SYSTEM_TICK_COUNT_LOW   *(volatile uint32 *)(SYS_CNT0)
#define SYSTEM_TICK_COUNT_HIGH  *(volatile uint32 *)(SYS_CNT1)
#define SYSTEM_TICK_COUNT   \
    ( ((SYSTEM_TICK_COUNT_LOW) & 0x0000FFFF) + ((SYSTEM_TICK_COUNT_HIGH) << 16) )

// Enable the system tick count
#define ENABLE_SYSTEM_TICK_COUNT

#define get_time_slot( _start, _end ) \
    ( (_end) >= (_start) ) ? ( (_end) - (_start) ) : (0xFFFFFFFF - (_start) + (_end) )


// 0 : fail,  1 : success
static int check_status (FLASH_PTR_TYPE addr_ptr, uint  max_tick)
{
    if (flash_is_simulate)
    {
        addr_ptr = addr_ptr;
        return 1;
    }
    else
    {
        volatile uint32  old_tick, cur_tick;
        volatile uint32  tick_count;
        volatile uint16  tmp, d;

        ENABLE_SYSTEM_TICK_COUNT;

        //lin.liu(2004-07-21). CR10905
        old_tick  = SCI_GetTickCount();

loop:
        //lin.liu(2004-07-21). CR10905
        cur_tick   = SCI_GetTickCount();
        tick_count = get_time_slot (old_tick, cur_tick);

        if (tick_count > max_tick)
        {
            // timerout
            RESET_CYCLE;
            FLASH_SAVE_TRACE_INFOR_CHECK_STATUS;
            return 0;
        }

        FLASH_DISABLE_IRQ;

        d = * (volatile uint16 *) (addr_ptr);
        // read the toggle bit again
        tmp = * (volatile uint16 *) (addr_ptr);

        FLASH_RESTORE_IRQ;

        if ( (d & SR_6) != (tmp & SR_6))
        {
            if (tmp & SR_5)
            {
                // read DQ7-0 twice
                FLASH_DISABLE_IRQ;
                d   = * (volatile uint16 *) (addr_ptr);
                tmp = * (volatile uint16 *) (addr_ptr);
                FLASH_RESTORE_IRQ;

                if ( (d & SR_6) != (tmp & SR_6))
                {
                    // program/erase operation not complete
                    // write reset command
                    RESET_CYCLE;
                    FLASH_SAVE_TRACE_INFOR_CHECK_STATUS;
                    return 0;
                }
                else
                {
                    // not toggle, complete
                    return 2;
                }
            }
            else
            {
                goto loop;
            }
        }
        else
        {
            // not toggle, has complete
            return 1;
        }
    } // if( flash_is_simulate )
}

// @lin.liu(2003-12-04). CR: MS5446.
// Added some function to support check sum feature.

//
// enter_read/exit_read should be called with pair
//
static void enter_read (void)
{
    //Get semp
    FLASH_GetSemaphore();

    FLASH_DISABLE_IRQ;

    if (FLASH_ERASE_PROCESSING_S == s_erase_status)
    {
        // the flash being erase, so suspend first
        FLASH_Suspend();
    }
}
static void exit_read (void)
{
    if (FLASH_ERASE_SUSPEND_S == s_erase_status)
    {
        // the flash being suspended, so resume it.
        FLASH_Resume();
    }

    FLASH_RESTORE_IRQ;

    // release semp
    FLASH_ReleaseSemaphore();

}

//
// Calculate the checksum
// Input:
//  addr : the start address that want to be check.
//  len  : the word counts that want to be check.
// Return:
//  The check sum.
//
uint16 FLASH_GetCheckSum (uint32 addr, uint32 len)
{
    uint16  check_sum = 0xC50F + (uint16) addr;

    enter_read();

    while (len--)
    {
#ifdef WIN32
        check_sum += MAKE_WORD (emu_buffer[addr],     emu_buffer[addr+1]);
#else
        check_sum += * (FLASH_PTR_TYPE) (addr);
#endif
        addr      += sizeof (uint16);

        //@lin.liu(2003-12-16). CR: 6250
        if (len && (0 == (len & 0x1FF)))
        {
            exit_read();
            // Enable Handle INT.
            enter_read();
        }

        // End 6250
    }

    exit_read();

    return check_sum;
}

//
// Check the content of given area are all 0xFFFF ?
//
//
uint32 FLASH_IsEmptySpace (uint32 addr, uint32 end_addr)
{
    uint16  tmp, cmp;

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (end_addr));/*assert verified*/

    // Word boundary.
    SCI_ASSERT (! (addr & 1));/*assert verified*/

    enter_read();

    cmp = 0xFFFF;

    while (addr < end_addr)
    {
#ifdef WIN32
        tmp = (MAKE_WORD (emu_buffer[addr],     emu_buffer[addr+1]));
#else
        tmp = (* ( (volatile uint16 *) addr));
#endif

        if (cmp != tmp)
        {
            break;
        }

        addr += sizeof (tmp);

        //@lin.liu(2003-12-16). CR: 6250
        if (0 == (addr & 0x1FF))
        {
            exit_read();
            // Enable Handle INT.
            enter_read();
        }

        // End 6250
    }

    exit_read();

    return (addr >= end_addr) ? 1 : 0;
}

//
// Read the given count from device.
//
void FLASH_ReadWordByCount (uint32  addr, uint16 *w16_ptr, uint32  count)
{
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr + count * 2));/*assert verified*/

    // Word boundary.
    SCI_ASSERT (! (addr & 1));/*assert verified*/

    enter_read();

    while (count--)
    {
#ifdef WIN32
        *w16_ptr++   = MAKE_WORD (emu_buffer[addr], emu_buffer[addr+1]);
#else
        *w16_ptr++   = * ( (FLASH_PTR_TYPE) addr);
#endif
        addr        += 2;

        //@lin.liu(2003-12-16). CR: 6250
        if (count && (0 == (count & 0x1FF)))
        {
            exit_read();
            // Enable Handle INT.
            enter_read();
        }

        // End 6250

    }

    exit_read();
}

//@ End CR: 5446


/*****************************************************************************/
//  Description :
//      Read word(two byte) from the given address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      the address MUST be word boundary.
/*****************************************************************************/
uint16 FLASH_ReadWord ( // The word that has read from the given address.
    uint32 addr)        // the address that will be read.
{
    uint16   read_data; // the data that be read.

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/

    // Word boundary.
    SCI_ASSERT (! (addr & 1));/*assert verified*/

    // @lin.liu(2003-12-04). CR: MS5446.
    enter_read();


#ifdef WIN32
    read_data = MAKE_WORD (emu_buffer[addr], emu_buffer[addr+1]);
#else
    read_data = * ( (FLASH_PTR_TYPE) addr);
#endif

    exit_read();
    // @End. CR: 5446

    return read_data;
}

/*****************************************************************************/
//  Description :
//      The function reads up to size bytes from the device and stores them in buffer.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN FLASH_Read (     // If read successfully, return true, else false;
    uint32    addr,      // the start address that will be read
    uint8    *buf,       // store the data that being read to this buffer.
    uint32    read_len)  // the count of bytes that will be read.
{
    FLASH_PTR_TYPE      addr_ptr = (FLASH_PTR_TYPE) (addr & 0xFFFFFFFE); // word boundary.


    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (flash_end_addr >= (addr + read_len));/*assert verified*/

    if (0 == read_len)
    {
        return TRUE;
    }

    // @lin.liu(2003-12-04). CR: MS5446.
    enter_read();
    // End.

    if (addr & 1)
    {
        /*
            Not word boundary, so need read the first byte
        */
#ifdef WIN32
        *buf++ = emu_buffer[ addr ];
#else
        *buf++ = SECOND_BYTE (*addr_ptr);
#endif

        addr++;
        read_len--;
    }

    addr_ptr = (FLASH_PTR_TYPE) (addr);

    while (read_len > 1)
    {
#ifdef WIN32
        *buf++  = emu_buffer[addr];
        *buf++  = emu_buffer[addr + 1];
        addr   += 2;
#else
        *buf++ = FIRST_BYTE (*addr_ptr);
        *buf++ = SECOND_BYTE (*addr_ptr);
        addr_ptr++;
#endif

        //@lin.liu(2003-12-16). CR: 6250
        if (0 == (read_len & 0x1FF))
        {
            exit_read();
            // Enable Handle INT.
            enter_read();
        }

        // End 6250

        read_len   -=2;
    }

    if (read_len)
    {
        /*
            Read the last byte.
        */
#ifdef WIN32
        *buf++ = emu_buffer[ addr ];
#else
        *buf++ = FIRST_BYTE (*addr_ptr);
#endif
    }

    exit_read();

    /*
        All Has Done.
    */
    return TRUE;
}

/*****************************************************************************/
//  Description :
//      The function unlock the sector that contain the address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Unlock (
    uint32   addr)  // the sector that contain the address will be unlock.
{
}

/*****************************************************************************/
//  Description :
//      The function lock down the sector that contain the address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Lock (uint32  addr)
{
}

__inline
int fast_check_status (FLASH_PTR_TYPE addr_ptr)
{
    int      i;
    volatile register uint16  d, tmp;

    for (i = 0; i < 200; i++)
    {
        d = * (volatile uint16 *) (addr_ptr);
        // read the toggle bit again
        tmp = * (volatile uint16 *) (addr_ptr);

        if ( (d & SR_6) != (tmp & SR_6))
        {
            if (tmp & SR_5)
            {
                // read DQ7-0 twice
                d   = * (volatile uint16 *) (addr_ptr);
                tmp = * (volatile uint16 *) (addr_ptr);

                if ( (d & SR_6) != (tmp & SR_6))
                {
                    // program/erase operation not complete
                    // write reset command
                    RESET_CYCLE;
                    return 0;
                }
                else
                {
                    // not toggle, complete
                    return 2;
                }
            }
            else
            {
                //goto loop;
            }
        }
        else
        {
            // not toggle, has complete
            return 1;
        }
    }

    RESET_CYCLE;
    return 0;
}

/*****************************************************************************/
//  Description :
//      The function write word to flash.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      The address MUST be word boundary.
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
static BOOLEAN  flash_write_word (
    uint32      addr,
    uint16      data)
{
    //SCI_INTERRUPT_SAVE_AREA
    FLASH_PTR_TYPE          addr_ptr = (FLASH_PTR_TYPE) addr;

    if (flash_is_simulate)
    {
        SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
        SCI_ASSERT (! (addr & 1));/*assert verified*/

        *addr_ptr = data;

        return TRUE;
    }
    else
    {
#ifndef WIN32
        volatile uint16         tmp;
        TX_INTERRUPT_SAVE_AREA

#else
        int                     ret;
#endif

        SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
        SCI_ASSERT (! (addr & 1));/*assert verified*/

#ifdef WIN32
        {
            uint8  fst, snd;

            // Convert to Big-endian
            fst = (data >> 8);
            snd = data & 0xFF;
            emu_buffer[ addr ]      &= fst;
            emu_buffer[ addr + 1 ]  &= snd;
            ret = ( (emu_buffer[addr] == fst) && (emu_buffer[addr+1] == snd))
                  ?  1 : 0;

            return ret;
        }
#else  // Target  

        tmp = *addr_ptr;

        if (tmp == data)
        {
            // It is same, needn't write
            return TRUE;
        }
        else if ( (tmp & data) != data)
        {
            // some bits from 0 to 1, can't write
            return FALSE;
        }

        //Get semp
        FLASH_GetSemaphore();

        TX_DISABLE
        UNLOCK_CYCLE;
        PROGRAM_SETUP_CYCLE;
        // PA = PD
        *addr_ptr = data;
        // polling until complete.
        fast_check_status (addr_ptr);
        RESET_CYCLE;
        TX_RESTORE

        // release semp
        FLASH_ReleaseSemaphore();

        tmp = *addr_ptr;
        //ret = (tmp != data) ? 0 : 1;

        return (BOOLEAN) ( (tmp == data));

#endif // WIN32    

    } // If( flash_is_simulate )
}

BOOLEAN  FLASH_WriteWord (
    uint32    addr,
    uint16    data)
{
    uint32  loop;
    uint16  content;

    for (loop = 0; loop < 3; loop++)
    {
        if (flash_write_word (addr, data))
        {
            return TRUE;
        }

        // failed. If can write to the address directly, try it again.
        content = * ( (FLASH_PTR_TYPE) addr);

        if (data != (data & content))
        {
            // Can't convert from 0 to 1.
            break;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description :
//      The function write data to flash.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
BOOLEAN FLASH_Write (   // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8       *buf,   // Pointer to the buffer containing the data to be written.
    uint32       len)   // number of bytes to write.
{
    //SCI_INTERRUPT_SAVE_AREA

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT ( (addr + len) < flash_end_addr);/*assert verified*/

    if (flash_is_simulate)
    {
        addr -= (uint32) flash_bank_addr;

        while (len)
        {
            emu_buffer[addr++] = *buf++;
            len --;
        }

        len = len;  /* avoid compile-warning */
        return TRUE;
    }
    else
    {
#ifndef WIN32
        FLASH_PTR_TYPE      addr_ptr;
#endif
        int                 ret;
        uint16              tmp, data;

        if (0 == len)
        {
            return 1;
        }

        // maybe need write first byte especially
        if (addr & 0x1)
        {
#ifdef WIN32
            tmp = emu_buffer[ addr ];
#else
            addr_ptr = (FLASH_PTR_TYPE) (addr & 0xFFFFFFFE);
            tmp      = FIRST_BYTE (*addr_ptr);
#endif
            data = MAKE_WORD (tmp, *buf);
            buf++;

            ret = FLASH_WriteWord (addr & 0xFFFFFFFE, data);

            if (!ret)
            {
                return 0;
            }

            addr++;
            len--;
        }

        while (len > 1)
        {
            // write word by word
            tmp  = (uint16) (*buf++);
            data = MAKE_WORD (tmp, *buf);
            buf++;

            if (FLASH_WriteWord (addr, data))
            {
                len         -= 2;
                addr        += 2;
            }
            else
            {
                return 0;
            }
        }

        // maybe need write last one byte
        if (len)
        {
#ifdef WIN32
            tmp = emu_buffer[ addr ];
#else
            addr_ptr = (FLASH_PTR_TYPE) (addr & 0xFFFFFFFE);
            tmp      = SECOND_BYTE (*addr_ptr);
#endif
            data = MAKE_WORD (*buf, tmp);

            ret = FLASH_WriteWord (addr & 0xFFFFFFFE, data);

            if (ret)
            {
                len--;
            }
        }

        return (0 == len);
    } // if ( flash_is_simulate )
}

/*****************************************************************************/
//  Description :
//      Copy data from one area to another.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      Not check before write.
/*****************************************************************************/
BOOLEAN FLASH_Copy (
    uint32 dest,   /* the area we copy to   */
    uint32 sour,   /* the area we copy from */
    uint32  len)   /* the length we want copy*/
{
    uint8     buf[64];  // temp buffer while copy data.

    SCI_ASSERT (FLASH_ADDR_IS_VALID (dest));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (sour));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (dest + len));/*assert verified*/
    SCI_ASSERT (FLASH_ADDR_IS_VALID (sour + len));/*assert verified*/

    //@lin.liu(2003-10-30). CR : MS5351.
    // Fix: the sour/dest is wrong.
    while (len > 64)
    {
        if (!FLASH_Read (sour, buf, 64))
        {
            return FALSE;
        }

        if (!FLASH_Write (dest, buf, 64))
        {
            return FALSE;
        }

        dest += 64;
        sour += 64;
        len  -= 64;
    }

    if (len)
    {
        if (FLASH_Read (sour, buf, len))
        {
            if (FLASH_Write (dest, buf, len))
            {
                return TRUE;
            }
        }

        return FALSE;
    }

    /* All Done */
    return TRUE;
}


/*****************************************************************************/
//  Description :
//      Check can we write data to the area?
//      If some bits of the area from 0 to 1, the write wouldn't success,
//      and return false;
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      Not check before write.
//      This function called from EFS Task, so needn't  check erase status.
//
/*****************************************************************************/
BOOLEAN FLASH_CheckRevisable (
    uint32      addr,  // the start address of the area that will be checked.
    const uint8  *buf, // the buffer that contain the data.
    uint32      len)   // the data length.
{
    FLASH_PTR_TYPE                addr_ptr  = (FLASH_PTR_TYPE) addr;
    uint16                        data, flash_data;
    uint8                         ch;


    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT (flash_end_addr >= (addr + len));/*assert verified*/

    if (0 == len)
    {
        return TRUE;
    }

    addr_ptr = (FLASH_PTR_TYPE) (addr & 0xFFFFFFFE);

    if (addr & 1)
    {
        /*
            check first byte
        */
#ifdef WIN32
        ch = emu_buffer[ addr ];
        addr++;
#else
        ch = SECOND_BYTE (*addr_ptr);
        addr_ptr++;
#endif

        if ( (ch & *buf) != *buf)
        {
            return FALSE;
        }

        buf ++;
        len --;
    }

    while (len > 1)
    {
#ifdef WIN32
        flash_data  = MAKE_WORD (emu_buffer[addr], emu_buffer[addr+1]);
        addr       += 2;
#else
        flash_data = *addr_ptr++;
#endif
        ch = *buf++;
        data = MAKE_WORD (ch, *buf);
        buf++;

        if (data != (flash_data & data))
        {
            return FALSE;
        }

        //@lin.liu(2003-12-16). CR: 6250
        if (0 == (len & 0x1FF))
        {
            exit_read();
            // Enable Handle INT.
            enter_read();
        }

        // End 6250

        len -= 2;         // decrease sizeof(word)
    }

    if (len)
    {
#ifdef WIN32
        ch = emu_buffer[ addr ];
#else
        ch = FIRST_BYTE (*addr_ptr);
#endif
        ch                         &= *buf;

        if (ch != *buf)
        {
            return FALSE;
        }
    }

    return TRUE;
}


/*****************************************************************************/
//  Description :
//      Is the last erase optr completed?
//
//  Global resource dependence :
//      None
//
//  Return :
//      If the erase has been completely, return true, other false
//  Author:
//      Lin.liu
//  Note:
//      This function MUST be called when system assert only,
/*****************************************************************************/
BOOLEAN FLASH_IsEraseCompleted (void)
{
    switch (s_erase_status)
    {
        case FLASH_ERASE_COMPLETED_S:
        case FLASH_ERASE_FAILED_S:
            return TRUE;
            //break;
        case FLASH_ERASE_SUSPEND_S:  // Not completed.
            return FALSE;
            //break;
        case FLASH_ERASE_PROCESSING_S:

            if (check_status ( (FLASH_PTR_TYPE) s_erase_addr, FLASH_MAX_ERASE_TIME))
            {
                s_erase_status = FLASH_ERASE_COMPLETED_S;
            }
            else
            {
                s_erase_status = FLASH_ERASE_FAILED_S;
            }

            return TRUE;
            break;
        default:  // invalid
            SCI_ASSERT (0);/*assert to do*/
            return FALSE;
            //break;
    }

    return FALSE;
}
/*****************************************************************************/
//  Description :
//      Erase the sector that contain the address.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/

BOOLEAN FLASH_Erase (
    uint32 addr)
{
    //SCI_INTERRUPT_SAVE_AREA
    FLASH_PTR_TYPE      addr_ptr;

    SCI_ASSERT (FLASH_ADDR_IS_VALID (addr));/*assert verified*/
    SCI_ASSERT ( (FLASH_ERASE_COMPLETED_S == s_erase_status)  /*assert verified*/
                 || (FLASH_ERASE_FAILED_S    == s_erase_status));

    /*
        only make sure word boundary.
    */
    addr_ptr        = (FLASH_PTR_TYPE) (addr & 0xFFFFFFFE);
    s_erase_addr    = addr;

    if (flash_is_simulate)
    {
        s_erase_status = FLASH_ERASE_PROCESSING_S;

        //addr = addr - (addr % flash_sector_size) - flash_bank_addr;
        addr -= flash_bank_addr;   // get offset from flash_bank_addr
        addr  = (addr / flash_sector_size) * flash_sector_size;  // round to sector boundary.

        memset (&emu_buffer[addr], 0xFF, flash_sector_size);
        emu_buffer[addr] = emu_buffer[addr];  /* avoid compile-warning */

        s_erase_status = FLASH_ERASE_COMPLETED_S;
        return TRUE;
    }
    else
    {
        int   ret;

        //SCI_TRACE_LOW( "Try erase 0x%08X", addr );

#ifdef WIN32
        memset (&emu_buffer[ addr & 0xFFFF0000 ], 0xFF, 0x10000);
        ret = 1;
#else

        //Get semp
        FLASH_GetSemaphore();


        //FLASH_DISABLE_IRQ;
        /*
            only make sure is even.
        */
        addr_ptr = (FLASH_PTR_TYPE) (addr & 0xFFFFFFFE);

        FLASH_DISABLE_IRQ;

        s_erase_status = FLASH_ERASE_PROCESSING_S;

        UNLOCK_CYCLE;

        ERASE_SETUP_CYCLE;

        // SADD = 0x30
        *addr_ptr = 0x30;

        // wait 80us, a sector erase time-out occurs.
        // for if write SRAM on flash, erase will failed.
        // But I don't known the reason.
        // @lin.liu
        {
            //register int  timeout;
            //for( timeout = 0; timeout < 300; timeout++ ){ NULL; }
        }

        FLASH_SAVE_TRACE_INFOR_ERASE;

        FLASH_RESTORE_IRQ;

        ret = check_status (addr_ptr, FLASH_MAX_ERASE_TIME);

        FLASH_DISABLE_IRQ;

        s_erase_status = (ret) ? FLASH_ERASE_COMPLETED_S : FLASH_ERASE_FAILED_S;

        RESET_CYCLE;

        FLASH_RESTORE_IRQ;

        // release semp
        FLASH_ReleaseSemaphore();

#endif // WIn32
        return (BOOLEAN) ret;
    } // if( flash_is_simulate )
}


/*****************************************************************************/
//  Description :
//      Suspend the program/erase operation
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Suspend (void)
{
    //Get semp
    FLASH_GetSemaphore();

    FLASH_DISABLE_IRQ;

    if (FLASH_ERASE_PROCESSING_S == s_erase_status)
    {
        if (!flash_is_simulate)
        {
#ifdef WIN32
            SCI_ASSERT (0);/*assert to do*/  // Shouldn't happen in Win32 Env.
#else
            FLASH_PTR_TYPE      addr_ptr = (FLASH_PTR_TYPE) s_erase_addr;

            *addr_ptr = 0xB0;   // suspend command

            FLASH_SAVE_TRACE_INFOR_SUSPEND;
#endif
        }

        s_erase_status = FLASH_ERASE_SUSPEND_S;
    }

    FLASH_RESTORE_IRQ;

    // release semp
    FLASH_ReleaseSemaphore();
}



/*****************************************************************************/
//  Description :
//      Resume the program/erase operation
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Resume (void)
{
    //Get semp
    FLASH_GetSemaphore();

    FLASH_DISABLE_IRQ ;

    /*
        Check the sector has been suspended ?
    */
    if (FLASH_ERASE_SUSPEND_S == s_erase_status)
    {
        if (!flash_is_simulate)
        {
#ifdef WIN32
            SCI_ASSERT (0);  /*assert to do*/// Shouldn't happen in Win32 Env.
#else
            FLASH_PTR_TYPE  addr_ptr = (FLASH_PTR_TYPE) (s_erase_addr & 0xFFFFFFFE);

            *addr_ptr = 0x30;

            FLASH_SAVE_TRACE_INFOR_RESUME;
#endif
        }

        s_erase_status = FLASH_ERASE_PROCESSING_S;
    }
    else
    {
        /*
            The sector is not suspended, Nothing to do.
        */
    }

    FLASH_RESTORE_IRQ;

    // release semp
    FLASH_ReleaseSemaphore();

}



/*****************************************************************************/
//  Description :
//      Get classmark data address.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_GetFixedNvitemAddr (uint32 *start_addr, uint32 *end_addr)
{
#ifdef WIN32
    *start_addr = (uint32) &emu_buffer[flash_fixed_nvitem_addr];
#else
    *start_addr = flash_fixed_nvitem_addr;
#endif
    *end_addr = *start_addr + flash_sector_size;
}

/*****************************************************************************/
//  Description :
//      Check use SRAM for simulate ?
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN FLASH_IsSimulate (void)
{
#ifdef WIN32
    return TRUE;
#else
    return flash_is_simulate;
#endif
}

uint32 FLASH_GetProductInfoAddr (void)
{
#ifdef WIN32
    return (uint32) &emu_buffer[flash_product_info_addr];
#else
    return flash_product_info_addr;
#endif
}

//
// Erase All the sector that managed by EFS
// This function should be called when disable IRQ.
// and maybe need a long time.
//
BOOLEAN FLASH_Format (uint32 param_1, uint32 param_2)
{
    uint32  addr;
    uint16  sector;

    if (param_1 != ~param_2)
    {
        // Input is wrong, to avoid call this function when run is not correct.
        return FALSE;
    }

    if (FLASH_INIT_MAGIC != s_flash_init_flag)
    {
        // The device has not be inited.
        return FALSE;
    }

    b_in_disable_irq_state = TRUE;

    if (!FLASH_IsEraseCompleted())
    {
        FLASH_Resume();

        if (!FLASH_IsEraseCompleted())
        {
            return FALSE;
        }
    }

    s_erase_status = FLASH_ERASE_COMPLETED_S;


#ifndef USE_NUCLEUS

    // the last sector is used to store protected item.
    // ENABLE_PROTECT_ITEM_FEATURE defined in efs_item.c, when you disable
    // the macro, please use (sector < flash_sector_num).
    for (sector = 0; sector < (flash_sector_num - 1); sector++)
#else

        // This project is for HTW, and only two sector can be used, so don't use
        //protected item feature.
        for (sector = 0; sector < flash_sector_num; sector++)
#endif
        {
            addr = FLASH_SectorToAddr (sector);

            if (!FLASH_Erase (addr))
            {
                return FALSE;
            }
        }

    return TRUE;
}

#ifdef UNIT_TEST
/*
    this function is add to format all the flash managed by the EFS including
    the protected items for test using
*/
BOOLEAN FLASH_FormatAll (uint32 param_1, uint32 param_2)
{
    uint32  addr;
    uint16  sector;

    if (param_1 != ~param_2)
    {
        // Input is wrong, to avoid call this function when run is not correct.
        return FALSE;
    }

    if (FLASH_INIT_MAGIC != s_flash_init_flag)
    {
        // The device has not be inited.
        return FALSE;
    }

    b_in_disable_irq_state = TRUE;

    if (!FLASH_IsEraseCompleted())
    {
        FLASH_Resume();

        if (!FLASH_IsEraseCompleted())
        {
            return FALSE;
        }
    }

    s_erase_status = FLASH_ERASE_COMPLETED_S;


    // This project is for HTW, and only two sector can be used, so don't use
    //protected item feature.
    for (sector = 0; sector < flash_sector_num; sector++)
    {
        addr = FLASH_SectorToAddr (sector);

        if (!FLASH_Erase (addr))
        {
            return FALSE;
        }
    }

    return TRUE;
}
#endif

/*
Daniel.ding  add these functions for HTW opreading flash synchronization with SCI .
*/
void FLASH_GetSemaphore (void)
{
#ifdef WIN32
    //NULL
#else
#ifdef HTW_INTEGRATION
    DrvFlashSemaphDec ();
#else
    //NULL
#endif
#endif
}

void FLASH_ReleaseSemaphore (void)
{
#ifdef WIN32
    //NULL
#else
#ifdef HTW_INTEGRATION
    DrvFlashSemaphInc ();
#else
    //NULL
#endif
#endif
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/*      End flash.c       */

/******************************************************************************
 ** File Name:     flash_drv.c                                                *
 ** Description:                                                              *
 **        norflash operations                                                *
 ** Author:         Younger.yang                                              *
 ** DATE:           11/23/2006                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 11/23/2006      Younger.yang       Create for Jiuyang                     *
 ******************************************************************************/

#include "flash_drv.h"
#include "flash_cfg.h"
#include "packet.h"
#include "fdl_main.h"
#include "sio_drv.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif


/**---------------------------------------------------------------------------*
 **                         Macro defines
 **---------------------------------------------------------------------------*/

//Intel Command Set
#define LOCK_BLOCK_SETUP           0x60
#define UNLOCK_BLOCK               0xd0
#define LOCK_BLOCK                 0x01
#define BUFFER_PROGRAM_SETUP       0xe8
/*start added by victorxu*/
/*start added by feng tao*/
#define M18_BUFFER_PROGRAM_SETUP   0xe9
/*end added by feng tao*/
/*end added by victorxu*/
#define BUFFERED_PROGRAM_CONFIRM   0xd0
#define READ_ARRAY                 0xff
#define DEVICE_READY               0x80

#define TMPL_PROGRAM_SETUP         0x40
/*start added by victorxu*/
/*start added by feng tao*/
#define TMPL_M18_PROGRAM_SETUP     0x41
/*end added by feng tao*/
/*end added by victorxu*/
#define TMPL_BASE_FLASH_ADDRESS    0x00
#define TMPL_READ_STATUS_REGISTER  0x70
#define TMPL_CLEAR_STATUS_REGISTER 0x50
#define TMPL_READ_ID_CODES         0x90
#define TMPL_READ_QUERY            0x98
#define TMPL_BLOCK_ERASE           0x20
#define TMPL_CONFIRM               0xD0
#define TMPL_READ_ARRAY            0xff
#define TMPL_CONFIG_SETUP          0x60

#define TMPL_BLOCK_LOCKED          0x02
#define TMPL_PROGRAM_SUSPENDED     0x04
#define TMPL_VPP_LOW               0x08
#define TMPL_PROGRAM_ERROR         0x10
#define TMPL_ERASE_ERROR           0x20
#define TMPL_ERASE_SUSPENDED       0x40
#define TMPL_STATUS_READY          0x80


#define TMPL_STATUS_READY          0x80
#define mIsStatusReady(A)          (( A & TMPL_STATUS_READY ) == TMPL_STATUS_READY )


//AMD Command Set
#define UNLOCK_CYCLE               *(s_flash_start_ptr + 0x555) = 0xAA; \
    *(s_flash_start_ptr + 0x2AA) = 0x55;
#define PROGRAM_SETUP_CYCLE        *(s_flash_start_ptr + 0x555) = 0xA0
#define ERASE_SETUP_CYCLE          *(s_flash_start_ptr + 0x555) = 0x80; \
    UNLOCK_CYCLE
#define ERASE_CHIP_CYCLE           *(s_flash_start_ptr + 0x555) = 0x10
#define RESET_CYCLE                *(s_flash_start_ptr) = 0xF0;

#define RESET_FLASH   \
{    *(s_flash_start_ptr) = 0xF0;\
     *(s_flash_start_ptr) = 0xFF;\
}


//Flash Command Set Type
#define AMD_SET                    0
#define INTEL_SET                  1

#ifndef MIN
#define MIN( _x, _y )              ( ((_x) < (_y)) ? (_x) : (_y) )
#endif
#define FLASH_DO_IDLE              FDL_PacketDoIdle();

/**--------------------------------------------------------------------------*
 **                         Data Structures                                  *
 **--------------------------------------------------------------------------*/
typedef volatile uint16 *flash_ptr;



/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
static uint32             s_flash_init_flag = 0;

static uint32             s_is_big_endian;
static uint32             s_flash_cmd_set;

/*start added by victorxu*/
/*start added by feng tao*/
uint32             s_intel_m18_serial = 0;
/*end added by feng tao*/
/*end added by victorxu*/

static flash_ptr          s_flash_start_ptr;
static NORFLASH_INFO_T    *s_norflash_info_ptr;



uint32                    g_flash_start_addr;

static uint32 manufacturer_id,device_id,extend_id;


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern const NORFLASH_INFO_T  g_norflash_info[];

extern uint32 FDL_InEfsSpace(uint32 addr);
extern uint32 FDL_EfsAddrConvert(uint32 addr);
/**---------------------------------------------------------------------------*
 **                         Functions
 **---------------------------------------------------------------------------*/

uint32 FLASH_Info_Init(uint32 start_addr)
{
	g_flash_start_addr    = start_addr;
}

/**********************************************
* get_extend_id
***********************************************/
static uint16 get_extend_id (uint32 manu_id,uint32 dev_id)
{
    flash_ptr   ptr = s_flash_start_ptr;
    uint32          b_1, b_2, b_3;
    uint32          ext_id;

    ext_id = 0x0;

    RESET_FLASH;

    //distinguish spansion PL127N between PL127J
    if ( (0x7E2000 == dev_id) && (FLASH_AMD_ID == manu_id))
    {

        // enter autoselect mode
        * (ptr + 0x55) = 0x98;

        b_1 = * (ptr + 0x20);
        b_2 = * (ptr + 0x2a);
        b_3 = * (ptr + 0x58);

        if ( (0x9 == b_1) && (0x6 == b_2) && (0xB == b_3))
        {
            ext_id = 0x1;    //PL127N mirrorbit
        }
        else
        {
            ext_id = 0x0;
        }
    }


    //distinguish  ST T2 between ST T1
    if ( (0x88C4 == dev_id) && (FLASH_ST_ID == manu_id))
    {

        // enter autoselect mode
        * (ptr + 0x55) = 0x98;

        b_1 = * (ptr + 0x127);

        if (0x3 == b_1)
        {
            ext_id = 0x1;    //page len_4
        }
        else if (0x4 == b_1)
        {
            ext_id = 0x0;    //page len_8
        }
    }

    // distinguish  SAMSUNG K5N2866-BQ12 and K5N2833-AF66 
    if((0x2404 == dev_id)&&(FLASH_SAMSUNG_ID == manu_id))
    {
        RESET_FLASH;       						

        // enter CFI mode
        *(ptr + 0x55) = 0x98;
        
        //read max clk freq in CFI mode
        b_1 = *(ptr + 0x4e); 
        
        if(0x6c == b_1)
        {
        	ext_id = 0x1; //K5N2866-BQ12, 108M
        }
        else
        {
        	ext_id = 0x0; //K5N2833-AF66
        }
        
    }
    
    RESET_FLASH;

    return ext_id;
}



/**********************************************
* Get manufacture ID and device ID
***********************************************/
void get_flash_id (uint32 *pManufacturerID, uint32 *pDeviceID, uint32 *pExtendID)
{
    uint32          manu_id, dev_id, ext_id;
    uint32          b_1, b_2, b_3;
    flash_ptr       ptr = s_flash_start_ptr;

    RESET_FLASH;

    UNLOCK_CYCLE;


    // enter autoselect mode
    * (ptr + 0x555) = 0x90;

    manu_id = * (ptr + 0x00);
    dev_id  = * (ptr + 0x01);
    ext_id  = 0x0;
    manu_id &= 0xFF;

    if ((FLASH_AMD_ID == manu_id) || (FLASH_CFEON_ID == manu_id))
    {
        b_1 = (* (ptr + 0x01)) & 0xFF;
        b_2 = (* (ptr + 0x0E)) & 0xFF;
        b_3 = (* (ptr + 0x0F)) & 0xFF;
        dev_id = (b_1 << 16) + (b_2 << 8) + b_3;

        if (0x7E != b_1) // single byte device code
        {
            dev_id = b_1;
        }

    }


    *pManufacturerID  = manu_id;
    *pDeviceID        = dev_id;
    *pExtendID        = get_extend_id (manu_id, dev_id);


    RESET_FLASH;
}

/**********************************************
* check_big_endian
***********************************************/
static void check_big_endian (void)
{
    volatile uint16    data = 0x1234;
    volatile uint8    *ptr = NULL;

    ptr = (volatile uint8 *) (&data);
    s_is_big_endian = (0x12 == *ptr) ? 1 : 0;
}


/**********************************************
* flash_init
***********************************************/
static int flash_init (void)
{

    uint8 i;


    check_big_endian();

    get_flash_id (&manufacturer_id, &device_id, &extend_id);

    for (i=0; g_norflash_info[i].manufacturer_id != NULL; i++)
    {
        if (manufacturer_id == g_norflash_info[i].manufacturer_id &&
                device_id == g_norflash_info[i].device_id &&
                extend_id == g_norflash_info[i].extend_id)
        {
            s_norflash_info_ptr = (NORFLASH_INFO_T *) &g_norflash_info[i];

            if (FLASH_AMD_ID == manufacturer_id || FLASH_SAMSUNG_ID == manufacturer_id || FLASH_SST_ID == manufacturer_id || FLASH_TSB_ID == manufacturer_id||FLASH_CFEON_ID == manufacturer_id)
            {
                s_flash_cmd_set = AMD_SET;
            }
            else
            {
                /*start added by victorxu*/
                /*start added by feng tao*/
                if ( (device_id == 0x8900) || (device_id == 0x8901) || (device_id == 0x887e) || (device_id == 0x8903))
                {
                    s_intel_m18_serial = 1;
                }
                else
                {
                    s_intel_m18_serial = 0;
                }

                /*start added by feng tao*/
                /*end added by victorxu*/
                s_flash_cmd_set = INTEL_SET;
            }

            return 1;
        }
    }

    return 0;

}



/**********************************************
* check_status_default
***********************************************/
int check_status_default (flash_ptr addr_ptr, uint32 retry_count)
{
    volatile uint16  tmp, d;
    volatile int a;

loop:
    FLASH_DO_IDLE

    if (0 == (retry_count--))
    {
        // too long,
        return 0;
    }

    // Read the toggle bit
    d = * (volatile uint16 *) (addr_ptr);

    for (a=0; a<2; a++);

    tmp = * (volatile uint16 *) (addr_ptr);

    if ( (d & SR_6) != (tmp & SR_6))
    {
        if (tmp & SR_5)
        {
            // read DQ7-0 twice
            d   = * (volatile uint16 *) (addr_ptr);

            for (a=0; a<2; a++);

            tmp = * (volatile uint16 *) (addr_ptr);

            if ( (d & SR_6) != (tmp & SR_6))
            {
                // program/erase operation not complete
                // write reset command
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
}



/**********************************************
* check_status_sst
***********************************************/
static int check_status_sst (flash_ptr addr_ptr, uint32 retry_count)
{
    volatile uint16  tmp, d;
    volatile int a;

    while (1)
    {
        FLASH_DO_IDLE

        if (0 == (retry_count--))
        {
            // too long,
            return 0;
        }

        // Read the toggle bit
        d = * (volatile uint16 *) (addr_ptr);

        for (a=0; a<2; a++);

        tmp = * (volatile uint16 *) (addr_ptr);

        if ( (d & SR_6) != (tmp & SR_6))
        {

            //continue;
        }
        else
        {
            // not toggle, has complete
            return 1;
        }
    }
}


/**********************************************
* check_status_intel
***********************************************/
static int check_status_intel (flash_ptr addr_ptr, uint32 retry_count)
{
    volatile uint16  d;

    while (1)
    {
        FLASH_DO_IDLE

        if (0 == (retry_count--))
        {
            // too long,
            return 0;
        }

        * (volatile uint16 *) (addr_ptr) = 0x70;
        // Read the toggle bit
        d = * (volatile uint16 *) (addr_ptr);

        if (!mIsStatusReady (d))
        {

            //continue;
        }
        else
        {

            // not toggle, has complete
            return 1;
        }
    }
}




/**********************************************
* check_status
***********************************************/
static int check_status (flash_ptr addr_ptr, uint32 retry_count)
{
#ifndef _SELF_ADAPT_NORFDL_

    switch (s_norflash_info_ptr->manufacturer_id)
    {
        case FLASH_SST_ID:
            return check_status_sst (addr_ptr, retry_count);
            break;
        case FLASH_INTEL_ID:
        case FLASH_ST_ID:
            return check_status_intel (addr_ptr, retry_count);
        default:
            return check_status_default (addr_ptr, retry_count);
            break;
    }

#else

    switch (g_flash_spec.FlashID)
    {
        case FLASH_SST_ID:
            return check_status_sst (addr_ptr, retry_count);
            break;
        case FLASH_INTEL_ID:
        case FLASH_ST_ID:
            return check_status_intel (addr_ptr, retry_count);
        default:
            return check_status_default (addr_ptr, retry_count);
            break;
    }

#endif
}

/*start added by victorxu*/
/*start added by feng tao*/
LOCAL  uint32 INTEL_EfsAddrConvert (uint32 addr)
{
    uint32 temp_addr;
    // if((addr >=s_efs_start_addr) && (addr < s_efs_start_addr + s_flash_sector_num*s_flash_sector_size))
    {
        if (addr & 0x10)
        {
            temp_addr = 0x10;
            addr = (addr+0x40000) & (~temp_addr);
        }
    }
    return addr;
}
/*end added by feng tao*/
/*end added by victorxu*/

BOOLEAN  Intel_GetFlashType(void)
 {
	return s_intel_m18_serial;
 }


/**********************************************
* NorFDL_FlashReadWord
***********************************************/
uint16 FDL_FlashReadWord (uint32 addr)
{
    RESET_CYCLE;
     if(s_intel_m18_serial)
     {
	     	addr = FDL_EfsAddrConvert(addr);
    addr &= 0xFFFFFFFE;  // word boundary;
      }
     else
     {
     	      addr &= 0xFFFFFFFE;  // word boundary;
     }
    return * ( (flash_ptr) addr);
}


/**********************************************
* NorFDL_FlashRead
***********************************************/
uint32 FDL_FlashRead (
    uint32 addr,
    uint8 *buf,
    uint32 read_len)
{
    uint8 *ptr = (uint8 *) (addr);
    uint8 *tmp = ptr;

    RESET_CYCLE;
	if(s_intel_m18_serial)
	{
        while (read_len--)
        {
			 ptr = (uint8*)FDL_EfsAddrConvert(addr);
			 addr++;
			*buf++ = *ptr;
		}
	}
	else
	{
		while( read_len-- )
		{
        	*buf++ = *ptr++;
		}
    }

    return (uint32) (ptr - tmp);
}


/**********************************************
* is_start_address_of_sector
***********************************************/
static int is_start_address_of_sector (uint32  addr)
{
    uint32  sector_size, start_addr;

    sector_size = s_norflash_info_ptr->get_sector_size (s_norflash_info_ptr->bootflag, addr);
    start_addr  = addr & (~ (sector_size - 1));
    return (start_addr == addr) ? 1 : 0;

}


/**********************************************
* flash_writebuffer
***********************************************/
__inline int flash_writebuffer (   // If the data being written to flash, return TRUE.
    uint32       addr,              // the start address that will be written.
    const uint8   *buf,         // Pointer to the buffer containing the data to be written.
    uint32       counter)           // number of byte to write.
{

    flash_ptr           addr_ptr = (flash_ptr) (addr & 0xFFFFFFFE);
    uint16              wc,d,tmp, data;
    uint32              sector_size;
    int                 ret = 0;
    uint32              dest_addr;
    uint32              writebufferlen;

    writebufferlen = s_norflash_info_ptr -> writebufferlen;


    dest_addr   = (uint32) addr_ptr & ~ (writebufferlen - 1);
    wc          = counter / 2 ;
    addr_ptr    = (uint16 *) addr;
    sector_size = s_norflash_info_ptr->get_sector_size (s_norflash_info_ptr->bootflag, addr);



    if (is_start_address_of_sector (addr))
    {
        // It is the start address of a sector, so, erase it first
        if (0 == FDL_FlashEraseBlock (addr))
        {
            // erase failed.
            sio_trace ("erase failed");
            return 0;
        }
    }

    if (AMD_SET == s_flash_cmd_set)
    {

        UNLOCK_CYCLE;
        * ( (volatile uint16 *) (dest_addr))   = 0x25;     /* write write buffer load command */
        * ( (volatile uint16 *) (dest_addr))   = wc - 1;   /* write word count (minus 1)      */
    }
    else
    {


        * ( (volatile uint16 *) (dest_addr)) = LOCK_BLOCK_SETUP;
        * ( (volatile uint16 *) (dest_addr)) = UNLOCK_BLOCK;

        // Buffer Setup command
        do
        {
            //        *((volatile uint16 *)(dest_addr)) = BUFFER_PROGRAM_SETUP;
            /*start added by victorxu*/
            /*start added by feng tao*/
            if (s_intel_m18_serial)
            {
                * ( (volatile uint16 *) (dest_addr)) = M18_BUFFER_PROGRAM_SETUP;
            }
            else
            {
                * ( (volatile uint16 *) (dest_addr)) =  BUFFER_PROGRAM_SETUP;
            }

            /*end added by feng tao*/
            /*end added by victorxu*/
        }
        while ( (* ( (volatile uint16 *) (dest_addr)) & DEVICE_READY) != DEVICE_READY);

        // Issue buffer write length
        * ( (volatile uint16 *) (dest_addr)) = (counter/2 - 1);

    }

    do
    {
        // write word by word
        data = (uint16) (*buf++);
        tmp  = (uint16) (*buf++);

        if (s_is_big_endian)
        {
            data = (data << 8) + (tmp & 0xFF);
        }
        else
        {
            data = (data & 0xFF) + (tmp << 8);
        }

        addr_ptr = (flash_ptr) (addr & 0xFFFFFFFE);
        *addr_ptr = data;
        addr += 2;
        wc--;

    }
    while (wc > 0);


    if (AMD_SET == s_flash_cmd_set)
    {
        * ( (volatile uint16 *) (dest_addr)) = 0x29; //*addr_ptr = 0x29;

        //ret = check_status( addr_ptr );
        while (1)
        {
            FLASH_DO_IDLE

            d = *addr_ptr;
            tmp = *addr_ptr;
            d ^= tmp;

            if (0 == (d & SR_6))
            {
                // no toggle, check result
                tmp = *addr_ptr;
                ret = (tmp == data) ? 1 : 0;
                break;
            }
        }

        // Reset it
        RESET_CYCLE;
    }
    else
    {

        * ( (volatile uint16 *) (dest_addr)) = BUFFERED_PROGRAM_CONFIRM;

        while ( (* ( (volatile uint16 *) (dest_addr)) & DEVICE_READY) != DEVICE_READY)
        {
            FLASH_DO_IDLE
        };

        // Re-lock the device
        * ( (volatile uint16 *) (dest_addr)) = LOCK_BLOCK_SETUP;

        * ( (volatile uint16 *) (dest_addr)) = LOCK_BLOCK;

        * ( (volatile uint16 *) (dest_addr)) = READ_ARRAY;
    }

    return ret;
}


/**********************************************
* NorFDL_FlashWriteWord
***********************************************/
__inline int  FDL_FlashWriteWord (
    uint32 addr,
    uint16 data)
{
    flash_ptr        addr_ptr = (flash_ptr) (addr & 0xFFFFFFFE);
    int              ret;
    volatile uint16  tmp;
    /*start added by victorxu*/
    //add by tao
    uint32 temp_addr=0;
    uint8 i=0;
    /*end added by victorxu*/


    if (is_start_address_of_sector (addr))
    {
        // It is the start address of a sector, so, erase it first
        if (0 == FDL_FlashEraseBlock (addr))
        {
            // erase failed.
            sio_trace ("erase failed");
            return 0;
        }
    }


    if (data == *addr_ptr)  // needn't write
    {
        //@lin.liu(2004-12-13). CR17520.
        FLASH_DO_IDLE;
        return 1;
    }


    if (AMD_SET == s_flash_cmd_set)
    {

        UNLOCK_CYCLE;

        PROGRAM_SETUP_CYCLE;

        // PA = PD
        *addr_ptr = data;

        // polling until complete.

        ret = check_status (addr_ptr, 0x1000000);

        // Reset it
        RESET_CYCLE;
    }
    else
    {

        *addr_ptr = TMPL_CONFIG_SETUP;//0x60 //Lock Setup
        *addr_ptr = TMPL_CONFIRM;//0xD0 (Block Unlock)

        //*addr_ptr = TMPL_PROGRAM_SETUP;//Program Setup

        /*start added by victorxu*/
        /*start added by feng tao*/
        if (s_intel_m18_serial)
        {
            *addr_ptr = TMPL_M18_PROGRAM_SETUP;//Program Setup
        }
        else
        {
            *addr_ptr = TMPL_PROGRAM_SETUP;//Program Setup
        }

        /*end added by feng tao*/
        /*end added by victorxu*/
        // PA = PD
        *addr_ptr = data;

        // polling until complete.

        ret = check_status (addr_ptr, 0x1000000);

        // Reset it

        *addr_ptr = TMPL_READ_ARRAY; //0xFF Read Array
    }



    tmp = *addr_ptr;
    ret = (tmp == data) ? 1 : 0;


    return ret;
}



/**********************************************
* flash_write
***********************************************/
int flash_write (
    uint32 addr,
    uint8 *buf,
    uint32 len)
{
    int     ret, count = len;
    uint16  tmp, data;
    flash_ptr addr_ptr;
    uint32  data_num;

    uint32  writebufferlen;

    if (0 == s_flash_init_flag)
    {
        return 0;
    }

    if (0 == len)
    {
        return 0;
    }

    // maybe need write first byte especially
    if (addr & 0x1)
    {
        addr_ptr = (flash_ptr) (addr & 0xFFFFFFFE);
        tmp = *addr_ptr;

        if (s_is_big_endian)
        {
            tmp &= 0xFF00;
            data = (uint16) (*buf++);
            data &= 0xFF;
        }
        else
        {
            tmp &= 0xFF;
            data = (uint16) * (buf++);
            data = (data << 8) & 0xFF00;
        }

        tmp += data;
        ret = FDL_FlashWriteWord (addr & 0xFFFFFFFE, tmp);

        if (!ret)
        {
            return 0;
        }

        addr++;
        len--;
    }

    writebufferlen = s_norflash_info_ptr -> writebufferlen;

    if (writebufferlen>0)
    {

        if (len>3)
        {
            //data number <= page size,use writebuffer
            data_num = ( (MIN ( (writebufferlen - (addr % writebufferlen)),len)) /2) *2;

            if ( (addr& (~ (writebufferlen - 1))) == ( (addr + data_num - 1) & (~ (writebufferlen - 1))))
            {
                flash_writebuffer (addr, buf, data_num);
                len -= data_num;
                addr+= data_num;
                buf += data_num;
            }

        }

        while (len>=writebufferlen)
        {
            data_num = writebufferlen;

            if ( (addr& (~ (writebufferlen - 1))) == ( (addr + data_num - 1) & (~ (writebufferlen - 1))))
            {
                flash_writebuffer (addr, buf, data_num);
                len -= data_num;
                addr+= data_num;
                buf += data_num;
            }
            else
            {
                break;
            }
        }


        if (len>3)
        {
            //data number <= page size,use writebuffer
            data_num = ( (MIN (len, writebufferlen)) /2) *2;

            if ( (addr& (~ (writebufferlen - 1))) == ( (addr + data_num - 1) & (~ (writebufferlen - 1))))
            {
                flash_writebuffer (addr, buf, data_num);
                len -= data_num;
                addr+= data_num;
                buf += data_num;
            }

        }

    }

    while (len > 1)
    {
        // write word by word
        data = (uint16) (*buf++);
        tmp  = (uint16) (*buf++);

        if (s_is_big_endian)
        {
            data = (data << 8) + (tmp & 0xFF);
        }
        else
        {
            data = (data & 0xFF) + (tmp << 8);
        }

        if (FDL_FlashWriteWord (addr, data))
        {
            len         -= 2;
            addr        += 2;
        }
        else
        {
            return (count - len);
        }
    }

    // maybe need write last one byte
    if (len)
    {
        addr_ptr = (flash_ptr) (addr);
        tmp = *addr_ptr;

        if (s_is_big_endian)
        {
            tmp  &= 0xFF;
            data  = (uint16) (*buf);
            data  = (data << 8) & 0xFF00;
        }
        else
        {
            tmp  &= 0xFF00;
            data  = (uint16) * (buf++);
            data &= 0xFF;
        }

        tmp += data;
        ret = FDL_FlashWriteWord (addr & 0xFFFFFFFE, tmp);
        len -= ret;
    }

    return (count - len);
}

/*start added by victorxu*/
/*start added by feng tao*/
int flash_intel_m18_write (
    uint32 addr,
    uint8 *buf,
    uint32 len)
{
    int     ret, count = len;
    uint16  tmp, data;
    flash_ptr addr_ptr;
    uint32  data_num;
    uint32  writebufferlen;

    if (0 == s_flash_init_flag)
    {
        return 0;
    }

    if (0 == len)
    {
        return 0;
    }


    // maybe need write first byte especially
    if (addr & 0x1)
    {
        addr_ptr = (flash_ptr) (addr & 0xFFFFFFFE);
        tmp = *addr_ptr;

        if (s_is_big_endian)
        {
            tmp &= 0xFF00;
            data = (uint16) (*buf++);
            data &= 0xFF;
        }
        else
        {
            tmp &= 0xFF;
            data = (uint16) * (buf++);
            data = (data << 8) & 0xFF00;
        }

        tmp += data;
        ret =flash_writebuffer (addr & 0xFFFFFFFE, (uint8 *) &tmp, 2);

        if (!ret)
        {
            return 0;
        }

        addr++;
        len--;
    }

#ifndef _SELF_ADAPT_NORFDL_
    writebufferlen = s_norflash_info_ptr -> writebufferlen;
#else
    writebufferlen = g_flash_spec.WriteBufLen;
#endif

    if (writebufferlen>0)
    {

        if (len>3)
        {
            //data number <= page size,use writebuffer
            data_num = ( (MIN ( (writebufferlen - (addr % writebufferlen)),len)) /2) *2;

            if ( (addr& (~ (writebufferlen - 1))) == ( (addr + data_num - 1) & (~ (writebufferlen - 1))))
            {
                flash_writebuffer (addr, buf, data_num);
                len -= data_num;
                addr+= data_num;
                buf += data_num;
            }

        }

        while (len>=writebufferlen)
        {
            data_num = writebufferlen;

            if ( (addr& (~ (writebufferlen - 1))) == ( (addr + data_num - 1) & (~ (writebufferlen - 1))))
            {
                flash_writebuffer (addr, buf, data_num);
                len -= data_num;
                addr+= data_num;
                buf += data_num;
            }
            else
            {
                break;
            }
        }


        if (len>3)
        {
            //data number <= page size,use writebuffer
            data_num = ( (MIN (len, writebufferlen)) /2) *2;

            if ( (addr& (~ (writebufferlen - 1))) == ( (addr + data_num - 1) & (~ (writebufferlen - 1))))
            {
                flash_writebuffer (addr, buf, data_num);
                len -= data_num;
                addr+= data_num;
                buf += data_num;
            }

        }

    }

    while (len > 1)
    {
        // write word by word
        data = (uint16) (*buf++);
        tmp  = (uint16) (*buf++);

        if (s_is_big_endian)
        {
            data = (data << 8) + (tmp & 0xFF);
        }
        else
        {
            data = (data & 0xFF) + (tmp << 8);
        }

        if (flash_writebuffer (addr, (uint8 *) &data, 2))
        {
            len         -= 2;
            addr        += 2;
        }
        else
        {
            return (count - len);
        }
    }

    // maybe need write last one byte
    if (len)
    {
        addr_ptr = (flash_ptr) (addr);
        tmp = *addr_ptr;

        if (s_is_big_endian)
        {
            tmp  &= 0xFF;
            data  = (uint16) (*buf);
            data  = (data << 8) & 0xFF00;
        }
        else
        {
            tmp  &= 0xFF00;
            data  = (uint16) * (buf++);
            data &= 0xFF;
        }

        tmp += data;
        ret =flash_writebuffer (addr & 0xFFFFFFFE, (uint8 *) &tmp, 2);
        len -= ret;
    }

    return (count - len);
}
int flash_intel_m18_write_ext(    
    uint32       addr,  // the start address that will be written.
    const uint8      * buf,   // Pointer to the buffer containing the data to be written.
    uint32       len)   // number of bytes to write.
{
	int 	ret;
	int 	count = len;
	flash_ptr    	addr_ptr,tmp_addr_ptr;    
	uint32      sec_addr,sector_size,writebufferlen,offset,i; 
	uint32      temp_addr = 0;
	uint8*      data_ptr = NULL;
	uint16  	 tmp, data;  
	uint16      loop_count = 0;
	uint32      data_num;
       uint16*     testbuf;
	if( 0 == len )
		return 0;
	if( addr & 0x1 )
	{
		addr_ptr = (flash_ptr)(addr & 0xFFFFFFFE);
		tmp_addr_ptr =  (flash_ptr)FDL_EfsAddrConvert((uint32)addr_ptr );
		tmp      = (uint8)( (*tmp_addr_ptr) >> 8 );
		data = ((uint16)(tmp) << 8) + ((uint16)(uint8)(*buf)) ;
		buf++;
		temp_addr = FDL_EfsAddrConvert(addr  & 0xFFFFFFFE );
		ret = FDL_FlashWriteWord(temp_addr, data );
		if( !ret )
		{
			return (count - len );
		}
		addr++;
		len--;
	}
	while( len > 1 )
	{
		tmp  = (uint16)(*buf++);
		data = ((uint16)(tmp) << 8) + ((uint16)(uint8)(*buf)) ;
		buf++;
		temp_addr = FDL_EfsAddrConvert(addr  & 0xFFFFFFFE );
		if( FDL_FlashWriteWord( temp_addr , data ) )
		{
			len	      -= 2;
			addr        += 2;		
		}
		else
		{
			return (count - len );
		}
	}
/*end added by feng tao*/
	if( len )
	{
		addr_ptr = (flash_ptr)(addr & 0xFFFFFFFE);
		tmp_addr_ptr =  (flash_ptr)FDL_EfsAddrConvert((uint32)addr_ptr );
		tmp      = (uint8)( (*tmp_addr_ptr) & 0xFF );
		data     = ((uint16)(*buf) << 8) + ((uint16)(uint8)(tmp));
		temp_addr = FDL_EfsAddrConvert(addr  & 0xFFFFFFFE );
		ret = FDL_FlashWriteWord(temp_addr, data );
		if( ret )  len--;
	}
	return (count - len ); 
}

LOCAL void flash_unprotect (uint32 start_addr)
{
    switch (manufacturer_id)
    {
        case FLASH_SAMSUNG_ID:
            {
                switch (device_id)
                {
                    case 0x2402:
                    case 0x2403:
                    case 0x2404:
                    case 0x2405:
                    case 0x2206:
                    case 0x2208:
                        * (volatile uint16 *) (start_addr) = 0x60;
                        * (volatile uint16 *) (start_addr) = 0x60;
                        * (volatile uint16 *) ( (start_addr&0xFFFFF000) + 0x84) = 0x60;
                        RESET_CYCLE
                        break;
                    default:
                        break;
                }
            }
            break;
            //add new case here if need
        default:
            break;
    }
}

/**********************************************
* NorFDL_FlashEraseBlock
***********************************************/
int FDL_FlashEraseBlock (
    uint32 addr)
{
    flash_ptr   addr_ptr;
    volatile    int ret;


    /*
        only make sure is even.
    */
    addr_ptr = (flash_ptr) (addr & 0xFFFFFFFE);

    flash_unprotect ( (uint32) addr_ptr);

    if (AMD_SET == s_flash_cmd_set)
    {
        UNLOCK_CYCLE;

        ERASE_SETUP_CYCLE;

        // SADD = 0x30
        *addr_ptr = 0x30;

        // wait 80us ( erase time-out )
        for (ret = 0; ret < 1000; ret++)
        {
            ;
        }

        ret = check_status (addr_ptr, 0x1000000);

        RESET_CYCLE;
    }
    else
    {
        *addr_ptr = TMPL_CONFIG_SETUP;//0x60 //Lock Setup
        *addr_ptr = TMPL_CONFIRM;//0xD0 (Block Unlock)

        *addr_ptr = TMPL_BLOCK_ERASE;//0x20 Block Erase Setup
        *addr_ptr = TMPL_CONFIRM;//0xD0 Erase Confirm

        ret = check_status (addr_ptr, 0x1000000);

        *addr_ptr = TMPL_READ_ARRAY;//0xff
    }


    return ret;
}



/**********************************************
* NorFDL_FlashErase
***********************************************/
uint32 FDL_FlashErase (uint32 start_addr,  uint32 size)
{
    uint32  sector_size, tmp;
    int     ret;
	uint32   addr;
	uint16	status = 0x55aa;

    if (0 == s_flash_init_flag)
    {
        return 0;
    }

    sector_size = s_norflash_info_ptr->get_sector_size (s_norflash_info_ptr->bootflag, start_addr);

    tmp = start_addr & (sector_size - 1);

    // roundary start address to sector boundary
    start_addr -= tmp;

    // after roundary the start address, need roundary erase size
    size += tmp;

    while ( (size != 0) && (0 == (size & 0x80000000)))   // check overflow
    {
        // size > 0,  not overflow
        ret = FDL_FlashEraseBlock (start_addr);

        if (0 == ret)
        {
            return ret;
        }

        sector_size = s_norflash_info_ptr->get_sector_size (s_norflash_info_ptr->bootflag, start_addr);

        start_addr += sector_size;
		addr = start_addr -2;
		if(FDL_InEfsSpace(addr))
		{
			if(s_intel_m18_serial)
			{
				addr =FDL_EfsAddrConvert(addr);
				ret = FDL_FlashEraseBlock( addr );
				if( 0 == ret )
				{
					return ret;
				}
			}
			FDL_FlashWriteWord(addr, status);
		}
        size       -= sector_size;
    }

    return ret;
}


/**********************************************
* NorFDL_FlashEraseChip
***********************************************/
uint32 FDL_FlashEraseChip (void)
{
    int ret;

    if (0 == s_flash_init_flag)
    {
        return 0;
    }

    UNLOCK_CYCLE;

    ERASE_SETUP_CYCLE;

    ERASE_CHIP_CYCLE;

    ret = check_status ( (flash_ptr) (g_flash_start_addr), 0xFFFFFFFF);

    RESET_CYCLE;

    return (uint32) ret;
}


/**********************************************
* NorFDL_FlashWriteEnable
***********************************************/
uint32 FDL_FlashWriteEnable (uint32 start_addr, uint32  enable)
{
    if (enable)
    {
        g_flash_start_addr    = start_addr;
        s_flash_start_ptr     = (flash_ptr) (start_addr);
        s_flash_init_flag     = flash_init();
    }
    else
    {
        s_flash_init_flag     = 0;
    }

    return 1;
}


/**********************************************
* NorFDL_FlashWriteArea
***********************************************/
uint32 FDL_FlashWriteArea (uint32 addr, char *buf, uint32 size)
{


    /*start added by victorxu*/
    /*start added by feng tao*/
    uint32 ret;

    uint32 i;
    uint16 dat1;
    uint16 dat2;

    if (0 == s_flash_init_flag)
    {
        return 0;
    }

    if (s_intel_m18_serial)
    {
     		if(FDL_InEfsSpace(addr))
     		{
     			ret = (uint32)flash_intel_m18_write_ext(addr, (uint8 *)buf, size);
     		}
     		else
     		{
			ret = (uint32)flash_intel_m18_write(addr, (uint8 *)buf,  size);
     		}
     }
    else
    {
        ret = (uint32) flash_write (addr, (uint8 *) buf, size);
    }

    return ret;
    /*start added by feng tao*/
    /*end added by victorxu*/
}





/**********************************************
* NorFDL_FlashCheckSectorSize
***********************************************/
uint32 FDL_FlashCheckSectorSize (uint32 start_addr)
{
    uint32  sec_size;
    uint32 temp_addr =0;
    temp_addr = g_flash_start_addr;
    FLASH_Info_Init(get_flash_base_addr(start_addr));
    sec_size =  s_norflash_info_ptr->get_sector_size( s_norflash_info_ptr->bootflag, start_addr );
    FLASH_Info_Init(temp_addr);
    return sec_size;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

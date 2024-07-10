/******************************************************************************
 ** File Name:      dsp_ctrl_hal.c                                              *
 ** Author:         yuhua.shi                                                *
 ** DATE:           08/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the function of dsp operating     *
 **                                                                                            *
*******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME                     DESCRIPTION                               *
 ** 08/12/2011      yuhua.shi       Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_api.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "dsp_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
 typedef struct
{
    uint16 arm_control_status;
    uint16 dsp_control_status;
    uint16 dl_offset;
    uint16 dl_block_size;
} DSP_DL_CTL_T;

 #define DSP_DL_WAIT_TIME         10000
 #define SHARE_MEM_CTRL_SIZE    10
 #define DSP_DL_BASE_ADDRESS    0xA800
 
// Arm control status
#define DSP_DL_ARM_READY         0x0001    // Indicate arm is ready to send data
#define DSP_DL_ARM_DATA_READY    0x0002    // Indicate arm has copied data to share memory
#define DSP_DL_ARM_START_COPY    0x0004    // Indicate arm is beginning tp copy data
#define DSP_DL_ARM_BOOT_DONE     0x0008    // Indicate dsp code is download successfully

// Dsp control status
#define DSP_DL_DSP_READY         0x0001    // Indicate dsp is ready to receive data
#define DSP_DL_DSP_READY_TO_COPY 0x0002    // Indicate dsp is ready to copy next block
#define DSP_DL_DSP_COPY_DONE     0x0004    // Indicate dsp has copied the current block
#define DSP_DL_DSP_RUN           0x0008    // Indicate dsp is running

#define DSP_DL_MAX_BLOCK_SIZE (2048 - sizeof(DSP_DL_CTL_T) / sizeof(uint16))
#define DSP_DL_BLOCK_SIZE  1024    // current block size each time arm download to dsp

 #define SHARE_MEM_WRITE(addr, value)   addr = value;\
    /* Make sure the status is OK!*/\
    while (addr != value)\
    {\
        addr = value;\
    }
#define SHARE_MEM_READ(addr, value)  \
    for (;;) \
    {\
        value = addr;\
        if (addr == value)\
        {\
            break;\
        }\
    }
    


/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
LOCAL volatile DSP_DL_CTL_T *s_dl_ctl_status;

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void _DSP_CTRL_SM_Init()                                                        *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Init share memory                                                     *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *     
 **---------------------------------------------------------------------------*/
LOCAL void _DSP_CTRL_SM_Init(void)
{
    SCI_MEM16SET((uint16 *) INTER_SHARE_MEM_BEGIN, 0, SHARE_MEM_CTRL_SIZE * sizeof(uint16));
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void _DSP_CTRL_InitHardware()                                                        *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     init hardware of dsp download                                                     *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *     
 **---------------------------------------------------------------------------*/
LOCAL void _DSP_CTRL_InitHardware(void)
{
    // First use dsp reset pin to hold DSP
    DSP_in_Reset();

    // Set dsp download program data base addr
    DSP_Set_Boot_Vector(DSP_DL_BASE_ADDRESS);

    // Set DSP run in internal mode
    DSP_Set_DSP_Strap_Mode(USER_RST_MODE);

    // Set dsp boot mode
    DSP_Boot_Enable();


    SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, DSP_DL_ARM_READY);

    //Clear Dsp Reset pin to let  DSP starts running.
    DSP_in_Release ();

    DSP_IRQ_CLR ();

    //DSP_IRQ_EN ();
    
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void _DSP_CTRL_DspIsResponse()                                                 *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Check DSP interrupt flag                                              *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *     
 **---------------------------------------------------------------------------*/
LOCAL BOOLEAN _DSP_CTRL_DspIsResponse(void)
{
    while (!DSP_is_DSP_IRQ_EN()) {}

    DSP_IRQ_CLR();
    DSP_FIQ_CLR();

    return TRUE;
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void _DSP_CTRL_DL_Data()                                                  *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Download a frame of data.                                             *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *     
 **---------------------------------------------------------------------------*/
LOCAL void _DSP_CTRL_DL_Data(uint16 *data, uint16 offset, uint16 size,uint16 last_block)
{
    // @PROTOCOL_6: ARM write an starting address, block size and the 
    //              associated data to the address word and data range.
    //              clear DSP copy done bit. Set ARM data bit high.
    //              Starts to poll for DSP ready copy bit.

    // write an starting address
    SHARE_MEM_WRITE(s_dl_ctl_status->dl_offset, offset);

    // Set download block size
    SHARE_MEM_WRITE(s_dl_ctl_status->dl_block_size, size);

    // Copy associated data to the address word and data range
    SCI_MEM16CPY((uint16 *) (s_dl_ctl_status + 1), data + offset, size * sizeof(uint16));

    // Set ARM data bit high
    SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, (DSP_DL_ARM_READY | DSP_DL_ARM_DATA_READY));

    // Starts to poll for DSP ready copy bit
    for (;;)
    {
        uint32 abort_count = 0;

        while (! ( s_dl_ctl_status->dsp_control_status &  DSP_DL_DSP_READY_TO_COPY) )
        {
            abort_count++;

            if (abort_count >= DSP_DL_WAIT_TIME)
            {
                // Set arm is ready for download
                // @PROTOCOL_1: ARM set ready bit 
                SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, (DSP_DL_ARM_READY | DSP_DL_ARM_DATA_READY));
            }
        }

        if (s_dl_ctl_status->dsp_control_status & DSP_DL_DSP_READY_TO_COPY)
        {
            break;
        }
    }

    // @PROTOCOL_8: MCU sees DSP resdy copy bit. clear ARM data bit. set
    //              start copy bit. poll for DSP copy done bit.

    // clear ARM data bit
    {
        uint32 status;
        SHARE_MEM_READ(s_dl_ctl_status->arm_control_status, status);
        status &= (uint16) ~DSP_DL_ARM_DATA_READY;
        SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, status);
    }
    
    //check if it is the last block, if yes, set boot done bit high. Note
    // it must be before the setting the start copy bit high to allow correct
    //execution sequence in the DSP side.
    if (last_block)
    {
        uint32 status;
        SHARE_MEM_READ(s_dl_ctl_status->arm_control_status, status);
        status |=  DSP_DL_ARM_BOOT_DONE;
        SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, status);
    }        

    // set start copy bit
    {
        uint32 status;
        SHARE_MEM_READ(s_dl_ctl_status->arm_control_status, status);
        status |= DSP_DL_ARM_START_COPY;
        SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, status);
    }        

    // poll for DSP copy done bit
    for (;;)
    {
        uint32 abort_count = 0;

        while (! ( s_dl_ctl_status->dsp_control_status &  DSP_DL_DSP_COPY_DONE) )
        {
            abort_count++;

            if (abort_count >= DSP_DL_WAIT_TIME)
            {
                // Set arm is ready for download
                // @PROTOCOL_1: ARM set ready bit 
                if (last_block)
                {
                    uint32 status;
                    SHARE_MEM_READ(s_dl_ctl_status->arm_control_status, status);
                    status |= DSP_DL_ARM_BOOT_DONE;
                    SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, status);
                }
                else
                {
                    uint32 status;
                    SHARE_MEM_READ(s_dl_ctl_status->arm_control_status, status);
                    status |= DSP_DL_ARM_START_COPY;
                    SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, status);            
                }
            }
        }

        if (s_dl_ctl_status->dsp_control_status & DSP_DL_DSP_COPY_DONE)
        {
            break;
        }
    }

    // @PROTOCOL_10: ARM sees DSP copy done and DSP interrupt! Clear start copy bit.
    while (!_DSP_CTRL_DspIsResponse()) {}

}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void _DSP_CTRL_DL_Loop()                                                        *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     DSP download main loop.                                               *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *     
 **---------------------------------------------------------------------------*/
LOCAL void _DSP_CTRL_DL_Loop(uint16 *data, uint32 size)
{
    uint32 i = 0;
    uint32 block_num;
    uint32 last_block_size;
    uint32 abort_count = 0;

    if (0 == size)
    {
        return;
    }

    // Wait until dsp is ready
    // @PROTOCOL_3 ARM starts to poll DSP ready
    for (;;)
    {
        while ( ! ( s_dl_ctl_status->dsp_control_status & DSP_DL_DSP_READY ) )
        {
            abort_count++;

            if (abort_count >= DSP_DL_WAIT_TIME)
            {
                // Set arm is ready for download
                // @PROTOCOL_1: ARM set ready bit 
                SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, DSP_DL_ARM_READY);
            }
        }

        if (s_dl_ctl_status->dsp_control_status & DSP_DL_DSP_READY)
        {
            break;
        }
    }

    // Get the total block number 
    block_num = ((size - 1) / DSP_DL_BLOCK_SIZE)+1;

    //get last block size
    last_block_size = size - (block_num-1)*DSP_DL_BLOCK_SIZE;
    // First copy the full blocks
    if (block_num ==1 )
    {    
        //we have only one block
        _DSP_CTRL_DL_Data(data, 0,last_block_size,1 );       
    }
    else
    {
        uint32 status;
        for (i = 0; i < block_num - 1; i++)
        {
            // Download data
            _DSP_CTRL_DL_Data(data, i * DSP_DL_BLOCK_SIZE,DSP_DL_BLOCK_SIZE,0 );
            // Clear start copy bit
            SHARE_MEM_READ(s_dl_ctl_status->arm_control_status, status);
            status &= (uint16) ~DSP_DL_ARM_START_COPY;
            SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, status);        
        }/*end for*/            

        // Download data 
        _DSP_CTRL_DL_Data(data, i * DSP_DL_BLOCK_SIZE, last_block_size,1);

        // Clear start copy bit
        SHARE_MEM_READ(s_dl_ctl_status->arm_control_status, status);
        status &= (uint16) ~DSP_DL_ARM_START_COPY;
        SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, status);

        // Set boot done!
        SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, DSP_DL_ARM_BOOT_DONE);   
    }
}

/**---------------------------------------------------------------------------*
 ** FUNCTION                                                                  *
 **     void _DSP_CTRL_Download()                                                   *
 **                                                                           *
 ** DESCRIPTION                                                               *
 **     Main entry to download the DSP program.                               *
 **                                                                           *
 ** DEPENDENCIES                                                              *
 **                                                                           *     
 **---------------------------------------------------------------------------*/
LOCAL void _DSP_CTRL_Download( uint16 *dsp_pdata_ptr, uint32 data_size )
{
    s_dl_ctl_status = (DSP_DL_CTL_T *) CHIP_DSPShareMemBase();

    // First hold DSP
    DSP_in_Reset ();

    // Init share memory! Clear all share memory.
    // @PROTOCOL_0: ARM clear the first 3 memory location at the protocol start address.
    _DSP_CTRL_SM_Init();

    // Set arm is ready for download
    // @PROTOCOL_1: ARM set ready bit 
    SHARE_MEM_WRITE(s_dl_ctl_status->arm_control_status, DSP_DL_ARM_READY);

    // Init hardware
    // Set Dsp to be boot mode and set the download addr
    // @PROTOCOL_2: ARM write the protocol start address to an address which is in DSP Z space.
    _DSP_CTRL_InitHardware();

    _DSP_CTRL_DL_Loop(dsp_pdata_ptr, data_size);

    //download finishes here

    DSP_Set_DSP_Strap_Mode(EXT_PROG_MODE);

    DSP_Boot_Disable ();

    DSP_in_Reset ();

    //DSP_in_Release ();

}

/*********************************************************************************************/
//Description:   This function is used to download dsp code
//Author:          Yuhua.Shi
//Note:         the unit of input data_size is half-word(16bit)
//Parameter:
/*********************************************************************************************/
PUBLIC void DSP_CTRL_DownLoadCode(const uint16 *dsp_pdata_ptr, uint32 data_size)
{

    _DSP_CTRL_Download((uint16*)dsp_pdata_ptr, data_size);       

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of dsp_ctrl_hal.c


#include "version.h"
#include "tf_load.h"
#include "tf_cfg.h"
#include "tf_main.h"
#include "cmddef.h"
#include "ldo_drvapi.h"

extern const char*  const s_version_info[];

extern TF_UPDATE_INFO_PTR tf_update_info_ptr;

__asm void __FPU_Enable(void)
{        
    ARM       
    //Permit access to VFP/NEON, registers by modifying CPACR   
    MRC     p15,0,R1,c1,c0,2      
    ORR     R1,R1,#0x00F00000   
    MCR     p15,0,R1,c1,c0,2   
    //Ensure that subsequent instructions occur in the context of VFP/NEON access permitted   
    ISB        //Enable VFP/NEON    
    VMRS    R1,FPEXC     
    ORR     R1,R1,#0x40000000     
    VMSR    FPEXC,R1        //Initialise VFP/NEON registers to 0    
    MOV     R2,#0        //Initialise D16 registers to 0    
    VMOV    D0, R2,R2       
    VMOV    D1, R2,R2      
    VMOV    D2, R2,R2     
    VMOV    D3, R2,R2      
    VMOV    D4, R2,R2       
    VMOV    D5, R2,R2    
    VMOV    D6, R2,R2      
    VMOV    D7, R2,R2     
    VMOV    D8, R2,R2    
    VMOV    D9, R2,R2      
    VMOV    D10,R2,R2     
    VMOV    D11,R2,R2     
    VMOV    D12,R2,R2     
    VMOV    D13,R2,R2      
    VMOV    D14,R2,R2      
    VMOV    D15,R2,R2 
    #if 0
    VMOV    D16,R2,R2      
    VMOV    D17,R2,R2    
    VMOV    D18,R2,R2     
    VMOV    D19,R2,R2      
    VMOV    D20,R2,R2      
    VMOV    D21,R2,R2      
    VMOV    D22,R2,R2    
    VMOV    D23,R2,R2     
    VMOV    D24,R2,R2     
    VMOV    D25,R2,R2      
    VMOV    D26,R2,R2      
    VMOV    D27,R2,R2    
    VMOV    D28,R2,R2     
    VMOV    D29,R2,R2    
    VMOV    D30,R2,R2   
    VMOV    D31,R2,R2 
    #endif
    VMRS    R2,FPSCR       
    LDR     R3,=0x00086060 //Mask off all bits that do not have to be preserved. Non-preserved bits can/should be zero.     
    AND     R2,R2,R3     
    VMSR    FPSCR,R2   
    BX      LR
}


int main(void)
{
    uint8 errno = ERR_SUCCESS;
    char szstep[100] = {0};

    ESAL_TS_RTE_CP_WRITE(p15, 0, 0x80000000, c12, c0, 0);
    ESAL_TS_RTE_CP_WRITE(p15, 0, 0x80000000, c12, c0, 1);
	__FPU_Enable();
	
    TF_UartTrace("TF Enter main\n");
    TF_SetLcdBackLight(FALSE, 0x1f);
    TF_HardWareInit();
    TF_ParamInit();

    while ( TF_LcdInit() != ERR_LCD_NONE)
    {
        TF_UartTrace("TF LCD Init XX:\n");
        SCI_Sleep(20*1000);
    }

    TF_UartTrace("TF update routin enter!\n");
    

    sprintf(szstep, "Image Updating Enter!\r\n%s\r\n WARNING:DO NOT POWER OFF!", s_version_info[PROJECT_VERSION]);


    TF_UartTrace("TF update Display Text!\n");
    TF_DisplayText(szstep, strlen(szstep));
    TF_ShowProgress(0,100);

    TF_UartTrace("TF update Turn on  LCD!\n");
    TF_SetLcdBackLight(TRUE, 0x1f);

    errno = TF_Load(TFLOAD_DEFAULT_DIR);
    if (errno != ERR_SUCCESS)
    {
        TF_Show_Error(errno);
        SCI_Sleep(3*1000);
        memset(szstep, 0, sizeof(szstep));
        sprintf(szstep, "Updating Failed!\r\nPlease use the correct software");
        TF_DisplayText(szstep, strlen(szstep));
        SCI_Sleep(3*1000);
        TF_UartTrace("TF Card Update Failed\n");
        tf_update_info_ptr->tf_update_magic = 0x0;
        TF_SetLcdBackLight(FALSE, 0);
        LDO_TurnOffLDO(LDO_LDO_LCD);
        LCD_Close();
        CHIP_PHY_Set_HwRstFlag(RESET_MODE);
        CHIP_Reset_MCU();
    }
/*    
    errno = TF_Load_IMS(TFLOAD_DEFAULT_DIR);
    if (errno != ERR_SUCCESS)
    {
        TF_Show_Error(errno);
        SCI_Sleep(3*1000);
    }
    
    errno = TF_Load_ConfigNV(TFLOAD_DEFAULT_DIR);
    if (errno != ERR_SUCCESS)
    {
        TF_Show_Error(errno);
        SCI_Sleep(3*1000);
    }
    
    errno = TF_Load_APN(TFLOAD_DEFAULT_DIR);
    if (errno != ERR_SUCCESS)
    {
        TF_Show_Error(errno);
        SCI_Sleep(3*1000);
    }
*/
    if (ERR_SUCCESS == errno)
    {
        TF_ShowProgress(100,100);
        tf_update_info_ptr->tf_update_magic = 0x0;
        TF_SetLcdBackLight(FALSE, 0);
        LDO_TurnOffLDO(LDO_LDO_LCD);
        LCD_Close();
        CHIP_PHY_Set_HwRstFlag(RESET_MODE);
        CHIP_Reset_MCU();
    }

    TF_UartTrace("TF Card Update finished\n");

    while(1);
}



void _fp_init ()
{
    //TODO
}


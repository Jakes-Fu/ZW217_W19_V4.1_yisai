
MCFLAG_OPT   	= -DCHIP_SC6600D -DCHIP_SC6600C -DSC6600M 


ifeq ($(strip $(CHIP_VER)),6610) 
    ifeq ($(strip $(DSPZIP_COMPRESS_SUPPORT)), TRUE)
        MCFLAG_OPT   	+= -DBZP_DSP_CODE
    endif
endif


ifeq ($(strip $(DSP_HW_COMPRESS)), TRUE)
MCFLAG_OPT   	+= -DHW_DSP_COMPRESS 
endif

ifeq ($(strip $(PLATFORM)), SC6530)
MCFLAG_OPT   	+= -DSC6600M3 -DCHIP_SC6600L  
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MCFLAG_OPT   	+= -DSC6600M3 -DCHIP_SC6600L  
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
MCFLAG_OPT   	+= -DSC6600M3 -DCHIP_SC6600L  
endif

ifeq ($(strip $(CHIP_DSP)), 6500)
MCFLAG_OPT   	+= -DLAYER1_SC6530C 
endif

ifeq ($(strip $(CHIP_DSP)), 6531)
MCFLAG_OPT   	+= -DLAYER1_SC6530C 
endif

MINCPATH =  BASE/PS/export/inc BASE/Layer1/export/inc BASE/layer1_dsp/export/inc
MINCPATH += chip_drv/export/inc/outdated
             
MSRCPATH =  BASE/layer1_dsp/source/c
              
SOURCES	 = dsp_operate.c layer1_dsp_download.c dump_DSP_pdata.c


ifeq ($(strip $(PLATFORM)), SC6600L)
SOURCES         += DSP_simple_pdata_sc6600l.c
endif

ifeq ($(strip $(PLATFORM)), SC6800H)
SOURCES         += DSP_simple_pdata_sc6800h.c
endif

ifeq ($(strip $(PLATFORM)), SC6530)
SOURCES         += DSP_simple_pdata_sc6530.c
endif
ifeq ($(strip $(PLATFORM)), SC6531EFM)
SOURCES         += DSP_simple_pdata_sc6531efm.c
endif
ifeq ($(strip $(PLATFORM)), UWS6121E)
SOURCES         += DSP_simple_pdata_sc6531efm.c
endif
ifeq ($(strip $(RF_TYPE)), QUORUM518)
    ifeq ($(strip $(PLATFORM)), SC6600L)
        ifeq ($(strip $(CHIP_VER)), 6600L7)	
        	SOURCES   		+= DSP_pdata_sc6600L7_quorum518.c
        	SOURCES   		+= DSP_pdata_sc6600L7B_quorum518.c
        	ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
        	    SOURCES   		+= DSP_pdata_sc6600L7A_quorum518_t_build.c
        	else
        			MCFLAG_OPT   	+= -DQUAD_TRI_SIM_DSP
        	    SOURCES   		+= DSP_pdata_sc6600L7A_quorum518_q_build.c
        	endif
        endif
        ifeq ($(strip $(CHIP_VER)), 6600L6)	
        	ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
        	    SOURCES   		+= DSP_pdata_sc6600L6_quorum518_t.c
        	else
        	    SOURCES   		+= DSP_pdata_sc6600L6_quorum518_q.c
        	    SOURCES   		+= DSP_pdata_sc6600L6_quorum518_t.c
        	endif
        endif   
        
        ifeq ($(strip $(CHIP_VER)),6610)
                SOURCES   		+= DSP_pdata_sc6610K_quorum518.c
        endif

    endif
    
    ifeq ($(strip $(PLATFORM)), SC6800H)
        ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
            SOURCES   		+= DSP_pdata_sc6800H_quorum518_t.c
        else
            SOURCES   		+= DSP_pdata_sc6800H_quorum518_q.c
            SOURCES   		+= DSP_pdata_sc6800H_quorum518_t.c
            
        endif
    endif
endif

ifeq ($(strip $(RF_TYPE)), SR528)
    ifeq ($(strip $(PLATFORM)), SC6600L)
        ifeq ($(strip $(CHIP_VER)), 6600L6)
            ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
                SOURCES   		+= DSP_pdata_sc6600L6_quorum528_t.c
            else
                SOURCES   		+= DSP_pdata_sc6600L6_quorum528_q.c
                SOURCES   		+= DSP_pdata_sc6600L6_quorum528_t.c
            endif
        endif
        ifeq ($(strip $(CHIP_VER)), 6600L7)
            ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
                SOURCES   		+= DSP_pdata_sc6600L7A_quorum528_t.c
            else
                SOURCES   		+= DSP_pdata_sc6600L7A_quorum528_q.c
                SOURCES   		+= DSP_pdata_sc6600L7A_quorum528_t.c
            endif
        endif
        ifeq ($(strip $(CHIP_VER)), 6610)
                SOURCES   		+= DSP_pdata_sc6610K_quorum528_build.c
        endif
    endif
    ifeq ($(strip $(PLATFORM)), SC6800H)
            SOURCES         += DSP_pdata_sc6800H3_quorum528.c
        ifeq ($(strip $(LOW_MEMORY_SUPPORT)), 32X16)
        else
   			MCFLAG_OPT   	+= -DQUAD_TRI_SIM_DSP
        endif    
            SOURCES   		+= DSP_pdata_sc6800H_sr528_q_build.c        
    endif
endif

ifeq ($(strip $(DSP_HW_COMPRESS)), TRUE)
ifeq ($(strip $(PLATFORM)), SC6530)
    ifeq ($(strip $(CHIP_DSP)), 6500)
        ifeq ($(strip $(GSM_CUSTOMER_AFP_SUPPORT)), TRUE)
            SOURCES         += DSP_pdata_sc6530C_zip_fdt_samsung.c
        else
            SOURCES         += DSP_pdata_sc6530C_zip_fdt.c
        endif
    else
        ifeq ($(strip $(CHIP_DSP)), 6531)
            SOURCES         += DSP_pdata_sc6531_zip_fdt.c
        else
            ifeq ($(strip $(GSM_CUSTOMER_AFP_SUPPORT)), TRUE)
                SOURCES         += DSP_pdata_sc6530_zip_fdt_samsung.c
            else
                SOURCES         += DSP_pdata_sc6530_zip_fdt.c
            endif
        endif
    endif
    
else
ifeq ($(strip $(PLATFORM)),SC6531EFM)
ifeq ($(strip $(CHIP_SUBVER)),AB) 
     SOURCES   		+= DSP_pdata_sc6531efm_ab_zip_fdt.c
else
     SOURCES   		+= DSP_pdata_sc6531efm_zip_fdt.c
endif
endif
ifeq ($(strip $(PLATFORM)),UWS6121E)
ifeq ($(strip $(CHIP_SUBVER)),AB) 
     SOURCES   		+= DSP_pdata_sc6531efm_ab_zip_fdt.c
else
     SOURCES   		+= DSP_pdata_sc6531efm_zip_fdt.c
endif
endif

endif 
endif



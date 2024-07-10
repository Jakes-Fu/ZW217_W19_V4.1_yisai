
MINCPATH    += CMMB/export/inc CMMB/source/dtl/inc CMMB/source/os_al/include MS_Customize/export/inc
MSRCPATH    = MS_Customize/source/product/config/$(strip $(PRODUCT_CONFIG))
MSRCPATH    += CMMB/source/dtl/src CMMB/source/os_al/source/KD/threadx MS_Customize/source/product/driver/gpio

ifeq ($(strip $(PLATFORM)), SC8800H)
MCFLAG_OPT  += -D_BL_NF_SC8800H_ -DCHIP_DRV_SC8800H -DPRODUCT_DRIVER_SC8800H -DPLATFORM_SC8800H -DPRODUCT_DRIVER_MTV_ENABLE -U_RTOS
endif

SOURCES      = lcmif_drv.c lcm_user.c mtv_cfg.c lcdc_regop.c time.c assert_log.c gpio_mtv.c           

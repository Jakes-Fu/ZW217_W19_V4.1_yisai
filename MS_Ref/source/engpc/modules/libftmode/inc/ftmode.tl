
AT_REG("AT+GETTESTMODE?", ftmode_getTestMode)
AT_REG("AT+SETTESTMODE=", ftmode_setTestMode)
DIAG_REG_FULL(0xFF, 0xFF, 0xFF, 0, ftmode_testmodeRsp, NULL, modeRsp_match)

DIAG_REG(0x0D, 0x02, eng_diag_product_ctrl)
DIAG_REG(0x0D, 0x03, eng_diag_product_ctrl)
DIAG_REG(0x0D, 0x04, eng_diag_product_ctrl)
DIAG_REG(0x0D, 0x12, eng_diag_product_ctrl_ex)
DIAG_REG(0x0D, 0x13, eng_diag_product_ctrl_ex)
DIAG_REG(0x0D, 0x14, eng_diag_product_ctrl_ex)
DIAG_REG_FULL(0x5E, 0xFF, 0xFF, 1, bt_wifi_mac_handler, get_fw_ptr, bt_wifi_mac_match)
DIAG_REG_FULL(0x5F, 0xFF, 0xFF, 0, phasecheck_handler, NULL, phasecheck_match)
DIAG_REG_FULL(0x5D, 0x05, 0x01, 0, ssid_save, NULL, NULL)
DIAG_REG_FULL(0x5D, 0x05, 0x02, 0, fixap_handle, NULL, NULL)


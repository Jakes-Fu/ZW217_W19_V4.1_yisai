#include "ms_customize_trc.h"
#include "i2c_api.h"
#include "tp_multi_cfg.h"
#include "gpio_prod_api.h"
#include "gpio_drv.h"
#include "dal_keypad.h"

#include "bl_fw_update.h"
#include "bl_chip_common.h"
#include "bl_chip_custom.h"
#if defined(DW_USD10A_ZZWL_Z5_S1_DRIVER_CONFIG)
#include "bl_fw_usd10a_zzwl_z5_s1_240.h"
#elif defined(DW_US15A_F31U_DRIVER_CONFIG)
#include "bl_fw_us15a_hm_f31u_128.h"
#elif defined(DW_US16A_HM_GS29_DRIVER_CONFIG )
#include "bl_fw_us16a_hm_gs29_128.h"
#elif defined(DW_US20_ZT_DT01_DRIVER_CONFIG )
#include "bl_fw_us20_zt_dt01_240.h"
#elif defined(DW_US06A_TYC_HY03_DRIVER_CONFIG )
#include "bl_fw_us06a_tyc_hy03_240.h"
#elif defined(DW_US06A_TYC_HY05_DRIVER_CONFIG )
#include "bl_fw_us06a_tyc_hy05_240.h"
#elif defined(DW_US20A_PGD_Q666_DRIVER_CONFIG)
#include "bl_fw_us20a_pgd_s666_240.h"
#elif defined(DW_US28A_SWD_X86_DRIVER_CONFIG)
//#include "bl_fw_us28a_x86_xr0173_240.h"
#include "bl_fw_us28a_x86_xr0173_xsgd_280.h"
#elif defined(DW_US06A_X5_DRIVER_CONFIG)
#include "bl_fw_us06_x5_G1002_240.h"
#elif defined(DW_US10_H12_DRIVER_CONFIG)
#include "bl_fw_us10_h12_tyc_280.h"
#elif defined(DW_US08_DRIVER_CONFIG)
#include "bl_fw_us08_240.h"
#elif defined(DW_US108_DRIVER_CONFIG)
#include "bl_fw_us108_a68d_240.h"
#elif defined(DW_US10_DRIVER_CONFIG)
#include "bl_fw_us10_gs17_240.h"
#elif defined(DW_US16A_AW46_DRIVER_CONFIG)
#include "bl_fw_us16a_aw46_128.h"
#elif defined(DW_US06A_CW1_C_1_DRIVER_CONFIG)
#include "bl_fw_us06a_cw1_128.h"
#elif defined(DW_US06A_ZMD_Y01U_DRIVER_CONFIG)
#include "bl_fw_us06a_zmd_y01u_240.h"
#elif defined(DW_US15_YLL_A4_1_DRIVER_CONFIG)
#include "bl_fw_us15a_yll_a4_240.h"
#elif defined(DW_US15_YLL_A5_B_1_DRIVER_CONFIG)
#include "bl_fw_us15a_yll_a5_b_1_240.h"
#elif defined(DW_US15_YLL_A4_B_1_DRIVER_CONFIG)
#include "bl_fw_us15a_yll_a4_b_1_240.h"
#elif defined(DW_US15_YLL_A3_B_1_DRIVER_CONFIG)
#include "bl_fw_us15a_yll_a3_b_1_240.h"
#elif defined(DW_US15B_YLL_A4B_1A_DRIVER_CONFIG)
#include "bl_fw_us15b_yll_a4b_1a_128.h"
#elif defined(DW_US15B_YLL_A7_1_DRIVER_CONFIG)
#include "bl_fw_us15a_yll_a7_128.h"
#elif defined(DW_US15A_K01_DRIVER_CONFIG)
#include "bl_fw_us15a_k01_128.h"
#elif defined(DW_US15_DRIVER_CONFIG)
#include "bl_fw_us15a_240.h"
#elif defined(DW_US15A_GS18_DRIVER_CONFIG)
#include "bl_fw_us15a_gs18_128.h"
#elif defined(DW_USD10A_P5_DRIVER_CONFIG)
#include "bl_fw_usd10a_p5_240.h"
#elif defined(DW_US10A_ZZ601_DRIVER_CONFIG)
#include "bl_fw_us10_zz601_128.h"
#elif defined(DW_US10A_ZZ603_DRIVER_CONFIG)
//#include "bl_fw_us10_zz603_128.h"
#include "bl_fw_us10_zz603_nq_128.h"
#elif defined(DW_US10A_SXY_C20_DRIVER_CONFIG)
#include "bl_fw_us10_sxy_c20_128.h"
#elif defined(DW_US06A_DRIVER_CONFIG)
#include "bl_fw_us06a_x5_240.h"
#elif defined(DW_US06A_ZZ_X6_1_DRIVER_CONFIG)
#include "bl_fw_us06a_x6_240.h"
#elif defined(DW_US06A_GR_X32_1_DRIVER_CONFIG)
#include "bl_fw_us06a_x32_240.h"
#elif defined(DW_US06A_DNYK_GT3_1_DRIVER_CONFIG)
#include "bl_fw_us06a_dnyk_gt3_240.h"
#elif defined(DW_US06D_ZMD_DF59_DRIVER_CONFIG)
#include "bl_fw_us06d_zmd_df59_240.h"
#elif defined(DW_US20_DRIVER_CONFIG)
#include "bl_fw_us20a_a6_240.h"
#elif defined(DW_US06A_ZZ_GS10_DRIVER_CONFIG)
#include "bl_fw_us06a_gs10_240.h"
#elif defined(DW_US06A_ZZ_GS12_DRIVER_CONFIG)
#include "bl_fw_us06a_gs12_128.h"
#elif defined(DW_US06A_ZZ_GS16_DRIVER_CONFIG)
#include "bl_fw_us06a_gs16_240.h"
#elif defined(DW_US22A_DRIVER_CONFIG)
#include "bl_fw_us22a_v10_240.h"
#elif defined(DW_US20_XH_H02_DRIVER_CONFIG)
#include "bl_fw_us20a_h2_240.h"
#elif defined(DW_US26A_XXY_L1_DRIVER_CONFIG)
#include "bl_fw_us26a_xxy_l1_240.h"
#elif defined(DW_US20A_PGD_S22_DRIVER_CONFIG)
#include "bl_fw_us20a_pgd_s22_240.h"
#elif defined(DW_US20A_PGD_S888_DRIVER_CONFIG)
//#include "bl_fw_us20a_pgd_s888_240.h"
#include "bl_fw_us20a_pgd_s22_240.h"
#elif defined(DW_US20A_MK_T6_DRIVER_CONFIG)
#include "bl_fw_us20a_mk_t6_240.h"
#elif defined(DW_US10A_NY_HZ02_DRIVER_CONFIG)
#include "bl_fw_us10_ny_hz02_240.h"
#elif defined(DW_US20_NY_XCS_DRIVER_CONFIG)
//#include "bl_fw_us20a_ny_240.h"
#include "bl_fw_us20a_WKD_240.h"
#elif defined(DW_USD10A_TYC_HY01_1_DRIVER_CONFIG)
#include "bl_fw_usd10_tyc_240.h"
#elif defined(DW_USD10A_SXY_DW168_A2_DRIVER_CONFIG)
#include "bl_fw_usd10_dw168_a2_240.h"
#elif defined(DW_USD10A_DW168_A2_DRIVER_CONFIG)
//#include "bl_fw_usd10_dw168_a2_240.h"
#include "bl_fw_usd10_dw168_rq_a2_240.h"
#elif defined(W217_W18_DRV_TP_6133_SERIES)
#include "BL6XX3_ZW30-W18-JJ100-Z1_P1F4A2__20230815.h"
// #include "BL6XX3_ZW201-X01- JLD817_P1F4A3__20231108.h"

#else
//#include "bl_fw.h"
#include "BL6133.h"
#endif

#ifdef BL_UPDATE_FIRMWARE_ENABLE
LOCAL I2C_DEV BL6133_I2C_fm_cfg =
{
    4,
    400000,                   // freq is standard for BL6133
    0,                          // bus
    BL_FLASH_I2C_ADDR,  // slave_addr
    1,                          // reg_addr_num
    0,                          // check_ack
    1                           // no_stop
};

LOCAL int32 bl_i2c_handle_firmware = -1;
#endif


#define   SET_WAKEUP_HIGH    bl_ts_set_intup(0)	 // 需要设置GPIO口
#define   SET_WAKEUP_LOW	 bl_ts_set_intup(1)	 // 需要设置GPIO口


#ifdef CTP_USE_HW_I2C

extern int BL6133_I2C_Write(uint8 *addr, uint32 cnt);
extern int BL6133_I2C_Read(uint8 *addr, uint32 cnt);

char CTP_FLASH_I2C_READ(unsigned char *value, unsigned short len)
{
	return I2C_HAL_Read_Byte(bl_i2c_handle_firmware, value,len);
}

char CTP_FLASH_I2C_WRITE(unsigned char *value, unsigned short len)
{
	return I2C_HAL_Write_Byte(bl_i2c_handle_firmware,value, len);
}

static int bl_flash_i2c_transfer(unsigned char *buf, int len,unsigned char rw)
{
	int ret = 0;
    switch(rw)
    {
        case I2C_WRITE:
			ret = CTP_FLASH_I2C_WRITE(buf, len);
			break;
	case I2C_READ:
			ret = CTP_FLASH_I2C_READ(buf, len);
			break;
    }
	if(ret<0){
		bl_log_trace("bl_flash_i2c_transfer error\n");
		return -1;
	}

	return 0;
}

PUBLIC int bl_read_fw(unsigned char *reg_addr, unsigned char *buf, int len)
{
	int ret;

	ret = BL6133_I2C_Write(reg_addr, 1);
	if(ret<0)
       {
      		bl_log_trace("bl_read_fw:i2c write error\n");
		return SCI_FALSE;
       }
	ret = BL6133_I2C_Read(buf, len);
	if(ret<0)
       {
      		bl_log_trace("bl_read_fw:i2c read error\n");
		return SCI_FALSE;
       }
	return SCI_TRUE;
}
#endif

int bl_soft_reset_switch_int_wakemode()
{
	unsigned char cmd[4];
	int ret = 0x00;

	cmd[0] = RW_REGISTER_CMD;
	cmd[1] = ~cmd[0];
	cmd[2] = CHIP_ID_REG;
	cmd[3] = 0xe8;

	ret = BL6133_I2C_Write(cmd,4);
	if(ret < 0){
	bl_log_trace("bl_soft_reset_switch_int_wakemode failed:i2c write flash error\n");
	}

	OS_TickDelay(50);

	return ret;
}

int bl_get_chip_id(unsigned char *buf)
{

	unsigned char cmd[3];
	int ret = 0x00;

	bl_log_trace("bl_get_chip_id\n");

	SET_WAKEUP_LOW;
	OS_TickDelay(50);
	bl_soft_reset_switch_int_wakemode();

	bl_i2c_handle_firmware  = I2C_HAL_Open(&BL6133_I2C_fm_cfg);
	if(bl_i2c_handle_firmware != -1 )
	{
		cmd[0] = RW_REGISTER_CMD;
		cmd[1] = ~cmd[0];
		cmd[2] = CHIP_ID_REG;

		ret = bl_flash_i2c_transfer(cmd,3,I2C_WRITE);
		if(ret < 0){
			bl_log_trace("bl_get_chip_id:i2c write flash error\n");
			return SCI_FALSE;
		}

		ret = bl_flash_i2c_transfer(buf,1,I2C_READ);
		if(ret < 0){
			bl_log_trace("bl_get_chip_id:i2c read flash error\n");
			return SCI_FALSE;
	}

	return SCI_TRUE;
	}
	else
	{
		bl_log_trace("i2c_handle_tp_firmware open error!!\n");
		return SCI_FALSE;
	}

}

int bl_get_fwArgPrj_id(unsigned char *buf)
{
	unsigned char cmd[1];
	cmd[0] = BL_FWVER_PJ_ID_REG;
	bl_log_trace("bl_get_fwArgPrj_id\n");
	return bl_read_fw(cmd, buf, 3);
}

#ifdef BL_UPDATE_FIRMWARE_ENABLE
static unsigned char bl_check_cobID(unsigned char *fw_data, unsigned char* cobID)
{
    unsigned char convertCobId[12] = {0x00};
	unsigned char i = 0;

	bl_log_trace("bl_check_cobID start\n");
    for(i = 0; i < sizeof(convertCobId); i++)
    {
    	if(i%2)
    	{
    		convertCobId[i] = cobID[i/2] & 0x0f;
    	}
    	else
    	{
    		convertCobId[i] = (cobID[i/2] & 0xf0) >> 4;
    	}
    	bl_log_trace("before convert:convertCobId[%d] is %x\n",i,convertCobId[i]);
    	if(convertCobId[i] < 10)
    	{
    		convertCobId[i] = '0' + convertCobId[i];
    	}
    	else
    	{
    		convertCobId[i] = 'a' + convertCobId[i] - 10;
    	}
    	bl_log_trace("after convert:convertCobId[%d] is %x\n",i,convertCobId[i]);
    }

    if(memcmp(convertCobId, fw_data + BL_ARGUMENT_BASE_OFFSET + BL_COB_ID_OFFSET, 12))
    {
        bl_log_trace("check cobID incompatible\n");
		return 0;
    }
	else
    {
        bl_log_trace("check cobID same\n");
		return 1;
    }
}

static int bl_get_specific_argument(unsigned int *arguOffset, unsigned char *cobID, unsigned char* fw_data, unsigned int fw_size, unsigned char arguCount)
{
    unsigned char convertCobId[12] = {0x00};
    unsigned char i = 0;
    unsigned int cobArguAddr = fw_size - arguCount * BL_ARGUMENT_FLASH_SIZE;
    bl_log_trace("fw_size is %x\n", fw_size);
    bl_log_trace("arguCount is %d\n", arguCount);
    bl_log_trace("cobArguAddr is %x\n",cobArguAddr);

    for(i = 0; i < sizeof(convertCobId); i++)
    {
        if(i%2)
        {
            convertCobId[i] = cobID[i/2] & 0x0f;
        }
        else
        {
            convertCobId[i] = (cobID[i/2] & 0xf0) >> 4;
        }
        bl_log_trace("before convert:convertCobId[%d] is %x\n",i,convertCobId[i]);
        if(convertCobId[i] < 10)
        {
            convertCobId[i] = '0' + convertCobId[i];
        }
        else
        {
            convertCobId[i] = 'a' + convertCobId[i] - 10;
        }
        bl_log_trace("after convert:convertCobId[%d] is %x\n",i,convertCobId[i]);
    }

    bl_log_trace("convertCobId is:\n");
    for(i= 0; i < 12; i++)
    {
        bl_log_trace("%x  ", convertCobId[i]);
    }
    bl_log_trace("\n");

    for(i = 0; i < arguCount; i++)
    {
        if(memcmp(convertCobId, fw_data + cobArguAddr + i * BL_ARGUMENT_FLASH_SIZE + BL_COB_ID_OFFSET, 12))
        {
            bl_log_trace("This argu is not the specific argu\n");
        }
        else
        {
            *arguOffset = cobArguAddr + i * BL_ARGUMENT_FLASH_SIZE;
            bl_log_trace("This argu is the specific argu, and arguOffset is %x\n",*arguOffset);
            break;
        }
    }

	if(i == arguCount)
    {
        *arguOffset = BL_ARGUMENT_BASE_OFFSET;
        return -1;
    }
	else
    {
        return 0;
    }
}

static unsigned char bl_get_argument_count(unsigned char* fw_data, unsigned int fw_size)
{
    unsigned char i = 0;
	unsigned addr = 0;
	addr = fw_size;
	bl_log_trace("addr is %x\n", addr);
	while(addr > (BL_ARGUMENT_BASE_OFFSET + BL_ARGUMENT_FLASH_SIZE))
    {
        addr = addr - BL_ARGUMENT_FLASH_SIZE;
        if(memcmp(fw_data+addr, ARGU_MARK, sizeof(ARGU_MARK) - 1))
        {
            bl_log_trace("arguMark found flow complete");
            break;
        }
        else
        {
            i++;
            bl_log_trace("arguMark founded\n");
        }
    }
    bl_log_trace("The argument count is %d\n",i);
    return i;
}

static unsigned int bl_get_cob_project_down_size_arguCnt(unsigned char* fw_data,unsigned int fw_size, unsigned char *arguCnt)
{
    unsigned int downSize = 0;

	*arguCnt = bl_get_argument_count(fw_data,fw_size);
	downSize = fw_size - (*arguCnt) * BL_ARGUMENT_FLASH_SIZE - FLASH_PAGE_SIZE;
	return downSize;
}

static unsigned char bl_is_cob_project(unsigned char* fw_data, int fw_size)
{
    unsigned char arguKey[4] = {0xaa,0x55,0x09,0x09};
	unsigned char* pfw;

	pfw = fw_data+fw_size-4;

    if(fw_size%FLASH_PAGE_SIZE)
    {
        return 0;
    }
	else
    {
	    if(memcmp(arguKey,pfw,4))
	    {
	        return 0;
	    }
        else
        {
            return 1;
        }
    }
}

static int bl_get_cob_id(unsigned char *buf)
{
	unsigned char cmd[1];
	cmd[0] = COB_ID_REG;
	bl_log_trace("bl_get_cob_id\n");
	return bl_read_fw(cmd, buf, 6);
}

static int bl_check_prj_info(unsigned char *fw_data, unsigned char prjType, int specifyArgAddr)
{
	char ret = 0;
    	unsigned char prj_info[PRJ_INFO_LEN]={0};
	unsigned char cmd[1];
	cmd[0] = BL_PRJ_INFO_REG;

    ret = bl_read_fw(cmd, prj_info, PRJ_INFO_LEN);
	if(ret < 0)
	{
		bl_log_trace("Read project info error\n");
		return -1;
	}

	if(!prjType)
    {
        ret = memcmp(fw_data + specifyArgAddr + BL_PROJECT_INFO_OFFSET,prj_info,PRJ_INFO_LEN);
    	if(ret == 0)
    	{
    		bl_log_trace("Check project info success\n");
    		return 0;
    	}
    	else
    	{
    		bl_log_trace("Check project info fail\n");
    		return -1;
    	}
    }
	else
    {
        bl_log_trace("COB project do not check\n");
		return 0;
    }
}

static int bl_get_fw_checksum(unsigned short *fw_checksum)
{
	unsigned char buf[3];
	unsigned char checksum_ready = 0;
	int retry = 5;
	int ret = 0x00;
	unsigned char cmd[1];
	cmd[0] = CHECKSUM_REG;

	bl_log_trace("bl_get_fw_checksum\n");

	buf[0] = CHECKSUM_CAL_REG;
	buf[1] = CHECKSUM_CAL;
	ret = BL6133_I2C_Write(buf,2);
	if(ret < 0){
		bl_log_trace("bl_get_fw_checksum:write checksum cmd error\n");
		return -1;
	}
	OS_TickDelay(FW_CHECKSUM_DELAY_TIME);

	ret = bl_read_fw(cmd, buf, 3);
	if(ret < 0){
		bl_log_trace("bl_get_fw_checksum:read checksum error\n");
		return -1;
	}

	checksum_ready = buf[0];

	while((retry--) && (checksum_ready != CHECKSUM_READY)){

		OS_TickDelay(50);
		ret = bl_read_fw(cmd, buf, 3);
		if(ret < 0){
			bl_log_trace("bl_get_fw_checksum:read checksum error\n");
			return -1;
		}

		checksum_ready = buf[0];
	}

	if(checksum_ready != CHECKSUM_READY){
		bl_log_trace("bl_get_fw_checksum:read checksum fail\n");
		return -1;
	}
	*fw_checksum = (buf[1]<<8)+buf[2];

	return 0;
}

static void bl_get_fw_bin_checksum(unsigned char *fw_data,unsigned short *fw_bin_checksum, int fw_size, int specifyArgAddr)
{
	int i = 0;
	int temp_checksum = 0x0;


		    for(i = 0; i < BL_ARGUMENT_BASE_OFFSET; i++)
			{
                temp_checksum += fw_data[i];
			}
			for(i = specifyArgAddr; i < specifyArgAddr + BL_ARGUMENT_FLASH_SIZE; i++)
			{
				temp_checksum += fw_data[i];
			}
			for(i = BL_ARGUMENT_BASE_OFFSET + BL_ARGUMENT_FLASH_SIZE; i < fw_size; i++)
			{
				temp_checksum += fw_data[i];
			}
			for(i = fw_size; i < MAX_FLASH_SIZE; i++)
			{
				temp_checksum += 0xff;
			}

			*fw_bin_checksum = temp_checksum & 0xffff;

}

static int bl_erase_flash(void)
{
	unsigned char cmd[2];

	bl_log_trace("bl_erase_flash\n");

	cmd[0] = ERASE_ALL_MAIN_CMD;
	cmd[1] = ~cmd[0];

	return bl_flash_i2c_transfer(cmd, 0x02,I2C_WRITE);
}

static int bl_write_flash(unsigned char cmd, int flash_start_addr, unsigned char *buf, int len)
{
	unsigned char cmd_buf[6+FLASH_WSIZE];
	unsigned short flash_end_addr;
	int ret;

	bl_log_trace("bl_write_flash\n");

	if(!len){
		bl_log_trace("write flash len is 0x00,return\n");
		return -1;
	}

	flash_end_addr = flash_start_addr + len -1;

	if(flash_end_addr >= MAX_FLASH_SIZE){
		bl_log_trace("write flash end addr is overflow,return\n");
		return -1;
	}

	cmd_buf[0] = cmd;
	cmd_buf[1] = ~cmd;
	cmd_buf[2] = flash_start_addr >> 0x08;
	cmd_buf[3] = flash_start_addr & 0xff;
	cmd_buf[4] = flash_end_addr >> 0x08;
	cmd_buf[5] = flash_end_addr & 0xff;

	memcpy(&cmd_buf[6],buf,len);

	ret = bl_flash_i2c_transfer(cmd_buf, len+6,I2C_WRITE);
	if(ret < 0){
		bl_log_trace("i2c transfer error\n");
		return -1;
	}

	return 0;
}

static int bl_read_flash(unsigned char cmd, int flash_start_addr, unsigned char *buf, int len)
{
	char ret =0;
	unsigned char cmd_buf[6];
	unsigned short flash_end_addr;

	flash_end_addr = flash_start_addr + len -1;
	cmd_buf[0] = cmd;
	cmd_buf[1] = ~cmd;
	cmd_buf[2] = flash_start_addr >> 0x08;
	cmd_buf[3] = flash_start_addr & 0xff;
	cmd_buf[4] = flash_end_addr >> 0x08;
	cmd_buf[5] = flash_end_addr & 0xff;
	ret = bl_flash_i2c_transfer(cmd_buf,6,I2C_WRITE);
	if(ret < 0)
	{
	    bl_log_trace("bl_read_flash:i2c transfer write error\n");
		return -1;
	}
	ret = bl_flash_i2c_transfer(buf,len,I2C_READ);
	if(ret < 0)
	{
	    bl_log_trace("bl_read_flash:i2c transfer read error\n");
		return -1;
	}

	return 0;
}

static int bl_download_fw(unsigned char *pfwbin,int specificArgAddr, int fwsize)
{
	unsigned int i;
	unsigned short size,len;
	unsigned short addr;
	unsigned char verifyBuf[4] = {0xff, 0xff, 0x09, 0x09};
	bl_log_trace("bl_download_fw\n");
	if(bl_erase_flash()){
		bl_log_trace("erase flash fail\n");
		return -1;
	}

	OS_TickDelay(50);

    //Write data before BL_ARGUMENT_BASE_OFFSET
	for(i=0;i< BL_ARGUMENT_BASE_OFFSET;)
	{
		size = BL_ARGUMENT_BASE_OFFSET - i;
		if(size > FLASH_WSIZE){
			len = FLASH_WSIZE;
		}else{
			len = size;
		}

		addr = i;

		if(bl_write_flash(WRITE_MAIN_CMD,addr, &pfwbin[i],len)){
			return -1;
		}
		i += len;
		OS_TickDelay(5);
	}

    //Write the data from BL_ARGUMENT_BASE_OFFSET to VERTIFY_START_OFFSET
    for(i=BL_ARGUMENT_BASE_OFFSET;i< (VERTIFY_START_OFFSET+BL_ARGUMENT_BASE_OFFSET);)
    {
    	size = VERTIFY_START_OFFSET + BL_ARGUMENT_BASE_OFFSET - i;
    	if(size > FLASH_WSIZE){
    		len = FLASH_WSIZE;
    	}else{
    		len = size;
    	}

    	addr = i;

    	if(bl_write_flash(WRITE_MAIN_CMD,addr, &pfwbin[i+specificArgAddr-BL_ARGUMENT_BASE_OFFSET],len)){
    		return -1;
    	}
    	i += len;
		OS_TickDelay(5);
    }

    //Write the four bytes verifyBuf from VERTIFY_START_OFFSET
    for(i=(VERTIFY_START_OFFSET + BL_ARGUMENT_BASE_OFFSET);i< (VERTIFY_START_OFFSET + BL_ARGUMENT_BASE_OFFSET + sizeof(verifyBuf));)
    {
    	size = VERTIFY_START_OFFSET + BL_ARGUMENT_BASE_OFFSET + sizeof(verifyBuf) - i;
    	if(size > FLASH_WSIZE){
    		len = FLASH_WSIZE;
    	}else{
    		len = size;
    	}

    	addr = i;

    	if(bl_write_flash(WRITE_MAIN_CMD,addr, &verifyBuf[i-VERTIFY_START_OFFSET-BL_ARGUMENT_BASE_OFFSET],len)){
    		return -1;
    	}
    	i += len;
    	OS_TickDelay(5);
    }

    //Write data after verityBuf from VERTIFY_START_OFFSET + 4
    for(i=(BL_ARGUMENT_BASE_OFFSET + BL_ARGUMENT_FLASH_SIZE);i< fwsize;)
    {
    	size = fwsize - i;
    	if(size > FLASH_WSIZE){
    		len = FLASH_WSIZE;
    	}else{
    		len = size;
    	}

    	addr = i;

    	if(bl_write_flash(WRITE_MAIN_CMD,addr, &pfwbin[i],len)){
    		return -1;
    	}
    	i += len;
    	OS_TickDelay(5);
    }

	return 0;
}


static int bl_read_flash_vertify(void)
{
	unsigned char cnt = 0;
	int ret = 0;
	unsigned char vertify[2] = {0xAA, 0x55};
	unsigned char vertify1[2] = {0};
	SET_WAKEUP_LOW;
	OS_TickDelay(50);
	bl_soft_reset_switch_int_wakemode();
	while(cnt < 3)
	{
        ret = bl_read_flash(READ_MAIN_CMD, BL_ARGUMENT_BASE_OFFSET + VERTIFY_START_OFFSET, vertify1, 2);
		if(ret < 0)
		{
			bl_log_trace("bl_write_flash_vertify: read fail\n");
			continue;
		}

		if(memcmp(vertify, vertify1, 2) == 0)
		{
			ret = 0;
			break;
		}
		else
        {
            ret = -1;
        }
		cnt++;
	}
	SET_WAKEUP_HIGH;
	OS_TickDelay(20);
	return ret;
}

static int bl_write_flash_vertify(void)
{
	unsigned char cnt = 0;
	int ret = 0;
	unsigned char vertify[2] = {0xAA, 0x55};
	unsigned char vertify1[2] = {0};
	SET_WAKEUP_LOW;
	OS_TickDelay(50);
	bl_soft_reset_switch_int_wakemode();
	while(cnt < 3)
	{
	    ret = bl_write_flash(WRITE_MAIN_CMD, BL_ARGUMENT_BASE_OFFSET + VERTIFY_START_OFFSET, vertify, 2);
		if(ret < 0)
		{
			bl_log_trace("bl_write_flash_vertify: write fail\n");
			continue;
		}

		OS_TickDelay(10);

        ret = bl_read_flash(READ_MAIN_CMD, BL_ARGUMENT_BASE_OFFSET + VERTIFY_START_OFFSET, vertify1, 2);
		if(ret < 0)
		{
			bl_log_trace("bl_write_flash_vertify: read fail\n");
			continue;
		}

		if(memcmp(vertify, vertify1, 2) == 0)
		{
			ret = 0;
			break;
		}
		else
        {
            ret = -1;
        }
		cnt++;
	}
	SET_WAKEUP_HIGH;
	OS_TickDelay(20);
	return ret;
}

static int bl_update_flash(unsigned char update_type, unsigned char *pfwbin,int fwsize, int specificArgAddr)
{
	int retry = 0;
	int ret = 0;
	unsigned short fw_checksum = 0x0;
	unsigned short fw_bin_checksum = 0x0;
	unsigned short lk_checksum = 0x0;
	unsigned short lk_bin_checksum = 0x0;
	retry =3;
	while(retry--)
	{
		SET_WAKEUP_LOW;
		OS_TickDelay(50);
		bl_soft_reset_switch_int_wakemode();

			    ret = bl_download_fw(pfwbin,specificArgAddr,fwsize);

		if(ret<0)
		{
			bl_log_trace("btl fw update start bl_download_fw error retry=%d\n",retry);
			continue;
		}

		OS_TickDelay(50);

		SET_WAKEUP_HIGH;

		OS_TickDelay(200);

		bl_ts_reset_wakeup();
	      OS_TickDelay(50);
			    bl_get_fw_bin_checksum(pfwbin,&fw_bin_checksum, fwsize,specificArgAddr);
			    ret = bl_get_fw_checksum(&fw_checksum);
				fw_checksum -= 0xff;
				bl_log_trace("btl fw update end,fw checksum = 0x%x,fw_bin_checksum =0x%x\n",fw_checksum, fw_bin_checksum);


		if((ret < 0) || ((update_type == FW_ARG_UPDATE)&&(fw_checksum != fw_bin_checksum)))
		{
			bl_log_trace("btl fw update start bl_download_fw bl_get_fw_checksum error");
			continue;
		}

		if((update_type == FW_ARG_UPDATE)&&(fw_checksum == fw_bin_checksum))
		{
            ret = bl_write_flash_vertify();
			if(ret < 0)
				continue;
		}
		break;
	}

	if(retry < 0)
	{
		bl_log_trace("btl fw update error\n");
		return -1;
	}

	bl_log_trace("btl fw update success\n");

	return 0;
}

static unsigned char choose_update_type_for_self_ctp(unsigned char isBlank, unsigned char* fw_data, unsigned char fwVer, unsigned char arguVer, unsigned short fwChecksum, unsigned short fwBinChecksum,int specifyArgAddr)
{
    unsigned char update_type = NONE_UPDATE;
    if(isBlank)
	{
		update_type = FW_ARG_UPDATE;
		bl_log_trace("Update case 0:FW_ARG_UPDATE\n");
	}
	else
	{
        if((fwVer != fw_data[specifyArgAddr + BL_FWVER_MAIN_OFFSET])
		    ||(arguVer != fw_data[specifyArgAddr + BL_FWVER_ARGU_OFFSET])
		    ||(fwChecksum != fwBinChecksum))
	    {
            update_type = FW_ARG_UPDATE;
		    bl_log_trace("Update case 1:FW_ARG_UPDATE\n");
	    }
	    else
	    {
            update_type = NONE_UPDATE;
                    	bl_log_trace("Update case 4:NONE_UPDATE\n");
	    }
	}
    return update_type;
}

static int bl_update_fw_for_self_ctp(unsigned char* fw_data, int fw_size)
{
	unsigned char fwArgPrjID[3];    //firmware version/argument version/project identification
	unsigned char chip_id = 0x00;   //The IC identification
	int ret = 0x00;
	unsigned char isBlank = 0x0;    //Indicate the IC have any firmware
	unsigned short fw_checksum = 0x0;  //The checksum for firmware in IC
	unsigned short fw_bin_checksum = 0x0;  //The checksum for firmware in file
	unsigned char update_type = NONE_UPDATE;
	unsigned int downSize = 0x0;       //The available size of firmware data in file
	unsigned char cobID[6] = {0};           //The identification for COB project
	unsigned int specificArguAddr = BL_ARGUMENT_BASE_OFFSET;   //The specific argument base address in firmware date with cobID
	unsigned char arguCount = 0x0;      //The argument count for COB firmware
	unsigned char IsCobPrj = 0;         //Judge the project type depend firmware file
    bl_log_trace("bl_update_fw_for_self_ctp start\n");

//Check chipID
#ifdef BTL_CHECK_CHIPID
	ret = bl_get_chip_id(&chip_id);

	if(ret < 0 || chip_id != BTL_FLASH_ID)
	{
		bl_log_trace("bl_update_fw_for_self_ctp:chip_id = %d", chip_id);
		return -1;
	}
	bl_log_trace("bl_update_fw_for_self_ctp:chip_id = %x", chip_id);
#endif

//Step 1:Obtain project type
    IsCobPrj = bl_is_cob_project(fw_data, fw_size);
    bl_log_trace("bl_update_fw_for_self_ctp:IsCobPrj = %x", IsCobPrj);
	SET_WAKEUP_HIGH;
//Step 2:Obtain IC version number
	OS_TickDelay(5);
	ret = bl_get_fwArgPrj_id(fwArgPrjID);
	if((ret < 0)
		|| ((ret == 0) && (fwArgPrjID[0] == 0))
		|| ((ret == 0) && (fwArgPrjID[0] == 0xff))
		|| ((ret == 0) && (fwArgPrjID[0] == BL_FWVER_PJ_ID_REG))
		|| (bl_read_flash_vertify() < 0))
	{
	    isBlank = 1;
		bl_log_trace("bl_update_fw_for_self_ctp:This is blank IC ret = %x fwArgPrjID[0]=%x\n",ret,fwArgPrjID[0]);
    }
	else
    {
        isBlank = 0;
		bl_log_trace("bl_update_fw_for_self_ctp:ret=%d fwID=%d argID=%d prjID=%d\n",ret,fwArgPrjID[0],fwArgPrjID[1], fwArgPrjID[2]);
    }
    bl_log_trace("bl_update_fw_for_self_ctp:isBlank = %x\n",isBlank);
	isBlank = 1;//Force update firmware
//Step 3:Specify download size
    if(IsCobPrj)
    {
    	downSize = bl_get_cob_project_down_size_arguCnt(fw_data,fw_size,&arguCount);
    	bl_log_trace("bl_update_fw_for_self_ctp:downSize = %x,arguCount = %x\n",downSize,arguCount);
    }
    else
    {
    	downSize = fw_size;
    	bl_log_trace("bl_update_fw_for_self_ctp:downSize = %x\n",downSize);
    }

UPDATE_SECOND_FOR_COB:
//Step 4:Update the fwArgPrjID
    if(!isBlank)
    {
        bl_get_fwArgPrj_id(fwArgPrjID);
    }

//Step 5:Specify the argument data for cob project
    if(IsCobPrj && !isBlank)
    {
    	 OS_TickDelay(100);
        ret = bl_get_cob_id(cobID);
		if(ret < 0)
        {
            bl_log_trace("bl_update_fw_for_self_ctp:bl_get_cob_id error\n");
			ret = -1;
			return SCI_FALSE;
        }
		else
		{
		    bl_log_trace("bl_update_fw_for_self_ctp:cobID = %x %x %x %x %x %x\n",cobID[0],cobID[1],cobID[2],cobID[3],cobID[4],cobID[5]);
		}
		ret = bl_get_specific_argument(&specificArguAddr,cobID,fw_data,fw_size,arguCount);
		if(ret < 0)
		{
            bl_log_trace("Can't found argument for CTP module,use default argu:\n");
		}
		bl_log_trace("bl_update_fw_for_self_ctp:specificArguAddr = %x\n",specificArguAddr);
       }

	bl_log_trace("fw_data[] = %x  fw_data[] = %x", fw_data[BL_ARGUMENT_BASE_OFFSET + VERTIFY_START_OFFSET],fw_data[BL_ARGUMENT_BASE_OFFSET + VERTIFY_END_OFFSET]);

//Step 6:Obtain IC firmware checksum when the version number is same between IC and host firmware
    bl_log_trace("isBlank = %d  ver1 = %d ver2 = %d binVer1 = %d binVer2 = %d specificAddr = %x", isBlank,fwArgPrjID[0],fwArgPrjID[1],fw_data[specificArguAddr+BL_FWVER_MAIN_OFFSET],fw_data[specificArguAddr + BL_FWVER_ARGU_OFFSET], specificArguAddr);
    if(!isBlank)
	{
		bl_get_fw_bin_checksum(fw_data, &fw_bin_checksum, downSize, specificArguAddr);
		ret = bl_get_fw_checksum(&fw_checksum);
		if((ret < 0) || (fw_checksum != fw_bin_checksum)){
			bl_log_trace("bl_update_fw_for_self_ctp:Read checksum fail fw_checksum = %x\n",fw_checksum);
			fw_checksum = 0x00;
		}
		bl_log_trace("bl_update_fw_for_self_ctp:fw_checksum = 0x%x,fw_bin_checksum = 0x%x\n",fw_checksum, fw_bin_checksum);
    }

//Step 7:Select update fw+arg or only update arg
    update_type = choose_update_type_for_self_ctp(isBlank, fw_data, fwArgPrjID[0], fwArgPrjID[1], fw_checksum, fw_bin_checksum,specificArguAddr);

//Step 8:Start Update depend condition
    if(update_type != NONE_UPDATE)
    {
        ret = bl_update_flash(update_type, fw_data, downSize,specificArguAddr);
        if(ret < 0)
        {
            bl_log_trace("bl_update_fw_for_self_ctp:bl_update_flash failed\n");
			return SCI_FALSE;
        }
    }

//Step 9:Execute second update flow when project firmware is cob and last update_type is FW_ARG_UPDATE
    if((ret == 0)&&(IsCobPrj)&&(isBlank))
    {
        isBlank = 0;
        bl_log_trace("bl_update_fw_for_self_ctp:bl_update_flash for COB project need second update with blank IC:isBlank = %d\n",isBlank);
        goto UPDATE_SECOND_FOR_COB;
    }
	bl_log_trace("bl_update_fw_for_self_ctp exit\n");


}

static int bl_update_fw(unsigned char* pFwData, unsigned int fwLen)
{
    int ret = 0;
	ret = bl_update_fw_for_self_ctp(pFwData,fwLen);
	return ret;
}
#ifdef BL_AUTO_UPDATE_FARMWARE
int bl_auto_update_fw(void)
{
	int ret = 0;
    unsigned int fwLen = sizeof(fwbin);

	bl_log_trace("bl_auto_update_fw:fwLen = %x\n",fwLen);
	ret = bl_update_fw((unsigned char *)fwbin, fwLen);
	//I2C_HAL_Close(bl_i2c_handle_firmware);
	//bl_i2c_handle_firmware = -1;
	if(ret < 0)
	{
		bl_log_trace("bl_auto_update_fw: bl_update_fw fail\n");
	}
	else
	{
		bl_log_trace("bl_auto_update_fw: bl_update_fw success\n");
	}

	return ret;
}
#endif

#endif

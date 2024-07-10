

	


/******************************************************************************
**
**	(A) Gpio Define
**
*******************************************************************************/
// step 1: configure NMI600 power pin, and reset pin


// step 2: configure NMI600 i2c bus gpio pin
// note: when nmi600 power off, nmi600 iic sda and scl will be set to input mode. 
//           so if other devices share iic with nmi600, maybe i2c needs initilize again. 

#define NMI_SCL_PIN                 68   //gpio24
#define NMI_SDA_PIN                69   //gpio25



/******************************************************************************
**
**	(B) Analog TV Define
**
*******************************************************************************/



/******************************************************************************
**
**	 (C) FM Define
**
*******************************************************************************/
// step 1: configure NMI600 FM start freq, end freq
#define FM_MIN_FRQ   875  //87.5M hz
#define FM_MAX_FRQ  1080 //108.0Mhz

#define FM_NMI_OFFSET  1//first channel ID

#define FM_RSSI_LENGTH  (FM_MAX_FRQ - FM_MIN_FRQ + 1)




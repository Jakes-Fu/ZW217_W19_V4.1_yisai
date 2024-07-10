/* Change History */
/* 2016-9-15 v0p1 */
/* Initial release by Jason.Nan */

/*#define RF_FM_OTHERS_CTRL0_value	(0x7000)*/

/* 2016-9-20 v0p2 */
/* Update reference buffer & ADC related settings */


/*#define RF_FM_OTHERS_CTRL0_value	(0x7800)*/
#define RF_FM_OTHERS_CTRL1_value	(0x8120)
/*#define FM_ADC_CTRL0_value	(0x5450)*/
/*#define FM_ADC_CTRL1_value	(0x5400)*/

/* 2016-9-23 v0p3 */
/* Update according updated register map*/

#define REG_RF_FM_OTHERS_CTRL0_value	(0x7000)
#define RF_FM_OTHERS_CTRL0_value	(0x8000)
#define FM_RX_RF_CTRL3_value   (0x1200)

/* 2016-9-26 v0p4 */
/* Update FM ADC register map*/
#define FM_ADC_CTRL0_value	(0x5540)
#define FM_ADC_CTRL1_value	(0x5800)

/* 2016-10-31 v0p5 */
/* Update FM block filter register map*/
#define FM_RX_RF_CTRL0_value	(0x0000)

/* 2017-1-4 v0p6 */
/* Update top 26M default value for NemoG AB*/
#define RF_FM_OTHERS_CTRL1_value_AB	(0x8020)


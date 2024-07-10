/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/


#ifndef _CAMERA_H_
#define _CAMERA_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'camera'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
#define FIFORAM_SIZE                            (80)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CAMERA_T
// -----------------------------------------------------------------------------
///
// =============================================================================
#define REG_CAMERA_BASE            0x04404000

typedef volatile struct
{
    REG32                          CTRL;                         //0x00000000
    REG32                          STATUS;                       //0x00000004
    REG32                          DATA;                         //0x00000008
    REG32                          IRQ_MASK;                     //0x0000000C
    REG32                          IRQ_CLEAR;                    //0x00000010
    REG32                          IRQ_CAUSE;                    //0x00000014
    REG32                          CMD_SET;                      //0x00000018
    REG32                          CMD_CLR;                      //0x0000001C
    REG32                          DSTWINCOL;                    //0x00000020
    REG32                          DSTWINRow;                    //0x00000024
    REG32                          SCL_CONFIG;                   //0x00000028
    REG32                          SPI_CAMERA_REG0;              //0x0000002C
    REG32                          SPI_CAMERA_REG1;              //0x00000030
    REG32                          SPI_CAMERA_REG2;              //0x00000034
    REG32                          SPI_CAMERA_REG3;              //0x00000038
    REG32                          SPI_CAMERA_REG4;              //0x0000003C
    REG32                          SPI_CAMERA_REG5;              //0x00000040
    REG32                          SPI_CAMERA_REG6;              //0x00000044
    REG32                          SPI_CAMERA_OBS0;              //0x00000048
    REG32                          SPI_CAMERA_OBS1;              //0x0000004C
    REG32                          CSI_CONFIG_REG0;              //0x00000050
    REG32                          CSI_CONFIG_REG1;              //0x00000054
    REG32                          CSI_CONFIG_REG2;              //0x00000058
    REG32                          CSI_CONFIG_REG3;              //0x0000005C
    REG32                          CSI_CONFIG_REG4;              //0x00000060
    REG32                          CSI_CONFIG_REG5;              //0x00000064
    REG32                          CSI_CONFIG_REG6;              //0x00000068
    REG32                          CSI_CONFIG_REG7;              //0x0000006C
    REG32                          CSI_OBS4;                     //0x00000070
    REG32                          CSI_OBS5;                     //0x00000074
    REG32                          CSI_OBS6;                     //0x00000078
    REG32                          CSI_OBS7;                     //0x0000007C
    REG32                          CSI_ENABLE;                   //0x00000080
    REG32                          CSI_CONFIG_REG8;              //0x00000084
    REG32 Reserved_00000088[222];               //0x00000088
    struct
    {
        REG32                      RAMData;                      //0x00000400
    } FIFORAM[FIFORAM_SIZE];
    REG32 Reserved_00000540[176];               //0x00000540
    REG32                          soft_reset;                   //0x00000800
    REG32 Reserved_00000804[17];                //0x00000804
    REG32                          awb_x1_min;                   //0x00000848
    REG32                          awb_x1_max;                   //0x0000084C
    REG32                          awb_y1_min;                   //0x00000850
    REG32                          awb_y1_max;                   //0x00000854
    REG32                          awb_x2_min;                   //0x00000858
    REG32                          awb_x2_max;                   //0x0000085C
    REG32                          awb_y2_min;                   //0x00000860
    REG32                          awb_y2_max;                   //0x00000864
    REG32                          awb_x3_min;                   //0x00000868
    REG32                          awb_x3_max;                   //0x0000086C
    REG32                          awb_y3_min;                   //0x00000870
    REG32                          awb_y3_max;                   //0x00000874
    REG32                          awb_x4_min;                   //0x00000878
    REG32                          awb_x4_max;                   //0x0000087C
    REG32                          awb_y4_min;                   //0x00000880
    REG32                          awb_y4_max;                   //0x00000884
    REG32                          awb_x5_min;                   //0x00000888
    REG32                          awb_x5_max;                   //0x0000088C
    REG32                          awb_y5_min;                   //0x00000890
    REG32                          awb_y5_max;                   //0x00000894
    REG32                          awb_skin_x1_min;              //0x00000898
    REG32                          awb_skin_x1_max;              //0x0000089C
    REG32                          awb_skin_y1_min;              //0x000008A0
    REG32                          awb_skin_y1_max;              //0x000008A4
    REG32                          awb_skin_x2_min;              //0x000008A8
    REG32                          awb_skin_x2_max;              //0x000008AC
    REG32                          awb_skin_y2_min;              //0x000008B0
    REG32                          awb_skin_y2_max;              //0x000008B4
    REG32                          awb_ctd_msb;                  //0x000008B8
    REG32                          Int_dif_thr_mid;              //0x000008BC
    REG32                          lb_soft_rstn;                 //0x000008C0
    REG32                          vsync_end_high;               //0x000008C4
    REG32                          vsync_end_low;                //0x000008C8
    REG32                          line_numL;                    //0x000008CC
    REG32                          pix_numL;                     //0x000008D0
    REG32                          pix_line_numH;                //0x000008D4
    REG32                          lb_ctrl;                      //0x000008D8
    REG32                          data_format;                  //0x000008DC
    REG32                          lb_enable;                    //0x000008E0
    REG32                          vh_inv;                       //0x000008E4
    REG32                          line_cnt_L;                   //0x000008E8
    REG32                          line_cnt_H;                   //0x000008EC
    REG32                          num_check;                    //0x000008F0
    REG32                          dci_ctrl_reg;                 //0x000008F4
    REG32                          dci_ofst_reg;                 //0x000008F8
    REG32                          dci_hist_reg;                 //0x000008FC
    REG32                          ae_sw_ctrl_reg;               //0x00000900
    REG32                          ae_thr_reg;                   //0x00000904
    REG32                          ae_misc_ctrl_reg;             //0x00000908
    REG32                          csup_xx_reg;                  //0x0000090C
    REG32                          contr_ythr_reg;               //0x00000910
    REG32                          contr_yave_offset_reg;        //0x00000914
    REG32                          contr_ku_Lo_reg;              //0x00000918
    REG32                          contr_kl_Lo_reg;              //0x0000091C
    REG32                          contr_ku_Mid_reg;             //0x00000920
    REG32                          contr_kl_Mid_reg;             //0x00000924
    REG32                          contr_ku_Hi_reg;              //0x00000928
    REG32                          contr_kl_Hi_reg;              //0x0000092C
    REG32                          luma_offset_Lo_reg;           //0x00000930
    REG32                          luma_offset_Mid_reg;          //0x00000934
    REG32                          luma_offset_Hi_reg;           //0x00000938
    REG32                          u_gain_Lo_reg;                //0x0000093C
    REG32                          v_gain_Lo_reg;                //0x00000940
    REG32                          u_gain_Mid_reg;               //0x00000944
    REG32                          v_gain_Mid_reg;               //0x00000948
    REG32                          u_gain_Hi_reg;                //0x0000094C
    REG32                          v_gain_Hi_reg;                //0x00000950
    REG32                          again_sel_th0_reg;            //0x00000954
    REG32                          awb_cc_type_ctrl_reg;         //0x00000958
    REG32                          awb_cc_type_th_reg;           //0x0000095C
    REG32                          isp_wrapper_ctrl_1;           //0x00000960
    REG32                          top_dummy;                    //0x00000964
    REG32                          left_dummy;                   //0x00000968
    REG32                          isp_wrapper_ctrl_2;           //0x0000096C
    REG32                          line_num_l_reg;               //0x00000970
    REG32                          pix_num_l_reg;                //0x00000974
    REG32                          v_dummy;                      //0x00000978
    REG32                          scg;                          //0x0000097C
    REG32                          y_gamma_b0;                   //0x00000980
    REG32                          y_gamma_b1;                   //0x00000984
    REG32                          y_gamma_b2;                   //0x00000988
    REG32                          y_gamma_b4;                   //0x0000098C
    REG32                          y_gamma_b6;                   //0x00000990
    REG32                          y_gamma_b8;                   //0x00000994
    REG32                          y_gamma_b10;                  //0x00000998
    REG32                          y_gamma_b12;                  //0x0000099C
    REG32                          y_gamma_b16;                  //0x000009A0
    REG32                          y_gamma_b20;                  //0x000009A4
    REG32                          y_gamma_b24;                  //0x000009A8
    REG32                          y_gamma_b28;                  //0x000009AC
    REG32                          y_gamma_b32;                  //0x000009B0
    REG32                          r_awb_gain_in;                //0x000009B4
    REG32                          g_awb_gain_in;                //0x000009B8
    REG32                          b_awb_gain_in;                //0x000009BC
    REG32                          r_drc_gain_in;                //0x000009C0
    REG32                          gr_drc_gain_in;               //0x000009C4
    REG32                          gb_drc_gain_in;               //0x000009C8
    REG32                          b_drc_gain_in;                //0x000009CC
    REG32                          ae_ctrl;                      //0x000009D0
    REG32                          ae_ctrl2;                     //0x000009D4
    REG32                          ae_ctrl3;                     //0x000009D8
    REG32                          ae_ctrl4;                     //0x000009DC
    REG32                          ae_win_start;                 //0x000009E0
    REG32                          ae_win_width;                 //0x000009E4
    REG32                          ae_win_height;                //0x000009E8
    REG32                          exp_init;                     //0x000009EC
    REG32                          exp_ceil_init;                //0x000009F0
    REG32                          ae_exp_1E;                    //0x000009F4
    REG32                          ae_diff_thr;                  //0x000009F8
    REG32                          ae_bh_sel;                    //0x000009FC
    REG32                          awb_ctrl;                     //0x00000A00
    REG32                          awb_ctrl2;                    //0x00000A04
    REG32                          awb_y_max;                    //0x00000A08
    REG32                          awb_stop;                     //0x00000A0C
    REG32                          awb_algo;                     //0x00000A10
    REG32                          awb_ctrl3;                    //0x00000A14
    REG32                          awb_ctrl4;                    //0x00000A18
    REG32                          dig_gain_in;                  //0x00000A1C
    REG32                          Y_init_thr;                   //0x00000A20
    REG32                          y_ave_target;                 //0x00000A24
    REG32                          y_lmt_offset;                 //0x00000A28
    REG32                          again_sel_th2;                //0x00000A2C
    REG32                          yave_target_chg1;             //0x00000A30
    REG32                          image_eff_reg;                //0x00000A34
    REG32                          Ywave_out;                    //0x00000A38
    REG32                          ae_bright_hist;               //0x00000A3C
    REG32                          yave_out;                     //0x00000A40
    REG32                          exp_out;                      //0x00000A44
    REG32                          misc_out;                     //0x00000A48
    REG32                          awb_debug_out;                //0x00000A4C
    REG32                          mono_color;                   //0x00000A50
    REG32                          r_awb_gain;                   //0x00000A54
    REG32                          b_awb_gain;                   //0x00000A58
    REG32                          Misc_status;                  //0x00000A5C
    REG32                          Yave_contr;                   //0x00000A60
    REG32                          Gamma_type;                   //0x00000A64
    REG32                          blc_line;                     //0x00000A68
    REG32                          lsc_xx;                       //0x00000A6C
    REG32                          lsc_blc_gain_th;              //0x00000A70
    REG32                          blc_ctrl;                     //0x00000A74
    REG32                          blc_init;                     //0x00000A78
    REG32                          blc_offset;                   //0x00000A7C
    REG32                          blc_thr;                      //0x00000A80
    REG32                          lsc_xy_cent;                  //0x00000A84
    REG32                          cnr_dif_thr;                  //0x00000A88
    REG32                          cnr_thr;                      //0x00000A8C
    REG32                          gamma_ctrl;                   //0x00000A90
    REG32                          bayer_gamma_b0;               //0x00000A94
    REG32                          bayer_gamma_b1;               //0x00000A98
    REG32                          bayer_gamma_b2;               //0x00000A9C
    REG32                          bayer_gamma_b3;               //0x00000AA0
    REG32                          bayer_gamma_b4;               //0x00000AA4
    REG32                          bayer_gamma_b6;               //0x00000AA8
    REG32                          bayer_gamma_b8;               //0x00000AAC
    REG32                          bayer_gamma_b10;              //0x00000AB0
    REG32                          bayer_gamma_b12;              //0x00000AB4
    REG32                          bayer_gamma_b16;              //0x00000AB8
    REG32                          bayer_gamma_b20;              //0x00000ABC
    REG32                          bayer_gamma_b24;              //0x00000AC0
    REG32                          bayer_gamma_b28;              //0x00000AC4
    REG32                          bayer_gamma_b32;              //0x00000AC8
    REG32                          bayer_gamma_b36;              //0x00000ACC
    REG32                          bayer_gamma_b40;              //0x00000AD0
    REG32                          bayer_gamma_b48;              //0x00000AD4
    REG32                          bayer_gamma_b56;              //0x00000AD8
    REG32                          bayer_gamma_b64;              //0x00000ADC
    REG32                          blc_out0;                     //0x00000AE0
    REG32                          blc_out1;                     //0x00000AE4
    REG32                          dpc_ctrl_0;                   //0x00000AE8
    REG32                          dpc_ctrl_1;                   //0x00000AEC
    REG32                          y_thr_Lo;                     //0x00000AF0
    REG32                          y_thr_Mid;                    //0x00000AF4
    REG32                          y_thr_Hi;                     //0x00000AF8
    REG32                          intp_cfa_hv;                  //0x00000AFC
    REG32                          manual_adj;                   //0x00000B00
    REG32                          dpc_int_thr_Lo;               //0x00000B04
    REG32                          dpc_int_thr_Hi;               //0x00000B08
    REG32                          again_sel_th1;                //0x00000B0C
    REG32                          dpc_nr_lf_str_Lo;             //0x00000B10
    REG32                          dpc_nr_hf_str_Lo;             //0x00000B14
    REG32                          dpc_nr_area_thr_Lo;           //0x00000B18
    REG32                          dpc_nr_lf_str_Mid;            //0x00000B1C
    REG32                          dpc_nr_hf_str_Mid;            //0x00000B20
    REG32                          dpc_nr_area_thr_Mid;          //0x00000B24
    REG32                          dpc_nr_lf_str_Hi;             //0x00000B28
    REG32                          dpc_nr_hf_str_Hi;             //0x00000B2C
    REG32                          dpc_nr_area_thr_Hi;           //0x00000B30
    REG32                          intp_ctrl;                    //0x00000B34
    REG32                          intp_cfa_h_thr;               //0x00000B38
    REG32                          intp_cfa_v_thr;               //0x00000B3C
    REG32                          intp_grgb_sel_lmt;            //0x00000B40
    REG32                          intp_gf_lmt_thr;              //0x00000B44
    REG32                          cc_r_offset;                  //0x00000B48
    REG32                          cc_g_offset;                  //0x00000B4C
    REG32                          cc_b_offset;                  //0x00000B50
    REG32                          cc_00;                        //0x00000B54
    REG32                          cc_01;                        //0x00000B58
    REG32                          cc_10;                        //0x00000B5C
    REG32                          cc_11;                        //0x00000B60
    REG32                          cc_20;                        //0x00000B64
    REG32                          cc_21;                        //0x00000B68
    REG32                          cc_R_offset_post;             //0x00000B6C
    REG32                          cc_G_offset_post;             //0x00000B70
    REG32                          cc_B_offset_post;             //0x00000B74
    REG32                          cc2_r_offset;                 //0x00000B78
    REG32                          cc2_g_offset;                 //0x00000B7C
    REG32                          cc2_b_offset;                 //0x00000B80
    REG32                          cc2_00;                       //0x00000B84
    REG32                          cc2_01;                       //0x00000B88
    REG32                          cc2_10;                       //0x00000B8C
    REG32                          cc2_11;                       //0x00000B90
    REG32                          cc2_20;                       //0x00000B94
    REG32                          cc2_21;                       //0x00000B98
    REG32                          sharp_lmt;                    //0x00000B9C
    REG32                          sharp_mode;                   //0x00000BA0
    REG32                          sharp_gain_str_Lo;            //0x00000BA4
    REG32                          sharp_nr_area_thr_Lo;         //0x00000BA8
    REG32                          sharp_gain_str_Mid;           //0x00000BAC
    REG32                          sharp_nr_area_thr_Mid;        //0x00000BB0
    REG32                          sharp_gain_str_Hi;            //0x00000BB4
    REG32                          sharp_nr_area_thr_Hi;         //0x00000BB8
    REG32                          ynr_ctrl_reg;                 //0x00000BBC
    REG32                          ynr_lf_method_str;            //0x00000BC0
    REG32                          ynr_lf_str_Lo;                //0x00000BC4
    REG32                          ynr_hf_str_Lo;                //0x00000BC8
    REG32                          ynr_area_thr_Lo;              //0x00000BCC
    REG32                          ynr_lf_str_Mid;               //0x00000BD0
    REG32                          ynr_hf_str_Mid;               //0x00000BD4
    REG32                          ynr_area_thr_Mid;             //0x00000BD8
    REG32                          ynr_lf_str_Hi;                //0x00000BDC
    REG32                          ynr_hf_str_Hi;                //0x00000BE0
    REG32                          ynr_area_thr_Hi;              //0x00000BE4
    REG32                          Hue_sin_reg;                  //0x00000BE8
    REG32                          Hue_cos_reg;                  //0x00000BEC
    REG32                          cnr_1d_ctrl_reg;              //0x00000BF0
    REG32                          cnr_xx_reg;                   //0x00000BF4
    REG32                          in5_low_th_reg;               //0x00000BF8
    REG32                          in5_high_th_reg;              //0x00000BFC
    REG32 Reserved_00000C00[72];                //0x00000C00
    REG32                          p2_up_r_reg;                  //0x00000D20
    REG32                          p2_up_g_reg;                  //0x00000D24
    REG32                          p2_up_b_reg;                  //0x00000D28
    REG32                          p2_down_r_reg;                //0x00000D2C
    REG32                          p2_down_g_reg;                //0x00000D30
    REG32                          p2_down_b_reg;                //0x00000D34
    REG32                          p2_left_r_reg;                //0x00000D38
    REG32                          p2_left_g_reg;                //0x00000D3C
    REG32                          p2_left_b_reg;                //0x00000D40
    REG32                          p2_right_r_reg;               //0x00000D44
    REG32                          p2_right_g_reg;               //0x00000D48
    REG32                          p2_right_b_reg;               //0x00000D4C
    REG32                          p4_q1_r_reg;                  //0x00000D50
    REG32                          p4_q1_g_reg;                  //0x00000D54
    REG32                          p4_q1_b_reg;                  //0x00000D58
    REG32                          p4_q2_r_reg;                  //0x00000D5C
    REG32                          p4_q2_g_reg;                  //0x00000D60
    REG32                          p4_q2_b_reg;                  //0x00000D64
    REG32                          p4_q3_r_reg;                  //0x00000D68
    REG32                          p4_q3_g_reg;                  //0x00000D6C
    REG32                          p4_q3_b_reg;                  //0x00000D70
    REG32                          p4_q4_r_reg;                  //0x00000D74
    REG32                          p4_q4_g_reg;                  //0x00000D78
    REG32                          p4_q4_b_reg;                  //0x00000D7C
    REG32                          ae_e00_sta_reg;               //0x00000D80
    REG32                          ae_e00_num_reg;               //0x00000D84
    REG32                          ae_e01_sta_reg;               //0x00000D88
    REG32                          ae_e01_num_reg;               //0x00000D8C
    REG32                          ae_e02_sta_reg;               //0x00000D90
    REG32                          ae_e02_num_reg;               //0x00000D94
    REG32                          ae_e1_sta_reg;                //0x00000D98
    REG32                          ae_e1_num_reg;                //0x00000D9C
    REG32                          ae_e2_sta_reg;                //0x00000DA0
    REG32                          ae_e2_num_reg;                //0x00000DA4
    REG32                          ae_e3_sta_reg;                //0x00000DA8
    REG32                          ae_e3_num_reg;                //0x00000DAC
    REG32                          ae_e4_sta_reg;                //0x00000DB0
    REG32                          ae_e4_num_reg;                //0x00000DB4
    REG32                          ae_e5_sta_reg;                //0x00000DB8
    REG32                          ae_e5_num_reg;                //0x00000DBC
    REG32                          ae_e6_sta_reg;                //0x00000DC0
    REG32                          ae_e6_num_reg;                //0x00000DC4
    REG32                          ae_e7_sta_reg;                //0x00000DC8
    REG32                          ae_e7_num_reg;                //0x00000DCC
    REG32                          ae_e8_sta_reg;                //0x00000DD0
    REG32                          ae_e8_num_reg;                //0x00000DD4
    REG32                          ae_e9_sta_reg;                //0x00000DD8
    REG32                          ae_e9_num_reg;                //0x00000DDC
    REG32                          ae_ea_sta_reg;                //0x00000DE0
    REG32                          ae_ea_num_reg;                //0x00000DE4
    REG32                          ae_eb_sta_reg;                //0x00000DE8
    REG32                          ae_eb_num_reg;                //0x00000DEC
    REG32                          ae_ec_sta_reg;                //0x00000DF0
    REG32                          ae_ec_num_reg;                //0x00000DF4
    REG32                          ae_ed_sta_reg;                //0x00000DF8
    REG32                          ae_ed_num_reg;                //0x00000DFC
    REG32                          bayer_gamma2_b0;              //0x00000E00
    REG32                          bayer_gamma2_b1;              //0x00000E04
    REG32                          bayer_gamma2_b2;              //0x00000E08
    REG32                          bayer_gamma2_b3;              //0x00000E0C
    REG32                          bayer_gamma2_b4;              //0x00000E10
    REG32                          bayer_gamma2_b6;              //0x00000E14
    REG32                          bayer_gamma2_b8;              //0x00000E18
    REG32                          bayer_gamma2_b10;             //0x00000E1C
    REG32                          bayer_gamma2_b12;             //0x00000E20
    REG32                          bayer_gamma2_b16;             //0x00000E24
    REG32                          bayer_gamma2_b20;             //0x00000E28
    REG32                          bayer_gamma2_b24;             //0x00000E2C
    REG32                          bayer_gamma2_b28;             //0x00000E30
    REG32                          bayer_gamma2_b32;             //0x00000E34
    REG32                          bayer_gamma2_b36;             //0x00000E38
    REG32                          bayer_gamma2_b40;             //0x00000E3C
    REG32                          bayer_gamma2_b48;             //0x00000E40
    REG32                          bayer_gamma2_b56;             //0x00000E44
    REG32                          bayer_gamma2_b64;             //0x00000E48
    REG32                          y_thr7_Lo_reg;                //0x00000E4C
    REG32                          y_thr7_Mid_reg;               //0x00000E50
    REG32                          y_thr7_Hi_reg;                //0x00000E54
    REG32                          dpa_new_ctrl_reg;             //0x00000E58
    REG32                          dpa_new_ctrl_hi_reg;          //0x00000E5C
    REG32                          ae_index_gap;                 //0x00000E60
    REG32                          awb_calc_height_reg;          //0x00000E64
    REG32                          drc_r_clp_value_reg;          //0x00000E68
    REG32                          drc_gr_clp_value_reg;         //0x00000E6C
    REG32                          drc_gb_clp_value_reg;         //0x00000E70
    REG32                          drc_b_clp_value_reg;          //0x00000E74
    REG32                          sepia_cr_reg;                 //0x00000E78
    REG32                          sepia_cb_reg;                 //0x00000E7C
    REG32                          csup_y_min_hi_reg;            //0x00000E80
    REG32                          csup_gain_hi_reg;             //0x00000E84
    REG32                          csup_y_max_low_reg;           //0x00000E88
    REG32                          csup_gain_low_reg;            //0x00000E8C
    REG32                          ae_dk_hist_thr_reg;           //0x00000E90
    REG32                          ae_br_hist_thr_reg;           //0x00000E94
    REG32                          hist_bp_level_reg;            //0x00000E98
    REG32                          outdoor_th_reg;               //0x00000E9C
    REG32                          awb_rgain_low_reg;            //0x00000EA0
    REG32                          awb_rgain_high_reg;           //0x00000EA4
    REG32                          awb_bgain_low_reg;            //0x00000EA8
    REG32                          awb_bgain_high_reg;           //0x00000EAC
    REG32                          awb_calc_start_reg;           //0x00000EB0
    REG32                          awb_calc_width_reg;           //0x00000EB4
    REG32                          hist_dp_level_reg;            //0x00000EB8
    REG32                          awb_y_fmin;                   //0x00000EBC
    REG32                          awb_y_fmax;                   //0x00000EC0
    REG32                          awb_cb_fmin;                  //0x00000EC4
    REG32                          awb_cb_fmax;                  //0x00000EC8
    REG32                          awb_cr_fmin;                  //0x00000ECC
    REG32                          awb_cr_fmax;                  //0x00000ED0
    REG32                          awb_y_fmin2;                  //0x00000ED4
    REG32                          awb_y_fmax2;                  //0x00000ED8
    REG32                          awb_cb_fmin2;                 //0x00000EDC
    REG32                          awb_cb_fmax2;                 //0x00000EE0
    REG32                          awb_cr_fmin2;                 //0x00000EE4
    REG32                          awb_cr_fmax2;                 //0x00000EE8
    REG32                          ae_use_mean;                  //0x00000EEC
    REG32                          ae_weight_sta;                //0x00000EF0
    REG32                          ae_qWidth;                    //0x00000EF4
    REG32                          ae_qHeight;                   //0x00000EF8
    REG32                          ae_win_sta;                   //0x00000EFC
    REG32                          ae_Width;                     //0x00000F00
    REG32                          ae_Height;                    //0x00000F04
    REG32                          sw_update;                    //0x00000F08
    REG32                          awb_ctrl5;                    //0x00000F0C
    REG32                          awb_ctrl6;                    //0x00000F10
    REG32                          sca_reg;                      //0x00000F14
    REG32                          ae_ee_sta_reg;                //0x00000F18
    REG32                          ae_ee_num_reg;                //0x00000F1C
    REG32                          ae_ef_sta_reg;                //0x00000F20
    REG32                          ae_ef_num_reg;                //0x00000F24
    REG32                          ae_thr_big_reg;               //0x00000F28
    REG32                          sharp_gain_minus_low;         //0x00000F2C
    REG32                          sharp_gain_minus_mid;         //0x00000F30
    REG32                          sharp_gain_minus_hi;          //0x00000F34
    REG32                          sharp_mode_mid_hi;            //0x00000F38
    REG32                          Fw_version_reg;               //0x00000F3C
    REG32                          awb_y_min_reg;                //0x00000F40
    REG32                          y_red_coef_reg;               //0x00000F44
    REG32                          y_blue_coef_reg;              //0x00000F48
    REG32                          cb_red_coef_reg;              //0x00000F4C
    REG32                          cr_blue_coef_reg;             //0x00000F50
    REG32                          Hist_vbp_level_reg;           //0x00000F54
    REG32                          Hist_vdp_level_reg;           //0x00000F58
    REG32 Reserved_00000F5C[40];                //0x00000F5C
} HWP_CAMERA_T;

#define hwp_camera                 ((HWP_CAMERA_T*) REG_ACCESS_ADDRESS(REG_CAMERA_BASE))


//CTRL
typedef union {
    REG32 v;
    struct {
        REG32 enable : 1; // [0]
        REG32 dctenable : 1; // [1]
        REG32 buf_enable : 1; // [2]
        REG32 rgb_rfirst : 1; // [3]
        REG32 dataformat : 2; // [5:4]
        REG32 cfg_cam_c2cse : 2; // [7:6]
        REG32 reset_pol : 1; // [8]
        REG32 pwdn_pol : 1; // [9]
        REG32 vsync_pol : 1; // [10]
        REG32 href_pol : 1; // [11]
        REG32 pixclk_pol : 1; // [12]
        REG32 __13_13 : 1;
        REG32 vsync_drop : 1; // [14]
        REG32 __15_15 : 1;
        REG32 decimfrm : 2; // [17:16]
        REG32 decimcol : 2; // [19:18]
        REG32 decimrow : 2; // [21:20]
        REG32 __23_22 : 2;
        REG32 reorder : 3; // [26:24]
        REG32 __27_27 : 1;
        REG32 cropen : 1; // [28]
        REG32 __29_29 : 1;
        REG32 bist_mode : 1; // [30]
        REG32 test : 1; // [31]
    } b;
} REG_CAMERA_CTRL_T;

//STATUS
typedef union {
    REG32 v;
    struct {
        REG32 ovfl : 1; // [0], read only
        REG32 vsync_r : 1; // [1], read only
        REG32 vsync_f : 1; // [2], read only
        REG32 dma_done : 1; // [3], read only
        REG32 fifo_empty : 1; // [4], read only
        REG32 spi_ovfl : 1; // [5], read only
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_STATUS_T;

//IRQ_MASK
typedef union {
    REG32 v;
    struct {
        REG32 ovfl : 1; // [0]
        REG32 vsync_r : 1; // [1]
        REG32 vsync_f : 1; // [2]
        REG32 dma_done : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_IRQ_MASK_T;

//IRQ_CLEAR
typedef union {
    REG32 v;
    struct {
        REG32 ovfl : 1; // [0]
        REG32 vsync_r : 1; // [1]
        REG32 vsync_f : 1; // [2]
        REG32 dma_done : 1; // [3]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_IRQ_CLEAR_T;

//IRQ_CAUSE
typedef union {
    REG32 v;
    struct {
        REG32 ovfl : 1; // [0], read only
        REG32 vsync_r : 1; // [1], read only
        REG32 vsync_f : 1; // [2], read only
        REG32 dma_done : 1; // [3], read only
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_IRQ_CAUSE_T;

//CMD_SET
typedef union {
    REG32 v;
    struct {
        REG32 pwdn : 1; // [0], write set
        REG32 __3_1 : 3;
        REG32 reset : 1; // [4], write set
        REG32 __7_5 : 3;
        REG32 fifo_reset : 1; // [8], write set
        REG32 __31_9 : 23;
    } b;
} REG_CAMERA_CMD_SET_T;

//CMD_CLR
typedef union {
    REG32 v;
    struct {
        REG32 pwdn : 1; // [0], write clear
        REG32 __3_1 : 3;
        REG32 reset : 1; // [4], write clear
        REG32 __31_5 : 27;
    } b;
} REG_CAMERA_CMD_CLR_T;

//DSTWINCOL
typedef union {
    REG32 v;
    struct {
        REG32 dstwincolstart : 12; // [11:0]
        REG32 __15_12 : 4;
        REG32 dstwincolend : 12; // [27:16]
        REG32 __31_28 : 4;
    } b;
} REG_CAMERA_DSTWINCOL_T;

//DSTWINRow
typedef union {
    REG32 v;
    struct {
        REG32 dstwinrowstart : 12; // [11:0]
        REG32 __15_12 : 4;
        REG32 dstwinrowend : 12; // [27:16]
        REG32 __31_28 : 4;
    } b;
} REG_CAMERA_DSTWINROW_T;

//SCL_CONFIG
typedef union {
    REG32 v;
    struct {
        REG32 scale_en : 1; // [0]
        REG32 __3_1 : 3;
        REG32 data_out_swap : 1; // [4]
        REG32 __7_5 : 3;
        REG32 scale_col : 2; // [9:8]
        REG32 __15_10 : 6;
        REG32 scale_row : 2; // [17:16]
        REG32 __31_18 : 14;
    } b;
} REG_CAMERA_SCL_CONFIG_T;

//SPI_CAMERA_REG0
typedef union {
    REG32 v;
    struct {
        REG32 camera_spi_slave_en : 1; // [0]
        REG32 camera_spi_master_en : 1; // [1]
        REG32 yuv_out_format : 3; // [4:2]
        REG32 overflow_rstn_only_vsync_low : 1; // [5]
        REG32 overflow_observe_only_vsync_low : 1; // [6]
        REG32 overflow_rstn_en : 1; // [7]
        REG32 big_end_dis : 1; // [8]
        REG32 overflow_inv : 1; // [9]
        REG32 href_inv : 1; // [10]
        REG32 vsync_inv : 1; // [11]
        REG32 block_num_per_line : 10; // [21:12]
        REG32 line_num_per_frame : 10; // [31:22]
    } b;
} REG_CAMERA_SPI_CAMERA_REG0_T;

//SPI_CAMERA_REG1
typedef union {
    REG32 v;
    struct {
        REG32 camera_clk_div_num : 16; // [15:0]
        REG32 cts_spi_master_reg : 1; // [16]
        REG32 ssn_cm_inv : 1; // [17]
        REG32 sck_cm_inv : 1; // [18]
        REG32 ssn_spi_oenb_dr : 1; // [19]
        REG32 ssn_spi_oenb_reg : 1; // [20]
        REG32 sck_spi_oenb_dr : 1; // [21]
        REG32 sck_spi_oenb_reg : 1; // [22]
        REG32 __28_23 : 6;
        REG32 sdo_spi_swap : 1; // [29]
        REG32 clk_inv : 1; // [30]
        REG32 sck_ddr_en : 1; // [31]
    } b;
} REG_CAMERA_SPI_CAMERA_REG1_T;

//SPI_CAMERA_REG2
typedef union {
    REG32 v;
    struct {
        REG32 ssn_wait_length : 8; // [7:0]
        REG32 init_wait_length : 8; // [15:8]
        REG32 word_num_per_block : 8; // [23:16]
        REG32 ssn_cs_delay : 2; // [25:24]
        REG32 data_receive_choose_bit : 2; // [27:26]
        REG32 ready_cs_inv : 1; // [28]
        REG32 ssn_cs_inv : 1; // [29]
        REG32 __31_30 : 2;
    } b;
} REG_CAMERA_SPI_CAMERA_REG2_T;

//SPI_CAMERA_REG3
typedef union {
    REG32 v;
    struct {
        REG32 line_wait_length : 16; // [15:0]
        REG32 block_wait_length : 8; // [23:16]
        REG32 ssn_high_length : 8; // [31:24]
    } b;
} REG_CAMERA_SPI_CAMERA_REG3_T;

//SPI_CAMERA_REG4
typedef union {
    REG32 v;
    struct {
        REG32 camera_spi_master_en_2 : 1; // [0]
        REG32 sdo_line_choose_bit : 2; // [2:1]
        REG32 data_size_choose_bit : 1; // [3]
        REG32 image_height_choose_bit : 1; // [4]
        REG32 image_width_choose_bit : 1; // [5]
        REG32 block_num_per_packet : 10; // [15:6]
        REG32 __31_16 : 16;
    } b;
} REG_CAMERA_SPI_CAMERA_REG4_T;

//SPI_CAMERA_REG5
typedef union {
    REG32 v;
    struct {
        REG32 sync_code : 24; // [23:0]
        REG32 __31_24 : 8;
    } b;
} REG_CAMERA_SPI_CAMERA_REG5_T;

//SPI_CAMERA_REG6
typedef union {
    REG32 v;
    struct {
        REG32 packet_id_data_start : 8; // [7:0]
        REG32 packet_id_line_start : 8; // [15:8]
        REG32 packet_id_frame_end : 8; // [23:16]
        REG32 packet_id_frame_start : 8; // [31:24]
    } b;
} REG_CAMERA_SPI_CAMERA_REG6_T;

//SPI_CAMERA_OBS0
typedef union {
    REG32 v;
    struct {
        REG32 line_id_15_0_ : 16; // [15:0], read only
        REG32 data_id_7_0_ : 8; // [23:16], read only
        REG32 observe_data_size_wrong : 1; // [24], read only
        REG32 observe_image_height_wrong : 1; // [25], read only
        REG32 observe_image_width_wrong : 1; // [26], read only
        REG32 observe_line_num_wrong : 1; // [27], read only
        REG32 observe_data_id_wrong : 1; // [28], read only
        REG32 __31_29 : 3;
    } b;
} REG_CAMERA_SPI_CAMERA_OBS0_T;

//SPI_CAMERA_OBS1
typedef union {
    REG32 v;
    struct {
        REG32 image_height : 16; // [15:0], read only
        REG32 image_width : 16; // [31:16], read only
    } b;
} REG_CAMERA_SPI_CAMERA_OBS1_T;

//CSI_CONFIG_REG0
typedef union {
    REG32 v;
    struct {
        REG32 num_d_term_en : 8; // [7:0]
        REG32 cur_frame_line_num : 13; // [20:8]
        REG32 data_lp_in_choose_bit : 2; // [22:21]
        REG32 clk_lp_inv : 1; // [23]
        REG32 trail_data_wrong_choose_bit : 1; // [24]
        REG32 sync_bypass : 1; // [25]
        REG32 rdata_bit_inv_en : 1; // [26]
        REG32 hs_sync_find_en : 1; // [27]
        REG32 line_packet_enable : 1; // [28]
        REG32 ecc_bypass : 1; // [29]
        REG32 data_lane_choose_bit : 1; // [30]
        REG32 csi_module_enable : 1; // [31]
    } b;
} REG_CAMERA_CSI_CONFIG_REG0_T;

//CSI_CONFIG_REG1
typedef union {
    REG32 v;
    struct {
        REG32 num_hs_settle : 8; // [7:0]
        REG32 lp_data_length_choose_bit : 3; // [10:8]
        REG32 data_clk_lp_posedge_choose : 3; // [13:11]
        REG32 clk_lp_ck_inv : 1; // [14]
        REG32 rclr_mask_en : 1; // [15]
        REG32 rinc_mask_en : 1; // [16]
        REG32 hs_enable_mask_en : 1; // [17]
        REG32 den_csi_inv_bit : 1; // [18]
        REG32 hsync_csi_inv_bit : 1; // [19]
        REG32 vsync_csi_inv_bit : 1; // [20]
        REG32 hs_data2_enable_reg : 1; // [21]
        REG32 hs_data1_enable_reg : 1; // [22]
        REG32 hs_data1_enable_choose_bit : 1; // [23]
        REG32 hs_data1_enable_dr : 1; // [24]
        REG32 data2_terminal_enable_reg : 1; // [25]
        REG32 data1_terminal_enable_reg : 1; // [26]
        REG32 data1_terminal_enable_dr : 1; // [27]
        REG32 lp_data_interrupt_clr : 1; // [28]
        REG32 lp_cmd_interrupt_clr : 1; // [29]
        REG32 lp_data_clr : 1; // [30]
        REG32 lp_cmd_clr : 1; // [31]
    } b;
} REG_CAMERA_CSI_CONFIG_REG1_T;

//CSI_CONFIG_REG2
typedef union {
    REG32 v;
    struct {
        REG32 num_hs_settle_clk : 16; // [15:0]
        REG32 num_c_term_en : 16; // [31:16]
    } b;
} REG_CAMERA_CSI_CONFIG_REG2_T;

//CSI_CONFIG_REG3
typedef union {
    REG32 v;
    struct {
        REG32 __5_0 : 6;
        REG32 clk_lp_in_choose_bit : 2; // [7:6]
        REG32 pu_lprx_reg : 1; // [8]
        REG32 pu_hsrx_reg : 1; // [9]
        REG32 pu_dr : 1; // [10]
        REG32 data_pnsw_reg : 1; // [11]
        REG32 hs_clk_enable_reg : 1; // [12]
        REG32 hs_clk_enable_choose_bit : 1; // [13]
        REG32 hs_clk_enable_dr : 1; // [14]
        REG32 clk_terminal_enable_reg : 1; // [15]
        REG32 clk_terminal_enable_dr : 1; // [16]
        REG32 observe_reg_5_low8_choose : 1; // [17]
        REG32 ecc_error_flag_reg : 1; // [18]
        REG32 ecc_error_dr : 1; // [19]
        REG32 csi_channel_sel : 1; // [20]
        REG32 two_lane_bit_reverse : 1; // [21]
        REG32 data2_lane_bit_reverse : 1; // [22]
        REG32 data1_lane_bit_reverse : 1; // [23]
        REG32 data2_hs_no_mask : 1; // [24]
        REG32 data1_hs_no_mask : 1; // [25]
        REG32 pu_lprx_d2_reg : 1; // [26]
        REG32 pu_lprx_d1_reg : 1; // [27]
        REG32 __28_28 : 1;
        REG32 clk_edge_sel : 1; // [29]
        REG32 clk_x2_sel : 1; // [30]
        REG32 single_data_lane_en : 1; // [31]
    } b;
} REG_CAMERA_CSI_CONFIG_REG3_T;

//CSI_CONFIG_REG4
typedef union {
    REG32 v;
    struct {
        REG32 num_hs_clk_useful : 31; // [30:0]
        REG32 num_hs_clk_useful_en : 1; // [31]
    } b;
} REG_CAMERA_CSI_CONFIG_REG4_T;

//CSI_CONFIG_REG5
typedef union {
    REG32 v;
    struct {
        REG32 vc_id_set : 2; // [1:0]
        REG32 data_lp_inv : 1; // [2]
        REG32 fifo_rclr_8809p_reg : 1; // [3]
        REG32 fifo_wclr_8809p_reg : 1; // [4]
        REG32 hs_sync_16bit_8809p_mode : 1; // [5]
        REG32 d_term_small_8809p_en : 1; // [6]
        REG32 data_line_inv_8809p_en : 1; // [7]
        REG32 hs_enable_8809p_mode : 1; // [8]
        REG32 sp_to_trail_8809p_en : 1; // [9]
        REG32 trail_wrong_8809p_bypass : 1; // [10]
        REG32 rinc_trail_8809p_bypass : 1; // [11]
        REG32 hs_data_enable_8809p_mode : 1; // [12]
        REG32 hs_clk_enable_8809p_mode : 1; // [13]
        REG32 data_type_re_check_en : 1; // [14]
        REG32 sync_id_reg : 8; // [22:15]
        REG32 sync_id_dr : 1; // [23]
        REG32 csi_observe_choose_bit : 5; // [28:24]
        REG32 crc_error_flag_reg : 1; // [29]
        REG32 crc_error_flag_dr : 1; // [30]
        REG32 csi_rinc_new_mode_dis : 1; // [31]
    } b;
} REG_CAMERA_CSI_CONFIG_REG5_T;

//CSI_CONFIG_REG6
typedef union {
    REG32 v;
    struct {
        REG32 data_type_dp_reg : 6; // [5:0]
        REG32 data_type_le_reg : 6; // [11:6]
        REG32 data_type_ls_reg : 6; // [17:12]
        REG32 data_type_fe_reg : 6; // [23:18]
        REG32 data_type_fs_reg : 6; // [29:24]
        REG32 data_type_dp_dr : 1; // [30]
        REG32 data_type_dr : 1; // [31]
    } b;
} REG_CAMERA_CSI_CONFIG_REG6_T;

//CSI_CONFIG_REG7
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 data_lane_16bits_mode : 1; // [2]
        REG32 terminal_2_hs_exchage_8809p : 1; // [3]
        REG32 terminal_1_hs_exchage_8809p : 1; // [4]
        REG32 data2_terminal_enable_8809p_dr : 1; // [5]
        REG32 hs_data2_enable_8809p_dr : 1; // [6]
        REG32 csi_dout_test_8809p_en : 1; // [7]
        REG32 csi_dout_test_8809p : 8; // [15:8]
        REG32 num_d_term_en : 8; // [23:16]
        REG32 num_hs_settle : 8; // [31:24]
    } b;
} REG_CAMERA_CSI_CONFIG_REG7_T;

//CSI_OBS4
typedef union {
    REG32 v;
    struct {
        REG32 hs_data_state : 14; // [13:0]
        REG32 phy_data_state : 15; // [28:14]
        REG32 fifo_wfull_almost : 1; // [29]
        REG32 fifo_wfull : 1; // [30]
        REG32 fifo_wempty : 1; // [31]
    } b;
} REG_CAMERA_CSI_OBS4_T;

//CSI_OBS5
typedef union {
    REG32 v;
    struct {
        REG32 csi_observe_reg_5_low : 8; // [7:0], read only
        REG32 lp_data_interrupt_flag : 1; // [8], read only
        REG32 lp_cmd_interrupt_flag : 1; // [9], read only
        REG32 phy_clk_state : 9; // [18:10], read only
        REG32 fifo_rcount : 9; // [27:19], read only
        REG32 crc_error : 1; // [28], read only
        REG32 err_ecc_corrected_flag : 1; // [29], read only
        REG32 err_data_corrected_flag : 1; // [30], read only
        REG32 err_data_zero_flag : 1; // [31], read only
    } b;
} REG_CAMERA_CSI_OBS5_T;

//CSI_ENABLE
typedef union {
    REG32 v;
    struct {
        REG32 csi_enable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CAMERA_CSI_ENABLE_T;

//CSI_CONFIG_REG8
typedef union {
    REG32 v;
    struct {
        REG32 dly_sel_clkn_reg : 4; // [3:0]
        REG32 dly_sel_clkp_reg : 4; // [7:4]
        REG32 dly_sel_data2_reg : 4; // [11:8]
        REG32 dly_sel_data1_reg : 4; // [15:12]
        REG32 vth_sel : 1; // [16]
        REG32 __31_17 : 15;
    } b;
} REG_CAMERA_CSI_CONFIG_REG8_T;

//soft_reset
typedef union {
    REG32 v;
    struct {
        REG32 dsp_reset : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CAMERA_SOFT_RESET_T;

//awb_x1_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_x1_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X1_MIN_T;

//awb_x1_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_x1_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X1_MAX_T;

//awb_y1_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_y1_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y1_MIN_T;

//awb_y1_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_y1_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y1_MAX_T;

//awb_x2_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_x2_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X2_MIN_T;

//awb_x2_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_x2_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X2_MAX_T;

//awb_y2_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_y2_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y2_MIN_T;

//awb_y2_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_y2_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y2_MAX_T;

//awb_x3_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_x3_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X3_MIN_T;

//awb_x3_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_x3_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X3_MAX_T;

//awb_y3_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_y3_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y3_MIN_T;

//awb_y3_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_y3_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y3_MAX_T;

//awb_x4_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_x4_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X4_MIN_T;

//awb_x4_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_x4_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X4_MAX_T;

//awb_y4_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_y4_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y4_MIN_T;

//awb_y4_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_y4_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y4_MAX_T;

//awb_x5_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_x5_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X5_MIN_T;

//awb_x5_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_x5_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_X5_MAX_T;

//awb_y5_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_y5_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y5_MIN_T;

//awb_y5_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_y5_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y5_MAX_T;

//awb_skin_x1_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_skin_x1_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_SKIN_X1_MIN_T;

//awb_skin_x1_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_skin_x1_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_SKIN_X1_MAX_T;

//awb_skin_y1_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_skin_y1_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_SKIN_Y1_MIN_T;

//awb_skin_y1_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_skin_y1_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_SKIN_Y1_MAX_T;

//awb_skin_x2_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_skin_x2_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_SKIN_X2_MIN_T;

//awb_skin_x2_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_skin_x2_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_SKIN_X2_MAX_T;

//awb_skin_y2_min
typedef union {
    REG32 v;
    struct {
        REG32 awb_skin_y2_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_SKIN_Y2_MIN_T;

//awb_skin_y2_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_skin_y2_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_SKIN_Y2_MAX_T;

//awb_ctd_msb
typedef union {
    REG32 v;
    struct {
        REG32 awb_x1_min_msb : 1; // [0]
        REG32 awb_x1_max_msb : 1; // [1]
        REG32 awb_y5_min_msb : 1; // [2]
        REG32 awb_y5_max_msb : 1; // [3]
        REG32 awb_adj_mode : 2; // [5:4]
        REG32 awb_ratio_mode : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CTD_MSB_T;

//Int_dif_thr_mid
typedef union {
    REG32 v;
    struct {
        REG32 int_dif_thr_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_INT_DIF_THR_MID_T;

//lb_soft_rstn
typedef union {
    REG32 v;
    struct {
        REG32 lb_soft_rstn : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CAMERA_LB_SOFT_RSTN_T;

//vsync_end_high
typedef union {
    REG32 v;
    struct {
        REG32 vsync_end_high : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_VSYNC_END_HIGH_T;

//vsync_end_low
typedef union {
    REG32 v;
    struct {
        REG32 vsync_end_low : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_VSYNC_END_LOW_T;

//line_numL
typedef union {
    REG32 v;
    struct {
        REG32 line_numl : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LINE_NUML_T;

//pix_numL
typedef union {
    REG32 v;
    struct {
        REG32 pix_numl : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_PIX_NUML_T;

//pix_line_numH
typedef union {
    REG32 v;
    struct {
        REG32 line_numh : 1; // [0]
        REG32 pix_numh_rsvd : 3; // [3:1]
        REG32 pix_numh : 2; // [5:4]
        REG32 line_numh_rsvd : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_PIX_LINE_NUMH_T;

//lb_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 low_order : 1; // [0]
        REG32 use_fb_reg : 1; // [1]
        REG32 not_cvp_reg : 1; // [2]
        REG32 first_byte_reg : 3; // [5:3]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_LB_CTRL_T;

//data_format
typedef union {
    REG32 v;
    struct {
        REG32 data_format : 2; // [1:0]
        REG32 __31_2 : 30;
    } b;
} REG_CAMERA_DATA_FORMAT_T;

//lb_enable
typedef union {
    REG32 v;
    struct {
        REG32 lb_enable : 1; // [0]
        REG32 __31_1 : 31;
    } b;
} REG_CAMERA_LB_ENABLE_T;

//vh_inv
typedef union {
    REG32 v;
    struct {
        REG32 hsync_inv : 1; // [0]
        REG32 vsync_inv : 1; // [1]
        REG32 __31_2 : 30;
    } b;
} REG_CAMERA_VH_INV_T;

//line_cnt_L
typedef union {
    REG32 v;
    struct {
        REG32 line_cnt_l : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LINE_CNT_L_T;

//line_cnt_H
typedef union {
    REG32 v;
    struct {
        REG32 line_cnt_h : 2; // [1:0], read only
        REG32 __31_2 : 30;
    } b;
} REG_CAMERA_LINE_CNT_H_T;

//num_check
typedef union {
    REG32 v;
    struct {
        REG32 line_num_check : 1; // [0], read only
        REG32 byte_num_check : 1; // [1], read only
        REG32 __3_2 : 2;
        REG32 line_num_clear : 1; // [4]
        REG32 byte_num_clear : 1; // [5]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_NUM_CHECK_T;

//dci_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 kl_low_light_fix : 1; // [0]
        REG32 kl_reg_fix : 1; // [1]
        REG32 ku_low_light_fix : 1; // [2]
        REG32 ku_reg_fix : 1; // [3]
        REG32 hofst : 2; // [5:4]
        REG32 vbh_sel : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DCI_CTRL_REG_T;

//dci_ofst_reg
typedef union {
    REG32 v;
    struct {
        REG32 kl_ofstx1 : 4; // [3:0]
        REG32 ku_ofstx1 : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DCI_OFST_REG_T;

//dci_hist_reg
typedef union {
    REG32 v;
    struct {
        REG32 dk_histx1 : 4; // [3:0]
        REG32 br_histx1 : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DCI_HIST_REG_T;

//ae_sw_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 nexp_sw_in : 4; // [3:0]
        REG32 __6_4 : 3;
        REG32 ae_ext_adj_start : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_SW_CTRL_REG_T;

//ae_thr_reg
typedef union {
    REG32 v;
    struct {
        REG32 thr_dark : 4; // [3:0]
        REG32 thr_bright : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_THR_REG_T;

//ae_misc_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 ofst_dec_low_sel : 2; // [1:0]
        REG32 ofst_dec_high_sel : 2; // [3:2]
        REG32 force_adj1 : 1; // [4]
        REG32 force_adj2 : 1; // [5]
        REG32 force_adj3 : 1; // [6]
        REG32 index_ofst_no_step : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_MISC_CTRL_REG_T;

//csup_xx_reg
typedef union {
    REG32 v;
    struct {
        REG32 x_low : 4; // [3:0]
        REG32 x_high : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CSUP_XX_REG_T;

//contr_ythr_reg
typedef union {
    REG32 v;
    struct {
        REG32 csup_gain_low_th_h : 1; // [0]
        REG32 csup_gain_high_th : 3; // [3:1]
        REG32 fixed_contr_ythr : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CONTR_YTHR_REG_T;

//contr_yave_offset_reg
typedef union {
    REG32 v;
    struct {
        REG32 yave_offset_reg : 6; // [5:0]
        REG32 ythr_sel : 1; // [6]
        REG32 yave_offset_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CONTR_YAVE_OFFSET_REG_T;

//contr_ku_Lo_reg
typedef union {
    REG32 v;
    struct {
        REG32 ku : 7; // [6:0]
        REG32 ku_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CONTR_KU_LO_REG_T;

//contr_kl_Lo_reg
typedef union {
    REG32 v;
    struct {
        REG32 kl : 7; // [6:0]
        REG32 kl_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CONTR_KL_LO_REG_T;

//contr_ku_Mid_reg
typedef union {
    REG32 v;
    struct {
        REG32 ku : 7; // [6:0]
        REG32 ku_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CONTR_KU_MID_REG_T;

//contr_kl_Mid_reg
typedef union {
    REG32 v;
    struct {
        REG32 kl : 7; // [6:0]
        REG32 kl_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CONTR_KL_MID_REG_T;

//contr_ku_Hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 ku : 7; // [6:0]
        REG32 ku_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CONTR_KU_HI_REG_T;

//contr_kl_Hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 kl : 7; // [6:0]
        REG32 kl_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CONTR_KL_HI_REG_T;

//luma_offset_Lo_reg
typedef union {
    REG32 v;
    struct {
        REG32 offset : 6; // [5:0]
        REG32 algo_sel : 1; // [6]
        REG32 offset_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LUMA_OFFSET_LO_REG_T;

//luma_offset_Mid_reg
typedef union {
    REG32 v;
    struct {
        REG32 offset : 6; // [5:0]
        REG32 algo_sel : 1; // [6]
        REG32 offset_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LUMA_OFFSET_MID_REG_T;

//luma_offset_Hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 offset : 6; // [5:0]
        REG32 algo_sel : 1; // [6]
        REG32 offset_sign : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LUMA_OFFSET_HI_REG_T;

//u_gain_Lo_reg
typedef union {
    REG32 v;
    struct {
        REG32 u_gain_lo_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_U_GAIN_LO_REG_T;

//v_gain_Lo_reg
typedef union {
    REG32 v;
    struct {
        REG32 v_gain_lo_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_V_GAIN_LO_REG_T;

//u_gain_Mid_reg
typedef union {
    REG32 v;
    struct {
        REG32 u_gain_mid_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_U_GAIN_MID_REG_T;

//v_gain_Mid_reg
typedef union {
    REG32 v;
    struct {
        REG32 v_gain_mid_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_V_GAIN_MID_REG_T;

//u_gain_Hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 u_gain_hi_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_U_GAIN_HI_REG_T;

//v_gain_Hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 v_gain_hi_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_V_GAIN_HI_REG_T;

//again_sel_th0_reg
typedef union {
    REG32 v;
    struct {
        REG32 contr_gain_low_th : 3; // [2:0]
        REG32 again_sel_th0_rsvd : 1; // [3]
        REG32 contr_gain_hi_th : 3; // [6:4]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_AGAIN_SEL_TH0_REG_T;

//awb_cc_type_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 cc_type_mode : 4; // [3:0]
        REG32 cc_gain_hi_th : 3; // [6:4]
        REG32 luma_first : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CC_TYPE_CTRL_REG_T;

//awb_cc_type_th_reg
typedef union {
    REG32 v;
    struct {
        REG32 r_big_th : 4; // [3:0]
        REG32 b_big_th : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CC_TYPE_TH_REG_T;

//isp_wrapper_ctrl_1
typedef union {
    REG32 v;
    struct {
        REG32 pout_mode : 2; // [1:0]
        REG32 yuv_mode : 2; // [3:2]
        REG32 vsync_toggle : 1; // [4]
        REG32 mipi_rstn : 1; // [5]
        REG32 hsync_fix : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_ISP_WRAPPER_CTRL_1_T;

//top_dummy
typedef union {
    REG32 v;
    struct {
        REG32 top_dummy : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_TOP_DUMMY_T;

//left_dummy
typedef union {
    REG32 v;
    struct {
        REG32 left_dummy : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LEFT_DUMMY_T;

//isp_wrapper_ctrl_2
typedef union {
    REG32 v;
    struct {
        REG32 rgb_mode_reg : 3; // [2:0]
        REG32 sub_mode : 1; // [3]
        REG32 mon_mode_reg : 1; // [4]
        REG32 oclk_inv_reg : 1; // [5]
        REG32 isp_out_en : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_ISP_WRAPPER_CTRL_2_T;

//line_num_l_reg
typedef union {
    REG32 v;
    struct {
        REG32 line_num_l_reg : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_LINE_NUM_L_REG_T;

//pix_num_l_reg
typedef union {
    REG32 v;
    struct {
        REG32 pix_num_l_reg : 7; // [6:0]
        REG32 csi_mon_reg : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_PIX_NUM_L_REG_T;

//v_dummy
typedef union {
    REG32 v;
    struct {
        REG32 vbot_dummy_reg : 4; // [3:0]
        REG32 vtop_dummy_reg : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_V_DUMMY_T;

//scg
typedef union {
    REG32 v;
    struct {
        REG32 kukl_sel : 1; // [0]
        REG32 reg94_rd_sel : 1; // [1]
        REG32 bayer_out_sel : 1; // [2]
        REG32 csup_en : 1; // [3]
        REG32 y_gamma_en : 2; // [5:4]
        REG32 yuv_sdi_en : 1; // [6]
        REG32 reg92_rd_sel : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SCG_T;

//y_gamma_b0
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b0 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B0_T;

//y_gamma_b1
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b1 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B1_T;

//y_gamma_b2
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B2_T;

//y_gamma_b4
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b4 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B4_T;

//y_gamma_b6
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b6 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B6_T;

//y_gamma_b8
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b8 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B8_T;

//y_gamma_b10
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b10 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B10_T;

//y_gamma_b12
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b12 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B12_T;

//y_gamma_b16
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b16 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B16_T;

//y_gamma_b20
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b20 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B20_T;

//y_gamma_b24
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b24 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B24_T;

//y_gamma_b28
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b28 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B28_T;

//y_gamma_b32
typedef union {
    REG32 v;
    struct {
        REG32 y_gamma_b32 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_GAMMA_B32_T;

//r_awb_gain_in
typedef union {
    REG32 v;
    struct {
        REG32 r_awb_gain_in : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_R_AWB_GAIN_IN_T;

//g_awb_gain_in
typedef union {
    REG32 v;
    struct {
        REG32 g_awb_gain_in : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_G_AWB_GAIN_IN_T;

//b_awb_gain_in
typedef union {
    REG32 v;
    struct {
        REG32 b_awb_gain_in : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_B_AWB_GAIN_IN_T;

//r_drc_gain_in
typedef union {
    REG32 v;
    struct {
        REG32 r_drc_gain_in : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_R_DRC_GAIN_IN_T;

//gr_drc_gain_in
typedef union {
    REG32 v;
    struct {
        REG32 gr_drc_gain_in : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_GR_DRC_GAIN_IN_T;

//gb_drc_gain_in
typedef union {
    REG32 v;
    struct {
        REG32 gb_drc_gain_in : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_GB_DRC_GAIN_IN_T;

//b_drc_gain_in
typedef union {
    REG32 v;
    struct {
        REG32 b_drc_gain_in : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_B_DRC_GAIN_IN_T;

//ae_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 ana_gain_in : 6; // [5:0]
        REG32 ae_update_en : 1; // [6]
        REG32 ae_en : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_CTRL_T;

//ae_ctrl2
typedef union {
    REG32 v;
    struct {
        REG32 awb_adj_sel : 2; // [1:0]
        REG32 gap_ae : 1; // [2]
        REG32 gap_be : 1; // [3]
        REG32 ae_action_period : 3; // [6:4]
        REG32 yave_mon_sel : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_CTRL2_T;

//ae_ctrl3
typedef union {
    REG32 v;
    struct {
        REG32 yave_use_mean : 2; // [1:0]
        REG32 yave_diff_thr_reg : 2; // [3:2]
        REG32 yave_sel : 2; // [5:4]
        REG32 yave_plus_bh_mode : 1; // [6]
        REG32 ywave_plus_bh_mode : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_CTRL3_T;

//ae_ctrl4
typedef union {
    REG32 v;
    struct {
        REG32 ae_hist_big_en : 1; // [0]
        REG32 ae_hist_too_big_en : 1; // [1]
        REG32 hist_ofst0 : 2; // [3:2]
        REG32 index_ofst0 : 2; // [5:4]
        REG32 index_ofst1 : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_CTRL4_T;

//ae_win_start
typedef union {
    REG32 v;
    struct {
        REG32 pcnt_left : 4; // [3:0]
        REG32 lcnt_top : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_WIN_START_T;

//ae_win_width
typedef union {
    REG32 v;
    struct {
        REG32 ae_win_width : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_WIN_WIDTH_T;

//ae_win_height
typedef union {
    REG32 v;
    struct {
        REG32 ae_win_height : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_WIN_HEIGHT_T;

//exp_init
typedef union {
    REG32 v;
    struct {
        REG32 exp_init : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_EXP_INIT_T;

//exp_ceil_init
typedef union {
    REG32 v;
    struct {
        REG32 exp_ceil_init : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_EXP_CEIL_INIT_T;

//ae_exp_1E
typedef union {
    REG32 v;
    struct {
        REG32 ae_exp_1e : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_EXP_1E_T;

//ae_diff_thr
typedef union {
    REG32 v;
    struct {
        REG32 thr2_dark : 4; // [3:0]
        REG32 thr2_bright : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_DIFF_THR_T;

//ae_bh_sel
typedef union {
    REG32 v;
    struct {
        REG32 bh_factor_indoor : 3; // [2:0]
        REG32 bh_factor_outdoor : 3; // [5:3]
        REG32 bh_mean_sel : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_BH_SEL_T;

//awb_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 awb_sw_mon_en : 1; // [0]
        REG32 fast_2x : 1; // [1]
        REG32 fast_4x : 1; // [2]
        REG32 awb_action_period : 3; // [5:3]
        REG32 awb_update_en : 1; // [6]
        REG32 awb_en : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CTRL_T;

//awb_ctrl2
typedef union {
    REG32 v;
    struct {
        REG32 awb_mon_sel : 3; // [2:0]
        REG32 awb_vld_sel : 1; // [3]
        REG32 awb_vld_mode : 3; // [6:4]
        REG32 awb_adj : 1; // [7], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CTRL2_T;

//awb_y_max
typedef union {
    REG32 v;
    struct {
        REG32 awb_y_max : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y_MAX_T;

//awb_stop
typedef union {
    REG32 v;
    struct {
        REG32 awb_stop_cb_neg_level : 2; // [1:0]
        REG32 awb_stop_cb_pos_level : 2; // [3:2]
        REG32 awb_stop_cr_neg_level : 2; // [5:4]
        REG32 awb_stop_cr_pos_level : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_STOP_T;

//awb_algo
typedef union {
    REG32 v;
    struct {
        REG32 awb_algo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_ALGO_T;

//awb_ctrl3
typedef union {
    REG32 v;
    struct {
        REG32 cr_ofst_lt1x : 1; // [0]
        REG32 cr_ofst_gt1x : 1; // [1]
        REG32 cb_ofst_lt1x : 1; // [2]
        REG32 cb_ofst_gt1x : 1; // [3]
        REG32 awb_sum_vld_sel : 1; // [4]
        REG32 awb_stop_sel_reg : 1; // [5]
        REG32 awb_skin_sel : 1; // [6]
        REG32 awb_algo_mode : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CTRL3_T;

//awb_ctrl4
typedef union {
    REG32 v;
    struct {
        REG32 awb_ctrl4 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CTRL4_T;

//dig_gain_in
typedef union {
    REG32 v;
    struct {
        REG32 dig_gain_in : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DIG_GAIN_IN_T;

//Y_init_thr
typedef union {
    REG32 v;
    struct {
        REG32 y_init_mode : 1; // [0]
        REG32 y_low_en : 1; // [1]
        REG32 y_high_en : 1; // [2]
        REG32 y_low_thr : 5; // [7:3]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_INIT_THR_T;

//y_ave_target
typedef union {
    REG32 v;
    struct {
        REG32 y_ave_target : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_AVE_TARGET_T;

//y_lmt_offset
typedef union {
    REG32 v;
    struct {
        REG32 y_low_limit : 3; // [2:0]
        REG32 y_lmt_ofst : 1; // [3]
        REG32 y_high_limit : 3; // [6:4]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_Y_LMT_OFFSET_T;

//again_sel_th2
typedef union {
    REG32 v;
    struct {
        REG32 ynr_gain_low_th : 3; // [2:0]
        REG32 again_sel_th2 : 1; // [3]
        REG32 ynr_gain_hi_th : 3; // [6:4]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_AGAIN_SEL_TH2_T;

//yave_target_chg1
typedef union {
    REG32 v;
    struct {
        REG32 yave_target_ofst_l : 4; // [3:0]
        REG32 yave_target_ofst_h : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YAVE_TARGET_CHG1_T;

//image_eff_reg
typedef union {
    REG32 v;
    struct {
        REG32 grey_en : 1; // [0]
        REG32 sepia_en : 1; // [1]
        REG32 negative_en : 1; // [2]
        REG32 color_bar_en : 1; // [3]
        REG32 image_eff_rsvd : 1; // [4]
        REG32 reg93_sel : 1; // [5]
        REG32 reg94_sel : 1; // [6]
        REG32 sharp_mon : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_IMAGE_EFF_REG_T;

//Ywave_out
typedef union {
    REG32 v;
    struct {
        REG32 ywave_out : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YWAVE_OUT_T;

//ae_bright_hist
typedef union {
    REG32 v;
    struct {
        REG32 ae_bright_hist : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_BRIGHT_HIST_T;

//yave_out
typedef union {
    REG32 v;
    struct {
        REG32 yave_out : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YAVE_OUT_T;

//exp_out
typedef union {
    REG32 v;
    struct {
        REG32 exp_out : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_EXP_OUT_T;

//misc_out
typedef union {
    REG32 v;
    struct {
        REG32 exp_out_h : 3; // [2:0], read only
        REG32 awb_ok : 1; // [3], read only
        REG32 nexp_sel : 2; // [5:4], read only
        REG32 fixed_0 : 1; // [6], read only
        REG32 ae_ok : 1; // [7], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_MISC_OUT_T;

//awb_debug_out
typedef union {
    REG32 v;
    struct {
        REG32 awb_crgt : 2; // [1:0], read only
        REG32 awb_cbgt : 2; // [3:2], read only
        REG32 awb_crsum_sign : 1; // [4], read only
        REG32 awb_cbsum_sign : 1; // [5], read only
        REG32 awb_cbcr : 1; // [6], read only
        REG32 awb_sum_vld : 1; // [7], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_DEBUG_OUT_T;

//mono_color
typedef union {
    REG32 v;
    struct {
        REG32 mono_color : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_MONO_COLOR_T;

//r_awb_gain
typedef union {
    REG32 v;
    struct {
        REG32 r_awb_gain : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_R_AWB_GAIN_T;

//b_awb_gain
typedef union {
    REG32 v;
    struct {
        REG32 b_awb_gain : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_B_AWB_GAIN_T;

//Misc_status
typedef union {
    REG32 v;
    struct {
        REG32 ana_gain_out : 6; // [5:0], read only
        REG32 cc_type : 1; // [6], read only
        REG32 is_outdoor : 1; // [7], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_MISC_STATUS_T;

//Yave_contr
typedef union {
    REG32 v;
    struct {
        REG32 yave_contr : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YAVE_CONTR_T;

//Gamma_type
typedef union {
    REG32 v;
    struct {
        REG32 gamma_type_mode : 3; // [2:0]
        REG32 gamma_gain_hi_th : 3; // [5:3]
        REG32 vgas : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_GAMMA_TYPE_T;

//blc_line
typedef union {
    REG32 v;
    struct {
        REG32 blc_line : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BLC_LINE_T;

//lsc_xx
typedef union {
    REG32 v;
    struct {
        REG32 x_low : 4; // [3:0]
        REG32 x_high : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LSC_XX_T;

//lsc_blc_gain_th
typedef union {
    REG32 v;
    struct {
        REG32 lsc_gain_low_th : 3; // [2:0]
        REG32 lsc_gain_hi_th : 3; // [5:3]
        REG32 csup_gain_low_th : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LSC_BLC_GAIN_TH_T;

//blc_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 blc_out_mode : 2; // [1:0]
        REG32 line_init_h : 1; // [2]
        REG32 blc_ofst_sign : 1; // [3]
        REG32 blc_mode : 2; // [5:4]
        REG32 blc_sel : 1; // [6]
        REG32 blc_en : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BLC_CTRL_T;

//blc_init
typedef union {
    REG32 v;
    struct {
        REG32 blc00_ofst : 4; // [3:0]
        REG32 blc01_ofst : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BLC_INIT_T;

//blc_offset
typedef union {
    REG32 v;
    struct {
        REG32 blc10_ofst : 4; // [3:0]
        REG32 blc11_ofst : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BLC_OFFSET_T;

//blc_thr
typedef union {
    REG32 v;
    struct {
        REG32 blc_thr : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_BLC_THR_T;

//lsc_xy_cent
typedef union {
    REG32 v;
    struct {
        REG32 y_cent : 4; // [3:0]
        REG32 x_cent : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_LSC_XY_CENT_T;

//cnr_dif_thr
typedef union {
    REG32 v;
    struct {
        REG32 cnr_v_en : 1; // [0]
        REG32 cnr_h_en : 1; // [1]
        REG32 vcnr_sel : 1; // [2]
        REG32 edge_mon : 1; // [3]
        REG32 awb_skin_mode : 3; // [6:4]
        REG32 gamma_type : 1; // [7], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CNR_DIF_THR_T;

//cnr_thr
typedef union {
    REG32 v;
    struct {
        REG32 cnr_thr_v : 3; // [2:0]
        REG32 edge_en_v : 1; // [3]
        REG32 cnr_thr_h : 3; // [6:4]
        REG32 edge_en_h : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CNR_THR_T;

//gamma_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 gamma_p_id : 1; // [0]
        REG32 gamma_l_id : 1; // [1]
        REG32 gamma_en_non_outdoor : 1; // [2]
        REG32 gamma_en_outdoor : 1; // [3]
        REG32 lsc_p_id : 1; // [4]
        REG32 lsc_l_id : 1; // [5]
        REG32 lsc_en_non_outdoor : 1; // [6]
        REG32 lsc_en_outdoor : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_GAMMA_CTRL_T;

//bayer_gamma_b0
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b0 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B0_T;

//bayer_gamma_b1
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b1 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B1_T;

//bayer_gamma_b2
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B2_T;

//bayer_gamma_b3
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b3 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B3_T;

//bayer_gamma_b4
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b4 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B4_T;

//bayer_gamma_b6
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b6 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B6_T;

//bayer_gamma_b8
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b8 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B8_T;

//bayer_gamma_b10
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b10 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B10_T;

//bayer_gamma_b12
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b12 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B12_T;

//bayer_gamma_b16
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b16 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B16_T;

//bayer_gamma_b20
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b20 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B20_T;

//bayer_gamma_b24
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b24 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B24_T;

//bayer_gamma_b28
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b28 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B28_T;

//bayer_gamma_b32
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b32 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B32_T;

//bayer_gamma_b36
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b36 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B36_T;

//bayer_gamma_b40
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b40 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B40_T;

//bayer_gamma_b48
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b48 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B48_T;

//bayer_gamma_b56
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b56 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B56_T;

//bayer_gamma_b64
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma_b64 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA_B64_T;

//blc_out0
typedef union {
    REG32 v;
    struct {
        REG32 blc_out0 : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BLC_OUT0_T;

//blc_out1
typedef union {
    REG32 v;
    struct {
        REG32 blc_out1 : 8; // [7:0], read only
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BLC_OUT1_T;

//dpc_ctrl_0
typedef union {
    REG32 v;
    struct {
        REG32 dpc_on : 1; // [0]
        REG32 adp_med_sel : 1; // [1]
        REG32 ana_gain_cmp : 2; // [3:2]
        REG32 rsvd : 1; // [4]
        REG32 nrf_gaus_sel : 1; // [5]
        REG32 bayer_nr_on : 1; // [6]
        REG32 cc_on : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_CTRL_0_T;

//dpc_ctrl_1
typedef union {
    REG32 v;
    struct {
        REG32 int_flg_cmp : 2; // [1:0]
        REG32 abs_sign_all_cmp : 2; // [3:2]
        REG32 int_dif_sel : 1; // [4]
        REG32 __31_5 : 27;
    } b;
} REG_CAMERA_DPC_CTRL_1_T;

//y_thr_Lo
typedef union {
    REG32 v;
    struct {
        REG32 y_thr_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_THR_LO_T;

//y_thr_Mid
typedef union {
    REG32 v;
    struct {
        REG32 y_thr_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_THR_MID_T;

//y_thr_Hi
typedef union {
    REG32 v;
    struct {
        REG32 y_thr_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_THR_HI_T;

//intp_cfa_hv
typedef union {
    REG32 v;
    struct {
        REG32 cfa_v_thr_l : 3; // [2:0]
        REG32 rsvd1 : 1; // [3]
        REG32 cfa_h_thr_l : 3; // [6:4]
        REG32 rsvd2 : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_INTP_CFA_HV_T;

//manual_adj
typedef union {
    REG32 v;
    struct {
        REG32 b_gain_adj : 1; // [0]
        REG32 g_gain_adj : 1; // [1]
        REG32 r_gain_adj : 1; // [2]
        REG32 ana_gain_adj : 1; // [3]
        REG32 adj_direction : 1; // [4]
        REG32 index_manual_adj : 1; // [5]
        REG32 in_capture_awb : 1; // [6]
        REG32 in_capture_ae : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_MANUAL_ADJ_T;

//dpc_int_thr_Lo
typedef union {
    REG32 v;
    struct {
        REG32 dpc_int_thr_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_INT_THR_LO_T;

//dpc_int_thr_Hi
typedef union {
    REG32 v;
    struct {
        REG32 dpc_int_thr_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_INT_THR_HI_T;

//again_sel_th1
typedef union {
    REG32 v;
    struct {
        REG32 bnr_gain_low_th : 3; // [2:0]
        REG32 again_sel_th1_rsvd : 1; // [3]
        REG32 bnr_gain_hi_th : 3; // [6:4]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_AGAIN_SEL_TH1_T;

//dpc_nr_lf_str_Lo
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_lf_str_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_LF_STR_LO_T;

//dpc_nr_hf_str_Lo
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_hf_str_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_HF_STR_LO_T;

//dpc_nr_area_thr_Lo
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_area_thr_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_AREA_THR_LO_T;

//dpc_nr_lf_str_Mid
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_lf_str_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_LF_STR_MID_T;

//dpc_nr_hf_str_Mid
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_hf_str_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_HF_STR_MID_T;

//dpc_nr_area_thr_Mid
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_area_thr_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_AREA_THR_MID_T;

//dpc_nr_lf_str_Hi
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_lf_str_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_LF_STR_HI_T;

//dpc_nr_hf_str_Hi
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_hf_str_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_HF_STR_HI_T;

//dpc_nr_area_thr_Hi
typedef union {
    REG32 v;
    struct {
        REG32 dpc_nr_area_thr_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_DPC_NR_AREA_THR_HI_T;

//intp_ctrl
typedef union {
    REG32 v;
    struct {
        REG32 pid_inv_en : 1; // [0]
        REG32 lid_inv_en : 1; // [1]
        REG32 gfilter_en : 1; // [2]
        REG32 gfilter3_en : 1; // [3]
        REG32 gfliter5_en : 1; // [4]
        REG32 sort_sel : 3; // [7:5]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_INTP_CTRL_T;

//intp_cfa_h_thr
typedef union {
    REG32 v;
    struct {
        REG32 intp_cfa_h_thr : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_INTP_CFA_H_THR_T;

//intp_cfa_v_thr
typedef union {
    REG32 v;
    struct {
        REG32 intp_cfa_v_thr : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_INTP_CFA_V_THR_T;

//intp_grgb_sel_lmt
typedef union {
    REG32 v;
    struct {
        REG32 intp_grgb_sel_lmt : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_INTP_GRGB_SEL_LMT_T;

//intp_gf_lmt_thr
typedef union {
    REG32 v;
    struct {
        REG32 intp_gf_lmt_thr : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_INTP_GF_LMT_THR_T;

//cc_r_offset
typedef union {
    REG32 v;
    struct {
        REG32 cc_r_offset : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_R_OFFSET_T;

//cc_g_offset
typedef union {
    REG32 v;
    struct {
        REG32 cc_g_offset : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_G_OFFSET_T;

//cc_b_offset
typedef union {
    REG32 v;
    struct {
        REG32 cc_b_offset : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_B_OFFSET_T;

//cc_00
typedef union {
    REG32 v;
    struct {
        REG32 cc_00 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_00_T;

//cc_01
typedef union {
    REG32 v;
    struct {
        REG32 cc_01 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_01_T;

//cc_10
typedef union {
    REG32 v;
    struct {
        REG32 cc_10 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_10_T;

//cc_11
typedef union {
    REG32 v;
    struct {
        REG32 cc_11 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_11_T;

//cc_20
typedef union {
    REG32 v;
    struct {
        REG32 cc_20 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_20_T;

//cc_21
typedef union {
    REG32 v;
    struct {
        REG32 cc_21 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_21_T;

//cc_R_offset_post
typedef union {
    REG32 v;
    struct {
        REG32 cc_r_offset_post : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_R_OFFSET_POST_T;

//cc_G_offset_post
typedef union {
    REG32 v;
    struct {
        REG32 cc_g_offset_post : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_G_OFFSET_POST_T;

//cc_B_offset_post
typedef union {
    REG32 v;
    struct {
        REG32 cc_b_offset_post : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC_B_OFFSET_POST_T;

//cc2_r_offset
typedef union {
    REG32 v;
    struct {
        REG32 cc2_r_offset : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_R_OFFSET_T;

//cc2_g_offset
typedef union {
    REG32 v;
    struct {
        REG32 cc2_g_offset : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_G_OFFSET_T;

//cc2_b_offset
typedef union {
    REG32 v;
    struct {
        REG32 cc2_b_offset : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_B_OFFSET_T;

//cc2_00
typedef union {
    REG32 v;
    struct {
        REG32 cc2_00 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_00_T;

//cc2_01
typedef union {
    REG32 v;
    struct {
        REG32 cc2_01 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_01_T;

//cc2_10
typedef union {
    REG32 v;
    struct {
        REG32 cc2_10 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_10_T;

//cc2_11
typedef union {
    REG32 v;
    struct {
        REG32 cc2_11 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_11_T;

//cc2_20
typedef union {
    REG32 v;
    struct {
        REG32 cc2_20 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_20_T;

//cc2_21
typedef union {
    REG32 v;
    struct {
        REG32 cc2_21 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CC2_21_T;

//sharp_lmt
typedef union {
    REG32 v;
    struct {
        REG32 sharp_lmt : 7; // [6:0]
        REG32 sharp_final_h : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_LMT_T;

//sharp_mode
typedef union {
    REG32 v;
    struct {
        REG32 sharp_cmp_gap_lo : 4; // [3:0]
        REG32 sharp_final : 2; // [5:4]
        REG32 sharp_sel : 1; // [6]
        REG32 rgb_test_pattern : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_MODE_T;

//sharp_gain_str_Lo
typedef union {
    REG32 v;
    struct {
        REG32 sharp_gain_str_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_GAIN_STR_LO_T;

//sharp_nr_area_thr_Lo
typedef union {
    REG32 v;
    struct {
        REG32 sharp_nr_area_thr_lo : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_SHARP_NR_AREA_THR_LO_T;

//sharp_gain_str_Mid
typedef union {
    REG32 v;
    struct {
        REG32 sharp_gain_str_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_GAIN_STR_MID_T;

//sharp_nr_area_thr_Mid
typedef union {
    REG32 v;
    struct {
        REG32 sharp_nr_area_thr_mid : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_SHARP_NR_AREA_THR_MID_T;

//sharp_gain_str_Hi
typedef union {
    REG32 v;
    struct {
        REG32 sharp_gain_str_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_GAIN_STR_HI_T;

//sharp_nr_area_thr_Hi
typedef union {
    REG32 v;
    struct {
        REG32 sharp_nr_area_thr_hi : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_SHARP_NR_AREA_THR_HI_T;

//ynr_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 ynr_on : 1; // [0]
        REG32 ynr_edge_methode : 2; // [2:1]
        REG32 sharp_on : 1; // [3]
        REG32 sharp_plus_mode : 2; // [5:4]
        REG32 y_ae_sel : 2; // [7:6]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_CTRL_REG_T;

//ynr_lf_method_str
typedef union {
    REG32 v;
    struct {
        REG32 ynr_lf_method_str : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_LF_METHOD_STR_T;

//ynr_lf_str_Lo
typedef union {
    REG32 v;
    struct {
        REG32 ynr_lf_str_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_LF_STR_LO_T;

//ynr_hf_str_Lo
typedef union {
    REG32 v;
    struct {
        REG32 ynr_hf_str_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_HF_STR_LO_T;

//ynr_area_thr_Lo
typedef union {
    REG32 v;
    struct {
        REG32 ynr_area_thr_lo : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_AREA_THR_LO_T;

//ynr_lf_str_Mid
typedef union {
    REG32 v;
    struct {
        REG32 ynr_lf_str_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_LF_STR_MID_T;

//ynr_hf_str_Mid
typedef union {
    REG32 v;
    struct {
        REG32 ynr_hf_str_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_HF_STR_MID_T;

//ynr_area_thr_Mid
typedef union {
    REG32 v;
    struct {
        REG32 ynr_area_thr_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_AREA_THR_MID_T;

//ynr_lf_str_Hi
typedef union {
    REG32 v;
    struct {
        REG32 ynr_lf_str_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_LF_STR_HI_T;

//ynr_hf_str_Hi
typedef union {
    REG32 v;
    struct {
        REG32 ynr_hf_str_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_HF_STR_HI_T;

//ynr_area_thr_Hi
typedef union {
    REG32 v;
    struct {
        REG32 ynr_area_thr_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_YNR_AREA_THR_HI_T;

//Hue_sin_reg
typedef union {
    REG32 v;
    struct {
        REG32 hue_sin_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_HUE_SIN_REG_T;

//Hue_cos_reg
typedef union {
    REG32 v;
    struct {
        REG32 hue_cosx_reg : 7; // [6:0]
        REG32 sin_sign_reg : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_HUE_COS_REG_T;

//cnr_1d_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 cnr_dif_thr_mid : 4; // [3:0]
        REG32 cnr_1d_on : 1; // [4]
        REG32 satur_on : 1; // [5]
        REG32 hue_on : 1; // [6]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_CNR_1D_CTRL_REG_T;

//cnr_xx_reg
typedef union {
    REG32 v;
    struct {
        REG32 cnr_dif_thr_low : 4; // [3:0]
        REG32 cnr_dif_thr_high : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CNR_XX_REG_T;

//in5_low_th_reg
typedef union {
    REG32 v;
    struct {
        REG32 in5_low_th_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_IN5_LOW_TH_REG_T;

//in5_high_th_reg
typedef union {
    REG32 v;
    struct {
        REG32 in5_high_th_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_IN5_HIGH_TH_REG_T;

//p2_up_r_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_up_r_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_UP_R_REG_T;

//p2_up_g_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_up_g_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_UP_G_REG_T;

//p2_up_b_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_up_b_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_UP_B_REG_T;

//p2_down_r_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_down_r_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_DOWN_R_REG_T;

//p2_down_g_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_down_g_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_DOWN_G_REG_T;

//p2_down_b_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_down_b_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_DOWN_B_REG_T;

//p2_left_r_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_left_r_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_LEFT_R_REG_T;

//p2_left_g_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_left_g_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_LEFT_G_REG_T;

//p2_left_b_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_left_b_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_LEFT_B_REG_T;

//p2_right_r_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_right_r_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_RIGHT_R_REG_T;

//p2_right_g_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_right_g_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_RIGHT_G_REG_T;

//p2_right_b_reg
typedef union {
    REG32 v;
    struct {
        REG32 p2_right_b_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P2_RIGHT_B_REG_T;

//p4_q1_r_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q1_r_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q1_R_REG_T;

//p4_q1_g_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q1_g_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q1_G_REG_T;

//p4_q1_b_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q1_b_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q1_B_REG_T;

//p4_q2_r_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q2_r_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q2_R_REG_T;

//p4_q2_g_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q2_g_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q2_G_REG_T;

//p4_q2_b_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q2_b_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q2_B_REG_T;

//p4_q3_r_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q3_r_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q3_R_REG_T;

//p4_q3_g_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q3_g_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q3_G_REG_T;

//p4_q3_b_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q3_b_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q3_B_REG_T;

//p4_q4_r_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q4_r_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q4_R_REG_T;

//p4_q4_g_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q4_g_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q4_G_REG_T;

//p4_q4_b_reg
typedef union {
    REG32 v;
    struct {
        REG32 p4_q4_b_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_P4_Q4_B_REG_T;

//ae_e00_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e00_sta_line : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E00_STA_REG_T;

//ae_e00_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e00_num : 4; // [3:0]
        REG32 ae_e00_interval : 2; // [5:4]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E00_NUM_REG_T;

//ae_e01_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e01_sta_line : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E01_STA_REG_T;

//ae_e01_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e01_num : 4; // [3:0]
        REG32 ae_e01_interval : 3; // [6:4]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_AE_E01_NUM_REG_T;

//ae_e02_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e02_sta_line : 7; // [6:0]
        REG32 __31_7 : 25;
    } b;
} REG_CAMERA_AE_E02_STA_REG_T;

//ae_e02_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e02_num : 4; // [3:0]
        REG32 ae_e02_interval : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_E02_NUM_REG_T;

//ae_e1_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e1_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E1_STA_REG_T;

//ae_e1_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e1_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_E1_NUM_REG_T;

//ae_e2_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e2_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E2_STA_REG_T;

//ae_e2_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e2_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_E2_NUM_REG_T;

//ae_e3_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e3_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E3_STA_REG_T;

//ae_e3_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e3_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_E3_NUM_REG_T;

//ae_e4_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e4_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E4_STA_REG_T;

//ae_e4_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e4_num_reg : 5; // [4:0]
        REG32 __31_5 : 27;
    } b;
} REG_CAMERA_AE_E4_NUM_REG_T;

//ae_e5_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e5_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E5_STA_REG_T;

//ae_e5_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e5_num_reg : 5; // [4:0]
        REG32 __31_5 : 27;
    } b;
} REG_CAMERA_AE_E5_NUM_REG_T;

//ae_e6_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e6_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E6_STA_REG_T;

//ae_e6_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e6_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_E6_NUM_REG_T;

//ae_e7_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e7_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E7_STA_REG_T;

//ae_e7_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e7_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_E7_NUM_REG_T;

//ae_e8_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e8_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E8_STA_REG_T;

//ae_e8_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e8_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_E8_NUM_REG_T;

//ae_e9_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e9_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_E9_STA_REG_T;

//ae_e9_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_e9_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_E9_NUM_REG_T;

//ae_ea_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ea_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_EA_STA_REG_T;

//ae_ea_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ea_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_EA_NUM_REG_T;

//ae_eb_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_eb_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_EB_STA_REG_T;

//ae_eb_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_eb_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_EB_NUM_REG_T;

//ae_ec_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ec_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_EC_STA_REG_T;

//ae_ec_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ec_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_EC_NUM_REG_T;

//ae_ed_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ed_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_ED_STA_REG_T;

//ae_ed_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ed_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_ED_NUM_REG_T;

//bayer_gamma2_b0
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b0 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B0_T;

//bayer_gamma2_b1
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b1 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B1_T;

//bayer_gamma2_b2
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B2_T;

//bayer_gamma2_b3
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b3 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B3_T;

//bayer_gamma2_b4
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b4 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B4_T;

//bayer_gamma2_b6
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b6 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B6_T;

//bayer_gamma2_b8
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b8 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B8_T;

//bayer_gamma2_b10
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b10 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B10_T;

//bayer_gamma2_b12
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b12 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B12_T;

//bayer_gamma2_b16
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b16 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B16_T;

//bayer_gamma2_b20
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b20 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B20_T;

//bayer_gamma2_b24
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b24 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B24_T;

//bayer_gamma2_b28
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b28 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B28_T;

//bayer_gamma2_b32
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b32 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B32_T;

//bayer_gamma2_b36
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b36 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B36_T;

//bayer_gamma2_b40
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b40 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B40_T;

//bayer_gamma2_b48
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b48 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B48_T;

//bayer_gamma2_b56
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b56 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B56_T;

//bayer_gamma2_b64
typedef union {
    REG32 v;
    struct {
        REG32 bayer_gamma2_b64 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_BAYER_GAMMA2_B64_T;

//y_thr7_Lo_reg
typedef union {
    REG32 v;
    struct {
        REG32 y_thr7_lo_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_THR7_LO_REG_T;

//y_thr7_Mid_reg
typedef union {
    REG32 v;
    struct {
        REG32 y_thr7_mid_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_THR7_MID_REG_T;

//y_thr7_Hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 y_thr7_hi_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_THR7_HI_REG_T;

//dpa_new_ctrl_reg
typedef union {
    REG32 v;
    struct {
        REG32 inflg_ctrl_reg_0 : 1; // [0]
        REG32 inflg_ctrl_reg_1 : 1; // [1]
        REG32 inflg_ctrl_reg_2 : 1; // [2]
        REG32 __31_3 : 29;
    } b;
} REG_CAMERA_DPA_NEW_CTRL_REG_T;

//dpa_new_ctrl_hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 inflg_ctrl_reg0_h : 1; // [0]
        REG32 inflg_ctrl_reg1_h : 1; // [1]
        REG32 inflg_ctrl_reg2_h : 1; // [2]
        REG32 threshold_rsvd : 2; // [4:3]
        REG32 __31_5 : 27;
    } b;
} REG_CAMERA_DPA_NEW_CTRL_HI_REG_T;

//ae_index_gap
typedef union {
    REG32 v;
    struct {
        REG32 gap_2e : 1; // [0]
        REG32 gap_3e : 1; // [1]
        REG32 gap_4e : 1; // [2]
        REG32 gap_5e : 1; // [3]
        REG32 gap_6e : 1; // [4]
        REG32 gap_7e : 1; // [5]
        REG32 gap_8e : 1; // [6]
        REG32 gap_9e : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_INDEX_GAP_T;

//awb_calc_height_reg
typedef union {
    REG32 v;
    struct {
        REG32 awb_calc_height_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CALC_HEIGHT_REG_T;

//drc_r_clp_value_reg
typedef union {
    REG32 v;
    struct {
        REG32 drc_r_clp_value_reg : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_DRC_R_CLP_VALUE_REG_T;

//drc_gr_clp_value_reg
typedef union {
    REG32 v;
    struct {
        REG32 drc_gr_clp_value_reg : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_DRC_GR_CLP_VALUE_REG_T;

//drc_gb_clp_value_reg
typedef union {
    REG32 v;
    struct {
        REG32 drc_gb_clp_value_reg : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_DRC_GB_CLP_VALUE_REG_T;

//drc_b_clp_value_reg
typedef union {
    REG32 v;
    struct {
        REG32 drc_b_clp_value_reg : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_DRC_B_CLP_VALUE_REG_T;

//sepia_cr_reg
typedef union {
    REG32 v;
    struct {
        REG32 sepia_cr_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SEPIA_CR_REG_T;

//sepia_cb_reg
typedef union {
    REG32 v;
    struct {
        REG32 sepia_cb_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SEPIA_CB_REG_T;

//csup_y_min_hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 csup_y_min_hi_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CSUP_Y_MIN_HI_REG_T;

//csup_gain_hi_reg
typedef union {
    REG32 v;
    struct {
        REG32 csup_gain_hi_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CSUP_GAIN_HI_REG_T;

//csup_y_max_low_reg
typedef union {
    REG32 v;
    struct {
        REG32 csup_y_max_low_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CSUP_Y_MAX_LOW_REG_T;

//csup_gain_low_reg
typedef union {
    REG32 v;
    struct {
        REG32 csup_gain_low_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CSUP_GAIN_LOW_REG_T;

//ae_dk_hist_thr_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_dk_hist_thr_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_DK_HIST_THR_REG_T;

//ae_br_hist_thr_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_br_hist_thr_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_BR_HIST_THR_REG_T;

//hist_bp_level_reg
typedef union {
    REG32 v;
    struct {
        REG32 hist_bp_level_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_HIST_BP_LEVEL_REG_T;

//outdoor_th_reg
typedef union {
    REG32 v;
    struct {
        REG32 outdoor_th : 4; // [3:0]
        REG32 non_outdoor_th : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_OUTDOOR_TH_REG_T;

//awb_rgain_low_reg
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 awb_rgain_low_reg : 6; // [7:2]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_RGAIN_LOW_REG_T;

//awb_rgain_high_reg
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 awb_rgain_high_reg : 6; // [7:2]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_RGAIN_HIGH_REG_T;

//awb_bgain_low_reg
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 awb_bgain_low_reg : 6; // [7:2]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_BGAIN_LOW_REG_T;

//awb_bgain_high_reg
typedef union {
    REG32 v;
    struct {
        REG32 __1_0 : 2;
        REG32 awb_bgain_high_reg : 6; // [7:2]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_BGAIN_HIGH_REG_T;

//awb_calc_start_reg
typedef union {
    REG32 v;
    struct {
        REG32 awb_win_y_start : 4; // [3:0]
        REG32 awb_win_x_start : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CALC_START_REG_T;

//awb_calc_width_reg
typedef union {
    REG32 v;
    struct {
        REG32 awb_calc_width_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CALC_WIDTH_REG_T;

//hist_dp_level_reg
typedef union {
    REG32 v;
    struct {
        REG32 hist_dp_level_reg : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_HIST_DP_LEVEL_REG_T;

//awb_y_fmin
typedef union {
    REG32 v;
    struct {
        REG32 awb_y_fmin : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y_FMIN_T;

//awb_y_fmax
typedef union {
    REG32 v;
    struct {
        REG32 awb_y_fmax : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y_FMAX_T;

//awb_cb_fmin
typedef union {
    REG32 v;
    struct {
        REG32 awb_cb_fmin : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CB_FMIN_T;

//awb_cb_fmax
typedef union {
    REG32 v;
    struct {
        REG32 awb_cb_fmax : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CB_FMAX_T;

//awb_cr_fmin
typedef union {
    REG32 v;
    struct {
        REG32 awb_cr_fmin : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CR_FMIN_T;

//awb_cr_fmax
typedef union {
    REG32 v;
    struct {
        REG32 awb_cr_fmax : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CR_FMAX_T;

//awb_y_fmin2
typedef union {
    REG32 v;
    struct {
        REG32 awb_y_fmin2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y_FMIN2_T;

//awb_y_fmax2
typedef union {
    REG32 v;
    struct {
        REG32 awb_y_fmax2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y_FMAX2_T;

//awb_cb_fmin2
typedef union {
    REG32 v;
    struct {
        REG32 awb_cb_fmin2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CB_FMIN2_T;

//awb_cb_fmax2
typedef union {
    REG32 v;
    struct {
        REG32 awb_cb_fmax2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CB_FMAX2_T;

//awb_cr_fmin2
typedef union {
    REG32 v;
    struct {
        REG32 awb_cr_fmin2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CR_FMIN2_T;

//awb_cr_fmax2
typedef union {
    REG32 v;
    struct {
        REG32 awb_cr_fmax2 : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CR_FMAX2_T;

//ae_use_mean
typedef union {
    REG32 v;
    struct {
        REG32 ycave_use_mean : 2; // [1:0]
        REG32 ywave_use_mean : 2; // [3:2]
        REG32 yave_weight_mode : 1; // [4]
        REG32 nexp_out_sel_reg : 1; // [5]
        REG32 ae_ext_adj_val_reg : 1; // [6]
        REG32 ae_ext_adj_on_reg : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_USE_MEAN_T;

//ae_weight_sta
typedef union {
    REG32 v;
    struct {
        REG32 ywave_pcnt_left : 4; // [3:0]
        REG32 ywave_lcnt_top : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_WEIGHT_STA_T;

//ae_qWidth
typedef union {
    REG32 v;
    struct {
        REG32 qwidth : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_QWIDTH_T;

//ae_qHeight
typedef union {
    REG32 v;
    struct {
        REG32 qheight : 7; // [6:0]
        REG32 ywave_sel : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_QHEIGHT_T;

//ae_win_sta
typedef union {
    REG32 v;
    struct {
        REG32 yave_pcnt_sta : 4; // [3:0]
        REG32 yave_lcnt_sta : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_WIN_STA_T;

//ae_Width
typedef union {
    REG32 v;
    struct {
        REG32 width : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_WIDTH_T;

//ae_Height
typedef union {
    REG32 v;
    struct {
        REG32 height : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_HEIGHT_T;

//sw_update
typedef union {
    REG32 v;
    struct {
        REG32 cc_type_sw : 1; // [0]
        REG32 is_outdoor_sw : 1; // [1]
        REG32 gamma_type_sw : 1; // [2]
        REG32 sw_update_rsvd : 1; // [3]
        REG32 is_outdoor_mode : 3; // [6:4]
        REG32 awb_outdoor_en : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SW_UPDATE_T;

//awb_ctrl5
typedef union {
    REG32 v;
    struct {
        REG32 r_low_non_a : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CTRL5_T;

//awb_ctrl6
typedef union {
    REG32 v;
    struct {
        REG32 awb_stop_h : 4; // [3:0]
        REG32 awb_adj_again : 2; // [5:4]
        REG32 awb_algo_en : 1; // [6]
        REG32 check_r_low : 1; // [7]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_CTRL6_T;

//sca_reg
typedef union {
    REG32 v;
    struct {
        REG32 sca_mode : 3; // [2:0]
        REG32 __31_3 : 29;
    } b;
} REG_CAMERA_SCA_REG_T;

//ae_ee_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ee_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_EE_STA_REG_T;

//ae_ee_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ee_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_EE_NUM_REG_T;

//ae_ef_sta_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ef_sta_gain : 6; // [5:0]
        REG32 __31_6 : 26;
    } b;
} REG_CAMERA_AE_EF_STA_REG_T;

//ae_ef_num_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_ef_num_reg : 4; // [3:0]
        REG32 __31_4 : 28;
    } b;
} REG_CAMERA_AE_EF_NUM_REG_T;

//ae_thr_big_reg
typedef union {
    REG32 v;
    struct {
        REG32 ae_thr_big_dark : 4; // [3:0]
        REG32 ae_thr_big_bright : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AE_THR_BIG_REG_T;

//sharp_gain_minus_low
typedef union {
    REG32 v;
    struct {
        REG32 sharp_gain_minus_low : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_GAIN_MINUS_LOW_T;

//sharp_gain_minus_mid
typedef union {
    REG32 v;
    struct {
        REG32 sharp_gain_minus_mid : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_GAIN_MINUS_MID_T;

//sharp_gain_minus_hi
typedef union {
    REG32 v;
    struct {
        REG32 sharp_gain_minus_hi : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_GAIN_MINUS_HI_T;

//sharp_mode_mid_hi
typedef union {
    REG32 v;
    struct {
        REG32 sharp_cmp_gap_mid : 4; // [3:0]
        REG32 sharp_cmp_gap_hi : 4; // [7:4]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_SHARP_MODE_MID_HI_T;

//Fw_version_reg
typedef union {
    REG32 v;
    struct {
        REG32 fw_version : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_FW_VERSION_REG_T;

//awb_y_min_reg
typedef union {
    REG32 v;
    struct {
        REG32 awb_y_min : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_AWB_Y_MIN_REG_T;

//y_red_coef_reg
typedef union {
    REG32 v;
    struct {
        REG32 y_red_coef : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_RED_COEF_REG_T;

//y_blue_coef_reg
typedef union {
    REG32 v;
    struct {
        REG32 y_blue_coef : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_Y_BLUE_COEF_REG_T;

//cb_red_coef_reg
typedef union {
    REG32 v;
    struct {
        REG32 cb_red_coef : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CB_RED_COEF_REG_T;

//cr_blue_coef_reg
typedef union {
    REG32 v;
    struct {
        REG32 cr_blue_coef : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_CR_BLUE_COEF_REG_T;

//Hist_vbp_level_reg
typedef union {
    REG32 v;
    struct {
        REG32 hist_vbp_level : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_HIST_VBP_LEVEL_REG_T;

//Hist_vdp_level_reg
typedef union {
    REG32 v;
    struct {
        REG32 hist_vdp_level : 8; // [7:0]
        REG32 __31_8 : 24;
    } b;
} REG_CAMERA_HIST_VDP_LEVEL_REG_T;


//CTRL
#define CAMERA_ENABLE              (1<<0)
#define CAMERA_ENABLE_ENABLE       (1<<0)
#define CAMERA_ENABLE_DISABLE      (0<<0)
#define CAMERA_ENABLE_V_ENABLE     (1)
#define CAMERA_ENABLE_V_DISABLE    (0)
#define CAMERA_DCTENABLE           (1<<1)
#define CAMERA_BUF_ENABLE          (1<<2)
#define CAMERA_RGB_RFIRST          (1<<3)
#define CAMERA_DATAFORMAT(n)       (((n)&3)<<4)
#define CAMERA_DATAFORMAT_RGB565   (0<<4)
#define CAMERA_DATAFORMAT_YUV422   (1<<4)
#define CAMERA_DATAFORMAT_JPEG     (2<<4)
#define CAMERA_DATAFORMAT_RESERVE  (3<<4)
#define CAMERA_DATAFORMAT_V_RGB565 (0)
#define CAMERA_DATAFORMAT_V_YUV422 (1)
#define CAMERA_DATAFORMAT_V_JPEG   (2)
#define CAMERA_DATAFORMAT_V_RESERVE (3)
#define CAMERA_CFG_CAM_C2CSE(n)    (((n)&3)<<6)
#define CAMERA_RESET_POL           (1<<8)
#define CAMERA_RESET_POL_INVERT    (1<<8)
#define CAMERA_RESET_POL_NORMAL    (0<<8)
#define CAMERA_RESET_POL_V_INVERT  (1)
#define CAMERA_RESET_POL_V_NORMAL  (0)
#define CAMERA_PWDN_POL            (1<<9)
#define CAMERA_PWDN_POL_INVERT     (1<<9)
#define CAMERA_PWDN_POL_NORMAL     (0<<9)
#define CAMERA_PWDN_POL_V_INVERT   (1)
#define CAMERA_PWDN_POL_V_NORMAL   (0)
#define CAMERA_VSYNC_POL           (1<<10)
#define CAMERA_VSYNC_POL_INVERT    (1<<10)
#define CAMERA_VSYNC_POL_NORMAL    (0<<10)
#define CAMERA_VSYNC_POL_V_INVERT  (1)
#define CAMERA_VSYNC_POL_V_NORMAL  (0)
#define CAMERA_HREF_POL            (1<<11)
#define CAMERA_HREF_POL_INVERT     (1<<11)
#define CAMERA_HREF_POL_NORMAL     (0<<11)
#define CAMERA_HREF_POL_V_INVERT   (1)
#define CAMERA_HREF_POL_V_NORMAL   (0)
#define CAMERA_PIXCLK_POL          (1<<12)
#define CAMERA_PIXCLK_POL_INVERT   (1<<12)
#define CAMERA_PIXCLK_POL_NORMAL   (0<<12)
#define CAMERA_PIXCLK_POL_V_INVERT (1)
#define CAMERA_PIXCLK_POL_V_NORMAL (0)
#define CAMERA_VSYNC_DROP          (1<<14)
#define CAMERA_VSYNC_DROP_DROP     (1<<14)
#define CAMERA_VSYNC_DROP_NORMAL   (0<<14)
#define CAMERA_VSYNC_DROP_V_DROP   (1)
#define CAMERA_VSYNC_DROP_V_NORMAL (0)
#define CAMERA_DECIMFRM(n)         (((n)&3)<<16)
#define CAMERA_DECIMFRM_ORIGINAL   (0<<16)
#define CAMERA_DECIMFRM_DIV_2      (1<<16)
#define CAMERA_DECIMFRM_DIV_3      (2<<16)
#define CAMERA_DECIMFRM_DIV_4      (3<<16)
#define CAMERA_DECIMFRM_V_ORIGINAL (0)
#define CAMERA_DECIMFRM_V_DIV_2    (1)
#define CAMERA_DECIMFRM_V_DIV_3    (2)
#define CAMERA_DECIMFRM_V_DIV_4    (3)
#define CAMERA_DECIMCOL(n)         (((n)&3)<<18)
#define CAMERA_DECIMCOL_ORIGINAL   (0<<18)
#define CAMERA_DECIMCOL_DIV_2      (1<<18)
#define CAMERA_DECIMCOL_DIV_3      (2<<18)
#define CAMERA_DECIMCOL_DIV_4      (3<<18)
#define CAMERA_DECIMCOL_V_ORIGINAL (0)
#define CAMERA_DECIMCOL_V_DIV_2    (1)
#define CAMERA_DECIMCOL_V_DIV_3    (2)
#define CAMERA_DECIMCOL_V_DIV_4    (3)
#define CAMERA_DECIMROW(n)         (((n)&3)<<20)
#define CAMERA_DECIMROW_ORIGINAL   (0<<20)
#define CAMERA_DECIMROW_DIV_2      (1<<20)
#define CAMERA_DECIMROW_DIV_3      (2<<20)
#define CAMERA_DECIMROW_DIV_4      (3<<20)
#define CAMERA_DECIMROW_V_ORIGINAL (0)
#define CAMERA_DECIMROW_V_DIV_2    (1)
#define CAMERA_DECIMROW_V_DIV_3    (2)
#define CAMERA_DECIMROW_V_DIV_4    (3)
#define CAMERA_REORDER(n)          (((n)&7)<<24)
#define CAMERA_CROPEN              (1<<28)
#define CAMERA_CROPEN_ENABLE       (1<<28)
#define CAMERA_CROPEN_DISABLE      (0<<28)
#define CAMERA_CROPEN_V_ENABLE     (1)
#define CAMERA_CROPEN_V_DISABLE    (0)
#define CAMERA_BIST_MODE           (1<<30)
#define CAMERA_BIST_MODE_BIST      (1<<30)
#define CAMERA_BIST_MODE_NORMAL    (0<<30)
#define CAMERA_BIST_MODE_V_BIST    (1)
#define CAMERA_BIST_MODE_V_NORMAL  (0)
#define CAMERA_TEST                (1<<31)
#define CAMERA_TEST_TEST           (1<<31)
#define CAMERA_TEST_NORMAL         (0<<31)
#define CAMERA_TEST_V_TEST         (1)
#define CAMERA_TEST_V_NORMAL       (0)

//STATUS
#define CAMERA_OVFL                (1<<0)
#define CAMERA_VSYNC_R             (1<<1)
#define CAMERA_VSYNC_F             (1<<2)
#define CAMERA_DMA_DONE            (1<<3)
#define CAMERA_FIFO_EMPTY          (1<<4)
#define CAMERA_SPI_OVFL            (1<<5)

//DATA
#define CAMERA_RX_DATA(n)          (((n)&0xFFFFFFFF)<<0)

//IRQ_MASK
//#define CAMERA_OVFL              (1<<0)
//#define CAMERA_VSYNC_R           (1<<1)
//#define CAMERA_VSYNC_F           (1<<2)
//#define CAMERA_DMA_DONE          (1<<3)

//IRQ_CLEAR
//#define CAMERA_OVFL              (1<<0)
//#define CAMERA_VSYNC_R           (1<<1)
//#define CAMERA_VSYNC_F           (1<<2)
//#define CAMERA_DMA_DONE          (1<<3)

//IRQ_CAUSE
//#define CAMERA_OVFL              (1<<0)
//#define CAMERA_VSYNC_R           (1<<1)
//#define CAMERA_VSYNC_F           (1<<2)
//#define CAMERA_DMA_DONE          (1<<3)

//CMD_SET
#define CAMERA_PWDN                (1<<0)
#define CAMERA_RESET               (1<<4)
#define CAMERA_FIFO_RESET          (1<<8)

//CMD_CLR
//#define CAMERA_PWDN              (1<<0)
//#define CAMERA_RESET             (1<<4)

//DSTWINCOL
#define CAMERA_DSTWINCOLSTART(n)   (((n)&0xFFF)<<0)
#define CAMERA_DSTWINCOLEND(n)     (((n)&0xFFF)<<16)

//DSTWINRow
#define CAMERA_DSTWINROWSTART(n)   (((n)&0xFFF)<<0)
#define CAMERA_DSTWINROWEND(n)     (((n)&0xFFF)<<16)

//SCL_CONFIG
#define CAMERA_SCALE_EN            (1<<0)
#define CAMERA_DATA_OUT_SWAP       (1<<4)
#define CAMERA_SCALE_COL(n)        (((n)&3)<<8)
#define CAMERA_SCALE_ROW(n)        (((n)&3)<<16)

//SPI_CAMERA_REG0
#define CAMERA_CAMERA_SPI_SLAVE_EN (1<<0)
#define CAMERA_CAMERA_SPI_MASTER_EN (1<<1)
#define CAMERA_YUV_OUT_FORMAT(n)   (((n)&7)<<2)
#define CAMERA_OVERFLOW_RSTN_ONLY_VSYNC_LOW (1<<5)
#define CAMERA_OVERFLOW_OBSERVE_ONLY_VSYNC_LOW (1<<6)
#define CAMERA_OVERFLOW_RSTN_EN    (1<<7)
#define CAMERA_BIG_END_DIS         (1<<8)
#define CAMERA_OVERFLOW_INV        (1<<9)
#define CAMERA_HREF_INV            (1<<10)
#define CAMERA_VSYNC_INV           (1<<11)
#define CAMERA_BLOCK_NUM_PER_LINE(n) (((n)&0x3FF)<<12)
#define CAMERA_LINE_NUM_PER_FRAME(n) (((n)&0x3FF)<<22)

//SPI_CAMERA_REG1
#define CAMERA_CAMERA_CLK_DIV_NUM(n) (((n)&0xFFFF)<<0)
#define CAMERA_CTS_SPI_MASTER_REG  (1<<16)
#define CAMERA_SSN_CM_INV          (1<<17)
#define CAMERA_SCK_CM_INV          (1<<18)
#define CAMERA_SSN_SPI_OENB_DR     (1<<19)
#define CAMERA_SSN_SPI_OENB_REG    (1<<20)
#define CAMERA_SCK_SPI_OENB_DR     (1<<21)
#define CAMERA_SCK_SPI_OENB_REG    (1<<22)
#define CAMERA_SDO_SPI_SWAP        (1<<29)
#define CAMERA_CLK_INV             (1<<30)
#define CAMERA_SCK_DDR_EN          (1<<31)

//SPI_CAMERA_REG2
#define CAMERA_SSN_WAIT_LENGTH(n)  (((n)&0xFF)<<0)
#define CAMERA_INIT_WAIT_LENGTH(n) (((n)&0xFF)<<8)
#define CAMERA_WORD_NUM_PER_BLOCK(n) (((n)&0xFF)<<16)
#define CAMERA_SSN_CS_DELAY(n)     (((n)&3)<<24)
#define CAMERA_DATA_RECEIVE_CHOOSE_BIT(n) (((n)&3)<<26)
#define CAMERA_READY_CS_INV        (1<<28)
#define CAMERA_SSN_CS_INV          (1<<29)

//SPI_CAMERA_REG3
#define CAMERA_LINE_WAIT_LENGTH(n) (((n)&0xFFFF)<<0)
#define CAMERA_BLOCK_WAIT_LENGTH(n) (((n)&0xFF)<<16)
#define CAMERA_SSN_HIGH_LENGTH(n)  (((n)&0xFF)<<24)

//SPI_CAMERA_REG4
#define CAMERA_CAMERA_SPI_MASTER_EN_2 (1<<0)
#define CAMERA_SDO_LINE_CHOOSE_BIT(n) (((n)&3)<<1)
#define CAMERA_DATA_SIZE_CHOOSE_BIT (1<<3)
#define CAMERA_IMAGE_HEIGHT_CHOOSE_BIT (1<<4)
#define CAMERA_IMAGE_WIDTH_CHOOSE_BIT (1<<5)
#define CAMERA_BLOCK_NUM_PER_PACKET(n) (((n)&0x3FF)<<6)

//SPI_CAMERA_REG5
#define CAMERA_SYNC_CODE(n)        (((n)&0xFFFFFF)<<0)

//SPI_CAMERA_REG6
#define CAMERA_PACKET_ID_DATA_START(n) (((n)&0xFF)<<0)
#define CAMERA_PACKET_ID_LINE_START(n) (((n)&0xFF)<<8)
#define CAMERA_PACKET_ID_FRAME_END(n) (((n)&0xFF)<<16)
#define CAMERA_PACKET_ID_FRAME_START(n) (((n)&0xFF)<<24)

//SPI_CAMERA_OBS0
#define CAMERA_LINE_ID_15_0_(n)    (((n)&0xFFFF)<<0)
#define CAMERA_DATA_ID_7_0_(n)     (((n)&0xFF)<<16)
#define CAMERA_OBSERVE_DATA_SIZE_WRONG (1<<24)
#define CAMERA_OBSERVE_IMAGE_HEIGHT_WRONG (1<<25)
#define CAMERA_OBSERVE_IMAGE_WIDTH_WRONG (1<<26)
#define CAMERA_OBSERVE_LINE_NUM_WRONG (1<<27)
#define CAMERA_OBSERVE_DATA_ID_WRONG (1<<28)

//SPI_CAMERA_OBS1
#define CAMERA_IMAGE_HEIGHT(n)     (((n)&0xFFFF)<<0)
#define CAMERA_IMAGE_WIDTH(n)      (((n)&0xFFFF)<<16)

//CSI_CONFIG_REG0
#define CAMERA_NUM_D_TERM_EN(n)    (((n)&0xFF)<<0)
#define CAMERA_CUR_FRAME_LINE_NUM(n) (((n)&0x1FFF)<<8)
#define CAMERA_DATA_LP_IN_CHOOSE_BIT(n) (((n)&3)<<21)
#define CAMERA_CLK_LP_INV          (1<<23)
#define CAMERA_TRAIL_DATA_WRONG_CHOOSE_BIT (1<<24)
#define CAMERA_SYNC_BYPASS         (1<<25)
#define CAMERA_RDATA_BIT_INV_EN    (1<<26)
#define CAMERA_HS_SYNC_FIND_EN     (1<<27)
#define CAMERA_LINE_PACKET_ENABLE  (1<<28)
#define CAMERA_ECC_BYPASS          (1<<29)
#define CAMERA_DATA_LANE_CHOOSE_BIT (1<<30)
#define CAMERA_CSI_MODULE_ENABLE   (1<<31)

//CSI_CONFIG_REG1
#define CAMERA_NUM_HS_SETTLE(n)    (((n)&0xFF)<<0)
#define CAMERA_LP_DATA_LENGTH_CHOOSE_BIT(n) (((n)&7)<<8)
#define CAMERA_DATA_CLK_LP_POSEDGE_CHOOSE(n) (((n)&7)<<11)
#define CAMERA_CLK_LP_CK_INV       (1<<14)
#define CAMERA_RCLR_MASK_EN        (1<<15)
#define CAMERA_RINC_MASK_EN        (1<<16)
#define CAMERA_HS_ENABLE_MASK_EN   (1<<17)
#define CAMERA_DEN_CSI_INV_BIT     (1<<18)
#define CAMERA_HSYNC_CSI_INV_BIT   (1<<19)
#define CAMERA_VSYNC_CSI_INV_BIT   (1<<20)
#define CAMERA_HS_DATA2_ENABLE_REG (1<<21)
#define CAMERA_HS_DATA1_ENABLE_REG (1<<22)
#define CAMERA_HS_DATA1_ENABLE_CHOOSE_BIT (1<<23)
#define CAMERA_HS_DATA1_ENABLE_DR  (1<<24)
#define CAMERA_DATA2_TERMINAL_ENABLE_REG (1<<25)
#define CAMERA_DATA1_TERMINAL_ENABLE_REG (1<<26)
#define CAMERA_DATA1_TERMINAL_ENABLE_DR (1<<27)
#define CAMERA_LP_DATA_INTERRUPT_CLR (1<<28)
#define CAMERA_LP_CMD_INTERRUPT_CLR (1<<29)
#define CAMERA_LP_DATA_CLR         (1<<30)
#define CAMERA_LP_CMD_CLR          (1<<31)

//CSI_CONFIG_REG2
#define CAMERA_NUM_HS_SETTLE_CLK(n) (((n)&0xFFFF)<<0)
#define CAMERA_NUM_C_TERM_EN(n)    (((n)&0xFFFF)<<16)

//CSI_CONFIG_REG3
#define CAMERA_CLK_LP_IN_CHOOSE_BIT(n) (((n)&3)<<6)
#define CAMERA_PU_LPRX_REG         (1<<8)
#define CAMERA_PU_HSRX_REG         (1<<9)
#define CAMERA_PU_DR               (1<<10)
#define CAMERA_DATA_PNSW_REG       (1<<11)
#define CAMERA_HS_CLK_ENABLE_REG   (1<<12)
#define CAMERA_HS_CLK_ENABLE_CHOOSE_BIT (1<<13)
#define CAMERA_HS_CLK_ENABLE_DR    (1<<14)
#define CAMERA_CLK_TERMINAL_ENABLE_REG (1<<15)
#define CAMERA_CLK_TERMINAL_ENABLE_DR (1<<16)
#define CAMERA_OBSERVE_REG_5_LOW8_CHOOSE (1<<17)
#define CAMERA_ECC_ERROR_FLAG_REG  (1<<18)
#define CAMERA_ECC_ERROR_DR        (1<<19)
#define CAMERA_CSI_CHANNEL_SEL     (1<<20)
#define CAMERA_TWO_LANE_BIT_REVERSE (1<<21)
#define CAMERA_DATA2_LANE_BIT_REVERSE (1<<22)
#define CAMERA_DATA1_LANE_BIT_REVERSE (1<<23)
#define CAMERA_DATA2_HS_NO_MASK    (1<<24)
#define CAMERA_DATA1_HS_NO_MASK    (1<<25)
#define CAMERA_PU_LPRX_D2_REG      (1<<26)
#define CAMERA_PU_LPRX_D1_REG      (1<<27)
#define CAMERA_CLK_EDGE_SEL        (1<<29)
#define CAMERA_CLK_X2_SEL          (1<<30)
#define CAMERA_SINGLE_DATA_LANE_EN (1<<31)

//CSI_CONFIG_REG4
#define CAMERA_NUM_HS_CLK_USEFUL(n) (((n)&0x7FFFFFFF)<<0)
#define CAMERA_NUM_HS_CLK_USEFUL_EN (1<<31)

//CSI_CONFIG_REG5
#define CAMERA_VC_ID_SET(n)        (((n)&3)<<0)
#define CAMERA_DATA_LP_INV         (1<<2)
#define CAMERA_FIFO_RCLR_8809P_REG (1<<3)
#define CAMERA_FIFO_WCLR_8809P_REG (1<<4)
#define CAMERA_HS_SYNC_16BIT_8809P_MODE (1<<5)
#define CAMERA_D_TERM_SMALL_8809P_EN (1<<6)
#define CAMERA_DATA_LINE_INV_8809P_EN (1<<7)
#define CAMERA_HS_ENABLE_8809P_MODE (1<<8)
#define CAMERA_SP_TO_TRAIL_8809P_EN (1<<9)
#define CAMERA_TRAIL_WRONG_8809P_BYPASS (1<<10)
#define CAMERA_RINC_TRAIL_8809P_BYPASS (1<<11)
#define CAMERA_HS_DATA_ENABLE_8809P_MODE (1<<12)
#define CAMERA_HS_CLK_ENABLE_8809P_MODE (1<<13)
#define CAMERA_DATA_TYPE_RE_CHECK_EN (1<<14)
#define CAMERA_SYNC_ID_REG(n)      (((n)&0xFF)<<15)
#define CAMERA_SYNC_ID_DR          (1<<23)
#define CAMERA_CSI_OBSERVE_CHOOSE_BIT(n) (((n)&31)<<24)
#define CAMERA_CRC_ERROR_FLAG_REG  (1<<29)
#define CAMERA_CRC_ERROR_FLAG_DR   (1<<30)
#define CAMERA_CSI_RINC_NEW_MODE_DIS (1<<31)

//CSI_CONFIG_REG6
#define CAMERA_DATA_TYPE_DP_REG(n) (((n)&0x3F)<<0)
#define CAMERA_DATA_TYPE_LE_REG(n) (((n)&0x3F)<<6)
#define CAMERA_DATA_TYPE_LS_REG(n) (((n)&0x3F)<<12)
#define CAMERA_DATA_TYPE_FE_REG(n) (((n)&0x3F)<<18)
#define CAMERA_DATA_TYPE_FS_REG(n) (((n)&0x3F)<<24)
#define CAMERA_DATA_TYPE_DP_DR     (1<<30)
#define CAMERA_DATA_TYPE_DR        (1<<31)

//CSI_CONFIG_REG7
#define CAMERA_DATA_LANE_16BITS_MODE (1<<2)
#define CAMERA_TERMINAL_2_HS_EXCHAGE_8809P (1<<3)
#define CAMERA_TERMINAL_1_HS_EXCHAGE_8809P (1<<4)
#define CAMERA_DATA2_TERMINAL_ENABLE_8809P_DR (1<<5)
#define CAMERA_HS_DATA2_ENABLE_8809P_DR (1<<6)
#define CAMERA_CSI_DOUT_TEST_8809P_EN (1<<7)
#define CAMERA_CSI_DOUT_TEST_8809P(n) (((n)&0xFF)<<8)
#define CAMERA_NUM_D_TERM_EN_X1(n) (((n)&0xFF)<<16)
#define CAMERA_NUM_HS_SETTLE_X1(n) (((n)&0xFF)<<24)

//CSI_OBS4
#define CAMERA_HS_DATA_STATE(n)    (((n)&0x3FFF)<<0)
#define CAMERA_PHY_DATA_STATE(n)   (((n)&0x7FFF)<<14)
#define CAMERA_FIFO_WFULL_ALMOST   (1<<29)
#define CAMERA_FIFO_WFULL          (1<<30)
#define CAMERA_FIFO_WEMPTY         (1<<31)

//CSI_OBS5
#define CAMERA_CSI_OBSERVE_REG_5_LOW(n) (((n)&0xFF)<<0)
#define CAMERA_LP_DATA_INTERRUPT_FLAG (1<<8)
#define CAMERA_LP_CMD_INTERRUPT_FLAG (1<<9)
#define CAMERA_PHY_CLK_STATE(n)    (((n)&0x1FF)<<10)
#define CAMERA_FIFO_RCOUNT(n)      (((n)&0x1FF)<<19)
#define CAMERA_CRC_ERROR           (1<<28)
#define CAMERA_ERR_ECC_CORRECTED_FLAG (1<<29)
#define CAMERA_ERR_DATA_CORRECTED_FLAG (1<<30)
#define CAMERA_ERR_DATA_ZERO_FLAG  (1<<31)

//CSI_OBS6
#define CAMERA_CSI_OBSERVE_REG_6(n) (((n)&0xFFFFFFFF)<<0)

//CSI_OBS7
#define CAMERA_CSI_OBSERVE_REG_7(n) (((n)&0xFFFFFFFF)<<0)

//CSI_ENABLE
#define CAMERA_CSI_ENABLE          (1<<0)

//CSI_CONFIG_REG8
#define CAMERA_DLY_SEL_CLKN_REG(n) (((n)&15)<<0)
#define CAMERA_DLY_SEL_CLKP_REG(n) (((n)&15)<<4)
#define CAMERA_DLY_SEL_DATA2_REG(n) (((n)&15)<<8)
#define CAMERA_DLY_SEL_DATA1_REG(n) (((n)&15)<<12)
#define CAMERA_VTH_SEL             (1<<16)

//RAMData
#define CAMERA_DATA(n)             (((n)&0xFFFFFFFF)<<0)
#define CAMERA_DATA_MASK           (0xFFFFFFFF<<0)
#define CAMERA_DATA_SHIFT          (0)

//soft_reset
#define CAMERA_DSP_RESET           (1<<0)

//awb_x1_min
#define CAMERA_AWB_X1_MIN(n)       (((n)&0xFF)<<0)

//awb_x1_max
#define CAMERA_AWB_X1_MAX(n)       (((n)&0xFF)<<0)

//awb_y1_min
#define CAMERA_AWB_Y1_MIN(n)       (((n)&0xFF)<<0)

//awb_y1_max
#define CAMERA_AWB_Y1_MAX(n)       (((n)&0xFF)<<0)

//awb_x2_min
#define CAMERA_AWB_X2_MIN(n)       (((n)&0xFF)<<0)

//awb_x2_max
#define CAMERA_AWB_X2_MAX(n)       (((n)&0xFF)<<0)

//awb_y2_min
#define CAMERA_AWB_Y2_MIN(n)       (((n)&0xFF)<<0)

//awb_y2_max
#define CAMERA_AWB_Y2_MAX(n)       (((n)&0xFF)<<0)

//awb_x3_min
#define CAMERA_AWB_X3_MIN(n)       (((n)&0xFF)<<0)

//awb_x3_max
#define CAMERA_AWB_X3_MAX(n)       (((n)&0xFF)<<0)

//awb_y3_min
#define CAMERA_AWB_Y3_MIN(n)       (((n)&0xFF)<<0)

//awb_y3_max
#define CAMERA_AWB_Y3_MAX(n)       (((n)&0xFF)<<0)

//awb_x4_min
#define CAMERA_AWB_X4_MIN(n)       (((n)&0xFF)<<0)

//awb_x4_max
#define CAMERA_AWB_X4_MAX(n)       (((n)&0xFF)<<0)

//awb_y4_min
#define CAMERA_AWB_Y4_MIN(n)       (((n)&0xFF)<<0)

//awb_y4_max
#define CAMERA_AWB_Y4_MAX(n)       (((n)&0xFF)<<0)

//awb_x5_min
#define CAMERA_AWB_X5_MIN(n)       (((n)&0xFF)<<0)

//awb_x5_max
#define CAMERA_AWB_X5_MAX(n)       (((n)&0xFF)<<0)

//awb_y5_min
#define CAMERA_AWB_Y5_MIN(n)       (((n)&0xFF)<<0)

//awb_y5_max
#define CAMERA_AWB_Y5_MAX(n)       (((n)&0xFF)<<0)

//awb_skin_x1_min
#define CAMERA_AWB_SKIN_X1_MIN(n)  (((n)&0xFF)<<0)

//awb_skin_x1_max
#define CAMERA_AWB_SKIN_X1_MAX(n)  (((n)&0xFF)<<0)

//awb_skin_y1_min
#define CAMERA_AWB_SKIN_Y1_MIN(n)  (((n)&0xFF)<<0)

//awb_skin_y1_max
#define CAMERA_AWB_SKIN_Y1_MAX(n)  (((n)&0xFF)<<0)

//awb_skin_x2_min
#define CAMERA_AWB_SKIN_X2_MIN(n)  (((n)&0xFF)<<0)

//awb_skin_x2_max
#define CAMERA_AWB_SKIN_X2_MAX(n)  (((n)&0xFF)<<0)

//awb_skin_y2_min
#define CAMERA_AWB_SKIN_Y2_MIN(n)  (((n)&0xFF)<<0)

//awb_skin_y2_max
#define CAMERA_AWB_SKIN_Y2_MAX(n)  (((n)&0xFF)<<0)

//awb_ctd_msb
#define CAMERA_AWB_X1_MIN_MSB      (1<<0)
#define CAMERA_AWB_X1_MAX_MSB      (1<<1)
#define CAMERA_AWB_Y5_MIN_MSB      (1<<2)
#define CAMERA_AWB_Y5_MAX_MSB      (1<<3)
#define CAMERA_AWB_ADJ_MODE(n)     (((n)&3)<<4)
#define CAMERA_AWB_RATIO_MODE(n)   (((n)&3)<<6)

//Int_dif_thr_mid
#define CAMERA_INT_DIF_THR_MID(n)  (((n)&0xFF)<<0)

//lb_soft_rstn
#define CAMERA_LB_SOFT_RSTN        (1<<0)

//vsync_end_high
#define CAMERA_VSYNC_END_HIGH(n)   (((n)&0xFF)<<0)

//vsync_end_low
#define CAMERA_VSYNC_END_LOW(n)    (((n)&0xFF)<<0)

//line_numL
#define CAMERA_LINE_NUML(n)        (((n)&0xFF)<<0)

//pix_numL
#define CAMERA_PIX_NUML(n)         (((n)&0xFF)<<0)

//pix_line_numH
#define CAMERA_LINE_NUMH           (1<<0)
#define CAMERA_PIX_NUMH_RSVD(n)    (((n)&7)<<1)
#define CAMERA_PIX_NUMH(n)         (((n)&3)<<4)
#define CAMERA_LINE_NUMH_RSVD(n)   (((n)&3)<<6)

//lb_ctrl
#define CAMERA_LOW_ORDER           (1<<0)
#define CAMERA_USE_FB_REG          (1<<1)
#define CAMERA_NOT_CVP_REG         (1<<2)
#define CAMERA_FIRST_BYTE_REG(n)   (((n)&7)<<3)

//data_format
#define CAMERA_DATA_FORMAT(n)      (((n)&3)<<0)

//lb_enable
#define CAMERA_LB_ENABLE           (1<<0)

//vh_inv
#define CAMERA_HSYNC_INV           (1<<0)
#define CAMERA_VSYNC_INV_X1        (1<<1)

//line_cnt_L
#define CAMERA_LINE_CNT_L(n)       (((n)&0xFF)<<0)

//line_cnt_H
#define CAMERA_LINE_CNT_H(n)       (((n)&3)<<0)

//num_check
#define CAMERA_LINE_NUM_CHECK      (1<<0)
#define CAMERA_BYTE_NUM_CHECK      (1<<1)
#define CAMERA_LINE_NUM_CLEAR      (1<<4)
#define CAMERA_BYTE_NUM_CLEAR      (1<<5)

//dci_ctrl_reg
#define CAMERA_KL_LOW_LIGHT_FIX    (1<<0)
#define CAMERA_KL_REG_FIX          (1<<1)
#define CAMERA_KU_LOW_LIGHT_FIX    (1<<2)
#define CAMERA_KU_REG_FIX          (1<<3)
#define CAMERA_HOFST(n)            (((n)&3)<<4)
#define CAMERA_VBH_SEL(n)          (((n)&3)<<6)

//dci_ofst_reg
#define CAMERA_KL_OFSTX1(n)        (((n)&15)<<0)
#define CAMERA_KU_OFSTX1(n)        (((n)&15)<<4)

//dci_hist_reg
#define CAMERA_DK_HISTX1(n)        (((n)&15)<<0)
#define CAMERA_BR_HISTX1(n)        (((n)&15)<<4)

//ae_sw_ctrl_reg
#define CAMERA_NEXP_SW_IN(n)       (((n)&15)<<0)
#define CAMERA_AE_EXT_ADJ_START    (1<<7)

//ae_thr_reg
#define CAMERA_THR_DARK(n)         (((n)&15)<<0)
#define CAMERA_THR_BRIGHT(n)       (((n)&15)<<4)

//ae_misc_ctrl_reg
#define CAMERA_OFST_DEC_LOW_SEL(n) (((n)&3)<<0)
#define CAMERA_OFST_DEC_HIGH_SEL(n) (((n)&3)<<2)
#define CAMERA_FORCE_ADJ1          (1<<4)
#define CAMERA_FORCE_ADJ2          (1<<5)
#define CAMERA_FORCE_ADJ3          (1<<6)
#define CAMERA_INDEX_OFST_NO_STEP  (1<<7)

//csup_xx_reg
#define CAMERA_X_LOW(n)            (((n)&15)<<0)
#define CAMERA_X_HIGH(n)           (((n)&15)<<4)

//contr_ythr_reg
#define CAMERA_CSUP_GAIN_LOW_TH_H  (1<<0)
#define CAMERA_CSUP_GAIN_HIGH_TH(n) (((n)&7)<<1)
#define CAMERA_FIXED_CONTR_YTHR(n) (((n)&15)<<4)

//contr_yave_offset_reg
#define CAMERA_YAVE_OFFSET_REG(n)  (((n)&0x3F)<<0)
#define CAMERA_YTHR_SEL            (1<<6)
#define CAMERA_YAVE_OFFSET_SIGN    (1<<7)

//contr_ku_Lo_reg
#define CAMERA_KU(n)               (((n)&0x7F)<<0)
#define CAMERA_KU_SIGN             (1<<7)

//contr_kl_Lo_reg
#define CAMERA_KL(n)               (((n)&0x7F)<<0)
#define CAMERA_KL_SIGN             (1<<7)

//contr_ku_Mid_reg
//#define CAMERA_KU(n)             (((n)&0x7F)<<0)
//#define CAMERA_KU_SIGN           (1<<7)

//contr_kl_Mid_reg
//#define CAMERA_KL(n)             (((n)&0x7F)<<0)
//#define CAMERA_KL_SIGN           (1<<7)

//contr_ku_Hi_reg
//#define CAMERA_KU(n)             (((n)&0x7F)<<0)
//#define CAMERA_KU_SIGN           (1<<7)

//contr_kl_Hi_reg
//#define CAMERA_KL(n)             (((n)&0x7F)<<0)
//#define CAMERA_KL_SIGN           (1<<7)

//luma_offset_Lo_reg
#define CAMERA_OFFSET(n)           (((n)&0x3F)<<0)
#define CAMERA_ALGO_SEL            (1<<6)
#define CAMERA_OFFSET_SIGN         (1<<7)

//luma_offset_Mid_reg
//#define CAMERA_OFFSET(n)         (((n)&0x3F)<<0)
//#define CAMERA_ALGO_SEL          (1<<6)
//#define CAMERA_OFFSET_SIGN       (1<<7)

//luma_offset_Hi_reg
//#define CAMERA_OFFSET(n)         (((n)&0x3F)<<0)
//#define CAMERA_ALGO_SEL          (1<<6)
//#define CAMERA_OFFSET_SIGN       (1<<7)

//u_gain_Lo_reg
#define CAMERA_U_GAIN_LO_REG(n)    (((n)&0xFF)<<0)

//v_gain_Lo_reg
#define CAMERA_V_GAIN_LO_REG(n)    (((n)&0xFF)<<0)

//u_gain_Mid_reg
#define CAMERA_U_GAIN_MID_REG(n)   (((n)&0xFF)<<0)

//v_gain_Mid_reg
#define CAMERA_V_GAIN_MID_REG(n)   (((n)&0xFF)<<0)

//u_gain_Hi_reg
#define CAMERA_U_GAIN_HI_REG(n)    (((n)&0xFF)<<0)

//v_gain_Hi_reg
#define CAMERA_V_GAIN_HI_REG(n)    (((n)&0xFF)<<0)

//again_sel_th0_reg
#define CAMERA_CONTR_GAIN_LOW_TH(n) (((n)&7)<<0)
#define CAMERA_AGAIN_SEL_TH0_RSVD  (1<<3)
#define CAMERA_CONTR_GAIN_HI_TH(n) (((n)&7)<<4)

//awb_cc_type_ctrl_reg
#define CAMERA_CC_TYPE_MODE(n)     (((n)&15)<<0)
#define CAMERA_CC_GAIN_HI_TH(n)    (((n)&7)<<4)
#define CAMERA_LUMA_FIRST          (1<<7)

//awb_cc_type_th_reg
#define CAMERA_R_BIG_TH(n)         (((n)&15)<<0)
#define CAMERA_B_BIG_TH(n)         (((n)&15)<<4)

//isp_wrapper_ctrl_1
#define CAMERA_POUT_MODE(n)        (((n)&3)<<0)
#define CAMERA_YUV_MODE(n)         (((n)&3)<<2)
#define CAMERA_VSYNC_TOGGLE        (1<<4)
#define CAMERA_MIPI_RSTN           (1<<5)
#define CAMERA_HSYNC_FIX           (1<<6)

//top_dummy
#define CAMERA_TOP_DUMMY(n)        (((n)&0x7F)<<0)

//left_dummy
#define CAMERA_LEFT_DUMMY(n)       (((n)&0xFF)<<0)

//isp_wrapper_ctrl_2
#define CAMERA_RGB_MODE_REG(n)     (((n)&7)<<0)
#define CAMERA_SUB_MODE            (1<<3)
#define CAMERA_MON_MODE_REG        (1<<4)
#define CAMERA_OCLK_INV_REG        (1<<5)
#define CAMERA_ISP_OUT_EN          (1<<6)

//line_num_l_reg
#define CAMERA_LINE_NUM_L_REG(n)   (((n)&0x3F)<<0)

//pix_num_l_reg
#define CAMERA_PIX_NUM_L_REG(n)    (((n)&0x7F)<<0)
#define CAMERA_CSI_MON_REG         (1<<7)

//v_dummy
#define CAMERA_VBOT_DUMMY_REG(n)   (((n)&15)<<0)
#define CAMERA_VTOP_DUMMY_REG(n)   (((n)&15)<<4)

//scg
#define CAMERA_KUKL_SEL            (1<<0)
#define CAMERA_REG94_RD_SEL        (1<<1)
#define CAMERA_BAYER_OUT_SEL       (1<<2)
#define CAMERA_CSUP_EN             (1<<3)
#define CAMERA_Y_GAMMA_EN(n)       (((n)&3)<<4)
#define CAMERA_YUV_SDI_EN          (1<<6)
#define CAMERA_REG92_RD_SEL        (1<<7)

//y_gamma_b0
#define CAMERA_Y_GAMMA_B0(n)       (((n)&0xFF)<<0)

//y_gamma_b1
#define CAMERA_Y_GAMMA_B1(n)       (((n)&0xFF)<<0)

//y_gamma_b2
#define CAMERA_Y_GAMMA_B2(n)       (((n)&0xFF)<<0)

//y_gamma_b4
#define CAMERA_Y_GAMMA_B4(n)       (((n)&0xFF)<<0)

//y_gamma_b6
#define CAMERA_Y_GAMMA_B6(n)       (((n)&0xFF)<<0)

//y_gamma_b8
#define CAMERA_Y_GAMMA_B8(n)       (((n)&0xFF)<<0)

//y_gamma_b10
#define CAMERA_Y_GAMMA_B10(n)      (((n)&0xFF)<<0)

//y_gamma_b12
#define CAMERA_Y_GAMMA_B12(n)      (((n)&0xFF)<<0)

//y_gamma_b16
#define CAMERA_Y_GAMMA_B16(n)      (((n)&0xFF)<<0)

//y_gamma_b20
#define CAMERA_Y_GAMMA_B20(n)      (((n)&0xFF)<<0)

//y_gamma_b24
#define CAMERA_Y_GAMMA_B24(n)      (((n)&0xFF)<<0)

//y_gamma_b28
#define CAMERA_Y_GAMMA_B28(n)      (((n)&0xFF)<<0)

//y_gamma_b32
#define CAMERA_Y_GAMMA_B32(n)      (((n)&0xFF)<<0)

//r_awb_gain_in
#define CAMERA_R_AWB_GAIN_IN(n)    (((n)&0xFF)<<0)

//g_awb_gain_in
#define CAMERA_G_AWB_GAIN_IN(n)    (((n)&0xFF)<<0)

//b_awb_gain_in
#define CAMERA_B_AWB_GAIN_IN(n)    (((n)&0xFF)<<0)

//r_drc_gain_in
#define CAMERA_R_DRC_GAIN_IN(n)    (((n)&0xFF)<<0)

//gr_drc_gain_in
#define CAMERA_GR_DRC_GAIN_IN(n)   (((n)&0xFF)<<0)

//gb_drc_gain_in
#define CAMERA_GB_DRC_GAIN_IN(n)   (((n)&0xFF)<<0)

//b_drc_gain_in
#define CAMERA_B_DRC_GAIN_IN(n)    (((n)&0xFF)<<0)

//ae_ctrl
#define CAMERA_ANA_GAIN_IN(n)      (((n)&0x3F)<<0)
#define CAMERA_AE_UPDATE_EN        (1<<6)
#define CAMERA_AE_EN               (1<<7)

//ae_ctrl2
#define CAMERA_AWB_ADJ_SEL(n)      (((n)&3)<<0)
#define CAMERA_GAP_AE              (1<<2)
#define CAMERA_GAP_BE              (1<<3)
#define CAMERA_AE_ACTION_PERIOD(n) (((n)&7)<<4)
#define CAMERA_YAVE_MON_SEL        (1<<7)

//ae_ctrl3
#define CAMERA_YAVE_USE_MEAN(n)    (((n)&3)<<0)
#define CAMERA_YAVE_DIFF_THR_REG(n) (((n)&3)<<2)
#define CAMERA_YAVE_SEL(n)         (((n)&3)<<4)
#define CAMERA_YAVE_PLUS_BH_MODE   (1<<6)
#define CAMERA_YWAVE_PLUS_BH_MODE  (1<<7)

//ae_ctrl4
#define CAMERA_AE_HIST_BIG_EN      (1<<0)
#define CAMERA_AE_HIST_TOO_BIG_EN  (1<<1)
#define CAMERA_HIST_OFST0(n)       (((n)&3)<<2)
#define CAMERA_INDEX_OFST0(n)      (((n)&3)<<4)
#define CAMERA_INDEX_OFST1(n)      (((n)&3)<<6)

//ae_win_start
#define CAMERA_PCNT_LEFT(n)        (((n)&15)<<0)
#define CAMERA_LCNT_TOP(n)         (((n)&15)<<4)

//ae_win_width
#define CAMERA_AE_WIN_WIDTH(n)     (((n)&0xFF)<<0)

//ae_win_height
#define CAMERA_AE_WIN_HEIGHT(n)    (((n)&0xFF)<<0)

//exp_init
#define CAMERA_EXP_INIT(n)         (((n)&0xFF)<<0)

//exp_ceil_init
#define CAMERA_EXP_CEIL_INIT(n)    (((n)&15)<<0)

//ae_exp_1E
#define CAMERA_AE_EXP_1E(n)        (((n)&0xFF)<<0)

//ae_diff_thr
#define CAMERA_THR2_DARK(n)        (((n)&15)<<0)
#define CAMERA_THR2_BRIGHT(n)      (((n)&15)<<4)

//ae_bh_sel
#define CAMERA_BH_FACTOR_INDOOR(n) (((n)&7)<<0)
#define CAMERA_BH_FACTOR_OUTDOOR(n) (((n)&7)<<3)
#define CAMERA_BH_MEAN_SEL(n)      (((n)&3)<<6)

//awb_ctrl
#define CAMERA_AWB_SW_MON_EN       (1<<0)
#define CAMERA_FAST_2X             (1<<1)
#define CAMERA_FAST_4X             (1<<2)
#define CAMERA_AWB_ACTION_PERIOD(n) (((n)&7)<<3)
#define CAMERA_AWB_UPDATE_EN       (1<<6)
#define CAMERA_AWB_EN              (1<<7)

//awb_ctrl2
#define CAMERA_AWB_MON_SEL(n)      (((n)&7)<<0)
#define CAMERA_AWB_VLD_SEL         (1<<3)
#define CAMERA_AWB_VLD_MODE(n)     (((n)&7)<<4)
#define CAMERA_AWB_ADJ             (1<<7)

//awb_y_max
#define CAMERA_AWB_Y_MAX(n)        (((n)&0xFF)<<0)

//awb_stop
#define CAMERA_AWB_STOP_CB_NEG_LEVEL(n) (((n)&3)<<0)
#define CAMERA_AWB_STOP_CB_POS_LEVEL(n) (((n)&3)<<2)
#define CAMERA_AWB_STOP_CR_NEG_LEVEL(n) (((n)&3)<<4)
#define CAMERA_AWB_STOP_CR_POS_LEVEL(n) (((n)&3)<<6)

//awb_algo
#define CAMERA_AWB_ALGO(n)         (((n)&0xFF)<<0)

//awb_ctrl3
#define CAMERA_CR_OFST_LT1X        (1<<0)
#define CAMERA_CR_OFST_GT1X        (1<<1)
#define CAMERA_CB_OFST_LT1X        (1<<2)
#define CAMERA_CB_OFST_GT1X        (1<<3)
#define CAMERA_AWB_SUM_VLD_SEL     (1<<4)
#define CAMERA_AWB_STOP_SEL_REG    (1<<5)
#define CAMERA_AWB_SKIN_SEL        (1<<6)
#define CAMERA_AWB_ALGO_MODE       (1<<7)

//awb_ctrl4
#define CAMERA_AWB_CTRL4(n)        (((n)&0xFF)<<0)

//dig_gain_in
#define CAMERA_DIG_GAIN_IN(n)      (((n)&0xFF)<<0)

//Y_init_thr
#define CAMERA_Y_INIT_MODE         (1<<0)
#define CAMERA_Y_LOW_EN            (1<<1)
#define CAMERA_Y_HIGH_EN           (1<<2)
#define CAMERA_Y_LOW_THR(n)        (((n)&31)<<3)

//y_ave_target
#define CAMERA_Y_AVE_TARGET(n)     (((n)&0xFF)<<0)

//y_lmt_offset
#define CAMERA_Y_LOW_LIMIT(n)      (((n)&7)<<0)
#define CAMERA_Y_LMT_OFST          (1<<3)
#define CAMERA_Y_HIGH_LIMIT(n)     (((n)&7)<<4)

//again_sel_th2
#define CAMERA_YNR_GAIN_LOW_TH(n)  (((n)&7)<<0)
#define CAMERA_AGAIN_SEL_TH2       (1<<3)
#define CAMERA_YNR_GAIN_HI_TH(n)   (((n)&7)<<4)

//yave_target_chg1
#define CAMERA_YAVE_TARGET_OFST_L(n) (((n)&15)<<0)
#define CAMERA_YAVE_TARGET_OFST_H(n) (((n)&15)<<4)

//image_eff_reg
#define CAMERA_GREY_EN             (1<<0)
#define CAMERA_SEPIA_EN            (1<<1)
#define CAMERA_NEGATIVE_EN         (1<<2)
#define CAMERA_COLOR_BAR_EN        (1<<3)
#define CAMERA_IMAGE_EFF_RSVD      (1<<4)
#define CAMERA_REG93_SEL           (1<<5)
#define CAMERA_REG94_SEL           (1<<6)
#define CAMERA_SHARP_MON           (1<<7)

//Ywave_out
#define CAMERA_YWAVE_OUT(n)        (((n)&0xFF)<<0)

//ae_bright_hist
#define CAMERA_AE_BRIGHT_HIST(n)   (((n)&0xFF)<<0)

//yave_out
#define CAMERA_YAVE_OUT(n)         (((n)&0xFF)<<0)

//exp_out
#define CAMERA_EXP_OUT(n)          (((n)&0xFF)<<0)

//misc_out
#define CAMERA_EXP_OUT_H(n)        (((n)&7)<<0)
#define CAMERA_AWB_OK              (1<<3)
#define CAMERA_NEXP_SEL(n)         (((n)&3)<<4)
#define CAMERA_FIXED_0             (1<<6)
#define CAMERA_AE_OK               (1<<7)

//awb_debug_out
#define CAMERA_AWB_CRGT(n)         (((n)&3)<<0)
#define CAMERA_AWB_CBGT(n)         (((n)&3)<<2)
#define CAMERA_AWB_CRSUM_SIGN      (1<<4)
#define CAMERA_AWB_CBSUM_SIGN      (1<<5)
#define CAMERA_AWB_CBCR            (1<<6)
#define CAMERA_AWB_SUM_VLD         (1<<7)

//mono_color
#define CAMERA_MONO_COLOR(n)       (((n)&0xFF)<<0)

//r_awb_gain
#define CAMERA_R_AWB_GAIN(n)       (((n)&0xFF)<<0)

//b_awb_gain
#define CAMERA_B_AWB_GAIN(n)       (((n)&0xFF)<<0)

//Misc_status
#define CAMERA_ANA_GAIN_OUT(n)     (((n)&0x3F)<<0)
#define CAMERA_CC_TYPE             (1<<6)
#define CAMERA_IS_OUTDOOR          (1<<7)

//Yave_contr
#define CAMERA_YAVE_CONTR(n)       (((n)&0xFF)<<0)

//Gamma_type
#define CAMERA_GAMMA_TYPE_MODE(n)  (((n)&7)<<0)
#define CAMERA_GAMMA_GAIN_HI_TH(n) (((n)&7)<<3)
#define CAMERA_VGAS(n)             (((n)&3)<<6)

//blc_line
#define CAMERA_BLC_LINE(n)         (((n)&0xFF)<<0)

//lsc_xx
//#define CAMERA_X_LOW(n)          (((n)&15)<<0)
//#define CAMERA_X_HIGH(n)         (((n)&15)<<4)

//lsc_blc_gain_th
#define CAMERA_LSC_GAIN_LOW_TH(n)  (((n)&7)<<0)
#define CAMERA_LSC_GAIN_HI_TH(n)   (((n)&7)<<3)
#define CAMERA_CSUP_GAIN_LOW_TH(n) (((n)&3)<<6)

//blc_ctrl
#define CAMERA_BLC_OUT_MODE(n)     (((n)&3)<<0)
#define CAMERA_LINE_INIT_H         (1<<2)
#define CAMERA_BLC_OFST_SIGN       (1<<3)
#define CAMERA_BLC_MODE(n)         (((n)&3)<<4)
#define CAMERA_BLC_SEL             (1<<6)
#define CAMERA_BLC_EN              (1<<7)

//blc_init
#define CAMERA_BLC00_OFST(n)       (((n)&15)<<0)
#define CAMERA_BLC01_OFST(n)       (((n)&15)<<4)

//blc_offset
#define CAMERA_BLC10_OFST(n)       (((n)&15)<<0)
#define CAMERA_BLC11_OFST(n)       (((n)&15)<<4)

//blc_thr
#define CAMERA_BLC_THR(n)          (((n)&0x3F)<<0)

//lsc_xy_cent
#define CAMERA_Y_CENT(n)           (((n)&15)<<0)
#define CAMERA_X_CENT(n)           (((n)&15)<<4)

//cnr_dif_thr
#define CAMERA_CNR_V_EN            (1<<0)
#define CAMERA_CNR_H_EN            (1<<1)
#define CAMERA_VCNR_SEL            (1<<2)
#define CAMERA_EDGE_MON            (1<<3)
#define CAMERA_AWB_SKIN_MODE(n)    (((n)&7)<<4)
#define CAMERA_GAMMA_TYPE          (1<<7)

//cnr_thr
#define CAMERA_CNR_THR_V(n)        (((n)&7)<<0)
#define CAMERA_EDGE_EN_V           (1<<3)
#define CAMERA_CNR_THR_H(n)        (((n)&7)<<4)
#define CAMERA_EDGE_EN_H           (1<<7)

//gamma_ctrl
#define CAMERA_GAMMA_P_ID          (1<<0)
#define CAMERA_GAMMA_L_ID          (1<<1)
#define CAMERA_GAMMA_EN_NON_OUTDOOR (1<<2)
#define CAMERA_GAMMA_EN_OUTDOOR    (1<<3)
#define CAMERA_LSC_P_ID            (1<<4)
#define CAMERA_LSC_L_ID            (1<<5)
#define CAMERA_LSC_EN_NON_OUTDOOR  (1<<6)
#define CAMERA_LSC_EN_OUTDOOR      (1<<7)

//bayer_gamma_b0
#define CAMERA_BAYER_GAMMA_B0(n)   (((n)&0xFF)<<0)

//bayer_gamma_b1
#define CAMERA_BAYER_GAMMA_B1(n)   (((n)&0xFF)<<0)

//bayer_gamma_b2
#define CAMERA_BAYER_GAMMA_B2(n)   (((n)&0xFF)<<0)

//bayer_gamma_b3
#define CAMERA_BAYER_GAMMA_B3(n)   (((n)&0xFF)<<0)

//bayer_gamma_b4
#define CAMERA_BAYER_GAMMA_B4(n)   (((n)&0xFF)<<0)

//bayer_gamma_b6
#define CAMERA_BAYER_GAMMA_B6(n)   (((n)&0xFF)<<0)

//bayer_gamma_b8
#define CAMERA_BAYER_GAMMA_B8(n)   (((n)&0xFF)<<0)

//bayer_gamma_b10
#define CAMERA_BAYER_GAMMA_B10(n)  (((n)&0xFF)<<0)

//bayer_gamma_b12
#define CAMERA_BAYER_GAMMA_B12(n)  (((n)&0xFF)<<0)

//bayer_gamma_b16
#define CAMERA_BAYER_GAMMA_B16(n)  (((n)&0xFF)<<0)

//bayer_gamma_b20
#define CAMERA_BAYER_GAMMA_B20(n)  (((n)&0xFF)<<0)

//bayer_gamma_b24
#define CAMERA_BAYER_GAMMA_B24(n)  (((n)&0xFF)<<0)

//bayer_gamma_b28
#define CAMERA_BAYER_GAMMA_B28(n)  (((n)&0xFF)<<0)

//bayer_gamma_b32
#define CAMERA_BAYER_GAMMA_B32(n)  (((n)&0xFF)<<0)

//bayer_gamma_b36
#define CAMERA_BAYER_GAMMA_B36(n)  (((n)&0xFF)<<0)

//bayer_gamma_b40
#define CAMERA_BAYER_GAMMA_B40(n)  (((n)&0xFF)<<0)

//bayer_gamma_b48
#define CAMERA_BAYER_GAMMA_B48(n)  (((n)&0xFF)<<0)

//bayer_gamma_b56
#define CAMERA_BAYER_GAMMA_B56(n)  (((n)&0xFF)<<0)

//bayer_gamma_b64
#define CAMERA_BAYER_GAMMA_B64(n)  (((n)&0xFF)<<0)

//blc_out0
#define CAMERA_BLC_OUT0(n)         (((n)&0xFF)<<0)

//blc_out1
#define CAMERA_BLC_OUT1(n)         (((n)&0xFF)<<0)

//dpc_ctrl_0
#define CAMERA_DPC_ON              (1<<0)
#define CAMERA_ADP_MED_SEL         (1<<1)
#define CAMERA_ANA_GAIN_CMP(n)     (((n)&3)<<2)
#define CAMERA_RSVD                (1<<4)
#define CAMERA_NRF_GAUS_SEL        (1<<5)
#define CAMERA_BAYER_NR_ON         (1<<6)
#define CAMERA_CC_ON               (1<<7)

//dpc_ctrl_1
#define CAMERA_INT_FLG_CMP(n)      (((n)&3)<<0)
#define CAMERA_ABS_SIGN_ALL_CMP(n) (((n)&3)<<2)
#define CAMERA_INT_DIF_SEL         (1<<4)

//y_thr_Lo
#define CAMERA_Y_THR_LO(n)         (((n)&0xFF)<<0)

//y_thr_Mid
#define CAMERA_Y_THR_MID(n)        (((n)&0xFF)<<0)

//y_thr_Hi
#define CAMERA_Y_THR_HI(n)         (((n)&0xFF)<<0)

//intp_cfa_hv
#define CAMERA_CFA_V_THR_L(n)      (((n)&7)<<0)
#define CAMERA_RSVD1               (1<<3)
#define CAMERA_CFA_H_THR_L(n)      (((n)&7)<<4)
#define CAMERA_RSVD2               (1<<7)

//manual_adj
#define CAMERA_B_GAIN_ADJ          (1<<0)
#define CAMERA_G_GAIN_ADJ          (1<<1)
#define CAMERA_R_GAIN_ADJ          (1<<2)
#define CAMERA_ANA_GAIN_ADJ        (1<<3)
#define CAMERA_ADJ_DIRECTION       (1<<4)
#define CAMERA_INDEX_MANUAL_ADJ    (1<<5)
#define CAMERA_IN_CAPTURE_AWB      (1<<6)
#define CAMERA_IN_CAPTURE_AE       (1<<7)

//dpc_int_thr_Lo
#define CAMERA_DPC_INT_THR_LO(n)   (((n)&0xFF)<<0)

//dpc_int_thr_Hi
#define CAMERA_DPC_INT_THR_HI(n)   (((n)&0xFF)<<0)

//again_sel_th1
#define CAMERA_BNR_GAIN_LOW_TH(n)  (((n)&7)<<0)
#define CAMERA_AGAIN_SEL_TH1_RSVD  (1<<3)
#define CAMERA_BNR_GAIN_HI_TH(n)   (((n)&7)<<4)

//dpc_nr_lf_str_Lo
#define CAMERA_DPC_NR_LF_STR_LO(n) (((n)&0xFF)<<0)

//dpc_nr_hf_str_Lo
#define CAMERA_DPC_NR_HF_STR_LO(n) (((n)&0xFF)<<0)

//dpc_nr_area_thr_Lo
#define CAMERA_DPC_NR_AREA_THR_LO(n) (((n)&0xFF)<<0)

//dpc_nr_lf_str_Mid
#define CAMERA_DPC_NR_LF_STR_MID(n) (((n)&0xFF)<<0)

//dpc_nr_hf_str_Mid
#define CAMERA_DPC_NR_HF_STR_MID(n) (((n)&0xFF)<<0)

//dpc_nr_area_thr_Mid
#define CAMERA_DPC_NR_AREA_THR_MID(n) (((n)&0xFF)<<0)

//dpc_nr_lf_str_Hi
#define CAMERA_DPC_NR_LF_STR_HI(n) (((n)&0xFF)<<0)

//dpc_nr_hf_str_Hi
#define CAMERA_DPC_NR_HF_STR_HI(n) (((n)&0xFF)<<0)

//dpc_nr_area_thr_Hi
#define CAMERA_DPC_NR_AREA_THR_HI(n) (((n)&0xFF)<<0)

//intp_ctrl
#define CAMERA_PID_INV_EN          (1<<0)
#define CAMERA_LID_INV_EN          (1<<1)
#define CAMERA_GFILTER_EN          (1<<2)
#define CAMERA_GFILTER3_EN         (1<<3)
#define CAMERA_GFLITER5_EN         (1<<4)
#define CAMERA_SORT_SEL(n)         (((n)&7)<<5)

//intp_cfa_h_thr
#define CAMERA_INTP_CFA_H_THR(n)   (((n)&0xFF)<<0)

//intp_cfa_v_thr
#define CAMERA_INTP_CFA_V_THR(n)   (((n)&0xFF)<<0)

//intp_grgb_sel_lmt
#define CAMERA_INTP_GRGB_SEL_LMT(n) (((n)&0xFF)<<0)

//intp_gf_lmt_thr
#define CAMERA_INTP_GF_LMT_THR(n)  (((n)&0xFF)<<0)

//cc_r_offset
#define CAMERA_CC_R_OFFSET(n)      (((n)&0xFF)<<0)

//cc_g_offset
#define CAMERA_CC_G_OFFSET(n)      (((n)&0xFF)<<0)

//cc_b_offset
#define CAMERA_CC_B_OFFSET(n)      (((n)&0xFF)<<0)

//cc_00
#define CAMERA_CC_00(n)            (((n)&0xFF)<<0)

//cc_01
#define CAMERA_CC_01(n)            (((n)&0xFF)<<0)

//cc_10
#define CAMERA_CC_10(n)            (((n)&0xFF)<<0)

//cc_11
#define CAMERA_CC_11(n)            (((n)&0xFF)<<0)

//cc_20
#define CAMERA_CC_20(n)            (((n)&0xFF)<<0)

//cc_21
#define CAMERA_CC_21(n)            (((n)&0xFF)<<0)

//cc_R_offset_post
#define CAMERA_CC_R_OFFSET_POST(n) (((n)&0xFF)<<0)

//cc_G_offset_post
#define CAMERA_CC_G_OFFSET_POST(n) (((n)&0xFF)<<0)

//cc_B_offset_post
#define CAMERA_CC_B_OFFSET_POST(n) (((n)&0xFF)<<0)

//cc2_r_offset
#define CAMERA_CC2_R_OFFSET(n)     (((n)&0xFF)<<0)

//cc2_g_offset
#define CAMERA_CC2_G_OFFSET(n)     (((n)&0xFF)<<0)

//cc2_b_offset
#define CAMERA_CC2_B_OFFSET(n)     (((n)&0xFF)<<0)

//cc2_00
#define CAMERA_CC2_00(n)           (((n)&0xFF)<<0)

//cc2_01
#define CAMERA_CC2_01(n)           (((n)&0xFF)<<0)

//cc2_10
#define CAMERA_CC2_10(n)           (((n)&0xFF)<<0)

//cc2_11
#define CAMERA_CC2_11(n)           (((n)&0xFF)<<0)

//cc2_20
#define CAMERA_CC2_20(n)           (((n)&0xFF)<<0)

//cc2_21
#define CAMERA_CC2_21(n)           (((n)&0xFF)<<0)

//sharp_lmt
#define CAMERA_SHARP_LMT(n)        (((n)&0x7F)<<0)
#define CAMERA_SHARP_FINAL_H       (1<<7)

//sharp_mode
#define CAMERA_SHARP_CMP_GAP_LO(n) (((n)&15)<<0)
#define CAMERA_SHARP_FINAL(n)      (((n)&3)<<4)
#define CAMERA_SHARP_SEL           (1<<6)
#define CAMERA_RGB_TEST_PATTERN    (1<<7)

//sharp_gain_str_Lo
#define CAMERA_SHARP_GAIN_STR_LO(n) (((n)&0xFF)<<0)

//sharp_nr_area_thr_Lo
#define CAMERA_SHARP_NR_AREA_THR_LO(n) (((n)&0x7F)<<0)

//sharp_gain_str_Mid
#define CAMERA_SHARP_GAIN_STR_MID(n) (((n)&0xFF)<<0)

//sharp_nr_area_thr_Mid
#define CAMERA_SHARP_NR_AREA_THR_MID(n) (((n)&0x7F)<<0)

//sharp_gain_str_Hi
#define CAMERA_SHARP_GAIN_STR_HI(n) (((n)&0xFF)<<0)

//sharp_nr_area_thr_Hi
#define CAMERA_SHARP_NR_AREA_THR_HI(n) (((n)&0x7F)<<0)

//ynr_ctrl_reg
#define CAMERA_YNR_ON              (1<<0)
#define CAMERA_YNR_EDGE_METHODE(n) (((n)&3)<<1)
#define CAMERA_SHARP_ON            (1<<3)
#define CAMERA_SHARP_PLUS_MODE(n)  (((n)&3)<<4)
#define CAMERA_Y_AE_SEL(n)         (((n)&3)<<6)

//ynr_lf_method_str
#define CAMERA_YNR_LF_METHOD_STR(n) (((n)&0xFF)<<0)

//ynr_lf_str_Lo
#define CAMERA_YNR_LF_STR_LO(n)    (((n)&0xFF)<<0)

//ynr_hf_str_Lo
#define CAMERA_YNR_HF_STR_LO(n)    (((n)&0xFF)<<0)

//ynr_area_thr_Lo
#define CAMERA_YNR_AREA_THR_LO(n)  (((n)&0xFF)<<0)

//ynr_lf_str_Mid
#define CAMERA_YNR_LF_STR_MID(n)   (((n)&0xFF)<<0)

//ynr_hf_str_Mid
#define CAMERA_YNR_HF_STR_MID(n)   (((n)&0xFF)<<0)

//ynr_area_thr_Mid
#define CAMERA_YNR_AREA_THR_MID(n) (((n)&0xFF)<<0)

//ynr_lf_str_Hi
#define CAMERA_YNR_LF_STR_HI(n)    (((n)&0xFF)<<0)

//ynr_hf_str_Hi
#define CAMERA_YNR_HF_STR_HI(n)    (((n)&0xFF)<<0)

//ynr_area_thr_Hi
#define CAMERA_YNR_AREA_THR_HI(n)  (((n)&0xFF)<<0)

//Hue_sin_reg
#define CAMERA_HUE_SIN_REG(n)      (((n)&0xFF)<<0)

//Hue_cos_reg
#define CAMERA_HUE_COSX_REG(n)     (((n)&0x7F)<<0)
#define CAMERA_SIN_SIGN_REG        (1<<7)

//cnr_1d_ctrl_reg
#define CAMERA_CNR_DIF_THR_MID(n)  (((n)&15)<<0)
#define CAMERA_CNR_1D_ON           (1<<4)
#define CAMERA_SATUR_ON            (1<<5)
#define CAMERA_HUE_ON              (1<<6)

//cnr_xx_reg
#define CAMERA_CNR_DIF_THR_LOW(n)  (((n)&15)<<0)
#define CAMERA_CNR_DIF_THR_HIGH(n) (((n)&15)<<4)

//in5_low_th_reg
#define CAMERA_IN5_LOW_TH_REG(n)   (((n)&0xFF)<<0)

//in5_high_th_reg
#define CAMERA_IN5_HIGH_TH_REG(n)  (((n)&0xFF)<<0)

//p2_up_r_reg
#define CAMERA_P2_UP_R_REG(n)      (((n)&0xFF)<<0)

//p2_up_g_reg
#define CAMERA_P2_UP_G_REG(n)      (((n)&0xFF)<<0)

//p2_up_b_reg
#define CAMERA_P2_UP_B_REG(n)      (((n)&0xFF)<<0)

//p2_down_r_reg
#define CAMERA_P2_DOWN_R_REG(n)    (((n)&0xFF)<<0)

//p2_down_g_reg
#define CAMERA_P2_DOWN_G_REG(n)    (((n)&0xFF)<<0)

//p2_down_b_reg
#define CAMERA_P2_DOWN_B_REG(n)    (((n)&0xFF)<<0)

//p2_left_r_reg
#define CAMERA_P2_LEFT_R_REG(n)    (((n)&0xFF)<<0)

//p2_left_g_reg
#define CAMERA_P2_LEFT_G_REG(n)    (((n)&0xFF)<<0)

//p2_left_b_reg
#define CAMERA_P2_LEFT_B_REG(n)    (((n)&0xFF)<<0)

//p2_right_r_reg
#define CAMERA_P2_RIGHT_R_REG(n)   (((n)&0xFF)<<0)

//p2_right_g_reg
#define CAMERA_P2_RIGHT_G_REG(n)   (((n)&0xFF)<<0)

//p2_right_b_reg
#define CAMERA_P2_RIGHT_B_REG(n)   (((n)&0xFF)<<0)

//p4_q1_r_reg
#define CAMERA_P4_Q1_R_REG(n)      (((n)&0xFF)<<0)

//p4_q1_g_reg
#define CAMERA_P4_Q1_G_REG(n)      (((n)&0xFF)<<0)

//p4_q1_b_reg
#define CAMERA_P4_Q1_B_REG(n)      (((n)&0xFF)<<0)

//p4_q2_r_reg
#define CAMERA_P4_Q2_R_REG(n)      (((n)&0xFF)<<0)

//p4_q2_g_reg
#define CAMERA_P4_Q2_G_REG(n)      (((n)&0xFF)<<0)

//p4_q2_b_reg
#define CAMERA_P4_Q2_B_REG(n)      (((n)&0xFF)<<0)

//p4_q3_r_reg
#define CAMERA_P4_Q3_R_REG(n)      (((n)&0xFF)<<0)

//p4_q3_g_reg
#define CAMERA_P4_Q3_G_REG(n)      (((n)&0xFF)<<0)

//p4_q3_b_reg
#define CAMERA_P4_Q3_B_REG(n)      (((n)&0xFF)<<0)

//p4_q4_r_reg
#define CAMERA_P4_Q4_R_REG(n)      (((n)&0xFF)<<0)

//p4_q4_g_reg
#define CAMERA_P4_Q4_G_REG(n)      (((n)&0xFF)<<0)

//p4_q4_b_reg
#define CAMERA_P4_Q4_B_REG(n)      (((n)&0xFF)<<0)

//ae_e00_sta_reg
#define CAMERA_AE_E00_STA_LINE(n)  (((n)&0x3F)<<0)

//ae_e00_num_reg
#define CAMERA_AE_E00_NUM(n)       (((n)&15)<<0)
#define CAMERA_AE_E00_INTERVAL(n)  (((n)&3)<<4)

//ae_e01_sta_reg
#define CAMERA_AE_E01_STA_LINE(n)  (((n)&0x3F)<<0)

//ae_e01_num_reg
#define CAMERA_AE_E01_NUM(n)       (((n)&15)<<0)
#define CAMERA_AE_E01_INTERVAL(n)  (((n)&7)<<4)

//ae_e02_sta_reg
#define CAMERA_AE_E02_STA_LINE(n)  (((n)&0x7F)<<0)

//ae_e02_num_reg
#define CAMERA_AE_E02_NUM(n)       (((n)&15)<<0)
#define CAMERA_AE_E02_INTERVAL(n)  (((n)&15)<<4)

//ae_e1_sta_reg
#define CAMERA_AE_E1_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e1_num_reg
#define CAMERA_AE_E1_NUM_REG(n)    (((n)&15)<<0)

//ae_e2_sta_reg
#define CAMERA_AE_E2_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e2_num_reg
#define CAMERA_AE_E2_NUM_REG(n)    (((n)&15)<<0)

//ae_e3_sta_reg
#define CAMERA_AE_E3_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e3_num_reg
#define CAMERA_AE_E3_NUM_REG(n)    (((n)&15)<<0)

//ae_e4_sta_reg
#define CAMERA_AE_E4_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e4_num_reg
#define CAMERA_AE_E4_NUM_REG(n)    (((n)&31)<<0)

//ae_e5_sta_reg
#define CAMERA_AE_E5_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e5_num_reg
#define CAMERA_AE_E5_NUM_REG(n)    (((n)&31)<<0)

//ae_e6_sta_reg
#define CAMERA_AE_E6_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e6_num_reg
#define CAMERA_AE_E6_NUM_REG(n)    (((n)&15)<<0)

//ae_e7_sta_reg
#define CAMERA_AE_E7_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e7_num_reg
#define CAMERA_AE_E7_NUM_REG(n)    (((n)&15)<<0)

//ae_e8_sta_reg
#define CAMERA_AE_E8_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e8_num_reg
#define CAMERA_AE_E8_NUM_REG(n)    (((n)&15)<<0)

//ae_e9_sta_reg
#define CAMERA_AE_E9_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_e9_num_reg
#define CAMERA_AE_E9_NUM_REG(n)    (((n)&15)<<0)

//ae_ea_sta_reg
#define CAMERA_AE_EA_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_ea_num_reg
#define CAMERA_AE_EA_NUM_REG(n)    (((n)&15)<<0)

//ae_eb_sta_reg
#define CAMERA_AE_EB_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_eb_num_reg
#define CAMERA_AE_EB_NUM_REG(n)    (((n)&15)<<0)

//ae_ec_sta_reg
#define CAMERA_AE_EC_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_ec_num_reg
#define CAMERA_AE_EC_NUM_REG(n)    (((n)&15)<<0)

//ae_ed_sta_reg
#define CAMERA_AE_ED_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_ed_num_reg
#define CAMERA_AE_ED_NUM_REG(n)    (((n)&15)<<0)

//bayer_gamma2_b0
#define CAMERA_BAYER_GAMMA2_B0(n)  (((n)&0xFF)<<0)

//bayer_gamma2_b1
#define CAMERA_BAYER_GAMMA2_B1(n)  (((n)&0xFF)<<0)

//bayer_gamma2_b2
#define CAMERA_BAYER_GAMMA2_B2(n)  (((n)&0xFF)<<0)

//bayer_gamma2_b3
#define CAMERA_BAYER_GAMMA2_B3(n)  (((n)&0xFF)<<0)

//bayer_gamma2_b4
#define CAMERA_BAYER_GAMMA2_B4(n)  (((n)&0xFF)<<0)

//bayer_gamma2_b6
#define CAMERA_BAYER_GAMMA2_B6(n)  (((n)&0xFF)<<0)

//bayer_gamma2_b8
#define CAMERA_BAYER_GAMMA2_B8(n)  (((n)&0xFF)<<0)

//bayer_gamma2_b10
#define CAMERA_BAYER_GAMMA2_B10(n) (((n)&0xFF)<<0)

//bayer_gamma2_b12
#define CAMERA_BAYER_GAMMA2_B12(n) (((n)&0xFF)<<0)

//bayer_gamma2_b16
#define CAMERA_BAYER_GAMMA2_B16(n) (((n)&0xFF)<<0)

//bayer_gamma2_b20
#define CAMERA_BAYER_GAMMA2_B20(n) (((n)&0xFF)<<0)

//bayer_gamma2_b24
#define CAMERA_BAYER_GAMMA2_B24(n) (((n)&0xFF)<<0)

//bayer_gamma2_b28
#define CAMERA_BAYER_GAMMA2_B28(n) (((n)&0xFF)<<0)

//bayer_gamma2_b32
#define CAMERA_BAYER_GAMMA2_B32(n) (((n)&0xFF)<<0)

//bayer_gamma2_b36
#define CAMERA_BAYER_GAMMA2_B36(n) (((n)&0xFF)<<0)

//bayer_gamma2_b40
#define CAMERA_BAYER_GAMMA2_B40(n) (((n)&0xFF)<<0)

//bayer_gamma2_b48
#define CAMERA_BAYER_GAMMA2_B48(n) (((n)&0xFF)<<0)

//bayer_gamma2_b56
#define CAMERA_BAYER_GAMMA2_B56(n) (((n)&0xFF)<<0)

//bayer_gamma2_b64
#define CAMERA_BAYER_GAMMA2_B64(n) (((n)&0xFF)<<0)

//y_thr7_Lo_reg
#define CAMERA_Y_THR7_LO_REG(n)    (((n)&0xFF)<<0)

//y_thr7_Mid_reg
#define CAMERA_Y_THR7_MID_REG(n)   (((n)&0xFF)<<0)

//y_thr7_Hi_reg
#define CAMERA_Y_THR7_HI_REG(n)    (((n)&0xFF)<<0)

//dpa_new_ctrl_reg
#define CAMERA_INFLG_CTRL_REG_0    (1<<0)
#define CAMERA_INFLG_CTRL_REG_1    (1<<1)
#define CAMERA_INFLG_CTRL_REG_2    (1<<2)

//dpa_new_ctrl_hi_reg
#define CAMERA_INFLG_CTRL_REG0_H   (1<<0)
#define CAMERA_INFLG_CTRL_REG1_H   (1<<1)
#define CAMERA_INFLG_CTRL_REG2_H   (1<<2)
#define CAMERA_THRESHOLD_RSVD(n)   (((n)&3)<<3)

//ae_index_gap
#define CAMERA_GAP_2E              (1<<0)
#define CAMERA_GAP_3E              (1<<1)
#define CAMERA_GAP_4E              (1<<2)
#define CAMERA_GAP_5E              (1<<3)
#define CAMERA_GAP_6E              (1<<4)
#define CAMERA_GAP_7E              (1<<5)
#define CAMERA_GAP_8E              (1<<6)
#define CAMERA_GAP_9E              (1<<7)

//awb_calc_height_reg
#define CAMERA_AWB_CALC_HEIGHT_REG(n) (((n)&0xFF)<<0)

//drc_r_clp_value_reg
#define CAMERA_DRC_R_CLP_VALUE_REG(n) (((n)&0x3F)<<0)

//drc_gr_clp_value_reg
#define CAMERA_DRC_GR_CLP_VALUE_REG(n) (((n)&0x3F)<<0)

//drc_gb_clp_value_reg
#define CAMERA_DRC_GB_CLP_VALUE_REG(n) (((n)&0x3F)<<0)

//drc_b_clp_value_reg
#define CAMERA_DRC_B_CLP_VALUE_REG(n) (((n)&0x3F)<<0)

//sepia_cr_reg
#define CAMERA_SEPIA_CR_REG(n)     (((n)&0xFF)<<0)

//sepia_cb_reg
#define CAMERA_SEPIA_CB_REG(n)     (((n)&0xFF)<<0)

//csup_y_min_hi_reg
#define CAMERA_CSUP_Y_MIN_HI_REG(n) (((n)&0xFF)<<0)

//csup_gain_hi_reg
#define CAMERA_CSUP_GAIN_HI_REG(n) (((n)&0xFF)<<0)

//csup_y_max_low_reg
#define CAMERA_CSUP_Y_MAX_LOW_REG(n) (((n)&0xFF)<<0)

//csup_gain_low_reg
#define CAMERA_CSUP_GAIN_LOW_REG(n) (((n)&0xFF)<<0)

//ae_dk_hist_thr_reg
#define CAMERA_AE_DK_HIST_THR_REG(n) (((n)&0xFF)<<0)

//ae_br_hist_thr_reg
#define CAMERA_AE_BR_HIST_THR_REG(n) (((n)&0xFF)<<0)

//hist_bp_level_reg
#define CAMERA_HIST_BP_LEVEL_REG(n) (((n)&0xFF)<<0)

//outdoor_th_reg
#define CAMERA_OUTDOOR_TH(n)       (((n)&15)<<0)
#define CAMERA_NON_OUTDOOR_TH(n)   (((n)&15)<<4)

//awb_rgain_low_reg
#define CAMERA_AWB_RGAIN_LOW_REG(n) (((n)&0x3F)<<2)

//awb_rgain_high_reg
#define CAMERA_AWB_RGAIN_HIGH_REG(n) (((n)&0x3F)<<2)

//awb_bgain_low_reg
#define CAMERA_AWB_BGAIN_LOW_REG(n) (((n)&0x3F)<<2)

//awb_bgain_high_reg
#define CAMERA_AWB_BGAIN_HIGH_REG(n) (((n)&0x3F)<<2)

//awb_calc_start_reg
#define CAMERA_AWB_WIN_Y_START(n)  (((n)&15)<<0)
#define CAMERA_AWB_WIN_X_START(n)  (((n)&15)<<4)

//awb_calc_width_reg
#define CAMERA_AWB_CALC_WIDTH_REG(n) (((n)&0xFF)<<0)

//hist_dp_level_reg
#define CAMERA_HIST_DP_LEVEL_REG(n) (((n)&0xFF)<<0)

//awb_y_fmin
#define CAMERA_AWB_Y_FMIN(n)       (((n)&0xFF)<<0)

//awb_y_fmax
#define CAMERA_AWB_Y_FMAX(n)       (((n)&0xFF)<<0)

//awb_cb_fmin
#define CAMERA_AWB_CB_FMIN(n)      (((n)&0xFF)<<0)

//awb_cb_fmax
#define CAMERA_AWB_CB_FMAX(n)      (((n)&0xFF)<<0)

//awb_cr_fmin
#define CAMERA_AWB_CR_FMIN(n)      (((n)&0xFF)<<0)

//awb_cr_fmax
#define CAMERA_AWB_CR_FMAX(n)      (((n)&0xFF)<<0)

//awb_y_fmin2
#define CAMERA_AWB_Y_FMIN2(n)      (((n)&0xFF)<<0)

//awb_y_fmax2
#define CAMERA_AWB_Y_FMAX2(n)      (((n)&0xFF)<<0)

//awb_cb_fmin2
#define CAMERA_AWB_CB_FMIN2(n)     (((n)&0xFF)<<0)

//awb_cb_fmax2
#define CAMERA_AWB_CB_FMAX2(n)     (((n)&0xFF)<<0)

//awb_cr_fmin2
#define CAMERA_AWB_CR_FMIN2(n)     (((n)&0xFF)<<0)

//awb_cr_fmax2
#define CAMERA_AWB_CR_FMAX2(n)     (((n)&0xFF)<<0)

//ae_use_mean
#define CAMERA_YCAVE_USE_MEAN(n)   (((n)&3)<<0)
#define CAMERA_YWAVE_USE_MEAN(n)   (((n)&3)<<2)
#define CAMERA_YAVE_WEIGHT_MODE    (1<<4)
#define CAMERA_NEXP_OUT_SEL_REG    (1<<5)
#define CAMERA_AE_EXT_ADJ_VAL_REG  (1<<6)
#define CAMERA_AE_EXT_ADJ_ON_REG   (1<<7)

//ae_weight_sta
#define CAMERA_YWAVE_PCNT_LEFT(n)  (((n)&15)<<0)
#define CAMERA_YWAVE_LCNT_TOP(n)   (((n)&15)<<4)

//ae_qWidth
#define CAMERA_QWIDTH(n)           (((n)&0xFF)<<0)

//ae_qHeight
#define CAMERA_QHEIGHT(n)          (((n)&0x7F)<<0)
#define CAMERA_YWAVE_SEL           (1<<7)

//ae_win_sta
#define CAMERA_YAVE_PCNT_STA(n)    (((n)&15)<<0)
#define CAMERA_YAVE_LCNT_STA(n)    (((n)&15)<<4)

//ae_Width
#define CAMERA_WIDTH(n)            (((n)&0xFF)<<0)

//ae_Height
#define CAMERA_HEIGHT(n)           (((n)&0xFF)<<0)

//sw_update
#define CAMERA_CC_TYPE_SW          (1<<0)
#define CAMERA_IS_OUTDOOR_SW       (1<<1)
#define CAMERA_GAMMA_TYPE_SW       (1<<2)
#define CAMERA_SW_UPDATE_RSVD      (1<<3)
#define CAMERA_IS_OUTDOOR_MODE(n)  (((n)&7)<<4)
#define CAMERA_AWB_OUTDOOR_EN      (1<<7)

//awb_ctrl5
#define CAMERA_R_LOW_NON_A(n)      (((n)&0xFF)<<0)

//awb_ctrl6
#define CAMERA_AWB_STOP_H(n)       (((n)&15)<<0)
#define CAMERA_AWB_ADJ_AGAIN(n)    (((n)&3)<<4)
#define CAMERA_AWB_ALGO_EN         (1<<6)
#define CAMERA_CHECK_R_LOW         (1<<7)

//sca_reg
#define CAMERA_SCA_MODE(n)         (((n)&7)<<0)

//ae_ee_sta_reg
#define CAMERA_AE_EE_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_ee_num_reg
#define CAMERA_AE_EE_NUM_REG(n)    (((n)&15)<<0)

//ae_ef_sta_reg
#define CAMERA_AE_EF_STA_GAIN(n)   (((n)&0x3F)<<0)

//ae_ef_num_reg
#define CAMERA_AE_EF_NUM_REG(n)    (((n)&15)<<0)

//ae_thr_big_reg
#define CAMERA_AE_THR_BIG_DARK(n)  (((n)&15)<<0)
#define CAMERA_AE_THR_BIG_BRIGHT(n) (((n)&15)<<4)

//sharp_gain_minus_low
#define CAMERA_SHARP_GAIN_MINUS_LOW(n) (((n)&0xFF)<<0)

//sharp_gain_minus_mid
#define CAMERA_SHARP_GAIN_MINUS_MID(n) (((n)&0xFF)<<0)

//sharp_gain_minus_hi
#define CAMERA_SHARP_GAIN_MINUS_HI(n) (((n)&0xFF)<<0)

//sharp_mode_mid_hi
#define CAMERA_SHARP_CMP_GAP_MID(n) (((n)&15)<<0)
#define CAMERA_SHARP_CMP_GAP_HI(n) (((n)&15)<<4)

//Fw_version_reg
#define CAMERA_FW_VERSION(n)       (((n)&0xFF)<<0)

//awb_y_min_reg
#define CAMERA_AWB_Y_MIN(n)        (((n)&0xFF)<<0)

//y_red_coef_reg
#define CAMERA_Y_RED_COEF(n)       (((n)&0xFF)<<0)

//y_blue_coef_reg
#define CAMERA_Y_BLUE_COEF(n)      (((n)&0xFF)<<0)

//cb_red_coef_reg
#define CAMERA_CB_RED_COEF(n)      (((n)&0xFF)<<0)

//cr_blue_coef_reg
#define CAMERA_CR_BLUE_COEF(n)     (((n)&0xFF)<<0)

//Hist_vbp_level_reg
#define CAMERA_HIST_VBP_LEVEL(n)   (((n)&0xFF)<<0)

//Hist_vdp_level_reg
#define CAMERA_HIST_VDP_LEVEL(n)   (((n)&0xFF)<<0)





#endif


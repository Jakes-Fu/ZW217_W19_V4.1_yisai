修 改 人：谭红云
修改日期：2019-06-17
修改版本：V2.1.8
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. cat1bis V4 0420 版本修改：
     1）st_at_l1cc_lte_instrument_test指令修改u16_bler_test_en参数的取值返回和含义。
        0:不开启;1:开启SCH;2:开启SI;3:开启PCH;
        


修 改 人：谭红云
修改日期：2019-06-13
修改版本：V2.1.7
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. catm V4 0421 版本/cat1bis V4 0420 版本修改：
     1）增加10.1.10、10.2.10 跟踪端口设置功能。
     2）静态NV工程模式，增加gcfTest控制标识。
   2. catm V3 0414 版本/cat1bis V3 0415 版本修改
     1）静态NV工程模式，增加gcfTest控制标识。


修 改 人：谭红云
修改日期：2019-05-17
修改版本：V2.1.6
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. 新增catm V4 0421 指令集。
   1）st_at_l1cc_lte_server_cellmeas_ctrl_para指令修改：
      u16_reseaved[17]修改为：
      u16_drx_inactive_len_th
      u16_no_scell_meas_time_th
      s16_drx_sinr_th1         
      u16_drx_sampnum_th1      
      s16_drx_sinr_th2         
      u16_drx_sampnum_th2      
      s16_drx_sinr_th3         
      u16_drx_sampnum_th3      
      s16_drx_sinr_th4         
      u16_drx_sampnum_th4                                    
      u16_drx_sampnum_cemode_b 
      s16_drx_lp_sinr_th       
      u16_idl_sampnum_th1      
      u16_idl_sampnum_th2      
      u16_idl_sampnum_th3      
      u16_idl_sampnum_th4      
      u16_idl_sampnum_th5      
   2） st_at_algo_lte_catm_measpwr_req_para指令修改。   
      u16_meas_wb_Index修改为b_static_subf_num_en



修 改 人：谭红云
修改日期：2019-04-24
修改版本：V2.1.5
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. 新增8910 cat1bis V4 0420 AT指令集。
     1) st_at_l1cc_lte_ul_ctrl_para指令s16_apc_adjust-->s16_ul_apc_adjust。
     2) st_at_l1cc_lte_server_cellmeas_ctrl_para指令，修改数据结构。
        增加b16_timing_jitter_set_flag、b16_freq_jitter_set_flag控制位；
        新增b_idl_sinr_outofsync_en、u16_idl_sinr_outofsync_th、u16_idl_outofsync_drx_num、u16_idl_irt_invalid_sinr_th、
        u16_idl_irt_limit_sinr_th、u16_idl_irt_limit_value、u16_drx_inactive_len_th、u16_drx_lowpwr_sinr_th、、
        u16_no_scell_meas_time_th参数设置；
        u16  u16_reseaved[17]-->u16  u16_reseaved[8]。
     3) st_prach_get_data_cfg_para(8.4.2.3)指令，修改数据结构。
        增加u16_get_prach_send_req_cnt、u16_preamble_id、u16_get_prach_rep_cnt。
     4）st_msg3_get_data_cfg_para(8.4.2.4)指令，修改数据结构。
        删除u16_uci_type、u16_get_pucch_cnt、u16_get_sr_send_req_cnt、u16_get_ack_csi_cnt；
        增加u16_get_msg3_send_req_cnt。                 
     5) st_at_l1cc_lte_srs_get_data_cfg_para(8.4.2.5) 修改u16_start_srs_sf_cnt参数含义。
     6）st_at_algo_lte_cat1bis_measpwr_req_para指令，调整了定义。
     7）st_at_algo_measctrl_out_posthandle 指令，修改数据结构。
        增加s16_offline_afc_limit_sinrth、u16_offline_soft_afc_limit、u16_offline_rf_afc_limit、s16_online_afc_limit_sinrth
         u16_reserved[9]。           
     8）调整了7.4--7.14，8.4--8.14指令的偏移和指令长度。
     
     
                                   
修 改 人：谭红云
修改日期：2019-03-28
修改版本：V2.1.4
验 收 人：
审 核 人：
参考文档：
   
修改内容：
  1. 增加V3 cat1bis 0415 指令集、V4 catm 0420/cat1bis 0419 指令集。
    1）T_Dynamic_nas 删除defaultPcoLen、defaultPco[MAX_PCO_LEN+2]参数。
    2）T_NVM_Dynamic_PS 增加BA表收集功能T_EasStoredInfo结构数组参数。



修 改 人：谭红云
修改日期：2019-03-22
修改版本：V2.1.3
验 收 人：
审 核 人：
参考文档：
   
修改内容：
    1. 增加catm v4 0419版本指令集.
      1）重新定义st_at_l1cc_lte_rf_ctrl_para参数结构。
      2）st_at_algo_lte_catm_measpwr_req指令 rsrp_compensate由u16修改为s16。
      3）st_at_algo_measctrl_out_posthandle指令 新增u16_rsrp_alpha_tmp成员，
         u16_reserved[4]修改为u16_reserved[3]。
      4）st_at_algo_lte_catm_ncell_measpwr_req指令 rsrp_compensate由u16修改为s16。
      


修 改 人：谭红云
修改日期：2019-03-19
修改版本：V2.1.2
验 收 人：
审 核 人：
参考文档：
   
修改内容：
    1. 新增catm v3/v4 NV版本0414/0418, cat1bis V3/V4 NV版本0414/0418。
       1) 静态NV，T_NVM_Static_Eng中的reserved3修改为highSpeedDetectPeriod。
       2) 动态NV，LTE、NB的GsmBa表由17个修改为33个。
    2. cat1bis V4 NV版本0418 PHY NV修改。
       1）st_at_algo_lte_cat1bis_measpwr_req_para，MeasPwr调优参数扩充了84字节的参数。
       2）st_at_algo_measctrl_out_posthandle，u16_reserved[3] 修改为u16_afc_comm_filter_cntmax 、
          u16_afc_hst_filter_cntmax、1个保留。
       3）调整7.5--7.14，8.5--8.14指令的偏移、长度。
       4）修改了几个参数的名称。


修 改 人：谭红云
修改日期：2019-02-19
修改版本：V2.1.1
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. 新增catm,cat1bis V3/V4 四套指令集（v3 catm 0413, cat1bis 0413; V4 catm 0417, cat1bis 0417）。
      增加了晶体老化温补方案中C0修正相关参数设置。
   


修 改 人：谭红云
修改日期：2019-02-15
修改版本：V2.1.0
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. 增加cat1bis 0416 指令集。
   


修 改 人：谭红云
修改日期：2019-01-23
修改版本：V2.0.9
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. 新增 cat1bis V4 0415 指令集。增加邻区测量样本和周期nv控制参数读写功能。
      （参见7.14/8.14指令)
    


修 改 人：谭红云
修改日期：2019-01-16
修改版本：V2.0.8
验 收 人：
审 核 人：
参考文档：
   
修改内容：
  1. catm V3/V4 89100412/89120416,cat1bis v3/v4 89110412/89130414 NV版本的静态NV修改：
    1) T_CeParameters增加tm6_CE_ModeA_r13_tdd,tm6_CE_ModeA_r13_fdd,tm9_CE_ModeA_r13_tdd,
       tm9_CE_ModeA_r13_fdd,tm9_CE_ModeB_r13_tdd,tm9_CE_ModeB_r13_fdd 控制位。
    2) 删除reserved3[2]，增加baRoundCnt(BA表FREQ MEAS的尝试次数)




修 改 人：谭红云
修改日期：2019-01-11
修改版本：V2.0.7
验 收 人：
审 核 人：
参考文档：
   
修改内容：
  1. catm V3/V4 89100412/89120416,cat1bis v3/v4 89110412/89130414 NV版本修改：
    1) T_supportedBandEutra 增加 support multiFreqSearch 控制。
    2) reserved2[2]修改为  u8 dchScellPeriod 和 u8 idlDetectPeriod。
  2. catm v4 89120416 NV版本 st_at_algo_lte_catm_pbmeas_req_t修改：
    增加触发pbch decode offline的最大寻呼次数；u16_reserved[5]修改为u16_reserved[4]
  3. reserved2[2] 修改为 u8 dchScellPeriod 和 u8 idlDetectPeriod


修 改 人：谭红云
修改日期：2019-01-04
修改版本：V2.0.6
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. 修改1.2、2.1、3.1、4.1 static NV、TraceConfig 读写指令。
   2. 新增3.4.1，3.4.2.1--3.4.2.32, 4.4.1,4.4.2.1--4.4.2.32 GGE Trace控制读写指令。
   3. 调整第3、4节指令的层次关系。
   4. 纠正指令列表窗口无法查询的bug。
   5. 优化NVM版本不匹配时的提示窗口。


修 改 人：谭红云
修改日期：2018-12-25
修改版本：V2.0.5
验 收 人：
审 核 人：
参考文档：
   
修改内容：
   1. 新增catm v4 89120415 版本指令集。st_at_algo_measctrl_out_posthandle_write指令修改
      修改un_measpwr_outpara_posthandle_bitmap的bit位配置；
      修改s16_low_afc_adjthr,s16_high_afc_adjthr成员名及其含义;
      s16_delta_delay,u16_afcadj_irt_protec_thr，u16_delta_agc_latitude成员前移10个字节；
      新增afc_hst调整值下限、afc_hst调整值上限的设置；
      删除s16_lim_delay[5]、u16_reserved成员。
      
   2. 新增cat1bis v4 89130413 版本指令集。st_at_algo_measctrl_out_posthandle_Write指令修改:     
      un_measpwr_outpara_posthandle_bitmap取消mpdcch_子帧9sigma特殊处理操作、IRT不同条件下每次的调整值
      设置。增加大时延频率调整值保护IRT门限设置控制位。
      delta_delay类型u16修改为s16；delta_delay、 u16_afcadj_irt_protec_thr 前移10个字节；
      新增afc_hst调整值下限、afc_hst调整值上限的设置；
      删除s16_lim_delay[5];增加u16_reserved[3]成员。


修 改 人：谭红云
修改日期：2018-12-14
修改版本：V2.0.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 新增catm V4 89120414 版本指令集。
      新增ldtc调优控制参数设置（见7.24/8.24 st_at_algo_lte_camt_ldtc_req_para1参数读写指令）。



修 改 人：谭红云
修改日期：2018-12-12
修改版本：V2.0.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. catm/cat1bis v3 89100411/89110411,v4 89120413/89130412 版本修改：
     T_NasCapability中 reserved[2]修改为reserved,增加PsDataOnlyPingAllowed。
     



修 改 人：谭红云
修改日期：2018-12-06
修改版本：V2.0.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. cat1bis v4 89130412 NV版本st_at_algo_lte_measpwr_outpara_check指令修改：
      l1cc_lte_measpwr_outpara_check_bit_t 的保留bit位修改为10位，末bit修改为target_card_id。



修 改 人：谭红云
修改日期：2018-12-01
修改版本：V2.0.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. cat1bis v4 89130412 NV版本st_at_l1cc_lte_dl_get_data_cfg_req指令修改：
    1）u16_reserved[3]-->u16_reserved[2]
    2）增加u16_busmon_pagespy_scene_bitmap 总线监控场景 按照bit位控制功能。
   
   

修 改 人：谭红云
修改日期：2018-11-21
修改版本：V2.0.0
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   修改catm/cat1bis v3 v4的 89100411，89110411，89120413，89130412 NVM版本指令集。
   1. 增加IMS 参数读写指令。（10.1.9，10.2.9）
   2. static NVM T_IdleOptimizedFeature子结构中u8  reserved[6]-->reserved[2],
      增加IE：sinrIntraSearchThsd、sinrInterSearchThsd、interMeasThsd、interSinrDis。
      
   



修 改 人：谭红云
修改日期：2018-11-13
修改版本：V1.9.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. catm V3/v4, cat1bis v3/v4 的0410以后的各NV版本中增加
       终端支持AMR语音编码格式、终端语音输出方式（耳机/扬声器）参数的读/写指令。
       指令见10.1.7/10.1.8,10.2.7/10.2.8。



修 改 人：谭红云
修改日期：2018-11-05
修改版本：V1.9.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 新增cat1bis v3 89110411 指令集。
    1）st_at_l1cc_lte_cqi_ctrl_para增加宽带CQI调整值、使能通过ber调整CQI参数。
      
   2. 新增cat1bis v4 89130412 指令集。
    1）st_at_l1cc_lte_cqi_ctrl_para增加宽带CQI调整值、使能通过ber调整CQI参数。
    2）st_at_algo_lte_cat1bis_measpwr_req_para 增加RBIS使能、RBIS使用直接位置指示是能、RBIS直接位置参数
    3) st_at_l1cc_lte_instrument_test u16_reserved 修改为 u16_test_line_en.
    4) 重新计算7.5 -- 7.13，8.5 -- 8.13 指令偏移、大小。
    
  3. 新增catm v4 89120413 指令集
    1）st_at_algo_lte_catm_measpwr_req_para 增加RBIS使能、RBIS使用直接位置指示是能、RBIS直接位置参数、保留字段.
    2）重新计算7.13 -- 7.23， 8.13 -- 8.23 各指令偏移、大小。


修 改 人：谭红云
修改日期：2018-11-01
修改版本：V1.9.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. 新增catm v3 89100411 指令集。
       （只是配合广桥升级V3的NV版本号，没有修改参数）
       

修 改 人：谭红云
修改日期：2018-10-30
修改版本：V1.9.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. 新增catm v4 89120412、cat1bis v4 89130411 指令集。
       （只是配合广桥升级V4的NV版本号，没有修改参数）


修 改 人：谭红云
修改日期：2018-10-17
修改版本：V1.9.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 新增指令集89100410/89110410/89120411/89100410。
      1）动态NV，mmcprevUsedLteFreq由u16的数组扩展为u32数组。
      2）静态NV，终端升级支持BAND66,频点数组从u16扩展到u32
                 新增IDL优化信息设置(T_IdleOptimizedFeature)。
      3）修改1.2/1.3/2.1/2.2/10.1.5/10.2.5指令的大小。
      4）89120411，PHY NV 修改8.12.1.9、8.12.2.9、8.12.3.9、8.12.4.9指令参数，支持band66。
 
   


修 改 人：谭红云
修改日期：2018-09-30
修改版本：V1.9.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1.修改CATM V3 0408，8910 CATM V4 0409 指令集。
    1）st_at_l1cc_lte_debug_assert_cfg_para指令结构修改。
  2. 新增CATM V3 0409, catm V4 0410, cat1bis v3 0409, cat1bis v4 0409 指令集。
    1）connMeasCtrl 修改了各子结构;
    2) 新增optimizedFeature相关参数;
    3) 增加band 28A、28B、41N的控制设置。
    4）PHY修改了一些参数名称；取值范围。
    


修 改 人：谭红云
修改日期：2018-09-27
修改版本：V1.9.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：

  1. 增加了89100408、89110408、89120409、89130408四套指令集。supportedBandListEUTRA修改为20组。
  
  

修 改 人：谭红云
修改日期：2018-09-13
修改版本：V1.9.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. CATM V3/Cat1bis V3/catM V4/Cat1bis V4 指令集增加10.1.6、10.2.6 boot_in_network_mode 读写指令。
   2. 新增catm V4 0408 AT指令集。st_at_algo_lte_camt_ldtc_req_para指令增加rbis_num、rbis_factor参数设置。 



修 改 人：谭红云
修改日期：2018-08-16
修改版本：V1.9.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 新增V3 catm/cat1bis, V4 catm/cat1bis 0407 四套 NVM 指令集。
   2. 协议栈NV修改
     1）静态NV的T_NVM_Static_Eng中reserved1修改为audioPath。
     2）动态NV增加GsmBaNb、GsmBaBandList相关参数的设置。
   3.catm v3 0407版本指令集。
     1）st_at_algo_measctrl_out_posthandle增加AGC档位调整幅度及保留参数。
     2）调整7.15--7.21，8.15--8.21指令偏移。

   

修 改 人：谭红云
修改日期：2018-08-02
修改版本：V1.9.0
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. cat1bis v3/v4 0406版本修改prach_apc_adjus参数的定义。



修 改 人：谭红云
修改日期：2018-07-21
修改版本：V1.8.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. 新增cat1bis v3/v4, catm v3/v4 89100406、89110406、89120406、89130406指令集。
    1）增加下行抓数相关功能
    2）新增st_at_l1cc_lte_ul_ctrl_para1 读、写指令。
    3）调整各相关指令偏移。
    


修 改 人：谭红云
修改日期：2018-07-11
修改版本：V1.8.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. 新增89100405、89110405、89120405、89130405 版本NV指令集。
      1) T_MeasurementParameters: 增加 ceMeasurements_r14
      2) T_NasCapability: 增加 DataPdnAutoAct
      3) 新增T_connMeasCtrl、T_reportEventPara相关参数
      4) 新增SleepControl参数控制。
      5）T_DefaultPdnCfg中u8 reserve[254]修改为u8 reserve[2]
      6）调整10.1.2--10.1.5,10.2.2--10.2.5指令偏移。
    2. 物理层调优修改。
      1）修改st_at_algo_lte_cat1bis_measpwr_req_para指令，新增20组参数。
      2）重新调整7.5 -- 7.12；8.5 -- 8.12 各指令偏移。  
      3) phy catm v4 89130405，catm v3 89100405指令集 st_at_algo_lte_camt_ldtc_req_para 指令
         增加s16_mib_enhance_sinr_th、u16_mib_enhance_max_rx_num参数    




修 改 人：谭红云
修改日期：2018-06-29
修改版本：V1.8.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. NVM 89100404、89110404、89120404、89130404 版本的指令集，静态NV 删除lteFrequencyBand子结构；
       增加reserved1[5]的u8数组。



修 改 人：谭红云
修改日期：2018-06-26
修改版本：V1.8.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、NV修改：动态NVM增加randSand参数设置；静态NVM增加ce_RetuningSymbols_r14参数设置。
   2. 新增89130404 cat1bis V4 指令集：
     1）修改st_at_algo_lte_cat1bis_measpwr_req_para读写指令；
     2）增加st_at_algo_lte_cat1bis_ncell_sw_cfg_para、st_at_algo_lte_cat1bis_ncell_measpwr_req_para读写指令。
     3）重新计算各指令的偏移、长度、操作码。
   3、新增89120404 catm V4 指令集：
     1）删除st_at_l1cc_lte_freqmeas_ctrl_freq_para、st_at_l1cc_lte_freqmeas_ctrl_cell_para相关读写指令。
     2）新增st_at_algo_lte_cam_iddet_req_para、st_at_algo_lte_catm_pbmeas_req_para、st_at_algo_lte_catm_pbmeas_req_para读写指令。
     3）重新计算各指令的偏移、长度、操作码。
   4、新增89110404 cat1bis V3 指令集：  
     1）新增st_at_algo_lte_cat1bis_ncell_sw_cfg_para、st_at_algo_lte_cat1bis_ncell_measpwr_req_para读写指令。
   5、新增89100404 catm V3 指令集：
     1）删除st_at_l1cc_lte_freqmeas_ctrl_freq_para、st_at_l1cc_lte_freqmeas_ctrl_cell_para相关读写指令。
     2）新增 st_at_algo_lte_catm_ncell_measpwr_req_para 读写指令。
     3）重新计算各指令的偏移、长度、操作码。
   6、新增指令集的数据结构对齐调整。

     
   





修 改 人：谭红云
修改日期：2018-06-20
修改版本：V1.8.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 根据最新NVM头文件，修改静态NV、动态NV参数。
      新增80100403、89110403、89120403、89130403 四套AT指令集。





修 改 人：谭红云
修改日期：2018-06-01
修改版本：V1.8.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. FreePlay功能使能AT间时间间隔有效。
   2. FreePlay功能，增加wait-for-response开关。
   3. 屏蔽工程中默认对ole对象的注册，避免无权写注册表而弹出的错误窗口。
   





修 改 人：谭红云
修改日期：2018-05-25
修改版本：V1.8.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 增加ata指令应答的特殊处理，解决该指令不回OK而超时的问题。
   2. 增加升级在线升级工具自身的功能。

   
   


修 改 人：谭红云
修改日期：2018-05-23
修改版本：V1.8.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 89100402 NVM指令集，修改8.21.1.1--8.21.4.9各指令中debug_type修改为允许修改。
   2. 解决超长指令响应数据有时组包错误的bug。
   3. 修改了软件的Logo和图标。
   
   
   


修 改 人：谭红云
修改日期：2018-05-07
修改版本：V1.8.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 新增89100402、89110402、89120402、89130402 NVM指令集。根据PHY最新的V3、V4 catm、cat1bis定义修改。
   2. 89130402 新增st_at_algo_lte_cat1bis_ncell_sw_cfg_para读写指令。
   3. v3、v4 cat1bis指令集，修改phyModuleBuff-B指令的定义。



修 改 人：谭红云
修改日期：2018-05-05
修改版本：V1.8.0
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. 修改8910 89100400, 89110400，89120400, 89130400；89100401, 89110401，89120401, 89130401各指令集中
     boot_rrcreq_reason_read/write,lock_band_read/write, cp_ciot_support_read/write, lock_frequency_read/write 
     指令的偏移地址。
     
     


修 改 人：谭红云
修改日期：2018-05-04
修改版本：V1.7.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. 新增8910 V3 catm/cat1bis融合NB的AT指令集89100401,89110401;
     新增8910 v4 catm/cat1bis融合NB的AT指令集89120401,89130401.
    
    
    


修 改 人：谭红云
修改日期：2018-05-02
修改版本：V1.7.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. 新增8910 V3 catm/cat1bis融合NB的AT指令集89100400,89110400;
     新增8910 v4 catm/cat1bis融合NB的AT指令集89120400,89130400.
    
    
    

修 改 人：谭红云
修改日期：2018-04-28
修改版本：V1.7.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. 新增89130105、89120106、89110207、89100309 NVM版本指令集，并根据最新的NVM定义修改。
     新增imsiSimulation相关信息。
    
  


修 改 人：谭红云
修改日期：2018-04-26
修改版本：V1.7.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. 新增融合NB的AT指令集（89100400），并根据最新的NVM定义修改。
  2. 修改了各AT指令集的描述信息。
  
  
  

修 改 人：谭红云
修改日期：2018-04-23
修改版本：V1.7.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. 新增catm 89120105、89100308，cat1bis 89110206、89130104 指令集，根据最新的NVM定义修改。
  
  
  

修 改 人：谭红云
修改日期：2018-04-20
修改版本：V1.7.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 组装发送功能增加对省电测试的支持。
   2. catm 89100307、89120104 NVM版本和cat1bis 89110205、89130103 NVM版本增加BA表读写AT指令。
   




修 改 人：谭红云
修改日期：2018-04-18
修改版本：V1.7.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. 89100307 V3 catm NVM指令集，根据需求修改st_at_debug_pdsch_err_para相关8.14.1.6、8.14.2.6、8.14.3.6、8.14.4.6指令参数。
    2. 89120104 V4 catm NVM指令集，根据需求修改st_at_debug_pdsch_err_para相关8.21.1.6、8.21.2.6、8.21.3.6、8.21.4.6指令参数。
    3. 因RF NVM机制与静态、动态NVM有差异，删除89100400 RF指令集。RF NVM独立成一套工具(RFGenius)使用。
    4. 打开串口时自动下发的AT指令，修改成可以在ini文件中配置。



修 改 人：谭红云
修改日期：2018-04-13
修改版本：V1.7.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. 新增89110205 NVM版本指令集，增加仪表测试相关的获取、设置指令。





修 改 人：谭红云
修改日期：2018-04-12
修改版本：V1.7.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. 新增89130103 NVM版本指令集，增加仪表测试相关的获取、设置指令。

    
    



修 改 人：谭红云
修改日期：2018-04-11
修改版本：V1.7.0
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. 89100400 NVM版本指令集，增加RF_NV_WRITE_TO_FLASH指令。
    2. 新增省电测试的串口波特率和发送0的个数可配置功能，立即生效。
    3. 89120104 NVM版本st_pucch_get_data_cfg_para_WRITE，修改ul抓数参数的取值范围。
    
    



修 改 人：谭红云
修改日期：2018-04-10
修改版本：V1.6.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. catm v3 0307, V4 0104;cat1bis v3 0204, v4 0102 NV版本指令集的assertBitmap参数修改为按照bit位控制。
      TraceConfig中bit7增加使能双跟踪设置。
   2. 新增超长AT指令分包、组包功能。
   3. 新增融合NB 的NVM 指令集（89100400NBCatmATDefine.dat），static、dynamic NVM指令支持分包、组包。
   4. 新增RF NVM设置相关的10条AT指令，每条指令都支持分包、组包。
   5. 根据NB融合版本NVM，重设外场测试相关的8条AT指令（boot_rrcreq_reason，lock_band，cp_ciot_support，lock_frequency)
      （注：只支持LTE相关设置）
   6. 工具栏增加省电测试控制开关及延时设置；软件主界面AT指令双击等操作及FreePlay窗口及各子窗口功能都增加省点测试支持。
   7. 8910 catm v4 (89120104 NVM版本）修改
      1）st_at_algo_measctrl_out_posthandle指令中u16_delta_delay 修改为s16_delta_delay。
      2）at_l1cc_lte_ldtc_tuning_ctrl_set_bit_t中增加b16_mpdcch_ue_band_ded_flag、b16_mpdcch_ue_band_com_flag标识。
      3）st_at_algo_lte_camt_ldtc_req_t中增加u16_mpdcch_ue_band_ded、u16_mpdcch_ue_band_com。


修 改 人：谭红云
修改日期：2018-03-24
修改版本：V1.6.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1.8910 v4 catm AT指令集修改
     st_at_algo_lte_catm_measpwr_req_para_write指令的irt_s_th(IRT的信号门限)参数
     的取值范围从0、1、3修改为0..32767。
     





修 改 人：谭红云
修改日期：2018-03-23
修改版本：V1.6.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1.8910 v4 catm AT指令集修改
     8.14.1.1 -- 8.14.1.10; 8.14.2.1 -- 8.14.2.10; 8.14.3.1 -- 8.14.3.10; 8.14.4.1 -- 8.14.4.10
     指令中的phy debug_type参数改为允许修改。




修 改 人：谭红云
修改日期：2018-03-20
修改版本：V1.6.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1.8910 v4 catm AT指令集修改
     1)  st_at_algo_measctrl_out_posthandle_write指令，删除"ABIS干扰小区筛选RSRP门限值"设置，
         增加 "最大允许AGC档位调整幅度" 设置。
     2) st_at_l1cc_lte_platform_ctrl_para_write指令，“reseaved”修改为“特殊子帧重复次数=1场景
         使用0.75*RB对应的TBSize标识” 设置。




修 改 人：谭红云
修改日期：2018-03-15
修改版本：V1.6.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1.增加8910 v4 catm AT指令集(NV版本：89120104)
     1) 新增【idle offline异常debug参数】相关的8条read/writeAT?噶睢?
     2) 修改【系统消息读取失败debug参数】相关的4条write指令。
   2.增加8910 V3 cat1bis AT指令集（NV版本：89110204）
     1）小区测控参数bitmap，增加 定时抖动、频偏抖动控制设置。
     2) 测控输出后处理参数设置,删除mpdcch 子帧9 sigma特殊处理操作设置，
        增加大时延频率调整值保护IRT门限设置。




修 改 人：谭红云
修改日期：2018-03-10
修改版本：V1.6.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1) 增加8910 v4 catm AT指令集(NV版本：89120103)
   2) 测控调优AT指令，增加定时抖动、频率抖动的控制设置。



修 改 人：谭红云
修改日期：2018-03-02
修改版本：V1.6.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1) 新增v3 catm/cat1bis两套AT脚本(catm 89100307  和 cat1bis 89110203 )。
  2）新增V4 catm/cat1bis两条AT脚本(catm 89120102  和 Cat1bis 89130102 )。
  3）新增st_at_algo_lte_cam_iddet_req_para调优相关AT。
  4) 根据新结构修改st_at_algo_lte_camt_ldtc_req_para相关AT。
  5) 因static NVM架构调整（分成静态NV、动态NV、PHY、TraceeConfig四大块）并修改了T_TraceConfig、T_NVM_Static_Eng。
     根据新架构修改静态NV AT指令，TraceConfig相关80多条AT。重新计算TracerConfig相关AT的编译，修改数据类型。



修 改 人：谭红云
修改日期：2018-02-06
修改版本：V1.6.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 8910 V4版本 catm AT指令修改。
     1） T_NasCapability增加apnRateCtrl、nonIpMTU、ipv4MTU控制。
     2） T_NVM_Static_Eng 增加PsFlowCtrlThresh，删除reserved[2]。 
   





修 改 人：谭红云
修改日期：2018-01-29
修改版本：V1.6.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 修改st_dl_get_data_para_req_write指令u16_com_end_req_type参数选择值定义。
   


修改人：谭红云
修改日期：2018-01-23
修改版本：V1.6.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 增加8910 v4 版本catm AT指令集（对应NVM版本89120101）。
     1）Trace相关：
       a) 修改3.34、3.35 AT名称；
       b) 新增3.36 TRA_MPHY_CATM_TRACE_INFO_1设置。.
       c) 4.6、4.42 增加TRA_MPHY_CATM_TRACE_INFO_1的bit位控制。
     2）PHY相关
       a) 重新计算、设置7.14.1.1 -- 7.14.1.9、7.14.2.1 -- 7.14.2.9、7.14.3.1 -- 7.14.3.9、7.14.4.1 -- 7.14.4.9、7.15 -- 7.21
          8.14.1.1 -- 8.14.1.9、8.14.2.1 -- 8.14.2.9、8.14.3.1 -- 8.14.3.9、8.14.4.1 -- 8.14.4.9、8.15 -- 8.21的偏移地址和功能码。
       b）根据PHY V4 头文件修改st_at_algo_lte_catm_measpwr_req、st_at_algo_measctrl_out_posthandle、st_at_algo_lte_camt_txrx_req_para
          st_at_algo_lte_camt_dlfft_req、st_at_algo_lte_camt_ldtc_req AT指令参数。
       
   2. 增加8910 v4 版本cat1bis AT指令集（对应NVM版本89130101）。
   
   3. 增加PHY catm v4 debug和Trace相关的9个.flw配置文件，
   



修改人：谭红云
修改日期：2018-01-07
修改版本：V1.6.0
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1.更新Flow目录下catm_v3的全部脚本。
   2.更新文件下载配置列表。


修改人：谭红云
修改日期：2017-12-19
修改版本：V1.5.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. catm 89100306 指令集，增加 PHY 系统消息读取失败debug参数 相关的8条AT指令。
      参见7.21.1.9、7.21.2.9、7.21.3.9、7.21.4.9，
          8.21.1.9、8.21.2.9、8.21.3.9、8.21.4.9。



修改人：谭红云
修改日期：2017-12-14
修改版本：V1.5.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   cat1bis 89100202 指令集修改。
   1. st_dl_get_data_para_req指令删除u16_dl_bhv_id参数。
   2. 修改7.4 -- 7.9，8.4 -- 8.9指令的偏移。 修改7.4.1、8.4.1的大小。



修改人：谭红云
修改日期：2017-12-04
修改版本：V1.5.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 数据结构对齐处理。增加catm 89100306和cat1bis 89100202版本指令集。
   2. nasCapability增加overrideSignallingPrioritySupport、overrideEabSupported设置。
   3. 工程模式中增加atUartControl、traceFlowControl设置。
   4. 修改lock_frequency读、写指令的偏移地址。


修改人：谭红云
修改日期：2017-12-01
修改版本：V1.5.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
  1. 增加一条AT指令只设置某参数一bit的特殊处理功能。
  2. catm 0305版本、cat1bis 0201版本增加NVM复位功能。
  3. 增加PHY catm 同步搜网失败设置相关16条指令。

        


修改人：谭红云
修改日期：2017-11-16
修改版本：V1.5.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. catm 0305 增加st_ul_get_data_para_req相关读写AT。见
        7.13.2.1 --- 7.13.2.5，8.13.2.1 --- 8.13.2.5 的10条指令。
        


修改人：谭红云
修改日期：2017-11-15
修改版本：V1.5.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. catm 0305 st_at_l1cc_lte_platform_ctrl_para指令修改
        u16  u16_reseaved[2]修改为u16  u16_reseaved；增加b16_mpdcch_sf9_valid_flag（子帧9 sigma特殊处理）
        



修改人：谭红云
修改日期：2017-11-14
修改版本：V1.5.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增cat1bis 0201 NVM 指令集。
        1) u16_com_start_req_type/u16_com_end_req_type 增加66个选择值定义。8.4.1 删除u16_dl_bhv_id。
        2）static NVM: ceSearchEnable u16修改为u8，增加assertBitmap的定义。
        3) TRA_MPHY_CAT1_PHY_INFO_BASE指令增加TRACE_PHY_LDTC1_PBCH_OUT_BASE_PARA设置。
     2. 新增catm 0305 NVM 指令集修改.
        1) static NVM: ceSearchEnable u16修改为u8，增加assertBitmap的定义。
        2）8.17 st_at_algo_measctrl_out_posthandle_write增加大时延平率调整值保护IRT门限设置。
        


修改人：谭红云
修改日期：2017-11-10
修改版本：V1.5.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. cat1bis 0200 NVM 指令集。
        1）根据需求重新定义3.7 -- 3.33，4.7 -- 4.33几十条AT。
        2) 修改4.6 PHY 跟踪 Module 设置。
        3）修改4.3 LTE 跟踪 Module 设置。
     2. catm 0304 NVM 指令集修改.
        1) 8.20 st_at_algo_lte_camt_ldtc_req_para增加dci_vaild_bypass_set_flag设置。
        2）修改4.3 LTE 跟踪 Module 设置。


修改人：谭红云
修改日期：2017-11-08
修改版本：V1.5.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增 cat1bis 0200 NVM 指令集。新增、修改7.1 -- 7.9, 8.1 -- 8.9 指令


修改人：谭红云
修改日期：2017-11-03
修改版本：V1.5.0
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增catm 89100304 NVM版本指令集；
        NasCapability增加srvccSupport控制bit；nvmStaticEng增加ceSearchEnable，删除flowCtrlPara中的reserve。
     2. 新增cat1bis 89110101 NVM版本指令集；
        NasCapability增加srvccSupport控制bit；nvmStaticEng增加ceSearchEnable，删除flowCtrlPara中的reserve；
        增加开机rrcrea原因、锁band、锁频、cp ciot  support、开始注册域读写10条AT指令。
      
修改人：谭红云
修改日期：2017-10-31
修改版本：V1.4.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. FreePlay下的Send Script窗口修改：
       1). 右键菜单增加【New Directory】、【New File】功能。
       2). 原【Delete File】修改为【Delete】，支持文件删除、多级子目录及其下文件的删除。
       3). Flow目录下的流程文件已按照平台版本分类存放。


修改人：谭红云
修改日期：2017-10-20
修改版本：V1.4.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 增加开机rrcrea原因、锁band、锁频、cp ciot  support、开始注册域读写10条AT指令。



修改人：谭红云
修改日期：2017-10-16
修改版本：V1.4.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 根据PHY最新头文件，重写编写8910 catm V3 AT指令集。修改Trace相关、PHY controll相关的AT。匹配NVM版本catm 89100303.
     2. 新增PHY 8910 cat1bis的AT指令集89110100Cat1bisATDefine.dat，匹配NVM版本cat1bis 89110100。
     3. 增加AT集脚本编辑功能。
     4. 增加允许下发一系列AT指令功能，并可在发送指令前修改AT参数值。



修改人：谭红云
修改日期：2017-10-12
修改版本：V1.4.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 增加第13节，默认设置4条AT命令。
     2. 更新了catm_v3_double_trace.flw文件。



修改人：谭红云
修改日期：2017-09-29
修改版本：V1.4.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 根据PHY的定义，重新定义89100302 AT指令集 3.7--3.42，4.7--4.42 的几十条AT指令。
     
        
        
修改人：谭红云
修改日期：2017-09-27
修改版本：V1.4.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增8910 CATM 89100204 AT指令集。
        新增3.43,4.43 AT指令（TA超时场景断言设置）



修改人：谭红云
修改日期：2017-09-26
修改版本：V1.4.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增8910 CATM&CAT1BIS 融合版本89100302 AT指令集。
        新增9.13--9.15，10.13--10.15 st_at_algo_lte_camt_txrx_req_para、
        st_at_algo_lte_camt_dlfft_req_para、st_at_algo_lte_camt_ldtc_req_para
        相关的6条AT指令。
     2. 更新了8910 CATM&CAT1BIS 融合版本89100301 AT指令集中start_req_type、
        com_start_req_type 、com_end_req_type参数的值定义。
     
     
     
修改人：谭红云
修改日期：2017-09-20
修改版本：V1.4.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增8910 CATM&CAT1BIS 融合版本89100301 AT指令集。


修改人：谭红云
修改日期：2017-09-16
修改版本：V1.4.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     新增89100203 AT指令集。在89100202 AT指令集基础上
     1. l1cc_lte_measctrl_out_posthandle_bit_t增加IRT估计补偿值设置、
        IRT不同条件下每次的调整值值设置。
     2. st_at_algo_lte_measctrl_out_posthandle_t增加IRT调整一定条件下
        的最大值、IRT调整补偿值设置。

    
    
修改人：谭红云
修改日期：2017-09-08
修改版本：V1.4.0
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    89100202 AT指令集：
    1、删除st_at_l1cc_lte_get_data_ctrl_para读、写指令。
    2、增加st_at_l1cc_lte_condition_assert_cfg_para读、写指令。
 
    
修改人：谭红云
修改日期：2017-09-07
修改版本：V1.3.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. static NVM的nvmStaticMTC第12bit修改为cp_hc_support。
    2. PHY NV，新增3.35（L1CC_LTE_PLOT_INFO）-- 3.38、4.35--4.38 八条指令。
    3. 原3.35 -- 3.38修改为3.39--3.42,4.35--4.38修改为4.39--4.42。
    
    
    

修改人：谭红云
修改日期：2017-08-30
修改版本：V1.3.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. st_at_algo_lte_measctrl_out_posthandle指令l1cc_lte_measctrl_out_posthandle_bit_t
       增加mpdcch 子帧9 sigma特殊处理操作bit位。


修改人：谭红云
修改日期：2017-08-22
修改版本：V1.3.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. st_at_algo_lte_catm_measpwr_req指令增加前后两个样本子帧AGC最大差值门限；
    2. st_at_algo_lte_measctrl_out_posthandle指令增加RLM_out_sync最大次数、RLM_in_sync最大次数、
       RLM_out_sync_sinr门限补偿值、RLM_in_sync_sinr门限补偿值、rlm补偿值修改、rlm_QIN_QOUT次数门限修改。


修改人：谭红云
修改日期：2017-08-17
修改版本：V1.3.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    根据PF要求，所有PHY相关的AT指令，不再多偏移4个字节。


修改人：谭红云
修改日期：2017-08-15
修改版本：V1.3.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1.增加NVM 89100200 版本 AT指令集。PHY从static NVM剥离。
   2.纠正工具和UE NVM版本不匹配时弹出提示窗口显示为空白的bug。
   3.st_at_algo_lte_measctrl_out_posthandle指令增加RSSI线性值放大缩小移位因子、
     IRT ALPHA COUNT最大值参数设置；l1cc_lte_measctrl_out_posthandle_bit_t增加
     trms滑动窗长设置标志、rssi放大缩小移位因子设置标志、irt alpha滤波count最大
     值设置标志设置；修改了一些参数的取值范围、初始值、参数名称。
   4. st_at_algo_lte_catm_measpwr_req指令增加前后两个样本子帧AGC最大差值门限、
     RBIS使能、RBIS直接位置指示使能、 RBIS检测个数、RBIS直接位置指示、RBIS因子
     的设置。
   5. 打开端口时增加TraceConfig读指令的下发。
   6. 修改了Update_NV_Version.flw中的指令。
   7. 增加5.3 PHY写入FLASH指令。
   8. 原9.1--9.12指令增加了读功能。原9.1--9.12修改为10.1--10.12。
   

修改人：谭红云
修改日期：2017-08-09
修改版本：V1.3.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 89100104版本AT指令集修改。  
     1）st_at_l1cc_lte_platform_ctrl_para
       a）增加ldtc双中断方案标识(u16_ldtc_2_int_flag).
       b）u16  u16_reseaved[3]-->u16  u16_reseaved[2]
   2. FreePlay功能中增加NV版本修改Update_NV_Version.flw
  


修改人：谭红云
修改日期：2017-08-01
修改版本：V1.3.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1. 增加89100104版本AT指令集。
     1）st_at_l1cc_lte_test_line_ctrl_t修改：
       a）取消u16_agc_fix_flag、u16_agc_val、u16_reseaved成员
       b）增加UINT16  u16_reseaved[3]成员。  
     2）st_at_l1cc_lte_platform_ctrl_t
       a）增加u16_close_uldft_err_flag
       b）u16  u16_reseaved[4]-->u16  u16_reseaved[3]
     3）新增st_at_algo_lte_measpwr_outpara_check（measpwr输出测控参数异常check参数设置）。
     4）新增st_at_algo_measctrl_out_posthandle（测控输出后处理参数设置。
   2. FreePlay功能中增加trace_phy_base_ulip.flw。
  

修改人：谭红云
修改日期：2017-07-28
修改版本：V1.3.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1、平台控制参数修改：
      1）?黾覷LDFT ERR蓝屏控制标识。
      2）reseaved[4]修改为reseaved[3]??
    


修改人：谭红云
修改日期：2017-07-26
修改版本：V1.3.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1、增加平台相关的控制参数（st_at_l1cc_lte_platform_ctrl_t）。


修改人：谭红云
修改日期：2017-07-21
修改版本：V1.3.0
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1、st_at_l1cc_lte_target_cellmeas_ctrl增加s16_rx_rssi_scale，u16_reseaved[3]修改为u16_reseaved[2]
    2、st_at_l1cc_lte_test_line_ctrl增加u16_agc_fix_flag、u16_agc_val，UINT16  u16_reseaved[3]修改为UINT16  u16_reseaved。
    3、增加异频或同频小区meas抓数支持。
    4、根据NVM 89100103 头文件修改脚本、调整指令顺序


修改人：谭红云
修改日期：2017-07-07
修改版本：V1.2.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1、动态NV，增加emmT3402Info信息(对应NVM 89100103)。
   


修改人：谭红云
修改日期：2017-06-30
修改版本：V1.2.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1、因广桥修改了物理层相关的很多结构，对应修改工具（对应NVM 89100102）。
   


修改人：谭红云
修改日期：2017-06-22
修改版本：V1.2.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. AT指令发送，响应错误或超时，弹出错误提示窗口。
    2、响应错误或超时当错误处理，不再发送后续指令。FreePlay窗口?词股柚昧酥馗捶⑺停膊换峒绦⑺秃笮鳤T。
    3、FreePlay窗口AT发送间隔时间默认修改为3秒。
   

修改人：谭红云
修改日期：2017-06-20
修改版本：V1.2.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    1. 纠正通过FreePlay发送AT后，关闭端口再打开端口时无法获取NV版本的bug。
    2、static_NV中nvmStaticMTC参数增加1bit的设置。
    3、修改com_rx抓数参数中的原语类型枚举选项。
    

修改人：谭红云
修改日期：2017-06-15
修改版本：V1.2.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、新增7.15 st_at_l1cc_lte_cat1bis_get_data_cfg_para_READ、7.17 st_at_algo_lte_cat1bis_coeff_req_para_READ
      8.15 st_at_l1cc_lte_cat1bis_get_data_cfg_para_WRITE、   8.17 st_at_algo_lte_cat1bis_coeff_req_para_WRITE四条指令.
   2、修改了7.16、7.18、8.16、8.18指令的偏移地址。


修改人：谭红云
修改日期：2017-06-13
修改?姹荆篤1.2.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、增加 打开端口时，自动获取Staic NVM和Dynamic NVM参数功能。
   2、 <9.6 st_com_rx_get_data_para> 指令修改：
      1）u16_dl_bhv_id 参数修改为允许修改。
      2）u16_com_start_req_type、u16_com_end_req_type参数增加了参数选择。
   3、 <9.7 st_prach_get_data_cfg_para>指令修改:
      1)u16_ul_chn_id参数修改为允许修改。
   4、CATM_trace_phy_base_v0.0.2-ul.flw、band8_only.flw加入Flow下载列表。



修改人：谭红云
修改日期：2017-06-09
修改版本：V1.2.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、CATM NVM 89100101版本增加【接收场景通用抓数控制设置】功能。参见序号9.6的指令。




修改人：谭红云
修改日期：2017-06-06
修改版本：V1.2.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、CAT1bis 增加7.15、7.16、8.15、8.16 st_at_algo_lte_cat1bis_measpwr_req_para、st_at_algo_lte_cat1bis_pdcch_fa_req_para的读写指令。
   2、CATM 增加st_at_algo_lte_cat1bis_pdcch_fa_req_para的读写指令。



修改人：谭红云
修改日期：2017-05-27
修改版本：V1.2.1
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、增加3.34、4.34指令phyMsgControl[27](L1CC_LTE_TRACE_INFO_BASE)。实现TRACE_PHY_RF_BASE_PARA、 TRACE_PHY_FCP_AGC_BASE_PARA、
      TRACE_PHY_UL_BASE_PARA、TRACE_PHY_MEAS_IRT_OUT_BASE_PARA、TRACE_PHY_MEAS_SINR_OUT_BASE_PARA、TRACE_PHY_RX_IP_SF_BASE_PARA
      TRACE_PHY_RX_IP_DHI_BASE_PARA、TRACE_PHY_LDTC_OUT_BASE_PARA、TRACE_PHY_FREQMEAS_MEAS_OUT_BASE_PARA、TRACE_PHY_FREQMEAS_IDDET_OUT_BASE_PARA
      的设置。
   2、4.32 phyMsgControl[25](L1CC_LTE_BHV_PRIVATE_PARA_BASE) 增加TRACE_INFO_BASE bit位设置。                                       
   3、调整3.34---3.38,4.34---4.38指令序号。                   
                                                                                    
                                                                                   
修改人：谭红云                                                                                    
修改日期：2017-05-25                                                       
修改版本：V1.2.0                                                          
验 ??人：
审 核 ?耍?
参考文档：
        
修改内容：
   1、dynamic NV 增加 ltePrevUsedBand[8]；
   2、修改指令长度和偏移
   3、动态NV指令将几个array修改为struct，以便于输入控制。


修改人：谭红云
修改日期：2017-05-17
修改版本：V1.1.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、修改了msg4抓数指令参数


修改人：谭红云
修改日期：2017-05-12
修改版本：V1.1.8
验 收 人：
审 核 人：
参考文档：
        
修?哪谌荩?
   1、增加MeasPwr调优参数，Coeff调优参数读指令；
   2、MeasPwr调优参数增加了几十个参数。
   3、更新了Flow升级文件列表





修改人：谭红云
修改日期：2017-05-10
修改版本：V1.1.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、增加10.1 MeasPwr调优参数设置，10.2 Coeff调优参数设置功能；
   2、修改了3.27--3.29，4.27--4.29的指令名称。
   3、修改了AT响应处理。
   4、更新了catm_v2_double_trace.flw。
   


修改人：谭红云
修改日期：2017-05-04
修改版本：V1.1.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、修改st_at_l1cc_lte_si_get_data_cfg_para_t AT指令参数；
   2、升级功能增加十几个流程文件的下载支持。


修 改 人：谭红云
修改日期：2017-05-02
修改版本：V1.1.5
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、 修改7.1, 7.2, 7.3.1---7.3.9, 7.4.1 --- 7.4.9, 7.5 --- 7.14, 8.1, 8.2, 8.3.1---8.3.9, 8.4.1 --- 8.4.9, 8.5 --- 8.14, 
       各AT指令的operationType参数。



修 改 人：谭红云
修改日期：2017-04-25
修改版本：V1.1.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
   1、各脚??.2-----4.37 增加Full01Flag，实现置0，置1功能。
   2、4.37改成跟4.6的设置一样。
   3、修改代码，实现Full01Flag相关功能的处理。
   4、89100101版本的脚本，修改4.7---4.33 AT的名称
   5、修改了st_at_l1cc_lte_server_cellmeas_ctrl_t结构参数。
     
     


修 改 人：谭红云
修改日期：2017-04-22
修改版本：V1.1.3
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增8910 CAT M PHY抓数相关9条AT指令支持。



修 改 人：谭红云
修改日期：2017-03-21
修改版本：V1.1.2
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增8910 CAT 1 AT指令集，实现物?聿愕腡race 消息配置控制。参见3.x---4.x的指令。





修 改 人：谭红云
修改日期：2017-03-06
修改版本：V1.1.1
验 收 人：
审 核 人：
参考文档：
        
修改内容??
     1. 因T_NVM_Dynamic_PHY从动态NV调整到静态NV中。重新计算、修改与T_NVM_Dynamic_PHY相关60条AT指令的偏移。




修 改 人：谭红云
修改日期：2017-03-01
修改版本：V1.1.0
验 收 人：
审 核 人：
参考文档：
        
修?哪谌荩?
     1. 增加了List Send、Script Send及日志显示窗口的复制、黏贴、剪切功能。


修 改 人：谭红云
修改日期：2017-02-28
修改版本：V1.0.9
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 优化软件?２⒔饩隽艘逊⑾值腷ug。
     2、新增自定义ASCII、HEX AT、数据列表发送功能。
     3、重新布局原freeplay窗口。将Input Send、Script Send及新增的List Send子窗口合并到一起管理。
     4、取消了原主界面的AT手工输入发送窗口及收发窗口中的输入发送按钮。
     5、TraceConfig中增加lteTraModuleBuff、ggeTraModuleBuff、pubTraModuleBuff、phyModuleBuff参数获取和设置功能。



修 改 人：谭红云
修改日期：2017-02-08
修改版本：V1.0.8
验 收 人：
审 核 人：
参考文档：
        
修改内容：
     1. 新增物理层L1CC control参数设置相关60来条AT指令。




修 改 人：谭红云
修改日期：2017-01-24
修改版本：V1.0.7
验 收 人：
审 核 人：
参考文档：
        
修改内容：
    葛坤、罗勇提出的SSCOM相关功能已实现。可编辑自定义脚本文件、发送脚本文件、删除脚本文件、可循?贩⑺汀⒎⑺图涓羯柚谩?
    结尾0x0D控制、发送AT支持ASCII指令和十六进制数据AT、收到的响应数据可显示为ASCII字符、也可显示十六进制；可以输入
    单条ASCII AT或十六进制数据发送。


修 改 人：谭红云
修改日期：2017-01-10
修改版本：V1.0.6
验 收 人：
审 核 人：
参考文档：
        
修改内容：
       1、增加RF TX、RX抓数据到SD卡的5条AT指令。
         1）RF_TX_to_SD_start
         2）RF_TX_to_SD_stop
         3）RF_RX_to_SD_start
         4）RF_RX_to_SD_stop
         5）Active_ASSERT
               




修 改 人：谭红云
修改日期：2017-01-05
修改版本：V1.0.5
验 收 ?耍?
审 核 人：
参考文档：
        
修改内容：
       1、AT指令参数输入窗口?黾佑壹说ァ?
       2、实现右键菜单中的三个功能。
          1）TraceConfig相关几十条AT的每一bit值可以全部置1。
          2）TraceConfig相关几十条AT的每一bit值可以全部??。
          3）TraceConfig相关几十条AT，可以输入任意U32参数去替换32bit的值。

       注意事项：
           AT定?褰疟局校琓raceConfig相关几十条AT的名称，千万不要修改！！


修 改 人：谭红云
修改日期：2016-12-28
修改?姹荆篤1.0.4
验 收 人：
审 核 人：
参考文档：
        
修改内容：
       1）增加89100003 NVM版本的static、dynamic、TraceConfig的读写支持。



修 改 人：谭红云
修改日期：2016-12-15
修改版本：V1.0.3
验 收 人：
??核 人：
参考文档：
        
修改内容：
       1）增加89100002 NVM版本的static、dynamic、TraceConfig的读写支持。
       2）解决按下部分功能键后，主UI顶部窗口显示为空白的BUG。
  


修 改 人：谭红云
修改日期：2016-11-15
修改版本：V1.0.2
验 收 人：
审 核 人：
参考文档：
        

修改内容：
       1）增加AutoUpdate功能。
       2）添加TraceConfig各独立指令的名称及各指令每一bit控制的消息名称。
       3）纠正About窗口版本显示错误。
       4) 增加TraceConfig各指令的名称、每一bit的含义，增加第26组设置。       

修 改 人：谭???
修改日期：2016-11-11
修改版本：V1.0.1
验 收 人：
审 核 人：
参考文档：
        

修改内容：
       1）静态NVM的数据结构新增物理层Log控制40个U32，并可控制每一bit的设置
       2）根据物理层需求，将TraceConfig独立出来，新增64条指令（32条读、32条写），
          实现每个u32消息类别的32bit可以独立设置和显示以方便使用。
       3）发布V1.0.1版本。


修 改 人：谭红云
修改日期：2016-10-30
修改版本：V1.0.0
验 收 人：
审 核 人：
参考文档：
        

修改内容：
        1、发布V1.0.0


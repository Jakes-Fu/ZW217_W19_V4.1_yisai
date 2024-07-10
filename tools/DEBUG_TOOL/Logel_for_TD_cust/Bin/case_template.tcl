###############################################################################
# 
#  Copyright (c) 2008 Spreadtrum  
#  
#  All Rights Reserved.  
#                            
#  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Spreadtrum. 
#  The above copyright notice does not evidence any actual or intended  
#  publication of such source code.  
#   
#  FILENAME   : xxxxxxxx.tcl   
#  SYNOPSIS   : to reproduce cr_xxx.
#             : The following modification should be applied to reproduce this 
#             : CR:
#             : ---------------------------------------------------------------
#             :
#             : --------------------------------------------------------------- 
#                      R E V I S I O N    H I S T O R Y
#  ---------------------------------------------------------------------------- 
#  DATE            NAME            DESCRIPTION				            
#  xxxx-xx-xx      xxxx.xx         First Creation					
###############################################################################

###############################################################################
#  FUNCTION  : xxxxxxxx_init
#  ABSTRACT  : initialization of xxxxxxxx
#  INPUT     : none
#  RETURN    : none
#  CREATE    : xxxx-xx-xx : xxxx.xx
#  UPDATE    :   
###############################################################################
proc xxxxxxxx_init {} {
    set_trace_log_files xxxxxxxx traces;
}

###############################################################################
#  FUNCTION  : xxxxxxxx
#  ABSTRACT  : main procedure of xxxxxxxx
#  INPUT     : none
#  RETURN    : none
#  CREATE    : xxxx-xx-xx : xxxx.xx
#  UPDATE    :   
###############################################################################
proc xxxxxxxx {} {
    #manual_part(set the estimated running time, unit: minute): begin
    bp_set_caseRunTime 1;
    #manual_part(set the estimated running time, unit: minute): end
    
    #manual_part(configure NV): begin    
    #bp_set_nv_para;
    #manual_part(configure NV): end
    
    #manual_part(configure SIM): begin
    bp_write_sim_usim EF_USIM_LOCI 0 11 "\\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFE \\xFF \\xFF";
    bp_write_sim_usim EF_PSLOCI 0 14 "\\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFF \\xFE \\xFF \\xFF";
    #manual_part(configure SIM): end

    #initialization of UE
    bp_init;

    xxxxxxxx_init;

    #TD_manual_part(configure cell parameters): begin
    bptd_ftr_set_umts_cell_num 2;
    debugputs 2 "set num_umts_cells to 2";
    bptd_ftr_configure_cell_paras 1 0x2750 0 40 70;
    bptd_ftr_set_plmn 1 "4 6 0 0 0 15";
    bptd_ftr_configure_cell_paras 2 0x2758 1 10 5;
    bptd_ftr_set_plmn 2 "4 6 0 0 0 15";
    #TD_manual_part(configure cell parameters): end

    #GSM_manual_part(configure cell parameters): begin
    bp2g_ftr_set_gsm_cell_num 2;
    debugputs 2 "set num_gsm_cells to 2";
    bp2g_ftr_configure_cell_paras 1 0x14 40 0 1 1;
    bp2g_ftr_set_plmn 1 "4 6 0 0 0 15";
    bp2g_ftr_configure_cell_paras 2 0xA 10 0 1 1;
    bp2g_ftr_set_plmn 2 "4 6 0 0 0 15";
    #GSM_manual_part(configure cell parameters): end
    
    #TD_system_information_part: begin
    #TD_system_information_part: end

    #GSM_system_information_part: begin
    #GSM_system_information_part: end

    #TD_manual_part(trigger TD system information): begin
    #bptd_ftr_trigger_sysinfo 1;
    #TD_manual_part(trigger TD system information): end
    
    #GSM_manual_part(trigger GSM system information): begin
    #bp2g_ftr_trigger_sysinfo 1;
    #GSM_manual_part(trigger GSM system information): end

    #TD_manual_part(set the index of current cell): begin
    #bptd_ftr_set_current_cellidx 1;
    #TD_manual_part(set the index of current cell): end    

    #GSM_manual_part(set the index of current cell): begin
    #bp2g_ftr_set_current_cellidx 1;
    #GSM_manual_part(set the index of current cell): end  

    #Message Flow
    #poweroff UE
    bp_poweroff;

    debugputs 0 "PASS";   
}    

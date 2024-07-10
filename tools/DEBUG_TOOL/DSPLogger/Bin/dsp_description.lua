--[[--------------------------------------------------------------------
lua script to dynamic interpret dsp log and output description strings

NOTE:  To add new log interpreting functions:
step 1:  Add function reference in init_func_list
step 2:  Add function body

2008/9/9:  create, by xiaoguang.feng
--]]--------------------------------------------------------------------

-- for bit-wise operations
local bit = require"bit"

-- global function list table
func_list={}

-- initialize function list
function init_func_list()
	func_list[0xDFFF]=func_DFFF
	func_list[0xD810]=func_D810
	func_list[0xD811]=func_D811
	func_list[0xD812]=func_D812
	func_list[0xD813]=func_D813
	func_list[0xD814]=func_D814
	func_list[0xD815]=func_D815
	func_list[0xD816]=func_D816
	func_list[0xD817]=func_D817
	func_list[0xD818]=func_D818
	func_list[0xD820]=func_D820
	func_list[0xD830]=func_D830
	func_list[0xD831]=func_D831
	func_list[0xD832]=func_D832
	func_list[0xD833]=func_D833
	func_list[0xD834]=func_D834
	func_list[0xD840]=func_D840
	func_list[0xD841]=func_D841
	func_list[0xD842]=func_D842
	func_list[0xD843]=func_D843
	func_list[0xD844]=func_D844
	func_list[0xD850]=func_D850
	func_list[0xD851]=func_D851
	func_list[0xD860]=func_D860
	func_list[0xD870]=func_D870
	func_list[0xD880]=func_D880
	func_list[0xD910]=func_D910
	func_list[0xD920]=func_D920
	func_list[0xDD37]=func_DD37
	func_list[0xDD3E]=func_DD3E
	func_list[0xDD35]=func_DD35
	func_list[0xDD38]=func_DD38
	func_list[0xDD39]=func_DD39
	func_list[0xDD33]=func_DD33
	func_list[0xDD36]=func_DD36
	func_list[0xDD3B]=func_DD3B
	func_list[0xD77A]=func_D77A
	func_list[0xD720]=func_D720
	func_list[0xD721]=func_D721
	func_list[0xD722]=func_D722
	func_list[0xD723]=func_D723
	func_list[0xD724]=func_D724
	func_list[0xD725]=func_D725
	func_list[0xD726]=func_D726
	func_list[0xDB27]=func_DB27
	func_list[0xDB11]=func_DB11
	func_list[0xDB12]=func_DB12
	func_list[0xD821]=func_D821
	func_list[0xDB21]=func_DB21
	func_list[0xDB30]=func_DB30
	func_list[0xDB31]=func_DB31
	func_list[0xDB16]=func_DB16
	func_list[0xDB17]=func_DB17
	func_list[0xDB32]=func_DB32
	func_list[0xDB70]=func_DB70
	func_list[0xDB72]=func_DB72
	func_list[0xD77C]=func_D77C
	func_list[0xD911]=func_D911
	func_list[0xD912]=func_D912
	func_list[0xD913]=func_D913
	func_list[0xD914]=func_D914
	func_list[0xD915]=func_D915
	func_list[0xD916]=func_D916
	func_list[0xD77B]=func_D77B
	func_list[0xD917]=func_D917
	func_list[0xD918]=func_D918
	func_list[0xD919]=func_D919
	func_list[0xD500]=func_D500
	func_list[0xD501]=func_D501
	func_list[0xD502]=func_D502
	func_list[0xD503]=func_D503
	func_list[0xD506]=func_D506
	func_list[0xD507]=func_D507
	func_list[0xD50A]=func_D50A
	func_list[0xD50C]=func_D50C
	func_list[0xD50D]=func_D50D
	func_list[0xD50E]=func_D50E
	func_list[0xD50F]=func_D50F
	func_list[0xD510]=func_D510
	func_list[0xD511]=func_D511
	func_list[0xDC40]=func_DC40
	func_list[0xD855]=func_D855
	func_list[0xD512]=func_D512
	func_list[0xD514]=func_D514
	func_list[0xD516]=func_D516
	func_list[0xD517]=func_D517
	func_list[0xD51A]=func_D51A
	func_list[0xD51B]=func_D51B
	func_list[0xD221]=func_D221
	func_list[0xD51D]=func_D51D
	func_list[0xDC41]=func_DC41
	func_list[0xD714]=func_D714
	func_list[0xD717]=func_D717
	func_list[0xD749]=func_D749
	func_list[0xD741]=func_D741
	func_list[0xD751]=func_D751
	func_list[0xD752]=func_D752
	func_list[0xD760]=func_D760
	func_list[0xD761]=func_D761
	func_list[0xD846]=func_D846
	func_list[0xD51E]=func_D51E
	func_list[0xD51F]=func_D51F
	func_list[0xD51C]=func_D51C
	func_list[0xD871]=func_D871
	func_list[0xD872]=func_D872
	func_list[0xD521]=func_D521
	func_list[0xD522]=func_D522
	func_list[0xDD46]=func_DD46
	func_list[0xD883]=func_D883
	func_list[0xD600]=func_D600
	func_list[0xD601]=func_D601
	func_list[0xD780]=func_D780
	func_list[0xDFFE]=func_DFFE
	func_list[0xDFA1]=func_DFA1
	func_list[0xDFB5]=func_DFB5
	func_list[0xDF98]=func_DF98
	func_list[0xDFB6]=func_DFB6
	func_list[0xD8D0]=func_D8D0
	func_list[0xDC80]=func_DC80
	func_list[0xD515]=func_D515
	func_list[0xD223]=func_D223
	func_list[0xDFB0]=func_DFB0
	func_list[0xDB63]=func_DB63
	func_list[0xDB60]=func_DB60
	func_list[0xDB67]=func_DB67
	func_list[0xDB65]=func_DB65
	func_list[0xDD41]=func_DD41
	func_list[0xDB33]=func_DB33
	func_list[0xDB39]=func_DB39
	func_list[0xDB25]=func_DB25
	func_list[0xDB26]=func_DB26
	func_list[0xDD44]=func_DD44
	func_list[0xD881]=func_D881
	func_list[0xDB13]=func_DB13
	func_list[0xDB14]=func_DB14
	func_list[0xDB10]=func_DB10
	func_list[0xDB18]=func_DB18
	func_list[0xDB19]=func_DB19
	func_list[0xDB1C]=func_DB1C
	func_list[0xDB1D]=func_DB1D
	func_list[0xDB1A]=func_DB1A
	func_list[0xD873]=func_D873
	func_list[0xDB20]=func_DB20
	func_list[0xD950]=func_D950
	func_list[0xD951]=func_D951
	func_list[0xD819]=func_D819
	func_list[0xD81A]=func_D81A
	func_list[0xD81B]=func_D81B
	func_list[0xD961]=func_D961
	func_list[0xD962]=func_D962
	func_list[0xD963]=func_D963
	func_list[0xD882]=func_D882
	func_list[0xDF01]=func_DF01
	func_list[0xD5FE]=func_D5FE
	func_list[0xD886]=func_D886
	func_list[0xDC4B]=func_DC4B
	func_list[0xDF77]=func_DF77
	func_list[0xD9A0]=func_D9A0
	func_list[0xD9A1]=func_D9A1
	func_list[0xDCFD]=func_DCFD
	func_list[0xDCFC]=func_DCFC
	func_list[0xDCFB]=func_DCFB
	func_list[0xD888]=func_D888
	func_list[0xD885]=func_D885
	func_list[0xD894]=func_D894
	func_list[0xD895]=func_D895
	func_list[0xD865]=func_D865
	func_list[0xD770]=func_D770
	func_list[0xD77E]=func_D77E
	func_list[0xD89A]=func_D89A
	func_list[0xD800]=func_D800
	func_list[0xD971]=func_D971
	func_list[0xD972]=func_D972
	func_list[0xD973]=func_D973
	func_list[0xD974]=func_D974
	func_list[0xD975]=func_D975
	func_list[0xD976]=func_D976
	func_list[0xD977]=func_D977
	func_list[0xD978]=func_D978
	func_list[0xD979]=func_D979
	func_list[0xDFF3]=func_DFF3
	func_list[0xD890]=func_D890
	func_list[0xD891]=func_D891
	func_list[0xD730]=func_D730
	func_list[0xD731]=func_D731
	func_list[0xD732]=func_D732
	func_list[0xD828]=func_D828
	func_list[0xD787]=func_D787
	func_list[0xD788]=func_D788
	func_list[0xD781]=func_D781
	func_list[0xDAA0]=func_DAA0
	func_list[0xDAA1]=func_DAA1
	func_list[0xDAA2]=func_DAA2
	func_list[0xDAA3]=func_DAA3
	func_list[0xDAA4]=func_DAA4
	func_list[0xDAB0]=func_DAB0
	func_list[0xDD8A]=func_DD8A
	func_list[0xDD8B]=func_DD8B
	func_list[0xDD8C]=func_DD8C
	func_list[0xDD8D]=func_DD8D
	func_list[0xDD8E]=func_DD8E
	func_list[0xDD8F]=func_DD8F
	func_list[0xDD86]=func_DD86
	func_list[0xDD87]=func_DD87
	func_list[0xDD88]=func_DD88
	func_list[0xDDA1]=func_DDA1
	func_list[0xDDA2]=func_DDA2
	func_list[0xDDA3]=func_DDA3
	func_list[0xDDA4]=func_DDA4
	func_list[0xDDA5]=func_DDA5
	func_list[0xDD91]=func_DD91
	func_list[0xDD80]=func_DD80
	func_list[0xDD81]=func_DD81
	func_list[0xDD82]=func_DD82
	func_list[0xDD83]=func_DD83
	func_list[0xDD84]=func_DD84
	func_list[0xD551]=func_D551
	func_list[0xD552]=func_D552
	func_list[0xD553]=func_D553
	func_list[0xD554]=func_D554
	func_list[0xD555]=func_D555
	func_list[0xD556]=func_D556
	func_list[0xD557]=func_D557
	func_list[0xD671]=func_D671
	func_list[0xD651]=func_D651
	func_list[0xD690]=func_D690
	func_list[0xD682]=func_D682
	func_list[0xD683]=func_D683
	func_list[0xD684]=func_D684
	func_list[0xD8A1]=func_D8A1
	func_list[0xD8A2]=func_D8A2
	func_list[0xDC0F]=func_DC0F
	func_list[0xDDD0]=func_DDD0
	func_list[0xDDD1]=func_DDD1
	func_list[0xDE08] = func_DE08
	func_list[0xDE07] = func_DE07
	func_list[0xDF80] = func_DF80
	func_list[0xD8CC] = func_D8CC
	func_list[0xDF02] = func_DF02
	func_list[0xDF61] = func_DF61
	func_list[0xD8A0] = func_D8A0
	func_list[0xD673] = func_D673
	func_list[0xD62A] = func_D62A
	func_list[0xD62B] = func_D62B
	func_list[0xD62C] = func_D62A
	func_list[0xD62D] = func_D62B	
	func_list[0xD8CE] = func_D8CE
	func_list[0xD990] = func_D990	
	func_list[0xD6FD] = func_D6FD	
	func_list[0xD6FE] = func_D6FE	
	func_list[0xD6FF] = func_D6FF
	func_list[0xD650] = func_D650
	
	func_list[0xDAD0] = func_DAD0
	func_list[0xDAD1] = func_DAD1
	

end

-- log interpreting functions
function func_DAD0(address,data)
		return string.format('RFT_CTRL_TD_GSM_SEL[31:16]: 0x%X',data)
end
function func_DAD1(address,data)
		return string.format('RFT_CTRL_TD_GSM_SEL[15:0]: 0x%X',data)
end


function func_D650(address,data)
		return string.format('isr_FIR() (TD RX int): 0x%X',data)
end

--RFT Interrupt ENUM
RFT_INT_FLAG = 
{
	[0] = 'td_frame',
	'timer',
	'rft_rx_action',
	'rft_tx_action',
	'Not RFT Int'
}

function func_D6FD(address,data)
	if (bit.band(data,0x0001) ~= 0) then
		return string.format('isr_rft() int_flag: 0x%X %s',data,RFT_INT_FLAG[0])
	elseif (bit.band(data,0x0002) ~= 0) then
		return string.format('isr_rft() int_flag: 0x%X %s',data,RFT_INT_FLAG[1])	
	elseif (bit.band(data,0x0008) ~= 0) then
		return string.format('isr_rft() int_flag: 0x%X %s',data,RFT_INT_FLAG[2])	
	elseif (bit.band(data,0x0004) ~= 0) then
		return string.format('isr_rft() int_flag: 0x%X %s',data,RFT_INT_FLAG[3])	
	else
		return string.format('isr_rft() int_flag: 0x%X %s',data,RFT_INT_FLAG[4])	
	end
end

function func_D6FE(address,data)
		return string.format('TD_Meas_isr_RFT_Tx_Action g_rf_pin_ctrl_gsm: 0x%X; RF control TD->GSM',data)
end

function func_D6FF(address,data)
		return string.format('TD_Meas_isr_RFT_Rx_Action g_rf_pin_ctrl_td: 0x%X; RF control GSM->TD',data)
end

--GSM Task ID
GSM_TaskID =
{
    [0]='RTOS_dummy_task',            --Seq Num = 0
    'RTOS_dummy_task',            --Seq Num = 1
    'RTOS_dummy_task',            --Seq Num = 2
    'run_mcu_to_dsp_task',            --Seq Num = 3
    'run_mcu_to_audio_task',            --Seq Num = 4	//@jinni.xu add for new audio control
    'run_rx_dispatch_task',            --Seq Num = 5
    'run_rx_deinterleaver_task',            --Seq Num = 6
    'run_rx_task',            		--Seq Num = 7
    'run_tx_interleaver_task',            --Seq Num = 8
    'run_frame_eng_task',            --Seq Num = 9
    'RTOS_dummy_task',            --Seq Num = 10
    'run_tx_encoder_task',            --Seq Num = 11
    'run_edge_encoder_task',       --Seq_Num = 12
    'run_fir_task',				--Seq Num = 13
    'run_che_task',          	  --Seq Num = 14
    'run_dma_che2dsp',   	 --Seq Num = 15
    'task_td_meas',            	      --Seq Num = 16
    'RTOS_dummy_task',            --Seq Num = 17
    'RTOS_dummy_task',            --Seq Num = 18
    'RTOS_dummy_task',            --Seq Num = 19
    'RTOS_dummy_task',            --Seq Num = 20
    'RTOS_dummy_task',            --Seq Num = 21
    'RTOS_dummy_task',            --Seq Num = 22
    'run_vocoder_task',           	 --Seq Num = 23
    'bg_audio_para_set_task',      --Seq Num = 24		//@jinni.xu_cr146220
    'bg_freq_estimation_task',            --Seq Num = 25
    'RTOS_dummy_task',            --Seq Num = 26
    'RTOS_dummy_task',            --Seq Num = 27
    'RTOS_dummy_task', 		--Seq Num = 28
    'RTOS_dummy_task',            --Seq Num = 29
    'RTOS_gsm_idle_task'         --Seq Num = 30
}


--Edge Header type
Header_type = 
{
    [0] = 'INVALID_HEADER_TYPE',	-- invalid header type
    'HEADER_TYPE_1',      		-- For MCS7,MCS8,MCS9
    'HEADER_TYPE_2',      		-- For MCS5,MCS6
    'HEADER_TYPE_3',       		-- For MCS1,MCS2,MCS3,MCS4
    'CS_TYPE',             		--For CS1,CS2,CS3
    'MAX_HEADER_TYPE'     		-- max header type
}

function func_D990(address,data)
	return string.format('IR_arrange_dma_oper()  rx_data_info[i].header_type: %s',Header_type[data])
end


function func_D8CE(address,data)
	return string.format('bg_run_edge_data_deinterleaver_task() IN QBC: %d',bit.band(data,0x1FFF))
end

function func_D62A(address,data)
	return string.format('ForcePowerDownRFT() RFT_POWERDOWM_IN: %d',bit.band(data,0x1FFF))
end

function func_D62B(address,data)
	return string.format('ForceWakeRFT() RFT_POWERDOWM_OUT: %d',bit.band(data,0x1FFF))
end

function func_D62C(address,data)
	return string.format('ForceWakeRFT() RFT_WAKEUP_IN: %d',bit.band(data,0x1FFF))
end

function func_D62D(address,data)
	return string.format('ForcePowerDownRFT() RFT_WAKEUP_OUT: %d',bit.band(data,0x1FFF))
end

function func_D673(address,data)
	if (data == 0) then
		return string.format('td_meas_exit_task() TD_Meas_L1C_Task End: %d',data)
	else
		return string.format('td_meas_exit_task() TD_Meas_L1C_Task_NUM: %d',data)
	end
end

function func_DF02(address,data)
	if Header_type[data] then
		return string.format('Header type: %s',Header_type[data])
	else
		return string.format('unknown Header type: 0x%X',data)
	end
end

function func_DF61(address,data)
	return string.format('cs1_head_decode_finish: %d',data)
end

function func_D8A0(address,data)
	return string.format('EDGE_CS1_BLK_Decode Start CDC QBC: %d',bit.band(data,0x1FFF))
end

function func_D8CC(address,data)
	return string.format('EDGE_rx_dispatch_process QBC: %d',bit.band(data,0x1FFF))
end

function func_DF80(address,data)
	if (data==0x8C00) then
		return string.format('ACCCLK_DIV = 0x%X;  76Mhz',data)
	elseif (data==0x8801) then
		return string.format('ACCCLK_DIV = 0x%X;  100Mhz',data)		
	elseif (data == 0x8411) then
		return string.format('ACCCLK_DIV = 0x%X;  150Mhz',data)	
	else
		return string.format('ACCCLK_DIV = 0x%X; 0x8C00:76Mhz  0x8801:100Mhz  0x8411:150Mhz',data)
	end
end

function func_DE07(address,data)
	return  string.format('ramp_isr_agency QBC: %d', bit.band(data,0x1FFF))
end

function func_DE08(address,data)
	return  string.format('Event Interrupt QBC: %d', bit.band(data,0x1FFF))
end

function func_DFFF(address, data)
	return string.format('Frame Head')
end

-- used in func_D810
-- NOTE:  by default, lua table start with index=1
RF_Operation=
{
    [0]='NOP',              -- 0x00
    'SLEEP',                -- 0x01
    'RX',                   -- 0x02
    'RX_TX',                -- 0x03
    'TCH_RX_TX',            -- 0x04
    'TX',                   -- 0x05
    'RACH',                 -- 0x06
    'RACH_RX',              -- 0x07
    'TCH_RACH_RX',          -- 0x08
    'SEARCH',               -- 0x09
    'SCH_RFOP',             -- 0x0a
    'RXLEV',                -- 0x0b
    'TCH_SEARCH_START',     -- 0x0c
    'TCH_SEARCH',           -- 0x0d
    'TBF_RX',               -- 0x0e
    'TBF_TX',               -- 0x0f
    'TBF_RX_TX',            -- 0x10
    'TBF_POLLING_RX',       -- 0x11
    'TBF_MEAS',             -- 0x12
    'IDLE_MEAS',            -- 0x13
    'TBF_RACH_TX',          -- 0x14
	-- 'PCH_MULTI'				-- 0x15
	-- sync with arm layer1 edge function
    'EDGE_TBF_RX',          -- 0x15
    'EDGE_TBF_TX',          -- 0x16
    'EDGE_TBF_RX_TX',       -- 0x17
    -- these two operation are not used in egprs mode,reserved for future use
    'EDGE_TBF_POLLING_RX',  -- 0x18
    'EDGE_TBF_POLLING_RX_TX', -- 0x19
    'EDGE_TBF_MEAS',        -- 0x1a
    'EDGE_IDLE_MEAS',       -- 0x1b
--#ifdef _EDGE_DEV_
--    EDGE_TBF_RACH_RX,
--#endif
    'TBF_RACH_RX',
    'END_RFOP'
}

function func_D810(address, data)
	if RF_Operation[data] then
		return string.format('Frm_Instr: RF_OP = %s',RF_Operation[data])
	else
		return string.format('Frm_Instr: RF_OP = 0x%X',data)
	end
end

function func_D811(address, data)
	local RxBurstNo=bit.band(data,0x03)
	local TxBurstNo=bit.rshift(bit.band(data,0x0C),2)
	local RxChnTag=bit.rshift(bit.band(data,0xF0),4)
	FN1=bit.rshift(bit.band(data,0xFE00),9) --global variable, used in D812, combined to generate the whole frame_no

	return string.format('Frm_Instr: RxBstNo=%d, TxBstNo=%d, RxChnTag=%d, FN1=0x%X',RxBurstNo,TxBurstNo,RxChnTag,FN1)
end

function func_D812(address, data)
	if not FN1  then
  	return ""
  else
		return string.format('Frm_Instr: Frame_No=0x%X',bit.lshift(FN1,16)+data)
	end
end

function func_D813(address, data)
	--this point has been used twice, need to check
	--SearchCount=bit.band(data,0xFFF)
	--TxSendDataType=bit.rshift(bit.band(data,0xF000),12)
	return string.format('Frm_Instr') --: SearchCount=%d, TxSendDataType=%d',SearchCount,TxSendDataType)
end

function func_D814(address, data)
	return string.format('Frm_Instr')
end

function func_D815(address, data)
	return string.format('Frm_Instr')
end

function func_D816(address, data)
	local ARFCNIdx0=bit.band(data,0x3FF)
	local GainIdx0=bit.rshift(bit.band(data,0xFC00),10)
	return string.format('Frm_Instr: GainIdx0=%d, ARFCNIdx0=%d',GainIdx0,ARFCNIdx0)
end

function func_D817(address, data)
	local ARFCNIdx1=bit.band(data,0x3FF)
	local GainIdx1=bit.rshift(bit.band(data,0xFC00),10)
	return string.format('Frm_Instr: GainIdx1=%d, ARFCNIdx1=%d',GainIdx1,ARFCNIdx1)
end

function func_D818(address, data)
	local ARFCNIdx2=bit.band(data,0x3FF)
	local GainIdx2=bit.rshift(bit.band(data,0xFC00),10)
	return string.format('Frm_Instr: GainIdx2=%d, ARFCNIdx2=%d',GainIdx2,ARFCNIdx2)
end

function func_D819(address, data)
	local ARFCNIdx3=bit.band(data,0x3FF)
	local GainIdx3=bit.rshift(bit.band(data,0xFC00),10)
	return string.format('Frm_Instr: GainIdx3=%d, ARFCNIdx3=%d',GainIdx3,ARFCNIdx3)
end

function func_D81A(address, data)
	local ARFCNIdx4=bit.band(data,0x3FF)
	local GainIdx4=bit.rshift(bit.band(data,0xFC00),10)
	return string.format('Frm_Instr: GainIdx4=%d, ARFCNIdx4=%d',GainIdx4,ARFCNIdx4)
end

function func_D81B(address, data)
	local CellIndex=bit.band(data,0x1F)
	local FrameType=bit.rshift(bit.band(data,0x7E00),9)
	return string.format('Frm_Instr: CellIndex=%d, FrameType=%d',CellIndex,FrameType)
end

function func_D820(address, data)
	if (data<0x7FFF) then -- positive
		return string.format('QBC offset=%d',data)
	else
		return string.format('QBC offset=%d',data-0xFFFF-1)
	end
end

-- used in func_D830
-- NOTE:  by default, lua table start with index=1
MCU_CMD=
{
    [0]='CMDQ_PARAM_IS_READY',                -- 0x00
    'CMDQ_MPH_START_FRAME_PROCESSING',        -- 0x01
    'CMDQ_MPH_STOP_FRAME_PROCESSING',         -- 0x02
    'CMDQ_MPH_CAMPED',                        -- 0x03
    'CMDQ_TXBUF_READY',                       -- 0x04
    'CMDQ_SET_CIPHER_ON',                     -- 0x05
    'CMDQ_SET_AUDIO_ENABLE',                  -- 0x06
    'CMDQ_SET_CODEC_ENABLE',                  -- 0x07
    'CMDQ_SET_ENGINEERING_AUDIO_PARAMETERS',  -- 0x08
    'CMDQ_SOUND_DEVICE_SWITCH',               -- 0x09
    'CMDQ_SET_VOLUME',                        -- 0x0A
    'CMDQ_SPECIAL_TONE_GENERATION',           -- 0x0B
    'CMDQ_SET_LOOPBACK',                      -- 0x0C
    'CMDQ_SET_DTX_ENABLE',                    -- 0x0D
    'CMDQ_SET_PCS1900_BAND',                  -- 0x0E
    'CMDQ_GENERIC_TONE_GENERATION',           -- 0x0F
    'CMDQ_DSP_RAM_WRITE',                     -- 0x10
    'CMDQ_DSP_MEMORY_READ',                   -- 0x11
    'CMDQ_STOP_TONE',                         -- 0x12
    'CMDQ_DAI_TEST',                          -- 0x13
    'CMDQ_MUTE_SWITCH',                       -- 0x14
    'CMDQ_SET_GPRS_DYN_TXPWR_CONTROL_IDLE',   -- 0x15
    'CMDQ_SET_GPRS_DYN_TXPWR_CONTROL_TBF',    -- 0x16
    'CMDQ_SET_GPRS_DYN_TXPWR_CONTROL_ALPHA',  -- 0x17
    'CMDQ_SET_GPRS_DYN_TXPWR_CONTROL_PMAX',   -- 0x18
    'CMDQ_SET_GPRS_DYN_TXPWR_CONTROL_GAMMA',  -- 0x19
    'CMDQ_SET_GPRS_DYN_TXPWR_CONTROL_START',  -- 0x1A
    'CMDQ_ARM_READY',                         -- 0x1B
    'CMDQ_MIDIBUF_READY',                     -- 0x1C
    'CMDQ_SET_MAX_TXPWR',                     -- 0x1D
    'CMDQ_SET_CIPHER_KEY',                    -- 0x1E
    'CMDQ_WAKEUP_DSP',                        -- 0x1F
    'CMDQ_SET_CHANNEL_MODE',                  -- 0x20
    'CMDQ_SET_GAIN_NOT_CAMP',                 -- 0x21
    'CMDQ_DSP_TESTING_TXRX',                  -- 0x22
    'CMDQ_DSP_RAT_CHANGE',              	  -- 0x23
    'CMDQ_AUDIO_PIN_TEST',                    -- 0x24
    'CMDQ_VOICECALLDATABUF_READY',            -- 0x25
    'CMDQ_ECHO_TEST_TONE_GENERATE',           -- 0x26
    'CMDQ_32K_OSCILLATOR_CALIBRATION',        -- 0x27
    'CMDQ_ECHO_CANCEL_SWITCH',                -- 0x28
    'CMDQ_SET_DIGITAL_AUDIO_GAIN',            -- 0x29
    'CMDQ_SET_AFC_ENABLE',                    -- 0x2a
    'CMDQ_START_VOICE_UPLOAD',                -- 0x2b
    'CMDQ_START_VOICE_LOOPBACK',              -- 0x2c
    'CMDQ_NEED_DSP_IRQ',                      -- 0x2d
    'CMDQ_DOWNLINK_MUTE',                     -- 0x2e
    'CMDQ_ENABLE_DSP_SLEEP',                  -- 0x2f
    'CMDQ_AAC_DECODE_HEADER',                 -- 0x30
    'CMDQ_AAC_DECODE_FRAME',                  -- 0x31
    'CMDQ_AAC_BRAKE',                         -- 0x32
	'CMDQ_SET_AFS_ICM_ACS',			 		  -- 0x33
    'CMDQ_SET_AFS_THRESH_HYST',				  -- 0x34
    'CMDQ_TBD1',							  -- 0x35
    'CMDQ_TBD2',							  -- 0x36
    'CMDQ_TBD3',							  -- 0x37
    'CMDQ_DEDICATE_EST',              		  -- 0x38
    'CMDQ_RAT_CHANGE_BACK',                   -- 0x39
    'CMDQ_RAT_CHANGE_BACK_FINISH',            -- 0x3A
    'CMDQ_RAT_CHANGE_DUMMY',                  -- 0x3b
    'CMDQ_IQ_CATCH',                          -- 0x3c
    'CMDQ_RXADC_DCVOLTAGE',                   -- 0x3d
    'CMDQ_MCU_ASSERT_DSP',			          -- 0x3e
    'CMDQ_PRACH_DELAY',                       -- 0x3f
    'CMDQ_END'                                -- 0x40
}

function func_D830(address, data)
	if MCU_CMD[data] then
		return string.format('MCU_CMD: %s',MCU_CMD[data])
	else
		return string.format('MCU_CMD: 0x%X',data)
	end
end

function func_D831(address, data)
	return string.format('MCU_CMD P0=0x%X',data)
end

function func_D832(address, data)
	return string.format('MCU_CMD P1=0x%X',data)
end

function func_D833(address, data)
	return string.format('MCU_CMD P2=0x%X',data)
end

function func_D834(address, data)
	return string.format('MCU_CMD P3=0x%X',data)
end

-- used in func_D840
-- NOTE:  by default, lua table start with index=1
DSP_RSP=
{
    [0]='STATUS_DSP_IS_READY',      --0x00
    'STATUS_RX_BLOCK_IS_READY',     --0x01
    'STATUS_BLOCK_SEND',            --0x02
    'STATUS_MEAS_REPORT',           --0x03
    'STATUS_MCU_SCHEDULE_INT',      --0x04
    'STATUS_POWERMONQUEUE_IS_FULL', --0x05
    'STATUS_DSPOverRunMCU',         --0x06
    'STATUS_GPRS_MEAS_IS_READY',    --0x07
    'STATUS_DSP_SCH_DECODE_FAIL',   --0x08
    'STATUS_MCU_ADJUST_DELTA_FN',   --0x09
    'STATUS_DSP_WAKEUP',            --0x0a
    'STATUS_ATCMD_READW_CNF',       --0x0b
    'STATUS_ECHO_TEST_FINISHED',    --0x0c
    'STATUS_UPLOAD_VOICE_SEND',     --0x0d
    'STATUS_VOICE_LOOPBACK_IND',    --0x0e
    'STATUS_PARAM_COPY_DONE',       --0x0f
    'STATUS_RTC_CALI_RESULT',       --0x10
    'STATUS_DOWNLOAD_VOICE_RECV',   --0x11
    'STATUS_DSP_STEAL_FCCH',        --0x12
    'STATUS_DSP_AUDIO_DELAY',       --0x13
    'STATUS_XXX',                   --0x14
    'STATUS_SBC_ENC_CONFIG_IND',    --0x15
    'STATUS_SBC_ENC_FRAME_IND',     --0x16
    'STATUS_AAC_DECODE_HEADER',     --0x17
    'STATUS_AAC_DECODE_FRAME',      --0x18
    'STATUS_GPS_CIPHER',            --0x19
    'STATUS_FRAME_QUEUE_EMPTY',	    --0x1A
    'STATUS_DIRECTCURRENT_RPT',     --0x1B
    'NOT_DEFINED',     				--0x1C
    'NOT_DEFINED',     				--0x1D
    'NOT_DEFINED',     				--0x1E
    'NOT_DEFINED',     				--0x1F
    'STATUS_TD_PWR_SWEEP',			--0x20
    'STATUS_TD_SYNC',				--0x21
    'STATUS_ADJUST_TD_AGIN',		--0x22
    'STATUS_TD_MEAS',				--0x23
    'STATUS_OTD_CHANGE',			--0x24
    'STATUS_MODE_CHANGE_RSP',		--0x25
    'STATUS_IR_MEMORY_LACK_IND'		--0x26
}

function func_D840(address, data)
	if DSP_RSP[data] then
		return string.format('DSP_RSP: %s',DSP_RSP[data])
	else
		return string.format('DSP_RSP: 0x%X',data)
	end
end

function func_D841(address, data)
	return string.format('DSP_RSP P0=0x%X',data)
end

function func_D842(address, data)
	return string.format('DSP_RSP P1=0x%X',data)
end

function func_D843(address, data)
	return string.format('DSP_RSP P2=0x%X',data)
end

function func_D844(address, data)
	return string.format('DSP_RSP P3=0x%X',data)
end

function func_D850(address, data)
	return string.format('Current Agc Gain (control register value)')
end

function func_D851(address, data)
	return string.format('Current no Agc Gain')
end

function func_D860(address, data)
	return string.format('RSSI=0x%X',data)
end

function func_D870(address, data)
	return string.format('AFC_CONTROL=0x%X',data)
end

function func_D880(address, data)
	return string.format('b_point=%d',data)
end

function func_D910(address, data)
	return string.format('CCCH: HW_ERROR_REG')
end

function func_D920(address, data)
	return string.format('Data from Mcu')
end

function func_DD37(address, data)
	return string.format('run_rx_task: QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_DD3E(address, data)
	return string.format('run_edge_encoder_task: QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_DD35(address, data)
	return string.format('run_frame_eng_task: QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_DD38(address, data)
	return string.format('run_rx_deinterleaver_task: QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_DD39(address, data)
	return string.format('run_rx_dispatch_task: QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_DD33(address, data)
	return string.format('run_tx_encoder_task(): QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_DD36(address, data)
	return string.format('run_tx_interleaver_task(): QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_DD3B(address, data)
	return string.format('run_mcu_to_dsp_task(): QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_D77A(address, data)
	return string.format('Flag')
end

function func_D720(address, data)
	return string.format('NOT_IN_SLEEP: DSP_sleep_frame_counter=%d',data)
end

function func_D721(address, data)
	return string.format('RF_IN_SLEEP: DSP_sleep_frame_counter=%d',data)
end

function func_D722(address, data)
	return string.format('DSP_IN_SLEEP: DSP_sleep_frame_counter=%d',data)
end

function func_D723(address, data)
	return string.format('IDLE_IN_SLEEP: DSP_sleep_frame_counter=%d',data)
end

function func_D724(address, data)
	return string.format('IDLE_AFTER_WAKEUP: DSP_sleep_frame_counter=%d',data)
end

function func_D725(address, data)
	return string.format('RF_IN_SLEEP_DEACT: DSP_sleep_frame_counter=%d',data)
end

function func_D726(address, data)
	return string.format('DSP_IN_SLEEP_DEACT: DSP_sleep_frame_counter=%d',data)
end

-- used in func_DB27
-- NOTE:  by default, lua table start with index=1
RX_Mode=
{
    [0]='RX_NO_MODE',
    'RX_PWRSCAN_MODE',
    'RX_FCCH_DET_MODE',
    'RX_SCH_MODE',
    'RX_NORMAL_MODE',
    'RX_GPRS_MODE',
    'RX_GPRS_MEAS_MODE',
    'RX_DTCH_MODE',
    'RX_FCCH_FREQ_EST',
    'RX_EDGE_MODE',
    'RX_EDGE_MEAS_MODE'
}

function func_DB27(address, data)
	if RX_Mode[data] then
		return string.format('RX_Mode: %s',RX_Mode[data])
	else
		return string.format('RX_Mode: 0x%X',data)
	end
end

function func_DB11(address, data)
	return string.format('prepare_search_event()')
end

function func_DB12(address, data)
	return string.format('search_action_type')
end

function func_D821(address, data)
	return string.format('frame_timing_adjust(): GL_timing_adjust_limit')
end

function func_DB21(address, data)
	if RX_TASK_STATUS[data] then
		return string.format('run_FCCH_SCH_detection(): GL_RX_task_status=%s',RX_TASK_STATUS[data])
	else
		return string.format('run_FCCH_SCH_detection(): GL_RX_task_status=0x%X',data)
	end
end

function func_DB30(address, data)
	return string.format('run_FCCH_detection(): fg_FCCH_1_detect_counter')
end

function func_DB31(address, data)
	return string.format('FCCH_burst_mod8_count')
end

function func_DB16(address, data)
	return string.format('FCCH_detection_counter')
end

function func_DB17(address, data)
	return string.format('service_SEARCH(): GL_RX_task_status')
end

function func_DB32(address, data)
	return string.format('FCCH_DET!')
end

function func_DB70(address, data)
	return string.format('bg_freq_estimation_task(): loop')
end

function func_DB72(address, data)
	return string.format('freq_set_afc_value()')
end

function func_D77C(address, data)
	return string.format('service_SLEEP(): SLEEP_FRAME_COUNT=0x%X',data)
end

function func_D911(address, data)
	return string.format('SCH: CDC_ERR_REG=0x%X',data)
end

function func_D912(address, data)
	return string.format('TCH_FS: CDC_ERR_REG=0x%X',data)
end

function func_D913(address, data)
	return string.format('TCH_EFS: CDC_ERR_REG=0x%X',data)
end

function func_D914(address, data)
	return string.format('SACCH: CDC_ERR_REG=0x%X',data)
end

function func_D915(address, data)
	return string.format('FACCH: CDC_ERR_REG=0x%X',data)
end

function func_D916(address, data)
	return string.format('PDTCH_CS1: CDC_ERR_REG=0x%X',data)
end

function func_D77B(address, data)
	return string.format('DSP_SLP_CNTL(BD75)')
end

function func_D917(address, data)
	return string.format('PDTCH_CS2: CDC_ERR_REG=0x%X',data)
end

function func_D918(address, data)
	return string.format('PDTCH_CS3: CDC_ERR_REG=0x%X',data)
end

function func_D919(address, data)
	return string.format('PDTCH_CS4: CDC_ERR_REG=0x%X',data)
end

function func_D500(address, data)
	if (data==0x10) then
		return string.format('dummy_pch_detection sucess? Yes')
	elseif (data==0) then
		return string.format('dummy_pch_detection sucess? No')
	else
		return string.format('dummy_pch_detection sucess?')
	end
end

function func_D501(address, data)
	return string.format('MCU_RX_burst_number')
end

function func_D502(address, data)
	return string.format('If Success: MCU_RX_burst_number')
end

function func_D503(address, data)
	return string.format('Pch3BurstRfOperation(): g_3burst_pch_enable')
end

function func_D506(address, data)
	return string.format('run_dummy_pch_detection(): _now.RX_burst_number')
end

function func_D507(address, data)
	return string.format('If(burst_index==0): GL_is_need_to_check_burst1')
end

function func_D50A(address, data)
	return string.format('If(burst_index==1): GL_is_need_to_check_burst1')
end

function func_D50C(address, data)
	return string.format('DummyPchRxSuccess(): g_rx_burst_num=%d',data)
end

function func_D50D(address, data)
	return string.format('rx_axc_idle_adapt(): g_rx_burst_num')
end

function func_D50E(address, data)
	return string.format('aero2_freq_track_common(): g_rx_burst_num')
end

function func_D50F(address, data)
	return string.format('rx_agc_slow_tracking(): g_rx_burst_num')
end

function func_D510(address, data)
	return string.format('Pch3BurstDetection(): Zero Byte Cnt')
end

function func_D511(address, data)
	return string.format('slow_time_tracking(): g_rx_burst_num')
end

-- NOTE:  by default, lua table start with index=1
Event_Type=
{
    [0]='NOP_EVENT',
    'RX_EVENT',
    'MONITOR_EVENT',
    'SEARCH_EVENT',
    'NORMAL_TX',
    'RACH1_TX',
    'RACH2_TX'
}
function func_DC40(address, data)
	local etype=bit.band(data,0x7)
	if Event_Type[etype] then
		return string.format('EventTable Low Word=0x%X; Event Type: %s',data,Event_Type[etype])
	else
		return string.format('EventTable Low Word=0x%X',data)
	end
end

function func_D855(address, data)
	local current_gain_index=bit.rshift(data,8)
	local band_info=bit.band(data,0xFF)
	return string.format('cal_agc_gain(): current_gain_index=%d, band_info=%d',current_gain_index,band_info)
end

function func_D512(address, data)
	return string.format('3BurstPchErr: HW_ERROR_REG')
end

function func_D514(address, data)
	return string.format('GiveUpPchRxEvent(): number_of_event')
end

function func_D516(address, data)
	return string.format('GiveUpPchRxEvent(): GL_last_rx_tx_slot_index')
end

function func_D517(address, data)
	return string.format('service_RX_patch(): sucess or not')
end

function func_D51A(address, data)
	return string.format('IDLE_C_compute(): g_rx_burst_num')
end

function func_D51B(address, data)
	return string.format('IDLE_C_compute(): C_block')
end

function func_D221(address, data)
	return string.format('aero2_freq_track_common(): z32')
end

function func_D51D(address, data)
	return string.format('DMI_write_rx_data_buffer_patch(): is_dummy_pch')
end

function func_DC41(address, data)
	return string.format('EventTable Word2')
end

function func_D714(address, data)
	return string.format('sleep_wakeup_dsp_in_sleep()')
end

function func_D717(address, data)
	return string.format('HW_MMIO_ICU_ISR_REG(BF40)')
end

function func_D749(address, data)
	return string.format('dsp_wakeup(): DSP_SLP_CNTL=0x%X',data)
end

function func_D741(address, data)
	if (bit.band(data,0x1000)==0x1000) then
		return string.format('DSP_STATE: 0x%X Scheduler Sleeped',data)
	else
		return string.format('DSP_STATE: 0x%X',data)
	end
end

function func_D751(address, data)
	return string.format('dsp_wakeup: sleep_left_count=%d',data)
end

function func_D752(address, data)
	return string.format('dsp_wakeup: S_total_sleep_count=%d',data)
end

function func_D760(address, data)
	return string.format('mcu_fiq_isr(): cmd_id')
end

function func_D761(address, data)
	return string.format('STC_DSP_QBC(BD5E)')
end

function func_D846(address, data)
	return string.format('REF_messages(): RefResp_block.Resp_done')
end

function func_D51E(address, data)
	return string.format('GiveUpPchRxFrameOp(): Frame Queue In Ptr')
end

function func_D51F(address, data)
	return string.format('GiveUpPchRxFrameOp(): Frame Queue out Ptr')
end

function func_D51C(address, data)
	return string.format('DMI_write_rx_data_buffer_patch(): _notsend2mcu_cnt')
end

function func_D871(address, data)
	return string.format('MCU_TX_pwrlev=%d',data)
end

function func_D872(address, data)
	return string.format('MCU_timing_advance=%d',data)
end

function func_D521(address, data)
	return string.format('DMI_write_rx_data_buffer(): ReportFN_H')
end

function func_D522(address, data)
	return string.format('DMI_write_rx_data_buffer(): ReportFN_L')
end

function func_DD46(address, data)
	return string.format('run_vocoder_task(): QBC=%d',data)
end

function func_D883(address, data)
	local SNR_INT=bit.rshift(bit.band(data,0xFF00),8)
	if (SNR_INT>0x7F) then
		return string.format('SNR_in_db=%d',SNR_INT-256)
	else
		return string.format('SNR_in_db=%d',SNR_INT)
	end
end

function func_D600(address, data)
	return string.format('run_rx_task(): Baseband_DC_offset_I=%d; ',data)
end

function func_D601(address, data)
	return string.format('run_rx_task(): Baseband_DC_offset_Q=%d',data)
end

function func_D780(address, data)
	return string.format('service_SLEEP(): g_dummypch_addsleep_cnt=%d',data)
end

function func_DFFE(address, data)
	return string.format('dual_sim_fr_interpreter(): MCU_RF_operation_next')
end

function func_DFA1(address, data)
	return string.format('GSM_operation_common_patch(): use_AGC_flag')
end

function func_DFB5(address, data)
	return string.format('freq_predict_func(): freq_delta')
end

function func_DF98(address, data)
	return string.format('dualsim_info.mph_exchanged')
end

function func_DFB6(address, data)
	return string.format('freq_predict_func(): freq_delta')
end

function func_D8D0(address, data)
	return string.format('cell_database_get_timing_offset(0): timing_offset')
end

function func_DC80(address, data)
	return string.format('frame_timing_adjust(): serving_cell_timing_adjust_v')
end

function func_D515(address, data)
	return string.format('GiveUpPchRxEvent(): number_of_event=0!!ASSERT!!')
end

function func_D223(address, data)
	return string.format('freq_predict_func(): rfi_info.rfi_vars->frame_count')
end

function func_DFB0(address, data)
	return string.format('aero2_freq_track_common(): freq_offset')
end

function func_DB63(address, data)
	return string.format('[6600D]GSM_operation_common(): frame_counter')
end

function func_DB60(address, data)
	return string.format('[D6]predict_frequency_step(): i* sign')
end

function func_DB67(address, data)
	return string.format('[D6]: predict_step_prev')
end

function func_DB65(address, data)
	return string.format('[D6]run_freq_track(): burst_snr_det*0x100')
end

function func_DD41(address, data)
	return string.format('bg_fcch_rev_detection_task()-><-')
end

function func_DB33(address, data)
	return string.format('SEARCH_context.fw_boundary')
end

function func_DB39(address, data)
	return string.format('idle_fcch_search_calculate_frame_boundary()')
end

-- used in func_DB25/DB21
-- NOTE:  by default, lua table start with index=1
RX_TASK_STATUS=
{
    [0]='NO_TASK',
     'FCCH_1_BURST_DONE',
     'FCCH_1_DET_SUCCESS',
     'SCH_1_BURST_DONE',
     'SCH_1_DECO_FAIL',
     'SCH_1_DECO_SUCCESS',
     'RX_CCH_DECODE',
     'SCH_1_TO_FCCH_2_TRANS',
     'FCCH_2_BURST_WAIT',
     'FCCH_2_BURST_DONE',
     'SCH_2_BURST_DONE',
     'SCH_2_DECO_FAIL',
     'SCH_2_DECO_SUCCESS',
     'FCCH_2_DET_SUCCESS',
     'TCH_FCCH_BURST_DONE',
     'TCH_FCCH_DET_SUCCESS',
     'TCH_SCH_DECO_DONE',
     'TCH_SCH_DECO_SUCCESS',
     'TCH_SCH_DECO_FAIL',
     'SEARCH_DONE',
     'SCH_DECO_DONE',
     'SCH_DECO_SUCCESS',
     'SCH_DECO_FAIL',
     'SEARCH_NONE_STATE'
}

function func_DB25(address, data)
	if RX_TASK_STATUS[data] then
		return string.format('SearchRxDispatch(): GL_RX_task_status=%s',RX_TASK_STATUS[data])
	else
		return string.format('SearchRxDispatch(): GL_RX_task_status=0x%X',data)
	end
end

function func_DB26(address, data)
	return string.format('SearchRxDispatch(): bfi_flag')
end

function func_DD44(address, data)
	return string.format('bg_freq_estimation_task_patch(): QBC=%d, EventTable_Idx=%d',bit.band(data,0x3FFF),bit.rshift(bit.band(data,0xC000),14))
end

function func_D881(address, data)
	return string.format('w_point=%d',data)
end

function func_DB13(address, data)
	return string.format('service_SEARCH_sch_1_to_fcch_2()')
end

function func_DB14(address, data)
	return string.format('service_SEARCH_fcch_2_burst_wait(): wait_FCCH_count')
end

function func_DB10(address, data)
	return string.format('SearchRxDispatch_fcch_2_burst_done(): QBC_offset')
end

function func_DB18(address, data)
	return string.format('service_SEARCH(): -GL_FCCH_QBC_report')
end

function func_DB19(address, data)
	return string.format('service_SCH_RFOP(): timing_adjust_current_frame')
end

function func_DB1C(address, data)
	return string.format('service_TCH_SEARCH(): FCCH_detection_counter')
end

function func_DB1D(address, data)
	return string.format('service_TCH_SEARCH(): GL_RX_task_status')
end

function func_DB1A(address, data)
	return string.format('service_TCH_SEARCH_tch_fcch_burst_done(): -qbc_adju')
end

function func_D873(address, data)
	return string.format('change_GPRS_tx_power(): real_tx_pwrlev index=%d',data)
end

function func_DB20(address, data)
	return string.format('run_FCCH_SCH_detection_fcch_1_burst_done(): FAIL!')
end

function func_D950(address, data)
	return string.format('fr_instr_interpreter(): counter_in=%d',data)
end

function func_D951(address, data)
	return string.format('fr_instr_interpreter(): counter=%d',data)
end

function func_D961(address, data)
	return string.format('EDGE_Header1_BFI=%d',data)
end

function func_D962(address, data)
	return string.format('EDGE_Header2_BFI=%d',data)
end

function func_D963(address, data)
	return string.format('EDGE_Header3_BFI=%d',data)
end

function func_D882(address, data)
	return string.format('ve_bit=%d',data)
end

-- used in func_DF01
-- NOTE:  by default, lua table start with index=1
EDGE_MODE_TYPE=
{
    [0]='UNKNOWN',
    'GMSK',
    '8PSK'
}

function func_DF01(address, data)
	if EDGE_MODE_TYPE[data] then
		return string.format('EDGE_Mode_Type: %s',EDGE_MODE_TYPE[data])
	else
		return string.format('EDGE_Mode_Type: 0x%X',data)
	end
end

function func_D5FE(address, data)
	return string.format('adjust_channel_mask_threshold()=%d',data)
end

function func_D886(address, data)
	return string.format('target b-point=%d',data)
end

function func_DC4B(address, data)
	return string.format('multislot agc, delta_blk_relev=%d',data-0xFFFF-1)
end

function func_DF77(address, data)
	return string.format('run_FCCH_detection(): rxlev=%d',data-0xFFFF-1)
end

function func_D9A0(address, data)
	return string.format('run_FCCH_detection_apart_corr(): enter STC_DSP_QBC=%d',data)
end

function func_D9A1(address, data)
	return string.format('run_FCCH_detection_apart_corr(): exit  STC_DSP_QBC=%d',data)
end

-- used in func_DCFD/DCFC
-- NOTE:  by default, lua table start with index=1
CHANNEL_TYPE=
{
    [0]='CCCH',
    'SCH',
    'TCH_FS',
    'TCH_EFS',
    'SACCH',
    'FACCH',
    'PDTCH_CS1',
    'PDTCH_CS2',
    'PDTCH_CS3',
    'PDTCH_CS4',
    'PTCCH',
    'DTCH_F24',
    'DTCH_F48',
    'DTCH_F96',
    'DTCH_F114',
    'NON_EXISTING_TYPE',
    'AMR',
    'AMR_SID_1ST',
    'AMR_OST',
    'AMR_SID_UPD',
    'AMR_RAT',
    'AMR_4_75',
    'AMR_5_15',
    'AMR_5_9',
    'AMR_6_7',
    'AMR_7_4',
    'AMR_7_95',
    'AMR_10_2',
    'AMR_12_2',
    'PDTCH_EDGE',
    'PDTCH_MCS1',
    'PDTCH_MCS2',
    'PDTCH_MCS3',
    'PDTCH_MCS4',
    'PDTCH_MCS5',
    'PDTCH_MCS6',
    'PDTCH_MCS7',
    'PDTCH_MCS8',
    'PDTCH_MCS9'
}

function func_DCFD(address, data)
	if CHANNEL_TYPE[data] then
		return string.format('run_tx_encoder_task: CHANNEL_TYPE=%s',CHANNEL_TYPE[data])
	else
		return string.format('run_tx_encoder_task: CHANNEL_TYPE=0x%X',data)
	end
end

function func_DCFC(address, data)
	if CHANNEL_TYPE[data] then
		return string.format('tx_encoder: CHANNEL_TYPE=%s',CHANNEL_TYPE[data])
	else
		return string.format('tx_encoder: CHANNEL_TYPE=0x%X',data)
	end
end

-- used in func_DCFB
-- NOTE:  by default, lua table start with index=1
MODE_TYPE=
{
	[0]='NORMAL_TCH_MODE',
    'ACCESS_MODE',
    'SEARCH_MODE',
    'IDLE_MODE',
    'LOOPBACK_MODE',
    'GPRS_MODE',
    'DTCH_MODE',
    'EDGE_MODE'
}

function func_DCFB(address, data)
	if MODE_TYPE[data] then
		return string.format('tx_encoder: MODE_TYPE=%s',MODE_TYPE[data])
	else
		return string.format('tx_encoder: MODE_TYPE=0x%X',data)
	end
end

function func_D888(address, data)
	if (bit.band(data,0xFF)==1) then -- nagative
		return string.format('frequency_bias=-%d',bit.rshift(bit.band(data,0xFF00),8))
	else -- positive
		return string.format('frequency_bias=%d',bit.rshift(bit.band(data,0xFF00),8))
	end
end

function func_D885(address, data)
	local GL_RX_int_count=bit.rshift(bit.band(data,0xF000),12)
	local GL_equa_int_count=bit.rshift(bit.band(data,0x0F00),8)
	local GL_GPRS_deinter_int_count=bit.rshift(bit.band(data,0x00F0),4)
	local GL_GPRS_dispatch_int_count=bit.band(data,0xF)
	return string.format('init_frame_eng(): Rx_int_cnt=%d,Equa_int_cnt=%d,GPRS_deinter_int_cnt=%d,GPRS_dispatch_int_cnt=%d',GL_RX_int_count,GL_equa_int_count,GL_GPRS_deinter_int_count,GL_GPRS_dispatch_int_count)
end

function func_D894(address, data)
	return string.format('DTX_Rxlev=%d dBm',data-110)
end

function func_D895(address, data)
	return string.format('DTX_SNR=%d',data)
end

function func_D865(address, data)
	return string.format('report_rxlev=%d',data)
end

function func_D770(address, data)
	return string.format('DSP_Sleep_Frame_Counter=%d',data)
end

function func_D77E(address, data)
	return string.format('Power Flag=0x%X',data)
end

function func_D89A(address, data)
	return string.format('RX: gain_index=%d, GL_RX_int_count=%d',bit.rshift(bit.band(data,0xFF00),8),bit.band(data,0xFF))
end

function func_D800(address, data)
	return string.format('idle percentage=%d',data)
end

function func_D971(address, data)
	return string.format('EDGE MCS1 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_D972(address, data)
	return string.format('EDGE MCS2 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_D973(address, data)
	return string.format('EDGE MCS3 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_D974(address, data)
	return string.format('EDGE MCS4 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_D975(address, data)
	return string.format('EDGE MCS5 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_D976(address, data)
	return string.format('EDGE MCS6 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_D977(address, data)
	return string.format('EDGE MCS7 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_D978(address, data)
	return string.format('EDGE MCS8 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_D979(address, data)
	return string.format('EDGE MCS9 CRC: error_count=%d, bfi=%d',bit.band(data,0xFFF),bit.rshift(bit.band(data,0xF000),12))
end

function func_DFF3(address, data)
	if (data==1) then
		return string.format('frame_eng(): In traffic? Yes')
	elseif (data==0) then
		return string.format('frame_eng(): In traffic? No')
	else
		return string.format('frame_eng(): In traffic? 0x%X',data)
	end
end

function func_D890(address, data)
	return string.format('empty frame detected? %d; local_bfi_flag!=0? %d; RSSI_not_saturate? %d; active_blk_num=%d',bit.rshift(bit.band(data,0x1000),12),bit.rshift(bit.band(data,0x100),8),bit.rshift(bit.band(data,0x10),4),bit.band(data,0xF))
end

function func_D891(address, data)
	return string.format('bfi_array[3]? %d; bfi_array[2]? %d; bfi_array[1]? %d; bfi_array[0]? %d;',bit.rshift(bit.band(data,0x1000),12),bit.rshift(bit.band(data,0x100),8),bit.rshift(bit.band(data,0x10),4),bit.band(data,0xF))
end

function func_D730(address, data)
	return string.format('sleep_calibration_control(): Start 32k Cal')
end

function func_D732(address, data)
	return string.format('sleep_calibration_control(): CAL_STATUS=0x%X',data)
end

function func_D731(address, data)
	return string.format('sleep_calibration_control(): CAL_STEP=0x%X',data)
end

function func_D828(address, data)
	return string.format('frame_timing_adjust(): GL_QBC_to_adjust=%d',bit.rshift(data,8))
end

function func_D787(address, data)
	return string.format('remainder_sleep_frame=%d',data)
end

function func_D788(address, data)
	return string.format('SLEEP_FRAME_COUNT=0x%X',data)
end

function func_D781(address, data)
	return string.format('DSP_STATE=0x%X',data)
end


-- used in func_DAA0
-- NOTE:  by default, lua table start with index=1
AUDIO_CMDQ=
{
	[0]='AUDIO_CMDQ_SET_AUDIO_ENABLE',
	'AUDIO_CMDQ_SET_CODEC_ENABLE',
	'AUDIO_CMDQ_SET_VOLUME',
	'AUDIO_CMDQ_SOUND_DEVICE_CHANGE',
	'AUDIO_CMDQ_SPECIAL_TONE_GENER',
	'AUDIO_CMDQ_GENERIC_TONE_GENER',
	'AUDIO_CMDQ_STOP_TONE',
	'AUDIO_CMDQ_MUTE_UL',
	'AUDIO_CMDQ_SPEAKER_OFF',
	'AUDIO_CMDQ_VOICE_UPLOAD',
	'AUDIO_CMDQ_VOICE_LOOPBACK',
	'AUDIO_CMDQ_VOICE_DOWNLOAD',
	'AUDIO_CMDQ_TEST_PIN_MODE',
	'AUDIO_CMDQ_ECHO_TEST',
	'AUDIO_CMDQ_GSM_TB1',
	'AUDIO_CMDQ_GSM_TB2',
	'AUDIO_CMDQ_GSM_TB3',
	'AUDIO_CMDQ_GSM_TB4',
	'AUDIO_CMDQ_GSM_TB5',
	'AUDIO_CMDQ_GSM_TB6',
	'AUDIO_CMDQ_GSM_TB7',
	'AUDIO_CMDQ_GSM_TB8',
	'AUDIO_CMDQ_GSM_TB9',
	'AUDIO_CMDQ_GSM_TB10',
	'AUDIO_CMDQ_GSM_TB11',
	'AUDIO_CMDQ_GSM_TB12',
	'AUDIO_CMDQ_GSM_TB13',
	'AUDIO_CMDQ_GSM_TB14',
	'AUDIO_CMDQ_GSM_TB15',
	'AUDIO_CMDQ_GSM_TB16',
	'AUDIO_CMDQ_GSM_TB17',
	'AUDIO_CMDQ_GSM_TB18',
	'AUDIO_CMDQ_CFG_AMR_ENCODER',
	'AUDIO_CMDQ_ENABLE_AMR_ENCODER',
	'AUDIO_CMDQ_CFG_AMR_DECODER',
	'AUDIO_CMDQ_ENABLE_AMR_DECODER',
	'AUDIO_CMD_TD_TB1',
}
function func_DAA0(address, data)
	if AUDIO_CMDQ[data] then
		return string.format('Audio command=%s',AUDIO_CMDQ[data])
	else
		return string.format('Audio command=0x%X',data)
	end
end

function func_DAA1(address, data)
	return string.format('AUDIO_CMD P0=0x%X',data)
end

function func_DAA2(address, data)
	return string.format('AUDIO_CMD P1=0x%X',data)
end

function func_DAA3(address, data)
	return string.format('AUDIO_CMD P2=0x%X',data)
end

function func_DAA4(address, data)
	return string.format('AUDIO_CMD P3=0x%X',data)
end

-- used in func_DAB0
-- NOTE:  by default, lua table start with index=1
AUDIO_RESP=
{
	[0]='AUDIO_STATUS_UPLOAD_VOICE_SEND',
    'AUDIO_STATUS_DOWNLOAD_VOICE_RECV',
    'AUDIO_STATUS_DSP_AMRDATA_READY',
}
function func_DAB0(address, data)
	if AUDIO_RESP[data] then
		return string.format('Audio response=%s',AUDIO_RESP[data])
	else
		return string.format('Audio response=0x%X',data)
	end
end

function func_DD8A(address, data)
	return string.format('[DM] CUR_FRAME_NUM=0x%X',data)
end

function func_DD8B(address, data)
	return string.format('[DM] TD_SYNC_FRAME_NUM=0x%X',data)
end

function func_DD8C(address, data)
	return string.format('[DM] TD_SYNC_FRAME_OFFSET=0x%X',data)
end

function func_DD8D(address, data)
	return string.format('[DM] DELTA_FRAME_NUM=0x%X',data)
end

function func_DD8E(address, data)
	return string.format('[DM] DELTA_QBC=0x%X',data)
end

function func_DD8F(address, data)
	return string.format('[DM] DELTA_ECC=0x%X',data)
end

function func_DD86(address, data)
	return string.format('[DM] AFC_VALUE=0x%X',data)
end

function func_DD87(address, data)
	return string.format('[DM] GSM_CELL_INFO=0x%X',data)
end

function func_DD88(address, data)
	return string.format('[DM] RAT_CHANGE_QBC=0x%X',data)
end

function func_DDA2(address, data)
	return string.format('[DM] GSM_SLEEP_FRAME_NUM=0x%X',data)
end

function func_DDA3(address, data)
	return string.format('[DM] GSM_SEARCH_FRAME_NUM=0x%X',data)
end

function func_DDA4(address, data)
	return string.format('[DM] GSM_TD_MEASURE_FRAME_NUM=0x%X',data)
end

function func_DDA5(address, data)
	return string.format('[DM] GSM_NORMAL_FRAME_NUM=0x%X',data)
end

function func_DD91(address, data)
	return string.format('[DM] NULL_FRAME_NUM=0x%X',data)
end

function func_DD92(address, data)
	return string.format('[DM] GSM_TIME_ACK_FN=0x%X',data)
end

function func_DD80(address, data)
	return string.format('[DM] DSP_sleep_state=0x%X',data)
end

function func_DD81(address, data)
	return string.format('[DM] SLEEP_FRAME_COUNT=0x%X',data)
end

function func_DD82(address, data)
	return string.format('[DM] MCU_FRAME_COUNT=0x%X',data)
end

function func_DD83(address, data)
	return string.format('[DM] FRAME_TIME_ADJUST=0x%X',data)
end

function func_DD84(address, data)
	return string.format('[DM] TD_MEAS_FRAME_COUNT=0x%X',data)
end

function func_DDA1(address, data)
	return string.format('[DM] INIT_TD_RF=0x%X',data)
end

function func_D551(address, data)
	dummy_total_paging_number_h=data
end

function func_D552(address, data)
	if not dummy_total_paging_number_h  then
  	return ""
  else
		return string.format('dummy_pch_statistic(): GL_total_paging_number=%d',bit.lshift(dummy_total_paging_number_h,16)+data)
	end
end

function func_D553(address, data)
	total_checked_dummy_pch_number_h=data
end

function func_D554(address, data)
	if not total_checked_dummy_pch_number_h  then
  	return ""
  else
		return string.format('dummy_pch_statistic(): GL_total_checked_dummy_pch_number=%d',bit.lshift(total_checked_dummy_pch_number_h,16)+data)
	end
end

function func_D555(address, data)
	return string.format('dummy_pch_statistic(): detect_dummy_pch_by_burst1=%d',data)
end

function func_D556(address, data)
	return string.format('dummy_pch_statistic(): GL_pch3burst_decodeErr_number=%d',data)
end

function func_D557(address, data)
	return string.format('dummy_pch_statistic(): GL_pch3burstErr4burstOk_number=%d',data)
end

-- used in func_D671
-- NOTE:  by default, lua table start with index=1
TD_MEAS_STATE=
{
	[0]='TD_MEAS_STATE_NULL',
	'TD_MEAS_STATE_CS_DWPTS',
	'TD_MEAS_STATE_ADJ_AGC',
	'TD_MEAS_STATE_CS_SYNCDL',
	'TD_MEAS_STATE_CS_MIDAMBLE',
	'TD_MEAS_STATE_TIMING_TRACKING',
	'TD_MEAS_STATE_CELL_MEAS',
	'TD_MEAS_STATE_SYNC_AND_CELL_MEAS',
	'TD_MEAS_STATE_POWERSWEEP'
}

function func_D671(address, data)
	if TD_MEAS_STATE[data] then
		return string.format('[DM] TD_MEAS_STATE=%s',TD_MEAS_STATE[data])
	else
		return string.format('[DM] TD_MEAS_STATE=0x%X',data)
	end
end

function func_D651(address, data)
	return string.format('[DM] TD_MEAS_IQ_AVE=%d',data)
end

function func_D690(address, data)
	return string.format('[DM] TD_MEAS_RSSI = -%d dBm',(bit.band(bit.bnot(data),0xFFFF)+1)/128)
end

function func_D682(address, data)
	return string.format('[DM] TD_MEAS rx_gain=0x%X',data)
end

function func_D683(address, data)
	return string.format('[DM] TD_MEAS RSSI=0x%X',data)
end

function func_D684(address, data)
	if (data==1) then
		return string.format('[DM] TD_MEAS RSSI STATE: GOOD')
	elseif (data==3) then
		return string.format('[DM] TD_MEAS RSSI STATE: BELOW_NOISE_LO')
	elseif (data==2) then
		return string.format('[DM] TD_MEAS RSSI STATE: SATURATE_HI')
	else
		return string.format('[DM] TD_MEAS RSSI STATE: 0x%X',data)
	end
end

function func_D8A1(address, data)
	return string.format('EDGE_rx_data_process(): data_bfi=%d, bsn[0]=0x%X',bit.rshift(data,15),bit.band(data,0x7FFF))
end

function func_D8A2(address, data)
	return string.format('EDGE_rx_data_process(): data_bfi=%d, bsn[1]=0x%X',bit.rshift(data,15),bit.band(data,0x7FFF))
end

function func_DC0F(address, data)
	if MODE_TYPE[data] then
		return string.format('tx_encoder: MODE_TYPE=%s',MODE_TYPE[data])
	else
		return string.format('tx_encoder: MODE_TYPE=0x%X',data)
	end
end

function func_DDD0(address, data)
	if GSM_TaskID[bit.rshift(data,8)] then
		return string.format('RTOS_CURRENT_TASK: CurTask = %s, CurPriority=%d',GSM_TaskID[bit.rshift(data,8)],bit.band(data,0xFF))
	else
		return string.format('RTOS_CURRENT_TASK: unknown CurTaskID = %d, CurPriority=%d',bit.rshift(data,8),bit.band(data,0xFF))
	end
end

function func_DDD1(address, data)
	if GSM_TaskID[bit.rshift(data,8)] then
		return string.format('RTOS_SET_TASK: Task = %s, task_run_num=%d',GSM_TaskID[bit.rshift(data,8)],bit.band(data,0xFF))
	else
		return string.format('RTOS_SET_TASK: unknown TaskID = %d, task_run_num=%d',bit.rshift(data,8),bit.band(data,0xFF))
	end
end

-------------------------------------------------------------------------------------------------------------------------------------------------
-- main interface function, load by C functions -------------------------------------
-- NOTE: !!!!!!!!!!!!!!!!!!!!DON'T CHANGE THIS FUNCTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
function dsp_desc( address, data )
	func_pt=func_list[address]

	if not func_pt then
		return ""
	else
		return func_list[address](address,data)
	end
end

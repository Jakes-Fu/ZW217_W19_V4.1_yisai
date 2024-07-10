--[[--------------------------------------------------------------------
lua script to dynamic interpret dsp log 

NOTE:  To add new log interpreting functions:
step 1:  Add item in gsm_item or td_item
step 2:  Fill the item(which added in step 1) with the content, such as "gsm_system_state" and so on ...
step 3:  Add function with the new Address, such as func_DB21 and so on ....


2009/12/04:  create, by teana.hu
--]]--------------------------------------------------------------------


gsm_item=
{
	"gsm_system_state",
	"gsm_serving_cell_info",
	"gsm_pich_info",
	"gsm_uplink_info",
	"gsm_downlink_info"	
}

gsm_system_state=
{
	["CHP Version"]=0xCCCC,
	["Software Version"]=0xCCCC,
	["RAT INFO"]=0xDB21,
	["L1 State"]=0xD0B1
}

gsm_serving_cell_info=
{
	["Uarfcn"]=0xD0CC,
	["Para ID"]=0xD0B4,
	["RSSI"]=0xCCCC,
	["K Cell"]=0xCCCC,
	["SFN"]=0xD0B0,
	["TS0 AGC"]=0xCCCC
}

gsm_pich_info=
{
	["BLER"]=0xD828,
	["PICH Period"]=0xCCCC,
	["TS"]=0xDC22,
	["N GAP"]=0xCCCC,
	["Lpi"]=0xDC2F,
	["q"]=0xDC2F,
	["Channel Info"]=0xDC2E,
	["SICH TS Info"]=0xDC24,
	["SICH Channel Info"]=0xDC24,
	["SCCH TS Info"]=0xDC24,
	["SCCH Channel Info"]=0xDC24,
	["CQI"]=0xD6480,
	["DSCH"]=0xDAA9
}

gsm_uplink_info=
{
	["TS"]=0xCCCC,
	["K Cell"]=0xCCCC,
	["SF"]=0xCCCC,
	["Channel Info"]=0xCCCC,
	["Mid Mode"]=0xCCCC,
	["TFCI"]=0xD615,
	["Tx Pow.(dbm)"]=0xD825,
	["TTI"]=0xCCCC,
	["TPC"]=0xD821,
	["SS"]=0xD840
}

gsm_downlink_info=
{
	["TS"]=0xD243,
	["K Cell"]=0xCCCC,
	["SF"]=0xCCCC,
	["Channel Info"]=0xCCCC,
	["Mid Mode"]=0xCCCC,
	["TFCI"]=0xD61A,
	["AGC"]=0xD243,
	["TTI"]=0xCCCC,
	["RSCP"]=0xD82E,
	["RSSI"]=0xD245,
	["SNR"]=0xCCCC,
	["SIR"]=0xD82A,
	["EVM(%)"]=0xD28C
}

td_item=
{
	"td_system_state",
	"td_serving_cell_info",
	"td_pich_info",
	"td_uplink_info",
	"td_downlink_info"
}

td_system_state=
{
	["CHP Version"]=0xCCCC,
	["Software Version"]=0xCCCC,
	["RAT INFO"]=0xEB21,
	["L1 State"]=0xE0B1
}

td_serving_cell_info=
{
	["Uarfcn"]=0xE0CC,
	["Para ID"]=0xE0B4,
	["RSSI"]=0xCCCC,
	["K Cell"]=0xCCCC,
	["SFN"]=0xE0B0,
	["TS0 AGC"]=0xCCCC
}

td_pich_info=
{
	["BLER"]=0xE828,
	["PICH Period"]=0xCCCC,
	["TS"]=0xEC22,
	["N GAP"]=0xCCCC,
	["Lpi"]=0xEC2F,
	["q"]=0xEC2F,
	["Channel Info"]=0xEC2E,
	["SICH TS Info"]=0xEC24,
	["SICH Channel Info"]=0xEC24,
	["SCCH TS Info"]=0xEC24,
	["SCCH Channel Info"]=0xEC24,
	["CQI"]=0xE648,
	["DSCH"]=0xEAA9
}

td_uplink_info=
{
	["TS"]=0xCCCC,
	["K Cell"]=0xCCCC,
	["SF"]=0xCCCC,
	["Channel Info"]=0xCCCC,
	["Mid Mode"]=0xCCCC,
	["TFCI"]=0xE615,
	["Tx Pow.(dbm)"]=0xE825,
	["TTI"]=0xCCCC,
	["TPC"]=0xE821,
	["SS"]=0xE840
}

td_downlink_info=
{
	["TS"]=0xE243,
	["K Cell"]=0xCCCC,
	["SF"]=0xCCCC,
	["Channel Info"]=0xCCCC,
	["Mid Mode"]=0xCCCC,
	["TFCI"]=0xE61A,
	["AGC"]=0xE243,
	["TTI"]=0xCCCC,
	["RSCP"]=0xE82E,
	["RSSI"]=0xE245,
	["SNR"]=0xCCCC,
	["SIR"]=0xE82A,
	["EVM(%)"]=0xE28C
}	
	

-- for Constellation Diagram & Channel Impulse Response Power
graph_info=
{
	["Constellation Diagram"]=0xE080,
	["Channel Impulse Response Power"]=0xE881,  
}
	
-- for bit-wise operations
local bit = require"bit"	

-- global function list table
func_list={}

-- initialize function list
function init_func_list()
	func_list[0xDB21]=func_DB21
	func_list[0xD0B1]=func_D0B1
	func_list[0xD0CC]=func_D0CC
	func_list[0xD0B4]=func_D0B4
	func_list[0xD0B0]=func_D0B0
	func_list[0xD828]=func_D828
	func_list[0xDC22]=func_DC22
	func_list[0xDC2F]=func_DC2F
	func_list[0xDC2E]=func_DC2E
	func_list[0xDC24]=func_DC24
	func_list[0xD648]=func_D648
	func_list[0xDAA9]=func_DAA9
	func_list[0xD615]=func_D615
	func_list[0xD825]=func_D825
	func_list[0xD821]=func_D821
	func_list[0xD840]=func_D840
	func_list[0xD243]=func_D243
	func_list[0xD61A]=func_D61A
	func_list[0xD82E]=func_D82E
	func_list[0xD245]=func_D245
	func_list[0xD82A]=func_D82A
	func_list[0xD28C]=func_D28C
	func_list[0xEB21]=func_EB21
	func_list[0xE0B1]=func_E0B1
	func_list[0xE0CC]=func_E0CC
	func_list[0xE0B4]=func_E0B4
	func_list[0xE0B0]=func_E0B0
	func_list[0xE828]=func_E828
	func_list[0xEC22]=func_EC22
	func_list[0xEC2F]=func_EC2F
	func_list[0xEC2E]=func_EC2E
	func_list[0xEC24]=func_EC24
	func_list[0xE648]=func_E648
	func_list[0xEAA9]=func_EAA9
	func_list[0xE615]=func_E615
	func_list[0xE825]=func_E825
	func_list[0xE821]=func_E821
	func_list[0xE840]=func_E840
	func_list[0xE243]=func_E243
	func_list[0xE61A]=func_E61A
	func_list[0xE82E]=func_E82E
	func_list[0xE245]=func_E245
	func_list[0xE82A]=func_E82A
	func_list[0xE28C]=func_E28C

end

-- data processing functions

function func_DB21(address, data, type)
	if (data==1 or data==2) then
		return string.format('SC88502A')
	elseif (data==3 or data==4) then
		return string.format('SC88503B')
	else
		return ''
	end
end

function func_D0B1(address, data, type)
	if (data==0x2) then
		return string.format('BCH')
	elseif (data==0x4 or data==0x8) then
		return string.format('SCCH')
	elseif (data==0x10 or data==0x20) then
		return string.format('DPCH')
	elseif (data==0x40 or data==0x80) then
		return string.format('HSDPA')
	elseif (data==0x100 or data==0x200) then
		return string.format('PICH')
	elseif (data==0x400 or data==0x800) then
		return string.format('FPACH')
	elseif (data==0x50 or data==0x60 or data==0x90 or data==0xA0) then
		return string.format('HSDPA&DPCH')
	elseif (data==0x0A or data==0x06) then
		return string.format('FACH&BCH')
	elseif (data==0x404 or data==0x808 or data==0x408 or data==0x804) then
		return string.format('FACH&FPACH')
	else
		return ''
	end
end

function func_D0CC(address, data, type)
	return string.format('0x%X',data)
end

function func_D0B4(address, data, type)
	return string.format('0x%X',data)
end

function func_D0B0(address, data, type)
	return string.format('0x%X',data)
end

function func_D828(address, data, type)
	return string.format('%d',data/0x8000)
end

function func_DC22(address, data, type)
	return string.format('%d', bit.band(data,0x00FF))
end

function func_DC2F(address, data, type)
	if (type=="Lpi") then
		return string.format('%d',bit.rshift(bit.band(data,0xFF00),8))
	elseif (type=="q") then
		return string.format('%d',bit.band(data,0x00FF))
	else
		return ''
	end
end

function func_DC2E(address, data, type)
	return string.format('0x%X,0x%X',bit.band(data,0x00FF), bit.rshift(bit.band(data,0xFF00),8))
end

function func_DC24(address, data, type)
	if (type=="SICH TS Info") then
		return string.format('%d',bit.band(data,0x000F))
	elseif (type=="SICH Channel Info") then
		return string.format('%d',bit.rshift(bit.band(data,0x00F0),4))
	elseif (type == "SCCH TS Info") then
		return string.format('%d',bit.rshift(bit.band(data,0x0F00),8))
	elseif (type=="SCCH Channel Info") then
		return string.format('%d',bit.rshift(bit.band(data,0xF000),12))
	else
		return ''
	end
end

function func_D648(address, data, type)
	return string.format('%d',bit.rshift(bit.band(data,0xFF00),8))
end

function func_DAA9(address, data, type)
	return string.format('0x%X',data)
end

function func_D615(address, data, type)
	return string.format('0x%X',data)
end

function func_D825(address, data, type)
	return string.format('%d',(bit.rshift(bit.band(data,0xFFF0),4)-115))
end

function func_D821(address, data, type)
	if (data==0) then
		return 'TPC_NONE'
	elseif (data==1) then
		return 'TPC_DOWN'
	elseif (data==3) then
		return 'TPC_UP'
	else
		return ''
	end
end

function func_D840(address, data, type)
	if (data==0) then
		return 'SS_DOWN'
	elseif (data==3) then
		return 'SS_UP'
	else
		return ''
	end
end

function func_D243(address, data, type)
	if (type=="TS") then
		return string.format('%d',bit.band(data,0x000F))
	elseif (type=="AGC") then
		return string.format('%d',bit.rshift(bit.band(data,0xFF00),8))
	else
		return ''
	end
end

function func_D61A(address, data, type)
	return string.format('0x%X',data)
end

function func_D82E(address, data, type)
	return string.format('%d',(bit.rshift(bit.band(data,0xFFF0),4)-115))
end

function func_D245(address, data, type)
	return string.format('%d',(0-bit.rshift(bit.band(data,0xFF00),8)))
end

function func_D82A(address, data, type)
	return string.format('0x%X',data)
end

function func_D28C(address, data, type)
	return string.format('%d',(data/147))
end

function func_EB21(address, data, type)
	if (data==1 or data==2) then
		return string.format('SC88502A')
	elseif (data==3 or data==4) then
		return string.format('SC88503B')
	else
		return ''
	end
end

function func_E0B1(address, data, type)
	if (data==0x2) then
		return string.format('BCH')
	elseif (data==0x4 or data==0x8) then
		return string.format('SCCH')
	elseif (data==0x10 or data==0x20) then
		return string.format('DPCH')
	elseif (data==0x40 or data==0x80) then
		return string.format('HSDPA')
	elseif (data==0x100 or data==0x200) then
		return string.format('PICH')
	elseif (data==0x400 or data==0x800) then
		return string.format('FPACH')
	elseif (data==0x50 or data==0x60 or data==0x90 or data==0xA0) then
		return string.format('HSDPA&DPCH')
	elseif (data==0x0A or data==0x06) then
		return string.format('FACH&BCH')
	elseif (data==0x404 or data==0x808 or data==0x408 or data==0x804) then
		return string.format('FACH&FPACH')
	else
		return ''
	end
end

function func_E0CC(address, data, type)
	return string.format('0x%X',data)
end

function func_E0B4(address, data, type)
	return string.format('0x%X',data)
end

function func_E0B0(address, data, type)
	return string.format('0x%X',data)
end

function func_E828(address, data, type)
	return string.format('%d',data/0x8000)
end

function func_EC22(address, data, type)
	return string.format('%d', bit.band(data,0x00FF))
end

function func_EC2F(address, data, type)
	if (type=="Lpi") then
		return string.format('%d',bit.rshift(bit.band(data,0xFF00),8))
	elseif (type=="q") then
		return string.format('%d',bit.band(data,0x00FF))
	else
		return ''
	end
end

function func_EC2E(address, data, type)
	return string.format('0x%X,0x%X',bit.band(data,0x00FF), bit.rshift(bit.band(data,0xFF00),8))
end

function func_EC24(address, data, type)
	if (type=="SICH TS Info") then
		return string.format('%d',bit.band(data,0x000F))
	elseif (type=="SICH Channel Info") then
		return string.format('%d',bit.rshift(bit.band(data,0x00F0),4))
	elseif (type == "SCCH TS Info") then
		return string.format('%d',bit.rshift(bit.band(data,0x0F00),8))
	elseif (type=="SCCH Channel Info") then
		return string.format('%d',bit.rshift(bit.band(data,0xF000),12))
	else
		return ''
	end
end

function func_E648(address, data, type)
	return string.format('%d',bit.rshift(bit.band(data,0xFF00),8))
end

function func_EAA9(address, data, type)
	return string.format('0x%X',data)
end

function func_E615(address, data, type)
	return string.format('0x%X',data)
end

function func_E825(address, data, type)
	return string.format('%d',(bit.rshift(bit.band(data,0xFFF0),4)-115))
end

function func_E821(address, data, type)
	if (data==0) then
		return 'TPC_NONE'
	elseif (data==1) then
		return 'TPC_DOWN'
	elseif (data==3) then
		return 'TPC_UP'
	else
		return ''
	end
end

function func_E840(address, data, type)
	if (data==0) then
		return 'SS_DOWN'
	elseif (data==3) then
		return 'SS_UP'
	else
		return ''
	end
end

function func_E243(address, data, type)
	if (type=="TS") then
		return string.format('%d',bit.band(data,0x000F))
	elseif (type=="AGC") then
		return string.format('%d',bit.rshift(bit.band(data,0xFF00),8))
	else
		return ''
	end
end

function func_E61A(address, data, type)
	return string.format('0x%X',data)
end

function func_E82E(address, data, type)
	return string.format('%d',(bit.rshift(bit.band(data,0xFFF0),4)-115))
end

function func_E245(address, data, type)
	return string.format('%d',(0-bit.rshift(bit.band(data,0xFF00),8)))
end

function func_E82A(address, data, type)
	return string.format('0x%X',data)
end

function func_E28C(address, data, type)
	return string.format('%d',(data/147)*100)
end

-- main interface function, load by C functions -------------------------------------
-- NOTE: !!!!!!!!!!!!!!!!!!!!DON'T CHANGE THIS FUNCTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
function dsp_keyinfo( address, data, type )
	func_pt=func_list[address]

	if not func_pt then
		return data
	else
		return func_list[address](address,data, type)
	end
end
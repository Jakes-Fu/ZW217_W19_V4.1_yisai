--[[--------------------------------------------------------------------
lua script to dynamic process dsp data and output processed data

NOTE: To add new log processing functions:
step 1: Add function reference in init_func_list
step 2: Add function body

2008/9/14: create, by xiaoguang.feng
--]]--------------------------------------------------------------------

-- for bit-wise operations
local bit = require"bit"

-- global function list table
func_list={}

-- initialize function list
function init_func_list()
	func_list[0xD883]=func_D883
	func_list[0xD811]=func_D811
	func_list[0xD888]=func_D888
	func_list[0xD894]=func_D894
	func_list[0xD971]=func_D971
	func_list[0xD972]=func_D972
	func_list[0xD973]=func_D973
	func_list[0xD974]=func_D974
	func_list[0xD975]=func_D975
	func_list[0xD976]=func_D976
	func_list[0xD977]=func_D977
	func_list[0xD978]=func_D978
	func_list[0xD979]=func_D979
end

-- data processing functions

function func_D883(address, data)
	local SNR_INT=bit.rshift(bit.band(data,0xFF00),8)
	if (SNR_INT>0x7F) then
		return (SNR_INT-256)
	else
		return SNR_INT
	end
end

function func_D811(address, data)
	local FN1=bit.rshift(bit.band(data,0xFE00),9)

	return FN1
end

function func_D888(address, data)
	if (bit.band(data,0xFF)==1) then -- nagative
		return -bit.rshift(bit.band(data,0xFF00),8)
	else -- positive
		return bit.rshift(bit.band(data,0xFF00),8)
	end
end

function func_D894(address, data)
	return (data-110)
end

function func_D971(address, data)
	return bit.band(data,0xFFF)
end

function func_D972(address, data)
	return bit.band(data,0xFFF)
end

function func_D973(address, data)
	return bit.band(data,0xFFF)
end

function func_D974(address, data)
	return bit.band(data,0xFFF)
end

function func_D975(address, data)
	return bit.band(data,0xFFF)
end

function func_D976(address, data)
	return bit.band(data,0xFFF)
end

function func_D977(address, data)
	return bit.band(data,0xFFF)
end

function func_D978(address, data)
	return bit.band(data,0xFFF)
end

function func_D979(address, data)
	return bit.band(data,0xFFF)
end

-- main interface function, load by C functions -------------------------------------
-- NOTE: !!!!!!!!!!!!!!!!!!!!DON'T CHANGE THIS FUNCTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
function dsp_data( address, data )
	func_pt=func_list[address]

	if not func_pt then
		return data
	else
		return func_list[address](address,data)
	end
end

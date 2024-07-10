'======================================
' Convert 
'	author:	xiongding@gmail.com	
' arg1: txtALLPath  - 生成结构体1，文件路径（输出文件）
' arg2: txtENUPath  - 生成结构体2，文件路径（输出文件）
' arg3: txtSTRPath  - 生成结构体3，文件路径（输出文件）
' arg4: txtEXTPath  - 生成结构体4，文件路径（输出文件）
' arg5: xlsFilePath - xls文件路径（输入文件）
' arg6: extFilePath - ext文件路径（输入文件）
' arg7: LogFile		  - Log文件路径（输出文件）
' arg8: TmpFile     - tmp临时文件路径 （输出文件）
'======================================

'Globe value
dim txtALLPath : txtALLPath = "..\..\..\..\..\MS_MMI_Main\source\mmi_app\app\phone\h\plmn_ts25_database_table.h"
dim txtENUPath : txtENUPath = "..\..\..\..\..\MS_MMI_Main\source\mmi_app\app\phone\h\plmn_ts25_database_enum.h"
dim txtSTRPath : txtSTRPath = "..\..\..\..\..\MS_MMI_Main\source\mmi_app\common\h\plmn_ts25_database_str.h"
dim txtEXTPath : txtEXTPath = ".\struct_ext.txt"

dim filename 
filename = inputbox("Please input plmn excel name, eg: Published TS25 Database 08 July 2019.xls", "File name")
filename = "\" & filename
dim xlsFilePath : xlsFilePath = filename
dim extFilePath : extFilePath = ".\ext.txt"
dim LogFile : LogFile = ".\123.txt"
dim TmpFile : TmpFile = ".\tmp.txt"

dim currentpath : currentpath = createobject("Scripting.FileSystemObject").getFolder(".").Path


'====================
Dim strMCC_bcakup : strMCC_bcakup = ""
Dim strMNC_bcakup : strMNC_bcakup = ""
Dim strTXT_bcakup : strTXT_bcakup = ""
Dim strline_backup : strline_backup = ""

'Function 提取出有效数据生成临时文件
Function CreatTmpFile(sPath)

Dim FSOTMP,myTmpFile
Set FSOTMP=CreateObject("scripting.filesystemobject")

if (FSOTMP.fileexists(TmpFile)) then  
	 FSOTMP.deleteFile TmpFile
end if
set myTmpFile=FSOTMP.createtextfile(TmpFile,8,ture)

Dim intRow : intRow 		= 2
Dim intLine : intLine 	= 1
Dim intMCC : intMCC 		= 8
Dim intMNC : intMNC 		= 9
Dim intPPCI : intPPCI 	= 5
Dim intABREV : intABREV = 6
Dim intBackFlag : intBackFlag = 1
Dim intTXTFlag : intTXTFlag = 1

Set objExcel = CreateObject("Excel.Application")
Set objWorkbook = objExcel.Workbooks.Open _
    (currentpath & xlsFilePath)

Do Until objExcel.Cells(intRow,1).Value = ""
	Dim strMCC : strMCC = objExcel.ActiveSheet.Cells(intRow,intMCC).Value
	Dim strMNC : strMNC = objExcel.ActiveSheet.Cells(intRow,intMNC).Value
	Dim strPPCI : strPPCI = objExcel.ActiveSheet.Cells(intRow,intPPCI).Value
	Dim strABREV : strABREV = objExcel.ActiveSheet.Cells(intRow,intABREV).Value
	Dim strPLMN : strPLMN = ""
	Dim strTXTNET : strTXTNET = ""
	
	If(strMCC_bcakup = strMCC) and (strMNC_bcakup = strMNC) then
		'Dim strBackFlag : strBackFlag = ""&intBackFlag
		'strPLMN = "PLMN_" & strMCC & "_" & strMNC & "_" & Cstr(intBackFlag)
		'intBackFlag = intBackFlag + 1
	else
		strPLMN = "PLMN_" & strMCC & "_" & strMNC
		intBackFlag = 1

		If Len(strPPCI) > 14 Then
			select case strABREV
				case "CMHK"
					strTXTNET = "qtn_operator_cmhk"
				case "APT"
					strTXTNET = "qtn_operator_apt"
				case "Chunghwa"
					strTXTNET = "qtn_operator_chunghwa"
				case else
					if strABREV = strTXT_bcakup Then
						 strTXTNET = "TXT_NET_" & Cstr(intTXTFlag)
					else
						 intTXTFlag = intTXTFlag + 1
						 strTXTNET = "TXT_NET_" & Cstr(intTXTFlag)
					end if
			end select
			myTmpFile.WriteLine(strMCC & "@*" & strMNC & "@*" & strABREV & "@*" & strTXTNET & "@*" & strPLMN)
			strTXT_bcakup = strABREV
		else
			select case strPPCI
				case "CHINA MOBILE"
					strTXTNET = "qtn_operator_china_mobile"
				case "CHN-UNICOM"
					strTXTNET = "qtn_operator_china_unicom"	
				case "CHN-CT"
					strTXTNET = "qtn_operator_chn_ct"	
				case "CSL"
					strTXTNET = "qtn_operator_csl"
				case "APT"
					strTXTNET = "qtn_operator_apt"
				case "Far EasTone"
					strTXTNET = "qtn_operator_far_eastone"
				case "KGT-Online"
					strTXTNET = "qtn_operator_kgt_online"
				case "T Star"
					strTXTNET = "qtn_operator_tstar"
				case "TWN MOBITAI"
					strTXTNET = "qtn_operator_twn_mobitai"
				case "TW Mobile"
					strTXTNET = "qtn_operator_tw_mobile"
				case "TWM TransAsi"
					strTXTNET = "qtn_operator_twm_transasi"
				case else
					if strPPCI = strTXT_bcakup Then
						 strTXTNET = "TXT_NET_" & Cstr(intTXTFlag)
					else
						 intTXTFlag = intTXTFlag + 1
						 strTXTNET = "TXT_NET_" & Cstr(intTXTFlag)
					end if
			end select
			myTmpFile.WriteLine(strMCC & "@*" & strMNC & "@*" & strPPCI & "@*" & strTXTNET & "@*" & strPLMN)
			strTXT_bcakup = strPPCI
		end if
	end if
	intRow = intRow + 1
	strMCC_bcakup = strMCC
	strMNC_bcakup = strMNC
Loop

'myLogFile.WriteLine(Cstr(Now) & " TmpFile complete!")
myTmpFile.close
objExcel.quit
End Function

'根据临时文件生成结构体1
Function CreatStructFile_ALL(sPath)
  on error resume Next
  
Dim FSOALL,myAllFile
Set FSOALL=CreateObject("scripting.filesystemobject")

if (FSOALL.fileexists(txtALLPath)) then  
	 FSOALL.deleteFile txtALLPath
end if
set myAllFile=FSOALL.createtextfile(txtALLPath,8,ture)
'myAllFile.WriteLine("{")

'myAllFile.WriteLine("001,01,TXT_NET_TEST,PLMN_TEST,")

set ftmpread=FSOALL.opentextfile(TmpFile, 1)

do until ftmpread.atendofstream
		strline = ftmpread.readline
		a=Split(strline,"@*")
		Dim strMCC : strMCC = a(0)
		Dim strMNC : strMNC = a(1)
		Dim strTXT_NET : strTXT_NET = a(3)
		Dim strPLMN : strPLMN = a(4)
		'Msgbox strline
		'Msgbox strMCC&strMNC&strTXT_NET&strPLMN

				If(strMNC = "") Then Exit Do End If

				if(strMNC = "029") Then strMNC = "29" End If
				if(strMNC = "039") Then strMNC = "39" End If
				if(strMNC = "038") Then strMNC = "38" End If
				If(strMNC = "08") Then strMNC = "8" End if
				If(strMNC = "09") Then strMNC = "9" End If
				If(strMNC = "080") Then strMNC = "80" End if
				If(strMNC = "090") Then strMNC = "90" End If


		myAllFile.WriteLine(strMCC & "," & strMNC & "," & strTXT_NET & "," & strPLMN & ",")
		
loop

ftmpread.close
'myAllFile.WriteLine("}")
myAllFile.close
End Function

'根据临时文件生成结构体2
Function CreatStructFile_ENU(sPath)
  on error resume Next
  
Dim FSOENU,myEnuFile
Set FSOENU=CreateObject("scripting.filesystemobject")

if (FSOENU.fileexists(txtENUPath)) then  
	 FSOENU.deleteFile txtENUPath
end if
set myEnuFile=FSOENU.createtextfile(txtENUPath,8,ture)
'myEnuFile.WriteLine("typedef enum")
'myEnuFile.WriteLine("{")


set ftmpread=FSOENU.opentextfile(TmpFile, 1)

do until ftmpread.atendofstream
		strline = ftmpread.readline
		a=Split(strline,"@*")
		Dim strPLMN : strPLMN = a(4)

		If(strPLMN = "") Then Exit Do End If

		myEnuFile.WriteLine(strPLMN & ",")
		
loop

ftmpread.close
'myEnuFile.WriteLine("}")
myEnuFile.close

End Function

'根据临时文件生成结构体3
Function CreatStructFile_STR(sPath)
  on error resume Next
  
Dim FSOSTR,myStrFile
Set FSOSTR=CreateObject("scripting.filesystemobject")

if (FSOSTR.fileexists(txtSTRPath)) then  
	 FSOSTR.deleteFile txtSTRPath
end if
set myStrFile=FSOSTR.createtextfile(txtSTRPath,8,ture)
set ftmpread=FSOSTR.opentextfile(TmpFile, 1)

Dim intBackFlag1 : intBackFlag1 = 0
Dim intBackFlag2 : intBackFlag2 = 0
Dim intBackFlag3 : intBackFlag3 = 0
Dim intBackFlag4 : intBackFlag4 = 0
Dim intBackFlag5 : intBackFlag5 = 0
Dim intBackFlag6 : intBackFlag6 = 0
Dim intBackFlag7 : intBackFlag7 = 0
Dim intBackFlag8 : intBackFlag8 = 0
Dim intBackFlag9 : intBackFlag9 = 0
Dim intBackFlag10 : intBackFlag10 = 0
Dim intBackFlag11 : intBackFlag11 = 0
Dim intBackFlag12 : intBackFlag12 = 0
Dim intBackFlag13 : intBackFlag13 = 0
do until ftmpread.atendofstream
    strline = ftmpread.readline
    a=Split(strline,"@*")

    Dim strTXT_NET : strTXT_NET = a(3)
    Dim strTXT : strTXT = a(2)

	Dim posat : posat = 0
	posat = instr(1, strTXT, "@")
	if (0 <> posat) Then
		b=Split(strTXT,"@") 
		strTXT = b(0) & "\@" & b(1)
	end if
	If(strTXT_NET = "") Then Exit Do End If

    if strline_backup <> strTXT_NET Then
		select case strTXT_NET
			case "qtn_operator_china_mobile"
				if (intBackFlag1 = 0) Then
					intBackFlag1 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")				
				end if
			case "qtn_operator_china_unicom"
				if (intBackFlag2 = 0) Then
					intBackFlag2 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")				
				end if	
			case "qtn_operator_chn_ct"
				if (intBackFlag3 = 0) Then
					intBackFlag3 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_cmhk"
				if (intBackFlag4 = 0) Then
					intBackFlag4 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_csl"
				if (intBackFlag5 = 0) Then
					intBackFlag5 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_apt"
				if (intBackFlag6 = 0) Then
					intBackFlag6 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_far_eastone"
				if (intBackFlag7 = 0) Then
					intBackFlag7 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_kgt_online"
				if (intBackFlag8 = 0) Then
					intBackFlag8 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_tstar"
				if (intBackFlag9 = 0) Then
					intBackFlag9 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_chunghwa"
				if (intBackFlag10 = 0) Then
					intBackFlag10 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_twn_mobitai"
				if (intBackFlag11 = 0) Then
					intBackFlag11 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_tw_mobile"
				if (intBackFlag12 = 0) Then
					intBackFlag12 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case "qtn_operator_twm_transasi"
				if (intBackFlag13 = 0) Then
					intBackFlag13 = 1		
					myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")	
				end if 
			case else
				myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")
		end select
    End if

    strline_backup = strTXT_NET
loop

ftmpread.close
myStrFile.close

End Function

'根据EXT文件生成结构体4
Function CreatStructFile_EXT(sPath)
	on error resume Next
Dim FSOEXT,myExtFile
Set FSOEXT=CreateObject("scripting.filesystemobject")

if (FSOEXT.fileexists(txtEXTPath)) then  
	 FSOEXT.deleteFile txtEXTPath
end if
set myExtFile=FSOEXT.createtextfile(txtEXTPath,8,ture)
set ftmpread=FSOEXT.opentextfile(TmpFile, 1)
set fextread=FSOEXT.opentextfile(extFilePath, 1)
'myExtFile.WriteLine("{")

do until ftmpread.atendofstream
		strline = ftmpread.readline
		a=Split(strline,"@*")
		Dim strMCC : strMCC = a(0)
		Dim strMNC : strMNC = a(1)
		Dim strTXT_NET : strTXT_NET = a(3)
		Dim strPLMN : strPLMN = a(4)

		If(strMNC = "") Then Exit Do End If
		
		do until fextread.atendofstream
			extstrline = fextread.readline
			b=Split(extstrline,"@*")
			Dim MCC : MCC = Trim(b(0))
			Dim MNC : MNC = Trim(b(1))
			
			If(MNC = "") Then Exit Do End If

			if(strMCC = MCC)and(strMNC = MNC)Then
				myExtFile.WriteLine(strMCC & "," & strMNC & "," & strTXT_NET & "," & strPLMN & ",")	
			end if
		loop		
loop
'myExtFile.WriteLine("}")

ftmpread.close
fextread.close
myExtFile.close

End Function

'main running...
on error resume Next

'1.初始化LOG文件
Dim FSOLOG,myLogFile
Set FSOLOG=CreateObject("scripting.filesystemobject")

if (FSOLOG.fileexists(LogFile)) then  
	 FSOLOG.deleteFile LogFile
end if
set myLogFile=FSOLOG.createtextfile(LogFile,8,ture)


'2.提取有效数据生成临时文件
myLogFile.WriteLine(Cstr(Now) & " Creat tmpFile...")
CreatTmpFile TmpFile
myLogFile.WriteLine(Cstr(Now) & " TmpFile complete!")
myLogFile.WriteLine(" ")



'3.根据临时文件，生成结构1
myLogFile.WriteLine(Cstr(Now) & " Creat StructFile_ALL...")
CreatStructFile_ALL TmpFile
myLogFile.WriteLine(Cstr(Now) & " StructFile_ALL complete!")
myLogFile.WriteLine(" ")

'4.根据临时文件，生成结构2
myLogFile.WriteLine(Cstr(Now) & " Creat StructFile_ENU...")
CreatStructFile_ENU TmpFile
myLogFile.WriteLine(Cstr(Now) & " StructFile_ENU complete!")
myLogFile.WriteLine(" ")

'5.根据临时文件，生成结构3
myLogFile.WriteLine(Cstr(Now) & " Creat StructFile_STR...")
CreatStructFile_STR TmpFile
myLogFile.WriteLine(Cstr(Now) & " StructFile_STR complete!")
myLogFile.WriteLine(" ")

'6.根据EXT文件，生成结构4
myLogFile.WriteLine(Cstr(Now) & " Creat StructFile_EXT...")
CreatStructFile_EXT TmpFile
myLogFile.WriteLine(Cstr(Now) & " StructFile_EXT complete!")
myLogFile.WriteLine(" ")

myLogFile.WriteLine(Cstr(Now) & " App complete!")
myLogFile.close

if (FSOLOG.fileexists(LogFile)) then  
	 FSOLOG.deleteFile LogFile
end if

FSOLOG.quit

if (FSOEXT.fileexists(extFilePath)) then  
	 FSOEXT.deleteFile extFilePath
end if

if (FSOEXT.fileexists(txtEXTPath)) then  
	 FSOEXT.deleteFile txtEXTPath
end if


if (FSOTMP.fileexists(TmpFile)) then  
	 FSOTMP.deleteFile TmpFile
end if

Msgbox "App complete!"


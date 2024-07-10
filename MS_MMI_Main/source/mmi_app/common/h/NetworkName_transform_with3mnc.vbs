'======================================
' Convert 
'	author:	xiongding@gmail.com	
' arg1: plmnTablePath  - 生成结构体1，文件路径（输出文件）
' arg2: txtEnumPath  - 生成结构体2，文件路径（输出文件）
' arg3: txtSTRPath  - 生成结构体3，文件路径（输出文件）
' arg4: txtEXTPath  - 生成结构体4，文件路径（输出文件）
' arg5: xlsFilePath - xls文件路径（输入文件）
' arg6: extFilePath - ext文件路径（输入文件）
' arg7: LogFile		  - Log文件路径（输出文件）
' arg8: TmpFile     - tmp临时文件路径 （输出文件）
'======================================

'======================================
'======================================
' HERE to define the input PLMN database
' HERE to define the input PLMN database

dim xlsFilePath : xlsFilePath = "Published TS25 Database 08 July 2019.xls"

'======================================
'======================================

'Globe value
Dim intGenerateLongNameFlag : intGenerateVariableNameLengthFlag = 1
Dim intGenerateFullStructFlag : intGenerateFullStructFlag = 0

dim plmnTablePath : plmnTablePath = ".\TS25_plmn_table.h"
dim plmnTable3MNCPath : plmnTable3MNCPath = ".\TS25_plmn_table_3digits.h"
dim txtEnumPath : txtEnumPath = ".\TS25_plmn_enum.h"
dim txtSTRPath : txtSTRPath = ".\TS25_plmn_str.h"
dim txtEXTPath : txtEXTPath = ".\struct_ext.txt"

dim extFilePath : extFilePath = ".\ext.txt"
dim LogFile : LogFile = ".\log.txt"
dim TmpFile : TmpFile = ".\tmp.txt"

'====================
Dim strMCC_bcakup : strMCC_bcakup = ""
Dim strMNC_bcakup : strMNC_bcakup = ""
Dim strTXT_bcakup : strTXT_bcakup = ""
Dim strline_backup : strline_backup = ""

Function GetFolder() 
    GetFolder = CreateObject("scripting.filesystemobject").GetFolder(".").Path
End Function 

'Function 提取出有效数据生成临时文件
Function CreatTmpFile(sPath)

    Dim FSOTMP,myTmpFile
    Set FSOTMP=CreateObject("scripting.filesystemobject")

    if (FSOTMP.fileexists(TmpFile)) Then  
	      FSOTMP.deleteFile TmpFile
    end if
    Set myTmpFile=FSOTMP.createtextfile(TmpFile,8,ture)

    Dim intRow  : intRow 		= 2
    Dim intLine : intLine 	= 1
    Dim intMCC  : intMCC 		= 8
    Dim intMNC  : intMNC 		= 9
    Dim intPPCI : intPPCI 	= 5
    Dim intABREV : intABREV = 6
    Dim intBackFlag : intBackFlag = 1
    Dim intTXTFlag  : intTXTFlag  = 1

    Set objExcel = CreateObject("Excel.Application")
    Set objWorkbook = objExcel.Workbooks.Open _
        (GetFolder & "\" & xlsFilePath)

    Do Until objExcel.Cells(intRow,1).Value = ""
        Dim strMCC    : strMCC    = objExcel.ActiveSheet.Cells(intRow,intMCC).Value
        Dim strMNC    : strMNC    = objExcel.ActiveSheet.Cells(intRow,intMNC).Value
        Dim strPPCI   : strPPCI   = objExcel.ActiveSheet.Cells(intRow,intPPCI).Value
        Dim strABREV  : strABREV  = objExcel.ActiveSheet.Cells(intRow,intABREV).Value
        Dim strPLMN   : strPLMN   = ""
        Dim strTXTNET : strTXTNET = ""
    
        If(strMCC_bcakup = strMCC) and (strMNC_bcakup = strMNC) Then
            'Dim strBackFlag : strBackFlag = ""&intBackFlag
            'strPLMN = "PLMN_" & strMCC & "_" & strMNC & "_" & Cstr(intBackFlag)
            'intBackFlag = intBackFlag + 1
        Else
            strPLMN = "PLMN_" & strMCC & "_" & strMNC
            intBackFlag = 1

            If Len(strPPCI) > 14 And (1 = intGenerateVariableNameLengthFlag) Then
                
                If strABREV = strTXT_bcakup Then
                     strTXTNET = "TXT_NET_" & Cstr(intTXTFlag)
                Else
                     intTXTFlag = intTXTFlag + 1
                     strTXTNET = "TXT_NET_" & Cstr(intTXTFlag)
                End If
            
                myTmpFile.WriteLine(strMCC & "@*" & strMNC & "@*" & strABREV & "@*" & strTXTNET & "@*" & strPLMN)
                strTXT_bcakup = strABREV
            Else
            
                If strPPCI = strTXT_bcakup Then
                     strTXTNET = "TXT_NET_" & Cstr(intTXTFlag)
                Else
                     intTXTFlag = intTXTFlag + 1
                     strTXTNET = "TXT_NET_" & Cstr(intTXTFlag)
                End If
            
                myTmpFile.WriteLine(strMCC & "@*" & strMNC & "@*" & strPPCI & "@*" & strTXTNET & "@*" & strPLMN)
                strTXT_bcakup = strPPCI
            End If
        End If
        
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

    ' HERE create normal case MNC PLMN table
    ' HERE create normal case MNC PLMN table
    ' HERE create normal case MNC PLMN table

    If (FSOALL.fileexists(plmnTablePath)) Then  
        FSOALL.deleteFile plmnTablePath
    End If

    Set myAllFile=FSOALL.createtextfile(plmnTablePath,8,ture)

    '==== nonused for included file
    If (1 = intGenerateFullStructFlag) Then
        myAllFile.WriteLine("{")
        myAllFile.WriteLine("001,01,TXT_NET_TEST,PLMN_TEST,")
    End If

    Set ftmpread = FSOALL.opentextfile(TmpFile, 1)

    Do until ftmpread.atendofstream
        strline = ftmpread.readline
        a = Split(strline,"@*")
        Dim strMCC     : strMCC = a(0)
        Dim strMNC     : strMNC = a(1)
        Dim strTXT_NET : strTXT_NET = a(3)
        Dim strPLMN    : strPLMN = a(4)
        'Msgbox strline
        'Msgbox strMCC&strMNC&strTXT_NET&strPLMN

        If(strMNC = "") Then 
        	  Exit Do 
        End If

        If(3=Len(strMNC)) And (1 = InStr(strMNC,"0")) Then
            ' mnc is 3 digits
        Else
        	  If(2 = Len(strMNC)) And (1 = InStr(strMNC,"0")) Then strMNC = right(strMNC,1) End If
            myAllFile.WriteLine(strMCC & "," & strMNC & "," & strTXT_NET & "," & strPLMN & ",")
        End If
        
    Loop

    ftmpread.close
    '==== nonused for included file
    If (1 = intGenerateFullStructFlag) Then
        myAllFile.WriteLine("}")
    End If
    myAllFile.close

    ' HERE create MNC 3digits PLMN table
    ' HERE create MNC 3digits PLMN table
    ' HERE create MNC 3digits PLMN table

    If (FSOALL.fileexists(plmnTable3MNCPath)) Then  
        FSOALL.deleteFile plmnTable3MNCPath
    End If

    Set myAllFile = FSOALL.createtextfile(plmnTable3MNCPath, 8, ture)
    '==== nonused for included file
    If (1 = intGenerateFullStructFlag) Then
        myAllFile.WriteLine("{")
    End If

    Set ftmpread = FSOALL.opentextfile(TmpFile, 1)

    do until ftmpread.atendofstream
        strline = ftmpread.readline
        a = Split(strline,"@*")
        strMCC = a(0)
        strMNC = a(1)
        strTXT_NET = a(3)
        strPLMN = a(4)

        If(strMNC = "") Then 
        	  Exit Do 
        End If

        If(3 = Len(strMNC)) And (1 = InStr(strMNC,"0")) Then
            ' mnc is 3 digits
            If(1 = InStr(strMNC,"00")) Then 
                strMNC = right(strMNC,1) 
            else
                strMNC = right(strMNC,2) 
            End If
            myAllFile.WriteLine(strMCC & "," & strMNC & "," & strTXT_NET & "," & strPLMN & ",")
        End If
        
    loop
    ftmpread.close

    '==== nonused for included file
    If (1 = intGenerateFullStructFlag) Then
        myAllFile.WriteLine("}")
    End If
    myAllFile.close

End Function

'根据临时文件生成结构体2
Function CreatStructFile_ENU(sPath)
    on error resume Next
  
    Dim FSOENU,myEnuFile
    Set FSOENU = CreateObject("scripting.filesystemobject")

    If (FSOENU.fileexists(txtEnumPath)) Then  
        FSOENU.deleteFile txtEnumPath
    End If
    Set myEnuFile = FSOENU.createtextfile(txtEnumPath,8,ture)
    '==== nonused for included file
    If (1 = intGenerateFullStructFlag) Then
        myEnuFile.WriteLine("typedef enum")
        myEnuFile.WriteLine("{")
    End If


    Set ftmpread=FSOENU.opentextfile(TmpFile, 1)

    do until ftmpread.atendofstream
        strline = ftmpread.readline
        a=Split(strline,"@*")

        Dim strPLMN : strPLMN = a(4)

        If(strPLMN = "") Then
            Exit Do
        End If

        myEnuFile.WriteLine(strPLMN & ",")
        
    loop

    ftmpread.close
    '==== nonused for included file
    If (1 = intGenerateFullStructFlag) Then
        myEnuFile.WriteLine("}")
    End If
    myEnuFile.close

End Function

'根据临时文件生成结构体3
Function CreatStructFile_STR(sPath)
    on error resume Next
  
    Dim FSOSTR,myStrFile
    Set FSOSTR = CreateObject("scripting.filesystemobject")

    If (FSOSTR.fileexists(txtSTRPath)) Then  
        FSOSTR.deleteFile txtSTRPath
    End If
    Set myStrFile = FSOSTR.createtextfile(txtSTRPath,8,ture)
    Set ftmpread = FSOSTR.opentextfile(TmpFile, 1)

    do until ftmpread.atendofstream
        strline = ftmpread.readline
        a = Split(strline,"@*")

        Dim strTXT_NET : strTXT_NET = a(3)
        Dim strTXT : strTXT = a(2)

        If(strTXT_NET = "") Then
            Exit Do
        End If

        If strline_backup <> strTXT_NET Then
            myStrFile.WriteLine("RES_ADD_STRING(" & strTXT_NET & "," & chr(34) & strTXT & chr(34) & ")")
        End If

        strline_backup = strTXT_NET
    loop

    ftmpread.close
    myStrFile.close

End Function

'根据EXT文件生成结构体4
Function CreatStructFile_EXT(sPath)
    on error resume Next
    Dim FSOEXT,myExtFile
    Set FSOEXT = CreateObject("scripting.filesystemobject")

    If (FSOEXT.fileexists(txtEXTPath)) Then  
        FSOEXT.deleteFile txtEXTPath
    End If
    Set myExtFile = FSOEXT.createtextfile(txtEXTPath,8,ture)
    Set ftmpread  = FSOEXT.opentextfile(TmpFile, 1)
    Set fextread  = FSOEXT.opentextfile(extFilePath, 1)
    '==== nonused for included file
    If (1 = intGenerateFullStructFlag) Then
        myExtFile.WriteLine("{")
    End If

    do until ftmpread.atendofstream
        strline = ftmpread.readline
        a = Split(strline,"@*")
        Dim strMCC : strMCC = a(0)
        Dim strMNC : strMNC = a(1)
        Dim strTXT_NET : strTXT_NET = a(3)
        Dim strPLMN : strPLMN = a(4)
        
        If(strMNC = "") Then
            Exit Do
        End If

        do until fextread.atendofstream
            extstrline = fextread.readline
            b = Split(extstrline,"@*")
            Dim MCC : MCC = Trim(b(0))
            Dim MNC : MNC = Trim(b(1))
            
            If(MNC = "") Then Exit Do End If

            If (strMCC = MCC) and (strMNC = MNC) Then
                myExtFile.WriteLine(strMCC & "," & strMNC & "," & strTXT_NET & "," & strPLMN & ",")    
            End If
        loop        
    loop
    '==== nonused for included file
    If (1 = intGenerateFullStructFlag) Then
        myExtFile.WriteLine("}")
    End If

    ftmpread.close
    fextread.close
    myExtFile.close

End Function

'MAIN PROCESS Running...
on error resume Next

'1.初始化LOG文件
Dim FSOLOG,myLogFile
Set FSOLOG = CreateObject("scripting.filesystemobject")
If (FSOLOG.fileexists(LogFile)) Then  
     FSOLOG.deleteFile LogFile
End If
Set myLogFile = FSOLOG.createtextfile(LogFile,8,ture)


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
FSOLOG.quit

' 7.Clean all templory files and log file. If need log for analysis, just mark the following functions
Dim FSO
Set FSO = CreateObject("scripting.filesystemobject")

If (FSO.fileexists(txtEXTPath)) Then  
    FSO.deleteFile txtEXTPath
End If

If (FSO.fileexists(extFilePath)) Then  
    FSO.deleteFile extFilePath
End If

If (FSO.fileexists(LogFile)) Then  
    FSO.deleteFile LogFile
End If

If (FSO.fileexists(TmpFile)) Then  
    FSO.deleteFile TmpFile
End If

Msgbox "App complete!"


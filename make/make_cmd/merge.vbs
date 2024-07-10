'------------------------------------------------------------------------------
' File Name:      merge.vbs                                              
' Author:         hongliang.xin                                                         
' Date:           18/12/2009                                                
' Copyright:      2009 Spreatrum Communications INC. All Rights Reserved.         
' Description:    merge two string tables (*.xls).
'                 usage: merge.vbs str1.xls str2.xls
'                 str2.xls will merge to str1.xls. if the IDs in str2.xls is 
'                 the same as the IDs in str1.xls, it will occur error,and list
'                 all repeated IDs.
'------------------------------------------------------------------------------

Set ScriptArgs = WScript.Arguments
If( ScriptArgs.Count < 2 ) Then
	Call Help
	WScript.Quit(1)
End If

Set SprdExAp = CreateObject("Excel.Application")
SprdExAp.Visible = FALSE
SprdExAp.DisplayAlerts = FALSE
Set SprdExBk = SprdExAp.Workbooks.Open(ScriptArgs(0))
Set SprdExSt = SprdExBk.Worksheets(1)  'Sheet1 or sheet's name

Set CustExAp = CreateObject("Excel.Application")
CustExAp.Visible = FALSE
CustExAp.DisplayAlerts = FALSE
Set CustExBk = CustExAp.Workbooks.Open(ScriptArgs(1))
Set CustExSt = CustExBk.Worksheets(1)  'Sheet1 or sheet's name

sprd_row_count = SprdExSt.UsedRange.Rows.count
sprd_col_count = SprdExSt.UsedRange.Columns.count

For i = 1 to sprd_col_count
	If Len(SprdExSt.Cells(1,i))=0 Then
		sprd_col_count = i-1
		Exit For
	End If
Next

cust_row_count = CustExSt.UsedRange.Rows.count
cust_col_count = CustExSt.UsedRange.Columns.count

For i = 1 to cust_col_count
	If Len(CustExSt.Cells(1,i))=0 Then
		cust_col_count = i-1
		Exit For
	End If
Next

If cust_row_count < 2 Then
	Call Final
	WScript.echo "complete"
	Wscript.Quit(0)

End If

'------------------------------------------
'check header
'------------------------------------------
If cust_col_count <> sprd_col_count Then
	Call Final
	WScript.echo "header colomns of two tables are not same"
	WScript.Quit(1)
End If

For i=1 to sprd_col_count
	If( SprdExSt.Cells(1,i) <> CustExSt.Cells(1,i)) Then
		Call Final
		WScript.echo "header colomns of two tables are not same"
		WScript.Quit(1)
	End If
Next
'------------------------------------------
' end check header
'------------------------------------------

' Create index for source excel file
Set d_index = CreateObject("Scripting.Dictionary")

For j = 2 To sprd_row_count          ' Ignore the first row
	Set cell = SprdExSt.Cells(j, 1)
	cell_text = cell.Text
	If (Len(cell_text) > 0) Then
    		d_index.Item(cell_text) = j
    		'Script.echo cell_text	
	End If
Next

flag = 0

For i=2 to cust_row_count
	cell_text = CustExSt.Cells(i,1).Text
	If Len(cell_text)>0 Then
		'Script.echo cell_text		
		If (d_index.exists(cell_text)) Then
			WScript.echo cell_text
			flag = 1
		Else
			For j=1 to cust_col_count					
				SprdExSt.Cells(sprd_row_count+i-1,j)=  CustExSt.Cells(i,j)	
			Next
		End If
	End If 
Next



Call Final

If Flag = 0 Then
	SprdExBk.Save
	WScript.echo "complete"
	WScript.Quit(0)
Else
	WScript.echo "above ID is repreated."
	WScript.Quit(1)
End If


Sub Final
	SprdExBk.close
	CustExBk.close
	
	SprdExAp.Quit
	CustExAp.Quit	
	'Release SprdExAp
	'Release CustExAp

End Sub

Sub Help
	WScript.echo "Usage: " & WScript.ScriptName & " sprd.xls cust.xls" & VbCrLf
End Sub


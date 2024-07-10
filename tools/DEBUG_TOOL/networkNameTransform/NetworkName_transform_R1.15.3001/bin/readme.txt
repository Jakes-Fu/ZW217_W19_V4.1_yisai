1.右击“NetworkName_transform.vbs”用UltraEdit打开
2.设置参数：
	' arg1: txtALLPath  - 生成结构体1，文件路径（输出文件）
	' arg2: txtENUPath  - 生成结构体2，文件路径（输出文件）
	' arg3: txtSTRPath  - 生成结构体3，文件路径（输出文件）
	' arg4: txtEXTPath  - 生成结构体4，文件路径（输出文件）
	' arg5: xlsFilePath - xls文件路径（输入文件）
	' arg6: extFilePath - ext文件路径（输入文件）
	' arg7: LogFile		  - Log文件路径（输出文件）
	' arg8: TmpFile     - tmp临时文件路径 （输出文件）
3.设置参数完成，关闭“NetworkName_transform.vbs”文件
4.右击“NetworkName_transform.vbs”
	选择“打开方式”---->“Microsoft@Windows Based Script Host”
5.“NetworkName_transform.vbs”开始后台自动运行
6.打开LogFile文件监控程序执行情况
7.程序运行完成会弹出“app complete！”对话框。
	脚本程序运行很慢，请使用者耐心等待。。。




附件文件介绍：
1.Published TS.25 Database 29 June 2015.xls：
	MS提供的输入文件，即arg5

2.ext.txt
	根据该输入文件判断特殊MCC\MNC，即arg6（注意该文件的第一行是我为测试，自行添加的MCC\MNC）
	
3.Log.txt
	LOG文件，即arg7

4.tmp.txt
	临时数据文件，即arg8
	
5.struct_all.txt
	所有MCC\MNC数据在该文件列举出来，即arg1
	
6.struct_enum.txt
	所有PLMN枚举值该文件列举出来，即arg2
	
7.struct_str.txt
	宏定义TXT_NET对于的String在该文件列举出来，即arg3
	
8.struct_ext.txt
	特殊的MCC\MNC数据在该文件列举出来，即arg4
1.代码位置：\MS_Code\MS_MMI\source\mmi_app\common\h中，有一个脚本NetworkName_transform_with3mnc.vbs

2.拿到客户的*.xls文件后，对比一下列是否有变化，若有变化需要修改
Dim intMCC  : intMCC = 8
Dim intMNC  : intMNC = 9
Dim intPPCI : intPPCI = 5
Dim intABREV : intABREV = 6
对应的列号
3.在excel表格中，依次按长名排序，按短名排序，按mnc排序，按mcc排序，保证修改的text名较少
4.运行脚本，NetworkName_transform_with3mnc.vbs
5.查看生成的TS25_plmn_enum.h，TS25_plmn_str.h，TS25_plmn_table.h和TS25_plmn_table_3digits.h文件，查看diff，是否有架构性变化，若无，说明成功
6.编译成功后，能够烧入手机

成功后需要手动修改以下运营商名称
1.639，07对应的运营商名称“GSM Telkom”
2.452，02对应的运营商名称“Vinaphone” //bug1066838
3.添加404，11对应的运营商名称//bug1106012
4.添加404，20对应的运营商名称//bug1106012
5.添加404，86对应的运营商名称//bug1106012，1126097
1.PCLINT安装

copy 即可，必要文件及目录结构如下：

	C:\pclint\LINT-NT.EXE
	C:\pclint\aloa.exe
	C:\pclint\makelint\所有文件

2.设置

	(1)ADS设置
		
	确认makelint\ADSconf.lnt中ADS头文件路径,默认为如下路径（其中有teak 路径）

		-i"C:\Program Files\ARM\ADSv1_2\Include"
		-i"C:\Program Files\ARM\ADSv1_2\Include\rw"
		-i"C:\teak\include"
		-i"C:\teak\include\sys"
		-i"C:\teak\include\machine"

(2)宏定义文件

	makelint\MACROdef.lnt
	这个文件中的宏会自动参与所有工程的pclint扫描，默认为 ：-D_RTOS -D_DEBUG -Ec ，一般不需要修改

(3)options.lnt 选择

	根据公司规则库定义选择相应的options.lnt文件 copy 到 C:\pclint\makelint\ 下

3.运行

	cygwin 下进入代码的 MS_Code 文件夹下
	执行如下类似命令之一即可： 
	make PROJECT=rocky_3201 pclint
	make PROJECT=rocky_3201 MODULES="app gui" pclint

4.结果查看

	/build/rocky_3201_builddir/pclint
	结果汇总文件：result.txt
	细节文件aloa.xml





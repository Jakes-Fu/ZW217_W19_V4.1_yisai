1.PCLINT��װ

copy ���ɣ���Ҫ�ļ���Ŀ¼�ṹ���£�

	C:\pclint\LINT-NT.EXE
	C:\pclint\aloa.exe
	C:\pclint\makelint\�����ļ�

2.����

	(1)ADS����
		
	ȷ��makelint\ADSconf.lnt��ADSͷ�ļ�·��,Ĭ��Ϊ����·����������teak ·����

		-i"C:\Program Files\ARM\ADSv1_2\Include"
		-i"C:\Program Files\ARM\ADSv1_2\Include\rw"
		-i"C:\teak\include"
		-i"C:\teak\include\sys"
		-i"C:\teak\include\machine"

(2)�궨���ļ�

	makelint\MACROdef.lnt
	����ļ��еĺ���Զ��������й��̵�pclintɨ�裬Ĭ��Ϊ ��-D_RTOS -D_DEBUG -Ec ��һ�㲻��Ҫ�޸�

(3)options.lnt ѡ��

	���ݹ�˾����ⶨ��ѡ����Ӧ��options.lnt�ļ� copy �� C:\pclint\makelint\ ��

3.����

	cygwin �½������� MS_Code �ļ�����
	ִ��������������֮һ���ɣ� 
	make PROJECT=rocky_3201 pclint
	make PROJECT=rocky_3201 MODULES="app gui" pclint

4.����鿴

	/build/rocky_3201_builddir/pclint
	��������ļ���result.txt
	ϸ���ļ�aloa.xml





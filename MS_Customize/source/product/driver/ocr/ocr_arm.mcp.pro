#Version 0.0.1


#TARGET��Ӧ��ԭ��ģ��MCP����������Ŀ�꣬����һ��*.a�ļ�
ocr_arm.a

TARGET = #�������ģ��ı���ѡ����������������θ�����ÿ��ѡ����ÿո�ֿ�
MODULE_OPTIONS =




#�оٳ���Դ�ļ�һ������Ҫ����ʵ�ʱ���ģ�������Ҫ�г�
SOURCES = ..\Src\OCR_driver.c    \ 
          ..\Src\thocr_lib.c    \ 
          ..\..\..\..\..\ThirdParty\ocr\lib\TH_Embeded_OCR.a


#��������������⣬���Խ�ȫ�����������棬 �����֮���ÿո�ֿ�
DEPENDENCE = ..\..\..\..\..\ThirdParty\ocr\lib\TH_Embeded_OCR.a
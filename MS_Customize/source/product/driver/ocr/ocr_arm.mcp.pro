#Version 0.0.1


#TARGET对应于原来模块MCP的最终生成目标，比如一个*.a文件
ocr_arm.a

TARGET = #如果存在模块的编译选择项，可以在下面依次给出，每个选项间用空格分开
MODULE_OPTIONS =




#列举出的源文件一定是需要参与实际编译的，否则不需要列出
SOURCES = ..\Src\OCR_driver.c    \ 
          ..\Src\thocr_lib.c    \ 
          ..\..\..\..\..\ThirdParty\ocr\lib\TH_Embeded_OCR.a


#如果依赖第三方库，可以将全名称列在下面， 多个库之间用空格分开
DEPENDENCE = ..\..\..\..\..\ThirdParty\ocr\lib\TH_Embeded_OCR.a
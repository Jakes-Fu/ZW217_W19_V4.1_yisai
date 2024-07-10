[1.3.9.30.23]
1. 修正IMG_CMP_USER的bug，ID写成了Path。
2. 修改Bmp压缩算法，解决色偏的问题
3. 拦截RAW压缩

[1.3.9.30.22]
1. 支持IMG_CMP_PNG_ARGB_RLE_AQ,
 IMG_CMP_BMP_32_ARGB_RLE_AQ,
 IMG_CMP_PNG_SABM_RLE,
 IMG_CMP_BMP_SABM_RLE,
 IMG_CMP_BMP_32_ARGB888_RLE,
 IMG_CMP_PNG_ARGB888_RLE,
 IMG_CMP_USER
2. 默认字符串unicode存储，跨语言去重,设置如下，同时支持命令行设置。
AllUseUnicode = 1
RemoveCrossRepeatString = 0
-alu [true|false] -crm [true|false]
3. IMG_CMP_USER压缩类型，压缩参数写入资源bin，不是Quality含义，有用户自己定义。
4. 支持Str_table按语言拆分成多个表
   规则：
   a.命名规则，str_table.xls, str_table1.xls, str_table2.xls ....
   b.工具组合规则，先从str_table.xls中抽出需要的语言字符串，然后从str_table1.xls中抽出之前没有的语言，再依次处理其他表。
   c.如果客户有自己的cust_str_table.xls, 可以不拆分，如果拆分命名规则为cust_str_table.xls，cust_str_table1.xls，cust_str_table2.xls...
   d.有客户表的情况下，先按b规则处理自己的表格，然后以同样b的规则处理客户的表格，ID相同语言相同的内容不空的则替换已有内容。
   e.表格中的default，和其他内部sheet页，则先读取default，其他sheet页的内容以resview.ini配置的先后顺序，依次读取，ID相同语言相同的内容不空的则替换已有内容。

   

[1.3.9.30.21]
1. 支持IMG_CMP_PNG_ARGB_RLE 和 IMG_CMP_BMP_32_ARGB_RLE格式。
2. 支持COMMON目录
3. 支持CSC
4. 支持导出使用的String到Excel文件
5. 每条String：History是与Default组合，即History非空项替换Default中的字符串。
6. 修改Tag输出标签repl为cscs。Cscs为变长存储，节省空间，同时导出lang标签，表示语言名称。
7. 支持图片解压缩为PNG格式。

[1.3.9.30.20]
1. 支持ANIM_CMP_BMP_ARGB888, ANIM_CMP_PNG_ARGB888
2. 支持配置，如果因为Color数超过64K用ARGB888压缩失败，则用ARGB压缩
   修改Resview.ini, 默认不自动转换，如需打开将ARGB888TOARGB 置1
   [Suppers]
   ARGB888TOARGB = 0
3. 如果PNG或者BMP32颜色数超过64K，用ARGB888压缩，工具会拦截并报Error
   如果ARGB888TOARGB = 1，则报Warning。
4. 解决动画大端没有转换的问题。
5. 解决WPS读表格失败，导致ResView死掉的问题。
6. 解决BMP8的编解码问题
7. 更新guangyao的BMP编码代码

[1.3.9.30.19]
1.JPG 去掉对RAW格式的支持
2.Str_table.xls支持更多可配置的Sheet
3.客户版将Warning变成Error。
4.SABM用一个byte表示是否为不透明。全不透明为1.

[1.3.9.30.18]
1.客户版字符串不存在用默认替换不报错。
2.BMP32编码限制必须有alpha
3.JPG压缩支持Dither
4.Color/Rect支持客户xls表，自动合并
5.修改BMP8编解码
6.No Alpha支持连续报错

[1.3.9.30.17]
1.支持不复制资源目录
  -resdir res_path

[1.3.9.30.16]
1.支持可配置,去掉字符串头标记，减少资源size。
2.支持命令行参数  -langsplit [true|false]
	true：将字符串按语言分离为独立的资源bin。
	False：保持不变。
3.支持压缩格式IMG_CMP_BMP_32_ARGB888 和 IMG_CMP_PNG_ARGB888
4.支持可配置，一次读入还是分批读入Excel Sheet，解决速度慢或者在WPS环境下出错的问题。

[1.3.9.30.15]
1.支持命令行参数  -rep [true|false]
    true：用默认资源替换，给出全部警告，正常生成资源。
    false：用默认资源替换，给出全部警告，不生成资源。

[1.3.9.30.14]
1. str_table支持History表单
2. 解决GIF编码受Size约束的bug。

[1.3.9.30.13]
1.解决图片不存在出错，不能报出所有不存在的图片的问题
2.解决Excel表格数据超过8000条后，无法读取的问题。
3.增加配置可以设置读Excel的方法，用MS Excel组件还是用WPS ET组件。
4.解决Excel表格读取空表单出错的问题
5.输出TEXT统计信息
6.支持IMG_CMP_BMP_CK格式


[1.3.9.30.12]
1.支持IMG_CMP_RAW
2.可以通过配置，设置客户字符串表和展迅字符串表以哪个为准：
   在ResView.ini中
   [ResTableSetting]
   StringFollowCust = 0
   默认StringFollowCust = 0，当两个表中ID重复，则报错。
   StringFollowCust = 0，当两个表中ID重复，已客户表中字符串为准，同时给出警告。
 

[1.3.9.30.11]
1.支持BMP(16/24/32)文件转换SABM格式图片
   压缩类型：IMG_CMP_BMP_SABM
   支持的FLAG：
   SABM_TILE 
   SABM_HOR_STRETCH
   SABM_VER_STRETCH  
   SABM_CEN_STRETCH  
2.支持RES_ADD_LOGO
    格式：RES_ADD_LOGO(IMAGE_ID, IMAGE_PATH, IMAGE_COMPRESS_TYPE, IMAGE_QULITY_TYPE) 
3.支持RES_ADD_XXX_TAG
   格式：RES_ADD_XXX_TAG( XXX_ID, "Description")
   说明：Description只能是ASCII码字串，建议就用XXX_ID。
   目前支持：
   RES_ADD_IMG_TAG
   RES_ADD_ANIM_TAG
   RES_ADD_RING_TAG
   RES_ADD_COLOR_TAG

[1.3.9.30.10]
1.支持PNG文件转换SABM格式图片
   压缩类型：IMG_CMP_PNG_SABM
   支持的FLAG： 
   SABM_TILE 
   SABM_HOR_STRETCH
   SABM_VER_STRETCH  
   SABM_CEN_STRETCH  

[1.3.9.30.10]
1.支持PNG文件转换成动画（SMOV ARGB）

[1.3.9.30.9]
1.修正Jpeg编码时分配的buffer太小，导致数组访问越界，程序崩溃。
2.限制Ring的size最大为500K(512000)。
   MMI可以修改\MS_Code\MS_MMI\source\resource\mmi_res_prj_def.h
   在其中添加RES_SET_MAX_RING_SIZE(512000)
   设置不同的size限制，同是需要在MS_Code\MS_MMI\source\mmi_app\common\h\macro_res_prj_def.h中添加
   #undef RES_SET_MAX_RING_SIZE
   #define RES_SET_MAX_RING_SIZE(size) 

[1.3.9.30.8]
1.修正不能使用WPS ET的bug。
2.修正GIF解码的bug。

[1.3.9.30.7]
1.修改字符串替换策略：如果表中有英语字符串，则用此字符串区替换其他语言为空的字符串。
2.脚本检测资源bin大小是否正确，如果资源bin不对，会删除它们。
3.脚本支持默认图片压缩格式。
  如果MMI需要支持默认压缩，需要修改如下：
	a. 修改\MS_Code\MS_MMI\source\resource\mmi_res_prj_def.h
	    在“//RES_SET_GLOBAL_JPEG_COMPRESS_RATE(75)”之下，添加以下内容：
		RES_SET_IMG_CMP_DEFAULT(BMP_8BIT,IMG_CMP_BMP_8_SCI,0)
		RES_SET_IMG_CMP_DEFAULT(BMP_16BIT,IMG_CMP_BMP_16_565_RLE,0)
		RES_SET_IMG_CMP_DEFAULT(BMP_24BIT,IMG_CMP_BMP_16_565_RLE,0)
		RES_SET_IMG_CMP_DEFAULT(BMP_32BIT,IMG_CMP_BMP_32_SCI,0)
		RES_SET_IMG_CMP_DEFAULT(JPG,IMG_CMP_JPG_SCI,75)
		RES_SET_IMG_CMP_DEFAULT(PNG,IMG_CMP_PNG_ARGB,0)
		RES_SET_IMG_CMP_DEFAULT(GIF,IMG_CMP_GIF_SCI,0)

		RES_SET_IMG_CMP_DEFAULT用法：
		RES_SET_IMG_CMP_DEFAULT(图片类型,图片压缩类型,压缩参数)
		除了JPG，其他图片类型的“压缩参数”都是“0”，没有意义。
		对于JPG，“压缩参数”为压缩质量，最高值为100。

	b. 修改模块定义资源头文件*_mdu_def.h
	    修改需要设置默认压缩，图片添加函数RES_ADD_IMG
	    将路径中的后缀名去掉，压缩类型设置成“IMG_CMP_DEFAULT”即可。

	c. 修改\MS_Code\MS_MMI\source\mmi_app\common\h\macro_res_prj_def.h
	   在“#define RES_SET_GLOBAL_JPEG_COMPRESS_RATE(percent)  //none”下增加一行
		#define RES_SET_IMG_CMP_DEFAULT(img_type,cmp_type,cmp_param)  //none

	说明：
	脚本按照BMP,JPG,PNG,GIF的顺序搜索图片，且只搜索四中图片。
	如果设置了“IMG_CMP_DEFAULT”压缩类型，但图片路径上又包含了后缀名，则脚本只搜索该类型图片。
5. [IMG库]修正PNG转换ARGB的bug
6. [IMG库]将ANIM和GIF的结构初始化为零
7. [IMG库]修正PNG转换ARGB的判断是否有Alpha的判断。
8. 修正创建ET.Application的成功的判断。
9. 去掉ResCmd中\-/-字符log。

[1.3.9.30.6]
1.资源用不存在用默认替换，同时给出warning，显示成功
2.去除IMG库打出的trace。

[1.3.9.30.5]
1.支持客户字符串表格于资源自带字符串表格合并,需要在ResView.ini中配置客户字符串表格的文件名。
  举例：
  [CustStrTable]
  1=cust_str_table.xls
2.报表显示ID的enum值

[1.3.9.30.4]
1.资源不存在直接报错，不用默认资源替换。

[1.3.9.30.3]
1.修正文本资源错位的问题（语言错位）
2.修正提取ARGB的压缩数据的高度错误，导致汇总信息不准确
3.将Anim的临时文件夹，放到系统的临时目录下。

[1.3.9.30.2]
1.修正内存覆盖导致程序Assert的问题

[1.3.9.30.1]
1.增加Color，Rect资源，Ring,Font作为主题。


[1.3.9.20.4]
1.去掉写临时文件到根目录的代码，解决根目录没有权限写文件导致工具崩溃的问题。

[1.3.9.20.2]
1.修改bmp_encoder.c
2.增加Ring类型的自适应（按后缀名确定Ring的类型）
3.修正JPEG的默认YUV采样率。

[1.3.9.20.1]
1.集成ResView功能
2.修改alpha_res_encoder.c

[1.3.9.16.3]
1.解决同时编译多个工程时，会失败的问题。

[1.3.9.16.2]
1.解决用wps表格打开str_table.xls失败的问题。
2.如果用wps表格打开str_table.xls，建议将wps表格的宏的安全级别设为“低”，
  否则wps表格会弹出宏的警告，只有点击了这个警告对话框上的按钮，才能继续编译资源。

[1.3.9.16.1]
1. 去掉一些图片压缩格式
2. 速度优化

[1.3.9.16.0]
1. 修正GIF Codec
    由宽高限制（240×320）增加到（1024×1024），如果超过1024×1024，编码仍然会有问题。
2. 支持BMP 8
    输入必须为8位BMP
    IMG_CMP_BMP_8_RAW
    IMG_CMP_BMP_8_SCI   

[1.3.9.15.9]
1. 支持PNG
    IMG_CMP_PNG_RAW
    IMG_CMP_PNG_ARGB


[1.3.9.15.8]
1. add another image compression type:
   IMG_CMP_JPG_STD
   recompress JPEG to standard format
2. modify the jpeg-sci algorithm
   only support MCU 422
3. remove trace content printed by jpeg codec 


[1.3.9.15.7]
1. add data resource to the theme
2. fix the bug: add JPG image failed and exit with no prompt

	
[1.3.9.15.6]
1. update alpha_res_encoder.c 

2. image compressions support Dithering
   IMG_CMP_BMP_16_565_RLE
   IMG_CMP_BMP_16_565_RAW
   
   * notice: IMG_CMP_BMP_OSD can not use dithering
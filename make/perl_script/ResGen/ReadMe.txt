[1.3.9.30.23]
1. ����IMG_CMP_USER��bug��IDд����Path��
2. �޸�Bmpѹ���㷨�����ɫƫ������
3. ����RAWѹ��

[1.3.9.30.22]
1. ֧��IMG_CMP_PNG_ARGB_RLE_AQ,
 IMG_CMP_BMP_32_ARGB_RLE_AQ,
 IMG_CMP_PNG_SABM_RLE,
 IMG_CMP_BMP_SABM_RLE,
 IMG_CMP_BMP_32_ARGB888_RLE,
 IMG_CMP_PNG_ARGB888_RLE,
 IMG_CMP_USER
2. Ĭ���ַ���unicode�洢��������ȥ��,�������£�ͬʱ֧�����������á�
AllUseUnicode = 1
RemoveCrossRepeatString = 0
-alu [true|false] -crm [true|false]
3. IMG_CMP_USERѹ�����ͣ�ѹ������д����Դbin������Quality���壬���û��Լ����塣
4. ֧��Str_table�����Բ�ֳɶ����
   ����
   a.��������str_table.xls, str_table1.xls, str_table2.xls ....
   b.������Ϲ����ȴ�str_table.xls�г����Ҫ�������ַ�����Ȼ���str_table1.xls�г��֮ǰû�е����ԣ������δ���������
   c.����ͻ����Լ���cust_str_table.xls, ���Բ���֣���������������Ϊcust_str_table.xls��cust_str_table1.xls��cust_str_table2.xls...
   d.�пͻ��������£��Ȱ�b�������Լ��ı��Ȼ����ͬ��b�Ĺ�����ͻ��ı��ID��ͬ������ͬ�����ݲ��յ����滻�������ݡ�
   e.����е�default���������ڲ�sheetҳ�����ȶ�ȡdefault������sheetҳ��������resview.ini���õ��Ⱥ�˳�����ζ�ȡ��ID��ͬ������ͬ�����ݲ��յ����滻�������ݡ�

   

[1.3.9.30.21]
1. ֧��IMG_CMP_PNG_ARGB_RLE �� IMG_CMP_BMP_32_ARGB_RLE��ʽ��
2. ֧��COMMONĿ¼
3. ֧��CSC
4. ֧�ֵ���ʹ�õ�String��Excel�ļ�
5. ÿ��String��History����Default��ϣ���History�ǿ����滻Default�е��ַ�����
6. �޸�Tag�����ǩreplΪcscs��CscsΪ�䳤�洢����ʡ�ռ䣬ͬʱ����lang��ǩ����ʾ�������ơ�
7. ֧��ͼƬ��ѹ��ΪPNG��ʽ��

[1.3.9.30.20]
1. ֧��ANIM_CMP_BMP_ARGB888, ANIM_CMP_PNG_ARGB888
2. ֧�����ã������ΪColor������64K��ARGB888ѹ��ʧ�ܣ�����ARGBѹ��
   �޸�Resview.ini, Ĭ�ϲ��Զ�ת��������򿪽�ARGB888TOARGB ��1
   [Suppers]
   ARGB888TOARGB = 0
3. ���PNG����BMP32��ɫ������64K����ARGB888ѹ�������߻����ز���Error
   ���ARGB888TOARGB = 1����Warning��
4. ����������û��ת�������⡣
5. ���WPS�����ʧ�ܣ�����ResView���������⡣
6. ���BMP8�ı��������
7. ����guangyao��BMP�������

[1.3.9.30.19]
1.JPG ȥ����RAW��ʽ��֧��
2.Str_table.xls֧�ָ�������õ�Sheet
3.�ͻ��潫Warning���Error��
4.SABM��һ��byte��ʾ�Ƿ�Ϊ��͸����ȫ��͸��Ϊ1.

[1.3.9.30.18]
1.�ͻ����ַ�����������Ĭ���滻������
2.BMP32�������Ʊ�����alpha
3.JPGѹ��֧��Dither
4.Color/Rect֧�ֿͻ�xls���Զ��ϲ�
5.�޸�BMP8�����
6.No Alpha֧����������

[1.3.9.30.17]
1.֧�ֲ�������ԴĿ¼
  -resdir res_path

[1.3.9.30.16]
1.֧�ֿ�����,ȥ���ַ���ͷ��ǣ�������Դsize��
2.֧�������в���  -langsplit [true|false]
	true�����ַ��������Է���Ϊ��������Դbin��
	False�����ֲ��䡣
3.֧��ѹ����ʽIMG_CMP_BMP_32_ARGB888 �� IMG_CMP_PNG_ARGB888
4.֧�ֿ����ã�һ�ζ��뻹�Ƿ�������Excel Sheet������ٶ���������WPS�����³�������⡣

[1.3.9.30.15]
1.֧�������в���  -rep [true|false]
    true����Ĭ����Դ�滻������ȫ�����棬����������Դ��
    false����Ĭ����Դ�滻������ȫ�����棬��������Դ��

[1.3.9.30.14]
1. str_table֧��History��
2. ���GIF������SizeԼ����bug��

[1.3.9.30.13]
1.���ͼƬ�����ڳ������ܱ������в����ڵ�ͼƬ������
2.���Excel������ݳ���8000�����޷���ȡ�����⡣
3.�������ÿ������ö�Excel�ķ�������MS Excel���������WPS ET�����
4.���Excel����ȡ�ձ����������
5.���TEXTͳ����Ϣ
6.֧��IMG_CMP_BMP_CK��ʽ


[1.3.9.30.12]
1.֧��IMG_CMP_RAW
2.����ͨ�����ã����ÿͻ��ַ������չѸ�ַ��������ĸ�Ϊ׼��
   ��ResView.ini��
   [ResTableSetting]
   StringFollowCust = 0
   Ĭ��StringFollowCust = 0������������ID�ظ����򱨴�
   StringFollowCust = 0������������ID�ظ����ѿͻ������ַ���Ϊ׼��ͬʱ�������档
 

[1.3.9.30.11]
1.֧��BMP(16/24/32)�ļ�ת��SABM��ʽͼƬ
   ѹ�����ͣ�IMG_CMP_BMP_SABM
   ֧�ֵ�FLAG��
   SABM_TILE 
   SABM_HOR_STRETCH
   SABM_VER_STRETCH  
   SABM_CEN_STRETCH  
2.֧��RES_ADD_LOGO
    ��ʽ��RES_ADD_LOGO(IMAGE_ID, IMAGE_PATH, IMAGE_COMPRESS_TYPE, IMAGE_QULITY_TYPE) 
3.֧��RES_ADD_XXX_TAG
   ��ʽ��RES_ADD_XXX_TAG( XXX_ID, "Description")
   ˵����Descriptionֻ����ASCII���ִ����������XXX_ID��
   Ŀǰ֧�֣�
   RES_ADD_IMG_TAG
   RES_ADD_ANIM_TAG
   RES_ADD_RING_TAG
   RES_ADD_COLOR_TAG

[1.3.9.30.10]
1.֧��PNG�ļ�ת��SABM��ʽͼƬ
   ѹ�����ͣ�IMG_CMP_PNG_SABM
   ֧�ֵ�FLAG�� 
   SABM_TILE 
   SABM_HOR_STRETCH
   SABM_VER_STRETCH  
   SABM_CEN_STRETCH  

[1.3.9.30.10]
1.֧��PNG�ļ�ת���ɶ�����SMOV ARGB��

[1.3.9.30.9]
1.����Jpeg����ʱ�����buffer̫С�������������Խ�磬���������
2.����Ring��size���Ϊ500K(512000)��
   MMI�����޸�\MS_Code\MS_MMI\source\resource\mmi_res_prj_def.h
   ���������RES_SET_MAX_RING_SIZE(512000)
   ���ò�ͬ��size���ƣ�ͬ����Ҫ��MS_Code\MS_MMI\source\mmi_app\common\h\macro_res_prj_def.h�����
   #undef RES_SET_MAX_RING_SIZE
   #define RES_SET_MAX_RING_SIZE(size) 

[1.3.9.30.8]
1.��������ʹ��WPS ET��bug��
2.����GIF�����bug��

[1.3.9.30.7]
1.�޸��ַ����滻���ԣ����������Ӣ���ַ��������ô��ַ������滻��������Ϊ�յ��ַ�����
2.�ű������Դbin��С�Ƿ���ȷ�������Դbin���ԣ���ɾ�����ǡ�
3.�ű�֧��Ĭ��ͼƬѹ����ʽ��
  ���MMI��Ҫ֧��Ĭ��ѹ������Ҫ�޸����£�
	a. �޸�\MS_Code\MS_MMI\source\resource\mmi_res_prj_def.h
	    �ڡ�//RES_SET_GLOBAL_JPEG_COMPRESS_RATE(75)��֮�£�����������ݣ�
		RES_SET_IMG_CMP_DEFAULT(BMP_8BIT,IMG_CMP_BMP_8_SCI,0)
		RES_SET_IMG_CMP_DEFAULT(BMP_16BIT,IMG_CMP_BMP_16_565_RLE,0)
		RES_SET_IMG_CMP_DEFAULT(BMP_24BIT,IMG_CMP_BMP_16_565_RLE,0)
		RES_SET_IMG_CMP_DEFAULT(BMP_32BIT,IMG_CMP_BMP_32_SCI,0)
		RES_SET_IMG_CMP_DEFAULT(JPG,IMG_CMP_JPG_SCI,75)
		RES_SET_IMG_CMP_DEFAULT(PNG,IMG_CMP_PNG_ARGB,0)
		RES_SET_IMG_CMP_DEFAULT(GIF,IMG_CMP_GIF_SCI,0)

		RES_SET_IMG_CMP_DEFAULT�÷���
		RES_SET_IMG_CMP_DEFAULT(ͼƬ����,ͼƬѹ������,ѹ������)
		����JPG������ͼƬ���͵ġ�ѹ�����������ǡ�0����û�����塣
		����JPG����ѹ��������Ϊѹ�����������ֵΪ100��

	b. �޸�ģ�鶨����Դͷ�ļ�*_mdu_def.h
	    �޸���Ҫ����Ĭ��ѹ����ͼƬ��Ӻ���RES_ADD_IMG
	    ��·���еĺ�׺��ȥ����ѹ���������óɡ�IMG_CMP_DEFAULT�����ɡ�

	c. �޸�\MS_Code\MS_MMI\source\mmi_app\common\h\macro_res_prj_def.h
	   �ڡ�#define RES_SET_GLOBAL_JPEG_COMPRESS_RATE(percent)  //none��������һ��
		#define RES_SET_IMG_CMP_DEFAULT(img_type,cmp_type,cmp_param)  //none

	˵����
	�ű�����BMP,JPG,PNG,GIF��˳������ͼƬ����ֻ��������ͼƬ��
	��������ˡ�IMG_CMP_DEFAULT��ѹ�����ͣ���ͼƬ·�����ְ����˺�׺������ű�ֻ����������ͼƬ��
5. [IMG��]����PNGת��ARGB��bug
6. [IMG��]��ANIM��GIF�Ľṹ��ʼ��Ϊ��
7. [IMG��]����PNGת��ARGB���ж��Ƿ���Alpha���жϡ�
8. ��������ET.Application�ĳɹ����жϡ�
9. ȥ��ResCmd��\-/-�ַ�log��

[1.3.9.30.6]
1.��Դ�ò�������Ĭ���滻��ͬʱ����warning����ʾ�ɹ�
2.ȥ��IMG������trace��

[1.3.9.30.5]
1.֧�ֿͻ��ַ����������Դ�Դ��ַ������ϲ�,��Ҫ��ResView.ini�����ÿͻ��ַ��������ļ�����
  ������
  [CustStrTable]
  1=cust_str_table.xls
2.������ʾID��enumֵ

[1.3.9.30.4]
1.��Դ������ֱ�ӱ�������Ĭ����Դ�滻��

[1.3.9.30.3]
1.�����ı���Դ��λ�����⣨���Դ�λ��
2.������ȡARGB��ѹ�����ݵĸ߶ȴ��󣬵��»�����Ϣ��׼ȷ
3.��Anim����ʱ�ļ��У��ŵ�ϵͳ����ʱĿ¼�¡�

[1.3.9.30.2]
1.�����ڴ渲�ǵ��³���Assert������

[1.3.9.30.1]
1.����Color��Rect��Դ��Ring,Font��Ϊ���⡣


[1.3.9.20.4]
1.ȥ��д��ʱ�ļ�����Ŀ¼�Ĵ��룬�����Ŀ¼û��Ȩ��д�ļ����¹��߱��������⡣

[1.3.9.20.2]
1.�޸�bmp_encoder.c
2.����Ring���͵�����Ӧ������׺��ȷ��Ring�����ͣ�
3.����JPEG��Ĭ��YUV�����ʡ�

[1.3.9.20.1]
1.����ResView����
2.�޸�alpha_res_encoder.c

[1.3.9.16.3]
1.���ͬʱ����������ʱ����ʧ�ܵ����⡣

[1.3.9.16.2]
1.�����wps����str_table.xlsʧ�ܵ����⡣
2.�����wps����str_table.xls�����齫wps���ĺ�İ�ȫ������Ϊ���͡���
  ����wps���ᵯ����ľ��棬ֻ�е�����������Ի����ϵİ�ť�����ܼ���������Դ��

[1.3.9.16.1]
1. ȥ��һЩͼƬѹ����ʽ
2. �ٶ��Ż�

[1.3.9.16.0]
1. ����GIF Codec
    �ɿ�����ƣ�240��320�����ӵ���1024��1024�����������1024��1024��������Ȼ�������⡣
2. ֧��BMP 8
    �������Ϊ8λBMP
    IMG_CMP_BMP_8_RAW
    IMG_CMP_BMP_8_SCI   

[1.3.9.15.9]
1. ֧��PNG
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
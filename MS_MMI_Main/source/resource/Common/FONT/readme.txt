���ʹ��font tool����Ĺ����Դ���˵���ĵ�.
����ʹ��font tool�е�shrink point size����, ��Сʵ�ʵ������С. һЩ������̩�ĵ�, 16������ĸ߶Ȼᳬ��16, ��ʱ, ����ʹ��14�������ʾ16������, ������ʾ��ȫ������. ����ʹ��customer shrink size�Ĺ���, ��ini�е���������ͳһ��С2��.
Ĭ�ϵ�latin�ֿ��ǵ�������, ���岻ȫ, Խ�������ȱʧ, ���Դ�windowsϵͳ�Դ���ARIALUNI.TTF����ȡ��ȫ��latin�����(��ϵͳ���Ҳ���������ɴ������ҵ�).
��������, ̩��, ӡ���������ⶼ�Ǵ�windowsϵͳ�Դ���ARIALUNI.TTF����ȡ. 
������������������������, ��˹����ڶ�����. ����������ʹ�õ�������һ��, ���в�ͬ����ʾ����, �����������������ͬʱ����, �����ARIALUNI.TTF����ȡ. �����ֻҪ��������, ���Դ�arial.ttf��times.ttf����ȡ(�����������Ϊwindows�Դ�), �ܼ��������ĳߴ�. 
ӡ����������������ӡ�ȷ���, ���ֻ��Ҫ�����(ͨ����ΪDevanagari��Hindi), ���޸�ini���MANGAL.TTF, akshar.ttf����ȡ(������������Դ������ҵ�), �ܼ��������ĳߴ�.
���Ŀ�ʹ��hans.ini����GB2312����,hant.ini����Big5����,hanst.ini����GB2312+Big5����. gbk.ini����GBK����. gbk_simple����GBKȥ�����������.

ʹ��compress�ı�ǽ�������ѹ��, �ή��2~5%����ʾЧ��, ��������30%���ҵ�ѹ����

ͨ�����ԶԱȷ���, arial unicode�е�������ʾ��̫��, ������ĳЩ���Ե��Ƽ�����. ���Ϊwindows xp�Դ��ĺ�������(windows 2k�ϲ�ȫ)
����御����mangal,
�ϼ���ʹ��Vrinda
̩¬��ʹ��Gautami
̩�׶� ʹ��Latha
���˽�(Gurmukhi) ʹ��Raavi
����ά��(Malayalam) ʹ��Kartika
�ż����� ʹ��Shruti
���ɴ���(Kannada) ʹ��Tunga
��������(ORIYA)ʹ��utkal
̩�� CourierThai
������(Syriac) ʹ��Estrangelo Edessa
������(Thaana) ʹ��MV Boli

//��mmi_res_prj_def.h����������

//ʸ������
#ifdef FONT_TYPE_SUPPORT_VECTOR

//������ʹ��latin.ini����
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN_VECTOR.lib","")

//���己�����Ķ�ʹ��HAN�ֿ�, ��������hans.ini����, ��������hant.ini����, ���己����hanst.ini����
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_VECTOR.lib","") 

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT
//��������ʹ��arabic.ini����
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC_VECTOR.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//̩��ʹ��thai.ini����
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI_VECTOR.lib","")
#endif

#if defined IM_HINDI_SUPPORT
//ӡ����ʹ��hindi.ini����
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI_VECTOR.lib","")
#endif

#ifdef IM_RUSSIAN_SUPPORT
//����ʹ��russian.ini����
RES_ADD_FONT(LANG_FONT_RUSSIAN,"\\FONT\\LANG_FONT_RUSSIAN_VECTOR.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//ϣ����ʹ��greek.ini����
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK_VECTOR.lib","")
#endif

//�Ҷ�����
#elif defined FONT_TYPE_SUPPORT_GRAY

//������ʹ��latin.ini����
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN_GRAY.lib","")

//���己�����Ķ�ʹ��HAN�ֿ�, ��������hans.ini����, ��������hant.ini����, ���己����hanst.ini����
#ifdef MAINLCD_SIZE_176X220
//176x220ʹ��14,16������
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_14_16_GRAY.lib","") 
#else
#ifdef PLATFORM_SC8800H
//8801hʹ��12,14,16,20������, ����Դ�ռ�ʣ��ϴ�, ��ʹ�ûҶ�����
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_8801h_GRAY.lib","") 
#else
//�������ʹ��16,20������
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_GRAY.lib","") 
#endif
#endif

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT
//��������ʹ��arabic.ini����
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC_GRAY.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//̩��ʹ��thai.ini����
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI_GRAY.lib","")
#endif

#if defined IM_HINDI_SUPPORT
//ӡ����ʹ��hindi.ini����
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI_GRAY.lib","")
#endif

#ifdef IM_RUSSIAN_SUPPORT
//����ʹ��russian.ini����
RES_ADD_FONT(LANG_FONT_RUSSIAN,"\\FONT\\LANG_FONT_RUSSIAN_GRAY.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//ϣ����ʹ��greek.ini����
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK_GRAY.lib","")
#endif

//��������
#else
//������ʹ��latin.ini����
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN.lib","")

//���己�����Ķ�ʹ��HAN�ֿ�, ��������hans.ini����, ��������hant.ini����, ���己����hanst.ini����
#ifdef MAINLCD_SIZE_176X220
//176x220ʹ��14,16������
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_14_16.lib","") 
#else
#ifdef PLATFORM_SC8800H
//8801hʹ��12,14,16,20������, ����Դ�ռ�ʣ��ϴ�, ��ʹ�ûҶ�����
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_8801h.lib","") 
#else
//�������ʹ��16,20������
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN.lib","") 
#endif
#endif

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT
//��������ʹ��arabic.ini����
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//̩��ʹ��thai.ini����
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI.lib","")
#endif

#if defined IM_HINDI_SUPPORT
//ӡ����ʹ��hindi.ini����
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI.lib","")
#endif

#ifdef IM_RUSSIAN_SUPPORT
//����ʹ��russian.ini����
RES_ADD_FONT(LANG_FONT_RUSSIAN,"\\FONT\\LANG_FONT_RUSSIAN.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//ϣ����ʹ��greek.ini����
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK.lib","")
#endif

#endif
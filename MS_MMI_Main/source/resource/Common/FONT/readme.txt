如何使用font tool请参阅工具自带的说明文档.
可以使用font tool中的shrink point size功能, 调小实际的字体大小. 一些外文如泰文等, 16号字体的高度会超过16, 这时, 建议使用14号字体表示16号字体, 避免显示不全的问题. 或者使用customer shrink size的功能, 把ini中的所有字体统一调小2号.
默认的latin字库是点阵字体, 字体不全, 越南语会有缺失, 可以从windows系统自带的ARIALUNI.TTF中提取较全的latin字体库(如系统中找不到该字体可从网上找到).
阿拉伯语, 泰语, 印度语的字体库都是从windows系统自带的ARIALUNI.TTF中提取. 
阿拉伯语字体库包含阿拉伯语, 波斯语和乌尔都语. 这三种语言使用的字形是一套, 但有不同的显示规则, 所以如果这三种语言同时存在, 必须从ARIALUNI.TTF中提取. 但如果只要阿拉伯语, 可以从arial.ttf或times.ttf中提取(这两个字体库为windows自带), 能减少字体库的尺寸. 
印度语字体库包括八种印度方言, 如果只需要天城体(通常称为Devanagari或Hindi), 可修改ini或从MANGAL.TTF, akshar.ttf中提取(这两个字体可以从网上找到), 能减少字体库的尺寸.
中文可使用hans.ini生成GB2312字体,hant.ini生成Big5字体,hanst.ini生成GB2312+Big5字体. gbk.ini生成GBK字体. gbk_simple生成GBK去掉繁体的字体.

使用compress的标记进行字体压缩, 会降低2~5%的显示效率, 但能增加30%左右的压缩率

通过测试对比发现, arial unicode有的字形显示不太对, 以下是某些语言的推荐字体. 大多为windows xp自带的核心字体(windows 2k上不全)
天城体尽量用mangal,
孟加拉使用Vrinda
泰卢固使用Gautami
泰米尔 使用Latha
锡克教(Gurmukhi) 使用Raavi
德拉维族(Malayalam) 使用Kartika
古吉拉特 使用Shruti
卡纳达文(Kannada) 使用Tunga
奥里亚文(ORIYA)使用utkal
泰语 CourierThai
叙利亚(Syriac) 使用Estrangelo Edessa
塔纳文(Thaana) 使用MV Boli

//在mmi_res_prj_def.h中添加字体库

//矢量字体
#ifdef FONT_TYPE_SUPPORT_VECTOR

//拉丁语使用latin.ini生成
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN_VECTOR.lib","")

//简体繁体中文都使用HAN字库, 纯简体用hans.ini生成, 纯繁体用hant.ini生成, 简体繁体用hanst.ini生成
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_VECTOR.lib","") 

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT
//阿拉伯语使用arabic.ini生成
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC_VECTOR.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//泰语使用thai.ini生成
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI_VECTOR.lib","")
#endif

#if defined IM_HINDI_SUPPORT
//印度语使用hindi.ini生成
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI_VECTOR.lib","")
#endif

#ifdef IM_RUSSIAN_SUPPORT
//俄语使用russian.ini生成
RES_ADD_FONT(LANG_FONT_RUSSIAN,"\\FONT\\LANG_FONT_RUSSIAN_VECTOR.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//希腊语使用greek.ini生成
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK_VECTOR.lib","")
#endif

//灰度字体
#elif defined FONT_TYPE_SUPPORT_GRAY

//拉丁语使用latin.ini生成
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN_GRAY.lib","")

//简体繁体中文都使用HAN字库, 纯简体用hans.ini生成, 纯繁体用hant.ini生成, 简体繁体用hanst.ini生成
#ifdef MAINLCD_SIZE_176X220
//176x220使用14,16号字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_14_16_GRAY.lib","") 
#else
#ifdef PLATFORM_SC8800H
//8801h使用12,14,16,20号字体, 如资源空间剩余较大, 可使用灰度字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_8801h_GRAY.lib","") 
#else
//其他情况使用16,20号字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_GRAY.lib","") 
#endif
#endif

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT
//阿拉伯语使用arabic.ini生成
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC_GRAY.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//泰语使用thai.ini生成
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI_GRAY.lib","")
#endif

#if defined IM_HINDI_SUPPORT
//印度语使用hindi.ini生成
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI_GRAY.lib","")
#endif

#ifdef IM_RUSSIAN_SUPPORT
//俄语使用russian.ini生成
RES_ADD_FONT(LANG_FONT_RUSSIAN,"\\FONT\\LANG_FONT_RUSSIAN_GRAY.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//希腊语使用greek.ini生成
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK_GRAY.lib","")
#endif

//点阵字体
#else
//拉丁语使用latin.ini生成
RES_ADD_FONT(LANG_FONT_LATIN,"\\FONT\\LANG_FONT_LATIN.lib","")

//简体繁体中文都使用HAN字库, 纯简体用hans.ini生成, 纯繁体用hant.ini生成, 简体繁体用hanst.ini生成
#ifdef MAINLCD_SIZE_176X220
//176x220使用14,16号字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_14_16.lib","") 
#else
#ifdef PLATFORM_SC8800H
//8801h使用12,14,16,20号字体, 如资源空间剩余较大, 可使用灰度字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN_8801h.lib","") 
#else
//其他情况使用16,20号字体
RES_ADD_FONT(LANG_FONT_HAN,"\\FONT\\LANG_FONT_HAN.lib","") 
#endif
#endif

#if defined IM_ARABIC_SUPPORT || defined IM_URDU_SUPPORT || defined IM_PERSIAN_SUPPORT
//阿拉伯语使用arabic.ini生成
RES_ADD_FONT(LANG_FONT_ARABIC,"\\FONT\\LANG_FONT_ARABIC.lib","")
#endif

#ifdef IM_THAI_SUPPORT
//泰语使用thai.ini生成
RES_ADD_FONT(LANG_FONT_THAI,"\\FONT\\LANG_FONT_THAI.lib","")
#endif

#if defined IM_HINDI_SUPPORT
//印度语使用hindi.ini生成
RES_ADD_FONT(LANG_FONT_HINDI,"\\FONT\\LANG_FONT_HINDI.lib","")
#endif

#ifdef IM_RUSSIAN_SUPPORT
//俄语使用russian.ini生成
RES_ADD_FONT(LANG_FONT_RUSSIAN,"\\FONT\\LANG_FONT_RUSSIAN.lib","")
#endif

#ifdef IM_GREEK_SUPPORT
//希腊语使用greek.ini生成
RES_ADD_FONT(LANG_FONT_GREEK,"\\FONT\\LANG_FONT_GREEK.lib","")
#endif

#endif
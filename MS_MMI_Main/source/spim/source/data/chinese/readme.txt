参看文档 __File_list.txt

所有生成的最终的数据文件，都以下划线开头。

数据文件的来源是网络或者通过一些非固定的程序生成，我们只需要最终的数据。

gb2312.txt - 该文件的内部结构为“{0xuuuu, 0xgbgb},”，即所有的 gb2312 的字符对应的 unicode 值。
chs_vs_cht.txt - 汉字简繁对照文件。内部结构为“{0xssss, 0xtttt},”的格式。
use_frequence.txt - 由 read_book_dir.pl 生成的数据文件 _DIR_read_data_freq.dat 手动转存成 unicode 格式。该文件内是汉字按照常用顺序排列而成的。
以上文件在 gen_ch_data.pm 中用作数据文件。

ucs_pinyin_spim.txt - 以usc拼音数据为蓝本，在格式上做了一些修改，并参考其他的拼音数据，通过取交集的方式，去掉了超过 5 个读音的汉字的不常见读音。用于 gen_ch_char.pl 中生成拼音数据文件。

SogouLabDic.dic - 数据文件，utf8格式。用于脚本 gen_ch_asso.pl 生成联想字代码文件。

stroke_chs.txt - 简体中文笔划数据
stroke_cht.txt - 繁体中文笔划数据
以上两个文件的数据由其他程序得到，用于生成笔划源文件。


脚本文件的说明详见各脚本最后的注释。
脚本文件是指 *.pm, *.pl, *.bat
特别的，脚本文件的输入输出文件都是写死的，并且必须满足一定的格式。并不是通用程序。

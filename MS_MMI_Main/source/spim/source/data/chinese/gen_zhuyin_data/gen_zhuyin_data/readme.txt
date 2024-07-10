使用：
1、按顺序依次调用perl批处理
2、此方案是生成9键的数据，如果要生成26键的，则将5_gen_dst_data.pl的zhuyin_9key_layout.txt
改成zhuyin_26key_layout.txt即可
3、如果要新增字，则只要在zhu_pin.txt文件里增加对应的拼音及汉字即可

各文件：
frequence.txt：字频
hanzi_unicode.txt：所有汉字的unicode码
zhu_pin.txt：繁体字的拼音与汉字的对照表
zy_py_map.txt：拼音与注音符号之音的对照表
zhuyin_9key_layout.txt：9键的键位布局
zhuyin_9key_layout_org.txt：9键上对应的注音符号
zhuyin_26key_layout.txt：26键的键位布局
zhuyin_26key_layout_org.txt：26键上对应的注音符号
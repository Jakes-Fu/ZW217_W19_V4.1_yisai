@rem char
@echo ==== Generane the character data ====
@perl gen_ch_char.pl
@perl gen_pinyin_9k.pl
@rem word
@echo ==== Generane the word data ====
@REM ����Ĳ��� n ��ʾ2��3��4�ִʻ�����Ϊ (3000, 2000, 2000) �� n ��
@perl gen_ch_word.pl 4
@rem associate
@echo ==== Generane the associate data ====
@perl gen_ch_asso.pl
@rem stroke
@echo ==== Generane the stroke data ====
@perl gen_stroke_data.pl stroke_chs
@perl gen_stroke_data.pl stroke_cht
@echo ==== END ====

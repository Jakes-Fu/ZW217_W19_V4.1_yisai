
open (ZHU_PIN_DATA, "zhu_pin.txt") or die "1";
open (ZHU_FREQUENCE_DATA, "frequence.txt") or die "@";
open (ZY_PY_MAP, "zy_py_map.txt") or die "2";
open (HANZI_UNICODE, "hanzi_unicode.txt") or die "3";

open (ZHUYIN_DATA, ">zhuyin_data.txt") or die "11";
open (USED_ZHU_FRE, ">_chrset_zy.src") or die "11";

#拼音映射到注音
while (<ZY_PY_MAP>) {
    chomp;

    my @cur_zy_py_map = split /;/, $_;
    
    if (@cur_zy_py_map > 1) {       
        $zy_py_map{$cur_zy_py_map[1]} = $cur_zy_py_map[0];
    }
}

#所有汉字的unicode值
while (<HANZI_UNICODE>) {
    chomp;

    if (/(.{2}):(\w{6})/) {
        $hanzi_unicode{$1} = $2;
    }
}

#繁体字字频
while (<ZHU_FREQUENCE_DATA>) {
    chomp;

    if (length > 1) {               
        push @zhu_frequence, $_;
    }
}

#汉字拼音数据
while (<ZHU_PIN_DATA>) {
    chomp;

    if (/(\w*)\s*(.{2})/) {
        die "1" unless defined($zy_py_map{$1});

        my $cur_zhuyin = $zy_py_map{$1};    #该拼音对应的注音
        my $cur_hanzi_unicode = $hanzi_unicode{$2};     #该汉字对应的unicode

        if (!defined($all_hanzi_unicode{$cur_zhuyin})) {
            $all_hanzi_unicode{$cur_zhuyin} = "$1\n";
        }

        $all_hanzi_unicode{$cur_zhuyin} .= "$cur_hanzi_unicode ";
        
        $all_used_hanzi_hash{$2} = 1;
    }
}

@all_hanzi_key = keys %all_hanzi_unicode;
foreach $cur_hanzi (@all_hanzi_key) {
    print ZHUYIN_DATA "$cur_hanzi\n$all_hanzi_unicode{$cur_hanzi}\n\n\n";
}

foreach my $cur_hanzi (@zhu_frequence) {
    if (defined($all_used_hanzi_hash{$cur_hanzi})) {
        print USED_ZHU_FRE $hanzi_unicode{$cur_hanzi}, ",\n";
    }
}

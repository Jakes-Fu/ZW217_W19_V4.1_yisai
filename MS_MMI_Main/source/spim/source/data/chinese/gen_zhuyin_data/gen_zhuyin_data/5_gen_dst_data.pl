#生成所有的注音表，注音对应的按键表，及每个注音的字数
#[in]
#        zhuyin_keypad.txt
#        1.txt
#        3.txt

#[out]
#        _zhuyin.txt
#        zhuyin_9k.txt
#        _zy_cnt.txt
#        _chr_zy_idx.txt
#        _chr_zy_mp2.txt
#        _chr_zy_mp3.txt
#        _chr_zy_mp4.txt


#全局变量
%letter_index_hash;

#1. 读取每个注音符号对应的按键
open (ZHUYIN_KEYPAD, "zhuyin_9key_layout.txt") or die "a";

while (<ZHUYIN_KEYPAD>) {
    chomp;

    #　键位与键值用" : "分隔
    if (/ : /) {
        my $cur_key = $`;

        #　键值之间用", "分隔
        my @cur_letters = split /, /, $';       #'

        foreach my $cur_letter (@cur_letters) {
            $key_mapping_hash{$cur_letter} = $cur_key;
        }
    }
}

close ZHUYIN_KEYPAD;

#2. 读取注音信息的文件，包括注音、拼音、字数
open (ZHUYIN_INFO, "3.txt") or die "a";

$state = 0;
$last_first_letter = "";
$cur_key_mapping = "";
$cur_letter = "";
$cur_pinyin = "";

while (<ZHUYIN_INFO>) {
    if ($_ eq "\n") {               #回车
        die "2 " if $state == 2;

        $state = 0;
    }
    elsif ($state == 0) {         #注音
        chomp;

        $cur_letter = $_;

        #　注音以" "分隔
        my @cur_letters = split / /, $_;

        #　如果第一个辅音还未定义，加入
        if ($cur_letters[0] ne $last_first_letter) {
            $last_first_letter = $cur_letters[0];
            
            $all_zhuyin{"$last_first_letter "} = "$last_first_letter ;$key_mapping_hash{$last_first_letter};0;$last_first_letter";
        }

        $cur_key_mapping = "";

        #　连起所有的按键
        foreach my $cur_letter (@cur_letters) {
            $cur_key_mapping .= $key_mapping_hash{$cur_letter};
        }

        $state = 1;
    }
    elsif ($state == 1) {           #拼音
        chomp;
        $cur_pinyin = $_;

        $state = 2;
    }
    elsif ($state == 2) {           #字的个数
        chomp;

        $all_zhuyin{$cur_letter} = "$cur_letter;$cur_key_mapping;$_;$cur_pinyin";

        $state = 1;
    }
}

close ZHUYIN_INFO;

#3. 生成注音表，按键表，个数表
open (ZHUYIN_TABLE, ">_zhuyin.src") or die "1";
open (ZHUYIN_KEY, ">zhuyin_9k.src") or die "2";
open (ZHUYIN_COUNT, ">_zy_cnt.src") or die "3";
open (ZHUYIN_PINYIN, ">zhuyin_pinyin.txt") or die "4";


#3.1 按长度排序
@all_zhuyin_letters_temp = sort keys %all_zhuyin;

foreach $this_letter (@all_zhuyin_letters_temp) {
    my $this_letter_length = length($this_letter);
    my $this_letter_key = sprintf "%2d%s", $this_letter_length, $this_letter;
    $this_letter_hash{$this_letter_key} = $this_letter;
    $this_letter_key_count{(sprintf "%2d", $this_letter_length)}++;
}

@temp_letters = sort keys %this_letter_hash;
@letter_key_count = reverse sort keys %this_letter_key_count;

foreach $i (@letter_key_count) {
    $temp_i = sprintf "%2d", $i;
    foreach $this_letter (@temp_letters) {
        if (substr($this_letter, 0, 2) eq $temp_i) {
            push @all_zhuyin_letters, $this_letter_hash{$this_letter};
        }
    }
}

#3.2　按unicode排序
#@all_zhuyin_letters = sort keys %all_zhuyin;


$cur_count = 0;
$letter_index = 0;

foreach my $cur_letter (@all_zhuyin_letters) {
    my @cur_data = split /;/, $all_zhuyin{$cur_letter};

    $cur_count++;

#打印注音表
    $zhuyin_data = shift @cur_data;
    
    # 注音在表中的索引值，以后会用
    $letter_index_hash{$zhuyin_data} = $letter_index;
    $letter_index++;
    
    $cur_zhuyin_letter = "";
    while($zhuyin_data =~ /0x(\w{4})/) {
        $cur_zhuyin_letter .= "\\x$1";
        $zhuyin_data = $';                  #'
    }
    
    $cur_length = length($cur_zhuyin_letter) + 5;
    print ZHUYIN_TABLE "L\"$cur_zhuyin_letter\",", " " x (24 - $cur_length);
    
    if ($cur_count % 4 == 0) {
        print ZHUYIN_TABLE "\n";
    }

#生成按键表
    $zhuyin_data = shift @cur_data;

    $cur_length = length($zhuyin_data) + 4;
    print ZHUYIN_KEY "\"$zhuyin_data\",", " " x (10 - $cur_length);
    
    if ($cur_count % 4 == 0) {
        print ZHUYIN_KEY "\n";
    }

#生成个数表
    $zhuyin_data = shift @cur_data;

    $cur_length = length($zhuyin_data) + 4;
    
    print ZHUYIN_COUNT "$zhuyin_data,", " " x (10 - $cur_length);
    
    if ($cur_count % 4 == 0) {
        print ZHUYIN_COUNT "\n";
    }

#生成注音拼音表
    $zhuyin_data = shift @cur_data;

    $cur_length = length($zhuyin_data) + 4;
    
    print ZHUYIN_PINYIN "$zhuyin_data,", " " x (20 - $cur_length);
    
    if ($cur_count % 4 == 0) {
        print ZHUYIN_PINYIN "\n";
    }
}

close ZHUYIN_TABLE;
close ZHUYIN_KEY;
close ZHUYIN_COUNT;
close ZHUYIN_PINYIN;


#4. 生成字的索引表
open (ZHUYIN_LETTER_WORD, "1.txt") or die "2";

$state = 0;
%zhuyin_word_mapping;

while (<ZHUYIN_LETTER_WORD>) {
    if ($_ eq "\n") {               #回车
        die "2 " if $state == 2;
        $state = 0;
    }
    elsif ($state == 0) {         #注音
        chomp;
        
        $cur_zhuyin_letter = $_;

        $state = 1;
    }
    elsif ($state == 1) {           #拼音
        $state = 2;
    }
    elsif ($state == 2) {           #字
        chomp;

        @zhuyin_words = split / /, $_;

        foreach $cur_word (@zhuyin_words) {
            $zhuyin_word_mapping{$cur_word} .= "$cur_zhuyin_letter,";
        }

        $state = 1;
    }
}

open ($file_hash{1}, ">_chr_zy_idx.src") or die "a";
open ($file_hash{2}, ">_chr_zy_mp2.src") or die "a";
open ($file_hash{3}, ">_chr_zy_mp3.src") or die "a";
open ($file_hash{4}, ">_chr_zy_mp4.src") or die "a";
open ($file_hash{5}, ">_chr_zy_mp5.src") or die "a";
open ($file_hash{6}, ">_chr_zy_mp6.src") or die "a";

@multi_pron = (0, 0, 0x2000, 0x4000, 0x6000);

@all_words = keys %zhuyin_word_mapping;

foreach $cur_word (@all_words) {
    @cur_zhuyin = split /,/, $zhuyin_word_mapping{$cur_word};
    $cur_zhuyin_count = @cur_zhuyin;
    
    if ($cur_zhuyin_count == 1) {
        die "s" unless defined $letter_index_hash{$cur_zhuyin[0]};

        $all_zhuyin_hash{$cur_word} = &get_hex($cur_zhuyin[0]);
    }
    elsif ($cur_zhuyin_count > 1) {
        die "a" if $cur_zhuyin_count > 6;

        $all_zhuyin_hash{$cur_word} = sprintf "0x%x", $multi_pron[$cur_zhuyin_count] + $pron_count[$cur_zhuyin_count];
        print $all_zhuyin_hash{$cur_word}, "   ";
        
        print {$file_hash{$cur_zhuyin_count}} "{";

        @cur_index_pool = ();
        for ($i = 0; $i < $cur_zhuyin_count; $i++) {
            push @cur_index_pool, &get_hex($cur_zhuyin[$i]);
        }
        print {$file_hash{$cur_zhuyin_count}} join ", ", @cur_index_pool;

        print {$file_hash{$cur_zhuyin_count}} "}, ";

        $pron_count[$cur_zhuyin_count]++;
        if ($pron_count[$cur_zhuyin_count] % 4 == 0) {
            print {$file_hash{$cur_zhuyin_count}} "\n";
        }
    }
}

$count = 0;

foreach $cur_word (0x4e00..0x9fa5) {
    $cur_word_hex = sprintf "0x%x", $cur_word;
    if (defined($all_zhuyin_hash{$cur_word_hex})) {
        print {$file_hash{1}} $all_zhuyin_hash{$cur_word_hex}, ", ";
    }
    else {
        print {$file_hash{1}} "0x0000, ";
    }

    $count++;
    if ($count % 10 == 0) {
        print {$file_hash{1}} "\n";
    }
}

#foreach $i (1..4) {
#    close {$file_hash{$i}};
#}

sub get_hex {
    $my_data = sprintf "%x", $letter_index_hash{$_[0]};
    $my_data = sprintf "0x%s%s", "0" x (4 - length($my_data)), $my_data;
    
    return $my_data;
}

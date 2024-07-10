#统计unicode码及对应的个数

open (ZHUYIN_DATA, "1.txt") or die "!";

open (DATA1, ">2.txt") or die "1";
select DATA1;

$state = 0;

while (<ZHUYIN_DATA>) {
    if ($_ eq "\n") {               #回车
        die "2 " if $state == 2;
        $state = 0;
    }
    elsif ($state == 0) {         #注音
        $state = 1;
    }
    elsif ($state == 1) {           #拼音
        $state = 2;
    }
    elsif ($state == 2) {           #字
        $state = 1;
        
        chomp;
        
        my @data = split / /, $_;
        foreach $key (@data) {
            $data_hash{$key}++;
        }
    }
}

@all_word = keys %data_hash;
$max_word_count = 0;
foreach $cur_key (@all_word) {
    $cur_word_count = $data_hash{$cur_key};
    if ($cur_word_count > $max_word_count) {
        $max_word_count = $cur_word_count;
    }

    print "$cur_key, // $cur_word_count\n";
}

select STDOUT;
$count = @all_word;
print "$count, $max_word_count";

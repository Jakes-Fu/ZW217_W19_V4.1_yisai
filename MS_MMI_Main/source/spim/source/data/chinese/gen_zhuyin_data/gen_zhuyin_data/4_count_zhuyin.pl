#生成已有的拼音及注音的符号

open (ZHUYIN_DATA, "1.txt") or die "!";

open (DATA1, ">3.txt") or die "1";
select DATA1;

$state = 0;

while (<ZHUYIN_DATA>) {
    if ($_ eq "\n") {               #回车
        die "2 " if $state == 2;
        print $_;
        $state = 0;
    }
    elsif ($state == 0) {         #注音
        print $_;
        $zhuyin_hash{$_} ++;

        if ($zhuyin_hash{$_} > 1) {     #看看有没有相同的注音
            die "a";
        }

        $state = 1;
    }
    elsif ($state == 1) {           #拼音
        print $_;
        $state = 2;
    }
    elsif ($state == 2) {           #字
        $state = 1;

        chomp;
        my @data = split / /, $_;
        my $data_count = @data;
        print "$data_count\n"
    }
}

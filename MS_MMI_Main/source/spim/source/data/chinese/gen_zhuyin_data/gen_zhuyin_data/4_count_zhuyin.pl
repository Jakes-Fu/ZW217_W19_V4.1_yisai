#�������е�ƴ����ע���ķ���

open (ZHUYIN_DATA, "1.txt") or die "!";

open (DATA1, ">3.txt") or die "1";
select DATA1;

$state = 0;

while (<ZHUYIN_DATA>) {
    if ($_ eq "\n") {               #�س�
        die "2 " if $state == 2;
        print $_;
        $state = 0;
    }
    elsif ($state == 0) {         #ע��
        print $_;
        $zhuyin_hash{$_} ++;

        if ($zhuyin_hash{$_} > 1) {     #������û����ͬ��ע��
            die "a";
        }

        $state = 1;
    }
    elsif ($state == 1) {           #ƴ��
        print $_;
        $state = 2;
    }
    elsif ($state == 2) {           #��
        $state = 1;

        chomp;
        my @data = split / /, $_;
        my $data_count = @data;
        print "$data_count\n"
    }
}

#����ע��������������Ҫ������
#���������������е����ݷ���һ��ƴ����
#������治׼����������������

open (ZHUYIN_DATA, "zhuyin_data.txt") or die "1";
open (DATA1, ">1.txt") or die "1";

select DATA1;

$state = 0;
$is_print_py = 0;

while (<ZHUYIN_DATA>) {
    if ($_ eq "\n") {               #�س�
        my $key_count = keys %data_hash;
        if ($key_count > 0) {
            print join " ", keys %data_hash;
        }

        print $_;
        die "2 " if $state == 2;
        $state = 0;
        $is_print_py = 0;
        %data_hash = ();
    }
    elsif ($state == 0) {         #ע��
        print $_;
        $state = 1;
    }
    elsif ($state == 1) {           #ƴ��
        $state = 2;
        
        if (0 == $is_print_py) {
            print $_;
            $is_print_py = 1;
        }
    }
    elsif ($state == 2) {           #��
        $state = 1;
        
        chomp;
        
        my @data = split / /, $_;
        foreach $key (@data) {
            $data_hash{$key}++;
        }
    }
}

print "\n\n";



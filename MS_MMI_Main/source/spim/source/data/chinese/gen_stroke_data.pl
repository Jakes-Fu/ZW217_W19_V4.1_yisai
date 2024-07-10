#c:\perl\bin -w
use strict;

use gen_ch_data qw (%freq_data);

die "Usage: exec file - input file.txt, output file.src" if (@ARGV != 1);

my $input_file = $ARGV[0] . ".txt";
my $output_file = "_" . $ARGV[0] . ".src";

my %stroke_data;

open(IN_FILE, "<", $input_file) or die "$input_file: $!";

while (<IN_FILE>) {
	chomp;
	if (/^{(0x[0-9a-f]{4}), "([1-5]+)"},$/) {
		$stroke_data{$1} = $2;
	}
}

close(IN_FILE);

#笔顺排序
my @uni_char = sort {$stroke_data{$a} cmp $stroke_data{$b}} keys %stroke_data;

#使用频率越高权重越小
my @seq_freq = sort {$freq_data{hex $b} <=> $freq_data{hex $a}} @uni_char;
my %height_freq;
$height_freq{$seq_freq[$_]} = ($_ + 1) foreach (0..$#seq_freq);

#笔划越少，权重越小
my @seq_len = sort {length($stroke_data{$a}) <=> length($stroke_data{$b})} @uni_char;
my %height_len;
$height_len{$seq_len[$_]} = ($_ + 1) foreach (0..$#seq_len);

#加权函数
sub c_height {
	my ($ch) = @_;
	return ($height_freq{$ch} < $height_len{$ch} ? $height_freq{$ch}: $height_len{$ch});
}
#权重小的在前
@uni_char = sort {c_height($a) <=> c_height($b)} @uni_char;


open(OUT_FILE, ">", $output_file) or die "$output_file: $!";

print OUT_FILE "{$_, \"$stroke_data{$_}\"},\n" foreach (@uni_char);

close(OUT_FILE);

__END__
{0x4e00, "1"},
input - *.txt
output - *.src

关于排序，有三种排序方式
1 按照笔划来排序，即横1竖2撇3点4勾5
2 按照使用频率来排序
3 按照笔划数来排序

阐述一下最终按照笔划数排序的重要性：这样可以优先给出符合当前笔划数的字。
比如“乙”字，使用频率不算考前，如果不按笔画数最终排序，则这个字将会很难被索引。

但是单纯的使用笔画数作为最终排序也不好，因为还是有很多的常用字要翻很久才出来。

采用一种加权算法，按照“常用字序”和“笔划数序”联合排序。
1 需要拉开权重的差距
2 需要增加常用字的权重

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

#��˳����
my @uni_char = sort {$stroke_data{$a} cmp $stroke_data{$b}} keys %stroke_data;

#ʹ��Ƶ��Խ��Ȩ��ԽС
my @seq_freq = sort {$freq_data{hex $b} <=> $freq_data{hex $a}} @uni_char;
my %height_freq;
$height_freq{$seq_freq[$_]} = ($_ + 1) foreach (0..$#seq_freq);

#�ʻ�Խ�٣�Ȩ��ԽС
my @seq_len = sort {length($stroke_data{$a}) <=> length($stroke_data{$b})} @uni_char;
my %height_len;
$height_len{$seq_len[$_]} = ($_ + 1) foreach (0..$#seq_len);

#��Ȩ����
sub c_height {
	my ($ch) = @_;
	return ($height_freq{$ch} < $height_len{$ch} ? $height_freq{$ch}: $height_len{$ch});
}
#Ȩ��С����ǰ
@uni_char = sort {c_height($a) <=> c_height($b)} @uni_char;


open(OUT_FILE, ">", $output_file) or die "$output_file: $!";

print OUT_FILE "{$_, \"$stroke_data{$_}\"},\n" foreach (@uni_char);

close(OUT_FILE);

__END__
{0x4e00, "1"},
input - *.txt
output - *.src

������������������ʽ
1 ���ձʻ������򣬼���1��2Ʋ3��4��5
2 ����ʹ��Ƶ��������
3 ���ձʻ���������

����һ�����հ��ձʻ����������Ҫ�ԣ������������ȸ������ϵ�ǰ�ʻ������֡�
���硰�ҡ��֣�ʹ��Ƶ�ʲ��㿼ǰ����������ʻ�����������������ֽ�����ѱ�������

���ǵ�����ʹ�ñʻ�����Ϊ��������Ҳ���ã���Ϊ�����кܶ�ĳ�����Ҫ���ܾòų�����

����һ�ּ�Ȩ�㷨�����ա��������򡱺͡��ʻ�������������
1 ��Ҫ����Ȩ�صĲ��
2 ��Ҫ���ӳ����ֵ�Ȩ��

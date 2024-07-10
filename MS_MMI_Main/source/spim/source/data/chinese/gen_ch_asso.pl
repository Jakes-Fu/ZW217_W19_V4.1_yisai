#!c:\perl\bin -w
use strict;
use Encode;

use gen_ch_data qw (%freq_data is_gb2312 utf8_uni);

my $word_file = "SogouLabDic.dic";
my $src_filename = "_asso.src";

my $ASSO_CH_CNT = 32 - 1;
my $IDIOM_ADDED_FREQ = 999999;

my %next_data = ();

print "Reading: $word_file\n";

die "$word_file: $!" if (!open(IN_FILE, "<:utf8", $word_file));

while (<IN_FILE>){
	my $word;
	my $freq = 0;
	
	if (/([\x{4e00}-\x{9fa5}]+)\s+(\d+)\s+IDIOM/) {
		$freq = $2;
		#成语，固定加一个权重
		$freq += $IDIOM_ADDED_FREQ;
		$word = $1;
	}
	elsif (/([\x{4e00}-\x{9fa5}]+)\s+(\d+)/) {
		$freq = $2;
		$word = $1;
	}
	else {
		next;
	}	
	
	my $word_len = length($word);
	
	#一个字在另一个字之后出现的频率
	foreach my $i (0 .. $word_len - 2) {
		my $ch1 = substr($word, $i, 1);
		my $ch2 = substr($word, $i + 1, 1);
		
		#增加gb2312判断，既然是联想字，那就可以忽略非常用字
		next unless (is_gb2312(utf8_uni($ch1)) && is_gb2312(utf8_uni($ch2)));
		
		$next_data{$ch1}{$ch2} += $freq;
	}
}

close(IN_FILE);

#output to file
print "Write data to code file $src_filename...\n";

my %asso_data;

foreach my $ch0 (sort {$freq_data{utf8_uni($b)} <=> $freq_data{utf8_uni($a)}} keys %next_data) {
	push @{$asso_data{0}} , utf8_uni($ch0) if ($#{$asso_data{0}} < $ASSO_CH_CNT);
	push @{$asso_data{utf8_uni($ch0)}}, utf8_uni($_) foreach (sort {$next_data{$ch0}{$b} <=> $next_data{$ch0}{$a}} keys %{$next_data{$ch0}});
}

open(SRC_FILE, ">", $src_filename) or die "$src_filename: $!";

foreach my $uni (sort keys %asso_data) {
	$#{$asso_data{$uni}} = $ASSO_CH_CNT if ($#{$asso_data{$uni}} > $ASSO_CH_CNT);	
	printf SRC_FILE "{0x%04x, L\"", $uni;
	printf SRC_FILE "\\x%04x", $_ foreach (@{$asso_data{$uni}});
	printf SRC_FILE "\"},\n";
}

close(SRC_FILE);

print "END...\n";
__END__
1 读入 SogouLabDic.dic，UTF-8格式的文档。
2 解析文件的每一行，统计联想字数据
3 联想字数据的两个属性：
	字，来自词内的后一个字
	频率，来自词的本身频率
4 对成语做了一个固定的频率加成，即使用频率加上一个固定值，比如 999999
5 将数据转化成 unicode 码写入代码文件 _asso.src
6 该脚本是采用的UTF-8格式处理文本，与其他的unicode格式不同，这样应该好一些。但是
那些脚本已稳定，就不修改了。




#c:\perl\bin -w
use strict;
use Encode;

use gen_ch_data qw (is_gb13000 is_gb2312 utf8_uni);

my $word_file = "SogouLabDic.dic";

#word count of length = 2,3,4, base count
my @mini_dict_size = (3000, 2000, 2000); 

#reset dict size
if (defined $ARGV[0] and 0 < $ARGV[0] and $ARGV[0] < 10) {
	$mini_dict_size[$_] *= $ARGV[0] foreach (0..$#mini_dict_size);
}

my @dict = ();

################################################################################
#word data
print "Reading: $word_file\n";

die "$word_file: $!" if (!open(IN_FILE, "<:utf8", $word_file));

READ_WORD: while (<IN_FILE>){
	my @word_data = ();
	my $word;
	
	if (/([\x{4e00}-\x{9fa5}]{2,})\s+\d+/) {
		$word = $1;
	}
	else {
		next;
	}
	
	my $word_len = length($word);
	
	foreach my $i (0 .. $word_len - 1) {
		my $ch = substr($word, $i, 1);
		my $uni = utf8_uni($ch);
		#增加gb2312判断，忽略非常用字
		next READ_WORD unless is_gb2312($uni);
		push @word_data, $uni;
	}
	
	push @dict, [@word_data];
}

close(IN_FILE);


print "Sort the word by length, and keep the order in the same length...\n";

@dict = sort {$#$a <=> $#$b} @dict;

die "the length of the first word must be 2~" if ($#{$dict[1]} != 1);

################################################################################
#Output to source File
print "Make source file...\n";

################################################################################
#generate word data

my %len_info;

my $first_idx = 0;
my $cur_len = 2;

foreach (0 .. $#dict) {
	if ($cur_len < $#{$dict[$_]} + 1) {
		$len_info{$cur_len}{"first"} = $first_idx;
		$len_info{$cur_len}{"count"} = $_ - $first_idx;
		
		$cur_len = $#{$dict[$_]} + 1;
		$first_idx = $_;
	}
}
#the longest words data
$len_info{$cur_len}{"first"} = $first_idx;
$len_info{$cur_len}{"count"} = $#dict + 1 - $first_idx;


my $file_pre_fix = "_word";
my $file_fix = ".src";

foreach my $w_len (sort {$a <=> $b} keys %len_info) {
	print "$w_len, from: $len_info{$w_len}{'first'}, count: $len_info{$w_len}{'count'}\n";

	my $src_filename = $file_pre_fix . $w_len . $file_fix;
	open(SRC_FILE, ">", $src_filename) or die "$src_filename: $!";
	
	my $dict_size = $len_info{$w_len}{"count"};
	
	if ($mini_dict_size[$w_len-2] > 0 and $dict_size > $mini_dict_size[$w_len-2]) {
		$dict_size = $mini_dict_size[$w_len-2];
		print "... fix count to $mini_dict_size[$w_len-2]\n";
	}

	foreach my $word_i ($len_info{$w_len}{"first"} .. ($len_info{$w_len}{'first'} + $dict_size - 1)) {
		my $word_data = "{";

		$word_data .= sprintf "0x%04x, ", $_ foreach (@{$dict[$word_i]});

		chop $word_data; # the " "
		chop $word_data; # the ","
		
		$word_data .= "},\n";
		
		print SRC_FILE $word_data;
	}

	close(SRC_FILE);
}

__END__
1 读入词库 word_list_all.txt
2 将词库按照unicode字符串格式输出，不同字数的词输出到不同的文件。
3 生成文件：_word[n].src
4 该文件运行的参数 n 表示2、3、4字词汇的数量为 (3000, 2000, 2000) 的 n 倍。因为
词库的大小是一个很重要的概念，并不是越大越好，词库过大，不仅仅会影响速度，而且还
会影响用户选词。



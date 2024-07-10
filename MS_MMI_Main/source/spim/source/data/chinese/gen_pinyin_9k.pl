#c:\perl\bin -w
use strict;

my $pinyin_26k_filename = "_pinyin.src";

open(SRC_FILE, "<", $pinyin_26k_filename) or die "$pinyin_26k_filename: $!";

undef $/;
my $all_pinyin_string = <SRC_FILE>;

close(SRC_FILE);

my %k9_map = (
	"a" => "2",
	"b" => "2",
	"c" => "2",
	"d" => "3",
	"e" => "3",
	"f" => "3",
	"g" => "4",
	"h" => "4",
	"i" => "4",
	"j" => "5",
	"k" => "5",
	"l" => "5",
	"m" => "6",
	"n" => "6",
	"o" => "6",
	"p" => "7",
	"q" => "7",
	"r" => "7",
	"s" => "7",
	"t" => "8",
	"u" => "8",
	"v" => "8",
	"w" => "9",
	"x" => "9",
	"y" => "9",
	"z" => "9",
);

$all_pinyin_string =~ s/$_/$k9_map{$_}/g foreach (keys %k9_map);

my $src_filename = "_pinyin_9k.src";
open(SRC_FILE, ">", $src_filename) or die "$src_filename: $!";

print SRC_FILE $all_pinyin_string;

close(SRC_FILE);


__END__
1 生成数字表示的拼音串
2 该文件的输入文件是 26key 的数据文件，将其拼音字母用数字替换即可 
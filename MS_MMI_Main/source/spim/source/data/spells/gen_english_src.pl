#!c:\perl\bin -w
use strict;
use gen_spell_dict qw(gen_src);

#内建规则：hi alpha 的第一个必须是零
my @hi_alpha = (0, 0, 0, 0, 0, ord("A")..ord("Z"));
my @lo_alpha = (ord(" "), ord("'"), ord("-"), ord("."), ord("/"), ord("a")..ord("z"));
my %key_map = (
	"a" => 2,
	"b" => 2,
	"c" => 2,
	"d" => 3,
	"e" => 3,
	"f" => 3,
	"g" => 4,
	"h" => 4,
	"i" => 4,
	"j" => 5,
	"k" => 5,
	"l" => 5,
	"m" => 6,
	"n" => 6,
	"o" => 6,
	"p" => 7,
	"q" => 7,
	"r" => 7,
	"s" => 7,
	"t" => 8,
	"u" => 8,
	"v" => 8,
	"w" => 9,
	"x" => 9,
	"y" => 9,
	"z" => 9,
);

die "usage: gen_dict file1 file2 file3 ..." if (@ARGV == 0);

print "Generate English source data...\n";

gen_src(\@hi_alpha, \@lo_alpha, \%key_map, \@ARGV, "english");

__END__
1 调用通用函数生成英文的字母表、字典表、字母键盘映射表
2 需要给定的数据：字符集、键盘映射表




package gen_ch_data;

use strict;
use Encode;

our (@ISA, $VERSION, @EXPORT_OK);

use Exporter;
@ISA = qw(Exporter);
$VERSION = 1.00;

@EXPORT_OK = qw(%freq_data is_gb13000 is_gb2312 is_chs is_cht utf8_uni);

# our var
our %freq_data;


################################################################################
#
# 字符集
#
my $gb2312_filename = "gb2312.txt";
my $chs_vs_cht_filename = "chs_vs_cht.txt";
my $frequence_filename = "use_frequence.txt";

#从文件获取字符集数据
my %chs_list;
my %cht_list;
my %gb2312_list;

open(GB_FILE, "<", $gb2312_filename) or die "$gb2312_filename: $!";
while (<GB_FILE>) {
	$gb2312_list{hex $1} = 1 if m/^\{0x([\da-fA-F]{4}), 0x([\da-fA-F]{4})\},$/;
}
close(GB_FILE);

open(CH_FILE, "<", $chs_vs_cht_filename) or die "$chs_vs_cht_filename: $!";
while (<CH_FILE>) {
	if (m/^\{0x([\da-fA-F]{4}), 0x([\da-fA-F]{4})\},$/) {
		$chs_list{hex $2} = hex $1;
		$cht_list{hex $1} = hex $2;
	}
}
close(CH_FILE);

#字符集判定函数
sub is_gb13000 {
	defined $_[0] and 0x4e00 <= $_[0] and $_[0] <= 0x9fa5
}

sub is_gb2312 {
	defined $gb2312_list{$_[0]}
}

sub is_chs {
	not is_gb13000($chs_list{$_[0]})
}

sub is_cht {
	not is_gb13000($cht_list{$_[0]})
}


#字符编码转换，从utf8到unicode
sub utf8_uni{
	my ($utf) = @_;
	Encode::_utf8_off($utf);
	my @utfs  = unpack("A1" x length($utf), $utf);
	my $uni = 0x4e00;
	
	if ($#utfs == 0) {
		$uni = ord($utf);
	}
	elsif ($#utfs == 1) {
		$uni = (ord($utfs[0]) & 0x3f) << 12;
		$uni += ord($utfs[1]) & 0x3f;
	}
	elsif ($#utfs == 2) {
		$uni = (ord($utfs[0]) & 0x1f) << 12;
		$uni += (ord($utfs[1]) & 0x3f) << 6;
		$uni += ord($utfs[2]) & 0x3f;
	}
	elsif ($#utfs == 3) {
		$uni = (ord($utfs[0]) & 0x0f) << 18;
		$uni += (ord($utfs[1]) & 0x3f) << 12;
		$uni += (ord($utfs[2]) & 0x3f) << 6;
		$uni += ord($utfs[3]) & 0x3f;
	}
	
	return $uni;
}


#
#从使用频率生成全汉字使用频率排序
#
my $buf;
my $freq_cnt = 0;

open(FREQ_FILE, "< :raw", $frequence_filename) or die "$frequence_filename: $!";
die "Not valide File: $frequence_filename." unless (read(FREQ_FILE, $buf, 2) == 2);
die "Not valide unicode File: $frequence_filename." unless (unpack("S", $buf) == 0xfeff);

while(read(FREQ_FILE, $buf, 4096)){
	my @uni_codes = unpack("S*", $buf);
	
	foreach my $uni (@uni_codes){
		if (is_gb13000($uni)) {
			$freq_data{$uni} = (0x9fa5 - 0x4e00 + 1) - $freq_cnt;
			$freq_cnt++;
		}
	}	
}

close(FREQ_FILE);

#生成全部的频率数据，即将未统计到的字符按0处理
foreach (0x4e00..0x9fa5) {
	$freq_data{$_} = 0 unless ($freq_data{$_} && $freq_data{$_} > 0);
}

#将简繁体的频率取最大值（虽然简繁不是一一对应的，但是可以忽略不计）
foreach (0x4e00..0x9fa5) {
	if (is_cht($_) and is_gb13000($chs_list{$_})) {
		$freq_data{$_} = $freq_data{$chs_list{$_}} if ($freq_data{$_} < $freq_data{$chs_list{$_}});
		$freq_data{$chs_list{$_}} = $freq_data{$_};
	}
}


1;
__END__
汉字数据
1 字符集
2 字符使用频率
3 该脚本是一些处理汉字的脚本的通用函数和数据。
#c:\perl\bin -w
use strict;

use gen_ch_data qw (%freq_data is_gb13000 is_gb2312 is_chs is_cht);

my $ucs_py_filename = "ucs_pinyin_spim.txt";

##-------------
print "Get source data...\n";


#从拼音数据文件获取拼音数据
sub generate_charset {
	my ($data_filename) = @_;
	my $buf;
	my %chrset_by_py;
	my %py_of_chr;
	
	#generate %chrset_by_py
	#open file and check file type
	open(IN_FILE, "< :raw", $data_filename)
		or die "$data_filename: $!";
	
	die "Not valide File: $data_filename."
		unless (read(IN_FILE, $buf, 2) == 2);
	die "Not valide unicode File: $data_filename."
		unless (unpack("S", $buf) == 0xfeff);
	
	my $getting_state = "wait_pinyin";
	my $pinyin;
	my @char_set;
	
	while(read(IN_FILE, $buf, 4096)){
		my @uni_codes = unpack("S*", $buf);
		
		foreach my $uni (@uni_codes){
			if (ord("a") <= $uni and $uni <= ord("z")) {
				#声母
				if ($getting_state eq "wait_char") {
					$chrset_by_py{$pinyin} = [];
					$getting_state = "wait_pinyin";
				}
				
				if ($getting_state eq "wait_pinyin") {
					$getting_state = "pinyin";
					$pinyin = "";
				}
				else {
					die "Error: $getting_state" unless ($getting_state eq "pinyin")
				}
				$pinyin .= chr($uni);
			} 
			elsif (is_gb13000($uni)) {
				if ($getting_state eq "wait_char") {
					$getting_state = "char";
					@char_set = ();
				}
				else {
					die "Error" unless ($getting_state eq "char");
				}
				push @char_set, $uni;
			}
			else
			{
				if ($getting_state eq "char") {
					$chrset_by_py{$pinyin} = [@char_set];
					$getting_state = "wait_pinyin";
				}
				elsif ($getting_state eq "pinyin") {
					$getting_state = "wait_char";
				}
			}
		}	
	}
	
	close(IN_FILE);
	
	#generate %py_of_chr
	foreach $pinyin (sort {$a cmp $b} keys %chrset_by_py) {
		@char_set = @{$chrset_by_py{$pinyin}};
		foreach (@char_set) {
			$py_of_chr{$_}{"count"} += 1;
			push @{$py_of_chr{$_}{"pinyin"}}, $pinyin;
		}
	}
	
	return (\%chrset_by_py, \%py_of_chr);
}

##-------------
#生成拼音数据
my ($ucs_chrset_by_py__h, $ucs_py_of_chr__h) = generate_charset($ucs_py_filename);

my %pinyin_of_char;
my @pinyin_set;
my %pinyin_index;

%pinyin_of_char = %{$ucs_py_of_chr__h};

#generate @pinyin_set
# 将拼音排序，此后生成的拼音数据即按照此索引来
# 先按字母排序，再按长短排序。两次排序会被 perl 保留。
@pinyin_set =  sort {$a cmp $b} keys %{$ucs_chrset_by_py__h};
@pinyin_set =  sort {length($b) <=> length($a)} @pinyin_set;

#generate %pinyin_index
foreach my $i (0 .. $#pinyin_set) {
	$pinyin_index{$pinyin_set[$i]} = $i;
}

################################################################################
#Generate character data
print "Generate character data...\n";

################################################################################
#generate character data

#统计每个拼音在不同字符集的字符个数
my @chr_cnt_by_py;

foreach my $uni (0x4e00..0x9fa5) {
	unless ($pinyin_of_char{$uni}{"count"}) {
		printf "lost 0x%04x\n", $uni;
		die "Please check your source data...";
	}
	
	foreach (@{$pinyin_of_char{$uni}{"pinyin"}}) {
		$chr_cnt_by_py[$pinyin_index{$_}]{"gb13000"} += 1; #always
		$chr_cnt_by_py[$pinyin_index{$_}]{"gb2312"} += 1 if is_gb2312($uni);
		$chr_cnt_by_py[$pinyin_index{$_}]{"chs"} += 1 if is_chs($uni);
		$chr_cnt_by_py[$pinyin_index{$_}]{"cht"} += 1 if is_cht($uni);
	}
}

#生成字符集，按使用频率排序
my @chrset_gb13000;
my @chrset_gb2312;
my @chrset_chs;
my @chrset_cht;

foreach (0x4e00..0x9fa5) {
	push @chrset_gb2312, $_ if is_gb2312($_);
	push @chrset_chs, $_ if is_chs($_);
	push @chrset_cht, $_ if is_cht($_);
}

@chrset_gb13000 = sort {$freq_data{$b} <=> $freq_data{$a}} (0x4e00..0x9fa5);
@chrset_gb2312 = sort {$freq_data{$b} <=> $freq_data{$a}} @chrset_gb2312;
@chrset_chs = sort {$freq_data{$b} <=> $freq_data{$a}} @chrset_chs;
@chrset_cht = sort {$freq_data{$b} <=> $freq_data{$a}} @chrset_cht;

printf "gb13000: %d, gb2312: %d, chs: %d, cht: %d\n", $#chrset_gb13000, $#chrset_gb2312, $#chrset_chs, $#chrset_cht; 

#生成拼音字符串
my $pinyin_set_str;
foreach my $i (0 .. $#pinyin_set) {
	$pinyin_set_str .= "\"$pinyin_set[$i]\",";
	$pinyin_set_str .= ((($i+1) % 8 == 0) ? "\n" : " " x (7 - length($pinyin_set[$i]))); 
}

#生成读音字符串
my $chr_tab_str;
my @chr_tab_mp_str; #2,3,4,5
my @tab_n_index = (0, 0, 0, 0);
my @index_per_line = (4, 3, 2, 1);
my @chr_index_flag = (0x0000, 0x2000, 0x4000, 0x6000, 0x8000); #1,2,3,4,5

foreach my $uni (0x4e00..0x9fa5) {
	my $cnt = $pinyin_of_char{$uni}{"count"};
	my $tab_index;
	
	if ($cnt == 1) {
		$tab_index = $pinyin_index{$pinyin_of_char{$uni}{"pinyin"}[0]};
	}
	elsif (2 <= $cnt and $cnt <= 5) {
		$tab_index = $tab_n_index[$cnt-2];
		$tab_n_index[$cnt-2]++;
		
		my $pinyin_list;
		$pinyin_list .= sprintf "0x%04x, ", $pinyin_index{$_} foreach (@{$pinyin_of_char{$uni}{"pinyin"}});
		chop $pinyin_list foreach (0..1); #chop the tailer ", "
		
		$chr_tab_mp_str[$cnt-2] .= "{$pinyin_list},";
		$chr_tab_mp_str[$cnt-2] .= (($tab_n_index[$cnt-2] % $index_per_line[$cnt-2] == 0) ? "\n" : " "); 
	}
	else {
		printf "0x%04x, %d", $uni, $cnt;
		die "Error count of the character spells.";
	}
	
	$chr_tab_str .= sprintf "0x%04x,", ($chr_index_flag[$cnt-1] | $tab_index);
	$chr_tab_str .= ((($uni - 0x4e00 + 1) % 10 == 0) ? "\n" : " "); 
}

printf "Pinyin data... p2: %d, p3: %d, p4: %d, p5: %d\n", @tab_n_index;



################################################################################
#Output to source File
print "Make source file...\n";
my %chrset = (
	"gb13000" => [@chrset_gb13000],
	"gb2312" => [@chrset_gb2312],
	"chs" => [@chrset_chs],
	"cht" => [@chrset_cht]
);

#输出字符集
foreach my $cs_name (keys %chrset) {
	my $src_filename = "_chrset_". $cs_name . ".src";
	open(SRC_FILE, ">", $src_filename) or die "$src_filename: $!";
	foreach my $i (0 .. $#{$chrset{$cs_name}}) {
		printf SRC_FILE "0x%04x,", $chrset{$cs_name}->[$i];
		print SRC_FILE (($i+1) % 10 == 0) ? "\n" : " ";
	}
	close(SRC_FILE);
}

#输出字符集读音统计
foreach my $cs_name (keys %chrset) {
	my $src_filename = "_py_cnt_". $cs_name . ".src";
	open(SRC_FILE, ">", $src_filename) or die "$src_filename: $!";
	foreach my $i (0..$#pinyin_set) {
		printf SRC_FILE "0x%04x,", $chr_cnt_by_py[$i]{$cs_name};
		print SRC_FILE (($i+1) % 8 == 0) ? "\n" : " ";
	}
	close(SRC_FILE);
}

#输出读音列表
my %src_file = (
	"pinyin" => $pinyin_set_str,
	"chr_py_idx" => $chr_tab_str,
	"chr_py_mp2" => @chr_tab_mp_str[0],
	"chr_py_mp3" => @chr_tab_mp_str[1],
	"chr_py_mp4" => @chr_tab_mp_str[2],
	"chr_py_mp5" => @chr_tab_mp_str[3],
);
foreach (keys %src_file) {
	my $src_filename = "_". $_ . ".src";
	open(SRC_FILE, ">", $src_filename) or die "$src_filename: $!";
	print SRC_FILE $src_file{$_} . "\n";
	close(SRC_FILE);
}

__END__
0 拼音和字符集是关联的，因为要统计字符集和拼音的有效个数。所以，一并处理。

1 所有合法的拼音组合列表
    目前从网络上得到的资料来看，很多的汉字的读音是无法正常发音的，也就是说，因为
    生僻，或者从来没有人用，导致有这个字，但是没有这个音。所以，对于这些奇怪的发
    音，为了避免对用户使用习惯的影响，考虑去除这些读音，而将这些字放在其他的部
    分，比如【难读字】索引等。
    既定方案：因为v下面是没有读音的，所以，单独的v下面可以放置所有的【难读字】。
    难读字的定义：所有的非单音节可读的汉字，即非简单的声母+夹母+韵母的形式。
    因为搜索的需要，将所有的单个声母也列在数据中，下面的字符列表空，也就是说，将
    声母也算入合法的拼音组合，方面快速输入。
    这样一来，我们就需要对数据进行认为的编辑，主要是以下两个方面：
      1 将【难读字】都归到v字下
      2 将所有的声母插入到数据序列中
    此列表按26个字母的顺序排列。

2 所有汉字的读音
    用拼音组合列表里的合法组合索引来表示汉字的读音。
    多音字方案。对于多音字，建立另外的索引，在读音的索引中，放置多音字索引位置。
    考虑到合法的拼音组合有五百多个，用10bit足以表示。两个音的读音字，三千多个，
    用13bit足以表示，其他三音、四音及以上，则就更少了。用一个16bit来做汉字读音的
    索引，前3bit表示读音的数目，这样一来，一个16bit读音索引就是xxxyyyyyyyyyyyyyb
    的形式，xxxb就是读音的数目减一，如果xxxb是零，则本索引的后13bit，即
    yyyyyyyyyyyyyb就表示拼音的索引，否则，就表示在相应的多音表中的索引。这样，我
    们就会另外建议2n的数组表示两个读音的字的读音，表中存储的就是相应的拼音索引。
    同样，会有3n、4n等等。目前读音最多的是5个读音。考虑到常用字的读音等因素，如
    果用两个表交叉的话，则读音最多的也就4个读音。
    目前我们弃用了建立词索引的做法，则可以本着保持数据完整性的做法，保留所有字的
    所有读音。
    此列表按照unicode值排序。

3 字符集
    简繁对照表来自wiki百科，出自ubuntu系统的简繁对照系统
    1 gb13000
    2 gb2312
    3 chs - 没有简体字的就是简体字
    4 cht - 没有繁体字的就是繁体字
    
4 生成数据文件：
	_pinyin.src - 合法的拼音组合
	_chr_py_idx.src - 读音索引
	_chr_py_mp2.src - 二音字
	_chr_py_mp3.src - 三音字
	_chr_py_mp4.src - 四音字
	_chr_py_mp5.src - 五音字
	_chrset_chs.src - 字符集，简体中文
	_chrset_cht.src - 繁体中文
	_chrset_gb13000.src - gb13000
	_chrset_gb2312.src - gb2312
	_py_cnt_chs.src - 拼音内字符数，简体中文
	_py_cnt_cht.src - 繁体中文
	_py_cnt_gb13000.src -gb13000
	_py_cnt_gb2312.src - gb2312
	


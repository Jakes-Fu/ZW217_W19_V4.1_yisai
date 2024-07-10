#c:\perl\bin -w
use strict;

use gen_ch_data qw (%freq_data is_gb13000 is_gb2312 is_chs is_cht);

my $ucs_py_filename = "ucs_pinyin_spim.txt";

##-------------
print "Get source data...\n";


#��ƴ�������ļ���ȡƴ������
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
				#��ĸ
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
#����ƴ������
my ($ucs_chrset_by_py__h, $ucs_py_of_chr__h) = generate_charset($ucs_py_filename);

my %pinyin_of_char;
my @pinyin_set;
my %pinyin_index;

%pinyin_of_char = %{$ucs_py_of_chr__h};

#generate @pinyin_set
# ��ƴ�����򣬴˺����ɵ�ƴ�����ݼ����մ�������
# �Ȱ���ĸ�����ٰ�����������������ᱻ perl ������
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

#ͳ��ÿ��ƴ���ڲ�ͬ�ַ������ַ�����
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

#�����ַ�������ʹ��Ƶ������
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

#����ƴ���ַ���
my $pinyin_set_str;
foreach my $i (0 .. $#pinyin_set) {
	$pinyin_set_str .= "\"$pinyin_set[$i]\",";
	$pinyin_set_str .= ((($i+1) % 8 == 0) ? "\n" : " " x (7 - length($pinyin_set[$i]))); 
}

#���ɶ����ַ���
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

#����ַ���
foreach my $cs_name (keys %chrset) {
	my $src_filename = "_chrset_". $cs_name . ".src";
	open(SRC_FILE, ">", $src_filename) or die "$src_filename: $!";
	foreach my $i (0 .. $#{$chrset{$cs_name}}) {
		printf SRC_FILE "0x%04x,", $chrset{$cs_name}->[$i];
		print SRC_FILE (($i+1) % 10 == 0) ? "\n" : " ";
	}
	close(SRC_FILE);
}

#����ַ�������ͳ��
foreach my $cs_name (keys %chrset) {
	my $src_filename = "_py_cnt_". $cs_name . ".src";
	open(SRC_FILE, ">", $src_filename) or die "$src_filename: $!";
	foreach my $i (0..$#pinyin_set) {
		printf SRC_FILE "0x%04x,", $chr_cnt_by_py[$i]{$cs_name};
		print SRC_FILE (($i+1) % 8 == 0) ? "\n" : " ";
	}
	close(SRC_FILE);
}

#��������б�
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
0 ƴ�����ַ����ǹ����ģ���ΪҪͳ���ַ�����ƴ������Ч���������ԣ�һ������

1 ���кϷ���ƴ������б�
    Ŀǰ�������ϵõ��������������ܶ�ĺ��ֵĶ������޷����������ģ�Ҳ����˵����Ϊ
    ��Ƨ�����ߴ���û�����ã�����������֣�����û������������ԣ�������Щ��ֵķ�
    ����Ϊ�˱�����û�ʹ��ϰ�ߵ�Ӱ�죬����ȥ����Щ������������Щ�ַ��������Ĳ�
    �֣����硾�Ѷ��֡������ȡ�
    �ȶ���������Ϊv������û�ж����ģ����ԣ�������v������Է������еġ��Ѷ��֡���
    �Ѷ��ֵĶ��壺���еķǵ����ڿɶ��ĺ��֣����Ǽ򵥵���ĸ+��ĸ+��ĸ����ʽ��
    ��Ϊ��������Ҫ�������еĵ�����ĸҲ���������У�������ַ��б�գ�Ҳ����˵����
    ��ĸҲ����Ϸ���ƴ����ϣ�����������롣
    ����һ�������Ǿ���Ҫ�����ݽ�����Ϊ�ı༭����Ҫ�������������棺
      1 �����Ѷ��֡����鵽v����
      2 �����е���ĸ���뵽����������
    ���б�26����ĸ��˳�����С�

2 ���к��ֵĶ���
    ��ƴ������б���ĺϷ������������ʾ���ֵĶ�����
    �����ַ��������ڶ����֣�����������������ڶ����������У����ö���������λ�á�
    ���ǵ��Ϸ���ƴ���������ٶ������10bit���Ա�ʾ���������Ķ����֣���ǧ�����
    ��13bit���Ա�ʾ���������������������ϣ���͸����ˡ���һ��16bit�������ֶ�����
    ������ǰ3bit��ʾ��������Ŀ������һ����һ��16bit������������xxxyyyyyyyyyyyyyb
    ����ʽ��xxxb���Ƕ�������Ŀ��һ�����xxxb���㣬�������ĺ�13bit����
    yyyyyyyyyyyyyb�ͱ�ʾƴ�������������򣬾ͱ�ʾ����Ӧ�Ķ������е���������������
    �Ǿͻ����⽨��2n�������ʾ�����������ֵĶ��������д洢�ľ�����Ӧ��ƴ��������
    ͬ��������3n��4n�ȵȡ�Ŀǰ����������5�����������ǵ������ֵĶ��������أ���
    ������������Ļ������������Ҳ��4��������
    Ŀǰ���������˽���������������������Ա��ű������������Ե����������������ֵ�
    ���ж�����
    ���б���unicodeֵ����

3 �ַ���
    �򷱶��ձ�����wiki�ٿƣ�����ubuntuϵͳ�ļ򷱶���ϵͳ
    1 gb13000
    2 gb2312
    3 chs - û�м����ֵľ��Ǽ�����
    4 cht - û�з����ֵľ��Ƿ�����
    
4 ���������ļ���
	_pinyin.src - �Ϸ���ƴ�����
	_chr_py_idx.src - ��������
	_chr_py_mp2.src - ������
	_chr_py_mp3.src - ������
	_chr_py_mp4.src - ������
	_chr_py_mp5.src - ������
	_chrset_chs.src - �ַ�������������
	_chrset_cht.src - ��������
	_chrset_gb13000.src - gb13000
	_chrset_gb2312.src - gb2312
	_py_cnt_chs.src - ƴ�����ַ�������������
	_py_cnt_cht.src - ��������
	_py_cnt_gb13000.src -gb13000
	_py_cnt_gb2312.src - gb2312
	


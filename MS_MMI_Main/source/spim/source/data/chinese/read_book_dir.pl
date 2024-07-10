#!c:\perl\bin -w
use strict;
use Encode;
use File::Find;

my $MAX_ASSO_COUNT = 64;

die("useage: exec dir_want_to_read\n") if (@ARGV < 1);

my %freq_data;
my %next_data;

sub read_file {
	my $filename;
	my $file_type;
	my $buf;
	my $read_len;
	my @uni_codes;
	my $uni;
	my $last_ch = 0;
	my $proc = 0;
	
	if (! -f) {
		print "$_: Not a File.\n";
		return;
	}

	$filename = $_;

	print "Reading: $filename ";
	
	if (!open(IN_FILE, "< :raw", $filename)) {
		print "$filename: $!\n";
		return;
	}
	
	$read_len = read(IN_FILE, $buf, 2);
	if ($read_len == 2) {
		$file_type = unpack("S", $buf);
	}
	else {
		$file_type = 0;
	}
	
	if ($file_type != 0xfeff) {
		print "Not valide unicode File: $filename.\n";
		return;
	}
	
	while ($read_len = read(IN_FILE, $buf, 4096)) {
		print "." if($proc++ % 25 == 1); #100k

		@uni_codes = unpack("S*", $buf);
		
		foreach $uni (@uni_codes) {
			if (0x4e00 <= $uni and $uni <= 0x9fa5) {
				$freq_data{$uni} += 1;
				$next_data{$last_ch}{$uni} += 1 if (0x4e00 <= $last_ch and $last_ch <= 0x9fa5);
			}	
			$last_ch = $uni;
		}	
	}
	
	close(IN_FILE);
	print "#\n";
}

sub output_data {
	my $file_fix = "";
	my $freq_filename;
	my $next_filename;
	my $freq_data_filename;
	my $next_data_filename;
	my @freq_list;
	my $asso_count = 0;
	my $cur_ch;
	my $next_ch;
	my $ch;
	my @asso_char = ();
	my $proc = 0;
	
	print "Processing data, please wait...\n";
	
	#refrence datas
	$file_fix = $_[0] if $_[0];
	
	$freq_filename = $file_fix . "read_result_freq.dat";
	open(FREQ_FILE, ">", $freq_filename) or die "$freq_filename: $!\n";
	
	$next_filename = $file_fix . "read_result_next.dat";
	open(NEXT_FILE, ">", $next_filename) or die "$next_filename: $!\n";
	
	$freq_data_filename = $file_fix . "read_data_freq.dat";
	open(FREQ_DATA_FILE, ">", $freq_data_filename) or die "$freq_data_filename: $!\n";
	
	$next_data_filename = $file_fix . "read_data_next.dat";
	open(NEXT_DATA_FILE, ">", $next_data_filename) or die "$next_data_filename: $!\n";
	
	$proc = 0;
	
	@freq_list = sort {$freq_data{$b} <=> $freq_data{$a}} keys %freq_data;

	foreach $cur_ch (@freq_list) {
		print "." if($proc++ % 100 == 1); #100 ch
		
		push @asso_char, $cur_ch if ($proc <= $MAX_ASSO_COUNT);
		
		$ch = encode("utf8", pack("U",$cur_ch));
		print FREQ_FILE "$ch: $freq_data{$cur_ch}\n";
		print NEXT_FILE "$ch: $freq_data{$cur_ch}\n";
		print FREQ_DATA_FILE $ch;
		print NEXT_DATA_FILE "$ch: ";
		
		my $asso_cnt = 100; #只要100个

		foreach $next_ch (sort {$next_data{$cur_ch}{$b} <=> $next_data{$cur_ch}{$a}} keys %{$next_data{$cur_ch}}) {
			$ch = encode("utf8", pack("U",$next_ch));
			print NEXT_FILE "	$ch: $next_data{$cur_ch}{$next_ch}\n";
			
			if ($asso_cnt > 0) {
				print NEXT_DATA_FILE $ch;
				$asso_cnt--;
			}
			
		}
		print NEXT_DATA_FILE "\n";
	}
	
	close(FREQ_FILE);
	close(NEXT_FILE);
	close(FREQ_DATA_FILE);
	close(NEXT_DATA_FILE);
}

$| = 1;

find(\&read_file, @ARGV);

&output_data($ARGV[0] . "_");

print "\nMission Complete!\n";

__END__
用程序扫描制定目录下的 unicode text 文件，统计字词的出现频率
输出两类文件：
1 单字的使用统计 freq
2 某字后的字出现统计 next
输出两类数据
1 带统计数字的 result
2 只有统计排序的 data

_DIR_read_result_freq.dat
_DIR_read_result_next.dat
_DIR_read_data_freq.dat
_DIR_read_data_next.dat

该脚本所得到的联想字数据太大，词的特性不是很明显，所以另写一个脚本，从词库获取联
想字。
所以，该脚本生成的数据仅有 _DIR_read_data_freq.dat 是有效的。
该脚本需要的数据文件必须是大量的 unicode 文档，所以，此脚本几乎不会用到。此处仅
仅是备份。

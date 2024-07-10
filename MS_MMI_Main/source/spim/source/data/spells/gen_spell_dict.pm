package gen_spell_dict;

use strict;
use Encode;

our (@ISA, $VERSION, @EXPORT_OK);

use Exporter;
$VERSION = 1.00;
@ISA = qw(Exporter);

@EXPORT_OK = qw(gen_src);

#�ַ�����ת������utf8��unicode
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


################################################################################
my $UNICODE_FLAG = 0xfeff;

#������index 0 �������գ����������ṩ����������

# 1 ���ɵĹ��̱�֤�϶��Ƿǿյ�����
# 2 ���Դ�Сд˳��
sub sort_word {
	my $i = 0;

	while ($a->[$i]>>1 == $b->[$i]>>1) {
		$i++;
		return -1 if ($i == @$a);
		return 1 if ($i == @$b);
	};
	
	return $a->[$i] <=> $b->[$i];
}


#����ȫ������
sub gen_src {
	my ($hi_alpha, $lo_alpha, $key_map, $files, $lang_tag) = @_;
	my %alpha_index;
	my $line_flag = 0;
	
	die "Wrong alphabet define." unless (@$lo_alpha == @$hi_alpha);
	die "usage: gen_dict file1 file2 file3 ..." if (@$files == 0);
	
	#generate alpha index
	#��0������1������Ϊ��ĸ����
	#��Ϊ��0����ʾ�ַ�������������1����Ӧ��0����Сд
	foreach (0 .. @$lo_alpha - 1){
		$alpha_index{$hi_alpha->[$_]} = 2*($_ + 1)
			unless ($hi_alpha->[$_] == 0);
		$alpha_index{$lo_alpha->[$_]} = 2*($_ + 1) + 1
			unless ($lo_alpha->[$_] == 0);
	}
	
	
	########################################################################
	#alphabet
	my $alphabet_file = "_" . $lang_tag . "_alphabet.src";
	open(ALPHABET, ">", $alphabet_file) or die "$alphabet_file: $!";
	
	#����д����õ���������
	print ALPHABET "0x0000, 0x0000, ";
	$line_flag = 1;
	
	foreach (0 .. @$lo_alpha - 1){
		printf ALPHABET "0x%04x, 0x%04x,", $hi_alpha->[$_], $lo_alpha->[$_];
		if ($line_flag > 3) {
			$line_flag = 0;
			printf ALPHABET "\n";
		} else {
			$line_flag += 1;
			printf ALPHABET " ";
		}
	}
	
	close(ALPHABET);
	
	########################################################################
	#keymap
	my $key_map_file = "_" . $lang_tag . "_key_map.src";
	open(KEY_MAP, ">", $key_map_file) or die "$key_map_file: $!";
	
	#����д����õ���������, ������������õ��ʾ
	print KEY_MAP "\"..";

	foreach (0 .. @$lo_alpha - 1){
		my $map_chr = $key_map->{chr($lo_alpha->[$_])};
		printf KEY_MAP defined($map_chr) ? $map_chr : ".";
		printf KEY_MAP defined($map_chr) ? $map_chr : ".";
	}

	print KEY_MAP "\"";
	
	close(KEY_MAP);
	
	########################################################################
	#vocabulary
	#1 ��Сд��ĸд��ʻ��
	#2 �������еĵ���
	my $vocabulary_file = "_" . $lang_tag . "_vocabulary.src";
	open(VOCABULARY, ">", $vocabulary_file) or die "$alphabet_file: $!";

	#1 ��Сд��ĸд��ʻ��
	my @vocabulary_alpha = ();

	foreach (@$lo_alpha){
		next if (0 == $_);
		my @word = ();
		push @word, $alpha_index{$_};
		push @vocabulary_alpha, [@word];
	}

	foreach (sort sort_word @vocabulary_alpha) {
		print VOCABULARY "\"";
		printf VOCABULARY "\\x%02x", $_ foreach (@{$_});
		print VOCABULARY "\",\n";
	}
	
	my $all_alpha = "";

	foreach (0 .. @$lo_alpha - 1){
		$all_alpha .= sprintf("\\x{%04x}", $hi_alpha->[$_]) if ($hi_alpha->[$_] > 0);
		$all_alpha .= sprintf("\\x{%04x}", $lo_alpha->[$_]) if ($lo_alpha->[$_] > 0);
	}
	
	#2 �������еĵ���
	#����Ψһ��ʶ
	my %word_exclusive = ();
	
	foreach (@{$files}) {
		my $filename = $_;
		my @vocabulary = ();
		
		print "Reading $filename...\n";

		open(WORD_FILE, "<:utf8", $filename) or die "$filename: $!";
		
		while (<WORD_FILE>) {
			if (/^([$all_alpha]+)$/) {
				#print $1 . "\n";
				my $word = $1;
				my @word_data = ();
				my $word_len = length($word);
				#ȥ������Ϊ1�ĵ��ʣ���Ϊ����ĸ�Ѿ�����
				next if ($word_len == 1);
				next if ($word_exclusive{$word});				
				$word_exclusive{$word} = 1;
				
				foreach my $i (0 .. $word_len - 1) {
					my $ch = substr($word, $i, 1);
					my $uni = utf8_uni($ch);
					push @word_data, $alpha_index{$uni};
				}
				push @vocabulary, [@word_data];
			}
		}

		close(WORD_FILE);

		foreach (sort sort_word @vocabulary) {
			print VOCABULARY "\"";
			printf VOCABULARY "\\x%02x", $_ foreach (@{$_});
			print VOCABULARY "\",\n";
		}
	}
		
	close(VOCABULARY);
}


1;
__END__
�����ֵ��ļ�
0 word file������UTF-8��ʽ��
1 ��ĸ����������д����ĸ������Сд1��д1Сд2��д2����
2 �ʿ��б���������ʾ���ʣ���Ϊ��������С��0xff�ģ�������һλ��ʾһ����ĸ
  - �˴��ɼ���
3 �ڽ�����hi alpha �ĵ�һ���������㣬�������ɴ����Ĵ���
4 �����������ļ�����ĸ���ʻ���ַ�����ӳ��
5 ������򣬽���ĸ���е�Сд��ĸ�ŵ��ʻ��ͷ
6 ���ӵ���Ψһ��ʶ��ȥ�������ʻ�����п�����ͬ�ĵ��ʡ���Сд��ͬ���㲻ͬ��
  

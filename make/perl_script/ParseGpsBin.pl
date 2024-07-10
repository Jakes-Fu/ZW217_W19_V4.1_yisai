#!/usr/bin/perl

$NUM_ARGV = @ARGV;

$bd_bin_name="gnssbdmodem.bin";
$bd_all_bin_file = "greeneye2_bd_all_bin.h";
$bd_file_name = "greeneye2_bd.h";
$bd_log_name = "greeneye2_bd_log.h";
$bd_head = "const unsigned char gps_bd_midi[] = {";
$bd_log_head = "const unsigned char gps_bd_log_midi[] = {";

$gnss_bin_name="gnssmodem.bin";
$glonass_all_bin_file = "greeneye2_glonass_all_bin.h";
$glonass_file_name = "greeneye2_glonass.h";
$glonass_log_name = "greeneye2_gnss_log.h";
$glonass_head = "const unsigned char gps_glonass_midi[] = {";
$glonass_log_head = "const unsigned char gps_glonass_log_midi[] = {";

$fdl_bin_name="gnssfdl.bin";
$fdl_head_file = "greeneye2_gnssfdl.h";
$gnssfdl_head = "const unsigned char gps_gnss_fdl[] = {";

@file_array=
(
	[$bd_bin_name,$bd_all_bin_file,$bd_file_name,$bd_log_name,$bd_head,$bd_log_head],
  [$gnss_bin_name,$glonass_all_bin_file,$glonass_file_name,$glonass_log_name,$glonass_head,$glonass_log_head],
);
$file_tail = "};";
$null_line="0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\n";

if($NUM_ARGV == 2)
{
	$input_path = $ARGV[0];
	$output_path = $ARGV[1];
}
else
{
	print "please input gps bin path and output file path!";
	return;
}

GenerateAllBinFile(0);
GenerateOtherFile(0);

GenerateAllBinFile(1);
GenerateOtherFile(1);
GenerateFdlBinFile();
sub GenerateOtherFile
{
	my $target_bin = $_[0];
	my $all_string;
	my @log_string;
	
	open(INPUT,"<$output_path/$file_array[$target_bin][1]") or die "can't open $file_array[$target_bin][1]\n";
	open(BIN_OUTPUT,">$output_path/$file_array[$target_bin][2]") or die "can't open $file_array[$target_bin][2]\n";

  while(<INPUT>)
	{
		$all_string=$all_string.$_;
	}

	if($all_string=~/([\s\S]*?)($null_line){64,}/)
	{
		 #print "test prel $1 and $2\n";
		 print BIN_OUTPUT "$1";
		 print BIN_OUTPUT "$file_tail";
		 print "$file_array[$target_bin][2] write done!\n";
	}

	close(INPUT);
	close(BIN_OUTPUT);
}

sub GenerateAllBinFile
{
	my $target_bin = $_[0];
	
	open(INPUT,"<$input_path/$file_array[$target_bin][0]") or die "can't open $file_array[$target_bin][0]\n";
	open(OUTPUT,">$output_path/$file_array[$target_bin][1]") or die "can't open $file_array[$target_bin][1]\n";
	binmode(INPUT);
	
	print OUTPUT "$file_array[$target_bin][4]\n";
	while(read(INPUT,$line,16))
	{
		my(@HEX) = unpack("H2H2H2H2H2H2H2H2H2H2H2H2H2H2H2H2", $line);
		my $string;
		
		foreach my $item (@HEX)
		{
			my $single=sprintf("0x%02X,",hex($item));
			$string=$string.$single;
		}
		
		print OUTPUT "$string\n";
	}
	print OUTPUT "$file_tail";
	print "$file_array[$target_bin][1] write done!\n";
	close(INPUT);
	close(OUTPUT);
}

sub GenerateFdlBinFile
{	
	open(INPUT,"<$input_path/$fdl_bin_name") or die "can't open $fdl_bin_name\n";
	open(OUTPUT,">$output_path/$fdl_head_file") or die "can't open $fdl_head_file\n";
	binmode(INPUT);
	
	print OUTPUT "$gnssfdl_head\n";
	while(read(INPUT,$line,16))
	{
		my(@HEX) = unpack("H2H2H2H2H2H2H2H2H2H2H2H2H2H2H2H2", $line);
		my $string;
		
		foreach my $item (@HEX)
		{
			my $single=sprintf("0x%02X,",hex($item));
			$string=$string.$single;
		}
		
		print OUTPUT "$string\n";
	}
	print OUTPUT "$file_tail";
	print "$fdl_head_file write done!\n";
	close(INPUT);
	close(OUTPUT);
}
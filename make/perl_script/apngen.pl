#!/usr/bin/perl

#################################################
## apngen.pl
##    apn data bin generator.
##    
## usage :
##    apngen.pl <apn file> <version> <output file>
##
## sample :
##    apngen.pl apn.ntac 12345678 apn.bin
##
#################################################

print "---------------------------------------------\r\n";
print "----------- apn data gen start  -------------\r\n";
print "---------------------------------------------\r\n";

if (scalar(@ARGV) < 3)
{
	die "[apngen.pl] param error, number of params at least is 3\n";
}

my $apn_file = ${ARGV[0]};
my $apn_version = ${ARGV[1]};
my $bin_file = ${ARGV[2]};

my $apn_file_size = (stat $apn_file)[7];
my $ver_len = length($apn_version);

if (($apn_file_size) < 1024)
{
	die "[apngen.pl] apn file size error\n";
}

if (($ver_len) != 8)
{
	die "[apngen.pl] apn version error [must 8 bit string]\n";
}

open(f_bin_file, ">$bin_file") or die "file $bin_file open error";
open(f_apn_file, "<$apn_file") or die "file $apn_file open error";

binmode(f_bin_file);
binmode(f_apn_file);

my $asize = pack('L', $apn_file_size);
my $asize_len = length($asize);

syswrite(f_bin_file, $apn_version, 8);
syswrite(f_bin_file, "00000000", 8);
syswrite(f_bin_file, $asize, $asize_len);

print "apn file version : $apn_version \n";
print "apn file size : $apn_file_size ($asize_len)\n";

while(read(f_apn_file, $buffer, 1024))
{
    syswrite(f_bin_file, $buffer, length($buffer));
}

close (f_bin_file);
close (f_apn_file);

print "---------------------------------------------\r\n";
print "--------- apn data gen finished -------------\r\n";
print "---------------------------------------------\r\n";

1;

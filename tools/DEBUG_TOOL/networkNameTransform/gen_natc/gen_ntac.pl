#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV < 2)
{
	print "Please input name : XXX.xls XXX.ntac \n";
	return;
}

if($NUM_ARGV == 2)
{
	$Name_excel = $ARGV[0];
	$Name_natc = $ARGV[1];
}

use Cwd;
my $dir = getcwd;

my @files=<$dir/ntac/*.ntac>;
unlink @files;

$dir =~s{/}{\\}g;

print ".\\Bin\\ResBOCA.exe -ntac -import $dir\\xls\\$Name_excel -export $dir\\ntac\\$Name_natc";
system(".\\Bin\\ResBOCA.exe -ntac -import $dir\\xls\\$Name_excel -export $dir\\ntac\\$Name_natc");


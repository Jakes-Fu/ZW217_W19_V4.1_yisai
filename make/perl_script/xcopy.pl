#!/usr/bin/perl

if(scalar(@ARGV)!=2)
{
	die "xcopy: the number of parameters must be 2\n";
}

my $src_path = ${ARGV[0]};
my $dst_path = ${ARGV[1]};

$src_path =~ s/\/+/\//g;
$dst_path =~ s/\/+/\//g;

my $ret = system "xcopy \"$src_path\" \"$dst_path\" /E/Q";

if($ret != 0)
{
	die "xcopy from \"$src_path\" to \"$dst_path\" failed.\n";
}

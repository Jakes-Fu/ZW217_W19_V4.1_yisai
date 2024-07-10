#!/usr/bin/perl

#########################################################################
## 
## File Name:   MacroConv.pl
## Author:      Hongliang.xin
## Description: Convert makefile marco to #define.
## Usage:       MacroConv.pl xxx.mk yyy.h
## 
## Modification History
## 2011-06-23   Create
#########################################################################


if(scalar(@ARGV)<2)
{
	die "[MacroConv.pl] param error, usage: MacroConv.pl xxx.mk yyy.h\n";
}

my $mk_path = ${ARGV[0]};
my $out_path = ${ARGV[1]};
## modify this later,it is better to use value
open(MK, "<$mk_path") or die "Can not open $mk_path" ;
open(OUT, ">$out_path") or die "Can not create and open $out_path" ;
		
while($line = <MK>)
{
	##remove space char
	$line =~ s/\s*//g;
	if($line == "" && $line =~ m/^\#/ )
	{
		next;
	}
	elsif($line =~s/([^=\#]*)=([^=\#]*)\#*.*/\#define $1_$2/g )
	{
		my $left = $1;
		my $right = $2;
		if(uc($2) ne "FALSE" && uc($2) ne "NONE" && ($left=~ m/:/) eq "" && ($right=~ m/\$/) eq "" )
		{
			if(uc($2) eq "TRUE")
			{
				print OUT "\#define $1\n";
			}
			else
			{
				print OUT "$line\n";
			}
		}	
	}
}

close OUT;
close MK;

1;
	
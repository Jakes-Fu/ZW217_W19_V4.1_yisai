#!/usr/bin/perl
BEGIN{push @INC,'./make/perl_script/perl_pm'};

$NUM_ARGV = @ARGV;

if($NUM_ARGV >= 2)
{
	for($i=0;$i<($NUM_ARGV-1);$i++)
	{
		system("copy /y $ARGV[$i] $ARGV[$NUM_ARGV-1]");
	}
}
else
{
	return 1;
}

#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV != 3)
{
	return 1;
}
else
{
	$param  = $ARGV[0];
	$myDir  = $ARGV[1];
	$myFlag = $ARGV[2];}

my $lang = "";
my $loc;

#print "$myDir $myflag\n";

opendir(DIR,$myDir) or die "can not open dir $myDir/n";
my @list = readdir(DIR);
closedir DIR;
my $f;

foreach $f (sort @list)
{
    my $f_cat;
    $f_cat = $myDir."/".$f;
    #big endian
    if($myFlag == 1) 
    {
        if($f=~ m/.*_lang_[^_]+\.bin$/)
        {
            $lang = $lang . "-lang \"$f_cat\" ";
        }
    }
    else #little endian
    {
        if($f=~ m/.*_lang_.[^_]+_win\.bin$/)
        {
            $lang = $lang . "-lang \"$f_cat\" ";
        }
    }
}

#print $lang;

system "$param $lang";


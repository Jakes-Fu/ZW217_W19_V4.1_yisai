#!/usr/bin/perl


$NUM_ARGV = @ARGV;

if($NUM_ARGV == 4)
{
	$dloader_ini  = $ARGV[0];
	$Segment  = $ARGV[1];
	$Item  = $ARGV[2];
	$value  = $ARGV[3];
}
else
{
	return 1;
}

open (inf,$dloader_ini) || die "$dloader_ini";
@alllines = <inf>;
close (inf);
$str="";
foreach (@alllines)
{
	$str.=$_;
}

$str = SetValue($str,"GUI","CmdPackage",'1');
$str = SetValue($str,"GUI","PacketMode",'0');
$str = SetValue($str,$Segment,$Item,$value);

open F,">$dloader_ini" or die "$dloader_ini";
print F $str;
close F;

sub SetValue
{
    my ($input_string,$Segment,$Item,$value) = @_;
    $input_string=~s/(\n*\s*\[$Segment\])(.*?)(\n*\s*$Item\s*\=)(.*?)(\n+)/$1$2$3$value$5/sg;
    return  $input_string;
}



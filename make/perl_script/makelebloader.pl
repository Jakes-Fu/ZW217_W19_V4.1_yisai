#!/usr/bin/perl

$block_len = 16896;		# 32 * ( 512 + 16 )

$number = scalar @ARGV;						
if($number < 2){
	die "Invalid parameters.\nUsage: perl makebloader.pl sourcefile targetfile\n"
}

$sourcef = ${ARGV[0]};		# source file handle
$targetf = ${ARGV[1]};		# target file handle

# get source file length
$src_len = -s $sourcef;

# the source file length can not be larger than $block_len
if( $src_len > $block_len - 4)
{
	die "$sourcef is too large!\n"
}

# open source file and target file in binary mode
open(SOURCE, "<$sourcef") or die "Can't open $sourcef" ;
binmode SOURCE;

open(TARGET, ">${targetf}") or die "Can't open $targetf";
binmode TARGET;

# read the whole source file
$src_len =  read( SOURCE,$buf,$block_len );

# expand to $block_len
$pad_len = $block_len - $src_len - 4;
if( $pad_len != 0 )
{
	$buf = $buf . pack( "C*",(0xff) x $pad_len );	
}

# convert to an array of short integer
#@src = unpack( "N*",$buf );
@src = unpack( "L*",$buf );

# compute checksum
$sum = 0;
foreach ( @src )
{
	$sum = $sum + $_;
}

# write to target file
print TARGET pack( "N*",@src );
print TARGET pack( "N",$sum );

close SOURCE;
close TARGET;
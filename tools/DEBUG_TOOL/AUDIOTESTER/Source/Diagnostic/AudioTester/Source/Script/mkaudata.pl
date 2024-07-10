#!/usr/bin/perl

$number = scalar @ARGV;						
if($number < 2){
	die "Invalid parameters.\nUsage: perl mkaudata.pl sourcefile targetfile\n"
}

$sourcef = ${ARGV[0]};		# source file handle
$targetf = ${ARGV[1]};		# target file handle

# open source file and target file in binary mode
open(SOURCE, "<$sourcef") or die "Can't open $sourcef" ;

open(TARGET, ">${targetf}") or die "Can't open $targetf";
binmode TARGET;

$id_max_len = 64;
$mode_max_len = 16;

# read souce file line by line
while( $L = <SOURCE> )
{
	print "$L\n";
	
	if( length( $L ) < 3 )
	{
		# blank line
		print "Blank line";
		next;
	}
	
	if( $L =~ /;.*/ )
	{
		# Comments
		print "Comments";
		next;
	}
	
	$L =~ /(.+),(.+),(.+),(.+),(.+),(.+),(.+),(.+)/;
	
	# mode name
	$len = $mode_max_len - length( $1 );
		
	if( $len <= 0 )
	{
		die "Invalid line: $L" 
	}
	
	print TARGET pack( "a16",$1 );
	
	# id name
	$len = $id_max_len - length( $2 );
	if( $len <= 0 )
	{
		die "Invalid line: $L" 
	}
	
	print TARGET pack( "a64",$2 );
	
	# other parameter
	print TARGET pack( "L*",$3,$4,$5,$6,$7,$8 );
}

close SOURCE;
close TARGET;
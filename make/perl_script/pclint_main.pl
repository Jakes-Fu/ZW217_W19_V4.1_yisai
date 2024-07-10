#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 4)
{
	$BUILD_DIR  = $ARGV[0];
	$MKLIBLIST  = $ARGV[1];
	$TOOL_DIR  = $ARGV[2];
	$PROJECT_NAME  = $ARGV[3];
}
else
{
	return;
}

print "Generate pclint depedencies files for pclint scanning\r\n";

mkdir("$BUILD_DIR") if ( !-d "$BUILD_DIR" );
mkdir("$BUILD_DIR/pclint") if ( !-d "$BUILD_DIR/pclint" );

@all_item = split(" ",$MKLIBLIST);
foreach $item(@all_item)
{
	$item=~s/(^\s+|\s+$)//g; 
	if($item ne "")
	{
		system("$TOOL_DIR/make PROJECT=$PROJECT_NAME -f ./make/pclint/pclint.mk -k -r -R COMPONENT=$item pldep"); 
		print "$item create OK! \r\n";
	}	
}
#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$BUILD_DIR  = $ARGV[0];
	$COMPONENT  = $ARGV[1];
	$TOOL_DIR  = $ARGV[2];
}
else
{
	return 1;
}

if(-e "$BUILD_DIR/dep/$COMPONENT.dep")
{
	if(-s "$BUILD_DIR/dep/$COMPONENT.cmp")
	{
		unlink("$BUILD_DIR/dep/$COMPONENT.dep");
		rename("$BUILD_DIR/dep/$COMPONENT.dep.tmp","$BUILD_DIR/dep/$COMPONENT.dep");
	}
	else
	{
		unlink("$BUILD_DIR/dep/$COMPONENT.dep.tmp");
	}
}
else
{
	rename("$BUILD_DIR/dep/$COMPONENT.dep.tmp","$BUILD_DIR/dep/$COMPONENT.dep");
}
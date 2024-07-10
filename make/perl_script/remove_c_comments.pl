#!/usr/bin/perl -w
BEGIN{push @INC,'./make/perl_script/perl_pm'};

$NUM_ARGV = @ARGV;

if($NUM_ARGV >= 2)
{
	my $c_file = $ARGV[0];
	my $c_file_out = $ARGV[1];
	open(CF,"<$c_file");
	my @cf = <CF>;
	close CF;
	my $cf = join '',@cf;
	$cf =~ s/((?<=\n)|^)[ \t]*\/\*.*?\*\/\n?|\/\*.*?\*\/|((?<=\n)|^)[ \t]*\/\/[^\n]*\n|\/\/[^\n]*//gs;
	open(OUT,">$c_file_out");
	print OUT "$cf";
	close OUT;
}
else
{
	return 1;
}
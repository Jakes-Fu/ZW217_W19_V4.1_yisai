#!/usr/bin/perl
BEGIN{push @INC,'./make/perl_script/perl_pm'};

$NUM_ARGV = @ARGV;

if($NUM_ARGV >= 2)
{
        $ProjectName = $ARGV[0];
        $all_loadrules="";
        $all_filelist="";
	for($i=1;$i<($NUM_ARGV);$i++)
	{
		$tmpfile="build/$ProjectName"."_builddir/dep/$ARGV[$i]/$ARGV[$i]"."_loadrule.txt";
		if( -e $tmpfile)
		{		
			open (inf,$tmpfile) || die ("$tmpfile!!!");
			@file_all = <inf>;
			close (inf);
			foreach $myline (@file_all)
			{
				$all_loadrules =$all_loadrules.$myline;
			}
		}
		$tmpfile="build/$ProjectName"."_builddir/dep/$ARGV[$i]/$ARGV[$i]"."_filelist.txt";
		if( -e $tmpfile)
		{		
			open (inf,$tmpfile) || die ("$tmpfile!!!");
			@file_all = <inf>;
			close (inf);
			foreach $myline (@file_all)
			{
				$all_filelist = $all_filelist.$myline;
			}
		}
	}

	$tmpfile="build/$ProjectName"."_builddir/tmp/$ProjectName"."_loadrule.txt";
	open (inf,">$tmpfile") || die ("$tmpfile");
	print inf $all_loadrules;
	close (inf);

	$tmpfile="build/$ProjectName"."_builddir/tmp/$ProjectName"."_filelist.txt";
	open (inf,">$tmpfile") || die ("$tmpfile");
	print inf $all_filelist;
	close (inf);
}
else
{
	return 1;
}

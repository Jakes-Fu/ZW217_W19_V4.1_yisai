#!/usr/bin/perl -w

my $myline;
my $need_replace;
my $outstring = "";
my $include_key = "#include";
my $include_file_string = "#include \"spload_main.h\"";
my $already_replace = 0;
my $sperate_load_dir = "../../../spload/src/lcm";
my @need_replace_string = ("LCD_Read", 
                           "LCD_SendCmd", 
                           "LCD_SendData", 
                           "LCD_SendCmdData", 
                           "OS_TickDelay", 
                           "LCD_Delayms", 
                           "SCI_Sleep",
                           "LCD_Reset",
                           "SCI_TRACE_LOW"
                           );
                           
my @replace_string = ("s_lcm_func_table->lcd_read",
                      "s_lcm_func_table->lcd_sendcmd",
                      "s_lcm_func_table->lcd_senddata", 
                      "s_lcm_func_table->lcd_sendcmdData", 
				      "s_lcm_func_table->lcd_delayms",
				      "s_lcm_func_table->lcd_delayms", 
				      "s_lcm_func_table->lcd_delayms", 
				      "s_lcm_func_table->lcd_reset", 
				      "s_lcm_func_table->trace"
				      );


opendir (SPLOADDIR, $sperate_load_dir) || die ("Cannot open $sperate_load_dir:$!");
while (my $name = readdir SPLOADDIR) {
	next unless $name =~/\.c$/i;
	$name = "$sperate_load_dir/$name";
	if(-f $name)
	{
		unless(-w $name and -r $name)
		{
			printf "please change file :$name attribute to read/write\n ";
			next;
		}
	}
	else
	{
		next;
	}
	my $tmp_file = "$sperate_load_dir.tmp\.c";
	#printf ("tmp file name is : $tmp_file \n");
	open (FH0,  "<$name") || die ("failed to open $name:$!\n\n");
	open (FH1,  ">$tmp_file") || die ("failed to open $name:$!\n\n");
	@alllines = <FH0>;
	close (FH0);
	$outstring = "";
	
	my $need_replace_str_num = @need_replace_string;
	my $replace_str_num = @replace_string;
	my $fist_line;
	if($need_replace_str_num != $replace_str_num)
	{
		die("need replace string num not equal to replace string num");
	}
	$fist_line = $alllines[0];
	if($fist_line =~/##!/)
	{
		$need_replace=0;
	}
	else
	{
		$outstring = $outstring."//##!(important notice: do not remove this line)"."\n";
		$need_replace=1;
	}

	foreach $myline (@alllines)
	{
		chomp($myline);
		$_ = $myline;

		if(/\s*$include_key/ and $need_replace)
		{
			$need_replace = 0;
			$outstring = $outstring.$_."\n"."$include_file_string"."\n";
			next;
		}
		$already_replace = 0;
		for(my $i=0; $i<$need_replace_str_num; $i++)
		{
			if(/$need_replace_string[$i]/)
			{
				s/$need_replace_string[$i]/$replace_string[$i]/;
				$outstring = $outstring.$_."\n";
				$already_replace = 1;
				next;
			}
		}
		if($already_replace == 0)
		{
			$outstring = $outstring.$_."\n";
		}
	}
	print FH1 $outstring;
	close (FH1);
	if(-e $name)
	{
		unlink($name)||die ($!);
		rename($tmp_file, $name)||die ($!);
	}
	else
	{
		die("generete lcm driver file failed!!!");
	}
}
closedir SPLOADDIR;
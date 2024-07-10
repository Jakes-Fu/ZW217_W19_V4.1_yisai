#!/usr/bin/perl -w

my $myline;
my $need_replace;
my $outstring = "";
my $include_key = "#include";
my $include_file_string = "#include \"spload_main.h\"";
my $already_replace = 0;
my $delete_string = "OS_TickDelay";
my $sperate_load_dir = "../../../spload/src/sensor";
my @need_replace_string = ("SCI_Assert",
                           "SCI_ASSERT",
			               "SCI_Sleep",
			               "SCI_TRACE_LOW",
			               "SCI_TraceLow",
			               "OS_TickDelay",
			               
                           "I2C_HAL_Write",
                           "I2C_HAL_Read",
                           
                           "Sensor_ReadReg",
                           "Sensor_WriteReg",
                           "Sensor_SetMCLK",
                           "Sensor_SetVoltage", 
			               "Sensor_SetResetLevel",
                           "Sensor_SetSensorExifInfo",
                           "Sensor_SetCurId",
                           "Sensor_GetCurId",
                           "Sensor_SetMode",
                           "Sensor_GetI2cHnadler",
                           "Sensor_SetInfo",
                           
			               "GPIO_SetSensorPwdn",
                           "GPIO_SetFrontSensorPwdn",
                           
			               "CHIP_DetectMemType"
			              );
			              
my @replace_string =  ("s_lcm_func_table->assert",
                       "s_lcm_func_table->Assert",
                       "s_lcm_func_table->sci_sleep",
                       "s_lcm_func_table->trace",
                       "s_lcm_func_table->trace",
		               "s_lcm_func_table->sci_sleep", 

                       "s_lcm_func_table->i2c_halwrite",
                       "s_lcm_func_table->i2c_halread",

                       "s_lcm_func_table->sensor_readReg", 
		               "s_lcm_func_table->sensor_writereg", 
		               "s_lcm_func_table->sensor_setmclk", 
		               "s_lcm_func_table->sensor_setvoltage", 
		               "s_lcm_func_table->sensor_setresetlevel", 
                       "s_lcm_func_table->sensor_setSensorExifInfo",
                       "s_lcm_func_table->sensor_setCurId",
                       "s_lcm_func_table->sensor_getCurId",
                       "s_lcm_func_table->sensor_setMode",
                       "s_lcm_func_table->sensor_geti2chandler",                       
                       "s_lcm_func_table->sensor_setInfo",       

                       "s_lcm_func_table->gpio_setsensorpwdn",
                       "s_lcm_func_table->gpio_setfrontsensorpwdn",

                       "s_lcm_func_table->chip_detectmemtype" 
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
		$need_replace = 0;
	}
	else
	{
		$outstring = $outstring."//##!(important notice: do not remove this line)"."\n";
		$need_replace = 1;
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
		if(/\buint32\b\s*$delete_string/)
		{
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
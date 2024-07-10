#!/usr/bin/perl
BEGIN{push @INC,'./make/perl_script/perl_pm'};

#$(PERL) make/perl_script/del_target.pl "$*" "nor_fdl resource nv_parameter" "$(BUILD_DIR)/dep/*.pac"

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$current_module = $ARGV[0];
	$current_module =~ s/^\s+|\s+$//g;
	@module_list    = split('\s+',$ARGV[1]);
	$del_file       = $ARGV[2];
	foreach (@module_list)
	{
		chomp;
		$_ =~ s/^\s+|\s+$//g;
		if($_ eq $current_module)
		{
			if($del_file=~/\*/)
			{
				@file_info = split('\*',$del_file);
				$dir = $file_info[0];
				$file = $file_info[1];
				opendir($DIR,$dir) or die "$dir";
				my   $line=readdir($DIR);
				while($line)   
				{   
					if($line ne '.'  &&  $line ne '..')   
  					{
						if($line=~/^$file|$file$/)
						{
							unlink($dir.$line);
						}
  					}   
  					$line=readdir($DIR);   
				}   
				closedir($DIR); 				
			}
			else
			{
				unlink($del_file) if( -e $del_file);
			}
		}
	}
}
else
{
	return 1;
}

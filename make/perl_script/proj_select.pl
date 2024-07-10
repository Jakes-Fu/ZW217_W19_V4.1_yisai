#!usr/bin/perl
#
# 2011/04/15,Cheney Wu 
# proj_select.pl is used to ask user to select project name for make command
# Dynamic detect Verno and project makefile and prompt for user selecting

#****************************************************************************
# Included Modules
#****************************************************************************
BEGIN{push @INC,'./make/perl_script/perl_pm'};
use strict;
use warnings;
my $verfile = $ARGV[0];
my @arguments;

# Get all verno
my $makefile_dir = "./";
my @project_list_str;

#Search valid make file and get the make project ID
opendir( my $DIR,$makefile_dir) || die "can't open $makefile_dir!";

while (my $file = readdir($DIR))
{
	if ($file =~ m/project_(.*)\.mk$/)
	{
		my $verno_temp = $1;
		push @project_list_str,$verno_temp;
	}
}
closedir($DIR);
#print "Found Vernos: @project_list_str \n";


# Echo the make command choice for user
my $option = 0;
my $answer = 0;
prompt_msg:
	print "Select the project to build?\n";
	foreach my $ver (@project_list_str)
	{
		$option++;
		print "$option : $ver \n";
	}
	
	if ($option > 1)
	{
		$answer = <>;
	}
	else
	{
		$answer = 1;
	}

	if (($answer <= $option) && ($answer >0))
	{
    my $selected= $project_list_str[$answer-1];
    @arguments =($selected);
    system("echo @arguments > ~cmd.tmp");
	}



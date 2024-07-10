#!/usr/bin/perl

BEGIN{push @INC,'./make/perl_script/perl_pm/CreateDSP/'};

use createDSP;

$simulator_name = "simulator";
$simulator_tmpdir = "win";
$mynamedir = "win";

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$project_name = $ARGV[0];
	$module_name = $ARGV[1];
}
else
{
	return 1;
}

#init

$ini_file = "build/"."$project_name"."_builddir/tmp/${simulator_name}.ini.res";
open INI_FILE, "<$ini_file" or die "$ini_file";

my $Config_Sec_Name = "";
my %Config_Hash;
while (<INI_FILE>) 
{
  next if ((/^\s*$/) || (/^\s*#/));
  if (/^\s*\[(\S+)\]/)
  {
    $Config_Sec_Name = uc($1);
  } 
  elsif (/^\s*(\S+)\s*=\s*(.*)\s*$/)
  {
    $myKey = "${Config_Sec_Name}_" . $1;
    my $myContent = $2;
    $myContent =~ s/\"//g; 	
    $Config_Hash{$myKey} = $myContent;
  }
  elsif (/^\s*(\S+)\s*\+=\s*(.*)\s*$/)
  {
    $myKey = "${Config_Sec_Name}_". $1;
    my $myContent = $2;
    $myContent =~ s/\"//g; 
    $Config_Hash{$myKey} = $Config_Hash{$myKey} . " $myContent";
  }
}
close INI_FILE;

foreach  (sort keys %Config_Hash) {
  if ($_ =~ /^([A-Z0-9_]+)_([A-Za-z0-9_.]+)$/) {
    push @$_,split('\s+',$Config_Hash{$_});
    $$1{$2} = $Config_Hash{$_};
  } else {
    die "Could not match $_\n";
  }
}


my $MAKE_PATH="build/"."$project_name"."_builddir/"."${simulator_tmpdir}";

push @All_Modules,$module_name;

foreach my $lib (sort @All_Modules) {

    next if($lib eq "" || $lib=~/^\s+$/);

    my $Module_OptionName = "$MAKE_PATH/$lib/$lib.dsp_mac_res";
    my $Module_OptionName_res = "$MAKE_PATH/$lib/$lib.dsp_mac_res_final";
    my @Module_Options;
    my $Module_OptionList = "";

    if(defined($STATIC_OPTION{$lib}))
    {
        push @Module_Options,split('\s+',$STATIC_OPTION{$lib});
	foreach my $option(@Module_Options)
    	{
        	while($option =~ /(\S+)/gs)
        	{
             		my $flag = $1;
 	     		$flag=~s/^-D//g;
            	    	$Module_OptionList .= "/D \"$flag\"\n";
             	}
        }
    }
    else
    {
        if(-e $Module_OptionName)
        {
            open F,"<$Module_OptionName" or die "$Module_OptionName";
            @Module_Options = <F>;
            close F;
        }
        else
        {
             print "\n\n$lib: Warning: Compile macro file was not found!!!\n\n";
        }
        push @Module_Options,split('\s+',$ADD_OPTION{'global'})if(defined($ADD_OPTION{'global'}));
        push @Module_Options,split('\s+',$ADD_OPTION{$lib}) if(defined($ADD_OPTION{$lib}));
        push @Module_Options, split('\s+',$common_compile_option);
 
    	foreach my $option(@Module_Options)
    	{
            while($option =~ /(\S+)/gs)
            {
               my $flag = $1;
 	       $flag=~s/^-D//g;
               if((!InList($flag,$DEL_OPTION{'global'}) && !InList($flag,$DEL_OPTION{$lib})) 
                 ||  $ADD_OPTION{$lib} =~ /\b$flag\b/ ||  $ADD_OPTION{$lib} =~ /\s?$flag\s?/) 
               {
            	    $Module_OptionList .= "-D$flag\n";
               }
            }
       }
    }
    open F,">$Module_OptionName_res" or die "$Module_OptionName_res";
    print F $Module_OptionList;
    close F;
}

sub ProcessStr
{
	my $string = shift;
	my @files = split("\n",$string);
	undef %saw;
	my @out = grep(!$saw{$_}++,@files);
	
	my $stringStr="";
	foreach my $CurrentFile(@out)
	{
      		$_ = $CurrentFile;
      		chomp;
		if($stringStr eq "")
		{
			$stringStr = $_;
		}
		else
		{
			$stringStr = $stringStr."\n".$_;
		}	
	}
	return $stringStr;
}

sub InList {
  my ($item,$list) = @_;
  return 0 if($list eq "");
  return 0 if($list=~/\*/);
  my @files = split(/\n|\s+/,$list);
  foreach my $CurrentFile(@files)
  {
    $tmp = $CurrentFile;
    $tmp =~ s/^\s+|\s+$//g;
    return 1 if($item=~ /^$tmp$/i);
  }
  return 0;
}
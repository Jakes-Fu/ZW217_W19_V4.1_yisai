#!/usr/bin/perl

BEGIN{push @INC,'./make/perl_script/perl_pm/CreateDSP/'};

use createDSP;

$simulator_name = "simulator";
$simulator_tmpdir = "win";
$mynamedir = "win";
$tmpdir = "../../../../";
$external_dsp_tmpdir = "../../../";
$project_root_dir = "../";

$NUM_ARGV = @ARGV;

$vs_ver = "VC6";

if($NUM_ARGV == 8)
{
	$project_name = $ARGV[0];
	$module_names = $ARGV[1];
	$module_names_cus = $ARGV[2];
	$sprd_cus = $ARGV[3];
	$add_link32 = $ARGV[4];
	$active_project_name = $ARGV[5];
	$base_lib_dir = $ARGV[6];
	$modules_no_debug= $ARGV[7];
}
elsif($NUM_ARGV == 10)
{
	$project_name = $ARGV[0];
	$module_names = $ARGV[1];
	$module_names_cus = $ARGV[2];
	$sprd_cus = $ARGV[3];
	$add_link32 = $ARGV[4];
	$active_project_name = $ARGV[5];
	$base_lib_dir = $ARGV[6];
	$modules_no_debug= $ARGV[7];
	$vs_ver= $ARGV[8];
	$root_dir= $ARGV[9];
}
else
{
	return 1;
}
print $module_names;
print "Simulator Version is $vs_ver\n";
#init

print "\n--------------------\n";
print "no debug info dsp: $modules_no_debug\n";
print "--------------------\n";

$ini_file = "build/"."$project_name"."_builddir/tmp/${simulator_name}.ini";
$lib_dir_cus = "build/"."$project_name"."_builddir/lib";
$lib_dir_sprd = "lib/$base_lib_dir";

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

# create all dsp

my $MAKE_PATH="build/"."$project_name"."_builddir/"."${simulator_tmpdir}";
@All_modules_Sprd=(split('\s+',$module_names));
@All_modules_Cus=(split('\s+',$module_names_cus));

@All_modules_no_debug = (split('\s+',$modules_no_debug));
%no_debug_module_hash={};
foreach my $module (sort @All_modules_no_debug) {
	$no_debug_module_hash{$module}=1;
}

if($sprd_cus eq "1")
{
	@All_Modules=(split('\s+',$module_names_cus));
}
else
{
	@All_Modules=(split('\s+',$module_names." ".$module_names_cus));
}

foreach my $lib (sort @All_Modules) {

    next if($lib eq "" || $lib=~/^\s+$/);

    if($module_names =~ /(^\s*$lib\s+)|(\s+$lib\s+)|(^\s*$lib\s*$)|(\s+$lib\s*$)/)
    {
        $lib_dir=$lib_dir_sprd;
    }
    else
    {
        $lib_dir=$lib_dir_cus;
    }

    my $Module_FileName = "$MAKE_PATH/$lib/$lib.dsp_lis";
    my @Module_Files;
    my $Module_FileList = "";

    if(-e "$Module_FileName")
    {
        open F,"<$Module_FileName" or die "$Module_FileName";
        if($DEL_FILE{$lib} ne "*")
        {
            push @Module_Files,<F>;
        }
        close F;
    }

    @replace_path=();
    push @replace_path,split('->',$REPLACE_FILE{$lib}) if(defined($REPLACE_FILE{$lib}));
    push @Module_Files,split('\s+',$ADD_FILE{'global'}) if(defined($ADD_FILE{'global'}));
    push @Module_Files,split('\s+',$ADD_FILE{$lib}) if(defined($ADD_FILE{$lib}));
    
    foreach my $file (@Module_Files) 
    {

      $file =~ s/(^\s+)|(\s+$)//gs;
      next if($file !~ /\S+/);
      if(!InList($file,$DEL_FILE{'global'}) && !InList($file,$DEL_FILE{$lib}))
      {
     	  if(@replace_path>0)     
      	  { 
		$file =~ s/$replace_path[0]/$replace_path[1]/s;
      	  }
          $Module_FileList .= $file ."\n";
      }
    }
    if($sprd_cus eq "1" && $lib eq $active_project_name)
    {
				foreach my $file (@All_modules_Sprd)
        {
          $Module_FileList .= $lib_dir_sprd."/".$file.".lib\n";
        }
    }
    $Module_FileList = join("\n",sort(split('\s+',$Module_FileList))) . "\n";


    my $Module_OptionName = "$MAKE_PATH/$lib/$lib.dsp_mac";
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
						if($vs_ver ne "VS2010")
						{
            	    	$Module_OptionList .= "/D \"$flag\"\n";
             	}
						else
						{
  	   	    	$Module_OptionList .= "-D$flag ";						
						}
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
           	if($vs_ver ne "VS2010")
						{ 	       				
               if((!InList($flag,$DEL_OPTION{'global'}) && !InList($flag,$DEL_OPTION{$lib})) 
                 ||  $ADD_OPTION{$lib} =~ /\b$flag\b/ ||  $ADD_OPTION{$lib} =~ /\s?$flag\s?/) 
               {
            	    $Module_OptionList .= "/D \"$flag\"\n";
               }
            }
            else
            {           	 
               if((!InList($flag,$DEL_OPTION{'global'}) && !InList($flag,$DEL_OPTION{$lib})) 
                 ||  $ADD_OPTION{$lib} =~ /\b$flag\b/ ||  $ADD_OPTION{$lib} =~ /\s?$flag\s?/) 
               {
            	    $Module_OptionList .= "-D$flag ";
               }            	
            }
          }
       	}
    }
 
    my $Module_IncName = "$MAKE_PATH/$lib/$lib.dsp_inc";
    my @Module_Incs;
    my $Module_IncList = "";
    if(-e $Module_IncName)
    {
        open F,"<$Module_IncName" or die "$Module_IncName";
        @Module_Incs=<F>;
        close F;
    }
    else
    {
         print "\n\n$lib: Warning: Include path file was not found!!!\n\n";
    }
    push @Module_Incs, split('\s+',$ADD_INC_PATH{'global'}) if(defined($ADD_INC_PATH{'global'}));
    push @Module_Incs, split('\s+',$ADD_INC_PATH{$lib}) if(defined($ADD_INC_PATH{$lib}));
    push @Module_Incs, split('\s+',$common_include_path);

    foreach (@Module_Incs) {
      while($_ =~ /(\S+)/gs) {
        my $mypath = $1;
        if(!InList($mypath,$DEL_INC_PATH{'global'}) && !InList($mypath,$DEL_INC_PATH{$lib})) 
        {
        	  if($vs_ver ne "VS2010")
						{
							$Module_IncList .= "/I \"$tmpdir$mypath\"\n" ;						}
						else
						{
							$Module_IncList .= "$mypath \\\n" ;
          	}
        }
      }
    }

		my $pdb_flag = "TRUE";
		if(exists($no_debug_module_hash{$lib}))
		{
		 	 $pdb_flag = "FALSE";
		}
				
		if($vs_ver eq "VS2010")
		{
	    createDSP::Get_Module_vcxproj(
	      $lib,
	      "build/"."$project_name"."_builddir/"."win/"."$lib/$lib.list",
	      $Module_OptionList,
	      $Module_IncList,
	      $Module_FileList,
	      \%ADD_OPTION,
	      "$tmpdir",
	      $tmpdir.$lib_dir,
	      $add_link32,
	      $pdb_flag
	    );			
	    	    
		}
		else
		{
    createDSP::Get_Module_Dsp(
      $lib,
      "build/"."$project_name"."_builddir/"."win/"."$lib/$lib.dsp",
      $Module_OptionList,
      $Module_IncList,
      $Module_FileList,
      \%ADD_OPTION,
      "$tmpdir",
      $tmpdir.$lib_dir,
      $add_link32,
      $pdb_flag
    );

if($sprd_cus ne "2")
{
	my $stringlib="";
	foreach my $lib (@All_Modules)
	{
		$stringlib.=$lib." ";
	}

	createDSP::create_workspace("build/"."$project_name"."_builddir/win/${simulator_name}.dsw",$ini_file,$active_project_name,$external_dsp_tmpdir,"$stringlib");
}
 		}
}

if($vs_ver eq "VS2010")
{
		print "start exec CMAKE \n";
		print "module_names_cus $module_names_cus\n";
		print "MAKE_PATH $MAKE_PATH\n";
		print "root_dir $root_dir\n";
		print "Check the C compiler , will take a few minutes , please wait ....\n";
		
		my $module_list .=$module_names_cus;	
		
		if($sprd_cus ne "1")
		{
			$module_list .=" ".$module_names;				
		}
		
		$module_list =~ s/\s/**/g;
				
		system ("./make/perl_script/cmake-3.14.4-win64-x64/bin/cmake.exe"." -G \"Visual Studio 10 2010\""." -DCMAKE_C_COMPILER_FORCED=TRUE -DCMAKE_CXX_COMPILER_FORCED=TRUE -DModule_list:STRING="."${module_list}"." -S ./ -B ${MAKE_PATH}");	
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
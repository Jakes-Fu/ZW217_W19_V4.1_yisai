#!/usr/bin/perl -w
BEGIN{push @INC,'./make/perl_script/perl_pm'};
use strict;
use Cwd;

#according to all dep file, this scirpt is used to generate all source and head files
#of CC:\MS_Code\chip_drv\


#################################################################################
#global varibles
#################################################################################
my $separator_sign = ">";
my %table_hash;
my %clearcaes_hash;

sub get_correct_clearcase_pathname{
    my ($file) = (@_);
    my $open_success;
    my $key_cc_hash;
    my $search;
    my @searchs;
    my $value_cc_hash;
    my $Work_Path;
    my $Work_Dir;
    #print "clearcase_pathname: $file\n";
    $open_success=open(INPUT,"<$file");
    if(!$open_success)
    {
        print "Warning: can not correctly reading data from $file!\n";
        #system('pause');
        die "Can not creat LOG :$!";
    }
    print "reading $file\n";
    @searchs=<INPUT>;
    
    #get correct working directory
    $Work_Path = getcwd;
    #print "debug: $Work_Path\n";
    if($Work_Path =~ /^\/cygdrive\//)
    {
        $Work_Path =~ s/^\/cygdrive\///g;
        $Work_Path =~ s/\//\:\//;
    }
    
    #get correct Work Directory
    $_ = $Work_Path;
    if($_ =~ /[\/]([^\/]+)$/i){
        $Work_Dir = $1;
		$_ = $Work_Dir;
		s/\./$separator_sign/g;
		$Work_Dir = $_;
        #print "debug, path=$Work_Path, Dir=$Work_Dir\n";
    }
    else{
        die "working dir has not been initiated successfully\n";
    }

    #save correct pathname for all files in working dir
    $value_cc_hash = 1;
    $key_cc_hash = "null";;
    foreach $search(@searchs){
        chomp $search;
        $_ = $search;
        s/\\/\//g;
		s/\./$separator_sign/g;
        s/\s//g;
        $search = $_;
        
        if($search=~/[\s\S]*\/$Work_Dir\/([\s\S]*)/)
        {
		    $_ = $1;
			s/$separator_sign/\\\./g;
    	    $search = $_;
        }
        else{
        	die "debug, $search has not match successfully\n";
        }
	
        
        $key_cc_hash = lc($search);
        $value_cc_hash = $search;
        if(exists $clearcaes_hash{$key_cc_hash})
        {
        }
        else
        { 
            $clearcaes_hash{$key_cc_hash}=$value_cc_hash;
            #print "CC <key:$key_cc_hash>,<value:$value_cc_hash>\n";
        }
    }
    close INPUT;
    $Work_Path; 
}
#################################################################################
#print all files of an array
#################################################################################
sub Print_an_Array
{
    my @array = @_;
    foreach (@array)
    {
        print "\t$_\n";
    }
}


#################################################################################
# print all elements of @table_hash
# global variable : %table_hash;
#################################################################################
sub Print_Table_Hash
{
    my ($key_hash,$value_hash,$len_hash);
    $len_hash = 0;
    
    print "------------ begin ------------ Print_Table_Hash\n";
    foreach $key_hash(sort keys%table_hash)
    {
        $key_hash=~s/\r//g;
        $len_hash = $len_hash +1;
        print "Print_Table_Hashing: $table_hash{$key_hash}, key: $key_hash\n";
    }
    print "len of %table_hash : $len_hash\n";
    print "------------ end -------------- Print_Table_Hash\n";
}

#################################################################################
# print all elements of @table_hash
# global variable : %table_hash;
#################################################################################
sub Print_ClearCase_Table_Hash
{
    my ($key_hash,$value_hash,$len_hash);
    $len_hash = 0;
    
    print "------------ begin ------------ Print ClearCase Table Hash\n";
    foreach $key_hash(sort keys%clearcaes_hash)
    {
        $key_hash=~s/\r//g;
        $len_hash = $len_hash +1;
        #print "Print_Table_Hashing: $table_hash{$key_hash}, key: $key_hash\n";
        print "clearcase <$key_hash>\n          <$clearcaes_hash{$key_hash}>\n";
    }
    print "len of %table_hash : $len_hash\n";
    print "------------ end -------------- Print ClearCase Table Hash\n";
}
#################################################################################
# delete all elements of @table_hash
# global variable : %table_hash;
#################################################################################
sub Clear_Table_Hash
{
    my ($key_hash,$value_hash,$len_hash);
    $len_hash = "0";

    foreach $key_hash(sort keys%table_hash)
    {
       delete $table_hash{$key_hash};
    }
        
    foreach $key_hash(sort keys%table_hash)
    {
    	 if(exists $table_hash{$key_hash})
    	 {
           if($len_hash<$table_hash{$key_hash})
           {
               $len_hash = $table_hash{$key_hash};
           }
       }
    }
    #print "len of %table_hash : $len_hash\n";
}


#################################################################################
# Writing Load Rule for each module
# global variable : %table_hash;
# input: %table_hash, name of module
# output: output file name of load rule
#################################################################################
sub Write_Load_Rule_Module
{
    my ($module_name_t,$Pathname_OutPutFile) = @_;
    my $key_hash;
    my $write_sts;
    my $string;
    #print "$module_name_t,$Pathname_OutPutFile\n";
    unlink("$Pathname_OutPutFile");
    $write_sts = open OUTPUT,">",$Pathname_OutPutFile;
    if (!$write_sts)
    {
        print "Warning: can not correctly writing data to $Pathname_OutPutFile!\n";
        die "Can not creat output :$!";
    }
    
    if($module_name_t =~/chip_drv/)
    {
        $string = "load \\MS_Code\\chip_drv\\export\\inc";
        print OUTPUT"$string\n";
        $string = "load \\MS_Code\\chip_drv\\source\\prj_win";
        print OUTPUT"$string\n";
    }
    
    foreach $key_hash(sort keys%table_hash)
    {
        if($module_name_t =~/chip_drv/)
        {
            $string = "chip_drv/export/inc";
            if($key_hash=~/$string/)
            {
                #skip
            }
            else
            {
                $string = "chip_drv/source/prj_win";
                if($key_hash=~/$string/)
                {
            		    #skip
                }
                else
                {
                	  if($key_hash =~/chip_drv/)
                	  {
                        $_=join"/","load /MS_Code",$key_hash;
                        s/\/\//\//g;
                        s/\\\./\./g;
                        s/\//\\/g;
                        print OUTPUT"$_\n";
                        #print "loadrule: $_\n";
                    }
                }
            }
        }
        else
        {
            if($key_hash=~/c:\/|build\//i){}
            else{
                #if($key_hash =~/\/chip_drv\/export\/inc\//)
                $_=join"/","load /MS_Code",$key_hash;
                s/\/\//\//g;
                s/\\\./\./g;
                s/\//\\/g;
                print OUTPUT"$_\n";
                #print "loadrule: $_\n";
            }
        }
    }
    print "$Pathname_OutPutFile created\n";
    close OUTPUT;
}


#################################################################################
# Writing FileList for each module with a project
# global variable : %table_hash;
# input: %table_hash, name of module
# output: output file name of file list
#################################################################################
sub Write_FileList_Module
{
    my ($module_name_t,$Pathname_OutPutFile,$MS_Code_path) = @_;
    my $key_hash;
    my $write_sts;
    my $string;
    #print "debug: $module_name_t,$Pathname_OutPutFile,$MS_Code_path\n";
    unlink("$Pathname_OutPutFile");
    $write_sts = open OUTPUT,">",$Pathname_OutPutFile;
    if (!$write_sts)
    {
        print "Warning: can not correctly writing data to $Pathname_OutPutFile!\n";
        die "Can not creat output :$!";
    }

    foreach $key_hash(sort keys%table_hash)
    {
    	if($key_hash=~/C:/)
      {
      	$_ = $key_hash;
      	s/\\\./\./g;
      	s/\//\\/g;
        print OUTPUT"$_\n";
        #print "$_\n";
      }
      else
      {
      	$_=join"/",$MS_Code_path,$key_hash;
      	#$_=join"/","/MS_Code",$key_hash;
      	s/\/\//\//g;
      	s/\\\./\./g;
        s/\//\\/g;
        print OUTPUT"$_\n";
      }
    }
    
    print "$Pathname_OutPutFile created\n";
    close OUTPUT;
}


#################################################################################
#program starts here,
#para 1: project name, such as "8801h_h5"
#para 2: name of module, such as "m = chip_drv"
#para 3: distribute parameter, such as "ib = 1"
#output:(ib:project_name:module_name)
#################################################################################
sub acquire_proj_module_name
{
    my (@Comand_Input_Array) = @_;
    #print "@array \n";
    my ($proj,$module,$mode);
    my @module_array;
    #my $generate_prj_related_file;
    #my $all_module_name;
    
    foreach (@Comand_Input_Array)
    {
      if(/(p=)\b([\w]+)/)
      {
          $proj = $2;
          #print "p : $proj\n";
      }
      if(/(m=)\b([\s\w]+)/)
      {
          #$module = $2;
          $_= $2;
          @module_array = split/\s/,$_;
          $module = join"$separator_sign",@module_array;
          #print "m : $module\n";
          #Print_an_Array(@module_array);
      }
      if(/(ib=)\b([\w]+)/)
      {
        $mode = $2;
        #print "mode : $mode\n";
      }
    }
    
    if(!$proj)
    {
        print "project name should be entered,please check comand line!\n";
        die "Can not creat LOG :$!";
    }
    
    if(!$mode)
    {
          #print "Important: dependant files should have existed\n";
          $mode = 0;
    }
    $_ = join"$separator_sign",$mode,$proj,$module;
}

#################################################################################
#para 1: project name, such as "8801h_h5"
#para 2: name of module, such as "m = chip_drv"
#output:(
#        path name of objectlist.txt ($read_file):
#        path name of chip drv load rule ($module_load_rules):
#        path name of chip drv files list ($module_file_list))
#       )
#################################################################################
sub acquire_inout_filename
{
  my ($ib_mode_t,$proj,$module) = @_;
  my ($read_file,$read_file_dir,$module_load_rules,$module_file_list,$prj_related_filelist);
  my ($temp);
  my $prj_dir;
  my $clearcase_pathname;
  $temp = "_builddir";
  $prj_dir = "$proj$temp";
  $temp = "obj";
  $read_file = "build/$prj_dir/$temp/$module/objectlist.txt";
  $clearcase_pathname = "build/$prj_dir/tmp/MS_Code_correct_pathname.txt";
  
  if($ib_mode_t == "1")
  {
    $read_file_dir = "build/$prj_dir/dep/$module/cache";
  }
  else
  {
    $read_file_dir = "build/$prj_dir/dep/$module";
  }
  
  #$temp = "dep";
  $ib_mode_t = "_loadrule.txt";
  $module_load_rules = "build/$prj_dir/dep/$module/$module$ib_mode_t";
  $ib_mode_t = "_filelist.txt";
  $module_file_list  = "build/$prj_dir/dep/$module/$module$ib_mode_t";
  $ib_mode_t = "_filelist.txt";
  $prj_related_filelist = "build/$prj_dir/dep/$proj$ib_mode_t";
  $_ = join"$separator_sign",$read_file,$read_file_dir,$module_load_rules,$module_file_list,$prj_related_filelist,$clearcase_pathname;
}

#return value is name of absolute path
sub acquire_absolute_pathname
{
    my($relative_pathname) = shift(@_);
    my $absolue_pathname;
    
    my $flag = 1;
    my $ch = "/";
    
    $_ = $relative_pathname;
    s/\/\//\//g;
    while($flag)
    {
	    s/\/\.\//\//;
        #print "0:$_\n";
        if(/([\S]+)(\/\.\.\/)([\S]+)/)
        {
            if(/([\S]+)(\/[\w]+\/\.\.\/)([\S]+)/)
            {
                #match relative pathname
                #print "1:$1,\n2:$2,\n3:$3\n";
                $flag = 1;
                $_ = join$ch,$1,$3;
                #print "ok:$_\n";
            }
            else 
            {
                if(/([\w]+)(\/\.\.\/)([\S]+)/)
                {
                    $flag = 1;
                    $_ = $3;
                    #print "--------7:$_\n";
                }
            }
        }
        else
        {
            #no match to relative pathname
            $flag = 0;
        }
    }
    
    #the last calculate value is returned by defalut
    $absolue_pathname = $_;
}

#command line:./GenerateFileList_CC.pl input output
#input:name of all files should be listed in file of "input"
#output is build\8801h_h5_builddir\tmp\chip_drv_filelist.txt
my $num_commandline = @ARGV;

#################################################################################
#Read dependant Files, get information
#################################################################################
sub Read_a_file
{
    my ($file) = @_;
    my $line;
    my $string = "";
    my $read_sts=open INPUT,"<","$file";
    my $absoulte_pathname;
    if (!$read_sts)
    {
        print "Warning: can not correctly reading data from $file!\n";
        die "Can not creat input :$!";
    }
    
    while (<INPUT>)
    {
        chomp;
        $line = $_;
        if(/([ \S]+):([\s]+)([ \S]+)/)
        {
            #$1 is neglected ,$3 is saved
            $_ = $3;
            if(/([\w]+)(\.dep)\b/)
            {
                #neglect fisrt line
            }
            else
            {
                #save in $out_file
                if(/(\.\.)+/)
                {
                    #relative pathname 
                    $absoulte_pathname = &acquire_absolute_pathname($_);
                    #print "relative pathname : $absoulte_pathname\n";
                    #print OUTPUT"$_\n";
                }
                else
                {
                    #absolute pathname
                    #print "absolute pathname : $_\n";
                    $absoulte_pathname = $_;
                    if(/(\\ )/)
                    {
                        $_ = $absoulte_pathname;
                        s/\\ / /;
                        #print "$_\n";
                        $absoulte_pathname = $_;
                    }
                }
                $string = "$string$separator_sign$absoulte_pathname";
                #print "line = $string\n";
           }
        }        
    }
    
  close INPUT;
  $string = $string;
}


#################################################################################
#Reading objectlist.txt and writing some information to loadrule.txe
#and filelist.txt. And these files is saved in "build/projeact_name/tmp/".
#################################################################################
sub Handle_Objectlist_per_module
{
    my ($ib_mode_t,$objectlist,$dir,$module_name_t,$output_loadrule,$output_filelist,$clearcase_pathname) = @_;
    my @object_list;
    my @file_list;
    my $line="";
    my ($key_hash,$value_hash) = ("","0");
    my $num = "0";
    my $vpath;
    my $key_cc_hash;
    my $read_sts=open INPUT,"<","$objectlist";
    if (!$read_sts)
    {
        #print "$objectlist may not exist!\n";
    }
    else
    {  
        #print "input : $objectlist\n";
        while (<INPUT>)
        {
            chomp($_);
            $line = "$line$_ ";
        }
        $_=$line;
        if($ib_mode_t=="1")
        {
            s/\.o(\s+|$)/\.ibd /g;
        }
        else
        {
            s/\.o(\s+|$)/\.d /g;
        }
        #print "$_\n";
        @object_list=split('\s+',$_);
        #Print_an_Array(@object_list);
    
        $vpath = get_correct_clearcase_pathname($clearcase_pathname);
        #print "searching directory: $vpath\n";
        
        Clear_Table_Hash();
        foreach(@object_list)
        {
            my $file = $_;
            $file = "$dir/$file";
            #print "file : $file\n";
            $_ = Read_a_file($file);
            @file_list = split/$separator_sign/,$_;
            #Print_an_Array(@file_list);
    
            foreach(@file_list)
            {
                s/\./\\\./;
                $value_hash = $_;
                $key_hash = lc($_);
                if($key_hash eq ""){}
                else{ 
                    if($key_hash =~/c\:\//i)
                    {
                        if(exists $table_hash{$value_hash}){}
                        else
                        { 
                            $table_hash{$value_hash}=0;
                        }
                    }
                    else{
                        if(exists $clearcaes_hash{$key_hash}){
                            if(exists $table_hash{$clearcaes_hash{$key_hash}}){}
                            else
                            { 
                                $table_hash{$clearcaes_hash{$key_hash}}=0;
                            }
                        }
                    }
                }
            }
        }
    
        close INPUT;

        #Print_Table_Hash();
        #Print_ClearCase_Table_Hash();
        Write_Load_Rule_Module($module_name_t,$output_loadrule);
        Write_FileList_Module($module_name_t,$output_filelist,$vpath);
    }
    close INPUT;
}


#################################################################################
#Read dependant Files, get information
#################################################################################
sub Read_Filelist
{
    my $module_filelist = shift(@_);
    my ($key_hash,$value_hash) = ("","0");
    #$module_filelist = "build/sc8800g_sp8803g_builddir/dep/chip_drv/chip_drv_filelist.txt";
    #print "read filelist: $module_filelist\n";
    my $read_sts=open INPUT2,"<","$module_filelist";
    if ($read_sts)
    {
        while (<INPUT2>)
        {
            chomp();
    	      $key_hash = $_;
    	  
            if(exists $table_hash{$key_hash})
            {
                #print "2:$value_hash,$_\n";
            }
            else
            { 
                $table_hash{$key_hash}=$value_hash;
                #print "1:$value_hash,$_\n";
                #print "$value_hash : $key_hash\n";
                $value_hash=$value_hash+1;
            }
        }
    }
    else
    {
    	#print "Warning: $module_filelist should exist!\n";
    	#die "Can not creat input :$!";
    }
    close INPUT2;
}


#################################################################################
#Read dependant Files, get information
#################################################################################
sub Write_Proj_Filelist
{
    my $prj_related_filelist = shift(@_);
    my $write_sts;
    my ($key_hash,$value_hash,$len_hash);
    $len_hash = "0";
    #print "$module_name_t,$prj_related_filelist\n";
    unlink("$prj_related_filelist");
    $write_sts = open OUTPUT,">",$prj_related_filelist;
    if (!$write_sts)
    {
        print "Warning: can not correctly writing data to $prj_related_filelist!\n";
        die "Can not creat output :$!";
    }
    
    foreach $key_hash(sort keys%table_hash)
    {
        $key_hash=~s/\r//g;
        #print "$key_hash";
        print OUTPUT"$key_hash\n";
        if(exists $table_hash{$key_hash})
        {
            if($len_hash<$table_hash{$key_hash})
            {
                $len_hash = $table_hash{$key_hash};
            }
        }
    }
    #print "len of %table_hash : $len_hash\n";
    print "Generate $prj_related_filelist\n";
    close OUTPUT;
}


#################################################################################
#program starts here,
#para 1: project name, such as "8801h_h5"
#para 2: name of module, such as "m = chip_drv"
#para 3: distribute parameter, such as "ib = 1"
#output: @File_Array
#       {   ib
#           project name
#           module name
#       }
#################################################################################
$_ = acquire_proj_module_name(@ARGV);
my @Command_Array = split/$separator_sign/,$_;
#Print_an_Array(@Command_Array);
my $Module_Array;
my $ib_mode = shift(@Command_Array);
my $project_name = shift(@Command_Array);
#my $generate_prj_filelist = shift(@Command_Array);
my $module_name;


#################################################################################
#Handing each module
#para 1: @Command_Array, the length of @Command_Array must be 3.
#################################################################################
my ($read_file,$read_file_dir,$output_loadrule,$output_filelist,$prj_filelist,$cc_correct_pathname);
my @module_file_list;
$Module_Array =  join"$separator_sign",@Command_Array;
#print "Module list: $Module_Array\n";
foreach (@Command_Array)
{
    $module_name = $_;
    #print "p = $project_name, m = $module_name\n";
    $_ = acquire_inout_filename($ib_mode,$project_name,$module_name);
    ($read_file,$read_file_dir,$output_loadrule,$output_filelist,$prj_filelist,$cc_correct_pathname)= split/$separator_sign/,$_;
    #print "$read_file\n$read_file_dir\n$output_loadrule\n$output_filelist\n$prj_filelist\n$cc_correct_pathname\n";
    Handle_Objectlist_per_module($ib_mode,$read_file,$read_file_dir,$module_name,$output_loadrule,$output_filelist,$cc_correct_pathname); 
}

#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$mydir  = $ARGV[0];
}
else
{
	return 1;
}
Searchdir($mydir);
sub Searchdir
{
	my ($input_dir) = @_;
	my $DIR;
	if(-d $input_dir)
	{
  		opendir($DIR,$input_dir) or die "$input_dir";
  		my   $line=readdir($DIR);
  		while   ($line)   
  		{   
                   if($line   ne   '.'   &&   $line   ne   '..'  && -d $input_dir."/".$line)
                   {
            #print $input_dir."/".$line."\n";
            if(uc($line) eq "MMI_RES_DEFAULT")
            {
              if(-e $input_dir."/"."MMI_RES_DEFAULT_H")
               {
                 if(-d $input_dir."/"."MMI_RES_DEFAULT_H")
                 {
		     print "xcopy \"$input_dir/MMI_RES_DEFAULT_H\" \"$input_dir/MMI_RES_DEFAULT\" /E /Y \n";
                     `xcopy \"$input_dir/MMI_RES_DEFAULT_H\" \"$input_dir/MMI_RES_DEFAULT\" /E /Y`;
                 }
               }
           }
           else
           {
             Searchdir($input_dir."/".$line);
           }
         }
			   $line=readdir($DIR); 
      }
		  closedir($DIR);
  	}
}   


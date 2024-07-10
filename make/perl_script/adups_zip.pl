use File::Copy;

my $debug_print = 0;
my $buildFolder = $ARGV[0];
my $rarName = $ARGV[1];

my @requiredFiles;

for my $eachbin(<$buildFolder\\*.bin>){
  if(-d "$eachbin"){
    for my $eachRom(<$eachbin\\*>){
      if(! -d "$eachRom"){
        push @requiredFiles,"$eachRom";
      }
    }
  }
  else{
    push @requiredFiles,"$eachbin";
  }
}


for my $eachTxtFile(<$buildFolder\\*.txt>){
  push @requiredFiles,"$eachTxtFile";
}

for my $eachPacFile(<$buildFolder\\*.pac>){
  push @requiredFiles,"$eachPacFile";
}

my @temprequiredFiles;
for $eachItems(@requiredFiles){
	chomp $eachItems;
  if($eachItems !~ /.sig$/i){
    push @temprequiredFiles,"$eachItems";
  }
}
@requiredFiles = @temprequiredFiles;
for $eachItems(@requiredFiles){
  print $eachItems."\n";
}

#===============================================================================================
# Get rar tool
#===============================================================================================
my $rarPath = ".\\Third-party\\adups\\rar.exe";

if(!-e $rarPath){
	die "No rar.exe in codebase\n";
}

#===============================================================================================
# Compression function
#===============================================================================================
if ($#requiredFiles > -1) {
  # Zip all needed files
  print "Compressing files ......\n";

  if (-e "$buildFolder\\$ENV{\"USERNAME\"}.rar") {
    system "del /q /f $buildFolder\\$ENV{\"USERNAME\"}.rar>nul";
  }

  $fail_num=0;
  foreach $f (@requiredFiles) {
  	next if(!-e "$f");
    print "$rarPath a -r -y -m0 -ep -o+ $buildFolder\\$ENV{\"USERNAME\"}.rar $f\n" if ($debug_print == 1);
    $result = system("$rarPath a -r -y -m0 -ep -o+ $buildFolder\\$ENV{\"USERNAME\"}.rar $f>nul");
    if ($result != 0) {
      $fail_num++;
    }
  }
  print "Compressing was DONE ......\n";
  if ($fail_num>0) {
    print "Some files failed when being compressed.\n" if ($debug_print == 1);
  }
} else {
  print "No file could be compression.\n";
}


if (-e "$buildFolder\\$ENV{\"USERNAME\"}.rar") {
  move("$buildFolder\\$ENV{\"USERNAME\"}.rar","$buildFolder\\$rarName.rar") or die "Can not rename rar file\n";
}

exit 0;

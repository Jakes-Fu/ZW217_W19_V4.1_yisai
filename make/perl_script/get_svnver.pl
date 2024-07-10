#!perl

my @svn_external_path = (
'.',
'PS',
'DSP_BIN/DM_BASE/GSM_DSP_BIN',
'MSL',
);

print "#define  SVN_VER \\\n";

foreach my $svnpath (@svn_external_path) {
  my $svn_info = `svn info $svnpath --xml`;
  my ($svnprop, $svn_num);
  if ($svn_info =~ /^\<url\>(\S+)\<\/url\>/m) {
    $svnprop = $1;
  }
  if ($svn_info =~ /^\<commit\n\s+revision\=\"(\d+)\"\>/m) {
    $svn_num = $1;
  }
  print "\"$svnpath  $svnprop $svn_num\\r\\n\"\\\n";
}

print "\n";



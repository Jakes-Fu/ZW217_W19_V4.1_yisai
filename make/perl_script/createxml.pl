#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$project_name = $ARGV[0];
	$module_name  = $ARGV[1];
}
else
{
		return;
}

$RN = "\n";

$XML_str = "";
$XML_str = $XML_str . '<?xml version="1.0" encoding="UTF-8" standalone="no" ?>'.$RN;
$XML_str = $XML_str . '<BuildSet FormatVersion="1">'.$RN;
$XML_str = $XML_str . '  <Environments>'.$RN;
$XML_str = $XML_str . '    <Environment Name="Default">'.$RN;
$XML_str = $XML_str . '      <Tools>'.$RN;
$XML_str = $XML_str . '        <Tool Name="Compiler" Path="..\..\..\make\make_cmd\CMD.exe" AllowRemote="True" Params="&lt;build\MYPROJECTNAME_builddir\dep\MYMODULENAME\$(SourceFileName)" OutputFileMasks="*.o,*.ibd.tmp,*.ibd"/>'.$RN;
$XML_str = $XML_str . '      </Tools>'.$RN;
$XML_str = $XML_str . '    </Environment>'.$RN;
$XML_str = $XML_str . '  </Environments>'.$RN;
$XML_str = $XML_str . '  <Project Env="Default" Name="'.$project_name."  Module: ".$module_name.'" WorkingDir="..\..\..\">'.$RN;
$XML_str = $XML_str . '    <TaskGroup Name="SourceFiles" Tool="Compiler">'.$RN;
$XML_str = $XML_str . '	        <Task SourceFile="build\MYPROJECTNAME_builddir\dep\MYMODULENAME\*.cmpl" />'.$RN;
$XML_str = $XML_str . '    </TaskGroup>'.$RN;
$XML_str = $XML_str . '  </Project>'.$RN;
$XML_str = $XML_str . '</BuildSet>'.$RN;


$XML_str =~ s/MYPROJECTNAME/$project_name/g;
$XML_str =~ s/MYMODULENAME/$module_name/g;

open (inf,">build/$project_name"."_builddir/dep/$module_name.xml") || die ("build/$project_name_builddir/tmp/$module_name.xml!!!");
@list = <inf>;
print inf $XML_str;
close(inf);





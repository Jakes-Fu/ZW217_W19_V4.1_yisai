#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$module_name  = $ARGV[0];
	$all_c_files = $ARGV[1];
}
else
{
	return 1;
}

%modules=();
$modules{"tdd_atc_hsdpa"} = "atc_lex.c ";
$modules{"img_proc"} = "Tumble.c ChipmunkDemo.c drawSpace.c Plink.c cpDampedSpring.c cpDampedRotarySpring.c cpSlideJoint.c cpPinJoint.c cpConstraint.c cpArray.c cpSweep1D.c cpArbiter.c cpBB.c cpBBTree.c cpHashSet.c cpPolyShape.c cpShape.c cpSpaceComponent.c cpSpaceHash.c cpSpatialIndex.c cpVect.c chipmunk.c cpBody.c cpCollision.c cpSpace.c cpSpaceQuery.c cpSpaceStep.c ";
#$modules{"rtos"} = "RTOS/source/src/c a.c b.c";  # RTOS/source/src/c/*.c + a.c + b.c
$modules{"xsr_layer"} = "BMLInterface.c BadBlkMgr.c GarbageQueue.c OpQueue.c SamBufMgr.c SectorMap.c STLInterface.c VirtualNand.c os_threadx.c pam.c lld.c ecc.c pnl.c pns.c swap.s compaction_task.c xsr_probe.c";
$modules{"xsr_layer_os_none"} = "BMLInterface.c BadBlkMgr.c GarbageQueue.c OpQueue.c SamBufMgr.c SectorMap.c STLInterface.c VirtualNand.c oslessoam.c pam.c lld.c ecc.c pnl.c pns.c xsr_probe.c";
$modules{"custom_drv"} = "atv_nmi601.c nmiport.c nmiapi.c nmidrv_fm.c nmidrv_i2c.c nmidrv_atv.c athe_wifi_adapter.c atv_qn8610.c tlg1120Api.c I2Csample.c atv_tlg1120.c tlg1120App.c qmafunc.c qmadriver.c tlg1120Customize.c tlg1120Mfg.c";
$modules{"gzip"} = "MS_Ref/source/gzip/src";
$modules{"sqlite"} = "external/sqlite";
$modules{"ps"} = "BASE/PS/source/sdl_gen_code/c";
$modules{"zlib"} = "Third-party/zlib/source/c";
$modules{"theme_unzip"} = "theme_util.c theme_unzip.c theme_inflate.c theme_gzip.c";
$modules{"nes"} = "nes";

open(inf,$all_c_files) or die "$all_c_files";
@modules_files = <inf>;
close (inf); 

our @modules_filter_files;

foreach $item (split('\s+',$modules{$module_name}))
{
	if( -d $item)
	{
		&Get_Filter_Files($item);
	}
	else
	{
		push @modules_filter_files,$item;
	}
}


$out_str="";
foreach $m_file (@modules_files)
{
	$m_file=~ s/^\s+|\s+$//g;
	$samefile=0;
	foreach (@modules_filter_files)
	{
		
		$_ =~ s/^\s+|\s+$//g;
		if($m_file eq $_)
		{
			$samefile=1;
			last;
		}
	}
	if($samefile ==0)
	{
		$out_str=$out_str." ".$m_file;
	}
}

print $out_str;

sub Get_Filter_Files
{ 
	my $input_dir = $_[0];
	opendir(DIR,$input_dir) or die "$input_dir";
	my @sub_item = readdir(DIR);
	close (DIR);
	foreach (@sub_item)
	{       
		if($_ ne '.'  &&  $_ ne '..' )
		{
			if(-d $_)
			{
				&Get_Filter_Files($_);
			}
			else
			{	
				push @modules_filter_files,$_;
			}
		}
	}
}	




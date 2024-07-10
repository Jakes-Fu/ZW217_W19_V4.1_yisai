#!/usr/bin/perl


$NUM_ARGV = @ARGV;

if($NUM_ARGV == 18)
{
	    $PLATFORM=           $ARGV[0];
        $MMI_RES_DIR =       $ARGV[1]; 
        $IMGFILENAME =       $ARGV[2]; 
        $logel_label =       $ARGV[3]; 
        $ROOT_DIR =          $ARGV[4]; 
        $PROJECT_NAME =      $ARGV[5];
        $NV_GROUP =          $ARGV[6];
        $VM_SUPPORT=         $ARGV[7];
        $PRELOAD_SUPPORT=    $ARGV[8];
        $NANDBOOT_SUPPORT=   $ARGV[9];
        $DEV_MANAGE_SUPPORT= $ARGV[10];
        $USB_VCOM_AUTORUN_SUPPORT= $ARGV[11];
	    $STONE_IMAGE_SUPPORT= $ARGV[12]; 
		$SPLOAD_ENABLE=      $ARGV[13];       
		$MEMORY_CUT        = $ARGV[14];
		$DUAL_DSP_ADAPTION = $ARGV[15];		        
		$IMG_NAME_PREFIX = $ARGV[16];
		$NDTCODE   		=  $ARGV[17];
}
elsif($NUM_ARGV == 13)
{
	    $PLATFORM=           $ARGV[0];
        $IMGFILENAME =       $ARGV[1]; 
        $logel_label =       $ARGV[2]; 
        $ROOT_DIR =          $ARGV[3]; 
        $PROJECT_NAME =      $ARGV[4];
        $NV_GROUP =          $ARGV[5];
        $VM_SUPPORT=         $ARGV[6];
        $PRELOAD_SUPPORT=    $ARGV[7];
        $NANDBOOT_SUPPORT=   $ARGV[8];
        $DEV_MANAGE_SUPPORT= $ARGV[9];
        $MODEM_PLATFORM =    $ARGV[10];
		$STONE_IMAGE_SUPPORT= $ARGV[11]; 
		$NOR_BOOTLOADER_SUPPORT = $ARGV[12];
}
elsif($NUM_ARGV == 14)
{
	    $PLATFORM=           $ARGV[0];
        $MMI_RES_DIR =       $ARGV[1]; 
        $IMGFILENAME =       $ARGV[2]; 
        $logel_label =       $ARGV[3]; 
        $ROOT_DIR =          $ARGV[4]; 
        $PROJECT_NAME =      $ARGV[5];
        $NV_GROUP =          $ARGV[6];
        $VM_SUPPORT=         $ARGV[7];
        $UMEM_SUPPORT=       $ARGV[8];
        $NANDBOOT_SUPPORT=   $ARGV[9];
        $DEV_MANAGE_SUPPORT= $ARGV[10];
        $USB_VCOM_AUTORUN_SUPPORT= $ARGV[11];
		$STONE_IMAGE_SUPPORT= $ARGV[12]; 
		$SPLOAD_ENABLE=      $ARGV[13];
}
elsif($NUM_ARGV == 15)
{
	    $PLATFORM=           $ARGV[0];
        $MMI_RES_DIR =       $ARGV[1]; 
        $IMGFILENAME =       $ARGV[2]; 
        $logel_label =       $ARGV[3]; 
        $ROOT_DIR =          $ARGV[4]; 
        $PROJECT_NAME =      $ARGV[5];
        $NV_GROUP =          $ARGV[6];
        $VM_SUPPORT=         $ARGV[7];
        $UMEM_SUPPORT=       $ARGV[8];
        $PRELOAD_SUPPORT=    $ARGV[9];
        $NANDBOOT_SUPPORT=   $ARGV[10];
        $DEV_MANAGE_SUPPORT= $ARGV[11];
        $USB_VCOM_AUTORUN_SUPPORT= $ARGV[12];
		$STONE_IMAGE_SUPPORT= $ARGV[13]; 
		$SPLOAD_ENABLE=      $ARGV[14];
}
else
{
	return 1;
}

$ROOT_DIR =~s/\//\\/g;

$BUILD_DIR =  $ROOT_DIR."\\build\\$PROJECT_NAME"."_builddir";

$dloader_ini = "$BUILD_DIR\\ResearchDownload\\ResearchDownload.ini";

$NV_GROUP =~s/^\s+|\s+$//g;

$error="";

$error.="NV is missing!\n"  if($NV_GROUP eq "");

@NV_GROUPS= split('\s+',$NV_GROUP);

$nv_sum=@NV_GROUPS;

foreach $CurrentNV(@NV_GROUPS)
{
		if (1) ##($PLATFORM eq "UIX8910")
		{
			  my @pac_input_args;
			  $pac_input_args[0]  = "$BUILD_DIR\\img\\"."$IMG_NAME_PREFIX"."$IMGFILENAME"."_$NDTCODE.pac";                                             
			  $pac_input_args[1]  = $logel_label."_".$NDTCODE;  
			  $pac_input_args[2]  = ".\\make\\perl_script\\UIX8910.xml";                                              
			  $pac_input_args[3]  = "$BUILD_DIR\\img\\"."$IMG_NAME_PREFIX"."nor_fdl1.img";                                                     #"FDL"        
			  $pac_input_args[4]  = "$BUILD_DIR\\img\\"."$IMG_NAME_PREFIX"."nor_fdl.bin";                                                     #"FDL2"       
			  $pac_input_args[5]  = "$BUILD_DIR\\img\\$CurrentNV.bin";                                               #"NV"         
			  $pac_input_args[6]  = "$BUILD_DIR\\img\\"."$IMG_NAME_PREFIX"."nor_bootloader.img";                                               #"BootLoader" 
			  $pac_input_args[7]  = "";			  
			  $pac_input_args[8]  = "$BUILD_DIR\\img\\"."$IMG_NAME_PREFIX"."$IMGFILENAME.bin";                            #"UserImg"    
			  $pac_input_args[9]  = "$BUILD_DIR\\img\\$MMI_RES_DIR.bin";                                             #"MMIRes"     
			  $pac_input_args[10] = "";   # "$BUILD_DIR\\img\\DSP_DM_G2.bin";                                                #"DSPCode"    
			  $pac_input_args[11] = "";                                                                              #"OmadmFota"
			  $pac_input_args[12] = "";
=pod			  
		    if($PRELOAD_SUPPORT eq "TRUE")
		    {
		        $pac_input_args[12] = "$BUILD_DIR\\img\\preload_img.bin";                                           #"Preload"    
		    }
		    else
		    {
			    $pac_input_args[12] = "";                                                                           #"Preload"    
		    }
			  $pac_input_args[13] = "";                                                                             #"RomDisk"    
			  $pac_input_args[14] = "$BUILD_DIR\\img\\"."$IMG_NAME_PREFIX"."EXEC_KERNEL_IMAGE".".bin";               #"KernelImg"    

		    if($DUAL_DSP_ADAPTION eq "TRUE")
		    {
			    $pac_input_args[15] = "$BUILD_DIR\\img\\DSP2_DM_G2.bin";                                             #"DSPCode2"
		    }
		    else
		    {
			    $pac_input_args[15] = "";                                                                          #"DSPCode2"    
		    }
			
			  my $cmd_str = ".\\make\\make_cmd\\perl \".\\make\\perl_script\\pac_UIX8910.pl\" \"$pac_input_args[0]\" \"$pac_input_args[1]\" \"$pac_input_args[2]\" \"$pac_input_args[3]\" \"$pac_input_args[4]\" \"$pac_input_args[5]\" \"$pac_input_args[6]\" \"$pac_input_args[7]\" \"$pac_input_args[8]\" \"$pac_input_args[9]\" \"$pac_input_args[10]\" \"$pac_input_args[11]\" \"$pac_input_args[12]\" \"$pac_input_args[13]\" \"$pac_input_args[14]\" \"$pac_input_args[15]\"";
=cut
			  my $cmd_str = ".\\make\\make_cmd\\perl \".\\make\\perl_script\\pac_UIX8910.pl\" \"$pac_input_args[0]\" \"NOR_FLASH_UIX8910\" \"$pac_input_args[1]\" \"$pac_input_args[2]\" \"$pac_input_args[3]\" \"$pac_input_args[4]\" \"$pac_input_args[5]\" \"$pac_input_args[6]\" \"$pac_input_args[7]\" \"$pac_input_args[8]\" \"$pac_input_args[9]\" \"$pac_input_args[10]\" \"$pac_input_args[11]\"";	
		  
			  print $cmd_str;
			  system ($cmd_str);
			  exit;
		}
    open (inf,$dloader_ini) || die "$dloader_ini";
    @alllines = <inf>;
    close (inf);
    $str="";
    foreach (@alllines)
    {
			$str.=$_;
    }

    $str = SetValue($str,"GUI","CmdPackage",'1');
    $str = SetValue($str,"GUI","PacketMode",'0');

    if($PLATFORM eq "SC8800H" || $PLATFORM eq "SC8800G" )
    {
	
        $str = SetValue($str,"Selection","SelectProduct","$PLATFORM");
        $str = SetValue($str,"$PLATFORM","FLASH","1@");
        $str = SetValue($str,"$PLATFORM","PhaseCheck","1@");
        $str = SetValue($str,"$PLATFORM","FDL","1@"."$BUILD_DIR\\img\\fdl1.bin");
        $str = SetValue($str,"$PLATFORM","FDL2","1@"."$BUILD_DIR\\img\\fdl2.bin");
        $str = SetValue($str,"$PLATFORM","BootLoader","1@"."$BUILD_DIR\\img\\bootloader.bin");
        $str = SetValue($str,"$PLATFORM","DSPCode","1@"."$BUILD_DIR\\img\\DSP_DM_G2.bin");
        $str = SetValue($str,"$PLATFORM","MMIRes","1@"."$BUILD_DIR\\img\\$MMI_RES_DIR.bin");
        $str = SetValue($str,"$PLATFORM","NV","1@"."$BUILD_DIR\\img\\$CurrentNV.bin");
        $str = SetValue($str,"$PLATFORM","KernelImg","1@"."$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE.bin");
        $str = SetValue($str,"$PLATFORM","UserImg","1@"."$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_USER_IMAGE.bin");
        $str = SetValue($str,"$PLATFORM","SPLOAD","0@");
       
        $error.="$BUILD_DIR\\img\\fdl1.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\fdl1.bin");
        $error.="$BUILD_DIR\\img\\fdl2.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\fdl2.bin");
        $error.="$BUILD_DIR\\img\\bootloader.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\bootloader.bin");
        $error.="$BUILD_DIR\\img\\DSP_DM_G2.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\DSP_DM_G2.bin");

        $error.="$BUILD_DIR\\img\\$MMI_RES_DIR.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\$MMI_RES_DIR.bin");
        $error.="$BUILD_DIR\\img\\$CurrentNV.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\$CurrentNV.bin");
        $error.="$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE.bin");
        $error.="$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_USER_IMAGE.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_USER_IMAGE.bin");

        if($DEV_MANAGE_SUPPORT eq "TRUE")
        {
            $str = SetValue($str,"$PLATFORM","OmadmFota","1@"."$BUILD_DIR\\img\\omadm_fota.bin");
            $error.="$BUILD_DIR\\img\\omadm_fota.bin is missing!\n" if(!-e "$BUILD_DIR\\img\\omadm_fota.bin");
        }
        if($VM_SUPPORT eq "TRUE")
        {
            $str = SetValue($str,"$PLATFORM","KernelImg2","1@"."$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE2.bin");
            $error.="$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE2.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE2.bin");
        }
        if($PRELOAD_SUPPORT eq "TRUE")
        {
            $str = SetValue($str,"$PLATFORM","Preload","1@"."$BUILD_DIR\\img\\preload_img.bin");
            $error.="$BUILD_DIR\\img\\preload_img.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\preload_img.bin");
        }
        if($USB_VCOM_AUTORUN_SUPPORT eq "TRUE")
        {
            $str = SetValue($str,"$PLATFORM","RomDisk","1@"."$BUILD_DIR\\img\\romdisk.bin");
            $error.="$BUILD_DIR\\img\\romdisk.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\romdisk.bin");
        }
          if($SPLOAD_ENABLE eq "TRUE")
        {
            $str = SetValue($str,"$PLATFORM","SPLOAD","1@"."$BUILD_DIR\\img\\spload.bin");
            $error.="$BUILD_DIR\\img\\spload.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\spload.bin");
    }
    }

    if($PLATFORM eq "SC6600L" || $PLATFORM eq "SC6530")
    {
		if($MODEM_PLATFORM eq "TRUE" )
		{
	        $str = SetValue($str,"Selection","SelectProduct","SC65XX_MODEM");
	        $str = SetValue($str,"SC65XX_MODEM","FLASH","1@");
	        $str = SetValue($str,"SC65XX_MODEM","PhaseCheck","1@");
	        $str = SetValue($str,"SC65XX_MODEM","NV","1@"."$BUILD_DIR\\img\\$CurrentNV.bin");
	        $str = SetValue($str,"SC65XX_MODEM","FDL","1@"."$BUILD_DIR\\img\\nor_fdl.bin");
	        $str = SetValue($str,"SC65XX_MODEM","PS","1@"."$BUILD_DIR\\img\\$IMGFILENAME");

	        $error.="$BUILD_DIR\\img\\$CurrentNV.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$CurrentNV.bin");
	        $error.="$BUILD_DIR\\img\\nor_fdl.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\nor_fdl.bin");
	        $error.="$BUILD_DIR\\img\\$IMGFILENAME is missing!\n"  if(!-e "$BUILD_DIR\\img\\$IMGFILENAME");

	        if($NOR_BOOTLOADER_SUPPORT eq "TRUE")
	        {
	           $str = SetValue($str,"SC65XX_MODEM","Bootloader","1@"."$BUILD_DIR\\img\\nor_bootloader.bin");
	           $error.="$BUILD_DIR\\img\\Bootloader is missing!\n"  if(!-e "$BUILD_DIR\\img\\nor_bootloader.bin");
	        }
		}
		else
		{
	        $str = SetValue($str,"Selection","SelectProduct","NOR_FLASH_SC6530");
	        $str = SetValue($str,"NOR_FLASH_SC6530","FLASH","1@");
	        $str = SetValue($str,"NOR_FLASH_SC6530","PhaseCheck","1@");
	        if($UMEM_SUPPORT eq "TRUE")
	        {
	            $str = SetValue($str,"NOR_FLASH_SC6530","EraseUdisk","1@");
	        }
	        if($PRELOAD_SUPPORT eq "TRUE")
	        {
                $str = SetValue($str,"NOR_FLASH_SC6530","UDISK","1@"."$BUILD_DIR\\img\\preload_img.bin");
                $error.="$BUILD_DIR\\img\\preload_img.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\preload_img.bin");
	        }
	        $str = SetValue($str,"NOR_FLASH_SC6530","NV","1@"."$BUILD_DIR\\img\\$CurrentNV.bin");
	        $str = SetValue($str,"NOR_FLASH_SC6530","MMIRes","0@"."$BUILD_DIR\\img\\$MMI_RES_DIR.bin");
	        $str = SetValue($str,"NOR_FLASH_SC6530","FDL","1@"."$BUILD_DIR\\img\\nor_fdl.bin");
	        $str = SetValue($str,"NOR_FLASH_SC6530","PS","1@"."$BUILD_DIR\\img\\$IMGFILENAME");
	        $error.="$BUILD_DIR\\img\\$CurrentNV.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$CurrentNV.bin");
	        $error.="$BUILD_DIR\\img\\nor_fdl.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\nor_fdl.bin");
	        $error.="$BUILD_DIR\\img\\$IMGFILENAME is missing!\n"  if(!-e "$BUILD_DIR\\img\\$IMGFILENAME");
		}
    }
    if($PLATFORM eq "SC6800H")
    {
      if($NANDBOOT_SUPPORT eq "TRUE")
      {
        $str = SetValue($str,"Selection","SelectProduct","NAND_FLASH_6800H");
        $str = SetValue($str,"NAND_FLASH_6800H","FLASH","1@");
        $str = SetValue($str,"NAND_FLASH_6800H","PhaseCheck","1@");
        $str = SetValue($str,"NAND_FLASH_6800H","FDL","1@"."$BUILD_DIR\\img\\fdl1.bin");
        $str = SetValue($str,"NAND_FLASH_6800H","FDL2","1@"."$BUILD_DIR\\img\\fdl2.bin");
        $str = SetValue($str,"NAND_FLASH_6800H","BootLoader","1@"."$BUILD_DIR\\img\\bootloader.bin");
        $str = SetValue($str,"NAND_FLASH_6800H","MMIRes","1@"."$BUILD_DIR\\img\\$MMI_RES_DIR.bin");
        $str = SetValue($str,"NAND_FLASH_6800H","NV","1@"."$BUILD_DIR\\img\\$CurrentNV.bin");
        $str = SetValue($str,"NAND_FLASH_6800H","KernelImg","1@"."$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE.bin");
        $str = SetValue($str,"NAND_FLASH_6800H","UserImg","1@"."$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_USER_IMAGE.bin");
        $str = SetValue($str,"NAND_FLASH_6800H","FormatFS","1@");
        $str = SetValue($str,"NAND_FLASH_6800H","FormatHFS","1@");
        $str = SetValue($str,"NAND_FLASH_6800H","Preload","0@");
       $str = SetValue($str,"NAND_FLASH_6800H","SPLOAD","0@");

        $error.="$BUILD_DIR\\img\\fdl1.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\fdl1.bin");
        $error.="$BUILD_DIR\\img\\fdl2.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\fdl2.bin");
        $error.="$BUILD_DIR\\img\\bootloader.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\bootloader.bin");
        $error.="$BUILD_DIR\\img\\$MMI_RES_DIR.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$MMI_RES_DIR.bin");
        $error.="$BUILD_DIR\\img\\$CurrentNV.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$CurrentNV.bin");
        $error.="$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_KERNEL_IMAGE.bin");
        $error.="$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_USER_IMAGE.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$IMGFILENAME\\EXEC_USER_IMAGE.bin");

        if($PRELOAD_SUPPORT eq "TRUE")
        {
            $str = SetValue($str,"NAND_FLASH_6800H","FormatHFS","1@");
            $str = SetValue($str,"NAND_FLASH_6800H","Preload","1@"."$BUILD_DIR\\img\\preload_img.bin");
            $error.="$BUILD_DIR\\img\\preload_img.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\preload_img.bin");
        }
        if($USB_VCOM_AUTORUN_SUPPORT eq "TRUE")
        {
            $str = SetValue($str,"$PLATFORM","RomDisk","1@"."$BUILD_DIR\\img\\romdisk.bin");
            $error.="$BUILD_DIR\\img\\romdisk.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\romdisk.bin");
        }
        if($SPLOAD_ENABLE eq "TRUE")
        {
            $str = SetValue($str,"NAND_FLASH_6800H","SPLOAD","1@"."$BUILD_DIR\\img\\spload.bin");
            $error.="$BUILD_DIR\\img\\spload.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\spload.bin");
      }
      }
      else
      {
        $str = SetValue($str,"Selection","SelectProduct","NOR_FLASH_6800H");
        $str = SetValue($str,"NOR_FLASH_6800H","FLASH","1@");
        $str = SetValue($str,"NOR_FLASH_6800H","PhaseCheck","1@");
        $str = SetValue($str,"NOR_FLASH_6800H","NV","1@"."$BUILD_DIR\\img\\$CurrentNV.bin");
        $str = SetValue($str,"NOR_FLASH_6800H","MMIRes","1@"."$BUILD_DIR\\img\\$MMI_RES_DIR.bin");
        $str = SetValue($str,"NOR_FLASH_6800H","FDL","1@"."$BUILD_DIR\\img\\nor_fdl.bin");
        $str = SetValue($str,"NOR_FLASH_6800H","PS","1@"."$BUILD_DIR\\img\\$IMGFILENAME");
      	$str = SetValue($str,"NOR_FLASH_6800H","SPLOAD","0@");
        
        if($STONE_IMAGE_SUPPORT eq "TRUE")
        {
        	$str = SetValue($str,"NOR_FLASH_6800H","MMIRes","0@");
        }

        if($SPLOAD_ENABLE eq "TRUE")
        {
            $str = SetValue($str,"NOR_FLASH_6800H","SPLOAD","1@"."$BUILD_DIR\\img\\spload.bin");
            $error.="$BUILD_DIR\\img\\spload.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\spload.bin");
        }

        $error.="$BUILD_DIR\\img\\$MMI_RES_DIR.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$MMI_RES_DIR.bin");
        $error.="$BUILD_DIR\\img\\$CurrentNV.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\$CurrentNV.bin");
        $error.="$BUILD_DIR\\img\\nor_fdl.bin is missing!\n"  if(!-e "$BUILD_DIR\\img\\nor_fdl.bin");
        $error.="$BUILD_DIR\\img\\$IMGFILENAME is missing!\n"  if(!-e "$BUILD_DIR\\img\\$IMGFILENAME");
      }
    }

    open F,">$dloader_ini" or die "$dloader_ini";
    print F $str;
    close F;
    $error=~s/^\s+|\s+$//g;
    if($error eq "")
    {
        if($nv_sum ==1)
        {
            system("$BUILD_DIR\\ResearchDownload\\ResearchDownload.exe $BUILD_DIR\\img\\$PROJECT_NAME.pac $logel_label");
        }
        else
        {
            system("$BUILD_DIR\\ResearchDownload\\ResearchDownload.exe $BUILD_DIR\\img\\$PROJECT_NAME"."_"."$CurrentNV.pac $logel_label");
        }
    }
    else
    {
        print $error;
    }
}
sub SetValue
{
    my ($input_string,$Segment,$Item,$value) = @_;
    $input_string=~s/(\n*\s*\[$Segment\])(.*?)(\n*\s*$Item\s*\=)(.*?)(\n+)/$1$2$3$value$5/sg;
    return  $input_string;
}


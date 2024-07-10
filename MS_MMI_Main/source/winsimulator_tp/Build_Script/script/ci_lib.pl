#!/usr/bin/perl


$COMMENT = "checkin *.lib for gphone base build";

# --------- BEGIN update MS_Code file -----------------
@FILE_PATH_1 = (	  "..\\..\\..\\..\\..\\MS_Code\\PS\\Lib\\DevPS.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\Ref\\Lib\\driver_common_win.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\Ref\\Lib\\driver_windows.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\Ref\\Lib\\img.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\Ref\\Lib\\ref_base_win.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\MM_Ref\\Lib\\driver_common_win.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\MM_Ref\\Lib\\driver_windows.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\MM_Ref\\Lib\\img.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\MM_Ref\\Lib\\ref_base_win.lib",
			  "..\\..\\..\\..\\..\\MS_Code\\RTOS\\Lib\\rtos_win.lib",
			  "..\\..\\..\\..\\..\\Tools\\Common\\Bin\\SIMConPad.dll",
			  "..\\..\\..\\..\\..\\Tools\\Common\\Bin\\MSDevKernel.lib",
			  "..\\..\\..\\..\\..\\Tools\\Common\\Bin\\MSDevKernel.dll",
			  "..\\..\\..\\..\\..\\Tools\\Common\\Bin\\MSDevBridge.lib",
			  "..\\..\\..\\..\\..\\Tools\\Common\\Bin\\MSDev.exe",
			  "..\\..\\..\\..\\..\\Tools\\Common\\Bin\\filesys.dll",
			  "..\\..\\..\\..\\..\\Tools\\Common\\Bin\\ConPan.lib",
			  "..\\..\\..\\..\\..\\Tools\\Common\\Bin\\ConPan.dll");
			 
			  

#1.0 checkin file
	do Checkin(@FILE_PATH_1);
# -------------  END of update MS_Code file ---------------


### check in files #########################################
#  @Param:
#   		@FPATH ,文件路径数组
############################################################
sub Checkin
{
	my (@FPATH) = @_;
	
	foreach $FL_PATH (@FPATH)
	{
		foreach $FILE (`cmd /c cleartool lsco -r -short -me $FL_PATH`)
		{
			
				print "Checking in *.lib $FILE";
				`cmd /c cleartool ci -c "$COMMENT"  $FILE`;
			
		}
	}
}

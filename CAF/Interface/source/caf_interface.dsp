# Microsoft Developer Studio Project File - Name="caf_interface" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=caf_interface - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "caf_interface.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "caf_interface.mak" CFG="caf_interface - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "caf_interface - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "caf_interface - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "caf_interface"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "caf_interface - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "caf_interface - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI /Od /I "..\..\..\DAPS\export\inc" /I "..\..\..\RTOS\export\inc" /I "..\..\..\MS_Customize\export\inc" /I "..\..\..\MS_MMI\source\mmi_kernel\include" /I "..\..\..\MS_MMI\source\mmi_kernel_sys\include" /I "..\..\..\chip_drv\export\inc" /I "..\..\..\Third-party\ImageDecoder\Include" /I "..\..\..\common\export\inc" /I "..\..\..\MS_MMI\source\mmi_gui\include" /I "..\..\..\MS_MMI\source\mmi_app\app\pubwin\h" /I "..\..\..\MS_MMI\source\mmi_app\common\h" /I "..\..\..\MS_MMI\source\mmi_app\kernel\h" /I "..\..\..\MS_Ref\export\inc" /I "..\..\Template\include" /I "..\..\..\MS_MMI\source\mmi_app\app\audio\h" /I "..\..\..\MS_MMI\source\mmi_app\app\fmm\h" /I "..\..\..\l4\export\inc" /I "..\..\..\MS_MMI\source\mmi_app\custom\h" /I "..\..\..\MS_MMI\source\resource" /I "..\..\..\CAF\Interface\include" /I "..\..\..\MS_MMI\source\mmi_app\App\camera\h" /I "..\..\..\sim\export\inc" /I "..\..\..\PS\export\inc" /I "..\..\..\MS_MMI\source\mmi_app\app\mpeg4\h" /I "..\..\..\MS_MMI\source\mmi_app\app\pdp\h" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_RTOS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\MS_MMI\source\Lib\caf_interface.lib"

!ENDIF 

# Begin Target

# Name "caf_interface - Win32 Release"
# Name "caf_interface - Win32 Debug"
# Begin Source File

SOURCE=.\filesys\caffilesys.c
# End Source File
# Begin Source File

SOURCE=.\heap\cafheap.c
# End Source File
# Begin Source File

SOURCE=.\image\cafimage.c
# End Source File
# Begin Source File

SOURCE=.\Media\cafmedia.c
# End Source File
# Begin Source File

SOURCE=.\Sound\cafmm.c
# End Source File
# Begin Source File

SOURCE=.\socket\cafsocket.c
# End Source File
# Begin Source File

SOURCE=.\Sound\cafsound.c
# End Source File
# Begin Source File

SOURCE=.\tapi\caftapi.c
# End Source File
# Begin Source File

SOURCE=.\thread\cafthread.c
# End Source File
# End Target
# End Project

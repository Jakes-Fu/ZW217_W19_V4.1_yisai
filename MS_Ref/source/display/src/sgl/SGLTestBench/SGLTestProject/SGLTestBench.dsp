# Microsoft Developer Studio Project File - Name="SGLTestBench" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SGLTestBench - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SGLTestBench.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SGLTestBench.mak" CFG="SGLTestBench - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SGLTestBench - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SGLTestBench - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SGLTestBench - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "SGLTestBench - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\inc" /I "..\..\src" /I "...\test" /I "..\..\src\line" /I "..\win_os\src" /I "..\win_os\inc" /I "..\input" /I "..\..\..\graphics\src" /I "..\..\..\graphics\inc" /I "..\..\..\..\..\..\export\inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "ROTATE_2D_SUPPORT" /D "WIN32_TEST" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SGLTestBench - Win32 Release"
# Name "SGLTestBench - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SGLTestBench.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "sgl"

# PROP Default_Filter ""
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\inc\sgl_api.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\sgl_canvas.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\sgl_color.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\sgl_os_api.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\sgl_scene.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\sgl_shape.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\sgl_types.h
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\sgl_canvas.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sgl_scene.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sgl_shape.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sgl_shape_line.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sgl_shape_polygon.c
# End Source File
# Begin Source File

SOURCE=..\..\src\sgl_shape_texture_plane.c
# End Source File
# End Group
# End Group
# Begin Group "line"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\draw_dot.c
# End Source File
# Begin Source File

SOURCE=..\..\src\draw_line.c
# End Source File
# End Group
# Begin Group "win_os"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\win_os\src\os_api.c
# End Source File
# Begin Source File

SOURCE=..\win_os\src\win_sci_trace.c
# End Source File
# End Group
# Begin Group "graphics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\graphics\src\draw_polygon.c
# End Source File
# Begin Source File

SOURCE=..\..\..\graphics\src\rotate_image.c
# End Source File
# Begin Source File

SOURCE=..\..\..\graphics\src\rotate_software.c
# End Source File
# Begin Source File

SOURCE=..\..\..\graphics\src\s2d_polygon.c
# End Source File
# Begin Source File

SOURCE=..\..\..\graphics\src\s2d_rotate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\graphics\src\s2d_s3d_use.c
# End Source File
# Begin Source File

SOURCE=..\..\..\graphics\src\s3d_rotation.c
# End Source File
# Begin Source File

SOURCE=..\..\..\graphics\src\s3d_rotation_se.c
# End Source File
# End Group
# Begin Group "test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\test\read_res_img.c
# End Source File
# Begin Source File

SOURCE=..\test\test_bench.c
# End Source File
# Begin Source File

SOURCE=..\test\writetobmp.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

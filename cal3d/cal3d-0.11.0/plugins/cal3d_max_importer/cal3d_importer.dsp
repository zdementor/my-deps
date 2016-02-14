# Microsoft Developer Studio Project File - Name="cal3d_importer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cal3d_importer - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cal3d_importer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cal3d_importer.mak" CFG="cal3d_importer - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cal3d_importer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\obj\cal3d_max_importer_release"
# PROP Intermediate_Dir "..\..\..\..\..\obj\cal3d_max_importer_release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3D_IMPORTER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I "..\src" /I "..\src\win32" /I "..\..\src" /I "..\..\..\maxsdk\7\include" /I "..\..\..\maxsdk\7\include\cs" /D "NDEBUG" /D "_WINDLL" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAL3D_IMPORTER_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 cal3d.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib paramblk2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /def:".\cal3d_max_importer.def" /out:"..\..\..\..\..\bin\tools\plugins\max\cal3d_max_importer.dli" /libpath:"..\..\..\maxsdk\7\lib" /libpath:"..\..\..\..\..\lib\dynamic"
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "cal3d_importer - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cal3d_max_importer.cpp
# End Source File
# Begin Source File

SOURCE=.\cal3d_max_importer.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationImport.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationImportDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonImport.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonImportDesc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bones.h
# End Source File
# Begin Source File

SOURCE=.\cal3d_max_importer.h
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationImport.h
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationImportDesc.h
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonImport.h
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonImportDesc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

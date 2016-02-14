# Microsoft Developer Studio Project File - Name="libMini" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libMini - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libMini.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libMini.mak" CFG="libMini - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libMini - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libMini - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libMini - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../lib/static"
# PROP Intermediate_Dir "../../../obj/LibMiniRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /GX /Ox /Ot /Ow /Og /Oi /Oy- /Ob2 /I "." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "NOOGL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libMini - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib/static"
# PROP Intermediate_Dir "../../../obj/LibMiniDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Gm /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "NOOGL" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../lib/static\libMini_d.lib"

!ENDIF 

# Begin Target

# Name "libMini - Win32 Release"
# Name "libMini - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\database.cpp
# End Source File
# Begin Source File

SOURCE=.\datacache.cpp
# End Source File
# Begin Source File

SOURCE=.\datacalc.cpp
# End Source File
# Begin Source File

SOURCE=.\datacloud.cpp
# End Source File
# Begin Source File

SOURCE=.\dataparse.cpp
# End Source File
# Begin Source File

SOURCE=.\example.cpp
# End Source File
# Begin Source File

SOURCE=.\mini.cpp
# End Source File
# Begin Source File

SOURCE=.\minibrick.cpp
# End Source File
# Begin Source File

SOURCE=.\minicache.cpp
# End Source File
# Begin Source File

SOURCE=.\miniload.cpp
# End Source File
# Begin Source File

SOURCE=.\minilod.cpp
# End Source File
# Begin Source File

SOURCE=.\miniOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\minipoint.cpp
# End Source File
# Begin Source File

SOURCE=.\minisky.cpp
# End Source File
# Begin Source File

SOURCE=.\ministub.cpp
# End Source File
# Begin Source File

SOURCE=.\minitext.cpp
# End Source File
# Begin Source File

SOURCE=.\minitile.cpp
# End Source File
# Begin Source File

SOURCE=.\minitree.cpp
# End Source File
# Begin Source File

SOURCE=.\pnmbase.cpp
# End Source File
# Begin Source File

SOURCE=.\pnmsample.cpp
# End Source File
# Begin Source File

SOURCE=.\stubtest.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\mini.h
# End Source File
# Begin Source File

SOURCE=.\minibase.h
# End Source File
# Begin Source File

SOURCE=.\minicache.hpp
# End Source File
# Begin Source File

SOURCE=.\miniload.hpp
# End Source File
# Begin Source File

SOURCE=.\miniOGL.h
# End Source File
# Begin Source File

SOURCE=.\miniOGLP.h
# End Source File
# Begin Source File

SOURCE=.\miniP.h
# End Source File
# Begin Source File

SOURCE=.\minipoint.hpp
# End Source File
# Begin Source File

SOURCE=.\minisky.hpp
# End Source File
# Begin Source File

SOURCE=.\ministub.hpp
# End Source File
# Begin Source File

SOURCE=.\minitile.hpp
# End Source File
# Begin Source File

SOURCE=.\minitime.h
# End Source File
# Begin Source File

SOURCE=.\minitree.hpp
# End Source File
# Begin Source File

SOURCE=.\pnmbase.h
# End Source File
# Begin Source File

SOURCE=.\pnmsample.h
# End Source File
# End Group
# End Target
# End Project

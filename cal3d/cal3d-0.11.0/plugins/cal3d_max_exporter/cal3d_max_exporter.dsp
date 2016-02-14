# Microsoft Developer Studio Project File - Name="cal3d_max_exporter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cal3d_max_exporter - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cal3d_max_exporter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cal3d_max_exporter.mak" CFG="cal3d_max_exporter - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cal3d_max_exporter - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\obj\cal3d_max_exporte_release"
# PROP Intermediate_Dir "..\..\..\..\..\obj\cal3d_max_exporter_release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "." /I "..\src" /I "..\src\win32" /I "..\..\src" /I "..\..\..\maxsdk\7\include" /I "..\..\..\maxsdk\7\include\cs" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x807 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 cal3d.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\bin\tools\plugins\max\cal3d_max_exporter.dle" /libpath:"..\..\cal3d\Release" /libpath:"..\..\..\maxsdk\7\lib" /libpath:"..\..\..\..\..\lib\dynamic"
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "cal3d_max_exporter - Win32 Release"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\win32\AnimationExportSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\AnimationTimePage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BaseInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BaseMaterial.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BaseMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BaseNode.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\BoneAssignmentPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\BoneCandidate.cpp
# End Source File
# Begin Source File

SOURCE=.\cal3d_max_exporter.cpp
# End Source File
# Begin Source File

SOURCE=.\cal3d_max_exporter.def
# End Source File
# Begin Source File

SOURCE=.\cal3d_max_exporter.rc
# End Source File
# Begin Source File

SOURCE=..\src\Exporter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\LevelOfDetailPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Lodder.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MaterialCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialExportSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MaterialLibraryCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialMapDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialMapsPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialSelectionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\max2ogl.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationExport.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationExportDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationExportMaxscriptCall.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxMaterialExport.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxMaterialExportDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxMaterialExportMaxscriptCall.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxMeshExport.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxMeshExportDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxMeshExportMaxscriptCall.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxNode.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxNullView.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonExport.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonExportDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonExportMaxscriptCall.cpp
# End Source File
# Begin Source File

SOURCE=..\src\MeshCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\MeshExportSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Quadric.cpp
# End Source File
# Begin Source File

SOURCE=..\src\SkeletonCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonExportSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonFilePage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonHierarchyPage.cpp
# End Source File
# Begin Source File

SOURCE=..\src\win32\SpringSystemPage.cpp
# ADD CPP /I "..\..\cal3d_max_exporter"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\src\SubmeshCandidate.cpp
# End Source File
# Begin Source File

SOURCE=..\src\VertexCandidate.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\win32\AnimationExportSheet.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\AnimationTimePage.h
# End Source File
# Begin Source File

SOURCE=..\src\BaseInterface.h
# End Source File
# Begin Source File

SOURCE=..\src\BaseMaterial.h
# End Source File
# Begin Source File

SOURCE=..\src\BaseMesh.h
# End Source File
# Begin Source File

SOURCE=..\src\BaseNode.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\BoneAssignmentPage.h
# End Source File
# Begin Source File

SOURCE=..\src\BoneCandidate.h
# End Source File
# Begin Source File

SOURCE=.\cal3d_max_exporter.h
# End Source File
# Begin Source File

SOURCE=..\src\Exporter.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\LevelOfDetailPage.h
# End Source File
# Begin Source File

SOURCE=..\src\Lodder.h
# End Source File
# Begin Source File

SOURCE=..\src\MaterialCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialExportSheet.h
# End Source File
# Begin Source File

SOURCE=..\src\MaterialLibraryCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialMapDialog.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialMapsPage.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\MaterialSelectionPage.h
# End Source File
# Begin Source File

SOURCE=.\max2ogl.h
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationExport.h
# End Source File
# Begin Source File

SOURCE=.\MaxAnimationExportDesc.h
# End Source File
# Begin Source File

SOURCE=.\MaxInterface.h
# End Source File
# Begin Source File

SOURCE=.\MaxMaterial.h
# End Source File
# Begin Source File

SOURCE=.\MaxMaterialExport.h
# End Source File
# Begin Source File

SOURCE=.\MaxMaterialExportDesc.h
# End Source File
# Begin Source File

SOURCE=.\MaxMesh.h
# End Source File
# Begin Source File

SOURCE=.\MaxMeshExport.h
# End Source File
# Begin Source File

SOURCE=.\MaxMeshExportDesc.h
# End Source File
# Begin Source File

SOURCE=.\MaxNode.h
# End Source File
# Begin Source File

SOURCE=.\MaxNullView.h
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonExport.h
# End Source File
# Begin Source File

SOURCE=.\MaxSkeletonExportDesc.h
# End Source File
# Begin Source File

SOURCE=..\src\MeshCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\MeshExportSheet.h
# End Source File
# Begin Source File

SOURCE=..\src\Quadric.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\src\SkeletonCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonExportSheet.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonFilePage.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\SkeletonHierarchyPage.h
# End Source File
# Begin Source File

SOURCE=..\src\win32\SpringSystemPage.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\src\SubmeshCandidate.h
# End Source File
# Begin Source File

SOURCE=..\src\VertexCandidate.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cal3d.bmp
# End Source File
# Begin Source File

SOURCE=..\src\res\cal3d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cal3d_bar.bmp
# End Source File
# Begin Source File

SOURCE=..\src\res\cal3d_bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cal3d_max_exporter.rc2
# End Source File
# Begin Source File

SOURCE=.\res\type_bone.ico
# End Source File
# Begin Source File

SOURCE=..\src\res\type_bone.ico
# End Source File
# Begin Source File

SOURCE=.\res\type_dummy.ico
# End Source File
# Begin Source File

SOURCE=..\src\res\type_dummy.ico
# End Source File
# Begin Source File

SOURCE=.\res\type_other.ico
# End Source File
# Begin Source File

SOURCE=..\src\res\type_other.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\AUTHORS
# End Source File
# Begin Source File

SOURCE=.\ChangeLog
# End Source File
# Begin Source File

SOURCE=.\COPYING
# End Source File
# Begin Source File

SOURCE=.\INSTALL
# End Source File
# Begin Source File

SOURCE=.\README
# End Source File
# Begin Source File

SOURCE=.\TODO
# End Source File
# End Target
# End Project

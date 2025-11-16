# Microsoft Developer Studio Generated NMAKE File, Based on GridControl.dsp
!IF "$(CFG)" == ""
CFG=GridControl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to GridControl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "GridControl - Win32 Release" && "$(CFG)" !=\
 "GridControl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GridControl.mak" CFG="GridControl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GridControl - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GridControl - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GridControl - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\BUILD\BIN\gridctrl.dll" "$(OUTDIR)\GridControl.bsc"

!ELSE 

ALL : "..\..\BUILD\BIN\gridctrl.dll" "$(OUTDIR)\GridControl.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\gc_cell_info.obj"
	-@erase "$(INTDIR)\gc_cell_info.sbr"
	-@erase "$(INTDIR)\GridCtrl.obj"
	-@erase "$(INTDIR)\GridCtrl.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\GridControl.bsc"
	-@erase "$(OUTDIR)\gridctrl.exp"
	-@erase "$(OUTDIR)\gridctrl.lib"
	-@erase "..\..\BUILD\BIN\gridctrl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\include" /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_GCLIB_" /FR"$(INTDIR)\\"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\Release/
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GridControl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\gc_cell_info.sbr" \
	"$(INTDIR)\GridCtrl.sbr"

"$(OUTDIR)\GridControl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\gridctrl.pdb" /machine:I386 /out:"..\..\BUILD\BIN\gridctrl.dll"\
 /implib:"$(OUTDIR)\gridctrl.lib" 
LINK32_OBJS= \
	"$(INTDIR)\gc_cell_info.obj" \
	"$(INTDIR)\GridCtrl.obj"

"..\..\BUILD\BIN\gridctrl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "GridControl - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\..\BUILD\BIN\gridctrl_dbg.dll" "$(OUTDIR)\GridControl.bsc"

!ELSE 

ALL : "..\..\BUILD\BIN\gridctrl_dbg.dll" "$(OUTDIR)\GridControl.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\gc_cell_info.obj"
	-@erase "$(INTDIR)\gc_cell_info.sbr"
	-@erase "$(INTDIR)\GridCtrl.obj"
	-@erase "$(INTDIR)\GridCtrl.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\GridControl.bsc"
	-@erase "$(OUTDIR)\gridctrl_dbg.exp"
	-@erase "$(OUTDIR)\gridctrl_dbg.lib"
	-@erase "$(OUTDIR)\gridctrl_dbg.pdb"
	-@erase "..\..\BUILD\BIN\gridctrl_dbg.dll"
	-@erase "..\..\BUILD\BIN\gridctrl_dbg.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_GCLIB_"\
 /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GridControl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\gc_cell_info.sbr" \
	"$(INTDIR)\GridCtrl.sbr"

"$(OUTDIR)\GridControl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)\gridctrl_dbg.pdb" /debug /machine:I386\
 /out:"..\..\BUILD\BIN\gridctrl_dbg.dll" /implib:"$(OUTDIR)\gridctrl_dbg.lib"\
 /pdbtype:sept /libpath:"..\lib" 
LINK32_OBJS= \
	"$(INTDIR)\gc_cell_info.obj" \
	"$(INTDIR)\GridCtrl.obj"

"..\..\BUILD\BIN\gridctrl_dbg.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "GridControl - Win32 Release" || "$(CFG)" ==\
 "GridControl - Win32 Debug"
SOURCE=.\gc_cell_info.cpp

!IF  "$(CFG)" == "GridControl - Win32 Release"

DEP_CPP_GC_CE=\
	"..\Include\gc_cell_info.h"\
	"..\include\gclibapi.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gc_cell_info.obj"	"$(INTDIR)\gc_cell_info.sbr" : $(SOURCE)\
 $(DEP_CPP_GC_CE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GridControl - Win32 Debug"

DEP_CPP_GC_CE=\
	"..\Include\gc_cell_info.h"\
	"..\include\gclibapi.h"\
	".\stdafx.h"\
	

"$(INTDIR)\gc_cell_info.obj"	"$(INTDIR)\gc_cell_info.sbr" : $(SOURCE)\
 $(DEP_CPP_GC_CE) "$(INTDIR)"


!ENDIF 

SOURCE=.\GridCtrl.cpp

!IF  "$(CFG)" == "GridControl - Win32 Release"

DEP_CPP_GRIDC=\
	"..\Include\gc_cell_info.h"\
	"..\include\gclibapi.h"\
	"..\Include\GridCtrl.h"\
	".\stdafx.h"\
	

"$(INTDIR)\GridCtrl.obj"	"$(INTDIR)\GridCtrl.sbr" : $(SOURCE) $(DEP_CPP_GRIDC)\
 "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GridControl - Win32 Debug"

DEP_CPP_GRIDC=\
	"..\Include\gc_cell_info.h"\
	"..\include\gclibapi.h"\
	"..\Include\GridCtrl.h"\
	".\stdafx.h"\
	

"$(INTDIR)\GridCtrl.obj"	"$(INTDIR)\GridCtrl.sbr" : $(SOURCE) $(DEP_CPP_GRIDC)\
 "$(INTDIR)"


!ENDIF 


!ENDIF 


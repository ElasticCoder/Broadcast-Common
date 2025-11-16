# Microsoft Developer Studio Project File - Name="services" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=services - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "services.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "services.mak" CFG="services - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "services - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "services - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "services - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "services - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Tx-Suite/Common/g00_services", KCAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "services - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\..\..\Build\lib"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_DLL_IS_SERVICES_" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\Release\services.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /version:1.0 /subsystem:windows /dll /pdb:".\Release\services.pdb" /machine:I386 /out:".\..\..\Build\bin\services.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "services - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Build\lib"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_DLL_IS_SERVICES_" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /FR /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\Debug\services.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /version:1.0 /subsystem:windows /dll /pdb:".\Debug\services_dbg.pdb" /debug /machine:I386 /out:"..\..\Build\bin\services_dbg.dll"
# SUBTRACT LINK32 /pdb:none /incremental:no /force

!ELSEIF  "$(CFG)" == "services - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "services___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "services___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Build\lib"
# PROP Intermediate_Dir "DebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_DLL_IS_SERVICES_" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "..\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_DLL_IS_SERVICES_" /D "_WINDLL" /D "_AFXDLL" /D "_UNICODE" /FR /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\Debug\services.bsc"
# ADD BSC32 /nologo /o".\DebugU\services.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /version:1.0 /subsystem:windows /dll /pdb:".\Debug\services_dbg.pdb" /debug /machine:I386 /out:"..\..\Build\bin\services_dbg.dll"
# SUBTRACT BASE LINK32 /pdb:none /incremental:no /force
# ADD LINK32 /nologo /version:1.0 /subsystem:windows /dll /pdb:".\Debug\services_dbg.pdb" /debug /machine:I386 /out:"..\..\Build\bin\services_dbg Unicode.dll"
# SUBTRACT LINK32 /pdb:none /incremental:no /force

!ELSEIF  "$(CFG)" == "services - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "services___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "services___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\lib"
# PROP Intermediate_Dir "ReleaseU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_DLL_IS_SERVICES_" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "..\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_DLL_IS_SERVICES_" /D "_UNICODE" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\Release\services.bsc"
# ADD BSC32 /nologo /o".\ReleaseU\services.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /version:1.0 /subsystem:windows /dll /pdb:".\Release\services.pdb" /machine:I386 /out:".\..\..\Build\bin\services.dll"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /version:1.0 /subsystem:windows /dll /pdb:".\Release\services.pdb" /machine:I386 /out:".\..\..\Build\bin\services Unicode.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "services - Win32 Release"
# Name "services - Win32 Debug"
# Name "services - Win32 Unicode Debug"
# Name "services - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\g00_process_registry.cpp
# End Source File
# Begin Source File

SOURCE=.\g01_debug_logging.cpp
# End Source File
# Begin Source File

SOURCE=.\g01_event_logging.cpp
# End Source File
# Begin Source File

SOURCE=.\g02_datetime.cpp
# End Source File
# Begin Source File

SOURCE=.\g02_datetime_span.cpp
# End Source File
# Begin Source File

SOURCE=.\g02_find_field.cpp
# End Source File
# Begin Source File

SOURCE=.\g02_systemtime.cpp
# End Source File
# Begin Source File

SOURCE=.\g02_timeout_control.cpp
# End Source File
# Begin Source File

SOURCE=.\g02_timer.cpp
# End Source File
# Begin Source File

SOURCE=.\g02_timer_id.cpp
# End Source File
# Begin Source File

SOURCE=.\g02_timer_thread.cpp
# End Source File
# Begin Source File

SOURCE=.\g03_access_control.cpp
# End Source File
# Begin Source File

SOURCE=.\g03_heap.cpp
# End Source File
# Begin Source File

SOURCE=.\g03_partition_ctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\g03_shared_memory.cpp
# End Source File
# Begin Source File

SOURCE=.\g04_CmdLineInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\HashCString.cpp
# End Source File
# Begin Source File

SOURCE=.\initialise_dll.cpp
# End Source File
# Begin Source File

SOURCE=.\ioctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ioctrl_pc272e.cpp
# End Source File
# Begin Source File

SOURCE=.\ipc.cpp
# End Source File
# Begin Source File

SOURCE=.\ipq.cpp
# End Source File
# Begin Source File

SOURCE=.\ipq_info.cpp
# End Source File
# Begin Source File

SOURCE=.\ipq_msg_list.cpp
# End Source File
# Begin Source File

SOURCE=.\iprq.cpp
# End Source File
# Begin Source File

SOURCE=.\ipwq.cpp
# End Source File
# Begin Source File

SOURCE=.\itc.cpp
# End Source File
# Begin Source File

SOURCE=.\itc_heap_dump.cpp
# End Source File
# Begin Source File

SOURCE=.\itq.cpp
# End Source File
# Begin Source File

SOURCE=.\itq_info.cpp
# End Source File
# Begin Source File

SOURCE=.\itq_msg_list.cpp
# End Source File
# Begin Source File

SOURCE=.\itrq.cpp
# End Source File
# Begin Source File

SOURCE=.\itwq.cpp
# End Source File
# Begin Source File

SOURCE=.\services.cpp
# End Source File
# Begin Source File

SOURCE=.\services.def

!IF  "$(CFG)" == "services - Win32 Release"

!ELSEIF  "$(CFG)" == "services - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "services - Win32 Unicode Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "services - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\services_dbg.def

!IF  "$(CFG)" == "services - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "services - Win32 Debug"

!ELSEIF  "$(CFG)" == "services - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "services - Win32 Unicode Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# Begin Source File

SOURCE=.\timer_info.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\comms_event_codes.h
# End Source File
# Begin Source File

SOURCE=..\include\devioctl.h
# End Source File
# Begin Source File

SOURCE=..\include\G00.h
# End Source File
# Begin Source File

SOURCE=..\include\g00_process_registry.h
# End Source File
# Begin Source File

SOURCE=..\include\g01_debug_logging.h
# End Source File
# Begin Source File

SOURCE=..\include\g01_event_logging.h
# End Source File
# Begin Source File

SOURCE=..\include\G02.h
# End Source File
# Begin Source File

SOURCE=..\include\g02_datetime.h
# End Source File
# Begin Source File

SOURCE=..\include\g02_datetime_span.h
# End Source File
# Begin Source File

SOURCE=.\g02_find_field.h
# End Source File
# Begin Source File

SOURCE=.\g02_systemtime.h
# End Source File
# Begin Source File

SOURCE=..\include\g02_timeout_control.h
# End Source File
# Begin Source File

SOURCE=..\include\g02_timer.h
# End Source File
# Begin Source File

SOURCE=..\include\g02_timer_id.h
# End Source File
# Begin Source File

SOURCE=..\include\g03_access_control.h
# End Source File
# Begin Source File

SOURCE=.\g03_event_codes.h
# End Source File
# Begin Source File

SOURCE=..\include\g03_heap.h
# End Source File
# Begin Source File

SOURCE=..\include\g03_partition_ctrl.h
# End Source File
# Begin Source File

SOURCE=..\include\g03_shared_memory.h
# End Source File
# Begin Source File

SOURCE=..\include\g03_types.h
# End Source File
# Begin Source File

SOURCE=..\include\g04_CmdLineInfo.h
# End Source File
# Begin Source File

SOURCE=..\Include\HashCString.h
# End Source File
# Begin Source File

SOURCE=.\ioctrl.h
# End Source File
# Begin Source File

SOURCE=.\ioctrl_pc272e.h
# End Source File
# Begin Source File

SOURCE=.\ioctrl_pc272e_types.h
# End Source File
# Begin Source File

SOURCE=..\include\ipc.h
# End Source File
# Begin Source File

SOURCE=..\include\ipq.h
# End Source File
# Begin Source File

SOURCE=.\ipq_info.h
# End Source File
# Begin Source File

SOURCE=.\ipq_msg_list.h
# End Source File
# Begin Source File

SOURCE=..\include\ipq_types.h
# End Source File
# Begin Source File

SOURCE=..\include\iprq.h
# End Source File
# Begin Source File

SOURCE=..\include\ipwq.h
# End Source File
# Begin Source File

SOURCE=..\include\itc.h
# End Source File
# Begin Source File

SOURCE=.\itc_heap_info.h
# End Source File
# Begin Source File

SOURCE=..\include\itq.h
# End Source File
# Begin Source File

SOURCE=.\itq_info.h
# End Source File
# Begin Source File

SOURCE=.\itq_msg_list.h
# End Source File
# Begin Source File

SOURCE=..\include\itq_types.h
# End Source File
# Begin Source File

SOURCE=..\include\itrq.h
# End Source File
# Begin Source File

SOURCE=..\include\itwq.h
# End Source File
# Begin Source File

SOURCE="..\include\Message ID Bases.h"
# End Source File
# Begin Source File

SOURCE=..\include\portable_types.h
# End Source File
# Begin Source File

SOURCE=.\services.h
# End Source File
# Begin Source File

SOURCE=..\include\standard.h
# End Source File
# Begin Source File

SOURCE=..\include\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\thread_info.h
# End Source File
# Begin Source File

SOURCE=.\timer_info.h
# End Source File
# Begin Source File

SOURCE=.\timer_thread.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

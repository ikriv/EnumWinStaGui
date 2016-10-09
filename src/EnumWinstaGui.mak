# Microsoft Developer Studio Generated NMAKE File, Based on EnumWinstaGui.dsp
!IF "$(CFG)" == ""
CFG=EnumWinstaGui - Win32 Static MFC Debug
!MESSAGE No configuration specified. Defaulting to EnumWinstaGui - Win32 Static MFC Debug.
!ENDIF 

!IF "$(CFG)" != "EnumWinstaGui - Win32 Release" && "$(CFG)" != "EnumWinstaGui - Win32 Debug" && "$(CFG)" != "EnumWinstaGui - Win32 Static MFC Release" && "$(CFG)" != "EnumWinstaGui - Win32 Static MFC Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EnumWinstaGui.mak" CFG="EnumWinstaGui - Win32 Static MFC Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EnumWinstaGui - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EnumWinstaGui - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "EnumWinstaGui - Win32 Static MFC Release" (based on "Win32 (x86) Application")
!MESSAGE "EnumWinstaGui - Win32 Static MFC Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\EnumWinstaGui.exe"


CLEAN :
	-@erase "$(INTDIR)\ace.obj"
	-@erase "$(INTDIR)\acl.obj"
	-@erase "$(INTDIR)\apis.obj"
	-@erase "$(INTDIR)\EnumWinstaGui.obj"
	-@erase "$(INTDIR)\EnumWinstaGui.pch"
	-@erase "$(INTDIR)\EnumWinstaGui.res"
	-@erase "$(INTDIR)\EnumWinstaGuiDlg.obj"
	-@erase "$(INTDIR)\ex.obj"
	-@erase "$(INTDIR)\NewDesktopDlg.obj"
	-@erase "$(INTDIR)\NewWinstaDlg.obj"
	-@erase "$(INTDIR)\priv.obj"
	-@erase "$(INTDIR)\sd.obj"
	-@erase "$(INTDIR)\sid.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WinstaDacl.obj"
	-@erase "$(INTDIR)\WinstaSwitcher.obj"
	-@erase "$(INTDIR)\XError.obj"
	-@erase "$(OUTDIR)\EnumWinstaGui.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\EnumWinstaGui.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\EnumWinstaGui.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\EnumWinstaGui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\EnumWinstaGui.pdb" /machine:I386 /out:"$(OUTDIR)\EnumWinstaGui.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ace.obj" \
	"$(INTDIR)\acl.obj" \
	"$(INTDIR)\apis.obj" \
	"$(INTDIR)\EnumWinstaGui.obj" \
	"$(INTDIR)\EnumWinstaGuiDlg.obj" \
	"$(INTDIR)\ex.obj" \
	"$(INTDIR)\NewDesktopDlg.obj" \
	"$(INTDIR)\NewWinstaDlg.obj" \
	"$(INTDIR)\priv.obj" \
	"$(INTDIR)\sd.obj" \
	"$(INTDIR)\sid.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WinstaDacl.obj" \
	"$(INTDIR)\WinstaSwitcher.obj" \
	"$(INTDIR)\XError.obj" \
	"$(INTDIR)\EnumWinstaGui.res"

"$(OUTDIR)\EnumWinstaGui.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\EnumWinstaGui.exe" "$(OUTDIR)\EnumWinstaGui.bsc"


CLEAN :
	-@erase "$(INTDIR)\ace.obj"
	-@erase "$(INTDIR)\ace.sbr"
	-@erase "$(INTDIR)\acl.obj"
	-@erase "$(INTDIR)\acl.sbr"
	-@erase "$(INTDIR)\apis.obj"
	-@erase "$(INTDIR)\apis.sbr"
	-@erase "$(INTDIR)\EnumWinstaGui.obj"
	-@erase "$(INTDIR)\EnumWinstaGui.pch"
	-@erase "$(INTDIR)\EnumWinstaGui.res"
	-@erase "$(INTDIR)\EnumWinstaGui.sbr"
	-@erase "$(INTDIR)\EnumWinstaGuiDlg.obj"
	-@erase "$(INTDIR)\EnumWinstaGuiDlg.sbr"
	-@erase "$(INTDIR)\ex.obj"
	-@erase "$(INTDIR)\ex.sbr"
	-@erase "$(INTDIR)\NewDesktopDlg.obj"
	-@erase "$(INTDIR)\NewDesktopDlg.sbr"
	-@erase "$(INTDIR)\NewWinstaDlg.obj"
	-@erase "$(INTDIR)\NewWinstaDlg.sbr"
	-@erase "$(INTDIR)\priv.obj"
	-@erase "$(INTDIR)\priv.sbr"
	-@erase "$(INTDIR)\sd.obj"
	-@erase "$(INTDIR)\sd.sbr"
	-@erase "$(INTDIR)\sid.obj"
	-@erase "$(INTDIR)\sid.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WinstaDacl.obj"
	-@erase "$(INTDIR)\WinstaDacl.sbr"
	-@erase "$(INTDIR)\WinstaSwitcher.obj"
	-@erase "$(INTDIR)\WinstaSwitcher.sbr"
	-@erase "$(INTDIR)\XError.obj"
	-@erase "$(INTDIR)\XError.sbr"
	-@erase "$(OUTDIR)\EnumWinstaGui.bsc"
	-@erase "$(OUTDIR)\EnumWinstaGui.exe"
	-@erase "$(OUTDIR)\EnumWinstaGui.ilk"
	-@erase "$(OUTDIR)\EnumWinstaGui.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EnumWinstaGui.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\EnumWinstaGui.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\EnumWinstaGui.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ace.sbr" \
	"$(INTDIR)\acl.sbr" \
	"$(INTDIR)\apis.sbr" \
	"$(INTDIR)\EnumWinstaGui.sbr" \
	"$(INTDIR)\EnumWinstaGuiDlg.sbr" \
	"$(INTDIR)\ex.sbr" \
	"$(INTDIR)\NewDesktopDlg.sbr" \
	"$(INTDIR)\NewWinstaDlg.sbr" \
	"$(INTDIR)\priv.sbr" \
	"$(INTDIR)\sd.sbr" \
	"$(INTDIR)\sid.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\WinstaDacl.sbr" \
	"$(INTDIR)\WinstaSwitcher.sbr" \
	"$(INTDIR)\XError.sbr"

"$(OUTDIR)\EnumWinstaGui.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\EnumWinstaGui.pdb" /debug /machine:I386 /out:"$(OUTDIR)\EnumWinstaGui.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ace.obj" \
	"$(INTDIR)\acl.obj" \
	"$(INTDIR)\apis.obj" \
	"$(INTDIR)\EnumWinstaGui.obj" \
	"$(INTDIR)\EnumWinstaGuiDlg.obj" \
	"$(INTDIR)\ex.obj" \
	"$(INTDIR)\NewDesktopDlg.obj" \
	"$(INTDIR)\NewWinstaDlg.obj" \
	"$(INTDIR)\priv.obj" \
	"$(INTDIR)\sd.obj" \
	"$(INTDIR)\sid.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WinstaDacl.obj" \
	"$(INTDIR)\WinstaSwitcher.obj" \
	"$(INTDIR)\XError.obj" \
	"$(INTDIR)\EnumWinstaGui.res"

"$(OUTDIR)\EnumWinstaGui.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"

OUTDIR=.\Static_MFC_Release
INTDIR=.\Static_MFC_Release
# Begin Custom Macros
OutDir=.\Static_MFC_Release
# End Custom Macros

ALL : "$(OUTDIR)\EnumWinstaGui.exe"


CLEAN :
	-@erase "$(INTDIR)\ace.obj"
	-@erase "$(INTDIR)\acl.obj"
	-@erase "$(INTDIR)\apis.obj"
	-@erase "$(INTDIR)\EnumWinstaGui.obj"
	-@erase "$(INTDIR)\EnumWinstaGui.pch"
	-@erase "$(INTDIR)\EnumWinstaGui.res"
	-@erase "$(INTDIR)\EnumWinstaGuiDlg.obj"
	-@erase "$(INTDIR)\ex.obj"
	-@erase "$(INTDIR)\NewDesktopDlg.obj"
	-@erase "$(INTDIR)\NewWinstaDlg.obj"
	-@erase "$(INTDIR)\priv.obj"
	-@erase "$(INTDIR)\sd.obj"
	-@erase "$(INTDIR)\sid.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WinstaDacl.obj"
	-@erase "$(INTDIR)\WinstaSwitcher.obj"
	-@erase "$(INTDIR)\XError.obj"
	-@erase "$(OUTDIR)\EnumWinstaGui.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\EnumWinstaGui.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\EnumWinstaGui.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\EnumWinstaGui.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\EnumWinstaGui.pdb" /machine:I386 /out:"$(OUTDIR)\EnumWinstaGui.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ace.obj" \
	"$(INTDIR)\acl.obj" \
	"$(INTDIR)\apis.obj" \
	"$(INTDIR)\EnumWinstaGui.obj" \
	"$(INTDIR)\EnumWinstaGuiDlg.obj" \
	"$(INTDIR)\ex.obj" \
	"$(INTDIR)\NewDesktopDlg.obj" \
	"$(INTDIR)\NewWinstaDlg.obj" \
	"$(INTDIR)\priv.obj" \
	"$(INTDIR)\sd.obj" \
	"$(INTDIR)\sid.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WinstaDacl.obj" \
	"$(INTDIR)\WinstaSwitcher.obj" \
	"$(INTDIR)\XError.obj" \
	"$(INTDIR)\EnumWinstaGui.res"

"$(OUTDIR)\EnumWinstaGui.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"

OUTDIR=.\Static_MFC_Debug
INTDIR=.\Static_MFC_Debug
# Begin Custom Macros
OutDir=.\Static_MFC_Debug
# End Custom Macros

ALL : "$(OUTDIR)\EnumWinstaGui.exe" "$(OUTDIR)\EnumWinstaGui.bsc"


CLEAN :
	-@erase "$(INTDIR)\ace.obj"
	-@erase "$(INTDIR)\ace.sbr"
	-@erase "$(INTDIR)\acl.obj"
	-@erase "$(INTDIR)\acl.sbr"
	-@erase "$(INTDIR)\apis.obj"
	-@erase "$(INTDIR)\apis.sbr"
	-@erase "$(INTDIR)\EnumWinstaGui.obj"
	-@erase "$(INTDIR)\EnumWinstaGui.pch"
	-@erase "$(INTDIR)\EnumWinstaGui.res"
	-@erase "$(INTDIR)\EnumWinstaGui.sbr"
	-@erase "$(INTDIR)\EnumWinstaGuiDlg.obj"
	-@erase "$(INTDIR)\EnumWinstaGuiDlg.sbr"
	-@erase "$(INTDIR)\ex.obj"
	-@erase "$(INTDIR)\ex.sbr"
	-@erase "$(INTDIR)\NewDesktopDlg.obj"
	-@erase "$(INTDIR)\NewDesktopDlg.sbr"
	-@erase "$(INTDIR)\NewWinstaDlg.obj"
	-@erase "$(INTDIR)\NewWinstaDlg.sbr"
	-@erase "$(INTDIR)\priv.obj"
	-@erase "$(INTDIR)\priv.sbr"
	-@erase "$(INTDIR)\sd.obj"
	-@erase "$(INTDIR)\sd.sbr"
	-@erase "$(INTDIR)\sid.obj"
	-@erase "$(INTDIR)\sid.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WinstaDacl.obj"
	-@erase "$(INTDIR)\WinstaDacl.sbr"
	-@erase "$(INTDIR)\WinstaSwitcher.obj"
	-@erase "$(INTDIR)\WinstaSwitcher.sbr"
	-@erase "$(INTDIR)\XError.obj"
	-@erase "$(INTDIR)\XError.sbr"
	-@erase "$(OUTDIR)\EnumWinstaGui.bsc"
	-@erase "$(OUTDIR)\EnumWinstaGui.exe"
	-@erase "$(OUTDIR)\EnumWinstaGui.ilk"
	-@erase "$(OUTDIR)\EnumWinstaGui.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EnumWinstaGui.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x419 /fo"$(INTDIR)\EnumWinstaGui.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\EnumWinstaGui.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ace.sbr" \
	"$(INTDIR)\acl.sbr" \
	"$(INTDIR)\apis.sbr" \
	"$(INTDIR)\EnumWinstaGui.sbr" \
	"$(INTDIR)\EnumWinstaGuiDlg.sbr" \
	"$(INTDIR)\ex.sbr" \
	"$(INTDIR)\NewDesktopDlg.sbr" \
	"$(INTDIR)\NewWinstaDlg.sbr" \
	"$(INTDIR)\priv.sbr" \
	"$(INTDIR)\sd.sbr" \
	"$(INTDIR)\sid.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\WinstaDacl.sbr" \
	"$(INTDIR)\WinstaSwitcher.sbr" \
	"$(INTDIR)\XError.sbr"

"$(OUTDIR)\EnumWinstaGui.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\EnumWinstaGui.pdb" /debug /machine:I386 /out:"$(OUTDIR)\EnumWinstaGui.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ace.obj" \
	"$(INTDIR)\acl.obj" \
	"$(INTDIR)\apis.obj" \
	"$(INTDIR)\EnumWinstaGui.obj" \
	"$(INTDIR)\EnumWinstaGuiDlg.obj" \
	"$(INTDIR)\ex.obj" \
	"$(INTDIR)\NewDesktopDlg.obj" \
	"$(INTDIR)\NewWinstaDlg.obj" \
	"$(INTDIR)\priv.obj" \
	"$(INTDIR)\sd.obj" \
	"$(INTDIR)\sid.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WinstaDacl.obj" \
	"$(INTDIR)\WinstaSwitcher.obj" \
	"$(INTDIR)\XError.obj" \
	"$(INTDIR)\EnumWinstaGui.res"

"$(OUTDIR)\EnumWinstaGui.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("EnumWinstaGui.dep")
!INCLUDE "EnumWinstaGui.dep"
!ELSE 
!MESSAGE Warning: cannot find "EnumWinstaGui.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "EnumWinstaGui - Win32 Release" || "$(CFG)" == "EnumWinstaGui - Win32 Debug" || "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release" || "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"
SOURCE=.\fksec\ace.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\ace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\ace.obj"	"$(INTDIR)\ace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\ace.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\ace.obj"	"$(INTDIR)\ace.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\fksec\acl.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\acl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\acl.obj"	"$(INTDIR)\acl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\acl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\acl.obj"	"$(INTDIR)\acl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\fksec\apis.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\apis.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\apis.obj"	"$(INTDIR)\apis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\apis.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\apis.obj"	"$(INTDIR)\apis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\EnumWinstaGui.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\EnumWinstaGui.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\EnumWinstaGui.obj"	"$(INTDIR)\EnumWinstaGui.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\EnumWinstaGui.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\EnumWinstaGui.obj"	"$(INTDIR)\EnumWinstaGui.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ENDIF 

SOURCE=.\EnumWinstaGui.rc

"$(INTDIR)\EnumWinstaGui.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\EnumWinstaGuiDlg.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\EnumWinstaGuiDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\EnumWinstaGuiDlg.obj"	"$(INTDIR)\EnumWinstaGuiDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\EnumWinstaGuiDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\EnumWinstaGuiDlg.obj"	"$(INTDIR)\EnumWinstaGuiDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ENDIF 

SOURCE=.\fksec\ex.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\ex.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\ex.obj"	"$(INTDIR)\ex.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\ex.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\ex.obj"	"$(INTDIR)\ex.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\NewDesktopDlg.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\NewDesktopDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\NewDesktopDlg.obj"	"$(INTDIR)\NewDesktopDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\NewDesktopDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\NewDesktopDlg.obj"	"$(INTDIR)\NewDesktopDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ENDIF 

SOURCE=.\NewWinstaDlg.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\NewWinstaDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\NewWinstaDlg.obj"	"$(INTDIR)\NewWinstaDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\NewWinstaDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\NewWinstaDlg.obj"	"$(INTDIR)\NewWinstaDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ENDIF 

SOURCE=.\fksec\priv.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\priv.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\priv.obj"	"$(INTDIR)\priv.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\priv.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\priv.obj"	"$(INTDIR)\priv.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\fksec\sd.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\sd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\sd.obj"	"$(INTDIR)\sd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\sd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\sd.obj"	"$(INTDIR)\sd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\fksec\sid.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\sid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\sid.obj"	"$(INTDIR)\sid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\sid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\sid.obj"	"$(INTDIR)\sid.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\EnumWinstaGui.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\EnumWinstaGui.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EnumWinstaGui.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\EnumWinstaGui.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\EnumWinstaGui.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\EnumWinstaGui.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\EnumWinstaGui.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\EnumWinstaGui.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\WinstaDacl.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\WinstaDacl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\WinstaDacl.obj"	"$(INTDIR)\WinstaDacl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\WinstaDacl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\WinstaDacl.obj"	"$(INTDIR)\WinstaDacl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ENDIF 

SOURCE=.\WinstaSwitcher.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\WinstaSwitcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\WinstaSwitcher.obj"	"$(INTDIR)\WinstaSwitcher.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\WinstaSwitcher.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\WinstaSwitcher.obj"	"$(INTDIR)\WinstaSwitcher.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ENDIF 

SOURCE=.\XError.cpp

!IF  "$(CFG)" == "EnumWinstaGui - Win32 Release"


"$(INTDIR)\XError.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Debug"


"$(INTDIR)\XError.obj"	"$(INTDIR)\XError.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Release"


"$(INTDIR)\XError.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ELSEIF  "$(CFG)" == "EnumWinstaGui - Win32 Static MFC Debug"


"$(INTDIR)\XError.obj"	"$(INTDIR)\XError.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\EnumWinstaGui.pch"


!ENDIF 


!ENDIF 


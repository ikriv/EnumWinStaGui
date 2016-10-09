; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNewWinstaDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "EnumWinstaGui.h"

ClassCount=5
Class1=CEnumWinstaGuiApp
Class2=CEnumWinstaGuiDlg
Class3=CAboutDlg

ResourceCount=10
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_ENUMWINSTAGUI_DIALOG
Resource4=IDR_ROOT_MENU (English (U.S.))
Resource5=IDD_ABOUTBOX (English (U.S.))
Resource6=IDD_NEW_WINSTA (English (U.S.))
Resource7=IDD_NEW_DESKTOP (English (U.S.))
Resource8=IDR_WINSTA_MENU (English (U.S.))
Class4=CNewDesktopDlg
Resource9=IDR_DESKTOP_MENU (English (U.S.))
Class5=CNewWinstaDlg
Resource10=IDD_ENUMWINSTAGUI_DIALOG (English (U.S.))

[CLS:CEnumWinstaGuiApp]
Type=0
HeaderFile=EnumWinstaGui.h
ImplementationFile=EnumWinstaGui.cpp
Filter=N

[CLS:CEnumWinstaGuiDlg]
Type=0
HeaderFile=EnumWinstaGuiDlg.h
ImplementationFile=EnumWinstaGuiDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CEnumWinstaGuiDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=EnumWinstaGuiDlg.h
ImplementationFile=EnumWinstaGuiDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_ENUMWINSTAGUI_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CEnumWinstaGuiDlg

[DLG:IDD_ENUMWINSTAGUI_DIALOG (English (U.S.))]
Type=1
Class=CEnumWinstaGuiDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_WINSTA_TREE,SysTreeView32,1350631427
Control3=IDC_REFRESH_TREE,button,1342242816
Control4=IDC_NEW_WINSTA,button,1342242816
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_ROOT_MENU (English (U.S.))]
Type=1
Class=?
CommandCount=0

[MNU:IDR_DESKTOP_MENU (English (U.S.))]
Type=1
Class=?
CommandCount=0

[MNU:IDR_WINSTA_MENU (English (U.S.))]
Type=1
Class=?
CommandCount=0

[CLS:CNewDesktopDlg]
Type=0
HeaderFile=NewDesktopDlg.h
ImplementationFile=NewDesktopDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_DESKTOP_NAME
VirtualFilter=dWC

[DLG:IDD_NEW_DESKTOP (English (U.S.))]
Type=1
Class=CNewDesktopDlg
ControlCount=4
Control1=IDC_DESKTOP_NAME,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_NEW_WINSTA (English (U.S.))]
Type=1
Class=CNewWinstaDlg
ControlCount=4
Control1=IDC_DESKTOP_NAME,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352

[CLS:CNewWinstaDlg]
Type=0
HeaderFile=NewWinstaDlg.h
ImplementationFile=NewWinstaDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC


; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Game2.h"
LastPage=0

ClassCount=4
Class1=CGameApp
Class3=CMainFrame
Class4=CAboutDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class2=CGameView
Resource2=IDD_ABOUTBOX

[CLS:CGameApp]
Type=0
HeaderFile=Game2.h
ImplementationFile=Game2.cpp
Filter=N

[CLS:CGameView]
Type=0
HeaderFile=GameView.h
ImplementationFile=GameView.cpp
Filter=W
BaseClass=CWnd 
VirtualFilter=WC
LastObject=CGameView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T




[CLS:CAboutDlg]
Type=0
HeaderFile=Game2.cpp
ImplementationFile=Game2.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_IMAGE,static,1342177294
Control7=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_APP_ABOUT
CommandCount=2

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10


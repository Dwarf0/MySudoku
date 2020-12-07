  Unicode True
; global variable INPUT_DIR needs to be defined for this script to work
; -------------------------------
; Start
 
  !define PRODUCT "MySudoku"
  !define LAUNCHER "SudokuUI.exe"
  !define VERSION "0.1.0"
  CRCCheck On
  
 
  ; We should test if we must use an absolute path 
  !include "${NSISDIR}\Contrib\Modern UI\System.nsh"
  ; !include "${NSISDIR}\Contrib\Modern UI 2\MUI2.nsh"
 
;---------------------------------
;General
 
  OutFile "Setup-MySudoku.exe"
  ShowInstDetails "nevershow"
  ShowUninstDetails "nevershow"
  ;SetCompressor "bzip2"
 
  ; !define MUI_ICON "icon.ico"
  ; !define MUI_UNICON "icon.ico"
  ; !define MUI_SPECIALBITMAP "Bitmap.bmp"
 
;--------------------------------
;Folder selection page
 
  InstallDir "$PROGRAMFILES\${PRODUCT}"
 
 
;--------------------------------
;Modern UI Configuration
  
  ; Pages for install
  !insertmacro MUI_PAGE_WELCOME
  ; !insertmacro MUI_PAGE_LICENSE
  !insertmacro MUI_PAGE_INIT
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  ; Pages for uninstall
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
  
  
;--------------------------------
;Language
 
  !insertmacro MUI_LANGUAGE "English"
 
 
;-------------------------------- 
;Modern UI System
 
  ; !insertmacro MUI_SYSTEM 
 
 
;--------------------------------
;Data
 
  ; LicenseData "Read_me.txt"
 
 
;-------------------------------- 
;Installer Sections     
Section "install"
 
;Add files
  SetOutPath "$INSTDIR\${VERSION}"
 
  File "${INPUT_DIR}\${LAUNCHER}"
  File "${INPUT_DIR}\SudokuSolver.dll"
  File "${INPUT_DIR}\SudokuSolver.lib"
  
  !ifdef DEBUG
    File "${INPUT_DIR}\Qt5Cored.dll"
    File "${INPUT_DIR}\Qt5Guid.dll"
    File "${INPUT_DIR}\Qt5Widgetsd.dll"
  !else
    File "${INPUT_DIR}\Qt5Core.dll"
    File "${INPUT_DIR}\Qt5Gui.dll"
    File "${INPUT_DIR}\Qt5Widgets.dll"
  !endif
  
  SetOutPath "$INSTDIR\${VERSION}\platforms"
  !ifdef DEBUG
    File "${INPUT_DIR}\platforms\qwindowsd.dll"
  !else
    File "${INPUT_DIR}\platforms\qwindows.dll"
  !endif
 
;create desktop shortcut
  CreateShortCut "$DESKTOP\${PRODUCT}_${VERSION}.lnk" "$INSTDIR\${LAUNCHER}" ""
 
;create start-menu items
  ; CreateDirectory "$SMPROGRAMS\${PRODUCT}"
  ; CreateShortCut "$SMPROGRAMS\${PRODUCT}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
  ; CreateShortCut "$SMPROGRAMS\${PRODUCT}\${PRODUCT}.lnk" "$INSTDIR\${VERSION}\${LAUNCHER}" "" "$INSTDIR\${VERSION}\${LAUNCHER}" 0
 
;write uninstall information to the registry
  ; WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "DisplayName" "${MUI_PRODUCT} (remove only)"
  ; WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}" "UninstallString" "$INSTDIR\Uninstall.exe"
 
  WriteUninstaller "$INSTDIR\${VERSION}\Uninstall.exe"
 
SectionEnd
 
 
;--------------------------------    
;Uninstaller Section  
Section "Uninstall"
 ; In uninstall section, $INSTDIR corresponds to the location of the Uninstall.exe
;Delete Files 
  RMDir /r "$INSTDIR\*.*"
 
;Remove the installation directory
  RMDir "$INSTDIR\"
 
;Delete Start Menu Shortcuts
  Delete "$DESKTOP\${PRODUCT}_${VERSION}.lnk"
  ; Delete "$SMPROGRAMS\${PRODUCT}\*.*"
  ; RmDir  "$SMPROGRAMS\${PRODUCT}"
 
;Delete Uninstaller And Unistall Registry Entries
  ; DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${PRODUCT}"
  ; DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"  
  StrCpy $0 "$INSTDIR\.."
  Call un.DeleteDirIfEmpty 
  
SectionEnd
 
 
;--------------------------------    

Function un.DeleteDirIfEmpty
  FindFirst $R0 $R1 "$0\*.*"
  strcmp $R1 "." 0 NoDelete
   FindNext $R0 $R1
   strcmp $R1 ".." 0 NoDelete
    ClearErrors
    FindNext $R0 $R1
    IfErrors 0 NoDelete
     FindClose $R0
     Sleep 1000
     RMDir "$0"
  NoDelete:
   FindClose $R0
FunctionEnd
 
;eof
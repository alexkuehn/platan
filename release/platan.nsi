!define PRODUCT_NAME "Platan"
!define EXE_NAME "platan.exe"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "Alex k�hn"
!define PRODUCT_WEB_SITE "http://www.alex-kuehn.de"
!define MUI_ICON platan.ico
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${EXE_NAME}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

!include "mui.nsh"
; Zeige im Falle eines Installationsabbruchs eine Warnung
!define MUI_ABORTWARNING
; Definiere den Text der Warnung
!define MUI_ABORTWARNING_TEXT "Are you sure you want to quit the installation process?"
; Setze den "Abbrechen" Button als default
!define MUI_ABORTWARNING_CANCEL_DEFAULT


!insertmacro MUI_PAGE_WELCOME
;!insertmacro MUI_PAGE_LICENSE "pfad-zu-textdatei-mit-lizenzbedingung.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
;f�ge H�kchen zur sofortigen Ausf�hrung ein
!define MUI_FINISHPAGE_RUN "$INSTDIR\${EXE_NAME}"
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH


;!define MUI_UNICON platan.ico

; setze Sprache
!insertmacro MUI_LANGUAGE "English"

Name "${PRODUCT_NAME}"
OutFile "platan_setup.exe"
InstallDir $PROGRAMFILES\${PRODUCT_NAME}
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""

ShowInstDetails show
ShowUnInstDetails show
Section "MainSection" SEC01
   SetOutPath $INSTDIR
   SetOverwrite ifnewer
   ;kopiere die .exe-Datei ins Installationsverzeichnis
   File ${EXE_NAME}
   ;kopiere die Bibliotheken ins Installationsverzeichnis
   File mingwm10.dll
   File QtCore4.dll
   File QtGui4.dll
   File QtHelp4.dll
   File QtNetwork4.dll
   File QtSvg4.dll
   File QtXml4.dll
   File platan_de.qm
   File /r iconengines
   File /r imageformats
   ;Lege Startmen�-Eintrag an ($SMPROGRAMS entspricht dem Startmen�-Ordner)
   CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
   CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${EXE_NAME} "" "$INSTDIR\${EXE_NAME}"
   ;Erstelle Icons f�r Uninstaller
   CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe"
SectionEnd

Section -Post
   WriteUninstaller $INSTDIR\uninstall.exe
;   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayName" '"$(PRODUCT_NAME)"'
;   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
;   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${PRODUCT_NAME}"
;   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
;   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
;   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
;   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "NoModify" 1
;   WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "NoRepair" 1

SectionEnd

;Meldung bei Erfolg
Function un.onUninstSuccess
   HideWindow
   MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer"
FunctionEnd

;Meldung bei Initialisierung
Function un.onInit
   MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
   Abort
FunctionEnd

;Sektion Uninstall
Section Uninstall
   ;L�sche die Dateien aus dem Programmordner
   Delete $INSTDIR\uninstall.exe
   Delete $INSTDIR\platan.exe
   Delete $INSTDIR\mingwm10.dll
   Delete $INSTDIR\QtCore4.dll
   Delete $INSTDIR\QtGui4.dll
   Delete $INSTDIR\QtHelp4.dll
   Delete $INSTDIR\QtNetwork4.dll
   Delete $INSTDIR\QtSvg4.dll
   Delete $INSTDIR\QtXml4.dll
   Delete $INSTDIR\platan_de.qm
   Delete $INSTDIR\iconengines
   Delete $INSTDIR\imageformats
   ;Entferne den Programmordner
   RMDir  $INSTDIR
   ;L�sche alle Dateien aus dem Startmen�-Ordner
   Delete "$SMPROGRAMS\${PRODUCT_NAME}\*.*"
   ;L�sche den Startmen�-Ordner
   RMDir "$SMPROGRAMS\${PRODUCT_NAME}"
   DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
SectionEnd


call winEnv.bat

REM Ajouter un appel au build/compilation/install avant la création du setup

%MAKENSIS_EXE% /DINPUT_DIR=%SUDOKU_BIN_DIR%\Release %PROJECT_NAME%.nsi 

pause

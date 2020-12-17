
call winEnv.bat

mkdir _build
cd _build

set PATH=%PATH%;%QT_DIRS%;%OUTPUT_DIR%/SudokuSolver/Debug;%OUTPUT_DIR%/SudokuSolver/Release

%CMAKE_EXE% .. -G %VISUAL_VERSION% -DGTEST_ROOT:STRING=%GTESTLIB_DIR% -DCMAKE_INSTALL_PREFIX=%OUTPUT_DIR%

%VISUAL_EXE% %PROJECT_NAME%.sln

pause

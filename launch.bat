
mkdir _build
cd _build

set PATH=C:\Qt\5.8\msvc2015_64\plugins\platforms\;C:\Qt\5.8\msvc2015_64\bin\;%PATH%
set GTESTLIB_PATH="C:\Program Files\googletest-distribution"
set OUTPUT_PATH="C:\Sandbox\Binaries"

set VISUAL_LAUNCHER="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe"

"C:\Program Files\CMake\bin\cmake.exe" .. -G "Visual Studio 14 Win64" -DGTEST_ROOT:STRING=%GTESTLIB_PATH% -DCMAKE_INSTALL_PREFIX=%OUTPUT_PATH%
%VISUAL_LAUNCHER% MySudoku.sln

pause

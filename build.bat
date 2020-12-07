
set PATH=C:\Program Files\CMake\bin\;%PATH%
set GTESTLIB_PATH="C:\Program Files\googletest-distribution"
set OUTPUT_PATH="C:\Sandbox\Binaries"

mkdir _build
cd _build

cmake.exe .. -G "Visual Studio 14 Win64" -DGTEST_ROOT:STRING=%GTESTLIB_PATH% -DCMAKE_INSTALL_PREFIX=%OUTPUT_PATH%

pause

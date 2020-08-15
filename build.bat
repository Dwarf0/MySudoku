
set PATH=C:\Program Files\CMake\bin\;%PATH%
set GTESTLIB_PATH="C:\Program Files\googletest-distribution"

mkdir _build
cd _build

cmake.exe .. -G "Visual Studio 14 Win64" -DGTEST_ROOT:STRING=%GTESTLIB_PATH%

pause

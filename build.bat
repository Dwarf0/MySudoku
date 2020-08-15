
set PATH=C:\Program Files\CMake\bin\;%PATH%

mkdir _build
cd _build

cmake.exe .. -G "Visual Studio 14 Win64"

pause
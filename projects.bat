cd /d "%~dp0"
set PATH=%~dp0;%PATH%
::premake5 --file=projects.lua vs2015
premake5 --file=projects.lua vs2005
::start "" "C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" build\vs2005\cpp-head-only\cpp-head-only.sln 
pause
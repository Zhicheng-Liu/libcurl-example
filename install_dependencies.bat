@echo off

if not exist windows_dependencies mkdir windows_dependencies
if exist windows_dependencies/curl-7.62.0-win32-mingw exit /b
cd windows_dependencies

:: Download zip files
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/download/curl-7.62.0.zip','curl-7.62.0.zip')"

:: Unzip zip files
powershell -command "Expand-Archive curl-7.62.0.zip ."

:: Copy headers
md curl\include
xcopy /e/i curl-7.62.0\include\* curl\include

:: Build libcurl
cd curl-7.62.0/winbuild
set RTLIBCFG=static
nmake /f Makefile.vc mode=static VC=15 DEBUG=no

:: Delete useless files
del curl-7.62.0.zip

dir
cd ..

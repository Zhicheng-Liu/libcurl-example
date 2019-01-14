@echo off

if not exist windows_dependencies mkdir windows_dependencies
if exist windows_dependencies/curl-7.62.0-win32-mingw exit /b
cd windows_dependencies

:: Download zip files
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/download/curl-7.62.0.zip','curl-7.62.0.zip')"

:: Unzip zip files
powershell -command "Expand-Archive curl-7.62.0.zip ."

:: Build libcurl
md curl\lib
cd curl-7.62.0/winbuild
set RTLIBCFG=static
nmake /f Makefile.vc mode=static VC=15 DEBUG=no
cd ../../

:: Copy headers and libs
md curl\include
xcopy /e/i curl-7.62.0\include\* curl\include
md curl\lib
xcopy curl-7.62.0\builds\libcurl-vc15-x86-release-static-ipv6-sspi-winssl-obj-lib\libcurl_a.lib curl\lib
ren curl\lib\libcurl_a.lib libcurl.lib

dir /s curl-7.62.0\builds
dir /s curl-7.62.0\winbuild
dir /s curl

:: Delete useless files
del curl-7.62.0.zip

dir
cd ..

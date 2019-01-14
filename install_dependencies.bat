@echo off

if not exist windows_dependencies mkdir windows_dependencies
if exist windows_dependencies/curl-7.62.0-win32-mingw exit /b
cd windows_dependencies

:: Download zip files
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/download/curl-7.62.0.zip','curl-7.62.0.zip')"

:: Unzip zip files
powershell -command "Expand-Archive curl-7.62.0.zip ."

:: Build libcurl
cd curl-7.62.0
cd winbuild
set RTLIBCFG=static
nmake /f Makefile.vc mode=static VC=14 DEBUG=no RTLIBCFG=static
cd ../../

:: Copy headers and libs
md curl\include
xcopy /e/i curl-7.62.0\include\* curl\include
md curl\lib
xcopy curl-7.62.0\builds\libcurl-vc14-x86-release-static-ipv6-sspi-winssl-obj-lib\libcurl_a.lib curl\lib
ren curl\lib\libcurl_a.lib libcurl.lib

:: Delete useless files
del curl-7.62.0.zip

dir
cd ..

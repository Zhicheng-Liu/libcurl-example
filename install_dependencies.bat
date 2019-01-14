@echo off

if not exist windows_dependencies mkdir windows_dependencies
if exist windows_dependencies/curl-7.62.0-win32-mingw exit /b
cd windows_dependencies

:: Download zip files
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/windows/dl-7.62.0/curl-7.62.0-win32-mingw.zip','curl-7.62.0.zip')"

:: Unzip zip files
powershell -command "Expand-Archive curl-7.62.0.zip ."

:: Rename libcurl static library
ren curl-7.62.0-win32-mingw\lib\libcurl.a libcurl.lib

:: Delete useless files
del curl-7.62.0.zip

dir
cd ..

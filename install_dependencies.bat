@echo off

if not exist windows_dependencies mkdir windows_dependencies
if exist windows_dependencies/curl-7.62.0-win32-mingw exit /b
cd windows_dependencies

:: Download zip files
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/windows/dl-7.62.0/curl-7.62.0-win32-mingw.zip','curl-7.62.0.zip')"
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/windows/dl-7.62.0/brotli-1.0.7-win32-mingw.zip','brotli-1.0.7.zip')"
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/windows/dl-7.62.0/libhsts-0.1.0-win32-mingw.zip','libhsts-0.1.0.zip')"
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/windows/dl-7.62.0/libssh2-1.8.0-win32-mingw.zip','libssh2-1.8.0.zip')"
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/windows/dl-7.62.0/nghttp2-1.34.0-win32-mingw.zip','nghttp2-1.34.0.zip')"
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/windows/dl-7.62.0/openssl-1.1.1-win32-mingw.zip','openssl-1.1.1.zip')"
powershell -command "(new-object System.Net.WebClient).DownloadFile('https://curl.haxx.se/windows/dl-7.62.0/zlib-1.2.11-win32-mingw.zip','zlib-1.2.11.zip')"

:: Unzip zip files
powershell -command "Expand-Archive curl-7.62.0.zip ."
powershell -command "Expand-Archive brotli-1.0.7.zip ."
powershell -command "Expand-Archive libhsts-0.1.0.zip ."
powershell -command "Expand-Archive libssh2-1.8.0.zip ."
powershell -command "Expand-Archive nghttp2-1.34.0.zip ."
powershell -command "Expand-Archive openssl-1.1.1.zip ."
powershell -command "Expand-Archive zlib-1.2.11.zip ."

:: Copy headers
md curl\include
xcopy /e/i curl-7.62.0-win32-mingw\include\* curl\include

:: Rename libcurl static library
ren curl-7.62.0-win32-mingw\lib\libcurl.a libcurl.lib
ren brotli-1.0.7-win32-mingw\lib\libbrotlienc-static.a libbrotlienc-static.lib
ren brotli-1.0.7-win32-mingw\lib\libbrotlidec-static.a libbrotlidec-static.lib
ren brotli-1.0.7-win32-mingw\lib\libbrotlicommon-static.a libbrotlicommon-static.lib
ren libhsts-0.1.0-win32-mingw\lib\libhsts.a libhsts.lib
ren libssh2-1.8.0-win32-mingw\lib\libssh2.a libssh2.lib
ren nghttp2-1.34.0-win32-mingw\lib\libnghttp2.a libnghttp2.lib
ren openssl-1.1.1-win32-mingw\lib\libssl.a libssl.lib
ren openssl-1.1.1-win32-mingw\lib\libcrypto.a libcrypto.lib
ren zlib-1.2.11-win32-mingw\libz.a libz.lib

:: Delete useless files
del curl-7.62.0.zip
del brotli-1.0.7.zip
del libhsts-0.1.0.zip
del libssh2-1.8.0.zip
del nghttp2-1.34.0.zip
del openssl-1.1.1.zip
del zlib-1.2.11.zip

dir
cd ..

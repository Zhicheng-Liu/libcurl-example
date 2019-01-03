#!/usr/bin/env bash
set -e

if [ "$#" -eq 0 ]
then
    OS_NAME="linux"
elif [ "$#" -eq 1 ]
then
  if [[ "$1" == "--help" ]]
  then
      echo "$help_install_dependencies"
      exit
  elif [[ "$1" == "linux" ]]
  then
      OS_NAME="linux"
  elif [[ "$1" == "osx" ]]
  then
      OS_NAME="osx"
  else
    echo "$help_install_dependencies"
    exit
  fi
else
  echo "requires 1 argument at max"
  echo "$help_install_dependencies"
  exit
fi

dependencies_dir=$OS_NAME"_dependencies"

echo "creating directory $dependencies_dir"
mkdir -p $dependencies_dir && cd $dependencies_dir


dependencies_dir_abs_path=`pwd`

echo "installing openssl"
mkdir openssl
wget https://www.openssl.org/source/openssl-1.1.1a.tar.gz -O /tmp/openssl-1.1.1a.tar.gz
tar xzf /tmp/openssl-1.1.1a.tar.gz
cd openssl-1.1.1a
LIBS="-lcrypto -ldl" ./config -fPIC no-shared no-threads --prefix=$dependencies_dir_abs_path/openssl --openssldir=$dependencies_dir_abs_path/openssl
make && make install_sw && cd ..

echo "installing c-ares"
mkdir c-ares
wget https://c-ares.haxx.se/download/c-ares-1.15.0.tar.gz -O /tmp/c-ares-1.15.0.tar.gz
tar xzf /tmp/c-ares-1.15.0.tar.gz
cd c-ares-1.15.0
./configure --prefix=$dependencies_dir_abs_path/c-ares
make && make install && cd ..


echo "installing libcurl"
mkdir curl
wget https://curl.haxx.se/download/curl-7.62.0.tar.gz -O /tmp/curl-7.62.0.tar.gz
tar zxf /tmp/curl-7.62.0.tar.gz
cd curl-7.62.0
#LDFLAGS="-L$dependencies_dir_abs_path/openssl/lib" CPPFLAGS="-I$dependencies_dir_abs_path/openssl/include" LIBS="-lssl -lcrypto -ldl -lpthread" ./configure --disable-shared --enable-static --without-librtmp --without-ca-bundle --disable-ldap --without-zlib --without-libidn2 --enable-pthreads --enable-threaded-resolver --with-ssl=$dependencies_dir_abs_path/openssl --prefix=$dependencies_dir_abs_path/curl
LDFLAGS="-L$dependencies_dir_abs_path/openssl/lib -L$dependencies_dir_abs_path/c-ares/lib" CPPFLAGS="-I$dependencies_dir_abs_path/openssl/include -I$dependencies_dir_abs_path/c-ares/include" LIBS="-lssl -lcrypto -lcares -ldl" ./configure --disable-shared --enable-static --without-librtmp --without-ca-bundle --disable-ldap --without-zlib --without-libidn2 --without-nss --with-ares=$dependencies_dir_abs_path/c-ares --with-ssl=$dependencies_dir_abs_path/openssl --prefix=$dependencies_dir_abs_path/curl
make && make install && cd ..

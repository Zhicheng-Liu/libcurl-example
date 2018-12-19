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
LIBS="-lcrypto -ldl" ./config -fPIC shared no-threads --prefix=$dependencies_dir_abs_path/openssl --openssldir=$dependencies_dir_abs_path/openssl
make && make install_sw && cd ..

echo "installing libcurl"
mkdir curl
wget https://curl.haxx.se/download/curl-7.62.0.tar.gz -O /tmp/curl-7.62.0.tar.gz
tar zxf /tmp/curl-7.62.0.tar.gz
cd curl-7.62.0
LDFLAGS="-L$dependencies_dir_abs_path/openssl/lib" CPPFLAGS="-I$dependencies_dir_abs_path/openssl/include -fPIC" LIBS="-lssl -lcrypto -ldl -lpthread" ./configure --enable-shared --without-librtmp --without-ca-bundle --disable-ldap --without-zlib --without-libidn2 --enable-pthreads --enable-threaded-resolver --with-ssl=$dependencies_dir_abs_path/openssl --prefix=$dependencies_dir_abs_path/curl
make && make install && cd ..

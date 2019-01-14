cmake_minimum_required (VERSION 2.8)
project (libcurl-example CXX C)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Detect OS
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message (STATUS "Operating system detected: Linux")
  set (LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  message (STATUS "Operating system detected: OSX")
  set (OSX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message (STATUS "Operating system detected: Windows")
  set (WINDOWS TRUE)
else ()
  message (ERROR "Operating system not recognised. Expected Windows, Linux or Darwin, but found ${CMAKE_SYSTEM_NAME}")
endif ()


# Include directories
if (LINUX AND EXISTS "${PROJECT_SOURCE_DIR}/linux_dependencies")
  include_directories (linux_dependencies)
  set (EXT_LIB_PATH "${PROJECT_SOURCE_DIR}/linux_dependencies" CACHE STRING "Absolute path to libraries")
  set (ODB_PATH "${EXT_LIB_PATH}/odb-2.4.0-x86_64-linux-gnu/bin/" CACHE STRING "Path to odb binaries")
elseif (OSX AND EXISTS "${PROJECT_SOURCE_DIR}/osx_dependencies")
  include_directories (osx_dependencies)
  set (EXT_LIB_PATH "${PROJECT_SOURCE_DIR}/osx_dependencies" CACHE STRING "Absolute path to libraries")
  set (ODB_PATH "${EXT_LIB_PATH}/odb-2.4.0-i686-macosx/bin/" CACHE STRING "Path to odb binaries")
elseif (WINDOWS AND EXISTS "${PROJECT_SOURCE_DIR}/windows_dependencies")
  include_directories (${PROJECT_SOURCE_DIR}/windows_dependencies)
  set (EXT_LIB_PATH "${PROJECT_SOURCE_DIR}/windows_dependencies" CACHE STRING "Absolute path to libraries")
else ()
  set (EXT_LIB_PATH "/usr/local/bin" CACHE STRING "Absolute path to libraries")
  if (OSX)
    message(ERROR "Please run first install_dependencies.sh osx")
  elseif (WINDOWS)
    message(ERROR "Please run first install_dependencies.bat")
  endif()
endif ()

include_directories (inc)
include_directories (${EXT_LIB_PATH}/curl/include)

# Compiler and linkers flags
if (LINUX)
  # Static Build Extra Flags for Linux Static Build
  set (BUILD_SHARED_LIBS OFF)
  set (CMAKE_EXE_LINKER_FLAGS "-static")
  set (CMAKE_FIND_LIBRARY_SUFFIXES .a)
  set (CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
  set (CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
  set (CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
  set (CMAKE_SHARED_LIBRARY_CXX_FLAGS)
  set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
  set (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
  set (Boost_USE_STATIC_LIBS ON)             # only find static libs
elseif (OSX)
  # Mac build not fully static. include system libraries to be dynamic
  set (CMAKE_EXE_LINKER_FLAGS)
  set (Boost_USE_STATIC_LIBS ON)             # only find static libs
elseif (WINDOWS)
  set (Boost_USE_STATIC_LIBS ON)             # only find static libs
#  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MT")
#  set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
#  set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
#  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
  set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT") # also requires boost with runtime-link=static
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd") # also requires boost with runtime-link=static
endif ()

# Build type
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE  "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

message(STATUS "Build type is ${CMAKE_BUILD_TYPE}")
find_package (Threads REQUIRED)

# Flags specific to Unix
if (LINUX OR OSX)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unknown-pragmas") # no unknows pragmas warnings
  set (CMAKE_C_FLAGS " -DSQLITE_ENABLE_UNLOCK_NOTIFY -DSQLITE_OMIT_LOAD_EXTENSION") # Needed to statically link ODB
  find_package (Boost COMPONENTS filesystem iostreams program_options regex log thread system REQUIRED )
  include_directories (${Boost_INCLUDE_DIR} )
elseif (WINDOWS)
  add_definitions(-DCURL_STATICLIB) # Needed to statically link libcurl
  set (CMAKE_C_FLAGS " -DCURL_STATICLIB")
endif ()

# Application modules
if (LINUX OR OSX)
  set (THIRD_PARTY_LIBRARIES
    ${EXT_LIB_PATH}/curl/lib/libcurl.a
          ${EXT_LIB_PATH}/c-ares/lib/libcares.a
    ${EXT_LIB_PATH}/openssl/lib/libssl.a
    ${EXT_LIB_PATH}/openssl/lib/libcrypto.a
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
  )
elseif (WINDOWS)
  set (THIRD_PARTY_LIBRARIES
    windows_dependencies/curl/lib/libcurl
    ${CMAKE_THREAD_LIBS_INIT}
  )
endif ()

set (MOD_CURL
  inc/util/curl_easy.hpp

  src/util/curl_easy.cpp
        )
add_library(mod_curl STATIC ${MOD_CURL})
target_link_libraries(mod_curl ${THIRD_PARTY_LIBRARIES})

# Libraries to link with executables
if (LINUX OR OSX)
  set (LIBRARIES_TO_LINK
          mod_curl
          ${EXT_LIB_PATH}/curl/lib/libcurl.a
          ${EXT_LIB_PATH}/c-ares/lib/libcares.a
          ${EXT_LIB_PATH}/openssl/lib/libssl.a
          ${EXT_LIB_PATH}/openssl/lib/libcrypto.a
    ${Boost_LIBRARIES}
    dl
    ${CMAKE_THREAD_LIBS_INIT}
  )
elseif (WINDOWS)
  set (LIBRARIES_TO_LINK
    mod_curl
    windows_dependencies/curl/lib/libcurl
    ${CMAKE_THREAD_LIBS_INIT}
  )
endif ()


# Build tests
set (ALL_TESTS
  test/test_curl.cpp
)

add_executable (test_suite test/main_test.cpp ${ALL_TESTS})
target_link_libraries (test_suite ${LIBRARIES_TO_LINK})
enable_testing ()
add_test (NAME LibcurlExampleTest COMMAND test_suite)

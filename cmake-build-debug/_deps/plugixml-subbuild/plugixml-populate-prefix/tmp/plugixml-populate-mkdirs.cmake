# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-src"
  "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-build"
  "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-subbuild/plugixml-populate-prefix"
  "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-subbuild/plugixml-populate-prefix/tmp"
  "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-subbuild/plugixml-populate-prefix/src/plugixml-populate-stamp"
  "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-subbuild/plugixml-populate-prefix/src"
  "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-subbuild/plugixml-populate-prefix/src/plugixml-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-subbuild/plugixml-populate-prefix/src/plugixml-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/timur/vscode/ОП_2СЕМ/rest/cmake-build-debug/_deps/plugixml-subbuild/plugixml-populate-prefix/src/plugixml-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()

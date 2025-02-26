# Install script for directory: E:/EffekseerForCpp170e/src/EffekseerRendererDX12

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Effekseer")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/Debug/EffekseerRendererDX12.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/Release/EffekseerRendererDX12.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/MinSizeRel/EffekseerRendererDX12.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/RelWithDebInfo/EffekseerRendererDX12.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/EffekseerRendererDX12" TYPE FILE FILES
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/EffekseerRendererDX12.Base.Pre.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/EffekseerRendererDX12.Base.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/EffekseerRendererDX12.Renderer.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_model_distortion_ps.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_model_distortion_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_model_lit_ps.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_model_lit_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_model_unlit_ps.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_model_unlit_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_sprite_distortion_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_sprite_lit_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/ad_sprite_unlit_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/model_distortion_ps.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/model_distortion_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/model_lit_ps.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/model_lit_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/model_unlit_ps.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/model_unlit_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/sprite_distortion_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/sprite_lit_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRenderer/ShaderHeader/sprite_unlit_vs.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/EffekseerRendererDX12.h"
    "E:/EffekseerForCpp170e/src/EffekseerRendererDX12/../EffekseerMaterialCompiler/DirectX12/EffekseerMaterialCompilerDX12.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/EffekseerRendererDX12-config.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/EffekseerRendererDX12-config.cmake"
         "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/CMakeFiles/Export/c220ae0af1591e9e9e916bba91f25986/EffekseerRendererDX12-config.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/EffekseerRendererDX12-config-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/EffekseerRendererDX12-config.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/CMakeFiles/Export/c220ae0af1591e9e9e916bba91f25986/EffekseerRendererDX12-config.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/CMakeFiles/Export/c220ae0af1591e9e9e916bba91f25986/EffekseerRendererDX12-config-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/CMakeFiles/Export/c220ae0af1591e9e9e916bba91f25986/EffekseerRendererDX12-config-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/CMakeFiles/Export/c220ae0af1591e9e9e916bba91f25986/EffekseerRendererDX12-config-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake" TYPE FILE FILES "E:/EffekseerForCpp170e/build/src/EffekseerRendererDX12/CMakeFiles/Export/c220ae0af1591e9e9e916bba91f25986/EffekseerRendererDX12-config-release.cmake")
  endif()
endif()


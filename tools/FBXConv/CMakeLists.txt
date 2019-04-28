#
# Fbx Conv
#

cmake_minimum_required(VERSION 3.7.1)

set(FbxConvName "FbxConv")
# 프로젝트명 설정
project(${FbxConvName})

message(STATUS "Processing...")

#Extension
set(EXTDIR "${CMAKE_SOURCE_DIR}/extension")

#FBX SDK setup
set(FBXSDK "${EXTDIR}/FBXSDK/2019.2")
get_filename_component(FBXSDK_PREFIX "${FBXSDK}" ABSOLUTE)
if(NOT EXISTS ${FBXSDK_PREFIX})
	message(FATAL_ERROR "Necessary fbx sdk do not exist: " ${FBXSDK_PREFIX})
endif()

#Engine Ext
set(ENGINE_EXTDIR "${CMAKE_SOURCE_DIR}/../../extension")

#RapidJson setup
set(RAPIDJSON_INCLUDES "${ENGINE_EXTDIR}/rapidjson")
get_filename_component(RAPIDJSON_INCLUDES_PREFIX "${RAPIDJSON_INCLUDES}" ABSOLUTE)
if(NOT EXISTS ${RAPIDJSON_INCLUDES_PREFIX})
	message(FATAL_ERROR "Necessary rapid json do not exist: " ${RAPIDJSON_INCLUDES_PREFIX})
endif()
include_directories(${RAPIDJSON_INCLUDES_PREFIX})

##############
##  source  ##
##############
source_group("source"							REGULAR_EXPRESSION		 "source/*")

################
##  extension ##
################
source_group("extension\\FBXSDK"				REGULAR_EXPRESSION		 "${FBXSDK}/include/*")

# include 경로 정의
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

# 컴파일을 위해 소스파일 목록 모으기
file (GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file (GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)
file (GLOB_RECURSE FBXSDK_HPP_FILES ${FBXSDK}/include/*.h)

# 프로젝트 빌드, 컴파일될 파일명과 include 파일
add_executable(${FbxConvName} ${CPP_FILES} ${HPP_FILES} ${FBXSDK_HPP_FILES})

# 프로젝트 속성정의
set_property(TARGET ${FbxConvName} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(TARGET ${FbxConvName} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set_property(TARGET ${FbxConvName} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)

set_property(TARGET ${FbxConvName} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)
set_property(TARGET ${FbxConvName} PROPERTY WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)

# 프로젝트 빌드에 사용할 c++ 버전정의	
set_property(TARGET ${FbxConvName} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${FbxConvName} PROPERTY CXX_STANDARD_REQUIRED ON)

# 프로젝트 빌드에 사용할 c 버전정의	
set_property(TARGET ${FbxConvName} PROPERTY C_STANDARD 99)
set_property(TARGET ${FbxConvName} PROPERTY C_STANDARD_REQUIRED ON)	

# FBX SDK lib파일 이름
set(FBXSDK_LIB_LIST "")

# 각 플랫폼별 FBX SDK lib 설정
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	# FBX SDK include header
	include_directories(AFTER ${FBXSDK_PREFIX}/include/)
	
	# FBX SDK 라이브러리 경로
	set(FBXSDK_LIB_PATH ${FBXSDK_PREFIX}/lib/vs2017/$(Platform)/)

	# FBX SDK Lib names
	set(FBXSDK_LIB_NAME_LIST libfbxsdk-md.lib libxml2-md.lib zlib-md.lib)

	# FBX SDK 라이브러리 경로 combine
	foreach(x ${FBXSDK_LIB_NAME_LIST})
		set(FBX_SDK_DEBUG_LIB ${FBXSDK_LIB_PATH}debug/${x})
		set(FBX_SDK_RELEASE_LIB ${FBXSDK_LIB_PATH}release/${x})

		if(NOT EXISTS ${FBX_SDK_DEBUG_LIB})
			#message(FATAL_ERROR "[SDK LIB not exist] " ${FBX_SDK_DEBUG_LIB})
			#return()
		endif()

		if(NOT EXISTS ${FBX_SDK_RELEASE_LIB})
			#message(FATAL_ERROR "[SDK LIB not exist] " ${FBX_SDK_RELEASE_LIB})
			#return()
		endif()

		list(APPEND FBXSDK_LIB_LIST debug ${FBX_SDK_DEBUG_LIB})
		list(APPEND FBXSDK_LIB_LIST optimized ${FBX_SDK_RELEASE_LIB})
	endforeach()

	foreach(x ${FBXSDK_LIB_LIST})
		message(STATUS "[SDK LIB path] " ${x})
	endforeach()

endif()

target_link_libraries(${FbxConvName} ${FBXSDK_LIB_LIST})

#
# SweetEngine Core
#

cmake_minimum_required(VERSION 3.7.1)

set(SweetEngineName "SweetEngine0.1.1")

#AUTO_LOCATE_VULKAN - ON or OFF 설정
# ON - Vulkan SDK 경로를 CMake가 자동결정
# OFF - Vulkan SDK 경로를 수동으로 정의
option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON)

if(AUTO_LOCATE_VULKAN)
	message(STATUS "Attempting auto locate Vulkan suing CMake......")

	# CMake의 Vulkan 모듈을 사용해 경로탐색
	# 이진값 Vulkan_FOUND 반환
	# Include 경로 - 'Vulkan_INCLUDE_DIRS'
	# lib 경로 - 'Vulkan_LIBRARY'
	find_package(Vulkan)

	# ${Vulkan_INCLUDE_DIRS}로부터 VulkanSDK 경로추출
	if(NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
	endif() # (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL ""))

	if(NOT Vulkan_FOUND)
		# CMake가 라이브러리 위치를 찿는데 실패했으나
		# 'Vulkan_INCLUDE_DIRS'의 경로를 찿을수 있는 경우 여기에서 경로 추출
		message(STATUS "Failed to locate Vulkan SDK, retrying again...")

		# Vulkan 경로가 사용 가능한지 점검, 가능하지 않으면 수동모드로 변경
		if(EXISTS "${VULKAN_PATH}")
			message(STATUS "Successfully located the Vulkan SDK:${VULKAN_PATH}")
		else() # (EXISTS "${VULKAN_PATH}")
			message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
			message("and specify manual path using 'VULKAN_SDK' and 'VULKAN_VERSION' variable in the CMakeLists.txt.")
			return()
		endif() # (EXISTS "${VULKAN_PATH}")
	endif() # (NOT Vulkan_FOUND)

else() # (AUTO_LOCATE_VULKAN)
	message(STATUS "Attempting to locate Vulkan SDK using manual path.....")
	set(VULKAN_SDK "C:\VulkanSDK")
	set(VULKAN_VERSION "1.1.77.0")
	set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
	message(STATUS "Using Manual specified path: ${VULKAN_PATH}")

	# 수동 설정 경로가 존재하는지 테스트
	if(NOT EXISTS "${VULKAN_PATH}")
		message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH:${VULKAN_PATH}")
		return()
	endif() # (NOT EXISTS "${VULKAN_PATH}")
endif() # (AUTO_LOCATE_VULKAN)

# BUILD_SPV_ON_COMPILE_TIME - ON or OFF 설정
# ON - SPIRV 실시간 컴파일
# OFF - SPIRV 로 컴파일된 파일 사용
option(BUILD_SPV_ON_COMPILE_TIME "BUILD_SPV_ON_COMPILE_TIME" ON)

# 프로젝트명 설정
project(${SweetEngineName})

# c언어 전처리 정의 추가
# add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)

# VULKAN_LIB_LIST 에 vulkan-1 설정하고 아래에 target_link_libraries에서 실제 lib link
set(VULKAN_LIB_LIST "vulkan-1")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	# vulkan sdk에서 include header
	include_directories(AFTER ${VULKAN_PATH}/Include)

	# vulkan-1 라이브러리 링크
	link_directories(${VULKAN_PATH}/Lib)

	if(BUILD_SPV_ON_COMPILE_TIME)
		
		# AUTO_COMPILE_GLSL_TO_SPV 켬
		add_definitions(-DAUTO_COMPILE_GLSL_TO_SPV)

		##########################################
		#GLSL 경로를 GLSLANGDIR에 정의
		set(GLSLANGDIR "${VULKAN_PATH}/glslang")
		get_filename_component(GLSLANG_PREFIX "${GLSLANGDIR}" ABSOLUTE)
		if(NOT EXISTS ${GLSLANG_PREFIX})
			message(FATAL_ERROR "Necessary glslang components do not exist: " ${GLSLANG_PREFIX})
		endif()
		# SPIR include
		include_directories( ${GLSLANG_PREFIX} )
				
		# Lib경로에 GLSL용 아래경로들 등록
		link_directories(${GLSLANGDIR}/build/OGLCompilersDLL )
		link_directories(${GLSLANGDIR}/build/glslang/OSDependent/Windows)
		link_directories(${GLSLANGDIR}/build/glslang)
		link_directories(${GLSLANGDIR}/build/SPIRV)
		link_directories(${GLSLANGDIR}/build/hlsl)

		# GLSLANG_LIBS에 아래 이름들 등록
		set(GLSLANG_LIBS SPIRV glslang OGLCompiler OSDependent HLSL)

		# debug일때는 lib이름에 d가 붙음 (안돼는거 같음)
		# todo1 : debug 일때 d가 안붙는거 같음. 확인해보기. 
		# todo2 : 어떻게 자동으로 additional library directory에 $(Configuration) 가 붙는지 알아보기
		# 그 이름을 VULKAN_LIB_LIST에 추가
		foreach(x ${GLSLANG_LIBS})
			list(APPEND VULKAN_LIB_LIST debug ${x}d optimized ${x})
		endforeach()

		##########################################
		#SPIRV-TOOLS 경로를 SPVTOOLS에 정의
		set(SPVTOOLS "${VULKAN_PATH}/spirv-tools")
		get_filename_component(SPVTOOLS_PREFIX "${SPVTOOLS}" ABSOLUTE)
		if(NOT EXISTS ${SPVTOOLS_PREFIX})
			message(FATAL_ERROR "Necessary spirv-tools components do not exist: " ${SPVTOOLS_PREFIX})
		endif()

		# Lib경로에 SPIRV-TOOLS용 아래경로들 등록
		link_directories(${SPVTOOLS}/build/source/opt)
		link_directories(${SPVTOOLS}/build/source)

		# SPVTOOLS_LIBS에 아래 이름들 등록
		set(SPVTOOLS_LIBS SPIRV-Tools-opt SPIRV-Tools)

		# 그 이름을 VULKAN_LIB_LIST에 추가
		foreach(x ${SPVTOOLS_LIBS})
			list(APPEND VULKAN_LIB_LIST debug ${x} optimized ${x})
		endforeach()

	endif()
endif() # (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# todo:왜 win32일때만 group정하지?
#if(WIN32)
	##############
	##  source  ##
	##############
	source_group("source"									REGULAR_EXPRESSION		 "source/*")
	# General
	source_group("source\\General"							REGULAR_EXPRESSION		 "source/General/*")
	source_group("source\\General\\Math"					REGULAR_EXPRESSION		 "source/General/Math/*")
	source_group("source\\General\\RenderState"				REGULAR_EXPRESSION		 "source/General/RenderState/*")
	source_group("source\\General\\Asset"					REGULAR_EXPRESSION		 "source/General/Asset/*")
	source_group("source\\General\\Serialized"				REGULAR_EXPRESSION		 "source/General/Serialized/*")
	source_group("source\\General\\Pattern"					REGULAR_EXPRESSION		 "source/General/Pattern/*")
	source_group("source\\General\\RawData"					REGULAR_EXPRESSION		 "source/General/RawData/*")
	source_group("source\\General\\RawData"					REGULAR_EXPRESSION		 "source/General/RawData/*")
	source_group("source\\General\\Resource"				REGULAR_EXPRESSION		 "source/General/Resource/*")
	source_group("source\\General\\Json"					REGULAR_EXPRESSION		 "source/General/Json/*")
	source_group("source\\General\\Json\\DOM"				REGULAR_EXPRESSION		 "source/General/Json/DOM/*")
	source_group("source\\General\\Json\\SAX"				REGULAR_EXPRESSION		 "source/General/Json/SAX/*")
	source_group("source\\General\\Entity"					REGULAR_EXPRESSION		 "source/General/Entity/*")

	# Platform
	source_group("source\\Platform"							REGULAR_EXPRESSION		 "source/Platform/*")
	# SVk
	source_group("source\\SVk"								REGULAR_EXPRESSION		 "source/SVk/*")
	# SVk LowLayer
	source_group("source\\SVk\\LowLayer"					REGULAR_EXPRESSION		 "source/SVk/LowLayer/*")
	source_group("source\\SVk\\LowLayer\\Buffer"			REGULAR_EXPRESSION		 "source/SVk/LowLayer/Buffer/*")
	source_group("source\\SVk\\LowLayer\\Canvas"			REGULAR_EXPRESSION		 "source/SVk/LowLayer/Canvas/*")
	source_group("source\\SVk\\LowLayer\\Command"			REGULAR_EXPRESSION		 "source/SVk/LowLayer/Command/*")
	source_group("source\\SVk\\LowLayer\\Device"			REGULAR_EXPRESSION		 "source/SVk/LowLayer/Device/*")
	source_group("source\\SVk\\LowLayer\\Memory"			REGULAR_EXPRESSION		 "source/SVk/LowLayer/Memory/*")
	source_group("source\\SVk\\LowLayer\\Descriptor"		REGULAR_EXPRESSION		 "source/SVk/LowLayer/Descriptor/*")
	source_group("source\\SVk\\LowLayer\\Pipeline"			REGULAR_EXPRESSION		 "source/SVk/LowLayer/Pipeline/*")

	source_group("source\\SVk\\LowLayer\\RenderTarget"		REGULAR_EXPRESSION		 "source/SVk/LowLayer/RenderTarget/*")
	source_group("source\\SVk\\LowLayer\\Shader"			REGULAR_EXPRESSION		 "source/SVk/LowLayer/Shader/*")
	source_group("source\\SVk\\LowLayer\\Sync"				REGULAR_EXPRESSION		 "source/SVk/LowLayer/Sync/*")
	source_group("source\\SVk\\LowLayer\\Texture"			REGULAR_EXPRESSION		 "source/SVk/LowLayer/Texture/*")

	source_group("source\\SVk\\LowLayer\\Etc"				REGULAR_EXPRESSION		 "source/SVk/LowLayer/Etc/*")

	# SVk HighLayer
	source_group("source\\SVk\\HighLayer"					REGULAR_EXPRESSION		 "source/SVk/HighLayer/*")
	source_group("source\\SVk\\HighLayer\\RenderPrimitive"	REGULAR_EXPRESSION		 "source/SVk/HighLayer/RenderPrimitive/*")
	source_group("source\\SVk\\HighLayer\\Material"			REGULAR_EXPRESSION		 "source/SVk/HighLayer/Material/*")

	################
	##  resource  ##
	################
	source_group("resource"									REGULAR_EXPRESSION		 "resource/*")
	source_group("resource\\shader"							REGULAR_EXPRESSION		 "resource/shader*")
	source_group("resource\\texture"						REGULAR_EXPRESSION		 "resource/texture*")
	source_group("resource\\fbx"							REGULAR_EXPRESSION		 "resource/fbx*")


	################
	##  extension ##
	################

#endif(WIN32)

# include 경로 정의
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

# 컴파일을 위해 소스파일 목록 모으기
file (GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file (GLOB_RECURSE HPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)
file (GLOB_RECURSE SHADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/shader/*.*)
file (GLOB_RECURSE TEXTURE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/texture/*.*)
file (GLOB_RECURSE FBX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resource/fbx/*.*)


# 프로젝트 빌드, 컴파일될 파일명과 include 파일
add_executable(${SweetEngineName} ${CPP_FILES} ${HPP_FILES} ${SHADER_FILES} ${TEXTURE_FILES} ${FBX_FILES})

# 디버그, 릴리즈 라이브러리 링크
target_link_libraries(${SweetEngineName} ${VULKAN_LIB_LIST})

# 프로젝트 속성정의
set_property(TARGET ${SweetEngineName} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_property(TARGET ${SweetEngineName} PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set_property(TARGET ${SweetEngineName} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)

set_property(TARGET ${SweetEngineName} PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)
set_property(TARGET ${SweetEngineName} PROPERTY WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)

# 프로젝트 빌드에 사용할 c++ 버전정의	
set_property(TARGET ${SweetEngineName} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${SweetEngineName} PROPERTY CXX_STANDARD_REQUIRED ON)

# 프로젝트 빌드에 사용할 c 버전정의	
set_property(TARGET ${SweetEngineName} PROPERTY C_STANDARD 99)
set_property(TARGET ${SweetEngineName} PROPERTY C_STANDARD_REQUIRED ON)	

#gli setup
#message("path: " ${CMAKE_SOURCE_DIR})

set(EXTDIR "${CMAKE_SOURCE_DIR}/extension")
set(GLIINCLUDES "${EXTDIR}/gli")
get_filename_component(GLIINC_PREFIX "${GLIINCLUDES}" ABSOLUTE)
if(NOT EXISTS ${GLIINC_PREFIX})
	message(FATAL_ERROR "Necessary gli headers do not exist: " ${GLIINC_PREFIX})
endif()
include_directories(${GLIINC_PREFIX})

set(GLMINCLUDES "${EXTDIR}/gli/external/glm")
get_filename_component(GLMINC_PREFIX "${GLMINCLUDES}" ABSOLUTE)
if(NOT EXISTS ${GLMINC_PREFIX})
	message(FATAL_ERROR "Necessary glm headers do not exist: " ${GLMINC_PREFIX})
endif()
include_directories(${GLMINC_PREFIX})

#stb setup
set(STB_INCLUDES "${EXTDIR}/stb-master")
get_filename_component(STB_INC_PREFIX "${STB_INCLUDES}" ABSOLUTE)
if(NOT EXISTS ${STB_INC_PREFIX})
	message(FATAL_ERROR "Necessary stb headers do not exist: " ${STB_INC_PREFIX})
endif()
include_directories(${STB_INC_PREFIX})

#rapidjson setup
set(RAPIDJSON_INCLUDES "${EXTDIR}/rapidjson")
get_filename_component(RAPIDJSON_INC_PREFIX "${RAPIDJSON_INCLUDES}" ABSOLUTE)
if(NOT EXISTS ${RAPIDJSON_INC_PREFIX})
	message(FATAL_ERROR "Necessary rapid json headers do not exist: " ${RAPIDJSON_INC_PREFIX})
endif()
include_directories(${RAPIDJSON_INC_PREFIX})
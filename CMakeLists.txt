cmake_minimum_required(VERSION 3.16)

include(local_conf.cmake OPTIONAL)
include(FetchContent)
FetchContent_Declare(
	ccommon
	GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(ccommon)
version_from_git(
	LOG       OFF
	TIMESTAMP "%Y%m%d%H%M%S"
	)

project(BuyMeACoffe VERSION ${VERSION} LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)


qt_standard_project_setup()

qt_add_executable(buymeacoffe buymeacoffe.cpp)

qt6_add_qml_module(buymeacoffe
	URI BuyMeACoffe
	VERSION 1.0
	SOURCES
	src/monitor.cpp
	src/model.cpp
	include/model.hpp
	include/monitor.hpp
	QML_FILES
	qml/window.qml
	qml/Message_list.qml
	qml/Message_delegate.qml
	RESOURCE_PREFIX
	"/esterVtech.com/imports"
	IMPORTS MyDesigns nodeConection account
	IMPORT_PATH
	${CMAKE_BINARY_DIR}/_deps/mydesigns-build
	${CMAKE_BINARY_DIR}/_deps/nodeconection-build
	${CMAKE_BINARY_DIR}/_deps/account-build
	)

qt_add_resources(buymeacoffe "buymeacoffe.fonts"
	PREFIX "/esterVtech.com/imports/BuyMeACoffe"
	FILES
	"qml/fonts/DeliciousHandrawn-Regular.ttf"
	)

target_link_libraries(buymeacoffe PRIVATE Qt::Core Qt::Gui Qt::Qml Qt::Quick)

target_include_directories(buymeacoffe PUBLIC include)

FetchContent_Declare(
	qrCode
	GIT_REPOSITORY https://github.com/EddyTheCo/qrCode.git
	GIT_TAG v0.0.1
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(qrCode)

FetchContent_Declare(
	MyDesigns
	GIT_REPOSITORY https://github.com/EddyTheCo/MyDesigns.git
	GIT_TAG v0.1.1 
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(MyDesigns)
FetchContent_Declare(
	account
	GIT_REPOSITORY https://github.com/EddyTheCo/account.git
	GIT_TAG v0.1.1
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(account)
FetchContent_Declare(
	nodeConection
	GIT_REPOSITORY https://github.com/EddyTheCo/ConectionSettings.git
	GIT_TAG v0.1.3
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(nodeConection)

target_link_libraries(buymeacoffe PRIVATE MyDesigns account nodeConection qrCode::QtQrGen)


if(EMSCRIPTEN)
	target_compile_definitions(buymeacoffe PRIVATE USE_EMSCRIPTEN)
	add_custom_command(
		TARGET buymeacoffe
		POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/buymeacoffe.js ${CMAKE_CURRENT_BINARY_DIR}/buymeacoffe.wasm ${CMAKE_CURRENT_BINARY_DIR}/qtloader.js ${CMAKE_CURRENT_SOURCE_DIR}/wasm
		)
endif()


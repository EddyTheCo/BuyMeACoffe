cmake_minimum_required(VERSION 3.16)

include(local_conf.cmake OPTIONAL)

project(BuyMeACoffe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)
qt_standard_project_setup()

qt_add_executable(buymeacoffe buymeacoffe.cpp)
qt6_add_qml_module(buymeacoffe
	URI BuyMeACoffe 
	VERSION 1.0
	SOURCES
	src/client.cpp
       	src/model.cpp  
	include/model.hpp
       	include/client.hpp
	QML_FILES
	qml/window.qml
	qml/Message_list.qml
	qml/Message_delegate.qml
	)
target_link_libraries(buymeacoffe PRIVATE Qt6::Gui Qt6::Quick QtQrGen )

target_include_directories(buymeacoffe PUBLIC include)

if(EMSCRIPTEN)
target_compile_options(buymeacoffe PRIVATE -pthread)
target_link_options(buymeacoffe PRIVATE -pthread -sPTHREAD_POOL_SIZE=1)
endif()

include(FetchContent)
FetchContent_Declare(
	qrCode
	GIT_REPOSITORY git@github.com:EddyTheCo/qrCode.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(qrCode)

include(FetchContent)
FetchContent_Declare(
	qclient
	GIT_REPOSITORY git@github.com:EddyTheCo/Qclient-IOTA.git
	GIT_TAG version-0.2
	)
FetchContent_MakeAvailable(qclient)
FetchContent_Declare(
	qclientMQTT
	GIT_REPOSITORY git@github.com:EddyTheCo/QclientMqtt-IOTA.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(qclientMQTT)

FetchContent_Declare(
        qbech32
        GIT_REPOSITORY git@github.com:EddyTheCo/Qbech32.git
        GIT_TAG main
        )
FetchContent_MakeAvailable(qbech32)

target_link_libraries(buymeacoffe PRIVATE  qclient qclientMQTT qbech32)
if(EMSCRIPTEN)
        target_compile_definitions(buymeacoffe PRIVATE USE_EMSCRIPTEN)
        add_custom_command(
          TARGET buymeacoffe
          POST_BUILD
          COMMAND ${CMAKE_COMMAND}
          ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/buymeacoffe.js ${CMAKE_CURRENT_BINARY_DIR}/buymeacoffe.wasm ${CMAKE_CURRENT_BINARY_DIR}/buymeacoffe.worker.js  ${CMAKE_CURRENT_SOURCE_DIR}/wasm
          )
endif()

target_compile_definitions(buymeacoffe
	PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

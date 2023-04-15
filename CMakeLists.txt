cmake_minimum_required(VERSION 3.16)

include(local_conf.cmake OPTIONAL)

project(BuyMeACoffe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)



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

include(FetchContent)
FetchContent_Declare(
    qrCode
    GIT_REPOSITORY git@github.com:EddyTheCo/qrCode.git
    GIT_TAG main
)
FetchContent_MakeAvailable(qrCode)

FetchContent_Declare(
    MyDesigns
    GIT_REPOSITORY git@github.com:EddyTheCo/MyDesigns.git
    GIT_TAG main
)
FetchContent_MakeAvailable(MyDesigns)
FetchContent_Declare(
    account
    GIT_REPOSITORY git@github.com:EddyTheCo/account.git
    GIT_TAG main
)
FetchContent_MakeAvailable(account)
FetchContent_Declare(
    nodeConection
    GIT_REPOSITORY git@github.com:EddyTheCo/ConectionSettings.git
    GIT_TAG main
)
FetchContent_MakeAvailable(nodeConection)

target_link_libraries(buymeacoffe PRIVATE MyDesigns account nodeConection QtQrGen)

target_compile_definitions(buymeacoffe
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

if(EMSCRIPTEN)
    target_compile_definitions(buymeacoffe PRIVATE USE_EMSCRIPTEN)
    add_custom_command(
        TARGET buymeacoffe
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/buymeacoffe.js ${CMAKE_CURRENT_BINARY_DIR}/buymeacoffe.wasm ${CMAKE_CURRENT_BINARY_DIR}/qtloader.js ${CMAKE_CURRENT_SOURCE_DIR}/wasm
    )
endif()


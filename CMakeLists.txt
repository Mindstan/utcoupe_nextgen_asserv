cmake_minimum_required(VERSION 3.15)

project(utcoupe_nextgen_asserv)
enable_language(ASM C CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if(ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(project_options INTERFACE -ftime-trace)
    endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# Link this 'library' to use third-party ressources
add_library(project_third_party INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(ENABLE_TESTING "Enable Test Builds" ON)

target_include_directories(project_options
    INTERFACE
        include
        ${OpenCV_INCLUDE_DIRS}
)

# add third-party headers
target_include_directories(project_third_party
    INTERFACE
    third_party/single_include
)

add_executable(utcoupe_nextgen_asserv src/main.cpp)

install(TARGETS utcoupe_nextgen_asserv RUNTIME DESTINATION bin)


if(ENABLE_TESTING)
    enable_testing()
    message("Building Tests.")
    
    # Disable warnings as errors since [boost::ext]::ut emits warnings
    set(WARNINGS_AS_ERRORS OFF)
    add_library(project_tests_warnings INTERFACE)
    set_project_warnings(project_tests_warnings)
    
    add_executable(asserv_tests
        test/ut_main.cpp
        test/serial/order_tests.cpp
        test/serial/order_parser_tests.cpp
        test/utilities/static_queue_tests.cpp
    )
    target_link_libraries(asserv_tests
        PRIVATE
            project_options
            project_tests_warnings
            project_third_party
    )
endif()

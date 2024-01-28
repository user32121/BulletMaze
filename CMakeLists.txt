cmake_minimum_required(VERSION 3.16)
set(PRIMARY_TARGET BulletMaze)
project(${PRIMARY_TARGET} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
#SFML dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

set(PRIMARY_TARGET_LIB ${PRIMARY_TARGET}_lib)
add_library(${PRIMARY_TARGET_LIB} STATIC)
target_link_libraries(${PRIMARY_TARGET_LIB} PRIVATE sfml-graphics)
target_compile_features(${PRIMARY_TARGET_LIB} PUBLIC cxx_std_17)

add_executable(${PRIMARY_TARGET})
add_subdirectory(src)
target_link_libraries(${PRIMARY_TARGET} PRIVATE sfml-graphics ${PRIMARY_TARGET_LIB})

set(TEST_TARGET Test)
add_executable(${TEST_TARGET} EXCLUDE_FROM_ALL)
target_link_libraries(${TEST_TARGET} PRIVATE sfml-graphics ${PRIMARY_TARGET_LIB})
add_subdirectory(test)

#windows DLL dependency
if(WIN32)
    add_custom_command(
        TARGET ${PRIMARY_TARGET}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PRIMARY_TARGET}>
        VERBATIM)
endif()

#game resources
add_custom_command(
    TARGET ${PRIMARY_TARGET}
    COMMENT "Copy resources"
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/resources
)
add_custom_command(
    TARGET ${TEST_TARGET}
    COMMENT "Copy test resources"
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test/test_resources ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/test_resources
)

#run
add_custom_target(Run
    COMMAND ${PRIMARY_TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
)
add_custom_target(RunTest
    COMMENT "Run Tests"
    COMMAND ${TEST_TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
)

#warnings
if(MSVC)
    target_compile_options(${PRIMARY_TARGET_LIB} PUBLIC /W4)
else()
    target_compile_options(${PRIMARY_TARGET_LIB} PUBLIC -Wall -Wextra -Wpedantic)
endif()

#address sanitizer
if(ENABLE_ASAN)
    if(MSVC)
        target_compile_options(${PRIMARY_TARGET_LIB} PUBLIC /fsanitize=address /D_DISABLE_VECTOR_ANNOTATION /D_DISABLE_STRING_ANNOTATION)
    else()
        target_compile_options(${PRIMARY_TARGET_LIB} PUBLIC -fsanitize=address -g)
        target_link_options(${PRIMARY_TARGET_LIB} PUBLIC -fsanitize=address)
    endif()
endif()

install(TARGETS ${PRIMARY_TARGET})

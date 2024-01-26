cmake_minimum_required(VERSION 3.16)
set(TARGET_NAME BulletMaze)
project(${TARGET_NAME} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
#SFML dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(${TARGET_NAME})
add_subdirectory(src)
target_link_libraries(${TARGET_NAME} PRIVATE sfml-graphics)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)

set(TEST_EXECUTABLE Test)
add_executable(${TEST_EXECUTABLE})
set_property(TARGET ${TEST_EXECUTABLE} PROPERTY EXCLUDE_FROM_ALL true)
add_subdirectory(test)
add_custom_command(
    TARGET ${TEST_EXECUTABLE}
    COMMENT "Run Tests"
    POST_BUILD
    COMMAND ${TEST_EXECUTABLE}
)

#windows DLL dependency
if(WIN32)
    add_custom_command(
        TARGET ${TARGET_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${TARGET_NAME}>
        VERBATIM)
endif()

#game resources
add_custom_command(
    TARGET ${TARGET_NAME}
    COMMENT "Copy resources"
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/resources
)

#warnings
if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /Wall)
else()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

#address sanitizer
if(ENABLE_ASAN)
    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /fsanitize=address /D_DISABLE_VECTOR_ANNOTATION /D_DISABLE_STRING_ANNOTATION)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address -g)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address)
    endif()
endif()

install(TARGETS ${TARGET_NAME})

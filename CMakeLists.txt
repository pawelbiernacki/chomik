cmake_minimum_required(VERSION 3.20)
project(chomik LANGUAGES C CXX)
set(version 0.2.1)
set(HEADER_FILES config.h inc/chomik.h)

find_package(FLEX)
find_package(BISON)

flex_target(lexer src/scanner.l src/scanner.c)
bison_target(parser src/parser.y src/parser.c DEFINES_FILE src/parsertab.h)

add_flex_bison_dependency(lexer parser)


add_library(libchomik SHARED src/execute.cc src/generators.cc src/get_actual_code_value.cc
    src/get_result_replacing_placeholders.cc src/make_copy_with_replacements.cc
    src/parser_functions.cc src/parser.c src/report.cc src/scanner_functions.cc src/scanner.c
    src/various_functions.cc)

target_include_directories(libchomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}")
install(TARGETS libchomik DESTINATION lib)

add_executable(chomik src/chomik.cc)

target_include_directories(chomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}")

find_path(CHOMIK_INCLUDES chomik.h inc)

target_link_libraries(chomik libchomik)
target_include_directories(chomik PUBLIC "${PROJECT_SOURCE_DIR}" "${CHOMIK_INCLUDES}")

install(PROGRAMS chomik DESTINATION bin)



add_executable(sdl_chomik src2/sdl_chomik.cc)
target_include_directories(sdl_chomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}")
find_library(TTF_LIBRARY NAMES SDL2_ttf)
find_library(IMAGE_LIBRARY NAMES SDL2_image)
find_library(SDL2_LIBRARY NAMES SDL2)


target_link_libraries(sdl_chomik libchomik ${SDL2_LIBRARY} ${IMAGE_LIBRARY} ${TTF_LIBRARY})
install(PROGRAMS sdl_chomik DESTINATION bin)
include(CTest)
add_subdirectory(tests)


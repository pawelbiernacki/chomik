cmake_minimum_required(VERSION 3.20)
project(chomik LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(version 0.2.1)
set(PACKAGE "chomik")
set(PACKAGE_BUGREPORT "pawel.f.biernacki@gmail.com")
set(PACKAGE_NAME "chomik")
set(PACKAGE_VERSION "${version}")
set(VERSION "${version}")
set(PACKAGE_URL "https://www.chomik.tech")
set(PACKAGE_STRING "chomik ${version}")

# create config.h in build directory ${CMAKE_CURRENT_BINARY_DIR}
configure_file(cmake.config.h.in config.h)
set(HEADER_FILES config.h inc/chomik.h)

find_package(FLEX)
find_package(BISON)

flex_target(lexer src/scanner.l src/scanner.c)
bison_target(parser src/parser.y src/parser.c DEFINES_FILE src/parser.tab.h)

add_flex_bison_dependency(lexer parser)


add_library(libchomik SHARED src/execute.cc src/generators.cc src/get_actual_code_value.cc
    src/get_result_replacing_placeholders.cc src/make_copy_with_replacements.cc
    src/parser_functions.cc src/parser.c src/report.cc src/scanner_functions.cc src/scanner.c
    src/various_functions.cc)

target_include_directories(libchomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
install(TARGETS libchomik DESTINATION lib)

add_executable(chomik src/chomik.cc)

target_include_directories(chomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}")

target_link_libraries(chomik libchomik)
target_include_directories(chomik PUBLIC "${PROJECT_SOURCE_DIR}" "${CHOMIK_INCLUDES}")

install(PROGRAMS chomik DESTINATION bin)



add_executable(sdl_chomik src2/sdl_chomik.cc)
target_include_directories(sdl_chomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}")
find_library(TTF_LIBRARY NAMES SDL2_ttf)
find_library(IMAGE_LIBRARY NAMES SDL2_image)
find_library(SDL2_LIBRARY NAMES SDL2)


target_link_libraries(sdl_chomik 
    libchomik 
    ${SDL2_LIBRARY} 
	$<$<BOOL:${WIN32}>:SDL2main> # fix problem on windows "undefined symbol: WinMain"
    ${IMAGE_LIBRARY} 
    ${TTF_LIBRARY}
)
install(PROGRAMS sdl_chomik DESTINATION bin)
include(CTest)
add_subdirectory(tests)

if(NOT WIN32)
add_library(libhttpchomik SHARED src3/machine.cc src3/server.cc src3/wrapper_base.cc)
target_include_directories(libhttpchomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}")
set(datadir "${DESTINATION}/share")
target_compile_definitions(libhttpchomik PUBLIC -DDATADIR="${datadir}")
install(TARGETS libhttpchomik DESTINATION lib)



add_executable(fancy_http_chomik src4/fancy_http_chomik.cc)
target_include_directories(fancy_http_chomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}")
target_link_libraries(fancy_http_chomik libchomik libhttpchomik)
install(PROGRAMS fancy_http_chomik DESTINATION bin)
install(FILES "${PROJECT_SOURCE_DIR}/css/chomik.css" DESTINATION share/http_chomik)
install(FILES "${PROJECT_SOURCE_DIR}/image/chomik.png" DESTINATION share/http_chomik)

add_executable(http_chomik src4/http_chomik.cc)
target_include_directories(http_chomik PUBLIC "${PROJECT_SOURCE_DIR}/inc" "${PROJECT_SOURCE_DIR}")
target_link_libraries(http_chomik libchomik libhttpchomik)
install(PROGRAMS http_chomik DESTINATION bin)

endif()


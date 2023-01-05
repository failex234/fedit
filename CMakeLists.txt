include(FetchContent)
cmake_minimum_required(VERSION 3.23)
project(feditcmake C)

FetchContent_Declare(cmake_git_version_tracking
        GIT_REPOSITORY https://github.com/andrew-hardin/cmake-git-version-tracking.git
        GIT_TAG 904dbda1336ba4b9a1415a68d5f203f576b696bb
        )
FetchContent_MakeAvailable(cmake_git_version_tracking)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DENABLE_DEV")

add_executable(fedit src/fedit.h src/appendbuffer.c src/commands.c src/config.h src/delete.c src/editor.c src/errorhandling.c src/file.c src/highlight.h src/io.c src/rows.c src/terminal.c src/terminalmode.c src/highlight.c src/fedit.c)
target_link_libraries(fedit cmake_git_version_tracking)
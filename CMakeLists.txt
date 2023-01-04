cmake_minimum_required(VERSION 3.23)
project(feditcmake C)

set(CMAKE_C_STANDARD 23)

add_executable(feditcmake src/fedit.h src/appendbuffer.c src/commands.c src/config.h src/delete.c src/editor.c src/errorhandling.c src/file.c src/highlight.h src/io.c src/rows.c src/terminal.c src/terminalmode.c src/highlight.c src/fedit.c)

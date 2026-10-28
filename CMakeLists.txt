cmake_minimum_required(VERSION 3.23)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER i686-elf-g++)
set(CMAKE_ASM_COMPILER i686-elf-as)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project("icarus"
    VERSION 0.0
    DESCRIPTION "Experimental operating system built with C++ and GRUB"
    LANGUAGES CXX ASM
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/kernel)
# add_subdirectory(src/libc)

install(FILES bootloader/grub.cfg DESTINATION boot/grub)

cmake_minimum_required(VERSION 3.23)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER i686-elf-g++)
set(CMAKE_ASM_COMPILER i686-elf-gcc)
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

find_program(GRUB_MKRESCUE_EXECUTABLE grub-mkrescue REQUIRED)

set(ISO_ROOT "${CMAKE_CURRENT_BINARY_DIR}/isodir")
set(ISO_IMAGE "${CMAKE_CURRENT_BINARY_DIR}/icarus.iso")

add_custom_command(
    OUTPUT "${ISO_IMAGE}"

    COMMAND "${CMAKE_COMMAND}" --install "${CMAKE_CURRENT_BINARY_DIR}"
             --prefix "${ISO_ROOT}"
             --config "$<CONFIG>"

    COMMAND "${GRUB_MKRESCUE_EXECUTABLE}"
             -o "${ISO_IMAGE}" "${ISO_ROOT}"

    DEPENDS
         kernel
         "${CMAKE_CURRENT_SOURCE_DIR}/bootloader/grub.cfg"
         "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
         "${CMAKE_CURRENT_BINARY_DIR}/src/kernel/cmake_install.cmake"

    COMMENT "Staging boot files and generating Icarus ISO"
    VERBATIM
)

add_custom_target(iso DEPENDS "${ISO_IMAGE}")

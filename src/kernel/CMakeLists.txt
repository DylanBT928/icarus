add_executable(kernel
    boot.s
    kernel.cpp
)

target_compile_options(kernel PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-fno-exceptions;-ffreestanding;-fno-rtti>"
   "$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:Debug>>:-O0;-g>"
   "$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<NOT:$<CONFIG:Debug>>>:-O2>"
)

set(linker_script "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")

target_link_options(kernel PRIVATE
   -nostdlib
   "LINKER:-T,${linker_script}"
)

target_link_libraries(kernel PRIVATE gcc)

set_property(TARGET kernel APPEND PROPERTY LINK_DEPENDS
   "${linker_script}"
)

target_include_directories(kernel PRIVATE kernel)

set_target_properties(kernel PROPERTIES OUTPUT_NAME icarus)

install(TARGETS kernel RUNTIME DESTINATION boot)

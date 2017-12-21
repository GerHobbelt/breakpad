cmake_minimum_required(VERSION 3.6)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(breakpad)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Get the platform
set(UNIX_SYS OFF)
if(WIN32)
    if(NOT WINDOWS)
        set(WINDOWS TRUE)
    endif()
    message( STATUS "platform: Win32" )
    set(PACK_SYSTEM_PROCESSOR "windows")
    add_compile_options(-DWIN32)
elseif(UNIX AND NOT APPLE)
    set(UNIX_SYS ON)
    if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
        message( STATUS "platform: Linux" )
        set(LINUX TRUE)
    else()
        message_error("No support for *NIX non LINUX system")
    endif()
    add_compile_options(-DLINUX)
elseif(APPLE)
    if(CMAKE_SYSTEM_NAME MATCHES ".*Darwin.*")
        set(DARWIN TRUE)
    elseif(CMAKE_SYSTEM_NAME MATCHES ".*MacOS.*")
        set(MACOSX TRUE)
    endif()
    # TODO: iOS?
    message( STATUS "platform: MacOS" )
    set(PACK_SYSTEM_PROCESSOR "mac")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")
    add_compile_options(-DAPPLE)
endif()

if(LINUX)
# Processing library
set(BREAKPAD_LINUX_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/common/breakpad_types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/common/minidump_format.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/common/minidump_size.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/basic_source_line_resolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/call_stack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/code_module.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/code_modules.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/dump_context.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/dump_object.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/exploitability.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/fast_source_line_resolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/memory_region.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/microdump.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/microdump_processor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/minidump.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/minidump_processor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/process_result.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/process_state.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/proc_maps_linux.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/source_line_resolver_base.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/source_line_resolver_interface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/stack_frame.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/stack_frame_cpu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/stack_frame_symbolizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/stackwalker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/symbol_supplier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/google_breakpad/processor/system_info.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/address_map-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/address_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/basic_code_module.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/basic_code_modules.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/basic_source_line_resolver_types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/cfi_frame_info.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/contained_range_map-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/contained_range_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/disassembler_x86.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/exploitability_linux.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/exploitability_win.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/fast_source_line_resolver_types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/linked_ptr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/logging.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/map_serializers-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/map_serializers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/module_comparer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/module_factory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/module_serializer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/pathname_stripper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/postfix_evaluator-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/postfix_evaluator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/range_map-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/range_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/simple_serializer-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/simple_serializer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/simple_symbol_supplier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/windows_frame_info.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/source_line_resolver_base_types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_amd64.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_arm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_arm64.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_address_list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_mips.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_ppc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_ppc64.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_sparc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_x86.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_address_map-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_address_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_contained_range_map-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_contained_range_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_map_iterator-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_map_iterator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_map-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_range_map-inl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/static_range_map.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/symbolic_constants_win.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/tokenize.h
)
set(BREAKPAD_LINUX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/basic_code_modules.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/basic_source_line_resolver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/call_stack.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/cfi_frame_info.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/disassembler_x86.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/dump_context.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/dump_object.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/exploitability.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/exploitability_linux.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/exploitability_win.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/fast_source_line_resolver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/logging.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/microdump.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/microdump_processor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/minidump.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/minidump_processor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/module_comparer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/module_serializer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/pathname_stripper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/process_state.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/proc_maps_linux.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/simple_symbol_supplier.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/source_line_resolver_base.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stack_frame_cpu.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stack_frame_symbolizer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_amd64.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_arm.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_arm64.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_address_list.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_mips.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_ppc.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_ppc64.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_sparc.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/stackwalker_x86.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/symbolic_constants_win.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/processor/tokenize.cc
)

add_library(breakpad
    ${BREAKPAD_LINUX_HEADERS}
    ${BREAKPAD_LINUX_SOURCES}
)
set_target_properties(breakpad PROPERTIES DEBUG_POSTFIX "d")

install ( TARGETS
    breakpad
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

endif()

add_subdirectory(src/client)
add_subdirectory(src/third_party/libdisasm)
add_subdirectory(src/tools)

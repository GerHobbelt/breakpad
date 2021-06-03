cmake_minimum_required(VERSION 3.6)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.302.tar.gz"
    SHA1 "5712bfde1b45b7ad8f7825a73ccef536eff0ded2"
)

project(breakpad)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")
include(GNUInstallDirs)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "cmake/config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

set(LIBBREAKPAD_SOURCES
	src/google_breakpad/common/breakpad_types.h
    src/google_breakpad/common/minidump_format.h
    src/google_breakpad/common/minidump_size.h
    src/google_breakpad/processor/basic_source_line_resolver.h
    src/google_breakpad/processor/call_stack.h
    src/google_breakpad/processor/code_module.h
    src/google_breakpad/processor/code_modules.h
    src/google_breakpad/processor/dump_context.h
    src/google_breakpad/processor/dump_object.h
    src/google_breakpad/processor/exploitability.h
    src/google_breakpad/processor/fast_source_line_resolver.h
    src/google_breakpad/processor/memory_region.h
    src/google_breakpad/processor/microdump.h
    src/google_breakpad/processor/microdump_processor.h
    src/google_breakpad/processor/minidump.h
    src/google_breakpad/processor/minidump_processor.h
    src/google_breakpad/processor/proc_maps_linux.h
    src/google_breakpad/processor/process_result.h
    src/google_breakpad/processor/process_state.h
    src/google_breakpad/processor/source_line_resolver_base.h
    src/google_breakpad/processor/source_line_resolver_interface.h
    src/google_breakpad/processor/stack_frame.h
    src/google_breakpad/processor/stack_frame_cpu.h
    src/google_breakpad/processor/stack_frame_symbolizer.h
    src/google_breakpad/processor/stackwalker.h
    src/google_breakpad/processor/symbol_supplier.h
    src/google_breakpad/processor/system_info.h
    src/processor/address_map-inl.h
    src/processor/address_map.h
    src/processor/basic_code_module.h   
    src/processor/basic_code_modules.h    
    src/processor/basic_source_line_resolver_types.h    
    src/processor/cfi_frame_info.h
    src/processor/contained_range_map-inl.h
    src/processor/contained_range_map.h    
    src/processor/convert_old_arm64_context.h    
    src/processor/disassembler_x86.h 
    src/processor/exploitability_linux.h    
    src/processor/exploitability_win.h    
    src/processor/fast_source_line_resolver_types.h
    src/processor/linked_ptr.h   
    src/processor/logging.h
    src/processor/map_serializers-inl.h
    src/processor/map_serializers.h    
    src/processor/module_comparer.h
    src/processor/module_factory.h    
    src/processor/module_serializer.h    
    src/processor/pathname_stripper.h
    src/processor/postfix_evaluator-inl.h
    src/processor/postfix_evaluator.h    
    src/processor/range_map-inl.h
    src/processor/range_map.h
    src/processor/simple_serializer-inl.h
    src/processor/simple_serializer.h   
    src/processor/simple_symbol_supplier.h    
    src/processor/source_line_resolver_base_types.h    
    src/processor/stackwalker_address_list.h    
    src/processor/stackwalker_amd64.h   
    src/processor/stackwalker_arm.h    
    src/processor/stackwalker_arm64.h    
    src/processor/stackwalker_mips.h    
    src/processor/stackwalker_ppc.h   
    src/processor/stackwalker_ppc64.h    
    src/processor/stackwalker_sparc.h    
    src/processor/stackwalker_x86.h
    src/processor/static_address_map-inl.h
    src/processor/static_address_map.h
    src/processor/static_contained_range_map-inl.h
    src/processor/static_contained_range_map.h
    src/processor/static_map-inl.h
    src/processor/static_map.h
    src/processor/static_map_iterator-inl.h
    src/processor/static_map_iterator.h
    src/processor/static_range_map-inl.h
    src/processor/static_range_map.h    
    src/processor/symbolic_constants_win.h   
    src/processor/tokenize.h
    src/processor/windows_frame_info.h
    src/processor/basic_code_modules.cc
    src/processor/basic_source_line_resolver.cc
    src/processor/call_stack.cc
    src/processor/cfi_frame_info.cc
    src/processor/convert_old_arm64_context.cc
    src/processor/dump_context.cc
    src/processor/disassembler_x86.cc
    src/processor/dump_object.cc
    src/processor/exploitability.cc
    src/processor/exploitability_linux.cc
    src/processor/exploitability_win.cc
    src/processor/fast_source_line_resolver.cc
    src/processor/logging.cc
    src/processor/microdump.cc
    src/processor/microdump_processor.cc
    src/processor/minidump.cc
    src/processor/minidump_processor.cc
    src/processor/module_comparer.cc 
    src/processor/module_serializer.cc
    src/processor/pathname_stripper.cc
    src/processor/proc_maps_linux.cc
    src/processor/process_state.cc
    src/processor/simple_symbol_supplier.cc
    src/processor/source_line_resolver_base.cc
    src/processor/stack_frame_cpu.cc
    src/processor/stack_frame_symbolizer.cc
    src/processor/stackwalker.cc
    src/processor/stackwalker_address_list.cc
    src/processor/stackwalker_amd64.cc
    src/processor/stackwalker_arm.cc
    src/processor/stackwalker_arm64.cc
    src/processor/stackwalker_mips.cc
    src/processor/stackwalker_ppc.cc
    src/processor/stackwalker_ppc64.cc
    src/processor/stackwalker_sparc.cc
    src/processor/stackwalker_x86.cc
    src/processor/symbolic_constants_win.cc
    src/processor/tokenize.cc
)

set(LIBDISASM_SOURCES
    src/third_party/libdisasm/ia32_implicit.c
    src/third_party/libdisasm/ia32_implicit.h
    src/third_party/libdisasm/ia32_insn.c
    src/third_party/libdisasm/ia32_insn.h
    src/third_party/libdisasm/ia32_invariant.c
    src/third_party/libdisasm/ia32_invariant.h
    src/third_party/libdisasm/ia32_modrm.c
    src/third_party/libdisasm/ia32_modrm.h
    src/third_party/libdisasm/ia32_opcode_tables.c
    src/third_party/libdisasm/ia32_opcode_tables.h
    src/third_party/libdisasm/ia32_operand.c
    src/third_party/libdisasm/ia32_operand.h
    src/third_party/libdisasm/ia32_reg.c
    src/third_party/libdisasm/ia32_reg.h
    src/third_party/libdisasm/ia32_settings.c
    src/third_party/libdisasm/ia32_settings.h
    src/third_party/libdisasm/libdis.h
    src/third_party/libdisasm/qword.h
    src/third_party/libdisasm/x86_disasm.c
    src/third_party/libdisasm/x86_format.c
    src/third_party/libdisasm/x86_imm.c
    src/third_party/libdisasm/x86_imm.h
    src/third_party/libdisasm/x86_insn.c
    src/third_party/libdisasm/x86_misc.c
    src/third_party/libdisasm/x86_operand_list.c
    src/third_party/libdisasm/x86_operand_list.h
)

add_library(libbreakpad ${LIBBREAKPAD_SOURCES})
add_library(libdisasm ${LIBDISASM_SOURCES} ${LIBDISASM_SOURCES})
target_link_libraries(libbreakpad PRIVATE libdisasm)
target_include_directories(libbreakpad
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${include_install_dir}/breakpad>
)
add_definitions(
    -DNOMINMAX
    -DUNICODE
    -DWIN32_LEAN_AND_MEAN
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_SECURE_NO_DEPRECATE
    -D_CRT_NONSTDC_NO_DEPRECATE
    -D_LIBCPP_VERSION
)

if(WIN32)
    file(GLOB_RECURSE LIBBREAKPAD_CLIENT_SOURCES src/client/windows/*.cc src/common/windows/*.cc)
    find_package(DiaSDK)
    include_directories("${DIASDK_INCLUDE_DIRS}")
elseif(APPLE)
    add_definitions(-DHAVE_MACH_O_NLIST_H)
    file(GLOB_RECURSE LIBBREAKPAD_CLIENT_SOURCES src/client/mac/*.cc src/common/mac/*.cc)
else()
    add_definitions(-DHAVE_A_OUT_H)
    file(GLOB_RECURSE LIBBREAKPAD_CLIENT_SOURCES src/client/linux/*.cc src/common/linux/*.cc)
endif()
file(GLOB LIBBREAKPAD_COMMON_SOURCES src/common/*.cc src/common/*.c src/client/*.cc)
list(APPEND LIBBREAKPAD_CLIENT_SOURCES ${LIBBREAKPAD_COMMON_SOURCES})
list(FILTER LIBBREAKPAD_CLIENT_SOURCES EXCLUDE REGEX "/tests|/unittests|_test.cc$|_unittest.cc$")
if(WIN32)
    list(FILTER LIBBREAKPAD_CLIENT_SOURCES EXCLUDE REGEX "language.cc|path_helper.cc|stabs_to_module.cc|stabs_reader.cc|minidump_file_writer.cc")
endif()
add_library(libbreakpad_client ${LIBBREAKPAD_CLIENT_SOURCES})
target_include_directories(libbreakpad_client
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${include_install_dir}/breakpad>
)
if(NOT WIN32 AND NOT APPLE)
    hunter_add_package(gflags)
    hunter_add_package(glog)
    hunter_add_package(lss)
    find_package(gflags CONFIG REQUIRED)
    find_package(glog CONFIG REQUIRED)
    find_package(lss CONFIG REQUIRED)
    target_link_libraries(libbreakpad_client PUBLIC gflags glog::glog)
endif()


install(
  TARGETS libbreakpad libdisasm libbreakpad_client
  EXPORT ${targets_export_name}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION "${include_install_dir}/breakpad"
)

if(WIN32)
    set(HEADER_EXCLUDE_REGEX "/apple|/ios|/linux|/mac|/solaris|/android|/dwarf|/tests|/testdata|/unittests")
    install(
        DIRECTORY src/client src/common src/google_breakpad
        DESTINATION "${include_install_dir}/breakpad"
        FILES_MATCHING PATTERN "*.h"
        REGEX "${HEADER_EXCLUDE_REGEX}" EXCLUDE    )
elseif(APPLE)
    set(HEADER_EXCLUDE_REGEX "/apple|/ios|/linux|/windows|/solaris|/android|/dwarf|/tests|/testdata|/unittests")
    install(
        DIRECTORY src/client src/common src/google_breakpad
        DESTINATION "${include_install_dir}/breakpad"
        FILES_MATCHING PATTERN "*.h"
        REGEX "${HEADER_EXCLUDE_REGEX}" EXCLUDE    )
else()
    set(HEADER_EXCLUDE_REGEX "/apple|/ios|/client/linux/data|/client/linux/sender|/windows|/mac|/solaris|/android|/dwarf|/tests|/testdata|/unittests")
    install(
        DIRECTORY src/client src/common src/google_breakpad
        DESTINATION "${include_install_dir}/breakpad"
        FILES_MATCHING PATTERN "*.h"
        REGEX "${HEADER_EXCLUDE_REGEX}" EXCLUDE    )
endif()
 
install(
    FILES "${project_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

cmake_minimum_required(VERSION 3.6)

project(breakpad_tools)

if(LINUX)
set(DUMP_SYMS_COMMON_SOURCES
    ../common/dwarf_cfi_to_module.cc
    ../common/dwarf_cu_to_module.cc
    ../common/dwarf_line_to_module.cc
    ../common/language.cc
    ../common/module.cc
    ../common/path_helper.cc
    ../common/stabs_reader.cc
    ../common/stabs_to_module.cc
	../common/dwarf/bytereader.cc
	../common/dwarf/dwarf2diehandler.cc
	../common/dwarf/dwarf2reader.cc
	../common/dwarf/elf_reader.cc
)
set(DUMP_SYMS_LINUX_SOURCES
	../common/linux/crc32.cc
	../common/linux/dump_symbols.cc
	../common/linux/dump_symbols.h
	../common/linux/elf_symbols_to_module.cc
	../common/linux/elf_symbols_to_module.h
	../common/linux/elfutils.cc
	../common/linux/file_id.cc
	../common/linux/linux_libc_support.cc
	../common/linux/memory_mapped_file.cc
	../common/linux/safe_readlink.cc
	linux/dump_syms/dump_syms.cc
)

set(DUMP_SYMS_MACOS_SOURCES
	../common/md5.cc
	../common/mac/arch_utilities.cc
	../common/mac/dump_syms.cc
	../common/mac/dump_syms.h
	../common/mac/file_id.cc
	../common/mac/file_id.h
	../common/mac/macho_id.cc
	../common/mac/macho_id.h
	../common/mac/macho_reader.cc
	../common/mac/macho_reader.h
	../common/mac/macho_utilities.cc
	../common/mac/macho_utilities.h
	../common/mac/macho_walker.cc
	../common/mac/macho_walker.h
    mac/dump_syms/dump_syms_tool.cc
)
    
add_executable(dump_syms
    ${DUMP_SYMS_COMMON_SOURCES}
    ${DUMP_SYMS_LINUX_SOURCES}
)
target_link_libraries(dump_syms
)
target_compile_definitions(dump_syms
    PRIVATE HAVE_MACH_O_NLIST_H
)
target_include_directories(dump_syms
    PRIVATE ../third_party/mac_headers
)
add_executable(dump_syms_mac
    ${DUMP_SYMS_COMMON_SOURCES}
    ${DUMP_SYMS_MACOS_SOURCES}
)
target_link_libraries(dump_syms_mac
)
target_compile_definitions(dump_syms_mac
    PRIVATE HAVE_MACH_O_NLIST_H
)
target_include_directories(dump_syms_mac
    PRIVATE ../third_party/mac_headers
)

set(SYM_UPLOAD_SOURCES
    ../common/linux/http_upload.cc
    ../common/linux/http_upload.h
    ../common/linux/symbol_upload.cc
    ../common/linux/symbol_upload.h
    linux/symupload/sym_upload.cc
)
add_executable(sym_upload
	${SYM_UPLOAD_SOURCES}
)
target_link_libraries(sym_upload
    breakpad_client
    dl
)

#install(TARGETS dump_syms DESTINATION . COMPONENT Runtime)

install(TARGETS
    dump_syms sym_upload dump_syms_mac
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
elseif(WIN32)
# Windows tools
set(MS_SYMBOL_SOURCES
    windows/converter/ms_symbol_server_converter.cc
    windows/converter/ms_symbol_server_converter.h
)
set(COMMON_TOOL_WINDOWS_SOURCES
    ../common/windows/dia_util.cc
    ../common/windows/dia_util.h
    ../common/windows/guid_string.cc
    ../common/windows/guid_string.h
    ../common/windows/http_upload.cc
    ../common/windows/http_upload.h
    ../common/windows/omap.cc
    ../common/windows/omap.h
    ../common/windows/omap_internal.h
    ../common/windows/pdb_source_line_writer.cc
    ../common/windows/pdb_source_line_writer.h
    ../common/windows/string_utils.cc
    ../common/windows/string_utils-inl.h
)
set(DUMP_SYMS_WINDOWS_SOURCES
    windows/dump_syms/dump_syms.cc
    ../third_party/dia_sdk/lib/dia2_guids.c
)
set(SYM_UPLOAD_WINDOWS_SOURCES
    windows/symupload/symupload.cc
    ../third_party/dia_sdk/lib/dia2_guids.c
)

add_executable(dump_syms
    ${MS_SYMBOL_SOURCES}
    ${COMMON_TOOL_WINDOWS_SOURCES}
	${DUMP_SYMS_WINDOWS_SOURCES}
)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wattributes -fpermissive -Wconversion-null -municode)
endif()

target_compile_definitions(dump_syms
    PRIVATE UNICODE
    PRIVATE _UNICODE
    PRIVATE _WIN32_WINNT=0x0600
    PRIVATE WINVER=0x0600
    PRIVATE WIN32
    PRIVATE _WINDOWS
    PRIVATE NOMINMAX
    PRIVATE WIN32_LEAN_AND_MEAN
 #   PRIVATE _HAS_EXCEPTIONS=0
 #   PRIVATE _CRT_RAND_S
 #   PRIVATE CERT_CHAIN_PARA_HAS_EXTRA_FIELDS
 #   PRIVATE _SECURE_ATL
 #   PRIVATE _HAS_TR1=0
 #   PRIVATE RC_INVOKED
 #   PRIVATE _ATL_ENABLE_PTM_WARNING
)

target_include_directories(dump_syms
    PRIVATE ../third_party/dia_sdk/include
    PRIVATE ../third_party/atl_compat
)
if(CMAKE_SIZEOF_VOID_P MATCHES "8")
target_link_libraries(dump_syms
    imagehlp
    version
    wininet
    #-Wl,--start-group uuid ${CMAKE_SOURCE_DIR}/src/third_party/dia_sdk/lib/amd64/diaguids.lib -Wl,--end-group
    -municode
)
else()
target_link_libraries(dump_syms
    imagehlp
    version
    wininet
    #-Wl,--start-group uuid ${CMAKE_SOURCE_DIR}/src/third_party/dia_sdk/lib/diaguids.lib -Wl,--end-group
    -municode
)
endif()

add_executable(sym_upload
    ${COMMON_TOOL_WINDOWS_SOURCES}
	${SYM_UPLOAD_WINDOWS_SOURCES}
)
target_compile_definitions(sym_upload
    PRIVATE UNICODE
    PRIVATE _UNICODE
    PRIVATE _WIN32_WINNT=0x0600
    PRIVATE WINVER=0x0600
    PRIVATE WIN32
    PRIVATE _WINDOWS
    PRIVATE NOMINMAX
    PRIVATE WIN32_LEAN_AND_MEAN
 #   PRIVATE _HAS_EXCEPTIONS=0
 #   PRIVATE _CRT_RAND_S
 #   PRIVATE CERT_CHAIN_PARA_HAS_EXTRA_FIELDS
 #   PRIVATE _SECURE_ATL
 #   PRIVATE _HAS_TR1=0
 #   PRIVATE RC_INVOKED
 #   PRIVATE _ATL_ENABLE_PTM_WARNING
)

target_include_directories(sym_upload
    PRIVATE ../third_party/dia_sdk/include
    PRIVATE ../third_party/atl_compat
)
if(CMAKE_SIZEOF_VOID_P MATCHES "8")
target_link_libraries(sym_upload
    imagehlp
    wininet
    version
    #-Wl,--start-group uuid ${CMAKE_SOURCE_DIR}/src/third_party/dia_sdk/lib/amd64/diaguids.lib -Wl,--end-group
    -municode
)
else()
target_link_libraries(sym_upload
    imagehlp
    wininet
    version
    #-Wl,--start-group uuid ${CMAKE_SOURCE_DIR}/src/third_party/dia_sdk/lib/diaguids.lib -Wl,--end-group
    -municode
)
endif()

install(TARGETS
    dump_syms sym_upload
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

elseif(APPLE)
endif()
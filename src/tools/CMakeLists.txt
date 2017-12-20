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
elseif(APPLE)
endif()
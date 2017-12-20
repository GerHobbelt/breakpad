cmake_minimum_required(VERSION 3.6)

project(breakpad_tools)

set(DUMP_SYMS_SOURCES
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

add_executable(dump_syms
	${DUMP_SYMS_SOURCES}
)
target_link_libraries(dump_syms
    breakpad_client
)
target_compile_definitions(dump_syms
    PRIVATE HAVE_MACH_O_NLIST_H
)
target_include_directories(dump_syms
    PRIVATE ../third_party/mac_headers
)

#install(TARGETS dump_syms DESTINATION . COMPONENT Runtime)

install(TARGETS
    dump_syms
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

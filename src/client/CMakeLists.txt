cmake_minimum_required(VERSION 3.6)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(breakpad_client)

# Common to both project
set(BREAKPAD_COMMON_HEADERS
    ../common/convert_UTF.h
    ../common/md5.h
    ../common/string_conversion.h
)
source_group( "Header Files\\Breakpad_Common" FILES ${BREAKPAD_COMMON_HEADERS} )
set(BREAKPAD_COMMON_SOURCES
    ../common/convert_UTF.c
    ../common/md5.cc
    ../common/string_conversion.cc
)
source_group( "Source Files\\Breakpad_Common" FILES ${BREAKPAD_COMMON_SOURCES} )

# Common to all client
set(BREAKPAD_CLIENT_COMMON_HEADERS
    minidump_file_writer-inl.h
    minidump_file_writer.h
)
source_group( "Header Files\\Breakpad_Client_Common" FILES ${BREAKPAD_CLIENT_COMMON_HEADERS} )

set(BREAKPAD_CLIENT_COMMON_SOURCES
    minidump_file_writer.cc
)
source_group( "Source Files\\Breakpad_Client_Common" FILES ${BREAKPAD_CLIENT_COMMON_SOURCES} )

# Common to all linux projects
set(BREAKPAD_COMMON_LINUX_HEADERS
    ../common/linux/elfutils.h
    ../common/linux/file_id.h
	../common/linux/guid_creator.h
)
source_group( "Header Files\\Breakpad_Linux_Common" FILES ${BREAKPAD_COMMON_LINUX_HEADERS} )
set(BREAKPAD_COMMON_LINUX_SOURCES
    ../common/linux/elf_core_dump.cc
    ../common/linux/elfutils.cc
    ../common/linux/file_id.cc
    ../common/linux/guid_creator.cc
	../common/linux/linux_libc_support.cc
	../common/linux/memory_mapped_file.cc
	../common/linux/safe_readlink.cc
)
source_group( "Source Files\\Breakpad_Linux_Common" FILES ${BREAKPAD_COMMON_LINUX_SOURCES} )

# Linux client 
set(BREAKPAD_CLIENT_LINUX_HANDLER_HEADERS
    linux/handler/exception_handler.h
    linux/handler/minidump_descriptor.h
)
set(BREAKPAD_CLIENT_LINUX_LOG_HEADERS
    linux/log/log.h
)
set(BREAKPAD_CLIENT_LINUX_DUMPWRITER_HEADERS
    linux/microdump_writer/microdump_writer.h
)
set(BREAKPAD_CLIENT_LINUX_HEADERS
    ${BREAKPAD_CLIENT_LINUX_HANDLER_HEADERS}
    ${BREAKPAD_CLIENT_LINUX_LOG_HEADERS}
    ${BREAKPAD_CLIENT_LINUX_DUMPWRITER_HEADERS}
)
source_group( "Header Files\\Breakpad_Client_Linux" FILES ${BREAKPAD_CLIENT_LINUX_HEADERS} )
set(BREAKPAD_CLIENT_LINUX_SOURCES
    linux/crash_generation/crash_generation_client.cc
	linux/crash_generation/crash_generation_server.cc
	linux/dump_writer_common/thread_info.cc
	linux/dump_writer_common/ucontext_reader.cc
	linux/handler/exception_handler.cc
	linux/handler/minidump_descriptor.cc
	linux/log/log.cc
	linux/microdump_writer/microdump_writer.cc
	linux/minidump_writer/linux_core_dumper.cc
	linux/minidump_writer/linux_dumper.cc
	linux/minidump_writer/linux_ptrace_dumper.cc
	linux/minidump_writer/minidump_writer.cc
)
source_group( "Source Files\\Breakpad_Client_Linux" FILES ${BREAKPAD_CLIENT_LINUX_SOURCES} )

add_library(breakpad_client
    ${BREAKPAD_COMMON_HEADERS}
    ${BREAKPAD_COMMON_SOURCES}
    ${BREAKPAD_CLIENT_COMMON_HEADERS}
    ${BREAKPAD_CLIENT_COMMON_SOURCES}
    ${BREAKPAD_COMMON_LINUX_HEADERS}
    ${BREAKPAD_COMMON_LINUX_SOURCES}
    ${BREAKPAD_CLIENT_LINUX_HEADERS}
    ${BREAKPAD_CLIENT_LINUX_SOURCES}
)
set_target_properties(breakpad_client PROPERTIES DEBUG_POSTFIX "d")

install ( FILES
	${BREAKPAD_CLIENT_COMMON_HEADERS}
	DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/breakpad/common"
)
install ( FILES
	${BREAKPAD_COMMON_LINUX_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/common/linux"
)
install ( FILES
	${BREAKPAD_CLIENT_LINUX_HANDLER_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/client/linux/handler"
)
install ( FILES
	${BREAKPAD_CLIENT_LINUX_LOG_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/client/linux/log"
)
install ( FILES
	${BREAKPAD_CLIENT_LINUX_DUMPWRITER_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/client/linux/microdump_writer"
)

install(TARGETS
    breakpad_client
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

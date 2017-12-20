cmake_minimum_required(VERSION 3.6)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(breakpad_client)

# Common to all client projects
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

# Common to all client projects
set(BREAKPAD_CLIENT_COMMON_HEADERS
    minidump_file_writer-inl.h
    minidump_file_writer.h
)
source_group( "Header Files\\Breakpad_Client_Common" FILES ${BREAKPAD_CLIENT_COMMON_HEADERS} )

set(BREAKPAD_CLIENT_COMMON_SOURCES
    minidump_file_writer.cc
)
source_group( "Source Files\\Breakpad_Client_Common" FILES ${BREAKPAD_CLIENT_COMMON_SOURCES} )

if(LINUX)

# Common to all linux projects
set(BREAKPAD_COMMON_LINUX_HEADERS
    ../common/linux/elfutils.h
    ../common/linux/file_id.h
	../common/linux/guid_creator.h
    ../common/linux/http_upload.h
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
    ../common/linux/http_upload.cc
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
elseif(WIN32)

add_library(breakpad_client
    ${BREAKPAD_COMMON_HEADERS}
    ${BREAKPAD_COMMON_SOURCES}
    ${BREAKPAD_CLIENT_COMMON_HEADERS}
    ${BREAKPAD_CLIENT_COMMON_SOURCES}
)
elseif(APPLE)
set(BREAKPAD_CLIENT_MACOS_HANDLER_HEADERS
    mac/handler/exception_handler.h
    mac/handler/minidump_generator.h
    mac/handler/dynamic_images.h
)
set(BREAKPAD_CLIENT_MACOS_CRASHGEN_HEADERS
    mac/crash_generation/crash_generation_client.h
)
set(BREAKPAD_CLIENT_MACOS_COMMON_HEADERS
    ../common/mac/http_upload.h
    ../common/mac/string_utilities.h
    ../common/mac/MachIPC.h
    ../common/mac/macho_id.h
    ../common/mac/macho_utilities.h
    ../common/mac/macho_walker.h
    ../common/mac/file_id.h
    ../common/mac/bootstrap_compat.h
)
set(BREAKPAD_CLIENT_MACOS_HEADERS
    ${BREAKPAD_CLIENT_MACOS_COMMON_HEADERS}
    ${BREAKPAD_CLIENT_MACOS_CRASHGEN_HEADERS}
    ${BREAKPAD_CLIENT_MACOS_HANDLER_HEADERS}
)
source_group( "Header Files\\Breakpad_Client_Apple" FILES ${BREAKPAD_CLIENT_MACOS_HEADERS} )
set(BREAKPAD_CLIENT_MACOS_SOURCES
    mac/handler/exception_handler.cc
    mac/handler/minidump_generator.cc
    mac/handler/dynamic_images.cc
    mac/crash_generation/crash_generation_client.cc
    ../common/mac/http_upload.cc
    ../common/mac/string_utilities.cc
    ../common/mac/MachIPC.mm
    ../common/mac/macho_id.cc
    ../common/mac/macho_utilities.cc
    ../common/mac/macho_walker.cc
    ../common/mac/file_id.cc
    ../common/mac/bootstrap_compat.cc
)
source_group( "Source Files\\Breakpad_Client_Apple" FILES ${BREAKPAD_CLIENT_LINUX_SOURCES} )

add_library(breakpad_client
    ${BREAKPAD_COMMON_HEADERS}
    ${BREAKPAD_COMMON_SOURCES}
    ${BREAKPAD_CLIENT_COMMON_HEADERS}
    ${BREAKPAD_CLIENT_COMMON_SOURCES}
    ${BREAKPAD_CLIENT_MACOS_SOURCES}
)
set_target_properties(breakpad_client PROPERTIES DEBUG_POSTFIX "d")

install ( FILES
	${BREAKPAD_CLIENT_MACOS_COMMON_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/common/mac"
)
install ( FILES
	${BREAKPAD_CLIENT_MACOS_HANDLER_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/client/mac/handler"
)
install ( FILES
	${BREAKPAD_CLIENT_MACOS_CRASHGEN_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/client/mac/crash_generation"
)

endif()


install ( FILES
	${BREAKPAD_COMMON_HEADERS}
	DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/breakpad/common"
)
install ( FILES
	${BREAKPAD_CLIENT_COMMON_HEADERS}
	DESTINATION
    "${CMAKE_INSTALL_PREFIX}/include/breakpad/client"
)

install(TARGETS
    breakpad_client
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

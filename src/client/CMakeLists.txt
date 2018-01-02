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
source_group( "Header Files\\Breakpad_Common" FILES ${BREAKPAD_CLIENT_COMMON_HEADERS} )

set(BREAKPAD_CLIENT_COMMON_SOURCES
    minidump_file_writer.cc
)
source_group( "Source Files\\Breakpad_Common" FILES ${BREAKPAD_CLIENT_COMMON_SOURCES} )

if(LINUX)
# Linux OS rules

# Common to all linux projects
set(BREAKPAD_COMMON_LINUX_HEADERS
    ../common/linux/elfutils.h
    ../common/linux/file_id.h
	../common/linux/guid_creator.h
    ../common/linux/http_upload.h
)
source_group( "Header Files\\Breakpad_Common" FILES ${BREAKPAD_COMMON_LINUX_HEADERS} )
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
source_group( "Source Files\\Breakpad_Common" FILES ${BREAKPAD_COMMON_LINUX_SOURCES} )

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
source_group( "Header Files\\Breakpad_Client" FILES ${BREAKPAD_CLIENT_LINUX_HEADERS} )
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
source_group( "Source Files\\Breakpad_Client" FILES ${BREAKPAD_CLIENT_LINUX_SOURCES} )

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
# Windows OS rules
set(BREAKPAD_CLIENT_WINDOWS_COMMON_HEADERS
    ../common/windows/guid_string.h
    ../common/windows/http_upload.h
)
set(BREAKPAD_CLIENT_WINDOWS_HANDLER_HEADERS
    windows/handler/exception_handler.h
)
set(BREAKPAD_CLIENT_WINDOWS_CRASHGEN_HEADERS
    windows/crash_generation/crash_generation_client.h
    windows/crash_generation/crash_generation_server.h
)
set(BREAKPAD_CLIENT_WINDOWS_SENDER_HEADERS
    windows/sender/crash_report_sender.h
)
set(BREAKPAD_CLIENT_WINDOWS_SOURCES
    ../common/windows/string_utils.cc
    ../common/windows/http_upload.cc
    ../common/windows/guid_string.cc
    windows/handler/exception_handler.cc
    windows/crash_generation/crash_generation_client.cc
    windows/sender/crash_report_sender.cc
)
source_group( "Source Files\\Breakpad_Client" FILES ${BREAKPAD_CLIENT_WINDOWS_SOURCES} )

set(BREAKPAD_CLIENT_WINDOWS_HEADERS
    ${BREAKPAD_CLIENT_WINDOWS_COMMON_HEADERS}
    ${BREAKPAD_CLIENT_WINDOWS_HANDLER_HEADERS}
    ${BREAKPAD_CLIENT_WINDOWS_CRASHGEN_HEADERS}
    ${BREAKPAD_CLIENT_WINDOWS_SENDER_HEADERS}
)
source_group( "Header Files\\Breakpad_Client" FILES ${BREAKPAD_CLIENT_WINDOWS_HEADERS} )

add_library(breakpad_client
    ${BREAKPAD_COMMON_HEADERS}
    ${BREAKPAD_COMMON_SOURCES}
    ${BREAKPAD_CLIENT_COMMON_HEADERS}
    ${BREAKPAD_CLIENT_WINDOWS_HEADERS}
    ${BREAKPAD_CLIENT_WINDOWS_SOURCES}
)
set_target_properties(breakpad_client PROPERTIES DEBUG_POSTFIX "d")
target_compile_definitions(breakpad_client
    PRIVATE UNICODE
    PRIVATE _WIN32_WINNT=0x0600
    PRIVATE WINVER=0x0600
    PRIVATE WIN32
    PRIVATE _WINDOWS
    PRIVATE _HAS_EXCEPTIONS=0
    PRIVATE NOMINMAX
    PRIVATE _CRT_RAND_S
    PRIVATE CERT_CHAIN_PARA_HAS_EXTRA_FIELDS
    PRIVATE WIN32_LEAN_AND_MEAN
    PRIVATE _SECURE_ATL
    PRIVATE _HAS_TR1=0
)
target_include_directories(breakpad_client
    PRIVATE ../third_party/dia_sdk/include
    PRIVATE ../third_party/atl_sdk/include
)
if(CMAKE_SIZEOF_VOID_P MATCHES "8")
target_link_libraries(breakpad_client
    PRIVATE ../third_party/dia_sdk/lib/amd64
)
else()
target_link_libraries(breakpad_client
    PRIVATE ../third_party/dia_sdk/lib
)
endif()

install ( FILES
	${BREAKPAD_CLIENT_WINDOWS_COMMON_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/common/windows"
)
install ( FILES
	${BREAKPAD_CLIENT_WINDOWS_HANDLER_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/client/windows/handler"
)
install ( FILES
	${BREAKPAD_CLIENT_WINDOWS_CRASHGEN_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/client/windows/crash_generation"
)
install ( FILES
	${BREAKPAD_CLIENT_WINDOWS_SENDER_HEADERS}
	DESTINATION
	"${CMAKE_INSTALL_PREFIX}/include/breakpad/client/windows/sender"
)


elseif(APPLE)
# Apple Mac OS rules
set(BREAKPAD_CLIENT_MACOS_HANDLER_HEADERS
    mac/handler/exception_handler.h
    mac/handler/minidump_generator.h
    mac/handler/dynamic_images.h
)
set(BREAKPAD_CLIENT_MACOS_CRASHGEN_HEADERS
    mac/crash_generation/crash_generation_client.h
)
set(BREAKPAD_CLIENT_MACOS_COMMON_HEADERS
    ../common/mac/HTTPMultipartUpload.h
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
source_group( "Header Files\\Breakpad_Client" FILES ${BREAKPAD_CLIENT_MACOS_HEADERS} )

set(BREAKPAD_CLIENT_MACOS_SOURCES
    mac/handler/exception_handler.cc
    mac/handler/minidump_generator.cc
    mac/handler/dynamic_images.cc
    mac/crash_generation/crash_generation_client.cc
    ../common/mac/HTTPMultipartUpload.m
    ../common/mac/string_utilities.cc
    ../common/mac/MachIPC.mm
    ../common/mac/macho_id.cc
    ../common/mac/macho_utilities.cc
    ../common/mac/macho_walker.cc
    ../common/mac/file_id.cc
    ../common/mac/bootstrap_compat.cc
)
source_group( "Source Files\\Breakpad_Client" FILES ${BREAKPAD_CLIENT_MACOS_SOURCES} )

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

# Common install rules

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

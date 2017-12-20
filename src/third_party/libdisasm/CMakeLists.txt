cmake_minimum_required(VERSION 3.6)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(disasm)

set(DISASM_SOURCES
    ia32_implicit.c
	ia32_implicit.h
	ia32_insn.c
	ia32_insn.h
	ia32_invariant.c
	ia32_invariant.h
	ia32_modrm.c
	ia32_modrm.h
	ia32_opcode_tables.c
	ia32_opcode_tables.h
	ia32_operand.c
	ia32_operand.h
	ia32_reg.c
	ia32_reg.h
	ia32_settings.c
	ia32_settings.h
	libdis.h
	qword.h
	x86_disasm.c
	x86_format.c
	x86_imm.c
	x86_imm.h
	x86_insn.c
	x86_misc.c
	x86_operand_list.c
    x86_operand_list.h
)

add_library(disasm
    ${DISASM_SOURCES}
)
set_target_properties(disasm PROPERTIES DEBUG_POSTFIX "d")
target_compile_definitions(disasm
    PRIVATE HAVE_CONFIG_H
)
#include <stdio.h>

#include "symbol.h"
#include "target.h"
#include "machine.h"

struct symbol *size_t_ctype = &uint_ctype;
struct symbol *ssize_t_ctype = &int_ctype;
struct symbol *intmax_ctype = &llong_ctype;
struct symbol *uintmax_ctype = &ullong_ctype;
struct symbol *int64_ctype = &long_ctype;
struct symbol *uint64_ctype = &ulong_ctype;
struct symbol *int32_ctype = &int_ctype;
struct symbol *uint32_ctype = &uint_ctype;
struct symbol *wchar_ctype = &int_ctype;
struct symbol *wint_ctype = &uint_ctype;

/*
 * For "__attribute__((aligned))"
 */
int max_alignment = 16;

/*
 * Integer data types
 */
int bits_in_bool = 1;
int bits_in_char = 8;
int bits_in_short = 16;
int bits_in_int = 32;
int bits_in_long = 32;
int bits_in_longlong = 64;
int bits_in_longlonglong = 128;

int max_int_alignment = 4;

/*
 * Floating point data types
 */
int bits_in_float = 32;
int bits_in_double = 64;
int bits_in_longdouble = 128;

int max_fp_alignment = 16;

/*
 * Pointer data type
 */
int bits_in_pointer = 32;
int pointer_alignment = 4;

/*
 * Enum data types
 */
int bits_in_enum = 32;
int enum_alignment = 4;


void init_target(void)
{
	switch (arch_mach) {
	case MACH_I386:
	case MACH_MIPS32:
	case MACH_PPC32:
	case MACH_RISCV32:
	case MACH_SPARC32:
	case MACH_S390:
		if (arch_m64 == ARCH_LP64)
			arch_mach++;
		break;
	case MACH_X86_64:
	case MACH_MIPS64:
	case MACH_PPC64:
	case MACH_RISCV64:
	case MACH_SPARC64:
	case MACH_S390X:
		if (arch_m64 == ARCH_LP32)
			arch_mach--;
		break;
	}

	switch (arch_mach) {
	case MACH_I386:
		wchar_ctype = &long_ctype;
		/* fall through */
	case MACH_X86_64:
		switch (arch_os) {
		case OS_CYGWIN:
			wchar_ctype = &ushort_ctype;
			break;
		case OS_DARWIN:
			int64_ctype = &llong_ctype;
			uint64_ctype = &ullong_ctype;
			wint_ctype = &int_ctype;
			break;
		case OS_FREEBSD:
			wint_ctype = &int_ctype;
			break;
		case OS_OPENBSD:
			wchar_ctype = &int_ctype;
			wint_ctype = &int_ctype;
			break;
		}
		break;
	case MACH_M68K:
	case MACH_PPC32:
		wchar_ctype = &long_ctype;
		break;
	case MACH_ARM:
	case MACH_ARM64:
		wchar_ctype = &uint_ctype;
		break;
	case MACH_SPARC32:
		if (arch_os == OS_SUNOS) {
			wint_ctype = &long_ctype;
			wchar_ctype = &long_ctype;
		}
		break;
	default:
		break;
	}
	if (fshort_wchar)
		wchar_ctype = &ushort_ctype;

	switch (arch_mach) {
	case MACH_SPARC32:
		if (arch_os == OS_SUNOS)
			break;
		/* fall through */
	case MACH_ARM:
	case MACH_MIPS64:
		// FIXME: ABI n32 & (n)64 have 128-bit ldouble
	case MACH_MIPS32:
	case MACH_S390:
	case MACH_S390X:
		bits_in_longdouble = 64;
		max_fp_alignment = 8;
		break;
	case MACH_X86_64:
		if (arch_m64 == ARCH_LP64 || arch_m64 == ARCH_X32)
			break;
		/* fall through */
	case MACH_I386:
	case MACH_M68K:
		bits_in_longdouble = 96;
		max_fp_alignment = 4;
		break;
	default:
		break;
	}

	switch (arch_mach) {
	case MACH_ARM:
	case MACH_ARM64:
	case MACH_PPC32:
	case MACH_PPC64:
	case MACH_RISCV32:
	case MACH_RISCV64:
	case MACH_S390:
	case MACH_S390X:
		if (funsigned_char == -1)
			funsigned_char = 1;
		break;
	default:
		if (funsigned_char == -1)
			funsigned_char = 0;
		break;
	}

	switch (arch_m64) {
	case ARCH_X32:
		max_int_alignment = 8;
		int64_ctype = &llong_ctype;
		uint64_ctype = &ullong_ctype;
		break;
	case ARCH_LP32:
		/* default values */
		int64_ctype = &llong_ctype;
		uint64_ctype = &ullong_ctype;
		intmax_ctype = &llong_ctype;
		uintmax_ctype = &ullong_ctype;
		break;
	case ARCH_LP64:
		bits_in_long = 64;
		max_int_alignment = 8;
		size_t_ctype = &ulong_ctype;
		ssize_t_ctype = &long_ctype;
		intmax_ctype = &long_ctype;
		uintmax_ctype = &ulong_ctype;
		goto case_64bit_common;
	case ARCH_LLP64:
		bits_in_long = 32;
		max_int_alignment = 8;
		size_t_ctype = &ullong_ctype;
		ssize_t_ctype = &llong_ctype;
		int64_ctype = &llong_ctype;
		uint64_ctype = &ullong_ctype;
		goto case_64bit_common;
	case_64bit_common:
		bits_in_pointer = 64;
		pointer_alignment = 8;
		break;
	}
}

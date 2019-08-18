/* ELF file layout */
/* Object file bitness (64-bit if not specified) */
#if !defined(_OBJ_BIT)
#define _OBJ_BIT 0x02
#endif
/* Object File Endianness (Little if not specified) */
#if !defined(_OBJ_ENDI)
#define _OBJ_ENDI 0x01
#endif
/* Object file version (Current if not specified) */
#if !defined(_OBJ_VER)
#define _OBJ_VER 0x01
#endif
/* Object file OS ABI (Linux if not specified) */
#if !defined(_OBJ_ABI)
#define _OBJ_ABI 0x03
#endif
/* Object file ABI version */
#if !defined(_OBJ_ABI_VER)
#define _OBJ_ABI_VER 0x00
#endif
/* Object file type (none if not specified) */
#if !defined(_OBJ_TYPE)
#define _OBJ_TYPE 0x02
#endif
/* Object file ISA (None if not specified) */
#if !defined(_OBJ_ISA)
#define _OBJ_ISA 0x3E
#endif

// #define phdr(_ph_type, _flags, _offset, _vaddr, _paddr, _fsz, _msz, )

.section .text
.global _start
_start:
/* ==== ELF File starts here ==== */
	/* Magic */
	.byte 0x7F, 'E', 'L', 'F'
	/* CLASS */
	.byte _OBJ_BIT
	/* Endianness */
	.byte _OBJ_ENDI
	/* Version */
	.byte _OBJ_VER
	/* OS ABI */
	.byte _OBJ_ABI
	/* ABI VER */
	.byte _OBJ_ABI_VER
	/* Padding */
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	/* Type */
	.short _OBJ_TYPE
	/* ISA */
	.short _OBJ_ISA
	/* ELF Version */
	.short 0x01, 0x00
	/* Entry addr */
	#if _OBJ_BIT == 0x02
		/* 64-bit address */
		.quad 0x0000000000000000
	#else
		.word 0x00000000
	#endif
	/* phoff addr */
	#if _OBJ_BIT == 0x02
		/* 64-bit address */
		.quad 0x0000000000000000
	#else
		.word 0x00000000
	#endif
	/* shoff addr */
	#if _OBJ_BIT == 0x02
		/* 64-bit address */
		.quad 0x0000000000000000
	#else
		.word 0x00000000
	#endif
	/* Flags */
	.short 0x00, 0x00
	/* Header size */
	.short __END_OF_HEADER - _start
	/* phentsize */
	.short 0x00
	/* phnum */
	.short 0x00
	/* shentsize */
	.short 0x00
	/* shnum */
	.short 0x00
	/* shstrndx */
	.short 0x00
	__END_OF_HEADER:

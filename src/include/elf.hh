/* elf.hh - ELF structures and utilities */
#pragma once
#if !defined(__SNS_ELF_HH__)
#define __SNS_ELF_HH__

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <iostream>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

#include <utility.hh>
#include <mmap_t.hh>
#include <fd_t.hh>
#include <zlib.hh>

/* Basic types used to composite the larger types */

/* 32-Bit ELF types */
typedef /* ALIGN(0x04) */ uint32_t elf32_addr_t;
typedef /* ALIGN(0x04) */ uint32_t elf32_off_t;
typedef /* ALIGN(0x02) */ uint16_t elf32_half_t;
typedef /* ALIGN(0x04) */ uint32_t elf32_word_t;
typedef /* ALIGN(0x08) */ uint64_t elf32_xword_t;
typedef /* ALIGN(0x04) */ int32_t  elf32_sword_t;
typedef /* ALIGN(0x08) */ int64_t  elf32_sxword_t;
typedef /* ALIGN(0x01) */ uint8_t  elf32_uchar_t;

/* 64-Bit ELF Types */
typedef /* ALIGN(0x08) */ uint64_t elf64_addr_t;
typedef /* ALIGN(0x08) */ uint64_t elf64_off_t;
typedef /* ALIGN(0x02) */ uint16_t elf64_half_t;
typedef /* ALIGN(0x04) */ uint32_t elf64_word_t;
typedef /* ALIGN(0x08) */ uint64_t elf64_xword_t;
typedef /* ALIGN(0x04) */ int32_t  elf64_sword_t;
typedef /* ALIGN(0x08) */ int64_t  elf64_sxword_t;
typedef /* ALIGN(0x01) */ uint8_t  elf64_uchar_t;

/* Alternative type declarations */
struct elf_types_32_t;
struct elf_types_64_t;

/* ELF Class */
enum class elf_class_t : uint8_t {
	None,
	ELF32,
	ELF64,
};
extern const std::array<const enum_pair_t<elf_class_t>, 3> elf_class_s;
extern std::ostream& operator<<(std::ostream& out, const elf_class_t& eclass);

/* ELF Data encoding */
enum class elf_data_t : uint8_t {
	None,
	LSB,
	MSB,
};
extern const std::array<const enum_pair_t<elf_data_t>, 3> elf_data_s;
extern std::ostream& operator<<(std::ostream& out, const elf_data_t& data);


/* ELF Ident Version */
enum class elf_ident_version_t : uint8_t {
	None,
	Current,
};
extern const std::array<const enum_pair_t<elf_ident_version_t>, 2> elf_ident_version_s;
extern std::ostream& operator<<(std::ostream& out, const elf_ident_version_t& ident_version);


/* ELF Operating System ABI */
enum class elf_osabi_t : uint8_t {
	None,
	HPUX,
	NetBSD,
	Linux,
	GNUMach, /* IA64 Supplemented */
	IA32,    /* IA64 Supplemented */
	Solaris,
	AIX,
	IRIX,
	FreeBSD,
	TRU64,
	Modesto,
	OpenBSD,
	OpenVMS,
	NonStopKernel,
	ArmEABI = 64,
	ARM = 97,
	Standalone = 255,
};
extern const std::array<const enum_pair_t<elf_osabi_t>, 18> elf_osabi_s;
extern std::ostream& operator<<(std::ostream& out, const elf_osabi_t& abi);


/* ELF Version */
enum class elf_version_t : uint32_t {
	None,
	Current,
};
extern const std::array<const enum_pair_t<elf_version_t>, 2> elf_version_s;
extern std::ostream& operator<<(std::ostream& out, const elf_version_t& version);


/* ELF Object type */
enum class ALIGN(0x02) elf_type_t : uint16_t {
	None = 0x0000,
	Relocatable,
	Executable,
	SharedObject,
	CoreFile,
	LOW_OS = 0xFE00,
	/* Operating System Specific */
	HIGH_OS = 0xFEFF,
	LOW_PROC = 0xFF00,
	/* Processor Specific */
	HIGH_PROC = 0xFFFF
};
extern const std::array<const enum_pair_t<elf_type_t>, 9> elf_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_type_t& type);


/* ELF Machine Types */
enum class ALIGN(0x02) elf_machine_t : uint16_t {
	None = 0,  /* No machine */
	M32 = 1,   /* AT&T WE 32100 */
	SPARC = 2, /* SUN SPARC */
	I386 = 3,  /* Intel 80386 */
	M68K = 4,  /* Motorola m68k family */
	M88K = 5,  /* Motorola m88k family */
	IAMCU = 6, /* Intel MCU */
	I860 = 7,  /* Intel 80860 */
	MIPS = 8,  /* MIPS R3000 big-endian */
	S370 = 9,  /* IBM System/370 */
	MIPS_RS3_LE = 10, /* MIPS R3000 little-endian */
	/* reserved 11-14 */
	PARISC = 15, /* HPPA */
	/* reserved 16 */
	VPP500 = 17, /* Fujitsu VPP500 */
	SPARC32PLUS = 18, /* Sun's "v8plus" */
	I960 = 19,  /* Intel 80960 */
	PPC = 20,   /* PowerPC */
	PPC64 = 21, /* PowerPC 64-bit */
	S390 = 22,  /* IBM S390 */
	SPU = 23,   /* IBM SPU/SPC */
	/* reserved 24-35 */
	V800 = 36, /* NEC V800 series */
	FR20 = 37, /* Fujitsu FR20 */
	RH32 = 38, /* TRW RH-32 */
	RCE = 39,  /* Motorola RCE */
	ARM = 40,  /* ARM */
	FAKE_ALPHA = 41, /* Digital Alpha */
	SH = 42, /* Hitachi SH */
	SPARCV9 = 43, /* SPARC v9 64-bit */
	TRICORE = 44, /* Siemens Tricore */
	ARC = 45, /* Argonaut RISC Core */
	H8_300 = 46, /* Hitachi H8/300 */
	H8_300H = 47, /* Hitachi H8/300H */
	H8S = 48, /* Hitachi H8S */
	H8_500 = 49, /* Hitachi H8/500 */
	IA_64 = 50, /* Intel Merced */
	MIPS_X = 51, /* Stanford MIPS-X */
	COLDFIRE = 52, /* Motorola Coldfire */
	M68HC12 = 53, /* Motorola M68HC12 */
	MMA = 54, /* Fujitsu MMA Multimedia Accelerator */
	PCP = 55, /* Siemens PCP */
	NCPU = 56, /* Sony nCPU embeeded RISC */
	NDR1 = 57, /* Denso NDR1 microprocessor */
	STARCORE = 58, /* Motorola Start*Core processor */
	ME16 = 59, /* Toyota ME16 processor */
	ST100 = 60, /* STMicroelectronic ST100 processor */
	TINYJ = 61, /* Advanced Logic Corp. Tinyj emb.fam */
	X86_64 = 62, /* AMD x86-64 architecture */
	PDSP = 63, /* Sony DSP Processor */
	PDP10 = 64, /* Digital PDP-10 */
	PDP11 = 65, /* Digital PDP-11 */
	FX66 = 66, /* Siemens FX66 microcontroller */
	ST9PLUS = 67, /* STMicroelectronics ST9+ 8/16 mc */
	ST7 = 68, /* STmicroelectronics ST7 8 bit mc */
	M68HC16 = 69, /* Motorola MC68HC16 microcontroller */
	M68HC11 = 70, /* Motorola MC68HC11 microcontroller */
	M68HC08 = 71, /* Motorola MC68HC08 microcontroller */
	M68HC05 = 72, /* Motorola MC68HC05 microcontroller */
	SVX = 73, /* Silicon Graphics SVx */
	ST19 = 74, /* STMicroelectronics ST19 8 bit mc */
	VAX = 75, /* Digital VAX */
	CRIS = 76, /* Axis Communications 32-bit emb.proc */
	JAVELIN = 77, /* Infineon Technologies 32-bit emb.proc */
	FIREPATH = 78, /* Element 14 64-bit DSP Processor */
	ZSP = 79, /* LSI Logic 16-bit DSP Processor */
	MMIX = 80, /* Donald Knuth's educational 64-bit proc */
	HUANY = 81, /* Harvard University machine-independent object files */
	PRISM = 82, /* SiTera Prism */
	AVR = 83, /* Atmel AVR 8-bit microcontroller */
	FR30 = 84, /* Fujitsu FR30 */
	D10V = 85, /* Mitsubishi D10V */
	D30V = 86, /* Mitsubishi D30V */
	V850 = 87, /* NEC v850 */
	M32R = 88, /* Mitsubishi M32R */
	MN10300 = 89, /* Matsushita MN10300 */
	MN10200 = 90, /* Matsushita MN10200 */
	PJ = 91, /* picoJava */
	OPENRISC = 92, /* OpenRISC 32-bit embedded processor */
	ARC_COMPACT = 93, /* ARC International ARCompact */
	XTENSA = 94, /* Tensilica Xtensa Architecture */
	VIDEOCORE = 95, /* Alphamosaic VideoCore */
	TMM_GPP = 96, /* Thompson Multimedia General Purpose Proc */
	NS32K = 97, /* National Semi. 32000 */
	TPC = 98, /* Tenor Network TPC */
	SNP1K = 99, /* Trebia SNP 1000 */
	ST200 = 100, /* STMicroelectronics ST200 */
	IP2K = 101, /* Ubicom IP2xxx */
	MAX = 102, /* MAX processor */
	CR = 103, /* National Semi. CompactRISC */
	F2MC16 = 104, /* Fujitsu F2MC16 */
	MSP430 = 105, /* Texas Instruments msp430 */
	BLACKFIN = 106, /* Analog Devices Blackfin DSP */
	SE_C33 = 107, /* Seiko Epson S1C33 family */
	SEP = 108, /* Sharp embedded microprocessor */
	ARCA = 109, /* Arca RISC */
	UNICORE = 110, /* PKU-Unity & MPRC Peking Uni. mc series */
	EXCESS = 111, /* eXcess configurable cpu */
	DXP = 112, /* Icera Semi. Deep Execution Processor */
	ALTERA_NIOS2 = 113, /* Altera Nios II */
	CRX = 114, /* National Semi. CompactRISC CRX */
	XGATE = 115, /* Motorola XGATE */
	C166 = 116, /* Infineon C16x/XC16x */
	M16C = 117, /* Renesas M16C */
	DSPIC30F = 118, /* Microchip Technology dsPIC30F */
	CE = 119, /* Freescale Communication Engine RISC */
	M32C = 120, /* Renesas M32C */
	/* reserved 121-130 */
	TSK3000 = 131, /* Altium TSK3000 */
	RS08 = 132, /* Freescale RS08 */
	SHARC = 133, /* Analog Devices SHARC family */
	ECOG2 = 134, /* Cyan Technology eCOG2 */
	SCORE7 = 135, /* Sunplus S+core7 RISC */
	DSP24 = 136, /* New Japan Radio (NJR) 24-bit DSP */
	VIDEOCORE3 = 137, /* Broadcom VideoCore III */
	LATTICEMICO32 = 138, /* RISC for Lattice FPGA */
	SE_C17 = 139, /* Seiko Epson C17 */
	TI_C6000 = 140, /* Texas Instruments TMS320C6000 DSP */
	TI_C2000 = 141, /* Texas Instruments TMS320C2000 DSP */
	TI_C5500 = 142, /* Texas Instruments TMS320C55x DSP */
	TI_ARP32 = 143, /* Texas Instruments App. Specific RISC */
	TI_PRU = 144, /* Texas Instruments Prog. Realtime Unit */
	/* reserved 145-159 */
	MMDSP_PLUS = 160, /* STMicroelectronics 64bit VLIW DSP */
	CYPRESS_M8C = 161, /* Cypress M8C */
	R32C = 162, /* Renesas R32C */
	TRIMEDIA = 163, /* NXP Semi. TriMedia */
	QDSP6 = 164, /* QUALCOMM DSP6 */
	I8051 = 165, /* Intel 8051 and variants */
	STXP7X = 166, /* STMicroelectronics STxP7x */
	NDS32 = 167, /* Andes Tech. compact code emb. RISC */
	ECOG1X = 168, /* Cyan Technology eCOG1X */
	MAXQ30 = 169, /* Dallas Semi. MAXQ30 mc */
	XIMO16 = 170, /* New Japan Radio (NJR) 16-bit DSP */
	MANIK = 171, /* M2000 Reconfigurable RISC */
	CRAYNV2 = 172, /* Cray NV2 vector architecture */
	RX = 173, /* Renesas RX */
	METAG = 174, /* Imagination Tech. META */
	MCST_ELBRUS = 175, /* MCST Elbrus */
	ECOG16 = 176, /* Cyan Technology eCOG16 */
	CR16 = 177, /* National Semi. CompactRISC CR16 */
	ETPU = 178, /* Freescale Extended Time Processing Unit */
	SLE9X = 179, /* Infineon Tech. SLE9X */
	L10M = 180, /* Intel L10M */
	K10M = 181, /* Intel K10M */
	/* reserved 182 */
	AARCH64 = 183, /* ARM AARCH64 */
	/* reserved 184 */
	AVR32 = 185, /* Amtel 32-bit microprocessor */
	STM8 = 186, /* STMicroelectronics STM8 */
	TILE64 = 187, /* Tileta TILE64 */
	TILEPRO = 188, /* Tilera TILEPro */
	MICROBLAZE = 189, /* Xilinx MicroBlaze */
	CUDA = 190, /* NVIDIA CUDA */
	TILEGX = 191, /* Tilera TILE-Gx */
	CLOUDSHIELD = 192, /* CloudShield */
	COREA_1ST = 193, /* KIPO-KAIST Core-A 1st gen. */
	COREA_2ND = 194, /* KIPO-KAIST Core-A 2nd gen. */
	ARC_COMPACT2 = 195, /* Synopsys ARCompact V2 */
	OPEN8 = 196, /* Open8 RISC */
	RL78 = 197, /* Renesas RL78 */
	VIDEOCORE5 = 198, /* Broadcom VideoCore V */
	R78KOR = 199, /* Renesas 78KOR */
	F56800EX = 200, /* Freescale 56800EX DSC */
	BA1 = 201, /* Beyond BA1 */
	BA2 = 202, /* Beyond BA2 */
	XCORE = 203, /* XMOS xCORE */
	MCHP_PIC = 204, /* Microchip 8-bit PIC(r) */
	/* reserved 205-209 */
	KM32 = 210, /* KM211 KM32 */
	KMX32 = 211, /* KM211 KMX32 */
	EMX16 = 212, /* KM211 KMX16 */
	EMX8 = 213, /* KM211 KMX8 */
	KVARC = 214, /* KM211 KVARC */
	CDP = 215, /* Paneve CDP */
	COGE = 216, /* Cognitive Smart Memory Processor */
	COOL = 217, /* Bluechip CoolEngine */
	NORC = 218, /* Nanoradio Optimized RISC */
	CSR_KALIMBA = 219, /* CSR Kalimba */
	Z80 = 220, /* Zilog Z80 */
	VISIUM = 221, /* Controls and Data Services VISIUMcore */
	FT32 = 222, /* FTDI Chip FT32 */
	MOXIE = 223, /* Moxie processor */
	AMDGPU = 224, /* AMD GPU */
	/* reserved 225-242 */
	RISCV = 243, /* RISC-V */
	BPF = 247, /* Linux BPF -- in-kernel virtual machine */
	/* Unofficial */
	BONELESS = 0x666, /* whitequark's Boneless-III CPU https://github.com/whitequark/Boneless-CPU */
	ALPHA = 0x9026, /* Digital Alpha */
};
extern const std::array<const enum_pair_t<elf_machine_t>, 180> elf_machine_s;
extern std::ostream& operator<<(std::ostream& out, const elf_machine_t& machine);


/* ELF Header Flags */
enum class elf_flag_t : uint32_t {
	None,
	/* ARM Processor Supplement */
	ARM_HasEntry      = 0x00000002,
	ARM_SymsAreSrtd   = 0x00000004,
	ARM_DynSymsSegIdx = 0x00000008,
	ARM_MapSymsFirst  = 0x00000010,
	ARM_EABIMask      = 0xFF000000,

	/* IA64 Processor Supplement */
	IA64_ReduceDFP    = 0x00000020,
	IA64_ConsGP       = 0x00000040,
	IA64_Nofuncdesc   = 0x00000080,
	IA64_Absolute     = 0x00000100,
	IA64_MaskOS       = 0x00FF000F,
	IA64_Arch         = 0xFF000000,
};
template<>
struct EnableBitmask<elf_flag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_flag_t>, 12> elf_flag_s;
extern std::ostream& operator<<(std::ostream& out, const elf_flag_t& flag);


/* ELF Special Section Indexes */
enum class elf_shns_t : uint16_t {
	Undefined,
	LowReserve = 0xFF00,
	LowProc = 0xFF00,
	Before = 0xFF00,
	After = 0xFF01,
	HighProc = 0xFF1F,
	LowOS = 0xFF20,
	HighOS = 0xFF3F,
	ABS = 0xFFF1,
	Common = 0xFFF2,
	XIndex = 0xFFFF,
	HighReserve = 0xFFFF,
};
extern const std::array<const enum_pair_t<elf_shns_t>, 12> elf_shns_s;
extern std::ostream& operator<<(std::ostream& out, const elf_shns_t& shns);


/* ELF section header types */
enum class elf_shtype_t : uint32_t {
	Null,
	ProgBits,
	SymbolTable,
	StringTable,
	RelA,
	HashTable,
	Dynamic,
	Note,
	NoBits,
	Rel,
	SHLib,
	DynamicSymbols,
	InitArray = 14,
	FiniArray,
	PreinitArray,
	Group,
	SymbolTableIndex,
	LowOS = 0x60000000,
	GNUAttributes = 0x6ffffff5,
	GNUHash,
	GNULibList,
	Checksum,
	SUNMove = 0x6ffffffa,
	SUNComDat,
	SUNSymInfo,
	GNUVerDef,
	GNUVerNeed,
	GNUVerSym,
	HighOS = 0x6fffffff,
	LowProc = 0x70000000,
	HighProc = 0x7fffffff,
	LowUser = 0x80000000,
	HighUser = 0xffffffff,
};
extern const std::array<const enum_pair_t<elf_shtype_t>, 33> elf_shtype_s;
extern std::ostream& operator<<(std::ostream& out, const elf_shtype_t& type);


/* 32-Bit Section flags */
enum class elf32_shflags_t : uint32_t {
	None,
	Write = 0x00000001,
	Alloc = 0x00000002,
	ExecInstr = 0x00000004,
	Merge = 0x00000010,
	Strings = 0x00000020,
	InfoLink = 0x00000040,
	LinkOrder = 0x00000080,
	NonconformingOS = 0x00000100,
	Group = 0x00000200,
	TLS = 0x00000400,
	Compressed = 0x00000800,
	Ordered = 0x40000000,
	Exclude = 0x80000000,
};
template<>
struct EnableBitmask<elf32_shflags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf32_shflags_t>, 14> elf32_shflags_s;
extern std::ostream& operator<<(std::ostream& out, const elf32_shflags_t& shflag);


/* 64-Bit Section flags */
enum class elf64_shflags_t : uint64_t {
	None,
	Write = 0x00000001,
	Alloc = 0x00000002,
	ExecInstr = 0x00000004,
	Merge = 0x00000010,
	Strings = 0x00000020,
	InfoLink = 0x00000040,
	LinkOrder = 0x00000080,
	NonconformingOS = 0x00000100,
	Group = 0x00000200,
	TLS = 0x00000400,
	Compressed = 0x00000800,
	Ordered = 0x40000000,
	Exclude = 0x80000000,
};
template<>
struct EnableBitmask<elf64_shflags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf64_shflags_t>, 14> elf64_shflags_s;
extern std::ostream& operator<<(std::ostream& out, const elf64_shflags_t& shflag);


/* Section group flags */
enum class elf_shgroup_t : uint32_t {
	None,
	Comdat = 0x00000001,
	MaskOS = 0x0FF00000,
	MaskProc = 0xF0000000,
};
template<>
struct EnableBitmask<elf_shgroup_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_shgroup_t>, 4> elf_shgroup_s;
extern std::ostream& operator<<(std::ostream& out, const elf_shgroup_t& shgroup);


/* Symbol Binding */
enum class elf_symbol_binding_t : uint8_t {
	Local,
	Global,
	Weak,
	LowOS = 10,
	HighOS = 12,
	LowProc = 13,
	HighProc = 15,
};
extern const std::array<const enum_pair_t<elf_symbol_binding_t>, 7> elf_symbol_binding_s;
extern std::ostream& operator<<(std::ostream& out, const elf_symbol_binding_t& symbind);


/* Symbol Type */
enum class elf_symbol_type_t : uint8_t {
	NoType,
	Object,
	Function,
	Section,
	File,
	Common,
	ThreadLocalStorage,
	LowOS = 10,
	HighOS = 12,
	LowProc = 13,
	HighProc = 15,
};
extern const std::array<const enum_pair_t<elf_symbol_type_t>, 11> elf_symbol_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_symbol_type_t& symtype);


/* Symbol Visibility */
enum class elf_symbol_visibility_t : uint8_t {
	Default,
	Internal,
	Hidden,
	Protected,
};
extern const std::array<const enum_pair_t<elf_symbol_visibility_t>, 4> elf_symbol_visibility_s;
extern std::ostream& operator<<(std::ostream& out, const elf_symbol_visibility_t& symvis);


/* Symbol binding */
enum  class elf_syminfo_bound_t : uint16_t {
	None,
	LowReserve = 0xFF00,
	Parent = 0xFFFE,
	Self = 0xFFFF,
};
extern const std::array<const enum_pair_t<elf_syminfo_bound_t>, 4> elf_syminfo_bound_s;
extern std::ostream& operator<<(std::ostream& out, const elf_syminfo_bound_t& symibind);


/* Symbol info flags */
enum class elf_syminfo_flag_t : uint16_t {
	None,
	Direct = 0x0001,
	Passthru = 0x0002,
	Copy = 0x0004,
	LazyLoad = 0x0008,
};
template<>
struct EnableBitmask<elf_syminfo_flag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_syminfo_flag_t>, 5> elf_syminfo_flag_s;
extern std::ostream& operator<<(std::ostream& out, const elf_syminfo_flag_t& symiflag);


/* Program header type */
enum class elf_phdr_type_t : uint32_t {
	None,
	Load,
	Dynamic,
	Interpreter,
	Note,
	SHLib,
	ProgramHeader,
	ThreadLocalStorage,
	LowOS = 0x60000000,
	GNUEHFrame = 0x6474e550,
	GNUStack,
	GNURelRO,
	SUNBSS = 0x6ffffffa,
	SUNStack = 0x6ffffffb,
	HighOS = 0x6fffffff,
	LowProc = 0x70000000,
	HighProc = 0x7fffffff,
};
extern const std::array<const enum_pair_t<elf_phdr_type_t>, 17> elf_phdr_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_phdr_type_t& phdrtype);


/* Program header flags */
enum class elf_phdr_flags_t : uint32_t {
	None,
	Execute = 0x01,
	Write = 0x02,
	Read = 0x04,
	MaskOS = 0x0ff00000,
	MaskProc = 0xf0000000,
};
template<>
struct EnableBitmask<elf_phdr_flags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_phdr_flags_t>, 6> elf_phdr_flags_s;
extern std::ostream& operator<<(std::ostream& out, const elf_phdr_flags_t& phdrflag);


/* Note Segment Descriptor Types */
enum class elf_note_desc_t : uint32_t {
	None,
	PRSTATUS,
	FPREGSET,
	PRPSINFO,
	PRXREG,
	TASKSTRUCT,
	PLATFORM,
	AUXV,
	GWINDOWS,
	ASRS,
	PSTATUS = 10,
	PSINFO = 13,
	PRCRED,
	UTSNAME,
	LWPSTATUS,
	LWPSINFO,
	PRFPXREG = 20,
	/* Power PC Specific */
	PPC_VMV = 0x100,
	PPC_SPE,
	PPC_VSX,
	PPC_TAR,
	PPC_PPR,
	PPC_DSCR,
	PPC_EBB,
	PPC_PMU,
	PPC_TM_CGPR,
	PPC_TM_CFPR,
	PPC_TM_CVMX,
	PPC_TM_CVSX,
	PPC_TM_SPR,
	PPC_TM_CTAR,
	PPC_TM_CPPR,
	PPC_TM_CDSCR,
	PPC_PKEY,
	/* x86 PC Specific */
	I386_TLS = 0x200,
	I386_IOPerm,
	X86_XState,
	/* System/390 Specific */
	S390_HighGprs = 0x300,
	S390_Timer,
	S390_TODCmp,
	S390_TODPreg,
	S390_CTRS,
	S390_Prefix,
	S390_LastBreak,
	S390_SystemCall,
	S390_TBD,
	S390_VXRS_Low,
	S390_VXRS_High,
	S390_GS_CB,
	S390_GS_BC,
	S390_RI_CB,
	/* ARM Specific */
	ARM_VFP = 0x400,
	ARM_TLS,
	ARM_HW_BREAK,
	ARM_HW_WATCH,
	ARM_SYSTEM_CALL,
	ARM_SVE,
	/* Extended */
	File = 0x46494c45,
	PRXFPReg = 0x46e62b7f,
	SigInfo = 0x53494749,
};
extern const std::array<const enum_pair_t<elf_note_desc_t>, 60> elf_note_desc_s;
extern std::ostream& operator<<(std::ostream& out, const elf_note_desc_t& notedesc);


/* Dynamic section tags */
enum class elf32_dyn_tag_t : elf32_sword_t {
	None,
	Needed,
	PLTRelSize,
	PLTGOT,
	Hash,
	StrTab,
	SymTab,
	RelA,
	RelASize,
	RelAEnt,
	StrTabSize,
	SymTabEnt,
	Init,
	Fini,
	SOName,
	RPath,
	Symbolic,
	Rel,
	RelSize,
	RelEnt,
	PLTRel,
	Debug,
	TextRel,
	JmpRel,
	BindNow,
	InitArray,
	FiniArray,
	InitArraySize,
	FiniArraySize,
	RunPath,
	Flags,
	Encoding,
	PreInitArray,
	PreInitArraySize,
	SymtabSHNDX,
	LowOS = 0x6000000D,
	HighOS = 0x6ffff000,
	GNUPrelinked = 0x6ffffdf5,
	GNUConflictSize,
	GNULibListSize,
	Checksum,
	PLTPadSize,
	MoveEnt,
	MoveSize,
	Feature,
	PosFlag,
	SyminfoSize,
	SyminfoEnt,
	AddrRNGLow,
	GNUHash = 0x6ffffef5,
	TLSDescPLT,
	TLSDescGOT,
	GNUConflict,
	GNULibList,
	Config,
	DEPAudit,
	Audit,
	PLTPad,
	MoveTAB,
	Syminfo,
	VerSym,
	RelACount = 0x6ffffff9,
	RelCount,
	Flags_1,
	VerDef,
	VerDefNum,
	VerNeed,
	VerNeedNum,
	LowProc = 0x70000000,
	Auxiliary = 0x7ffffffd,
	HighProc = 0x7fffffff,
};
extern const std::array<const enum_pair_t<elf32_dyn_tag_t>, 71> elf32_dyn_tag_s;
extern std::ostream& operator<<(std::ostream& out, const elf32_dyn_tag_t& dyntag);


enum class elf64_dyn_tag_t : elf64_sxword_t {
	None,
	Needed,
	PLTRelSize,
	PLTGOT,
	Hash,
	StrTab,
	SymTab,
	RelA,
	RelASize,
	RelAEnt,
	StrTabSize,
	SymTabEnt,
	Init,
	Fini,
	SOName,
	RPath,
	Symbolic,
	Rel,
	RelSize,
	RelEnt,
	PLTRel,
	Debug,
	TextRel,
	JmpRel,
	BindNow,
	InitArray,
	FiniArray,
	InitArraySize,
	FiniArraySize,
	RunPath,
	Flags,
	Encoding,
	PreInitArray,
	PreInitArraySize,
	SymtabSHNDX,
	LowOS = 0x6000000D,
	HighOS = 0x6ffff000,
	GNUPrelinked = 0x6ffffdf5,
	GNUConflictSize,
	GNULibListSize,
	Checksum,
	PLTPadSize,
	MoveEnt,
	MoveSize,
	Feature,
	PosFlag,
	SyminfoSize,
	SyminfoEnt,
	AddrRNGLow,
	GNUHash = 0x6ffffef5,
	TLSDescPLT,
	TLSDescGOT,
	GNUConflict,
	GNULibList,
	Config,
	DEPAudit,
	Audit,
	PLTPad,
	MoveTAB,
	Syminfo,
	VerSym,
	RelACount = 0x6ffffff9,
	RelCount,
	Flags_1,
	VerDef,
	VerDefNum,
	VerNeed,
	VerNeedNum,
	LowProc = 0x70000000,
	Auxiliary = 0x7ffffffd,
	HighProc = 0x7fffffff,
};
extern const std::array<const enum_pair_t<elf64_dyn_tag_t>, 71> elf64_dyn_tag_s;
extern std::ostream& operator<<(std::ostream& out, const elf64_dyn_tag_t& dyntag);


/* Dynamic Entry Flags */
enum class elf_dyn_flags_t : uint8_t {
	None,
	Origin = 0x01,
	Symbolic = 0x02,
	TextRel = 0x04,
	BindNow = 0x08,
	StaticTLS = 0x10,
};
template<>
struct EnableBitmask<elf_dyn_flags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_dyn_flags_t>, 6> elf_dyn_flags_s;
extern std::ostream& operator<<(std::ostream& out, const elf_dyn_flags_t& dynflag);


/* Extended Dynamic Entry Flags */
enum class elf_dyn_eflags_t : uint32_t {
	None,
	Now = 0x00000001,
	Global = 0x00000002,
	Group = 0x00000004,
	NoDelete = 0x00000008,
	LoadFLTR = 0x00000010,
	InitFirst = 0x00000020,
	NoOpen = 0x00000040,
	Origin = 0x00000080,
	Direct = 0x00000100,
	Trans = 0x00000200,
	Interpose = 0x00000400,
	NoDefaultLib = 0x00000800,
	NoDump = 0x00001000,
	ConFALT = 0x00002000,
	EndFileTEE = 0x00004000,
	DispRelDNE = 0x00008000,
	DispRelPND = 0x00010000,
	NoDirect = 0x00020000,
	IgnoreMultiDef = 0x00040000,
	NoKSyms = 0x00080000,
	NoHDR = 0x00100000,
	Edited = 0x00200000,
	NoReloc = 0x00400000,
	SymInterpose = 0x00800000,
	GlobalAudit = 0x01000000,
	Singleton = 0x02000000,
	Stub = 0x04000000,
	PIE = 0x08000000,
};
template<>
struct EnableBitmask<elf_dyn_eflags_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_dyn_eflags_t>, 29> elf_dyn_eflags_s;
extern std::ostream& operator<<(std::ostream& out, const elf_dyn_eflags_t& dyneflag);


/* Dynamic Table Features */
enum class elf_dyn_feature_t : uint32_t {
	None,
	ParInit = 0x01,
	ConfEXP = 0x02,
};
template<>
struct EnableBitmask<elf_dyn_feature_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_dyn_feature_t>, 3> elf_dyn_feature_s;
extern std::ostream& operator<<(std::ostream& out, const elf_dyn_feature_t& dynfeat);


/* Dynamic entry posflags */
enum class elf_dyn_posflag_t : uint32_t {
	None,
	LazyLoad = 0x01,
	GroupPerm = 0x02,
};
template<>
struct EnableBitmask<elf_dyn_posflag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_dyn_posflag_t>, 3> elf_dyn_posflag_s;
extern std::ostream& operator<<(std::ostream& out, const elf_dyn_posflag_t& dynposf);


/* Compressed Header Type */
enum class elf_chdr_type_t : uint32_t {
	None,
	Zlib,
	LowOS = 0x60000000,
	HighOS = 0x6fffffff,
	LowProc = 0x70000000,
	HighProc = 0x7fffffff,
};
extern const std::array<const enum_pair_t<elf_chdr_type_t>, 6> elf_chdr_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_chdr_type_t& chdrtype);

/* Version definition revisions */
enum class elf_verdef_revision_t : uint16_t {
	None,
	Current,
};
extern const std::array<const enum_pair_t<elf_verdef_revision_t>, 2> elf_verdef_revision_s;
extern std::ostream& operator<<(std::ostream& out, const elf_verdef_revision_t& verdefr);


/* Version definitions flags */
enum class elf_verdef_flag_t : uint16_t {
	None,
	Base = 0x01,
	Weak = 0x02,
};
template<>
struct EnableBitmask<elf_verdef_flag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_verdef_flag_t>, 3> elf_verdef_flag_s;
extern std::ostream& operator<<(std::ostream& out, const elf_verdef_flag_t& verdeff);


/* Versym index values */
enum class elf_verdef_index_t : uint16_t {
	Local,
	Global,
	LowReserve = 0xFF00,
	Eliminate = 0xFF01,
};
extern const std::array<const enum_pair_t<elf_verdef_index_t>, 4> elf_verdef_index_s;
extern std::ostream& operator<<(std::ostream& out, const elf_verdef_index_t& verdefi);


/* Vernaux flags */
enum class elf_vernaux_flag_t : uint16_t {
	None,
	Weak = 0x02,
};
template<>
struct EnableBitmask<elf_vernaux_flag_t>{
		static constexpr bool enabled = true;
};
extern const std::array<const enum_pair_t<elf_vernaux_flag_t>, 2> elf32_vernaux_s;
extern std::ostream& operator<<(std::ostream& out, const elf_vernaux_flag_t& verauxf);


/* Auxiliary Vector Types */
enum elf_auxv_type_t : uint32_t {
	Null,
	Ignore,
	ExecFD,
	PHDR,
	PHEnt,
	PHNum,
	PageSize,
	Base,
	Entry,
	NotElf,
	UID,
	EUID,
	GID,
	EGID,
	Platform,
	HWCap,
	ClkTck,
	FPUControlWord,
	DataCacheBSize,
	InstructionCacheBSize,
	UnifiedCacheBSize,
	IgnorePPC, /* poor PPC */
	Secure,
	BasePlatform,
	Random,
	HWCap2, /* The electric boogaloo */
	ExecFN,
	Sysinfo,
	SysinfoEHDR,
	L1InstCacheShape,
	L1DataCacheShape,
	L2CacheShape,
	L3CacheShape,
	L1InstCacheSize,
	L1InstCacheGeometry,
	L1DataCacheSize,
	L1DataCacheGeometry,
	L2CacheSize,
	L2CacheGeometry,
	L3CacheSize,
	L3CacheGeometry,
};
extern const std::array<const enum_pair_t<elf_auxv_type_t>, 41> elf_auxv_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_auxv_type_t& auxvtype);


/* Note known systems */
enum class elf_note_os_t : uint32_t {
	Linux,
	GNU,
	Solaris2,
	FreeBSD,
};
extern const std::array<const enum_pair_t<elf_note_os_t>, 4> elf_note_os_s;
extern std::ostream& operator<<(std::ostream& out, const elf_note_os_t& noteos);


/* Note Type */
enum class elf_note_type_t : uint32_t {
	None,
	GNUABI,
	GNUHWCap,
	GNUBuildID,
	GNUGoldVersion,
	GNUPropertyType,
};
extern const std::array<const enum_pair_t<elf_note_type_t>, 6> elf_note_type_s;
extern std::ostream& operator<<(std::ostream& out, const elf_note_type_t& notetype);



/* ELF Structure definitions */
/* ELF Magic */
struct elf_magic_t final {
private:
	uint8_t _magic1; /* Should be 0x7F*/
	uint8_t _magic2; /* Should be 'E' */
	uint8_t _magic3; /* Should be 'L' */
	uint8_t _magic4; /* Should be 'F' */
public:
	constexpr elf_magic_t() noexcept :
		_magic1{}, _magic2{}, _magic3{}, _magic4{}
		{ /* NOP */ }

	bool is_valid() const noexcept {
		return (_magic1 == 0x7F && _magic2 == 0x45
			 && _magic3 == 0x4C && _magic4 == 0x46);
	}

	void set() noexcept {
		_magic1 = 0x7F;
		_magic2 = 0x45;
		_magic3 = 0x4C;
		_magic4 = 0x46;
	}

	std::array<uint8_t, 4> get() const noexcept {
		return { _magic1, _magic2, _magic3, _magic4 };
	}

};


/* ELF Identity */
struct elf_ident_t final {
private:
	elf_magic_t _magic;              /* The ELF magic number (0x7F 'E' 'L' 'F') */
	elf_class_t _eclass;             /* Class of the object file, 32 vs 64 bit */
	elf_data_t _data;                /* The endianness of the object file */
	elf_ident_version_t _version;    /* File version (Should be Current) */
	elf_osabi_t _abi;                /* Platform ABI */
	uint8_t _abi_version;            /* Version of the osabi_t ABI */
	std::array<uint8_t, 7> _padding; /* unused padding bytes */
public:
	constexpr elf_ident_t() noexcept :
		_magic{}, _eclass{}, _data{}, _version{},
		_abi{}, _abi_version{}, _padding{} { /* NOP */ }

	elf_ident_t(elf_magic_t magic, elf_class_t eclass,
			elf_data_t data, elf_ident_version_t version, elf_osabi_t abi,
			uint8_t abi_version
		) noexcept :
			_magic{magic}, _eclass{eclass}, _data{data}, _version{version},
			_abi{abi}, _abi_version{abi_version}, _padding{} { /* NOP */ }


	elf_magic_t magic() const noexcept { return _magic; }

	void eclass(const elf_class_t eclass) noexcept { _eclass = eclass; }
	elf_class_t eclass() const noexcept { return _eclass; }

	void data(const elf_data_t data) noexcept { _data = data; }
	elf_data_t data() const noexcept { return _data; }

	void version(const elf_ident_version_t version) noexcept { _version = version; }
	elf_ident_version_t version() const noexcept { return _version; }

	void abi(const elf_osabi_t abi) noexcept { _abi = abi; }
	elf_osabi_t abi() const noexcept { return _abi; }

	void abi_version(const uint8_t abi_version) noexcept { _abi_version = abi_version; }
	uint8_t abi_version() const noexcept { return _abi_version; }

	void padding(const std::array<uint8_t, 7> padding) noexcept { _padding = padding; }
	std::array<uint8_t, 7> padding() const noexcept { return _padding; }
};


/* ELF Header */
template<typename T>
struct elf_ehdr_t final {
	using addr_t   = typename T::addr_t;
	using offset_t = typename T::offset_t;
	using word_t   = typename T::word_t;
	using half_t   = typename T::half_t;
private:
	elf_ident_t _ident;     /* ELF file identity */
	elf_type_t _type;       /* Type of object file */
	elf_machine_t _machine; /* Object architecture */
	elf_version_t _version; /* Object file version */
	addr_t _entry;          /* Virtual Address of entry point */
	offset_t _phoff;        /* Program header table offset */
	offset_t _shoff;        /* Section header table offset */
	word_t _flags;          /* Processor dependent flags */
	half_t _ehsize;         /* Size of elf header */
	half_t _phentsize;      /* Size of program header table entry */
	half_t _phnum;          /* Number of program header table entries */
	half_t _shentsize;      /* Size of section header table entry */
	half_t _shnum;          /* Number of section header table entries */
	half_t _shstrndx;       /* Index of section header string table */
public:
	constexpr elf_ehdr_t() noexcept :
		_ident{}, _type{}, _machine{}, _version{}, _entry{},
		_phoff{}, _shoff{}, _flags{}, _ehsize{}, _phentsize{},
		_phnum{}, _shentsize{}, _shnum{}, _shstrndx{} { /* NOP */ }

	elf_ehdr_t(elf_ident_t ident, elf_type_t type,
			elf_machine_t machine, elf_version_t version,
			addr_t entry, offset_t phoff,
			offset_t shoff, word_t flags,
			half_t ehsize, half_t phentsize,
			half_t phnum, half_t shentsize,
			half_t shnum, half_t shstrndx) noexcept :
		_ident{ident}, _type{type}, _machine{machine}, _version{version},
		_entry{entry}, _phoff{phoff}, _shoff{shoff}, _flags{flags},
		_ehsize{ehsize}, _phentsize{phentsize}, _phnum{phnum},
		_shentsize{shentsize}, _shnum{shnum}, _shstrndx{shstrndx} { /* NOP */ }

	void ident(const elf_ident_t ident) noexcept { _ident = ident; }
	elf_ident_t ident() const noexcept { return _ident; }

	void type(const elf_type_t type) noexcept { _type = type; }
	elf_type_t type() const noexcept { return _type; }

	void machine(const elf_machine_t machine) noexcept { _machine = machine; }
	elf_machine_t machine() const noexcept { return _machine; }

	void version(const elf_version_t version) noexcept { _version = version; }
	elf_version_t version() const noexcept { return _version; }

	void entry(const addr_t entry) noexcept { _entry = entry; }
	addr_t entry() const noexcept { return _entry; }

	void phoff(const offset_t phoff) noexcept { _phoff = phoff; }
	offset_t phoff() const noexcept { return _phoff; }

	void shoff(const offset_t shoff) noexcept { _shoff = shoff; }
	offset_t shoff() const noexcept { return _shoff; }

	void flags(const word_t flags) noexcept { _flags = flags; }
	word_t flags() const noexcept { return _flags; }

	void ehsize(const half_t ehsize) noexcept { _ehsize = ehsize; }
	half_t ehsize() const noexcept { return _ehsize; }

	void phentsize(const half_t phentsize) noexcept { _phentsize = phentsize; }
	half_t phentsize() const noexcept { return _phentsize; }

	void phnum(const half_t phnum) noexcept { _phnum = phnum; }
	half_t phnum() const noexcept { return _phnum; }

	void shentsize(const half_t shentsize) noexcept { _shentsize = shentsize; }
	half_t shentsize() const noexcept { return _shentsize; }

	void shnum(const half_t shnum) noexcept { _shnum = shnum; }
	half_t shnum() const noexcept { return _shnum; }

	void shstrndx(const half_t shstrndx) noexcept { _shstrndx = shstrndx; }
	half_t shstrndx() const noexcept { return _shstrndx; }
};
using elf32_ehdr_t = elf_ehdr_t<elf_types_32_t>;
using elf64_ehdr_t = elf_ehdr_t<elf_types_64_t>;



/* 32-Bit Section header */
struct elf32_shdr_t final {
private:
	elf32_word_t _name;
	elf_shtype_t _type;
	elf32_shflags_t _flags;
	elf32_addr_t _addr;
	elf32_off_t _offset;
	elf32_word_t _size;
	elf32_word_t _link;
	elf32_word_t _info;
	elf32_word_t _addraline;
	elf32_word_t _entsize;
public:
	constexpr elf32_shdr_t() noexcept :
		_name{}, _type{}, _flags{}, _addr{}, _offset{}, _size{},
		_link{}, _info{}, _addraline{}, _entsize{} { /* NOP */ }

	elf32_shdr_t(elf32_word_t name, elf32_word_t type, elf32_word_t flags,
			elf32_addr_t addr, elf32_off_t offset, elf32_word_t size,
			elf32_word_t link, elf32_word_t info, elf32_word_t addraline,
			elf32_word_t entsize) noexcept :
		_name{name}, _type{type}, _flags{flags}, _addr{addr}, _offset{offset},
		_size{size}, _link{link}, _info{info}, _addraline{addraline},
		_entsize{entsize} { /* NOP */}

	void name(const elf32_word_t name) noexcept { _name = name; }
	elf32_word_t name() const noexcept { return _name; }

	void type(const elf_shtype_t type) noexcept { _type = type; }
	elf_shtype_t type() const noexcept { return _type; }

	void flags(const elf32_shflags_t flags) noexcept { _flags = flags; }
	elf32_shflags_t flags() const noexcept { return _flags; }

	void addr(const elf32_addr_t addr) noexcept { _addr = addr; }
	elf32_addr_t addr() const noexcept { return _addr; }

	void offset(const elf32_off_t offset) noexcept { _offset = offset; }
	elf32_off_t offset() const noexcept { return _offset; }

	void size(const elf32_word_t size) noexcept { _size = size; }
	elf32_word_t size() const noexcept { return _size; }

	void link(const elf32_word_t link) noexcept { _link = link; }
	elf32_word_t link() const noexcept { return _link; }

	void info(const elf32_word_t info) noexcept { _info = info; }
	elf32_word_t info() const noexcept { return _info; }

	void addraline(const elf32_word_t addraline) noexcept { _addraline = addraline; }
	elf32_word_t addraline() const noexcept { return _addraline; }

	void entsize(const elf32_word_t entsize) noexcept { _entsize = entsize; }
	elf32_word_t entsize() const noexcept { return _entsize; }
};

/* 64-Bit Section header */
struct elf64_shdr_t final {
private:
	elf64_word_t _name;
	elf_shtype_t _type;
	elf64_shflags_t _flags;
	elf64_addr_t _addr;
	elf64_off_t  _offset;
	elf64_xword_t _size;
	elf64_word_t _link;
	elf64_word_t _info;
	elf64_xword_t _addraline;
	elf64_xword_t _entsize;
public:
	constexpr elf64_shdr_t() noexcept :
		_name{}, _type{}, _flags{}, _addr{}, _offset{}, _size{},
		_link{}, _info{}, _addraline{}, _entsize{} { /* NOP */ }

	elf64_shdr_t(elf64_word_t name, elf64_word_t type, elf64_word_t flags,
			elf64_addr_t addr, elf64_off_t offset, elf64_word_t size,
			elf64_word_t link, elf64_word_t info, elf64_word_t addraline,
			elf64_word_t entsize) noexcept :
		_name{name}, _type{type}, _flags{flags}, _addr{addr}, _offset{offset},
		_size{size}, _link{link}, _info{info}, _addraline{addraline},
		_entsize{entsize} { /* NOP */ }

	void name(const elf64_word_t name) noexcept { _name = name; }
	elf64_word_t name() const noexcept { return _name; }

	void type(const elf_shtype_t type) noexcept { _type = type; }
	elf_shtype_t type() const noexcept { return _type; }

	void flags(const elf64_shflags_t flags) noexcept { _flags = flags; }
	elf64_shflags_t flags() const noexcept { return _flags; }

	void addr(const elf64_addr_t addr) noexcept { _addr = addr; }
	elf64_addr_t addr() const noexcept { return _addr; }

	void offset(const elf64_off_t offset) noexcept { _offset = offset; }
	elf64_off_t offset() const noexcept { return _offset; }

	void size(const elf64_xword_t size) noexcept { _size = size; }
	elf64_xword_t size() const noexcept { return _size; }

	void link(const elf64_word_t link) noexcept { _link = link; }
	elf64_word_t link() const noexcept { return _link; }

	void info(const elf64_word_t info) noexcept { _info = info; }
	elf64_word_t info() const noexcept { return _info; }

	void addraline(const elf64_xword_t addraline) noexcept { _addraline = addraline; }
	elf64_xword_t addraline() const noexcept { return _addraline; }

	void entsize(const elf64_xword_t entsize) noexcept { _entsize = entsize; }
	elf64_xword_t entsize() const noexcept { return _entsize; }
};

/* 32-Bit Symbol Table Entry */
struct elf32_symbol_t final {
private:
	elf32_word_t _name;
	elf32_addr_t _value;
	elf32_word_t _size;
	elf32_uchar_t _info;
	elf32_uchar_t _other;
	elf32_half_t _shndx;
public:
	constexpr elf32_symbol_t() noexcept :
		_name{}, _value{}, _size{}, _info{}, _other{}, _shndx{}
	{ /* NOP */ }

	elf32_symbol_t(elf32_word_t name, elf32_addr_t value,
			elf32_word_t size, elf32_uchar_t info, elf32_uchar_t other,
			elf32_half_t shndx) noexcept :
		_name{name}, _value{value}, _size{size}, _info{info}, _other{other},
		_shndx{shndx} { /* NOP */ }

	void name(elf32_word_t name) noexcept { _name = name; }
	elf32_word_t name() const noexcept { return _name; }

	void value(const elf32_addr_t value) noexcept { _value = value; }
	elf32_addr_t value() const noexcept { return _value; }

	void size(const elf32_word_t size) noexcept { _size = size; }
	elf32_word_t size() const noexcept { return _size; }

	void info(const elf32_uchar_t info) noexcept { _info = info; }
	elf32_uchar_t info() const noexcept { return _info; }

	void other(const elf32_uchar_t other) noexcept { _other = other; }
	elf32_uchar_t other() const noexcept { return _other; }

	void shndx(const elf32_half_t shndx) noexcept { _shndx = shndx; }
	elf32_half_t shndx() const noexcept { return _shndx; }
};

#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))
#define ELF32_ST_VISIBILITY(o) ((o)&0x3)

/* 64-Bit Symbol Table Entry */
struct elf64_symbol_t final {
private:
	elf64_word_t _name;
	elf64_uchar_t _info;
	elf64_uchar_t _other;
	elf64_half_t _shndx;
	elf64_addr_t _value;
	elf64_xword_t _size;
public:
	constexpr elf64_symbol_t() noexcept :
		_name{}, _info{}, _other{}, _shndx{}, _value{}, _size{} { /* NOP */ }

	elf64_symbol_t(elf64_word_t name, elf64_uchar_t info,
			elf64_uchar_t other, elf64_half_t shndx, elf64_addr_t value,
			elf64_xword_t size) noexcept :
		_name{name}, _info{info}, _other{other}, _shndx{shndx}, _value{value},
		_size{size} { /* NOP */ }

	void name(const elf64_word_t name) noexcept { _name = name; }
	elf64_word_t name() const noexcept { return _name; }

	void info(const elf64_uchar_t info) noexcept { _info = info; }
	elf64_uchar_t info() const noexcept { return _info; }

	void other(const elf64_uchar_t other) noexcept { _other = other; }
	elf64_uchar_t other() const noexcept { return _other; }

	void shndx(const elf64_half_t shndx) noexcept { _shndx = shndx; }
	elf64_half_t shndx() const noexcept { return _shndx; }

	void value(const elf64_addr_t value) noexcept { _value = value; }
	elf64_addr_t value() const noexcept { return _value; }

	void size(const elf64_xword_t size) noexcept { _size = size; }
	elf64_xword_t size() const noexcept { return _size; }
};

#define ELF64_ST_BIND(i)   ((i)>>4)
#define ELF64_ST_TYPE(i)   ((i)&0xf)
#define ELF64_ST_INFO(b,t) (((b)<<4)+((t)&0xf))
#define ELF64_ST_VISIBILITY(o) ((o)&0x3)

/* 32/64-Bit syminfo section */
struct elf_syminfo_t final {
private:
	elf_syminfo_bound_t _boundto;
	elf_syminfo_flag_t _flags;
public:
	constexpr elf_syminfo_t() noexcept :
		_boundto{}, _flags{} { /* NOP */ }

	elf_syminfo_t(elf_syminfo_bound_t boundto, elf_syminfo_flag_t flags) noexcept :
		_boundto{boundto}, _flags{flags} { /* NOP */ }

	void boundto(elf_syminfo_bound_t boundto) noexcept { _boundto = boundto; }
	elf_syminfo_bound_t boundto() const noexcept { return _boundto; }

	void flags(elf_syminfo_flag_t flags) noexcept { _flags = flags; }
	elf_syminfo_flag_t flags() const noexcept { return _flags; }
};

/* 32-Bit Relocation Entries */
struct elf32_rel_t final {
private:
	elf32_addr_t _offset;
	elf32_word_t _info;
public:
	constexpr elf32_rel_t() noexcept :
		_offset{}, _info{} { /* NOP */ }

	elf32_rel_t(elf32_addr_t offset, elf32_word_t info) noexcept :
		_offset{offset}, _info{info} { /* NOP */ }

	void offset(const elf32_addr_t offset) noexcept { _offset = offset; }
	elf32_addr_t offset() const noexcept { return _offset; }

	void info(const elf32_word_t info) noexcept { _info = info; }
	elf32_word_t info() const noexcept { return _info; }
};

struct elf32_rela_t final {
private:
	elf32_addr_t _offset;
	elf32_word_t _info;
	elf32_sword_t _addend;
public:
	constexpr elf32_rela_t() noexcept :
		_offset{}, _info{}, _addend{} { /* NOP */ }

	elf32_rela_t(elf32_addr_t offset, elf32_word_t info,
			elf32_sword_t addend) noexcept :
		_offset{offset}, _info{info}, _addend{addend} { /* NOP */ }

	void offset(const elf32_addr_t offset) noexcept { _offset = offset; }
	elf32_addr_t offset() const noexcept { return _offset; }

	void info(const elf32_word_t info) noexcept { _info = info; }
	elf32_word_t info() const noexcept { return _info; }

	void addend(const elf32_sword_t addend) noexcept { _addend = addend; }
	elf32_sword_t addend() const noexcept { return _addend; }
};

#define ELF32_R_SYM(i)	((i)>>8)
#define ELF32_R_TYPE(i)   ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

/* 64-Bit Relocation Entries */
struct elf64_rel_t final {
private:
	elf64_addr_t _offset;
	elf64_xword_t _info;
public:
	constexpr elf64_rel_t() noexcept :
		_offset{}, _info{} { /* NOP */ }

	elf64_rel_t(elf64_addr_t offset, elf64_xword_t info) noexcept :
		_offset{offset}, _info{info} { /* NOP */ }

	void offset(const elf64_addr_t offset) noexcept { _offset = offset; }
	elf64_addr_t offset() const noexcept { return _offset; }

	void info(const elf64_xword_t info) noexcept { _info = info; }
	elf64_xword_t info() const noexcept { return _info; }
};

struct elf64_rela_t final {
private:
	elf64_addr_t _offset;
	elf64_xword_t _info;
	elf64_sword_t _addend;
public:
	constexpr elf64_rela_t() noexcept :
		_offset{}, _info{}, _addend{} { /* NOP */ }

	elf64_rela_t(elf64_addr_t offset, elf64_xword_t info,
			elf64_sword_t addend) noexcept :
		_offset{offset}, _info{info}, _addend{addend} { /* NOP */ }

	void offset(const elf64_addr_t offset) noexcept { _offset = offset; }
	elf64_addr_t offset() const noexcept { return _offset; }

	void info(const elf64_xword_t info) noexcept { _info = info; }
	elf64_xword_t info() const noexcept { return _info; }

	void addend(const elf64_sword_t addend) noexcept { _addend = addend; }
	elf64_sword_t addend() const noexcept { return _addend; }
};

#define ELF64_R_SYM(i)    ((i)>>32)
#define ELF64_R_TYPE(i)   ((i)&0xffffffffL)
#define ELF64_R_INFO(s,t) (((s)<<32)+((t)&0xffffffffL))

/* 32-Bit Program header */
struct elf32_phdr_t final {
private:
	elf_phdr_type_t _type;
	elf32_off_t _offset;
	elf32_addr_t _vaddr;
	elf32_addr_t _paddr;
	elf32_word_t _filesz;
	elf32_word_t _memsize;
	elf_phdr_flags_t _flags;
	elf32_word_t _align;
public:
	constexpr elf32_phdr_t() noexcept :
		_type{}, _offset{}, _vaddr{}, _paddr{}, _filesz{}, _memsize{},
		_flags{}, _align{} { /* NOP */ }

	elf32_phdr_t(elf_phdr_type_t type, elf32_off_t offset, elf32_addr_t vaddr,
			elf32_addr_t paddr, elf32_word_t filesz, elf32_word_t memsize,
			elf_phdr_flags_t flags, elf32_word_t align) noexcept :
		_type{type}, _offset{offset}, _vaddr{vaddr}, _paddr{paddr}, _filesz{filesz},
		_memsize{memsize}, _flags{flags}, _align{align} { /* NOP */ }

	void type(const elf_phdr_type_t type) noexcept { _type = type; }
	elf_phdr_type_t type() const noexcept { return _type; }

	void flags(const elf_phdr_flags_t flags) noexcept { _flags = flags; }
	elf_phdr_flags_t flags() const noexcept { return _flags; }

	void offset(const elf32_off_t offset) noexcept { _offset = offset; }
	elf32_off_t offset() const noexcept { return _offset; }

	void vaddr(const elf32_addr_t vaddr) noexcept { _vaddr = vaddr; }
	elf32_addr_t vaddr() const noexcept { return _vaddr; }

	void paddr(const elf32_addr_t paddr) noexcept { _paddr = paddr; }
	elf32_addr_t paddr() const noexcept { return _paddr; }

	void filesz(const elf32_word_t filesz) noexcept { _filesz = filesz; }
	elf32_word_t filesz() const noexcept { return _filesz; }

	void memsize(const elf32_word_t memsize) noexcept { _memsize = memsize; }
	elf32_word_t memsize() const noexcept { return _memsize; }

	void align(const elf32_word_t align) noexcept { _align = align; }
	elf32_word_t align() const noexcept { return _align; }
};

/* 64-Bit Program Header */
struct elf64_phdr_t final {
private:
	elf_phdr_type_t _type;
	elf_phdr_flags_t _flags;
	elf64_off_t _offset;
	elf64_addr_t _vaddr;
	elf64_addr_t _paddr;
	elf64_xword_t _filesz;
	elf64_xword_t _memsize;
	elf64_xword_t _align;
public:
	constexpr elf64_phdr_t() noexcept :
		_type{}, _flags{}, _offset{}, _vaddr{}, _paddr{}, _filesz{}, _memsize{},
		_align{} { /* NOP */ }

	elf64_phdr_t(elf_phdr_type_t type, elf_phdr_flags_t flags, elf64_off_t offset,
			elf64_addr_t vaddr, elf64_addr_t paddr, elf64_xword_t filesz,
			elf64_xword_t memsize, elf64_xword_t align) noexcept :
		_type{type}, _flags{flags}, _offset{offset}, _vaddr{vaddr}, _paddr{paddr},
		_filesz{filesz}, _memsize{memsize}, _align{align} { /* NOP */ }


	void type(const elf_phdr_type_t type) noexcept { _type = type; }
	elf_phdr_type_t type() const noexcept { return _type; }

	void flags(const elf_phdr_flags_t flags) noexcept { _flags = flags; }
	elf_phdr_flags_t flags() const noexcept { return _flags; }

	void offset(const elf64_off_t offset) noexcept { _offset = offset; }
	elf64_off_t offset() const noexcept { return _offset; }

	void vaddr(const elf64_addr_t vaddr) noexcept { _vaddr = vaddr; }
	elf64_addr_t vaddr() const noexcept { return _vaddr; }

	void paddr(const elf64_addr_t paddr) noexcept { _paddr = paddr; }
	elf64_addr_t paddr() const noexcept { return _paddr; }

	void filesz(const elf64_xword_t filesz) noexcept { _filesz = filesz; }
	elf64_xword_t filesz() const noexcept { return _filesz; }

	void memsize(const elf64_xword_t memsize) noexcept { _memsize = memsize; }
	elf64_xword_t memsize() const noexcept { return _memsize; }

	void align(const elf64_xword_t align) noexcept { _align = align; }
	elf64_xword_t align() const noexcept { return _align; }
};

/* 32-Bit Dynamic Section */
struct elf32_dyn_t final {
private:
	elf32_dyn_tag_t _tag;
	union {
		elf32_word_t value;
		elf32_addr_t pointer;
	} _data;
public:
	constexpr elf32_dyn_t() noexcept :
		_tag{}, _data{} { /* NOP */ }

	elf32_dyn_t(elf32_dyn_tag_t tag, elf32_word_t value) noexcept :
		_tag{tag}, _data{value} { /* NOP */ }

	void tag(elf32_dyn_tag_t tag) noexcept { _tag = tag; }
	elf32_dyn_tag_t tag() const noexcept { return _tag; }

	void value(elf32_word_t value) noexcept { _data.value = value; }
	elf32_word_t value() const noexcept { return _data.value; }

	void pointer(elf32_addr_t pointer) noexcept { _data.pointer = pointer; }
	elf32_addr_t pointer() const noexcept { return _data.pointer; }
};

/* 64-Bit Dynamic Section */
struct elf64_dyn_t final {
private:
	elf64_dyn_tag_t _tag;
	union {
		elf64_xword_t value;
		elf64_addr_t pointer;
	} _data;
public:
	constexpr elf64_dyn_t() noexcept :
		_tag{}, _data{} { /* NOP */ }

	elf64_dyn_t(elf64_dyn_tag_t tag, elf64_xword_t value) noexcept :
		_tag{tag}, _data{value} { /* NOP */ }

	void tag(elf64_dyn_tag_t tag) noexcept { _tag = tag; }
	elf64_dyn_tag_t tag() const noexcept { return _tag; }

	void value(elf64_xword_t value) noexcept { _data.value = value; }
	elf64_xword_t value() const noexcept { return _data.value; }

	void pointer(elf64_addr_t pointer) noexcept { _data.pointer = pointer; }
	elf64_addr_t pointer() const noexcept { return _data.pointer; }
};

/* 32-Bit Compressed section header */
struct elf32_chdr_t final {
private:
	elf_chdr_type_t _type;
	elf32_word_t _size;
	elf32_word_t _addr_align;
public:
	constexpr elf32_chdr_t() noexcept :
		_type{}, _size{}, _addr_align{} { /* NOP */ }

	elf32_chdr_t(elf_chdr_type_t type, elf32_word_t size,
			elf32_word_t addr_align ) noexcept :
		_type{type}, _size{size}, _addr_align{addr_align} { /* NOP */ }

	void type(const elf_chdr_type_t type) noexcept { _type = type; }
	elf_chdr_type_t type() const noexcept { return _type; }

	void size(const elf32_word_t size) noexcept { _size = size; }
	elf32_word_t size() const noexcept { return _size; }

	void addr_align(const elf32_word_t addr_align) noexcept { _addr_align = addr_align;	}
	elf32_word_t addr_align() const noexcept { return _addr_align; }
};

/* 64-Bit Compressed section header */
struct elf64_chdr_t final {
private:
	elf_chdr_type_t _type;
	elf64_word_t _reserved;
	elf64_xword_t _size;
	elf64_xword_t _addr_align;
public:
	constexpr elf64_chdr_t() noexcept :
		_type{}, _reserved{}, _size{}, _addr_align{} { /* NOP */ }

	elf64_chdr_t(elf_chdr_type_t type, elf64_xword_t size,
			elf64_xword_t addr_align) noexcept :
		_type{type}, _reserved{}, _size{size}, _addr_align{addr_align} { /* NOP */ }

	void type(const elf_chdr_type_t type) noexcept { _type = type; }
	elf_chdr_type_t type() const noexcept { return _type; }

	void reserved(const elf64_word_t reserved) noexcept { _reserved = reserved; }
	elf64_word_t reserved() const noexcept { return _reserved; }

	void size(const elf64_xword_t size) noexcept { _size = size; }
	elf64_xword_t size() const noexcept { return _size; }

	void addr_align(const elf64_xword_t addr_align) noexcept { _addr_align = addr_align; }
	elf64_xword_t addr_align() const noexcept { return _addr_align; }
};

/* Version Definition */
template<typename T>
struct elf_verdef_t final {
	using half_t = typename T::half_t;
	using word_t = typename T::word_t;
private:
	elf_verdef_revision_t _version; /* Version revision */
	elf_verdef_flag_t _flags;       /* Version flags */
	elf_verdef_index_t _index;      /* Version index */
	half_t _count;                  /* Number of associated verdaux entries */
	word_t _hash;                   /* Version name hash */
	word_t _aux_offset;             /* Offset to the verdaux array */
	word_t _next_offset;            /* Offset to the next verdef_t */
public:
	constexpr elf_verdef_t() noexcept :
		_version{}, _flags{}, _index{}, _count{}, _hash{}, _aux_offset{},
		_next_offset{} { /* NOP */ }

	elf_verdef_t(elf_verdef_revision_t version, elf_verdef_flag_t flags,
			elf_verdef_index_t index, half_t count, word_t hash,
			word_t aux_offset, word_t next_offset) noexcept :
		_version{version}, _flags{flags}, _index{index}, _count{count},
		_hash{hash}, _aux_offset{aux_offset}, _next_offset{next_offset}
		{ /* NOP */ }

	void version(const elf_verdef_revision_t version) noexcept { _version = version; }
	elf_verdef_revision_t version() const noexcept { return _version; }

	void flags(const elf_verdef_flag_t flags) noexcept { _flags = flags; }
	elf_verdef_flag_t flags() const noexcept { return _flags; }

	void index(const elf_verdef_index_t index) noexcept { _index = index; }
	elf_verdef_index_t index() const noexcept { return _index; }

	void count(const half_t count) noexcept { _count = count; }
	half_t count() const noexcept { return _count; }

	void hash(const word_t hash) noexcept { _hash = hash; }
	word_t hash() const noexcept { return _hash; }

	void aux_offset(const word_t aux_offset) noexcept { _aux_offset = aux_offset; }
	word_t aux_offset() const noexcept { return _aux_offset; }

	void next_offset(const word_t next_offset) noexcept { _next_offset = next_offset; }
	word_t next_offset() const noexcept { return _next_offset; }
};
using elf32_verdef_t = elf_verdef_t<elf_types_32_t>;
using elf64_verdef_t = elf_verdef_t<elf_types_64_t>;

/* Auxiliary Version Information */
template<typename T>
struct elf_verdaux_t final {
	using word_t = typename T::word_t;
private:
	word_t _name; /* Version or dependency names */
	word_t _next; /* Offset to next verdaux entry */
public:
	constexpr elf_verdaux_t() noexcept :
		_name{}, _next{} { /* NOP */ }

	elf_verdaux_t(word_t name, word_t next) noexcept :
		_name{name}, _next{next} { /* NOP */ }

	void name(const word_t name) noexcept { _name = name; }
	word_t name() const noexcept { return _name; }

	void next(const word_t next) noexcept { _next = next; }
	word_t next() const noexcept { return _next; }
};
using elf32_verdaux_t = elf_verdaux_t<elf_types_32_t>;
using elf64_verdaux_t = elf_verdaux_t<elf_types_64_t>;

/* Version dependency */
template<typename T>
struct elf_verneed_t final {
	using word_t = typename T::word_t;
	using half_t = typename T::half_t;
private:
	half_t _version; /* Version */
	half_t _count;   /* Number of associated vernaux elements */
	word_t _file;    /* String table offset for filename */
	word_t _aux;     /* Offset to vernaux array */
	word_t _next;    /* Offset to next verneed_t */
public:
	constexpr elf_verneed_t() noexcept :
		_version{}, _count{}, _file{}, _aux{}, _next{} { /* NOP */ }

	elf_verneed_t(half_t version, half_t count, word_t file,
			word_t aux, word_t next) noexcept :
		_version{version}, _count{count}, _file{file}, _aux{aux}, _next{next}
		{ /* NOP */ }

	void version(const half_t version) noexcept { _version = version; }
	half_t version() const noexcept { return _version; }

	void count(const half_t count) noexcept { _count = count; }
	half_t count() const noexcept { return _count; }

	void file(const word_t file) noexcept { _file = file; }
	word_t file() const noexcept { return _file; }

	void aux(const word_t aux) noexcept { _aux = aux; }
	word_t aux() const noexcept { return _aux; }

	void next(const word_t next) noexcept { _next = next; }
	word_t next() const noexcept { return _next; }
};
using elf32_verneed_t = elf_verneed_t<elf_types_32_t>;
using elf64_verneed_t = elf_verneed_t<elf_types_64_t>;

/* Version need Auxiliary */
template<typename T>
struct elf_vernaux_t final {
	using word_t = typename T::word_t;
	using half_t = typename T::half_t;
private:
	word_t _hash;        /* Dependency name hash */
	elf_vernaux_flag_t _flags; /* Dependency flags */
	half_t _other;
	word_t _name;        /* Offset to dependency name in the string table */
	word_t _next;        /* offset to next vernaux_t */
public:
	constexpr elf_vernaux_t() noexcept :
		_hash{}, _flags{}, _other{}, _name{}, _next{} { /* NOP */ }

	elf_vernaux_t(word_t hash, elf_vernaux_flag_t flags, half_t other, word_t name,
			word_t next) noexcept :
		_hash{hash}, _flags{flags}, _other{other}, _name{name}, _next{next}
		{ /* NOP */ }

	void hash(const word_t hash) noexcept { _hash = hash; }
	word_t hash() const noexcept { return _hash; }

	void flags(const elf_vernaux_flag_t flags) noexcept { _flags = flags; }
	elf_vernaux_flag_t flags() const noexcept { return _flags; }

	void other(const half_t other) noexcept { _other = other; }
	half_t other() const noexcept { return _other; }

	void name(const word_t name) noexcept { _name = name; }
	word_t name() const noexcept { return _name; }

	void next(const word_t next) noexcept { _next = next; }
	word_t next() const noexcept { return _next; }
};
using elf32_vernaux_t = elf_vernaux_t<elf_types_32_t>;
using elf64_vernaux_t = elf_vernaux_t<elf_types_64_t>;

/* Auxiliary vector */
template<typename T>
struct elf_auxv_t final {
	using xword_t = typename T::xword_t;
	using addr_t  = typename T::addr_t;
private:
	xword_t _type;       /* Entry Type */
	union {
		xword_t value;   /* Value */
		addr_t pointer; /* Pointer */
	} _data;
public:
	constexpr elf_auxv_t() noexcept :
		_type{}, _data{} { /* NOP */ }

	elf_auxv_t(xword_t type, xword_t data) noexcept :
		_type{type}, _data{data} { /* NOP */ }

	void type(const xword_t type) noexcept { _type = type; }
	xword_t type() const noexcept { return _type; }

	void value(const xword_t value) noexcept { _data.value = value; }
	xword_t value() const noexcept { return _data.value; }

	void pointer(const addr_t pointer) noexcept { _data.pointer = pointer; }
	addr_t pointer() const noexcept { return _data.pointer; }
};
using elf32_auxv_t = elf_auxv_t<elf_types_32_t>;
using elf64_auxv_t = elf_auxv_t<elf_types_64_t>;


/* Note section header */
template<typename T>
struct elf_nhdr_t final {
	using word_t = typename T::word_t;
private:
	word_t _name_sz; /* Size of the notes name */
	word_t _desc_sz; /* Size of the notes descriptor */
	word_t _type;    /* Type of the note */
public:
	constexpr elf_nhdr_t() noexcept :
		_name_sz{}, _desc_sz{}, _type{} { /* NOP */ }

	elf_nhdr_t(word_t name_sz, word_t desc_sz,
			word_t type) noexcept :
		_name_sz{_name_sz}, _desc_sz{desc_sz}, _type{type} { /* NOP */ }

	void name_sz(const word_t name_sz) noexcept { _name_sz = name_sz; }
	word_t name_sz() const noexcept { return _name_sz; }

	void desc_sz(const word_t desc_sz) noexcept { _desc_sz = desc_sz; }
	word_t desc_sz() const noexcept { return _desc_sz; }

	void type(const word_t type) noexcept { _type = type; }
	word_t type() const noexcept { return type; }
};
using elf32_nhdr_t = elf_nhdr_t<elf_types_32_t>;
using elf64_nhdr_t = elf_nhdr_t<elf_types_64_t>;

/* 32-Bit Move Record */
struct elf32_move_t final {
private:
	elf32_xword_t _value; /* Symbol value */
	elf32_word_t _info;   /* Size and index */
	elf32_off_t _offset;  /* Symbol offset */
	elf32_half_t _repeat; /* Repeat count */
	elf32_half_t _stride; /* Stride info */
public:
	constexpr elf32_move_t() noexcept :
		_value{}, _info{}, _offset{}, _repeat{}, _stride{} { /* NOP */ }

	elf32_move_t(elf32_xword_t value, elf32_word_t info, elf32_off_t offset,
			elf32_half_t repeat, elf32_half_t stride) noexcept :
		_value{value}, _info{info}, _offset{offset}, _repeat{repeat},
		_stride{stride} { /* NOP */ }

	void value(const elf32_xword_t value) noexcept { _value = value; }
	elf32_xword_t value() const noexcept { return _value; }

	void info(const elf32_word_t info) noexcept { _info = info; }
	elf32_word_t info() const noexcept { return _info; }

	void offset(const elf32_off_t offset) noexcept { _offset = offset; }
	elf32_off_t offset() const noexcept { return _offset; }

	void repeat(const elf32_half_t repeat) noexcept { _repeat = repeat; }
	elf32_half_t repeat() const noexcept { return _repeat; }

	void stride(const elf32_half_t stride) noexcept { _stride = stride; }
	elf32_half_t stride() const noexcept { return _stride; }
};
#define ELF32_M_SYM(info)	((info) >> 8)
#define ELF32_M_SIZE(info)	((unsigned char) (info))
#define ELF32_M_INFO(sym, size)	(((sym) << 8) + (unsigned char) (size))

/* 64-Bit Move Record */
struct elf64_move_t final {
private:
	elf64_xword_t _value; /* Symbol value */
	elf64_xword_t _info;   /* Size and index */
	elf64_off_t _offset;  /* Symbol offset */
	elf64_half_t _repeat; /* Repeat count */
	elf64_half_t _stride; /* Stride info */
public:
	constexpr elf64_move_t() noexcept :
		_value{}, _info{}, _offset{}, _repeat{}, _stride{} { /* NOP */ }

	elf64_move_t(elf64_xword_t value, elf64_xword_t info, elf64_off_t offset,
			elf32_half_t repeat, elf32_half_t stride) noexcept :
		_value{value}, _info{info}, _offset{offset}, _repeat{repeat},
		_stride{stride} { /* NOP */ }

	void value(const elf64_xword_t value) noexcept { _value = value; }
	elf64_xword_t value() const noexcept { return _value; }

	void info(const elf64_xword_t info) noexcept { _info = info; }
	elf64_xword_t info() const noexcept { return _info; }

	void offset(const elf64_off_t offset) noexcept { _offset = offset; }
	elf64_off_t offset() const noexcept { return _offset; }

	void repeat(const elf64_half_t repeat) noexcept { _repeat = repeat; }
	elf64_half_t repeat() const noexcept { return _repeat; }

	void stride(const elf64_half_t stride) noexcept { _stride = stride; }
	elf64_half_t stride() const noexcept { return _stride; }
};
#define ELF64_M_SYM(info)	((info) >> 8)
#define ELF64_M_SIZE(info)	((unsigned char) (info))
#define ELF64_M_INFO(sym, size)	(((sym) << 8) + (unsigned char) (size))


struct elf_types_32_t final {
	/* Basic Types */
	using addr_t    = ALIGN(0x04) uint32_t;
	using offset_t  = ALIGN(0x04) uint32_t;
	using half_t    = ALIGN(0x02) uint16_t;
	using word_t    = ALIGN(0x04) uint32_t;
	using xword_t   = word_t;
	using sword_t   = ALIGN(0x04) int32_t;
	using xsword_t  = sword_t;
	using uchar_t   = ALIGN(0x01) uint8_t;
	/* Structure Definitions */
	using ident_t   = elf_ident_t;
	using ehdr_t    = elf32_ehdr_t;
	using shdr_t    = elf32_shdr_t;
	using symbol_t  = elf32_symbol_t;
	using syminfo_t = elf_syminfo_t;
	using rel_t     = elf32_rel_t;
	using rela_t    = elf32_rela_t;
	using phdr_t    = elf32_phdr_t;
	using dyn_t     = elf32_dyn_t;
	using chdr_t    = elf32_chdr_t;
	using verdef_t  = elf32_verdef_t;
	using verdaux_t = elf32_verdaux_t;
	using verneed_t = elf32_verneed_t;
	using vernaux_t = elf32_vernaux_t;
	using auxv_t    = elf32_auxv_t;
	using nhdr_t    = elf32_nhdr_t;
	using move_t    = elf32_move_t;
};

struct elf_types_64_t final {
	/* Basic Types */
	using addr_t   = ALIGN(0x08) uint64_t;
	using offset_t = ALIGN(0x08) uint64_t;
	using half_t   = ALIGN(0x02) uint16_t;
	using word_t   = ALIGN(0x04) uint32_t;
	using xword_t  = ALIGN(0x08) uint64_t;
	using sword_t  = ALIGN(0x04) int32_t;
	using sxword_t = ALIGN(0x08) int64_t;
	using uchar_t  = ALIGN(0x01) uint8_t;
	/* Structure Definitions */
	using ident_t   = elf_ident_t;
	using ehdr_t    = elf64_ehdr_t;
	using shdr_t    = elf64_shdr_t;
	using symbol_t  = elf64_symbol_t;
	using syminfo_t = elf_syminfo_t;
	using rel_t     = elf64_rel_t;
	using rela_t    = elf64_rela_t;
	using phdr_t    = elf64_phdr_t;
	using dyn_t     = elf64_dyn_t;
	using chdr_t    = elf64_chdr_t;
	using verdef_t  = elf64_verdef_t;
	using verdaux_t = elf64_verdaux_t;
	using verneed_t = elf64_verneed_t;
	using vernaux_t = elf64_vernaux_t;
	using auxv_t    = elf64_auxv_t;
	using nhdr_t    = elf64_nhdr_t;
	using move_t    = elf64_move_t;
};


/* Main Elf file interface */
// struct elf_t final {
// private:
// 	fs::path _file;
// 	fd_t _file_fd;
// 	mappedFile_t _file_map;
// 	elf_ident_t _ident;
// public:
// 	elf_t() noexcept :
// 		_file{}, _file_fd{}, _file_map{}, _ident{} { /* NOP */ }

// 	elf_t(fs::path file) noexcept :
// 		_file{file}, _file_fd{_file.c_str(), O_RDONLY},
// 		_file_map{_file_fd.map(PROT_READ)},
// 		_ident{_file_map.at<elf_ident_t>(0)} { /* NOP */ }

// 	bool is_valid() const noexcept { return _ident.magic().is_valid(); }

// 	void ident(const elf_ident_t ident) noexcept { _ident = ident; }
// 	elf_ident_t ident() const noexcept { return _ident; }
// };

template<typename T>
struct elf_t final {
	using ident_t   = typename T::ident_t;
	using ehdr_t    = typename T::ehdr_t;
	using shdr_t    = typename T::shdr_t;
	using symbol_t  = typename T::symbol_t;
	using syminfo_t = typename T::syminfo_t;
	using rel_t     = typename T::rel_t;
	using rela_t    = typename T::rela_t;
	using phdr_t    = typename T::phdr_t;
	using dyn_t     = typename T::dyn_t;
	using chdr_t    = typename T::chdr_t;
	using verdef_t  = typename T::verdef_t;
	using verdaux_t = typename T::verdaux_t;
	using verneed_t = typename T::verneed_t;
	using vernaux_t = typename T::vernaux_t;
	using auxv_t    = typename T::auxv_t;
	using nhdr_t    = typename T::nhdr_t;
	using move_t    = typename T::move_t;
private:
	fs::path _file;          /* The path to the object file */
	fd_t _file_fd;           /* File descriptor */
	mmap_t _file_map;        /* mmap object for object file */
	ehdr_t _header;          /* The executable header */
	set_t<phdr_t> _pheaders; /* Program Headers */
	set_t<shdr_t> _sheaders; /* Section Headers */
	char* _strtbl;           /* Section name string table */
public:
	constexpr elf_t() noexcept :
		_file{}, _file_fd{}, _file_map{}, _header{}, _pheaders{}, _sheaders{}
		{ /* NOP */ }

	elf_t(fs::path file, bool readonly = true) noexcept :
		_file{file}, _file_fd{_file.c_str(), O_RDONLY},
		_file_map{_file_fd.map(PROT_READ)},
		_header{_file_map.at<ehdr_t>(0)}, _pheaders{}, _sheaders{} {

		if(_header.phnum() > 0)
			_pheaders = {&_file_map.at<phdr_t>(_header.phoff()), _header.phnum()};


		if(_header.shnum() > 0)
			_sheaders = {&_file_map.at<shdr_t>(_header.shoff()), _header.shnum()};

		/* Map the string table */
		_strtbl = &_file_map.at<char>((_sheaders[_header.shstrndx()]).offset());

	}

	bool is_valid() const noexcept { return _header.ident().magic().is_valid(); }

	void header(const ehdr_t header) noexcept { _header = header; }
	ehdr_t header() const noexcept { return _header; }

	void pheaders(const set_t<phdr_t> pheaders) noexcept { _pheaders = pheaders; }
	set_t<phdr_t> pheaders() const noexcept { return _pheaders; }

	void sheaders(const set_t<shdr_t> sheaders) noexcept { _sheaders = sheaders; }
	set_t<shdr_t> sheaders() const noexcept { return _sheaders; }

	std::string section_name(const size_t index) const noexcept { return std::string(_strtbl + index); }

};
using elf32_t = elf_t<elf_types_32_t>;
using elf64_t = elf_t<elf_types_64_t>;

uint32_t elf_hash(const uint8_t name[]);


#endif /* __SNS_ELF_HH__ */
